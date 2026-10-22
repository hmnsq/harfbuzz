/*
 * Copyright © 2012  Google, Inc.
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Google Author(s): Behdad Esfahbod
 */

#اگر_غیر_تعریف_شدہ HB_سیٹ_DIGEST_HH
#تعریف_کرو HB_سیٹ_DIGEST_HH

#شامل "hb.hh"
#شامل "hb-machinery.hh"

/*
 * The set-digests implement "filters" that support "approximate
 * member query".  Conceptually these are like Bloom Filter and
 * Quotient Filter, however, much smaller, faster, and designed
 * to fit the requirements of our uses for glyph coverage queries.
 *
 * Our filters are highly accurate if the lookup covers fairly local
 * set of glyphs, but fully flooded and ineffective if coverage is
 * all over the place.
 *
 * The way these are used is that the filter is first populated by
 * a lookup's or subtable's Coverage table(s), and then when we
 * want to apply the lookup or subtable to a glyph, before trying
 * to apply, we ask the filter if the glyph may be covered. If it's
 * not, we return early.  We can also match a digest against another
 * digest.
 *
 * We use these filters at three levels:
 *   - If the digest for all the glyphs in the buffer as a whole
 *     does not match the digest for the lookup, skip the lookup.
 *   - For each glyph, if it doesn't match the lookup digest,
 *     skip it.
 *   - For each glyph, if it doesn't match the subtable digest,
 *     skip it.
 *
 * The filter we use is a combination of three bits-pattern
 * filters. A bits-pattern filter checks a number of bits (5 or 6)
 * of the input number (glyph-id in most cases) and checks whether
 * its pattern is amongst the patterns of any of the accepted values.
 * The accepted patterns are represented as a "long" integer. Each
 * check is done using four bitwise operations only.
 */

ساکن constexpr غیرعلامتی hb_سیٹ_digest_shifts[] = {4, 0, 6};

ساخت hb_سیٹ_digest_t
{
  // No science in these. Intuition and testing only.
  استعمال نقاب_t = uint64_t;

  ساکن constexpr غیرعلامتی n = ARRAY_لمبائی_CONST (hb_سیٹ_digest_shifts);
  ساکن constexpr غیرعلامتی نقاب_بائٹیں = حجم (نقاب_t);
  ساکن constexpr غیرعلامتی نقاب_بٹ = حجم (نقاب_t) * 8;
  ساکن constexpr hb_codepoint_t mb1 = نقاب_بٹ - 1;
  ساکن constexpr نقاب_t one = 1;
  ساکن constexpr نقاب_t all = (نقاب_t) -1;

  خالی init ()
  { برائے (غیرعلامتی i = 0; i < n; i++) masks[i] = 0; }

  خالی clear () { init (); }

  ساکن hb_سیٹ_digest_t بھرا ()
  {
    hb_سیٹ_digest_t d;
    برائے (غیرعلامتی i = 0; i < n; i++) d.masks[i] = all;
    واپس d;
  }

  خالی union_ (ثابت hb_سیٹ_digest_t &o)
  { برائے (غیرعلامتی i = 0; i < n; i++) masks[i] |= o.masks[i]; }

  منطقی جوڑو_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    منطقی ret;

    ret = جھوٹ;
    برائے (غیرعلامتی i = 0; i < n; i++)
      اگر (masks[i] != all)
	ret = سچ;
    اگر (!ret) واپس جھوٹ;

    ret = جھوٹ;
    برائے (غیرعلامتی i = 0; i < n; i++)
    {
      نقاب_t سرکاؤ = hb_سیٹ_digest_shifts[i];
      اگر ((b >> سرکاؤ) - (a >> سرکاؤ) >= mb1)
	masks[i] = all;
      ورنہ
      {
	نقاب_t ma = one << ((a >> سرکاؤ) & mb1);
	نقاب_t mb = one << ((b >> سرکاؤ) & mb1);
	masks[i] |= mb + (mb - ma) - (mb < ma);
	ret = سچ;
      }
    }
    واپس ret;
  }

  سانچہ <قسم_نام T>
  خالی جوڑو_array (ثابت T *array, غیرعلامتی صحیح گنتی, غیرعلامتی صحیح stride=حجم(T))
  {
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      جوڑو (*array);
      array = &Struct_At_انحراف_Unaligned<T> ((ثابت خالی *) array, stride);
    }
  }
  سانچہ <قسم_نام T>
  خالی جوڑو_array (ثابت hb_array_t<ثابت T>& arr) { جوڑو_array (&arr, arr.len ()); }
  سانچہ <قسم_نام T>
  منطقی جوڑو_sorted_array (ثابت T *array, غیرعلامتی صحیح گنتی, غیرعلامتی صحیح stride=حجم(T))
  {
    جوڑو_array (array, گنتی, stride);
    واپس سچ;
  }
  سانچہ <قسم_نام T>
  منطقی جوڑو_sorted_array (ثابت hb_sorted_array_t<ثابت T>& arr) { واپس جوڑو_sorted_array (&arr, arr.len ()); }

  منطقی عامل [] (hb_codepoint_t g) ثابت
  { واپس may_have (g); }


  خالی جوڑو (hb_codepoint_t g)
  {
    برائے (غیرعلامتی i = 0; i < n; i++)
      masks[i] |= one << ((g >> hb_سیٹ_digest_shifts[i]) & mb1);
  }

  HB_ALWAYS_INLINE
  منطقی may_have (hb_codepoint_t g) ثابت
  {
    برائے (غیرعلامتی i = 0; i < n; i++)
      اگر (!(masks[i] & (one << ((g >> hb_سیٹ_digest_shifts[i]) & mb1))))
	واپس جھوٹ;
    واپس سچ;
  }

  منطقی may_intersect (ثابت hb_سیٹ_digest_t &o) ثابت
  {
    برائے (غیرعلامتی i = 0; i < n; i++)
      اگر (!(masks[i] & o.masks[i]))
	واپس جھوٹ;
    واپس سچ;
  }

  نجی:

  نقاب_t masks[n] = {};
};


#اگر_ختم /* HB_SET_DIGEST_HH */

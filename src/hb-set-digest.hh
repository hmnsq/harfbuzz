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

#اگر_تعریف_نشده HB_مجموعه_DIGEST_HH
#تعریف_کن HB_مجموعه_DIGEST_HH

#ضمن "hb.hh"
#ضمن "hb-machinery.hh"

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

ساكن constexpr بیعلامت hb_مجموعه_digest_shifts[] = {4, 0, 6};

بنية hb_مجموعه_digest_t
{
  // No science in these. Intuition and testing only.
  استفاده ماسک_t = uint64_t;

  ساكن constexpr بیعلامت n = ARRAY_درازی_CONST (hb_مجموعه_digest_shifts);
  ساكن constexpr بیعلامت ماسک_بایتها = حجم (ماسک_t);
  ساكن constexpr بیعلامت ماسک_بیتها = حجم (ماسک_t) * 8;
  ساكن constexpr hb_codepoint_t mb1 = ماسک_بیتها - 1;
  ساكن constexpr ماسک_t one = 1;
  ساكن constexpr ماسک_t all = (ماسک_t) -1;

  فراغ init ()
  { لكل (بیعلامت i = 0; i < n; i++) masks[i] = 0; }

  فراغ clear () { init (); }

  ساكن hb_مجموعه_digest_t پر ()
  {
    hb_مجموعه_digest_t d;
    لكل (بیعلامت i = 0; i < n; i++) d.masks[i] = all;
    أرجع d;
  }

  فراغ union_ (ثابت hb_مجموعه_digest_t &o)
  { لكل (بیعلامت i = 0; i < n; i++) masks[i] |= o.masks[i]; }

  منطقي جمع_کن_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    منطقي ret;

    ret = خطأ;
    لكل (بیعلامت i = 0; i < n; i++)
      إذا (masks[i] != all)
	ret = درست;
    إذا (!ret) أرجع خطأ;

    ret = خطأ;
    لكل (بیعلامت i = 0; i < n; i++)
    {
      ماسک_t لغزش = hb_مجموعه_digest_shifts[i];
      إذا ((b >> لغزش) - (a >> لغزش) >= mb1)
	masks[i] = all;
      وإلا
      {
	ماسک_t ma = one << ((a >> لغزش) & mb1);
	ماسک_t mb = one << ((b >> لغزش) & mb1);
	masks[i] |= mb + (mb - ma) - (mb < ma);
	ret = درست;
      }
    }
    أرجع ret;
  }

  قالب <نام_نوع T>
  فراغ جمع_کن_array (ثابت T *array, بیعلامت صحيح شمار, بیعلامت صحيح stride=حجم(T))
  {
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      جمع_کن (*array);
      array = &Struct_At_جابه_جایی_Unaligned<T> ((ثابت فراغ *) array, stride);
    }
  }
  قالب <نام_نوع T>
  فراغ جمع_کن_array (ثابت hb_array_t<ثابت T>& arr) { جمع_کن_array (&arr, arr.len ()); }
  قالب <نام_نوع T>
  منطقي جمع_کن_sorted_array (ثابت T *array, بیعلامت صحيح شمار, بیعلامت صحيح stride=حجم(T))
  {
    جمع_کن_array (array, شمار, stride);
    أرجع درست;
  }
  قالب <نام_نوع T>
  منطقي جمع_کن_sorted_array (ثابت hb_sorted_array_t<ثابت T>& arr) { أرجع جمع_کن_sorted_array (&arr, arr.len ()); }

  منطقي عملگر [] (hb_codepoint_t g) ثابت
  { أرجع may_have (g); }


  فراغ جمع_کن (hb_codepoint_t g)
  {
    لكل (بیعلامت i = 0; i < n; i++)
      masks[i] |= one << ((g >> hb_مجموعه_digest_shifts[i]) & mb1);
  }

  HB_ALWAYS_INLINE
  منطقي may_have (hb_codepoint_t g) ثابت
  {
    لكل (بیعلامت i = 0; i < n; i++)
      إذا (!(masks[i] & (one << ((g >> hb_مجموعه_digest_shifts[i]) & mb1))))
	أرجع خطأ;
    أرجع درست;
  }

  منطقي may_intersect (ثابت hb_مجموعه_digest_t &o) ثابت
  {
    لكل (بیعلامت i = 0; i < n; i++)
      إذا (!(masks[i] & o.masks[i]))
	أرجع خطأ;
    أرجع درست;
  }

  خاص:

  ماسک_t masks[n] = {};
};


#پایان_اگر /* HB_SET_DIGEST_HH */

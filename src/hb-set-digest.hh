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

#که_نه_تعریف_شوی HB_سېټ_DIGEST_HH
#تعریف_کړه HB_سېټ_DIGEST_HH

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

ساكن constexpr بینښه hb_سېټ_digest_shifts[] = {4, 0, 6};

بنية hb_سېټ_digest_t
{
  // No science in these. Intuition and testing only.
  کارول ماسک_t = uint64_t;

  ساكن constexpr بینښه n = ARRAY_اوږدتیا_CONST (hb_سېټ_digest_shifts);
  ساكن constexpr بینښه ماسک_بایټونه = حجم (ماسک_t);
  ساكن constexpr بینښه ماسک_بټونه = حجم (ماسک_t) * 8;
  ساكن constexpr hb_codepoint_t mb1 = ماسک_بټونه - 1;
  ساكن constexpr ماسک_t one = 1;
  ساكن constexpr ماسک_t all = (ماسک_t) -1;

  فراغ init ()
  { لكل (بینښه i = 0; i < n; i++) masks[i] = 0; }

  فراغ clear () { init (); }

  ساكن hb_سېټ_digest_t ډک ()
  {
    hb_سېټ_digest_t d;
    لكل (بینښه i = 0; i < n; i++) d.masks[i] = all;
    أرجع d;
  }

  فراغ union_ (ثابت hb_سېټ_digest_t &o)
  { لكل (بینښه i = 0; i < n; i++) masks[i] |= o.masks[i]; }

  منطقي جمع_کړه_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    منطقي ret;

    ret = خطأ;
    لكل (بینښه i = 0; i < n; i++)
      إذا (masks[i] != all)
	ret = ریښتیا;
    إذا (!ret) أرجع خطأ;

    ret = خطأ;
    لكل (بینښه i = 0; i < n; i++)
    {
      ماسک_t ښويدنه = hb_سېټ_digest_shifts[i];
      إذا ((b >> ښويدنه) - (a >> ښويدنه) >= mb1)
	masks[i] = all;
      وإلا
      {
	ماسک_t ma = one << ((a >> ښويدنه) & mb1);
	ماسک_t mb = one << ((b >> ښويدنه) & mb1);
	masks[i] |= mb + (mb - ma) - (mb < ma);
	ret = ریښتیا;
      }
    }
    أرجع ret;
  }

  کينډۍ <ډول_نوم T>
  فراغ جمع_کړه_array (ثابت T *array, بینښه صحيح شمېر, بینښه صحيح stride=حجم(T))
  {
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      جمع_کړه (*array);
      array = &Struct_At_دننه_واټن_Unaligned<T> ((ثابت فراغ *) array, stride);
    }
  }
  کينډۍ <ډول_نوم T>
  فراغ جمع_کړه_array (ثابت hb_array_t<ثابت T>& arr) { جمع_کړه_array (&arr, arr.len ()); }
  کينډۍ <ډول_نوم T>
  منطقي جمع_کړه_sorted_array (ثابت T *array, بینښه صحيح شمېر, بینښه صحيح stride=حجم(T))
  {
    جمع_کړه_array (array, شمېر, stride);
    أرجع ریښتیا;
  }
  کينډۍ <ډول_نوم T>
  منطقي جمع_کړه_sorted_array (ثابت hb_sorted_array_t<ثابت T>& arr) { أرجع جمع_کړه_sorted_array (&arr, arr.len ()); }

  منطقي چلوونکی [] (hb_codepoint_t g) ثابت
  { أرجع may_have (g); }


  فراغ جمع_کړه (hb_codepoint_t g)
  {
    لكل (بینښه i = 0; i < n; i++)
      masks[i] |= one << ((g >> hb_سېټ_digest_shifts[i]) & mb1);
  }

  HB_ALWAYS_INLINE
  منطقي may_have (hb_codepoint_t g) ثابت
  {
    لكل (بینښه i = 0; i < n; i++)
      إذا (!(masks[i] & (one << ((g >> hb_سېټ_digest_shifts[i]) & mb1))))
	أرجع خطأ;
    أرجع ریښتیا;
  }

  منطقي may_intersect (ثابت hb_سېټ_digest_t &o) ثابت
  {
    لكل (بینښه i = 0; i < n; i++)
      إذا (!(masks[i] & o.masks[i]))
	أرجع خطأ;
    أرجع ریښتیا;
  }

  خاص:

  ماسک_t masks[n] = {};
};


#د_که_پای /* HB_SET_DIGEST_HH */

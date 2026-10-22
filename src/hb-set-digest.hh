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

#ifndef HB_SET_DIGEST_HH
#define HB_SET_DIGEST_HH

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-machinery.hh"

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

স্থির constexpr unsigned hb_set_digest_shifts[] = {4, 0, 6};

কাঠামো hb_set_digest_t
{
  // No science in these. Intuition and testing only.
  using mask_t = uint64_t;

  স্থির constexpr unsigned n = ARRAY_দৈর্ঘ্য_CONST (hb_set_digest_shifts);
  স্থির constexpr unsigned mask_bytes = আকার (mask_t);
  স্থির constexpr unsigned mask_bits = আকার (mask_t) * 8;
  স্থির constexpr hb_codepoint_t mb1 = mask_bits - 1;
  স্থির constexpr mask_t one = 1;
  স্থির constexpr mask_t all = (mask_t) -1;

  শূন্য init ()
  { জন্য (unsigned i = 0; i < n; i++) masks[i] = 0; }

  শূন্য clear () { init (); }

  স্থির hb_set_digest_t full ()
  {
    hb_set_digest_t d;
    জন্য (unsigned i = 0; i < n; i++) d.masks[i] = all;
    ফেরত d;
  }

  শূন্য union_ (ধ্রুবক hb_set_digest_t &o)
  { জন্য (unsigned i = 0; i < n; i++) masks[i] |= o.masks[i]; }

  যৌক্তিক add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    যৌক্তিক ret;

    ret = মিথ্যা;
    জন্য (unsigned i = 0; i < n; i++)
      যদি (masks[i] != all)
	ret = সত্য;
    যদি (!ret) ফেরত মিথ্যা;

    ret = মিথ্যা;
    জন্য (unsigned i = 0; i < n; i++)
    {
      mask_t shift = hb_set_digest_shifts[i];
      যদি ((b >> shift) - (a >> shift) >= mb1)
	masks[i] = all;
      অন্যথা
      {
	mask_t ma = one << ((a >> shift) & mb1);
	mask_t mb = one << ((b >> shift) & mb1);
	masks[i] |= mb + (mb - ma) - (mb < ma);
	ret = সত্য;
      }
    }
    ফেরত ret;
  }

  template <typename T>
  শূন্য add_array (ধ্রুবক T *array, unsigned পূর্ণসংখ্যা গুনতি, unsigned পূর্ণসংখ্যা stride=আকার(T))
  {
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      add (*array);
      array = &StructAtOffsetUnaligned<T> ((ধ্রুবক শূন্য *) array, stride);
    }
  }
  template <typename T>
  শূন্য add_array (ধ্রুবক hb_array_t<ধ্রুবক T>& arr) { add_array (&arr, arr.len ()); }
  template <typename T>
  যৌক্তিক add_sorted_array (ধ্রুবক T *array, unsigned পূর্ণসংখ্যা গুনতি, unsigned পূর্ণসংখ্যা stride=আকার(T))
  {
    add_array (array, গুনতি, stride);
    ফেরত সত্য;
  }
  template <typename T>
  যৌক্তিক add_sorted_array (ধ্রুবক hb_sorted_array_t<ধ্রুবক T>& arr) { ফেরত add_sorted_array (&arr, arr.len ()); }

  যৌক্তিক operator [] (hb_codepoint_t g) ধ্রুবক
  { ফেরত may_have (g); }


  শূন্য add (hb_codepoint_t g)
  {
    জন্য (unsigned i = 0; i < n; i++)
      masks[i] |= one << ((g >> hb_set_digest_shifts[i]) & mb1);
  }

  HB_ALWAYS_INLINE
  যৌক্তিক may_have (hb_codepoint_t g) ধ্রুবক
  {
    জন্য (unsigned i = 0; i < n; i++)
      যদি (!(masks[i] & (one << ((g >> hb_set_digest_shifts[i]) & mb1))))
	ফেরত মিথ্যা;
    ফেরত সত্য;
  }

  যৌক্তিক may_intersect (ধ্রুবক hb_set_digest_t &o) ধ্রুবক
  {
    জন্য (unsigned i = 0; i < n; i++)
      যদি (!(masks[i] & o.masks[i]))
	ফেরত মিথ্যা;
    ফেরত সত্য;
  }

  ব্যক্তিগত:

  mask_t masks[n] = {};
};


#endif /* HB_SET_DIGEST_HH */

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

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-machinery.hh"

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

ಸ್ಥಿರ constexpr unsigned hb_set_digest_shifts[] = {4, 0, 6};

ಸಂರಚನಾ hb_set_digest_t
{
  // No science in these. Intuition and testing only.
  using mask_t = uint64_t;

  ಸ್ಥಿರ constexpr unsigned n = ARRAY_LENGTH_CONST (hb_set_digest_shifts);
  ಸ್ಥಿರ constexpr unsigned mask_bytes = ಪರಿಮಾಣ (mask_t);
  ಸ್ಥಿರ constexpr unsigned mask_bits = ಪರಿಮಾಣ (mask_t) * 8;
  ಸ್ಥಿರ constexpr hb_codepoint_t mb1 = mask_bits - 1;
  ಸ್ಥಿರ constexpr mask_t one = 1;
  ಸ್ಥಿರ constexpr mask_t all = (mask_t) -1;

  ಶೂನ್ಯ init ()
  { ಪ್ರತಿ (unsigned i = 0; i < n; i++) masks[i] = 0; }

  ಶೂನ್ಯ clear () { init (); }

  ಸ್ಥಿರ hb_set_digest_t full ()
  {
    hb_set_digest_t d;
    ಪ್ರತಿ (unsigned i = 0; i < n; i++) d.masks[i] = all;
    ಪ್ರತ್ಯಾಗಮ d;
  }

  ಶೂನ್ಯ union_ (ಧ್ರುವ hb_set_digest_t &o)
  { ಪ್ರತಿ (unsigned i = 0; i < n; i++) masks[i] |= o.masks[i]; }

  ತಾರ್ಕಿಕ add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    ತಾರ್ಕಿಕ ret;

    ret = ಅಸತ್ಯ;
    ಪ್ರತಿ (unsigned i = 0; i < n; i++)
      ಯದಿ (masks[i] != all)
	ret = ಸತ್ಯ;
    ಯದಿ (!ret) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ret = ಅಸತ್ಯ;
    ಪ್ರತಿ (unsigned i = 0; i < n; i++)
    {
      mask_t shift = hb_set_digest_shifts[i];
      ಯದಿ ((b >> shift) - (a >> shift) >= mb1)
	masks[i] = all;
      ಅನ್ಯಥಾ
      {
	mask_t ma = one << ((a >> shift) & mb1);
	mask_t mb = one << ((b >> shift) & mb1);
	masks[i] |= mb + (mb - ma) - (mb < ma);
	ret = ಸತ್ಯ;
      }
    }
    ಪ್ರತ್ಯಾಗಮ ret;
  }

  template <typename T>
  ಶೂನ್ಯ add_array (ಧ್ರುವ T *array, unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, unsigned ಪೂರ್ಣಾಙ್ಕ stride=ಪರಿಮಾಣ(T))
  {
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      add (*array);
      array = &StructAtOffsetUnaligned<T> ((ಧ್ರುವ ಶೂನ್ಯ *) array, stride);
    }
  }
  template <typename T>
  ಶೂನ್ಯ add_array (ಧ್ರುವ hb_array_t<ಧ್ರುವ T>& arr) { add_array (&arr, arr.len ()); }
  template <typename T>
  ತಾರ್ಕಿಕ add_sorted_array (ಧ್ರುವ T *array, unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, unsigned ಪೂರ್ಣಾಙ್ಕ stride=ಪರಿಮಾಣ(T))
  {
    add_array (array, ಎಣಿಕೆ, stride);
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }
  template <typename T>
  ತಾರ್ಕಿಕ add_sorted_array (ಧ್ರುವ hb_sorted_array_t<ಧ್ರುವ T>& arr) { ಪ್ರತ್ಯಾಗಮ add_sorted_array (&arr, arr.len ()); }

  ತಾರ್ಕಿಕ operator [] (hb_codepoint_t g) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ may_have (g); }


  ಶೂನ್ಯ add (hb_codepoint_t g)
  {
    ಪ್ರತಿ (unsigned i = 0; i < n; i++)
      masks[i] |= one << ((g >> hb_set_digest_shifts[i]) & mb1);
  }

  HB_ALWAYS_INLINE
  ತಾರ್ಕಿಕ may_have (hb_codepoint_t g) ಧ್ರುವ
  {
    ಪ್ರತಿ (unsigned i = 0; i < n; i++)
      ಯದಿ (!(masks[i] & (one << ((g >> hb_set_digest_shifts[i]) & mb1))))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ may_intersect (ಧ್ರುವ hb_set_digest_t &o) ಧ್ರುವ
  {
    ಪ್ರತಿ (unsigned i = 0; i < n; i++)
      ಯದಿ (!(masks[i] & o.masks[i]))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ಗುಪ್ತ:

  mask_t masks[n] = {};
};


#endif /* HB_SET_DIGEST_HH */

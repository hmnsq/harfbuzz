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

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-machinery.hh"

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

ସ୍ଥିର constexpr unsigned hb_set_digest_shifts[] = {4, 0, 6};

ସଂରଚନା hb_set_digest_t
{
  // No science in these. Intuition and testing only.
  using mask_t = uint64_t;

  ସ୍ଥିର constexpr unsigned n = ARRAY_LENGTH_CONST (hb_set_digest_shifts);
  ସ୍ଥିର constexpr unsigned mask_bytes = ଆକାର (mask_t);
  ସ୍ଥିର constexpr unsigned mask_bits = ଆକାର (mask_t) * 8;
  ସ୍ଥିର constexpr hb_codepoint_t mb1 = mask_bits - 1;
  ସ୍ଥିର constexpr mask_t one = 1;
  ସ୍ଥିର constexpr mask_t all = (mask_t) -1;

  ଶୂନ୍ୟ init ()
  { ପାଇଁ (unsigned i = 0; i < n; i++) masks[i] = 0; }

  ଶୂନ୍ୟ clear () { init (); }

  ସ୍ଥିର hb_set_digest_t full ()
  {
    hb_set_digest_t d;
    ପାଇଁ (unsigned i = 0; i < n; i++) d.masks[i] = all;
    ଫେରନ୍ତୁ d;
  }

  ଶୂନ୍ୟ union_ (ଅଚଳ hb_set_digest_t &o)
  { ପାଇଁ (unsigned i = 0; i < n; i++) masks[i] |= o.masks[i]; }

  ତାର୍କିକ add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    ତାର୍କିକ ret;

    ret = ମିଥ୍ୟା;
    ପାଇଁ (unsigned i = 0; i < n; i++)
      ଯଦି (masks[i] != all)
	ret = ସତ୍ୟ;
    ଯଦି (!ret) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ret = ମିଥ୍ୟା;
    ପାଇଁ (unsigned i = 0; i < n; i++)
    {
      mask_t shift = hb_set_digest_shifts[i];
      ଯଦି ((b >> shift) - (a >> shift) >= mb1)
	masks[i] = all;
      ଅନ୍ୟଥା
      {
	mask_t ma = one << ((a >> shift) & mb1);
	mask_t mb = one << ((b >> shift) & mb1);
	masks[i] |= mb + (mb - ma) - (mb < ma);
	ret = ସତ୍ୟ;
      }
    }
    ଫେରନ୍ତୁ ret;
  }

  template <typename T>
  ଶୂନ୍ୟ add_array (ଅଚଳ T *array, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride=ଆକାର(T))
  {
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      add (*array);
      array = &StructAtOffsetUnaligned<T> ((ଅଚଳ ଶୂନ୍ୟ *) array, stride);
    }
  }
  template <typename T>
  ଶୂନ୍ୟ add_array (ଅଚଳ hb_array_t<ଅଚଳ T>& arr) { add_array (&arr, arr.len ()); }
  template <typename T>
  ତାର୍କିକ add_sorted_array (ଅଚଳ T *array, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride=ଆକାର(T))
  {
    add_array (array, ଗଣନା_2, stride);
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }
  template <typename T>
  ତାର୍କିକ add_sorted_array (ଅଚଳ hb_sorted_array_t<ଅଚଳ T>& arr) { ଫେରନ୍ତୁ add_sorted_array (&arr, arr.len ()); }

  ତାର୍କିକ operator [] (hb_codepoint_t g) ଅଚଳ
  { ଫେରନ୍ତୁ may_have (g); }


  ଶୂନ୍ୟ add (hb_codepoint_t g)
  {
    ପାଇଁ (unsigned i = 0; i < n; i++)
      masks[i] |= one << ((g >> hb_set_digest_shifts[i]) & mb1);
  }

  HB_ALWAYS_INLINE
  ତାର୍କିକ may_have (hb_codepoint_t g) ଅଚଳ
  {
    ପାଇଁ (unsigned i = 0; i < n; i++)
      ଯଦି (!(masks[i] & (one << ((g >> hb_set_digest_shifts[i]) & mb1))))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ତାର୍କିକ may_intersect (ଅଚଳ hb_set_digest_t &o) ଅଚଳ
  {
    ପାଇଁ (unsigned i = 0; i < n; i++)
      ଯଦି (!(masks[i] & o.masks[i]))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ବ୍ୟକ୍ତିଗତ:

  mask_t masks[n] = {};
};


#endif /* HB_SET_DIGEST_HH */

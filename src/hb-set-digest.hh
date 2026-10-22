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

#அந்தர்பாவ "hb.hh"
#அந்தர்பாவ "hb-machinery.hh"

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

நிலையான constexpr unsigned hb_set_digest_shifts[] = {4, 0, 6};

கட்டமைப்பு hb_set_digest_t
{
  // No science in these. Intuition and testing only.
  using mask_t = uint64_t;

  நிலையான constexpr unsigned n = ARRAY_LENGTH_CONST (hb_set_digest_shifts);
  நிலையான constexpr unsigned mask_bytes = அளவு (mask_t);
  நிலையான constexpr unsigned mask_bits = அளவு (mask_t) * 8;
  நிலையான constexpr hb_codepoint_t mb1 = mask_bits - 1;
  நிலையான constexpr mask_t one = 1;
  நிலையான constexpr mask_t all = (mask_t) -1;

  வெறுமை init ()
  { க்கு (unsigned i = 0; i < n; i++) masks[i] = 0; }

  வெறுமை clear () { init (); }

  நிலையான hb_set_digest_t full ()
  {
    hb_set_digest_t d;
    க்கு (unsigned i = 0; i < n; i++) d.masks[i] = all;
    திரும்பு d;
  }

  வெறுமை union_ (மாறிலி hb_set_digest_t &o)
  { க்கு (unsigned i = 0; i < n; i++) masks[i] |= o.masks[i]; }

  தருக்க add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    தருக்க ret;

    ret = பொய்;
    க்கு (unsigned i = 0; i < n; i++)
      எனில் (masks[i] != all)
	ret = உண்மை;
    எனில் (!ret) திரும்பு பொய்;

    ret = பொய்;
    க்கு (unsigned i = 0; i < n; i++)
    {
      mask_t shift = hb_set_digest_shifts[i];
      எனில் ((b >> shift) - (a >> shift) >= mb1)
	masks[i] = all;
      இல்லையெனில்
      {
	mask_t ma = one << ((a >> shift) & mb1);
	mask_t mb = one << ((b >> shift) & mb1);
	masks[i] |= mb + (mb - ma) - (mb < ma);
	ret = உண்மை;
      }
    }
    திரும்பு ret;
  }

  template <typename T>
  வெறுமை add_array (மாறிலி T *array, unsigned முழு மொத்தம், unsigned முழு stride=அளவு(T))
  {
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      add (*array);
      array = &StructAtOffsetUnaligned<T> ((மாறிலி வெறுமை *) array, stride);
    }
  }
  template <typename T>
  வெறுமை add_array (மாறிலி hb_array_t<மாறிலி T>& arr) { add_array (&arr, arr.len ()); }
  template <typename T>
  தருக்க add_sorted_array (மாறிலி T *array, unsigned முழு மொத்தம், unsigned முழு stride=அளவு(T))
  {
    add_array (array, மொத்தம், stride);
    திரும்பு உண்மை;
  }
  template <typename T>
  தருக்க add_sorted_array (மாறிலி hb_sorted_array_t<மாறிலி T>& arr) { திரும்பு add_sorted_array (&arr, arr.len ()); }

  தருக்க operator [] (hb_codepoint_t g) மாறிலி
  { திரும்பு may_have (g); }


  வெறுமை add (hb_codepoint_t g)
  {
    க்கு (unsigned i = 0; i < n; i++)
      masks[i] |= one << ((g >> hb_set_digest_shifts[i]) & mb1);
  }

  HB_ALWAYS_INLINE
  தருக்க may_have (hb_codepoint_t g) மாறிலி
  {
    க்கு (unsigned i = 0; i < n; i++)
      எனில் (!(masks[i] & (one << ((g >> hb_set_digest_shifts[i]) & mb1))))
	திரும்பு பொய்;
    திரும்பு உண்மை;
  }

  தருக்க may_intersect (மாறிலி hb_set_digest_t &o) மாறிலி
  {
    க்கு (unsigned i = 0; i < n; i++)
      எனில் (!(masks[i] & o.masks[i]))
	திரும்பு பொய்;
    திரும்பு உண்மை;
  }

  தனிப்பட்ட:

  mask_t masks[n] = {};
};


#endif /* HB_SET_DIGEST_HH */

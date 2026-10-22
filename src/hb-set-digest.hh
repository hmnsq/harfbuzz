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

#અન્તર્ભાવ "hb.hh"
#અન્તર્ભાવ "hb-machinery.hh"

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

સ્થિર constexpr unsigned hb_set_digest_shifts[] = {4, 0, 6};

સંરચના hb_set_digest_t
{
  // No science in these. Intuition and testing only.
  using mask_t = uint64_t;

  સ્થિર constexpr unsigned n = ARRAY_LENGTH_CONST (hb_set_digest_shifts);
  સ્થિર constexpr unsigned mask_bytes = કદ (mask_t);
  સ્થિર constexpr unsigned mask_bits = કદ (mask_t) * 8;
  સ્થિર constexpr hb_codepoint_t mb1 = mask_bits - 1;
  સ્થિર constexpr mask_t one = 1;
  સ્થિર constexpr mask_t all = (mask_t) -1;

  શૂન્ય init ()
  { માટે (unsigned i = 0; i < n; i++) masks[i] = 0; }

  શૂન્ય clear () { init (); }

  સ્થિર hb_set_digest_t full ()
  {
    hb_set_digest_t d;
    માટે (unsigned i = 0; i < n; i++) d.masks[i] = all;
    પાછા d;
  }

  શૂન્ય union_ (અચલ hb_set_digest_t &o)
  { માટે (unsigned i = 0; i < n; i++) masks[i] |= o.masks[i]; }

  તાર્કિક add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    તાર્કિક ret;

    ret = ખોટું;
    માટે (unsigned i = 0; i < n; i++)
      જો (masks[i] != all)
	ret = સાચું;
    જો (!ret) પાછા ખોટું;

    ret = ખોટું;
    માટે (unsigned i = 0; i < n; i++)
    {
      mask_t shift = hb_set_digest_shifts[i];
      જો ((b >> shift) - (a >> shift) >= mb1)
	masks[i] = all;
      નહીંતર
      {
	mask_t ma = one << ((a >> shift) & mb1);
	mask_t mb = one << ((b >> shift) & mb1);
	masks[i] |= mb + (mb - ma) - (mb < ma);
	ret = સાચું;
      }
    }
    પાછા ret;
  }

  template <typename T>
  શૂન્ય add_array (અચલ T *array, unsigned પૂર્ણાંક ગણતરી, unsigned પૂર્ણાંક stride=કદ(T))
  {
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      add (*array);
      array = &StructAtOffsetUnaligned<T> ((અચલ શૂન્ય *) array, stride);
    }
  }
  template <typename T>
  શૂન્ય add_array (અચલ hb_array_t<અચલ T>& arr) { add_array (&arr, arr.len ()); }
  template <typename T>
  તાર્કિક add_sorted_array (અચલ T *array, unsigned પૂર્ણાંક ગણતરી, unsigned પૂર્ણાંક stride=કદ(T))
  {
    add_array (array, ગણતરી, stride);
    પાછા સાચું;
  }
  template <typename T>
  તાર્કિક add_sorted_array (અચલ hb_sorted_array_t<અચલ T>& arr) { પાછા add_sorted_array (&arr, arr.len ()); }

  તાર્કિક operator [] (hb_codepoint_t g) અચલ
  { પાછા may_have (g); }


  શૂન્ય add (hb_codepoint_t g)
  {
    માટે (unsigned i = 0; i < n; i++)
      masks[i] |= one << ((g >> hb_set_digest_shifts[i]) & mb1);
  }

  HB_ALWAYS_INLINE
  તાર્કિક may_have (hb_codepoint_t g) અચલ
  {
    માટે (unsigned i = 0; i < n; i++)
      જો (!(masks[i] & (one << ((g >> hb_set_digest_shifts[i]) & mb1))))
	પાછા ખોટું;
    પાછા સાચું;
  }

  તાર્કિક may_intersect (અચલ hb_set_digest_t &o) અચલ
  {
    માટે (unsigned i = 0; i < n; i++)
      જો (!(masks[i] & o.masks[i]))
	પાછા ખોટું;
    પાછા સાચું;
  }

  ખાનગી:

  mask_t masks[n] = {};
};


#endif /* HB_SET_DIGEST_HH */

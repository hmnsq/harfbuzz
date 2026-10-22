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

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-machinery.hh"

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

സ്ഥിതം constexpr unsigned hb_set_digest_shifts[] = {4, 0, 6};

ഘടന hb_set_digest_t
{
  // No science in these. Intuition and testing only.
  using mask_t = uint64_t;

  സ്ഥിതം constexpr unsigned n = ARRAY_LENGTH_CONST (hb_set_digest_shifts);
  സ്ഥിതം constexpr unsigned mask_bytes = വലുപ്പം (mask_t);
  സ്ഥിതം constexpr unsigned mask_bits = വലുപ്പം (mask_t) * 8;
  സ്ഥിതം constexpr hb_codepoint_t mb1 = mask_bits - 1;
  സ്ഥിതം constexpr mask_t one = 1;
  സ്ഥിതം constexpr mask_t all = (mask_t) -1;

  ശൂന്യം init ()
  { വേണ്ടി (unsigned i = 0; i < n; i++) masks[i] = 0; }

  ശൂന്യം clear () { init (); }

  സ്ഥിതം hb_set_digest_t full ()
  {
    hb_set_digest_t d;
    വേണ്ടി (unsigned i = 0; i < n; i++) d.masks[i] = all;
    തിരികെ d;
  }

  ശൂന്യം union_ (സ്ഥിരം hb_set_digest_t &o)
  { വേണ്ടി (unsigned i = 0; i < n; i++) masks[i] |= o.masks[i]; }

  തർക്കം add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    തർക്കം ret;

    ret = അസത്യം;
    വേണ്ടി (unsigned i = 0; i < n; i++)
      എങ്കിൽ (masks[i] != all)
	ret = സത്യം;
    എങ്കിൽ (!ret) തിരികെ അസത്യം;

    ret = അസത്യം;
    വേണ്ടി (unsigned i = 0; i < n; i++)
    {
      mask_t shift = hb_set_digest_shifts[i];
      എങ്കിൽ ((b >> shift) - (a >> shift) >= mb1)
	masks[i] = all;
      അല്ലെങ്കിൽ
      {
	mask_t ma = one << ((a >> shift) & mb1);
	mask_t mb = one << ((b >> shift) & mb1);
	masks[i] |= mb + (mb - ma) - (mb < ma);
	ret = സത്യം;
      }
    }
    തിരികെ ret;
  }

  template <typename T>
  ശൂന്യം add_array (സ്ഥിരം T *array, unsigned പൂർണ്ണസംഖ്യ എണ്ണം, unsigned പൂർണ്ണസംഖ്യ stride=വലുപ്പം(T))
  {
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      add (*array);
      array = &StructAtOffsetUnaligned<T> ((സ്ഥിരം ശൂന്യം *) array, stride);
    }
  }
  template <typename T>
  ശൂന്യം add_array (സ്ഥിരം hb_array_t<സ്ഥിരം T>& arr) { add_array (&arr, arr.len ()); }
  template <typename T>
  തർക്കം add_sorted_array (സ്ഥിരം T *array, unsigned പൂർണ്ണസംഖ്യ എണ്ണം, unsigned പൂർണ്ണസംഖ്യ stride=വലുപ്പം(T))
  {
    add_array (array, എണ്ണം, stride);
    തിരികെ സത്യം;
  }
  template <typename T>
  തർക്കം add_sorted_array (സ്ഥിരം hb_sorted_array_t<സ്ഥിരം T>& arr) { തിരികെ add_sorted_array (&arr, arr.len ()); }

  തർക്കം operator [] (hb_codepoint_t g) സ്ഥിരം
  { തിരികെ may_have (g); }


  ശൂന്യം add (hb_codepoint_t g)
  {
    വേണ്ടി (unsigned i = 0; i < n; i++)
      masks[i] |= one << ((g >> hb_set_digest_shifts[i]) & mb1);
  }

  HB_ALWAYS_INLINE
  തർക്കം may_have (hb_codepoint_t g) സ്ഥിരം
  {
    വേണ്ടി (unsigned i = 0; i < n; i++)
      എങ്കിൽ (!(masks[i] & (one << ((g >> hb_set_digest_shifts[i]) & mb1))))
	തിരികെ അസത്യം;
    തിരികെ സത്യം;
  }

  തർക്കം may_intersect (സ്ഥിരം hb_set_digest_t &o) സ്ഥിരം
  {
    വേണ്ടി (unsigned i = 0; i < n; i++)
      എങ്കിൽ (!(masks[i] & o.masks[i]))
	തിരികെ അസത്യം;
    തിരികെ സത്യം;
  }

  സ്വകാര്യ:

  mask_t masks[n] = {};
};


#endif /* HB_SET_DIGEST_HH */

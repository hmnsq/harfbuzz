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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ਸੈੱਟ_DIGEST_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਸੈੱਟ_DIGEST_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-machinery.hh"

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

ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ hb_ਸੈੱਟ_digest_shifts[] = {4, 0, 6};

ਬਣਤਰ hb_ਸੈੱਟ_digest_t
{
  // No science in these. Intuition and testing only.
  ਵਰਤੋਂ ਨਕਾਬ_t = uint64_t;

  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ n = ARRAY_ਲੰਮਾਈ_CONST (hb_ਸੈੱਟ_digest_shifts);
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ ਨਕਾਬ_ਬਾਈਟਾਂ = ਆਕਾਰ (ਨਕਾਬ_t);
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ ਨਕਾਬ_ਬਿੱਟ = ਆਕਾਰ (ਨਕਾਬ_t) * 8;
  ਸਥਿਰ constexpr hb_codepoint_t mb1 = ਨਕਾਬ_ਬਿੱਟ - 1;
  ਸਥਿਰ constexpr ਨਕਾਬ_t one = 1;
  ਸਥਿਰ constexpr ਨਕਾਬ_t all = (ਨਕਾਬ_t) -1;

  ਖਾਲੀ init ()
  { ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < n; i++) masks[i] = 0; }

  ਖਾਲੀ clear () { init (); }

  ਸਥਿਰ hb_ਸੈੱਟ_digest_t ਭਰਿਆ ()
  {
    hb_ਸੈੱਟ_digest_t d;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < n; i++) d.masks[i] = all;
    ਵਾਪਸ d;
  }

  ਖਾਲੀ union_ (ਅਚਲ hb_ਸੈੱਟ_digest_t &o)
  { ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < n; i++) masks[i] |= o.masks[i]; }

  ਤਾਰਕਿਕ ਜੋੜੋ_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    ਤਾਰਕਿਕ ret;

    ret = ਝੂਠ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < n; i++)
      ਜੇ (masks[i] != all)
	ret = ਸੱਚ;
    ਜੇ (!ret) ਵਾਪਸ ਝੂਠ;

    ret = ਝੂਠ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < n; i++)
    {
      ਨਕਾਬ_t ਖਿਸਕਾਓ = hb_ਸੈੱਟ_digest_shifts[i];
      ਜੇ ((b >> ਖਿਸਕਾਓ) - (a >> ਖਿਸਕਾਓ) >= mb1)
	masks[i] = all;
      ਨਹੀਂਤਾਂ
      {
	ਨਕਾਬ_t ma = one << ((a >> ਖਿਸਕਾਓ) & mb1);
	ਨਕਾਬ_t mb = one << ((b >> ਖਿਸਕਾਓ) & mb1);
	masks[i] |= mb + (mb - ma) - (mb < ma);
	ret = ਸੱਚ;
      }
    }
    ਵਾਪਸ ret;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਖਾਲੀ ਜੋੜੋ_array (ਅਚਲ T *array, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ stride=ਆਕਾਰ(T))
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਜੋੜੋ (*array);
      array = &Struct_At_ਵਿੱਥ_Unaligned<T> ((ਅਚਲ ਖਾਲੀ *) array, stride);
    }
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਖਾਲੀ ਜੋੜੋ_array (ਅਚਲ hb_array_t<ਅਚਲ T>& arr) { ਜੋੜੋ_array (&arr, arr.len ()); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਤਾਰਕਿਕ ਜੋੜੋ_sorted_array (ਅਚਲ T *array, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ stride=ਆਕਾਰ(T))
  {
    ਜੋੜੋ_array (array, ਗਿਣਤੀ_2, stride);
    ਵਾਪਸ ਸੱਚ;
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਤਾਰਕਿਕ ਜੋੜੋ_sorted_array (ਅਚਲ hb_sorted_array_t<ਅਚਲ T>& arr) { ਵਾਪਸ ਜੋੜੋ_sorted_array (&arr, arr.len ()); }

  ਤਾਰਕਿਕ ਸੰਚਾਲਕ [] (hb_codepoint_t g) ਅਚਲ
  { ਵਾਪਸ may_have (g); }


  ਖਾਲੀ ਜੋੜੋ (hb_codepoint_t g)
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < n; i++)
      masks[i] |= one << ((g >> hb_ਸੈੱਟ_digest_shifts[i]) & mb1);
  }

  HB_ALWAYS_INLINE
  ਤਾਰਕਿਕ may_have (hb_codepoint_t g) ਅਚਲ
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < n; i++)
      ਜੇ (!(masks[i] & (one << ((g >> hb_ਸੈੱਟ_digest_shifts[i]) & mb1))))
	ਵਾਪਸ ਝੂਠ;
    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ may_intersect (ਅਚਲ hb_ਸੈੱਟ_digest_t &o) ਅਚਲ
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < n; i++)
      ਜੇ (!(masks[i] & o.masks[i]))
	ਵਾਪਸ ਝੂਠ;
    ਵਾਪਸ ਸੱਚ;
  }

  ਨਿਜੀ:

  ਨਕਾਬ_t masks[n] = {};
};


#ਜੇ_ਅੰਤ /* HB_SET_DIGEST_HH */

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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-machinery.hh"

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

स्थिर constexpr अचिन्हित hb_set_digest_shifts[] = {4, 0, 6};

संरचना hb_set_digest_t
{
  // No science in these. Intuition and testing only.
  using mask_t = uint64_t;

  स्थिर constexpr अचिन्हित n = ARRAY_LENGTH_CONST (hb_set_digest_shifts);
  स्थिर constexpr अचिन्हित mask_bytes = परिमाण (mask_t);
  स्थिर constexpr अचिन्हित mask_bits = परिमाण (mask_t) * 8;
  स्थिर constexpr hb_codepoint_t mb1 = mask_bits - 1;
  स्थिर constexpr mask_t one = 1;
  स्थिर constexpr mask_t all = (mask_t) -1;

  शून्य init ()
  { प्रति (अचिन्हित i = 0; i < n; i++) masks[i] = 0; }

  शून्य clear () { init (); }

  स्थिर hb_set_digest_t full ()
  {
    hb_set_digest_t d;
    प्रति (अचिन्हित i = 0; i < n; i++) d.masks[i] = all;
    प्रत्यागम d;
  }

  शून्य union_ (ध्रुव hb_set_digest_t &o)
  { प्रति (अचिन्हित i = 0; i < n; i++) masks[i] |= o.masks[i]; }

  तार्किक add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    तार्किक ret;

    ret = असत्य;
    प्रति (अचिन्हित i = 0; i < n; i++)
      यदि (masks[i] != all)
	ret = सत्य;
    यदि (!ret) प्रत्यागम असत्य;

    ret = असत्य;
    प्रति (अचिन्हित i = 0; i < n; i++)
    {
      mask_t shift = hb_set_digest_shifts[i];
      यदि ((b >> shift) - (a >> shift) >= mb1)
	masks[i] = all;
      अन्यथा
      {
	mask_t ma = one << ((a >> shift) & mb1);
	mask_t mb = one << ((b >> shift) & mb1);
	masks[i] |= mb + (mb - ma) - (mb < ma);
	ret = सत्य;
      }
    }
    प्रत्यागम ret;
  }

  ढाँचा <typename T>
  शून्य add_array (ध्रुव T *array, अचिन्हित पूर्णाङ्क गिनती, अचिन्हित पूर्णाङ्क stride=परिमाण(T))
  {
    प्रति (अचिन्हित पूर्णाङ्क i = 0; i < गिनती; i++)
    {
      add (*array);
      array = &StructAtOffsetUnaligned<T> ((ध्रुव शून्य *) array, stride);
    }
  }
  ढाँचा <typename T>
  शून्य add_array (ध्रुव hb_array_t<ध्रुव T>& arr) { add_array (&arr, arr.len ()); }
  ढाँचा <typename T>
  तार्किक add_sorted_array (ध्रुव T *array, अचिन्हित पूर्णाङ्क गिनती, अचिन्हित पूर्णाङ्क stride=परिमाण(T))
  {
    add_array (array, गिनती, stride);
    प्रत्यागम सत्य;
  }
  ढाँचा <typename T>
  तार्किक add_sorted_array (ध्रुव hb_sorted_array_t<ध्रुव T>& arr) { प्रत्यागम add_sorted_array (&arr, arr.len ()); }

  तार्किक चालक [] (hb_codepoint_t g) ध्रुव
  { प्रत्यागम may_have (g); }


  शून्य add (hb_codepoint_t g)
  {
    प्रति (अचिन्हित i = 0; i < n; i++)
      masks[i] |= one << ((g >> hb_set_digest_shifts[i]) & mb1);
  }

  HB_ALWAYS_INLINE
  तार्किक may_have (hb_codepoint_t g) ध्रुव
  {
    प्रति (अचिन्हित i = 0; i < n; i++)
      यदि (!(masks[i] & (one << ((g >> hb_set_digest_shifts[i]) & mb1))))
	प्रत्यागम असत्य;
    प्रत्यागम सत्य;
  }

  तार्किक may_intersect (ध्रुव hb_set_digest_t &o) ध्रुव
  {
    प्रति (अचिन्हित i = 0; i < n; i++)
      यदि (!(masks[i] & o.masks[i]))
	प्रत्यागम असत्य;
    प्रत्यागम सत्य;
  }

  गुप्त:

  mask_t masks[n] = {};
};


#endif /* HB_SET_DIGEST_HH */

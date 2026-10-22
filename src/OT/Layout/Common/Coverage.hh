/*
 * Copyright © 2007,2008,2009  Red Hat, Inc.
 * Copyright © 2010,2012  Google, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod, Garret Rieger
 */

#ifndef OT_LAYOUT_COMMON_COVERAGE_HH
#define OT_LAYOUT_COMMON_COVERAGE_HH

#ಅನ್ತರ್ಭಾವ "../../../hb-ot-dual.hh"
#ಅನ್ತರ್ಭಾವ "CoverageFormat1.hh"
#ಅನ್ತರ್ಭಾವ "CoverageFormat2.hh"

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ Common {

template<typename Iterator>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ Coverage_serialize (hb_serialize_context_t *c,
                                       Iterator it);

ಸಂರಚನಾ Coverage
{

  ರಕ್ಷಿತ:
  ಸಙ್ಘ {
  ಸಂರಚನಾ { HBUINT16 v; }        format;         /* Format identifier */
  CoverageFormat1_3<SmallTypes> format1;
  CoverageFormat2_4<SmallTypes> format2;
#ifndef HB_NO_BEYOND_64K
  CoverageFormat1_3<MediumTypes>format3;
  CoverageFormat2_4<MediumTypes>format4;
#endif
  } u;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_UNION (2, format.v);

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (!u.format.v.sanitize (c)) return_trace (ಅಸತ್ಯ);
    hb_barrier ();
    ವಿಕಲ್ಪ (u.format.v)
    {
    ಪಕ್ಷ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    ಪಕ್ಷ 2: hb_barrier (); return_trace (u.format2.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    ಪಕ್ಷ 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    ಸ್ವಭಾವ:return_trace (ಸತ್ಯ);
    }
  }

  /* Has interface. */
  unsigned operator [] (hb_codepoint_t k) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get (k); }
  ತಾರ್ಕಿಕ has (hb_codepoint_t k) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ (*ಏತತ್)[k] != NOT_COVERED; }
  /* Predicate. */
  ತಾರ್ಕಿಕ operator () (hb_codepoint_t k) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ has (k); }

  unsigned ಪೂರ್ಣಾಙ್ಕ get (hb_codepoint_t k) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get_coverage (k); }
  unsigned ಪೂರ್ಣಾಙ್ಕ get_coverage (hb_codepoint_t glyph_id) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.get_coverage (glyph_id);
    ಪಕ್ಷ 2: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format2.get_coverage (glyph_id);
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 3: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format3.get_coverage (glyph_id);
    ಪಕ್ಷ 4: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format4.get_coverage (glyph_id);
#endif
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ NOT_COVERED;
    }
  }
  unsigned ಪೂರ್ಣಾಙ್ಕ get_coverage (hb_codepoint_t glyph_id,
			     hb_ot_layout_mapping_cache_t *cache) ಧ್ರುವ
  {
    unsigned coverage;
    ಯದಿ (cache && cache->get (glyph_id, &coverage)) ಪ್ರತ್ಯಾಗಮ coverage < cache->MAX_ಮೌಲ್ಯ ? coverage : NOT_COVERED;
    coverage = get_coverage (glyph_id);
    ಯದಿ (cache) {
      ಯದಿ (coverage == NOT_COVERED)
	cache->set (glyph_id, cache->MAX_ಮೌಲ್ಯ);
      ಅನ್ಯಥಾ ಯದಿ (likely (coverage < cache->MAX_ಮೌಲ್ಯ))
	cache->set (glyph_id, coverage);
    }
    ಪ್ರತ್ಯಾಗಮ coverage;
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_coverage_ದ್ವಿಮಾನ (hb_codepoint_t glyph_id,
				    hb_ot_layout_ದ್ವಿಮಾನ_cache_t *cache) ಧ್ರುವ
  {
    unsigned coverage;
    ಯದಿ (cache && cache->get (glyph_id, &coverage)) ಪ್ರತ್ಯಾಗಮ coverage < cache->MAX_ಮೌಲ್ಯ ? coverage : NOT_COVERED;
    coverage = get_coverage (glyph_id);
    ಯದಿ (cache) {
      ಯದಿ (coverage == NOT_COVERED)
	cache->set (glyph_id, cache->MAX_ಮೌಲ್ಯ);
      ಅನ್ಯಥಾ
	cache->set (glyph_id, 0);
    }
    ಪ್ರತ್ಯಾಗಮ coverage;
  }

  unsigned get_population () ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.get_population ();
    ಪಕ್ಷ 2: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format2.get_population ();
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 3: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format3.get_population ();
    ಪಕ್ಷ 4: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format4.get_population ();
#endif
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ NOT_COVERED;
    }
  }

  template <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (ಏತತ್);
    ಯದಿ (unlikely (!c->extend_min (ಏತತ್))) return_trace (ಅಸತ್ಯ);

    unsigned ಎಣಿಕೆ = hb_len (+ glyphs);
    unsigned ಅಂಕೆ_ranges = 0;
    hb_codepoint_t last = (hb_codepoint_t) -2;
    hb_codepoint_t max = 0;
    ತಾರ್ಕಿಕ unsorted = ಅಸತ್ಯ;
    ಪ್ರತಿ (auto g: + glyphs)
    {
      ಯದಿ (last != (hb_codepoint_t) -2 && g < last)
	unsorted = ಸತ್ಯ;
      ಯದಿ (last + 1 != g)
	ಅಂಕೆ_ranges++;
      last = g;
      ಯದಿ (g > max) max = g;
    }
    u.format.v = !unsorted && ಎಣಿಕೆ <= ಅಂಕೆ_ranges * 3 ? 1 : 2;

#ifndef HB_NO_BEYOND_64K
    ಯದಿ (max > 0xFFFFu)
      u.format.v += 2;
    ಯದಿ (unlikely (max > 0xFFFFFFu))
#ಅನ್ಯಥಾ
    ಯದಿ (unlikely (max > 0xFFFFu))
#endif
    {
      c->check_success (ಅಸತ್ಯ, HB_SERIALIZE_ERROR_INT_OVERFLOW);
      return_trace (ಅಸತ್ಯ);
    }

    ವಿಕಲ್ಪ (u.format.v)
    {
    ಪಕ್ಷ 1: hb_barrier (); return_trace (u.format1.serialize (c, glyphs));
    ಪಕ್ಷ 2: hb_barrier (); return_trace (u.format2.serialize (c, glyphs));
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 3: hb_barrier (); return_trace (u.format3.serialize (c, glyphs));
    ಪಕ್ಷ 4: hb_barrier (); return_trace (u.format4.serialize (c, glyphs));
#endif
    ಸ್ವಭಾವ:return_trace (ಅಸತ್ಯ);
    }
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಧ್ರುವ hb_subset_plan_t *plan = c->plan;
    auto it =
    + iter ()
    | hb_take (plan->source->get_ಅಂಕೆ_glyphs ())
    | hb_map_retains_sorting ([plan] (hb_codepoint_t g) {
	ಪ್ರತ್ಯಾಗಮ plan->map_gsub_glyph (g);
      })
    | hb_filter ([] (hb_codepoint_t glyph) { ಪ್ರತ್ಯಾಗಮ glyph != HB_MAP_ಮೌಲ್ಯ_INVALID; })
    ;

    // Cache the iterator result as it will be iterated multiple times
    // by the serialize code below.
    hb_sorted_vector_t<hb_codepoint_t> glyphs (it);
    Coverage_serialize (c->serializer, glyphs.iter ());
    return_trace (ತಾರ್ಕಿಕ (glyphs));
  }

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v)
    {
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.intersects (glyphs);
    ಪಕ್ಷ 2: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format2.intersects (glyphs);
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 3: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format3.intersects (glyphs);
    ಪಕ್ಷ 4: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format4.intersects (glyphs);
#endif
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
  }
  ತಾರ್ಕಿಕ intersects_coverage (ಧ್ರುವ hb_set_t *glyphs, unsigned ಪೂರ್ಣಾಙ್ಕ index) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v)
    {
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.intersects_coverage (glyphs, index);
    ಪಕ್ಷ 2: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format2.intersects_coverage (glyphs, index);
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 3: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format3.intersects_coverage (glyphs, index);
    ಪಕ್ಷ 4: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format4.intersects_coverage (glyphs, index);
#endif
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
  }

  unsigned cost () ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.cost ();
    ಪಕ್ಷ 2: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format2.cost ();
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 3: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format3.cost ();
    ಪಕ್ಷ 4: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format4.cost ();
#endif
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename set_t>
  ತಾರ್ಕಿಕ collect_coverage (set_t *glyphs) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v)
    {
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.collect_coverage (glyphs);
    ಪಕ್ಷ 2: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format2.collect_coverage (glyphs);
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 3: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format3.collect_coverage (glyphs);
    ಪಕ್ಷ 4: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format4.collect_coverage (glyphs);
#endif
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
  }

  template <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  ಶೂನ್ಯ intersect_set (ಧ್ರುವ hb_set_t &glyphs, IterableOut&& intersect_glyphs) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v)
    {
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.intersect_set (glyphs, intersect_glyphs);
    ಪಕ್ಷ 2: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format2.intersect_set (glyphs, intersect_glyphs);
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 3: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format3.intersect_set (glyphs, intersect_glyphs);
    ಪಕ್ಷ 4: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format4.intersect_set (glyphs, intersect_glyphs);
#endif
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ ;
    }
  }

  ಸಂರಚನಾ iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ is_sorted_iterator = ಸತ್ಯ;
    iter_t (ಧ್ರುವ Coverage &c_ = Null (Coverage))
    {
      hb_memset (ಏತತ್, 0, ಪರಿಮಾಣ (*ಏತತ್));
      format = c_.u.format.v;
      ವಿಕಲ್ಪ (format)
      {
      ಪಕ್ಷ 1: hb_barrier (); u.format1.init (c_.u.format1); ಪ್ರತ್ಯಾಗಮ;
      ಪಕ್ಷ 2: hb_barrier (); u.format2.init (c_.u.format2); ಪ್ರತ್ಯಾಗಮ;
#ifndef HB_NO_BEYOND_64K
      ಪಕ್ಷ 3: hb_barrier (); u.format3.init (c_.u.format3); ಪ್ರತ್ಯಾಗಮ;
      ಪಕ್ಷ 4: hb_barrier (); u.format4.init (c_.u.format4); ಪ್ರತ್ಯಾಗಮ;
#endif
      ಸ್ವಭಾವ:                               ಪ್ರತ್ಯಾಗಮ;
      }
    }
    ತಾರ್ಕಿಕ __more__ () ಧ್ರುವ
    {
      ವಿಕಲ್ಪ (format)
      {
      ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.__more__ ();
      ಪಕ್ಷ 2: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format2.__more__ ();
#ifndef HB_NO_BEYOND_64K
      ಪಕ್ಷ 3: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format3.__more__ ();
      ಪಕ್ಷ 4: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format4.__more__ ();
#endif
      ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      }
    }
    ಶೂನ್ಯ __next__ ()
    {
      ವಿಕಲ್ಪ (format)
      {
      ಪಕ್ಷ 1: hb_barrier (); u.format1.__next__ (); ಭಙ್ಗ;
      ಪಕ್ಷ 2: hb_barrier (); u.format2.__next__ (); ಭಙ್ಗ;
#ifndef HB_NO_BEYOND_64K
      ಪಕ್ಷ 3: hb_barrier (); u.format3.__next__ (); ಭಙ್ಗ;
      ಪಕ್ಷ 4: hb_barrier (); u.format4.__next__ (); ಭಙ್ಗ;
#endif
      ಸ್ವಭಾವ:                   ಭಙ್ಗ;
      }
    }
    ಪ್ರಕಾರನಿರ್ದೇಶ hb_codepoint_t __item_t__;
    __item_t__ __item__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get_glyph (); }

    hb_codepoint_t get_glyph () ಧ್ರುವ
    {
      ವಿಕಲ್ಪ (format)
      {
      ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.get_glyph ();
      ಪಕ್ಷ 2: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format2.get_glyph ();
#ifndef HB_NO_BEYOND_64K
      ಪಕ್ಷ 3: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format3.get_glyph ();
      ಪಕ್ಷ 4: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format4.get_glyph ();
#endif
      ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ 0;
      }
    }
    ತಾರ್ಕಿಕ operator != (ಧ್ರುವ iter_t& o) ಧ್ರುವ
    {
      ಯದಿ (unlikely (format != o.format)) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      ವಿಕಲ್ಪ (format)
      {
      ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1 != o.u.format1;
      ಪಕ್ಷ 2: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format2 != o.u.format2;
#ifndef HB_NO_BEYOND_64K
      ಪಕ್ಷ 3: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format3 != o.u.format3;
      ಪಕ್ಷ 4: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format4 != o.u.format4;
#endif
      ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      }
    }
    iter_t __end__ () ಧ್ರುವ
    {
      iter_t it;
      it.format = format;
      ವಿಕಲ್ಪ (format)
      {
      ಪಕ್ಷ 1: hb_barrier (); it.u.format1 = u.format1.__end__ (); ಭಙ್ಗ;
      ಪಕ್ಷ 2: hb_barrier (); it.u.format2 = u.format2.__end__ (); ಭಙ್ಗ;
#ifndef HB_NO_BEYOND_64K
      ಪಕ್ಷ 3: hb_barrier (); it.u.format3 = u.format3.__end__ (); ಭಙ್ಗ;
      ಪಕ್ಷ 4: hb_barrier (); it.u.format4 = u.format4.__end__ (); ಭಙ್ಗ;
#endif
      ಸ್ವಭಾವ: ಭಙ್ಗ;
      }
      ಪ್ರತ್ಯಾಗಮ it;
    }

    ಗುಪ್ತ:
    unsigned ಪೂರ್ಣಾಙ್ಕ format;
    ಸಙ್ಘ {
#ifndef HB_NO_BEYOND_64K
    CoverageFormat2_4<MediumTypes>::iter_t      format4; /* Put this one first since it's larger; helps shut up compiler. */
    CoverageFormat1_3<MediumTypes>::iter_t      format3;
#endif
    CoverageFormat2_4<SmallTypes>::iter_t       format2; /* Put this one first since it's larger; helps shut up compiler. */
    CoverageFormat1_3<SmallTypes>::iter_t       format1;
    } u;
  };
  iter_t iter () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ iter_t (*ಏತತ್); }
};

template<typename Iterator>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
Coverage_serialize (hb_serialize_context_t *c,
                    Iterator it)
{ c->start_embed<Coverage> ()->serialize (c, it); }

}
}
}

#endif  // #ifndef OT_LAYOUT_COMMON_COVERAGE_HH

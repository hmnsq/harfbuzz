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

#അന്തര്ഭാവ "../../../hb-ot-dual.hh"
#അന്തര്ഭാവ "CoverageFormat1.hh"
#അന്തര്ഭാവ "CoverageFormat2.hh"

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ Common {

template<typename Iterator>
സ്ഥിതം വരിയിൽ ശൂന്യം Coverage_serialize (hb_serialize_context_t *c,
                                       Iterator it);

ഘടന Coverage
{

  സംരക്ഷിത:
  സംഘം {
  ഘടന { HBUINT16 v; }        format;         /* Format identifier */
  CoverageFormat1_3<SmallTypes> format1;
  CoverageFormat2_4<SmallTypes> format2;
#ifndef HB_NO_BEYOND_64K
  CoverageFormat1_3<MediumTypes>format3;
  CoverageFormat2_4<MediumTypes>format4;
#endif
  } u;
  പൊതു:
  DEFINE_SIZE_UNION (2, format.v);

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (!u.format.v.sanitize (c)) return_trace (അസത്യം);
    hb_barrier ();
    തിരഞ്ഞെടുപ്പ് (u.format.v)
    {
    അവസ്ഥ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    അവസ്ഥ 2: hb_barrier (); return_trace (u.format2.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    അവസ്ഥ 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    സ്ഥിരസ്ഥിതി:return_trace (സത്യം);
    }
  }

  /* Has interface. */
  unsigned operator [] (hb_codepoint_t k) സ്ഥിരം { തിരികെ get (k); }
  തർക്കം has (hb_codepoint_t k) സ്ഥിരം { തിരികെ (*ഇത്)[k] != NOT_COVERED; }
  /* Predicate. */
  തർക്കം operator () (hb_codepoint_t k) സ്ഥിരം { തിരികെ has (k); }

  unsigned പൂർണ്ണസംഖ്യ get (hb_codepoint_t k) സ്ഥിരം { തിരികെ get_coverage (k); }
  unsigned പൂർണ്ണസംഖ്യ get_coverage (hb_codepoint_t glyph_id) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.get_coverage (glyph_id);
    അവസ്ഥ 2: hb_barrier (); തിരികെ u.format2.get_coverage (glyph_id);
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 3: hb_barrier (); തിരികെ u.format3.get_coverage (glyph_id);
    അവസ്ഥ 4: hb_barrier (); തിരികെ u.format4.get_coverage (glyph_id);
#endif
    സ്ഥിരസ്ഥിതി:തിരികെ NOT_COVERED;
    }
  }
  unsigned പൂർണ്ണസംഖ്യ get_coverage (hb_codepoint_t glyph_id,
			     hb_ot_layout_mapping_cache_t *cache) സ്ഥിരം
  {
    unsigned coverage;
    എങ്കിൽ (cache && cache->get (glyph_id, &coverage)) തിരികെ coverage < cache->MAX_മൂല്യം ? coverage : NOT_COVERED;
    coverage = get_coverage (glyph_id);
    എങ്കിൽ (cache) {
      എങ്കിൽ (coverage == NOT_COVERED)
	cache->set (glyph_id, cache->MAX_മൂല്യം);
      അല്ലെങ്കിൽ എങ്കിൽ (likely (coverage < cache->MAX_മൂല്യം))
	cache->set (glyph_id, coverage);
    }
    തിരികെ coverage;
  }

  unsigned പൂർണ്ണസംഖ്യ get_coverage_ദ്വിമാന (hb_codepoint_t glyph_id,
				    hb_ot_layout_ദ്വിമാന_cache_t *cache) സ്ഥിരം
  {
    unsigned coverage;
    എങ്കിൽ (cache && cache->get (glyph_id, &coverage)) തിരികെ coverage < cache->MAX_മൂല്യം ? coverage : NOT_COVERED;
    coverage = get_coverage (glyph_id);
    എങ്കിൽ (cache) {
      എങ്കിൽ (coverage == NOT_COVERED)
	cache->set (glyph_id, cache->MAX_മൂല്യം);
      അല്ലെങ്കിൽ
	cache->set (glyph_id, 0);
    }
    തിരികെ coverage;
  }

  unsigned get_population () സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.get_population ();
    അവസ്ഥ 2: hb_barrier (); തിരികെ u.format2.get_population ();
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 3: hb_barrier (); തിരികെ u.format3.get_population ();
    അവസ്ഥ 4: hb_barrier (); തിരികെ u.format4.get_population ();
#endif
    സ്ഥിരസ്ഥിതി:തിരികെ NOT_COVERED;
    }
  }

  template <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  തർക്കം serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (ഇത്);
    എങ്കിൽ (unlikely (!c->extend_min (ഇത്))) return_trace (അസത്യം);

    unsigned എണ്ണം = hb_len (+ glyphs);
    unsigned അക്കം_ranges = 0;
    hb_codepoint_t last = (hb_codepoint_t) -2;
    hb_codepoint_t max = 0;
    തർക്കം unsorted = അസത്യം;
    വേണ്ടി (auto g: + glyphs)
    {
      എങ്കിൽ (last != (hb_codepoint_t) -2 && g < last)
	unsorted = സത്യം;
      എങ്കിൽ (last + 1 != g)
	അക്കം_ranges++;
      last = g;
      എങ്കിൽ (g > max) max = g;
    }
    u.format.v = !unsorted && എണ്ണം <= അക്കം_ranges * 3 ? 1 : 2;

#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (max > 0xFFFFu)
      u.format.v += 2;
    എങ്കിൽ (unlikely (max > 0xFFFFFFu))
#അല്ലെങ്കിൽ
    എങ്കിൽ (unlikely (max > 0xFFFFu))
#endif
    {
      c->check_success (അസത്യം, HB_SERIALIZE_ERROR_INT_OVERFLOW);
      return_trace (അസത്യം);
    }

    തിരഞ്ഞെടുപ്പ് (u.format.v)
    {
    അവസ്ഥ 1: hb_barrier (); return_trace (u.format1.serialize (c, glyphs));
    അവസ്ഥ 2: hb_barrier (); return_trace (u.format2.serialize (c, glyphs));
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 3: hb_barrier (); return_trace (u.format3.serialize (c, glyphs));
    അവസ്ഥ 4: hb_barrier (); return_trace (u.format4.serialize (c, glyphs));
#endif
    സ്ഥിരസ്ഥിതി:return_trace (അസത്യം);
    }
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    സ്ഥിരം hb_subset_plan_t *plan = c->plan;
    auto it =
    + iter ()
    | hb_take (plan->source->get_അക്കം_glyphs ())
    | hb_map_retains_sorting ([plan] (hb_codepoint_t g) {
	തിരികെ plan->map_gsub_glyph (g);
      })
    | hb_filter ([] (hb_codepoint_t glyph) { തിരികെ glyph != HB_MAP_മൂല്യം_INVALID; })
    ;

    // Cache the iterator result as it will be iterated multiple times
    // by the serialize code below.
    hb_sorted_vector_t<hb_codepoint_t> glyphs (it);
    Coverage_serialize (c->serializer, glyphs.iter ());
    return_trace (തർക്കം (glyphs));
  }

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v)
    {
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.intersects (glyphs);
    അവസ്ഥ 2: hb_barrier (); തിരികെ u.format2.intersects (glyphs);
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 3: hb_barrier (); തിരികെ u.format3.intersects (glyphs);
    അവസ്ഥ 4: hb_barrier (); തിരികെ u.format4.intersects (glyphs);
#endif
    സ്ഥിരസ്ഥിതി:തിരികെ അസത്യം;
    }
  }
  തർക്കം intersects_coverage (സ്ഥിരം hb_set_t *glyphs, unsigned പൂർണ്ണസംഖ്യ index) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v)
    {
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.intersects_coverage (glyphs, index);
    അവസ്ഥ 2: hb_barrier (); തിരികെ u.format2.intersects_coverage (glyphs, index);
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 3: hb_barrier (); തിരികെ u.format3.intersects_coverage (glyphs, index);
    അവസ്ഥ 4: hb_barrier (); തിരികെ u.format4.intersects_coverage (glyphs, index);
#endif
    സ്ഥിരസ്ഥിതി:തിരികെ അസത്യം;
    }
  }

  unsigned cost () സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.cost ();
    അവസ്ഥ 2: hb_barrier (); തിരികെ u.format2.cost ();
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 3: hb_barrier (); തിരികെ u.format3.cost ();
    അവസ്ഥ 4: hb_barrier (); തിരികെ u.format4.cost ();
#endif
    സ്ഥിരസ്ഥിതി:തിരികെ 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename set_t>
  തർക്കം collect_coverage (set_t *glyphs) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v)
    {
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.collect_coverage (glyphs);
    അവസ്ഥ 2: hb_barrier (); തിരികെ u.format2.collect_coverage (glyphs);
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 3: hb_barrier (); തിരികെ u.format3.collect_coverage (glyphs);
    അവസ്ഥ 4: hb_barrier (); തിരികെ u.format4.collect_coverage (glyphs);
#endif
    സ്ഥിരസ്ഥിതി:തിരികെ അസത്യം;
    }
  }

  template <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  ശൂന്യം intersect_set (സ്ഥിരം hb_set_t &glyphs, IterableOut&& intersect_glyphs) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v)
    {
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.intersect_set (glyphs, intersect_glyphs);
    അവസ്ഥ 2: hb_barrier (); തിരികെ u.format2.intersect_set (glyphs, intersect_glyphs);
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 3: hb_barrier (); തിരികെ u.format3.intersect_set (glyphs, intersect_glyphs);
    അവസ്ഥ 4: hb_barrier (); തിരികെ u.format4.intersect_set (glyphs, intersect_glyphs);
#endif
    സ്ഥിരസ്ഥിതി:തിരികെ ;
    }
  }

  ഘടന iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    സ്ഥിതം constexpr തർക്കം is_sorted_iterator = സത്യം;
    iter_t (സ്ഥിരം Coverage &c_ = Null (Coverage))
    {
      hb_memset (ഇത്, 0, വലുപ്പം (*ഇത്));
      format = c_.u.format.v;
      തിരഞ്ഞെടുപ്പ് (format)
      {
      അവസ്ഥ 1: hb_barrier (); u.format1.init (c_.u.format1); തിരികെ;
      അവസ്ഥ 2: hb_barrier (); u.format2.init (c_.u.format2); തിരികെ;
#ifndef HB_NO_BEYOND_64K
      അവസ്ഥ 3: hb_barrier (); u.format3.init (c_.u.format3); തിരികെ;
      അവസ്ഥ 4: hb_barrier (); u.format4.init (c_.u.format4); തിരികെ;
#endif
      സ്ഥിരസ്ഥിതി:                               തിരികെ;
      }
    }
    തർക്കം __more__ () സ്ഥിരം
    {
      തിരഞ്ഞെടുപ്പ് (format)
      {
      അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.__more__ ();
      അവസ്ഥ 2: hb_barrier (); തിരികെ u.format2.__more__ ();
#ifndef HB_NO_BEYOND_64K
      അവസ്ഥ 3: hb_barrier (); തിരികെ u.format3.__more__ ();
      അവസ്ഥ 4: hb_barrier (); തിരികെ u.format4.__more__ ();
#endif
      സ്ഥിരസ്ഥിതി:തിരികെ അസത്യം;
      }
    }
    ശൂന്യം __next__ ()
    {
      തിരഞ്ഞെടുപ്പ് (format)
      {
      അവസ്ഥ 1: hb_barrier (); u.format1.__next__ (); നിർത്തുക;
      അവസ്ഥ 2: hb_barrier (); u.format2.__next__ (); നിർത്തുക;
#ifndef HB_NO_BEYOND_64K
      അവസ്ഥ 3: hb_barrier (); u.format3.__next__ (); നിർത്തുക;
      അവസ്ഥ 4: hb_barrier (); u.format4.__next__ (); നിർത്തുക;
#endif
      സ്ഥിരസ്ഥിതി:                   നിർത്തുക;
      }
    }
    തരനിർവചനം hb_codepoint_t __item_t__;
    __item_t__ __item__ () സ്ഥിരം { തിരികെ get_glyph (); }

    hb_codepoint_t get_glyph () സ്ഥിരം
    {
      തിരഞ്ഞെടുപ്പ് (format)
      {
      അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.get_glyph ();
      അവസ്ഥ 2: hb_barrier (); തിരികെ u.format2.get_glyph ();
#ifndef HB_NO_BEYOND_64K
      അവസ്ഥ 3: hb_barrier (); തിരികെ u.format3.get_glyph ();
      അവസ്ഥ 4: hb_barrier (); തിരികെ u.format4.get_glyph ();
#endif
      സ്ഥിരസ്ഥിതി:തിരികെ 0;
      }
    }
    തർക്കം operator != (സ്ഥിരം iter_t& o) സ്ഥിരം
    {
      എങ്കിൽ (unlikely (format != o.format)) തിരികെ സത്യം;
      തിരഞ്ഞെടുപ്പ് (format)
      {
      അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1 != o.u.format1;
      അവസ്ഥ 2: hb_barrier (); തിരികെ u.format2 != o.u.format2;
#ifndef HB_NO_BEYOND_64K
      അവസ്ഥ 3: hb_barrier (); തിരികെ u.format3 != o.u.format3;
      അവസ്ഥ 4: hb_barrier (); തിരികെ u.format4 != o.u.format4;
#endif
      സ്ഥിരസ്ഥിതി:തിരികെ അസത്യം;
      }
    }
    iter_t __end__ () സ്ഥിരം
    {
      iter_t it;
      it.format = format;
      തിരഞ്ഞെടുപ്പ് (format)
      {
      അവസ്ഥ 1: hb_barrier (); it.u.format1 = u.format1.__end__ (); നിർത്തുക;
      അവസ്ഥ 2: hb_barrier (); it.u.format2 = u.format2.__end__ (); നിർത്തുക;
#ifndef HB_NO_BEYOND_64K
      അവസ്ഥ 3: hb_barrier (); it.u.format3 = u.format3.__end__ (); നിർത്തുക;
      അവസ്ഥ 4: hb_barrier (); it.u.format4 = u.format4.__end__ (); നിർത്തുക;
#endif
      സ്ഥിരസ്ഥിതി: നിർത്തുക;
      }
      തിരികെ it;
    }

    സ്വകാര്യ:
    unsigned പൂർണ്ണസംഖ്യ format;
    സംഘം {
#ifndef HB_NO_BEYOND_64K
    CoverageFormat2_4<MediumTypes>::iter_t      format4; /* Put this one first since it's larger; helps shut up compiler. */
    CoverageFormat1_3<MediumTypes>::iter_t      format3;
#endif
    CoverageFormat2_4<SmallTypes>::iter_t       format2; /* Put this one first since it's larger; helps shut up compiler. */
    CoverageFormat1_3<SmallTypes>::iter_t       format1;
    } u;
  };
  iter_t iter () സ്ഥിരം { തിരികെ iter_t (*ഇത്); }
};

template<typename Iterator>
സ്ഥിതം വരിയിൽ ശൂന്യം
Coverage_serialize (hb_serialize_context_t *c,
                    Iterator it)
{ c->start_embed<Coverage> ()->serialize (c, it); }

}
}
}

#endif  // #ifndef OT_LAYOUT_COMMON_COVERAGE_HH

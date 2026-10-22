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

#અન્તર્ભાવ "../../../hb-ot-dual.hh"
#અન્તર્ભાવ "CoverageFormat1.hh"
#અન્તર્ભાવ "CoverageFormat2.hh"

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ Common {

template<typename Iterator>
સ્થિર અંતઃરેખ શૂન્ય Coverage_serialize (hb_serialize_context_t *c,
                                       Iterator it);

સંરચના Coverage
{

  સુરક્ષિત:
  સંઘ {
  સંરચના { HBUINT16 v; }        format;         /* Format identifier */
  CoverageFormat1_3<SmallTypes> format1;
  CoverageFormat2_4<SmallTypes> format2;
#ifndef HB_NO_BEYOND_64K
  CoverageFormat1_3<MediumTypes>format3;
  CoverageFormat2_4<MediumTypes>format4;
#endif
  } u;
  જાહેર:
  DEFINE_SIZE_UNION (2, format.v);

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (!u.format.v.sanitize (c)) return_trace (ખોટું);
    hb_barrier ();
    પસંદગી (u.format.v)
    {
    સ્થિતિ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    સ્થિતિ 2: hb_barrier (); return_trace (u.format2.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    સ્થિતિ 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    મૂળભૂત:return_trace (સાચું);
    }
  }

  /* Has interface. */
  unsigned operator [] (hb_codepoint_t k) અચલ { પાછા get (k); }
  તાર્કિક has (hb_codepoint_t k) અચલ { પાછા (*આ)[k] != NOT_COVERED; }
  /* Predicate. */
  તાર્કિક operator () (hb_codepoint_t k) અચલ { પાછા has (k); }

  unsigned પૂર્ણાંક get (hb_codepoint_t k) અચલ { પાછા get_coverage (k); }
  unsigned પૂર્ણાંક get_coverage (hb_codepoint_t glyph_id) અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); પાછા u.format1.get_coverage (glyph_id);
    સ્થિતિ 2: hb_barrier (); પાછા u.format2.get_coverage (glyph_id);
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 3: hb_barrier (); પાછા u.format3.get_coverage (glyph_id);
    સ્થિતિ 4: hb_barrier (); પાછા u.format4.get_coverage (glyph_id);
#endif
    મૂળભૂત:પાછા NOT_COVERED;
    }
  }
  unsigned પૂર્ણાંક get_coverage (hb_codepoint_t glyph_id,
			     hb_ot_layout_mapping_cache_t *cache) અચલ
  {
    unsigned coverage;
    જો (cache && cache->get (glyph_id, &coverage)) પાછા coverage < cache->MAX_કિંમત ? coverage : NOT_COVERED;
    coverage = get_coverage (glyph_id);
    જો (cache) {
      જો (coverage == NOT_COVERED)
	cache->set (glyph_id, cache->MAX_કિંમત);
      નહીંતર જો (likely (coverage < cache->MAX_કિંમત))
	cache->set (glyph_id, coverage);
    }
    પાછા coverage;
  }

  unsigned પૂર્ણાંક get_coverage_દ્વિઅંકી (hb_codepoint_t glyph_id,
				    hb_ot_layout_દ્વિઅંકી_cache_t *cache) અચલ
  {
    unsigned coverage;
    જો (cache && cache->get (glyph_id, &coverage)) પાછા coverage < cache->MAX_કિંમત ? coverage : NOT_COVERED;
    coverage = get_coverage (glyph_id);
    જો (cache) {
      જો (coverage == NOT_COVERED)
	cache->set (glyph_id, cache->MAX_કિંમત);
      નહીંતર
	cache->set (glyph_id, 0);
    }
    પાછા coverage;
  }

  unsigned get_population () અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); પાછા u.format1.get_population ();
    સ્થિતિ 2: hb_barrier (); પાછા u.format2.get_population ();
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 3: hb_barrier (); પાછા u.format3.get_population ();
    સ્થિતિ 4: hb_barrier (); પાછા u.format4.get_population ();
#endif
    મૂળભૂત:પાછા NOT_COVERED;
    }
  }

  template <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  તાર્કિક serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (આ);
    જો (unlikely (!c->extend_min (આ))) return_trace (ખોટું);

    unsigned ગણતરી = hb_len (+ glyphs);
    unsigned અંક_ranges = 0;
    hb_codepoint_t last = (hb_codepoint_t) -2;
    hb_codepoint_t max = 0;
    તાર્કિક unsorted = ખોટું;
    માટે (auto g: + glyphs)
    {
      જો (last != (hb_codepoint_t) -2 && g < last)
	unsorted = સાચું;
      જો (last + 1 != g)
	અંક_ranges++;
      last = g;
      જો (g > max) max = g;
    }
    u.format.v = !unsorted && ગણતરી <= અંક_ranges * 3 ? 1 : 2;

#ifndef HB_NO_BEYOND_64K
    જો (max > 0xFFFFu)
      u.format.v += 2;
    જો (unlikely (max > 0xFFFFFFu))
#નહીંતર
    જો (unlikely (max > 0xFFFFu))
#endif
    {
      c->check_success (ખોટું, HB_SERIALIZE_ERROR_INT_OVERFLOW);
      return_trace (ખોટું);
    }

    પસંદગી (u.format.v)
    {
    સ્થિતિ 1: hb_barrier (); return_trace (u.format1.serialize (c, glyphs));
    સ્થિતિ 2: hb_barrier (); return_trace (u.format2.serialize (c, glyphs));
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 3: hb_barrier (); return_trace (u.format3.serialize (c, glyphs));
    સ્થિતિ 4: hb_barrier (); return_trace (u.format4.serialize (c, glyphs));
#endif
    મૂળભૂત:return_trace (ખોટું);
    }
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    અચલ hb_subset_plan_t *plan = c->plan;
    auto it =
    + iter ()
    | hb_take (plan->source->get_અંક_glyphs ())
    | hb_map_retains_sorting ([plan] (hb_codepoint_t g) {
	પાછા plan->map_gsub_glyph (g);
      })
    | hb_filter ([] (hb_codepoint_t glyph) { પાછા glyph != HB_MAP_કિંમત_INVALID; })
    ;

    // Cache the iterator result as it will be iterated multiple times
    // by the serialize code below.
    hb_sorted_vector_t<hb_codepoint_t> glyphs (it);
    Coverage_serialize (c->serializer, glyphs.iter ());
    return_trace (તાર્કિક (glyphs));
  }

  તાર્કિક intersects (અચલ hb_set_t *glyphs) અચલ
  {
    પસંદગી (u.format.v)
    {
    સ્થિતિ 1: hb_barrier (); પાછા u.format1.intersects (glyphs);
    સ્થિતિ 2: hb_barrier (); પાછા u.format2.intersects (glyphs);
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 3: hb_barrier (); પાછા u.format3.intersects (glyphs);
    સ્થિતિ 4: hb_barrier (); પાછા u.format4.intersects (glyphs);
#endif
    મૂળભૂત:પાછા ખોટું;
    }
  }
  તાર્કિક intersects_coverage (અચલ hb_set_t *glyphs, unsigned પૂર્ણાંક index) અચલ
  {
    પસંદગી (u.format.v)
    {
    સ્થિતિ 1: hb_barrier (); પાછા u.format1.intersects_coverage (glyphs, index);
    સ્થિતિ 2: hb_barrier (); પાછા u.format2.intersects_coverage (glyphs, index);
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 3: hb_barrier (); પાછા u.format3.intersects_coverage (glyphs, index);
    સ્થિતિ 4: hb_barrier (); પાછા u.format4.intersects_coverage (glyphs, index);
#endif
    મૂળભૂત:પાછા ખોટું;
    }
  }

  unsigned cost () અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); પાછા u.format1.cost ();
    સ્થિતિ 2: hb_barrier (); પાછા u.format2.cost ();
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 3: hb_barrier (); પાછા u.format3.cost ();
    સ્થિતિ 4: hb_barrier (); પાછા u.format4.cost ();
#endif
    મૂળભૂત:પાછા 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename set_t>
  તાર્કિક collect_coverage (set_t *glyphs) અચલ
  {
    પસંદગી (u.format.v)
    {
    સ્થિતિ 1: hb_barrier (); પાછા u.format1.collect_coverage (glyphs);
    સ્થિતિ 2: hb_barrier (); પાછા u.format2.collect_coverage (glyphs);
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 3: hb_barrier (); પાછા u.format3.collect_coverage (glyphs);
    સ્થિતિ 4: hb_barrier (); પાછા u.format4.collect_coverage (glyphs);
#endif
    મૂળભૂત:પાછા ખોટું;
    }
  }

  template <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  શૂન્ય intersect_set (અચલ hb_set_t &glyphs, IterableOut&& intersect_glyphs) અચલ
  {
    પસંદગી (u.format.v)
    {
    સ્થિતિ 1: hb_barrier (); પાછા u.format1.intersect_set (glyphs, intersect_glyphs);
    સ્થિતિ 2: hb_barrier (); પાછા u.format2.intersect_set (glyphs, intersect_glyphs);
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 3: hb_barrier (); પાછા u.format3.intersect_set (glyphs, intersect_glyphs);
    સ્થિતિ 4: hb_barrier (); પાછા u.format4.intersect_set (glyphs, intersect_glyphs);
#endif
    મૂળભૂત:પાછા ;
    }
  }

  સંરચના iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    સ્થિર constexpr તાર્કિક is_sorted_iterator = સાચું;
    iter_t (અચલ Coverage &c_ = Null (Coverage))
    {
      hb_memset (આ, 0, કદ (*આ));
      format = c_.u.format.v;
      પસંદગી (format)
      {
      સ્થિતિ 1: hb_barrier (); u.format1.init (c_.u.format1); પાછા;
      સ્થિતિ 2: hb_barrier (); u.format2.init (c_.u.format2); પાછા;
#ifndef HB_NO_BEYOND_64K
      સ્થિતિ 3: hb_barrier (); u.format3.init (c_.u.format3); પાછા;
      સ્થિતિ 4: hb_barrier (); u.format4.init (c_.u.format4); પાછા;
#endif
      મૂળભૂત:                               પાછા;
      }
    }
    તાર્કિક __more__ () અચલ
    {
      પસંદગી (format)
      {
      સ્થિતિ 1: hb_barrier (); પાછા u.format1.__more__ ();
      સ્થિતિ 2: hb_barrier (); પાછા u.format2.__more__ ();
#ifndef HB_NO_BEYOND_64K
      સ્થિતિ 3: hb_barrier (); પાછા u.format3.__more__ ();
      સ્થિતિ 4: hb_barrier (); પાછા u.format4.__more__ ();
#endif
      મૂળભૂત:પાછા ખોટું;
      }
    }
    શૂન્ય __next__ ()
    {
      પસંદગી (format)
      {
      સ્થિતિ 1: hb_barrier (); u.format1.__next__ (); તોડો;
      સ્થિતિ 2: hb_barrier (); u.format2.__next__ (); તોડો;
#ifndef HB_NO_BEYOND_64K
      સ્થિતિ 3: hb_barrier (); u.format3.__next__ (); તોડો;
      સ્થિતિ 4: hb_barrier (); u.format4.__next__ (); તોડો;
#endif
      મૂળભૂત:                   તોડો;
      }
    }
    પ્રકારવ્યાખ્યા hb_codepoint_t __item_t__;
    __item_t__ __item__ () અચલ { પાછા get_glyph (); }

    hb_codepoint_t get_glyph () અચલ
    {
      પસંદગી (format)
      {
      સ્થિતિ 1: hb_barrier (); પાછા u.format1.get_glyph ();
      સ્થિતિ 2: hb_barrier (); પાછા u.format2.get_glyph ();
#ifndef HB_NO_BEYOND_64K
      સ્થિતિ 3: hb_barrier (); પાછા u.format3.get_glyph ();
      સ્થિતિ 4: hb_barrier (); પાછા u.format4.get_glyph ();
#endif
      મૂળભૂત:પાછા 0;
      }
    }
    તાર્કિક operator != (અચલ iter_t& o) અચલ
    {
      જો (unlikely (format != o.format)) પાછા સાચું;
      પસંદગી (format)
      {
      સ્થિતિ 1: hb_barrier (); પાછા u.format1 != o.u.format1;
      સ્થિતિ 2: hb_barrier (); પાછા u.format2 != o.u.format2;
#ifndef HB_NO_BEYOND_64K
      સ્થિતિ 3: hb_barrier (); પાછા u.format3 != o.u.format3;
      સ્થિતિ 4: hb_barrier (); પાછા u.format4 != o.u.format4;
#endif
      મૂળભૂત:પાછા ખોટું;
      }
    }
    iter_t __end__ () અચલ
    {
      iter_t it;
      it.format = format;
      પસંદગી (format)
      {
      સ્થિતિ 1: hb_barrier (); it.u.format1 = u.format1.__end__ (); તોડો;
      સ્થિતિ 2: hb_barrier (); it.u.format2 = u.format2.__end__ (); તોડો;
#ifndef HB_NO_BEYOND_64K
      સ્થિતિ 3: hb_barrier (); it.u.format3 = u.format3.__end__ (); તોડો;
      સ્થિતિ 4: hb_barrier (); it.u.format4 = u.format4.__end__ (); તોડો;
#endif
      મૂળભૂત: તોડો;
      }
      પાછા it;
    }

    ખાનગી:
    unsigned પૂર્ણાંક format;
    સંઘ {
#ifndef HB_NO_BEYOND_64K
    CoverageFormat2_4<MediumTypes>::iter_t      format4; /* Put this one first since it's larger; helps shut up compiler. */
    CoverageFormat1_3<MediumTypes>::iter_t      format3;
#endif
    CoverageFormat2_4<SmallTypes>::iter_t       format2; /* Put this one first since it's larger; helps shut up compiler. */
    CoverageFormat1_3<SmallTypes>::iter_t       format1;
    } u;
  };
  iter_t iter () અચલ { પાછા iter_t (*આ); }
};

template<typename Iterator>
સ્થિર અંતઃરેખ શૂન્ય
Coverage_serialize (hb_serialize_context_t *c,
                    Iterator it)
{ c->start_embed<Coverage> ()->serialize (c, it); }

}
}
}

#endif  // #ifndef OT_LAYOUT_COMMON_COVERAGE_HH

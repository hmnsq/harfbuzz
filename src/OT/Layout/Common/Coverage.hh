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

#अन्तर्भाव "../../../hb-ot-dual.hh"
#अन्तर्भाव "CoverageFormat1.hh"
#अन्तर्भाव "CoverageFormat2.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश Common {

ढाँचा<typename Iterator>
स्थिर ओळीत शून्य Coverage_serialize (hb_serialize_context_t *c,
                                       Iterator it);

संरचना Coverage
{

  संरक्षित:
  संघ {
  संरचना { HBUINT16 v; }        format;         /* Format identifier */
  CoverageFormat1_3<SmallTypes> format1;
  CoverageFormat2_4<SmallTypes> format2;
#ifndef HB_NO_BEYOND_64K
  CoverageFormat1_3<MediumTypes>format3;
  CoverageFormat2_4<MediumTypes>format4;
#endif
  } u;
  सार्वजनिक:
  DEFINE_SIZE_UNION (2, format.v);

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (!u.format.v.sanitize (c)) return_trace (असत्य);
    hb_barrier ();
    निवड (u.format.v)
    {
    स्थिती 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    स्थिती 2: hb_barrier (); return_trace (u.format2.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    स्थिती 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    स्थिती 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    पूर्वनिर्धारित:return_trace (सत्य);
    }
  }

  /* Has interface. */
  अचिन्हित चालक [] (hb_codepoint_t k) स्थिरांक { परत get (k); }
  तार्किक has (hb_codepoint_t k) स्थिरांक { परत (*हे)[k] != NOT_COVERED; }
  /* Predicate. */
  तार्किक चालक () (hb_codepoint_t k) स्थिरांक { परत has (k); }

  अचिन्हित पूर्णांक get (hb_codepoint_t k) स्थिरांक { परत get_coverage (k); }
  अचिन्हित पूर्णांक get_coverage (hb_codepoint_t glyph_id) स्थिरांक
  {
    निवड (u.format.v) {
    स्थिती 1: hb_barrier (); परत u.format1.get_coverage (glyph_id);
    स्थिती 2: hb_barrier (); परत u.format2.get_coverage (glyph_id);
#ifndef HB_NO_BEYOND_64K
    स्थिती 3: hb_barrier (); परत u.format3.get_coverage (glyph_id);
    स्थिती 4: hb_barrier (); परत u.format4.get_coverage (glyph_id);
#endif
    पूर्वनिर्धारित:परत NOT_COVERED;
    }
  }
  अचिन्हित पूर्णांक get_coverage (hb_codepoint_t glyph_id,
			     hb_ot_layout_mapping_cache_t *cache) स्थिरांक
  {
    अचिन्हित coverage;
    जर (cache && cache->get (glyph_id, &coverage)) परत coverage < cache->MAX_मूल्य ? coverage : NOT_COVERED;
    coverage = get_coverage (glyph_id);
    जर (cache) {
      जर (coverage == NOT_COVERED)
	cache->set (glyph_id, cache->MAX_मूल्य);
      अन्यथा जर (likely (coverage < cache->MAX_मूल्य))
	cache->set (glyph_id, coverage);
    }
    परत coverage;
  }

  अचिन्हित पूर्णांक get_coverage_द्विमान (hb_codepoint_t glyph_id,
				    hb_ot_layout_द्विमान_cache_t *cache) स्थिरांक
  {
    अचिन्हित coverage;
    जर (cache && cache->get (glyph_id, &coverage)) परत coverage < cache->MAX_मूल्य ? coverage : NOT_COVERED;
    coverage = get_coverage (glyph_id);
    जर (cache) {
      जर (coverage == NOT_COVERED)
	cache->set (glyph_id, cache->MAX_मूल्य);
      अन्यथा
	cache->set (glyph_id, 0);
    }
    परत coverage;
  }

  अचिन्हित get_population () स्थिरांक
  {
    निवड (u.format.v) {
    स्थिती 1: hb_barrier (); परत u.format1.get_population ();
    स्थिती 2: hb_barrier (); परत u.format2.get_population ();
#ifndef HB_NO_BEYOND_64K
    स्थिती 3: hb_barrier (); परत u.format3.get_population ();
    स्थिती 4: hb_barrier (); परत u.format4.get_population ();
#endif
    पूर्वनिर्धारित:परत NOT_COVERED;
    }
  }

  ढाँचा <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  तार्किक serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (हे);
    जर (unlikely (!c->extend_min (हे))) return_trace (असत्य);

    अचिन्हित मोजणी = hb_len (+ glyphs);
    अचिन्हित अंक_ranges = 0;
    hb_codepoint_t last = (hb_codepoint_t) -2;
    hb_codepoint_t max = 0;
    तार्किक unsorted = असत्य;
    साठी (स्वतः g: + glyphs)
    {
      जर (last != (hb_codepoint_t) -2 && g < last)
	unsorted = सत्य;
      जर (last + 1 != g)
	अंक_ranges++;
      last = g;
      जर (g > max) max = g;
    }
    u.format.v = !unsorted && मोजणी <= अंक_ranges * 3 ? 1 : 2;

#ifndef HB_NO_BEYOND_64K
    जर (max > 0xFFFFu)
      u.format.v += 2;
    जर (unlikely (max > 0xFFFFFFu))
#अन्यथा
    जर (unlikely (max > 0xFFFFu))
#endif
    {
      c->check_success (असत्य, HB_SERIALIZE_ERROR_INT_OVERFLOW);
      return_trace (असत्य);
    }

    निवड (u.format.v)
    {
    स्थिती 1: hb_barrier (); return_trace (u.format1.serialize (c, glyphs));
    स्थिती 2: hb_barrier (); return_trace (u.format2.serialize (c, glyphs));
#ifndef HB_NO_BEYOND_64K
    स्थिती 3: hb_barrier (); return_trace (u.format3.serialize (c, glyphs));
    स्थिती 4: hb_barrier (); return_trace (u.format4.serialize (c, glyphs));
#endif
    पूर्वनिर्धारित:return_trace (असत्य);
    }
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्थिरांक hb_subset_plan_t *plan = c->plan;
    स्वतः it =
    + iter ()
    | hb_take (plan->source->get_अंक_glyphs ())
    | hb_map_retains_sorting ([plan] (hb_codepoint_t g) {
	परत plan->map_gsub_glyph (g);
      })
    | hb_filter ([] (hb_codepoint_t glyph) { परत glyph != HB_MAP_मूल्य_INVALID; })
    ;

    // Cache the iterator result as it will be iterated multiple times
    // by the serialize code below.
    hb_sorted_vector_t<hb_codepoint_t> glyphs (it);
    Coverage_serialize (c->serializer, glyphs.iter ());
    return_trace (तार्किक (glyphs));
  }

  तार्किक intersects (स्थिरांक hb_set_t *glyphs) स्थिरांक
  {
    निवड (u.format.v)
    {
    स्थिती 1: hb_barrier (); परत u.format1.intersects (glyphs);
    स्थिती 2: hb_barrier (); परत u.format2.intersects (glyphs);
#ifndef HB_NO_BEYOND_64K
    स्थिती 3: hb_barrier (); परत u.format3.intersects (glyphs);
    स्थिती 4: hb_barrier (); परत u.format4.intersects (glyphs);
#endif
    पूर्वनिर्धारित:परत असत्य;
    }
  }
  तार्किक intersects_coverage (स्थिरांक hb_set_t *glyphs, अचिन्हित पूर्णांक index) स्थिरांक
  {
    निवड (u.format.v)
    {
    स्थिती 1: hb_barrier (); परत u.format1.intersects_coverage (glyphs, index);
    स्थिती 2: hb_barrier (); परत u.format2.intersects_coverage (glyphs, index);
#ifndef HB_NO_BEYOND_64K
    स्थिती 3: hb_barrier (); परत u.format3.intersects_coverage (glyphs, index);
    स्थिती 4: hb_barrier (); परत u.format4.intersects_coverage (glyphs, index);
#endif
    पूर्वनिर्धारित:परत असत्य;
    }
  }

  अचिन्हित cost () स्थिरांक
  {
    निवड (u.format.v) {
    स्थिती 1: hb_barrier (); परत u.format1.cost ();
    स्थिती 2: hb_barrier (); परत u.format2.cost ();
#ifndef HB_NO_BEYOND_64K
    स्थिती 3: hb_barrier (); परत u.format3.cost ();
    स्थिती 4: hb_barrier (); परत u.format4.cost ();
#endif
    पूर्वनिर्धारित:परत 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  ढाँचा <typename set_t>
  तार्किक collect_coverage (set_t *glyphs) स्थिरांक
  {
    निवड (u.format.v)
    {
    स्थिती 1: hb_barrier (); परत u.format1.collect_coverage (glyphs);
    स्थिती 2: hb_barrier (); परत u.format2.collect_coverage (glyphs);
#ifndef HB_NO_BEYOND_64K
    स्थिती 3: hb_barrier (); परत u.format3.collect_coverage (glyphs);
    स्थिती 4: hb_barrier (); परत u.format4.collect_coverage (glyphs);
#endif
    पूर्वनिर्धारित:परत असत्य;
    }
  }

  ढाँचा <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  शून्य intersect_set (स्थिरांक hb_set_t &glyphs, IterableOut&& intersect_glyphs) स्थिरांक
  {
    निवड (u.format.v)
    {
    स्थिती 1: hb_barrier (); परत u.format1.intersect_set (glyphs, intersect_glyphs);
    स्थिती 2: hb_barrier (); परत u.format2.intersect_set (glyphs, intersect_glyphs);
#ifndef HB_NO_BEYOND_64K
    स्थिती 3: hb_barrier (); परत u.format3.intersect_set (glyphs, intersect_glyphs);
    स्थिती 4: hb_barrier (); परत u.format4.intersect_set (glyphs, intersect_glyphs);
#endif
    पूर्वनिर्धारित:परत ;
    }
  }

  संरचना iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    स्थिर constexpr तार्किक is_sorted_iterator = सत्य;
    iter_t (स्थिरांक Coverage &c_ = Null (Coverage))
    {
      hb_memset (हे, 0, आकार (*हे));
      format = c_.u.format.v;
      निवड (format)
      {
      स्थिती 1: hb_barrier (); u.format1.init (c_.u.format1); परत;
      स्थिती 2: hb_barrier (); u.format2.init (c_.u.format2); परत;
#ifndef HB_NO_BEYOND_64K
      स्थिती 3: hb_barrier (); u.format3.init (c_.u.format3); परत;
      स्थिती 4: hb_barrier (); u.format4.init (c_.u.format4); परत;
#endif
      पूर्वनिर्धारित:                               परत;
      }
    }
    तार्किक __more__ () स्थिरांक
    {
      निवड (format)
      {
      स्थिती 1: hb_barrier (); परत u.format1.__more__ ();
      स्थिती 2: hb_barrier (); परत u.format2.__more__ ();
#ifndef HB_NO_BEYOND_64K
      स्थिती 3: hb_barrier (); परत u.format3.__more__ ();
      स्थिती 4: hb_barrier (); परत u.format4.__more__ ();
#endif
      पूर्वनिर्धारित:परत असत्य;
      }
    }
    शून्य __next__ ()
    {
      निवड (format)
      {
      स्थिती 1: hb_barrier (); u.format1.__next__ (); थांबा;
      स्थिती 2: hb_barrier (); u.format2.__next__ (); थांबा;
#ifndef HB_NO_BEYOND_64K
      स्थिती 3: hb_barrier (); u.format3.__next__ (); थांबा;
      स्थिती 4: hb_barrier (); u.format4.__next__ (); थांबा;
#endif
      पूर्वनिर्धारित:                   थांबा;
      }
    }
    प्रकारव्याख्या hb_codepoint_t __item_t__;
    __item_t__ __item__ () स्थिरांक { परत get_glyph (); }

    hb_codepoint_t get_glyph () स्थिरांक
    {
      निवड (format)
      {
      स्थिती 1: hb_barrier (); परत u.format1.get_glyph ();
      स्थिती 2: hb_barrier (); परत u.format2.get_glyph ();
#ifndef HB_NO_BEYOND_64K
      स्थिती 3: hb_barrier (); परत u.format3.get_glyph ();
      स्थिती 4: hb_barrier (); परत u.format4.get_glyph ();
#endif
      पूर्वनिर्धारित:परत 0;
      }
    }
    तार्किक चालक != (स्थिरांक iter_t& o) स्थिरांक
    {
      जर (unlikely (format != o.format)) परत सत्य;
      निवड (format)
      {
      स्थिती 1: hb_barrier (); परत u.format1 != o.u.format1;
      स्थिती 2: hb_barrier (); परत u.format2 != o.u.format2;
#ifndef HB_NO_BEYOND_64K
      स्थिती 3: hb_barrier (); परत u.format3 != o.u.format3;
      स्थिती 4: hb_barrier (); परत u.format4 != o.u.format4;
#endif
      पूर्वनिर्धारित:परत असत्य;
      }
    }
    iter_t __end__ () स्थिरांक
    {
      iter_t it;
      it.format = format;
      निवड (format)
      {
      स्थिती 1: hb_barrier (); it.u.format1 = u.format1.__end__ (); थांबा;
      स्थिती 2: hb_barrier (); it.u.format2 = u.format2.__end__ (); थांबा;
#ifndef HB_NO_BEYOND_64K
      स्थिती 3: hb_barrier (); it.u.format3 = u.format3.__end__ (); थांबा;
      स्थिती 4: hb_barrier (); it.u.format4 = u.format4.__end__ (); थांबा;
#endif
      पूर्वनिर्धारित: थांबा;
      }
      परत it;
    }

    खाजगी:
    अचिन्हित पूर्णांक format;
    संघ {
#ifndef HB_NO_BEYOND_64K
    CoverageFormat2_4<MediumTypes>::iter_t      format4; /* Put this one first since it's larger; helps shut up compiler. */
    CoverageFormat1_3<MediumTypes>::iter_t      format3;
#endif
    CoverageFormat2_4<SmallTypes>::iter_t       format2; /* Put this one first since it's larger; helps shut up compiler. */
    CoverageFormat1_3<SmallTypes>::iter_t       format1;
    } u;
  };
  iter_t iter () स्थिरांक { परत iter_t (*हे); }
};

ढाँचा<typename Iterator>
स्थिर ओळीत शून्य
Coverage_serialize (hb_serialize_context_t *c,
                    Iterator it)
{ c->start_embed<Coverage> ()->serialize (c, it); }

}
}
}

#endif  // #ifndef OT_LAYOUT_COMMON_COVERAGE_HH

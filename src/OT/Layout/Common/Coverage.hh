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

template<typename Iterator>
स्थिर अन्तःस्थ शून्य Coverage_serialize (hb_serialize_context_t *c,
                                       Iterator it);

संरचना Coverage
{

  रक्षित:
  सङ्घ {
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
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    यदि (!u.format.v.sanitize (c)) return_trace (असत्य);
    hb_barrier ();
    विकल्प (u.format.v)
    {
    पक्ष 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    पक्ष 2: hb_barrier (); return_trace (u.format2.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    पक्ष 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    स्वभाव:return_trace (सत्य);
    }
  }

  /* Has interface. */
  unsigned operator [] (hb_codepoint_t k) ध्रुव { प्रत्यागम get (k); }
  तार्किक has (hb_codepoint_t k) ध्रुव { प्रत्यागम (*एतत्)[k] != NOT_COVERED; }
  /* Predicate. */
  तार्किक operator () (hb_codepoint_t k) ध्रुव { प्रत्यागम has (k); }

  unsigned पूर्णाङ्क get (hb_codepoint_t k) ध्रुव { प्रत्यागम get_coverage (k); }
  unsigned पूर्णाङ्क get_coverage (hb_codepoint_t glyph_id) ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); प्रत्यागम u.format1.get_coverage (glyph_id);
    पक्ष 2: hb_barrier (); प्रत्यागम u.format2.get_coverage (glyph_id);
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: hb_barrier (); प्रत्यागम u.format3.get_coverage (glyph_id);
    पक्ष 4: hb_barrier (); प्रत्यागम u.format4.get_coverage (glyph_id);
#endif
    स्वभाव:प्रत्यागम NOT_COVERED;
    }
  }
  unsigned पूर्णाङ्क get_coverage (hb_codepoint_t glyph_id,
			     hb_ot_layout_mapping_cache_t *cache) ध्रुव
  {
    unsigned coverage;
    यदि (cache && cache->get (glyph_id, &coverage)) प्रत्यागम coverage < cache->MAX_मूल्य ? coverage : NOT_COVERED;
    coverage = get_coverage (glyph_id);
    यदि (cache) {
      यदि (coverage == NOT_COVERED)
	cache->set (glyph_id, cache->MAX_मूल्य);
      अन्यथा यदि (likely (coverage < cache->MAX_मूल्य))
	cache->set (glyph_id, coverage);
    }
    प्रत्यागम coverage;
  }

  unsigned पूर्णाङ्क get_coverage_द्विमान (hb_codepoint_t glyph_id,
				    hb_ot_layout_द्विमान_cache_t *cache) ध्रुव
  {
    unsigned coverage;
    यदि (cache && cache->get (glyph_id, &coverage)) प्रत्यागम coverage < cache->MAX_मूल्य ? coverage : NOT_COVERED;
    coverage = get_coverage (glyph_id);
    यदि (cache) {
      यदि (coverage == NOT_COVERED)
	cache->set (glyph_id, cache->MAX_मूल्य);
      अन्यथा
	cache->set (glyph_id, 0);
    }
    प्रत्यागम coverage;
  }

  unsigned get_population () ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); प्रत्यागम u.format1.get_population ();
    पक्ष 2: hb_barrier (); प्रत्यागम u.format2.get_population ();
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: hb_barrier (); प्रत्यागम u.format3.get_population ();
    पक्ष 4: hb_barrier (); प्रत्यागम u.format4.get_population ();
#endif
    स्वभाव:प्रत्यागम NOT_COVERED;
    }
  }

  template <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  तार्किक serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (एतत्);
    यदि (unlikely (!c->extend_min (एतत्))) return_trace (असत्य);

    unsigned मोजणी = hb_len (+ glyphs);
    unsigned अंक_ranges = 0;
    hb_codepoint_t last = (hb_codepoint_t) -2;
    hb_codepoint_t max = 0;
    तार्किक unsorted = असत्य;
    प्रति (auto g: + glyphs)
    {
      यदि (last != (hb_codepoint_t) -2 && g < last)
	unsorted = सत्य;
      यदि (last + 1 != g)
	अंक_ranges++;
      last = g;
      यदि (g > max) max = g;
    }
    u.format.v = !unsorted && मोजणी <= अंक_ranges * 3 ? 1 : 2;

#ifndef HB_NO_BEYOND_64K
    यदि (max > 0xFFFFu)
      u.format.v += 2;
    यदि (unlikely (max > 0xFFFFFFu))
#अन्यथा
    यदि (unlikely (max > 0xFFFFu))
#endif
    {
      c->check_success (असत्य, HB_SERIALIZE_ERROR_INT_OVERFLOW);
      return_trace (असत्य);
    }

    विकल्प (u.format.v)
    {
    पक्ष 1: hb_barrier (); return_trace (u.format1.serialize (c, glyphs));
    पक्ष 2: hb_barrier (); return_trace (u.format2.serialize (c, glyphs));
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: hb_barrier (); return_trace (u.format3.serialize (c, glyphs));
    पक्ष 4: hb_barrier (); return_trace (u.format4.serialize (c, glyphs));
#endif
    स्वभाव:return_trace (असत्य);
    }
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    ध्रुव hb_subset_plan_t *plan = c->plan;
    auto it =
    + iter ()
    | hb_take (plan->source->get_अंक_glyphs ())
    | hb_map_retains_sorting ([plan] (hb_codepoint_t g) {
	प्रत्यागम plan->map_gsub_glyph (g);
      })
    | hb_filter ([] (hb_codepoint_t glyph) { प्रत्यागम glyph != HB_MAP_मूल्य_INVALID; })
    ;

    // Cache the iterator result as it will be iterated multiple times
    // by the serialize code below.
    hb_sorted_vector_t<hb_codepoint_t> glyphs (it);
    Coverage_serialize (c->serializer, glyphs.iter ());
    return_trace (तार्किक (glyphs));
  }

  तार्किक intersects (ध्रुव hb_set_t *glyphs) ध्रुव
  {
    विकल्प (u.format.v)
    {
    पक्ष 1: hb_barrier (); प्रत्यागम u.format1.intersects (glyphs);
    पक्ष 2: hb_barrier (); प्रत्यागम u.format2.intersects (glyphs);
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: hb_barrier (); प्रत्यागम u.format3.intersects (glyphs);
    पक्ष 4: hb_barrier (); प्रत्यागम u.format4.intersects (glyphs);
#endif
    स्वभाव:प्रत्यागम असत्य;
    }
  }
  तार्किक intersects_coverage (ध्रुव hb_set_t *glyphs, unsigned पूर्णाङ्क index) ध्रुव
  {
    विकल्प (u.format.v)
    {
    पक्ष 1: hb_barrier (); प्रत्यागम u.format1.intersects_coverage (glyphs, index);
    पक्ष 2: hb_barrier (); प्रत्यागम u.format2.intersects_coverage (glyphs, index);
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: hb_barrier (); प्रत्यागम u.format3.intersects_coverage (glyphs, index);
    पक्ष 4: hb_barrier (); प्रत्यागम u.format4.intersects_coverage (glyphs, index);
#endif
    स्वभाव:प्रत्यागम असत्य;
    }
  }

  unsigned cost () ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); प्रत्यागम u.format1.cost ();
    पक्ष 2: hb_barrier (); प्रत्यागम u.format2.cost ();
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: hb_barrier (); प्रत्यागम u.format3.cost ();
    पक्ष 4: hb_barrier (); प्रत्यागम u.format4.cost ();
#endif
    स्वभाव:प्रत्यागम 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename set_t>
  तार्किक collect_coverage (set_t *glyphs) ध्रुव
  {
    विकल्प (u.format.v)
    {
    पक्ष 1: hb_barrier (); प्रत्यागम u.format1.collect_coverage (glyphs);
    पक्ष 2: hb_barrier (); प्रत्यागम u.format2.collect_coverage (glyphs);
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: hb_barrier (); प्रत्यागम u.format3.collect_coverage (glyphs);
    पक्ष 4: hb_barrier (); प्रत्यागम u.format4.collect_coverage (glyphs);
#endif
    स्वभाव:प्रत्यागम असत्य;
    }
  }

  template <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  शून्य intersect_set (ध्रुव hb_set_t &glyphs, IterableOut&& intersect_glyphs) ध्रुव
  {
    विकल्प (u.format.v)
    {
    पक्ष 1: hb_barrier (); प्रत्यागम u.format1.intersect_set (glyphs, intersect_glyphs);
    पक्ष 2: hb_barrier (); प्रत्यागम u.format2.intersect_set (glyphs, intersect_glyphs);
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: hb_barrier (); प्रत्यागम u.format3.intersect_set (glyphs, intersect_glyphs);
    पक्ष 4: hb_barrier (); प्रत्यागम u.format4.intersect_set (glyphs, intersect_glyphs);
#endif
    स्वभाव:प्रत्यागम ;
    }
  }

  संरचना iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    स्थिर constexpr तार्किक is_sorted_iterator = सत्य;
    iter_t (ध्रुव Coverage &c_ = Null (Coverage))
    {
      hb_memset (एतत्, 0, परिमाण (*एतत्));
      format = c_.u.format.v;
      विकल्प (format)
      {
      पक्ष 1: hb_barrier (); u.format1.init (c_.u.format1); प्रत्यागम;
      पक्ष 2: hb_barrier (); u.format2.init (c_.u.format2); प्रत्यागम;
#ifndef HB_NO_BEYOND_64K
      पक्ष 3: hb_barrier (); u.format3.init (c_.u.format3); प्रत्यागम;
      पक्ष 4: hb_barrier (); u.format4.init (c_.u.format4); प्रत्यागम;
#endif
      स्वभाव:                               प्रत्यागम;
      }
    }
    तार्किक __more__ () ध्रुव
    {
      विकल्प (format)
      {
      पक्ष 1: hb_barrier (); प्रत्यागम u.format1.__more__ ();
      पक्ष 2: hb_barrier (); प्रत्यागम u.format2.__more__ ();
#ifndef HB_NO_BEYOND_64K
      पक्ष 3: hb_barrier (); प्रत्यागम u.format3.__more__ ();
      पक्ष 4: hb_barrier (); प्रत्यागम u.format4.__more__ ();
#endif
      स्वभाव:प्रत्यागम असत्य;
      }
    }
    शून्य __next__ ()
    {
      विकल्प (format)
      {
      पक्ष 1: hb_barrier (); u.format1.__next__ (); भङ्ग;
      पक्ष 2: hb_barrier (); u.format2.__next__ (); भङ्ग;
#ifndef HB_NO_BEYOND_64K
      पक्ष 3: hb_barrier (); u.format3.__next__ (); भङ्ग;
      पक्ष 4: hb_barrier (); u.format4.__next__ (); भङ्ग;
#endif
      स्वभाव:                   भङ्ग;
      }
    }
    प्रकारनिर्देश hb_codepoint_t __item_t__;
    __item_t__ __item__ () ध्रुव { प्रत्यागम get_glyph (); }

    hb_codepoint_t get_glyph () ध्रुव
    {
      विकल्प (format)
      {
      पक्ष 1: hb_barrier (); प्रत्यागम u.format1.get_glyph ();
      पक्ष 2: hb_barrier (); प्रत्यागम u.format2.get_glyph ();
#ifndef HB_NO_BEYOND_64K
      पक्ष 3: hb_barrier (); प्रत्यागम u.format3.get_glyph ();
      पक्ष 4: hb_barrier (); प्रत्यागम u.format4.get_glyph ();
#endif
      स्वभाव:प्रत्यागम 0;
      }
    }
    तार्किक operator != (ध्रुव iter_t& o) ध्रुव
    {
      यदि (unlikely (format != o.format)) प्रत्यागम सत्य;
      विकल्प (format)
      {
      पक्ष 1: hb_barrier (); प्रत्यागम u.format1 != o.u.format1;
      पक्ष 2: hb_barrier (); प्रत्यागम u.format2 != o.u.format2;
#ifndef HB_NO_BEYOND_64K
      पक्ष 3: hb_barrier (); प्रत्यागम u.format3 != o.u.format3;
      पक्ष 4: hb_barrier (); प्रत्यागम u.format4 != o.u.format4;
#endif
      स्वभाव:प्रत्यागम असत्य;
      }
    }
    iter_t __end__ () ध्रुव
    {
      iter_t it;
      it.format = format;
      विकल्प (format)
      {
      पक्ष 1: hb_barrier (); it.u.format1 = u.format1.__end__ (); भङ्ग;
      पक्ष 2: hb_barrier (); it.u.format2 = u.format2.__end__ (); भङ्ग;
#ifndef HB_NO_BEYOND_64K
      पक्ष 3: hb_barrier (); it.u.format3 = u.format3.__end__ (); भङ्ग;
      पक्ष 4: hb_barrier (); it.u.format4 = u.format4.__end__ (); भङ्ग;
#endif
      स्वभाव: भङ्ग;
      }
      प्रत्यागम it;
    }

    गुप्त:
    unsigned पूर्णाङ्क format;
    सङ्घ {
#ifndef HB_NO_BEYOND_64K
    CoverageFormat2_4<MediumTypes>::iter_t      format4; /* Put this one first since it's larger; helps shut up compiler. */
    CoverageFormat1_3<MediumTypes>::iter_t      format3;
#endif
    CoverageFormat2_4<SmallTypes>::iter_t       format2; /* Put this one first since it's larger; helps shut up compiler. */
    CoverageFormat1_3<SmallTypes>::iter_t       format1;
    } u;
  };
  iter_t iter () ध्रुव { प्रत्यागम iter_t (*एतत्); }
};

template<typename Iterator>
स्थिर अन्तःस्थ शून्य
Coverage_serialize (hb_serialize_context_t *c,
                    Iterator it)
{ c->start_embed<Coverage> ()->serialize (c, it); }

}
}
}

#endif  // #ifndef OT_LAYOUT_COMMON_COVERAGE_HH

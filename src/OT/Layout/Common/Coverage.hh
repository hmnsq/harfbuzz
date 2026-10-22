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

#ضمن "../../../hb-ot-dual.hh"
#ضمن "CoverageFormat1.hh"
#ضمن "CoverageFormat2.hh"

نطاق OT {
نطاق Layout {
نطاق Common {

template<typename Iterator>
ساكن مضمن فراغ Coverage_serialize (hb_serialize_context_t *c,
                                       Iterator it);

بنية Coverage
{

  محمي:
  اتحاد {
  بنية { HBUINT16 v; }        format;         /* Format identifier */
  CoverageFormat1_3<SmallTypes> format1;
  CoverageFormat2_4<SmallTypes> format2;
#ifndef HB_NO_BEYOND_64K
  CoverageFormat1_3<MediumTypes>format3;
  CoverageFormat2_4<MediumTypes>format4;
#endif
  } u;
  عام:
  DEFINE_SIZE_UNION (2, format.v);

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    𐡄𐡍 (!u.format.v.sanitize (c)) return_trace (خطأ);
    hb_barrier ();
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    حالة 2: hb_barrier (); return_trace (u.format2.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    حالة 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    افتراضي:return_trace (𐡒𐡔𐡈);
    }
  }

  /* Has interface. */
  unsigned operator [] (hb_codepoint_t k) ثابت { أرجع get (k); }
  منطقي has (hb_codepoint_t k) ثابت { أرجع (*هذا)[k] != NOT_COVERED; }
  /* Predicate. */
  منطقي operator () (hb_codepoint_t k) ثابت { أرجع has (k); }

  unsigned صحيح get (hb_codepoint_t k) ثابت { أرجع get_coverage (k); }
  unsigned صحيح get_coverage (hb_codepoint_t glyph_id) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.get_coverage (glyph_id);
    حالة 2: hb_barrier (); أرجع u.format2.get_coverage (glyph_id);
#ifndef HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.get_coverage (glyph_id);
    حالة 4: hb_barrier (); أرجع u.format4.get_coverage (glyph_id);
#endif
    افتراضي:أرجع NOT_COVERED;
    }
  }
  unsigned صحيح get_coverage (hb_codepoint_t glyph_id,
			     hb_ot_layout_mapping_cache_t *cache) ثابت
  {
    unsigned coverage;
    𐡄𐡍 (cache && cache->get (glyph_id, &coverage)) أرجع coverage < cache->MAX_VALUE ? coverage : NOT_COVERED;
    coverage = get_coverage (glyph_id);
    𐡄𐡍 (cache) {
      𐡄𐡍 (coverage == NOT_COVERED)
	cache->set (glyph_id, cache->MAX_VALUE);
      وإلا 𐡄𐡍 (likely (coverage < cache->MAX_VALUE))
	cache->set (glyph_id, coverage);
    }
    أرجع coverage;
  }

  unsigned صحيح get_coverage_binary (hb_codepoint_t glyph_id,
				    hb_ot_layout_binary_cache_t *cache) ثابت
  {
    unsigned coverage;
    𐡄𐡍 (cache && cache->get (glyph_id, &coverage)) أرجع coverage < cache->MAX_VALUE ? coverage : NOT_COVERED;
    coverage = get_coverage (glyph_id);
    𐡄𐡍 (cache) {
      𐡄𐡍 (coverage == NOT_COVERED)
	cache->set (glyph_id, cache->MAX_VALUE);
      وإلا
	cache->set (glyph_id, 0);
    }
    أرجع coverage;
  }

  unsigned get_population () ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.get_population ();
    حالة 2: hb_barrier (); أرجع u.format2.get_population ();
#ifndef HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.get_population ();
    حالة 4: hb_barrier (); أرجع u.format4.get_population ();
#endif
    افتراضي:أرجع NOT_COVERED;
    }
  }

  template <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (هذا);
    𐡄𐡍 (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    unsigned count = hb_len (+ glyphs);
    unsigned num_ranges = 0;
    hb_codepoint_t last = (hb_codepoint_t) -2;
    hb_codepoint_t max = 0;
    منطقي unsorted = خطأ;
    لكل (auto g: + glyphs)
    {
      𐡄𐡍 (last != (hb_codepoint_t) -2 && g < last)
	unsorted = 𐡒𐡔𐡈;
      𐡄𐡍 (last + 1 != g)
	num_ranges++;
      last = g;
      𐡄𐡍 (g > max) max = g;
    }
    u.format.v = !unsorted && count <= num_ranges * 3 ? 1 : 2;

#ifndef HB_NO_BEYOND_64K
    𐡄𐡍 (max > 0xFFFFu)
      u.format.v += 2;
    𐡄𐡍 (unlikely (max > 0xFFFFFFu))
#وإلا
    𐡄𐡍 (unlikely (max > 0xFFFFu))
#endif
    {
      c->check_success (خطأ, HB_SERIALIZE_ERROR_INT_OVERFLOW);
      return_trace (خطأ);
    }

    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); return_trace (u.format1.serialize (c, glyphs));
    حالة 2: hb_barrier (); return_trace (u.format2.serialize (c, glyphs));
#ifndef HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); return_trace (u.format3.serialize (c, glyphs));
    حالة 4: hb_barrier (); return_trace (u.format4.serialize (c, glyphs));
#endif
    افتراضي:return_trace (خطأ);
    }
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_subset_plan_t *plan = c->plan;
    auto it =
    + iter ()
    | hb_take (plan->source->get_num_glyphs ())
    | hb_map_retains_sorting ([plan] (hb_codepoint_t g) {
	أرجع plan->map_gsub_glyph (g);
      })
    | hb_filter ([] (hb_codepoint_t glyph) { أرجع glyph != HB_MAP_VALUE_INVALID; })
    ;

    // Cache the iterator result as it will be iterated multiple times
    // by the serialize code below.
    hb_sorted_vector_t<hb_codepoint_t> glyphs (it);
    Coverage_serialize (c->serializer, glyphs.iter ());
    return_trace (منطقي (glyphs));
  }

  منطقي intersects (ثابت hb_set_t *glyphs) ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); أرجع u.format1.intersects (glyphs);
    حالة 2: hb_barrier (); أرجع u.format2.intersects (glyphs);
#ifndef HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.intersects (glyphs);
    حالة 4: hb_barrier (); أرجع u.format4.intersects (glyphs);
#endif
    افتراضي:أرجع خطأ;
    }
  }
  منطقي intersects_coverage (ثابت hb_set_t *glyphs, unsigned صحيح index) ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); أرجع u.format1.intersects_coverage (glyphs, index);
    حالة 2: hb_barrier (); أرجع u.format2.intersects_coverage (glyphs, index);
#ifndef HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.intersects_coverage (glyphs, index);
    حالة 4: hb_barrier (); أرجع u.format4.intersects_coverage (glyphs, index);
#endif
    افتراضي:أرجع خطأ;
    }
  }

  unsigned cost () ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.cost ();
    حالة 2: hb_barrier (); أرجع u.format2.cost ();
#ifndef HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.cost ();
    حالة 4: hb_barrier (); أرجع u.format4.cost ();
#endif
    افتراضي:أرجع 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename set_t>
  منطقي collect_coverage (set_t *glyphs) ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); أرجع u.format1.collect_coverage (glyphs);
    حالة 2: hb_barrier (); أرجع u.format2.collect_coverage (glyphs);
#ifndef HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.collect_coverage (glyphs);
    حالة 4: hb_barrier (); أرجع u.format4.collect_coverage (glyphs);
#endif
    افتراضي:أرجع خطأ;
    }
  }

  template <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  فراغ intersect_set (ثابت hb_set_t &glyphs, IterableOut&& intersect_glyphs) ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); أرجع u.format1.intersect_set (glyphs, intersect_glyphs);
    حالة 2: hb_barrier (); أرجع u.format2.intersect_set (glyphs, intersect_glyphs);
#ifndef HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.intersect_set (glyphs, intersect_glyphs);
    حالة 4: hb_barrier (); أرجع u.format4.intersect_set (glyphs, intersect_glyphs);
#endif
    افتراضي:أرجع ;
    }
  }

  بنية iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ساكن constexpr منطقي is_sorted_iterator = 𐡒𐡔𐡈;
    iter_t (ثابت Coverage &c_ = Null (Coverage))
    {
      hb_memset (هذا, 0, حجم (*هذا));
      format = c_.u.format.v;
      بدل (format)
      {
      حالة 1: hb_barrier (); u.format1.init (c_.u.format1); أرجع;
      حالة 2: hb_barrier (); u.format2.init (c_.u.format2); أرجع;
#ifndef HB_NO_BEYOND_64K
      حالة 3: hb_barrier (); u.format3.init (c_.u.format3); أرجع;
      حالة 4: hb_barrier (); u.format4.init (c_.u.format4); أرجع;
#endif
      افتراضي:                               أرجع;
      }
    }
    منطقي __more__ () ثابت
    {
      بدل (format)
      {
      حالة 1: hb_barrier (); أرجع u.format1.__more__ ();
      حالة 2: hb_barrier (); أرجع u.format2.__more__ ();
#ifndef HB_NO_BEYOND_64K
      حالة 3: hb_barrier (); أرجع u.format3.__more__ ();
      حالة 4: hb_barrier (); أرجع u.format4.__more__ ();
#endif
      افتراضي:أرجع خطأ;
      }
    }
    فراغ __next__ ()
    {
      بدل (format)
      {
      حالة 1: hb_barrier (); u.format1.__next__ (); اكسر;
      حالة 2: hb_barrier (); u.format2.__next__ (); اكسر;
#ifndef HB_NO_BEYOND_64K
      حالة 3: hb_barrier (); u.format3.__next__ (); اكسر;
      حالة 4: hb_barrier (); u.format4.__next__ (); اكسر;
#endif
      افتراضي:                   اكسر;
      }
    }
    تعريفنوع hb_codepoint_t __item_t__;
    __item_t__ __item__ () ثابت { أرجع get_glyph (); }

    hb_codepoint_t get_glyph () ثابت
    {
      بدل (format)
      {
      حالة 1: hb_barrier (); أرجع u.format1.get_glyph ();
      حالة 2: hb_barrier (); أرجع u.format2.get_glyph ();
#ifndef HB_NO_BEYOND_64K
      حالة 3: hb_barrier (); أرجع u.format3.get_glyph ();
      حالة 4: hb_barrier (); أرجع u.format4.get_glyph ();
#endif
      افتراضي:أرجع 0;
      }
    }
    منطقي operator != (ثابت iter_t& o) ثابت
    {
      𐡄𐡍 (unlikely (format != o.format)) أرجع 𐡒𐡔𐡈;
      بدل (format)
      {
      حالة 1: hb_barrier (); أرجع u.format1 != o.u.format1;
      حالة 2: hb_barrier (); أرجع u.format2 != o.u.format2;
#ifndef HB_NO_BEYOND_64K
      حالة 3: hb_barrier (); أرجع u.format3 != o.u.format3;
      حالة 4: hb_barrier (); أرجع u.format4 != o.u.format4;
#endif
      افتراضي:أرجع خطأ;
      }
    }
    iter_t __end__ () ثابت
    {
      iter_t it;
      it.format = format;
      بدل (format)
      {
      حالة 1: hb_barrier (); it.u.format1 = u.format1.__end__ (); اكسر;
      حالة 2: hb_barrier (); it.u.format2 = u.format2.__end__ (); اكسر;
#ifndef HB_NO_BEYOND_64K
      حالة 3: hb_barrier (); it.u.format3 = u.format3.__end__ (); اكسر;
      حالة 4: hb_barrier (); it.u.format4 = u.format4.__end__ (); اكسر;
#endif
      افتراضي: اكسر;
      }
      أرجع it;
    }

    خاص:
    unsigned صحيح format;
    اتحاد {
#ifndef HB_NO_BEYOND_64K
    CoverageFormat2_4<MediumTypes>::iter_t      format4; /* Put this one first since it's larger; helps shut up compiler. */
    CoverageFormat1_3<MediumTypes>::iter_t      format3;
#endif
    CoverageFormat2_4<SmallTypes>::iter_t       format2; /* Put this one first since it's larger; helps shut up compiler. */
    CoverageFormat1_3<SmallTypes>::iter_t       format1;
    } u;
  };
  iter_t iter () ثابت { أرجع iter_t (*هذا); }
};

template<typename Iterator>
ساكن مضمن فراغ
Coverage_serialize (hb_serialize_context_t *c,
                    Iterator it)
{ c->start_embed<Coverage> ()->serialize (c, it); }

}
}
}

#endif  // #ifndef OT_LAYOUT_COMMON_COVERAGE_HH

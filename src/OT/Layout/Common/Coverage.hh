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

#شامل "../../../hb-ot-dual.hh"
#شامل "CoverageFormat1.hh"
#شامل "CoverageFormat2.hh"

فضاینام OT {
فضاینام Layout {
فضاینام Common {

template<typename Iterator>
ایستا درونخطی تهی Coverage_serialize (hb_serialize_context_t *c,
                                       Iterator it);

ساختار Coverage
{

  محافظتشده:
  اجتماع {
  ساختار { HBUINT16 v; }        format;         /* Format identifier */
  CoverageFormat1_3<SmallTypes> format1;
  CoverageFormat2_4<SmallTypes> format2;
#ifndef HB_NO_BEYOND_64K
  CoverageFormat1_3<MediumTypes>format3;
  CoverageFormat2_4<MediumTypes>format4;
#endif
  } u;
  عمومی:
  DEFINE_SIZE_UNION (2, format.v);

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (!u.format.v.sanitize (c)) return_trace (نادرست);
    hb_barrier ();
    انتخاب (u.format.v)
    {
    حالت 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    حالت 2: hb_barrier (); return_trace (u.format2.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    حالت 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    پیشفرض:return_trace (درست);
    }
  }

  /* Has interface. */
  unsigned operator [] (hb_codepoint_t k) ثابت { بازگشت get (k); }
  منطقی has (hb_codepoint_t k) ثابت { بازگشت (*این)[k] != NOT_COVERED; }
  /* Predicate. */
  منطقی operator () (hb_codepoint_t k) ثابت { بازگشت has (k); }

  unsigned صحیح get (hb_codepoint_t k) ثابت { بازگشت get_coverage (k); }
  unsigned صحیح get_coverage (hb_codepoint_t glyph_id) ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); بازگشت u.format1.get_coverage (glyph_id);
    حالت 2: hb_barrier (); بازگشت u.format2.get_coverage (glyph_id);
#ifndef HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); بازگشت u.format3.get_coverage (glyph_id);
    حالت 4: hb_barrier (); بازگشت u.format4.get_coverage (glyph_id);
#endif
    پیشفرض:بازگشت NOT_COVERED;
    }
  }
  unsigned صحیح get_coverage (hb_codepoint_t glyph_id,
			     hb_ot_layout_mapping_cache_t *cache) ثابت
  {
    unsigned coverage;
    اگر (cache && cache->get (glyph_id, &coverage)) بازگشت coverage < cache->MAX_مقدار ? coverage : NOT_COVERED;
    coverage = get_coverage (glyph_id);
    اگر (cache) {
      اگر (coverage == NOT_COVERED)
	cache->set (glyph_id, cache->MAX_مقدار);
      وگرنه اگر (likely (coverage < cache->MAX_مقدار))
	cache->set (glyph_id, coverage);
    }
    بازگشت coverage;
  }

  unsigned صحیح get_coverage_دودویی (hb_codepoint_t glyph_id,
				    hb_ot_layout_دودویی_cache_t *cache) ثابت
  {
    unsigned coverage;
    اگر (cache && cache->get (glyph_id, &coverage)) بازگشت coverage < cache->MAX_مقدار ? coverage : NOT_COVERED;
    coverage = get_coverage (glyph_id);
    اگر (cache) {
      اگر (coverage == NOT_COVERED)
	cache->set (glyph_id, cache->MAX_مقدار);
      وگرنه
	cache->set (glyph_id, 0);
    }
    بازگشت coverage;
  }

  unsigned get_population () ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); بازگشت u.format1.get_population ();
    حالت 2: hb_barrier (); بازگشت u.format2.get_population ();
#ifndef HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); بازگشت u.format3.get_population ();
    حالت 4: hb_barrier (); بازگشت u.format4.get_population ();
#endif
    پیشفرض:بازگشت NOT_COVERED;
    }
  }

  template <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  منطقی serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (این);
    اگر (unlikely (!c->extend_min (این))) return_trace (نادرست);

    unsigned شمار = hb_len (+ glyphs);
    unsigned رقم_ranges = 0;
    hb_codepoint_t last = (hb_codepoint_t) -2;
    hb_codepoint_t max = 0;
    منطقی unsorted = نادرست;
    برای (auto g: + glyphs)
    {
      اگر (last != (hb_codepoint_t) -2 && g < last)
	unsorted = درست;
      اگر (last + 1 != g)
	رقم_ranges++;
      last = g;
      اگر (g > max) max = g;
    }
    u.format.v = !unsorted && شمار <= رقم_ranges * 3 ? 1 : 2;

#ifndef HB_NO_BEYOND_64K
    اگر (max > 0xFFFFu)
      u.format.v += 2;
    اگر (unlikely (max > 0xFFFFFFu))
#وگرنه
    اگر (unlikely (max > 0xFFFFu))
#endif
    {
      c->check_success (نادرست, HB_SERIALIZE_ERROR_INT_OVERFLOW);
      return_trace (نادرست);
    }

    انتخاب (u.format.v)
    {
    حالت 1: hb_barrier (); return_trace (u.format1.serialize (c, glyphs));
    حالت 2: hb_barrier (); return_trace (u.format2.serialize (c, glyphs));
#ifndef HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); return_trace (u.format3.serialize (c, glyphs));
    حالت 4: hb_barrier (); return_trace (u.format4.serialize (c, glyphs));
#endif
    پیشفرض:return_trace (نادرست);
    }
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    ثابت hb_subset_plan_t *plan = c->plan;
    auto it =
    + iter ()
    | hb_take (plan->source->get_رقم_glyphs ())
    | hb_map_retains_sorting ([plan] (hb_codepoint_t g) {
	بازگشت plan->map_gsub_glyph (g);
      })
    | hb_filter ([] (hb_codepoint_t glyph) { بازگشت glyph != HB_MAP_مقدار_INVALID; })
    ;

    // Cache the iterator result as it will be iterated multiple times
    // by the serialize code below.
    hb_sorted_vector_t<hb_codepoint_t> glyphs (it);
    Coverage_serialize (c->serializer, glyphs.iter ());
    return_trace (منطقی (glyphs));
  }

  منطقی intersects (ثابت hb_set_t *glyphs) ثابت
  {
    انتخاب (u.format.v)
    {
    حالت 1: hb_barrier (); بازگشت u.format1.intersects (glyphs);
    حالت 2: hb_barrier (); بازگشت u.format2.intersects (glyphs);
#ifndef HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); بازگشت u.format3.intersects (glyphs);
    حالت 4: hb_barrier (); بازگشت u.format4.intersects (glyphs);
#endif
    پیشفرض:بازگشت نادرست;
    }
  }
  منطقی intersects_coverage (ثابت hb_set_t *glyphs, unsigned صحیح index) ثابت
  {
    انتخاب (u.format.v)
    {
    حالت 1: hb_barrier (); بازگشت u.format1.intersects_coverage (glyphs, index);
    حالت 2: hb_barrier (); بازگشت u.format2.intersects_coverage (glyphs, index);
#ifndef HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); بازگشت u.format3.intersects_coverage (glyphs, index);
    حالت 4: hb_barrier (); بازگشت u.format4.intersects_coverage (glyphs, index);
#endif
    پیشفرض:بازگشت نادرست;
    }
  }

  unsigned cost () ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); بازگشت u.format1.cost ();
    حالت 2: hb_barrier (); بازگشت u.format2.cost ();
#ifndef HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); بازگشت u.format3.cost ();
    حالت 4: hb_barrier (); بازگشت u.format4.cost ();
#endif
    پیشفرض:بازگشت 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename set_t>
  منطقی collect_coverage (set_t *glyphs) ثابت
  {
    انتخاب (u.format.v)
    {
    حالت 1: hb_barrier (); بازگشت u.format1.collect_coverage (glyphs);
    حالت 2: hb_barrier (); بازگشت u.format2.collect_coverage (glyphs);
#ifndef HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); بازگشت u.format3.collect_coverage (glyphs);
    حالت 4: hb_barrier (); بازگشت u.format4.collect_coverage (glyphs);
#endif
    پیشفرض:بازگشت نادرست;
    }
  }

  template <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  تهی intersect_set (ثابت hb_set_t &glyphs, IterableOut&& intersect_glyphs) ثابت
  {
    انتخاب (u.format.v)
    {
    حالت 1: hb_barrier (); بازگشت u.format1.intersect_set (glyphs, intersect_glyphs);
    حالت 2: hb_barrier (); بازگشت u.format2.intersect_set (glyphs, intersect_glyphs);
#ifndef HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); بازگشت u.format3.intersect_set (glyphs, intersect_glyphs);
    حالت 4: hb_barrier (); بازگشت u.format4.intersect_set (glyphs, intersect_glyphs);
#endif
    پیشفرض:بازگشت ;
    }
  }

  ساختار iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ایستا constexpr منطقی is_sorted_iterator = درست;
    iter_t (ثابت Coverage &c_ = Null (Coverage))
    {
      hb_memset (این, 0, اندازه (*این));
      format = c_.u.format.v;
      انتخاب (format)
      {
      حالت 1: hb_barrier (); u.format1.init (c_.u.format1); بازگشت;
      حالت 2: hb_barrier (); u.format2.init (c_.u.format2); بازگشت;
#ifndef HB_NO_BEYOND_64K
      حالت 3: hb_barrier (); u.format3.init (c_.u.format3); بازگشت;
      حالت 4: hb_barrier (); u.format4.init (c_.u.format4); بازگشت;
#endif
      پیشفرض:                               بازگشت;
      }
    }
    منطقی __more__ () ثابت
    {
      انتخاب (format)
      {
      حالت 1: hb_barrier (); بازگشت u.format1.__more__ ();
      حالت 2: hb_barrier (); بازگشت u.format2.__more__ ();
#ifndef HB_NO_BEYOND_64K
      حالت 3: hb_barrier (); بازگشت u.format3.__more__ ();
      حالت 4: hb_barrier (); بازگشت u.format4.__more__ ();
#endif
      پیشفرض:بازگشت نادرست;
      }
    }
    تهی __next__ ()
    {
      انتخاب (format)
      {
      حالت 1: hb_barrier (); u.format1.__next__ (); بشکن;
      حالت 2: hb_barrier (); u.format2.__next__ (); بشکن;
#ifndef HB_NO_BEYOND_64K
      حالت 3: hb_barrier (); u.format3.__next__ (); بشکن;
      حالت 4: hb_barrier (); u.format4.__next__ (); بشکن;
#endif
      پیشفرض:                   بشکن;
      }
    }
    تعریفنوع hb_codepoint_t __item_t__;
    __item_t__ __item__ () ثابت { بازگشت get_glyph (); }

    hb_codepoint_t get_glyph () ثابت
    {
      انتخاب (format)
      {
      حالت 1: hb_barrier (); بازگشت u.format1.get_glyph ();
      حالت 2: hb_barrier (); بازگشت u.format2.get_glyph ();
#ifndef HB_NO_BEYOND_64K
      حالت 3: hb_barrier (); بازگشت u.format3.get_glyph ();
      حالت 4: hb_barrier (); بازگشت u.format4.get_glyph ();
#endif
      پیشفرض:بازگشت 0;
      }
    }
    منطقی operator != (ثابت iter_t& o) ثابت
    {
      اگر (unlikely (format != o.format)) بازگشت درست;
      انتخاب (format)
      {
      حالت 1: hb_barrier (); بازگشت u.format1 != o.u.format1;
      حالت 2: hb_barrier (); بازگشت u.format2 != o.u.format2;
#ifndef HB_NO_BEYOND_64K
      حالت 3: hb_barrier (); بازگشت u.format3 != o.u.format3;
      حالت 4: hb_barrier (); بازگشت u.format4 != o.u.format4;
#endif
      پیشفرض:بازگشت نادرست;
      }
    }
    iter_t __end__ () ثابت
    {
      iter_t it;
      it.format = format;
      انتخاب (format)
      {
      حالت 1: hb_barrier (); it.u.format1 = u.format1.__end__ (); بشکن;
      حالت 2: hb_barrier (); it.u.format2 = u.format2.__end__ (); بشکن;
#ifndef HB_NO_BEYOND_64K
      حالت 3: hb_barrier (); it.u.format3 = u.format3.__end__ (); بشکن;
      حالت 4: hb_barrier (); it.u.format4 = u.format4.__end__ (); بشکن;
#endif
      پیشفرض: بشکن;
      }
      بازگشت it;
    }

    خصوصی:
    unsigned صحیح format;
    اجتماع {
#ifndef HB_NO_BEYOND_64K
    CoverageFormat2_4<MediumTypes>::iter_t      format4; /* Put this one first since it's larger; helps shut up compiler. */
    CoverageFormat1_3<MediumTypes>::iter_t      format3;
#endif
    CoverageFormat2_4<SmallTypes>::iter_t       format2; /* Put this one first since it's larger; helps shut up compiler. */
    CoverageFormat1_3<SmallTypes>::iter_t       format1;
    } u;
  };
  iter_t iter () ثابت { بازگشت iter_t (*این); }
};

template<typename Iterator>
ایستا درونخطی تهی
Coverage_serialize (hb_serialize_context_t *c,
                    Iterator it)
{ c->start_embed<Coverage> ()->serialize (c, it); }

}
}
}

#endif  // #ifndef OT_LAYOUT_COMMON_COVERAGE_HH

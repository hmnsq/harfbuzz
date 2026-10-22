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

#جے_بنا_تعریف OT_LAYOUT_COMMON_COVERAGE_HH
#تعریف_کرو OT_LAYOUT_COMMON_COVERAGE_HH

#ضمن "../../../hb-ot-dual.hh"
#ضمن "CoverageFormat1.hh"
#ضمن "CoverageFormat2.hh"

نطاق OT {
نطاق Layout {
نطاق Common {

سانچا<قسم_ناں Iterator>
ساكن مضمن فراغ Coverage_serialize (hb_serialize_context_t *c,
                                       Iterator it);

بنية Coverage
{

  محمي:
  اتحاد {
  بنية { HBUINT16 v; }        format;         /* Format identifier */
  CoverageFormat1_3<SmallTypes> format1;
  CoverageFormat2_4<SmallTypes> format2;
#جے_بنا_تعریف HB_NO_BEYOND_64K
  CoverageFormat1_3<MediumTypes>format3;
  CoverageFormat2_4<MediumTypes>format4;
#جے_انت
  } u;
  عام:
  DEFINE_ناپ_UNION (2, format.v);

#جے_بنا_تعریف HB_OPTIMIZE_ناپ
  HB_ALWAYS_INLINE
#جے_انت
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!u.format.v.sanitize (c)) return_trace (خطأ);
    hb_barrier ();
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    حالة 2: hb_barrier (); return_trace (u.format2.sanitize (c));
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    حالة 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#جے_انت
    افتراضي:return_trace (سچ);
    }
  }

  /* Has interface. */
  بنانشانی عامل [] (hb_codepoint_t k) ثابت { أرجع get (k); }
  منطقي has (hb_codepoint_t k) ثابت { أرجع (*هذا)[k] != NOT_COVERED; }
  /* Predicate. */
  منطقي عامل () (hb_codepoint_t k) ثابت { أرجع has (k); }

  بنانشانی صحيح get (hb_codepoint_t k) ثابت { أرجع get_coverage (k); }
  بنانشانی صحيح get_coverage (hb_codepoint_t glyph_id) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.get_coverage (glyph_id);
    حالة 2: hb_barrier (); أرجع u.format2.get_coverage (glyph_id);
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.get_coverage (glyph_id);
    حالة 4: hb_barrier (); أرجع u.format4.get_coverage (glyph_id);
#جے_انت
    افتراضي:أرجع NOT_COVERED;
    }
  }
  بنانشانی صحيح get_coverage (hb_codepoint_t glyph_id,
			     hb_ot_layout_mapping_cache_t *cache) ثابت
  {
    بنانشانی coverage;
    إذا (cache && cache->get (glyph_id, &coverage)) أرجع coverage < cache->MAX_قدر ? coverage : NOT_COVERED;
    coverage = get_coverage (glyph_id);
    إذا (cache) {
      إذا (coverage == NOT_COVERED)
	cache->سیٹ (glyph_id, cache->MAX_قدر);
      وإلا إذا (likely (coverage < cache->MAX_قدر))
	cache->سیٹ (glyph_id, coverage);
    }
    أرجع coverage;
  }

  بنانشانی صحيح get_coverage_ثنائی (hb_codepoint_t glyph_id,
				    hb_ot_layout_ثنائی_cache_t *cache) ثابت
  {
    بنانشانی coverage;
    إذا (cache && cache->get (glyph_id, &coverage)) أرجع coverage < cache->MAX_قدر ? coverage : NOT_COVERED;
    coverage = get_coverage (glyph_id);
    إذا (cache) {
      إذا (coverage == NOT_COVERED)
	cache->سیٹ (glyph_id, cache->MAX_قدر);
      وإلا
	cache->سیٹ (glyph_id, 0);
    }
    أرجع coverage;
  }

  بنانشانی get_population () ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.get_population ();
    حالة 2: hb_barrier (); أرجع u.format2.get_population ();
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.get_population ();
    حالة 4: hb_barrier (); أرجع u.format4.get_population ();
#جے_انت
    افتراضي:أرجع NOT_COVERED;
    }
  }

  سانچا <قسم_ناں Iterator,
      hb_requires (hb_is_sorted_سروت_دا (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    بنانشانی گنتی = hb_len (+ glyphs);
    بنانشانی عدد_ranges = 0;
    hb_codepoint_t آخری = (hb_codepoint_t) -2;
    hb_codepoint_t max = 0;
    منطقي unsorted = خطأ;
    لكل (خودکار g: + glyphs)
    {
      إذا (آخری != (hb_codepoint_t) -2 && g < آخری)
	unsorted = سچ;
      إذا (آخری + 1 != g)
	عدد_ranges++;
      آخری = g;
      إذا (g > max) max = g;
    }
    u.format.v = !unsorted && گنتی <= عدد_ranges * 3 ? 1 : 2;

#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (max > 0xFFFFu)
      u.format.v += 2;
    إذا (unlikely (max > 0xFFFFFFu))
#وإلا
    إذا (unlikely (max > 0xFFFFu))
#جے_انت
    {
      c->check_success (خطأ, HB_SERIALIZE_ERROR_INT_حد_توں_ودھ);
      return_trace (خطأ);
    }

    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); return_trace (u.format1.serialize (c, glyphs));
    حالة 2: hb_barrier (); return_trace (u.format2.serialize (c, glyphs));
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); return_trace (u.format3.serialize (c, glyphs));
    حالة 4: hb_barrier (); return_trace (u.format4.serialize (c, glyphs));
#جے_انت
    افتراضي:return_trace (خطأ);
    }
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_subset_plan_t *plan = c->plan;
    خودکار it =
    + iter ()
    | hb_take (plan->سروت->get_عدد_glyphs ())
    | hb_map_retains_چھانٹی ([plan] (hb_codepoint_t g) {
	أرجع plan->map_gsub_glyph (g);
      })
    | hb_filter ([] (hb_codepoint_t glyph) { أرجع glyph != HB_MAP_قدر_INVALID; })
    ;

    // Cache the iterator result as it will be iterated multiple times
    // by the serialize code below.
    hb_sorted_سمتیہ_t<hb_codepoint_t> glyphs (it);
    Coverage_serialize (c->serializer, glyphs.iter ());
    return_trace (منطقي (glyphs));
  }

  منطقي intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); أرجع u.format1.intersects (glyphs);
    حالة 2: hb_barrier (); أرجع u.format2.intersects (glyphs);
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.intersects (glyphs);
    حالة 4: hb_barrier (); أرجع u.format4.intersects (glyphs);
#جے_انت
    افتراضي:أرجع خطأ;
    }
  }
  منطقي intersects_coverage (ثابت hb_سیٹ_t *glyphs, بنانشانی صحيح اشاریہ_2) ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); أرجع u.format1.intersects_coverage (glyphs, اشاریہ_2);
    حالة 2: hb_barrier (); أرجع u.format2.intersects_coverage (glyphs, اشاریہ_2);
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.intersects_coverage (glyphs, اشاریہ_2);
    حالة 4: hb_barrier (); أرجع u.format4.intersects_coverage (glyphs, اشاریہ_2);
#جے_انت
    افتراضي:أرجع خطأ;
    }
  }

  بنانشانی لاگت () ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.لاگت ();
    حالة 2: hb_barrier (); أرجع u.format2.لاگت ();
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.لاگت ();
    حالة 4: hb_barrier (); أرجع u.format4.لاگت ();
#جے_انت
    افتراضي:أرجع 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  سانچا <قسم_ناں سیٹ_t>
  منطقي collect_coverage (سیٹ_t *glyphs) ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); أرجع u.format1.collect_coverage (glyphs);
    حالة 2: hb_barrier (); أرجع u.format2.collect_coverage (glyphs);
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.collect_coverage (glyphs);
    حالة 4: hb_barrier (); أرجع u.format4.collect_coverage (glyphs);
#جے_انت
    افتراضي:أرجع خطأ;
    }
  }

  سانچا <قسم_ناں Iterable_باہرلا,
	    hb_requires (hb_is_sink_دا (Iterable_باہرلا, hb_codepoint_t))>
  فراغ intersect_سیٹ (ثابت hb_سیٹ_t &glyphs, Iterable_باہرلا&& intersect_glyphs) ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); أرجع u.format1.intersect_سیٹ (glyphs, intersect_glyphs);
    حالة 2: hb_barrier (); أرجع u.format2.intersect_سیٹ (glyphs, intersect_glyphs);
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.intersect_سیٹ (glyphs, intersect_glyphs);
    حالة 4: hb_barrier (); أرجع u.format4.intersect_سیٹ (glyphs, intersect_glyphs);
#جے_انت
    افتراضي:أرجع ;
    }
  }

  بنية iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ساكن constexpr منطقي is_sorted_iterator = سچ;
    iter_t (ثابت Coverage &c_ = Null (Coverage))
    {
      hb_memset (هذا, 0, حجم (*هذا));
      format = c_.u.format.v;
      بدل (format)
      {
      حالة 1: hb_barrier (); u.format1.init (c_.u.format1); أرجع;
      حالة 2: hb_barrier (); u.format2.init (c_.u.format2); أرجع;
#جے_بنا_تعریف HB_NO_BEYOND_64K
      حالة 3: hb_barrier (); u.format3.init (c_.u.format3); أرجع;
      حالة 4: hb_barrier (); u.format4.init (c_.u.format4); أرجع;
#جے_انت
      افتراضي:                               أرجع;
      }
    }
    منطقي __more__ () ثابت
    {
      بدل (format)
      {
      حالة 1: hb_barrier (); أرجع u.format1.__more__ ();
      حالة 2: hb_barrier (); أرجع u.format2.__more__ ();
#جے_بنا_تعریف HB_NO_BEYOND_64K
      حالة 3: hb_barrier (); أرجع u.format3.__more__ ();
      حالة 4: hb_barrier (); أرجع u.format4.__more__ ();
#جے_انت
      افتراضي:أرجع خطأ;
      }
    }
    فراغ __next__ ()
    {
      بدل (format)
      {
      حالة 1: hb_barrier (); u.format1.__next__ (); اكسر;
      حالة 2: hb_barrier (); u.format2.__next__ (); اكسر;
#جے_بنا_تعریف HB_NO_BEYOND_64K
      حالة 3: hb_barrier (); u.format3.__next__ (); اكسر;
      حالة 4: hb_barrier (); u.format4.__next__ (); اكسر;
#جے_انت
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
#جے_بنا_تعریف HB_NO_BEYOND_64K
      حالة 3: hb_barrier (); أرجع u.format3.get_glyph ();
      حالة 4: hb_barrier (); أرجع u.format4.get_glyph ();
#جے_انت
      افتراضي:أرجع 0;
      }
    }
    منطقي عامل != (ثابت iter_t& o) ثابت
    {
      إذا (unlikely (format != o.format)) أرجع سچ;
      بدل (format)
      {
      حالة 1: hb_barrier (); أرجع u.format1 != o.u.format1;
      حالة 2: hb_barrier (); أرجع u.format2 != o.u.format2;
#جے_بنا_تعریف HB_NO_BEYOND_64K
      حالة 3: hb_barrier (); أرجع u.format3 != o.u.format3;
      حالة 4: hb_barrier (); أرجع u.format4 != o.u.format4;
#جے_انت
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
#جے_بنا_تعریف HB_NO_BEYOND_64K
      حالة 3: hb_barrier (); it.u.format3 = u.format3.__end__ (); اكسر;
      حالة 4: hb_barrier (); it.u.format4 = u.format4.__end__ (); اكسر;
#جے_انت
      افتراضي: اكسر;
      }
      أرجع it;
    }

    خاص:
    بنانشانی صحيح format;
    اتحاد {
#جے_بنا_تعریف HB_NO_BEYOND_64K
    CoverageFormat2_4<MediumTypes>::iter_t      format4; /* Put this one first since it's larger; helps shut up compiler. */
    CoverageFormat1_3<MediumTypes>::iter_t      format3;
#جے_انت
    CoverageFormat2_4<SmallTypes>::iter_t       format2; /* Put this one first since it's larger; helps shut up compiler. */
    CoverageFormat1_3<SmallTypes>::iter_t       format1;
    } u;
  };
  iter_t iter () ثابت { أرجع iter_t (*هذا); }
};

سانچا<قسم_ناں Iterator>
ساكن مضمن فراغ
Coverage_serialize (hb_serialize_context_t *c,
                    Iterator it)
{ c->شروع_embed<Coverage> ()->serialize (c, it); }

}
}
}

#جے_انت  // #ifndef OT_LAYOUT_COMMON_COVERAGE_HH

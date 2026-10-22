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

#که_نه_تعریف_شوی OT_LAYOUT_COMMON_COVERAGE_HH
#تعریف_کړه OT_LAYOUT_COMMON_COVERAGE_HH

#ضمن "../../../hb-ot-dual.hh"
#ضمن "CoverageFormat1.hh"
#ضمن "CoverageFormat2.hh"

نطاق OT {
نطاق Layout {
نطاق Common {

کينډۍ<ډول_نوم Iterator>
ساكن مضمن فراغ Coverage_serialize (hb_serialize_context_t *c,
                                       Iterator it);

بنية Coverage
{

  محمي:
  اتحاد {
  بنية { HBUINT16 v; }        format;         /* Format identifier */
  CoverageFormat1_3<SmallTypes> format1;
  CoverageFormat2_4<SmallTypes> format2;
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  CoverageFormat1_3<MediumTypes>format3;
  CoverageFormat2_4<MediumTypes>format4;
#د_که_پای
  } u;
  عام:
  DEFINE_اندازه_UNION (2, format.v);

#که_نه_تعریف_شوی HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#د_که_پای
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!u.format.v.sanitize (c)) return_trace (خطأ);
    hb_barrier ();
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    حالة 2: hb_barrier (); return_trace (u.format2.sanitize (c));
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    حالة 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#د_که_پای
    افتراضي:return_trace (ریښتیا);
    }
  }

  /* Has interface. */
  بینښه چلوونکی [] (hb_codepoint_t k) ثابت { أرجع get (k); }
  منطقي has (hb_codepoint_t k) ثابت { أرجع (*هذا)[k] != NOT_COVERED; }
  /* Predicate. */
  منطقي چلوونکی () (hb_codepoint_t k) ثابت { أرجع has (k); }

  بینښه صحيح get (hb_codepoint_t k) ثابت { أرجع get_coverage (k); }
  بینښه صحيح get_coverage (hb_codepoint_t glyph_id) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.get_coverage (glyph_id);
    حالة 2: hb_barrier (); أرجع u.format2.get_coverage (glyph_id);
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.get_coverage (glyph_id);
    حالة 4: hb_barrier (); أرجع u.format4.get_coverage (glyph_id);
#د_که_پای
    افتراضي:أرجع NOT_COVERED;
    }
  }
  بینښه صحيح get_coverage (hb_codepoint_t glyph_id,
			     hb_ot_layout_mapping_cache_t *cache) ثابت
  {
    بینښه coverage;
    إذا (cache && cache->get (glyph_id, &coverage)) أرجع coverage < cache->MAX_ارزښت ? coverage : NOT_COVERED;
    coverage = get_coverage (glyph_id);
    إذا (cache) {
      إذا (coverage == NOT_COVERED)
	cache->سېټ (glyph_id, cache->MAX_ارزښت);
      وإلا إذا (likely (coverage < cache->MAX_ارزښت))
	cache->سېټ (glyph_id, coverage);
    }
    أرجع coverage;
  }

  بینښه صحيح get_coverage_دوګونی (hb_codepoint_t glyph_id,
				    hb_ot_layout_دوګونی_cache_t *cache) ثابت
  {
    بینښه coverage;
    إذا (cache && cache->get (glyph_id, &coverage)) أرجع coverage < cache->MAX_ارزښت ? coverage : NOT_COVERED;
    coverage = get_coverage (glyph_id);
    إذا (cache) {
      إذا (coverage == NOT_COVERED)
	cache->سېټ (glyph_id, cache->MAX_ارزښت);
      وإلا
	cache->سېټ (glyph_id, 0);
    }
    أرجع coverage;
  }

  بینښه get_population () ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.get_population ();
    حالة 2: hb_barrier (); أرجع u.format2.get_population ();
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.get_population ();
    حالة 4: hb_barrier (); أرجع u.format4.get_population ();
#د_که_پای
    افتراضي:أرجع NOT_COVERED;
    }
  }

  کينډۍ <ډول_نوم Iterator,
      hb_requires (hb_is_sorted_سرچینه_د (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    بینښه شمېر = hb_len (+ glyphs);
    بینښه عدد_ranges = 0;
    hb_codepoint_t وروستی = (hb_codepoint_t) -2;
    hb_codepoint_t max = 0;
    منطقي unsorted = خطأ;
    لكل (خپلکاری g: + glyphs)
    {
      إذا (وروستی != (hb_codepoint_t) -2 && g < وروستی)
	unsorted = ریښتیا;
      إذا (وروستی + 1 != g)
	عدد_ranges++;
      وروستی = g;
      إذا (g > max) max = g;
    }
    u.format.v = !unsorted && شمېر <= عدد_ranges * 3 ? 1 : 2;

#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (max > 0xFFFFu)
      u.format.v += 2;
    إذا (unlikely (max > 0xFFFFFFu))
#وإلا
    إذا (unlikely (max > 0xFFFFu))
#د_که_پای
    {
      c->check_success (خطأ, HB_SERIALIZE_ERROR_INT_ډکېدنه);
      return_trace (خطأ);
    }

    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); return_trace (u.format1.serialize (c, glyphs));
    حالة 2: hb_barrier (); return_trace (u.format2.serialize (c, glyphs));
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); return_trace (u.format3.serialize (c, glyphs));
    حالة 4: hb_barrier (); return_trace (u.format4.serialize (c, glyphs));
#د_که_پای
    افتراضي:return_trace (خطأ);
    }
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_subset_plan_t *plan = c->plan;
    خپلکاری it =
    + iter ()
    | hb_take (plan->سرچینه->get_عدد_glyphs ())
    | hb_map_retains_ترتیبونه ([plan] (hb_codepoint_t g) {
	أرجع plan->map_gsub_glyph (g);
      })
    | hb_filter ([] (hb_codepoint_t glyph) { أرجع glyph != HB_MAP_ارزښت_INVALID; })
    ;

    // Cache the iterator result as it will be iterated multiple times
    // by the serialize code below.
    hb_sorted_ویکټور_t<hb_codepoint_t> glyphs (it);
    Coverage_serialize (c->serializer, glyphs.iter ());
    return_trace (منطقي (glyphs));
  }

  منطقي intersects (ثابت hb_سېټ_t *glyphs) ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); أرجع u.format1.intersects (glyphs);
    حالة 2: hb_barrier (); أرجع u.format2.intersects (glyphs);
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.intersects (glyphs);
    حالة 4: hb_barrier (); أرجع u.format4.intersects (glyphs);
#د_که_پای
    افتراضي:أرجع خطأ;
    }
  }
  منطقي intersects_coverage (ثابت hb_سېټ_t *glyphs, بینښه صحيح شاخص_2) ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); أرجع u.format1.intersects_coverage (glyphs, شاخص_2);
    حالة 2: hb_barrier (); أرجع u.format2.intersects_coverage (glyphs, شاخص_2);
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.intersects_coverage (glyphs, شاخص_2);
    حالة 4: hb_barrier (); أرجع u.format4.intersects_coverage (glyphs, شاخص_2);
#د_که_پای
    افتراضي:أرجع خطأ;
    }
  }

  بینښه لګښت () ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.لګښت ();
    حالة 2: hb_barrier (); أرجع u.format2.لګښت ();
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.لګښت ();
    حالة 4: hb_barrier (); أرجع u.format4.لګښت ();
#د_که_پای
    افتراضي:أرجع 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  کينډۍ <ډول_نوم سېټ_t>
  منطقي collect_coverage (سېټ_t *glyphs) ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); أرجع u.format1.collect_coverage (glyphs);
    حالة 2: hb_barrier (); أرجع u.format2.collect_coverage (glyphs);
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.collect_coverage (glyphs);
    حالة 4: hb_barrier (); أرجع u.format4.collect_coverage (glyphs);
#د_که_پای
    افتراضي:أرجع خطأ;
    }
  }

  کينډۍ <ډول_نوم Iterable_بهر,
	    hb_requires (hb_is_sink_د (Iterable_بهر, hb_codepoint_t))>
  فراغ intersect_سېټ (ثابت hb_سېټ_t &glyphs, Iterable_بهر&& intersect_glyphs) ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); أرجع u.format1.intersect_سېټ (glyphs, intersect_glyphs);
    حالة 2: hb_barrier (); أرجع u.format2.intersect_سېټ (glyphs, intersect_glyphs);
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.intersect_سېټ (glyphs, intersect_glyphs);
    حالة 4: hb_barrier (); أرجع u.format4.intersect_سېټ (glyphs, intersect_glyphs);
#د_که_پای
    افتراضي:أرجع ;
    }
  }

  بنية iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ساكن constexpr منطقي is_sorted_iterator = ریښتیا;
    iter_t (ثابت Coverage &c_ = Null (Coverage))
    {
      hb_memset (هذا, 0, حجم (*هذا));
      format = c_.u.format.v;
      بدل (format)
      {
      حالة 1: hb_barrier (); u.format1.init (c_.u.format1); أرجع;
      حالة 2: hb_barrier (); u.format2.init (c_.u.format2); أرجع;
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
      حالة 3: hb_barrier (); u.format3.init (c_.u.format3); أرجع;
      حالة 4: hb_barrier (); u.format4.init (c_.u.format4); أرجع;
#د_که_پای
      افتراضي:                               أرجع;
      }
    }
    منطقي __more__ () ثابت
    {
      بدل (format)
      {
      حالة 1: hb_barrier (); أرجع u.format1.__more__ ();
      حالة 2: hb_barrier (); أرجع u.format2.__more__ ();
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
      حالة 3: hb_barrier (); أرجع u.format3.__more__ ();
      حالة 4: hb_barrier (); أرجع u.format4.__more__ ();
#د_که_پای
      افتراضي:أرجع خطأ;
      }
    }
    فراغ __next__ ()
    {
      بدل (format)
      {
      حالة 1: hb_barrier (); u.format1.__next__ (); اكسر;
      حالة 2: hb_barrier (); u.format2.__next__ (); اكسر;
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
      حالة 3: hb_barrier (); u.format3.__next__ (); اكسر;
      حالة 4: hb_barrier (); u.format4.__next__ (); اكسر;
#د_که_پای
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
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
      حالة 3: hb_barrier (); أرجع u.format3.get_glyph ();
      حالة 4: hb_barrier (); أرجع u.format4.get_glyph ();
#د_که_پای
      افتراضي:أرجع 0;
      }
    }
    منطقي چلوونکی != (ثابت iter_t& o) ثابت
    {
      إذا (unlikely (format != o.format)) أرجع ریښتیا;
      بدل (format)
      {
      حالة 1: hb_barrier (); أرجع u.format1 != o.u.format1;
      حالة 2: hb_barrier (); أرجع u.format2 != o.u.format2;
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
      حالة 3: hb_barrier (); أرجع u.format3 != o.u.format3;
      حالة 4: hb_barrier (); أرجع u.format4 != o.u.format4;
#د_که_پای
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
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
      حالة 3: hb_barrier (); it.u.format3 = u.format3.__end__ (); اكسر;
      حالة 4: hb_barrier (); it.u.format4 = u.format4.__end__ (); اكسر;
#د_که_پای
      افتراضي: اكسر;
      }
      أرجع it;
    }

    خاص:
    بینښه صحيح format;
    اتحاد {
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    CoverageFormat2_4<MediumTypes>::iter_t      format4; /* Put this one first since it's larger; helps shut up compiler. */
    CoverageFormat1_3<MediumTypes>::iter_t      format3;
#د_که_پای
    CoverageFormat2_4<SmallTypes>::iter_t       format2; /* Put this one first since it's larger; helps shut up compiler. */
    CoverageFormat1_3<SmallTypes>::iter_t       format1;
    } u;
  };
  iter_t iter () ثابت { أرجع iter_t (*هذا); }
};

کينډۍ<ډول_نوم Iterator>
ساكن مضمن فراغ
Coverage_serialize (hb_serialize_context_t *c,
                    Iterator it)
{ c->پیل_embed<Coverage> ()->serialize (c, it); }

}
}
}

#د_که_پای  // #ifndef OT_LAYOUT_COMMON_COVERAGE_HH

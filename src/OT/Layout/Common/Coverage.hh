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

#اگر_غیر_تعریف_شدہ OT_LAYOUT_COMMON_COVERAGE_HH
#تعریف_کرو OT_LAYOUT_COMMON_COVERAGE_HH

#شامل "../../../hb-ot-dual.hh"
#شامل "CoverageFormat1.hh"
#شامل "CoverageFormat2.hh"

نامخلا OT {
نامخلا Layout {
نامخلا Common {

سانچہ<قسم_نام Iterator>
ساکن اندرونی خالی Coverage_serialize (hb_serialize_context_t *c,
                                       Iterator it);

ساخت Coverage
{

  محفوظ:
  اتحاد {
  ساخت { HBUINT16 v; }        format;         /* Format identifier */
  CoverageFormat1_3<SmallTypes> format1;
  CoverageFormat2_4<SmallTypes> format2;
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  CoverageFormat1_3<MediumTypes>format3;
  CoverageFormat2_4<MediumTypes>format4;
#اگر_ختم
  } u;
  عوامی:
  DEFINE_سائز_UNION (2, format.v);

#اگر_غیر_تعریف_شدہ HB_OPTIMIZE_سائز
  HB_ALWAYS_INLINE
#اگر_ختم
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (!u.format.v.sanitize (c)) return_trace (جھوٹ);
    hb_barrier ();
    انتخاب (u.format.v)
    {
    حالت 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    حالت 2: hb_barrier (); return_trace (u.format2.sanitize (c));
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    حالت 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#اگر_ختم
    طےشدہ:return_trace (سچ);
    }
  }

  /* Has interface. */
  غیرعلامتی عامل [] (hb_codepoint_t k) ثابت { واپس get (k); }
  منطقی has (hb_codepoint_t k) ثابت { واپس (*یہ)[k] != NOT_COVERED; }
  /* Predicate. */
  منطقی عامل () (hb_codepoint_t k) ثابت { واپس has (k); }

  غیرعلامتی صحیح get (hb_codepoint_t k) ثابت { واپس get_coverage (k); }
  غیرعلامتی صحیح get_coverage (hb_codepoint_t glyph_id) ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); واپس u.format1.get_coverage (glyph_id);
    حالت 2: hb_barrier (); واپس u.format2.get_coverage (glyph_id);
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); واپس u.format3.get_coverage (glyph_id);
    حالت 4: hb_barrier (); واپس u.format4.get_coverage (glyph_id);
#اگر_ختم
    طےشدہ:واپس NOT_COVERED;
    }
  }
  غیرعلامتی صحیح get_coverage (hb_codepoint_t glyph_id,
			     hb_ot_layout_mapping_cache_t *cache) ثابت
  {
    غیرعلامتی coverage;
    اگر (cache && cache->get (glyph_id, &coverage)) واپس coverage < cache->MAX_قدر ? coverage : NOT_COVERED;
    coverage = get_coverage (glyph_id);
    اگر (cache) {
      اگر (coverage == NOT_COVERED)
	cache->سیٹ (glyph_id, cache->MAX_قدر);
      ورنہ اگر (likely (coverage < cache->MAX_قدر))
	cache->سیٹ (glyph_id, coverage);
    }
    واپس coverage;
  }

  غیرعلامتی صحیح get_coverage_ثنائی (hb_codepoint_t glyph_id,
				    hb_ot_layout_ثنائی_cache_t *cache) ثابت
  {
    غیرعلامتی coverage;
    اگر (cache && cache->get (glyph_id, &coverage)) واپس coverage < cache->MAX_قدر ? coverage : NOT_COVERED;
    coverage = get_coverage (glyph_id);
    اگر (cache) {
      اگر (coverage == NOT_COVERED)
	cache->سیٹ (glyph_id, cache->MAX_قدر);
      ورنہ
	cache->سیٹ (glyph_id, 0);
    }
    واپس coverage;
  }

  غیرعلامتی get_population () ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); واپس u.format1.get_population ();
    حالت 2: hb_barrier (); واپس u.format2.get_population ();
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); واپس u.format3.get_population ();
    حالت 4: hb_barrier (); واپس u.format4.get_population ();
#اگر_ختم
    طےشدہ:واپس NOT_COVERED;
    }
  }

  سانچہ <قسم_نام Iterator,
      hb_requires (hb_is_sorted_ماخذ_کا (Iterator, hb_codepoint_t))>
  منطقی serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (یہ);
    اگر (unlikely (!c->extend_min (یہ))) return_trace (جھوٹ);

    غیرعلامتی گنتی = hb_len (+ glyphs);
    غیرعلامتی عدد_ranges = 0;
    hb_codepoint_t آخری = (hb_codepoint_t) -2;
    hb_codepoint_t max = 0;
    منطقی unsorted = جھوٹ;
    برائے (خودکار g: + glyphs)
    {
      اگر (آخری != (hb_codepoint_t) -2 && g < آخری)
	unsorted = سچ;
      اگر (آخری + 1 != g)
	عدد_ranges++;
      آخری = g;
      اگر (g > max) max = g;
    }
    u.format.v = !unsorted && گنتی <= عدد_ranges * 3 ? 1 : 2;

#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (max > 0xFFFFu)
      u.format.v += 2;
    اگر (unlikely (max > 0xFFFFFFu))
#ورنہ
    اگر (unlikely (max > 0xFFFFu))
#اگر_ختم
    {
      c->check_success (جھوٹ, HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز);
      return_trace (جھوٹ);
    }

    انتخاب (u.format.v)
    {
    حالت 1: hb_barrier (); return_trace (u.format1.serialize (c, glyphs));
    حالت 2: hb_barrier (); return_trace (u.format2.serialize (c, glyphs));
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); return_trace (u.format3.serialize (c, glyphs));
    حالت 4: hb_barrier (); return_trace (u.format4.serialize (c, glyphs));
#اگر_ختم
    طےشدہ:return_trace (جھوٹ);
    }
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    ثابت hb_subset_plan_t *plan = c->plan;
    خودکار it =
    + iter ()
    | hb_take (plan->ماخذ->get_عدد_glyphs ())
    | hb_map_retains_ترتیب_کاری ([plan] (hb_codepoint_t g) {
	واپس plan->map_gsub_glyph (g);
      })
    | hb_filter ([] (hb_codepoint_t glyph) { واپس glyph != HB_MAP_قدر_INVALID; })
    ;

    // Cache the iterator result as it will be iterated multiple times
    // by the serialize code below.
    hb_sorted_سمتیہ_t<hb_codepoint_t> glyphs (it);
    Coverage_serialize (c->serializer, glyphs.iter ());
    return_trace (منطقی (glyphs));
  }

  منطقی intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    انتخاب (u.format.v)
    {
    حالت 1: hb_barrier (); واپس u.format1.intersects (glyphs);
    حالت 2: hb_barrier (); واپس u.format2.intersects (glyphs);
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); واپس u.format3.intersects (glyphs);
    حالت 4: hb_barrier (); واپس u.format4.intersects (glyphs);
#اگر_ختم
    طےشدہ:واپس جھوٹ;
    }
  }
  منطقی intersects_coverage (ثابت hb_سیٹ_t *glyphs, غیرعلامتی صحیح اشاریہ_2) ثابت
  {
    انتخاب (u.format.v)
    {
    حالت 1: hb_barrier (); واپس u.format1.intersects_coverage (glyphs, اشاریہ_2);
    حالت 2: hb_barrier (); واپس u.format2.intersects_coverage (glyphs, اشاریہ_2);
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); واپس u.format3.intersects_coverage (glyphs, اشاریہ_2);
    حالت 4: hb_barrier (); واپس u.format4.intersects_coverage (glyphs, اشاریہ_2);
#اگر_ختم
    طےشدہ:واپس جھوٹ;
    }
  }

  غیرعلامتی لاگت () ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); واپس u.format1.لاگت ();
    حالت 2: hb_barrier (); واپس u.format2.لاگت ();
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); واپس u.format3.لاگت ();
    حالت 4: hb_barrier (); واپس u.format4.لاگت ();
#اگر_ختم
    طےشدہ:واپس 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  سانچہ <قسم_نام سیٹ_t>
  منطقی collect_coverage (سیٹ_t *glyphs) ثابت
  {
    انتخاب (u.format.v)
    {
    حالت 1: hb_barrier (); واپس u.format1.collect_coverage (glyphs);
    حالت 2: hb_barrier (); واپس u.format2.collect_coverage (glyphs);
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); واپس u.format3.collect_coverage (glyphs);
    حالت 4: hb_barrier (); واپس u.format4.collect_coverage (glyphs);
#اگر_ختم
    طےشدہ:واپس جھوٹ;
    }
  }

  سانچہ <قسم_نام Iterable_برآمد,
	    hb_requires (hb_is_sink_کا (Iterable_برآمد, hb_codepoint_t))>
  خالی intersect_سیٹ (ثابت hb_سیٹ_t &glyphs, Iterable_برآمد&& intersect_glyphs) ثابت
  {
    انتخاب (u.format.v)
    {
    حالت 1: hb_barrier (); واپس u.format1.intersect_سیٹ (glyphs, intersect_glyphs);
    حالت 2: hb_barrier (); واپس u.format2.intersect_سیٹ (glyphs, intersect_glyphs);
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); واپس u.format3.intersect_سیٹ (glyphs, intersect_glyphs);
    حالت 4: hb_barrier (); واپس u.format4.intersect_سیٹ (glyphs, intersect_glyphs);
#اگر_ختم
    طےشدہ:واپس ;
    }
  }

  ساخت iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ساکن constexpr منطقی is_sorted_iterator = سچ;
    iter_t (ثابت Coverage &c_ = Null (Coverage))
    {
      hb_memset (یہ, 0, حجم (*یہ));
      format = c_.u.format.v;
      انتخاب (format)
      {
      حالت 1: hb_barrier (); u.format1.init (c_.u.format1); واپس;
      حالت 2: hb_barrier (); u.format2.init (c_.u.format2); واپس;
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
      حالت 3: hb_barrier (); u.format3.init (c_.u.format3); واپس;
      حالت 4: hb_barrier (); u.format4.init (c_.u.format4); واپس;
#اگر_ختم
      طےشدہ:                               واپس;
      }
    }
    منطقی __more__ () ثابت
    {
      انتخاب (format)
      {
      حالت 1: hb_barrier (); واپس u.format1.__more__ ();
      حالت 2: hb_barrier (); واپس u.format2.__more__ ();
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
      حالت 3: hb_barrier (); واپس u.format3.__more__ ();
      حالت 4: hb_barrier (); واپس u.format4.__more__ ();
#اگر_ختم
      طےشدہ:واپس جھوٹ;
      }
    }
    خالی __next__ ()
    {
      انتخاب (format)
      {
      حالت 1: hb_barrier (); u.format1.__next__ (); توڑو;
      حالت 2: hb_barrier (); u.format2.__next__ (); توڑو;
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
      حالت 3: hb_barrier (); u.format3.__next__ (); توڑو;
      حالت 4: hb_barrier (); u.format4.__next__ (); توڑو;
#اگر_ختم
      طےشدہ:                   توڑو;
      }
    }
    قسمتعریف hb_codepoint_t __item_t__;
    __item_t__ __item__ () ثابت { واپس get_glyph (); }

    hb_codepoint_t get_glyph () ثابت
    {
      انتخاب (format)
      {
      حالت 1: hb_barrier (); واپس u.format1.get_glyph ();
      حالت 2: hb_barrier (); واپس u.format2.get_glyph ();
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
      حالت 3: hb_barrier (); واپس u.format3.get_glyph ();
      حالت 4: hb_barrier (); واپس u.format4.get_glyph ();
#اگر_ختم
      طےشدہ:واپس 0;
      }
    }
    منطقی عامل != (ثابت iter_t& o) ثابت
    {
      اگر (unlikely (format != o.format)) واپس سچ;
      انتخاب (format)
      {
      حالت 1: hb_barrier (); واپس u.format1 != o.u.format1;
      حالت 2: hb_barrier (); واپس u.format2 != o.u.format2;
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
      حالت 3: hb_barrier (); واپس u.format3 != o.u.format3;
      حالت 4: hb_barrier (); واپس u.format4 != o.u.format4;
#اگر_ختم
      طےشدہ:واپس جھوٹ;
      }
    }
    iter_t __end__ () ثابت
    {
      iter_t it;
      it.format = format;
      انتخاب (format)
      {
      حالت 1: hb_barrier (); it.u.format1 = u.format1.__end__ (); توڑو;
      حالت 2: hb_barrier (); it.u.format2 = u.format2.__end__ (); توڑو;
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
      حالت 3: hb_barrier (); it.u.format3 = u.format3.__end__ (); توڑو;
      حالت 4: hb_barrier (); it.u.format4 = u.format4.__end__ (); توڑو;
#اگر_ختم
      طےشدہ: توڑو;
      }
      واپس it;
    }

    نجی:
    غیرعلامتی صحیح format;
    اتحاد {
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    CoverageFormat2_4<MediumTypes>::iter_t      format4; /* Put this one first since it's larger; helps shut up compiler. */
    CoverageFormat1_3<MediumTypes>::iter_t      format3;
#اگر_ختم
    CoverageFormat2_4<SmallTypes>::iter_t       format2; /* Put this one first since it's larger; helps shut up compiler. */
    CoverageFormat1_3<SmallTypes>::iter_t       format1;
    } u;
  };
  iter_t iter () ثابت { واپس iter_t (*یہ); }
};

سانچہ<قسم_نام Iterator>
ساکن اندرونی خالی
Coverage_serialize (hb_serialize_context_t *c,
                    Iterator it)
{ c->آغاز_embed<Coverage> ()->serialize (c, it); }

}
}
}

#اگر_ختم  // #ifndef OT_LAYOUT_COMMON_COVERAGE_HH

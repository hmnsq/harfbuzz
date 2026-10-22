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

#অন্তর্ভাব "../../../hb-ot-dual.hh"
#অন্তর্ভাব "CoverageFormat1.hh"
#অন্তর্ভাব "CoverageFormat2.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ Common {

template<typename Iterator>
স্থির অন্তঃস্থ শূন্য Coverage_serialize (hb_serialize_context_t *c,
                                       Iterator it);

সংরচনা Coverage
{

  রক্ষিত:
  সঙ্ঘ {
  সংরচনা { HBUINT16 v; }        format;         /* Format identifier */
  CoverageFormat1_3<SmallTypes> format1;
  CoverageFormat2_4<SmallTypes> format2;
#ifndef HB_NO_BEYOND_64K
  CoverageFormat1_3<MediumTypes>format3;
  CoverageFormat2_4<MediumTypes>format4;
#endif
  } u;
  সার্বজনিক:
  DEFINE_SIZE_UNION (2, format.v);

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    যদি (!u.format.v.sanitize (c)) return_trace (অসত্য);
    hb_barrier ();
    বিকল্প (u.format.v)
    {
    পক্ষ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    পক্ষ 2: hb_barrier (); return_trace (u.format2.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    পক্ষ 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    পক্ষ 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    স্বভাব:return_trace (সত্য);
    }
  }

  /* Has interface. */
  unsigned operator [] (hb_codepoint_t k) ধ্রুব { প্রত্যাগম get (k); }
  তার্কিক has (hb_codepoint_t k) ধ্রুব { প্রত্যাগম (*এতত্)[k] != NOT_COVERED; }
  /* Predicate. */
  তার্কিক operator () (hb_codepoint_t k) ধ্রুব { প্রত্যাগম has (k); }

  unsigned পূর্ণাঙ্ক get (hb_codepoint_t k) ধ্রুব { প্রত্যাগম get_coverage (k); }
  unsigned পূর্ণাঙ্ক get_coverage (hb_codepoint_t glyph_id) ধ্রুব
  {
    বিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); প্রত্যাগম u.format1.get_coverage (glyph_id);
    পক্ষ 2: hb_barrier (); প্রত্যাগম u.format2.get_coverage (glyph_id);
#ifndef HB_NO_BEYOND_64K
    পক্ষ 3: hb_barrier (); প্রত্যাগম u.format3.get_coverage (glyph_id);
    পক্ষ 4: hb_barrier (); প্রত্যাগম u.format4.get_coverage (glyph_id);
#endif
    স্বভাব:প্রত্যাগম NOT_COVERED;
    }
  }
  unsigned পূর্ণাঙ্ক get_coverage (hb_codepoint_t glyph_id,
			     hb_ot_layout_mapping_cache_t *cache) ধ্রুব
  {
    unsigned coverage;
    যদি (cache && cache->get (glyph_id, &coverage)) প্রত্যাগম coverage < cache->MAX_মান ? coverage : NOT_COVERED;
    coverage = get_coverage (glyph_id);
    যদি (cache) {
      যদি (coverage == NOT_COVERED)
	cache->set (glyph_id, cache->MAX_মান);
      অন্যথা যদি (likely (coverage < cache->MAX_মান))
	cache->set (glyph_id, coverage);
    }
    প্রত্যাগম coverage;
  }

  unsigned পূর্ণাঙ্ক get_coverage_দ্বিমিক (hb_codepoint_t glyph_id,
				    hb_ot_layout_দ্বিমিক_cache_t *cache) ধ্রুব
  {
    unsigned coverage;
    যদি (cache && cache->get (glyph_id, &coverage)) প্রত্যাগম coverage < cache->MAX_মান ? coverage : NOT_COVERED;
    coverage = get_coverage (glyph_id);
    যদি (cache) {
      যদি (coverage == NOT_COVERED)
	cache->set (glyph_id, cache->MAX_মান);
      অন্যথা
	cache->set (glyph_id, 0);
    }
    প্রত্যাগম coverage;
  }

  unsigned get_population () ধ্রুব
  {
    বিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); প্রত্যাগম u.format1.get_population ();
    পক্ষ 2: hb_barrier (); প্রত্যাগম u.format2.get_population ();
#ifndef HB_NO_BEYOND_64K
    পক্ষ 3: hb_barrier (); প্রত্যাগম u.format3.get_population ();
    পক্ষ 4: hb_barrier (); প্রত্যাগম u.format4.get_population ();
#endif
    স্বভাব:প্রত্যাগম NOT_COVERED;
    }
  }

  template <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  তার্কিক serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (এতত্);
    যদি (unlikely (!c->extend_min (এতত্))) return_trace (অসত্য);

    unsigned গুনতি = hb_len (+ glyphs);
    unsigned অঙ্ক_ranges = 0;
    hb_codepoint_t last = (hb_codepoint_t) -2;
    hb_codepoint_t max = 0;
    তার্কিক unsorted = অসত্য;
    প্রতি (auto g: + glyphs)
    {
      যদি (last != (hb_codepoint_t) -2 && g < last)
	unsorted = সত্য;
      যদি (last + 1 != g)
	অঙ্ক_ranges++;
      last = g;
      যদি (g > max) max = g;
    }
    u.format.v = !unsorted && গুনতি <= অঙ্ক_ranges * 3 ? 1 : 2;

#ifndef HB_NO_BEYOND_64K
    যদি (max > 0xFFFFu)
      u.format.v += 2;
    যদি (unlikely (max > 0xFFFFFFu))
#অন্যথা
    যদি (unlikely (max > 0xFFFFu))
#endif
    {
      c->check_success (অসত্য, HB_SERIALIZE_ERROR_INT_OVERFLOW);
      return_trace (অসত্য);
    }

    বিকল্প (u.format.v)
    {
    পক্ষ 1: hb_barrier (); return_trace (u.format1.serialize (c, glyphs));
    পক্ষ 2: hb_barrier (); return_trace (u.format2.serialize (c, glyphs));
#ifndef HB_NO_BEYOND_64K
    পক্ষ 3: hb_barrier (); return_trace (u.format3.serialize (c, glyphs));
    পক্ষ 4: hb_barrier (); return_trace (u.format4.serialize (c, glyphs));
#endif
    স্বভাব:return_trace (অসত্য);
    }
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    ধ্রুব hb_subset_plan_t *plan = c->plan;
    auto it =
    + iter ()
    | hb_take (plan->source->get_অঙ্ক_glyphs ())
    | hb_map_retains_sorting ([plan] (hb_codepoint_t g) {
	প্রত্যাগম plan->map_gsub_glyph (g);
      })
    | hb_filter ([] (hb_codepoint_t glyph) { প্রত্যাগম glyph != HB_MAP_মান_INVALID; })
    ;

    // Cache the iterator result as it will be iterated multiple times
    // by the serialize code below.
    hb_sorted_vector_t<hb_codepoint_t> glyphs (it);
    Coverage_serialize (c->serializer, glyphs.iter ());
    return_trace (তার্কিক (glyphs));
  }

  তার্কিক intersects (ধ্রুব hb_set_t *glyphs) ধ্রুব
  {
    বিকল্প (u.format.v)
    {
    পক্ষ 1: hb_barrier (); প্রত্যাগম u.format1.intersects (glyphs);
    পক্ষ 2: hb_barrier (); প্রত্যাগম u.format2.intersects (glyphs);
#ifndef HB_NO_BEYOND_64K
    পক্ষ 3: hb_barrier (); প্রত্যাগম u.format3.intersects (glyphs);
    পক্ষ 4: hb_barrier (); প্রত্যাগম u.format4.intersects (glyphs);
#endif
    স্বভাব:প্রত্যাগম অসত্য;
    }
  }
  তার্কিক intersects_coverage (ধ্রুব hb_set_t *glyphs, unsigned পূর্ণাঙ্ক index) ধ্রুব
  {
    বিকল্প (u.format.v)
    {
    পক্ষ 1: hb_barrier (); প্রত্যাগম u.format1.intersects_coverage (glyphs, index);
    পক্ষ 2: hb_barrier (); প্রত্যাগম u.format2.intersects_coverage (glyphs, index);
#ifndef HB_NO_BEYOND_64K
    পক্ষ 3: hb_barrier (); প্রত্যাগম u.format3.intersects_coverage (glyphs, index);
    পক্ষ 4: hb_barrier (); প্রত্যাগম u.format4.intersects_coverage (glyphs, index);
#endif
    স্বভাব:প্রত্যাগম অসত্য;
    }
  }

  unsigned cost () ধ্রুব
  {
    বিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); প্রত্যাগম u.format1.cost ();
    পক্ষ 2: hb_barrier (); প্রত্যাগম u.format2.cost ();
#ifndef HB_NO_BEYOND_64K
    পক্ষ 3: hb_barrier (); প্রত্যাগম u.format3.cost ();
    পক্ষ 4: hb_barrier (); প্রত্যাগম u.format4.cost ();
#endif
    স্বভাব:প্রত্যাগম 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename set_t>
  তার্কিক collect_coverage (set_t *glyphs) ধ্রুব
  {
    বিকল্প (u.format.v)
    {
    পক্ষ 1: hb_barrier (); প্রত্যাগম u.format1.collect_coverage (glyphs);
    পক্ষ 2: hb_barrier (); প্রত্যাগম u.format2.collect_coverage (glyphs);
#ifndef HB_NO_BEYOND_64K
    পক্ষ 3: hb_barrier (); প্রত্যাগম u.format3.collect_coverage (glyphs);
    পক্ষ 4: hb_barrier (); প্রত্যাগম u.format4.collect_coverage (glyphs);
#endif
    স্বভাব:প্রত্যাগম অসত্য;
    }
  }

  template <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  শূন্য intersect_set (ধ্রুব hb_set_t &glyphs, IterableOut&& intersect_glyphs) ধ্রুব
  {
    বিকল্প (u.format.v)
    {
    পক্ষ 1: hb_barrier (); প্রত্যাগম u.format1.intersect_set (glyphs, intersect_glyphs);
    পক্ষ 2: hb_barrier (); প্রত্যাগম u.format2.intersect_set (glyphs, intersect_glyphs);
#ifndef HB_NO_BEYOND_64K
    পক্ষ 3: hb_barrier (); প্রত্যাগম u.format3.intersect_set (glyphs, intersect_glyphs);
    পক্ষ 4: hb_barrier (); প্রত্যাগম u.format4.intersect_set (glyphs, intersect_glyphs);
#endif
    স্বভাব:প্রত্যাগম ;
    }
  }

  সংরচনা iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    স্থির constexpr তার্কিক is_sorted_iterator = সত্য;
    iter_t (ধ্রুব Coverage &c_ = Null (Coverage))
    {
      hb_memset (এতত্, 0, পরিমাণ (*এতত্));
      format = c_.u.format.v;
      বিকল্প (format)
      {
      পক্ষ 1: hb_barrier (); u.format1.init (c_.u.format1); প্রত্যাগম;
      পক্ষ 2: hb_barrier (); u.format2.init (c_.u.format2); প্রত্যাগম;
#ifndef HB_NO_BEYOND_64K
      পক্ষ 3: hb_barrier (); u.format3.init (c_.u.format3); প্রত্যাগম;
      পক্ষ 4: hb_barrier (); u.format4.init (c_.u.format4); প্রত্যাগম;
#endif
      স্বভাব:                               প্রত্যাগম;
      }
    }
    তার্কিক __more__ () ধ্রুব
    {
      বিকল্প (format)
      {
      পক্ষ 1: hb_barrier (); প্রত্যাগম u.format1.__more__ ();
      পক্ষ 2: hb_barrier (); প্রত্যাগম u.format2.__more__ ();
#ifndef HB_NO_BEYOND_64K
      পক্ষ 3: hb_barrier (); প্রত্যাগম u.format3.__more__ ();
      পক্ষ 4: hb_barrier (); প্রত্যাগম u.format4.__more__ ();
#endif
      স্বভাব:প্রত্যাগম অসত্য;
      }
    }
    শূন্য __next__ ()
    {
      বিকল্প (format)
      {
      পক্ষ 1: hb_barrier (); u.format1.__next__ (); ভঙ্গ;
      পক্ষ 2: hb_barrier (); u.format2.__next__ (); ভঙ্গ;
#ifndef HB_NO_BEYOND_64K
      পক্ষ 3: hb_barrier (); u.format3.__next__ (); ভঙ্গ;
      পক্ষ 4: hb_barrier (); u.format4.__next__ (); ভঙ্গ;
#endif
      স্বভাব:                   ভঙ্গ;
      }
    }
    প্রকারনির্দেশ hb_codepoint_t __item_t__;
    __item_t__ __item__ () ধ্রুব { প্রত্যাগম get_glyph (); }

    hb_codepoint_t get_glyph () ধ্রুব
    {
      বিকল্প (format)
      {
      পক্ষ 1: hb_barrier (); প্রত্যাগম u.format1.get_glyph ();
      পক্ষ 2: hb_barrier (); প্রত্যাগম u.format2.get_glyph ();
#ifndef HB_NO_BEYOND_64K
      পক্ষ 3: hb_barrier (); প্রত্যাগম u.format3.get_glyph ();
      পক্ষ 4: hb_barrier (); প্রত্যাগম u.format4.get_glyph ();
#endif
      স্বভাব:প্রত্যাগম 0;
      }
    }
    তার্কিক operator != (ধ্রুব iter_t& o) ধ্রুব
    {
      যদি (unlikely (format != o.format)) প্রত্যাগম সত্য;
      বিকল্প (format)
      {
      পক্ষ 1: hb_barrier (); প্রত্যাগম u.format1 != o.u.format1;
      পক্ষ 2: hb_barrier (); প্রত্যাগম u.format2 != o.u.format2;
#ifndef HB_NO_BEYOND_64K
      পক্ষ 3: hb_barrier (); প্রত্যাগম u.format3 != o.u.format3;
      পক্ষ 4: hb_barrier (); প্রত্যাগম u.format4 != o.u.format4;
#endif
      স্বভাব:প্রত্যাগম অসত্য;
      }
    }
    iter_t __end__ () ধ্রুব
    {
      iter_t it;
      it.format = format;
      বিকল্প (format)
      {
      পক্ষ 1: hb_barrier (); it.u.format1 = u.format1.__end__ (); ভঙ্গ;
      পক্ষ 2: hb_barrier (); it.u.format2 = u.format2.__end__ (); ভঙ্গ;
#ifndef HB_NO_BEYOND_64K
      পক্ষ 3: hb_barrier (); it.u.format3 = u.format3.__end__ (); ভঙ্গ;
      পক্ষ 4: hb_barrier (); it.u.format4 = u.format4.__end__ (); ভঙ্গ;
#endif
      স্বভাব: ভঙ্গ;
      }
      প্রত্যাগম it;
    }

    গুপ্ত:
    unsigned পূর্ণাঙ্ক format;
    সঙ্ঘ {
#ifndef HB_NO_BEYOND_64K
    CoverageFormat2_4<MediumTypes>::iter_t      format4; /* Put this one first since it's larger; helps shut up compiler. */
    CoverageFormat1_3<MediumTypes>::iter_t      format3;
#endif
    CoverageFormat2_4<SmallTypes>::iter_t       format2; /* Put this one first since it's larger; helps shut up compiler. */
    CoverageFormat1_3<SmallTypes>::iter_t       format1;
    } u;
  };
  iter_t iter () ধ্রুব { প্রত্যাগম iter_t (*এতত্); }
};

template<typename Iterator>
স্থির অন্তঃস্থ শূন্য
Coverage_serialize (hb_serialize_context_t *c,
                    Iterator it)
{ c->start_embed<Coverage> ()->serialize (c, it); }

}
}
}

#endif  // #ifndef OT_LAYOUT_COMMON_COVERAGE_HH

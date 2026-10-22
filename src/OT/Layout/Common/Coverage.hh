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
স্থির অন্তঃরেখ শূন্য Coverage_serialize (hb_serialize_context_t *c,
                                       Iterator it);

কাঠামো Coverage
{

  সুরক্ষিত:
  সংঘ {
  কাঠামো { HBUINT16 v; }        format;         /* Format identifier */
  CoverageFormat1_3<SmallTypes> format1;
  CoverageFormat2_4<SmallTypes> format2;
#ifndef HB_NO_BEYOND_64K
  CoverageFormat1_3<MediumTypes>format3;
  CoverageFormat2_4<MediumTypes>format4;
#endif
  } u;
  সর্বজনীন:
  DEFINE_আকার_UNION (2, format.v);

#ifndef HB_OPTIMIZE_আকার
  HB_ALWAYS_INLINE
#endif
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (!u.format.v.sanitize (c)) return_trace (মিথ্যা);
    hb_barrier ();
    নির্বাচন (u.format.v)
    {
    ক্ষেত্র 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    ক্ষেত্র 2: hb_barrier (); return_trace (u.format2.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    ক্ষেত্র 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    পূর্বনির্ধারিত:return_trace (সত্য);
    }
  }

  /* Has interface. */
  unsigned operator [] (hb_codepoint_t k) ধ্রুবক { ফেরত get (k); }
  যৌক্তিক has (hb_codepoint_t k) ধ্রুবক { ফেরত (*এই)[k] != NOT_COVERED; }
  /* Predicate. */
  যৌক্তিক operator () (hb_codepoint_t k) ধ্রুবক { ফেরত has (k); }

  unsigned পূর্ণসংখ্যা get (hb_codepoint_t k) ধ্রুবক { ফেরত get_coverage (k); }
  unsigned পূর্ণসংখ্যা get_coverage (hb_codepoint_t glyph_id) ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.get_coverage (glyph_id);
    ক্ষেত্র 2: hb_barrier (); ফেরত u.format2.get_coverage (glyph_id);
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 3: hb_barrier (); ফেরত u.format3.get_coverage (glyph_id);
    ক্ষেত্র 4: hb_barrier (); ফেরত u.format4.get_coverage (glyph_id);
#endif
    পূর্বনির্ধারিত:ফেরত NOT_COVERED;
    }
  }
  unsigned পূর্ণসংখ্যা get_coverage (hb_codepoint_t glyph_id,
			     hb_ot_layout_mapping_cache_t *cache) ধ্রুবক
  {
    unsigned coverage;
    যদি (cache && cache->get (glyph_id, &coverage)) ফেরত coverage < cache->MAX_মান ? coverage : NOT_COVERED;
    coverage = get_coverage (glyph_id);
    যদি (cache) {
      যদি (coverage == NOT_COVERED)
	cache->set (glyph_id, cache->MAX_মান);
      অন্যথা যদি (likely (coverage < cache->MAX_মান))
	cache->set (glyph_id, coverage);
    }
    ফেরত coverage;
  }

  unsigned পূর্ণসংখ্যা get_coverage_দ্বিমিক (hb_codepoint_t glyph_id,
				    hb_ot_layout_দ্বিমিক_cache_t *cache) ধ্রুবক
  {
    unsigned coverage;
    যদি (cache && cache->get (glyph_id, &coverage)) ফেরত coverage < cache->MAX_মান ? coverage : NOT_COVERED;
    coverage = get_coverage (glyph_id);
    যদি (cache) {
      যদি (coverage == NOT_COVERED)
	cache->set (glyph_id, cache->MAX_মান);
      অন্যথা
	cache->set (glyph_id, 0);
    }
    ফেরত coverage;
  }

  unsigned get_population () ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.get_population ();
    ক্ষেত্র 2: hb_barrier (); ফেরত u.format2.get_population ();
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 3: hb_barrier (); ফেরত u.format3.get_population ();
    ক্ষেত্র 4: hb_barrier (); ফেরত u.format4.get_population ();
#endif
    পূর্বনির্ধারিত:ফেরত NOT_COVERED;
    }
  }

  template <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  যৌক্তিক serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (এই);
    যদি (unlikely (!c->extend_min (এই))) return_trace (মিথ্যা);

    unsigned গুনতি = hb_len (+ glyphs);
    unsigned অঙ্ক_ranges = 0;
    hb_codepoint_t last = (hb_codepoint_t) -2;
    hb_codepoint_t max = 0;
    যৌক্তিক unsorted = মিথ্যা;
    জন্য (auto g: + glyphs)
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
      c->check_success (মিথ্যা, HB_SERIALIZE_ERROR_INT_OVERFLOW);
      return_trace (মিথ্যা);
    }

    নির্বাচন (u.format.v)
    {
    ক্ষেত্র 1: hb_barrier (); return_trace (u.format1.serialize (c, glyphs));
    ক্ষেত্র 2: hb_barrier (); return_trace (u.format2.serialize (c, glyphs));
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 3: hb_barrier (); return_trace (u.format3.serialize (c, glyphs));
    ক্ষেত্র 4: hb_barrier (); return_trace (u.format4.serialize (c, glyphs));
#endif
    পূর্বনির্ধারিত:return_trace (মিথ্যা);
    }
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    ধ্রুবক hb_subset_plan_t *plan = c->plan;
    auto it =
    + iter ()
    | hb_take (plan->source->get_অঙ্ক_glyphs ())
    | hb_map_retains_sorting ([plan] (hb_codepoint_t g) {
	ফেরত plan->map_gsub_glyph (g);
      })
    | hb_filter ([] (hb_codepoint_t glyph) { ফেরত glyph != HB_MAP_মান_INVALID; })
    ;

    // Cache the iterator result as it will be iterated multiple times
    // by the serialize code below.
    hb_sorted_vector_t<hb_codepoint_t> glyphs (it);
    Coverage_serialize (c->serializer, glyphs.iter ());
    return_trace (যৌক্তিক (glyphs));
  }

  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs) ধ্রুবক
  {
    নির্বাচন (u.format.v)
    {
    ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.intersects (glyphs);
    ক্ষেত্র 2: hb_barrier (); ফেরত u.format2.intersects (glyphs);
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 3: hb_barrier (); ফেরত u.format3.intersects (glyphs);
    ক্ষেত্র 4: hb_barrier (); ফেরত u.format4.intersects (glyphs);
#endif
    পূর্বনির্ধারিত:ফেরত মিথ্যা;
    }
  }
  যৌক্তিক intersects_coverage (ধ্রুবক hb_set_t *glyphs, unsigned পূর্ণসংখ্যা সূচক_2) ধ্রুবক
  {
    নির্বাচন (u.format.v)
    {
    ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.intersects_coverage (glyphs, সূচক_2);
    ক্ষেত্র 2: hb_barrier (); ফেরত u.format2.intersects_coverage (glyphs, সূচক_2);
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 3: hb_barrier (); ফেরত u.format3.intersects_coverage (glyphs, সূচক_2);
    ক্ষেত্র 4: hb_barrier (); ফেরত u.format4.intersects_coverage (glyphs, সূচক_2);
#endif
    পূর্বনির্ধারিত:ফেরত মিথ্যা;
    }
  }

  unsigned cost () ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.cost ();
    ক্ষেত্র 2: hb_barrier (); ফেরত u.format2.cost ();
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 3: hb_barrier (); ফেরত u.format3.cost ();
    ক্ষেত্র 4: hb_barrier (); ফেরত u.format4.cost ();
#endif
    পূর্বনির্ধারিত:ফেরত 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename set_t>
  যৌক্তিক collect_coverage (set_t *glyphs) ধ্রুবক
  {
    নির্বাচন (u.format.v)
    {
    ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.collect_coverage (glyphs);
    ক্ষেত্র 2: hb_barrier (); ফেরত u.format2.collect_coverage (glyphs);
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 3: hb_barrier (); ফেরত u.format3.collect_coverage (glyphs);
    ক্ষেত্র 4: hb_barrier (); ফেরত u.format4.collect_coverage (glyphs);
#endif
    পূর্বনির্ধারিত:ফেরত মিথ্যা;
    }
  }

  template <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  শূন্য intersect_set (ধ্রুবক hb_set_t &glyphs, IterableOut&& intersect_glyphs) ধ্রুবক
  {
    নির্বাচন (u.format.v)
    {
    ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.intersect_set (glyphs, intersect_glyphs);
    ক্ষেত্র 2: hb_barrier (); ফেরত u.format2.intersect_set (glyphs, intersect_glyphs);
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 3: hb_barrier (); ফেরত u.format3.intersect_set (glyphs, intersect_glyphs);
    ক্ষেত্র 4: hb_barrier (); ফেরত u.format4.intersect_set (glyphs, intersect_glyphs);
#endif
    পূর্বনির্ধারিত:ফেরত ;
    }
  }

  কাঠামো iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    স্থির constexpr যৌক্তিক is_sorted_iterator = সত্য;
    iter_t (ধ্রুবক Coverage &c_ = Null (Coverage))
    {
      hb_memset (এই, 0, আকার (*এই));
      format = c_.u.format.v;
      নির্বাচন (format)
      {
      ক্ষেত্র 1: hb_barrier (); u.format1.init (c_.u.format1); ফেরত;
      ক্ষেত্র 2: hb_barrier (); u.format2.init (c_.u.format2); ফেরত;
#ifndef HB_NO_BEYOND_64K
      ক্ষেত্র 3: hb_barrier (); u.format3.init (c_.u.format3); ফেরত;
      ক্ষেত্র 4: hb_barrier (); u.format4.init (c_.u.format4); ফেরত;
#endif
      পূর্বনির্ধারিত:                               ফেরত;
      }
    }
    যৌক্তিক __more__ () ধ্রুবক
    {
      নির্বাচন (format)
      {
      ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.__more__ ();
      ক্ষেত্র 2: hb_barrier (); ফেরত u.format2.__more__ ();
#ifndef HB_NO_BEYOND_64K
      ক্ষেত্র 3: hb_barrier (); ফেরত u.format3.__more__ ();
      ক্ষেত্র 4: hb_barrier (); ফেরত u.format4.__more__ ();
#endif
      পূর্বনির্ধারিত:ফেরত মিথ্যা;
      }
    }
    শূন্য __next__ ()
    {
      নির্বাচন (format)
      {
      ক্ষেত্র 1: hb_barrier (); u.format1.__next__ (); বিরতি;
      ক্ষেত্র 2: hb_barrier (); u.format2.__next__ (); বিরতি;
#ifndef HB_NO_BEYOND_64K
      ক্ষেত্র 3: hb_barrier (); u.format3.__next__ (); বিরতি;
      ক্ষেত্র 4: hb_barrier (); u.format4.__next__ (); বিরতি;
#endif
      পূর্বনির্ধারিত:                   বিরতি;
      }
    }
    প্রকারলেখ hb_codepoint_t __item_t__;
    __item_t__ __item__ () ধ্রুবক { ফেরত get_glyph (); }

    hb_codepoint_t get_glyph () ধ্রুবক
    {
      নির্বাচন (format)
      {
      ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.get_glyph ();
      ক্ষেত্র 2: hb_barrier (); ফেরত u.format2.get_glyph ();
#ifndef HB_NO_BEYOND_64K
      ক্ষেত্র 3: hb_barrier (); ফেরত u.format3.get_glyph ();
      ক্ষেত্র 4: hb_barrier (); ফেরত u.format4.get_glyph ();
#endif
      পূর্বনির্ধারিত:ফেরত 0;
      }
    }
    যৌক্তিক operator != (ধ্রুবক iter_t& o) ধ্রুবক
    {
      যদি (unlikely (format != o.format)) ফেরত সত্য;
      নির্বাচন (format)
      {
      ক্ষেত্র 1: hb_barrier (); ফেরত u.format1 != o.u.format1;
      ক্ষেত্র 2: hb_barrier (); ফেরত u.format2 != o.u.format2;
#ifndef HB_NO_BEYOND_64K
      ক্ষেত্র 3: hb_barrier (); ফেরত u.format3 != o.u.format3;
      ক্ষেত্র 4: hb_barrier (); ফেরত u.format4 != o.u.format4;
#endif
      পূর্বনির্ধারিত:ফেরত মিথ্যা;
      }
    }
    iter_t __end__ () ধ্রুবক
    {
      iter_t it;
      it.format = format;
      নির্বাচন (format)
      {
      ক্ষেত্র 1: hb_barrier (); it.u.format1 = u.format1.__end__ (); বিরতি;
      ক্ষেত্র 2: hb_barrier (); it.u.format2 = u.format2.__end__ (); বিরতি;
#ifndef HB_NO_BEYOND_64K
      ক্ষেত্র 3: hb_barrier (); it.u.format3 = u.format3.__end__ (); বিরতি;
      ক্ষেত্র 4: hb_barrier (); it.u.format4 = u.format4.__end__ (); বিরতি;
#endif
      পূর্বনির্ধারিত: বিরতি;
      }
      ফেরত it;
    }

    ব্যক্তিগত:
    unsigned পূর্ণসংখ্যা format;
    সংঘ {
#ifndef HB_NO_BEYOND_64K
    CoverageFormat2_4<MediumTypes>::iter_t      format4; /* Put this one first since it's larger; helps shut up compiler. */
    CoverageFormat1_3<MediumTypes>::iter_t      format3;
#endif
    CoverageFormat2_4<SmallTypes>::iter_t       format2; /* Put this one first since it's larger; helps shut up compiler. */
    CoverageFormat1_3<SmallTypes>::iter_t       format1;
    } u;
  };
  iter_t iter () ধ্রুবক { ফেরত iter_t (*এই); }
};

template<typename Iterator>
স্থির অন্তঃরেখ শূন্য
Coverage_serialize (hb_serialize_context_t *c,
                    Iterator it)
{ c->start_embed<Coverage> ()->serialize (c, it); }

}
}
}

#endif  // #ifndef OT_LAYOUT_COMMON_COVERAGE_HH

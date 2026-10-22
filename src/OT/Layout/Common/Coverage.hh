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

#ଅନ୍ତର୍ଭାଵ "../../../hb-ot-dual.hh"
#ଅନ୍ତର୍ଭାଵ "CoverageFormat1.hh"
#ଅନ୍ତର୍ଭାଵ "CoverageFormat2.hh"

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ Common {

template<typename Iterator>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ Coverage_serialize (hb_serialize_context_t *c,
                                       Iterator it);

ସଂରଚନା Coverage
{

  ସୁରକ୍ଷିତ:
  ସଂଘ {
  ସଂରଚନା { HBUINT16 v; }        format;         /* Format identifier */
  CoverageFormat1_3<SmallTypes> format1;
  CoverageFormat2_4<SmallTypes> format2;
#ifndef HB_NO_BEYOND_64K
  CoverageFormat1_3<MediumTypes>format3;
  CoverageFormat2_4<MediumTypes>format4;
#endif
  } u;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_UNION (2, format.v);

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (!u.format.v.sanitize (c)) return_trace (ମିଥ୍ୟା);
    hb_barrier ();
    ବାଛ (u.format.v)
    {
    ସ୍ଥିତି 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    ସ୍ଥିତି 2: hb_barrier (); return_trace (u.format2.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    ସ୍ଥିତି 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (ସତ୍ୟ);
    }
  }

  /* Has interface. */
  unsigned operator [] (hb_codepoint_t k) ଅଚଳ { ଫେରନ୍ତୁ get (k); }
  ତାର୍କିକ has (hb_codepoint_t k) ଅଚଳ { ଫେରନ୍ତୁ (*ଏହି)[k] != NOT_COVERED; }
  /* Predicate. */
  ତାର୍କିକ operator () (hb_codepoint_t k) ଅଚଳ { ଫେରନ୍ତୁ has (k); }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get (hb_codepoint_t k) ଅଚଳ { ଫେରନ୍ତୁ get_coverage (k); }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_coverage (hb_codepoint_t glyph_id) ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.get_coverage (glyph_id);
    ସ୍ଥିତି 2: hb_barrier (); ଫେରନ୍ତୁ u.format2.get_coverage (glyph_id);
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 3: hb_barrier (); ଫେରନ୍ତୁ u.format3.get_coverage (glyph_id);
    ସ୍ଥିତି 4: hb_barrier (); ଫେରନ୍ତୁ u.format4.get_coverage (glyph_id);
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ NOT_COVERED;
    }
  }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_coverage (hb_codepoint_t glyph_id,
			     hb_ot_layout_mapping_cache_t *cache) ଅଚଳ
  {
    unsigned coverage;
    ଯଦି (cache && cache->get (glyph_id, &coverage)) ଫେରନ୍ତୁ coverage < cache->MAX_ମୂଲ୍ୟ ? coverage : NOT_COVERED;
    coverage = get_coverage (glyph_id);
    ଯଦି (cache) {
      ଯଦି (coverage == NOT_COVERED)
	cache->set (glyph_id, cache->MAX_ମୂଲ୍ୟ);
      ଅନ୍ୟଥା ଯଦି (likely (coverage < cache->MAX_ମୂଲ୍ୟ))
	cache->set (glyph_id, coverage);
    }
    ଫେରନ୍ତୁ coverage;
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_coverage_ଦ୍ୱିମିକ (hb_codepoint_t glyph_id,
				    hb_ot_layout_ଦ୍ୱିମିକ_cache_t *cache) ଅଚଳ
  {
    unsigned coverage;
    ଯଦି (cache && cache->get (glyph_id, &coverage)) ଫେରନ୍ତୁ coverage < cache->MAX_ମୂଲ୍ୟ ? coverage : NOT_COVERED;
    coverage = get_coverage (glyph_id);
    ଯଦି (cache) {
      ଯଦି (coverage == NOT_COVERED)
	cache->set (glyph_id, cache->MAX_ମୂଲ୍ୟ);
      ଅନ୍ୟଥା
	cache->set (glyph_id, 0);
    }
    ଫେରନ୍ତୁ coverage;
  }

  unsigned get_population () ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.get_population ();
    ସ୍ଥିତି 2: hb_barrier (); ଫେରନ୍ତୁ u.format2.get_population ();
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 3: hb_barrier (); ଫେରନ୍ତୁ u.format3.get_population ();
    ସ୍ଥିତି 4: hb_barrier (); ଫେରନ୍ତୁ u.format4.get_population ();
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ NOT_COVERED;
    }
  }

  template <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  ତାର୍କିକ serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (ଏହି);
    ଯଦି (unlikely (!c->extend_min (ଏହି))) return_trace (ମିଥ୍ୟା);

    unsigned ଗଣନା_2 = hb_len (+ glyphs);
    unsigned ଅଙ୍କ_ranges = 0;
    hb_codepoint_t last = (hb_codepoint_t) -2;
    hb_codepoint_t max = 0;
    ତାର୍କିକ unsorted = ମିଥ୍ୟା;
    ପାଇଁ (auto g: + glyphs)
    {
      ଯଦି (last != (hb_codepoint_t) -2 && g < last)
	unsorted = ସତ୍ୟ;
      ଯଦି (last + 1 != g)
	ଅଙ୍କ_ranges++;
      last = g;
      ଯଦି (g > max) max = g;
    }
    u.format.v = !unsorted && ଗଣନା_2 <= ଅଙ୍କ_ranges * 3 ? 1 : 2;

#ifndef HB_NO_BEYOND_64K
    ଯଦି (max > 0xFFFFu)
      u.format.v += 2;
    ଯଦି (unlikely (max > 0xFFFFFFu))
#ଅନ୍ୟଥା
    ଯଦି (unlikely (max > 0xFFFFu))
#endif
    {
      c->check_success (ମିଥ୍ୟା, HB_SERIALIZE_ERROR_INT_OVERFLOW);
      return_trace (ମିଥ୍ୟା);
    }

    ବାଛ (u.format.v)
    {
    ସ୍ଥିତି 1: hb_barrier (); return_trace (u.format1.serialize (c, glyphs));
    ସ୍ଥିତି 2: hb_barrier (); return_trace (u.format2.serialize (c, glyphs));
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 3: hb_barrier (); return_trace (u.format3.serialize (c, glyphs));
    ସ୍ଥିତି 4: hb_barrier (); return_trace (u.format4.serialize (c, glyphs));
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (ମିଥ୍ୟା);
    }
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଅଚଳ hb_subset_plan_t *plan = c->plan;
    auto it =
    + iter ()
    | hb_take (plan->source->get_ଅଙ୍କ_glyphs ())
    | hb_map_retains_sorting ([plan] (hb_codepoint_t g) {
	ଫେରନ୍ତୁ plan->map_gsub_glyph (g);
      })
    | hb_filter ([] (hb_codepoint_t glyph) { ଫେରନ୍ତୁ glyph != HB_MAP_ମୂଲ୍ୟ_INVALID; })
    ;

    // Cache the iterator result as it will be iterated multiple times
    // by the serialize code below.
    hb_sorted_vector_t<hb_codepoint_t> glyphs (it);
    Coverage_serialize (c->serializer, glyphs.iter ());
    return_trace (ତାର୍କିକ (glyphs));
  }

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs) ଅଚଳ
  {
    ବାଛ (u.format.v)
    {
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.intersects (glyphs);
    ସ୍ଥିତି 2: hb_barrier (); ଫେରନ୍ତୁ u.format2.intersects (glyphs);
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 3: hb_barrier (); ଫେରନ୍ତୁ u.format3.intersects (glyphs);
    ସ୍ଥିତି 4: hb_barrier (); ଫେରନ୍ତୁ u.format4.intersects (glyphs);
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
  }
  ତାର୍କିକ intersects_coverage (ଅଚଳ hb_set_t *glyphs, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index) ଅଚଳ
  {
    ବାଛ (u.format.v)
    {
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.intersects_coverage (glyphs, index);
    ସ୍ଥିତି 2: hb_barrier (); ଫେରନ୍ତୁ u.format2.intersects_coverage (glyphs, index);
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 3: hb_barrier (); ଫେରନ୍ତୁ u.format3.intersects_coverage (glyphs, index);
    ସ୍ଥିତି 4: hb_barrier (); ଫେରନ୍ତୁ u.format4.intersects_coverage (glyphs, index);
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
  }

  unsigned cost () ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.cost ();
    ସ୍ଥିତି 2: hb_barrier (); ଫେରନ୍ତୁ u.format2.cost ();
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 3: hb_barrier (); ଫେରନ୍ତୁ u.format3.cost ();
    ସ୍ଥିତି 4: hb_barrier (); ଫେରନ୍ତୁ u.format4.cost ();
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename set_t>
  ତାର୍କିକ collect_coverage (set_t *glyphs) ଅଚଳ
  {
    ବାଛ (u.format.v)
    {
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.collect_coverage (glyphs);
    ସ୍ଥିତି 2: hb_barrier (); ଫେରନ୍ତୁ u.format2.collect_coverage (glyphs);
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 3: hb_barrier (); ଫେରନ୍ତୁ u.format3.collect_coverage (glyphs);
    ସ୍ଥିତି 4: hb_barrier (); ଫେରନ୍ତୁ u.format4.collect_coverage (glyphs);
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
  }

  template <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  ଶୂନ୍ୟ intersect_set (ଅଚଳ hb_set_t &glyphs, IterableOut&& intersect_glyphs) ଅଚଳ
  {
    ବାଛ (u.format.v)
    {
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.intersect_set (glyphs, intersect_glyphs);
    ସ୍ଥିତି 2: hb_barrier (); ଫେରନ୍ତୁ u.format2.intersect_set (glyphs, intersect_glyphs);
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 3: hb_barrier (); ଫେରନ୍ତୁ u.format3.intersect_set (glyphs, intersect_glyphs);
    ସ୍ଥିତି 4: hb_barrier (); ଫେରନ୍ତୁ u.format4.intersect_set (glyphs, intersect_glyphs);
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ ;
    }
  }

  ସଂରଚନା iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ସ୍ଥିର constexpr ତାର୍କିକ is_sorted_iterator = ସତ୍ୟ;
    iter_t (ଅଚଳ Coverage &c_ = Null (Coverage))
    {
      hb_memset (ଏହି, 0, ଆକାର (*ଏହି));
      format = c_.u.format.v;
      ବାଛ (format)
      {
      ସ୍ଥିତି 1: hb_barrier (); u.format1.init (c_.u.format1); ଫେରନ୍ତୁ;
      ସ୍ଥିତି 2: hb_barrier (); u.format2.init (c_.u.format2); ଫେରନ୍ତୁ;
#ifndef HB_NO_BEYOND_64K
      ସ୍ଥିତି 3: hb_barrier (); u.format3.init (c_.u.format3); ଫେରନ୍ତୁ;
      ସ୍ଥିତି 4: hb_barrier (); u.format4.init (c_.u.format4); ଫେରନ୍ତୁ;
#endif
      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:                               ଫେରନ୍ତୁ;
      }
    }
    ତାର୍କିକ __more__ () ଅଚଳ
    {
      ବାଛ (format)
      {
      ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.__more__ ();
      ସ୍ଥିତି 2: hb_barrier (); ଫେରନ୍ତୁ u.format2.__more__ ();
#ifndef HB_NO_BEYOND_64K
      ସ୍ଥିତି 3: hb_barrier (); ଫେରନ୍ତୁ u.format3.__more__ ();
      ସ୍ଥିତି 4: hb_barrier (); ଫେରନ୍ତୁ u.format4.__more__ ();
#endif
      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }
    }
    ଶୂନ୍ୟ __next__ ()
    {
      ବାଛ (format)
      {
      ସ୍ଥିତି 1: hb_barrier (); u.format1.__next__ (); ଭାଙ୍ଗ;
      ସ୍ଥିତି 2: hb_barrier (); u.format2.__next__ (); ଭାଙ୍ଗ;
#ifndef HB_NO_BEYOND_64K
      ସ୍ଥିତି 3: hb_barrier (); u.format3.__next__ (); ଭାଙ୍ଗ;
      ସ୍ଥିତି 4: hb_barrier (); u.format4.__next__ (); ଭାଙ୍ଗ;
#endif
      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:                   ଭାଙ୍ଗ;
      }
    }
    ପ୍ରକାରବ୍ୟାଖ୍ୟା hb_codepoint_t __item_t__;
    __item_t__ __item__ () ଅଚଳ { ଫେରନ୍ତୁ get_glyph (); }

    hb_codepoint_t get_glyph () ଅଚଳ
    {
      ବାଛ (format)
      {
      ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.get_glyph ();
      ସ୍ଥିତି 2: hb_barrier (); ଫେରନ୍ତୁ u.format2.get_glyph ();
#ifndef HB_NO_BEYOND_64K
      ସ୍ଥିତି 3: hb_barrier (); ଫେରନ୍ତୁ u.format3.get_glyph ();
      ସ୍ଥିତି 4: hb_barrier (); ଫେରନ୍ତୁ u.format4.get_glyph ();
#endif
      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ 0;
      }
    }
    ତାର୍କିକ operator != (ଅଚଳ iter_t& o) ଅଚଳ
    {
      ଯଦି (unlikely (format != o.format)) ଫେରନ୍ତୁ ସତ୍ୟ;
      ବାଛ (format)
      {
      ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1 != o.u.format1;
      ସ୍ଥିତି 2: hb_barrier (); ଫେରନ୍ତୁ u.format2 != o.u.format2;
#ifndef HB_NO_BEYOND_64K
      ସ୍ଥିତି 3: hb_barrier (); ଫେରନ୍ତୁ u.format3 != o.u.format3;
      ସ୍ଥିତି 4: hb_barrier (); ଫେରନ୍ତୁ u.format4 != o.u.format4;
#endif
      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }
    }
    iter_t __end__ () ଅଚଳ
    {
      iter_t it;
      it.format = format;
      ବାଛ (format)
      {
      ସ୍ଥିତି 1: hb_barrier (); it.u.format1 = u.format1.__end__ (); ଭାଙ୍ଗ;
      ସ୍ଥିତି 2: hb_barrier (); it.u.format2 = u.format2.__end__ (); ଭାଙ୍ଗ;
#ifndef HB_NO_BEYOND_64K
      ସ୍ଥିତି 3: hb_barrier (); it.u.format3 = u.format3.__end__ (); ଭାଙ୍ଗ;
      ସ୍ଥିତି 4: hb_barrier (); it.u.format4 = u.format4.__end__ (); ଭାଙ୍ଗ;
#endif
      ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ଭାଙ୍ଗ;
      }
      ଫେରନ୍ତୁ it;
    }

    ବ୍ୟକ୍ତିଗତ:
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା format;
    ସଂଘ {
#ifndef HB_NO_BEYOND_64K
    CoverageFormat2_4<MediumTypes>::iter_t      format4; /* Put this one first since it's larger; helps shut up compiler. */
    CoverageFormat1_3<MediumTypes>::iter_t      format3;
#endif
    CoverageFormat2_4<SmallTypes>::iter_t       format2; /* Put this one first since it's larger; helps shut up compiler. */
    CoverageFormat1_3<SmallTypes>::iter_t       format1;
    } u;
  };
  iter_t iter () ଅଚଳ { ଫେରନ୍ତୁ iter_t (*ଏହି); }
};

template<typename Iterator>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
Coverage_serialize (hb_serialize_context_t *c,
                    Iterator it)
{ c->start_embed<Coverage> ()->serialize (c, it); }

}
}
}

#endif  // #ifndef OT_LAYOUT_COMMON_COVERAGE_HH

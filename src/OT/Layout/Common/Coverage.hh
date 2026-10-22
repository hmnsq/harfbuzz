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

#నిర్వచితం_కాకపోతే OT_LAYOUT_COMMON_COVERAGE_HH
#నిర్వచించు OT_LAYOUT_COMMON_COVERAGE_HH

#అన్తర్భావ "../../../hb-ot-dual.hh"
#అన్తర్భావ "CoverageFormat1.hh"
#అన్తర్భావ "CoverageFormat2.hh"

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ Common {

మూస<రకం_పేరు Iterator>
స్థిర అంతఃవరుస శూన్య Coverage_serialize (hb_serialize_context_t *c,
                                       Iterator it);

నిర్మాణం Coverage
{

  రక్షిత:
  సంఘం {
  నిర్మాణం { HBUINT16 v; }        format;         /* Format identifier */
  CoverageFormat1_3<SmallTypes> format1;
  CoverageFormat2_4<SmallTypes> format2;
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  CoverageFormat1_3<MediumTypes>format3;
  CoverageFormat2_4<MediumTypes>format4;
#అయితే_ముగింపు
  } u;
  బహిరంగ:
  DEFINE_కొలత_UNION (2, format.v);

#నిర్వచితం_కాకపోతే HB_OPTIMIZE_కొలత
  HB_ALWAYS_INLINE
#అయితే_ముగింపు
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (!u.format.v.sanitize (c)) return_trace (అబద్ధం);
    hb_barrier ();
    ఎంపిక (u.format.v)
    {
    స్థితి 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    స్థితి 2: hb_barrier (); return_trace (u.format2.sanitize (c));
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    స్థితి 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#అయితే_ముగింపు
    అప్రమేయ:return_trace (నిజం);
    }
  }

  /* Has interface. */
  గుర్తులేని క్రియాకారకం [] (hb_codepoint_t k) స్థిరరాశి { తిరిగి get (k); }
  తార్కిక has (hb_codepoint_t k) స్థిరరాశి { తిరిగి (*ఇది)[k] != NOT_COVERED; }
  /* Predicate. */
  తార్కిక క్రియాకారకం () (hb_codepoint_t k) స్థిరరాశి { తిరిగి has (k); }

  గుర్తులేని పూర్ణాంకం get (hb_codepoint_t k) స్థిరరాశి { తిరిగి get_coverage (k); }
  గుర్తులేని పూర్ణాంకం get_coverage (hb_codepoint_t glyph_id) స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); తిరిగి u.format1.get_coverage (glyph_id);
    స్థితి 2: hb_barrier (); తిరిగి u.format2.get_coverage (glyph_id);
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 3: hb_barrier (); తిరిగి u.format3.get_coverage (glyph_id);
    స్థితి 4: hb_barrier (); తిరిగి u.format4.get_coverage (glyph_id);
#అయితే_ముగింపు
    అప్రమేయ:తిరిగి NOT_COVERED;
    }
  }
  గుర్తులేని పూర్ణాంకం get_coverage (hb_codepoint_t glyph_id,
			     hb_ot_layout_mapping_cache_t *cache) స్థిరరాశి
  {
    గుర్తులేని coverage;
    ఒకవేళ (cache && cache->get (glyph_id, &coverage)) తిరిగి coverage < cache->MAX_విలువ ? coverage : NOT_COVERED;
    coverage = get_coverage (glyph_id);
    ఒకవేళ (cache) {
      ఒకవేళ (coverage == NOT_COVERED)
	cache->సమితి (glyph_id, cache->MAX_విలువ);
      లేకపోతే ఒకవేళ (likely (coverage < cache->MAX_విలువ))
	cache->సమితి (glyph_id, coverage);
    }
    తిరిగి coverage;
  }

  గుర్తులేని పూర్ణాంకం get_coverage_ద్విమాన (hb_codepoint_t glyph_id,
				    hb_ot_layout_ద్విమాన_cache_t *cache) స్థిరరాశి
  {
    గుర్తులేని coverage;
    ఒకవేళ (cache && cache->get (glyph_id, &coverage)) తిరిగి coverage < cache->MAX_విలువ ? coverage : NOT_COVERED;
    coverage = get_coverage (glyph_id);
    ఒకవేళ (cache) {
      ఒకవేళ (coverage == NOT_COVERED)
	cache->సమితి (glyph_id, cache->MAX_విలువ);
      లేకపోతే
	cache->సమితి (glyph_id, 0);
    }
    తిరిగి coverage;
  }

  గుర్తులేని get_population () స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); తిరిగి u.format1.get_population ();
    స్థితి 2: hb_barrier (); తిరిగి u.format2.get_population ();
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 3: hb_barrier (); తిరిగి u.format3.get_population ();
    స్థితి 4: hb_barrier (); తిరిగి u.format4.get_population ();
#అయితే_ముగింపు
    అప్రమేయ:తిరిగి NOT_COVERED;
    }
  }

  మూస <రకం_పేరు Iterator,
      hb_requires (hb_is_sorted_ఆధారం_యొక్క (Iterator, hb_codepoint_t))>
  తార్కిక serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (ఇది);
    ఒకవేళ (unlikely (!c->extend_min (ఇది))) return_trace (అబద్ధం);

    గుర్తులేని లెక్క = hb_len (+ glyphs);
    గుర్తులేని అంకె_ranges = 0;
    hb_codepoint_t చివరి = (hb_codepoint_t) -2;
    hb_codepoint_t max = 0;
    తార్కిక unsorted = అబద్ధం;
    కొరకు (స్వయంచాలక g: + glyphs)
    {
      ఒకవేళ (చివరి != (hb_codepoint_t) -2 && g < చివరి)
	unsorted = నిజం;
      ఒకవేళ (చివరి + 1 != g)
	అంకె_ranges++;
      చివరి = g;
      ఒకవేళ (g > max) max = g;
    }
    u.format.v = !unsorted && లెక్క <= అంకె_ranges * 3 ? 1 : 2;

#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (max > 0xFFFFu)
      u.format.v += 2;
    ఒకవేళ (unlikely (max > 0xFFFFFFu))
#లేకపోతే
    ఒకవేళ (unlikely (max > 0xFFFFu))
#అయితే_ముగింపు
    {
      c->check_success (అబద్ధం, HB_SERIALIZE_ERROR_INT_అధికప్రవాహం);
      return_trace (అబద్ధం);
    }

    ఎంపిక (u.format.v)
    {
    స్థితి 1: hb_barrier (); return_trace (u.format1.serialize (c, glyphs));
    స్థితి 2: hb_barrier (); return_trace (u.format2.serialize (c, glyphs));
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 3: hb_barrier (); return_trace (u.format3.serialize (c, glyphs));
    స్థితి 4: hb_barrier (); return_trace (u.format4.serialize (c, glyphs));
#అయితే_ముగింపు
    అప్రమేయ:return_trace (అబద్ధం);
    }
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్థిరరాశి hb_subset_plan_t *plan = c->plan;
    స్వయంచాలక it =
    + iter ()
    | hb_take (plan->ఆధారం->get_అంకె_glyphs ())
    | hb_map_retains_క్రమీకరణ ([plan] (hb_codepoint_t g) {
	తిరిగి plan->map_gsub_glyph (g);
      })
    | hb_filter ([] (hb_codepoint_t glyph) { తిరిగి glyph != HB_MAP_విలువ_INVALID; })
    ;

    // Cache the iterator result as it will be iterated multiple times
    // by the serialize code below.
    hb_sorted_సదిశ_t<hb_codepoint_t> glyphs (it);
    Coverage_serialize (c->serializer, glyphs.iter ());
    return_trace (తార్కిక (glyphs));
  }

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs) స్థిరరాశి
  {
    ఎంపిక (u.format.v)
    {
    స్థితి 1: hb_barrier (); తిరిగి u.format1.intersects (glyphs);
    స్థితి 2: hb_barrier (); తిరిగి u.format2.intersects (glyphs);
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 3: hb_barrier (); తిరిగి u.format3.intersects (glyphs);
    స్థితి 4: hb_barrier (); తిరిగి u.format4.intersects (glyphs);
#అయితే_ముగింపు
    అప్రమేయ:తిరిగి అబద్ధం;
    }
  }
  తార్కిక intersects_coverage (స్థిరరాశి hb_సమితి_t *glyphs, గుర్తులేని పూర్ణాంకం సూచీ_2) స్థిరరాశి
  {
    ఎంపిక (u.format.v)
    {
    స్థితి 1: hb_barrier (); తిరిగి u.format1.intersects_coverage (glyphs, సూచీ_2);
    స్థితి 2: hb_barrier (); తిరిగి u.format2.intersects_coverage (glyphs, సూచీ_2);
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 3: hb_barrier (); తిరిగి u.format3.intersects_coverage (glyphs, సూచీ_2);
    స్థితి 4: hb_barrier (); తిరిగి u.format4.intersects_coverage (glyphs, సూచీ_2);
#అయితే_ముగింపు
    అప్రమేయ:తిరిగి అబద్ధం;
    }
  }

  గుర్తులేని ఖర్చు () స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); తిరిగి u.format1.ఖర్చు ();
    స్థితి 2: hb_barrier (); తిరిగి u.format2.ఖర్చు ();
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 3: hb_barrier (); తిరిగి u.format3.ఖర్చు ();
    స్థితి 4: hb_barrier (); తిరిగి u.format4.ఖర్చు ();
#అయితే_ముగింపు
    అప్రమేయ:తిరిగి 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  మూస <రకం_పేరు సమితి_t>
  తార్కిక collect_coverage (సమితి_t *glyphs) స్థిరరాశి
  {
    ఎంపిక (u.format.v)
    {
    స్థితి 1: hb_barrier (); తిరిగి u.format1.collect_coverage (glyphs);
    స్థితి 2: hb_barrier (); తిరిగి u.format2.collect_coverage (glyphs);
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 3: hb_barrier (); తిరిగి u.format3.collect_coverage (glyphs);
    స్థితి 4: hb_barrier (); తిరిగి u.format4.collect_coverage (glyphs);
#అయితే_ముగింపు
    అప్రమేయ:తిరిగి అబద్ధం;
    }
  }

  మూస <రకం_పేరు Iterable_బహిర్గతం,
	    hb_requires (hb_is_sink_యొక్క (Iterable_బహిర్గతం, hb_codepoint_t))>
  శూన్య intersect_సమితి (స్థిరరాశి hb_సమితి_t &glyphs, Iterable_బహిర్గతం&& intersect_glyphs) స్థిరరాశి
  {
    ఎంపిక (u.format.v)
    {
    స్థితి 1: hb_barrier (); తిరిగి u.format1.intersect_సమితి (glyphs, intersect_glyphs);
    స్థితి 2: hb_barrier (); తిరిగి u.format2.intersect_సమితి (glyphs, intersect_glyphs);
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 3: hb_barrier (); తిరిగి u.format3.intersect_సమితి (glyphs, intersect_glyphs);
    స్థితి 4: hb_barrier (); తిరిగి u.format4.intersect_సమితి (glyphs, intersect_glyphs);
#అయితే_ముగింపు
    అప్రమేయ:తిరిగి ;
    }
  }

  నిర్మాణం iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    స్థిర constexpr తార్కిక is_sorted_iterator = నిజం;
    iter_t (స్థిరరాశి Coverage &c_ = Null (Coverage))
    {
      hb_memset (ఇది, 0, పరిమాణం (*ఇది));
      format = c_.u.format.v;
      ఎంపిక (format)
      {
      స్థితి 1: hb_barrier (); u.format1.init (c_.u.format1); తిరిగి;
      స్థితి 2: hb_barrier (); u.format2.init (c_.u.format2); తిరిగి;
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
      స్థితి 3: hb_barrier (); u.format3.init (c_.u.format3); తిరిగి;
      స్థితి 4: hb_barrier (); u.format4.init (c_.u.format4); తిరిగి;
#అయితే_ముగింపు
      అప్రమేయ:                               తిరిగి;
      }
    }
    తార్కిక __more__ () స్థిరరాశి
    {
      ఎంపిక (format)
      {
      స్థితి 1: hb_barrier (); తిరిగి u.format1.__more__ ();
      స్థితి 2: hb_barrier (); తిరిగి u.format2.__more__ ();
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
      స్థితి 3: hb_barrier (); తిరిగి u.format3.__more__ ();
      స్థితి 4: hb_barrier (); తిరిగి u.format4.__more__ ();
#అయితే_ముగింపు
      అప్రమేయ:తిరిగి అబద్ధం;
      }
    }
    శూన్య __next__ ()
    {
      ఎంపిక (format)
      {
      స్థితి 1: hb_barrier (); u.format1.__next__ (); ఆపు;
      స్థితి 2: hb_barrier (); u.format2.__next__ (); ఆపు;
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
      స్థితి 3: hb_barrier (); u.format3.__next__ (); ఆపు;
      స్థితి 4: hb_barrier (); u.format4.__next__ (); ఆపు;
#అయితే_ముగింపు
      అప్రమేయ:                   ఆపు;
      }
    }
    రకనిర్వచనం hb_codepoint_t __item_t__;
    __item_t__ __item__ () స్థిరరాశి { తిరిగి get_glyph (); }

    hb_codepoint_t get_glyph () స్థిరరాశి
    {
      ఎంపిక (format)
      {
      స్థితి 1: hb_barrier (); తిరిగి u.format1.get_glyph ();
      స్థితి 2: hb_barrier (); తిరిగి u.format2.get_glyph ();
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
      స్థితి 3: hb_barrier (); తిరిగి u.format3.get_glyph ();
      స్థితి 4: hb_barrier (); తిరిగి u.format4.get_glyph ();
#అయితే_ముగింపు
      అప్రమేయ:తిరిగి 0;
      }
    }
    తార్కిక క్రియాకారకం != (స్థిరరాశి iter_t& o) స్థిరరాశి
    {
      ఒకవేళ (unlikely (format != o.format)) తిరిగి నిజం;
      ఎంపిక (format)
      {
      స్థితి 1: hb_barrier (); తిరిగి u.format1 != o.u.format1;
      స్థితి 2: hb_barrier (); తిరిగి u.format2 != o.u.format2;
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
      స్థితి 3: hb_barrier (); తిరిగి u.format3 != o.u.format3;
      స్థితి 4: hb_barrier (); తిరిగి u.format4 != o.u.format4;
#అయితే_ముగింపు
      అప్రమేయ:తిరిగి అబద్ధం;
      }
    }
    iter_t __end__ () స్థిరరాశి
    {
      iter_t it;
      it.format = format;
      ఎంపిక (format)
      {
      స్థితి 1: hb_barrier (); it.u.format1 = u.format1.__end__ (); ఆపు;
      స్థితి 2: hb_barrier (); it.u.format2 = u.format2.__end__ (); ఆపు;
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
      స్థితి 3: hb_barrier (); it.u.format3 = u.format3.__end__ (); ఆపు;
      స్థితి 4: hb_barrier (); it.u.format4 = u.format4.__end__ (); ఆపు;
#అయితే_ముగింపు
      అప్రమేయ: ఆపు;
      }
      తిరిగి it;
    }

    అంతరంగ:
    గుర్తులేని పూర్ణాంకం format;
    సంఘం {
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    CoverageFormat2_4<MediumTypes>::iter_t      format4; /* Put this one first since it's larger; helps shut up compiler. */
    CoverageFormat1_3<MediumTypes>::iter_t      format3;
#అయితే_ముగింపు
    CoverageFormat2_4<SmallTypes>::iter_t       format2; /* Put this one first since it's larger; helps shut up compiler. */
    CoverageFormat1_3<SmallTypes>::iter_t       format1;
    } u;
  };
  iter_t iter () స్థిరరాశి { తిరిగి iter_t (*ఇది); }
};

మూస<రకం_పేరు Iterator>
స్థిర అంతఃవరుస శూన్య
Coverage_serialize (hb_serialize_context_t *c,
                    Iterator it)
{ c->ప్రారంభం_embed<Coverage> ()->serialize (c, it); }

}
}
}

#అయితే_ముగింపు  // #ifndef OT_LAYOUT_COMMON_COVERAGE_HH

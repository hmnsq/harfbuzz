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

#నిర్వచితం_కాకపోతే OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH
#నిర్వచించు OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH

#అన్తర్భావ "RangeRecord.hh"

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ Common {

మూస <రకం_పేరు Types>
నిర్మాణం CoverageFormat2_4
{
  మిత్ర నిర్మాణం Coverage;
  ఉపయోగించి Coverage_Range_రికార్డు = Range_రికార్డు<Types, రకం_పేరు Types::HBUINT>;

  బహిరంగ:
  HBUINT16      coverageFormat; /* Format identifier--format = 2 */
  రకం_పేరు Types::మూస Sorted_Array_యొక్క<Coverage_Range_రికార్డు>
                range_రికార్డు;    /* Array of glyph ranges--ordered by
                                 * Start GlyphID. rangeCount entries
                                 * long */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (2 + Types::HBUINT::static_కొలత, range_రికార్డు);

  అంతరంగ:

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (range_రికార్డు.sanitize (c));
  }

  గుర్తులేని పూర్ణాంకం get_coverage (hb_codepoint_t glyph_id) స్థిరరాశి
  {
    స్థిరరాశి Coverage_Range_రికార్డు &range = range_రికార్డు.bsearch (glyph_id);
    తిరిగి likely (range.మొదటి <= range.చివరి)
         ? (గుర్తులేని పూర్ణాంకం) range.value + (glyph_id - range.మొదటి)
         : NOT_COVERED;
  }

  గుర్తులేని get_population () స్థిరరాశి
  {
    రకం_పేరు Types::large_int ret = 0;
    కొరకు (స్థిరరాశి స్వయంచాలక &r : range_రికార్డు)
      ret += r.get_population ();
    తిరిగి ret > UINT_MAX ? UINT_MAX : (గుర్తులేని) ret;
  }

  మూస <రకం_పేరు Iterator,
      hb_requires (hb_is_sorted_ఆధారం_యొక్క (Iterator, hb_codepoint_t))>
  తార్కిక serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (ఇది);
    ఒకవేళ (unlikely (!c->extend_min (ఇది))) return_trace (అబద్ధం);

    గుర్తులేని అంకె_ranges = 0;
    hb_codepoint_t చివరి = (hb_codepoint_t) -2;
    కొరకు (స్వయంచాలక g: glyphs)
    {
      ఒకవేళ (చివరి + 1 != g)
        అంకె_ranges++;
      చివరి = g;
    }

    ఒకవేళ (unlikely (!range_రికార్డు.serialize (c, అంకె_ranges))) return_trace (అబద్ధం);
    ఒకవేళ (!అంకె_ranges) return_trace (నిజం);

    గుర్తులేని లెక్క = 0;
    గుర్తులేని range = (గుర్తులేని) -1;
    చివరి = (hb_codepoint_t) -2;
    గుర్తులేని unsorted = అబద్ధం;
    కొరకు (స్వయంచాలక g: glyphs)
    {
      ఒకవేళ (చివరి + 1 != g)
      {
	ఒకవేళ (unlikely (చివరి != (hb_codepoint_t) -2 && చివరి + 1 > g))
	  unsorted = నిజం;

        range++;
        range_రికార్డు.arrayZ[range].మొదటి = g;
        range_రికార్డు.arrayZ[range].value = లెక్క;
      }
      range_రికార్డు.arrayZ[range].చివరి = g;
      చివరి = g;
      లెక్క++;
    }

    ఒకవేళ (unlikely (unsorted))
      range_రికార్డు.as_array ().qsort (Coverage_Range_రికార్డు::cmp_range);

    return_trace (నిజం);
  }

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs) స్థిరరాశి
  {
    ఒకవేళ (range_రికార్డు.len > glyphs->get_population () * hb_బిట్టు_storage ((గుర్తులేని) range_రికార్డు.len))
    {
      కొరకు (స్వయంచాలక g : *glyphs)
        ఒకవేళ (get_coverage (g) != NOT_COVERED)
	  తిరిగి నిజం;
      తిరిగి అబద్ధం;
    }

    తిరిగి hb_any (+ hb_iter (range_రికార్డు)
                   | hb_map ([glyphs] (స్థిరరాశి Coverage_Range_రికార్డు &range) { తిరిగి range.intersects (*glyphs); }));
  }
  తార్కిక intersects_coverage (స్థిరరాశి hb_సమితి_t *glyphs, గుర్తులేని పూర్ణాంకం సూచీ_2) స్థిరరాశి
  {
    స్వయంచాలక *range = range_రికార్డు.as_array ().bsearch (సూచీ_2);
    ఒకవేళ (range)
      తిరిగి range->intersects (*glyphs);
    తిరిగి అబద్ధం;
  }

  స్థిర శూన్య కూడు_intersected_బిట్లు (hb_సమితి_t &సమితి, hb_codepoint_t పునాది, uint64_t బిట్లు)
  { సమితి.కూడు_బిట్లు (పునాది, బిట్లు); }

  మూస <రకం_పేరు Iterable_బహిర్గతం>
  స్థిర శూన్య కూడు_intersected_బిట్లు (Iterable_బహిర్గతం&& బహిర్గతం, hb_codepoint_t పునాది, uint64_t బిట్లు)
  {
    అయితే (బిట్లు)
    {
      బహిర్గతం << పునాది + hb_ctz (బిట్లు);
      బిట్లు &= బిట్లు - 1;
    }
  }

  మూస <రకం_పేరు Iterable_బహిర్గతం,
	    hb_requires (hb_is_sink_యొక్క (Iterable_బహిర్గతం, hb_codepoint_t))>
  శూన్య intersect_సమితి (స్థిరరాశి hb_సమితి_t &glyphs, Iterable_బహిర్గతం&& intersect_glyphs) స్థిరరాశి
  {
    /* Break out of loop for overlapping, broken, tables,
     * to avoid fuzzer timouts. */
    hb_codepoint_t చివరి = 0;
    కొరకు (స్థిరరాశి స్వయంచాలక& range : range_రికార్డు)
    {
      ఒకవేళ (unlikely (range.మొదటి < చివరి))
        ఆపు;
      చివరి = range.చివరి;
      hb_codepoint_t cursor = range.మొదటి & ~hb_బిట్టు_పుట_t::ELT_ముసుగు;
      cursor = cursor ? cursor - 1 : HB_సమితి_విలువ_INVALID;
      uint64_t బిట్లు;
      అయితే (glyphs.తదుపరి_బిట్లు (&cursor, &బిట్లు) && cursor <= చివరి)
      {
	ఒకవేళ (range.మొదటి > cursor)
	  బిట్లు &= UINT64_MAX << (range.మొదటి - cursor);
	గుర్తులేని పూర్ణాంకం చివర = చివరి - cursor;
	ఒకవేళ (చివర < hb_బిట్టు_పుట_t::ELT_బిట్లు - 1)
	  బిట్లు &= (uint64_t (1) << (చివర + 1)) - 1;
	కూడు_intersected_బిట్లు (intersect_glyphs, cursor, బిట్లు);
      }
    }
  }

  గుర్తులేని ఖర్చు () స్థిరరాశి { తిరిగి hb_బిట్టు_storage ((గుర్తులేని) range_రికార్డు.len); /* bsearch cost */ }

  మూస <రకం_పేరు సమితి_t>
  తార్కిక collect_coverage (సమితి_t *glyphs) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి స్వయంచాలక& range: range_రికార్డు)
      ఒకవేళ (unlikely (!range.collect_coverage (glyphs)))
        తిరిగి అబద్ధం;
    తిరిగి నిజం;
  }

  బహిరంగ:
  /* Older compilers need this to be public. */
  నిర్మాణం iter_t
  {
    శూన్య init (స్థిరరాశి CoverageFormat2_4 &c_)
    {
      c = &c_;
      coverage = 0;
      i = 0;
      j = c->range_రికార్డు.len ? c->range_రికార్డు[0].మొదటి : 0;
      ఒకవేళ (unlikely (c->range_రికార్డు[0].మొదటి > c->range_రికార్డు[0].చివరి))
      {
        /* Broken table. Skip. */
        i = c->range_రికార్డు.len;
        j = 0;
      }
    }
    తార్కిక __more__ () స్థిరరాశి { తిరిగి i < c->range_రికార్డు.len; }
    శూన్య __next__ ()
    {
      ఒకవేళ (j >= c->range_రికార్డు[i].చివరి)
      {
        i++;
        ఒకవేళ (__more__ ())
        {
          గుర్తులేని పూర్ణాంకం old = coverage;
          j = c->range_రికార్డు.arrayZ[i].మొదటి;
          coverage = c->range_రికార్డు.arrayZ[i].value;
          ఒకవేళ (unlikely (coverage != old + 1))
          {
            /* Broken table. Skip. Important to avoid DoS.
             * Also, our callers depend on coverage being
             * consecutive and monotonically increasing,
             * ie. iota(). */
           i = c->range_రికార్డు.len;
           j = 0;
           తిరిగి;
          }
        }
        లేకపోతే
          j = 0;
        తిరిగి;
      }
      coverage++;
      j++;
    }
    hb_codepoint_t get_glyph () స్థిరరాశి { తిరిగి j; }
    తార్కిక క్రియాకారకం != (స్థిరరాశి iter_t& o) స్థిరరాశి
    { తిరిగి i != o.i || j != o.j; }
    iter_t __end__ () స్థిరరాశి
    {
      iter_t it;
      it.init (*c);
      it.i = c->range_రికార్డు.len;
      it.j = 0;
      తిరిగి it;
    }

    అంతరంగ:
    స్థిరరాశి నిర్మాణం CoverageFormat2_4 *c;
    గుర్తులేని పూర్ణాంకం i, coverage;
    hb_codepoint_t j;
  };
  అంతరంగ:
};

}
}
}

#అయితే_ముగింపు  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT2_HH

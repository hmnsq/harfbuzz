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


#నిర్వచితం_కాకపోతే OT_LAYOUT_COMMON_COVERAGEFORMAT1_HH
#నిర్వచించు OT_LAYOUT_COMMON_COVERAGEFORMAT1_HH

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ Common {

#నిర్వచించు NOT_COVERED             ((గుర్తులేని పూర్ణాంకం) -1)

మూస <రకం_పేరు Types>
నిర్మాణం CoverageFormat1_3
{
  మిత్ర నిర్మాణం Coverage;

  బహిరంగ:
  HBUINT16      coverageFormat; /* Format identifier--format = 1 */
  రకం_పేరు Types::మూస Sorted_Array_యొక్క<రకం_పేరు Types::HBGlyphID>
                glyphArray;     /* Array of GlyphIDs--in numerical order */

  DEFINE_కొలత_ARRAY (2 + Types::HBUINT::static_కొలత, glyphArray);

  అంతరంగ:
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (glyphArray.sanitize (c));
  }

  గుర్తులేని పూర్ణాంకం get_coverage (hb_codepoint_t glyph_id) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం i;
    glyphArray.bfind (glyph_id, &i, HB_NOT_దొరికింది_STORE, NOT_COVERED);
    తిరిగి i;
  }

  గుర్తులేని get_population () స్థిరరాశి
  {
    తిరిగి glyphArray.len;
  }

  మూస <రకం_పేరు Iterator,
      hb_requires (hb_is_sorted_ఆధారం_యొక్క (Iterator, hb_codepoint_t))>
  తార్కిక serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (ఇది);
    return_trace (glyphArray.serialize (c, glyphs));
  }

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs) స్థిరరాశి
  {
    ఒకవేళ (glyphArray.len > glyphs->get_population () * hb_బిట్టు_storage ((గుర్తులేని) glyphArray.len))
    {
      కొరకు (స్వయంచాలక g : *glyphs)
        ఒకవేళ (get_coverage (g) != NOT_COVERED)
	  తిరిగి నిజం;
      తిరిగి అబద్ధం;
    }

    కొరకు (స్థిరరాశి స్వయంచాలక& g : glyphArray.as_array ())
      ఒకవేళ (glyphs->has (g))
        తిరిగి నిజం;
    తిరిగి అబద్ధం;
  }
  తార్కిక intersects_coverage (స్థిరరాశి hb_సమితి_t *glyphs, గుర్తులేని పూర్ణాంకం సూచీ_2) స్థిరరాశి
  { తిరిగి glyphs->has (glyphArray[సూచీ_2]); }

  మూస <రకం_పేరు Iterable_బహిర్గతం,
	    hb_requires (hb_is_sink_యొక్క (Iterable_బహిర్గతం, hb_codepoint_t))>
  శూన్య intersect_సమితి (స్థిరరాశి hb_సమితి_t &glyphs, Iterable_బహిర్గతం&& intersect_glyphs) స్థిరరాశి
  {
    గుర్తులేని లెక్క = glyphArray.len;
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
      ఒకవేళ (glyphs.has (glyphArray[i]))
        intersect_glyphs << glyphArray[i];
  }

  గుర్తులేని ఖర్చు () స్థిరరాశి { తిరిగి hb_బిట్టు_storage ((గుర్తులేని) glyphArray.len); /* bsearch cost */ }

  మూస <రకం_పేరు సమితి_t>
  తార్కిక collect_coverage (సమితి_t *glyphs) స్థిరరాశి
  { తిరిగి glyphs->కూడు_sorted_array (glyphArray.as_array ()); }

  బహిరంగ:
  /* Older compilers need this to be public. */
  నిర్మాణం iter_t
  {
    శూన్య init (స్థిరరాశి నిర్మాణం CoverageFormat1_3 &c_) { c = &c_; i = 0; }
    తార్కిక __more__ () స్థిరరాశి { తిరిగి i < c->glyphArray.len; }
    శూన్య __next__ () { i++; }
    hb_codepoint_t get_glyph () స్థిరరాశి { తిరిగి c->glyphArray[i]; }
    తార్కిక క్రియాకారకం != (స్థిరరాశి iter_t& o) స్థిరరాశి
    { తిరిగి i != o.i; }
    iter_t __end__ () స్థిరరాశి { iter_t it; it.init (*c); it.i = c->glyphArray.len; తిరిగి it; }

    అంతరంగ:
    స్థిరరాశి నిర్మాణం CoverageFormat1_3 *c;
    గుర్తులేని పూర్ణాంకం i;
  };
  అంతరంగ:
};

}
}
}

#అయితే_ముగింపు  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT1_HH

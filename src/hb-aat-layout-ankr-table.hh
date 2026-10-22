/*
 * Copyright © 2018  Ebrahim Byagowi
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
 */

#నిర్వచితం_కాకపోతే HB_AAT_LAYOUT_ANKR_పట్టిక_HH
#నిర్వచించు HB_AAT_LAYOUT_ANKR_పట్టిక_HH

#అన్తర్భావ "hb-aat-layout-common.hh"

/*
 * ankr -- Anchor Point
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6ankr.html
 */
#నిర్వచించు HB_AAT_ట్యాగ్_ankr HB_ట్యాగ్('a','n','k','r')


నామాకాశ AAT {

ఉపయోగించి నామాకాశ OT;


నిర్మాణం Anchor
{
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  బహిరంగ:
  FWORD		xCoordinate;
  FWORD		yCoordinate;
  బహిరంగ:
  DEFINE_కొలత_STATIC (4);
};

రకనిర్వచనం Array32_యొక్క<Anchor> GlyphAnchors;

నిర్మాణం ankr
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_AAT_ట్యాగ్_ankr;

  స్థిరరాశి Anchor &get_anchor (hb_codepoint_t glyph_id,
			    గుర్తులేని పూర్ణాంకం i,
			    గుర్తులేని పూర్ణాంకం అంకె_glyphs) స్థిరరాశి
  {
    స్థిరరాశి NNOffset16_గమ్యం<GlyphAnchors> *స్థానభ్రంశం = (ఇది+lookup_పట్టిక).get_విలువ (glyph_id, అంకె_glyphs);
    ఒకవేళ (!స్థానభ్రంశం)
      తిరిగి Null (Anchor);
    స్థిరరాశి GlyphAnchors &anchors = &(ఇది+anchor_దత్తాంశం) + *స్థానభ్రంశం;
    తిరిగి anchors[i];
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (likely (c->check_struct (ఇది) &&
			  hb_barrier () &&
			  సంస్కరణ == 0 &&
			  c->check_range (ఇది, anchor_దత్తాంశం) &&
			  lookup_పట్టిక.sanitize (c, ఇది, &(ఇది+anchor_దత్తాంశం))));
  }

  రక్షిత:
  HBUINT16	సంస్కరణ;	/* Version number (set to zero) */
  HBUINT16	flags;		/* Flags (currently unused; set to zero) */
  Offset32_గమ్యం<Lookup<NNOffset16_గమ్యం<GlyphAnchors>>>
		lookup_పట్టిక;	/* Offset to the table's lookup table */
  NNOffset32_గమ్యం<HBUINT8>
		anchor_దత్తాంశం;	/* Offset to the glyph data table */

  బహిరంగ:
  DEFINE_కొలత_STATIC (12);
};

} /* namespace AAT */


#అయితే_ముగింపు /* HB_AAT_LAYOUT_ANKR_TABLE_HH */

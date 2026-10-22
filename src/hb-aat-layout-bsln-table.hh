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

#నిర్వచితం_కాకపోతే HB_AAT_LAYOUT_BSLN_పట్టిక_HH
#నిర్వచించు HB_AAT_LAYOUT_BSLN_పట్టిక_HH

#అన్తర్భావ "hb-aat-layout-common.hh"

/*
 * bsln -- Baseline
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6bsln.html
 */
#నిర్వచించు HB_AAT_ట్యాగ్_bsln HB_ట్యాగ్('b','s','l','n')


నామాకాశ AAT {


నిర్మాణం Baseline_పట్టిక_Format0_Part
{
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  రక్షిత:
  // Roman, Ideographic centered, Ideographic low, Hanging and Math
  // are the default defined ones, but any other maybe accessed also.
  HBINT16	deltas[32];	/* These are the FUnit distance deltas from
				 * the font's natural baseline to the other
				 * baselines used in the font. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (64);
};

నిర్మాణం Baseline_పట్టిక_Format1_Part
{
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (likely (c->check_struct (ఇది) &&
			  lookup_పట్టిక.sanitize (c)));
  }

  రక్షిత:
  HBINT16	deltas[32];	/* ditto */
  Lookup<HBUINT16>
		lookup_పట్టిక;	/* Lookup table that maps glyphs to their
				 * baseline values. */
  బహిరంగ:
  DEFINE_కొలత_MIN (66);
};

నిర్మాణం Baseline_పట్టిక_Format2_Part
{
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  రక్షిత:
  HBGlyphID16	stdGlyph;	/* The specific glyph index number in this
				 * font that is used to set the baseline values.
				 * This is the standard glyph.
				 * This glyph must contain a set of control points
				 * (whose numbers are contained in the ctlPoints field)
				 * that are used to determine baseline distances. */
  HBUINT16	ctlPoints[32];	/* Set of control point numbers,
				 * associated with the standard glyph.
				 * A value of 0xFFFF means there is no corresponding
				 * control point in the standard glyph. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (66);
};

నిర్మాణం Baseline_పట్టిక_Format3_Part
{
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (likely (c->check_struct (ఇది) && lookup_పట్టిక.sanitize (c)));
  }

  రక్షిత:
  HBGlyphID16	stdGlyph;	/* ditto */
  HBUINT16	ctlPoints[32];	/* ditto */
  Lookup<HBUINT16>
		lookup_పట్టిక;	/* Lookup table that maps glyphs to their
				 * baseline values. */
  బహిరంగ:
  DEFINE_కొలత_MIN (68);
};

నిర్మాణం bsln
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_AAT_ట్యాగ్_bsln;

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (unlikely (!(c->check_struct (ఇది) && defaultBaseline < 32)))
      return_trace (అబద్ధం);
    hb_barrier ();

    ఎంపిక (format)
    {
    స్థితి 0: hb_barrier (); return_trace (parts.format0.sanitize (c));
    స్థితి 1: hb_barrier (); return_trace (parts.format1.sanitize (c));
    స్థితి 2: hb_barrier (); return_trace (parts.format2.sanitize (c));
    స్థితి 3: hb_barrier (); return_trace (parts.format3.sanitize (c));
    అప్రమేయ:return_trace (నిజం);
    }
  }

  రక్షిత:
  Fixed_సంస్కరణ<>సంస్కరణ;	/* Version number of the Baseline table. */
  HBUINT16	format;		/* Format of the baseline table. Only one baseline
				 * format may be selected for the font. */
  HBUINT16	defaultBaseline;/* Default baseline value for all glyphs.
				 * This value can be from 0 through 31. */
  సంఘం {
  // Distance-Based Formats
  Baseline_పట్టిక_Format0_Part	format0;
  Baseline_పట్టిక_Format1_Part	format1;
  // Control Point-based Formats
  Baseline_పట్టిక_Format2_Part	format2;
  Baseline_పట్టిక_Format3_Part	format3;
  } parts;
  బహిరంగ:
  DEFINE_కొలత_MIN (8);
};

} /* namespace AAT */


#అయితే_ముగింపు /* HB_AAT_LAYOUT_BSLN_TABLE_HH */

/*
 * Copyright © 2011,2012  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#నిర్వచితం_కాకపోతే HB_OT_HHEA_పట్టిక_HH
#నిర్వచించు HB_OT_HHEA_పట్టిక_HH

#అన్తర్భావ "hb-open-type.hh"
#అన్తర్భావ "hb-ot-dual.hh"

/*
 * hhea -- Horizontal Header
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hhea
 * vhea -- Vertical Header
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vhea
 */
#నిర్వచించు HB_OT_ట్యాగ్_hhea HB_ట్యాగ్('h','h','e','a')
#నిర్వచించు HB_OT_ట్యాగ్_HHEA HB_ట్యాగ్('H','H','E','A')
#నిర్వచించు HB_OT_ట్యాగ్_vhea HB_ట్యాగ్('v','h','e','a')
#నిర్వచించు HB_OT_ట్యాగ్_VHEA HB_ట్యాగ్('V','H','E','A')


నామాకాశ OT {


మూస <రకం_పేరు T, రకం_పేరు Types>
నిర్మాణం _hea
{
  ఉపయోగించి Long_Metric_లెక్క = రకం_పేరు Types::HBLUINT;

  స్థిర constexpr గుర్తులేని long_metric_లెక్క_max ()
  { తిరిగి Long_Metric_లెక్క::static_కొలత == 4 ? 0xFFFFFFFFu : 0xFFFFu; }

  తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి సంస్కరణ.major; }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
		  likely (సంస్కరణ.major == 1));
  }

  బహిరంగ:
  Fixed_సంస్కరణ<>సంస్కరణ;	/* 0x00010000u for version 1.0. */
  FWORD		ascender;	/* Typographic ascent. */
  FWORD		descender;	/* Typographic descent. */
  FWORD		పంక్తి_Gap;	/* Typographic line gap. */
  UFWORD	advanceMax;	/* Maximum advance width/height value in
				 * metrics table. */
  FWORD		minLeadingBearing;
				/* Minimum left/top sidebearing value in
				 * metrics table. */
  FWORD		minTrailingBearing;
				/* Minimum right/bottom sidebearing value;
				 * calculated as Min(aw - lsb -
				 * (xMax - xMin)) for horizontal. */
  FWORD		maxExtent;	/* horizontal: Max(lsb + (xMax - xMin)),
				 * vertical: minLeadingBearing+(yMax-yMin). */
  HBINT16	caret_వాలు_Rise;	/* Used to calculate the slope of the
				 * cursor (rise/run); 1 for vertical caret,
				 * 0 for horizontal.*/
  HBINT16	caret_వాలు_నిరంతరం;	/* 0 for vertical caret, 1 for horizontal. */
  HBINT16	caret_స్థానభ్రంశం;	/* The amount by which a slanted
				 * highlight on a glyph needs
				 * to be shifted to produce the
				 * best appearance. Set to 0 for
				 * non-slanted fonts. */
  HBINT16	reserved1;	/* Set to 0. */
  HBINT16	reserved2;	/* Set to 0. */
  HBINT16	reserved3;	/* Set to 0. */
  HBINT16	reserved4;	/* Set to 0. */
  HBINT16	metric_దత్తాంశం_Format;/* 0 for current format. */
  Long_Metric_లెక్క
		సంఖ్య_యొక్క_Long_Metrics;
				/* Number of LongMetric entries in metric
				 * table. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (34 + Types::HBLUINT::static_కొలత);
};

నిర్మాణం hhea : _hea<hhea, Layout::SmallTypes> {
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_hhea;
};
నిర్మాణం HHEA : _hea<HHEA, Layout::MediumTypes> {
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_HHEA;
};
నిర్మాణం vhea : _hea<vhea, Layout::SmallTypes> {
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_vhea;
};
నిర్మాణం VHEA : _hea<VHEA, Layout::MediumTypes> {
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_VHEA;
};

నిర్మాణం hhea_accelerator_t : hb_dual_పట్టిక_t<hhea, HHEA>
{
  hhea_accelerator_t (hb_face_t *face) :
    hb_dual_పట్టిక_t<hhea, HHEA> (face) {}
};

నిర్మాణం vhea_accelerator_t : hb_dual_పట్టిక_t<vhea, VHEA>
{
  vhea_accelerator_t (hb_face_t *face) :
    hb_dual_పట్టిక_t<vhea, VHEA> (face) {}
};


} /* namespace OT */


#అయితే_ముగింపు /* HB_OT_HHEA_TABLE_HH */

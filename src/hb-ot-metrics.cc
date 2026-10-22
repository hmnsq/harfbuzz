/*
 * Copyright © 2018-2019  Ebrahim Byagowi
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

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-ot-var-mvar-table.hh"
#అన్తర్భావ "hb-ot-gasp-table.hh" // Just so we compile it; unused otherwise.
#అన్తర్భావ "hb-ot-os2-table.hh"
#అన్తర్భావ "hb-ot-post-table.hh"
#అన్తర్భావ "hb-ot-hhea-table.hh"
#అన్తర్భావ "hb-ot-metrics.hh"
#అన్తర్భావ "hb-ot-face.hh"


/**
 * SECTION:hb-ot-metrics
 * @title: hb-ot-metrics
 * @short_description: OpenType Metrics
 * @include: hb-ot.h
 *
 * Functions for fetching metrics from fonts.
 **/

స్థిర దశాంశం
_fix_ascender_descender (దశాంశం value, hb_ot_metrics_ట్యాగ్_t metrics_ట్యాగ్)
{
  ఒకవేళ (metrics_ట్యాగ్ == HB_OT_METRICS_ట్యాగ్_HORIZONTAL_ASCENDER ||
      metrics_ట్యాగ్ == HB_OT_METRICS_ట్యాగ్_VERTICAL_ASCENDER)
    తిరిగి fabs ((ద్విగుణ) value);
  ఒకవేళ (metrics_ట్యాగ్ == HB_OT_METRICS_ట్యాగ్_HORIZONTAL_DESCENDER ||
      metrics_ట్యాగ్ == HB_OT_METRICS_ట్యాగ్_VERTICAL_DESCENDER)
    తిరిగి -fabs ((ద్విగుణ) value);
  తిరిగి value;
}

/* The common part of _get_position logic needed on hb-ot-font and here
   to be able to have slim builds without the not always needed parts */
తార్కిక
_hb_ot_metrics_get_position_common (hb_font_t           *font,
				    hb_ot_metrics_ట్యాగ్_t  metrics_ట్యాగ్,
				    hb_position_t       *position     /* OUT.  May be NULL. */)
{
  hb_face_t *face = font->face;
  ఎంపిక ((గుర్తులేని) metrics_ట్యాగ్)
  {
#నిర్వచితం_కాకపోతే HB_NO_VAR
#నిర్వచించు GET_VAR face->పట్టిక.MVAR->get_var (metrics_ట్యాగ్, font->coords, font->అంకె_coords)
#లేకపోతే
#నిర్వచించు GET_VAR .0f
#అయితే_ముగింపు
#నిర్వచించు GET_METRIC_Y(పట్టిక_4, ATTR) \
  (face->పట్టిక.పట్టిక_4->has_దత్తాంశం () && \
    ((శూన్య) (position && (*position = font->em_scalef_y (_fix_ascender_descender ( \
      face->పట్టిక.పట్టిక_4->ATTR + GET_VAR, metrics_ట్యాగ్)))), నిజం))
#నిర్వచించు GET_HEA_METRIC_X(పట్టిక_4, ATTR) \
  (face->పట్టిక.పట్టిక_4->has_దత్తాంశం () && \
    ((శూన్య) (position && (*position = font->em_scalef_x (_fix_ascender_descender ( \
      HB_DUAL_GET (*face->పట్టిక.పట్టిక_4, పట్టిక->ATTR) + GET_VAR, metrics_ట్యాగ్)))), నిజం))
#నిర్వచించు GET_HEA_METRIC_Y(పట్టిక_4, ATTR) \
  (face->పట్టిక.పట్టిక_4->has_దత్తాంశం () && \
    ((శూన్య) (position && (*position = font->em_scalef_y (_fix_ascender_descender ( \
      HB_DUAL_GET (*face->పట్టిక.పట్టిక_4, పట్టిక->ATTR) + GET_VAR, metrics_ట్యాగ్)))), నిజం))

  స్థితి HB_OT_METRICS_ట్యాగ్_HORIZONTAL_ASCENDER:
    తిరిగి (face->పట్టిక.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, sTypoAscender)) ||
	   GET_HEA_METRIC_Y (hhea, ascender);
  స్థితి HB_OT_METRICS_ట్యాగ్_HORIZONTAL_DESCENDER:
    తిరిగి (face->పట్టిక.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, sTypoDescender)) ||
	   GET_HEA_METRIC_Y (hhea, descender);
  స్థితి HB_OT_METRICS_ట్యాగ్_HORIZONTAL_పంక్తి_GAP:
    తిరిగి (face->పట్టిక.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, s_Typo_పంక్తి_Gap)) ||
	   GET_HEA_METRIC_Y (hhea, పంక్తి_Gap);

#నిర్వచితం_కాకపోతే HB_NO_VERTICAL
  స్థితి HB_OT_METRICS_ట్యాగ్_VERTICAL_ASCENDER:  తిరిగి GET_HEA_METRIC_X (vhea, ascender);
  స్థితి HB_OT_METRICS_ట్యాగ్_VERTICAL_DESCENDER: తిరిగి GET_HEA_METRIC_X (vhea, descender);
  స్థితి HB_OT_METRICS_ట్యాగ్_VERTICAL_పంక్తి_GAP:  తిరిగి GET_HEA_METRIC_X (vhea, పంక్తి_Gap);
#అయితే_ముగింపు

#నిర్వచనం_రద్దు GET_HEA_METRIC_Y
#నిర్వచనం_రద్దు GET_HEA_METRIC_X
#నిర్వచనం_రద్దు GET_METRIC_Y
#నిర్వచనం_రద్దు GET_VAR
  అప్రమేయ:                               ప్రతిజ్ఞా (0); తిరిగి అబద్ధం;
  }
}

#నిర్వచితం_కాకపోతే HB_NO_METRICS

#ఒకవేళ 0
స్థిర తార్కిక
_get_gasp (hb_face_t *face, దశాంశం *ఫలితం, hb_ot_metrics_ట్యాగ్_t metrics_ట్యాగ్)
{
  స్థిరరాశి OT::GaspRange& range = face->పట్టిక.gasp->get_gasp_range (metrics_ట్యాగ్ - HB_ట్యాగ్ ('g','s','p','0'));
  ఒకవేళ (&range == &Null (OT::GaspRange)) తిరిగి అబద్ధం;
  ఒకవేళ (ఫలితం) *ఫలితం = range.rangeMaxPPEM + font->face->పట్టిక.MVAR->get_var (metrics_ట్యాగ్, font->coords, font->అంకె_coords);
  తిరిగి నిజం;
}
#అయితే_ముగింపు

/* Private tags for https://github.com/harfbuzz/harfbuzz/issues/1866 */
#నిర్వచించు HB_OT_METRICS_ట్యాగ్_HORIZONTAL_ASCENDER_OS2   HB_ట్యాగ్ ('O','a','s','c')
#నిర్వచించు HB_OT_METRICS_ట్యాగ్_HORIZONTAL_ASCENDER_HHEA  HB_ట్యాగ్ ('H','a','s','c')
#నిర్వచించు HB_OT_METRICS_ట్యాగ్_HORIZONTAL_DESCENDER_OS2  HB_ట్యాగ్ ('O','d','s','c')
#నిర్వచించు HB_OT_METRICS_ట్యాగ్_HORIZONTAL_DESCENDER_HHEA HB_ట్యాగ్ ('H','d','s','c')
#నిర్వచించు HB_OT_METRICS_ట్యాగ్_HORIZONTAL_పంక్తి_GAP_OS2   HB_ట్యాగ్ ('O','l','g','p')
#నిర్వచించు HB_OT_METRICS_ట్యాగ్_HORIZONTAL_పంక్తి_GAP_HHEA  HB_ట్యాగ్ ('H','l','g','p')

/**
 * hb_ot_metrics_get_position:
 * @font: an #hb_font_t object.
 * @metrics_tag: tag of metrics value you like to fetch.
 * @position: (out) (optional): result of metrics value from the font.
 *
 * Fetches metrics value corresponding to @metrics_tag from @font.
 *
 * Returns: Whether found the requested metrics in the font.
 * Since: 2.6.0
 **/
hb_bool_t
hb_ot_metrics_get_position (hb_font_t           *font,
			    hb_ot_metrics_ట్యాగ్_t  metrics_ట్యాగ్,
			    hb_position_t       *position     /* OUT.  May be NULL. */)
{
  hb_face_t *face = font->face;
  ఎంపిక ((గుర్తులేని) metrics_ట్యాగ్)
  {
  స్థితి HB_OT_METRICS_ట్యాగ్_HORIZONTAL_ASCENDER:
  స్థితి HB_OT_METRICS_ట్యాగ్_HORIZONTAL_DESCENDER:
  స్థితి HB_OT_METRICS_ట్యాగ్_HORIZONTAL_పంక్తి_GAP:
  స్థితి HB_OT_METRICS_ట్యాగ్_VERTICAL_ASCENDER:
  స్థితి HB_OT_METRICS_ట్యాగ్_VERTICAL_DESCENDER:
  స్థితి HB_OT_METRICS_ట్యాగ్_VERTICAL_పంక్తి_GAP:           తిరిగి _hb_ot_metrics_get_position_common (font, metrics_ట్యాగ్, position);
#నిర్వచితం_కాకపోతే HB_NO_VAR
#నిర్వచించు GET_VAR hb_ot_metrics_get_variation (font, metrics_ట్యాగ్)
#లేకపోతే
#నిర్వచించు GET_VAR 0
#అయితే_ముగింపు
#నిర్వచించు GET_METRIC_X(పట్టిక_4, ATTR) \
  (face->పట్టిక.పట్టిక_4->has_దత్తాంశం () && \
    ((శూన్య) (position && (*position = font->em_scalef_x (face->పట్టిక.పట్టిక_4->ATTR + GET_VAR))), నిజం))
#నిర్వచించు GET_METRIC_Y(పట్టిక_4, ATTR) \
  (face->పట్టిక.పట్టిక_4->has_దత్తాంశం () && \
    ((శూన్య) (position && (*position = font->em_scalef_y (face->పట్టిక.పట్టిక_4->ATTR + GET_VAR))), నిజం))
#నిర్వచించు GET_HEA_METRIC_X(పట్టిక_4, ATTR) \
  (face->పట్టిక.పట్టిక_4->has_దత్తాంశం () && \
    ((శూన్య) (position && (*position = font->em_scalef_x (HB_DUAL_GET (*face->పట్టిక.పట్టిక_4, పట్టిక->ATTR) + GET_VAR))), నిజం))
#నిర్వచించు GET_HEA_METRIC_Y(పట్టిక_4, ATTR) \
  (face->పట్టిక.పట్టిక_4->has_దత్తాంశం () && \
    ((శూన్య) (position && (*position = font->em_scalef_y (HB_DUAL_GET (*face->పట్టిక.పట్టిక_4, పట్టిక->ATTR) + GET_VAR))), నిజం))
  స్థితి HB_OT_METRICS_ట్యాగ్_HORIZONTAL_CLIPPING_ASCENT:  తిరిగి GET_METRIC_Y (OS2, usWinAscent);
  స్థితి HB_OT_METRICS_ట్యాగ్_HORIZONTAL_CLIPPING_DESCENT: తిరిగి GET_METRIC_Y (OS2, usWinDescent);

  స్థితి HB_OT_METRICS_ట్యాగ్_HORIZONTAL_CARET_RISE:
  స్థితి HB_OT_METRICS_ట్యాగ్_HORIZONTAL_CARET_నిరంతరం:
  {
    గుర్తులేని mult = 1u;

    ఒకవేళ (font->slant)
    {
      గుర్తులేని rise = HB_DUAL_GET (*face->పట్టిక.hhea, పట్టిక->caret_వాలు_Rise);
      గుర్తులేని upem = face->get_upem ();
      mult = (rise && rise < upem) ? hb_min (upem / rise, 256u) : 1u;
    }

    ఒకవేళ (metrics_ట్యాగ్ == HB_OT_METRICS_ట్యాగ్_HORIZONTAL_CARET_RISE)
    {
      తార్కిక ret = GET_HEA_METRIC_Y (hhea, caret_వాలు_Rise);

      ఒకవేళ (position)
	*position = hb_clamp_గమ్యం<hb_position_t> ((int64_t) *position * mult);

      తిరిగి ret;
    }
    లేకపోతే
    {
      hb_position_t rise = 0;

      ఒకవేళ (font->slant && position && GET_HEA_METRIC_Y (hhea, caret_వాలు_Rise))
	rise = *position;

      తార్కిక ret = GET_HEA_METRIC_X (hhea, caret_వాలు_నిరంతరం);

      ఒకవేళ (position)
      {
	*position = hb_clamp_గమ్యం<hb_position_t> ((int64_t) *position * mult);

	ఒకవేళ (font->slant)
	  *position = hb_saturate_కూడు (*position,
				       hb_clamp_గమ్యం<hb_position_t> (roundf (mult * font->slant_xy * rise)));
      }

      తిరిగి ret;
    }
  }
  స్థితి HB_OT_METRICS_ట్యాగ్_HORIZONTAL_CARET_స్థానభ్రంశం:     తిరిగి GET_HEA_METRIC_X (hhea, caret_స్థానభ్రంశం);

#నిర్వచితం_కాకపోతే HB_NO_VERTICAL
  స్థితి HB_OT_METRICS_ట్యాగ్_VERTICAL_CARET_RISE:         తిరిగి GET_HEA_METRIC_X (vhea, caret_వాలు_Rise);
  స్థితి HB_OT_METRICS_ట్యాగ్_VERTICAL_CARET_నిరంతరం:          తిరిగి GET_HEA_METRIC_Y (vhea, caret_వాలు_నిరంతరం);
  స్థితి HB_OT_METRICS_ట్యాగ్_VERTICAL_CARET_స్థానభ్రంశం:       తిరిగి GET_HEA_METRIC_Y (vhea, caret_స్థానభ్రంశం);
#అయితే_ముగింపు
  స్థితి HB_OT_METRICS_ట్యాగ్_X_ఎత్తు:                    తిరిగి GET_METRIC_Y (OS2->v2 (), sx_ఎత్తు);
  స్థితి HB_OT_METRICS_ట్యాగ్_CAP_ఎత్తు:                  తిరిగి GET_METRIC_Y (OS2->v2 (), s_Cap_ఎత్తు);
  స్థితి HB_OT_METRICS_ట్యాగ్_SUBSCRIPT_EM_X_కొలత:         తిరిగి GET_METRIC_X (OS2, ySubscriptXSize);
  స్థితి HB_OT_METRICS_ట్యాగ్_SUBSCRIPT_EM_Y_కొలత:         తిరిగి GET_METRIC_Y (OS2, ySubscriptYSize);
  స్థితి HB_OT_METRICS_ట్యాగ్_SUBSCRIPT_EM_X_స్థానభ్రంశం:       తిరిగి GET_METRIC_X (OS2, ySubscriptXOffset);
  స్థితి HB_OT_METRICS_ట్యాగ్_SUBSCRIPT_EM_Y_స్థానభ్రంశం:       తిరిగి GET_METRIC_Y (OS2, ySubscriptYOffset);
  స్థితి HB_OT_METRICS_ట్యాగ్_SUPERSCRIPT_EM_X_కొలత:       తిరిగి GET_METRIC_X (OS2, ySuperscriptXSize);
  స్థితి HB_OT_METRICS_ట్యాగ్_SUPERSCRIPT_EM_Y_కొలత:       తిరిగి GET_METRIC_Y (OS2, ySuperscriptYSize);
  స్థితి HB_OT_METRICS_ట్యాగ్_SUPERSCRIPT_EM_X_స్థానభ్రంశం:     తిరిగి GET_METRIC_X (OS2, ySuperscriptXOffset);
  స్థితి HB_OT_METRICS_ట్యాగ్_SUPERSCRIPT_EM_Y_స్థానభ్రంశం:     తిరిగి GET_METRIC_Y (OS2, ySuperscriptYOffset);
  స్థితి HB_OT_METRICS_ట్యాగ్_STRIKEOUT_కొలత:              తిరిగి GET_METRIC_Y (OS2, y_Strikeout_కొలత);
  స్థితి HB_OT_METRICS_ట్యాగ్_STRIKEOUT_స్థానభ్రంశం:            తిరిగి GET_METRIC_Y (OS2, yStrikeoutPosition);
  స్థితి HB_OT_METRICS_ట్యాగ్_UNDERLINE_కొలత:              తిరిగి GET_METRIC_Y (post->పట్టిక, underlineThickness);
  స్థితి HB_OT_METRICS_ట్యాగ్_UNDERLINE_స్థానభ్రంశం:            తిరిగి GET_METRIC_Y (post->పట్టిక, underlinePosition);

  /* Private tags */
  స్థితి HB_OT_METRICS_ట్యాగ్_HORIZONTAL_ASCENDER_OS2:    తిరిగి GET_METRIC_Y (OS2, sTypoAscender);
  స్థితి HB_OT_METRICS_ట్యాగ్_HORIZONTAL_ASCENDER_HHEA:   తిరిగి GET_HEA_METRIC_Y (hhea, ascender);
  స్థితి HB_OT_METRICS_ట్యాగ్_HORIZONTAL_DESCENDER_OS2:   తిరిగి GET_METRIC_Y (OS2, sTypoDescender);
  స్థితి HB_OT_METRICS_ట్యాగ్_HORIZONTAL_DESCENDER_HHEA:  తిరిగి GET_HEA_METRIC_Y (hhea, descender);
  స్థితి HB_OT_METRICS_ట్యాగ్_HORIZONTAL_పంక్తి_GAP_OS2:    తిరిగి GET_METRIC_Y (OS2, s_Typo_పంక్తి_Gap);
  స్థితి HB_OT_METRICS_ట్యాగ్_HORIZONTAL_పంక్తి_GAP_HHEA:   తిరిగి GET_HEA_METRIC_Y (hhea, పంక్తి_Gap);
#నిర్వచనం_రద్దు GET_HEA_METRIC_Y
#నిర్వచనం_రద్దు GET_HEA_METRIC_X
#నిర్వచనం_రద్దు GET_METRIC_Y
#నిర్వచనం_రద్దు GET_METRIC_X
#నిర్వచనం_రద్దు GET_VAR
  అప్రమేయ:                                        తిరిగి అబద్ధం;
  }
}

/**
 * hb_ot_metrics_get_position_with_fallback:
 * @font: an #hb_font_t object.
 * @metrics_tag: tag of metrics value you like to fetch.
 * @position: (out) (optional): result of metrics value from the font.
 *
 * Fetches metrics value corresponding to @metrics_tag from @font,
 * and synthesizes a value if it the value is missing in the font.
 *
 * Since: 4.0.0
 **/
శూన్య
hb_ot_metrics_get_position_with_fallback (hb_font_t           *font,
					  hb_ot_metrics_ట్యాగ్_t  metrics_ట్యాగ్,
					  hb_position_t       *position     /*బహిర్గతం*/)
{
  hb_font_extents_t font_extents;
  hb_codepoint_t glyph;
  hb_glyph_extents_t extents;

  ఒకవేళ (hb_ot_metrics_get_position (font, metrics_ట్యాగ్, position))
    {
      ఒకవేళ ((metrics_ట్యాగ్ != HB_OT_METRICS_ట్యాగ్_STRIKEOUT_కొలత &&
           metrics_ట్యాగ్ != HB_OT_METRICS_ట్యాగ్_UNDERLINE_కొలత) ||
          *position != 0)
        తిరిగి;
    }

  ఎంపిక (metrics_ట్యాగ్)
  {
  స్థితి HB_OT_METRICS_ట్యాగ్_HORIZONTAL_ASCENDER:
  స్థితి HB_OT_METRICS_ట్యాగ్_HORIZONTAL_CLIPPING_ASCENT:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.ascender;
    ఆపు;

  స్థితి HB_OT_METRICS_ట్యాగ్_VERTICAL_ASCENDER:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ascender;
    ఆపు;

  స్థితి HB_OT_METRICS_ట్యాగ్_HORIZONTAL_DESCENDER:
  స్థితి HB_OT_METRICS_ట్యాగ్_HORIZONTAL_CLIPPING_DESCENT:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.descender;
    ఆపు;

  స్థితి HB_OT_METRICS_ట్యాగ్_VERTICAL_DESCENDER:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ascender;
    ఆపు;

  స్థితి HB_OT_METRICS_ట్యాగ్_HORIZONTAL_పంక్తి_GAP:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.పంక్తి_gap;
    ఆపు;

  స్థితి HB_OT_METRICS_ట్యాగ్_VERTICAL_పంక్తి_GAP:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.పంక్తి_gap;
    ఆపు;

  స్థితి HB_OT_METRICS_ట్యాగ్_HORIZONTAL_CARET_RISE:
  స్థితి HB_OT_METRICS_ట్యాగ్_VERTICAL_CARET_RISE:
    *position = 1;
    ఆపు;

  స్థితి HB_OT_METRICS_ట్యాగ్_HORIZONTAL_CARET_నిరంతరం:
  స్థితి HB_OT_METRICS_ట్యాగ్_VERTICAL_CARET_నిరంతరం:
    *position = 0;
    ఆపు;

  స్థితి HB_OT_METRICS_ట్యాగ్_HORIZONTAL_CARET_స్థానభ్రంశం:
  స్థితి HB_OT_METRICS_ట్యాగ్_VERTICAL_CARET_స్థానభ్రంశం:
    *position = 0;
    ఆపు;

  స్థితి HB_OT_METRICS_ట్యాగ్_X_ఎత్తు:
    ఒకవేళ (hb_font_get_nominal_glyph (font, 'x', &glyph) &&
        hb_font_get_glyph_extents (font, glyph, &extents))
      *position = extents.y_bearing;
    లేకపోతే
      *position = font->y_scale / 2;
    ఆపు;

  స్థితి HB_OT_METRICS_ట్యాగ్_CAP_ఎత్తు:
    ఒకవేళ (hb_font_get_nominal_glyph (font, 'O', &glyph) &&
        hb_font_get_glyph_extents (font, glyph, &extents))
      *position = hb_clamp_గమ్యం<hb_position_t> ((int64_t) extents.ఎత్తు +
					      2 * (int64_t) extents.y_bearing);
    లేకపోతే
      *position = hb_clamp_గమ్యం<hb_position_t> ((int64_t) font->y_scale * 2 / 3);
    ఆపు;

  స్థితి HB_OT_METRICS_ట్యాగ్_STRIKEOUT_కొలత:
  స్థితి HB_OT_METRICS_ట్యాగ్_UNDERLINE_కొలత:
    *position = font->y_scale / 18;
    ఆపు;

  స్థితి HB_OT_METRICS_ట్యాగ్_STRIKEOUT_స్థానభ్రంశం:
    {
      hb_position_t ascender;
      hb_ot_metrics_get_position_with_fallback (font,
                                                HB_OT_METRICS_ట్యాగ్_HORIZONTAL_ASCENDER,
                                                &ascender);
      *position = ascender / 2;
    }
    ఆపు;

  స్థితి HB_OT_METRICS_ట్యాగ్_UNDERLINE_స్థానభ్రంశం:
    *position = hb_saturate_neg (font->y_scale) / 18;
    ఆపు;

  స్థితి HB_OT_METRICS_ట్యాగ్_SUBSCRIPT_EM_X_కొలత:
  స్థితి HB_OT_METRICS_ట్యాగ్_SUPERSCRIPT_EM_X_కొలత:
    *position = hb_clamp_గమ్యం<hb_position_t> ((int64_t) font->x_scale * 10 / 12);
    ఆపు;

  స్థితి HB_OT_METRICS_ట్యాగ్_SUBSCRIPT_EM_Y_కొలత:
  స్థితి HB_OT_METRICS_ట్యాగ్_SUPERSCRIPT_EM_Y_కొలత:
    *position = hb_clamp_గమ్యం<hb_position_t> ((int64_t) font->y_scale * 10 / 12);
    ఆపు;

  స్థితి HB_OT_METRICS_ట్యాగ్_SUBSCRIPT_EM_X_స్థానభ్రంశం:
  స్థితి HB_OT_METRICS_ట్యాగ్_SUPERSCRIPT_EM_X_స్థానభ్రంశం:
    *position = 0;
    ఆపు;

  స్థితి HB_OT_METRICS_ట్యాగ్_SUBSCRIPT_EM_Y_స్థానభ్రంశం:
  స్థితి HB_OT_METRICS_ట్యాగ్_SUPERSCRIPT_EM_Y_స్థానభ్రంశం:
    *position = font->y_scale / 5;
    ఆపు;

  స్థితి HB_OT_METRICS_ట్యాగ్_MAX_విలువ:
  అప్రమేయ:
    *position = 0;
    ఆపు;
  }
}

#నిర్వచితం_కాకపోతే HB_NO_VAR
/**
 * hb_ot_metrics_get_variation:
 * @font: an #hb_font_t object.
 * @metrics_tag: tag of metrics value you like to fetch.
 *
 * Fetches metrics value corresponding to @metrics_tag from @font with the
 * current font variation settings applied.
 *
 * Returns: The requested metric value.
 *
 * Since: 2.6.0
 **/
దశాంశం
hb_ot_metrics_get_variation (hb_font_t *font, hb_ot_metrics_ట్యాగ్_t metrics_ట్యాగ్)
{
  తిరిగి font->face->పట్టిక.MVAR->get_var (metrics_ట్యాగ్, font->coords, font->అంకె_coords);
}

/**
 * hb_ot_metrics_get_x_variation:
 * @font: an #hb_font_t object.
 * @metrics_tag: tag of metrics value you like to fetch.
 *
 * Fetches horizontal metrics value corresponding to @metrics_tag from @font
 * with the current font variation settings applied.
 *
 * Returns: The requested metric value.
 *
 * Since: 2.6.0
 **/
hb_position_t
hb_ot_metrics_get_x_variation (hb_font_t *font, hb_ot_metrics_ట్యాగ్_t metrics_ట్యాగ్)
{
  తిరిగి font->em_scalef_x (hb_ot_metrics_get_variation (font, metrics_ట్యాగ్));
}

/**
 * hb_ot_metrics_get_y_variation:
 * @font: an #hb_font_t object.
 * @metrics_tag: tag of metrics value you like to fetch.
 *
 * Fetches vertical metrics value corresponding to @metrics_tag from @font with
 * the current font variation settings applied.
 *
 * Returns: The requested metric value.
 *
 * Since: 2.6.0
 **/
hb_position_t
hb_ot_metrics_get_y_variation (hb_font_t *font, hb_ot_metrics_ట్యాగ్_t metrics_ట్యాగ్)
{
  తిరిగి font->em_scalef_y (hb_ot_metrics_get_variation (font, metrics_ట్యాగ్));
}
#అయితే_ముగింపు

#అయితే_ముగింపు

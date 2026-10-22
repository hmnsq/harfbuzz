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

#समावेश "hb.hh"

#समावेश "hb-ot-var-mvar-table.hh"
#समावेश "hb-ot-gasp-table.hh" // Just so we compile it; unused otherwise.
#समावेश "hb-ot-os2-table.hh"
#समावेश "hb-ot-post-table.hh"
#समावेश "hb-ot-hhea-table.hh"
#समावेश "hb-ot-metrics.hh"
#समावेश "hb-ot-face.hh"


/**
 * SECTION:hb-ot-metrics
 * @title: hb-ot-metrics
 * @short_description: OpenType Metrics
 * @include: hb-ot.h
 *
 * Functions for fetching metrics from fonts.
 **/

अटल भग्न
_fix_ascender_descender (भग्न value, hb_ot_metrics_tag_t metrics_tag)
{
  अगर (metrics_tag == HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER ||
      metrics_tag == HB_OT_METRICS_TAG_VERTICAL_ASCENDER)
    वापस भ_असल ((द्विगुन) value);
  अगर (metrics_tag == HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER ||
      metrics_tag == HB_OT_METRICS_TAG_VERTICAL_DESCENDER)
    वापस -भ_असल ((द्विगुन) value);
  वापस value;
}

/* The common part of _get_position logic needed on hb-ot-font and here
   to be able to have slim builds without the not always needed parts */
bool
_hb_ot_metrics_get_position_common (hb_font_t           *font,
				    hb_ot_metrics_tag_t  metrics_tag,
				    hb_position_t       *position     /* OUT.  May be NULL. */)
{
  hb_face_t *face = font->face;
  चयन ((अचिन्हित) metrics_tag)
  {
#अगर_अघोषित HB_NO_VAR
#घोषणा GET_VAR face->table.MVAR->get_var (metrics_tag, font->coords, font->अंक_coords)
#अन्यथा
#घोषणा GET_VAR .0f
#पूर्ण_अगर
#घोषणा GET_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_तथ्य () && \
    ((व्योम) (position && (*position = font->em_scalef_y (_fix_ascender_descender ( \
      face->table.TABLE->ATTR + GET_VAR, metrics_tag)))), true))
#घोषणा GET_HEA_METRIC_X(TABLE, ATTR) \
  (face->table.TABLE->has_तथ्य () && \
    ((व्योम) (position && (*position = font->em_scalef_x (_fix_ascender_descender ( \
      HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR, metrics_tag)))), true))
#घोषणा GET_HEA_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_तथ्य () && \
    ((व्योम) (position && (*position = font->em_scalef_y (_fix_ascender_descender ( \
      HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR, metrics_tag)))), true))

  हाल HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER:
    वापस (face->table.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, sTypoAscender)) ||
	   GET_HEA_METRIC_Y (hhea, ascender);
  हाल HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER:
    वापस (face->table.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, sTypoDescender)) ||
	   GET_HEA_METRIC_Y (hhea, descender);
  हाल HB_OT_METRICS_TAG_HORIZONTAL_पंक्ति_GAP:
    वापस (face->table.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, s_Typo_पंक्ति_Gap)) ||
	   GET_HEA_METRIC_Y (hhea, पंक्ति_Gap);

#अगर_अघोषित HB_NO_VERTICAL
  हाल HB_OT_METRICS_TAG_VERTICAL_ASCENDER:  वापस GET_HEA_METRIC_X (vhea, ascender);
  हाल HB_OT_METRICS_TAG_VERTICAL_DESCENDER: वापस GET_HEA_METRIC_X (vhea, descender);
  हाल HB_OT_METRICS_TAG_VERTICAL_पंक्ति_GAP:  वापस GET_HEA_METRIC_X (vhea, पंक्ति_Gap);
#पूर्ण_अगर

#अघोषित GET_HEA_METRIC_Y
#अघोषित GET_HEA_METRIC_X
#अघोषित GET_METRIC_Y
#अघोषित GET_VAR
  शेष:                               निश्चित (0); वापस false;
  }
}

#अगर_अघोषित HB_NO_METRICS

#अगर 0
अटल bool
_get_gasp (hb_face_t *face, भग्न *परिणाम, hb_ot_metrics_tag_t metrics_tag)
{
  स्थिर OT::GaspRange& range = face->table.gasp->get_gasp_range (metrics_tag - HB_TAG ('g','s','p','0'));
  अगर (&range == &Null (OT::GaspRange)) वापस false;
  अगर (परिणाम) *परिणाम = range.rangeMaxPPEM + font->face->table.MVAR->get_var (metrics_tag, font->coords, font->अंक_coords);
  वापस true;
}
#पूर्ण_अगर

/* Private tags for https://github.com/harfbuzz/harfbuzz/issues/1866 */
#घोषणा _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_OS2   HB_TAG ('O','a','s','c')
#घोषणा _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_HHEA  HB_TAG ('H','a','s','c')
#घोषणा _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_OS2  HB_TAG ('O','d','s','c')
#घोषणा _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_HHEA HB_TAG ('H','d','s','c')
#घोषणा HB_OT_METRICS_TAG_HORIZONTAL_पंक्ति_GAP_OS2   HB_TAG ('O','l','g','p')
#घोषणा HB_OT_METRICS_TAG_HORIZONTAL_पंक्ति_GAP_HHEA  HB_TAG ('H','l','g','p')

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
			    hb_ot_metrics_tag_t  metrics_tag,
			    hb_position_t       *position     /* OUT.  May be NULL. */)
{
  hb_face_t *face = font->face;
  चयन ((अचिन्हित) metrics_tag)
  {
  हाल HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER:
  हाल HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER:
  हाल HB_OT_METRICS_TAG_HORIZONTAL_पंक्ति_GAP:
  हाल HB_OT_METRICS_TAG_VERTICAL_ASCENDER:
  हाल HB_OT_METRICS_TAG_VERTICAL_DESCENDER:
  हाल HB_OT_METRICS_TAG_VERTICAL_पंक्ति_GAP:           वापस _hb_ot_metrics_get_position_common (font, metrics_tag, position);
#अगर_अघोषित HB_NO_VAR
#घोषणा GET_VAR hb_ot_metrics_get_variation (font, metrics_tag)
#अन्यथा
#घोषणा GET_VAR 0
#पूर्ण_अगर
#घोषणा GET_METRIC_X(TABLE, ATTR) \
  (face->table.TABLE->has_तथ्य () && \
    ((व्योम) (position && (*position = font->em_scalef_x (face->table.TABLE->ATTR + GET_VAR))), true))
#घोषणा GET_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_तथ्य () && \
    ((व्योम) (position && (*position = font->em_scalef_y (face->table.TABLE->ATTR + GET_VAR))), true))
#घोषणा GET_HEA_METRIC_X(TABLE, ATTR) \
  (face->table.TABLE->has_तथ्य () && \
    ((व्योम) (position && (*position = font->em_scalef_x (HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR))), true))
#घोषणा GET_HEA_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_तथ्य () && \
    ((व्योम) (position && (*position = font->em_scalef_y (HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR))), true))
  हाल HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_ASCENT:  वापस GET_METRIC_Y (OS2, usWinAscent);
  हाल HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_DESCENT: वापस GET_METRIC_Y (OS2, usWinDescent);

  हाल HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE:
  हाल HB_OT_METRICS_TAG_HORIZONTAL_CARET_RUN:
  {
    अचिन्हित mult = 1u;

    अगर (font->slant)
    {
      अचिन्हित rise = HB_DUAL_GET (*face->table.hhea, table->caretSlopeRise);
      अचिन्हित upem = face->get_upem ();
      mult = (rise && rise < upem) ? hb_min (upem / rise, 256u) : 1u;
    }

    अगर (metrics_tag == HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE)
    {
      bool ret = GET_HEA_METRIC_Y (hhea, caretSlopeRise);

      अगर (position)
	*position = hb_clamp_to<hb_position_t> ((int64_t) *position * mult);

      वापस ret;
    }
    अन्यथा
    {
      hb_position_t rise = 0;

      अगर (font->slant && position && GET_HEA_METRIC_Y (hhea, caretSlopeRise))
	rise = *position;

      bool ret = GET_HEA_METRIC_X (hhea, caretSlopeRun);

      अगर (position)
      {
	*position = hb_clamp_to<hb_position_t> ((int64_t) *position * mult);

	अगर (font->slant)
	  *position = hb_saturate_add (*position,
				       hb_clamp_to<hb_position_t> (roundf (mult * font->slant_xy * rise)));
      }

      वापस ret;
    }
  }
  हाल HB_OT_METRICS_TAG_HORIZONTAL_CARET_OFFSET:     वापस GET_HEA_METRIC_X (hhea, caretOffset);

#अगर_अघोषित HB_NO_VERTICAL
  हाल HB_OT_METRICS_TAG_VERTICAL_CARET_RISE:         वापस GET_HEA_METRIC_X (vhea, caretSlopeRise);
  हाल HB_OT_METRICS_TAG_VERTICAL_CARET_RUN:          वापस GET_HEA_METRIC_Y (vhea, caretSlopeRun);
  हाल HB_OT_METRICS_TAG_VERTICAL_CARET_OFFSET:       वापस GET_HEA_METRIC_Y (vhea, caretOffset);
#पूर्ण_अगर
  हाल HB_OT_METRICS_TAG_X_HEIGHT:                    वापस GET_METRIC_Y (OS2->v2 (), sxHeight);
  हाल HB_OT_METRICS_TAG_CAP_HEIGHT:                  वापस GET_METRIC_Y (OS2->v2 (), sCapHeight);
  हाल HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_आकार:         वापस GET_METRIC_X (OS2, ySubscriptXSize);
  हाल HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_आकार:         वापस GET_METRIC_Y (OS2, ySubscriptYSize);
  हाल HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_OFFSET:       वापस GET_METRIC_X (OS2, ySubscriptXOffset);
  हाल HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_OFFSET:       वापस GET_METRIC_Y (OS2, ySubscriptYOffset);
  हाल HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_आकार:       वापस GET_METRIC_X (OS2, ySuperscriptXSize);
  हाल HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_आकार:       वापस GET_METRIC_Y (OS2, ySuperscriptYSize);
  हाल HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_OFFSET:     वापस GET_METRIC_X (OS2, ySuperscriptXOffset);
  हाल HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_OFFSET:     वापस GET_METRIC_Y (OS2, ySuperscriptYOffset);
  हाल HB_OT_METRICS_TAG_STRIKEOUT_आकार:              वापस GET_METRIC_Y (OS2, y_Strikeout_आकार);
  हाल HB_OT_METRICS_TAG_STRIKEOUT_OFFSET:            वापस GET_METRIC_Y (OS2, yStrikeoutPosition);
  हाल HB_OT_METRICS_TAG_UNDERLINE_आकार:              वापस GET_METRIC_Y (post->table, underlineThickness);
  हाल HB_OT_METRICS_TAG_UNDERLINE_OFFSET:            वापस GET_METRIC_Y (post->table, underlinePosition);

  /* Private tags */
  हाल _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_OS2:    वापस GET_METRIC_Y (OS2, sTypoAscender);
  हाल _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_HHEA:   वापस GET_HEA_METRIC_Y (hhea, ascender);
  हाल _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_OS2:   वापस GET_METRIC_Y (OS2, sTypoDescender);
  हाल _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_HHEA:  वापस GET_HEA_METRIC_Y (hhea, descender);
  हाल HB_OT_METRICS_TAG_HORIZONTAL_पंक्ति_GAP_OS2:    वापस GET_METRIC_Y (OS2, s_Typo_पंक्ति_Gap);
  हाल HB_OT_METRICS_TAG_HORIZONTAL_पंक्ति_GAP_HHEA:   वापस GET_HEA_METRIC_Y (hhea, पंक्ति_Gap);
#अघोषित GET_HEA_METRIC_Y
#अघोषित GET_HEA_METRIC_X
#अघोषित GET_METRIC_Y
#अघोषित GET_METRIC_X
#अघोषित GET_VAR
  शेष:                                        वापस false;
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
व्योम
hb_ot_metrics_get_position_with_fallback (hb_font_t           *font,
					  hb_ot_metrics_tag_t  metrics_tag,
					  hb_position_t       *position     /* OUT */)
{
  hb_font_extents_t font_extents;
  hb_codepoint_t glyph;
  hb_glyph_extents_t extents;

  अगर (hb_ot_metrics_get_position (font, metrics_tag, position))
    {
      अगर ((metrics_tag != HB_OT_METRICS_TAG_STRIKEOUT_आकार &&
           metrics_tag != HB_OT_METRICS_TAG_UNDERLINE_आकार) ||
          *position != 0)
        वापस;
    }

  चयन (metrics_tag)
  {
  हाल HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER:
  हाल HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_ASCENT:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.ascender;
    अवरोध;

  हाल HB_OT_METRICS_TAG_VERTICAL_ASCENDER:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ascender;
    अवरोध;

  हाल HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER:
  हाल HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_DESCENT:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.descender;
    अवरोध;

  हाल HB_OT_METRICS_TAG_VERTICAL_DESCENDER:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ascender;
    अवरोध;

  हाल HB_OT_METRICS_TAG_HORIZONTAL_पंक्ति_GAP:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.पंक्ति_gap;
    अवरोध;

  हाल HB_OT_METRICS_TAG_VERTICAL_पंक्ति_GAP:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.पंक्ति_gap;
    अवरोध;

  हाल HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE:
  हाल HB_OT_METRICS_TAG_VERTICAL_CARET_RISE:
    *position = 1;
    अवरोध;

  हाल HB_OT_METRICS_TAG_HORIZONTAL_CARET_RUN:
  हाल HB_OT_METRICS_TAG_VERTICAL_CARET_RUN:
    *position = 0;
    अवरोध;

  हाल HB_OT_METRICS_TAG_HORIZONTAL_CARET_OFFSET:
  हाल HB_OT_METRICS_TAG_VERTICAL_CARET_OFFSET:
    *position = 0;
    अवरोध;

  हाल HB_OT_METRICS_TAG_X_HEIGHT:
    अगर (hb_font_get_nominal_glyph (font, 'x', &glyph) &&
        hb_font_get_glyph_extents (font, glyph, &extents))
      *position = extents.y_bearing;
    अन्यथा
      *position = font->y_scale / 2;
    अवरोध;

  हाल HB_OT_METRICS_TAG_CAP_HEIGHT:
    अगर (hb_font_get_nominal_glyph (font, 'O', &glyph) &&
        hb_font_get_glyph_extents (font, glyph, &extents))
      *position = hb_clamp_to<hb_position_t> ((int64_t) extents.height +
					      2 * (int64_t) extents.y_bearing);
    अन्यथा
      *position = hb_clamp_to<hb_position_t> ((int64_t) font->y_scale * 2 / 3);
    अवरोध;

  हाल HB_OT_METRICS_TAG_STRIKEOUT_आकार:
  हाल HB_OT_METRICS_TAG_UNDERLINE_आकार:
    *position = font->y_scale / 18;
    अवरोध;

  हाल HB_OT_METRICS_TAG_STRIKEOUT_OFFSET:
    {
      hb_position_t ascender;
      hb_ot_metrics_get_position_with_fallback (font,
                                                HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER,
                                                &ascender);
      *position = ascender / 2;
    }
    अवरोध;

  हाल HB_OT_METRICS_TAG_UNDERLINE_OFFSET:
    *position = hb_saturate_neg (font->y_scale) / 18;
    अवरोध;

  हाल HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_आकार:
  हाल HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_आकार:
    *position = hb_clamp_to<hb_position_t> ((int64_t) font->x_scale * 10 / 12);
    अवरोध;

  हाल HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_आकार:
  हाल HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_आकार:
    *position = hb_clamp_to<hb_position_t> ((int64_t) font->y_scale * 10 / 12);
    अवरोध;

  हाल HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_OFFSET:
  हाल HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_OFFSET:
    *position = 0;
    अवरोध;

  हाल HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_OFFSET:
  हाल HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_OFFSET:
    *position = font->y_scale / 5;
    अवरोध;

  हाल HB_OT_METRICS_TAG_MAX_मूल्य:
  शेष:
    *position = 0;
    अवरोध;
  }
}

#अगर_अघोषित HB_NO_VAR
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
भग्न
hb_ot_metrics_get_variation (hb_font_t *font, hb_ot_metrics_tag_t metrics_tag)
{
  वापस font->face->table.MVAR->get_var (metrics_tag, font->coords, font->अंक_coords);
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
hb_ot_metrics_get_x_variation (hb_font_t *font, hb_ot_metrics_tag_t metrics_tag)
{
  वापस font->em_scalef_x (hb_ot_metrics_get_variation (font, metrics_tag));
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
hb_ot_metrics_get_y_variation (hb_font_t *font, hb_ot_metrics_tag_t metrics_tag)
{
  वापस font->em_scalef_y (hb_ot_metrics_get_variation (font, metrics_tag));
}
#पूर्ण_अगर

#पूर्ण_अगर

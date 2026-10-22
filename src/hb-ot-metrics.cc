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

#अन्तर्भाव "hb.hh"

#अन्तर्भाव "hb-ot-var-mvar-table.hh"
#अन्तर्भाव "hb-ot-gasp-table.hh" // Just so we compile it; unused otherwise.
#अन्तर्भाव "hb-ot-os2-table.hh"
#अन्तर्भाव "hb-ot-post-table.hh"
#अन्तर्भाव "hb-ot-hhea-table.hh"
#अन्तर्भाव "hb-ot-metrics.hh"
#अन्तर्भाव "hb-ot-face.hh"


/**
 * SECTION:hb-ot-metrics
 * @title: hb-ot-metrics
 * @short_description: OpenType Metrics
 * @include: hb-ot.h
 *
 * Functions for fetching metrics from fonts.
 **/

स्थिर भिन्न
_fix_ascender_descender (भिन्न value, hb_ot_metrics_tag_t metrics_tag)
{
  यदि (metrics_tag == HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER ||
      metrics_tag == HB_OT_METRICS_TAG_VERTICAL_ASCENDER)
    फर्काऊ भ_असल ((द्विगुण) value);
  यदि (metrics_tag == HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER ||
      metrics_tag == HB_OT_METRICS_TAG_VERTICAL_DESCENDER)
    फर्काऊ -भ_असल ((द्विगुण) value);
  फर्काऊ value;
}

/* The common part of _get_position logic needed on hb-ot-font and here
   to be able to have slim builds without the not always needed parts */
तार्किक
_hb_ot_metrics_get_position_common (hb_font_t           *font,
				    hb_ot_metrics_tag_t  metrics_tag,
				    hb_position_t       *position     /* OUT.  May be NULL. */)
{
  hb_face_t *face = font->face;
  विकल्प ((अचिन्हित) metrics_tag)
  {
#ifndef HB_NO_VAR
#define GET_VAR face->table.MVAR->get_var (metrics_tag, font->coords, font->अङ्क_coords)
#अन्यथा
#define GET_VAR .0f
#endif
#define GET_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((शून्य) (position && (*position = font->em_scalef_y (_fix_ascender_descender ( \
      face->table.TABLE->ATTR + GET_VAR, metrics_tag)))), सत्य))
#define GET_HEA_METRIC_X(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((शून्य) (position && (*position = font->em_scalef_x (_fix_ascender_descender ( \
      HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR, metrics_tag)))), सत्य))
#define GET_HEA_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((शून्य) (position && (*position = font->em_scalef_y (_fix_ascender_descender ( \
      HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR, metrics_tag)))), सत्य))

  पक्ष HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER:
    फर्काऊ (face->table.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, sTypoAscender)) ||
	   GET_HEA_METRIC_Y (hhea, ascender);
  पक्ष HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER:
    फर्काऊ (face->table.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, sTypoDescender)) ||
	   GET_HEA_METRIC_Y (hhea, descender);
  पक्ष HB_OT_METRICS_TAG_HORIZONTAL_पङ्क्ति_GAP:
    फर्काऊ (face->table.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, s_Typo_पङ्क्ति_Gap)) ||
	   GET_HEA_METRIC_Y (hhea, पङ्क्ति_Gap);

#ifndef HB_NO_VERTICAL
  पक्ष HB_OT_METRICS_TAG_VERTICAL_ASCENDER:  फर्काऊ GET_HEA_METRIC_X (vhea, ascender);
  पक्ष HB_OT_METRICS_TAG_VERTICAL_DESCENDER: फर्काऊ GET_HEA_METRIC_X (vhea, descender);
  पक्ष HB_OT_METRICS_TAG_VERTICAL_पङ्क्ति_GAP:  फर्काऊ GET_HEA_METRIC_X (vhea, पङ्क्ति_Gap);
#endif

#undef GET_HEA_METRIC_Y
#undef GET_HEA_METRIC_X
#undef GET_METRIC_Y
#undef GET_VAR
  स्वभाव:                               प्रतिज्ञा (0); फर्काऊ असत्य;
  }
}

#ifndef HB_NO_METRICS

#यदि 0
स्थिर तार्किक
_get_gasp (hb_face_t *face, भिन्न *result, hb_ot_metrics_tag_t metrics_tag)
{
  अचर OT::GaspRange& range = face->table.gasp->get_gasp_range (metrics_tag - HB_TAG ('g','s','p','0'));
  यदि (&range == &Null (OT::GaspRange)) फर्काऊ असत्य;
  यदि (result) *result = range.rangeMaxPPEM + font->face->table.MVAR->get_var (metrics_tag, font->coords, font->अङ्क_coords);
  फर्काऊ सत्य;
}
#endif

/* Private tags for https://github.com/harfbuzz/harfbuzz/issues/1866 */
#define _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_OS2   HB_TAG ('O','a','s','c')
#define _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_HHEA  HB_TAG ('H','a','s','c')
#define _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_OS2  HB_TAG ('O','d','s','c')
#define _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_HHEA HB_TAG ('H','d','s','c')
#define HB_OT_METRICS_TAG_HORIZONTAL_पङ्क्ति_GAP_OS2   HB_TAG ('O','l','g','p')
#define HB_OT_METRICS_TAG_HORIZONTAL_पङ्क्ति_GAP_HHEA  HB_TAG ('H','l','g','p')

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
  विकल्प ((अचिन्हित) metrics_tag)
  {
  पक्ष HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER:
  पक्ष HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER:
  पक्ष HB_OT_METRICS_TAG_HORIZONTAL_पङ्क्ति_GAP:
  पक्ष HB_OT_METRICS_TAG_VERTICAL_ASCENDER:
  पक्ष HB_OT_METRICS_TAG_VERTICAL_DESCENDER:
  पक्ष HB_OT_METRICS_TAG_VERTICAL_पङ्क्ति_GAP:           फर्काऊ _hb_ot_metrics_get_position_common (font, metrics_tag, position);
#ifndef HB_NO_VAR
#define GET_VAR hb_ot_metrics_get_variation (font, metrics_tag)
#अन्यथा
#define GET_VAR 0
#endif
#define GET_METRIC_X(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((शून्य) (position && (*position = font->em_scalef_x (face->table.TABLE->ATTR + GET_VAR))), सत्य))
#define GET_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((शून्य) (position && (*position = font->em_scalef_y (face->table.TABLE->ATTR + GET_VAR))), सत्य))
#define GET_HEA_METRIC_X(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((शून्य) (position && (*position = font->em_scalef_x (HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR))), सत्य))
#define GET_HEA_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((शून्य) (position && (*position = font->em_scalef_y (HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR))), सत्य))
  पक्ष HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_ASCENT:  फर्काऊ GET_METRIC_Y (OS2, usWinAscent);
  पक्ष HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_DESCENT: फर्काऊ GET_METRIC_Y (OS2, usWinDescent);

  पक्ष HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE:
  पक्ष HB_OT_METRICS_TAG_HORIZONTAL_CARET_RUN:
  {
    अचिन्हित mult = 1u;

    यदि (font->slant)
    {
      अचिन्हित rise = HB_DUAL_GET (*face->table.hhea, table->caretSlopeRise);
      अचिन्हित upem = face->get_upem ();
      mult = (rise && rise < upem) ? hb_min (upem / rise, 256u) : 1u;
    }

    यदि (metrics_tag == HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE)
    {
      तार्किक ret = GET_HEA_METRIC_Y (hhea, caretSlopeRise);

      यदि (position)
	*position = hb_clamp_to<hb_position_t> ((int64_t) *position * mult);

      फर्काऊ ret;
    }
    अन्यथा
    {
      hb_position_t rise = 0;

      यदि (font->slant && position && GET_HEA_METRIC_Y (hhea, caretSlopeRise))
	rise = *position;

      तार्किक ret = GET_HEA_METRIC_X (hhea, caretSlopeRun);

      यदि (position)
      {
	*position = hb_clamp_to<hb_position_t> ((int64_t) *position * mult);

	यदि (font->slant)
	  *position = hb_saturate_add (*position,
				       hb_clamp_to<hb_position_t> (roundf (mult * font->slant_xy * rise)));
      }

      फर्काऊ ret;
    }
  }
  पक्ष HB_OT_METRICS_TAG_HORIZONTAL_CARET_OFFSET:     फर्काऊ GET_HEA_METRIC_X (hhea, caretOffset);

#ifndef HB_NO_VERTICAL
  पक्ष HB_OT_METRICS_TAG_VERTICAL_CARET_RISE:         फर्काऊ GET_HEA_METRIC_X (vhea, caretSlopeRise);
  पक्ष HB_OT_METRICS_TAG_VERTICAL_CARET_RUN:          फर्काऊ GET_HEA_METRIC_Y (vhea, caretSlopeRun);
  पक्ष HB_OT_METRICS_TAG_VERTICAL_CARET_OFFSET:       फर्काऊ GET_HEA_METRIC_Y (vhea, caretOffset);
#endif
  पक्ष HB_OT_METRICS_TAG_X_HEIGHT:                    फर्काऊ GET_METRIC_Y (OS2->v2 (), sxHeight);
  पक्ष HB_OT_METRICS_TAG_CAP_HEIGHT:                  फर्काऊ GET_METRIC_Y (OS2->v2 (), sCapHeight);
  पक्ष HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_SIZE:         फर्काऊ GET_METRIC_X (OS2, ySubscriptXSize);
  पक्ष HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_SIZE:         फर्काऊ GET_METRIC_Y (OS2, ySubscriptYSize);
  पक्ष HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_OFFSET:       फर्काऊ GET_METRIC_X (OS2, ySubscriptXOffset);
  पक्ष HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_OFFSET:       फर्काऊ GET_METRIC_Y (OS2, ySubscriptYOffset);
  पक्ष HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_SIZE:       फर्काऊ GET_METRIC_X (OS2, ySuperscriptXSize);
  पक्ष HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_SIZE:       फर्काऊ GET_METRIC_Y (OS2, ySuperscriptYSize);
  पक्ष HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_OFFSET:     फर्काऊ GET_METRIC_X (OS2, ySuperscriptXOffset);
  पक्ष HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_OFFSET:     फर्काऊ GET_METRIC_Y (OS2, ySuperscriptYOffset);
  पक्ष HB_OT_METRICS_TAG_STRIKEOUT_SIZE:              फर्काऊ GET_METRIC_Y (OS2, yStrikeoutSize);
  पक्ष HB_OT_METRICS_TAG_STRIKEOUT_OFFSET:            फर्काऊ GET_METRIC_Y (OS2, yStrikeoutPosition);
  पक्ष HB_OT_METRICS_TAG_UNDERLINE_SIZE:              फर्काऊ GET_METRIC_Y (post->table, underlineThickness);
  पक्ष HB_OT_METRICS_TAG_UNDERLINE_OFFSET:            फर्काऊ GET_METRIC_Y (post->table, underlinePosition);

  /* Private tags */
  पक्ष _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_OS2:    फर्काऊ GET_METRIC_Y (OS2, sTypoAscender);
  पक्ष _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_HHEA:   फर्काऊ GET_HEA_METRIC_Y (hhea, ascender);
  पक्ष _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_OS2:   फर्काऊ GET_METRIC_Y (OS2, sTypoDescender);
  पक्ष _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_HHEA:  फर्काऊ GET_HEA_METRIC_Y (hhea, descender);
  पक्ष HB_OT_METRICS_TAG_HORIZONTAL_पङ्क्ति_GAP_OS2:    फर्काऊ GET_METRIC_Y (OS2, s_Typo_पङ्क्ति_Gap);
  पक्ष HB_OT_METRICS_TAG_HORIZONTAL_पङ्क्ति_GAP_HHEA:   फर्काऊ GET_HEA_METRIC_Y (hhea, पङ्क्ति_Gap);
#undef GET_HEA_METRIC_Y
#undef GET_HEA_METRIC_X
#undef GET_METRIC_Y
#undef GET_METRIC_X
#undef GET_VAR
  स्वभाव:                                        फर्काऊ असत्य;
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
शून्य
hb_ot_metrics_get_position_with_fallback (hb_font_t           *font,
					  hb_ot_metrics_tag_t  metrics_tag,
					  hb_position_t       *position     /* OUT */)
{
  hb_font_extents_t font_extents;
  hb_codepoint_t glyph;
  hb_glyph_extents_t extents;

  यदि (hb_ot_metrics_get_position (font, metrics_tag, position))
    {
      यदि ((metrics_tag != HB_OT_METRICS_TAG_STRIKEOUT_SIZE &&
           metrics_tag != HB_OT_METRICS_TAG_UNDERLINE_SIZE) ||
          *position != 0)
        फर्काऊ;
    }

  विकल्प (metrics_tag)
  {
  पक्ष HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER:
  पक्ष HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_ASCENT:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.ascender;
    रोक;

  पक्ष HB_OT_METRICS_TAG_VERTICAL_ASCENDER:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ascender;
    रोक;

  पक्ष HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER:
  पक्ष HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_DESCENT:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.descender;
    रोक;

  पक्ष HB_OT_METRICS_TAG_VERTICAL_DESCENDER:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ascender;
    रोक;

  पक्ष HB_OT_METRICS_TAG_HORIZONTAL_पङ्क्ति_GAP:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.पङ्क्ति_gap;
    रोक;

  पक्ष HB_OT_METRICS_TAG_VERTICAL_पङ्क्ति_GAP:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.पङ्क्ति_gap;
    रोक;

  पक्ष HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE:
  पक्ष HB_OT_METRICS_TAG_VERTICAL_CARET_RISE:
    *position = 1;
    रोक;

  पक्ष HB_OT_METRICS_TAG_HORIZONTAL_CARET_RUN:
  पक्ष HB_OT_METRICS_TAG_VERTICAL_CARET_RUN:
    *position = 0;
    रोक;

  पक्ष HB_OT_METRICS_TAG_HORIZONTAL_CARET_OFFSET:
  पक्ष HB_OT_METRICS_TAG_VERTICAL_CARET_OFFSET:
    *position = 0;
    रोक;

  पक्ष HB_OT_METRICS_TAG_X_HEIGHT:
    यदि (hb_font_get_nominal_glyph (font, 'x', &glyph) &&
        hb_font_get_glyph_extents (font, glyph, &extents))
      *position = extents.y_bearing;
    अन्यथा
      *position = font->y_scale / 2;
    रोक;

  पक्ष HB_OT_METRICS_TAG_CAP_HEIGHT:
    यदि (hb_font_get_nominal_glyph (font, 'O', &glyph) &&
        hb_font_get_glyph_extents (font, glyph, &extents))
      *position = hb_clamp_to<hb_position_t> ((int64_t) extents.height +
					      2 * (int64_t) extents.y_bearing);
    अन्यथा
      *position = hb_clamp_to<hb_position_t> ((int64_t) font->y_scale * 2 / 3);
    रोक;

  पक्ष HB_OT_METRICS_TAG_STRIKEOUT_SIZE:
  पक्ष HB_OT_METRICS_TAG_UNDERLINE_SIZE:
    *position = font->y_scale / 18;
    रोक;

  पक्ष HB_OT_METRICS_TAG_STRIKEOUT_OFFSET:
    {
      hb_position_t ascender;
      hb_ot_metrics_get_position_with_fallback (font,
                                                HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER,
                                                &ascender);
      *position = ascender / 2;
    }
    रोक;

  पक्ष HB_OT_METRICS_TAG_UNDERLINE_OFFSET:
    *position = hb_saturate_neg (font->y_scale) / 18;
    रोक;

  पक्ष HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_SIZE:
  पक्ष HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_SIZE:
    *position = hb_clamp_to<hb_position_t> ((int64_t) font->x_scale * 10 / 12);
    रोक;

  पक्ष HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_SIZE:
  पक्ष HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_SIZE:
    *position = hb_clamp_to<hb_position_t> ((int64_t) font->y_scale * 10 / 12);
    रोक;

  पक्ष HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_OFFSET:
  पक्ष HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_OFFSET:
    *position = 0;
    रोक;

  पक्ष HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_OFFSET:
  पक्ष HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_OFFSET:
    *position = font->y_scale / 5;
    रोक;

  पक्ष HB_OT_METRICS_TAG_MAX_मान:
  स्वभाव:
    *position = 0;
    रोक;
  }
}

#ifndef HB_NO_VAR
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
भिन्न
hb_ot_metrics_get_variation (hb_font_t *font, hb_ot_metrics_tag_t metrics_tag)
{
  फर्काऊ font->face->table.MVAR->get_var (metrics_tag, font->coords, font->अङ्क_coords);
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
  फर्काऊ font->em_scalef_x (hb_ot_metrics_get_variation (font, metrics_tag));
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
  फर्काऊ font->em_scalef_y (hb_ot_metrics_get_variation (font, metrics_tag));
}
#endif

#endif

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

#شامل "hb.hh"

#شامل "hb-ot-var-mvar-table.hh"
#شامل "hb-ot-gasp-table.hh" // Just so we compile it; unused otherwise.
#شامل "hb-ot-os2-table.hh"
#شامل "hb-ot-post-table.hh"
#شامل "hb-ot-hhea-table.hh"
#شامل "hb-ot-metrics.hh"
#شامل "hb-ot-face.hh"


/**
 * SECTION:hb-ot-metrics
 * @title: hb-ot-metrics
 * @short_description: OpenType Metrics
 * @include: hb-ot.h
 *
 * Functions for fetching metrics from fonts.
 **/

ایستا اعشاری
_fix_ascender_descender (اعشاری value, hb_ot_metrics_tag_t metrics_tag)
{
  اگر (metrics_tag == HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER ||
      metrics_tag == HB_OT_METRICS_TAG_VERTICAL_ASCENDER)
    بازگشت fabs ((دوگانه) value);
  اگر (metrics_tag == HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER ||
      metrics_tag == HB_OT_METRICS_TAG_VERTICAL_DESCENDER)
    بازگشت -fabs ((دوگانه) value);
  بازگشت value;
}

/* The common part of _get_position logic needed on hb-ot-font and here
   to be able to have slim builds without the not always needed parts */
منطقی
_hb_ot_metrics_get_position_common (hb_font_t           *font,
				    hb_ot_metrics_tag_t  metrics_tag,
				    hb_position_t       *position     /* OUT.  May be NULL. */)
{
  hb_face_t *face = font->face;
  انتخاب ((unsigned) metrics_tag)
  {
#ifndef HB_NO_VAR
#define GET_VAR face->table.MVAR->get_var (metrics_tag, font->coords, font->رقم_coords)
#وگرنه
#define GET_VAR .0f
#endif
#define GET_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((تهی) (position && (*position = font->em_scalef_y (_fix_ascender_descender ( \
      face->table.TABLE->ATTR + GET_VAR, metrics_tag)))), درست))
#define GET_HEA_METRIC_X(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((تهی) (position && (*position = font->em_scalef_x (_fix_ascender_descender ( \
      HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR, metrics_tag)))), درست))
#define GET_HEA_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((تهی) (position && (*position = font->em_scalef_y (_fix_ascender_descender ( \
      HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR, metrics_tag)))), درست))

  حالت HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER:
    بازگشت (face->table.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, sTypoAscender)) ||
	   GET_HEA_METRIC_Y (hhea, ascender);
  حالت HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER:
    بازگشت (face->table.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, sTypoDescender)) ||
	   GET_HEA_METRIC_Y (hhea, descender);
  حالت HB_OT_METRICS_TAG_HORIZONTAL_سطر_GAP:
    بازگشت (face->table.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, s_Typo_سطر_Gap)) ||
	   GET_HEA_METRIC_Y (hhea, سطر_Gap);

#ifndef HB_NO_VERTICAL
  حالت HB_OT_METRICS_TAG_VERTICAL_ASCENDER:  بازگشت GET_HEA_METRIC_X (vhea, ascender);
  حالت HB_OT_METRICS_TAG_VERTICAL_DESCENDER: بازگشت GET_HEA_METRIC_X (vhea, descender);
  حالت HB_OT_METRICS_TAG_VERTICAL_سطر_GAP:  بازگشت GET_HEA_METRIC_X (vhea, سطر_Gap);
#endif

#undef GET_HEA_METRIC_Y
#undef GET_HEA_METRIC_X
#undef GET_METRIC_Y
#undef GET_VAR
  پیشفرض:                               أكد (0); بازگشت نادرست;
  }
}

#ifndef HB_NO_METRICS

#اگر 0
ایستا منطقی
_get_gasp (hb_face_t *face, اعشاری *result, hb_ot_metrics_tag_t metrics_tag)
{
  ثابت OT::GaspRange& range = face->table.gasp->get_gasp_range (metrics_tag - HB_TAG ('g','s','p','0'));
  اگر (&range == &Null (OT::GaspRange)) بازگشت نادرست;
  اگر (result) *result = range.rangeMaxPPEM + font->face->table.MVAR->get_var (metrics_tag, font->coords, font->رقم_coords);
  بازگشت درست;
}
#endif

/* Private tags for https://github.com/harfbuzz/harfbuzz/issues/1866 */
#define _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_OS2   HB_TAG ('O','a','s','c')
#define _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_HHEA  HB_TAG ('H','a','s','c')
#define _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_OS2  HB_TAG ('O','d','s','c')
#define _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_HHEA HB_TAG ('H','d','s','c')
#define HB_OT_METRICS_TAG_HORIZONTAL_سطر_GAP_OS2   HB_TAG ('O','l','g','p')
#define HB_OT_METRICS_TAG_HORIZONTAL_سطر_GAP_HHEA  HB_TAG ('H','l','g','p')

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
  انتخاب ((unsigned) metrics_tag)
  {
  حالت HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER:
  حالت HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER:
  حالت HB_OT_METRICS_TAG_HORIZONTAL_سطر_GAP:
  حالت HB_OT_METRICS_TAG_VERTICAL_ASCENDER:
  حالت HB_OT_METRICS_TAG_VERTICAL_DESCENDER:
  حالت HB_OT_METRICS_TAG_VERTICAL_سطر_GAP:           بازگشت _hb_ot_metrics_get_position_common (font, metrics_tag, position);
#ifndef HB_NO_VAR
#define GET_VAR hb_ot_metrics_get_variation (font, metrics_tag)
#وگرنه
#define GET_VAR 0
#endif
#define GET_METRIC_X(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((تهی) (position && (*position = font->em_scalef_x (face->table.TABLE->ATTR + GET_VAR))), درست))
#define GET_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((تهی) (position && (*position = font->em_scalef_y (face->table.TABLE->ATTR + GET_VAR))), درست))
#define GET_HEA_METRIC_X(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((تهی) (position && (*position = font->em_scalef_x (HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR))), درست))
#define GET_HEA_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((تهی) (position && (*position = font->em_scalef_y (HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR))), درست))
  حالت HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_ASCENT:  بازگشت GET_METRIC_Y (OS2, usWinAscent);
  حالت HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_DESCENT: بازگشت GET_METRIC_Y (OS2, usWinDescent);

  حالت HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE:
  حالت HB_OT_METRICS_TAG_HORIZONTAL_CARET_RUN:
  {
    unsigned mult = 1u;

    اگر (font->slant)
    {
      unsigned rise = HB_DUAL_GET (*face->table.hhea, table->caretSlopeRise);
      unsigned upem = face->get_upem ();
      mult = (rise && rise < upem) ? hb_min (upem / rise, 256u) : 1u;
    }

    اگر (metrics_tag == HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE)
    {
      منطقی ret = GET_HEA_METRIC_Y (hhea, caretSlopeRise);

      اگر (position)
	*position = hb_clamp_to<hb_position_t> ((int64_t) *position * mult);

      بازگشت ret;
    }
    وگرنه
    {
      hb_position_t rise = 0;

      اگر (font->slant && position && GET_HEA_METRIC_Y (hhea, caretSlopeRise))
	rise = *position;

      منطقی ret = GET_HEA_METRIC_X (hhea, caretSlopeRun);

      اگر (position)
      {
	*position = hb_clamp_to<hb_position_t> ((int64_t) *position * mult);

	اگر (font->slant)
	  *position = hb_saturate_add (*position,
				       hb_clamp_to<hb_position_t> (roundf (mult * font->slant_xy * rise)));
      }

      بازگشت ret;
    }
  }
  حالت HB_OT_METRICS_TAG_HORIZONTAL_CARET_OFFSET:     بازگشت GET_HEA_METRIC_X (hhea, caretOffset);

#ifndef HB_NO_VERTICAL
  حالت HB_OT_METRICS_TAG_VERTICAL_CARET_RISE:         بازگشت GET_HEA_METRIC_X (vhea, caretSlopeRise);
  حالت HB_OT_METRICS_TAG_VERTICAL_CARET_RUN:          بازگشت GET_HEA_METRIC_Y (vhea, caretSlopeRun);
  حالت HB_OT_METRICS_TAG_VERTICAL_CARET_OFFSET:       بازگشت GET_HEA_METRIC_Y (vhea, caretOffset);
#endif
  حالت HB_OT_METRICS_TAG_X_HEIGHT:                    بازگشت GET_METRIC_Y (OS2->v2 (), sxHeight);
  حالت HB_OT_METRICS_TAG_CAP_HEIGHT:                  بازگشت GET_METRIC_Y (OS2->v2 (), sCapHeight);
  حالت HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_SIZE:         بازگشت GET_METRIC_X (OS2, ySubscriptXSize);
  حالت HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_SIZE:         بازگشت GET_METRIC_Y (OS2, ySubscriptYSize);
  حالت HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_OFFSET:       بازگشت GET_METRIC_X (OS2, ySubscriptXOffset);
  حالت HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_OFFSET:       بازگشت GET_METRIC_Y (OS2, ySubscriptYOffset);
  حالت HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_SIZE:       بازگشت GET_METRIC_X (OS2, ySuperscriptXSize);
  حالت HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_SIZE:       بازگشت GET_METRIC_Y (OS2, ySuperscriptYSize);
  حالت HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_OFFSET:     بازگشت GET_METRIC_X (OS2, ySuperscriptXOffset);
  حالت HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_OFFSET:     بازگشت GET_METRIC_Y (OS2, ySuperscriptYOffset);
  حالت HB_OT_METRICS_TAG_STRIKEOUT_SIZE:              بازگشت GET_METRIC_Y (OS2, yStrikeoutSize);
  حالت HB_OT_METRICS_TAG_STRIKEOUT_OFFSET:            بازگشت GET_METRIC_Y (OS2, yStrikeoutPosition);
  حالت HB_OT_METRICS_TAG_UNDERLINE_SIZE:              بازگشت GET_METRIC_Y (post->table, underlineThickness);
  حالت HB_OT_METRICS_TAG_UNDERLINE_OFFSET:            بازگشت GET_METRIC_Y (post->table, underlinePosition);

  /* Private tags */
  حالت _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_OS2:    بازگشت GET_METRIC_Y (OS2, sTypoAscender);
  حالت _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_HHEA:   بازگشت GET_HEA_METRIC_Y (hhea, ascender);
  حالت _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_OS2:   بازگشت GET_METRIC_Y (OS2, sTypoDescender);
  حالت _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_HHEA:  بازگشت GET_HEA_METRIC_Y (hhea, descender);
  حالت HB_OT_METRICS_TAG_HORIZONTAL_سطر_GAP_OS2:    بازگشت GET_METRIC_Y (OS2, s_Typo_سطر_Gap);
  حالت HB_OT_METRICS_TAG_HORIZONTAL_سطر_GAP_HHEA:   بازگشت GET_HEA_METRIC_Y (hhea, سطر_Gap);
#undef GET_HEA_METRIC_Y
#undef GET_HEA_METRIC_X
#undef GET_METRIC_Y
#undef GET_METRIC_X
#undef GET_VAR
  پیشفرض:                                        بازگشت نادرست;
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
تهی
hb_ot_metrics_get_position_with_fallback (hb_font_t           *font,
					  hb_ot_metrics_tag_t  metrics_tag,
					  hb_position_t       *position     /* OUT */)
{
  hb_font_extents_t font_extents;
  hb_codepoint_t glyph;
  hb_glyph_extents_t extents;

  اگر (hb_ot_metrics_get_position (font, metrics_tag, position))
    {
      اگر ((metrics_tag != HB_OT_METRICS_TAG_STRIKEOUT_SIZE &&
           metrics_tag != HB_OT_METRICS_TAG_UNDERLINE_SIZE) ||
          *position != 0)
        بازگشت;
    }

  انتخاب (metrics_tag)
  {
  حالت HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER:
  حالت HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_ASCENT:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.ascender;
    بشکن;

  حالت HB_OT_METRICS_TAG_VERTICAL_ASCENDER:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ascender;
    بشکن;

  حالت HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER:
  حالت HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_DESCENT:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.descender;
    بشکن;

  حالت HB_OT_METRICS_TAG_VERTICAL_DESCENDER:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ascender;
    بشکن;

  حالت HB_OT_METRICS_TAG_HORIZONTAL_سطر_GAP:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.سطر_gap;
    بشکن;

  حالت HB_OT_METRICS_TAG_VERTICAL_سطر_GAP:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.سطر_gap;
    بشکن;

  حالت HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE:
  حالت HB_OT_METRICS_TAG_VERTICAL_CARET_RISE:
    *position = 1;
    بشکن;

  حالت HB_OT_METRICS_TAG_HORIZONTAL_CARET_RUN:
  حالت HB_OT_METRICS_TAG_VERTICAL_CARET_RUN:
    *position = 0;
    بشکن;

  حالت HB_OT_METRICS_TAG_HORIZONTAL_CARET_OFFSET:
  حالت HB_OT_METRICS_TAG_VERTICAL_CARET_OFFSET:
    *position = 0;
    بشکن;

  حالت HB_OT_METRICS_TAG_X_HEIGHT:
    اگر (hb_font_get_nominal_glyph (font, 'x', &glyph) &&
        hb_font_get_glyph_extents (font, glyph, &extents))
      *position = extents.y_bearing;
    وگرنه
      *position = font->y_scale / 2;
    بشکن;

  حالت HB_OT_METRICS_TAG_CAP_HEIGHT:
    اگر (hb_font_get_nominal_glyph (font, 'O', &glyph) &&
        hb_font_get_glyph_extents (font, glyph, &extents))
      *position = hb_clamp_to<hb_position_t> ((int64_t) extents.height +
					      2 * (int64_t) extents.y_bearing);
    وگرنه
      *position = hb_clamp_to<hb_position_t> ((int64_t) font->y_scale * 2 / 3);
    بشکن;

  حالت HB_OT_METRICS_TAG_STRIKEOUT_SIZE:
  حالت HB_OT_METRICS_TAG_UNDERLINE_SIZE:
    *position = font->y_scale / 18;
    بشکن;

  حالت HB_OT_METRICS_TAG_STRIKEOUT_OFFSET:
    {
      hb_position_t ascender;
      hb_ot_metrics_get_position_with_fallback (font,
                                                HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER,
                                                &ascender);
      *position = ascender / 2;
    }
    بشکن;

  حالت HB_OT_METRICS_TAG_UNDERLINE_OFFSET:
    *position = hb_saturate_neg (font->y_scale) / 18;
    بشکن;

  حالت HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_SIZE:
  حالت HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_SIZE:
    *position = hb_clamp_to<hb_position_t> ((int64_t) font->x_scale * 10 / 12);
    بشکن;

  حالت HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_SIZE:
  حالت HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_SIZE:
    *position = hb_clamp_to<hb_position_t> ((int64_t) font->y_scale * 10 / 12);
    بشکن;

  حالت HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_OFFSET:
  حالت HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_OFFSET:
    *position = 0;
    بشکن;

  حالت HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_OFFSET:
  حالت HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_OFFSET:
    *position = font->y_scale / 5;
    بشکن;

  حالت HB_OT_METRICS_TAG_MAX_مقدار:
  پیشفرض:
    *position = 0;
    بشکن;
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
اعشاری
hb_ot_metrics_get_variation (hb_font_t *font, hb_ot_metrics_tag_t metrics_tag)
{
  بازگشت font->face->table.MVAR->get_var (metrics_tag, font->coords, font->رقم_coords);
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
  بازگشت font->em_scalef_x (hb_ot_metrics_get_variation (font, metrics_tag));
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
  بازگشت font->em_scalef_y (hb_ot_metrics_get_variation (font, metrics_tag));
}
#endif

#endif

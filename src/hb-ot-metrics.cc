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

ساکن اعشاری
_fix_ascender_descender (اعشاری value, hb_ot_metrics_لیبل_t metrics_لیبل)
{
  اگر (metrics_لیبل == HB_OT_METRICS_لیبل_HORIZONTAL_ASCENDER ||
      metrics_لیبل == HB_OT_METRICS_لیبل_VERTICAL_ASCENDER)
    واپس fabs ((دوہرا) value);
  اگر (metrics_لیبل == HB_OT_METRICS_لیبل_HORIZONTAL_DESCENDER ||
      metrics_لیبل == HB_OT_METRICS_لیبل_VERTICAL_DESCENDER)
    واپس -fabs ((دوہرا) value);
  واپس value;
}

/* The common part of _get_position logic needed on hb-ot-font and here
   to be able to have slim builds without the not always needed parts */
منطقی
_hb_ot_metrics_get_position_common (hb_font_t           *font,
				    hb_ot_metrics_لیبل_t  metrics_لیبل,
				    hb_position_t       *position     /* OUT.  May be NULL. */)
{
  hb_face_t *face = font->face;
  انتخاب ((غیرعلامتی) metrics_لیبل)
  {
#اگر_غیر_تعریف_شدہ HB_NO_VAR
#تعریف_کرو GET_VAR face->جدول.MVAR->get_var (metrics_لیبل, font->coords, font->عدد_coords)
#ورنہ
#تعریف_کرو GET_VAR .0f
#اگر_ختم
#تعریف_کرو GET_METRIC_Y(جدول_4, ATTR) \
  (face->جدول.جدول_4->has_ڈیٹا () && \
    ((خالی) (position && (*position = font->em_scalef_y (_fix_ascender_descender ( \
      face->جدول.جدول_4->ATTR + GET_VAR, metrics_لیبل)))), سچ))
#تعریف_کرو GET_HEA_METRIC_X(جدول_4, ATTR) \
  (face->جدول.جدول_4->has_ڈیٹا () && \
    ((خالی) (position && (*position = font->em_scalef_x (_fix_ascender_descender ( \
      HB_DUAL_GET (*face->جدول.جدول_4, جدول->ATTR) + GET_VAR, metrics_لیبل)))), سچ))
#تعریف_کرو GET_HEA_METRIC_Y(جدول_4, ATTR) \
  (face->جدول.جدول_4->has_ڈیٹا () && \
    ((خالی) (position && (*position = font->em_scalef_y (_fix_ascender_descender ( \
      HB_DUAL_GET (*face->جدول.جدول_4, جدول->ATTR) + GET_VAR, metrics_لیبل)))), سچ))

  حالت HB_OT_METRICS_لیبل_HORIZONTAL_ASCENDER:
    واپس (face->جدول.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, sTypoAscender)) ||
	   GET_HEA_METRIC_Y (hhea, ascender);
  حالت HB_OT_METRICS_لیبل_HORIZONTAL_DESCENDER:
    واپس (face->جدول.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, sTypoDescender)) ||
	   GET_HEA_METRIC_Y (hhea, descender);
  حالت HB_OT_METRICS_لیبل_HORIZONTAL_سطر_GAP:
    واپس (face->جدول.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, s_Typo_سطر_Gap)) ||
	   GET_HEA_METRIC_Y (hhea, سطر_Gap);

#اگر_غیر_تعریف_شدہ HB_NO_VERTICAL
  حالت HB_OT_METRICS_لیبل_VERTICAL_ASCENDER:  واپس GET_HEA_METRIC_X (vhea, ascender);
  حالت HB_OT_METRICS_لیبل_VERTICAL_DESCENDER: واپس GET_HEA_METRIC_X (vhea, descender);
  حالت HB_OT_METRICS_لیبل_VERTICAL_سطر_GAP:  واپس GET_HEA_METRIC_X (vhea, سطر_Gap);
#اگر_ختم

#تعریف_منسوخ GET_HEA_METRIC_Y
#تعریف_منسوخ GET_HEA_METRIC_X
#تعریف_منسوخ GET_METRIC_Y
#تعریف_منسوخ GET_VAR
  طےشدہ:                               أكد (0); واپس جھوٹ;
  }
}

#اگر_غیر_تعریف_شدہ HB_NO_METRICS

#اگر 0
ساکن منطقی
_get_gasp (hb_face_t *face, اعشاری *نتیجہ, hb_ot_metrics_لیبل_t metrics_لیبل)
{
  ثابت OT::GaspRange& range = face->جدول.gasp->get_gasp_range (metrics_لیبل - HB_لیبل ('g','s','p','0'));
  اگر (&range == &Null (OT::GaspRange)) واپس جھوٹ;
  اگر (نتیجہ) *نتیجہ = range.rangeMaxPPEM + font->face->جدول.MVAR->get_var (metrics_لیبل, font->coords, font->عدد_coords);
  واپس سچ;
}
#اگر_ختم

/* Private tags for https://github.com/harfbuzz/harfbuzz/issues/1866 */
#تعریف_کرو HB_OT_METRICS_لیبل_HORIZONTAL_ASCENDER_OS2   HB_لیبل ('O','a','s','c')
#تعریف_کرو HB_OT_METRICS_لیبل_HORIZONTAL_ASCENDER_HHEA  HB_لیبل ('H','a','s','c')
#تعریف_کرو HB_OT_METRICS_لیبل_HORIZONTAL_DESCENDER_OS2  HB_لیبل ('O','d','s','c')
#تعریف_کرو HB_OT_METRICS_لیبل_HORIZONTAL_DESCENDER_HHEA HB_لیبل ('H','d','s','c')
#تعریف_کرو HB_OT_METRICS_لیبل_HORIZONTAL_سطر_GAP_OS2   HB_لیبل ('O','l','g','p')
#تعریف_کرو HB_OT_METRICS_لیبل_HORIZONTAL_سطر_GAP_HHEA  HB_لیبل ('H','l','g','p')

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
			    hb_ot_metrics_لیبل_t  metrics_لیبل,
			    hb_position_t       *position     /* OUT.  May be NULL. */)
{
  hb_face_t *face = font->face;
  انتخاب ((غیرعلامتی) metrics_لیبل)
  {
  حالت HB_OT_METRICS_لیبل_HORIZONTAL_ASCENDER:
  حالت HB_OT_METRICS_لیبل_HORIZONTAL_DESCENDER:
  حالت HB_OT_METRICS_لیبل_HORIZONTAL_سطر_GAP:
  حالت HB_OT_METRICS_لیبل_VERTICAL_ASCENDER:
  حالت HB_OT_METRICS_لیبل_VERTICAL_DESCENDER:
  حالت HB_OT_METRICS_لیبل_VERTICAL_سطر_GAP:           واپس _hb_ot_metrics_get_position_common (font, metrics_لیبل, position);
#اگر_غیر_تعریف_شدہ HB_NO_VAR
#تعریف_کرو GET_VAR hb_ot_metrics_get_variation (font, metrics_لیبل)
#ورنہ
#تعریف_کرو GET_VAR 0
#اگر_ختم
#تعریف_کرو GET_METRIC_X(جدول_4, ATTR) \
  (face->جدول.جدول_4->has_ڈیٹا () && \
    ((خالی) (position && (*position = font->em_scalef_x (face->جدول.جدول_4->ATTR + GET_VAR))), سچ))
#تعریف_کرو GET_METRIC_Y(جدول_4, ATTR) \
  (face->جدول.جدول_4->has_ڈیٹا () && \
    ((خالی) (position && (*position = font->em_scalef_y (face->جدول.جدول_4->ATTR + GET_VAR))), سچ))
#تعریف_کرو GET_HEA_METRIC_X(جدول_4, ATTR) \
  (face->جدول.جدول_4->has_ڈیٹا () && \
    ((خالی) (position && (*position = font->em_scalef_x (HB_DUAL_GET (*face->جدول.جدول_4, جدول->ATTR) + GET_VAR))), سچ))
#تعریف_کرو GET_HEA_METRIC_Y(جدول_4, ATTR) \
  (face->جدول.جدول_4->has_ڈیٹا () && \
    ((خالی) (position && (*position = font->em_scalef_y (HB_DUAL_GET (*face->جدول.جدول_4, جدول->ATTR) + GET_VAR))), سچ))
  حالت HB_OT_METRICS_لیبل_HORIZONTAL_CLIPPING_ASCENT:  واپس GET_METRIC_Y (OS2, usWinAscent);
  حالت HB_OT_METRICS_لیبل_HORIZONTAL_CLIPPING_DESCENT: واپس GET_METRIC_Y (OS2, usWinDescent);

  حالت HB_OT_METRICS_لیبل_HORIZONTAL_CARET_RISE:
  حالت HB_OT_METRICS_لیبل_HORIZONTAL_CARET_تسلسل:
  {
    غیرعلامتی mult = 1u;

    اگر (font->slant)
    {
      غیرعلامتی rise = HB_DUAL_GET (*face->جدول.hhea, جدول->caret_ڈھلان_Rise);
      غیرعلامتی upem = face->get_upem ();
      mult = (rise && rise < upem) ? hb_min (upem / rise, 256u) : 1u;
    }

    اگر (metrics_لیبل == HB_OT_METRICS_لیبل_HORIZONTAL_CARET_RISE)
    {
      منطقی ret = GET_HEA_METRIC_Y (hhea, caret_ڈھلان_Rise);

      اگر (position)
	*position = hb_clamp_مقصد<hb_position_t> ((int64_t) *position * mult);

      واپس ret;
    }
    ورنہ
    {
      hb_position_t rise = 0;

      اگر (font->slant && position && GET_HEA_METRIC_Y (hhea, caret_ڈھلان_Rise))
	rise = *position;

      منطقی ret = GET_HEA_METRIC_X (hhea, caret_ڈھلان_تسلسل);

      اگر (position)
      {
	*position = hb_clamp_مقصد<hb_position_t> ((int64_t) *position * mult);

	اگر (font->slant)
	  *position = hb_saturate_جوڑو (*position,
				       hb_clamp_مقصد<hb_position_t> (roundf (mult * font->slant_xy * rise)));
      }

      واپس ret;
    }
  }
  حالت HB_OT_METRICS_لیبل_HORIZONTAL_CARET_انحراف:     واپس GET_HEA_METRIC_X (hhea, caret_انحراف);

#اگر_غیر_تعریف_شدہ HB_NO_VERTICAL
  حالت HB_OT_METRICS_لیبل_VERTICAL_CARET_RISE:         واپس GET_HEA_METRIC_X (vhea, caret_ڈھلان_Rise);
  حالت HB_OT_METRICS_لیبل_VERTICAL_CARET_تسلسل:          واپس GET_HEA_METRIC_Y (vhea, caret_ڈھلان_تسلسل);
  حالت HB_OT_METRICS_لیبل_VERTICAL_CARET_انحراف:       واپس GET_HEA_METRIC_Y (vhea, caret_انحراف);
#اگر_ختم
  حالت HB_OT_METRICS_لیبل_X_اونچائی:                    واپس GET_METRIC_Y (OS2->v2 (), sx_اونچائی);
  حالت HB_OT_METRICS_لیبل_CAP_اونچائی:                  واپس GET_METRIC_Y (OS2->v2 (), s_Cap_اونچائی);
  حالت HB_OT_METRICS_لیبل_SUBSCRIPT_EM_X_سائز:         واپس GET_METRIC_X (OS2, ySubscriptXSize);
  حالت HB_OT_METRICS_لیبل_SUBSCRIPT_EM_Y_سائز:         واپس GET_METRIC_Y (OS2, ySubscriptYSize);
  حالت HB_OT_METRICS_لیبل_SUBSCRIPT_EM_X_انحراف:       واپس GET_METRIC_X (OS2, ySubscriptXOffset);
  حالت HB_OT_METRICS_لیبل_SUBSCRIPT_EM_Y_انحراف:       واپس GET_METRIC_Y (OS2, ySubscriptYOffset);
  حالت HB_OT_METRICS_لیبل_SUPERSCRIPT_EM_X_سائز:       واپس GET_METRIC_X (OS2, ySuperscriptXSize);
  حالت HB_OT_METRICS_لیبل_SUPERSCRIPT_EM_Y_سائز:       واپس GET_METRIC_Y (OS2, ySuperscriptYSize);
  حالت HB_OT_METRICS_لیبل_SUPERSCRIPT_EM_X_انحراف:     واپس GET_METRIC_X (OS2, ySuperscriptXOffset);
  حالت HB_OT_METRICS_لیبل_SUPERSCRIPT_EM_Y_انحراف:     واپس GET_METRIC_Y (OS2, ySuperscriptYOffset);
  حالت HB_OT_METRICS_لیبل_STRIKEOUT_سائز:              واپس GET_METRIC_Y (OS2, y_Strikeout_سائز);
  حالت HB_OT_METRICS_لیبل_STRIKEOUT_انحراف:            واپس GET_METRIC_Y (OS2, yStrikeoutPosition);
  حالت HB_OT_METRICS_لیبل_UNDERLINE_سائز:              واپس GET_METRIC_Y (post->جدول, underlineThickness);
  حالت HB_OT_METRICS_لیبل_UNDERLINE_انحراف:            واپس GET_METRIC_Y (post->جدول, underlinePosition);

  /* Private tags */
  حالت HB_OT_METRICS_لیبل_HORIZONTAL_ASCENDER_OS2:    واپس GET_METRIC_Y (OS2, sTypoAscender);
  حالت HB_OT_METRICS_لیبل_HORIZONTAL_ASCENDER_HHEA:   واپس GET_HEA_METRIC_Y (hhea, ascender);
  حالت HB_OT_METRICS_لیبل_HORIZONTAL_DESCENDER_OS2:   واپس GET_METRIC_Y (OS2, sTypoDescender);
  حالت HB_OT_METRICS_لیبل_HORIZONTAL_DESCENDER_HHEA:  واپس GET_HEA_METRIC_Y (hhea, descender);
  حالت HB_OT_METRICS_لیبل_HORIZONTAL_سطر_GAP_OS2:    واپس GET_METRIC_Y (OS2, s_Typo_سطر_Gap);
  حالت HB_OT_METRICS_لیبل_HORIZONTAL_سطر_GAP_HHEA:   واپس GET_HEA_METRIC_Y (hhea, سطر_Gap);
#تعریف_منسوخ GET_HEA_METRIC_Y
#تعریف_منسوخ GET_HEA_METRIC_X
#تعریف_منسوخ GET_METRIC_Y
#تعریف_منسوخ GET_METRIC_X
#تعریف_منسوخ GET_VAR
  طےشدہ:                                        واپس جھوٹ;
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
خالی
hb_ot_metrics_get_position_with_fallback (hb_font_t           *font,
					  hb_ot_metrics_لیبل_t  metrics_لیبل,
					  hb_position_t       *position     /*برآمد*/)
{
  hb_font_extents_t font_extents;
  hb_codepoint_t glyph;
  hb_glyph_extents_t extents;

  اگر (hb_ot_metrics_get_position (font, metrics_لیبل, position))
    {
      اگر ((metrics_لیبل != HB_OT_METRICS_لیبل_STRIKEOUT_سائز &&
           metrics_لیبل != HB_OT_METRICS_لیبل_UNDERLINE_سائز) ||
          *position != 0)
        واپس;
    }

  انتخاب (metrics_لیبل)
  {
  حالت HB_OT_METRICS_لیبل_HORIZONTAL_ASCENDER:
  حالت HB_OT_METRICS_لیبل_HORIZONTAL_CLIPPING_ASCENT:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.ascender;
    توڑو;

  حالت HB_OT_METRICS_لیبل_VERTICAL_ASCENDER:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ascender;
    توڑو;

  حالت HB_OT_METRICS_لیبل_HORIZONTAL_DESCENDER:
  حالت HB_OT_METRICS_لیبل_HORIZONTAL_CLIPPING_DESCENT:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.descender;
    توڑو;

  حالت HB_OT_METRICS_لیبل_VERTICAL_DESCENDER:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ascender;
    توڑو;

  حالت HB_OT_METRICS_لیبل_HORIZONTAL_سطر_GAP:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.سطر_gap;
    توڑو;

  حالت HB_OT_METRICS_لیبل_VERTICAL_سطر_GAP:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.سطر_gap;
    توڑو;

  حالت HB_OT_METRICS_لیبل_HORIZONTAL_CARET_RISE:
  حالت HB_OT_METRICS_لیبل_VERTICAL_CARET_RISE:
    *position = 1;
    توڑو;

  حالت HB_OT_METRICS_لیبل_HORIZONTAL_CARET_تسلسل:
  حالت HB_OT_METRICS_لیبل_VERTICAL_CARET_تسلسل:
    *position = 0;
    توڑو;

  حالت HB_OT_METRICS_لیبل_HORIZONTAL_CARET_انحراف:
  حالت HB_OT_METRICS_لیبل_VERTICAL_CARET_انحراف:
    *position = 0;
    توڑو;

  حالت HB_OT_METRICS_لیبل_X_اونچائی:
    اگر (hb_font_get_nominal_glyph (font, 'x', &glyph) &&
        hb_font_get_glyph_extents (font, glyph, &extents))
      *position = extents.y_bearing;
    ورنہ
      *position = font->y_scale / 2;
    توڑو;

  حالت HB_OT_METRICS_لیبل_CAP_اونچائی:
    اگر (hb_font_get_nominal_glyph (font, 'O', &glyph) &&
        hb_font_get_glyph_extents (font, glyph, &extents))
      *position = hb_clamp_مقصد<hb_position_t> ((int64_t) extents.اونچائی +
					      2 * (int64_t) extents.y_bearing);
    ورنہ
      *position = hb_clamp_مقصد<hb_position_t> ((int64_t) font->y_scale * 2 / 3);
    توڑو;

  حالت HB_OT_METRICS_لیبل_STRIKEOUT_سائز:
  حالت HB_OT_METRICS_لیبل_UNDERLINE_سائز:
    *position = font->y_scale / 18;
    توڑو;

  حالت HB_OT_METRICS_لیبل_STRIKEOUT_انحراف:
    {
      hb_position_t ascender;
      hb_ot_metrics_get_position_with_fallback (font,
                                                HB_OT_METRICS_لیبل_HORIZONTAL_ASCENDER,
                                                &ascender);
      *position = ascender / 2;
    }
    توڑو;

  حالت HB_OT_METRICS_لیبل_UNDERLINE_انحراف:
    *position = hb_saturate_neg (font->y_scale) / 18;
    توڑو;

  حالت HB_OT_METRICS_لیبل_SUBSCRIPT_EM_X_سائز:
  حالت HB_OT_METRICS_لیبل_SUPERSCRIPT_EM_X_سائز:
    *position = hb_clamp_مقصد<hb_position_t> ((int64_t) font->x_scale * 10 / 12);
    توڑو;

  حالت HB_OT_METRICS_لیبل_SUBSCRIPT_EM_Y_سائز:
  حالت HB_OT_METRICS_لیبل_SUPERSCRIPT_EM_Y_سائز:
    *position = hb_clamp_مقصد<hb_position_t> ((int64_t) font->y_scale * 10 / 12);
    توڑو;

  حالت HB_OT_METRICS_لیبل_SUBSCRIPT_EM_X_انحراف:
  حالت HB_OT_METRICS_لیبل_SUPERSCRIPT_EM_X_انحراف:
    *position = 0;
    توڑو;

  حالت HB_OT_METRICS_لیبل_SUBSCRIPT_EM_Y_انحراف:
  حالت HB_OT_METRICS_لیبل_SUPERSCRIPT_EM_Y_انحراف:
    *position = font->y_scale / 5;
    توڑو;

  حالت HB_OT_METRICS_لیبل_MAX_قدر:
  طےشدہ:
    *position = 0;
    توڑو;
  }
}

#اگر_غیر_تعریف_شدہ HB_NO_VAR
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
hb_ot_metrics_get_variation (hb_font_t *font, hb_ot_metrics_لیبل_t metrics_لیبل)
{
  واپس font->face->جدول.MVAR->get_var (metrics_لیبل, font->coords, font->عدد_coords);
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
hb_ot_metrics_get_x_variation (hb_font_t *font, hb_ot_metrics_لیبل_t metrics_لیبل)
{
  واپس font->em_scalef_x (hb_ot_metrics_get_variation (font, metrics_لیبل));
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
hb_ot_metrics_get_y_variation (hb_font_t *font, hb_ot_metrics_لیبل_t metrics_لیبل)
{
  واپس font->em_scalef_y (hb_ot_metrics_get_variation (font, metrics_لیبل));
}
#اگر_ختم

#اگر_ختم

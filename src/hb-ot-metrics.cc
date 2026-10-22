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

#ضمن "hb.hh"

#ضمن "hb-ot-var-mvar-table.hh"
#ضمن "hb-ot-gasp-table.hh" // Just so we compile it; unused otherwise.
#ضمن "hb-ot-os2-table.hh"
#ضمن "hb-ot-post-table.hh"
#ضمن "hb-ot-hhea-table.hh"
#ضمن "hb-ot-metrics.hh"
#ضمن "hb-ot-face.hh"


/**
 * SECTION:hb-ot-metrics
 * @title: hb-ot-metrics
 * @short_description: OpenType Metrics
 * @include: hb-ot.h
 *
 * Functions for fetching metrics from fonts.
 **/

ساكن عائم
_fix_ascender_descender (عائم value, hb_ot_metrics_نښان_t metrics_نښان)
{
  إذا (metrics_نښان == HB_OT_METRICS_نښان_HORIZONTAL_ASCENDER ||
      metrics_نښان == HB_OT_METRICS_نښان_VERTICAL_ASCENDER)
    أرجع fabs ((مزدوج) value);
  إذا (metrics_نښان == HB_OT_METRICS_نښان_HORIZONTAL_DESCENDER ||
      metrics_نښان == HB_OT_METRICS_نښان_VERTICAL_DESCENDER)
    أرجع -fabs ((مزدوج) value);
  أرجع value;
}

/* The common part of _get_position logic needed on hb-ot-font and here
   to be able to have slim builds without the not always needed parts */
منطقي
_hb_ot_metrics_get_position_common (hb_font_t           *font,
				    hb_ot_metrics_نښان_t  metrics_نښان,
				    hb_position_t       *position     /* OUT.  May be NULL. */)
{
  hb_face_t *face = font->face;
  بدل ((بینښه) metrics_نښان)
  {
#که_نه_تعریف_شوی HB_NO_VAR
#تعریف_کړه GET_VAR face->جدول.MVAR->get_var (metrics_نښان, font->coords, font->عدد_coords)
#وإلا
#تعریف_کړه GET_VAR .0f
#د_که_پای
#تعریف_کړه GET_METRIC_Y(جدول_4, ATTR) \
  (face->جدول.جدول_4->has_معلومات () && \
    ((فراغ) (position && (*position = font->em_scalef_y (_fix_ascender_descender ( \
      face->جدول.جدول_4->ATTR + GET_VAR, metrics_نښان)))), ریښتیا))
#تعریف_کړه GET_HEA_METRIC_X(جدول_4, ATTR) \
  (face->جدول.جدول_4->has_معلومات () && \
    ((فراغ) (position && (*position = font->em_scalef_x (_fix_ascender_descender ( \
      HB_DUAL_GET (*face->جدول.جدول_4, جدول->ATTR) + GET_VAR, metrics_نښان)))), ریښتیا))
#تعریف_کړه GET_HEA_METRIC_Y(جدول_4, ATTR) \
  (face->جدول.جدول_4->has_معلومات () && \
    ((فراغ) (position && (*position = font->em_scalef_y (_fix_ascender_descender ( \
      HB_DUAL_GET (*face->جدول.جدول_4, جدول->ATTR) + GET_VAR, metrics_نښان)))), ریښتیا))

  حالة HB_OT_METRICS_نښان_HORIZONTAL_ASCENDER:
    أرجع (face->جدول.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, sTypoAscender)) ||
	   GET_HEA_METRIC_Y (hhea, ascender);
  حالة HB_OT_METRICS_نښان_HORIZONTAL_DESCENDER:
    أرجع (face->جدول.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, sTypoDescender)) ||
	   GET_HEA_METRIC_Y (hhea, descender);
  حالة HB_OT_METRICS_نښان_HORIZONTAL_کرښه_GAP:
    أرجع (face->جدول.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, s_Typo_کرښه_Gap)) ||
	   GET_HEA_METRIC_Y (hhea, کرښه_Gap);

#که_نه_تعریف_شوی HB_NO_VERTICAL
  حالة HB_OT_METRICS_نښان_VERTICAL_ASCENDER:  أرجع GET_HEA_METRIC_X (vhea, ascender);
  حالة HB_OT_METRICS_نښان_VERTICAL_DESCENDER: أرجع GET_HEA_METRIC_X (vhea, descender);
  حالة HB_OT_METRICS_نښان_VERTICAL_کرښه_GAP:  أرجع GET_HEA_METRIC_X (vhea, کرښه_Gap);
#د_که_پای

#تعریف_لغوه GET_HEA_METRIC_Y
#تعریف_لغوه GET_HEA_METRIC_X
#تعریف_لغوه GET_METRIC_Y
#تعریف_لغوه GET_VAR
  افتراضي:                               أكد (0); أرجع خطأ;
  }
}

#که_نه_تعریف_شوی HB_NO_METRICS

#إذا 0
ساكن منطقي
_get_gasp (hb_face_t *face, عائم *پایله, hb_ot_metrics_نښان_t metrics_نښان)
{
  ثابت OT::GaspRange& range = face->جدول.gasp->get_gasp_range (metrics_نښان - HB_نښان ('g','s','p','0'));
  إذا (&range == &Null (OT::GaspRange)) أرجع خطأ;
  إذا (پایله) *پایله = range.rangeMaxPPEM + font->face->جدول.MVAR->get_var (metrics_نښان, font->coords, font->عدد_coords);
  أرجع ریښتیا;
}
#د_که_پای

/* Private tags for https://github.com/harfbuzz/harfbuzz/issues/1866 */
#تعریف_کړه HB_OT_METRICS_نښان_HORIZONTAL_ASCENDER_OS2   HB_نښان ('O','a','s','c')
#تعریف_کړه HB_OT_METRICS_نښان_HORIZONTAL_ASCENDER_HHEA  HB_نښان ('H','a','s','c')
#تعریف_کړه HB_OT_METRICS_نښان_HORIZONTAL_DESCENDER_OS2  HB_نښان ('O','d','s','c')
#تعریف_کړه HB_OT_METRICS_نښان_HORIZONTAL_DESCENDER_HHEA HB_نښان ('H','d','s','c')
#تعریف_کړه HB_OT_METRICS_نښان_HORIZONTAL_کرښه_GAP_OS2   HB_نښان ('O','l','g','p')
#تعریف_کړه HB_OT_METRICS_نښان_HORIZONTAL_کرښه_GAP_HHEA  HB_نښان ('H','l','g','p')

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
			    hb_ot_metrics_نښان_t  metrics_نښان,
			    hb_position_t       *position     /* OUT.  May be NULL. */)
{
  hb_face_t *face = font->face;
  بدل ((بینښه) metrics_نښان)
  {
  حالة HB_OT_METRICS_نښان_HORIZONTAL_ASCENDER:
  حالة HB_OT_METRICS_نښان_HORIZONTAL_DESCENDER:
  حالة HB_OT_METRICS_نښان_HORIZONTAL_کرښه_GAP:
  حالة HB_OT_METRICS_نښان_VERTICAL_ASCENDER:
  حالة HB_OT_METRICS_نښان_VERTICAL_DESCENDER:
  حالة HB_OT_METRICS_نښان_VERTICAL_کرښه_GAP:           أرجع _hb_ot_metrics_get_position_common (font, metrics_نښان, position);
#که_نه_تعریف_شوی HB_NO_VAR
#تعریف_کړه GET_VAR hb_ot_metrics_get_variation (font, metrics_نښان)
#وإلا
#تعریف_کړه GET_VAR 0
#د_که_پای
#تعریف_کړه GET_METRIC_X(جدول_4, ATTR) \
  (face->جدول.جدول_4->has_معلومات () && \
    ((فراغ) (position && (*position = font->em_scalef_x (face->جدول.جدول_4->ATTR + GET_VAR))), ریښتیا))
#تعریف_کړه GET_METRIC_Y(جدول_4, ATTR) \
  (face->جدول.جدول_4->has_معلومات () && \
    ((فراغ) (position && (*position = font->em_scalef_y (face->جدول.جدول_4->ATTR + GET_VAR))), ریښتیا))
#تعریف_کړه GET_HEA_METRIC_X(جدول_4, ATTR) \
  (face->جدول.جدول_4->has_معلومات () && \
    ((فراغ) (position && (*position = font->em_scalef_x (HB_DUAL_GET (*face->جدول.جدول_4, جدول->ATTR) + GET_VAR))), ریښتیا))
#تعریف_کړه GET_HEA_METRIC_Y(جدول_4, ATTR) \
  (face->جدول.جدول_4->has_معلومات () && \
    ((فراغ) (position && (*position = font->em_scalef_y (HB_DUAL_GET (*face->جدول.جدول_4, جدول->ATTR) + GET_VAR))), ریښتیا))
  حالة HB_OT_METRICS_نښان_HORIZONTAL_CLIPPING_ASCENT:  أرجع GET_METRIC_Y (OS2, usWinAscent);
  حالة HB_OT_METRICS_نښان_HORIZONTAL_CLIPPING_DESCENT: أرجع GET_METRIC_Y (OS2, usWinDescent);

  حالة HB_OT_METRICS_نښان_HORIZONTAL_CARET_RISE:
  حالة HB_OT_METRICS_نښان_HORIZONTAL_CARET_پرلپسې:
  {
    بینښه mult = 1u;

    إذا (font->slant)
    {
      بینښه rise = HB_DUAL_GET (*face->جدول.hhea, جدول->caret_میلان_Rise);
      بینښه upem = face->get_upem ();
      mult = (rise && rise < upem) ? hb_min (upem / rise, 256u) : 1u;
    }

    إذا (metrics_نښان == HB_OT_METRICS_نښان_HORIZONTAL_CARET_RISE)
    {
      منطقي ret = GET_HEA_METRIC_Y (hhea, caret_میلان_Rise);

      إذا (position)
	*position = hb_clamp_منزل<hb_position_t> ((int64_t) *position * mult);

      أرجع ret;
    }
    وإلا
    {
      hb_position_t rise = 0;

      إذا (font->slant && position && GET_HEA_METRIC_Y (hhea, caret_میلان_Rise))
	rise = *position;

      منطقي ret = GET_HEA_METRIC_X (hhea, caret_میلان_پرلپسې);

      إذا (position)
      {
	*position = hb_clamp_منزل<hb_position_t> ((int64_t) *position * mult);

	إذا (font->slant)
	  *position = hb_saturate_جمع_کړه (*position,
				       hb_clamp_منزل<hb_position_t> (roundf (mult * font->slant_xy * rise)));
      }

      أرجع ret;
    }
  }
  حالة HB_OT_METRICS_نښان_HORIZONTAL_CARET_دننه_واټن:     أرجع GET_HEA_METRIC_X (hhea, caret_دننه_واټن);

#که_نه_تعریف_شوی HB_NO_VERTICAL
  حالة HB_OT_METRICS_نښان_VERTICAL_CARET_RISE:         أرجع GET_HEA_METRIC_X (vhea, caret_میلان_Rise);
  حالة HB_OT_METRICS_نښان_VERTICAL_CARET_پرلپسې:          أرجع GET_HEA_METRIC_Y (vhea, caret_میلان_پرلپسې);
  حالة HB_OT_METRICS_نښان_VERTICAL_CARET_دننه_واټن:       أرجع GET_HEA_METRIC_Y (vhea, caret_دننه_واټن);
#د_که_پای
  حالة HB_OT_METRICS_نښان_X_لوړوالی:                    أرجع GET_METRIC_Y (OS2->v2 (), sx_لوړوالی);
  حالة HB_OT_METRICS_نښان_CAP_لوړوالی:                  أرجع GET_METRIC_Y (OS2->v2 (), s_Cap_لوړوالی);
  حالة HB_OT_METRICS_نښان_SUBSCRIPT_EM_X_اندازه:         أرجع GET_METRIC_X (OS2, ySubscriptXSize);
  حالة HB_OT_METRICS_نښان_SUBSCRIPT_EM_Y_اندازه:         أرجع GET_METRIC_Y (OS2, ySubscriptYSize);
  حالة HB_OT_METRICS_نښان_SUBSCRIPT_EM_X_دننه_واټن:       أرجع GET_METRIC_X (OS2, ySubscriptXOffset);
  حالة HB_OT_METRICS_نښان_SUBSCRIPT_EM_Y_دننه_واټن:       أرجع GET_METRIC_Y (OS2, ySubscriptYOffset);
  حالة HB_OT_METRICS_نښان_SUPERSCRIPT_EM_X_اندازه:       أرجع GET_METRIC_X (OS2, ySuperscriptXSize);
  حالة HB_OT_METRICS_نښان_SUPERSCRIPT_EM_Y_اندازه:       أرجع GET_METRIC_Y (OS2, ySuperscriptYSize);
  حالة HB_OT_METRICS_نښان_SUPERSCRIPT_EM_X_دننه_واټن:     أرجع GET_METRIC_X (OS2, ySuperscriptXOffset);
  حالة HB_OT_METRICS_نښان_SUPERSCRIPT_EM_Y_دننه_واټن:     أرجع GET_METRIC_Y (OS2, ySuperscriptYOffset);
  حالة HB_OT_METRICS_نښان_STRIKEOUT_اندازه:              أرجع GET_METRIC_Y (OS2, y_Strikeout_اندازه);
  حالة HB_OT_METRICS_نښان_STRIKEOUT_دننه_واټن:            أرجع GET_METRIC_Y (OS2, yStrikeoutPosition);
  حالة HB_OT_METRICS_نښان_UNDERLINE_اندازه:              أرجع GET_METRIC_Y (post->جدول, underlineThickness);
  حالة HB_OT_METRICS_نښان_UNDERLINE_دننه_واټن:            أرجع GET_METRIC_Y (post->جدول, underlinePosition);

  /* Private tags */
  حالة HB_OT_METRICS_نښان_HORIZONTAL_ASCENDER_OS2:    أرجع GET_METRIC_Y (OS2, sTypoAscender);
  حالة HB_OT_METRICS_نښان_HORIZONTAL_ASCENDER_HHEA:   أرجع GET_HEA_METRIC_Y (hhea, ascender);
  حالة HB_OT_METRICS_نښان_HORIZONTAL_DESCENDER_OS2:   أرجع GET_METRIC_Y (OS2, sTypoDescender);
  حالة HB_OT_METRICS_نښان_HORIZONTAL_DESCENDER_HHEA:  أرجع GET_HEA_METRIC_Y (hhea, descender);
  حالة HB_OT_METRICS_نښان_HORIZONTAL_کرښه_GAP_OS2:    أرجع GET_METRIC_Y (OS2, s_Typo_کرښه_Gap);
  حالة HB_OT_METRICS_نښان_HORIZONTAL_کرښه_GAP_HHEA:   أرجع GET_HEA_METRIC_Y (hhea, کرښه_Gap);
#تعریف_لغوه GET_HEA_METRIC_Y
#تعریف_لغوه GET_HEA_METRIC_X
#تعریف_لغوه GET_METRIC_Y
#تعریف_لغوه GET_METRIC_X
#تعریف_لغوه GET_VAR
  افتراضي:                                        أرجع خطأ;
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
فراغ
hb_ot_metrics_get_position_with_fallback (hb_font_t           *font,
					  hb_ot_metrics_نښان_t  metrics_نښان,
					  hb_position_t       *position     /*بهر*/)
{
  hb_font_extents_t font_extents;
  hb_codepoint_t glyph;
  hb_glyph_extents_t extents;

  إذا (hb_ot_metrics_get_position (font, metrics_نښان, position))
    {
      إذا ((metrics_نښان != HB_OT_METRICS_نښان_STRIKEOUT_اندازه &&
           metrics_نښان != HB_OT_METRICS_نښان_UNDERLINE_اندازه) ||
          *position != 0)
        أرجع;
    }

  بدل (metrics_نښان)
  {
  حالة HB_OT_METRICS_نښان_HORIZONTAL_ASCENDER:
  حالة HB_OT_METRICS_نښان_HORIZONTAL_CLIPPING_ASCENT:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.ascender;
    اكسر;

  حالة HB_OT_METRICS_نښان_VERTICAL_ASCENDER:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ascender;
    اكسر;

  حالة HB_OT_METRICS_نښان_HORIZONTAL_DESCENDER:
  حالة HB_OT_METRICS_نښان_HORIZONTAL_CLIPPING_DESCENT:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.descender;
    اكسر;

  حالة HB_OT_METRICS_نښان_VERTICAL_DESCENDER:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ascender;
    اكسر;

  حالة HB_OT_METRICS_نښان_HORIZONTAL_کرښه_GAP:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.کرښه_gap;
    اكسر;

  حالة HB_OT_METRICS_نښان_VERTICAL_کرښه_GAP:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.کرښه_gap;
    اكسر;

  حالة HB_OT_METRICS_نښان_HORIZONTAL_CARET_RISE:
  حالة HB_OT_METRICS_نښان_VERTICAL_CARET_RISE:
    *position = 1;
    اكسر;

  حالة HB_OT_METRICS_نښان_HORIZONTAL_CARET_پرلپسې:
  حالة HB_OT_METRICS_نښان_VERTICAL_CARET_پرلپسې:
    *position = 0;
    اكسر;

  حالة HB_OT_METRICS_نښان_HORIZONTAL_CARET_دننه_واټن:
  حالة HB_OT_METRICS_نښان_VERTICAL_CARET_دننه_واټن:
    *position = 0;
    اكسر;

  حالة HB_OT_METRICS_نښان_X_لوړوالی:
    إذا (hb_font_get_nominal_glyph (font, 'x', &glyph) &&
        hb_font_get_glyph_extents (font, glyph, &extents))
      *position = extents.y_bearing;
    وإلا
      *position = font->y_scale / 2;
    اكسر;

  حالة HB_OT_METRICS_نښان_CAP_لوړوالی:
    إذا (hb_font_get_nominal_glyph (font, 'O', &glyph) &&
        hb_font_get_glyph_extents (font, glyph, &extents))
      *position = hb_clamp_منزل<hb_position_t> ((int64_t) extents.لوړوالی +
					      2 * (int64_t) extents.y_bearing);
    وإلا
      *position = hb_clamp_منزل<hb_position_t> ((int64_t) font->y_scale * 2 / 3);
    اكسر;

  حالة HB_OT_METRICS_نښان_STRIKEOUT_اندازه:
  حالة HB_OT_METRICS_نښان_UNDERLINE_اندازه:
    *position = font->y_scale / 18;
    اكسر;

  حالة HB_OT_METRICS_نښان_STRIKEOUT_دننه_واټن:
    {
      hb_position_t ascender;
      hb_ot_metrics_get_position_with_fallback (font,
                                                HB_OT_METRICS_نښان_HORIZONTAL_ASCENDER,
                                                &ascender);
      *position = ascender / 2;
    }
    اكسر;

  حالة HB_OT_METRICS_نښان_UNDERLINE_دننه_واټن:
    *position = hb_saturate_neg (font->y_scale) / 18;
    اكسر;

  حالة HB_OT_METRICS_نښان_SUBSCRIPT_EM_X_اندازه:
  حالة HB_OT_METRICS_نښان_SUPERSCRIPT_EM_X_اندازه:
    *position = hb_clamp_منزل<hb_position_t> ((int64_t) font->x_scale * 10 / 12);
    اكسر;

  حالة HB_OT_METRICS_نښان_SUBSCRIPT_EM_Y_اندازه:
  حالة HB_OT_METRICS_نښان_SUPERSCRIPT_EM_Y_اندازه:
    *position = hb_clamp_منزل<hb_position_t> ((int64_t) font->y_scale * 10 / 12);
    اكسر;

  حالة HB_OT_METRICS_نښان_SUBSCRIPT_EM_X_دننه_واټن:
  حالة HB_OT_METRICS_نښان_SUPERSCRIPT_EM_X_دننه_واټن:
    *position = 0;
    اكسر;

  حالة HB_OT_METRICS_نښان_SUBSCRIPT_EM_Y_دننه_واټن:
  حالة HB_OT_METRICS_نښان_SUPERSCRIPT_EM_Y_دننه_واټن:
    *position = font->y_scale / 5;
    اكسر;

  حالة HB_OT_METRICS_نښان_MAX_ارزښت:
  افتراضي:
    *position = 0;
    اكسر;
  }
}

#که_نه_تعریف_شوی HB_NO_VAR
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
عائم
hb_ot_metrics_get_variation (hb_font_t *font, hb_ot_metrics_نښان_t metrics_نښان)
{
  أرجع font->face->جدول.MVAR->get_var (metrics_نښان, font->coords, font->عدد_coords);
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
hb_ot_metrics_get_x_variation (hb_font_t *font, hb_ot_metrics_نښان_t metrics_نښان)
{
  أرجع font->em_scalef_x (hb_ot_metrics_get_variation (font, metrics_نښان));
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
hb_ot_metrics_get_y_variation (hb_font_t *font, hb_ot_metrics_نښان_t metrics_نښان)
{
  أرجع font->em_scalef_y (hb_ot_metrics_get_variation (font, metrics_نښان));
}
#د_که_پای

#د_که_پای

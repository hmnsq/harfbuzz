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
_fix_ascender_descender (عائم value, hb_ot_metrics_ٽيگ_t metrics_ٽيگ)
{
  إذا (metrics_ٽيگ == HB_OT_METRICS_ٽيگ_HORIZONTAL_ASCENDER ||
      metrics_ٽيگ == HB_OT_METRICS_ٽيگ_VERTICAL_ASCENDER)
    أرجع fabs ((مزدوج) value);
  إذا (metrics_ٽيگ == HB_OT_METRICS_ٽيگ_HORIZONTAL_DESCENDER ||
      metrics_ٽيگ == HB_OT_METRICS_ٽيگ_VERTICAL_DESCENDER)
    أرجع -fabs ((مزدوج) value);
  أرجع value;
}

/* The common part of _get_position logic needed on hb-ot-font and here
   to be able to have slim builds without the not always needed parts */
منطقي
_hb_ot_metrics_get_position_common (hb_font_t           *font,
				    hb_ot_metrics_ٽيگ_t  metrics_ٽيگ,
				    hb_position_t       *position     /* OUT.  May be NULL. */)
{
  hb_face_t *face = font->face;
  بدل ((بنانشان) metrics_ٽيگ)
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
#وصف_ڪريو GET_VAR face->جدول.MVAR->get_var (metrics_ٽيگ, font->coords, font->انگ_coords)
#وإلا
#وصف_ڪريو GET_VAR .0f
#جيڪڏهن_پورو
#وصف_ڪريو GET_METRIC_Y(جدول_4, ATTR) \
  (face->جدول.جدول_4->has_معلومات () && \
    ((فراغ) (position && (*position = font->em_scalef_y (_fix_ascender_descender ( \
      face->جدول.جدول_4->ATTR + GET_VAR, metrics_ٽيگ)))), سچ))
#وصف_ڪريو GET_HEA_METRIC_X(جدول_4, ATTR) \
  (face->جدول.جدول_4->has_معلومات () && \
    ((فراغ) (position && (*position = font->em_scalef_x (_fix_ascender_descender ( \
      HB_DUAL_GET (*face->جدول.جدول_4, جدول->ATTR) + GET_VAR, metrics_ٽيگ)))), سچ))
#وصف_ڪريو GET_HEA_METRIC_Y(جدول_4, ATTR) \
  (face->جدول.جدول_4->has_معلومات () && \
    ((فراغ) (position && (*position = font->em_scalef_y (_fix_ascender_descender ( \
      HB_DUAL_GET (*face->جدول.جدول_4, جدول->ATTR) + GET_VAR, metrics_ٽيگ)))), سچ))

  حالة HB_OT_METRICS_ٽيگ_HORIZONTAL_ASCENDER:
    أرجع (face->جدول.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, sTypoAscender)) ||
	   GET_HEA_METRIC_Y (hhea, ascender);
  حالة HB_OT_METRICS_ٽيگ_HORIZONTAL_DESCENDER:
    أرجع (face->جدول.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, sTypoDescender)) ||
	   GET_HEA_METRIC_Y (hhea, descender);
  حالة HB_OT_METRICS_ٽيگ_HORIZONTAL_سٽ_GAP:
    أرجع (face->جدول.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, s_Typo_سٽ_Gap)) ||
	   GET_HEA_METRIC_Y (hhea, سٽ_Gap);

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VERTICAL
  حالة HB_OT_METRICS_ٽيگ_VERTICAL_ASCENDER:  أرجع GET_HEA_METRIC_X (vhea, ascender);
  حالة HB_OT_METRICS_ٽيگ_VERTICAL_DESCENDER: أرجع GET_HEA_METRIC_X (vhea, descender);
  حالة HB_OT_METRICS_ٽيگ_VERTICAL_سٽ_GAP:  أرجع GET_HEA_METRIC_X (vhea, سٽ_Gap);
#جيڪڏهن_پورو

#وصف_رد GET_HEA_METRIC_Y
#وصف_رد GET_HEA_METRIC_X
#وصف_رد GET_METRIC_Y
#وصف_رد GET_VAR
  افتراضي:                               أكد (0); أرجع خطأ;
  }
}

#جيڪڏهن_وصف_نه_ٿيل HB_NO_METRICS

#إذا 0
ساكن منطقي
_get_gasp (hb_face_t *face, عائم *نتيجو, hb_ot_metrics_ٽيگ_t metrics_ٽيگ)
{
  ثابت OT::GaspRange& range = face->جدول.gasp->get_gasp_range (metrics_ٽيگ - HB_ٽيگ ('g','s','p','0'));
  إذا (&range == &Null (OT::GaspRange)) أرجع خطأ;
  إذا (نتيجو) *نتيجو = range.rangeMaxPPEM + font->face->جدول.MVAR->get_var (metrics_ٽيگ, font->coords, font->انگ_coords);
  أرجع سچ;
}
#جيڪڏهن_پورو

/* Private tags for https://github.com/harfbuzz/harfbuzz/issues/1866 */
#وصف_ڪريو HB_OT_METRICS_ٽيگ_HORIZONTAL_ASCENDER_OS2   HB_ٽيگ ('O','a','s','c')
#وصف_ڪريو HB_OT_METRICS_ٽيگ_HORIZONTAL_ASCENDER_HHEA  HB_ٽيگ ('H','a','s','c')
#وصف_ڪريو HB_OT_METRICS_ٽيگ_HORIZONTAL_DESCENDER_OS2  HB_ٽيگ ('O','d','s','c')
#وصف_ڪريو HB_OT_METRICS_ٽيگ_HORIZONTAL_DESCENDER_HHEA HB_ٽيگ ('H','d','s','c')
#وصف_ڪريو HB_OT_METRICS_ٽيگ_HORIZONTAL_سٽ_GAP_OS2   HB_ٽيگ ('O','l','g','p')
#وصف_ڪريو HB_OT_METRICS_ٽيگ_HORIZONTAL_سٽ_GAP_HHEA  HB_ٽيگ ('H','l','g','p')

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
			    hb_ot_metrics_ٽيگ_t  metrics_ٽيگ,
			    hb_position_t       *position     /* OUT.  May be NULL. */)
{
  hb_face_t *face = font->face;
  بدل ((بنانشان) metrics_ٽيگ)
  {
  حالة HB_OT_METRICS_ٽيگ_HORIZONTAL_ASCENDER:
  حالة HB_OT_METRICS_ٽيگ_HORIZONTAL_DESCENDER:
  حالة HB_OT_METRICS_ٽيگ_HORIZONTAL_سٽ_GAP:
  حالة HB_OT_METRICS_ٽيگ_VERTICAL_ASCENDER:
  حالة HB_OT_METRICS_ٽيگ_VERTICAL_DESCENDER:
  حالة HB_OT_METRICS_ٽيگ_VERTICAL_سٽ_GAP:           أرجع _hb_ot_metrics_get_position_common (font, metrics_ٽيگ, position);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
#وصف_ڪريو GET_VAR hb_ot_metrics_get_variation (font, metrics_ٽيگ)
#وإلا
#وصف_ڪريو GET_VAR 0
#جيڪڏهن_پورو
#وصف_ڪريو GET_METRIC_X(جدول_4, ATTR) \
  (face->جدول.جدول_4->has_معلومات () && \
    ((فراغ) (position && (*position = font->em_scalef_x (face->جدول.جدول_4->ATTR + GET_VAR))), سچ))
#وصف_ڪريو GET_METRIC_Y(جدول_4, ATTR) \
  (face->جدول.جدول_4->has_معلومات () && \
    ((فراغ) (position && (*position = font->em_scalef_y (face->جدول.جدول_4->ATTR + GET_VAR))), سچ))
#وصف_ڪريو GET_HEA_METRIC_X(جدول_4, ATTR) \
  (face->جدول.جدول_4->has_معلومات () && \
    ((فراغ) (position && (*position = font->em_scalef_x (HB_DUAL_GET (*face->جدول.جدول_4, جدول->ATTR) + GET_VAR))), سچ))
#وصف_ڪريو GET_HEA_METRIC_Y(جدول_4, ATTR) \
  (face->جدول.جدول_4->has_معلومات () && \
    ((فراغ) (position && (*position = font->em_scalef_y (HB_DUAL_GET (*face->جدول.جدول_4, جدول->ATTR) + GET_VAR))), سچ))
  حالة HB_OT_METRICS_ٽيگ_HORIZONTAL_CLIPPING_ASCENT:  أرجع GET_METRIC_Y (OS2, usWinAscent);
  حالة HB_OT_METRICS_ٽيگ_HORIZONTAL_CLIPPING_DESCENT: أرجع GET_METRIC_Y (OS2, usWinDescent);

  حالة HB_OT_METRICS_ٽيگ_HORIZONTAL_CARET_RISE:
  حالة HB_OT_METRICS_ٽيگ_HORIZONTAL_CARET_لاڳيتو:
  {
    بنانشان mult = 1u;

    إذا (font->slant)
    {
      بنانشان rise = HB_DUAL_GET (*face->جدول.hhea, جدول->caret_لاهي_Rise);
      بنانشان upem = face->get_upem ();
      mult = (rise && rise < upem) ? hb_min (upem / rise, 256u) : 1u;
    }

    إذا (metrics_ٽيگ == HB_OT_METRICS_ٽيگ_HORIZONTAL_CARET_RISE)
    {
      منطقي ret = GET_HEA_METRIC_Y (hhea, caret_لاهي_Rise);

      إذا (position)
	*position = hb_clamp_منزل<hb_position_t> ((int64_t) *position * mult);

      أرجع ret;
    }
    وإلا
    {
      hb_position_t rise = 0;

      إذا (font->slant && position && GET_HEA_METRIC_Y (hhea, caret_لاهي_Rise))
	rise = *position;

      منطقي ret = GET_HEA_METRIC_X (hhea, caret_لاهي_لاڳيتو);

      إذا (position)
      {
	*position = hb_clamp_منزل<hb_position_t> ((int64_t) *position * mult);

	إذا (font->slant)
	  *position = hb_saturate_جوڙيو (*position,
				       hb_clamp_منزل<hb_position_t> (roundf (mult * font->slant_xy * rise)));
      }

      أرجع ret;
    }
  }
  حالة HB_OT_METRICS_ٽيگ_HORIZONTAL_CARET_وٿي:     أرجع GET_HEA_METRIC_X (hhea, caret_وٿي);

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VERTICAL
  حالة HB_OT_METRICS_ٽيگ_VERTICAL_CARET_RISE:         أرجع GET_HEA_METRIC_X (vhea, caret_لاهي_Rise);
  حالة HB_OT_METRICS_ٽيگ_VERTICAL_CARET_لاڳيتو:          أرجع GET_HEA_METRIC_Y (vhea, caret_لاهي_لاڳيتو);
  حالة HB_OT_METRICS_ٽيگ_VERTICAL_CARET_وٿي:       أرجع GET_HEA_METRIC_Y (vhea, caret_وٿي);
#جيڪڏهن_پورو
  حالة HB_OT_METRICS_ٽيگ_X_اوچائي:                    أرجع GET_METRIC_Y (OS2->v2 (), sx_اوچائي);
  حالة HB_OT_METRICS_ٽيگ_CAP_اوچائي:                  أرجع GET_METRIC_Y (OS2->v2 (), s_Cap_اوچائي);
  حالة HB_OT_METRICS_ٽيگ_SUBSCRIPT_EM_X_سائيز:         أرجع GET_METRIC_X (OS2, ySubscriptXSize);
  حالة HB_OT_METRICS_ٽيگ_SUBSCRIPT_EM_Y_سائيز:         أرجع GET_METRIC_Y (OS2, ySubscriptYSize);
  حالة HB_OT_METRICS_ٽيگ_SUBSCRIPT_EM_X_وٿي:       أرجع GET_METRIC_X (OS2, ySubscriptXOffset);
  حالة HB_OT_METRICS_ٽيگ_SUBSCRIPT_EM_Y_وٿي:       أرجع GET_METRIC_Y (OS2, ySubscriptYOffset);
  حالة HB_OT_METRICS_ٽيگ_SUPERSCRIPT_EM_X_سائيز:       أرجع GET_METRIC_X (OS2, ySuperscriptXSize);
  حالة HB_OT_METRICS_ٽيگ_SUPERSCRIPT_EM_Y_سائيز:       أرجع GET_METRIC_Y (OS2, ySuperscriptYSize);
  حالة HB_OT_METRICS_ٽيگ_SUPERSCRIPT_EM_X_وٿي:     أرجع GET_METRIC_X (OS2, ySuperscriptXOffset);
  حالة HB_OT_METRICS_ٽيگ_SUPERSCRIPT_EM_Y_وٿي:     أرجع GET_METRIC_Y (OS2, ySuperscriptYOffset);
  حالة HB_OT_METRICS_ٽيگ_STRIKEOUT_سائيز:              أرجع GET_METRIC_Y (OS2, y_Strikeout_سائيز);
  حالة HB_OT_METRICS_ٽيگ_STRIKEOUT_وٿي:            أرجع GET_METRIC_Y (OS2, yStrikeoutPosition);
  حالة HB_OT_METRICS_ٽيگ_UNDERLINE_سائيز:              أرجع GET_METRIC_Y (post->جدول, underlineThickness);
  حالة HB_OT_METRICS_ٽيگ_UNDERLINE_وٿي:            أرجع GET_METRIC_Y (post->جدول, underlinePosition);

  /* Private tags */
  حالة HB_OT_METRICS_ٽيگ_HORIZONTAL_ASCENDER_OS2:    أرجع GET_METRIC_Y (OS2, sTypoAscender);
  حالة HB_OT_METRICS_ٽيگ_HORIZONTAL_ASCENDER_HHEA:   أرجع GET_HEA_METRIC_Y (hhea, ascender);
  حالة HB_OT_METRICS_ٽيگ_HORIZONTAL_DESCENDER_OS2:   أرجع GET_METRIC_Y (OS2, sTypoDescender);
  حالة HB_OT_METRICS_ٽيگ_HORIZONTAL_DESCENDER_HHEA:  أرجع GET_HEA_METRIC_Y (hhea, descender);
  حالة HB_OT_METRICS_ٽيگ_HORIZONTAL_سٽ_GAP_OS2:    أرجع GET_METRIC_Y (OS2, s_Typo_سٽ_Gap);
  حالة HB_OT_METRICS_ٽيگ_HORIZONTAL_سٽ_GAP_HHEA:   أرجع GET_HEA_METRIC_Y (hhea, سٽ_Gap);
#وصف_رد GET_HEA_METRIC_Y
#وصف_رد GET_HEA_METRIC_X
#وصف_رد GET_METRIC_Y
#وصف_رد GET_METRIC_X
#وصف_رد GET_VAR
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
					  hb_ot_metrics_ٽيگ_t  metrics_ٽيگ,
					  hb_position_t       *position     /*ٻاهريون*/)
{
  hb_font_extents_t font_extents;
  hb_codepoint_t glyph;
  hb_glyph_extents_t extents;

  إذا (hb_ot_metrics_get_position (font, metrics_ٽيگ, position))
    {
      إذا ((metrics_ٽيگ != HB_OT_METRICS_ٽيگ_STRIKEOUT_سائيز &&
           metrics_ٽيگ != HB_OT_METRICS_ٽيگ_UNDERLINE_سائيز) ||
          *position != 0)
        أرجع;
    }

  بدل (metrics_ٽيگ)
  {
  حالة HB_OT_METRICS_ٽيگ_HORIZONTAL_ASCENDER:
  حالة HB_OT_METRICS_ٽيگ_HORIZONTAL_CLIPPING_ASCENT:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.ascender;
    اكسر;

  حالة HB_OT_METRICS_ٽيگ_VERTICAL_ASCENDER:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ascender;
    اكسر;

  حالة HB_OT_METRICS_ٽيگ_HORIZONTAL_DESCENDER:
  حالة HB_OT_METRICS_ٽيگ_HORIZONTAL_CLIPPING_DESCENT:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.descender;
    اكسر;

  حالة HB_OT_METRICS_ٽيگ_VERTICAL_DESCENDER:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ascender;
    اكسر;

  حالة HB_OT_METRICS_ٽيگ_HORIZONTAL_سٽ_GAP:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.سٽ_gap;
    اكسر;

  حالة HB_OT_METRICS_ٽيگ_VERTICAL_سٽ_GAP:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.سٽ_gap;
    اكسر;

  حالة HB_OT_METRICS_ٽيگ_HORIZONTAL_CARET_RISE:
  حالة HB_OT_METRICS_ٽيگ_VERTICAL_CARET_RISE:
    *position = 1;
    اكسر;

  حالة HB_OT_METRICS_ٽيگ_HORIZONTAL_CARET_لاڳيتو:
  حالة HB_OT_METRICS_ٽيگ_VERTICAL_CARET_لاڳيتو:
    *position = 0;
    اكسر;

  حالة HB_OT_METRICS_ٽيگ_HORIZONTAL_CARET_وٿي:
  حالة HB_OT_METRICS_ٽيگ_VERTICAL_CARET_وٿي:
    *position = 0;
    اكسر;

  حالة HB_OT_METRICS_ٽيگ_X_اوچائي:
    إذا (hb_font_get_nominal_glyph (font, 'x', &glyph) &&
        hb_font_get_glyph_extents (font, glyph, &extents))
      *position = extents.y_bearing;
    وإلا
      *position = font->y_scale / 2;
    اكسر;

  حالة HB_OT_METRICS_ٽيگ_CAP_اوچائي:
    إذا (hb_font_get_nominal_glyph (font, 'O', &glyph) &&
        hb_font_get_glyph_extents (font, glyph, &extents))
      *position = hb_clamp_منزل<hb_position_t> ((int64_t) extents.اوچائي +
					      2 * (int64_t) extents.y_bearing);
    وإلا
      *position = hb_clamp_منزل<hb_position_t> ((int64_t) font->y_scale * 2 / 3);
    اكسر;

  حالة HB_OT_METRICS_ٽيگ_STRIKEOUT_سائيز:
  حالة HB_OT_METRICS_ٽيگ_UNDERLINE_سائيز:
    *position = font->y_scale / 18;
    اكسر;

  حالة HB_OT_METRICS_ٽيگ_STRIKEOUT_وٿي:
    {
      hb_position_t ascender;
      hb_ot_metrics_get_position_with_fallback (font,
                                                HB_OT_METRICS_ٽيگ_HORIZONTAL_ASCENDER,
                                                &ascender);
      *position = ascender / 2;
    }
    اكسر;

  حالة HB_OT_METRICS_ٽيگ_UNDERLINE_وٿي:
    *position = hb_saturate_neg (font->y_scale) / 18;
    اكسر;

  حالة HB_OT_METRICS_ٽيگ_SUBSCRIPT_EM_X_سائيز:
  حالة HB_OT_METRICS_ٽيگ_SUPERSCRIPT_EM_X_سائيز:
    *position = hb_clamp_منزل<hb_position_t> ((int64_t) font->x_scale * 10 / 12);
    اكسر;

  حالة HB_OT_METRICS_ٽيگ_SUBSCRIPT_EM_Y_سائيز:
  حالة HB_OT_METRICS_ٽيگ_SUPERSCRIPT_EM_Y_سائيز:
    *position = hb_clamp_منزل<hb_position_t> ((int64_t) font->y_scale * 10 / 12);
    اكسر;

  حالة HB_OT_METRICS_ٽيگ_SUBSCRIPT_EM_X_وٿي:
  حالة HB_OT_METRICS_ٽيگ_SUPERSCRIPT_EM_X_وٿي:
    *position = 0;
    اكسر;

  حالة HB_OT_METRICS_ٽيگ_SUBSCRIPT_EM_Y_وٿي:
  حالة HB_OT_METRICS_ٽيگ_SUPERSCRIPT_EM_Y_وٿي:
    *position = font->y_scale / 5;
    اكسر;

  حالة HB_OT_METRICS_ٽيگ_MAX_قدر:
  افتراضي:
    *position = 0;
    اكسر;
  }
}

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
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
hb_ot_metrics_get_variation (hb_font_t *font, hb_ot_metrics_ٽيگ_t metrics_ٽيگ)
{
  أرجع font->face->جدول.MVAR->get_var (metrics_ٽيگ, font->coords, font->انگ_coords);
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
hb_ot_metrics_get_x_variation (hb_font_t *font, hb_ot_metrics_ٽيگ_t metrics_ٽيگ)
{
  أرجع font->em_scalef_x (hb_ot_metrics_get_variation (font, metrics_ٽيگ));
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
hb_ot_metrics_get_y_variation (hb_font_t *font, hb_ot_metrics_ٽيگ_t metrics_ٽيگ)
{
  أرجع font->em_scalef_y (hb_ot_metrics_get_variation (font, metrics_ٽيگ));
}
#جيڪڏهن_پورو

#جيڪڏهن_پورو

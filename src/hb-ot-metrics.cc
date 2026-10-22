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
_fix_ascender_descender (عائم value, hb_ot_metrics_tag_t metrics_tag)
{
  إذا (metrics_tag == HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER ||
      metrics_tag == HB_OT_METRICS_TAG_VERTICAL_ASCENDER)
    أرجع fabs ((مزدوج) value);
  إذا (metrics_tag == HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER ||
      metrics_tag == HB_OT_METRICS_TAG_VERTICAL_DESCENDER)
    أرجع -fabs ((مزدوج) value);
  أرجع value;
}

/* The common part of _get_position logic needed on hb-ot-font and here
   to be able to have slim builds without the not always needed parts */
منطقي
_hb_ot_metrics_get_position_common (hb_font_t           *font,
				    hb_ot_metrics_tag_t  metrics_tag,
				    hb_position_t       *position     /* OUT.  May be NULL. */)
{
  hb_face_t *face = font->face;
  بدل ((unsigned) metrics_tag)
  {
#ifndef HB_NO_VAR
#define GET_VAR face->table.MVAR->get_var (metrics_tag, font->coords, font->رقم_coords)
#وإلا
#define GET_VAR .0f
#endif
#define GET_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((فراغ) (position && (*position = font->em_scalef_y (_fix_ascender_descender ( \
      face->table.TABLE->ATTR + GET_VAR, metrics_tag)))), true))
#define GET_HEA_METRIC_X(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((فراغ) (position && (*position = font->em_scalef_x (_fix_ascender_descender ( \
      HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR, metrics_tag)))), true))
#define GET_HEA_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((فراغ) (position && (*position = font->em_scalef_y (_fix_ascender_descender ( \
      HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR, metrics_tag)))), true))

  حالة HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER:
    أرجع (face->table.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, sTypoAscender)) ||
	   GET_HEA_METRIC_Y (hhea, ascender);
  حالة HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER:
    أرجع (face->table.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, sTypoDescender)) ||
	   GET_HEA_METRIC_Y (hhea, descender);
  حالة HB_OT_METRICS_TAG_HORIZONTAL_سطر_GAP:
    أرجع (face->table.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, s_Typo_سطر_Gap)) ||
	   GET_HEA_METRIC_Y (hhea, سطر_Gap);

#ifndef HB_NO_VERTICAL
  حالة HB_OT_METRICS_TAG_VERTICAL_ASCENDER:  أرجع GET_HEA_METRIC_X (vhea, ascender);
  حالة HB_OT_METRICS_TAG_VERTICAL_DESCENDER: أرجع GET_HEA_METRIC_X (vhea, descender);
  حالة HB_OT_METRICS_TAG_VERTICAL_سطر_GAP:  أرجع GET_HEA_METRIC_X (vhea, سطر_Gap);
#endif

#undef GET_HEA_METRIC_Y
#undef GET_HEA_METRIC_X
#undef GET_METRIC_Y
#undef GET_VAR
  افتراضي:                               أكد (0); أرجع خطأ;
  }
}

#ifndef HB_NO_METRICS

#إذا 0
ساكن منطقي
_get_gasp (hb_face_t *face, عائم *result, hb_ot_metrics_tag_t metrics_tag)
{
  ثابت OT::GaspRange& range = face->table.gasp->get_gasp_range (metrics_tag - HB_TAG ('g','s','p','0'));
  إذا (&range == &Null (OT::GaspRange)) أرجع خطأ;
  إذا (result) *result = range.rangeMaxPPEM + font->face->table.MVAR->get_var (metrics_tag, font->coords, font->رقم_coords);
  أرجع true;
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
  بدل ((unsigned) metrics_tag)
  {
  حالة HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER:
  حالة HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER:
  حالة HB_OT_METRICS_TAG_HORIZONTAL_سطر_GAP:
  حالة HB_OT_METRICS_TAG_VERTICAL_ASCENDER:
  حالة HB_OT_METRICS_TAG_VERTICAL_DESCENDER:
  حالة HB_OT_METRICS_TAG_VERTICAL_سطر_GAP:           أرجع _hb_ot_metrics_get_position_common (font, metrics_tag, position);
#ifndef HB_NO_VAR
#define GET_VAR hb_ot_metrics_get_variation (font, metrics_tag)
#وإلا
#define GET_VAR 0
#endif
#define GET_METRIC_X(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((فراغ) (position && (*position = font->em_scalef_x (face->table.TABLE->ATTR + GET_VAR))), true))
#define GET_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((فراغ) (position && (*position = font->em_scalef_y (face->table.TABLE->ATTR + GET_VAR))), true))
#define GET_HEA_METRIC_X(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((فراغ) (position && (*position = font->em_scalef_x (HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR))), true))
#define GET_HEA_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((فراغ) (position && (*position = font->em_scalef_y (HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR))), true))
  حالة HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_ASCENT:  أرجع GET_METRIC_Y (OS2, usWinAscent);
  حالة HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_DESCENT: أرجع GET_METRIC_Y (OS2, usWinDescent);

  حالة HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE:
  حالة HB_OT_METRICS_TAG_HORIZONTAL_CARET_RUN:
  {
    unsigned mult = 1u;

    إذا (font->slant)
    {
      unsigned rise = HB_DUAL_GET (*face->table.hhea, table->caretSlopeRise);
      unsigned upem = face->get_upem ();
      mult = (rise && rise < upem) ? hb_min (upem / rise, 256u) : 1u;
    }

    إذا (metrics_tag == HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE)
    {
      منطقي ret = GET_HEA_METRIC_Y (hhea, caretSlopeRise);

      إذا (position)
	*position = hb_clamp_to<hb_position_t> ((int64_t) *position * mult);

      أرجع ret;
    }
    وإلا
    {
      hb_position_t rise = 0;

      إذا (font->slant && position && GET_HEA_METRIC_Y (hhea, caretSlopeRise))
	rise = *position;

      منطقي ret = GET_HEA_METRIC_X (hhea, caretSlopeRun);

      إذا (position)
      {
	*position = hb_clamp_to<hb_position_t> ((int64_t) *position * mult);

	إذا (font->slant)
	  *position = hb_saturate_add (*position,
				       hb_clamp_to<hb_position_t> (roundf (mult * font->slant_xy * rise)));
      }

      أرجع ret;
    }
  }
  حالة HB_OT_METRICS_TAG_HORIZONTAL_CARET_OFFSET:     أرجع GET_HEA_METRIC_X (hhea, caretOffset);

#ifndef HB_NO_VERTICAL
  حالة HB_OT_METRICS_TAG_VERTICAL_CARET_RISE:         أرجع GET_HEA_METRIC_X (vhea, caretSlopeRise);
  حالة HB_OT_METRICS_TAG_VERTICAL_CARET_RUN:          أرجع GET_HEA_METRIC_Y (vhea, caretSlopeRun);
  حالة HB_OT_METRICS_TAG_VERTICAL_CARET_OFFSET:       أرجع GET_HEA_METRIC_Y (vhea, caretOffset);
#endif
  حالة HB_OT_METRICS_TAG_X_HEIGHT:                    أرجع GET_METRIC_Y (OS2->v2 (), sxHeight);
  حالة HB_OT_METRICS_TAG_CAP_HEIGHT:                  أرجع GET_METRIC_Y (OS2->v2 (), sCapHeight);
  حالة HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_SIZE:         أرجع GET_METRIC_X (OS2, ySubscriptXSize);
  حالة HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_SIZE:         أرجع GET_METRIC_Y (OS2, ySubscriptYSize);
  حالة HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_OFFSET:       أرجع GET_METRIC_X (OS2, ySubscriptXOffset);
  حالة HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_OFFSET:       أرجع GET_METRIC_Y (OS2, ySubscriptYOffset);
  حالة HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_SIZE:       أرجع GET_METRIC_X (OS2, ySuperscriptXSize);
  حالة HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_SIZE:       أرجع GET_METRIC_Y (OS2, ySuperscriptYSize);
  حالة HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_OFFSET:     أرجع GET_METRIC_X (OS2, ySuperscriptXOffset);
  حالة HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_OFFSET:     أرجع GET_METRIC_Y (OS2, ySuperscriptYOffset);
  حالة HB_OT_METRICS_TAG_STRIKEOUT_SIZE:              أرجع GET_METRIC_Y (OS2, yStrikeoutSize);
  حالة HB_OT_METRICS_TAG_STRIKEOUT_OFFSET:            أرجع GET_METRIC_Y (OS2, yStrikeoutPosition);
  حالة HB_OT_METRICS_TAG_UNDERLINE_SIZE:              أرجع GET_METRIC_Y (post->table, underlineThickness);
  حالة HB_OT_METRICS_TAG_UNDERLINE_OFFSET:            أرجع GET_METRIC_Y (post->table, underlinePosition);

  /* Private tags */
  حالة _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_OS2:    أرجع GET_METRIC_Y (OS2, sTypoAscender);
  حالة _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_HHEA:   أرجع GET_HEA_METRIC_Y (hhea, ascender);
  حالة _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_OS2:   أرجع GET_METRIC_Y (OS2, sTypoDescender);
  حالة _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_HHEA:  أرجع GET_HEA_METRIC_Y (hhea, descender);
  حالة HB_OT_METRICS_TAG_HORIZONTAL_سطر_GAP_OS2:    أرجع GET_METRIC_Y (OS2, s_Typo_سطر_Gap);
  حالة HB_OT_METRICS_TAG_HORIZONTAL_سطر_GAP_HHEA:   أرجع GET_HEA_METRIC_Y (hhea, سطر_Gap);
#undef GET_HEA_METRIC_Y
#undef GET_HEA_METRIC_X
#undef GET_METRIC_Y
#undef GET_METRIC_X
#undef GET_VAR
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
					  hb_ot_metrics_tag_t  metrics_tag,
					  hb_position_t       *position     /* OUT */)
{
  hb_font_extents_t font_extents;
  hb_codepoint_t glyph;
  hb_glyph_extents_t extents;

  إذا (hb_ot_metrics_get_position (font, metrics_tag, position))
    {
      إذا ((metrics_tag != HB_OT_METRICS_TAG_STRIKEOUT_SIZE &&
           metrics_tag != HB_OT_METRICS_TAG_UNDERLINE_SIZE) ||
          *position != 0)
        أرجع;
    }

  بدل (metrics_tag)
  {
  حالة HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER:
  حالة HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_ASCENT:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.ascender;
    اكسر;

  حالة HB_OT_METRICS_TAG_VERTICAL_ASCENDER:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ascender;
    اكسر;

  حالة HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER:
  حالة HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_DESCENT:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.descender;
    اكسر;

  حالة HB_OT_METRICS_TAG_VERTICAL_DESCENDER:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ascender;
    اكسر;

  حالة HB_OT_METRICS_TAG_HORIZONTAL_سطر_GAP:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.سطر_gap;
    اكسر;

  حالة HB_OT_METRICS_TAG_VERTICAL_سطر_GAP:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.سطر_gap;
    اكسر;

  حالة HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE:
  حالة HB_OT_METRICS_TAG_VERTICAL_CARET_RISE:
    *position = 1;
    اكسر;

  حالة HB_OT_METRICS_TAG_HORIZONTAL_CARET_RUN:
  حالة HB_OT_METRICS_TAG_VERTICAL_CARET_RUN:
    *position = 0;
    اكسر;

  حالة HB_OT_METRICS_TAG_HORIZONTAL_CARET_OFFSET:
  حالة HB_OT_METRICS_TAG_VERTICAL_CARET_OFFSET:
    *position = 0;
    اكسر;

  حالة HB_OT_METRICS_TAG_X_HEIGHT:
    إذا (hb_font_get_nominal_glyph (font, 'x', &glyph) &&
        hb_font_get_glyph_extents (font, glyph, &extents))
      *position = extents.y_bearing;
    وإلا
      *position = font->y_scale / 2;
    اكسر;

  حالة HB_OT_METRICS_TAG_CAP_HEIGHT:
    إذا (hb_font_get_nominal_glyph (font, 'O', &glyph) &&
        hb_font_get_glyph_extents (font, glyph, &extents))
      *position = hb_clamp_to<hb_position_t> ((int64_t) extents.height +
					      2 * (int64_t) extents.y_bearing);
    وإلا
      *position = hb_clamp_to<hb_position_t> ((int64_t) font->y_scale * 2 / 3);
    اكسر;

  حالة HB_OT_METRICS_TAG_STRIKEOUT_SIZE:
  حالة HB_OT_METRICS_TAG_UNDERLINE_SIZE:
    *position = font->y_scale / 18;
    اكسر;

  حالة HB_OT_METRICS_TAG_STRIKEOUT_OFFSET:
    {
      hb_position_t ascender;
      hb_ot_metrics_get_position_with_fallback (font,
                                                HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER,
                                                &ascender);
      *position = ascender / 2;
    }
    اكسر;

  حالة HB_OT_METRICS_TAG_UNDERLINE_OFFSET:
    *position = hb_saturate_neg (font->y_scale) / 18;
    اكسر;

  حالة HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_SIZE:
  حالة HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_SIZE:
    *position = hb_clamp_to<hb_position_t> ((int64_t) font->x_scale * 10 / 12);
    اكسر;

  حالة HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_SIZE:
  حالة HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_SIZE:
    *position = hb_clamp_to<hb_position_t> ((int64_t) font->y_scale * 10 / 12);
    اكسر;

  حالة HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_OFFSET:
  حالة HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_OFFSET:
    *position = 0;
    اكسر;

  حالة HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_OFFSET:
  حالة HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_OFFSET:
    *position = font->y_scale / 5;
    اكسر;

  حالة HB_OT_METRICS_TAG_MAX_قيمة:
  افتراضي:
    *position = 0;
    اكسر;
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
عائم
hb_ot_metrics_get_variation (hb_font_t *font, hb_ot_metrics_tag_t metrics_tag)
{
  أرجع font->face->table.MVAR->get_var (metrics_tag, font->coords, font->رقم_coords);
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
  أرجع font->em_scalef_x (hb_ot_metrics_get_variation (font, metrics_tag));
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
  أرجع font->em_scalef_y (hb_ot_metrics_get_variation (font, metrics_tag));
}
#endif

#endif

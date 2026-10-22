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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ಅನ್ತರ್ಭಾವ "hb-ot-var-mvar-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-gasp-table.hh" // Just so we compile it; unused otherwise.
#ಅನ್ತರ್ಭಾವ "hb-ot-os2-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-post-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-hhea-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-metrics.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-face.hh"


/**
 * SECTION:hb-ot-metrics
 * @title: hb-ot-metrics
 * @short_description: OpenType Metrics
 * @include: hb-ot.h
 *
 * Functions for fetching metrics from fonts.
 **/

ಸ್ಥಿರ ಭಿನ್ನ
_fix_ascender_descender (ಭಿನ್ನ value, hb_ot_metrics_tag_t metrics_tag)
{
  ಯದಿ (metrics_tag == HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER ||
      metrics_tag == HB_OT_METRICS_TAG_VERTICAL_ASCENDER)
    ಪ್ರತ್ಯಾಗಮ fabs ((ದ್ವಿಗುಣ) value);
  ಯದಿ (metrics_tag == HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER ||
      metrics_tag == HB_OT_METRICS_TAG_VERTICAL_DESCENDER)
    ಪ್ರತ್ಯಾಗಮ -fabs ((ದ್ವಿಗುಣ) value);
  ಪ್ರತ್ಯಾಗಮ value;
}

/* The common part of _get_position logic needed on hb-ot-font and here
   to be able to have slim builds without the not always needed parts */
ತಾರ್ಕಿಕ
_hb_ot_metrics_get_position_common (hb_font_t           *font,
				    hb_ot_metrics_tag_t  metrics_tag,
				    hb_position_t       *position     /* OUT.  May be NULL. */)
{
  hb_face_t *face = font->face;
  ವಿಕಲ್ಪ ((unsigned) metrics_tag)
  {
#ifndef HB_NO_VAR
#define GET_VAR face->table.MVAR->get_var (metrics_tag, font->coords, font->ಅಂಕೆ_coords)
#ಅನ್ಯಥಾ
#define GET_VAR .0f
#endif
#define GET_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((ಶೂನ್ಯ) (position && (*position = font->em_scalef_y (_fix_ascender_descender ( \
      face->table.TABLE->ATTR + GET_VAR, metrics_tag)))), ಸತ್ಯ))
#define GET_HEA_METRIC_X(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((ಶೂನ್ಯ) (position && (*position = font->em_scalef_x (_fix_ascender_descender ( \
      HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR, metrics_tag)))), ಸತ್ಯ))
#define GET_HEA_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((ಶೂನ್ಯ) (position && (*position = font->em_scalef_y (_fix_ascender_descender ( \
      HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR, metrics_tag)))), ಸತ್ಯ))

  ಪಕ್ಷ HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER:
    ಪ್ರತ್ಯಾಗಮ (face->table.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, sTypoAscender)) ||
	   GET_HEA_METRIC_Y (hhea, ascender);
  ಪಕ್ಷ HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER:
    ಪ್ರತ್ಯಾಗಮ (face->table.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, sTypoDescender)) ||
	   GET_HEA_METRIC_Y (hhea, descender);
  ಪಕ್ಷ HB_OT_METRICS_TAG_HORIZONTAL_ಸಾಲು_GAP:
    ಪ್ರತ್ಯಾಗಮ (face->table.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, s_Typo_ಸಾಲು_Gap)) ||
	   GET_HEA_METRIC_Y (hhea, ಸಾಲು_Gap);

#ifndef HB_NO_VERTICAL
  ಪಕ್ಷ HB_OT_METRICS_TAG_VERTICAL_ASCENDER:  ಪ್ರತ್ಯಾಗಮ GET_HEA_METRIC_X (vhea, ascender);
  ಪಕ್ಷ HB_OT_METRICS_TAG_VERTICAL_DESCENDER: ಪ್ರತ್ಯಾಗಮ GET_HEA_METRIC_X (vhea, descender);
  ಪಕ್ಷ HB_OT_METRICS_TAG_VERTICAL_ಸಾಲು_GAP:  ಪ್ರತ್ಯಾಗಮ GET_HEA_METRIC_X (vhea, ಸಾಲು_Gap);
#endif

#undef GET_HEA_METRIC_Y
#undef GET_HEA_METRIC_X
#undef GET_METRIC_Y
#undef GET_VAR
  ಸ್ವಭಾವ:                               ಪ್ರತಿಜ್ಞಾ (0); ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }
}

#ifndef HB_NO_METRICS

#ಯದಿ 0
ಸ್ಥಿರ ತಾರ್ಕಿಕ
_get_gasp (hb_face_t *face, ಭಿನ್ನ *result, hb_ot_metrics_tag_t metrics_tag)
{
  ಧ್ರುವ OT::GaspRange& range = face->table.gasp->get_gasp_range (metrics_tag - HB_TAG ('g','s','p','0'));
  ಯದಿ (&range == &Null (OT::GaspRange)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  ಯದಿ (result) *result = range.rangeMaxPPEM + font->face->table.MVAR->get_var (metrics_tag, font->coords, font->ಅಂಕೆ_coords);
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}
#endif

/* Private tags for https://github.com/harfbuzz/harfbuzz/issues/1866 */
#define _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_OS2   HB_TAG ('O','a','s','c')
#define _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_HHEA  HB_TAG ('H','a','s','c')
#define _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_OS2  HB_TAG ('O','d','s','c')
#define _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_HHEA HB_TAG ('H','d','s','c')
#define HB_OT_METRICS_TAG_HORIZONTAL_ಸಾಲು_GAP_OS2   HB_TAG ('O','l','g','p')
#define HB_OT_METRICS_TAG_HORIZONTAL_ಸಾಲು_GAP_HHEA  HB_TAG ('H','l','g','p')

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
  ವಿಕಲ್ಪ ((unsigned) metrics_tag)
  {
  ಪಕ್ಷ HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER:
  ಪಕ್ಷ HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER:
  ಪಕ್ಷ HB_OT_METRICS_TAG_HORIZONTAL_ಸಾಲು_GAP:
  ಪಕ್ಷ HB_OT_METRICS_TAG_VERTICAL_ASCENDER:
  ಪಕ್ಷ HB_OT_METRICS_TAG_VERTICAL_DESCENDER:
  ಪಕ್ಷ HB_OT_METRICS_TAG_VERTICAL_ಸಾಲು_GAP:           ಪ್ರತ್ಯಾಗಮ _hb_ot_metrics_get_position_common (font, metrics_tag, position);
#ifndef HB_NO_VAR
#define GET_VAR hb_ot_metrics_get_variation (font, metrics_tag)
#ಅನ್ಯಥಾ
#define GET_VAR 0
#endif
#define GET_METRIC_X(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((ಶೂನ್ಯ) (position && (*position = font->em_scalef_x (face->table.TABLE->ATTR + GET_VAR))), ಸತ್ಯ))
#define GET_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((ಶೂನ್ಯ) (position && (*position = font->em_scalef_y (face->table.TABLE->ATTR + GET_VAR))), ಸತ್ಯ))
#define GET_HEA_METRIC_X(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((ಶೂನ್ಯ) (position && (*position = font->em_scalef_x (HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR))), ಸತ್ಯ))
#define GET_HEA_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((ಶೂನ್ಯ) (position && (*position = font->em_scalef_y (HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR))), ಸತ್ಯ))
  ಪಕ್ಷ HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_ASCENT:  ಪ್ರತ್ಯಾಗಮ GET_METRIC_Y (OS2, usWinAscent);
  ಪಕ್ಷ HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_DESCENT: ಪ್ರತ್ಯಾಗಮ GET_METRIC_Y (OS2, usWinDescent);

  ಪಕ್ಷ HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE:
  ಪಕ್ಷ HB_OT_METRICS_TAG_HORIZONTAL_CARET_RUN:
  {
    unsigned mult = 1u;

    ಯದಿ (font->slant)
    {
      unsigned rise = HB_DUAL_GET (*face->table.hhea, table->caretSlopeRise);
      unsigned upem = face->get_upem ();
      mult = (rise && rise < upem) ? hb_min (upem / rise, 256u) : 1u;
    }

    ಯದಿ (metrics_tag == HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE)
    {
      ತಾರ್ಕಿಕ ret = GET_HEA_METRIC_Y (hhea, caretSlopeRise);

      ಯದಿ (position)
	*position = hb_clamp_to<hb_position_t> ((int64_t) *position * mult);

      ಪ್ರತ್ಯಾಗಮ ret;
    }
    ಅನ್ಯಥಾ
    {
      hb_position_t rise = 0;

      ಯದಿ (font->slant && position && GET_HEA_METRIC_Y (hhea, caretSlopeRise))
	rise = *position;

      ತಾರ್ಕಿಕ ret = GET_HEA_METRIC_X (hhea, caretSlopeRun);

      ಯದಿ (position)
      {
	*position = hb_clamp_to<hb_position_t> ((int64_t) *position * mult);

	ಯದಿ (font->slant)
	  *position = hb_saturate_add (*position,
				       hb_clamp_to<hb_position_t> (roundf (mult * font->slant_xy * rise)));
      }

      ಪ್ರತ್ಯಾಗಮ ret;
    }
  }
  ಪಕ್ಷ HB_OT_METRICS_TAG_HORIZONTAL_CARET_OFFSET:     ಪ್ರತ್ಯಾಗಮ GET_HEA_METRIC_X (hhea, caretOffset);

#ifndef HB_NO_VERTICAL
  ಪಕ್ಷ HB_OT_METRICS_TAG_VERTICAL_CARET_RISE:         ಪ್ರತ್ಯಾಗಮ GET_HEA_METRIC_X (vhea, caretSlopeRise);
  ಪಕ್ಷ HB_OT_METRICS_TAG_VERTICAL_CARET_RUN:          ಪ್ರತ್ಯಾಗಮ GET_HEA_METRIC_Y (vhea, caretSlopeRun);
  ಪಕ್ಷ HB_OT_METRICS_TAG_VERTICAL_CARET_OFFSET:       ಪ್ರತ್ಯಾಗಮ GET_HEA_METRIC_Y (vhea, caretOffset);
#endif
  ಪಕ್ಷ HB_OT_METRICS_TAG_X_HEIGHT:                    ಪ್ರತ್ಯಾಗಮ GET_METRIC_Y (OS2->v2 (), sxHeight);
  ಪಕ್ಷ HB_OT_METRICS_TAG_CAP_HEIGHT:                  ಪ್ರತ್ಯಾಗಮ GET_METRIC_Y (OS2->v2 (), sCapHeight);
  ಪಕ್ಷ HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_SIZE:         ಪ್ರತ್ಯಾಗಮ GET_METRIC_X (OS2, ySubscriptXSize);
  ಪಕ್ಷ HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_SIZE:         ಪ್ರತ್ಯಾಗಮ GET_METRIC_Y (OS2, ySubscriptYSize);
  ಪಕ್ಷ HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_OFFSET:       ಪ್ರತ್ಯಾಗಮ GET_METRIC_X (OS2, ySubscriptXOffset);
  ಪಕ್ಷ HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_OFFSET:       ಪ್ರತ್ಯಾಗಮ GET_METRIC_Y (OS2, ySubscriptYOffset);
  ಪಕ್ಷ HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_SIZE:       ಪ್ರತ್ಯಾಗಮ GET_METRIC_X (OS2, ySuperscriptXSize);
  ಪಕ್ಷ HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_SIZE:       ಪ್ರತ್ಯಾಗಮ GET_METRIC_Y (OS2, ySuperscriptYSize);
  ಪಕ್ಷ HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_OFFSET:     ಪ್ರತ್ಯಾಗಮ GET_METRIC_X (OS2, ySuperscriptXOffset);
  ಪಕ್ಷ HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_OFFSET:     ಪ್ರತ್ಯಾಗಮ GET_METRIC_Y (OS2, ySuperscriptYOffset);
  ಪಕ್ಷ HB_OT_METRICS_TAG_STRIKEOUT_SIZE:              ಪ್ರತ್ಯಾಗಮ GET_METRIC_Y (OS2, yStrikeoutSize);
  ಪಕ್ಷ HB_OT_METRICS_TAG_STRIKEOUT_OFFSET:            ಪ್ರತ್ಯಾಗಮ GET_METRIC_Y (OS2, yStrikeoutPosition);
  ಪಕ್ಷ HB_OT_METRICS_TAG_UNDERLINE_SIZE:              ಪ್ರತ್ಯಾಗಮ GET_METRIC_Y (post->table, underlineThickness);
  ಪಕ್ಷ HB_OT_METRICS_TAG_UNDERLINE_OFFSET:            ಪ್ರತ್ಯಾಗಮ GET_METRIC_Y (post->table, underlinePosition);

  /* Private tags */
  ಪಕ್ಷ _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_OS2:    ಪ್ರತ್ಯಾಗಮ GET_METRIC_Y (OS2, sTypoAscender);
  ಪಕ್ಷ _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_HHEA:   ಪ್ರತ್ಯಾಗಮ GET_HEA_METRIC_Y (hhea, ascender);
  ಪಕ್ಷ _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_OS2:   ಪ್ರತ್ಯಾಗಮ GET_METRIC_Y (OS2, sTypoDescender);
  ಪಕ್ಷ _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_HHEA:  ಪ್ರತ್ಯಾಗಮ GET_HEA_METRIC_Y (hhea, descender);
  ಪಕ್ಷ HB_OT_METRICS_TAG_HORIZONTAL_ಸಾಲು_GAP_OS2:    ಪ್ರತ್ಯಾಗಮ GET_METRIC_Y (OS2, s_Typo_ಸಾಲು_Gap);
  ಪಕ್ಷ HB_OT_METRICS_TAG_HORIZONTAL_ಸಾಲು_GAP_HHEA:   ಪ್ರತ್ಯಾಗಮ GET_HEA_METRIC_Y (hhea, ಸಾಲು_Gap);
#undef GET_HEA_METRIC_Y
#undef GET_HEA_METRIC_X
#undef GET_METRIC_Y
#undef GET_METRIC_X
#undef GET_VAR
  ಸ್ವಭಾವ:                                        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
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
ಶೂನ್ಯ
hb_ot_metrics_get_position_with_fallback (hb_font_t           *font,
					  hb_ot_metrics_tag_t  metrics_tag,
					  hb_position_t       *position     /* OUT */)
{
  hb_font_extents_t font_extents;
  hb_codepoint_t glyph;
  hb_glyph_extents_t extents;

  ಯದಿ (hb_ot_metrics_get_position (font, metrics_tag, position))
    {
      ಯದಿ ((metrics_tag != HB_OT_METRICS_TAG_STRIKEOUT_SIZE &&
           metrics_tag != HB_OT_METRICS_TAG_UNDERLINE_SIZE) ||
          *position != 0)
        ಪ್ರತ್ಯಾಗಮ;
    }

  ವಿಕಲ್ಪ (metrics_tag)
  {
  ಪಕ್ಷ HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER:
  ಪಕ್ಷ HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_ASCENT:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.ascender;
    ಭಙ್ಗ;

  ಪಕ್ಷ HB_OT_METRICS_TAG_VERTICAL_ASCENDER:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ascender;
    ಭಙ್ಗ;

  ಪಕ್ಷ HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER:
  ಪಕ್ಷ HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_DESCENT:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.descender;
    ಭಙ್ಗ;

  ಪಕ್ಷ HB_OT_METRICS_TAG_VERTICAL_DESCENDER:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ascender;
    ಭಙ್ಗ;

  ಪಕ್ಷ HB_OT_METRICS_TAG_HORIZONTAL_ಸಾಲು_GAP:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.ಸಾಲು_gap;
    ಭಙ್ಗ;

  ಪಕ್ಷ HB_OT_METRICS_TAG_VERTICAL_ಸಾಲು_GAP:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ಸಾಲು_gap;
    ಭಙ್ಗ;

  ಪಕ್ಷ HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE:
  ಪಕ್ಷ HB_OT_METRICS_TAG_VERTICAL_CARET_RISE:
    *position = 1;
    ಭಙ್ಗ;

  ಪಕ್ಷ HB_OT_METRICS_TAG_HORIZONTAL_CARET_RUN:
  ಪಕ್ಷ HB_OT_METRICS_TAG_VERTICAL_CARET_RUN:
    *position = 0;
    ಭಙ್ಗ;

  ಪಕ್ಷ HB_OT_METRICS_TAG_HORIZONTAL_CARET_OFFSET:
  ಪಕ್ಷ HB_OT_METRICS_TAG_VERTICAL_CARET_OFFSET:
    *position = 0;
    ಭಙ್ಗ;

  ಪಕ್ಷ HB_OT_METRICS_TAG_X_HEIGHT:
    ಯದಿ (hb_font_get_nominal_glyph (font, 'x', &glyph) &&
        hb_font_get_glyph_extents (font, glyph, &extents))
      *position = extents.y_bearing;
    ಅನ್ಯಥಾ
      *position = font->y_scale / 2;
    ಭಙ್ಗ;

  ಪಕ್ಷ HB_OT_METRICS_TAG_CAP_HEIGHT:
    ಯದಿ (hb_font_get_nominal_glyph (font, 'O', &glyph) &&
        hb_font_get_glyph_extents (font, glyph, &extents))
      *position = hb_clamp_to<hb_position_t> ((int64_t) extents.height +
					      2 * (int64_t) extents.y_bearing);
    ಅನ್ಯಥಾ
      *position = hb_clamp_to<hb_position_t> ((int64_t) font->y_scale * 2 / 3);
    ಭಙ್ಗ;

  ಪಕ್ಷ HB_OT_METRICS_TAG_STRIKEOUT_SIZE:
  ಪಕ್ಷ HB_OT_METRICS_TAG_UNDERLINE_SIZE:
    *position = font->y_scale / 18;
    ಭಙ್ಗ;

  ಪಕ್ಷ HB_OT_METRICS_TAG_STRIKEOUT_OFFSET:
    {
      hb_position_t ascender;
      hb_ot_metrics_get_position_with_fallback (font,
                                                HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER,
                                                &ascender);
      *position = ascender / 2;
    }
    ಭಙ್ಗ;

  ಪಕ್ಷ HB_OT_METRICS_TAG_UNDERLINE_OFFSET:
    *position = hb_saturate_neg (font->y_scale) / 18;
    ಭಙ್ಗ;

  ಪಕ್ಷ HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_SIZE:
  ಪಕ್ಷ HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_SIZE:
    *position = hb_clamp_to<hb_position_t> ((int64_t) font->x_scale * 10 / 12);
    ಭಙ್ಗ;

  ಪಕ್ಷ HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_SIZE:
  ಪಕ್ಷ HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_SIZE:
    *position = hb_clamp_to<hb_position_t> ((int64_t) font->y_scale * 10 / 12);
    ಭಙ್ಗ;

  ಪಕ್ಷ HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_OFFSET:
  ಪಕ್ಷ HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_OFFSET:
    *position = 0;
    ಭಙ್ಗ;

  ಪಕ್ಷ HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_OFFSET:
  ಪಕ್ಷ HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_OFFSET:
    *position = font->y_scale / 5;
    ಭಙ್ಗ;

  ಪಕ್ಷ HB_OT_METRICS_TAG_MAX_ಮೌಲ್ಯ:
  ಸ್ವಭಾವ:
    *position = 0;
    ಭಙ್ಗ;
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
ಭಿನ್ನ
hb_ot_metrics_get_variation (hb_font_t *font, hb_ot_metrics_tag_t metrics_tag)
{
  ಪ್ರತ್ಯಾಗಮ font->face->table.MVAR->get_var (metrics_tag, font->coords, font->ಅಂಕೆ_coords);
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
  ಪ್ರತ್ಯಾಗಮ font->em_scalef_x (hb_ot_metrics_get_variation (font, metrics_tag));
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
  ಪ್ರತ್ಯಾಗಮ font->em_scalef_y (hb_ot_metrics_get_variation (font, metrics_tag));
}
#endif

#endif

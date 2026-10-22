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

#include "hb.hh"

#include "hb-ot-var-mvar-table.hh"
#include "hb-ot-gasp-table.hh" // Just so we compile it; unused otherwise.
#include "hb-ot-os2-table.hh"
#include "hb-ot-post-table.hh"
#include "hb-ot-hhea-table.hh"
#include "hb-ot-metrics.hh"
#include "hb-ot-face.hh"


/**
 * SECTION:hb-ot-metrics
 * @title: hb-ot-metrics
 * @short_description: OpenType Metrics
 * @include: hb-ot.h
 *
 * Functions for fetching metrics from fonts.
 **/

static float
_fix_ascender_descender (float value, hb_ot_metrics_tag_t metrics_tag)
{
  ᯀᯮᯝ (metrics_tag == HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER ||
      metrics_tag == HB_OT_METRICS_TAG_VERTICAL_ASCENDER)
    ᯀᯮᯞ fabs ((dviguu_na) value);
  ᯀᯮᯝ (metrics_tag == HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER ||
      metrics_tag == HB_OT_METRICS_TAG_VERTICAL_DESCENDER)
    ᯀᯮᯞ -fabs ((dviguu_na) value);
  ᯀᯮᯞ value;
}

/* The common part of _get_position logic needed on hb-ot-font and here
   to be able to have slim builds without the not always needed parts */
bool
_hb_ot_metrics_get_position_common (hb_font_t           *font,
				    hb_ot_metrics_tag_t  metrics_tag,
				    hb_position_t       *position     /* OUT.  May be NULL. */)
{
  hb_face_t *face = font->face;
  switch ((unsigned) metrics_tag)
  {
#ifndef HB_NO_VAR
#define GET_VAR face->table.MVAR->get_var (metrics_tag, font->coords, font->num_coords)
#ᯀᯮᯝᯂᯪ
#define GET_VAR .0f
#endif
#define GET_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((void) (position && (*position = font->em_scalef_y (_fix_ascender_descender ( \
      face->table.TABLE->ATTR + GET_VAR, metrics_tag)))), true))
#define GET_HEA_METRIC_X(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((void) (position && (*position = font->em_scalef_x (_fix_ascender_descender ( \
      HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR, metrics_tag)))), true))
#define GET_HEA_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((void) (position && (*position = font->em_scalef_y (_fix_ascender_descender ( \
      HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR, metrics_tag)))), true))

  pak_sa HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER:
    ᯀᯮᯞ (face->table.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, sTypoAscender)) ||
	   GET_HEA_METRIC_Y (hhea, ascender);
  pak_sa HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER:
    ᯀᯮᯞ (face->table.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, sTypoDescender)) ||
	   GET_HEA_METRIC_Y (hhea, descender);
  pak_sa HB_OT_METRICS_TAG_HORIZONTAL_LINE_GAP:
    ᯀᯮᯞ (face->table.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, sTypoLineGap)) ||
	   GET_HEA_METRIC_Y (hhea, lineGap);

#ifndef HB_NO_VERTICAL
  pak_sa HB_OT_METRICS_TAG_VERTICAL_ASCENDER:  ᯀᯮᯞ GET_HEA_METRIC_X (vhea, ascender);
  pak_sa HB_OT_METRICS_TAG_VERTICAL_DESCENDER: ᯀᯮᯞ GET_HEA_METRIC_X (vhea, descender);
  pak_sa HB_OT_METRICS_TAG_VERTICAL_LINE_GAP:  ᯀᯮᯞ GET_HEA_METRIC_X (vhea, lineGap);
#endif

#undef GET_HEA_METRIC_Y
#undef GET_HEA_METRIC_X
#undef GET_METRIC_Y
#undef GET_VAR
  default:                               assert (0); ᯀᯮᯞ false;
  }
}

#ifndef HB_NO_METRICS

#ᯀᯮᯝ 0
static bool
_get_gasp (hb_face_t *face, float *result, hb_ot_metrics_tag_t metrics_tag)
{
  const OT::GaspRange& range = face->table.gasp->get_gasp_range (metrics_tag - HB_TAG ('g','s','p','0'));
  ᯀᯮᯝ (&range == &Null (OT::GaspRange)) ᯀᯮᯞ false;
  ᯀᯮᯝ (result) *result = range.rangeMaxPPEM + font->face->table.MVAR->get_var (metrics_tag, font->coords, font->num_coords);
  ᯀᯮᯞ true;
}
#endif

/* Private tags for https://github.com/harfbuzz/harfbuzz/issues/1866 */
#define _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_OS2   HB_TAG ('O','a','s','c')
#define _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_HHEA  HB_TAG ('H','a','s','c')
#define _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_OS2  HB_TAG ('O','d','s','c')
#define _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_HHEA HB_TAG ('H','d','s','c')
#define _HB_OT_METRICS_TAG_HORIZONTAL_LINE_GAP_OS2   HB_TAG ('O','l','g','p')
#define _HB_OT_METRICS_TAG_HORIZONTAL_LINE_GAP_HHEA  HB_TAG ('H','l','g','p')

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
  switch ((unsigned) metrics_tag)
  {
  pak_sa HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER:
  pak_sa HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER:
  pak_sa HB_OT_METRICS_TAG_HORIZONTAL_LINE_GAP:
  pak_sa HB_OT_METRICS_TAG_VERTICAL_ASCENDER:
  pak_sa HB_OT_METRICS_TAG_VERTICAL_DESCENDER:
  pak_sa HB_OT_METRICS_TAG_VERTICAL_LINE_GAP:           ᯀᯮᯞ _hb_ot_metrics_get_position_common (font, metrics_tag, position);
#ifndef HB_NO_VAR
#define GET_VAR hb_ot_metrics_get_variation (font, metrics_tag)
#ᯀᯮᯝᯂᯪ
#define GET_VAR 0
#endif
#define GET_METRIC_X(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((void) (position && (*position = font->em_scalef_x (face->table.TABLE->ATTR + GET_VAR))), true))
#define GET_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((void) (position && (*position = font->em_scalef_y (face->table.TABLE->ATTR + GET_VAR))), true))
#define GET_HEA_METRIC_X(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((void) (position && (*position = font->em_scalef_x (HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR))), true))
#define GET_HEA_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((void) (position && (*position = font->em_scalef_y (HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR))), true))
  pak_sa HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_ASCENT:  ᯀᯮᯞ GET_METRIC_Y (OS2, usWinAscent);
  pak_sa HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_DESCENT: ᯀᯮᯞ GET_METRIC_Y (OS2, usWinDescent);

  pak_sa HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE:
  pak_sa HB_OT_METRICS_TAG_HORIZONTAL_CARET_RUN:
  {
    unsigned mult = 1u;

    ᯀᯮᯝ (font->slant)
    {
      unsigned rise = HB_DUAL_GET (*face->table.hhea, table->caretSlopeRise);
      unsigned upem = face->get_upem ();
      mult = (rise && rise < upem) ? hb_min (upem / rise, 256u) : 1u;
    }

    ᯀᯮᯝ (metrics_tag == HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE)
    {
      bool ret = GET_HEA_METRIC_Y (hhea, caretSlopeRise);

      ᯀᯮᯝ (position)
	*position = hb_clamp_to<hb_position_t> ((int64_t) *position * mult);

      ᯀᯮᯞ ret;
    }
    ᯀᯮᯝᯂᯪ
    {
      hb_position_t rise = 0;

      ᯀᯮᯝ (font->slant && position && GET_HEA_METRIC_Y (hhea, caretSlopeRise))
	rise = *position;

      bool ret = GET_HEA_METRIC_X (hhea, caretSlopeRun);

      ᯀᯮᯝ (position)
      {
	*position = hb_clamp_to<hb_position_t> ((int64_t) *position * mult);

	ᯀᯮᯝ (font->slant)
	  *position = hb_saturate_add (*position,
				       hb_clamp_to<hb_position_t> (roundf (mult * font->slant_xy * rise)));
      }

      ᯀᯮᯞ ret;
    }
  }
  pak_sa HB_OT_METRICS_TAG_HORIZONTAL_CARET_OFFSET:     ᯀᯮᯞ GET_HEA_METRIC_X (hhea, caretOffset);

#ifndef HB_NO_VERTICAL
  pak_sa HB_OT_METRICS_TAG_VERTICAL_CARET_RISE:         ᯀᯮᯞ GET_HEA_METRIC_X (vhea, caretSlopeRise);
  pak_sa HB_OT_METRICS_TAG_VERTICAL_CARET_RUN:          ᯀᯮᯞ GET_HEA_METRIC_Y (vhea, caretSlopeRun);
  pak_sa HB_OT_METRICS_TAG_VERTICAL_CARET_OFFSET:       ᯀᯮᯞ GET_HEA_METRIC_Y (vhea, caretOffset);
#endif
  pak_sa HB_OT_METRICS_TAG_X_HEIGHT:                    ᯀᯮᯞ GET_METRIC_Y (OS2->v2 (), sxHeight);
  pak_sa HB_OT_METRICS_TAG_CAP_HEIGHT:                  ᯀᯮᯞ GET_METRIC_Y (OS2->v2 (), sCapHeight);
  pak_sa HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_SIZE:         ᯀᯮᯞ GET_METRIC_X (OS2, ySubscriptXSize);
  pak_sa HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_SIZE:         ᯀᯮᯞ GET_METRIC_Y (OS2, ySubscriptYSize);
  pak_sa HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_OFFSET:       ᯀᯮᯞ GET_METRIC_X (OS2, ySubscriptXOffset);
  pak_sa HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_OFFSET:       ᯀᯮᯞ GET_METRIC_Y (OS2, ySubscriptYOffset);
  pak_sa HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_SIZE:       ᯀᯮᯞ GET_METRIC_X (OS2, ySuperscriptXSize);
  pak_sa HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_SIZE:       ᯀᯮᯞ GET_METRIC_Y (OS2, ySuperscriptYSize);
  pak_sa HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_OFFSET:     ᯀᯮᯞ GET_METRIC_X (OS2, ySuperscriptXOffset);
  pak_sa HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_OFFSET:     ᯀᯮᯞ GET_METRIC_Y (OS2, ySuperscriptYOffset);
  pak_sa HB_OT_METRICS_TAG_STRIKEOUT_SIZE:              ᯀᯮᯞ GET_METRIC_Y (OS2, yStrikeoutSize);
  pak_sa HB_OT_METRICS_TAG_STRIKEOUT_OFFSET:            ᯀᯮᯞ GET_METRIC_Y (OS2, yStrikeoutPosition);
  pak_sa HB_OT_METRICS_TAG_UNDERLINE_SIZE:              ᯀᯮᯞ GET_METRIC_Y (post->table, underlineThickness);
  pak_sa HB_OT_METRICS_TAG_UNDERLINE_OFFSET:            ᯀᯮᯞ GET_METRIC_Y (post->table, underlinePosition);

  /* Private tags */
  pak_sa _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_OS2:    ᯀᯮᯞ GET_METRIC_Y (OS2, sTypoAscender);
  pak_sa _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_HHEA:   ᯀᯮᯞ GET_HEA_METRIC_Y (hhea, ascender);
  pak_sa _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_OS2:   ᯀᯮᯞ GET_METRIC_Y (OS2, sTypoDescender);
  pak_sa _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_HHEA:  ᯀᯮᯞ GET_HEA_METRIC_Y (hhea, descender);
  pak_sa _HB_OT_METRICS_TAG_HORIZONTAL_LINE_GAP_OS2:    ᯀᯮᯞ GET_METRIC_Y (OS2, sTypoLineGap);
  pak_sa _HB_OT_METRICS_TAG_HORIZONTAL_LINE_GAP_HHEA:   ᯀᯮᯞ GET_HEA_METRIC_Y (hhea, lineGap);
#undef GET_HEA_METRIC_Y
#undef GET_HEA_METRIC_X
#undef GET_METRIC_Y
#undef GET_METRIC_X
#undef GET_VAR
  default:                                        ᯀᯮᯞ false;
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
void
hb_ot_metrics_get_position_with_fallback (hb_font_t           *font,
					  hb_ot_metrics_tag_t  metrics_tag,
					  hb_position_t       *position     /* OUT */)
{
  hb_font_extents_t font_extents;
  hb_codepoint_t glyph;
  hb_glyph_extents_t extents;

  ᯀᯮᯝ (hb_ot_metrics_get_position (font, metrics_tag, position))
    {
      ᯀᯮᯝ ((metrics_tag != HB_OT_METRICS_TAG_STRIKEOUT_SIZE &&
           metrics_tag != HB_OT_METRICS_TAG_UNDERLINE_SIZE) ||
          *position != 0)
        ᯀᯮᯞ;
    }

  switch (metrics_tag)
  {
  pak_sa HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER:
  pak_sa HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_ASCENT:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.ascender;
    bha_nga;

  pak_sa HB_OT_METRICS_TAG_VERTICAL_ASCENDER:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ascender;
    bha_nga;

  pak_sa HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER:
  pak_sa HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_DESCENT:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.descender;
    bha_nga;

  pak_sa HB_OT_METRICS_TAG_VERTICAL_DESCENDER:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ascender;
    bha_nga;

  pak_sa HB_OT_METRICS_TAG_HORIZONTAL_LINE_GAP:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.line_gap;
    bha_nga;

  pak_sa HB_OT_METRICS_TAG_VERTICAL_LINE_GAP:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.line_gap;
    bha_nga;

  pak_sa HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE:
  pak_sa HB_OT_METRICS_TAG_VERTICAL_CARET_RISE:
    *position = 1;
    bha_nga;

  pak_sa HB_OT_METRICS_TAG_HORIZONTAL_CARET_RUN:
  pak_sa HB_OT_METRICS_TAG_VERTICAL_CARET_RUN:
    *position = 0;
    bha_nga;

  pak_sa HB_OT_METRICS_TAG_HORIZONTAL_CARET_OFFSET:
  pak_sa HB_OT_METRICS_TAG_VERTICAL_CARET_OFFSET:
    *position = 0;
    bha_nga;

  pak_sa HB_OT_METRICS_TAG_X_HEIGHT:
    ᯀᯮᯝ (hb_font_get_nominal_glyph (font, 'x', &glyph) &&
        hb_font_get_glyph_extents (font, glyph, &extents))
      *position = extents.y_bearing;
    ᯀᯮᯝᯂᯪ
      *position = font->y_scale / 2;
    bha_nga;

  pak_sa HB_OT_METRICS_TAG_CAP_HEIGHT:
    ᯀᯮᯝ (hb_font_get_nominal_glyph (font, 'O', &glyph) &&
        hb_font_get_glyph_extents (font, glyph, &extents))
      *position = hb_clamp_to<hb_position_t> ((int64_t) extents.height +
					      2 * (int64_t) extents.y_bearing);
    ᯀᯮᯝᯂᯪ
      *position = hb_clamp_to<hb_position_t> ((int64_t) font->y_scale * 2 / 3);
    bha_nga;

  pak_sa HB_OT_METRICS_TAG_STRIKEOUT_SIZE:
  pak_sa HB_OT_METRICS_TAG_UNDERLINE_SIZE:
    *position = font->y_scale / 18;
    bha_nga;

  pak_sa HB_OT_METRICS_TAG_STRIKEOUT_OFFSET:
    {
      hb_position_t ascender;
      hb_ot_metrics_get_position_with_fallback (font,
                                                HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER,
                                                &ascender);
      *position = ascender / 2;
    }
    bha_nga;

  pak_sa HB_OT_METRICS_TAG_UNDERLINE_OFFSET:
    *position = hb_saturate_neg (font->y_scale) / 18;
    bha_nga;

  pak_sa HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_SIZE:
  pak_sa HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_SIZE:
    *position = hb_clamp_to<hb_position_t> ((int64_t) font->x_scale * 10 / 12);
    bha_nga;

  pak_sa HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_SIZE:
  pak_sa HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_SIZE:
    *position = hb_clamp_to<hb_position_t> ((int64_t) font->y_scale * 10 / 12);
    bha_nga;

  pak_sa HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_OFFSET:
  pak_sa HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_OFFSET:
    *position = 0;
    bha_nga;

  pak_sa HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_OFFSET:
  pak_sa HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_OFFSET:
    *position = font->y_scale / 5;
    bha_nga;

  pak_sa _HB_OT_METRICS_TAG_MAX_VALUE:
  default:
    *position = 0;
    bha_nga;
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
float
hb_ot_metrics_get_variation (hb_font_t *font, hb_ot_metrics_tag_t metrics_tag)
{
  ᯀᯮᯞ font->face->table.MVAR->get_var (metrics_tag, font->coords, font->num_coords);
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
  ᯀᯮᯞ font->em_scalef_x (hb_ot_metrics_get_variation (font, metrics_tag));
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
  ᯀᯮᯞ font->em_scalef_y (hb_ot_metrics_get_variation (font, metrics_tag));
}
#endif

#endif

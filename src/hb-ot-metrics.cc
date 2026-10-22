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

#অন্তর্ভাৱ "hb.hh"

#অন্তর্ভাৱ "hb-ot-var-mvar-table.hh"
#অন্তর্ভাৱ "hb-ot-gasp-table.hh" // Just so we compile it; unused otherwise.
#অন্তর্ভাৱ "hb-ot-os2-table.hh"
#অন্তর্ভাৱ "hb-ot-post-table.hh"
#অন্তর্ভাৱ "hb-ot-hhea-table.hh"
#অন্তর্ভাৱ "hb-ot-metrics.hh"
#অন্তর্ভাৱ "hb-ot-face.hh"


/**
 * SECTION:hb-ot-metrics
 * @title: hb-ot-metrics
 * @short_description: OpenType Metrics
 * @include: hb-ot.h
 *
 * Functions for fetching metrics from fonts.
 **/

স্থির ভিন্ন
_fix_ascender_descender (ভিন্ন value, hb_ot_metrics_tag_t metrics_tag)
{
  যদি (metrics_tag == HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER ||
      metrics_tag == HB_OT_METRICS_TAG_VERTICAL_ASCENDER)
    ঘূৰাই fabs ((দ্ৱিগুণ) value);
  যদি (metrics_tag == HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER ||
      metrics_tag == HB_OT_METRICS_TAG_VERTICAL_DESCENDER)
    ঘূৰাই -fabs ((দ্ৱিগুণ) value);
  ঘূৰাই value;
}

/* The common part of _get_position logic needed on hb-ot-font and here
   to be able to have slim builds without the not always needed parts */
তার্কিক
_hb_ot_metrics_get_position_common (hb_font_t           *font,
				    hb_ot_metrics_tag_t  metrics_tag,
				    hb_position_t       *position     /* OUT.  May be NULL. */)
{
  hb_face_t *face = font->face;
  ৱিকল্প ((unsigned) metrics_tag)
  {
#ifndef HB_NO_VAR
#define GET_VAR face->table.MVAR->get_var (metrics_tag, font->coords, font->অঙ্ক_coords)
#অন্যথা
#define GET_VAR .0f
#endif
#define GET_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((শূন্য) (position && (*position = font->em_scalef_y (_fix_ascender_descender ( \
      face->table.TABLE->ATTR + GET_VAR, metrics_tag)))), সঁচা))
#define GET_HEA_METRIC_X(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((শূন্য) (position && (*position = font->em_scalef_x (_fix_ascender_descender ( \
      HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR, metrics_tag)))), সঁচা))
#define GET_HEA_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((শূন্য) (position && (*position = font->em_scalef_y (_fix_ascender_descender ( \
      HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR, metrics_tag)))), সঁচা))

  পক্ষ HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER:
    ঘূৰাই (face->table.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, sTypoAscender)) ||
	   GET_HEA_METRIC_Y (hhea, ascender);
  পক্ষ HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER:
    ঘূৰাই (face->table.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, sTypoDescender)) ||
	   GET_HEA_METRIC_Y (hhea, descender);
  পক্ষ HB_OT_METRICS_TAG_HORIZONTAL_সাৰি_GAP:
    ঘূৰাই (face->table.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, s_Typo_সাৰি_Gap)) ||
	   GET_HEA_METRIC_Y (hhea, সাৰি_Gap);

#ifndef HB_NO_VERTICAL
  পক্ষ HB_OT_METRICS_TAG_VERTICAL_ASCENDER:  ঘূৰাই GET_HEA_METRIC_X (vhea, ascender);
  পক্ষ HB_OT_METRICS_TAG_VERTICAL_DESCENDER: ঘূৰাই GET_HEA_METRIC_X (vhea, descender);
  পক্ষ HB_OT_METRICS_TAG_VERTICAL_সাৰি_GAP:  ঘূৰাই GET_HEA_METRIC_X (vhea, সাৰি_Gap);
#endif

#undef GET_HEA_METRIC_Y
#undef GET_HEA_METRIC_X
#undef GET_METRIC_Y
#undef GET_VAR
  স্ৱভাৱ:                               প্রতিজ্ঞা (0); ঘূৰাই মিছা;
  }
}

#ifndef HB_NO_METRICS

#যদি 0
স্থির তার্কিক
_get_gasp (hb_face_t *face, ভিন্ন *result, hb_ot_metrics_tag_t metrics_tag)
{
  ধ্রুৱ OT::GaspRange& range = face->table.gasp->get_gasp_range (metrics_tag - HB_TAG ('g','s','p','0'));
  যদি (&range == &Null (OT::GaspRange)) ঘূৰাই মিছা;
  যদি (result) *result = range.rangeMaxPPEM + font->face->table.MVAR->get_var (metrics_tag, font->coords, font->অঙ্ক_coords);
  ঘূৰাই সঁচা;
}
#endif

/* Private tags for https://github.com/harfbuzz/harfbuzz/issues/1866 */
#define _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_OS2   HB_TAG ('O','a','s','c')
#define _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_HHEA  HB_TAG ('H','a','s','c')
#define _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_OS2  HB_TAG ('O','d','s','c')
#define _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_HHEA HB_TAG ('H','d','s','c')
#define HB_OT_METRICS_TAG_HORIZONTAL_সাৰি_GAP_OS2   HB_TAG ('O','l','g','p')
#define HB_OT_METRICS_TAG_HORIZONTAL_সাৰি_GAP_HHEA  HB_TAG ('H','l','g','p')

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
  ৱিকল্প ((unsigned) metrics_tag)
  {
  পক্ষ HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER:
  পক্ষ HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER:
  পক্ষ HB_OT_METRICS_TAG_HORIZONTAL_সাৰি_GAP:
  পক্ষ HB_OT_METRICS_TAG_VERTICAL_ASCENDER:
  পক্ষ HB_OT_METRICS_TAG_VERTICAL_DESCENDER:
  পক্ষ HB_OT_METRICS_TAG_VERTICAL_সাৰি_GAP:           ঘূৰাই _hb_ot_metrics_get_position_common (font, metrics_tag, position);
#ifndef HB_NO_VAR
#define GET_VAR hb_ot_metrics_get_variation (font, metrics_tag)
#অন্যথা
#define GET_VAR 0
#endif
#define GET_METRIC_X(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((শূন্য) (position && (*position = font->em_scalef_x (face->table.TABLE->ATTR + GET_VAR))), সঁচা))
#define GET_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((শূন্য) (position && (*position = font->em_scalef_y (face->table.TABLE->ATTR + GET_VAR))), সঁচা))
#define GET_HEA_METRIC_X(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((শূন্য) (position && (*position = font->em_scalef_x (HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR))), সঁচা))
#define GET_HEA_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((শূন্য) (position && (*position = font->em_scalef_y (HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR))), সঁচা))
  পক্ষ HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_ASCENT:  ঘূৰাই GET_METRIC_Y (OS2, usWinAscent);
  পক্ষ HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_DESCENT: ঘূৰাই GET_METRIC_Y (OS2, usWinDescent);

  পক্ষ HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE:
  পক্ষ HB_OT_METRICS_TAG_HORIZONTAL_CARET_RUN:
  {
    unsigned mult = 1u;

    যদি (font->slant)
    {
      unsigned rise = HB_DUAL_GET (*face->table.hhea, table->caretSlopeRise);
      unsigned upem = face->get_upem ();
      mult = (rise && rise < upem) ? hb_min (upem / rise, 256u) : 1u;
    }

    যদি (metrics_tag == HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE)
    {
      তার্কিক ret = GET_HEA_METRIC_Y (hhea, caretSlopeRise);

      যদি (position)
	*position = hb_clamp_to<hb_position_t> ((int64_t) *position * mult);

      ঘূৰাই ret;
    }
    অন্যথা
    {
      hb_position_t rise = 0;

      যদি (font->slant && position && GET_HEA_METRIC_Y (hhea, caretSlopeRise))
	rise = *position;

      তার্কিক ret = GET_HEA_METRIC_X (hhea, caretSlopeRun);

      যদি (position)
      {
	*position = hb_clamp_to<hb_position_t> ((int64_t) *position * mult);

	যদি (font->slant)
	  *position = hb_saturate_add (*position,
				       hb_clamp_to<hb_position_t> (roundf (mult * font->slant_xy * rise)));
      }

      ঘূৰাই ret;
    }
  }
  পক্ষ HB_OT_METRICS_TAG_HORIZONTAL_CARET_OFFSET:     ঘূৰাই GET_HEA_METRIC_X (hhea, caretOffset);

#ifndef HB_NO_VERTICAL
  পক্ষ HB_OT_METRICS_TAG_VERTICAL_CARET_RISE:         ঘূৰাই GET_HEA_METRIC_X (vhea, caretSlopeRise);
  পক্ষ HB_OT_METRICS_TAG_VERTICAL_CARET_RUN:          ঘূৰাই GET_HEA_METRIC_Y (vhea, caretSlopeRun);
  পক্ষ HB_OT_METRICS_TAG_VERTICAL_CARET_OFFSET:       ঘূৰাই GET_HEA_METRIC_Y (vhea, caretOffset);
#endif
  পক্ষ HB_OT_METRICS_TAG_X_HEIGHT:                    ঘূৰাই GET_METRIC_Y (OS2->v2 (), sxHeight);
  পক্ষ HB_OT_METRICS_TAG_CAP_HEIGHT:                  ঘূৰাই GET_METRIC_Y (OS2->v2 (), sCapHeight);
  পক্ষ HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_SIZE:         ঘূৰাই GET_METRIC_X (OS2, ySubscriptXSize);
  পক্ষ HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_SIZE:         ঘূৰাই GET_METRIC_Y (OS2, ySubscriptYSize);
  পক্ষ HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_OFFSET:       ঘূৰাই GET_METRIC_X (OS2, ySubscriptXOffset);
  পক্ষ HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_OFFSET:       ঘূৰাই GET_METRIC_Y (OS2, ySubscriptYOffset);
  পক্ষ HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_SIZE:       ঘূৰাই GET_METRIC_X (OS2, ySuperscriptXSize);
  পক্ষ HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_SIZE:       ঘূৰাই GET_METRIC_Y (OS2, ySuperscriptYSize);
  পক্ষ HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_OFFSET:     ঘূৰাই GET_METRIC_X (OS2, ySuperscriptXOffset);
  পক্ষ HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_OFFSET:     ঘূৰাই GET_METRIC_Y (OS2, ySuperscriptYOffset);
  পক্ষ HB_OT_METRICS_TAG_STRIKEOUT_SIZE:              ঘূৰাই GET_METRIC_Y (OS2, yStrikeoutSize);
  পক্ষ HB_OT_METRICS_TAG_STRIKEOUT_OFFSET:            ঘূৰাই GET_METRIC_Y (OS2, yStrikeoutPosition);
  পক্ষ HB_OT_METRICS_TAG_UNDERLINE_SIZE:              ঘূৰাই GET_METRIC_Y (post->table, underlineThickness);
  পক্ষ HB_OT_METRICS_TAG_UNDERLINE_OFFSET:            ঘূৰাই GET_METRIC_Y (post->table, underlinePosition);

  /* Private tags */
  পক্ষ _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_OS2:    ঘূৰাই GET_METRIC_Y (OS2, sTypoAscender);
  পক্ষ _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_HHEA:   ঘূৰাই GET_HEA_METRIC_Y (hhea, ascender);
  পক্ষ _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_OS2:   ঘূৰাই GET_METRIC_Y (OS2, sTypoDescender);
  পক্ষ _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_HHEA:  ঘূৰাই GET_HEA_METRIC_Y (hhea, descender);
  পক্ষ HB_OT_METRICS_TAG_HORIZONTAL_সাৰি_GAP_OS2:    ঘূৰাই GET_METRIC_Y (OS2, s_Typo_সাৰি_Gap);
  পক্ষ HB_OT_METRICS_TAG_HORIZONTAL_সাৰি_GAP_HHEA:   ঘূৰাই GET_HEA_METRIC_Y (hhea, সাৰি_Gap);
#undef GET_HEA_METRIC_Y
#undef GET_HEA_METRIC_X
#undef GET_METRIC_Y
#undef GET_METRIC_X
#undef GET_VAR
  স্ৱভাৱ:                                        ঘূৰাই মিছা;
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
শূন্য
hb_ot_metrics_get_position_with_fallback (hb_font_t           *font,
					  hb_ot_metrics_tag_t  metrics_tag,
					  hb_position_t       *position     /* OUT */)
{
  hb_font_extents_t font_extents;
  hb_codepoint_t glyph;
  hb_glyph_extents_t extents;

  যদি (hb_ot_metrics_get_position (font, metrics_tag, position))
    {
      যদি ((metrics_tag != HB_OT_METRICS_TAG_STRIKEOUT_SIZE &&
           metrics_tag != HB_OT_METRICS_TAG_UNDERLINE_SIZE) ||
          *position != 0)
        ঘূৰাই;
    }

  ৱিকল্প (metrics_tag)
  {
  পক্ষ HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER:
  পক্ষ HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_ASCENT:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.ascender;
    ভঙ্গ;

  পক্ষ HB_OT_METRICS_TAG_VERTICAL_ASCENDER:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ascender;
    ভঙ্গ;

  পক্ষ HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER:
  পক্ষ HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_DESCENT:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.descender;
    ভঙ্গ;

  পক্ষ HB_OT_METRICS_TAG_VERTICAL_DESCENDER:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ascender;
    ভঙ্গ;

  পক্ষ HB_OT_METRICS_TAG_HORIZONTAL_সাৰি_GAP:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.সাৰি_gap;
    ভঙ্গ;

  পক্ষ HB_OT_METRICS_TAG_VERTICAL_সাৰি_GAP:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.সাৰি_gap;
    ভঙ্গ;

  পক্ষ HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE:
  পক্ষ HB_OT_METRICS_TAG_VERTICAL_CARET_RISE:
    *position = 1;
    ভঙ্গ;

  পক্ষ HB_OT_METRICS_TAG_HORIZONTAL_CARET_RUN:
  পক্ষ HB_OT_METRICS_TAG_VERTICAL_CARET_RUN:
    *position = 0;
    ভঙ্গ;

  পক্ষ HB_OT_METRICS_TAG_HORIZONTAL_CARET_OFFSET:
  পক্ষ HB_OT_METRICS_TAG_VERTICAL_CARET_OFFSET:
    *position = 0;
    ভঙ্গ;

  পক্ষ HB_OT_METRICS_TAG_X_HEIGHT:
    যদি (hb_font_get_nominal_glyph (font, 'x', &glyph) &&
        hb_font_get_glyph_extents (font, glyph, &extents))
      *position = extents.y_bearing;
    অন্যথা
      *position = font->y_scale / 2;
    ভঙ্গ;

  পক্ষ HB_OT_METRICS_TAG_CAP_HEIGHT:
    যদি (hb_font_get_nominal_glyph (font, 'O', &glyph) &&
        hb_font_get_glyph_extents (font, glyph, &extents))
      *position = hb_clamp_to<hb_position_t> ((int64_t) extents.height +
					      2 * (int64_t) extents.y_bearing);
    অন্যথা
      *position = hb_clamp_to<hb_position_t> ((int64_t) font->y_scale * 2 / 3);
    ভঙ্গ;

  পক্ষ HB_OT_METRICS_TAG_STRIKEOUT_SIZE:
  পক্ষ HB_OT_METRICS_TAG_UNDERLINE_SIZE:
    *position = font->y_scale / 18;
    ভঙ্গ;

  পক্ষ HB_OT_METRICS_TAG_STRIKEOUT_OFFSET:
    {
      hb_position_t ascender;
      hb_ot_metrics_get_position_with_fallback (font,
                                                HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER,
                                                &ascender);
      *position = ascender / 2;
    }
    ভঙ্গ;

  পক্ষ HB_OT_METRICS_TAG_UNDERLINE_OFFSET:
    *position = hb_saturate_neg (font->y_scale) / 18;
    ভঙ্গ;

  পক্ষ HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_SIZE:
  পক্ষ HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_SIZE:
    *position = hb_clamp_to<hb_position_t> ((int64_t) font->x_scale * 10 / 12);
    ভঙ্গ;

  পক্ষ HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_SIZE:
  পক্ষ HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_SIZE:
    *position = hb_clamp_to<hb_position_t> ((int64_t) font->y_scale * 10 / 12);
    ভঙ্গ;

  পক্ষ HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_OFFSET:
  পক্ষ HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_OFFSET:
    *position = 0;
    ভঙ্গ;

  পক্ষ HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_OFFSET:
  পক্ষ HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_OFFSET:
    *position = font->y_scale / 5;
    ভঙ্গ;

  পক্ষ HB_OT_METRICS_TAG_MAX_মান:
  স্ৱভাৱ:
    *position = 0;
    ভঙ্গ;
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
ভিন্ন
hb_ot_metrics_get_variation (hb_font_t *font, hb_ot_metrics_tag_t metrics_tag)
{
  ঘূৰাই font->face->table.MVAR->get_var (metrics_tag, font->coords, font->অঙ্ক_coords);
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
  ঘূৰাই font->em_scalef_x (hb_ot_metrics_get_variation (font, metrics_tag));
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
  ঘূৰাই font->em_scalef_y (hb_ot_metrics_get_variation (font, metrics_tag));
}
#endif

#endif

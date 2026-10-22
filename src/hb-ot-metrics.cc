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

#അന്തര്ഭാവ "hb.hh"

#അന്തര്ഭാവ "hb-ot-var-mvar-table.hh"
#അന്തര്ഭാവ "hb-ot-gasp-table.hh" // Just so we compile it; unused otherwise.
#അന്തര്ഭാവ "hb-ot-os2-table.hh"
#അന്തര്ഭാവ "hb-ot-post-table.hh"
#അന്തര്ഭാവ "hb-ot-hhea-table.hh"
#അന്തര്ഭാവ "hb-ot-metrics.hh"
#അന്തര്ഭാവ "hb-ot-face.hh"


/**
 * SECTION:hb-ot-metrics
 * @title: hb-ot-metrics
 * @short_description: OpenType Metrics
 * @include: hb-ot.h
 *
 * Functions for fetching metrics from fonts.
 **/

സ്ഥിതം ദശാംശം
_fix_ascender_descender (ദശാംശം value, hb_ot_metrics_tag_t metrics_tag)
{
  എങ്കിൽ (metrics_tag == HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER ||
      metrics_tag == HB_OT_METRICS_TAG_VERTICAL_ASCENDER)
    തിരികെ fabs ((ഇരട്ട) value);
  എങ്കിൽ (metrics_tag == HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER ||
      metrics_tag == HB_OT_METRICS_TAG_VERTICAL_DESCENDER)
    തിരികെ -fabs ((ഇരട്ട) value);
  തിരികെ value;
}

/* The common part of _get_position logic needed on hb-ot-font and here
   to be able to have slim builds without the not always needed parts */
തർക്കം
_hb_ot_metrics_get_position_common (hb_font_t           *font,
				    hb_ot_metrics_tag_t  metrics_tag,
				    hb_position_t       *position     /* OUT.  May be NULL. */)
{
  hb_face_t *face = font->face;
  തിരഞ്ഞെടുപ്പ് ((unsigned) metrics_tag)
  {
#ifndef HB_NO_VAR
#define GET_VAR face->table.MVAR->get_var (metrics_tag, font->coords, font->അക്കം_coords)
#അല്ലെങ്കിൽ
#define GET_VAR .0f
#endif
#define GET_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((ശൂന്യം) (position && (*position = font->em_scalef_y (_fix_ascender_descender ( \
      face->table.TABLE->ATTR + GET_VAR, metrics_tag)))), സത്യം))
#define GET_HEA_METRIC_X(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((ശൂന്യം) (position && (*position = font->em_scalef_x (_fix_ascender_descender ( \
      HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR, metrics_tag)))), സത്യം))
#define GET_HEA_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((ശൂന്യം) (position && (*position = font->em_scalef_y (_fix_ascender_descender ( \
      HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR, metrics_tag)))), സത്യം))

  അവസ്ഥ HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER:
    തിരികെ (face->table.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, sTypoAscender)) ||
	   GET_HEA_METRIC_Y (hhea, ascender);
  അവസ്ഥ HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER:
    തിരികെ (face->table.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, sTypoDescender)) ||
	   GET_HEA_METRIC_Y (hhea, descender);
  അവസ്ഥ HB_OT_METRICS_TAG_HORIZONTAL_വരി_GAP:
    തിരികെ (face->table.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, s_Typo_വരി_Gap)) ||
	   GET_HEA_METRIC_Y (hhea, വരി_Gap);

#ifndef HB_NO_VERTICAL
  അവസ്ഥ HB_OT_METRICS_TAG_VERTICAL_ASCENDER:  തിരികെ GET_HEA_METRIC_X (vhea, ascender);
  അവസ്ഥ HB_OT_METRICS_TAG_VERTICAL_DESCENDER: തിരികെ GET_HEA_METRIC_X (vhea, descender);
  അവസ്ഥ HB_OT_METRICS_TAG_VERTICAL_വരി_GAP:  തിരികെ GET_HEA_METRIC_X (vhea, വരി_Gap);
#endif

#undef GET_HEA_METRIC_Y
#undef GET_HEA_METRIC_X
#undef GET_METRIC_Y
#undef GET_VAR
  സ്ഥിരസ്ഥിതി:                               പ്രതിജ്ഞാ (0); തിരികെ അസത്യം;
  }
}

#ifndef HB_NO_METRICS

#എങ്കിൽ 0
സ്ഥിതം തർക്കം
_get_gasp (hb_face_t *face, ദശാംശം *result, hb_ot_metrics_tag_t metrics_tag)
{
  സ്ഥിരം OT::GaspRange& range = face->table.gasp->get_gasp_range (metrics_tag - HB_TAG ('g','s','p','0'));
  എങ്കിൽ (&range == &Null (OT::GaspRange)) തിരികെ അസത്യം;
  എങ്കിൽ (result) *result = range.rangeMaxPPEM + font->face->table.MVAR->get_var (metrics_tag, font->coords, font->അക്കം_coords);
  തിരികെ സത്യം;
}
#endif

/* Private tags for https://github.com/harfbuzz/harfbuzz/issues/1866 */
#define _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_OS2   HB_TAG ('O','a','s','c')
#define _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_HHEA  HB_TAG ('H','a','s','c')
#define _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_OS2  HB_TAG ('O','d','s','c')
#define _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_HHEA HB_TAG ('H','d','s','c')
#define HB_OT_METRICS_TAG_HORIZONTAL_വരി_GAP_OS2   HB_TAG ('O','l','g','p')
#define HB_OT_METRICS_TAG_HORIZONTAL_വരി_GAP_HHEA  HB_TAG ('H','l','g','p')

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
  തിരഞ്ഞെടുപ്പ് ((unsigned) metrics_tag)
  {
  അവസ്ഥ HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER:
  അവസ്ഥ HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER:
  അവസ്ഥ HB_OT_METRICS_TAG_HORIZONTAL_വരി_GAP:
  അവസ്ഥ HB_OT_METRICS_TAG_VERTICAL_ASCENDER:
  അവസ്ഥ HB_OT_METRICS_TAG_VERTICAL_DESCENDER:
  അവസ്ഥ HB_OT_METRICS_TAG_VERTICAL_വരി_GAP:           തിരികെ _hb_ot_metrics_get_position_common (font, metrics_tag, position);
#ifndef HB_NO_VAR
#define GET_VAR hb_ot_metrics_get_variation (font, metrics_tag)
#അല്ലെങ്കിൽ
#define GET_VAR 0
#endif
#define GET_METRIC_X(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((ശൂന്യം) (position && (*position = font->em_scalef_x (face->table.TABLE->ATTR + GET_VAR))), സത്യം))
#define GET_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((ശൂന്യം) (position && (*position = font->em_scalef_y (face->table.TABLE->ATTR + GET_VAR))), സത്യം))
#define GET_HEA_METRIC_X(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((ശൂന്യം) (position && (*position = font->em_scalef_x (HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR))), സത്യം))
#define GET_HEA_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((ശൂന്യം) (position && (*position = font->em_scalef_y (HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR))), സത്യം))
  അവസ്ഥ HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_ASCENT:  തിരികെ GET_METRIC_Y (OS2, usWinAscent);
  അവസ്ഥ HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_DESCENT: തിരികെ GET_METRIC_Y (OS2, usWinDescent);

  അവസ്ഥ HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE:
  അവസ്ഥ HB_OT_METRICS_TAG_HORIZONTAL_CARET_RUN:
  {
    unsigned mult = 1u;

    എങ്കിൽ (font->slant)
    {
      unsigned rise = HB_DUAL_GET (*face->table.hhea, table->caretSlopeRise);
      unsigned upem = face->get_upem ();
      mult = (rise && rise < upem) ? hb_min (upem / rise, 256u) : 1u;
    }

    എങ്കിൽ (metrics_tag == HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE)
    {
      തർക്കം ret = GET_HEA_METRIC_Y (hhea, caretSlopeRise);

      എങ്കിൽ (position)
	*position = hb_clamp_to<hb_position_t> ((int64_t) *position * mult);

      തിരികെ ret;
    }
    അല്ലെങ്കിൽ
    {
      hb_position_t rise = 0;

      എങ്കിൽ (font->slant && position && GET_HEA_METRIC_Y (hhea, caretSlopeRise))
	rise = *position;

      തർക്കം ret = GET_HEA_METRIC_X (hhea, caretSlopeRun);

      എങ്കിൽ (position)
      {
	*position = hb_clamp_to<hb_position_t> ((int64_t) *position * mult);

	എങ്കിൽ (font->slant)
	  *position = hb_saturate_add (*position,
				       hb_clamp_to<hb_position_t> (roundf (mult * font->slant_xy * rise)));
      }

      തിരികെ ret;
    }
  }
  അവസ്ഥ HB_OT_METRICS_TAG_HORIZONTAL_CARET_OFFSET:     തിരികെ GET_HEA_METRIC_X (hhea, caretOffset);

#ifndef HB_NO_VERTICAL
  അവസ്ഥ HB_OT_METRICS_TAG_VERTICAL_CARET_RISE:         തിരികെ GET_HEA_METRIC_X (vhea, caretSlopeRise);
  അവസ്ഥ HB_OT_METRICS_TAG_VERTICAL_CARET_RUN:          തിരികെ GET_HEA_METRIC_Y (vhea, caretSlopeRun);
  അവസ്ഥ HB_OT_METRICS_TAG_VERTICAL_CARET_OFFSET:       തിരികെ GET_HEA_METRIC_Y (vhea, caretOffset);
#endif
  അവസ്ഥ HB_OT_METRICS_TAG_X_HEIGHT:                    തിരികെ GET_METRIC_Y (OS2->v2 (), sxHeight);
  അവസ്ഥ HB_OT_METRICS_TAG_CAP_HEIGHT:                  തിരികെ GET_METRIC_Y (OS2->v2 (), sCapHeight);
  അവസ്ഥ HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_SIZE:         തിരികെ GET_METRIC_X (OS2, ySubscriptXSize);
  അവസ്ഥ HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_SIZE:         തിരികെ GET_METRIC_Y (OS2, ySubscriptYSize);
  അവസ്ഥ HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_OFFSET:       തിരികെ GET_METRIC_X (OS2, ySubscriptXOffset);
  അവസ്ഥ HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_OFFSET:       തിരികെ GET_METRIC_Y (OS2, ySubscriptYOffset);
  അവസ്ഥ HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_SIZE:       തിരികെ GET_METRIC_X (OS2, ySuperscriptXSize);
  അവസ്ഥ HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_SIZE:       തിരികെ GET_METRIC_Y (OS2, ySuperscriptYSize);
  അവസ്ഥ HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_OFFSET:     തിരികെ GET_METRIC_X (OS2, ySuperscriptXOffset);
  അവസ്ഥ HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_OFFSET:     തിരികെ GET_METRIC_Y (OS2, ySuperscriptYOffset);
  അവസ്ഥ HB_OT_METRICS_TAG_STRIKEOUT_SIZE:              തിരികെ GET_METRIC_Y (OS2, yStrikeoutSize);
  അവസ്ഥ HB_OT_METRICS_TAG_STRIKEOUT_OFFSET:            തിരികെ GET_METRIC_Y (OS2, yStrikeoutPosition);
  അവസ്ഥ HB_OT_METRICS_TAG_UNDERLINE_SIZE:              തിരികെ GET_METRIC_Y (post->table, underlineThickness);
  അവസ്ഥ HB_OT_METRICS_TAG_UNDERLINE_OFFSET:            തിരികെ GET_METRIC_Y (post->table, underlinePosition);

  /* Private tags */
  അവസ്ഥ _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_OS2:    തിരികെ GET_METRIC_Y (OS2, sTypoAscender);
  അവസ്ഥ _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_HHEA:   തിരികെ GET_HEA_METRIC_Y (hhea, ascender);
  അവസ്ഥ _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_OS2:   തിരികെ GET_METRIC_Y (OS2, sTypoDescender);
  അവസ്ഥ _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_HHEA:  തിരികെ GET_HEA_METRIC_Y (hhea, descender);
  അവസ്ഥ HB_OT_METRICS_TAG_HORIZONTAL_വരി_GAP_OS2:    തിരികെ GET_METRIC_Y (OS2, s_Typo_വരി_Gap);
  അവസ്ഥ HB_OT_METRICS_TAG_HORIZONTAL_വരി_GAP_HHEA:   തിരികെ GET_HEA_METRIC_Y (hhea, വരി_Gap);
#undef GET_HEA_METRIC_Y
#undef GET_HEA_METRIC_X
#undef GET_METRIC_Y
#undef GET_METRIC_X
#undef GET_VAR
  സ്ഥിരസ്ഥിതി:                                        തിരികെ അസത്യം;
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
ശൂന്യം
hb_ot_metrics_get_position_with_fallback (hb_font_t           *font,
					  hb_ot_metrics_tag_t  metrics_tag,
					  hb_position_t       *position     /* OUT */)
{
  hb_font_extents_t font_extents;
  hb_codepoint_t glyph;
  hb_glyph_extents_t extents;

  എങ്കിൽ (hb_ot_metrics_get_position (font, metrics_tag, position))
    {
      എങ്കിൽ ((metrics_tag != HB_OT_METRICS_TAG_STRIKEOUT_SIZE &&
           metrics_tag != HB_OT_METRICS_TAG_UNDERLINE_SIZE) ||
          *position != 0)
        തിരികെ;
    }

  തിരഞ്ഞെടുപ്പ് (metrics_tag)
  {
  അവസ്ഥ HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER:
  അവസ്ഥ HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_ASCENT:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.ascender;
    നിർത്തുക;

  അവസ്ഥ HB_OT_METRICS_TAG_VERTICAL_ASCENDER:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ascender;
    നിർത്തുക;

  അവസ്ഥ HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER:
  അവസ്ഥ HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_DESCENT:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.descender;
    നിർത്തുക;

  അവസ്ഥ HB_OT_METRICS_TAG_VERTICAL_DESCENDER:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ascender;
    നിർത്തുക;

  അവസ്ഥ HB_OT_METRICS_TAG_HORIZONTAL_വരി_GAP:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.വരി_gap;
    നിർത്തുക;

  അവസ്ഥ HB_OT_METRICS_TAG_VERTICAL_വരി_GAP:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.വരി_gap;
    നിർത്തുക;

  അവസ്ഥ HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE:
  അവസ്ഥ HB_OT_METRICS_TAG_VERTICAL_CARET_RISE:
    *position = 1;
    നിർത്തുക;

  അവസ്ഥ HB_OT_METRICS_TAG_HORIZONTAL_CARET_RUN:
  അവസ്ഥ HB_OT_METRICS_TAG_VERTICAL_CARET_RUN:
    *position = 0;
    നിർത്തുക;

  അവസ്ഥ HB_OT_METRICS_TAG_HORIZONTAL_CARET_OFFSET:
  അവസ്ഥ HB_OT_METRICS_TAG_VERTICAL_CARET_OFFSET:
    *position = 0;
    നിർത്തുക;

  അവസ്ഥ HB_OT_METRICS_TAG_X_HEIGHT:
    എങ്കിൽ (hb_font_get_nominal_glyph (font, 'x', &glyph) &&
        hb_font_get_glyph_extents (font, glyph, &extents))
      *position = extents.y_bearing;
    അല്ലെങ്കിൽ
      *position = font->y_scale / 2;
    നിർത്തുക;

  അവസ്ഥ HB_OT_METRICS_TAG_CAP_HEIGHT:
    എങ്കിൽ (hb_font_get_nominal_glyph (font, 'O', &glyph) &&
        hb_font_get_glyph_extents (font, glyph, &extents))
      *position = hb_clamp_to<hb_position_t> ((int64_t) extents.height +
					      2 * (int64_t) extents.y_bearing);
    അല്ലെങ്കിൽ
      *position = hb_clamp_to<hb_position_t> ((int64_t) font->y_scale * 2 / 3);
    നിർത്തുക;

  അവസ്ഥ HB_OT_METRICS_TAG_STRIKEOUT_SIZE:
  അവസ്ഥ HB_OT_METRICS_TAG_UNDERLINE_SIZE:
    *position = font->y_scale / 18;
    നിർത്തുക;

  അവസ്ഥ HB_OT_METRICS_TAG_STRIKEOUT_OFFSET:
    {
      hb_position_t ascender;
      hb_ot_metrics_get_position_with_fallback (font,
                                                HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER,
                                                &ascender);
      *position = ascender / 2;
    }
    നിർത്തുക;

  അവസ്ഥ HB_OT_METRICS_TAG_UNDERLINE_OFFSET:
    *position = hb_saturate_neg (font->y_scale) / 18;
    നിർത്തുക;

  അവസ്ഥ HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_SIZE:
  അവസ്ഥ HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_SIZE:
    *position = hb_clamp_to<hb_position_t> ((int64_t) font->x_scale * 10 / 12);
    നിർത്തുക;

  അവസ്ഥ HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_SIZE:
  അവസ്ഥ HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_SIZE:
    *position = hb_clamp_to<hb_position_t> ((int64_t) font->y_scale * 10 / 12);
    നിർത്തുക;

  അവസ്ഥ HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_OFFSET:
  അവസ്ഥ HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_OFFSET:
    *position = 0;
    നിർത്തുക;

  അവസ്ഥ HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_OFFSET:
  അവസ്ഥ HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_OFFSET:
    *position = font->y_scale / 5;
    നിർത്തുക;

  അവസ്ഥ HB_OT_METRICS_TAG_MAX_മൂല്യം:
  സ്ഥിരസ്ഥിതി:
    *position = 0;
    നിർത്തുക;
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
ദശാംശം
hb_ot_metrics_get_variation (hb_font_t *font, hb_ot_metrics_tag_t metrics_tag)
{
  തിരികെ font->face->table.MVAR->get_var (metrics_tag, font->coords, font->അക്കം_coords);
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
  തിരികെ font->em_scalef_x (hb_ot_metrics_get_variation (font, metrics_tag));
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
  തിരികെ font->em_scalef_y (hb_ot_metrics_get_variation (font, metrics_tag));
}
#endif

#endif

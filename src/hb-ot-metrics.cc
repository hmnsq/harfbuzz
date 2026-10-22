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

#அந்தர்பாவ "hb.hh"

#அந்தர்பாவ "hb-ot-var-mvar-table.hh"
#அந்தர்பாவ "hb-ot-gasp-table.hh" // Just so we compile it; unused otherwise.
#அந்தர்பாவ "hb-ot-os2-table.hh"
#அந்தர்பாவ "hb-ot-post-table.hh"
#அந்தர்பாவ "hb-ot-hhea-table.hh"
#அந்தர்பாவ "hb-ot-metrics.hh"
#அந்தர்பாவ "hb-ot-face.hh"


/**
 * SECTION:hb-ot-metrics
 * @title: hb-ot-metrics
 * @short_description: OpenType Metrics
 * @include: hb-ot.h
 *
 * Functions for fetching metrics from fonts.
 **/

நிலையான தசம
_fix_ascender_descender (தசம value, hb_ot_metrics_tag_t metrics_tag)
{
  எனில் (metrics_tag == HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER ||
      metrics_tag == HB_OT_METRICS_TAG_VERTICAL_ASCENDER)
    திரும்பு fabs ((இரட்டை) value);
  எனில் (metrics_tag == HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER ||
      metrics_tag == HB_OT_METRICS_TAG_VERTICAL_DESCENDER)
    திரும்பு -fabs ((இரட்டை) value);
  திரும்பு value;
}

/* The common part of _get_position logic needed on hb-ot-font and here
   to be able to have slim builds without the not always needed parts */
தருக்க
_hb_ot_metrics_get_position_common (hb_font_t           *font,
				    hb_ot_metrics_tag_t  metrics_tag,
				    hb_position_t       *position     /* OUT.  May be NULL. */)
{
  hb_face_t *face = font->face;
  தேர்வு ((unsigned) metrics_tag)
  {
#ifndef HB_NO_VAR
#define GET_VAR face->table.MVAR->get_var (metrics_tag, font->coords, font->இலக்கம்_coords)
#இல்லையெனில்
#define GET_VAR .0f
#endif
#define GET_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((வெறுமை) (position && (*position = font->em_scalef_y (_fix_ascender_descender ( \
      face->table.TABLE->ATTR + GET_VAR, metrics_tag)))), உண்மை))
#define GET_HEA_METRIC_X(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((வெறுமை) (position && (*position = font->em_scalef_x (_fix_ascender_descender ( \
      HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR, metrics_tag)))), உண்மை))
#define GET_HEA_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((வெறுமை) (position && (*position = font->em_scalef_y (_fix_ascender_descender ( \
      HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR, metrics_tag)))), உண்மை))

  நிலை HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER:
    திரும்பு (face->table.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, sTypoAscender)) ||
	   GET_HEA_METRIC_Y (hhea, ascender);
  நிலை HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER:
    திரும்பு (face->table.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, sTypoDescender)) ||
	   GET_HEA_METRIC_Y (hhea, descender);
  நிலை HB_OT_METRICS_TAG_HORIZONTAL_வரி_GAP:
    திரும்பு (face->table.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, s_Typo_வரி_Gap)) ||
	   GET_HEA_METRIC_Y (hhea, வரி_Gap);

#ifndef HB_NO_VERTICAL
  நிலை HB_OT_METRICS_TAG_VERTICAL_ASCENDER:  திரும்பு GET_HEA_METRIC_X (vhea, ascender);
  நிலை HB_OT_METRICS_TAG_VERTICAL_DESCENDER: திரும்பு GET_HEA_METRIC_X (vhea, descender);
  நிலை HB_OT_METRICS_TAG_VERTICAL_வரி_GAP:  திரும்பு GET_HEA_METRIC_X (vhea, வரி_Gap);
#endif

#undef GET_HEA_METRIC_Y
#undef GET_HEA_METRIC_X
#undef GET_METRIC_Y
#undef GET_VAR
  இயல்பு:                               ப்ரதிஜ்ஞா (0); திரும்பு பொய்;
  }
}

#ifndef HB_NO_METRICS

#எனில் 0
நிலையான தருக்க
_get_gasp (hb_face_t *face, தசம *result, hb_ot_metrics_tag_t metrics_tag)
{
  மாறிலி OT::GaspRange& range = face->table.gasp->get_gasp_range (metrics_tag - HB_TAG ('g','s','p','0'));
  எனில் (&range == &Null (OT::GaspRange)) திரும்பு பொய்;
  எனில் (result) *result = range.rangeMaxPPEM + font->face->table.MVAR->get_var (metrics_tag, font->coords, font->இலக்கம்_coords);
  திரும்பு உண்மை;
}
#endif

/* Private tags for https://github.com/harfbuzz/harfbuzz/issues/1866 */
#define _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_OS2   HB_TAG ('O','a','s','c')
#define _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_HHEA  HB_TAG ('H','a','s','c')
#define _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_OS2  HB_TAG ('O','d','s','c')
#define _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_HHEA HB_TAG ('H','d','s','c')
#define HB_OT_METRICS_TAG_HORIZONTAL_வரி_GAP_OS2   HB_TAG ('O','l','g','p')
#define HB_OT_METRICS_TAG_HORIZONTAL_வரி_GAP_HHEA  HB_TAG ('H','l','g','p')

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
  தேர்வு ((unsigned) metrics_tag)
  {
  நிலை HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER:
  நிலை HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER:
  நிலை HB_OT_METRICS_TAG_HORIZONTAL_வரி_GAP:
  நிலை HB_OT_METRICS_TAG_VERTICAL_ASCENDER:
  நிலை HB_OT_METRICS_TAG_VERTICAL_DESCENDER:
  நிலை HB_OT_METRICS_TAG_VERTICAL_வரி_GAP:           திரும்பு _hb_ot_metrics_get_position_common (font, metrics_tag, position);
#ifndef HB_NO_VAR
#define GET_VAR hb_ot_metrics_get_variation (font, metrics_tag)
#இல்லையெனில்
#define GET_VAR 0
#endif
#define GET_METRIC_X(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((வெறுமை) (position && (*position = font->em_scalef_x (face->table.TABLE->ATTR + GET_VAR))), உண்மை))
#define GET_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((வெறுமை) (position && (*position = font->em_scalef_y (face->table.TABLE->ATTR + GET_VAR))), உண்மை))
#define GET_HEA_METRIC_X(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((வெறுமை) (position && (*position = font->em_scalef_x (HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR))), உண்மை))
#define GET_HEA_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((வெறுமை) (position && (*position = font->em_scalef_y (HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR))), உண்மை))
  நிலை HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_ASCENT:  திரும்பு GET_METRIC_Y (OS2, usWinAscent);
  நிலை HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_DESCENT: திரும்பு GET_METRIC_Y (OS2, usWinDescent);

  நிலை HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE:
  நிலை HB_OT_METRICS_TAG_HORIZONTAL_CARET_RUN:
  {
    unsigned mult = 1u;

    எனில் (font->slant)
    {
      unsigned rise = HB_DUAL_GET (*face->table.hhea, table->caretSlopeRise);
      unsigned upem = face->get_upem ();
      mult = (rise && rise < upem) ? hb_min (upem / rise, 256u) : 1u;
    }

    எனில் (metrics_tag == HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE)
    {
      தருக்க ret = GET_HEA_METRIC_Y (hhea, caretSlopeRise);

      எனில் (position)
	*position = hb_clamp_to<hb_position_t> ((int64_t) *position * mult);

      திரும்பு ret;
    }
    இல்லையெனில்
    {
      hb_position_t rise = 0;

      எனில் (font->slant && position && GET_HEA_METRIC_Y (hhea, caretSlopeRise))
	rise = *position;

      தருக்க ret = GET_HEA_METRIC_X (hhea, caretSlopeRun);

      எனில் (position)
      {
	*position = hb_clamp_to<hb_position_t> ((int64_t) *position * mult);

	எனில் (font->slant)
	  *position = hb_saturate_add (*position,
				       hb_clamp_to<hb_position_t> (roundf (mult * font->slant_xy * rise)));
      }

      திரும்பு ret;
    }
  }
  நிலை HB_OT_METRICS_TAG_HORIZONTAL_CARET_OFFSET:     திரும்பு GET_HEA_METRIC_X (hhea, caretOffset);

#ifndef HB_NO_VERTICAL
  நிலை HB_OT_METRICS_TAG_VERTICAL_CARET_RISE:         திரும்பு GET_HEA_METRIC_X (vhea, caretSlopeRise);
  நிலை HB_OT_METRICS_TAG_VERTICAL_CARET_RUN:          திரும்பு GET_HEA_METRIC_Y (vhea, caretSlopeRun);
  நிலை HB_OT_METRICS_TAG_VERTICAL_CARET_OFFSET:       திரும்பு GET_HEA_METRIC_Y (vhea, caretOffset);
#endif
  நிலை HB_OT_METRICS_TAG_X_HEIGHT:                    திரும்பு GET_METRIC_Y (OS2->v2 (), sxHeight);
  நிலை HB_OT_METRICS_TAG_CAP_HEIGHT:                  திரும்பு GET_METRIC_Y (OS2->v2 (), sCapHeight);
  நிலை HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_SIZE:         திரும்பு GET_METRIC_X (OS2, ySubscriptXSize);
  நிலை HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_SIZE:         திரும்பு GET_METRIC_Y (OS2, ySubscriptYSize);
  நிலை HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_OFFSET:       திரும்பு GET_METRIC_X (OS2, ySubscriptXOffset);
  நிலை HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_OFFSET:       திரும்பு GET_METRIC_Y (OS2, ySubscriptYOffset);
  நிலை HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_SIZE:       திரும்பு GET_METRIC_X (OS2, ySuperscriptXSize);
  நிலை HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_SIZE:       திரும்பு GET_METRIC_Y (OS2, ySuperscriptYSize);
  நிலை HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_OFFSET:     திரும்பு GET_METRIC_X (OS2, ySuperscriptXOffset);
  நிலை HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_OFFSET:     திரும்பு GET_METRIC_Y (OS2, ySuperscriptYOffset);
  நிலை HB_OT_METRICS_TAG_STRIKEOUT_SIZE:              திரும்பு GET_METRIC_Y (OS2, yStrikeoutSize);
  நிலை HB_OT_METRICS_TAG_STRIKEOUT_OFFSET:            திரும்பு GET_METRIC_Y (OS2, yStrikeoutPosition);
  நிலை HB_OT_METRICS_TAG_UNDERLINE_SIZE:              திரும்பு GET_METRIC_Y (post->table, underlineThickness);
  நிலை HB_OT_METRICS_TAG_UNDERLINE_OFFSET:            திரும்பு GET_METRIC_Y (post->table, underlinePosition);

  /* Private tags */
  நிலை _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_OS2:    திரும்பு GET_METRIC_Y (OS2, sTypoAscender);
  நிலை _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_HHEA:   திரும்பு GET_HEA_METRIC_Y (hhea, ascender);
  நிலை _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_OS2:   திரும்பு GET_METRIC_Y (OS2, sTypoDescender);
  நிலை _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_HHEA:  திரும்பு GET_HEA_METRIC_Y (hhea, descender);
  நிலை HB_OT_METRICS_TAG_HORIZONTAL_வரி_GAP_OS2:    திரும்பு GET_METRIC_Y (OS2, s_Typo_வரி_Gap);
  நிலை HB_OT_METRICS_TAG_HORIZONTAL_வரி_GAP_HHEA:   திரும்பு GET_HEA_METRIC_Y (hhea, வரி_Gap);
#undef GET_HEA_METRIC_Y
#undef GET_HEA_METRIC_X
#undef GET_METRIC_Y
#undef GET_METRIC_X
#undef GET_VAR
  இயல்பு:                                        திரும்பு பொய்;
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
வெறுமை
hb_ot_metrics_get_position_with_fallback (hb_font_t           *font,
					  hb_ot_metrics_tag_t  metrics_tag,
					  hb_position_t       *position     /* OUT */)
{
  hb_font_extents_t font_extents;
  hb_codepoint_t glyph;
  hb_glyph_extents_t extents;

  எனில் (hb_ot_metrics_get_position (font, metrics_tag, position))
    {
      எனில் ((metrics_tag != HB_OT_METRICS_TAG_STRIKEOUT_SIZE &&
           metrics_tag != HB_OT_METRICS_TAG_UNDERLINE_SIZE) ||
          *position != 0)
        திரும்பு;
    }

  தேர்வு (metrics_tag)
  {
  நிலை HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER:
  நிலை HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_ASCENT:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.ascender;
    நிறுத்து;

  நிலை HB_OT_METRICS_TAG_VERTICAL_ASCENDER:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ascender;
    நிறுத்து;

  நிலை HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER:
  நிலை HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_DESCENT:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.descender;
    நிறுத்து;

  நிலை HB_OT_METRICS_TAG_VERTICAL_DESCENDER:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ascender;
    நிறுத்து;

  நிலை HB_OT_METRICS_TAG_HORIZONTAL_வரி_GAP:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.வரி_gap;
    நிறுத்து;

  நிலை HB_OT_METRICS_TAG_VERTICAL_வரி_GAP:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.வரி_gap;
    நிறுத்து;

  நிலை HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE:
  நிலை HB_OT_METRICS_TAG_VERTICAL_CARET_RISE:
    *position = 1;
    நிறுத்து;

  நிலை HB_OT_METRICS_TAG_HORIZONTAL_CARET_RUN:
  நிலை HB_OT_METRICS_TAG_VERTICAL_CARET_RUN:
    *position = 0;
    நிறுத்து;

  நிலை HB_OT_METRICS_TAG_HORIZONTAL_CARET_OFFSET:
  நிலை HB_OT_METRICS_TAG_VERTICAL_CARET_OFFSET:
    *position = 0;
    நிறுத்து;

  நிலை HB_OT_METRICS_TAG_X_HEIGHT:
    எனில் (hb_font_get_nominal_glyph (font, 'x', &glyph) &&
        hb_font_get_glyph_extents (font, glyph, &extents))
      *position = extents.y_bearing;
    இல்லையெனில்
      *position = font->y_scale / 2;
    நிறுத்து;

  நிலை HB_OT_METRICS_TAG_CAP_HEIGHT:
    எனில் (hb_font_get_nominal_glyph (font, 'O', &glyph) &&
        hb_font_get_glyph_extents (font, glyph, &extents))
      *position = hb_clamp_to<hb_position_t> ((int64_t) extents.height +
					      2 * (int64_t) extents.y_bearing);
    இல்லையெனில்
      *position = hb_clamp_to<hb_position_t> ((int64_t) font->y_scale * 2 / 3);
    நிறுத்து;

  நிலை HB_OT_METRICS_TAG_STRIKEOUT_SIZE:
  நிலை HB_OT_METRICS_TAG_UNDERLINE_SIZE:
    *position = font->y_scale / 18;
    நிறுத்து;

  நிலை HB_OT_METRICS_TAG_STRIKEOUT_OFFSET:
    {
      hb_position_t ascender;
      hb_ot_metrics_get_position_with_fallback (font,
                                                HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER,
                                                &ascender);
      *position = ascender / 2;
    }
    நிறுத்து;

  நிலை HB_OT_METRICS_TAG_UNDERLINE_OFFSET:
    *position = hb_saturate_neg (font->y_scale) / 18;
    நிறுத்து;

  நிலை HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_SIZE:
  நிலை HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_SIZE:
    *position = hb_clamp_to<hb_position_t> ((int64_t) font->x_scale * 10 / 12);
    நிறுத்து;

  நிலை HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_SIZE:
  நிலை HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_SIZE:
    *position = hb_clamp_to<hb_position_t> ((int64_t) font->y_scale * 10 / 12);
    நிறுத்து;

  நிலை HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_OFFSET:
  நிலை HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_OFFSET:
    *position = 0;
    நிறுத்து;

  நிலை HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_OFFSET:
  நிலை HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_OFFSET:
    *position = font->y_scale / 5;
    நிறுத்து;

  நிலை HB_OT_METRICS_TAG_MAX_மதிப்பு:
  இயல்பு:
    *position = 0;
    நிறுத்து;
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
தசம
hb_ot_metrics_get_variation (hb_font_t *font, hb_ot_metrics_tag_t metrics_tag)
{
  திரும்பு font->face->table.MVAR->get_var (metrics_tag, font->coords, font->இலக்கம்_coords);
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
  திரும்பு font->em_scalef_x (hb_ot_metrics_get_variation (font, metrics_tag));
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
  திரும்பு font->em_scalef_y (hb_ot_metrics_get_variation (font, metrics_tag));
}
#endif

#endif

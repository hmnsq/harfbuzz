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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ଅନ୍ତର୍ଭାଵ "hb-ot-var-mvar-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-gasp-table.hh" // Just so we compile it; unused otherwise.
#ଅନ୍ତର୍ଭାଵ "hb-ot-os2-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-post-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-hhea-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-metrics.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-face.hh"


/**
 * SECTION:hb-ot-metrics
 * @title: hb-ot-metrics
 * @short_description: OpenType Metrics
 * @include: hb-ot.h
 *
 * Functions for fetching metrics from fonts.
 **/

ସ୍ଥିର ଦଶମିକ
_fix_ascender_descender (ଦଶମିକ value, hb_ot_metrics_tag_t metrics_tag)
{
  ଯଦି (metrics_tag == HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER ||
      metrics_tag == HB_OT_METRICS_TAG_VERTICAL_ASCENDER)
    ଫେରନ୍ତୁ fabs ((ଦ୍ୱିଗୁଣ) value);
  ଯଦି (metrics_tag == HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER ||
      metrics_tag == HB_OT_METRICS_TAG_VERTICAL_DESCENDER)
    ଫେରନ୍ତୁ -fabs ((ଦ୍ୱିଗୁଣ) value);
  ଫେରନ୍ତୁ value;
}

/* The common part of _get_position logic needed on hb-ot-font and here
   to be able to have slim builds without the not always needed parts */
ତାର୍କିକ
_hb_ot_metrics_get_position_common (hb_font_t           *font,
				    hb_ot_metrics_tag_t  metrics_tag,
				    hb_position_t       *position     /* OUT.  May be NULL. */)
{
  hb_face_t *face = font->face;
  ବାଛ ((unsigned) metrics_tag)
  {
#ifndef HB_NO_VAR
#define GET_VAR face->table.MVAR->get_var (metrics_tag, font->coords, font->ଅଙ୍କ_coords)
#ଅନ୍ୟଥା
#define GET_VAR .0f
#endif
#define GET_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((ଶୂନ୍ୟ) (position && (*position = font->em_scalef_y (_fix_ascender_descender ( \
      face->table.TABLE->ATTR + GET_VAR, metrics_tag)))), ସତ୍ୟ))
#define GET_HEA_METRIC_X(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((ଶୂନ୍ୟ) (position && (*position = font->em_scalef_x (_fix_ascender_descender ( \
      HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR, metrics_tag)))), ସତ୍ୟ))
#define GET_HEA_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((ଶୂନ୍ୟ) (position && (*position = font->em_scalef_y (_fix_ascender_descender ( \
      HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR, metrics_tag)))), ସତ୍ୟ))

  ସ୍ଥିତି HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER:
    ଫେରନ୍ତୁ (face->table.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, sTypoAscender)) ||
	   GET_HEA_METRIC_Y (hhea, ascender);
  ସ୍ଥିତି HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER:
    ଫେରନ୍ତୁ (face->table.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, sTypoDescender)) ||
	   GET_HEA_METRIC_Y (hhea, descender);
  ସ୍ଥିତି HB_OT_METRICS_TAG_HORIZONTAL_ଧାଡ଼ି_GAP:
    ଫେରନ୍ତୁ (face->table.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, s_Typo_ଧାଡ଼ି_Gap)) ||
	   GET_HEA_METRIC_Y (hhea, ଧାଡ଼ି_Gap);

#ifndef HB_NO_VERTICAL
  ସ୍ଥିତି HB_OT_METRICS_TAG_VERTICAL_ASCENDER:  ଫେରନ୍ତୁ GET_HEA_METRIC_X (vhea, ascender);
  ସ୍ଥିତି HB_OT_METRICS_TAG_VERTICAL_DESCENDER: ଫେରନ୍ତୁ GET_HEA_METRIC_X (vhea, descender);
  ସ୍ଥିତି HB_OT_METRICS_TAG_VERTICAL_ଧାଡ଼ି_GAP:  ଫେରନ୍ତୁ GET_HEA_METRIC_X (vhea, ଧାଡ଼ି_Gap);
#endif

#undef GET_HEA_METRIC_Y
#undef GET_HEA_METRIC_X
#undef GET_METRIC_Y
#undef GET_VAR
  ପୂର୍ବନିର୍ଦ୍ଧାରିତ:                               ପ୍ରତିଜ୍ଞା (0); ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }
}

#ifndef HB_NO_METRICS

#ଯଦି 0
ସ୍ଥିର ତାର୍କିକ
_get_gasp (hb_face_t *face, ଦଶମିକ *result, hb_ot_metrics_tag_t metrics_tag)
{
  ଅଚଳ OT::GaspRange& range = face->table.gasp->get_gasp_range (metrics_tag - HB_TAG ('g','s','p','0'));
  ଯଦି (&range == &Null (OT::GaspRange)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
  ଯଦି (result) *result = range.rangeMaxPPEM + font->face->table.MVAR->get_var (metrics_tag, font->coords, font->ଅଙ୍କ_coords);
  ଫେରନ୍ତୁ ସତ୍ୟ;
}
#endif

/* Private tags for https://github.com/harfbuzz/harfbuzz/issues/1866 */
#define _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_OS2   HB_TAG ('O','a','s','c')
#define _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_HHEA  HB_TAG ('H','a','s','c')
#define _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_OS2  HB_TAG ('O','d','s','c')
#define _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_HHEA HB_TAG ('H','d','s','c')
#define HB_OT_METRICS_TAG_HORIZONTAL_ଧାଡ଼ି_GAP_OS2   HB_TAG ('O','l','g','p')
#define HB_OT_METRICS_TAG_HORIZONTAL_ଧାଡ଼ି_GAP_HHEA  HB_TAG ('H','l','g','p')

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
  ବାଛ ((unsigned) metrics_tag)
  {
  ସ୍ଥିତି HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER:
  ସ୍ଥିତି HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER:
  ସ୍ଥିତି HB_OT_METRICS_TAG_HORIZONTAL_ଧାଡ଼ି_GAP:
  ସ୍ଥିତି HB_OT_METRICS_TAG_VERTICAL_ASCENDER:
  ସ୍ଥିତି HB_OT_METRICS_TAG_VERTICAL_DESCENDER:
  ସ୍ଥିତି HB_OT_METRICS_TAG_VERTICAL_ଧାଡ଼ି_GAP:           ଫେରନ୍ତୁ _hb_ot_metrics_get_position_common (font, metrics_tag, position);
#ifndef HB_NO_VAR
#define GET_VAR hb_ot_metrics_get_variation (font, metrics_tag)
#ଅନ୍ୟଥା
#define GET_VAR 0
#endif
#define GET_METRIC_X(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((ଶୂନ୍ୟ) (position && (*position = font->em_scalef_x (face->table.TABLE->ATTR + GET_VAR))), ସତ୍ୟ))
#define GET_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((ଶୂନ୍ୟ) (position && (*position = font->em_scalef_y (face->table.TABLE->ATTR + GET_VAR))), ସତ୍ୟ))
#define GET_HEA_METRIC_X(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((ଶୂନ୍ୟ) (position && (*position = font->em_scalef_x (HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR))), ସତ୍ୟ))
#define GET_HEA_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((ଶୂନ୍ୟ) (position && (*position = font->em_scalef_y (HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR))), ସତ୍ୟ))
  ସ୍ଥିତି HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_ASCENT:  ଫେରନ୍ତୁ GET_METRIC_Y (OS2, usWinAscent);
  ସ୍ଥିତି HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_DESCENT: ଫେରନ୍ତୁ GET_METRIC_Y (OS2, usWinDescent);

  ସ୍ଥିତି HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE:
  ସ୍ଥିତି HB_OT_METRICS_TAG_HORIZONTAL_CARET_RUN:
  {
    unsigned mult = 1u;

    ଯଦି (font->slant)
    {
      unsigned rise = HB_DUAL_GET (*face->table.hhea, table->caretSlopeRise);
      unsigned upem = face->get_upem ();
      mult = (rise && rise < upem) ? hb_min (upem / rise, 256u) : 1u;
    }

    ଯଦି (metrics_tag == HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE)
    {
      ତାର୍କିକ ret = GET_HEA_METRIC_Y (hhea, caretSlopeRise);

      ଯଦି (position)
	*position = hb_clamp_to<hb_position_t> ((int64_t) *position * mult);

      ଫେରନ୍ତୁ ret;
    }
    ଅନ୍ୟଥା
    {
      hb_position_t rise = 0;

      ଯଦି (font->slant && position && GET_HEA_METRIC_Y (hhea, caretSlopeRise))
	rise = *position;

      ତାର୍କିକ ret = GET_HEA_METRIC_X (hhea, caretSlopeRun);

      ଯଦି (position)
      {
	*position = hb_clamp_to<hb_position_t> ((int64_t) *position * mult);

	ଯଦି (font->slant)
	  *position = hb_saturate_add (*position,
				       hb_clamp_to<hb_position_t> (roundf (mult * font->slant_xy * rise)));
      }

      ଫେରନ୍ତୁ ret;
    }
  }
  ସ୍ଥିତି HB_OT_METRICS_TAG_HORIZONTAL_CARET_OFFSET:     ଫେରନ୍ତୁ GET_HEA_METRIC_X (hhea, caretOffset);

#ifndef HB_NO_VERTICAL
  ସ୍ଥିତି HB_OT_METRICS_TAG_VERTICAL_CARET_RISE:         ଫେରନ୍ତୁ GET_HEA_METRIC_X (vhea, caretSlopeRise);
  ସ୍ଥିତି HB_OT_METRICS_TAG_VERTICAL_CARET_RUN:          ଫେରନ୍ତୁ GET_HEA_METRIC_Y (vhea, caretSlopeRun);
  ସ୍ଥିତି HB_OT_METRICS_TAG_VERTICAL_CARET_OFFSET:       ଫେରନ୍ତୁ GET_HEA_METRIC_Y (vhea, caretOffset);
#endif
  ସ୍ଥିତି HB_OT_METRICS_TAG_X_HEIGHT:                    ଫେରନ୍ତୁ GET_METRIC_Y (OS2->v2 (), sxHeight);
  ସ୍ଥିତି HB_OT_METRICS_TAG_CAP_HEIGHT:                  ଫେରନ୍ତୁ GET_METRIC_Y (OS2->v2 (), sCapHeight);
  ସ୍ଥିତି HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_SIZE:         ଫେରନ୍ତୁ GET_METRIC_X (OS2, ySubscriptXSize);
  ସ୍ଥିତି HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_SIZE:         ଫେରନ୍ତୁ GET_METRIC_Y (OS2, ySubscriptYSize);
  ସ୍ଥିତି HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_OFFSET:       ଫେରନ୍ତୁ GET_METRIC_X (OS2, ySubscriptXOffset);
  ସ୍ଥିତି HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_OFFSET:       ଫେରନ୍ତୁ GET_METRIC_Y (OS2, ySubscriptYOffset);
  ସ୍ଥିତି HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_SIZE:       ଫେରନ୍ତୁ GET_METRIC_X (OS2, ySuperscriptXSize);
  ସ୍ଥିତି HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_SIZE:       ଫେରନ୍ତୁ GET_METRIC_Y (OS2, ySuperscriptYSize);
  ସ୍ଥିତି HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_OFFSET:     ଫେରନ୍ତୁ GET_METRIC_X (OS2, ySuperscriptXOffset);
  ସ୍ଥିତି HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_OFFSET:     ଫେରନ୍ତୁ GET_METRIC_Y (OS2, ySuperscriptYOffset);
  ସ୍ଥିତି HB_OT_METRICS_TAG_STRIKEOUT_SIZE:              ଫେରନ୍ତୁ GET_METRIC_Y (OS2, yStrikeoutSize);
  ସ୍ଥିତି HB_OT_METRICS_TAG_STRIKEOUT_OFFSET:            ଫେରନ୍ତୁ GET_METRIC_Y (OS2, yStrikeoutPosition);
  ସ୍ଥିତି HB_OT_METRICS_TAG_UNDERLINE_SIZE:              ଫେରନ୍ତୁ GET_METRIC_Y (post->table, underlineThickness);
  ସ୍ଥିତି HB_OT_METRICS_TAG_UNDERLINE_OFFSET:            ଫେରନ୍ତୁ GET_METRIC_Y (post->table, underlinePosition);

  /* Private tags */
  ସ୍ଥିତି _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_OS2:    ଫେରନ୍ତୁ GET_METRIC_Y (OS2, sTypoAscender);
  ସ୍ଥିତି _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_HHEA:   ଫେରନ୍ତୁ GET_HEA_METRIC_Y (hhea, ascender);
  ସ୍ଥିତି _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_OS2:   ଫେରନ୍ତୁ GET_METRIC_Y (OS2, sTypoDescender);
  ସ୍ଥିତି _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_HHEA:  ଫେରନ୍ତୁ GET_HEA_METRIC_Y (hhea, descender);
  ସ୍ଥିତି HB_OT_METRICS_TAG_HORIZONTAL_ଧାଡ଼ି_GAP_OS2:    ଫେରନ୍ତୁ GET_METRIC_Y (OS2, s_Typo_ଧାଡ଼ି_Gap);
  ସ୍ଥିତି HB_OT_METRICS_TAG_HORIZONTAL_ଧାଡ଼ି_GAP_HHEA:   ଫେରନ୍ତୁ GET_HEA_METRIC_Y (hhea, ଧାଡ଼ି_Gap);
#undef GET_HEA_METRIC_Y
#undef GET_HEA_METRIC_X
#undef GET_METRIC_Y
#undef GET_METRIC_X
#undef GET_VAR
  ପୂର୍ବନିର୍ଦ୍ଧାରିତ:                                        ଫେରନ୍ତୁ ମିଥ୍ୟା;
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
ଶୂନ୍ୟ
hb_ot_metrics_get_position_with_fallback (hb_font_t           *font,
					  hb_ot_metrics_tag_t  metrics_tag,
					  hb_position_t       *position     /* OUT */)
{
  hb_font_extents_t font_extents;
  hb_codepoint_t glyph;
  hb_glyph_extents_t extents;

  ଯଦି (hb_ot_metrics_get_position (font, metrics_tag, position))
    {
      ଯଦି ((metrics_tag != HB_OT_METRICS_TAG_STRIKEOUT_SIZE &&
           metrics_tag != HB_OT_METRICS_TAG_UNDERLINE_SIZE) ||
          *position != 0)
        ଫେରନ୍ତୁ;
    }

  ବାଛ (metrics_tag)
  {
  ସ୍ଥିତି HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER:
  ସ୍ଥିତି HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_ASCENT:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.ascender;
    ଭାଙ୍ଗ;

  ସ୍ଥିତି HB_OT_METRICS_TAG_VERTICAL_ASCENDER:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ascender;
    ଭାଙ୍ଗ;

  ସ୍ଥିତି HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER:
  ସ୍ଥିତି HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_DESCENT:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.descender;
    ଭାଙ୍ଗ;

  ସ୍ଥିତି HB_OT_METRICS_TAG_VERTICAL_DESCENDER:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ascender;
    ଭାଙ୍ଗ;

  ସ୍ଥିତି HB_OT_METRICS_TAG_HORIZONTAL_ଧାଡ଼ି_GAP:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.ଧାଡ଼ି_gap;
    ଭାଙ୍ଗ;

  ସ୍ଥିତି HB_OT_METRICS_TAG_VERTICAL_ଧାଡ଼ି_GAP:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ଧାଡ଼ି_gap;
    ଭାଙ୍ଗ;

  ସ୍ଥିତି HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE:
  ସ୍ଥିତି HB_OT_METRICS_TAG_VERTICAL_CARET_RISE:
    *position = 1;
    ଭାଙ୍ଗ;

  ସ୍ଥିତି HB_OT_METRICS_TAG_HORIZONTAL_CARET_RUN:
  ସ୍ଥିତି HB_OT_METRICS_TAG_VERTICAL_CARET_RUN:
    *position = 0;
    ଭାଙ୍ଗ;

  ସ୍ଥିତି HB_OT_METRICS_TAG_HORIZONTAL_CARET_OFFSET:
  ସ୍ଥିତି HB_OT_METRICS_TAG_VERTICAL_CARET_OFFSET:
    *position = 0;
    ଭାଙ୍ଗ;

  ସ୍ଥିତି HB_OT_METRICS_TAG_X_HEIGHT:
    ଯଦି (hb_font_get_nominal_glyph (font, 'x', &glyph) &&
        hb_font_get_glyph_extents (font, glyph, &extents))
      *position = extents.y_bearing;
    ଅନ୍ୟଥା
      *position = font->y_scale / 2;
    ଭାଙ୍ଗ;

  ସ୍ଥିତି HB_OT_METRICS_TAG_CAP_HEIGHT:
    ଯଦି (hb_font_get_nominal_glyph (font, 'O', &glyph) &&
        hb_font_get_glyph_extents (font, glyph, &extents))
      *position = hb_clamp_to<hb_position_t> ((int64_t) extents.height +
					      2 * (int64_t) extents.y_bearing);
    ଅନ୍ୟଥା
      *position = hb_clamp_to<hb_position_t> ((int64_t) font->y_scale * 2 / 3);
    ଭାଙ୍ଗ;

  ସ୍ଥିତି HB_OT_METRICS_TAG_STRIKEOUT_SIZE:
  ସ୍ଥିତି HB_OT_METRICS_TAG_UNDERLINE_SIZE:
    *position = font->y_scale / 18;
    ଭାଙ୍ଗ;

  ସ୍ଥିତି HB_OT_METRICS_TAG_STRIKEOUT_OFFSET:
    {
      hb_position_t ascender;
      hb_ot_metrics_get_position_with_fallback (font,
                                                HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER,
                                                &ascender);
      *position = ascender / 2;
    }
    ଭାଙ୍ଗ;

  ସ୍ଥିତି HB_OT_METRICS_TAG_UNDERLINE_OFFSET:
    *position = hb_saturate_neg (font->y_scale) / 18;
    ଭାଙ୍ଗ;

  ସ୍ଥିତି HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_SIZE:
  ସ୍ଥିତି HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_SIZE:
    *position = hb_clamp_to<hb_position_t> ((int64_t) font->x_scale * 10 / 12);
    ଭାଙ୍ଗ;

  ସ୍ଥିତି HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_SIZE:
  ସ୍ଥିତି HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_SIZE:
    *position = hb_clamp_to<hb_position_t> ((int64_t) font->y_scale * 10 / 12);
    ଭାଙ୍ଗ;

  ସ୍ଥିତି HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_OFFSET:
  ସ୍ଥିତି HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_OFFSET:
    *position = 0;
    ଭାଙ୍ଗ;

  ସ୍ଥିତି HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_OFFSET:
  ସ୍ଥିତି HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_OFFSET:
    *position = font->y_scale / 5;
    ଭାଙ୍ଗ;

  ସ୍ଥିତି HB_OT_METRICS_TAG_MAX_ମୂଲ୍ୟ:
  ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
    *position = 0;
    ଭାଙ୍ଗ;
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
ଦଶମିକ
hb_ot_metrics_get_variation (hb_font_t *font, hb_ot_metrics_tag_t metrics_tag)
{
  ଫେରନ୍ତୁ font->face->table.MVAR->get_var (metrics_tag, font->coords, font->ଅଙ୍କ_coords);
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
  ଫେରନ୍ତୁ font->em_scalef_x (hb_ot_metrics_get_variation (font, metrics_tag));
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
  ଫେରନ୍ତୁ font->em_scalef_y (hb_ot_metrics_get_variation (font, metrics_tag));
}
#endif

#endif

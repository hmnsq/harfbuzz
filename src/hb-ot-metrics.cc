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

#અન્તર્ભાવ "hb.hh"

#અન્તર્ભાવ "hb-ot-var-mvar-table.hh"
#અન્તર્ભાવ "hb-ot-gasp-table.hh" // Just so we compile it; unused otherwise.
#અન્તર્ભાવ "hb-ot-os2-table.hh"
#અન્તર્ભાવ "hb-ot-post-table.hh"
#અન્તર્ભાવ "hb-ot-hhea-table.hh"
#અન્તર્ભાવ "hb-ot-metrics.hh"
#અન્તર્ભાવ "hb-ot-face.hh"


/**
 * SECTION:hb-ot-metrics
 * @title: hb-ot-metrics
 * @short_description: OpenType Metrics
 * @include: hb-ot.h
 *
 * Functions for fetching metrics from fonts.
 **/

સ્થિર દશાંશ
_fix_ascender_descender (દશાંશ value, hb_ot_metrics_tag_t metrics_tag)
{
  જો (metrics_tag == HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER ||
      metrics_tag == HB_OT_METRICS_TAG_VERTICAL_ASCENDER)
    પાછા fabs ((દ્વિગુણ) value);
  જો (metrics_tag == HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER ||
      metrics_tag == HB_OT_METRICS_TAG_VERTICAL_DESCENDER)
    પાછા -fabs ((દ્વિગુણ) value);
  પાછા value;
}

/* The common part of _get_position logic needed on hb-ot-font and here
   to be able to have slim builds without the not always needed parts */
તાર્કિક
_hb_ot_metrics_get_position_common (hb_font_t           *font,
				    hb_ot_metrics_tag_t  metrics_tag,
				    hb_position_t       *position     /* OUT.  May be NULL. */)
{
  hb_face_t *face = font->face;
  પસંદગી ((unsigned) metrics_tag)
  {
#ifndef HB_NO_VAR
#define GET_VAR face->table.MVAR->get_var (metrics_tag, font->coords, font->અંક_coords)
#નહીંતર
#define GET_VAR .0f
#endif
#define GET_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((શૂન્ય) (position && (*position = font->em_scalef_y (_fix_ascender_descender ( \
      face->table.TABLE->ATTR + GET_VAR, metrics_tag)))), સાચું))
#define GET_HEA_METRIC_X(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((શૂન્ય) (position && (*position = font->em_scalef_x (_fix_ascender_descender ( \
      HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR, metrics_tag)))), સાચું))
#define GET_HEA_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((શૂન્ય) (position && (*position = font->em_scalef_y (_fix_ascender_descender ( \
      HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR, metrics_tag)))), સાચું))

  સ્થિતિ HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER:
    પાછા (face->table.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, sTypoAscender)) ||
	   GET_HEA_METRIC_Y (hhea, ascender);
  સ્થિતિ HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER:
    પાછા (face->table.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, sTypoDescender)) ||
	   GET_HEA_METRIC_Y (hhea, descender);
  સ્થિતિ HB_OT_METRICS_TAG_HORIZONTAL_લીટી_GAP:
    પાછા (face->table.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, s_Typo_લીટી_Gap)) ||
	   GET_HEA_METRIC_Y (hhea, લીટી_Gap);

#ifndef HB_NO_VERTICAL
  સ્થિતિ HB_OT_METRICS_TAG_VERTICAL_ASCENDER:  પાછા GET_HEA_METRIC_X (vhea, ascender);
  સ્થિતિ HB_OT_METRICS_TAG_VERTICAL_DESCENDER: પાછા GET_HEA_METRIC_X (vhea, descender);
  સ્થિતિ HB_OT_METRICS_TAG_VERTICAL_લીટી_GAP:  પાછા GET_HEA_METRIC_X (vhea, લીટી_Gap);
#endif

#undef GET_HEA_METRIC_Y
#undef GET_HEA_METRIC_X
#undef GET_METRIC_Y
#undef GET_VAR
  મૂળભૂત:                               પ્રતિજ્ઞા (0); પાછા ખોટું;
  }
}

#ifndef HB_NO_METRICS

#જો 0
સ્થિર તાર્કિક
_get_gasp (hb_face_t *face, દશાંશ *result, hb_ot_metrics_tag_t metrics_tag)
{
  અચલ OT::GaspRange& range = face->table.gasp->get_gasp_range (metrics_tag - HB_TAG ('g','s','p','0'));
  જો (&range == &Null (OT::GaspRange)) પાછા ખોટું;
  જો (result) *result = range.rangeMaxPPEM + font->face->table.MVAR->get_var (metrics_tag, font->coords, font->અંક_coords);
  પાછા સાચું;
}
#endif

/* Private tags for https://github.com/harfbuzz/harfbuzz/issues/1866 */
#define _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_OS2   HB_TAG ('O','a','s','c')
#define _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_HHEA  HB_TAG ('H','a','s','c')
#define _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_OS2  HB_TAG ('O','d','s','c')
#define _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_HHEA HB_TAG ('H','d','s','c')
#define HB_OT_METRICS_TAG_HORIZONTAL_લીટી_GAP_OS2   HB_TAG ('O','l','g','p')
#define HB_OT_METRICS_TAG_HORIZONTAL_લીટી_GAP_HHEA  HB_TAG ('H','l','g','p')

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
  પસંદગી ((unsigned) metrics_tag)
  {
  સ્થિતિ HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER:
  સ્થિતિ HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER:
  સ્થિતિ HB_OT_METRICS_TAG_HORIZONTAL_લીટી_GAP:
  સ્થિતિ HB_OT_METRICS_TAG_VERTICAL_ASCENDER:
  સ્થિતિ HB_OT_METRICS_TAG_VERTICAL_DESCENDER:
  સ્થિતિ HB_OT_METRICS_TAG_VERTICAL_લીટી_GAP:           પાછા _hb_ot_metrics_get_position_common (font, metrics_tag, position);
#ifndef HB_NO_VAR
#define GET_VAR hb_ot_metrics_get_variation (font, metrics_tag)
#નહીંતર
#define GET_VAR 0
#endif
#define GET_METRIC_X(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((શૂન્ય) (position && (*position = font->em_scalef_x (face->table.TABLE->ATTR + GET_VAR))), સાચું))
#define GET_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((શૂન્ય) (position && (*position = font->em_scalef_y (face->table.TABLE->ATTR + GET_VAR))), સાચું))
#define GET_HEA_METRIC_X(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((શૂન્ય) (position && (*position = font->em_scalef_x (HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR))), સાચું))
#define GET_HEA_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_data () && \
    ((શૂન્ય) (position && (*position = font->em_scalef_y (HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR))), સાચું))
  સ્થિતિ HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_ASCENT:  પાછા GET_METRIC_Y (OS2, usWinAscent);
  સ્થિતિ HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_DESCENT: પાછા GET_METRIC_Y (OS2, usWinDescent);

  સ્થિતિ HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE:
  સ્થિતિ HB_OT_METRICS_TAG_HORIZONTAL_CARET_RUN:
  {
    unsigned mult = 1u;

    જો (font->slant)
    {
      unsigned rise = HB_DUAL_GET (*face->table.hhea, table->caretSlopeRise);
      unsigned upem = face->get_upem ();
      mult = (rise && rise < upem) ? hb_min (upem / rise, 256u) : 1u;
    }

    જો (metrics_tag == HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE)
    {
      તાર્કિક ret = GET_HEA_METRIC_Y (hhea, caretSlopeRise);

      જો (position)
	*position = hb_clamp_to<hb_position_t> ((int64_t) *position * mult);

      પાછા ret;
    }
    નહીંતર
    {
      hb_position_t rise = 0;

      જો (font->slant && position && GET_HEA_METRIC_Y (hhea, caretSlopeRise))
	rise = *position;

      તાર્કિક ret = GET_HEA_METRIC_X (hhea, caretSlopeRun);

      જો (position)
      {
	*position = hb_clamp_to<hb_position_t> ((int64_t) *position * mult);

	જો (font->slant)
	  *position = hb_saturate_add (*position,
				       hb_clamp_to<hb_position_t> (roundf (mult * font->slant_xy * rise)));
      }

      પાછા ret;
    }
  }
  સ્થિતિ HB_OT_METRICS_TAG_HORIZONTAL_CARET_OFFSET:     પાછા GET_HEA_METRIC_X (hhea, caretOffset);

#ifndef HB_NO_VERTICAL
  સ્થિતિ HB_OT_METRICS_TAG_VERTICAL_CARET_RISE:         પાછા GET_HEA_METRIC_X (vhea, caretSlopeRise);
  સ્થિતિ HB_OT_METRICS_TAG_VERTICAL_CARET_RUN:          પાછા GET_HEA_METRIC_Y (vhea, caretSlopeRun);
  સ્થિતિ HB_OT_METRICS_TAG_VERTICAL_CARET_OFFSET:       પાછા GET_HEA_METRIC_Y (vhea, caretOffset);
#endif
  સ્થિતિ HB_OT_METRICS_TAG_X_HEIGHT:                    પાછા GET_METRIC_Y (OS2->v2 (), sxHeight);
  સ્થિતિ HB_OT_METRICS_TAG_CAP_HEIGHT:                  પાછા GET_METRIC_Y (OS2->v2 (), sCapHeight);
  સ્થિતિ HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_SIZE:         પાછા GET_METRIC_X (OS2, ySubscriptXSize);
  સ્થિતિ HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_SIZE:         પાછા GET_METRIC_Y (OS2, ySubscriptYSize);
  સ્થિતિ HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_OFFSET:       પાછા GET_METRIC_X (OS2, ySubscriptXOffset);
  સ્થિતિ HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_OFFSET:       પાછા GET_METRIC_Y (OS2, ySubscriptYOffset);
  સ્થિતિ HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_SIZE:       પાછા GET_METRIC_X (OS2, ySuperscriptXSize);
  સ્થિતિ HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_SIZE:       પાછા GET_METRIC_Y (OS2, ySuperscriptYSize);
  સ્થિતિ HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_OFFSET:     પાછા GET_METRIC_X (OS2, ySuperscriptXOffset);
  સ્થિતિ HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_OFFSET:     પાછા GET_METRIC_Y (OS2, ySuperscriptYOffset);
  સ્થિતિ HB_OT_METRICS_TAG_STRIKEOUT_SIZE:              પાછા GET_METRIC_Y (OS2, yStrikeoutSize);
  સ્થિતિ HB_OT_METRICS_TAG_STRIKEOUT_OFFSET:            પાછા GET_METRIC_Y (OS2, yStrikeoutPosition);
  સ્થિતિ HB_OT_METRICS_TAG_UNDERLINE_SIZE:              પાછા GET_METRIC_Y (post->table, underlineThickness);
  સ્થિતિ HB_OT_METRICS_TAG_UNDERLINE_OFFSET:            પાછા GET_METRIC_Y (post->table, underlinePosition);

  /* Private tags */
  સ્થિતિ _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_OS2:    પાછા GET_METRIC_Y (OS2, sTypoAscender);
  સ્થિતિ _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_HHEA:   પાછા GET_HEA_METRIC_Y (hhea, ascender);
  સ્થિતિ _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_OS2:   પાછા GET_METRIC_Y (OS2, sTypoDescender);
  સ્થિતિ _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_HHEA:  પાછા GET_HEA_METRIC_Y (hhea, descender);
  સ્થિતિ HB_OT_METRICS_TAG_HORIZONTAL_લીટી_GAP_OS2:    પાછા GET_METRIC_Y (OS2, s_Typo_લીટી_Gap);
  સ્થિતિ HB_OT_METRICS_TAG_HORIZONTAL_લીટી_GAP_HHEA:   પાછા GET_HEA_METRIC_Y (hhea, લીટી_Gap);
#undef GET_HEA_METRIC_Y
#undef GET_HEA_METRIC_X
#undef GET_METRIC_Y
#undef GET_METRIC_X
#undef GET_VAR
  મૂળભૂત:                                        પાછા ખોટું;
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
શૂન્ય
hb_ot_metrics_get_position_with_fallback (hb_font_t           *font,
					  hb_ot_metrics_tag_t  metrics_tag,
					  hb_position_t       *position     /* OUT */)
{
  hb_font_extents_t font_extents;
  hb_codepoint_t glyph;
  hb_glyph_extents_t extents;

  જો (hb_ot_metrics_get_position (font, metrics_tag, position))
    {
      જો ((metrics_tag != HB_OT_METRICS_TAG_STRIKEOUT_SIZE &&
           metrics_tag != HB_OT_METRICS_TAG_UNDERLINE_SIZE) ||
          *position != 0)
        પાછા;
    }

  પસંદગી (metrics_tag)
  {
  સ્થિતિ HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER:
  સ્થિતિ HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_ASCENT:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.ascender;
    તોડો;

  સ્થિતિ HB_OT_METRICS_TAG_VERTICAL_ASCENDER:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ascender;
    તોડો;

  સ્થિતિ HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER:
  સ્થિતિ HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_DESCENT:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.descender;
    તોડો;

  સ્થિતિ HB_OT_METRICS_TAG_VERTICAL_DESCENDER:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ascender;
    તોડો;

  સ્થિતિ HB_OT_METRICS_TAG_HORIZONTAL_લીટી_GAP:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.લીટી_gap;
    તોડો;

  સ્થિતિ HB_OT_METRICS_TAG_VERTICAL_લીટી_GAP:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.લીટી_gap;
    તોડો;

  સ્થિતિ HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE:
  સ્થિતિ HB_OT_METRICS_TAG_VERTICAL_CARET_RISE:
    *position = 1;
    તોડો;

  સ્થિતિ HB_OT_METRICS_TAG_HORIZONTAL_CARET_RUN:
  સ્થિતિ HB_OT_METRICS_TAG_VERTICAL_CARET_RUN:
    *position = 0;
    તોડો;

  સ્થિતિ HB_OT_METRICS_TAG_HORIZONTAL_CARET_OFFSET:
  સ્થિતિ HB_OT_METRICS_TAG_VERTICAL_CARET_OFFSET:
    *position = 0;
    તોડો;

  સ્થિતિ HB_OT_METRICS_TAG_X_HEIGHT:
    જો (hb_font_get_nominal_glyph (font, 'x', &glyph) &&
        hb_font_get_glyph_extents (font, glyph, &extents))
      *position = extents.y_bearing;
    નહીંતર
      *position = font->y_scale / 2;
    તોડો;

  સ્થિતિ HB_OT_METRICS_TAG_CAP_HEIGHT:
    જો (hb_font_get_nominal_glyph (font, 'O', &glyph) &&
        hb_font_get_glyph_extents (font, glyph, &extents))
      *position = hb_clamp_to<hb_position_t> ((int64_t) extents.height +
					      2 * (int64_t) extents.y_bearing);
    નહીંતર
      *position = hb_clamp_to<hb_position_t> ((int64_t) font->y_scale * 2 / 3);
    તોડો;

  સ્થિતિ HB_OT_METRICS_TAG_STRIKEOUT_SIZE:
  સ્થિતિ HB_OT_METRICS_TAG_UNDERLINE_SIZE:
    *position = font->y_scale / 18;
    તોડો;

  સ્થિતિ HB_OT_METRICS_TAG_STRIKEOUT_OFFSET:
    {
      hb_position_t ascender;
      hb_ot_metrics_get_position_with_fallback (font,
                                                HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER,
                                                &ascender);
      *position = ascender / 2;
    }
    તોડો;

  સ્થિતિ HB_OT_METRICS_TAG_UNDERLINE_OFFSET:
    *position = hb_saturate_neg (font->y_scale) / 18;
    તોડો;

  સ્થિતિ HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_SIZE:
  સ્થિતિ HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_SIZE:
    *position = hb_clamp_to<hb_position_t> ((int64_t) font->x_scale * 10 / 12);
    તોડો;

  સ્થિતિ HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_SIZE:
  સ્થિતિ HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_SIZE:
    *position = hb_clamp_to<hb_position_t> ((int64_t) font->y_scale * 10 / 12);
    તોડો;

  સ્થિતિ HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_OFFSET:
  સ્થિતિ HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_OFFSET:
    *position = 0;
    તોડો;

  સ્થિતિ HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_OFFSET:
  સ્થિતિ HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_OFFSET:
    *position = font->y_scale / 5;
    તોડો;

  સ્થિતિ HB_OT_METRICS_TAG_MAX_કિંમત:
  મૂળભૂત:
    *position = 0;
    તોડો;
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
દશાંશ
hb_ot_metrics_get_variation (hb_font_t *font, hb_ot_metrics_tag_t metrics_tag)
{
  પાછા font->face->table.MVAR->get_var (metrics_tag, font->coords, font->અંક_coords);
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
  પાછા font->em_scalef_x (hb_ot_metrics_get_variation (font, metrics_tag));
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
  પાછા font->em_scalef_y (hb_ot_metrics_get_variation (font, metrics_tag));
}
#endif

#endif

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

#অন্তর্ভাব "hb.hh"

#অন্তর্ভাব "hb-ot-var-mvar-table.hh"
#অন্তর্ভাব "hb-ot-gasp-table.hh" // Just so we compile it; unused otherwise.
#অন্তর্ভাব "hb-ot-os2-table.hh"
#অন্তর্ভাব "hb-ot-post-table.hh"
#অন্তর্ভাব "hb-ot-hhea-table.hh"
#অন্তর্ভাব "hb-ot-metrics.hh"
#অন্তর্ভাব "hb-ot-face.hh"


/**
 * SECTION:hb-ot-metrics
 * @title: hb-ot-metrics
 * @short_description: OpenType Metrics
 * @include: hb-ot.h
 *
 * Functions for fetching metrics from fonts.
 **/

স্থির দশমিক
_fix_ascender_descender (দশমিক value, hb_ot_metrics_tag_t metrics_tag)
{
  যদি (metrics_tag == HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER ||
      metrics_tag == HB_OT_METRICS_TAG_VERTICAL_ASCENDER)
    ফেরত fabs ((দ্বিগুণ) value);
  যদি (metrics_tag == HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER ||
      metrics_tag == HB_OT_METRICS_TAG_VERTICAL_DESCENDER)
    ফেরত -fabs ((দ্বিগুণ) value);
  ফেরত value;
}

/* The common part of _get_position logic needed on hb-ot-font and here
   to be able to have slim builds without the not always needed parts */
যৌক্তিক
_hb_ot_metrics_get_position_common (hb_font_t           *font,
				    hb_ot_metrics_tag_t  metrics_tag,
				    hb_position_t       *position     /* OUT.  May be NULL. */)
{
  hb_face_t *face = font->face;
  নির্বাচন ((unsigned) metrics_tag)
  {
#ifndef HB_NO_VAR
#define GET_VAR face->table.MVAR->get_var (metrics_tag, font->coords, font->অঙ্ক_coords)
#অন্যথা
#define GET_VAR .0f
#endif
#define GET_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_তথ্য () && \
    ((শূন্য) (position && (*position = font->em_scalef_y (_fix_ascender_descender ( \
      face->table.TABLE->ATTR + GET_VAR, metrics_tag)))), সত্য))
#define GET_HEA_METRIC_X(TABLE, ATTR) \
  (face->table.TABLE->has_তথ্য () && \
    ((শূন্য) (position && (*position = font->em_scalef_x (_fix_ascender_descender ( \
      HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR, metrics_tag)))), সত্য))
#define GET_HEA_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_তথ্য () && \
    ((শূন্য) (position && (*position = font->em_scalef_y (_fix_ascender_descender ( \
      HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR, metrics_tag)))), সত্য))

  ক্ষেত্র HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER:
    ফেরত (face->table.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, sTypoAscender)) ||
	   GET_HEA_METRIC_Y (hhea, ascender);
  ক্ষেত্র HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER:
    ফেরত (face->table.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, sTypoDescender)) ||
	   GET_HEA_METRIC_Y (hhea, descender);
  ক্ষেত্র HB_OT_METRICS_TAG_HORIZONTAL_সারি_GAP:
    ফেরত (face->table.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, s_Typo_সারি_Gap)) ||
	   GET_HEA_METRIC_Y (hhea, সারি_Gap);

#ifndef HB_NO_VERTICAL
  ক্ষেত্র HB_OT_METRICS_TAG_VERTICAL_ASCENDER:  ফেরত GET_HEA_METRIC_X (vhea, ascender);
  ক্ষেত্র HB_OT_METRICS_TAG_VERTICAL_DESCENDER: ফেরত GET_HEA_METRIC_X (vhea, descender);
  ক্ষেত্র HB_OT_METRICS_TAG_VERTICAL_সারি_GAP:  ফেরত GET_HEA_METRIC_X (vhea, সারি_Gap);
#endif

#undef GET_HEA_METRIC_Y
#undef GET_HEA_METRIC_X
#undef GET_METRIC_Y
#undef GET_VAR
  পূর্বনির্ধারিত:                               প্রতিজ্ঞা (0); ফেরত মিথ্যা;
  }
}

#ifndef HB_NO_METRICS

#যদি 0
স্থির যৌক্তিক
_get_gasp (hb_face_t *face, দশমিক *ফলাফল, hb_ot_metrics_tag_t metrics_tag)
{
  ধ্রুবক OT::GaspRange& range = face->table.gasp->get_gasp_range (metrics_tag - HB_TAG ('g','s','p','0'));
  যদি (&range == &Null (OT::GaspRange)) ফেরত মিথ্যা;
  যদি (ফলাফল) *ফলাফল = range.rangeMaxPPEM + font->face->table.MVAR->get_var (metrics_tag, font->coords, font->অঙ্ক_coords);
  ফেরত সত্য;
}
#endif

/* Private tags for https://github.com/harfbuzz/harfbuzz/issues/1866 */
#define _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_OS2   HB_TAG ('O','a','s','c')
#define _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_HHEA  HB_TAG ('H','a','s','c')
#define _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_OS2  HB_TAG ('O','d','s','c')
#define _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_HHEA HB_TAG ('H','d','s','c')
#define HB_OT_METRICS_TAG_HORIZONTAL_সারি_GAP_OS2   HB_TAG ('O','l','g','p')
#define HB_OT_METRICS_TAG_HORIZONTAL_সারি_GAP_HHEA  HB_TAG ('H','l','g','p')

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
  নির্বাচন ((unsigned) metrics_tag)
  {
  ক্ষেত্র HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER:
  ক্ষেত্র HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER:
  ক্ষেত্র HB_OT_METRICS_TAG_HORIZONTAL_সারি_GAP:
  ক্ষেত্র HB_OT_METRICS_TAG_VERTICAL_ASCENDER:
  ক্ষেত্র HB_OT_METRICS_TAG_VERTICAL_DESCENDER:
  ক্ষেত্র HB_OT_METRICS_TAG_VERTICAL_সারি_GAP:           ফেরত _hb_ot_metrics_get_position_common (font, metrics_tag, position);
#ifndef HB_NO_VAR
#define GET_VAR hb_ot_metrics_get_variation (font, metrics_tag)
#অন্যথা
#define GET_VAR 0
#endif
#define GET_METRIC_X(TABLE, ATTR) \
  (face->table.TABLE->has_তথ্য () && \
    ((শূন্য) (position && (*position = font->em_scalef_x (face->table.TABLE->ATTR + GET_VAR))), সত্য))
#define GET_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_তথ্য () && \
    ((শূন্য) (position && (*position = font->em_scalef_y (face->table.TABLE->ATTR + GET_VAR))), সত্য))
#define GET_HEA_METRIC_X(TABLE, ATTR) \
  (face->table.TABLE->has_তথ্য () && \
    ((শূন্য) (position && (*position = font->em_scalef_x (HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR))), সত্য))
#define GET_HEA_METRIC_Y(TABLE, ATTR) \
  (face->table.TABLE->has_তথ্য () && \
    ((শূন্য) (position && (*position = font->em_scalef_y (HB_DUAL_GET (*face->table.TABLE, table->ATTR) + GET_VAR))), সত্য))
  ক্ষেত্র HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_ASCENT:  ফেরত GET_METRIC_Y (OS2, usWinAscent);
  ক্ষেত্র HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_DESCENT: ফেরত GET_METRIC_Y (OS2, usWinDescent);

  ক্ষেত্র HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE:
  ক্ষেত্র HB_OT_METRICS_TAG_HORIZONTAL_CARET_RUN:
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
      যৌক্তিক ret = GET_HEA_METRIC_Y (hhea, caretSlopeRise);

      যদি (position)
	*position = hb_clamp_to<hb_position_t> ((int64_t) *position * mult);

      ফেরত ret;
    }
    অন্যথা
    {
      hb_position_t rise = 0;

      যদি (font->slant && position && GET_HEA_METRIC_Y (hhea, caretSlopeRise))
	rise = *position;

      যৌক্তিক ret = GET_HEA_METRIC_X (hhea, caretSlopeRun);

      যদি (position)
      {
	*position = hb_clamp_to<hb_position_t> ((int64_t) *position * mult);

	যদি (font->slant)
	  *position = hb_saturate_add (*position,
				       hb_clamp_to<hb_position_t> (roundf (mult * font->slant_xy * rise)));
      }

      ফেরত ret;
    }
  }
  ক্ষেত্র HB_OT_METRICS_TAG_HORIZONTAL_CARET_OFFSET:     ফেরত GET_HEA_METRIC_X (hhea, caretOffset);

#ifndef HB_NO_VERTICAL
  ক্ষেত্র HB_OT_METRICS_TAG_VERTICAL_CARET_RISE:         ফেরত GET_HEA_METRIC_X (vhea, caretSlopeRise);
  ক্ষেত্র HB_OT_METRICS_TAG_VERTICAL_CARET_RUN:          ফেরত GET_HEA_METRIC_Y (vhea, caretSlopeRun);
  ক্ষেত্র HB_OT_METRICS_TAG_VERTICAL_CARET_OFFSET:       ফেরত GET_HEA_METRIC_Y (vhea, caretOffset);
#endif
  ক্ষেত্র HB_OT_METRICS_TAG_X_HEIGHT:                    ফেরত GET_METRIC_Y (OS2->v2 (), sxHeight);
  ক্ষেত্র HB_OT_METRICS_TAG_CAP_HEIGHT:                  ফেরত GET_METRIC_Y (OS2->v2 (), sCapHeight);
  ক্ষেত্র HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_আকার:         ফেরত GET_METRIC_X (OS2, ySubscriptXSize);
  ক্ষেত্র HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_আকার:         ফেরত GET_METRIC_Y (OS2, ySubscriptYSize);
  ক্ষেত্র HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_OFFSET:       ফেরত GET_METRIC_X (OS2, ySubscriptXOffset);
  ক্ষেত্র HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_OFFSET:       ফেরত GET_METRIC_Y (OS2, ySubscriptYOffset);
  ক্ষেত্র HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_আকার:       ফেরত GET_METRIC_X (OS2, ySuperscriptXSize);
  ক্ষেত্র HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_আকার:       ফেরত GET_METRIC_Y (OS2, ySuperscriptYSize);
  ক্ষেত্র HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_OFFSET:     ফেরত GET_METRIC_X (OS2, ySuperscriptXOffset);
  ক্ষেত্র HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_OFFSET:     ফেরত GET_METRIC_Y (OS2, ySuperscriptYOffset);
  ক্ষেত্র HB_OT_METRICS_TAG_STRIKEOUT_আকার:              ফেরত GET_METRIC_Y (OS2, y_Strikeout_আকার);
  ক্ষেত্র HB_OT_METRICS_TAG_STRIKEOUT_OFFSET:            ফেরত GET_METRIC_Y (OS2, yStrikeoutPosition);
  ক্ষেত্র HB_OT_METRICS_TAG_UNDERLINE_আকার:              ফেরত GET_METRIC_Y (post->table, underlineThickness);
  ক্ষেত্র HB_OT_METRICS_TAG_UNDERLINE_OFFSET:            ফেরত GET_METRIC_Y (post->table, underlinePosition);

  /* Private tags */
  ক্ষেত্র _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_OS2:    ফেরত GET_METRIC_Y (OS2, sTypoAscender);
  ক্ষেত্র _HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER_HHEA:   ফেরত GET_HEA_METRIC_Y (hhea, ascender);
  ক্ষেত্র _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_OS2:   ফেরত GET_METRIC_Y (OS2, sTypoDescender);
  ক্ষেত্র _HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER_HHEA:  ফেরত GET_HEA_METRIC_Y (hhea, descender);
  ক্ষেত্র HB_OT_METRICS_TAG_HORIZONTAL_সারি_GAP_OS2:    ফেরত GET_METRIC_Y (OS2, s_Typo_সারি_Gap);
  ক্ষেত্র HB_OT_METRICS_TAG_HORIZONTAL_সারি_GAP_HHEA:   ফেরত GET_HEA_METRIC_Y (hhea, সারি_Gap);
#undef GET_HEA_METRIC_Y
#undef GET_HEA_METRIC_X
#undef GET_METRIC_Y
#undef GET_METRIC_X
#undef GET_VAR
  পূর্বনির্ধারিত:                                        ফেরত মিথ্যা;
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
      যদি ((metrics_tag != HB_OT_METRICS_TAG_STRIKEOUT_আকার &&
           metrics_tag != HB_OT_METRICS_TAG_UNDERLINE_আকার) ||
          *position != 0)
        ফেরত;
    }

  নির্বাচন (metrics_tag)
  {
  ক্ষেত্র HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER:
  ক্ষেত্র HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_ASCENT:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.ascender;
    বিরতি;

  ক্ষেত্র HB_OT_METRICS_TAG_VERTICAL_ASCENDER:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ascender;
    বিরতি;

  ক্ষেত্র HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER:
  ক্ষেত্র HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_DESCENT:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.descender;
    বিরতি;

  ক্ষেত্র HB_OT_METRICS_TAG_VERTICAL_DESCENDER:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ascender;
    বিরতি;

  ক্ষেত্র HB_OT_METRICS_TAG_HORIZONTAL_সারি_GAP:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.সারি_gap;
    বিরতি;

  ক্ষেত্র HB_OT_METRICS_TAG_VERTICAL_সারি_GAP:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.সারি_gap;
    বিরতি;

  ক্ষেত্র HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE:
  ক্ষেত্র HB_OT_METRICS_TAG_VERTICAL_CARET_RISE:
    *position = 1;
    বিরতি;

  ক্ষেত্র HB_OT_METRICS_TAG_HORIZONTAL_CARET_RUN:
  ক্ষেত্র HB_OT_METRICS_TAG_VERTICAL_CARET_RUN:
    *position = 0;
    বিরতি;

  ক্ষেত্র HB_OT_METRICS_TAG_HORIZONTAL_CARET_OFFSET:
  ক্ষেত্র HB_OT_METRICS_TAG_VERTICAL_CARET_OFFSET:
    *position = 0;
    বিরতি;

  ক্ষেত্র HB_OT_METRICS_TAG_X_HEIGHT:
    যদি (hb_font_get_nominal_glyph (font, 'x', &glyph) &&
        hb_font_get_glyph_extents (font, glyph, &extents))
      *position = extents.y_bearing;
    অন্যথা
      *position = font->y_scale / 2;
    বিরতি;

  ক্ষেত্র HB_OT_METRICS_TAG_CAP_HEIGHT:
    যদি (hb_font_get_nominal_glyph (font, 'O', &glyph) &&
        hb_font_get_glyph_extents (font, glyph, &extents))
      *position = hb_clamp_to<hb_position_t> ((int64_t) extents.height +
					      2 * (int64_t) extents.y_bearing);
    অন্যথা
      *position = hb_clamp_to<hb_position_t> ((int64_t) font->y_scale * 2 / 3);
    বিরতি;

  ক্ষেত্র HB_OT_METRICS_TAG_STRIKEOUT_আকার:
  ক্ষেত্র HB_OT_METRICS_TAG_UNDERLINE_আকার:
    *position = font->y_scale / 18;
    বিরতি;

  ক্ষেত্র HB_OT_METRICS_TAG_STRIKEOUT_OFFSET:
    {
      hb_position_t ascender;
      hb_ot_metrics_get_position_with_fallback (font,
                                                HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER,
                                                &ascender);
      *position = ascender / 2;
    }
    বিরতি;

  ক্ষেত্র HB_OT_METRICS_TAG_UNDERLINE_OFFSET:
    *position = hb_saturate_neg (font->y_scale) / 18;
    বিরতি;

  ক্ষেত্র HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_আকার:
  ক্ষেত্র HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_আকার:
    *position = hb_clamp_to<hb_position_t> ((int64_t) font->x_scale * 10 / 12);
    বিরতি;

  ক্ষেত্র HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_আকার:
  ক্ষেত্র HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_আকার:
    *position = hb_clamp_to<hb_position_t> ((int64_t) font->y_scale * 10 / 12);
    বিরতি;

  ক্ষেত্র HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_OFFSET:
  ক্ষেত্র HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_OFFSET:
    *position = 0;
    বিরতি;

  ক্ষেত্র HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_OFFSET:
  ক্ষেত্র HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_OFFSET:
    *position = font->y_scale / 5;
    বিরতি;

  ক্ষেত্র HB_OT_METRICS_TAG_MAX_মান:
  পূর্বনির্ধারিত:
    *position = 0;
    বিরতি;
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
দশমিক
hb_ot_metrics_get_variation (hb_font_t *font, hb_ot_metrics_tag_t metrics_tag)
{
  ফেরত font->face->table.MVAR->get_var (metrics_tag, font->coords, font->অঙ্ক_coords);
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
  ফেরত font->em_scalef_x (hb_ot_metrics_get_variation (font, metrics_tag));
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
  ফেরত font->em_scalef_y (hb_ot_metrics_get_variation (font, metrics_tag));
}
#endif

#endif

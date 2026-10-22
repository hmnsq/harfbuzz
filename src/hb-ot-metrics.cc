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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-var-mvar-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-gasp-table.hh" // Just so we compile it; unused otherwise.
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-os2-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-post-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-hhea-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-metrics.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-face.hh"


/**
 * SECTION:hb-ot-metrics
 * @title: hb-ot-metrics
 * @short_description: OpenType Metrics
 * @include: hb-ot.h
 *
 * Functions for fetching metrics from fonts.
 **/

ਸਥਿਰ ਦਸ਼ਮਲਵ
_fix_ascender_descender (ਦਸ਼ਮਲਵ value, hb_ot_metrics_ਟੈਗ_t metrics_ਟੈਗ)
{
  ਜੇ (metrics_ਟੈਗ == HB_OT_METRICS_ਟੈਗ_HORIZONTAL_ASCENDER ||
      metrics_ਟੈਗ == HB_OT_METRICS_ਟੈਗ_VERTICAL_ASCENDER)
    ਵਾਪਸ fabs ((ਦੁੱਗਣਾ) value);
  ਜੇ (metrics_ਟੈਗ == HB_OT_METRICS_ਟੈਗ_HORIZONTAL_DESCENDER ||
      metrics_ਟੈਗ == HB_OT_METRICS_ਟੈਗ_VERTICAL_DESCENDER)
    ਵਾਪਸ -fabs ((ਦੁੱਗਣਾ) value);
  ਵਾਪਸ value;
}

/* The common part of _get_position logic needed on hb-ot-font and here
   to be able to have slim builds without the not always needed parts */
ਤਾਰਕਿਕ
_hb_ot_metrics_get_position_common (hb_font_t           *font,
				    hb_ot_metrics_ਟੈਗ_t  metrics_ਟੈਗ,
				    hb_position_t       *position     /* OUT.  May be NULL. */)
{
  hb_face_t *face = font->face;
  ਚੋਣ ((ਬਿਨਾਨਿਸ਼ਾਨੀ) metrics_ਟੈਗ)
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GET_VAR face->ਸਾਰਣੀ.MVAR->get_var (metrics_ਟੈਗ, font->coords, font->ਅੰਕ_coords)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GET_VAR .0f
#ਜੇ_ਅੰਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GET_METRIC_Y(ਸਾਰਣੀ_4, ATTR) \
  (face->ਸਾਰਣੀ.ਸਾਰਣੀ_4->has_ਅੰਕੜੇ () && \
    ((ਖਾਲੀ) (position && (*position = font->em_scalef_y (_fix_ascender_descender ( \
      face->ਸਾਰਣੀ.ਸਾਰਣੀ_4->ATTR + GET_VAR, metrics_ਟੈਗ)))), ਸੱਚ))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GET_HEA_METRIC_X(ਸਾਰਣੀ_4, ATTR) \
  (face->ਸਾਰਣੀ.ਸਾਰਣੀ_4->has_ਅੰਕੜੇ () && \
    ((ਖਾਲੀ) (position && (*position = font->em_scalef_x (_fix_ascender_descender ( \
      HB_DUAL_GET (*face->ਸਾਰਣੀ.ਸਾਰਣੀ_4, ਸਾਰਣੀ->ATTR) + GET_VAR, metrics_ਟੈਗ)))), ਸੱਚ))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GET_HEA_METRIC_Y(ਸਾਰਣੀ_4, ATTR) \
  (face->ਸਾਰਣੀ.ਸਾਰਣੀ_4->has_ਅੰਕੜੇ () && \
    ((ਖਾਲੀ) (position && (*position = font->em_scalef_y (_fix_ascender_descender ( \
      HB_DUAL_GET (*face->ਸਾਰਣੀ.ਸਾਰਣੀ_4, ਸਾਰਣੀ->ATTR) + GET_VAR, metrics_ਟੈਗ)))), ਸੱਚ))

  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_HORIZONTAL_ASCENDER:
    ਵਾਪਸ (face->ਸਾਰਣੀ.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, sTypoAscender)) ||
	   GET_HEA_METRIC_Y (hhea, ascender);
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_HORIZONTAL_DESCENDER:
    ਵਾਪਸ (face->ਸਾਰਣੀ.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, sTypoDescender)) ||
	   GET_HEA_METRIC_Y (hhea, descender);
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_HORIZONTAL_ਸਤਰ_GAP:
    ਵਾਪਸ (face->ਸਾਰਣੀ.OS2->use_typo_metrics () && GET_METRIC_Y (OS2, s_Typo_ਸਤਰ_Gap)) ||
	   GET_HEA_METRIC_Y (hhea, ਸਤਰ_Gap);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VERTICAL
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_VERTICAL_ASCENDER:  ਵਾਪਸ GET_HEA_METRIC_X (vhea, ascender);
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_VERTICAL_DESCENDER: ਵਾਪਸ GET_HEA_METRIC_X (vhea, descender);
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_VERTICAL_ਸਤਰ_GAP:  ਵਾਪਸ GET_HEA_METRIC_X (vhea, ਸਤਰ_Gap);
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ GET_HEA_METRIC_Y
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ GET_HEA_METRIC_X
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ GET_METRIC_Y
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ GET_VAR
  ਮੂਲ:                               ਪ੍ਰਤਿਜ੍ਞਾ (0); ਵਾਪਸ ਝੂਠ;
  }
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_METRICS

#ਜੇ 0
ਸਥਿਰ ਤਾਰਕਿਕ
_get_gasp (hb_face_t *face, ਦਸ਼ਮਲਵ *ਨਤੀਜਾ, hb_ot_metrics_ਟੈਗ_t metrics_ਟੈਗ)
{
  ਅਚਲ OT::GaspRange& range = face->ਸਾਰਣੀ.gasp->get_gasp_range (metrics_ਟੈਗ - HB_ਟੈਗ ('g','s','p','0'));
  ਜੇ (&range == &Null (OT::GaspRange)) ਵਾਪਸ ਝੂਠ;
  ਜੇ (ਨਤੀਜਾ) *ਨਤੀਜਾ = range.rangeMaxPPEM + font->face->ਸਾਰਣੀ.MVAR->get_var (metrics_ਟੈਗ, font->coords, font->ਅੰਕ_coords);
  ਵਾਪਸ ਸੱਚ;
}
#ਜੇ_ਅੰਤ

/* Private tags for https://github.com/harfbuzz/harfbuzz/issues/1866 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_METRICS_ਟੈਗ_HORIZONTAL_ASCENDER_OS2   HB_ਟੈਗ ('O','a','s','c')
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_METRICS_ਟੈਗ_HORIZONTAL_ASCENDER_HHEA  HB_ਟੈਗ ('H','a','s','c')
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_METRICS_ਟੈਗ_HORIZONTAL_DESCENDER_OS2  HB_ਟੈਗ ('O','d','s','c')
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_METRICS_ਟੈਗ_HORIZONTAL_DESCENDER_HHEA HB_ਟੈਗ ('H','d','s','c')
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_METRICS_ਟੈਗ_HORIZONTAL_ਸਤਰ_GAP_OS2   HB_ਟੈਗ ('O','l','g','p')
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_METRICS_ਟੈਗ_HORIZONTAL_ਸਤਰ_GAP_HHEA  HB_ਟੈਗ ('H','l','g','p')

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
			    hb_ot_metrics_ਟੈਗ_t  metrics_ਟੈਗ,
			    hb_position_t       *position     /* OUT.  May be NULL. */)
{
  hb_face_t *face = font->face;
  ਚੋਣ ((ਬਿਨਾਨਿਸ਼ਾਨੀ) metrics_ਟੈਗ)
  {
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_HORIZONTAL_ASCENDER:
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_HORIZONTAL_DESCENDER:
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_HORIZONTAL_ਸਤਰ_GAP:
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_VERTICAL_ASCENDER:
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_VERTICAL_DESCENDER:
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_VERTICAL_ਸਤਰ_GAP:           ਵਾਪਸ _hb_ot_metrics_get_position_common (font, metrics_ਟੈਗ, position);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GET_VAR hb_ot_metrics_get_variation (font, metrics_ਟੈਗ)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GET_VAR 0
#ਜੇ_ਅੰਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GET_METRIC_X(ਸਾਰਣੀ_4, ATTR) \
  (face->ਸਾਰਣੀ.ਸਾਰਣੀ_4->has_ਅੰਕੜੇ () && \
    ((ਖਾਲੀ) (position && (*position = font->em_scalef_x (face->ਸਾਰਣੀ.ਸਾਰਣੀ_4->ATTR + GET_VAR))), ਸੱਚ))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GET_METRIC_Y(ਸਾਰਣੀ_4, ATTR) \
  (face->ਸਾਰਣੀ.ਸਾਰਣੀ_4->has_ਅੰਕੜੇ () && \
    ((ਖਾਲੀ) (position && (*position = font->em_scalef_y (face->ਸਾਰਣੀ.ਸਾਰਣੀ_4->ATTR + GET_VAR))), ਸੱਚ))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GET_HEA_METRIC_X(ਸਾਰਣੀ_4, ATTR) \
  (face->ਸਾਰਣੀ.ਸਾਰਣੀ_4->has_ਅੰਕੜੇ () && \
    ((ਖਾਲੀ) (position && (*position = font->em_scalef_x (HB_DUAL_GET (*face->ਸਾਰਣੀ.ਸਾਰਣੀ_4, ਸਾਰਣੀ->ATTR) + GET_VAR))), ਸੱਚ))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GET_HEA_METRIC_Y(ਸਾਰਣੀ_4, ATTR) \
  (face->ਸਾਰਣੀ.ਸਾਰਣੀ_4->has_ਅੰਕੜੇ () && \
    ((ਖਾਲੀ) (position && (*position = font->em_scalef_y (HB_DUAL_GET (*face->ਸਾਰਣੀ.ਸਾਰਣੀ_4, ਸਾਰਣੀ->ATTR) + GET_VAR))), ਸੱਚ))
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_HORIZONTAL_CLIPPING_ASCENT:  ਵਾਪਸ GET_METRIC_Y (OS2, usWinAscent);
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_HORIZONTAL_CLIPPING_DESCENT: ਵਾਪਸ GET_METRIC_Y (OS2, usWinDescent);

  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_HORIZONTAL_CARET_RISE:
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_HORIZONTAL_CARET_ਲੜੀਵਾਰ:
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ mult = 1u;

    ਜੇ (font->slant)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ rise = HB_DUAL_GET (*face->ਸਾਰਣੀ.hhea, ਸਾਰਣੀ->caret_ਢਲਾਨ_Rise);
      ਬਿਨਾਨਿਸ਼ਾਨੀ upem = face->get_upem ();
      mult = (rise && rise < upem) ? hb_min (upem / rise, 256u) : 1u;
    }

    ਜੇ (metrics_ਟੈਗ == HB_OT_METRICS_ਟੈਗ_HORIZONTAL_CARET_RISE)
    {
      ਤਾਰਕਿਕ ret = GET_HEA_METRIC_Y (hhea, caret_ਢਲਾਨ_Rise);

      ਜੇ (position)
	*position = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((int64_t) *position * mult);

      ਵਾਪਸ ret;
    }
    ਨਹੀਂਤਾਂ
    {
      hb_position_t rise = 0;

      ਜੇ (font->slant && position && GET_HEA_METRIC_Y (hhea, caret_ਢਲਾਨ_Rise))
	rise = *position;

      ਤਾਰਕਿਕ ret = GET_HEA_METRIC_X (hhea, caret_ਢਲਾਨ_ਲੜੀਵਾਰ);

      ਜੇ (position)
      {
	*position = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((int64_t) *position * mult);

	ਜੇ (font->slant)
	  *position = hb_saturate_ਜੋੜੋ (*position,
				       hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (roundf (mult * font->slant_xy * rise)));
      }

      ਵਾਪਸ ret;
    }
  }
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_HORIZONTAL_CARET_ਵਿੱਥ:     ਵਾਪਸ GET_HEA_METRIC_X (hhea, caret_ਵਿੱਥ);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VERTICAL
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_VERTICAL_CARET_RISE:         ਵਾਪਸ GET_HEA_METRIC_X (vhea, caret_ਢਲਾਨ_Rise);
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_VERTICAL_CARET_ਲੜੀਵਾਰ:          ਵਾਪਸ GET_HEA_METRIC_Y (vhea, caret_ਢਲਾਨ_ਲੜੀਵਾਰ);
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_VERTICAL_CARET_ਵਿੱਥ:       ਵਾਪਸ GET_HEA_METRIC_Y (vhea, caret_ਵਿੱਥ);
#ਜੇ_ਅੰਤ
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_X_ਉਚਾਈ:                    ਵਾਪਸ GET_METRIC_Y (OS2->v2 (), sx_ਉਚਾਈ);
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_CAP_ਉਚਾਈ:                  ਵਾਪਸ GET_METRIC_Y (OS2->v2 (), s_Cap_ਉਚਾਈ);
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_SUBSCRIPT_EM_X_ਮਾਪ:         ਵਾਪਸ GET_METRIC_X (OS2, ySubscriptXSize);
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_SUBSCRIPT_EM_Y_ਮਾਪ:         ਵਾਪਸ GET_METRIC_Y (OS2, ySubscriptYSize);
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_SUBSCRIPT_EM_X_ਵਿੱਥ:       ਵਾਪਸ GET_METRIC_X (OS2, ySubscriptXOffset);
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_SUBSCRIPT_EM_Y_ਵਿੱਥ:       ਵਾਪਸ GET_METRIC_Y (OS2, ySubscriptYOffset);
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_SUPERSCRIPT_EM_X_ਮਾਪ:       ਵਾਪਸ GET_METRIC_X (OS2, ySuperscriptXSize);
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_SUPERSCRIPT_EM_Y_ਮਾਪ:       ਵਾਪਸ GET_METRIC_Y (OS2, ySuperscriptYSize);
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_SUPERSCRIPT_EM_X_ਵਿੱਥ:     ਵਾਪਸ GET_METRIC_X (OS2, ySuperscriptXOffset);
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_SUPERSCRIPT_EM_Y_ਵਿੱਥ:     ਵਾਪਸ GET_METRIC_Y (OS2, ySuperscriptYOffset);
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_STRIKEOUT_ਮਾਪ:              ਵਾਪਸ GET_METRIC_Y (OS2, y_Strikeout_ਮਾਪ);
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_STRIKEOUT_ਵਿੱਥ:            ਵਾਪਸ GET_METRIC_Y (OS2, yStrikeoutPosition);
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_UNDERLINE_ਮਾਪ:              ਵਾਪਸ GET_METRIC_Y (post->ਸਾਰਣੀ, underlineThickness);
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_UNDERLINE_ਵਿੱਥ:            ਵਾਪਸ GET_METRIC_Y (post->ਸਾਰਣੀ, underlinePosition);

  /* Private tags */
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_HORIZONTAL_ASCENDER_OS2:    ਵਾਪਸ GET_METRIC_Y (OS2, sTypoAscender);
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_HORIZONTAL_ASCENDER_HHEA:   ਵਾਪਸ GET_HEA_METRIC_Y (hhea, ascender);
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_HORIZONTAL_DESCENDER_OS2:   ਵਾਪਸ GET_METRIC_Y (OS2, sTypoDescender);
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_HORIZONTAL_DESCENDER_HHEA:  ਵਾਪਸ GET_HEA_METRIC_Y (hhea, descender);
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_HORIZONTAL_ਸਤਰ_GAP_OS2:    ਵਾਪਸ GET_METRIC_Y (OS2, s_Typo_ਸਤਰ_Gap);
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_HORIZONTAL_ਸਤਰ_GAP_HHEA:   ਵਾਪਸ GET_HEA_METRIC_Y (hhea, ਸਤਰ_Gap);
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ GET_HEA_METRIC_Y
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ GET_HEA_METRIC_X
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ GET_METRIC_Y
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ GET_METRIC_X
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ GET_VAR
  ਮੂਲ:                                        ਵਾਪਸ ਝੂਠ;
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
ਖਾਲੀ
hb_ot_metrics_get_position_with_fallback (hb_font_t           *font,
					  hb_ot_metrics_ਟੈਗ_t  metrics_ਟੈਗ,
					  hb_position_t       *position     /*ਬਾਹਰਲਾ*/)
{
  hb_font_extents_t font_extents;
  hb_codepoint_t glyph;
  hb_glyph_extents_t extents;

  ਜੇ (hb_ot_metrics_get_position (font, metrics_ਟੈਗ, position))
    {
      ਜੇ ((metrics_ਟੈਗ != HB_OT_METRICS_ਟੈਗ_STRIKEOUT_ਮਾਪ &&
           metrics_ਟੈਗ != HB_OT_METRICS_ਟੈਗ_UNDERLINE_ਮਾਪ) ||
          *position != 0)
        ਵਾਪਸ;
    }

  ਚੋਣ (metrics_ਟੈਗ)
  {
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_HORIZONTAL_ASCENDER:
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_HORIZONTAL_CLIPPING_ASCENT:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.ascender;
    ਰੋਕੋ;

  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_VERTICAL_ASCENDER:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ascender;
    ਰੋਕੋ;

  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_HORIZONTAL_DESCENDER:
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_HORIZONTAL_CLIPPING_DESCENT:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.descender;
    ਰੋਕੋ;

  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_VERTICAL_DESCENDER:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ascender;
    ਰੋਕੋ;

  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_HORIZONTAL_ਸਤਰ_GAP:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    *position = font_extents.ਸਤਰ_gap;
    ਰੋਕੋ;

  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_VERTICAL_ਸਤਰ_GAP:
    hb_font_get_extents_for_direction (font, HB_DIRECTION_TTB, &font_extents);
    *position = font_extents.ਸਤਰ_gap;
    ਰੋਕੋ;

  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_HORIZONTAL_CARET_RISE:
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_VERTICAL_CARET_RISE:
    *position = 1;
    ਰੋਕੋ;

  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_HORIZONTAL_CARET_ਲੜੀਵਾਰ:
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_VERTICAL_CARET_ਲੜੀਵਾਰ:
    *position = 0;
    ਰੋਕੋ;

  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_HORIZONTAL_CARET_ਵਿੱਥ:
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_VERTICAL_CARET_ਵਿੱਥ:
    *position = 0;
    ਰੋਕੋ;

  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_X_ਉਚਾਈ:
    ਜੇ (hb_font_get_nominal_glyph (font, 'x', &glyph) &&
        hb_font_get_glyph_extents (font, glyph, &extents))
      *position = extents.y_bearing;
    ਨਹੀਂਤਾਂ
      *position = font->y_scale / 2;
    ਰੋਕੋ;

  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_CAP_ਉਚਾਈ:
    ਜੇ (hb_font_get_nominal_glyph (font, 'O', &glyph) &&
        hb_font_get_glyph_extents (font, glyph, &extents))
      *position = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((int64_t) extents.ਉਚਾਈ +
					      2 * (int64_t) extents.y_bearing);
    ਨਹੀਂਤਾਂ
      *position = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((int64_t) font->y_scale * 2 / 3);
    ਰੋਕੋ;

  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_STRIKEOUT_ਮਾਪ:
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_UNDERLINE_ਮਾਪ:
    *position = font->y_scale / 18;
    ਰੋਕੋ;

  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_STRIKEOUT_ਵਿੱਥ:
    {
      hb_position_t ascender;
      hb_ot_metrics_get_position_with_fallback (font,
                                                HB_OT_METRICS_ਟੈਗ_HORIZONTAL_ASCENDER,
                                                &ascender);
      *position = ascender / 2;
    }
    ਰੋਕੋ;

  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_UNDERLINE_ਵਿੱਥ:
    *position = hb_saturate_neg (font->y_scale) / 18;
    ਰੋਕੋ;

  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_SUBSCRIPT_EM_X_ਮਾਪ:
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_SUPERSCRIPT_EM_X_ਮਾਪ:
    *position = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((int64_t) font->x_scale * 10 / 12);
    ਰੋਕੋ;

  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_SUBSCRIPT_EM_Y_ਮਾਪ:
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_SUPERSCRIPT_EM_Y_ਮਾਪ:
    *position = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((int64_t) font->y_scale * 10 / 12);
    ਰੋਕੋ;

  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_SUBSCRIPT_EM_X_ਵਿੱਥ:
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_SUPERSCRIPT_EM_X_ਵਿੱਥ:
    *position = 0;
    ਰੋਕੋ;

  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_SUBSCRIPT_EM_Y_ਵਿੱਥ:
  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_SUPERSCRIPT_EM_Y_ਵਿੱਥ:
    *position = font->y_scale / 5;
    ਰੋਕੋ;

  ਹਾਲਤ HB_OT_METRICS_ਟੈਗ_MAX_ਮੁੱਲ:
  ਮੂਲ:
    *position = 0;
    ਰੋਕੋ;
  }
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
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
ਦਸ਼ਮਲਵ
hb_ot_metrics_get_variation (hb_font_t *font, hb_ot_metrics_ਟੈਗ_t metrics_ਟੈਗ)
{
  ਵਾਪਸ font->face->ਸਾਰਣੀ.MVAR->get_var (metrics_ਟੈਗ, font->coords, font->ਅੰਕ_coords);
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
hb_ot_metrics_get_x_variation (hb_font_t *font, hb_ot_metrics_ਟੈਗ_t metrics_ਟੈਗ)
{
  ਵਾਪਸ font->em_scalef_x (hb_ot_metrics_get_variation (font, metrics_ਟੈਗ));
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
hb_ot_metrics_get_y_variation (hb_font_t *font, hb_ot_metrics_ਟੈਗ_t metrics_ਟੈਗ)
{
  ਵਾਪਸ font->em_scalef_y (hb_ot_metrics_get_variation (font, metrics_ਟੈਗ));
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ

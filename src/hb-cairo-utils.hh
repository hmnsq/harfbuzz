/*
 * Copyright © 2022  Red Hat, Inc
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
 *
 * Google Author(s): Matthias Clasen
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_CAIRO_UTILS_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_CAIRO_UTILS_H

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-cairo.h"


ਬਣਤਰ hb_cairo_budget_t
{
  /* Cairo serializes glyph-cache population for a scaled font.  Keeping the
   * owner there lets recursive color-glyph callbacks share one session. */
  int64_t budget = HB_BUDGET_DEFAULT;
  int64_t budget_ਬਚਿਆ = HB_BUDGET_GLYPH;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਰਗਰਮ = 0;

  ਖਾਲੀ enter ()
  {
    ਜੇ (!ਸਰਗਰਮ++)
      recharge ();
  }

  ਖਾਲੀ leave ()
  {
    ਪ੍ਰਤਿਜ੍ਞਾ (ਸਰਗਰਮ);
    ਸਰਗਰਮ--;
  }

  ਖਾਲੀ recharge ()
  {
    budget_ਬਚਿਆ = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }
};

ਬਣਤਰ hb_cairo_context_t
{
  hb_cairo_context_t (cairo_scaled_font_t *scaled_font_,
		      cairo_t *cr_,
		      hb_map_t *color_cache_,
		      hb_cairo_budget_t *budget_) :
    scaled_font (scaled_font_), cr (cr_), color_cache (color_cache_),
    budget (budget_)
  {
    budget->enter ();
  }

  ~hb_cairo_context_t ()
  {
    budget->leave ();
  }

  ਤਾਰਕਿਕ ਸੈੱਟ_budget (int64_t value)
  {
    budget->budget = value;
    budget->recharge ();
    ਵਾਪਸ ਸੱਚ;
  }

  int64_t get_budget () ਅਚਲ { ਵਾਪਸ budget->budget; }
  int64_t *get_budget_ਬਚਿਆ () { ਵਾਪਸ &budget->budget_ਬਚਿਆ; }

  ਤਾਰਕਿਕ spend (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਲਾਗਤ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ mult = 1)
  { ਵਾਪਸ hb_budget_spend (budget->budget_ਬਚਿਆ, ਲਾਗਤ, mult); }

  cairo_scaled_font_t *scaled_font;
  cairo_t *cr;
  hb_map_t *color_cache;
  hb_cairo_budget_t *budget;
};

ਸਥਿਰ ਸਤਰਅੰਦਰ cairo_operator_t
hb_paint_ਸੰਯੁਕਤ_ਬਹੁਲਕ_ਮੰਜ਼ਿਲ_cairo (hb_paint_ਸੰਯੁਕਤ_ਬਹੁਲਕ_t ਬਹੁਲਕ)
{
  ਚੋਣ (ਬਹੁਲਕ)
    {
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_CLEAR: ਵਾਪਸ CAIRO_OPERATOR_CLEAR;
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC: ਵਾਪਸ CAIRO_OPERATOR_ਸਰੋਤ;
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DEST: ਵਾਪਸ CAIRO_OPERATOR_DEST;
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC_OVER: ਵਾਪਸ CAIRO_OPERATOR_OVER;
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DEST_OVER: ਵਾਪਸ CAIRO_OPERATOR_DEST_OVER;
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC_ਵਿੱਚ: ਵਾਪਸ CAIRO_OPERATOR_ਵਿੱਚ;
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DEST_ਵਿੱਚ: ਵਾਪਸ CAIRO_OPERATOR_DEST_ਵਿੱਚ;
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC_ਬਾਹਰਲਾ: ਵਾਪਸ CAIRO_OPERATOR_ਬਾਹਰਲਾ;
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DEST_ਬਾਹਰਲਾ: ਵਾਪਸ CAIRO_OPERATOR_DEST_ਬਾਹਰਲਾ;
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC_ATOP: ਵਾਪਸ CAIRO_OPERATOR_ATOP;
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DEST_ATOP: ਵਾਪਸ CAIRO_OPERATOR_DEST_ATOP;
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_XOR: ਵਾਪਸ CAIRO_OPERATOR_XOR;
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_PLUS: ਵਾਪਸ CAIRO_OPERATOR_ਜੋੜੋ;
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SCREEN: ਵਾਪਸ CAIRO_OPERATOR_SCREEN;
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_OVERLAY: ਵਾਪਸ CAIRO_OPERATOR_OVERLAY;
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DARKEN: ਵਾਪਸ CAIRO_OPERATOR_DARKEN;
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_LIGHTEN: ਵਾਪਸ CAIRO_OPERATOR_LIGHTEN;
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_COLOR_DODGE: ਵਾਪਸ CAIRO_OPERATOR_COLOR_DODGE;
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_COLOR_BURN: ਵਾਪਸ CAIRO_OPERATOR_COLOR_BURN;
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_HARD_LIGHT: ਵਾਪਸ CAIRO_OPERATOR_HARD_LIGHT;
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SOFT_LIGHT: ਵਾਪਸ CAIRO_OPERATOR_SOFT_LIGHT;
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DIFFERENCE: ਵਾਪਸ CAIRO_OPERATOR_DIFFERENCE;
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_EXCLUSION: ਵਾਪਸ CAIRO_OPERATOR_EXCLUSION;
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_MULTIPLY: ਵਾਪਸ CAIRO_OPERATOR_MULTIPLY;
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_HSL_HUE: ਵਾਪਸ CAIRO_OPERATOR_HSL_HUE;
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_HSL_SATURATION: ਵਾਪਸ CAIRO_OPERATOR_HSL_SATURATION;
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_HSL_COLOR: ਵਾਪਸ CAIRO_OPERATOR_HSL_COLOR;
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_HSL_LUMINOSITY: ਵਾਪਸ CAIRO_OPERATOR_HSL_LUMINOSITY;
    ਮੂਲ: ਵਾਪਸ CAIRO_OPERATOR_CLEAR;
    }
}

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ
hb_cairo_ਸੈੱਟ_ਸਰੋਤ_color (hb_cairo_context_t *c,
			    hb_bool_t use_foreground,
			    hb_color_t color);

HB_ਅੰਦਰੂਨੀ hb_bool_t
_hb_cairo_paint_glyph_image (hb_cairo_context_t *c,
			     hb_blob_t *blob,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਚੌੜਾਈ,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਉਚਾਈ,
			     hb_ਟੈਗ_t format,
			     ਦਸ਼ਮਲਵ slant,
			     hb_glyph_extents_t *extents);

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ
hb_cairo_paint_ਰੇਖਿਕ_gradient (hb_cairo_context_t *c,
				 hb_color_ਸਤਰ_t *color_ਸਤਰ,
				 ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0,
				 ਦਸ਼ਮਲਵ x1, ਦਸ਼ਮਲਵ y1,
				 ਦਸ਼ਮਲਵ x2, ਦਸ਼ਮਲਵ y2);

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ
_hb_cairo_paint_radial_gradient (hb_cairo_context_t *c,
				 hb_color_ਸਤਰ_t *color_ਸਤਰ,
				 ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0, ਦਸ਼ਮਲਵ r0,
				 ਦਸ਼ਮਲਵ x1, ਦਸ਼ਮਲਵ y1, ਦਸ਼ਮਲਵ r1);

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ
_hb_cairo_paint_sweep_gradient (hb_cairo_context_t *c,
				hb_color_ਸਤਰ_t *color_ਸਤਰ,
				ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0,
				ਦਸ਼ਮਲਵ ਸ਼ੁਰੂ_angle, ਦਸ਼ਮਲਵ ਸਿਰਾ_angle);


#ਜੇ_ਅੰਤ /* HB_CAIRO_UTILS_H */

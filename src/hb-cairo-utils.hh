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

#ifndef HB_CAIRO_UTILS_H
#define HB_CAIRO_UTILS_H

#ضمن "hb.hh"
#ضمن "hb-cairo.h"


بنية hb_cairo_budget_t
{
  /* Cairo serializes glyph-cache population for a scaled font.  Keeping the
   * owner there lets recursive color-glyph callbacks share one session. */
  int64_t budget = HB_BUDGET_DEFAULT;
  int64_t budget_remaining = HB_BUDGET_GLYPH;
  unsigned صحيح active = 0;

  فراغ enter ()
  {
    إذا (!active++)
      recharge ();
  }

  فراغ leave ()
  {
    أكد (active);
    active--;
  }

  فراغ recharge ()
  {
    budget_remaining = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }
};

بنية hb_cairo_context_t
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

  منطقي set_budget (int64_t value)
  {
    budget->budget = value;
    budget->recharge ();
    أرجع true;
  }

  int64_t get_budget () ثابت { أرجع budget->budget; }
  int64_t *get_budget_remaining () { أرجع &budget->budget_remaining; }

  منطقي spend (unsigned صحيح cost, unsigned صحيح mult = 1)
  { أرجع hb_budget_spend (budget->budget_remaining, cost, mult); }

  cairo_scaled_font_t *scaled_font;
  cairo_t *cr;
  hb_map_t *color_cache;
  hb_cairo_budget_t *budget;
};

ساكن مضمن cairo_operator_t
_hb_paint_composite_mode_to_cairo (hb_paint_composite_mode_t mode)
{
  بدل (mode)
    {
    حالة HB_PAINT_COMPOSITE_MODE_CLEAR: أرجع CAIRO_OPERATOR_CLEAR;
    حالة HB_PAINT_COMPOSITE_MODE_SRC: أرجع CAIRO_OPERATOR_SOURCE;
    حالة HB_PAINT_COMPOSITE_MODE_DEST: أرجع CAIRO_OPERATOR_DEST;
    حالة HB_PAINT_COMPOSITE_MODE_SRC_OVER: أرجع CAIRO_OPERATOR_OVER;
    حالة HB_PAINT_COMPOSITE_MODE_DEST_OVER: أرجع CAIRO_OPERATOR_DEST_OVER;
    حالة HB_PAINT_COMPOSITE_MODE_SRC_IN: أرجع CAIRO_OPERATOR_IN;
    حالة HB_PAINT_COMPOSITE_MODE_DEST_IN: أرجع CAIRO_OPERATOR_DEST_IN;
    حالة HB_PAINT_COMPOSITE_MODE_SRC_OUT: أرجع CAIRO_OPERATOR_OUT;
    حالة HB_PAINT_COMPOSITE_MODE_DEST_OUT: أرجع CAIRO_OPERATOR_DEST_OUT;
    حالة HB_PAINT_COMPOSITE_MODE_SRC_ATOP: أرجع CAIRO_OPERATOR_ATOP;
    حالة HB_PAINT_COMPOSITE_MODE_DEST_ATOP: أرجع CAIRO_OPERATOR_DEST_ATOP;
    حالة HB_PAINT_COMPOSITE_MODE_XOR: أرجع CAIRO_OPERATOR_XOR;
    حالة HB_PAINT_COMPOSITE_MODE_PLUS: أرجع CAIRO_OPERATOR_ADD;
    حالة HB_PAINT_COMPOSITE_MODE_SCREEN: أرجع CAIRO_OPERATOR_SCREEN;
    حالة HB_PAINT_COMPOSITE_MODE_OVERLAY: أرجع CAIRO_OPERATOR_OVERLAY;
    حالة HB_PAINT_COMPOSITE_MODE_DARKEN: أرجع CAIRO_OPERATOR_DARKEN;
    حالة HB_PAINT_COMPOSITE_MODE_LIGHTEN: أرجع CAIRO_OPERATOR_LIGHTEN;
    حالة HB_PAINT_COMPOSITE_MODE_COLOR_DODGE: أرجع CAIRO_OPERATOR_COLOR_DODGE;
    حالة HB_PAINT_COMPOSITE_MODE_COLOR_BURN: أرجع CAIRO_OPERATOR_COLOR_BURN;
    حالة HB_PAINT_COMPOSITE_MODE_HARD_LIGHT: أرجع CAIRO_OPERATOR_HARD_LIGHT;
    حالة HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT: أرجع CAIRO_OPERATOR_SOFT_LIGHT;
    حالة HB_PAINT_COMPOSITE_MODE_DIFFERENCE: أرجع CAIRO_OPERATOR_DIFFERENCE;
    حالة HB_PAINT_COMPOSITE_MODE_EXCLUSION: أرجع CAIRO_OPERATOR_EXCLUSION;
    حالة HB_PAINT_COMPOSITE_MODE_MULTIPLY: أرجع CAIRO_OPERATOR_MULTIPLY;
    حالة HB_PAINT_COMPOSITE_MODE_HSL_HUE: أرجع CAIRO_OPERATOR_HSL_HUE;
    حالة HB_PAINT_COMPOSITE_MODE_HSL_SATURATION: أرجع CAIRO_OPERATOR_HSL_SATURATION;
    حالة HB_PAINT_COMPOSITE_MODE_HSL_COLOR: أرجع CAIRO_OPERATOR_HSL_COLOR;
    حالة HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY: أرجع CAIRO_OPERATOR_HSL_LUMINOSITY;
    افتراضي: أرجع CAIRO_OPERATOR_CLEAR;
    }
}

HB_INTERNAL فراغ
_hb_cairo_set_source_color (hb_cairo_context_t *c,
			    hb_bool_t use_foreground,
			    hb_color_t color);

HB_INTERNAL hb_bool_t
_hb_cairo_paint_glyph_image (hb_cairo_context_t *c,
			     hb_blob_t *blob,
			     unsigned width,
			     unsigned height,
			     hb_tag_t format,
			     عائم slant,
			     hb_glyph_extents_t *extents);

HB_INTERNAL فراغ
_hb_cairo_paint_linear_gradient (hb_cairo_context_t *c,
				 hb_color_هێڵ_t *color_هێڵ,
				 عائم x0, عائم y0,
				 عائم x1, عائم y1,
				 عائم x2, عائم y2);

HB_INTERNAL فراغ
_hb_cairo_paint_radial_gradient (hb_cairo_context_t *c,
				 hb_color_هێڵ_t *color_هێڵ,
				 عائم x0, عائم y0, عائم r0,
				 عائم x1, عائم y1, عائم r1);

HB_INTERNAL فراغ
_hb_cairo_paint_sweep_gradient (hb_cairo_context_t *c,
				hb_color_هێڵ_t *color_هێڵ,
				عائم x0, عائم y0,
				عائم start_angle, عائم end_angle);


#endif /* HB_CAIRO_UTILS_H */

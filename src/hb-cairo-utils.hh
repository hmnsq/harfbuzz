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

#شامل "hb.hh"
#شامل "hb-cairo.h"


ساختار hb_cairo_budget_t
{
  /* Cairo serializes glyph-cache population for a scaled font.  Keeping the
   * owner there lets recursive color-glyph callbacks share one session. */
  int64_t budget = HB_BUDGET_DEFAULT;
  int64_t budget_remaining = HB_BUDGET_GLYPH;
  unsigned صحیح active = 0;

  تهی enter ()
  {
    اگر (!active++)
      recharge ();
  }

  تهی leave ()
  {
    أكد (active);
    active--;
  }

  تهی recharge ()
  {
    budget_remaining = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }
};

ساختار hb_cairo_context_t
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

  منطقی set_budget (int64_t value)
  {
    budget->budget = value;
    budget->recharge ();
    بازگشت درست;
  }

  int64_t get_budget () ثابت { بازگشت budget->budget; }
  int64_t *get_budget_remaining () { بازگشت &budget->budget_remaining; }

  منطقی spend (unsigned صحیح cost, unsigned صحیح mult = 1)
  { بازگشت hb_budget_spend (budget->budget_remaining, cost, mult); }

  cairo_scaled_font_t *scaled_font;
  cairo_t *cr;
  hb_map_t *color_cache;
  hb_cairo_budget_t *budget;
};

ایستا درونخطی cairo_operator_t
_hb_paint_composite_mode_to_cairo (hb_paint_composite_mode_t mode)
{
  انتخاب (mode)
    {
    حالت HB_PAINT_COMPOSITE_MODE_CLEAR: بازگشت CAIRO_OPERATOR_CLEAR;
    حالت HB_PAINT_COMPOSITE_MODE_SRC: بازگشت CAIRO_OPERATOR_SOURCE;
    حالت HB_PAINT_COMPOSITE_MODE_DEST: بازگشت CAIRO_OPERATOR_DEST;
    حالت HB_PAINT_COMPOSITE_MODE_SRC_OVER: بازگشت CAIRO_OPERATOR_OVER;
    حالت HB_PAINT_COMPOSITE_MODE_DEST_OVER: بازگشت CAIRO_OPERATOR_DEST_OVER;
    حالت HB_PAINT_COMPOSITE_MODE_SRC_IN: بازگشت CAIRO_OPERATOR_IN;
    حالت HB_PAINT_COMPOSITE_MODE_DEST_IN: بازگشت CAIRO_OPERATOR_DEST_IN;
    حالت HB_PAINT_COMPOSITE_MODE_SRC_OUT: بازگشت CAIRO_OPERATOR_OUT;
    حالت HB_PAINT_COMPOSITE_MODE_DEST_OUT: بازگشت CAIRO_OPERATOR_DEST_OUT;
    حالت HB_PAINT_COMPOSITE_MODE_SRC_ATOP: بازگشت CAIRO_OPERATOR_ATOP;
    حالت HB_PAINT_COMPOSITE_MODE_DEST_ATOP: بازگشت CAIRO_OPERATOR_DEST_ATOP;
    حالت HB_PAINT_COMPOSITE_MODE_XOR: بازگشت CAIRO_OPERATOR_XOR;
    حالت HB_PAINT_COMPOSITE_MODE_PLUS: بازگشت CAIRO_OPERATOR_ADD;
    حالت HB_PAINT_COMPOSITE_MODE_SCREEN: بازگشت CAIRO_OPERATOR_SCREEN;
    حالت HB_PAINT_COMPOSITE_MODE_OVERLAY: بازگشت CAIRO_OPERATOR_OVERLAY;
    حالت HB_PAINT_COMPOSITE_MODE_DARKEN: بازگشت CAIRO_OPERATOR_DARKEN;
    حالت HB_PAINT_COMPOSITE_MODE_LIGHTEN: بازگشت CAIRO_OPERATOR_LIGHTEN;
    حالت HB_PAINT_COMPOSITE_MODE_COLOR_DODGE: بازگشت CAIRO_OPERATOR_COLOR_DODGE;
    حالت HB_PAINT_COMPOSITE_MODE_COLOR_BURN: بازگشت CAIRO_OPERATOR_COLOR_BURN;
    حالت HB_PAINT_COMPOSITE_MODE_HARD_LIGHT: بازگشت CAIRO_OPERATOR_HARD_LIGHT;
    حالت HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT: بازگشت CAIRO_OPERATOR_SOFT_LIGHT;
    حالت HB_PAINT_COMPOSITE_MODE_DIFFERENCE: بازگشت CAIRO_OPERATOR_DIFFERENCE;
    حالت HB_PAINT_COMPOSITE_MODE_EXCLUSION: بازگشت CAIRO_OPERATOR_EXCLUSION;
    حالت HB_PAINT_COMPOSITE_MODE_MULTIPLY: بازگشت CAIRO_OPERATOR_MULTIPLY;
    حالت HB_PAINT_COMPOSITE_MODE_HSL_HUE: بازگشت CAIRO_OPERATOR_HSL_HUE;
    حالت HB_PAINT_COMPOSITE_MODE_HSL_SATURATION: بازگشت CAIRO_OPERATOR_HSL_SATURATION;
    حالت HB_PAINT_COMPOSITE_MODE_HSL_COLOR: بازگشت CAIRO_OPERATOR_HSL_COLOR;
    حالت HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY: بازگشت CAIRO_OPERATOR_HSL_LUMINOSITY;
    پیشفرض: بازگشت CAIRO_OPERATOR_CLEAR;
    }
}

HB_INTERNAL تهی
_hb_cairo_set_source_color (hb_cairo_context_t *c,
			    hb_bool_t use_foreground,
			    hb_color_t color);

HB_INTERNAL hb_bool_t
_hb_cairo_paint_glyph_image (hb_cairo_context_t *c,
			     hb_blob_t *blob,
			     unsigned width,
			     unsigned height,
			     hb_tag_t format,
			     اعشاری slant,
			     hb_glyph_extents_t *extents);

HB_INTERNAL تهی
_hb_cairo_paint_linear_gradient (hb_cairo_context_t *c,
				 hb_color_سطر_t *color_سطر,
				 اعشاری x0, اعشاری y0,
				 اعشاری x1, اعشاری y1,
				 اعشاری x2, اعشاری y2);

HB_INTERNAL تهی
_hb_cairo_paint_radial_gradient (hb_cairo_context_t *c,
				 hb_color_سطر_t *color_سطر,
				 اعشاری x0, اعشاری y0, اعشاری r0,
				 اعشاری x1, اعشاری y1, اعشاری r1);

HB_INTERNAL تهی
_hb_cairo_paint_sweep_gradient (hb_cairo_context_t *c,
				hb_color_سطر_t *color_سطر,
				اعشاری x0, اعشاری y0,
				اعشاری start_angle, اعشاری end_angle);


#endif /* HB_CAIRO_UTILS_H */

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

#অন্তর্ভাৱ "hb.hh"
#অন্তর্ভাৱ "hb-cairo.h"


সংরচনা hb_cairo_budget_t
{
  /* Cairo serializes glyph-cache population for a scaled font.  Keeping the
   * owner there lets recursive color-glyph callbacks share one session. */
  int64_t budget = HB_BUDGET_DEFAULT;
  int64_t budget_remaining = HB_BUDGET_GLYPH;
  unsigned পূর্ণাঙ্ক active = 0;

  শূন্য enter ()
  {
    যদি (!active++)
      recharge ();
  }

  শূন্য leave ()
  {
    প্রতিজ্ঞা (active);
    active--;
  }

  শূন্য recharge ()
  {
    budget_remaining = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }
};

সংরচনা hb_cairo_context_t
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

  তার্কিক set_budget (int64_t value)
  {
    budget->budget = value;
    budget->recharge ();
    ঘূৰাই সঁচা;
  }

  int64_t get_budget () ধ্রুৱ { ঘূৰাই budget->budget; }
  int64_t *get_budget_remaining () { ঘূৰাই &budget->budget_remaining; }

  তার্কিক spend (unsigned পূর্ণাঙ্ক cost, unsigned পূর্ণাঙ্ক mult = 1)
  { ঘূৰাই hb_budget_spend (budget->budget_remaining, cost, mult); }

  cairo_scaled_font_t *scaled_font;
  cairo_t *cr;
  hb_map_t *color_cache;
  hb_cairo_budget_t *budget;
};

স্থির অন্তঃস্থ cairo_operator_t
_hb_paint_composite_mode_to_cairo (hb_paint_composite_mode_t mode)
{
  ৱিকল্প (mode)
    {
    পক্ষ HB_PAINT_COMPOSITE_MODE_CLEAR: ঘূৰাই CAIRO_OPERATOR_CLEAR;
    পক্ষ HB_PAINT_COMPOSITE_MODE_SRC: ঘূৰাই CAIRO_OPERATOR_SOURCE;
    পক্ষ HB_PAINT_COMPOSITE_MODE_DEST: ঘূৰাই CAIRO_OPERATOR_DEST;
    পক্ষ HB_PAINT_COMPOSITE_MODE_SRC_OVER: ঘূৰাই CAIRO_OPERATOR_OVER;
    পক্ষ HB_PAINT_COMPOSITE_MODE_DEST_OVER: ঘূৰাই CAIRO_OPERATOR_DEST_OVER;
    পক্ষ HB_PAINT_COMPOSITE_MODE_SRC_IN: ঘূৰাই CAIRO_OPERATOR_IN;
    পক্ষ HB_PAINT_COMPOSITE_MODE_DEST_IN: ঘূৰাই CAIRO_OPERATOR_DEST_IN;
    পক্ষ HB_PAINT_COMPOSITE_MODE_SRC_OUT: ঘূৰাই CAIRO_OPERATOR_OUT;
    পক্ষ HB_PAINT_COMPOSITE_MODE_DEST_OUT: ঘূৰাই CAIRO_OPERATOR_DEST_OUT;
    পক্ষ HB_PAINT_COMPOSITE_MODE_SRC_ATOP: ঘূৰাই CAIRO_OPERATOR_ATOP;
    পক্ষ HB_PAINT_COMPOSITE_MODE_DEST_ATOP: ঘূৰাই CAIRO_OPERATOR_DEST_ATOP;
    পক্ষ HB_PAINT_COMPOSITE_MODE_XOR: ঘূৰাই CAIRO_OPERATOR_XOR;
    পক্ষ HB_PAINT_COMPOSITE_MODE_PLUS: ঘূৰাই CAIRO_OPERATOR_ADD;
    পক্ষ HB_PAINT_COMPOSITE_MODE_SCREEN: ঘূৰাই CAIRO_OPERATOR_SCREEN;
    পক্ষ HB_PAINT_COMPOSITE_MODE_OVERLAY: ঘূৰাই CAIRO_OPERATOR_OVERLAY;
    পক্ষ HB_PAINT_COMPOSITE_MODE_DARKEN: ঘূৰাই CAIRO_OPERATOR_DARKEN;
    পক্ষ HB_PAINT_COMPOSITE_MODE_LIGHTEN: ঘূৰাই CAIRO_OPERATOR_LIGHTEN;
    পক্ষ HB_PAINT_COMPOSITE_MODE_COLOR_DODGE: ঘূৰাই CAIRO_OPERATOR_COLOR_DODGE;
    পক্ষ HB_PAINT_COMPOSITE_MODE_COLOR_BURN: ঘূৰাই CAIRO_OPERATOR_COLOR_BURN;
    পক্ষ HB_PAINT_COMPOSITE_MODE_HARD_LIGHT: ঘূৰাই CAIRO_OPERATOR_HARD_LIGHT;
    পক্ষ HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT: ঘূৰাই CAIRO_OPERATOR_SOFT_LIGHT;
    পক্ষ HB_PAINT_COMPOSITE_MODE_DIFFERENCE: ঘূৰাই CAIRO_OPERATOR_DIFFERENCE;
    পক্ষ HB_PAINT_COMPOSITE_MODE_EXCLUSION: ঘূৰাই CAIRO_OPERATOR_EXCLUSION;
    পক্ষ HB_PAINT_COMPOSITE_MODE_MULTIPLY: ঘূৰাই CAIRO_OPERATOR_MULTIPLY;
    পক্ষ HB_PAINT_COMPOSITE_MODE_HSL_HUE: ঘূৰাই CAIRO_OPERATOR_HSL_HUE;
    পক্ষ HB_PAINT_COMPOSITE_MODE_HSL_SATURATION: ঘূৰাই CAIRO_OPERATOR_HSL_SATURATION;
    পক্ষ HB_PAINT_COMPOSITE_MODE_HSL_COLOR: ঘূৰাই CAIRO_OPERATOR_HSL_COLOR;
    পক্ষ HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY: ঘূৰাই CAIRO_OPERATOR_HSL_LUMINOSITY;
    স্ৱভাৱ: ঘূৰাই CAIRO_OPERATOR_CLEAR;
    }
}

HB_INTERNAL শূন্য
_hb_cairo_set_source_color (hb_cairo_context_t *c,
			    hb_bool_t use_foreground,
			    hb_color_t color);

HB_INTERNAL hb_bool_t
_hb_cairo_paint_glyph_image (hb_cairo_context_t *c,
			     hb_blob_t *blob,
			     unsigned width,
			     unsigned height,
			     hb_tag_t format,
			     ভিন্ন slant,
			     hb_glyph_extents_t *extents);

HB_INTERNAL শূন্য
_hb_cairo_paint_linear_gradient (hb_cairo_context_t *c,
				 hb_color_সাৰি_t *color_সাৰি,
				 ভিন্ন x0, ভিন্ন y0,
				 ভিন্ন x1, ভিন্ন y1,
				 ভিন্ন x2, ভিন্ন y2);

HB_INTERNAL শূন্য
_hb_cairo_paint_radial_gradient (hb_cairo_context_t *c,
				 hb_color_সাৰি_t *color_সাৰি,
				 ভিন্ন x0, ভিন্ন y0, ভিন্ন r0,
				 ভিন্ন x1, ভিন্ন y1, ভিন্ন r1);

HB_INTERNAL শূন্য
_hb_cairo_paint_sweep_gradient (hb_cairo_context_t *c,
				hb_color_সাৰি_t *color_সাৰি,
				ভিন্ন x0, ভিন্ন y0,
				ভিন্ন start_angle, ভিন্ন end_angle);


#endif /* HB_CAIRO_UTILS_H */

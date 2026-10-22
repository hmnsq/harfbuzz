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

#include "hb.hh"
#include "hb-cairo.h"


struct hb_cairo_budget_t
{
  /* Cairo serializes glyph-cache population for a scaled font.  Keeping the
   * owner there lets recursive color-glyph callbacks share one session. */
  int64_t budget = HB_BUDGET_DEFAULT;
  int64_t budget_remaining = HB_BUDGET_GLYPH;
  unsigned ціле active = 0;

  void enter ()
  {
    якщо (!active++)
      recharge ();
  }

  void leave ()
  {
    assert (active);
    active--;
  }

  void recharge ()
  {
    budget_remaining = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }
};

struct hb_cairo_context_t
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

  bool set_budget (int64_t value)
  {
    budget->budget = value;
    budget->recharge ();
    повернути true;
  }

  int64_t get_budget () const { повернути budget->budget; }
  int64_t *get_budget_remaining () { повернути &budget->budget_remaining; }

  bool spend (unsigned ціле cost, unsigned ціле mult = 1)
  { повернути hb_budget_spend (budget->budget_remaining, cost, mult); }

  cairo_scaled_font_t *scaled_font;
  cairo_t *cr;
  hb_map_t *color_cache;
  hb_cairo_budget_t *budget;
};

static inline cairo_operator_t
_hb_paint_composite_mode_to_cairo (hb_paint_composite_mode_t mode)
{
  switch (mode)
    {
    case HB_PAINT_COMPOSITE_MODE_CLEAR: повернути CAIRO_OPERATOR_CLEAR;
    case HB_PAINT_COMPOSITE_MODE_SRC: повернути CAIRO_OPERATOR_SOURCE;
    case HB_PAINT_COMPOSITE_MODE_DEST: повернути CAIRO_OPERATOR_DEST;
    case HB_PAINT_COMPOSITE_MODE_SRC_OVER: повернути CAIRO_OPERATOR_OVER;
    case HB_PAINT_COMPOSITE_MODE_DEST_OVER: повернути CAIRO_OPERATOR_DEST_OVER;
    case HB_PAINT_COMPOSITE_MODE_SRC_IN: повернути CAIRO_OPERATOR_IN;
    case HB_PAINT_COMPOSITE_MODE_DEST_IN: повернути CAIRO_OPERATOR_DEST_IN;
    case HB_PAINT_COMPOSITE_MODE_SRC_OUT: повернути CAIRO_OPERATOR_OUT;
    case HB_PAINT_COMPOSITE_MODE_DEST_OUT: повернути CAIRO_OPERATOR_DEST_OUT;
    case HB_PAINT_COMPOSITE_MODE_SRC_ATOP: повернути CAIRO_OPERATOR_ATOP;
    case HB_PAINT_COMPOSITE_MODE_DEST_ATOP: повернути CAIRO_OPERATOR_DEST_ATOP;
    case HB_PAINT_COMPOSITE_MODE_XOR: повернути CAIRO_OPERATOR_XOR;
    case HB_PAINT_COMPOSITE_MODE_PLUS: повернути CAIRO_OPERATOR_ADD;
    case HB_PAINT_COMPOSITE_MODE_SCREEN: повернути CAIRO_OPERATOR_SCREEN;
    case HB_PAINT_COMPOSITE_MODE_OVERLAY: повернути CAIRO_OPERATOR_OVERLAY;
    case HB_PAINT_COMPOSITE_MODE_DARKEN: повернути CAIRO_OPERATOR_DARKEN;
    case HB_PAINT_COMPOSITE_MODE_LIGHTEN: повернути CAIRO_OPERATOR_LIGHTEN;
    case HB_PAINT_COMPOSITE_MODE_COLOR_DODGE: повернути CAIRO_OPERATOR_COLOR_DODGE;
    case HB_PAINT_COMPOSITE_MODE_COLOR_BURN: повернути CAIRO_OPERATOR_COLOR_BURN;
    case HB_PAINT_COMPOSITE_MODE_HARD_LIGHT: повернути CAIRO_OPERATOR_HARD_LIGHT;
    case HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT: повернути CAIRO_OPERATOR_SOFT_LIGHT;
    case HB_PAINT_COMPOSITE_MODE_DIFFERENCE: повернути CAIRO_OPERATOR_DIFFERENCE;
    case HB_PAINT_COMPOSITE_MODE_EXCLUSION: повернути CAIRO_OPERATOR_EXCLUSION;
    case HB_PAINT_COMPOSITE_MODE_MULTIPLY: повернути CAIRO_OPERATOR_MULTIPLY;
    case HB_PAINT_COMPOSITE_MODE_HSL_HUE: повернути CAIRO_OPERATOR_HSL_HUE;
    case HB_PAINT_COMPOSITE_MODE_HSL_SATURATION: повернути CAIRO_OPERATOR_HSL_SATURATION;
    case HB_PAINT_COMPOSITE_MODE_HSL_COLOR: повернути CAIRO_OPERATOR_HSL_COLOR;
    case HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY: повернути CAIRO_OPERATOR_HSL_LUMINOSITY;
    default: повернути CAIRO_OPERATOR_CLEAR;
    }
}

HB_INTERNAL void
_hb_cairo_set_source_color (hb_cairo_context_t *c,
			    hb_bool_t use_foreground,
			    hb_color_t color);

HB_INTERNAL hb_bool_t
_hb_cairo_paint_glyph_image (hb_cairo_context_t *c,
			     hb_blob_t *blob,
			     unsigned width,
			     unsigned height,
			     hb_tag_t format,
			     float slant,
			     hb_glyph_extents_t *extents);

HB_INTERNAL void
_hb_cairo_paint_linear_gradient (hb_cairo_context_t *c,
				 hb_color_line_t *color_line,
				 float x0, float y0,
				 float x1, float y1,
				 float x2, float y2);

HB_INTERNAL void
_hb_cairo_paint_radial_gradient (hb_cairo_context_t *c,
				 hb_color_line_t *color_line,
				 float x0, float y0, float r0,
				 float x1, float y1, float r1);

HB_INTERNAL void
_hb_cairo_paint_sweep_gradient (hb_cairo_context_t *c,
				hb_color_line_t *color_line,
				float x0, float y0,
				float start_angle, float end_angle);


#endif /* HB_CAIRO_UTILS_H */

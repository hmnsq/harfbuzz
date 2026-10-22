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

#அந்தர்பாவ "hb.hh"
#அந்தர்பாவ "hb-cairo.h"


கட்டமைப்பு hb_cairo_budget_t
{
  /* Cairo serializes glyph-cache population for a scaled font.  Keeping the
   * owner there lets recursive color-glyph callbacks share one session. */
  int64_t budget = HB_BUDGET_DEFAULT;
  int64_t budget_remaining = HB_BUDGET_GLYPH;
  unsigned முழு active = 0;

  வெறுமை enter ()
  {
    எனில் (!active++)
      recharge ();
  }

  வெறுமை leave ()
  {
    ப்ரதிஜ்ஞா (active);
    active--;
  }

  வெறுமை recharge ()
  {
    budget_remaining = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }
};

கட்டமைப்பு hb_cairo_context_t
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

  தருக்க set_budget (int64_t value)
  {
    budget->budget = value;
    budget->recharge ();
    திரும்பு உண்மை;
  }

  int64_t get_budget () மாறிலி { திரும்பு budget->budget; }
  int64_t *get_budget_remaining () { திரும்பு &budget->budget_remaining; }

  தருக்க spend (unsigned முழு cost, unsigned முழு mult = 1)
  { திரும்பு hb_budget_spend (budget->budget_remaining, cost, mult); }

  cairo_scaled_font_t *scaled_font;
  cairo_t *cr;
  hb_map_t *color_cache;
  hb_cairo_budget_t *budget;
};

நிலையான உள்வரி cairo_operator_t
_hb_paint_composite_mode_to_cairo (hb_paint_composite_mode_t mode)
{
  தேர்வு (mode)
    {
    நிலை HB_PAINT_COMPOSITE_MODE_CLEAR: திரும்பு CAIRO_OPERATOR_CLEAR;
    நிலை HB_PAINT_COMPOSITE_MODE_SRC: திரும்பு CAIRO_OPERATOR_SOURCE;
    நிலை HB_PAINT_COMPOSITE_MODE_DEST: திரும்பு CAIRO_OPERATOR_DEST;
    நிலை HB_PAINT_COMPOSITE_MODE_SRC_OVER: திரும்பு CAIRO_OPERATOR_OVER;
    நிலை HB_PAINT_COMPOSITE_MODE_DEST_OVER: திரும்பு CAIRO_OPERATOR_DEST_OVER;
    நிலை HB_PAINT_COMPOSITE_MODE_SRC_IN: திரும்பு CAIRO_OPERATOR_IN;
    நிலை HB_PAINT_COMPOSITE_MODE_DEST_IN: திரும்பு CAIRO_OPERATOR_DEST_IN;
    நிலை HB_PAINT_COMPOSITE_MODE_SRC_OUT: திரும்பு CAIRO_OPERATOR_OUT;
    நிலை HB_PAINT_COMPOSITE_MODE_DEST_OUT: திரும்பு CAIRO_OPERATOR_DEST_OUT;
    நிலை HB_PAINT_COMPOSITE_MODE_SRC_ATOP: திரும்பு CAIRO_OPERATOR_ATOP;
    நிலை HB_PAINT_COMPOSITE_MODE_DEST_ATOP: திரும்பு CAIRO_OPERATOR_DEST_ATOP;
    நிலை HB_PAINT_COMPOSITE_MODE_XOR: திரும்பு CAIRO_OPERATOR_XOR;
    நிலை HB_PAINT_COMPOSITE_MODE_PLUS: திரும்பு CAIRO_OPERATOR_ADD;
    நிலை HB_PAINT_COMPOSITE_MODE_SCREEN: திரும்பு CAIRO_OPERATOR_SCREEN;
    நிலை HB_PAINT_COMPOSITE_MODE_OVERLAY: திரும்பு CAIRO_OPERATOR_OVERLAY;
    நிலை HB_PAINT_COMPOSITE_MODE_DARKEN: திரும்பு CAIRO_OPERATOR_DARKEN;
    நிலை HB_PAINT_COMPOSITE_MODE_LIGHTEN: திரும்பு CAIRO_OPERATOR_LIGHTEN;
    நிலை HB_PAINT_COMPOSITE_MODE_COLOR_DODGE: திரும்பு CAIRO_OPERATOR_COLOR_DODGE;
    நிலை HB_PAINT_COMPOSITE_MODE_COLOR_BURN: திரும்பு CAIRO_OPERATOR_COLOR_BURN;
    நிலை HB_PAINT_COMPOSITE_MODE_HARD_LIGHT: திரும்பு CAIRO_OPERATOR_HARD_LIGHT;
    நிலை HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT: திரும்பு CAIRO_OPERATOR_SOFT_LIGHT;
    நிலை HB_PAINT_COMPOSITE_MODE_DIFFERENCE: திரும்பு CAIRO_OPERATOR_DIFFERENCE;
    நிலை HB_PAINT_COMPOSITE_MODE_EXCLUSION: திரும்பு CAIRO_OPERATOR_EXCLUSION;
    நிலை HB_PAINT_COMPOSITE_MODE_MULTIPLY: திரும்பு CAIRO_OPERATOR_MULTIPLY;
    நிலை HB_PAINT_COMPOSITE_MODE_HSL_HUE: திரும்பு CAIRO_OPERATOR_HSL_HUE;
    நிலை HB_PAINT_COMPOSITE_MODE_HSL_SATURATION: திரும்பு CAIRO_OPERATOR_HSL_SATURATION;
    நிலை HB_PAINT_COMPOSITE_MODE_HSL_COLOR: திரும்பு CAIRO_OPERATOR_HSL_COLOR;
    நிலை HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY: திரும்பு CAIRO_OPERATOR_HSL_LUMINOSITY;
    இயல்பு: திரும்பு CAIRO_OPERATOR_CLEAR;
    }
}

HB_INTERNAL வெறுமை
_hb_cairo_set_source_color (hb_cairo_context_t *c,
			    hb_bool_t use_foreground,
			    hb_color_t color);

HB_INTERNAL hb_bool_t
_hb_cairo_paint_glyph_image (hb_cairo_context_t *c,
			     hb_blob_t *blob,
			     unsigned width,
			     unsigned height,
			     hb_tag_t format,
			     தசம slant,
			     hb_glyph_extents_t *extents);

HB_INTERNAL வெறுமை
_hb_cairo_paint_linear_gradient (hb_cairo_context_t *c,
				 hb_color_வரி_t *color_வரி,
				 தசம x0, தசம y0,
				 தசம x1, தசம y1,
				 தசம x2, தசம y2);

HB_INTERNAL வெறுமை
_hb_cairo_paint_radial_gradient (hb_cairo_context_t *c,
				 hb_color_வரி_t *color_வரி,
				 தசம x0, தசம y0, தசம r0,
				 தசம x1, தசம y1, தசம r1);

HB_INTERNAL வெறுமை
_hb_cairo_paint_sweep_gradient (hb_cairo_context_t *c,
				hb_color_வரி_t *color_வரி,
				தசம x0, தசம y0,
				தசம start_angle, தசம end_angle);


#endif /* HB_CAIRO_UTILS_H */

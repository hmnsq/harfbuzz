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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-cairo.h"


संरचना hb_cairo_budget_t
{
  /* Cairo serializes glyph-cache population for a scaled font.  Keeping the
   * owner there lets recursive color-glyph callbacks share one session. */
  int64_t budget = HB_BUDGET_DEFAULT;
  int64_t budget_remaining = HB_BUDGET_GLYPH;
  अचिन्हित पूर्णांक active = 0;

  शून्य enter ()
  {
    जर (!active++)
      recharge ();
  }

  शून्य leave ()
  {
    प्रतिज्ञा (active);
    active--;
  }

  शून्य recharge ()
  {
    budget_remaining = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }
};

संरचना hb_cairo_context_t
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

  तार्किक set_budget (int64_t value)
  {
    budget->budget = value;
    budget->recharge ();
    परत सत्य;
  }

  int64_t get_budget () स्थिरांक { परत budget->budget; }
  int64_t *get_budget_remaining () { परत &budget->budget_remaining; }

  तार्किक spend (अचिन्हित पूर्णांक cost, अचिन्हित पूर्णांक mult = 1)
  { परत hb_budget_spend (budget->budget_remaining, cost, mult); }

  cairo_scaled_font_t *scaled_font;
  cairo_t *cr;
  hb_map_t *color_cache;
  hb_cairo_budget_t *budget;
};

स्थिर ओळीत cairo_operator_t
_hb_paint_composite_mode_to_cairo (hb_paint_composite_mode_t mode)
{
  निवड (mode)
    {
    स्थिती HB_PAINT_COMPOSITE_MODE_CLEAR: परत CAIRO_OPERATOR_CLEAR;
    स्थिती HB_PAINT_COMPOSITE_MODE_SRC: परत CAIRO_OPERATOR_SOURCE;
    स्थिती HB_PAINT_COMPOSITE_MODE_DEST: परत CAIRO_OPERATOR_DEST;
    स्थिती HB_PAINT_COMPOSITE_MODE_SRC_OVER: परत CAIRO_OPERATOR_OVER;
    स्थिती HB_PAINT_COMPOSITE_MODE_DEST_OVER: परत CAIRO_OPERATOR_DEST_OVER;
    स्थिती HB_PAINT_COMPOSITE_MODE_SRC_IN: परत CAIRO_OPERATOR_IN;
    स्थिती HB_PAINT_COMPOSITE_MODE_DEST_IN: परत CAIRO_OPERATOR_DEST_IN;
    स्थिती HB_PAINT_COMPOSITE_MODE_SRC_OUT: परत CAIRO_OPERATOR_OUT;
    स्थिती HB_PAINT_COMPOSITE_MODE_DEST_OUT: परत CAIRO_OPERATOR_DEST_OUT;
    स्थिती HB_PAINT_COMPOSITE_MODE_SRC_ATOP: परत CAIRO_OPERATOR_ATOP;
    स्थिती HB_PAINT_COMPOSITE_MODE_DEST_ATOP: परत CAIRO_OPERATOR_DEST_ATOP;
    स्थिती HB_PAINT_COMPOSITE_MODE_XOR: परत CAIRO_OPERATOR_XOR;
    स्थिती HB_PAINT_COMPOSITE_MODE_PLUS: परत CAIRO_OPERATOR_ADD;
    स्थिती HB_PAINT_COMPOSITE_MODE_SCREEN: परत CAIRO_OPERATOR_SCREEN;
    स्थिती HB_PAINT_COMPOSITE_MODE_OVERLAY: परत CAIRO_OPERATOR_OVERLAY;
    स्थिती HB_PAINT_COMPOSITE_MODE_DARKEN: परत CAIRO_OPERATOR_DARKEN;
    स्थिती HB_PAINT_COMPOSITE_MODE_LIGHTEN: परत CAIRO_OPERATOR_LIGHTEN;
    स्थिती HB_PAINT_COMPOSITE_MODE_COLOR_DODGE: परत CAIRO_OPERATOR_COLOR_DODGE;
    स्थिती HB_PAINT_COMPOSITE_MODE_COLOR_BURN: परत CAIRO_OPERATOR_COLOR_BURN;
    स्थिती HB_PAINT_COMPOSITE_MODE_HARD_LIGHT: परत CAIRO_OPERATOR_HARD_LIGHT;
    स्थिती HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT: परत CAIRO_OPERATOR_SOFT_LIGHT;
    स्थिती HB_PAINT_COMPOSITE_MODE_DIFFERENCE: परत CAIRO_OPERATOR_DIFFERENCE;
    स्थिती HB_PAINT_COMPOSITE_MODE_EXCLUSION: परत CAIRO_OPERATOR_EXCLUSION;
    स्थिती HB_PAINT_COMPOSITE_MODE_MULTIPLY: परत CAIRO_OPERATOR_MULTIPLY;
    स्थिती HB_PAINT_COMPOSITE_MODE_HSL_HUE: परत CAIRO_OPERATOR_HSL_HUE;
    स्थिती HB_PAINT_COMPOSITE_MODE_HSL_SATURATION: परत CAIRO_OPERATOR_HSL_SATURATION;
    स्थिती HB_PAINT_COMPOSITE_MODE_HSL_COLOR: परत CAIRO_OPERATOR_HSL_COLOR;
    स्थिती HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY: परत CAIRO_OPERATOR_HSL_LUMINOSITY;
    पूर्वनिर्धारित: परत CAIRO_OPERATOR_CLEAR;
    }
}

HB_INTERNAL शून्य
_hb_cairo_set_source_color (hb_cairo_context_t *c,
			    hb_bool_t use_foreground,
			    hb_color_t color);

HB_INTERNAL hb_bool_t
_hb_cairo_paint_glyph_image (hb_cairo_context_t *c,
			     hb_blob_t *blob,
			     अचिन्हित width,
			     अचिन्हित height,
			     hb_tag_t format,
			     दशांश slant,
			     hb_glyph_extents_t *extents);

HB_INTERNAL शून्य
_hb_cairo_paint_linear_gradient (hb_cairo_context_t *c,
				 hb_color_ओळ_t *color_ओळ,
				 दशांश x0, दशांश y0,
				 दशांश x1, दशांश y1,
				 दशांश x2, दशांश y2);

HB_INTERNAL शून्य
_hb_cairo_paint_radial_gradient (hb_cairo_context_t *c,
				 hb_color_ओळ_t *color_ओळ,
				 दशांश x0, दशांश y0, दशांश r0,
				 दशांश x1, दशांश y1, दशांश r1);

HB_INTERNAL शून्य
_hb_cairo_paint_sweep_gradient (hb_cairo_context_t *c,
				hb_color_ओळ_t *color_ओळ,
				दशांश x0, दशांश y0,
				दशांश start_angle, दशांश end_angle);


#endif /* HB_CAIRO_UTILS_H */

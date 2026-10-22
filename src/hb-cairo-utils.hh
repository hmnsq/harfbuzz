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

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-cairo.h"


ಸಂರಚನಾ hb_cairo_budget_t
{
  /* Cairo serializes glyph-cache population for a scaled font.  Keeping the
   * owner there lets recursive color-glyph callbacks share one session. */
  int64_t budget = HB_BUDGET_DEFAULT;
  int64_t budget_remaining = HB_BUDGET_GLYPH;
  unsigned ಪೂರ್ಣಾಙ್ಕ active = 0;

  ಶೂನ್ಯ enter ()
  {
    ಯದಿ (!active++)
      recharge ();
  }

  ಶೂನ್ಯ leave ()
  {
    ಪ್ರತಿಜ್ಞಾ (active);
    active--;
  }

  ಶೂನ್ಯ recharge ()
  {
    budget_remaining = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }
};

ಸಂರಚನಾ hb_cairo_context_t
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

  ತಾರ್ಕಿಕ set_budget (int64_t value)
  {
    budget->budget = value;
    budget->recharge ();
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  int64_t get_budget () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ budget->budget; }
  int64_t *get_budget_remaining () { ಪ್ರತ್ಯಾಗಮ &budget->budget_remaining; }

  ತಾರ್ಕಿಕ spend (unsigned ಪೂರ್ಣಾಙ್ಕ cost, unsigned ಪೂರ್ಣಾಙ್ಕ mult = 1)
  { ಪ್ರತ್ಯಾಗಮ hb_budget_spend (budget->budget_remaining, cost, mult); }

  cairo_scaled_font_t *scaled_font;
  cairo_t *cr;
  hb_map_t *color_cache;
  hb_cairo_budget_t *budget;
};

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ cairo_operator_t
_hb_paint_composite_mode_to_cairo (hb_paint_composite_mode_t mode)
{
  ವಿಕಲ್ಪ (mode)
    {
    ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_CLEAR: ಪ್ರತ್ಯಾಗಮ CAIRO_OPERATOR_CLEAR;
    ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_SRC: ಪ್ರತ್ಯಾಗಮ CAIRO_OPERATOR_SOURCE;
    ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_DEST: ಪ್ರತ್ಯಾಗಮ CAIRO_OPERATOR_DEST;
    ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_SRC_OVER: ಪ್ರತ್ಯಾಗಮ CAIRO_OPERATOR_OVER;
    ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_DEST_OVER: ಪ್ರತ್ಯಾಗಮ CAIRO_OPERATOR_DEST_OVER;
    ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_SRC_IN: ಪ್ರತ್ಯಾಗಮ CAIRO_OPERATOR_IN;
    ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_DEST_IN: ಪ್ರತ್ಯಾಗಮ CAIRO_OPERATOR_DEST_IN;
    ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_SRC_OUT: ಪ್ರತ್ಯಾಗಮ CAIRO_OPERATOR_OUT;
    ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_DEST_OUT: ಪ್ರತ್ಯಾಗಮ CAIRO_OPERATOR_DEST_OUT;
    ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_SRC_ATOP: ಪ್ರತ್ಯಾಗಮ CAIRO_OPERATOR_ATOP;
    ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_DEST_ATOP: ಪ್ರತ್ಯಾಗಮ CAIRO_OPERATOR_DEST_ATOP;
    ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_XOR: ಪ್ರತ್ಯಾಗಮ CAIRO_OPERATOR_XOR;
    ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_PLUS: ಪ್ರತ್ಯಾಗಮ CAIRO_OPERATOR_ADD;
    ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_SCREEN: ಪ್ರತ್ಯಾಗಮ CAIRO_OPERATOR_SCREEN;
    ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_OVERLAY: ಪ್ರತ್ಯಾಗಮ CAIRO_OPERATOR_OVERLAY;
    ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_DARKEN: ಪ್ರತ್ಯಾಗಮ CAIRO_OPERATOR_DARKEN;
    ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_LIGHTEN: ಪ್ರತ್ಯಾಗಮ CAIRO_OPERATOR_LIGHTEN;
    ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_COLOR_DODGE: ಪ್ರತ್ಯಾಗಮ CAIRO_OPERATOR_COLOR_DODGE;
    ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_COLOR_BURN: ಪ್ರತ್ಯಾಗಮ CAIRO_OPERATOR_COLOR_BURN;
    ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_HARD_LIGHT: ಪ್ರತ್ಯಾಗಮ CAIRO_OPERATOR_HARD_LIGHT;
    ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT: ಪ್ರತ್ಯಾಗಮ CAIRO_OPERATOR_SOFT_LIGHT;
    ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_DIFFERENCE: ಪ್ರತ್ಯಾಗಮ CAIRO_OPERATOR_DIFFERENCE;
    ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_EXCLUSION: ಪ್ರತ್ಯಾಗಮ CAIRO_OPERATOR_EXCLUSION;
    ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_MULTIPLY: ಪ್ರತ್ಯಾಗಮ CAIRO_OPERATOR_MULTIPLY;
    ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_HSL_HUE: ಪ್ರತ್ಯಾಗಮ CAIRO_OPERATOR_HSL_HUE;
    ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_HSL_SATURATION: ಪ್ರತ್ಯಾಗಮ CAIRO_OPERATOR_HSL_SATURATION;
    ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_HSL_COLOR: ಪ್ರತ್ಯಾಗಮ CAIRO_OPERATOR_HSL_COLOR;
    ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY: ಪ್ರತ್ಯಾಗಮ CAIRO_OPERATOR_HSL_LUMINOSITY;
    ಸ್ವಭಾವ: ಪ್ರತ್ಯಾಗಮ CAIRO_OPERATOR_CLEAR;
    }
}

HB_INTERNAL ಶೂನ್ಯ
_hb_cairo_set_source_color (hb_cairo_context_t *c,
			    hb_bool_t use_foreground,
			    hb_color_t color);

HB_INTERNAL hb_bool_t
_hb_cairo_paint_glyph_image (hb_cairo_context_t *c,
			     hb_blob_t *blob,
			     unsigned width,
			     unsigned height,
			     hb_tag_t format,
			     ಭಿನ್ನ slant,
			     hb_glyph_extents_t *extents);

HB_INTERNAL ಶೂನ್ಯ
_hb_cairo_paint_linear_gradient (hb_cairo_context_t *c,
				 hb_color_ಸಾಲು_t *color_ಸಾಲು,
				 ಭಿನ್ನ x0, ಭಿನ್ನ y0,
				 ಭಿನ್ನ x1, ಭಿನ್ನ y1,
				 ಭಿನ್ನ x2, ಭಿನ್ನ y2);

HB_INTERNAL ಶೂನ್ಯ
_hb_cairo_paint_radial_gradient (hb_cairo_context_t *c,
				 hb_color_ಸಾಲು_t *color_ಸಾಲು,
				 ಭಿನ್ನ x0, ಭಿನ್ನ y0, ಭಿನ್ನ r0,
				 ಭಿನ್ನ x1, ಭಿನ್ನ y1, ಭಿನ್ನ r1);

HB_INTERNAL ಶೂನ್ಯ
_hb_cairo_paint_sweep_gradient (hb_cairo_context_t *c,
				hb_color_ಸಾಲು_t *color_ಸಾಲು,
				ಭಿನ್ನ x0, ಭಿನ್ನ y0,
				ಭಿನ್ನ start_angle, ಭಿನ್ನ end_angle);


#endif /* HB_CAIRO_UTILS_H */

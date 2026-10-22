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

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-cairo.h"


ഘടന hb_cairo_budget_t
{
  /* Cairo serializes glyph-cache population for a scaled font.  Keeping the
   * owner there lets recursive color-glyph callbacks share one session. */
  int64_t budget = HB_BUDGET_DEFAULT;
  int64_t budget_remaining = HB_BUDGET_GLYPH;
  unsigned പൂർണ്ണസംഖ്യ active = 0;

  ശൂന്യം enter ()
  {
    എങ്കിൽ (!active++)
      recharge ();
  }

  ശൂന്യം leave ()
  {
    പ്രതിജ്ഞാ (active);
    active--;
  }

  ശൂന്യം recharge ()
  {
    budget_remaining = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }
};

ഘടന hb_cairo_context_t
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

  തർക്കം set_budget (int64_t value)
  {
    budget->budget = value;
    budget->recharge ();
    തിരികെ സത്യം;
  }

  int64_t get_budget () സ്ഥിരം { തിരികെ budget->budget; }
  int64_t *get_budget_remaining () { തിരികെ &budget->budget_remaining; }

  തർക്കം spend (unsigned പൂർണ്ണസംഖ്യ cost, unsigned പൂർണ്ണസംഖ്യ mult = 1)
  { തിരികെ hb_budget_spend (budget->budget_remaining, cost, mult); }

  cairo_scaled_font_t *scaled_font;
  cairo_t *cr;
  hb_map_t *color_cache;
  hb_cairo_budget_t *budget;
};

സ്ഥിതം വരിയിൽ cairo_operator_t
_hb_paint_composite_mode_to_cairo (hb_paint_composite_mode_t mode)
{
  തിരഞ്ഞെടുപ്പ് (mode)
    {
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_CLEAR: തിരികെ CAIRO_OPERATOR_CLEAR;
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_SRC: തിരികെ CAIRO_OPERATOR_SOURCE;
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_DEST: തിരികെ CAIRO_OPERATOR_DEST;
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_SRC_OVER: തിരികെ CAIRO_OPERATOR_OVER;
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_DEST_OVER: തിരികെ CAIRO_OPERATOR_DEST_OVER;
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_SRC_IN: തിരികെ CAIRO_OPERATOR_IN;
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_DEST_IN: തിരികെ CAIRO_OPERATOR_DEST_IN;
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_SRC_OUT: തിരികെ CAIRO_OPERATOR_OUT;
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_DEST_OUT: തിരികെ CAIRO_OPERATOR_DEST_OUT;
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_SRC_ATOP: തിരികെ CAIRO_OPERATOR_ATOP;
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_DEST_ATOP: തിരികെ CAIRO_OPERATOR_DEST_ATOP;
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_XOR: തിരികെ CAIRO_OPERATOR_XOR;
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_PLUS: തിരികെ CAIRO_OPERATOR_ADD;
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_SCREEN: തിരികെ CAIRO_OPERATOR_SCREEN;
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_OVERLAY: തിരികെ CAIRO_OPERATOR_OVERLAY;
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_DARKEN: തിരികെ CAIRO_OPERATOR_DARKEN;
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_LIGHTEN: തിരികെ CAIRO_OPERATOR_LIGHTEN;
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_COLOR_DODGE: തിരികെ CAIRO_OPERATOR_COLOR_DODGE;
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_COLOR_BURN: തിരികെ CAIRO_OPERATOR_COLOR_BURN;
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_HARD_LIGHT: തിരികെ CAIRO_OPERATOR_HARD_LIGHT;
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT: തിരികെ CAIRO_OPERATOR_SOFT_LIGHT;
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_DIFFERENCE: തിരികെ CAIRO_OPERATOR_DIFFERENCE;
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_EXCLUSION: തിരികെ CAIRO_OPERATOR_EXCLUSION;
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_MULTIPLY: തിരികെ CAIRO_OPERATOR_MULTIPLY;
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_HSL_HUE: തിരികെ CAIRO_OPERATOR_HSL_HUE;
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_HSL_SATURATION: തിരികെ CAIRO_OPERATOR_HSL_SATURATION;
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_HSL_COLOR: തിരികെ CAIRO_OPERATOR_HSL_COLOR;
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY: തിരികെ CAIRO_OPERATOR_HSL_LUMINOSITY;
    സ്ഥിരസ്ഥിതി: തിരികെ CAIRO_OPERATOR_CLEAR;
    }
}

HB_INTERNAL ശൂന്യം
_hb_cairo_set_source_color (hb_cairo_context_t *c,
			    hb_bool_t use_foreground,
			    hb_color_t color);

HB_INTERNAL hb_bool_t
_hb_cairo_paint_glyph_image (hb_cairo_context_t *c,
			     hb_blob_t *blob,
			     unsigned width,
			     unsigned height,
			     hb_tag_t format,
			     ദശാംശം slant,
			     hb_glyph_extents_t *extents);

HB_INTERNAL ശൂന്യം
_hb_cairo_paint_linear_gradient (hb_cairo_context_t *c,
				 hb_color_വരി_t *color_വരി,
				 ദശാംശം x0, ദശാംശം y0,
				 ദശാംശം x1, ദശാംശം y1,
				 ദശാംശം x2, ദശാംശം y2);

HB_INTERNAL ശൂന്യം
_hb_cairo_paint_radial_gradient (hb_cairo_context_t *c,
				 hb_color_വരി_t *color_വരി,
				 ദശാംശം x0, ദശാംശം y0, ദശാംശം r0,
				 ദശാംശം x1, ദശാംശം y1, ദശാംശം r1);

HB_INTERNAL ശൂന്യം
_hb_cairo_paint_sweep_gradient (hb_cairo_context_t *c,
				hb_color_വരി_t *color_വരി,
				ദശാംശം x0, ദശാംശം y0,
				ദശാംശം start_angle, ദശാംശം end_angle);


#endif /* HB_CAIRO_UTILS_H */

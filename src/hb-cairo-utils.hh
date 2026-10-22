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

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-cairo.h"


ସଂରଚନା hb_cairo_budget_t
{
  /* Cairo serializes glyph-cache population for a scaled font.  Keeping the
   * owner there lets recursive color-glyph callbacks share one session. */
  int64_t budget = HB_BUDGET_DEFAULT;
  int64_t budget_remaining = HB_BUDGET_GLYPH;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା active = 0;

  ଶୂନ୍ୟ enter ()
  {
    ଯଦି (!active++)
      recharge ();
  }

  ଶୂନ୍ୟ leave ()
  {
    ପ୍ରତିଜ୍ଞା (active);
    active--;
  }

  ଶୂନ୍ୟ recharge ()
  {
    budget_remaining = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }
};

ସଂରଚନା hb_cairo_context_t
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

  ତାର୍କିକ set_budget (int64_t value)
  {
    budget->budget = value;
    budget->recharge ();
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  int64_t get_budget () ଅଚଳ { ଫେରନ୍ତୁ budget->budget; }
  int64_t *get_budget_remaining () { ଫେରନ୍ତୁ &budget->budget_remaining; }

  ତାର୍କିକ spend (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cost, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mult = 1)
  { ଫେରନ୍ତୁ hb_budget_spend (budget->budget_remaining, cost, mult); }

  cairo_scaled_font_t *scaled_font;
  cairo_t *cr;
  hb_map_t *color_cache;
  hb_cairo_budget_t *budget;
};

ସ୍ଥିର ଧାଡ଼ିଭିତରେ cairo_operator_t
_hb_paint_composite_mode_to_cairo (hb_paint_composite_mode_t mode)
{
  ବାଛ (mode)
    {
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_CLEAR: ଫେରନ୍ତୁ CAIRO_OPERATOR_CLEAR;
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_SRC: ଫେରନ୍ତୁ CAIRO_OPERATOR_SOURCE;
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_DEST: ଫେରନ୍ତୁ CAIRO_OPERATOR_DEST;
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_SRC_OVER: ଫେରନ୍ତୁ CAIRO_OPERATOR_OVER;
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_DEST_OVER: ଫେରନ୍ତୁ CAIRO_OPERATOR_DEST_OVER;
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_SRC_IN: ଫେରନ୍ତୁ CAIRO_OPERATOR_IN;
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_DEST_IN: ଫେରନ୍ତୁ CAIRO_OPERATOR_DEST_IN;
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_SRC_OUT: ଫେରନ୍ତୁ CAIRO_OPERATOR_OUT;
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_DEST_OUT: ଫେରନ୍ତୁ CAIRO_OPERATOR_DEST_OUT;
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_SRC_ATOP: ଫେରନ୍ତୁ CAIRO_OPERATOR_ATOP;
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_DEST_ATOP: ଫେରନ୍ତୁ CAIRO_OPERATOR_DEST_ATOP;
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_XOR: ଫେରନ୍ତୁ CAIRO_OPERATOR_XOR;
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_PLUS: ଫେରନ୍ତୁ CAIRO_OPERATOR_ADD;
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_SCREEN: ଫେରନ୍ତୁ CAIRO_OPERATOR_SCREEN;
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_OVERLAY: ଫେରନ୍ତୁ CAIRO_OPERATOR_OVERLAY;
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_DARKEN: ଫେରନ୍ତୁ CAIRO_OPERATOR_DARKEN;
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_LIGHTEN: ଫେରନ୍ତୁ CAIRO_OPERATOR_LIGHTEN;
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_COLOR_DODGE: ଫେରନ୍ତୁ CAIRO_OPERATOR_COLOR_DODGE;
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_COLOR_BURN: ଫେରନ୍ତୁ CAIRO_OPERATOR_COLOR_BURN;
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_HARD_LIGHT: ଫେରନ୍ତୁ CAIRO_OPERATOR_HARD_LIGHT;
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT: ଫେରନ୍ତୁ CAIRO_OPERATOR_SOFT_LIGHT;
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_DIFFERENCE: ଫେରନ୍ତୁ CAIRO_OPERATOR_DIFFERENCE;
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_EXCLUSION: ଫେରନ୍ତୁ CAIRO_OPERATOR_EXCLUSION;
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_MULTIPLY: ଫେରନ୍ତୁ CAIRO_OPERATOR_MULTIPLY;
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_HSL_HUE: ଫେରନ୍ତୁ CAIRO_OPERATOR_HSL_HUE;
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_HSL_SATURATION: ଫେରନ୍ତୁ CAIRO_OPERATOR_HSL_SATURATION;
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_HSL_COLOR: ଫେରନ୍ତୁ CAIRO_OPERATOR_HSL_COLOR;
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY: ଫେରନ୍ତୁ CAIRO_OPERATOR_HSL_LUMINOSITY;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ଫେରନ୍ତୁ CAIRO_OPERATOR_CLEAR;
    }
}

HB_INTERNAL ଶୂନ୍ୟ
_hb_cairo_set_source_color (hb_cairo_context_t *c,
			    hb_bool_t use_foreground,
			    hb_color_t color);

HB_INTERNAL hb_bool_t
_hb_cairo_paint_glyph_image (hb_cairo_context_t *c,
			     hb_blob_t *blob,
			     unsigned width,
			     unsigned height,
			     hb_tag_t format,
			     ଦଶମିକ slant,
			     hb_glyph_extents_t *extents);

HB_INTERNAL ଶୂନ୍ୟ
_hb_cairo_paint_linear_gradient (hb_cairo_context_t *c,
				 hb_color_ଧାଡ଼ି_t *color_ଧାଡ଼ି,
				 ଦଶମିକ x0, ଦଶମିକ y0,
				 ଦଶମିକ x1, ଦଶମିକ y1,
				 ଦଶମିକ x2, ଦଶମିକ y2);

HB_INTERNAL ଶୂନ୍ୟ
_hb_cairo_paint_radial_gradient (hb_cairo_context_t *c,
				 hb_color_ଧାଡ଼ି_t *color_ଧାଡ଼ି,
				 ଦଶମିକ x0, ଦଶମିକ y0, ଦଶମିକ r0,
				 ଦଶମିକ x1, ଦଶମିକ y1, ଦଶମିକ r1);

HB_INTERNAL ଶୂନ୍ୟ
_hb_cairo_paint_sweep_gradient (hb_cairo_context_t *c,
				hb_color_ଧାଡ଼ି_t *color_ଧାଡ଼ି,
				ଦଶମିକ x0, ଦଶମିକ y0,
				ଦଶମିକ start_angle, ଦଶମିକ end_angle);


#endif /* HB_CAIRO_UTILS_H */

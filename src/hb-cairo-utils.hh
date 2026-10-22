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

#અન્તર્ભાવ "hb.hh"
#અન્તર્ભાવ "hb-cairo.h"


સંરચના hb_cairo_budget_t
{
  /* Cairo serializes glyph-cache population for a scaled font.  Keeping the
   * owner there lets recursive color-glyph callbacks share one session. */
  int64_t budget = HB_BUDGET_DEFAULT;
  int64_t budget_remaining = HB_BUDGET_GLYPH;
  unsigned પૂર્ણાંક active = 0;

  શૂન્ય enter ()
  {
    જો (!active++)
      recharge ();
  }

  શૂન્ય leave ()
  {
    પ્રતિજ્ઞા (active);
    active--;
  }

  શૂન્ય recharge ()
  {
    budget_remaining = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }
};

સંરચના hb_cairo_context_t
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

  તાર્કિક set_budget (int64_t value)
  {
    budget->budget = value;
    budget->recharge ();
    પાછા સાચું;
  }

  int64_t get_budget () અચલ { પાછા budget->budget; }
  int64_t *get_budget_remaining () { પાછા &budget->budget_remaining; }

  તાર્કિક spend (unsigned પૂર્ણાંક cost, unsigned પૂર્ણાંક mult = 1)
  { પાછા hb_budget_spend (budget->budget_remaining, cost, mult); }

  cairo_scaled_font_t *scaled_font;
  cairo_t *cr;
  hb_map_t *color_cache;
  hb_cairo_budget_t *budget;
};

સ્થિર અંતઃરેખ cairo_operator_t
_hb_paint_composite_mode_to_cairo (hb_paint_composite_mode_t mode)
{
  પસંદગી (mode)
    {
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_CLEAR: પાછા CAIRO_OPERATOR_CLEAR;
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_SRC: પાછા CAIRO_OPERATOR_SOURCE;
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_DEST: પાછા CAIRO_OPERATOR_DEST;
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_SRC_OVER: પાછા CAIRO_OPERATOR_OVER;
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_DEST_OVER: પાછા CAIRO_OPERATOR_DEST_OVER;
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_SRC_IN: પાછા CAIRO_OPERATOR_IN;
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_DEST_IN: પાછા CAIRO_OPERATOR_DEST_IN;
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_SRC_OUT: પાછા CAIRO_OPERATOR_OUT;
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_DEST_OUT: પાછા CAIRO_OPERATOR_DEST_OUT;
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_SRC_ATOP: પાછા CAIRO_OPERATOR_ATOP;
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_DEST_ATOP: પાછા CAIRO_OPERATOR_DEST_ATOP;
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_XOR: પાછા CAIRO_OPERATOR_XOR;
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_PLUS: પાછા CAIRO_OPERATOR_ADD;
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_SCREEN: પાછા CAIRO_OPERATOR_SCREEN;
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_OVERLAY: પાછા CAIRO_OPERATOR_OVERLAY;
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_DARKEN: પાછા CAIRO_OPERATOR_DARKEN;
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_LIGHTEN: પાછા CAIRO_OPERATOR_LIGHTEN;
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_COLOR_DODGE: પાછા CAIRO_OPERATOR_COLOR_DODGE;
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_COLOR_BURN: પાછા CAIRO_OPERATOR_COLOR_BURN;
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_HARD_LIGHT: પાછા CAIRO_OPERATOR_HARD_LIGHT;
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT: પાછા CAIRO_OPERATOR_SOFT_LIGHT;
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_DIFFERENCE: પાછા CAIRO_OPERATOR_DIFFERENCE;
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_EXCLUSION: પાછા CAIRO_OPERATOR_EXCLUSION;
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_MULTIPLY: પાછા CAIRO_OPERATOR_MULTIPLY;
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_HSL_HUE: પાછા CAIRO_OPERATOR_HSL_HUE;
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_HSL_SATURATION: પાછા CAIRO_OPERATOR_HSL_SATURATION;
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_HSL_COLOR: પાછા CAIRO_OPERATOR_HSL_COLOR;
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY: પાછા CAIRO_OPERATOR_HSL_LUMINOSITY;
    મૂળભૂત: પાછા CAIRO_OPERATOR_CLEAR;
    }
}

HB_INTERNAL શૂન્ય
_hb_cairo_set_source_color (hb_cairo_context_t *c,
			    hb_bool_t use_foreground,
			    hb_color_t color);

HB_INTERNAL hb_bool_t
_hb_cairo_paint_glyph_image (hb_cairo_context_t *c,
			     hb_blob_t *blob,
			     unsigned width,
			     unsigned height,
			     hb_tag_t format,
			     દશાંશ slant,
			     hb_glyph_extents_t *extents);

HB_INTERNAL શૂન્ય
_hb_cairo_paint_linear_gradient (hb_cairo_context_t *c,
				 hb_color_લીટી_t *color_લીટી,
				 દશાંશ x0, દશાંશ y0,
				 દશાંશ x1, દશાંશ y1,
				 દશાંશ x2, દશાંશ y2);

HB_INTERNAL શૂન્ય
_hb_cairo_paint_radial_gradient (hb_cairo_context_t *c,
				 hb_color_લીટી_t *color_લીટી,
				 દશાંશ x0, દશાંશ y0, દશાંશ r0,
				 દશાંશ x1, દશાંશ y1, દશાંશ r1);

HB_INTERNAL શૂન્ય
_hb_cairo_paint_sweep_gradient (hb_cairo_context_t *c,
				hb_color_લીટી_t *color_લીટી,
				દશાંશ x0, દશાંશ y0,
				દશાંશ start_angle, દશાંશ end_angle);


#endif /* HB_CAIRO_UTILS_H */

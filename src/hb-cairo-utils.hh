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

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-cairo.h"


কাঠামো hb_cairo_budget_t
{
  /* Cairo serializes glyph-cache population for a scaled font.  Keeping the
   * owner there lets recursive color-glyph callbacks share one session. */
  int64_t budget = HB_BUDGET_DEFAULT;
  int64_t budget_remaining = HB_BUDGET_GLYPH;
  unsigned পূর্ণসংখ্যা active = 0;

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

কাঠামো hb_cairo_context_t
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

  যৌক্তিক set_budget (int64_t value)
  {
    budget->budget = value;
    budget->recharge ();
    ফেরত সত্য;
  }

  int64_t get_budget () ধ্রুবক { ফেরত budget->budget; }
  int64_t *get_budget_remaining () { ফেরত &budget->budget_remaining; }

  যৌক্তিক spend (unsigned পূর্ণসংখ্যা cost, unsigned পূর্ণসংখ্যা mult = 1)
  { ফেরত hb_budget_spend (budget->budget_remaining, cost, mult); }

  cairo_scaled_font_t *scaled_font;
  cairo_t *cr;
  hb_map_t *color_cache;
  hb_cairo_budget_t *budget;
};

স্থির অন্তঃরেখ cairo_operator_t
_hb_paint_composite_mode_to_cairo (hb_paint_composite_mode_t mode)
{
  নির্বাচন (mode)
    {
    ক্ষেত্র HB_PAINT_COMPOSITE_MODE_CLEAR: ফেরত CAIRO_OPERATOR_CLEAR;
    ক্ষেত্র HB_PAINT_COMPOSITE_MODE_SRC: ফেরত CAIRO_OPERATOR_SOURCE;
    ক্ষেত্র HB_PAINT_COMPOSITE_MODE_DEST: ফেরত CAIRO_OPERATOR_DEST;
    ক্ষেত্র HB_PAINT_COMPOSITE_MODE_SRC_OVER: ফেরত CAIRO_OPERATOR_OVER;
    ক্ষেত্র HB_PAINT_COMPOSITE_MODE_DEST_OVER: ফেরত CAIRO_OPERATOR_DEST_OVER;
    ক্ষেত্র HB_PAINT_COMPOSITE_MODE_SRC_IN: ফেরত CAIRO_OPERATOR_IN;
    ক্ষেত্র HB_PAINT_COMPOSITE_MODE_DEST_IN: ফেরত CAIRO_OPERATOR_DEST_IN;
    ক্ষেত্র HB_PAINT_COMPOSITE_MODE_SRC_OUT: ফেরত CAIRO_OPERATOR_OUT;
    ক্ষেত্র HB_PAINT_COMPOSITE_MODE_DEST_OUT: ফেরত CAIRO_OPERATOR_DEST_OUT;
    ক্ষেত্র HB_PAINT_COMPOSITE_MODE_SRC_ATOP: ফেরত CAIRO_OPERATOR_ATOP;
    ক্ষেত্র HB_PAINT_COMPOSITE_MODE_DEST_ATOP: ফেরত CAIRO_OPERATOR_DEST_ATOP;
    ক্ষেত্র HB_PAINT_COMPOSITE_MODE_XOR: ফেরত CAIRO_OPERATOR_XOR;
    ক্ষেত্র HB_PAINT_COMPOSITE_MODE_PLUS: ফেরত CAIRO_OPERATOR_ADD;
    ক্ষেত্র HB_PAINT_COMPOSITE_MODE_SCREEN: ফেরত CAIRO_OPERATOR_SCREEN;
    ক্ষেত্র HB_PAINT_COMPOSITE_MODE_OVERLAY: ফেরত CAIRO_OPERATOR_OVERLAY;
    ক্ষেত্র HB_PAINT_COMPOSITE_MODE_DARKEN: ফেরত CAIRO_OPERATOR_DARKEN;
    ক্ষেত্র HB_PAINT_COMPOSITE_MODE_LIGHTEN: ফেরত CAIRO_OPERATOR_LIGHTEN;
    ক্ষেত্র HB_PAINT_COMPOSITE_MODE_COLOR_DODGE: ফেরত CAIRO_OPERATOR_COLOR_DODGE;
    ক্ষেত্র HB_PAINT_COMPOSITE_MODE_COLOR_BURN: ফেরত CAIRO_OPERATOR_COLOR_BURN;
    ক্ষেত্র HB_PAINT_COMPOSITE_MODE_HARD_LIGHT: ফেরত CAIRO_OPERATOR_HARD_LIGHT;
    ক্ষেত্র HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT: ফেরত CAIRO_OPERATOR_SOFT_LIGHT;
    ক্ষেত্র HB_PAINT_COMPOSITE_MODE_DIFFERENCE: ফেরত CAIRO_OPERATOR_DIFFERENCE;
    ক্ষেত্র HB_PAINT_COMPOSITE_MODE_EXCLUSION: ফেরত CAIRO_OPERATOR_EXCLUSION;
    ক্ষেত্র HB_PAINT_COMPOSITE_MODE_MULTIPLY: ফেরত CAIRO_OPERATOR_MULTIPLY;
    ক্ষেত্র HB_PAINT_COMPOSITE_MODE_HSL_HUE: ফেরত CAIRO_OPERATOR_HSL_HUE;
    ক্ষেত্র HB_PAINT_COMPOSITE_MODE_HSL_SATURATION: ফেরত CAIRO_OPERATOR_HSL_SATURATION;
    ক্ষেত্র HB_PAINT_COMPOSITE_MODE_HSL_COLOR: ফেরত CAIRO_OPERATOR_HSL_COLOR;
    ক্ষেত্র HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY: ফেরত CAIRO_OPERATOR_HSL_LUMINOSITY;
    পূর্বনির্ধারিত: ফেরত CAIRO_OPERATOR_CLEAR;
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
			     দশমিক slant,
			     hb_glyph_extents_t *extents);

HB_INTERNAL শূন্য
_hb_cairo_paint_linear_gradient (hb_cairo_context_t *c,
				 hb_color_সারি_t *color_সারি,
				 দশমিক x0, দশমিক y0,
				 দশমিক x1, দশমিক y1,
				 দশমিক x2, দশমিক y2);

HB_INTERNAL শূন্য
_hb_cairo_paint_radial_gradient (hb_cairo_context_t *c,
				 hb_color_সারি_t *color_সারি,
				 দশমিক x0, দশমিক y0, দশমিক r0,
				 দশমিক x1, দশমিক y1, দশমিক r1);

HB_INTERNAL শূন্য
_hb_cairo_paint_sweep_gradient (hb_cairo_context_t *c,
				hb_color_সারি_t *color_সারি,
				দশমিক x0, দশমিক y0,
				দশমিক start_angle, দশমিক end_angle);


#endif /* HB_CAIRO_UTILS_H */

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
  unsigned पूर्णाङ्क active = 0;

  शून्य enter ()
  {
    यदि (!active++)
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
    प्रत्यागम सत्य;
  }

  int64_t get_budget () ध्रुव { प्रत्यागम budget->budget; }
  int64_t *get_budget_remaining () { प्रत्यागम &budget->budget_remaining; }

  तार्किक spend (unsigned पूर्णाङ्क cost, unsigned पूर्णाङ्क mult = 1)
  { प्रत्यागम hb_budget_spend (budget->budget_remaining, cost, mult); }

  cairo_scaled_font_t *scaled_font;
  cairo_t *cr;
  hb_map_t *color_cache;
  hb_cairo_budget_t *budget;
};

स्थिर अन्तःस्थ cairo_operator_t
_hb_paint_composite_mode_to_cairo (hb_paint_composite_mode_t mode)
{
  विकल्प (mode)
    {
    पक्ष HB_PAINT_COMPOSITE_MODE_CLEAR: प्रत्यागम CAIRO_OPERATOR_CLEAR;
    पक्ष HB_PAINT_COMPOSITE_MODE_SRC: प्रत्यागम CAIRO_OPERATOR_SOURCE;
    पक्ष HB_PAINT_COMPOSITE_MODE_DEST: प्रत्यागम CAIRO_OPERATOR_DEST;
    पक्ष HB_PAINT_COMPOSITE_MODE_SRC_OVER: प्रत्यागम CAIRO_OPERATOR_OVER;
    पक्ष HB_PAINT_COMPOSITE_MODE_DEST_OVER: प्रत्यागम CAIRO_OPERATOR_DEST_OVER;
    पक्ष HB_PAINT_COMPOSITE_MODE_SRC_IN: प्रत्यागम CAIRO_OPERATOR_IN;
    पक्ष HB_PAINT_COMPOSITE_MODE_DEST_IN: प्रत्यागम CAIRO_OPERATOR_DEST_IN;
    पक्ष HB_PAINT_COMPOSITE_MODE_SRC_OUT: प्रत्यागम CAIRO_OPERATOR_OUT;
    पक्ष HB_PAINT_COMPOSITE_MODE_DEST_OUT: प्रत्यागम CAIRO_OPERATOR_DEST_OUT;
    पक्ष HB_PAINT_COMPOSITE_MODE_SRC_ATOP: प्रत्यागम CAIRO_OPERATOR_ATOP;
    पक्ष HB_PAINT_COMPOSITE_MODE_DEST_ATOP: प्रत्यागम CAIRO_OPERATOR_DEST_ATOP;
    पक्ष HB_PAINT_COMPOSITE_MODE_XOR: प्रत्यागम CAIRO_OPERATOR_XOR;
    पक्ष HB_PAINT_COMPOSITE_MODE_PLUS: प्रत्यागम CAIRO_OPERATOR_ADD;
    पक्ष HB_PAINT_COMPOSITE_MODE_SCREEN: प्रत्यागम CAIRO_OPERATOR_SCREEN;
    पक्ष HB_PAINT_COMPOSITE_MODE_OVERLAY: प्रत्यागम CAIRO_OPERATOR_OVERLAY;
    पक्ष HB_PAINT_COMPOSITE_MODE_DARKEN: प्रत्यागम CAIRO_OPERATOR_DARKEN;
    पक्ष HB_PAINT_COMPOSITE_MODE_LIGHTEN: प्रत्यागम CAIRO_OPERATOR_LIGHTEN;
    पक्ष HB_PAINT_COMPOSITE_MODE_COLOR_DODGE: प्रत्यागम CAIRO_OPERATOR_COLOR_DODGE;
    पक्ष HB_PAINT_COMPOSITE_MODE_COLOR_BURN: प्रत्यागम CAIRO_OPERATOR_COLOR_BURN;
    पक्ष HB_PAINT_COMPOSITE_MODE_HARD_LIGHT: प्रत्यागम CAIRO_OPERATOR_HARD_LIGHT;
    पक्ष HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT: प्रत्यागम CAIRO_OPERATOR_SOFT_LIGHT;
    पक्ष HB_PAINT_COMPOSITE_MODE_DIFFERENCE: प्रत्यागम CAIRO_OPERATOR_DIFFERENCE;
    पक्ष HB_PAINT_COMPOSITE_MODE_EXCLUSION: प्रत्यागम CAIRO_OPERATOR_EXCLUSION;
    पक्ष HB_PAINT_COMPOSITE_MODE_MULTIPLY: प्रत्यागम CAIRO_OPERATOR_MULTIPLY;
    पक्ष HB_PAINT_COMPOSITE_MODE_HSL_HUE: प्रत्यागम CAIRO_OPERATOR_HSL_HUE;
    पक्ष HB_PAINT_COMPOSITE_MODE_HSL_SATURATION: प्रत्यागम CAIRO_OPERATOR_HSL_SATURATION;
    पक्ष HB_PAINT_COMPOSITE_MODE_HSL_COLOR: प्रत्यागम CAIRO_OPERATOR_HSL_COLOR;
    पक्ष HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY: प्रत्यागम CAIRO_OPERATOR_HSL_LUMINOSITY;
    स्वभाव: प्रत्यागम CAIRO_OPERATOR_CLEAR;
    }
}

HB_INTERNAL शून्य
_hb_cairo_set_source_color (hb_cairo_context_t *c,
			    hb_bool_t use_foreground,
			    hb_color_t color);

HB_INTERNAL hb_bool_t
_hb_cairo_paint_glyph_image (hb_cairo_context_t *c,
			     hb_blob_t *blob,
			     unsigned width,
			     unsigned height,
			     hb_tag_t format,
			     भिन्न slant,
			     hb_glyph_extents_t *extents);

HB_INTERNAL शून्य
_hb_cairo_paint_linear_gradient (hb_cairo_context_t *c,
				 hb_color_पंक्ति_t *color_पंक्ति,
				 भिन्न x0, भिन्न y0,
				 भिन्न x1, भिन्न y1,
				 भिन्न x2, भिन्न y2);

HB_INTERNAL शून्य
_hb_cairo_paint_radial_gradient (hb_cairo_context_t *c,
				 hb_color_पंक्ति_t *color_पंक्ति,
				 भिन्न x0, भिन्न y0, भिन्न r0,
				 भिन्न x1, भिन्न y1, भिन्न r1);

HB_INTERNAL शून्य
_hb_cairo_paint_sweep_gradient (hb_cairo_context_t *c,
				hb_color_पंक्ति_t *color_पंक्ति,
				भिन्न x0, भिन्न y0,
				भिन्न start_angle, भिन्न end_angle);


#endif /* HB_CAIRO_UTILS_H */

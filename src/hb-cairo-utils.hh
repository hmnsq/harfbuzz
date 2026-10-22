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

#अगर_अघोषित HB_CAIRO_UTILS_H
#घोषणा HB_CAIRO_UTILS_H

#समावेश "hb.hh"
#समावेश "hb-cairo.h"


काष्ठा hb_cairo_budget_t
{
  /* Cairo serializes glyph-cache population for a scaled font.  Keeping the
   * owner there lets recursive color-glyph callbacks share one session. */
  int64_t budget = HB_BUDGET_DEFAULT;
  int64_t budget_remaining = HB_BUDGET_GLYPH;
  अचिन्हित पूर्णांक active = 0;

  व्योम enter ()
  {
    अगर (!active++)
      recharge ();
  }

  व्योम leave ()
  {
    निश्चित (active);
    active--;
  }

  व्योम recharge ()
  {
    budget_remaining = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }
};

काष्ठा hb_cairo_context_t
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
    वापस true;
  }

  int64_t get_budget () स्थिर { वापस budget->budget; }
  int64_t *get_budget_remaining () { वापस &budget->budget_remaining; }

  bool spend (अचिन्हित पूर्णांक cost, अचिन्हित पूर्णांक mult = 1)
  { वापस hb_budget_spend (budget->budget_remaining, cost, mult); }

  cairo_scaled_font_t *scaled_font;
  cairo_t *cr;
  hb_map_t *color_cache;
  hb_cairo_budget_t *budget;
};

अटल अंतरभूत cairo_operator_t
_hb_paint_composite_mode_to_cairo (hb_paint_composite_mode_t mode)
{
  चयन (mode)
    {
    हाल HB_PAINT_COMPOSITE_MODE_CLEAR: वापस CAIRO_OPERATOR_CLEAR;
    हाल HB_PAINT_COMPOSITE_MODE_SRC: वापस CAIRO_OPERATOR_SOURCE;
    हाल HB_PAINT_COMPOSITE_MODE_DEST: वापस CAIRO_OPERATOR_DEST;
    हाल HB_PAINT_COMPOSITE_MODE_SRC_OVER: वापस CAIRO_OPERATOR_OVER;
    हाल HB_PAINT_COMPOSITE_MODE_DEST_OVER: वापस CAIRO_OPERATOR_DEST_OVER;
    हाल HB_PAINT_COMPOSITE_MODE_SRC_IN: वापस CAIRO_OPERATOR_IN;
    हाल HB_PAINT_COMPOSITE_MODE_DEST_IN: वापस CAIRO_OPERATOR_DEST_IN;
    हाल HB_PAINT_COMPOSITE_MODE_SRC_OUT: वापस CAIRO_OPERATOR_OUT;
    हाल HB_PAINT_COMPOSITE_MODE_DEST_OUT: वापस CAIRO_OPERATOR_DEST_OUT;
    हाल HB_PAINT_COMPOSITE_MODE_SRC_ATOP: वापस CAIRO_OPERATOR_ATOP;
    हाल HB_PAINT_COMPOSITE_MODE_DEST_ATOP: वापस CAIRO_OPERATOR_DEST_ATOP;
    हाल HB_PAINT_COMPOSITE_MODE_XOR: वापस CAIRO_OPERATOR_XOR;
    हाल HB_PAINT_COMPOSITE_MODE_PLUS: वापस CAIRO_OPERATOR_ADD;
    हाल HB_PAINT_COMPOSITE_MODE_SCREEN: वापस CAIRO_OPERATOR_SCREEN;
    हाल HB_PAINT_COMPOSITE_MODE_OVERLAY: वापस CAIRO_OPERATOR_OVERLAY;
    हाल HB_PAINT_COMPOSITE_MODE_DARKEN: वापस CAIRO_OPERATOR_DARKEN;
    हाल HB_PAINT_COMPOSITE_MODE_LIGHTEN: वापस CAIRO_OPERATOR_LIGHTEN;
    हाल HB_PAINT_COMPOSITE_MODE_COLOR_DODGE: वापस CAIRO_OPERATOR_COLOR_DODGE;
    हाल HB_PAINT_COMPOSITE_MODE_COLOR_BURN: वापस CAIRO_OPERATOR_COLOR_BURN;
    हाल HB_PAINT_COMPOSITE_MODE_HARD_LIGHT: वापस CAIRO_OPERATOR_HARD_LIGHT;
    हाल HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT: वापस CAIRO_OPERATOR_SOFT_LIGHT;
    हाल HB_PAINT_COMPOSITE_MODE_DIFFERENCE: वापस CAIRO_OPERATOR_DIFFERENCE;
    हाल HB_PAINT_COMPOSITE_MODE_EXCLUSION: वापस CAIRO_OPERATOR_EXCLUSION;
    हाल HB_PAINT_COMPOSITE_MODE_MULTIPLY: वापस CAIRO_OPERATOR_MULTIPLY;
    हाल HB_PAINT_COMPOSITE_MODE_HSL_HUE: वापस CAIRO_OPERATOR_HSL_HUE;
    हाल HB_PAINT_COMPOSITE_MODE_HSL_SATURATION: वापस CAIRO_OPERATOR_HSL_SATURATION;
    हाल HB_PAINT_COMPOSITE_MODE_HSL_COLOR: वापस CAIRO_OPERATOR_HSL_COLOR;
    हाल HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY: वापस CAIRO_OPERATOR_HSL_LUMINOSITY;
    शेष: वापस CAIRO_OPERATOR_CLEAR;
    }
}

HB_INTERNAL व्योम
_hb_cairo_set_source_color (hb_cairo_context_t *c,
			    hb_bool_t use_foreground,
			    hb_color_t color);

HB_INTERNAL hb_bool_t
_hb_cairo_paint_glyph_image (hb_cairo_context_t *c,
			     hb_blob_t *blob,
			     अचिन्हित width,
			     अचिन्हित height,
			     hb_tag_t format,
			     भग्न slant,
			     hb_glyph_extents_t *extents);

HB_INTERNAL व्योम
_hb_cairo_paint_linear_gradient (hb_cairo_context_t *c,
				 hb_color_पंक्ति_t *color_पंक्ति,
				 भग्न x0, भग्न y0,
				 भग्न x1, भग्न y1,
				 भग्न x2, भग्न y2);

HB_INTERNAL व्योम
_hb_cairo_paint_radial_gradient (hb_cairo_context_t *c,
				 hb_color_पंक्ति_t *color_पंक्ति,
				 भग्न x0, भग्न y0, भग्न r0,
				 भग्न x1, भग्न y1, भग्न r1);

HB_INTERNAL व्योम
_hb_cairo_paint_sweep_gradient (hb_cairo_context_t *c,
				hb_color_पंक्ति_t *color_पंक्ति,
				भग्न x0, भग्न y0,
				भग्न start_angle, भग्न end_angle);


#पूर्ण_अगर /* HB_CAIRO_UTILS_H */

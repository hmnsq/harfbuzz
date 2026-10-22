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

#اگر_غیر_تعریف_شدہ HB_CAIRO_UTILS_H
#تعریف_کرو HB_CAIRO_UTILS_H

#شامل "hb.hh"
#شامل "hb-cairo.h"


ساخت hb_cairo_budget_t
{
  /* Cairo serializes glyph-cache population for a scaled font.  Keeping the
   * owner there lets recursive color-glyph callbacks share one session. */
  int64_t budget = HB_BUDGET_DEFAULT;
  int64_t budget_بقایا = HB_BUDGET_GLYPH;
  غیرعلامتی صحیح فعال = 0;

  خالی enter ()
  {
    اگر (!فعال++)
      recharge ();
  }

  خالی leave ()
  {
    أكد (فعال);
    فعال--;
  }

  خالی recharge ()
  {
    budget_بقایا = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }
};

ساخت hb_cairo_context_t
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

  منطقی سیٹ_budget (int64_t value)
  {
    budget->budget = value;
    budget->recharge ();
    واپس سچ;
  }

  int64_t get_budget () ثابت { واپس budget->budget; }
  int64_t *get_budget_بقایا () { واپس &budget->budget_بقایا; }

  منطقی spend (غیرعلامتی صحیح لاگت, غیرعلامتی صحیح mult = 1)
  { واپس hb_budget_spend (budget->budget_بقایا, لاگت, mult); }

  cairo_scaled_font_t *scaled_font;
  cairo_t *cr;
  hb_map_t *color_cache;
  hb_cairo_budget_t *budget;
};

ساکن اندرونی cairo_operator_t
hb_paint_مرکب_منوال_مقصد_cairo (hb_paint_مرکب_منوال_t منوال)
{
  انتخاب (منوال)
    {
    حالت HB_PAINT_مرکب_منوال_CLEAR: واپس CAIRO_OPERATOR_CLEAR;
    حالت HB_PAINT_مرکب_منوال_SRC: واپس CAIRO_OPERATOR_ماخذ;
    حالت HB_PAINT_مرکب_منوال_DEST: واپس CAIRO_OPERATOR_DEST;
    حالت HB_PAINT_مرکب_منوال_SRC_OVER: واپس CAIRO_OPERATOR_OVER;
    حالت HB_PAINT_مرکب_منوال_DEST_OVER: واپس CAIRO_OPERATOR_DEST_OVER;
    حالت HB_PAINT_مرکب_منوال_SRC_اندر: واپس CAIRO_OPERATOR_اندر;
    حالت HB_PAINT_مرکب_منوال_DEST_اندر: واپس CAIRO_OPERATOR_DEST_اندر;
    حالت HB_PAINT_مرکب_منوال_SRC_برآمد: واپس CAIRO_OPERATOR_برآمد;
    حالت HB_PAINT_مرکب_منوال_DEST_برآمد: واپس CAIRO_OPERATOR_DEST_برآمد;
    حالت HB_PAINT_مرکب_منوال_SRC_ATOP: واپس CAIRO_OPERATOR_ATOP;
    حالت HB_PAINT_مرکب_منوال_DEST_ATOP: واپس CAIRO_OPERATOR_DEST_ATOP;
    حالت HB_PAINT_مرکب_منوال_XOR: واپس CAIRO_OPERATOR_XOR;
    حالت HB_PAINT_مرکب_منوال_PLUS: واپس CAIRO_OPERATOR_جوڑو;
    حالت HB_PAINT_مرکب_منوال_SCREEN: واپس CAIRO_OPERATOR_SCREEN;
    حالت HB_PAINT_مرکب_منوال_OVERLAY: واپس CAIRO_OPERATOR_OVERLAY;
    حالت HB_PAINT_مرکب_منوال_DARKEN: واپس CAIRO_OPERATOR_DARKEN;
    حالت HB_PAINT_مرکب_منوال_LIGHTEN: واپس CAIRO_OPERATOR_LIGHTEN;
    حالت HB_PAINT_مرکب_منوال_COLOR_DODGE: واپس CAIRO_OPERATOR_COLOR_DODGE;
    حالت HB_PAINT_مرکب_منوال_COLOR_BURN: واپس CAIRO_OPERATOR_COLOR_BURN;
    حالت HB_PAINT_مرکب_منوال_HARD_LIGHT: واپس CAIRO_OPERATOR_HARD_LIGHT;
    حالت HB_PAINT_مرکب_منوال_SOFT_LIGHT: واپس CAIRO_OPERATOR_SOFT_LIGHT;
    حالت HB_PAINT_مرکب_منوال_DIFFERENCE: واپس CAIRO_OPERATOR_DIFFERENCE;
    حالت HB_PAINT_مرکب_منوال_EXCLUSION: واپس CAIRO_OPERATOR_EXCLUSION;
    حالت HB_PAINT_مرکب_منوال_MULTIPLY: واپس CAIRO_OPERATOR_MULTIPLY;
    حالت HB_PAINT_مرکب_منوال_HSL_HUE: واپس CAIRO_OPERATOR_HSL_HUE;
    حالت HB_PAINT_مرکب_منوال_HSL_SATURATION: واپس CAIRO_OPERATOR_HSL_SATURATION;
    حالت HB_PAINT_مرکب_منوال_HSL_COLOR: واپس CAIRO_OPERATOR_HSL_COLOR;
    حالت HB_PAINT_مرکب_منوال_HSL_LUMINOSITY: واپس CAIRO_OPERATOR_HSL_LUMINOSITY;
    طےشدہ: واپس CAIRO_OPERATOR_CLEAR;
    }
}

HB_اندرونی خالی
hb_cairo_سیٹ_ماخذ_color (hb_cairo_context_t *c,
			    hb_bool_t use_foreground,
			    hb_color_t color);

HB_اندرونی hb_bool_t
_hb_cairo_paint_glyph_image (hb_cairo_context_t *c,
			     hb_blob_t *blob,
			     غیرعلامتی چوڑائی,
			     غیرعلامتی اونچائی,
			     hb_لیبل_t format,
			     اعشاری slant,
			     hb_glyph_extents_t *extents);

HB_اندرونی خالی
hb_cairo_paint_خطی_gradient (hb_cairo_context_t *c,
				 hb_color_سطر_t *color_سطر,
				 اعشاری x0, اعشاری y0,
				 اعشاری x1, اعشاری y1,
				 اعشاری x2, اعشاری y2);

HB_اندرونی خالی
_hb_cairo_paint_radial_gradient (hb_cairo_context_t *c,
				 hb_color_سطر_t *color_سطر,
				 اعشاری x0, اعشاری y0, اعشاری r0,
				 اعشاری x1, اعشاری y1, اعشاری r1);

HB_اندرونی خالی
_hb_cairo_paint_sweep_gradient (hb_cairo_context_t *c,
				hb_color_سطر_t *color_سطر,
				اعشاری x0, اعشاری y0,
				اعشاری آغاز_angle, اعشاری انتہا_angle);


#اگر_ختم /* HB_CAIRO_UTILS_H */

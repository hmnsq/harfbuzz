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

#اگر_تعریف_نشده HB_CAIRO_UTILS_H
#تعریف_کن HB_CAIRO_UTILS_H

#ضمن "hb.hh"
#ضمن "hb-cairo.h"


بنية hb_cairo_budget_t
{
  /* Cairo serializes glyph-cache population for a scaled font.  Keeping the
   * owner there lets recursive color-glyph callbacks share one session. */
  int64_t budget = HB_BUDGET_DEFAULT;
  int64_t budget_مانده = HB_BUDGET_GLYPH;
  بیعلامت صحيح فعال = 0;

  فراغ enter ()
  {
    إذا (!فعال++)
      recharge ();
  }

  فراغ leave ()
  {
    أكد (فعال);
    فعال--;
  }

  فراغ recharge ()
  {
    budget_مانده = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }
};

بنية hb_cairo_context_t
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

  منطقي مجموعه_budget (int64_t value)
  {
    budget->budget = value;
    budget->recharge ();
    أرجع درست;
  }

  int64_t get_budget () ثابت { أرجع budget->budget; }
  int64_t *get_budget_مانده () { أرجع &budget->budget_مانده; }

  منطقي spend (بیعلامت صحيح هزینه, بیعلامت صحيح mult = 1)
  { أرجع hb_budget_spend (budget->budget_مانده, هزینه, mult); }

  cairo_scaled_font_t *scaled_font;
  cairo_t *cr;
  hb_map_t *color_cache;
  hb_cairo_budget_t *budget;
};

ساكن مضمن cairo_operator_t
hb_paint_مرکب_نما_مقصد_cairo (hb_paint_مرکب_نما_t نما)
{
  بدل (نما)
    {
    حالة HB_PAINT_مرکب_نما_CLEAR: أرجع CAIRO_OPERATOR_CLEAR;
    حالة HB_PAINT_مرکب_نما_SRC: أرجع CAIRO_OPERATOR_منبع;
    حالة HB_PAINT_مرکب_نما_DEST: أرجع CAIRO_OPERATOR_DEST;
    حالة HB_PAINT_مرکب_نما_SRC_OVER: أرجع CAIRO_OPERATOR_OVER;
    حالة HB_PAINT_مرکب_نما_DEST_OVER: أرجع CAIRO_OPERATOR_DEST_OVER;
    حالة HB_PAINT_مرکب_نما_SRC_درون: أرجع CAIRO_OPERATOR_درون;
    حالة HB_PAINT_مرکب_نما_DEST_درون: أرجع CAIRO_OPERATOR_DEST_درون;
    حالة HB_PAINT_مرکب_نما_SRC_بیرون: أرجع CAIRO_OPERATOR_بیرون;
    حالة HB_PAINT_مرکب_نما_DEST_بیرون: أرجع CAIRO_OPERATOR_DEST_بیرون;
    حالة HB_PAINT_مرکب_نما_SRC_ATOP: أرجع CAIRO_OPERATOR_ATOP;
    حالة HB_PAINT_مرکب_نما_DEST_ATOP: أرجع CAIRO_OPERATOR_DEST_ATOP;
    حالة HB_PAINT_مرکب_نما_XOR: أرجع CAIRO_OPERATOR_XOR;
    حالة HB_PAINT_مرکب_نما_PLUS: أرجع CAIRO_OPERATOR_جمع_کن;
    حالة HB_PAINT_مرکب_نما_SCREEN: أرجع CAIRO_OPERATOR_SCREEN;
    حالة HB_PAINT_مرکب_نما_OVERLAY: أرجع CAIRO_OPERATOR_OVERLAY;
    حالة HB_PAINT_مرکب_نما_DARKEN: أرجع CAIRO_OPERATOR_DARKEN;
    حالة HB_PAINT_مرکب_نما_LIGHTEN: أرجع CAIRO_OPERATOR_LIGHTEN;
    حالة HB_PAINT_مرکب_نما_COLOR_DODGE: أرجع CAIRO_OPERATOR_COLOR_DODGE;
    حالة HB_PAINT_مرکب_نما_COLOR_BURN: أرجع CAIRO_OPERATOR_COLOR_BURN;
    حالة HB_PAINT_مرکب_نما_HARD_LIGHT: أرجع CAIRO_OPERATOR_HARD_LIGHT;
    حالة HB_PAINT_مرکب_نما_SOFT_LIGHT: أرجع CAIRO_OPERATOR_SOFT_LIGHT;
    حالة HB_PAINT_مرکب_نما_DIFFERENCE: أرجع CAIRO_OPERATOR_DIFFERENCE;
    حالة HB_PAINT_مرکب_نما_EXCLUSION: أرجع CAIRO_OPERATOR_EXCLUSION;
    حالة HB_PAINT_مرکب_نما_MULTIPLY: أرجع CAIRO_OPERATOR_MULTIPLY;
    حالة HB_PAINT_مرکب_نما_HSL_HUE: أرجع CAIRO_OPERATOR_HSL_HUE;
    حالة HB_PAINT_مرکب_نما_HSL_SATURATION: أرجع CAIRO_OPERATOR_HSL_SATURATION;
    حالة HB_PAINT_مرکب_نما_HSL_COLOR: أرجع CAIRO_OPERATOR_HSL_COLOR;
    حالة HB_PAINT_مرکب_نما_HSL_LUMINOSITY: أرجع CAIRO_OPERATOR_HSL_LUMINOSITY;
    افتراضي: أرجع CAIRO_OPERATOR_CLEAR;
    }
}

HB_داخلی فراغ
hb_cairo_مجموعه_منبع_color (hb_cairo_context_t *c,
			    hb_bool_t use_foreground,
			    hb_color_t color);

HB_داخلی hb_bool_t
_hb_cairo_paint_glyph_image (hb_cairo_context_t *c,
			     hb_blob_t *blob,
			     بیعلامت عرض,
			     بیعلامت ارتفاع,
			     hb_برچسب_t format,
			     عائم slant,
			     hb_glyph_extents_t *extents);

HB_داخلی فراغ
hb_cairo_paint_خطی_gradient (hb_cairo_context_t *c,
				 hb_color_سطر_t *color_سطر,
				 عائم x0, عائم y0,
				 عائم x1, عائم y1,
				 عائم x2, عائم y2);

HB_داخلی فراغ
_hb_cairo_paint_radial_gradient (hb_cairo_context_t *c,
				 hb_color_سطر_t *color_سطر,
				 عائم x0, عائم y0, عائم r0,
				 عائم x1, عائم y1, عائم r1);

HB_داخلی فراغ
_hb_cairo_paint_sweep_gradient (hb_cairo_context_t *c,
				hb_color_سطر_t *color_سطر,
				عائم x0, عائم y0,
				عائم شروع_angle, عائم انتها_angle);


#پایان_اگر /* HB_CAIRO_UTILS_H */

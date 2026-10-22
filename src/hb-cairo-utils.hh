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

#که_نه_تعریف_شوی HB_CAIRO_UTILS_H
#تعریف_کړه HB_CAIRO_UTILS_H

#ضمن "hb.hh"
#ضمن "hb-cairo.h"


بنية hb_cairo_budget_t
{
  /* Cairo serializes glyph-cache population for a scaled font.  Keeping the
   * owner there lets recursive color-glyph callbacks share one session. */
  int64_t budget = HB_BUDGET_DEFAULT;
  int64_t budget_پاتې_شوی = HB_BUDGET_GLYPH;
  بینښه صحيح فعال = 0;

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
    budget_پاتې_شوی = budget == HB_BUDGET_DEFAULT ?
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

  منطقي سېټ_budget (int64_t value)
  {
    budget->budget = value;
    budget->recharge ();
    أرجع ریښتیا;
  }

  int64_t get_budget () ثابت { أرجع budget->budget; }
  int64_t *get_budget_پاتې_شوی () { أرجع &budget->budget_پاتې_شوی; }

  منطقي spend (بینښه صحيح لګښت, بینښه صحيح mult = 1)
  { أرجع hb_budget_spend (budget->budget_پاتې_شوی, لګښت, mult); }

  cairo_scaled_font_t *scaled_font;
  cairo_t *cr;
  hb_map_t *color_cache;
  hb_cairo_budget_t *budget;
};

ساكن مضمن cairo_operator_t
hb_paint_مرکب_ډېرپېښ_منزل_cairo (hb_paint_مرکب_ډېرپېښ_t ډېرپېښ)
{
  بدل (ډېرپېښ)
    {
    حالة HB_PAINT_مرکب_ډېرپېښ_CLEAR: أرجع CAIRO_OPERATOR_CLEAR;
    حالة HB_PAINT_مرکب_ډېرپېښ_SRC: أرجع CAIRO_OPERATOR_سرچینه;
    حالة HB_PAINT_مرکب_ډېرپېښ_DEST: أرجع CAIRO_OPERATOR_DEST;
    حالة HB_PAINT_مرکب_ډېرپېښ_SRC_OVER: أرجع CAIRO_OPERATOR_OVER;
    حالة HB_PAINT_مرکب_ډېرپېښ_DEST_OVER: أرجع CAIRO_OPERATOR_DEST_OVER;
    حالة HB_PAINT_مرکب_ډېرپېښ_SRC_دننه: أرجع CAIRO_OPERATOR_دننه;
    حالة HB_PAINT_مرکب_ډېرپېښ_DEST_دننه: أرجع CAIRO_OPERATOR_DEST_دننه;
    حالة HB_PAINT_مرکب_ډېرپېښ_SRC_بهر: أرجع CAIRO_OPERATOR_بهر;
    حالة HB_PAINT_مرکب_ډېرپېښ_DEST_بهر: أرجع CAIRO_OPERATOR_DEST_بهر;
    حالة HB_PAINT_مرکب_ډېرپېښ_SRC_ATOP: أرجع CAIRO_OPERATOR_ATOP;
    حالة HB_PAINT_مرکب_ډېرپېښ_DEST_ATOP: أرجع CAIRO_OPERATOR_DEST_ATOP;
    حالة HB_PAINT_مرکب_ډېرپېښ_XOR: أرجع CAIRO_OPERATOR_XOR;
    حالة HB_PAINT_مرکب_ډېرپېښ_PLUS: أرجع CAIRO_OPERATOR_جمع_کړه;
    حالة HB_PAINT_مرکب_ډېرپېښ_SCREEN: أرجع CAIRO_OPERATOR_SCREEN;
    حالة HB_PAINT_مرکب_ډېرپېښ_OVERLAY: أرجع CAIRO_OPERATOR_OVERLAY;
    حالة HB_PAINT_مرکب_ډېرپېښ_DARKEN: أرجع CAIRO_OPERATOR_DARKEN;
    حالة HB_PAINT_مرکب_ډېرپېښ_LIGHTEN: أرجع CAIRO_OPERATOR_LIGHTEN;
    حالة HB_PAINT_مرکب_ډېرپېښ_COLOR_DODGE: أرجع CAIRO_OPERATOR_COLOR_DODGE;
    حالة HB_PAINT_مرکب_ډېرپېښ_COLOR_BURN: أرجع CAIRO_OPERATOR_COLOR_BURN;
    حالة HB_PAINT_مرکب_ډېرپېښ_HARD_LIGHT: أرجع CAIRO_OPERATOR_HARD_LIGHT;
    حالة HB_PAINT_مرکب_ډېرپېښ_SOFT_LIGHT: أرجع CAIRO_OPERATOR_SOFT_LIGHT;
    حالة HB_PAINT_مرکب_ډېرپېښ_DIFFERENCE: أرجع CAIRO_OPERATOR_DIFFERENCE;
    حالة HB_PAINT_مرکب_ډېرپېښ_EXCLUSION: أرجع CAIRO_OPERATOR_EXCLUSION;
    حالة HB_PAINT_مرکب_ډېرپېښ_MULTIPLY: أرجع CAIRO_OPERATOR_MULTIPLY;
    حالة HB_PAINT_مرکب_ډېرپېښ_HSL_HUE: أرجع CAIRO_OPERATOR_HSL_HUE;
    حالة HB_PAINT_مرکب_ډېرپېښ_HSL_SATURATION: أرجع CAIRO_OPERATOR_HSL_SATURATION;
    حالة HB_PAINT_مرکب_ډېرپېښ_HSL_COLOR: أرجع CAIRO_OPERATOR_HSL_COLOR;
    حالة HB_PAINT_مرکب_ډېرپېښ_HSL_LUMINOSITY: أرجع CAIRO_OPERATOR_HSL_LUMINOSITY;
    افتراضي: أرجع CAIRO_OPERATOR_CLEAR;
    }
}

HB_دننی فراغ
hb_cairo_سېټ_سرچینه_color (hb_cairo_context_t *c,
			    hb_bool_t use_foreground,
			    hb_color_t color);

HB_دننی hb_bool_t
_hb_cairo_paint_glyph_image (hb_cairo_context_t *c,
			     hb_blob_t *blob,
			     بینښه پلنوالی,
			     بینښه لوړوالی,
			     hb_نښان_t format,
			     عائم slant,
			     hb_glyph_extents_t *extents);

HB_دننی فراغ
hb_cairo_paint_خطي_gradient (hb_cairo_context_t *c,
				 hb_color_کرښه_t *color_کرښه,
				 عائم x0, عائم y0,
				 عائم x1, عائم y1,
				 عائم x2, عائم y2);

HB_دننی فراغ
_hb_cairo_paint_radial_gradient (hb_cairo_context_t *c,
				 hb_color_کرښه_t *color_کرښه,
				 عائم x0, عائم y0, عائم r0,
				 عائم x1, عائم y1, عائم r1);

HB_دننی فراغ
_hb_cairo_paint_sweep_gradient (hb_cairo_context_t *c,
				hb_color_کرښه_t *color_کرښه,
				عائم x0, عائم y0,
				عائم پیل_angle, عائم پای_ټکی_angle);


#د_که_پای /* HB_CAIRO_UTILS_H */

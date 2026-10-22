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

#جيڪڏهن_وصف_نه_ٿيل HB_CAIRO_UTILS_H
#وصف_ڪريو HB_CAIRO_UTILS_H

#ضمن "hb.hh"
#ضمن "hb-cairo.h"


بنية hb_cairo_budget_t
{
  /* Cairo serializes glyph-cache population for a scaled font.  Keeping the
   * owner there lets recursive color-glyph callbacks share one session. */
  int64_t budget = HB_BUDGET_DEFAULT;
  int64_t budget_بچيل = HB_BUDGET_GLYPH;
  بنانشان صحيح سرگرم = 0;

  فراغ enter ()
  {
    إذا (!سرگرم++)
      recharge ();
  }

  فراغ leave ()
  {
    أكد (سرگرم);
    سرگرم--;
  }

  فراغ recharge ()
  {
    budget_بچيل = budget == HB_BUDGET_DEFAULT ?
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

  منطقي سيٽ_budget (int64_t value)
  {
    budget->budget = value;
    budget->recharge ();
    أرجع سچ;
  }

  int64_t get_budget () ثابت { أرجع budget->budget; }
  int64_t *get_budget_بچيل () { أرجع &budget->budget_بچيل; }

  منطقي spend (بنانشان صحيح لاڳت, بنانشان صحيح mult = 1)
  { أرجع hb_budget_spend (budget->budget_بچيل, لاڳت, mult); }

  cairo_scaled_font_t *scaled_font;
  cairo_t *cr;
  hb_map_t *color_cache;
  hb_cairo_budget_t *budget;
};

ساكن مضمن cairo_operator_t
hb_paint_مرکب_گهڻائي_منزل_cairo (hb_paint_مرکب_گهڻائي_t گهڻائي)
{
  بدل (گهڻائي)
    {
    حالة HB_PAINT_مرکب_گهڻائي_CLEAR: أرجع CAIRO_OPERATOR_CLEAR;
    حالة HB_PAINT_مرکب_گهڻائي_SRC: أرجع CAIRO_OPERATOR_ذريعو;
    حالة HB_PAINT_مرکب_گهڻائي_DEST: أرجع CAIRO_OPERATOR_DEST;
    حالة HB_PAINT_مرکب_گهڻائي_SRC_OVER: أرجع CAIRO_OPERATOR_OVER;
    حالة HB_PAINT_مرکب_گهڻائي_DEST_OVER: أرجع CAIRO_OPERATOR_DEST_OVER;
    حالة HB_PAINT_مرکب_گهڻائي_SRC_اندريون: أرجع CAIRO_OPERATOR_اندريون;
    حالة HB_PAINT_مرکب_گهڻائي_DEST_اندريون: أرجع CAIRO_OPERATOR_DEST_اندريون;
    حالة HB_PAINT_مرکب_گهڻائي_SRC_ٻاهريون: أرجع CAIRO_OPERATOR_ٻاهريون;
    حالة HB_PAINT_مرکب_گهڻائي_DEST_ٻاهريون: أرجع CAIRO_OPERATOR_DEST_ٻاهريون;
    حالة HB_PAINT_مرکب_گهڻائي_SRC_ATOP: أرجع CAIRO_OPERATOR_ATOP;
    حالة HB_PAINT_مرکب_گهڻائي_DEST_ATOP: أرجع CAIRO_OPERATOR_DEST_ATOP;
    حالة HB_PAINT_مرکب_گهڻائي_XOR: أرجع CAIRO_OPERATOR_XOR;
    حالة HB_PAINT_مرکب_گهڻائي_PLUS: أرجع CAIRO_OPERATOR_جوڙيو;
    حالة HB_PAINT_مرکب_گهڻائي_SCREEN: أرجع CAIRO_OPERATOR_SCREEN;
    حالة HB_PAINT_مرکب_گهڻائي_OVERLAY: أرجع CAIRO_OPERATOR_OVERLAY;
    حالة HB_PAINT_مرکب_گهڻائي_DARKEN: أرجع CAIRO_OPERATOR_DARKEN;
    حالة HB_PAINT_مرکب_گهڻائي_LIGHTEN: أرجع CAIRO_OPERATOR_LIGHTEN;
    حالة HB_PAINT_مرکب_گهڻائي_COLOR_DODGE: أرجع CAIRO_OPERATOR_COLOR_DODGE;
    حالة HB_PAINT_مرکب_گهڻائي_COLOR_BURN: أرجع CAIRO_OPERATOR_COLOR_BURN;
    حالة HB_PAINT_مرکب_گهڻائي_HARD_LIGHT: أرجع CAIRO_OPERATOR_HARD_LIGHT;
    حالة HB_PAINT_مرکب_گهڻائي_SOFT_LIGHT: أرجع CAIRO_OPERATOR_SOFT_LIGHT;
    حالة HB_PAINT_مرکب_گهڻائي_DIFFERENCE: أرجع CAIRO_OPERATOR_DIFFERENCE;
    حالة HB_PAINT_مرکب_گهڻائي_EXCLUSION: أرجع CAIRO_OPERATOR_EXCLUSION;
    حالة HB_PAINT_مرکب_گهڻائي_MULTIPLY: أرجع CAIRO_OPERATOR_MULTIPLY;
    حالة HB_PAINT_مرکب_گهڻائي_HSL_HUE: أرجع CAIRO_OPERATOR_HSL_HUE;
    حالة HB_PAINT_مرکب_گهڻائي_HSL_SATURATION: أرجع CAIRO_OPERATOR_HSL_SATURATION;
    حالة HB_PAINT_مرکب_گهڻائي_HSL_COLOR: أرجع CAIRO_OPERATOR_HSL_COLOR;
    حالة HB_PAINT_مرکب_گهڻائي_HSL_LUMINOSITY: أرجع CAIRO_OPERATOR_HSL_LUMINOSITY;
    افتراضي: أرجع CAIRO_OPERATOR_CLEAR;
    }
}

HB_اندروني فراغ
hb_cairo_سيٽ_ذريعو_color (hb_cairo_context_t *c,
			    hb_bool_t use_foreground,
			    hb_color_t color);

HB_اندروني hb_bool_t
_hb_cairo_paint_glyph_image (hb_cairo_context_t *c,
			     hb_blob_t *blob,
			     بنانشان ويڪر,
			     بنانشان اوچائي,
			     hb_ٽيگ_t format,
			     عائم slant,
			     hb_glyph_extents_t *extents);

HB_اندروني فراغ
hb_cairo_paint_لڪيري_gradient (hb_cairo_context_t *c,
				 hb_color_سٽ_t *color_سٽ,
				 عائم x0, عائم y0,
				 عائم x1, عائم y1,
				 عائم x2, عائم y2);

HB_اندروني فراغ
_hb_cairo_paint_radial_gradient (hb_cairo_context_t *c,
				 hb_color_سٽ_t *color_سٽ,
				 عائم x0, عائم y0, عائم r0,
				 عائم x1, عائم y1, عائم r1);

HB_اندروني فراغ
_hb_cairo_paint_sweep_gradient (hb_cairo_context_t *c,
				hb_color_سٽ_t *color_سٽ,
				عائم x0, عائم y0,
				عائم شروعات_angle, عائم پڇاڙي_angle);


#جيڪڏهن_پورو /* HB_CAIRO_UTILS_H */

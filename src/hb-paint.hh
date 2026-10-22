/*
 * Copyright © 2022 Matthias Clasen
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
 */

#جے_بنا_تعریف HB_PAINT_HH
#تعریف_کرو HB_PAINT_HH

#ضمن "hb.hh"
#ضمن "hb-face.hh"
#ضمن "hb-font.hh"
#ضمن "hb-geometry.hh"

#تعریف_کرو HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS \
  HB_PAINT_FUNC_IMPLEMENT (ڈالو_transform) \
  HB_PAINT_FUNC_IMPLEMENT (نکالو_transform) \
  HB_PAINT_FUNC_IMPLEMENT (color_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (ڈالو_clip_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (ڈالو_clip_rectangle) \
  HB_PAINT_FUNC_IMPLEMENT (ڈالو_clip_رستہ_شروع) \
  HB_PAINT_FUNC_IMPLEMENT (ڈالو_clip_رستہ_سرا) \
  HB_PAINT_FUNC_IMPLEMENT (نکالو_clip) \
  HB_PAINT_FUNC_IMPLEMENT (color) \
  HB_PAINT_FUNC_IMPLEMENT (image) \
  HB_PAINT_FUNC_IMPLEMENT (خطی_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (radial_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (sweep_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (ڈالو_گروہ) \
  HB_PAINT_FUNC_IMPLEMENT (ڈالو_گروہ_for) \
  HB_PAINT_FUNC_IMPLEMENT (نکالو_گروہ) \
  HB_PAINT_FUNC_IMPLEMENT (custom_palette_color) \
  HB_PAINT_FUNC_IMPLEMENT (fill_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (سیٹ_budget) \
  HB_PAINT_FUNC_IMPLEMENT (get_budget) \
  HB_PAINT_FUNC_IMPLEMENT (get_budget_بچیا) \
  /* ^--- Add new callbacks here */

بنية hb_paint_funcs_t
{
  hb_object_سرخی_t سرخی;

  بنية {
#تعریف_کرو HB_PAINT_FUNC_IMPLEMENT(name) hb_paint_##name##_func_t name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#تعریف_رد HB_PAINT_FUNC_IMPLEMENT
  } func;

  بنية {
#تعریف_کرو HB_PAINT_FUNC_IMPLEMENT(name) فراغ *name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#تعریف_رد HB_PAINT_FUNC_IMPLEMENT
  } *user_انکڑے;

  بنية {
#تعریف_کرو HB_PAINT_FUNC_IMPLEMENT(name) hb_destroy_func_t name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#تعریف_رد HB_PAINT_FUNC_IMPLEMENT
  } *destroy;

  فراغ ڈالو_transform (فراغ *paint_انکڑے,
                       عائم xx, عائم yx,
                       عائم xy, عائم yy,
                       عائم dx, عائم dy)
  {
    // Handle -0.f to avoid -0.f == 0.f in the transform matrix.
    إذا (dx == -0.f) dx = 0.f;
    إذا (dy == -0.f) dy = 0.f;
    func.ڈالو_transform (هذا, paint_انکڑے,
                         xx, yx, xy, yy, dx, dy,
                         !user_انکڑے ? خالی_سوچک : user_انکڑے->ڈالو_transform); }
  فراغ نکالو_transform (فراغ *paint_انکڑے)
  { func.نکالو_transform (هذا, paint_انکڑے,
                        !user_انکڑے ? خالی_سوچک : user_انکڑے->نکالو_transform); }
  منطقي color_glyph (فراغ *paint_انکڑے,
                    hb_codepoint_t glyph,
                    hb_font_t *font)
  { أرجع func.color_glyph (هذا, paint_انکڑے,
                             glyph,
                             font,
                             !user_انکڑے ? خالی_سوچک : user_انکڑے->color_glyph); }
  فراغ ڈالو_clip_glyph (فراغ *paint_انکڑے,
                        hb_codepoint_t glyph,
                        hb_font_t *font)
  { func.ڈالو_clip_glyph (هذا, paint_انکڑے,
                          glyph,
                          font,
                          !user_انکڑے ? خالی_سوچک : user_انکڑے->ڈالو_clip_glyph); }
  فراغ ڈالو_clip_rectangle (فراغ *paint_انکڑے,
                           عائم xmin, عائم ymin, عائم xmax, عائم ymax)
  { func.ڈالو_clip_rectangle (هذا, paint_انکڑے,
                              xmin, ymin, xmax, ymax,
                              !user_انکڑے ? خالی_سوچک : user_انکڑے->ڈالو_clip_rectangle); }
  hb_draw_funcs_t *ڈالو_clip_رستہ_شروع (فراغ *paint_انکڑے,
                                         فراغ **draw_انکڑے)
  { أرجع func.ڈالو_clip_رستہ_شروع (هذا, paint_انکڑے, draw_انکڑے,
                                      !user_انکڑے ? خالی_سوچک : user_انکڑے->ڈالو_clip_رستہ_شروع); }
  فراغ ڈالو_clip_رستہ_سرا (فراغ *paint_انکڑے)
  { func.ڈالو_clip_رستہ_سرا (هذا, paint_انکڑے,
                             !user_انکڑے ? خالی_سوچک : user_انکڑے->ڈالو_clip_رستہ_سرا); }
  فراغ نکالو_clip (فراغ *paint_انکڑے)
  { func.نکالو_clip (هذا, paint_انکڑے,
                   !user_انکڑے ? خالی_سوچک : user_انکڑے->نکالو_clip); }
  فراغ color (فراغ *paint_انکڑے,
              hb_bool_t is_foreground,
              hb_color_t color)
  { func.color (هذا, paint_انکڑے,
                is_foreground, color,
                !user_انکڑے ? خالی_سوچک : user_انکڑے->color); }
  فراغ fill_glyph (فراغ *paint_انکڑے,
                   hb_codepoint_t glyph,
                   hb_font_t *font,
                   hb_bool_t is_foreground,
                   hb_color_t color)
  { func.fill_glyph (هذا, paint_انکڑے,
                     glyph, font,
                     is_foreground, color,
                     !user_انکڑے ? خالی_سوچک : user_انکڑے->fill_glyph); }
  منطقي image (فراغ *paint_انکڑے,
              hb_blob_t *image,
              بنانشانی چوڑائی, بنانشانی اچائی,
              hb_ٹیگ_t format,
              عائم slant,
              hb_glyph_extents_t *extents)
  { أرجع func.image (هذا, paint_انکڑے,
                       image, چوڑائی, اچائی, format, slant, extents,
                       !user_انکڑے ? خالی_سوچک : user_انکڑے->image); }
  فراغ خطی_gradient (فراغ *paint_انکڑے,
                        hb_color_سطر_t *color_سطر,
                        عائم x0, عائم y0,
                        عائم x1, عائم y1,
                        عائم x2, عائم y2)
  { func.خطی_gradient (هذا, paint_انکڑے,
                          color_سطر, x0, y0, x1, y1, x2, y2,
                          !user_انکڑے ? خالی_سوچک : user_انکڑے->خطی_gradient); }
  فراغ radial_gradient (فراغ *paint_انکڑے,
                        hb_color_سطر_t *color_سطر,
                        عائم x0, عائم y0, عائم r0,
                        عائم x1, عائم y1, عائم r1)
  { func.radial_gradient (هذا, paint_انکڑے,
                          color_سطر, x0, y0, r0, x1, y1, r1,
                          !user_انکڑے ? خالی_سوچک : user_انکڑے->radial_gradient); }
  فراغ sweep_gradient (فراغ *paint_انکڑے,
                       hb_color_سطر_t *color_سطر,
                       عائم x0, عائم y0,
                       عائم شروع_angle,
                       عائم سرا_angle)
  { func.sweep_gradient (هذا, paint_انکڑے,
                         color_سطر, x0, y0, شروع_angle, سرا_angle,
                         !user_انکڑے ? خالی_سوچک : user_انکڑے->sweep_gradient); }
  فراغ ڈالو_گروہ (فراغ *paint_انکڑے)
  { func.ڈالو_گروہ (هذا, paint_انکڑے,
                     !user_انکڑے ? خالی_سوچک : user_انکڑے->ڈالو_گروہ); }
  فراغ ڈالو_گروہ_for (فراغ *paint_انکڑے,
                       hb_paint_مرکب_منوال_t منوال)
  { func.ڈالو_گروہ_for (هذا, paint_انکڑے,
                         منوال,
                         !user_انکڑے ? خالی_سوچک : user_انکڑے->ڈالو_گروہ_for); }
  فراغ نکالو_گروہ (فراغ *paint_انکڑے,
                  hb_paint_مرکب_منوال_t منوال)
  { func.نکالو_گروہ (هذا, paint_انکڑے,
                    منوال,
                    !user_انکڑے ? خالی_سوچک : user_انکڑے->نکالو_گروہ); }
  منطقي custom_palette_color (فراغ *paint_انکڑے,
                             بنانشانی صحيح color_اشاریہ,
                             hb_color_t *color)
  { أرجع func.custom_palette_color (هذا, paint_انکڑے,
                                      color_اشاریہ,
                                      color,
                                      !user_انکڑے ? خالی_سوچک : user_انکڑے->custom_palette_color); }
  منطقي سیٹ_budget (فراغ *paint_انکڑے, int64_t budget)
  {
    إذا (budget < 0 && budget != HB_BUDGET_DEFAULT)
      budget = 0;
    أرجع func.سیٹ_budget (هذا, paint_انکڑے, budget,
			    !user_انکڑے ? خالی_سوچک : user_انکڑے->سیٹ_budget);
  }
  int64_t get_budget (فراغ *paint_انکڑے)
  { أرجع func.get_budget (هذا, paint_انکڑے,
			    !user_انکڑے ? خالی_سوچک : user_انکڑے->get_budget); }
  int64_t *get_budget_بچیا_ptr (فراغ *paint_انکڑے)
  { أرجع func.get_budget_بچیا (هذا, paint_انکڑے,
				      !user_انکڑے ? خالی_سوچک : user_انکڑے->get_budget_بچیا); }


  /* Internal specializations. */

  فراغ ڈالو_font_transform (فراغ *paint_انکڑے,
                            ثابت hb_font_t *font)
  {
    عائم upem = font->face->get_upem ();
    صحيح xscale = font->x_scale, yscale = font->y_scale;

    ڈالو_transform (paint_انکڑے,
		    xscale/upem, 0,
		    0, yscale/upem,
		    0, 0);
  }

  فراغ ڈالو_inverse_font_transform (فراغ *paint_انکڑے,
                                    ثابت hb_font_t *font)
  {
    عائم upem = font->face->get_upem ();
    صحيح xscale = font->x_scale ? font->x_scale : upem;
    صحيح yscale = font->y_scale ? font->y_scale : upem;

    ڈالو_transform (paint_انکڑے,
		    upem/xscale, 0,
		    0, upem/yscale,
		    0, 0);
  }

  فراغ ڈالو_transform (فراغ *paint_انکڑے, hb_transform_t<عائم> t)
  {
    ڈالو_transform (paint_انکڑے, t.xx, t.yx, t.xy, t.yy, t.x0, t.y0);
  }

  فراغ ڈالو_translate (فراغ *paint_انکڑے,
                       عائم dx, عائم dy)
  {
    ڈالو_transform (paint_انکڑے,
		    hb_transform_t<عائم>::translation (dx, dy));
  }

  فراغ ڈالو_scale (فراغ *paint_انکڑے,
                   عائم sx, عائم sy)
  {
    ڈالو_transform (paint_انکڑے,
		    hb_transform_t<عائم>::scaling (sx, sy));
  }
  فراغ ڈالو_scale_around_center (فراغ *paint_انکڑے,
				 عائم sx, عائم sy,
				 عائم cx, عائم cy)
  {
    ڈالو_transform (paint_انکڑے,
		    hb_transform_t<عائم>::scaling_around_center (sx, sy, cx, cy));
  }

  فراغ ڈالو_rotate (فراغ *paint_انکڑے,
                    عائم a)
  {
    ڈالو_transform (paint_انکڑے,
		    hb_transform_t<عائم>::rotation (a * HB_PI));
  }

  فراغ ڈالو_rotate_around_center (فراغ *paint_انکڑے,
				  عائم a,
				  عائم cx, عائم cy)
  {
    ڈالو_transform (paint_انکڑے,
		    hb_transform_t<عائم>::rotation_around_center (a * HB_PI, cx, cy));
  }

  فراغ ڈالو_skew (فراغ *paint_انکڑے,
                  عائم sx, عائم sy)
  {
    ڈالو_transform (paint_انکڑے,
		    hb_transform_t<عائم>::skewing (-sx * HB_PI, sy * HB_PI));
  }
  فراغ ڈالو_skew_around_center (فراغ *paint_انکڑے,
				عائم sx, عائم sy,
				عائم cx, عائم cy)
  {
    ڈالو_transform (paint_انکڑے,
		    hb_transform_t<عائم>::skewing_around_center (-sx * HB_PI, sy * HB_PI, cx, cy));
  }
};
DECLARE_NULL_INSTANCE (hb_paint_funcs_t);


/* Linearly interpolate between two hb_color_t values, component-wise,
 * in byte-channel space with rounding. */
ساكن مضمن hb_color_t
hb_color_lerp (hb_color_t c0, hb_color_t c1, عائم t)
{
  خودکار lerp = [&] (بنانشانی کھسکاؤ) -> بنانشانی {
    بنانشانی v0 = (c0 >> کھسکاؤ) & 0xFF;
    بنانشانی v1 = (c1 >> کھسکاؤ) & 0xFF;
    عائم v = v0 + t * ((عائم) v1 - (عائم) v0) + 0.5f;
    /* t may be outside [0,1] (or NaN) for malformed gradients;
     * keep the float-to-unsigned conversion defined. */
    إذا (!(v > 0.f)) أرجع 0;
    إذا (v > 255.f) أرجع 255;
    أرجع (بنانشانی) v;
  };
  أرجع HB_COLOR (lerp (0), lerp (8), lerp (16), lerp (24));
}

#جے_انت /* HB_PAINT_HH */

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

#اگر_تعریف_نشده HB_PAINT_HH
#تعریف_کن HB_PAINT_HH

#ضمن "hb.hh"
#ضمن "hb-face.hh"
#ضمن "hb-font.hh"
#ضمن "hb-geometry.hh"

#تعریف_کن HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS \
  HB_PAINT_FUNC_IMPLEMENT (افزودن_transform) \
  HB_PAINT_FUNC_IMPLEMENT (برداشت_transform) \
  HB_PAINT_FUNC_IMPLEMENT (color_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (افزودن_clip_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (افزودن_clip_rectangle) \
  HB_PAINT_FUNC_IMPLEMENT (افزودن_clip_مسیر_شروع) \
  HB_PAINT_FUNC_IMPLEMENT (افزودن_clip_مسیر_انتها) \
  HB_PAINT_FUNC_IMPLEMENT (برداشت_clip) \
  HB_PAINT_FUNC_IMPLEMENT (color) \
  HB_PAINT_FUNC_IMPLEMENT (image) \
  HB_PAINT_FUNC_IMPLEMENT (خطی_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (radial_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (sweep_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (افزودن_گروه) \
  HB_PAINT_FUNC_IMPLEMENT (افزودن_گروه_for) \
  HB_PAINT_FUNC_IMPLEMENT (برداشت_گروه) \
  HB_PAINT_FUNC_IMPLEMENT (custom_palette_color) \
  HB_PAINT_FUNC_IMPLEMENT (fill_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (مجموعه_budget) \
  HB_PAINT_FUNC_IMPLEMENT (get_budget) \
  HB_PAINT_FUNC_IMPLEMENT (get_budget_مانده) \
  /* ^--- Add new callbacks here */

بنية hb_paint_funcs_t
{
  hb_object_سرآیند_t سرآیند;

  بنية {
#تعریف_کن HB_PAINT_FUNC_IMPLEMENT(name) hb_paint_##name##_func_t name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#لغو_تعریف HB_PAINT_FUNC_IMPLEMENT
  } func;

  بنية {
#تعریف_کن HB_PAINT_FUNC_IMPLEMENT(name) فراغ *name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#لغو_تعریف HB_PAINT_FUNC_IMPLEMENT
  } *user_داده;

  بنية {
#تعریف_کن HB_PAINT_FUNC_IMPLEMENT(name) hb_destroy_func_t name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#لغو_تعریف HB_PAINT_FUNC_IMPLEMENT
  } *destroy;

  فراغ افزودن_transform (فراغ *paint_داده,
                       عائم xx, عائم yx,
                       عائم xy, عائم yy,
                       عائم dx, عائم dy)
  {
    // Handle -0.f to avoid -0.f == 0.f in the transform matrix.
    إذا (dx == -0.f) dx = 0.f;
    إذا (dy == -0.f) dy = 0.f;
    func.افزودن_transform (هذا, paint_داده,
                         xx, yx, xy, yy, dx, dy,
                         !user_داده ? اشاره_تهی : user_داده->افزودن_transform); }
  فراغ برداشت_transform (فراغ *paint_داده)
  { func.برداشت_transform (هذا, paint_داده,
                        !user_داده ? اشاره_تهی : user_داده->برداشت_transform); }
  منطقي color_glyph (فراغ *paint_داده,
                    hb_codepoint_t glyph,
                    hb_font_t *font)
  { أرجع func.color_glyph (هذا, paint_داده,
                             glyph,
                             font,
                             !user_داده ? اشاره_تهی : user_داده->color_glyph); }
  فراغ افزودن_clip_glyph (فراغ *paint_داده,
                        hb_codepoint_t glyph,
                        hb_font_t *font)
  { func.افزودن_clip_glyph (هذا, paint_داده,
                          glyph,
                          font,
                          !user_داده ? اشاره_تهی : user_داده->افزودن_clip_glyph); }
  فراغ افزودن_clip_rectangle (فراغ *paint_داده,
                           عائم xmin, عائم ymin, عائم xmax, عائم ymax)
  { func.افزودن_clip_rectangle (هذا, paint_داده,
                              xmin, ymin, xmax, ymax,
                              !user_داده ? اشاره_تهی : user_داده->افزودن_clip_rectangle); }
  hb_draw_funcs_t *افزودن_clip_مسیر_شروع (فراغ *paint_داده,
                                         فراغ **draw_داده)
  { أرجع func.افزودن_clip_مسیر_شروع (هذا, paint_داده, draw_داده,
                                      !user_داده ? اشاره_تهی : user_داده->افزودن_clip_مسیر_شروع); }
  فراغ افزودن_clip_مسیر_انتها (فراغ *paint_داده)
  { func.افزودن_clip_مسیر_انتها (هذا, paint_داده,
                             !user_داده ? اشاره_تهی : user_داده->افزودن_clip_مسیر_انتها); }
  فراغ برداشت_clip (فراغ *paint_داده)
  { func.برداشت_clip (هذا, paint_داده,
                   !user_داده ? اشاره_تهی : user_داده->برداشت_clip); }
  فراغ color (فراغ *paint_داده,
              hb_bool_t is_foreground,
              hb_color_t color)
  { func.color (هذا, paint_داده,
                is_foreground, color,
                !user_داده ? اشاره_تهی : user_داده->color); }
  فراغ fill_glyph (فراغ *paint_داده,
                   hb_codepoint_t glyph,
                   hb_font_t *font,
                   hb_bool_t is_foreground,
                   hb_color_t color)
  { func.fill_glyph (هذا, paint_داده,
                     glyph, font,
                     is_foreground, color,
                     !user_داده ? اشاره_تهی : user_داده->fill_glyph); }
  منطقي image (فراغ *paint_داده,
              hb_blob_t *image,
              بیعلامت عرض, بیعلامت ارتفاع,
              hb_برچسب_t format,
              عائم slant,
              hb_glyph_extents_t *extents)
  { أرجع func.image (هذا, paint_داده,
                       image, عرض, ارتفاع, format, slant, extents,
                       !user_داده ? اشاره_تهی : user_داده->image); }
  فراغ خطی_gradient (فراغ *paint_داده,
                        hb_color_سطر_t *color_سطر,
                        عائم x0, عائم y0,
                        عائم x1, عائم y1,
                        عائم x2, عائم y2)
  { func.خطی_gradient (هذا, paint_داده,
                          color_سطر, x0, y0, x1, y1, x2, y2,
                          !user_داده ? اشاره_تهی : user_داده->خطی_gradient); }
  فراغ radial_gradient (فراغ *paint_داده,
                        hb_color_سطر_t *color_سطر,
                        عائم x0, عائم y0, عائم r0,
                        عائم x1, عائم y1, عائم r1)
  { func.radial_gradient (هذا, paint_داده,
                          color_سطر, x0, y0, r0, x1, y1, r1,
                          !user_داده ? اشاره_تهی : user_داده->radial_gradient); }
  فراغ sweep_gradient (فراغ *paint_داده,
                       hb_color_سطر_t *color_سطر,
                       عائم x0, عائم y0,
                       عائم شروع_angle,
                       عائم انتها_angle)
  { func.sweep_gradient (هذا, paint_داده,
                         color_سطر, x0, y0, شروع_angle, انتها_angle,
                         !user_داده ? اشاره_تهی : user_داده->sweep_gradient); }
  فراغ افزودن_گروه (فراغ *paint_داده)
  { func.افزودن_گروه (هذا, paint_داده,
                     !user_داده ? اشاره_تهی : user_داده->افزودن_گروه); }
  فراغ افزودن_گروه_for (فراغ *paint_داده,
                       hb_paint_مرکب_نما_t نما)
  { func.افزودن_گروه_for (هذا, paint_داده,
                         نما,
                         !user_داده ? اشاره_تهی : user_داده->افزودن_گروه_for); }
  فراغ برداشت_گروه (فراغ *paint_داده,
                  hb_paint_مرکب_نما_t نما)
  { func.برداشت_گروه (هذا, paint_داده,
                    نما,
                    !user_داده ? اشاره_تهی : user_داده->برداشت_گروه); }
  منطقي custom_palette_color (فراغ *paint_داده,
                             بیعلامت صحيح color_شاخص,
                             hb_color_t *color)
  { أرجع func.custom_palette_color (هذا, paint_داده,
                                      color_شاخص,
                                      color,
                                      !user_داده ? اشاره_تهی : user_داده->custom_palette_color); }
  منطقي مجموعه_budget (فراغ *paint_داده, int64_t budget)
  {
    إذا (budget < 0 && budget != HB_BUDGET_DEFAULT)
      budget = 0;
    أرجع func.مجموعه_budget (هذا, paint_داده, budget,
			    !user_داده ? اشاره_تهی : user_داده->مجموعه_budget);
  }
  int64_t get_budget (فراغ *paint_داده)
  { أرجع func.get_budget (هذا, paint_داده,
			    !user_داده ? اشاره_تهی : user_داده->get_budget); }
  int64_t *get_budget_مانده_ptr (فراغ *paint_داده)
  { أرجع func.get_budget_مانده (هذا, paint_داده,
				      !user_داده ? اشاره_تهی : user_داده->get_budget_مانده); }


  /* Internal specializations. */

  فراغ افزودن_font_transform (فراغ *paint_داده,
                            ثابت hb_font_t *font)
  {
    عائم upem = font->face->get_upem ();
    صحيح xscale = font->x_scale, yscale = font->y_scale;

    افزودن_transform (paint_داده,
		    xscale/upem, 0,
		    0, yscale/upem,
		    0, 0);
  }

  فراغ افزودن_inverse_font_transform (فراغ *paint_داده,
                                    ثابت hb_font_t *font)
  {
    عائم upem = font->face->get_upem ();
    صحيح xscale = font->x_scale ? font->x_scale : upem;
    صحيح yscale = font->y_scale ? font->y_scale : upem;

    افزودن_transform (paint_داده,
		    upem/xscale, 0,
		    0, upem/yscale,
		    0, 0);
  }

  فراغ افزودن_transform (فراغ *paint_داده, hb_transform_t<عائم> t)
  {
    افزودن_transform (paint_داده, t.xx, t.yx, t.xy, t.yy, t.x0, t.y0);
  }

  فراغ افزودن_translate (فراغ *paint_داده,
                       عائم dx, عائم dy)
  {
    افزودن_transform (paint_داده,
		    hb_transform_t<عائم>::translation (dx, dy));
  }

  فراغ افزودن_scale (فراغ *paint_داده,
                   عائم sx, عائم sy)
  {
    افزودن_transform (paint_داده,
		    hb_transform_t<عائم>::scaling (sx, sy));
  }
  فراغ افزودن_scale_around_center (فراغ *paint_داده,
				 عائم sx, عائم sy,
				 عائم cx, عائم cy)
  {
    افزودن_transform (paint_داده,
		    hb_transform_t<عائم>::scaling_around_center (sx, sy, cx, cy));
  }

  فراغ افزودن_rotate (فراغ *paint_داده,
                    عائم a)
  {
    افزودن_transform (paint_داده,
		    hb_transform_t<عائم>::rotation (a * HB_PI));
  }

  فراغ افزودن_rotate_around_center (فراغ *paint_داده,
				  عائم a,
				  عائم cx, عائم cy)
  {
    افزودن_transform (paint_داده,
		    hb_transform_t<عائم>::rotation_around_center (a * HB_PI, cx, cy));
  }

  فراغ افزودن_skew (فراغ *paint_داده,
                  عائم sx, عائم sy)
  {
    افزودن_transform (paint_داده,
		    hb_transform_t<عائم>::skewing (-sx * HB_PI, sy * HB_PI));
  }
  فراغ افزودن_skew_around_center (فراغ *paint_داده,
				عائم sx, عائم sy,
				عائم cx, عائم cy)
  {
    افزودن_transform (paint_داده,
		    hb_transform_t<عائم>::skewing_around_center (-sx * HB_PI, sy * HB_PI, cx, cy));
  }
};
DECLARE_NULL_INSTANCE (hb_paint_funcs_t);


/* Linearly interpolate between two hb_color_t values, component-wise,
 * in byte-channel space with rounding. */
ساكن مضمن hb_color_t
hb_color_lerp (hb_color_t c0, hb_color_t c1, عائم t)
{
  خودکار lerp = [&] (بیعلامت لغزش) -> بیعلامت {
    بیعلامت v0 = (c0 >> لغزش) & 0xFF;
    بیعلامت v1 = (c1 >> لغزش) & 0xFF;
    عائم v = v0 + t * ((عائم) v1 - (عائم) v0) + 0.5f;
    /* t may be outside [0,1] (or NaN) for malformed gradients;
     * keep the float-to-unsigned conversion defined. */
    إذا (!(v > 0.f)) أرجع 0;
    إذا (v > 255.f) أرجع 255;
    أرجع (بیعلامت) v;
  };
  أرجع HB_COLOR (lerp (0), lerp (8), lerp (16), lerp (24));
}

#پایان_اگر /* HB_PAINT_HH */

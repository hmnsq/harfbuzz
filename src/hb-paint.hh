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

#اگر_غیر_تعریف_شدہ HB_PAINT_HH
#تعریف_کرو HB_PAINT_HH

#شامل "hb.hh"
#شامل "hb-face.hh"
#شامل "hb-font.hh"
#شامل "hb-geometry.hh"

#تعریف_کرو HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS \
  HB_PAINT_FUNC_IMPLEMENT (ڈالو_transform) \
  HB_PAINT_FUNC_IMPLEMENT (نکالو_transform) \
  HB_PAINT_FUNC_IMPLEMENT (color_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (ڈالو_clip_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (ڈالو_clip_rectangle) \
  HB_PAINT_FUNC_IMPLEMENT (ڈالو_clip_راستہ_آغاز) \
  HB_PAINT_FUNC_IMPLEMENT (ڈالو_clip_راستہ_انتہا) \
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
  HB_PAINT_FUNC_IMPLEMENT (get_budget_بقایا) \
  /* ^--- Add new callbacks here */

ساخت hb_paint_funcs_t
{
  hb_object_سرخی_t سرخی;

  ساخت {
#تعریف_کرو HB_PAINT_FUNC_IMPLEMENT(name) hb_paint_##name##_func_t name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#تعریف_منسوخ HB_PAINT_FUNC_IMPLEMENT
  } func;

  ساخت {
#تعریف_کرو HB_PAINT_FUNC_IMPLEMENT(name) خالی *name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#تعریف_منسوخ HB_PAINT_FUNC_IMPLEMENT
  } *user_ڈیٹا;

  ساخت {
#تعریف_کرو HB_PAINT_FUNC_IMPLEMENT(name) hb_destroy_func_t name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#تعریف_منسوخ HB_PAINT_FUNC_IMPLEMENT
  } *destroy;

  خالی ڈالو_transform (خالی *paint_ڈیٹا,
                       اعشاری xx, اعشاری yx,
                       اعشاری xy, اعشاری yy,
                       اعشاری dx, اعشاری dy)
  {
    // Handle -0.f to avoid -0.f == 0.f in the transform matrix.
    اگر (dx == -0.f) dx = 0.f;
    اگر (dy == -0.f) dy = 0.f;
    func.ڈالو_transform (یہ, paint_ڈیٹا,
                         xx, yx, xy, yy, dx, dy,
                         !user_ڈیٹا ? خالی_اشاریہ : user_ڈیٹا->ڈالو_transform); }
  خالی نکالو_transform (خالی *paint_ڈیٹا)
  { func.نکالو_transform (یہ, paint_ڈیٹا,
                        !user_ڈیٹا ? خالی_اشاریہ : user_ڈیٹا->نکالو_transform); }
  منطقی color_glyph (خالی *paint_ڈیٹا,
                    hb_codepoint_t glyph,
                    hb_font_t *font)
  { واپس func.color_glyph (یہ, paint_ڈیٹا,
                             glyph,
                             font,
                             !user_ڈیٹا ? خالی_اشاریہ : user_ڈیٹا->color_glyph); }
  خالی ڈالو_clip_glyph (خالی *paint_ڈیٹا,
                        hb_codepoint_t glyph,
                        hb_font_t *font)
  { func.ڈالو_clip_glyph (یہ, paint_ڈیٹا,
                          glyph,
                          font,
                          !user_ڈیٹا ? خالی_اشاریہ : user_ڈیٹا->ڈالو_clip_glyph); }
  خالی ڈالو_clip_rectangle (خالی *paint_ڈیٹا,
                           اعشاری xmin, اعشاری ymin, اعشاری xmax, اعشاری ymax)
  { func.ڈالو_clip_rectangle (یہ, paint_ڈیٹا,
                              xmin, ymin, xmax, ymax,
                              !user_ڈیٹا ? خالی_اشاریہ : user_ڈیٹا->ڈالو_clip_rectangle); }
  hb_draw_funcs_t *ڈالو_clip_راستہ_آغاز (خالی *paint_ڈیٹا,
                                         خالی **draw_ڈیٹا)
  { واپس func.ڈالو_clip_راستہ_آغاز (یہ, paint_ڈیٹا, draw_ڈیٹا,
                                      !user_ڈیٹا ? خالی_اشاریہ : user_ڈیٹا->ڈالو_clip_راستہ_آغاز); }
  خالی ڈالو_clip_راستہ_انتہا (خالی *paint_ڈیٹا)
  { func.ڈالو_clip_راستہ_انتہا (یہ, paint_ڈیٹا,
                             !user_ڈیٹا ? خالی_اشاریہ : user_ڈیٹا->ڈالو_clip_راستہ_انتہا); }
  خالی نکالو_clip (خالی *paint_ڈیٹا)
  { func.نکالو_clip (یہ, paint_ڈیٹا,
                   !user_ڈیٹا ? خالی_اشاریہ : user_ڈیٹا->نکالو_clip); }
  خالی color (خالی *paint_ڈیٹا,
              hb_bool_t is_foreground,
              hb_color_t color)
  { func.color (یہ, paint_ڈیٹا,
                is_foreground, color,
                !user_ڈیٹا ? خالی_اشاریہ : user_ڈیٹا->color); }
  خالی fill_glyph (خالی *paint_ڈیٹا,
                   hb_codepoint_t glyph,
                   hb_font_t *font,
                   hb_bool_t is_foreground,
                   hb_color_t color)
  { func.fill_glyph (یہ, paint_ڈیٹا,
                     glyph, font,
                     is_foreground, color,
                     !user_ڈیٹا ? خالی_اشاریہ : user_ڈیٹا->fill_glyph); }
  منطقی image (خالی *paint_ڈیٹا,
              hb_blob_t *image,
              غیرعلامتی چوڑائی, غیرعلامتی اونچائی,
              hb_لیبل_t format,
              اعشاری slant,
              hb_glyph_extents_t *extents)
  { واپس func.image (یہ, paint_ڈیٹا,
                       image, چوڑائی, اونچائی, format, slant, extents,
                       !user_ڈیٹا ? خالی_اشاریہ : user_ڈیٹا->image); }
  خالی خطی_gradient (خالی *paint_ڈیٹا,
                        hb_color_سطر_t *color_سطر,
                        اعشاری x0, اعشاری y0,
                        اعشاری x1, اعشاری y1,
                        اعشاری x2, اعشاری y2)
  { func.خطی_gradient (یہ, paint_ڈیٹا,
                          color_سطر, x0, y0, x1, y1, x2, y2,
                          !user_ڈیٹا ? خالی_اشاریہ : user_ڈیٹا->خطی_gradient); }
  خالی radial_gradient (خالی *paint_ڈیٹا,
                        hb_color_سطر_t *color_سطر,
                        اعشاری x0, اعشاری y0, اعشاری r0,
                        اعشاری x1, اعشاری y1, اعشاری r1)
  { func.radial_gradient (یہ, paint_ڈیٹا,
                          color_سطر, x0, y0, r0, x1, y1, r1,
                          !user_ڈیٹا ? خالی_اشاریہ : user_ڈیٹا->radial_gradient); }
  خالی sweep_gradient (خالی *paint_ڈیٹا,
                       hb_color_سطر_t *color_سطر,
                       اعشاری x0, اعشاری y0,
                       اعشاری آغاز_angle,
                       اعشاری انتہا_angle)
  { func.sweep_gradient (یہ, paint_ڈیٹا,
                         color_سطر, x0, y0, آغاز_angle, انتہا_angle,
                         !user_ڈیٹا ? خالی_اشاریہ : user_ڈیٹا->sweep_gradient); }
  خالی ڈالو_گروہ (خالی *paint_ڈیٹا)
  { func.ڈالو_گروہ (یہ, paint_ڈیٹا,
                     !user_ڈیٹا ? خالی_اشاریہ : user_ڈیٹا->ڈالو_گروہ); }
  خالی ڈالو_گروہ_for (خالی *paint_ڈیٹا,
                       hb_paint_مرکب_منوال_t منوال)
  { func.ڈالو_گروہ_for (یہ, paint_ڈیٹا,
                         منوال,
                         !user_ڈیٹا ? خالی_اشاریہ : user_ڈیٹا->ڈالو_گروہ_for); }
  خالی نکالو_گروہ (خالی *paint_ڈیٹا,
                  hb_paint_مرکب_منوال_t منوال)
  { func.نکالو_گروہ (یہ, paint_ڈیٹا,
                    منوال,
                    !user_ڈیٹا ? خالی_اشاریہ : user_ڈیٹا->نکالو_گروہ); }
  منطقی custom_palette_color (خالی *paint_ڈیٹا,
                             غیرعلامتی صحیح color_اشاریہ,
                             hb_color_t *color)
  { واپس func.custom_palette_color (یہ, paint_ڈیٹا,
                                      color_اشاریہ,
                                      color,
                                      !user_ڈیٹا ? خالی_اشاریہ : user_ڈیٹا->custom_palette_color); }
  منطقی سیٹ_budget (خالی *paint_ڈیٹا, int64_t budget)
  {
    اگر (budget < 0 && budget != HB_BUDGET_DEFAULT)
      budget = 0;
    واپس func.سیٹ_budget (یہ, paint_ڈیٹا, budget,
			    !user_ڈیٹا ? خالی_اشاریہ : user_ڈیٹا->سیٹ_budget);
  }
  int64_t get_budget (خالی *paint_ڈیٹا)
  { واپس func.get_budget (یہ, paint_ڈیٹا,
			    !user_ڈیٹا ? خالی_اشاریہ : user_ڈیٹا->get_budget); }
  int64_t *get_budget_بقایا_ptr (خالی *paint_ڈیٹا)
  { واپس func.get_budget_بقایا (یہ, paint_ڈیٹا,
				      !user_ڈیٹا ? خالی_اشاریہ : user_ڈیٹا->get_budget_بقایا); }


  /* Internal specializations. */

  خالی ڈالو_font_transform (خالی *paint_ڈیٹا,
                            ثابت hb_font_t *font)
  {
    اعشاری upem = font->face->get_upem ();
    صحیح xscale = font->x_scale, yscale = font->y_scale;

    ڈالو_transform (paint_ڈیٹا,
		    xscale/upem, 0,
		    0, yscale/upem,
		    0, 0);
  }

  خالی ڈالو_inverse_font_transform (خالی *paint_ڈیٹا,
                                    ثابت hb_font_t *font)
  {
    اعشاری upem = font->face->get_upem ();
    صحیح xscale = font->x_scale ? font->x_scale : upem;
    صحیح yscale = font->y_scale ? font->y_scale : upem;

    ڈالو_transform (paint_ڈیٹا,
		    upem/xscale, 0,
		    0, upem/yscale,
		    0, 0);
  }

  خالی ڈالو_transform (خالی *paint_ڈیٹا, hb_transform_t<اعشاری> t)
  {
    ڈالو_transform (paint_ڈیٹا, t.xx, t.yx, t.xy, t.yy, t.x0, t.y0);
  }

  خالی ڈالو_translate (خالی *paint_ڈیٹا,
                       اعشاری dx, اعشاری dy)
  {
    ڈالو_transform (paint_ڈیٹا,
		    hb_transform_t<اعشاری>::translation (dx, dy));
  }

  خالی ڈالو_scale (خالی *paint_ڈیٹا,
                   اعشاری sx, اعشاری sy)
  {
    ڈالو_transform (paint_ڈیٹا,
		    hb_transform_t<اعشاری>::scaling (sx, sy));
  }
  خالی ڈالو_scale_around_center (خالی *paint_ڈیٹا,
				 اعشاری sx, اعشاری sy,
				 اعشاری cx, اعشاری cy)
  {
    ڈالو_transform (paint_ڈیٹا,
		    hb_transform_t<اعشاری>::scaling_around_center (sx, sy, cx, cy));
  }

  خالی ڈالو_rotate (خالی *paint_ڈیٹا,
                    اعشاری a)
  {
    ڈالو_transform (paint_ڈیٹا,
		    hb_transform_t<اعشاری>::rotation (a * HB_PI));
  }

  خالی ڈالو_rotate_around_center (خالی *paint_ڈیٹا,
				  اعشاری a,
				  اعشاری cx, اعشاری cy)
  {
    ڈالو_transform (paint_ڈیٹا,
		    hb_transform_t<اعشاری>::rotation_around_center (a * HB_PI, cx, cy));
  }

  خالی ڈالو_skew (خالی *paint_ڈیٹا,
                  اعشاری sx, اعشاری sy)
  {
    ڈالو_transform (paint_ڈیٹا,
		    hb_transform_t<اعشاری>::skewing (-sx * HB_PI, sy * HB_PI));
  }
  خالی ڈالو_skew_around_center (خالی *paint_ڈیٹا,
				اعشاری sx, اعشاری sy,
				اعشاری cx, اعشاری cy)
  {
    ڈالو_transform (paint_ڈیٹا,
		    hb_transform_t<اعشاری>::skewing_around_center (-sx * HB_PI, sy * HB_PI, cx, cy));
  }
};
DECLARE_NULL_INSTANCE (hb_paint_funcs_t);


/* Linearly interpolate between two hb_color_t values, component-wise,
 * in byte-channel space with rounding. */
ساکن اندرونی hb_color_t
hb_color_lerp (hb_color_t c0, hb_color_t c1, اعشاری t)
{
  خودکار lerp = [&] (غیرعلامتی سرکاؤ) -> غیرعلامتی {
    غیرعلامتی v0 = (c0 >> سرکاؤ) & 0xFF;
    غیرعلامتی v1 = (c1 >> سرکاؤ) & 0xFF;
    اعشاری v = v0 + t * ((اعشاری) v1 - (اعشاری) v0) + 0.5f;
    /* t may be outside [0,1] (or NaN) for malformed gradients;
     * keep the float-to-unsigned conversion defined. */
    اگر (!(v > 0.f)) واپس 0;
    اگر (v > 255.f) واپس 255;
    واپس (غیرعلامتی) v;
  };
  واپس HB_COLOR (lerp (0), lerp (8), lerp (16), lerp (24));
}

#اگر_ختم /* HB_PAINT_HH */

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

#جيڪڏهن_وصف_نه_ٿيل HB_PAINT_HH
#وصف_ڪريو HB_PAINT_HH

#ضمن "hb.hh"
#ضمن "hb-face.hh"
#ضمن "hb-font.hh"
#ضمن "hb-geometry.hh"

#وصف_ڪريو HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS \
  HB_PAINT_FUNC_IMPLEMENT (ڌڪو_transform) \
  HB_PAINT_FUNC_IMPLEMENT (ڪڍو_transform) \
  HB_PAINT_FUNC_IMPLEMENT (color_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (ڌڪو_clip_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (ڌڪو_clip_rectangle) \
  HB_PAINT_FUNC_IMPLEMENT (ڌڪو_clip_رستو_شروعات) \
  HB_PAINT_FUNC_IMPLEMENT (ڌڪو_clip_رستو_پڇاڙي) \
  HB_PAINT_FUNC_IMPLEMENT (ڪڍو_clip) \
  HB_PAINT_FUNC_IMPLEMENT (color) \
  HB_PAINT_FUNC_IMPLEMENT (image) \
  HB_PAINT_FUNC_IMPLEMENT (لڪيري_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (radial_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (sweep_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (ڌڪو_گروهه) \
  HB_PAINT_FUNC_IMPLEMENT (ڌڪو_گروهه_for) \
  HB_PAINT_FUNC_IMPLEMENT (ڪڍو_گروهه) \
  HB_PAINT_FUNC_IMPLEMENT (custom_palette_color) \
  HB_PAINT_FUNC_IMPLEMENT (fill_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (سيٽ_budget) \
  HB_PAINT_FUNC_IMPLEMENT (get_budget) \
  HB_PAINT_FUNC_IMPLEMENT (get_budget_بچيل) \
  /* ^--- Add new callbacks here */

بنية hb_paint_funcs_t
{
  hb_object_سرو_t سرو;

  بنية {
#وصف_ڪريو HB_PAINT_FUNC_IMPLEMENT(name) hb_paint_##name##_func_t name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#وصف_رد HB_PAINT_FUNC_IMPLEMENT
  } func;

  بنية {
#وصف_ڪريو HB_PAINT_FUNC_IMPLEMENT(name) فراغ *name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#وصف_رد HB_PAINT_FUNC_IMPLEMENT
  } *user_معلومات;

  بنية {
#وصف_ڪريو HB_PAINT_FUNC_IMPLEMENT(name) hb_destroy_func_t name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#وصف_رد HB_PAINT_FUNC_IMPLEMENT
  } *destroy;

  فراغ ڌڪو_transform (فراغ *paint_معلومات,
                       عائم xx, عائم yx,
                       عائم xy, عائم yy,
                       عائم dx, عائم dy)
  {
    // Handle -0.f to avoid -0.f == 0.f in the transform matrix.
    إذا (dx == -0.f) dx = 0.f;
    إذا (dy == -0.f) dy = 0.f;
    func.ڌڪو_transform (هذا, paint_معلومات,
                         xx, yx, xy, yy, dx, dy,
                         !user_معلومات ? خالي_اشارو : user_معلومات->ڌڪو_transform); }
  فراغ ڪڍو_transform (فراغ *paint_معلومات)
  { func.ڪڍو_transform (هذا, paint_معلومات,
                        !user_معلومات ? خالي_اشارو : user_معلومات->ڪڍو_transform); }
  منطقي color_glyph (فراغ *paint_معلومات,
                    hb_codepoint_t glyph,
                    hb_font_t *font)
  { أرجع func.color_glyph (هذا, paint_معلومات,
                             glyph,
                             font,
                             !user_معلومات ? خالي_اشارو : user_معلومات->color_glyph); }
  فراغ ڌڪو_clip_glyph (فراغ *paint_معلومات,
                        hb_codepoint_t glyph,
                        hb_font_t *font)
  { func.ڌڪو_clip_glyph (هذا, paint_معلومات,
                          glyph,
                          font,
                          !user_معلومات ? خالي_اشارو : user_معلومات->ڌڪو_clip_glyph); }
  فراغ ڌڪو_clip_rectangle (فراغ *paint_معلومات,
                           عائم xmin, عائم ymin, عائم xmax, عائم ymax)
  { func.ڌڪو_clip_rectangle (هذا, paint_معلومات,
                              xmin, ymin, xmax, ymax,
                              !user_معلومات ? خالي_اشارو : user_معلومات->ڌڪو_clip_rectangle); }
  hb_draw_funcs_t *ڌڪو_clip_رستو_شروعات (فراغ *paint_معلومات,
                                         فراغ **draw_معلومات)
  { أرجع func.ڌڪو_clip_رستو_شروعات (هذا, paint_معلومات, draw_معلومات,
                                      !user_معلومات ? خالي_اشارو : user_معلومات->ڌڪو_clip_رستو_شروعات); }
  فراغ ڌڪو_clip_رستو_پڇاڙي (فراغ *paint_معلومات)
  { func.ڌڪو_clip_رستو_پڇاڙي (هذا, paint_معلومات,
                             !user_معلومات ? خالي_اشارو : user_معلومات->ڌڪو_clip_رستو_پڇاڙي); }
  فراغ ڪڍو_clip (فراغ *paint_معلومات)
  { func.ڪڍو_clip (هذا, paint_معلومات,
                   !user_معلومات ? خالي_اشارو : user_معلومات->ڪڍو_clip); }
  فراغ color (فراغ *paint_معلومات,
              hb_bool_t is_foreground,
              hb_color_t color)
  { func.color (هذا, paint_معلومات,
                is_foreground, color,
                !user_معلومات ? خالي_اشارو : user_معلومات->color); }
  فراغ fill_glyph (فراغ *paint_معلومات,
                   hb_codepoint_t glyph,
                   hb_font_t *font,
                   hb_bool_t is_foreground,
                   hb_color_t color)
  { func.fill_glyph (هذا, paint_معلومات,
                     glyph, font,
                     is_foreground, color,
                     !user_معلومات ? خالي_اشارو : user_معلومات->fill_glyph); }
  منطقي image (فراغ *paint_معلومات,
              hb_blob_t *image,
              بنانشان ويڪر, بنانشان اوچائي,
              hb_ٽيگ_t format,
              عائم slant,
              hb_glyph_extents_t *extents)
  { أرجع func.image (هذا, paint_معلومات,
                       image, ويڪر, اوچائي, format, slant, extents,
                       !user_معلومات ? خالي_اشارو : user_معلومات->image); }
  فراغ لڪيري_gradient (فراغ *paint_معلومات,
                        hb_color_سٽ_t *color_سٽ,
                        عائم x0, عائم y0,
                        عائم x1, عائم y1,
                        عائم x2, عائم y2)
  { func.لڪيري_gradient (هذا, paint_معلومات,
                          color_سٽ, x0, y0, x1, y1, x2, y2,
                          !user_معلومات ? خالي_اشارو : user_معلومات->لڪيري_gradient); }
  فراغ radial_gradient (فراغ *paint_معلومات,
                        hb_color_سٽ_t *color_سٽ,
                        عائم x0, عائم y0, عائم r0,
                        عائم x1, عائم y1, عائم r1)
  { func.radial_gradient (هذا, paint_معلومات,
                          color_سٽ, x0, y0, r0, x1, y1, r1,
                          !user_معلومات ? خالي_اشارو : user_معلومات->radial_gradient); }
  فراغ sweep_gradient (فراغ *paint_معلومات,
                       hb_color_سٽ_t *color_سٽ,
                       عائم x0, عائم y0,
                       عائم شروعات_angle,
                       عائم پڇاڙي_angle)
  { func.sweep_gradient (هذا, paint_معلومات,
                         color_سٽ, x0, y0, شروعات_angle, پڇاڙي_angle,
                         !user_معلومات ? خالي_اشارو : user_معلومات->sweep_gradient); }
  فراغ ڌڪو_گروهه (فراغ *paint_معلومات)
  { func.ڌڪو_گروهه (هذا, paint_معلومات,
                     !user_معلومات ? خالي_اشارو : user_معلومات->ڌڪو_گروهه); }
  فراغ ڌڪو_گروهه_for (فراغ *paint_معلومات,
                       hb_paint_مرکب_گهڻائي_t گهڻائي)
  { func.ڌڪو_گروهه_for (هذا, paint_معلومات,
                         گهڻائي,
                         !user_معلومات ? خالي_اشارو : user_معلومات->ڌڪو_گروهه_for); }
  فراغ ڪڍو_گروهه (فراغ *paint_معلومات,
                  hb_paint_مرکب_گهڻائي_t گهڻائي)
  { func.ڪڍو_گروهه (هذا, paint_معلومات,
                    گهڻائي,
                    !user_معلومات ? خالي_اشارو : user_معلومات->ڪڍو_گروهه); }
  منطقي custom_palette_color (فراغ *paint_معلومات,
                             بنانشان صحيح color_اشاريو,
                             hb_color_t *color)
  { أرجع func.custom_palette_color (هذا, paint_معلومات,
                                      color_اشاريو,
                                      color,
                                      !user_معلومات ? خالي_اشارو : user_معلومات->custom_palette_color); }
  منطقي سيٽ_budget (فراغ *paint_معلومات, int64_t budget)
  {
    إذا (budget < 0 && budget != HB_BUDGET_DEFAULT)
      budget = 0;
    أرجع func.سيٽ_budget (هذا, paint_معلومات, budget,
			    !user_معلومات ? خالي_اشارو : user_معلومات->سيٽ_budget);
  }
  int64_t get_budget (فراغ *paint_معلومات)
  { أرجع func.get_budget (هذا, paint_معلومات,
			    !user_معلومات ? خالي_اشارو : user_معلومات->get_budget); }
  int64_t *get_budget_بچيل_ptr (فراغ *paint_معلومات)
  { أرجع func.get_budget_بچيل (هذا, paint_معلومات,
				      !user_معلومات ? خالي_اشارو : user_معلومات->get_budget_بچيل); }


  /* Internal specializations. */

  فراغ ڌڪو_font_transform (فراغ *paint_معلومات,
                            ثابت hb_font_t *font)
  {
    عائم upem = font->face->get_upem ();
    صحيح xscale = font->x_scale, yscale = font->y_scale;

    ڌڪو_transform (paint_معلومات,
		    xscale/upem, 0,
		    0, yscale/upem,
		    0, 0);
  }

  فراغ ڌڪو_inverse_font_transform (فراغ *paint_معلومات,
                                    ثابت hb_font_t *font)
  {
    عائم upem = font->face->get_upem ();
    صحيح xscale = font->x_scale ? font->x_scale : upem;
    صحيح yscale = font->y_scale ? font->y_scale : upem;

    ڌڪو_transform (paint_معلومات,
		    upem/xscale, 0,
		    0, upem/yscale,
		    0, 0);
  }

  فراغ ڌڪو_transform (فراغ *paint_معلومات, hb_transform_t<عائم> t)
  {
    ڌڪو_transform (paint_معلومات, t.xx, t.yx, t.xy, t.yy, t.x0, t.y0);
  }

  فراغ ڌڪو_translate (فراغ *paint_معلومات,
                       عائم dx, عائم dy)
  {
    ڌڪو_transform (paint_معلومات,
		    hb_transform_t<عائم>::translation (dx, dy));
  }

  فراغ ڌڪو_scale (فراغ *paint_معلومات,
                   عائم sx, عائم sy)
  {
    ڌڪو_transform (paint_معلومات,
		    hb_transform_t<عائم>::scaling (sx, sy));
  }
  فراغ ڌڪو_scale_around_center (فراغ *paint_معلومات,
				 عائم sx, عائم sy,
				 عائم cx, عائم cy)
  {
    ڌڪو_transform (paint_معلومات,
		    hb_transform_t<عائم>::scaling_around_center (sx, sy, cx, cy));
  }

  فراغ ڌڪو_rotate (فراغ *paint_معلومات,
                    عائم a)
  {
    ڌڪو_transform (paint_معلومات,
		    hb_transform_t<عائم>::rotation (a * HB_PI));
  }

  فراغ ڌڪو_rotate_around_center (فراغ *paint_معلومات,
				  عائم a,
				  عائم cx, عائم cy)
  {
    ڌڪو_transform (paint_معلومات,
		    hb_transform_t<عائم>::rotation_around_center (a * HB_PI, cx, cy));
  }

  فراغ ڌڪو_skew (فراغ *paint_معلومات,
                  عائم sx, عائم sy)
  {
    ڌڪو_transform (paint_معلومات,
		    hb_transform_t<عائم>::skewing (-sx * HB_PI, sy * HB_PI));
  }
  فراغ ڌڪو_skew_around_center (فراغ *paint_معلومات,
				عائم sx, عائم sy,
				عائم cx, عائم cy)
  {
    ڌڪو_transform (paint_معلومات,
		    hb_transform_t<عائم>::skewing_around_center (-sx * HB_PI, sy * HB_PI, cx, cy));
  }
};
DECLARE_NULL_INSTANCE (hb_paint_funcs_t);


/* Linearly interpolate between two hb_color_t values, component-wise,
 * in byte-channel space with rounding. */
ساكن مضمن hb_color_t
hb_color_lerp (hb_color_t c0, hb_color_t c1, عائم t)
{
  خودڪار lerp = [&] (بنانشان سرڪايو) -> بنانشان {
    بنانشان v0 = (c0 >> سرڪايو) & 0xFF;
    بنانشان v1 = (c1 >> سرڪايو) & 0xFF;
    عائم v = v0 + t * ((عائم) v1 - (عائم) v0) + 0.5f;
    /* t may be outside [0,1] (or NaN) for malformed gradients;
     * keep the float-to-unsigned conversion defined. */
    إذا (!(v > 0.f)) أرجع 0;
    إذا (v > 255.f) أرجع 255;
    أرجع (بنانشان) v;
  };
  أرجع HB_COLOR (lerp (0), lerp (8), lerp (16), lerp (24));
}

#جيڪڏهن_پورو /* HB_PAINT_HH */

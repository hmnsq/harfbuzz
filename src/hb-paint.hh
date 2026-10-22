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

#ifndef HB_PAINT_HH
#define HB_PAINT_HH

#ضمن "hb.hh"
#ضمن "hb-face.hh"
#ضمن "hb-font.hh"
#ضمن "hb-geometry.hh"

#define HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS \
  HB_PAINT_FUNC_IMPLEMENT (تور_transform) \
  HB_PAINT_FUNC_IMPLEMENT (چرو_transform) \
  HB_PAINT_FUNC_IMPLEMENT (color_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (تور_clip_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (تور_clip_rectangle) \
  HB_PAINT_FUNC_IMPLEMENT (تور_clip_path_start) \
  HB_PAINT_FUNC_IMPLEMENT (تور_clip_path_end) \
  HB_PAINT_FUNC_IMPLEMENT (چرو_clip) \
  HB_PAINT_FUNC_IMPLEMENT (color) \
  HB_PAINT_FUNC_IMPLEMENT (image) \
  HB_PAINT_FUNC_IMPLEMENT (linear_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (radial_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (sweep_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (تور_group) \
  HB_PAINT_FUNC_IMPLEMENT (تور_group_for) \
  HB_PAINT_FUNC_IMPLEMENT (چرو_group) \
  HB_PAINT_FUNC_IMPLEMENT (custom_palette_color) \
  HB_PAINT_FUNC_IMPLEMENT (fill_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (set_budget) \
  HB_PAINT_FUNC_IMPLEMENT (get_budget) \
  HB_PAINT_FUNC_IMPLEMENT (get_budget_remaining) \
  /* ^--- Add new callbacks here */

بنية hb_paint_funcs_t
{
  hb_object_header_t header;

  بنية {
#define HB_PAINT_FUNC_IMPLEMENT(name) hb_paint_##name##_func_t name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_PAINT_FUNC_IMPLEMENT
  } func;

  بنية {
#define HB_PAINT_FUNC_IMPLEMENT(name) فراغ *name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_PAINT_FUNC_IMPLEMENT
  } *user_data;

  بنية {
#define HB_PAINT_FUNC_IMPLEMENT(name) hb_destroy_func_t name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_PAINT_FUNC_IMPLEMENT
  } *destroy;

  فراغ تور_transform (فراغ *paint_data,
                       عائم xx, عائم yx,
                       عائم xy, عائم yy,
                       عائم dx, عائم dy)
  {
    // Handle -0.f to avoid -0.f == 0.f in the transform matrix.
    إذا (dx == -0.f) dx = 0.f;
    إذا (dy == -0.f) dy = 0.f;
    func.تور_transform (هذا, paint_data,
                         xx, yx, xy, yy, dx, dy,
                         !user_data ? nullptr : user_data->تور_transform); }
  فراغ چرو_transform (فراغ *paint_data)
  { func.چرو_transform (هذا, paint_data,
                        !user_data ? nullptr : user_data->چرو_transform); }
  منطقي color_glyph (فراغ *paint_data,
                    hb_codepoint_t glyph,
                    hb_font_t *font)
  { أرجع func.color_glyph (هذا, paint_data,
                             glyph,
                             font,
                             !user_data ? nullptr : user_data->color_glyph); }
  فراغ تور_clip_glyph (فراغ *paint_data,
                        hb_codepoint_t glyph,
                        hb_font_t *font)
  { func.تور_clip_glyph (هذا, paint_data,
                          glyph,
                          font,
                          !user_data ? nullptr : user_data->تور_clip_glyph); }
  فراغ تور_clip_rectangle (فراغ *paint_data,
                           عائم xmin, عائم ymin, عائم xmax, عائم ymax)
  { func.تور_clip_rectangle (هذا, paint_data,
                              xmin, ymin, xmax, ymax,
                              !user_data ? nullptr : user_data->تور_clip_rectangle); }
  hb_draw_funcs_t *تور_clip_path_start (فراغ *paint_data,
                                         فراغ **draw_data)
  { أرجع func.تور_clip_path_start (هذا, paint_data, draw_data,
                                      !user_data ? nullptr : user_data->تور_clip_path_start); }
  فراغ تور_clip_path_end (فراغ *paint_data)
  { func.تور_clip_path_end (هذا, paint_data,
                             !user_data ? nullptr : user_data->تور_clip_path_end); }
  فراغ چرو_clip (فراغ *paint_data)
  { func.چرو_clip (هذا, paint_data,
                   !user_data ? nullptr : user_data->چرو_clip); }
  فراغ color (فراغ *paint_data,
              hb_bool_t is_foreground,
              hb_color_t color)
  { func.color (هذا, paint_data,
                is_foreground, color,
                !user_data ? nullptr : user_data->color); }
  فراغ fill_glyph (فراغ *paint_data,
                   hb_codepoint_t glyph,
                   hb_font_t *font,
                   hb_bool_t is_foreground,
                   hb_color_t color)
  { func.fill_glyph (هذا, paint_data,
                     glyph, font,
                     is_foreground, color,
                     !user_data ? nullptr : user_data->fill_glyph); }
  منطقي image (فراغ *paint_data,
              hb_blob_t *image,
              unsigned width, unsigned height,
              hb_tag_t format,
              عائم slant,
              hb_glyph_extents_t *extents)
  { أرجع func.image (هذا, paint_data,
                       image, width, height, format, slant, extents,
                       !user_data ? nullptr : user_data->image); }
  فراغ linear_gradient (فراغ *paint_data,
                        hb_color_لي_t *color_لي,
                        عائم x0, عائم y0,
                        عائم x1, عائم y1,
                        عائم x2, عائم y2)
  { func.linear_gradient (هذا, paint_data,
                          color_لي, x0, y0, x1, y1, x2, y2,
                          !user_data ? nullptr : user_data->linear_gradient); }
  فراغ radial_gradient (فراغ *paint_data,
                        hb_color_لي_t *color_لي,
                        عائم x0, عائم y0, عائم r0,
                        عائم x1, عائم y1, عائم r1)
  { func.radial_gradient (هذا, paint_data,
                          color_لي, x0, y0, r0, x1, y1, r1,
                          !user_data ? nullptr : user_data->radial_gradient); }
  فراغ sweep_gradient (فراغ *paint_data,
                       hb_color_لي_t *color_لي,
                       عائم x0, عائم y0,
                       عائم start_angle,
                       عائم end_angle)
  { func.sweep_gradient (هذا, paint_data,
                         color_لي, x0, y0, start_angle, end_angle,
                         !user_data ? nullptr : user_data->sweep_gradient); }
  فراغ تور_group (فراغ *paint_data)
  { func.تور_group (هذا, paint_data,
                     !user_data ? nullptr : user_data->تور_group); }
  فراغ تور_group_for (فراغ *paint_data,
                       hb_paint_composite_mode_t mode)
  { func.تور_group_for (هذا, paint_data,
                         mode,
                         !user_data ? nullptr : user_data->تور_group_for); }
  فراغ چرو_group (فراغ *paint_data,
                  hb_paint_composite_mode_t mode)
  { func.چرو_group (هذا, paint_data,
                    mode,
                    !user_data ? nullptr : user_data->چرو_group); }
  منطقي custom_palette_color (فراغ *paint_data,
                             unsigned صحيح color_index,
                             hb_color_t *color)
  { أرجع func.custom_palette_color (هذا, paint_data,
                                      color_index,
                                      color,
                                      !user_data ? nullptr : user_data->custom_palette_color); }
  منطقي set_budget (فراغ *paint_data, int64_t budget)
  {
    إذا (budget < 0 && budget != HB_BUDGET_DEFAULT)
      budget = 0;
    أرجع func.set_budget (هذا, paint_data, budget,
			    !user_data ? nullptr : user_data->set_budget);
  }
  int64_t get_budget (فراغ *paint_data)
  { أرجع func.get_budget (هذا, paint_data,
			    !user_data ? nullptr : user_data->get_budget); }
  int64_t *get_budget_remaining_ptr (فراغ *paint_data)
  { أرجع func.get_budget_remaining (هذا, paint_data,
				      !user_data ? nullptr : user_data->get_budget_remaining); }


  /* Internal specializations. */

  فراغ تور_font_transform (فراغ *paint_data,
                            ثابت hb_font_t *font)
  {
    عائم upem = font->face->get_upem ();
    صحيح xscale = font->x_scale, yscale = font->y_scale;

    تور_transform (paint_data,
		    xscale/upem, 0,
		    0, yscale/upem,
		    0, 0);
  }

  فراغ تور_inverse_font_transform (فراغ *paint_data,
                                    ثابت hb_font_t *font)
  {
    عائم upem = font->face->get_upem ();
    صحيح xscale = font->x_scale ? font->x_scale : upem;
    صحيح yscale = font->y_scale ? font->y_scale : upem;

    تور_transform (paint_data,
		    upem/xscale, 0,
		    0, upem/yscale,
		    0, 0);
  }

  فراغ تور_transform (فراغ *paint_data, hb_transform_t<عائم> t)
  {
    تور_transform (paint_data, t.xx, t.yx, t.xy, t.yy, t.x0, t.y0);
  }

  فراغ تور_translate (فراغ *paint_data,
                       عائم dx, عائم dy)
  {
    تور_transform (paint_data,
		    hb_transform_t<عائم>::translation (dx, dy));
  }

  فراغ تور_scale (فراغ *paint_data,
                   عائم sx, عائم sy)
  {
    تور_transform (paint_data,
		    hb_transform_t<عائم>::scaling (sx, sy));
  }
  فراغ تور_scale_around_center (فراغ *paint_data,
				 عائم sx, عائم sy,
				 عائم cx, عائم cy)
  {
    تور_transform (paint_data,
		    hb_transform_t<عائم>::scaling_around_center (sx, sy, cx, cy));
  }

  فراغ تور_rotate (فراغ *paint_data,
                    عائم a)
  {
    تور_transform (paint_data,
		    hb_transform_t<عائم>::rotation (a * HB_PI));
  }

  فراغ تور_rotate_around_center (فراغ *paint_data,
				  عائم a,
				  عائم cx, عائم cy)
  {
    تور_transform (paint_data,
		    hb_transform_t<عائم>::rotation_around_center (a * HB_PI, cx, cy));
  }

  فراغ تور_skew (فراغ *paint_data,
                  عائم sx, عائم sy)
  {
    تور_transform (paint_data,
		    hb_transform_t<عائم>::skewing (-sx * HB_PI, sy * HB_PI));
  }
  فراغ تور_skew_around_center (فراغ *paint_data,
				عائم sx, عائم sy,
				عائم cx, عائم cy)
  {
    تور_transform (paint_data,
		    hb_transform_t<عائم>::skewing_around_center (-sx * HB_PI, sy * HB_PI, cx, cy));
  }
};
DECLARE_NULL_INSTANCE (hb_paint_funcs_t);


/* Linearly interpolate between two hb_color_t values, component-wise,
 * in byte-channel space with rounding. */
ساكن مضمن hb_color_t
hb_color_lerp (hb_color_t c0, hb_color_t c1, عائم t)
{
  auto lerp = [&] (unsigned shift) -> unsigned {
    unsigned v0 = (c0 >> shift) & 0xFF;
    unsigned v1 = (c1 >> shift) & 0xFF;
    عائم v = v0 + t * ((عائم) v1 - (عائم) v0) + 0.5f;
    /* t may be outside [0,1] (or NaN) for malformed gradients;
     * keep the float-to-unsigned conversion defined. */
    إذا (!(v > 0.f)) أرجع 0;
    إذا (v > 255.f) أرجع 255;
    أرجع (unsigned) v;
  };
  أرجع HB_COLOR (lerp (0), lerp (8), lerp (16), lerp (24));
}

#endif /* HB_PAINT_HH */

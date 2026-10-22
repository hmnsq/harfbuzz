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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-face.hh"
#अन्तर्भाव "hb-font.hh"
#अन्तर्भाव "hb-geometry.hh"

#define HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS \
  HB_PAINT_FUNC_IMPLEMENT (थप_transform) \
  HB_PAINT_FUNC_IMPLEMENT (झिक_transform) \
  HB_PAINT_FUNC_IMPLEMENT (color_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (थप_clip_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (थप_clip_rectangle) \
  HB_PAINT_FUNC_IMPLEMENT (थप_clip_path_start) \
  HB_PAINT_FUNC_IMPLEMENT (थप_clip_path_end) \
  HB_PAINT_FUNC_IMPLEMENT (झिक_clip) \
  HB_PAINT_FUNC_IMPLEMENT (color) \
  HB_PAINT_FUNC_IMPLEMENT (image) \
  HB_PAINT_FUNC_IMPLEMENT (linear_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (radial_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (sweep_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (थप_group) \
  HB_PAINT_FUNC_IMPLEMENT (थप_group_for) \
  HB_PAINT_FUNC_IMPLEMENT (झिक_group) \
  HB_PAINT_FUNC_IMPLEMENT (custom_palette_color) \
  HB_PAINT_FUNC_IMPLEMENT (fill_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (set_budget) \
  HB_PAINT_FUNC_IMPLEMENT (get_budget) \
  HB_PAINT_FUNC_IMPLEMENT (get_budget_remaining) \
  /* ^--- Add new callbacks here */

संरचना hb_paint_funcs_t
{
  hb_object_header_t header;

  संरचना {
#define HB_PAINT_FUNC_IMPLEMENT(name) hb_paint_##name##_func_t name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_PAINT_FUNC_IMPLEMENT
  } func;

  संरचना {
#define HB_PAINT_FUNC_IMPLEMENT(name) शून्य *name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_PAINT_FUNC_IMPLEMENT
  } *user_data;

  संरचना {
#define HB_PAINT_FUNC_IMPLEMENT(name) hb_destroy_func_t name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_PAINT_FUNC_IMPLEMENT
  } *destroy;

  शून्य थप_transform (शून्य *paint_data,
                       भिन्न xx, भिन्न yx,
                       भिन्न xy, भिन्न yy,
                       भिन्न dx, भिन्न dy)
  {
    // Handle -0.f to avoid -0.f == 0.f in the transform matrix.
    यदि (dx == -0.f) dx = 0.f;
    यदि (dy == -0.f) dy = 0.f;
    func.थप_transform (एतत्, paint_data,
                         xx, yx, xy, yy, dx, dy,
                         !user_data ? nullptr : user_data->थप_transform); }
  शून्य झिक_transform (शून्य *paint_data)
  { func.झिक_transform (एतत्, paint_data,
                        !user_data ? nullptr : user_data->झिक_transform); }
  तार्किक color_glyph (शून्य *paint_data,
                    hb_codepoint_t glyph,
                    hb_font_t *font)
  { फर्काऊ func.color_glyph (एतत्, paint_data,
                             glyph,
                             font,
                             !user_data ? nullptr : user_data->color_glyph); }
  शून्य थप_clip_glyph (शून्य *paint_data,
                        hb_codepoint_t glyph,
                        hb_font_t *font)
  { func.थप_clip_glyph (एतत्, paint_data,
                          glyph,
                          font,
                          !user_data ? nullptr : user_data->थप_clip_glyph); }
  शून्य थप_clip_rectangle (शून्य *paint_data,
                           भिन्न xmin, भिन्न ymin, भिन्न xmax, भिन्न ymax)
  { func.थप_clip_rectangle (एतत्, paint_data,
                              xmin, ymin, xmax, ymax,
                              !user_data ? nullptr : user_data->थप_clip_rectangle); }
  hb_draw_funcs_t *थप_clip_path_start (शून्य *paint_data,
                                         शून्य **draw_data)
  { फर्काऊ func.थप_clip_path_start (एतत्, paint_data, draw_data,
                                      !user_data ? nullptr : user_data->थप_clip_path_start); }
  शून्य थप_clip_path_end (शून्य *paint_data)
  { func.थप_clip_path_end (एतत्, paint_data,
                             !user_data ? nullptr : user_data->थप_clip_path_end); }
  शून्य झिक_clip (शून्य *paint_data)
  { func.झिक_clip (एतत्, paint_data,
                   !user_data ? nullptr : user_data->झिक_clip); }
  शून्य color (शून्य *paint_data,
              hb_bool_t is_foreground,
              hb_color_t color)
  { func.color (एतत्, paint_data,
                is_foreground, color,
                !user_data ? nullptr : user_data->color); }
  शून्य fill_glyph (शून्य *paint_data,
                   hb_codepoint_t glyph,
                   hb_font_t *font,
                   hb_bool_t is_foreground,
                   hb_color_t color)
  { func.fill_glyph (एतत्, paint_data,
                     glyph, font,
                     is_foreground, color,
                     !user_data ? nullptr : user_data->fill_glyph); }
  तार्किक image (शून्य *paint_data,
              hb_blob_t *image,
              अचिन्हित width, अचिन्हित height,
              hb_tag_t format,
              भिन्न slant,
              hb_glyph_extents_t *extents)
  { फर्काऊ func.image (एतत्, paint_data,
                       image, width, height, format, slant, extents,
                       !user_data ? nullptr : user_data->image); }
  शून्य linear_gradient (शून्य *paint_data,
                        hb_color_पङ्क्ति_t *color_पङ्क्ति,
                        भिन्न x0, भिन्न y0,
                        भिन्न x1, भिन्न y1,
                        भिन्न x2, भिन्न y2)
  { func.linear_gradient (एतत्, paint_data,
                          color_पङ्क्ति, x0, y0, x1, y1, x2, y2,
                          !user_data ? nullptr : user_data->linear_gradient); }
  शून्य radial_gradient (शून्य *paint_data,
                        hb_color_पङ्क्ति_t *color_पङ्क्ति,
                        भिन्न x0, भिन्न y0, भिन्न r0,
                        भिन्न x1, भिन्न y1, भिन्न r1)
  { func.radial_gradient (एतत्, paint_data,
                          color_पङ्क्ति, x0, y0, r0, x1, y1, r1,
                          !user_data ? nullptr : user_data->radial_gradient); }
  शून्य sweep_gradient (शून्य *paint_data,
                       hb_color_पङ्क्ति_t *color_पङ्क्ति,
                       भिन्न x0, भिन्न y0,
                       भिन्न start_angle,
                       भिन्न end_angle)
  { func.sweep_gradient (एतत्, paint_data,
                         color_पङ्क्ति, x0, y0, start_angle, end_angle,
                         !user_data ? nullptr : user_data->sweep_gradient); }
  शून्य थप_group (शून्य *paint_data)
  { func.थप_group (एतत्, paint_data,
                     !user_data ? nullptr : user_data->थप_group); }
  शून्य थप_group_for (शून्य *paint_data,
                       hb_paint_composite_mode_t mode)
  { func.थप_group_for (एतत्, paint_data,
                         mode,
                         !user_data ? nullptr : user_data->थप_group_for); }
  शून्य झिक_group (शून्य *paint_data,
                  hb_paint_composite_mode_t mode)
  { func.झिक_group (एतत्, paint_data,
                    mode,
                    !user_data ? nullptr : user_data->झिक_group); }
  तार्किक custom_palette_color (शून्य *paint_data,
                             अचिन्हित पूर्णाङ्क color_index,
                             hb_color_t *color)
  { फर्काऊ func.custom_palette_color (एतत्, paint_data,
                                      color_index,
                                      color,
                                      !user_data ? nullptr : user_data->custom_palette_color); }
  तार्किक set_budget (शून्य *paint_data, int64_t budget)
  {
    यदि (budget < 0 && budget != HB_BUDGET_DEFAULT)
      budget = 0;
    फर्काऊ func.set_budget (एतत्, paint_data, budget,
			    !user_data ? nullptr : user_data->set_budget);
  }
  int64_t get_budget (शून्य *paint_data)
  { फर्काऊ func.get_budget (एतत्, paint_data,
			    !user_data ? nullptr : user_data->get_budget); }
  int64_t *get_budget_remaining_ptr (शून्य *paint_data)
  { फर्काऊ func.get_budget_remaining (एतत्, paint_data,
				      !user_data ? nullptr : user_data->get_budget_remaining); }


  /* Internal specializations. */

  शून्य थप_font_transform (शून्य *paint_data,
                            अचर hb_font_t *font)
  {
    भिन्न upem = font->face->get_upem ();
    पूर्णाङ्क xscale = font->x_scale, yscale = font->y_scale;

    थप_transform (paint_data,
		    xscale/upem, 0,
		    0, yscale/upem,
		    0, 0);
  }

  शून्य थप_inverse_font_transform (शून्य *paint_data,
                                    अचर hb_font_t *font)
  {
    भिन्न upem = font->face->get_upem ();
    पूर्णाङ्क xscale = font->x_scale ? font->x_scale : upem;
    पूर्णाङ्क yscale = font->y_scale ? font->y_scale : upem;

    थप_transform (paint_data,
		    upem/xscale, 0,
		    0, upem/yscale,
		    0, 0);
  }

  शून्य थप_transform (शून्य *paint_data, hb_transform_t<भिन्न> t)
  {
    थप_transform (paint_data, t.xx, t.yx, t.xy, t.yy, t.x0, t.y0);
  }

  शून्य थप_translate (शून्य *paint_data,
                       भिन्न dx, भिन्न dy)
  {
    थप_transform (paint_data,
		    hb_transform_t<भिन्न>::translation (dx, dy));
  }

  शून्य थप_scale (शून्य *paint_data,
                   भिन्न sx, भिन्न sy)
  {
    थप_transform (paint_data,
		    hb_transform_t<भिन्न>::scaling (sx, sy));
  }
  शून्य थप_scale_around_center (शून्य *paint_data,
				 भिन्न sx, भिन्न sy,
				 भिन्न cx, भिन्न cy)
  {
    थप_transform (paint_data,
		    hb_transform_t<भिन्न>::scaling_around_center (sx, sy, cx, cy));
  }

  शून्य थप_rotate (शून्य *paint_data,
                    भिन्न a)
  {
    थप_transform (paint_data,
		    hb_transform_t<भिन्न>::rotation (a * HB_PI));
  }

  शून्य थप_rotate_around_center (शून्य *paint_data,
				  भिन्न a,
				  भिन्न cx, भिन्न cy)
  {
    थप_transform (paint_data,
		    hb_transform_t<भिन्न>::rotation_around_center (a * HB_PI, cx, cy));
  }

  शून्य थप_skew (शून्य *paint_data,
                  भिन्न sx, भिन्न sy)
  {
    थप_transform (paint_data,
		    hb_transform_t<भिन्न>::skewing (-sx * HB_PI, sy * HB_PI));
  }
  शून्य थप_skew_around_center (शून्य *paint_data,
				भिन्न sx, भिन्न sy,
				भिन्न cx, भिन्न cy)
  {
    थप_transform (paint_data,
		    hb_transform_t<भिन्न>::skewing_around_center (-sx * HB_PI, sy * HB_PI, cx, cy));
  }
};
DECLARE_NULL_INSTANCE (hb_paint_funcs_t);


/* Linearly interpolate between two hb_color_t values, component-wise,
 * in byte-channel space with rounding. */
स्थिर अन्तःस्थ hb_color_t
hb_color_lerp (hb_color_t c0, hb_color_t c1, भिन्न t)
{
  स्वतः lerp = [&] (अचिन्हित shift) -> अचिन्हित {
    अचिन्हित v0 = (c0 >> shift) & 0xFF;
    अचिन्हित v1 = (c1 >> shift) & 0xFF;
    भिन्न v = v0 + t * ((भिन्न) v1 - (भिन्न) v0) + 0.5f;
    /* t may be outside [0,1] (or NaN) for malformed gradients;
     * keep the float-to-unsigned conversion defined. */
    यदि (!(v > 0.f)) फर्काऊ 0;
    यदि (v > 255.f) फर्काऊ 255;
    फर्काऊ (अचिन्हित) v;
  };
  फर्काऊ HB_COLOR (lerp (0), lerp (8), lerp (16), lerp (24));
}

#endif /* HB_PAINT_HH */

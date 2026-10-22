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

#شامل "hb.hh"
#شامل "hb-face.hh"
#شامل "hb-font.hh"
#شامل "hb-geometry.hh"

#define HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS \
  HB_PAINT_FUNC_IMPLEMENT (افزودن_transform) \
  HB_PAINT_FUNC_IMPLEMENT (برداشت_transform) \
  HB_PAINT_FUNC_IMPLEMENT (color_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (افزودن_clip_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (افزودن_clip_rectangle) \
  HB_PAINT_FUNC_IMPLEMENT (افزودن_clip_path_start) \
  HB_PAINT_FUNC_IMPLEMENT (افزودن_clip_path_end) \
  HB_PAINT_FUNC_IMPLEMENT (برداشت_clip) \
  HB_PAINT_FUNC_IMPLEMENT (color) \
  HB_PAINT_FUNC_IMPLEMENT (image) \
  HB_PAINT_FUNC_IMPLEMENT (linear_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (radial_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (sweep_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (افزودن_group) \
  HB_PAINT_FUNC_IMPLEMENT (افزودن_group_for) \
  HB_PAINT_FUNC_IMPLEMENT (برداشت_group) \
  HB_PAINT_FUNC_IMPLEMENT (custom_palette_color) \
  HB_PAINT_FUNC_IMPLEMENT (fill_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (set_budget) \
  HB_PAINT_FUNC_IMPLEMENT (get_budget) \
  HB_PAINT_FUNC_IMPLEMENT (get_budget_remaining) \
  /* ^--- Add new callbacks here */

ساختار hb_paint_funcs_t
{
  hb_object_header_t header;

  ساختار {
#define HB_PAINT_FUNC_IMPLEMENT(name) hb_paint_##name##_func_t name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_PAINT_FUNC_IMPLEMENT
  } func;

  ساختار {
#define HB_PAINT_FUNC_IMPLEMENT(name) تهی *name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_PAINT_FUNC_IMPLEMENT
  } *user_data;

  ساختار {
#define HB_PAINT_FUNC_IMPLEMENT(name) hb_destroy_func_t name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_PAINT_FUNC_IMPLEMENT
  } *destroy;

  تهی افزودن_transform (تهی *paint_data,
                       اعشاری xx, اعشاری yx,
                       اعشاری xy, اعشاری yy,
                       اعشاری dx, اعشاری dy)
  {
    // Handle -0.f to avoid -0.f == 0.f in the transform matrix.
    اگر (dx == -0.f) dx = 0.f;
    اگر (dy == -0.f) dy = 0.f;
    func.افزودن_transform (این, paint_data,
                         xx, yx, xy, yy, dx, dy,
                         !user_data ? nullptr : user_data->افزودن_transform); }
  تهی برداشت_transform (تهی *paint_data)
  { func.برداشت_transform (این, paint_data,
                        !user_data ? nullptr : user_data->برداشت_transform); }
  منطقی color_glyph (تهی *paint_data,
                    hb_codepoint_t glyph,
                    hb_font_t *font)
  { بازگشت func.color_glyph (این, paint_data,
                             glyph,
                             font,
                             !user_data ? nullptr : user_data->color_glyph); }
  تهی افزودن_clip_glyph (تهی *paint_data,
                        hb_codepoint_t glyph,
                        hb_font_t *font)
  { func.افزودن_clip_glyph (این, paint_data,
                          glyph,
                          font,
                          !user_data ? nullptr : user_data->افزودن_clip_glyph); }
  تهی افزودن_clip_rectangle (تهی *paint_data,
                           اعشاری xmin, اعشاری ymin, اعشاری xmax, اعشاری ymax)
  { func.افزودن_clip_rectangle (این, paint_data,
                              xmin, ymin, xmax, ymax,
                              !user_data ? nullptr : user_data->افزودن_clip_rectangle); }
  hb_draw_funcs_t *افزودن_clip_path_start (تهی *paint_data,
                                         تهی **draw_data)
  { بازگشت func.افزودن_clip_path_start (این, paint_data, draw_data,
                                      !user_data ? nullptr : user_data->افزودن_clip_path_start); }
  تهی افزودن_clip_path_end (تهی *paint_data)
  { func.افزودن_clip_path_end (این, paint_data,
                             !user_data ? nullptr : user_data->افزودن_clip_path_end); }
  تهی برداشت_clip (تهی *paint_data)
  { func.برداشت_clip (این, paint_data,
                   !user_data ? nullptr : user_data->برداشت_clip); }
  تهی color (تهی *paint_data,
              hb_bool_t is_foreground,
              hb_color_t color)
  { func.color (این, paint_data,
                is_foreground, color,
                !user_data ? nullptr : user_data->color); }
  تهی fill_glyph (تهی *paint_data,
                   hb_codepoint_t glyph,
                   hb_font_t *font,
                   hb_bool_t is_foreground,
                   hb_color_t color)
  { func.fill_glyph (این, paint_data,
                     glyph, font,
                     is_foreground, color,
                     !user_data ? nullptr : user_data->fill_glyph); }
  منطقی image (تهی *paint_data,
              hb_blob_t *image,
              unsigned width, unsigned height,
              hb_tag_t format,
              اعشاری slant,
              hb_glyph_extents_t *extents)
  { بازگشت func.image (این, paint_data,
                       image, width, height, format, slant, extents,
                       !user_data ? nullptr : user_data->image); }
  تهی linear_gradient (تهی *paint_data,
                        hb_color_سطر_t *color_سطر,
                        اعشاری x0, اعشاری y0,
                        اعشاری x1, اعشاری y1,
                        اعشاری x2, اعشاری y2)
  { func.linear_gradient (این, paint_data,
                          color_سطر, x0, y0, x1, y1, x2, y2,
                          !user_data ? nullptr : user_data->linear_gradient); }
  تهی radial_gradient (تهی *paint_data,
                        hb_color_سطر_t *color_سطر,
                        اعشاری x0, اعشاری y0, اعشاری r0,
                        اعشاری x1, اعشاری y1, اعشاری r1)
  { func.radial_gradient (این, paint_data,
                          color_سطر, x0, y0, r0, x1, y1, r1,
                          !user_data ? nullptr : user_data->radial_gradient); }
  تهی sweep_gradient (تهی *paint_data,
                       hb_color_سطر_t *color_سطر,
                       اعشاری x0, اعشاری y0,
                       اعشاری start_angle,
                       اعشاری end_angle)
  { func.sweep_gradient (این, paint_data,
                         color_سطر, x0, y0, start_angle, end_angle,
                         !user_data ? nullptr : user_data->sweep_gradient); }
  تهی افزودن_group (تهی *paint_data)
  { func.افزودن_group (این, paint_data,
                     !user_data ? nullptr : user_data->افزودن_group); }
  تهی افزودن_group_for (تهی *paint_data,
                       hb_paint_composite_mode_t mode)
  { func.افزودن_group_for (این, paint_data,
                         mode,
                         !user_data ? nullptr : user_data->افزودن_group_for); }
  تهی برداشت_group (تهی *paint_data,
                  hb_paint_composite_mode_t mode)
  { func.برداشت_group (این, paint_data,
                    mode,
                    !user_data ? nullptr : user_data->برداشت_group); }
  منطقی custom_palette_color (تهی *paint_data,
                             unsigned صحیح color_index,
                             hb_color_t *color)
  { بازگشت func.custom_palette_color (این, paint_data,
                                      color_index,
                                      color,
                                      !user_data ? nullptr : user_data->custom_palette_color); }
  منطقی set_budget (تهی *paint_data, int64_t budget)
  {
    اگر (budget < 0 && budget != HB_BUDGET_DEFAULT)
      budget = 0;
    بازگشت func.set_budget (این, paint_data, budget,
			    !user_data ? nullptr : user_data->set_budget);
  }
  int64_t get_budget (تهی *paint_data)
  { بازگشت func.get_budget (این, paint_data,
			    !user_data ? nullptr : user_data->get_budget); }
  int64_t *get_budget_remaining_ptr (تهی *paint_data)
  { بازگشت func.get_budget_remaining (این, paint_data,
				      !user_data ? nullptr : user_data->get_budget_remaining); }


  /* Internal specializations. */

  تهی افزودن_font_transform (تهی *paint_data,
                            ثابت hb_font_t *font)
  {
    اعشاری upem = font->face->get_upem ();
    صحیح xscale = font->x_scale, yscale = font->y_scale;

    افزودن_transform (paint_data,
		    xscale/upem, 0,
		    0, yscale/upem,
		    0, 0);
  }

  تهی افزودن_inverse_font_transform (تهی *paint_data,
                                    ثابت hb_font_t *font)
  {
    اعشاری upem = font->face->get_upem ();
    صحیح xscale = font->x_scale ? font->x_scale : upem;
    صحیح yscale = font->y_scale ? font->y_scale : upem;

    افزودن_transform (paint_data,
		    upem/xscale, 0,
		    0, upem/yscale,
		    0, 0);
  }

  تهی افزودن_transform (تهی *paint_data, hb_transform_t<اعشاری> t)
  {
    افزودن_transform (paint_data, t.xx, t.yx, t.xy, t.yy, t.x0, t.y0);
  }

  تهی افزودن_translate (تهی *paint_data,
                       اعشاری dx, اعشاری dy)
  {
    افزودن_transform (paint_data,
		    hb_transform_t<اعشاری>::translation (dx, dy));
  }

  تهی افزودن_scale (تهی *paint_data,
                   اعشاری sx, اعشاری sy)
  {
    افزودن_transform (paint_data,
		    hb_transform_t<اعشاری>::scaling (sx, sy));
  }
  تهی افزودن_scale_around_center (تهی *paint_data,
				 اعشاری sx, اعشاری sy,
				 اعشاری cx, اعشاری cy)
  {
    افزودن_transform (paint_data,
		    hb_transform_t<اعشاری>::scaling_around_center (sx, sy, cx, cy));
  }

  تهی افزودن_rotate (تهی *paint_data,
                    اعشاری a)
  {
    افزودن_transform (paint_data,
		    hb_transform_t<اعشاری>::rotation (a * HB_PI));
  }

  تهی افزودن_rotate_around_center (تهی *paint_data,
				  اعشاری a,
				  اعشاری cx, اعشاری cy)
  {
    افزودن_transform (paint_data,
		    hb_transform_t<اعشاری>::rotation_around_center (a * HB_PI, cx, cy));
  }

  تهی افزودن_skew (تهی *paint_data,
                  اعشاری sx, اعشاری sy)
  {
    افزودن_transform (paint_data,
		    hb_transform_t<اعشاری>::skewing (-sx * HB_PI, sy * HB_PI));
  }
  تهی افزودن_skew_around_center (تهی *paint_data,
				اعشاری sx, اعشاری sy,
				اعشاری cx, اعشاری cy)
  {
    افزودن_transform (paint_data,
		    hb_transform_t<اعشاری>::skewing_around_center (-sx * HB_PI, sy * HB_PI, cx, cy));
  }
};
DECLARE_NULL_INSTANCE (hb_paint_funcs_t);


/* Linearly interpolate between two hb_color_t values, component-wise,
 * in byte-channel space with rounding. */
ایستا درونخطی hb_color_t
hb_color_lerp (hb_color_t c0, hb_color_t c1, اعشاری t)
{
  auto lerp = [&] (unsigned shift) -> unsigned {
    unsigned v0 = (c0 >> shift) & 0xFF;
    unsigned v1 = (c1 >> shift) & 0xFF;
    اعشاری v = v0 + t * ((اعشاری) v1 - (اعشاری) v0) + 0.5f;
    /* t may be outside [0,1] (or NaN) for malformed gradients;
     * keep the float-to-unsigned conversion defined. */
    اگر (!(v > 0.f)) بازگشت 0;
    اگر (v > 255.f) بازگشت 255;
    بازگشت (unsigned) v;
  };
  بازگشت HB_COLOR (lerp (0), lerp (8), lerp (16), lerp (24));
}

#endif /* HB_PAINT_HH */

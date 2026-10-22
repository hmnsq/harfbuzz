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

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-face.hh"
#ಅನ್ತರ್ಭಾವ "hb-font.hh"
#ಅನ್ತರ್ಭಾವ "hb-geometry.hh"

#define HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS \
  HB_PAINT_FUNC_IMPLEMENT (ತಳ್ಳು_transform) \
  HB_PAINT_FUNC_IMPLEMENT (ತೆಗೆ_transform) \
  HB_PAINT_FUNC_IMPLEMENT (color_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (ತಳ್ಳು_clip_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (ತಳ್ಳು_clip_rectangle) \
  HB_PAINT_FUNC_IMPLEMENT (ತಳ್ಳು_clip_path_start) \
  HB_PAINT_FUNC_IMPLEMENT (ತಳ್ಳು_clip_path_end) \
  HB_PAINT_FUNC_IMPLEMENT (ತೆಗೆ_clip) \
  HB_PAINT_FUNC_IMPLEMENT (color) \
  HB_PAINT_FUNC_IMPLEMENT (image) \
  HB_PAINT_FUNC_IMPLEMENT (linear_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (radial_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (sweep_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (ತಳ್ಳು_group) \
  HB_PAINT_FUNC_IMPLEMENT (ತಳ್ಳು_group_for) \
  HB_PAINT_FUNC_IMPLEMENT (ತೆಗೆ_group) \
  HB_PAINT_FUNC_IMPLEMENT (custom_palette_color) \
  HB_PAINT_FUNC_IMPLEMENT (fill_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (set_budget) \
  HB_PAINT_FUNC_IMPLEMENT (get_budget) \
  HB_PAINT_FUNC_IMPLEMENT (get_budget_remaining) \
  /* ^--- Add new callbacks here */

ಸಂರಚನಾ hb_paint_funcs_t
{
  hb_object_header_t header;

  ಸಂರಚನಾ {
#define HB_PAINT_FUNC_IMPLEMENT(name) hb_paint_##name##_func_t name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_PAINT_FUNC_IMPLEMENT
  } func;

  ಸಂರಚನಾ {
#define HB_PAINT_FUNC_IMPLEMENT(name) ಶೂನ್ಯ *name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_PAINT_FUNC_IMPLEMENT
  } *user_data;

  ಸಂರಚನಾ {
#define HB_PAINT_FUNC_IMPLEMENT(name) hb_destroy_func_t name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_PAINT_FUNC_IMPLEMENT
  } *destroy;

  ಶೂನ್ಯ ತಳ್ಳು_transform (ಶೂನ್ಯ *paint_data,
                       ಭಿನ್ನ xx, ಭಿನ್ನ yx,
                       ಭಿನ್ನ xy, ಭಿನ್ನ yy,
                       ಭಿನ್ನ dx, ಭಿನ್ನ dy)
  {
    // Handle -0.f to avoid -0.f == 0.f in the transform matrix.
    ಯದಿ (dx == -0.f) dx = 0.f;
    ಯದಿ (dy == -0.f) dy = 0.f;
    func.ತಳ್ಳು_transform (ಏತತ್, paint_data,
                         xx, yx, xy, yy, dx, dy,
                         !user_data ? nullptr : user_data->ತಳ್ಳು_transform); }
  ಶೂನ್ಯ ತೆಗೆ_transform (ಶೂನ್ಯ *paint_data)
  { func.ತೆಗೆ_transform (ಏತತ್, paint_data,
                        !user_data ? nullptr : user_data->ತೆಗೆ_transform); }
  ತಾರ್ಕಿಕ color_glyph (ಶೂನ್ಯ *paint_data,
                    hb_codepoint_t glyph,
                    hb_font_t *font)
  { ಪ್ರತ್ಯಾಗಮ func.color_glyph (ಏತತ್, paint_data,
                             glyph,
                             font,
                             !user_data ? nullptr : user_data->color_glyph); }
  ಶೂನ್ಯ ತಳ್ಳು_clip_glyph (ಶೂನ್ಯ *paint_data,
                        hb_codepoint_t glyph,
                        hb_font_t *font)
  { func.ತಳ್ಳು_clip_glyph (ಏತತ್, paint_data,
                          glyph,
                          font,
                          !user_data ? nullptr : user_data->ತಳ್ಳು_clip_glyph); }
  ಶೂನ್ಯ ತಳ್ಳು_clip_rectangle (ಶೂನ್ಯ *paint_data,
                           ಭಿನ್ನ xmin, ಭಿನ್ನ ymin, ಭಿನ್ನ xmax, ಭಿನ್ನ ymax)
  { func.ತಳ್ಳು_clip_rectangle (ಏತತ್, paint_data,
                              xmin, ymin, xmax, ymax,
                              !user_data ? nullptr : user_data->ತಳ್ಳು_clip_rectangle); }
  hb_draw_funcs_t *ತಳ್ಳು_clip_path_start (ಶೂನ್ಯ *paint_data,
                                         ಶೂನ್ಯ **draw_data)
  { ಪ್ರತ್ಯಾಗಮ func.ತಳ್ಳು_clip_path_start (ಏತತ್, paint_data, draw_data,
                                      !user_data ? nullptr : user_data->ತಳ್ಳು_clip_path_start); }
  ಶೂನ್ಯ ತಳ್ಳು_clip_path_end (ಶೂನ್ಯ *paint_data)
  { func.ತಳ್ಳು_clip_path_end (ಏತತ್, paint_data,
                             !user_data ? nullptr : user_data->ತಳ್ಳು_clip_path_end); }
  ಶೂನ್ಯ ತೆಗೆ_clip (ಶೂನ್ಯ *paint_data)
  { func.ತೆಗೆ_clip (ಏತತ್, paint_data,
                   !user_data ? nullptr : user_data->ತೆಗೆ_clip); }
  ಶೂನ್ಯ color (ಶೂನ್ಯ *paint_data,
              hb_bool_t is_foreground,
              hb_color_t color)
  { func.color (ಏತತ್, paint_data,
                is_foreground, color,
                !user_data ? nullptr : user_data->color); }
  ಶೂನ್ಯ fill_glyph (ಶೂನ್ಯ *paint_data,
                   hb_codepoint_t glyph,
                   hb_font_t *font,
                   hb_bool_t is_foreground,
                   hb_color_t color)
  { func.fill_glyph (ಏತತ್, paint_data,
                     glyph, font,
                     is_foreground, color,
                     !user_data ? nullptr : user_data->fill_glyph); }
  ತಾರ್ಕಿಕ image (ಶೂನ್ಯ *paint_data,
              hb_blob_t *image,
              unsigned width, unsigned height,
              hb_tag_t format,
              ಭಿನ್ನ slant,
              hb_glyph_extents_t *extents)
  { ಪ್ರತ್ಯಾಗಮ func.image (ಏತತ್, paint_data,
                       image, width, height, format, slant, extents,
                       !user_data ? nullptr : user_data->image); }
  ಶೂನ್ಯ linear_gradient (ಶೂನ್ಯ *paint_data,
                        hb_color_ಸಾಲು_t *color_ಸಾಲು,
                        ಭಿನ್ನ x0, ಭಿನ್ನ y0,
                        ಭಿನ್ನ x1, ಭಿನ್ನ y1,
                        ಭಿನ್ನ x2, ಭಿನ್ನ y2)
  { func.linear_gradient (ಏತತ್, paint_data,
                          color_ಸಾಲು, x0, y0, x1, y1, x2, y2,
                          !user_data ? nullptr : user_data->linear_gradient); }
  ಶೂನ್ಯ radial_gradient (ಶೂನ್ಯ *paint_data,
                        hb_color_ಸಾಲು_t *color_ಸಾಲು,
                        ಭಿನ್ನ x0, ಭಿನ್ನ y0, ಭಿನ್ನ r0,
                        ಭಿನ್ನ x1, ಭಿನ್ನ y1, ಭಿನ್ನ r1)
  { func.radial_gradient (ಏತತ್, paint_data,
                          color_ಸಾಲು, x0, y0, r0, x1, y1, r1,
                          !user_data ? nullptr : user_data->radial_gradient); }
  ಶೂನ್ಯ sweep_gradient (ಶೂನ್ಯ *paint_data,
                       hb_color_ಸಾಲು_t *color_ಸಾಲು,
                       ಭಿನ್ನ x0, ಭಿನ್ನ y0,
                       ಭಿನ್ನ start_angle,
                       ಭಿನ್ನ end_angle)
  { func.sweep_gradient (ಏತತ್, paint_data,
                         color_ಸಾಲು, x0, y0, start_angle, end_angle,
                         !user_data ? nullptr : user_data->sweep_gradient); }
  ಶೂನ್ಯ ತಳ್ಳು_group (ಶೂನ್ಯ *paint_data)
  { func.ತಳ್ಳು_group (ಏತತ್, paint_data,
                     !user_data ? nullptr : user_data->ತಳ್ಳು_group); }
  ಶೂನ್ಯ ತಳ್ಳು_group_for (ಶೂನ್ಯ *paint_data,
                       hb_paint_composite_mode_t mode)
  { func.ತಳ್ಳು_group_for (ಏತತ್, paint_data,
                         mode,
                         !user_data ? nullptr : user_data->ತಳ್ಳು_group_for); }
  ಶೂನ್ಯ ತೆಗೆ_group (ಶೂನ್ಯ *paint_data,
                  hb_paint_composite_mode_t mode)
  { func.ತೆಗೆ_group (ಏತತ್, paint_data,
                    mode,
                    !user_data ? nullptr : user_data->ತೆಗೆ_group); }
  ತಾರ್ಕಿಕ custom_palette_color (ಶೂನ್ಯ *paint_data,
                             unsigned ಪೂರ್ಣಾಙ್ಕ color_index,
                             hb_color_t *color)
  { ಪ್ರತ್ಯಾಗಮ func.custom_palette_color (ಏತತ್, paint_data,
                                      color_index,
                                      color,
                                      !user_data ? nullptr : user_data->custom_palette_color); }
  ತಾರ್ಕಿಕ set_budget (ಶೂನ್ಯ *paint_data, int64_t budget)
  {
    ಯದಿ (budget < 0 && budget != HB_BUDGET_DEFAULT)
      budget = 0;
    ಪ್ರತ್ಯಾಗಮ func.set_budget (ಏತತ್, paint_data, budget,
			    !user_data ? nullptr : user_data->set_budget);
  }
  int64_t get_budget (ಶೂನ್ಯ *paint_data)
  { ಪ್ರತ್ಯಾಗಮ func.get_budget (ಏತತ್, paint_data,
			    !user_data ? nullptr : user_data->get_budget); }
  int64_t *get_budget_remaining_ptr (ಶೂನ್ಯ *paint_data)
  { ಪ್ರತ್ಯಾಗಮ func.get_budget_remaining (ಏತತ್, paint_data,
				      !user_data ? nullptr : user_data->get_budget_remaining); }


  /* Internal specializations. */

  ಶೂನ್ಯ ತಳ್ಳು_font_transform (ಶೂನ್ಯ *paint_data,
                            ಧ್ರುವ hb_font_t *font)
  {
    ಭಿನ್ನ upem = font->face->get_upem ();
    ಪೂರ್ಣಾಙ್ಕ xscale = font->x_scale, yscale = font->y_scale;

    ತಳ್ಳು_transform (paint_data,
		    xscale/upem, 0,
		    0, yscale/upem,
		    0, 0);
  }

  ಶೂನ್ಯ ತಳ್ಳು_inverse_font_transform (ಶೂನ್ಯ *paint_data,
                                    ಧ್ರುವ hb_font_t *font)
  {
    ಭಿನ್ನ upem = font->face->get_upem ();
    ಪೂರ್ಣಾಙ್ಕ xscale = font->x_scale ? font->x_scale : upem;
    ಪೂರ್ಣಾಙ್ಕ yscale = font->y_scale ? font->y_scale : upem;

    ತಳ್ಳು_transform (paint_data,
		    upem/xscale, 0,
		    0, upem/yscale,
		    0, 0);
  }

  ಶೂನ್ಯ ತಳ್ಳು_transform (ಶೂನ್ಯ *paint_data, hb_transform_t<ಭಿನ್ನ> t)
  {
    ತಳ್ಳು_transform (paint_data, t.xx, t.yx, t.xy, t.yy, t.x0, t.y0);
  }

  ಶೂನ್ಯ ತಳ್ಳು_translate (ಶೂನ್ಯ *paint_data,
                       ಭಿನ್ನ dx, ಭಿನ್ನ dy)
  {
    ತಳ್ಳು_transform (paint_data,
		    hb_transform_t<ಭಿನ್ನ>::translation (dx, dy));
  }

  ಶೂನ್ಯ ತಳ್ಳು_scale (ಶೂನ್ಯ *paint_data,
                   ಭಿನ್ನ sx, ಭಿನ್ನ sy)
  {
    ತಳ್ಳು_transform (paint_data,
		    hb_transform_t<ಭಿನ್ನ>::scaling (sx, sy));
  }
  ಶೂನ್ಯ ತಳ್ಳು_scale_around_center (ಶೂನ್ಯ *paint_data,
				 ಭಿನ್ನ sx, ಭಿನ್ನ sy,
				 ಭಿನ್ನ cx, ಭಿನ್ನ cy)
  {
    ತಳ್ಳು_transform (paint_data,
		    hb_transform_t<ಭಿನ್ನ>::scaling_around_center (sx, sy, cx, cy));
  }

  ಶೂನ್ಯ ತಳ್ಳು_rotate (ಶೂನ್ಯ *paint_data,
                    ಭಿನ್ನ a)
  {
    ತಳ್ಳು_transform (paint_data,
		    hb_transform_t<ಭಿನ್ನ>::rotation (a * HB_PI));
  }

  ಶೂನ್ಯ ತಳ್ಳು_rotate_around_center (ಶೂನ್ಯ *paint_data,
				  ಭಿನ್ನ a,
				  ಭಿನ್ನ cx, ಭಿನ್ನ cy)
  {
    ತಳ್ಳು_transform (paint_data,
		    hb_transform_t<ಭಿನ್ನ>::rotation_around_center (a * HB_PI, cx, cy));
  }

  ಶೂನ್ಯ ತಳ್ಳು_skew (ಶೂನ್ಯ *paint_data,
                  ಭಿನ್ನ sx, ಭಿನ್ನ sy)
  {
    ತಳ್ಳು_transform (paint_data,
		    hb_transform_t<ಭಿನ್ನ>::skewing (-sx * HB_PI, sy * HB_PI));
  }
  ಶೂನ್ಯ ತಳ್ಳು_skew_around_center (ಶೂನ್ಯ *paint_data,
				ಭಿನ್ನ sx, ಭಿನ್ನ sy,
				ಭಿನ್ನ cx, ಭಿನ್ನ cy)
  {
    ತಳ್ಳು_transform (paint_data,
		    hb_transform_t<ಭಿನ್ನ>::skewing_around_center (-sx * HB_PI, sy * HB_PI, cx, cy));
  }
};
DECLARE_NULL_INSTANCE (hb_paint_funcs_t);


/* Linearly interpolate between two hb_color_t values, component-wise,
 * in byte-channel space with rounding. */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ hb_color_t
hb_color_lerp (hb_color_t c0, hb_color_t c1, ಭಿನ್ನ t)
{
  auto lerp = [&] (unsigned shift) -> unsigned {
    unsigned v0 = (c0 >> shift) & 0xFF;
    unsigned v1 = (c1 >> shift) & 0xFF;
    ಭಿನ್ನ v = v0 + t * ((ಭಿನ್ನ) v1 - (ಭಿನ್ನ) v0) + 0.5f;
    /* t may be outside [0,1] (or NaN) for malformed gradients;
     * keep the float-to-unsigned conversion defined. */
    ಯದಿ (!(v > 0.f)) ಪ್ರತ್ಯಾಗಮ 0;
    ಯದಿ (v > 255.f) ಪ್ರತ್ಯಾಗಮ 255;
    ಪ್ರತ್ಯಾಗಮ (unsigned) v;
  };
  ಪ್ರತ್ಯಾಗಮ HB_COLOR (lerp (0), lerp (8), lerp (16), lerp (24));
}

#endif /* HB_PAINT_HH */

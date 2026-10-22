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
  HB_PAINT_FUNC_IMPLEMENT (ढकला_transform) \
  HB_PAINT_FUNC_IMPLEMENT (काढा_transform) \
  HB_PAINT_FUNC_IMPLEMENT (color_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (ढकला_clip_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (ढकला_clip_rectangle) \
  HB_PAINT_FUNC_IMPLEMENT (ढकला_clip_path_start) \
  HB_PAINT_FUNC_IMPLEMENT (ढकला_clip_path_end) \
  HB_PAINT_FUNC_IMPLEMENT (काढा_clip) \
  HB_PAINT_FUNC_IMPLEMENT (color) \
  HB_PAINT_FUNC_IMPLEMENT (image) \
  HB_PAINT_FUNC_IMPLEMENT (linear_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (radial_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (sweep_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (ढकला_group) \
  HB_PAINT_FUNC_IMPLEMENT (ढकला_group_for) \
  HB_PAINT_FUNC_IMPLEMENT (काढा_group) \
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

  शून्य ढकला_transform (शून्य *paint_data,
                       दशांश xx, दशांश yx,
                       दशांश xy, दशांश yy,
                       दशांश dx, दशांश dy)
  {
    // Handle -0.f to avoid -0.f == 0.f in the transform matrix.
    जर (dx == -0.f) dx = 0.f;
    जर (dy == -0.f) dy = 0.f;
    func.ढकला_transform (हे, paint_data,
                         xx, yx, xy, yy, dx, dy,
                         !user_data ? nullptr : user_data->ढकला_transform); }
  शून्य काढा_transform (शून्य *paint_data)
  { func.काढा_transform (हे, paint_data,
                        !user_data ? nullptr : user_data->काढा_transform); }
  तार्किक color_glyph (शून्य *paint_data,
                    hb_codepoint_t glyph,
                    hb_font_t *font)
  { परत func.color_glyph (हे, paint_data,
                             glyph,
                             font,
                             !user_data ? nullptr : user_data->color_glyph); }
  शून्य ढकला_clip_glyph (शून्य *paint_data,
                        hb_codepoint_t glyph,
                        hb_font_t *font)
  { func.ढकला_clip_glyph (हे, paint_data,
                          glyph,
                          font,
                          !user_data ? nullptr : user_data->ढकला_clip_glyph); }
  शून्य ढकला_clip_rectangle (शून्य *paint_data,
                           दशांश xmin, दशांश ymin, दशांश xmax, दशांश ymax)
  { func.ढकला_clip_rectangle (हे, paint_data,
                              xmin, ymin, xmax, ymax,
                              !user_data ? nullptr : user_data->ढकला_clip_rectangle); }
  hb_draw_funcs_t *ढकला_clip_path_start (शून्य *paint_data,
                                         शून्य **draw_data)
  { परत func.ढकला_clip_path_start (हे, paint_data, draw_data,
                                      !user_data ? nullptr : user_data->ढकला_clip_path_start); }
  शून्य ढकला_clip_path_end (शून्य *paint_data)
  { func.ढकला_clip_path_end (हे, paint_data,
                             !user_data ? nullptr : user_data->ढकला_clip_path_end); }
  शून्य काढा_clip (शून्य *paint_data)
  { func.काढा_clip (हे, paint_data,
                   !user_data ? nullptr : user_data->काढा_clip); }
  शून्य color (शून्य *paint_data,
              hb_bool_t is_foreground,
              hb_color_t color)
  { func.color (हे, paint_data,
                is_foreground, color,
                !user_data ? nullptr : user_data->color); }
  शून्य fill_glyph (शून्य *paint_data,
                   hb_codepoint_t glyph,
                   hb_font_t *font,
                   hb_bool_t is_foreground,
                   hb_color_t color)
  { func.fill_glyph (हे, paint_data,
                     glyph, font,
                     is_foreground, color,
                     !user_data ? nullptr : user_data->fill_glyph); }
  तार्किक image (शून्य *paint_data,
              hb_blob_t *image,
              अचिन्हित width, अचिन्हित height,
              hb_tag_t format,
              दशांश slant,
              hb_glyph_extents_t *extents)
  { परत func.image (हे, paint_data,
                       image, width, height, format, slant, extents,
                       !user_data ? nullptr : user_data->image); }
  शून्य linear_gradient (शून्य *paint_data,
                        hb_color_ओळ_t *color_ओळ,
                        दशांश x0, दशांश y0,
                        दशांश x1, दशांश y1,
                        दशांश x2, दशांश y2)
  { func.linear_gradient (हे, paint_data,
                          color_ओळ, x0, y0, x1, y1, x2, y2,
                          !user_data ? nullptr : user_data->linear_gradient); }
  शून्य radial_gradient (शून्य *paint_data,
                        hb_color_ओळ_t *color_ओळ,
                        दशांश x0, दशांश y0, दशांश r0,
                        दशांश x1, दशांश y1, दशांश r1)
  { func.radial_gradient (हे, paint_data,
                          color_ओळ, x0, y0, r0, x1, y1, r1,
                          !user_data ? nullptr : user_data->radial_gradient); }
  शून्य sweep_gradient (शून्य *paint_data,
                       hb_color_ओळ_t *color_ओळ,
                       दशांश x0, दशांश y0,
                       दशांश start_angle,
                       दशांश end_angle)
  { func.sweep_gradient (हे, paint_data,
                         color_ओळ, x0, y0, start_angle, end_angle,
                         !user_data ? nullptr : user_data->sweep_gradient); }
  शून्य ढकला_group (शून्य *paint_data)
  { func.ढकला_group (हे, paint_data,
                     !user_data ? nullptr : user_data->ढकला_group); }
  शून्य ढकला_group_for (शून्य *paint_data,
                       hb_paint_composite_mode_t mode)
  { func.ढकला_group_for (हे, paint_data,
                         mode,
                         !user_data ? nullptr : user_data->ढकला_group_for); }
  शून्य काढा_group (शून्य *paint_data,
                  hb_paint_composite_mode_t mode)
  { func.काढा_group (हे, paint_data,
                    mode,
                    !user_data ? nullptr : user_data->काढा_group); }
  तार्किक custom_palette_color (शून्य *paint_data,
                             अचिन्हित पूर्णांक color_index,
                             hb_color_t *color)
  { परत func.custom_palette_color (हे, paint_data,
                                      color_index,
                                      color,
                                      !user_data ? nullptr : user_data->custom_palette_color); }
  तार्किक set_budget (शून्य *paint_data, int64_t budget)
  {
    जर (budget < 0 && budget != HB_BUDGET_DEFAULT)
      budget = 0;
    परत func.set_budget (हे, paint_data, budget,
			    !user_data ? nullptr : user_data->set_budget);
  }
  int64_t get_budget (शून्य *paint_data)
  { परत func.get_budget (हे, paint_data,
			    !user_data ? nullptr : user_data->get_budget); }
  int64_t *get_budget_remaining_ptr (शून्य *paint_data)
  { परत func.get_budget_remaining (हे, paint_data,
				      !user_data ? nullptr : user_data->get_budget_remaining); }


  /* Internal specializations. */

  शून्य ढकला_font_transform (शून्य *paint_data,
                            स्थिरांक hb_font_t *font)
  {
    दशांश upem = font->face->get_upem ();
    पूर्णांक xscale = font->x_scale, yscale = font->y_scale;

    ढकला_transform (paint_data,
		    xscale/upem, 0,
		    0, yscale/upem,
		    0, 0);
  }

  शून्य ढकला_inverse_font_transform (शून्य *paint_data,
                                    स्थिरांक hb_font_t *font)
  {
    दशांश upem = font->face->get_upem ();
    पूर्णांक xscale = font->x_scale ? font->x_scale : upem;
    पूर्णांक yscale = font->y_scale ? font->y_scale : upem;

    ढकला_transform (paint_data,
		    upem/xscale, 0,
		    0, upem/yscale,
		    0, 0);
  }

  शून्य ढकला_transform (शून्य *paint_data, hb_transform_t<दशांश> t)
  {
    ढकला_transform (paint_data, t.xx, t.yx, t.xy, t.yy, t.x0, t.y0);
  }

  शून्य ढकला_translate (शून्य *paint_data,
                       दशांश dx, दशांश dy)
  {
    ढकला_transform (paint_data,
		    hb_transform_t<दशांश>::translation (dx, dy));
  }

  शून्य ढकला_scale (शून्य *paint_data,
                   दशांश sx, दशांश sy)
  {
    ढकला_transform (paint_data,
		    hb_transform_t<दशांश>::scaling (sx, sy));
  }
  शून्य ढकला_scale_around_center (शून्य *paint_data,
				 दशांश sx, दशांश sy,
				 दशांश cx, दशांश cy)
  {
    ढकला_transform (paint_data,
		    hb_transform_t<दशांश>::scaling_around_center (sx, sy, cx, cy));
  }

  शून्य ढकला_rotate (शून्य *paint_data,
                    दशांश a)
  {
    ढकला_transform (paint_data,
		    hb_transform_t<दशांश>::rotation (a * HB_PI));
  }

  शून्य ढकला_rotate_around_center (शून्य *paint_data,
				  दशांश a,
				  दशांश cx, दशांश cy)
  {
    ढकला_transform (paint_data,
		    hb_transform_t<दशांश>::rotation_around_center (a * HB_PI, cx, cy));
  }

  शून्य ढकला_skew (शून्य *paint_data,
                  दशांश sx, दशांश sy)
  {
    ढकला_transform (paint_data,
		    hb_transform_t<दशांश>::skewing (-sx * HB_PI, sy * HB_PI));
  }
  शून्य ढकला_skew_around_center (शून्य *paint_data,
				दशांश sx, दशांश sy,
				दशांश cx, दशांश cy)
  {
    ढकला_transform (paint_data,
		    hb_transform_t<दशांश>::skewing_around_center (-sx * HB_PI, sy * HB_PI, cx, cy));
  }
};
DECLARE_NULL_INSTANCE (hb_paint_funcs_t);


/* Linearly interpolate between two hb_color_t values, component-wise,
 * in byte-channel space with rounding. */
स्थिर ओळीत hb_color_t
hb_color_lerp (hb_color_t c0, hb_color_t c1, दशांश t)
{
  स्वतः lerp = [&] (अचिन्हित shift) -> अचिन्हित {
    अचिन्हित v0 = (c0 >> shift) & 0xFF;
    अचिन्हित v1 = (c1 >> shift) & 0xFF;
    दशांश v = v0 + t * ((दशांश) v1 - (दशांश) v0) + 0.5f;
    /* t may be outside [0,1] (or NaN) for malformed gradients;
     * keep the float-to-unsigned conversion defined. */
    जर (!(v > 0.f)) परत 0;
    जर (v > 255.f) परत 255;
    परत (अचिन्हित) v;
  };
  परत HB_COLOR (lerp (0), lerp (8), lerp (16), lerp (24));
}

#endif /* HB_PAINT_HH */

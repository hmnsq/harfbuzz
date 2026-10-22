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

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-face.hh"
#অন্তর্ভাব "hb-font.hh"
#অন্তর্ভাব "hb-geometry.hh"

#define HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS \
  HB_PAINT_FUNC_IMPLEMENT (ঠেলো_transform) \
  HB_PAINT_FUNC_IMPLEMENT (বের_করো_transform) \
  HB_PAINT_FUNC_IMPLEMENT (color_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (ঠেলো_clip_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (ঠেলো_clip_rectangle) \
  HB_PAINT_FUNC_IMPLEMENT (ঠেলো_clip_path_start) \
  HB_PAINT_FUNC_IMPLEMENT (ঠেলো_clip_path_end) \
  HB_PAINT_FUNC_IMPLEMENT (বের_করো_clip) \
  HB_PAINT_FUNC_IMPLEMENT (color) \
  HB_PAINT_FUNC_IMPLEMENT (image) \
  HB_PAINT_FUNC_IMPLEMENT (linear_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (radial_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (sweep_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (ঠেলো_group) \
  HB_PAINT_FUNC_IMPLEMENT (ঠেলো_group_for) \
  HB_PAINT_FUNC_IMPLEMENT (বের_করো_group) \
  HB_PAINT_FUNC_IMPLEMENT (custom_palette_color) \
  HB_PAINT_FUNC_IMPLEMENT (fill_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (set_budget) \
  HB_PAINT_FUNC_IMPLEMENT (get_budget) \
  HB_PAINT_FUNC_IMPLEMENT (get_budget_remaining) \
  /* ^--- Add new callbacks here */

সংরচনা hb_paint_funcs_t
{
  hb_object_header_t header;

  সংরচনা {
#define HB_PAINT_FUNC_IMPLEMENT(name) hb_paint_##name##_func_t name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_PAINT_FUNC_IMPLEMENT
  } func;

  সংরচনা {
#define HB_PAINT_FUNC_IMPLEMENT(name) শূন্য *name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_PAINT_FUNC_IMPLEMENT
  } *user_data;

  সংরচনা {
#define HB_PAINT_FUNC_IMPLEMENT(name) hb_destroy_func_t name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_PAINT_FUNC_IMPLEMENT
  } *destroy;

  শূন্য ঠেলো_transform (শূন্য *paint_data,
                       ভিন্ন xx, ভিন্ন yx,
                       ভিন্ন xy, ভিন্ন yy,
                       ভিন্ন dx, ভিন্ন dy)
  {
    // Handle -0.f to avoid -0.f == 0.f in the transform matrix.
    যদি (dx == -0.f) dx = 0.f;
    যদি (dy == -0.f) dy = 0.f;
    func.ঠেলো_transform (এতত্, paint_data,
                         xx, yx, xy, yy, dx, dy,
                         !user_data ? nullptr : user_data->ঠেলো_transform); }
  শূন্য বের_করো_transform (শূন্য *paint_data)
  { func.বের_করো_transform (এতত্, paint_data,
                        !user_data ? nullptr : user_data->বের_করো_transform); }
  তার্কিক color_glyph (শূন্য *paint_data,
                    hb_codepoint_t glyph,
                    hb_font_t *font)
  { প্রত্যাগম func.color_glyph (এতত্, paint_data,
                             glyph,
                             font,
                             !user_data ? nullptr : user_data->color_glyph); }
  শূন্য ঠেলো_clip_glyph (শূন্য *paint_data,
                        hb_codepoint_t glyph,
                        hb_font_t *font)
  { func.ঠেলো_clip_glyph (এতত্, paint_data,
                          glyph,
                          font,
                          !user_data ? nullptr : user_data->ঠেলো_clip_glyph); }
  শূন্য ঠেলো_clip_rectangle (শূন্য *paint_data,
                           ভিন্ন xmin, ভিন্ন ymin, ভিন্ন xmax, ভিন্ন ymax)
  { func.ঠেলো_clip_rectangle (এতত্, paint_data,
                              xmin, ymin, xmax, ymax,
                              !user_data ? nullptr : user_data->ঠেলো_clip_rectangle); }
  hb_draw_funcs_t *ঠেলো_clip_path_start (শূন্য *paint_data,
                                         শূন্য **draw_data)
  { প্রত্যাগম func.ঠেলো_clip_path_start (এতত্, paint_data, draw_data,
                                      !user_data ? nullptr : user_data->ঠেলো_clip_path_start); }
  শূন্য ঠেলো_clip_path_end (শূন্য *paint_data)
  { func.ঠেলো_clip_path_end (এতত্, paint_data,
                             !user_data ? nullptr : user_data->ঠেলো_clip_path_end); }
  শূন্য বের_করো_clip (শূন্য *paint_data)
  { func.বের_করো_clip (এতত্, paint_data,
                   !user_data ? nullptr : user_data->বের_করো_clip); }
  শূন্য color (শূন্য *paint_data,
              hb_bool_t is_foreground,
              hb_color_t color)
  { func.color (এতত্, paint_data,
                is_foreground, color,
                !user_data ? nullptr : user_data->color); }
  শূন্য fill_glyph (শূন্য *paint_data,
                   hb_codepoint_t glyph,
                   hb_font_t *font,
                   hb_bool_t is_foreground,
                   hb_color_t color)
  { func.fill_glyph (এতত্, paint_data,
                     glyph, font,
                     is_foreground, color,
                     !user_data ? nullptr : user_data->fill_glyph); }
  তার্কিক image (শূন্য *paint_data,
              hb_blob_t *image,
              unsigned width, unsigned height,
              hb_tag_t format,
              ভিন্ন slant,
              hb_glyph_extents_t *extents)
  { প্রত্যাগম func.image (এতত্, paint_data,
                       image, width, height, format, slant, extents,
                       !user_data ? nullptr : user_data->image); }
  শূন্য linear_gradient (শূন্য *paint_data,
                        hb_color_সারি_t *color_সারি,
                        ভিন্ন x0, ভিন্ন y0,
                        ভিন্ন x1, ভিন্ন y1,
                        ভিন্ন x2, ভিন্ন y2)
  { func.linear_gradient (এতত্, paint_data,
                          color_সারি, x0, y0, x1, y1, x2, y2,
                          !user_data ? nullptr : user_data->linear_gradient); }
  শূন্য radial_gradient (শূন্য *paint_data,
                        hb_color_সারি_t *color_সারি,
                        ভিন্ন x0, ভিন্ন y0, ভিন্ন r0,
                        ভিন্ন x1, ভিন্ন y1, ভিন্ন r1)
  { func.radial_gradient (এতত্, paint_data,
                          color_সারি, x0, y0, r0, x1, y1, r1,
                          !user_data ? nullptr : user_data->radial_gradient); }
  শূন্য sweep_gradient (শূন্য *paint_data,
                       hb_color_সারি_t *color_সারি,
                       ভিন্ন x0, ভিন্ন y0,
                       ভিন্ন start_angle,
                       ভিন্ন end_angle)
  { func.sweep_gradient (এতত্, paint_data,
                         color_সারি, x0, y0, start_angle, end_angle,
                         !user_data ? nullptr : user_data->sweep_gradient); }
  শূন্য ঠেলো_group (শূন্য *paint_data)
  { func.ঠেলো_group (এতত্, paint_data,
                     !user_data ? nullptr : user_data->ঠেলো_group); }
  শূন্য ঠেলো_group_for (শূন্য *paint_data,
                       hb_paint_composite_mode_t mode)
  { func.ঠেলো_group_for (এতত্, paint_data,
                         mode,
                         !user_data ? nullptr : user_data->ঠেলো_group_for); }
  শূন্য বের_করো_group (শূন্য *paint_data,
                  hb_paint_composite_mode_t mode)
  { func.বের_করো_group (এতত্, paint_data,
                    mode,
                    !user_data ? nullptr : user_data->বের_করো_group); }
  তার্কিক custom_palette_color (শূন্য *paint_data,
                             unsigned পূর্ণাঙ্ক color_index,
                             hb_color_t *color)
  { প্রত্যাগম func.custom_palette_color (এতত্, paint_data,
                                      color_index,
                                      color,
                                      !user_data ? nullptr : user_data->custom_palette_color); }
  তার্কিক set_budget (শূন্য *paint_data, int64_t budget)
  {
    যদি (budget < 0 && budget != HB_BUDGET_DEFAULT)
      budget = 0;
    প্রত্যাগম func.set_budget (এতত্, paint_data, budget,
			    !user_data ? nullptr : user_data->set_budget);
  }
  int64_t get_budget (শূন্য *paint_data)
  { প্রত্যাগম func.get_budget (এতত্, paint_data,
			    !user_data ? nullptr : user_data->get_budget); }
  int64_t *get_budget_remaining_ptr (শূন্য *paint_data)
  { প্রত্যাগম func.get_budget_remaining (এতত্, paint_data,
				      !user_data ? nullptr : user_data->get_budget_remaining); }


  /* Internal specializations. */

  শূন্য ঠেলো_font_transform (শূন্য *paint_data,
                            ধ্রুব hb_font_t *font)
  {
    ভিন্ন upem = font->face->get_upem ();
    পূর্ণাঙ্ক xscale = font->x_scale, yscale = font->y_scale;

    ঠেলো_transform (paint_data,
		    xscale/upem, 0,
		    0, yscale/upem,
		    0, 0);
  }

  শূন্য ঠেলো_inverse_font_transform (শূন্য *paint_data,
                                    ধ্রুব hb_font_t *font)
  {
    ভিন্ন upem = font->face->get_upem ();
    পূর্ণাঙ্ক xscale = font->x_scale ? font->x_scale : upem;
    পূর্ণাঙ্ক yscale = font->y_scale ? font->y_scale : upem;

    ঠেলো_transform (paint_data,
		    upem/xscale, 0,
		    0, upem/yscale,
		    0, 0);
  }

  শূন্য ঠেলো_transform (শূন্য *paint_data, hb_transform_t<ভিন্ন> t)
  {
    ঠেলো_transform (paint_data, t.xx, t.yx, t.xy, t.yy, t.x0, t.y0);
  }

  শূন্য ঠেলো_translate (শূন্য *paint_data,
                       ভিন্ন dx, ভিন্ন dy)
  {
    ঠেলো_transform (paint_data,
		    hb_transform_t<ভিন্ন>::translation (dx, dy));
  }

  শূন্য ঠেলো_scale (শূন্য *paint_data,
                   ভিন্ন sx, ভিন্ন sy)
  {
    ঠেলো_transform (paint_data,
		    hb_transform_t<ভিন্ন>::scaling (sx, sy));
  }
  শূন্য ঠেলো_scale_around_center (শূন্য *paint_data,
				 ভিন্ন sx, ভিন্ন sy,
				 ভিন্ন cx, ভিন্ন cy)
  {
    ঠেলো_transform (paint_data,
		    hb_transform_t<ভিন্ন>::scaling_around_center (sx, sy, cx, cy));
  }

  শূন্য ঠেলো_rotate (শূন্য *paint_data,
                    ভিন্ন a)
  {
    ঠেলো_transform (paint_data,
		    hb_transform_t<ভিন্ন>::rotation (a * HB_PI));
  }

  শূন্য ঠেলো_rotate_around_center (শূন্য *paint_data,
				  ভিন্ন a,
				  ভিন্ন cx, ভিন্ন cy)
  {
    ঠেলো_transform (paint_data,
		    hb_transform_t<ভিন্ন>::rotation_around_center (a * HB_PI, cx, cy));
  }

  শূন্য ঠেলো_skew (শূন্য *paint_data,
                  ভিন্ন sx, ভিন্ন sy)
  {
    ঠেলো_transform (paint_data,
		    hb_transform_t<ভিন্ন>::skewing (-sx * HB_PI, sy * HB_PI));
  }
  শূন্য ঠেলো_skew_around_center (শূন্য *paint_data,
				ভিন্ন sx, ভিন্ন sy,
				ভিন্ন cx, ভিন্ন cy)
  {
    ঠেলো_transform (paint_data,
		    hb_transform_t<ভিন্ন>::skewing_around_center (-sx * HB_PI, sy * HB_PI, cx, cy));
  }
};
DECLARE_NULL_INSTANCE (hb_paint_funcs_t);


/* Linearly interpolate between two hb_color_t values, component-wise,
 * in byte-channel space with rounding. */
স্থির অন্তঃস্থ hb_color_t
hb_color_lerp (hb_color_t c0, hb_color_t c1, ভিন্ন t)
{
  auto lerp = [&] (unsigned shift) -> unsigned {
    unsigned v0 = (c0 >> shift) & 0xFF;
    unsigned v1 = (c1 >> shift) & 0xFF;
    ভিন্ন v = v0 + t * ((ভিন্ন) v1 - (ভিন্ন) v0) + 0.5f;
    /* t may be outside [0,1] (or NaN) for malformed gradients;
     * keep the float-to-unsigned conversion defined. */
    যদি (!(v > 0.f)) প্রত্যাগম 0;
    যদি (v > 255.f) প্রত্যাগম 255;
    প্রত্যাগম (unsigned) v;
  };
  প্রত্যাগম HB_COLOR (lerp (0), lerp (8), lerp (16), lerp (24));
}

#endif /* HB_PAINT_HH */

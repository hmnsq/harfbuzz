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

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-face.hh"
#ଅନ୍ତର୍ଭାଵ "hb-font.hh"
#ଅନ୍ତର୍ଭାଵ "hb-geometry.hh"

#define HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS \
  HB_PAINT_FUNC_IMPLEMENT (ଠେଲ_transform) \
  HB_PAINT_FUNC_IMPLEMENT (କାଢ଼_transform) \
  HB_PAINT_FUNC_IMPLEMENT (color_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (ଠେଲ_clip_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (ଠେଲ_clip_rectangle) \
  HB_PAINT_FUNC_IMPLEMENT (ଠେଲ_clip_path_start) \
  HB_PAINT_FUNC_IMPLEMENT (ଠେଲ_clip_path_end) \
  HB_PAINT_FUNC_IMPLEMENT (କାଢ଼_clip) \
  HB_PAINT_FUNC_IMPLEMENT (color) \
  HB_PAINT_FUNC_IMPLEMENT (image) \
  HB_PAINT_FUNC_IMPLEMENT (linear_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (radial_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (sweep_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (ଠେଲ_group) \
  HB_PAINT_FUNC_IMPLEMENT (ଠେଲ_group_for) \
  HB_PAINT_FUNC_IMPLEMENT (କାଢ଼_group) \
  HB_PAINT_FUNC_IMPLEMENT (custom_palette_color) \
  HB_PAINT_FUNC_IMPLEMENT (fill_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (set_budget) \
  HB_PAINT_FUNC_IMPLEMENT (get_budget) \
  HB_PAINT_FUNC_IMPLEMENT (get_budget_remaining) \
  /* ^--- Add new callbacks here */

ସଂରଚନା hb_paint_funcs_t
{
  hb_object_header_t header;

  ସଂରଚନା {
#define HB_PAINT_FUNC_IMPLEMENT(name) hb_paint_##name##_func_t name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_PAINT_FUNC_IMPLEMENT
  } func;

  ସଂରଚନା {
#define HB_PAINT_FUNC_IMPLEMENT(name) ଶୂନ୍ୟ *name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_PAINT_FUNC_IMPLEMENT
  } *user_data;

  ସଂରଚନା {
#define HB_PAINT_FUNC_IMPLEMENT(name) hb_destroy_func_t name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_PAINT_FUNC_IMPLEMENT
  } *destroy;

  ଶୂନ୍ୟ ଠେଲ_transform (ଶୂନ୍ୟ *paint_data,
                       ଦଶମିକ xx, ଦଶମିକ yx,
                       ଦଶମିକ xy, ଦଶମିକ yy,
                       ଦଶମିକ dx, ଦଶମିକ dy)
  {
    // Handle -0.f to avoid -0.f == 0.f in the transform matrix.
    ଯଦି (dx == -0.f) dx = 0.f;
    ଯଦି (dy == -0.f) dy = 0.f;
    func.ଠେଲ_transform (ଏହି, paint_data,
                         xx, yx, xy, yy, dx, dy,
                         !user_data ? nullptr : user_data->ଠେଲ_transform); }
  ଶୂନ୍ୟ କାଢ଼_transform (ଶୂନ୍ୟ *paint_data)
  { func.କାଢ଼_transform (ଏହି, paint_data,
                        !user_data ? nullptr : user_data->କାଢ଼_transform); }
  ତାର୍କିକ color_glyph (ଶୂନ୍ୟ *paint_data,
                    hb_codepoint_t glyph,
                    hb_font_t *font)
  { ଫେରନ୍ତୁ func.color_glyph (ଏହି, paint_data,
                             glyph,
                             font,
                             !user_data ? nullptr : user_data->color_glyph); }
  ଶୂନ୍ୟ ଠେଲ_clip_glyph (ଶୂନ୍ୟ *paint_data,
                        hb_codepoint_t glyph,
                        hb_font_t *font)
  { func.ଠେଲ_clip_glyph (ଏହି, paint_data,
                          glyph,
                          font,
                          !user_data ? nullptr : user_data->ଠେଲ_clip_glyph); }
  ଶୂନ୍ୟ ଠେଲ_clip_rectangle (ଶୂନ୍ୟ *paint_data,
                           ଦଶମିକ xmin, ଦଶମିକ ymin, ଦଶମିକ xmax, ଦଶମିକ ymax)
  { func.ଠେଲ_clip_rectangle (ଏହି, paint_data,
                              xmin, ymin, xmax, ymax,
                              !user_data ? nullptr : user_data->ଠେଲ_clip_rectangle); }
  hb_draw_funcs_t *ଠେଲ_clip_path_start (ଶୂନ୍ୟ *paint_data,
                                         ଶୂନ୍ୟ **draw_data)
  { ଫେରନ୍ତୁ func.ଠେଲ_clip_path_start (ଏହି, paint_data, draw_data,
                                      !user_data ? nullptr : user_data->ଠେଲ_clip_path_start); }
  ଶୂନ୍ୟ ଠେଲ_clip_path_end (ଶୂନ୍ୟ *paint_data)
  { func.ଠେଲ_clip_path_end (ଏହି, paint_data,
                             !user_data ? nullptr : user_data->ଠେଲ_clip_path_end); }
  ଶୂନ୍ୟ କାଢ଼_clip (ଶୂନ୍ୟ *paint_data)
  { func.କାଢ଼_clip (ଏହି, paint_data,
                   !user_data ? nullptr : user_data->କାଢ଼_clip); }
  ଶୂନ୍ୟ color (ଶୂନ୍ୟ *paint_data,
              hb_bool_t is_foreground,
              hb_color_t color)
  { func.color (ଏହି, paint_data,
                is_foreground, color,
                !user_data ? nullptr : user_data->color); }
  ଶୂନ୍ୟ fill_glyph (ଶୂନ୍ୟ *paint_data,
                   hb_codepoint_t glyph,
                   hb_font_t *font,
                   hb_bool_t is_foreground,
                   hb_color_t color)
  { func.fill_glyph (ଏହି, paint_data,
                     glyph, font,
                     is_foreground, color,
                     !user_data ? nullptr : user_data->fill_glyph); }
  ତାର୍କିକ image (ଶୂନ୍ୟ *paint_data,
              hb_blob_t *image,
              unsigned width, unsigned height,
              hb_tag_t format,
              ଦଶମିକ slant,
              hb_glyph_extents_t *extents)
  { ଫେରନ୍ତୁ func.image (ଏହି, paint_data,
                       image, width, height, format, slant, extents,
                       !user_data ? nullptr : user_data->image); }
  ଶୂନ୍ୟ linear_gradient (ଶୂନ୍ୟ *paint_data,
                        hb_color_ଧାଡ଼ି_t *color_ଧାଡ଼ି,
                        ଦଶମିକ x0, ଦଶମିକ y0,
                        ଦଶମିକ x1, ଦଶମିକ y1,
                        ଦଶମିକ x2, ଦଶମିକ y2)
  { func.linear_gradient (ଏହି, paint_data,
                          color_ଧାଡ଼ି, x0, y0, x1, y1, x2, y2,
                          !user_data ? nullptr : user_data->linear_gradient); }
  ଶୂନ୍ୟ radial_gradient (ଶୂନ୍ୟ *paint_data,
                        hb_color_ଧାଡ଼ି_t *color_ଧାଡ଼ି,
                        ଦଶମିକ x0, ଦଶମିକ y0, ଦଶମିକ r0,
                        ଦଶମିକ x1, ଦଶମିକ y1, ଦଶମିକ r1)
  { func.radial_gradient (ଏହି, paint_data,
                          color_ଧାଡ଼ି, x0, y0, r0, x1, y1, r1,
                          !user_data ? nullptr : user_data->radial_gradient); }
  ଶୂନ୍ୟ sweep_gradient (ଶୂନ୍ୟ *paint_data,
                       hb_color_ଧାଡ଼ି_t *color_ଧାଡ଼ି,
                       ଦଶମିକ x0, ଦଶମିକ y0,
                       ଦଶମିକ start_angle,
                       ଦଶମିକ end_angle)
  { func.sweep_gradient (ଏହି, paint_data,
                         color_ଧାଡ଼ି, x0, y0, start_angle, end_angle,
                         !user_data ? nullptr : user_data->sweep_gradient); }
  ଶୂନ୍ୟ ଠେଲ_group (ଶୂନ୍ୟ *paint_data)
  { func.ଠେଲ_group (ଏହି, paint_data,
                     !user_data ? nullptr : user_data->ଠେଲ_group); }
  ଶୂନ୍ୟ ଠେଲ_group_for (ଶୂନ୍ୟ *paint_data,
                       hb_paint_composite_mode_t mode)
  { func.ଠେଲ_group_for (ଏହି, paint_data,
                         mode,
                         !user_data ? nullptr : user_data->ଠେଲ_group_for); }
  ଶୂନ୍ୟ କାଢ଼_group (ଶୂନ୍ୟ *paint_data,
                  hb_paint_composite_mode_t mode)
  { func.କାଢ଼_group (ଏହି, paint_data,
                    mode,
                    !user_data ? nullptr : user_data->କାଢ଼_group); }
  ତାର୍କିକ custom_palette_color (ଶୂନ୍ୟ *paint_data,
                             unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା color_index,
                             hb_color_t *color)
  { ଫେରନ୍ତୁ func.custom_palette_color (ଏହି, paint_data,
                                      color_index,
                                      color,
                                      !user_data ? nullptr : user_data->custom_palette_color); }
  ତାର୍କିକ set_budget (ଶୂନ୍ୟ *paint_data, int64_t budget)
  {
    ଯଦି (budget < 0 && budget != HB_BUDGET_DEFAULT)
      budget = 0;
    ଫେରନ୍ତୁ func.set_budget (ଏହି, paint_data, budget,
			    !user_data ? nullptr : user_data->set_budget);
  }
  int64_t get_budget (ଶୂନ୍ୟ *paint_data)
  { ଫେରନ୍ତୁ func.get_budget (ଏହି, paint_data,
			    !user_data ? nullptr : user_data->get_budget); }
  int64_t *get_budget_remaining_ptr (ଶୂନ୍ୟ *paint_data)
  { ଫେରନ୍ତୁ func.get_budget_remaining (ଏହି, paint_data,
				      !user_data ? nullptr : user_data->get_budget_remaining); }


  /* Internal specializations. */

  ଶୂନ୍ୟ ଠେଲ_font_transform (ଶୂନ୍ୟ *paint_data,
                            ଅଚଳ hb_font_t *font)
  {
    ଦଶମିକ upem = font->face->get_upem ();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା xscale = font->x_scale, yscale = font->y_scale;

    ଠେଲ_transform (paint_data,
		    xscale/upem, 0,
		    0, yscale/upem,
		    0, 0);
  }

  ଶୂନ୍ୟ ଠେଲ_inverse_font_transform (ଶୂନ୍ୟ *paint_data,
                                    ଅଚଳ hb_font_t *font)
  {
    ଦଶମିକ upem = font->face->get_upem ();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା xscale = font->x_scale ? font->x_scale : upem;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା yscale = font->y_scale ? font->y_scale : upem;

    ଠେଲ_transform (paint_data,
		    upem/xscale, 0,
		    0, upem/yscale,
		    0, 0);
  }

  ଶୂନ୍ୟ ଠେଲ_transform (ଶୂନ୍ୟ *paint_data, hb_transform_t<ଦଶମିକ> t)
  {
    ଠେଲ_transform (paint_data, t.xx, t.yx, t.xy, t.yy, t.x0, t.y0);
  }

  ଶୂନ୍ୟ ଠେଲ_translate (ଶୂନ୍ୟ *paint_data,
                       ଦଶମିକ dx, ଦଶମିକ dy)
  {
    ଠେଲ_transform (paint_data,
		    hb_transform_t<ଦଶମିକ>::translation (dx, dy));
  }

  ଶୂନ୍ୟ ଠେଲ_scale (ଶୂନ୍ୟ *paint_data,
                   ଦଶମିକ sx, ଦଶମିକ sy)
  {
    ଠେଲ_transform (paint_data,
		    hb_transform_t<ଦଶମିକ>::scaling (sx, sy));
  }
  ଶୂନ୍ୟ ଠେଲ_scale_around_center (ଶୂନ୍ୟ *paint_data,
				 ଦଶମିକ sx, ଦଶମିକ sy,
				 ଦଶମିକ cx, ଦଶମିକ cy)
  {
    ଠେଲ_transform (paint_data,
		    hb_transform_t<ଦଶମିକ>::scaling_around_center (sx, sy, cx, cy));
  }

  ଶୂନ୍ୟ ଠେଲ_rotate (ଶୂନ୍ୟ *paint_data,
                    ଦଶମିକ a)
  {
    ଠେଲ_transform (paint_data,
		    hb_transform_t<ଦଶମିକ>::rotation (a * HB_PI));
  }

  ଶୂନ୍ୟ ଠେଲ_rotate_around_center (ଶୂନ୍ୟ *paint_data,
				  ଦଶମିକ a,
				  ଦଶମିକ cx, ଦଶମିକ cy)
  {
    ଠେଲ_transform (paint_data,
		    hb_transform_t<ଦଶମିକ>::rotation_around_center (a * HB_PI, cx, cy));
  }

  ଶୂନ୍ୟ ଠେଲ_skew (ଶୂନ୍ୟ *paint_data,
                  ଦଶମିକ sx, ଦଶମିକ sy)
  {
    ଠେଲ_transform (paint_data,
		    hb_transform_t<ଦଶମିକ>::skewing (-sx * HB_PI, sy * HB_PI));
  }
  ଶୂନ୍ୟ ଠେଲ_skew_around_center (ଶୂନ୍ୟ *paint_data,
				ଦଶମିକ sx, ଦଶମିକ sy,
				ଦଶମିକ cx, ଦଶମିକ cy)
  {
    ଠେଲ_transform (paint_data,
		    hb_transform_t<ଦଶମିକ>::skewing_around_center (-sx * HB_PI, sy * HB_PI, cx, cy));
  }
};
DECLARE_NULL_INSTANCE (hb_paint_funcs_t);


/* Linearly interpolate between two hb_color_t values, component-wise,
 * in byte-channel space with rounding. */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ hb_color_t
hb_color_lerp (hb_color_t c0, hb_color_t c1, ଦଶମିକ t)
{
  auto lerp = [&] (unsigned shift) -> unsigned {
    unsigned v0 = (c0 >> shift) & 0xFF;
    unsigned v1 = (c1 >> shift) & 0xFF;
    ଦଶମିକ v = v0 + t * ((ଦଶମିକ) v1 - (ଦଶମିକ) v0) + 0.5f;
    /* t may be outside [0,1] (or NaN) for malformed gradients;
     * keep the float-to-unsigned conversion defined. */
    ଯଦି (!(v > 0.f)) ଫେରନ୍ତୁ 0;
    ଯଦି (v > 255.f) ଫେରନ୍ତୁ 255;
    ଫେରନ୍ତୁ (unsigned) v;
  };
  ଫେରନ୍ତୁ HB_COLOR (lerp (0), lerp (8), lerp (16), lerp (24));
}

#endif /* HB_PAINT_HH */

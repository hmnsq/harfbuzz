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

#अगर_अघोषित HB_PAINT_HH
#घोषणा HB_PAINT_HH

#समावेश "hb.hh"
#समावेश "hb-face.hh"
#समावेश "hb-font.hh"
#समावेश "hb-geometry.hh"

#घोषणा HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS \
  HB_PAINT_FUNC_IMPLEMENT (धकेलो_transform) \
  HB_PAINT_FUNC_IMPLEMENT (निकालो_transform) \
  HB_PAINT_FUNC_IMPLEMENT (color_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (धकेलो_clip_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (धकेलो_clip_rectangle) \
  HB_PAINT_FUNC_IMPLEMENT (धकेलो_clip_path_start) \
  HB_PAINT_FUNC_IMPLEMENT (धकेलो_clip_path_end) \
  HB_PAINT_FUNC_IMPLEMENT (निकालो_clip) \
  HB_PAINT_FUNC_IMPLEMENT (color) \
  HB_PAINT_FUNC_IMPLEMENT (image) \
  HB_PAINT_FUNC_IMPLEMENT (linear_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (radial_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (sweep_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (धकेलो_group) \
  HB_PAINT_FUNC_IMPLEMENT (धकेलो_group_for) \
  HB_PAINT_FUNC_IMPLEMENT (निकालो_group) \
  HB_PAINT_FUNC_IMPLEMENT (custom_palette_color) \
  HB_PAINT_FUNC_IMPLEMENT (fill_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (set_budget) \
  HB_PAINT_FUNC_IMPLEMENT (get_budget) \
  HB_PAINT_FUNC_IMPLEMENT (get_budget_remaining) \
  /* ^--- Add new callbacks here */

काष्ठा hb_paint_funcs_t
{
  hb_object_header_t header;

  काष्ठा {
#घोषणा HB_PAINT_FUNC_IMPLEMENT(name) hb_paint_##name##_func_t name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#अघोषित HB_PAINT_FUNC_IMPLEMENT
  } func;

  काष्ठा {
#घोषणा HB_PAINT_FUNC_IMPLEMENT(name) व्योम *name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#अघोषित HB_PAINT_FUNC_IMPLEMENT
  } *user_तथ्य;

  काष्ठा {
#घोषणा HB_PAINT_FUNC_IMPLEMENT(name) hb_destroy_func_t name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#अघोषित HB_PAINT_FUNC_IMPLEMENT
  } *destroy;

  व्योम धकेलो_transform (व्योम *paint_तथ्य,
                       भग्न xx, भग्न yx,
                       भग्न xy, भग्न yy,
                       भग्न dx, भग्न dy)
  {
    // Handle -0.f to avoid -0.f == 0.f in the transform matrix.
    अगर (dx == -0.f) dx = 0.f;
    अगर (dy == -0.f) dy = 0.f;
    func.धकेलो_transform (यह, paint_तथ्य,
                         xx, yx, xy, yy, dx, dy,
                         !user_तथ्य ? nullptr : user_तथ्य->धकेलो_transform); }
  व्योम निकालो_transform (व्योम *paint_तथ्य)
  { func.निकालो_transform (यह, paint_तथ्य,
                        !user_तथ्य ? nullptr : user_तथ्य->निकालो_transform); }
  bool color_glyph (व्योम *paint_तथ्य,
                    hb_codepoint_t glyph,
                    hb_font_t *font)
  { वापस func.color_glyph (यह, paint_तथ्य,
                             glyph,
                             font,
                             !user_तथ्य ? nullptr : user_तथ्य->color_glyph); }
  व्योम धकेलो_clip_glyph (व्योम *paint_तथ्य,
                        hb_codepoint_t glyph,
                        hb_font_t *font)
  { func.धकेलो_clip_glyph (यह, paint_तथ्य,
                          glyph,
                          font,
                          !user_तथ्य ? nullptr : user_तथ्य->धकेलो_clip_glyph); }
  व्योम धकेलो_clip_rectangle (व्योम *paint_तथ्य,
                           भग्न xmin, भग्न ymin, भग्न xmax, भग्न ymax)
  { func.धकेलो_clip_rectangle (यह, paint_तथ्य,
                              xmin, ymin, xmax, ymax,
                              !user_तथ्य ? nullptr : user_तथ्य->धकेलो_clip_rectangle); }
  hb_draw_funcs_t *धकेलो_clip_path_start (व्योम *paint_तथ्य,
                                         व्योम **draw_तथ्य)
  { वापस func.धकेलो_clip_path_start (यह, paint_तथ्य, draw_तथ्य,
                                      !user_तथ्य ? nullptr : user_तथ्य->धकेलो_clip_path_start); }
  व्योम धकेलो_clip_path_end (व्योम *paint_तथ्य)
  { func.धकेलो_clip_path_end (यह, paint_तथ्य,
                             !user_तथ्य ? nullptr : user_तथ्य->धकेलो_clip_path_end); }
  व्योम निकालो_clip (व्योम *paint_तथ्य)
  { func.निकालो_clip (यह, paint_तथ्य,
                   !user_तथ्य ? nullptr : user_तथ्य->निकालो_clip); }
  व्योम color (व्योम *paint_तथ्य,
              hb_bool_t is_foreground,
              hb_color_t color)
  { func.color (यह, paint_तथ्य,
                is_foreground, color,
                !user_तथ्य ? nullptr : user_तथ्य->color); }
  व्योम fill_glyph (व्योम *paint_तथ्य,
                   hb_codepoint_t glyph,
                   hb_font_t *font,
                   hb_bool_t is_foreground,
                   hb_color_t color)
  { func.fill_glyph (यह, paint_तथ्य,
                     glyph, font,
                     is_foreground, color,
                     !user_तथ्य ? nullptr : user_तथ्य->fill_glyph); }
  bool image (व्योम *paint_तथ्य,
              hb_blob_t *image,
              अचिन्हित width, अचिन्हित height,
              hb_tag_t format,
              भग्न slant,
              hb_glyph_extents_t *extents)
  { वापस func.image (यह, paint_तथ्य,
                       image, width, height, format, slant, extents,
                       !user_तथ्य ? nullptr : user_तथ्य->image); }
  व्योम linear_gradient (व्योम *paint_तथ्य,
                        hb_color_पंक्ति_t *color_पंक्ति,
                        भग्न x0, भग्न y0,
                        भग्न x1, भग्न y1,
                        भग्न x2, भग्न y2)
  { func.linear_gradient (यह, paint_तथ्य,
                          color_पंक्ति, x0, y0, x1, y1, x2, y2,
                          !user_तथ्य ? nullptr : user_तथ्य->linear_gradient); }
  व्योम radial_gradient (व्योम *paint_तथ्य,
                        hb_color_पंक्ति_t *color_पंक्ति,
                        भग्न x0, भग्न y0, भग्न r0,
                        भग्न x1, भग्न y1, भग्न r1)
  { func.radial_gradient (यह, paint_तथ्य,
                          color_पंक्ति, x0, y0, r0, x1, y1, r1,
                          !user_तथ्य ? nullptr : user_तथ्य->radial_gradient); }
  व्योम sweep_gradient (व्योम *paint_तथ्य,
                       hb_color_पंक्ति_t *color_पंक्ति,
                       भग्न x0, भग्न y0,
                       भग्न start_angle,
                       भग्न end_angle)
  { func.sweep_gradient (यह, paint_तथ्य,
                         color_पंक्ति, x0, y0, start_angle, end_angle,
                         !user_तथ्य ? nullptr : user_तथ्य->sweep_gradient); }
  व्योम धकेलो_group (व्योम *paint_तथ्य)
  { func.धकेलो_group (यह, paint_तथ्य,
                     !user_तथ्य ? nullptr : user_तथ्य->धकेलो_group); }
  व्योम धकेलो_group_for (व्योम *paint_तथ्य,
                       hb_paint_composite_mode_t mode)
  { func.धकेलो_group_for (यह, paint_तथ्य,
                         mode,
                         !user_तथ्य ? nullptr : user_तथ्य->धकेलो_group_for); }
  व्योम निकालो_group (व्योम *paint_तथ्य,
                  hb_paint_composite_mode_t mode)
  { func.निकालो_group (यह, paint_तथ्य,
                    mode,
                    !user_तथ्य ? nullptr : user_तथ्य->निकालो_group); }
  bool custom_palette_color (व्योम *paint_तथ्य,
                             अचिन्हित पूर्णांक color_सूचकांक,
                             hb_color_t *color)
  { वापस func.custom_palette_color (यह, paint_तथ्य,
                                      color_सूचकांक,
                                      color,
                                      !user_तथ्य ? nullptr : user_तथ्य->custom_palette_color); }
  bool set_budget (व्योम *paint_तथ्य, int64_t budget)
  {
    अगर (budget < 0 && budget != HB_BUDGET_DEFAULT)
      budget = 0;
    वापस func.set_budget (यह, paint_तथ्य, budget,
			    !user_तथ्य ? nullptr : user_तथ्य->set_budget);
  }
  int64_t get_budget (व्योम *paint_तथ्य)
  { वापस func.get_budget (यह, paint_तथ्य,
			    !user_तथ्य ? nullptr : user_तथ्य->get_budget); }
  int64_t *get_budget_remaining_ptr (व्योम *paint_तथ्य)
  { वापस func.get_budget_remaining (यह, paint_तथ्य,
				      !user_तथ्य ? nullptr : user_तथ्य->get_budget_remaining); }


  /* Internal specializations. */

  व्योम धकेलो_font_transform (व्योम *paint_तथ्य,
                            स्थिर hb_font_t *font)
  {
    भग्न upem = font->face->get_upem ();
    पूर्णांक xscale = font->x_scale, yscale = font->y_scale;

    धकेलो_transform (paint_तथ्य,
		    xscale/upem, 0,
		    0, yscale/upem,
		    0, 0);
  }

  व्योम धकेलो_inverse_font_transform (व्योम *paint_तथ्य,
                                    स्थिर hb_font_t *font)
  {
    भग्न upem = font->face->get_upem ();
    पूर्णांक xscale = font->x_scale ? font->x_scale : upem;
    पूर्णांक yscale = font->y_scale ? font->y_scale : upem;

    धकेलो_transform (paint_तथ्य,
		    upem/xscale, 0,
		    0, upem/yscale,
		    0, 0);
  }

  व्योम धकेलो_transform (व्योम *paint_तथ्य, hb_transform_t<भग्न> t)
  {
    धकेलो_transform (paint_तथ्य, t.xx, t.yx, t.xy, t.yy, t.x0, t.y0);
  }

  व्योम धकेलो_translate (व्योम *paint_तथ्य,
                       भग्न dx, भग्न dy)
  {
    धकेलो_transform (paint_तथ्य,
		    hb_transform_t<भग्न>::translation (dx, dy));
  }

  व्योम धकेलो_scale (व्योम *paint_तथ्य,
                   भग्न sx, भग्न sy)
  {
    धकेलो_transform (paint_तथ्य,
		    hb_transform_t<भग्न>::scaling (sx, sy));
  }
  व्योम धकेलो_scale_around_center (व्योम *paint_तथ्य,
				 भग्न sx, भग्न sy,
				 भग्न cx, भग्न cy)
  {
    धकेलो_transform (paint_तथ्य,
		    hb_transform_t<भग्न>::scaling_around_center (sx, sy, cx, cy));
  }

  व्योम धकेलो_rotate (व्योम *paint_तथ्य,
                    भग्न a)
  {
    धकेलो_transform (paint_तथ्य,
		    hb_transform_t<भग्न>::rotation (a * HB_PI));
  }

  व्योम धकेलो_rotate_around_center (व्योम *paint_तथ्य,
				  भग्न a,
				  भग्न cx, भग्न cy)
  {
    धकेलो_transform (paint_तथ्य,
		    hb_transform_t<भग्न>::rotation_around_center (a * HB_PI, cx, cy));
  }

  व्योम धकेलो_skew (व्योम *paint_तथ्य,
                  भग्न sx, भग्न sy)
  {
    धकेलो_transform (paint_तथ्य,
		    hb_transform_t<भग्न>::skewing (-sx * HB_PI, sy * HB_PI));
  }
  व्योम धकेलो_skew_around_center (व्योम *paint_तथ्य,
				भग्न sx, भग्न sy,
				भग्न cx, भग्न cy)
  {
    धकेलो_transform (paint_तथ्य,
		    hb_transform_t<भग्न>::skewing_around_center (-sx * HB_PI, sy * HB_PI, cx, cy));
  }
};
DECLARE_NULL_INSTANCE (hb_paint_funcs_t);


/* Linearly interpolate between two hb_color_t values, component-wise,
 * in byte-channel space with rounding. */
अटल अंतरभूत hb_color_t
hb_color_lerp (hb_color_t c0, hb_color_t c1, भग्न t)
{
  स्वतः lerp = [&] (अचिन्हित shift) -> अचिन्हित {
    अचिन्हित v0 = (c0 >> shift) & 0xFF;
    अचिन्हित v1 = (c1 >> shift) & 0xFF;
    भग्न v = v0 + t * ((भग्न) v1 - (भग्न) v0) + 0.5f;
    /* t may be outside [0,1] (or NaN) for malformed gradients;
     * keep the float-to-unsigned conversion defined. */
    अगर (!(v > 0.f)) वापस 0;
    अगर (v > 255.f) वापस 255;
    वापस (अचिन्हित) v;
  };
  वापस HB_COLOR (lerp (0), lerp (8), lerp (16), lerp (24));
}

#पूर्ण_अगर /* HB_PAINT_HH */

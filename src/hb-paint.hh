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

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-face.hh"
#അന്തര്ഭാവ "hb-font.hh"
#അന്തര്ഭാവ "hb-geometry.hh"

#define HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS \
  HB_PAINT_FUNC_IMPLEMENT (തള്ളുക_transform) \
  HB_PAINT_FUNC_IMPLEMENT (എടുക്കുക_transform) \
  HB_PAINT_FUNC_IMPLEMENT (color_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (തള്ളുക_clip_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (തള്ളുക_clip_rectangle) \
  HB_PAINT_FUNC_IMPLEMENT (തള്ളുക_clip_path_start) \
  HB_PAINT_FUNC_IMPLEMENT (തള്ളുക_clip_path_end) \
  HB_PAINT_FUNC_IMPLEMENT (എടുക്കുക_clip) \
  HB_PAINT_FUNC_IMPLEMENT (color) \
  HB_PAINT_FUNC_IMPLEMENT (image) \
  HB_PAINT_FUNC_IMPLEMENT (linear_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (radial_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (sweep_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (തള്ളുക_group) \
  HB_PAINT_FUNC_IMPLEMENT (തള്ളുക_group_for) \
  HB_PAINT_FUNC_IMPLEMENT (എടുക്കുക_group) \
  HB_PAINT_FUNC_IMPLEMENT (custom_palette_color) \
  HB_PAINT_FUNC_IMPLEMENT (fill_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (set_budget) \
  HB_PAINT_FUNC_IMPLEMENT (get_budget) \
  HB_PAINT_FUNC_IMPLEMENT (get_budget_remaining) \
  /* ^--- Add new callbacks here */

ഘടന hb_paint_funcs_t
{
  hb_object_header_t header;

  ഘടന {
#define HB_PAINT_FUNC_IMPLEMENT(name) hb_paint_##name##_func_t name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_PAINT_FUNC_IMPLEMENT
  } func;

  ഘടന {
#define HB_PAINT_FUNC_IMPLEMENT(name) ശൂന്യം *name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_PAINT_FUNC_IMPLEMENT
  } *user_data;

  ഘടന {
#define HB_PAINT_FUNC_IMPLEMENT(name) hb_destroy_func_t name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_PAINT_FUNC_IMPLEMENT
  } *destroy;

  ശൂന്യം തള്ളുക_transform (ശൂന്യം *paint_data,
                       ദശാംശം xx, ദശാംശം yx,
                       ദശാംശം xy, ദശാംശം yy,
                       ദശാംശം dx, ദശാംശം dy)
  {
    // Handle -0.f to avoid -0.f == 0.f in the transform matrix.
    എങ്കിൽ (dx == -0.f) dx = 0.f;
    എങ്കിൽ (dy == -0.f) dy = 0.f;
    func.തള്ളുക_transform (ഇത്, paint_data,
                         xx, yx, xy, yy, dx, dy,
                         !user_data ? nullptr : user_data->തള്ളുക_transform); }
  ശൂന്യം എടുക്കുക_transform (ശൂന്യം *paint_data)
  { func.എടുക്കുക_transform (ഇത്, paint_data,
                        !user_data ? nullptr : user_data->എടുക്കുക_transform); }
  തർക്കം color_glyph (ശൂന്യം *paint_data,
                    hb_codepoint_t glyph,
                    hb_font_t *font)
  { തിരികെ func.color_glyph (ഇത്, paint_data,
                             glyph,
                             font,
                             !user_data ? nullptr : user_data->color_glyph); }
  ശൂന്യം തള്ളുക_clip_glyph (ശൂന്യം *paint_data,
                        hb_codepoint_t glyph,
                        hb_font_t *font)
  { func.തള്ളുക_clip_glyph (ഇത്, paint_data,
                          glyph,
                          font,
                          !user_data ? nullptr : user_data->തള്ളുക_clip_glyph); }
  ശൂന്യം തള്ളുക_clip_rectangle (ശൂന്യം *paint_data,
                           ദശാംശം xmin, ദശാംശം ymin, ദശാംശം xmax, ദശാംശം ymax)
  { func.തള്ളുക_clip_rectangle (ഇത്, paint_data,
                              xmin, ymin, xmax, ymax,
                              !user_data ? nullptr : user_data->തള്ളുക_clip_rectangle); }
  hb_draw_funcs_t *തള്ളുക_clip_path_start (ശൂന്യം *paint_data,
                                         ശൂന്യം **draw_data)
  { തിരികെ func.തള്ളുക_clip_path_start (ഇത്, paint_data, draw_data,
                                      !user_data ? nullptr : user_data->തള്ളുക_clip_path_start); }
  ശൂന്യം തള്ളുക_clip_path_end (ശൂന്യം *paint_data)
  { func.തള്ളുക_clip_path_end (ഇത്, paint_data,
                             !user_data ? nullptr : user_data->തള്ളുക_clip_path_end); }
  ശൂന്യം എടുക്കുക_clip (ശൂന്യം *paint_data)
  { func.എടുക്കുക_clip (ഇത്, paint_data,
                   !user_data ? nullptr : user_data->എടുക്കുക_clip); }
  ശൂന്യം color (ശൂന്യം *paint_data,
              hb_bool_t is_foreground,
              hb_color_t color)
  { func.color (ഇത്, paint_data,
                is_foreground, color,
                !user_data ? nullptr : user_data->color); }
  ശൂന്യം fill_glyph (ശൂന്യം *paint_data,
                   hb_codepoint_t glyph,
                   hb_font_t *font,
                   hb_bool_t is_foreground,
                   hb_color_t color)
  { func.fill_glyph (ഇത്, paint_data,
                     glyph, font,
                     is_foreground, color,
                     !user_data ? nullptr : user_data->fill_glyph); }
  തർക്കം image (ശൂന്യം *paint_data,
              hb_blob_t *image,
              unsigned width, unsigned height,
              hb_tag_t format,
              ദശാംശം slant,
              hb_glyph_extents_t *extents)
  { തിരികെ func.image (ഇത്, paint_data,
                       image, width, height, format, slant, extents,
                       !user_data ? nullptr : user_data->image); }
  ശൂന്യം linear_gradient (ശൂന്യം *paint_data,
                        hb_color_വരി_t *color_വരി,
                        ദശാംശം x0, ദശാംശം y0,
                        ദശാംശം x1, ദശാംശം y1,
                        ദശാംശം x2, ദശാംശം y2)
  { func.linear_gradient (ഇത്, paint_data,
                          color_വരി, x0, y0, x1, y1, x2, y2,
                          !user_data ? nullptr : user_data->linear_gradient); }
  ശൂന്യം radial_gradient (ശൂന്യം *paint_data,
                        hb_color_വരി_t *color_വരി,
                        ദശാംശം x0, ദശാംശം y0, ദശാംശം r0,
                        ദശാംശം x1, ദശാംശം y1, ദശാംശം r1)
  { func.radial_gradient (ഇത്, paint_data,
                          color_വരി, x0, y0, r0, x1, y1, r1,
                          !user_data ? nullptr : user_data->radial_gradient); }
  ശൂന്യം sweep_gradient (ശൂന്യം *paint_data,
                       hb_color_വരി_t *color_വരി,
                       ദശാംശം x0, ദശാംശം y0,
                       ദശാംശം start_angle,
                       ദശാംശം end_angle)
  { func.sweep_gradient (ഇത്, paint_data,
                         color_വരി, x0, y0, start_angle, end_angle,
                         !user_data ? nullptr : user_data->sweep_gradient); }
  ശൂന്യം തള്ളുക_group (ശൂന്യം *paint_data)
  { func.തള്ളുക_group (ഇത്, paint_data,
                     !user_data ? nullptr : user_data->തള്ളുക_group); }
  ശൂന്യം തള്ളുക_group_for (ശൂന്യം *paint_data,
                       hb_paint_composite_mode_t mode)
  { func.തള്ളുക_group_for (ഇത്, paint_data,
                         mode,
                         !user_data ? nullptr : user_data->തള്ളുക_group_for); }
  ശൂന്യം എടുക്കുക_group (ശൂന്യം *paint_data,
                  hb_paint_composite_mode_t mode)
  { func.എടുക്കുക_group (ഇത്, paint_data,
                    mode,
                    !user_data ? nullptr : user_data->എടുക്കുക_group); }
  തർക്കം custom_palette_color (ശൂന്യം *paint_data,
                             unsigned പൂർണ്ണസംഖ്യ color_index,
                             hb_color_t *color)
  { തിരികെ func.custom_palette_color (ഇത്, paint_data,
                                      color_index,
                                      color,
                                      !user_data ? nullptr : user_data->custom_palette_color); }
  തർക്കം set_budget (ശൂന്യം *paint_data, int64_t budget)
  {
    എങ്കിൽ (budget < 0 && budget != HB_BUDGET_DEFAULT)
      budget = 0;
    തിരികെ func.set_budget (ഇത്, paint_data, budget,
			    !user_data ? nullptr : user_data->set_budget);
  }
  int64_t get_budget (ശൂന്യം *paint_data)
  { തിരികെ func.get_budget (ഇത്, paint_data,
			    !user_data ? nullptr : user_data->get_budget); }
  int64_t *get_budget_remaining_ptr (ശൂന്യം *paint_data)
  { തിരികെ func.get_budget_remaining (ഇത്, paint_data,
				      !user_data ? nullptr : user_data->get_budget_remaining); }


  /* Internal specializations. */

  ശൂന്യം തള്ളുക_font_transform (ശൂന്യം *paint_data,
                            സ്ഥിരം hb_font_t *font)
  {
    ദശാംശം upem = font->face->get_upem ();
    പൂർണ്ണസംഖ്യ xscale = font->x_scale, yscale = font->y_scale;

    തള്ളുക_transform (paint_data,
		    xscale/upem, 0,
		    0, yscale/upem,
		    0, 0);
  }

  ശൂന്യം തള്ളുക_inverse_font_transform (ശൂന്യം *paint_data,
                                    സ്ഥിരം hb_font_t *font)
  {
    ദശാംശം upem = font->face->get_upem ();
    പൂർണ്ണസംഖ്യ xscale = font->x_scale ? font->x_scale : upem;
    പൂർണ്ണസംഖ്യ yscale = font->y_scale ? font->y_scale : upem;

    തള്ളുക_transform (paint_data,
		    upem/xscale, 0,
		    0, upem/yscale,
		    0, 0);
  }

  ശൂന്യം തള്ളുക_transform (ശൂന്യം *paint_data, hb_transform_t<ദശാംശം> t)
  {
    തള്ളുക_transform (paint_data, t.xx, t.yx, t.xy, t.yy, t.x0, t.y0);
  }

  ശൂന്യം തള്ളുക_translate (ശൂന്യം *paint_data,
                       ദശാംശം dx, ദശാംശം dy)
  {
    തള്ളുക_transform (paint_data,
		    hb_transform_t<ദശാംശം>::translation (dx, dy));
  }

  ശൂന്യം തള്ളുക_scale (ശൂന്യം *paint_data,
                   ദശാംശം sx, ദശാംശം sy)
  {
    തള്ളുക_transform (paint_data,
		    hb_transform_t<ദശാംശം>::scaling (sx, sy));
  }
  ശൂന്യം തള്ളുക_scale_around_center (ശൂന്യം *paint_data,
				 ദശാംശം sx, ദശാംശം sy,
				 ദശാംശം cx, ദശാംശം cy)
  {
    തള്ളുക_transform (paint_data,
		    hb_transform_t<ദശാംശം>::scaling_around_center (sx, sy, cx, cy));
  }

  ശൂന്യം തള്ളുക_rotate (ശൂന്യം *paint_data,
                    ദശാംശം a)
  {
    തള്ളുക_transform (paint_data,
		    hb_transform_t<ദശാംശം>::rotation (a * HB_PI));
  }

  ശൂന്യം തള്ളുക_rotate_around_center (ശൂന്യം *paint_data,
				  ദശാംശം a,
				  ദശാംശം cx, ദശാംശം cy)
  {
    തള്ളുക_transform (paint_data,
		    hb_transform_t<ദശാംശം>::rotation_around_center (a * HB_PI, cx, cy));
  }

  ശൂന്യം തള്ളുക_skew (ശൂന്യം *paint_data,
                  ദശാംശം sx, ദശാംശം sy)
  {
    തള്ളുക_transform (paint_data,
		    hb_transform_t<ദശാംശം>::skewing (-sx * HB_PI, sy * HB_PI));
  }
  ശൂന്യം തള്ളുക_skew_around_center (ശൂന്യം *paint_data,
				ദശാംശം sx, ദശാംശം sy,
				ദശാംശം cx, ദശാംശം cy)
  {
    തള്ളുക_transform (paint_data,
		    hb_transform_t<ദശാംശം>::skewing_around_center (-sx * HB_PI, sy * HB_PI, cx, cy));
  }
};
DECLARE_NULL_INSTANCE (hb_paint_funcs_t);


/* Linearly interpolate between two hb_color_t values, component-wise,
 * in byte-channel space with rounding. */
സ്ഥിതം വരിയിൽ hb_color_t
hb_color_lerp (hb_color_t c0, hb_color_t c1, ദശാംശം t)
{
  auto lerp = [&] (unsigned shift) -> unsigned {
    unsigned v0 = (c0 >> shift) & 0xFF;
    unsigned v1 = (c1 >> shift) & 0xFF;
    ദശാംശം v = v0 + t * ((ദശാംശം) v1 - (ദശാംശം) v0) + 0.5f;
    /* t may be outside [0,1] (or NaN) for malformed gradients;
     * keep the float-to-unsigned conversion defined. */
    എങ്കിൽ (!(v > 0.f)) തിരികെ 0;
    എങ്കിൽ (v > 255.f) തിരികെ 255;
    തിരികെ (unsigned) v;
  };
  തിരികെ HB_COLOR (lerp (0), lerp (8), lerp (16), lerp (24));
}

#endif /* HB_PAINT_HH */

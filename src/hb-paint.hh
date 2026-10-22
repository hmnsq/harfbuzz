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

#અન્તર્ભાવ "hb.hh"
#અન્તર્ભાવ "hb-face.hh"
#અન્તર્ભાવ "hb-font.hh"
#અન્તર્ભાવ "hb-geometry.hh"

#define HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS \
  HB_PAINT_FUNC_IMPLEMENT (ધકેલો_transform) \
  HB_PAINT_FUNC_IMPLEMENT (કાઢો_transform) \
  HB_PAINT_FUNC_IMPLEMENT (color_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (ધકેલો_clip_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (ધકેલો_clip_rectangle) \
  HB_PAINT_FUNC_IMPLEMENT (ધકેલો_clip_path_start) \
  HB_PAINT_FUNC_IMPLEMENT (ધકેલો_clip_path_end) \
  HB_PAINT_FUNC_IMPLEMENT (કાઢો_clip) \
  HB_PAINT_FUNC_IMPLEMENT (color) \
  HB_PAINT_FUNC_IMPLEMENT (image) \
  HB_PAINT_FUNC_IMPLEMENT (linear_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (radial_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (sweep_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (ધકેલો_group) \
  HB_PAINT_FUNC_IMPLEMENT (ધકેલો_group_for) \
  HB_PAINT_FUNC_IMPLEMENT (કાઢો_group) \
  HB_PAINT_FUNC_IMPLEMENT (custom_palette_color) \
  HB_PAINT_FUNC_IMPLEMENT (fill_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (set_budget) \
  HB_PAINT_FUNC_IMPLEMENT (get_budget) \
  HB_PAINT_FUNC_IMPLEMENT (get_budget_remaining) \
  /* ^--- Add new callbacks here */

સંરચના hb_paint_funcs_t
{
  hb_object_header_t header;

  સંરચના {
#define HB_PAINT_FUNC_IMPLEMENT(name) hb_paint_##name##_func_t name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_PAINT_FUNC_IMPLEMENT
  } func;

  સંરચના {
#define HB_PAINT_FUNC_IMPLEMENT(name) શૂન્ય *name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_PAINT_FUNC_IMPLEMENT
  } *user_data;

  સંરચના {
#define HB_PAINT_FUNC_IMPLEMENT(name) hb_destroy_func_t name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_PAINT_FUNC_IMPLEMENT
  } *destroy;

  શૂન્ય ધકેલો_transform (શૂન્ય *paint_data,
                       દશાંશ xx, દશાંશ yx,
                       દશાંશ xy, દશાંશ yy,
                       દશાંશ dx, દશાંશ dy)
  {
    // Handle -0.f to avoid -0.f == 0.f in the transform matrix.
    જો (dx == -0.f) dx = 0.f;
    જો (dy == -0.f) dy = 0.f;
    func.ધકેલો_transform (આ, paint_data,
                         xx, yx, xy, yy, dx, dy,
                         !user_data ? nullptr : user_data->ધકેલો_transform); }
  શૂન્ય કાઢો_transform (શૂન્ય *paint_data)
  { func.કાઢો_transform (આ, paint_data,
                        !user_data ? nullptr : user_data->કાઢો_transform); }
  તાર્કિક color_glyph (શૂન્ય *paint_data,
                    hb_codepoint_t glyph,
                    hb_font_t *font)
  { પાછા func.color_glyph (આ, paint_data,
                             glyph,
                             font,
                             !user_data ? nullptr : user_data->color_glyph); }
  શૂન્ય ધકેલો_clip_glyph (શૂન્ય *paint_data,
                        hb_codepoint_t glyph,
                        hb_font_t *font)
  { func.ધકેલો_clip_glyph (આ, paint_data,
                          glyph,
                          font,
                          !user_data ? nullptr : user_data->ધકેલો_clip_glyph); }
  શૂન્ય ધકેલો_clip_rectangle (શૂન્ય *paint_data,
                           દશાંશ xmin, દશાંશ ymin, દશાંશ xmax, દશાંશ ymax)
  { func.ધકેલો_clip_rectangle (આ, paint_data,
                              xmin, ymin, xmax, ymax,
                              !user_data ? nullptr : user_data->ધકેલો_clip_rectangle); }
  hb_draw_funcs_t *ધકેલો_clip_path_start (શૂન્ય *paint_data,
                                         શૂન્ય **draw_data)
  { પાછા func.ધકેલો_clip_path_start (આ, paint_data, draw_data,
                                      !user_data ? nullptr : user_data->ધકેલો_clip_path_start); }
  શૂન્ય ધકેલો_clip_path_end (શૂન્ય *paint_data)
  { func.ધકેલો_clip_path_end (આ, paint_data,
                             !user_data ? nullptr : user_data->ધકેલો_clip_path_end); }
  શૂન્ય કાઢો_clip (શૂન્ય *paint_data)
  { func.કાઢો_clip (આ, paint_data,
                   !user_data ? nullptr : user_data->કાઢો_clip); }
  શૂન્ય color (શૂન્ય *paint_data,
              hb_bool_t is_foreground,
              hb_color_t color)
  { func.color (આ, paint_data,
                is_foreground, color,
                !user_data ? nullptr : user_data->color); }
  શૂન્ય fill_glyph (શૂન્ય *paint_data,
                   hb_codepoint_t glyph,
                   hb_font_t *font,
                   hb_bool_t is_foreground,
                   hb_color_t color)
  { func.fill_glyph (આ, paint_data,
                     glyph, font,
                     is_foreground, color,
                     !user_data ? nullptr : user_data->fill_glyph); }
  તાર્કિક image (શૂન્ય *paint_data,
              hb_blob_t *image,
              unsigned width, unsigned height,
              hb_tag_t format,
              દશાંશ slant,
              hb_glyph_extents_t *extents)
  { પાછા func.image (આ, paint_data,
                       image, width, height, format, slant, extents,
                       !user_data ? nullptr : user_data->image); }
  શૂન્ય linear_gradient (શૂન્ય *paint_data,
                        hb_color_લીટી_t *color_લીટી,
                        દશાંશ x0, દશાંશ y0,
                        દશાંશ x1, દશાંશ y1,
                        દશાંશ x2, દશાંશ y2)
  { func.linear_gradient (આ, paint_data,
                          color_લીટી, x0, y0, x1, y1, x2, y2,
                          !user_data ? nullptr : user_data->linear_gradient); }
  શૂન્ય radial_gradient (શૂન્ય *paint_data,
                        hb_color_લીટી_t *color_લીટી,
                        દશાંશ x0, દશાંશ y0, દશાંશ r0,
                        દશાંશ x1, દશાંશ y1, દશાંશ r1)
  { func.radial_gradient (આ, paint_data,
                          color_લીટી, x0, y0, r0, x1, y1, r1,
                          !user_data ? nullptr : user_data->radial_gradient); }
  શૂન્ય sweep_gradient (શૂન્ય *paint_data,
                       hb_color_લીટી_t *color_લીટી,
                       દશાંશ x0, દશાંશ y0,
                       દશાંશ start_angle,
                       દશાંશ end_angle)
  { func.sweep_gradient (આ, paint_data,
                         color_લીટી, x0, y0, start_angle, end_angle,
                         !user_data ? nullptr : user_data->sweep_gradient); }
  શૂન્ય ધકેલો_group (શૂન્ય *paint_data)
  { func.ધકેલો_group (આ, paint_data,
                     !user_data ? nullptr : user_data->ધકેલો_group); }
  શૂન્ય ધકેલો_group_for (શૂન્ય *paint_data,
                       hb_paint_composite_mode_t mode)
  { func.ધકેલો_group_for (આ, paint_data,
                         mode,
                         !user_data ? nullptr : user_data->ધકેલો_group_for); }
  શૂન્ય કાઢો_group (શૂન્ય *paint_data,
                  hb_paint_composite_mode_t mode)
  { func.કાઢો_group (આ, paint_data,
                    mode,
                    !user_data ? nullptr : user_data->કાઢો_group); }
  તાર્કિક custom_palette_color (શૂન્ય *paint_data,
                             unsigned પૂર્ણાંક color_index,
                             hb_color_t *color)
  { પાછા func.custom_palette_color (આ, paint_data,
                                      color_index,
                                      color,
                                      !user_data ? nullptr : user_data->custom_palette_color); }
  તાર્કિક set_budget (શૂન્ય *paint_data, int64_t budget)
  {
    જો (budget < 0 && budget != HB_BUDGET_DEFAULT)
      budget = 0;
    પાછા func.set_budget (આ, paint_data, budget,
			    !user_data ? nullptr : user_data->set_budget);
  }
  int64_t get_budget (શૂન્ય *paint_data)
  { પાછા func.get_budget (આ, paint_data,
			    !user_data ? nullptr : user_data->get_budget); }
  int64_t *get_budget_remaining_ptr (શૂન્ય *paint_data)
  { પાછા func.get_budget_remaining (આ, paint_data,
				      !user_data ? nullptr : user_data->get_budget_remaining); }


  /* Internal specializations. */

  શૂન્ય ધકેલો_font_transform (શૂન્ય *paint_data,
                            અચલ hb_font_t *font)
  {
    દશાંશ upem = font->face->get_upem ();
    પૂર્ણાંક xscale = font->x_scale, yscale = font->y_scale;

    ધકેલો_transform (paint_data,
		    xscale/upem, 0,
		    0, yscale/upem,
		    0, 0);
  }

  શૂન્ય ધકેલો_inverse_font_transform (શૂન્ય *paint_data,
                                    અચલ hb_font_t *font)
  {
    દશાંશ upem = font->face->get_upem ();
    પૂર્ણાંક xscale = font->x_scale ? font->x_scale : upem;
    પૂર્ણાંક yscale = font->y_scale ? font->y_scale : upem;

    ધકેલો_transform (paint_data,
		    upem/xscale, 0,
		    0, upem/yscale,
		    0, 0);
  }

  શૂન્ય ધકેલો_transform (શૂન્ય *paint_data, hb_transform_t<દશાંશ> t)
  {
    ધકેલો_transform (paint_data, t.xx, t.yx, t.xy, t.yy, t.x0, t.y0);
  }

  શૂન્ય ધકેલો_translate (શૂન્ય *paint_data,
                       દશાંશ dx, દશાંશ dy)
  {
    ધકેલો_transform (paint_data,
		    hb_transform_t<દશાંશ>::translation (dx, dy));
  }

  શૂન્ય ધકેલો_scale (શૂન્ય *paint_data,
                   દશાંશ sx, દશાંશ sy)
  {
    ધકેલો_transform (paint_data,
		    hb_transform_t<દશાંશ>::scaling (sx, sy));
  }
  શૂન્ય ધકેલો_scale_around_center (શૂન્ય *paint_data,
				 દશાંશ sx, દશાંશ sy,
				 દશાંશ cx, દશાંશ cy)
  {
    ધકેલો_transform (paint_data,
		    hb_transform_t<દશાંશ>::scaling_around_center (sx, sy, cx, cy));
  }

  શૂન્ય ધકેલો_rotate (શૂન્ય *paint_data,
                    દશાંશ a)
  {
    ધકેલો_transform (paint_data,
		    hb_transform_t<દશાંશ>::rotation (a * HB_PI));
  }

  શૂન્ય ધકેલો_rotate_around_center (શૂન્ય *paint_data,
				  દશાંશ a,
				  દશાંશ cx, દશાંશ cy)
  {
    ધકેલો_transform (paint_data,
		    hb_transform_t<દશાંશ>::rotation_around_center (a * HB_PI, cx, cy));
  }

  શૂન્ય ધકેલો_skew (શૂન્ય *paint_data,
                  દશાંશ sx, દશાંશ sy)
  {
    ધકેલો_transform (paint_data,
		    hb_transform_t<દશાંશ>::skewing (-sx * HB_PI, sy * HB_PI));
  }
  શૂન્ય ધકેલો_skew_around_center (શૂન્ય *paint_data,
				દશાંશ sx, દશાંશ sy,
				દશાંશ cx, દશાંશ cy)
  {
    ધકેલો_transform (paint_data,
		    hb_transform_t<દશાંશ>::skewing_around_center (-sx * HB_PI, sy * HB_PI, cx, cy));
  }
};
DECLARE_NULL_INSTANCE (hb_paint_funcs_t);


/* Linearly interpolate between two hb_color_t values, component-wise,
 * in byte-channel space with rounding. */
સ્થિર અંતઃરેખ hb_color_t
hb_color_lerp (hb_color_t c0, hb_color_t c1, દશાંશ t)
{
  auto lerp = [&] (unsigned shift) -> unsigned {
    unsigned v0 = (c0 >> shift) & 0xFF;
    unsigned v1 = (c1 >> shift) & 0xFF;
    દશાંશ v = v0 + t * ((દશાંશ) v1 - (દશાંશ) v0) + 0.5f;
    /* t may be outside [0,1] (or NaN) for malformed gradients;
     * keep the float-to-unsigned conversion defined. */
    જો (!(v > 0.f)) પાછા 0;
    જો (v > 255.f) પાછા 255;
    પાછા (unsigned) v;
  };
  પાછા HB_COLOR (lerp (0), lerp (8), lerp (16), lerp (24));
}

#endif /* HB_PAINT_HH */

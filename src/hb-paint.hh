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

#அந்தர்பாவ "hb.hh"
#அந்தர்பாவ "hb-face.hh"
#அந்தர்பாவ "hb-font.hh"
#அந்தர்பாவ "hb-geometry.hh"

#define HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS \
  HB_PAINT_FUNC_IMPLEMENT (தள்ளு_transform) \
  HB_PAINT_FUNC_IMPLEMENT (எடு_transform) \
  HB_PAINT_FUNC_IMPLEMENT (color_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (தள்ளு_clip_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (தள்ளு_clip_rectangle) \
  HB_PAINT_FUNC_IMPLEMENT (தள்ளு_clip_path_start) \
  HB_PAINT_FUNC_IMPLEMENT (தள்ளு_clip_path_end) \
  HB_PAINT_FUNC_IMPLEMENT (எடு_clip) \
  HB_PAINT_FUNC_IMPLEMENT (color) \
  HB_PAINT_FUNC_IMPLEMENT (image) \
  HB_PAINT_FUNC_IMPLEMENT (linear_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (radial_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (sweep_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (தள்ளு_group) \
  HB_PAINT_FUNC_IMPLEMENT (தள்ளு_group_for) \
  HB_PAINT_FUNC_IMPLEMENT (எடு_group) \
  HB_PAINT_FUNC_IMPLEMENT (custom_palette_color) \
  HB_PAINT_FUNC_IMPLEMENT (fill_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (set_budget) \
  HB_PAINT_FUNC_IMPLEMENT (get_budget) \
  HB_PAINT_FUNC_IMPLEMENT (get_budget_remaining) \
  /* ^--- Add new callbacks here */

கட்டமைப்பு hb_paint_funcs_t
{
  hb_object_header_t header;

  கட்டமைப்பு {
#define HB_PAINT_FUNC_IMPLEMENT(name) hb_paint_##name##_func_t name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_PAINT_FUNC_IMPLEMENT
  } func;

  கட்டமைப்பு {
#define HB_PAINT_FUNC_IMPLEMENT(name) வெறுமை *name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_PAINT_FUNC_IMPLEMENT
  } *user_data;

  கட்டமைப்பு {
#define HB_PAINT_FUNC_IMPLEMENT(name) hb_destroy_func_t name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_PAINT_FUNC_IMPLEMENT
  } *destroy;

  வெறுமை தள்ளு_transform (வெறுமை *paint_data,
                       தசம xx, தசம yx,
                       தசம xy, தசம yy,
                       தசம dx, தசம dy)
  {
    // Handle -0.f to avoid -0.f == 0.f in the transform matrix.
    எனில் (dx == -0.f) dx = 0.f;
    எனில் (dy == -0.f) dy = 0.f;
    func.தள்ளு_transform (இது, paint_data,
                         xx, yx, xy, yy, dx, dy,
                         !user_data ? nullptr : user_data->தள்ளு_transform); }
  வெறுமை எடு_transform (வெறுமை *paint_data)
  { func.எடு_transform (இது, paint_data,
                        !user_data ? nullptr : user_data->எடு_transform); }
  தருக்க color_glyph (வெறுமை *paint_data,
                    hb_codepoint_t glyph,
                    hb_font_t *font)
  { திரும்பு func.color_glyph (இது, paint_data,
                             glyph,
                             font,
                             !user_data ? nullptr : user_data->color_glyph); }
  வெறுமை தள்ளு_clip_glyph (வெறுமை *paint_data,
                        hb_codepoint_t glyph,
                        hb_font_t *font)
  { func.தள்ளு_clip_glyph (இது, paint_data,
                          glyph,
                          font,
                          !user_data ? nullptr : user_data->தள்ளு_clip_glyph); }
  வெறுமை தள்ளு_clip_rectangle (வெறுமை *paint_data,
                           தசம xmin, தசம ymin, தசம xmax, தசம ymax)
  { func.தள்ளு_clip_rectangle (இது, paint_data,
                              xmin, ymin, xmax, ymax,
                              !user_data ? nullptr : user_data->தள்ளு_clip_rectangle); }
  hb_draw_funcs_t *தள்ளு_clip_path_start (வெறுமை *paint_data,
                                         வெறுமை **draw_data)
  { திரும்பு func.தள்ளு_clip_path_start (இது, paint_data, draw_data,
                                      !user_data ? nullptr : user_data->தள்ளு_clip_path_start); }
  வெறுமை தள்ளு_clip_path_end (வெறுமை *paint_data)
  { func.தள்ளு_clip_path_end (இது, paint_data,
                             !user_data ? nullptr : user_data->தள்ளு_clip_path_end); }
  வெறுமை எடு_clip (வெறுமை *paint_data)
  { func.எடு_clip (இது, paint_data,
                   !user_data ? nullptr : user_data->எடு_clip); }
  வெறுமை color (வெறுமை *paint_data,
              hb_bool_t is_foreground,
              hb_color_t color)
  { func.color (இது, paint_data,
                is_foreground, color,
                !user_data ? nullptr : user_data->color); }
  வெறுமை fill_glyph (வெறுமை *paint_data,
                   hb_codepoint_t glyph,
                   hb_font_t *font,
                   hb_bool_t is_foreground,
                   hb_color_t color)
  { func.fill_glyph (இது, paint_data,
                     glyph, font,
                     is_foreground, color,
                     !user_data ? nullptr : user_data->fill_glyph); }
  தருக்க image (வெறுமை *paint_data,
              hb_blob_t *image,
              unsigned width, unsigned height,
              hb_tag_t format,
              தசம slant,
              hb_glyph_extents_t *extents)
  { திரும்பு func.image (இது, paint_data,
                       image, width, height, format, slant, extents,
                       !user_data ? nullptr : user_data->image); }
  வெறுமை linear_gradient (வெறுமை *paint_data,
                        hb_color_வரி_t *color_வரி,
                        தசம x0, தசம y0,
                        தசம x1, தசம y1,
                        தசம x2, தசம y2)
  { func.linear_gradient (இது, paint_data,
                          color_வரி, x0, y0, x1, y1, x2, y2,
                          !user_data ? nullptr : user_data->linear_gradient); }
  வெறுமை radial_gradient (வெறுமை *paint_data,
                        hb_color_வரி_t *color_வரி,
                        தசம x0, தசம y0, தசம r0,
                        தசம x1, தசம y1, தசம r1)
  { func.radial_gradient (இது, paint_data,
                          color_வரி, x0, y0, r0, x1, y1, r1,
                          !user_data ? nullptr : user_data->radial_gradient); }
  வெறுமை sweep_gradient (வெறுமை *paint_data,
                       hb_color_வரி_t *color_வரி,
                       தசம x0, தசம y0,
                       தசம start_angle,
                       தசம end_angle)
  { func.sweep_gradient (இது, paint_data,
                         color_வரி, x0, y0, start_angle, end_angle,
                         !user_data ? nullptr : user_data->sweep_gradient); }
  வெறுமை தள்ளு_group (வெறுமை *paint_data)
  { func.தள்ளு_group (இது, paint_data,
                     !user_data ? nullptr : user_data->தள்ளு_group); }
  வெறுமை தள்ளு_group_for (வெறுமை *paint_data,
                       hb_paint_composite_mode_t mode)
  { func.தள்ளு_group_for (இது, paint_data,
                         mode,
                         !user_data ? nullptr : user_data->தள்ளு_group_for); }
  வெறுமை எடு_group (வெறுமை *paint_data,
                  hb_paint_composite_mode_t mode)
  { func.எடு_group (இது, paint_data,
                    mode,
                    !user_data ? nullptr : user_data->எடு_group); }
  தருக்க custom_palette_color (வெறுமை *paint_data,
                             unsigned முழு color_index,
                             hb_color_t *color)
  { திரும்பு func.custom_palette_color (இது, paint_data,
                                      color_index,
                                      color,
                                      !user_data ? nullptr : user_data->custom_palette_color); }
  தருக்க set_budget (வெறுமை *paint_data, int64_t budget)
  {
    எனில் (budget < 0 && budget != HB_BUDGET_DEFAULT)
      budget = 0;
    திரும்பு func.set_budget (இது, paint_data, budget,
			    !user_data ? nullptr : user_data->set_budget);
  }
  int64_t get_budget (வெறுமை *paint_data)
  { திரும்பு func.get_budget (இது, paint_data,
			    !user_data ? nullptr : user_data->get_budget); }
  int64_t *get_budget_remaining_ptr (வெறுமை *paint_data)
  { திரும்பு func.get_budget_remaining (இது, paint_data,
				      !user_data ? nullptr : user_data->get_budget_remaining); }


  /* Internal specializations. */

  வெறுமை தள்ளு_font_transform (வெறுமை *paint_data,
                            மாறிலி hb_font_t *font)
  {
    தசம upem = font->face->get_upem ();
    முழு xscale = font->x_scale, yscale = font->y_scale;

    தள்ளு_transform (paint_data,
		    xscale/upem, 0,
		    0, yscale/upem,
		    0, 0);
  }

  வெறுமை தள்ளு_inverse_font_transform (வெறுமை *paint_data,
                                    மாறிலி hb_font_t *font)
  {
    தசம upem = font->face->get_upem ();
    முழு xscale = font->x_scale ? font->x_scale : upem;
    முழு yscale = font->y_scale ? font->y_scale : upem;

    தள்ளு_transform (paint_data,
		    upem/xscale, 0,
		    0, upem/yscale,
		    0, 0);
  }

  வெறுமை தள்ளு_transform (வெறுமை *paint_data, hb_transform_t<தசம> t)
  {
    தள்ளு_transform (paint_data, t.xx, t.yx, t.xy, t.yy, t.x0, t.y0);
  }

  வெறுமை தள்ளு_translate (வெறுமை *paint_data,
                       தசம dx, தசம dy)
  {
    தள்ளு_transform (paint_data,
		    hb_transform_t<தசம>::translation (dx, dy));
  }

  வெறுமை தள்ளு_scale (வெறுமை *paint_data,
                   தசம sx, தசம sy)
  {
    தள்ளு_transform (paint_data,
		    hb_transform_t<தசம>::scaling (sx, sy));
  }
  வெறுமை தள்ளு_scale_around_center (வெறுமை *paint_data,
				 தசம sx, தசம sy,
				 தசம cx, தசம cy)
  {
    தள்ளு_transform (paint_data,
		    hb_transform_t<தசம>::scaling_around_center (sx, sy, cx, cy));
  }

  வெறுமை தள்ளு_rotate (வெறுமை *paint_data,
                    தசம a)
  {
    தள்ளு_transform (paint_data,
		    hb_transform_t<தசம>::rotation (a * HB_PI));
  }

  வெறுமை தள்ளு_rotate_around_center (வெறுமை *paint_data,
				  தசம a,
				  தசம cx, தசம cy)
  {
    தள்ளு_transform (paint_data,
		    hb_transform_t<தசம>::rotation_around_center (a * HB_PI, cx, cy));
  }

  வெறுமை தள்ளு_skew (வெறுமை *paint_data,
                  தசம sx, தசம sy)
  {
    தள்ளு_transform (paint_data,
		    hb_transform_t<தசம>::skewing (-sx * HB_PI, sy * HB_PI));
  }
  வெறுமை தள்ளு_skew_around_center (வெறுமை *paint_data,
				தசம sx, தசம sy,
				தசம cx, தசம cy)
  {
    தள்ளு_transform (paint_data,
		    hb_transform_t<தசம>::skewing_around_center (-sx * HB_PI, sy * HB_PI, cx, cy));
  }
};
DECLARE_NULL_INSTANCE (hb_paint_funcs_t);


/* Linearly interpolate between two hb_color_t values, component-wise,
 * in byte-channel space with rounding. */
நிலையான உள்வரி hb_color_t
hb_color_lerp (hb_color_t c0, hb_color_t c1, தசம t)
{
  auto lerp = [&] (unsigned shift) -> unsigned {
    unsigned v0 = (c0 >> shift) & 0xFF;
    unsigned v1 = (c1 >> shift) & 0xFF;
    தசம v = v0 + t * ((தசம) v1 - (தசம) v0) + 0.5f;
    /* t may be outside [0,1] (or NaN) for malformed gradients;
     * keep the float-to-unsigned conversion defined. */
    எனில் (!(v > 0.f)) திரும்பு 0;
    எனில் (v > 255.f) திரும்பு 255;
    திரும்பு (unsigned) v;
  };
  திரும்பு HB_COLOR (lerp (0), lerp (8), lerp (16), lerp (24));
}

#endif /* HB_PAINT_HH */

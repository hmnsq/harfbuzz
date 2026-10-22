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

কাঠামো hb_paint_funcs_t
{
  hb_object_header_t header;

  কাঠামো {
#define HB_PAINT_FUNC_IMPLEMENT(name) hb_paint_##name##_func_t name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_PAINT_FUNC_IMPLEMENT
  } func;

  কাঠামো {
#define HB_PAINT_FUNC_IMPLEMENT(name) শূন্য *name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_PAINT_FUNC_IMPLEMENT
  } *user_তথ্য;

  কাঠামো {
#define HB_PAINT_FUNC_IMPLEMENT(name) hb_destroy_func_t name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_PAINT_FUNC_IMPLEMENT
  } *destroy;

  শূন্য ঠেলো_transform (শূন্য *paint_তথ্য,
                       দশমিক xx, দশমিক yx,
                       দশমিক xy, দশমিক yy,
                       দশমিক dx, দশমিক dy)
  {
    // Handle -0.f to avoid -0.f == 0.f in the transform matrix.
    যদি (dx == -0.f) dx = 0.f;
    যদি (dy == -0.f) dy = 0.f;
    func.ঠেলো_transform (এই, paint_তথ্য,
                         xx, yx, xy, yy, dx, dy,
                         !user_তথ্য ? nullptr : user_তথ্য->ঠেলো_transform); }
  শূন্য বের_করো_transform (শূন্য *paint_তথ্য)
  { func.বের_করো_transform (এই, paint_তথ্য,
                        !user_তথ্য ? nullptr : user_তথ্য->বের_করো_transform); }
  যৌক্তিক color_glyph (শূন্য *paint_তথ্য,
                    hb_codepoint_t glyph,
                    hb_font_t *font)
  { ফেরত func.color_glyph (এই, paint_তথ্য,
                             glyph,
                             font,
                             !user_তথ্য ? nullptr : user_তথ্য->color_glyph); }
  শূন্য ঠেলো_clip_glyph (শূন্য *paint_তথ্য,
                        hb_codepoint_t glyph,
                        hb_font_t *font)
  { func.ঠেলো_clip_glyph (এই, paint_তথ্য,
                          glyph,
                          font,
                          !user_তথ্য ? nullptr : user_তথ্য->ঠেলো_clip_glyph); }
  শূন্য ঠেলো_clip_rectangle (শূন্য *paint_তথ্য,
                           দশমিক xmin, দশমিক ymin, দশমিক xmax, দশমিক ymax)
  { func.ঠেলো_clip_rectangle (এই, paint_তথ্য,
                              xmin, ymin, xmax, ymax,
                              !user_তথ্য ? nullptr : user_তথ্য->ঠেলো_clip_rectangle); }
  hb_draw_funcs_t *ঠেলো_clip_path_start (শূন্য *paint_তথ্য,
                                         শূন্য **draw_তথ্য)
  { ফেরত func.ঠেলো_clip_path_start (এই, paint_তথ্য, draw_তথ্য,
                                      !user_তথ্য ? nullptr : user_তথ্য->ঠেলো_clip_path_start); }
  শূন্য ঠেলো_clip_path_end (শূন্য *paint_তথ্য)
  { func.ঠেলো_clip_path_end (এই, paint_তথ্য,
                             !user_তথ্য ? nullptr : user_তথ্য->ঠেলো_clip_path_end); }
  শূন্য বের_করো_clip (শূন্য *paint_তথ্য)
  { func.বের_করো_clip (এই, paint_তথ্য,
                   !user_তথ্য ? nullptr : user_তথ্য->বের_করো_clip); }
  শূন্য color (শূন্য *paint_তথ্য,
              hb_bool_t is_foreground,
              hb_color_t color)
  { func.color (এই, paint_তথ্য,
                is_foreground, color,
                !user_তথ্য ? nullptr : user_তথ্য->color); }
  শূন্য fill_glyph (শূন্য *paint_তথ্য,
                   hb_codepoint_t glyph,
                   hb_font_t *font,
                   hb_bool_t is_foreground,
                   hb_color_t color)
  { func.fill_glyph (এই, paint_তথ্য,
                     glyph, font,
                     is_foreground, color,
                     !user_তথ্য ? nullptr : user_তথ্য->fill_glyph); }
  যৌক্তিক image (শূন্য *paint_তথ্য,
              hb_blob_t *image,
              unsigned width, unsigned height,
              hb_tag_t format,
              দশমিক slant,
              hb_glyph_extents_t *extents)
  { ফেরত func.image (এই, paint_তথ্য,
                       image, width, height, format, slant, extents,
                       !user_তথ্য ? nullptr : user_তথ্য->image); }
  শূন্য linear_gradient (শূন্য *paint_তথ্য,
                        hb_color_সারি_t *color_সারি,
                        দশমিক x0, দশমিক y0,
                        দশমিক x1, দশমিক y1,
                        দশমিক x2, দশমিক y2)
  { func.linear_gradient (এই, paint_তথ্য,
                          color_সারি, x0, y0, x1, y1, x2, y2,
                          !user_তথ্য ? nullptr : user_তথ্য->linear_gradient); }
  শূন্য radial_gradient (শূন্য *paint_তথ্য,
                        hb_color_সারি_t *color_সারি,
                        দশমিক x0, দশমিক y0, দশমিক r0,
                        দশমিক x1, দশমিক y1, দশমিক r1)
  { func.radial_gradient (এই, paint_তথ্য,
                          color_সারি, x0, y0, r0, x1, y1, r1,
                          !user_তথ্য ? nullptr : user_তথ্য->radial_gradient); }
  শূন্য sweep_gradient (শূন্য *paint_তথ্য,
                       hb_color_সারি_t *color_সারি,
                       দশমিক x0, দশমিক y0,
                       দশমিক start_angle,
                       দশমিক end_angle)
  { func.sweep_gradient (এই, paint_তথ্য,
                         color_সারি, x0, y0, start_angle, end_angle,
                         !user_তথ্য ? nullptr : user_তথ্য->sweep_gradient); }
  শূন্য ঠেলো_group (শূন্য *paint_তথ্য)
  { func.ঠেলো_group (এই, paint_তথ্য,
                     !user_তথ্য ? nullptr : user_তথ্য->ঠেলো_group); }
  শূন্য ঠেলো_group_for (শূন্য *paint_তথ্য,
                       hb_paint_composite_mode_t mode)
  { func.ঠেলো_group_for (এই, paint_তথ্য,
                         mode,
                         !user_তথ্য ? nullptr : user_তথ্য->ঠেলো_group_for); }
  শূন্য বের_করো_group (শূন্য *paint_তথ্য,
                  hb_paint_composite_mode_t mode)
  { func.বের_করো_group (এই, paint_তথ্য,
                    mode,
                    !user_তথ্য ? nullptr : user_তথ্য->বের_করো_group); }
  যৌক্তিক custom_palette_color (শূন্য *paint_তথ্য,
                             unsigned পূর্ণসংখ্যা color_সূচক,
                             hb_color_t *color)
  { ফেরত func.custom_palette_color (এই, paint_তথ্য,
                                      color_সূচক,
                                      color,
                                      !user_তথ্য ? nullptr : user_তথ্য->custom_palette_color); }
  যৌক্তিক set_budget (শূন্য *paint_তথ্য, int64_t budget)
  {
    যদি (budget < 0 && budget != HB_BUDGET_DEFAULT)
      budget = 0;
    ফেরত func.set_budget (এই, paint_তথ্য, budget,
			    !user_তথ্য ? nullptr : user_তথ্য->set_budget);
  }
  int64_t get_budget (শূন্য *paint_তথ্য)
  { ফেরত func.get_budget (এই, paint_তথ্য,
			    !user_তথ্য ? nullptr : user_তথ্য->get_budget); }
  int64_t *get_budget_remaining_ptr (শূন্য *paint_তথ্য)
  { ফেরত func.get_budget_remaining (এই, paint_তথ্য,
				      !user_তথ্য ? nullptr : user_তথ্য->get_budget_remaining); }


  /* Internal specializations. */

  শূন্য ঠেলো_font_transform (শূন্য *paint_তথ্য,
                            ধ্রুবক hb_font_t *font)
  {
    দশমিক upem = font->face->get_upem ();
    পূর্ণসংখ্যা xscale = font->x_scale, yscale = font->y_scale;

    ঠেলো_transform (paint_তথ্য,
		    xscale/upem, 0,
		    0, yscale/upem,
		    0, 0);
  }

  শূন্য ঠেলো_inverse_font_transform (শূন্য *paint_তথ্য,
                                    ধ্রুবক hb_font_t *font)
  {
    দশমিক upem = font->face->get_upem ();
    পূর্ণসংখ্যা xscale = font->x_scale ? font->x_scale : upem;
    পূর্ণসংখ্যা yscale = font->y_scale ? font->y_scale : upem;

    ঠেলো_transform (paint_তথ্য,
		    upem/xscale, 0,
		    0, upem/yscale,
		    0, 0);
  }

  শূন্য ঠেলো_transform (শূন্য *paint_তথ্য, hb_transform_t<দশমিক> t)
  {
    ঠেলো_transform (paint_তথ্য, t.xx, t.yx, t.xy, t.yy, t.x0, t.y0);
  }

  শূন্য ঠেলো_translate (শূন্য *paint_তথ্য,
                       দশমিক dx, দশমিক dy)
  {
    ঠেলো_transform (paint_তথ্য,
		    hb_transform_t<দশমিক>::translation (dx, dy));
  }

  শূন্য ঠেলো_scale (শূন্য *paint_তথ্য,
                   দশমিক sx, দশমিক sy)
  {
    ঠেলো_transform (paint_তথ্য,
		    hb_transform_t<দশমিক>::scaling (sx, sy));
  }
  শূন্য ঠেলো_scale_around_center (শূন্য *paint_তথ্য,
				 দশমিক sx, দশমিক sy,
				 দশমিক cx, দশমিক cy)
  {
    ঠেলো_transform (paint_তথ্য,
		    hb_transform_t<দশমিক>::scaling_around_center (sx, sy, cx, cy));
  }

  শূন্য ঠেলো_rotate (শূন্য *paint_তথ্য,
                    দশমিক a)
  {
    ঠেলো_transform (paint_তথ্য,
		    hb_transform_t<দশমিক>::rotation (a * HB_PI));
  }

  শূন্য ঠেলো_rotate_around_center (শূন্য *paint_তথ্য,
				  দশমিক a,
				  দশমিক cx, দশমিক cy)
  {
    ঠেলো_transform (paint_তথ্য,
		    hb_transform_t<দশমিক>::rotation_around_center (a * HB_PI, cx, cy));
  }

  শূন্য ঠেলো_skew (শূন্য *paint_তথ্য,
                  দশমিক sx, দশমিক sy)
  {
    ঠেলো_transform (paint_তথ্য,
		    hb_transform_t<দশমিক>::skewing (-sx * HB_PI, sy * HB_PI));
  }
  শূন্য ঠেলো_skew_around_center (শূন্য *paint_তথ্য,
				দশমিক sx, দশমিক sy,
				দশমিক cx, দশমিক cy)
  {
    ঠেলো_transform (paint_তথ্য,
		    hb_transform_t<দশমিক>::skewing_around_center (-sx * HB_PI, sy * HB_PI, cx, cy));
  }
};
DECLARE_NULL_INSTANCE (hb_paint_funcs_t);


/* Linearly interpolate between two hb_color_t values, component-wise,
 * in byte-channel space with rounding. */
স্থির অন্তঃরেখ hb_color_t
hb_color_lerp (hb_color_t c0, hb_color_t c1, দশমিক t)
{
  auto lerp = [&] (unsigned shift) -> unsigned {
    unsigned v0 = (c0 >> shift) & 0xFF;
    unsigned v1 = (c1 >> shift) & 0xFF;
    দশমিক v = v0 + t * ((দশমিক) v1 - (দশমিক) v0) + 0.5f;
    /* t may be outside [0,1] (or NaN) for malformed gradients;
     * keep the float-to-unsigned conversion defined. */
    যদি (!(v > 0.f)) ফেরত 0;
    যদি (v > 255.f) ফেরত 255;
    ফেরত (unsigned) v;
  };
  ফেরত HB_COLOR (lerp (0), lerp (8), lerp (16), lerp (24));
}

#endif /* HB_PAINT_HH */

/*
 * Copyright (C) 2026  Behdad Esfahbod
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
 *
 * Author(s): Behdad Esfahbod
 */

#ifndef HB_VECTOR_H
#define HB_VECTOR_H

#অন্তর্ভাব "hb.h"

HB_BEGIN_DECLS

/**
 * hb_vector_format_t:
 * @HB_VECTOR_FORMAT_INVALID: Invalid format.
 * @HB_VECTOR_FORMAT_SVG: SVG output.
 * @HB_VECTOR_FORMAT_PDF: PDF output.
 *
 * Output format for vector conversion.
 *
 * Since: 13.0.0
 */
প্রকারনির্দেশ গণনা {
  HB_VECTOR_FORMAT_INVALID = HB_TAG_NONE,
  HB_VECTOR_FORMAT_SVG = HB_TAG ('s','v','g',' '),
  HB_VECTOR_FORMAT_PDF = HB_TAG ('p','d','f',' '),
} hb_vector_format_t;

/**
 * hb_vector_extents_t:
 * @x: Left edge of the output coordinate system.
 * @y: Top edge of the output coordinate system.
 * @width: Width of the output coordinate system.
 * @height: Height of the output coordinate system.
 *
 * Vector output extents, mapped to SVG viewBox.
 *
 * Since: 13.0.0
 */
প্রকারনির্দেশ সংরচনা hb_vector_extents_t {
  ভিন্ন x, y;
  ভিন্ন width, height;
} hb_vector_extents_t;

/**
 * hb_vector_extents_mode_t:
 * @HB_VECTOR_EXTENTS_MODE_NONE: Do not update extents.
 * @HB_VECTOR_EXTENTS_MODE_EXPAND: Union glyph ink extents into current extents.
 *
 * Controls whether convenience glyph APIs update context extents.
 *
 * Since: 13.0.0
 */
প্রকারনির্দেশ গণনা {
  HB_VECTOR_EXTENTS_MODE_NONE = 0,
  HB_VECTOR_EXTENTS_MODE_EXPAND = 1,
} hb_vector_extents_mode_t;

/* hb_vector_draw_t */

/**
 * hb_vector_draw_t:
 *
 * Opaque draw context for vector outline conversion.
 *
 * Since: 13.0.0
 */
প্রকারনির্দেশ সংরচনা hb_vector_draw_t hb_vector_draw_t;

HB_EXTERN hb_vector_draw_t *
hb_vector_draw_create_or_fail (hb_vector_format_t format);

HB_EXTERN hb_vector_draw_t *
hb_vector_draw_reference (hb_vector_draw_t *draw);

HB_EXTERN শূন্য
hb_vector_draw_destroy (hb_vector_draw_t *draw);

HB_EXTERN hb_bool_t
hb_vector_draw_set_user_data (hb_vector_draw_t   *draw,
                              hb_user_data_চাবি_t *চাবি_2,
                              শূন্য               *data,
                              hb_destroy_func_t   destroy,
                              hb_bool_t           replace);

HB_EXTERN শূন্য *
hb_vector_draw_get_user_data (ধ্রুব hb_vector_draw_t   *draw,
                              hb_user_data_চাবি_t *চাবি_2);

HB_EXTERN শূন্য
hb_vector_draw_set_transform (hb_vector_draw_t *draw,
                              ভিন্ন xx, ভিন্ন yx,
                              ভিন্ন xy, ভিন্ন yy,
                              ভিন্ন dx, ভিন্ন dy);

HB_EXTERN শূন্য
hb_vector_draw_get_transform (ধ্রুব hb_vector_draw_t *draw,
                              ভিন্ন *xx, ভিন্ন *yx,
                              ভিন্ন *xy, ভিন্ন *yy,
                              ভিন্ন *dx, ভিন্ন *dy);

HB_EXTERN শূন্য
hb_vector_draw_set_scale_factor (hb_vector_draw_t *draw,
                                 ভিন্ন x_scale_factor,
                                 ভিন্ন y_scale_factor);

HB_EXTERN শূন্য
hb_vector_draw_get_scale_factor (ধ্রুব hb_vector_draw_t *draw,
                                 ভিন্ন *x_scale_factor,
                                 ভিন্ন *y_scale_factor);

HB_EXTERN শূন্য
hb_vector_draw_set_extents (hb_vector_draw_t *draw,
                            ধ্রুব hb_vector_extents_t *extents);

HB_EXTERN hb_bool_t
hb_vector_draw_get_extents (ধ্রুব hb_vector_draw_t *draw,
                            hb_vector_extents_t *extents);

HB_EXTERN hb_bool_t
hb_vector_draw_set_glyph_extents (hb_vector_draw_t *draw,
                                  ধ্রুব hb_glyph_extents_t *glyph_extents);

HB_EXTERN hb_vector_format_t
hb_vector_draw_get_format (ধ্রুব hb_vector_draw_t *draw);

HB_EXTERN hb_draw_funcs_t *
hb_vector_draw_get_funcs (ধ্রুব hb_vector_draw_t *draw);

HB_EXTERN শূন্য
hb_vector_draw_new_path (hb_vector_draw_t *draw);

HB_EXTERN শূন্য
hb_vector_draw_glyph (hb_vector_draw_t *draw,
                      hb_font_t *font,
                      hb_codepoint_t glyph,
                      hb_vector_extents_mode_t extents_mode);

HB_EXTERN hb_bool_t
hb_vector_draw_glyph_or_fail (hb_vector_draw_t *draw,
                              hb_font_t *font,
                              hb_codepoint_t glyph,
                              hb_vector_extents_mode_t extents_mode);

HB_EXTERN শূন্য
hb_vector_draw_set_precision (hb_vector_draw_t *draw,
                             unsigned precision);

HB_EXTERN unsigned
hb_vector_draw_get_precision (ধ্রুব hb_vector_draw_t *draw);

HB_EXTERN শূন্য
hb_vector_draw_set_foreground (hb_vector_draw_t *draw,
                               hb_color_t foreground);

HB_EXTERN hb_color_t
hb_vector_draw_get_foreground (ধ্রুব hb_vector_draw_t *draw);

HB_EXTERN শূন্য
hb_vector_draw_set_background (hb_vector_draw_t *draw,
                               hb_color_t background);

HB_EXTERN hb_color_t
hb_vector_draw_get_background (ধ্রুব hb_vector_draw_t *draw);

HB_EXTERN hb_blob_t *
hb_vector_draw_render (hb_vector_draw_t *draw);

HB_EXTERN শূন্য
hb_vector_draw_clear (hb_vector_draw_t *draw);

HB_EXTERN শূন্য
hb_vector_draw_reset (hb_vector_draw_t *draw);

HB_EXTERN শূন্য
hb_vector_draw_recycle_blob (hb_vector_draw_t *draw,
                             hb_blob_t *blob);


/* hb_vector_paint_t */

/**
 * hb_vector_paint_t:
 *
 * Opaque paint context for vector color-glyph conversion.
 *
 * Since: 13.0.0
 */
প্রকারনির্দেশ সংরচনা hb_vector_paint_t hb_vector_paint_t;

HB_EXTERN hb_vector_paint_t *
hb_vector_paint_create_or_fail (hb_vector_format_t format);

HB_EXTERN hb_vector_paint_t *
hb_vector_paint_reference (hb_vector_paint_t *paint);

HB_EXTERN শূন্য
hb_vector_paint_destroy (hb_vector_paint_t *paint);

HB_EXTERN hb_bool_t
hb_vector_paint_set_user_data (hb_vector_paint_t  *paint,
                               hb_user_data_চাবি_t *চাবি_2,
                               শূন্য               *data,
                               hb_destroy_func_t   destroy,
                               hb_bool_t           replace);

HB_EXTERN শূন্য *
hb_vector_paint_get_user_data (ধ্রুব hb_vector_paint_t  *paint,
                               hb_user_data_চাবি_t *চাবি_2);

HB_EXTERN শূন্য
hb_vector_paint_set_transform (hb_vector_paint_t *paint,
                               ভিন্ন xx, ভিন্ন yx,
                               ভিন্ন xy, ভিন্ন yy,
                               ভিন্ন dx, ভিন্ন dy);

HB_EXTERN শূন্য
hb_vector_paint_get_transform (ধ্রুব hb_vector_paint_t *paint,
                               ভিন্ন *xx, ভিন্ন *yx,
                               ভিন্ন *xy, ভিন্ন *yy,
                               ভিন্ন *dx, ভিন্ন *dy);

HB_EXTERN শূন্য
hb_vector_paint_set_scale_factor (hb_vector_paint_t *paint,
                                  ভিন্ন x_scale_factor,
                                  ভিন্ন y_scale_factor);

HB_EXTERN শূন্য
hb_vector_paint_get_scale_factor (ধ্রুব hb_vector_paint_t *paint,
                                  ভিন্ন *x_scale_factor,
                                  ভিন্ন *y_scale_factor);

HB_EXTERN শূন্য
hb_vector_paint_set_extents (hb_vector_paint_t *paint,
                             ধ্রুব hb_vector_extents_t *extents);

HB_EXTERN hb_bool_t
hb_vector_paint_get_extents (ধ্রুব hb_vector_paint_t *paint,
                             hb_vector_extents_t *extents);

HB_EXTERN hb_bool_t
hb_vector_paint_set_glyph_extents (hb_vector_paint_t *paint,
                                   ধ্রুব hb_glyph_extents_t *glyph_extents);

HB_EXTERN শূন্য
hb_vector_paint_set_foreground (hb_vector_paint_t *paint,
                                hb_color_t foreground);

HB_EXTERN hb_color_t
hb_vector_paint_get_foreground (ধ্রুব hb_vector_paint_t *paint);

HB_EXTERN শূন্য
hb_vector_paint_set_background (hb_vector_paint_t *paint,
                                hb_color_t background);

HB_EXTERN hb_color_t
hb_vector_paint_get_background (ধ্রুব hb_vector_paint_t *paint);

HB_EXTERN শূন্য
hb_vector_paint_set_palette (hb_vector_paint_t *paint,
                             পূর্ণাঙ্ক palette);

HB_EXTERN পূর্ণাঙ্ক
hb_vector_paint_get_palette (ধ্রুব hb_vector_paint_t *paint);

HB_EXTERN শূন্য
hb_vector_paint_set_custom_palette_color (hb_vector_paint_t *paint,
                                          unsigned color_index,
                                          hb_color_t color);

HB_EXTERN শূন্য
hb_vector_paint_clear_custom_palette_colors (hb_vector_paint_t *paint);

HB_EXTERN hb_vector_format_t
hb_vector_paint_get_format (ধ্রুব hb_vector_paint_t *paint);

HB_EXTERN hb_paint_funcs_t *
hb_vector_paint_get_funcs (ধ্রুব hb_vector_paint_t *paint);

HB_EXTERN শূন্য
hb_vector_paint_glyph (hb_vector_paint_t *paint,
		       hb_font_t         *font,
		       hb_codepoint_t     glyph,
		       hb_vector_extents_mode_t extents_mode);

HB_EXTERN hb_bool_t
hb_vector_paint_glyph_or_fail (hb_vector_paint_t *paint,
			       hb_font_t         *font,
			       hb_codepoint_t     glyph,
			       hb_vector_extents_mode_t extents_mode);

HB_EXTERN শূন্য
hb_vector_paint_set_precision (hb_vector_paint_t *paint,
                                   unsigned precision);

HB_EXTERN unsigned
hb_vector_paint_get_precision (ধ্রুব hb_vector_paint_t *paint);

HB_EXTERN শূন্য
hb_vector_paint_set_svg_prefix (hb_vector_paint_t *paint,
                                ধ্রুব বর্ণ *prefix);

HB_EXTERN ধ্রুব বর্ণ *
hb_vector_paint_get_svg_prefix (ধ্রুব hb_vector_paint_t *paint);

HB_EXTERN hb_blob_t *
hb_vector_paint_render (hb_vector_paint_t *paint);

HB_EXTERN শূন্য
hb_vector_paint_clear (hb_vector_paint_t *paint);

HB_EXTERN শূন্য
hb_vector_paint_reset (hb_vector_paint_t *paint);

HB_EXTERN শূন্য
hb_vector_paint_recycle_blob (hb_vector_paint_t *paint,
                              hb_blob_t *blob);

HB_END_DECLS


#যদি defined(__cplusplus) && defined(HB_CPLUSPLUS_HH)
namespace hb {
HB_DEFINE_VTABLE (vector_draw,  nullptr);
HB_DEFINE_VTABLE (vector_paint, nullptr);
} // namespace hb
#endif

#endif /* HB_VECTOR_H */

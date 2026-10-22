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

#اگر_غیر_تعریف_شدہ HB_سمتیہ_H
#تعریف_کرو HB_سمتیہ_H

#شامل "hb.h"

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
قسمتعریف شمار {
  HB_سمتیہ_FORMAT_INVALID = HB_لیبل_NONE,
  HB_سمتیہ_FORMAT_SVG = HB_لیبل ('s','v','g',' '),
  HB_سمتیہ_FORMAT_PDF = HB_لیبل ('p','d','f',' '),
} hb_سمتیہ_format_t;

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
قسمتعریف ساخت hb_سمتیہ_extents_t {
  اعشاری x, y;
  اعشاری چوڑائی, اونچائی;
} hb_سمتیہ_extents_t;

/**
 * hb_vector_extents_mode_t:
 * @HB_VECTOR_EXTENTS_MODE_NONE: Do not update extents.
 * @HB_VECTOR_EXTENTS_MODE_EXPAND: Union glyph ink extents into current extents.
 *
 * Controls whether convenience glyph APIs update context extents.
 *
 * Since: 13.0.0
 */
قسمتعریف شمار {
  HB_سمتیہ_EXTENTS_منوال_NONE = 0,
  HB_سمتیہ_EXTENTS_منوال_EXPAND = 1,
} hb_سمتیہ_extents_منوال_t;

/* hb_vector_draw_t */

/**
 * hb_vector_draw_t:
 *
 * Opaque draw context for vector outline conversion.
 *
 * Since: 13.0.0
 */
قسمتعریف ساخت hb_سمتیہ_draw_t hb_سمتیہ_draw_t;

HB_EXTERN hb_سمتیہ_draw_t *
hb_سمتیہ_draw_create_or_fail (hb_سمتیہ_format_t format);

HB_EXTERN hb_سمتیہ_draw_t *
hb_سمتیہ_draw_reference (hb_سمتیہ_draw_t *draw);

HB_EXTERN خالی
hb_سمتیہ_draw_destroy (hb_سمتیہ_draw_t *draw);

HB_EXTERN hb_bool_t
hb_سمتیہ_draw_سیٹ_user_ڈیٹا (hb_سمتیہ_draw_t   *draw,
                              hb_user_ڈیٹا_کلید_t *کلید_2,
                              خالی               *ڈیٹا,
                              hb_destroy_func_t   destroy,
                              hb_bool_t           replace);

HB_EXTERN خالی *
hb_سمتیہ_draw_get_user_ڈیٹا (ثابت hb_سمتیہ_draw_t   *draw,
                              hb_user_ڈیٹا_کلید_t *کلید_2);

HB_EXTERN خالی
hb_سمتیہ_draw_سیٹ_transform (hb_سمتیہ_draw_t *draw,
                              اعشاری xx, اعشاری yx,
                              اعشاری xy, اعشاری yy,
                              اعشاری dx, اعشاری dy);

HB_EXTERN خالی
hb_سمتیہ_draw_get_transform (ثابت hb_سمتیہ_draw_t *draw,
                              اعشاری *xx, اعشاری *yx,
                              اعشاری *xy, اعشاری *yy,
                              اعشاری *dx, اعشاری *dy);

HB_EXTERN خالی
hb_سمتیہ_draw_سیٹ_scale_ضربی_عدد (hb_سمتیہ_draw_t *draw,
                                 اعشاری x_scale_ضربی_عدد,
                                 اعشاری y_scale_ضربی_عدد);

HB_EXTERN خالی
hb_سمتیہ_draw_get_scale_ضربی_عدد (ثابت hb_سمتیہ_draw_t *draw,
                                 اعشاری *x_scale_ضربی_عدد,
                                 اعشاری *y_scale_ضربی_عدد);

HB_EXTERN خالی
hb_سمتیہ_draw_سیٹ_extents (hb_سمتیہ_draw_t *draw,
                            ثابت hb_سمتیہ_extents_t *extents);

HB_EXTERN hb_bool_t
hb_سمتیہ_draw_get_extents (ثابت hb_سمتیہ_draw_t *draw,
                            hb_سمتیہ_extents_t *extents);

HB_EXTERN hb_bool_t
hb_سمتیہ_draw_سیٹ_glyph_extents (hb_سمتیہ_draw_t *draw,
                                  ثابت hb_glyph_extents_t *glyph_extents);

HB_EXTERN hb_سمتیہ_format_t
hb_سمتیہ_draw_get_format (ثابت hb_سمتیہ_draw_t *draw);

HB_EXTERN hb_draw_funcs_t *
hb_سمتیہ_draw_get_funcs (ثابت hb_سمتیہ_draw_t *draw);

HB_EXTERN خالی
hb_سمتیہ_draw_new_راستہ (hb_سمتیہ_draw_t *draw);

HB_EXTERN خالی
hb_سمتیہ_draw_glyph (hb_سمتیہ_draw_t *draw,
                      hb_font_t *font,
                      hb_codepoint_t glyph,
                      hb_سمتیہ_extents_منوال_t extents_منوال);

HB_EXTERN hb_bool_t
hb_سمتیہ_draw_glyph_or_fail (hb_سمتیہ_draw_t *draw,
                              hb_font_t *font,
                              hb_codepoint_t glyph,
                              hb_سمتیہ_extents_منوال_t extents_منوال);

HB_EXTERN خالی
hb_سمتیہ_draw_سیٹ_precision (hb_سمتیہ_draw_t *draw,
                             غیرعلامتی precision);

HB_EXTERN غیرعلامتی
hb_سمتیہ_draw_get_precision (ثابت hb_سمتیہ_draw_t *draw);

HB_EXTERN خالی
hb_سمتیہ_draw_سیٹ_foreground (hb_سمتیہ_draw_t *draw,
                               hb_color_t foreground);

HB_EXTERN hb_color_t
hb_سمتیہ_draw_get_foreground (ثابت hb_سمتیہ_draw_t *draw);

HB_EXTERN خالی
hb_سمتیہ_draw_سیٹ_background (hb_سمتیہ_draw_t *draw,
                               hb_color_t background);

HB_EXTERN hb_color_t
hb_سمتیہ_draw_get_background (ثابت hb_سمتیہ_draw_t *draw);

HB_EXTERN hb_blob_t *
hb_سمتیہ_draw_render (hb_سمتیہ_draw_t *draw);

HB_EXTERN خالی
hb_سمتیہ_draw_clear (hb_سمتیہ_draw_t *draw);

HB_EXTERN خالی
hb_سمتیہ_draw_reset (hb_سمتیہ_draw_t *draw);

HB_EXTERN خالی
hb_سمتیہ_draw_recycle_blob (hb_سمتیہ_draw_t *draw,
                             hb_blob_t *blob);


/* hb_vector_paint_t */

/**
 * hb_vector_paint_t:
 *
 * Opaque paint context for vector color-glyph conversion.
 *
 * Since: 13.0.0
 */
قسمتعریف ساخت hb_سمتیہ_paint_t hb_سمتیہ_paint_t;

HB_EXTERN hb_سمتیہ_paint_t *
hb_سمتیہ_paint_create_or_fail (hb_سمتیہ_format_t format);

HB_EXTERN hb_سمتیہ_paint_t *
hb_سمتیہ_paint_reference (hb_سمتیہ_paint_t *paint);

HB_EXTERN خالی
hb_سمتیہ_paint_destroy (hb_سمتیہ_paint_t *paint);

HB_EXTERN hb_bool_t
hb_سمتیہ_paint_سیٹ_user_ڈیٹا (hb_سمتیہ_paint_t  *paint,
                               hb_user_ڈیٹا_کلید_t *کلید_2,
                               خالی               *ڈیٹا,
                               hb_destroy_func_t   destroy,
                               hb_bool_t           replace);

HB_EXTERN خالی *
hb_سمتیہ_paint_get_user_ڈیٹا (ثابت hb_سمتیہ_paint_t  *paint,
                               hb_user_ڈیٹا_کلید_t *کلید_2);

HB_EXTERN خالی
hb_سمتیہ_paint_سیٹ_transform (hb_سمتیہ_paint_t *paint,
                               اعشاری xx, اعشاری yx,
                               اعشاری xy, اعشاری yy,
                               اعشاری dx, اعشاری dy);

HB_EXTERN خالی
hb_سمتیہ_paint_get_transform (ثابت hb_سمتیہ_paint_t *paint,
                               اعشاری *xx, اعشاری *yx,
                               اعشاری *xy, اعشاری *yy,
                               اعشاری *dx, اعشاری *dy);

HB_EXTERN خالی
hb_سمتیہ_paint_سیٹ_scale_ضربی_عدد (hb_سمتیہ_paint_t *paint,
                                  اعشاری x_scale_ضربی_عدد,
                                  اعشاری y_scale_ضربی_عدد);

HB_EXTERN خالی
hb_سمتیہ_paint_get_scale_ضربی_عدد (ثابت hb_سمتیہ_paint_t *paint,
                                  اعشاری *x_scale_ضربی_عدد,
                                  اعشاری *y_scale_ضربی_عدد);

HB_EXTERN خالی
hb_سمتیہ_paint_سیٹ_extents (hb_سمتیہ_paint_t *paint,
                             ثابت hb_سمتیہ_extents_t *extents);

HB_EXTERN hb_bool_t
hb_سمتیہ_paint_get_extents (ثابت hb_سمتیہ_paint_t *paint,
                             hb_سمتیہ_extents_t *extents);

HB_EXTERN hb_bool_t
hb_سمتیہ_paint_سیٹ_glyph_extents (hb_سمتیہ_paint_t *paint,
                                   ثابت hb_glyph_extents_t *glyph_extents);

HB_EXTERN خالی
hb_سمتیہ_paint_سیٹ_foreground (hb_سمتیہ_paint_t *paint,
                                hb_color_t foreground);

HB_EXTERN hb_color_t
hb_سمتیہ_paint_get_foreground (ثابت hb_سمتیہ_paint_t *paint);

HB_EXTERN خالی
hb_سمتیہ_paint_سیٹ_background (hb_سمتیہ_paint_t *paint,
                                hb_color_t background);

HB_EXTERN hb_color_t
hb_سمتیہ_paint_get_background (ثابت hb_سمتیہ_paint_t *paint);

HB_EXTERN خالی
hb_سمتیہ_paint_سیٹ_palette (hb_سمتیہ_paint_t *paint,
                             صحیح palette);

HB_EXTERN صحیح
hb_سمتیہ_paint_get_palette (ثابت hb_سمتیہ_paint_t *paint);

HB_EXTERN خالی
hb_سمتیہ_paint_سیٹ_custom_palette_color (hb_سمتیہ_paint_t *paint,
                                          غیرعلامتی color_اشاریہ,
                                          hb_color_t color);

HB_EXTERN خالی
hb_سمتیہ_paint_clear_custom_palette_colors (hb_سمتیہ_paint_t *paint);

HB_EXTERN hb_سمتیہ_format_t
hb_سمتیہ_paint_get_format (ثابت hb_سمتیہ_paint_t *paint);

HB_EXTERN hb_paint_funcs_t *
hb_سمتیہ_paint_get_funcs (ثابت hb_سمتیہ_paint_t *paint);

HB_EXTERN خالی
hb_سمتیہ_paint_glyph (hb_سمتیہ_paint_t *paint,
		       hb_font_t         *font,
		       hb_codepoint_t     glyph,
		       hb_سمتیہ_extents_منوال_t extents_منوال);

HB_EXTERN hb_bool_t
hb_سمتیہ_paint_glyph_or_fail (hb_سمتیہ_paint_t *paint,
			       hb_font_t         *font,
			       hb_codepoint_t     glyph,
			       hb_سمتیہ_extents_منوال_t extents_منوال);

HB_EXTERN خالی
hb_سمتیہ_paint_سیٹ_precision (hb_سمتیہ_paint_t *paint,
                                   غیرعلامتی precision);

HB_EXTERN غیرعلامتی
hb_سمتیہ_paint_get_precision (ثابت hb_سمتیہ_paint_t *paint);

HB_EXTERN خالی
hb_سمتیہ_paint_سیٹ_svg_سابقہ (hb_سمتیہ_paint_t *paint,
                                ثابت حرف *سابقہ);

HB_EXTERN ثابت حرف *
hb_سمتیہ_paint_get_svg_سابقہ (ثابت hb_سمتیہ_paint_t *paint);

HB_EXTERN hb_blob_t *
hb_سمتیہ_paint_render (hb_سمتیہ_paint_t *paint);

HB_EXTERN خالی
hb_سمتیہ_paint_clear (hb_سمتیہ_paint_t *paint);

HB_EXTERN خالی
hb_سمتیہ_paint_reset (hb_سمتیہ_paint_t *paint);

HB_EXTERN خالی
hb_سمتیہ_paint_recycle_blob (hb_سمتیہ_paint_t *paint,
                              hb_blob_t *blob);

HB_انتہا_DECLS


#اگر defined(__cplusplus) && defined(HB_CPLUSPLUS_HH)
namespace hb {
HB_DEFINE_VTABLE (سمتیہ_draw,  nullptr);
HB_DEFINE_VTABLE (سمتیہ_paint, nullptr);
} // namespace hb
#اگر_ختم

#اگر_ختم /* HB_VECTOR_H */

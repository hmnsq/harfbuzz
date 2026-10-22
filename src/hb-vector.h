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

#اگر_تعریف_نشده HB_وکتور_H
#تعریف_کن HB_وکتور_H

#ضمن "hb.h"

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
تعريفنوع تعداد {
  HB_وکتور_FORMAT_INVALID = HB_برچسب_NONE,
  HB_وکتور_FORMAT_SVG = HB_برچسب ('s','v','g',' '),
  HB_وکتور_FORMAT_PDF = HB_برچسب ('p','d','f',' '),
} hb_وکتور_format_t;

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
تعريفنوع بنية hb_وکتور_extents_t {
  عائم x, y;
  عائم عرض, ارتفاع;
} hb_وکتور_extents_t;

/**
 * hb_vector_extents_mode_t:
 * @HB_VECTOR_EXTENTS_MODE_NONE: Do not update extents.
 * @HB_VECTOR_EXTENTS_MODE_EXPAND: Union glyph ink extents into current extents.
 *
 * Controls whether convenience glyph APIs update context extents.
 *
 * Since: 13.0.0
 */
تعريفنوع تعداد {
  HB_وکتور_EXTENTS_نما_NONE = 0,
  HB_وکتور_EXTENTS_نما_EXPAND = 1,
} hb_وکتور_extents_نما_t;

/* hb_vector_draw_t */

/**
 * hb_vector_draw_t:
 *
 * Opaque draw context for vector outline conversion.
 *
 * Since: 13.0.0
 */
تعريفنوع بنية hb_وکتور_draw_t hb_وکتور_draw_t;

HB_EXTERN hb_وکتور_draw_t *
hb_وکتور_draw_create_or_fail (hb_وکتور_format_t format);

HB_EXTERN hb_وکتور_draw_t *
hb_وکتور_draw_reference (hb_وکتور_draw_t *draw);

HB_EXTERN فراغ
hb_وکتور_draw_destroy (hb_وکتور_draw_t *draw);

HB_EXTERN hb_bool_t
hb_وکتور_draw_مجموعه_user_داده (hb_وکتور_draw_t   *draw,
                              hb_user_داده_کلید_t *کلید_2,
                              فراغ               *داده,
                              hb_destroy_func_t   destroy,
                              hb_bool_t           replace);

HB_EXTERN فراغ *
hb_وکتور_draw_get_user_داده (ثابت hb_وکتور_draw_t   *draw,
                              hb_user_داده_کلید_t *کلید_2);

HB_EXTERN فراغ
hb_وکتور_draw_مجموعه_transform (hb_وکتور_draw_t *draw,
                              عائم xx, عائم yx,
                              عائم xy, عائم yy,
                              عائم dx, عائم dy);

HB_EXTERN فراغ
hb_وکتور_draw_get_transform (ثابت hb_وکتور_draw_t *draw,
                              عائم *xx, عائم *yx,
                              عائم *xy, عائم *yy,
                              عائم *dx, عائم *dy);

HB_EXTERN فراغ
hb_وکتور_draw_مجموعه_scale_ضریب (hb_وکتور_draw_t *draw,
                                 عائم x_scale_ضریب,
                                 عائم y_scale_ضریب);

HB_EXTERN فراغ
hb_وکتور_draw_get_scale_ضریب (ثابت hb_وکتور_draw_t *draw,
                                 عائم *x_scale_ضریب,
                                 عائم *y_scale_ضریب);

HB_EXTERN فراغ
hb_وکتور_draw_مجموعه_extents (hb_وکتور_draw_t *draw,
                            ثابت hb_وکتور_extents_t *extents);

HB_EXTERN hb_bool_t
hb_وکتور_draw_get_extents (ثابت hb_وکتور_draw_t *draw,
                            hb_وکتور_extents_t *extents);

HB_EXTERN hb_bool_t
hb_وکتور_draw_مجموعه_glyph_extents (hb_وکتور_draw_t *draw,
                                  ثابت hb_glyph_extents_t *glyph_extents);

HB_EXTERN hb_وکتور_format_t
hb_وکتور_draw_get_format (ثابت hb_وکتور_draw_t *draw);

HB_EXTERN hb_draw_funcs_t *
hb_وکتور_draw_get_funcs (ثابت hb_وکتور_draw_t *draw);

HB_EXTERN فراغ
hb_وکتور_draw_new_مسیر (hb_وکتور_draw_t *draw);

HB_EXTERN فراغ
hb_وکتور_draw_glyph (hb_وکتور_draw_t *draw,
                      hb_font_t *font,
                      hb_codepoint_t glyph,
                      hb_وکتور_extents_نما_t extents_نما);

HB_EXTERN hb_bool_t
hb_وکتور_draw_glyph_or_fail (hb_وکتور_draw_t *draw,
                              hb_font_t *font,
                              hb_codepoint_t glyph,
                              hb_وکتور_extents_نما_t extents_نما);

HB_EXTERN فراغ
hb_وکتور_draw_مجموعه_precision (hb_وکتور_draw_t *draw,
                             بیعلامت precision);

HB_EXTERN بیعلامت
hb_وکتور_draw_get_precision (ثابت hb_وکتور_draw_t *draw);

HB_EXTERN فراغ
hb_وکتور_draw_مجموعه_foreground (hb_وکتور_draw_t *draw,
                               hb_color_t foreground);

HB_EXTERN hb_color_t
hb_وکتور_draw_get_foreground (ثابت hb_وکتور_draw_t *draw);

HB_EXTERN فراغ
hb_وکتور_draw_مجموعه_background (hb_وکتور_draw_t *draw,
                               hb_color_t background);

HB_EXTERN hb_color_t
hb_وکتور_draw_get_background (ثابت hb_وکتور_draw_t *draw);

HB_EXTERN hb_blob_t *
hb_وکتور_draw_render (hb_وکتور_draw_t *draw);

HB_EXTERN فراغ
hb_وکتور_draw_clear (hb_وکتور_draw_t *draw);

HB_EXTERN فراغ
hb_وکتور_draw_reset (hb_وکتور_draw_t *draw);

HB_EXTERN فراغ
hb_وکتور_draw_recycle_blob (hb_وکتور_draw_t *draw,
                             hb_blob_t *blob);


/* hb_vector_paint_t */

/**
 * hb_vector_paint_t:
 *
 * Opaque paint context for vector color-glyph conversion.
 *
 * Since: 13.0.0
 */
تعريفنوع بنية hb_وکتور_paint_t hb_وکتور_paint_t;

HB_EXTERN hb_وکتور_paint_t *
hb_وکتور_paint_create_or_fail (hb_وکتور_format_t format);

HB_EXTERN hb_وکتور_paint_t *
hb_وکتور_paint_reference (hb_وکتور_paint_t *paint);

HB_EXTERN فراغ
hb_وکتور_paint_destroy (hb_وکتور_paint_t *paint);

HB_EXTERN hb_bool_t
hb_وکتور_paint_مجموعه_user_داده (hb_وکتور_paint_t  *paint,
                               hb_user_داده_کلید_t *کلید_2,
                               فراغ               *داده,
                               hb_destroy_func_t   destroy,
                               hb_bool_t           replace);

HB_EXTERN فراغ *
hb_وکتور_paint_get_user_داده (ثابت hb_وکتور_paint_t  *paint,
                               hb_user_داده_کلید_t *کلید_2);

HB_EXTERN فراغ
hb_وکتور_paint_مجموعه_transform (hb_وکتور_paint_t *paint,
                               عائم xx, عائم yx,
                               عائم xy, عائم yy,
                               عائم dx, عائم dy);

HB_EXTERN فراغ
hb_وکتور_paint_get_transform (ثابت hb_وکتور_paint_t *paint,
                               عائم *xx, عائم *yx,
                               عائم *xy, عائم *yy,
                               عائم *dx, عائم *dy);

HB_EXTERN فراغ
hb_وکتور_paint_مجموعه_scale_ضریب (hb_وکتور_paint_t *paint,
                                  عائم x_scale_ضریب,
                                  عائم y_scale_ضریب);

HB_EXTERN فراغ
hb_وکتور_paint_get_scale_ضریب (ثابت hb_وکتور_paint_t *paint,
                                  عائم *x_scale_ضریب,
                                  عائم *y_scale_ضریب);

HB_EXTERN فراغ
hb_وکتور_paint_مجموعه_extents (hb_وکتور_paint_t *paint,
                             ثابت hb_وکتور_extents_t *extents);

HB_EXTERN hb_bool_t
hb_وکتور_paint_get_extents (ثابت hb_وکتور_paint_t *paint,
                             hb_وکتور_extents_t *extents);

HB_EXTERN hb_bool_t
hb_وکتور_paint_مجموعه_glyph_extents (hb_وکتور_paint_t *paint,
                                   ثابت hb_glyph_extents_t *glyph_extents);

HB_EXTERN فراغ
hb_وکتور_paint_مجموعه_foreground (hb_وکتور_paint_t *paint,
                                hb_color_t foreground);

HB_EXTERN hb_color_t
hb_وکتور_paint_get_foreground (ثابت hb_وکتور_paint_t *paint);

HB_EXTERN فراغ
hb_وکتور_paint_مجموعه_background (hb_وکتور_paint_t *paint,
                                hb_color_t background);

HB_EXTERN hb_color_t
hb_وکتور_paint_get_background (ثابت hb_وکتور_paint_t *paint);

HB_EXTERN فراغ
hb_وکتور_paint_مجموعه_palette (hb_وکتور_paint_t *paint,
                             صحيح palette);

HB_EXTERN صحيح
hb_وکتور_paint_get_palette (ثابت hb_وکتور_paint_t *paint);

HB_EXTERN فراغ
hb_وکتور_paint_مجموعه_custom_palette_color (hb_وکتور_paint_t *paint,
                                          بیعلامت color_شاخص,
                                          hb_color_t color);

HB_EXTERN فراغ
hb_وکتور_paint_clear_custom_palette_colors (hb_وکتور_paint_t *paint);

HB_EXTERN hb_وکتور_format_t
hb_وکتور_paint_get_format (ثابت hb_وکتور_paint_t *paint);

HB_EXTERN hb_paint_funcs_t *
hb_وکتور_paint_get_funcs (ثابت hb_وکتور_paint_t *paint);

HB_EXTERN فراغ
hb_وکتور_paint_glyph (hb_وکتور_paint_t *paint,
		       hb_font_t         *font,
		       hb_codepoint_t     glyph,
		       hb_وکتور_extents_نما_t extents_نما);

HB_EXTERN hb_bool_t
hb_وکتور_paint_glyph_or_fail (hb_وکتور_paint_t *paint,
			       hb_font_t         *font,
			       hb_codepoint_t     glyph,
			       hb_وکتور_extents_نما_t extents_نما);

HB_EXTERN فراغ
hb_وکتور_paint_مجموعه_precision (hb_وکتور_paint_t *paint,
                                   بیعلامت precision);

HB_EXTERN بیعلامت
hb_وکتور_paint_get_precision (ثابت hb_وکتور_paint_t *paint);

HB_EXTERN فراغ
hb_وکتور_paint_مجموعه_svg_پیشوند (hb_وکتور_paint_t *paint,
                                ثابت حرف *پیشوند);

HB_EXTERN ثابت حرف *
hb_وکتور_paint_get_svg_پیشوند (ثابت hb_وکتور_paint_t *paint);

HB_EXTERN hb_blob_t *
hb_وکتور_paint_render (hb_وکتور_paint_t *paint);

HB_EXTERN فراغ
hb_وکتور_paint_clear (hb_وکتور_paint_t *paint);

HB_EXTERN فراغ
hb_وکتور_paint_reset (hb_وکتور_paint_t *paint);

HB_EXTERN فراغ
hb_وکتور_paint_recycle_blob (hb_وکتور_paint_t *paint,
                              hb_blob_t *blob);

HB_انتها_DECLS


#إذا defined(__cplusplus) && defined(HB_CPLUSPLUS_HH)
namespace hb {
HB_DEFINE_VTABLE (وکتور_draw,  nullptr);
HB_DEFINE_VTABLE (وکتور_paint, nullptr);
} // namespace hb
#پایان_اگر

#پایان_اگر /* HB_VECTOR_H */

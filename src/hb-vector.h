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

#جيڪڏهن_وصف_نه_ٿيل HB_ويڪٽر_H
#وصف_ڪريو HB_ويڪٽر_H

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
  HB_ويڪٽر_FORMAT_INVALID = HB_ٽيگ_NONE,
  HB_ويڪٽر_FORMAT_SVG = HB_ٽيگ ('s','v','g',' '),
  HB_ويڪٽر_FORMAT_PDF = HB_ٽيگ ('p','d','f',' '),
} hb_ويڪٽر_format_t;

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
تعريفنوع بنية hb_ويڪٽر_extents_t {
  عائم x, y;
  عائم ويڪر, اوچائي;
} hb_ويڪٽر_extents_t;

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
  HB_ويڪٽر_EXTENTS_گهڻائي_NONE = 0,
  HB_ويڪٽر_EXTENTS_گهڻائي_EXPAND = 1,
} hb_ويڪٽر_extents_گهڻائي_t;

/* hb_vector_draw_t */

/**
 * hb_vector_draw_t:
 *
 * Opaque draw context for vector outline conversion.
 *
 * Since: 13.0.0
 */
تعريفنوع بنية hb_ويڪٽر_draw_t hb_ويڪٽر_draw_t;

HB_EXTERN hb_ويڪٽر_draw_t *
hb_ويڪٽر_draw_create_or_fail (hb_ويڪٽر_format_t format);

HB_EXTERN hb_ويڪٽر_draw_t *
hb_ويڪٽر_draw_reference (hb_ويڪٽر_draw_t *draw);

HB_EXTERN فراغ
hb_ويڪٽر_draw_destroy (hb_ويڪٽر_draw_t *draw);

HB_EXTERN hb_bool_t
hb_ويڪٽر_draw_سيٽ_user_معلومات (hb_ويڪٽر_draw_t   *draw,
                              hb_user_معلومات_ڪنجي_t *ڪنجي_2,
                              فراغ               *معلومات,
                              hb_destroy_func_t   destroy,
                              hb_bool_t           replace);

HB_EXTERN فراغ *
hb_ويڪٽر_draw_get_user_معلومات (ثابت hb_ويڪٽر_draw_t   *draw,
                              hb_user_معلومات_ڪنجي_t *ڪنجي_2);

HB_EXTERN فراغ
hb_ويڪٽر_draw_سيٽ_transform (hb_ويڪٽر_draw_t *draw,
                              عائم xx, عائم yx,
                              عائم xy, عائم yy,
                              عائم dx, عائم dy);

HB_EXTERN فراغ
hb_ويڪٽر_draw_get_transform (ثابت hb_ويڪٽر_draw_t *draw,
                              عائم *xx, عائم *yx,
                              عائم *xy, عائم *yy,
                              عائم *dx, عائم *dy);

HB_EXTERN فراغ
hb_ويڪٽر_draw_سيٽ_scale_ضرب_ڪندڙ (hb_ويڪٽر_draw_t *draw,
                                 عائم x_scale_ضرب_ڪندڙ,
                                 عائم y_scale_ضرب_ڪندڙ);

HB_EXTERN فراغ
hb_ويڪٽر_draw_get_scale_ضرب_ڪندڙ (ثابت hb_ويڪٽر_draw_t *draw,
                                 عائم *x_scale_ضرب_ڪندڙ,
                                 عائم *y_scale_ضرب_ڪندڙ);

HB_EXTERN فراغ
hb_ويڪٽر_draw_سيٽ_extents (hb_ويڪٽر_draw_t *draw,
                            ثابت hb_ويڪٽر_extents_t *extents);

HB_EXTERN hb_bool_t
hb_ويڪٽر_draw_get_extents (ثابت hb_ويڪٽر_draw_t *draw,
                            hb_ويڪٽر_extents_t *extents);

HB_EXTERN hb_bool_t
hb_ويڪٽر_draw_سيٽ_glyph_extents (hb_ويڪٽر_draw_t *draw,
                                  ثابت hb_glyph_extents_t *glyph_extents);

HB_EXTERN hb_ويڪٽر_format_t
hb_ويڪٽر_draw_get_format (ثابت hb_ويڪٽر_draw_t *draw);

HB_EXTERN hb_draw_funcs_t *
hb_ويڪٽر_draw_get_funcs (ثابت hb_ويڪٽر_draw_t *draw);

HB_EXTERN فراغ
hb_ويڪٽر_draw_new_رستو (hb_ويڪٽر_draw_t *draw);

HB_EXTERN فراغ
hb_ويڪٽر_draw_glyph (hb_ويڪٽر_draw_t *draw,
                      hb_font_t *font,
                      hb_codepoint_t glyph,
                      hb_ويڪٽر_extents_گهڻائي_t extents_گهڻائي);

HB_EXTERN hb_bool_t
hb_ويڪٽر_draw_glyph_or_fail (hb_ويڪٽر_draw_t *draw,
                              hb_font_t *font,
                              hb_codepoint_t glyph,
                              hb_ويڪٽر_extents_گهڻائي_t extents_گهڻائي);

HB_EXTERN فراغ
hb_ويڪٽر_draw_سيٽ_precision (hb_ويڪٽر_draw_t *draw,
                             بنانشان precision);

HB_EXTERN بنانشان
hb_ويڪٽر_draw_get_precision (ثابت hb_ويڪٽر_draw_t *draw);

HB_EXTERN فراغ
hb_ويڪٽر_draw_سيٽ_foreground (hb_ويڪٽر_draw_t *draw,
                               hb_color_t foreground);

HB_EXTERN hb_color_t
hb_ويڪٽر_draw_get_foreground (ثابت hb_ويڪٽر_draw_t *draw);

HB_EXTERN فراغ
hb_ويڪٽر_draw_سيٽ_background (hb_ويڪٽر_draw_t *draw,
                               hb_color_t background);

HB_EXTERN hb_color_t
hb_ويڪٽر_draw_get_background (ثابت hb_ويڪٽر_draw_t *draw);

HB_EXTERN hb_blob_t *
hb_ويڪٽر_draw_render (hb_ويڪٽر_draw_t *draw);

HB_EXTERN فراغ
hb_ويڪٽر_draw_clear (hb_ويڪٽر_draw_t *draw);

HB_EXTERN فراغ
hb_ويڪٽر_draw_reset (hb_ويڪٽر_draw_t *draw);

HB_EXTERN فراغ
hb_ويڪٽر_draw_recycle_blob (hb_ويڪٽر_draw_t *draw,
                             hb_blob_t *blob);


/* hb_vector_paint_t */

/**
 * hb_vector_paint_t:
 *
 * Opaque paint context for vector color-glyph conversion.
 *
 * Since: 13.0.0
 */
تعريفنوع بنية hb_ويڪٽر_paint_t hb_ويڪٽر_paint_t;

HB_EXTERN hb_ويڪٽر_paint_t *
hb_ويڪٽر_paint_create_or_fail (hb_ويڪٽر_format_t format);

HB_EXTERN hb_ويڪٽر_paint_t *
hb_ويڪٽر_paint_reference (hb_ويڪٽر_paint_t *paint);

HB_EXTERN فراغ
hb_ويڪٽر_paint_destroy (hb_ويڪٽر_paint_t *paint);

HB_EXTERN hb_bool_t
hb_ويڪٽر_paint_سيٽ_user_معلومات (hb_ويڪٽر_paint_t  *paint,
                               hb_user_معلومات_ڪنجي_t *ڪنجي_2,
                               فراغ               *معلومات,
                               hb_destroy_func_t   destroy,
                               hb_bool_t           replace);

HB_EXTERN فراغ *
hb_ويڪٽر_paint_get_user_معلومات (ثابت hb_ويڪٽر_paint_t  *paint,
                               hb_user_معلومات_ڪنجي_t *ڪنجي_2);

HB_EXTERN فراغ
hb_ويڪٽر_paint_سيٽ_transform (hb_ويڪٽر_paint_t *paint,
                               عائم xx, عائم yx,
                               عائم xy, عائم yy,
                               عائم dx, عائم dy);

HB_EXTERN فراغ
hb_ويڪٽر_paint_get_transform (ثابت hb_ويڪٽر_paint_t *paint,
                               عائم *xx, عائم *yx,
                               عائم *xy, عائم *yy,
                               عائم *dx, عائم *dy);

HB_EXTERN فراغ
hb_ويڪٽر_paint_سيٽ_scale_ضرب_ڪندڙ (hb_ويڪٽر_paint_t *paint,
                                  عائم x_scale_ضرب_ڪندڙ,
                                  عائم y_scale_ضرب_ڪندڙ);

HB_EXTERN فراغ
hb_ويڪٽر_paint_get_scale_ضرب_ڪندڙ (ثابت hb_ويڪٽر_paint_t *paint,
                                  عائم *x_scale_ضرب_ڪندڙ,
                                  عائم *y_scale_ضرب_ڪندڙ);

HB_EXTERN فراغ
hb_ويڪٽر_paint_سيٽ_extents (hb_ويڪٽر_paint_t *paint,
                             ثابت hb_ويڪٽر_extents_t *extents);

HB_EXTERN hb_bool_t
hb_ويڪٽر_paint_get_extents (ثابت hb_ويڪٽر_paint_t *paint,
                             hb_ويڪٽر_extents_t *extents);

HB_EXTERN hb_bool_t
hb_ويڪٽر_paint_سيٽ_glyph_extents (hb_ويڪٽر_paint_t *paint,
                                   ثابت hb_glyph_extents_t *glyph_extents);

HB_EXTERN فراغ
hb_ويڪٽر_paint_سيٽ_foreground (hb_ويڪٽر_paint_t *paint,
                                hb_color_t foreground);

HB_EXTERN hb_color_t
hb_ويڪٽر_paint_get_foreground (ثابت hb_ويڪٽر_paint_t *paint);

HB_EXTERN فراغ
hb_ويڪٽر_paint_سيٽ_background (hb_ويڪٽر_paint_t *paint,
                                hb_color_t background);

HB_EXTERN hb_color_t
hb_ويڪٽر_paint_get_background (ثابت hb_ويڪٽر_paint_t *paint);

HB_EXTERN فراغ
hb_ويڪٽر_paint_سيٽ_palette (hb_ويڪٽر_paint_t *paint,
                             صحيح palette);

HB_EXTERN صحيح
hb_ويڪٽر_paint_get_palette (ثابت hb_ويڪٽر_paint_t *paint);

HB_EXTERN فراغ
hb_ويڪٽر_paint_سيٽ_custom_palette_color (hb_ويڪٽر_paint_t *paint,
                                          بنانشان color_اشاريو,
                                          hb_color_t color);

HB_EXTERN فراغ
hb_ويڪٽر_paint_clear_custom_palette_colors (hb_ويڪٽر_paint_t *paint);

HB_EXTERN hb_ويڪٽر_format_t
hb_ويڪٽر_paint_get_format (ثابت hb_ويڪٽر_paint_t *paint);

HB_EXTERN hb_paint_funcs_t *
hb_ويڪٽر_paint_get_funcs (ثابت hb_ويڪٽر_paint_t *paint);

HB_EXTERN فراغ
hb_ويڪٽر_paint_glyph (hb_ويڪٽر_paint_t *paint,
		       hb_font_t         *font,
		       hb_codepoint_t     glyph,
		       hb_ويڪٽر_extents_گهڻائي_t extents_گهڻائي);

HB_EXTERN hb_bool_t
hb_ويڪٽر_paint_glyph_or_fail (hb_ويڪٽر_paint_t *paint,
			       hb_font_t         *font,
			       hb_codepoint_t     glyph,
			       hb_ويڪٽر_extents_گهڻائي_t extents_گهڻائي);

HB_EXTERN فراغ
hb_ويڪٽر_paint_سيٽ_precision (hb_ويڪٽر_paint_t *paint,
                                   بنانشان precision);

HB_EXTERN بنانشان
hb_ويڪٽر_paint_get_precision (ثابت hb_ويڪٽر_paint_t *paint);

HB_EXTERN فراغ
hb_ويڪٽر_paint_سيٽ_svg_اڳياڙي (hb_ويڪٽر_paint_t *paint,
                                ثابت حرف *اڳياڙي);

HB_EXTERN ثابت حرف *
hb_ويڪٽر_paint_get_svg_اڳياڙي (ثابت hb_ويڪٽر_paint_t *paint);

HB_EXTERN hb_blob_t *
hb_ويڪٽر_paint_render (hb_ويڪٽر_paint_t *paint);

HB_EXTERN فراغ
hb_ويڪٽر_paint_clear (hb_ويڪٽر_paint_t *paint);

HB_EXTERN فراغ
hb_ويڪٽر_paint_reset (hb_ويڪٽر_paint_t *paint);

HB_EXTERN فراغ
hb_ويڪٽر_paint_recycle_blob (hb_ويڪٽر_paint_t *paint,
                              hb_blob_t *blob);

HB_پڇاڙي_DECLS


#إذا defined(__cplusplus) && defined(HB_CPLUSPLUS_HH)
namespace hb {
HB_DEFINE_VTABLE (ويڪٽر_draw,  nullptr);
HB_DEFINE_VTABLE (ويڪٽر_paint, nullptr);
} // namespace hb
#جيڪڏهن_پورو

#جيڪڏهن_پورو /* HB_VECTOR_H */

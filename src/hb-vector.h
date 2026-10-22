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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ਸਦਿਸ਼_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਸਦਿਸ਼_H

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.h"

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
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਗਿਣਤੀ {
  HB_ਸਦਿਸ਼_FORMAT_INVALID = HB_ਟੈਗ_NONE,
  HB_ਸਦਿਸ਼_FORMAT_SVG = HB_ਟੈਗ ('s','v','g',' '),
  HB_ਸਦਿਸ਼_FORMAT_PDF = HB_ਟੈਗ ('p','d','f',' '),
} hb_ਸਦਿਸ਼_format_t;

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
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ hb_ਸਦਿਸ਼_extents_t {
  ਦਸ਼ਮਲਵ x, y;
  ਦਸ਼ਮਲਵ ਚੌੜਾਈ, ਉਚਾਈ;
} hb_ਸਦਿਸ਼_extents_t;

/**
 * hb_vector_extents_mode_t:
 * @HB_VECTOR_EXTENTS_MODE_NONE: Do not update extents.
 * @HB_VECTOR_EXTENTS_MODE_EXPAND: Union glyph ink extents into current extents.
 *
 * Controls whether convenience glyph APIs update context extents.
 *
 * Since: 13.0.0
 */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਗਿਣਤੀ {
  HB_ਸਦਿਸ਼_EXTENTS_ਬਹੁਲਕ_NONE = 0,
  HB_ਸਦਿਸ਼_EXTENTS_ਬਹੁਲਕ_EXPAND = 1,
} hb_ਸਦਿਸ਼_extents_ਬਹੁਲਕ_t;

/* hb_vector_draw_t */

/**
 * hb_vector_draw_t:
 *
 * Opaque draw context for vector outline conversion.
 *
 * Since: 13.0.0
 */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ hb_ਸਦਿਸ਼_draw_t hb_ਸਦਿਸ਼_draw_t;

HB_EXTERN hb_ਸਦਿਸ਼_draw_t *
hb_ਸਦਿਸ਼_draw_create_or_fail (hb_ਸਦਿਸ਼_format_t format);

HB_EXTERN hb_ਸਦਿਸ਼_draw_t *
hb_ਸਦਿਸ਼_draw_reference (hb_ਸਦਿਸ਼_draw_t *draw);

HB_EXTERN ਖਾਲੀ
hb_ਸਦਿਸ਼_draw_destroy (hb_ਸਦਿਸ਼_draw_t *draw);

HB_EXTERN hb_bool_t
hb_ਸਦਿਸ਼_draw_ਸੈੱਟ_user_ਅੰਕੜੇ (hb_ਸਦਿਸ਼_draw_t   *draw,
                              hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *ਕੁੰਜੀ_2,
                              ਖਾਲੀ               *ਅੰਕੜੇ,
                              hb_destroy_func_t   destroy,
                              hb_bool_t           replace);

HB_EXTERN ਖਾਲੀ *
hb_ਸਦਿਸ਼_draw_get_user_ਅੰਕੜੇ (ਅਚਲ hb_ਸਦਿਸ਼_draw_t   *draw,
                              hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *ਕੁੰਜੀ_2);

HB_EXTERN ਖਾਲੀ
hb_ਸਦਿਸ਼_draw_ਸੈੱਟ_transform (hb_ਸਦਿਸ਼_draw_t *draw,
                              ਦਸ਼ਮਲਵ xx, ਦਸ਼ਮਲਵ yx,
                              ਦਸ਼ਮਲਵ xy, ਦਸ਼ਮਲਵ yy,
                              ਦਸ਼ਮਲਵ dx, ਦਸ਼ਮਲਵ dy);

HB_EXTERN ਖਾਲੀ
hb_ਸਦਿਸ਼_draw_get_transform (ਅਚਲ hb_ਸਦਿਸ਼_draw_t *draw,
                              ਦਸ਼ਮਲਵ *xx, ਦਸ਼ਮਲਵ *yx,
                              ਦਸ਼ਮਲਵ *xy, ਦਸ਼ਮਲਵ *yy,
                              ਦਸ਼ਮਲਵ *dx, ਦਸ਼ਮਲਵ *dy);

HB_EXTERN ਖਾਲੀ
hb_ਸਦਿਸ਼_draw_ਸੈੱਟ_scale_ਗੁਣਕ (hb_ਸਦਿਸ਼_draw_t *draw,
                                 ਦਸ਼ਮਲਵ x_scale_ਗੁਣਕ,
                                 ਦਸ਼ਮਲਵ y_scale_ਗੁਣਕ);

HB_EXTERN ਖਾਲੀ
hb_ਸਦਿਸ਼_draw_get_scale_ਗੁਣਕ (ਅਚਲ hb_ਸਦਿਸ਼_draw_t *draw,
                                 ਦਸ਼ਮਲਵ *x_scale_ਗੁਣਕ,
                                 ਦਸ਼ਮਲਵ *y_scale_ਗੁਣਕ);

HB_EXTERN ਖਾਲੀ
hb_ਸਦਿਸ਼_draw_ਸੈੱਟ_extents (hb_ਸਦਿਸ਼_draw_t *draw,
                            ਅਚਲ hb_ਸਦਿਸ਼_extents_t *extents);

HB_EXTERN hb_bool_t
hb_ਸਦਿਸ਼_draw_get_extents (ਅਚਲ hb_ਸਦਿਸ਼_draw_t *draw,
                            hb_ਸਦਿਸ਼_extents_t *extents);

HB_EXTERN hb_bool_t
hb_ਸਦਿਸ਼_draw_ਸੈੱਟ_glyph_extents (hb_ਸਦਿਸ਼_draw_t *draw,
                                  ਅਚਲ hb_glyph_extents_t *glyph_extents);

HB_EXTERN hb_ਸਦਿਸ਼_format_t
hb_ਸਦਿਸ਼_draw_get_format (ਅਚਲ hb_ਸਦਿਸ਼_draw_t *draw);

HB_EXTERN hb_draw_funcs_t *
hb_ਸਦਿਸ਼_draw_get_funcs (ਅਚਲ hb_ਸਦਿਸ਼_draw_t *draw);

HB_EXTERN ਖਾਲੀ
hb_ਸਦਿਸ਼_draw_new_ਰਸਤਾ (hb_ਸਦਿਸ਼_draw_t *draw);

HB_EXTERN ਖਾਲੀ
hb_ਸਦਿਸ਼_draw_glyph (hb_ਸਦਿਸ਼_draw_t *draw,
                      hb_font_t *font,
                      hb_codepoint_t glyph,
                      hb_ਸਦਿਸ਼_extents_ਬਹੁਲਕ_t extents_ਬਹੁਲਕ);

HB_EXTERN hb_bool_t
hb_ਸਦਿਸ਼_draw_glyph_or_fail (hb_ਸਦਿਸ਼_draw_t *draw,
                              hb_font_t *font,
                              hb_codepoint_t glyph,
                              hb_ਸਦਿਸ਼_extents_ਬਹੁਲਕ_t extents_ਬਹੁਲਕ);

HB_EXTERN ਖਾਲੀ
hb_ਸਦਿਸ਼_draw_ਸੈੱਟ_precision (hb_ਸਦਿਸ਼_draw_t *draw,
                             ਬਿਨਾਨਿਸ਼ਾਨੀ precision);

HB_EXTERN ਬਿਨਾਨਿਸ਼ਾਨੀ
hb_ਸਦਿਸ਼_draw_get_precision (ਅਚਲ hb_ਸਦਿਸ਼_draw_t *draw);

HB_EXTERN ਖਾਲੀ
hb_ਸਦਿਸ਼_draw_ਸੈੱਟ_foreground (hb_ਸਦਿਸ਼_draw_t *draw,
                               hb_color_t foreground);

HB_EXTERN hb_color_t
hb_ਸਦਿਸ਼_draw_get_foreground (ਅਚਲ hb_ਸਦਿਸ਼_draw_t *draw);

HB_EXTERN ਖਾਲੀ
hb_ਸਦਿਸ਼_draw_ਸੈੱਟ_background (hb_ਸਦਿਸ਼_draw_t *draw,
                               hb_color_t background);

HB_EXTERN hb_color_t
hb_ਸਦਿਸ਼_draw_get_background (ਅਚਲ hb_ਸਦਿਸ਼_draw_t *draw);

HB_EXTERN hb_blob_t *
hb_ਸਦਿਸ਼_draw_render (hb_ਸਦਿਸ਼_draw_t *draw);

HB_EXTERN ਖਾਲੀ
hb_ਸਦਿਸ਼_draw_clear (hb_ਸਦਿਸ਼_draw_t *draw);

HB_EXTERN ਖਾਲੀ
hb_ਸਦਿਸ਼_draw_reset (hb_ਸਦਿਸ਼_draw_t *draw);

HB_EXTERN ਖਾਲੀ
hb_ਸਦਿਸ਼_draw_recycle_blob (hb_ਸਦਿਸ਼_draw_t *draw,
                             hb_blob_t *blob);


/* hb_vector_paint_t */

/**
 * hb_vector_paint_t:
 *
 * Opaque paint context for vector color-glyph conversion.
 *
 * Since: 13.0.0
 */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ hb_ਸਦਿਸ਼_paint_t hb_ਸਦਿਸ਼_paint_t;

HB_EXTERN hb_ਸਦਿਸ਼_paint_t *
hb_ਸਦਿਸ਼_paint_create_or_fail (hb_ਸਦਿਸ਼_format_t format);

HB_EXTERN hb_ਸਦਿਸ਼_paint_t *
hb_ਸਦਿਸ਼_paint_reference (hb_ਸਦਿਸ਼_paint_t *paint);

HB_EXTERN ਖਾਲੀ
hb_ਸਦਿਸ਼_paint_destroy (hb_ਸਦਿਸ਼_paint_t *paint);

HB_EXTERN hb_bool_t
hb_ਸਦਿਸ਼_paint_ਸੈੱਟ_user_ਅੰਕੜੇ (hb_ਸਦਿਸ਼_paint_t  *paint,
                               hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *ਕੁੰਜੀ_2,
                               ਖਾਲੀ               *ਅੰਕੜੇ,
                               hb_destroy_func_t   destroy,
                               hb_bool_t           replace);

HB_EXTERN ਖਾਲੀ *
hb_ਸਦਿਸ਼_paint_get_user_ਅੰਕੜੇ (ਅਚਲ hb_ਸਦਿਸ਼_paint_t  *paint,
                               hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *ਕੁੰਜੀ_2);

HB_EXTERN ਖਾਲੀ
hb_ਸਦਿਸ਼_paint_ਸੈੱਟ_transform (hb_ਸਦਿਸ਼_paint_t *paint,
                               ਦਸ਼ਮਲਵ xx, ਦਸ਼ਮਲਵ yx,
                               ਦਸ਼ਮਲਵ xy, ਦਸ਼ਮਲਵ yy,
                               ਦਸ਼ਮਲਵ dx, ਦਸ਼ਮਲਵ dy);

HB_EXTERN ਖਾਲੀ
hb_ਸਦਿਸ਼_paint_get_transform (ਅਚਲ hb_ਸਦਿਸ਼_paint_t *paint,
                               ਦਸ਼ਮਲਵ *xx, ਦਸ਼ਮਲਵ *yx,
                               ਦਸ਼ਮਲਵ *xy, ਦਸ਼ਮਲਵ *yy,
                               ਦਸ਼ਮਲਵ *dx, ਦਸ਼ਮਲਵ *dy);

HB_EXTERN ਖਾਲੀ
hb_ਸਦਿਸ਼_paint_ਸੈੱਟ_scale_ਗੁਣਕ (hb_ਸਦਿਸ਼_paint_t *paint,
                                  ਦਸ਼ਮਲਵ x_scale_ਗੁਣਕ,
                                  ਦਸ਼ਮਲਵ y_scale_ਗੁਣਕ);

HB_EXTERN ਖਾਲੀ
hb_ਸਦਿਸ਼_paint_get_scale_ਗੁਣਕ (ਅਚਲ hb_ਸਦਿਸ਼_paint_t *paint,
                                  ਦਸ਼ਮਲਵ *x_scale_ਗੁਣਕ,
                                  ਦਸ਼ਮਲਵ *y_scale_ਗੁਣਕ);

HB_EXTERN ਖਾਲੀ
hb_ਸਦਿਸ਼_paint_ਸੈੱਟ_extents (hb_ਸਦਿਸ਼_paint_t *paint,
                             ਅਚਲ hb_ਸਦਿਸ਼_extents_t *extents);

HB_EXTERN hb_bool_t
hb_ਸਦਿਸ਼_paint_get_extents (ਅਚਲ hb_ਸਦਿਸ਼_paint_t *paint,
                             hb_ਸਦਿਸ਼_extents_t *extents);

HB_EXTERN hb_bool_t
hb_ਸਦਿਸ਼_paint_ਸੈੱਟ_glyph_extents (hb_ਸਦਿਸ਼_paint_t *paint,
                                   ਅਚਲ hb_glyph_extents_t *glyph_extents);

HB_EXTERN ਖਾਲੀ
hb_ਸਦਿਸ਼_paint_ਸੈੱਟ_foreground (hb_ਸਦਿਸ਼_paint_t *paint,
                                hb_color_t foreground);

HB_EXTERN hb_color_t
hb_ਸਦਿਸ਼_paint_get_foreground (ਅਚਲ hb_ਸਦਿਸ਼_paint_t *paint);

HB_EXTERN ਖਾਲੀ
hb_ਸਦਿਸ਼_paint_ਸੈੱਟ_background (hb_ਸਦਿਸ਼_paint_t *paint,
                                hb_color_t background);

HB_EXTERN hb_color_t
hb_ਸਦਿਸ਼_paint_get_background (ਅਚਲ hb_ਸਦਿਸ਼_paint_t *paint);

HB_EXTERN ਖਾਲੀ
hb_ਸਦਿਸ਼_paint_ਸੈੱਟ_palette (hb_ਸਦਿਸ਼_paint_t *paint,
                             ਪੂਰਨ palette);

HB_EXTERN ਪੂਰਨ
hb_ਸਦਿਸ਼_paint_get_palette (ਅਚਲ hb_ਸਦਿਸ਼_paint_t *paint);

HB_EXTERN ਖਾਲੀ
hb_ਸਦਿਸ਼_paint_ਸੈੱਟ_custom_palette_color (hb_ਸਦਿਸ਼_paint_t *paint,
                                          ਬਿਨਾਨਿਸ਼ਾਨੀ color_ਸੂਚਕਾਂਕ,
                                          hb_color_t color);

HB_EXTERN ਖਾਲੀ
hb_ਸਦਿਸ਼_paint_clear_custom_palette_colors (hb_ਸਦਿਸ਼_paint_t *paint);

HB_EXTERN hb_ਸਦਿਸ਼_format_t
hb_ਸਦਿਸ਼_paint_get_format (ਅਚਲ hb_ਸਦਿਸ਼_paint_t *paint);

HB_EXTERN hb_paint_funcs_t *
hb_ਸਦਿਸ਼_paint_get_funcs (ਅਚਲ hb_ਸਦਿਸ਼_paint_t *paint);

HB_EXTERN ਖਾਲੀ
hb_ਸਦਿਸ਼_paint_glyph (hb_ਸਦਿਸ਼_paint_t *paint,
		       hb_font_t         *font,
		       hb_codepoint_t     glyph,
		       hb_ਸਦਿਸ਼_extents_ਬਹੁਲਕ_t extents_ਬਹੁਲਕ);

HB_EXTERN hb_bool_t
hb_ਸਦਿਸ਼_paint_glyph_or_fail (hb_ਸਦਿਸ਼_paint_t *paint,
			       hb_font_t         *font,
			       hb_codepoint_t     glyph,
			       hb_ਸਦਿਸ਼_extents_ਬਹੁਲਕ_t extents_ਬਹੁਲਕ);

HB_EXTERN ਖਾਲੀ
hb_ਸਦਿਸ਼_paint_ਸੈੱਟ_precision (hb_ਸਦਿਸ਼_paint_t *paint,
                                   ਬਿਨਾਨਿਸ਼ਾਨੀ precision);

HB_EXTERN ਬਿਨਾਨਿਸ਼ਾਨੀ
hb_ਸਦਿਸ਼_paint_get_precision (ਅਚਲ hb_ਸਦਿਸ਼_paint_t *paint);

HB_EXTERN ਖਾਲੀ
hb_ਸਦਿਸ਼_paint_ਸੈੱਟ_svg_ਅਗੇਤਰ (hb_ਸਦਿਸ਼_paint_t *paint,
                                ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ);

HB_EXTERN ਅਚਲ ਅੱਖਰ *
hb_ਸਦਿਸ਼_paint_get_svg_ਅਗੇਤਰ (ਅਚਲ hb_ਸਦਿਸ਼_paint_t *paint);

HB_EXTERN hb_blob_t *
hb_ਸਦਿਸ਼_paint_render (hb_ਸਦਿਸ਼_paint_t *paint);

HB_EXTERN ਖਾਲੀ
hb_ਸਦਿਸ਼_paint_clear (hb_ਸਦਿਸ਼_paint_t *paint);

HB_EXTERN ਖਾਲੀ
hb_ਸਦਿਸ਼_paint_reset (hb_ਸਦਿਸ਼_paint_t *paint);

HB_EXTERN ਖਾਲੀ
hb_ਸਦਿਸ਼_paint_recycle_blob (hb_ਸਦਿਸ਼_paint_t *paint,
                              hb_blob_t *blob);

HB_ਸਿਰਾ_DECLS


#ਜੇ defined(__cplusplus) && defined(HB_CPLUSPLUS_HH)
namespace hb {
HB_DEFINE_VTABLE (ਸਦਿਸ਼_draw,  nullptr);
HB_DEFINE_VTABLE (ਸਦਿਸ਼_paint, nullptr);
} // namespace hb
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ /* HB_VECTOR_H */

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

#నిర్వచితం_కాకపోతే HB_సదిశ_H
#నిర్వచించు HB_సదిశ_H

#అన్తర్భావ "hb.h"

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
రకనిర్వచనం గణన {
  HB_సదిశ_FORMAT_INVALID = HB_ట్యాగ్_NONE,
  HB_సదిశ_FORMAT_SVG = HB_ట్యాగ్ ('s','v','g',' '),
  HB_సదిశ_FORMAT_PDF = HB_ట్యాగ్ ('p','d','f',' '),
} hb_సదిశ_format_t;

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
రకనిర్వచనం నిర్మాణం hb_సదిశ_extents_t {
  దశాంశం x, y;
  దశాంశం వెడల్పు, ఎత్తు;
} hb_సదిశ_extents_t;

/**
 * hb_vector_extents_mode_t:
 * @HB_VECTOR_EXTENTS_MODE_NONE: Do not update extents.
 * @HB_VECTOR_EXTENTS_MODE_EXPAND: Union glyph ink extents into current extents.
 *
 * Controls whether convenience glyph APIs update context extents.
 *
 * Since: 13.0.0
 */
రకనిర్వచనం గణన {
  HB_సదిశ_EXTENTS_బహుళకం_NONE = 0,
  HB_సదిశ_EXTENTS_బహుళకం_EXPAND = 1,
} hb_సదిశ_extents_బహుళకం_t;

/* hb_vector_draw_t */

/**
 * hb_vector_draw_t:
 *
 * Opaque draw context for vector outline conversion.
 *
 * Since: 13.0.0
 */
రకనిర్వచనం నిర్మాణం hb_సదిశ_draw_t hb_సదిశ_draw_t;

HB_EXTERN hb_సదిశ_draw_t *
hb_సదిశ_draw_create_or_fail (hb_సదిశ_format_t format);

HB_EXTERN hb_సదిశ_draw_t *
hb_సదిశ_draw_reference (hb_సదిశ_draw_t *draw);

HB_EXTERN శూన్య
hb_సదిశ_draw_destroy (hb_సదిశ_draw_t *draw);

HB_EXTERN hb_bool_t
hb_సదిశ_draw_సమితి_user_దత్తాంశం (hb_సదిశ_draw_t   *draw,
                              hb_user_దత్తాంశం_కీలకం_t *కీలకం_2,
                              శూన్య               *దత్తాంశం,
                              hb_destroy_func_t   destroy,
                              hb_bool_t           replace);

HB_EXTERN శూన్య *
hb_సదిశ_draw_get_user_దత్తాంశం (స్థిరరాశి hb_సదిశ_draw_t   *draw,
                              hb_user_దత్తాంశం_కీలకం_t *కీలకం_2);

HB_EXTERN శూన్య
hb_సదిశ_draw_సమితి_transform (hb_సదిశ_draw_t *draw,
                              దశాంశం xx, దశాంశం yx,
                              దశాంశం xy, దశాంశం yy,
                              దశాంశం dx, దశాంశం dy);

HB_EXTERN శూన్య
hb_సదిశ_draw_get_transform (స్థిరరాశి hb_సదిశ_draw_t *draw,
                              దశాంశం *xx, దశాంశం *yx,
                              దశాంశం *xy, దశాంశం *yy,
                              దశాంశం *dx, దశాంశం *dy);

HB_EXTERN శూన్య
hb_సదిశ_draw_సమితి_scale_గుణకం (hb_సదిశ_draw_t *draw,
                                 దశాంశం x_scale_గుణకం,
                                 దశాంశం y_scale_గుణకం);

HB_EXTERN శూన్య
hb_సదిశ_draw_get_scale_గుణకం (స్థిరరాశి hb_సదిశ_draw_t *draw,
                                 దశాంశం *x_scale_గుణకం,
                                 దశాంశం *y_scale_గుణకం);

HB_EXTERN శూన్య
hb_సదిశ_draw_సమితి_extents (hb_సదిశ_draw_t *draw,
                            స్థిరరాశి hb_సదిశ_extents_t *extents);

HB_EXTERN hb_bool_t
hb_సదిశ_draw_get_extents (స్థిరరాశి hb_సదిశ_draw_t *draw,
                            hb_సదిశ_extents_t *extents);

HB_EXTERN hb_bool_t
hb_సదిశ_draw_సమితి_glyph_extents (hb_సదిశ_draw_t *draw,
                                  స్థిరరాశి hb_glyph_extents_t *glyph_extents);

HB_EXTERN hb_సదిశ_format_t
hb_సదిశ_draw_get_format (స్థిరరాశి hb_సదిశ_draw_t *draw);

HB_EXTERN hb_draw_funcs_t *
hb_సదిశ_draw_get_funcs (స్థిరరాశి hb_సదిశ_draw_t *draw);

HB_EXTERN శూన్య
hb_సదిశ_draw_new_మార్గం (hb_సదిశ_draw_t *draw);

HB_EXTERN శూన్య
hb_సదిశ_draw_glyph (hb_సదిశ_draw_t *draw,
                      hb_font_t *font,
                      hb_codepoint_t glyph,
                      hb_సదిశ_extents_బహుళకం_t extents_బహుళకం);

HB_EXTERN hb_bool_t
hb_సదిశ_draw_glyph_or_fail (hb_సదిశ_draw_t *draw,
                              hb_font_t *font,
                              hb_codepoint_t glyph,
                              hb_సదిశ_extents_బహుళకం_t extents_బహుళకం);

HB_EXTERN శూన్య
hb_సదిశ_draw_సమితి_precision (hb_సదిశ_draw_t *draw,
                             గుర్తులేని precision);

HB_EXTERN గుర్తులేని
hb_సదిశ_draw_get_precision (స్థిరరాశి hb_సదిశ_draw_t *draw);

HB_EXTERN శూన్య
hb_సదిశ_draw_సమితి_foreground (hb_సదిశ_draw_t *draw,
                               hb_color_t foreground);

HB_EXTERN hb_color_t
hb_సదిశ_draw_get_foreground (స్థిరరాశి hb_సదిశ_draw_t *draw);

HB_EXTERN శూన్య
hb_సదిశ_draw_సమితి_background (hb_సదిశ_draw_t *draw,
                               hb_color_t background);

HB_EXTERN hb_color_t
hb_సదిశ_draw_get_background (స్థిరరాశి hb_సదిశ_draw_t *draw);

HB_EXTERN hb_blob_t *
hb_సదిశ_draw_render (hb_సదిశ_draw_t *draw);

HB_EXTERN శూన్య
hb_సదిశ_draw_clear (hb_సదిశ_draw_t *draw);

HB_EXTERN శూన్య
hb_సదిశ_draw_reset (hb_సదిశ_draw_t *draw);

HB_EXTERN శూన్య
hb_సదిశ_draw_recycle_blob (hb_సదిశ_draw_t *draw,
                             hb_blob_t *blob);


/* hb_vector_paint_t */

/**
 * hb_vector_paint_t:
 *
 * Opaque paint context for vector color-glyph conversion.
 *
 * Since: 13.0.0
 */
రకనిర్వచనం నిర్మాణం hb_సదిశ_paint_t hb_సదిశ_paint_t;

HB_EXTERN hb_సదిశ_paint_t *
hb_సదిశ_paint_create_or_fail (hb_సదిశ_format_t format);

HB_EXTERN hb_సదిశ_paint_t *
hb_సదిశ_paint_reference (hb_సదిశ_paint_t *paint);

HB_EXTERN శూన్య
hb_సదిశ_paint_destroy (hb_సదిశ_paint_t *paint);

HB_EXTERN hb_bool_t
hb_సదిశ_paint_సమితి_user_దత్తాంశం (hb_సదిశ_paint_t  *paint,
                               hb_user_దత్తాంశం_కీలకం_t *కీలకం_2,
                               శూన్య               *దత్తాంశం,
                               hb_destroy_func_t   destroy,
                               hb_bool_t           replace);

HB_EXTERN శూన్య *
hb_సదిశ_paint_get_user_దత్తాంశం (స్థిరరాశి hb_సదిశ_paint_t  *paint,
                               hb_user_దత్తాంశం_కీలకం_t *కీలకం_2);

HB_EXTERN శూన్య
hb_సదిశ_paint_సమితి_transform (hb_సదిశ_paint_t *paint,
                               దశాంశం xx, దశాంశం yx,
                               దశాంశం xy, దశాంశం yy,
                               దశాంశం dx, దశాంశం dy);

HB_EXTERN శూన్య
hb_సదిశ_paint_get_transform (స్థిరరాశి hb_సదిశ_paint_t *paint,
                               దశాంశం *xx, దశాంశం *yx,
                               దశాంశం *xy, దశాంశం *yy,
                               దశాంశం *dx, దశాంశం *dy);

HB_EXTERN శూన్య
hb_సదిశ_paint_సమితి_scale_గుణకం (hb_సదిశ_paint_t *paint,
                                  దశాంశం x_scale_గుణకం,
                                  దశాంశం y_scale_గుణకం);

HB_EXTERN శూన్య
hb_సదిశ_paint_get_scale_గుణకం (స్థిరరాశి hb_సదిశ_paint_t *paint,
                                  దశాంశం *x_scale_గుణకం,
                                  దశాంశం *y_scale_గుణకం);

HB_EXTERN శూన్య
hb_సదిశ_paint_సమితి_extents (hb_సదిశ_paint_t *paint,
                             స్థిరరాశి hb_సదిశ_extents_t *extents);

HB_EXTERN hb_bool_t
hb_సదిశ_paint_get_extents (స్థిరరాశి hb_సదిశ_paint_t *paint,
                             hb_సదిశ_extents_t *extents);

HB_EXTERN hb_bool_t
hb_సదిశ_paint_సమితి_glyph_extents (hb_సదిశ_paint_t *paint,
                                   స్థిరరాశి hb_glyph_extents_t *glyph_extents);

HB_EXTERN శూన్య
hb_సదిశ_paint_సమితి_foreground (hb_సదిశ_paint_t *paint,
                                hb_color_t foreground);

HB_EXTERN hb_color_t
hb_సదిశ_paint_get_foreground (స్థిరరాశి hb_సదిశ_paint_t *paint);

HB_EXTERN శూన్య
hb_సదిశ_paint_సమితి_background (hb_సదిశ_paint_t *paint,
                                hb_color_t background);

HB_EXTERN hb_color_t
hb_సదిశ_paint_get_background (స్థిరరాశి hb_సదిశ_paint_t *paint);

HB_EXTERN శూన్య
hb_సదిశ_paint_సమితి_palette (hb_సదిశ_paint_t *paint,
                             పూర్ణాంకం palette);

HB_EXTERN పూర్ణాంకం
hb_సదిశ_paint_get_palette (స్థిరరాశి hb_సదిశ_paint_t *paint);

HB_EXTERN శూన్య
hb_సదిశ_paint_సమితి_custom_palette_color (hb_సదిశ_paint_t *paint,
                                          గుర్తులేని color_సూచీ,
                                          hb_color_t color);

HB_EXTERN శూన్య
hb_సదిశ_paint_clear_custom_palette_colors (hb_సదిశ_paint_t *paint);

HB_EXTERN hb_సదిశ_format_t
hb_సదిశ_paint_get_format (స్థిరరాశి hb_సదిశ_paint_t *paint);

HB_EXTERN hb_paint_funcs_t *
hb_సదిశ_paint_get_funcs (స్థిరరాశి hb_సదిశ_paint_t *paint);

HB_EXTERN శూన్య
hb_సదిశ_paint_glyph (hb_సదిశ_paint_t *paint,
		       hb_font_t         *font,
		       hb_codepoint_t     glyph,
		       hb_సదిశ_extents_బహుళకం_t extents_బహుళకం);

HB_EXTERN hb_bool_t
hb_సదిశ_paint_glyph_or_fail (hb_సదిశ_paint_t *paint,
			       hb_font_t         *font,
			       hb_codepoint_t     glyph,
			       hb_సదిశ_extents_బహుళకం_t extents_బహుళకం);

HB_EXTERN శూన్య
hb_సదిశ_paint_సమితి_precision (hb_సదిశ_paint_t *paint,
                                   గుర్తులేని precision);

HB_EXTERN గుర్తులేని
hb_సదిశ_paint_get_precision (స్థిరరాశి hb_సదిశ_paint_t *paint);

HB_EXTERN శూన్య
hb_సదిశ_paint_సమితి_svg_ఉపసర్గ (hb_సదిశ_paint_t *paint,
                                స్థిరరాశి అక్షరం *ఉపసర్గ);

HB_EXTERN స్థిరరాశి అక్షరం *
hb_సదిశ_paint_get_svg_ఉపసర్గ (స్థిరరాశి hb_సదిశ_paint_t *paint);

HB_EXTERN hb_blob_t *
hb_సదిశ_paint_render (hb_సదిశ_paint_t *paint);

HB_EXTERN శూన్య
hb_సదిశ_paint_clear (hb_సదిశ_paint_t *paint);

HB_EXTERN శూన్య
hb_సదిశ_paint_reset (hb_సదిశ_paint_t *paint);

HB_EXTERN శూన్య
hb_సదిశ_paint_recycle_blob (hb_సదిశ_paint_t *paint,
                              hb_blob_t *blob);

HB_చివర_DECLS


#ఒకవేళ defined(__cplusplus) && defined(HB_CPLUSPLUS_HH)
namespace hb {
HB_DEFINE_VTABLE (సదిశ_draw,  nullptr);
HB_DEFINE_VTABLE (సదిశ_paint, nullptr);
} // namespace hb
#అయితే_ముగింపు

#అయితే_ముగింపు /* HB_VECTOR_H */

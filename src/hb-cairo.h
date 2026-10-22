/*
 * Copyright © 2022 Red Hat, Inc.
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
 * Red Hat Author(s): Matthias Clasen
 */

#నిర్వచితం_కాకపోతే HB_CAIRO_H
#నిర్వచించు HB_CAIRO_H

#అన్తర్భావ "hb.h"

#అన్తర్భావ <cairo.h>

HB_BEGIN_DECLS

HB_EXTERN cairo_font_face_t *
hb_cairo_font_face_create_for_font (hb_font_t *font);

HB_EXTERN hb_font_t *
hb_cairo_font_face_get_font (cairo_font_face_t *font_face);

HB_EXTERN cairo_font_face_t *
hb_cairo_font_face_create_for_face (hb_face_t *face);

HB_EXTERN hb_face_t *
hb_cairo_font_face_get_face (cairo_font_face_t *font_face);

/**
 * hb_cairo_font_init_func_t:
 * @font: The #hb_font_t being created
 * @scaled_font: The respective #cairo_scaled_font_t
 * @user_data: User data accompanying this method
 *
 * The type of a virtual method to be called when a cairo
 * face created using hb_cairo_font_face_create_for_face()
 * creates an #hb_font_t for a #cairo_scaled_font_t.
 *
 * Return value: the #hb_font_t value to use; in most cases same as @font
 *
 * Since: 7.0.0
 */
రకనిర్వచనం hb_font_t * (*hb_cairo_font_init_func_t) (hb_font_t *font,
						  cairo_scaled_font_t *scaled_font,
						  శూన్య *user_దత్తాంశం);

HB_EXTERN శూన్య
hb_cairo_font_face_సమితి_font_init_func (cairo_font_face_t *font_face,
				       hb_cairo_font_init_func_t func,
				       శూన్య *user_దత్తాంశం,
				       hb_destroy_func_t destroy);

HB_EXTERN hb_font_t *
hb_cairo_scaled_font_get_font (cairo_scaled_font_t *scaled_font);

HB_EXTERN శూన్య
hb_cairo_font_face_సమితి_scale_గుణకం (cairo_font_face_t *font_face,
				     గుర్తులేని పూర్ణాంకం scale_గుణకం);

HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_cairo_font_face_get_scale_గుణకం (cairo_font_face_t *font_face);

HB_EXTERN శూన్య
hb_cairo_glyphs_from_బఫరు (hb_బఫరు_t *బఫరు,
			     hb_bool_t utf8_clusters,
			     ద్విగుణ x_scale_గుణకం,
			     ద్విగుణ y_scale_గుణకం,
			     ద్విగుణ x,
			     ద్విగుణ y,
			     స్థిరరాశి అక్షరం *utf8,
			     పూర్ణాంకం utf8_len,
			     cairo_glyph_t **glyphs,
			     గుర్తులేని పూర్ణాంకం *అంకె_glyphs,
			     cairo_పాఠ్యం_cluster_t **clusters,
			     గుర్తులేని పూర్ణాంకం *అంకె_clusters,
			     cairo_పాఠ్యం_cluster_flags_t *cluster_flags);

HB_చివర_DECLS

#అయితే_ముగింపు /* HB_CAIRO_H */

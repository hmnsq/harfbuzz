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

#اگر_غیر_تعریف_شدہ HB_CAIRO_H
#تعریف_کرو HB_CAIRO_H

#شامل "hb.h"

#شامل <cairo.h>

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
قسمتعریف hb_font_t * (*hb_cairo_font_init_func_t) (hb_font_t *font,
						  cairo_scaled_font_t *scaled_font,
						  خالی *user_ڈیٹا);

HB_EXTERN خالی
hb_cairo_font_face_سیٹ_font_init_func (cairo_font_face_t *font_face,
				       hb_cairo_font_init_func_t func,
				       خالی *user_ڈیٹا,
				       hb_destroy_func_t destroy);

HB_EXTERN hb_font_t *
hb_cairo_scaled_font_get_font (cairo_scaled_font_t *scaled_font);

HB_EXTERN خالی
hb_cairo_font_face_سیٹ_scale_ضربی_عدد (cairo_font_face_t *font_face,
				     غیرعلامتی صحیح scale_ضربی_عدد);

HB_EXTERN غیرعلامتی صحیح
hb_cairo_font_face_get_scale_ضربی_عدد (cairo_font_face_t *font_face);

HB_EXTERN خالی
hb_cairo_glyphs_from_بفر (hb_بفر_t *بفر,
			     hb_bool_t utf8_clusters,
			     دوہرا x_scale_ضربی_عدد,
			     دوہرا y_scale_ضربی_عدد,
			     دوہرا x,
			     دوہرا y,
			     ثابت حرف *utf8,
			     صحیح utf8_len,
			     cairo_glyph_t **glyphs,
			     غیرعلامتی صحیح *عدد_glyphs,
			     cairo_متن_cluster_t **clusters,
			     غیرعلامتی صحیح *عدد_clusters,
			     cairo_متن_cluster_flags_t *cluster_flags);

HB_انتہا_DECLS

#اگر_ختم /* HB_CAIRO_H */

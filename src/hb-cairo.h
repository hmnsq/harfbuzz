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

#که_نه_تعریف_شوی HB_CAIRO_H
#تعریف_کړه HB_CAIRO_H

#ضمن "hb.h"

#ضمن <cairo.h>

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
تعريفنوع hb_font_t * (*hb_cairo_font_init_func_t) (hb_font_t *font,
						  cairo_scaled_font_t *scaled_font,
						  فراغ *user_معلومات);

HB_EXTERN فراغ
hb_cairo_font_face_سېټ_font_init_func (cairo_font_face_t *font_face,
				       hb_cairo_font_init_func_t func,
				       فراغ *user_معلومات,
				       hb_destroy_func_t destroy);

HB_EXTERN hb_font_t *
hb_cairo_scaled_font_get_font (cairo_scaled_font_t *scaled_font);

HB_EXTERN فراغ
hb_cairo_font_face_سېټ_scale_ضریب (cairo_font_face_t *font_face,
				     بینښه صحيح scale_ضریب);

HB_EXTERN بینښه صحيح
hb_cairo_font_face_get_scale_ضریب (cairo_font_face_t *font_face);

HB_EXTERN فراغ
hb_cairo_glyphs_from_بفر (hb_بفر_t *بفر,
			     hb_bool_t utf8_clusters,
			     مزدوج x_scale_ضریب,
			     مزدوج y_scale_ضریب,
			     مزدوج x,
			     مزدوج y,
			     ثابت حرف *utf8,
			     صحيح utf8_len,
			     cairo_glyph_t **glyphs,
			     بینښه صحيح *عدد_glyphs,
			     cairo_متن_cluster_t **clusters,
			     بینښه صحيح *عدد_clusters,
			     cairo_متن_cluster_flags_t *cluster_flags);

HB_پای_ټکی_DECLS

#د_که_پای /* HB_CAIRO_H */

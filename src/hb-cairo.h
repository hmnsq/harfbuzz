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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_CAIRO_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_CAIRO_H

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <cairo.h>

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
ਕਿਸਮਪਰਿਭਾਸ਼ਾ hb_font_t * (*hb_cairo_font_init_func_t) (hb_font_t *font,
						  cairo_scaled_font_t *scaled_font,
						  ਖਾਲੀ *user_ਅੰਕੜੇ);

HB_EXTERN ਖਾਲੀ
hb_cairo_font_face_ਸੈੱਟ_font_init_func (cairo_font_face_t *font_face,
				       hb_cairo_font_init_func_t func,
				       ਖਾਲੀ *user_ਅੰਕੜੇ,
				       hb_destroy_func_t destroy);

HB_EXTERN hb_font_t *
hb_cairo_scaled_font_get_font (cairo_scaled_font_t *scaled_font);

HB_EXTERN ਖਾਲੀ
hb_cairo_font_face_ਸੈੱਟ_scale_ਗੁਣਕ (cairo_font_face_t *font_face,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ scale_ਗੁਣਕ);

HB_EXTERN ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_cairo_font_face_get_scale_ਗੁਣਕ (cairo_font_face_t *font_face);

HB_EXTERN ਖਾਲੀ
hb_cairo_glyphs_from_ਬਫਰ (hb_ਬਫਰ_t *ਬਫਰ,
			     hb_bool_t utf8_clusters,
			     ਦੁੱਗਣਾ x_scale_ਗੁਣਕ,
			     ਦੁੱਗਣਾ y_scale_ਗੁਣਕ,
			     ਦੁੱਗਣਾ x,
			     ਦੁੱਗਣਾ y,
			     ਅਚਲ ਅੱਖਰ *utf8,
			     ਪੂਰਨ utf8_len,
			     cairo_glyph_t **glyphs,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਅੰਕ_glyphs,
			     cairo_ਪਾਠ_cluster_t **clusters,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਅੰਕ_clusters,
			     cairo_ਪਾਠ_cluster_flags_t *cluster_flags);

HB_ਸਿਰਾ_DECLS

#ਜੇ_ਅੰਤ /* HB_CAIRO_H */

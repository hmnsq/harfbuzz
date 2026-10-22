/*
 * Copyright © 2015-2019  Ebrahim Byagowi
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
 */

#که_نه_تعریف_شوی HB_DIRECTWRITE_H
#تعریف_کړه HB_DIRECTWRITE_H

#ضمن "hb.h"

#ضمن <dwrite_3.h>

HB_BEGIN_DECLS

HB_EXTERN hb_face_t *
hb_directwrite_face_create (IDWriteFontFace *dw_face);

HB_EXTERN hb_face_t *
hb_directwrite_face_create_from_file_or_fail (ثابت حرف   *file_name,
					      بینښه صحيح  شاخص_2);

HB_EXTERN hb_face_t *
hb_directwrite_face_create_from_blob_or_fail (hb_blob_t    *blob,
					      بینښه صحيح  شاخص_2);

HB_EXTERN IDWriteFontFace *
hb_directwrite_face_get_dw_font_face (hb_face_t *face);

HB_EXTERN hb_font_t *
hb_directwrite_font_create (IDWriteFontFace *dw_face);

HB_EXTERN IDWriteFontFace *
hb_directwrite_font_get_dw_font_face (hb_font_t *font);

HB_EXTERN فراغ
hb_directwrite_font_سېټ_funcs (hb_font_t *font);

#که_نه_تعریف_شوی HB_DISABLE_DEPRECATED

HB_DEPRECATED_FOR (hb_directwrite_face_get_dw_font_face)
HB_EXTERN IDWriteFontFace *
hb_directwrite_face_get_font_face (hb_face_t *face);

HB_DEPRECATED
HB_EXTERN IDWriteFont *
hb_directwrite_font_get_dw_font (hb_font_t *font);

#د_که_پای

HB_پای_ټکی_DECLS

#د_که_پای /* HB_DIRECTWRITE_H */

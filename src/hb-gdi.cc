/*
 * Copyright © 2019  Ebrahim Byagowi
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

#شامل "hb.hh"

#اگر_تعریف_شدہ HAVE_GDI

#شامل "hb-gdi.h"


/**
 * SECTION:hb-gdi
 * @title: hb-gdi
 * @short_description: GDI integration
 * @include: hb-gdi.h
 *
 * Functions for using HarfBuzz with GDI fonts.
 **/

ساکن hb_blob_t *
hb_gdi_reference_جدول (hb_face_t *face HB_غیر_مستعمل, hb_لیبل_t لیبل_2, خالی *user_ڈیٹا)
{
  حرف *بفر = خالی_اشاریہ;
  DWORD لمبائی = 0;

  HDC hdc = GetDC (خالی_اشاریہ);
  اگر (unlikely (!SelectObject (hdc, (HFONT) user_ڈیٹا)))
    واپس hb_blob_get_خالی_خانے ();
  HB_SCOPE_GUARD (ReleaseDC (خالی_اشاریہ, hdc));

  لمبائی = Get_Font_ڈیٹا (hdc, hb_uint32_تبادلہ (لیبل_2), 0, بفر, لمبائی);
  اگر (unlikely (لمبائی == GDI_ERROR))
    واپس hb_blob_get_خالی_خانے ();

  بفر = (حرف *) hb_malloc (لمبائی);
  اگر (unlikely (!بفر))
    واپس hb_blob_get_خالی_خانے ();
  خودکار بفر_guard = hb_make_scope_guard ([&]() { hb_free (بفر); });

  لمبائی = Get_Font_ڈیٹا (hdc, hb_uint32_تبادلہ (لیبل_2), 0, بفر, لمبائی);
  اگر (unlikely (لمبائی == GDI_ERROR))
    واپس hb_blob_get_خالی_خانے ();

  بفر_guard.release ();
  واپس hb_blob_create ((ثابت حرف *) بفر, لمبائی, HB_حافظہ_منوال_WRITABLE, بفر, hb_free);
}

/**
 * hb_gdi_face_create:
 * @hfont: a HFONT object.
 *
 * Constructs a new face object from the specified GDI HFONT.
 *
 * Return value: #hb_face_t object corresponding to the given input
 *
 * Since: 2.6.0
 **/
hb_face_t *
hb_gdi_face_create (HFONT hfont)
{
  واپس hb_face_create_for_tables (hb_gdi_reference_جدول, (خالی *) hfont, خالی_اشاریہ);
}

#اگر_ختم

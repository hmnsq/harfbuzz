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

#ضمن "hb.hh"

#اگر_تعریف_شده HAVE_GDI

#ضمن "hb-gdi.h"


/**
 * SECTION:hb-gdi
 * @title: hb-gdi
 * @short_description: GDI integration
 * @include: hb-gdi.h
 *
 * Functions for using HarfBuzz with GDI fonts.
 **/

ساكن hb_blob_t *
hb_gdi_reference_جدول (hb_face_t *face HB_استفادهنشده, hb_برچسب_t برچسب_2, فراغ *user_داده)
{
  حرف *میانگیر = اشاره_تهی;
  DWORD درازی = 0;

  HDC hdc = GetDC (اشاره_تهی);
  إذا (unlikely (!SelectObject (hdc, (HFONT) user_داده)))
    أرجع hb_blob_get_خالی ();
  HB_SCOPE_GUARD (ReleaseDC (اشاره_تهی, hdc));

  درازی = Get_Font_داده (hdc, hb_uint32_جابجا (برچسب_2), 0, میانگیر, درازی);
  إذا (unlikely (درازی == GDI_ERROR))
    أرجع hb_blob_get_خالی ();

  میانگیر = (حرف *) hb_malloc (درازی);
  إذا (unlikely (!میانگیر))
    أرجع hb_blob_get_خالی ();
  خودکار میانگیر_guard = hb_make_scope_guard ([&]() { hb_free (میانگیر); });

  درازی = Get_Font_داده (hdc, hb_uint32_جابجا (برچسب_2), 0, میانگیر, درازی);
  إذا (unlikely (درازی == GDI_ERROR))
    أرجع hb_blob_get_خالی ();

  میانگیر_guard.release ();
  أرجع hb_blob_create ((ثابت حرف *) میانگیر, درازی, HB_حافظه_نما_WRITABLE, میانگیر, hb_free);
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
  أرجع hb_face_create_for_tables (hb_gdi_reference_جدول, (فراغ *) hfont, اشاره_تهی);
}

#پایان_اگر

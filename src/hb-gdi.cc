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

#جيڪڏهن_وصف_ٿيل HAVE_GDI

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
hb_gdi_reference_جدول (hb_face_t *face HB_اڻ_استعمال_ٿيل, hb_ٽيگ_t ٽيگ_2, فراغ *user_معلومات)
{
  حرف *بفر = خالي_اشارو;
  DWORD ڊيگهائي = 0;

  HDC hdc = GetDC (خالي_اشارو);
  إذا (unlikely (!SelectObject (hdc, (HFONT) user_معلومات)))
    أرجع hb_blob_get_خالي_جايون ();
  HB_SCOPE_GUARD (ReleaseDC (خالي_اشارو, hdc));

  ڊيگهائي = Get_Font_معلومات (hdc, hb_uint32_مٽا_سٽا (ٽيگ_2), 0, بفر, ڊيگهائي);
  إذا (unlikely (ڊيگهائي == GDI_ERROR))
    أرجع hb_blob_get_خالي_جايون ();

  بفر = (حرف *) hb_malloc (ڊيگهائي);
  إذا (unlikely (!بفر))
    أرجع hb_blob_get_خالي_جايون ();
  خودڪار بفر_guard = hb_make_scope_guard ([&]() { hb_free (بفر); });

  ڊيگهائي = Get_Font_معلومات (hdc, hb_uint32_مٽا_سٽا (ٽيگ_2), 0, بفر, ڊيگهائي);
  إذا (unlikely (ڊيگهائي == GDI_ERROR))
    أرجع hb_blob_get_خالي_جايون ();

  بفر_guard.release ();
  أرجع hb_blob_create ((ثابت حرف *) بفر, ڊيگهائي, HB_يادگيري_گهڻائي_WRITABLE, بفر, hb_free);
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
  أرجع hb_face_create_for_tables (hb_gdi_reference_جدول, (فراغ *) hfont, خالي_اشارو);
}

#جيڪڏهن_پورو

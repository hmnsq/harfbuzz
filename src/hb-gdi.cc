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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_GDI

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-gdi.h"


/**
 * SECTION:hb-gdi
 * @title: hb-gdi
 * @short_description: GDI integration
 * @include: hb-gdi.h
 *
 * Functions for using HarfBuzz with GDI fonts.
 **/

ਸਥਿਰ hb_blob_t *
hb_gdi_reference_ਸਾਰਣੀ (hb_face_t *face HB_ਅਣਵਰਤਿਆ, hb_ਟੈਗ_t ਟੈਗ_2, ਖਾਲੀ *user_ਅੰਕੜੇ)
{
  ਅੱਖਰ *ਬਫਰ = ਖਾਲੀ_ਸੂਚਕ;
  DWORD ਲੰਮਾਈ = 0;

  HDC hdc = GetDC (ਖਾਲੀ_ਸੂਚਕ);
  ਜੇ (unlikely (!SelectObject (hdc, (HFONT) user_ਅੰਕੜੇ)))
    ਵਾਪਸ hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ();
  HB_SCOPE_GUARD (ReleaseDC (ਖਾਲੀ_ਸੂਚਕ, hdc));

  ਲੰਮਾਈ = Get_Font_ਅੰਕੜੇ (hdc, hb_uint32_ਅਦਲਾ_ਬਦਲੀ (ਟੈਗ_2), 0, ਬਫਰ, ਲੰਮਾਈ);
  ਜੇ (unlikely (ਲੰਮਾਈ == GDI_ERROR))
    ਵਾਪਸ hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ();

  ਬਫਰ = (ਅੱਖਰ *) hb_malloc (ਲੰਮਾਈ);
  ਜੇ (unlikely (!ਬਫਰ))
    ਵਾਪਸ hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ();
  ਸਵੈਚਾਲਿਤ ਬਫਰ_guard = hb_make_scope_guard ([&]() { hb_free (ਬਫਰ); });

  ਲੰਮਾਈ = Get_Font_ਅੰਕੜੇ (hdc, hb_uint32_ਅਦਲਾ_ਬਦਲੀ (ਟੈਗ_2), 0, ਬਫਰ, ਲੰਮਾਈ);
  ਜੇ (unlikely (ਲੰਮਾਈ == GDI_ERROR))
    ਵਾਪਸ hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ();

  ਬਫਰ_guard.release ();
  ਵਾਪਸ hb_blob_create ((ਅਚਲ ਅੱਖਰ *) ਬਫਰ, ਲੰਮਾਈ, HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_WRITABLE, ਬਫਰ, hb_free);
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
  ਵਾਪਸ hb_face_create_for_tables (hb_gdi_reference_ਸਾਰਣੀ, (ਖਾਲੀ *) hfont, ਖਾਲੀ_ਸੂਚਕ);
}

#ਜੇ_ਅੰਤ

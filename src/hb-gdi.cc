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

#अन्तर्भाव "hb.hh"

#ifdef HAVE_GDI

#अन्तर्भाव "hb-gdi.h"


/**
 * SECTION:hb-gdi
 * @title: hb-gdi
 * @short_description: GDI integration
 * @include: hb-gdi.h
 *
 * Functions for using HarfBuzz with GDI fonts.
 **/

स्थिर hb_blob_t *
_hb_gdi_reference_table (hb_face_t *face HB_UNUSED, hb_tag_t tag, शून्य *user_data)
{
  वर्ण *buffer = nullptr;
  DWORD length = 0;

  HDC hdc = GetDC (nullptr);
  यदि (unlikely (!SelectObject (hdc, (HFONT) user_data)))
    फर्काऊ hb_blob_get_empty ();
  HB_SCOPE_GUARD (ReleaseDC (nullptr, hdc));

  length = GetFontData (hdc, hb_uint32_swap (tag), 0, buffer, length);
  यदि (unlikely (length == GDI_ERROR))
    फर्काऊ hb_blob_get_empty ();

  buffer = (वर्ण *) hb_malloc (length);
  यदि (unlikely (!buffer))
    फर्काऊ hb_blob_get_empty ();
  स्वतः buffer_guard = hb_make_scope_guard ([&]() { hb_free (buffer); });

  length = GetFontData (hdc, hb_uint32_swap (tag), 0, buffer, length);
  यदि (unlikely (length == GDI_ERROR))
    फर्काऊ hb_blob_get_empty ();

  buffer_guard.release ();
  फर्काऊ hb_blob_create ((अचर वर्ण *) buffer, length, HB_MEMORY_MODE_WRITABLE, buffer, hb_free);
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
  फर्काऊ hb_face_create_for_tables (_hb_gdi_reference_table, (शून्य *) hfont, nullptr);
}

#endif

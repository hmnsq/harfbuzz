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

#అన్తర్భావ "hb.hh"

#నిర్వచితమైతే HAVE_GDI

#అన్తర్భావ "hb-gdi.h"


/**
 * SECTION:hb-gdi
 * @title: hb-gdi
 * @short_description: GDI integration
 * @include: hb-gdi.h
 *
 * Functions for using HarfBuzz with GDI fonts.
 **/

స్థిర hb_blob_t *
hb_gdi_reference_పట్టిక (hb_face_t *face HB_వాడని, hb_ట్యాగ్_t ట్యాగ్_2, శూన్య *user_దత్తాంశం)
{
  అక్షరం *బఫరు = శూన్యసూచిక;
  DWORD నిడివి = 0;

  HDC hdc = GetDC (శూన్యసూచిక);
  ఒకవేళ (unlikely (!SelectObject (hdc, (HFONT) user_దత్తాంశం)))
    తిరిగి hb_blob_get_ఖాళీలు ();
  HB_SCOPE_GUARD (ReleaseDC (శూన్యసూచిక, hdc));

  నిడివి = Get_Font_దత్తాంశం (hdc, hb_uint32_మార్పిడి (ట్యాగ్_2), 0, బఫరు, నిడివి);
  ఒకవేళ (unlikely (నిడివి == GDI_ERROR))
    తిరిగి hb_blob_get_ఖాళీలు ();

  బఫరు = (అక్షరం *) hb_malloc (నిడివి);
  ఒకవేళ (unlikely (!బఫరు))
    తిరిగి hb_blob_get_ఖాళీలు ();
  స్వయంచాలక బఫరు_guard = hb_make_scope_guard ([&]() { hb_free (బఫరు); });

  నిడివి = Get_Font_దత్తాంశం (hdc, hb_uint32_మార్పిడి (ట్యాగ్_2), 0, బఫరు, నిడివి);
  ఒకవేళ (unlikely (నిడివి == GDI_ERROR))
    తిరిగి hb_blob_get_ఖాళీలు ();

  బఫరు_guard.release ();
  తిరిగి hb_blob_create ((స్థిరరాశి అక్షరం *) బఫరు, నిడివి, HB_జ్ఞాపకం_బహుళకం_WRITABLE, బఫరు, hb_free);
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
  తిరిగి hb_face_create_for_tables (hb_gdi_reference_పట్టిక, (శూన్య *) hfont, శూన్యసూచిక);
}

#అయితే_ముగింపు

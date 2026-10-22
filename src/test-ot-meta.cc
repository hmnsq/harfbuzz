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
#ضمن "hb-ot.h"

#اگر_تعریف_شده HB_NO_OPEN
#تعریف_کن hb_blob_create_from_file_or_fail(x)  hb_blob_get_خالی ()
#پایان_اگر

صحيح
اصلی (صحيح argc, حرف **argv)
{
  إذا (argc != 2) {
    fprintf (stderr, "usage: %s font-file\n", argv[0]);
    خروج (1);
  }

  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_always_assert (blob);
  hb_face_t *face = hb_face_create (blob, 0 /* first face */);
  hb_blob_destroy (blob);
  blob = اشاره_تهی;

  بیعلامت صحيح شمار = 0;

#اگر_تعریف_نشده HB_NO_META
  شمار = hb_ot_meta_get_مدخل_برچسبها (face, 0, اشاره_تهی, اشاره_تهی);

  hb_ot_meta_برچسب_t *برچسبها = (hb_ot_meta_برچسب_t *)
			   حافظه_بده (حجم (hb_ot_meta_برچسب_t) * شمار);
  hb_ot_meta_get_مدخل_برچسبها (face, 0, &شمار, برچسبها);
  لكل (بیعلامت i = 0; i < شمار; ++i)
  {
    hb_blob_t *مدخل = hb_ot_meta_reference_مدخل (face, برچسبها[i]);
    چاپ_کن ("%c%c%c%c, size: %u: %.*s\n",
	    HB_UNTAG (برچسبها[i]), hb_blob_get_درازی (مدخل),
	    (صحيح) hb_blob_get_درازی (مدخل), hb_blob_get_داده (مدخل, اشاره_تهی));
    hb_blob_destroy (مدخل);
  }
  حرر (برچسبها);
#پایان_اگر

  hb_face_destroy (face);

  أرجع !شمار;
}

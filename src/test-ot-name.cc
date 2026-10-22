/*
 * Copyright © 2018  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
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

#اگر_تعریف_نشده HB_NO_NAME
  ثابت hb_ot_name_مدخل_t *entries = hb_ot_name_list_نامها (face, &شمار);

  لكل (بیعلامت صحيح i = 0; i < شمار; i++)
  {
    چاپ_کن ("%u	%s	",
	    entries[i].name_id,
	    hb_language_مقصد_string (entries[i].language));

    حرف buf[64];
    بیعلامت صحيح buf_اندازه = حجم (buf);
    hb_ot_name_get_utf8 (face,
			 entries[i].name_id,
			 entries[i].language,
			 &buf_اندازه,
			 buf);

    چاپ_کن ("%s\n", buf);
  }
#پایان_اگر

  hb_face_destroy (face);

  أرجع شمار ? 0 : 1;
}

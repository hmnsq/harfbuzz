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

#جيڪڏهن_وصف_ٿيل HB_NO_OPEN
#وصف_ڪريو hb_blob_create_from_file_or_fail(x)  hb_blob_get_خالي_جايون ()
#جيڪڏهن_پورو

صحيح
مکيه (صحيح argc, حرف **argv)
{
  إذا (argc != 2) {
    fprintf (stderr, "usage: %s font-file\n", argv[0]);
    ٻاهر_نڪرو (1);
  }

  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_always_assert (blob);
  hb_face_t *face = hb_face_create (blob, 0 /* first face */);
  hb_blob_destroy (blob);
  blob = خالي_اشارو;

  بنانشان صحيح ڳڻپ = 0;

#جيڪڏهن_وصف_نه_ٿيل HB_NO_NAME
  ثابت hb_ot_name_داخلا_t *entries = hb_ot_name_list_نالا (face, &ڳڻپ);

  لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
  {
    لکو ("%u	%s	",
	    entries[i].name_id,
	    hb_language_منزل_string (entries[i].language));

    حرف buf[64];
    بنانشان صحيح buf_سائيز = حجم (buf);
    hb_ot_name_get_utf8 (face,
			 entries[i].name_id,
			 entries[i].language,
			 &buf_سائيز,
			 buf);

    لکو ("%s\n", buf);
  }
#جيڪڏهن_پورو

  hb_face_destroy (face);

  أرجع ڳڻپ ? 0 : 1;
}

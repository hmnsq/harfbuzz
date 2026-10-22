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
#شامل "hb-ot.h"

#اگر_تعریف_شدہ HB_NO_OPEN
#تعریف_کرو hb_blob_create_from_file_or_fail(x)  hb_blob_get_خالی_خانے ()
#اگر_ختم

صحیح
مین (صحیح argc, حرف **argv)
{
  اگر (argc != 2) {
    fprintf (stderr, "usage: %s font-file\n", argv[0]);
    باہر_نکلو (1);
  }

  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_always_assert (blob);
  hb_face_t *face = hb_face_create (blob, 0 /* first face */);
  hb_blob_destroy (blob);
  blob = خالی_اشاریہ;

  غیرعلامتی صحیح گنتی = 0;

#اگر_غیر_تعریف_شدہ HB_NO_META
  گنتی = hb_ot_meta_get_اندراج_لیبل_فہرست (face, 0, خالی_اشاریہ, خالی_اشاریہ);

  hb_ot_meta_لیبل_t *لیبل_فہرست = (hb_ot_meta_لیبل_t *)
			   حافظہ_دو (حجم (hb_ot_meta_لیبل_t) * گنتی);
  hb_ot_meta_get_اندراج_لیبل_فہرست (face, 0, &گنتی, لیبل_فہرست);
  برائے (غیرعلامتی i = 0; i < گنتی; ++i)
  {
    hb_blob_t *اندراج_2 = hb_ot_meta_reference_اندراج (face, لیبل_فہرست[i]);
    لکھو ("%c%c%c%c, size: %u: %.*s\n",
	    HB_UNTAG (لیبل_فہرست[i]), hb_blob_get_لمبائی (اندراج_2),
	    (صحیح) hb_blob_get_لمبائی (اندراج_2), hb_blob_get_ڈیٹا (اندراج_2, خالی_اشاریہ));
    hb_blob_destroy (اندراج_2);
  }
  آزاد (لیبل_فہرست);
#اگر_ختم

  hb_face_destroy (face);

  واپس !گنتی;
}

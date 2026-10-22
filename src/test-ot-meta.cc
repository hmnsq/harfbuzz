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

#که_تعریف_شوی HB_NO_OPEN
#تعریف_کړه hb_blob_create_from_file_or_fail(x)  hb_blob_get_تشه ()
#د_که_پای

صحيح
اصلي (صحيح argc, حرف **argv)
{
  إذا (argc != 2) {
    fprintf (stderr, "usage: %s font-file\n", argv[0]);
    وتل (1);
  }

  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_always_assert (blob);
  hb_face_t *face = hb_face_create (blob, 0 /* first face */);
  hb_blob_destroy (blob);
  blob = تش_اشاره;

  بینښه صحيح شمېر = 0;

#که_نه_تعریف_شوی HB_NO_META
  شمېر = hb_ot_meta_get_ثبت_نښانونه (face, 0, تش_اشاره, تش_اشاره);

  hb_ot_meta_نښان_t *نښانونه = (hb_ot_meta_نښان_t *)
			   حافظه_ورکړه (حجم (hb_ot_meta_نښان_t) * شمېر);
  hb_ot_meta_get_ثبت_نښانونه (face, 0, &شمېر, نښانونه);
  لكل (بینښه i = 0; i < شمېر; ++i)
  {
    hb_blob_t *ثبت = hb_ot_meta_reference_ثبت (face, نښانونه[i]);
    چاپ_کړه ("%c%c%c%c, size: %u: %.*s\n",
	    HB_UNTAG (نښانونه[i]), hb_blob_get_اوږدتیا (ثبت),
	    (صحيح) hb_blob_get_اوږدتیا (ثبت), hb_blob_get_معلومات (ثبت, تش_اشاره));
    hb_blob_destroy (ثبت);
  }
  حرر (نښانونه);
#د_که_پای

  hb_face_destroy (face);

  أرجع !شمېر;
}

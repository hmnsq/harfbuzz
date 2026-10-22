/*
 * Copyright © 2010,2011  Google, Inc.
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

#ضمن <hb.h>
#ضمن <hb-ot.h>

#ضمن <stdlib.h>
#ضمن <stdio.h>

صحيح
اصلی (صحيح argc, حرف **argv)
{
  إذا (argc != 3) {
    fprintf (stderr, "usage: %s font-file text\n", argv[0]);
    خروج (1);
  }

  /* Create the face */
  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_face_t *face = hb_face_create (blob, 0 /* first face */);
  hb_blob_destroy (blob);
  blob = اشاره_تهی;

  hb_font_t *font = hb_font_create (face);
  hb_میانگیر_t *میانگیر = hb_میانگیر_create ();

  hb_میانگیر_جمع_کن_utf8 (میانگیر, argv[2], -1, 0, -1);
  hb_میانگیر_guess_segment_properties (میانگیر);
  hb_shape (font, میانگیر, عدم, 0);

  hb_برچسب_t features[] = {HB_برچسب('a','a','l','t'), HB_برچسب_NONE};
  hb_مجموعه_t *lookup_indexes = hb_مجموعه_create ();
  hb_ot_layout_collect_lookups (face,
				HB_OT_برچسب_GSUB,
				عدم, عدم,
				features,
				lookup_indexes);
  چاپ_کن ("lookups %u\n", hb_مجموعه_get_population (lookup_indexes));

  بیعلامت شمار;
  hb_glyph_info_t *info = hb_میانگیر_get_glyph_infos (میانگیر, &شمار);
  لكل (بیعلامت i = 0; i < شمار; i++)
  {
    بیعلامت alt_شمار = 0;
    لكل (hb_codepoint_t lookup_شاخص = HB_مجموعه_مقدار_INVALID;
	 hb_مجموعه_بعدی (lookup_indexes, &lookup_شاخص);)
      إذا ((alt_شمار = hb_ot_layout_lookup_get_glyph_alternates (face,
								 lookup_شاخص,
								 info[i].codepoint,
								 0,
								 عدم,
								 عدم)))
        اكسر;
    چاپ_کن ("glyph %u alt count %u\n", info[i].codepoint, alt_شمار);
  }

  hb_مجموعه_destroy (lookup_indexes);
  hb_میانگیر_destroy (میانگیر);
  hb_font_destroy (font);
  hb_face_destroy (face);

  أرجع 0;
}

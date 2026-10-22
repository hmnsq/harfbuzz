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
اصلي (صحيح argc, حرف **argv)
{
  إذا (argc != 3) {
    fprintf (stderr, "usage: %s font-file text\n", argv[0]);
    وتل (1);
  }

  /* Create the face */
  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_face_t *face = hb_face_create (blob, 0 /* first face */);
  hb_blob_destroy (blob);
  blob = تش_اشاره;

  hb_font_t *font = hb_font_create (face);
  hb_بفر_t *بفر = hb_بفر_create ();

  hb_بفر_جمع_کړه_utf8 (بفر, argv[2], -1, 0, -1);
  hb_بفر_guess_segment_properties (بفر);
  hb_shape (font, بفر, عدم, 0);

  hb_نښان_t features[] = {HB_نښان('a','a','l','t'), HB_نښان_NONE};
  hb_سېټ_t *lookup_indexes = hb_سېټ_create ();
  hb_ot_layout_collect_lookups (face,
				HB_OT_نښان_GSUB,
				عدم, عدم,
				features,
				lookup_indexes);
  چاپ_کړه ("lookups %u\n", hb_سېټ_get_population (lookup_indexes));

  بینښه شمېر;
  hb_glyph_info_t *info = hb_بفر_get_glyph_infos (بفر, &شمېر);
  لكل (بینښه i = 0; i < شمېر; i++)
  {
    بینښه alt_شمېر = 0;
    لكل (hb_codepoint_t lookup_شاخص = HB_سېټ_ارزښت_INVALID;
	 hb_سېټ_بل (lookup_indexes, &lookup_شاخص);)
      إذا ((alt_شمېر = hb_ot_layout_lookup_get_glyph_alternates (face,
								 lookup_شاخص,
								 info[i].codepoint,
								 0,
								 عدم,
								 عدم)))
        اكسر;
    چاپ_کړه ("glyph %u alt count %u\n", info[i].codepoint, alt_شمېر);
  }

  hb_سېټ_destroy (lookup_indexes);
  hb_بفر_destroy (بفر);
  hb_font_destroy (font);
  hb_face_destroy (face);

  أرجع 0;
}

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
مکھ (صحيح argc, حرف **argv)
{
  إذا (argc != 3) {
    fprintf (stderr, "usage: %s font-file text\n", argv[0]);
    باہر_نکلو (1);
  }

  /* Create the face */
  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_face_t *face = hb_face_create (blob, 0 /* first face */);
  hb_blob_destroy (blob);
  blob = خالی_سوچک;

  hb_font_t *font = hb_font_create (face);
  hb_بفر_t *بفر = hb_بفر_create ();

  hb_بفر_جوڑو_utf8 (بفر, argv[2], -1, 0, -1);
  hb_بفر_guess_segment_properties (بفر);
  hb_shape (font, بفر, عدم, 0);

  hb_ٹیگ_t features[] = {HB_ٹیگ('a','a','l','t'), HB_ٹیگ_NONE};
  hb_سیٹ_t *lookup_indexes = hb_سیٹ_create ();
  hb_ot_layout_collect_lookups (face,
				HB_OT_ٹیگ_GSUB,
				عدم, عدم,
				features,
				lookup_indexes);
  لکھو ("lookups %u\n", hb_سیٹ_get_population (lookup_indexes));

  بنانشانی گنتی;
  hb_glyph_info_t *info = hb_بفر_get_glyph_infos (بفر, &گنتی);
  لكل (بنانشانی i = 0; i < گنتی; i++)
  {
    بنانشانی alt_گنتی = 0;
    لكل (hb_codepoint_t lookup_اشاریہ = HB_سیٹ_قدر_INVALID;
	 hb_سیٹ_اگلا (lookup_indexes, &lookup_اشاریہ);)
      إذا ((alt_گنتی = hb_ot_layout_lookup_get_glyph_alternates (face,
								 lookup_اشاریہ,
								 info[i].codepoint,
								 0,
								 عدم,
								 عدم)))
        اكسر;
    لکھو ("glyph %u alt count %u\n", info[i].codepoint, alt_گنتی);
  }

  hb_سیٹ_destroy (lookup_indexes);
  hb_بفر_destroy (بفر);
  hb_font_destroy (font);
  hb_face_destroy (face);

  أرجع 0;
}

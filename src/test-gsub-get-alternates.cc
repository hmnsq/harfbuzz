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

#অন্তর্ভাব <hb.h>
#অন্তর্ভাব <hb-ot.h>

#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <stdio.h>

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  যদি (argc != 3) {
    fprintf (stderr, "usage: %s font-file text\n", argv[0]);
    exit (1);
  }

  /* Create the face */
  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_face_t *face = hb_face_create (blob, 0 /* first face */);
  hb_blob_destroy (blob);
  blob = nullptr;

  hb_font_t *font = hb_font_create (face);
  hb_বাফার_t *বাফার = hb_বাফার_create ();

  hb_বাফার_add_utf8 (বাফার, argv[2], -1, 0, -1);
  hb_বাফার_guess_segment_properties (বাফার);
  hb_shape (font, বাফার, ফাঁকা, 0);

  hb_tag_t features[] = {HB_TAG('a','a','l','t'), HB_TAG_NONE};
  hb_set_t *lookup_indexes = hb_set_create ();
  hb_ot_layout_collect_lookups (face,
				HB_OT_TAG_GSUB,
				ফাঁকা, ফাঁকা,
				features,
				lookup_indexes);
  printf ("lookups %u\n", hb_set_get_population (lookup_indexes));

  unsigned গুনতি;
  hb_glyph_info_t *info = hb_বাফার_get_glyph_infos (বাফার, &গুনতি);
  জন্য (unsigned i = 0; i < গুনতি; i++)
  {
    unsigned alt_গুনতি = 0;
    জন্য (hb_codepoint_t lookup_সূচক = HB_SET_মান_INVALID;
	 hb_set_পরবর্তী (lookup_indexes, &lookup_সূচক);)
      যদি ((alt_গুনতি = hb_ot_layout_lookup_get_glyph_alternates (face,
								 lookup_সূচক,
								 info[i].codepoint,
								 0,
								 ফাঁকা,
								 ফাঁকা)))
        বিরতি;
    printf ("glyph %u alt count %u\n", info[i].codepoint, alt_গুনতি);
  }

  hb_set_destroy (lookup_indexes);
  hb_বাফার_destroy (বাফার);
  hb_font_destroy (font);
  hb_face_destroy (face);

  ফেরত 0;
}

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

#അന്തര്ഭാവ <hb.h>
#അന്തര്ഭാവ <hb-ot.h>

#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <stdio.h>

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  എങ്കിൽ (argc != 3) {
    fprintf (stderr, "usage: %s font-file text\n", argv[0]);
    exit (1);
  }

  /* Create the face */
  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_face_t *face = hb_face_create (blob, 0 /* first face */);
  hb_blob_destroy (blob);
  blob = nullptr;

  hb_font_t *font = hb_font_create (face);
  hb_buffer_t *buffer = hb_buffer_create ();

  hb_buffer_add_utf8 (buffer, argv[2], -1, 0, -1);
  hb_buffer_guess_segment_properties (buffer);
  hb_shape (font, buffer, ശൂന്യ, 0);

  hb_tag_t features[] = {HB_TAG('a','a','l','t'), HB_TAG_NONE};
  hb_set_t *lookup_indexes = hb_set_create ();
  hb_ot_layout_collect_lookups (face,
				HB_OT_TAG_GSUB,
				ശൂന്യ, ശൂന്യ,
				features,
				lookup_indexes);
  printf ("lookups %u\n", hb_set_get_population (lookup_indexes));

  unsigned എണ്ണം;
  hb_glyph_info_t *info = hb_buffer_get_glyph_infos (buffer, &എണ്ണം);
  വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
  {
    unsigned alt_എണ്ണം = 0;
    വേണ്ടി (hb_codepoint_t lookup_index = HB_SET_മൂല്യം_INVALID;
	 hb_set_next (lookup_indexes, &lookup_index);)
      എങ്കിൽ ((alt_എണ്ണം = hb_ot_layout_lookup_get_glyph_alternates (face,
								 lookup_index,
								 info[i].codepoint,
								 0,
								 ശൂന്യ,
								 ശൂന്യ)))
        നിർത്തുക;
    printf ("glyph %u alt count %u\n", info[i].codepoint, alt_എണ്ണം);
  }

  hb_set_destroy (lookup_indexes);
  hb_buffer_destroy (buffer);
  hb_font_destroy (font);
  hb_face_destroy (face);

  തിരികെ 0;
}

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

#ಅನ್ತರ್ಭಾವ <hb.h>
#ಅನ್ತರ್ಭಾವ <hb-ot.h>

#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ಯದಿ (argc != 3) {
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
  hb_shape (font, buffer, ರಿಕ್ತ, 0);

  hb_tag_t features[] = {HB_TAG('a','a','l','t'), HB_TAG_NONE};
  hb_set_t *lookup_indexes = hb_set_create ();
  hb_ot_layout_collect_lookups (face,
				HB_OT_TAG_GSUB,
				ರಿಕ್ತ, ರಿಕ್ತ,
				features,
				lookup_indexes);
  printf ("lookups %u\n", hb_set_get_population (lookup_indexes));

  unsigned ಎಣಿಕೆ;
  hb_glyph_info_t *info = hb_buffer_get_glyph_infos (buffer, &ಎಣಿಕೆ);
  ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
  {
    unsigned alt_ಎಣಿಕೆ = 0;
    ಪ್ರತಿ (hb_codepoint_t lookup_index = HB_SET_ಮೌಲ್ಯ_INVALID;
	 hb_set_next (lookup_indexes, &lookup_index);)
      ಯದಿ ((alt_ಎಣಿಕೆ = hb_ot_layout_lookup_get_glyph_alternates (face,
								 lookup_index,
								 info[i].codepoint,
								 0,
								 ರಿಕ್ತ,
								 ರಿಕ್ತ)))
        ಭಙ್ಗ;
    printf ("glyph %u alt count %u\n", info[i].codepoint, alt_ಎಣಿಕೆ);
  }

  hb_set_destroy (lookup_indexes);
  hb_buffer_destroy (buffer);
  hb_font_destroy (font);
  hb_face_destroy (face);

  ಪ್ರತ್ಯಾಗಮ 0;
}

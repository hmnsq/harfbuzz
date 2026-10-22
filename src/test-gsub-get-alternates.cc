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

#ਅਨ੍ਤਰ੍ਭਾਵ <hb.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <hb-ot.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਜੇ (argc != 3) {
    fprintf (stderr, "usage: %s font-file text\n", argv[0]);
    ਬਾਹਰ_ਨਿਕਲੋ (1);
  }

  /* Create the face */
  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_face_t *face = hb_face_create (blob, 0 /* first face */);
  hb_blob_destroy (blob);
  blob = ਖਾਲੀ_ਸੂਚਕ;

  hb_font_t *font = hb_font_create (face);
  hb_ਬਫਰ_t *ਬਫਰ = hb_ਬਫਰ_create ();

  hb_ਬਫਰ_ਜੋੜੋ_utf8 (ਬਫਰ, argv[2], -1, 0, -1);
  hb_ਬਫਰ_guess_segment_properties (ਬਫਰ);
  hb_shape (font, ਬਫਰ, ਨਿਰਾ, 0);

  hb_ਟੈਗ_t features[] = {HB_ਟੈਗ('a','a','l','t'), HB_ਟੈਗ_NONE};
  hb_ਸੈੱਟ_t *lookup_indexes = hb_ਸੈੱਟ_create ();
  hb_ot_layout_collect_lookups (face,
				HB_OT_ਟੈਗ_GSUB,
				ਨਿਰਾ, ਨਿਰਾ,
				features,
				lookup_indexes);
  ਲਿਖੋ ("lookups %u\n", hb_ਸੈੱਟ_get_population (lookup_indexes));

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2;
  hb_glyph_info_t *info = hb_ਬਫਰ_get_glyph_infos (ਬਫਰ, &ਗਿਣਤੀ_2);
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ alt_ਗਿਣਤੀ = 0;
    ਲਈ (hb_codepoint_t lookup_ਸੂਚਕਾਂਕ = HB_ਸੈੱਟ_ਮੁੱਲ_INVALID;
	 hb_ਸੈੱਟ_ਅਗਲਾ (lookup_indexes, &lookup_ਸੂਚਕਾਂਕ);)
      ਜੇ ((alt_ਗਿਣਤੀ = hb_ot_layout_lookup_get_glyph_alternates (face,
								 lookup_ਸੂਚਕਾਂਕ,
								 info[i].codepoint,
								 0,
								 ਨਿਰਾ,
								 ਨਿਰਾ)))
        ਰੋਕੋ;
    ਲਿਖੋ ("glyph %u alt count %u\n", info[i].codepoint, alt_ਗਿਣਤੀ);
  }

  hb_ਸੈੱਟ_destroy (lookup_indexes);
  hb_ਬਫਰ_destroy (ਬਫਰ);
  hb_font_destroy (font);
  hb_face_destroy (face);

  ਵਾਪਸ 0;
}

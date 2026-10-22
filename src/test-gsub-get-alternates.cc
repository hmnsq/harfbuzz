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

#అన్తర్భావ <hb.h>
#అన్తర్భావ <hb-ot.h>

#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <stdio.h>

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  ఒకవేళ (argc != 3) {
    fprintf (stderr, "usage: %s font-file text\n", argv[0]);
    నిష్క్రమించు (1);
  }

  /* Create the face */
  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_face_t *face = hb_face_create (blob, 0 /* first face */);
  hb_blob_destroy (blob);
  blob = శూన్యసూచిక;

  hb_font_t *font = hb_font_create (face);
  hb_బఫరు_t *బఫరు = hb_బఫరు_create ();

  hb_బఫరు_కూడు_utf8 (బఫరు, argv[2], -1, 0, -1);
  hb_బఫరు_guess_segment_properties (బఫరు);
  hb_shape (font, బఫరు, ఖాళీ, 0);

  hb_ట్యాగ్_t features[] = {HB_ట్యాగ్('a','a','l','t'), HB_ట్యాగ్_NONE};
  hb_సమితి_t *lookup_indexes = hb_సమితి_create ();
  hb_ot_layout_collect_lookups (face,
				HB_OT_ట్యాగ్_GSUB,
				ఖాళీ, ఖాళీ,
				features,
				lookup_indexes);
  రాయి ("lookups %u\n", hb_సమితి_get_population (lookup_indexes));

  గుర్తులేని లెక్క;
  hb_glyph_info_t *info = hb_బఫరు_get_glyph_infos (బఫరు, &లెక్క);
  కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
  {
    గుర్తులేని alt_లెక్క = 0;
    కొరకు (hb_codepoint_t lookup_సూచీ = HB_సమితి_విలువ_INVALID;
	 hb_సమితి_తదుపరి (lookup_indexes, &lookup_సూచీ);)
      ఒకవేళ ((alt_లెక్క = hb_ot_layout_lookup_get_glyph_alternates (face,
								 lookup_సూచీ,
								 info[i].codepoint,
								 0,
								 ఖాళీ,
								 ఖాళీ)))
        ఆపు;
    రాయి ("glyph %u alt count %u\n", info[i].codepoint, alt_లెక్క);
  }

  hb_సమితి_destroy (lookup_indexes);
  hb_బఫరు_destroy (బఫరు);
  hb_font_destroy (font);
  hb_face_destroy (face);

  తిరిగి 0;
}

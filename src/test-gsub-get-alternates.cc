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

#अन्तर्भाव <hb.h>
#अन्तर्भाव <hb-ot.h>

#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <stdio.h>

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  जर (argc != 3) {
    ख_लिखो (मानक_त्रुटि, "usage: %s font-file text\n", argv[0]);
    निकास (1);
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
  hb_shape (font, buffer, रिक्त, 0);

  hb_tag_t features[] = {HB_TAG('a','a','l','t'), HB_TAG_NONE};
  hb_set_t *lookup_indexes = hb_set_create ();
  hb_ot_layout_collect_lookups (face,
				HB_OT_TAG_GSUB,
				रिक्त, रिक्त,
				features,
				lookup_indexes);
  म_लिखो ("lookups %u\n", hb_set_get_population (lookup_indexes));

  अचिन्हित मोजणी;
  hb_glyph_info_t *info = hb_buffer_get_glyph_infos (buffer, &मोजणी);
  साठी (अचिन्हित i = 0; i < मोजणी; i++)
  {
    अचिन्हित alt_मोजणी = 0;
    साठी (hb_codepoint_t lookup_index = HB_SET_मूल्य_INVALID;
	 hb_set_next (lookup_indexes, &lookup_index);)
      जर ((alt_मोजणी = hb_ot_layout_lookup_get_glyph_alternates (face,
								 lookup_index,
								 info[i].codepoint,
								 0,
								 रिक्त,
								 रिक्त)))
        थांबा;
    म_लिखो ("glyph %u alt count %u\n", info[i].codepoint, alt_मोजणी);
  }

  hb_set_destroy (lookup_indexes);
  hb_buffer_destroy (buffer);
  hb_font_destroy (font);
  hb_face_destroy (face);

  परत 0;
}

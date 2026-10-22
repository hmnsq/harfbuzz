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

#समावेश <hb.h>
#समावेश <hb-ot.h>

#समावेश <stdlib.h>
#समावेश <stdio.h>

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  अगर (argc != 3) {
    ख_लिखो (मानक_त्रुटि, "usage: %s font-file text\n", argv[0]);
    निकास (1);
  }

  /* Create the face */
  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_face_t *face = hb_face_create (blob, 0 /* first face */);
  hb_blob_destroy (blob);
  blob = nullptr;

  hb_font_t *font = hb_font_create (face);
  hb_बफर_t *बफर = hb_बफर_create ();

  hb_बफर_add_utf8 (बफर, argv[2], -1, 0, -1);
  hb_बफर_guess_segment_properties (बफर);
  hb_shape (font, बफर, शून्य, 0);

  hb_tag_t features[] = {HB_TAG('a','a','l','t'), HB_TAG_NONE};
  hb_set_t *lookup_indexes = hb_set_create ();
  hb_ot_layout_collect_lookups (face,
				HB_OT_TAG_GSUB,
				शून्य, शून्य,
				features,
				lookup_indexes);
  म_लिखो ("lookups %u\n", hb_set_get_population (lookup_indexes));

  अचिन्हित गिनती;
  hb_glyph_info_t *info = hb_बफर_get_glyph_infos (बफर, &गिनती);
  क्रम (अचिन्हित i = 0; i < गिनती; i++)
  {
    अचिन्हित alt_गिनती = 0;
    क्रम (hb_codepoint_t lookup_सूचकांक = HB_SET_मूल्य_INVALID;
	 hb_set_अग्र (lookup_indexes, &lookup_सूचकांक);)
      अगर ((alt_गिनती = hb_ot_layout_lookup_get_glyph_alternates (face,
								 lookup_सूचकांक,
								 info[i].codepoint,
								 0,
								 शून्य,
								 शून्य)))
        अवरोध;
    म_लिखो ("glyph %u alt count %u\n", info[i].codepoint, alt_गिनती);
  }

  hb_set_destroy (lookup_indexes);
  hb_बफर_destroy (बफर);
  hb_font_destroy (font);
  hb_face_destroy (face);

  वापस 0;
}

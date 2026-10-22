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

#समावेश "hb.hh"

#समावेश "hb.h"
#समावेश "hb-ot.h"

#अगर_घोषित HAVE_FREETYPE
#समावेश "hb-ft.h"
#पूर्ण_अगर

#अगर_घोषित HB_NO_OPEN
#घोषणा hb_blob_create_from_file_or_fail(x)  hb_blob_get_empty ()
#पूर्ण_अगर

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  अगर (argc != 4 && argc != 5) {
    ख_लिखो (मानक_त्रुटि, "usage: %s font-file lookup-index first-glyph [second-glyph]\n", argv[0]);
    निकास (1);
  }

  /* Create the face */
  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_always_assert (blob);
  hb_face_t *face = hb_face_create (blob, 0 /* first face */);
  hb_blob_destroy (blob);
  blob = nullptr;

  hb_font_t *font = hb_font_create (face);
#अगर_घोषित HAVE_FREETYPE
  hb_ft_font_set_funcs (font);
#पूर्ण_अगर

  अचिन्हित पूर्णांक len = argc - 3;
  hb_codepoint_t glyphs[2];
  अगर (!hb_font_glyph_from_string (font, argv[3], -1, &glyphs[0]) ||
      (argc > 4 &&
       !hb_font_glyph_from_string (font, argv[4], -1, &glyphs[1])))
    वापस 2;
  वापस !hb_ot_layout_lookup_would_substitute (face, म_से_दीर्घ (argv[2], nullptr, 0), glyphs, len, false);
}

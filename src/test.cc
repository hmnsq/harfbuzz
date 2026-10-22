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

#अगर_घोषित HAVE_FREETYPE
#समावेश "hb-ft.h"
#पूर्ण_अगर

#अगर_घोषित HB_NO_OPEN
#घोषणा hb_blob_create_from_file_or_fail(x)  hb_blob_get_empty ()
#पूर्ण_अगर

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  अगर (argc != 2) {
    ख_लिखो (मानक_त्रुटि, "usage: %s font-file.ttf\n", argv[0]);
    निकास (1);
  }

  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  निश्चित (blob);
  म_लिखो ("Opened font file %s: %u bytes long\n", argv[1], hb_blob_get_लंबाई (blob));

  /* Create the face */
  hb_face_t *face = hb_face_create (blob, 0 /* first face */);
  hb_blob_destroy (blob);
  blob = nullptr;
  अचिन्हित पूर्णांक upem = hb_face_get_upem (face);

  hb_font_t *font = hb_font_create (face);
  hb_font_set_scale (font, upem, upem);

#अगर_घोषित HAVE_FREETYPE
  hb_ft_font_set_funcs (font);
#पूर्ण_अगर

  hb_बफर_t *बफर = hb_बफर_create ();

  hb_बफर_add_utf8 (बफर, "\xe0\xa4\x95\xe0\xa5\x8d\xe0\xa4\xb0\xe0\xa5\x8d\xe0\xa4\x95", -1, 0, -1);
  hb_बफर_guess_segment_properties (बफर);

  hb_shape (font, बफर, nullptr, 0);

  अचिन्हित पूर्णांक गिनती = hb_बफर_get_लंबाई (बफर);
  hb_glyph_info_t *infos = hb_बफर_get_glyph_infos (बफर, nullptr);
  hb_glyph_position_t *positions = hb_बफर_get_glyph_positions (बफर, nullptr);

  क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
  {
    hb_glyph_info_t *info = &infos[i];
    hb_glyph_position_t *pos = &positions[i];

    म_लिखो ("cluster %u	glyph 0x%x at	(%d,%d)+(%d,%d)\n",
	    info->cluster,
	    info->codepoint,
	    pos->x_offset,
	    pos->y_offset,
	    pos->x_advance,
	    pos->y_advance);

  }

  hb_बफर_destroy (बफर);
  hb_font_destroy (font);
  hb_face_destroy (face);

  वापस 0;
}



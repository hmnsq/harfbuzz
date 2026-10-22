/*
 * Copyright © 2019  Adobe, Inc.
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
 * Adobe Author(s): Michiharu Ariza
 */

#समावेश "hb.hh"
#समावेश "hb-ot.h"

#अगर_घोषित HB_NO_OPEN
#घोषणा hb_blob_create_from_file_or_fail(x)  hb_blob_get_empty ()
#पूर्ण_अगर

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  अगर (argc != 2) {
    ख_लिखो (मानक_त्रुटि, "usage: %s font-file\n", argv[0]);
    निकास (1);
  }

  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_always_assert (blob);
  hb_face_t *face = hb_face_create (blob, 0 /* first face */);
  hb_font_t *font = hb_font_create (face);
  hb_blob_destroy (blob);
  blob = nullptr;
  

  स्थिर अचिन्हित पूर्णांक अंक_glyphs = hb_face_get_glyph_गिनती (face);
  पूर्णांक	परिणाम = 1;

  क्रम (hb_codepoint_t gid = 0; gid < अंक_glyphs; gid++)
  {
    अक्षर buf[64];
    अचिन्हित पूर्णांक buf_आकार = माप (buf);
    अगर (hb_font_get_glyph_name (font, gid, buf, buf_आकार))
    {
      hb_codepoint_t	gid_inv;
      अगर (hb_font_get_glyph_from_name(font, buf, म_माप (buf), &gid_inv))
      {
	अगर (gid == gid_inv)
	{
	  म_लिखो ("%u <-> %s\n", gid, buf);
	}
	अन्यथा
	{
	  म_लिखो ("%u -> %s -> %u\n", gid, buf, gid_inv);
	  परिणाम = 0;
	}
      }
      अन्यथा
      {
	म_लिखो ("%u -> %s -> ?\n", gid, buf);
	परिणाम = 0;
      }
    }
    अन्यथा
    {
      म_लिखो ("%u -> ?\n", gid);
      परिणाम = 0;
    }
  }

  hb_font_destroy (font);
  hb_face_destroy (face);

  वापस !परिणाम;
}

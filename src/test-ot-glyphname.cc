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

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot.h"

#ifdef HB_NO_OPEN
#define hb_blob_create_from_file_or_fail(x)  hb_blob_get_empty ()
#endif

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ଯଦି (argc != 2) {
    fprintf (stderr, "usage: %s font-file\n", argv[0]);
    exit (1);
  }

  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_always_assert (blob);
  hb_face_t *face = hb_face_create (blob, 0 /* first face */);
  hb_font_t *font = hb_font_create (face);
  hb_blob_destroy (blob);
  blob = nullptr;
  

  ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs = hb_face_get_glyph_ଗଣନା (face);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା	result = 1;

  ପାଇଁ (hb_codepoint_t gid = 0; gid < ଅଙ୍କ_glyphs; gid++)
  {
    ଅକ୍ଷର buf[64];
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା buf_size = ଆକାର (buf);
    ଯଦି (hb_font_get_glyph_name (font, gid, buf, buf_size))
    {
      hb_codepoint_t	gid_inv;
      ଯଦି (hb_font_get_glyph_from_name(font, buf, strlen (buf), &gid_inv))
      {
	ଯଦି (gid == gid_inv)
	{
	  printf ("%u <-> %s\n", gid, buf);
	}
	ଅନ୍ୟଥା
	{
	  printf ("%u -> %s -> %u\n", gid, buf, gid_inv);
	  result = 0;
	}
      }
      ଅନ୍ୟଥା
      {
	printf ("%u -> %s -> ?\n", gid, buf);
	result = 0;
      }
    }
    ଅନ୍ୟଥା
    {
      printf ("%u -> ?\n", gid);
      result = 0;
    }
  }

  hb_font_destroy (font);
  hb_face_destroy (face);

  ଫେରନ୍ତୁ !result;
}

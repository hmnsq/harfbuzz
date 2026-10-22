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

#شامل "hb.hh"
#شامل "hb-ot.h"

#اگر_تعریف_شدہ HB_NO_OPEN
#تعریف_کرو hb_blob_create_from_file_or_fail(x)  hb_blob_get_خالی_خانے ()
#اگر_ختم

صحیح
مین (صحیح argc, حرف **argv)
{
  اگر (argc != 2) {
    fprintf (stderr, "usage: %s font-file\n", argv[0]);
    باہر_نکلو (1);
  }

  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_always_assert (blob);
  hb_face_t *face = hb_face_create (blob, 0 /* first face */);
  hb_font_t *font = hb_font_create (face);
  hb_blob_destroy (blob);
  blob = خالی_اشاریہ;
  

  ثابت غیرعلامتی صحیح عدد_glyphs = hb_face_get_glyph_گنتی (face);
  صحیح	نتیجہ = 1;

  برائے (hb_codepoint_t gid = 0; gid < عدد_glyphs; gid++)
  {
    حرف buf[64];
    غیرعلامتی صحیح buf_سائز = حجم (buf);
    اگر (hb_font_get_glyph_name (font, gid, buf, buf_سائز))
    {
      hb_codepoint_t	gid_inv;
      اگر (hb_font_get_glyph_from_name(font, buf, لڑی_لمبائی (buf), &gid_inv))
      {
	اگر (gid == gid_inv)
	{
	  لکھو ("%u <-> %s\n", gid, buf);
	}
	ورنہ
	{
	  لکھو ("%u -> %s -> %u\n", gid, buf, gid_inv);
	  نتیجہ = 0;
	}
      }
      ورنہ
      {
	لکھو ("%u -> %s -> ?\n", gid, buf);
	نتیجہ = 0;
      }
    }
    ورنہ
    {
      لکھو ("%u -> ?\n", gid);
      نتیجہ = 0;
    }
  }

  hb_font_destroy (font);
  hb_face_destroy (face);

  واپس !نتیجہ;
}

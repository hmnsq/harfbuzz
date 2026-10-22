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

#ضمن "hb.hh"
#ضمن "hb-ot.h"

#جيڪڏهن_وصف_ٿيل HB_NO_OPEN
#وصف_ڪريو hb_blob_create_from_file_or_fail(x)  hb_blob_get_خالي_جايون ()
#جيڪڏهن_پورو

صحيح
مکيه (صحيح argc, حرف **argv)
{
  إذا (argc != 2) {
    fprintf (stderr, "usage: %s font-file\n", argv[0]);
    ٻاهر_نڪرو (1);
  }

  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_always_assert (blob);
  hb_face_t *face = hb_face_create (blob, 0 /* first face */);
  hb_font_t *font = hb_font_create (face);
  hb_blob_destroy (blob);
  blob = خالي_اشارو;
  

  ثابت بنانشان صحيح انگ_glyphs = hb_face_get_glyph_ڳڻپ (face);
  صحيح	نتيجو = 1;

  لكل (hb_codepoint_t gid = 0; gid < انگ_glyphs; gid++)
  {
    حرف buf[64];
    بنانشان صحيح buf_سائيز = حجم (buf);
    إذا (hb_font_get_glyph_name (font, gid, buf, buf_سائيز))
    {
      hb_codepoint_t	gid_inv;
      إذا (hb_font_get_glyph_from_name(font, buf, لڙي_ڊيگهه (buf), &gid_inv))
      {
	إذا (gid == gid_inv)
	{
	  لکو ("%u <-> %s\n", gid, buf);
	}
	وإلا
	{
	  لکو ("%u -> %s -> %u\n", gid, buf, gid_inv);
	  نتيجو = 0;
	}
      }
      وإلا
      {
	لکو ("%u -> %s -> ?\n", gid, buf);
	نتيجو = 0;
      }
    }
    وإلا
    {
      لکو ("%u -> ?\n", gid);
      نتيجو = 0;
    }
  }

  hb_font_destroy (font);
  hb_face_destroy (face);

  أرجع !نتيجو;
}

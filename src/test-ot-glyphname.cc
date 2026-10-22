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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot.h"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_OPEN
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_blob_create_from_file_or_fail(x)  hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ()
#ਜੇ_ਅੰਤ

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਜੇ (argc != 2) {
    fprintf (stderr, "usage: %s font-file\n", argv[0]);
    ਬਾਹਰ_ਨਿਕਲੋ (1);
  }

  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_always_assert (blob);
  hb_face_t *face = hb_face_create (blob, 0 /* first face */);
  hb_font_t *font = hb_font_create (face);
  hb_blob_destroy (blob);
  blob = ਖਾਲੀ_ਸੂਚਕ;
  

  ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs = hb_face_get_glyph_ਗਿਣਤੀ (face);
  ਪੂਰਨ	ਨਤੀਜਾ = 1;

  ਲਈ (hb_codepoint_t gid = 0; gid < ਅੰਕ_glyphs; gid++)
  {
    ਅੱਖਰ buf[64];
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ buf_ਮਾਪ = ਆਕਾਰ (buf);
    ਜੇ (hb_font_get_glyph_name (font, gid, buf, buf_ਮਾਪ))
    {
      hb_codepoint_t	gid_inv;
      ਜੇ (hb_font_get_glyph_from_name(font, buf, ਲੜੀ_ਲੰਬਾਈ (buf), &gid_inv))
      {
	ਜੇ (gid == gid_inv)
	{
	  ਲਿਖੋ ("%u <-> %s\n", gid, buf);
	}
	ਨਹੀਂਤਾਂ
	{
	  ਲਿਖੋ ("%u -> %s -> %u\n", gid, buf, gid_inv);
	  ਨਤੀਜਾ = 0;
	}
      }
      ਨਹੀਂਤਾਂ
      {
	ਲਿਖੋ ("%u -> %s -> ?\n", gid, buf);
	ਨਤੀਜਾ = 0;
      }
    }
    ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ ("%u -> ?\n", gid);
      ਨਤੀਜਾ = 0;
    }
  }

  hb_font_destroy (font);
  hb_face_destroy (face);

  ਵਾਪਸ !ਨਤੀਜਾ;
}

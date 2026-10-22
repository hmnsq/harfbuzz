/*
 * Copyright © 2010,2011,2013  Google, Inc.
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

#ضمن "hb.hh"

#ضمن "hb.h"
#ضمن "hb-ot.h"
#جيڪڏهن_وصف_ٿيل HAVE_FREETYPE
#ضمن "hb-ft.h"
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HB_NO_OPEN
#وصف_ڪريو hb_blob_create_from_file_or_fail(x)  hb_blob_get_خالي_جايون ()
#جيڪڏهن_پورو

صحيح
مکيه (صحيح argc, حرف **argv)
{
  منطقي ret = سچ;

#جيڪڏهن_وصف_نه_ٿيل HB_NO_بفر_SERIALIZE

  hb_بفر_serialize_format_t format = HB_بفر_SERIALIZE_FORMAT_متن;
  إذا (argc > 1)
  {
    إذا (!strcmp (argv[1], "--json"))
    {
      format = HB_بفر_SERIALIZE_FORMAT_JSON;
      argc--;
      argv++;
    }
    وإلا إذا (!strcmp (argv[1], "--text"))
    {
      format = HB_بفر_SERIALIZE_FORMAT_متن;
      argc--;
      argv++;
    }
  }

  إذا (argc < 2)
    argv[1] = (حرف *) "/dev/null";

  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_always_assert (blob);
  hb_face_t *face = hb_face_create (blob, 0 /* first face */);
  hb_blob_destroy (blob);
  blob = خالي_اشارو;

  بنانشان صحيح upem = hb_face_get_upem (face);
  hb_font_t *font = hb_font_create (face);
  hb_face_destroy (face);
  hb_font_سيٽ_scale (font, upem, upem);

  hb_بفر_t *buf;
  buf = hb_بفر_create ();

  حرف سٽ[BUFSIZ], ٻاهريون[BUFSIZ];
  طالما (fgets (سٽ, حجم(سٽ), stdin))
  {
    hb_بفر_clear_contents (buf);

    طالما (سچ)
    {
      ثابت حرف *p = سٽ;
      ثابت حرف *پڇاڙي = p + لڙي_ڊيگهه (p);
      إذا (p < پڇاڙي && *(پڇاڙي - 1) == '\n')
	  پڇاڙي--;
      إذا (!hb_بفر_deserialize_glyphs (buf,
					 p, پڇاڙي - p, &p,
					 font,
					 format))
      {
        ret = خطأ;
        اكسر;
      }

      إذا (*p == '\n')
        اكسر;
      إذا (p == سٽ)
      {
	ret = خطأ;
	اكسر;
      }

      بنانشان len = لڙي_ڊيگهه (p);
      memmove (سٽ, p, len);
      إذا (!fgets (سٽ + len, حجم(سٽ) - len, stdin))
        سٽ[len] = '\0';
    }

    بنانشان ڳڻپ = hb_بفر_get_ڊيگهائي (buf);
    لكل (بنانشان وٿي = 0; وٿي < ڳڻپ;)
    {
      بنانشان len;
      وٿي += hb_بفر_serialize_glyphs (buf, وٿي, ڳڻپ,
					    ٻاهريون, حجم (ٻاهريون), &len,
					    font,
					    format,
					    HB_بفر_SERIALIZE_FLAG_GLYPH_FLAGS);
      fwrite (ٻاهريون, 1, len, stdout);
    }
    fputs ("\n", stdout);
  }

  hb_بفر_destroy (buf);

  hb_font_destroy (font);

#جيڪڏهن_پورو

  أرجع !ret;
}

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
#جے_تعریف_شدہ HAVE_FREETYPE
#ضمن "hb-ft.h"
#جے_انت

#جے_تعریف_شدہ HB_NO_OPEN
#تعریف_کرو hb_blob_create_from_file_or_fail(x)  hb_blob_get_خالی_تھاواں ()
#جے_انت

صحيح
مکھ (صحيح argc, حرف **argv)
{
  منطقي ret = سچ;

#جے_بنا_تعریف HB_NO_بفر_SERIALIZE

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
  blob = خالی_سوچک;

  بنانشانی صحيح upem = hb_face_get_upem (face);
  hb_font_t *font = hb_font_create (face);
  hb_face_destroy (face);
  hb_font_سیٹ_scale (font, upem, upem);

  hb_بفر_t *buf;
  buf = hb_بفر_create ();

  حرف سطر[BUFSIZ], باہرلا[BUFSIZ];
  طالما (fgets (سطر, حجم(سطر), stdin))
  {
    hb_بفر_clear_contents (buf);

    طالما (سچ)
    {
      ثابت حرف *p = سطر;
      ثابت حرف *سرا = p + لڑی_لمبائی (p);
      إذا (p < سرا && *(سرا - 1) == '\n')
	  سرا--;
      إذا (!hb_بفر_deserialize_glyphs (buf,
					 p, سرا - p, &p,
					 font,
					 format))
      {
        ret = خطأ;
        اكسر;
      }

      إذا (*p == '\n')
        اكسر;
      إذا (p == سطر)
      {
	ret = خطأ;
	اكسر;
      }

      بنانشانی len = لڑی_لمبائی (p);
      memmove (سطر, p, len);
      إذا (!fgets (سطر + len, حجم(سطر) - len, stdin))
        سطر[len] = '\0';
    }

    بنانشانی گنتی = hb_بفر_get_لمائی (buf);
    لكل (بنانشانی وتھ = 0; وتھ < گنتی;)
    {
      بنانشانی len;
      وتھ += hb_بفر_serialize_glyphs (buf, وتھ, گنتی,
					    باہرلا, حجم (باہرلا), &len,
					    font,
					    format,
					    HB_بفر_SERIALIZE_FLAG_GLYPH_FLAGS);
      fwrite (باہرلا, 1, len, stdout);
    }
    fputs ("\n", stdout);
  }

  hb_بفر_destroy (buf);

  hb_font_destroy (font);

#جے_انت

  أرجع !ret;
}

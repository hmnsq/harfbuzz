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
#اگر_تعریف_شده HAVE_FREETYPE
#ضمن "hb-ft.h"
#پایان_اگر

#اگر_تعریف_شده HB_NO_OPEN
#تعریف_کن hb_blob_create_from_file_or_fail(x)  hb_blob_get_خالی ()
#پایان_اگر

صحيح
اصلی (صحيح argc, حرف **argv)
{
  منطقي ret = درست;

#اگر_تعریف_نشده HB_NO_میانگیر_SERIALIZE

  hb_میانگیر_serialize_format_t format = HB_میانگیر_SERIALIZE_FORMAT_متن;
  إذا (argc > 1)
  {
    إذا (!strcmp (argv[1], "--json"))
    {
      format = HB_میانگیر_SERIALIZE_FORMAT_JSON;
      argc--;
      argv++;
    }
    وإلا إذا (!strcmp (argv[1], "--text"))
    {
      format = HB_میانگیر_SERIALIZE_FORMAT_متن;
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
  blob = اشاره_تهی;

  بیعلامت صحيح upem = hb_face_get_upem (face);
  hb_font_t *font = hb_font_create (face);
  hb_face_destroy (face);
  hb_font_مجموعه_scale (font, upem, upem);

  hb_میانگیر_t *buf;
  buf = hb_میانگیر_create ();

  حرف سطر[BUFSIZ], بیرون[BUFSIZ];
  طالما (fgets (سطر, حجم(سطر), stdin))
  {
    hb_میانگیر_clear_contents (buf);

    طالما (درست)
    {
      ثابت حرف *p = سطر;
      ثابت حرف *انتها = p + طول_رشته (p);
      إذا (p < انتها && *(انتها - 1) == '\n')
	  انتها--;
      إذا (!hb_میانگیر_deserialize_glyphs (buf,
					 p, انتها - p, &p,
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

      بیعلامت len = طول_رشته (p);
      memmove (سطر, p, len);
      إذا (!fgets (سطر + len, حجم(سطر) - len, stdin))
        سطر[len] = '\0';
    }

    بیعلامت شمار = hb_میانگیر_get_درازی (buf);
    لكل (بیعلامت جابه_جایی = 0; جابه_جایی < شمار;)
    {
      بیعلامت len;
      جابه_جایی += hb_میانگیر_serialize_glyphs (buf, جابه_جایی, شمار,
					    بیرون, حجم (بیرون), &len,
					    font,
					    format,
					    HB_میانگیر_SERIALIZE_FLAG_GLYPH_FLAGS);
      fwrite (بیرون, 1, len, stdout);
    }
    fputs ("\n", stdout);
  }

  hb_میانگیر_destroy (buf);

  hb_font_destroy (font);

#پایان_اگر

  أرجع !ret;
}

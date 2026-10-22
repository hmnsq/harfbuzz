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
#که_تعریف_شوی HAVE_FREETYPE
#ضمن "hb-ft.h"
#د_که_پای

#که_تعریف_شوی HB_NO_OPEN
#تعریف_کړه hb_blob_create_from_file_or_fail(x)  hb_blob_get_تشه ()
#د_که_پای

صحيح
اصلي (صحيح argc, حرف **argv)
{
  منطقي ret = ریښتیا;

#که_نه_تعریف_شوی HB_NO_بفر_SERIALIZE

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
  blob = تش_اشاره;

  بینښه صحيح upem = hb_face_get_upem (face);
  hb_font_t *font = hb_font_create (face);
  hb_face_destroy (face);
  hb_font_سېټ_scale (font, upem, upem);

  hb_بفر_t *buf;
  buf = hb_بفر_create ();

  حرف کرښه[BUFSIZ], بهر[BUFSIZ];
  طالما (fgets (کرښه, حجم(کرښه), stdin))
  {
    hb_بفر_clear_contents (buf);

    طالما (ریښتیا)
    {
      ثابت حرف *p = کرښه;
      ثابت حرف *پای_ټکی = p + مزي_اوږدوالی (p);
      إذا (p < پای_ټکی && *(پای_ټکی - 1) == '\n')
	  پای_ټکی--;
      إذا (!hb_بفر_deserialize_glyphs (buf,
					 p, پای_ټکی - p, &p,
					 font,
					 format))
      {
        ret = خطأ;
        اكسر;
      }

      إذا (*p == '\n')
        اكسر;
      إذا (p == کرښه)
      {
	ret = خطأ;
	اكسر;
      }

      بینښه len = مزي_اوږدوالی (p);
      memmove (کرښه, p, len);
      إذا (!fgets (کرښه + len, حجم(کرښه) - len, stdin))
        کرښه[len] = '\0';
    }

    بینښه شمېر = hb_بفر_get_اوږدتیا (buf);
    لكل (بینښه دننه_واټن = 0; دننه_واټن < شمېر;)
    {
      بینښه len;
      دننه_واټن += hb_بفر_serialize_glyphs (buf, دننه_واټن, شمېر,
					    بهر, حجم (بهر), &len,
					    font,
					    format,
					    HB_بفر_SERIALIZE_FLAG_GLYPH_FLAGS);
      fwrite (بهر, 1, len, stdout);
    }
    fputs ("\n", stdout);
  }

  hb_بفر_destroy (buf);

  hb_font_destroy (font);

#د_که_پای

  أرجع !ret;
}

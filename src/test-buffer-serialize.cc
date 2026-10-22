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
#ifdef HAVE_FREETYPE
#ضمن "hb-ft.h"
#endif

#ifdef HB_NO_OPEN
#define hb_blob_create_from_file_or_fail(x)  hb_blob_get_empty ()
#endif

صحيح
main (صحيح argc, حرف **argv)
{
  منطقي ret = true;

#ifndef HB_NO_BUFFER_SERIALIZE

  hb_buffer_serialize_format_t format = HB_BUFFER_SERIALIZE_FORMAT_TEXT;
  إذا (argc > 1)
  {
    إذا (!strcmp (argv[1], "--json"))
    {
      format = HB_BUFFER_SERIALIZE_FORMAT_JSON;
      argc--;
      argv++;
    }
    وإلا إذا (!strcmp (argv[1], "--text"))
    {
      format = HB_BUFFER_SERIALIZE_FORMAT_TEXT;
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
  blob = nullptr;

  unsigned صحيح upem = hb_face_get_upem (face);
  hb_font_t *font = hb_font_create (face);
  hb_face_destroy (face);
  hb_font_set_scale (font, upem, upem);

  hb_buffer_t *buf;
  buf = hb_buffer_create ();

  حرف لي[BUFSIZ], out[BUFSIZ];
  طالما (fgets (لي, حجم(لي), stdin))
  {
    hb_buffer_clear_contents (buf);

    طالما (true)
    {
      ثابت حرف *p = لي;
      ثابت حرف *end = p + strlen (p);
      إذا (p < end && *(end - 1) == '\n')
	  end--;
      إذا (!hb_buffer_deserialize_glyphs (buf,
					 p, end - p, &p,
					 font,
					 format))
      {
        ret = خطأ;
        اكسر;
      }

      إذا (*p == '\n')
        اكسر;
      إذا (p == لي)
      {
	ret = خطأ;
	اكسر;
      }

      unsigned len = strlen (p);
      memmove (لي, p, len);
      إذا (!fgets (لي + len, حجم(لي) - len, stdin))
        لي[len] = '\0';
    }

    unsigned كدي = hb_buffer_get_length (buf);
    لكل (unsigned offset = 0; offset < كدي;)
    {
      unsigned len;
      offset += hb_buffer_serialize_glyphs (buf, offset, كدي,
					    out, حجم (out), &len,
					    font,
					    format,
					    HB_BUFFER_SERIALIZE_FLAG_GLYPH_FLAGS);
      fwrite (out, 1, len, stdout);
    }
    fputs ("\n", stdout);
  }

  hb_buffer_destroy (buf);

  hb_font_destroy (font);

#endif

  أرجع !ret;
}

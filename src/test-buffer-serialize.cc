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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot.h"
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_FREETYPE
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ft.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_OPEN
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_blob_create_from_file_or_fail(x)  hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ()
#ਜੇ_ਅੰਤ

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਤਾਰਕਿਕ ret = ਸੱਚ;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_ਬਫਰ_SERIALIZE

  hb_ਬਫਰ_serialize_format_t format = HB_ਬਫਰ_SERIALIZE_FORMAT_ਪਾਠ;
  ਜੇ (argc > 1)
  {
    ਜੇ (!strcmp (argv[1], "--json"))
    {
      format = HB_ਬਫਰ_SERIALIZE_FORMAT_JSON;
      argc--;
      argv++;
    }
    ਨਹੀਂਤਾਂ ਜੇ (!strcmp (argv[1], "--text"))
    {
      format = HB_ਬਫਰ_SERIALIZE_FORMAT_ਪਾਠ;
      argc--;
      argv++;
    }
  }

  ਜੇ (argc < 2)
    argv[1] = (ਅੱਖਰ *) "/dev/null";

  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_always_assert (blob);
  hb_face_t *face = hb_face_create (blob, 0 /* first face */);
  hb_blob_destroy (blob);
  blob = ਖਾਲੀ_ਸੂਚਕ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ upem = hb_face_get_upem (face);
  hb_font_t *font = hb_font_create (face);
  hb_face_destroy (face);
  hb_font_ਸੈੱਟ_scale (font, upem, upem);

  hb_ਬਫਰ_t *buf;
  buf = hb_ਬਫਰ_create ();

  ਅੱਖਰ ਸਤਰ[BUFSIZ], ਬਾਹਰਲਾ[BUFSIZ];
  ਜਦੋਂ (fgets (ਸਤਰ, ਆਕਾਰ(ਸਤਰ), stdin))
  {
    hb_ਬਫਰ_clear_contents (buf);

    ਜਦੋਂ (ਸੱਚ)
    {
      ਅਚਲ ਅੱਖਰ *p = ਸਤਰ;
      ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = p + ਲੜੀ_ਲੰਬਾਈ (p);
      ਜੇ (p < ਸਿਰਾ && *(ਸਿਰਾ - 1) == '\n')
	  ਸਿਰਾ--;
      ਜੇ (!hb_ਬਫਰ_deserialize_glyphs (buf,
					 p, ਸਿਰਾ - p, &p,
					 font,
					 format))
      {
        ret = ਝੂਠ;
        ਰੋਕੋ;
      }

      ਜੇ (*p == '\n')
        ਰੋਕੋ;
      ਜੇ (p == ਸਤਰ)
      {
	ret = ਝੂਠ;
	ਰੋਕੋ;
      }

      ਬਿਨਾਨਿਸ਼ਾਨੀ len = ਲੜੀ_ਲੰਬਾਈ (p);
      memmove (ਸਤਰ, p, len);
      ਜੇ (!fgets (ਸਤਰ + len, ਆਕਾਰ(ਸਤਰ) - len, stdin))
        ਸਤਰ[len] = '\0';
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = hb_ਬਫਰ_get_ਲੰਮਾਈ (buf);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਿੱਥ = 0; ਵਿੱਥ < ਗਿਣਤੀ_2;)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ len;
      ਵਿੱਥ += hb_ਬਫਰ_serialize_glyphs (buf, ਵਿੱਥ, ਗਿਣਤੀ_2,
					    ਬਾਹਰਲਾ, ਆਕਾਰ (ਬਾਹਰਲਾ), &len,
					    font,
					    format,
					    HB_ਬਫਰ_SERIALIZE_FLAG_GLYPH_FLAGS);
      fwrite (ਬਾਹਰਲਾ, 1, len, stdout);
    }
    fputs ("\n", stdout);
  }

  hb_ਬਫਰ_destroy (buf);

  hb_font_destroy (font);

#ਜੇ_ਅੰਤ

  ਵਾਪਸ !ret;
}

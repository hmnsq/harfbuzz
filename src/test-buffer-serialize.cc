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

#અન્તર્ભાવ "hb.hh"

#અન્તર્ભાવ "hb.h"
#અન્તર્ભાવ "hb-ot.h"
#ifdef HAVE_FREETYPE
#અન્તર્ભાવ "hb-ft.h"
#endif

#ifdef HB_NO_OPEN
#define hb_blob_create_from_file_or_fail(x)  hb_blob_get_empty ()
#endif

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  તાર્કિક ret = સાચું;

#ifndef HB_NO_BUFFER_SERIALIZE

  hb_buffer_serialize_format_t format = HB_BUFFER_SERIALIZE_FORMAT_TEXT;
  જો (argc > 1)
  {
    જો (!strcmp (argv[1], "--json"))
    {
      format = HB_BUFFER_SERIALIZE_FORMAT_JSON;
      argc--;
      argv++;
    }
    નહીંતર જો (!strcmp (argv[1], "--text"))
    {
      format = HB_BUFFER_SERIALIZE_FORMAT_TEXT;
      argc--;
      argv++;
    }
  }

  જો (argc < 2)
    argv[1] = (અક્ષર *) "/dev/null";

  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_always_assert (blob);
  hb_face_t *face = hb_face_create (blob, 0 /* first face */);
  hb_blob_destroy (blob);
  blob = nullptr;

  unsigned પૂર્ણાંક upem = hb_face_get_upem (face);
  hb_font_t *font = hb_font_create (face);
  hb_face_destroy (face);
  hb_font_set_scale (font, upem, upem);

  hb_buffer_t *buf;
  buf = hb_buffer_create ();

  અક્ષર લીટી[BUFSIZ], out[BUFSIZ];
  જ્યારે (fgets (લીટી, કદ(લીટી), stdin))
  {
    hb_buffer_clear_contents (buf);

    જ્યારે (સાચું)
    {
      અચલ અક્ષર *p = લીટી;
      અચલ અક્ષર *end = p + strlen (p);
      જો (p < end && *(end - 1) == '\n')
	  end--;
      જો (!hb_buffer_deserialize_glyphs (buf,
					 p, end - p, &p,
					 font,
					 format))
      {
        ret = ખોટું;
        તોડો;
      }

      જો (*p == '\n')
        તોડો;
      જો (p == લીટી)
      {
	ret = ખોટું;
	તોડો;
      }

      unsigned len = strlen (p);
      memmove (લીટી, p, len);
      જો (!fgets (લીટી + len, કદ(લીટી) - len, stdin))
        લીટી[len] = '\0';
    }

    unsigned ગણતરી = hb_buffer_get_length (buf);
    માટે (unsigned offset = 0; offset < ગણતરી;)
    {
      unsigned len;
      offset += hb_buffer_serialize_glyphs (buf, offset, ગણતરી,
					    out, કદ (out), &len,
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

  પાછા !ret;
}

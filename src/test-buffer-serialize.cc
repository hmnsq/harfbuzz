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

#অন্তর্ভাব "hb.hh"

#অন্তর্ভাব "hb.h"
#অন্তর্ভাব "hb-ot.h"
#ifdef HAVE_FREETYPE
#অন্তর্ভাব "hb-ft.h"
#endif

#ifdef HB_NO_OPEN
#define hb_blob_create_from_file_or_fail(x)  hb_blob_get_empty ()
#endif

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  যৌক্তিক ret = সত্য;

#ifndef HB_NO_বাফার_SERIALIZE

  hb_বাফার_serialize_format_t format = HB_বাফার_SERIALIZE_FORMAT_TEXT;
  যদি (argc > 1)
  {
    যদি (!strcmp (argv[1], "--json"))
    {
      format = HB_বাফার_SERIALIZE_FORMAT_JSON;
      argc--;
      argv++;
    }
    অন্যথা যদি (!strcmp (argv[1], "--text"))
    {
      format = HB_বাফার_SERIALIZE_FORMAT_TEXT;
      argc--;
      argv++;
    }
  }

  যদি (argc < 2)
    argv[1] = (অক্ষর *) "/dev/null";

  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_always_assert (blob);
  hb_face_t *face = hb_face_create (blob, 0 /* first face */);
  hb_blob_destroy (blob);
  blob = nullptr;

  unsigned পূর্ণসংখ্যা upem = hb_face_get_upem (face);
  hb_font_t *font = hb_font_create (face);
  hb_face_destroy (face);
  hb_font_set_scale (font, upem, upem);

  hb_বাফার_t *buf;
  buf = hb_বাফার_create ();

  অক্ষর সারি[BUFSIZ], out[BUFSIZ];
  যখন (fgets (সারি, আকার(সারি), stdin))
  {
    hb_বাফার_clear_contents (buf);

    যখন (সত্য)
    {
      ধ্রুবক অক্ষর *p = সারি;
      ধ্রুবক অক্ষর *end = p + strlen (p);
      যদি (p < end && *(end - 1) == '\n')
	  end--;
      যদি (!hb_বাফার_deserialize_glyphs (buf,
					 p, end - p, &p,
					 font,
					 format))
      {
        ret = মিথ্যা;
        বিরতি;
      }

      যদি (*p == '\n')
        বিরতি;
      যদি (p == সারি)
      {
	ret = মিথ্যা;
	বিরতি;
      }

      unsigned len = strlen (p);
      memmove (সারি, p, len);
      যদি (!fgets (সারি + len, আকার(সারি) - len, stdin))
        সারি[len] = '\0';
    }

    unsigned গুনতি = hb_বাফার_get_দৈর্ঘ্য (buf);
    জন্য (unsigned offset = 0; offset < গুনতি;)
    {
      unsigned len;
      offset += hb_বাফার_serialize_glyphs (buf, offset, গুনতি,
					    out, আকার (out), &len,
					    font,
					    format,
					    HB_বাফার_SERIALIZE_FLAG_GLYPH_FLAGS);
      fwrite (out, 1, len, stdout);
    }
    fputs ("\n", stdout);
  }

  hb_বাফার_destroy (buf);

  hb_font_destroy (font);

#endif

  ফেরত !ret;
}

/*
 * Copyright © 2010,2011  Google, Inc.
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

#ifdef HAVE_FREETYPE
#অন্তর্ভাব "hb-ft.h"
#endif

#ifdef HB_NO_OPEN
#define hb_blob_create_from_file_or_fail(x)  hb_blob_get_empty ()
#endif

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  যদি (argc != 2) {
    fprintf (stderr, "usage: %s font-file.ttf\n", argv[0]);
    exit (1);
  }

  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  প্রতিজ্ঞা (blob);
  printf ("Opened font file %s: %u bytes long\n", argv[1], hb_blob_get_দৈর্ঘ্য (blob));

  /* Create the face */
  hb_face_t *face = hb_face_create (blob, 0 /* first face */);
  hb_blob_destroy (blob);
  blob = nullptr;
  unsigned পূর্ণসংখ্যা upem = hb_face_get_upem (face);

  hb_font_t *font = hb_font_create (face);
  hb_font_set_scale (font, upem, upem);

#ifdef HAVE_FREETYPE
  hb_ft_font_set_funcs (font);
#endif

  hb_বাফার_t *বাফার = hb_বাফার_create ();

  hb_বাফার_add_utf8 (বাফার, "\xe0\xa4\x95\xe0\xa5\x8d\xe0\xa4\xb0\xe0\xa5\x8d\xe0\xa4\x95", -1, 0, -1);
  hb_বাফার_guess_segment_properties (বাফার);

  hb_shape (font, বাফার, nullptr, 0);

  unsigned পূর্ণসংখ্যা গুনতি = hb_বাফার_get_দৈর্ঘ্য (বাফার);
  hb_glyph_info_t *infos = hb_বাফার_get_glyph_infos (বাফার, nullptr);
  hb_glyph_position_t *positions = hb_বাফার_get_glyph_positions (বাফার, nullptr);

  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
  {
    hb_glyph_info_t *info = &infos[i];
    hb_glyph_position_t *pos = &positions[i];

    printf ("cluster %u	glyph 0x%x at	(%d,%d)+(%d,%d)\n",
	    info->cluster,
	    info->codepoint,
	    pos->x_offset,
	    pos->y_offset,
	    pos->x_advance,
	    pos->y_advance);

  }

  hb_বাফার_destroy (বাফার);
  hb_font_destroy (font);
  hb_face_destroy (face);

  ফেরত 0;
}



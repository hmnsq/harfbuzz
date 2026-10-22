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
  ਜੇ (argc != 4 && argc != 5) {
    fprintf (stderr, "usage: %s font-file lookup-index first-glyph [second-glyph]\n", argv[0]);
    ਬਾਹਰ_ਨਿਕਲੋ (1);
  }

  /* Create the face */
  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_always_assert (blob);
  hb_face_t *face = hb_face_create (blob, 0 /* first face */);
  hb_blob_destroy (blob);
  blob = ਖਾਲੀ_ਸੂਚਕ;

  hb_font_t *font = hb_font_create (face);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_FREETYPE
  hb_ft_font_ਸੈੱਟ_funcs (font);
#ਜੇ_ਅੰਤ

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len = argc - 3;
  hb_codepoint_t glyphs[2];
  ਜੇ (!hb_font_glyph_from_string (font, argv[3], -1, &glyphs[0]) ||
      (argc > 4 &&
       !hb_font_glyph_from_string (font, argv[4], -1, &glyphs[1])))
    ਵਾਪਸ 2;
  ਵਾਪਸ !hb_ot_layout_lookup_would_substitute (face, strtol (argv[2], ਖਾਲੀ_ਸੂਚਕ, 0), glyphs, len, ਝੂਠ);
}

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

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_FREETYPE
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ft.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_OPEN
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_blob_create_from_file_or_fail(x)  hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ()
#ਜੇ_ਅੰਤ

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਜੇ (argc != 2) {
    fprintf (stderr, "usage: %s font-file.ttf\n", argv[0]);
    ਬਾਹਰ_ਨਿਕਲੋ (1);
  }

  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  ਪ੍ਰਤਿਜ੍ਞਾ (blob);
  ਲਿਖੋ ("Opened font file %s: %u bytes long\n", argv[1], hb_blob_get_ਲੰਮਾਈ (blob));

  /* Create the face */
  hb_face_t *face = hb_face_create (blob, 0 /* first face */);
  hb_blob_destroy (blob);
  blob = ਖਾਲੀ_ਸੂਚਕ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ upem = hb_face_get_upem (face);

  hb_font_t *font = hb_font_create (face);
  hb_font_ਸੈੱਟ_scale (font, upem, upem);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_FREETYPE
  hb_ft_font_ਸੈੱਟ_funcs (font);
#ਜੇ_ਅੰਤ

  hb_ਬਫਰ_t *ਬਫਰ = hb_ਬਫਰ_create ();

  hb_ਬਫਰ_ਜੋੜੋ_utf8 (ਬਫਰ, "\xe0\xa4\x95\xe0\xa5\x8d\xe0\xa4\xb0\xe0\xa5\x8d\xe0\xa4\x95", -1, 0, -1);
  hb_ਬਫਰ_guess_segment_properties (ਬਫਰ);

  hb_shape (font, ਬਫਰ, ਖਾਲੀ_ਸੂਚਕ, 0);

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = hb_ਬਫਰ_get_ਲੰਮਾਈ (ਬਫਰ);
  hb_glyph_info_t *infos = hb_ਬਫਰ_get_glyph_infos (ਬਫਰ, ਖਾਲੀ_ਸੂਚਕ);
  hb_glyph_position_t *positions = hb_ਬਫਰ_get_glyph_positions (ਬਫਰ, ਖਾਲੀ_ਸੂਚਕ);

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
  {
    hb_glyph_info_t *info = &infos[i];
    hb_glyph_position_t *pos = &positions[i];

    ਲਿਖੋ ("cluster %u	glyph 0x%x at	(%d,%d)+(%d,%d)\n",
	    info->cluster,
	    info->codepoint,
	    pos->x_ਵਿੱਥ_2,
	    pos->y_ਵਿੱਥ_2,
	    pos->x_advance,
	    pos->y_advance);

  }

  hb_ਬਫਰ_destroy (ਬਫਰ);
  hb_font_destroy (font);
  hb_face_destroy (face);

  ਵਾਪਸ 0;
}



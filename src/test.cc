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

#ضمن "hb.hh"

#اگر_تعریف_شده HAVE_FREETYPE
#ضمن "hb-ft.h"
#پایان_اگر

#اگر_تعریف_شده HB_NO_OPEN
#تعریف_کن hb_blob_create_from_file_or_fail(x)  hb_blob_get_خالی ()
#پایان_اگر

صحيح
اصلی (صحيح argc, حرف **argv)
{
  إذا (argc != 2) {
    fprintf (stderr, "usage: %s font-file.ttf\n", argv[0]);
    خروج (1);
  }

  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  أكد (blob);
  چاپ_کن ("Opened font file %s: %u bytes long\n", argv[1], hb_blob_get_درازی (blob));

  /* Create the face */
  hb_face_t *face = hb_face_create (blob, 0 /* first face */);
  hb_blob_destroy (blob);
  blob = اشاره_تهی;
  بیعلامت صحيح upem = hb_face_get_upem (face);

  hb_font_t *font = hb_font_create (face);
  hb_font_مجموعه_scale (font, upem, upem);

#اگر_تعریف_شده HAVE_FREETYPE
  hb_ft_font_مجموعه_funcs (font);
#پایان_اگر

  hb_میانگیر_t *میانگیر = hb_میانگیر_create ();

  hb_میانگیر_جمع_کن_utf8 (میانگیر, "\xe0\xa4\x95\xe0\xa5\x8d\xe0\xa4\xb0\xe0\xa5\x8d\xe0\xa4\x95", -1, 0, -1);
  hb_میانگیر_guess_segment_properties (میانگیر);

  hb_shape (font, میانگیر, اشاره_تهی, 0);

  بیعلامت صحيح شمار = hb_میانگیر_get_درازی (میانگیر);
  hb_glyph_info_t *infos = hb_میانگیر_get_glyph_infos (میانگیر, اشاره_تهی);
  hb_glyph_position_t *positions = hb_میانگیر_get_glyph_positions (میانگیر, اشاره_تهی);

  لكل (بیعلامت صحيح i = 0; i < شمار; i++)
  {
    hb_glyph_info_t *info = &infos[i];
    hb_glyph_position_t *pos = &positions[i];

    چاپ_کن ("cluster %u	glyph 0x%x at	(%d,%d)+(%d,%d)\n",
	    info->cluster,
	    info->codepoint,
	    pos->x_جابه_جایی_2,
	    pos->y_جابه_جایی_2,
	    pos->x_advance,
	    pos->y_advance);

  }

  hb_میانگیر_destroy (میانگیر);
  hb_font_destroy (font);
  hb_face_destroy (face);

  أرجع 0;
}



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

#جے_تعریف_شدہ HAVE_FREETYPE
#ضمن "hb-ft.h"
#جے_انت

#جے_تعریف_شدہ HB_NO_OPEN
#تعریف_کرو hb_blob_create_from_file_or_fail(x)  hb_blob_get_خالی_تھاواں ()
#جے_انت

صحيح
مکھ (صحيح argc, حرف **argv)
{
  إذا (argc != 2) {
    fprintf (stderr, "usage: %s font-file.ttf\n", argv[0]);
    باہر_نکلو (1);
  }

  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  أكد (blob);
  لکھو ("Opened font file %s: %u bytes long\n", argv[1], hb_blob_get_لمائی (blob));

  /* Create the face */
  hb_face_t *face = hb_face_create (blob, 0 /* first face */);
  hb_blob_destroy (blob);
  blob = خالی_سوچک;
  بنانشانی صحيح upem = hb_face_get_upem (face);

  hb_font_t *font = hb_font_create (face);
  hb_font_سیٹ_scale (font, upem, upem);

#جے_تعریف_شدہ HAVE_FREETYPE
  hb_ft_font_سیٹ_funcs (font);
#جے_انت

  hb_بفر_t *بفر = hb_بفر_create ();

  hb_بفر_جوڑو_utf8 (بفر, "\xe0\xa4\x95\xe0\xa5\x8d\xe0\xa4\xb0\xe0\xa5\x8d\xe0\xa4\x95", -1, 0, -1);
  hb_بفر_guess_segment_properties (بفر);

  hb_shape (font, بفر, خالی_سوچک, 0);

  بنانشانی صحيح گنتی = hb_بفر_get_لمائی (بفر);
  hb_glyph_info_t *infos = hb_بفر_get_glyph_infos (بفر, خالی_سوچک);
  hb_glyph_position_t *positions = hb_بفر_get_glyph_positions (بفر, خالی_سوچک);

  لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
  {
    hb_glyph_info_t *info = &infos[i];
    hb_glyph_position_t *pos = &positions[i];

    لکھو ("cluster %u	glyph 0x%x at	(%d,%d)+(%d,%d)\n",
	    info->cluster,
	    info->codepoint,
	    pos->x_وتھ_2,
	    pos->y_وتھ_2,
	    pos->x_advance,
	    pos->y_advance);

  }

  hb_بفر_destroy (بفر);
  hb_font_destroy (font);
  hb_face_destroy (face);

  أرجع 0;
}



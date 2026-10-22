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

#అన్తర్భావ "hb.hh"

#నిర్వచితమైతే HAVE_FREETYPE
#అన్తర్భావ "hb-ft.h"
#అయితే_ముగింపు

#నిర్వచితమైతే HB_NO_OPEN
#నిర్వచించు hb_blob_create_from_file_or_fail(x)  hb_blob_get_ఖాళీలు ()
#అయితే_ముగింపు

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  ఒకవేళ (argc != 2) {
    fprintf (stderr, "usage: %s font-file.ttf\n", argv[0]);
    నిష్క్రమించు (1);
  }

  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  ప్రతిజ్ఞా (blob);
  రాయి ("Opened font file %s: %u bytes long\n", argv[1], hb_blob_get_నిడివి (blob));

  /* Create the face */
  hb_face_t *face = hb_face_create (blob, 0 /* first face */);
  hb_blob_destroy (blob);
  blob = శూన్యసూచిక;
  గుర్తులేని పూర్ణాంకం upem = hb_face_get_upem (face);

  hb_font_t *font = hb_font_create (face);
  hb_font_సమితి_scale (font, upem, upem);

#నిర్వచితమైతే HAVE_FREETYPE
  hb_ft_font_సమితి_funcs (font);
#అయితే_ముగింపు

  hb_బఫరు_t *బఫరు = hb_బఫరు_create ();

  hb_బఫరు_కూడు_utf8 (బఫరు, "\xe0\xa4\x95\xe0\xa5\x8d\xe0\xa4\xb0\xe0\xa5\x8d\xe0\xa4\x95", -1, 0, -1);
  hb_బఫరు_guess_segment_properties (బఫరు);

  hb_shape (font, బఫరు, శూన్యసూచిక, 0);

  గుర్తులేని పూర్ణాంకం లెక్క = hb_బఫరు_get_నిడివి (బఫరు);
  hb_glyph_info_t *infos = hb_బఫరు_get_glyph_infos (బఫరు, శూన్యసూచిక);
  hb_glyph_position_t *positions = hb_బఫరు_get_glyph_positions (బఫరు, శూన్యసూచిక);

  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
  {
    hb_glyph_info_t *info = &infos[i];
    hb_glyph_position_t *pos = &positions[i];

    రాయి ("cluster %u	glyph 0x%x at	(%d,%d)+(%d,%d)\n",
	    info->cluster,
	    info->codepoint,
	    pos->x_స్థానభ్రంశం_2,
	    pos->y_స్థానభ్రంశం_2,
	    pos->x_advance,
	    pos->y_advance);

  }

  hb_బఫరు_destroy (బఫరు);
  hb_font_destroy (font);
  hb_face_destroy (face);

  తిరిగి 0;
}



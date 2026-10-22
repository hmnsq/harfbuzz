/*
 * Copyright © 2018  Google, Inc.
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
#అన్తర్భావ "hb-ot.h"

#నిర్వచితమైతే HB_NO_OPEN
#నిర్వచించు hb_blob_create_from_file_or_fail(x)  hb_blob_get_ఖాళీలు ()
#అయితే_ముగింపు

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  ఒకవేళ (argc != 2) {
    fprintf (stderr, "usage: %s font-file\n", argv[0]);
    నిష్క్రమించు (1);
  }

  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_always_assert (blob);
  hb_face_t *face = hb_face_create (blob, 0 /* first face */);
  hb_blob_destroy (blob);
  blob = శూన్యసూచిక;

  గుర్తులేని పూర్ణాంకం లెక్క = 0;

#నిర్వచితం_కాకపోతే HB_NO_NAME
  స్థిరరాశి hb_ot_name_నమోదు_t *entries = hb_ot_name_list_పేర్లు (face, &లెక్క);

  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
  {
    రాయి ("%u	%s	",
	    entries[i].name_id,
	    hb_language_గమ్యం_string (entries[i].language));

    అక్షరం buf[64];
    గుర్తులేని పూర్ణాంకం buf_కొలత = పరిమాణం (buf);
    hb_ot_name_get_utf8 (face,
			 entries[i].name_id,
			 entries[i].language,
			 &buf_కొలత,
			 buf);

    రాయి ("%s\n", buf);
  }
#అయితే_ముగింపు

  hb_face_destroy (face);

  తిరిగి లెక్క ? 0 : 1;
}

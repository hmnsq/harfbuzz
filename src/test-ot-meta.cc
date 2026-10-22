/*
 * Copyright © 2019  Ebrahim Byagowi
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

#నిర్వచితం_కాకపోతే HB_NO_META
  లెక్క = hb_ot_meta_get_నమోదు_ట్యాగులు (face, 0, శూన్యసూచిక, శూన్యసూచిక);

  hb_ot_meta_ట్యాగ్_t *ట్యాగులు = (hb_ot_meta_ట్యాగ్_t *)
			   జ్ఞాపకం_ఇవ్వు (పరిమాణం (hb_ot_meta_ట్యాగ్_t) * లెక్క);
  hb_ot_meta_get_నమోదు_ట్యాగులు (face, 0, &లెక్క, ట్యాగులు);
  కొరకు (గుర్తులేని i = 0; i < లెక్క; ++i)
  {
    hb_blob_t *నమోదు_2 = hb_ot_meta_reference_నమోదు (face, ట్యాగులు[i]);
    రాయి ("%c%c%c%c, size: %u: %.*s\n",
	    HB_UNTAG (ట్యాగులు[i]), hb_blob_get_నిడివి (నమోదు_2),
	    (పూర్ణాంకం) hb_blob_get_నిడివి (నమోదు_2), hb_blob_get_దత్తాంశం (నమోదు_2, శూన్యసూచిక));
    hb_blob_destroy (నమోదు_2);
  }
  విడుదల (ట్యాగులు);
#అయితే_ముగింపు

  hb_face_destroy (face);

  తిరిగి !లెక్క;
}

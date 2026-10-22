/*
 * Copyright © 2019  Adobe, Inc.
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
 * Adobe Author(s): Michiharu Ariza
 */

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-bimap.hh"

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  hb_bimap_t	bm;

  hb_always_assert (bm.is_empty () == నిజం);
  bm.సమితి (1, 4);
  bm.సమితి (2, 5);
  bm.సమితి (3, 6);
  hb_always_assert (bm.get_population () == 3);
  hb_always_assert (bm.has (1) == నిజం);
  hb_always_assert (bm.has (4) == అబద్ధం);
  hb_always_assert (bm[2] == 5);
  hb_always_assert (bm.backward (6) == 3);
  bm.del (1);
  hb_always_assert (bm.has (1) == అబద్ధం);
  hb_always_assert (bm.has (3) == నిజం);
  bm.clear ();
  hb_always_assert (bm.get_population () == 0);

  hb_inc_bimap_t  ibm;

  hb_always_assert (ibm.కూడు (13) == 0);
  hb_always_assert (ibm.కూడు (8) == 1);
  hb_always_assert (ibm.కూడు (10) == 2);
  hb_always_assert (ibm.కూడు (8) == 1);
  hb_always_assert (ibm.కూడు (7) == 3);
  hb_always_assert (ibm.get_population () == 4);
  hb_always_assert (ibm[7] == 3);

  ibm.sort ();
  hb_always_assert (ibm.get_population () == 4);
  hb_always_assert (ibm[7] == 0);
  hb_always_assert (ibm[13] == 3);

  ibm.తటస్థం (3);
  hb_always_assert (ibm.get_population () == 3);
  hb_always_assert (ibm[0] == 0);
  hb_always_assert (ibm[1] == 1);
  hb_always_assert (ibm[2] == 2);
  hb_always_assert (ibm.backward (0) == 0);
  hb_always_assert (ibm.backward (1) == 1);
  hb_always_assert (ibm.backward (2) == 2);
  hb_always_assert (ibm.has (4) == అబద్ధం);

  తిరిగి 0;
}

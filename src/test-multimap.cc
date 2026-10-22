/*
 * Copyright © 2022  Behdad Esfahbod
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
#అన్తర్భావ "hb-multimap.hh"

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  hb_multimap_t m;

  hb_always_assert (m.get (10).నిడివి == 0);

  m.కూడు (10, 11);
  hb_always_assert (m.get (10).నిడివి == 1);

  m.కూడు (10, 12);
  hb_always_assert (m.get (10).నిడివి == 2);

  m.కూడు (10, 13);
  hb_always_assert (m.get (10).నిడివి == 3);
  hb_always_assert (m.get (10)[0] == 11);
  hb_always_assert (m.get (10)[1] == 12);
  hb_always_assert (m.get (10)[2] == 13);

  hb_always_assert (m.get (11).నిడివి == 0);
  m.కూడు (11, 14);
  hb_always_assert (m.get (10).నిడివి == 3);
  hb_always_assert (m.get (11).నిడివి == 1);
  hb_always_assert (m.get (12).నిడివి == 0);
  hb_always_assert (m.get (10)[0] == 11);
  hb_always_assert (m.get (10)[1] == 12);
  hb_always_assert (m.get (10)[2] == 13);
  hb_always_assert (m.get (11)[0] == 14);
  hb_always_assert (m.get (12)[0] == 0); // Array fallback value

  తిరిగి 0;
}

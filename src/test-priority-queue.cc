/*
 * Copyright © 2020  Google, Inc.
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
 * Google Author(s): Garret Rieger
 */

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-priority-queue.hh"

స్థిర శూన్య
test_చొప్పించు ()
{
  hb_ప్రాధాన్యత_వరుస_t<int64_t> వరుస;
  hb_always_assert (వరుస.is_empty ());

  వరుస.insert (10, 0);
  hb_always_assert (!వరుస.is_empty ());
  hb_always_assert (వరుస.minimum () == hb_pair (10, 0));

  వరుస.insert (20, 1);
  hb_always_assert (వరుస.minimum () == hb_pair (10, 0));

  వరుస.insert (5, 2);
  hb_always_assert (వరుస.minimum () == hb_pair (5, 2));

  వరుస.insert (15, 3);
  hb_always_assert (వరుస.minimum () == hb_pair (5, 2));

  వరుస.insert (1, 4);
  hb_always_assert (వరుస.minimum () == hb_pair (1, 4));
}

స్థిర శూన్య
test_వెలికితీయి ()
{
  hb_ప్రాధాన్యత_వరుస_t<int32_t> వరుస;
  వరుస.insert (0, 0);
  వరుస.insert (60, 6);
  వరుస.insert (30, 3);
  వరుస.insert (40 ,4);
  వరుస.insert (20, 2);
  వరుస.insert (50, 5);
  వరుస.insert (70, 7);
  వరుస.insert (10, 1);

  కొరకు (పూర్ణాంకం i = 0; i < 8; i++)
  {
    hb_always_assert (!వరుస.is_empty ());
    hb_always_assert (వరుస.minimum () == hb_pair (i * 10, i));
    hb_always_assert (వరుస.తీయి_minimum () == hb_pair (i * 10, i));
  }

  hb_always_assert (వరుస.is_empty ());
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  test_చొప్పించు ();
  test_వెలికితీయి ();
}

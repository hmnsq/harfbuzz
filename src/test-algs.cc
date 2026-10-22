/*
 * Copyright © 2019  Facebook, Inc.
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
 * Facebook Author(s): Behdad Esfahbod
 */

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-algs.hh"
#అన్తర్భావ "hb-set.hh"


స్థిర అక్షరం *
test_func (పూర్ణాంకం a, అక్షరం **b)
{
  తిరిగి b ? b[a] : శూన్యసూచిక;
}

నిర్మాణం A
{
  శూన్య a () {}
};

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  hb_always_assert (roundf (0.5f) == 1.f);
  hb_always_assert (roundf (-0.5f) == 0.f);
  hb_always_assert (roundf (1.5f) == 2.f);
  hb_always_assert (roundf (-1.5f) == -1.f);
  hb_always_assert (roundf (nextafterf (0.5f, 0.f)) == 0.f);
  hb_always_assert (roundf (nextafterf (-0.5f, -1.f)) == -1.f);
  hb_always_assert (roundf (nextafterf (1.5f, 1.f)) == 1.f);
  hb_always_assert (roundf (nextafterf (-1.5f, -2.f)) == -2.f);
  hb_always_assert (roundf (15383929.f) == 15383929.f);
  hb_always_assert (roundf (-15383929.f) == -15383929.f);
  hb_always_assert (roundf (8388609.f) == 8388609.f);
  hb_always_assert (roundf (-8388609.f) == -8388609.f);

  పూర్ణాంకం i = 1;
  స్వయంచాలక p = hb_pair (1, i);

  p.second = 2;
  hb_always_assert (i == 2);

  స్థిరరాశి పూర్ణాంకం c = 3;
  స్వయంచాలక pc = hb_pair (1, c);
  hb_always_assert (pc.second == 3);

  స్వయంచాలక q = p;
  hb_always_assert (&q != &p);
  q.second = 4;
  hb_always_assert (i == 4);

  hb_invoke (test_func, 0, శూన్యసూచిక);

  A a;
  hb_invoke (&A::a, a);

  hb_always_assert (1 == hb_min (8, 1));
  hb_always_assert (8 == hb_max (8, 1));

  పూర్ణాంకం x = 1, y = 2;
  hb_min (x, 3);
  hb_min (3, x);
  hb_min (x, 4 + 3);
  పూర్ణాంకం &z = hb_min (x, y);
  z = 3;
  hb_always_assert (x == 3);

  hb_pair_t<స్థిరరాశి పూర్ణాంకం*, పూర్ణాంకం> xp = hb_pair_t<పూర్ణాంకం *, దీర్ఘ> (శూన్యసూచిక, 0);
  xp = hb_pair_t<పూర్ణాంకం *, ద్విగుణ> (శూన్యసూచిక, 1);
  xp = hb_pair_t<స్థిరరాశి పూర్ణాంకం*, పూర్ణాంకం> (శూన్యసూచిక, 1);

  hb_always_assert (3 == hb_partial (hb_min, 3) (4));
  hb_always_assert (3 == hb_partial<1> (hb_min, 4) (3));

  స్వయంచాలక M0 = hb_partial<2> (hb_max, 0);
  hb_always_assert (M0 (-2) == 0);
  hb_always_assert (M0 (+2) == 2);

  hb_always_assert (hb_కూడు (2) (5) == 7);
  hb_always_assert (hb_కూడు (5) (2) == 7);

  hb_always_assert (hb_saturate_కూడు (1, 2) == 3);
  hb_always_assert (hb_saturate_కూడు (గరిష్ఠ_పూర్ణాంకం, 1) == గరిష్ఠ_పూర్ణాంకం);
  hb_always_assert (hb_saturate_కూడు (INT_MIN, -1) == INT_MIN);
  hb_always_assert (hb_saturate_కూడు (గరిష్ఠ_పూర్ణాంకం, INT_MIN) == -1);
  hb_always_assert (hb_saturate_sub (3, 2) == 1);
  hb_always_assert (hb_saturate_sub (గరిష్ఠ_పూర్ణాంకం, -1) == గరిష్ఠ_పూర్ణాంకం);
  hb_always_assert (hb_saturate_sub (INT_MIN, 1) == INT_MIN);
  hb_always_assert (hb_saturate_sub (INT_MIN, INT_MIN) == 0);
  hb_always_assert (hb_saturate_neg (2) == -2);
  hb_always_assert (hb_saturate_neg (-2) == 2);
  hb_always_assert (hb_saturate_neg (INT_MIN) == గరిష్ఠ_పూర్ణాంకం);
  hb_always_assert (hb_clamp_గమ్యం<పూర్ణాంకం> (1.) == 1);
  hb_always_assert (hb_clamp_గమ్యం<పూర్ణాంకం> ((ద్విగుణ) గరిష్ఠ_పూర్ణాంకం + 1.) == గరిష్ఠ_పూర్ణాంకం);
  hb_always_assert (hb_clamp_గమ్యం<పూర్ణాంకం> ((ద్విగుణ) INT_MIN - 1.) == INT_MIN);
  hb_always_assert (hb_clamp_గమ్యం<పూర్ణాంకం> ((int64_t) 1) == 1);
  hb_always_assert (hb_clamp_గమ్యం<పూర్ణాంకం> ((int64_t) గరిష్ఠ_పూర్ణాంకం + 1) == గరిష్ఠ_పూర్ణాంకం);
  hb_always_assert (hb_clamp_గమ్యం<పూర్ణాంకం> ((int64_t) INT_MIN - 1) == INT_MIN);

  int64_t budget = 10;
  hb_always_assert (hb_budget_spend (budget, 4, 2));
  hb_always_assert (budget == 2);
  hb_always_assert (!hb_budget_spend (budget, 3));
  hb_always_assert (budget == -1);
  hb_always_assert (!hb_budget_spend (budget, 1));
  hb_always_assert (budget == -2);

  budget = 0;
  hb_always_assert (hb_budget_spend (budget, 0));
  hb_always_assert (!hb_budget_spend (budget, 1));
  hb_always_assert (budget == -1);

  budget = INT64_MAX;
  hb_always_assert (hb_budget_spend (budget, 1024));
  hb_always_assert (budget == INT64_MAX - 1024);

  x = 1;
  hb_always_assert (++hb_inc (x) == 3);
  hb_always_assert (x == 3);

  hb_సమితి_t set1 {1};
  hb_సమితి_t set2 {2};

  hb_always_assert (hb_hash (set1) != hb_hash (set2));
  hb_always_assert (hb_hash (set1) == hb_hash (hb_సమితి_t {1}));
  hb_always_assert (hb_hash (set1) != hb_hash (hb_సమితి_t {}));
  hb_always_assert (hb_hash (set1) != hb_hash (hb_సమితి_t {2}));
  hb_always_assert (hb_hash (set2) == hb_hash (hb_సమితి_t {2}));

  /* hb_hash, unlike std::hash, dereferences pointers. */
  hb_always_assert (hb_hash (set1) == hb_hash (&set1));
  hb_always_assert (hb_hash (set1) == hb_hash (hb::shared_ptr<hb_సమితి_t> {hb_సమితి_reference (&set1)}));
  hb_always_assert (hb_hash (set1) == hb_hash (hb::unique_ptr<hb_సమితి_t> {hb_సమితి_reference (&set1)}));

  తిరిగి 0;
}

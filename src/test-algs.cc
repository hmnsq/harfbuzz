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

#ضمن "hb.hh"
#ضمن "hb-algs.hh"
#ضمن "hb-set.hh"


ساكن حرف *
test_func (صحيح a, حرف **b)
{
  أرجع b ? b[a] : خالي_اشارو;
}

بنية A
{
  فراغ a () {}
};

صحيح
مکيه (صحيح argc, حرف **argv)
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

  صحيح i = 1;
  خودڪار p = hb_pair (1, i);

  p.second = 2;
  hb_always_assert (i == 2);

  ثابت صحيح c = 3;
  خودڪار pc = hb_pair (1, c);
  hb_always_assert (pc.second == 3);

  خودڪار q = p;
  hb_always_assert (&q != &p);
  q.second = 4;
  hb_always_assert (i == 4);

  hb_invoke (test_func, 0, خالي_اشارو);

  A a;
  hb_invoke (&A::a, a);

  hb_always_assert (1 == hb_min (8, 1));
  hb_always_assert (8 == hb_max (8, 1));

  صحيح x = 1, y = 2;
  hb_min (x, 3);
  hb_min (3, x);
  hb_min (x, 4 + 3);
  صحيح &z = hb_min (x, y);
  z = 3;
  hb_always_assert (x == 3);

  hb_pair_t<ثابت صحيح*, صحيح> xp = hb_pair_t<صحيح *, ڊگهو> (خالي_اشارو, 0);
  xp = hb_pair_t<صحيح *, مزدوج> (خالي_اشارو, 1);
  xp = hb_pair_t<ثابت صحيح*, صحيح> (خالي_اشارو, 1);

  hb_always_assert (3 == hb_partial (hb_min, 3) (4));
  hb_always_assert (3 == hb_partial<1> (hb_min, 4) (3));

  خودڪار M0 = hb_partial<2> (hb_max, 0);
  hb_always_assert (M0 (-2) == 0);
  hb_always_assert (M0 (+2) == 2);

  hb_always_assert (hb_جوڙيو (2) (5) == 7);
  hb_always_assert (hb_جوڙيو (5) (2) == 7);

  hb_always_assert (hb_saturate_جوڙيو (1, 2) == 3);
  hb_always_assert (hb_saturate_جوڙيو (وڌ_کان_وڌ_عدد, 1) == وڌ_کان_وڌ_عدد);
  hb_always_assert (hb_saturate_جوڙيو (INT_MIN, -1) == INT_MIN);
  hb_always_assert (hb_saturate_جوڙيو (وڌ_کان_وڌ_عدد, INT_MIN) == -1);
  hb_always_assert (hb_saturate_sub (3, 2) == 1);
  hb_always_assert (hb_saturate_sub (وڌ_کان_وڌ_عدد, -1) == وڌ_کان_وڌ_عدد);
  hb_always_assert (hb_saturate_sub (INT_MIN, 1) == INT_MIN);
  hb_always_assert (hb_saturate_sub (INT_MIN, INT_MIN) == 0);
  hb_always_assert (hb_saturate_neg (2) == -2);
  hb_always_assert (hb_saturate_neg (-2) == 2);
  hb_always_assert (hb_saturate_neg (INT_MIN) == وڌ_کان_وڌ_عدد);
  hb_always_assert (hb_clamp_منزل<صحيح> (1.) == 1);
  hb_always_assert (hb_clamp_منزل<صحيح> ((مزدوج) وڌ_کان_وڌ_عدد + 1.) == وڌ_کان_وڌ_عدد);
  hb_always_assert (hb_clamp_منزل<صحيح> ((مزدوج) INT_MIN - 1.) == INT_MIN);
  hb_always_assert (hb_clamp_منزل<صحيح> ((int64_t) 1) == 1);
  hb_always_assert (hb_clamp_منزل<صحيح> ((int64_t) وڌ_کان_وڌ_عدد + 1) == وڌ_کان_وڌ_عدد);
  hb_always_assert (hb_clamp_منزل<صحيح> ((int64_t) INT_MIN - 1) == INT_MIN);

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

  hb_سيٽ_t set1 {1};
  hb_سيٽ_t set2 {2};

  hb_always_assert (hb_hash (set1) != hb_hash (set2));
  hb_always_assert (hb_hash (set1) == hb_hash (hb_سيٽ_t {1}));
  hb_always_assert (hb_hash (set1) != hb_hash (hb_سيٽ_t {}));
  hb_always_assert (hb_hash (set1) != hb_hash (hb_سيٽ_t {2}));
  hb_always_assert (hb_hash (set2) == hb_hash (hb_سيٽ_t {2}));

  /* hb_hash, unlike std::hash, dereferences pointers. */
  hb_always_assert (hb_hash (set1) == hb_hash (&set1));
  hb_always_assert (hb_hash (set1) == hb_hash (hb::shared_ptr<hb_سيٽ_t> {hb_سيٽ_reference (&set1)}));
  hb_always_assert (hb_hash (set1) == hb_hash (hb::unique_ptr<hb_سيٽ_t> {hb_سيٽ_reference (&set1)}));

  أرجع 0;
}

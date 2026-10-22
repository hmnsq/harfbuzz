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

#شامل "hb.hh"
#شامل "hb-algs.hh"
#شامل "hb-set.hh"


ساکن حرف *
test_func (صحیح a, حرف **b)
{
  واپس b ? b[a] : خالی_اشاریہ;
}

ساخت A
{
  خالی a () {}
};

صحیح
مین (صحیح argc, حرف **argv)
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

  صحیح i = 1;
  خودکار p = hb_pair (1, i);

  p.second = 2;
  hb_always_assert (i == 2);

  ثابت صحیح c = 3;
  خودکار pc = hb_pair (1, c);
  hb_always_assert (pc.second == 3);

  خودکار q = p;
  hb_always_assert (&q != &p);
  q.second = 4;
  hb_always_assert (i == 4);

  hb_invoke (test_func, 0, خالی_اشاریہ);

  A a;
  hb_invoke (&A::a, a);

  hb_always_assert (1 == hb_min (8, 1));
  hb_always_assert (8 == hb_max (8, 1));

  صحیح x = 1, y = 2;
  hb_min (x, 3);
  hb_min (3, x);
  hb_min (x, 4 + 3);
  صحیح &z = hb_min (x, y);
  z = 3;
  hb_always_assert (x == 3);

  hb_pair_t<ثابت صحیح*, صحیح> xp = hb_pair_t<صحیح *, طویل> (خالی_اشاریہ, 0);
  xp = hb_pair_t<صحیح *, دوہرا> (خالی_اشاریہ, 1);
  xp = hb_pair_t<ثابت صحیح*, صحیح> (خالی_اشاریہ, 1);

  hb_always_assert (3 == hb_partial (hb_min, 3) (4));
  hb_always_assert (3 == hb_partial<1> (hb_min, 4) (3));

  خودکار M0 = hb_partial<2> (hb_max, 0);
  hb_always_assert (M0 (-2) == 0);
  hb_always_assert (M0 (+2) == 2);

  hb_always_assert (hb_جوڑو (2) (5) == 7);
  hb_always_assert (hb_جوڑو (5) (2) == 7);

  hb_always_assert (hb_saturate_جوڑو (1, 2) == 3);
  hb_always_assert (hb_saturate_جوڑو (زیادہ_سے_زیادہ_عدد, 1) == زیادہ_سے_زیادہ_عدد);
  hb_always_assert (hb_saturate_جوڑو (INT_MIN, -1) == INT_MIN);
  hb_always_assert (hb_saturate_جوڑو (زیادہ_سے_زیادہ_عدد, INT_MIN) == -1);
  hb_always_assert (hb_saturate_sub (3, 2) == 1);
  hb_always_assert (hb_saturate_sub (زیادہ_سے_زیادہ_عدد, -1) == زیادہ_سے_زیادہ_عدد);
  hb_always_assert (hb_saturate_sub (INT_MIN, 1) == INT_MIN);
  hb_always_assert (hb_saturate_sub (INT_MIN, INT_MIN) == 0);
  hb_always_assert (hb_saturate_neg (2) == -2);
  hb_always_assert (hb_saturate_neg (-2) == 2);
  hb_always_assert (hb_saturate_neg (INT_MIN) == زیادہ_سے_زیادہ_عدد);
  hb_always_assert (hb_clamp_مقصد<صحیح> (1.) == 1);
  hb_always_assert (hb_clamp_مقصد<صحیح> ((دوہرا) زیادہ_سے_زیادہ_عدد + 1.) == زیادہ_سے_زیادہ_عدد);
  hb_always_assert (hb_clamp_مقصد<صحیح> ((دوہرا) INT_MIN - 1.) == INT_MIN);
  hb_always_assert (hb_clamp_مقصد<صحیح> ((int64_t) 1) == 1);
  hb_always_assert (hb_clamp_مقصد<صحیح> ((int64_t) زیادہ_سے_زیادہ_عدد + 1) == زیادہ_سے_زیادہ_عدد);
  hb_always_assert (hb_clamp_مقصد<صحیح> ((int64_t) INT_MIN - 1) == INT_MIN);

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

  hb_سیٹ_t set1 {1};
  hb_سیٹ_t set2 {2};

  hb_always_assert (hb_hash (set1) != hb_hash (set2));
  hb_always_assert (hb_hash (set1) == hb_hash (hb_سیٹ_t {1}));
  hb_always_assert (hb_hash (set1) != hb_hash (hb_سیٹ_t {}));
  hb_always_assert (hb_hash (set1) != hb_hash (hb_سیٹ_t {2}));
  hb_always_assert (hb_hash (set2) == hb_hash (hb_سیٹ_t {2}));

  /* hb_hash, unlike std::hash, dereferences pointers. */
  hb_always_assert (hb_hash (set1) == hb_hash (&set1));
  hb_always_assert (hb_hash (set1) == hb_hash (hb::shared_ptr<hb_سیٹ_t> {hb_سیٹ_reference (&set1)}));
  hb_always_assert (hb_hash (set1) == hb_hash (hb::unique_ptr<hb_سیٹ_t> {hb_سیٹ_reference (&set1)}));

  واپس 0;
}

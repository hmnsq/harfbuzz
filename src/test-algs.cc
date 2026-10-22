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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-algs.hh"
#अन्तर्भाव "hb-set.hh"


स्थिर वर्ण *
test_func (पूर्णाङ्क a, वर्ण **b)
{
  प्रत्यागम b ? b[a] : nullptr;
}

संरचना A
{
  शून्य a () {}
};

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
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

  पूर्णाङ्क i = 1;
  auto p = hb_pair (1, i);

  p.second = 2;
  hb_always_assert (i == 2);

  ध्रुव पूर्णाङ्क c = 3;
  auto pc = hb_pair (1, c);
  hb_always_assert (pc.second == 3);

  auto q = p;
  hb_always_assert (&q != &p);
  q.second = 4;
  hb_always_assert (i == 4);

  hb_invoke (test_func, 0, nullptr);

  A a;
  hb_invoke (&A::a, a);

  hb_always_assert (1 == hb_min (8, 1));
  hb_always_assert (8 == hb_max (8, 1));

  पूर्णाङ्क x = 1, y = 2;
  hb_min (x, 3);
  hb_min (3, x);
  hb_min (x, 4 + 3);
  पूर्णाङ्क &z = hb_min (x, y);
  z = 3;
  hb_always_assert (x == 3);

  hb_pair_t<ध्रुव पूर्णाङ्क*, पूर्णाङ्क> xp = hb_pair_t<पूर्णाङ्क *, long> (nullptr, 0);
  xp = hb_pair_t<पूर्णाङ्क *, द्विगुण> (nullptr, 1);
  xp = hb_pair_t<ध्रुव पूर्णाङ्क*, पूर्णाङ्क> (nullptr, 1);

  hb_always_assert (3 == hb_partial (hb_min, 3) (4));
  hb_always_assert (3 == hb_partial<1> (hb_min, 4) (3));

  auto M0 = hb_partial<2> (hb_max, 0);
  hb_always_assert (M0 (-2) == 0);
  hb_always_assert (M0 (+2) == 2);

  hb_always_assert (hb_add (2) (5) == 7);
  hb_always_assert (hb_add (5) (2) == 7);

  hb_always_assert (hb_saturate_add (1, 2) == 3);
  hb_always_assert (hb_saturate_add (INT_MAX, 1) == INT_MAX);
  hb_always_assert (hb_saturate_add (INT_MIN, -1) == INT_MIN);
  hb_always_assert (hb_saturate_add (INT_MAX, INT_MIN) == -1);
  hb_always_assert (hb_saturate_sub (3, 2) == 1);
  hb_always_assert (hb_saturate_sub (INT_MAX, -1) == INT_MAX);
  hb_always_assert (hb_saturate_sub (INT_MIN, 1) == INT_MIN);
  hb_always_assert (hb_saturate_sub (INT_MIN, INT_MIN) == 0);
  hb_always_assert (hb_saturate_neg (2) == -2);
  hb_always_assert (hb_saturate_neg (-2) == 2);
  hb_always_assert (hb_saturate_neg (INT_MIN) == INT_MAX);
  hb_always_assert (hb_clamp_to<पूर्णाङ्क> (1.) == 1);
  hb_always_assert (hb_clamp_to<पूर्णाङ्क> ((द्विगुण) INT_MAX + 1.) == INT_MAX);
  hb_always_assert (hb_clamp_to<पूर्णाङ्क> ((द्विगुण) INT_MIN - 1.) == INT_MIN);
  hb_always_assert (hb_clamp_to<पूर्णाङ्क> ((int64_t) 1) == 1);
  hb_always_assert (hb_clamp_to<पूर्णाङ्क> ((int64_t) INT_MAX + 1) == INT_MAX);
  hb_always_assert (hb_clamp_to<पूर्णाङ्क> ((int64_t) INT_MIN - 1) == INT_MIN);

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

  hb_set_t set1 {1};
  hb_set_t set2 {2};

  hb_always_assert (hb_hash (set1) != hb_hash (set2));
  hb_always_assert (hb_hash (set1) == hb_hash (hb_set_t {1}));
  hb_always_assert (hb_hash (set1) != hb_hash (hb_set_t {}));
  hb_always_assert (hb_hash (set1) != hb_hash (hb_set_t {2}));
  hb_always_assert (hb_hash (set2) == hb_hash (hb_set_t {2}));

  /* hb_hash, unlike std::hash, dereferences pointers. */
  hb_always_assert (hb_hash (set1) == hb_hash (&set1));
  hb_always_assert (hb_hash (set1) == hb_hash (hb::shared_ptr<hb_set_t> {hb_set_reference (&set1)}));
  hb_always_assert (hb_hash (set1) == hb_hash (hb::unique_ptr<hb_set_t> {hb_set_reference (&set1)}));

  प्रत्यागम 0;
}

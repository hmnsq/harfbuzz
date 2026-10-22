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


स्थिर अक्षर *
test_func (पूर्णांक a, अक्षर **b)
{
  परत b ? b[a] : nullptr;
}

संरचना A
{
  शून्य a () {}
};

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
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

  पूर्णांक i = 1;
  स्वतः p = hb_pair (1, i);

  p.second = 2;
  hb_always_assert (i == 2);

  स्थिरांक पूर्णांक c = 3;
  स्वतः pc = hb_pair (1, c);
  hb_always_assert (pc.second == 3);

  स्वतः q = p;
  hb_always_assert (&q != &p);
  q.second = 4;
  hb_always_assert (i == 4);

  hb_invoke (test_func, 0, nullptr);

  A a;
  hb_invoke (&A::a, a);

  hb_always_assert (1 == hb_min (8, 1));
  hb_always_assert (8 == hb_max (8, 1));

  पूर्णांक x = 1, y = 2;
  hb_min (x, 3);
  hb_min (3, x);
  hb_min (x, 4 + 3);
  पूर्णांक &z = hb_min (x, y);
  z = 3;
  hb_always_assert (x == 3);

  hb_pair_t<स्थिरांक पूर्णांक*, पूर्णांक> xp = hb_pair_t<पूर्णांक *, दीर्घ> (nullptr, 0);
  xp = hb_pair_t<पूर्णांक *, द्विगुण> (nullptr, 1);
  xp = hb_pair_t<स्थिरांक पूर्णांक*, पूर्णांक> (nullptr, 1);

  hb_always_assert (3 == hb_partial (hb_min, 3) (4));
  hb_always_assert (3 == hb_partial<1> (hb_min, 4) (3));

  स्वतः M0 = hb_partial<2> (hb_max, 0);
  hb_always_assert (M0 (-2) == 0);
  hb_always_assert (M0 (+2) == 2);

  hb_always_assert (hb_add (2) (5) == 7);
  hb_always_assert (hb_add (5) (2) == 7);

  hb_always_assert (hb_saturate_add (1, 2) == 3);
  hb_always_assert (hb_saturate_add (पूर्णांक_उच्च, 1) == पूर्णांक_उच्च);
  hb_always_assert (hb_saturate_add (पूर्णांक_न्यून, -1) == पूर्णांक_न्यून);
  hb_always_assert (hb_saturate_add (पूर्णांक_उच्च, पूर्णांक_न्यून) == -1);
  hb_always_assert (hb_saturate_sub (3, 2) == 1);
  hb_always_assert (hb_saturate_sub (पूर्णांक_उच्च, -1) == पूर्णांक_उच्च);
  hb_always_assert (hb_saturate_sub (पूर्णांक_न्यून, 1) == पूर्णांक_न्यून);
  hb_always_assert (hb_saturate_sub (पूर्णांक_न्यून, पूर्णांक_न्यून) == 0);
  hb_always_assert (hb_saturate_neg (2) == -2);
  hb_always_assert (hb_saturate_neg (-2) == 2);
  hb_always_assert (hb_saturate_neg (पूर्णांक_न्यून) == पूर्णांक_उच्च);
  hb_always_assert (hb_clamp_to<पूर्णांक> (1.) == 1);
  hb_always_assert (hb_clamp_to<पूर्णांक> ((द्विगुण) पूर्णांक_उच्च + 1.) == पूर्णांक_उच्च);
  hb_always_assert (hb_clamp_to<पूर्णांक> ((द्विगुण) पूर्णांक_न्यून - 1.) == पूर्णांक_न्यून);
  hb_always_assert (hb_clamp_to<पूर्णांक> ((int64_t) 1) == 1);
  hb_always_assert (hb_clamp_to<पूर्णांक> ((int64_t) पूर्णांक_उच्च + 1) == पूर्णांक_उच्च);
  hb_always_assert (hb_clamp_to<पूर्णांक> ((int64_t) पूर्णांक_न्यून - 1) == पूर्णांक_न्यून);

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

  परत 0;
}

/*
 * Copyright © 2021  Behdad Esfahbod
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
 */

#ضمن "hb.hh"
#ضمن "hb-vector.hh"
#ضمن "hb-set.hh"
#ضمن "hb-map.hh"
#ضمن <string>


صحيح
اصلي (صحيح argc, حرف **argv)
{
  hb_always_assert (حجم (hb_ویکټور_t<صحيح>) == حجم (hb_sorted_ویکټور_t<صحيح>));

  /* Test copy constructor. */
  {
    hb_ویکټور_t<صحيح> v1 {1, 2};
    hb_ویکټور_t<صحيح> v2 {v1};
    hb_ویکټور_t<صحيح> V2 {v1};
    hb_always_assert (v1.اوږدتیا == 2);
    hb_always_assert (v1[0] == 1);
    hb_always_assert (v1[1] == 2);
    hb_always_assert (v2.اوږدتیا == 2);
    hb_always_assert (v2[0] == 1);
    hb_always_assert (v2[1] == 2);
  }

  /* Test copy assignment. */
  {
    hb_ویکټور_t<صحيح> v1 {1, 2};
    hb_ویکټور_t<صحيح> v2 = v1;
    hb_ویکټور_t<صحيح> V2 = v1;
    hb_always_assert (v1.اوږدتیا == 2);
    hb_always_assert (v1[0] == 1);
    hb_always_assert (v1[1] == 2);
    hb_always_assert (v2.اوږدتیا == 2);
    hb_always_assert (v2[0] == 1);
    hb_always_assert (v2[1] == 2);
  }

  /* Self-copy assignment must preserve both trivial and nontrivial items. */
  {
    hb_ویکټور_t<صحيح> v {1, 2};
    ثابت خپلکاری &self = v;
    v = self;
    hb_always_assert (v.اوږدتیا == 2);
    hb_always_assert (v[0] == 1);
    hb_always_assert (v[1] == 2);
  }
  {
    hb_ویکټور_t<std::string> v;
    v.push (std::string ("one"));
    v.push (std::string ("two"));
    ثابت خپلکاری &self = v;
    v = self;
    hb_always_assert (v.اوږدتیا == 2);
    hb_always_assert (v[0] == "one");
    hb_always_assert (v[1] == "two");
  }

  /* Test pushing an element of the vector itself while it grows. */
  {
    hb_ویکټور_t<صحيح> v {7};
    لكل (بینښه i = 0; i < 1000; i++)
      v.push (v.arrayZ[0]);
    hb_always_assert (v.اوږدتیا == 1001);
    لكل (بینښه i = 0; i < v.اوږدتیا; i++)
      hb_always_assert (v[i] == 7);
  }
  {
    hb_ویکټور_t<std::string> v;
    v.push (std::string ("a string longer than the small-string buffer"));
    لكل (بینښه i = 0; i < 1000; i++)
      v.push (v.arrayZ[i]);
    hb_always_assert (v.اوږدتیا == 1001);
    لكل (بینښه i = 0; i < v.اوږدتیا; i++)
      hb_always_assert (v[i] == "a string longer than the small-string buffer");
  }
  {
    hb_ویکټور_t<hb_ویکټور_t<صحيح>> v;
    v.push (hb_ویکټور_t<صحيح> {1, 2, 3});
    لكل (بینښه i = 0; i < 1000; i++)
      v.push (v.arrayZ[0]);
    hb_always_assert (v.اوږدتیا == 1001);
    لكل (بینښه i = 0; i < v.اوږدتیا; i++)
      hb_always_assert (v[i].اوږدتیا == 3 && v[i][2] == 3);
  }
  {
    hb_ویکټور_t<hb_ویکټور_t<صحيح>> v {hb_ویکټور_t<صحيح> {1, 2, 3}};
    v.push (std::move (v[0]));
    hb_always_assert (v.اوږدتیا == 2 && v[0].اوږدتیا == 0);
    hb_always_assert (v[1].اوږدتیا == 3 && v[1][2] == 3);
  }
  {
    hb_ویکټور_t<std::string> v {"one"};
    v.push (3, v[0][0]);
    hb_always_assert (v.اوږدتیا == 2);
    hb_always_assert (v[0] == "one" && v[1] == "ooo");
  }
  {
    hb_ویکټور_t<صحيح> v {1, 2, 3};
    v.extend (v.as_array ());
    hb_always_assert (v.اوږدتیا == 6);
    hb_always_assert (v[3] == 1 && v[4] == 2 && v[5] == 3);
  }
  {
    hb_ویکټور_t<صحيح> v {1, 2, 3};
    v.extend (hb_array_t<ثابت صحيح> (v.arrayZ + 1, 2));
    hb_always_assert (v.اوږدتیا == 5);
    hb_always_assert (v[3] == 2 && v[4] == 3);
  }
  {
    hb_ویکټور_t<std::string> v {"one", "two", "three"};
    v.extend (v.as_array ());
    hb_always_assert (v.اوږدتیا == 6);
    لكل (بینښه i = 0; i < 3; i++)
      hb_always_assert (v[i] == v[i + 3]);
  }
  {
    hb_ویکټور_t<std::string> v {"one", "two", "three"};
    v.extend (v);
    hb_always_assert (v.اوږدتیا == 6);
    لكل (بینښه i = 0; i < 3; i++)
      hb_always_assert (v[i] == v[i + 3]);
  }

  /* A failed vector must not move from an aliased argument. */
  {
    hb_ویکټور_t<std::string> v {"one"};
    v.سېټ_error ();
    hb_always_assert (!v.ټېل_or_fail (std::move (v[0])));
    hb_always_assert (v.اوږدتیا == 1 && v[0] == "one");
  }

  /* Test move constructor. */
  {
    hb_ویکټور_t<صحيح> s {1, 2};
    hb_sorted_ویکټور_t<صحيح> S {1, 2};
    hb_ویکټور_t<صحيح> v (std::move (s));
    hb_sorted_ویکټور_t<صحيح> V (std::move (S));
    hb_always_assert (s.اوږدتیا == 0);
    hb_always_assert (S.اوږدتیا == 0);
    hb_always_assert (v.اوږدتیا == 2);
    hb_always_assert (v[0] == 1);
    hb_always_assert (v[1] == 2);
  }

  /* Test move assignment. */
  {
    hb_ویکټور_t<صحيح> s {1, 2};
    hb_sorted_ویکټور_t<صحيح> S {1, 2};
    hb_ویکټور_t<صحيح> v;
    hb_sorted_ویکټور_t<صحيح> V;
    v = std::move (s);
    V = std::move (S);
    hb_always_assert (s.اوږدتیا == 0);
    hb_always_assert (S.اوږدتیا == 0);
    hb_always_assert (v.اوږدتیا == 2);
    hb_always_assert (V.اوږدتیا == 2);
    hb_always_assert (v[0] == 1);
    hb_always_assert (v[1] == 2);
  }

  /* Test initializing from iterable. */
  {
    hb_سېټ_t s;

    s.جمع_کړه (18);
    s.جمع_کړه (12);

    hb_ویکټور_t<صحيح> v (s);
    hb_sorted_ویکټور_t<صحيح> V (s);

    hb_always_assert (v.اوږدتیا == 2);
    hb_always_assert (V.اوږدتیا == 2);
    hb_always_assert (v[0] == 12);
    hb_always_assert (V[0] == 12);
    hb_always_assert (v[1] == 18);
    hb_always_assert (V[1] == 18);
  }

  /* Test initializing from iterator. */
  {
    hb_سېټ_t s;

    s.جمع_کړه (18);
    s.جمع_کړه (12);

    hb_ویکټور_t<صحيح> v (hb_iter (s));
    hb_ویکټور_t<صحيح> V (hb_iter (s));

    hb_always_assert (v.اوږدتیا == 2);
    hb_always_assert (V.اوږدتیا == 2);
    hb_always_assert (v[0] == 12);
    hb_always_assert (V[0] == 12);
    hb_always_assert (v[1] == 18);
    hb_always_assert (V[1] == 18);
  }

  /* Test initializing from initializer list and swapping. */
  {
    hb_ویکټور_t<صحيح> v1 {1, 2, 3};
    hb_ویکټور_t<صحيح> v2 {4, 5};
    hb_بدلون (v1, v2);
    hb_always_assert (v1.اوږدتیا == 2);
    hb_always_assert (v1[0] == 4);
    hb_always_assert (v2.اوږدتیا == 3);
    hb_always_assert (v2[2] == 3);
  }

  /* Test initializing sorted-vector from initializer list and swapping. */
  {
    hb_sorted_ویکټور_t<صحيح> v1 {1, 2, 3};
    hb_sorted_ویکټور_t<صحيح> v2 {4, 5};
    hb_بدلون (v1, v2);
    hb_always_assert (v1.اوږدتیا == 2);
    hb_always_assert (v1[0] == 4);
    hb_always_assert (v2.اوږدتیا == 3);
    hb_always_assert (v2[2] == 3);
  }

  {
    hb_ویکټور_t<std::string> v;

    std::string s;
    لكل (بینښه i = 1; i < 100; i++)
    {
      s += "x";
      v.push (s);
    }

    hb_ویکټور_t<std::string> v2;

    v2 = v;

    v2.remove_ordered (50);
    v2.remove_unordered (50);
  }

  {
    hb_ویکټور_t<hb_سېټ_t> v;
    hb_سېټ_t s {1, 5, 7};
    v.push (s);
    v << s;
    hb_always_assert (s.get_population () == 3);
    v << std::move (s);
    hb_always_assert (s.get_population () == 0);
  }

  {
    hb_ویکټور_t<hb_map_t> v;
    hb_map_t m;
    v.push (m);
  }
  {
    hb_ویکټور_t<hb_map_t> v;
    hb_map_t m;
    v.push (m);
  }

  أرجع 0;
}

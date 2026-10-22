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

#شامل "hb.hh"
#شامل "hb-vector.hh"
#شامل "hb-set.hh"
#شامل "hb-map.hh"
#شامل <string>


صحیح
مین (صحیح argc, حرف **argv)
{
  hb_always_assert (حجم (hb_سمتیہ_t<صحیح>) == حجم (hb_sorted_سمتیہ_t<صحیح>));

  /* Test copy constructor. */
  {
    hb_سمتیہ_t<صحیح> v1 {1, 2};
    hb_سمتیہ_t<صحیح> v2 {v1};
    hb_سمتیہ_t<صحیح> V2 {v1};
    hb_always_assert (v1.لمبائی == 2);
    hb_always_assert (v1[0] == 1);
    hb_always_assert (v1[1] == 2);
    hb_always_assert (v2.لمبائی == 2);
    hb_always_assert (v2[0] == 1);
    hb_always_assert (v2[1] == 2);
  }

  /* Test copy assignment. */
  {
    hb_سمتیہ_t<صحیح> v1 {1, 2};
    hb_سمتیہ_t<صحیح> v2 = v1;
    hb_سمتیہ_t<صحیح> V2 = v1;
    hb_always_assert (v1.لمبائی == 2);
    hb_always_assert (v1[0] == 1);
    hb_always_assert (v1[1] == 2);
    hb_always_assert (v2.لمبائی == 2);
    hb_always_assert (v2[0] == 1);
    hb_always_assert (v2[1] == 2);
  }

  /* Self-copy assignment must preserve both trivial and nontrivial items. */
  {
    hb_سمتیہ_t<صحیح> v {1, 2};
    ثابت خودکار &self = v;
    v = self;
    hb_always_assert (v.لمبائی == 2);
    hb_always_assert (v[0] == 1);
    hb_always_assert (v[1] == 2);
  }
  {
    hb_سمتیہ_t<std::string> v;
    v.push (std::string ("one"));
    v.push (std::string ("two"));
    ثابت خودکار &self = v;
    v = self;
    hb_always_assert (v.لمبائی == 2);
    hb_always_assert (v[0] == "one");
    hb_always_assert (v[1] == "two");
  }

  /* Test pushing an element of the vector itself while it grows. */
  {
    hb_سمتیہ_t<صحیح> v {7};
    برائے (غیرعلامتی i = 0; i < 1000; i++)
      v.push (v.arrayZ[0]);
    hb_always_assert (v.لمبائی == 1001);
    برائے (غیرعلامتی i = 0; i < v.لمبائی; i++)
      hb_always_assert (v[i] == 7);
  }
  {
    hb_سمتیہ_t<std::string> v;
    v.push (std::string ("a string longer than the small-string buffer"));
    برائے (غیرعلامتی i = 0; i < 1000; i++)
      v.push (v.arrayZ[i]);
    hb_always_assert (v.لمبائی == 1001);
    برائے (غیرعلامتی i = 0; i < v.لمبائی; i++)
      hb_always_assert (v[i] == "a string longer than the small-string buffer");
  }
  {
    hb_سمتیہ_t<hb_سمتیہ_t<صحیح>> v;
    v.push (hb_سمتیہ_t<صحیح> {1, 2, 3});
    برائے (غیرعلامتی i = 0; i < 1000; i++)
      v.push (v.arrayZ[0]);
    hb_always_assert (v.لمبائی == 1001);
    برائے (غیرعلامتی i = 0; i < v.لمبائی; i++)
      hb_always_assert (v[i].لمبائی == 3 && v[i][2] == 3);
  }
  {
    hb_سمتیہ_t<hb_سمتیہ_t<صحیح>> v {hb_سمتیہ_t<صحیح> {1, 2, 3}};
    v.push (std::move (v[0]));
    hb_always_assert (v.لمبائی == 2 && v[0].لمبائی == 0);
    hb_always_assert (v[1].لمبائی == 3 && v[1][2] == 3);
  }
  {
    hb_سمتیہ_t<std::string> v {"one"};
    v.push (3, v[0][0]);
    hb_always_assert (v.لمبائی == 2);
    hb_always_assert (v[0] == "one" && v[1] == "ooo");
  }
  {
    hb_سمتیہ_t<صحیح> v {1, 2, 3};
    v.extend (v.as_array ());
    hb_always_assert (v.لمبائی == 6);
    hb_always_assert (v[3] == 1 && v[4] == 2 && v[5] == 3);
  }
  {
    hb_سمتیہ_t<صحیح> v {1, 2, 3};
    v.extend (hb_array_t<ثابت صحیح> (v.arrayZ + 1, 2));
    hb_always_assert (v.لمبائی == 5);
    hb_always_assert (v[3] == 2 && v[4] == 3);
  }
  {
    hb_سمتیہ_t<std::string> v {"one", "two", "three"};
    v.extend (v.as_array ());
    hb_always_assert (v.لمبائی == 6);
    برائے (غیرعلامتی i = 0; i < 3; i++)
      hb_always_assert (v[i] == v[i + 3]);
  }
  {
    hb_سمتیہ_t<std::string> v {"one", "two", "three"};
    v.extend (v);
    hb_always_assert (v.لمبائی == 6);
    برائے (غیرعلامتی i = 0; i < 3; i++)
      hb_always_assert (v[i] == v[i + 3]);
  }

  /* A failed vector must not move from an aliased argument. */
  {
    hb_سمتیہ_t<std::string> v {"one"};
    v.سیٹ_error ();
    hb_always_assert (!v.ڈالو_or_fail (std::move (v[0])));
    hb_always_assert (v.لمبائی == 1 && v[0] == "one");
  }

  /* Test move constructor. */
  {
    hb_سمتیہ_t<صحیح> s {1, 2};
    hb_sorted_سمتیہ_t<صحیح> S {1, 2};
    hb_سمتیہ_t<صحیح> v (std::move (s));
    hb_sorted_سمتیہ_t<صحیح> V (std::move (S));
    hb_always_assert (s.لمبائی == 0);
    hb_always_assert (S.لمبائی == 0);
    hb_always_assert (v.لمبائی == 2);
    hb_always_assert (v[0] == 1);
    hb_always_assert (v[1] == 2);
  }

  /* Test move assignment. */
  {
    hb_سمتیہ_t<صحیح> s {1, 2};
    hb_sorted_سمتیہ_t<صحیح> S {1, 2};
    hb_سمتیہ_t<صحیح> v;
    hb_sorted_سمتیہ_t<صحیح> V;
    v = std::move (s);
    V = std::move (S);
    hb_always_assert (s.لمبائی == 0);
    hb_always_assert (S.لمبائی == 0);
    hb_always_assert (v.لمبائی == 2);
    hb_always_assert (V.لمبائی == 2);
    hb_always_assert (v[0] == 1);
    hb_always_assert (v[1] == 2);
  }

  /* Test initializing from iterable. */
  {
    hb_سیٹ_t s;

    s.جوڑو (18);
    s.جوڑو (12);

    hb_سمتیہ_t<صحیح> v (s);
    hb_sorted_سمتیہ_t<صحیح> V (s);

    hb_always_assert (v.لمبائی == 2);
    hb_always_assert (V.لمبائی == 2);
    hb_always_assert (v[0] == 12);
    hb_always_assert (V[0] == 12);
    hb_always_assert (v[1] == 18);
    hb_always_assert (V[1] == 18);
  }

  /* Test initializing from iterator. */
  {
    hb_سیٹ_t s;

    s.جوڑو (18);
    s.جوڑو (12);

    hb_سمتیہ_t<صحیح> v (hb_iter (s));
    hb_سمتیہ_t<صحیح> V (hb_iter (s));

    hb_always_assert (v.لمبائی == 2);
    hb_always_assert (V.لمبائی == 2);
    hb_always_assert (v[0] == 12);
    hb_always_assert (V[0] == 12);
    hb_always_assert (v[1] == 18);
    hb_always_assert (V[1] == 18);
  }

  /* Test initializing from initializer list and swapping. */
  {
    hb_سمتیہ_t<صحیح> v1 {1, 2, 3};
    hb_سمتیہ_t<صحیح> v2 {4, 5};
    hb_تبادلہ (v1, v2);
    hb_always_assert (v1.لمبائی == 2);
    hb_always_assert (v1[0] == 4);
    hb_always_assert (v2.لمبائی == 3);
    hb_always_assert (v2[2] == 3);
  }

  /* Test initializing sorted-vector from initializer list and swapping. */
  {
    hb_sorted_سمتیہ_t<صحیح> v1 {1, 2, 3};
    hb_sorted_سمتیہ_t<صحیح> v2 {4, 5};
    hb_تبادلہ (v1, v2);
    hb_always_assert (v1.لمبائی == 2);
    hb_always_assert (v1[0] == 4);
    hb_always_assert (v2.لمبائی == 3);
    hb_always_assert (v2[2] == 3);
  }

  {
    hb_سمتیہ_t<std::string> v;

    std::string s;
    برائے (غیرعلامتی i = 1; i < 100; i++)
    {
      s += "x";
      v.push (s);
    }

    hb_سمتیہ_t<std::string> v2;

    v2 = v;

    v2.remove_ordered (50);
    v2.remove_unordered (50);
  }

  {
    hb_سمتیہ_t<hb_سیٹ_t> v;
    hb_سیٹ_t s {1, 5, 7};
    v.push (s);
    v << s;
    hb_always_assert (s.get_population () == 3);
    v << std::move (s);
    hb_always_assert (s.get_population () == 0);
  }

  {
    hb_سمتیہ_t<hb_map_t> v;
    hb_map_t m;
    v.push (m);
  }
  {
    hb_سمتیہ_t<hb_map_t> v;
    hb_map_t m;
    v.push (m);
  }

  واپس 0;
}

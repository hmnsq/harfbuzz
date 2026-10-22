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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-vector.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-set.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-map.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ <string>


ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  hb_always_assert (ਆਕਾਰ (hb_ਸਦਿਸ਼_t<ਪੂਰਨ>) == ਆਕਾਰ (hb_sorted_ਸਦਿਸ਼_t<ਪੂਰਨ>));

  /* Test copy constructor. */
  {
    hb_ਸਦਿਸ਼_t<ਪੂਰਨ> v1 {1, 2};
    hb_ਸਦਿਸ਼_t<ਪੂਰਨ> v2 {v1};
    hb_ਸਦਿਸ਼_t<ਪੂਰਨ> V2 {v1};
    hb_always_assert (v1.ਲੰਮਾਈ == 2);
    hb_always_assert (v1[0] == 1);
    hb_always_assert (v1[1] == 2);
    hb_always_assert (v2.ਲੰਮਾਈ == 2);
    hb_always_assert (v2[0] == 1);
    hb_always_assert (v2[1] == 2);
  }

  /* Test copy assignment. */
  {
    hb_ਸਦਿਸ਼_t<ਪੂਰਨ> v1 {1, 2};
    hb_ਸਦਿਸ਼_t<ਪੂਰਨ> v2 = v1;
    hb_ਸਦਿਸ਼_t<ਪੂਰਨ> V2 = v1;
    hb_always_assert (v1.ਲੰਮਾਈ == 2);
    hb_always_assert (v1[0] == 1);
    hb_always_assert (v1[1] == 2);
    hb_always_assert (v2.ਲੰਮਾਈ == 2);
    hb_always_assert (v2[0] == 1);
    hb_always_assert (v2[1] == 2);
  }

  /* Self-copy assignment must preserve both trivial and nontrivial items. */
  {
    hb_ਸਦਿਸ਼_t<ਪੂਰਨ> v {1, 2};
    ਅਚਲ ਸਵੈਚਾਲਿਤ &self = v;
    v = self;
    hb_always_assert (v.ਲੰਮਾਈ == 2);
    hb_always_assert (v[0] == 1);
    hb_always_assert (v[1] == 2);
  }
  {
    hb_ਸਦਿਸ਼_t<std::string> v;
    v.push (std::string ("one"));
    v.push (std::string ("two"));
    ਅਚਲ ਸਵੈਚਾਲਿਤ &self = v;
    v = self;
    hb_always_assert (v.ਲੰਮਾਈ == 2);
    hb_always_assert (v[0] == "one");
    hb_always_assert (v[1] == "two");
  }

  /* Test pushing an element of the vector itself while it grows. */
  {
    hb_ਸਦਿਸ਼_t<ਪੂਰਨ> v {7};
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < 1000; i++)
      v.push (v.arrayZ[0]);
    hb_always_assert (v.ਲੰਮਾਈ == 1001);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < v.ਲੰਮਾਈ; i++)
      hb_always_assert (v[i] == 7);
  }
  {
    hb_ਸਦਿਸ਼_t<std::string> v;
    v.push (std::string ("a string longer than the small-string buffer"));
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < 1000; i++)
      v.push (v.arrayZ[i]);
    hb_always_assert (v.ਲੰਮਾਈ == 1001);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < v.ਲੰਮਾਈ; i++)
      hb_always_assert (v[i] == "a string longer than the small-string buffer");
  }
  {
    hb_ਸਦਿਸ਼_t<hb_ਸਦਿਸ਼_t<ਪੂਰਨ>> v;
    v.push (hb_ਸਦਿਸ਼_t<ਪੂਰਨ> {1, 2, 3});
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < 1000; i++)
      v.push (v.arrayZ[0]);
    hb_always_assert (v.ਲੰਮਾਈ == 1001);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < v.ਲੰਮਾਈ; i++)
      hb_always_assert (v[i].ਲੰਮਾਈ == 3 && v[i][2] == 3);
  }
  {
    hb_ਸਦਿਸ਼_t<hb_ਸਦਿਸ਼_t<ਪੂਰਨ>> v {hb_ਸਦਿਸ਼_t<ਪੂਰਨ> {1, 2, 3}};
    v.push (std::move (v[0]));
    hb_always_assert (v.ਲੰਮਾਈ == 2 && v[0].ਲੰਮਾਈ == 0);
    hb_always_assert (v[1].ਲੰਮਾਈ == 3 && v[1][2] == 3);
  }
  {
    hb_ਸਦਿਸ਼_t<std::string> v {"one"};
    v.push (3, v[0][0]);
    hb_always_assert (v.ਲੰਮਾਈ == 2);
    hb_always_assert (v[0] == "one" && v[1] == "ooo");
  }
  {
    hb_ਸਦਿਸ਼_t<ਪੂਰਨ> v {1, 2, 3};
    v.extend (v.as_array ());
    hb_always_assert (v.ਲੰਮਾਈ == 6);
    hb_always_assert (v[3] == 1 && v[4] == 2 && v[5] == 3);
  }
  {
    hb_ਸਦਿਸ਼_t<ਪੂਰਨ> v {1, 2, 3};
    v.extend (hb_array_t<ਅਚਲ ਪੂਰਨ> (v.arrayZ + 1, 2));
    hb_always_assert (v.ਲੰਮਾਈ == 5);
    hb_always_assert (v[3] == 2 && v[4] == 3);
  }
  {
    hb_ਸਦਿਸ਼_t<std::string> v {"one", "two", "three"};
    v.extend (v.as_array ());
    hb_always_assert (v.ਲੰਮਾਈ == 6);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < 3; i++)
      hb_always_assert (v[i] == v[i + 3]);
  }
  {
    hb_ਸਦਿਸ਼_t<std::string> v {"one", "two", "three"};
    v.extend (v);
    hb_always_assert (v.ਲੰਮਾਈ == 6);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < 3; i++)
      hb_always_assert (v[i] == v[i + 3]);
  }

  /* A failed vector must not move from an aliased argument. */
  {
    hb_ਸਦਿਸ਼_t<std::string> v {"one"};
    v.ਸੈੱਟ_error ();
    hb_always_assert (!v.ਧੱਕੋ_or_fail (std::move (v[0])));
    hb_always_assert (v.ਲੰਮਾਈ == 1 && v[0] == "one");
  }

  /* Test move constructor. */
  {
    hb_ਸਦਿਸ਼_t<ਪੂਰਨ> s {1, 2};
    hb_sorted_ਸਦਿਸ਼_t<ਪੂਰਨ> S {1, 2};
    hb_ਸਦਿਸ਼_t<ਪੂਰਨ> v (std::move (s));
    hb_sorted_ਸਦਿਸ਼_t<ਪੂਰਨ> V (std::move (S));
    hb_always_assert (s.ਲੰਮਾਈ == 0);
    hb_always_assert (S.ਲੰਮਾਈ == 0);
    hb_always_assert (v.ਲੰਮਾਈ == 2);
    hb_always_assert (v[0] == 1);
    hb_always_assert (v[1] == 2);
  }

  /* Test move assignment. */
  {
    hb_ਸਦਿਸ਼_t<ਪੂਰਨ> s {1, 2};
    hb_sorted_ਸਦਿਸ਼_t<ਪੂਰਨ> S {1, 2};
    hb_ਸਦਿਸ਼_t<ਪੂਰਨ> v;
    hb_sorted_ਸਦਿਸ਼_t<ਪੂਰਨ> V;
    v = std::move (s);
    V = std::move (S);
    hb_always_assert (s.ਲੰਮਾਈ == 0);
    hb_always_assert (S.ਲੰਮਾਈ == 0);
    hb_always_assert (v.ਲੰਮਾਈ == 2);
    hb_always_assert (V.ਲੰਮਾਈ == 2);
    hb_always_assert (v[0] == 1);
    hb_always_assert (v[1] == 2);
  }

  /* Test initializing from iterable. */
  {
    hb_ਸੈੱਟ_t s;

    s.ਜੋੜੋ (18);
    s.ਜੋੜੋ (12);

    hb_ਸਦਿਸ਼_t<ਪੂਰਨ> v (s);
    hb_sorted_ਸਦਿਸ਼_t<ਪੂਰਨ> V (s);

    hb_always_assert (v.ਲੰਮਾਈ == 2);
    hb_always_assert (V.ਲੰਮਾਈ == 2);
    hb_always_assert (v[0] == 12);
    hb_always_assert (V[0] == 12);
    hb_always_assert (v[1] == 18);
    hb_always_assert (V[1] == 18);
  }

  /* Test initializing from iterator. */
  {
    hb_ਸੈੱਟ_t s;

    s.ਜੋੜੋ (18);
    s.ਜੋੜੋ (12);

    hb_ਸਦਿਸ਼_t<ਪੂਰਨ> v (hb_iter (s));
    hb_ਸਦਿਸ਼_t<ਪੂਰਨ> V (hb_iter (s));

    hb_always_assert (v.ਲੰਮਾਈ == 2);
    hb_always_assert (V.ਲੰਮਾਈ == 2);
    hb_always_assert (v[0] == 12);
    hb_always_assert (V[0] == 12);
    hb_always_assert (v[1] == 18);
    hb_always_assert (V[1] == 18);
  }

  /* Test initializing from initializer list and swapping. */
  {
    hb_ਸਦਿਸ਼_t<ਪੂਰਨ> v1 {1, 2, 3};
    hb_ਸਦਿਸ਼_t<ਪੂਰਨ> v2 {4, 5};
    hb_ਅਦਲਾ_ਬਦਲੀ (v1, v2);
    hb_always_assert (v1.ਲੰਮਾਈ == 2);
    hb_always_assert (v1[0] == 4);
    hb_always_assert (v2.ਲੰਮਾਈ == 3);
    hb_always_assert (v2[2] == 3);
  }

  /* Test initializing sorted-vector from initializer list and swapping. */
  {
    hb_sorted_ਸਦਿਸ਼_t<ਪੂਰਨ> v1 {1, 2, 3};
    hb_sorted_ਸਦਿਸ਼_t<ਪੂਰਨ> v2 {4, 5};
    hb_ਅਦਲਾ_ਬਦਲੀ (v1, v2);
    hb_always_assert (v1.ਲੰਮਾਈ == 2);
    hb_always_assert (v1[0] == 4);
    hb_always_assert (v2.ਲੰਮਾਈ == 3);
    hb_always_assert (v2[2] == 3);
  }

  {
    hb_ਸਦਿਸ਼_t<std::string> v;

    std::string s;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 1; i < 100; i++)
    {
      s += "x";
      v.push (s);
    }

    hb_ਸਦਿਸ਼_t<std::string> v2;

    v2 = v;

    v2.remove_ordered (50);
    v2.remove_unordered (50);
  }

  {
    hb_ਸਦਿਸ਼_t<hb_ਸੈੱਟ_t> v;
    hb_ਸੈੱਟ_t s {1, 5, 7};
    v.push (s);
    v << s;
    hb_always_assert (s.get_population () == 3);
    v << std::move (s);
    hb_always_assert (s.get_population () == 0);
  }

  {
    hb_ਸਦਿਸ਼_t<hb_map_t> v;
    hb_map_t m;
    v.push (m);
  }
  {
    hb_ਸਦਿਸ਼_t<hb_map_t> v;
    hb_map_t m;
    v.push (m);
  }

  ਵਾਪਸ 0;
}

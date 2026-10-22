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

#समावेश "hb.hh"
#समावेश "hb-vector.hh"
#समावेश "hb-set.hh"
#समावेश "hb-map.hh"
#समावेश <string>


पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  hb_always_assert (माप (hb_vector_t<पूर्णांक>) == माप (hb_sorted_vector_t<पूर्णांक>));

  /* Test copy constructor. */
  {
    hb_vector_t<पूर्णांक> v1 {1, 2};
    hb_vector_t<पूर्णांक> v2 {v1};
    hb_vector_t<पूर्णांक> V2 {v1};
    hb_always_assert (v1.लंबाई == 2);
    hb_always_assert (v1[0] == 1);
    hb_always_assert (v1[1] == 2);
    hb_always_assert (v2.लंबाई == 2);
    hb_always_assert (v2[0] == 1);
    hb_always_assert (v2[1] == 2);
  }

  /* Test copy assignment. */
  {
    hb_vector_t<पूर्णांक> v1 {1, 2};
    hb_vector_t<पूर्णांक> v2 = v1;
    hb_vector_t<पूर्णांक> V2 = v1;
    hb_always_assert (v1.लंबाई == 2);
    hb_always_assert (v1[0] == 1);
    hb_always_assert (v1[1] == 2);
    hb_always_assert (v2.लंबाई == 2);
    hb_always_assert (v2[0] == 1);
    hb_always_assert (v2[1] == 2);
  }

  /* Self-copy assignment must preserve both trivial and nontrivial items. */
  {
    hb_vector_t<पूर्णांक> v {1, 2};
    स्थिर स्वतः &self = v;
    v = self;
    hb_always_assert (v.लंबाई == 2);
    hb_always_assert (v[0] == 1);
    hb_always_assert (v[1] == 2);
  }
  {
    hb_vector_t<std::string> v;
    v.push (std::string ("one"));
    v.push (std::string ("two"));
    स्थिर स्वतः &self = v;
    v = self;
    hb_always_assert (v.लंबाई == 2);
    hb_always_assert (v[0] == "one");
    hb_always_assert (v[1] == "two");
  }

  /* Test pushing an element of the vector itself while it grows. */
  {
    hb_vector_t<पूर्णांक> v {7};
    क्रम (अचिन्हित i = 0; i < 1000; i++)
      v.push (v.arrayZ[0]);
    hb_always_assert (v.लंबाई == 1001);
    क्रम (अचिन्हित i = 0; i < v.लंबाई; i++)
      hb_always_assert (v[i] == 7);
  }
  {
    hb_vector_t<std::string> v;
    v.push (std::string ("a string longer than the small-string buffer"));
    क्रम (अचिन्हित i = 0; i < 1000; i++)
      v.push (v.arrayZ[i]);
    hb_always_assert (v.लंबाई == 1001);
    क्रम (अचिन्हित i = 0; i < v.लंबाई; i++)
      hb_always_assert (v[i] == "a string longer than the small-string buffer");
  }
  {
    hb_vector_t<hb_vector_t<पूर्णांक>> v;
    v.push (hb_vector_t<पूर्णांक> {1, 2, 3});
    क्रम (अचिन्हित i = 0; i < 1000; i++)
      v.push (v.arrayZ[0]);
    hb_always_assert (v.लंबाई == 1001);
    क्रम (अचिन्हित i = 0; i < v.लंबाई; i++)
      hb_always_assert (v[i].लंबाई == 3 && v[i][2] == 3);
  }
  {
    hb_vector_t<hb_vector_t<पूर्णांक>> v {hb_vector_t<पूर्णांक> {1, 2, 3}};
    v.push (std::move (v[0]));
    hb_always_assert (v.लंबाई == 2 && v[0].लंबाई == 0);
    hb_always_assert (v[1].लंबाई == 3 && v[1][2] == 3);
  }
  {
    hb_vector_t<std::string> v {"one"};
    v.push (3, v[0][0]);
    hb_always_assert (v.लंबाई == 2);
    hb_always_assert (v[0] == "one" && v[1] == "ooo");
  }
  {
    hb_vector_t<पूर्णांक> v {1, 2, 3};
    v.extend (v.as_array ());
    hb_always_assert (v.लंबाई == 6);
    hb_always_assert (v[3] == 1 && v[4] == 2 && v[5] == 3);
  }
  {
    hb_vector_t<पूर्णांक> v {1, 2, 3};
    v.extend (hb_array_t<स्थिर पूर्णांक> (v.arrayZ + 1, 2));
    hb_always_assert (v.लंबाई == 5);
    hb_always_assert (v[3] == 2 && v[4] == 3);
  }
  {
    hb_vector_t<std::string> v {"one", "two", "three"};
    v.extend (v.as_array ());
    hb_always_assert (v.लंबाई == 6);
    क्रम (अचिन्हित i = 0; i < 3; i++)
      hb_always_assert (v[i] == v[i + 3]);
  }
  {
    hb_vector_t<std::string> v {"one", "two", "three"};
    v.extend (v);
    hb_always_assert (v.लंबाई == 6);
    क्रम (अचिन्हित i = 0; i < 3; i++)
      hb_always_assert (v[i] == v[i + 3]);
  }

  /* A failed vector must not move from an aliased argument. */
  {
    hb_vector_t<std::string> v {"one"};
    v.set_error ();
    hb_always_assert (!v.धकेलो_or_fail (std::move (v[0])));
    hb_always_assert (v.लंबाई == 1 && v[0] == "one");
  }

  /* Test move constructor. */
  {
    hb_vector_t<पूर्णांक> s {1, 2};
    hb_sorted_vector_t<पूर्णांक> S {1, 2};
    hb_vector_t<पूर्णांक> v (std::move (s));
    hb_sorted_vector_t<पूर्णांक> V (std::move (S));
    hb_always_assert (s.लंबाई == 0);
    hb_always_assert (S.लंबाई == 0);
    hb_always_assert (v.लंबाई == 2);
    hb_always_assert (v[0] == 1);
    hb_always_assert (v[1] == 2);
  }

  /* Test move assignment. */
  {
    hb_vector_t<पूर्णांक> s {1, 2};
    hb_sorted_vector_t<पूर्णांक> S {1, 2};
    hb_vector_t<पूर्णांक> v;
    hb_sorted_vector_t<पूर्णांक> V;
    v = std::move (s);
    V = std::move (S);
    hb_always_assert (s.लंबाई == 0);
    hb_always_assert (S.लंबाई == 0);
    hb_always_assert (v.लंबाई == 2);
    hb_always_assert (V.लंबाई == 2);
    hb_always_assert (v[0] == 1);
    hb_always_assert (v[1] == 2);
  }

  /* Test initializing from iterable. */
  {
    hb_set_t s;

    s.add (18);
    s.add (12);

    hb_vector_t<पूर्णांक> v (s);
    hb_sorted_vector_t<पूर्णांक> V (s);

    hb_always_assert (v.लंबाई == 2);
    hb_always_assert (V.लंबाई == 2);
    hb_always_assert (v[0] == 12);
    hb_always_assert (V[0] == 12);
    hb_always_assert (v[1] == 18);
    hb_always_assert (V[1] == 18);
  }

  /* Test initializing from iterator. */
  {
    hb_set_t s;

    s.add (18);
    s.add (12);

    hb_vector_t<पूर्णांक> v (hb_iter (s));
    hb_vector_t<पूर्णांक> V (hb_iter (s));

    hb_always_assert (v.लंबाई == 2);
    hb_always_assert (V.लंबाई == 2);
    hb_always_assert (v[0] == 12);
    hb_always_assert (V[0] == 12);
    hb_always_assert (v[1] == 18);
    hb_always_assert (V[1] == 18);
  }

  /* Test initializing from initializer list and swapping. */
  {
    hb_vector_t<पूर्णांक> v1 {1, 2, 3};
    hb_vector_t<पूर्णांक> v2 {4, 5};
    hb_swap (v1, v2);
    hb_always_assert (v1.लंबाई == 2);
    hb_always_assert (v1[0] == 4);
    hb_always_assert (v2.लंबाई == 3);
    hb_always_assert (v2[2] == 3);
  }

  /* Test initializing sorted-vector from initializer list and swapping. */
  {
    hb_sorted_vector_t<पूर्णांक> v1 {1, 2, 3};
    hb_sorted_vector_t<पूर्णांक> v2 {4, 5};
    hb_swap (v1, v2);
    hb_always_assert (v1.लंबाई == 2);
    hb_always_assert (v1[0] == 4);
    hb_always_assert (v2.लंबाई == 3);
    hb_always_assert (v2[2] == 3);
  }

  {
    hb_vector_t<std::string> v;

    std::string s;
    क्रम (अचिन्हित i = 1; i < 100; i++)
    {
      s += "x";
      v.push (s);
    }

    hb_vector_t<std::string> v2;

    v2 = v;

    v2.remove_ordered (50);
    v2.remove_unordered (50);
  }

  {
    hb_vector_t<hb_set_t> v;
    hb_set_t s {1, 5, 7};
    v.push (s);
    v << s;
    hb_always_assert (s.get_population () == 3);
    v << std::move (s);
    hb_always_assert (s.get_population () == 0);
  }

  {
    hb_vector_t<hb_map_t> v;
    hb_map_t m;
    v.push (m);
  }
  {
    hb_vector_t<hb_map_t> v;
    hb_map_t m;
    v.push (m);
  }

  वापस 0;
}

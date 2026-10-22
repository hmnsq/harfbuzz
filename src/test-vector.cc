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

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-vector.hh"
#అన్తర్భావ "hb-set.hh"
#అన్తర్భావ "hb-map.hh"
#అన్తర్భావ <string>


పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  hb_always_assert (పరిమాణం (hb_సదిశ_t<పూర్ణాంకం>) == పరిమాణం (hb_sorted_సదిశ_t<పూర్ణాంకం>));

  /* Test copy constructor. */
  {
    hb_సదిశ_t<పూర్ణాంకం> v1 {1, 2};
    hb_సదిశ_t<పూర్ణాంకం> v2 {v1};
    hb_సదిశ_t<పూర్ణాంకం> V2 {v1};
    hb_always_assert (v1.నిడివి == 2);
    hb_always_assert (v1[0] == 1);
    hb_always_assert (v1[1] == 2);
    hb_always_assert (v2.నిడివి == 2);
    hb_always_assert (v2[0] == 1);
    hb_always_assert (v2[1] == 2);
  }

  /* Test copy assignment. */
  {
    hb_సదిశ_t<పూర్ణాంకం> v1 {1, 2};
    hb_సదిశ_t<పూర్ణాంకం> v2 = v1;
    hb_సదిశ_t<పూర్ణాంకం> V2 = v1;
    hb_always_assert (v1.నిడివి == 2);
    hb_always_assert (v1[0] == 1);
    hb_always_assert (v1[1] == 2);
    hb_always_assert (v2.నిడివి == 2);
    hb_always_assert (v2[0] == 1);
    hb_always_assert (v2[1] == 2);
  }

  /* Self-copy assignment must preserve both trivial and nontrivial items. */
  {
    hb_సదిశ_t<పూర్ణాంకం> v {1, 2};
    స్థిరరాశి స్వయంచాలక &self = v;
    v = self;
    hb_always_assert (v.నిడివి == 2);
    hb_always_assert (v[0] == 1);
    hb_always_assert (v[1] == 2);
  }
  {
    hb_సదిశ_t<std::string> v;
    v.push (std::string ("one"));
    v.push (std::string ("two"));
    స్థిరరాశి స్వయంచాలక &self = v;
    v = self;
    hb_always_assert (v.నిడివి == 2);
    hb_always_assert (v[0] == "one");
    hb_always_assert (v[1] == "two");
  }

  /* Test pushing an element of the vector itself while it grows. */
  {
    hb_సదిశ_t<పూర్ణాంకం> v {7};
    కొరకు (గుర్తులేని i = 0; i < 1000; i++)
      v.push (v.arrayZ[0]);
    hb_always_assert (v.నిడివి == 1001);
    కొరకు (గుర్తులేని i = 0; i < v.నిడివి; i++)
      hb_always_assert (v[i] == 7);
  }
  {
    hb_సదిశ_t<std::string> v;
    v.push (std::string ("a string longer than the small-string buffer"));
    కొరకు (గుర్తులేని i = 0; i < 1000; i++)
      v.push (v.arrayZ[i]);
    hb_always_assert (v.నిడివి == 1001);
    కొరకు (గుర్తులేని i = 0; i < v.నిడివి; i++)
      hb_always_assert (v[i] == "a string longer than the small-string buffer");
  }
  {
    hb_సదిశ_t<hb_సదిశ_t<పూర్ణాంకం>> v;
    v.push (hb_సదిశ_t<పూర్ణాంకం> {1, 2, 3});
    కొరకు (గుర్తులేని i = 0; i < 1000; i++)
      v.push (v.arrayZ[0]);
    hb_always_assert (v.నిడివి == 1001);
    కొరకు (గుర్తులేని i = 0; i < v.నిడివి; i++)
      hb_always_assert (v[i].నిడివి == 3 && v[i][2] == 3);
  }
  {
    hb_సదిశ_t<hb_సదిశ_t<పూర్ణాంకం>> v {hb_సదిశ_t<పూర్ణాంకం> {1, 2, 3}};
    v.push (std::move (v[0]));
    hb_always_assert (v.నిడివి == 2 && v[0].నిడివి == 0);
    hb_always_assert (v[1].నిడివి == 3 && v[1][2] == 3);
  }
  {
    hb_సదిశ_t<std::string> v {"one"};
    v.push (3, v[0][0]);
    hb_always_assert (v.నిడివి == 2);
    hb_always_assert (v[0] == "one" && v[1] == "ooo");
  }
  {
    hb_సదిశ_t<పూర్ణాంకం> v {1, 2, 3};
    v.extend (v.as_array ());
    hb_always_assert (v.నిడివి == 6);
    hb_always_assert (v[3] == 1 && v[4] == 2 && v[5] == 3);
  }
  {
    hb_సదిశ_t<పూర్ణాంకం> v {1, 2, 3};
    v.extend (hb_array_t<స్థిరరాశి పూర్ణాంకం> (v.arrayZ + 1, 2));
    hb_always_assert (v.నిడివి == 5);
    hb_always_assert (v[3] == 2 && v[4] == 3);
  }
  {
    hb_సదిశ_t<std::string> v {"one", "two", "three"};
    v.extend (v.as_array ());
    hb_always_assert (v.నిడివి == 6);
    కొరకు (గుర్తులేని i = 0; i < 3; i++)
      hb_always_assert (v[i] == v[i + 3]);
  }
  {
    hb_సదిశ_t<std::string> v {"one", "two", "three"};
    v.extend (v);
    hb_always_assert (v.నిడివి == 6);
    కొరకు (గుర్తులేని i = 0; i < 3; i++)
      hb_always_assert (v[i] == v[i + 3]);
  }

  /* A failed vector must not move from an aliased argument. */
  {
    hb_సదిశ_t<std::string> v {"one"};
    v.సమితి_error ();
    hb_always_assert (!v.నెట్టు_or_fail (std::move (v[0])));
    hb_always_assert (v.నిడివి == 1 && v[0] == "one");
  }

  /* Test move constructor. */
  {
    hb_సదిశ_t<పూర్ణాంకం> s {1, 2};
    hb_sorted_సదిశ_t<పూర్ణాంకం> S {1, 2};
    hb_సదిశ_t<పూర్ణాంకం> v (std::move (s));
    hb_sorted_సదిశ_t<పూర్ణాంకం> V (std::move (S));
    hb_always_assert (s.నిడివి == 0);
    hb_always_assert (S.నిడివి == 0);
    hb_always_assert (v.నిడివి == 2);
    hb_always_assert (v[0] == 1);
    hb_always_assert (v[1] == 2);
  }

  /* Test move assignment. */
  {
    hb_సదిశ_t<పూర్ణాంకం> s {1, 2};
    hb_sorted_సదిశ_t<పూర్ణాంకం> S {1, 2};
    hb_సదిశ_t<పూర్ణాంకం> v;
    hb_sorted_సదిశ_t<పూర్ణాంకం> V;
    v = std::move (s);
    V = std::move (S);
    hb_always_assert (s.నిడివి == 0);
    hb_always_assert (S.నిడివి == 0);
    hb_always_assert (v.నిడివి == 2);
    hb_always_assert (V.నిడివి == 2);
    hb_always_assert (v[0] == 1);
    hb_always_assert (v[1] == 2);
  }

  /* Test initializing from iterable. */
  {
    hb_సమితి_t s;

    s.కూడు (18);
    s.కూడు (12);

    hb_సదిశ_t<పూర్ణాంకం> v (s);
    hb_sorted_సదిశ_t<పూర్ణాంకం> V (s);

    hb_always_assert (v.నిడివి == 2);
    hb_always_assert (V.నిడివి == 2);
    hb_always_assert (v[0] == 12);
    hb_always_assert (V[0] == 12);
    hb_always_assert (v[1] == 18);
    hb_always_assert (V[1] == 18);
  }

  /* Test initializing from iterator. */
  {
    hb_సమితి_t s;

    s.కూడు (18);
    s.కూడు (12);

    hb_సదిశ_t<పూర్ణాంకం> v (hb_iter (s));
    hb_సదిశ_t<పూర్ణాంకం> V (hb_iter (s));

    hb_always_assert (v.నిడివి == 2);
    hb_always_assert (V.నిడివి == 2);
    hb_always_assert (v[0] == 12);
    hb_always_assert (V[0] == 12);
    hb_always_assert (v[1] == 18);
    hb_always_assert (V[1] == 18);
  }

  /* Test initializing from initializer list and swapping. */
  {
    hb_సదిశ_t<పూర్ణాంకం> v1 {1, 2, 3};
    hb_సదిశ_t<పూర్ణాంకం> v2 {4, 5};
    hb_మార్పిడి (v1, v2);
    hb_always_assert (v1.నిడివి == 2);
    hb_always_assert (v1[0] == 4);
    hb_always_assert (v2.నిడివి == 3);
    hb_always_assert (v2[2] == 3);
  }

  /* Test initializing sorted-vector from initializer list and swapping. */
  {
    hb_sorted_సదిశ_t<పూర్ణాంకం> v1 {1, 2, 3};
    hb_sorted_సదిశ_t<పూర్ణాంకం> v2 {4, 5};
    hb_మార్పిడి (v1, v2);
    hb_always_assert (v1.నిడివి == 2);
    hb_always_assert (v1[0] == 4);
    hb_always_assert (v2.నిడివి == 3);
    hb_always_assert (v2[2] == 3);
  }

  {
    hb_సదిశ_t<std::string> v;

    std::string s;
    కొరకు (గుర్తులేని i = 1; i < 100; i++)
    {
      s += "x";
      v.push (s);
    }

    hb_సదిశ_t<std::string> v2;

    v2 = v;

    v2.remove_ordered (50);
    v2.remove_unordered (50);
  }

  {
    hb_సదిశ_t<hb_సమితి_t> v;
    hb_సమితి_t s {1, 5, 7};
    v.push (s);
    v << s;
    hb_always_assert (s.get_population () == 3);
    v << std::move (s);
    hb_always_assert (s.get_population () == 0);
  }

  {
    hb_సదిశ_t<hb_map_t> v;
    hb_map_t m;
    v.push (m);
  }
  {
    hb_సదిశ_t<hb_map_t> v;
    hb_map_t m;
    v.push (m);
  }

  తిరిగి 0;
}

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
 */

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-map.hh"
#అన్తర్భావ "hb-set.hh"
#అన్తర్భావ <string>

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{

  /* Test copy constructor. */
  {
    hb_map_t v1;
    v1.సమితి (1, 2);
    hb_map_t v2 {v1};
    hb_always_assert (v1.get_population () == 1);
    hb_always_assert (v2.get_population () == 1);
    hb_always_assert (v1[1] == 2);
    hb_always_assert (v2[1] == 2);
  }

  /* Test copy assignment. */
  {
    hb_map_t v1;
    v1.సమితి (1, 2);
    hb_map_t v2 = v1;
    hb_always_assert (v1.get_population () == 1);
    hb_always_assert (v2.get_population () == 1);
    hb_always_assert (v1[1] == 2);
    hb_always_assert (v2[1] == 2);
  }

  /* Test move constructor. */
  {
    hb_map_t s {};
    s.సమితి (1, 2);
    hb_map_t v (std::move (s));
    hb_always_assert (s.get_population () == 0);
    hb_always_assert (v.get_population () == 1);
  }

  /* Test move assignment. */
  {
    hb_map_t s {};
    s.సమితి (1, 2);
    hb_map_t v;
    v = std::move (s);
    hb_always_assert (s.get_population () == 0);
    hb_always_assert (v.get_population () == 1);
  }

  /* Test initializing from iterable. */
  {
    hb_map_t s;

    s.సమితి (1, 2);
    s.సమితి (3, 4);

    hb_సదిశ_t<hb_codepoint_pair_t> v (s);
    hb_map_t v0 (v);
    hb_map_t v1 (s);
    hb_map_t v2 (std::move (s));

    hb_always_assert (s.get_population () == 0);
    hb_always_assert (v0.get_population () == 2);
    hb_always_assert (v1.get_population () == 2);
    hb_always_assert (v2.get_population () == 2);
  }

  /* Test call fini() twice. */
  {
    hb_map_t s;
    కొరకు (పూర్ణాంకం i = 0; i < 16; i++)
      s.సమితి(i, i+1);
    s.fini();
  }

  /* Test initializing from iterator. */
  {
    hb_map_t s;

    s.సమితి (1, 2);
    s.సమితి (3, 4);

    hb_map_t v (hb_iter (s));

    hb_always_assert (v.get_population () == 2);
  }

  /* Test initializing from initializer list and swapping. */
  {
    ఉపయోగించి pair_t = hb_codepoint_pair_t;
    hb_map_t v1 {pair_t{1,2}, pair_t{4,5}};
    hb_map_t v2 {pair_t{3,4}};
    hb_మార్పిడి (v1, v2);
    hb_always_assert (v1.get_population () == 1);
    hb_always_assert (v2.get_population () == 2);
  }

  /* Test class key / value types. */
  {
    hb_hashmap_t<hb_బైట్లు_t, పూర్ణాంకం> m1;
    hb_hashmap_t<పూర్ణాంకం, hb_బైట్లు_t> m2;
    hb_hashmap_t<hb_బైట్లు_t, hb_బైట్లు_t> m3;
    hb_always_assert (m1.get_population () == 0);
    hb_always_assert (m2.get_population () == 0);
    hb_always_assert (m3.get_population () == 0);
  }

  {
    hb_hashmap_t<పూర్ణాంకం, పూర్ణాంకం> m0;
    hb_hashmap_t<std::string, పూర్ణాంకం> m1;
    hb_hashmap_t<పూర్ణాంకం, std::string> m2;
    hb_hashmap_t<std::string, std::string> m3;

    std::string s;
    కొరకు (గుర్తులేని i = 1; i < 1000; i++)
    {
      s += "x";
      m0.సమితి (i, i);
      m1.సమితి (s, i);
      m2.సమితి (i, s);
      m3.సమితి (s, s);
    }
  }

  /* Test hashing maps. */
  {
    ఉపయోగించి pair = hb_codepoint_pair_t;

    hb_hashmap_t<hb_map_t, hb_map_t> m1;

    m1.సమితి (hb_map_t (), hb_map_t {});
    m1.సమితి (hb_map_t (), hb_map_t {pair (1u, 2u)});
    m1.సమితి (hb_map_t {pair (1u, 2u)}, hb_map_t {pair (2u, 3u)});

    hb_always_assert (m1.get (hb_map_t ()) == hb_map_t {pair (1u, 2u)});
    hb_always_assert (m1.get (hb_map_t {pair (1u, 2u)}) == hb_map_t {pair (2u, 3u)});
  }

  /* Test hashing sets. */
  {
    hb_hashmap_t<hb_సమితి_t, hb_సమితి_t> m1;

    m1.సమితి (hb_సమితి_t (), hb_సమితి_t ());
    m1.సమితి (hb_సమితి_t (), hb_సమితి_t {1});
    m1.సమితి (hb_సమితి_t {1, 1000}, hb_సమితి_t {2});

    hb_always_assert (m1.get (hb_సమితి_t ()) == hb_సమితి_t {1});
    hb_always_assert (m1.get (hb_సమితి_t {1000, 1}) == hb_సమితి_t {2});
  }

  /* Test hashing vectors. */
  {
    ఉపయోగించి సదిశ_t = hb_సదిశ_t<గుర్తులేని>;

    hb_hashmap_t<సదిశ_t, సదిశ_t> m1;

    m1.సమితి (సదిశ_t (), సదిశ_t {1});
    m1.సమితి (సదిశ_t {1}, సదిశ_t {2});

    m1 << hb_pair_t<సదిశ_t, సదిశ_t> {సదిశ_t {2}, సదిశ_t ()};

    hb_always_assert (m1.get (సదిశ_t ()) == సదిశ_t {1});
    hb_always_assert (m1.get (సదిశ_t {1}) == సదిశ_t {2});
  }

  /* Test moving values */
  {
    ఉపయోగించి సదిశ_t = hb_సదిశ_t<గుర్తులేని>;

    hb_hashmap_t<సదిశ_t, సదిశ_t> m1;
    సదిశ_t v {3};
    hb_always_assert (v.నిడివి == 1);
    m1 << hb_pair_t<సదిశ_t, సదిశ_t> {సదిశ_t {3}, v};
    hb_always_assert (v.నిడివి == 1);
    m1 << hb_pair_t<సదిశ_t, సదిశ_t&&> {సదిశ_t {4}, std::move (v)};
    hb_always_assert (v.నిడివి == 0);
    m1 << hb_pair_t<సదిశ_t&&, సదిశ_t> {సదిశ_t {4}, సదిశ_t {5}};
    m1 << hb_pair_t<సదిశ_t&&, సదిశ_t&&> {సదిశ_t {4}, సదిశ_t {5}};

    hb_hashmap_t<సదిశ_t, సదిశ_t> m2;
    సదిశ_t v2 {3};
    m2.సమితి (సదిశ_t {4}, v2);
    hb_always_assert (v2.నిడివి == 1);
    m2.సమితి (సదిశ_t {5}, std::move (v2));
    hb_always_assert (v2.నిడివి == 0);
  }

  /* Test hb::shared_ptr. */
  {
    hb_hashmap_t<hb::shared_ptr<hb_సమితి_t>, hb::shared_ptr<hb_సమితి_t>> m;

    m.సమితి (hb::shared_ptr<hb_సమితి_t> (hb_సమితి_get_ఖాళీలు ()),
	   hb::shared_ptr<hb_సమితి_t> (hb_సమితి_get_ఖాళీలు ()));
    m.get (hb::shared_ptr<hb_సమితి_t> (hb_సమితి_get_ఖాళీలు ()));
    m.iter ();
    m.కీలకాలు ();
    m.values ();
    m.iter_ref ();
    m.కీలకాలు_ref ();
    m.విలువలు_ref ();
  }
  /* Test hb::unique_ptr. */
  {
    hb_hashmap_t<hb::unique_ptr<hb_సమితి_t>, hb::unique_ptr<hb_సమితి_t>> m;

    m.సమితి (hb::unique_ptr<hb_సమితి_t> (hb_సమితి_get_ఖాళీలు ()),
           hb::unique_ptr<hb_సమితి_t> (hb_సమితి_get_ఖాళీలు ()));
    m.get (hb::unique_ptr<hb_సమితి_t> (hb_సమితి_get_ఖాళీలు ()));
    hb::unique_ptr<hb_సమితి_t> *v;
    m.has (hb::unique_ptr<hb_సమితి_t> (hb_సమితి_get_ఖాళీలు ()), &v);
    m.iter_ref ();
    m.కీలకాలు_ref ();
    m.విలువలు_ref ();
  }
  /* Test hashmap with complex shared_ptrs as keys. */
  {
    hb_hashmap_t<hb::shared_ptr<hb_map_t>, గుర్తులేని> m;

    hb_map_t *m1 = hb_map_create ();
    hb_map_t *m2 = hb_map_create ();
    m1->సమితి (1,3);
    m2->సమితి (1,3);

    hb::shared_ptr<hb_map_t> p1 {m1};
    hb::shared_ptr<hb_map_t> p2 {m2};
    m.సమితి (p1,1);

    hb_always_assert (m.has (p2));

    m1->సమితి (2,4);
    hb_always_assert (!m.has (p2));
  }
  /* Test value type with hb_bytes_t. */
  {
    hb_hashmap_t<పూర్ణాంకం, hb_బైట్లు_t> m;
    అక్షరం c_str[] = "Test";
    hb_బైట్లు_t బైట్లు (c_str);

    m.సమితి (1, బైట్లు);
    hb_always_assert (m.has (1));
    hb_always_assert (m.get (1) == hb_బైట్లు_t {"Test"});
  }
  /* Test operators. */
  {
    hb_map_t m1, m2, m3;
    m1.సమితి (1, 2);
    m1.సమితి (2, 4);
    m2.సమితి (1, 2);
    m2.సమితి (2, 4);
    m3.సమితి (1, 3);
    m3.సమితి (3, 5);

    hb_always_assert (m1 == m2);
    hb_always_assert (m1 != m3);
    hb_always_assert (!(m2 == m3));

    m2 = m3;
    hb_always_assert (m2.has (1));
    hb_always_assert (!m2.has (2));
    hb_always_assert (m2.has (3));

    hb_always_assert (m3.has (3));
  }
  /* Test reset. */
  {
    hb_hashmap_t<పూర్ణాంకం, hb_సమితి_t> m;
    m.సమితి (1, hb_సమితి_t {1, 2, 3});
    m.reset ();
  }
  /* Test iteration. */
  {
    hb_map_t m;
    m.సమితి (1, 1);
    m.సమితి (4, 3);
    m.సమితి (5, 5);
    m.సమితి (2, 1);
    m.సమితి (3, 2);
    m.సమితి (6, 8);

    hb_codepoint_t k;
    hb_codepoint_t v;
    గుర్తులేని తీయి = 0;
    కొరకు (గుర్తుగల i = -1;
	 m.next (&i, &k, &v);)
    {
      తీయి++;
           ఒకవేళ (k == 1) hb_always_assert (v == 1);
      లేకపోతే ఒకవేళ (k == 2) hb_always_assert (v == 1);
      లేకపోతే ఒకవేళ (k == 3) hb_always_assert (v == 2);
      లేకపోతే ఒకవేళ (k == 4) hb_always_assert (v == 3);
      లేకపోతే ఒకవేళ (k == 5) hb_always_assert (v == 5);
      లేకపోతే ఒకవేళ (k == 6) hb_always_assert (v == 8);
      లేకపోతే hb_always_assert (అబద్ధం);
    }
    hb_always_assert (తీయి == m.get_population ());
  }
  /* Test update */
  {
    hb_map_t m1, m2;
    m1.సమితి (1, 2);
    m1.సమితి (2, 4);
    m2.సమితి (1, 3);

    m1.update (m2);
    hb_always_assert (m1.get_population () == 2);
    hb_always_assert (m1[1] == 3);
    hb_always_assert (m1[2] == 4);
  }
  /* Test keys / values */
  {
    hb_map_t m;
    m.సమితి (1, 1);
    m.సమితి (4, 3);
    m.సమితి (5, 5);
    m.సమితి (2, 1);
    m.సమితి (3, 2);
    m.సమితి (6, 8);

    hb_సమితి_t కీలకాలు;
    hb_సమితి_t values;

    hb_copy (m.కీలకాలు (), కీలకాలు);
    hb_copy (m.values (), values);

    hb_always_assert (కీలకాలు.is_equal (hb_సమితి_t ({1, 2, 3, 4, 5, 6})));
    hb_always_assert (values.is_equal (hb_సమితి_t ({1, 1, 2, 3, 5, 8})));

    hb_always_assert (కీలకాలు.is_equal (hb_సమితి_t (m.కీలకాలు ())));
    hb_always_assert (values.is_equal (hb_సమితి_t (m.values ())));
  }
  /* Test setting a key or value that lives in the map's own storage
   * while the map grows. */
  {
    hb_map_t m;
    m.సమితి (1, 2);
    కొరకు (గుర్తులేని i = 0; i < 1000; i++)
    {
      m.సమితి (1000 + i, m.get (1));
      hb_always_assert (m.get (1000 + i) == 2);
    }
    hb_map_t m2;
    m2.సమితి (1, 5000);
    కొరకు (గుర్తులేని i = 0; i < 1000; i++)
    {
      m2.సమితి (m2.get (1), i);
      m2.సమితి (1, 5001 + i);
      hb_always_assert (m2.get (5000 + i) == i);
    }
    hb_hashmap_t<గుర్తులేని, hb_సదిశ_t<గుర్తులేని>> v;
    v.సమితి (1, hb_సదిశ_t<గుర్తులేని> {1, 2, 3});
    కొరకు (గుర్తులేని i = 0; i < 1000; i++)
    {
      v.సమితి (1000 + i, v.get (1));
      hb_always_assert (v.get (1000 + i).నిడివి == 3);
    }
  }
  /* Test allocation bounds and overflow protection. */
  {
    hb_map_t m1;
    hb_always_assert (!m1.alloc ((గుర్తులేని) -1));
    hb_always_assert (m1.లోపల_error ());

    hb_map_t m2;
    hb_always_assert (!m2.alloc (0x40000000u));
    hb_always_assert (m2.లోపల_error ());

    /* Test copy of map in error state. */
    hb_map_t m3 (m1);
    hb_always_assert (m3.లోపల_error ());
  }

  తిరిగి 0;
}

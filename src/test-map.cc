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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-map.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-set.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ <string>

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{

  /* Test copy constructor. */
  {
    hb_map_t v1;
    v1.ਸੈੱਟ (1, 2);
    hb_map_t v2 {v1};
    hb_always_assert (v1.get_population () == 1);
    hb_always_assert (v2.get_population () == 1);
    hb_always_assert (v1[1] == 2);
    hb_always_assert (v2[1] == 2);
  }

  /* Test copy assignment. */
  {
    hb_map_t v1;
    v1.ਸੈੱਟ (1, 2);
    hb_map_t v2 = v1;
    hb_always_assert (v1.get_population () == 1);
    hb_always_assert (v2.get_population () == 1);
    hb_always_assert (v1[1] == 2);
    hb_always_assert (v2[1] == 2);
  }

  /* Test move constructor. */
  {
    hb_map_t s {};
    s.ਸੈੱਟ (1, 2);
    hb_map_t v (std::move (s));
    hb_always_assert (s.get_population () == 0);
    hb_always_assert (v.get_population () == 1);
  }

  /* Test move assignment. */
  {
    hb_map_t s {};
    s.ਸੈੱਟ (1, 2);
    hb_map_t v;
    v = std::move (s);
    hb_always_assert (s.get_population () == 0);
    hb_always_assert (v.get_population () == 1);
  }

  /* Test initializing from iterable. */
  {
    hb_map_t s;

    s.ਸੈੱਟ (1, 2);
    s.ਸੈੱਟ (3, 4);

    hb_ਸਦਿਸ਼_t<hb_codepoint_pair_t> v (s);
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
    ਲਈ (ਪੂਰਨ i = 0; i < 16; i++)
      s.ਸੈੱਟ(i, i+1);
    s.fini();
  }

  /* Test initializing from iterator. */
  {
    hb_map_t s;

    s.ਸੈੱਟ (1, 2);
    s.ਸੈੱਟ (3, 4);

    hb_map_t v (hb_iter (s));

    hb_always_assert (v.get_population () == 2);
  }

  /* Test initializing from initializer list and swapping. */
  {
    ਵਰਤੋਂ pair_t = hb_codepoint_pair_t;
    hb_map_t v1 {pair_t{1,2}, pair_t{4,5}};
    hb_map_t v2 {pair_t{3,4}};
    hb_ਅਦਲਾ_ਬਦਲੀ (v1, v2);
    hb_always_assert (v1.get_population () == 1);
    hb_always_assert (v2.get_population () == 2);
  }

  /* Test class key / value types. */
  {
    hb_hashmap_t<hb_ਬਾਈਟਾਂ_t, ਪੂਰਨ> m1;
    hb_hashmap_t<ਪੂਰਨ, hb_ਬਾਈਟਾਂ_t> m2;
    hb_hashmap_t<hb_ਬਾਈਟਾਂ_t, hb_ਬਾਈਟਾਂ_t> m3;
    hb_always_assert (m1.get_population () == 0);
    hb_always_assert (m2.get_population () == 0);
    hb_always_assert (m3.get_population () == 0);
  }

  {
    hb_hashmap_t<ਪੂਰਨ, ਪੂਰਨ> m0;
    hb_hashmap_t<std::string, ਪੂਰਨ> m1;
    hb_hashmap_t<ਪੂਰਨ, std::string> m2;
    hb_hashmap_t<std::string, std::string> m3;

    std::string s;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 1; i < 1000; i++)
    {
      s += "x";
      m0.ਸੈੱਟ (i, i);
      m1.ਸੈੱਟ (s, i);
      m2.ਸੈੱਟ (i, s);
      m3.ਸੈੱਟ (s, s);
    }
  }

  /* Test hashing maps. */
  {
    ਵਰਤੋਂ pair = hb_codepoint_pair_t;

    hb_hashmap_t<hb_map_t, hb_map_t> m1;

    m1.ਸੈੱਟ (hb_map_t (), hb_map_t {});
    m1.ਸੈੱਟ (hb_map_t (), hb_map_t {pair (1u, 2u)});
    m1.ਸੈੱਟ (hb_map_t {pair (1u, 2u)}, hb_map_t {pair (2u, 3u)});

    hb_always_assert (m1.get (hb_map_t ()) == hb_map_t {pair (1u, 2u)});
    hb_always_assert (m1.get (hb_map_t {pair (1u, 2u)}) == hb_map_t {pair (2u, 3u)});
  }

  /* Test hashing sets. */
  {
    hb_hashmap_t<hb_ਸੈੱਟ_t, hb_ਸੈੱਟ_t> m1;

    m1.ਸੈੱਟ (hb_ਸੈੱਟ_t (), hb_ਸੈੱਟ_t ());
    m1.ਸੈੱਟ (hb_ਸੈੱਟ_t (), hb_ਸੈੱਟ_t {1});
    m1.ਸੈੱਟ (hb_ਸੈੱਟ_t {1, 1000}, hb_ਸੈੱਟ_t {2});

    hb_always_assert (m1.get (hb_ਸੈੱਟ_t ()) == hb_ਸੈੱਟ_t {1});
    hb_always_assert (m1.get (hb_ਸੈੱਟ_t {1000, 1}) == hb_ਸੈੱਟ_t {2});
  }

  /* Test hashing vectors. */
  {
    ਵਰਤੋਂ ਸਦਿਸ਼_t = hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ>;

    hb_hashmap_t<ਸਦਿਸ਼_t, ਸਦਿਸ਼_t> m1;

    m1.ਸੈੱਟ (ਸਦਿਸ਼_t (), ਸਦਿਸ਼_t {1});
    m1.ਸੈੱਟ (ਸਦਿਸ਼_t {1}, ਸਦਿਸ਼_t {2});

    m1 << hb_pair_t<ਸਦਿਸ਼_t, ਸਦਿਸ਼_t> {ਸਦਿਸ਼_t {2}, ਸਦਿਸ਼_t ()};

    hb_always_assert (m1.get (ਸਦਿਸ਼_t ()) == ਸਦਿਸ਼_t {1});
    hb_always_assert (m1.get (ਸਦਿਸ਼_t {1}) == ਸਦਿਸ਼_t {2});
  }

  /* Test moving values */
  {
    ਵਰਤੋਂ ਸਦਿਸ਼_t = hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ>;

    hb_hashmap_t<ਸਦਿਸ਼_t, ਸਦਿਸ਼_t> m1;
    ਸਦਿਸ਼_t v {3};
    hb_always_assert (v.ਲੰਮਾਈ == 1);
    m1 << hb_pair_t<ਸਦਿਸ਼_t, ਸਦਿਸ਼_t> {ਸਦਿਸ਼_t {3}, v};
    hb_always_assert (v.ਲੰਮਾਈ == 1);
    m1 << hb_pair_t<ਸਦਿਸ਼_t, ਸਦਿਸ਼_t&&> {ਸਦਿਸ਼_t {4}, std::move (v)};
    hb_always_assert (v.ਲੰਮਾਈ == 0);
    m1 << hb_pair_t<ਸਦਿਸ਼_t&&, ਸਦਿਸ਼_t> {ਸਦਿਸ਼_t {4}, ਸਦਿਸ਼_t {5}};
    m1 << hb_pair_t<ਸਦਿਸ਼_t&&, ਸਦਿਸ਼_t&&> {ਸਦਿਸ਼_t {4}, ਸਦਿਸ਼_t {5}};

    hb_hashmap_t<ਸਦਿਸ਼_t, ਸਦਿਸ਼_t> m2;
    ਸਦਿਸ਼_t v2 {3};
    m2.ਸੈੱਟ (ਸਦਿਸ਼_t {4}, v2);
    hb_always_assert (v2.ਲੰਮਾਈ == 1);
    m2.ਸੈੱਟ (ਸਦਿਸ਼_t {5}, std::move (v2));
    hb_always_assert (v2.ਲੰਮਾਈ == 0);
  }

  /* Test hb::shared_ptr. */
  {
    hb_hashmap_t<hb::shared_ptr<hb_ਸੈੱਟ_t>, hb::shared_ptr<hb_ਸੈੱਟ_t>> m;

    m.ਸੈੱਟ (hb::shared_ptr<hb_ਸੈੱਟ_t> (hb_ਸੈੱਟ_get_ਖਾਲੀ_ਥਾਵਾਂ ()),
	   hb::shared_ptr<hb_ਸੈੱਟ_t> (hb_ਸੈੱਟ_get_ਖਾਲੀ_ਥਾਵਾਂ ()));
    m.get (hb::shared_ptr<hb_ਸੈੱਟ_t> (hb_ਸੈੱਟ_get_ਖਾਲੀ_ਥਾਵਾਂ ()));
    m.iter ();
    m.ਕੁੰਜੀਆਂ ();
    m.values ();
    m.iter_ref ();
    m.ਕੁੰਜੀਆਂ_ref ();
    m.ਮੁੱਲਾਂ_ref ();
  }
  /* Test hb::unique_ptr. */
  {
    hb_hashmap_t<hb::unique_ptr<hb_ਸੈੱਟ_t>, hb::unique_ptr<hb_ਸੈੱਟ_t>> m;

    m.ਸੈੱਟ (hb::unique_ptr<hb_ਸੈੱਟ_t> (hb_ਸੈੱਟ_get_ਖਾਲੀ_ਥਾਵਾਂ ()),
           hb::unique_ptr<hb_ਸੈੱਟ_t> (hb_ਸੈੱਟ_get_ਖਾਲੀ_ਥਾਵਾਂ ()));
    m.get (hb::unique_ptr<hb_ਸੈੱਟ_t> (hb_ਸੈੱਟ_get_ਖਾਲੀ_ਥਾਵਾਂ ()));
    hb::unique_ptr<hb_ਸੈੱਟ_t> *v;
    m.has (hb::unique_ptr<hb_ਸੈੱਟ_t> (hb_ਸੈੱਟ_get_ਖਾਲੀ_ਥਾਵਾਂ ()), &v);
    m.iter_ref ();
    m.ਕੁੰਜੀਆਂ_ref ();
    m.ਮੁੱਲਾਂ_ref ();
  }
  /* Test hashmap with complex shared_ptrs as keys. */
  {
    hb_hashmap_t<hb::shared_ptr<hb_map_t>, ਬਿਨਾਨਿਸ਼ਾਨੀ> m;

    hb_map_t *m1 = hb_map_create ();
    hb_map_t *m2 = hb_map_create ();
    m1->ਸੈੱਟ (1,3);
    m2->ਸੈੱਟ (1,3);

    hb::shared_ptr<hb_map_t> p1 {m1};
    hb::shared_ptr<hb_map_t> p2 {m2};
    m.ਸੈੱਟ (p1,1);

    hb_always_assert (m.has (p2));

    m1->ਸੈੱਟ (2,4);
    hb_always_assert (!m.has (p2));
  }
  /* Test value type with hb_bytes_t. */
  {
    hb_hashmap_t<ਪੂਰਨ, hb_ਬਾਈਟਾਂ_t> m;
    ਅੱਖਰ c_str[] = "Test";
    hb_ਬਾਈਟਾਂ_t ਬਾਈਟਾਂ (c_str);

    m.ਸੈੱਟ (1, ਬਾਈਟਾਂ);
    hb_always_assert (m.has (1));
    hb_always_assert (m.get (1) == hb_ਬਾਈਟਾਂ_t {"Test"});
  }
  /* Test operators. */
  {
    hb_map_t m1, m2, m3;
    m1.ਸੈੱਟ (1, 2);
    m1.ਸੈੱਟ (2, 4);
    m2.ਸੈੱਟ (1, 2);
    m2.ਸੈੱਟ (2, 4);
    m3.ਸੈੱਟ (1, 3);
    m3.ਸੈੱਟ (3, 5);

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
    hb_hashmap_t<ਪੂਰਨ, hb_ਸੈੱਟ_t> m;
    m.ਸੈੱਟ (1, hb_ਸੈੱਟ_t {1, 2, 3});
    m.reset ();
  }
  /* Test iteration. */
  {
    hb_map_t m;
    m.ਸੈੱਟ (1, 1);
    m.ਸੈੱਟ (4, 3);
    m.ਸੈੱਟ (5, 5);
    m.ਸੈੱਟ (2, 1);
    m.ਸੈੱਟ (3, 2);
    m.ਸੈੱਟ (6, 8);

    hb_codepoint_t k;
    hb_codepoint_t v;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੱਢੋ = 0;
    ਲਈ (ਨਿਸ਼ਾਨੀਵਾਲਾ i = -1;
	 m.next (&i, &k, &v);)
    {
      ਕੱਢੋ++;
           ਜੇ (k == 1) hb_always_assert (v == 1);
      ਨਹੀਂਤਾਂ ਜੇ (k == 2) hb_always_assert (v == 1);
      ਨਹੀਂਤਾਂ ਜੇ (k == 3) hb_always_assert (v == 2);
      ਨਹੀਂਤਾਂ ਜੇ (k == 4) hb_always_assert (v == 3);
      ਨਹੀਂਤਾਂ ਜੇ (k == 5) hb_always_assert (v == 5);
      ਨਹੀਂਤਾਂ ਜੇ (k == 6) hb_always_assert (v == 8);
      ਨਹੀਂਤਾਂ hb_always_assert (ਝੂਠ);
    }
    hb_always_assert (ਕੱਢੋ == m.get_population ());
  }
  /* Test update */
  {
    hb_map_t m1, m2;
    m1.ਸੈੱਟ (1, 2);
    m1.ਸੈੱਟ (2, 4);
    m2.ਸੈੱਟ (1, 3);

    m1.update (m2);
    hb_always_assert (m1.get_population () == 2);
    hb_always_assert (m1[1] == 3);
    hb_always_assert (m1[2] == 4);
  }
  /* Test keys / values */
  {
    hb_map_t m;
    m.ਸੈੱਟ (1, 1);
    m.ਸੈੱਟ (4, 3);
    m.ਸੈੱਟ (5, 5);
    m.ਸੈੱਟ (2, 1);
    m.ਸੈੱਟ (3, 2);
    m.ਸੈੱਟ (6, 8);

    hb_ਸੈੱਟ_t ਕੁੰਜੀਆਂ;
    hb_ਸੈੱਟ_t values;

    hb_copy (m.ਕੁੰਜੀਆਂ (), ਕੁੰਜੀਆਂ);
    hb_copy (m.values (), values);

    hb_always_assert (ਕੁੰਜੀਆਂ.is_equal (hb_ਸੈੱਟ_t ({1, 2, 3, 4, 5, 6})));
    hb_always_assert (values.is_equal (hb_ਸੈੱਟ_t ({1, 1, 2, 3, 5, 8})));

    hb_always_assert (ਕੁੰਜੀਆਂ.is_equal (hb_ਸੈੱਟ_t (m.ਕੁੰਜੀਆਂ ())));
    hb_always_assert (values.is_equal (hb_ਸੈੱਟ_t (m.values ())));
  }
  /* Test setting a key or value that lives in the map's own storage
   * while the map grows. */
  {
    hb_map_t m;
    m.ਸੈੱਟ (1, 2);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < 1000; i++)
    {
      m.ਸੈੱਟ (1000 + i, m.get (1));
      hb_always_assert (m.get (1000 + i) == 2);
    }
    hb_map_t m2;
    m2.ਸੈੱਟ (1, 5000);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < 1000; i++)
    {
      m2.ਸੈੱਟ (m2.get (1), i);
      m2.ਸੈੱਟ (1, 5001 + i);
      hb_always_assert (m2.get (5000 + i) == i);
    }
    hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ>> v;
    v.ਸੈੱਟ (1, hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> {1, 2, 3});
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < 1000; i++)
    {
      v.ਸੈੱਟ (1000 + i, v.get (1));
      hb_always_assert (v.get (1000 + i).ਲੰਮਾਈ == 3);
    }
  }
  /* Test allocation bounds and overflow protection. */
  {
    hb_map_t m1;
    hb_always_assert (!m1.alloc ((ਬਿਨਾਨਿਸ਼ਾਨੀ) -1));
    hb_always_assert (m1.ਵਿੱਚ_error ());

    hb_map_t m2;
    hb_always_assert (!m2.alloc (0x40000000u));
    hb_always_assert (m2.ਵਿੱਚ_error ());

    /* Test copy of map in error state. */
    hb_map_t m3 (m1);
    hb_always_assert (m3.ਵਿੱਚ_error ());
  }

  ਵਾਪਸ 0;
}

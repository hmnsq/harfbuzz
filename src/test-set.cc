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
#అన్తర్భావ "hb-set.hh"

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{

  /* Test copy constructor. */
  {
    hb_సమితి_t v1 {1, 2};
    hb_సమితి_t v2 {v1};
    hb_always_assert (v1.get_population () == 2);
    hb_always_assert (hb_len (hb_iter (v1)) == 2);
    hb_always_assert (v2.get_population () == 2);
  }

  /* Test copy assignment. */
  {
    hb_సమితి_t v1 {1, 2};
    hb_సమితి_t v2;
    v2 = v1;
    hb_always_assert (v1.get_population () == 2);
    hb_always_assert (v2.get_population () == 2);
  }

  /* Test move constructor. */
  {
    hb_సమితి_t s {1, 2};
    hb_సమితి_t v (std::move (s));
    hb_always_assert (s.get_population () == 0);
    hb_always_assert (hb_len (hb_iter (s)) == 0);
    hb_always_assert (v.get_population () == 2);
  }

  /* Test move assignment. */
  {
    hb_సమితి_t s = hb_సమితి_t {1, 2};
    hb_సమితి_t v;
    v = std::move (s);
    hb_always_assert (s.get_population () == 0);
    hb_always_assert (v.get_population () == 2);
  }

  /* Test initializing from iterable. */
  {
    hb_సమితి_t s;

    s.కూడు (18);
    s.కూడు (12);

    hb_సదిశ_t<hb_codepoint_t> v (s);
    hb_సమితి_t v0 (v);
    hb_సమితి_t v1 (s);
    hb_సమితి_t v2 (std::move (s));

    hb_always_assert (s.get_population () == 0);
    hb_always_assert (v0.get_population () == 2);
    hb_always_assert (v1.get_population () == 2);
    hb_always_assert (v2.get_population () == 2);
  }

  /* Test initializing from iterator. */
  {
    hb_సమితి_t s;

    s.కూడు (18);
    s << 12;

    /* Sink a range. */
    s << hb_codepoint_pair_t {1, 3};

    hb_సమితి_t v (hb_iter (s));

    hb_always_assert (v.get_population () == 5);
  }

  /* Test initializing from initializer list and swapping. */
  {
    hb_సమితి_t v1 {1, 2, 3};
    hb_సమితి_t v2 {4, 5};
    hb_మార్పిడి (v1, v2);
    hb_always_assert (v1.get_population () == 2);
    hb_always_assert (v2.get_population () == 3);
  }

  /* Test inverted sets. */
  {
    hb_సమితి_t s;
    s.invert();
    s.del (5);

    hb_codepoint_t ప్రారంభం = HB_సమితి_విలువ_INVALID, చివరి = HB_సమితి_విలువ_INVALID;
    hb_always_assert (s.తదుపరి_range (&ప్రారంభం, &చివరి));
    hb_always_assert (ప్రారంభం == 0);
    hb_always_assert (చివరి == 4);
    hb_always_assert (s.తదుపరి_range (&ప్రారంభం, &చివరి));
    hb_always_assert (ప్రారంభం == 6);
    hb_always_assert (చివరి == HB_సమితి_విలువ_INVALID - 1);
    hb_always_assert (!s.తదుపరి_range (&ప్రారంభం, &చివరి));

    ప్రారంభం = HB_సమితి_విలువ_INVALID;
    చివరి = HB_సమితి_విలువ_INVALID;
    hb_always_assert (s.మునుపటి_range (&ప్రారంభం, &చివరి));
    hb_always_assert (ప్రారంభం == 6);
    hb_always_assert (చివరి == HB_సమితి_విలువ_INVALID - 1);
    hb_always_assert (s.మునుపటి_range (&ప్రారంభం, &చివరి));
    hb_always_assert (ప్రారంభం == 0);
    hb_always_assert (చివరి == 4);
    hb_always_assert (!s.మునుపటి_range (&ప్రారంభం, &చివరి));

    hb_always_assert (s.is_inverted ());
    /* Inverted set returns true for invalid value; oh well. */
    hb_always_assert (s.has (HB_సమితి_విలువ_INVALID));
  }

  /* Test set intersection. */
  {
    hb_సమితి_t a {1, 2};
    hb_సమితి_t b {2, 3};
    hb_సమితి_t c {3, 4};

    hb_always_assert (a.intersects (b));
    hb_always_assert (!a.intersects (c));

    b.invert ();
    hb_always_assert (a.intersects (b));
    hb_always_assert (b.intersects (a));

    hb_సమితి_t singleton {2};
    hb_always_assert (!singleton.intersects (b));
    hb_always_assert (!b.intersects (singleton));

    c.invert ();
    hb_always_assert (b.intersects (c));
  }

  /* Test singleton detection. */
  {
    hb_సమితి_t s;
    hb_codepoint_t singleton;
    hb_always_assert (!s.get_singleton (&singleton));

    s.కూడు (63);
    hb_always_assert (s.get_singleton (&singleton));
    hb_always_assert (singleton == 63);

    s.కూడు (1024);
    hb_always_assert (!s.get_singleton (&singleton));
    s.del (63);
    hb_always_assert (s.get_singleton (&singleton));
    hb_always_assert (singleton == 1024);

    s.invert ();
    hb_always_assert (!s.get_singleton (&singleton));
  }

  /* Test word-at-a-time insertion. */
  {
    hb_సమితి_t s;
    s.కూడు_బిట్లు (64, uint64_t (1) | (uint64_t (1) << 63));
    hb_always_assert (s.get_population () == 2);
    hb_always_assert (s.has (64));
    hb_always_assert (s.has (127));

    s.invert ();
    s.కూడు_బిట్లు (64, uint64_t (1) | (uint64_t (1) << 63));
    hb_always_assert (s.has (64));
    hb_always_assert (s.has (127));
  }

  /* Adding HB_SET_VALUE_INVALID */
  {
    hb_సమితి_t s;

    s.కూడు(HB_సమితి_విలువ_INVALID);
    hb_always_assert(!s.has(HB_సమితి_విలువ_INVALID));

    s.clear();
    hb_always_assert(!s.కూడు_range(HB_సమితి_విలువ_INVALID - 2, HB_సమితి_విలువ_INVALID));
    hb_always_assert(!s.has(HB_సమితి_విలువ_INVALID));

    hb_codepoint_t array[] = {(గుర్తులేని) HB_సమితి_విలువ_INVALID, 0, 2};
    s.clear();
    s.కూడు_array(array, 3);
    hb_always_assert(!s.has(HB_సమితి_విలువ_INVALID));
    hb_always_assert(s.has(2));

    hb_codepoint_t sorted_array[] = {0, 2, (గుర్తులేని) HB_సమితి_విలువ_INVALID};
    s.clear();
    s.కూడు_sorted_array(sorted_array, 3);
    hb_always_assert(!s.has(HB_సమితి_విలువ_INVALID));
    hb_always_assert(s.has(2));
  }

  /* Test iteration across words and pages. */
  {
    hb_సమితి_t s {0, 1, 63, 64, 65, 127, 511, 512, 513, 1024,
		HB_సమితి_విలువ_INVALID - 1};
    hb_codepoint_t expected[] = {0, 1, 63, 64, 65, 127, 511, 512, 513,
				 1024, HB_సమితి_విలువ_INVALID - 1};
    గుర్తులేని పూర్ణాంకం i = 0;
    కొరకు (hb_codepoint_t v : s)
    {
      hb_always_assert (i < ARRAY_నిడివి (expected));
      hb_always_assert (v == expected[i++]);
    }
    hb_always_assert (i == ARRAY_నిడివి (expected));

    స్వయంచాలక it = s.iter ();
    hb_always_assert (*it == 0);
    hb_always_assert (it.len () == ARRAY_నిడివి (expected));
    ++it;
    hb_always_assert (*it == 1);
    hb_always_assert (it.len () == ARRAY_నిడివి (expected) - 1);
    --it;
    hb_always_assert (*it == 0);
    ++it;
    hb_always_assert (*it == 1);
    it += 3;
    hb_always_assert (*it == 65);
    --it;
    hb_always_assert (*it == 64);
    --it;
    hb_always_assert (*it == 63);
    ++it;
    hb_always_assert (*it == 64);

    it = it.చివర ();
    --it;
    hb_always_assert (*it == HB_సమితి_విలువ_INVALID - 1);
    --it;
    hb_always_assert (*it == 1024);
    ++it;
    hb_always_assert (*it == HB_సమితి_విలువ_INVALID - 1);

    it = s.iter ();
    it += 2;
    hb_always_assert (it.len () == ARRAY_నిడివి (expected) - 2);
  }

  /* Test inverted-set iteration. */
  {
    hb_సమితి_t s {0, 1, 63, 64};
    s.invert ();
    స్వయంచాలక it = s.iter ();
    hb_always_assert (*it == 2);
    hb_always_assert (it.len () == HB_సమితి_విలువ_INVALID - 4);
    ++it;
    hb_always_assert (*it == 3);
    --it;
    hb_always_assert (*it == 2);

    it = it.చివర ();
    --it;
    hb_always_assert (*it == HB_సమితి_విలువ_INVALID - 1);
    --it;
    hb_always_assert (*it == HB_సమితి_విలువ_INVALID - 2);
  }

  తిరిగి 0;
}

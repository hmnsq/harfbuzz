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

#ضمن "hb.hh"
#ضمن "hb-set.hh"

صحيح
اصلی (صحيح argc, حرف **argv)
{

  /* Test copy constructor. */
  {
    hb_مجموعه_t v1 {1, 2};
    hb_مجموعه_t v2 {v1};
    hb_always_assert (v1.get_population () == 2);
    hb_always_assert (hb_len (hb_iter (v1)) == 2);
    hb_always_assert (v2.get_population () == 2);
  }

  /* Test copy assignment. */
  {
    hb_مجموعه_t v1 {1, 2};
    hb_مجموعه_t v2;
    v2 = v1;
    hb_always_assert (v1.get_population () == 2);
    hb_always_assert (v2.get_population () == 2);
  }

  /* Test move constructor. */
  {
    hb_مجموعه_t s {1, 2};
    hb_مجموعه_t v (std::move (s));
    hb_always_assert (s.get_population () == 0);
    hb_always_assert (hb_len (hb_iter (s)) == 0);
    hb_always_assert (v.get_population () == 2);
  }

  /* Test move assignment. */
  {
    hb_مجموعه_t s = hb_مجموعه_t {1, 2};
    hb_مجموعه_t v;
    v = std::move (s);
    hb_always_assert (s.get_population () == 0);
    hb_always_assert (v.get_population () == 2);
  }

  /* Test initializing from iterable. */
  {
    hb_مجموعه_t s;

    s.جمع_کن (18);
    s.جمع_کن (12);

    hb_وکتور_t<hb_codepoint_t> v (s);
    hb_مجموعه_t v0 (v);
    hb_مجموعه_t v1 (s);
    hb_مجموعه_t v2 (std::move (s));

    hb_always_assert (s.get_population () == 0);
    hb_always_assert (v0.get_population () == 2);
    hb_always_assert (v1.get_population () == 2);
    hb_always_assert (v2.get_population () == 2);
  }

  /* Test initializing from iterator. */
  {
    hb_مجموعه_t s;

    s.جمع_کن (18);
    s << 12;

    /* Sink a range. */
    s << hb_codepoint_pair_t {1, 3};

    hb_مجموعه_t v (hb_iter (s));

    hb_always_assert (v.get_population () == 5);
  }

  /* Test initializing from initializer list and swapping. */
  {
    hb_مجموعه_t v1 {1, 2, 3};
    hb_مجموعه_t v2 {4, 5};
    hb_جابجا (v1, v2);
    hb_always_assert (v1.get_population () == 2);
    hb_always_assert (v2.get_population () == 3);
  }

  /* Test inverted sets. */
  {
    hb_مجموعه_t s;
    s.invert();
    s.del (5);

    hb_codepoint_t شروع = HB_مجموعه_مقدار_INVALID, آخرین = HB_مجموعه_مقدار_INVALID;
    hb_always_assert (s.بعدی_range (&شروع, &آخرین));
    hb_always_assert (شروع == 0);
    hb_always_assert (آخرین == 4);
    hb_always_assert (s.بعدی_range (&شروع, &آخرین));
    hb_always_assert (شروع == 6);
    hb_always_assert (آخرین == HB_مجموعه_مقدار_INVALID - 1);
    hb_always_assert (!s.بعدی_range (&شروع, &آخرین));

    شروع = HB_مجموعه_مقدار_INVALID;
    آخرین = HB_مجموعه_مقدار_INVALID;
    hb_always_assert (s.قبلی_range (&شروع, &آخرین));
    hb_always_assert (شروع == 6);
    hb_always_assert (آخرین == HB_مجموعه_مقدار_INVALID - 1);
    hb_always_assert (s.قبلی_range (&شروع, &آخرین));
    hb_always_assert (شروع == 0);
    hb_always_assert (آخرین == 4);
    hb_always_assert (!s.قبلی_range (&شروع, &آخرین));

    hb_always_assert (s.is_inverted ());
    /* Inverted set returns true for invalid value; oh well. */
    hb_always_assert (s.has (HB_مجموعه_مقدار_INVALID));
  }

  /* Test set intersection. */
  {
    hb_مجموعه_t a {1, 2};
    hb_مجموعه_t b {2, 3};
    hb_مجموعه_t c {3, 4};

    hb_always_assert (a.intersects (b));
    hb_always_assert (!a.intersects (c));

    b.invert ();
    hb_always_assert (a.intersects (b));
    hb_always_assert (b.intersects (a));

    hb_مجموعه_t singleton {2};
    hb_always_assert (!singleton.intersects (b));
    hb_always_assert (!b.intersects (singleton));

    c.invert ();
    hb_always_assert (b.intersects (c));
  }

  /* Test singleton detection. */
  {
    hb_مجموعه_t s;
    hb_codepoint_t singleton;
    hb_always_assert (!s.get_singleton (&singleton));

    s.جمع_کن (63);
    hb_always_assert (s.get_singleton (&singleton));
    hb_always_assert (singleton == 63);

    s.جمع_کن (1024);
    hb_always_assert (!s.get_singleton (&singleton));
    s.del (63);
    hb_always_assert (s.get_singleton (&singleton));
    hb_always_assert (singleton == 1024);

    s.invert ();
    hb_always_assert (!s.get_singleton (&singleton));
  }

  /* Test word-at-a-time insertion. */
  {
    hb_مجموعه_t s;
    s.جمع_کن_بیتها (64, uint64_t (1) | (uint64_t (1) << 63));
    hb_always_assert (s.get_population () == 2);
    hb_always_assert (s.has (64));
    hb_always_assert (s.has (127));

    s.invert ();
    s.جمع_کن_بیتها (64, uint64_t (1) | (uint64_t (1) << 63));
    hb_always_assert (s.has (64));
    hb_always_assert (s.has (127));
  }

  /* Adding HB_SET_VALUE_INVALID */
  {
    hb_مجموعه_t s;

    s.جمع_کن(HB_مجموعه_مقدار_INVALID);
    hb_always_assert(!s.has(HB_مجموعه_مقدار_INVALID));

    s.clear();
    hb_always_assert(!s.جمع_کن_range(HB_مجموعه_مقدار_INVALID - 2, HB_مجموعه_مقدار_INVALID));
    hb_always_assert(!s.has(HB_مجموعه_مقدار_INVALID));

    hb_codepoint_t array[] = {(بیعلامت) HB_مجموعه_مقدار_INVALID, 0, 2};
    s.clear();
    s.جمع_کن_array(array, 3);
    hb_always_assert(!s.has(HB_مجموعه_مقدار_INVALID));
    hb_always_assert(s.has(2));

    hb_codepoint_t sorted_array[] = {0, 2, (بیعلامت) HB_مجموعه_مقدار_INVALID};
    s.clear();
    s.جمع_کن_sorted_array(sorted_array, 3);
    hb_always_assert(!s.has(HB_مجموعه_مقدار_INVALID));
    hb_always_assert(s.has(2));
  }

  /* Test iteration across words and pages. */
  {
    hb_مجموعه_t s {0, 1, 63, 64, 65, 127, 511, 512, 513, 1024,
		HB_مجموعه_مقدار_INVALID - 1};
    hb_codepoint_t expected[] = {0, 1, 63, 64, 65, 127, 511, 512, 513,
				 1024, HB_مجموعه_مقدار_INVALID - 1};
    بیعلامت صحيح i = 0;
    لكل (hb_codepoint_t v : s)
    {
      hb_always_assert (i < ARRAY_درازی (expected));
      hb_always_assert (v == expected[i++]);
    }
    hb_always_assert (i == ARRAY_درازی (expected));

    خودکار it = s.iter ();
    hb_always_assert (*it == 0);
    hb_always_assert (it.len () == ARRAY_درازی (expected));
    ++it;
    hb_always_assert (*it == 1);
    hb_always_assert (it.len () == ARRAY_درازی (expected) - 1);
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

    it = it.انتها ();
    --it;
    hb_always_assert (*it == HB_مجموعه_مقدار_INVALID - 1);
    --it;
    hb_always_assert (*it == 1024);
    ++it;
    hb_always_assert (*it == HB_مجموعه_مقدار_INVALID - 1);

    it = s.iter ();
    it += 2;
    hb_always_assert (it.len () == ARRAY_درازی (expected) - 2);
  }

  /* Test inverted-set iteration. */
  {
    hb_مجموعه_t s {0, 1, 63, 64};
    s.invert ();
    خودکار it = s.iter ();
    hb_always_assert (*it == 2);
    hb_always_assert (it.len () == HB_مجموعه_مقدار_INVALID - 4);
    ++it;
    hb_always_assert (*it == 3);
    --it;
    hb_always_assert (*it == 2);

    it = it.انتها ();
    --it;
    hb_always_assert (*it == HB_مجموعه_مقدار_INVALID - 1);
    --it;
    hb_always_assert (*it == HB_مجموعه_مقدار_INVALID - 2);
  }

  أرجع 0;
}

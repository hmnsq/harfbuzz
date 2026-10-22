/*
 * Copyright © 2018  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-iter.hh"

#অন্তর্ভাব "hb-array.hh"
#অন্তর্ভাব "hb-set.hh"
#অন্তর্ভাব "hb-ot-layout-common.hh"

template <typename T>
কাঠামো array_iter_t : hb_iter_with_fallback_t<array_iter_t<T>, T&>
{
  array_iter_t (hb_array_t<T> arr_) : arr (arr_) {}

  প্রকারলেখ T& __item_t__;
  স্থির constexpr যৌক্তিক is_random_access_iterator = সত্য;
  T& __item_at__ (unsigned i) ধ্রুবক { ফেরত arr[i]; }
  শূন্য __forward__ (unsigned n) { arr += n; }
  শূন্য __rewind__ (unsigned n) { arr -= n; }
  unsigned __len__ () ধ্রুবক { ফেরত arr.দৈর্ঘ্য; }
  যৌক্তিক operator != (ধ্রুবক array_iter_t& o) { ফেরত arr != o.arr; }

  ব্যক্তিগত:
  hb_array_t<T> arr;
};

template <typename T>
কাঠামো some_array_t
{
  some_array_t (hb_array_t<T> arr_) : arr (arr_) {}

  প্রকারলেখ array_iter_t<T> iter_t;
  array_iter_t<T> iter () { ফেরত array_iter_t<T> (arr); }
  operator array_iter_t<T> () { ফেরত iter (); }
  operator hb_iter_t<array_iter_t<T>> () { ফেরত iter (); }

  ব্যক্তিগত:
  hb_array_t<T> arr;
};


template <typename Iter,
	  hb_requires (hb_is_iterator (Iter))>
স্থির শূন্য
test_iterator_non_default_constructable (Iter it)
{
  /* Iterate over a copy of it. */
  জন্য (auto c = it.iter (); c; c++)
    *c;

  /* Same. */
  জন্য (auto c = +it; c; c++)
    *c;

  /* Range-based for over a copy. */
  জন্য (auto _ : +it)
    (শূন্য) _;

  it += it.len ();
  it = it + 10;
  it = 10 + it;

  hb_always_assert (*it == it[0]);

  static_assert (সত্য || it.is_random_access_iterator, "");
  static_assert (সত্য || it.is_sorted_iterator, "");
}

template <typename Iter,
	  hb_requires (hb_is_iterator (Iter))>
স্থির শূন্য
test_iterator (Iter it)
{
  Iter default_constructed;
  hb_always_assert (!default_constructed);

  test_iterator_non_default_constructable (it);
}

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
স্থির শূন্য
test_iterable (ধ্রুবক Iterable &lst = Null (Iterable))
{
  জন্য (auto _ : lst)
    (শূন্য) _;

  // Test that can take iterator from.
  test_iterator (lst.iter ());
}

template <typename It>
স্থির শূন্য check_sequential (It it)
{
  পূর্ণসংখ্যা i = 1;
  জন্য (পূর্ণসংখ্যা v : +it) {
    hb_always_assert (v == i++);
  }
}

স্থির শূন্য test_concat ()
{
  hb_vector_t<পূর্ণসংখ্যা> a = {1, 2, 3};
  hb_vector_t<পূর্ণসংখ্যা> b = {4, 5};

  hb_vector_t<পূর্ণসংখ্যা> c = {};
  hb_vector_t<পূর্ণসংখ্যা> d = {1, 2, 3, 4, 5};

  auto it1 = hb_concat (a, b);
  hb_always_assert (it1.len () == 5);
  hb_always_assert (it1.is_random_access_iterator);
  auto it2 = hb_concat (c, d);
  hb_always_assert (it2.len () == 5);
  auto it3 = hb_concat (d, c);
  hb_always_assert (it3.len () == 5);
  জন্য (পূর্ণসংখ্যা i = 0; i < 5; i++) {
    hb_always_assert(it1[i] == i + 1);
    hb_always_assert(it2[i] == i + 1);
    hb_always_assert(it3[i] == i + 1);
  }

  check_sequential (it1);
  check_sequential (it2);
  check_sequential (it3);

  auto it4 = +it1;
  it4 += 0;
  hb_always_assert (*it4 == 1);

  it4 += 2;
  hb_always_assert (*it4 == 3);
  hb_always_assert (it4);
  hb_always_assert (it4.len () == 3);

  it4 += 2;
  hb_always_assert (*it4 == 5);
  hb_always_assert (it4);
  hb_always_assert (it4.len () == 1);

  it4++;
  hb_always_assert (!it4);
  hb_always_assert (it4.len () == 0);

  auto it5 = +it1;
  it5 += 3;
  hb_always_assert (*it5 == 4);

  hb_set_t s_a = {1, 2, 3};
  hb_set_t s_b = {4, 5};
  auto it6 = hb_concat (s_a, s_b);
  hb_always_assert (!it6.is_random_access_iterator);
  check_sequential (it6);
  hb_always_assert (it6.len () == 5);

  it6 += 0;
  hb_always_assert (*it6 == 1);

  it6 += 3;
  hb_always_assert (*it6 == 4);
  hb_always_assert (it6);
  hb_always_assert (it6.len () == 2);
}

স্থির শূন্য
test_backward_filter ()
{
  hb_vector_t<পূর্ণসংখ্যা> V = {2, 4, 5, 7, 8, 9, 10};
  auto it = hb_iter (V) | hb_filter ([] (পূর্ণসংখ্যা x) { ফেরত x % 2 != 0; });
  it += 2;
  hb_always_assert (*it == 9);
  --it;
  hb_always_assert (*it == 7);
  --it;
  hb_always_assert (*it == 5);
  --it;
  hb_always_assert (*it == 5);
}

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  ধ্রুবক পূর্ণসংখ্যা src[10] = {};
  পূর্ণসংখ্যা dst[20];
  hb_vector_t<পূর্ণসংখ্যা> v;

  array_iter_t<ধ্রুবক পূর্ণসংখ্যা> s (src); /* Implicit conversion from static array. */
  array_iter_t<ধ্রুবক পূর্ণসংখ্যা> s2 (v); /* Implicit conversion from vector. */
  array_iter_t<পূর্ণসংখ্যা> t (dst);

  static_assert (array_iter_t<পূর্ণসংখ্যা>::is_random_access_iterator, "");

  some_array_t<ধ্রুবক পূর্ণসংখ্যা> a (src);

  s2 = s;

  hb_iter (src);
  hb_iter (src, 2);

  hb_fill (t, 42);
  hb_copy (s, t);
  hb_copy (a.iter (), t);

  test_iterable (v);
  hb_set_t st;
  st << 1 << 15 << 43;
  test_iterable (st);
  hb_sorted_array_t<পূর্ণসংখ্যা> sa;
  (শূন্য) static_cast<hb_iter_t<hb_sorted_array_t<পূর্ণসংখ্যা>, hb_sorted_array_t<পূর্ণসংখ্যা>::item_t>&> (sa);
  (শূন্য) static_cast<hb_iter_t<hb_sorted_array_t<পূর্ণসংখ্যা>, hb_sorted_array_t<পূর্ণসংখ্যা>::__item_t__>&> (sa);
  (শূন্য) static_cast<hb_iter_t<hb_sorted_array_t<পূর্ণসংখ্যা>, পূর্ণসংখ্যা&>&>(sa);
  (শূন্য) static_cast<hb_iter_t<hb_sorted_array_t<পূর্ণসংখ্যা>>&>(sa);
  (শূন্য) static_cast<hb_iter_t<hb_array_t<পূর্ণসংখ্যা>, পূর্ণসংখ্যা&>&> (sa);
  test_iterable (sa);

  test_iterable<hb_array_t<পূর্ণসংখ্যা>> ();
  test_iterable<hb_sorted_array_t<ধ্রুবক পূর্ণসংখ্যা>> ();
  test_iterable<hb_vector_t<দশমিক>> ();
  test_iterable<hb_set_t> ();
  test_iterable<OT::Array16Of<OT::HBUINT16>> ();

  test_iterator (hb_zip (st, v));
  test_iterator_non_default_constructable (hb_enumerate (st));
  test_iterator_non_default_constructable (hb_enumerate (st, -5));
  test_iterator_non_default_constructable (hb_enumerate (hb_iter (st)));
  test_iterator_non_default_constructable (hb_enumerate (hb_iter (st) + 1));
  test_iterator_non_default_constructable (hb_iter (st) | hb_filter ());
  test_iterator_non_default_constructable (hb_iter (st) | hb_map (hb_lidentity));

  hb_always_assert (সত্য == hb_all (st));
  hb_always_assert (মিথ্যা == hb_all (st, 42u));
  hb_always_assert (সত্য == hb_any (st));
  hb_always_assert (মিথ্যা == hb_any (st, 14u));
  hb_always_assert (সত্য == hb_any (st, 14u, [] (unsigned _) { ফেরত _ - 1u; }));
  hb_always_assert (সত্য == hb_any (st, [] (unsigned _) { ফেরত _ == 15u; }));
  hb_always_assert (সত্য == hb_any (st, 15u));
  hb_always_assert (মিথ্যা == hb_none (st));
  hb_always_assert (মিথ্যা == hb_none (st, 15u));
  hb_always_assert (সত্য == hb_none (st, 17u));

  hb_array_t<hb_vector_t<পূর্ণসংখ্যা>> pa;
  pa->as_array ();

  hb_map_t m;

  hb_iter (st);
  hb_iter (&st);

  + hb_iter (src)
  | hb_map (m)
  | hb_map (&m)
  | hb_filter ()
  | hb_filter (st)
  | hb_filter (&st)
  | hb_filter (hb_bool)
  | hb_filter (hb_bool, hb_identity)
  | hb_sink (st)
  ;

  + hb_iter (src)
  | hb_sink (hb_array (dst))
  ;

  + hb_iter (src)
  | hb_apply (&st)
  ;

  + hb_iter (src)
  | hb_map ([] (পূর্ণসংখ্যা i) { ফেরত 1; })
  | hb_reduce ([=] (পূর্ণসংখ্যা acc, পূর্ণসংখ্যা value) { ফেরত acc; }, 2)
  ;

  using map_pair_t = hb_item_type<hb_map_t>;
  + hb_iter (m)
  | hb_map ([] (map_pair_t p) { ফেরত p.first * p.second; })
  ;

  m.চাবিগুলি ();
  using map_চাবি_t = decltype (*m.চাবিগুলি());
  + hb_iter (m.চাবিগুলি ())
  | hb_filter ([] (map_চাবি_t k) { ফেরত k < 42; })
  | hb_drain
  ;

  m.values ();
  using map_মান_t = decltype (*m.values());
  + hb_iter (m.values ())
  | hb_filter ([] (map_মান_t k) { ফেরত k < 42; })
  | hb_drain
  ;

  unsigned পূর্ণসংখ্যা temp1 = 10;
  unsigned পূর্ণসংখ্যা temp2 = 0;
  hb_map_t *ফলাফল =
  + hb_iter (src)
  | hb_map ([&] (পূর্ণসংখ্যা i) -> hb_set_t *
	    {
	      hb_set_t *set = hb_set_create ();
	      জন্য (unsigned পূর্ণসংখ্যা i = 0; i < temp1; ++i)
		hb_set_add (set, i);
	      temp1++;
	      ফেরত set;
	    })
  | hb_reduce ([&] (hb_map_t *acc, hb_set_t *value) -> hb_map_t *
	       {
		 hb_map_set (acc, temp2++, hb_set_get_population (value));
		 /* This is not a memory managed language, take care! */
		 hb_set_destroy (value);
		 ফেরত acc;
	       }, hb_map_create ())
  ;
  /* The result should be something like 0->10, 1->11, ..., 9->19 */
  hb_always_assert (hb_map_get (ফলাফল, 9) == 19);
  hb_map_destroy (ফলাফল);

  /* Like above, but passing hb_set_t instead of hb_set_t * */
  temp1 = 10;
  temp2 = 0;
  ফলাফল =
  + hb_iter (src)
  | hb_map ([&] (পূর্ণসংখ্যা i) -> hb_set_t
	    {
	      hb_set_t set;
	      জন্য (unsigned পূর্ণসংখ্যা i = 0; i < temp1; ++i)
		hb_set_add (&set, i);
	      temp1++;
	      ফেরত set;
	    })
  | hb_reduce ([&] (hb_map_t *acc, hb_set_t value) -> hb_map_t *
	       {
		 hb_map_set (acc, temp2++, hb_set_get_population (&value));
		 ফেরত acc;
	       }, hb_map_create ())
  ;
  /* The result should be something like 0->10, 1->11, ..., 9->19 */
  hb_always_assert (hb_map_get (ফলাফল, 9) == 19);
  hb_map_destroy (ফলাফল);

  unsigned পূর্ণসংখ্যা temp3 = 0;
  + hb_iter(src)
  | hb_map([&] (পূর্ণসংখ্যা i) { ফেরত ++temp3; })
  | hb_reduce([&] (দশমিক acc, পূর্ণসংখ্যা value) { ফেরত acc + value; }, 0)
  ;

  + hb_iter (src)
  | hb_drain
  ;

  t << 1;
  long vl;
  s >> vl;

  hb_iota ();
  hb_iota (3);
  hb_iota (3, 2);
  hb_always_assert ((&vl) + 1 == *++hb_iota (&vl, hb_inc));
  hb_range ();
  hb_repeat (7u);
  hb_repeat (nullptr);
  hb_repeat (vl) | hb_chop (3);
  hb_always_assert (hb_len (hb_range (10) | hb_take (3)) == 3);
  hb_always_assert (hb_range (9).len () == 9);
  hb_always_assert (hb_range (2, 9).len () == 7);
  hb_always_assert (hb_range (2, 9, 3).len () == 3);
  hb_always_assert (hb_range (2, 8, 3).len () == 2);
  hb_always_assert (hb_range (2, 7, 3).len () == 2);
  hb_always_assert (hb_range (-2, -9, -3).len () == 3);
  hb_always_assert (hb_range (-2, -8, -3).len () == 2);
  hb_always_assert (hb_range (-2, -7, -3).len () == 2);

  test_concat ();
  test_backward_filter ();

  ফেরত 0;
}

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

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-iter.hh"

#ଅନ୍ତର୍ଭାଵ "hb-array.hh"
#ଅନ୍ତର୍ଭାଵ "hb-set.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-layout-common.hh"

template <typename T>
ସଂରଚନା array_iter_t : hb_iter_with_fallback_t<array_iter_t<T>, T&>
{
  array_iter_t (hb_array_t<T> arr_) : arr (arr_) {}

  ପ୍ରକାରବ୍ୟାଖ୍ୟା T& __item_t__;
  ସ୍ଥିର constexpr ତାର୍କିକ is_random_access_iterator = ସତ୍ୟ;
  T& __item_at__ (unsigned i) ଅଚଳ { ଫେରନ୍ତୁ arr[i]; }
  ଶୂନ୍ୟ __forward__ (unsigned n) { arr += n; }
  ଶୂନ୍ୟ __rewind__ (unsigned n) { arr -= n; }
  unsigned __len__ () ଅଚଳ { ଫେରନ୍ତୁ arr.length; }
  ତାର୍କିକ operator != (ଅଚଳ array_iter_t& o) { ଫେରନ୍ତୁ arr != o.arr; }

  ବ୍ୟକ୍ତିଗତ:
  hb_array_t<T> arr;
};

template <typename T>
ସଂରଚନା some_array_t
{
  some_array_t (hb_array_t<T> arr_) : arr (arr_) {}

  ପ୍ରକାରବ୍ୟାଖ୍ୟା array_iter_t<T> iter_t;
  array_iter_t<T> iter () { ଫେରନ୍ତୁ array_iter_t<T> (arr); }
  operator array_iter_t<T> () { ଫେରନ୍ତୁ iter (); }
  operator hb_iter_t<array_iter_t<T>> () { ଫେରନ୍ତୁ iter (); }

  ବ୍ୟକ୍ତିଗତ:
  hb_array_t<T> arr;
};


template <typename Iter,
	  hb_requires (hb_is_iterator (Iter))>
ସ୍ଥିର ଶୂନ୍ୟ
test_iterator_non_default_constructable (Iter it)
{
  /* Iterate over a copy of it. */
  ପାଇଁ (auto c = it.iter (); c; c++)
    *c;

  /* Same. */
  ପାଇଁ (auto c = +it; c; c++)
    *c;

  /* Range-based for over a copy. */
  ପାଇଁ (auto _ : +it)
    (ଶୂନ୍ୟ) _;

  it += it.len ();
  it = it + 10;
  it = 10 + it;

  hb_always_assert (*it == it[0]);

  static_assert (ସତ୍ୟ || it.is_random_access_iterator, "");
  static_assert (ସତ୍ୟ || it.is_sorted_iterator, "");
}

template <typename Iter,
	  hb_requires (hb_is_iterator (Iter))>
ସ୍ଥିର ଶୂନ୍ୟ
test_iterator (Iter it)
{
  Iter default_constructed;
  hb_always_assert (!default_constructed);

  test_iterator_non_default_constructable (it);
}

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ସ୍ଥିର ଶୂନ୍ୟ
test_iterable (ଅଚଳ Iterable &lst = Null (Iterable))
{
  ପାଇଁ (auto _ : lst)
    (ଶୂନ୍ୟ) _;

  // Test that can take iterator from.
  test_iterator (lst.iter ());
}

template <typename It>
ସ୍ଥିର ଶୂନ୍ୟ check_sequential (It it)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1;
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା v : +it) {
    hb_always_assert (v == i++);
  }
}

ସ୍ଥିର ଶୂନ୍ୟ test_concat ()
{
  hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> a = {1, 2, 3};
  hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> b = {4, 5};

  hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> c = {};
  hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> d = {1, 2, 3, 4, 5};

  auto it1 = hb_concat (a, b);
  hb_always_assert (it1.len () == 5);
  hb_always_assert (it1.is_random_access_iterator);
  auto it2 = hb_concat (c, d);
  hb_always_assert (it2.len () == 5);
  auto it3 = hb_concat (d, c);
  hb_always_assert (it3.len () == 5);
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < 5; i++) {
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

ସ୍ଥିର ଶୂନ୍ୟ
test_backward_filter ()
{
  hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> V = {2, 4, 5, 7, 8, 9, 10};
  auto it = hb_iter (V) | hb_filter ([] (ପୂର୍ଣ୍ଣସଂଖ୍ୟା x) { ଫେରନ୍ତୁ x % 2 != 0; });
  it += 2;
  hb_always_assert (*it == 9);
  --it;
  hb_always_assert (*it == 7);
  --it;
  hb_always_assert (*it == 5);
  --it;
  hb_always_assert (*it == 5);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା src[10] = {};
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା dst[20];
  hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> v;

  array_iter_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> s (src); /* Implicit conversion from static array. */
  array_iter_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> s2 (v); /* Implicit conversion from vector. */
  array_iter_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> t (dst);

  static_assert (array_iter_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>::is_random_access_iterator, "");

  some_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> a (src);

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
  hb_sorted_array_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> sa;
  (ଶୂନ୍ୟ) static_cast<hb_iter_t<hb_sorted_array_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, hb_sorted_array_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>::item_t>&> (sa);
  (ଶୂନ୍ୟ) static_cast<hb_iter_t<hb_sorted_array_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, hb_sorted_array_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>::__item_t__>&> (sa);
  (ଶୂନ୍ୟ) static_cast<hb_iter_t<hb_sorted_array_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, ପୂର୍ଣ୍ଣସଂଖ୍ୟା&>&>(sa);
  (ଶୂନ୍ୟ) static_cast<hb_iter_t<hb_sorted_array_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>>&>(sa);
  (ଶୂନ୍ୟ) static_cast<hb_iter_t<hb_array_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, ପୂର୍ଣ୍ଣସଂଖ୍ୟା&>&> (sa);
  test_iterable (sa);

  test_iterable<hb_array_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> ();
  test_iterable<hb_sorted_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> ();
  test_iterable<hb_vector_t<ଦଶମିକ>> ();
  test_iterable<hb_set_t> ();
  test_iterable<OT::Array16Of<OT::HBUINT16>> ();

  test_iterator (hb_zip (st, v));
  test_iterator_non_default_constructable (hb_enumerate (st));
  test_iterator_non_default_constructable (hb_enumerate (st, -5));
  test_iterator_non_default_constructable (hb_enumerate (hb_iter (st)));
  test_iterator_non_default_constructable (hb_enumerate (hb_iter (st) + 1));
  test_iterator_non_default_constructable (hb_iter (st) | hb_filter ());
  test_iterator_non_default_constructable (hb_iter (st) | hb_map (hb_lidentity));

  hb_always_assert (ସତ୍ୟ == hb_all (st));
  hb_always_assert (ମିଥ୍ୟା == hb_all (st, 42u));
  hb_always_assert (ସତ୍ୟ == hb_any (st));
  hb_always_assert (ମିଥ୍ୟା == hb_any (st, 14u));
  hb_always_assert (ସତ୍ୟ == hb_any (st, 14u, [] (unsigned _) { ଫେରନ୍ତୁ _ - 1u; }));
  hb_always_assert (ସତ୍ୟ == hb_any (st, [] (unsigned _) { ଫେରନ୍ତୁ _ == 15u; }));
  hb_always_assert (ସତ୍ୟ == hb_any (st, 15u));
  hb_always_assert (ମିଥ୍ୟା == hb_none (st));
  hb_always_assert (ମିଥ୍ୟା == hb_none (st, 15u));
  hb_always_assert (ସତ୍ୟ == hb_none (st, 17u));

  hb_array_t<hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> pa;
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
  | hb_map ([] (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) { ଫେରନ୍ତୁ 1; })
  | hb_reduce ([=] (ପୂର୍ଣ୍ଣସଂଖ୍ୟା acc, ପୂର୍ଣ୍ଣସଂଖ୍ୟା value) { ଫେରନ୍ତୁ acc; }, 2)
  ;

  using map_pair_t = hb_item_type<hb_map_t>;
  + hb_iter (m)
  | hb_map ([] (map_pair_t p) { ଫେରନ୍ତୁ p.first * p.second; })
  ;

  m.ଚାବିଗୁଡ଼ିକ ();
  using map_ଚାବି_t = decltype (*m.ଚାବିଗୁଡ଼ିକ());
  + hb_iter (m.ଚାବିଗୁଡ଼ିକ ())
  | hb_filter ([] (map_ଚାବି_t k) { ଫେରନ୍ତୁ k < 42; })
  | hb_drain
  ;

  m.values ();
  using map_ମୂଲ୍ୟ_t = decltype (*m.values());
  + hb_iter (m.values ())
  | hb_filter ([] (map_ମୂଲ୍ୟ_t k) { ଫେରନ୍ତୁ k < 42; })
  | hb_drain
  ;

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା temp1 = 10;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା temp2 = 0;
  hb_map_t *result =
  + hb_iter (src)
  | hb_map ([&] (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) -> hb_set_t *
	    {
	      hb_set_t *set = hb_set_create ();
	      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < temp1; ++i)
		hb_set_add (set, i);
	      temp1++;
	      ଫେରନ୍ତୁ set;
	    })
  | hb_reduce ([&] (hb_map_t *acc, hb_set_t *value) -> hb_map_t *
	       {
		 hb_map_set (acc, temp2++, hb_set_get_population (value));
		 /* This is not a memory managed language, take care! */
		 hb_set_destroy (value);
		 ଫେରନ୍ତୁ acc;
	       }, hb_map_create ())
  ;
  /* The result should be something like 0->10, 1->11, ..., 9->19 */
  hb_always_assert (hb_map_get (result, 9) == 19);
  hb_map_destroy (result);

  /* Like above, but passing hb_set_t instead of hb_set_t * */
  temp1 = 10;
  temp2 = 0;
  result =
  + hb_iter (src)
  | hb_map ([&] (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) -> hb_set_t
	    {
	      hb_set_t set;
	      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < temp1; ++i)
		hb_set_add (&set, i);
	      temp1++;
	      ଫେରନ୍ତୁ set;
	    })
  | hb_reduce ([&] (hb_map_t *acc, hb_set_t value) -> hb_map_t *
	       {
		 hb_map_set (acc, temp2++, hb_set_get_population (&value));
		 ଫେରନ୍ତୁ acc;
	       }, hb_map_create ())
  ;
  /* The result should be something like 0->10, 1->11, ..., 9->19 */
  hb_always_assert (hb_map_get (result, 9) == 19);
  hb_map_destroy (result);

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା temp3 = 0;
  + hb_iter(src)
  | hb_map([&] (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) { ଫେରନ୍ତୁ ++temp3; })
  | hb_reduce([&] (ଦଶମିକ acc, ପୂର୍ଣ୍ଣସଂଖ୍ୟା value) { ଫେରନ୍ତୁ acc + value; }, 0)
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

  ଫେରନ୍ତୁ 0;
}

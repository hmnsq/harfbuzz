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

#ضمن "hb.hh"
#ضمن "hb-iter.hh"

#ضمن "hb-array.hh"
#ضمن "hb-set.hh"
#ضمن "hb-ot-layout-common.hh"

template <typename T>
بنية array_iter_t : hb_iter_with_fallback_t<array_iter_t<T>, T&>
{
  array_iter_t (hb_array_t<T> arr_) : arr (arr_) {}

  تعريفنوع T& __item_t__;
  ساكن constexpr منطقي is_random_access_iterator = אמת;
  T& __item_at__ (unsigned i) ثابت { החזר arr[i]; }
  فراغ __forward__ (unsigned n) { arr += n; }
  فراغ __rewind__ (unsigned n) { arr -= n; }
  unsigned __len__ () ثابت { החזר arr.length; }
  منطقي operator != (ثابت array_iter_t& o) { החזר arr != o.arr; }

  خاص:
  hb_array_t<T> arr;
};

template <typename T>
بنية some_array_t
{
  some_array_t (hb_array_t<T> arr_) : arr (arr_) {}

  تعريفنوع array_iter_t<T> iter_t;
  array_iter_t<T> iter () { החזר array_iter_t<T> (arr); }
  operator array_iter_t<T> () { החזר iter (); }
  operator hb_iter_t<array_iter_t<T>> () { החזר iter (); }

  خاص:
  hb_array_t<T> arr;
};


template <typename Iter,
	  hb_requires (hb_is_iterator (Iter))>
ساكن فراغ
test_iterator_non_default_constructable (Iter it)
{
  /* Iterate over a copy of it. */
  עבור (auto c = it.iter (); c; c++)
    *c;

  /* Same. */
  עבור (auto c = +it; c; c++)
    *c;

  /* Range-based for over a copy. */
  עבור (auto _ : +it)
    (فراغ) _;

  it += it.len ();
  it = it + 10;
  it = 10 + it;

  hb_always_assert (*it == it[0]);

  static_assert (אמת || it.is_random_access_iterator, "");
  static_assert (אמת || it.is_sorted_iterator, "");
}

template <typename Iter,
	  hb_requires (hb_is_iterator (Iter))>
ساكن فراغ
test_iterator (Iter it)
{
  Iter default_constructed;
  hb_always_assert (!default_constructed);

  test_iterator_non_default_constructable (it);
}

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ساكن فراغ
test_iterable (ثابت Iterable &lst = Null (Iterable))
{
  עבור (auto _ : lst)
    (فراغ) _;

  // Test that can take iterator from.
  test_iterator (lst.iter ());
}

template <typename It>
ساكن فراغ check_sequential (It it)
{
  صحيح i = 1;
  עבור (صحيح v : +it) {
    hb_always_assert (v == i++);
  }
}

ساكن فراغ test_concat ()
{
  hb_vector_t<صحيح> a = {1, 2, 3};
  hb_vector_t<صحيح> b = {4, 5};

  hb_vector_t<صحيح> c = {};
  hb_vector_t<صحيح> d = {1, 2, 3, 4, 5};

  auto it1 = hb_concat (a, b);
  hb_always_assert (it1.len () == 5);
  hb_always_assert (it1.is_random_access_iterator);
  auto it2 = hb_concat (c, d);
  hb_always_assert (it2.len () == 5);
  auto it3 = hb_concat (d, c);
  hb_always_assert (it3.len () == 5);
  עבור (صحيح i = 0; i < 5; i++) {
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

ساكن فراغ
test_backward_filter ()
{
  hb_vector_t<صحيح> V = {2, 4, 5, 7, 8, 9, 10};
  auto it = hb_iter (V) | hb_filter ([] (صحيح x) { החזר x % 2 != 0; });
  it += 2;
  hb_always_assert (*it == 9);
  --it;
  hb_always_assert (*it == 7);
  --it;
  hb_always_assert (*it == 5);
  --it;
  hb_always_assert (*it == 5);
}

صحيح
main (صحيح argc, حرف **argv)
{
  ثابت صحيح src[10] = {};
  صحيح dst[20];
  hb_vector_t<صحيح> v;

  array_iter_t<ثابت صحيح> s (src); /* Implicit conversion from static array. */
  array_iter_t<ثابت صحيح> s2 (v); /* Implicit conversion from vector. */
  array_iter_t<صحيح> t (dst);

  static_assert (array_iter_t<صحيح>::is_random_access_iterator, "");

  some_array_t<ثابت صحيح> a (src);

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
  hb_sorted_array_t<صحيح> sa;
  (فراغ) static_cast<hb_iter_t<hb_sorted_array_t<صحيح>, hb_sorted_array_t<صحيح>::item_t>&> (sa);
  (فراغ) static_cast<hb_iter_t<hb_sorted_array_t<صحيح>, hb_sorted_array_t<صحيح>::__item_t__>&> (sa);
  (فراغ) static_cast<hb_iter_t<hb_sorted_array_t<صحيح>, صحيح&>&>(sa);
  (فراغ) static_cast<hb_iter_t<hb_sorted_array_t<صحيح>>&>(sa);
  (فراغ) static_cast<hb_iter_t<hb_array_t<صحيح>, صحيح&>&> (sa);
  test_iterable (sa);

  test_iterable<hb_array_t<صحيح>> ();
  test_iterable<hb_sorted_array_t<ثابت صحيح>> ();
  test_iterable<hb_vector_t<عائم>> ();
  test_iterable<hb_set_t> ();
  test_iterable<OT::Array16Of<OT::HBUINT16>> ();

  test_iterator (hb_zip (st, v));
  test_iterator_non_default_constructable (hb_enumerate (st));
  test_iterator_non_default_constructable (hb_enumerate (st, -5));
  test_iterator_non_default_constructable (hb_enumerate (hb_iter (st)));
  test_iterator_non_default_constructable (hb_enumerate (hb_iter (st) + 1));
  test_iterator_non_default_constructable (hb_iter (st) | hb_filter ());
  test_iterator_non_default_constructable (hb_iter (st) | hb_map (hb_lidentity));

  hb_always_assert (אמת == hb_all (st));
  hb_always_assert (שקר == hb_all (st, 42u));
  hb_always_assert (אמת == hb_any (st));
  hb_always_assert (שקר == hb_any (st, 14u));
  hb_always_assert (אמת == hb_any (st, 14u, [] (unsigned _) { החזר _ - 1u; }));
  hb_always_assert (אמת == hb_any (st, [] (unsigned _) { החזר _ == 15u; }));
  hb_always_assert (אמת == hb_any (st, 15u));
  hb_always_assert (שקר == hb_none (st));
  hb_always_assert (שקר == hb_none (st, 15u));
  hb_always_assert (אמת == hb_none (st, 17u));

  hb_array_t<hb_vector_t<صحيح>> pa;
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
  | hb_map ([] (صحيح i) { החזר 1; })
  | hb_reduce ([=] (صحيح acc, صحيح value) { החזר acc; }, 2)
  ;

  using map_pair_t = hb_item_type<hb_map_t>;
  + hb_iter (m)
  | hb_map ([] (map_pair_t p) { החזר p.first * p.second; })
  ;

  m.keys ();
  using map_key_t = decltype (*m.keys());
  + hb_iter (m.keys ())
  | hb_filter ([] (map_key_t k) { החזר k < 42; })
  | hb_drain
  ;

  m.values ();
  using map_value_t = decltype (*m.values());
  + hb_iter (m.values ())
  | hb_filter ([] (map_value_t k) { החזר k < 42; })
  | hb_drain
  ;

  unsigned صحيح temp1 = 10;
  unsigned صحيح temp2 = 0;
  hb_map_t *result =
  + hb_iter (src)
  | hb_map ([&] (صحيح i) -> hb_set_t *
	    {
	      hb_set_t *set = hb_set_create ();
	      עבור (unsigned صحيح i = 0; i < temp1; ++i)
		hb_set_add (set, i);
	      temp1++;
	      החזר set;
	    })
  | hb_reduce ([&] (hb_map_t *acc, hb_set_t *value) -> hb_map_t *
	       {
		 hb_map_set (acc, temp2++, hb_set_get_population (value));
		 /* This is not a memory managed language, take care! */
		 hb_set_destroy (value);
		 החזר acc;
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
  | hb_map ([&] (صحيح i) -> hb_set_t
	    {
	      hb_set_t set;
	      עבור (unsigned صحيح i = 0; i < temp1; ++i)
		hb_set_add (&set, i);
	      temp1++;
	      החזר set;
	    })
  | hb_reduce ([&] (hb_map_t *acc, hb_set_t value) -> hb_map_t *
	       {
		 hb_map_set (acc, temp2++, hb_set_get_population (&value));
		 החזר acc;
	       }, hb_map_create ())
  ;
  /* The result should be something like 0->10, 1->11, ..., 9->19 */
  hb_always_assert (hb_map_get (result, 9) == 19);
  hb_map_destroy (result);

  unsigned صحيح temp3 = 0;
  + hb_iter(src)
  | hb_map([&] (صحيح i) { החזר ++temp3; })
  | hb_reduce([&] (عائم acc, صحيح value) { החזר acc + value; }, 0)
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

  החזר 0;
}

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

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-iter.hh"

#അന്തര്ഭാവ "hb-array.hh"
#അന്തര്ഭാവ "hb-set.hh"
#അന്തര്ഭാവ "hb-ot-layout-common.hh"

template <typename T>
ഘടന array_iter_t : hb_iter_with_fallback_t<array_iter_t<T>, T&>
{
  array_iter_t (hb_array_t<T> arr_) : arr (arr_) {}

  തരനിർവചനം T& __item_t__;
  സ്ഥിതം constexpr തർക്കം is_random_access_iterator = സത്യം;
  T& __item_at__ (unsigned i) സ്ഥിരം { തിരികെ arr[i]; }
  ശൂന്യം __forward__ (unsigned n) { arr += n; }
  ശൂന്യം __rewind__ (unsigned n) { arr -= n; }
  unsigned __len__ () സ്ഥിരം { തിരികെ arr.length; }
  തർക്കം operator != (സ്ഥിരം array_iter_t& o) { തിരികെ arr != o.arr; }

  സ്വകാര്യ:
  hb_array_t<T> arr;
};

template <typename T>
ഘടന some_array_t
{
  some_array_t (hb_array_t<T> arr_) : arr (arr_) {}

  തരനിർവചനം array_iter_t<T> iter_t;
  array_iter_t<T> iter () { തിരികെ array_iter_t<T> (arr); }
  operator array_iter_t<T> () { തിരികെ iter (); }
  operator hb_iter_t<array_iter_t<T>> () { തിരികെ iter (); }

  സ്വകാര്യ:
  hb_array_t<T> arr;
};


template <typename Iter,
	  hb_requires (hb_is_iterator (Iter))>
സ്ഥിതം ശൂന്യം
test_iterator_non_default_constructable (Iter it)
{
  /* Iterate over a copy of it. */
  വേണ്ടി (auto c = it.iter (); c; c++)
    *c;

  /* Same. */
  വേണ്ടി (auto c = +it; c; c++)
    *c;

  /* Range-based for over a copy. */
  വേണ്ടി (auto _ : +it)
    (ശൂന്യം) _;

  it += it.len ();
  it = it + 10;
  it = 10 + it;

  hb_always_assert (*it == it[0]);

  static_assert (സത്യം || it.is_random_access_iterator, "");
  static_assert (സത്യം || it.is_sorted_iterator, "");
}

template <typename Iter,
	  hb_requires (hb_is_iterator (Iter))>
സ്ഥിതം ശൂന്യം
test_iterator (Iter it)
{
  Iter default_constructed;
  hb_always_assert (!default_constructed);

  test_iterator_non_default_constructable (it);
}

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
സ്ഥിതം ശൂന്യം
test_iterable (സ്ഥിരം Iterable &lst = Null (Iterable))
{
  വേണ്ടി (auto _ : lst)
    (ശൂന്യം) _;

  // Test that can take iterator from.
  test_iterator (lst.iter ());
}

template <typename It>
സ്ഥിതം ശൂന്യം check_sequential (It it)
{
  പൂർണ്ണസംഖ്യ i = 1;
  വേണ്ടി (പൂർണ്ണസംഖ്യ v : +it) {
    hb_always_assert (v == i++);
  }
}

സ്ഥിതം ശൂന്യം test_concat ()
{
  hb_vector_t<പൂർണ്ണസംഖ്യ> a = {1, 2, 3};
  hb_vector_t<പൂർണ്ണസംഖ്യ> b = {4, 5};

  hb_vector_t<പൂർണ്ണസംഖ്യ> c = {};
  hb_vector_t<പൂർണ്ണസംഖ്യ> d = {1, 2, 3, 4, 5};

  auto it1 = hb_concat (a, b);
  hb_always_assert (it1.len () == 5);
  hb_always_assert (it1.is_random_access_iterator);
  auto it2 = hb_concat (c, d);
  hb_always_assert (it2.len () == 5);
  auto it3 = hb_concat (d, c);
  hb_always_assert (it3.len () == 5);
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < 5; i++) {
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

സ്ഥിതം ശൂന്യം
test_backward_filter ()
{
  hb_vector_t<പൂർണ്ണസംഖ്യ> V = {2, 4, 5, 7, 8, 9, 10};
  auto it = hb_iter (V) | hb_filter ([] (പൂർണ്ണസംഖ്യ x) { തിരികെ x % 2 != 0; });
  it += 2;
  hb_always_assert (*it == 9);
  --it;
  hb_always_assert (*it == 7);
  --it;
  hb_always_assert (*it == 5);
  --it;
  hb_always_assert (*it == 5);
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  സ്ഥിരം പൂർണ്ണസംഖ്യ src[10] = {};
  പൂർണ്ണസംഖ്യ dst[20];
  hb_vector_t<പൂർണ്ണസംഖ്യ> v;

  array_iter_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> s (src); /* Implicit conversion from static array. */
  array_iter_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> s2 (v); /* Implicit conversion from vector. */
  array_iter_t<പൂർണ്ണസംഖ്യ> t (dst);

  static_assert (array_iter_t<പൂർണ്ണസംഖ്യ>::is_random_access_iterator, "");

  some_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> a (src);

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
  hb_sorted_array_t<പൂർണ്ണസംഖ്യ> sa;
  (ശൂന്യം) static_cast<hb_iter_t<hb_sorted_array_t<പൂർണ്ണസംഖ്യ>, hb_sorted_array_t<പൂർണ്ണസംഖ്യ>::item_t>&> (sa);
  (ശൂന്യം) static_cast<hb_iter_t<hb_sorted_array_t<പൂർണ്ണസംഖ്യ>, hb_sorted_array_t<പൂർണ്ണസംഖ്യ>::__item_t__>&> (sa);
  (ശൂന്യം) static_cast<hb_iter_t<hb_sorted_array_t<പൂർണ്ണസംഖ്യ>, പൂർണ്ണസംഖ്യ&>&>(sa);
  (ശൂന്യം) static_cast<hb_iter_t<hb_sorted_array_t<പൂർണ്ണസംഖ്യ>>&>(sa);
  (ശൂന്യം) static_cast<hb_iter_t<hb_array_t<പൂർണ്ണസംഖ്യ>, പൂർണ്ണസംഖ്യ&>&> (sa);
  test_iterable (sa);

  test_iterable<hb_array_t<പൂർണ്ണസംഖ്യ>> ();
  test_iterable<hb_sorted_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ>> ();
  test_iterable<hb_vector_t<ദശാംശം>> ();
  test_iterable<hb_set_t> ();
  test_iterable<OT::Array16Of<OT::HBUINT16>> ();

  test_iterator (hb_zip (st, v));
  test_iterator_non_default_constructable (hb_enumerate (st));
  test_iterator_non_default_constructable (hb_enumerate (st, -5));
  test_iterator_non_default_constructable (hb_enumerate (hb_iter (st)));
  test_iterator_non_default_constructable (hb_enumerate (hb_iter (st) + 1));
  test_iterator_non_default_constructable (hb_iter (st) | hb_filter ());
  test_iterator_non_default_constructable (hb_iter (st) | hb_map (hb_lidentity));

  hb_always_assert (സത്യം == hb_all (st));
  hb_always_assert (അസത്യം == hb_all (st, 42u));
  hb_always_assert (സത്യം == hb_any (st));
  hb_always_assert (അസത്യം == hb_any (st, 14u));
  hb_always_assert (സത്യം == hb_any (st, 14u, [] (unsigned _) { തിരികെ _ - 1u; }));
  hb_always_assert (സത്യം == hb_any (st, [] (unsigned _) { തിരികെ _ == 15u; }));
  hb_always_assert (സത്യം == hb_any (st, 15u));
  hb_always_assert (അസത്യം == hb_none (st));
  hb_always_assert (അസത്യം == hb_none (st, 15u));
  hb_always_assert (സത്യം == hb_none (st, 17u));

  hb_array_t<hb_vector_t<പൂർണ്ണസംഖ്യ>> pa;
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
  | hb_map ([] (പൂർണ്ണസംഖ്യ i) { തിരികെ 1; })
  | hb_reduce ([=] (പൂർണ്ണസംഖ്യ acc, പൂർണ്ണസംഖ്യ value) { തിരികെ acc; }, 2)
  ;

  using map_pair_t = hb_item_type<hb_map_t>;
  + hb_iter (m)
  | hb_map ([] (map_pair_t p) { തിരികെ p.first * p.second; })
  ;

  m.താക്കോലുകൾ ();
  using map_താക്കോൽ_t = decltype (*m.താക്കോലുകൾ());
  + hb_iter (m.താക്കോലുകൾ ())
  | hb_filter ([] (map_താക്കോൽ_t k) { തിരികെ k < 42; })
  | hb_drain
  ;

  m.values ();
  using map_മൂല്യം_t = decltype (*m.values());
  + hb_iter (m.values ())
  | hb_filter ([] (map_മൂല്യം_t k) { തിരികെ k < 42; })
  | hb_drain
  ;

  unsigned പൂർണ്ണസംഖ്യ temp1 = 10;
  unsigned പൂർണ്ണസംഖ്യ temp2 = 0;
  hb_map_t *result =
  + hb_iter (src)
  | hb_map ([&] (പൂർണ്ണസംഖ്യ i) -> hb_set_t *
	    {
	      hb_set_t *set = hb_set_create ();
	      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < temp1; ++i)
		hb_set_add (set, i);
	      temp1++;
	      തിരികെ set;
	    })
  | hb_reduce ([&] (hb_map_t *acc, hb_set_t *value) -> hb_map_t *
	       {
		 hb_map_set (acc, temp2++, hb_set_get_population (value));
		 /* This is not a memory managed language, take care! */
		 hb_set_destroy (value);
		 തിരികെ acc;
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
  | hb_map ([&] (പൂർണ്ണസംഖ്യ i) -> hb_set_t
	    {
	      hb_set_t set;
	      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < temp1; ++i)
		hb_set_add (&set, i);
	      temp1++;
	      തിരികെ set;
	    })
  | hb_reduce ([&] (hb_map_t *acc, hb_set_t value) -> hb_map_t *
	       {
		 hb_map_set (acc, temp2++, hb_set_get_population (&value));
		 തിരികെ acc;
	       }, hb_map_create ())
  ;
  /* The result should be something like 0->10, 1->11, ..., 9->19 */
  hb_always_assert (hb_map_get (result, 9) == 19);
  hb_map_destroy (result);

  unsigned പൂർണ്ണസംഖ്യ temp3 = 0;
  + hb_iter(src)
  | hb_map([&] (പൂർണ്ണസംഖ്യ i) { തിരികെ ++temp3; })
  | hb_reduce([&] (ദശാംശം acc, പൂർണ്ണസംഖ്യ value) { തിരികെ acc + value; }, 0)
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

  തിരികെ 0;
}

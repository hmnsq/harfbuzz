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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-iter.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-array.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-set.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-common.hh"

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ array_iter_t : hb_iter_with_fallback_t<array_iter_t<T>, T&>
{
  array_iter_t (hb_array_t<T> arr_) : arr (arr_) {}

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ T& __item_t__;
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ is_random_access_iterator = ਸੱਚ;
  T& __item_at__ (ਬਿਨਾਨਿਸ਼ਾਨੀ i) ਅਚਲ { ਵਾਪਸ arr[i]; }
  ਖਾਲੀ __forward__ (ਬਿਨਾਨਿਸ਼ਾਨੀ n) { arr += n; }
  ਖਾਲੀ __rewind__ (ਬਿਨਾਨਿਸ਼ਾਨੀ n) { arr -= n; }
  ਬਿਨਾਨਿਸ਼ਾਨੀ __len__ () ਅਚਲ { ਵਾਪਸ arr.ਲੰਮਾਈ; }
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ array_iter_t& o) { ਵਾਪਸ arr != o.arr; }

  ਨਿਜੀ:
  hb_array_t<T> arr;
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ some_array_t
{
  some_array_t (hb_array_t<T> arr_) : arr (arr_) {}

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ array_iter_t<T> iter_t;
  array_iter_t<T> iter () { ਵਾਪਸ array_iter_t<T> (arr); }
  ਸੰਚਾਲਕ array_iter_t<T> () { ਵਾਪਸ iter (); }
  ਸੰਚਾਲਕ hb_iter_t<array_iter_t<T>> () { ਵਾਪਸ iter (); }

  ਨਿਜੀ:
  hb_array_t<T> arr;
};


ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iter,
	  hb_requires (hb_is_iterator (Iter))>
ਸਥਿਰ ਖਾਲੀ
test_iterator_non_default_constructable (Iter it)
{
  /* Iterate over a copy of it. */
  ਲਈ (ਸਵੈਚਾਲਿਤ c = it.iter (); c; c++)
    *c;

  /* Same. */
  ਲਈ (ਸਵੈਚਾਲਿਤ c = +it; c; c++)
    *c;

  /* Range-based for over a copy. */
  ਲਈ (ਸਵੈਚਾਲਿਤ _ : +it)
    (ਖਾਲੀ) _;

  it += it.len ();
  it = it + 10;
  it = 10 + it;

  hb_always_assert (*it == it[0]);

  static_assert (ਸੱਚ || it.is_random_access_iterator, "");
  static_assert (ਸੱਚ || it.is_sorted_iterator, "");
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iter,
	  hb_requires (hb_is_iterator (Iter))>
ਸਥਿਰ ਖਾਲੀ
test_iterator (Iter it)
{
  Iter default_constructed;
  hb_always_assert (!default_constructed);

  test_iterator_non_default_constructable (it);
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ਸਥਿਰ ਖਾਲੀ
test_iterable (ਅਚਲ Iterable &lst = Null (Iterable))
{
  ਲਈ (ਸਵੈਚਾਲਿਤ _ : lst)
    (ਖਾਲੀ) _;

  // Test that can take iterator from.
  test_iterator (lst.iter ());
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ It>
ਸਥਿਰ ਖਾਲੀ check_sequential (It it)
{
  ਪੂਰਨ i = 1;
  ਲਈ (ਪੂਰਨ v : +it) {
    hb_always_assert (v == i++);
  }
}

ਸਥਿਰ ਖਾਲੀ test_concat ()
{
  hb_ਸਦਿਸ਼_t<ਪੂਰਨ> a = {1, 2, 3};
  hb_ਸਦਿਸ਼_t<ਪੂਰਨ> b = {4, 5};

  hb_ਸਦਿਸ਼_t<ਪੂਰਨ> c = {};
  hb_ਸਦਿਸ਼_t<ਪੂਰਨ> d = {1, 2, 3, 4, 5};

  ਸਵੈਚਾਲਿਤ it1 = hb_concat (a, b);
  hb_always_assert (it1.len () == 5);
  hb_always_assert (it1.is_random_access_iterator);
  ਸਵੈਚਾਲਿਤ it2 = hb_concat (c, d);
  hb_always_assert (it2.len () == 5);
  ਸਵੈਚਾਲਿਤ it3 = hb_concat (d, c);
  hb_always_assert (it3.len () == 5);
  ਲਈ (ਪੂਰਨ i = 0; i < 5; i++) {
    hb_always_assert(it1[i] == i + 1);
    hb_always_assert(it2[i] == i + 1);
    hb_always_assert(it3[i] == i + 1);
  }

  check_sequential (it1);
  check_sequential (it2);
  check_sequential (it3);

  ਸਵੈਚਾਲਿਤ it4 = +it1;
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

  ਸਵੈਚਾਲਿਤ it5 = +it1;
  it5 += 3;
  hb_always_assert (*it5 == 4);

  hb_ਸੈੱਟ_t s_a = {1, 2, 3};
  hb_ਸੈੱਟ_t s_b = {4, 5};
  ਸਵੈਚਾਲਿਤ it6 = hb_concat (s_a, s_b);
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

ਸਥਿਰ ਖਾਲੀ
test_backward_filter ()
{
  hb_ਸਦਿਸ਼_t<ਪੂਰਨ> V = {2, 4, 5, 7, 8, 9, 10};
  ਸਵੈਚਾਲਿਤ it = hb_iter (V) | hb_filter ([] (ਪੂਰਨ x) { ਵਾਪਸ x % 2 != 0; });
  it += 2;
  hb_always_assert (*it == 9);
  --it;
  hb_always_assert (*it == 7);
  --it;
  hb_always_assert (*it == 5);
  --it;
  hb_always_assert (*it == 5);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਅਚਲ ਪੂਰਨ src[10] = {};
  ਪੂਰਨ dst[20];
  hb_ਸਦਿਸ਼_t<ਪੂਰਨ> v;

  array_iter_t<ਅਚਲ ਪੂਰਨ> s (src); /* Implicit conversion from static array. */
  array_iter_t<ਅਚਲ ਪੂਰਨ> s2 (v); /* Implicit conversion from vector. */
  array_iter_t<ਪੂਰਨ> t (dst);

  static_assert (array_iter_t<ਪੂਰਨ>::is_random_access_iterator, "");

  some_array_t<ਅਚਲ ਪੂਰਨ> a (src);

  s2 = s;

  hb_iter (src);
  hb_iter (src, 2);

  hb_fill (t, 42);
  hb_copy (s, t);
  hb_copy (a.iter (), t);

  test_iterable (v);
  hb_ਸੈੱਟ_t st;
  st << 1 << 15 << 43;
  test_iterable (st);
  hb_sorted_array_t<ਪੂਰਨ> sa;
  (ਖਾਲੀ) static_cast<hb_iter_t<hb_sorted_array_t<ਪੂਰਨ>, hb_sorted_array_t<ਪੂਰਨ>::ਵਸਤੂ_t>&> (sa);
  (ਖਾਲੀ) static_cast<hb_iter_t<hb_sorted_array_t<ਪੂਰਨ>, hb_sorted_array_t<ਪੂਰਨ>::__item_t__>&> (sa);
  (ਖਾਲੀ) static_cast<hb_iter_t<hb_sorted_array_t<ਪੂਰਨ>, ਪੂਰਨ&>&>(sa);
  (ਖਾਲੀ) static_cast<hb_iter_t<hb_sorted_array_t<ਪੂਰਨ>>&>(sa);
  (ਖਾਲੀ) static_cast<hb_iter_t<hb_array_t<ਪੂਰਨ>, ਪੂਰਨ&>&> (sa);
  test_iterable (sa);

  test_iterable<hb_array_t<ਪੂਰਨ>> ();
  test_iterable<hb_sorted_array_t<ਅਚਲ ਪੂਰਨ>> ();
  test_iterable<hb_ਸਦਿਸ਼_t<ਦਸ਼ਮਲਵ>> ();
  test_iterable<hb_ਸੈੱਟ_t> ();
  test_iterable<OT::Array16_ਦਾ<OT::HBUINT16>> ();

  test_iterator (hb_zip (st, v));
  test_iterator_non_default_constructable (hb_enumerate (st));
  test_iterator_non_default_constructable (hb_enumerate (st, -5));
  test_iterator_non_default_constructable (hb_enumerate (hb_iter (st)));
  test_iterator_non_default_constructable (hb_enumerate (hb_iter (st) + 1));
  test_iterator_non_default_constructable (hb_iter (st) | hb_filter ());
  test_iterator_non_default_constructable (hb_iter (st) | hb_map (hb_lidentity));

  hb_always_assert (ਸੱਚ == hb_all (st));
  hb_always_assert (ਝੂਠ == hb_all (st, 42u));
  hb_always_assert (ਸੱਚ == hb_any (st));
  hb_always_assert (ਝੂਠ == hb_any (st, 14u));
  hb_always_assert (ਸੱਚ == hb_any (st, 14u, [] (ਬਿਨਾਨਿਸ਼ਾਨੀ _) { ਵਾਪਸ _ - 1u; }));
  hb_always_assert (ਸੱਚ == hb_any (st, [] (ਬਿਨਾਨਿਸ਼ਾਨੀ _) { ਵਾਪਸ _ == 15u; }));
  hb_always_assert (ਸੱਚ == hb_any (st, 15u));
  hb_always_assert (ਝੂਠ == hb_none (st));
  hb_always_assert (ਝੂਠ == hb_none (st, 15u));
  hb_always_assert (ਸੱਚ == hb_none (st, 17u));

  hb_array_t<hb_ਸਦਿਸ਼_t<ਪੂਰਨ>> pa;
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
  | hb_filter (hb_bool, hb_ਪਛਾਣ_ਤੱਤ)
  | hb_sink (st)
  ;

  + hb_iter (src)
  | hb_sink (hb_array (dst))
  ;

  + hb_iter (src)
  | hb_apply (&st)
  ;

  + hb_iter (src)
  | hb_map ([] (ਪੂਰਨ i) { ਵਾਪਸ 1; })
  | hb_reduce ([=] (ਪੂਰਨ acc, ਪੂਰਨ value) { ਵਾਪਸ acc; }, 2)
  ;

  ਵਰਤੋਂ map_pair_t = hb_ਵਸਤੂ_type<hb_map_t>;
  + hb_iter (m)
  | hb_map ([] (map_pair_t p) { ਵਾਪਸ p.ਪਹਿਲਾ * p.second; })
  ;

  m.ਕੁੰਜੀਆਂ ();
  ਵਰਤੋਂ map_ਕੁੰਜੀ_t = decltype (*m.ਕੁੰਜੀਆਂ());
  + hb_iter (m.ਕੁੰਜੀਆਂ ())
  | hb_filter ([] (map_ਕੁੰਜੀ_t k) { ਵਾਪਸ k < 42; })
  | hb_drain
  ;

  m.values ();
  ਵਰਤੋਂ map_ਮੁੱਲ_t = decltype (*m.values());
  + hb_iter (m.values ())
  | hb_filter ([] (map_ਮੁੱਲ_t k) { ਵਾਪਸ k < 42; })
  | hb_drain
  ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ temp1 = 10;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ temp2 = 0;
  hb_map_t *ਨਤੀਜਾ =
  + hb_iter (src)
  | hb_map ([&] (ਪੂਰਨ i) -> hb_ਸੈੱਟ_t *
	    {
	      hb_ਸੈੱਟ_t *ਸੈੱਟ = hb_ਸੈੱਟ_create ();
	      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < temp1; ++i)
		hb_ਸੈੱਟ_ਜੋੜੋ (ਸੈੱਟ, i);
	      temp1++;
	      ਵਾਪਸ ਸੈੱਟ;
	    })
  | hb_reduce ([&] (hb_map_t *acc, hb_ਸੈੱਟ_t *value) -> hb_map_t *
	       {
		 hb_map_ਸੈੱਟ (acc, temp2++, hb_ਸੈੱਟ_get_population (value));
		 /* This is not a memory managed language, take care! */
		 hb_ਸੈੱਟ_destroy (value);
		 ਵਾਪਸ acc;
	       }, hb_map_create ())
  ;
  /* The result should be something like 0->10, 1->11, ..., 9->19 */
  hb_always_assert (hb_map_get (ਨਤੀਜਾ, 9) == 19);
  hb_map_destroy (ਨਤੀਜਾ);

  /* Like above, but passing hb_set_t instead of hb_set_t * */
  temp1 = 10;
  temp2 = 0;
  ਨਤੀਜਾ =
  + hb_iter (src)
  | hb_map ([&] (ਪੂਰਨ i) -> hb_ਸੈੱਟ_t
	    {
	      hb_ਸੈੱਟ_t ਸੈੱਟ;
	      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < temp1; ++i)
		hb_ਸੈੱਟ_ਜੋੜੋ (&ਸੈੱਟ, i);
	      temp1++;
	      ਵਾਪਸ ਸੈੱਟ;
	    })
  | hb_reduce ([&] (hb_map_t *acc, hb_ਸੈੱਟ_t value) -> hb_map_t *
	       {
		 hb_map_ਸੈੱਟ (acc, temp2++, hb_ਸੈੱਟ_get_population (&value));
		 ਵਾਪਸ acc;
	       }, hb_map_create ())
  ;
  /* The result should be something like 0->10, 1->11, ..., 9->19 */
  hb_always_assert (hb_map_get (ਨਤੀਜਾ, 9) == 19);
  hb_map_destroy (ਨਤੀਜਾ);

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ temp3 = 0;
  + hb_iter(src)
  | hb_map([&] (ਪੂਰਨ i) { ਵਾਪਸ ++temp3; })
  | hb_reduce([&] (ਦਸ਼ਮਲਵ acc, ਪੂਰਨ value) { ਵਾਪਸ acc + value; }, 0)
  ;

  + hb_iter (src)
  | hb_drain
  ;

  t << 1;
  ਲੰਮਾ vl;
  s >> vl;

  hb_iota ();
  hb_iota (3);
  hb_iota (3, 2);
  hb_always_assert ((&vl) + 1 == *++hb_iota (&vl, hb_inc));
  hb_range ();
  hb_repeat (7u);
  hb_repeat (ਖਾਲੀ_ਸੂਚਕ);
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

  ਵਾਪਸ 0;
}

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

#شامل "hb.hh"
#شامل "hb-iter.hh"

#شامل "hb-array.hh"
#شامل "hb-set.hh"
#شامل "hb-ot-layout-common.hh"

سانچہ <قسم_نام T>
ساخت array_iter_t : hb_iter_with_fallback_t<array_iter_t<T>, T&>
{
  array_iter_t (hb_array_t<T> arr_) : arr (arr_) {}

  قسمتعریف T& __item_t__;
  ساکن constexpr منطقی is_random_access_iterator = سچ;
  T& __item_at__ (غیرعلامتی i) ثابت { واپس arr[i]; }
  خالی __forward__ (غیرعلامتی n) { arr += n; }
  خالی __rewind__ (غیرعلامتی n) { arr -= n; }
  غیرعلامتی __len__ () ثابت { واپس arr.لمبائی; }
  منطقی عامل != (ثابت array_iter_t& o) { واپس arr != o.arr; }

  نجی:
  hb_array_t<T> arr;
};

سانچہ <قسم_نام T>
ساخت some_array_t
{
  some_array_t (hb_array_t<T> arr_) : arr (arr_) {}

  قسمتعریف array_iter_t<T> iter_t;
  array_iter_t<T> iter () { واپس array_iter_t<T> (arr); }
  عامل array_iter_t<T> () { واپس iter (); }
  عامل hb_iter_t<array_iter_t<T>> () { واپس iter (); }

  نجی:
  hb_array_t<T> arr;
};


سانچہ <قسم_نام Iter,
	  hb_requires (hb_is_iterator (Iter))>
ساکن خالی
test_iterator_non_default_constructable (Iter it)
{
  /* Iterate over a copy of it. */
  برائے (خودکار c = it.iter (); c; c++)
    *c;

  /* Same. */
  برائے (خودکار c = +it; c; c++)
    *c;

  /* Range-based for over a copy. */
  برائے (خودکار _ : +it)
    (خالی) _;

  it += it.len ();
  it = it + 10;
  it = 10 + it;

  hb_always_assert (*it == it[0]);

  static_assert (سچ || it.is_random_access_iterator, "");
  static_assert (سچ || it.is_sorted_iterator, "");
}

سانچہ <قسم_نام Iter,
	  hb_requires (hb_is_iterator (Iter))>
ساکن خالی
test_iterator (Iter it)
{
  Iter default_constructed;
  hb_always_assert (!default_constructed);

  test_iterator_non_default_constructable (it);
}

سانچہ <قسم_نام Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ساکن خالی
test_iterable (ثابت Iterable &lst = Null (Iterable))
{
  برائے (خودکار _ : lst)
    (خالی) _;

  // Test that can take iterator from.
  test_iterator (lst.iter ());
}

سانچہ <قسم_نام It>
ساکن خالی check_sequential (It it)
{
  صحیح i = 1;
  برائے (صحیح v : +it) {
    hb_always_assert (v == i++);
  }
}

ساکن خالی test_concat ()
{
  hb_سمتیہ_t<صحیح> a = {1, 2, 3};
  hb_سمتیہ_t<صحیح> b = {4, 5};

  hb_سمتیہ_t<صحیح> c = {};
  hb_سمتیہ_t<صحیح> d = {1, 2, 3, 4, 5};

  خودکار it1 = hb_concat (a, b);
  hb_always_assert (it1.len () == 5);
  hb_always_assert (it1.is_random_access_iterator);
  خودکار it2 = hb_concat (c, d);
  hb_always_assert (it2.len () == 5);
  خودکار it3 = hb_concat (d, c);
  hb_always_assert (it3.len () == 5);
  برائے (صحیح i = 0; i < 5; i++) {
    hb_always_assert(it1[i] == i + 1);
    hb_always_assert(it2[i] == i + 1);
    hb_always_assert(it3[i] == i + 1);
  }

  check_sequential (it1);
  check_sequential (it2);
  check_sequential (it3);

  خودکار it4 = +it1;
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

  خودکار it5 = +it1;
  it5 += 3;
  hb_always_assert (*it5 == 4);

  hb_سیٹ_t s_a = {1, 2, 3};
  hb_سیٹ_t s_b = {4, 5};
  خودکار it6 = hb_concat (s_a, s_b);
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

ساکن خالی
test_backward_filter ()
{
  hb_سمتیہ_t<صحیح> V = {2, 4, 5, 7, 8, 9, 10};
  خودکار it = hb_iter (V) | hb_filter ([] (صحیح x) { واپس x % 2 != 0; });
  it += 2;
  hb_always_assert (*it == 9);
  --it;
  hb_always_assert (*it == 7);
  --it;
  hb_always_assert (*it == 5);
  --it;
  hb_always_assert (*it == 5);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  ثابت صحیح src[10] = {};
  صحیح dst[20];
  hb_سمتیہ_t<صحیح> v;

  array_iter_t<ثابت صحیح> s (src); /* Implicit conversion from static array. */
  array_iter_t<ثابت صحیح> s2 (v); /* Implicit conversion from vector. */
  array_iter_t<صحیح> t (dst);

  static_assert (array_iter_t<صحیح>::is_random_access_iterator, "");

  some_array_t<ثابت صحیح> a (src);

  s2 = s;

  hb_iter (src);
  hb_iter (src, 2);

  hb_fill (t, 42);
  hb_copy (s, t);
  hb_copy (a.iter (), t);

  test_iterable (v);
  hb_سیٹ_t st;
  st << 1 << 15 << 43;
  test_iterable (st);
  hb_sorted_array_t<صحیح> sa;
  (خالی) static_cast<hb_iter_t<hb_sorted_array_t<صحیح>, hb_sorted_array_t<صحیح>::شے_t>&> (sa);
  (خالی) static_cast<hb_iter_t<hb_sorted_array_t<صحیح>, hb_sorted_array_t<صحیح>::__item_t__>&> (sa);
  (خالی) static_cast<hb_iter_t<hb_sorted_array_t<صحیح>, صحیح&>&>(sa);
  (خالی) static_cast<hb_iter_t<hb_sorted_array_t<صحیح>>&>(sa);
  (خالی) static_cast<hb_iter_t<hb_array_t<صحیح>, صحیح&>&> (sa);
  test_iterable (sa);

  test_iterable<hb_array_t<صحیح>> ();
  test_iterable<hb_sorted_array_t<ثابت صحیح>> ();
  test_iterable<hb_سمتیہ_t<اعشاری>> ();
  test_iterable<hb_سیٹ_t> ();
  test_iterable<OT::Array16_کا<OT::HBUINT16>> ();

  test_iterator (hb_zip (st, v));
  test_iterator_non_default_constructable (hb_enumerate (st));
  test_iterator_non_default_constructable (hb_enumerate (st, -5));
  test_iterator_non_default_constructable (hb_enumerate (hb_iter (st)));
  test_iterator_non_default_constructable (hb_enumerate (hb_iter (st) + 1));
  test_iterator_non_default_constructable (hb_iter (st) | hb_filter ());
  test_iterator_non_default_constructable (hb_iter (st) | hb_map (hb_lidentity));

  hb_always_assert (سچ == hb_all (st));
  hb_always_assert (جھوٹ == hb_all (st, 42u));
  hb_always_assert (سچ == hb_any (st));
  hb_always_assert (جھوٹ == hb_any (st, 14u));
  hb_always_assert (سچ == hb_any (st, 14u, [] (غیرعلامتی _) { واپس _ - 1u; }));
  hb_always_assert (سچ == hb_any (st, [] (غیرعلامتی _) { واپس _ == 15u; }));
  hb_always_assert (سچ == hb_any (st, 15u));
  hb_always_assert (جھوٹ == hb_none (st));
  hb_always_assert (جھوٹ == hb_none (st, 15u));
  hb_always_assert (سچ == hb_none (st, 17u));

  hb_array_t<hb_سمتیہ_t<صحیح>> pa;
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
  | hb_filter (hb_bool, hb_شناختی_عنصر)
  | hb_sink (st)
  ;

  + hb_iter (src)
  | hb_sink (hb_array (dst))
  ;

  + hb_iter (src)
  | hb_apply (&st)
  ;

  + hb_iter (src)
  | hb_map ([] (صحیح i) { واپس 1; })
  | hb_reduce ([=] (صحیح acc, صحیح value) { واپس acc; }, 2)
  ;

  استعمال map_pair_t = hb_شے_type<hb_map_t>;
  + hb_iter (m)
  | hb_map ([] (map_pair_t p) { واپس p.پہلا * p.second; })
  ;

  m.کلیدیں ();
  استعمال map_کلید_t = decltype (*m.کلیدیں());
  + hb_iter (m.کلیدیں ())
  | hb_filter ([] (map_کلید_t k) { واپس k < 42; })
  | hb_drain
  ;

  m.values ();
  استعمال map_قدر_t = decltype (*m.values());
  + hb_iter (m.values ())
  | hb_filter ([] (map_قدر_t k) { واپس k < 42; })
  | hb_drain
  ;

  غیرعلامتی صحیح temp1 = 10;
  غیرعلامتی صحیح temp2 = 0;
  hb_map_t *نتیجہ =
  + hb_iter (src)
  | hb_map ([&] (صحیح i) -> hb_سیٹ_t *
	    {
	      hb_سیٹ_t *سیٹ = hb_سیٹ_create ();
	      برائے (غیرعلامتی صحیح i = 0; i < temp1; ++i)
		hb_سیٹ_جوڑو (سیٹ, i);
	      temp1++;
	      واپس سیٹ;
	    })
  | hb_reduce ([&] (hb_map_t *acc, hb_سیٹ_t *value) -> hb_map_t *
	       {
		 hb_map_سیٹ (acc, temp2++, hb_سیٹ_get_population (value));
		 /* This is not a memory managed language, take care! */
		 hb_سیٹ_destroy (value);
		 واپس acc;
	       }, hb_map_create ())
  ;
  /* The result should be something like 0->10, 1->11, ..., 9->19 */
  hb_always_assert (hb_map_get (نتیجہ, 9) == 19);
  hb_map_destroy (نتیجہ);

  /* Like above, but passing hb_set_t instead of hb_set_t * */
  temp1 = 10;
  temp2 = 0;
  نتیجہ =
  + hb_iter (src)
  | hb_map ([&] (صحیح i) -> hb_سیٹ_t
	    {
	      hb_سیٹ_t سیٹ;
	      برائے (غیرعلامتی صحیح i = 0; i < temp1; ++i)
		hb_سیٹ_جوڑو (&سیٹ, i);
	      temp1++;
	      واپس سیٹ;
	    })
  | hb_reduce ([&] (hb_map_t *acc, hb_سیٹ_t value) -> hb_map_t *
	       {
		 hb_map_سیٹ (acc, temp2++, hb_سیٹ_get_population (&value));
		 واپس acc;
	       }, hb_map_create ())
  ;
  /* The result should be something like 0->10, 1->11, ..., 9->19 */
  hb_always_assert (hb_map_get (نتیجہ, 9) == 19);
  hb_map_destroy (نتیجہ);

  غیرعلامتی صحیح temp3 = 0;
  + hb_iter(src)
  | hb_map([&] (صحیح i) { واپس ++temp3; })
  | hb_reduce([&] (اعشاری acc, صحیح value) { واپس acc + value; }, 0)
  ;

  + hb_iter (src)
  | hb_drain
  ;

  t << 1;
  طویل vl;
  s >> vl;

  hb_iota ();
  hb_iota (3);
  hb_iota (3, 2);
  hb_always_assert ((&vl) + 1 == *++hb_iota (&vl, hb_inc));
  hb_range ();
  hb_repeat (7u);
  hb_repeat (خالی_اشاریہ);
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

  واپس 0;
}

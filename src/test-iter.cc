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

کينډۍ <ډول_نوم T>
بنية array_iter_t : hb_iter_with_fallback_t<array_iter_t<T>, T&>
{
  array_iter_t (hb_array_t<T> arr_) : arr (arr_) {}

  تعريفنوع T& __item_t__;
  ساكن constexpr منطقي is_random_access_iterator = ریښتیا;
  T& __item_at__ (بینښه i) ثابت { أرجع arr[i]; }
  فراغ __forward__ (بینښه n) { arr += n; }
  فراغ __rewind__ (بینښه n) { arr -= n; }
  بینښه __len__ () ثابت { أرجع arr.اوږدتیا; }
  منطقي چلوونکی != (ثابت array_iter_t& o) { أرجع arr != o.arr; }

  خاص:
  hb_array_t<T> arr;
};

کينډۍ <ډول_نوم T>
بنية some_array_t
{
  some_array_t (hb_array_t<T> arr_) : arr (arr_) {}

  تعريفنوع array_iter_t<T> iter_t;
  array_iter_t<T> iter () { أرجع array_iter_t<T> (arr); }
  چلوونکی array_iter_t<T> () { أرجع iter (); }
  چلوونکی hb_iter_t<array_iter_t<T>> () { أرجع iter (); }

  خاص:
  hb_array_t<T> arr;
};


کينډۍ <ډول_نوم Iter,
	  hb_requires (hb_is_iterator (Iter))>
ساكن فراغ
test_iterator_non_default_constructable (Iter it)
{
  /* Iterate over a copy of it. */
  لكل (خپلکاری c = it.iter (); c; c++)
    *c;

  /* Same. */
  لكل (خپلکاری c = +it; c; c++)
    *c;

  /* Range-based for over a copy. */
  لكل (خپلکاری _ : +it)
    (فراغ) _;

  it += it.len ();
  it = it + 10;
  it = 10 + it;

  hb_always_assert (*it == it[0]);

  static_assert (ریښتیا || it.is_random_access_iterator, "");
  static_assert (ریښتیا || it.is_sorted_iterator, "");
}

کينډۍ <ډول_نوم Iter,
	  hb_requires (hb_is_iterator (Iter))>
ساكن فراغ
test_iterator (Iter it)
{
  Iter default_constructed;
  hb_always_assert (!default_constructed);

  test_iterator_non_default_constructable (it);
}

کينډۍ <ډول_نوم Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ساكن فراغ
test_iterable (ثابت Iterable &lst = Null (Iterable))
{
  لكل (خپلکاری _ : lst)
    (فراغ) _;

  // Test that can take iterator from.
  test_iterator (lst.iter ());
}

کينډۍ <ډول_نوم It>
ساكن فراغ check_sequential (It it)
{
  صحيح i = 1;
  لكل (صحيح v : +it) {
    hb_always_assert (v == i++);
  }
}

ساكن فراغ test_concat ()
{
  hb_ویکټور_t<صحيح> a = {1, 2, 3};
  hb_ویکټور_t<صحيح> b = {4, 5};

  hb_ویکټور_t<صحيح> c = {};
  hb_ویکټور_t<صحيح> d = {1, 2, 3, 4, 5};

  خپلکاری it1 = hb_concat (a, b);
  hb_always_assert (it1.len () == 5);
  hb_always_assert (it1.is_random_access_iterator);
  خپلکاری it2 = hb_concat (c, d);
  hb_always_assert (it2.len () == 5);
  خپلکاری it3 = hb_concat (d, c);
  hb_always_assert (it3.len () == 5);
  لكل (صحيح i = 0; i < 5; i++) {
    hb_always_assert(it1[i] == i + 1);
    hb_always_assert(it2[i] == i + 1);
    hb_always_assert(it3[i] == i + 1);
  }

  check_sequential (it1);
  check_sequential (it2);
  check_sequential (it3);

  خپلکاری it4 = +it1;
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

  خپلکاری it5 = +it1;
  it5 += 3;
  hb_always_assert (*it5 == 4);

  hb_سېټ_t s_a = {1, 2, 3};
  hb_سېټ_t s_b = {4, 5};
  خپلکاری it6 = hb_concat (s_a, s_b);
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
  hb_ویکټور_t<صحيح> V = {2, 4, 5, 7, 8, 9, 10};
  خپلکاری it = hb_iter (V) | hb_filter ([] (صحيح x) { أرجع x % 2 != 0; });
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
اصلي (صحيح argc, حرف **argv)
{
  ثابت صحيح src[10] = {};
  صحيح dst[20];
  hb_ویکټور_t<صحيح> v;

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
  hb_سېټ_t st;
  st << 1 << 15 << 43;
  test_iterable (st);
  hb_sorted_array_t<صحيح> sa;
  (فراغ) static_cast<hb_iter_t<hb_sorted_array_t<صحيح>, hb_sorted_array_t<صحيح>::توکی_t>&> (sa);
  (فراغ) static_cast<hb_iter_t<hb_sorted_array_t<صحيح>, hb_sorted_array_t<صحيح>::__item_t__>&> (sa);
  (فراغ) static_cast<hb_iter_t<hb_sorted_array_t<صحيح>, صحيح&>&>(sa);
  (فراغ) static_cast<hb_iter_t<hb_sorted_array_t<صحيح>>&>(sa);
  (فراغ) static_cast<hb_iter_t<hb_array_t<صحيح>, صحيح&>&> (sa);
  test_iterable (sa);

  test_iterable<hb_array_t<صحيح>> ();
  test_iterable<hb_sorted_array_t<ثابت صحيح>> ();
  test_iterable<hb_ویکټور_t<عائم>> ();
  test_iterable<hb_سېټ_t> ();
  test_iterable<OT::Array16_د<OT::HBUINT16>> ();

  test_iterator (hb_zip (st, v));
  test_iterator_non_default_constructable (hb_enumerate (st));
  test_iterator_non_default_constructable (hb_enumerate (st, -5));
  test_iterator_non_default_constructable (hb_enumerate (hb_iter (st)));
  test_iterator_non_default_constructable (hb_enumerate (hb_iter (st) + 1));
  test_iterator_non_default_constructable (hb_iter (st) | hb_filter ());
  test_iterator_non_default_constructable (hb_iter (st) | hb_map (hb_lidentity));

  hb_always_assert (ریښتیا == hb_all (st));
  hb_always_assert (خطأ == hb_all (st, 42u));
  hb_always_assert (ریښتیا == hb_any (st));
  hb_always_assert (خطأ == hb_any (st, 14u));
  hb_always_assert (ریښتیا == hb_any (st, 14u, [] (بینښه _) { أرجع _ - 1u; }));
  hb_always_assert (ریښتیا == hb_any (st, [] (بینښه _) { أرجع _ == 15u; }));
  hb_always_assert (ریښتیا == hb_any (st, 15u));
  hb_always_assert (خطأ == hb_none (st));
  hb_always_assert (خطأ == hb_none (st, 15u));
  hb_always_assert (ریښتیا == hb_none (st, 17u));

  hb_array_t<hb_ویکټور_t<صحيح>> pa;
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
  | hb_filter (hb_bool, hb_بې_اغېزه_عنصر)
  | hb_sink (st)
  ;

  + hb_iter (src)
  | hb_sink (hb_array (dst))
  ;

  + hb_iter (src)
  | hb_apply (&st)
  ;

  + hb_iter (src)
  | hb_map ([] (صحيح i) { أرجع 1; })
  | hb_reduce ([=] (صحيح acc, صحيح value) { أرجع acc; }, 2)
  ;

  کارول map_pair_t = hb_توکی_type<hb_map_t>;
  + hb_iter (m)
  | hb_map ([] (map_pair_t p) { أرجع p.لومړی * p.second; })
  ;

  m.کلیګانې ();
  کارول map_کلی_t = decltype (*m.کلیګانې());
  + hb_iter (m.کلیګانې ())
  | hb_filter ([] (map_کلی_t k) { أرجع k < 42; })
  | hb_drain
  ;

  m.values ();
  کارول map_ارزښت_t = decltype (*m.values());
  + hb_iter (m.values ())
  | hb_filter ([] (map_ارزښت_t k) { أرجع k < 42; })
  | hb_drain
  ;

  بینښه صحيح temp1 = 10;
  بینښه صحيح temp2 = 0;
  hb_map_t *پایله =
  + hb_iter (src)
  | hb_map ([&] (صحيح i) -> hb_سېټ_t *
	    {
	      hb_سېټ_t *سېټ = hb_سېټ_create ();
	      لكل (بینښه صحيح i = 0; i < temp1; ++i)
		hb_سېټ_جمع_کړه (سېټ, i);
	      temp1++;
	      أرجع سېټ;
	    })
  | hb_reduce ([&] (hb_map_t *acc, hb_سېټ_t *value) -> hb_map_t *
	       {
		 hb_map_سېټ (acc, temp2++, hb_سېټ_get_population (value));
		 /* This is not a memory managed language, take care! */
		 hb_سېټ_destroy (value);
		 أرجع acc;
	       }, hb_map_create ())
  ;
  /* The result should be something like 0->10, 1->11, ..., 9->19 */
  hb_always_assert (hb_map_get (پایله, 9) == 19);
  hb_map_destroy (پایله);

  /* Like above, but passing hb_set_t instead of hb_set_t * */
  temp1 = 10;
  temp2 = 0;
  پایله =
  + hb_iter (src)
  | hb_map ([&] (صحيح i) -> hb_سېټ_t
	    {
	      hb_سېټ_t سېټ;
	      لكل (بینښه صحيح i = 0; i < temp1; ++i)
		hb_سېټ_جمع_کړه (&سېټ, i);
	      temp1++;
	      أرجع سېټ;
	    })
  | hb_reduce ([&] (hb_map_t *acc, hb_سېټ_t value) -> hb_map_t *
	       {
		 hb_map_سېټ (acc, temp2++, hb_سېټ_get_population (&value));
		 أرجع acc;
	       }, hb_map_create ())
  ;
  /* The result should be something like 0->10, 1->11, ..., 9->19 */
  hb_always_assert (hb_map_get (پایله, 9) == 19);
  hb_map_destroy (پایله);

  بینښه صحيح temp3 = 0;
  + hb_iter(src)
  | hb_map([&] (صحيح i) { أرجع ++temp3; })
  | hb_reduce([&] (عائم acc, صحيح value) { أرجع acc + value; }, 0)
  ;

  + hb_iter (src)
  | hb_drain
  ;

  t << 1;
  اوږد vl;
  s >> vl;

  hb_iota ();
  hb_iota (3);
  hb_iota (3, 2);
  hb_always_assert ((&vl) + 1 == *++hb_iota (&vl, hb_inc));
  hb_range ();
  hb_repeat (7u);
  hb_repeat (تش_اشاره);
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

  أرجع 0;
}

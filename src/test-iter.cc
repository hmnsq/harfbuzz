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

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-iter.hh"

#అన్తర్భావ "hb-array.hh"
#అన్తర్భావ "hb-set.hh"
#అన్తర్భావ "hb-ot-layout-common.hh"

మూస <రకం_పేరు T>
నిర్మాణం array_iter_t : hb_iter_with_fallback_t<array_iter_t<T>, T&>
{
  array_iter_t (hb_array_t<T> arr_) : arr (arr_) {}

  రకనిర్వచనం T& __item_t__;
  స్థిర constexpr తార్కిక is_random_access_iterator = నిజం;
  T& __item_at__ (గుర్తులేని i) స్థిరరాశి { తిరిగి arr[i]; }
  శూన్య __forward__ (గుర్తులేని n) { arr += n; }
  శూన్య __rewind__ (గుర్తులేని n) { arr -= n; }
  గుర్తులేని __len__ () స్థిరరాశి { తిరిగి arr.నిడివి; }
  తార్కిక క్రియాకారకం != (స్థిరరాశి array_iter_t& o) { తిరిగి arr != o.arr; }

  అంతరంగ:
  hb_array_t<T> arr;
};

మూస <రకం_పేరు T>
నిర్మాణం some_array_t
{
  some_array_t (hb_array_t<T> arr_) : arr (arr_) {}

  రకనిర్వచనం array_iter_t<T> iter_t;
  array_iter_t<T> iter () { తిరిగి array_iter_t<T> (arr); }
  క్రియాకారకం array_iter_t<T> () { తిరిగి iter (); }
  క్రియాకారకం hb_iter_t<array_iter_t<T>> () { తిరిగి iter (); }

  అంతరంగ:
  hb_array_t<T> arr;
};


మూస <రకం_పేరు Iter,
	  hb_requires (hb_is_iterator (Iter))>
స్థిర శూన్య
test_iterator_non_default_constructable (Iter it)
{
  /* Iterate over a copy of it. */
  కొరకు (స్వయంచాలక c = it.iter (); c; c++)
    *c;

  /* Same. */
  కొరకు (స్వయంచాలక c = +it; c; c++)
    *c;

  /* Range-based for over a copy. */
  కొరకు (స్వయంచాలక _ : +it)
    (శూన్య) _;

  it += it.len ();
  it = it + 10;
  it = 10 + it;

  hb_always_assert (*it == it[0]);

  static_assert (నిజం || it.is_random_access_iterator, "");
  static_assert (నిజం || it.is_sorted_iterator, "");
}

మూస <రకం_పేరు Iter,
	  hb_requires (hb_is_iterator (Iter))>
స్థిర శూన్య
test_iterator (Iter it)
{
  Iter default_constructed;
  hb_always_assert (!default_constructed);

  test_iterator_non_default_constructable (it);
}

మూస <రకం_పేరు Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
స్థిర శూన్య
test_iterable (స్థిరరాశి Iterable &lst = Null (Iterable))
{
  కొరకు (స్వయంచాలక _ : lst)
    (శూన్య) _;

  // Test that can take iterator from.
  test_iterator (lst.iter ());
}

మూస <రకం_పేరు It>
స్థిర శూన్య check_sequential (It it)
{
  పూర్ణాంకం i = 1;
  కొరకు (పూర్ణాంకం v : +it) {
    hb_always_assert (v == i++);
  }
}

స్థిర శూన్య test_concat ()
{
  hb_సదిశ_t<పూర్ణాంకం> a = {1, 2, 3};
  hb_సదిశ_t<పూర్ణాంకం> b = {4, 5};

  hb_సదిశ_t<పూర్ణాంకం> c = {};
  hb_సదిశ_t<పూర్ణాంకం> d = {1, 2, 3, 4, 5};

  స్వయంచాలక it1 = hb_concat (a, b);
  hb_always_assert (it1.len () == 5);
  hb_always_assert (it1.is_random_access_iterator);
  స్వయంచాలక it2 = hb_concat (c, d);
  hb_always_assert (it2.len () == 5);
  స్వయంచాలక it3 = hb_concat (d, c);
  hb_always_assert (it3.len () == 5);
  కొరకు (పూర్ణాంకం i = 0; i < 5; i++) {
    hb_always_assert(it1[i] == i + 1);
    hb_always_assert(it2[i] == i + 1);
    hb_always_assert(it3[i] == i + 1);
  }

  check_sequential (it1);
  check_sequential (it2);
  check_sequential (it3);

  స్వయంచాలక it4 = +it1;
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

  స్వయంచాలక it5 = +it1;
  it5 += 3;
  hb_always_assert (*it5 == 4);

  hb_సమితి_t s_a = {1, 2, 3};
  hb_సమితి_t s_b = {4, 5};
  స్వయంచాలక it6 = hb_concat (s_a, s_b);
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

స్థిర శూన్య
test_backward_filter ()
{
  hb_సదిశ_t<పూర్ణాంకం> V = {2, 4, 5, 7, 8, 9, 10};
  స్వయంచాలక it = hb_iter (V) | hb_filter ([] (పూర్ణాంకం x) { తిరిగి x % 2 != 0; });
  it += 2;
  hb_always_assert (*it == 9);
  --it;
  hb_always_assert (*it == 7);
  --it;
  hb_always_assert (*it == 5);
  --it;
  hb_always_assert (*it == 5);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  స్థిరరాశి పూర్ణాంకం src[10] = {};
  పూర్ణాంకం dst[20];
  hb_సదిశ_t<పూర్ణాంకం> v;

  array_iter_t<స్థిరరాశి పూర్ణాంకం> s (src); /* Implicit conversion from static array. */
  array_iter_t<స్థిరరాశి పూర్ణాంకం> s2 (v); /* Implicit conversion from vector. */
  array_iter_t<పూర్ణాంకం> t (dst);

  static_assert (array_iter_t<పూర్ణాంకం>::is_random_access_iterator, "");

  some_array_t<స్థిరరాశి పూర్ణాంకం> a (src);

  s2 = s;

  hb_iter (src);
  hb_iter (src, 2);

  hb_fill (t, 42);
  hb_copy (s, t);
  hb_copy (a.iter (), t);

  test_iterable (v);
  hb_సమితి_t st;
  st << 1 << 15 << 43;
  test_iterable (st);
  hb_sorted_array_t<పూర్ణాంకం> sa;
  (శూన్య) static_cast<hb_iter_t<hb_sorted_array_t<పూర్ణాంకం>, hb_sorted_array_t<పూర్ణాంకం>::అంశం_t>&> (sa);
  (శూన్య) static_cast<hb_iter_t<hb_sorted_array_t<పూర్ణాంకం>, hb_sorted_array_t<పూర్ణాంకం>::__item_t__>&> (sa);
  (శూన్య) static_cast<hb_iter_t<hb_sorted_array_t<పూర్ణాంకం>, పూర్ణాంకం&>&>(sa);
  (శూన్య) static_cast<hb_iter_t<hb_sorted_array_t<పూర్ణాంకం>>&>(sa);
  (శూన్య) static_cast<hb_iter_t<hb_array_t<పూర్ణాంకం>, పూర్ణాంకం&>&> (sa);
  test_iterable (sa);

  test_iterable<hb_array_t<పూర్ణాంకం>> ();
  test_iterable<hb_sorted_array_t<స్థిరరాశి పూర్ణాంకం>> ();
  test_iterable<hb_సదిశ_t<దశాంశం>> ();
  test_iterable<hb_సమితి_t> ();
  test_iterable<OT::Array16_యొక్క<OT::HBUINT16>> ();

  test_iterator (hb_zip (st, v));
  test_iterator_non_default_constructable (hb_enumerate (st));
  test_iterator_non_default_constructable (hb_enumerate (st, -5));
  test_iterator_non_default_constructable (hb_enumerate (hb_iter (st)));
  test_iterator_non_default_constructable (hb_enumerate (hb_iter (st) + 1));
  test_iterator_non_default_constructable (hb_iter (st) | hb_filter ());
  test_iterator_non_default_constructable (hb_iter (st) | hb_map (hb_lidentity));

  hb_always_assert (నిజం == hb_all (st));
  hb_always_assert (అబద్ధం == hb_all (st, 42u));
  hb_always_assert (నిజం == hb_any (st));
  hb_always_assert (అబద్ధం == hb_any (st, 14u));
  hb_always_assert (నిజం == hb_any (st, 14u, [] (గుర్తులేని _) { తిరిగి _ - 1u; }));
  hb_always_assert (నిజం == hb_any (st, [] (గుర్తులేని _) { తిరిగి _ == 15u; }));
  hb_always_assert (నిజం == hb_any (st, 15u));
  hb_always_assert (అబద్ధం == hb_none (st));
  hb_always_assert (అబద్ధం == hb_none (st, 15u));
  hb_always_assert (నిజం == hb_none (st, 17u));

  hb_array_t<hb_సదిశ_t<పూర్ణాంకం>> pa;
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
  | hb_filter (hb_bool, hb_తటస్థం)
  | hb_sink (st)
  ;

  + hb_iter (src)
  | hb_sink (hb_array (dst))
  ;

  + hb_iter (src)
  | hb_apply (&st)
  ;

  + hb_iter (src)
  | hb_map ([] (పూర్ణాంకం i) { తిరిగి 1; })
  | hb_reduce ([=] (పూర్ణాంకం acc, పూర్ణాంకం value) { తిరిగి acc; }, 2)
  ;

  ఉపయోగించి map_pair_t = hb_అంశం_type<hb_map_t>;
  + hb_iter (m)
  | hb_map ([] (map_pair_t p) { తిరిగి p.మొదటి * p.second; })
  ;

  m.కీలకాలు ();
  ఉపయోగించి map_కీలకం_t = decltype (*m.కీలకాలు());
  + hb_iter (m.కీలకాలు ())
  | hb_filter ([] (map_కీలకం_t k) { తిరిగి k < 42; })
  | hb_drain
  ;

  m.values ();
  ఉపయోగించి map_విలువ_t = decltype (*m.values());
  + hb_iter (m.values ())
  | hb_filter ([] (map_విలువ_t k) { తిరిగి k < 42; })
  | hb_drain
  ;

  గుర్తులేని పూర్ణాంకం temp1 = 10;
  గుర్తులేని పూర్ణాంకం temp2 = 0;
  hb_map_t *ఫలితం =
  + hb_iter (src)
  | hb_map ([&] (పూర్ణాంకం i) -> hb_సమితి_t *
	    {
	      hb_సమితి_t *సమితి = hb_సమితి_create ();
	      కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < temp1; ++i)
		hb_సమితి_కూడు (సమితి, i);
	      temp1++;
	      తిరిగి సమితి;
	    })
  | hb_reduce ([&] (hb_map_t *acc, hb_సమితి_t *value) -> hb_map_t *
	       {
		 hb_map_సమితి (acc, temp2++, hb_సమితి_get_population (value));
		 /* This is not a memory managed language, take care! */
		 hb_సమితి_destroy (value);
		 తిరిగి acc;
	       }, hb_map_create ())
  ;
  /* The result should be something like 0->10, 1->11, ..., 9->19 */
  hb_always_assert (hb_map_get (ఫలితం, 9) == 19);
  hb_map_destroy (ఫలితం);

  /* Like above, but passing hb_set_t instead of hb_set_t * */
  temp1 = 10;
  temp2 = 0;
  ఫలితం =
  + hb_iter (src)
  | hb_map ([&] (పూర్ణాంకం i) -> hb_సమితి_t
	    {
	      hb_సమితి_t సమితి;
	      కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < temp1; ++i)
		hb_సమితి_కూడు (&సమితి, i);
	      temp1++;
	      తిరిగి సమితి;
	    })
  | hb_reduce ([&] (hb_map_t *acc, hb_సమితి_t value) -> hb_map_t *
	       {
		 hb_map_సమితి (acc, temp2++, hb_సమితి_get_population (&value));
		 తిరిగి acc;
	       }, hb_map_create ())
  ;
  /* The result should be something like 0->10, 1->11, ..., 9->19 */
  hb_always_assert (hb_map_get (ఫలితం, 9) == 19);
  hb_map_destroy (ఫలితం);

  గుర్తులేని పూర్ణాంకం temp3 = 0;
  + hb_iter(src)
  | hb_map([&] (పూర్ణాంకం i) { తిరిగి ++temp3; })
  | hb_reduce([&] (దశాంశం acc, పూర్ణాంకం value) { తిరిగి acc + value; }, 0)
  ;

  + hb_iter (src)
  | hb_drain
  ;

  t << 1;
  దీర్ఘ vl;
  s >> vl;

  hb_iota ();
  hb_iota (3);
  hb_iota (3, 2);
  hb_always_assert ((&vl) + 1 == *++hb_iota (&vl, hb_inc));
  hb_range ();
  hb_repeat (7u);
  hb_repeat (శూన్యసూచిక);
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

  తిరిగి 0;
}

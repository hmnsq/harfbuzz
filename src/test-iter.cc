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

#समावेश "hb.hh"
#समावेश "hb-iter.hh"

#समावेश "hb-array.hh"
#समावेश "hb-set.hh"
#समावेश "hb-ot-layout-common.hh"

ढाँचा <typename T>
काष्ठा array_iter_t : hb_iter_with_fallback_t<array_iter_t<T>, T&>
{
  array_iter_t (hb_array_t<T> arr_) : arr (arr_) {}

  प्रकार T& __item_t__;
  अटल constexpr bool is_random_access_iterator = true;
  T& __item_at__ (अचिन्हित i) स्थिर { वापस arr[i]; }
  व्योम __forward__ (अचिन्हित n) { arr += n; }
  व्योम __rewind__ (अचिन्हित n) { arr -= n; }
  अचिन्हित __len__ () स्थिर { वापस arr.लंबाई; }
  bool चालक != (स्थिर array_iter_t& o) { वापस arr != o.arr; }

  निजी:
  hb_array_t<T> arr;
};

ढाँचा <typename T>
काष्ठा some_array_t
{
  some_array_t (hb_array_t<T> arr_) : arr (arr_) {}

  प्रकार array_iter_t<T> iter_t;
  array_iter_t<T> iter () { वापस array_iter_t<T> (arr); }
  चालक array_iter_t<T> () { वापस iter (); }
  चालक hb_iter_t<array_iter_t<T>> () { वापस iter (); }

  निजी:
  hb_array_t<T> arr;
};


ढाँचा <typename Iter,
	  hb_requires (hb_is_iterator (Iter))>
अटल व्योम
test_iterator_non_default_constructable (Iter it)
{
  /* Iterate over a copy of it. */
  क्रम (स्वतः c = it.iter (); c; c++)
    *c;

  /* Same. */
  क्रम (स्वतः c = +it; c; c++)
    *c;

  /* Range-based for over a copy. */
  क्रम (स्वतः _ : +it)
    (व्योम) _;

  it += it.len ();
  it = it + 10;
  it = 10 + it;

  hb_always_assert (*it == it[0]);

  static_assert (true || it.is_random_access_iterator, "");
  static_assert (true || it.is_sorted_iterator, "");
}

ढाँचा <typename Iter,
	  hb_requires (hb_is_iterator (Iter))>
अटल व्योम
test_iterator (Iter it)
{
  Iter default_constructed;
  hb_always_assert (!default_constructed);

  test_iterator_non_default_constructable (it);
}

ढाँचा <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
अटल व्योम
test_iterable (स्थिर Iterable &lst = Null (Iterable))
{
  क्रम (स्वतः _ : lst)
    (व्योम) _;

  // Test that can take iterator from.
  test_iterator (lst.iter ());
}

ढाँचा <typename It>
अटल व्योम check_sequential (It it)
{
  पूर्णांक i = 1;
  क्रम (पूर्णांक v : +it) {
    hb_always_assert (v == i++);
  }
}

अटल व्योम test_concat ()
{
  hb_vector_t<पूर्णांक> a = {1, 2, 3};
  hb_vector_t<पूर्णांक> b = {4, 5};

  hb_vector_t<पूर्णांक> c = {};
  hb_vector_t<पूर्णांक> d = {1, 2, 3, 4, 5};

  स्वतः it1 = hb_concat (a, b);
  hb_always_assert (it1.len () == 5);
  hb_always_assert (it1.is_random_access_iterator);
  स्वतः it2 = hb_concat (c, d);
  hb_always_assert (it2.len () == 5);
  स्वतः it3 = hb_concat (d, c);
  hb_always_assert (it3.len () == 5);
  क्रम (पूर्णांक i = 0; i < 5; i++) {
    hb_always_assert(it1[i] == i + 1);
    hb_always_assert(it2[i] == i + 1);
    hb_always_assert(it3[i] == i + 1);
  }

  check_sequential (it1);
  check_sequential (it2);
  check_sequential (it3);

  स्वतः it4 = +it1;
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

  स्वतः it5 = +it1;
  it5 += 3;
  hb_always_assert (*it5 == 4);

  hb_set_t s_a = {1, 2, 3};
  hb_set_t s_b = {4, 5};
  स्वतः it6 = hb_concat (s_a, s_b);
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

अटल व्योम
test_backward_filter ()
{
  hb_vector_t<पूर्णांक> V = {2, 4, 5, 7, 8, 9, 10};
  स्वतः it = hb_iter (V) | hb_filter ([] (पूर्णांक x) { वापस x % 2 != 0; });
  it += 2;
  hb_always_assert (*it == 9);
  --it;
  hb_always_assert (*it == 7);
  --it;
  hb_always_assert (*it == 5);
  --it;
  hb_always_assert (*it == 5);
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  स्थिर पूर्णांक src[10] = {};
  पूर्णांक dst[20];
  hb_vector_t<पूर्णांक> v;

  array_iter_t<स्थिर पूर्णांक> s (src); /* Implicit conversion from static array. */
  array_iter_t<स्थिर पूर्णांक> s2 (v); /* Implicit conversion from vector. */
  array_iter_t<पूर्णांक> t (dst);

  static_assert (array_iter_t<पूर्णांक>::is_random_access_iterator, "");

  some_array_t<स्थिर पूर्णांक> a (src);

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
  hb_sorted_array_t<पूर्णांक> sa;
  (व्योम) static_cast<hb_iter_t<hb_sorted_array_t<पूर्णांक>, hb_sorted_array_t<पूर्णांक>::item_t>&> (sa);
  (व्योम) static_cast<hb_iter_t<hb_sorted_array_t<पूर्णांक>, hb_sorted_array_t<पूर्णांक>::__item_t__>&> (sa);
  (व्योम) static_cast<hb_iter_t<hb_sorted_array_t<पूर्णांक>, पूर्णांक&>&>(sa);
  (व्योम) static_cast<hb_iter_t<hb_sorted_array_t<पूर्णांक>>&>(sa);
  (व्योम) static_cast<hb_iter_t<hb_array_t<पूर्णांक>, पूर्णांक&>&> (sa);
  test_iterable (sa);

  test_iterable<hb_array_t<पूर्णांक>> ();
  test_iterable<hb_sorted_array_t<स्थिर पूर्णांक>> ();
  test_iterable<hb_vector_t<भग्न>> ();
  test_iterable<hb_set_t> ();
  test_iterable<OT::Array16Of<OT::HBUINT16>> ();

  test_iterator (hb_zip (st, v));
  test_iterator_non_default_constructable (hb_enumerate (st));
  test_iterator_non_default_constructable (hb_enumerate (st, -5));
  test_iterator_non_default_constructable (hb_enumerate (hb_iter (st)));
  test_iterator_non_default_constructable (hb_enumerate (hb_iter (st) + 1));
  test_iterator_non_default_constructable (hb_iter (st) | hb_filter ());
  test_iterator_non_default_constructable (hb_iter (st) | hb_map (hb_lidentity));

  hb_always_assert (true == hb_all (st));
  hb_always_assert (false == hb_all (st, 42u));
  hb_always_assert (true == hb_any (st));
  hb_always_assert (false == hb_any (st, 14u));
  hb_always_assert (true == hb_any (st, 14u, [] (अचिन्हित _) { वापस _ - 1u; }));
  hb_always_assert (true == hb_any (st, [] (अचिन्हित _) { वापस _ == 15u; }));
  hb_always_assert (true == hb_any (st, 15u));
  hb_always_assert (false == hb_none (st));
  hb_always_assert (false == hb_none (st, 15u));
  hb_always_assert (true == hb_none (st, 17u));

  hb_array_t<hb_vector_t<पूर्णांक>> pa;
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
  | hb_map ([] (पूर्णांक i) { वापस 1; })
  | hb_reduce ([=] (पूर्णांक acc, पूर्णांक value) { वापस acc; }, 2)
  ;

  using map_pair_t = hb_item_type<hb_map_t>;
  + hb_iter (m)
  | hb_map ([] (map_pair_t p) { वापस p.first * p.second; })
  ;

  m.कुंजियाँ ();
  using map_कुंजी_t = decltype (*m.कुंजियाँ());
  + hb_iter (m.कुंजियाँ ())
  | hb_filter ([] (map_कुंजी_t k) { वापस k < 42; })
  | hb_drain
  ;

  m.values ();
  using map_मूल्य_t = decltype (*m.values());
  + hb_iter (m.values ())
  | hb_filter ([] (map_मूल्य_t k) { वापस k < 42; })
  | hb_drain
  ;

  अचिन्हित पूर्णांक temp1 = 10;
  अचिन्हित पूर्णांक temp2 = 0;
  hb_map_t *परिणाम =
  + hb_iter (src)
  | hb_map ([&] (पूर्णांक i) -> hb_set_t *
	    {
	      hb_set_t *set = hb_set_create ();
	      क्रम (अचिन्हित पूर्णांक i = 0; i < temp1; ++i)
		hb_set_add (set, i);
	      temp1++;
	      वापस set;
	    })
  | hb_reduce ([&] (hb_map_t *acc, hb_set_t *value) -> hb_map_t *
	       {
		 hb_map_set (acc, temp2++, hb_set_get_population (value));
		 /* This is not a memory managed language, take care! */
		 hb_set_destroy (value);
		 वापस acc;
	       }, hb_map_create ())
  ;
  /* The result should be something like 0->10, 1->11, ..., 9->19 */
  hb_always_assert (hb_map_get (परिणाम, 9) == 19);
  hb_map_destroy (परिणाम);

  /* Like above, but passing hb_set_t instead of hb_set_t * */
  temp1 = 10;
  temp2 = 0;
  परिणाम =
  + hb_iter (src)
  | hb_map ([&] (पूर्णांक i) -> hb_set_t
	    {
	      hb_set_t set;
	      क्रम (अचिन्हित पूर्णांक i = 0; i < temp1; ++i)
		hb_set_add (&set, i);
	      temp1++;
	      वापस set;
	    })
  | hb_reduce ([&] (hb_map_t *acc, hb_set_t value) -> hb_map_t *
	       {
		 hb_map_set (acc, temp2++, hb_set_get_population (&value));
		 वापस acc;
	       }, hb_map_create ())
  ;
  /* The result should be something like 0->10, 1->11, ..., 9->19 */
  hb_always_assert (hb_map_get (परिणाम, 9) == 19);
  hb_map_destroy (परिणाम);

  अचिन्हित पूर्णांक temp3 = 0;
  + hb_iter(src)
  | hb_map([&] (पूर्णांक i) { वापस ++temp3; })
  | hb_reduce([&] (भग्न acc, पूर्णांक value) { वापस acc + value; }, 0)
  ;

  + hb_iter (src)
  | hb_drain
  ;

  t << 1;
  दीर्घ vl;
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

  वापस 0;
}

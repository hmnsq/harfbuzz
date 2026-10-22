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

#ifndef HB_ARRAY_HH
#define HB_ARRAY_HH

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-algs.hh"
#অন্তর্ভাব "hb-iter.hh"
#অন্তর্ভাব "hb-null.hh"


template <typename Type>
সংরচনা hb_sorted_array_t;

গণনা hb_not_found_t
{
  HB_NOT_FOUND_DONT_STORE,
  HB_NOT_FOUND_STORE,
  HB_NOT_FOUND_STORE_CLOSEST,
};


template <typename Type>
সংরচনা hb_array_t : hb_iter_with_fallback_t<hb_array_t<Type>, Type&>
{
  স্থির constexpr তার্কিক realloc_move = সত্য;

  /*
   * Constructors.
   */
  hb_array_t () = স্বভাব;
  hb_array_t (ধ্রুব hb_array_t&) = স্বভাব;
  ~hb_array_t () = স্বভাব;
  hb_array_t& operator= (ধ্রুব hb_array_t&) = স্বভাব;
  hb_array_t& operator= (hb_array_t&&) = স্বভাব;

  constexpr hb_array_t (Type *array_, unsigned পূর্ণাঙ্ক length_) : arrayZ (array_), length (length_) {}
  template <unsigned পূর্ণাঙ্ক length_>
  constexpr hb_array_t (Type (&array_)[length_]) : hb_array_t (array_, length_) {}

  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_array_t (ধ্রুব hb_array_t<U> &o) :
    hb_iter_with_fallback_t<hb_array_t, Type&> (),
    arrayZ (o.arrayZ), length (o.length), backwards_length (o.backwards_length) {}
  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_array_t& operator = (ধ্রুব hb_array_t<U> &o)
  { arrayZ = o.arrayZ; length = o.length; backwards_length = o.backwards_length; প্রত্যাগম *এতত্; }

  /*
   * Iterator implementation.
   */
  প্রকারনির্দেশ Type& __item_t__;
  স্থির constexpr তার্কিক is_random_access_iterator = সত্য;
  স্থির constexpr তার্কিক has_fast_len = সত্য;
  Type& __item__ () ধ্রুব
  {
    যদি (unlikely (!length)) প্রত্যাগম CrapOrNull (Type);
    প্রত্যাগম *arrayZ;
  }
  Type& __item_at__ (unsigned i) ধ্রুব
  {
    যদি (unlikely (i >= length)) প্রত্যাগম CrapOrNull (Type);
    প্রত্যাগম arrayZ[i];
  }
  শূন্য __next__ ()
  {
    যদি (unlikely (!length))
      প্রত্যাগম;
    length--;
    backwards_length++;
    arrayZ++;
  }
  শূন্য __forward__ (unsigned n)
  {
    যদি (unlikely (n > length))
      n = length;
    length -= n;
    backwards_length += n;
    arrayZ += n;
  }
  শূন্য __prev__ ()
  {
    যদি (unlikely (!backwards_length))
      প্রত্যাগম;
    length++;
    backwards_length--;
    arrayZ--;
  }
  শূন্য __rewind__ (unsigned n)
  {
    যদি (unlikely (n > backwards_length))
      n = backwards_length;
    length += n;
    backwards_length -= n;
    arrayZ -= n;
  }
  unsigned __len__ () ধ্রুব { প্রত্যাগম length; }
  /* Ouch. The operator== compares the contents of the array.  For range-based for loops,
   * it's best if we can just compare arrayZ, though comparing contents is still fast,
   * but also would require that Type has operator==.  As such, we optimize this operator
   * for range-based for loop and just compare arrayZ and length.
   *
   * The above comment is outdated now because we implemented separate begin/end to
   * objects that were using hb_array_t for range-based loop before. */
  তার্কিক operator != (ধ্রুব hb_array_t& o) ধ্রুব
  { প্রত্যাগম এতত্->arrayZ != o.arrayZ || এতত্->length != o.length; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () ধ্রুব { প্রত্যাগম arrayZ; }
  Type *end () ধ্রুব { প্রত্যাগম arrayZ + length; }


  /* Extra operators.
   */
  Type * operator & () ধ্রুব { প্রত্যাগম arrayZ; }
  operator hb_array_t<ধ্রুব Type> () { প্রত্যাগম hb_array_t<ধ্রুব Type> (arrayZ, length); }
  template <typename T> operator T * () ধ্রুব { প্রত্যাগম arrayZ; }

  HB_INTERNAL তার্কিক operator == (ধ্রুব hb_array_t &o) ধ্রুব;

  uint32_t hash () ধ্রুব
  {
    // FNV-1a hash function
    // https://github.com/harfbuzz/harfbuzz/pull/4228
    uint32_t current = /*cbf29ce4*/0x84222325;
    প্রতি (auto &v : *এতত্)
    {
      current = current ^ hb_hash (v);
      current = current * 16777619;
    }
    প্রত্যাগম current;
  }

  /*
   * Compare, Sort, and Search.
   */

  /* Note: our compare is NOT lexicographic; it also does NOT call Type::cmp. */
  পূর্ণাঙ্ক cmp (ধ্রুব hb_array_t &a) ধ্রুব
  {
    যদি (length != a.length)
      প্রত্যাগম (পূর্ণাঙ্ক) a.length - (পূর্ণাঙ্ক) length;
    প্রত্যাগম hb_memcmp (a.arrayZ, arrayZ, get_size ());
  }
  HB_INTERNAL স্থির পূর্ণাঙ্ক cmp (ধ্রুব শূন্য *pa, ধ্রুব শূন্য *pb)
  {
    hb_array_t *a = (hb_array_t *) pa;
    hb_array_t *b = (hb_array_t *) pb;
    প্রত্যাগম b->cmp (*a);
  }

  template <typename T>
  Type *lsearch (ধ্রুব T &x, Type *not_found = nullptr)
  {
    unsigned i;
    প্রত্যাগম lfind (x, &i) ? &এতত্->arrayZ[i] : not_found;
  }
  template <typename T>
  ধ্রুব Type *lsearch (ধ্রুব T &x, ধ্রুব Type *not_found = nullptr) ধ্রুব
  {
    unsigned i;
    প্রত্যাগম lfind (x, &i) ? &এতত্->arrayZ[i] : not_found;
  }
  template <typename T>
  তার্কিক lfind (ধ্রুব T &x, unsigned *pos = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned পূর্ণাঙ্ক to_store = (unsigned পূর্ণাঙ্ক) -1) ধ্রুব
  {
    প্রতি (unsigned i = 0; i < length; ++i)
      যদি (hb_equal (x, এতত্->arrayZ[i]))
      {
	যদি (pos)
	  *pos = i;
	প্রত্যাগম সত্য;
      }

    যদি (pos)
    {
      বিকল্প (not_found)
      {
	পক্ষ HB_NOT_FOUND_DONT_STORE:
	  ভঙ্গ;

	পক্ষ HB_NOT_FOUND_STORE:
	  *pos = to_store;
	  ভঙ্গ;

	পক্ষ HB_NOT_FOUND_STORE_CLOSEST:
	  *pos = length;
	  ভঙ্গ;
      }
    }
    প্রত্যাগম অসত্য;
  }

  hb_sorted_array_t<Type> qsort (পূর্ণাঙ্ক (*cmp)(ধ্রুব শূন্য*, ধ্রুব শূন্য*))
  {
    যদি (likely (length))
      hb_qsort (arrayZ, length, এতত্->get_item_size (), cmp);
    প্রত্যাগম hb_sorted_array_t<Type> (*এতত্);
  }

  /* Comparator follows the C qsort convention: returns
   * negative / zero / positive int. */
  template <typename Compar>
  hb_sorted_array_t<Type> qsort (Compar compar)
  {
    যদি (likely (length))
      hb_qsort_inline (arrayZ, length, compar);
    প্রত্যাগম hb_sorted_array_t<Type> (*এতত্);
  }

  গুপ্ত:
  template <typename T = Type,
	    hb_enable_if (std::is_move_assignable<T>::value)>
  hb_sorted_array_t<Type> _qsort (hb_priority<1>)
  {
    প্রত্যাগম qsort ([] (ধ্রুব Type &a, ধ্রুব Type &b) { প্রত্যাগম Type::cmp (&a, &b); });
  }
  hb_sorted_array_t<Type> _qsort (hb_priority<0>)
  {
    প্রত্যাগম qsort ((পূর্ণাঙ্ক(*)(ধ্রুব শূন্য*, ধ্রুব শূন্য*)) Type::cmp);
  }
  সার্বজনিক:

  hb_sorted_array_t<Type> qsort ()
  {
    প্রত্যাগম _qsort (hb_prioritize);
  }

  /*
   * Other methods.
   */

  size_t get_size () ধ্রুব { প্রত্যাগম length * এতত্->get_item_size (); }

  /*
   * Reverse the order of items in this array in the range [start, end).
   */
  শূন্য reverse (unsigned start = 0, unsigned end = -1)
  {
    start = hb_min (start, length);
    end = hb_min (end, length);

    যদি (end < start + 2)
      প্রত্যাগম;

    unsigned stop = start + (end - start) / 2;
    প্রতি (unsigned lhs = start, rhs = end - 1; lhs < stop; lhs++, rhs--)
      hb_swap (arrayZ[rhs], arrayZ[lhs]);
  }

  hb_array_t sub_array (unsigned পূর্ণাঙ্ক start_offset = 0, unsigned পূর্ণাঙ্ক *seg_গুনতি = nullptr /* IN/OUT */) ধ্রুব
  {
    যদি (!start_offset && !seg_গুনতি)
      প্রত্যাগম *এতত্;

    unsigned পূর্ণাঙ্ক গুনতি = length;
    যদি (unlikely (start_offset > গুনতি))
      গুনতি = 0;
    অন্যথা
      গুনতি -= start_offset;
    যদি (seg_গুনতি)
      গুনতি = *seg_গুনতি = hb_min (গুনতি, *seg_গুনতি);
    প্রত্যাগম hb_array_t (arrayZ + start_offset, গুনতি);
  }
  hb_array_t sub_array (unsigned পূর্ণাঙ্ক start_offset, unsigned পূর্ণাঙ্ক seg_গুনতি) ধ্রুব
  { প্রত্যাগম sub_array (start_offset, &seg_গুনতি); }

  hb_array_t truncate (unsigned length) ধ্রুব { প্রত্যাগম sub_array (0, length); }

  template <typename T,
	    unsigned P = পরিমাণ (Type),
	    hb_enable_if (P == 1)>
  ধ্রুব T *as () ধ্রুব
  { প্রত্যাগম length < hb_min_size (T) ? &Null (T) : reinterpret_cast<ধ্রুব T *> (arrayZ); }

  template <typename T,
	    unsigned P = পরিমাণ (Type),
	    hb_enable_if (P == 1)>
  তার্কিক check_range (ধ্রুব T *p, unsigned পূর্ণাঙ্ক size = T::static_size) ধ্রুব
  {
    প্রত্যাগম arrayZ <= ((ধ্রুব বর্ণ *) p)
	&& ((ধ্রুব বর্ণ *) p) <= arrayZ + length
	&& (unsigned পূর্ণাঙ্ক) (arrayZ + length - (ধ্রুব বর্ণ *) p) >= size;
  }

  template <unsigned P = পরিমাণ (Type),
	    hb_enable_if (P == 1)>
  তার্কিক check_end (ধ্রুব শূন্য *p) ধ্রুব
  {
    প্রত্যাগম (uintptr_t) (((ধ্রুব বর্ণ *) p) - arrayZ) <= length;
  }

  /* Only call if you allocated the underlying array using hb_malloc() or similar. */
  শূন্য fini ()
  { hb_free ((শূন্য *) arrayZ); arrayZ = nullptr; length = 0; }

  template <typename hb_serialize_context_t,
	    typename U = Type,
	    hb_enable_if (!(পরিমাণ (U) < পরিমাণ (long long) && hb_is_trivially_copy_assignable(hb_decay<Type>)))>
  hb_array_t copy (hb_serialize_context_t *c) ধ্রুব
  {
    TRACE_SERIALIZE (এতত্);
    auto* out = c->start_embed (arrayZ);
    যদি (unlikely (!c->extend_size (out, get_size (), অসত্য))) return_trace (hb_array_t ());
    প্রতি (unsigned i = 0; i < length; i++)
      out[i] = arrayZ[i]; /* TODO: add version that calls c->copy() */
    return_trace (hb_array_t (out, length));
  }

  template <typename hb_serialize_context_t,
	    typename U = Type,
	    hb_enable_if (পরিমাণ (U) < পরিমাণ (long long) && hb_is_trivially_copy_assignable(hb_decay<Type>))>
  hb_array_t copy (hb_serialize_context_t *c) ধ্রুব
  {
    TRACE_SERIALIZE (এতত্);
    auto* out = c->start_embed (arrayZ);
    যদি (unlikely (!c->extend_size (out, get_size (), অসত্য))) return_trace (hb_array_t ());
    hb_memcpy (out, arrayZ, get_size ());
    return_trace (hb_array_t (out, length));
  }

  template <typename hb_sanitize_context_t>
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  { প্রত্যাগম c->check_array (arrayZ, length); }

  /*
   * Members
   */

  সার্বজনিক:
  Type *arrayZ = nullptr;
  unsigned পূর্ণাঙ্ক length = 0;
  unsigned পূর্ণাঙ্ক backwards_length = 0;
};
template <typename T> অন্তঃস্থ hb_array_t<T>
hb_array ()
{ প্রত্যাগম hb_array_t<T> (); }
template <typename T> অন্তঃস্থ hb_array_t<T>
hb_array (T *array, unsigned পূর্ণাঙ্ক length)
{ প্রত্যাগম hb_array_t<T> (array, length); }
template <typename T, unsigned পূর্ণাঙ্ক length_> অন্তঃস্থ hb_array_t<T>
hb_array (T (&array_)[length_])
{ প্রত্যাগম hb_array_t<T> (array_); }

template <typename Type>
সংরচনা hb_sorted_array_t :
	hb_array_t<Type>,
	hb_iter_t<hb_sorted_array_t<Type>, Type&>
{
  প্রকারনির্দেশ hb_iter_t<hb_sorted_array_t, Type&> iter_base_t;
  HB_ITER_USING (iter_base_t);
  স্থির constexpr তার্কিক is_random_access_iterator = সত্য;
  স্থির constexpr তার্কিক is_sorted_iterator = সত্য;
  স্থির constexpr তার্কিক has_fast_len = সত্য;

  hb_sorted_array_t () = স্বভাব;
  hb_sorted_array_t (ধ্রুব hb_sorted_array_t&) = স্বভাব;
  ~hb_sorted_array_t () = স্বভাব;
  hb_sorted_array_t& operator= (ধ্রুব hb_sorted_array_t&) = স্বভাব;
  hb_sorted_array_t& operator= (hb_sorted_array_t&&) = স্বভাব;

  constexpr hb_sorted_array_t (Type *array_, unsigned পূর্ণাঙ্ক length_) : hb_array_t<Type> (array_, length_) {}
  template <unsigned পূর্ণাঙ্ক length_>
  constexpr hb_sorted_array_t (Type (&array_)[length_]) : hb_array_t<Type> (array_) {}

  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_sorted_array_t (ধ্রুব hb_array_t<U> &o) :
    hb_array_t<Type> (o),
    hb_iter_t<hb_sorted_array_t, Type&> () {}
  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_sorted_array_t& operator = (ধ্রুব hb_array_t<U> &o)
  { hb_array_t<Type> (*এতত্) = o; প্রত্যাগম *এতত্; }

  /* Iterator implementation. */

  /* See comment in hb_array_of::operator != */
  তার্কিক operator != (ধ্রুব hb_sorted_array_t& o) ধ্রুব
  { প্রত্যাগম এতত্->arrayZ != o.arrayZ || এতত্->length != o.length; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () ধ্রুব { প্রত্যাগম এতত্->arrayZ; }
  Type *end () ধ্রুব { প্রত্যাগম এতত্->arrayZ + এতত্->length; }


  hb_sorted_array_t sub_array (unsigned পূর্ণাঙ্ক start_offset, unsigned পূর্ণাঙ্ক *seg_গুনতি /* IN/OUT */) ধ্রুব
  { প্রত্যাগম hb_sorted_array_t (((ধ্রুব hb_array_t<Type> *) (এতত্))->sub_array (start_offset, seg_গুনতি)); }
  hb_sorted_array_t sub_array (unsigned পূর্ণাঙ্ক start_offset, unsigned পূর্ণাঙ্ক seg_গুনতি) ধ্রুব
  { প্রত্যাগম sub_array (start_offset, &seg_গুনতি); }

  hb_sorted_array_t truncate (unsigned length) ধ্রুব { প্রত্যাগম sub_array (0, length); }

  template <typename T>
  Type *bsearch (ধ্রুব T &x, Type *not_found = nullptr)
  {
    unsigned পূর্ণাঙ্ক i;
    প্রত্যাগম bfind (x, &i) ? &এতত্->arrayZ[i] : not_found;
  }
  template <typename T>
  ধ্রুব Type *bsearch (ধ্রুব T &x, ধ্রুব Type *not_found = nullptr) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক i;
    প্রত্যাগম bfind (x, &i) ? &এতত্->arrayZ[i] : not_found;
  }
  template <typename T>
  তার্কিক bfind (ধ্রুব T &x, unsigned পূর্ণাঙ্ক *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned পূর্ণাঙ্ক to_store = (unsigned পূর্ণাঙ্ক) -1) ধ্রুব
  {
    unsigned pos;

    যদি (bsearch_impl (x, &pos))
    {
      যদি (i)
	*i = pos;
      প্রত্যাগম সত্য;
    }

    যদি (i)
    {
      বিকল্প (not_found)
      {
	পক্ষ HB_NOT_FOUND_DONT_STORE:
	  ভঙ্গ;

	পক্ষ HB_NOT_FOUND_STORE:
	  *i = to_store;
	  ভঙ্গ;

	পক্ষ HB_NOT_FOUND_STORE_CLOSEST:
	  *i = pos;
	  ভঙ্গ;
      }
    }
    প্রত্যাগম অসত্য;
  }
  template <typename T, typename ...Ts>
  তার্কিক bsearch_impl (ধ্রুব T &x, unsigned *pos, Ts... ds) ধ্রুব
  {
    প্রত্যাগম hb_bsearch_impl (pos,
			    x,
			    এতত্->arrayZ,
			    এতত্->length,
			    পরিমাণ (Type),
			    _hb_cmp_method<T, Type, Ts...>,
			    std::forward<Ts> (ds)...);
  }
};
template <typename T> অন্তঃস্থ hb_sorted_array_t<T>
hb_sorted_array (T *array, unsigned পূর্ণাঙ্ক length)
{ প্রত্যাগম hb_sorted_array_t<T> (array, length); }
template <typename T, unsigned পূর্ণাঙ্ক length_> অন্তঃস্থ hb_sorted_array_t<T>
hb_sorted_array (T (&array_)[length_])
{ প্রত্যাগম hb_sorted_array_t<T> (array_); }

template <typename T>
অন্তঃস্থ তার্কিক hb_array_t<T>::operator == (ধ্রুব hb_array_t<T> &o) ধ্রুব
{
  যদি (o.length != এতত্->length) প্রত্যাগম অসত্য;
  প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < এতত্->length; i++) {
    যদি (এতত্->arrayZ[i] != o.arrayZ[i]) প্রত্যাগম অসত্য;
  }
  প্রত্যাগম সত্য;
}
template <>
অন্তঃস্থ তার্কিক hb_array_t<ধ্রুব বর্ণ>::operator == (ধ্রুব hb_array_t<ধ্রুব বর্ণ> &o) ধ্রুব
{
  যদি (o.length != এতত্->length) প্রত্যাগম অসত্য;
  প্রত্যাগম 0 == hb_memcmp (arrayZ, o.arrayZ, length);
}
template <>
অন্তঃস্থ তার্কিক hb_array_t<ধ্রুব unsigned বর্ণ>::operator == (ধ্রুব hb_array_t<ধ্রুব unsigned বর্ণ> &o) ধ্রুব
{
  যদি (o.length != এতত্->length) প্রত্যাগম অসত্য;
  প্রত্যাগম 0 == hb_memcmp (arrayZ, o.arrayZ, length);
}


/* Specialize hash() for byte arrays. */

#ifndef HB_OPTIMIZE_SIZE_MORE
template <>
অন্তঃস্থ uint32_t hb_array_t<ধ্রুব বর্ণ>::hash () ধ্রুব
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  প্রত্যাগম fasthash32(arrayZ, length, 0xf437ffe6 /* magic? */);
}

template <>
অন্তঃস্থ uint32_t hb_array_t<ধ্রুব unsigned বর্ণ>::hash () ধ্রুব
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  প্রত্যাগম fasthash32(arrayZ, length, 0xf437ffe6 /* magic? */);
}
#endif


প্রকারনির্দেশ hb_array_t<ধ্রুব বর্ণ> hb_bytes_t;
প্রকারনির্দেশ hb_array_t<ধ্রুব unsigned বর্ণ> hb_ubytes_t;



#endif /* HB_ARRAY_HH */

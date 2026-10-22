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

#شامل "hb.hh"
#شامل "hb-algs.hh"
#شامل "hb-iter.hh"
#شامل "hb-null.hh"


template <typename Type>
ساختار hb_sorted_array_t;

شمارش hb_not_found_t
{
  HB_NOT_FOUND_DONT_STORE,
  HB_NOT_FOUND_STORE,
  HB_NOT_FOUND_STORE_CLOSEST,
};


template <typename Type>
ساختار hb_array_t : hb_iter_with_fallback_t<hb_array_t<Type>, Type&>
{
  ایستا constexpr منطقی realloc_move = درست;

  /*
   * Constructors.
   */
  hb_array_t () = پیشفرض;
  hb_array_t (ثابت hb_array_t&) = پیشفرض;
  ~hb_array_t () = پیشفرض;
  hb_array_t& operator= (ثابت hb_array_t&) = پیشفرض;
  hb_array_t& operator= (hb_array_t&&) = پیشفرض;

  constexpr hb_array_t (Type *array_, unsigned صحیح length_) : arrayZ (array_), length (length_) {}
  template <unsigned صحیح length_>
  constexpr hb_array_t (Type (&array_)[length_]) : hb_array_t (array_, length_) {}

  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_array_t (ثابت hb_array_t<U> &o) :
    hb_iter_with_fallback_t<hb_array_t, Type&> (),
    arrayZ (o.arrayZ), length (o.length), backwards_length (o.backwards_length) {}
  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_array_t& operator = (ثابت hb_array_t<U> &o)
  { arrayZ = o.arrayZ; length = o.length; backwards_length = o.backwards_length; بازگشت *این; }

  /*
   * Iterator implementation.
   */
  تعریفنوع Type& __item_t__;
  ایستا constexpr منطقی is_random_access_iterator = درست;
  ایستا constexpr منطقی has_fast_len = درست;
  Type& __item__ () ثابت
  {
    اگر (unlikely (!length)) بازگشت CrapOrNull (Type);
    بازگشت *arrayZ;
  }
  Type& __item_at__ (unsigned i) ثابت
  {
    اگر (unlikely (i >= length)) بازگشت CrapOrNull (Type);
    بازگشت arrayZ[i];
  }
  تهی __next__ ()
  {
    اگر (unlikely (!length))
      بازگشت;
    length--;
    backwards_length++;
    arrayZ++;
  }
  تهی __forward__ (unsigned n)
  {
    اگر (unlikely (n > length))
      n = length;
    length -= n;
    backwards_length += n;
    arrayZ += n;
  }
  تهی __prev__ ()
  {
    اگر (unlikely (!backwards_length))
      بازگشت;
    length++;
    backwards_length--;
    arrayZ--;
  }
  تهی __rewind__ (unsigned n)
  {
    اگر (unlikely (n > backwards_length))
      n = backwards_length;
    length += n;
    backwards_length -= n;
    arrayZ -= n;
  }
  unsigned __len__ () ثابت { بازگشت length; }
  /* Ouch. The operator== compares the contents of the array.  For range-based for loops,
   * it's best if we can just compare arrayZ, though comparing contents is still fast,
   * but also would require that Type has operator==.  As such, we optimize this operator
   * for range-based for loop and just compare arrayZ and length.
   *
   * The above comment is outdated now because we implemented separate begin/end to
   * objects that were using hb_array_t for range-based loop before. */
  منطقی operator != (ثابت hb_array_t& o) ثابت
  { بازگشت این->arrayZ != o.arrayZ || این->length != o.length; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () ثابت { بازگشت arrayZ; }
  Type *end () ثابت { بازگشت arrayZ + length; }


  /* Extra operators.
   */
  Type * operator & () ثابت { بازگشت arrayZ; }
  operator hb_array_t<ثابت Type> () { بازگشت hb_array_t<ثابت Type> (arrayZ, length); }
  template <typename T> operator T * () ثابت { بازگشت arrayZ; }

  HB_INTERNAL منطقی operator == (ثابت hb_array_t &o) ثابت;

  uint32_t hash () ثابت
  {
    // FNV-1a hash function
    // https://github.com/harfbuzz/harfbuzz/pull/4228
    uint32_t current = /*cbf29ce4*/0x84222325;
    برای (auto &v : *این)
    {
      current = current ^ hb_hash (v);
      current = current * 16777619;
    }
    بازگشت current;
  }

  /*
   * Compare, Sort, and Search.
   */

  /* Note: our compare is NOT lexicographic; it also does NOT call Type::cmp. */
  صحیح cmp (ثابت hb_array_t &a) ثابت
  {
    اگر (length != a.length)
      بازگشت (صحیح) a.length - (صحیح) length;
    بازگشت hb_memcmp (a.arrayZ, arrayZ, get_size ());
  }
  HB_INTERNAL ایستا صحیح cmp (ثابت تهی *pa, ثابت تهی *pb)
  {
    hb_array_t *a = (hb_array_t *) pa;
    hb_array_t *b = (hb_array_t *) pb;
    بازگشت b->cmp (*a);
  }

  template <typename T>
  Type *lsearch (ثابت T &x, Type *not_found = nullptr)
  {
    unsigned i;
    بازگشت lfind (x, &i) ? &این->arrayZ[i] : not_found;
  }
  template <typename T>
  ثابت Type *lsearch (ثابت T &x, ثابت Type *not_found = nullptr) ثابت
  {
    unsigned i;
    بازگشت lfind (x, &i) ? &این->arrayZ[i] : not_found;
  }
  template <typename T>
  منطقی lfind (ثابت T &x, unsigned *pos = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned صحیح to_store = (unsigned صحیح) -1) ثابت
  {
    برای (unsigned i = 0; i < length; ++i)
      اگر (hb_equal (x, این->arrayZ[i]))
      {
	اگر (pos)
	  *pos = i;
	بازگشت درست;
      }

    اگر (pos)
    {
      انتخاب (not_found)
      {
	حالت HB_NOT_FOUND_DONT_STORE:
	  بشکن;

	حالت HB_NOT_FOUND_STORE:
	  *pos = to_store;
	  بشکن;

	حالت HB_NOT_FOUND_STORE_CLOSEST:
	  *pos = length;
	  بشکن;
      }
    }
    بازگشت نادرست;
  }

  hb_sorted_array_t<Type> qsort (صحیح (*cmp)(ثابت تهی*, ثابت تهی*))
  {
    اگر (likely (length))
      hb_qsort (arrayZ, length, این->get_item_size (), cmp);
    بازگشت hb_sorted_array_t<Type> (*این);
  }

  /* Comparator follows the C qsort convention: returns
   * negative / zero / positive int. */
  template <typename Compar>
  hb_sorted_array_t<Type> qsort (Compar compar)
  {
    اگر (likely (length))
      hb_qsort_inline (arrayZ, length, compar);
    بازگشت hb_sorted_array_t<Type> (*این);
  }

  خصوصی:
  template <typename T = Type,
	    hb_enable_if (std::is_move_assignable<T>::value)>
  hb_sorted_array_t<Type> _qsort (hb_priority<1>)
  {
    بازگشت qsort ([] (ثابت Type &a, ثابت Type &b) { بازگشت Type::cmp (&a, &b); });
  }
  hb_sorted_array_t<Type> _qsort (hb_priority<0>)
  {
    بازگشت qsort ((صحیح(*)(ثابت تهی*, ثابت تهی*)) Type::cmp);
  }
  عمومی:

  hb_sorted_array_t<Type> qsort ()
  {
    بازگشت _qsort (hb_prioritize);
  }

  /*
   * Other methods.
   */

  size_t get_size () ثابت { بازگشت length * این->get_item_size (); }

  /*
   * Reverse the order of items in this array in the range [start, end).
   */
  تهی reverse (unsigned start = 0, unsigned end = -1)
  {
    start = hb_min (start, length);
    end = hb_min (end, length);

    اگر (end < start + 2)
      بازگشت;

    unsigned stop = start + (end - start) / 2;
    برای (unsigned lhs = start, rhs = end - 1; lhs < stop; lhs++, rhs--)
      hb_swap (arrayZ[rhs], arrayZ[lhs]);
  }

  hb_array_t sub_array (unsigned صحیح start_offset = 0, unsigned صحیح *seg_شمار = nullptr /* IN/OUT */) ثابت
  {
    اگر (!start_offset && !seg_شمار)
      بازگشت *این;

    unsigned صحیح شمار = length;
    اگر (unlikely (start_offset > شمار))
      شمار = 0;
    وگرنه
      شمار -= start_offset;
    اگر (seg_شمار)
      شمار = *seg_شمار = hb_min (شمار, *seg_شمار);
    بازگشت hb_array_t (arrayZ + start_offset, شمار);
  }
  hb_array_t sub_array (unsigned صحیح start_offset, unsigned صحیح seg_شمار) ثابت
  { بازگشت sub_array (start_offset, &seg_شمار); }

  hb_array_t truncate (unsigned length) ثابت { بازگشت sub_array (0, length); }

  template <typename T,
	    unsigned P = اندازه (Type),
	    hb_enable_if (P == 1)>
  ثابت T *as () ثابت
  { بازگشت length < hb_min_size (T) ? &Null (T) : reinterpret_cast<ثابت T *> (arrayZ); }

  template <typename T,
	    unsigned P = اندازه (Type),
	    hb_enable_if (P == 1)>
  منطقی check_range (ثابت T *p, unsigned صحیح size = T::static_size) ثابت
  {
    بازگشت arrayZ <= ((ثابت نویسه *) p)
	&& ((ثابت نویسه *) p) <= arrayZ + length
	&& (unsigned صحیح) (arrayZ + length - (ثابت نویسه *) p) >= size;
  }

  template <unsigned P = اندازه (Type),
	    hb_enable_if (P == 1)>
  منطقی check_end (ثابت تهی *p) ثابت
  {
    بازگشت (uintptr_t) (((ثابت نویسه *) p) - arrayZ) <= length;
  }

  /* Only call if you allocated the underlying array using hb_malloc() or similar. */
  تهی fini ()
  { hb_free ((تهی *) arrayZ); arrayZ = nullptr; length = 0; }

  template <typename hb_serialize_context_t,
	    typename U = Type,
	    hb_enable_if (!(اندازه (U) < اندازه (long long) && hb_is_trivially_copy_assignable(hb_decay<Type>)))>
  hb_array_t copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (این);
    auto* out = c->start_embed (arrayZ);
    اگر (unlikely (!c->extend_size (out, get_size (), نادرست))) return_trace (hb_array_t ());
    برای (unsigned i = 0; i < length; i++)
      out[i] = arrayZ[i]; /* TODO: add version that calls c->copy() */
    return_trace (hb_array_t (out, length));
  }

  template <typename hb_serialize_context_t,
	    typename U = Type,
	    hb_enable_if (اندازه (U) < اندازه (long long) && hb_is_trivially_copy_assignable(hb_decay<Type>))>
  hb_array_t copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (این);
    auto* out = c->start_embed (arrayZ);
    اگر (unlikely (!c->extend_size (out, get_size (), نادرست))) return_trace (hb_array_t ());
    hb_memcpy (out, arrayZ, get_size ());
    return_trace (hb_array_t (out, length));
  }

  template <typename hb_sanitize_context_t>
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  { بازگشت c->check_array (arrayZ, length); }

  /*
   * Members
   */

  عمومی:
  Type *arrayZ = nullptr;
  unsigned صحیح length = 0;
  unsigned صحیح backwards_length = 0;
};
template <typename T> درونخطی hb_array_t<T>
hb_array ()
{ بازگشت hb_array_t<T> (); }
template <typename T> درونخطی hb_array_t<T>
hb_array (T *array, unsigned صحیح length)
{ بازگشت hb_array_t<T> (array, length); }
template <typename T, unsigned صحیح length_> درونخطی hb_array_t<T>
hb_array (T (&array_)[length_])
{ بازگشت hb_array_t<T> (array_); }

template <typename Type>
ساختار hb_sorted_array_t :
	hb_array_t<Type>,
	hb_iter_t<hb_sorted_array_t<Type>, Type&>
{
  تعریفنوع hb_iter_t<hb_sorted_array_t, Type&> iter_base_t;
  HB_ITER_USING (iter_base_t);
  ایستا constexpr منطقی is_random_access_iterator = درست;
  ایستا constexpr منطقی is_sorted_iterator = درست;
  ایستا constexpr منطقی has_fast_len = درست;

  hb_sorted_array_t () = پیشفرض;
  hb_sorted_array_t (ثابت hb_sorted_array_t&) = پیشفرض;
  ~hb_sorted_array_t () = پیشفرض;
  hb_sorted_array_t& operator= (ثابت hb_sorted_array_t&) = پیشفرض;
  hb_sorted_array_t& operator= (hb_sorted_array_t&&) = پیشفرض;

  constexpr hb_sorted_array_t (Type *array_, unsigned صحیح length_) : hb_array_t<Type> (array_, length_) {}
  template <unsigned صحیح length_>
  constexpr hb_sorted_array_t (Type (&array_)[length_]) : hb_array_t<Type> (array_) {}

  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_sorted_array_t (ثابت hb_array_t<U> &o) :
    hb_array_t<Type> (o),
    hb_iter_t<hb_sorted_array_t, Type&> () {}
  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_sorted_array_t& operator = (ثابت hb_array_t<U> &o)
  { hb_array_t<Type> (*این) = o; بازگشت *این; }

  /* Iterator implementation. */

  /* See comment in hb_array_of::operator != */
  منطقی operator != (ثابت hb_sorted_array_t& o) ثابت
  { بازگشت این->arrayZ != o.arrayZ || این->length != o.length; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () ثابت { بازگشت این->arrayZ; }
  Type *end () ثابت { بازگشت این->arrayZ + این->length; }


  hb_sorted_array_t sub_array (unsigned صحیح start_offset, unsigned صحیح *seg_شمار /* IN/OUT */) ثابت
  { بازگشت hb_sorted_array_t (((ثابت hb_array_t<Type> *) (این))->sub_array (start_offset, seg_شمار)); }
  hb_sorted_array_t sub_array (unsigned صحیح start_offset, unsigned صحیح seg_شمار) ثابت
  { بازگشت sub_array (start_offset, &seg_شمار); }

  hb_sorted_array_t truncate (unsigned length) ثابت { بازگشت sub_array (0, length); }

  template <typename T>
  Type *bsearch (ثابت T &x, Type *not_found = nullptr)
  {
    unsigned صحیح i;
    بازگشت bfind (x, &i) ? &این->arrayZ[i] : not_found;
  }
  template <typename T>
  ثابت Type *bsearch (ثابت T &x, ثابت Type *not_found = nullptr) ثابت
  {
    unsigned صحیح i;
    بازگشت bfind (x, &i) ? &این->arrayZ[i] : not_found;
  }
  template <typename T>
  منطقی bfind (ثابت T &x, unsigned صحیح *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned صحیح to_store = (unsigned صحیح) -1) ثابت
  {
    unsigned pos;

    اگر (bsearch_impl (x, &pos))
    {
      اگر (i)
	*i = pos;
      بازگشت درست;
    }

    اگر (i)
    {
      انتخاب (not_found)
      {
	حالت HB_NOT_FOUND_DONT_STORE:
	  بشکن;

	حالت HB_NOT_FOUND_STORE:
	  *i = to_store;
	  بشکن;

	حالت HB_NOT_FOUND_STORE_CLOSEST:
	  *i = pos;
	  بشکن;
      }
    }
    بازگشت نادرست;
  }
  template <typename T, typename ...Ts>
  منطقی bsearch_impl (ثابت T &x, unsigned *pos, Ts... ds) ثابت
  {
    بازگشت hb_bsearch_impl (pos,
			    x,
			    این->arrayZ,
			    این->length,
			    اندازه (Type),
			    _hb_cmp_method<T, Type, Ts...>,
			    std::forward<Ts> (ds)...);
  }
};
template <typename T> درونخطی hb_sorted_array_t<T>
hb_sorted_array (T *array, unsigned صحیح length)
{ بازگشت hb_sorted_array_t<T> (array, length); }
template <typename T, unsigned صحیح length_> درونخطی hb_sorted_array_t<T>
hb_sorted_array (T (&array_)[length_])
{ بازگشت hb_sorted_array_t<T> (array_); }

template <typename T>
درونخطی منطقی hb_array_t<T>::operator == (ثابت hb_array_t<T> &o) ثابت
{
  اگر (o.length != این->length) بازگشت نادرست;
  برای (unsigned صحیح i = 0; i < این->length; i++) {
    اگر (این->arrayZ[i] != o.arrayZ[i]) بازگشت نادرست;
  }
  بازگشت درست;
}
template <>
درونخطی منطقی hb_array_t<ثابت نویسه>::operator == (ثابت hb_array_t<ثابت نویسه> &o) ثابت
{
  اگر (o.length != این->length) بازگشت نادرست;
  بازگشت 0 == hb_memcmp (arrayZ, o.arrayZ, length);
}
template <>
درونخطی منطقی hb_array_t<ثابت unsigned نویسه>::operator == (ثابت hb_array_t<ثابت unsigned نویسه> &o) ثابت
{
  اگر (o.length != این->length) بازگشت نادرست;
  بازگشت 0 == hb_memcmp (arrayZ, o.arrayZ, length);
}


/* Specialize hash() for byte arrays. */

#ifndef HB_OPTIMIZE_SIZE_MORE
template <>
درونخطی uint32_t hb_array_t<ثابت نویسه>::hash () ثابت
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  بازگشت fasthash32(arrayZ, length, 0xf437ffe6 /* magic? */);
}

template <>
درونخطی uint32_t hb_array_t<ثابت unsigned نویسه>::hash () ثابت
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  بازگشت fasthash32(arrayZ, length, 0xf437ffe6 /* magic? */);
}
#endif


تعریفنوع hb_array_t<ثابت نویسه> hb_bytes_t;
تعریفنوع hb_array_t<ثابت unsigned نویسه> hb_ubytes_t;



#endif /* HB_ARRAY_HH */

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

#ضمن "hb.hh"
#ضمن "hb-algs.hh"
#ضمن "hb-iter.hh"
#ضمن "hb-null.hh"


template <typename Type>
بنية hb_sorted_array_t;

تعداد hb_not_found_t
{
  HB_NOT_FOUND_DONT_STORE,
  HB_NOT_FOUND_STORE,
  HB_NOT_FOUND_STORE_CLOSEST,
};


template <typename Type>
بنية hb_array_t : hb_iter_with_fallback_t<hb_array_t<Type>, Type&>
{
  ساكن constexpr منطقي realloc_move = ܫܪܪܐ;

  /*
   * Constructors.
   */
  hb_array_t () = افتراضي;
  hb_array_t (ثابت hb_array_t&) = افتراضي;
  ~hb_array_t () = افتراضي;
  hb_array_t& operator= (ثابت hb_array_t&) = افتراضي;
  hb_array_t& operator= (hb_array_t&&) = افتراضي;

  constexpr hb_array_t (Type *array_, unsigned صحيح length_) : arrayZ (array_), length (length_) {}
  template <unsigned صحيح length_>
  constexpr hb_array_t (Type (&array_)[length_]) : hb_array_t (array_, length_) {}

  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_array_t (ثابت hb_array_t<U> &o) :
    hb_iter_with_fallback_t<hb_array_t, Type&> (),
    arrayZ (o.arrayZ), length (o.length), backwards_length (o.backwards_length) {}
  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_array_t& operator = (ثابت hb_array_t<U> &o)
  { arrayZ = o.arrayZ; length = o.length; backwards_length = o.backwards_length; ܦܢܐ *هذا; }

  /*
   * Iterator implementation.
   */
  تعريفنوع Type& __item_t__;
  ساكن constexpr منطقي is_random_access_iterator = ܫܪܪܐ;
  ساكن constexpr منطقي has_fast_len = ܫܪܪܐ;
  Type& __item__ () ثابت
  {
    ܐܢ (unlikely (!length)) ܦܢܐ CrapOrNull (Type);
    ܦܢܐ *arrayZ;
  }
  Type& __item_at__ (unsigned i) ثابت
  {
    ܐܢ (unlikely (i >= length)) ܦܢܐ CrapOrNull (Type);
    ܦܢܐ arrayZ[i];
  }
  فراغ __next__ ()
  {
    ܐܢ (unlikely (!length))
      ܦܢܐ;
    length--;
    backwards_length++;
    arrayZ++;
  }
  فراغ __forward__ (unsigned n)
  {
    ܐܢ (unlikely (n > length))
      n = length;
    length -= n;
    backwards_length += n;
    arrayZ += n;
  }
  فراغ __prev__ ()
  {
    ܐܢ (unlikely (!backwards_length))
      ܦܢܐ;
    length++;
    backwards_length--;
    arrayZ--;
  }
  فراغ __rewind__ (unsigned n)
  {
    ܐܢ (unlikely (n > backwards_length))
      n = backwards_length;
    length += n;
    backwards_length -= n;
    arrayZ -= n;
  }
  unsigned __len__ () ثابت { ܦܢܐ length; }
  /* Ouch. The operator== compares the contents of the array.  For range-based for loops,
   * it's best if we can just compare arrayZ, though comparing contents is still fast,
   * but also would require that Type has operator==.  As such, we optimize this operator
   * for range-based for loop and just compare arrayZ and length.
   *
   * The above comment is outdated now because we implemented separate begin/end to
   * objects that were using hb_array_t for range-based loop before. */
  منطقي operator != (ثابت hb_array_t& o) ثابت
  { ܦܢܐ هذا->arrayZ != o.arrayZ || هذا->length != o.length; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () ثابت { ܦܢܐ arrayZ; }
  Type *end () ثابت { ܦܢܐ arrayZ + length; }


  /* Extra operators.
   */
  Type * operator & () ثابت { ܦܢܐ arrayZ; }
  operator hb_array_t<ثابت Type> () { ܦܢܐ hb_array_t<ثابت Type> (arrayZ, length); }
  template <typename T> operator T * () ثابت { ܦܢܐ arrayZ; }

  HB_INTERNAL منطقي operator == (ثابت hb_array_t &o) ثابت;

  uint32_t hash () ثابت
  {
    // FNV-1a hash function
    // https://github.com/harfbuzz/harfbuzz/pull/4228
    uint32_t current = /*cbf29ce4*/0x84222325;
    لكل (auto &v : *هذا)
    {
      current = current ^ hb_hash (v);
      current = current * 16777619;
    }
    ܦܢܐ current;
  }

  /*
   * Compare, Sort, and Search.
   */

  /* Note: our compare is NOT lexicographic; it also does NOT call Type::cmp. */
  صحيح cmp (ثابت hb_array_t &a) ثابت
  {
    ܐܢ (length != a.length)
      ܦܢܐ (صحيح) a.length - (صحيح) length;
    ܦܢܐ hb_memcmp (a.arrayZ, arrayZ, get_size ());
  }
  HB_INTERNAL ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    hb_array_t *a = (hb_array_t *) pa;
    hb_array_t *b = (hb_array_t *) pb;
    ܦܢܐ b->cmp (*a);
  }

  template <typename T>
  Type *lsearch (ثابت T &x, Type *not_found = nullptr)
  {
    unsigned i;
    ܦܢܐ lfind (x, &i) ? &هذا->arrayZ[i] : not_found;
  }
  template <typename T>
  ثابت Type *lsearch (ثابت T &x, ثابت Type *not_found = nullptr) ثابت
  {
    unsigned i;
    ܦܢܐ lfind (x, &i) ? &هذا->arrayZ[i] : not_found;
  }
  template <typename T>
  منطقي lfind (ثابت T &x, unsigned *pos = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned صحيح to_store = (unsigned صحيح) -1) ثابت
  {
    لكل (unsigned i = 0; i < length; ++i)
      ܐܢ (hb_equal (x, هذا->arrayZ[i]))
      {
	ܐܢ (pos)
	  *pos = i;
	ܦܢܐ ܫܪܪܐ;
      }

    ܐܢ (pos)
    {
      بدل (not_found)
      {
	حالة HB_NOT_FOUND_DONT_STORE:
	  اكسر;

	حالة HB_NOT_FOUND_STORE:
	  *pos = to_store;
	  اكسر;

	حالة HB_NOT_FOUND_STORE_CLOSEST:
	  *pos = length;
	  اكسر;
      }
    }
    ܦܢܐ ܕܓܠܐ;
  }

  hb_sorted_array_t<Type> qsort (صحيح (*cmp)(ثابت فراغ*, ثابت فراغ*))
  {
    ܐܢ (likely (length))
      hb_qsort (arrayZ, length, هذا->get_item_size (), cmp);
    ܦܢܐ hb_sorted_array_t<Type> (*هذا);
  }

  /* Comparator follows the C qsort convention: returns
   * negative / zero / positive int. */
  template <typename Compar>
  hb_sorted_array_t<Type> qsort (Compar compar)
  {
    ܐܢ (likely (length))
      hb_qsort_inline (arrayZ, length, compar);
    ܦܢܐ hb_sorted_array_t<Type> (*هذا);
  }

  خاص:
  template <typename T = Type,
	    hb_enable_if (std::is_move_assignable<T>::value)>
  hb_sorted_array_t<Type> _qsort (hb_priority<1>)
  {
    ܦܢܐ qsort ([] (ثابت Type &a, ثابت Type &b) { ܦܢܐ Type::cmp (&a, &b); });
  }
  hb_sorted_array_t<Type> _qsort (hb_priority<0>)
  {
    ܦܢܐ qsort ((صحيح(*)(ثابت فراغ*, ثابت فراغ*)) Type::cmp);
  }
  عام:

  hb_sorted_array_t<Type> qsort ()
  {
    ܦܢܐ _qsort (hb_prioritize);
  }

  /*
   * Other methods.
   */

  size_t get_size () ثابت { ܦܢܐ length * هذا->get_item_size (); }

  /*
   * Reverse the order of items in this array in the range [start, end).
   */
  فراغ reverse (unsigned start = 0, unsigned end = -1)
  {
    start = hb_min (start, length);
    end = hb_min (end, length);

    ܐܢ (end < start + 2)
      ܦܢܐ;

    unsigned stop = start + (end - start) / 2;
    لكل (unsigned lhs = start, rhs = end - 1; lhs < stop; lhs++, rhs--)
      hb_swap (arrayZ[rhs], arrayZ[lhs]);
  }

  hb_array_t sub_array (unsigned صحيح start_offset = 0, unsigned صحيح *seg_count = nullptr /* IN/OUT */) ثابت
  {
    ܐܢ (!start_offset && !seg_count)
      ܦܢܐ *هذا;

    unsigned صحيح count = length;
    ܐܢ (unlikely (start_offset > count))
      count = 0;
    ܐܚܪܢܐ
      count -= start_offset;
    ܐܢ (seg_count)
      count = *seg_count = hb_min (count, *seg_count);
    ܦܢܐ hb_array_t (arrayZ + start_offset, count);
  }
  hb_array_t sub_array (unsigned صحيح start_offset, unsigned صحيح seg_count) ثابت
  { ܦܢܐ sub_array (start_offset, &seg_count); }

  hb_array_t truncate (unsigned length) ثابت { ܦܢܐ sub_array (0, length); }

  template <typename T,
	    unsigned P = حجم (Type),
	    hb_enable_if (P == 1)>
  ثابت T *as () ثابت
  { ܦܢܐ length < hb_min_size (T) ? &Null (T) : reinterpret_cast<ثابت T *> (arrayZ); }

  template <typename T,
	    unsigned P = حجم (Type),
	    hb_enable_if (P == 1)>
  منطقي check_range (ثابت T *p, unsigned صحيح size = T::static_size) ثابت
  {
    ܦܢܐ arrayZ <= ((ثابت حرف *) p)
	&& ((ثابت حرف *) p) <= arrayZ + length
	&& (unsigned صحيح) (arrayZ + length - (ثابت حرف *) p) >= size;
  }

  template <unsigned P = حجم (Type),
	    hb_enable_if (P == 1)>
  منطقي check_end (ثابت فراغ *p) ثابت
  {
    ܦܢܐ (uintptr_t) (((ثابت حرف *) p) - arrayZ) <= length;
  }

  /* Only call if you allocated the underlying array using hb_malloc() or similar. */
  فراغ fini ()
  { hb_free ((فراغ *) arrayZ); arrayZ = nullptr; length = 0; }

  template <typename hb_serialize_context_t,
	    typename U = Type,
	    hb_enable_if (!(حجم (U) < حجم (long long) && hb_is_trivially_copy_assignable(hb_decay<Type>)))>
  hb_array_t copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    auto* out = c->start_embed (arrayZ);
    ܐܢ (unlikely (!c->extend_size (out, get_size (), ܕܓܠܐ))) return_trace (hb_array_t ());
    لكل (unsigned i = 0; i < length; i++)
      out[i] = arrayZ[i]; /* TODO: add version that calls c->copy() */
    return_trace (hb_array_t (out, length));
  }

  template <typename hb_serialize_context_t,
	    typename U = Type,
	    hb_enable_if (حجم (U) < حجم (long long) && hb_is_trivially_copy_assignable(hb_decay<Type>))>
  hb_array_t copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    auto* out = c->start_embed (arrayZ);
    ܐܢ (unlikely (!c->extend_size (out, get_size (), ܕܓܠܐ))) return_trace (hb_array_t ());
    hb_memcpy (out, arrayZ, get_size ());
    return_trace (hb_array_t (out, length));
  }

  template <typename hb_sanitize_context_t>
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  { ܦܢܐ c->check_array (arrayZ, length); }

  /*
   * Members
   */

  عام:
  Type *arrayZ = nullptr;
  unsigned صحيح length = 0;
  unsigned صحيح backwards_length = 0;
};
template <typename T> مضمن hb_array_t<T>
hb_array ()
{ ܦܢܐ hb_array_t<T> (); }
template <typename T> مضمن hb_array_t<T>
hb_array (T *array, unsigned صحيح length)
{ ܦܢܐ hb_array_t<T> (array, length); }
template <typename T, unsigned صحيح length_> مضمن hb_array_t<T>
hb_array (T (&array_)[length_])
{ ܦܢܐ hb_array_t<T> (array_); }

template <typename Type>
بنية hb_sorted_array_t :
	hb_array_t<Type>,
	hb_iter_t<hb_sorted_array_t<Type>, Type&>
{
  تعريفنوع hb_iter_t<hb_sorted_array_t, Type&> iter_base_t;
  HB_ITER_USING (iter_base_t);
  ساكن constexpr منطقي is_random_access_iterator = ܫܪܪܐ;
  ساكن constexpr منطقي is_sorted_iterator = ܫܪܪܐ;
  ساكن constexpr منطقي has_fast_len = ܫܪܪܐ;

  hb_sorted_array_t () = افتراضي;
  hb_sorted_array_t (ثابت hb_sorted_array_t&) = افتراضي;
  ~hb_sorted_array_t () = افتراضي;
  hb_sorted_array_t& operator= (ثابت hb_sorted_array_t&) = افتراضي;
  hb_sorted_array_t& operator= (hb_sorted_array_t&&) = افتراضي;

  constexpr hb_sorted_array_t (Type *array_, unsigned صحيح length_) : hb_array_t<Type> (array_, length_) {}
  template <unsigned صحيح length_>
  constexpr hb_sorted_array_t (Type (&array_)[length_]) : hb_array_t<Type> (array_) {}

  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_sorted_array_t (ثابت hb_array_t<U> &o) :
    hb_array_t<Type> (o),
    hb_iter_t<hb_sorted_array_t, Type&> () {}
  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_sorted_array_t& operator = (ثابت hb_array_t<U> &o)
  { hb_array_t<Type> (*هذا) = o; ܦܢܐ *هذا; }

  /* Iterator implementation. */

  /* See comment in hb_array_of::operator != */
  منطقي operator != (ثابت hb_sorted_array_t& o) ثابت
  { ܦܢܐ هذا->arrayZ != o.arrayZ || هذا->length != o.length; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () ثابت { ܦܢܐ هذا->arrayZ; }
  Type *end () ثابت { ܦܢܐ هذا->arrayZ + هذا->length; }


  hb_sorted_array_t sub_array (unsigned صحيح start_offset, unsigned صحيح *seg_count /* IN/OUT */) ثابت
  { ܦܢܐ hb_sorted_array_t (((ثابت hb_array_t<Type> *) (هذا))->sub_array (start_offset, seg_count)); }
  hb_sorted_array_t sub_array (unsigned صحيح start_offset, unsigned صحيح seg_count) ثابت
  { ܦܢܐ sub_array (start_offset, &seg_count); }

  hb_sorted_array_t truncate (unsigned length) ثابت { ܦܢܐ sub_array (0, length); }

  template <typename T>
  Type *bsearch (ثابت T &x, Type *not_found = nullptr)
  {
    unsigned صحيح i;
    ܦܢܐ bfind (x, &i) ? &هذا->arrayZ[i] : not_found;
  }
  template <typename T>
  ثابت Type *bsearch (ثابت T &x, ثابت Type *not_found = nullptr) ثابت
  {
    unsigned صحيح i;
    ܦܢܐ bfind (x, &i) ? &هذا->arrayZ[i] : not_found;
  }
  template <typename T>
  منطقي bfind (ثابت T &x, unsigned صحيح *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned صحيح to_store = (unsigned صحيح) -1) ثابت
  {
    unsigned pos;

    ܐܢ (bsearch_impl (x, &pos))
    {
      ܐܢ (i)
	*i = pos;
      ܦܢܐ ܫܪܪܐ;
    }

    ܐܢ (i)
    {
      بدل (not_found)
      {
	حالة HB_NOT_FOUND_DONT_STORE:
	  اكسر;

	حالة HB_NOT_FOUND_STORE:
	  *i = to_store;
	  اكسر;

	حالة HB_NOT_FOUND_STORE_CLOSEST:
	  *i = pos;
	  اكسر;
      }
    }
    ܦܢܐ ܕܓܠܐ;
  }
  template <typename T, typename ...Ts>
  منطقي bsearch_impl (ثابت T &x, unsigned *pos, Ts... ds) ثابت
  {
    ܦܢܐ hb_bsearch_impl (pos,
			    x,
			    هذا->arrayZ,
			    هذا->length,
			    حجم (Type),
			    _hb_cmp_method<T, Type, Ts...>,
			    std::forward<Ts> (ds)...);
  }
};
template <typename T> مضمن hb_sorted_array_t<T>
hb_sorted_array (T *array, unsigned صحيح length)
{ ܦܢܐ hb_sorted_array_t<T> (array, length); }
template <typename T, unsigned صحيح length_> مضمن hb_sorted_array_t<T>
hb_sorted_array (T (&array_)[length_])
{ ܦܢܐ hb_sorted_array_t<T> (array_); }

template <typename T>
مضمن منطقي hb_array_t<T>::operator == (ثابت hb_array_t<T> &o) ثابت
{
  ܐܢ (o.length != هذا->length) ܦܢܐ ܕܓܠܐ;
  لكل (unsigned صحيح i = 0; i < هذا->length; i++) {
    ܐܢ (هذا->arrayZ[i] != o.arrayZ[i]) ܦܢܐ ܕܓܠܐ;
  }
  ܦܢܐ ܫܪܪܐ;
}
template <>
مضمن منطقي hb_array_t<ثابت حرف>::operator == (ثابت hb_array_t<ثابت حرف> &o) ثابت
{
  ܐܢ (o.length != هذا->length) ܦܢܐ ܕܓܠܐ;
  ܦܢܐ 0 == hb_memcmp (arrayZ, o.arrayZ, length);
}
template <>
مضمن منطقي hb_array_t<ثابت unsigned حرف>::operator == (ثابت hb_array_t<ثابت unsigned حرف> &o) ثابت
{
  ܐܢ (o.length != هذا->length) ܦܢܐ ܕܓܠܐ;
  ܦܢܐ 0 == hb_memcmp (arrayZ, o.arrayZ, length);
}


/* Specialize hash() for byte arrays. */

#ifndef HB_OPTIMIZE_SIZE_MORE
template <>
مضمن uint32_t hb_array_t<ثابت حرف>::hash () ثابت
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  ܦܢܐ fasthash32(arrayZ, length, 0xf437ffe6 /* magic? */);
}

template <>
مضمن uint32_t hb_array_t<ثابت unsigned حرف>::hash () ثابت
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  ܦܢܐ fasthash32(arrayZ, length, 0xf437ffe6 /* magic? */);
}
#endif


تعريفنوع hb_array_t<ثابت حرف> hb_bytes_t;
تعريفنوع hb_array_t<ثابت unsigned حرف> hb_ubytes_t;



#endif /* HB_ARRAY_HH */

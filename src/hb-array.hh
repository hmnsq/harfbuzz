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
  ساكن constexpr منطقي realloc_move = true;

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
  { arrayZ = o.arrayZ; length = o.length; backwards_length = o.backwards_length; أرجع *هذا; }

  /*
   * Iterator implementation.
   */
  تعريفنوع Type& __item_t__;
  ساكن constexpr منطقي is_random_access_iterator = true;
  ساكن constexpr منطقي has_fast_len = true;
  Type& __item__ () ثابت
  {
    إذا (unlikely (!length)) أرجع CrapOrNull (Type);
    أرجع *arrayZ;
  }
  Type& __item_at__ (unsigned i) ثابت
  {
    إذا (unlikely (i >= length)) أرجع CrapOrNull (Type);
    أرجع arrayZ[i];
  }
  فراغ __next__ ()
  {
    إذا (unlikely (!length))
      أرجع;
    length--;
    backwards_length++;
    arrayZ++;
  }
  فراغ __forward__ (unsigned n)
  {
    إذا (unlikely (n > length))
      n = length;
    length -= n;
    backwards_length += n;
    arrayZ += n;
  }
  فراغ __prev__ ()
  {
    إذا (unlikely (!backwards_length))
      أرجع;
    length++;
    backwards_length--;
    arrayZ--;
  }
  فراغ __rewind__ (unsigned n)
  {
    إذا (unlikely (n > backwards_length))
      n = backwards_length;
    length += n;
    backwards_length -= n;
    arrayZ -= n;
  }
  unsigned __len__ () ثابت { أرجع length; }
  /* Ouch. The operator== compares the contents of the array.  For range-based for loops,
   * it's best if we can just compare arrayZ, though comparing contents is still fast,
   * but also would require that Type has operator==.  As such, we optimize this operator
   * for range-based for loop and just compare arrayZ and length.
   *
   * The above comment is outdated now because we implemented separate begin/end to
   * objects that were using hb_array_t for range-based loop before. */
  منطقي operator != (ثابت hb_array_t& o) ثابت
  { أرجع هذا->arrayZ != o.arrayZ || هذا->length != o.length; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () ثابت { أرجع arrayZ; }
  Type *end () ثابت { أرجع arrayZ + length; }


  /* Extra operators.
   */
  Type * operator & () ثابت { أرجع arrayZ; }
  operator hb_array_t<ثابت Type> () { أرجع hb_array_t<ثابت Type> (arrayZ, length); }
  template <typename T> operator T * () ثابت { أرجع arrayZ; }

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
    أرجع current;
  }

  /*
   * Compare, Sort, and Search.
   */

  /* Note: our compare is NOT lexicographic; it also does NOT call Type::cmp. */
  صحيح cmp (ثابت hb_array_t &a) ثابت
  {
    إذا (length != a.length)
      أرجع (صحيح) a.length - (صحيح) length;
    أرجع hb_memcmp (a.arrayZ, arrayZ, get_size ());
  }
  HB_INTERNAL ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    hb_array_t *a = (hb_array_t *) pa;
    hb_array_t *b = (hb_array_t *) pb;
    أرجع b->cmp (*a);
  }

  template <typename T>
  Type *lsearch (ثابت T &x, Type *not_found = nullptr)
  {
    unsigned i;
    أرجع lfind (x, &i) ? &هذا->arrayZ[i] : not_found;
  }
  template <typename T>
  ثابت Type *lsearch (ثابت T &x, ثابت Type *not_found = nullptr) ثابت
  {
    unsigned i;
    أرجع lfind (x, &i) ? &هذا->arrayZ[i] : not_found;
  }
  template <typename T>
  منطقي lfind (ثابت T &x, unsigned *pos = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned صحيح to_store = (unsigned صحيح) -1) ثابت
  {
    لكل (unsigned i = 0; i < length; ++i)
      إذا (hb_equal (x, هذا->arrayZ[i]))
      {
	إذا (pos)
	  *pos = i;
	أرجع true;
      }

    إذا (pos)
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
    أرجع خطأ;
  }

  hb_sorted_array_t<Type> qsort (صحيح (*cmp)(ثابت فراغ*, ثابت فراغ*))
  {
    إذا (likely (length))
      hb_qsort (arrayZ, length, هذا->get_item_size (), cmp);
    أرجع hb_sorted_array_t<Type> (*هذا);
  }

  /* Comparator follows the C qsort convention: returns
   * negative / zero / positive int. */
  template <typename Compar>
  hb_sorted_array_t<Type> qsort (Compar compar)
  {
    إذا (likely (length))
      hb_qsort_inline (arrayZ, length, compar);
    أرجع hb_sorted_array_t<Type> (*هذا);
  }

  خاص:
  template <typename T = Type,
	    hb_enable_if (std::is_move_assignable<T>::value)>
  hb_sorted_array_t<Type> _qsort (hb_priority<1>)
  {
    أرجع qsort ([] (ثابت Type &a, ثابت Type &b) { أرجع Type::cmp (&a, &b); });
  }
  hb_sorted_array_t<Type> _qsort (hb_priority<0>)
  {
    أرجع qsort ((صحيح(*)(ثابت فراغ*, ثابت فراغ*)) Type::cmp);
  }
  عام:

  hb_sorted_array_t<Type> qsort ()
  {
    أرجع _qsort (hb_prioritize);
  }

  /*
   * Other methods.
   */

  size_t get_size () ثابت { أرجع length * هذا->get_item_size (); }

  /*
   * Reverse the order of items in this array in the range [start, end).
   */
  فراغ reverse (unsigned start = 0, unsigned end = -1)
  {
    start = hb_min (start, length);
    end = hb_min (end, length);

    إذا (end < start + 2)
      أرجع;

    unsigned stop = start + (end - start) / 2;
    لكل (unsigned lhs = start, rhs = end - 1; lhs < stop; lhs++, rhs--)
      hb_swap (arrayZ[rhs], arrayZ[lhs]);
  }

  hb_array_t sub_array (unsigned صحيح start_offset = 0, unsigned صحيح *seg_سانى = nullptr /* IN/OUT */) ثابت
  {
    إذا (!start_offset && !seg_سانى)
      أرجع *هذا;

    unsigned صحيح سانى = length;
    إذا (unlikely (start_offset > سانى))
      سانى = 0;
    وإلا
      سانى -= start_offset;
    إذا (seg_سانى)
      سانى = *seg_سانى = hb_min (سانى, *seg_سانى);
    أرجع hb_array_t (arrayZ + start_offset, سانى);
  }
  hb_array_t sub_array (unsigned صحيح start_offset, unsigned صحيح seg_سانى) ثابت
  { أرجع sub_array (start_offset, &seg_سانى); }

  hb_array_t truncate (unsigned length) ثابت { أرجع sub_array (0, length); }

  template <typename T,
	    unsigned P = حجم (Type),
	    hb_enable_if (P == 1)>
  ثابت T *as () ثابت
  { أرجع length < hb_min_size (T) ? &Null (T) : reinterpret_cast<ثابت T *> (arrayZ); }

  template <typename T,
	    unsigned P = حجم (Type),
	    hb_enable_if (P == 1)>
  منطقي check_range (ثابت T *p, unsigned صحيح size = T::static_size) ثابت
  {
    أرجع arrayZ <= ((ثابت حرف *) p)
	&& ((ثابت حرف *) p) <= arrayZ + length
	&& (unsigned صحيح) (arrayZ + length - (ثابت حرف *) p) >= size;
  }

  template <unsigned P = حجم (Type),
	    hb_enable_if (P == 1)>
  منطقي check_end (ثابت فراغ *p) ثابت
  {
    أرجع (uintptr_t) (((ثابت حرف *) p) - arrayZ) <= length;
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
    إذا (unlikely (!c->extend_size (out, get_size (), خطأ))) return_trace (hb_array_t ());
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
    إذا (unlikely (!c->extend_size (out, get_size (), خطأ))) return_trace (hb_array_t ());
    hb_memcpy (out, arrayZ, get_size ());
    return_trace (hb_array_t (out, length));
  }

  template <typename hb_sanitize_context_t>
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  { أرجع c->check_array (arrayZ, length); }

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
{ أرجع hb_array_t<T> (); }
template <typename T> مضمن hb_array_t<T>
hb_array (T *array, unsigned صحيح length)
{ أرجع hb_array_t<T> (array, length); }
template <typename T, unsigned صحيح length_> مضمن hb_array_t<T>
hb_array (T (&array_)[length_])
{ أرجع hb_array_t<T> (array_); }

template <typename Type>
بنية hb_sorted_array_t :
	hb_array_t<Type>,
	hb_iter_t<hb_sorted_array_t<Type>, Type&>
{
  تعريفنوع hb_iter_t<hb_sorted_array_t, Type&> iter_base_t;
  HB_ITER_USING (iter_base_t);
  ساكن constexpr منطقي is_random_access_iterator = true;
  ساكن constexpr منطقي is_sorted_iterator = true;
  ساكن constexpr منطقي has_fast_len = true;

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
  { hb_array_t<Type> (*هذا) = o; أرجع *هذا; }

  /* Iterator implementation. */

  /* See comment in hb_array_of::operator != */
  منطقي operator != (ثابت hb_sorted_array_t& o) ثابت
  { أرجع هذا->arrayZ != o.arrayZ || هذا->length != o.length; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () ثابت { أرجع هذا->arrayZ; }
  Type *end () ثابت { أرجع هذا->arrayZ + هذا->length; }


  hb_sorted_array_t sub_array (unsigned صحيح start_offset, unsigned صحيح *seg_سانى /* IN/OUT */) ثابت
  { أرجع hb_sorted_array_t (((ثابت hb_array_t<Type> *) (هذا))->sub_array (start_offset, seg_سانى)); }
  hb_sorted_array_t sub_array (unsigned صحيح start_offset, unsigned صحيح seg_سانى) ثابت
  { أرجع sub_array (start_offset, &seg_سانى); }

  hb_sorted_array_t truncate (unsigned length) ثابت { أرجع sub_array (0, length); }

  template <typename T>
  Type *bsearch (ثابت T &x, Type *not_found = nullptr)
  {
    unsigned صحيح i;
    أرجع bfind (x, &i) ? &هذا->arrayZ[i] : not_found;
  }
  template <typename T>
  ثابت Type *bsearch (ثابت T &x, ثابت Type *not_found = nullptr) ثابت
  {
    unsigned صحيح i;
    أرجع bfind (x, &i) ? &هذا->arrayZ[i] : not_found;
  }
  template <typename T>
  منطقي bfind (ثابت T &x, unsigned صحيح *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned صحيح to_store = (unsigned صحيح) -1) ثابت
  {
    unsigned pos;

    إذا (bsearch_impl (x, &pos))
    {
      إذا (i)
	*i = pos;
      أرجع true;
    }

    إذا (i)
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
    أرجع خطأ;
  }
  template <typename T, typename ...Ts>
  منطقي bsearch_impl (ثابت T &x, unsigned *pos, Ts... ds) ثابت
  {
    أرجع hb_bsearch_impl (pos,
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
{ أرجع hb_sorted_array_t<T> (array, length); }
template <typename T, unsigned صحيح length_> مضمن hb_sorted_array_t<T>
hb_sorted_array (T (&array_)[length_])
{ أرجع hb_sorted_array_t<T> (array_); }

template <typename T>
مضمن منطقي hb_array_t<T>::operator == (ثابت hb_array_t<T> &o) ثابت
{
  إذا (o.length != هذا->length) أرجع خطأ;
  لكل (unsigned صحيح i = 0; i < هذا->length; i++) {
    إذا (هذا->arrayZ[i] != o.arrayZ[i]) أرجع خطأ;
  }
  أرجع true;
}
template <>
مضمن منطقي hb_array_t<ثابت حرف>::operator == (ثابت hb_array_t<ثابت حرف> &o) ثابت
{
  إذا (o.length != هذا->length) أرجع خطأ;
  أرجع 0 == hb_memcmp (arrayZ, o.arrayZ, length);
}
template <>
مضمن منطقي hb_array_t<ثابت unsigned حرف>::operator == (ثابت hb_array_t<ثابت unsigned حرف> &o) ثابت
{
  إذا (o.length != هذا->length) أرجع خطأ;
  أرجع 0 == hb_memcmp (arrayZ, o.arrayZ, length);
}


/* Specialize hash() for byte arrays. */

#ifndef HB_OPTIMIZE_SIZE_MORE
template <>
مضمن uint32_t hb_array_t<ثابت حرف>::hash () ثابت
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  أرجع fasthash32(arrayZ, length, 0xf437ffe6 /* magic? */);
}

template <>
مضمن uint32_t hb_array_t<ثابت unsigned حرف>::hash () ثابت
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  أرجع fasthash32(arrayZ, length, 0xf437ffe6 /* magic? */);
}
#endif


تعريفنوع hb_array_t<ثابت حرف> hb_bytes_t;
تعريفنوع hb_array_t<ثابت unsigned حرف> hb_ubytes_t;



#endif /* HB_ARRAY_HH */

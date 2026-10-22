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

#அந்தர்பாவ "hb.hh"
#அந்தர்பாவ "hb-algs.hh"
#அந்தர்பாவ "hb-iter.hh"
#அந்தர்பாவ "hb-null.hh"


template <typename Type>
கட்டமைப்பு hb_sorted_array_t;

எண்ணிக்கை hb_not_found_t
{
  HB_NOT_FOUND_DONT_STORE,
  HB_NOT_FOUND_STORE,
  HB_NOT_FOUND_STORE_CLOSEST,
};


template <typename Type>
கட்டமைப்பு hb_array_t : hb_iter_with_fallback_t<hb_array_t<Type>, Type&>
{
  நிலையான constexpr தருக்க realloc_move = உண்மை;

  /*
   * Constructors.
   */
  hb_array_t () = இயல்பு;
  hb_array_t (மாறிலி hb_array_t&) = இயல்பு;
  ~hb_array_t () = இயல்பு;
  hb_array_t& operator= (மாறிலி hb_array_t&) = இயல்பு;
  hb_array_t& operator= (hb_array_t&&) = இயல்பு;

  constexpr hb_array_t (Type *array_, unsigned முழு length_) : arrayZ (array_), length (length_) {}
  template <unsigned முழு length_>
  constexpr hb_array_t (Type (&array_)[length_]) : hb_array_t (array_, length_) {}

  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_array_t (மாறிலி hb_array_t<U> &o) :
    hb_iter_with_fallback_t<hb_array_t, Type&> (),
    arrayZ (o.arrayZ), length (o.length), backwards_length (o.backwards_length) {}
  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_array_t& operator = (மாறிலி hb_array_t<U> &o)
  { arrayZ = o.arrayZ; length = o.length; backwards_length = o.backwards_length; திரும்பு *இது; }

  /*
   * Iterator implementation.
   */
  வகைவரை Type& __item_t__;
  நிலையான constexpr தருக்க is_random_access_iterator = உண்மை;
  நிலையான constexpr தருக்க has_fast_len = உண்மை;
  Type& __item__ () மாறிலி
  {
    எனில் (unlikely (!length)) திரும்பு CrapOrNull (Type);
    திரும்பு *arrayZ;
  }
  Type& __item_at__ (unsigned i) மாறிலி
  {
    எனில் (unlikely (i >= length)) திரும்பு CrapOrNull (Type);
    திரும்பு arrayZ[i];
  }
  வெறுமை __next__ ()
  {
    எனில் (unlikely (!length))
      திரும்பு;
    length--;
    backwards_length++;
    arrayZ++;
  }
  வெறுமை __forward__ (unsigned n)
  {
    எனில் (unlikely (n > length))
      n = length;
    length -= n;
    backwards_length += n;
    arrayZ += n;
  }
  வெறுமை __prev__ ()
  {
    எனில் (unlikely (!backwards_length))
      திரும்பு;
    length++;
    backwards_length--;
    arrayZ--;
  }
  வெறுமை __rewind__ (unsigned n)
  {
    எனில் (unlikely (n > backwards_length))
      n = backwards_length;
    length += n;
    backwards_length -= n;
    arrayZ -= n;
  }
  unsigned __len__ () மாறிலி { திரும்பு length; }
  /* Ouch. The operator== compares the contents of the array.  For range-based for loops,
   * it's best if we can just compare arrayZ, though comparing contents is still fast,
   * but also would require that Type has operator==.  As such, we optimize this operator
   * for range-based for loop and just compare arrayZ and length.
   *
   * The above comment is outdated now because we implemented separate begin/end to
   * objects that were using hb_array_t for range-based loop before. */
  தருக்க operator != (மாறிலி hb_array_t& o) மாறிலி
  { திரும்பு இது->arrayZ != o.arrayZ || இது->length != o.length; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () மாறிலி { திரும்பு arrayZ; }
  Type *end () மாறிலி { திரும்பு arrayZ + length; }


  /* Extra operators.
   */
  Type * operator & () மாறிலி { திரும்பு arrayZ; }
  operator hb_array_t<மாறிலி Type> () { திரும்பு hb_array_t<மாறிலி Type> (arrayZ, length); }
  template <typename T> operator T * () மாறிலி { திரும்பு arrayZ; }

  HB_INTERNAL தருக்க operator == (மாறிலி hb_array_t &o) மாறிலி;

  uint32_t hash () மாறிலி
  {
    // FNV-1a hash function
    // https://github.com/harfbuzz/harfbuzz/pull/4228
    uint32_t current = /*cbf29ce4*/0x84222325;
    க்கு (auto &v : *இது)
    {
      current = current ^ hb_hash (v);
      current = current * 16777619;
    }
    திரும்பு current;
  }

  /*
   * Compare, Sort, and Search.
   */

  /* Note: our compare is NOT lexicographic; it also does NOT call Type::cmp. */
  முழு cmp (மாறிலி hb_array_t &a) மாறிலி
  {
    எனில் (length != a.length)
      திரும்பு (முழு) a.length - (முழு) length;
    திரும்பு hb_memcmp (a.arrayZ, arrayZ, get_size ());
  }
  HB_INTERNAL நிலையான முழு cmp (மாறிலி வெறுமை *pa, மாறிலி வெறுமை *pb)
  {
    hb_array_t *a = (hb_array_t *) pa;
    hb_array_t *b = (hb_array_t *) pb;
    திரும்பு b->cmp (*a);
  }

  template <typename T>
  Type *lsearch (மாறிலி T &x, Type *not_found = nullptr)
  {
    unsigned i;
    திரும்பு lfind (x, &i) ? &இது->arrayZ[i] : not_found;
  }
  template <typename T>
  மாறிலி Type *lsearch (மாறிலி T &x, மாறிலி Type *not_found = nullptr) மாறிலி
  {
    unsigned i;
    திரும்பு lfind (x, &i) ? &இது->arrayZ[i] : not_found;
  }
  template <typename T>
  தருக்க lfind (மாறிலி T &x, unsigned *pos = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned முழு to_store = (unsigned முழு) -1) மாறிலி
  {
    க்கு (unsigned i = 0; i < length; ++i)
      எனில் (hb_equal (x, இது->arrayZ[i]))
      {
	எனில் (pos)
	  *pos = i;
	திரும்பு உண்மை;
      }

    எனில் (pos)
    {
      தேர்வு (not_found)
      {
	நிலை HB_NOT_FOUND_DONT_STORE:
	  நிறுத்து;

	நிலை HB_NOT_FOUND_STORE:
	  *pos = to_store;
	  நிறுத்து;

	நிலை HB_NOT_FOUND_STORE_CLOSEST:
	  *pos = length;
	  நிறுத்து;
      }
    }
    திரும்பு பொய்;
  }

  hb_sorted_array_t<Type> qsort (முழு (*cmp)(மாறிலி வெறுமை*, மாறிலி வெறுமை*))
  {
    எனில் (likely (length))
      hb_qsort (arrayZ, length, இது->get_item_size (), cmp);
    திரும்பு hb_sorted_array_t<Type> (*இது);
  }

  /* Comparator follows the C qsort convention: returns
   * negative / zero / positive int. */
  template <typename Compar>
  hb_sorted_array_t<Type> qsort (Compar compar)
  {
    எனில் (likely (length))
      hb_qsort_inline (arrayZ, length, compar);
    திரும்பு hb_sorted_array_t<Type> (*இது);
  }

  தனிப்பட்ட:
  template <typename T = Type,
	    hb_enable_if (std::is_move_assignable<T>::value)>
  hb_sorted_array_t<Type> _qsort (hb_priority<1>)
  {
    திரும்பு qsort ([] (மாறிலி Type &a, மாறிலி Type &b) { திரும்பு Type::cmp (&a, &b); });
  }
  hb_sorted_array_t<Type> _qsort (hb_priority<0>)
  {
    திரும்பு qsort ((முழு(*)(மாறிலி வெறுமை*, மாறிலி வெறுமை*)) Type::cmp);
  }
  பொது:

  hb_sorted_array_t<Type> qsort ()
  {
    திரும்பு _qsort (hb_prioritize);
  }

  /*
   * Other methods.
   */

  size_t get_size () மாறிலி { திரும்பு length * இது->get_item_size (); }

  /*
   * Reverse the order of items in this array in the range [start, end).
   */
  வெறுமை reverse (unsigned start = 0, unsigned end = -1)
  {
    start = hb_min (start, length);
    end = hb_min (end, length);

    எனில் (end < start + 2)
      திரும்பு;

    unsigned stop = start + (end - start) / 2;
    க்கு (unsigned lhs = start, rhs = end - 1; lhs < stop; lhs++, rhs--)
      hb_swap (arrayZ[rhs], arrayZ[lhs]);
  }

  hb_array_t sub_array (unsigned முழு start_offset = 0, unsigned முழு *seg_மொத்தம் = nullptr /* IN/OUT */) மாறிலி
  {
    எனில் (!start_offset && !seg_மொத்தம்)
      திரும்பு *இது;

    unsigned முழு மொத்தம் = length;
    எனில் (unlikely (start_offset > மொத்தம்))
      மொத்தம் = 0;
    இல்லையெனில்
      மொத்தம் -= start_offset;
    எனில் (seg_மொத்தம்)
      மொத்தம் = *seg_மொத்தம் = hb_min (மொத்தம், *seg_மொத்தம்);
    திரும்பு hb_array_t (arrayZ + start_offset, மொத்தம்);
  }
  hb_array_t sub_array (unsigned முழு start_offset, unsigned முழு seg_மொத்தம்) மாறிலி
  { திரும்பு sub_array (start_offset, &seg_மொத்தம்); }

  hb_array_t truncate (unsigned length) மாறிலி { திரும்பு sub_array (0, length); }

  template <typename T,
	    unsigned P = அளவு (Type),
	    hb_enable_if (P == 1)>
  மாறிலி T *as () மாறிலி
  { திரும்பு length < hb_min_size (T) ? &Null (T) : reinterpret_cast<மாறிலி T *> (arrayZ); }

  template <typename T,
	    unsigned P = அளவு (Type),
	    hb_enable_if (P == 1)>
  தருக்க check_range (மாறிலி T *p, unsigned முழு size = T::static_size) மாறிலி
  {
    திரும்பு arrayZ <= ((மாறிலி எழுத்து *) p)
	&& ((மாறிலி எழுத்து *) p) <= arrayZ + length
	&& (unsigned முழு) (arrayZ + length - (மாறிலி எழுத்து *) p) >= size;
  }

  template <unsigned P = அளவு (Type),
	    hb_enable_if (P == 1)>
  தருக்க check_end (மாறிலி வெறுமை *p) மாறிலி
  {
    திரும்பு (uintptr_t) (((மாறிலி எழுத்து *) p) - arrayZ) <= length;
  }

  /* Only call if you allocated the underlying array using hb_malloc() or similar. */
  வெறுமை fini ()
  { hb_free ((வெறுமை *) arrayZ); arrayZ = nullptr; length = 0; }

  template <typename hb_serialize_context_t,
	    typename U = Type,
	    hb_enable_if (!(அளவு (U) < அளவு (long long) && hb_is_trivially_copy_assignable(hb_decay<Type>)))>
  hb_array_t copy (hb_serialize_context_t *c) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    auto* out = c->start_embed (arrayZ);
    எனில் (unlikely (!c->extend_size (out, get_size (), பொய்))) return_trace (hb_array_t ());
    க்கு (unsigned i = 0; i < length; i++)
      out[i] = arrayZ[i]; /* TODO: add version that calls c->copy() */
    return_trace (hb_array_t (out, length));
  }

  template <typename hb_serialize_context_t,
	    typename U = Type,
	    hb_enable_if (அளவு (U) < அளவு (long long) && hb_is_trivially_copy_assignable(hb_decay<Type>))>
  hb_array_t copy (hb_serialize_context_t *c) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    auto* out = c->start_embed (arrayZ);
    எனில் (unlikely (!c->extend_size (out, get_size (), பொய்))) return_trace (hb_array_t ());
    hb_memcpy (out, arrayZ, get_size ());
    return_trace (hb_array_t (out, length));
  }

  template <typename hb_sanitize_context_t>
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  { திரும்பு c->check_array (arrayZ, length); }

  /*
   * Members
   */

  பொது:
  Type *arrayZ = nullptr;
  unsigned முழு length = 0;
  unsigned முழு backwards_length = 0;
};
template <typename T> உள்வரி hb_array_t<T>
hb_array ()
{ திரும்பு hb_array_t<T> (); }
template <typename T> உள்வரி hb_array_t<T>
hb_array (T *array, unsigned முழு length)
{ திரும்பு hb_array_t<T> (array, length); }
template <typename T, unsigned முழு length_> உள்வரி hb_array_t<T>
hb_array (T (&array_)[length_])
{ திரும்பு hb_array_t<T> (array_); }

template <typename Type>
கட்டமைப்பு hb_sorted_array_t :
	hb_array_t<Type>,
	hb_iter_t<hb_sorted_array_t<Type>, Type&>
{
  வகைவரை hb_iter_t<hb_sorted_array_t, Type&> iter_base_t;
  HB_ITER_USING (iter_base_t);
  நிலையான constexpr தருக்க is_random_access_iterator = உண்மை;
  நிலையான constexpr தருக்க is_sorted_iterator = உண்மை;
  நிலையான constexpr தருக்க has_fast_len = உண்மை;

  hb_sorted_array_t () = இயல்பு;
  hb_sorted_array_t (மாறிலி hb_sorted_array_t&) = இயல்பு;
  ~hb_sorted_array_t () = இயல்பு;
  hb_sorted_array_t& operator= (மாறிலி hb_sorted_array_t&) = இயல்பு;
  hb_sorted_array_t& operator= (hb_sorted_array_t&&) = இயல்பு;

  constexpr hb_sorted_array_t (Type *array_, unsigned முழு length_) : hb_array_t<Type> (array_, length_) {}
  template <unsigned முழு length_>
  constexpr hb_sorted_array_t (Type (&array_)[length_]) : hb_array_t<Type> (array_) {}

  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_sorted_array_t (மாறிலி hb_array_t<U> &o) :
    hb_array_t<Type> (o),
    hb_iter_t<hb_sorted_array_t, Type&> () {}
  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_sorted_array_t& operator = (மாறிலி hb_array_t<U> &o)
  { hb_array_t<Type> (*இது) = o; திரும்பு *இது; }

  /* Iterator implementation. */

  /* See comment in hb_array_of::operator != */
  தருக்க operator != (மாறிலி hb_sorted_array_t& o) மாறிலி
  { திரும்பு இது->arrayZ != o.arrayZ || இது->length != o.length; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () மாறிலி { திரும்பு இது->arrayZ; }
  Type *end () மாறிலி { திரும்பு இது->arrayZ + இது->length; }


  hb_sorted_array_t sub_array (unsigned முழு start_offset, unsigned முழு *seg_மொத்தம் /* IN/OUT */) மாறிலி
  { திரும்பு hb_sorted_array_t (((மாறிலி hb_array_t<Type> *) (இது))->sub_array (start_offset, seg_மொத்தம்)); }
  hb_sorted_array_t sub_array (unsigned முழு start_offset, unsigned முழு seg_மொத்தம்) மாறிலி
  { திரும்பு sub_array (start_offset, &seg_மொத்தம்); }

  hb_sorted_array_t truncate (unsigned length) மாறிலி { திரும்பு sub_array (0, length); }

  template <typename T>
  Type *bsearch (மாறிலி T &x, Type *not_found = nullptr)
  {
    unsigned முழு i;
    திரும்பு bfind (x, &i) ? &இது->arrayZ[i] : not_found;
  }
  template <typename T>
  மாறிலி Type *bsearch (மாறிலி T &x, மாறிலி Type *not_found = nullptr) மாறிலி
  {
    unsigned முழு i;
    திரும்பு bfind (x, &i) ? &இது->arrayZ[i] : not_found;
  }
  template <typename T>
  தருக்க bfind (மாறிலி T &x, unsigned முழு *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned முழு to_store = (unsigned முழு) -1) மாறிலி
  {
    unsigned pos;

    எனில் (bsearch_impl (x, &pos))
    {
      எனில் (i)
	*i = pos;
      திரும்பு உண்மை;
    }

    எனில் (i)
    {
      தேர்வு (not_found)
      {
	நிலை HB_NOT_FOUND_DONT_STORE:
	  நிறுத்து;

	நிலை HB_NOT_FOUND_STORE:
	  *i = to_store;
	  நிறுத்து;

	நிலை HB_NOT_FOUND_STORE_CLOSEST:
	  *i = pos;
	  நிறுத்து;
      }
    }
    திரும்பு பொய்;
  }
  template <typename T, typename ...Ts>
  தருக்க bsearch_impl (மாறிலி T &x, unsigned *pos, Ts... ds) மாறிலி
  {
    திரும்பு hb_bsearch_impl (pos,
			    x,
			    இது->arrayZ,
			    இது->length,
			    அளவு (Type),
			    _hb_cmp_method<T, Type, Ts...>,
			    std::forward<Ts> (ds)...);
  }
};
template <typename T> உள்வரி hb_sorted_array_t<T>
hb_sorted_array (T *array, unsigned முழு length)
{ திரும்பு hb_sorted_array_t<T> (array, length); }
template <typename T, unsigned முழு length_> உள்வரி hb_sorted_array_t<T>
hb_sorted_array (T (&array_)[length_])
{ திரும்பு hb_sorted_array_t<T> (array_); }

template <typename T>
உள்வரி தருக்க hb_array_t<T>::operator == (மாறிலி hb_array_t<T> &o) மாறிலி
{
  எனில் (o.length != இது->length) திரும்பு பொய்;
  க்கு (unsigned முழு i = 0; i < இது->length; i++) {
    எனில் (இது->arrayZ[i] != o.arrayZ[i]) திரும்பு பொய்;
  }
  திரும்பு உண்மை;
}
template <>
உள்வரி தருக்க hb_array_t<மாறிலி எழுத்து>::operator == (மாறிலி hb_array_t<மாறிலி எழுத்து> &o) மாறிலி
{
  எனில் (o.length != இது->length) திரும்பு பொய்;
  திரும்பு 0 == hb_memcmp (arrayZ, o.arrayZ, length);
}
template <>
உள்வரி தருக்க hb_array_t<மாறிலி unsigned எழுத்து>::operator == (மாறிலி hb_array_t<மாறிலி unsigned எழுத்து> &o) மாறிலி
{
  எனில் (o.length != இது->length) திரும்பு பொய்;
  திரும்பு 0 == hb_memcmp (arrayZ, o.arrayZ, length);
}


/* Specialize hash() for byte arrays. */

#ifndef HB_OPTIMIZE_SIZE_MORE
template <>
உள்வரி uint32_t hb_array_t<மாறிலி எழுத்து>::hash () மாறிலி
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  திரும்பு fasthash32(arrayZ, length, 0xf437ffe6 /* magic? */);
}

template <>
உள்வரி uint32_t hb_array_t<மாறிலி unsigned எழுத்து>::hash () மாறிலி
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  திரும்பு fasthash32(arrayZ, length, 0xf437ffe6 /* magic? */);
}
#endif


வகைவரை hb_array_t<மாறிலி எழுத்து> hb_bytes_t;
வகைவரை hb_array_t<மாறிலி unsigned எழுத்து> hb_ubytes_t;



#endif /* HB_ARRAY_HH */

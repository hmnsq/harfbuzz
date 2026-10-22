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

#અન્તર્ભાવ "hb.hh"
#અન્તર્ભાવ "hb-algs.hh"
#અન્તર્ભાવ "hb-iter.hh"
#અન્તર્ભાવ "hb-null.hh"


template <typename Type>
સંરચના hb_sorted_array_t;

ગણના hb_not_found_t
{
  HB_NOT_FOUND_DONT_STORE,
  HB_NOT_FOUND_STORE,
  HB_NOT_FOUND_STORE_CLOSEST,
};


template <typename Type>
સંરચના hb_array_t : hb_iter_with_fallback_t<hb_array_t<Type>, Type&>
{
  સ્થિર constexpr તાર્કિક realloc_move = સાચું;

  /*
   * Constructors.
   */
  hb_array_t () = મૂળભૂત;
  hb_array_t (અચલ hb_array_t&) = મૂળભૂત;
  ~hb_array_t () = મૂળભૂત;
  hb_array_t& operator= (અચલ hb_array_t&) = મૂળભૂત;
  hb_array_t& operator= (hb_array_t&&) = મૂળભૂત;

  constexpr hb_array_t (Type *array_, unsigned પૂર્ણાંક length_) : arrayZ (array_), length (length_) {}
  template <unsigned પૂર્ણાંક length_>
  constexpr hb_array_t (Type (&array_)[length_]) : hb_array_t (array_, length_) {}

  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_array_t (અચલ hb_array_t<U> &o) :
    hb_iter_with_fallback_t<hb_array_t, Type&> (),
    arrayZ (o.arrayZ), length (o.length), backwards_length (o.backwards_length) {}
  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_array_t& operator = (અચલ hb_array_t<U> &o)
  { arrayZ = o.arrayZ; length = o.length; backwards_length = o.backwards_length; પાછા *આ; }

  /*
   * Iterator implementation.
   */
  પ્રકારવ્યાખ્યા Type& __item_t__;
  સ્થિર constexpr તાર્કિક is_random_access_iterator = સાચું;
  સ્થિર constexpr તાર્કિક has_fast_len = સાચું;
  Type& __item__ () અચલ
  {
    જો (unlikely (!length)) પાછા CrapOrNull (Type);
    પાછા *arrayZ;
  }
  Type& __item_at__ (unsigned i) અચલ
  {
    જો (unlikely (i >= length)) પાછા CrapOrNull (Type);
    પાછા arrayZ[i];
  }
  શૂન્ય __next__ ()
  {
    જો (unlikely (!length))
      પાછા;
    length--;
    backwards_length++;
    arrayZ++;
  }
  શૂન્ય __forward__ (unsigned n)
  {
    જો (unlikely (n > length))
      n = length;
    length -= n;
    backwards_length += n;
    arrayZ += n;
  }
  શૂન્ય __prev__ ()
  {
    જો (unlikely (!backwards_length))
      પાછા;
    length++;
    backwards_length--;
    arrayZ--;
  }
  શૂન્ય __rewind__ (unsigned n)
  {
    જો (unlikely (n > backwards_length))
      n = backwards_length;
    length += n;
    backwards_length -= n;
    arrayZ -= n;
  }
  unsigned __len__ () અચલ { પાછા length; }
  /* Ouch. The operator== compares the contents of the array.  For range-based for loops,
   * it's best if we can just compare arrayZ, though comparing contents is still fast,
   * but also would require that Type has operator==.  As such, we optimize this operator
   * for range-based for loop and just compare arrayZ and length.
   *
   * The above comment is outdated now because we implemented separate begin/end to
   * objects that were using hb_array_t for range-based loop before. */
  તાર્કિક operator != (અચલ hb_array_t& o) અચલ
  { પાછા આ->arrayZ != o.arrayZ || આ->length != o.length; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () અચલ { પાછા arrayZ; }
  Type *end () અચલ { પાછા arrayZ + length; }


  /* Extra operators.
   */
  Type * operator & () અચલ { પાછા arrayZ; }
  operator hb_array_t<અચલ Type> () { પાછા hb_array_t<અચલ Type> (arrayZ, length); }
  template <typename T> operator T * () અચલ { પાછા arrayZ; }

  HB_INTERNAL તાર્કિક operator == (અચલ hb_array_t &o) અચલ;

  uint32_t hash () અચલ
  {
    // FNV-1a hash function
    // https://github.com/harfbuzz/harfbuzz/pull/4228
    uint32_t current = /*cbf29ce4*/0x84222325;
    માટે (auto &v : *આ)
    {
      current = current ^ hb_hash (v);
      current = current * 16777619;
    }
    પાછા current;
  }

  /*
   * Compare, Sort, and Search.
   */

  /* Note: our compare is NOT lexicographic; it also does NOT call Type::cmp. */
  પૂર્ણાંક cmp (અચલ hb_array_t &a) અચલ
  {
    જો (length != a.length)
      પાછા (પૂર્ણાંક) a.length - (પૂર્ણાંક) length;
    પાછા hb_memcmp (a.arrayZ, arrayZ, get_size ());
  }
  HB_INTERNAL સ્થિર પૂર્ણાંક cmp (અચલ શૂન્ય *pa, અચલ શૂન્ય *pb)
  {
    hb_array_t *a = (hb_array_t *) pa;
    hb_array_t *b = (hb_array_t *) pb;
    પાછા b->cmp (*a);
  }

  template <typename T>
  Type *lsearch (અચલ T &x, Type *not_found = nullptr)
  {
    unsigned i;
    પાછા lfind (x, &i) ? &આ->arrayZ[i] : not_found;
  }
  template <typename T>
  અચલ Type *lsearch (અચલ T &x, અચલ Type *not_found = nullptr) અચલ
  {
    unsigned i;
    પાછા lfind (x, &i) ? &આ->arrayZ[i] : not_found;
  }
  template <typename T>
  તાર્કિક lfind (અચલ T &x, unsigned *pos = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned પૂર્ણાંક to_store = (unsigned પૂર્ણાંક) -1) અચલ
  {
    માટે (unsigned i = 0; i < length; ++i)
      જો (hb_equal (x, આ->arrayZ[i]))
      {
	જો (pos)
	  *pos = i;
	પાછા સાચું;
      }

    જો (pos)
    {
      પસંદગી (not_found)
      {
	સ્થિતિ HB_NOT_FOUND_DONT_STORE:
	  તોડો;

	સ્થિતિ HB_NOT_FOUND_STORE:
	  *pos = to_store;
	  તોડો;

	સ્થિતિ HB_NOT_FOUND_STORE_CLOSEST:
	  *pos = length;
	  તોડો;
      }
    }
    પાછા ખોટું;
  }

  hb_sorted_array_t<Type> qsort (પૂર્ણાંક (*cmp)(અચલ શૂન્ય*, અચલ શૂન્ય*))
  {
    જો (likely (length))
      hb_qsort (arrayZ, length, આ->get_item_size (), cmp);
    પાછા hb_sorted_array_t<Type> (*આ);
  }

  /* Comparator follows the C qsort convention: returns
   * negative / zero / positive int. */
  template <typename Compar>
  hb_sorted_array_t<Type> qsort (Compar compar)
  {
    જો (likely (length))
      hb_qsort_inline (arrayZ, length, compar);
    પાછા hb_sorted_array_t<Type> (*આ);
  }

  ખાનગી:
  template <typename T = Type,
	    hb_enable_if (std::is_move_assignable<T>::value)>
  hb_sorted_array_t<Type> _qsort (hb_priority<1>)
  {
    પાછા qsort ([] (અચલ Type &a, અચલ Type &b) { પાછા Type::cmp (&a, &b); });
  }
  hb_sorted_array_t<Type> _qsort (hb_priority<0>)
  {
    પાછા qsort ((પૂર્ણાંક(*)(અચલ શૂન્ય*, અચલ શૂન્ય*)) Type::cmp);
  }
  જાહેર:

  hb_sorted_array_t<Type> qsort ()
  {
    પાછા _qsort (hb_prioritize);
  }

  /*
   * Other methods.
   */

  size_t get_size () અચલ { પાછા length * આ->get_item_size (); }

  /*
   * Reverse the order of items in this array in the range [start, end).
   */
  શૂન્ય reverse (unsigned start = 0, unsigned end = -1)
  {
    start = hb_min (start, length);
    end = hb_min (end, length);

    જો (end < start + 2)
      પાછા;

    unsigned stop = start + (end - start) / 2;
    માટે (unsigned lhs = start, rhs = end - 1; lhs < stop; lhs++, rhs--)
      hb_swap (arrayZ[rhs], arrayZ[lhs]);
  }

  hb_array_t sub_array (unsigned પૂર્ણાંક start_offset = 0, unsigned પૂર્ણાંક *seg_ગણતરી = nullptr /* IN/OUT */) અચલ
  {
    જો (!start_offset && !seg_ગણતરી)
      પાછા *આ;

    unsigned પૂર્ણાંક ગણતરી = length;
    જો (unlikely (start_offset > ગણતરી))
      ગણતરી = 0;
    નહીંતર
      ગણતરી -= start_offset;
    જો (seg_ગણતરી)
      ગણતરી = *seg_ગણતરી = hb_min (ગણતરી, *seg_ગણતરી);
    પાછા hb_array_t (arrayZ + start_offset, ગણતરી);
  }
  hb_array_t sub_array (unsigned પૂર્ણાંક start_offset, unsigned પૂર્ણાંક seg_ગણતરી) અચલ
  { પાછા sub_array (start_offset, &seg_ગણતરી); }

  hb_array_t truncate (unsigned length) અચલ { પાછા sub_array (0, length); }

  template <typename T,
	    unsigned P = કદ (Type),
	    hb_enable_if (P == 1)>
  અચલ T *as () અચલ
  { પાછા length < hb_min_size (T) ? &Null (T) : reinterpret_cast<અચલ T *> (arrayZ); }

  template <typename T,
	    unsigned P = કદ (Type),
	    hb_enable_if (P == 1)>
  તાર્કિક check_range (અચલ T *p, unsigned પૂર્ણાંક size = T::static_size) અચલ
  {
    પાછા arrayZ <= ((અચલ અક્ષર *) p)
	&& ((અચલ અક્ષર *) p) <= arrayZ + length
	&& (unsigned પૂર્ણાંક) (arrayZ + length - (અચલ અક્ષર *) p) >= size;
  }

  template <unsigned P = કદ (Type),
	    hb_enable_if (P == 1)>
  તાર્કિક check_end (અચલ શૂન્ય *p) અચલ
  {
    પાછા (uintptr_t) (((અચલ અક્ષર *) p) - arrayZ) <= length;
  }

  /* Only call if you allocated the underlying array using hb_malloc() or similar. */
  શૂન્ય fini ()
  { hb_free ((શૂન્ય *) arrayZ); arrayZ = nullptr; length = 0; }

  template <typename hb_serialize_context_t,
	    typename U = Type,
	    hb_enable_if (!(કદ (U) < કદ (long long) && hb_is_trivially_copy_assignable(hb_decay<Type>)))>
  hb_array_t copy (hb_serialize_context_t *c) અચલ
  {
    TRACE_SERIALIZE (આ);
    auto* out = c->start_embed (arrayZ);
    જો (unlikely (!c->extend_size (out, get_size (), ખોટું))) return_trace (hb_array_t ());
    માટે (unsigned i = 0; i < length; i++)
      out[i] = arrayZ[i]; /* TODO: add version that calls c->copy() */
    return_trace (hb_array_t (out, length));
  }

  template <typename hb_serialize_context_t,
	    typename U = Type,
	    hb_enable_if (કદ (U) < કદ (long long) && hb_is_trivially_copy_assignable(hb_decay<Type>))>
  hb_array_t copy (hb_serialize_context_t *c) અચલ
  {
    TRACE_SERIALIZE (આ);
    auto* out = c->start_embed (arrayZ);
    જો (unlikely (!c->extend_size (out, get_size (), ખોટું))) return_trace (hb_array_t ());
    hb_memcpy (out, arrayZ, get_size ());
    return_trace (hb_array_t (out, length));
  }

  template <typename hb_sanitize_context_t>
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  { પાછા c->check_array (arrayZ, length); }

  /*
   * Members
   */

  જાહેર:
  Type *arrayZ = nullptr;
  unsigned પૂર્ણાંક length = 0;
  unsigned પૂર્ણાંક backwards_length = 0;
};
template <typename T> અંતઃરેખ hb_array_t<T>
hb_array ()
{ પાછા hb_array_t<T> (); }
template <typename T> અંતઃરેખ hb_array_t<T>
hb_array (T *array, unsigned પૂર્ણાંક length)
{ પાછા hb_array_t<T> (array, length); }
template <typename T, unsigned પૂર્ણાંક length_> અંતઃરેખ hb_array_t<T>
hb_array (T (&array_)[length_])
{ પાછા hb_array_t<T> (array_); }

template <typename Type>
સંરચના hb_sorted_array_t :
	hb_array_t<Type>,
	hb_iter_t<hb_sorted_array_t<Type>, Type&>
{
  પ્રકારવ્યાખ્યા hb_iter_t<hb_sorted_array_t, Type&> iter_base_t;
  HB_ITER_USING (iter_base_t);
  સ્થિર constexpr તાર્કિક is_random_access_iterator = સાચું;
  સ્થિર constexpr તાર્કિક is_sorted_iterator = સાચું;
  સ્થિર constexpr તાર્કિક has_fast_len = સાચું;

  hb_sorted_array_t () = મૂળભૂત;
  hb_sorted_array_t (અચલ hb_sorted_array_t&) = મૂળભૂત;
  ~hb_sorted_array_t () = મૂળભૂત;
  hb_sorted_array_t& operator= (અચલ hb_sorted_array_t&) = મૂળભૂત;
  hb_sorted_array_t& operator= (hb_sorted_array_t&&) = મૂળભૂત;

  constexpr hb_sorted_array_t (Type *array_, unsigned પૂર્ણાંક length_) : hb_array_t<Type> (array_, length_) {}
  template <unsigned પૂર્ણાંક length_>
  constexpr hb_sorted_array_t (Type (&array_)[length_]) : hb_array_t<Type> (array_) {}

  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_sorted_array_t (અચલ hb_array_t<U> &o) :
    hb_array_t<Type> (o),
    hb_iter_t<hb_sorted_array_t, Type&> () {}
  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_sorted_array_t& operator = (અચલ hb_array_t<U> &o)
  { hb_array_t<Type> (*આ) = o; પાછા *આ; }

  /* Iterator implementation. */

  /* See comment in hb_array_of::operator != */
  તાર્કિક operator != (અચલ hb_sorted_array_t& o) અચલ
  { પાછા આ->arrayZ != o.arrayZ || આ->length != o.length; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () અચલ { પાછા આ->arrayZ; }
  Type *end () અચલ { પાછા આ->arrayZ + આ->length; }


  hb_sorted_array_t sub_array (unsigned પૂર્ણાંક start_offset, unsigned પૂર્ણાંક *seg_ગણતરી /* IN/OUT */) અચલ
  { પાછા hb_sorted_array_t (((અચલ hb_array_t<Type> *) (આ))->sub_array (start_offset, seg_ગણતરી)); }
  hb_sorted_array_t sub_array (unsigned પૂર્ણાંક start_offset, unsigned પૂર્ણાંક seg_ગણતરી) અચલ
  { પાછા sub_array (start_offset, &seg_ગણતરી); }

  hb_sorted_array_t truncate (unsigned length) અચલ { પાછા sub_array (0, length); }

  template <typename T>
  Type *bsearch (અચલ T &x, Type *not_found = nullptr)
  {
    unsigned પૂર્ણાંક i;
    પાછા bfind (x, &i) ? &આ->arrayZ[i] : not_found;
  }
  template <typename T>
  અચલ Type *bsearch (અચલ T &x, અચલ Type *not_found = nullptr) અચલ
  {
    unsigned પૂર્ણાંક i;
    પાછા bfind (x, &i) ? &આ->arrayZ[i] : not_found;
  }
  template <typename T>
  તાર્કિક bfind (અચલ T &x, unsigned પૂર્ણાંક *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned પૂર્ણાંક to_store = (unsigned પૂર્ણાંક) -1) અચલ
  {
    unsigned pos;

    જો (bsearch_impl (x, &pos))
    {
      જો (i)
	*i = pos;
      પાછા સાચું;
    }

    જો (i)
    {
      પસંદગી (not_found)
      {
	સ્થિતિ HB_NOT_FOUND_DONT_STORE:
	  તોડો;

	સ્થિતિ HB_NOT_FOUND_STORE:
	  *i = to_store;
	  તોડો;

	સ્થિતિ HB_NOT_FOUND_STORE_CLOSEST:
	  *i = pos;
	  તોડો;
      }
    }
    પાછા ખોટું;
  }
  template <typename T, typename ...Ts>
  તાર્કિક bsearch_impl (અચલ T &x, unsigned *pos, Ts... ds) અચલ
  {
    પાછા hb_bsearch_impl (pos,
			    x,
			    આ->arrayZ,
			    આ->length,
			    કદ (Type),
			    _hb_cmp_method<T, Type, Ts...>,
			    std::forward<Ts> (ds)...);
  }
};
template <typename T> અંતઃરેખ hb_sorted_array_t<T>
hb_sorted_array (T *array, unsigned પૂર્ણાંક length)
{ પાછા hb_sorted_array_t<T> (array, length); }
template <typename T, unsigned પૂર્ણાંક length_> અંતઃરેખ hb_sorted_array_t<T>
hb_sorted_array (T (&array_)[length_])
{ પાછા hb_sorted_array_t<T> (array_); }

template <typename T>
અંતઃરેખ તાર્કિક hb_array_t<T>::operator == (અચલ hb_array_t<T> &o) અચલ
{
  જો (o.length != આ->length) પાછા ખોટું;
  માટે (unsigned પૂર્ણાંક i = 0; i < આ->length; i++) {
    જો (આ->arrayZ[i] != o.arrayZ[i]) પાછા ખોટું;
  }
  પાછા સાચું;
}
template <>
અંતઃરેખ તાર્કિક hb_array_t<અચલ અક્ષર>::operator == (અચલ hb_array_t<અચલ અક્ષર> &o) અચલ
{
  જો (o.length != આ->length) પાછા ખોટું;
  પાછા 0 == hb_memcmp (arrayZ, o.arrayZ, length);
}
template <>
અંતઃરેખ તાર્કિક hb_array_t<અચલ unsigned અક્ષર>::operator == (અચલ hb_array_t<અચલ unsigned અક્ષર> &o) અચલ
{
  જો (o.length != આ->length) પાછા ખોટું;
  પાછા 0 == hb_memcmp (arrayZ, o.arrayZ, length);
}


/* Specialize hash() for byte arrays. */

#ifndef HB_OPTIMIZE_SIZE_MORE
template <>
અંતઃરેખ uint32_t hb_array_t<અચલ અક્ષર>::hash () અચલ
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  પાછા fasthash32(arrayZ, length, 0xf437ffe6 /* magic? */);
}

template <>
અંતઃરેખ uint32_t hb_array_t<અચલ unsigned અક્ષર>::hash () અચલ
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  પાછા fasthash32(arrayZ, length, 0xf437ffe6 /* magic? */);
}
#endif


પ્રકારવ્યાખ્યા hb_array_t<અચલ અક્ષર> hb_bytes_t;
પ્રકારવ્યાખ્યા hb_array_t<અચલ unsigned અક્ષર> hb_ubytes_t;



#endif /* HB_ARRAY_HH */

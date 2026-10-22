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

#include "hb.hh"
#include "hb-algs.hh"
#include "hb-iter.hh"
#include "hb-null.hh"


template <typename Type>
sa_nracanaa hb_sorted_array_t;

ga_nanaa hb_not_found_t
{
  HB_NOT_FOUND_DONT_STORE,
  HB_NOT_FOUND_STORE,
  HB_NOT_FOUND_STORE_CLOSEST,
};


template <typename Type>
sa_nracanaa hb_array_t : hb_iter_with_fallback_t<hb_array_t<Type>, Type&>
{
  static constexpr bool realloc_move = true;

  /*
   * Constructors.
   */
  hb_array_t () = default;
  hb_array_t (const hb_array_t&) = default;
  ~hb_array_t () = default;
  hb_array_t& operator= (const hb_array_t&) = default;
  hb_array_t& operator= (hb_array_t&&) = default;

  constexpr hb_array_t (Type *array_, unsigned จำนวนเต็ม length_) : arrayZ (array_), length (length_) {}
  template <unsigned จำนวนเต็ม length_>
  constexpr hb_array_t (Type (&array_)[length_]) : hb_array_t (array_, length_) {}

  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_array_t (const hb_array_t<U> &o) :
    hb_iter_with_fallback_t<hb_array_t, Type&> (),
    arrayZ (o.arrayZ), length (o.length), backwards_length (o.backwards_length) {}
  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_array_t& operator = (const hb_array_t<U> &o)
  { arrayZ = o.arrayZ; length = o.length; backwards_length = o.backwards_length; คืน *this; }

  /*
   * Iterator implementation.
   */
  typedef Type& __item_t__;
  static constexpr bool is_random_access_iterator = true;
  static constexpr bool has_fast_len = true;
  Type& __item__ () const
  {
    ถ้า (unlikely (!length)) คืน CrapOrNull (Type);
    คืน *arrayZ;
  }
  Type& __item_at__ (unsigned i) const
  {
    ถ้า (unlikely (i >= length)) คืน CrapOrNull (Type);
    คืน arrayZ[i];
  }
  void __next__ ()
  {
    ถ้า (unlikely (!length))
      คืน;
    length--;
    backwards_length++;
    arrayZ++;
  }
  void __forward__ (unsigned n)
  {
    ถ้า (unlikely (n > length))
      n = length;
    length -= n;
    backwards_length += n;
    arrayZ += n;
  }
  void __prev__ ()
  {
    ถ้า (unlikely (!backwards_length))
      คืน;
    length++;
    backwards_length--;
    arrayZ--;
  }
  void __rewind__ (unsigned n)
  {
    ถ้า (unlikely (n > backwards_length))
      n = backwards_length;
    length += n;
    backwards_length -= n;
    arrayZ -= n;
  }
  unsigned __len__ () const { คืน length; }
  /* Ouch. The operator== compares the contents of the array.  For range-based for loops,
   * it's best if we can just compare arrayZ, though comparing contents is still fast,
   * but also would require that Type has operator==.  As such, we optimize this operator
   * for range-based for loop and just compare arrayZ and length.
   *
   * The above comment is outdated now because we implemented separate begin/end to
   * objects that were using hb_array_t for range-based loop before. */
  bool operator != (const hb_array_t& o) const
  { คืน this->arrayZ != o.arrayZ || this->length != o.length; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () const { คืน arrayZ; }
  Type *end () const { คืน arrayZ + length; }


  /* Extra operators.
   */
  Type * operator & () const { คืน arrayZ; }
  operator hb_array_t<const Type> () { คืน hb_array_t<const Type> (arrayZ, length); }
  template <typename T> operator T * () const { คืน arrayZ; }

  HB_INTERNAL bool operator == (const hb_array_t &o) const;

  uint32_t hash () const
  {
    // FNV-1a hash function
    // https://github.com/harfbuzz/harfbuzz/pull/4228
    uint32_t current = /*cbf29ce4*/0x84222325;
    สำหรับ (auto &v : *this)
    {
      current = current ^ hb_hash (v);
      current = current * 16777619;
    }
    คืน current;
  }

  /*
   * Compare, Sort, and Search.
   */

  /* Note: our compare is NOT lexicographic; it also does NOT call Type::cmp. */
  จำนวนเต็ม cmp (const hb_array_t &a) const
  {
    ถ้า (length != a.length)
      คืน (จำนวนเต็ม) a.length - (จำนวนเต็ม) length;
    คืน hb_memcmp (a.arrayZ, arrayZ, get_size ());
  }
  HB_INTERNAL static จำนวนเต็ม cmp (const void *pa, const void *pb)
  {
    hb_array_t *a = (hb_array_t *) pa;
    hb_array_t *b = (hb_array_t *) pb;
    คืน b->cmp (*a);
  }

  template <typename T>
  Type *lsearch (const T &x, Type *not_found = nullptr)
  {
    unsigned i;
    คืน lfind (x, &i) ? &this->arrayZ[i] : not_found;
  }
  template <typename T>
  const Type *lsearch (const T &x, const Type *not_found = nullptr) const
  {
    unsigned i;
    คืน lfind (x, &i) ? &this->arrayZ[i] : not_found;
  }
  template <typename T>
  bool lfind (const T &x, unsigned *pos = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned จำนวนเต็ม to_store = (unsigned จำนวนเต็ม) -1) const
  {
    สำหรับ (unsigned i = 0; i < length; ++i)
      ถ้า (hb_equal (x, this->arrayZ[i]))
      {
	ถ้า (pos)
	  *pos = i;
	คืน true;
      }

    ถ้า (pos)
    {
      switch (not_found)
      {
	pak_sa HB_NOT_FOUND_DONT_STORE:
	  bha_nga;

	pak_sa HB_NOT_FOUND_STORE:
	  *pos = to_store;
	  bha_nga;

	pak_sa HB_NOT_FOUND_STORE_CLOSEST:
	  *pos = length;
	  bha_nga;
      }
    }
    คืน false;
  }

  hb_sorted_array_t<Type> qsort (จำนวนเต็ม (*cmp)(const void*, const void*))
  {
    ถ้า (likely (length))
      hb_qsort (arrayZ, length, this->get_item_size (), cmp);
    คืน hb_sorted_array_t<Type> (*this);
  }

  /* Comparator follows the C qsort convention: returns
   * negative / zero / positive int. */
  template <typename Compar>
  hb_sorted_array_t<Type> qsort (Compar compar)
  {
    ถ้า (likely (length))
      hb_qsort_inline (arrayZ, length, compar);
    คืน hb_sorted_array_t<Type> (*this);
  }

  private:
  template <typename T = Type,
	    hb_enable_if (std::is_move_assignable<T>::value)>
  hb_sorted_array_t<Type> _qsort (hb_priority<1>)
  {
    คืน qsort ([] (const Type &a, const Type &b) { คืน Type::cmp (&a, &b); });
  }
  hb_sorted_array_t<Type> _qsort (hb_priority<0>)
  {
    คืน qsort ((จำนวนเต็ม(*)(const void*, const void*)) Type::cmp);
  }
  public:

  hb_sorted_array_t<Type> qsort ()
  {
    คืน _qsort (hb_prioritize);
  }

  /*
   * Other methods.
   */

  size_t get_size () const { คืน length * this->get_item_size (); }

  /*
   * Reverse the order of items in this array in the range [start, end).
   */
  void reverse (unsigned start = 0, unsigned end = -1)
  {
    start = hb_min (start, length);
    end = hb_min (end, length);

    ถ้า (end < start + 2)
      คืน;

    unsigned stop = start + (end - start) / 2;
    สำหรับ (unsigned lhs = start, rhs = end - 1; lhs < stop; lhs++, rhs--)
      hb_swap (arrayZ[rhs], arrayZ[lhs]);
  }

  hb_array_t sub_array (unsigned จำนวนเต็ม start_offset = 0, unsigned จำนวนเต็ม *seg_count = nullptr /* IN/OUT */) const
  {
    ถ้า (!start_offset && !seg_count)
      คืน *this;

    unsigned จำนวนเต็ม count = length;
    ถ้า (unlikely (start_offset > count))
      count = 0;
    มิฉะนั้น
      count -= start_offset;
    ถ้า (seg_count)
      count = *seg_count = hb_min (count, *seg_count);
    คืน hb_array_t (arrayZ + start_offset, count);
  }
  hb_array_t sub_array (unsigned จำนวนเต็ม start_offset, unsigned จำนวนเต็ม seg_count) const
  { คืน sub_array (start_offset, &seg_count); }

  hb_array_t truncate (unsigned length) const { คืน sub_array (0, length); }

  template <typename T,
	    unsigned P = parimaa_na (Type),
	    hb_enable_if (P == 1)>
  const T *as () const
  { คืน length < hb_min_size (T) ? &Null (T) : reinterpret_cast<const T *> (arrayZ); }

  template <typename T,
	    unsigned P = parimaa_na (Type),
	    hb_enable_if (P == 1)>
  bool check_range (const T *p, unsigned จำนวนเต็ม size = T::static_size) const
  {
    คืน arrayZ <= ((const var_na *) p)
	&& ((const var_na *) p) <= arrayZ + length
	&& (unsigned จำนวนเต็ม) (arrayZ + length - (const var_na *) p) >= size;
  }

  template <unsigned P = parimaa_na (Type),
	    hb_enable_if (P == 1)>
  bool check_end (const void *p) const
  {
    คืน (uintptr_t) (((const var_na *) p) - arrayZ) <= length;
  }

  /* Only call if you allocated the underlying array using hb_malloc() or similar. */
  void fini ()
  { hb_free ((void *) arrayZ); arrayZ = nullptr; length = 0; }

  template <typename hb_serialize_context_t,
	    typename U = Type,
	    hb_enable_if (!(parimaa_na (U) < parimaa_na (long long) && hb_is_trivially_copy_assignable(hb_decay<Type>)))>
  hb_array_t copy (hb_serialize_context_t *c) const
  {
    TRACE_SERIALIZE (this);
    auto* out = c->start_embed (arrayZ);
    ถ้า (unlikely (!c->extend_size (out, get_size (), false))) return_trace (hb_array_t ());
    สำหรับ (unsigned i = 0; i < length; i++)
      out[i] = arrayZ[i]; /* TODO: add version that calls c->copy() */
    return_trace (hb_array_t (out, length));
  }

  template <typename hb_serialize_context_t,
	    typename U = Type,
	    hb_enable_if (parimaa_na (U) < parimaa_na (long long) && hb_is_trivially_copy_assignable(hb_decay<Type>))>
  hb_array_t copy (hb_serialize_context_t *c) const
  {
    TRACE_SERIALIZE (this);
    auto* out = c->start_embed (arrayZ);
    ถ้า (unlikely (!c->extend_size (out, get_size (), false))) return_trace (hb_array_t ());
    hb_memcpy (out, arrayZ, get_size ());
    return_trace (hb_array_t (out, length));
  }

  template <typename hb_sanitize_context_t>
  bool sanitize (hb_sanitize_context_t *c) const
  { คืน c->check_array (arrayZ, length); }

  /*
   * Members
   */

  public:
  Type *arrayZ = nullptr;
  unsigned จำนวนเต็ม length = 0;
  unsigned จำนวนเต็ม backwards_length = 0;
};
template <typename T> anta_hstha hb_array_t<T>
hb_array ()
{ คืน hb_array_t<T> (); }
template <typename T> anta_hstha hb_array_t<T>
hb_array (T *array, unsigned จำนวนเต็ม length)
{ คืน hb_array_t<T> (array, length); }
template <typename T, unsigned จำนวนเต็ม length_> anta_hstha hb_array_t<T>
hb_array (T (&array_)[length_])
{ คืน hb_array_t<T> (array_); }

template <typename Type>
sa_nracanaa hb_sorted_array_t :
	hb_array_t<Type>,
	hb_iter_t<hb_sorted_array_t<Type>, Type&>
{
  typedef hb_iter_t<hb_sorted_array_t, Type&> iter_base_t;
  HB_ITER_USING (iter_base_t);
  static constexpr bool is_random_access_iterator = true;
  static constexpr bool is_sorted_iterator = true;
  static constexpr bool has_fast_len = true;

  hb_sorted_array_t () = default;
  hb_sorted_array_t (const hb_sorted_array_t&) = default;
  ~hb_sorted_array_t () = default;
  hb_sorted_array_t& operator= (const hb_sorted_array_t&) = default;
  hb_sorted_array_t& operator= (hb_sorted_array_t&&) = default;

  constexpr hb_sorted_array_t (Type *array_, unsigned จำนวนเต็ม length_) : hb_array_t<Type> (array_, length_) {}
  template <unsigned จำนวนเต็ม length_>
  constexpr hb_sorted_array_t (Type (&array_)[length_]) : hb_array_t<Type> (array_) {}

  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_sorted_array_t (const hb_array_t<U> &o) :
    hb_array_t<Type> (o),
    hb_iter_t<hb_sorted_array_t, Type&> () {}
  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_sorted_array_t& operator = (const hb_array_t<U> &o)
  { hb_array_t<Type> (*this) = o; คืน *this; }

  /* Iterator implementation. */

  /* See comment in hb_array_of::operator != */
  bool operator != (const hb_sorted_array_t& o) const
  { คืน this->arrayZ != o.arrayZ || this->length != o.length; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () const { คืน this->arrayZ; }
  Type *end () const { คืน this->arrayZ + this->length; }


  hb_sorted_array_t sub_array (unsigned จำนวนเต็ม start_offset, unsigned จำนวนเต็ม *seg_count /* IN/OUT */) const
  { คืน hb_sorted_array_t (((const hb_array_t<Type> *) (this))->sub_array (start_offset, seg_count)); }
  hb_sorted_array_t sub_array (unsigned จำนวนเต็ม start_offset, unsigned จำนวนเต็ม seg_count) const
  { คืน sub_array (start_offset, &seg_count); }

  hb_sorted_array_t truncate (unsigned length) const { คืน sub_array (0, length); }

  template <typename T>
  Type *bsearch (const T &x, Type *not_found = nullptr)
  {
    unsigned จำนวนเต็ม i;
    คืน bfind (x, &i) ? &this->arrayZ[i] : not_found;
  }
  template <typename T>
  const Type *bsearch (const T &x, const Type *not_found = nullptr) const
  {
    unsigned จำนวนเต็ม i;
    คืน bfind (x, &i) ? &this->arrayZ[i] : not_found;
  }
  template <typename T>
  bool bfind (const T &x, unsigned จำนวนเต็ม *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned จำนวนเต็ม to_store = (unsigned จำนวนเต็ม) -1) const
  {
    unsigned pos;

    ถ้า (bsearch_impl (x, &pos))
    {
      ถ้า (i)
	*i = pos;
      คืน true;
    }

    ถ้า (i)
    {
      switch (not_found)
      {
	pak_sa HB_NOT_FOUND_DONT_STORE:
	  bha_nga;

	pak_sa HB_NOT_FOUND_STORE:
	  *i = to_store;
	  bha_nga;

	pak_sa HB_NOT_FOUND_STORE_CLOSEST:
	  *i = pos;
	  bha_nga;
      }
    }
    คืน false;
  }
  template <typename T, typename ...Ts>
  bool bsearch_impl (const T &x, unsigned *pos, Ts... ds) const
  {
    คืน hb_bsearch_impl (pos,
			    x,
			    this->arrayZ,
			    this->length,
			    parimaa_na (Type),
			    _hb_cmp_method<T, Type, Ts...>,
			    std::forward<Ts> (ds)...);
  }
};
template <typename T> anta_hstha hb_sorted_array_t<T>
hb_sorted_array (T *array, unsigned จำนวนเต็ม length)
{ คืน hb_sorted_array_t<T> (array, length); }
template <typename T, unsigned จำนวนเต็ม length_> anta_hstha hb_sorted_array_t<T>
hb_sorted_array (T (&array_)[length_])
{ คืน hb_sorted_array_t<T> (array_); }

template <typename T>
anta_hstha bool hb_array_t<T>::operator == (const hb_array_t<T> &o) const
{
  ถ้า (o.length != this->length) คืน false;
  สำหรับ (unsigned จำนวนเต็ม i = 0; i < this->length; i++) {
    ถ้า (this->arrayZ[i] != o.arrayZ[i]) คืน false;
  }
  คืน true;
}
template <>
anta_hstha bool hb_array_t<const var_na>::operator == (const hb_array_t<const var_na> &o) const
{
  ถ้า (o.length != this->length) คืน false;
  คืน 0 == hb_memcmp (arrayZ, o.arrayZ, length);
}
template <>
anta_hstha bool hb_array_t<const unsigned var_na>::operator == (const hb_array_t<const unsigned var_na> &o) const
{
  ถ้า (o.length != this->length) คืน false;
  คืน 0 == hb_memcmp (arrayZ, o.arrayZ, length);
}


/* Specialize hash() for byte arrays. */

#ifndef HB_OPTIMIZE_SIZE_MORE
template <>
anta_hstha uint32_t hb_array_t<const var_na>::hash () const
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  คืน fasthash32(arrayZ, length, 0xf437ffe6 /* magic? */);
}

template <>
anta_hstha uint32_t hb_array_t<const unsigned var_na>::hash () const
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  คืน fasthash32(arrayZ, length, 0xf437ffe6 /* magic? */);
}
#endif


typedef hb_array_t<const var_na> hb_bytes_t;
typedef hb_array_t<const unsigned var_na> hb_ubytes_t;



#endif /* HB_ARRAY_HH */

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
struct hb_sorted_array_t;

enum hb_not_found_t
{
  HB_NOT_FOUND_DONT_STORE,
  HB_NOT_FOUND_STORE,
  HB_NOT_FOUND_STORE_CLOSEST,
};


template <typename Type>
struct hb_array_t : hb_iter_with_fallback_t<hb_array_t<Type>, Type&>
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

  constexpr hb_array_t (Type *array_, unsigned số_nguyên length_) : arrayZ (array_), length (length_) {}
  template <unsigned số_nguyên length_>
  constexpr hb_array_t (Type (&array_)[length_]) : hb_array_t (array_, length_) {}

  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_array_t (const hb_array_t<U> &o) :
    hb_iter_with_fallback_t<hb_array_t, Type&> (),
    arrayZ (o.arrayZ), length (o.length), backwards_length (o.backwards_length) {}
  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_array_t& operator = (const hb_array_t<U> &o)
  { arrayZ = o.arrayZ; length = o.length; backwards_length = o.backwards_length; trả_về *this; }

  /*
   * Iterator implementation.
   */
  typedef Type& __item_t__;
  static constexpr bool is_random_access_iterator = true;
  static constexpr bool has_fast_len = true;
  Type& __item__ () const
  {
    nếu (unlikely (!length)) trả_về CrapOrNull (Type);
    trả_về *arrayZ;
  }
  Type& __item_at__ (unsigned i) const
  {
    nếu (unlikely (i >= length)) trả_về CrapOrNull (Type);
    trả_về arrayZ[i];
  }
  void __next__ ()
  {
    nếu (unlikely (!length))
      trả_về;
    length--;
    backwards_length++;
    arrayZ++;
  }
  void __forward__ (unsigned n)
  {
    nếu (unlikely (n > length))
      n = length;
    length -= n;
    backwards_length += n;
    arrayZ += n;
  }
  void __prev__ ()
  {
    nếu (unlikely (!backwards_length))
      trả_về;
    length++;
    backwards_length--;
    arrayZ--;
  }
  void __rewind__ (unsigned n)
  {
    nếu (unlikely (n > backwards_length))
      n = backwards_length;
    length += n;
    backwards_length -= n;
    arrayZ -= n;
  }
  unsigned __len__ () const { trả_về length; }
  /* Ouch. The operator== compares the contents of the array.  For range-based for loops,
   * it's best if we can just compare arrayZ, though comparing contents is still fast,
   * but also would require that Type has operator==.  As such, we optimize this operator
   * for range-based for loop and just compare arrayZ and length.
   *
   * The above comment is outdated now because we implemented separate begin/end to
   * objects that were using hb_array_t for range-based loop before. */
  bool operator != (const hb_array_t& o) const
  { trả_về this->arrayZ != o.arrayZ || this->length != o.length; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () const { trả_về arrayZ; }
  Type *end () const { trả_về arrayZ + length; }


  /* Extra operators.
   */
  Type * operator & () const { trả_về arrayZ; }
  operator hb_array_t<const Type> () { trả_về hb_array_t<const Type> (arrayZ, length); }
  template <typename T> operator T * () const { trả_về arrayZ; }

  HB_INTERNAL bool operator == (const hb_array_t &o) const;

  uint32_t hash () const
  {
    // FNV-1a hash function
    // https://github.com/harfbuzz/harfbuzz/pull/4228
    uint32_t current = /*cbf29ce4*/0x84222325;
    cho (auto &v : *this)
    {
      current = current ^ hb_hash (v);
      current = current * 16777619;
    }
    trả_về current;
  }

  /*
   * Compare, Sort, and Search.
   */

  /* Note: our compare is NOT lexicographic; it also does NOT call Type::cmp. */
  số_nguyên cmp (const hb_array_t &a) const
  {
    nếu (length != a.length)
      trả_về (số_nguyên) a.length - (số_nguyên) length;
    trả_về hb_memcmp (a.arrayZ, arrayZ, get_size ());
  }
  HB_INTERNAL static số_nguyên cmp (const void *pa, const void *pb)
  {
    hb_array_t *a = (hb_array_t *) pa;
    hb_array_t *b = (hb_array_t *) pb;
    trả_về b->cmp (*a);
  }

  template <typename T>
  Type *lsearch (const T &x, Type *not_found = nullptr)
  {
    unsigned i;
    trả_về lfind (x, &i) ? &this->arrayZ[i] : not_found;
  }
  template <typename T>
  const Type *lsearch (const T &x, const Type *not_found = nullptr) const
  {
    unsigned i;
    trả_về lfind (x, &i) ? &this->arrayZ[i] : not_found;
  }
  template <typename T>
  bool lfind (const T &x, unsigned *pos = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned số_nguyên to_store = (unsigned số_nguyên) -1) const
  {
    cho (unsigned i = 0; i < length; ++i)
      nếu (hb_equal (x, this->arrayZ[i]))
      {
	nếu (pos)
	  *pos = i;
	trả_về true;
      }

    nếu (pos)
    {
      switch (not_found)
      {
	case HB_NOT_FOUND_DONT_STORE:
	  break;

	case HB_NOT_FOUND_STORE:
	  *pos = to_store;
	  break;

	case HB_NOT_FOUND_STORE_CLOSEST:
	  *pos = length;
	  break;
      }
    }
    trả_về false;
  }

  hb_sorted_array_t<Type> qsort (số_nguyên (*cmp)(const void*, const void*))
  {
    nếu (likely (length))
      hb_qsort (arrayZ, length, this->get_item_size (), cmp);
    trả_về hb_sorted_array_t<Type> (*this);
  }

  /* Comparator follows the C qsort convention: returns
   * negative / zero / positive int. */
  template <typename Compar>
  hb_sorted_array_t<Type> qsort (Compar compar)
  {
    nếu (likely (length))
      hb_qsort_inline (arrayZ, length, compar);
    trả_về hb_sorted_array_t<Type> (*this);
  }

  private:
  template <typename T = Type,
	    hb_enable_if (std::is_move_assignable<T>::value)>
  hb_sorted_array_t<Type> _qsort (hb_priority<1>)
  {
    trả_về qsort ([] (const Type &a, const Type &b) { trả_về Type::cmp (&a, &b); });
  }
  hb_sorted_array_t<Type> _qsort (hb_priority<0>)
  {
    trả_về qsort ((số_nguyên(*)(const void*, const void*)) Type::cmp);
  }
  public:

  hb_sorted_array_t<Type> qsort ()
  {
    trả_về _qsort (hb_prioritize);
  }

  /*
   * Other methods.
   */

  size_t get_size () const { trả_về length * this->get_item_size (); }

  /*
   * Reverse the order of items in this array in the range [start, end).
   */
  void reverse (unsigned start = 0, unsigned end = -1)
  {
    start = hb_min (start, length);
    end = hb_min (end, length);

    nếu (end < start + 2)
      trả_về;

    unsigned stop = start + (end - start) / 2;
    cho (unsigned lhs = start, rhs = end - 1; lhs < stop; lhs++, rhs--)
      hb_swap (arrayZ[rhs], arrayZ[lhs]);
  }

  hb_array_t sub_array (unsigned số_nguyên start_offset = 0, unsigned số_nguyên *seg_count = nullptr /* IN/OUT */) const
  {
    nếu (!start_offset && !seg_count)
      trả_về *this;

    unsigned số_nguyên count = length;
    nếu (unlikely (start_offset > count))
      count = 0;
    không_thì
      count -= start_offset;
    nếu (seg_count)
      count = *seg_count = hb_min (count, *seg_count);
    trả_về hb_array_t (arrayZ + start_offset, count);
  }
  hb_array_t sub_array (unsigned số_nguyên start_offset, unsigned số_nguyên seg_count) const
  { trả_về sub_array (start_offset, &seg_count); }

  hb_array_t truncate (unsigned length) const { trả_về sub_array (0, length); }

  template <typename T,
	    unsigned P = sizeof (Type),
	    hb_enable_if (P == 1)>
  const T *as () const
  { trả_về length < hb_min_size (T) ? &Null (T) : reinterpret_cast<const T *> (arrayZ); }

  template <typename T,
	    unsigned P = sizeof (Type),
	    hb_enable_if (P == 1)>
  bool check_range (const T *p, unsigned số_nguyên size = T::static_size) const
  {
    trả_về arrayZ <= ((const char *) p)
	&& ((const char *) p) <= arrayZ + length
	&& (unsigned số_nguyên) (arrayZ + length - (const char *) p) >= size;
  }

  template <unsigned P = sizeof (Type),
	    hb_enable_if (P == 1)>
  bool check_end (const void *p) const
  {
    trả_về (uintptr_t) (((const char *) p) - arrayZ) <= length;
  }

  /* Only call if you allocated the underlying array using hb_malloc() or similar. */
  void fini ()
  { hb_free ((void *) arrayZ); arrayZ = nullptr; length = 0; }

  template <typename hb_serialize_context_t,
	    typename U = Type,
	    hb_enable_if (!(sizeof (U) < sizeof (long long) && hb_is_trivially_copy_assignable(hb_decay<Type>)))>
  hb_array_t copy (hb_serialize_context_t *c) const
  {
    TRACE_SERIALIZE (this);
    auto* out = c->start_embed (arrayZ);
    nếu (unlikely (!c->extend_size (out, get_size (), false))) return_trace (hb_array_t ());
    cho (unsigned i = 0; i < length; i++)
      out[i] = arrayZ[i]; /* TODO: add version that calls c->copy() */
    return_trace (hb_array_t (out, length));
  }

  template <typename hb_serialize_context_t,
	    typename U = Type,
	    hb_enable_if (sizeof (U) < sizeof (long long) && hb_is_trivially_copy_assignable(hb_decay<Type>))>
  hb_array_t copy (hb_serialize_context_t *c) const
  {
    TRACE_SERIALIZE (this);
    auto* out = c->start_embed (arrayZ);
    nếu (unlikely (!c->extend_size (out, get_size (), false))) return_trace (hb_array_t ());
    hb_memcpy (out, arrayZ, get_size ());
    return_trace (hb_array_t (out, length));
  }

  template <typename hb_sanitize_context_t>
  bool sanitize (hb_sanitize_context_t *c) const
  { trả_về c->check_array (arrayZ, length); }

  /*
   * Members
   */

  public:
  Type *arrayZ = nullptr;
  unsigned số_nguyên length = 0;
  unsigned số_nguyên backwards_length = 0;
};
template <typename T> inline hb_array_t<T>
hb_array ()
{ trả_về hb_array_t<T> (); }
template <typename T> inline hb_array_t<T>
hb_array (T *array, unsigned số_nguyên length)
{ trả_về hb_array_t<T> (array, length); }
template <typename T, unsigned số_nguyên length_> inline hb_array_t<T>
hb_array (T (&array_)[length_])
{ trả_về hb_array_t<T> (array_); }

template <typename Type>
struct hb_sorted_array_t :
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

  constexpr hb_sorted_array_t (Type *array_, unsigned số_nguyên length_) : hb_array_t<Type> (array_, length_) {}
  template <unsigned số_nguyên length_>
  constexpr hb_sorted_array_t (Type (&array_)[length_]) : hb_array_t<Type> (array_) {}

  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_sorted_array_t (const hb_array_t<U> &o) :
    hb_array_t<Type> (o),
    hb_iter_t<hb_sorted_array_t, Type&> () {}
  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_sorted_array_t& operator = (const hb_array_t<U> &o)
  { hb_array_t<Type> (*this) = o; trả_về *this; }

  /* Iterator implementation. */

  /* See comment in hb_array_of::operator != */
  bool operator != (const hb_sorted_array_t& o) const
  { trả_về this->arrayZ != o.arrayZ || this->length != o.length; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () const { trả_về this->arrayZ; }
  Type *end () const { trả_về this->arrayZ + this->length; }


  hb_sorted_array_t sub_array (unsigned số_nguyên start_offset, unsigned số_nguyên *seg_count /* IN/OUT */) const
  { trả_về hb_sorted_array_t (((const hb_array_t<Type> *) (this))->sub_array (start_offset, seg_count)); }
  hb_sorted_array_t sub_array (unsigned số_nguyên start_offset, unsigned số_nguyên seg_count) const
  { trả_về sub_array (start_offset, &seg_count); }

  hb_sorted_array_t truncate (unsigned length) const { trả_về sub_array (0, length); }

  template <typename T>
  Type *bsearch (const T &x, Type *not_found = nullptr)
  {
    unsigned số_nguyên i;
    trả_về bfind (x, &i) ? &this->arrayZ[i] : not_found;
  }
  template <typename T>
  const Type *bsearch (const T &x, const Type *not_found = nullptr) const
  {
    unsigned số_nguyên i;
    trả_về bfind (x, &i) ? &this->arrayZ[i] : not_found;
  }
  template <typename T>
  bool bfind (const T &x, unsigned số_nguyên *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned số_nguyên to_store = (unsigned số_nguyên) -1) const
  {
    unsigned pos;

    nếu (bsearch_impl (x, &pos))
    {
      nếu (i)
	*i = pos;
      trả_về true;
    }

    nếu (i)
    {
      switch (not_found)
      {
	case HB_NOT_FOUND_DONT_STORE:
	  break;

	case HB_NOT_FOUND_STORE:
	  *i = to_store;
	  break;

	case HB_NOT_FOUND_STORE_CLOSEST:
	  *i = pos;
	  break;
      }
    }
    trả_về false;
  }
  template <typename T, typename ...Ts>
  bool bsearch_impl (const T &x, unsigned *pos, Ts... ds) const
  {
    trả_về hb_bsearch_impl (pos,
			    x,
			    this->arrayZ,
			    this->length,
			    sizeof (Type),
			    _hb_cmp_method<T, Type, Ts...>,
			    std::forward<Ts> (ds)...);
  }
};
template <typename T> inline hb_sorted_array_t<T>
hb_sorted_array (T *array, unsigned số_nguyên length)
{ trả_về hb_sorted_array_t<T> (array, length); }
template <typename T, unsigned số_nguyên length_> inline hb_sorted_array_t<T>
hb_sorted_array (T (&array_)[length_])
{ trả_về hb_sorted_array_t<T> (array_); }

template <typename T>
inline bool hb_array_t<T>::operator == (const hb_array_t<T> &o) const
{
  nếu (o.length != this->length) trả_về false;
  cho (unsigned số_nguyên i = 0; i < this->length; i++) {
    nếu (this->arrayZ[i] != o.arrayZ[i]) trả_về false;
  }
  trả_về true;
}
template <>
inline bool hb_array_t<const char>::operator == (const hb_array_t<const char> &o) const
{
  nếu (o.length != this->length) trả_về false;
  trả_về 0 == hb_memcmp (arrayZ, o.arrayZ, length);
}
template <>
inline bool hb_array_t<const unsigned char>::operator == (const hb_array_t<const unsigned char> &o) const
{
  nếu (o.length != this->length) trả_về false;
  trả_về 0 == hb_memcmp (arrayZ, o.arrayZ, length);
}


/* Specialize hash() for byte arrays. */

#ifndef HB_OPTIMIZE_SIZE_MORE
template <>
inline uint32_t hb_array_t<const char>::hash () const
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  trả_về fasthash32(arrayZ, length, 0xf437ffe6 /* magic? */);
}

template <>
inline uint32_t hb_array_t<const unsigned char>::hash () const
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  trả_về fasthash32(arrayZ, length, 0xf437ffe6 /* magic? */);
}
#endif


typedef hb_array_t<const char> hb_bytes_t;
typedef hb_array_t<const unsigned char> hb_ubytes_t;



#endif /* HB_ARRAY_HH */

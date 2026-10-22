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

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-algs.hh"
#ಅನ್ತರ್ಭಾವ "hb-iter.hh"
#ಅನ್ತರ್ಭಾವ "hb-null.hh"


template <typename Type>
ಸಂರಚನಾ hb_sorted_array_t;

ಗಣನಾ hb_not_found_t
{
  HB_NOT_FOUND_DONT_STORE,
  HB_NOT_FOUND_STORE,
  HB_NOT_FOUND_STORE_CLOSEST,
};


template <typename Type>
ಸಂರಚನಾ hb_array_t : hb_iter_with_fallback_t<hb_array_t<Type>, Type&>
{
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ realloc_move = ಸತ್ಯ;

  /*
   * Constructors.
   */
  hb_array_t () = ಸ್ವಭಾವ;
  hb_array_t (ಧ್ರುವ hb_array_t&) = ಸ್ವಭಾವ;
  ~hb_array_t () = ಸ್ವಭಾವ;
  hb_array_t& operator= (ಧ್ರುವ hb_array_t&) = ಸ್ವಭಾವ;
  hb_array_t& operator= (hb_array_t&&) = ಸ್ವಭಾವ;

  constexpr hb_array_t (Type *array_, unsigned ಪೂರ್ಣಾಙ್ಕ length_) : arrayZ (array_), length (length_) {}
  template <unsigned ಪೂರ್ಣಾಙ್ಕ length_>
  constexpr hb_array_t (Type (&array_)[length_]) : hb_array_t (array_, length_) {}

  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_array_t (ಧ್ರುವ hb_array_t<U> &o) :
    hb_iter_with_fallback_t<hb_array_t, Type&> (),
    arrayZ (o.arrayZ), length (o.length), backwards_length (o.backwards_length) {}
  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_array_t& operator = (ಧ್ರುವ hb_array_t<U> &o)
  { arrayZ = o.arrayZ; length = o.length; backwards_length = o.backwards_length; ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }

  /*
   * Iterator implementation.
   */
  ಪ್ರಕಾರನಿರ್ದೇಶ Type& __item_t__;
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ is_random_access_iterator = ಸತ್ಯ;
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ has_fast_len = ಸತ್ಯ;
  Type& __item__ () ಧ್ರುವ
  {
    ಯದಿ (unlikely (!length)) ಪ್ರತ್ಯಾಗಮ CrapOrNull (Type);
    ಪ್ರತ್ಯಾಗಮ *arrayZ;
  }
  Type& __item_at__ (unsigned i) ಧ್ರುವ
  {
    ಯದಿ (unlikely (i >= length)) ಪ್ರತ್ಯಾಗಮ CrapOrNull (Type);
    ಪ್ರತ್ಯಾಗಮ arrayZ[i];
  }
  ಶೂನ್ಯ __next__ ()
  {
    ಯದಿ (unlikely (!length))
      ಪ್ರತ್ಯಾಗಮ;
    length--;
    backwards_length++;
    arrayZ++;
  }
  ಶೂನ್ಯ __forward__ (unsigned n)
  {
    ಯದಿ (unlikely (n > length))
      n = length;
    length -= n;
    backwards_length += n;
    arrayZ += n;
  }
  ಶೂನ್ಯ __prev__ ()
  {
    ಯದಿ (unlikely (!backwards_length))
      ಪ್ರತ್ಯಾಗಮ;
    length++;
    backwards_length--;
    arrayZ--;
  }
  ಶೂನ್ಯ __rewind__ (unsigned n)
  {
    ಯದಿ (unlikely (n > backwards_length))
      n = backwards_length;
    length += n;
    backwards_length -= n;
    arrayZ -= n;
  }
  unsigned __len__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ length; }
  /* Ouch. The operator== compares the contents of the array.  For range-based for loops,
   * it's best if we can just compare arrayZ, though comparing contents is still fast,
   * but also would require that Type has operator==.  As such, we optimize this operator
   * for range-based for loop and just compare arrayZ and length.
   *
   * The above comment is outdated now because we implemented separate begin/end to
   * objects that were using hb_array_t for range-based loop before. */
  ತಾರ್ಕಿಕ operator != (ಧ್ರುವ hb_array_t& o) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ ಏತತ್->arrayZ != o.arrayZ || ಏತತ್->length != o.length; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ arrayZ; }
  Type *end () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ arrayZ + length; }


  /* Extra operators.
   */
  Type * operator & () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ arrayZ; }
  operator hb_array_t<ಧ್ರುವ Type> () { ಪ್ರತ್ಯಾಗಮ hb_array_t<ಧ್ರುವ Type> (arrayZ, length); }
  template <typename T> operator T * () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ arrayZ; }

  HB_INTERNAL ತಾರ್ಕಿಕ operator == (ಧ್ರುವ hb_array_t &o) ಧ್ರುವ;

  uint32_t hash () ಧ್ರುವ
  {
    // FNV-1a hash function
    // https://github.com/harfbuzz/harfbuzz/pull/4228
    uint32_t current = /*cbf29ce4*/0x84222325;
    ಪ್ರತಿ (auto &v : *ಏತತ್)
    {
      current = current ^ hb_hash (v);
      current = current * 16777619;
    }
    ಪ್ರತ್ಯಾಗಮ current;
  }

  /*
   * Compare, Sort, and Search.
   */

  /* Note: our compare is NOT lexicographic; it also does NOT call Type::cmp. */
  ಪೂರ್ಣಾಙ್ಕ cmp (ಧ್ರುವ hb_array_t &a) ಧ್ರುವ
  {
    ಯದಿ (length != a.length)
      ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ) a.length - (ಪೂರ್ಣಾಙ್ಕ) length;
    ಪ್ರತ್ಯಾಗಮ hb_memcmp (a.arrayZ, arrayZ, get_size ());
  }
  HB_INTERNAL ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmp (ಧ್ರುವ ಶೂನ್ಯ *pa, ಧ್ರುವ ಶೂನ್ಯ *pb)
  {
    hb_array_t *a = (hb_array_t *) pa;
    hb_array_t *b = (hb_array_t *) pb;
    ಪ್ರತ್ಯಾಗಮ b->cmp (*a);
  }

  template <typename T>
  Type *lsearch (ಧ್ರುವ T &x, Type *not_found = nullptr)
  {
    unsigned i;
    ಪ್ರತ್ಯಾಗಮ lfind (x, &i) ? &ಏತತ್->arrayZ[i] : not_found;
  }
  template <typename T>
  ಧ್ರುವ Type *lsearch (ಧ್ರುವ T &x, ಧ್ರುವ Type *not_found = nullptr) ಧ್ರುವ
  {
    unsigned i;
    ಪ್ರತ್ಯಾಗಮ lfind (x, &i) ? &ಏತತ್->arrayZ[i] : not_found;
  }
  template <typename T>
  ತಾರ್ಕಿಕ lfind (ಧ್ರುವ T &x, unsigned *pos = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned ಪೂರ್ಣಾಙ್ಕ to_store = (unsigned ಪೂರ್ಣಾಙ್ಕ) -1) ಧ್ರುವ
  {
    ಪ್ರತಿ (unsigned i = 0; i < length; ++i)
      ಯದಿ (hb_equal (x, ಏತತ್->arrayZ[i]))
      {
	ಯದಿ (pos)
	  *pos = i;
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      }

    ಯದಿ (pos)
    {
      ವಿಕಲ್ಪ (not_found)
      {
	ಪಕ್ಷ HB_NOT_FOUND_DONT_STORE:
	  ಭಙ್ಗ;

	ಪಕ್ಷ HB_NOT_FOUND_STORE:
	  *pos = to_store;
	  ಭಙ್ಗ;

	ಪಕ್ಷ HB_NOT_FOUND_STORE_CLOSEST:
	  *pos = length;
	  ಭಙ್ಗ;
      }
    }
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  hb_sorted_array_t<Type> qsort (ಪೂರ್ಣಾಙ್ಕ (*cmp)(ಧ್ರುವ ಶೂನ್ಯ*, ಧ್ರುವ ಶೂನ್ಯ*))
  {
    ಯದಿ (likely (length))
      hb_qsort (arrayZ, length, ಏತತ್->get_item_size (), cmp);
    ಪ್ರತ್ಯಾಗಮ hb_sorted_array_t<Type> (*ಏತತ್);
  }

  /* Comparator follows the C qsort convention: returns
   * negative / zero / positive int. */
  template <typename Compar>
  hb_sorted_array_t<Type> qsort (Compar compar)
  {
    ಯದಿ (likely (length))
      hb_qsort_inline (arrayZ, length, compar);
    ಪ್ರತ್ಯಾಗಮ hb_sorted_array_t<Type> (*ಏತತ್);
  }

  ಗುಪ್ತ:
  template <typename T = Type,
	    hb_enable_if (std::is_move_assignable<T>::value)>
  hb_sorted_array_t<Type> _qsort (hb_priority<1>)
  {
    ಪ್ರತ್ಯಾಗಮ qsort ([] (ಧ್ರುವ Type &a, ಧ್ರುವ Type &b) { ಪ್ರತ್ಯಾಗಮ Type::cmp (&a, &b); });
  }
  hb_sorted_array_t<Type> _qsort (hb_priority<0>)
  {
    ಪ್ರತ್ಯಾಗಮ qsort ((ಪೂರ್ಣಾಙ್ಕ(*)(ಧ್ರುವ ಶೂನ್ಯ*, ಧ್ರುವ ಶೂನ್ಯ*)) Type::cmp);
  }
  ಸಾರ್ವಜನಿಕ:

  hb_sorted_array_t<Type> qsort ()
  {
    ಪ್ರತ್ಯಾಗಮ _qsort (hb_prioritize);
  }

  /*
   * Other methods.
   */

  size_t get_size () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ length * ಏತತ್->get_item_size (); }

  /*
   * Reverse the order of items in this array in the range [start, end).
   */
  ಶೂನ್ಯ reverse (unsigned start = 0, unsigned end = -1)
  {
    start = hb_min (start, length);
    end = hb_min (end, length);

    ಯದಿ (end < start + 2)
      ಪ್ರತ್ಯಾಗಮ;

    unsigned stop = start + (end - start) / 2;
    ಪ್ರತಿ (unsigned lhs = start, rhs = end - 1; lhs < stop; lhs++, rhs--)
      hb_swap (arrayZ[rhs], arrayZ[lhs]);
  }

  hb_array_t sub_array (unsigned ಪೂರ್ಣಾಙ್ಕ start_offset = 0, unsigned ಪೂರ್ಣಾಙ್ಕ *seg_ಎಣಿಕೆ = nullptr /* IN/OUT */) ಧ್ರುವ
  {
    ಯದಿ (!start_offset && !seg_ಎಣಿಕೆ)
      ಪ್ರತ್ಯಾಗಮ *ಏತತ್;

    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = length;
    ಯದಿ (unlikely (start_offset > ಎಣಿಕೆ))
      ಎಣಿಕೆ = 0;
    ಅನ್ಯಥಾ
      ಎಣಿಕೆ -= start_offset;
    ಯದಿ (seg_ಎಣಿಕೆ)
      ಎಣಿಕೆ = *seg_ಎಣಿಕೆ = hb_min (ಎಣಿಕೆ, *seg_ಎಣಿಕೆ);
    ಪ್ರತ್ಯಾಗಮ hb_array_t (arrayZ + start_offset, ಎಣಿಕೆ);
  }
  hb_array_t sub_array (unsigned ಪೂರ್ಣಾಙ್ಕ start_offset, unsigned ಪೂರ್ಣಾಙ್ಕ seg_ಎಣಿಕೆ) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ sub_array (start_offset, &seg_ಎಣಿಕೆ); }

  hb_array_t truncate (unsigned length) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ sub_array (0, length); }

  template <typename T,
	    unsigned P = ಪರಿಮಾಣ (Type),
	    hb_enable_if (P == 1)>
  ಧ್ರುವ T *as () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ length < hb_min_size (T) ? &Null (T) : reinterpret_cast<ಧ್ರುವ T *> (arrayZ); }

  template <typename T,
	    unsigned P = ಪರಿಮಾಣ (Type),
	    hb_enable_if (P == 1)>
  ತಾರ್ಕಿಕ check_range (ಧ್ರುವ T *p, unsigned ಪೂರ್ಣಾಙ್ಕ size = T::static_size) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ arrayZ <= ((ಧ್ರುವ ವರ್ಣ *) p)
	&& ((ಧ್ರುವ ವರ್ಣ *) p) <= arrayZ + length
	&& (unsigned ಪೂರ್ಣಾಙ್ಕ) (arrayZ + length - (ಧ್ರುವ ವರ್ಣ *) p) >= size;
  }

  template <unsigned P = ಪರಿಮಾಣ (Type),
	    hb_enable_if (P == 1)>
  ತಾರ್ಕಿಕ check_end (ಧ್ರುವ ಶೂನ್ಯ *p) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ (uintptr_t) (((ಧ್ರುವ ವರ್ಣ *) p) - arrayZ) <= length;
  }

  /* Only call if you allocated the underlying array using hb_malloc() or similar. */
  ಶೂನ್ಯ fini ()
  { hb_free ((ಶೂನ್ಯ *) arrayZ); arrayZ = nullptr; length = 0; }

  template <typename hb_serialize_context_t,
	    typename U = Type,
	    hb_enable_if (!(ಪರಿಮಾಣ (U) < ಪರಿಮಾಣ (long long) && hb_is_trivially_copy_assignable(hb_decay<Type>)))>
  hb_array_t copy (hb_serialize_context_t *c) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    auto* out = c->start_embed (arrayZ);
    ಯದಿ (unlikely (!c->extend_size (out, get_size (), ಅಸತ್ಯ))) return_trace (hb_array_t ());
    ಪ್ರತಿ (unsigned i = 0; i < length; i++)
      out[i] = arrayZ[i]; /* TODO: add version that calls c->copy() */
    return_trace (hb_array_t (out, length));
  }

  template <typename hb_serialize_context_t,
	    typename U = Type,
	    hb_enable_if (ಪರಿಮಾಣ (U) < ಪರಿಮಾಣ (long long) && hb_is_trivially_copy_assignable(hb_decay<Type>))>
  hb_array_t copy (hb_serialize_context_t *c) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    auto* out = c->start_embed (arrayZ);
    ಯದಿ (unlikely (!c->extend_size (out, get_size (), ಅಸತ್ಯ))) return_trace (hb_array_t ());
    hb_memcpy (out, arrayZ, get_size ());
    return_trace (hb_array_t (out, length));
  }

  template <typename hb_sanitize_context_t>
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ c->check_array (arrayZ, length); }

  /*
   * Members
   */

  ಸಾರ್ವಜನಿಕ:
  Type *arrayZ = nullptr;
  unsigned ಪೂರ್ಣಾಙ್ಕ length = 0;
  unsigned ಪೂರ್ಣಾಙ್ಕ backwards_length = 0;
};
template <typename T> ಅನ್ತಃಸ್ಥ hb_array_t<T>
hb_array ()
{ ಪ್ರತ್ಯಾಗಮ hb_array_t<T> (); }
template <typename T> ಅನ್ತಃಸ್ಥ hb_array_t<T>
hb_array (T *array, unsigned ಪೂರ್ಣಾಙ್ಕ length)
{ ಪ್ರತ್ಯಾಗಮ hb_array_t<T> (array, length); }
template <typename T, unsigned ಪೂರ್ಣಾಙ್ಕ length_> ಅನ್ತಃಸ್ಥ hb_array_t<T>
hb_array (T (&array_)[length_])
{ ಪ್ರತ್ಯಾಗಮ hb_array_t<T> (array_); }

template <typename Type>
ಸಂರಚನಾ hb_sorted_array_t :
	hb_array_t<Type>,
	hb_iter_t<hb_sorted_array_t<Type>, Type&>
{
  ಪ್ರಕಾರನಿರ್ದೇಶ hb_iter_t<hb_sorted_array_t, Type&> iter_base_t;
  HB_ITER_USING (iter_base_t);
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ is_random_access_iterator = ಸತ್ಯ;
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ is_sorted_iterator = ಸತ್ಯ;
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ has_fast_len = ಸತ್ಯ;

  hb_sorted_array_t () = ಸ್ವಭಾವ;
  hb_sorted_array_t (ಧ್ರುವ hb_sorted_array_t&) = ಸ್ವಭಾವ;
  ~hb_sorted_array_t () = ಸ್ವಭಾವ;
  hb_sorted_array_t& operator= (ಧ್ರುವ hb_sorted_array_t&) = ಸ್ವಭಾವ;
  hb_sorted_array_t& operator= (hb_sorted_array_t&&) = ಸ್ವಭಾವ;

  constexpr hb_sorted_array_t (Type *array_, unsigned ಪೂರ್ಣಾಙ್ಕ length_) : hb_array_t<Type> (array_, length_) {}
  template <unsigned ಪೂರ್ಣಾಙ್ಕ length_>
  constexpr hb_sorted_array_t (Type (&array_)[length_]) : hb_array_t<Type> (array_) {}

  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_sorted_array_t (ಧ್ರುವ hb_array_t<U> &o) :
    hb_array_t<Type> (o),
    hb_iter_t<hb_sorted_array_t, Type&> () {}
  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_sorted_array_t& operator = (ಧ್ರುವ hb_array_t<U> &o)
  { hb_array_t<Type> (*ಏತತ್) = o; ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }

  /* Iterator implementation. */

  /* See comment in hb_array_of::operator != */
  ತಾರ್ಕಿಕ operator != (ಧ್ರುವ hb_sorted_array_t& o) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ ಏತತ್->arrayZ != o.arrayZ || ಏತತ್->length != o.length; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಏತತ್->arrayZ; }
  Type *end () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಏತತ್->arrayZ + ಏತತ್->length; }


  hb_sorted_array_t sub_array (unsigned ಪೂರ್ಣಾಙ್ಕ start_offset, unsigned ಪೂರ್ಣಾಙ್ಕ *seg_ಎಣಿಕೆ /* IN/OUT */) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_sorted_array_t (((ಧ್ರುವ hb_array_t<Type> *) (ಏತತ್))->sub_array (start_offset, seg_ಎಣಿಕೆ)); }
  hb_sorted_array_t sub_array (unsigned ಪೂರ್ಣಾಙ್ಕ start_offset, unsigned ಪೂರ್ಣಾಙ್ಕ seg_ಎಣಿಕೆ) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ sub_array (start_offset, &seg_ಎಣಿಕೆ); }

  hb_sorted_array_t truncate (unsigned length) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ sub_array (0, length); }

  template <typename T>
  Type *bsearch (ಧ್ರುವ T &x, Type *not_found = nullptr)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತ್ಯಾಗಮ bfind (x, &i) ? &ಏತತ್->arrayZ[i] : not_found;
  }
  template <typename T>
  ಧ್ರುವ Type *bsearch (ಧ್ರುವ T &x, ಧ್ರುವ Type *not_found = nullptr) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತ್ಯಾಗಮ bfind (x, &i) ? &ಏತತ್->arrayZ[i] : not_found;
  }
  template <typename T>
  ತಾರ್ಕಿಕ bfind (ಧ್ರುವ T &x, unsigned ಪೂರ್ಣಾಙ್ಕ *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned ಪೂರ್ಣಾಙ್ಕ to_store = (unsigned ಪೂರ್ಣಾಙ್ಕ) -1) ಧ್ರುವ
  {
    unsigned pos;

    ಯದಿ (bsearch_impl (x, &pos))
    {
      ಯದಿ (i)
	*i = pos;
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    ಯದಿ (i)
    {
      ವಿಕಲ್ಪ (not_found)
      {
	ಪಕ್ಷ HB_NOT_FOUND_DONT_STORE:
	  ಭಙ್ಗ;

	ಪಕ್ಷ HB_NOT_FOUND_STORE:
	  *i = to_store;
	  ಭಙ್ಗ;

	ಪಕ್ಷ HB_NOT_FOUND_STORE_CLOSEST:
	  *i = pos;
	  ಭಙ್ಗ;
      }
    }
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }
  template <typename T, typename ...Ts>
  ತಾರ್ಕಿಕ bsearch_impl (ಧ್ರುವ T &x, unsigned *pos, Ts... ds) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ hb_bsearch_impl (pos,
			    x,
			    ಏತತ್->arrayZ,
			    ಏತತ್->length,
			    ಪರಿಮಾಣ (Type),
			    _hb_cmp_method<T, Type, Ts...>,
			    std::forward<Ts> (ds)...);
  }
};
template <typename T> ಅನ್ತಃಸ್ಥ hb_sorted_array_t<T>
hb_sorted_array (T *array, unsigned ಪೂರ್ಣಾಙ್ಕ length)
{ ಪ್ರತ್ಯಾಗಮ hb_sorted_array_t<T> (array, length); }
template <typename T, unsigned ಪೂರ್ಣಾಙ್ಕ length_> ಅನ್ತಃಸ್ಥ hb_sorted_array_t<T>
hb_sorted_array (T (&array_)[length_])
{ ಪ್ರತ್ಯಾಗಮ hb_sorted_array_t<T> (array_); }

template <typename T>
ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ hb_array_t<T>::operator == (ಧ್ರುವ hb_array_t<T> &o) ಧ್ರುವ
{
  ಯದಿ (o.length != ಏತತ್->length) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಏತತ್->length; i++) {
    ಯದಿ (ಏತತ್->arrayZ[i] != o.arrayZ[i]) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}
template <>
ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ hb_array_t<ಧ್ರುವ ವರ್ಣ>::operator == (ಧ್ರುವ hb_array_t<ಧ್ರುವ ವರ್ಣ> &o) ಧ್ರುವ
{
  ಯದಿ (o.length != ಏತತ್->length) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  ಪ್ರತ್ಯಾಗಮ 0 == hb_memcmp (arrayZ, o.arrayZ, length);
}
template <>
ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ hb_array_t<ಧ್ರುವ unsigned ವರ್ಣ>::operator == (ಧ್ರುವ hb_array_t<ಧ್ರುವ unsigned ವರ್ಣ> &o) ಧ್ರುವ
{
  ಯದಿ (o.length != ಏತತ್->length) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  ಪ್ರತ್ಯಾಗಮ 0 == hb_memcmp (arrayZ, o.arrayZ, length);
}


/* Specialize hash() for byte arrays. */

#ifndef HB_OPTIMIZE_SIZE_MORE
template <>
ಅನ್ತಃಸ್ಥ uint32_t hb_array_t<ಧ್ರುವ ವರ್ಣ>::hash () ಧ್ರುವ
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  ಪ್ರತ್ಯಾಗಮ fasthash32(arrayZ, length, 0xf437ffe6 /* magic? */);
}

template <>
ಅನ್ತಃಸ್ಥ uint32_t hb_array_t<ಧ್ರುವ unsigned ವರ್ಣ>::hash () ಧ್ರುವ
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  ಪ್ರತ್ಯಾಗಮ fasthash32(arrayZ, length, 0xf437ffe6 /* magic? */);
}
#endif


ಪ್ರಕಾರನಿರ್ದೇಶ hb_array_t<ಧ್ರುವ ವರ್ಣ> hb_bytes_t;
ಪ್ರಕಾರನಿರ್ದೇಶ hb_array_t<ಧ್ರುವ unsigned ವರ್ಣ> hb_ubytes_t;



#endif /* HB_ARRAY_HH */

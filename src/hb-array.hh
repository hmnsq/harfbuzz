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
কাঠামো hb_sorted_array_t;

গণনা hb_not_found_t
{
  HB_NOT_FOUND_DONT_STORE,
  HB_NOT_FOUND_STORE,
  HB_NOT_FOUND_STORE_CLOSEST,
};


template <typename Type>
কাঠামো hb_array_t : hb_iter_with_fallback_t<hb_array_t<Type>, Type&>
{
  স্থির constexpr যৌক্তিক realloc_move = সত্য;

  /*
   * Constructors.
   */
  hb_array_t () = পূর্বনির্ধারিত;
  hb_array_t (ধ্রুবক hb_array_t&) = পূর্বনির্ধারিত;
  ~hb_array_t () = পূর্বনির্ধারিত;
  hb_array_t& operator= (ধ্রুবক hb_array_t&) = পূর্বনির্ধারিত;
  hb_array_t& operator= (hb_array_t&&) = পূর্বনির্ধারিত;

  constexpr hb_array_t (Type *array_, unsigned পূর্ণসংখ্যা দৈর্ঘ্য_2) : arrayZ (array_), দৈর্ঘ্য (দৈর্ঘ্য_2) {}
  template <unsigned পূর্ণসংখ্যা দৈর্ঘ্য_2>
  constexpr hb_array_t (Type (&array_)[দৈর্ঘ্য_2]) : hb_array_t (array_, দৈর্ঘ্য_2) {}

  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_array_t (ধ্রুবক hb_array_t<U> &o) :
    hb_iter_with_fallback_t<hb_array_t, Type&> (),
    arrayZ (o.arrayZ), দৈর্ঘ্য (o.দৈর্ঘ্য), backwards_দৈর্ঘ্য (o.backwards_দৈর্ঘ্য) {}
  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_array_t& operator = (ধ্রুবক hb_array_t<U> &o)
  { arrayZ = o.arrayZ; দৈর্ঘ্য = o.দৈর্ঘ্য; backwards_দৈর্ঘ্য = o.backwards_দৈর্ঘ্য; ফেরত *এই; }

  /*
   * Iterator implementation.
   */
  প্রকারলেখ Type& __item_t__;
  স্থির constexpr যৌক্তিক is_random_access_iterator = সত্য;
  স্থির constexpr যৌক্তিক has_fast_len = সত্য;
  Type& __item__ () ধ্রুবক
  {
    যদি (unlikely (!দৈর্ঘ্য)) ফেরত CrapOrNull (Type);
    ফেরত *arrayZ;
  }
  Type& __item_at__ (unsigned i) ধ্রুবক
  {
    যদি (unlikely (i >= দৈর্ঘ্য)) ফেরত CrapOrNull (Type);
    ফেরত arrayZ[i];
  }
  শূন্য __next__ ()
  {
    যদি (unlikely (!দৈর্ঘ্য))
      ফেরত;
    দৈর্ঘ্য--;
    backwards_দৈর্ঘ্য++;
    arrayZ++;
  }
  শূন্য __forward__ (unsigned n)
  {
    যদি (unlikely (n > দৈর্ঘ্য))
      n = দৈর্ঘ্য;
    দৈর্ঘ্য -= n;
    backwards_দৈর্ঘ্য += n;
    arrayZ += n;
  }
  শূন্য __prev__ ()
  {
    যদি (unlikely (!backwards_দৈর্ঘ্য))
      ফেরত;
    দৈর্ঘ্য++;
    backwards_দৈর্ঘ্য--;
    arrayZ--;
  }
  শূন্য __rewind__ (unsigned n)
  {
    যদি (unlikely (n > backwards_দৈর্ঘ্য))
      n = backwards_দৈর্ঘ্য;
    দৈর্ঘ্য += n;
    backwards_দৈর্ঘ্য -= n;
    arrayZ -= n;
  }
  unsigned __len__ () ধ্রুবক { ফেরত দৈর্ঘ্য; }
  /* Ouch. The operator== compares the contents of the array.  For range-based for loops,
   * it's best if we can just compare arrayZ, though comparing contents is still fast,
   * but also would require that Type has operator==.  As such, we optimize this operator
   * for range-based for loop and just compare arrayZ and length.
   *
   * The above comment is outdated now because we implemented separate begin/end to
   * objects that were using hb_array_t for range-based loop before. */
  যৌক্তিক operator != (ধ্রুবক hb_array_t& o) ধ্রুবক
  { ফেরত এই->arrayZ != o.arrayZ || এই->দৈর্ঘ্য != o.দৈর্ঘ্য; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () ধ্রুবক { ফেরত arrayZ; }
  Type *end () ধ্রুবক { ফেরত arrayZ + দৈর্ঘ্য; }


  /* Extra operators.
   */
  Type * operator & () ধ্রুবক { ফেরত arrayZ; }
  operator hb_array_t<ধ্রুবক Type> () { ফেরত hb_array_t<ধ্রুবক Type> (arrayZ, দৈর্ঘ্য); }
  template <typename T> operator T * () ধ্রুবক { ফেরত arrayZ; }

  HB_INTERNAL যৌক্তিক operator == (ধ্রুবক hb_array_t &o) ধ্রুবক;

  uint32_t hash () ধ্রুবক
  {
    // FNV-1a hash function
    // https://github.com/harfbuzz/harfbuzz/pull/4228
    uint32_t current = /*cbf29ce4*/0x84222325;
    জন্য (auto &v : *এই)
    {
      current = current ^ hb_hash (v);
      current = current * 16777619;
    }
    ফেরত current;
  }

  /*
   * Compare, Sort, and Search.
   */

  /* Note: our compare is NOT lexicographic; it also does NOT call Type::cmp. */
  পূর্ণসংখ্যা cmp (ধ্রুবক hb_array_t &a) ধ্রুবক
  {
    যদি (দৈর্ঘ্য != a.দৈর্ঘ্য)
      ফেরত (পূর্ণসংখ্যা) a.দৈর্ঘ্য - (পূর্ণসংখ্যা) দৈর্ঘ্য;
    ফেরত hb_memcmp (a.arrayZ, arrayZ, get_আকার ());
  }
  HB_INTERNAL স্থির পূর্ণসংখ্যা cmp (ধ্রুবক শূন্য *pa, ধ্রুবক শূন্য *pb)
  {
    hb_array_t *a = (hb_array_t *) pa;
    hb_array_t *b = (hb_array_t *) pb;
    ফেরত b->cmp (*a);
  }

  template <typename T>
  Type *lsearch (ধ্রুবক T &x, Type *not_found = nullptr)
  {
    unsigned i;
    ফেরত lfind (x, &i) ? &এই->arrayZ[i] : not_found;
  }
  template <typename T>
  ধ্রুবক Type *lsearch (ধ্রুবক T &x, ধ্রুবক Type *not_found = nullptr) ধ্রুবক
  {
    unsigned i;
    ফেরত lfind (x, &i) ? &এই->arrayZ[i] : not_found;
  }
  template <typename T>
  যৌক্তিক lfind (ধ্রুবক T &x, unsigned *pos = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned পূর্ণসংখ্যা to_store = (unsigned পূর্ণসংখ্যা) -1) ধ্রুবক
  {
    জন্য (unsigned i = 0; i < দৈর্ঘ্য; ++i)
      যদি (hb_equal (x, এই->arrayZ[i]))
      {
	যদি (pos)
	  *pos = i;
	ফেরত সত্য;
      }

    যদি (pos)
    {
      নির্বাচন (not_found)
      {
	ক্ষেত্র HB_NOT_FOUND_DONT_STORE:
	  বিরতি;

	ক্ষেত্র HB_NOT_FOUND_STORE:
	  *pos = to_store;
	  বিরতি;

	ক্ষেত্র HB_NOT_FOUND_STORE_CLOSEST:
	  *pos = দৈর্ঘ্য;
	  বিরতি;
      }
    }
    ফেরত মিথ্যা;
  }

  hb_sorted_array_t<Type> qsort (পূর্ণসংখ্যা (*cmp)(ধ্রুবক শূন্য*, ধ্রুবক শূন্য*))
  {
    যদি (likely (দৈর্ঘ্য))
      hb_qsort (arrayZ, দৈর্ঘ্য, এই->get_item_আকার (), cmp);
    ফেরত hb_sorted_array_t<Type> (*এই);
  }

  /* Comparator follows the C qsort convention: returns
   * negative / zero / positive int. */
  template <typename Compar>
  hb_sorted_array_t<Type> qsort (Compar compar)
  {
    যদি (likely (দৈর্ঘ্য))
      hb_qsort_inline (arrayZ, দৈর্ঘ্য, compar);
    ফেরত hb_sorted_array_t<Type> (*এই);
  }

  ব্যক্তিগত:
  template <typename T = Type,
	    hb_enable_if (std::is_move_assignable<T>::value)>
  hb_sorted_array_t<Type> _qsort (hb_priority<1>)
  {
    ফেরত qsort ([] (ধ্রুবক Type &a, ধ্রুবক Type &b) { ফেরত Type::cmp (&a, &b); });
  }
  hb_sorted_array_t<Type> _qsort (hb_priority<0>)
  {
    ফেরত qsort ((পূর্ণসংখ্যা(*)(ধ্রুবক শূন্য*, ধ্রুবক শূন্য*)) Type::cmp);
  }
  সর্বজনীন:

  hb_sorted_array_t<Type> qsort ()
  {
    ফেরত _qsort (hb_prioritize);
  }

  /*
   * Other methods.
   */

  size_t get_আকার () ধ্রুবক { ফেরত দৈর্ঘ্য * এই->get_item_আকার (); }

  /*
   * Reverse the order of items in this array in the range [start, end).
   */
  শূন্য reverse (unsigned start = 0, unsigned end = -1)
  {
    start = hb_min (start, দৈর্ঘ্য);
    end = hb_min (end, দৈর্ঘ্য);

    যদি (end < start + 2)
      ফেরত;

    unsigned stop = start + (end - start) / 2;
    জন্য (unsigned lhs = start, rhs = end - 1; lhs < stop; lhs++, rhs--)
      hb_swap (arrayZ[rhs], arrayZ[lhs]);
  }

  hb_array_t sub_array (unsigned পূর্ণসংখ্যা start_offset = 0, unsigned পূর্ণসংখ্যা *seg_গুনতি = nullptr /* IN/OUT */) ধ্রুবক
  {
    যদি (!start_offset && !seg_গুনতি)
      ফেরত *এই;

    unsigned পূর্ণসংখ্যা গুনতি = দৈর্ঘ্য;
    যদি (unlikely (start_offset > গুনতি))
      গুনতি = 0;
    অন্যথা
      গুনতি -= start_offset;
    যদি (seg_গুনতি)
      গুনতি = *seg_গুনতি = hb_min (গুনতি, *seg_গুনতি);
    ফেরত hb_array_t (arrayZ + start_offset, গুনতি);
  }
  hb_array_t sub_array (unsigned পূর্ণসংখ্যা start_offset, unsigned পূর্ণসংখ্যা seg_গুনতি) ধ্রুবক
  { ফেরত sub_array (start_offset, &seg_গুনতি); }

  hb_array_t truncate (unsigned দৈর্ঘ্য) ধ্রুবক { ফেরত sub_array (0, দৈর্ঘ্য); }

  template <typename T,
	    unsigned P = আকার (Type),
	    hb_enable_if (P == 1)>
  ধ্রুবক T *as () ধ্রুবক
  { ফেরত দৈর্ঘ্য < hb_min_আকার (T) ? &Null (T) : reinterpret_cast<ধ্রুবক T *> (arrayZ); }

  template <typename T,
	    unsigned P = আকার (Type),
	    hb_enable_if (P == 1)>
  যৌক্তিক check_range (ধ্রুবক T *p, unsigned পূর্ণসংখ্যা আকার_2 = T::static_আকার) ধ্রুবক
  {
    ফেরত arrayZ <= ((ধ্রুবক অক্ষর *) p)
	&& ((ধ্রুবক অক্ষর *) p) <= arrayZ + দৈর্ঘ্য
	&& (unsigned পূর্ণসংখ্যা) (arrayZ + দৈর্ঘ্য - (ধ্রুবক অক্ষর *) p) >= আকার_2;
  }

  template <unsigned P = আকার (Type),
	    hb_enable_if (P == 1)>
  যৌক্তিক check_end (ধ্রুবক শূন্য *p) ধ্রুবক
  {
    ফেরত (uintptr_t) (((ধ্রুবক অক্ষর *) p) - arrayZ) <= দৈর্ঘ্য;
  }

  /* Only call if you allocated the underlying array using hb_malloc() or similar. */
  শূন্য fini ()
  { hb_free ((শূন্য *) arrayZ); arrayZ = nullptr; দৈর্ঘ্য = 0; }

  template <typename hb_serialize_context_t,
	    typename U = Type,
	    hb_enable_if (!(আকার (U) < আকার (long long) && hb_is_trivially_copy_assignable(hb_decay<Type>)))>
  hb_array_t copy (hb_serialize_context_t *c) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    auto* out = c->start_embed (arrayZ);
    যদি (unlikely (!c->extend_আকার (out, get_আকার (), মিথ্যা))) return_trace (hb_array_t ());
    জন্য (unsigned i = 0; i < দৈর্ঘ্য; i++)
      out[i] = arrayZ[i]; /* TODO: add version that calls c->copy() */
    return_trace (hb_array_t (out, দৈর্ঘ্য));
  }

  template <typename hb_serialize_context_t,
	    typename U = Type,
	    hb_enable_if (আকার (U) < আকার (long long) && hb_is_trivially_copy_assignable(hb_decay<Type>))>
  hb_array_t copy (hb_serialize_context_t *c) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    auto* out = c->start_embed (arrayZ);
    যদি (unlikely (!c->extend_আকার (out, get_আকার (), মিথ্যা))) return_trace (hb_array_t ());
    hb_memcpy (out, arrayZ, get_আকার ());
    return_trace (hb_array_t (out, দৈর্ঘ্য));
  }

  template <typename hb_sanitize_context_t>
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  { ফেরত c->check_array (arrayZ, দৈর্ঘ্য); }

  /*
   * Members
   */

  সর্বজনীন:
  Type *arrayZ = nullptr;
  unsigned পূর্ণসংখ্যা দৈর্ঘ্য = 0;
  unsigned পূর্ণসংখ্যা backwards_দৈর্ঘ্য = 0;
};
template <typename T> অন্তঃরেখ hb_array_t<T>
hb_array ()
{ ফেরত hb_array_t<T> (); }
template <typename T> অন্তঃরেখ hb_array_t<T>
hb_array (T *array, unsigned পূর্ণসংখ্যা দৈর্ঘ্য)
{ ফেরত hb_array_t<T> (array, দৈর্ঘ্য); }
template <typename T, unsigned পূর্ণসংখ্যা দৈর্ঘ্য_2> অন্তঃরেখ hb_array_t<T>
hb_array (T (&array_)[দৈর্ঘ্য_2])
{ ফেরত hb_array_t<T> (array_); }

template <typename Type>
কাঠামো hb_sorted_array_t :
	hb_array_t<Type>,
	hb_iter_t<hb_sorted_array_t<Type>, Type&>
{
  প্রকারলেখ hb_iter_t<hb_sorted_array_t, Type&> iter_base_t;
  HB_ITER_USING (iter_base_t);
  স্থির constexpr যৌক্তিক is_random_access_iterator = সত্য;
  স্থির constexpr যৌক্তিক is_sorted_iterator = সত্য;
  স্থির constexpr যৌক্তিক has_fast_len = সত্য;

  hb_sorted_array_t () = পূর্বনির্ধারিত;
  hb_sorted_array_t (ধ্রুবক hb_sorted_array_t&) = পূর্বনির্ধারিত;
  ~hb_sorted_array_t () = পূর্বনির্ধারিত;
  hb_sorted_array_t& operator= (ধ্রুবক hb_sorted_array_t&) = পূর্বনির্ধারিত;
  hb_sorted_array_t& operator= (hb_sorted_array_t&&) = পূর্বনির্ধারিত;

  constexpr hb_sorted_array_t (Type *array_, unsigned পূর্ণসংখ্যা দৈর্ঘ্য_2) : hb_array_t<Type> (array_, দৈর্ঘ্য_2) {}
  template <unsigned পূর্ণসংখ্যা দৈর্ঘ্য_2>
  constexpr hb_sorted_array_t (Type (&array_)[দৈর্ঘ্য_2]) : hb_array_t<Type> (array_) {}

  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_sorted_array_t (ধ্রুবক hb_array_t<U> &o) :
    hb_array_t<Type> (o),
    hb_iter_t<hb_sorted_array_t, Type&> () {}
  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_sorted_array_t& operator = (ধ্রুবক hb_array_t<U> &o)
  { hb_array_t<Type> (*এই) = o; ফেরত *এই; }

  /* Iterator implementation. */

  /* See comment in hb_array_of::operator != */
  যৌক্তিক operator != (ধ্রুবক hb_sorted_array_t& o) ধ্রুবক
  { ফেরত এই->arrayZ != o.arrayZ || এই->দৈর্ঘ্য != o.দৈর্ঘ্য; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () ধ্রুবক { ফেরত এই->arrayZ; }
  Type *end () ধ্রুবক { ফেরত এই->arrayZ + এই->দৈর্ঘ্য; }


  hb_sorted_array_t sub_array (unsigned পূর্ণসংখ্যা start_offset, unsigned পূর্ণসংখ্যা *seg_গুনতি /* IN/OUT */) ধ্রুবক
  { ফেরত hb_sorted_array_t (((ধ্রুবক hb_array_t<Type> *) (এই))->sub_array (start_offset, seg_গুনতি)); }
  hb_sorted_array_t sub_array (unsigned পূর্ণসংখ্যা start_offset, unsigned পূর্ণসংখ্যা seg_গুনতি) ধ্রুবক
  { ফেরত sub_array (start_offset, &seg_গুনতি); }

  hb_sorted_array_t truncate (unsigned দৈর্ঘ্য) ধ্রুবক { ফেরত sub_array (0, দৈর্ঘ্য); }

  template <typename T>
  Type *bsearch (ধ্রুবক T &x, Type *not_found = nullptr)
  {
    unsigned পূর্ণসংখ্যা i;
    ফেরত bfind (x, &i) ? &এই->arrayZ[i] : not_found;
  }
  template <typename T>
  ধ্রুবক Type *bsearch (ধ্রুবক T &x, ধ্রুবক Type *not_found = nullptr) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা i;
    ফেরত bfind (x, &i) ? &এই->arrayZ[i] : not_found;
  }
  template <typename T>
  যৌক্তিক bfind (ধ্রুবক T &x, unsigned পূর্ণসংখ্যা *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned পূর্ণসংখ্যা to_store = (unsigned পূর্ণসংখ্যা) -1) ধ্রুবক
  {
    unsigned pos;

    যদি (bsearch_impl (x, &pos))
    {
      যদি (i)
	*i = pos;
      ফেরত সত্য;
    }

    যদি (i)
    {
      নির্বাচন (not_found)
      {
	ক্ষেত্র HB_NOT_FOUND_DONT_STORE:
	  বিরতি;

	ক্ষেত্র HB_NOT_FOUND_STORE:
	  *i = to_store;
	  বিরতি;

	ক্ষেত্র HB_NOT_FOUND_STORE_CLOSEST:
	  *i = pos;
	  বিরতি;
      }
    }
    ফেরত মিথ্যা;
  }
  template <typename T, typename ...Ts>
  যৌক্তিক bsearch_impl (ধ্রুবক T &x, unsigned *pos, Ts... ds) ধ্রুবক
  {
    ফেরত hb_bsearch_impl (pos,
			    x,
			    এই->arrayZ,
			    এই->দৈর্ঘ্য,
			    আকার (Type),
			    _hb_cmp_method<T, Type, Ts...>,
			    std::forward<Ts> (ds)...);
  }
};
template <typename T> অন্তঃরেখ hb_sorted_array_t<T>
hb_sorted_array (T *array, unsigned পূর্ণসংখ্যা দৈর্ঘ্য)
{ ফেরত hb_sorted_array_t<T> (array, দৈর্ঘ্য); }
template <typename T, unsigned পূর্ণসংখ্যা দৈর্ঘ্য_2> অন্তঃরেখ hb_sorted_array_t<T>
hb_sorted_array (T (&array_)[দৈর্ঘ্য_2])
{ ফেরত hb_sorted_array_t<T> (array_); }

template <typename T>
অন্তঃরেখ যৌক্তিক hb_array_t<T>::operator == (ধ্রুবক hb_array_t<T> &o) ধ্রুবক
{
  যদি (o.দৈর্ঘ্য != এই->দৈর্ঘ্য) ফেরত মিথ্যা;
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < এই->দৈর্ঘ্য; i++) {
    যদি (এই->arrayZ[i] != o.arrayZ[i]) ফেরত মিথ্যা;
  }
  ফেরত সত্য;
}
template <>
অন্তঃরেখ যৌক্তিক hb_array_t<ধ্রুবক অক্ষর>::operator == (ধ্রুবক hb_array_t<ধ্রুবক অক্ষর> &o) ধ্রুবক
{
  যদি (o.দৈর্ঘ্য != এই->দৈর্ঘ্য) ফেরত মিথ্যা;
  ফেরত 0 == hb_memcmp (arrayZ, o.arrayZ, দৈর্ঘ্য);
}
template <>
অন্তঃরেখ যৌক্তিক hb_array_t<ধ্রুবক unsigned অক্ষর>::operator == (ধ্রুবক hb_array_t<ধ্রুবক unsigned অক্ষর> &o) ধ্রুবক
{
  যদি (o.দৈর্ঘ্য != এই->দৈর্ঘ্য) ফেরত মিথ্যা;
  ফেরত 0 == hb_memcmp (arrayZ, o.arrayZ, দৈর্ঘ্য);
}


/* Specialize hash() for byte arrays. */

#ifndef HB_OPTIMIZE_আকার_MORE
template <>
অন্তঃরেখ uint32_t hb_array_t<ধ্রুবক অক্ষর>::hash () ধ্রুবক
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  ফেরত fasthash32(arrayZ, দৈর্ঘ্য, 0xf437ffe6 /* magic? */);
}

template <>
অন্তঃরেখ uint32_t hb_array_t<ধ্রুবক unsigned অক্ষর>::hash () ধ্রুবক
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  ফেরত fasthash32(arrayZ, দৈর্ঘ্য, 0xf437ffe6 /* magic? */);
}
#endif


প্রকারলেখ hb_array_t<ধ্রুবক অক্ষর> hb_bytes_t;
প্রকারলেখ hb_array_t<ধ্রুবক unsigned অক্ষর> hb_ubytes_t;



#endif /* HB_ARRAY_HH */

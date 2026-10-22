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

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-algs.hh"
#ଅନ୍ତର୍ଭାଵ "hb-iter.hh"
#ଅନ୍ତର୍ଭାଵ "hb-null.hh"


template <typename Type>
ସଂରଚନା hb_sorted_array_t;

ଗଣନା hb_not_found_t
{
  HB_NOT_FOUND_DONT_STORE,
  HB_NOT_FOUND_STORE,
  HB_NOT_FOUND_STORE_CLOSEST,
};


template <typename Type>
ସଂରଚନା hb_array_t : hb_iter_with_fallback_t<hb_array_t<Type>, Type&>
{
  ସ୍ଥିର constexpr ତାର୍କିକ realloc_move = ସତ୍ୟ;

  /*
   * Constructors.
   */
  hb_array_t () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  hb_array_t (ଅଚଳ hb_array_t&) = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  ~hb_array_t () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  hb_array_t& operator= (ଅଚଳ hb_array_t&) = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  hb_array_t& operator= (hb_array_t&&) = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;

  constexpr hb_array_t (Type *array_, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା length_) : arrayZ (array_), length (length_) {}
  template <unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା length_>
  constexpr hb_array_t (Type (&array_)[length_]) : hb_array_t (array_, length_) {}

  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_array_t (ଅଚଳ hb_array_t<U> &o) :
    hb_iter_with_fallback_t<hb_array_t, Type&> (),
    arrayZ (o.arrayZ), length (o.length), backwards_length (o.backwards_length) {}
  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_array_t& operator = (ଅଚଳ hb_array_t<U> &o)
  { arrayZ = o.arrayZ; length = o.length; backwards_length = o.backwards_length; ଫେରନ୍ତୁ *ଏହି; }

  /*
   * Iterator implementation.
   */
  ପ୍ରକାରବ୍ୟାଖ୍ୟା Type& __item_t__;
  ସ୍ଥିର constexpr ତାର୍କିକ is_random_access_iterator = ସତ୍ୟ;
  ସ୍ଥିର constexpr ତାର୍କିକ has_fast_len = ସତ୍ୟ;
  Type& __item__ () ଅଚଳ
  {
    ଯଦି (unlikely (!length)) ଫେରନ୍ତୁ CrapOrNull (Type);
    ଫେରନ୍ତୁ *arrayZ;
  }
  Type& __item_at__ (unsigned i) ଅଚଳ
  {
    ଯଦି (unlikely (i >= length)) ଫେରନ୍ତୁ CrapOrNull (Type);
    ଫେରନ୍ତୁ arrayZ[i];
  }
  ଶୂନ୍ୟ __next__ ()
  {
    ଯଦି (unlikely (!length))
      ଫେରନ୍ତୁ;
    length--;
    backwards_length++;
    arrayZ++;
  }
  ଶୂନ୍ୟ __forward__ (unsigned n)
  {
    ଯଦି (unlikely (n > length))
      n = length;
    length -= n;
    backwards_length += n;
    arrayZ += n;
  }
  ଶୂନ୍ୟ __prev__ ()
  {
    ଯଦି (unlikely (!backwards_length))
      ଫେରନ୍ତୁ;
    length++;
    backwards_length--;
    arrayZ--;
  }
  ଶୂନ୍ୟ __rewind__ (unsigned n)
  {
    ଯଦି (unlikely (n > backwards_length))
      n = backwards_length;
    length += n;
    backwards_length -= n;
    arrayZ -= n;
  }
  unsigned __len__ () ଅଚଳ { ଫେରନ୍ତୁ length; }
  /* Ouch. The operator== compares the contents of the array.  For range-based for loops,
   * it's best if we can just compare arrayZ, though comparing contents is still fast,
   * but also would require that Type has operator==.  As such, we optimize this operator
   * for range-based for loop and just compare arrayZ and length.
   *
   * The above comment is outdated now because we implemented separate begin/end to
   * objects that were using hb_array_t for range-based loop before. */
  ତାର୍କିକ operator != (ଅଚଳ hb_array_t& o) ଅଚଳ
  { ଫେରନ୍ତୁ ଏହି->arrayZ != o.arrayZ || ଏହି->length != o.length; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () ଅଚଳ { ଫେରନ୍ତୁ arrayZ; }
  Type *end () ଅଚଳ { ଫେରନ୍ତୁ arrayZ + length; }


  /* Extra operators.
   */
  Type * operator & () ଅଚଳ { ଫେରନ୍ତୁ arrayZ; }
  operator hb_array_t<ଅଚଳ Type> () { ଫେରନ୍ତୁ hb_array_t<ଅଚଳ Type> (arrayZ, length); }
  template <typename T> operator T * () ଅଚଳ { ଫେରନ୍ତୁ arrayZ; }

  HB_INTERNAL ତାର୍କିକ operator == (ଅଚଳ hb_array_t &o) ଅଚଳ;

  uint32_t hash () ଅଚଳ
  {
    // FNV-1a hash function
    // https://github.com/harfbuzz/harfbuzz/pull/4228
    uint32_t current = /*cbf29ce4*/0x84222325;
    ପାଇଁ (auto &v : *ଏହି)
    {
      current = current ^ hb_hash (v);
      current = current * 16777619;
    }
    ଫେରନ୍ତୁ current;
  }

  /*
   * Compare, Sort, and Search.
   */

  /* Note: our compare is NOT lexicographic; it also does NOT call Type::cmp. */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (ଅଚଳ hb_array_t &a) ଅଚଳ
  {
    ଯଦି (length != a.length)
      ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) a.length - (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) length;
    ଫେରନ୍ତୁ hb_memcmp (a.arrayZ, arrayZ, get_size ());
  }
  HB_INTERNAL ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (ଅଚଳ ଶୂନ୍ୟ *pa, ଅଚଳ ଶୂନ୍ୟ *pb)
  {
    hb_array_t *a = (hb_array_t *) pa;
    hb_array_t *b = (hb_array_t *) pb;
    ଫେରନ୍ତୁ b->cmp (*a);
  }

  template <typename T>
  Type *lsearch (ଅଚଳ T &x, Type *not_found = nullptr)
  {
    unsigned i;
    ଫେରନ୍ତୁ lfind (x, &i) ? &ଏହି->arrayZ[i] : not_found;
  }
  template <typename T>
  ଅଚଳ Type *lsearch (ଅଚଳ T &x, ଅଚଳ Type *not_found = nullptr) ଅଚଳ
  {
    unsigned i;
    ଫେରନ୍ତୁ lfind (x, &i) ? &ଏହି->arrayZ[i] : not_found;
  }
  template <typename T>
  ତାର୍କିକ lfind (ଅଚଳ T &x, unsigned *pos = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା to_store = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) -1) ଅଚଳ
  {
    ପାଇଁ (unsigned i = 0; i < length; ++i)
      ଯଦି (hb_equal (x, ଏହି->arrayZ[i]))
      {
	ଯଦି (pos)
	  *pos = i;
	ଫେରନ୍ତୁ ସତ୍ୟ;
      }

    ଯଦି (pos)
    {
      ବାଛ (not_found)
      {
	ସ୍ଥିତି HB_NOT_FOUND_DONT_STORE:
	  ଭାଙ୍ଗ;

	ସ୍ଥିତି HB_NOT_FOUND_STORE:
	  *pos = to_store;
	  ଭାଙ୍ଗ;

	ସ୍ଥିତି HB_NOT_FOUND_STORE_CLOSEST:
	  *pos = length;
	  ଭାଙ୍ଗ;
      }
    }
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  hb_sorted_array_t<Type> qsort (ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*cmp)(ଅଚଳ ଶୂନ୍ୟ*, ଅଚଳ ଶୂନ୍ୟ*))
  {
    ଯଦି (likely (length))
      hb_qsort (arrayZ, length, ଏହି->get_item_size (), cmp);
    ଫେରନ୍ତୁ hb_sorted_array_t<Type> (*ଏହି);
  }

  /* Comparator follows the C qsort convention: returns
   * negative / zero / positive int. */
  template <typename Compar>
  hb_sorted_array_t<Type> qsort (Compar compar)
  {
    ଯଦି (likely (length))
      hb_qsort_inline (arrayZ, length, compar);
    ଫେରନ୍ତୁ hb_sorted_array_t<Type> (*ଏହି);
  }

  ବ୍ୟକ୍ତିଗତ:
  template <typename T = Type,
	    hb_enable_if (std::is_move_assignable<T>::value)>
  hb_sorted_array_t<Type> _qsort (hb_priority<1>)
  {
    ଫେରନ୍ତୁ qsort ([] (ଅଚଳ Type &a, ଅଚଳ Type &b) { ଫେରନ୍ତୁ Type::cmp (&a, &b); });
  }
  hb_sorted_array_t<Type> _qsort (hb_priority<0>)
  {
    ଫେରନ୍ତୁ qsort ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା(*)(ଅଚଳ ଶୂନ୍ୟ*, ଅଚଳ ଶୂନ୍ୟ*)) Type::cmp);
  }
  ସର୍ବସାଧାରଣ:

  hb_sorted_array_t<Type> qsort ()
  {
    ଫେରନ୍ତୁ _qsort (hb_prioritize);
  }

  /*
   * Other methods.
   */

  size_t get_size () ଅଚଳ { ଫେରନ୍ତୁ length * ଏହି->get_item_size (); }

  /*
   * Reverse the order of items in this array in the range [start, end).
   */
  ଶୂନ୍ୟ reverse (unsigned start = 0, unsigned end = -1)
  {
    start = hb_min (start, length);
    end = hb_min (end, length);

    ଯଦି (end < start + 2)
      ଫେରନ୍ତୁ;

    unsigned stop = start + (end - start) / 2;
    ପାଇଁ (unsigned lhs = start, rhs = end - 1; lhs < stop; lhs++, rhs--)
      hb_swap (arrayZ[rhs], arrayZ[lhs]);
  }

  hb_array_t sub_array (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_offset = 0, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *seg_ଗଣନା = nullptr /* IN/OUT */) ଅଚଳ
  {
    ଯଦି (!start_offset && !seg_ଗଣନା)
      ଫେରନ୍ତୁ *ଏହି;

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = length;
    ଯଦି (unlikely (start_offset > ଗଣନା_2))
      ଗଣନା_2 = 0;
    ଅନ୍ୟଥା
      ଗଣନା_2 -= start_offset;
    ଯଦି (seg_ଗଣନା)
      ଗଣନା_2 = *seg_ଗଣନା = hb_min (ଗଣନା_2, *seg_ଗଣନା);
    ଫେରନ୍ତୁ hb_array_t (arrayZ + start_offset, ଗଣନା_2);
  }
  hb_array_t sub_array (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_offset, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା seg_ଗଣନା) ଅଚଳ
  { ଫେରନ୍ତୁ sub_array (start_offset, &seg_ଗଣନା); }

  hb_array_t truncate (unsigned length) ଅଚଳ { ଫେରନ୍ତୁ sub_array (0, length); }

  template <typename T,
	    unsigned P = ଆକାର (Type),
	    hb_enable_if (P == 1)>
  ଅଚଳ T *as () ଅଚଳ
  { ଫେରନ୍ତୁ length < hb_min_size (T) ? &Null (T) : reinterpret_cast<ଅଚଳ T *> (arrayZ); }

  template <typename T,
	    unsigned P = ଆକାର (Type),
	    hb_enable_if (P == 1)>
  ତାର୍କିକ check_range (ଅଚଳ T *p, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା size = T::static_size) ଅଚଳ
  {
    ଫେରନ୍ତୁ arrayZ <= ((ଅଚଳ ଅକ୍ଷର *) p)
	&& ((ଅଚଳ ଅକ୍ଷର *) p) <= arrayZ + length
	&& (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (arrayZ + length - (ଅଚଳ ଅକ୍ଷର *) p) >= size;
  }

  template <unsigned P = ଆକାର (Type),
	    hb_enable_if (P == 1)>
  ତାର୍କିକ check_end (ଅଚଳ ଶୂନ୍ୟ *p) ଅଚଳ
  {
    ଫେରନ୍ତୁ (uintptr_t) (((ଅଚଳ ଅକ୍ଷର *) p) - arrayZ) <= length;
  }

  /* Only call if you allocated the underlying array using hb_malloc() or similar. */
  ଶୂନ୍ୟ fini ()
  { hb_free ((ଶୂନ୍ୟ *) arrayZ); arrayZ = nullptr; length = 0; }

  template <typename hb_serialize_context_t,
	    typename U = Type,
	    hb_enable_if (!(ଆକାର (U) < ଆକାର (long long) && hb_is_trivially_copy_assignable(hb_decay<Type>)))>
  hb_array_t copy (hb_serialize_context_t *c) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    auto* out = c->start_embed (arrayZ);
    ଯଦି (unlikely (!c->extend_size (out, get_size (), ମିଥ୍ୟା))) return_trace (hb_array_t ());
    ପାଇଁ (unsigned i = 0; i < length; i++)
      out[i] = arrayZ[i]; /* TODO: add version that calls c->copy() */
    return_trace (hb_array_t (out, length));
  }

  template <typename hb_serialize_context_t,
	    typename U = Type,
	    hb_enable_if (ଆକାର (U) < ଆକାର (long long) && hb_is_trivially_copy_assignable(hb_decay<Type>))>
  hb_array_t copy (hb_serialize_context_t *c) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    auto* out = c->start_embed (arrayZ);
    ଯଦି (unlikely (!c->extend_size (out, get_size (), ମିଥ୍ୟା))) return_trace (hb_array_t ());
    hb_memcpy (out, arrayZ, get_size ());
    return_trace (hb_array_t (out, length));
  }

  template <typename hb_sanitize_context_t>
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  { ଫେରନ୍ତୁ c->check_array (arrayZ, length); }

  /*
   * Members
   */

  ସର୍ବସାଧାରଣ:
  Type *arrayZ = nullptr;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା length = 0;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା backwards_length = 0;
};
template <typename T> ଧାଡ଼ିଭିତରେ hb_array_t<T>
hb_array ()
{ ଫେରନ୍ତୁ hb_array_t<T> (); }
template <typename T> ଧାଡ଼ିଭିତରେ hb_array_t<T>
hb_array (T *array, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା length)
{ ଫେରନ୍ତୁ hb_array_t<T> (array, length); }
template <typename T, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା length_> ଧାଡ଼ିଭିତରେ hb_array_t<T>
hb_array (T (&array_)[length_])
{ ଫେରନ୍ତୁ hb_array_t<T> (array_); }

template <typename Type>
ସଂରଚନା hb_sorted_array_t :
	hb_array_t<Type>,
	hb_iter_t<hb_sorted_array_t<Type>, Type&>
{
  ପ୍ରକାରବ୍ୟାଖ୍ୟା hb_iter_t<hb_sorted_array_t, Type&> iter_base_t;
  HB_ITER_USING (iter_base_t);
  ସ୍ଥିର constexpr ତାର୍କିକ is_random_access_iterator = ସତ୍ୟ;
  ସ୍ଥିର constexpr ତାର୍କିକ is_sorted_iterator = ସତ୍ୟ;
  ସ୍ଥିର constexpr ତାର୍କିକ has_fast_len = ସତ୍ୟ;

  hb_sorted_array_t () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  hb_sorted_array_t (ଅଚଳ hb_sorted_array_t&) = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  ~hb_sorted_array_t () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  hb_sorted_array_t& operator= (ଅଚଳ hb_sorted_array_t&) = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  hb_sorted_array_t& operator= (hb_sorted_array_t&&) = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;

  constexpr hb_sorted_array_t (Type *array_, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା length_) : hb_array_t<Type> (array_, length_) {}
  template <unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା length_>
  constexpr hb_sorted_array_t (Type (&array_)[length_]) : hb_array_t<Type> (array_) {}

  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_sorted_array_t (ଅଚଳ hb_array_t<U> &o) :
    hb_array_t<Type> (o),
    hb_iter_t<hb_sorted_array_t, Type&> () {}
  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_sorted_array_t& operator = (ଅଚଳ hb_array_t<U> &o)
  { hb_array_t<Type> (*ଏହି) = o; ଫେରନ୍ତୁ *ଏହି; }

  /* Iterator implementation. */

  /* See comment in hb_array_of::operator != */
  ତାର୍କିକ operator != (ଅଚଳ hb_sorted_array_t& o) ଅଚଳ
  { ଫେରନ୍ତୁ ଏହି->arrayZ != o.arrayZ || ଏହି->length != o.length; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () ଅଚଳ { ଫେରନ୍ତୁ ଏହି->arrayZ; }
  Type *end () ଅଚଳ { ଫେରନ୍ତୁ ଏହି->arrayZ + ଏହି->length; }


  hb_sorted_array_t sub_array (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_offset, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *seg_ଗଣନା /* IN/OUT */) ଅଚଳ
  { ଫେରନ୍ତୁ hb_sorted_array_t (((ଅଚଳ hb_array_t<Type> *) (ଏହି))->sub_array (start_offset, seg_ଗଣନା)); }
  hb_sorted_array_t sub_array (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_offset, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା seg_ଗଣନା) ଅଚଳ
  { ଫେରନ୍ତୁ sub_array (start_offset, &seg_ଗଣନା); }

  hb_sorted_array_t truncate (unsigned length) ଅଚଳ { ଫେରନ୍ତୁ sub_array (0, length); }

  template <typename T>
  Type *bsearch (ଅଚଳ T &x, Type *not_found = nullptr)
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ଫେରନ୍ତୁ bfind (x, &i) ? &ଏହି->arrayZ[i] : not_found;
  }
  template <typename T>
  ଅଚଳ Type *bsearch (ଅଚଳ T &x, ଅଚଳ Type *not_found = nullptr) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ଫେରନ୍ତୁ bfind (x, &i) ? &ଏହି->arrayZ[i] : not_found;
  }
  template <typename T>
  ତାର୍କିକ bfind (ଅଚଳ T &x, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା to_store = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) -1) ଅଚଳ
  {
    unsigned pos;

    ଯଦି (bsearch_impl (x, &pos))
    {
      ଯଦି (i)
	*i = pos;
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    ଯଦି (i)
    {
      ବାଛ (not_found)
      {
	ସ୍ଥିତି HB_NOT_FOUND_DONT_STORE:
	  ଭାଙ୍ଗ;

	ସ୍ଥିତି HB_NOT_FOUND_STORE:
	  *i = to_store;
	  ଭାଙ୍ଗ;

	ସ୍ଥିତି HB_NOT_FOUND_STORE_CLOSEST:
	  *i = pos;
	  ଭାଙ୍ଗ;
      }
    }
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }
  template <typename T, typename ...Ts>
  ତାର୍କିକ bsearch_impl (ଅଚଳ T &x, unsigned *pos, Ts... ds) ଅଚଳ
  {
    ଫେରନ୍ତୁ hb_bsearch_impl (pos,
			    x,
			    ଏହି->arrayZ,
			    ଏହି->length,
			    ଆକାର (Type),
			    _hb_cmp_method<T, Type, Ts...>,
			    std::forward<Ts> (ds)...);
  }
};
template <typename T> ଧାଡ଼ିଭିତରେ hb_sorted_array_t<T>
hb_sorted_array (T *array, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା length)
{ ଫେରନ୍ତୁ hb_sorted_array_t<T> (array, length); }
template <typename T, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା length_> ଧାଡ଼ିଭିତରେ hb_sorted_array_t<T>
hb_sorted_array (T (&array_)[length_])
{ ଫେରନ୍ତୁ hb_sorted_array_t<T> (array_); }

template <typename T>
ଧାଡ଼ିଭିତରେ ତାର୍କିକ hb_array_t<T>::operator == (ଅଚଳ hb_array_t<T> &o) ଅଚଳ
{
  ଯଦି (o.length != ଏହି->length) ଫେରନ୍ତୁ ମିଥ୍ୟା;
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଏହି->length; i++) {
    ଯଦି (ଏହି->arrayZ[i] != o.arrayZ[i]) ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }
  ଫେରନ୍ତୁ ସତ୍ୟ;
}
template <>
ଧାଡ଼ିଭିତରେ ତାର୍କିକ hb_array_t<ଅଚଳ ଅକ୍ଷର>::operator == (ଅଚଳ hb_array_t<ଅଚଳ ଅକ୍ଷର> &o) ଅଚଳ
{
  ଯଦି (o.length != ଏହି->length) ଫେରନ୍ତୁ ମିଥ୍ୟା;
  ଫେରନ୍ତୁ 0 == hb_memcmp (arrayZ, o.arrayZ, length);
}
template <>
ଧାଡ଼ିଭିତରେ ତାର୍କିକ hb_array_t<ଅଚଳ unsigned ଅକ୍ଷର>::operator == (ଅଚଳ hb_array_t<ଅଚଳ unsigned ଅକ୍ଷର> &o) ଅଚଳ
{
  ଯଦି (o.length != ଏହି->length) ଫେରନ୍ତୁ ମିଥ୍ୟା;
  ଫେରନ୍ତୁ 0 == hb_memcmp (arrayZ, o.arrayZ, length);
}


/* Specialize hash() for byte arrays. */

#ifndef HB_OPTIMIZE_SIZE_MORE
template <>
ଧାଡ଼ିଭିତରେ uint32_t hb_array_t<ଅଚଳ ଅକ୍ଷର>::hash () ଅଚଳ
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  ଫେରନ୍ତୁ fasthash32(arrayZ, length, 0xf437ffe6 /* magic? */);
}

template <>
ଧାଡ଼ିଭିତରେ uint32_t hb_array_t<ଅଚଳ unsigned ଅକ୍ଷର>::hash () ଅଚଳ
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  ଫେରନ୍ତୁ fasthash32(arrayZ, length, 0xf437ffe6 /* magic? */);
}
#endif


ପ୍ରକାରବ୍ୟାଖ୍ୟା hb_array_t<ଅଚଳ ଅକ୍ଷର> hb_bytes_t;
ପ୍ରକାରବ୍ୟାଖ୍ୟା hb_array_t<ଅଚଳ unsigned ଅକ୍ଷର> hb_ubytes_t;



#endif /* HB_ARRAY_HH */

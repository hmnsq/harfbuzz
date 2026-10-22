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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-algs.hh"
#अन्तर्भाव "hb-iter.hh"
#अन्तर्भाव "hb-null.hh"


template <typename Type>
संरचना hb_sorted_array_t;

गणना hb_not_found_t
{
  HB_NOT_FOUND_DONT_STORE,
  HB_NOT_FOUND_STORE,
  HB_NOT_FOUND_STORE_CLOSEST,
};


template <typename Type>
संरचना hb_array_t : hb_iter_with_fallback_t<hb_array_t<Type>, Type&>
{
  स्थिर constexpr तार्किक realloc_move = सत्य;

  /*
   * Constructors.
   */
  hb_array_t () = स्वभाव;
  hb_array_t (ध्रुव hb_array_t&) = स्वभाव;
  ~hb_array_t () = स्वभाव;
  hb_array_t& operator= (ध्रुव hb_array_t&) = स्वभाव;
  hb_array_t& operator= (hb_array_t&&) = स्वभाव;

  constexpr hb_array_t (Type *array_, unsigned पूर्णाङ्क length_) : arrayZ (array_), length (length_) {}
  template <unsigned पूर्णाङ्क length_>
  constexpr hb_array_t (Type (&array_)[length_]) : hb_array_t (array_, length_) {}

  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_array_t (ध्रुव hb_array_t<U> &o) :
    hb_iter_with_fallback_t<hb_array_t, Type&> (),
    arrayZ (o.arrayZ), length (o.length), backwards_length (o.backwards_length) {}
  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_array_t& operator = (ध्रुव hb_array_t<U> &o)
  { arrayZ = o.arrayZ; length = o.length; backwards_length = o.backwards_length; प्रत्यागम *एतत्; }

  /*
   * Iterator implementation.
   */
  प्रकारनिर्देश Type& __item_t__;
  स्थिर constexpr तार्किक is_random_access_iterator = सत्य;
  स्थिर constexpr तार्किक has_fast_len = सत्य;
  Type& __item__ () ध्रुव
  {
    यदि (unlikely (!length)) प्रत्यागम CrapOrNull (Type);
    प्रत्यागम *arrayZ;
  }
  Type& __item_at__ (unsigned i) ध्रुव
  {
    यदि (unlikely (i >= length)) प्रत्यागम CrapOrNull (Type);
    प्रत्यागम arrayZ[i];
  }
  शून्य __next__ ()
  {
    यदि (unlikely (!length))
      प्रत्यागम;
    length--;
    backwards_length++;
    arrayZ++;
  }
  शून्य __forward__ (unsigned n)
  {
    यदि (unlikely (n > length))
      n = length;
    length -= n;
    backwards_length += n;
    arrayZ += n;
  }
  शून्य __prev__ ()
  {
    यदि (unlikely (!backwards_length))
      प्रत्यागम;
    length++;
    backwards_length--;
    arrayZ--;
  }
  शून्य __rewind__ (unsigned n)
  {
    यदि (unlikely (n > backwards_length))
      n = backwards_length;
    length += n;
    backwards_length -= n;
    arrayZ -= n;
  }
  unsigned __len__ () ध्रुव { प्रत्यागम length; }
  /* Ouch. The operator== compares the contents of the array.  For range-based for loops,
   * it's best if we can just compare arrayZ, though comparing contents is still fast,
   * but also would require that Type has operator==.  As such, we optimize this operator
   * for range-based for loop and just compare arrayZ and length.
   *
   * The above comment is outdated now because we implemented separate begin/end to
   * objects that were using hb_array_t for range-based loop before. */
  तार्किक operator != (ध्रुव hb_array_t& o) ध्रुव
  { प्रत्यागम एतत्->arrayZ != o.arrayZ || एतत्->length != o.length; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () ध्रुव { प्रत्यागम arrayZ; }
  Type *end () ध्रुव { प्रत्यागम arrayZ + length; }


  /* Extra operators.
   */
  Type * operator & () ध्रुव { प्रत्यागम arrayZ; }
  operator hb_array_t<ध्रुव Type> () { प्रत्यागम hb_array_t<ध्रुव Type> (arrayZ, length); }
  template <typename T> operator T * () ध्रुव { प्रत्यागम arrayZ; }

  HB_INTERNAL तार्किक operator == (ध्रुव hb_array_t &o) ध्रुव;

  uint32_t hash () ध्रुव
  {
    // FNV-1a hash function
    // https://github.com/harfbuzz/harfbuzz/pull/4228
    uint32_t current = /*cbf29ce4*/0x84222325;
    प्रति (auto &v : *एतत्)
    {
      current = current ^ hb_hash (v);
      current = current * 16777619;
    }
    प्रत्यागम current;
  }

  /*
   * Compare, Sort, and Search.
   */

  /* Note: our compare is NOT lexicographic; it also does NOT call Type::cmp. */
  पूर्णाङ्क cmp (ध्रुव hb_array_t &a) ध्रुव
  {
    यदि (length != a.length)
      प्रत्यागम (पूर्णाङ्क) a.length - (पूर्णाङ्क) length;
    प्रत्यागम hb_memcmp (a.arrayZ, arrayZ, get_size ());
  }
  HB_INTERNAL स्थिर पूर्णाङ्क cmp (ध्रुव शून्य *pa, ध्रुव शून्य *pb)
  {
    hb_array_t *a = (hb_array_t *) pa;
    hb_array_t *b = (hb_array_t *) pb;
    प्रत्यागम b->cmp (*a);
  }

  template <typename T>
  Type *lsearch (ध्रुव T &x, Type *not_found = nullptr)
  {
    unsigned i;
    प्रत्यागम lfind (x, &i) ? &एतत्->arrayZ[i] : not_found;
  }
  template <typename T>
  ध्रुव Type *lsearch (ध्रुव T &x, ध्रुव Type *not_found = nullptr) ध्रुव
  {
    unsigned i;
    प्रत्यागम lfind (x, &i) ? &एतत्->arrayZ[i] : not_found;
  }
  template <typename T>
  तार्किक lfind (ध्रुव T &x, unsigned *pos = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned पूर्णाङ्क to_store = (unsigned पूर्णाङ्क) -1) ध्रुव
  {
    प्रति (unsigned i = 0; i < length; ++i)
      यदि (hb_equal (x, एतत्->arrayZ[i]))
      {
	यदि (pos)
	  *pos = i;
	प्रत्यागम सत्य;
      }

    यदि (pos)
    {
      विकल्प (not_found)
      {
	पक्ष HB_NOT_FOUND_DONT_STORE:
	  भङ्ग;

	पक्ष HB_NOT_FOUND_STORE:
	  *pos = to_store;
	  भङ्ग;

	पक्ष HB_NOT_FOUND_STORE_CLOSEST:
	  *pos = length;
	  भङ्ग;
      }
    }
    प्रत्यागम असत्य;
  }

  hb_sorted_array_t<Type> qsort (पूर्णाङ्क (*cmp)(ध्रुव शून्य*, ध्रुव शून्य*))
  {
    यदि (likely (length))
      hb_qsort (arrayZ, length, एतत्->get_item_size (), cmp);
    प्रत्यागम hb_sorted_array_t<Type> (*एतत्);
  }

  /* Comparator follows the C qsort convention: returns
   * negative / zero / positive int. */
  template <typename Compar>
  hb_sorted_array_t<Type> qsort (Compar compar)
  {
    यदि (likely (length))
      hb_qsort_inline (arrayZ, length, compar);
    प्रत्यागम hb_sorted_array_t<Type> (*एतत्);
  }

  गुप्त:
  template <typename T = Type,
	    hb_enable_if (std::is_move_assignable<T>::value)>
  hb_sorted_array_t<Type> _qsort (hb_priority<1>)
  {
    प्रत्यागम qsort ([] (ध्रुव Type &a, ध्रुव Type &b) { प्रत्यागम Type::cmp (&a, &b); });
  }
  hb_sorted_array_t<Type> _qsort (hb_priority<0>)
  {
    प्रत्यागम qsort ((पूर्णाङ्क(*)(ध्रुव शून्य*, ध्रुव शून्य*)) Type::cmp);
  }
  सार्वजनिक:

  hb_sorted_array_t<Type> qsort ()
  {
    प्रत्यागम _qsort (hb_prioritize);
  }

  /*
   * Other methods.
   */

  size_t get_size () ध्रुव { प्रत्यागम length * एतत्->get_item_size (); }

  /*
   * Reverse the order of items in this array in the range [start, end).
   */
  शून्य reverse (unsigned start = 0, unsigned end = -1)
  {
    start = hb_min (start, length);
    end = hb_min (end, length);

    यदि (end < start + 2)
      प्रत्यागम;

    unsigned stop = start + (end - start) / 2;
    प्रति (unsigned lhs = start, rhs = end - 1; lhs < stop; lhs++, rhs--)
      hb_swap (arrayZ[rhs], arrayZ[lhs]);
  }

  hb_array_t sub_array (unsigned पूर्णाङ्क start_offset = 0, unsigned पूर्णाङ्क *seg_मोजणी = nullptr /* IN/OUT */) ध्रुव
  {
    यदि (!start_offset && !seg_मोजणी)
      प्रत्यागम *एतत्;

    unsigned पूर्णाङ्क मोजणी = length;
    यदि (unlikely (start_offset > मोजणी))
      मोजणी = 0;
    अन्यथा
      मोजणी -= start_offset;
    यदि (seg_मोजणी)
      मोजणी = *seg_मोजणी = hb_min (मोजणी, *seg_मोजणी);
    प्रत्यागम hb_array_t (arrayZ + start_offset, मोजणी);
  }
  hb_array_t sub_array (unsigned पूर्णाङ्क start_offset, unsigned पूर्णाङ्क seg_मोजणी) ध्रुव
  { प्रत्यागम sub_array (start_offset, &seg_मोजणी); }

  hb_array_t truncate (unsigned length) ध्रुव { प्रत्यागम sub_array (0, length); }

  template <typename T,
	    unsigned P = परिमाण (Type),
	    hb_enable_if (P == 1)>
  ध्रुव T *as () ध्रुव
  { प्रत्यागम length < hb_min_size (T) ? &Null (T) : reinterpret_cast<ध्रुव T *> (arrayZ); }

  template <typename T,
	    unsigned P = परिमाण (Type),
	    hb_enable_if (P == 1)>
  तार्किक check_range (ध्रुव T *p, unsigned पूर्णाङ्क size = T::static_size) ध्रुव
  {
    प्रत्यागम arrayZ <= ((ध्रुव वर्ण *) p)
	&& ((ध्रुव वर्ण *) p) <= arrayZ + length
	&& (unsigned पूर्णाङ्क) (arrayZ + length - (ध्रुव वर्ण *) p) >= size;
  }

  template <unsigned P = परिमाण (Type),
	    hb_enable_if (P == 1)>
  तार्किक check_end (ध्रुव शून्य *p) ध्रुव
  {
    प्रत्यागम (uintptr_t) (((ध्रुव वर्ण *) p) - arrayZ) <= length;
  }

  /* Only call if you allocated the underlying array using hb_malloc() or similar. */
  शून्य fini ()
  { hb_free ((शून्य *) arrayZ); arrayZ = nullptr; length = 0; }

  template <typename hb_serialize_context_t,
	    typename U = Type,
	    hb_enable_if (!(परिमाण (U) < परिमाण (long long) && hb_is_trivially_copy_assignable(hb_decay<Type>)))>
  hb_array_t copy (hb_serialize_context_t *c) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    auto* out = c->start_embed (arrayZ);
    यदि (unlikely (!c->extend_size (out, get_size (), असत्य))) return_trace (hb_array_t ());
    प्रति (unsigned i = 0; i < length; i++)
      out[i] = arrayZ[i]; /* TODO: add version that calls c->copy() */
    return_trace (hb_array_t (out, length));
  }

  template <typename hb_serialize_context_t,
	    typename U = Type,
	    hb_enable_if (परिमाण (U) < परिमाण (long long) && hb_is_trivially_copy_assignable(hb_decay<Type>))>
  hb_array_t copy (hb_serialize_context_t *c) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    auto* out = c->start_embed (arrayZ);
    यदि (unlikely (!c->extend_size (out, get_size (), असत्य))) return_trace (hb_array_t ());
    hb_memcpy (out, arrayZ, get_size ());
    return_trace (hb_array_t (out, length));
  }

  template <typename hb_sanitize_context_t>
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  { प्रत्यागम c->check_array (arrayZ, length); }

  /*
   * Members
   */

  सार्वजनिक:
  Type *arrayZ = nullptr;
  unsigned पूर्णाङ्क length = 0;
  unsigned पूर्णाङ्क backwards_length = 0;
};
template <typename T> अन्तःस्थ hb_array_t<T>
hb_array ()
{ प्रत्यागम hb_array_t<T> (); }
template <typename T> अन्तःस्थ hb_array_t<T>
hb_array (T *array, unsigned पूर्णाङ्क length)
{ प्रत्यागम hb_array_t<T> (array, length); }
template <typename T, unsigned पूर्णाङ्क length_> अन्तःस्थ hb_array_t<T>
hb_array (T (&array_)[length_])
{ प्रत्यागम hb_array_t<T> (array_); }

template <typename Type>
संरचना hb_sorted_array_t :
	hb_array_t<Type>,
	hb_iter_t<hb_sorted_array_t<Type>, Type&>
{
  प्रकारनिर्देश hb_iter_t<hb_sorted_array_t, Type&> iter_base_t;
  HB_ITER_USING (iter_base_t);
  स्थिर constexpr तार्किक is_random_access_iterator = सत्य;
  स्थिर constexpr तार्किक is_sorted_iterator = सत्य;
  स्थिर constexpr तार्किक has_fast_len = सत्य;

  hb_sorted_array_t () = स्वभाव;
  hb_sorted_array_t (ध्रुव hb_sorted_array_t&) = स्वभाव;
  ~hb_sorted_array_t () = स्वभाव;
  hb_sorted_array_t& operator= (ध्रुव hb_sorted_array_t&) = स्वभाव;
  hb_sorted_array_t& operator= (hb_sorted_array_t&&) = स्वभाव;

  constexpr hb_sorted_array_t (Type *array_, unsigned पूर्णाङ्क length_) : hb_array_t<Type> (array_, length_) {}
  template <unsigned पूर्णाङ्क length_>
  constexpr hb_sorted_array_t (Type (&array_)[length_]) : hb_array_t<Type> (array_) {}

  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_sorted_array_t (ध्रुव hb_array_t<U> &o) :
    hb_array_t<Type> (o),
    hb_iter_t<hb_sorted_array_t, Type&> () {}
  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_sorted_array_t& operator = (ध्रुव hb_array_t<U> &o)
  { hb_array_t<Type> (*एतत्) = o; प्रत्यागम *एतत्; }

  /* Iterator implementation. */

  /* See comment in hb_array_of::operator != */
  तार्किक operator != (ध्रुव hb_sorted_array_t& o) ध्रुव
  { प्रत्यागम एतत्->arrayZ != o.arrayZ || एतत्->length != o.length; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () ध्रुव { प्रत्यागम एतत्->arrayZ; }
  Type *end () ध्रुव { प्रत्यागम एतत्->arrayZ + एतत्->length; }


  hb_sorted_array_t sub_array (unsigned पूर्णाङ्क start_offset, unsigned पूर्णाङ्क *seg_मोजणी /* IN/OUT */) ध्रुव
  { प्रत्यागम hb_sorted_array_t (((ध्रुव hb_array_t<Type> *) (एतत्))->sub_array (start_offset, seg_मोजणी)); }
  hb_sorted_array_t sub_array (unsigned पूर्णाङ्क start_offset, unsigned पूर्णाङ्क seg_मोजणी) ध्रुव
  { प्रत्यागम sub_array (start_offset, &seg_मोजणी); }

  hb_sorted_array_t truncate (unsigned length) ध्रुव { प्रत्यागम sub_array (0, length); }

  template <typename T>
  Type *bsearch (ध्रुव T &x, Type *not_found = nullptr)
  {
    unsigned पूर्णाङ्क i;
    प्रत्यागम bfind (x, &i) ? &एतत्->arrayZ[i] : not_found;
  }
  template <typename T>
  ध्रुव Type *bsearch (ध्रुव T &x, ध्रुव Type *not_found = nullptr) ध्रुव
  {
    unsigned पूर्णाङ्क i;
    प्रत्यागम bfind (x, &i) ? &एतत्->arrayZ[i] : not_found;
  }
  template <typename T>
  तार्किक bfind (ध्रुव T &x, unsigned पूर्णाङ्क *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned पूर्णाङ्क to_store = (unsigned पूर्णाङ्क) -1) ध्रुव
  {
    unsigned pos;

    यदि (bsearch_impl (x, &pos))
    {
      यदि (i)
	*i = pos;
      प्रत्यागम सत्य;
    }

    यदि (i)
    {
      विकल्प (not_found)
      {
	पक्ष HB_NOT_FOUND_DONT_STORE:
	  भङ्ग;

	पक्ष HB_NOT_FOUND_STORE:
	  *i = to_store;
	  भङ्ग;

	पक्ष HB_NOT_FOUND_STORE_CLOSEST:
	  *i = pos;
	  भङ्ग;
      }
    }
    प्रत्यागम असत्य;
  }
  template <typename T, typename ...Ts>
  तार्किक bsearch_impl (ध्रुव T &x, unsigned *pos, Ts... ds) ध्रुव
  {
    प्रत्यागम hb_bsearch_impl (pos,
			    x,
			    एतत्->arrayZ,
			    एतत्->length,
			    परिमाण (Type),
			    _hb_cmp_method<T, Type, Ts...>,
			    std::forward<Ts> (ds)...);
  }
};
template <typename T> अन्तःस्थ hb_sorted_array_t<T>
hb_sorted_array (T *array, unsigned पूर्णाङ्क length)
{ प्रत्यागम hb_sorted_array_t<T> (array, length); }
template <typename T, unsigned पूर्णाङ्क length_> अन्तःस्थ hb_sorted_array_t<T>
hb_sorted_array (T (&array_)[length_])
{ प्रत्यागम hb_sorted_array_t<T> (array_); }

template <typename T>
अन्तःस्थ तार्किक hb_array_t<T>::operator == (ध्रुव hb_array_t<T> &o) ध्रुव
{
  यदि (o.length != एतत्->length) प्रत्यागम असत्य;
  प्रति (unsigned पूर्णाङ्क i = 0; i < एतत्->length; i++) {
    यदि (एतत्->arrayZ[i] != o.arrayZ[i]) प्रत्यागम असत्य;
  }
  प्रत्यागम सत्य;
}
template <>
अन्तःस्थ तार्किक hb_array_t<ध्रुव वर्ण>::operator == (ध्रुव hb_array_t<ध्रुव वर्ण> &o) ध्रुव
{
  यदि (o.length != एतत्->length) प्रत्यागम असत्य;
  प्रत्यागम 0 == hb_memcmp (arrayZ, o.arrayZ, length);
}
template <>
अन्तःस्थ तार्किक hb_array_t<ध्रुव unsigned वर्ण>::operator == (ध्रुव hb_array_t<ध्रुव unsigned वर्ण> &o) ध्रुव
{
  यदि (o.length != एतत्->length) प्रत्यागम असत्य;
  प्रत्यागम 0 == hb_memcmp (arrayZ, o.arrayZ, length);
}


/* Specialize hash() for byte arrays. */

#ifndef HB_OPTIMIZE_SIZE_MORE
template <>
अन्तःस्थ uint32_t hb_array_t<ध्रुव वर्ण>::hash () ध्रुव
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  प्रत्यागम fasthash32(arrayZ, length, 0xf437ffe6 /* magic? */);
}

template <>
अन्तःस्थ uint32_t hb_array_t<ध्रुव unsigned वर्ण>::hash () ध्रुव
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  प्रत्यागम fasthash32(arrayZ, length, 0xf437ffe6 /* magic? */);
}
#endif


प्रकारनिर्देश hb_array_t<ध्रुव वर्ण> hb_bytes_t;
प्रकारनिर्देश hb_array_t<ध्रुव unsigned वर्ण> hb_ubytes_t;



#endif /* HB_ARRAY_HH */

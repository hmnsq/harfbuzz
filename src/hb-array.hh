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


ढाँचा <typename Type>
संरचना hb_sorted_array_t;

गणना hb_not_found_t
{
  HB_NOT_FOUND_DONT_STORE,
  HB_NOT_FOUND_STORE,
  HB_NOT_FOUND_STORE_CLOSEST,
};


ढाँचा <typename Type>
संरचना hb_array_t : hb_iter_with_fallback_t<hb_array_t<Type>, Type&>
{
  स्थिर constexpr तार्किक realloc_move = सत्य;

  /*
   * Constructors.
   */
  hb_array_t () = पूर्वनिर्धारित;
  hb_array_t (स्थिरांक hb_array_t&) = पूर्वनिर्धारित;
  ~hb_array_t () = पूर्वनिर्धारित;
  hb_array_t& चालक= (स्थिरांक hb_array_t&) = पूर्वनिर्धारित;
  hb_array_t& चालक= (hb_array_t&&) = पूर्वनिर्धारित;

  constexpr hb_array_t (Type *array_, अचिन्हित पूर्णांक length_) : arrayZ (array_), length (length_) {}
  ढाँचा <अचिन्हित पूर्णांक length_>
  constexpr hb_array_t (Type (&array_)[length_]) : hb_array_t (array_, length_) {}

  ढाँचा <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_array_t (स्थिरांक hb_array_t<U> &o) :
    hb_iter_with_fallback_t<hb_array_t, Type&> (),
    arrayZ (o.arrayZ), length (o.length), backwards_length (o.backwards_length) {}
  ढाँचा <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_array_t& चालक = (स्थिरांक hb_array_t<U> &o)
  { arrayZ = o.arrayZ; length = o.length; backwards_length = o.backwards_length; परत *हे; }

  /*
   * Iterator implementation.
   */
  प्रकारव्याख्या Type& __item_t__;
  स्थिर constexpr तार्किक is_random_access_iterator = सत्य;
  स्थिर constexpr तार्किक has_fast_len = सत्य;
  Type& __item__ () स्थिरांक
  {
    जर (unlikely (!length)) परत CrapOrNull (Type);
    परत *arrayZ;
  }
  Type& __item_at__ (अचिन्हित i) स्थिरांक
  {
    जर (unlikely (i >= length)) परत CrapOrNull (Type);
    परत arrayZ[i];
  }
  शून्य __next__ ()
  {
    जर (unlikely (!length))
      परत;
    length--;
    backwards_length++;
    arrayZ++;
  }
  शून्य __forward__ (अचिन्हित n)
  {
    जर (unlikely (n > length))
      n = length;
    length -= n;
    backwards_length += n;
    arrayZ += n;
  }
  शून्य __prev__ ()
  {
    जर (unlikely (!backwards_length))
      परत;
    length++;
    backwards_length--;
    arrayZ--;
  }
  शून्य __rewind__ (अचिन्हित n)
  {
    जर (unlikely (n > backwards_length))
      n = backwards_length;
    length += n;
    backwards_length -= n;
    arrayZ -= n;
  }
  अचिन्हित __len__ () स्थिरांक { परत length; }
  /* Ouch. The operator== compares the contents of the array.  For range-based for loops,
   * it's best if we can just compare arrayZ, though comparing contents is still fast,
   * but also would require that Type has operator==.  As such, we optimize this operator
   * for range-based for loop and just compare arrayZ and length.
   *
   * The above comment is outdated now because we implemented separate begin/end to
   * objects that were using hb_array_t for range-based loop before. */
  तार्किक चालक != (स्थिरांक hb_array_t& o) स्थिरांक
  { परत हे->arrayZ != o.arrayZ || हे->length != o.length; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () स्थिरांक { परत arrayZ; }
  Type *end () स्थिरांक { परत arrayZ + length; }


  /* Extra operators.
   */
  Type * चालक & () स्थिरांक { परत arrayZ; }
  चालक hb_array_t<स्थिरांक Type> () { परत hb_array_t<स्थिरांक Type> (arrayZ, length); }
  ढाँचा <typename T> चालक T * () स्थिरांक { परत arrayZ; }

  HB_INTERNAL तार्किक चालक == (स्थिरांक hb_array_t &o) स्थिरांक;

  uint32_t hash () स्थिरांक
  {
    // FNV-1a hash function
    // https://github.com/harfbuzz/harfbuzz/pull/4228
    uint32_t current = /*cbf29ce4*/0x84222325;
    साठी (स्वतः &v : *हे)
    {
      current = current ^ hb_hash (v);
      current = current * 16777619;
    }
    परत current;
  }

  /*
   * Compare, Sort, and Search.
   */

  /* Note: our compare is NOT lexicographic; it also does NOT call Type::cmp. */
  पूर्णांक cmp (स्थिरांक hb_array_t &a) स्थिरांक
  {
    जर (length != a.length)
      परत (पूर्णांक) a.length - (पूर्णांक) length;
    परत hb_memcmp (a.arrayZ, arrayZ, get_size ());
  }
  HB_INTERNAL स्थिर पूर्णांक cmp (स्थिरांक शून्य *pa, स्थिरांक शून्य *pb)
  {
    hb_array_t *a = (hb_array_t *) pa;
    hb_array_t *b = (hb_array_t *) pb;
    परत b->cmp (*a);
  }

  ढाँचा <typename T>
  Type *lsearch (स्थिरांक T &x, Type *not_found = nullptr)
  {
    अचिन्हित i;
    परत lfind (x, &i) ? &हे->arrayZ[i] : not_found;
  }
  ढाँचा <typename T>
  स्थिरांक Type *lsearch (स्थिरांक T &x, स्थिरांक Type *not_found = nullptr) स्थिरांक
  {
    अचिन्हित i;
    परत lfind (x, &i) ? &हे->arrayZ[i] : not_found;
  }
  ढाँचा <typename T>
  तार्किक lfind (स्थिरांक T &x, अचिन्हित *pos = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      अचिन्हित पूर्णांक to_store = (अचिन्हित पूर्णांक) -1) स्थिरांक
  {
    साठी (अचिन्हित i = 0; i < length; ++i)
      जर (hb_equal (x, हे->arrayZ[i]))
      {
	जर (pos)
	  *pos = i;
	परत सत्य;
      }

    जर (pos)
    {
      निवड (not_found)
      {
	स्थिती HB_NOT_FOUND_DONT_STORE:
	  थांबा;

	स्थिती HB_NOT_FOUND_STORE:
	  *pos = to_store;
	  थांबा;

	स्थिती HB_NOT_FOUND_STORE_CLOSEST:
	  *pos = length;
	  थांबा;
      }
    }
    परत असत्य;
  }

  hb_sorted_array_t<Type> क्विक (पूर्णांक (*cmp)(स्थिरांक शून्य*, स्थिरांक शून्य*))
  {
    जर (likely (length))
      hb_qsort (arrayZ, length, हे->get_item_size (), cmp);
    परत hb_sorted_array_t<Type> (*हे);
  }

  /* Comparator follows the C qsort convention: returns
   * negative / zero / positive int. */
  ढाँचा <typename Compar>
  hb_sorted_array_t<Type> क्विक (Compar compar)
  {
    जर (likely (length))
      hb_qsort_inline (arrayZ, length, compar);
    परत hb_sorted_array_t<Type> (*हे);
  }

  खाजगी:
  ढाँचा <typename T = Type,
	    hb_enable_if (std::is_move_assignable<T>::value)>
  hb_sorted_array_t<Type> _qsort (hb_priority<1>)
  {
    परत क्विक ([] (स्थिरांक Type &a, स्थिरांक Type &b) { परत Type::cmp (&a, &b); });
  }
  hb_sorted_array_t<Type> _qsort (hb_priority<0>)
  {
    परत क्विक ((पूर्णांक(*)(स्थिरांक शून्य*, स्थिरांक शून्य*)) Type::cmp);
  }
  सार्वजनिक:

  hb_sorted_array_t<Type> क्विक ()
  {
    परत _qsort (hb_prioritize);
  }

  /*
   * Other methods.
   */

  माप_प्रकार get_size () स्थिरांक { परत length * हे->get_item_size (); }

  /*
   * Reverse the order of items in this array in the range [start, end).
   */
  शून्य reverse (अचिन्हित start = 0, अचिन्हित end = -1)
  {
    start = hb_min (start, length);
    end = hb_min (end, length);

    जर (end < start + 2)
      परत;

    अचिन्हित stop = start + (end - start) / 2;
    साठी (अचिन्हित lhs = start, rhs = end - 1; lhs < stop; lhs++, rhs--)
      hb_swap (arrayZ[rhs], arrayZ[lhs]);
  }

  hb_array_t sub_array (अचिन्हित पूर्णांक start_offset = 0, अचिन्हित पूर्णांक *seg_मोजणी = nullptr /* IN/OUT */) स्थिरांक
  {
    जर (!start_offset && !seg_मोजणी)
      परत *हे;

    अचिन्हित पूर्णांक मोजणी = length;
    जर (unlikely (start_offset > मोजणी))
      मोजणी = 0;
    अन्यथा
      मोजणी -= start_offset;
    जर (seg_मोजणी)
      मोजणी = *seg_मोजणी = hb_min (मोजणी, *seg_मोजणी);
    परत hb_array_t (arrayZ + start_offset, मोजणी);
  }
  hb_array_t sub_array (अचिन्हित पूर्णांक start_offset, अचिन्हित पूर्णांक seg_मोजणी) स्थिरांक
  { परत sub_array (start_offset, &seg_मोजणी); }

  hb_array_t truncate (अचिन्हित length) स्थिरांक { परत sub_array (0, length); }

  ढाँचा <typename T,
	    अचिन्हित P = आकार (Type),
	    hb_enable_if (P == 1)>
  स्थिरांक T *as () स्थिरांक
  { परत length < hb_min_size (T) ? &Null (T) : reinterpret_cast<स्थिरांक T *> (arrayZ); }

  ढाँचा <typename T,
	    अचिन्हित P = आकार (Type),
	    hb_enable_if (P == 1)>
  तार्किक check_range (स्थिरांक T *p, अचिन्हित पूर्णांक size = T::static_size) स्थिरांक
  {
    परत arrayZ <= ((स्थिरांक अक्षर *) p)
	&& ((स्थिरांक अक्षर *) p) <= arrayZ + length
	&& (अचिन्हित पूर्णांक) (arrayZ + length - (स्थिरांक अक्षर *) p) >= size;
  }

  ढाँचा <अचिन्हित P = आकार (Type),
	    hb_enable_if (P == 1)>
  तार्किक check_end (स्थिरांक शून्य *p) स्थिरांक
  {
    परत (uintptr_t) (((स्थिरांक अक्षर *) p) - arrayZ) <= length;
  }

  /* Only call if you allocated the underlying array using hb_malloc() or similar. */
  शून्य fini ()
  { hb_free ((शून्य *) arrayZ); arrayZ = nullptr; length = 0; }

  ढाँचा <typename hb_serialize_context_t,
	    typename U = Type,
	    hb_enable_if (!(आकार (U) < आकार (दीर्घ दीर्घ) && hb_is_trivially_copy_assignable(hb_decay<Type>)))>
  hb_array_t copy (hb_serialize_context_t *c) स्थिरांक
  {
    TRACE_SERIALIZE (हे);
    स्वतः* out = c->start_embed (arrayZ);
    जर (unlikely (!c->extend_size (out, get_size (), असत्य))) return_trace (hb_array_t ());
    साठी (अचिन्हित i = 0; i < length; i++)
      out[i] = arrayZ[i]; /* TODO: add version that calls c->copy() */
    return_trace (hb_array_t (out, length));
  }

  ढाँचा <typename hb_serialize_context_t,
	    typename U = Type,
	    hb_enable_if (आकार (U) < आकार (दीर्घ दीर्घ) && hb_is_trivially_copy_assignable(hb_decay<Type>))>
  hb_array_t copy (hb_serialize_context_t *c) स्थिरांक
  {
    TRACE_SERIALIZE (हे);
    स्वतः* out = c->start_embed (arrayZ);
    जर (unlikely (!c->extend_size (out, get_size (), असत्य))) return_trace (hb_array_t ());
    hb_memcpy (out, arrayZ, get_size ());
    return_trace (hb_array_t (out, length));
  }

  ढाँचा <typename hb_sanitize_context_t>
  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  { परत c->check_array (arrayZ, length); }

  /*
   * Members
   */

  सार्वजनिक:
  Type *arrayZ = nullptr;
  अचिन्हित पूर्णांक length = 0;
  अचिन्हित पूर्णांक backwards_length = 0;
};
ढाँचा <typename T> ओळीत hb_array_t<T>
hb_array ()
{ परत hb_array_t<T> (); }
ढाँचा <typename T> ओळीत hb_array_t<T>
hb_array (T *array, अचिन्हित पूर्णांक length)
{ परत hb_array_t<T> (array, length); }
ढाँचा <typename T, अचिन्हित पूर्णांक length_> ओळीत hb_array_t<T>
hb_array (T (&array_)[length_])
{ परत hb_array_t<T> (array_); }

ढाँचा <typename Type>
संरचना hb_sorted_array_t :
	hb_array_t<Type>,
	hb_iter_t<hb_sorted_array_t<Type>, Type&>
{
  प्रकारव्याख्या hb_iter_t<hb_sorted_array_t, Type&> iter_base_t;
  HB_ITER_USING (iter_base_t);
  स्थिर constexpr तार्किक is_random_access_iterator = सत्य;
  स्थिर constexpr तार्किक is_sorted_iterator = सत्य;
  स्थिर constexpr तार्किक has_fast_len = सत्य;

  hb_sorted_array_t () = पूर्वनिर्धारित;
  hb_sorted_array_t (स्थिरांक hb_sorted_array_t&) = पूर्वनिर्धारित;
  ~hb_sorted_array_t () = पूर्वनिर्धारित;
  hb_sorted_array_t& चालक= (स्थिरांक hb_sorted_array_t&) = पूर्वनिर्धारित;
  hb_sorted_array_t& चालक= (hb_sorted_array_t&&) = पूर्वनिर्धारित;

  constexpr hb_sorted_array_t (Type *array_, अचिन्हित पूर्णांक length_) : hb_array_t<Type> (array_, length_) {}
  ढाँचा <अचिन्हित पूर्णांक length_>
  constexpr hb_sorted_array_t (Type (&array_)[length_]) : hb_array_t<Type> (array_) {}

  ढाँचा <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_sorted_array_t (स्थिरांक hb_array_t<U> &o) :
    hb_array_t<Type> (o),
    hb_iter_t<hb_sorted_array_t, Type&> () {}
  ढाँचा <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_sorted_array_t& चालक = (स्थिरांक hb_array_t<U> &o)
  { hb_array_t<Type> (*हे) = o; परत *हे; }

  /* Iterator implementation. */

  /* See comment in hb_array_of::operator != */
  तार्किक चालक != (स्थिरांक hb_sorted_array_t& o) स्थिरांक
  { परत हे->arrayZ != o.arrayZ || हे->length != o.length; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () स्थिरांक { परत हे->arrayZ; }
  Type *end () स्थिरांक { परत हे->arrayZ + हे->length; }


  hb_sorted_array_t sub_array (अचिन्हित पूर्णांक start_offset, अचिन्हित पूर्णांक *seg_मोजणी /* IN/OUT */) स्थिरांक
  { परत hb_sorted_array_t (((स्थिरांक hb_array_t<Type> *) (हे))->sub_array (start_offset, seg_मोजणी)); }
  hb_sorted_array_t sub_array (अचिन्हित पूर्णांक start_offset, अचिन्हित पूर्णांक seg_मोजणी) स्थिरांक
  { परत sub_array (start_offset, &seg_मोजणी); }

  hb_sorted_array_t truncate (अचिन्हित length) स्थिरांक { परत sub_array (0, length); }

  ढाँचा <typename T>
  Type *द्वा_खोज (स्थिरांक T &x, Type *not_found = nullptr)
  {
    अचिन्हित पूर्णांक i;
    परत bfind (x, &i) ? &हे->arrayZ[i] : not_found;
  }
  ढाँचा <typename T>
  स्थिरांक Type *द्वा_खोज (स्थिरांक T &x, स्थिरांक Type *not_found = nullptr) स्थिरांक
  {
    अचिन्हित पूर्णांक i;
    परत bfind (x, &i) ? &हे->arrayZ[i] : not_found;
  }
  ढाँचा <typename T>
  तार्किक bfind (स्थिरांक T &x, अचिन्हित पूर्णांक *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      अचिन्हित पूर्णांक to_store = (अचिन्हित पूर्णांक) -1) स्थिरांक
  {
    अचिन्हित pos;

    जर (bsearch_impl (x, &pos))
    {
      जर (i)
	*i = pos;
      परत सत्य;
    }

    जर (i)
    {
      निवड (not_found)
      {
	स्थिती HB_NOT_FOUND_DONT_STORE:
	  थांबा;

	स्थिती HB_NOT_FOUND_STORE:
	  *i = to_store;
	  थांबा;

	स्थिती HB_NOT_FOUND_STORE_CLOSEST:
	  *i = pos;
	  थांबा;
      }
    }
    परत असत्य;
  }
  ढाँचा <typename T, typename ...Ts>
  तार्किक bsearch_impl (स्थिरांक T &x, अचिन्हित *pos, Ts... ds) स्थिरांक
  {
    परत hb_bsearch_impl (pos,
			    x,
			    हे->arrayZ,
			    हे->length,
			    आकार (Type),
			    _hb_cmp_method<T, Type, Ts...>,
			    std::forward<Ts> (ds)...);
  }
};
ढाँचा <typename T> ओळीत hb_sorted_array_t<T>
hb_sorted_array (T *array, अचिन्हित पूर्णांक length)
{ परत hb_sorted_array_t<T> (array, length); }
ढाँचा <typename T, अचिन्हित पूर्णांक length_> ओळीत hb_sorted_array_t<T>
hb_sorted_array (T (&array_)[length_])
{ परत hb_sorted_array_t<T> (array_); }

ढाँचा <typename T>
ओळीत तार्किक hb_array_t<T>::चालक == (स्थिरांक hb_array_t<T> &o) स्थिरांक
{
  जर (o.length != हे->length) परत असत्य;
  साठी (अचिन्हित पूर्णांक i = 0; i < हे->length; i++) {
    जर (हे->arrayZ[i] != o.arrayZ[i]) परत असत्य;
  }
  परत सत्य;
}
ढाँचा <>
ओळीत तार्किक hb_array_t<स्थिरांक अक्षर>::चालक == (स्थिरांक hb_array_t<स्थिरांक अक्षर> &o) स्थिरांक
{
  जर (o.length != हे->length) परत असत्य;
  परत 0 == hb_memcmp (arrayZ, o.arrayZ, length);
}
ढाँचा <>
ओळीत तार्किक hb_array_t<स्थिरांक अचिन्हित अक्षर>::चालक == (स्थिरांक hb_array_t<स्थिरांक अचिन्हित अक्षर> &o) स्थिरांक
{
  जर (o.length != हे->length) परत असत्य;
  परत 0 == hb_memcmp (arrayZ, o.arrayZ, length);
}


/* Specialize hash() for byte arrays. */

#ifndef HB_OPTIMIZE_SIZE_MORE
ढाँचा <>
ओळीत uint32_t hb_array_t<स्थिरांक अक्षर>::hash () स्थिरांक
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  परत fasthash32(arrayZ, length, 0xf437ffe6 /* magic? */);
}

ढाँचा <>
ओळीत uint32_t hb_array_t<स्थिरांक अचिन्हित अक्षर>::hash () स्थिरांक
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  परत fasthash32(arrayZ, length, 0xf437ffe6 /* magic? */);
}
#endif


प्रकारव्याख्या hb_array_t<स्थिरांक अक्षर> hb_bytes_t;
प्रकारव्याख्या hb_array_t<स्थिरांक अचिन्हित अक्षर> hb_ubytes_t;



#endif /* HB_ARRAY_HH */

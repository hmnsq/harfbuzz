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
  hb_array_t () = स्वभाव;
  hb_array_t (अचर hb_array_t&) = स्वभाव;
  ~hb_array_t () = स्वभाव;
  hb_array_t& चालक= (अचर hb_array_t&) = स्वभाव;
  hb_array_t& चालक= (hb_array_t&&) = स्वभाव;

  constexpr hb_array_t (Type *array_, अचिन्हित पूर्णाङ्क length_) : arrayZ (array_), length (length_) {}
  ढाँचा <अचिन्हित पूर्णाङ्क length_>
  constexpr hb_array_t (Type (&array_)[length_]) : hb_array_t (array_, length_) {}

  ढाँचा <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_array_t (अचर hb_array_t<U> &o) :
    hb_iter_with_fallback_t<hb_array_t, Type&> (),
    arrayZ (o.arrayZ), length (o.length), backwards_length (o.backwards_length) {}
  ढाँचा <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_array_t& चालक = (अचर hb_array_t<U> &o)
  { arrayZ = o.arrayZ; length = o.length; backwards_length = o.backwards_length; फर्काऊ *एतत्; }

  /*
   * Iterator implementation.
   */
  प्रकारनिर्देश Type& __item_t__;
  स्थिर constexpr तार्किक is_random_access_iterator = सत्य;
  स्थिर constexpr तार्किक has_fast_len = सत्य;
  Type& __item__ () अचर
  {
    यदि (unlikely (!length)) फर्काऊ CrapOrNull (Type);
    फर्काऊ *arrayZ;
  }
  Type& __item_at__ (अचिन्हित i) अचर
  {
    यदि (unlikely (i >= length)) फर्काऊ CrapOrNull (Type);
    फर्काऊ arrayZ[i];
  }
  शून्य __next__ ()
  {
    यदि (unlikely (!length))
      फर्काऊ;
    length--;
    backwards_length++;
    arrayZ++;
  }
  शून्य __forward__ (अचिन्हित n)
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
      फर्काऊ;
    length++;
    backwards_length--;
    arrayZ--;
  }
  शून्य __rewind__ (अचिन्हित n)
  {
    यदि (unlikely (n > backwards_length))
      n = backwards_length;
    length += n;
    backwards_length -= n;
    arrayZ -= n;
  }
  अचिन्हित __len__ () अचर { फर्काऊ length; }
  /* Ouch. The operator== compares the contents of the array.  For range-based for loops,
   * it's best if we can just compare arrayZ, though comparing contents is still fast,
   * but also would require that Type has operator==.  As such, we optimize this operator
   * for range-based for loop and just compare arrayZ and length.
   *
   * The above comment is outdated now because we implemented separate begin/end to
   * objects that were using hb_array_t for range-based loop before. */
  तार्किक चालक != (अचर hb_array_t& o) अचर
  { फर्काऊ एतत्->arrayZ != o.arrayZ || एतत्->length != o.length; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () अचर { फर्काऊ arrayZ; }
  Type *end () अचर { फर्काऊ arrayZ + length; }


  /* Extra operators.
   */
  Type * चालक & () अचर { फर्काऊ arrayZ; }
  चालक hb_array_t<अचर Type> () { फर्काऊ hb_array_t<अचर Type> (arrayZ, length); }
  ढाँचा <typename T> चालक T * () अचर { फर्काऊ arrayZ; }

  HB_INTERNAL तार्किक चालक == (अचर hb_array_t &o) अचर;

  uint32_t hash () अचर
  {
    // FNV-1a hash function
    // https://github.com/harfbuzz/harfbuzz/pull/4228
    uint32_t current = /*cbf29ce4*/0x84222325;
    लागि (स्वतः &v : *एतत्)
    {
      current = current ^ hb_hash (v);
      current = current * 16777619;
    }
    फर्काऊ current;
  }

  /*
   * Compare, Sort, and Search.
   */

  /* Note: our compare is NOT lexicographic; it also does NOT call Type::cmp. */
  पूर्णाङ्क cmp (अचर hb_array_t &a) अचर
  {
    यदि (length != a.length)
      फर्काऊ (पूर्णाङ्क) a.length - (पूर्णाङ्क) length;
    फर्काऊ hb_memcmp (a.arrayZ, arrayZ, get_size ());
  }
  HB_INTERNAL स्थिर पूर्णाङ्क cmp (अचर शून्य *pa, अचर शून्य *pb)
  {
    hb_array_t *a = (hb_array_t *) pa;
    hb_array_t *b = (hb_array_t *) pb;
    फर्काऊ b->cmp (*a);
  }

  ढाँचा <typename T>
  Type *lsearch (अचर T &x, Type *not_found = nullptr)
  {
    अचिन्हित i;
    फर्काऊ lfind (x, &i) ? &एतत्->arrayZ[i] : not_found;
  }
  ढाँचा <typename T>
  अचर Type *lsearch (अचर T &x, अचर Type *not_found = nullptr) अचर
  {
    अचिन्हित i;
    फर्काऊ lfind (x, &i) ? &एतत्->arrayZ[i] : not_found;
  }
  ढाँचा <typename T>
  तार्किक lfind (अचर T &x, अचिन्हित *pos = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      अचिन्हित पूर्णाङ्क to_store = (अचिन्हित पूर्णाङ्क) -1) अचर
  {
    लागि (अचिन्हित i = 0; i < length; ++i)
      यदि (hb_equal (x, एतत्->arrayZ[i]))
      {
	यदि (pos)
	  *pos = i;
	फर्काऊ सत्य;
      }

    यदि (pos)
    {
      विकल्प (not_found)
      {
	पक्ष HB_NOT_FOUND_DONT_STORE:
	  रोक;

	पक्ष HB_NOT_FOUND_STORE:
	  *pos = to_store;
	  रोक;

	पक्ष HB_NOT_FOUND_STORE_CLOSEST:
	  *pos = length;
	  रोक;
      }
    }
    फर्काऊ असत्य;
  }

  hb_sorted_array_t<Type> क्विक (पूर्णाङ्क (*cmp)(अचर शून्य*, अचर शून्य*))
  {
    यदि (likely (length))
      hb_qsort (arrayZ, length, एतत्->get_item_size (), cmp);
    फर्काऊ hb_sorted_array_t<Type> (*एतत्);
  }

  /* Comparator follows the C qsort convention: returns
   * negative / zero / positive int. */
  ढाँचा <typename Compar>
  hb_sorted_array_t<Type> क्विक (Compar compar)
  {
    यदि (likely (length))
      hb_qsort_inline (arrayZ, length, compar);
    फर्काऊ hb_sorted_array_t<Type> (*एतत्);
  }

  गुप्त:
  ढाँचा <typename T = Type,
	    hb_enable_if (std::is_move_assignable<T>::value)>
  hb_sorted_array_t<Type> _qsort (hb_priority<1>)
  {
    फर्काऊ क्विक ([] (अचर Type &a, अचर Type &b) { फर्काऊ Type::cmp (&a, &b); });
  }
  hb_sorted_array_t<Type> _qsort (hb_priority<0>)
  {
    फर्काऊ क्विक ((पूर्णाङ्क(*)(अचर शून्य*, अचर शून्य*)) Type::cmp);
  }
  सार्वजनिक:

  hb_sorted_array_t<Type> क्विक ()
  {
    फर्काऊ _qsort (hb_prioritize);
  }

  /*
   * Other methods.
   */

  माप_प्रकार get_size () अचर { फर्काऊ length * एतत्->get_item_size (); }

  /*
   * Reverse the order of items in this array in the range [start, end).
   */
  शून्य reverse (अचिन्हित start = 0, अचिन्हित end = -1)
  {
    start = hb_min (start, length);
    end = hb_min (end, length);

    यदि (end < start + 2)
      फर्काऊ;

    अचिन्हित stop = start + (end - start) / 2;
    लागि (अचिन्हित lhs = start, rhs = end - 1; lhs < stop; lhs++, rhs--)
      hb_swap (arrayZ[rhs], arrayZ[lhs]);
  }

  hb_array_t sub_array (अचिन्हित पूर्णाङ्क start_offset = 0, अचिन्हित पूर्णाङ्क *seg_गन्ती = nullptr /* IN/OUT */) अचर
  {
    यदि (!start_offset && !seg_गन्ती)
      फर्काऊ *एतत्;

    अचिन्हित पूर्णाङ्क गन्ती = length;
    यदि (unlikely (start_offset > गन्ती))
      गन्ती = 0;
    अन्यथा
      गन्ती -= start_offset;
    यदि (seg_गन्ती)
      गन्ती = *seg_गन्ती = hb_min (गन्ती, *seg_गन्ती);
    फर्काऊ hb_array_t (arrayZ + start_offset, गन्ती);
  }
  hb_array_t sub_array (अचिन्हित पूर्णाङ्क start_offset, अचिन्हित पूर्णाङ्क seg_गन्ती) अचर
  { फर्काऊ sub_array (start_offset, &seg_गन्ती); }

  hb_array_t truncate (अचिन्हित length) अचर { फर्काऊ sub_array (0, length); }

  ढाँचा <typename T,
	    अचिन्हित P = परिमाण (Type),
	    hb_enable_if (P == 1)>
  अचर T *as () अचर
  { फर्काऊ length < hb_min_size (T) ? &Null (T) : reinterpret_cast<अचर T *> (arrayZ); }

  ढाँचा <typename T,
	    अचिन्हित P = परिमाण (Type),
	    hb_enable_if (P == 1)>
  तार्किक check_range (अचर T *p, अचिन्हित पूर्णाङ्क size = T::static_size) अचर
  {
    फर्काऊ arrayZ <= ((अचर वर्ण *) p)
	&& ((अचर वर्ण *) p) <= arrayZ + length
	&& (अचिन्हित पूर्णाङ्क) (arrayZ + length - (अचर वर्ण *) p) >= size;
  }

  ढाँचा <अचिन्हित P = परिमाण (Type),
	    hb_enable_if (P == 1)>
  तार्किक check_end (अचर शून्य *p) अचर
  {
    फर्काऊ (uintptr_t) (((अचर वर्ण *) p) - arrayZ) <= length;
  }

  /* Only call if you allocated the underlying array using hb_malloc() or similar. */
  शून्य fini ()
  { hb_free ((शून्य *) arrayZ); arrayZ = nullptr; length = 0; }

  ढाँचा <typename hb_serialize_context_t,
	    typename U = Type,
	    hb_enable_if (!(परिमाण (U) < परिमाण (दीर्घ दीर्घ) && hb_is_trivially_copy_assignable(hb_decay<Type>)))>
  hb_array_t copy (hb_serialize_context_t *c) अचर
  {
    TRACE_SERIALIZE (एतत्);
    स्वतः* out = c->start_embed (arrayZ);
    यदि (unlikely (!c->extend_size (out, get_size (), असत्य))) return_trace (hb_array_t ());
    लागि (अचिन्हित i = 0; i < length; i++)
      out[i] = arrayZ[i]; /* TODO: add version that calls c->copy() */
    return_trace (hb_array_t (out, length));
  }

  ढाँचा <typename hb_serialize_context_t,
	    typename U = Type,
	    hb_enable_if (परिमाण (U) < परिमाण (दीर्घ दीर्घ) && hb_is_trivially_copy_assignable(hb_decay<Type>))>
  hb_array_t copy (hb_serialize_context_t *c) अचर
  {
    TRACE_SERIALIZE (एतत्);
    स्वतः* out = c->start_embed (arrayZ);
    यदि (unlikely (!c->extend_size (out, get_size (), असत्य))) return_trace (hb_array_t ());
    hb_memcpy (out, arrayZ, get_size ());
    return_trace (hb_array_t (out, length));
  }

  ढाँचा <typename hb_sanitize_context_t>
  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  { फर्काऊ c->check_array (arrayZ, length); }

  /*
   * Members
   */

  सार्वजनिक:
  Type *arrayZ = nullptr;
  अचिन्हित पूर्णाङ्क length = 0;
  अचिन्हित पूर्णाङ्क backwards_length = 0;
};
ढाँचा <typename T> अन्तःस्थ hb_array_t<T>
hb_array ()
{ फर्काऊ hb_array_t<T> (); }
ढाँचा <typename T> अन्तःस्थ hb_array_t<T>
hb_array (T *array, अचिन्हित पूर्णाङ्क length)
{ फर्काऊ hb_array_t<T> (array, length); }
ढाँचा <typename T, अचिन्हित पूर्णाङ्क length_> अन्तःस्थ hb_array_t<T>
hb_array (T (&array_)[length_])
{ फर्काऊ hb_array_t<T> (array_); }

ढाँचा <typename Type>
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
  hb_sorted_array_t (अचर hb_sorted_array_t&) = स्वभाव;
  ~hb_sorted_array_t () = स्वभाव;
  hb_sorted_array_t& चालक= (अचर hb_sorted_array_t&) = स्वभाव;
  hb_sorted_array_t& चालक= (hb_sorted_array_t&&) = स्वभाव;

  constexpr hb_sorted_array_t (Type *array_, अचिन्हित पूर्णाङ्क length_) : hb_array_t<Type> (array_, length_) {}
  ढाँचा <अचिन्हित पूर्णाङ्क length_>
  constexpr hb_sorted_array_t (Type (&array_)[length_]) : hb_array_t<Type> (array_) {}

  ढाँचा <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_sorted_array_t (अचर hb_array_t<U> &o) :
    hb_array_t<Type> (o),
    hb_iter_t<hb_sorted_array_t, Type&> () {}
  ढाँचा <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_sorted_array_t& चालक = (अचर hb_array_t<U> &o)
  { hb_array_t<Type> (*एतत्) = o; फर्काऊ *एतत्; }

  /* Iterator implementation. */

  /* See comment in hb_array_of::operator != */
  तार्किक चालक != (अचर hb_sorted_array_t& o) अचर
  { फर्काऊ एतत्->arrayZ != o.arrayZ || एतत्->length != o.length; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () अचर { फर्काऊ एतत्->arrayZ; }
  Type *end () अचर { फर्काऊ एतत्->arrayZ + एतत्->length; }


  hb_sorted_array_t sub_array (अचिन्हित पूर्णाङ्क start_offset, अचिन्हित पूर्णाङ्क *seg_गन्ती /* IN/OUT */) अचर
  { फर्काऊ hb_sorted_array_t (((अचर hb_array_t<Type> *) (एतत्))->sub_array (start_offset, seg_गन्ती)); }
  hb_sorted_array_t sub_array (अचिन्हित पूर्णाङ्क start_offset, अचिन्हित पूर्णाङ्क seg_गन्ती) अचर
  { फर्काऊ sub_array (start_offset, &seg_गन्ती); }

  hb_sorted_array_t truncate (अचिन्हित length) अचर { फर्काऊ sub_array (0, length); }

  ढाँचा <typename T>
  Type *द्वा_खोज (अचर T &x, Type *not_found = nullptr)
  {
    अचिन्हित पूर्णाङ्क i;
    फर्काऊ bfind (x, &i) ? &एतत्->arrayZ[i] : not_found;
  }
  ढाँचा <typename T>
  अचर Type *द्वा_खोज (अचर T &x, अचर Type *not_found = nullptr) अचर
  {
    अचिन्हित पूर्णाङ्क i;
    फर्काऊ bfind (x, &i) ? &एतत्->arrayZ[i] : not_found;
  }
  ढाँचा <typename T>
  तार्किक bfind (अचर T &x, अचिन्हित पूर्णाङ्क *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      अचिन्हित पूर्णाङ्क to_store = (अचिन्हित पूर्णाङ्क) -1) अचर
  {
    अचिन्हित pos;

    यदि (bsearch_impl (x, &pos))
    {
      यदि (i)
	*i = pos;
      फर्काऊ सत्य;
    }

    यदि (i)
    {
      विकल्प (not_found)
      {
	पक्ष HB_NOT_FOUND_DONT_STORE:
	  रोक;

	पक्ष HB_NOT_FOUND_STORE:
	  *i = to_store;
	  रोक;

	पक्ष HB_NOT_FOUND_STORE_CLOSEST:
	  *i = pos;
	  रोक;
      }
    }
    फर्काऊ असत्य;
  }
  ढाँचा <typename T, typename ...Ts>
  तार्किक bsearch_impl (अचर T &x, अचिन्हित *pos, Ts... ds) अचर
  {
    फर्काऊ hb_bsearch_impl (pos,
			    x,
			    एतत्->arrayZ,
			    एतत्->length,
			    परिमाण (Type),
			    _hb_cmp_method<T, Type, Ts...>,
			    std::forward<Ts> (ds)...);
  }
};
ढाँचा <typename T> अन्तःस्थ hb_sorted_array_t<T>
hb_sorted_array (T *array, अचिन्हित पूर्णाङ्क length)
{ फर्काऊ hb_sorted_array_t<T> (array, length); }
ढाँचा <typename T, अचिन्हित पूर्णाङ्क length_> अन्तःस्थ hb_sorted_array_t<T>
hb_sorted_array (T (&array_)[length_])
{ फर्काऊ hb_sorted_array_t<T> (array_); }

ढाँचा <typename T>
अन्तःस्थ तार्किक hb_array_t<T>::चालक == (अचर hb_array_t<T> &o) अचर
{
  यदि (o.length != एतत्->length) फर्काऊ असत्य;
  लागि (अचिन्हित पूर्णाङ्क i = 0; i < एतत्->length; i++) {
    यदि (एतत्->arrayZ[i] != o.arrayZ[i]) फर्काऊ असत्य;
  }
  फर्काऊ सत्य;
}
ढाँचा <>
अन्तःस्थ तार्किक hb_array_t<अचर वर्ण>::चालक == (अचर hb_array_t<अचर वर्ण> &o) अचर
{
  यदि (o.length != एतत्->length) फर्काऊ असत्य;
  फर्काऊ 0 == hb_memcmp (arrayZ, o.arrayZ, length);
}
ढाँचा <>
अन्तःस्थ तार्किक hb_array_t<अचर अचिन्हित वर्ण>::चालक == (अचर hb_array_t<अचर अचिन्हित वर्ण> &o) अचर
{
  यदि (o.length != एतत्->length) फर्काऊ असत्य;
  फर्काऊ 0 == hb_memcmp (arrayZ, o.arrayZ, length);
}


/* Specialize hash() for byte arrays. */

#ifndef HB_OPTIMIZE_SIZE_MORE
ढाँचा <>
अन्तःस्थ uint32_t hb_array_t<अचर वर्ण>::hash () अचर
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  फर्काऊ fasthash32(arrayZ, length, 0xf437ffe6 /* magic? */);
}

ढाँचा <>
अन्तःस्थ uint32_t hb_array_t<अचर अचिन्हित वर्ण>::hash () अचर
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  फर्काऊ fasthash32(arrayZ, length, 0xf437ffe6 /* magic? */);
}
#endif


प्रकारनिर्देश hb_array_t<अचर वर्ण> hb_bytes_t;
प्रकारनिर्देश hb_array_t<अचर अचिन्हित वर्ण> hb_ubytes_t;



#endif /* HB_ARRAY_HH */

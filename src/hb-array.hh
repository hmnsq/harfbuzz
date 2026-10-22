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

#अगर_अघोषित HB_ARRAY_HH
#घोषणा HB_ARRAY_HH

#समावेश "hb.hh"
#समावेश "hb-algs.hh"
#समावेश "hb-iter.hh"
#समावेश "hb-null.hh"


ढाँचा <typename Type>
काष्ठा hb_sorted_array_t;

क्रमागत hb_not_found_t
{
  HB_NOT_FOUND_DONT_STORE,
  HB_NOT_FOUND_STORE,
  HB_NOT_FOUND_STORE_CLOSEST,
};


ढाँचा <typename Type>
काष्ठा hb_array_t : hb_iter_with_fallback_t<hb_array_t<Type>, Type&>
{
  अटल constexpr bool realloc_move = true;

  /*
   * Constructors.
   */
  hb_array_t () = शेष;
  hb_array_t (स्थिर hb_array_t&) = शेष;
  ~hb_array_t () = शेष;
  hb_array_t& चालक= (स्थिर hb_array_t&) = शेष;
  hb_array_t& चालक= (hb_array_t&&) = शेष;

  constexpr hb_array_t (Type *array_, अचिन्हित पूर्णांक लंबाई_2) : arrayZ (array_), लंबाई (लंबाई_2) {}
  ढाँचा <अचिन्हित पूर्णांक लंबाई_2>
  constexpr hb_array_t (Type (&array_)[लंबाई_2]) : hb_array_t (array_, लंबाई_2) {}

  ढाँचा <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_array_t (स्थिर hb_array_t<U> &o) :
    hb_iter_with_fallback_t<hb_array_t, Type&> (),
    arrayZ (o.arrayZ), लंबाई (o.लंबाई), backwards_लंबाई (o.backwards_लंबाई) {}
  ढाँचा <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_array_t& चालक = (स्थिर hb_array_t<U> &o)
  { arrayZ = o.arrayZ; लंबाई = o.लंबाई; backwards_लंबाई = o.backwards_लंबाई; वापस *यह; }

  /*
   * Iterator implementation.
   */
  प्रकार Type& __item_t__;
  अटल constexpr bool is_random_access_iterator = true;
  अटल constexpr bool has_fast_len = true;
  Type& __item__ () स्थिर
  {
    अगर (unlikely (!लंबाई)) वापस CrapOrNull (Type);
    वापस *arrayZ;
  }
  Type& __item_at__ (अचिन्हित i) स्थिर
  {
    अगर (unlikely (i >= लंबाई)) वापस CrapOrNull (Type);
    वापस arrayZ[i];
  }
  व्योम __next__ ()
  {
    अगर (unlikely (!लंबाई))
      वापस;
    लंबाई--;
    backwards_लंबाई++;
    arrayZ++;
  }
  व्योम __forward__ (अचिन्हित n)
  {
    अगर (unlikely (n > लंबाई))
      n = लंबाई;
    लंबाई -= n;
    backwards_लंबाई += n;
    arrayZ += n;
  }
  व्योम __prev__ ()
  {
    अगर (unlikely (!backwards_लंबाई))
      वापस;
    लंबाई++;
    backwards_लंबाई--;
    arrayZ--;
  }
  व्योम __rewind__ (अचिन्हित n)
  {
    अगर (unlikely (n > backwards_लंबाई))
      n = backwards_लंबाई;
    लंबाई += n;
    backwards_लंबाई -= n;
    arrayZ -= n;
  }
  अचिन्हित __len__ () स्थिर { वापस लंबाई; }
  /* Ouch. The operator== compares the contents of the array.  For range-based for loops,
   * it's best if we can just compare arrayZ, though comparing contents is still fast,
   * but also would require that Type has operator==.  As such, we optimize this operator
   * for range-based for loop and just compare arrayZ and length.
   *
   * The above comment is outdated now because we implemented separate begin/end to
   * objects that were using hb_array_t for range-based loop before. */
  bool चालक != (स्थिर hb_array_t& o) स्थिर
  { वापस यह->arrayZ != o.arrayZ || यह->लंबाई != o.लंबाई; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () स्थिर { वापस arrayZ; }
  Type *end () स्थिर { वापस arrayZ + लंबाई; }


  /* Extra operators.
   */
  Type * चालक & () स्थिर { वापस arrayZ; }
  चालक hb_array_t<स्थिर Type> () { वापस hb_array_t<स्थिर Type> (arrayZ, लंबाई); }
  ढाँचा <typename T> चालक T * () स्थिर { वापस arrayZ; }

  HB_INTERNAL bool चालक == (स्थिर hb_array_t &o) स्थिर;

  uint32_t hash () स्थिर
  {
    // FNV-1a hash function
    // https://github.com/harfbuzz/harfbuzz/pull/4228
    uint32_t current = /*cbf29ce4*/0x84222325;
    क्रम (स्वतः &v : *यह)
    {
      current = current ^ hb_hash (v);
      current = current * 16777619;
    }
    वापस current;
  }

  /*
   * Compare, Sort, and Search.
   */

  /* Note: our compare is NOT lexicographic; it also does NOT call Type::cmp. */
  पूर्णांक cmp (स्थिर hb_array_t &a) स्थिर
  {
    अगर (लंबाई != a.लंबाई)
      वापस (पूर्णांक) a.लंबाई - (पूर्णांक) लंबाई;
    वापस hb_memcmp (a.arrayZ, arrayZ, get_आकार ());
  }
  HB_INTERNAL अटल पूर्णांक cmp (स्थिर व्योम *pa, स्थिर व्योम *pb)
  {
    hb_array_t *a = (hb_array_t *) pa;
    hb_array_t *b = (hb_array_t *) pb;
    वापस b->cmp (*a);
  }

  ढाँचा <typename T>
  Type *lsearch (स्थिर T &x, Type *not_found = nullptr)
  {
    अचिन्हित i;
    वापस lfind (x, &i) ? &यह->arrayZ[i] : not_found;
  }
  ढाँचा <typename T>
  स्थिर Type *lsearch (स्थिर T &x, स्थिर Type *not_found = nullptr) स्थिर
  {
    अचिन्हित i;
    वापस lfind (x, &i) ? &यह->arrayZ[i] : not_found;
  }
  ढाँचा <typename T>
  bool lfind (स्थिर T &x, अचिन्हित *pos = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      अचिन्हित पूर्णांक to_store = (अचिन्हित पूर्णांक) -1) स्थिर
  {
    क्रम (अचिन्हित i = 0; i < लंबाई; ++i)
      अगर (hb_equal (x, यह->arrayZ[i]))
      {
	अगर (pos)
	  *pos = i;
	वापस true;
      }

    अगर (pos)
    {
      चयन (not_found)
      {
	हाल HB_NOT_FOUND_DONT_STORE:
	  अवरोध;

	हाल HB_NOT_FOUND_STORE:
	  *pos = to_store;
	  अवरोध;

	हाल HB_NOT_FOUND_STORE_CLOSEST:
	  *pos = लंबाई;
	  अवरोध;
      }
    }
    वापस false;
  }

  hb_sorted_array_t<Type> क्विक (पूर्णांक (*cmp)(स्थिर व्योम*, स्थिर व्योम*))
  {
    अगर (likely (लंबाई))
      hb_qsort (arrayZ, लंबाई, यह->get_item_आकार (), cmp);
    वापस hb_sorted_array_t<Type> (*यह);
  }

  /* Comparator follows the C qsort convention: returns
   * negative / zero / positive int. */
  ढाँचा <typename Compar>
  hb_sorted_array_t<Type> क्विक (Compar compar)
  {
    अगर (likely (लंबाई))
      hb_qsort_inline (arrayZ, लंबाई, compar);
    वापस hb_sorted_array_t<Type> (*यह);
  }

  निजी:
  ढाँचा <typename T = Type,
	    hb_enable_if (std::is_move_assignable<T>::value)>
  hb_sorted_array_t<Type> _qsort (hb_priority<1>)
  {
    वापस क्विक ([] (स्थिर Type &a, स्थिर Type &b) { वापस Type::cmp (&a, &b); });
  }
  hb_sorted_array_t<Type> _qsort (hb_priority<0>)
  {
    वापस क्विक ((पूर्णांक(*)(स्थिर व्योम*, स्थिर व्योम*)) Type::cmp);
  }
  खुला:

  hb_sorted_array_t<Type> क्विक ()
  {
    वापस _qsort (hb_prioritize);
  }

  /*
   * Other methods.
   */

  माप_प्रकार get_आकार () स्थिर { वापस लंबाई * यह->get_item_आकार (); }

  /*
   * Reverse the order of items in this array in the range [start, end).
   */
  व्योम reverse (अचिन्हित start = 0, अचिन्हित end = -1)
  {
    start = hb_min (start, लंबाई);
    end = hb_min (end, लंबाई);

    अगर (end < start + 2)
      वापस;

    अचिन्हित stop = start + (end - start) / 2;
    क्रम (अचिन्हित lhs = start, rhs = end - 1; lhs < stop; lhs++, rhs--)
      hb_swap (arrayZ[rhs], arrayZ[lhs]);
  }

  hb_array_t sub_array (अचिन्हित पूर्णांक start_offset = 0, अचिन्हित पूर्णांक *seg_गिनती = nullptr /* IN/OUT */) स्थिर
  {
    अगर (!start_offset && !seg_गिनती)
      वापस *यह;

    अचिन्हित पूर्णांक गिनती = लंबाई;
    अगर (unlikely (start_offset > गिनती))
      गिनती = 0;
    अन्यथा
      गिनती -= start_offset;
    अगर (seg_गिनती)
      गिनती = *seg_गिनती = hb_min (गिनती, *seg_गिनती);
    वापस hb_array_t (arrayZ + start_offset, गिनती);
  }
  hb_array_t sub_array (अचिन्हित पूर्णांक start_offset, अचिन्हित पूर्णांक seg_गिनती) स्थिर
  { वापस sub_array (start_offset, &seg_गिनती); }

  hb_array_t truncate (अचिन्हित लंबाई) स्थिर { वापस sub_array (0, लंबाई); }

  ढाँचा <typename T,
	    अचिन्हित P = माप (Type),
	    hb_enable_if (P == 1)>
  स्थिर T *as () स्थिर
  { वापस लंबाई < hb_min_आकार (T) ? &Null (T) : reinterpret_cast<स्थिर T *> (arrayZ); }

  ढाँचा <typename T,
	    अचिन्हित P = माप (Type),
	    hb_enable_if (P == 1)>
  bool check_range (स्थिर T *p, अचिन्हित पूर्णांक आकार = T::static_आकार) स्थिर
  {
    वापस arrayZ <= ((स्थिर अक्षर *) p)
	&& ((स्थिर अक्षर *) p) <= arrayZ + लंबाई
	&& (अचिन्हित पूर्णांक) (arrayZ + लंबाई - (स्थिर अक्षर *) p) >= आकार;
  }

  ढाँचा <अचिन्हित P = माप (Type),
	    hb_enable_if (P == 1)>
  bool check_end (स्थिर व्योम *p) स्थिर
  {
    वापस (uintptr_t) (((स्थिर अक्षर *) p) - arrayZ) <= लंबाई;
  }

  /* Only call if you allocated the underlying array using hb_malloc() or similar. */
  व्योम fini ()
  { hb_free ((व्योम *) arrayZ); arrayZ = nullptr; लंबाई = 0; }

  ढाँचा <typename hb_serialize_context_t,
	    typename U = Type,
	    hb_enable_if (!(माप (U) < माप (दीर्घ दीर्घ) && hb_is_trivially_copy_assignable(hb_decay<Type>)))>
  hb_array_t copy (hb_serialize_context_t *c) स्थिर
  {
    TRACE_SERIALIZE (यह);
    स्वतः* out = c->start_embed (arrayZ);
    अगर (unlikely (!c->extend_आकार (out, get_आकार (), false))) return_trace (hb_array_t ());
    क्रम (अचिन्हित i = 0; i < लंबाई; i++)
      out[i] = arrayZ[i]; /* TODO: add version that calls c->copy() */
    return_trace (hb_array_t (out, लंबाई));
  }

  ढाँचा <typename hb_serialize_context_t,
	    typename U = Type,
	    hb_enable_if (माप (U) < माप (दीर्घ दीर्घ) && hb_is_trivially_copy_assignable(hb_decay<Type>))>
  hb_array_t copy (hb_serialize_context_t *c) स्थिर
  {
    TRACE_SERIALIZE (यह);
    स्वतः* out = c->start_embed (arrayZ);
    अगर (unlikely (!c->extend_आकार (out, get_आकार (), false))) return_trace (hb_array_t ());
    hb_memcpy (out, arrayZ, get_आकार ());
    return_trace (hb_array_t (out, लंबाई));
  }

  ढाँचा <typename hb_sanitize_context_t>
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  { वापस c->check_array (arrayZ, लंबाई); }

  /*
   * Members
   */

  खुला:
  Type *arrayZ = nullptr;
  अचिन्हित पूर्णांक लंबाई = 0;
  अचिन्हित पूर्णांक backwards_लंबाई = 0;
};
ढाँचा <typename T> अंतरभूत hb_array_t<T>
hb_array ()
{ वापस hb_array_t<T> (); }
ढाँचा <typename T> अंतरभूत hb_array_t<T>
hb_array (T *array, अचिन्हित पूर्णांक लंबाई)
{ वापस hb_array_t<T> (array, लंबाई); }
ढाँचा <typename T, अचिन्हित पूर्णांक लंबाई_2> अंतरभूत hb_array_t<T>
hb_array (T (&array_)[लंबाई_2])
{ वापस hb_array_t<T> (array_); }

ढाँचा <typename Type>
काष्ठा hb_sorted_array_t :
	hb_array_t<Type>,
	hb_iter_t<hb_sorted_array_t<Type>, Type&>
{
  प्रकार hb_iter_t<hb_sorted_array_t, Type&> iter_base_t;
  HB_ITER_USING (iter_base_t);
  अटल constexpr bool is_random_access_iterator = true;
  अटल constexpr bool is_sorted_iterator = true;
  अटल constexpr bool has_fast_len = true;

  hb_sorted_array_t () = शेष;
  hb_sorted_array_t (स्थिर hb_sorted_array_t&) = शेष;
  ~hb_sorted_array_t () = शेष;
  hb_sorted_array_t& चालक= (स्थिर hb_sorted_array_t&) = शेष;
  hb_sorted_array_t& चालक= (hb_sorted_array_t&&) = शेष;

  constexpr hb_sorted_array_t (Type *array_, अचिन्हित पूर्णांक लंबाई_2) : hb_array_t<Type> (array_, लंबाई_2) {}
  ढाँचा <अचिन्हित पूर्णांक लंबाई_2>
  constexpr hb_sorted_array_t (Type (&array_)[लंबाई_2]) : hb_array_t<Type> (array_) {}

  ढाँचा <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_sorted_array_t (स्थिर hb_array_t<U> &o) :
    hb_array_t<Type> (o),
    hb_iter_t<hb_sorted_array_t, Type&> () {}
  ढाँचा <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_sorted_array_t& चालक = (स्थिर hb_array_t<U> &o)
  { hb_array_t<Type> (*यह) = o; वापस *यह; }

  /* Iterator implementation. */

  /* See comment in hb_array_of::operator != */
  bool चालक != (स्थिर hb_sorted_array_t& o) स्थिर
  { वापस यह->arrayZ != o.arrayZ || यह->लंबाई != o.लंबाई; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () स्थिर { वापस यह->arrayZ; }
  Type *end () स्थिर { वापस यह->arrayZ + यह->लंबाई; }


  hb_sorted_array_t sub_array (अचिन्हित पूर्णांक start_offset, अचिन्हित पूर्णांक *seg_गिनती /* IN/OUT */) स्थिर
  { वापस hb_sorted_array_t (((स्थिर hb_array_t<Type> *) (यह))->sub_array (start_offset, seg_गिनती)); }
  hb_sorted_array_t sub_array (अचिन्हित पूर्णांक start_offset, अचिन्हित पूर्णांक seg_गिनती) स्थिर
  { वापस sub_array (start_offset, &seg_गिनती); }

  hb_sorted_array_t truncate (अचिन्हित लंबाई) स्थिर { वापस sub_array (0, लंबाई); }

  ढाँचा <typename T>
  Type *द्वा_खोज (स्थिर T &x, Type *not_found = nullptr)
  {
    अचिन्हित पूर्णांक i;
    वापस bfind (x, &i) ? &यह->arrayZ[i] : not_found;
  }
  ढाँचा <typename T>
  स्थिर Type *द्वा_खोज (स्थिर T &x, स्थिर Type *not_found = nullptr) स्थिर
  {
    अचिन्हित पूर्णांक i;
    वापस bfind (x, &i) ? &यह->arrayZ[i] : not_found;
  }
  ढाँचा <typename T>
  bool bfind (स्थिर T &x, अचिन्हित पूर्णांक *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      अचिन्हित पूर्णांक to_store = (अचिन्हित पूर्णांक) -1) स्थिर
  {
    अचिन्हित pos;

    अगर (bsearch_impl (x, &pos))
    {
      अगर (i)
	*i = pos;
      वापस true;
    }

    अगर (i)
    {
      चयन (not_found)
      {
	हाल HB_NOT_FOUND_DONT_STORE:
	  अवरोध;

	हाल HB_NOT_FOUND_STORE:
	  *i = to_store;
	  अवरोध;

	हाल HB_NOT_FOUND_STORE_CLOSEST:
	  *i = pos;
	  अवरोध;
      }
    }
    वापस false;
  }
  ढाँचा <typename T, typename ...Ts>
  bool bsearch_impl (स्थिर T &x, अचिन्हित *pos, Ts... ds) स्थिर
  {
    वापस hb_bsearch_impl (pos,
			    x,
			    यह->arrayZ,
			    यह->लंबाई,
			    माप (Type),
			    _hb_cmp_method<T, Type, Ts...>,
			    std::forward<Ts> (ds)...);
  }
};
ढाँचा <typename T> अंतरभूत hb_sorted_array_t<T>
hb_sorted_array (T *array, अचिन्हित पूर्णांक लंबाई)
{ वापस hb_sorted_array_t<T> (array, लंबाई); }
ढाँचा <typename T, अचिन्हित पूर्णांक लंबाई_2> अंतरभूत hb_sorted_array_t<T>
hb_sorted_array (T (&array_)[लंबाई_2])
{ वापस hb_sorted_array_t<T> (array_); }

ढाँचा <typename T>
अंतरभूत bool hb_array_t<T>::चालक == (स्थिर hb_array_t<T> &o) स्थिर
{
  अगर (o.लंबाई != यह->लंबाई) वापस false;
  क्रम (अचिन्हित पूर्णांक i = 0; i < यह->लंबाई; i++) {
    अगर (यह->arrayZ[i] != o.arrayZ[i]) वापस false;
  }
  वापस true;
}
ढाँचा <>
अंतरभूत bool hb_array_t<स्थिर अक्षर>::चालक == (स्थिर hb_array_t<स्थिर अक्षर> &o) स्थिर
{
  अगर (o.लंबाई != यह->लंबाई) वापस false;
  वापस 0 == hb_memcmp (arrayZ, o.arrayZ, लंबाई);
}
ढाँचा <>
अंतरभूत bool hb_array_t<स्थिर अचिन्हित अक्षर>::चालक == (स्थिर hb_array_t<स्थिर अचिन्हित अक्षर> &o) स्थिर
{
  अगर (o.लंबाई != यह->लंबाई) वापस false;
  वापस 0 == hb_memcmp (arrayZ, o.arrayZ, लंबाई);
}


/* Specialize hash() for byte arrays. */

#अगर_अघोषित HB_OPTIMIZE_आकार_MORE
ढाँचा <>
अंतरभूत uint32_t hb_array_t<स्थिर अक्षर>::hash () स्थिर
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  वापस fasthash32(arrayZ, लंबाई, 0xf437ffe6 /* magic? */);
}

ढाँचा <>
अंतरभूत uint32_t hb_array_t<स्थिर अचिन्हित अक्षर>::hash () स्थिर
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  वापस fasthash32(arrayZ, लंबाई, 0xf437ffe6 /* magic? */);
}
#पूर्ण_अगर


प्रकार hb_array_t<स्थिर अक्षर> hb_bytes_t;
प्रकार hb_array_t<स्थिर अचिन्हित अक्षर> hb_ubytes_t;



#पूर्ण_अगर /* HB_ARRAY_HH */

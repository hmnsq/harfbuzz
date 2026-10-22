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

#اگر_تعریف_نشده HB_ARRAY_HH
#تعریف_کن HB_ARRAY_HH

#ضمن "hb.hh"
#ضمن "hb-algs.hh"
#ضمن "hb-iter.hh"
#ضمن "hb-null.hh"


قالب <نام_نوع Type>
بنية hb_sorted_array_t;

تعداد hb_not_یافت_t
{
  HB_NOT_یافت_DONT_STORE,
  HB_NOT_یافت_STORE,
  HB_NOT_یافت_STORE_CLOSEST,
};


قالب <نام_نوع Type>
بنية hb_array_t : hb_iter_with_fallback_t<hb_array_t<Type>, Type&>
{
  ساكن constexpr منطقي realloc_move = درست;

  /*
   * Constructors.
   */
  hb_array_t () = افتراضي;
  hb_array_t (ثابت hb_array_t&) = افتراضي;
  ~hb_array_t () = افتراضي;
  hb_array_t& عملگر= (ثابت hb_array_t&) = افتراضي;
  hb_array_t& عملگر= (hb_array_t&&) = افتراضي;

  constexpr hb_array_t (Type *array_, بیعلامت صحيح درازی_2) : arrayZ (array_), درازی (درازی_2) {}
  قالب <بیعلامت صحيح درازی_2>
  constexpr hb_array_t (Type (&array_)[درازی_2]) : hb_array_t (array_, درازی_2) {}

  قالب <نام_نوع U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_array_t (ثابت hb_array_t<U> &o) :
    hb_iter_with_fallback_t<hb_array_t, Type&> (),
    arrayZ (o.arrayZ), درازی (o.درازی), backwards_درازی (o.backwards_درازی) {}
  قالب <نام_نوع U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_array_t& عملگر = (ثابت hb_array_t<U> &o)
  { arrayZ = o.arrayZ; درازی = o.درازی; backwards_درازی = o.backwards_درازی; أرجع *هذا; }

  /*
   * Iterator implementation.
   */
  تعريفنوع Type& __item_t__;
  ساكن constexpr منطقي is_random_access_iterator = درست;
  ساكن constexpr منطقي has_fast_len = درست;
  Type& __item__ () ثابت
  {
    إذا (unlikely (!درازی)) أرجع CrapOrNull (Type);
    أرجع *arrayZ;
  }
  Type& __item_at__ (بیعلامت i) ثابت
  {
    إذا (unlikely (i >= درازی)) أرجع CrapOrNull (Type);
    أرجع arrayZ[i];
  }
  فراغ __next__ ()
  {
    إذا (unlikely (!درازی))
      أرجع;
    درازی--;
    backwards_درازی++;
    arrayZ++;
  }
  فراغ __forward__ (بیعلامت n)
  {
    إذا (unlikely (n > درازی))
      n = درازی;
    درازی -= n;
    backwards_درازی += n;
    arrayZ += n;
  }
  فراغ __prev__ ()
  {
    إذا (unlikely (!backwards_درازی))
      أرجع;
    درازی++;
    backwards_درازی--;
    arrayZ--;
  }
  فراغ __rewind__ (بیعلامت n)
  {
    إذا (unlikely (n > backwards_درازی))
      n = backwards_درازی;
    درازی += n;
    backwards_درازی -= n;
    arrayZ -= n;
  }
  بیعلامت __len__ () ثابت { أرجع درازی; }
  /* Ouch. The operator== compares the contents of the array.  For range-based for loops,
   * it's best if we can just compare arrayZ, though comparing contents is still fast,
   * but also would require that Type has operator==.  As such, we optimize this operator
   * for range-based for loop and just compare arrayZ and length.
   *
   * The above comment is outdated now because we implemented separate begin/end to
   * objects that were using hb_array_t for range-based loop before. */
  منطقي عملگر != (ثابت hb_array_t& o) ثابت
  { أرجع هذا->arrayZ != o.arrayZ || هذا->درازی != o.درازی; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () ثابت { أرجع arrayZ; }
  Type *انتها () ثابت { أرجع arrayZ + درازی; }


  /* Extra operators.
   */
  Type * عملگر & () ثابت { أرجع arrayZ; }
  عملگر hb_array_t<ثابت Type> () { أرجع hb_array_t<ثابت Type> (arrayZ, درازی); }
  قالب <نام_نوع T> عملگر T * () ثابت { أرجع arrayZ; }

  HB_داخلی منطقي عملگر == (ثابت hb_array_t &o) ثابت;

  uint32_t hash () ثابت
  {
    // FNV-1a hash function
    // https://github.com/harfbuzz/harfbuzz/pull/4228
    uint32_t current = /*cbf29ce4*/0x84222325;
    لكل (خودکار &v : *هذا)
    {
      current = current ^ hb_hash (v);
      current = current * 16777619;
    }
    أرجع current;
  }

  /*
   * Compare, Sort, and Search.
   */

  /* Note: our compare is NOT lexicographic; it also does NOT call Type::cmp. */
  صحيح cmp (ثابت hb_array_t &a) ثابت
  {
    إذا (درازی != a.درازی)
      أرجع (صحيح) a.درازی - (صحيح) درازی;
    أرجع hb_memcmp (a.arrayZ, arrayZ, get_اندازه ());
  }
  HB_داخلی ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    hb_array_t *a = (hb_array_t *) pa;
    hb_array_t *b = (hb_array_t *) pb;
    أرجع b->cmp (*a);
  }

  قالب <نام_نوع T>
  Type *lsearch (ثابت T &x, Type *not_یافت = اشاره_تهی)
  {
    بیعلامت i;
    أرجع lfind (x, &i) ? &هذا->arrayZ[i] : not_یافت;
  }
  قالب <نام_نوع T>
  ثابت Type *lsearch (ثابت T &x, ثابت Type *not_یافت = اشاره_تهی) ثابت
  {
    بیعلامت i;
    أرجع lfind (x, &i) ? &هذا->arrayZ[i] : not_یافت;
  }
  قالب <نام_نوع T>
  منطقي lfind (ثابت T &x, بیعلامت *pos = اشاره_تهی,
	      hb_not_یافت_t not_یافت = HB_NOT_یافت_DONT_STORE,
	      بیعلامت صحيح مقصد_store = (بیعلامت صحيح) -1) ثابت
  {
    لكل (بیعلامت i = 0; i < درازی; ++i)
      إذا (hb_equal (x, هذا->arrayZ[i]))
      {
	إذا (pos)
	  *pos = i;
	أرجع درست;
      }

    إذا (pos)
    {
      بدل (not_یافت)
      {
	حالة HB_NOT_یافت_DONT_STORE:
	  اكسر;

	حالة HB_NOT_یافت_STORE:
	  *pos = مقصد_store;
	  اكسر;

	حالة HB_NOT_یافت_STORE_CLOSEST:
	  *pos = درازی;
	  اكسر;
      }
    }
    أرجع خطأ;
  }

  hb_sorted_array_t<Type> qsort (صحيح (*cmp)(ثابت فراغ*, ثابت فراغ*))
  {
    إذا (likely (درازی))
      hb_qsort (arrayZ, درازی, هذا->get_قلم_اندازه (), cmp);
    أرجع hb_sorted_array_t<Type> (*هذا);
  }

  /* Comparator follows the C qsort convention: returns
   * negative / zero / positive int. */
  قالب <نام_نوع Compar>
  hb_sorted_array_t<Type> qsort (Compar compar)
  {
    إذا (likely (درازی))
      hb_qsort_inline (arrayZ, درازی, compar);
    أرجع hb_sorted_array_t<Type> (*هذا);
  }

  خاص:
  قالب <نام_نوع T = Type,
	    hb_enable_if (std::is_move_assignable<T>::value)>
  hb_sorted_array_t<Type> _qsort (hb_اولویت<1>)
  {
    أرجع qsort ([] (ثابت Type &a, ثابت Type &b) { أرجع Type::cmp (&a, &b); });
  }
  hb_sorted_array_t<Type> _qsort (hb_اولویت<0>)
  {
    أرجع qsort ((صحيح(*)(ثابت فراغ*, ثابت فراغ*)) Type::cmp);
  }
  عام:

  hb_sorted_array_t<Type> qsort ()
  {
    أرجع _qsort (hb_prioritize);
  }

  /*
   * Other methods.
   */

  size_t get_اندازه () ثابت { أرجع درازی * هذا->get_قلم_اندازه (); }

  /*
   * Reverse the order of items in this array in the range [start, end).
   */
  فراغ reverse (بیعلامت شروع = 0, بیعلامت انتها = -1)
  {
    شروع = hb_min (شروع, درازی);
    انتها = hb_min (انتها, درازی);

    إذا (انتها < شروع + 2)
      أرجع;

    بیعلامت stop = شروع + (انتها - شروع) / 2;
    لكل (بیعلامت lhs = شروع, rhs = انتها - 1; lhs < stop; lhs++, rhs--)
      hb_جابجا (arrayZ[rhs], arrayZ[lhs]);
  }

  hb_array_t sub_array (بیعلامت صحيح شروع_جابه_جایی = 0, بیعلامت صحيح *seg_شمار = اشاره_تهی /* IN/OUT */) ثابت
  {
    إذا (!شروع_جابه_جایی && !seg_شمار)
      أرجع *هذا;

    بیعلامت صحيح شمار = درازی;
    إذا (unlikely (شروع_جابه_جایی > شمار))
      شمار = 0;
    وإلا
      شمار -= شروع_جابه_جایی;
    إذا (seg_شمار)
      شمار = *seg_شمار = hb_min (شمار, *seg_شمار);
    أرجع hb_array_t (arrayZ + شروع_جابه_جایی, شمار);
  }
  hb_array_t sub_array (بیعلامت صحيح شروع_جابه_جایی, بیعلامت صحيح seg_شمار) ثابت
  { أرجع sub_array (شروع_جابه_جایی, &seg_شمار); }

  hb_array_t truncate (بیعلامت درازی) ثابت { أرجع sub_array (0, درازی); }

  قالب <نام_نوع T,
	    بیعلامت P = حجم (Type),
	    hb_enable_if (P == 1)>
  ثابت T *as () ثابت
  { أرجع درازی < hb_min_اندازه (T) ? &Null (T) : reinterpret_cast<ثابت T *> (arrayZ); }

  قالب <نام_نوع T,
	    بیعلامت P = حجم (Type),
	    hb_enable_if (P == 1)>
  منطقي check_range (ثابت T *p, بیعلامت صحيح اندازه = T::static_اندازه) ثابت
  {
    أرجع arrayZ <= ((ثابت حرف *) p)
	&& ((ثابت حرف *) p) <= arrayZ + درازی
	&& (بیعلامت صحيح) (arrayZ + درازی - (ثابت حرف *) p) >= اندازه;
  }

  قالب <بیعلامت P = حجم (Type),
	    hb_enable_if (P == 1)>
  منطقي check_انتها (ثابت فراغ *p) ثابت
  {
    أرجع (uintptr_t) (((ثابت حرف *) p) - arrayZ) <= درازی;
  }

  /* Only call if you allocated the underlying array using hb_malloc() or similar. */
  فراغ fini ()
  { hb_free ((فراغ *) arrayZ); arrayZ = اشاره_تهی; درازی = 0; }

  قالب <نام_نوع hb_serialize_context_t,
	    نام_نوع U = Type,
	    hb_enable_if (!(حجم (U) < حجم (دراز دراز) && hb_is_trivially_copy_assignable(hb_decay<Type>)))>
  hb_array_t copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار* بیرون = c->شروع_embed (arrayZ);
    إذا (unlikely (!c->extend_اندازه (بیرون, get_اندازه (), خطأ))) return_trace (hb_array_t ());
    لكل (بیعلامت i = 0; i < درازی; i++)
      بیرون[i] = arrayZ[i]; /* TODO: add version that calls c->copy() */
    return_trace (hb_array_t (بیرون, درازی));
  }

  قالب <نام_نوع hb_serialize_context_t,
	    نام_نوع U = Type,
	    hb_enable_if (حجم (U) < حجم (دراز دراز) && hb_is_trivially_copy_assignable(hb_decay<Type>))>
  hb_array_t copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار* بیرون = c->شروع_embed (arrayZ);
    إذا (unlikely (!c->extend_اندازه (بیرون, get_اندازه (), خطأ))) return_trace (hb_array_t ());
    hb_memcpy (بیرون, arrayZ, get_اندازه ());
    return_trace (hb_array_t (بیرون, درازی));
  }

  قالب <نام_نوع hb_sanitize_context_t>
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  { أرجع c->check_array (arrayZ, درازی); }

  /*
   * Members
   */

  عام:
  Type *arrayZ = اشاره_تهی;
  بیعلامت صحيح درازی = 0;
  بیعلامت صحيح backwards_درازی = 0;
};
قالب <نام_نوع T> مضمن hb_array_t<T>
hb_array ()
{ أرجع hb_array_t<T> (); }
قالب <نام_نوع T> مضمن hb_array_t<T>
hb_array (T *array, بیعلامت صحيح درازی)
{ أرجع hb_array_t<T> (array, درازی); }
قالب <نام_نوع T, بیعلامت صحيح درازی_2> مضمن hb_array_t<T>
hb_array (T (&array_)[درازی_2])
{ أرجع hb_array_t<T> (array_); }

قالب <نام_نوع Type>
بنية hb_sorted_array_t :
	hb_array_t<Type>,
	hb_iter_t<hb_sorted_array_t<Type>, Type&>
{
  تعريفنوع hb_iter_t<hb_sorted_array_t, Type&> iter_پایه_t;
  HB_ITER_USING (iter_پایه_t);
  ساكن constexpr منطقي is_random_access_iterator = درست;
  ساكن constexpr منطقي is_sorted_iterator = درست;
  ساكن constexpr منطقي has_fast_len = درست;

  hb_sorted_array_t () = افتراضي;
  hb_sorted_array_t (ثابت hb_sorted_array_t&) = افتراضي;
  ~hb_sorted_array_t () = افتراضي;
  hb_sorted_array_t& عملگر= (ثابت hb_sorted_array_t&) = افتراضي;
  hb_sorted_array_t& عملگر= (hb_sorted_array_t&&) = افتراضي;

  constexpr hb_sorted_array_t (Type *array_, بیعلامت صحيح درازی_2) : hb_array_t<Type> (array_, درازی_2) {}
  قالب <بیعلامت صحيح درازی_2>
  constexpr hb_sorted_array_t (Type (&array_)[درازی_2]) : hb_array_t<Type> (array_) {}

  قالب <نام_نوع U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_sorted_array_t (ثابت hb_array_t<U> &o) :
    hb_array_t<Type> (o),
    hb_iter_t<hb_sorted_array_t, Type&> () {}
  قالب <نام_نوع U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_sorted_array_t& عملگر = (ثابت hb_array_t<U> &o)
  { hb_array_t<Type> (*هذا) = o; أرجع *هذا; }

  /* Iterator implementation. */

  /* See comment in hb_array_of::operator != */
  منطقي عملگر != (ثابت hb_sorted_array_t& o) ثابت
  { أرجع هذا->arrayZ != o.arrayZ || هذا->درازی != o.درازی; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () ثابت { أرجع هذا->arrayZ; }
  Type *انتها () ثابت { أرجع هذا->arrayZ + هذا->درازی; }


  hb_sorted_array_t sub_array (بیعلامت صحيح شروع_جابه_جایی, بیعلامت صحيح *seg_شمار /* IN/OUT */) ثابت
  { أرجع hb_sorted_array_t (((ثابت hb_array_t<Type> *) (هذا))->sub_array (شروع_جابه_جایی, seg_شمار)); }
  hb_sorted_array_t sub_array (بیعلامت صحيح شروع_جابه_جایی, بیعلامت صحيح seg_شمار) ثابت
  { أرجع sub_array (شروع_جابه_جایی, &seg_شمار); }

  hb_sorted_array_t truncate (بیعلامت درازی) ثابت { أرجع sub_array (0, درازی); }

  قالب <نام_نوع T>
  Type *bsearch (ثابت T &x, Type *not_یافت = اشاره_تهی)
  {
    بیعلامت صحيح i;
    أرجع bfind (x, &i) ? &هذا->arrayZ[i] : not_یافت;
  }
  قالب <نام_نوع T>
  ثابت Type *bsearch (ثابت T &x, ثابت Type *not_یافت = اشاره_تهی) ثابت
  {
    بیعلامت صحيح i;
    أرجع bfind (x, &i) ? &هذا->arrayZ[i] : not_یافت;
  }
  قالب <نام_نوع T>
  منطقي bfind (ثابت T &x, بیعلامت صحيح *i = اشاره_تهی,
	      hb_not_یافت_t not_یافت = HB_NOT_یافت_DONT_STORE,
	      بیعلامت صحيح مقصد_store = (بیعلامت صحيح) -1) ثابت
  {
    بیعلامت pos;

    إذا (bsearch_impl (x, &pos))
    {
      إذا (i)
	*i = pos;
      أرجع درست;
    }

    إذا (i)
    {
      بدل (not_یافت)
      {
	حالة HB_NOT_یافت_DONT_STORE:
	  اكسر;

	حالة HB_NOT_یافت_STORE:
	  *i = مقصد_store;
	  اكسر;

	حالة HB_NOT_یافت_STORE_CLOSEST:
	  *i = pos;
	  اكسر;
      }
    }
    أرجع خطأ;
  }
  قالب <نام_نوع T, نام_نوع ...Ts>
  منطقي bsearch_impl (ثابت T &x, بیعلامت *pos, Ts... ds) ثابت
  {
    أرجع hb_bsearch_impl (pos,
			    x,
			    هذا->arrayZ,
			    هذا->درازی,
			    حجم (Type),
			    hb_cmp_روش<T, Type, Ts...>,
			    std::forward<Ts> (ds)...);
  }
};
قالب <نام_نوع T> مضمن hb_sorted_array_t<T>
hb_sorted_array (T *array, بیعلامت صحيح درازی)
{ أرجع hb_sorted_array_t<T> (array, درازی); }
قالب <نام_نوع T, بیعلامت صحيح درازی_2> مضمن hb_sorted_array_t<T>
hb_sorted_array (T (&array_)[درازی_2])
{ أرجع hb_sorted_array_t<T> (array_); }

قالب <نام_نوع T>
مضمن منطقي hb_array_t<T>::عملگر == (ثابت hb_array_t<T> &o) ثابت
{
  إذا (o.درازی != هذا->درازی) أرجع خطأ;
  لكل (بیعلامت صحيح i = 0; i < هذا->درازی; i++) {
    إذا (هذا->arrayZ[i] != o.arrayZ[i]) أرجع خطأ;
  }
  أرجع درست;
}
قالب <>
مضمن منطقي hb_array_t<ثابت حرف>::عملگر == (ثابت hb_array_t<ثابت حرف> &o) ثابت
{
  إذا (o.درازی != هذا->درازی) أرجع خطأ;
  أرجع 0 == hb_memcmp (arrayZ, o.arrayZ, درازی);
}
قالب <>
مضمن منطقي hb_array_t<ثابت بیعلامت حرف>::عملگر == (ثابت hb_array_t<ثابت بیعلامت حرف> &o) ثابت
{
  إذا (o.درازی != هذا->درازی) أرجع خطأ;
  أرجع 0 == hb_memcmp (arrayZ, o.arrayZ, درازی);
}


/* Specialize hash() for byte arrays. */

#اگر_تعریف_نشده HB_OPTIMIZE_اندازه_بیشتر
قالب <>
مضمن uint32_t hb_array_t<ثابت حرف>::hash () ثابت
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  أرجع fasthash32(arrayZ, درازی, 0xf437ffe6 /* magic? */);
}

قالب <>
مضمن uint32_t hb_array_t<ثابت بیعلامت حرف>::hash () ثابت
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  أرجع fasthash32(arrayZ, درازی, 0xf437ffe6 /* magic? */);
}
#پایان_اگر


تعريفنوع hb_array_t<ثابت حرف> hb_بایتها_t;
تعريفنوع hb_array_t<ثابت بیعلامت حرف> hb_ubytes_t;



#پایان_اگر /* HB_ARRAY_HH */

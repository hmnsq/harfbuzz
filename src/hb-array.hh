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

#جے_بنا_تعریف HB_ARRAY_HH
#تعریف_کرو HB_ARRAY_HH

#ضمن "hb.hh"
#ضمن "hb-algs.hh"
#ضمن "hb-iter.hh"
#ضمن "hb-null.hh"


سانچا <قسم_ناں Type>
بنية hb_sorted_array_t;

تعداد hb_not_لبھیا_t
{
  HB_NOT_لبھیا_DONT_STORE,
  HB_NOT_لبھیا_STORE,
  HB_NOT_لبھیا_STORE_CLOSEST,
};


سانچا <قسم_ناں Type>
بنية hb_array_t : hb_iter_with_fallback_t<hb_array_t<Type>, Type&>
{
  ساكن constexpr منطقي realloc_move = سچ;

  /*
   * Constructors.
   */
  hb_array_t () = افتراضي;
  hb_array_t (ثابت hb_array_t&) = افتراضي;
  ~hb_array_t () = افتراضي;
  hb_array_t& عامل= (ثابت hb_array_t&) = افتراضي;
  hb_array_t& عامل= (hb_array_t&&) = افتراضي;

  constexpr hb_array_t (Type *array_, بنانشانی صحيح لمائی_2) : arrayZ (array_), لمائی (لمائی_2) {}
  سانچا <بنانشانی صحيح لمائی_2>
  constexpr hb_array_t (Type (&array_)[لمائی_2]) : hb_array_t (array_, لمائی_2) {}

  سانچا <قسم_ناں U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_array_t (ثابت hb_array_t<U> &o) :
    hb_iter_with_fallback_t<hb_array_t, Type&> (),
    arrayZ (o.arrayZ), لمائی (o.لمائی), backwards_لمائی (o.backwards_لمائی) {}
  سانچا <قسم_ناں U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_array_t& عامل = (ثابت hb_array_t<U> &o)
  { arrayZ = o.arrayZ; لمائی = o.لمائی; backwards_لمائی = o.backwards_لمائی; أرجع *هذا; }

  /*
   * Iterator implementation.
   */
  تعريفنوع Type& __item_t__;
  ساكن constexpr منطقي is_random_access_iterator = سچ;
  ساكن constexpr منطقي has_fast_len = سچ;
  Type& __item__ () ثابت
  {
    إذا (unlikely (!لمائی)) أرجع CrapOrNull (Type);
    أرجع *arrayZ;
  }
  Type& __item_at__ (بنانشانی i) ثابت
  {
    إذا (unlikely (i >= لمائی)) أرجع CrapOrNull (Type);
    أرجع arrayZ[i];
  }
  فراغ __next__ ()
  {
    إذا (unlikely (!لمائی))
      أرجع;
    لمائی--;
    backwards_لمائی++;
    arrayZ++;
  }
  فراغ __forward__ (بنانشانی n)
  {
    إذا (unlikely (n > لمائی))
      n = لمائی;
    لمائی -= n;
    backwards_لمائی += n;
    arrayZ += n;
  }
  فراغ __prev__ ()
  {
    إذا (unlikely (!backwards_لمائی))
      أرجع;
    لمائی++;
    backwards_لمائی--;
    arrayZ--;
  }
  فراغ __rewind__ (بنانشانی n)
  {
    إذا (unlikely (n > backwards_لمائی))
      n = backwards_لمائی;
    لمائی += n;
    backwards_لمائی -= n;
    arrayZ -= n;
  }
  بنانشانی __len__ () ثابت { أرجع لمائی; }
  /* Ouch. The operator== compares the contents of the array.  For range-based for loops,
   * it's best if we can just compare arrayZ, though comparing contents is still fast,
   * but also would require that Type has operator==.  As such, we optimize this operator
   * for range-based for loop and just compare arrayZ and length.
   *
   * The above comment is outdated now because we implemented separate begin/end to
   * objects that were using hb_array_t for range-based loop before. */
  منطقي عامل != (ثابت hb_array_t& o) ثابت
  { أرجع هذا->arrayZ != o.arrayZ || هذا->لمائی != o.لمائی; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () ثابت { أرجع arrayZ; }
  Type *سرا () ثابت { أرجع arrayZ + لمائی; }


  /* Extra operators.
   */
  Type * عامل & () ثابت { أرجع arrayZ; }
  عامل hb_array_t<ثابت Type> () { أرجع hb_array_t<ثابت Type> (arrayZ, لمائی); }
  سانچا <قسم_ناں T> عامل T * () ثابت { أرجع arrayZ; }

  HB_اندرونی منطقي عامل == (ثابت hb_array_t &o) ثابت;

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
    إذا (لمائی != a.لمائی)
      أرجع (صحيح) a.لمائی - (صحيح) لمائی;
    أرجع hb_memcmp (a.arrayZ, arrayZ, get_ناپ ());
  }
  HB_اندرونی ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    hb_array_t *a = (hb_array_t *) pa;
    hb_array_t *b = (hb_array_t *) pb;
    أرجع b->cmp (*a);
  }

  سانچا <قسم_ناں T>
  Type *lsearch (ثابت T &x, Type *not_لبھیا = خالی_سوچک)
  {
    بنانشانی i;
    أرجع lfind (x, &i) ? &هذا->arrayZ[i] : not_لبھیا;
  }
  سانچا <قسم_ناں T>
  ثابت Type *lsearch (ثابت T &x, ثابت Type *not_لبھیا = خالی_سوچک) ثابت
  {
    بنانشانی i;
    أرجع lfind (x, &i) ? &هذا->arrayZ[i] : not_لبھیا;
  }
  سانچا <قسم_ناں T>
  منطقي lfind (ثابت T &x, بنانشانی *pos = خالی_سوچک,
	      hb_not_لبھیا_t not_لبھیا = HB_NOT_لبھیا_DONT_STORE,
	      بنانشانی صحيح منزل_store = (بنانشانی صحيح) -1) ثابت
  {
    لكل (بنانشانی i = 0; i < لمائی; ++i)
      إذا (hb_equal (x, هذا->arrayZ[i]))
      {
	إذا (pos)
	  *pos = i;
	أرجع سچ;
      }

    إذا (pos)
    {
      بدل (not_لبھیا)
      {
	حالة HB_NOT_لبھیا_DONT_STORE:
	  اكسر;

	حالة HB_NOT_لبھیا_STORE:
	  *pos = منزل_store;
	  اكسر;

	حالة HB_NOT_لبھیا_STORE_CLOSEST:
	  *pos = لمائی;
	  اكسر;
      }
    }
    أرجع خطأ;
  }

  hb_sorted_array_t<Type> qsort (صحيح (*cmp)(ثابت فراغ*, ثابت فراغ*))
  {
    إذا (likely (لمائی))
      hb_qsort (arrayZ, لمائی, هذا->get_شے_ناپ (), cmp);
    أرجع hb_sorted_array_t<Type> (*هذا);
  }

  /* Comparator follows the C qsort convention: returns
   * negative / zero / positive int. */
  سانچا <قسم_ناں Compar>
  hb_sorted_array_t<Type> qsort (Compar compar)
  {
    إذا (likely (لمائی))
      hb_qsort_inline (arrayZ, لمائی, compar);
    أرجع hb_sorted_array_t<Type> (*هذا);
  }

  خاص:
  سانچا <قسم_ناں T = Type,
	    hb_enable_if (std::is_move_assignable<T>::value)>
  hb_sorted_array_t<Type> _qsort (hb_ترجیح<1>)
  {
    أرجع qsort ([] (ثابت Type &a, ثابت Type &b) { أرجع Type::cmp (&a, &b); });
  }
  hb_sorted_array_t<Type> _qsort (hb_ترجیح<0>)
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

  size_t get_ناپ () ثابت { أرجع لمائی * هذا->get_شے_ناپ (); }

  /*
   * Reverse the order of items in this array in the range [start, end).
   */
  فراغ reverse (بنانشانی شروع = 0, بنانشانی سرا = -1)
  {
    شروع = hb_min (شروع, لمائی);
    سرا = hb_min (سرا, لمائی);

    إذا (سرا < شروع + 2)
      أرجع;

    بنانشانی stop = شروع + (سرا - شروع) / 2;
    لكل (بنانشانی lhs = شروع, rhs = سرا - 1; lhs < stop; lhs++, rhs--)
      hb_وٹاندرا (arrayZ[rhs], arrayZ[lhs]);
  }

  hb_array_t sub_array (بنانشانی صحيح شروع_وتھ = 0, بنانشانی صحيح *seg_گنتی = خالی_سوچک /* IN/OUT */) ثابت
  {
    إذا (!شروع_وتھ && !seg_گنتی)
      أرجع *هذا;

    بنانشانی صحيح گنتی = لمائی;
    إذا (unlikely (شروع_وتھ > گنتی))
      گنتی = 0;
    وإلا
      گنتی -= شروع_وتھ;
    إذا (seg_گنتی)
      گنتی = *seg_گنتی = hb_min (گنتی, *seg_گنتی);
    أرجع hb_array_t (arrayZ + شروع_وتھ, گنتی);
  }
  hb_array_t sub_array (بنانشانی صحيح شروع_وتھ, بنانشانی صحيح seg_گنتی) ثابت
  { أرجع sub_array (شروع_وتھ, &seg_گنتی); }

  hb_array_t truncate (بنانشانی لمائی) ثابت { أرجع sub_array (0, لمائی); }

  سانچا <قسم_ناں T,
	    بنانشانی P = حجم (Type),
	    hb_enable_if (P == 1)>
  ثابت T *as () ثابت
  { أرجع لمائی < hb_min_ناپ (T) ? &Null (T) : reinterpret_cast<ثابت T *> (arrayZ); }

  سانچا <قسم_ناں T,
	    بنانشانی P = حجم (Type),
	    hb_enable_if (P == 1)>
  منطقي check_range (ثابت T *p, بنانشانی صحيح ناپ = T::static_ناپ) ثابت
  {
    أرجع arrayZ <= ((ثابت حرف *) p)
	&& ((ثابت حرف *) p) <= arrayZ + لمائی
	&& (بنانشانی صحيح) (arrayZ + لمائی - (ثابت حرف *) p) >= ناپ;
  }

  سانچا <بنانشانی P = حجم (Type),
	    hb_enable_if (P == 1)>
  منطقي check_سرا (ثابت فراغ *p) ثابت
  {
    أرجع (uintptr_t) (((ثابت حرف *) p) - arrayZ) <= لمائی;
  }

  /* Only call if you allocated the underlying array using hb_malloc() or similar. */
  فراغ fini ()
  { hb_free ((فراغ *) arrayZ); arrayZ = خالی_سوچک; لمائی = 0; }

  سانچا <قسم_ناں hb_serialize_context_t,
	    قسم_ناں U = Type,
	    hb_enable_if (!(حجم (U) < حجم (لما لما) && hb_is_trivially_copy_assignable(hb_decay<Type>)))>
  hb_array_t copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار* باہرلا = c->شروع_embed (arrayZ);
    إذا (unlikely (!c->extend_ناپ (باہرلا, get_ناپ (), خطأ))) return_trace (hb_array_t ());
    لكل (بنانشانی i = 0; i < لمائی; i++)
      باہرلا[i] = arrayZ[i]; /* TODO: add version that calls c->copy() */
    return_trace (hb_array_t (باہرلا, لمائی));
  }

  سانچا <قسم_ناں hb_serialize_context_t,
	    قسم_ناں U = Type,
	    hb_enable_if (حجم (U) < حجم (لما لما) && hb_is_trivially_copy_assignable(hb_decay<Type>))>
  hb_array_t copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار* باہرلا = c->شروع_embed (arrayZ);
    إذا (unlikely (!c->extend_ناپ (باہرلا, get_ناپ (), خطأ))) return_trace (hb_array_t ());
    hb_memcpy (باہرلا, arrayZ, get_ناپ ());
    return_trace (hb_array_t (باہرلا, لمائی));
  }

  سانچا <قسم_ناں hb_sanitize_context_t>
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  { أرجع c->check_array (arrayZ, لمائی); }

  /*
   * Members
   */

  عام:
  Type *arrayZ = خالی_سوچک;
  بنانشانی صحيح لمائی = 0;
  بنانشانی صحيح backwards_لمائی = 0;
};
سانچا <قسم_ناں T> مضمن hb_array_t<T>
hb_array ()
{ أرجع hb_array_t<T> (); }
سانچا <قسم_ناں T> مضمن hb_array_t<T>
hb_array (T *array, بنانشانی صحيح لمائی)
{ أرجع hb_array_t<T> (array, لمائی); }
سانچا <قسم_ناں T, بنانشانی صحيح لمائی_2> مضمن hb_array_t<T>
hb_array (T (&array_)[لمائی_2])
{ أرجع hb_array_t<T> (array_); }

سانچا <قسم_ناں Type>
بنية hb_sorted_array_t :
	hb_array_t<Type>,
	hb_iter_t<hb_sorted_array_t<Type>, Type&>
{
  تعريفنوع hb_iter_t<hb_sorted_array_t, Type&> iter_ادھار_t;
  HB_ITER_USING (iter_ادھار_t);
  ساكن constexpr منطقي is_random_access_iterator = سچ;
  ساكن constexpr منطقي is_sorted_iterator = سچ;
  ساكن constexpr منطقي has_fast_len = سچ;

  hb_sorted_array_t () = افتراضي;
  hb_sorted_array_t (ثابت hb_sorted_array_t&) = افتراضي;
  ~hb_sorted_array_t () = افتراضي;
  hb_sorted_array_t& عامل= (ثابت hb_sorted_array_t&) = افتراضي;
  hb_sorted_array_t& عامل= (hb_sorted_array_t&&) = افتراضي;

  constexpr hb_sorted_array_t (Type *array_, بنانشانی صحيح لمائی_2) : hb_array_t<Type> (array_, لمائی_2) {}
  سانچا <بنانشانی صحيح لمائی_2>
  constexpr hb_sorted_array_t (Type (&array_)[لمائی_2]) : hb_array_t<Type> (array_) {}

  سانچا <قسم_ناں U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_sorted_array_t (ثابت hb_array_t<U> &o) :
    hb_array_t<Type> (o),
    hb_iter_t<hb_sorted_array_t, Type&> () {}
  سانچا <قسم_ناں U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_sorted_array_t& عامل = (ثابت hb_array_t<U> &o)
  { hb_array_t<Type> (*هذا) = o; أرجع *هذا; }

  /* Iterator implementation. */

  /* See comment in hb_array_of::operator != */
  منطقي عامل != (ثابت hb_sorted_array_t& o) ثابت
  { أرجع هذا->arrayZ != o.arrayZ || هذا->لمائی != o.لمائی; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () ثابت { أرجع هذا->arrayZ; }
  Type *سرا () ثابت { أرجع هذا->arrayZ + هذا->لمائی; }


  hb_sorted_array_t sub_array (بنانشانی صحيح شروع_وتھ, بنانشانی صحيح *seg_گنتی /* IN/OUT */) ثابت
  { أرجع hb_sorted_array_t (((ثابت hb_array_t<Type> *) (هذا))->sub_array (شروع_وتھ, seg_گنتی)); }
  hb_sorted_array_t sub_array (بنانشانی صحيح شروع_وتھ, بنانشانی صحيح seg_گنتی) ثابت
  { أرجع sub_array (شروع_وتھ, &seg_گنتی); }

  hb_sorted_array_t truncate (بنانشانی لمائی) ثابت { أرجع sub_array (0, لمائی); }

  سانچا <قسم_ناں T>
  Type *bsearch (ثابت T &x, Type *not_لبھیا = خالی_سوچک)
  {
    بنانشانی صحيح i;
    أرجع bfind (x, &i) ? &هذا->arrayZ[i] : not_لبھیا;
  }
  سانچا <قسم_ناں T>
  ثابت Type *bsearch (ثابت T &x, ثابت Type *not_لبھیا = خالی_سوچک) ثابت
  {
    بنانشانی صحيح i;
    أرجع bfind (x, &i) ? &هذا->arrayZ[i] : not_لبھیا;
  }
  سانچا <قسم_ناں T>
  منطقي bfind (ثابت T &x, بنانشانی صحيح *i = خالی_سوچک,
	      hb_not_لبھیا_t not_لبھیا = HB_NOT_لبھیا_DONT_STORE,
	      بنانشانی صحيح منزل_store = (بنانشانی صحيح) -1) ثابت
  {
    بنانشانی pos;

    إذا (bsearch_impl (x, &pos))
    {
      إذا (i)
	*i = pos;
      أرجع سچ;
    }

    إذا (i)
    {
      بدل (not_لبھیا)
      {
	حالة HB_NOT_لبھیا_DONT_STORE:
	  اكسر;

	حالة HB_NOT_لبھیا_STORE:
	  *i = منزل_store;
	  اكسر;

	حالة HB_NOT_لبھیا_STORE_CLOSEST:
	  *i = pos;
	  اكسر;
      }
    }
    أرجع خطأ;
  }
  سانچا <قسم_ناں T, قسم_ناں ...Ts>
  منطقي bsearch_impl (ثابت T &x, بنانشانی *pos, Ts... ds) ثابت
  {
    أرجع hb_bsearch_impl (pos,
			    x,
			    هذا->arrayZ,
			    هذا->لمائی,
			    حجم (Type),
			    hb_cmp_طریقہ<T, Type, Ts...>,
			    std::forward<Ts> (ds)...);
  }
};
سانچا <قسم_ناں T> مضمن hb_sorted_array_t<T>
hb_sorted_array (T *array, بنانشانی صحيح لمائی)
{ أرجع hb_sorted_array_t<T> (array, لمائی); }
سانچا <قسم_ناں T, بنانشانی صحيح لمائی_2> مضمن hb_sorted_array_t<T>
hb_sorted_array (T (&array_)[لمائی_2])
{ أرجع hb_sorted_array_t<T> (array_); }

سانچا <قسم_ناں T>
مضمن منطقي hb_array_t<T>::عامل == (ثابت hb_array_t<T> &o) ثابت
{
  إذا (o.لمائی != هذا->لمائی) أرجع خطأ;
  لكل (بنانشانی صحيح i = 0; i < هذا->لمائی; i++) {
    إذا (هذا->arrayZ[i] != o.arrayZ[i]) أرجع خطأ;
  }
  أرجع سچ;
}
سانچا <>
مضمن منطقي hb_array_t<ثابت حرف>::عامل == (ثابت hb_array_t<ثابت حرف> &o) ثابت
{
  إذا (o.لمائی != هذا->لمائی) أرجع خطأ;
  أرجع 0 == hb_memcmp (arrayZ, o.arrayZ, لمائی);
}
سانچا <>
مضمن منطقي hb_array_t<ثابت بنانشانی حرف>::عامل == (ثابت hb_array_t<ثابت بنانشانی حرف> &o) ثابت
{
  إذا (o.لمائی != هذا->لمائی) أرجع خطأ;
  أرجع 0 == hb_memcmp (arrayZ, o.arrayZ, لمائی);
}


/* Specialize hash() for byte arrays. */

#جے_بنا_تعریف HB_OPTIMIZE_ناپ_ہور
سانچا <>
مضمن uint32_t hb_array_t<ثابت حرف>::hash () ثابت
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  أرجع fasthash32(arrayZ, لمائی, 0xf437ffe6 /* magic? */);
}

سانچا <>
مضمن uint32_t hb_array_t<ثابت بنانشانی حرف>::hash () ثابت
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  أرجع fasthash32(arrayZ, لمائی, 0xf437ffe6 /* magic? */);
}
#جے_انت


تعريفنوع hb_array_t<ثابت حرف> hb_بائٹاں_t;
تعريفنوع hb_array_t<ثابت بنانشانی حرف> hb_ubytes_t;



#جے_انت /* HB_ARRAY_HH */

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

#اگر_غیر_تعریف_شدہ HB_ARRAY_HH
#تعریف_کرو HB_ARRAY_HH

#شامل "hb.hh"
#شامل "hb-algs.hh"
#شامل "hb-iter.hh"
#شامل "hb-null.hh"


سانچہ <قسم_نام Type>
ساخت hb_sorted_array_t;

شمار hb_not_ملا_t
{
  HB_NOT_ملا_DONT_STORE,
  HB_NOT_ملا_STORE,
  HB_NOT_ملا_STORE_CLOSEST,
};


سانچہ <قسم_نام Type>
ساخت hb_array_t : hb_iter_with_fallback_t<hb_array_t<Type>, Type&>
{
  ساکن constexpr منطقی realloc_move = سچ;

  /*
   * Constructors.
   */
  hb_array_t () = طےشدہ;
  hb_array_t (ثابت hb_array_t&) = طےشدہ;
  ~hb_array_t () = طےشدہ;
  hb_array_t& عامل= (ثابت hb_array_t&) = طےشدہ;
  hb_array_t& عامل= (hb_array_t&&) = طےشدہ;

  constexpr hb_array_t (Type *array_, غیرعلامتی صحیح لمبائی_2) : arrayZ (array_), لمبائی (لمبائی_2) {}
  سانچہ <غیرعلامتی صحیح لمبائی_2>
  constexpr hb_array_t (Type (&array_)[لمبائی_2]) : hb_array_t (array_, لمبائی_2) {}

  سانچہ <قسم_نام U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_array_t (ثابت hb_array_t<U> &o) :
    hb_iter_with_fallback_t<hb_array_t, Type&> (),
    arrayZ (o.arrayZ), لمبائی (o.لمبائی), backwards_لمبائی (o.backwards_لمبائی) {}
  سانچہ <قسم_نام U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_array_t& عامل = (ثابت hb_array_t<U> &o)
  { arrayZ = o.arrayZ; لمبائی = o.لمبائی; backwards_لمبائی = o.backwards_لمبائی; واپس *یہ; }

  /*
   * Iterator implementation.
   */
  قسمتعریف Type& __item_t__;
  ساکن constexpr منطقی is_random_access_iterator = سچ;
  ساکن constexpr منطقی has_fast_len = سچ;
  Type& __item__ () ثابت
  {
    اگر (unlikely (!لمبائی)) واپس CrapOrNull (Type);
    واپس *arrayZ;
  }
  Type& __item_at__ (غیرعلامتی i) ثابت
  {
    اگر (unlikely (i >= لمبائی)) واپس CrapOrNull (Type);
    واپس arrayZ[i];
  }
  خالی __next__ ()
  {
    اگر (unlikely (!لمبائی))
      واپس;
    لمبائی--;
    backwards_لمبائی++;
    arrayZ++;
  }
  خالی __forward__ (غیرعلامتی n)
  {
    اگر (unlikely (n > لمبائی))
      n = لمبائی;
    لمبائی -= n;
    backwards_لمبائی += n;
    arrayZ += n;
  }
  خالی __prev__ ()
  {
    اگر (unlikely (!backwards_لمبائی))
      واپس;
    لمبائی++;
    backwards_لمبائی--;
    arrayZ--;
  }
  خالی __rewind__ (غیرعلامتی n)
  {
    اگر (unlikely (n > backwards_لمبائی))
      n = backwards_لمبائی;
    لمبائی += n;
    backwards_لمبائی -= n;
    arrayZ -= n;
  }
  غیرعلامتی __len__ () ثابت { واپس لمبائی; }
  /* Ouch. The operator== compares the contents of the array.  For range-based for loops,
   * it's best if we can just compare arrayZ, though comparing contents is still fast,
   * but also would require that Type has operator==.  As such, we optimize this operator
   * for range-based for loop and just compare arrayZ and length.
   *
   * The above comment is outdated now because we implemented separate begin/end to
   * objects that were using hb_array_t for range-based loop before. */
  منطقی عامل != (ثابت hb_array_t& o) ثابت
  { واپس یہ->arrayZ != o.arrayZ || یہ->لمبائی != o.لمبائی; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () ثابت { واپس arrayZ; }
  Type *انتہا () ثابت { واپس arrayZ + لمبائی; }


  /* Extra operators.
   */
  Type * عامل & () ثابت { واپس arrayZ; }
  عامل hb_array_t<ثابت Type> () { واپس hb_array_t<ثابت Type> (arrayZ, لمبائی); }
  سانچہ <قسم_نام T> عامل T * () ثابت { واپس arrayZ; }

  HB_اندرونی منطقی عامل == (ثابت hb_array_t &o) ثابت;

  uint32_t hash () ثابت
  {
    // FNV-1a hash function
    // https://github.com/harfbuzz/harfbuzz/pull/4228
    uint32_t current = /*cbf29ce4*/0x84222325;
    برائے (خودکار &v : *یہ)
    {
      current = current ^ hb_hash (v);
      current = current * 16777619;
    }
    واپس current;
  }

  /*
   * Compare, Sort, and Search.
   */

  /* Note: our compare is NOT lexicographic; it also does NOT call Type::cmp. */
  صحیح cmp (ثابت hb_array_t &a) ثابت
  {
    اگر (لمبائی != a.لمبائی)
      واپس (صحیح) a.لمبائی - (صحیح) لمبائی;
    واپس hb_memcmp (a.arrayZ, arrayZ, get_سائز ());
  }
  HB_اندرونی ساکن صحیح cmp (ثابت خالی *pa, ثابت خالی *pb)
  {
    hb_array_t *a = (hb_array_t *) pa;
    hb_array_t *b = (hb_array_t *) pb;
    واپس b->cmp (*a);
  }

  سانچہ <قسم_نام T>
  Type *lsearch (ثابت T &x, Type *not_ملا = خالی_اشاریہ)
  {
    غیرعلامتی i;
    واپس lfind (x, &i) ? &یہ->arrayZ[i] : not_ملا;
  }
  سانچہ <قسم_نام T>
  ثابت Type *lsearch (ثابت T &x, ثابت Type *not_ملا = خالی_اشاریہ) ثابت
  {
    غیرعلامتی i;
    واپس lfind (x, &i) ? &یہ->arrayZ[i] : not_ملا;
  }
  سانچہ <قسم_نام T>
  منطقی lfind (ثابت T &x, غیرعلامتی *pos = خالی_اشاریہ,
	      hb_not_ملا_t not_ملا = HB_NOT_ملا_DONT_STORE,
	      غیرعلامتی صحیح مقصد_store = (غیرعلامتی صحیح) -1) ثابت
  {
    برائے (غیرعلامتی i = 0; i < لمبائی; ++i)
      اگر (hb_equal (x, یہ->arrayZ[i]))
      {
	اگر (pos)
	  *pos = i;
	واپس سچ;
      }

    اگر (pos)
    {
      انتخاب (not_ملا)
      {
	حالت HB_NOT_ملا_DONT_STORE:
	  توڑو;

	حالت HB_NOT_ملا_STORE:
	  *pos = مقصد_store;
	  توڑو;

	حالت HB_NOT_ملا_STORE_CLOSEST:
	  *pos = لمبائی;
	  توڑو;
      }
    }
    واپس جھوٹ;
  }

  hb_sorted_array_t<Type> qsort (صحیح (*cmp)(ثابت خالی*, ثابت خالی*))
  {
    اگر (likely (لمبائی))
      hb_qsort (arrayZ, لمبائی, یہ->get_شے_سائز (), cmp);
    واپس hb_sorted_array_t<Type> (*یہ);
  }

  /* Comparator follows the C qsort convention: returns
   * negative / zero / positive int. */
  سانچہ <قسم_نام Compar>
  hb_sorted_array_t<Type> qsort (Compar compar)
  {
    اگر (likely (لمبائی))
      hb_qsort_inline (arrayZ, لمبائی, compar);
    واپس hb_sorted_array_t<Type> (*یہ);
  }

  نجی:
  سانچہ <قسم_نام T = Type,
	    hb_enable_if (std::is_move_assignable<T>::value)>
  hb_sorted_array_t<Type> _qsort (hb_ترجیح<1>)
  {
    واپس qsort ([] (ثابت Type &a, ثابت Type &b) { واپس Type::cmp (&a, &b); });
  }
  hb_sorted_array_t<Type> _qsort (hb_ترجیح<0>)
  {
    واپس qsort ((صحیح(*)(ثابت خالی*, ثابت خالی*)) Type::cmp);
  }
  عوامی:

  hb_sorted_array_t<Type> qsort ()
  {
    واپس _qsort (hb_prioritize);
  }

  /*
   * Other methods.
   */

  size_t get_سائز () ثابت { واپس لمبائی * یہ->get_شے_سائز (); }

  /*
   * Reverse the order of items in this array in the range [start, end).
   */
  خالی reverse (غیرعلامتی آغاز = 0, غیرعلامتی انتہا = -1)
  {
    آغاز = hb_min (آغاز, لمبائی);
    انتہا = hb_min (انتہا, لمبائی);

    اگر (انتہا < آغاز + 2)
      واپس;

    غیرعلامتی stop = آغاز + (انتہا - آغاز) / 2;
    برائے (غیرعلامتی lhs = آغاز, rhs = انتہا - 1; lhs < stop; lhs++, rhs--)
      hb_تبادلہ (arrayZ[rhs], arrayZ[lhs]);
  }

  hb_array_t sub_array (غیرعلامتی صحیح آغاز_انحراف = 0, غیرعلامتی صحیح *seg_گنتی = خالی_اشاریہ /* IN/OUT */) ثابت
  {
    اگر (!آغاز_انحراف && !seg_گنتی)
      واپس *یہ;

    غیرعلامتی صحیح گنتی = لمبائی;
    اگر (unlikely (آغاز_انحراف > گنتی))
      گنتی = 0;
    ورنہ
      گنتی -= آغاز_انحراف;
    اگر (seg_گنتی)
      گنتی = *seg_گنتی = hb_min (گنتی, *seg_گنتی);
    واپس hb_array_t (arrayZ + آغاز_انحراف, گنتی);
  }
  hb_array_t sub_array (غیرعلامتی صحیح آغاز_انحراف, غیرعلامتی صحیح seg_گنتی) ثابت
  { واپس sub_array (آغاز_انحراف, &seg_گنتی); }

  hb_array_t truncate (غیرعلامتی لمبائی) ثابت { واپس sub_array (0, لمبائی); }

  سانچہ <قسم_نام T,
	    غیرعلامتی P = حجم (Type),
	    hb_enable_if (P == 1)>
  ثابت T *as () ثابت
  { واپس لمبائی < hb_min_سائز (T) ? &Null (T) : reinterpret_cast<ثابت T *> (arrayZ); }

  سانچہ <قسم_نام T,
	    غیرعلامتی P = حجم (Type),
	    hb_enable_if (P == 1)>
  منطقی check_range (ثابت T *p, غیرعلامتی صحیح سائز = T::static_سائز) ثابت
  {
    واپس arrayZ <= ((ثابت حرف *) p)
	&& ((ثابت حرف *) p) <= arrayZ + لمبائی
	&& (غیرعلامتی صحیح) (arrayZ + لمبائی - (ثابت حرف *) p) >= سائز;
  }

  سانچہ <غیرعلامتی P = حجم (Type),
	    hb_enable_if (P == 1)>
  منطقی check_انتہا (ثابت خالی *p) ثابت
  {
    واپس (uintptr_t) (((ثابت حرف *) p) - arrayZ) <= لمبائی;
  }

  /* Only call if you allocated the underlying array using hb_malloc() or similar. */
  خالی fini ()
  { hb_free ((خالی *) arrayZ); arrayZ = خالی_اشاریہ; لمبائی = 0; }

  سانچہ <قسم_نام hb_serialize_context_t,
	    قسم_نام U = Type,
	    hb_enable_if (!(حجم (U) < حجم (طویل طویل) && hb_is_trivially_copy_assignable(hb_decay<Type>)))>
  hb_array_t copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (یہ);
    خودکار* برآمد = c->آغاز_embed (arrayZ);
    اگر (unlikely (!c->extend_سائز (برآمد, get_سائز (), جھوٹ))) return_trace (hb_array_t ());
    برائے (غیرعلامتی i = 0; i < لمبائی; i++)
      برآمد[i] = arrayZ[i]; /* TODO: add version that calls c->copy() */
    return_trace (hb_array_t (برآمد, لمبائی));
  }

  سانچہ <قسم_نام hb_serialize_context_t,
	    قسم_نام U = Type,
	    hb_enable_if (حجم (U) < حجم (طویل طویل) && hb_is_trivially_copy_assignable(hb_decay<Type>))>
  hb_array_t copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (یہ);
    خودکار* برآمد = c->آغاز_embed (arrayZ);
    اگر (unlikely (!c->extend_سائز (برآمد, get_سائز (), جھوٹ))) return_trace (hb_array_t ());
    hb_memcpy (برآمد, arrayZ, get_سائز ());
    return_trace (hb_array_t (برآمد, لمبائی));
  }

  سانچہ <قسم_نام hb_sanitize_context_t>
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  { واپس c->check_array (arrayZ, لمبائی); }

  /*
   * Members
   */

  عوامی:
  Type *arrayZ = خالی_اشاریہ;
  غیرعلامتی صحیح لمبائی = 0;
  غیرعلامتی صحیح backwards_لمبائی = 0;
};
سانچہ <قسم_نام T> اندرونی hb_array_t<T>
hb_array ()
{ واپس hb_array_t<T> (); }
سانچہ <قسم_نام T> اندرونی hb_array_t<T>
hb_array (T *array, غیرعلامتی صحیح لمبائی)
{ واپس hb_array_t<T> (array, لمبائی); }
سانچہ <قسم_نام T, غیرعلامتی صحیح لمبائی_2> اندرونی hb_array_t<T>
hb_array (T (&array_)[لمبائی_2])
{ واپس hb_array_t<T> (array_); }

سانچہ <قسم_نام Type>
ساخت hb_sorted_array_t :
	hb_array_t<Type>,
	hb_iter_t<hb_sorted_array_t<Type>, Type&>
{
  قسمتعریف hb_iter_t<hb_sorted_array_t, Type&> iter_بنیاد_t;
  HB_ITER_USING (iter_بنیاد_t);
  ساکن constexpr منطقی is_random_access_iterator = سچ;
  ساکن constexpr منطقی is_sorted_iterator = سچ;
  ساکن constexpr منطقی has_fast_len = سچ;

  hb_sorted_array_t () = طےشدہ;
  hb_sorted_array_t (ثابت hb_sorted_array_t&) = طےشدہ;
  ~hb_sorted_array_t () = طےشدہ;
  hb_sorted_array_t& عامل= (ثابت hb_sorted_array_t&) = طےشدہ;
  hb_sorted_array_t& عامل= (hb_sorted_array_t&&) = طےشدہ;

  constexpr hb_sorted_array_t (Type *array_, غیرعلامتی صحیح لمبائی_2) : hb_array_t<Type> (array_, لمبائی_2) {}
  سانچہ <غیرعلامتی صحیح لمبائی_2>
  constexpr hb_sorted_array_t (Type (&array_)[لمبائی_2]) : hb_array_t<Type> (array_) {}

  سانچہ <قسم_نام U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_sorted_array_t (ثابت hb_array_t<U> &o) :
    hb_array_t<Type> (o),
    hb_iter_t<hb_sorted_array_t, Type&> () {}
  سانچہ <قسم_نام U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_sorted_array_t& عامل = (ثابت hb_array_t<U> &o)
  { hb_array_t<Type> (*یہ) = o; واپس *یہ; }

  /* Iterator implementation. */

  /* See comment in hb_array_of::operator != */
  منطقی عامل != (ثابت hb_sorted_array_t& o) ثابت
  { واپس یہ->arrayZ != o.arrayZ || یہ->لمبائی != o.لمبائی; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () ثابت { واپس یہ->arrayZ; }
  Type *انتہا () ثابت { واپس یہ->arrayZ + یہ->لمبائی; }


  hb_sorted_array_t sub_array (غیرعلامتی صحیح آغاز_انحراف, غیرعلامتی صحیح *seg_گنتی /* IN/OUT */) ثابت
  { واپس hb_sorted_array_t (((ثابت hb_array_t<Type> *) (یہ))->sub_array (آغاز_انحراف, seg_گنتی)); }
  hb_sorted_array_t sub_array (غیرعلامتی صحیح آغاز_انحراف, غیرعلامتی صحیح seg_گنتی) ثابت
  { واپس sub_array (آغاز_انحراف, &seg_گنتی); }

  hb_sorted_array_t truncate (غیرعلامتی لمبائی) ثابت { واپس sub_array (0, لمبائی); }

  سانچہ <قسم_نام T>
  Type *bsearch (ثابت T &x, Type *not_ملا = خالی_اشاریہ)
  {
    غیرعلامتی صحیح i;
    واپس bfind (x, &i) ? &یہ->arrayZ[i] : not_ملا;
  }
  سانچہ <قسم_نام T>
  ثابت Type *bsearch (ثابت T &x, ثابت Type *not_ملا = خالی_اشاریہ) ثابت
  {
    غیرعلامتی صحیح i;
    واپس bfind (x, &i) ? &یہ->arrayZ[i] : not_ملا;
  }
  سانچہ <قسم_نام T>
  منطقی bfind (ثابت T &x, غیرعلامتی صحیح *i = خالی_اشاریہ,
	      hb_not_ملا_t not_ملا = HB_NOT_ملا_DONT_STORE,
	      غیرعلامتی صحیح مقصد_store = (غیرعلامتی صحیح) -1) ثابت
  {
    غیرعلامتی pos;

    اگر (bsearch_impl (x, &pos))
    {
      اگر (i)
	*i = pos;
      واپس سچ;
    }

    اگر (i)
    {
      انتخاب (not_ملا)
      {
	حالت HB_NOT_ملا_DONT_STORE:
	  توڑو;

	حالت HB_NOT_ملا_STORE:
	  *i = مقصد_store;
	  توڑو;

	حالت HB_NOT_ملا_STORE_CLOSEST:
	  *i = pos;
	  توڑو;
      }
    }
    واپس جھوٹ;
  }
  سانچہ <قسم_نام T, قسم_نام ...Ts>
  منطقی bsearch_impl (ثابت T &x, غیرعلامتی *pos, Ts... ds) ثابت
  {
    واپس hb_bsearch_impl (pos,
			    x,
			    یہ->arrayZ,
			    یہ->لمبائی,
			    حجم (Type),
			    hb_cmp_طریقہ<T, Type, Ts...>,
			    std::forward<Ts> (ds)...);
  }
};
سانچہ <قسم_نام T> اندرونی hb_sorted_array_t<T>
hb_sorted_array (T *array, غیرعلامتی صحیح لمبائی)
{ واپس hb_sorted_array_t<T> (array, لمبائی); }
سانچہ <قسم_نام T, غیرعلامتی صحیح لمبائی_2> اندرونی hb_sorted_array_t<T>
hb_sorted_array (T (&array_)[لمبائی_2])
{ واپس hb_sorted_array_t<T> (array_); }

سانچہ <قسم_نام T>
اندرونی منطقی hb_array_t<T>::عامل == (ثابت hb_array_t<T> &o) ثابت
{
  اگر (o.لمبائی != یہ->لمبائی) واپس جھوٹ;
  برائے (غیرعلامتی صحیح i = 0; i < یہ->لمبائی; i++) {
    اگر (یہ->arrayZ[i] != o.arrayZ[i]) واپس جھوٹ;
  }
  واپس سچ;
}
سانچہ <>
اندرونی منطقی hb_array_t<ثابت حرف>::عامل == (ثابت hb_array_t<ثابت حرف> &o) ثابت
{
  اگر (o.لمبائی != یہ->لمبائی) واپس جھوٹ;
  واپس 0 == hb_memcmp (arrayZ, o.arrayZ, لمبائی);
}
سانچہ <>
اندرونی منطقی hb_array_t<ثابت غیرعلامتی حرف>::عامل == (ثابت hb_array_t<ثابت غیرعلامتی حرف> &o) ثابت
{
  اگر (o.لمبائی != یہ->لمبائی) واپس جھوٹ;
  واپس 0 == hb_memcmp (arrayZ, o.arrayZ, لمبائی);
}


/* Specialize hash() for byte arrays. */

#اگر_غیر_تعریف_شدہ HB_OPTIMIZE_سائز_مزید
سانچہ <>
اندرونی uint32_t hb_array_t<ثابت حرف>::hash () ثابت
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  واپس fasthash32(arrayZ, لمبائی, 0xf437ffe6 /* magic? */);
}

سانچہ <>
اندرونی uint32_t hb_array_t<ثابت غیرعلامتی حرف>::hash () ثابت
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  واپس fasthash32(arrayZ, لمبائی, 0xf437ffe6 /* magic? */);
}
#اگر_ختم


قسمتعریف hb_array_t<ثابت حرف> hb_بائٹیں_t;
قسمتعریف hb_array_t<ثابت غیرعلامتی حرف> hb_ubytes_t;



#اگر_ختم /* HB_ARRAY_HH */

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

#جيڪڏهن_وصف_نه_ٿيل HB_ARRAY_HH
#وصف_ڪريو HB_ARRAY_HH

#ضمن "hb.hh"
#ضمن "hb-algs.hh"
#ضمن "hb-iter.hh"
#ضمن "hb-null.hh"


سانچو <قسم_نالو Type>
بنية hb_sorted_array_t;

تعداد hb_not_مليو_t
{
  HB_NOT_مليو_DONT_STORE,
  HB_NOT_مليو_STORE,
  HB_NOT_مليو_STORE_CLOSEST,
};


سانچو <قسم_نالو Type>
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

  constexpr hb_array_t (Type *array_, بنانشان صحيح ڊيگهائي_2) : arrayZ (array_), ڊيگهائي (ڊيگهائي_2) {}
  سانچو <بنانشان صحيح ڊيگهائي_2>
  constexpr hb_array_t (Type (&array_)[ڊيگهائي_2]) : hb_array_t (array_, ڊيگهائي_2) {}

  سانچو <قسم_نالو U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_array_t (ثابت hb_array_t<U> &o) :
    hb_iter_with_fallback_t<hb_array_t, Type&> (),
    arrayZ (o.arrayZ), ڊيگهائي (o.ڊيگهائي), backwards_ڊيگهائي (o.backwards_ڊيگهائي) {}
  سانچو <قسم_نالو U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_array_t& عامل = (ثابت hb_array_t<U> &o)
  { arrayZ = o.arrayZ; ڊيگهائي = o.ڊيگهائي; backwards_ڊيگهائي = o.backwards_ڊيگهائي; أرجع *هذا; }

  /*
   * Iterator implementation.
   */
  تعريفنوع Type& __item_t__;
  ساكن constexpr منطقي is_random_access_iterator = سچ;
  ساكن constexpr منطقي has_fast_len = سچ;
  Type& __item__ () ثابت
  {
    إذا (unlikely (!ڊيگهائي)) أرجع CrapOrNull (Type);
    أرجع *arrayZ;
  }
  Type& __item_at__ (بنانشان i) ثابت
  {
    إذا (unlikely (i >= ڊيگهائي)) أرجع CrapOrNull (Type);
    أرجع arrayZ[i];
  }
  فراغ __next__ ()
  {
    إذا (unlikely (!ڊيگهائي))
      أرجع;
    ڊيگهائي--;
    backwards_ڊيگهائي++;
    arrayZ++;
  }
  فراغ __forward__ (بنانشان n)
  {
    إذا (unlikely (n > ڊيگهائي))
      n = ڊيگهائي;
    ڊيگهائي -= n;
    backwards_ڊيگهائي += n;
    arrayZ += n;
  }
  فراغ __prev__ ()
  {
    إذا (unlikely (!backwards_ڊيگهائي))
      أرجع;
    ڊيگهائي++;
    backwards_ڊيگهائي--;
    arrayZ--;
  }
  فراغ __rewind__ (بنانشان n)
  {
    إذا (unlikely (n > backwards_ڊيگهائي))
      n = backwards_ڊيگهائي;
    ڊيگهائي += n;
    backwards_ڊيگهائي -= n;
    arrayZ -= n;
  }
  بنانشان __len__ () ثابت { أرجع ڊيگهائي; }
  /* Ouch. The operator== compares the contents of the array.  For range-based for loops,
   * it's best if we can just compare arrayZ, though comparing contents is still fast,
   * but also would require that Type has operator==.  As such, we optimize this operator
   * for range-based for loop and just compare arrayZ and length.
   *
   * The above comment is outdated now because we implemented separate begin/end to
   * objects that were using hb_array_t for range-based loop before. */
  منطقي عامل != (ثابت hb_array_t& o) ثابت
  { أرجع هذا->arrayZ != o.arrayZ || هذا->ڊيگهائي != o.ڊيگهائي; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () ثابت { أرجع arrayZ; }
  Type *پڇاڙي () ثابت { أرجع arrayZ + ڊيگهائي; }


  /* Extra operators.
   */
  Type * عامل & () ثابت { أرجع arrayZ; }
  عامل hb_array_t<ثابت Type> () { أرجع hb_array_t<ثابت Type> (arrayZ, ڊيگهائي); }
  سانچو <قسم_نالو T> عامل T * () ثابت { أرجع arrayZ; }

  HB_اندروني منطقي عامل == (ثابت hb_array_t &o) ثابت;

  uint32_t hash () ثابت
  {
    // FNV-1a hash function
    // https://github.com/harfbuzz/harfbuzz/pull/4228
    uint32_t current = /*cbf29ce4*/0x84222325;
    لكل (خودڪار &v : *هذا)
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
    إذا (ڊيگهائي != a.ڊيگهائي)
      أرجع (صحيح) a.ڊيگهائي - (صحيح) ڊيگهائي;
    أرجع hb_memcmp (a.arrayZ, arrayZ, get_سائيز ());
  }
  HB_اندروني ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    hb_array_t *a = (hb_array_t *) pa;
    hb_array_t *b = (hb_array_t *) pb;
    أرجع b->cmp (*a);
  }

  سانچو <قسم_نالو T>
  Type *lsearch (ثابت T &x, Type *not_مليو = خالي_اشارو)
  {
    بنانشان i;
    أرجع lfind (x, &i) ? &هذا->arrayZ[i] : not_مليو;
  }
  سانچو <قسم_نالو T>
  ثابت Type *lsearch (ثابت T &x, ثابت Type *not_مليو = خالي_اشارو) ثابت
  {
    بنانشان i;
    أرجع lfind (x, &i) ? &هذا->arrayZ[i] : not_مليو;
  }
  سانچو <قسم_نالو T>
  منطقي lfind (ثابت T &x, بنانشان *pos = خالي_اشارو,
	      hb_not_مليو_t not_مليو = HB_NOT_مليو_DONT_STORE,
	      بنانشان صحيح منزل_store = (بنانشان صحيح) -1) ثابت
  {
    لكل (بنانشان i = 0; i < ڊيگهائي; ++i)
      إذا (hb_equal (x, هذا->arrayZ[i]))
      {
	إذا (pos)
	  *pos = i;
	أرجع سچ;
      }

    إذا (pos)
    {
      بدل (not_مليو)
      {
	حالة HB_NOT_مليو_DONT_STORE:
	  اكسر;

	حالة HB_NOT_مليو_STORE:
	  *pos = منزل_store;
	  اكسر;

	حالة HB_NOT_مليو_STORE_CLOSEST:
	  *pos = ڊيگهائي;
	  اكسر;
      }
    }
    أرجع خطأ;
  }

  hb_sorted_array_t<Type> qsort (صحيح (*cmp)(ثابت فراغ*, ثابت فراغ*))
  {
    إذا (likely (ڊيگهائي))
      hb_qsort (arrayZ, ڊيگهائي, هذا->get_شي_سائيز (), cmp);
    أرجع hb_sorted_array_t<Type> (*هذا);
  }

  /* Comparator follows the C qsort convention: returns
   * negative / zero / positive int. */
  سانچو <قسم_نالو Compar>
  hb_sorted_array_t<Type> qsort (Compar compar)
  {
    إذا (likely (ڊيگهائي))
      hb_qsort_inline (arrayZ, ڊيگهائي, compar);
    أرجع hb_sorted_array_t<Type> (*هذا);
  }

  خاص:
  سانچو <قسم_نالو T = Type,
	    hb_enable_if (std::is_move_assignable<T>::value)>
  hb_sorted_array_t<Type> _qsort (hb_ترجيح<1>)
  {
    أرجع qsort ([] (ثابت Type &a, ثابت Type &b) { أرجع Type::cmp (&a, &b); });
  }
  hb_sorted_array_t<Type> _qsort (hb_ترجيح<0>)
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

  size_t get_سائيز () ثابت { أرجع ڊيگهائي * هذا->get_شي_سائيز (); }

  /*
   * Reverse the order of items in this array in the range [start, end).
   */
  فراغ reverse (بنانشان شروعات = 0, بنانشان پڇاڙي = -1)
  {
    شروعات = hb_min (شروعات, ڊيگهائي);
    پڇاڙي = hb_min (پڇاڙي, ڊيگهائي);

    إذا (پڇاڙي < شروعات + 2)
      أرجع;

    بنانشان stop = شروعات + (پڇاڙي - شروعات) / 2;
    لكل (بنانشان lhs = شروعات, rhs = پڇاڙي - 1; lhs < stop; lhs++, rhs--)
      hb_مٽا_سٽا (arrayZ[rhs], arrayZ[lhs]);
  }

  hb_array_t sub_array (بنانشان صحيح شروعات_وٿي = 0, بنانشان صحيح *seg_ڳڻپ = خالي_اشارو /* IN/OUT */) ثابت
  {
    إذا (!شروعات_وٿي && !seg_ڳڻپ)
      أرجع *هذا;

    بنانشان صحيح ڳڻپ = ڊيگهائي;
    إذا (unlikely (شروعات_وٿي > ڳڻپ))
      ڳڻپ = 0;
    وإلا
      ڳڻپ -= شروعات_وٿي;
    إذا (seg_ڳڻپ)
      ڳڻپ = *seg_ڳڻپ = hb_min (ڳڻپ, *seg_ڳڻپ);
    أرجع hb_array_t (arrayZ + شروعات_وٿي, ڳڻپ);
  }
  hb_array_t sub_array (بنانشان صحيح شروعات_وٿي, بنانشان صحيح seg_ڳڻپ) ثابت
  { أرجع sub_array (شروعات_وٿي, &seg_ڳڻپ); }

  hb_array_t truncate (بنانشان ڊيگهائي) ثابت { أرجع sub_array (0, ڊيگهائي); }

  سانچو <قسم_نالو T,
	    بنانشان P = حجم (Type),
	    hb_enable_if (P == 1)>
  ثابت T *as () ثابت
  { أرجع ڊيگهائي < hb_min_سائيز (T) ? &Null (T) : reinterpret_cast<ثابت T *> (arrayZ); }

  سانچو <قسم_نالو T,
	    بنانشان P = حجم (Type),
	    hb_enable_if (P == 1)>
  منطقي check_range (ثابت T *p, بنانشان صحيح سائيز = T::static_سائيز) ثابت
  {
    أرجع arrayZ <= ((ثابت حرف *) p)
	&& ((ثابت حرف *) p) <= arrayZ + ڊيگهائي
	&& (بنانشان صحيح) (arrayZ + ڊيگهائي - (ثابت حرف *) p) >= سائيز;
  }

  سانچو <بنانشان P = حجم (Type),
	    hb_enable_if (P == 1)>
  منطقي check_پڇاڙي (ثابت فراغ *p) ثابت
  {
    أرجع (uintptr_t) (((ثابت حرف *) p) - arrayZ) <= ڊيگهائي;
  }

  /* Only call if you allocated the underlying array using hb_malloc() or similar. */
  فراغ fini ()
  { hb_free ((فراغ *) arrayZ); arrayZ = خالي_اشارو; ڊيگهائي = 0; }

  سانچو <قسم_نالو hb_serialize_context_t,
	    قسم_نالو U = Type,
	    hb_enable_if (!(حجم (U) < حجم (ڊگهو ڊگهو) && hb_is_trivially_copy_assignable(hb_decay<Type>)))>
  hb_array_t copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودڪار* ٻاهريون = c->شروعات_embed (arrayZ);
    إذا (unlikely (!c->extend_سائيز (ٻاهريون, get_سائيز (), خطأ))) return_trace (hb_array_t ());
    لكل (بنانشان i = 0; i < ڊيگهائي; i++)
      ٻاهريون[i] = arrayZ[i]; /* TODO: add version that calls c->copy() */
    return_trace (hb_array_t (ٻاهريون, ڊيگهائي));
  }

  سانچو <قسم_نالو hb_serialize_context_t,
	    قسم_نالو U = Type,
	    hb_enable_if (حجم (U) < حجم (ڊگهو ڊگهو) && hb_is_trivially_copy_assignable(hb_decay<Type>))>
  hb_array_t copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودڪار* ٻاهريون = c->شروعات_embed (arrayZ);
    إذا (unlikely (!c->extend_سائيز (ٻاهريون, get_سائيز (), خطأ))) return_trace (hb_array_t ());
    hb_memcpy (ٻاهريون, arrayZ, get_سائيز ());
    return_trace (hb_array_t (ٻاهريون, ڊيگهائي));
  }

  سانچو <قسم_نالو hb_sanitize_context_t>
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  { أرجع c->check_array (arrayZ, ڊيگهائي); }

  /*
   * Members
   */

  عام:
  Type *arrayZ = خالي_اشارو;
  بنانشان صحيح ڊيگهائي = 0;
  بنانشان صحيح backwards_ڊيگهائي = 0;
};
سانچو <قسم_نالو T> مضمن hb_array_t<T>
hb_array ()
{ أرجع hb_array_t<T> (); }
سانچو <قسم_نالو T> مضمن hb_array_t<T>
hb_array (T *array, بنانشان صحيح ڊيگهائي)
{ أرجع hb_array_t<T> (array, ڊيگهائي); }
سانچو <قسم_نالو T, بنانشان صحيح ڊيگهائي_2> مضمن hb_array_t<T>
hb_array (T (&array_)[ڊيگهائي_2])
{ أرجع hb_array_t<T> (array_); }

سانچو <قسم_نالو Type>
بنية hb_sorted_array_t :
	hb_array_t<Type>,
	hb_iter_t<hb_sorted_array_t<Type>, Type&>
{
  تعريفنوع hb_iter_t<hb_sorted_array_t, Type&> iter_بنياد_t;
  HB_ITER_USING (iter_بنياد_t);
  ساكن constexpr منطقي is_random_access_iterator = سچ;
  ساكن constexpr منطقي is_sorted_iterator = سچ;
  ساكن constexpr منطقي has_fast_len = سچ;

  hb_sorted_array_t () = افتراضي;
  hb_sorted_array_t (ثابت hb_sorted_array_t&) = افتراضي;
  ~hb_sorted_array_t () = افتراضي;
  hb_sorted_array_t& عامل= (ثابت hb_sorted_array_t&) = افتراضي;
  hb_sorted_array_t& عامل= (hb_sorted_array_t&&) = افتراضي;

  constexpr hb_sorted_array_t (Type *array_, بنانشان صحيح ڊيگهائي_2) : hb_array_t<Type> (array_, ڊيگهائي_2) {}
  سانچو <بنانشان صحيح ڊيگهائي_2>
  constexpr hb_sorted_array_t (Type (&array_)[ڊيگهائي_2]) : hb_array_t<Type> (array_) {}

  سانچو <قسم_نالو U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_sorted_array_t (ثابت hb_array_t<U> &o) :
    hb_array_t<Type> (o),
    hb_iter_t<hb_sorted_array_t, Type&> () {}
  سانچو <قسم_نالو U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_sorted_array_t& عامل = (ثابت hb_array_t<U> &o)
  { hb_array_t<Type> (*هذا) = o; أرجع *هذا; }

  /* Iterator implementation. */

  /* See comment in hb_array_of::operator != */
  منطقي عامل != (ثابت hb_sorted_array_t& o) ثابت
  { أرجع هذا->arrayZ != o.arrayZ || هذا->ڊيگهائي != o.ڊيگهائي; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () ثابت { أرجع هذا->arrayZ; }
  Type *پڇاڙي () ثابت { أرجع هذا->arrayZ + هذا->ڊيگهائي; }


  hb_sorted_array_t sub_array (بنانشان صحيح شروعات_وٿي, بنانشان صحيح *seg_ڳڻپ /* IN/OUT */) ثابت
  { أرجع hb_sorted_array_t (((ثابت hb_array_t<Type> *) (هذا))->sub_array (شروعات_وٿي, seg_ڳڻپ)); }
  hb_sorted_array_t sub_array (بنانشان صحيح شروعات_وٿي, بنانشان صحيح seg_ڳڻپ) ثابت
  { أرجع sub_array (شروعات_وٿي, &seg_ڳڻپ); }

  hb_sorted_array_t truncate (بنانشان ڊيگهائي) ثابت { أرجع sub_array (0, ڊيگهائي); }

  سانچو <قسم_نالو T>
  Type *bsearch (ثابت T &x, Type *not_مليو = خالي_اشارو)
  {
    بنانشان صحيح i;
    أرجع bfind (x, &i) ? &هذا->arrayZ[i] : not_مليو;
  }
  سانچو <قسم_نالو T>
  ثابت Type *bsearch (ثابت T &x, ثابت Type *not_مليو = خالي_اشارو) ثابت
  {
    بنانشان صحيح i;
    أرجع bfind (x, &i) ? &هذا->arrayZ[i] : not_مليو;
  }
  سانچو <قسم_نالو T>
  منطقي bfind (ثابت T &x, بنانشان صحيح *i = خالي_اشارو,
	      hb_not_مليو_t not_مليو = HB_NOT_مليو_DONT_STORE,
	      بنانشان صحيح منزل_store = (بنانشان صحيح) -1) ثابت
  {
    بنانشان pos;

    إذا (bsearch_impl (x, &pos))
    {
      إذا (i)
	*i = pos;
      أرجع سچ;
    }

    إذا (i)
    {
      بدل (not_مليو)
      {
	حالة HB_NOT_مليو_DONT_STORE:
	  اكسر;

	حالة HB_NOT_مليو_STORE:
	  *i = منزل_store;
	  اكسر;

	حالة HB_NOT_مليو_STORE_CLOSEST:
	  *i = pos;
	  اكسر;
      }
    }
    أرجع خطأ;
  }
  سانچو <قسم_نالو T, قسم_نالو ...Ts>
  منطقي bsearch_impl (ثابت T &x, بنانشان *pos, Ts... ds) ثابت
  {
    أرجع hb_bsearch_impl (pos,
			    x,
			    هذا->arrayZ,
			    هذا->ڊيگهائي,
			    حجم (Type),
			    hb_cmp_طريقو<T, Type, Ts...>,
			    std::forward<Ts> (ds)...);
  }
};
سانچو <قسم_نالو T> مضمن hb_sorted_array_t<T>
hb_sorted_array (T *array, بنانشان صحيح ڊيگهائي)
{ أرجع hb_sorted_array_t<T> (array, ڊيگهائي); }
سانچو <قسم_نالو T, بنانشان صحيح ڊيگهائي_2> مضمن hb_sorted_array_t<T>
hb_sorted_array (T (&array_)[ڊيگهائي_2])
{ أرجع hb_sorted_array_t<T> (array_); }

سانچو <قسم_نالو T>
مضمن منطقي hb_array_t<T>::عامل == (ثابت hb_array_t<T> &o) ثابت
{
  إذا (o.ڊيگهائي != هذا->ڊيگهائي) أرجع خطأ;
  لكل (بنانشان صحيح i = 0; i < هذا->ڊيگهائي; i++) {
    إذا (هذا->arrayZ[i] != o.arrayZ[i]) أرجع خطأ;
  }
  أرجع سچ;
}
سانچو <>
مضمن منطقي hb_array_t<ثابت حرف>::عامل == (ثابت hb_array_t<ثابت حرف> &o) ثابت
{
  إذا (o.ڊيگهائي != هذا->ڊيگهائي) أرجع خطأ;
  أرجع 0 == hb_memcmp (arrayZ, o.arrayZ, ڊيگهائي);
}
سانچو <>
مضمن منطقي hb_array_t<ثابت بنانشان حرف>::عامل == (ثابت hb_array_t<ثابت بنانشان حرف> &o) ثابت
{
  إذا (o.ڊيگهائي != هذا->ڊيگهائي) أرجع خطأ;
  أرجع 0 == hb_memcmp (arrayZ, o.arrayZ, ڊيگهائي);
}


/* Specialize hash() for byte arrays. */

#جيڪڏهن_وصف_نه_ٿيل HB_OPTIMIZE_سائيز_وڌيڪ
سانچو <>
مضمن uint32_t hb_array_t<ثابت حرف>::hash () ثابت
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  أرجع fasthash32(arrayZ, ڊيگهائي, 0xf437ffe6 /* magic? */);
}

سانچو <>
مضمن uint32_t hb_array_t<ثابت بنانشان حرف>::hash () ثابت
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  أرجع fasthash32(arrayZ, ڊيگهائي, 0xf437ffe6 /* magic? */);
}
#جيڪڏهن_پورو


تعريفنوع hb_array_t<ثابت حرف> hb_بائيٽون_t;
تعريفنوع hb_array_t<ثابت بنانشان حرف> hb_ubytes_t;



#جيڪڏهن_پورو /* HB_ARRAY_HH */

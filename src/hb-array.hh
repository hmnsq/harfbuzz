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

#که_نه_تعریف_شوی HB_ARRAY_HH
#تعریف_کړه HB_ARRAY_HH

#ضمن "hb.hh"
#ضمن "hb-algs.hh"
#ضمن "hb-iter.hh"
#ضمن "hb-null.hh"


کينډۍ <ډول_نوم Type>
بنية hb_sorted_array_t;

تعداد hb_not_وموندل_t
{
  HB_NOT_وموندل_DONT_STORE,
  HB_NOT_وموندل_STORE,
  HB_NOT_وموندل_STORE_CLOSEST,
};


کينډۍ <ډول_نوم Type>
بنية hb_array_t : hb_iter_with_fallback_t<hb_array_t<Type>, Type&>
{
  ساكن constexpr منطقي realloc_move = ریښتیا;

  /*
   * Constructors.
   */
  hb_array_t () = افتراضي;
  hb_array_t (ثابت hb_array_t&) = افتراضي;
  ~hb_array_t () = افتراضي;
  hb_array_t& چلوونکی= (ثابت hb_array_t&) = افتراضي;
  hb_array_t& چلوونکی= (hb_array_t&&) = افتراضي;

  constexpr hb_array_t (Type *array_, بینښه صحيح اوږدتیا_2) : arrayZ (array_), اوږدتیا (اوږدتیا_2) {}
  کينډۍ <بینښه صحيح اوږدتیا_2>
  constexpr hb_array_t (Type (&array_)[اوږدتیا_2]) : hb_array_t (array_, اوږدتیا_2) {}

  کينډۍ <ډول_نوم U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_array_t (ثابت hb_array_t<U> &o) :
    hb_iter_with_fallback_t<hb_array_t, Type&> (),
    arrayZ (o.arrayZ), اوږدتیا (o.اوږدتیا), backwards_اوږدتیا (o.backwards_اوږدتیا) {}
  کينډۍ <ډول_نوم U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_array_t& چلوونکی = (ثابت hb_array_t<U> &o)
  { arrayZ = o.arrayZ; اوږدتیا = o.اوږدتیا; backwards_اوږدتیا = o.backwards_اوږدتیا; أرجع *هذا; }

  /*
   * Iterator implementation.
   */
  تعريفنوع Type& __item_t__;
  ساكن constexpr منطقي is_random_access_iterator = ریښتیا;
  ساكن constexpr منطقي has_fast_len = ریښتیا;
  Type& __item__ () ثابت
  {
    إذا (unlikely (!اوږدتیا)) أرجع CrapOrNull (Type);
    أرجع *arrayZ;
  }
  Type& __item_at__ (بینښه i) ثابت
  {
    إذا (unlikely (i >= اوږدتیا)) أرجع CrapOrNull (Type);
    أرجع arrayZ[i];
  }
  فراغ __next__ ()
  {
    إذا (unlikely (!اوږدتیا))
      أرجع;
    اوږدتیا--;
    backwards_اوږدتیا++;
    arrayZ++;
  }
  فراغ __forward__ (بینښه n)
  {
    إذا (unlikely (n > اوږدتیا))
      n = اوږدتیا;
    اوږدتیا -= n;
    backwards_اوږدتیا += n;
    arrayZ += n;
  }
  فراغ __prev__ ()
  {
    إذا (unlikely (!backwards_اوږدتیا))
      أرجع;
    اوږدتیا++;
    backwards_اوږدتیا--;
    arrayZ--;
  }
  فراغ __rewind__ (بینښه n)
  {
    إذا (unlikely (n > backwards_اوږدتیا))
      n = backwards_اوږدتیا;
    اوږدتیا += n;
    backwards_اوږدتیا -= n;
    arrayZ -= n;
  }
  بینښه __len__ () ثابت { أرجع اوږدتیا; }
  /* Ouch. The operator== compares the contents of the array.  For range-based for loops,
   * it's best if we can just compare arrayZ, though comparing contents is still fast,
   * but also would require that Type has operator==.  As such, we optimize this operator
   * for range-based for loop and just compare arrayZ and length.
   *
   * The above comment is outdated now because we implemented separate begin/end to
   * objects that were using hb_array_t for range-based loop before. */
  منطقي چلوونکی != (ثابت hb_array_t& o) ثابت
  { أرجع هذا->arrayZ != o.arrayZ || هذا->اوږدتیا != o.اوږدتیا; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () ثابت { أرجع arrayZ; }
  Type *پای_ټکی () ثابت { أرجع arrayZ + اوږدتیا; }


  /* Extra operators.
   */
  Type * چلوونکی & () ثابت { أرجع arrayZ; }
  چلوونکی hb_array_t<ثابت Type> () { أرجع hb_array_t<ثابت Type> (arrayZ, اوږدتیا); }
  کينډۍ <ډول_نوم T> چلوونکی T * () ثابت { أرجع arrayZ; }

  HB_دننی منطقي چلوونکی == (ثابت hb_array_t &o) ثابت;

  uint32_t hash () ثابت
  {
    // FNV-1a hash function
    // https://github.com/harfbuzz/harfbuzz/pull/4228
    uint32_t current = /*cbf29ce4*/0x84222325;
    لكل (خپلکاری &v : *هذا)
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
    إذا (اوږدتیا != a.اوږدتیا)
      أرجع (صحيح) a.اوږدتیا - (صحيح) اوږدتیا;
    أرجع hb_memcmp (a.arrayZ, arrayZ, get_اندازه ());
  }
  HB_دننی ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    hb_array_t *a = (hb_array_t *) pa;
    hb_array_t *b = (hb_array_t *) pb;
    أرجع b->cmp (*a);
  }

  کينډۍ <ډول_نوم T>
  Type *lsearch (ثابت T &x, Type *not_وموندل = تش_اشاره)
  {
    بینښه i;
    أرجع lfind (x, &i) ? &هذا->arrayZ[i] : not_وموندل;
  }
  کينډۍ <ډول_نوم T>
  ثابت Type *lsearch (ثابت T &x, ثابت Type *not_وموندل = تش_اشاره) ثابت
  {
    بینښه i;
    أرجع lfind (x, &i) ? &هذا->arrayZ[i] : not_وموندل;
  }
  کينډۍ <ډول_نوم T>
  منطقي lfind (ثابت T &x, بینښه *pos = تش_اشاره,
	      hb_not_وموندل_t not_وموندل = HB_NOT_وموندل_DONT_STORE,
	      بینښه صحيح منزل_store = (بینښه صحيح) -1) ثابت
  {
    لكل (بینښه i = 0; i < اوږدتیا; ++i)
      إذا (hb_equal (x, هذا->arrayZ[i]))
      {
	إذا (pos)
	  *pos = i;
	أرجع ریښتیا;
      }

    إذا (pos)
    {
      بدل (not_وموندل)
      {
	حالة HB_NOT_وموندل_DONT_STORE:
	  اكسر;

	حالة HB_NOT_وموندل_STORE:
	  *pos = منزل_store;
	  اكسر;

	حالة HB_NOT_وموندل_STORE_CLOSEST:
	  *pos = اوږدتیا;
	  اكسر;
      }
    }
    أرجع خطأ;
  }

  hb_sorted_array_t<Type> qsort (صحيح (*cmp)(ثابت فراغ*, ثابت فراغ*))
  {
    إذا (likely (اوږدتیا))
      hb_qsort (arrayZ, اوږدتیا, هذا->get_توکی_اندازه (), cmp);
    أرجع hb_sorted_array_t<Type> (*هذا);
  }

  /* Comparator follows the C qsort convention: returns
   * negative / zero / positive int. */
  کينډۍ <ډول_نوم Compar>
  hb_sorted_array_t<Type> qsort (Compar compar)
  {
    إذا (likely (اوږدتیا))
      hb_qsort_inline (arrayZ, اوږدتیا, compar);
    أرجع hb_sorted_array_t<Type> (*هذا);
  }

  خاص:
  کينډۍ <ډول_نوم T = Type,
	    hb_enable_if (std::is_move_assignable<T>::value)>
  hb_sorted_array_t<Type> _qsort (hb_لومړیتوب<1>)
  {
    أرجع qsort ([] (ثابت Type &a, ثابت Type &b) { أرجع Type::cmp (&a, &b); });
  }
  hb_sorted_array_t<Type> _qsort (hb_لومړیتوب<0>)
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

  size_t get_اندازه () ثابت { أرجع اوږدتیا * هذا->get_توکی_اندازه (); }

  /*
   * Reverse the order of items in this array in the range [start, end).
   */
  فراغ reverse (بینښه پیل = 0, بینښه پای_ټکی = -1)
  {
    پیل = hb_min (پیل, اوږدتیا);
    پای_ټکی = hb_min (پای_ټکی, اوږدتیا);

    إذا (پای_ټکی < پیل + 2)
      أرجع;

    بینښه stop = پیل + (پای_ټکی - پیل) / 2;
    لكل (بینښه lhs = پیل, rhs = پای_ټکی - 1; lhs < stop; lhs++, rhs--)
      hb_بدلون (arrayZ[rhs], arrayZ[lhs]);
  }

  hb_array_t sub_array (بینښه صحيح پیل_دننه_واټن = 0, بینښه صحيح *seg_شمېر = تش_اشاره /* IN/OUT */) ثابت
  {
    إذا (!پیل_دننه_واټن && !seg_شمېر)
      أرجع *هذا;

    بینښه صحيح شمېر = اوږدتیا;
    إذا (unlikely (پیل_دننه_واټن > شمېر))
      شمېر = 0;
    وإلا
      شمېر -= پیل_دننه_واټن;
    إذا (seg_شمېر)
      شمېر = *seg_شمېر = hb_min (شمېر, *seg_شمېر);
    أرجع hb_array_t (arrayZ + پیل_دننه_واټن, شمېر);
  }
  hb_array_t sub_array (بینښه صحيح پیل_دننه_واټن, بینښه صحيح seg_شمېر) ثابت
  { أرجع sub_array (پیل_دننه_واټن, &seg_شمېر); }

  hb_array_t truncate (بینښه اوږدتیا) ثابت { أرجع sub_array (0, اوږدتیا); }

  کينډۍ <ډول_نوم T,
	    بینښه P = حجم (Type),
	    hb_enable_if (P == 1)>
  ثابت T *as () ثابت
  { أرجع اوږدتیا < hb_min_اندازه (T) ? &Null (T) : reinterpret_cast<ثابت T *> (arrayZ); }

  کينډۍ <ډول_نوم T,
	    بینښه P = حجم (Type),
	    hb_enable_if (P == 1)>
  منطقي check_range (ثابت T *p, بینښه صحيح اندازه = T::static_اندازه) ثابت
  {
    أرجع arrayZ <= ((ثابت حرف *) p)
	&& ((ثابت حرف *) p) <= arrayZ + اوږدتیا
	&& (بینښه صحيح) (arrayZ + اوږدتیا - (ثابت حرف *) p) >= اندازه;
  }

  کينډۍ <بینښه P = حجم (Type),
	    hb_enable_if (P == 1)>
  منطقي check_پای_ټکی (ثابت فراغ *p) ثابت
  {
    أرجع (uintptr_t) (((ثابت حرف *) p) - arrayZ) <= اوږدتیا;
  }

  /* Only call if you allocated the underlying array using hb_malloc() or similar. */
  فراغ fini ()
  { hb_free ((فراغ *) arrayZ); arrayZ = تش_اشاره; اوږدتیا = 0; }

  کينډۍ <ډول_نوم hb_serialize_context_t,
	    ډول_نوم U = Type,
	    hb_enable_if (!(حجم (U) < حجم (اوږد اوږد) && hb_is_trivially_copy_assignable(hb_decay<Type>)))>
  hb_array_t copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خپلکاری* بهر = c->پیل_embed (arrayZ);
    إذا (unlikely (!c->extend_اندازه (بهر, get_اندازه (), خطأ))) return_trace (hb_array_t ());
    لكل (بینښه i = 0; i < اوږدتیا; i++)
      بهر[i] = arrayZ[i]; /* TODO: add version that calls c->copy() */
    return_trace (hb_array_t (بهر, اوږدتیا));
  }

  کينډۍ <ډول_نوم hb_serialize_context_t,
	    ډول_نوم U = Type,
	    hb_enable_if (حجم (U) < حجم (اوږد اوږد) && hb_is_trivially_copy_assignable(hb_decay<Type>))>
  hb_array_t copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خپلکاری* بهر = c->پیل_embed (arrayZ);
    إذا (unlikely (!c->extend_اندازه (بهر, get_اندازه (), خطأ))) return_trace (hb_array_t ());
    hb_memcpy (بهر, arrayZ, get_اندازه ());
    return_trace (hb_array_t (بهر, اوږدتیا));
  }

  کينډۍ <ډول_نوم hb_sanitize_context_t>
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  { أرجع c->check_array (arrayZ, اوږدتیا); }

  /*
   * Members
   */

  عام:
  Type *arrayZ = تش_اشاره;
  بینښه صحيح اوږدتیا = 0;
  بینښه صحيح backwards_اوږدتیا = 0;
};
کينډۍ <ډول_نوم T> مضمن hb_array_t<T>
hb_array ()
{ أرجع hb_array_t<T> (); }
کينډۍ <ډول_نوم T> مضمن hb_array_t<T>
hb_array (T *array, بینښه صحيح اوږدتیا)
{ أرجع hb_array_t<T> (array, اوږدتیا); }
کينډۍ <ډول_نوم T, بینښه صحيح اوږدتیا_2> مضمن hb_array_t<T>
hb_array (T (&array_)[اوږدتیا_2])
{ أرجع hb_array_t<T> (array_); }

کينډۍ <ډول_نوم Type>
بنية hb_sorted_array_t :
	hb_array_t<Type>,
	hb_iter_t<hb_sorted_array_t<Type>, Type&>
{
  تعريفنوع hb_iter_t<hb_sorted_array_t, Type&> iter_بنسټ_t;
  HB_ITER_USING (iter_بنسټ_t);
  ساكن constexpr منطقي is_random_access_iterator = ریښتیا;
  ساكن constexpr منطقي is_sorted_iterator = ریښتیا;
  ساكن constexpr منطقي has_fast_len = ریښتیا;

  hb_sorted_array_t () = افتراضي;
  hb_sorted_array_t (ثابت hb_sorted_array_t&) = افتراضي;
  ~hb_sorted_array_t () = افتراضي;
  hb_sorted_array_t& چلوونکی= (ثابت hb_sorted_array_t&) = افتراضي;
  hb_sorted_array_t& چلوونکی= (hb_sorted_array_t&&) = افتراضي;

  constexpr hb_sorted_array_t (Type *array_, بینښه صحيح اوږدتیا_2) : hb_array_t<Type> (array_, اوږدتیا_2) {}
  کينډۍ <بینښه صحيح اوږدتیا_2>
  constexpr hb_sorted_array_t (Type (&array_)[اوږدتیا_2]) : hb_array_t<Type> (array_) {}

  کينډۍ <ډول_نوم U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_sorted_array_t (ثابت hb_array_t<U> &o) :
    hb_array_t<Type> (o),
    hb_iter_t<hb_sorted_array_t, Type&> () {}
  کينډۍ <ډول_نوم U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_sorted_array_t& چلوونکی = (ثابت hb_array_t<U> &o)
  { hb_array_t<Type> (*هذا) = o; أرجع *هذا; }

  /* Iterator implementation. */

  /* See comment in hb_array_of::operator != */
  منطقي چلوونکی != (ثابت hb_sorted_array_t& o) ثابت
  { أرجع هذا->arrayZ != o.arrayZ || هذا->اوږدتیا != o.اوږدتیا; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () ثابت { أرجع هذا->arrayZ; }
  Type *پای_ټکی () ثابت { أرجع هذا->arrayZ + هذا->اوږدتیا; }


  hb_sorted_array_t sub_array (بینښه صحيح پیل_دننه_واټن, بینښه صحيح *seg_شمېر /* IN/OUT */) ثابت
  { أرجع hb_sorted_array_t (((ثابت hb_array_t<Type> *) (هذا))->sub_array (پیل_دننه_واټن, seg_شمېر)); }
  hb_sorted_array_t sub_array (بینښه صحيح پیل_دننه_واټن, بینښه صحيح seg_شمېر) ثابت
  { أرجع sub_array (پیل_دننه_واټن, &seg_شمېر); }

  hb_sorted_array_t truncate (بینښه اوږدتیا) ثابت { أرجع sub_array (0, اوږدتیا); }

  کينډۍ <ډول_نوم T>
  Type *bsearch (ثابت T &x, Type *not_وموندل = تش_اشاره)
  {
    بینښه صحيح i;
    أرجع bfind (x, &i) ? &هذا->arrayZ[i] : not_وموندل;
  }
  کينډۍ <ډول_نوم T>
  ثابت Type *bsearch (ثابت T &x, ثابت Type *not_وموندل = تش_اشاره) ثابت
  {
    بینښه صحيح i;
    أرجع bfind (x, &i) ? &هذا->arrayZ[i] : not_وموندل;
  }
  کينډۍ <ډول_نوم T>
  منطقي bfind (ثابت T &x, بینښه صحيح *i = تش_اشاره,
	      hb_not_وموندل_t not_وموندل = HB_NOT_وموندل_DONT_STORE,
	      بینښه صحيح منزل_store = (بینښه صحيح) -1) ثابت
  {
    بینښه pos;

    إذا (bsearch_impl (x, &pos))
    {
      إذا (i)
	*i = pos;
      أرجع ریښتیا;
    }

    إذا (i)
    {
      بدل (not_وموندل)
      {
	حالة HB_NOT_وموندل_DONT_STORE:
	  اكسر;

	حالة HB_NOT_وموندل_STORE:
	  *i = منزل_store;
	  اكسر;

	حالة HB_NOT_وموندل_STORE_CLOSEST:
	  *i = pos;
	  اكسر;
      }
    }
    أرجع خطأ;
  }
  کينډۍ <ډول_نوم T, ډول_نوم ...Ts>
  منطقي bsearch_impl (ثابت T &x, بینښه *pos, Ts... ds) ثابت
  {
    أرجع hb_bsearch_impl (pos,
			    x,
			    هذا->arrayZ,
			    هذا->اوږدتیا,
			    حجم (Type),
			    hb_cmp_لاره<T, Type, Ts...>,
			    std::forward<Ts> (ds)...);
  }
};
کينډۍ <ډول_نوم T> مضمن hb_sorted_array_t<T>
hb_sorted_array (T *array, بینښه صحيح اوږدتیا)
{ أرجع hb_sorted_array_t<T> (array, اوږدتیا); }
کينډۍ <ډول_نوم T, بینښه صحيح اوږدتیا_2> مضمن hb_sorted_array_t<T>
hb_sorted_array (T (&array_)[اوږدتیا_2])
{ أرجع hb_sorted_array_t<T> (array_); }

کينډۍ <ډول_نوم T>
مضمن منطقي hb_array_t<T>::چلوونکی == (ثابت hb_array_t<T> &o) ثابت
{
  إذا (o.اوږدتیا != هذا->اوږدتیا) أرجع خطأ;
  لكل (بینښه صحيح i = 0; i < هذا->اوږدتیا; i++) {
    إذا (هذا->arrayZ[i] != o.arrayZ[i]) أرجع خطأ;
  }
  أرجع ریښتیا;
}
کينډۍ <>
مضمن منطقي hb_array_t<ثابت حرف>::چلوونکی == (ثابت hb_array_t<ثابت حرف> &o) ثابت
{
  إذا (o.اوږدتیا != هذا->اوږدتیا) أرجع خطأ;
  أرجع 0 == hb_memcmp (arrayZ, o.arrayZ, اوږدتیا);
}
کينډۍ <>
مضمن منطقي hb_array_t<ثابت بینښه حرف>::چلوونکی == (ثابت hb_array_t<ثابت بینښه حرف> &o) ثابت
{
  إذا (o.اوږدتیا != هذا->اوږدتیا) أرجع خطأ;
  أرجع 0 == hb_memcmp (arrayZ, o.arrayZ, اوږدتیا);
}


/* Specialize hash() for byte arrays. */

#که_نه_تعریف_شوی HB_OPTIMIZE_اندازه_نور
کينډۍ <>
مضمن uint32_t hb_array_t<ثابت حرف>::hash () ثابت
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  أرجع fasthash32(arrayZ, اوږدتیا, 0xf437ffe6 /* magic? */);
}

کينډۍ <>
مضمن uint32_t hb_array_t<ثابت بینښه حرف>::hash () ثابت
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  أرجع fasthash32(arrayZ, اوږدتیا, 0xf437ffe6 /* magic? */);
}
#د_که_پای


تعريفنوع hb_array_t<ثابت حرف> hb_بایټونه_t;
تعريفنوع hb_array_t<ثابت بینښه حرف> hb_ubytes_t;



#د_که_پای /* HB_ARRAY_HH */

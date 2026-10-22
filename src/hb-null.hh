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

#که_نه_تعریف_شوی HB_NULL_HH
#تعریف_کړه HB_NULL_HH

#ضمن "hb.hh"
#ضمن "hb-meta.hh"


/*
 * Static pools
 */

/* Global nul-content Null pool.  Enlarge as necessary. */

#تعریف_کړه HB_NULL_POOL_اندازه 640

کينډۍ <ډول_نوم T, ډول_نوم>
بنية hb_has_min_اندازه : hb_false_type {};
کينډۍ <ډول_نوم T>
بنية hb_has_min_اندازه<T, hb_void_t<decltype (T::min_اندازه)>>
	: hb_true_type {};
کينډۍ <ډول_نوم T>
کارول hb_has_min_اندازه_2 = hb_has_min_اندازه<T, فراغ>;
#تعریف_کړه hb_has_min_اندازه_2(T) hb_has_min_اندازه_2<T>::value

کينډۍ <ډول_نوم T, ډول_نوم>
بنية hb_has_null_اندازه : hb_false_type {};
کينډۍ <ډول_نوم T>
بنية hb_has_null_اندازه<T, hb_void_t<decltype (T::null_اندازه)>>
	: hb_true_type {};
کينډۍ <ډول_نوم T>
کارول hb_has_null_اندازه_2 = hb_has_null_اندازه<T, فراغ>;
#تعریف_کړه hb_has_null_اندازه_2(T) hb_has_null_اندازه_2<T>::value

/* Use SFINAE to sniff whether T has min_size; in which case return the larger
 * of sizeof(T) and T::null_size, otherwise return sizeof(T).
 *
 * The main purpose of this is to let structs communicate that they are not nullable,
 * by defining min_size but *not* null_size. */

/* The hard way...
 * https://stackoverflow.com/questions/7776448/sfinae-tried-with-bool-gives-compiler-error-template-argument-tvalue-invol
 */

کينډۍ <ډول_نوم T, ډول_نوم>
بنية hb_null_اندازه : hb_integral_constant<بینښه, حجم (T)> {};
کينډۍ <ډول_نوم T>
بنية hb_null_اندازه<T, hb_void_t<decltype (T::min_اندازه)>>
	: hb_integral_constant<بینښه,
			       (حجم (T) > T::null_اندازه ? حجم (T) : T::null_اندازه)> {};
کينډۍ <ډول_نوم T>
کارول hb_null_اندازه_2 = hb_null_اندازه<T, فراغ>;
#تعریف_کړه hb_null_اندازه_2(T) hb_null_اندازه_2<T>::value

/* These doesn't belong here, but since is copy/paste from above, put it here. */

/* hb_static_size (T)
 * Returns T::static_size if T::min_size is defined, or sizeof (T) otherwise. */

کينډۍ <ډول_نوم T, ډول_نوم>
بنية hb_static_اندازه_2 : hb_integral_constant<بینښه, حجم (T)> {};
کينډۍ <ډول_نوم T>
بنية hb_static_اندازه_2<T, hb_void_t<decltype (T::static_اندازه)>> : hb_integral_constant<بینښه, T::static_اندازه> {};
کينډۍ <ډول_نوم T>
کارول hb_static_اندازه = hb_static_اندازه_2<T, فراغ>;
#تعریف_کړه hb_static_اندازه(T) hb_static_اندازه<T>::value

کينډۍ <ډول_نوم T, ډول_نوم>
بنية hb_min_اندازه_2 : hb_integral_constant<بینښه, حجم (T)> {};
کينډۍ <ډول_نوم T>
بنية hb_min_اندازه_2<T, hb_void_t<decltype (T::min_اندازه)>> : hb_integral_constant<بینښه, T::min_اندازه> {};
کينډۍ <ډول_نوم T>
کارول hb_min_اندازه = hb_min_اندازه_2<T, فراغ>;
#تعریف_کړه hb_min_اندازه(T) hb_min_اندازه<T>::value


/*
 * Null()
 */

خارجي HB_دننی
uint64_t ثابت _hb_NullPool[(HB_NULL_POOL_اندازه + حجم (uint64_t) - 1) / حجم (uint64_t)];

/* Generic nul-content Null objects. */
کينډۍ <ډول_نوم Type>
بنية Null {
  ساكن Type ثابت & get_null ()
  {
    static_assert (hb_null_اندازه_2 (Type) <= HB_NULL_POOL_اندازه, "Increase HB_NULL_POOL_SIZE.");
    أرجع *reinterpret_cast<Type ثابت *> (_hb_NullPool);
  }
};
کينډۍ <ډول_نوم QType>
بنية NullHelper
{
  تعريفنوع hb_remove_const<hb_remove_reference<QType>> Type;
  ساكن ثابت Type & get_null () { أرجع Null<Type>::get_null (); }
};
#تعریف_کړه Null(Type) NullHelper<Type>::get_null ()

/* Specializations for arbitrary-content Null objects expressed in bytes. */
#تعریف_کړه DECLARE_NULL_NAMESPACE_بایټونه(Namespace, Type) \
	} /* Close namespace. */ \
	خارجي HB_دننی ثابت بینښه حرف _hb_Null_##Namespace##_##Type[hb_null_اندازه_2 (Namespace::Type)]; \
	کينډۍ <> \
	بنية Null<Namespace::Type> { \
	  ساكن Namespace::Type ثابت & get_null () { \
	    أرجع *reinterpret_cast<ثابت Namespace::Type *> (_hb_Null_##Namespace##_##Type); \
	  } \
	}; \
	نطاق Namespace { \
	static_assert (ریښتیا, "") /* Require semicolon after. */
#تعریف_کړه DECLARE_NULL_NAMESPACE_بایټونه_TEMPLATE1(Namespace, Type, اندازه_2) \
	} /* Close namespace. */ \
	خارجي HB_دننی ثابت بینښه حرف _hb_Null_##Namespace##_##Type[اندازه_2]; \
	کينډۍ <ډول_نوم Spec> \
	بنية Null<Namespace::Type<Spec>> { \
	  ساكن Namespace::Type<Spec> ثابت & get_null () { \
	    أرجع *reinterpret_cast<ثابت Namespace::Type<Spec> *> (_hb_Null_##Namespace##_##Type); \
	  } \
	}; \
	نطاق Namespace { \
	static_assert (ریښتیا, "") /* Require semicolon after. */
#تعریف_کړه DEFINE_NULL_NAMESPACE_بایټونه(Namespace, Type) \
	ثابت بینښه حرف _hb_Null_##Namespace##_##Type[حجم (_hb_Null_##Namespace##_##Type)]

/* Specializations for arbitrary-content Null objects expressed as struct initializer. */
#تعریف_کړه DECLARE_NULL_INSTANCE(Type) \
	خارجي HB_دننی ثابت Type _hb_Null_##Type; \
	کينډۍ <> \
	بنية Null<Type> { \
	  ساكن Type ثابت & get_null () { \
	    أرجع _hb_Null_##Type; \
	  } \
	}; \
	static_assert (ریښتیا, "") /* Require semicolon after. */
#تعریف_کړه DEFINE_NULL_INSTANCE(Type) \
	ثابت Type _hb_Null_##Type

/* Global writable pool.  Enlarge as necessary. */

/* To be fully correct, CrapPool must be thread_local. However, we do not rely on CrapPool
 * for correct operation. It only exist to catch and divert program logic bugs instead of
 * causing bad memory access. So, races there are not actually introducing incorrectness
 * in the code. Has ~12kb binary size overhead to have it, also clang build fails with it. */
خارجي HB_دننی
/*thread_local*/ uint64_t _hb_CrapPool[(HB_NULL_POOL_اندازه + حجم (uint64_t) - 1) / حجم (uint64_t)];

/* CRAP pool: Common Region for Access Protection. */
کينډۍ <ډول_نوم Type>
ساكن مضمن Type& Crap () {
  static_assert (hb_null_اندازه_2 (Type) <= HB_NULL_POOL_اندازه, "Increase HB_NULL_POOL_SIZE.");
  Type *obj = reinterpret_cast<Type *> (_hb_CrapPool);
  حافظه_نقل (reinterpret_cast<فراغ*>(obj), std::addressof (Null (Type)), حجم (*obj));
  أرجع *obj;
}
کينډۍ <ډول_نوم QType>
بنية CrapHelper
{
  تعريفنوع hb_remove_const<hb_remove_reference<QType>> Type;
  ساكن Type & get_crap () { أرجع Crap<Type> (); }
};
#تعریف_کړه Crap(Type) CrapHelper<Type>::get_crap ()

کينډۍ <ډول_نوم Type>
بنية CrapOrNullHelper {
  ساكن Type & get () { أرجع Crap (Type); }
};
کينډۍ <ډول_نوم Type>
بنية CrapOrNullHelper<ثابت Type> {
  ساكن ثابت Type & get () { أرجع Null (Type); }
};
#تعریف_کړه CrapOrNull(Type) CrapOrNullHelper<Type>::get ()


/*
 * hb_nonnull_ptr_t
 */

کينډۍ <ډول_نوم P>
بنية hb_nonnull_ptr_t
{
  تعريفنوع hb_remove_اشاره_ګر<P> T;

  hb_nonnull_ptr_t (T *v_ = تش_اشاره) : v (v_) {}
  T * چلوونکی = (T *v_)   { أرجع v = v_; }
  T * چلوونکی -> () ثابت { أرجع get (); }
  T & چلوونکی * () ثابت  { أرجع *get (); }
  T ** چلوونکی & () ثابت { أرجع std::addressof (v); }
  /* Only auto-cast to const types. */
  کينډۍ <ډول_نوم C> چلوونکی ثابت C * () ثابت { أرجع get (); }
  چلوونکی ثابت حرف * () ثابت { أرجع (ثابت حرف *) get (); }
  T * get () ثابت { أرجع v ? v : const_cast<T *> (std::addressof (Null (T))); }
  T * get_raw () ثابت { أرجع v; }

  خاص:
  T *v;
};


#د_که_پای /* HB_NULL_HH */

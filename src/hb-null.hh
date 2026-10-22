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

#جيڪڏهن_وصف_نه_ٿيل HB_NULL_HH
#وصف_ڪريو HB_NULL_HH

#ضمن "hb.hh"
#ضمن "hb-meta.hh"


/*
 * Static pools
 */

/* Global nul-content Null pool.  Enlarge as necessary. */

#وصف_ڪريو HB_NULL_POOL_سائيز 640

سانچو <قسم_نالو T, قسم_نالو>
بنية hb_has_min_سائيز : hb_false_type {};
سانچو <قسم_نالو T>
بنية hb_has_min_سائيز<T, hb_void_t<decltype (T::min_سائيز)>>
	: hb_true_type {};
سانچو <قسم_نالو T>
استعمال hb_has_min_سائيز_2 = hb_has_min_سائيز<T, فراغ>;
#وصف_ڪريو hb_has_min_سائيز_2(T) hb_has_min_سائيز_2<T>::value

سانچو <قسم_نالو T, قسم_نالو>
بنية hb_has_null_سائيز : hb_false_type {};
سانچو <قسم_نالو T>
بنية hb_has_null_سائيز<T, hb_void_t<decltype (T::null_سائيز)>>
	: hb_true_type {};
سانچو <قسم_نالو T>
استعمال hb_has_null_سائيز_2 = hb_has_null_سائيز<T, فراغ>;
#وصف_ڪريو hb_has_null_سائيز_2(T) hb_has_null_سائيز_2<T>::value

/* Use SFINAE to sniff whether T has min_size; in which case return the larger
 * of sizeof(T) and T::null_size, otherwise return sizeof(T).
 *
 * The main purpose of this is to let structs communicate that they are not nullable,
 * by defining min_size but *not* null_size. */

/* The hard way...
 * https://stackoverflow.com/questions/7776448/sfinae-tried-with-bool-gives-compiler-error-template-argument-tvalue-invol
 */

سانچو <قسم_نالو T, قسم_نالو>
بنية hb_null_سائيز : hb_integral_constant<بنانشان, حجم (T)> {};
سانچو <قسم_نالو T>
بنية hb_null_سائيز<T, hb_void_t<decltype (T::min_سائيز)>>
	: hb_integral_constant<بنانشان,
			       (حجم (T) > T::null_سائيز ? حجم (T) : T::null_سائيز)> {};
سانچو <قسم_نالو T>
استعمال hb_null_سائيز_2 = hb_null_سائيز<T, فراغ>;
#وصف_ڪريو hb_null_سائيز_2(T) hb_null_سائيز_2<T>::value

/* These doesn't belong here, but since is copy/paste from above, put it here. */

/* hb_static_size (T)
 * Returns T::static_size if T::min_size is defined, or sizeof (T) otherwise. */

سانچو <قسم_نالو T, قسم_نالو>
بنية hb_static_سائيز_2 : hb_integral_constant<بنانشان, حجم (T)> {};
سانچو <قسم_نالو T>
بنية hb_static_سائيز_2<T, hb_void_t<decltype (T::static_سائيز)>> : hb_integral_constant<بنانشان, T::static_سائيز> {};
سانچو <قسم_نالو T>
استعمال hb_static_سائيز = hb_static_سائيز_2<T, فراغ>;
#وصف_ڪريو hb_static_سائيز(T) hb_static_سائيز<T>::value

سانچو <قسم_نالو T, قسم_نالو>
بنية hb_min_سائيز_2 : hb_integral_constant<بنانشان, حجم (T)> {};
سانچو <قسم_نالو T>
بنية hb_min_سائيز_2<T, hb_void_t<decltype (T::min_سائيز)>> : hb_integral_constant<بنانشان, T::min_سائيز> {};
سانچو <قسم_نالو T>
استعمال hb_min_سائيز = hb_min_سائيز_2<T, فراغ>;
#وصف_ڪريو hb_min_سائيز(T) hb_min_سائيز<T>::value


/*
 * Null()
 */

خارجي HB_اندروني
uint64_t ثابت _hb_NullPool[(HB_NULL_POOL_سائيز + حجم (uint64_t) - 1) / حجم (uint64_t)];

/* Generic nul-content Null objects. */
سانچو <قسم_نالو Type>
بنية Null {
  ساكن Type ثابت & get_null ()
  {
    static_assert (hb_null_سائيز_2 (Type) <= HB_NULL_POOL_سائيز, "Increase HB_NULL_POOL_SIZE.");
    أرجع *reinterpret_cast<Type ثابت *> (_hb_NullPool);
  }
};
سانچو <قسم_نالو QType>
بنية NullHelper
{
  تعريفنوع hb_remove_const<hb_remove_reference<QType>> Type;
  ساكن ثابت Type & get_null () { أرجع Null<Type>::get_null (); }
};
#وصف_ڪريو Null(Type) NullHelper<Type>::get_null ()

/* Specializations for arbitrary-content Null objects expressed in bytes. */
#وصف_ڪريو DECLARE_NULL_NAMESPACE_بائيٽون(Namespace, Type) \
	} /* Close namespace. */ \
	خارجي HB_اندروني ثابت بنانشان حرف _hb_Null_##Namespace##_##Type[hb_null_سائيز_2 (Namespace::Type)]; \
	سانچو <> \
	بنية Null<Namespace::Type> { \
	  ساكن Namespace::Type ثابت & get_null () { \
	    أرجع *reinterpret_cast<ثابت Namespace::Type *> (_hb_Null_##Namespace##_##Type); \
	  } \
	}; \
	نطاق Namespace { \
	static_assert (سچ, "") /* Require semicolon after. */
#وصف_ڪريو DECLARE_NULL_NAMESPACE_بائيٽون_TEMPLATE1(Namespace, Type, سائيز_2) \
	} /* Close namespace. */ \
	خارجي HB_اندروني ثابت بنانشان حرف _hb_Null_##Namespace##_##Type[سائيز_2]; \
	سانچو <قسم_نالو Spec> \
	بنية Null<Namespace::Type<Spec>> { \
	  ساكن Namespace::Type<Spec> ثابت & get_null () { \
	    أرجع *reinterpret_cast<ثابت Namespace::Type<Spec> *> (_hb_Null_##Namespace##_##Type); \
	  } \
	}; \
	نطاق Namespace { \
	static_assert (سچ, "") /* Require semicolon after. */
#وصف_ڪريو DEFINE_NULL_NAMESPACE_بائيٽون(Namespace, Type) \
	ثابت بنانشان حرف _hb_Null_##Namespace##_##Type[حجم (_hb_Null_##Namespace##_##Type)]

/* Specializations for arbitrary-content Null objects expressed as struct initializer. */
#وصف_ڪريو DECLARE_NULL_INSTANCE(Type) \
	خارجي HB_اندروني ثابت Type _hb_Null_##Type; \
	سانچو <> \
	بنية Null<Type> { \
	  ساكن Type ثابت & get_null () { \
	    أرجع _hb_Null_##Type; \
	  } \
	}; \
	static_assert (سچ, "") /* Require semicolon after. */
#وصف_ڪريو DEFINE_NULL_INSTANCE(Type) \
	ثابت Type _hb_Null_##Type

/* Global writable pool.  Enlarge as necessary. */

/* To be fully correct, CrapPool must be thread_local. However, we do not rely on CrapPool
 * for correct operation. It only exist to catch and divert program logic bugs instead of
 * causing bad memory access. So, races there are not actually introducing incorrectness
 * in the code. Has ~12kb binary size overhead to have it, also clang build fails with it. */
خارجي HB_اندروني
/*thread_local*/ uint64_t _hb_CrapPool[(HB_NULL_POOL_سائيز + حجم (uint64_t) - 1) / حجم (uint64_t)];

/* CRAP pool: Common Region for Access Protection. */
سانچو <قسم_نالو Type>
ساكن مضمن Type& Crap () {
  static_assert (hb_null_سائيز_2 (Type) <= HB_NULL_POOL_سائيز, "Increase HB_NULL_POOL_SIZE.");
  Type *obj = reinterpret_cast<Type *> (_hb_CrapPool);
  يادگيري_نقل (reinterpret_cast<فراغ*>(obj), std::addressof (Null (Type)), حجم (*obj));
  أرجع *obj;
}
سانچو <قسم_نالو QType>
بنية CrapHelper
{
  تعريفنوع hb_remove_const<hb_remove_reference<QType>> Type;
  ساكن Type & get_crap () { أرجع Crap<Type> (); }
};
#وصف_ڪريو Crap(Type) CrapHelper<Type>::get_crap ()

سانچو <قسم_نالو Type>
بنية CrapOrNullHelper {
  ساكن Type & get () { أرجع Crap (Type); }
};
سانچو <قسم_نالو Type>
بنية CrapOrNullHelper<ثابت Type> {
  ساكن ثابت Type & get () { أرجع Null (Type); }
};
#وصف_ڪريو CrapOrNull(Type) CrapOrNullHelper<Type>::get ()


/*
 * hb_nonnull_ptr_t
 */

سانچو <قسم_نالو P>
بنية hb_nonnull_ptr_t
{
  تعريفنوع hb_remove_اشارو<P> T;

  hb_nonnull_ptr_t (T *v_ = خالي_اشارو) : v (v_) {}
  T * عامل = (T *v_)   { أرجع v = v_; }
  T * عامل -> () ثابت { أرجع get (); }
  T & عامل * () ثابت  { أرجع *get (); }
  T ** عامل & () ثابت { أرجع std::addressof (v); }
  /* Only auto-cast to const types. */
  سانچو <قسم_نالو C> عامل ثابت C * () ثابت { أرجع get (); }
  عامل ثابت حرف * () ثابت { أرجع (ثابت حرف *) get (); }
  T * get () ثابت { أرجع v ? v : const_cast<T *> (std::addressof (Null (T))); }
  T * get_raw () ثابت { أرجع v; }

  خاص:
  T *v;
};


#جيڪڏهن_پورو /* HB_NULL_HH */

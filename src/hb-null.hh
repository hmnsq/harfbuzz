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

#اگر_غیر_تعریف_شدہ HB_NULL_HH
#تعریف_کرو HB_NULL_HH

#شامل "hb.hh"
#شامل "hb-meta.hh"


/*
 * Static pools
 */

/* Global nul-content Null pool.  Enlarge as necessary. */

#تعریف_کرو HB_NULL_POOL_سائز 640

سانچہ <قسم_نام T, قسم_نام>
ساخت hb_has_min_سائز : hb_false_type {};
سانچہ <قسم_نام T>
ساخت hb_has_min_سائز<T, hb_void_t<decltype (T::min_سائز)>>
	: hb_true_type {};
سانچہ <قسم_نام T>
استعمال hb_has_min_سائز_2 = hb_has_min_سائز<T, خالی>;
#تعریف_کرو hb_has_min_سائز_2(T) hb_has_min_سائز_2<T>::value

سانچہ <قسم_نام T, قسم_نام>
ساخت hb_has_null_سائز : hb_false_type {};
سانچہ <قسم_نام T>
ساخت hb_has_null_سائز<T, hb_void_t<decltype (T::null_سائز)>>
	: hb_true_type {};
سانچہ <قسم_نام T>
استعمال hb_has_null_سائز_2 = hb_has_null_سائز<T, خالی>;
#تعریف_کرو hb_has_null_سائز_2(T) hb_has_null_سائز_2<T>::value

/* Use SFINAE to sniff whether T has min_size; in which case return the larger
 * of sizeof(T) and T::null_size, otherwise return sizeof(T).
 *
 * The main purpose of this is to let structs communicate that they are not nullable,
 * by defining min_size but *not* null_size. */

/* The hard way...
 * https://stackoverflow.com/questions/7776448/sfinae-tried-with-bool-gives-compiler-error-template-argument-tvalue-invol
 */

سانچہ <قسم_نام T, قسم_نام>
ساخت hb_null_سائز : hb_integral_constant<غیرعلامتی, حجم (T)> {};
سانچہ <قسم_نام T>
ساخت hb_null_سائز<T, hb_void_t<decltype (T::min_سائز)>>
	: hb_integral_constant<غیرعلامتی,
			       (حجم (T) > T::null_سائز ? حجم (T) : T::null_سائز)> {};
سانچہ <قسم_نام T>
استعمال hb_null_سائز_2 = hb_null_سائز<T, خالی>;
#تعریف_کرو hb_null_سائز_2(T) hb_null_سائز_2<T>::value

/* These doesn't belong here, but since is copy/paste from above, put it here. */

/* hb_static_size (T)
 * Returns T::static_size if T::min_size is defined, or sizeof (T) otherwise. */

سانچہ <قسم_نام T, قسم_نام>
ساخت hb_static_سائز_2 : hb_integral_constant<غیرعلامتی, حجم (T)> {};
سانچہ <قسم_نام T>
ساخت hb_static_سائز_2<T, hb_void_t<decltype (T::static_سائز)>> : hb_integral_constant<غیرعلامتی, T::static_سائز> {};
سانچہ <قسم_نام T>
استعمال hb_static_سائز = hb_static_سائز_2<T, خالی>;
#تعریف_کرو hb_static_سائز(T) hb_static_سائز<T>::value

سانچہ <قسم_نام T, قسم_نام>
ساخت hb_min_سائز_2 : hb_integral_constant<غیرعلامتی, حجم (T)> {};
سانچہ <قسم_نام T>
ساخت hb_min_سائز_2<T, hb_void_t<decltype (T::min_سائز)>> : hb_integral_constant<غیرعلامتی, T::min_سائز> {};
سانچہ <قسم_نام T>
استعمال hb_min_سائز = hb_min_سائز_2<T, خالی>;
#تعریف_کرو hb_min_سائز(T) hb_min_سائز<T>::value


/*
 * Null()
 */

خارجی HB_اندرونی
uint64_t ثابت _hb_NullPool[(HB_NULL_POOL_سائز + حجم (uint64_t) - 1) / حجم (uint64_t)];

/* Generic nul-content Null objects. */
سانچہ <قسم_نام Type>
ساخت Null {
  ساکن Type ثابت & get_null ()
  {
    static_assert (hb_null_سائز_2 (Type) <= HB_NULL_POOL_سائز, "Increase HB_NULL_POOL_SIZE.");
    واپس *reinterpret_cast<Type ثابت *> (_hb_NullPool);
  }
};
سانچہ <قسم_نام QType>
ساخت NullHelper
{
  قسمتعریف hb_remove_const<hb_remove_reference<QType>> Type;
  ساکن ثابت Type & get_null () { واپس Null<Type>::get_null (); }
};
#تعریف_کرو Null(Type) NullHelper<Type>::get_null ()

/* Specializations for arbitrary-content Null objects expressed in bytes. */
#تعریف_کرو DECLARE_NULL_NAMESPACE_بائٹیں(Namespace, Type) \
	} /* Close namespace. */ \
	خارجی HB_اندرونی ثابت غیرعلامتی حرف _hb_Null_##Namespace##_##Type[hb_null_سائز_2 (Namespace::Type)]; \
	سانچہ <> \
	ساخت Null<Namespace::Type> { \
	  ساکن Namespace::Type ثابت & get_null () { \
	    واپس *reinterpret_cast<ثابت Namespace::Type *> (_hb_Null_##Namespace##_##Type); \
	  } \
	}; \
	نامخلا Namespace { \
	static_assert (سچ, "") /* Require semicolon after. */
#تعریف_کرو DECLARE_NULL_NAMESPACE_بائٹیں_TEMPLATE1(Namespace, Type, سائز_2) \
	} /* Close namespace. */ \
	خارجی HB_اندرونی ثابت غیرعلامتی حرف _hb_Null_##Namespace##_##Type[سائز_2]; \
	سانچہ <قسم_نام Spec> \
	ساخت Null<Namespace::Type<Spec>> { \
	  ساکن Namespace::Type<Spec> ثابت & get_null () { \
	    واپس *reinterpret_cast<ثابت Namespace::Type<Spec> *> (_hb_Null_##Namespace##_##Type); \
	  } \
	}; \
	نامخلا Namespace { \
	static_assert (سچ, "") /* Require semicolon after. */
#تعریف_کرو DEFINE_NULL_NAMESPACE_بائٹیں(Namespace, Type) \
	ثابت غیرعلامتی حرف _hb_Null_##Namespace##_##Type[حجم (_hb_Null_##Namespace##_##Type)]

/* Specializations for arbitrary-content Null objects expressed as struct initializer. */
#تعریف_کرو DECLARE_NULL_INSTANCE(Type) \
	خارجی HB_اندرونی ثابت Type _hb_Null_##Type; \
	سانچہ <> \
	ساخت Null<Type> { \
	  ساکن Type ثابت & get_null () { \
	    واپس _hb_Null_##Type; \
	  } \
	}; \
	static_assert (سچ, "") /* Require semicolon after. */
#تعریف_کرو DEFINE_NULL_INSTANCE(Type) \
	ثابت Type _hb_Null_##Type

/* Global writable pool.  Enlarge as necessary. */

/* To be fully correct, CrapPool must be thread_local. However, we do not rely on CrapPool
 * for correct operation. It only exist to catch and divert program logic bugs instead of
 * causing bad memory access. So, races there are not actually introducing incorrectness
 * in the code. Has ~12kb binary size overhead to have it, also clang build fails with it. */
خارجی HB_اندرونی
/*thread_local*/ uint64_t _hb_CrapPool[(HB_NULL_POOL_سائز + حجم (uint64_t) - 1) / حجم (uint64_t)];

/* CRAP pool: Common Region for Access Protection. */
سانچہ <قسم_نام Type>
ساکن اندرونی Type& Crap () {
  static_assert (hb_null_سائز_2 (Type) <= HB_NULL_POOL_سائز, "Increase HB_NULL_POOL_SIZE.");
  Type *obj = reinterpret_cast<Type *> (_hb_CrapPool);
  حافظہ_نقل (reinterpret_cast<خالی*>(obj), std::addressof (Null (Type)), حجم (*obj));
  واپس *obj;
}
سانچہ <قسم_نام QType>
ساخت CrapHelper
{
  قسمتعریف hb_remove_const<hb_remove_reference<QType>> Type;
  ساکن Type & get_crap () { واپس Crap<Type> (); }
};
#تعریف_کرو Crap(Type) CrapHelper<Type>::get_crap ()

سانچہ <قسم_نام Type>
ساخت CrapOrNullHelper {
  ساکن Type & get () { واپس Crap (Type); }
};
سانچہ <قسم_نام Type>
ساخت CrapOrNullHelper<ثابت Type> {
  ساکن ثابت Type & get () { واپس Null (Type); }
};
#تعریف_کرو CrapOrNull(Type) CrapOrNullHelper<Type>::get ()


/*
 * hb_nonnull_ptr_t
 */

سانچہ <قسم_نام P>
ساخت hb_nonnull_ptr_t
{
  قسمتعریف hb_remove_اشارہ_گر<P> T;

  hb_nonnull_ptr_t (T *v_ = خالی_اشاریہ) : v (v_) {}
  T * عامل = (T *v_)   { واپس v = v_; }
  T * عامل -> () ثابت { واپس get (); }
  T & عامل * () ثابت  { واپس *get (); }
  T ** عامل & () ثابت { واپس std::addressof (v); }
  /* Only auto-cast to const types. */
  سانچہ <قسم_نام C> عامل ثابت C * () ثابت { واپس get (); }
  عامل ثابت حرف * () ثابت { واپس (ثابت حرف *) get (); }
  T * get () ثابت { واپس v ? v : const_cast<T *> (std::addressof (Null (T))); }
  T * get_raw () ثابت { واپس v; }

  نجی:
  T *v;
};


#اگر_ختم /* HB_NULL_HH */

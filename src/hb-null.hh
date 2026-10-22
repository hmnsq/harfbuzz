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

#اگر_تعریف_نشده HB_NULL_HH
#تعریف_کن HB_NULL_HH

#ضمن "hb.hh"
#ضمن "hb-meta.hh"


/*
 * Static pools
 */

/* Global nul-content Null pool.  Enlarge as necessary. */

#تعریف_کن HB_NULL_POOL_اندازه 640

قالب <نام_نوع T, نام_نوع>
بنية hb_has_min_اندازه : hb_false_type {};
قالب <نام_نوع T>
بنية hb_has_min_اندازه<T, hb_void_t<decltype (T::min_اندازه)>>
	: hb_true_type {};
قالب <نام_نوع T>
استفاده hb_has_min_اندازه_2 = hb_has_min_اندازه<T, فراغ>;
#تعریف_کن hb_has_min_اندازه_2(T) hb_has_min_اندازه_2<T>::value

قالب <نام_نوع T, نام_نوع>
بنية hb_has_null_اندازه : hb_false_type {};
قالب <نام_نوع T>
بنية hb_has_null_اندازه<T, hb_void_t<decltype (T::null_اندازه)>>
	: hb_true_type {};
قالب <نام_نوع T>
استفاده hb_has_null_اندازه_2 = hb_has_null_اندازه<T, فراغ>;
#تعریف_کن hb_has_null_اندازه_2(T) hb_has_null_اندازه_2<T>::value

/* Use SFINAE to sniff whether T has min_size; in which case return the larger
 * of sizeof(T) and T::null_size, otherwise return sizeof(T).
 *
 * The main purpose of this is to let structs communicate that they are not nullable,
 * by defining min_size but *not* null_size. */

/* The hard way...
 * https://stackoverflow.com/questions/7776448/sfinae-tried-with-bool-gives-compiler-error-template-argument-tvalue-invol
 */

قالب <نام_نوع T, نام_نوع>
بنية hb_null_اندازه : hb_integral_constant<بیعلامت, حجم (T)> {};
قالب <نام_نوع T>
بنية hb_null_اندازه<T, hb_void_t<decltype (T::min_اندازه)>>
	: hb_integral_constant<بیعلامت,
			       (حجم (T) > T::null_اندازه ? حجم (T) : T::null_اندازه)> {};
قالب <نام_نوع T>
استفاده hb_null_اندازه_2 = hb_null_اندازه<T, فراغ>;
#تعریف_کن hb_null_اندازه_2(T) hb_null_اندازه_2<T>::value

/* These doesn't belong here, but since is copy/paste from above, put it here. */

/* hb_static_size (T)
 * Returns T::static_size if T::min_size is defined, or sizeof (T) otherwise. */

قالب <نام_نوع T, نام_نوع>
بنية hb_static_اندازه_2 : hb_integral_constant<بیعلامت, حجم (T)> {};
قالب <نام_نوع T>
بنية hb_static_اندازه_2<T, hb_void_t<decltype (T::static_اندازه)>> : hb_integral_constant<بیعلامت, T::static_اندازه> {};
قالب <نام_نوع T>
استفاده hb_static_اندازه = hb_static_اندازه_2<T, فراغ>;
#تعریف_کن hb_static_اندازه(T) hb_static_اندازه<T>::value

قالب <نام_نوع T, نام_نوع>
بنية hb_min_اندازه_2 : hb_integral_constant<بیعلامت, حجم (T)> {};
قالب <نام_نوع T>
بنية hb_min_اندازه_2<T, hb_void_t<decltype (T::min_اندازه)>> : hb_integral_constant<بیعلامت, T::min_اندازه> {};
قالب <نام_نوع T>
استفاده hb_min_اندازه = hb_min_اندازه_2<T, فراغ>;
#تعریف_کن hb_min_اندازه(T) hb_min_اندازه<T>::value


/*
 * Null()
 */

خارجي HB_داخلی
uint64_t ثابت _hb_NullPool[(HB_NULL_POOL_اندازه + حجم (uint64_t) - 1) / حجم (uint64_t)];

/* Generic nul-content Null objects. */
قالب <نام_نوع Type>
بنية Null {
  ساكن Type ثابت & get_null ()
  {
    static_assert (hb_null_اندازه_2 (Type) <= HB_NULL_POOL_اندازه, "Increase HB_NULL_POOL_SIZE.");
    أرجع *reinterpret_cast<Type ثابت *> (_hb_NullPool);
  }
};
قالب <نام_نوع QType>
بنية NullHelper
{
  تعريفنوع hb_remove_const<hb_remove_reference<QType>> Type;
  ساكن ثابت Type & get_null () { أرجع Null<Type>::get_null (); }
};
#تعریف_کن Null(Type) NullHelper<Type>::get_null ()

/* Specializations for arbitrary-content Null objects expressed in bytes. */
#تعریف_کن DECLARE_NULL_NAMESPACE_بایتها(Namespace, Type) \
	} /* Close namespace. */ \
	خارجي HB_داخلی ثابت بیعلامت حرف _hb_Null_##Namespace##_##Type[hb_null_اندازه_2 (Namespace::Type)]; \
	قالب <> \
	بنية Null<Namespace::Type> { \
	  ساكن Namespace::Type ثابت & get_null () { \
	    أرجع *reinterpret_cast<ثابت Namespace::Type *> (_hb_Null_##Namespace##_##Type); \
	  } \
	}; \
	نطاق Namespace { \
	static_assert (درست, "") /* Require semicolon after. */
#تعریف_کن DECLARE_NULL_NAMESPACE_بایتها_TEMPLATE1(Namespace, Type, اندازه_2) \
	} /* Close namespace. */ \
	خارجي HB_داخلی ثابت بیعلامت حرف _hb_Null_##Namespace##_##Type[اندازه_2]; \
	قالب <نام_نوع Spec> \
	بنية Null<Namespace::Type<Spec>> { \
	  ساكن Namespace::Type<Spec> ثابت & get_null () { \
	    أرجع *reinterpret_cast<ثابت Namespace::Type<Spec> *> (_hb_Null_##Namespace##_##Type); \
	  } \
	}; \
	نطاق Namespace { \
	static_assert (درست, "") /* Require semicolon after. */
#تعریف_کن DEFINE_NULL_NAMESPACE_بایتها(Namespace, Type) \
	ثابت بیعلامت حرف _hb_Null_##Namespace##_##Type[حجم (_hb_Null_##Namespace##_##Type)]

/* Specializations for arbitrary-content Null objects expressed as struct initializer. */
#تعریف_کن DECLARE_NULL_INSTANCE(Type) \
	خارجي HB_داخلی ثابت Type _hb_Null_##Type; \
	قالب <> \
	بنية Null<Type> { \
	  ساكن Type ثابت & get_null () { \
	    أرجع _hb_Null_##Type; \
	  } \
	}; \
	static_assert (درست, "") /* Require semicolon after. */
#تعریف_کن DEFINE_NULL_INSTANCE(Type) \
	ثابت Type _hb_Null_##Type

/* Global writable pool.  Enlarge as necessary. */

/* To be fully correct, CrapPool must be thread_local. However, we do not rely on CrapPool
 * for correct operation. It only exist to catch and divert program logic bugs instead of
 * causing bad memory access. So, races there are not actually introducing incorrectness
 * in the code. Has ~12kb binary size overhead to have it, also clang build fails with it. */
خارجي HB_داخلی
/*thread_local*/ uint64_t _hb_CrapPool[(HB_NULL_POOL_اندازه + حجم (uint64_t) - 1) / حجم (uint64_t)];

/* CRAP pool: Common Region for Access Protection. */
قالب <نام_نوع Type>
ساكن مضمن Type& Crap () {
  static_assert (hb_null_اندازه_2 (Type) <= HB_NULL_POOL_اندازه, "Increase HB_NULL_POOL_SIZE.");
  Type *obj = reinterpret_cast<Type *> (_hb_CrapPool);
  کپی_حافظه (reinterpret_cast<فراغ*>(obj), std::addressof (Null (Type)), حجم (*obj));
  أرجع *obj;
}
قالب <نام_نوع QType>
بنية CrapHelper
{
  تعريفنوع hb_remove_const<hb_remove_reference<QType>> Type;
  ساكن Type & get_crap () { أرجع Crap<Type> (); }
};
#تعریف_کن Crap(Type) CrapHelper<Type>::get_crap ()

قالب <نام_نوع Type>
بنية CrapOrNullHelper {
  ساكن Type & get () { أرجع Crap (Type); }
};
قالب <نام_نوع Type>
بنية CrapOrNullHelper<ثابت Type> {
  ساكن ثابت Type & get () { أرجع Null (Type); }
};
#تعریف_کن CrapOrNull(Type) CrapOrNullHelper<Type>::get ()


/*
 * hb_nonnull_ptr_t
 */

قالب <نام_نوع P>
بنية hb_nonnull_ptr_t
{
  تعريفنوع hb_remove_اشاره_گر<P> T;

  hb_nonnull_ptr_t (T *v_ = اشاره_تهی) : v (v_) {}
  T * عملگر = (T *v_)   { أرجع v = v_; }
  T * عملگر -> () ثابت { أرجع get (); }
  T & عملگر * () ثابت  { أرجع *get (); }
  T ** عملگر & () ثابت { أرجع std::addressof (v); }
  /* Only auto-cast to const types. */
  قالب <نام_نوع C> عملگر ثابت C * () ثابت { أرجع get (); }
  عملگر ثابت حرف * () ثابت { أرجع (ثابت حرف *) get (); }
  T * get () ثابت { أرجع v ? v : const_cast<T *> (std::addressof (Null (T))); }
  T * get_raw () ثابت { أرجع v; }

  خاص:
  T *v;
};


#پایان_اگر /* HB_NULL_HH */

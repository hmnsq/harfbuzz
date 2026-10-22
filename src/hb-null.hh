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

#ifndef HB_NULL_HH
#define HB_NULL_HH

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-meta.hh"


/*
 * Static pools
 */

/* Global nul-content Null pool.  Enlarge as necessary. */

#define HB_NULL_POOL_আকার 640

template <typename T, typename>
কাঠামো hb_has_min_আকার : hb_false_type {};
template <typename T>
কাঠামো hb_has_min_আকার<T, hb_void_t<decltype (T::min_আকার)>>
	: hb_true_type {};
template <typename T>
using hb_has_min_আকার_2 = hb_has_min_আকার<T, শূন্য>;
#define hb_has_min_আকার_2(T) hb_has_min_আকার_2<T>::value

template <typename T, typename>
কাঠামো hb_has_null_আকার : hb_false_type {};
template <typename T>
কাঠামো hb_has_null_আকার<T, hb_void_t<decltype (T::null_আকার)>>
	: hb_true_type {};
template <typename T>
using hb_has_null_আকার_2 = hb_has_null_আকার<T, শূন্য>;
#define hb_has_null_আকার_2(T) hb_has_null_আকার_2<T>::value

/* Use SFINAE to sniff whether T has min_size; in which case return the larger
 * of sizeof(T) and T::null_size, otherwise return sizeof(T).
 *
 * The main purpose of this is to let structs communicate that they are not nullable,
 * by defining min_size but *not* null_size. */

/* The hard way...
 * https://stackoverflow.com/questions/7776448/sfinae-tried-with-bool-gives-compiler-error-template-argument-tvalue-invol
 */

template <typename T, typename>
কাঠামো hb_null_আকার : hb_integral_constant<unsigned, আকার (T)> {};
template <typename T>
কাঠামো hb_null_আকার<T, hb_void_t<decltype (T::min_আকার)>>
	: hb_integral_constant<unsigned,
			       (আকার (T) > T::null_আকার ? আকার (T) : T::null_আকার)> {};
template <typename T>
using hb_null_আকার_2 = hb_null_আকার<T, শূন্য>;
#define hb_null_আকার_2(T) hb_null_আকার_2<T>::value

/* These doesn't belong here, but since is copy/paste from above, put it here. */

/* hb_static_size (T)
 * Returns T::static_size if T::min_size is defined, or sizeof (T) otherwise. */

template <typename T, typename>
কাঠামো hb_static_আকার_2 : hb_integral_constant<unsigned, আকার (T)> {};
template <typename T>
কাঠামো hb_static_আকার_2<T, hb_void_t<decltype (T::static_আকার)>> : hb_integral_constant<unsigned, T::static_আকার> {};
template <typename T>
using hb_static_আকার = hb_static_আকার_2<T, শূন্য>;
#define hb_static_আকার(T) hb_static_আকার<T>::value

template <typename T, typename>
কাঠামো hb_min_আকার_2 : hb_integral_constant<unsigned, আকার (T)> {};
template <typename T>
কাঠামো hb_min_আকার_2<T, hb_void_t<decltype (T::min_আকার)>> : hb_integral_constant<unsigned, T::min_আকার> {};
template <typename T>
using hb_min_আকার = hb_min_আকার_2<T, শূন্য>;
#define hb_min_আকার(T) hb_min_আকার<T>::value


/*
 * Null()
 */

বাহ্যিক HB_INTERNAL
uint64_t ধ্রুবক _hb_NullPool[(HB_NULL_POOL_আকার + আকার (uint64_t) - 1) / আকার (uint64_t)];

/* Generic nul-content Null objects. */
template <typename Type>
কাঠামো Null {
  স্থির Type ধ্রুবক & get_null ()
  {
    static_assert (hb_null_আকার_2 (Type) <= HB_NULL_POOL_আকার, "Increase HB_NULL_POOL_SIZE.");
    ফেরত *reinterpret_cast<Type ধ্রুবক *> (_hb_NullPool);
  }
};
template <typename QType>
কাঠামো NullHelper
{
  প্রকারলেখ hb_remove_const<hb_remove_reference<QType>> Type;
  স্থির ধ্রুবক Type & get_null () { ফেরত Null<Type>::get_null (); }
};
#define Null(Type) NullHelper<Type>::get_null ()

/* Specializations for arbitrary-content Null objects expressed in bytes. */
#define DECLARE_NULL_NAMESPACE_BYTES(Namespace, Type) \
	} /* Close namespace. */ \
	বাহ্যিক HB_INTERNAL ধ্রুবক unsigned অক্ষর _hb_Null_##Namespace##_##Type[hb_null_আকার_2 (Namespace::Type)]; \
	template <> \
	কাঠামো Null<Namespace::Type> { \
	  স্থির Namespace::Type ধ্রুবক & get_null () { \
	    ফেরত *reinterpret_cast<ধ্রুবক Namespace::Type *> (_hb_Null_##Namespace##_##Type); \
	  } \
	}; \
	নামাকাশ Namespace { \
	static_assert (সত্য, "") /* Require semicolon after. */
#define DECLARE_NULL_NAMESPACE_BYTES_TEMPLATE1(Namespace, Type, আকার_3) \
	} /* Close namespace. */ \
	বাহ্যিক HB_INTERNAL ধ্রুবক unsigned অক্ষর _hb_Null_##Namespace##_##Type[আকার_3]; \
	template <typename Spec> \
	কাঠামো Null<Namespace::Type<Spec>> { \
	  স্থির Namespace::Type<Spec> ধ্রুবক & get_null () { \
	    ফেরত *reinterpret_cast<ধ্রুবক Namespace::Type<Spec> *> (_hb_Null_##Namespace##_##Type); \
	  } \
	}; \
	নামাকাশ Namespace { \
	static_assert (সত্য, "") /* Require semicolon after. */
#define DEFINE_NULL_NAMESPACE_BYTES(Namespace, Type) \
	ধ্রুবক unsigned অক্ষর _hb_Null_##Namespace##_##Type[আকার (_hb_Null_##Namespace##_##Type)]

/* Specializations for arbitrary-content Null objects expressed as struct initializer. */
#define DECLARE_NULL_INSTANCE(Type) \
	বাহ্যিক HB_INTERNAL ধ্রুবক Type _hb_Null_##Type; \
	template <> \
	কাঠামো Null<Type> { \
	  স্থির Type ধ্রুবক & get_null () { \
	    ফেরত _hb_Null_##Type; \
	  } \
	}; \
	static_assert (সত্য, "") /* Require semicolon after. */
#define DEFINE_NULL_INSTANCE(Type) \
	ধ্রুবক Type _hb_Null_##Type

/* Global writable pool.  Enlarge as necessary. */

/* To be fully correct, CrapPool must be thread_local. However, we do not rely on CrapPool
 * for correct operation. It only exist to catch and divert program logic bugs instead of
 * causing bad memory access. So, races there are not actually introducing incorrectness
 * in the code. Has ~12kb binary size overhead to have it, also clang build fails with it. */
বাহ্যিক HB_INTERNAL
/*thread_local*/ uint64_t _hb_CrapPool[(HB_NULL_POOL_আকার + আকার (uint64_t) - 1) / আকার (uint64_t)];

/* CRAP pool: Common Region for Access Protection. */
template <typename Type>
স্থির অন্তঃরেখ Type& Crap () {
  static_assert (hb_null_আকার_2 (Type) <= HB_NULL_POOL_আকার, "Increase HB_NULL_POOL_SIZE.");
  Type *obj = reinterpret_cast<Type *> (_hb_CrapPool);
  memcpy (reinterpret_cast<শূন্য*>(obj), std::addressof (Null (Type)), আকার (*obj));
  ফেরত *obj;
}
template <typename QType>
কাঠামো CrapHelper
{
  প্রকারলেখ hb_remove_const<hb_remove_reference<QType>> Type;
  স্থির Type & get_crap () { ফেরত Crap<Type> (); }
};
#define Crap(Type) CrapHelper<Type>::get_crap ()

template <typename Type>
কাঠামো CrapOrNullHelper {
  স্থির Type & get () { ফেরত Crap (Type); }
};
template <typename Type>
কাঠামো CrapOrNullHelper<ধ্রুবক Type> {
  স্থির ধ্রুবক Type & get () { ফেরত Null (Type); }
};
#define CrapOrNull(Type) CrapOrNullHelper<Type>::get ()


/*
 * hb_nonnull_ptr_t
 */

template <typename P>
কাঠামো hb_nonnull_ptr_t
{
  প্রকারলেখ hb_remove_pointer<P> T;

  hb_nonnull_ptr_t (T *v_ = nullptr) : v (v_) {}
  T * operator = (T *v_)   { ফেরত v = v_; }
  T * operator -> () ধ্রুবক { ফেরত get (); }
  T & operator * () ধ্রুবক  { ফেরত *get (); }
  T ** operator & () ধ্রুবক { ফেরত std::addressof (v); }
  /* Only auto-cast to const types. */
  template <typename C> operator ধ্রুবক C * () ধ্রুবক { ফেরত get (); }
  operator ধ্রুবক অক্ষর * () ধ্রুবক { ফেরত (ধ্রুবক অক্ষর *) get (); }
  T * get () ধ্রুবক { ফেরত v ? v : const_cast<T *> (std::addressof (Null (T))); }
  T * get_raw () ধ্রুবক { ফেরত v; }

  ব্যক্তিগত:
  T *v;
};


#endif /* HB_NULL_HH */

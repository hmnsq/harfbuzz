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

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-meta.hh"


/*
 * Static pools
 */

/* Global nul-content Null pool.  Enlarge as necessary. */

#define HB_NULL_POOL_SIZE 640

template <typename T, typename>
ഘടന _hb_has_min_size : hb_false_type {};
template <typename T>
ഘടന _hb_has_min_size<T, hb_void_t<decltype (T::min_size)>>
	: hb_true_type {};
template <typename T>
using hb_has_min_size = _hb_has_min_size<T, ശൂന്യം>;
#define hb_has_min_size(T) hb_has_min_size<T>::value

template <typename T, typename>
ഘടന _hb_has_null_size : hb_false_type {};
template <typename T>
ഘടന _hb_has_null_size<T, hb_void_t<decltype (T::null_size)>>
	: hb_true_type {};
template <typename T>
using hb_has_null_size = _hb_has_null_size<T, ശൂന്യം>;
#define hb_has_null_size(T) hb_has_null_size<T>::value

/* Use SFINAE to sniff whether T has min_size; in which case return the larger
 * of sizeof(T) and T::null_size, otherwise return sizeof(T).
 *
 * The main purpose of this is to let structs communicate that they are not nullable,
 * by defining min_size but *not* null_size. */

/* The hard way...
 * https://stackoverflow.com/questions/7776448/sfinae-tried-with-bool-gives-compiler-error-template-argument-tvalue-invol
 */

template <typename T, typename>
ഘടന _hb_null_size : hb_integral_constant<unsigned, വലുപ്പം (T)> {};
template <typename T>
ഘടന _hb_null_size<T, hb_void_t<decltype (T::min_size)>>
	: hb_integral_constant<unsigned,
			       (വലുപ്പം (T) > T::null_size ? വലുപ്പം (T) : T::null_size)> {};
template <typename T>
using hb_null_size = _hb_null_size<T, ശൂന്യം>;
#define hb_null_size(T) hb_null_size<T>::value

/* These doesn't belong here, but since is copy/paste from above, put it here. */

/* hb_static_size (T)
 * Returns T::static_size if T::min_size is defined, or sizeof (T) otherwise. */

template <typename T, typename>
ഘടന _hb_static_size : hb_integral_constant<unsigned, വലുപ്പം (T)> {};
template <typename T>
ഘടന _hb_static_size<T, hb_void_t<decltype (T::static_size)>> : hb_integral_constant<unsigned, T::static_size> {};
template <typename T>
using hb_static_size = _hb_static_size<T, ശൂന്യം>;
#define hb_static_size(T) hb_static_size<T>::value

template <typename T, typename>
ഘടന _hb_min_size : hb_integral_constant<unsigned, വലുപ്പം (T)> {};
template <typename T>
ഘടന _hb_min_size<T, hb_void_t<decltype (T::min_size)>> : hb_integral_constant<unsigned, T::min_size> {};
template <typename T>
using hb_min_size = _hb_min_size<T, ശൂന്യം>;
#define hb_min_size(T) hb_min_size<T>::value


/*
 * Null()
 */

ബാഹ്യ HB_INTERNAL
uint64_t സ്ഥിരം _hb_NullPool[(HB_NULL_POOL_SIZE + വലുപ്പം (uint64_t) - 1) / വലുപ്പം (uint64_t)];

/* Generic nul-content Null objects. */
template <typename Type>
ഘടന Null {
  സ്ഥിതം Type സ്ഥിരം & get_null ()
  {
    static_assert (hb_null_size (Type) <= HB_NULL_POOL_SIZE, "Increase HB_NULL_POOL_SIZE.");
    തിരികെ *reinterpret_cast<Type സ്ഥിരം *> (_hb_NullPool);
  }
};
template <typename QType>
ഘടന NullHelper
{
  തരനിർവചനം hb_remove_const<hb_remove_reference<QType>> Type;
  സ്ഥിതം സ്ഥിരം Type & get_null () { തിരികെ Null<Type>::get_null (); }
};
#define Null(Type) NullHelper<Type>::get_null ()

/* Specializations for arbitrary-content Null objects expressed in bytes. */
#define DECLARE_NULL_NAMESPACE_BYTES(Namespace, Type) \
	} /* Close namespace. */ \
	ബാഹ്യ HB_INTERNAL സ്ഥിരം unsigned അക്ഷരം _hb_Null_##Namespace##_##Type[hb_null_size (Namespace::Type)]; \
	template <> \
	ഘടന Null<Namespace::Type> { \
	  സ്ഥിതം Namespace::Type സ്ഥിരം & get_null () { \
	    തിരികെ *reinterpret_cast<സ്ഥിരം Namespace::Type *> (_hb_Null_##Namespace##_##Type); \
	  } \
	}; \
	നാമാകാശ Namespace { \
	static_assert (സത്യം, "") /* Require semicolon after. */
#define DECLARE_NULL_NAMESPACE_BYTES_TEMPLATE1(Namespace, Type, Size) \
	} /* Close namespace. */ \
	ബാഹ്യ HB_INTERNAL സ്ഥിരം unsigned അക്ഷരം _hb_Null_##Namespace##_##Type[Size]; \
	template <typename Spec> \
	ഘടന Null<Namespace::Type<Spec>> { \
	  സ്ഥിതം Namespace::Type<Spec> സ്ഥിരം & get_null () { \
	    തിരികെ *reinterpret_cast<സ്ഥിരം Namespace::Type<Spec> *> (_hb_Null_##Namespace##_##Type); \
	  } \
	}; \
	നാമാകാശ Namespace { \
	static_assert (സത്യം, "") /* Require semicolon after. */
#define DEFINE_NULL_NAMESPACE_BYTES(Namespace, Type) \
	സ്ഥിരം unsigned അക്ഷരം _hb_Null_##Namespace##_##Type[വലുപ്പം (_hb_Null_##Namespace##_##Type)]

/* Specializations for arbitrary-content Null objects expressed as struct initializer. */
#define DECLARE_NULL_INSTANCE(Type) \
	ബാഹ്യ HB_INTERNAL സ്ഥിരം Type _hb_Null_##Type; \
	template <> \
	ഘടന Null<Type> { \
	  സ്ഥിതം Type സ്ഥിരം & get_null () { \
	    തിരികെ _hb_Null_##Type; \
	  } \
	}; \
	static_assert (സത്യം, "") /* Require semicolon after. */
#define DEFINE_NULL_INSTANCE(Type) \
	സ്ഥിരം Type _hb_Null_##Type

/* Global writable pool.  Enlarge as necessary. */

/* To be fully correct, CrapPool must be thread_local. However, we do not rely on CrapPool
 * for correct operation. It only exist to catch and divert program logic bugs instead of
 * causing bad memory access. So, races there are not actually introducing incorrectness
 * in the code. Has ~12kb binary size overhead to have it, also clang build fails with it. */
ബാഹ്യ HB_INTERNAL
/*thread_local*/ uint64_t _hb_CrapPool[(HB_NULL_POOL_SIZE + വലുപ്പം (uint64_t) - 1) / വലുപ്പം (uint64_t)];

/* CRAP pool: Common Region for Access Protection. */
template <typename Type>
സ്ഥിതം വരിയിൽ Type& Crap () {
  static_assert (hb_null_size (Type) <= HB_NULL_POOL_SIZE, "Increase HB_NULL_POOL_SIZE.");
  Type *obj = reinterpret_cast<Type *> (_hb_CrapPool);
  memcpy (reinterpret_cast<ശൂന്യം*>(obj), std::addressof (Null (Type)), വലുപ്പം (*obj));
  തിരികെ *obj;
}
template <typename QType>
ഘടന CrapHelper
{
  തരനിർവചനം hb_remove_const<hb_remove_reference<QType>> Type;
  സ്ഥിതം Type & get_crap () { തിരികെ Crap<Type> (); }
};
#define Crap(Type) CrapHelper<Type>::get_crap ()

template <typename Type>
ഘടന CrapOrNullHelper {
  സ്ഥിതം Type & get () { തിരികെ Crap (Type); }
};
template <typename Type>
ഘടന CrapOrNullHelper<സ്ഥിരം Type> {
  സ്ഥിതം സ്ഥിരം Type & get () { തിരികെ Null (Type); }
};
#define CrapOrNull(Type) CrapOrNullHelper<Type>::get ()


/*
 * hb_nonnull_ptr_t
 */

template <typename P>
ഘടന hb_nonnull_ptr_t
{
  തരനിർവചനം hb_remove_pointer<P> T;

  hb_nonnull_ptr_t (T *v_ = nullptr) : v (v_) {}
  T * operator = (T *v_)   { തിരികെ v = v_; }
  T * operator -> () സ്ഥിരം { തിരികെ get (); }
  T & operator * () സ്ഥിരം  { തിരികെ *get (); }
  T ** operator & () സ്ഥിരം { തിരികെ std::addressof (v); }
  /* Only auto-cast to const types. */
  template <typename C> operator സ്ഥിരം C * () സ്ഥിരം { തിരികെ get (); }
  operator സ്ഥിരം അക്ഷരം * () സ്ഥിരം { തിരികെ (സ്ഥിരം അക്ഷരം *) get (); }
  T * get () സ്ഥിരം { തിരികെ v ? v : const_cast<T *> (std::addressof (Null (T))); }
  T * get_raw () സ്ഥിരം { തിരികെ v; }

  സ്വകാര്യ:
  T *v;
};


#endif /* HB_NULL_HH */

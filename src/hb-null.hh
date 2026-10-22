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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NULL_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NULL_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-meta.hh"


/*
 * Static pools
 */

/* Global nul-content Null pool.  Enlarge as necessary. */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NULL_POOL_ਮਾਪ 640

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ>
ਬਣਤਰ hb_has_min_ਮਾਪ : hb_false_type {};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ hb_has_min_ਮਾਪ<T, hb_void_t<decltype (T::min_ਮਾਪ)>>
	: hb_true_type {};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਵਰਤੋਂ hb_has_min_ਮਾਪ_2 = hb_has_min_ਮਾਪ<T, ਖਾਲੀ>;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_has_min_ਮਾਪ_2(T) hb_has_min_ਮਾਪ_2<T>::value

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ>
ਬਣਤਰ hb_has_null_ਮਾਪ : hb_false_type {};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ hb_has_null_ਮਾਪ<T, hb_void_t<decltype (T::null_ਮਾਪ)>>
	: hb_true_type {};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਵਰਤੋਂ hb_has_null_ਮਾਪ_2 = hb_has_null_ਮਾਪ<T, ਖਾਲੀ>;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_has_null_ਮਾਪ_2(T) hb_has_null_ਮਾਪ_2<T>::value

/* Use SFINAE to sniff whether T has min_size; in which case return the larger
 * of sizeof(T) and T::null_size, otherwise return sizeof(T).
 *
 * The main purpose of this is to let structs communicate that they are not nullable,
 * by defining min_size but *not* null_size. */

/* The hard way...
 * https://stackoverflow.com/questions/7776448/sfinae-tried-with-bool-gives-compiler-error-template-argument-tvalue-invol
 */

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ>
ਬਣਤਰ hb_null_ਮਾਪ : hb_integral_constant<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਆਕਾਰ (T)> {};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ hb_null_ਮਾਪ<T, hb_void_t<decltype (T::min_ਮਾਪ)>>
	: hb_integral_constant<ਬਿਨਾਨਿਸ਼ਾਨੀ,
			       (ਆਕਾਰ (T) > T::null_ਮਾਪ ? ਆਕਾਰ (T) : T::null_ਮਾਪ)> {};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਵਰਤੋਂ hb_null_ਮਾਪ_2 = hb_null_ਮਾਪ<T, ਖਾਲੀ>;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_null_ਮਾਪ_2(T) hb_null_ਮਾਪ_2<T>::value

/* These doesn't belong here, but since is copy/paste from above, put it here. */

/* hb_static_size (T)
 * Returns T::static_size if T::min_size is defined, or sizeof (T) otherwise. */

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ>
ਬਣਤਰ hb_static_ਮਾਪ_2 : hb_integral_constant<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਆਕਾਰ (T)> {};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ hb_static_ਮਾਪ_2<T, hb_void_t<decltype (T::static_ਮਾਪ)>> : hb_integral_constant<ਬਿਨਾਨਿਸ਼ਾਨੀ, T::static_ਮਾਪ> {};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਵਰਤੋਂ hb_static_ਮਾਪ = hb_static_ਮਾਪ_2<T, ਖਾਲੀ>;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_static_ਮਾਪ(T) hb_static_ਮਾਪ<T>::value

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ>
ਬਣਤਰ hb_min_ਮਾਪ_2 : hb_integral_constant<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਆਕਾਰ (T)> {};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ hb_min_ਮਾਪ_2<T, hb_void_t<decltype (T::min_ਮਾਪ)>> : hb_integral_constant<ਬਿਨਾਨਿਸ਼ਾਨੀ, T::min_ਮਾਪ> {};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਵਰਤੋਂ hb_min_ਮਾਪ = hb_min_ਮਾਪ_2<T, ਖਾਲੀ>;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_min_ਮਾਪ(T) hb_min_ਮਾਪ<T>::value


/*
 * Null()
 */

ਬਾਹਰੀ HB_ਅੰਦਰੂਨੀ
uint64_t ਅਚਲ _hb_NullPool[(HB_NULL_POOL_ਮਾਪ + ਆਕਾਰ (uint64_t) - 1) / ਆਕਾਰ (uint64_t)];

/* Generic nul-content Null objects. */
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
ਬਣਤਰ Null {
  ਸਥਿਰ Type ਅਚਲ & get_null ()
  {
    static_assert (hb_null_ਮਾਪ_2 (Type) <= HB_NULL_POOL_ਮਾਪ, "Increase HB_NULL_POOL_SIZE.");
    ਵਾਪਸ *reinterpret_cast<Type ਅਚਲ *> (_hb_NullPool);
  }
};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ QType>
ਬਣਤਰ NullHelper
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ hb_remove_const<hb_remove_reference<QType>> Type;
  ਸਥਿਰ ਅਚਲ Type & get_null () { ਵਾਪਸ Null<Type>::get_null (); }
};
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Null(Type) NullHelper<Type>::get_null ()

/* Specializations for arbitrary-content Null objects expressed in bytes. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DECLARE_NULL_NAMESPACE_ਬਾਈਟਾਂ(Namespace, Type) \
	} /* Close namespace. */ \
	ਬਾਹਰੀ HB_ਅੰਦਰੂਨੀ ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ _hb_Null_##Namespace##_##Type[hb_null_ਮਾਪ_2 (Namespace::Type)]; \
	ਸਾਂਚਾ <> \
	ਬਣਤਰ Null<Namespace::Type> { \
	  ਸਥਿਰ Namespace::Type ਅਚਲ & get_null () { \
	    ਵਾਪਸ *reinterpret_cast<ਅਚਲ Namespace::Type *> (_hb_Null_##Namespace##_##Type); \
	  } \
	}; \
	ਨਾਮਾਕਾਸ਼ Namespace { \
	static_assert (ਸੱਚ, "") /* Require semicolon after. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DECLARE_NULL_NAMESPACE_ਬਾਈਟਾਂ_TEMPLATE1(Namespace, Type, ਮਾਪ_2) \
	} /* Close namespace. */ \
	ਬਾਹਰੀ HB_ਅੰਦਰੂਨੀ ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ _hb_Null_##Namespace##_##Type[ਮਾਪ_2]; \
	ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Spec> \
	ਬਣਤਰ Null<Namespace::Type<Spec>> { \
	  ਸਥਿਰ Namespace::Type<Spec> ਅਚਲ & get_null () { \
	    ਵਾਪਸ *reinterpret_cast<ਅਚਲ Namespace::Type<Spec> *> (_hb_Null_##Namespace##_##Type); \
	  } \
	}; \
	ਨਾਮਾਕਾਸ਼ Namespace { \
	static_assert (ਸੱਚ, "") /* Require semicolon after. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFINE_NULL_NAMESPACE_ਬਾਈਟਾਂ(Namespace, Type) \
	ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ _hb_Null_##Namespace##_##Type[ਆਕਾਰ (_hb_Null_##Namespace##_##Type)]

/* Specializations for arbitrary-content Null objects expressed as struct initializer. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DECLARE_NULL_INSTANCE(Type) \
	ਬਾਹਰੀ HB_ਅੰਦਰੂਨੀ ਅਚਲ Type _hb_Null_##Type; \
	ਸਾਂਚਾ <> \
	ਬਣਤਰ Null<Type> { \
	  ਸਥਿਰ Type ਅਚਲ & get_null () { \
	    ਵਾਪਸ _hb_Null_##Type; \
	  } \
	}; \
	static_assert (ਸੱਚ, "") /* Require semicolon after. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFINE_NULL_INSTANCE(Type) \
	ਅਚਲ Type _hb_Null_##Type

/* Global writable pool.  Enlarge as necessary. */

/* To be fully correct, CrapPool must be thread_local. However, we do not rely on CrapPool
 * for correct operation. It only exist to catch and divert program logic bugs instead of
 * causing bad memory access. So, races there are not actually introducing incorrectness
 * in the code. Has ~12kb binary size overhead to have it, also clang build fails with it. */
ਬਾਹਰੀ HB_ਅੰਦਰੂਨੀ
/*thread_local*/ uint64_t _hb_CrapPool[(HB_NULL_POOL_ਮਾਪ + ਆਕਾਰ (uint64_t) - 1) / ਆਕਾਰ (uint64_t)];

/* CRAP pool: Common Region for Access Protection. */
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
ਸਥਿਰ ਸਤਰਅੰਦਰ Type& Crap () {
  static_assert (hb_null_ਮਾਪ_2 (Type) <= HB_NULL_POOL_ਮਾਪ, "Increase HB_NULL_POOL_SIZE.");
  Type *obj = reinterpret_cast<Type *> (_hb_CrapPool);
  ਸਮ੍ਰਿਤੀ_ਨਕਲ (reinterpret_cast<ਖਾਲੀ*>(obj), std::addressof (Null (Type)), ਆਕਾਰ (*obj));
  ਵਾਪਸ *obj;
}
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ QType>
ਬਣਤਰ CrapHelper
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ hb_remove_const<hb_remove_reference<QType>> Type;
  ਸਥਿਰ Type & get_crap () { ਵਾਪਸ Crap<Type> (); }
};
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Crap(Type) CrapHelper<Type>::get_crap ()

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
ਬਣਤਰ CrapOrNullHelper {
  ਸਥਿਰ Type & get () { ਵਾਪਸ Crap (Type); }
};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
ਬਣਤਰ CrapOrNullHelper<ਅਚਲ Type> {
  ਸਥਿਰ ਅਚਲ Type & get () { ਵਾਪਸ Null (Type); }
};
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CrapOrNull(Type) CrapOrNullHelper<Type>::get ()


/*
 * hb_nonnull_ptr_t
 */

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ P>
ਬਣਤਰ hb_nonnull_ptr_t
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ hb_remove_ਸੂਚਕ<P> T;

  hb_nonnull_ptr_t (T *v_ = ਖਾਲੀ_ਸੂਚਕ) : v (v_) {}
  T * ਸੰਚਾਲਕ = (T *v_)   { ਵਾਪਸ v = v_; }
  T * ਸੰਚਾਲਕ -> () ਅਚਲ { ਵਾਪਸ get (); }
  T & ਸੰਚਾਲਕ * () ਅਚਲ  { ਵਾਪਸ *get (); }
  T ** ਸੰਚਾਲਕ & () ਅਚਲ { ਵਾਪਸ std::addressof (v); }
  /* Only auto-cast to const types. */
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ C> ਸੰਚਾਲਕ ਅਚਲ C * () ਅਚਲ { ਵਾਪਸ get (); }
  ਸੰਚਾਲਕ ਅਚਲ ਅੱਖਰ * () ਅਚਲ { ਵਾਪਸ (ਅਚਲ ਅੱਖਰ *) get (); }
  T * get () ਅਚਲ { ਵਾਪਸ v ? v : const_cast<T *> (std::addressof (Null (T))); }
  T * get_raw () ਅਚਲ { ਵਾਪਸ v; }

  ਨਿਜੀ:
  T *v;
};


#ਜੇ_ਅੰਤ /* HB_NULL_HH */

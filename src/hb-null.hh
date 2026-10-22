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

#अगर_अघोषित HB_NULL_HH
#घोषणा HB_NULL_HH

#समावेश "hb.hh"
#समावेश "hb-meta.hh"


/*
 * Static pools
 */

/* Global nul-content Null pool.  Enlarge as necessary. */

#घोषणा HB_NULL_POOL_आकार 640

ढाँचा <typename T, typename>
काष्ठा hb_has_min_आकार : hb_false_type {};
ढाँचा <typename T>
काष्ठा hb_has_min_आकार<T, hb_void_t<decltype (T::min_आकार)>>
	: hb_true_type {};
ढाँचा <typename T>
using hb_has_min_आकार_2 = hb_has_min_आकार<T, व्योम>;
#घोषणा hb_has_min_आकार_2(T) hb_has_min_आकार_2<T>::value

ढाँचा <typename T, typename>
काष्ठा hb_has_null_आकार : hb_false_type {};
ढाँचा <typename T>
काष्ठा hb_has_null_आकार<T, hb_void_t<decltype (T::null_आकार)>>
	: hb_true_type {};
ढाँचा <typename T>
using hb_has_null_आकार_2 = hb_has_null_आकार<T, व्योम>;
#घोषणा hb_has_null_आकार_2(T) hb_has_null_आकार_2<T>::value

/* Use SFINAE to sniff whether T has min_size; in which case return the larger
 * of sizeof(T) and T::null_size, otherwise return sizeof(T).
 *
 * The main purpose of this is to let structs communicate that they are not nullable,
 * by defining min_size but *not* null_size. */

/* The hard way...
 * https://stackoverflow.com/questions/7776448/sfinae-tried-with-bool-gives-compiler-error-template-argument-tvalue-invol
 */

ढाँचा <typename T, typename>
काष्ठा hb_null_आकार : hb_integral_constant<अचिन्हित, माप (T)> {};
ढाँचा <typename T>
काष्ठा hb_null_आकार<T, hb_void_t<decltype (T::min_आकार)>>
	: hb_integral_constant<अचिन्हित,
			       (माप (T) > T::null_आकार ? माप (T) : T::null_आकार)> {};
ढाँचा <typename T>
using hb_null_आकार_2 = hb_null_आकार<T, व्योम>;
#घोषणा hb_null_आकार_2(T) hb_null_आकार_2<T>::value

/* These doesn't belong here, but since is copy/paste from above, put it here. */

/* hb_static_size (T)
 * Returns T::static_size if T::min_size is defined, or sizeof (T) otherwise. */

ढाँचा <typename T, typename>
काष्ठा hb_static_आकार_2 : hb_integral_constant<अचिन्हित, माप (T)> {};
ढाँचा <typename T>
काष्ठा hb_static_आकार_2<T, hb_void_t<decltype (T::static_आकार)>> : hb_integral_constant<अचिन्हित, T::static_आकार> {};
ढाँचा <typename T>
using hb_static_आकार = hb_static_आकार_2<T, व्योम>;
#घोषणा hb_static_आकार(T) hb_static_आकार<T>::value

ढाँचा <typename T, typename>
काष्ठा hb_min_आकार_2 : hb_integral_constant<अचिन्हित, माप (T)> {};
ढाँचा <typename T>
काष्ठा hb_min_आकार_2<T, hb_void_t<decltype (T::min_आकार)>> : hb_integral_constant<अचिन्हित, T::min_आकार> {};
ढाँचा <typename T>
using hb_min_आकार = hb_min_आकार_2<T, व्योम>;
#घोषणा hb_min_आकार(T) hb_min_आकार<T>::value


/*
 * Null()
 */

बाह्य HB_INTERNAL
uint64_t स्थिर _hb_NullPool[(HB_NULL_POOL_आकार + माप (uint64_t) - 1) / माप (uint64_t)];

/* Generic nul-content Null objects. */
ढाँचा <typename Type>
काष्ठा Null {
  अटल Type स्थिर & get_null ()
  {
    static_assert (hb_null_आकार_2 (Type) <= HB_NULL_POOL_आकार, "Increase HB_NULL_POOL_SIZE.");
    वापस *reinterpret_cast<Type स्थिर *> (_hb_NullPool);
  }
};
ढाँचा <typename QType>
काष्ठा NullHelper
{
  प्रकार hb_remove_const<hb_remove_reference<QType>> Type;
  अटल स्थिर Type & get_null () { वापस Null<Type>::get_null (); }
};
#घोषणा Null(Type) NullHelper<Type>::get_null ()

/* Specializations for arbitrary-content Null objects expressed in bytes. */
#घोषणा DECLARE_NULL_NAMESPACE_BYTES(Namespace, Type) \
	} /* Close namespace. */ \
	बाह्य HB_INTERNAL स्थिर अचिन्हित अक्षर _hb_Null_##Namespace##_##Type[hb_null_आकार_2 (Namespace::Type)]; \
	ढाँचा <> \
	काष्ठा Null<Namespace::Type> { \
	  अटल Namespace::Type स्थिर & get_null () { \
	    वापस *reinterpret_cast<स्थिर Namespace::Type *> (_hb_Null_##Namespace##_##Type); \
	  } \
	}; \
	namespace Namespace { \
	static_assert (true, "") /* Require semicolon after. */
#घोषणा DECLARE_NULL_NAMESPACE_BYTES_TEMPLATE1(Namespace, Type, आकार_2) \
	} /* Close namespace. */ \
	बाह्य HB_INTERNAL स्थिर अचिन्हित अक्षर _hb_Null_##Namespace##_##Type[आकार_2]; \
	ढाँचा <typename Spec> \
	काष्ठा Null<Namespace::Type<Spec>> { \
	  अटल Namespace::Type<Spec> स्थिर & get_null () { \
	    वापस *reinterpret_cast<स्थिर Namespace::Type<Spec> *> (_hb_Null_##Namespace##_##Type); \
	  } \
	}; \
	namespace Namespace { \
	static_assert (true, "") /* Require semicolon after. */
#घोषणा DEFINE_NULL_NAMESPACE_BYTES(Namespace, Type) \
	स्थिर अचिन्हित अक्षर _hb_Null_##Namespace##_##Type[माप (_hb_Null_##Namespace##_##Type)]

/* Specializations for arbitrary-content Null objects expressed as struct initializer. */
#घोषणा DECLARE_NULL_INSTANCE(Type) \
	बाह्य HB_INTERNAL स्थिर Type _hb_Null_##Type; \
	ढाँचा <> \
	काष्ठा Null<Type> { \
	  अटल Type स्थिर & get_null () { \
	    वापस _hb_Null_##Type; \
	  } \
	}; \
	static_assert (true, "") /* Require semicolon after. */
#घोषणा DEFINE_NULL_INSTANCE(Type) \
	स्थिर Type _hb_Null_##Type

/* Global writable pool.  Enlarge as necessary. */

/* To be fully correct, CrapPool must be thread_local. However, we do not rely on CrapPool
 * for correct operation. It only exist to catch and divert program logic bugs instead of
 * causing bad memory access. So, races there are not actually introducing incorrectness
 * in the code. Has ~12kb binary size overhead to have it, also clang build fails with it. */
बाह्य HB_INTERNAL
/*thread_local*/ uint64_t _hb_CrapPool[(HB_NULL_POOL_आकार + माप (uint64_t) - 1) / माप (uint64_t)];

/* CRAP pool: Common Region for Access Protection. */
ढाँचा <typename Type>
अटल अंतरभूत Type& Crap () {
  static_assert (hb_null_आकार_2 (Type) <= HB_NULL_POOL_आकार, "Increase HB_NULL_POOL_SIZE.");
  Type *obj = reinterpret_cast<Type *> (_hb_CrapPool);
  स_नकल (reinterpret_cast<व्योम*>(obj), std::addressof (Null (Type)), माप (*obj));
  वापस *obj;
}
ढाँचा <typename QType>
काष्ठा CrapHelper
{
  प्रकार hb_remove_const<hb_remove_reference<QType>> Type;
  अटल Type & get_crap () { वापस Crap<Type> (); }
};
#घोषणा Crap(Type) CrapHelper<Type>::get_crap ()

ढाँचा <typename Type>
काष्ठा CrapOrNullHelper {
  अटल Type & लो () { वापस Crap (Type); }
};
ढाँचा <typename Type>
काष्ठा CrapOrNullHelper<स्थिर Type> {
  अटल स्थिर Type & लो () { वापस Null (Type); }
};
#घोषणा CrapOrNull(Type) CrapOrNullHelper<Type>::लो ()


/*
 * hb_nonnull_ptr_t
 */

ढाँचा <typename P>
काष्ठा hb_nonnull_ptr_t
{
  प्रकार hb_remove_pointer<P> T;

  hb_nonnull_ptr_t (T *v_ = nullptr) : v (v_) {}
  T * चालक = (T *v_)   { वापस v = v_; }
  T * चालक -> () स्थिर { वापस लो (); }
  T & चालक * () स्थिर  { वापस *लो (); }
  T ** चालक & () स्थिर { वापस std::addressof (v); }
  /* Only auto-cast to const types. */
  ढाँचा <typename C> चालक स्थिर C * () स्थिर { वापस लो (); }
  चालक स्थिर अक्षर * () स्थिर { वापस (स्थिर अक्षर *) लो (); }
  T * लो () स्थिर { वापस v ? v : const_cast<T *> (std::addressof (Null (T))); }
  T * get_raw () स्थिर { वापस v; }

  निजी:
  T *v;
};


#पूर्ण_अगर /* HB_NULL_HH */

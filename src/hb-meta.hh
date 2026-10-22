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

#ifndef HB_META_HH
#define HB_META_HH

#अन्तर्भाव "hb.hh"

#अन्तर्भाव <memory>
#अन्तर्भाव <type_traits>
#अन्तर्भाव <utility>


/*
 * C++ template meta-programming & fundamentals used with them.
 */

/* Void!  For when we need a expression-type of void. */
संरचना hb_empty_t {};

/* https://en.cppreference.com/w/cpp/types/void_t */
ढाँचा<typename... Ts> संरचना _hb_void_t { प्रकारव्याख्या शून्य type; };
ढाँचा<typename... Ts> using hb_void_t = typename _hb_void_t<Ts...>::type;

ढाँचा<typename Head, typename... Ts> संरचना _hb_head_t { प्रकारव्याख्या Head type; };
ढाँचा<typename... Ts> using hb_head_t = typename _hb_head_t<Ts...>::type;

ढाँचा <typename T, T v> संरचना hb_integral_constant { स्थिर constexpr T value = v; };
ढाँचा <तार्किक b> using hb_bool_constant = hb_integral_constant<तार्किक, b>;
using hb_true_type = hb_bool_constant<सत्य>;
using hb_false_type = hb_bool_constant<असत्य>;

/* Static-assert as expression. */
ढाँचा <तार्किक cond> संरचना static_assert_अभिव्यक्ती;
ढाँचा <> संरचना static_assert_अभिव्यक्ती<सत्य> : hb_false_type {};
#define static_assert_अभिव्यक्ती(C) static_assert_अभिव्यक्ती<C>::value

/* Basic type SFINAE. */

ढाँचा <तार्किक B, typename T = शून्य> संरचना hb_enable_if {};
ढाँचा <typename T>                संरचना hb_enable_if<सत्य, T> { प्रकारव्याख्या T type; };
#define hb_enable_if(Cond) typename hb_enable_if<(Cond)>::type* = nullptr
/* Concepts/Requires alias: */
#define hb_requires(Cond) hb_enable_if((Cond))

ढाँचा <typename T, typename T2> संरचना hb_is_same : hb_false_type {};
ढाँचा <typename T>              संरचना hb_is_same<T, T> : hb_true_type {};
#define hb_is_same(T, T2) hb_is_same<T, T2>::value

/* Function overloading SFINAE and priority. */

#define HB_RETURN(Ret, E) -> hb_head_t<Ret, decltype ((E))> { परत (E); }
#define HB_AUTO_RETURN(E) -> decltype ((E)) { परत (E); }
#define HB_VOID_RETURN(E) -> hb_void_t<decltype ((E))> { (E); }

ढाँचा <अचिन्हित Pri> संरचना hb_priority : hb_priority<Pri - 1> {};
ढाँचा <>             संरचना hb_priority<0> {};
#define hb_prioritize hb_priority<16> ()

#define HB_FUNCOBJ(x) static_const x HB_UNUSED


ढाँचा <typename T> संरचना hb_type_identity_t { प्रकारव्याख्या T type; };
ढाँचा <typename T> using hb_type_identity = typename hb_type_identity_t<T>::type;

ढाँचा <typename T> स्थिर ओळीत T hb_declval ();
#define hb_declval(T) (hb_declval<T> ())

ढाँचा <typename T> संरचना hb_match_const		: hb_type_identity_t<T>, hb_false_type	{};
ढाँचा <typename T> संरचना hb_match_const<स्थिरांक T>	: hb_type_identity_t<T>, hb_true_type	{};
ढाँचा <typename T> using hb_remove_const = typename hb_match_const<T>::type;

ढाँचा <typename T> संरचना hb_match_reference		: hb_type_identity_t<T>, hb_false_type	{};
ढाँचा <typename T> संरचना hb_match_reference<T &>	: hb_type_identity_t<T>, hb_true_type	{};
ढाँचा <typename T> संरचना hb_match_reference<T &&>	: hb_type_identity_t<T>, hb_true_type	{};
ढाँचा <typename T> using hb_remove_reference = typename hb_match_reference<T>::type;
ढाँचा <typename T> स्वतः _hb_try_add_lvalue_reference (hb_priority<1>) -> hb_type_identity<T&>;
ढाँचा <typename T> स्वतः _hb_try_add_lvalue_reference (hb_priority<0>) -> hb_type_identity<T>;
ढाँचा <typename T> using hb_add_lvalue_reference = decltype (_hb_try_add_lvalue_reference<T> (hb_prioritize));
ढाँचा <typename T> स्वतः _hb_try_add_rvalue_reference (hb_priority<1>) -> hb_type_identity<T&&>;
ढाँचा <typename T> स्वतः _hb_try_add_rvalue_reference (hb_priority<0>) -> hb_type_identity<T>;
ढाँचा <typename T> using hb_add_rvalue_reference = decltype (_hb_try_add_rvalue_reference<T> (hb_prioritize));

ढाँचा <typename T> संरचना hb_match_pointer		: hb_type_identity_t<T>, hb_false_type	{};
ढाँचा <typename T> संरचना hb_match_pointer<T *>	: hb_type_identity_t<T>, hb_true_type	{};
ढाँचा <typename T> using hb_remove_pointer = typename hb_match_pointer<T>::type;
ढाँचा <typename T> स्वतः _hb_try_add_pointer (hb_priority<1>) -> hb_type_identity<hb_remove_reference<T>*>;
ढाँचा <typename T> स्वतः _hb_try_add_pointer (hb_priority<1>) -> hb_type_identity<T>;
ढाँचा <typename T> using hb_add_pointer = decltype (_hb_try_add_pointer<T> (hb_prioritize));


ढाँचा <typename T> using hb_decay = typename std::decay<T>::type;

#define hb_is_convertible(From,To) std::is_convertible<From, To>::value

ढाँचा <typename From, typename To>
using hb_is_cr_convertible = hb_bool_constant<
  hb_is_same (hb_decay<From>, hb_decay<To>) &&
  (!std::is_const<From>::value || std::is_const<To>::value) &&
  (!std::is_reference<To>::value || std::is_const<To>::value || std::is_reference<To>::value)
>;
#define hb_is_cr_convertible(From,To) hb_is_cr_convertible<From, To>::value


संरचना
{
  ढाँचा <typename T> constexpr स्वतः
  चालक () (T&& v) स्थिरांक HB_AUTO_RETURN (std::forward<T> (v))

  ढाँचा <typename T> constexpr स्वतः
  चालक () (T *v) स्थिरांक HB_AUTO_RETURN (*v)

  ढाँचा <typename T> constexpr स्वतः
  चालक () (स्थिरांक hb::shared_ptr<T>& v) स्थिरांक HB_AUTO_RETURN (*v)

  ढाँचा <typename T> constexpr स्वतः
  चालक () (hb::shared_ptr<T>& v) स्थिरांक HB_AUTO_RETURN (*v)
  
  ढाँचा <typename T> constexpr स्वतः
  चालक () (स्थिरांक hb::unique_ptr<T>& v) स्थिरांक HB_AUTO_RETURN (*v)

  ढाँचा <typename T> constexpr स्वतः
  चालक () (hb::unique_ptr<T>& v) स्थिरांक HB_AUTO_RETURN (*v)
}
HB_FUNCOBJ (hb_deref);

ढाँचा <typename T>
संरचना hb_reference_wrapper
{
  hb_reference_wrapper (T v) : v (v) {}
  तार्किक चालक == (स्थिरांक hb_reference_wrapper& o) स्थिरांक { परत v == o.v; }
  तार्किक चालक != (स्थिरांक hb_reference_wrapper& o) स्थिरांक { परत v != o.v; }
  चालक T& () { परत v; }
  T& get () { परत v; }
  T v;
};
ढाँचा <typename T>
संरचना hb_reference_wrapper<T&>
{
  hb_reference_wrapper (T& v) : v (std::addressof (v)) {}
  तार्किक चालक == (स्थिरांक hb_reference_wrapper& o) स्थिरांक { परत v == o.v; }
  तार्किक चालक != (स्थिरांक hb_reference_wrapper& o) स्थिरांक { परत v != o.v; }
  चालक T& () { परत *v; }
  T& get () { परत *v; }
  T* v;
};


/* Type traits */

ढाँचा <typename T> संरचना hb_int_min;
ढाँचा <> संरचना hb_int_min<अक्षर>			: hb_integral_constant<अक्षर,			अक्षर_न्यून>	{};
ढाँचा <> संरचना hb_int_min<चिन्हित अक्षर>		: hb_integral_constant<चिन्हित अक्षर,		च_अक्षर_न्यून>	{};
ढाँचा <> संरचना hb_int_min<अचिन्हित अक्षर>		: hb_integral_constant<अचिन्हित अक्षर,		0>		{};
ढाँचा <> संरचना hb_int_min<चिन्हित लघु>		: hb_integral_constant<चिन्हित लघु,		लघु_न्यून>	{};
ढाँचा <> संरचना hb_int_min<अचिन्हित लघु>		: hb_integral_constant<अचिन्हित लघु,		0>		{};
ढाँचा <> संरचना hb_int_min<चिन्हित पूर्णांक>		: hb_integral_constant<चिन्हित पूर्णांक,		पूर्णांक_न्यून>	{};
ढाँचा <> संरचना hb_int_min<अचिन्हित पूर्णांक>		: hb_integral_constant<अचिन्हित पूर्णांक,		0>		{};
ढाँचा <> संरचना hb_int_min<चिन्हित दीर्घ>		: hb_integral_constant<चिन्हित दीर्घ,		दीर्घ_न्यून>	{};
ढाँचा <> संरचना hb_int_min<अचिन्हित दीर्घ>		: hb_integral_constant<अचिन्हित दीर्घ,		0>		{};
ढाँचा <> संरचना hb_int_min<चिन्हित दीर्घ दीर्घ>		: hb_integral_constant<चिन्हित दीर्घ दीर्घ,	LLONG_MIN>	{};
ढाँचा <> संरचना hb_int_min<अचिन्हित दीर्घ दीर्घ>	: hb_integral_constant<अचिन्हित दीर्घ दीर्घ,	0>		{};
ढाँचा <typename T> संरचना hb_int_min<T *>		: hb_integral_constant<T *,			nullptr>	{};
#define hb_int_min(T) hb_int_min<T>::value
ढाँचा <typename T> संरचना hb_int_max;
ढाँचा <> संरचना hb_int_max<अक्षर>			: hb_integral_constant<अक्षर,			अक्षर_उच्च>	{};
ढाँचा <> संरचना hb_int_max<चिन्हित अक्षर>		: hb_integral_constant<चिन्हित अक्षर,		च_अक्षर_उच्च>	{};
ढाँचा <> संरचना hb_int_max<अचिन्हित अक्षर>		: hb_integral_constant<अचिन्हित अक्षर,		अच_अक्षर_उच्च>	{};
ढाँचा <> संरचना hb_int_max<चिन्हित लघु>		: hb_integral_constant<चिन्हित लघु,		लघु_उच्च>	{};
ढाँचा <> संरचना hb_int_max<अचिन्हित लघु>		: hb_integral_constant<अचिन्हित लघु,		अच_लघु_उच्च>	{};
ढाँचा <> संरचना hb_int_max<चिन्हित पूर्णांक>		: hb_integral_constant<चिन्हित पूर्णांक,		पूर्णांक_उच्च>	{};
ढाँचा <> संरचना hb_int_max<अचिन्हित पूर्णांक>		: hb_integral_constant<अचिन्हित पूर्णांक,		अच_पूर्णांक_उच्च>	{};
ढाँचा <> संरचना hb_int_max<चिन्हित दीर्घ>		: hb_integral_constant<चिन्हित दीर्घ,		दीर्घ_उच्च>	{};
ढाँचा <> संरचना hb_int_max<अचिन्हित दीर्घ>		: hb_integral_constant<अचिन्हित दीर्घ,		अच_दीर्घ_उच्च>	{};
ढाँचा <> संरचना hb_int_max<चिन्हित दीर्घ दीर्घ>		: hb_integral_constant<चिन्हित दीर्घ दीर्घ,	LLONG_MAX>	{};
ढाँचा <> संरचना hb_int_max<अचिन्हित दीर्घ दीर्घ>	: hb_integral_constant<अचिन्हित दीर्घ दीर्घ,	ULLONG_MAX>	{};
#define hb_int_max(T) hb_int_max<T>::value

#जर defined(__GNUC__) && __GNUC__ < 5 && !defined(__clang__)
#define hb_is_trivially_copyable(T) __has_trivial_copy(T)
#define hb_is_trivially_copy_assignable(T) __has_trivial_assign(T)
#define hb_is_trivially_constructible(T) __has_trivial_constructor(T)
#define hb_is_trivially_copy_constructible(T) __has_trivial_copy_constructor(T)
#define hb_is_trivially_destructible(T) __has_trivial_destructor(T)
#अन्यथा
#define hb_is_trivially_copyable(T) std::is_trivially_copyable<T>::value
#define hb_is_trivially_copy_assignable(T) std::is_trivially_copy_assignable<T>::value
#define hb_is_trivially_constructible(T) std::is_trivially_constructible<T>::value
#define hb_is_trivially_copy_constructible(T) std::is_trivially_copy_constructible<T>::value
#define hb_is_trivially_destructible(T) std::is_trivially_destructible<T>::value
#endif

/* Class traits. */

#define HB_DELETE_COPY_ASSIGN(TypeName) \
  TypeName(स्थिरांक TypeName&) = delete; \
  शून्य चालक=(स्थिरांक TypeName&) = delete
#define HB_DELETE_CREATE_COPY_ASSIGN(TypeName) \
  TypeName() = delete; \
  TypeName(स्थिरांक TypeName&) = delete; \
  शून्य चालक=(स्थिरांक TypeName&) = delete

/* hb_unwrap_type (T)
 * If T has no T::type, returns T. Otherwise calls itself on T::type recursively.
 */

ढाँचा <typename T, typename>
संरचना _hb_unwrap_type : hb_type_identity_t<T> {};
ढाँचा <typename T>
संरचना _hb_unwrap_type<T, hb_void_t<typename T::type>> : _hb_unwrap_type<typename T::type, शून्य> {};
ढाँचा <typename T>
using hb_unwrap_type = _hb_unwrap_type<T, शून्य>;
#define hb_unwrap_type(T) typename hb_unwrap_type<T>::type

#endif /* HB_META_HH */

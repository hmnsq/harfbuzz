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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ಅನ್ತರ್ಭಾವ <memory>
#ಅನ್ತರ್ಭಾವ <type_traits>
#ಅನ್ತರ್ಭಾವ <utility>


/*
 * C++ template meta-programming & fundamentals used with them.
 */

/* Void!  For when we need a expression-type of void. */
ಸಂರಚನಾ hb_empty_t {};

/* https://en.cppreference.com/w/cpp/types/void_t */
template<typename... Ts> ಸಂರಚನಾ _hb_void_t { ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ type; };
template<typename... Ts> using hb_void_t = typename _hb_void_t<Ts...>::type;

template<typename Head, typename... Ts> ಸಂರಚನಾ _hb_head_t { ಪ್ರಕಾರನಿರ್ದೇಶ Head type; };
template<typename... Ts> using hb_head_t = typename _hb_head_t<Ts...>::type;

template <typename T, T v> ಸಂರಚನಾ hb_integral_constant { ಸ್ಥಿರ constexpr T value = v; };
template <ತಾರ್ಕಿಕ b> using hb_bool_constant = hb_integral_constant<ತಾರ್ಕಿಕ, b>;
using hb_true_type = hb_bool_constant<ಸತ್ಯ>;
using hb_false_type = hb_bool_constant<ಅಸತ್ಯ>;

/* Static-assert as expression. */
template <ತಾರ್ಕಿಕ cond> ಸಂರಚನಾ static_assert_ಅಭಿವ್ಯಕ್ತಿ;
template <> ಸಂರಚನಾ static_assert_ಅಭಿವ್ಯಕ್ತಿ<ಸತ್ಯ> : hb_false_type {};
#define static_assert_ಅಭಿವ್ಯಕ್ತಿ(C) static_assert_ಅಭಿವ್ಯಕ್ತಿ<C>::value

/* Basic type SFINAE. */

template <ತಾರ್ಕಿಕ B, typename T = ಶೂನ್ಯ> ಸಂರಚನಾ hb_enable_if {};
template <typename T>                ಸಂರಚನಾ hb_enable_if<ಸತ್ಯ, T> { ಪ್ರಕಾರನಿರ್ದೇಶ T type; };
#define hb_enable_if(Cond) typename hb_enable_if<(Cond)>::type* = nullptr
/* Concepts/Requires alias: */
#define hb_requires(Cond) hb_enable_if((Cond))

template <typename T, typename T2> ಸಂರಚನಾ hb_is_same : hb_false_type {};
template <typename T>              ಸಂರಚನಾ hb_is_same<T, T> : hb_true_type {};
#define hb_is_same(T, T2) hb_is_same<T, T2>::value

/* Function overloading SFINAE and priority. */

#define HB_RETURN(Ret, E) -> hb_head_t<Ret, decltype ((E))> { ಪ್ರತ್ಯಾಗಮ (E); }
#define HB_AUTO_RETURN(E) -> decltype ((E)) { ಪ್ರತ್ಯಾಗಮ (E); }
#define HB_VOID_RETURN(E) -> hb_void_t<decltype ((E))> { (E); }

template <unsigned Pri> ಸಂರಚನಾ hb_priority : hb_priority<Pri - 1> {};
template <>             ಸಂರಚನಾ hb_priority<0> {};
#define hb_prioritize hb_priority<16> ()

#define HB_FUNCOBJ(x) static_const x HB_UNUSED


template <typename T> ಸಂರಚನಾ hb_type_identity_t { ಪ್ರಕಾರನಿರ್ದೇಶ T type; };
template <typename T> using hb_type_identity = typename hb_type_identity_t<T>::type;

template <typename T> ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ T hb_declval ();
#define hb_declval(T) (hb_declval<T> ())

template <typename T> ಸಂರಚನಾ hb_match_const		: hb_type_identity_t<T>, hb_false_type	{};
template <typename T> ಸಂರಚನಾ hb_match_const<ಧ್ರುವ T>	: hb_type_identity_t<T>, hb_true_type	{};
template <typename T> using hb_remove_const = typename hb_match_const<T>::type;

template <typename T> ಸಂರಚನಾ hb_match_reference		: hb_type_identity_t<T>, hb_false_type	{};
template <typename T> ಸಂರಚನಾ hb_match_reference<T &>	: hb_type_identity_t<T>, hb_true_type	{};
template <typename T> ಸಂರಚನಾ hb_match_reference<T &&>	: hb_type_identity_t<T>, hb_true_type	{};
template <typename T> using hb_remove_reference = typename hb_match_reference<T>::type;
template <typename T> auto _hb_try_add_lvalue_reference (hb_priority<1>) -> hb_type_identity<T&>;
template <typename T> auto _hb_try_add_lvalue_reference (hb_priority<0>) -> hb_type_identity<T>;
template <typename T> using hb_add_lvalue_reference = decltype (_hb_try_add_lvalue_reference<T> (hb_prioritize));
template <typename T> auto _hb_try_add_rvalue_reference (hb_priority<1>) -> hb_type_identity<T&&>;
template <typename T> auto _hb_try_add_rvalue_reference (hb_priority<0>) -> hb_type_identity<T>;
template <typename T> using hb_add_rvalue_reference = decltype (_hb_try_add_rvalue_reference<T> (hb_prioritize));

template <typename T> ಸಂರಚನಾ hb_match_pointer		: hb_type_identity_t<T>, hb_false_type	{};
template <typename T> ಸಂರಚನಾ hb_match_pointer<T *>	: hb_type_identity_t<T>, hb_true_type	{};
template <typename T> using hb_remove_pointer = typename hb_match_pointer<T>::type;
template <typename T> auto _hb_try_add_pointer (hb_priority<1>) -> hb_type_identity<hb_remove_reference<T>*>;
template <typename T> auto _hb_try_add_pointer (hb_priority<1>) -> hb_type_identity<T>;
template <typename T> using hb_add_pointer = decltype (_hb_try_add_pointer<T> (hb_prioritize));


template <typename T> using hb_decay = typename std::decay<T>::type;

#define hb_is_convertible(From,To) std::is_convertible<From, To>::value

template <typename From, typename To>
using hb_is_cr_convertible = hb_bool_constant<
  hb_is_same (hb_decay<From>, hb_decay<To>) &&
  (!std::is_const<From>::value || std::is_const<To>::value) &&
  (!std::is_reference<To>::value || std::is_const<To>::value || std::is_reference<To>::value)
>;
#define hb_is_cr_convertible(From,To) hb_is_cr_convertible<From, To>::value


ಸಂರಚನಾ
{
  template <typename T> constexpr auto
  operator () (T&& v) ಧ್ರುವ HB_AUTO_RETURN (std::forward<T> (v))

  template <typename T> constexpr auto
  operator () (T *v) ಧ್ರುವ HB_AUTO_RETURN (*v)

  template <typename T> constexpr auto
  operator () (ಧ್ರುವ hb::shared_ptr<T>& v) ಧ್ರುವ HB_AUTO_RETURN (*v)

  template <typename T> constexpr auto
  operator () (hb::shared_ptr<T>& v) ಧ್ರುವ HB_AUTO_RETURN (*v)
  
  template <typename T> constexpr auto
  operator () (ಧ್ರುವ hb::unique_ptr<T>& v) ಧ್ರುವ HB_AUTO_RETURN (*v)

  template <typename T> constexpr auto
  operator () (hb::unique_ptr<T>& v) ಧ್ರುವ HB_AUTO_RETURN (*v)
}
HB_FUNCOBJ (hb_deref);

template <typename T>
ಸಂರಚನಾ hb_reference_wrapper
{
  hb_reference_wrapper (T v) : v (v) {}
  ತಾರ್ಕಿಕ operator == (ಧ್ರುವ hb_reference_wrapper& o) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ v == o.v; }
  ತಾರ್ಕಿಕ operator != (ಧ್ರುವ hb_reference_wrapper& o) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ v != o.v; }
  operator T& () { ಪ್ರತ್ಯಾಗಮ v; }
  T& get () { ಪ್ರತ್ಯಾಗಮ v; }
  T v;
};
template <typename T>
ಸಂರಚನಾ hb_reference_wrapper<T&>
{
  hb_reference_wrapper (T& v) : v (std::addressof (v)) {}
  ತಾರ್ಕಿಕ operator == (ಧ್ರುವ hb_reference_wrapper& o) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ v == o.v; }
  ತಾರ್ಕಿಕ operator != (ಧ್ರುವ hb_reference_wrapper& o) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ v != o.v; }
  operator T& () { ಪ್ರತ್ಯಾಗಮ *v; }
  T& get () { ಪ್ರತ್ಯಾಗಮ *v; }
  T* v;
};


/* Type traits */

template <typename T> ಸಂರಚನಾ hb_int_min;
template <> ಸಂರಚನಾ hb_int_min<ವರ್ಣ>			: hb_integral_constant<ವರ್ಣ,			CHAR_MIN>	{};
template <> ಸಂರಚನಾ hb_int_min<signed ವರ್ಣ>		: hb_integral_constant<signed ವರ್ಣ,		SCHAR_MIN>	{};
template <> ಸಂರಚನಾ hb_int_min<unsigned ವರ್ಣ>		: hb_integral_constant<unsigned ವರ್ಣ,		0>		{};
template <> ಸಂರಚನಾ hb_int_min<signed short>		: hb_integral_constant<signed short,		SHRT_MIN>	{};
template <> ಸಂರಚನಾ hb_int_min<unsigned short>		: hb_integral_constant<unsigned short,		0>		{};
template <> ಸಂರಚನಾ hb_int_min<signed ಪೂರ್ಣಾಙ್ಕ>		: hb_integral_constant<signed ಪೂರ್ಣಾಙ್ಕ,		INT_MIN>	{};
template <> ಸಂರಚನಾ hb_int_min<unsigned ಪೂರ್ಣಾಙ್ಕ>		: hb_integral_constant<unsigned ಪೂರ್ಣಾಙ್ಕ,		0>		{};
template <> ಸಂರಚನಾ hb_int_min<signed long>		: hb_integral_constant<signed long,		LONG_MIN>	{};
template <> ಸಂರಚನಾ hb_int_min<unsigned long>		: hb_integral_constant<unsigned long,		0>		{};
template <> ಸಂರಚನಾ hb_int_min<signed long long>		: hb_integral_constant<signed long long,	LLONG_MIN>	{};
template <> ಸಂರಚನಾ hb_int_min<unsigned long long>	: hb_integral_constant<unsigned long long,	0>		{};
template <typename T> ಸಂರಚನಾ hb_int_min<T *>		: hb_integral_constant<T *,			nullptr>	{};
#define hb_int_min(T) hb_int_min<T>::value
template <typename T> ಸಂರಚನಾ hb_int_max;
template <> ಸಂರಚನಾ hb_int_max<ವರ್ಣ>			: hb_integral_constant<ವರ್ಣ,			CHAR_MAX>	{};
template <> ಸಂರಚನಾ hb_int_max<signed ವರ್ಣ>		: hb_integral_constant<signed ವರ್ಣ,		SCHAR_MAX>	{};
template <> ಸಂರಚನಾ hb_int_max<unsigned ವರ್ಣ>		: hb_integral_constant<unsigned ವರ್ಣ,		UCHAR_MAX>	{};
template <> ಸಂರಚನಾ hb_int_max<signed short>		: hb_integral_constant<signed short,		SHRT_MAX>	{};
template <> ಸಂರಚನಾ hb_int_max<unsigned short>		: hb_integral_constant<unsigned short,		USHRT_MAX>	{};
template <> ಸಂರಚನಾ hb_int_max<signed ಪೂರ್ಣಾಙ್ಕ>		: hb_integral_constant<signed ಪೂರ್ಣಾಙ್ಕ,		INT_MAX>	{};
template <> ಸಂರಚನಾ hb_int_max<unsigned ಪೂರ್ಣಾಙ್ಕ>		: hb_integral_constant<unsigned ಪೂರ್ಣಾಙ್ಕ,		UINT_MAX>	{};
template <> ಸಂರಚನಾ hb_int_max<signed long>		: hb_integral_constant<signed long,		LONG_MAX>	{};
template <> ಸಂರಚನಾ hb_int_max<unsigned long>		: hb_integral_constant<unsigned long,		ULONG_MAX>	{};
template <> ಸಂರಚನಾ hb_int_max<signed long long>		: hb_integral_constant<signed long long,	LLONG_MAX>	{};
template <> ಸಂರಚನಾ hb_int_max<unsigned long long>	: hb_integral_constant<unsigned long long,	ULLONG_MAX>	{};
#define hb_int_max(T) hb_int_max<T>::value

#ಯದಿ defined(__GNUC__) && __GNUC__ < 5 && !defined(__clang__)
#define hb_is_trivially_copyable(T) __has_trivial_copy(T)
#define hb_is_trivially_copy_assignable(T) __has_trivial_assign(T)
#define hb_is_trivially_constructible(T) __has_trivial_constructor(T)
#define hb_is_trivially_copy_constructible(T) __has_trivial_copy_constructor(T)
#define hb_is_trivially_destructible(T) __has_trivial_destructor(T)
#ಅನ್ಯಥಾ
#define hb_is_trivially_copyable(T) std::is_trivially_copyable<T>::value
#define hb_is_trivially_copy_assignable(T) std::is_trivially_copy_assignable<T>::value
#define hb_is_trivially_constructible(T) std::is_trivially_constructible<T>::value
#define hb_is_trivially_copy_constructible(T) std::is_trivially_copy_constructible<T>::value
#define hb_is_trivially_destructible(T) std::is_trivially_destructible<T>::value
#endif

/* Class traits. */

#define HB_DELETE_COPY_ASSIGN(TypeName) \
  TypeName(ಧ್ರುವ TypeName&) = delete; \
  ಶೂನ್ಯ operator=(ಧ್ರುವ TypeName&) = delete
#define HB_DELETE_CREATE_COPY_ASSIGN(TypeName) \
  TypeName() = delete; \
  TypeName(ಧ್ರುವ TypeName&) = delete; \
  ಶೂನ್ಯ operator=(ಧ್ರುವ TypeName&) = delete

/* hb_unwrap_type (T)
 * If T has no T::type, returns T. Otherwise calls itself on T::type recursively.
 */

template <typename T, typename>
ಸಂರಚನಾ _hb_unwrap_type : hb_type_identity_t<T> {};
template <typename T>
ಸಂರಚನಾ _hb_unwrap_type<T, hb_void_t<typename T::type>> : _hb_unwrap_type<typename T::type, ಶೂನ್ಯ> {};
template <typename T>
using hb_unwrap_type = _hb_unwrap_type<T, ಶೂನ್ಯ>;
#define hb_unwrap_type(T) typename hb_unwrap_type<T>::type

#endif /* HB_META_HH */

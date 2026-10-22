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

#അന്തര്ഭാവ "hb.hh"

#അന്തര്ഭാവ <memory>
#അന്തര്ഭാവ <type_traits>
#അന്തര്ഭാവ <utility>


/*
 * C++ template meta-programming & fundamentals used with them.
 */

/* Void!  For when we need a expression-type of void. */
ഘടന hb_empty_t {};

/* https://en.cppreference.com/w/cpp/types/void_t */
template<typename... Ts> ഘടന _hb_void_t { തരനിർവചനം ശൂന്യം type; };
template<typename... Ts> using hb_void_t = typename _hb_void_t<Ts...>::type;

template<typename Head, typename... Ts> ഘടന _hb_head_t { തരനിർവചനം Head type; };
template<typename... Ts> using hb_head_t = typename _hb_head_t<Ts...>::type;

template <typename T, T v> ഘടന hb_integral_constant { സ്ഥിതം constexpr T value = v; };
template <തർക്കം b> using hb_bool_constant = hb_integral_constant<തർക്കം, b>;
using hb_true_type = hb_bool_constant<സത്യം>;
using hb_false_type = hb_bool_constant<അസത്യം>;

/* Static-assert as expression. */
template <തർക്കം cond> ഘടന static_assert_വ്യഞ്ജകം;
template <> ഘടന static_assert_വ്യഞ്ജകം<സത്യം> : hb_false_type {};
#define static_assert_വ്യഞ്ജകം(C) static_assert_വ്യഞ്ജകം<C>::value

/* Basic type SFINAE. */

template <തർക്കം B, typename T = ശൂന്യം> ഘടന hb_enable_if {};
template <typename T>                ഘടന hb_enable_if<സത്യം, T> { തരനിർവചനം T type; };
#define hb_enable_if(Cond) typename hb_enable_if<(Cond)>::type* = nullptr
/* Concepts/Requires alias: */
#define hb_requires(Cond) hb_enable_if((Cond))

template <typename T, typename T2> ഘടന hb_is_same : hb_false_type {};
template <typename T>              ഘടന hb_is_same<T, T> : hb_true_type {};
#define hb_is_same(T, T2) hb_is_same<T, T2>::value

/* Function overloading SFINAE and priority. */

#define HB_RETURN(Ret, E) -> hb_head_t<Ret, decltype ((E))> { തിരികെ (E); }
#define HB_AUTO_RETURN(E) -> decltype ((E)) { തിരികെ (E); }
#define HB_VOID_RETURN(E) -> hb_void_t<decltype ((E))> { (E); }

template <unsigned Pri> ഘടന hb_priority : hb_priority<Pri - 1> {};
template <>             ഘടന hb_priority<0> {};
#define hb_prioritize hb_priority<16> ()

#define HB_FUNCOBJ(x) static_const x HB_UNUSED


template <typename T> ഘടന hb_type_identity_t { തരനിർവചനം T type; };
template <typename T> using hb_type_identity = typename hb_type_identity_t<T>::type;

template <typename T> സ്ഥിതം വരിയിൽ T hb_declval ();
#define hb_declval(T) (hb_declval<T> ())

template <typename T> ഘടന hb_match_const		: hb_type_identity_t<T>, hb_false_type	{};
template <typename T> ഘടന hb_match_const<സ്ഥിരം T>	: hb_type_identity_t<T>, hb_true_type	{};
template <typename T> using hb_remove_const = typename hb_match_const<T>::type;

template <typename T> ഘടന hb_match_reference		: hb_type_identity_t<T>, hb_false_type	{};
template <typename T> ഘടന hb_match_reference<T &>	: hb_type_identity_t<T>, hb_true_type	{};
template <typename T> ഘടന hb_match_reference<T &&>	: hb_type_identity_t<T>, hb_true_type	{};
template <typename T> using hb_remove_reference = typename hb_match_reference<T>::type;
template <typename T> auto _hb_try_add_lvalue_reference (hb_priority<1>) -> hb_type_identity<T&>;
template <typename T> auto _hb_try_add_lvalue_reference (hb_priority<0>) -> hb_type_identity<T>;
template <typename T> using hb_add_lvalue_reference = decltype (_hb_try_add_lvalue_reference<T> (hb_prioritize));
template <typename T> auto _hb_try_add_rvalue_reference (hb_priority<1>) -> hb_type_identity<T&&>;
template <typename T> auto _hb_try_add_rvalue_reference (hb_priority<0>) -> hb_type_identity<T>;
template <typename T> using hb_add_rvalue_reference = decltype (_hb_try_add_rvalue_reference<T> (hb_prioritize));

template <typename T> ഘടന hb_match_pointer		: hb_type_identity_t<T>, hb_false_type	{};
template <typename T> ഘടന hb_match_pointer<T *>	: hb_type_identity_t<T>, hb_true_type	{};
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


ഘടന
{
  template <typename T> constexpr auto
  operator () (T&& v) സ്ഥിരം HB_AUTO_RETURN (std::forward<T> (v))

  template <typename T> constexpr auto
  operator () (T *v) സ്ഥിരം HB_AUTO_RETURN (*v)

  template <typename T> constexpr auto
  operator () (സ്ഥിരം hb::shared_ptr<T>& v) സ്ഥിരം HB_AUTO_RETURN (*v)

  template <typename T> constexpr auto
  operator () (hb::shared_ptr<T>& v) സ്ഥിരം HB_AUTO_RETURN (*v)
  
  template <typename T> constexpr auto
  operator () (സ്ഥിരം hb::unique_ptr<T>& v) സ്ഥിരം HB_AUTO_RETURN (*v)

  template <typename T> constexpr auto
  operator () (hb::unique_ptr<T>& v) സ്ഥിരം HB_AUTO_RETURN (*v)
}
HB_FUNCOBJ (hb_deref);

template <typename T>
ഘടന hb_reference_wrapper
{
  hb_reference_wrapper (T v) : v (v) {}
  തർക്കം operator == (സ്ഥിരം hb_reference_wrapper& o) സ്ഥിരം { തിരികെ v == o.v; }
  തർക്കം operator != (സ്ഥിരം hb_reference_wrapper& o) സ്ഥിരം { തിരികെ v != o.v; }
  operator T& () { തിരികെ v; }
  T& get () { തിരികെ v; }
  T v;
};
template <typename T>
ഘടന hb_reference_wrapper<T&>
{
  hb_reference_wrapper (T& v) : v (std::addressof (v)) {}
  തർക്കം operator == (സ്ഥിരം hb_reference_wrapper& o) സ്ഥിരം { തിരികെ v == o.v; }
  തർക്കം operator != (സ്ഥിരം hb_reference_wrapper& o) സ്ഥിരം { തിരികെ v != o.v; }
  operator T& () { തിരികെ *v; }
  T& get () { തിരികെ *v; }
  T* v;
};


/* Type traits */

template <typename T> ഘടന hb_int_min;
template <> ഘടന hb_int_min<അക്ഷരം>			: hb_integral_constant<അക്ഷരം,			CHAR_MIN>	{};
template <> ഘടന hb_int_min<signed അക്ഷരം>		: hb_integral_constant<signed അക്ഷരം,		SCHAR_MIN>	{};
template <> ഘടന hb_int_min<unsigned അക്ഷരം>		: hb_integral_constant<unsigned അക്ഷരം,		0>		{};
template <> ഘടന hb_int_min<signed short>		: hb_integral_constant<signed short,		SHRT_MIN>	{};
template <> ഘടന hb_int_min<unsigned short>		: hb_integral_constant<unsigned short,		0>		{};
template <> ഘടന hb_int_min<signed പൂർണ്ണസംഖ്യ>		: hb_integral_constant<signed പൂർണ്ണസംഖ്യ,		INT_MIN>	{};
template <> ഘടന hb_int_min<unsigned പൂർണ്ണസംഖ്യ>		: hb_integral_constant<unsigned പൂർണ്ണസംഖ്യ,		0>		{};
template <> ഘടന hb_int_min<signed long>		: hb_integral_constant<signed long,		LONG_MIN>	{};
template <> ഘടന hb_int_min<unsigned long>		: hb_integral_constant<unsigned long,		0>		{};
template <> ഘടന hb_int_min<signed long long>		: hb_integral_constant<signed long long,	LLONG_MIN>	{};
template <> ഘടന hb_int_min<unsigned long long>	: hb_integral_constant<unsigned long long,	0>		{};
template <typename T> ഘടന hb_int_min<T *>		: hb_integral_constant<T *,			nullptr>	{};
#define hb_int_min(T) hb_int_min<T>::value
template <typename T> ഘടന hb_int_max;
template <> ഘടന hb_int_max<അക്ഷരം>			: hb_integral_constant<അക്ഷരം,			CHAR_MAX>	{};
template <> ഘടന hb_int_max<signed അക്ഷരം>		: hb_integral_constant<signed അക്ഷരം,		SCHAR_MAX>	{};
template <> ഘടന hb_int_max<unsigned അക്ഷരം>		: hb_integral_constant<unsigned അക്ഷരം,		UCHAR_MAX>	{};
template <> ഘടന hb_int_max<signed short>		: hb_integral_constant<signed short,		SHRT_MAX>	{};
template <> ഘടന hb_int_max<unsigned short>		: hb_integral_constant<unsigned short,		USHRT_MAX>	{};
template <> ഘടന hb_int_max<signed പൂർണ്ണസംഖ്യ>		: hb_integral_constant<signed പൂർണ്ണസംഖ്യ,		INT_MAX>	{};
template <> ഘടന hb_int_max<unsigned പൂർണ്ണസംഖ്യ>		: hb_integral_constant<unsigned പൂർണ്ണസംഖ്യ,		UINT_MAX>	{};
template <> ഘടന hb_int_max<signed long>		: hb_integral_constant<signed long,		LONG_MAX>	{};
template <> ഘടന hb_int_max<unsigned long>		: hb_integral_constant<unsigned long,		ULONG_MAX>	{};
template <> ഘടന hb_int_max<signed long long>		: hb_integral_constant<signed long long,	LLONG_MAX>	{};
template <> ഘടന hb_int_max<unsigned long long>	: hb_integral_constant<unsigned long long,	ULLONG_MAX>	{};
#define hb_int_max(T) hb_int_max<T>::value

#എങ്കിൽ defined(__GNUC__) && __GNUC__ < 5 && !defined(__clang__)
#define hb_is_trivially_copyable(T) __has_trivial_copy(T)
#define hb_is_trivially_copy_assignable(T) __has_trivial_assign(T)
#define hb_is_trivially_constructible(T) __has_trivial_constructor(T)
#define hb_is_trivially_copy_constructible(T) __has_trivial_copy_constructor(T)
#define hb_is_trivially_destructible(T) __has_trivial_destructor(T)
#അല്ലെങ്കിൽ
#define hb_is_trivially_copyable(T) std::is_trivially_copyable<T>::value
#define hb_is_trivially_copy_assignable(T) std::is_trivially_copy_assignable<T>::value
#define hb_is_trivially_constructible(T) std::is_trivially_constructible<T>::value
#define hb_is_trivially_copy_constructible(T) std::is_trivially_copy_constructible<T>::value
#define hb_is_trivially_destructible(T) std::is_trivially_destructible<T>::value
#endif

/* Class traits. */

#define HB_DELETE_COPY_ASSIGN(TypeName) \
  TypeName(സ്ഥിരം TypeName&) = delete; \
  ശൂന്യം operator=(സ്ഥിരം TypeName&) = delete
#define HB_DELETE_CREATE_COPY_ASSIGN(TypeName) \
  TypeName() = delete; \
  TypeName(സ്ഥിരം TypeName&) = delete; \
  ശൂന്യം operator=(സ്ഥിരം TypeName&) = delete

/* hb_unwrap_type (T)
 * If T has no T::type, returns T. Otherwise calls itself on T::type recursively.
 */

template <typename T, typename>
ഘടന _hb_unwrap_type : hb_type_identity_t<T> {};
template <typename T>
ഘടന _hb_unwrap_type<T, hb_void_t<typename T::type>> : _hb_unwrap_type<typename T::type, ശൂന്യം> {};
template <typename T>
using hb_unwrap_type = _hb_unwrap_type<T, ശൂന്യം>;
#define hb_unwrap_type(T) typename hb_unwrap_type<T>::type

#endif /* HB_META_HH */

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

#ضمن "hb.hh"

#ضمن <memory>
#ضمن <type_traits>
#ضمن <utility>


/*
 * C++ template meta-programming & fundamentals used with them.
 */

/* Void!  For when we need a expression-type of void. */
بنية hb_empty_t {};

/* https://en.cppreference.com/w/cpp/types/void_t */
template<typename... Ts> بنية _hb_void_t { تعريفنوع فراغ type; };
template<typename... Ts> using hb_void_t = typename _hb_void_t<Ts...>::type;

template<typename Head, typename... Ts> بنية _hb_head_t { تعريفنوع Head type; };
template<typename... Ts> using hb_head_t = typename _hb_head_t<Ts...>::type;

template <typename T, T v> بنية hb_integral_constant { ساكن constexpr T value = v; };
template <منطقي b> using hb_bool_constant = hb_integral_constant<منطقي, b>;
using hb_true_type = hb_bool_constant<true>;
using hb_false_type = hb_bool_constant<خطأ>;

/* Static-assert as expression. */
template <منطقي cond> بنية static_assert_تعبير;
template <> بنية static_assert_تعبير<true> : hb_false_type {};
#define static_assert_تعبير(C) static_assert_تعبير<C>::value

/* Basic type SFINAE. */

template <منطقي B, typename T = فراغ> بنية hb_enable_if {};
template <typename T>                بنية hb_enable_if<true, T> { تعريفنوع T type; };
#define hb_enable_if(Cond) typename hb_enable_if<(Cond)>::type* = nullptr
/* Concepts/Requires alias: */
#define hb_requires(Cond) hb_enable_if((Cond))

template <typename T, typename T2> بنية hb_is_same : hb_false_type {};
template <typename T>              بنية hb_is_same<T, T> : hb_true_type {};
#define hb_is_same(T, T2) hb_is_same<T, T2>::value

/* Function overloading SFINAE and priority. */

#define HB_RETURN(Ret, E) -> hb_head_t<Ret, decltype ((E))> { أرجع (E); }
#define HB_AUTO_RETURN(E) -> decltype ((E)) { أرجع (E); }
#define HB_VOID_RETURN(E) -> hb_void_t<decltype ((E))> { (E); }

template <unsigned Pri> بنية hb_priority : hb_priority<Pri - 1> {};
template <>             بنية hb_priority<0> {};
#define hb_prioritize hb_priority<16> ()

#define HB_FUNCOBJ(x) static_const x HB_UNUSED


template <typename T> بنية hb_type_identity_t { تعريفنوع T type; };
template <typename T> using hb_type_identity = typename hb_type_identity_t<T>::type;

template <typename T> ساكن مضمن T hb_declval ();
#define hb_declval(T) (hb_declval<T> ())

template <typename T> بنية hb_match_const		: hb_type_identity_t<T>, hb_false_type	{};
template <typename T> بنية hb_match_const<ثابت T>	: hb_type_identity_t<T>, hb_true_type	{};
template <typename T> using hb_remove_const = typename hb_match_const<T>::type;

template <typename T> بنية hb_match_reference		: hb_type_identity_t<T>, hb_false_type	{};
template <typename T> بنية hb_match_reference<T &>	: hb_type_identity_t<T>, hb_true_type	{};
template <typename T> بنية hb_match_reference<T &&>	: hb_type_identity_t<T>, hb_true_type	{};
template <typename T> using hb_remove_reference = typename hb_match_reference<T>::type;
template <typename T> auto _hb_try_add_lvalue_reference (hb_priority<1>) -> hb_type_identity<T&>;
template <typename T> auto _hb_try_add_lvalue_reference (hb_priority<0>) -> hb_type_identity<T>;
template <typename T> using hb_add_lvalue_reference = decltype (_hb_try_add_lvalue_reference<T> (hb_prioritize));
template <typename T> auto _hb_try_add_rvalue_reference (hb_priority<1>) -> hb_type_identity<T&&>;
template <typename T> auto _hb_try_add_rvalue_reference (hb_priority<0>) -> hb_type_identity<T>;
template <typename T> using hb_add_rvalue_reference = decltype (_hb_try_add_rvalue_reference<T> (hb_prioritize));

template <typename T> بنية hb_match_pointer		: hb_type_identity_t<T>, hb_false_type	{};
template <typename T> بنية hb_match_pointer<T *>	: hb_type_identity_t<T>, hb_true_type	{};
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


بنية
{
  template <typename T> constexpr auto
  operator () (T&& v) ثابت HB_AUTO_RETURN (std::forward<T> (v))

  template <typename T> constexpr auto
  operator () (T *v) ثابت HB_AUTO_RETURN (*v)

  template <typename T> constexpr auto
  operator () (ثابت hb::shared_ptr<T>& v) ثابت HB_AUTO_RETURN (*v)

  template <typename T> constexpr auto
  operator () (hb::shared_ptr<T>& v) ثابت HB_AUTO_RETURN (*v)
  
  template <typename T> constexpr auto
  operator () (ثابت hb::unique_ptr<T>& v) ثابت HB_AUTO_RETURN (*v)

  template <typename T> constexpr auto
  operator () (hb::unique_ptr<T>& v) ثابت HB_AUTO_RETURN (*v)
}
HB_FUNCOBJ (hb_deref);

template <typename T>
بنية hb_reference_wrapper
{
  hb_reference_wrapper (T v) : v (v) {}
  منطقي operator == (ثابت hb_reference_wrapper& o) ثابت { أرجع v == o.v; }
  منطقي operator != (ثابت hb_reference_wrapper& o) ثابت { أرجع v != o.v; }
  operator T& () { أرجع v; }
  T& get () { أرجع v; }
  T v;
};
template <typename T>
بنية hb_reference_wrapper<T&>
{
  hb_reference_wrapper (T& v) : v (std::addressof (v)) {}
  منطقي operator == (ثابت hb_reference_wrapper& o) ثابت { أرجع v == o.v; }
  منطقي operator != (ثابت hb_reference_wrapper& o) ثابت { أرجع v != o.v; }
  operator T& () { أرجع *v; }
  T& get () { أرجع *v; }
  T* v;
};


/* Type traits */

template <typename T> بنية hb_int_min;
template <> بنية hb_int_min<حرف>			: hb_integral_constant<حرف,			CHAR_MIN>	{};
template <> بنية hb_int_min<signed حرف>		: hb_integral_constant<signed حرف,		SCHAR_MIN>	{};
template <> بنية hb_int_min<unsigned حرف>		: hb_integral_constant<unsigned حرف,		0>		{};
template <> بنية hb_int_min<signed short>		: hb_integral_constant<signed short,		SHRT_MIN>	{};
template <> بنية hb_int_min<unsigned short>		: hb_integral_constant<unsigned short,		0>		{};
template <> بنية hb_int_min<signed صحيح>		: hb_integral_constant<signed صحيح,		INT_MIN>	{};
template <> بنية hb_int_min<unsigned صحيح>		: hb_integral_constant<unsigned صحيح,		0>		{};
template <> بنية hb_int_min<signed long>		: hb_integral_constant<signed long,		LONG_MIN>	{};
template <> بنية hb_int_min<unsigned long>		: hb_integral_constant<unsigned long,		0>		{};
template <> بنية hb_int_min<signed long long>		: hb_integral_constant<signed long long,	LLONG_MIN>	{};
template <> بنية hb_int_min<unsigned long long>	: hb_integral_constant<unsigned long long,	0>		{};
template <typename T> بنية hb_int_min<T *>		: hb_integral_constant<T *,			nullptr>	{};
#define hb_int_min(T) hb_int_min<T>::value
template <typename T> بنية hb_int_max;
template <> بنية hb_int_max<حرف>			: hb_integral_constant<حرف,			CHAR_MAX>	{};
template <> بنية hb_int_max<signed حرف>		: hb_integral_constant<signed حرف,		SCHAR_MAX>	{};
template <> بنية hb_int_max<unsigned حرف>		: hb_integral_constant<unsigned حرف,		UCHAR_MAX>	{};
template <> بنية hb_int_max<signed short>		: hb_integral_constant<signed short,		SHRT_MAX>	{};
template <> بنية hb_int_max<unsigned short>		: hb_integral_constant<unsigned short,		USHRT_MAX>	{};
template <> بنية hb_int_max<signed صحيح>		: hb_integral_constant<signed صحيح,		INT_MAX>	{};
template <> بنية hb_int_max<unsigned صحيح>		: hb_integral_constant<unsigned صحيح,		UINT_MAX>	{};
template <> بنية hb_int_max<signed long>		: hb_integral_constant<signed long,		LONG_MAX>	{};
template <> بنية hb_int_max<unsigned long>		: hb_integral_constant<unsigned long,		ULONG_MAX>	{};
template <> بنية hb_int_max<signed long long>		: hb_integral_constant<signed long long,	LLONG_MAX>	{};
template <> بنية hb_int_max<unsigned long long>	: hb_integral_constant<unsigned long long,	ULLONG_MAX>	{};
#define hb_int_max(T) hb_int_max<T>::value

#إذا defined(__GNUC__) && __GNUC__ < 5 && !defined(__clang__)
#define hb_is_trivially_copyable(T) __has_trivial_copy(T)
#define hb_is_trivially_copy_assignable(T) __has_trivial_assign(T)
#define hb_is_trivially_constructible(T) __has_trivial_constructor(T)
#define hb_is_trivially_copy_constructible(T) __has_trivial_copy_constructor(T)
#define hb_is_trivially_destructible(T) __has_trivial_destructor(T)
#وإلا
#define hb_is_trivially_copyable(T) std::is_trivially_copyable<T>::value
#define hb_is_trivially_copy_assignable(T) std::is_trivially_copy_assignable<T>::value
#define hb_is_trivially_constructible(T) std::is_trivially_constructible<T>::value
#define hb_is_trivially_copy_constructible(T) std::is_trivially_copy_constructible<T>::value
#define hb_is_trivially_destructible(T) std::is_trivially_destructible<T>::value
#endif

/* Class traits. */

#define HB_DELETE_COPY_ASSIGN(TypeName) \
  TypeName(ثابت TypeName&) = delete; \
  فراغ operator=(ثابت TypeName&) = delete
#define HB_DELETE_CREATE_COPY_ASSIGN(TypeName) \
  TypeName() = delete; \
  TypeName(ثابت TypeName&) = delete; \
  فراغ operator=(ثابت TypeName&) = delete

/* hb_unwrap_type (T)
 * If T has no T::type, returns T. Otherwise calls itself on T::type recursively.
 */

template <typename T, typename>
بنية _hb_unwrap_type : hb_type_identity_t<T> {};
template <typename T>
بنية _hb_unwrap_type<T, hb_void_t<typename T::type>> : _hb_unwrap_type<typename T::type, فراغ> {};
template <typename T>
using hb_unwrap_type = _hb_unwrap_type<T, فراغ>;
#define hb_unwrap_type(T) typename hb_unwrap_type<T>::type

#endif /* HB_META_HH */

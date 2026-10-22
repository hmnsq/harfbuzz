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

#অন্তর্ভাব "hb.hh"

#অন্তর্ভাব <memory>
#অন্তর্ভাব <type_traits>
#অন্তর্ভাব <utility>


/*
 * C++ template meta-programming & fundamentals used with them.
 */

/* Void!  For when we need a expression-type of void. */
কাঠামো hb_empty_t {};

/* https://en.cppreference.com/w/cpp/types/void_t */
template<typename... Ts> কাঠামো _hb_void_t { প্রকারলেখ শূন্য type; };
template<typename... Ts> using hb_void_t = typename _hb_void_t<Ts...>::type;

template<typename শীর্ষ, typename... Ts> কাঠামো hb_শীর্ষ_t { প্রকারলেখ শীর্ষ type; };
template<typename... Ts> using hb_শীর্ষ_t_2 = typename hb_শীর্ষ_t<Ts...>::type;

template <typename T, T v> কাঠামো hb_integral_constant { স্থির constexpr T value = v; };
template <যৌক্তিক b> using hb_bool_constant = hb_integral_constant<যৌক্তিক, b>;
using hb_true_type = hb_bool_constant<সত্য>;
using hb_false_type = hb_bool_constant<মিথ্যা>;

/* Static-assert as expression. */
template <যৌক্তিক cond> কাঠামো static_assert_রাশিমালা;
template <> কাঠামো static_assert_রাশিমালা<সত্য> : hb_false_type {};
#define static_assert_রাশিমালা(C) static_assert_রাশিমালা<C>::value

/* Basic type SFINAE. */

template <যৌক্তিক B, typename T = শূন্য> কাঠামো hb_enable_if {};
template <typename T>                কাঠামো hb_enable_if<সত্য, T> { প্রকারলেখ T type; };
#define hb_enable_if(Cond) typename hb_enable_if<(Cond)>::type* = nullptr
/* Concepts/Requires alias: */
#define hb_requires(Cond) hb_enable_if((Cond))

template <typename T, typename T2> কাঠামো hb_is_same : hb_false_type {};
template <typename T>              কাঠামো hb_is_same<T, T> : hb_true_type {};
#define hb_is_same(T, T2) hb_is_same<T, T2>::value

/* Function overloading SFINAE and priority. */

#define HB_RETURN(Ret, E) -> hb_শীর্ষ_t_2<Ret, decltype ((E))> { ফেরত (E); }
#define HB_AUTO_RETURN(E) -> decltype ((E)) { ফেরত (E); }
#define HB_VOID_RETURN(E) -> hb_void_t<decltype ((E))> { (E); }

template <unsigned Pri> কাঠামো hb_priority : hb_priority<Pri - 1> {};
template <>             কাঠামো hb_priority<0> {};
#define hb_prioritize hb_priority<16> ()

#define HB_FUNCOBJ(x) static_const x HB_UNUSED


template <typename T> কাঠামো hb_type_identity_t { প্রকারলেখ T type; };
template <typename T> using hb_type_identity = typename hb_type_identity_t<T>::type;

template <typename T> স্থির অন্তঃরেখ T hb_declval ();
#define hb_declval(T) (hb_declval<T> ())

template <typename T> কাঠামো hb_match_const		: hb_type_identity_t<T>, hb_false_type	{};
template <typename T> কাঠামো hb_match_const<ধ্রুবক T>	: hb_type_identity_t<T>, hb_true_type	{};
template <typename T> using hb_remove_const = typename hb_match_const<T>::type;

template <typename T> কাঠামো hb_match_reference		: hb_type_identity_t<T>, hb_false_type	{};
template <typename T> কাঠামো hb_match_reference<T &>	: hb_type_identity_t<T>, hb_true_type	{};
template <typename T> কাঠামো hb_match_reference<T &&>	: hb_type_identity_t<T>, hb_true_type	{};
template <typename T> using hb_remove_reference = typename hb_match_reference<T>::type;
template <typename T> auto _hb_try_add_lvalue_reference (hb_priority<1>) -> hb_type_identity<T&>;
template <typename T> auto _hb_try_add_lvalue_reference (hb_priority<0>) -> hb_type_identity<T>;
template <typename T> using hb_add_lvalue_reference = decltype (_hb_try_add_lvalue_reference<T> (hb_prioritize));
template <typename T> auto _hb_try_add_rvalue_reference (hb_priority<1>) -> hb_type_identity<T&&>;
template <typename T> auto _hb_try_add_rvalue_reference (hb_priority<0>) -> hb_type_identity<T>;
template <typename T> using hb_add_rvalue_reference = decltype (_hb_try_add_rvalue_reference<T> (hb_prioritize));

template <typename T> কাঠামো hb_match_pointer		: hb_type_identity_t<T>, hb_false_type	{};
template <typename T> কাঠামো hb_match_pointer<T *>	: hb_type_identity_t<T>, hb_true_type	{};
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


কাঠামো
{
  template <typename T> constexpr auto
  operator () (T&& v) ধ্রুবক HB_AUTO_RETURN (std::forward<T> (v))

  template <typename T> constexpr auto
  operator () (T *v) ধ্রুবক HB_AUTO_RETURN (*v)

  template <typename T> constexpr auto
  operator () (ধ্রুবক hb::shared_ptr<T>& v) ধ্রুবক HB_AUTO_RETURN (*v)

  template <typename T> constexpr auto
  operator () (hb::shared_ptr<T>& v) ধ্রুবক HB_AUTO_RETURN (*v)
  
  template <typename T> constexpr auto
  operator () (ধ্রুবক hb::unique_ptr<T>& v) ধ্রুবক HB_AUTO_RETURN (*v)

  template <typename T> constexpr auto
  operator () (hb::unique_ptr<T>& v) ধ্রুবক HB_AUTO_RETURN (*v)
}
HB_FUNCOBJ (hb_deref);

template <typename T>
কাঠামো hb_reference_wrapper
{
  hb_reference_wrapper (T v) : v (v) {}
  যৌক্তিক operator == (ধ্রুবক hb_reference_wrapper& o) ধ্রুবক { ফেরত v == o.v; }
  যৌক্তিক operator != (ধ্রুবক hb_reference_wrapper& o) ধ্রুবক { ফেরত v != o.v; }
  operator T& () { ফেরত v; }
  T& get () { ফেরত v; }
  T v;
};
template <typename T>
কাঠামো hb_reference_wrapper<T&>
{
  hb_reference_wrapper (T& v) : v (std::addressof (v)) {}
  যৌক্তিক operator == (ধ্রুবক hb_reference_wrapper& o) ধ্রুবক { ফেরত v == o.v; }
  যৌক্তিক operator != (ধ্রুবক hb_reference_wrapper& o) ধ্রুবক { ফেরত v != o.v; }
  operator T& () { ফেরত *v; }
  T& get () { ফেরত *v; }
  T* v;
};


/* Type traits */

template <typename T> কাঠামো hb_int_min;
template <> কাঠামো hb_int_min<অক্ষর>			: hb_integral_constant<অক্ষর,			CHAR_MIN>	{};
template <> কাঠামো hb_int_min<signed অক্ষর>		: hb_integral_constant<signed অক্ষর,		SCHAR_MIN>	{};
template <> কাঠামো hb_int_min<unsigned অক্ষর>		: hb_integral_constant<unsigned অক্ষর,		0>		{};
template <> কাঠামো hb_int_min<signed short>		: hb_integral_constant<signed short,		SHRT_MIN>	{};
template <> কাঠামো hb_int_min<unsigned short>		: hb_integral_constant<unsigned short,		0>		{};
template <> কাঠামো hb_int_min<signed পূর্ণসংখ্যা>		: hb_integral_constant<signed পূর্ণসংখ্যা,		INT_MIN>	{};
template <> কাঠামো hb_int_min<unsigned পূর্ণসংখ্যা>		: hb_integral_constant<unsigned পূর্ণসংখ্যা,		0>		{};
template <> কাঠামো hb_int_min<signed long>		: hb_integral_constant<signed long,		LONG_MIN>	{};
template <> কাঠামো hb_int_min<unsigned long>		: hb_integral_constant<unsigned long,		0>		{};
template <> কাঠামো hb_int_min<signed long long>		: hb_integral_constant<signed long long,	LLONG_MIN>	{};
template <> কাঠামো hb_int_min<unsigned long long>	: hb_integral_constant<unsigned long long,	0>		{};
template <typename T> কাঠামো hb_int_min<T *>		: hb_integral_constant<T *,			nullptr>	{};
#define hb_int_min(T) hb_int_min<T>::value
template <typename T> কাঠামো hb_int_max;
template <> কাঠামো hb_int_max<অক্ষর>			: hb_integral_constant<অক্ষর,			CHAR_MAX>	{};
template <> কাঠামো hb_int_max<signed অক্ষর>		: hb_integral_constant<signed অক্ষর,		SCHAR_MAX>	{};
template <> কাঠামো hb_int_max<unsigned অক্ষর>		: hb_integral_constant<unsigned অক্ষর,		UCHAR_MAX>	{};
template <> কাঠামো hb_int_max<signed short>		: hb_integral_constant<signed short,		SHRT_MAX>	{};
template <> কাঠামো hb_int_max<unsigned short>		: hb_integral_constant<unsigned short,		USHRT_MAX>	{};
template <> কাঠামো hb_int_max<signed পূর্ণসংখ্যা>		: hb_integral_constant<signed পূর্ণসংখ্যা,		INT_MAX>	{};
template <> কাঠামো hb_int_max<unsigned পূর্ণসংখ্যা>		: hb_integral_constant<unsigned পূর্ণসংখ্যা,		UINT_MAX>	{};
template <> কাঠামো hb_int_max<signed long>		: hb_integral_constant<signed long,		LONG_MAX>	{};
template <> কাঠামো hb_int_max<unsigned long>		: hb_integral_constant<unsigned long,		ULONG_MAX>	{};
template <> কাঠামো hb_int_max<signed long long>		: hb_integral_constant<signed long long,	LLONG_MAX>	{};
template <> কাঠামো hb_int_max<unsigned long long>	: hb_integral_constant<unsigned long long,	ULLONG_MAX>	{};
#define hb_int_max(T) hb_int_max<T>::value

#যদি defined(__GNUC__) && __GNUC__ < 5 && !defined(__clang__)
#define hb_is_trivially_copyable(T) __has_trivial_copy(T)
#define hb_is_trivially_copy_assignable(T) __has_trivial_assign(T)
#define hb_is_trivially_constructible(T) __has_trivial_constructor(T)
#define hb_is_trivially_copy_constructible(T) __has_trivial_copy_constructor(T)
#define hb_is_trivially_destructible(T) __has_trivial_destructor(T)
#অন্যথা
#define hb_is_trivially_copyable(T) std::is_trivially_copyable<T>::value
#define hb_is_trivially_copy_assignable(T) std::is_trivially_copy_assignable<T>::value
#define hb_is_trivially_constructible(T) std::is_trivially_constructible<T>::value
#define hb_is_trivially_copy_constructible(T) std::is_trivially_copy_constructible<T>::value
#define hb_is_trivially_destructible(T) std::is_trivially_destructible<T>::value
#endif

/* Class traits. */

#define HB_DELETE_COPY_ASSIGN(TypeName) \
  TypeName(ধ্রুবক TypeName&) = delete; \
  শূন্য operator=(ধ্রুবক TypeName&) = delete
#define HB_DELETE_CREATE_COPY_ASSIGN(TypeName) \
  TypeName() = delete; \
  TypeName(ধ্রুবক TypeName&) = delete; \
  শূন্য operator=(ধ্রুবক TypeName&) = delete

/* hb_unwrap_type (T)
 * If T has no T::type, returns T. Otherwise calls itself on T::type recursively.
 */

template <typename T, typename>
কাঠামো _hb_unwrap_type : hb_type_identity_t<T> {};
template <typename T>
কাঠামো _hb_unwrap_type<T, hb_void_t<typename T::type>> : _hb_unwrap_type<typename T::type, শূন্য> {};
template <typename T>
using hb_unwrap_type = _hb_unwrap_type<T, শূন্য>;
#define hb_unwrap_type(T) typename hb_unwrap_type<T>::type

#endif /* HB_META_HH */

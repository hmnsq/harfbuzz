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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ଅନ୍ତର୍ଭାଵ <memory>
#ଅନ୍ତର୍ଭାଵ <type_traits>
#ଅନ୍ତର୍ଭାଵ <utility>


/*
 * C++ template meta-programming & fundamentals used with them.
 */

/* Void!  For when we need a expression-type of void. */
ସଂରଚନା hb_empty_t {};

/* https://en.cppreference.com/w/cpp/types/void_t */
template<typename... Ts> ସଂରଚନା _hb_void_t { ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ type; };
template<typename... Ts> using hb_void_t = typename _hb_void_t<Ts...>::type;

template<typename Head, typename... Ts> ସଂରଚନା _hb_head_t { ପ୍ରକାରବ୍ୟାଖ୍ୟା Head type; };
template<typename... Ts> using hb_head_t = typename _hb_head_t<Ts...>::type;

template <typename T, T v> ସଂରଚନା hb_integral_constant { ସ୍ଥିର constexpr T value = v; };
template <ତାର୍କିକ b> using hb_bool_constant = hb_integral_constant<ତାର୍କିକ, b>;
using hb_true_type = hb_bool_constant<ସତ୍ୟ>;
using hb_false_type = hb_bool_constant<ମିଥ୍ୟା>;

/* Static-assert as expression. */
template <ତାର୍କିକ cond> ସଂରଚନା static_assert_ପଦସମଷ୍ଟି;
template <> ସଂରଚନା static_assert_ପଦସମଷ୍ଟି<ସତ୍ୟ> : hb_false_type {};
#define static_assert_ପଦସମଷ୍ଟି(C) static_assert_ପଦସମଷ୍ଟି<C>::value

/* Basic type SFINAE. */

template <ତାର୍କିକ B, typename T = ଶୂନ୍ୟ> ସଂରଚନା hb_enable_if {};
template <typename T>                ସଂରଚନା hb_enable_if<ସତ୍ୟ, T> { ପ୍ରକାରବ୍ୟାଖ୍ୟା T type; };
#define hb_enable_if(Cond) typename hb_enable_if<(Cond)>::type* = nullptr
/* Concepts/Requires alias: */
#define hb_requires(Cond) hb_enable_if((Cond))

template <typename T, typename T2> ସଂରଚନା hb_is_same : hb_false_type {};
template <typename T>              ସଂରଚନା hb_is_same<T, T> : hb_true_type {};
#define hb_is_same(T, T2) hb_is_same<T, T2>::value

/* Function overloading SFINAE and priority. */

#define HB_RETURN(Ret, E) -> hb_head_t<Ret, decltype ((E))> { ଫେରନ୍ତୁ (E); }
#define HB_AUTO_RETURN(E) -> decltype ((E)) { ଫେରନ୍ତୁ (E); }
#define HB_VOID_RETURN(E) -> hb_void_t<decltype ((E))> { (E); }

template <unsigned Pri> ସଂରଚନା hb_priority : hb_priority<Pri - 1> {};
template <>             ସଂରଚନା hb_priority<0> {};
#define hb_prioritize hb_priority<16> ()

#define HB_FUNCOBJ(x) static_const x HB_UNUSED


template <typename T> ସଂରଚନା hb_type_identity_t { ପ୍ରକାରବ୍ୟାଖ୍ୟା T type; };
template <typename T> using hb_type_identity = typename hb_type_identity_t<T>::type;

template <typename T> ସ୍ଥିର ଧାଡ଼ିଭିତରେ T hb_declval ();
#define hb_declval(T) (hb_declval<T> ())

template <typename T> ସଂରଚନା hb_match_const		: hb_type_identity_t<T>, hb_false_type	{};
template <typename T> ସଂରଚନା hb_match_const<ଅଚଳ T>	: hb_type_identity_t<T>, hb_true_type	{};
template <typename T> using hb_remove_const = typename hb_match_const<T>::type;

template <typename T> ସଂରଚନା hb_match_reference		: hb_type_identity_t<T>, hb_false_type	{};
template <typename T> ସଂରଚନା hb_match_reference<T &>	: hb_type_identity_t<T>, hb_true_type	{};
template <typename T> ସଂରଚନା hb_match_reference<T &&>	: hb_type_identity_t<T>, hb_true_type	{};
template <typename T> using hb_remove_reference = typename hb_match_reference<T>::type;
template <typename T> auto _hb_try_add_lvalue_reference (hb_priority<1>) -> hb_type_identity<T&>;
template <typename T> auto _hb_try_add_lvalue_reference (hb_priority<0>) -> hb_type_identity<T>;
template <typename T> using hb_add_lvalue_reference = decltype (_hb_try_add_lvalue_reference<T> (hb_prioritize));
template <typename T> auto _hb_try_add_rvalue_reference (hb_priority<1>) -> hb_type_identity<T&&>;
template <typename T> auto _hb_try_add_rvalue_reference (hb_priority<0>) -> hb_type_identity<T>;
template <typename T> using hb_add_rvalue_reference = decltype (_hb_try_add_rvalue_reference<T> (hb_prioritize));

template <typename T> ସଂରଚନା hb_match_pointer		: hb_type_identity_t<T>, hb_false_type	{};
template <typename T> ସଂରଚନା hb_match_pointer<T *>	: hb_type_identity_t<T>, hb_true_type	{};
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


ସଂରଚନା
{
  template <typename T> constexpr auto
  operator () (T&& v) ଅଚଳ HB_AUTO_RETURN (std::forward<T> (v))

  template <typename T> constexpr auto
  operator () (T *v) ଅଚଳ HB_AUTO_RETURN (*v)

  template <typename T> constexpr auto
  operator () (ଅଚଳ hb::shared_ptr<T>& v) ଅଚଳ HB_AUTO_RETURN (*v)

  template <typename T> constexpr auto
  operator () (hb::shared_ptr<T>& v) ଅଚଳ HB_AUTO_RETURN (*v)
  
  template <typename T> constexpr auto
  operator () (ଅଚଳ hb::unique_ptr<T>& v) ଅଚଳ HB_AUTO_RETURN (*v)

  template <typename T> constexpr auto
  operator () (hb::unique_ptr<T>& v) ଅଚଳ HB_AUTO_RETURN (*v)
}
HB_FUNCOBJ (hb_deref);

template <typename T>
ସଂରଚନା hb_reference_wrapper
{
  hb_reference_wrapper (T v) : v (v) {}
  ତାର୍କିକ operator == (ଅଚଳ hb_reference_wrapper& o) ଅଚଳ { ଫେରନ୍ତୁ v == o.v; }
  ତାର୍କିକ operator != (ଅଚଳ hb_reference_wrapper& o) ଅଚଳ { ଫେରନ୍ତୁ v != o.v; }
  operator T& () { ଫେରନ୍ତୁ v; }
  T& get () { ଫେରନ୍ତୁ v; }
  T v;
};
template <typename T>
ସଂରଚନା hb_reference_wrapper<T&>
{
  hb_reference_wrapper (T& v) : v (std::addressof (v)) {}
  ତାର୍କିକ operator == (ଅଚଳ hb_reference_wrapper& o) ଅଚଳ { ଫେରନ୍ତୁ v == o.v; }
  ତାର୍କିକ operator != (ଅଚଳ hb_reference_wrapper& o) ଅଚଳ { ଫେରନ୍ତୁ v != o.v; }
  operator T& () { ଫେରନ୍ତୁ *v; }
  T& get () { ଫେରନ୍ତୁ *v; }
  T* v;
};


/* Type traits */

template <typename T> ସଂରଚନା hb_int_min;
template <> ସଂରଚନା hb_int_min<ଅକ୍ଷର>			: hb_integral_constant<ଅକ୍ଷର,			CHAR_MIN>	{};
template <> ସଂରଚନା hb_int_min<signed ଅକ୍ଷର>		: hb_integral_constant<signed ଅକ୍ଷର,		SCHAR_MIN>	{};
template <> ସଂରଚନା hb_int_min<unsigned ଅକ୍ଷର>		: hb_integral_constant<unsigned ଅକ୍ଷର,		0>		{};
template <> ସଂରଚନା hb_int_min<signed short>		: hb_integral_constant<signed short,		SHRT_MIN>	{};
template <> ସଂରଚନା hb_int_min<unsigned short>		: hb_integral_constant<unsigned short,		0>		{};
template <> ସଂରଚନା hb_int_min<signed ପୂର୍ଣ୍ଣସଂଖ୍ୟା>		: hb_integral_constant<signed ପୂର୍ଣ୍ଣସଂଖ୍ୟା,		INT_MIN>	{};
template <> ସଂରଚନା hb_int_min<unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା>		: hb_integral_constant<unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା,		0>		{};
template <> ସଂରଚନା hb_int_min<signed long>		: hb_integral_constant<signed long,		LONG_MIN>	{};
template <> ସଂରଚନା hb_int_min<unsigned long>		: hb_integral_constant<unsigned long,		0>		{};
template <> ସଂରଚନା hb_int_min<signed long long>		: hb_integral_constant<signed long long,	LLONG_MIN>	{};
template <> ସଂରଚନା hb_int_min<unsigned long long>	: hb_integral_constant<unsigned long long,	0>		{};
template <typename T> ସଂରଚନା hb_int_min<T *>		: hb_integral_constant<T *,			nullptr>	{};
#define hb_int_min(T) hb_int_min<T>::value
template <typename T> ସଂରଚନା hb_int_max;
template <> ସଂରଚନା hb_int_max<ଅକ୍ଷର>			: hb_integral_constant<ଅକ୍ଷର,			CHAR_MAX>	{};
template <> ସଂରଚନା hb_int_max<signed ଅକ୍ଷର>		: hb_integral_constant<signed ଅକ୍ଷର,		SCHAR_MAX>	{};
template <> ସଂରଚନା hb_int_max<unsigned ଅକ୍ଷର>		: hb_integral_constant<unsigned ଅକ୍ଷର,		UCHAR_MAX>	{};
template <> ସଂରଚନା hb_int_max<signed short>		: hb_integral_constant<signed short,		SHRT_MAX>	{};
template <> ସଂରଚନା hb_int_max<unsigned short>		: hb_integral_constant<unsigned short,		USHRT_MAX>	{};
template <> ସଂରଚନା hb_int_max<signed ପୂର୍ଣ୍ଣସଂଖ୍ୟା>		: hb_integral_constant<signed ପୂର୍ଣ୍ଣସଂଖ୍ୟା,		INT_MAX>	{};
template <> ସଂରଚନା hb_int_max<unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା>		: hb_integral_constant<unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା,		UINT_MAX>	{};
template <> ସଂରଚନା hb_int_max<signed long>		: hb_integral_constant<signed long,		LONG_MAX>	{};
template <> ସଂରଚନା hb_int_max<unsigned long>		: hb_integral_constant<unsigned long,		ULONG_MAX>	{};
template <> ସଂରଚନା hb_int_max<signed long long>		: hb_integral_constant<signed long long,	LLONG_MAX>	{};
template <> ସଂରଚନା hb_int_max<unsigned long long>	: hb_integral_constant<unsigned long long,	ULLONG_MAX>	{};
#define hb_int_max(T) hb_int_max<T>::value

#ଯଦି defined(__GNUC__) && __GNUC__ < 5 && !defined(__clang__)
#define hb_is_trivially_copyable(T) __has_trivial_copy(T)
#define hb_is_trivially_copy_assignable(T) __has_trivial_assign(T)
#define hb_is_trivially_constructible(T) __has_trivial_constructor(T)
#define hb_is_trivially_copy_constructible(T) __has_trivial_copy_constructor(T)
#define hb_is_trivially_destructible(T) __has_trivial_destructor(T)
#ଅନ୍ୟଥା
#define hb_is_trivially_copyable(T) std::is_trivially_copyable<T>::value
#define hb_is_trivially_copy_assignable(T) std::is_trivially_copy_assignable<T>::value
#define hb_is_trivially_constructible(T) std::is_trivially_constructible<T>::value
#define hb_is_trivially_copy_constructible(T) std::is_trivially_copy_constructible<T>::value
#define hb_is_trivially_destructible(T) std::is_trivially_destructible<T>::value
#endif

/* Class traits. */

#define HB_DELETE_COPY_ASSIGN(TypeName) \
  TypeName(ଅଚଳ TypeName&) = delete; \
  ଶୂନ୍ୟ operator=(ଅଚଳ TypeName&) = delete
#define HB_DELETE_CREATE_COPY_ASSIGN(TypeName) \
  TypeName() = delete; \
  TypeName(ଅଚଳ TypeName&) = delete; \
  ଶୂନ୍ୟ operator=(ଅଚଳ TypeName&) = delete

/* hb_unwrap_type (T)
 * If T has no T::type, returns T. Otherwise calls itself on T::type recursively.
 */

template <typename T, typename>
ସଂରଚନା _hb_unwrap_type : hb_type_identity_t<T> {};
template <typename T>
ସଂରଚନା _hb_unwrap_type<T, hb_void_t<typename T::type>> : _hb_unwrap_type<typename T::type, ଶୂନ୍ୟ> {};
template <typename T>
using hb_unwrap_type = _hb_unwrap_type<T, ଶୂନ୍ୟ>;
#define hb_unwrap_type(T) typename hb_unwrap_type<T>::type

#endif /* HB_META_HH */

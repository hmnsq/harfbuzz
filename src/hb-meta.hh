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

#اگر_غیر_تعریف_شدہ HB_META_HH
#تعریف_کرو HB_META_HH

#شامل "hb.hh"

#شامل <memory>
#شامل <type_traits>
#شامل <utility>


/*
 * C++ template meta-programming & fundamentals used with them.
 */

/* Void!  For when we need a expression-type of void. */
ساخت hb_خالی_خانے_t {};

/* https://en.cppreference.com/w/cpp/types/void_t */
سانچہ<قسم_نام... Ts> ساخت _hb_void_t { قسمتعریف خالی type; };
سانچہ<قسم_نام... Ts> استعمال hb_void_t = قسم_نام _hb_void_t<Ts...>::type;

سانچہ<قسم_نام سر, قسم_نام... Ts> ساخت hb_سر_t { قسمتعریف سر type; };
سانچہ<قسم_نام... Ts> استعمال hb_سر_t_2 = قسم_نام hb_سر_t<Ts...>::type;

سانچہ <قسم_نام T, T v> ساخت hb_integral_constant { ساکن constexpr T value = v; };
سانچہ <منطقی b> استعمال hb_bool_constant = hb_integral_constant<منطقی, b>;
استعمال hb_true_type = hb_bool_constant<سچ>;
استعمال hb_false_type = hb_bool_constant<جھوٹ>;

/* Static-assert as expression. */
سانچہ <منطقی cond> ساخت static_assert_اظہاریہ;
سانچہ <> ساخت static_assert_اظہاریہ<سچ> : hb_false_type {};
#تعریف_کرو static_assert_اظہاریہ(C) static_assert_اظہاریہ<C>::value

/* Basic type SFINAE. */

سانچہ <منطقی B, قسم_نام T = خالی> ساخت hb_enable_if {};
سانچہ <قسم_نام T>                ساخت hb_enable_if<سچ, T> { قسمتعریف T type; };
#تعریف_کرو hb_enable_if(Cond) قسم_نام hb_enable_if<(Cond)>::type* = خالی_اشاریہ
/* Concepts/Requires alias: */
#تعریف_کرو hb_requires(Cond) hb_enable_if((Cond))

سانچہ <قسم_نام T, قسم_نام T2> ساخت hb_is_same : hb_false_type {};
سانچہ <قسم_نام T>              ساخت hb_is_same<T, T> : hb_true_type {};
#تعریف_کرو hb_is_same(T, T2) hb_is_same<T, T2>::value

/* Function overloading SFINAE and priority. */

#تعریف_کرو HB_RETURN(Ret, E) -> hb_سر_t_2<Ret, decltype ((E))> { واپس (E); }
#تعریف_کرو HB_AUTO_RETURN(E) -> decltype ((E)) { واپس (E); }
#تعریف_کرو HB_VOID_RETURN(E) -> hb_void_t<decltype ((E))> { (E); }

سانچہ <غیرعلامتی Pri> ساخت hb_ترجیح : hb_ترجیح<Pri - 1> {};
سانچہ <>             ساخت hb_ترجیح<0> {};
#تعریف_کرو hb_prioritize hb_ترجیح<16> ()

#تعریف_کرو HB_FUNCOBJ(x) static_const x HB_غیر_مستعمل


سانچہ <قسم_نام T> ساخت hb_type_شناختی_عنصر_t { قسمتعریف T type; };
سانچہ <قسم_نام T> استعمال hb_type_شناختی_عنصر = قسم_نام hb_type_شناختی_عنصر_t<T>::type;

سانچہ <قسم_نام T> ساکن اندرونی T hb_declval ();
#تعریف_کرو hb_declval(T) (hb_declval<T> ())

سانچہ <قسم_نام T> ساخت hb_مطابقت_const		: hb_type_شناختی_عنصر_t<T>, hb_false_type	{};
سانچہ <قسم_نام T> ساخت hb_مطابقت_const<ثابت T>	: hb_type_شناختی_عنصر_t<T>, hb_true_type	{};
سانچہ <قسم_نام T> استعمال hb_remove_const = قسم_نام hb_مطابقت_const<T>::type;

سانچہ <قسم_نام T> ساخت hb_مطابقت_reference		: hb_type_شناختی_عنصر_t<T>, hb_false_type	{};
سانچہ <قسم_نام T> ساخت hb_مطابقت_reference<T &>	: hb_type_شناختی_عنصر_t<T>, hb_true_type	{};
سانچہ <قسم_نام T> ساخت hb_مطابقت_reference<T &&>	: hb_type_شناختی_عنصر_t<T>, hb_true_type	{};
سانچہ <قسم_نام T> استعمال hb_remove_reference = قسم_نام hb_مطابقت_reference<T>::type;
سانچہ <قسم_نام T> خودکار hb_try_جوڑو_lvalue_reference (hb_ترجیح<1>) -> hb_type_شناختی_عنصر<T&>;
سانچہ <قسم_نام T> خودکار hb_try_جوڑو_lvalue_reference (hb_ترجیح<0>) -> hb_type_شناختی_عنصر<T>;
سانچہ <قسم_نام T> استعمال hb_جوڑو_lvalue_reference = decltype (hb_try_جوڑو_lvalue_reference<T> (hb_prioritize));
سانچہ <قسم_نام T> خودکار hb_try_جوڑو_rvalue_reference (hb_ترجیح<1>) -> hb_type_شناختی_عنصر<T&&>;
سانچہ <قسم_نام T> خودکار hb_try_جوڑو_rvalue_reference (hb_ترجیح<0>) -> hb_type_شناختی_عنصر<T>;
سانچہ <قسم_نام T> استعمال hb_جوڑو_rvalue_reference = decltype (hb_try_جوڑو_rvalue_reference<T> (hb_prioritize));

سانچہ <قسم_نام T> ساخت hb_مطابقت_اشارہ_گر		: hb_type_شناختی_عنصر_t<T>, hb_false_type	{};
سانچہ <قسم_نام T> ساخت hb_مطابقت_اشارہ_گر<T *>	: hb_type_شناختی_عنصر_t<T>, hb_true_type	{};
سانچہ <قسم_نام T> استعمال hb_remove_اشارہ_گر = قسم_نام hb_مطابقت_اشارہ_گر<T>::type;
سانچہ <قسم_نام T> خودکار hb_try_جوڑو_اشارہ_گر (hb_ترجیح<1>) -> hb_type_شناختی_عنصر<hb_remove_reference<T>*>;
سانچہ <قسم_نام T> خودکار hb_try_جوڑو_اشارہ_گر (hb_ترجیح<1>) -> hb_type_شناختی_عنصر<T>;
سانچہ <قسم_نام T> استعمال hb_جوڑو_اشارہ_گر = decltype (hb_try_جوڑو_اشارہ_گر<T> (hb_prioritize));


سانچہ <قسم_نام T> استعمال hb_decay = قسم_نام std::decay<T>::type;

#تعریف_کرو hb_is_convertible(From,مقصد) std::is_convertible<From, مقصد>::value

سانچہ <قسم_نام From, قسم_نام مقصد>
استعمال hb_is_cr_convertible = hb_bool_constant<
  hb_is_same (hb_decay<From>, hb_decay<مقصد>) &&
  (!std::is_const<From>::value || std::is_const<مقصد>::value) &&
  (!std::is_reference<مقصد>::value || std::is_const<مقصد>::value || std::is_reference<مقصد>::value)
>;
#تعریف_کرو hb_is_cr_convertible(From,مقصد) hb_is_cr_convertible<From, مقصد>::value


ساخت
{
  سانچہ <قسم_نام T> constexpr خودکار
  عامل () (T&& v) ثابت HB_AUTO_RETURN (std::forward<T> (v))

  سانچہ <قسم_نام T> constexpr خودکار
  عامل () (T *v) ثابت HB_AUTO_RETURN (*v)

  سانچہ <قسم_نام T> constexpr خودکار
  عامل () (ثابت hb::shared_ptr<T>& v) ثابت HB_AUTO_RETURN (*v)

  سانچہ <قسم_نام T> constexpr خودکار
  عامل () (hb::shared_ptr<T>& v) ثابت HB_AUTO_RETURN (*v)
  
  سانچہ <قسم_نام T> constexpr خودکار
  عامل () (ثابت hb::unique_ptr<T>& v) ثابت HB_AUTO_RETURN (*v)

  سانچہ <قسم_نام T> constexpr خودکار
  عامل () (hb::unique_ptr<T>& v) ثابت HB_AUTO_RETURN (*v)
}
HB_FUNCOBJ (hb_deref);

سانچہ <قسم_نام T>
ساخت hb_reference_wrapper
{
  hb_reference_wrapper (T v) : v (v) {}
  منطقی عامل == (ثابت hb_reference_wrapper& o) ثابت { واپس v == o.v; }
  منطقی عامل != (ثابت hb_reference_wrapper& o) ثابت { واپس v != o.v; }
  عامل T& () { واپس v; }
  T& get () { واپس v; }
  T v;
};
سانچہ <قسم_نام T>
ساخت hb_reference_wrapper<T&>
{
  hb_reference_wrapper (T& v) : v (std::addressof (v)) {}
  منطقی عامل == (ثابت hb_reference_wrapper& o) ثابت { واپس v == o.v; }
  منطقی عامل != (ثابت hb_reference_wrapper& o) ثابت { واپس v != o.v; }
  عامل T& () { واپس *v; }
  T& get () { واپس *v; }
  T* v;
};


/* Type traits */

سانچہ <قسم_نام T> ساخت hb_int_min;
سانچہ <> ساخت hb_int_min<حرف>			: hb_integral_constant<حرف,			CHAR_MIN>	{};
سانچہ <> ساخت hb_int_min<علامتی حرف>		: hb_integral_constant<علامتی حرف,		SCHAR_MIN>	{};
سانچہ <> ساخت hb_int_min<غیرعلامتی حرف>		: hb_integral_constant<غیرعلامتی حرف,		0>		{};
سانچہ <> ساخت hb_int_min<علامتی مختصر>		: hb_integral_constant<علامتی مختصر,		SHRT_MIN>	{};
سانچہ <> ساخت hb_int_min<غیرعلامتی مختصر>		: hb_integral_constant<غیرعلامتی مختصر,		0>		{};
سانچہ <> ساخت hb_int_min<علامتی صحیح>		: hb_integral_constant<علامتی صحیح,		INT_MIN>	{};
سانچہ <> ساخت hb_int_min<غیرعلامتی صحیح>		: hb_integral_constant<غیرعلامتی صحیح,		0>		{};
سانچہ <> ساخت hb_int_min<علامتی طویل>		: hb_integral_constant<علامتی طویل,		LONG_MIN>	{};
سانچہ <> ساخت hb_int_min<غیرعلامتی طویل>		: hb_integral_constant<غیرعلامتی طویل,		0>		{};
سانچہ <> ساخت hb_int_min<علامتی طویل طویل>		: hb_integral_constant<علامتی طویل طویل,	LLONG_MIN>	{};
سانچہ <> ساخت hb_int_min<غیرعلامتی طویل طویل>	: hb_integral_constant<غیرعلامتی طویل طویل,	0>		{};
سانچہ <قسم_نام T> ساخت hb_int_min<T *>		: hb_integral_constant<T *,			خالی_اشاریہ>	{};
#تعریف_کرو hb_int_min(T) hb_int_min<T>::value
سانچہ <قسم_نام T> ساخت hb_int_max;
سانچہ <> ساخت hb_int_max<حرف>			: hb_integral_constant<حرف,			CHAR_MAX>	{};
سانچہ <> ساخت hb_int_max<علامتی حرف>		: hb_integral_constant<علامتی حرف,		SCHAR_MAX>	{};
سانچہ <> ساخت hb_int_max<غیرعلامتی حرف>		: hb_integral_constant<غیرعلامتی حرف,		UCHAR_MAX>	{};
سانچہ <> ساخت hb_int_max<علامتی مختصر>		: hb_integral_constant<علامتی مختصر,		SHRT_MAX>	{};
سانچہ <> ساخت hb_int_max<غیرعلامتی مختصر>		: hb_integral_constant<غیرعلامتی مختصر,		USHRT_MAX>	{};
سانچہ <> ساخت hb_int_max<علامتی صحیح>		: hb_integral_constant<علامتی صحیح,		زیادہ_سے_زیادہ_عدد>	{};
سانچہ <> ساخت hb_int_max<غیرعلامتی صحیح>		: hb_integral_constant<غیرعلامتی صحیح,		UINT_MAX>	{};
سانچہ <> ساخت hb_int_max<علامتی طویل>		: hb_integral_constant<علامتی طویل,		LONG_MAX>	{};
سانچہ <> ساخت hb_int_max<غیرعلامتی طویل>		: hb_integral_constant<غیرعلامتی طویل,		ULONG_MAX>	{};
سانچہ <> ساخت hb_int_max<علامتی طویل طویل>		: hb_integral_constant<علامتی طویل طویل,	LLONG_MAX>	{};
سانچہ <> ساخت hb_int_max<غیرعلامتی طویل طویل>	: hb_integral_constant<غیرعلامتی طویل طویل,	ULLONG_MAX>	{};
#تعریف_کرو hb_int_max(T) hb_int_max<T>::value

#اگر defined(__GNUC__) && __GNUC__ < 5 && !defined(__clang__)
#تعریف_کرو hb_is_trivially_copyable(T) __has_trivial_copy(T)
#تعریف_کرو hb_is_trivially_copy_assignable(T) __has_trivial_assign(T)
#تعریف_کرو hb_is_trivially_constructible(T) __has_trivial_constructor(T)
#تعریف_کرو hb_is_trivially_copy_constructible(T) __has_trivial_copy_constructor(T)
#تعریف_کرو hb_is_trivially_destructible(T) __has_trivial_destructor(T)
#ورنہ
#تعریف_کرو hb_is_trivially_copyable(T) std::is_trivially_copyable<T>::value
#تعریف_کرو hb_is_trivially_copy_assignable(T) std::is_trivially_copy_assignable<T>::value
#تعریف_کرو hb_is_trivially_constructible(T) std::is_trivially_constructible<T>::value
#تعریف_کرو hb_is_trivially_copy_constructible(T) std::is_trivially_copy_constructible<T>::value
#تعریف_کرو hb_is_trivially_destructible(T) std::is_trivially_destructible<T>::value
#اگر_ختم

/* Class traits. */

#تعریف_کرو HB_DELETE_COPY_تفویض(TypeName) \
  TypeName(ثابت TypeName&) = حذف; \
  خالی عامل=(ثابت TypeName&) = حذف
#تعریف_کرو HB_DELETE_CREATE_COPY_تفویض(TypeName) \
  TypeName() = حذف; \
  TypeName(ثابت TypeName&) = حذف; \
  خالی عامل=(ثابت TypeName&) = حذف

/* hb_unwrap_type (T)
 * If T has no T::type, returns T. Otherwise calls itself on T::type recursively.
 */

سانچہ <قسم_نام T, قسم_نام>
ساخت _hb_unwrap_type : hb_type_شناختی_عنصر_t<T> {};
سانچہ <قسم_نام T>
ساخت _hb_unwrap_type<T, hb_void_t<قسم_نام T::type>> : _hb_unwrap_type<قسم_نام T::type, خالی> {};
سانچہ <قسم_نام T>
استعمال hb_unwrap_type = _hb_unwrap_type<T, خالی>;
#تعریف_کرو hb_unwrap_type(T) قسم_نام hb_unwrap_type<T>::type

#اگر_ختم /* HB_META_HH */

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

#جے_بنا_تعریف HB_META_HH
#تعریف_کرو HB_META_HH

#ضمن "hb.hh"

#ضمن <memory>
#ضمن <type_traits>
#ضمن <utility>


/*
 * C++ template meta-programming & fundamentals used with them.
 */

/* Void!  For when we need a expression-type of void. */
بنية hb_خالی_تھاواں_t {};

/* https://en.cppreference.com/w/cpp/types/void_t */
سانچا<قسم_ناں... Ts> بنية _hb_void_t { تعريفنوع فراغ type; };
سانچا<قسم_ناں... Ts> ورتوں hb_void_t = قسم_ناں _hb_void_t<Ts...>::type;

سانچا<قسم_ناں سر, قسم_ناں... Ts> بنية hb_سر_t { تعريفنوع سر type; };
سانچا<قسم_ناں... Ts> ورتوں hb_سر_t_2 = قسم_ناں hb_سر_t<Ts...>::type;

سانچا <قسم_ناں T, T v> بنية hb_integral_constant { ساكن constexpr T value = v; };
سانچا <منطقي b> ورتوں hb_bool_constant = hb_integral_constant<منطقي, b>;
ورتوں hb_true_type = hb_bool_constant<سچ>;
ورتوں hb_false_type = hb_bool_constant<خطأ>;

/* Static-assert as expression. */
سانچا <منطقي cond> بنية static_assert_اظہاریہ;
سانچا <> بنية static_assert_اظہاریہ<سچ> : hb_false_type {};
#تعریف_کرو static_assert_اظہاریہ(C) static_assert_اظہاریہ<C>::value

/* Basic type SFINAE. */

سانچا <منطقي B, قسم_ناں T = فراغ> بنية hb_enable_if {};
سانچا <قسم_ناں T>                بنية hb_enable_if<سچ, T> { تعريفنوع T type; };
#تعریف_کرو hb_enable_if(Cond) قسم_ناں hb_enable_if<(Cond)>::type* = خالی_سوچک
/* Concepts/Requires alias: */
#تعریف_کرو hb_requires(Cond) hb_enable_if((Cond))

سانچا <قسم_ناں T, قسم_ناں T2> بنية hb_is_same : hb_false_type {};
سانچا <قسم_ناں T>              بنية hb_is_same<T, T> : hb_true_type {};
#تعریف_کرو hb_is_same(T, T2) hb_is_same<T, T2>::value

/* Function overloading SFINAE and priority. */

#تعریف_کرو HB_RETURN(Ret, E) -> hb_سر_t_2<Ret, decltype ((E))> { أرجع (E); }
#تعریف_کرو HB_AUTO_RETURN(E) -> decltype ((E)) { أرجع (E); }
#تعریف_کرو HB_VOID_RETURN(E) -> hb_void_t<decltype ((E))> { (E); }

سانچا <بنانشانی Pri> بنية hb_ترجیح : hb_ترجیح<Pri - 1> {};
سانچا <>             بنية hb_ترجیح<0> {};
#تعریف_کرو hb_prioritize hb_ترجیح<16> ()

#تعریف_کرو HB_FUNCOBJ(x) static_const x HB_بن_ورتیا


سانچا <قسم_ناں T> بنية hb_type_پچھان_تت_t { تعريفنوع T type; };
سانچا <قسم_ناں T> ورتوں hb_type_پچھان_تت = قسم_ناں hb_type_پچھان_تت_t<T>::type;

سانچا <قسم_ناں T> ساكن مضمن T hb_declval ();
#تعریف_کرو hb_declval(T) (hb_declval<T> ())

سانچا <قسم_ناں T> بنية hb_میل_const		: hb_type_پچھان_تت_t<T>, hb_false_type	{};
سانچا <قسم_ناں T> بنية hb_میل_const<ثابت T>	: hb_type_پچھان_تت_t<T>, hb_true_type	{};
سانچا <قسم_ناں T> ورتوں hb_remove_const = قسم_ناں hb_میل_const<T>::type;

سانچا <قسم_ناں T> بنية hb_میل_reference		: hb_type_پچھان_تت_t<T>, hb_false_type	{};
سانچا <قسم_ناں T> بنية hb_میل_reference<T &>	: hb_type_پچھان_تت_t<T>, hb_true_type	{};
سانچا <قسم_ناں T> بنية hb_میل_reference<T &&>	: hb_type_پچھان_تت_t<T>, hb_true_type	{};
سانچا <قسم_ناں T> ورتوں hb_remove_reference = قسم_ناں hb_میل_reference<T>::type;
سانچا <قسم_ناں T> خودکار hb_try_جوڑو_lvalue_reference (hb_ترجیح<1>) -> hb_type_پچھان_تت<T&>;
سانچا <قسم_ناں T> خودکار hb_try_جوڑو_lvalue_reference (hb_ترجیح<0>) -> hb_type_پچھان_تت<T>;
سانچا <قسم_ناں T> ورتوں hb_جوڑو_lvalue_reference = decltype (hb_try_جوڑو_lvalue_reference<T> (hb_prioritize));
سانچا <قسم_ناں T> خودکار hb_try_جوڑو_rvalue_reference (hb_ترجیح<1>) -> hb_type_پچھان_تت<T&&>;
سانچا <قسم_ناں T> خودکار hb_try_جوڑو_rvalue_reference (hb_ترجیح<0>) -> hb_type_پچھان_تت<T>;
سانچا <قسم_ناں T> ورتوں hb_جوڑو_rvalue_reference = decltype (hb_try_جوڑو_rvalue_reference<T> (hb_prioritize));

سانچا <قسم_ناں T> بنية hb_میل_اشارہ_گر		: hb_type_پچھان_تت_t<T>, hb_false_type	{};
سانچا <قسم_ناں T> بنية hb_میل_اشارہ_گر<T *>	: hb_type_پچھان_تت_t<T>, hb_true_type	{};
سانچا <قسم_ناں T> ورتوں hb_remove_اشارہ_گر = قسم_ناں hb_میل_اشارہ_گر<T>::type;
سانچا <قسم_ناں T> خودکار hb_try_جوڑو_اشارہ_گر (hb_ترجیح<1>) -> hb_type_پچھان_تت<hb_remove_reference<T>*>;
سانچا <قسم_ناں T> خودکار hb_try_جوڑو_اشارہ_گر (hb_ترجیح<1>) -> hb_type_پچھان_تت<T>;
سانچا <قسم_ناں T> ورتوں hb_جوڑو_اشارہ_گر = decltype (hb_try_جوڑو_اشارہ_گر<T> (hb_prioritize));


سانچا <قسم_ناں T> ورتوں hb_decay = قسم_ناں std::decay<T>::type;

#تعریف_کرو hb_is_convertible(From,منزل) std::is_convertible<From, منزل>::value

سانچا <قسم_ناں From, قسم_ناں منزل>
ورتوں hb_is_cr_convertible = hb_bool_constant<
  hb_is_same (hb_decay<From>, hb_decay<منزل>) &&
  (!std::is_const<From>::value || std::is_const<منزل>::value) &&
  (!std::is_reference<منزل>::value || std::is_const<منزل>::value || std::is_reference<منزل>::value)
>;
#تعریف_کرو hb_is_cr_convertible(From,منزل) hb_is_cr_convertible<From, منزل>::value


بنية
{
  سانچا <قسم_ناں T> constexpr خودکار
  عامل () (T&& v) ثابت HB_AUTO_RETURN (std::forward<T> (v))

  سانچا <قسم_ناں T> constexpr خودکار
  عامل () (T *v) ثابت HB_AUTO_RETURN (*v)

  سانچا <قسم_ناں T> constexpr خودکار
  عامل () (ثابت hb::shared_ptr<T>& v) ثابت HB_AUTO_RETURN (*v)

  سانچا <قسم_ناں T> constexpr خودکار
  عامل () (hb::shared_ptr<T>& v) ثابت HB_AUTO_RETURN (*v)
  
  سانچا <قسم_ناں T> constexpr خودکار
  عامل () (ثابت hb::unique_ptr<T>& v) ثابت HB_AUTO_RETURN (*v)

  سانچا <قسم_ناں T> constexpr خودکار
  عامل () (hb::unique_ptr<T>& v) ثابت HB_AUTO_RETURN (*v)
}
HB_FUNCOBJ (hb_deref);

سانچا <قسم_ناں T>
بنية hb_reference_wrapper
{
  hb_reference_wrapper (T v) : v (v) {}
  منطقي عامل == (ثابت hb_reference_wrapper& o) ثابت { أرجع v == o.v; }
  منطقي عامل != (ثابت hb_reference_wrapper& o) ثابت { أرجع v != o.v; }
  عامل T& () { أرجع v; }
  T& get () { أرجع v; }
  T v;
};
سانچا <قسم_ناں T>
بنية hb_reference_wrapper<T&>
{
  hb_reference_wrapper (T& v) : v (std::addressof (v)) {}
  منطقي عامل == (ثابت hb_reference_wrapper& o) ثابت { أرجع v == o.v; }
  منطقي عامل != (ثابت hb_reference_wrapper& o) ثابت { أرجع v != o.v; }
  عامل T& () { أرجع *v; }
  T& get () { أرجع *v; }
  T* v;
};


/* Type traits */

سانچا <قسم_ناں T> بنية hb_int_min;
سانچا <> بنية hb_int_min<حرف>			: hb_integral_constant<حرف,			CHAR_MIN>	{};
سانچا <> بنية hb_int_min<نشانیوالا حرف>		: hb_integral_constant<نشانیوالا حرف,		SCHAR_MIN>	{};
سانچا <> بنية hb_int_min<بنانشانی حرف>		: hb_integral_constant<بنانشانی حرف,		0>		{};
سانچا <> بنية hb_int_min<نشانیوالا چھوٹا>		: hb_integral_constant<نشانیوالا چھوٹا,		SHRT_MIN>	{};
سانچا <> بنية hb_int_min<بنانشانی چھوٹا>		: hb_integral_constant<بنانشانی چھوٹا,		0>		{};
سانچا <> بنية hb_int_min<نشانیوالا صحيح>		: hb_integral_constant<نشانیوالا صحيح,		INT_MIN>	{};
سانچا <> بنية hb_int_min<بنانشانی صحيح>		: hb_integral_constant<بنانشانی صحيح,		0>		{};
سانچا <> بنية hb_int_min<نشانیوالا لما>		: hb_integral_constant<نشانیوالا لما,		LONG_MIN>	{};
سانچا <> بنية hb_int_min<بنانشانی لما>		: hb_integral_constant<بنانشانی لما,		0>		{};
سانچا <> بنية hb_int_min<نشانیوالا لما لما>		: hb_integral_constant<نشانیوالا لما لما,	LLONG_MIN>	{};
سانچا <> بنية hb_int_min<بنانشانی لما لما>	: hb_integral_constant<بنانشانی لما لما,	0>		{};
سانچا <قسم_ناں T> بنية hb_int_min<T *>		: hb_integral_constant<T *,			خالی_سوچک>	{};
#تعریف_کرو hb_int_min(T) hb_int_min<T>::value
سانچا <قسم_ناں T> بنية hb_int_max;
سانچا <> بنية hb_int_max<حرف>			: hb_integral_constant<حرف,			CHAR_MAX>	{};
سانچا <> بنية hb_int_max<نشانیوالا حرف>		: hb_integral_constant<نشانیوالا حرف,		SCHAR_MAX>	{};
سانچا <> بنية hb_int_max<بنانشانی حرف>		: hb_integral_constant<بنانشانی حرف,		UCHAR_MAX>	{};
سانچا <> بنية hb_int_max<نشانیوالا چھوٹا>		: hb_integral_constant<نشانیوالا چھوٹا,		SHRT_MAX>	{};
سانچا <> بنية hb_int_max<بنانشانی چھوٹا>		: hb_integral_constant<بنانشانی چھوٹا,		USHRT_MAX>	{};
سانچا <> بنية hb_int_max<نشانیوالا صحيح>		: hb_integral_constant<نشانیوالا صحيح,		ودھ_توں_ودھ_عدد>	{};
سانچا <> بنية hb_int_max<بنانشانی صحيح>		: hb_integral_constant<بنانشانی صحيح,		UINT_MAX>	{};
سانچا <> بنية hb_int_max<نشانیوالا لما>		: hb_integral_constant<نشانیوالا لما,		LONG_MAX>	{};
سانچا <> بنية hb_int_max<بنانشانی لما>		: hb_integral_constant<بنانشانی لما,		ULONG_MAX>	{};
سانچا <> بنية hb_int_max<نشانیوالا لما لما>		: hb_integral_constant<نشانیوالا لما لما,	LLONG_MAX>	{};
سانچا <> بنية hb_int_max<بنانشانی لما لما>	: hb_integral_constant<بنانشانی لما لما,	ULLONG_MAX>	{};
#تعریف_کرو hb_int_max(T) hb_int_max<T>::value

#إذا defined(__GNUC__) && __GNUC__ < 5 && !defined(__clang__)
#تعریف_کرو hb_is_trivially_copyable(T) __has_trivial_copy(T)
#تعریف_کرو hb_is_trivially_copy_assignable(T) __has_trivial_assign(T)
#تعریف_کرو hb_is_trivially_constructible(T) __has_trivial_constructor(T)
#تعریف_کرو hb_is_trivially_copy_constructible(T) __has_trivial_copy_constructor(T)
#تعریف_کرو hb_is_trivially_destructible(T) __has_trivial_destructor(T)
#وإلا
#تعریف_کرو hb_is_trivially_copyable(T) std::is_trivially_copyable<T>::value
#تعریف_کرو hb_is_trivially_copy_assignable(T) std::is_trivially_copy_assignable<T>::value
#تعریف_کرو hb_is_trivially_constructible(T) std::is_trivially_constructible<T>::value
#تعریف_کرو hb_is_trivially_copy_constructible(T) std::is_trivially_copy_constructible<T>::value
#تعریف_کرو hb_is_trivially_destructible(T) std::is_trivially_destructible<T>::value
#جے_انت

/* Class traits. */

#تعریف_کرو HB_DELETE_COPY_سونپو(TypeName) \
  TypeName(ثابت TypeName&) = مٹاؤ; \
  فراغ عامل=(ثابت TypeName&) = مٹاؤ
#تعریف_کرو HB_DELETE_CREATE_COPY_سونپو(TypeName) \
  TypeName() = مٹاؤ; \
  TypeName(ثابت TypeName&) = مٹاؤ; \
  فراغ عامل=(ثابت TypeName&) = مٹاؤ

/* hb_unwrap_type (T)
 * If T has no T::type, returns T. Otherwise calls itself on T::type recursively.
 */

سانچا <قسم_ناں T, قسم_ناں>
بنية _hb_unwrap_type : hb_type_پچھان_تت_t<T> {};
سانچا <قسم_ناں T>
بنية _hb_unwrap_type<T, hb_void_t<قسم_ناں T::type>> : _hb_unwrap_type<قسم_ناں T::type, فراغ> {};
سانچا <قسم_ناں T>
ورتوں hb_unwrap_type = _hb_unwrap_type<T, فراغ>;
#تعریف_کرو hb_unwrap_type(T) قسم_ناں hb_unwrap_type<T>::type

#جے_انت /* HB_META_HH */

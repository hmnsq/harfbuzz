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

#جيڪڏهن_وصف_نه_ٿيل HB_META_HH
#وصف_ڪريو HB_META_HH

#ضمن "hb.hh"

#ضمن <memory>
#ضمن <type_traits>
#ضمن <utility>


/*
 * C++ template meta-programming & fundamentals used with them.
 */

/* Void!  For when we need a expression-type of void. */
بنية hb_خالي_جايون_t {};

/* https://en.cppreference.com/w/cpp/types/void_t */
سانچو<قسم_نالو... Ts> بنية _hb_void_t { تعريفنوع فراغ type; };
سانچو<قسم_نالو... Ts> استعمال hb_void_t = قسم_نالو _hb_void_t<Ts...>::type;

سانچو<قسم_نالو سر, قسم_نالو... Ts> بنية hb_سر_t { تعريفنوع سر type; };
سانچو<قسم_نالو... Ts> استعمال hb_سر_t_2 = قسم_نالو hb_سر_t<Ts...>::type;

سانچو <قسم_نالو T, T v> بنية hb_integral_constant { ساكن constexpr T value = v; };
سانچو <منطقي b> استعمال hb_bool_constant = hb_integral_constant<منطقي, b>;
استعمال hb_true_type = hb_bool_constant<سچ>;
استعمال hb_false_type = hb_bool_constant<خطأ>;

/* Static-assert as expression. */
سانچو <منطقي cond> بنية static_assert_اظهار;
سانچو <> بنية static_assert_اظهار<سچ> : hb_false_type {};
#وصف_ڪريو static_assert_اظهار(C) static_assert_اظهار<C>::value

/* Basic type SFINAE. */

سانچو <منطقي B, قسم_نالو T = فراغ> بنية hb_enable_if {};
سانچو <قسم_نالو T>                بنية hb_enable_if<سچ, T> { تعريفنوع T type; };
#وصف_ڪريو hb_enable_if(Cond) قسم_نالو hb_enable_if<(Cond)>::type* = خالي_اشارو
/* Concepts/Requires alias: */
#وصف_ڪريو hb_requires(Cond) hb_enable_if((Cond))

سانچو <قسم_نالو T, قسم_نالو T2> بنية hb_is_same : hb_false_type {};
سانچو <قسم_نالو T>              بنية hb_is_same<T, T> : hb_true_type {};
#وصف_ڪريو hb_is_same(T, T2) hb_is_same<T, T2>::value

/* Function overloading SFINAE and priority. */

#وصف_ڪريو HB_RETURN(Ret, E) -> hb_سر_t_2<Ret, decltype ((E))> { أرجع (E); }
#وصف_ڪريو HB_AUTO_RETURN(E) -> decltype ((E)) { أرجع (E); }
#وصف_ڪريو HB_VOID_RETURN(E) -> hb_void_t<decltype ((E))> { (E); }

سانچو <بنانشان Pri> بنية hb_ترجيح : hb_ترجيح<Pri - 1> {};
سانچو <>             بنية hb_ترجيح<0> {};
#وصف_ڪريو hb_prioritize hb_ترجيح<16> ()

#وصف_ڪريو HB_FUNCOBJ(x) static_const x HB_اڻ_استعمال_ٿيل


سانچو <قسم_نالو T> بنية hb_type_سڃاڻپ_عنصر_t { تعريفنوع T type; };
سانچو <قسم_نالو T> استعمال hb_type_سڃاڻپ_عنصر = قسم_نالو hb_type_سڃاڻپ_عنصر_t<T>::type;

سانچو <قسم_نالو T> ساكن مضمن T hb_declval ();
#وصف_ڪريو hb_declval(T) (hb_declval<T> ())

سانچو <قسم_نالو T> بنية hb_ميلاپ_const		: hb_type_سڃاڻپ_عنصر_t<T>, hb_false_type	{};
سانچو <قسم_نالو T> بنية hb_ميلاپ_const<ثابت T>	: hb_type_سڃاڻپ_عنصر_t<T>, hb_true_type	{};
سانچو <قسم_نالو T> استعمال hb_remove_const = قسم_نالو hb_ميلاپ_const<T>::type;

سانچو <قسم_نالو T> بنية hb_ميلاپ_reference		: hb_type_سڃاڻپ_عنصر_t<T>, hb_false_type	{};
سانچو <قسم_نالو T> بنية hb_ميلاپ_reference<T &>	: hb_type_سڃاڻپ_عنصر_t<T>, hb_true_type	{};
سانچو <قسم_نالو T> بنية hb_ميلاپ_reference<T &&>	: hb_type_سڃاڻپ_عنصر_t<T>, hb_true_type	{};
سانچو <قسم_نالو T> استعمال hb_remove_reference = قسم_نالو hb_ميلاپ_reference<T>::type;
سانچو <قسم_نالو T> خودڪار hb_try_جوڙيو_lvalue_reference (hb_ترجيح<1>) -> hb_type_سڃاڻپ_عنصر<T&>;
سانچو <قسم_نالو T> خودڪار hb_try_جوڙيو_lvalue_reference (hb_ترجيح<0>) -> hb_type_سڃاڻپ_عنصر<T>;
سانچو <قسم_نالو T> استعمال hb_جوڙيو_lvalue_reference = decltype (hb_try_جوڙيو_lvalue_reference<T> (hb_prioritize));
سانچو <قسم_نالو T> خودڪار hb_try_جوڙيو_rvalue_reference (hb_ترجيح<1>) -> hb_type_سڃاڻپ_عنصر<T&&>;
سانچو <قسم_نالو T> خودڪار hb_try_جوڙيو_rvalue_reference (hb_ترجيح<0>) -> hb_type_سڃاڻپ_عنصر<T>;
سانچو <قسم_نالو T> استعمال hb_جوڙيو_rvalue_reference = decltype (hb_try_جوڙيو_rvalue_reference<T> (hb_prioritize));

سانچو <قسم_نالو T> بنية hb_ميلاپ_اشارو		: hb_type_سڃاڻپ_عنصر_t<T>, hb_false_type	{};
سانچو <قسم_نالو T> بنية hb_ميلاپ_اشارو<T *>	: hb_type_سڃاڻپ_عنصر_t<T>, hb_true_type	{};
سانچو <قسم_نالو T> استعمال hb_remove_اشارو = قسم_نالو hb_ميلاپ_اشارو<T>::type;
سانچو <قسم_نالو T> خودڪار hb_try_جوڙيو_اشارو (hb_ترجيح<1>) -> hb_type_سڃاڻپ_عنصر<hb_remove_reference<T>*>;
سانچو <قسم_نالو T> خودڪار hb_try_جوڙيو_اشارو (hb_ترجيح<1>) -> hb_type_سڃاڻپ_عنصر<T>;
سانچو <قسم_نالو T> استعمال hb_جوڙيو_اشارو = decltype (hb_try_جوڙيو_اشارو<T> (hb_prioritize));


سانچو <قسم_نالو T> استعمال hb_decay = قسم_نالو std::decay<T>::type;

#وصف_ڪريو hb_is_convertible(From,منزل) std::is_convertible<From, منزل>::value

سانچو <قسم_نالو From, قسم_نالو منزل>
استعمال hb_is_cr_convertible = hb_bool_constant<
  hb_is_same (hb_decay<From>, hb_decay<منزل>) &&
  (!std::is_const<From>::value || std::is_const<منزل>::value) &&
  (!std::is_reference<منزل>::value || std::is_const<منزل>::value || std::is_reference<منزل>::value)
>;
#وصف_ڪريو hb_is_cr_convertible(From,منزل) hb_is_cr_convertible<From, منزل>::value


بنية
{
  سانچو <قسم_نالو T> constexpr خودڪار
  عامل () (T&& v) ثابت HB_AUTO_RETURN (std::forward<T> (v))

  سانچو <قسم_نالو T> constexpr خودڪار
  عامل () (T *v) ثابت HB_AUTO_RETURN (*v)

  سانچو <قسم_نالو T> constexpr خودڪار
  عامل () (ثابت hb::shared_ptr<T>& v) ثابت HB_AUTO_RETURN (*v)

  سانچو <قسم_نالو T> constexpr خودڪار
  عامل () (hb::shared_ptr<T>& v) ثابت HB_AUTO_RETURN (*v)
  
  سانچو <قسم_نالو T> constexpr خودڪار
  عامل () (ثابت hb::unique_ptr<T>& v) ثابت HB_AUTO_RETURN (*v)

  سانچو <قسم_نالو T> constexpr خودڪار
  عامل () (hb::unique_ptr<T>& v) ثابت HB_AUTO_RETURN (*v)
}
HB_FUNCOBJ (hb_deref);

سانچو <قسم_نالو T>
بنية hb_reference_wrapper
{
  hb_reference_wrapper (T v) : v (v) {}
  منطقي عامل == (ثابت hb_reference_wrapper& o) ثابت { أرجع v == o.v; }
  منطقي عامل != (ثابت hb_reference_wrapper& o) ثابت { أرجع v != o.v; }
  عامل T& () { أرجع v; }
  T& get () { أرجع v; }
  T v;
};
سانچو <قسم_نالو T>
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

سانچو <قسم_نالو T> بنية hb_int_min;
سانچو <> بنية hb_int_min<حرف>			: hb_integral_constant<حرف,			CHAR_MIN>	{};
سانچو <> بنية hb_int_min<نشانوارو حرف>		: hb_integral_constant<نشانوارو حرف,		SCHAR_MIN>	{};
سانچو <> بنية hb_int_min<بنانشان حرف>		: hb_integral_constant<بنانشان حرف,		0>		{};
سانچو <> بنية hb_int_min<نشانوارو ننڍو>		: hb_integral_constant<نشانوارو ننڍو,		SHRT_MIN>	{};
سانچو <> بنية hb_int_min<بنانشان ننڍو>		: hb_integral_constant<بنانشان ننڍو,		0>		{};
سانچو <> بنية hb_int_min<نشانوارو صحيح>		: hb_integral_constant<نشانوارو صحيح,		INT_MIN>	{};
سانچو <> بنية hb_int_min<بنانشان صحيح>		: hb_integral_constant<بنانشان صحيح,		0>		{};
سانچو <> بنية hb_int_min<نشانوارو ڊگهو>		: hb_integral_constant<نشانوارو ڊگهو,		LONG_MIN>	{};
سانچو <> بنية hb_int_min<بنانشان ڊگهو>		: hb_integral_constant<بنانشان ڊگهو,		0>		{};
سانچو <> بنية hb_int_min<نشانوارو ڊگهو ڊگهو>		: hb_integral_constant<نشانوارو ڊگهو ڊگهو,	LLONG_MIN>	{};
سانچو <> بنية hb_int_min<بنانشان ڊگهو ڊگهو>	: hb_integral_constant<بنانشان ڊگهو ڊگهو,	0>		{};
سانچو <قسم_نالو T> بنية hb_int_min<T *>		: hb_integral_constant<T *,			خالي_اشارو>	{};
#وصف_ڪريو hb_int_min(T) hb_int_min<T>::value
سانچو <قسم_نالو T> بنية hb_int_max;
سانچو <> بنية hb_int_max<حرف>			: hb_integral_constant<حرف,			CHAR_MAX>	{};
سانچو <> بنية hb_int_max<نشانوارو حرف>		: hb_integral_constant<نشانوارو حرف,		SCHAR_MAX>	{};
سانچو <> بنية hb_int_max<بنانشان حرف>		: hb_integral_constant<بنانشان حرف,		UCHAR_MAX>	{};
سانچو <> بنية hb_int_max<نشانوارو ننڍو>		: hb_integral_constant<نشانوارو ننڍو,		SHRT_MAX>	{};
سانچو <> بنية hb_int_max<بنانشان ننڍو>		: hb_integral_constant<بنانشان ننڍو,		USHRT_MAX>	{};
سانچو <> بنية hb_int_max<نشانوارو صحيح>		: hb_integral_constant<نشانوارو صحيح,		وڌ_کان_وڌ_عدد>	{};
سانچو <> بنية hb_int_max<بنانشان صحيح>		: hb_integral_constant<بنانشان صحيح,		UINT_MAX>	{};
سانچو <> بنية hb_int_max<نشانوارو ڊگهو>		: hb_integral_constant<نشانوارو ڊگهو,		LONG_MAX>	{};
سانچو <> بنية hb_int_max<بنانشان ڊگهو>		: hb_integral_constant<بنانشان ڊگهو,		ULONG_MAX>	{};
سانچو <> بنية hb_int_max<نشانوارو ڊگهو ڊگهو>		: hb_integral_constant<نشانوارو ڊگهو ڊگهو,	LLONG_MAX>	{};
سانچو <> بنية hb_int_max<بنانشان ڊگهو ڊگهو>	: hb_integral_constant<بنانشان ڊگهو ڊگهو,	ULLONG_MAX>	{};
#وصف_ڪريو hb_int_max(T) hb_int_max<T>::value

#إذا defined(__GNUC__) && __GNUC__ < 5 && !defined(__clang__)
#وصف_ڪريو hb_is_trivially_copyable(T) __has_trivial_copy(T)
#وصف_ڪريو hb_is_trivially_copy_assignable(T) __has_trivial_assign(T)
#وصف_ڪريو hb_is_trivially_constructible(T) __has_trivial_constructor(T)
#وصف_ڪريو hb_is_trivially_copy_constructible(T) __has_trivial_copy_constructor(T)
#وصف_ڪريو hb_is_trivially_destructible(T) __has_trivial_destructor(T)
#وإلا
#وصف_ڪريو hb_is_trivially_copyable(T) std::is_trivially_copyable<T>::value
#وصف_ڪريو hb_is_trivially_copy_assignable(T) std::is_trivially_copy_assignable<T>::value
#وصف_ڪريو hb_is_trivially_constructible(T) std::is_trivially_constructible<T>::value
#وصف_ڪريو hb_is_trivially_copy_constructible(T) std::is_trivially_copy_constructible<T>::value
#وصف_ڪريو hb_is_trivially_destructible(T) std::is_trivially_destructible<T>::value
#جيڪڏهن_پورو

/* Class traits. */

#وصف_ڪريو HB_DELETE_COPY_سونپيو(TypeName) \
  TypeName(ثابت TypeName&) = ميٽيو; \
  فراغ عامل=(ثابت TypeName&) = ميٽيو
#وصف_ڪريو HB_DELETE_CREATE_COPY_سونپيو(TypeName) \
  TypeName() = ميٽيو; \
  TypeName(ثابت TypeName&) = ميٽيو; \
  فراغ عامل=(ثابت TypeName&) = ميٽيو

/* hb_unwrap_type (T)
 * If T has no T::type, returns T. Otherwise calls itself on T::type recursively.
 */

سانچو <قسم_نالو T, قسم_نالو>
بنية _hb_unwrap_type : hb_type_سڃاڻپ_عنصر_t<T> {};
سانچو <قسم_نالو T>
بنية _hb_unwrap_type<T, hb_void_t<قسم_نالو T::type>> : _hb_unwrap_type<قسم_نالو T::type, فراغ> {};
سانچو <قسم_نالو T>
استعمال hb_unwrap_type = _hb_unwrap_type<T, فراغ>;
#وصف_ڪريو hb_unwrap_type(T) قسم_نالو hb_unwrap_type<T>::type

#جيڪڏهن_پورو /* HB_META_HH */

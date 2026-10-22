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

#که_نه_تعریف_شوی HB_META_HH
#تعریف_کړه HB_META_HH

#ضمن "hb.hh"

#ضمن <memory>
#ضمن <type_traits>
#ضمن <utility>


/*
 * C++ template meta-programming & fundamentals used with them.
 */

/* Void!  For when we need a expression-type of void. */
بنية hb_تشه_t {};

/* https://en.cppreference.com/w/cpp/types/void_t */
کينډۍ<ډول_نوم... Ts> بنية _hb_void_t { تعريفنوع فراغ type; };
کينډۍ<ډول_نوم... Ts> کارول hb_void_t = ډول_نوم _hb_void_t<Ts...>::type;

کينډۍ<ډول_نوم سر, ډول_نوم... Ts> بنية hb_سر_t { تعريفنوع سر type; };
کينډۍ<ډول_نوم... Ts> کارول hb_سر_t_2 = ډول_نوم hb_سر_t<Ts...>::type;

کينډۍ <ډول_نوم T, T v> بنية hb_integral_constant { ساكن constexpr T value = v; };
کينډۍ <منطقي b> کارول hb_bool_constant = hb_integral_constant<منطقي, b>;
کارول hb_true_type = hb_bool_constant<ریښتیا>;
کارول hb_false_type = hb_bool_constant<خطأ>;

/* Static-assert as expression. */
کينډۍ <منطقي cond> بنية static_assert_بیان;
کينډۍ <> بنية static_assert_بیان<ریښتیا> : hb_false_type {};
#تعریف_کړه static_assert_بیان(C) static_assert_بیان<C>::value

/* Basic type SFINAE. */

کينډۍ <منطقي B, ډول_نوم T = فراغ> بنية hb_enable_if {};
کينډۍ <ډول_نوم T>                بنية hb_enable_if<ریښتیا, T> { تعريفنوع T type; };
#تعریف_کړه hb_enable_if(Cond) ډول_نوم hb_enable_if<(Cond)>::type* = تش_اشاره
/* Concepts/Requires alias: */
#تعریف_کړه hb_requires(Cond) hb_enable_if((Cond))

کينډۍ <ډول_نوم T, ډول_نوم T2> بنية hb_is_same : hb_false_type {};
کينډۍ <ډول_نوم T>              بنية hb_is_same<T, T> : hb_true_type {};
#تعریف_کړه hb_is_same(T, T2) hb_is_same<T, T2>::value

/* Function overloading SFINAE and priority. */

#تعریف_کړه HB_RETURN(Ret, E) -> hb_سر_t_2<Ret, decltype ((E))> { أرجع (E); }
#تعریف_کړه HB_AUTO_RETURN(E) -> decltype ((E)) { أرجع (E); }
#تعریف_کړه HB_VOID_RETURN(E) -> hb_void_t<decltype ((E))> { (E); }

کينډۍ <بینښه Pri> بنية hb_لومړیتوب : hb_لومړیتوب<Pri - 1> {};
کينډۍ <>             بنية hb_لومړیتوب<0> {};
#تعریف_کړه hb_prioritize hb_لومړیتوب<16> ()

#تعریف_کړه HB_FUNCOBJ(x) static_const x HB_ناکارول_شوی


کينډۍ <ډول_نوم T> بنية hb_type_بې_اغېزه_عنصر_t { تعريفنوع T type; };
کينډۍ <ډول_نوم T> کارول hb_type_بې_اغېزه_عنصر = ډول_نوم hb_type_بې_اغېزه_عنصر_t<T>::type;

کينډۍ <ډول_نوم T> ساكن مضمن T hb_declval ();
#تعریف_کړه hb_declval(T) (hb_declval<T> ())

کينډۍ <ډول_نوم T> بنية hb_سمون_const		: hb_type_بې_اغېزه_عنصر_t<T>, hb_false_type	{};
کينډۍ <ډول_نوم T> بنية hb_سمون_const<ثابت T>	: hb_type_بې_اغېزه_عنصر_t<T>, hb_true_type	{};
کينډۍ <ډول_نوم T> کارول hb_remove_const = ډول_نوم hb_سمون_const<T>::type;

کينډۍ <ډول_نوم T> بنية hb_سمون_reference		: hb_type_بې_اغېزه_عنصر_t<T>, hb_false_type	{};
کينډۍ <ډول_نوم T> بنية hb_سمون_reference<T &>	: hb_type_بې_اغېزه_عنصر_t<T>, hb_true_type	{};
کينډۍ <ډول_نوم T> بنية hb_سمون_reference<T &&>	: hb_type_بې_اغېزه_عنصر_t<T>, hb_true_type	{};
کينډۍ <ډول_نوم T> کارول hb_remove_reference = ډول_نوم hb_سمون_reference<T>::type;
کينډۍ <ډول_نوم T> خپلکاری hb_try_جمع_کړه_lvalue_reference (hb_لومړیتوب<1>) -> hb_type_بې_اغېزه_عنصر<T&>;
کينډۍ <ډول_نوم T> خپلکاری hb_try_جمع_کړه_lvalue_reference (hb_لومړیتوب<0>) -> hb_type_بې_اغېزه_عنصر<T>;
کينډۍ <ډول_نوم T> کارول hb_جمع_کړه_lvalue_reference = decltype (hb_try_جمع_کړه_lvalue_reference<T> (hb_prioritize));
کينډۍ <ډول_نوم T> خپلکاری hb_try_جمع_کړه_rvalue_reference (hb_لومړیتوب<1>) -> hb_type_بې_اغېزه_عنصر<T&&>;
کينډۍ <ډول_نوم T> خپلکاری hb_try_جمع_کړه_rvalue_reference (hb_لومړیتوب<0>) -> hb_type_بې_اغېزه_عنصر<T>;
کينډۍ <ډول_نوم T> کارول hb_جمع_کړه_rvalue_reference = decltype (hb_try_جمع_کړه_rvalue_reference<T> (hb_prioritize));

کينډۍ <ډول_نوم T> بنية hb_سمون_اشاره_ګر		: hb_type_بې_اغېزه_عنصر_t<T>, hb_false_type	{};
کينډۍ <ډول_نوم T> بنية hb_سمون_اشاره_ګر<T *>	: hb_type_بې_اغېزه_عنصر_t<T>, hb_true_type	{};
کينډۍ <ډول_نوم T> کارول hb_remove_اشاره_ګر = ډول_نوم hb_سمون_اشاره_ګر<T>::type;
کينډۍ <ډول_نوم T> خپلکاری hb_try_جمع_کړه_اشاره_ګر (hb_لومړیتوب<1>) -> hb_type_بې_اغېزه_عنصر<hb_remove_reference<T>*>;
کينډۍ <ډول_نوم T> خپلکاری hb_try_جمع_کړه_اشاره_ګر (hb_لومړیتوب<1>) -> hb_type_بې_اغېزه_عنصر<T>;
کينډۍ <ډول_نوم T> کارول hb_جمع_کړه_اشاره_ګر = decltype (hb_try_جمع_کړه_اشاره_ګر<T> (hb_prioritize));


کينډۍ <ډول_نوم T> کارول hb_decay = ډول_نوم std::decay<T>::type;

#تعریف_کړه hb_is_convertible(From,منزل) std::is_convertible<From, منزل>::value

کينډۍ <ډول_نوم From, ډول_نوم منزل>
کارول hb_is_cr_convertible = hb_bool_constant<
  hb_is_same (hb_decay<From>, hb_decay<منزل>) &&
  (!std::is_const<From>::value || std::is_const<منزل>::value) &&
  (!std::is_reference<منزل>::value || std::is_const<منزل>::value || std::is_reference<منزل>::value)
>;
#تعریف_کړه hb_is_cr_convertible(From,منزل) hb_is_cr_convertible<From, منزل>::value


بنية
{
  کينډۍ <ډول_نوم T> constexpr خپلکاری
  چلوونکی () (T&& v) ثابت HB_AUTO_RETURN (std::forward<T> (v))

  کينډۍ <ډول_نوم T> constexpr خپلکاری
  چلوونکی () (T *v) ثابت HB_AUTO_RETURN (*v)

  کينډۍ <ډول_نوم T> constexpr خپلکاری
  چلوونکی () (ثابت hb::shared_ptr<T>& v) ثابت HB_AUTO_RETURN (*v)

  کينډۍ <ډول_نوم T> constexpr خپلکاری
  چلوونکی () (hb::shared_ptr<T>& v) ثابت HB_AUTO_RETURN (*v)
  
  کينډۍ <ډول_نوم T> constexpr خپلکاری
  چلوونکی () (ثابت hb::unique_ptr<T>& v) ثابت HB_AUTO_RETURN (*v)

  کينډۍ <ډول_نوم T> constexpr خپلکاری
  چلوونکی () (hb::unique_ptr<T>& v) ثابت HB_AUTO_RETURN (*v)
}
HB_FUNCOBJ (hb_deref);

کينډۍ <ډول_نوم T>
بنية hb_reference_wrapper
{
  hb_reference_wrapper (T v) : v (v) {}
  منطقي چلوونکی == (ثابت hb_reference_wrapper& o) ثابت { أرجع v == o.v; }
  منطقي چلوونکی != (ثابت hb_reference_wrapper& o) ثابت { أرجع v != o.v; }
  چلوونکی T& () { أرجع v; }
  T& get () { أرجع v; }
  T v;
};
کينډۍ <ډول_نوم T>
بنية hb_reference_wrapper<T&>
{
  hb_reference_wrapper (T& v) : v (std::addressof (v)) {}
  منطقي چلوونکی == (ثابت hb_reference_wrapper& o) ثابت { أرجع v == o.v; }
  منطقي چلوونکی != (ثابت hb_reference_wrapper& o) ثابت { أرجع v != o.v; }
  چلوونکی T& () { أرجع *v; }
  T& get () { أرجع *v; }
  T* v;
};


/* Type traits */

کينډۍ <ډول_نوم T> بنية hb_int_min;
کينډۍ <> بنية hb_int_min<حرف>			: hb_integral_constant<حرف,			CHAR_MIN>	{};
کينډۍ <> بنية hb_int_min<نښهدار حرف>		: hb_integral_constant<نښهدار حرف,		SCHAR_MIN>	{};
کينډۍ <> بنية hb_int_min<بینښه حرف>		: hb_integral_constant<بینښه حرف,		0>		{};
کينډۍ <> بنية hb_int_min<نښهدار لنډ>		: hb_integral_constant<نښهدار لنډ,		SHRT_MIN>	{};
کينډۍ <> بنية hb_int_min<بینښه لنډ>		: hb_integral_constant<بینښه لنډ,		0>		{};
کينډۍ <> بنية hb_int_min<نښهدار صحيح>		: hb_integral_constant<نښهدار صحيح,		INT_MIN>	{};
کينډۍ <> بنية hb_int_min<بینښه صحيح>		: hb_integral_constant<بینښه صحيح,		0>		{};
کينډۍ <> بنية hb_int_min<نښهدار اوږد>		: hb_integral_constant<نښهدار اوږد,		LONG_MIN>	{};
کينډۍ <> بنية hb_int_min<بینښه اوږد>		: hb_integral_constant<بینښه اوږد,		0>		{};
کينډۍ <> بنية hb_int_min<نښهدار اوږد اوږد>		: hb_integral_constant<نښهدار اوږد اوږد,	LLONG_MIN>	{};
کينډۍ <> بنية hb_int_min<بینښه اوږد اوږد>	: hb_integral_constant<بینښه اوږد اوږد,	0>		{};
کينډۍ <ډول_نوم T> بنية hb_int_min<T *>		: hb_integral_constant<T *,			تش_اشاره>	{};
#تعریف_کړه hb_int_min(T) hb_int_min<T>::value
کينډۍ <ډول_نوم T> بنية hb_int_max;
کينډۍ <> بنية hb_int_max<حرف>			: hb_integral_constant<حرف,			CHAR_MAX>	{};
کينډۍ <> بنية hb_int_max<نښهدار حرف>		: hb_integral_constant<نښهدار حرف,		SCHAR_MAX>	{};
کينډۍ <> بنية hb_int_max<بینښه حرف>		: hb_integral_constant<بینښه حرف,		UCHAR_MAX>	{};
کينډۍ <> بنية hb_int_max<نښهدار لنډ>		: hb_integral_constant<نښهدار لنډ,		SHRT_MAX>	{};
کينډۍ <> بنية hb_int_max<بینښه لنډ>		: hb_integral_constant<بینښه لنډ,		USHRT_MAX>	{};
کينډۍ <> بنية hb_int_max<نښهدار صحيح>		: hb_integral_constant<نښهدار صحيح,		لوی_عدد>	{};
کينډۍ <> بنية hb_int_max<بینښه صحيح>		: hb_integral_constant<بینښه صحيح,		UINT_MAX>	{};
کينډۍ <> بنية hb_int_max<نښهدار اوږد>		: hb_integral_constant<نښهدار اوږد,		LONG_MAX>	{};
کينډۍ <> بنية hb_int_max<بینښه اوږد>		: hb_integral_constant<بینښه اوږد,		ULONG_MAX>	{};
کينډۍ <> بنية hb_int_max<نښهدار اوږد اوږد>		: hb_integral_constant<نښهدار اوږد اوږد,	LLONG_MAX>	{};
کينډۍ <> بنية hb_int_max<بینښه اوږد اوږد>	: hb_integral_constant<بینښه اوږد اوږد,	ULLONG_MAX>	{};
#تعریف_کړه hb_int_max(T) hb_int_max<T>::value

#إذا defined(__GNUC__) && __GNUC__ < 5 && !defined(__clang__)
#تعریف_کړه hb_is_trivially_copyable(T) __has_trivial_copy(T)
#تعریف_کړه hb_is_trivially_copy_assignable(T) __has_trivial_assign(T)
#تعریف_کړه hb_is_trivially_constructible(T) __has_trivial_constructor(T)
#تعریف_کړه hb_is_trivially_copy_constructible(T) __has_trivial_copy_constructor(T)
#تعریف_کړه hb_is_trivially_destructible(T) __has_trivial_destructor(T)
#وإلا
#تعریف_کړه hb_is_trivially_copyable(T) std::is_trivially_copyable<T>::value
#تعریف_کړه hb_is_trivially_copy_assignable(T) std::is_trivially_copy_assignable<T>::value
#تعریف_کړه hb_is_trivially_constructible(T) std::is_trivially_constructible<T>::value
#تعریف_کړه hb_is_trivially_copy_constructible(T) std::is_trivially_copy_constructible<T>::value
#تعریف_کړه hb_is_trivially_destructible(T) std::is_trivially_destructible<T>::value
#د_که_پای

/* Class traits. */

#تعریف_کړه HB_DELETE_COPY_ټاکل(TypeName) \
  TypeName(ثابت TypeName&) = ړنګول; \
  فراغ چلوونکی=(ثابت TypeName&) = ړنګول
#تعریف_کړه HB_DELETE_CREATE_COPY_ټاکل(TypeName) \
  TypeName() = ړنګول; \
  TypeName(ثابت TypeName&) = ړنګول; \
  فراغ چلوونکی=(ثابت TypeName&) = ړنګول

/* hb_unwrap_type (T)
 * If T has no T::type, returns T. Otherwise calls itself on T::type recursively.
 */

کينډۍ <ډول_نوم T, ډول_نوم>
بنية _hb_unwrap_type : hb_type_بې_اغېزه_عنصر_t<T> {};
کينډۍ <ډول_نوم T>
بنية _hb_unwrap_type<T, hb_void_t<ډول_نوم T::type>> : _hb_unwrap_type<ډول_نوم T::type, فراغ> {};
کينډۍ <ډول_نوم T>
کارول hb_unwrap_type = _hb_unwrap_type<T, فراغ>;
#تعریف_کړه hb_unwrap_type(T) ډول_نوم hb_unwrap_type<T>::type

#د_که_پای /* HB_META_HH */

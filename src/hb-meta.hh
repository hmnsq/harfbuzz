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

#اگر_تعریف_نشده HB_META_HH
#تعریف_کن HB_META_HH

#ضمن "hb.hh"

#ضمن <memory>
#ضمن <type_traits>
#ضمن <utility>


/*
 * C++ template meta-programming & fundamentals used with them.
 */

/* Void!  For when we need a expression-type of void. */
بنية hb_خالی_t {};

/* https://en.cppreference.com/w/cpp/types/void_t */
قالب<نام_نوع... Ts> بنية _hb_void_t { تعريفنوع فراغ type; };
قالب<نام_نوع... Ts> استفاده hb_void_t = نام_نوع _hb_void_t<Ts...>::type;

قالب<نام_نوع سر, نام_نوع... Ts> بنية hb_سر_t { تعريفنوع سر type; };
قالب<نام_نوع... Ts> استفاده hb_سر_t_2 = نام_نوع hb_سر_t<Ts...>::type;

قالب <نام_نوع T, T v> بنية hb_integral_constant { ساكن constexpr T value = v; };
قالب <منطقي b> استفاده hb_bool_constant = hb_integral_constant<منطقي, b>;
استفاده hb_true_type = hb_bool_constant<درست>;
استفاده hb_false_type = hb_bool_constant<خطأ>;

/* Static-assert as expression. */
قالب <منطقي cond> بنية static_assert_عبارت;
قالب <> بنية static_assert_عبارت<درست> : hb_false_type {};
#تعریف_کن static_assert_عبارت(C) static_assert_عبارت<C>::value

/* Basic type SFINAE. */

قالب <منطقي B, نام_نوع T = فراغ> بنية hb_enable_if {};
قالب <نام_نوع T>                بنية hb_enable_if<درست, T> { تعريفنوع T type; };
#تعریف_کن hb_enable_if(Cond) نام_نوع hb_enable_if<(Cond)>::type* = اشاره_تهی
/* Concepts/Requires alias: */
#تعریف_کن hb_requires(Cond) hb_enable_if((Cond))

قالب <نام_نوع T, نام_نوع T2> بنية hb_is_same : hb_false_type {};
قالب <نام_نوع T>              بنية hb_is_same<T, T> : hb_true_type {};
#تعریف_کن hb_is_same(T, T2) hb_is_same<T, T2>::value

/* Function overloading SFINAE and priority. */

#تعریف_کن HB_RETURN(Ret, E) -> hb_سر_t_2<Ret, decltype ((E))> { أرجع (E); }
#تعریف_کن HB_AUTO_RETURN(E) -> decltype ((E)) { أرجع (E); }
#تعریف_کن HB_VOID_RETURN(E) -> hb_void_t<decltype ((E))> { (E); }

قالب <بیعلامت Pri> بنية hb_اولویت : hb_اولویت<Pri - 1> {};
قالب <>             بنية hb_اولویت<0> {};
#تعریف_کن hb_prioritize hb_اولویت<16> ()

#تعریف_کن HB_FUNCOBJ(x) static_const x HB_استفادهنشده


قالب <نام_نوع T> بنية hb_type_عنصر_خنثی_t { تعريفنوع T type; };
قالب <نام_نوع T> استفاده hb_type_عنصر_خنثی = نام_نوع hb_type_عنصر_خنثی_t<T>::type;

قالب <نام_نوع T> ساكن مضمن T hb_declval ();
#تعریف_کن hb_declval(T) (hb_declval<T> ())

قالب <نام_نوع T> بنية hb_تطابق_const		: hb_type_عنصر_خنثی_t<T>, hb_false_type	{};
قالب <نام_نوع T> بنية hb_تطابق_const<ثابت T>	: hb_type_عنصر_خنثی_t<T>, hb_true_type	{};
قالب <نام_نوع T> استفاده hb_remove_const = نام_نوع hb_تطابق_const<T>::type;

قالب <نام_نوع T> بنية hb_تطابق_reference		: hb_type_عنصر_خنثی_t<T>, hb_false_type	{};
قالب <نام_نوع T> بنية hb_تطابق_reference<T &>	: hb_type_عنصر_خنثی_t<T>, hb_true_type	{};
قالب <نام_نوع T> بنية hb_تطابق_reference<T &&>	: hb_type_عنصر_خنثی_t<T>, hb_true_type	{};
قالب <نام_نوع T> استفاده hb_remove_reference = نام_نوع hb_تطابق_reference<T>::type;
قالب <نام_نوع T> خودکار hb_try_جمع_کن_lvalue_reference (hb_اولویت<1>) -> hb_type_عنصر_خنثی<T&>;
قالب <نام_نوع T> خودکار hb_try_جمع_کن_lvalue_reference (hb_اولویت<0>) -> hb_type_عنصر_خنثی<T>;
قالب <نام_نوع T> استفاده hb_جمع_کن_lvalue_reference = decltype (hb_try_جمع_کن_lvalue_reference<T> (hb_prioritize));
قالب <نام_نوع T> خودکار hb_try_جمع_کن_rvalue_reference (hb_اولویت<1>) -> hb_type_عنصر_خنثی<T&&>;
قالب <نام_نوع T> خودکار hb_try_جمع_کن_rvalue_reference (hb_اولویت<0>) -> hb_type_عنصر_خنثی<T>;
قالب <نام_نوع T> استفاده hb_جمع_کن_rvalue_reference = decltype (hb_try_جمع_کن_rvalue_reference<T> (hb_prioritize));

قالب <نام_نوع T> بنية hb_تطابق_اشاره_گر		: hb_type_عنصر_خنثی_t<T>, hb_false_type	{};
قالب <نام_نوع T> بنية hb_تطابق_اشاره_گر<T *>	: hb_type_عنصر_خنثی_t<T>, hb_true_type	{};
قالب <نام_نوع T> استفاده hb_remove_اشاره_گر = نام_نوع hb_تطابق_اشاره_گر<T>::type;
قالب <نام_نوع T> خودکار hb_try_جمع_کن_اشاره_گر (hb_اولویت<1>) -> hb_type_عنصر_خنثی<hb_remove_reference<T>*>;
قالب <نام_نوع T> خودکار hb_try_جمع_کن_اشاره_گر (hb_اولویت<1>) -> hb_type_عنصر_خنثی<T>;
قالب <نام_نوع T> استفاده hb_جمع_کن_اشاره_گر = decltype (hb_try_جمع_کن_اشاره_گر<T> (hb_prioritize));


قالب <نام_نوع T> استفاده hb_decay = نام_نوع std::decay<T>::type;

#تعریف_کن hb_is_convertible(From,مقصد) std::is_convertible<From, مقصد>::value

قالب <نام_نوع From, نام_نوع مقصد>
استفاده hb_is_cr_convertible = hb_bool_constant<
  hb_is_same (hb_decay<From>, hb_decay<مقصد>) &&
  (!std::is_const<From>::value || std::is_const<مقصد>::value) &&
  (!std::is_reference<مقصد>::value || std::is_const<مقصد>::value || std::is_reference<مقصد>::value)
>;
#تعریف_کن hb_is_cr_convertible(From,مقصد) hb_is_cr_convertible<From, مقصد>::value


بنية
{
  قالب <نام_نوع T> constexpr خودکار
  عملگر () (T&& v) ثابت HB_AUTO_RETURN (std::forward<T> (v))

  قالب <نام_نوع T> constexpr خودکار
  عملگر () (T *v) ثابت HB_AUTO_RETURN (*v)

  قالب <نام_نوع T> constexpr خودکار
  عملگر () (ثابت hb::shared_ptr<T>& v) ثابت HB_AUTO_RETURN (*v)

  قالب <نام_نوع T> constexpr خودکار
  عملگر () (hb::shared_ptr<T>& v) ثابت HB_AUTO_RETURN (*v)
  
  قالب <نام_نوع T> constexpr خودکار
  عملگر () (ثابت hb::unique_ptr<T>& v) ثابت HB_AUTO_RETURN (*v)

  قالب <نام_نوع T> constexpr خودکار
  عملگر () (hb::unique_ptr<T>& v) ثابت HB_AUTO_RETURN (*v)
}
HB_FUNCOBJ (hb_deref);

قالب <نام_نوع T>
بنية hb_reference_wrapper
{
  hb_reference_wrapper (T v) : v (v) {}
  منطقي عملگر == (ثابت hb_reference_wrapper& o) ثابت { أرجع v == o.v; }
  منطقي عملگر != (ثابت hb_reference_wrapper& o) ثابت { أرجع v != o.v; }
  عملگر T& () { أرجع v; }
  T& get () { أرجع v; }
  T v;
};
قالب <نام_نوع T>
بنية hb_reference_wrapper<T&>
{
  hb_reference_wrapper (T& v) : v (std::addressof (v)) {}
  منطقي عملگر == (ثابت hb_reference_wrapper& o) ثابت { أرجع v == o.v; }
  منطقي عملگر != (ثابت hb_reference_wrapper& o) ثابت { أرجع v != o.v; }
  عملگر T& () { أرجع *v; }
  T& get () { أرجع *v; }
  T* v;
};


/* Type traits */

قالب <نام_نوع T> بنية hb_int_min;
قالب <> بنية hb_int_min<حرف>			: hb_integral_constant<حرف,			CHAR_MIN>	{};
قالب <> بنية hb_int_min<علامتدار حرف>		: hb_integral_constant<علامتدار حرف,		SCHAR_MIN>	{};
قالب <> بنية hb_int_min<بیعلامت حرف>		: hb_integral_constant<بیعلامت حرف,		0>		{};
قالب <> بنية hb_int_min<علامتدار کوتاه>		: hb_integral_constant<علامتدار کوتاه,		SHRT_MIN>	{};
قالب <> بنية hb_int_min<بیعلامت کوتاه>		: hb_integral_constant<بیعلامت کوتاه,		0>		{};
قالب <> بنية hb_int_min<علامتدار صحيح>		: hb_integral_constant<علامتدار صحيح,		INT_MIN>	{};
قالب <> بنية hb_int_min<بیعلامت صحيح>		: hb_integral_constant<بیعلامت صحيح,		0>		{};
قالب <> بنية hb_int_min<علامتدار دراز>		: hb_integral_constant<علامتدار دراز,		LONG_MIN>	{};
قالب <> بنية hb_int_min<بیعلامت دراز>		: hb_integral_constant<بیعلامت دراز,		0>		{};
قالب <> بنية hb_int_min<علامتدار دراز دراز>		: hb_integral_constant<علامتدار دراز دراز,	LLONG_MIN>	{};
قالب <> بنية hb_int_min<بیعلامت دراز دراز>	: hb_integral_constant<بیعلامت دراز دراز,	0>		{};
قالب <نام_نوع T> بنية hb_int_min<T *>		: hb_integral_constant<T *,			اشاره_تهی>	{};
#تعریف_کن hb_int_min(T) hb_int_min<T>::value
قالب <نام_نوع T> بنية hb_int_max;
قالب <> بنية hb_int_max<حرف>			: hb_integral_constant<حرف,			CHAR_MAX>	{};
قالب <> بنية hb_int_max<علامتدار حرف>		: hb_integral_constant<علامتدار حرف,		SCHAR_MAX>	{};
قالب <> بنية hb_int_max<بیعلامت حرف>		: hb_integral_constant<بیعلامت حرف,		UCHAR_MAX>	{};
قالب <> بنية hb_int_max<علامتدار کوتاه>		: hb_integral_constant<علامتدار کوتاه,		SHRT_MAX>	{};
قالب <> بنية hb_int_max<بیعلامت کوتاه>		: hb_integral_constant<بیعلامت کوتاه,		USHRT_MAX>	{};
قالب <> بنية hb_int_max<علامتدار صحيح>		: hb_integral_constant<علامتدار صحيح,		بیشترین_صحیح>	{};
قالب <> بنية hb_int_max<بیعلامت صحيح>		: hb_integral_constant<بیعلامت صحيح,		UINT_MAX>	{};
قالب <> بنية hb_int_max<علامتدار دراز>		: hb_integral_constant<علامتدار دراز,		LONG_MAX>	{};
قالب <> بنية hb_int_max<بیعلامت دراز>		: hb_integral_constant<بیعلامت دراز,		ULONG_MAX>	{};
قالب <> بنية hb_int_max<علامتدار دراز دراز>		: hb_integral_constant<علامتدار دراز دراز,	LLONG_MAX>	{};
قالب <> بنية hb_int_max<بیعلامت دراز دراز>	: hb_integral_constant<بیعلامت دراز دراز,	ULLONG_MAX>	{};
#تعریف_کن hb_int_max(T) hb_int_max<T>::value

#إذا defined(__GNUC__) && __GNUC__ < 5 && !defined(__clang__)
#تعریف_کن hb_is_trivially_copyable(T) __has_trivial_copy(T)
#تعریف_کن hb_is_trivially_copy_assignable(T) __has_trivial_assign(T)
#تعریف_کن hb_is_trivially_constructible(T) __has_trivial_constructor(T)
#تعریف_کن hb_is_trivially_copy_constructible(T) __has_trivial_copy_constructor(T)
#تعریف_کن hb_is_trivially_destructible(T) __has_trivial_destructor(T)
#وإلا
#تعریف_کن hb_is_trivially_copyable(T) std::is_trivially_copyable<T>::value
#تعریف_کن hb_is_trivially_copy_assignable(T) std::is_trivially_copy_assignable<T>::value
#تعریف_کن hb_is_trivially_constructible(T) std::is_trivially_constructible<T>::value
#تعریف_کن hb_is_trivially_copy_constructible(T) std::is_trivially_copy_constructible<T>::value
#تعریف_کن hb_is_trivially_destructible(T) std::is_trivially_destructible<T>::value
#پایان_اگر

/* Class traits. */

#تعریف_کن HB_DELETE_COPY_تخصیص_بده(TypeName) \
  TypeName(ثابت TypeName&) = حذف; \
  فراغ عملگر=(ثابت TypeName&) = حذف
#تعریف_کن HB_DELETE_CREATE_COPY_تخصیص_بده(TypeName) \
  TypeName() = حذف; \
  TypeName(ثابت TypeName&) = حذف; \
  فراغ عملگر=(ثابت TypeName&) = حذف

/* hb_unwrap_type (T)
 * If T has no T::type, returns T. Otherwise calls itself on T::type recursively.
 */

قالب <نام_نوع T, نام_نوع>
بنية _hb_unwrap_type : hb_type_عنصر_خنثی_t<T> {};
قالب <نام_نوع T>
بنية _hb_unwrap_type<T, hb_void_t<نام_نوع T::type>> : _hb_unwrap_type<نام_نوع T::type, فراغ> {};
قالب <نام_نوع T>
استفاده hb_unwrap_type = _hb_unwrap_type<T, فراغ>;
#تعریف_کن hb_unwrap_type(T) نام_نوع hb_unwrap_type<T>::type

#پایان_اگر /* HB_META_HH */

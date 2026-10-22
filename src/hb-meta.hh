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

#अगर_अघोषित HB_META_HH
#घोषणा HB_META_HH

#समावेश "hb.hh"

#समावेश <memory>
#समावेश <type_traits>
#समावेश <utility>


/*
 * C++ template meta-programming & fundamentals used with them.
 */

/* Void!  For when we need a expression-type of void. */
काष्ठा hb_empty_t {};

/* https://en.cppreference.com/w/cpp/types/void_t */
ढाँचा<typename... Ts> काष्ठा _hb_void_t { प्रकार व्योम type; };
ढाँचा<typename... Ts> using hb_void_t = typename _hb_void_t<Ts...>::type;

ढाँचा<typename शीर्ष, typename... Ts> काष्ठा hb_शीर्ष_t { प्रकार शीर्ष type; };
ढाँचा<typename... Ts> using hb_शीर्ष_t_2 = typename hb_शीर्ष_t<Ts...>::type;

ढाँचा <typename T, T v> काष्ठा hb_integral_constant { अटल constexpr T value = v; };
ढाँचा <bool b> using hb_bool_constant = hb_integral_constant<bool, b>;
using hb_true_type = hb_bool_constant<true>;
using hb_false_type = hb_bool_constant<false>;

/* Static-assert as expression. */
ढाँचा <bool cond> काष्ठा static_assert_व्यंजक;
ढाँचा <> काष्ठा static_assert_व्यंजक<true> : hb_false_type {};
#घोषणा static_assert_व्यंजक(C) static_assert_व्यंजक<C>::value

/* Basic type SFINAE. */

ढाँचा <bool B, typename T = व्योम> काष्ठा hb_enable_if {};
ढाँचा <typename T>                काष्ठा hb_enable_if<true, T> { प्रकार T type; };
#घोषणा hb_enable_if(Cond) typename hb_enable_if<(Cond)>::type* = nullptr
/* Concepts/Requires alias: */
#घोषणा hb_requires(Cond) hb_enable_if((Cond))

ढाँचा <typename T, typename T2> काष्ठा hb_is_same : hb_false_type {};
ढाँचा <typename T>              काष्ठा hb_is_same<T, T> : hb_true_type {};
#घोषणा hb_is_same(T, T2) hb_is_same<T, T2>::value

/* Function overloading SFINAE and priority. */

#घोषणा HB_RETURN(Ret, E) -> hb_शीर्ष_t_2<Ret, decltype ((E))> { वापस (E); }
#घोषणा HB_AUTO_RETURN(E) -> decltype ((E)) { वापस (E); }
#घोषणा HB_VOID_RETURN(E) -> hb_void_t<decltype ((E))> { (E); }

ढाँचा <अचिन्हित Pri> काष्ठा hb_priority : hb_priority<Pri - 1> {};
ढाँचा <>             काष्ठा hb_priority<0> {};
#घोषणा hb_prioritize hb_priority<16> ()

#घोषणा HB_FUNCOBJ(x) static_const x HB_UNUSED


ढाँचा <typename T> काष्ठा hb_type_identity_t { प्रकार T type; };
ढाँचा <typename T> using hb_type_identity = typename hb_type_identity_t<T>::type;

ढाँचा <typename T> अटल अंतरभूत T hb_declval ();
#घोषणा hb_declval(T) (hb_declval<T> ())

ढाँचा <typename T> काष्ठा hb_match_const		: hb_type_identity_t<T>, hb_false_type	{};
ढाँचा <typename T> काष्ठा hb_match_const<स्थिर T>	: hb_type_identity_t<T>, hb_true_type	{};
ढाँचा <typename T> using hb_remove_const = typename hb_match_const<T>::type;

ढाँचा <typename T> काष्ठा hb_match_reference		: hb_type_identity_t<T>, hb_false_type	{};
ढाँचा <typename T> काष्ठा hb_match_reference<T &>	: hb_type_identity_t<T>, hb_true_type	{};
ढाँचा <typename T> काष्ठा hb_match_reference<T &&>	: hb_type_identity_t<T>, hb_true_type	{};
ढाँचा <typename T> using hb_remove_reference = typename hb_match_reference<T>::type;
ढाँचा <typename T> स्वतः _hb_try_add_lvalue_reference (hb_priority<1>) -> hb_type_identity<T&>;
ढाँचा <typename T> स्वतः _hb_try_add_lvalue_reference (hb_priority<0>) -> hb_type_identity<T>;
ढाँचा <typename T> using hb_add_lvalue_reference = decltype (_hb_try_add_lvalue_reference<T> (hb_prioritize));
ढाँचा <typename T> स्वतः _hb_try_add_rvalue_reference (hb_priority<1>) -> hb_type_identity<T&&>;
ढाँचा <typename T> स्वतः _hb_try_add_rvalue_reference (hb_priority<0>) -> hb_type_identity<T>;
ढाँचा <typename T> using hb_add_rvalue_reference = decltype (_hb_try_add_rvalue_reference<T> (hb_prioritize));

ढाँचा <typename T> काष्ठा hb_match_pointer		: hb_type_identity_t<T>, hb_false_type	{};
ढाँचा <typename T> काष्ठा hb_match_pointer<T *>	: hb_type_identity_t<T>, hb_true_type	{};
ढाँचा <typename T> using hb_remove_pointer = typename hb_match_pointer<T>::type;
ढाँचा <typename T> स्वतः _hb_try_add_pointer (hb_priority<1>) -> hb_type_identity<hb_remove_reference<T>*>;
ढाँचा <typename T> स्वतः _hb_try_add_pointer (hb_priority<1>) -> hb_type_identity<T>;
ढाँचा <typename T> using hb_add_pointer = decltype (_hb_try_add_pointer<T> (hb_prioritize));


ढाँचा <typename T> using hb_decay = typename std::decay<T>::type;

#घोषणा hb_is_convertible(From,To) std::is_convertible<From, To>::value

ढाँचा <typename From, typename To>
using hb_is_cr_convertible = hb_bool_constant<
  hb_is_same (hb_decay<From>, hb_decay<To>) &&
  (!std::is_const<From>::value || std::is_const<To>::value) &&
  (!std::is_reference<To>::value || std::is_const<To>::value || std::is_reference<To>::value)
>;
#घोषणा hb_is_cr_convertible(From,To) hb_is_cr_convertible<From, To>::value


काष्ठा
{
  ढाँचा <typename T> constexpr स्वतः
  चालक () (T&& v) स्थिर HB_AUTO_RETURN (std::forward<T> (v))

  ढाँचा <typename T> constexpr स्वतः
  चालक () (T *v) स्थिर HB_AUTO_RETURN (*v)

  ढाँचा <typename T> constexpr स्वतः
  चालक () (स्थिर hb::shared_ptr<T>& v) स्थिर HB_AUTO_RETURN (*v)

  ढाँचा <typename T> constexpr स्वतः
  चालक () (hb::shared_ptr<T>& v) स्थिर HB_AUTO_RETURN (*v)
  
  ढाँचा <typename T> constexpr स्वतः
  चालक () (स्थिर hb::unique_ptr<T>& v) स्थिर HB_AUTO_RETURN (*v)

  ढाँचा <typename T> constexpr स्वतः
  चालक () (hb::unique_ptr<T>& v) स्थिर HB_AUTO_RETURN (*v)
}
HB_FUNCOBJ (hb_deref);

ढाँचा <typename T>
काष्ठा hb_reference_wrapper
{
  hb_reference_wrapper (T v) : v (v) {}
  bool चालक == (स्थिर hb_reference_wrapper& o) स्थिर { वापस v == o.v; }
  bool चालक != (स्थिर hb_reference_wrapper& o) स्थिर { वापस v != o.v; }
  चालक T& () { वापस v; }
  T& लो () { वापस v; }
  T v;
};
ढाँचा <typename T>
काष्ठा hb_reference_wrapper<T&>
{
  hb_reference_wrapper (T& v) : v (std::addressof (v)) {}
  bool चालक == (स्थिर hb_reference_wrapper& o) स्थिर { वापस v == o.v; }
  bool चालक != (स्थिर hb_reference_wrapper& o) स्थिर { वापस v != o.v; }
  चालक T& () { वापस *v; }
  T& लो () { वापस *v; }
  T* v;
};


/* Type traits */

ढाँचा <typename T> काष्ठा hb_int_min;
ढाँचा <> काष्ठा hb_int_min<अक्षर>			: hb_integral_constant<अक्षर,			अक्षर_न्यून>	{};
ढाँचा <> काष्ठा hb_int_min<चिन्हित अक्षर>		: hb_integral_constant<चिन्हित अक्षर,		च_अक्षर_न्यून>	{};
ढाँचा <> काष्ठा hb_int_min<अचिन्हित अक्षर>		: hb_integral_constant<अचिन्हित अक्षर,		0>		{};
ढाँचा <> काष्ठा hb_int_min<चिन्हित लघु>		: hb_integral_constant<चिन्हित लघु,		लघु_न्यून>	{};
ढाँचा <> काष्ठा hb_int_min<अचिन्हित लघु>		: hb_integral_constant<अचिन्हित लघु,		0>		{};
ढाँचा <> काष्ठा hb_int_min<चिन्हित पूर्णांक>		: hb_integral_constant<चिन्हित पूर्णांक,		पूर्णांक_न्यून>	{};
ढाँचा <> काष्ठा hb_int_min<अचिन्हित पूर्णांक>		: hb_integral_constant<अचिन्हित पूर्णांक,		0>		{};
ढाँचा <> काष्ठा hb_int_min<चिन्हित दीर्घ>		: hb_integral_constant<चिन्हित दीर्घ,		दीर्घ_न्यून>	{};
ढाँचा <> काष्ठा hb_int_min<अचिन्हित दीर्घ>		: hb_integral_constant<अचिन्हित दीर्घ,		0>		{};
ढाँचा <> काष्ठा hb_int_min<चिन्हित दीर्घ दीर्घ>		: hb_integral_constant<चिन्हित दीर्घ दीर्घ,	LLONG_MIN>	{};
ढाँचा <> काष्ठा hb_int_min<अचिन्हित दीर्घ दीर्घ>	: hb_integral_constant<अचिन्हित दीर्घ दीर्घ,	0>		{};
ढाँचा <typename T> काष्ठा hb_int_min<T *>		: hb_integral_constant<T *,			nullptr>	{};
#घोषणा hb_int_min(T) hb_int_min<T>::value
ढाँचा <typename T> काष्ठा hb_int_max;
ढाँचा <> काष्ठा hb_int_max<अक्षर>			: hb_integral_constant<अक्षर,			अक्षर_उच्च>	{};
ढाँचा <> काष्ठा hb_int_max<चिन्हित अक्षर>		: hb_integral_constant<चिन्हित अक्षर,		च_अक्षर_उच्च>	{};
ढाँचा <> काष्ठा hb_int_max<अचिन्हित अक्षर>		: hb_integral_constant<अचिन्हित अक्षर,		अच_अक्षर_उच्च>	{};
ढाँचा <> काष्ठा hb_int_max<चिन्हित लघु>		: hb_integral_constant<चिन्हित लघु,		लघु_उच्च>	{};
ढाँचा <> काष्ठा hb_int_max<अचिन्हित लघु>		: hb_integral_constant<अचिन्हित लघु,		अच_लघु_उच्च>	{};
ढाँचा <> काष्ठा hb_int_max<चिन्हित पूर्णांक>		: hb_integral_constant<चिन्हित पूर्णांक,		पूर्णांक_उच्च>	{};
ढाँचा <> काष्ठा hb_int_max<अचिन्हित पूर्णांक>		: hb_integral_constant<अचिन्हित पूर्णांक,		अच_पूर्णांक_उच्च>	{};
ढाँचा <> काष्ठा hb_int_max<चिन्हित दीर्घ>		: hb_integral_constant<चिन्हित दीर्घ,		दीर्घ_उच्च>	{};
ढाँचा <> काष्ठा hb_int_max<अचिन्हित दीर्घ>		: hb_integral_constant<अचिन्हित दीर्घ,		अच_दीर्घ_उच्च>	{};
ढाँचा <> काष्ठा hb_int_max<चिन्हित दीर्घ दीर्घ>		: hb_integral_constant<चिन्हित दीर्घ दीर्घ,	LLONG_MAX>	{};
ढाँचा <> काष्ठा hb_int_max<अचिन्हित दीर्घ दीर्घ>	: hb_integral_constant<अचिन्हित दीर्घ दीर्घ,	ULLONG_MAX>	{};
#घोषणा hb_int_max(T) hb_int_max<T>::value

#अगर defined(__GNUC__) && __GNUC__ < 5 && !defined(__clang__)
#घोषणा hb_is_trivially_copyable(T) __has_trivial_copy(T)
#घोषणा hb_is_trivially_copy_assignable(T) __has_trivial_assign(T)
#घोषणा hb_is_trivially_constructible(T) __has_trivial_constructor(T)
#घोषणा hb_is_trivially_copy_constructible(T) __has_trivial_copy_constructor(T)
#घोषणा hb_is_trivially_destructible(T) __has_trivial_destructor(T)
#अन्यथा
#घोषणा hb_is_trivially_copyable(T) std::is_trivially_copyable<T>::value
#घोषणा hb_is_trivially_copy_assignable(T) std::is_trivially_copy_assignable<T>::value
#घोषणा hb_is_trivially_constructible(T) std::is_trivially_constructible<T>::value
#घोषणा hb_is_trivially_copy_constructible(T) std::is_trivially_copy_constructible<T>::value
#घोषणा hb_is_trivially_destructible(T) std::is_trivially_destructible<T>::value
#पूर्ण_अगर

/* Class traits. */

#घोषणा HB_DELETE_COPY_ASSIGN(TypeName) \
  TypeName(स्थिर TypeName&) = मिटाओ; \
  व्योम चालक=(स्थिर TypeName&) = मिटाओ
#घोषणा HB_DELETE_CREATE_COPY_ASSIGN(TypeName) \
  TypeName() = मिटाओ; \
  TypeName(स्थिर TypeName&) = मिटाओ; \
  व्योम चालक=(स्थिर TypeName&) = मिटाओ

/* hb_unwrap_type (T)
 * If T has no T::type, returns T. Otherwise calls itself on T::type recursively.
 */

ढाँचा <typename T, typename>
काष्ठा _hb_unwrap_type : hb_type_identity_t<T> {};
ढाँचा <typename T>
काष्ठा _hb_unwrap_type<T, hb_void_t<typename T::type>> : _hb_unwrap_type<typename T::type, व्योम> {};
ढाँचा <typename T>
using hb_unwrap_type = _hb_unwrap_type<T, व्योम>;
#घोषणा hb_unwrap_type(T) typename hb_unwrap_type<T>::type

#पूर्ण_अगर /* HB_META_HH */

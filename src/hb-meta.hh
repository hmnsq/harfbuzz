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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_META_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_META_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ <memory>
#ਅਨ੍ਤਰ੍ਭਾਵ <type_traits>
#ਅਨ੍ਤਰ੍ਭਾਵ <utility>


/*
 * C++ template meta-programming & fundamentals used with them.
 */

/* Void!  For when we need a expression-type of void. */
ਬਣਤਰ hb_ਖਾਲੀ_ਥਾਵਾਂ_t {};

/* https://en.cppreference.com/w/cpp/types/void_t */
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ... Ts> ਬਣਤਰ _hb_void_t { ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ type; };
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ... Ts> ਵਰਤੋਂ hb_void_t = ਕਿਸਮ_ਨਾਂ _hb_void_t<Ts...>::type;

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ ਸਿਰ, ਕਿਸਮ_ਨਾਂ... Ts> ਬਣਤਰ hb_ਸਿਰ_t { ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਸਿਰ type; };
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ... Ts> ਵਰਤੋਂ hb_ਸਿਰ_t_2 = ਕਿਸਮ_ਨਾਂ hb_ਸਿਰ_t<Ts...>::type;

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, T v> ਬਣਤਰ hb_integral_constant { ਸਥਿਰ constexpr T value = v; };
ਸਾਂਚਾ <ਤਾਰਕਿਕ b> ਵਰਤੋਂ hb_bool_constant = hb_integral_constant<ਤਾਰਕਿਕ, b>;
ਵਰਤੋਂ hb_true_type = hb_bool_constant<ਸੱਚ>;
ਵਰਤੋਂ hb_false_type = hb_bool_constant<ਝੂਠ>;

/* Static-assert as expression. */
ਸਾਂਚਾ <ਤਾਰਕਿਕ cond> ਬਣਤਰ static_assert_ਵਿਅੰਜਕ;
ਸਾਂਚਾ <> ਬਣਤਰ static_assert_ਵਿਅੰਜਕ<ਸੱਚ> : hb_false_type {};
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ static_assert_ਵਿਅੰਜਕ(C) static_assert_ਵਿਅੰਜਕ<C>::value

/* Basic type SFINAE. */

ਸਾਂਚਾ <ਤਾਰਕਿਕ B, ਕਿਸਮ_ਨਾਂ T = ਖਾਲੀ> ਬਣਤਰ hb_enable_if {};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>                ਬਣਤਰ hb_enable_if<ਸੱਚ, T> { ਕਿਸਮਪਰਿਭਾਸ਼ਾ T type; };
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_enable_if(Cond) ਕਿਸਮ_ਨਾਂ hb_enable_if<(Cond)>::type* = ਖਾਲੀ_ਸੂਚਕ
/* Concepts/Requires alias: */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_requires(Cond) hb_enable_if((Cond))

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ T2> ਬਣਤਰ hb_is_same : hb_false_type {};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>              ਬਣਤਰ hb_is_same<T, T> : hb_true_type {};
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_is_same(T, T2) hb_is_same<T, T2>::value

/* Function overloading SFINAE and priority. */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_RETURN(Ret, E) -> hb_ਸਿਰ_t_2<Ret, decltype ((E))> { ਵਾਪਸ (E); }
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_AUTO_RETURN(E) -> decltype ((E)) { ਵਾਪਸ (E); }
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_VOID_RETURN(E) -> hb_void_t<decltype ((E))> { (E); }

ਸਾਂਚਾ <ਬਿਨਾਨਿਸ਼ਾਨੀ Pri> ਬਣਤਰ hb_ਤਰਜੀਹ : hb_ਤਰਜੀਹ<Pri - 1> {};
ਸਾਂਚਾ <>             ਬਣਤਰ hb_ਤਰਜੀਹ<0> {};
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_prioritize hb_ਤਰਜੀਹ<16> ()

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_FUNCOBJ(x) static_const x HB_ਅਣਵਰਤਿਆ


ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਬਣਤਰ hb_type_ਪਛਾਣ_ਤੱਤ_t { ਕਿਸਮਪਰਿਭਾਸ਼ਾ T type; };
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਵਰਤੋਂ hb_type_ਪਛਾਣ_ਤੱਤ = ਕਿਸਮ_ਨਾਂ hb_type_ਪਛਾਣ_ਤੱਤ_t<T>::type;

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਸਥਿਰ ਸਤਰਅੰਦਰ T hb_declval ();
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_declval(T) (hb_declval<T> ())

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਬਣਤਰ hb_ਮੇਲ_const		: hb_type_ਪਛਾਣ_ਤੱਤ_t<T>, hb_false_type	{};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਬਣਤਰ hb_ਮੇਲ_const<ਅਚਲ T>	: hb_type_ਪਛਾਣ_ਤੱਤ_t<T>, hb_true_type	{};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਵਰਤੋਂ hb_remove_const = ਕਿਸਮ_ਨਾਂ hb_ਮੇਲ_const<T>::type;

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਬਣਤਰ hb_ਮੇਲ_reference		: hb_type_ਪਛਾਣ_ਤੱਤ_t<T>, hb_false_type	{};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਬਣਤਰ hb_ਮੇਲ_reference<T &>	: hb_type_ਪਛਾਣ_ਤੱਤ_t<T>, hb_true_type	{};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਬਣਤਰ hb_ਮੇਲ_reference<T &&>	: hb_type_ਪਛਾਣ_ਤੱਤ_t<T>, hb_true_type	{};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਵਰਤੋਂ hb_remove_reference = ਕਿਸਮ_ਨਾਂ hb_ਮੇਲ_reference<T>::type;
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਸਵੈਚਾਲਿਤ hb_try_ਜੋੜੋ_lvalue_reference (hb_ਤਰਜੀਹ<1>) -> hb_type_ਪਛਾਣ_ਤੱਤ<T&>;
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਸਵੈਚਾਲਿਤ hb_try_ਜੋੜੋ_lvalue_reference (hb_ਤਰਜੀਹ<0>) -> hb_type_ਪਛਾਣ_ਤੱਤ<T>;
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਵਰਤੋਂ hb_ਜੋੜੋ_lvalue_reference = decltype (hb_try_ਜੋੜੋ_lvalue_reference<T> (hb_prioritize));
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਸਵੈਚਾਲਿਤ hb_try_ਜੋੜੋ_rvalue_reference (hb_ਤਰਜੀਹ<1>) -> hb_type_ਪਛਾਣ_ਤੱਤ<T&&>;
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਸਵੈਚਾਲਿਤ hb_try_ਜੋੜੋ_rvalue_reference (hb_ਤਰਜੀਹ<0>) -> hb_type_ਪਛਾਣ_ਤੱਤ<T>;
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਵਰਤੋਂ hb_ਜੋੜੋ_rvalue_reference = decltype (hb_try_ਜੋੜੋ_rvalue_reference<T> (hb_prioritize));

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਬਣਤਰ hb_ਮੇਲ_ਸੂਚਕ		: hb_type_ਪਛਾਣ_ਤੱਤ_t<T>, hb_false_type	{};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਬਣਤਰ hb_ਮੇਲ_ਸੂਚਕ<T *>	: hb_type_ਪਛਾਣ_ਤੱਤ_t<T>, hb_true_type	{};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਵਰਤੋਂ hb_remove_ਸੂਚਕ = ਕਿਸਮ_ਨਾਂ hb_ਮੇਲ_ਸੂਚਕ<T>::type;
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਸਵੈਚਾਲਿਤ hb_try_ਜੋੜੋ_ਸੂਚਕ (hb_ਤਰਜੀਹ<1>) -> hb_type_ਪਛਾਣ_ਤੱਤ<hb_remove_reference<T>*>;
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਸਵੈਚਾਲਿਤ hb_try_ਜੋੜੋ_ਸੂਚਕ (hb_ਤਰਜੀਹ<1>) -> hb_type_ਪਛਾਣ_ਤੱਤ<T>;
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਵਰਤੋਂ hb_ਜੋੜੋ_ਸੂਚਕ = decltype (hb_try_ਜੋੜੋ_ਸੂਚਕ<T> (hb_prioritize));


ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਵਰਤੋਂ hb_decay = ਕਿਸਮ_ਨਾਂ std::decay<T>::type;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_is_convertible(From,ਮੰਜ਼ਿਲ) std::is_convertible<From, ਮੰਜ਼ਿਲ>::value

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ From, ਕਿਸਮ_ਨਾਂ ਮੰਜ਼ਿਲ>
ਵਰਤੋਂ hb_is_cr_convertible = hb_bool_constant<
  hb_is_same (hb_decay<From>, hb_decay<ਮੰਜ਼ਿਲ>) &&
  (!std::is_const<From>::value || std::is_const<ਮੰਜ਼ਿਲ>::value) &&
  (!std::is_reference<ਮੰਜ਼ਿਲ>::value || std::is_const<ਮੰਜ਼ਿਲ>::value || std::is_reference<ਮੰਜ਼ਿਲ>::value)
>;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_is_cr_convertible(From,ਮੰਜ਼ਿਲ) hb_is_cr_convertible<From, ਮੰਜ਼ਿਲ>::value


ਬਣਤਰ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> constexpr ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (T&& v) ਅਚਲ HB_AUTO_RETURN (std::forward<T> (v))

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> constexpr ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (T *v) ਅਚਲ HB_AUTO_RETURN (*v)

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> constexpr ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (ਅਚਲ hb::shared_ptr<T>& v) ਅਚਲ HB_AUTO_RETURN (*v)

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> constexpr ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (hb::shared_ptr<T>& v) ਅਚਲ HB_AUTO_RETURN (*v)
  
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> constexpr ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (ਅਚਲ hb::unique_ptr<T>& v) ਅਚਲ HB_AUTO_RETURN (*v)

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> constexpr ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (hb::unique_ptr<T>& v) ਅਚਲ HB_AUTO_RETURN (*v)
}
HB_FUNCOBJ (hb_deref);

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ hb_reference_wrapper
{
  hb_reference_wrapper (T v) : v (v) {}
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ hb_reference_wrapper& o) ਅਚਲ { ਵਾਪਸ v == o.v; }
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ hb_reference_wrapper& o) ਅਚਲ { ਵਾਪਸ v != o.v; }
  ਸੰਚਾਲਕ T& () { ਵਾਪਸ v; }
  T& get () { ਵਾਪਸ v; }
  T v;
};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ hb_reference_wrapper<T&>
{
  hb_reference_wrapper (T& v) : v (std::addressof (v)) {}
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ hb_reference_wrapper& o) ਅਚਲ { ਵਾਪਸ v == o.v; }
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ hb_reference_wrapper& o) ਅਚਲ { ਵਾਪਸ v != o.v; }
  ਸੰਚਾਲਕ T& () { ਵਾਪਸ *v; }
  T& get () { ਵਾਪਸ *v; }
  T* v;
};


/* Type traits */

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਬਣਤਰ hb_int_min;
ਸਾਂਚਾ <> ਬਣਤਰ hb_int_min<ਅੱਖਰ>			: hb_integral_constant<ਅੱਖਰ,			CHAR_MIN>	{};
ਸਾਂਚਾ <> ਬਣਤਰ hb_int_min<ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ>		: hb_integral_constant<ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ,		SCHAR_MIN>	{};
ਸਾਂਚਾ <> ਬਣਤਰ hb_int_min<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>		: hb_integral_constant<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ,		0>		{};
ਸਾਂਚਾ <> ਬਣਤਰ hb_int_min<ਨਿਸ਼ਾਨੀਵਾਲਾ ਛੋਟਾ>		: hb_integral_constant<ਨਿਸ਼ਾਨੀਵਾਲਾ ਛੋਟਾ,		SHRT_MIN>	{};
ਸਾਂਚਾ <> ਬਣਤਰ hb_int_min<ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ>		: hb_integral_constant<ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ,		0>		{};
ਸਾਂਚਾ <> ਬਣਤਰ hb_int_min<ਨਿਸ਼ਾਨੀਵਾਲਾ ਪੂਰਨ>		: hb_integral_constant<ਨਿਸ਼ਾਨੀਵਾਲਾ ਪੂਰਨ,		INT_MIN>	{};
ਸਾਂਚਾ <> ਬਣਤਰ hb_int_min<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ>		: hb_integral_constant<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ,		0>		{};
ਸਾਂਚਾ <> ਬਣਤਰ hb_int_min<ਨਿਸ਼ਾਨੀਵਾਲਾ ਲੰਮਾ>		: hb_integral_constant<ਨਿਸ਼ਾਨੀਵਾਲਾ ਲੰਮਾ,		LONG_MIN>	{};
ਸਾਂਚਾ <> ਬਣਤਰ hb_int_min<ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ>		: hb_integral_constant<ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ,		0>		{};
ਸਾਂਚਾ <> ਬਣਤਰ hb_int_min<ਨਿਸ਼ਾਨੀਵਾਲਾ ਲੰਮਾ ਲੰਮਾ>		: hb_integral_constant<ਨਿਸ਼ਾਨੀਵਾਲਾ ਲੰਮਾ ਲੰਮਾ,	LLONG_MIN>	{};
ਸਾਂਚਾ <> ਬਣਤਰ hb_int_min<ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ>	: hb_integral_constant<ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ,	0>		{};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਬਣਤਰ hb_int_min<T *>		: hb_integral_constant<T *,			ਖਾਲੀ_ਸੂਚਕ>	{};
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_int_min(T) hb_int_min<T>::value
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਬਣਤਰ hb_int_max;
ਸਾਂਚਾ <> ਬਣਤਰ hb_int_max<ਅੱਖਰ>			: hb_integral_constant<ਅੱਖਰ,			CHAR_MAX>	{};
ਸਾਂਚਾ <> ਬਣਤਰ hb_int_max<ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ>		: hb_integral_constant<ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ,		SCHAR_MAX>	{};
ਸਾਂਚਾ <> ਬਣਤਰ hb_int_max<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>		: hb_integral_constant<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ,		UCHAR_MAX>	{};
ਸਾਂਚਾ <> ਬਣਤਰ hb_int_max<ਨਿਸ਼ਾਨੀਵਾਲਾ ਛੋਟਾ>		: hb_integral_constant<ਨਿਸ਼ਾਨੀਵਾਲਾ ਛੋਟਾ,		SHRT_MAX>	{};
ਸਾਂਚਾ <> ਬਣਤਰ hb_int_max<ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ>		: hb_integral_constant<ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ,		USHRT_MAX>	{};
ਸਾਂਚਾ <> ਬਣਤਰ hb_int_max<ਨਿਸ਼ਾਨੀਵਾਲਾ ਪੂਰਨ>		: hb_integral_constant<ਨਿਸ਼ਾਨੀਵਾਲਾ ਪੂਰਨ,		ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ>	{};
ਸਾਂਚਾ <> ਬਣਤਰ hb_int_max<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ>		: hb_integral_constant<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ,		UINT_MAX>	{};
ਸਾਂਚਾ <> ਬਣਤਰ hb_int_max<ਨਿਸ਼ਾਨੀਵਾਲਾ ਲੰਮਾ>		: hb_integral_constant<ਨਿਸ਼ਾਨੀਵਾਲਾ ਲੰਮਾ,		LONG_MAX>	{};
ਸਾਂਚਾ <> ਬਣਤਰ hb_int_max<ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ>		: hb_integral_constant<ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ,		ULONG_MAX>	{};
ਸਾਂਚਾ <> ਬਣਤਰ hb_int_max<ਨਿਸ਼ਾਨੀਵਾਲਾ ਲੰਮਾ ਲੰਮਾ>		: hb_integral_constant<ਨਿਸ਼ਾਨੀਵਾਲਾ ਲੰਮਾ ਲੰਮਾ,	LLONG_MAX>	{};
ਸਾਂਚਾ <> ਬਣਤਰ hb_int_max<ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ>	: hb_integral_constant<ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ,	ULLONG_MAX>	{};
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_int_max(T) hb_int_max<T>::value

#ਜੇ defined(__GNUC__) && __GNUC__ < 5 && !defined(__clang__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_is_trivially_copyable(T) __has_trivial_copy(T)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_is_trivially_copy_assignable(T) __has_trivial_assign(T)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_is_trivially_constructible(T) __has_trivial_constructor(T)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_is_trivially_copy_constructible(T) __has_trivial_copy_constructor(T)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_is_trivially_destructible(T) __has_trivial_destructor(T)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_is_trivially_copyable(T) std::is_trivially_copyable<T>::value
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_is_trivially_copy_assignable(T) std::is_trivially_copy_assignable<T>::value
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_is_trivially_constructible(T) std::is_trivially_constructible<T>::value
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_is_trivially_copy_constructible(T) std::is_trivially_copy_constructible<T>::value
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_is_trivially_destructible(T) std::is_trivially_destructible<T>::value
#ਜੇ_ਅੰਤ

/* Class traits. */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DELETE_COPY_ਸੌਂਪੋ(TypeName) \
  TypeName(ਅਚਲ TypeName&) = ਮਿਟਾਓ; \
  ਖਾਲੀ ਸੰਚਾਲਕ=(ਅਚਲ TypeName&) = ਮਿਟਾਓ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DELETE_CREATE_COPY_ਸੌਂਪੋ(TypeName) \
  TypeName() = ਮਿਟਾਓ; \
  TypeName(ਅਚਲ TypeName&) = ਮਿਟਾਓ; \
  ਖਾਲੀ ਸੰਚਾਲਕ=(ਅਚਲ TypeName&) = ਮਿਟਾਓ

/* hb_unwrap_type (T)
 * If T has no T::type, returns T. Otherwise calls itself on T::type recursively.
 */

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ>
ਬਣਤਰ _hb_unwrap_type : hb_type_ਪਛਾਣ_ਤੱਤ_t<T> {};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ _hb_unwrap_type<T, hb_void_t<ਕਿਸਮ_ਨਾਂ T::type>> : _hb_unwrap_type<ਕਿਸਮ_ਨਾਂ T::type, ਖਾਲੀ> {};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਵਰਤੋਂ hb_unwrap_type = _hb_unwrap_type<T, ਖਾਲੀ>;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_unwrap_type(T) ਕਿਸਮ_ਨਾਂ hb_unwrap_type<T>::type

#ਜੇ_ਅੰਤ /* HB_META_HH */

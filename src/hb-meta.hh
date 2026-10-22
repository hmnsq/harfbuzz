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

#నిర్వచితం_కాకపోతే HB_META_HH
#నిర్వచించు HB_META_HH

#అన్తర్భావ "hb.hh"

#అన్తర్భావ <memory>
#అన్తర్భావ <type_traits>
#అన్తర్భావ <utility>


/*
 * C++ template meta-programming & fundamentals used with them.
 */

/* Void!  For when we need a expression-type of void. */
నిర్మాణం hb_ఖాళీలు_t {};

/* https://en.cppreference.com/w/cpp/types/void_t */
మూస<రకం_పేరు... Ts> నిర్మాణం _hb_void_t { రకనిర్వచనం శూన్య type; };
మూస<రకం_పేరు... Ts> ఉపయోగించి hb_void_t = రకం_పేరు _hb_void_t<Ts...>::type;

మూస<రకం_పేరు తల, రకం_పేరు... Ts> నిర్మాణం hb_తల_t { రకనిర్వచనం తల type; };
మూస<రకం_పేరు... Ts> ఉపయోగించి hb_తల_t_2 = రకం_పేరు hb_తల_t<Ts...>::type;

మూస <రకం_పేరు T, T v> నిర్మాణం hb_integral_constant { స్థిర constexpr T value = v; };
మూస <తార్కిక b> ఉపయోగించి hb_bool_constant = hb_integral_constant<తార్కిక, b>;
ఉపయోగించి hb_true_type = hb_bool_constant<నిజం>;
ఉపయోగించి hb_false_type = hb_bool_constant<అబద్ధం>;

/* Static-assert as expression. */
మూస <తార్కిక cond> నిర్మాణం static_assert_సమాసం;
మూస <> నిర్మాణం static_assert_సమాసం<నిజం> : hb_false_type {};
#నిర్వచించు static_assert_సమాసం(C) static_assert_సమాసం<C>::value

/* Basic type SFINAE. */

మూస <తార్కిక B, రకం_పేరు T = శూన్య> నిర్మాణం hb_enable_if {};
మూస <రకం_పేరు T>                నిర్మాణం hb_enable_if<నిజం, T> { రకనిర్వచనం T type; };
#నిర్వచించు hb_enable_if(Cond) రకం_పేరు hb_enable_if<(Cond)>::type* = శూన్యసూచిక
/* Concepts/Requires alias: */
#నిర్వచించు hb_requires(Cond) hb_enable_if((Cond))

మూస <రకం_పేరు T, రకం_పేరు T2> నిర్మాణం hb_is_same : hb_false_type {};
మూస <రకం_పేరు T>              నిర్మాణం hb_is_same<T, T> : hb_true_type {};
#నిర్వచించు hb_is_same(T, T2) hb_is_same<T, T2>::value

/* Function overloading SFINAE and priority. */

#నిర్వచించు HB_RETURN(Ret, E) -> hb_తల_t_2<Ret, decltype ((E))> { తిరిగి (E); }
#నిర్వచించు HB_AUTO_RETURN(E) -> decltype ((E)) { తిరిగి (E); }
#నిర్వచించు HB_VOID_RETURN(E) -> hb_void_t<decltype ((E))> { (E); }

మూస <గుర్తులేని Pri> నిర్మాణం hb_ప్రాధాన్యత : hb_ప్రాధాన్యత<Pri - 1> {};
మూస <>             నిర్మాణం hb_ప్రాధాన్యత<0> {};
#నిర్వచించు hb_prioritize hb_ప్రాధాన్యత<16> ()

#నిర్వచించు HB_FUNCOBJ(x) static_const x HB_వాడని


మూస <రకం_పేరు T> నిర్మాణం hb_type_తటస్థం_t { రకనిర్వచనం T type; };
మూస <రకం_పేరు T> ఉపయోగించి hb_type_తటస్థం = రకం_పేరు hb_type_తటస్థం_t<T>::type;

మూస <రకం_పేరు T> స్థిర అంతఃవరుస T hb_declval ();
#నిర్వచించు hb_declval(T) (hb_declval<T> ())

మూస <రకం_పేరు T> నిర్మాణం hb_సరిపోలిక_const		: hb_type_తటస్థం_t<T>, hb_false_type	{};
మూస <రకం_పేరు T> నిర్మాణం hb_సరిపోలిక_const<స్థిరరాశి T>	: hb_type_తటస్థం_t<T>, hb_true_type	{};
మూస <రకం_పేరు T> ఉపయోగించి hb_remove_const = రకం_పేరు hb_సరిపోలిక_const<T>::type;

మూస <రకం_పేరు T> నిర్మాణం hb_సరిపోలిక_reference		: hb_type_తటస్థం_t<T>, hb_false_type	{};
మూస <రకం_పేరు T> నిర్మాణం hb_సరిపోలిక_reference<T &>	: hb_type_తటస్థం_t<T>, hb_true_type	{};
మూస <రకం_పేరు T> నిర్మాణం hb_సరిపోలిక_reference<T &&>	: hb_type_తటస్థం_t<T>, hb_true_type	{};
మూస <రకం_పేరు T> ఉపయోగించి hb_remove_reference = రకం_పేరు hb_సరిపోలిక_reference<T>::type;
మూస <రకం_పేరు T> స్వయంచాలక hb_try_కూడు_lvalue_reference (hb_ప్రాధాన్యత<1>) -> hb_type_తటస్థం<T&>;
మూస <రకం_పేరు T> స్వయంచాలక hb_try_కూడు_lvalue_reference (hb_ప్రాధాన్యత<0>) -> hb_type_తటస్థం<T>;
మూస <రకం_పేరు T> ఉపయోగించి hb_కూడు_lvalue_reference = decltype (hb_try_కూడు_lvalue_reference<T> (hb_prioritize));
మూస <రకం_పేరు T> స్వయంచాలక hb_try_కూడు_rvalue_reference (hb_ప్రాధాన్యత<1>) -> hb_type_తటస్థం<T&&>;
మూస <రకం_పేరు T> స్వయంచాలక hb_try_కూడు_rvalue_reference (hb_ప్రాధాన్యత<0>) -> hb_type_తటస్థం<T>;
మూస <రకం_పేరు T> ఉపయోగించి hb_కూడు_rvalue_reference = decltype (hb_try_కూడు_rvalue_reference<T> (hb_prioritize));

మూస <రకం_పేరు T> నిర్మాణం hb_సరిపోలిక_సూచిక		: hb_type_తటస్థం_t<T>, hb_false_type	{};
మూస <రకం_పేరు T> నిర్మాణం hb_సరిపోలిక_సూచిక<T *>	: hb_type_తటస్థం_t<T>, hb_true_type	{};
మూస <రకం_పేరు T> ఉపయోగించి hb_remove_సూచిక = రకం_పేరు hb_సరిపోలిక_సూచిక<T>::type;
మూస <రకం_పేరు T> స్వయంచాలక hb_try_కూడు_సూచిక (hb_ప్రాధాన్యత<1>) -> hb_type_తటస్థం<hb_remove_reference<T>*>;
మూస <రకం_పేరు T> స్వయంచాలక hb_try_కూడు_సూచిక (hb_ప్రాధాన్యత<1>) -> hb_type_తటస్థం<T>;
మూస <రకం_పేరు T> ఉపయోగించి hb_కూడు_సూచిక = decltype (hb_try_కూడు_సూచిక<T> (hb_prioritize));


మూస <రకం_పేరు T> ఉపయోగించి hb_decay = రకం_పేరు std::decay<T>::type;

#నిర్వచించు hb_is_convertible(From,గమ్యం) std::is_convertible<From, గమ్యం>::value

మూస <రకం_పేరు From, రకం_పేరు గమ్యం>
ఉపయోగించి hb_is_cr_convertible = hb_bool_constant<
  hb_is_same (hb_decay<From>, hb_decay<గమ్యం>) &&
  (!std::is_const<From>::value || std::is_const<గమ్యం>::value) &&
  (!std::is_reference<గమ్యం>::value || std::is_const<గమ్యం>::value || std::is_reference<గమ్యం>::value)
>;
#నిర్వచించు hb_is_cr_convertible(From,గమ్యం) hb_is_cr_convertible<From, గమ్యం>::value


నిర్మాణం
{
  మూస <రకం_పేరు T> constexpr స్వయంచాలక
  క్రియాకారకం () (T&& v) స్థిరరాశి HB_AUTO_RETURN (std::forward<T> (v))

  మూస <రకం_పేరు T> constexpr స్వయంచాలక
  క్రియాకారకం () (T *v) స్థిరరాశి HB_AUTO_RETURN (*v)

  మూస <రకం_పేరు T> constexpr స్వయంచాలక
  క్రియాకారకం () (స్థిరరాశి hb::shared_ptr<T>& v) స్థిరరాశి HB_AUTO_RETURN (*v)

  మూస <రకం_పేరు T> constexpr స్వయంచాలక
  క్రియాకారకం () (hb::shared_ptr<T>& v) స్థిరరాశి HB_AUTO_RETURN (*v)
  
  మూస <రకం_పేరు T> constexpr స్వయంచాలక
  క్రియాకారకం () (స్థిరరాశి hb::unique_ptr<T>& v) స్థిరరాశి HB_AUTO_RETURN (*v)

  మూస <రకం_పేరు T> constexpr స్వయంచాలక
  క్రియాకారకం () (hb::unique_ptr<T>& v) స్థిరరాశి HB_AUTO_RETURN (*v)
}
HB_FUNCOBJ (hb_deref);

మూస <రకం_పేరు T>
నిర్మాణం hb_reference_wrapper
{
  hb_reference_wrapper (T v) : v (v) {}
  తార్కిక క్రియాకారకం == (స్థిరరాశి hb_reference_wrapper& o) స్థిరరాశి { తిరిగి v == o.v; }
  తార్కిక క్రియాకారకం != (స్థిరరాశి hb_reference_wrapper& o) స్థిరరాశి { తిరిగి v != o.v; }
  క్రియాకారకం T& () { తిరిగి v; }
  T& get () { తిరిగి v; }
  T v;
};
మూస <రకం_పేరు T>
నిర్మాణం hb_reference_wrapper<T&>
{
  hb_reference_wrapper (T& v) : v (std::addressof (v)) {}
  తార్కిక క్రియాకారకం == (స్థిరరాశి hb_reference_wrapper& o) స్థిరరాశి { తిరిగి v == o.v; }
  తార్కిక క్రియాకారకం != (స్థిరరాశి hb_reference_wrapper& o) స్థిరరాశి { తిరిగి v != o.v; }
  క్రియాకారకం T& () { తిరిగి *v; }
  T& get () { తిరిగి *v; }
  T* v;
};


/* Type traits */

మూస <రకం_పేరు T> నిర్మాణం hb_int_min;
మూస <> నిర్మాణం hb_int_min<అక్షరం>			: hb_integral_constant<అక్షరం,			CHAR_MIN>	{};
మూస <> నిర్మాణం hb_int_min<గుర్తుగల అక్షరం>		: hb_integral_constant<గుర్తుగల అక్షరం,		SCHAR_MIN>	{};
మూస <> నిర్మాణం hb_int_min<గుర్తులేని అక్షరం>		: hb_integral_constant<గుర్తులేని అక్షరం,		0>		{};
మూస <> నిర్మాణం hb_int_min<గుర్తుగల పొట్టి>		: hb_integral_constant<గుర్తుగల పొట్టి,		SHRT_MIN>	{};
మూస <> నిర్మాణం hb_int_min<గుర్తులేని పొట్టి>		: hb_integral_constant<గుర్తులేని పొట్టి,		0>		{};
మూస <> నిర్మాణం hb_int_min<గుర్తుగల పూర్ణాంకం>		: hb_integral_constant<గుర్తుగల పూర్ణాంకం,		INT_MIN>	{};
మూస <> నిర్మాణం hb_int_min<గుర్తులేని పూర్ణాంకం>		: hb_integral_constant<గుర్తులేని పూర్ణాంకం,		0>		{};
మూస <> నిర్మాణం hb_int_min<గుర్తుగల దీర్ఘ>		: hb_integral_constant<గుర్తుగల దీర్ఘ,		LONG_MIN>	{};
మూస <> నిర్మాణం hb_int_min<గుర్తులేని దీర్ఘ>		: hb_integral_constant<గుర్తులేని దీర్ఘ,		0>		{};
మూస <> నిర్మాణం hb_int_min<గుర్తుగల దీర్ఘ దీర్ఘ>		: hb_integral_constant<గుర్తుగల దీర్ఘ దీర్ఘ,	LLONG_MIN>	{};
మూస <> నిర్మాణం hb_int_min<గుర్తులేని దీర్ఘ దీర్ఘ>	: hb_integral_constant<గుర్తులేని దీర్ఘ దీర్ఘ,	0>		{};
మూస <రకం_పేరు T> నిర్మాణం hb_int_min<T *>		: hb_integral_constant<T *,			శూన్యసూచిక>	{};
#నిర్వచించు hb_int_min(T) hb_int_min<T>::value
మూస <రకం_పేరు T> నిర్మాణం hb_int_max;
మూస <> నిర్మాణం hb_int_max<అక్షరం>			: hb_integral_constant<అక్షరం,			CHAR_MAX>	{};
మూస <> నిర్మాణం hb_int_max<గుర్తుగల అక్షరం>		: hb_integral_constant<గుర్తుగల అక్షరం,		SCHAR_MAX>	{};
మూస <> నిర్మాణం hb_int_max<గుర్తులేని అక్షరం>		: hb_integral_constant<గుర్తులేని అక్షరం,		UCHAR_MAX>	{};
మూస <> నిర్మాణం hb_int_max<గుర్తుగల పొట్టి>		: hb_integral_constant<గుర్తుగల పొట్టి,		SHRT_MAX>	{};
మూస <> నిర్మాణం hb_int_max<గుర్తులేని పొట్టి>		: hb_integral_constant<గుర్తులేని పొట్టి,		USHRT_MAX>	{};
మూస <> నిర్మాణం hb_int_max<గుర్తుగల పూర్ణాంకం>		: hb_integral_constant<గుర్తుగల పూర్ణాంకం,		గరిష్ఠ_పూర్ణాంకం>	{};
మూస <> నిర్మాణం hb_int_max<గుర్తులేని పూర్ణాంకం>		: hb_integral_constant<గుర్తులేని పూర్ణాంకం,		UINT_MAX>	{};
మూస <> నిర్మాణం hb_int_max<గుర్తుగల దీర్ఘ>		: hb_integral_constant<గుర్తుగల దీర్ఘ,		LONG_MAX>	{};
మూస <> నిర్మాణం hb_int_max<గుర్తులేని దీర్ఘ>		: hb_integral_constant<గుర్తులేని దీర్ఘ,		ULONG_MAX>	{};
మూస <> నిర్మాణం hb_int_max<గుర్తుగల దీర్ఘ దీర్ఘ>		: hb_integral_constant<గుర్తుగల దీర్ఘ దీర్ఘ,	LLONG_MAX>	{};
మూస <> నిర్మాణం hb_int_max<గుర్తులేని దీర్ఘ దీర్ఘ>	: hb_integral_constant<గుర్తులేని దీర్ఘ దీర్ఘ,	ULLONG_MAX>	{};
#నిర్వచించు hb_int_max(T) hb_int_max<T>::value

#ఒకవేళ defined(__GNUC__) && __GNUC__ < 5 && !defined(__clang__)
#నిర్వచించు hb_is_trivially_copyable(T) __has_trivial_copy(T)
#నిర్వచించు hb_is_trivially_copy_assignable(T) __has_trivial_assign(T)
#నిర్వచించు hb_is_trivially_constructible(T) __has_trivial_constructor(T)
#నిర్వచించు hb_is_trivially_copy_constructible(T) __has_trivial_copy_constructor(T)
#నిర్వచించు hb_is_trivially_destructible(T) __has_trivial_destructor(T)
#లేకపోతే
#నిర్వచించు hb_is_trivially_copyable(T) std::is_trivially_copyable<T>::value
#నిర్వచించు hb_is_trivially_copy_assignable(T) std::is_trivially_copy_assignable<T>::value
#నిర్వచించు hb_is_trivially_constructible(T) std::is_trivially_constructible<T>::value
#నిర్వచించు hb_is_trivially_copy_constructible(T) std::is_trivially_copy_constructible<T>::value
#నిర్వచించు hb_is_trivially_destructible(T) std::is_trivially_destructible<T>::value
#అయితే_ముగింపు

/* Class traits. */

#నిర్వచించు HB_DELETE_COPY_కేటాయించు(TypeName) \
  TypeName(స్థిరరాశి TypeName&) = తొలగించు; \
  శూన్య క్రియాకారకం=(స్థిరరాశి TypeName&) = తొలగించు
#నిర్వచించు HB_DELETE_CREATE_COPY_కేటాయించు(TypeName) \
  TypeName() = తొలగించు; \
  TypeName(స్థిరరాశి TypeName&) = తొలగించు; \
  శూన్య క్రియాకారకం=(స్థిరరాశి TypeName&) = తొలగించు

/* hb_unwrap_type (T)
 * If T has no T::type, returns T. Otherwise calls itself on T::type recursively.
 */

మూస <రకం_పేరు T, రకం_పేరు>
నిర్మాణం _hb_unwrap_type : hb_type_తటస్థం_t<T> {};
మూస <రకం_పేరు T>
నిర్మాణం _hb_unwrap_type<T, hb_void_t<రకం_పేరు T::type>> : _hb_unwrap_type<రకం_పేరు T::type, శూన్య> {};
మూస <రకం_పేరు T>
ఉపయోగించి hb_unwrap_type = _hb_unwrap_type<T, శూన్య>;
#నిర్వచించు hb_unwrap_type(T) రకం_పేరు hb_unwrap_type<T>::type

#అయితే_ముగింపు /* HB_META_HH */

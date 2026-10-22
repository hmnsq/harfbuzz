/*
 * Copyright © 2018  Google, Inc.
 * Copyright © 2019  Facebook, Inc.
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
 * Facebook Author(s): Behdad Esfahbod
 */

#ifndef HB_ITER_HH
#define HB_ITER_HH

#અન્તર્ભાવ "hb.hh"
#અન્તર્ભાવ "hb-algs.hh"
#અન્તર્ભાવ "hb-meta.hh"


/* Unified iterator object.
 *
 * The goal of this template is to make the same iterator interface
 * available to all types, and make it very easy and compact to use.
 * hb_iter_tator objects are small, light-weight, objects that can be
 * copied by value.  If the collection / object being iterated on
 * is writable, then the iterator returns lvalues, otherwise it
 * returns rvalues.
 *
 * If iterator implementation implements operator!=, then it can be
 * used in range-based for loop.  That already happens if the iterator
 * is random-access.  Otherwise, the range-based for loop incurs
 * one traversal to find end(), which can be avoided if written
 * as a while-style for loop, or if iterator implements a faster
 * __end__() method. */

/*
 * Base classes for iterators.
 */

/* Base class for all iterators. */
template <typename iter_t, typename Item = typename iter_t::__item_t__>
સંરચના hb_iter_t
{
  પ્રકારવ્યાખ્યા Item item_t;
  constexpr unsigned get_item_size () અચલ { પાછા hb_static_size (Item); }
  સ્થિર constexpr તાર્કિક is_iterator = સાચું;
  સ્થિર constexpr તાર્કિક is_random_access_iterator = ખોટું;
  સ્થિર constexpr તાર્કિક is_sorted_iterator = ખોટું;
  સ્થિર constexpr તાર્કિક has_fast_len = ખોટું; // Should be checked in combination with is_random_access_iterator.

  ખાનગી:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  અચલ iter_t* thiz () અચલ { પાછા static_cast<અચલ iter_t *> (આ); }
	iter_t* thiz ()       { પાછા static_cast<      iter_t *> (આ); }
  જાહેર:

  /* Operators. */
  iter_t iter () અચલ { પાછા *thiz(); }
  iter_t operator + () અચલ { પાછા *thiz(); }
  iter_t _begin () અચલ { પાછા *thiz(); }
  iter_t begin () અચલ { પાછા _begin (); }
  iter_t _end () અચલ { પાછા thiz()->__end__ (); }
  iter_t end () અચલ { પાછા _end (); }
  explicit operator તાર્કિક () અચલ { પાછા thiz()->__more__ (); }
  unsigned len () અચલ { પાછા thiz()->__len__ (); }
  /* The following can only be enabled if item_t is reference type.  Otherwise
   * it will be returning pointer to temporary rvalue. */
  template <typename T = item_t,
	    hb_enable_if (std::is_reference<T>::value)>
  hb_remove_reference<item_t>* operator -> () અચલ { પાછા std::addressof (**thiz()); }
  item_t operator * () અચલ { પાછા thiz()->__item__ (); }
  item_t operator * () { પાછા thiz()->__item__ (); }
  item_t operator [] (unsigned i) અચલ { પાછા thiz()->__item_at__ (i); }
  item_t operator [] (unsigned i) { પાછા thiz()->__item_at__ (i); }
  iter_t& operator += (unsigned ગણતરી) &  { thiz()->__forward__ (ગણતરી); પાછા *thiz(); }
  iter_t  operator += (unsigned ગણતરી) && { thiz()->__forward__ (ગણતરી); પાછા *thiz(); }
  iter_t& operator ++ () &  { thiz()->__next__ (); પાછા *thiz(); }
  iter_t  operator ++ () && { thiz()->__next__ (); પાછા *thiz(); }
  iter_t& operator -= (unsigned ગણતરી) &  { thiz()->__rewind__ (ગણતરી); પાછા *thiz(); }
  iter_t  operator -= (unsigned ગણતરી) && { thiz()->__rewind__ (ગણતરી); પાછા *thiz(); }
  iter_t& operator -- () &  { thiz()->__prev__ (); પાછા *thiz(); }
  iter_t  operator -- () && { thiz()->__prev__ (); પાછા *thiz(); }
  iter_t operator + (unsigned ગણતરી) અચલ { auto c = thiz()->iter (); c += ગણતરી; પાછા c; }
  friend iter_t operator + (unsigned ગણતરી, અચલ iter_t &it) { પાછા it + ગણતરી; }
  iter_t operator ++ (પૂર્ણાંક) { iter_t c (*thiz()); ++*thiz(); પાછા c; }
  iter_t operator - (unsigned ગણતરી) અચલ { auto c = thiz()->iter (); c -= ગણતરી; પાછા c; }
  iter_t operator -- (પૂર્ણાંક) { iter_t c (*thiz()); --*thiz(); પાછા c; }
  template <typename T>
  iter_t& operator >> (T &v) &  { v = **thiz(); ++*thiz(); પાછા *thiz(); }
  template <typename T>
  iter_t  operator >> (T &v) && { v = **thiz(); ++*thiz(); પાછા *thiz(); }
  template <typename T>
  iter_t& operator << (અચલ T v) &  { **thiz() = v; ++*thiz(); પાછા *thiz(); }
  template <typename T>
  iter_t  operator << (અચલ T v) && { **thiz() = v; ++*thiz(); પાછા *thiz(); }

  સુરક્ષિત:
  hb_iter_t () = મૂળભૂત;
  hb_iter_t (અચલ hb_iter_t &o HB_UNUSED) = મૂળભૂત;
  hb_iter_t (hb_iter_t &&o HB_UNUSED) = મૂળભૂત;
  hb_iter_t& operator = (અચલ hb_iter_t &o HB_UNUSED) = મૂળભૂત;
  hb_iter_t& operator = (hb_iter_t &&o HB_UNUSED) = મૂળભૂત;
};

#define HB_ITER_USING(Name) \
  using item_t = typename Name::item_t; \
  using Name::_begin; \
  using Name::begin; \
  using Name::_end; \
  using Name::end; \
  using Name::get_item_size; \
  using Name::is_iterator; \
  using Name::iter; \
  using Name::operator તાર્કિક; \
  using Name::len; \
  using Name::operator ->; \
  using Name::operator *; \
  using Name::operator []; \
  using Name::operator +=; \
  using Name::operator ++; \
  using Name::operator -=; \
  using Name::operator --; \
  using Name::operator +; \
  using Name::operator -; \
  using Name::operator >>; \
  using Name::operator <<; \
  static_assert (સાચું, "")

/* Returns iterator / item type of a type. */
template <typename Iterable>
using hb_iter_type = decltype (hb_deref (hb_declval (Iterable)).iter ());
template <typename Iterable>
using hb_item_type = decltype (*hb_deref (hb_declval (Iterable)).iter ());


template <typename> સંરચના hb_array_t;
template <typename> સંરચના hb_sorted_array_t;

સંરચના
{
  template <typename T> hb_iter_type<T>
  operator () (T&& c) અચલ
  { પાછા hb_deref (std::forward<T> (c)).iter (); }

  /* Specialization for C arrays. */

  template <typename Type> અંતઃરેખ hb_array_t<Type>
  operator () (Type *array, unsigned પૂર્ણાંક length) અચલ
  { પાછા hb_array_t<Type> (array, length); }

  template <typename Type, unsigned પૂર્ણાંક length> hb_array_t<Type>
  operator () (Type (&array)[length]) અચલ
  { પાછા hb_array_t<Type> (array, length); }

}
HB_FUNCOBJ (hb_iter);
સંરચના
{
  template <typename T> auto
  impl (T&& c, hb_priority<1>) અચલ HB_RETURN (unsigned, c.len ())

  template <typename T> auto
  impl (T&& c, hb_priority<0>) અચલ HB_RETURN (unsigned, c.len)

  જાહેર:

  template <typename T> auto
  operator () (T&& c) અચલ HB_RETURN (unsigned, impl (std::forward<T> (c), hb_prioritize))
}
HB_FUNCOBJ (hb_len);

/* Mixin to fill in what the subclass doesn't provide. */
template <typename iter_t, typename item_t = typename iter_t::__item_t__>
સંરચના hb_iter_fallback_mixin_t
{
  ખાનગી:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  અચલ iter_t* thiz () અચલ { પાછા static_cast<અચલ iter_t *> (આ); }
	iter_t* thiz ()       { પાછા static_cast<      iter_t *> (આ); }
  જાહેર:

  /* Access: Implement __item__(), or __item_at__() if random-access. */
  item_t __item__ () અચલ { પાછા (*thiz())[0]; }
  item_t __item_at__ (unsigned i) અચલ { પાછા *(*thiz() + i); }

  /* Termination: Implement __more__(), or __len__() if random-access. */
  તાર્કિક __more__ () અચલ { પાછા તાર્કિક (thiz()->len ()); }
  unsigned __len__ () અચલ
  { iter_t c (*thiz()); unsigned l = 0; જ્યારે (c) { c++; l++; } પાછા l; }

  /* Advancing: Implement __next__(), or __forward__() if random-access. */
  શૂન્ય __next__ () { *thiz() += 1; }
  શૂન્ય __forward__ (unsigned n) { જ્યારે (*thiz() && n--) ++*thiz(); }

  /* Rewinding: Implement __prev__() or __rewind__() if bidirectional. */
  શૂન્ય __prev__ () { *thiz() -= 1; }
  શૂન્ય __rewind__ (unsigned n) { જ્યારે (*thiz() && n--) --*thiz(); }

  /* Range-based for: Implement __end__() if can be done faster,
   * and operator!=. */
  iter_t __end__ () અચલ
  {
    જો (thiz()->is_random_access_iterator)
      પાછા *thiz() + thiz()->len ();
    /* Above expression loops twice. Following loops once. */
    auto it = *thiz();
    જ્યારે (it) ++it;
    પાછા it;
  }

  સુરક્ષિત:
  hb_iter_fallback_mixin_t () = મૂળભૂત;
  hb_iter_fallback_mixin_t (અચલ hb_iter_fallback_mixin_t &o HB_UNUSED) = મૂળભૂત;
  hb_iter_fallback_mixin_t (hb_iter_fallback_mixin_t &&o HB_UNUSED) = મૂળભૂત;
  hb_iter_fallback_mixin_t& operator = (અચલ hb_iter_fallback_mixin_t &o HB_UNUSED) = મૂળભૂત;
  hb_iter_fallback_mixin_t& operator = (hb_iter_fallback_mixin_t &&o HB_UNUSED) = મૂળભૂત;
};

template <typename iter_t, typename item_t = typename iter_t::__item_t__>
સંરચના hb_iter_with_fallback_t :
  hb_iter_t<iter_t, item_t>,
  hb_iter_fallback_mixin_t<iter_t, item_t>
{
  સુરક્ષિત:
  hb_iter_with_fallback_t () = મૂળભૂત;
  hb_iter_with_fallback_t (અચલ hb_iter_with_fallback_t &o HB_UNUSED) = મૂળભૂત;
  hb_iter_with_fallback_t (hb_iter_with_fallback_t &&o HB_UNUSED) = મૂળભૂત;
  hb_iter_with_fallback_t& operator = (અચલ hb_iter_with_fallback_t &o HB_UNUSED) = મૂળભૂત;
  hb_iter_with_fallback_t& operator = (hb_iter_with_fallback_t &&o HB_UNUSED) = મૂળભૂત;
};

/*
 * Meta-programming predicates.
 */

/* hb_is_iterator() / hb_is_iterator_of() */

template<typename Iter, typename Item>
સંરચના hb_is_iterator_of
{
  template <typename Item2 = Item>
  સ્થિર hb_true_type impl (hb_priority<2>, hb_iter_t<Iter, hb_type_identity<Item2>> *);
  સ્થિર hb_false_type impl (hb_priority<0>, અચલ શૂન્ય *);

  જાહેર:
  સ્થિર constexpr તાર્કિક value = decltype (impl (hb_prioritize, hb_declval (Iter*)))::value;
};
#define hb_is_iterator_of(Iter, Item) hb_is_iterator_of<Iter, Item>::value
#define hb_is_iterator(Iter) hb_is_iterator_of (Iter, typename Iter::item_t)
#define hb_is_sorted_iterator_of(Iter, Item) (hb_is_iterator_of<Iter, Item>::value && Iter::is_sorted_iterator)
#define hb_is_sorted_iterator(Iter) hb_is_sorted_iterator_of (Iter, typename Iter::item_t)

/* hb_is_iterable() */

template <typename T>
સંરચના hb_is_iterable
{
  ખાનગી:

  template <typename U>
  સ્થિર auto impl (hb_priority<1>) -> decltype (hb_declval (U).iter (), hb_true_type ());

  template <typename>
  સ્થિર hb_false_type impl (hb_priority<0>);

  જાહેર:
  સ્થિર constexpr તાર્કિક value = decltype (impl<T> (hb_prioritize))::value;
};
#define hb_is_iterable(Iterable) hb_is_iterable<Iterable>::value

/* hb_is_source_of() / hb_is_sink_of() */

template<typename Iter, typename Item>
સંરચના hb_is_source_of
{
  ખાનગી:
  template <typename Iter2 = Iter,
	    hb_enable_if (hb_is_convertible (typename Iter2::item_t, hb_add_lvalue_reference<અચલ Item>))>
  સ્થિર hb_true_type impl (hb_priority<2>);
  template <typename Iter2 = Iter>
  સ્થિર auto impl (hb_priority<1>) -> decltype (hb_declval (Iter2) >> hb_declval (Item &), hb_true_type ());
  સ્થિર hb_false_type impl (hb_priority<0>);

  જાહેર:
  સ્થિર constexpr તાર્કિક value = decltype (impl (hb_prioritize))::value;
};
#define hb_is_source_of(Iter, Item) hb_is_source_of<Iter, Item>::value

template<typename Iter, typename Item>
સંરચના hb_is_sink_of
{
  ખાનગી:
  template <typename Iter2 = Iter,
	    hb_enable_if (hb_is_convertible (typename Iter2::item_t, hb_add_lvalue_reference<Item>))>
  સ્થિર hb_true_type impl (hb_priority<2>);
  template <typename Iter2 = Iter>
  સ્થિર auto impl (hb_priority<1>) -> decltype (hb_declval (Iter2) << hb_declval (Item), hb_true_type ());
  સ્થિર hb_false_type impl (hb_priority<0>);

  જાહેર:
  સ્થિર constexpr તાર્કિક value = decltype (impl (hb_prioritize))::value;
};
#define hb_is_sink_of(Iter, Item) hb_is_sink_of<Iter, Item>::value

/* This is commonly used, so define: */
#define hb_is_sorted_source_of(Iter, Item) \
	(hb_is_source_of(Iter, Item) && Iter::is_sorted_iterator)


સંરચના
{
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  unsigned operator () (અચલ Iterable &_) અચલ { પાછા hb_len (hb_iter (_)); }

  unsigned operator () (unsigned _) અચલ { પાછા _; }
}
HB_FUNCOBJ (hb_len_of);

/* Range-based 'for' for iterables. */

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
સ્થિર અંતઃરેખ auto begin (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).begin ())

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
સ્થિર અંતઃરેખ auto end (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).end ())

/* begin()/end() are NOT looked up non-ADL.  So each namespace must declare them.
 * Do it for namespace OT. */
નામાકાશ OT {

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
સ્થિર અંતઃરેખ auto begin (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).begin ())

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
સ્થિર અંતઃરેખ auto end (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).end ())

}


/*
 * Adaptors, combiners, etc.
 */

template <typename Lhs, typename Rhs,
	  hb_requires (hb_is_iterator (Lhs))>
સ્થિર અંતઃરેખ auto
operator | (Lhs&& lhs, Rhs&& rhs) HB_AUTO_RETURN (std::forward<Rhs> (rhs) (std::forward<Lhs> (lhs)))

/* hb_map(), hb_filter(), hb_reduce() */

ગણના  વર્ગ hb_function_sortedness_t {
  NOT_SORTED,
  RETAINS_SORTING,
  SORTED,
};

template <typename Iter, typename Proj, hb_function_sortedness_t Sorted,
	 hb_requires (hb_is_iterator (Iter))>
સંરચના hb_map_iter_t :
  hb_iter_t<hb_map_iter_t<Iter, Proj, Sorted>,
	    decltype (hb_get (hb_declval (Proj), *hb_declval (Iter)))>
{
  hb_map_iter_t (અચલ Iter& it, Proj f_) : it (it), f (f_) {}

  પ્રકારવ્યાખ્યા decltype (hb_get (hb_declval (Proj), *hb_declval (Iter))) __item_t__;
  સ્થિર constexpr તાર્કિક is_random_access_iterator = Iter::is_random_access_iterator;
  સ્થિર constexpr તાર્કિક is_sorted_iterator =
    Sorted == hb_function_sortedness_t::SORTED ? સાચું :
    Sorted == hb_function_sortedness_t::RETAINS_SORTING ? Iter::is_sorted_iterator :
    ખોટું;
  __item_t__ __item__ () અચલ { પાછા hb_get (f.get (), *it); }
  __item_t__ __item_at__ (unsigned i) અચલ { પાછા hb_get (f.get (), it[i]); }
  તાર્કિક __more__ () અચલ { પાછા તાર્કિક (it); }
  unsigned __len__ () અચલ { પાછા it.len (); }
  શૂન્ય __next__ () { ++it; }
  શૂન્ય __forward__ (unsigned n) { it += n; }
  શૂન્ય __prev__ () { --it; }
  શૂન્ય __rewind__ (unsigned n) { it -= n; }
  hb_map_iter_t __end__ () અચલ { પાછા hb_map_iter_t (it._end (), f); }
  તાર્કિક operator != (અચલ hb_map_iter_t& o) અચલ
  { પાછા it != o.it; }

  ખાનગી:
  Iter it;
  mutable hb_reference_wrapper<Proj> f;
};

template <typename Proj, hb_function_sortedness_t Sorted>
સંરચના hb_map_iter_factory_t
{
  hb_map_iter_factory_t (Proj f) : f (f) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  hb_map_iter_t<Iter, Proj, Sorted>
  operator () (Iter it)
  { પાછા hb_map_iter_t<Iter, Proj, Sorted> (it, f); }

  ખાનગી:
  Proj f;
};
સંરચના
{
  template <typename Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::NOT_SORTED>
  operator () (Proj&& f) અચલ
  { પાછા hb_map_iter_factory_t<Proj, hb_function_sortedness_t::NOT_SORTED> (f); }
}
HB_FUNCOBJ (hb_map);
સંરચના
{
  template <typename Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::RETAINS_SORTING>
  operator () (Proj&& f) અચલ
  { પાછા hb_map_iter_factory_t<Proj, hb_function_sortedness_t::RETAINS_SORTING> (f); }
}
HB_FUNCOBJ (hb_map_retains_sorting);
સંરચના
{
  template <typename Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::SORTED>
  operator () (Proj&& f) અચલ
  { પાછા hb_map_iter_factory_t<Proj, hb_function_sortedness_t::SORTED> (f); }
}
HB_FUNCOBJ (hb_map_sorted);

template <typename Iter, typename Pred, typename Proj,
	 hb_requires (hb_is_iterator (Iter))>
સંરચના hb_filter_iter_t :
  hb_iter_with_fallback_t<hb_filter_iter_t<Iter, Pred, Proj>,
			  typename Iter::item_t>
{
  hb_filter_iter_t (અચલ Iter& it_, Pred p_, Proj f_) : it (it_), p (p_), f (f_)
  { જ્યારે (it && !hb_has (p.get (), hb_get (f.get (), *it))) ++it; }

  પ્રકારવ્યાખ્યા typename Iter::item_t __item_t__;
  સ્થિર constexpr તાર્કિક is_sorted_iterator = Iter::is_sorted_iterator;
  __item_t__ __item__ () અચલ { પાછા *it; }
  તાર્કિક __more__ () અચલ { પાછા તાર્કિક (it); }
  શૂન્ય __next__ () { કરો ++it; જ્યારે (it && !hb_has (p.get (), hb_get (f.get (), *it))); }
  શૂન્ય __prev__ ()
  {
    unsigned ગણતરી = 0;
    કરો
    {
      Iter old = it;
      --it;
      જો (!(it != old))
      {
	it += ગણતરી;
	પાછા;
      }
      ગણતરી++;
    }
    જ્યારે (!hb_has (p.get (), hb_get (f.get (), *it)));
  }
  hb_filter_iter_t __end__ () અચલ { પાછા hb_filter_iter_t (it._end (), p, f); }
  તાર્કિક operator != (અચલ hb_filter_iter_t& o) અચલ
  { પાછા it != o.it; }

  ખાનગી:
  Iter it;
  mutable hb_reference_wrapper<Pred> p;
  mutable hb_reference_wrapper<Proj> f;
};
template <typename Pred, typename Proj>
સંરચના hb_filter_iter_factory_t
{
  hb_filter_iter_factory_t (Pred p, Proj f) : p (p), f (f) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  hb_filter_iter_t<Iter, Pred, Proj>
  operator () (Iter it)
  { પાછા hb_filter_iter_t<Iter, Pred, Proj> (it, p, f); }

  ખાનગી:
  Pred p;
  Proj f;
};
સંરચના
{
  template <typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity))>
  hb_filter_iter_factory_t<Pred, Proj>
  operator () (Pred&& p = hb_identity, Proj&& f = hb_identity) અચલ
  { પાછા hb_filter_iter_factory_t<Pred, Proj> (p, f); }
}
HB_FUNCOBJ (hb_filter);

template <typename Redu, typename InitT>
સંરચના hb_reduce_t
{
  hb_reduce_t (Redu r, InitT init_કિંમત) : r (r), init_કિંમત (init_કિંમત) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter)),
	    typename AccuT = hb_decay<decltype (hb_declval (Redu) (hb_declval (InitT), hb_declval (typename Iter::item_t)))>>
  AccuT
  operator () (Iter it)
  {
    AccuT value = init_કિંમત;
    માટે (; it; ++it)
      value = r (value, *it);
    પાછા value;
  }

  ખાનગી:
  Redu r;
  InitT init_કિંમત;
};
સંરચના
{
  template <typename Redu, typename InitT>
  hb_reduce_t<Redu, InitT>
  operator () (Redu&& r, InitT init_કિંમત) અચલ
  { પાછા hb_reduce_t<Redu, InitT> (r, init_કિંમત); }
}
HB_FUNCOBJ (hb_reduce);


/* hb_zip() */

template <typename A, typename B>
સંરચના hb_zip_iter_t :
  hb_iter_t<hb_zip_iter_t<A, B>,
	    hb_pair_t<typename A::item_t, typename B::item_t>>
{
  hb_zip_iter_t () {}
  hb_zip_iter_t (અચલ A& a, અચલ B& b) : a (a), b (b) {}

  પ્રકારવ્યાખ્યા hb_pair_t<typename A::item_t, typename B::item_t> __item_t__;
  સ્થિર constexpr તાર્કિક is_random_access_iterator =
    A::is_random_access_iterator &&
    B::is_random_access_iterator;
  /* Note.  The following categorization is only valid if A is strictly sorted,
   * ie. does NOT have duplicates.  Previously I tried to categorize sortedness
   * more granularly, see commits:
   *
   *   513762849a683914fc266a17ddf38f133cccf072
   *   4d3cf2adb669c345cc43832d11689271995e160a
   *
   * However, that was not enough, since hb_sorted_array_t, hb_sorted_vector_t,
   * SortedArrayOf, etc all needed to be updated to add more variants.  At that
   * point I saw it not worth the effort, and instead we now deem all sorted
   * collections as essentially strictly-sorted for the purposes of zip.
   *
   * The above assumption is not as bad as it sounds.  Our "sorted" comes with
   * no guarantees.  It's just a contract, put in place to help you remember,
   * and think about, whether an iterator you receive is expected to be
   * sorted or not.  As such, it's not perfect by definition, and should not
   * be treated so.  The inaccuracy here just errs in the direction of being
   * more permissive, so your code compiles instead of erring on the side of
   * marking your zipped iterator unsorted in which case your code won't
   * compile.
   *
   * This semantical limitation does NOT affect logic in any other place I
   * know of as of this writing.
   */
  સ્થિર constexpr તાર્કિક is_sorted_iterator = A::is_sorted_iterator;

  __item_t__ __item__ () અચલ { પાછા __item_t__ (*a, *b); }
  __item_t__ __item_at__ (unsigned i) અચલ { પાછા __item_t__ (a[i], b[i]); }
  તાર્કિક __more__ () અચલ { પાછા તાર્કિક (a) && તાર્કિક (b); }
  unsigned __len__ () અચલ { પાછા hb_min (a.len (), b.len ()); }
  શૂન્ય __next__ () { ++a; ++b; }
  શૂન્ય __forward__ (unsigned n) { a += n; b += n; }
  શૂન્ય __prev__ () { --a; --b; }
  શૂન્ય __rewind__ (unsigned n) { a -= n; b -= n; }
  hb_zip_iter_t __end__ () અચલ { પાછા hb_zip_iter_t (a._end (), b._end ()); }
  /* Note, we should stop if ANY of the iters reaches end.  As such two compare
   * unequal if both items are unequal, NOT if either is unequal. */
  તાર્કિક operator != (અચલ hb_zip_iter_t& o) અચલ
  { પાછા a != o.a && b != o.b; }

  ખાનગી:
  A a;
  B b;
};
સંરચના
{ HB_PARTIALIZE(2);
  template <typename A, typename B,
	    hb_requires (hb_is_iterable (A) && hb_is_iterable (B))>
  hb_zip_iter_t<hb_iter_type<A>, hb_iter_type<B>>
  operator () (A&& a, B&& b) અચલ
  { પાછા hb_zip_iter_t<hb_iter_type<A>, hb_iter_type<B>> (hb_iter (a), hb_iter (b)); }
}
HB_FUNCOBJ (hb_zip);

/* hb_concat() */

template <typename A, typename B>
સંરચના hb_concat_iter_t :
    hb_iter_t<hb_concat_iter_t<A, B>, typename A::item_t>
{
  hb_concat_iter_t () {}
  hb_concat_iter_t (A& a, B& b) : a (a), b (b) {}
  hb_concat_iter_t (અચલ A& a, અચલ B& b) : a (a), b (b) {}


  પ્રકારવ્યાખ્યા typename A::item_t __item_t__;
  સ્થિર constexpr તાર્કિક is_random_access_iterator =
    A::is_random_access_iterator &&
    B::is_random_access_iterator;
  સ્થિર constexpr તાર્કિક is_sorted_iterator = ખોટું;

  __item_t__ __item__ () અચલ
  {
    જો (!a)
      પાછા *b;
    પાછા *a;
  }

  __item_t__ __item_at__ (unsigned i) અચલ
  {
    unsigned a_len = a.len ();
    જો (i < a_len)
      પાછા a[i];
    પાછા b[i - a_len];
  }

  તાર્કિક __more__ () અચલ { પાછા તાર્કિક (a) || તાર્કિક (b); }

  unsigned __len__ () અચલ { પાછા a.len () + b.len (); }

  શૂન્ય __next__ ()
  {
    જો (a)
      ++a;
    નહીંતર
      ++b;
  }

  શૂન્ય __forward__ (unsigned n)
  {
    જો (!n) પાછા;
    જો (!is_random_access_iterator) {
      જ્યારે (n-- && *આ) {
        (*આ)++;
      }
      પાછા;
    }

    unsigned a_len = a.len ();
    જો (n > a_len) {
      n -= a_len;
      a.__forward__ (a_len);
      b.__forward__ (n);
    } નહીંતર {
      a.__forward__ (n);
    }
  }

  hb_concat_iter_t __end__ () અચલ { પાછા hb_concat_iter_t (a._end (), b._end ()); }
  તાર્કિક operator != (અચલ hb_concat_iter_t& o) અચલ
  {
    પાછા a != o.a
        || b != o.b;
  }

  ખાનગી:
  A a;
  B b;
};
સંરચના
{ HB_PARTIALIZE(2);
  template <typename A, typename B,
	    hb_requires (hb_is_iterable (A) && hb_is_iterable (B))>
  hb_concat_iter_t<hb_iter_type<A>, hb_iter_type<B>>
  operator () (A&& a, B&& b) અચલ
  { પાછા hb_concat_iter_t<hb_iter_type<A>, hb_iter_type<B>> (hb_iter (a), hb_iter (b)); }
}
HB_FUNCOBJ (hb_concat);

/* hb_apply() */

template <typename Appl>
સંરચના hb_apply_t
{
  hb_apply_t (Appl a) : a (a) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  શૂન્ય operator () (Iter it)
  {
    માટે (; it; ++it)
      (શૂન્ય) hb_invoke (a, *it);
  }

  ખાનગી:
  Appl a;
};
સંરચના
{
  template <typename Appl> hb_apply_t<Appl>
  operator () (Appl&& a) અચલ
  { પાછા hb_apply_t<Appl> (a); }

  template <typename Appl> hb_apply_t<Appl&>
  operator () (Appl *a) અચલ
  { પાછા hb_apply_t<Appl&> (*a); }
}
HB_FUNCOBJ (hb_apply);

/* hb_range()/hb_iota()/hb_repeat() */

template <typename T, typename S>
સંરચના hb_range_iter_t :
  hb_iter_t<hb_range_iter_t<T, S>, T>
{
  hb_range_iter_t (T start, T end_, S step) : v (start), end_ (end_for (start, end_, step)), step (step) {}

  પ્રકારવ્યાખ્યા T __item_t__;
  સ્થિર constexpr તાર્કિક is_random_access_iterator = સાચું;
  સ્થિર constexpr તાર્કિક is_sorted_iterator = સાચું;
  __item_t__ __item__ () અચલ { પાછા hb_ridentity (v); }
  __item_t__ __item_at__ (unsigned j) અચલ { પાછા v + j * step; }
  તાર્કિક __more__ () અચલ { પાછા v != end_; }
  unsigned __len__ () અચલ { પાછા !step ? UINT_MAX : (end_ - v) / step; }
  શૂન્ય __next__ () { v += step; }
  શૂન્ય __forward__ (unsigned n) { v += n * step; }
  શૂન્ય __prev__ () { v -= step; }
  શૂન્ય __rewind__ (unsigned n) { v -= n * step; }
  hb_range_iter_t __end__ () અચલ { પાછા hb_range_iter_t (end_, end_, step); }
  તાર્કિક operator != (અચલ hb_range_iter_t& o) અચલ
  { પાછા v != o.v; }

  ખાનગી:
  સ્થિર અંતઃરેખ T end_for (T start, T end_, S step)
  {
    જો (!step)
      પાછા end_;
    auto res = (end_ - start) % step;
    જો (!res)
      પાછા end_;
    end_ += step - res;
    પાછા end_;
  }

  ખાનગી:
  T v;
  T end_;
  S step;
};
સંરચના
{
  template <typename T = unsigned> hb_range_iter_t<T, unsigned>
  operator () (T end = (unsigned) -1) અચલ
  { પાછા hb_range_iter_t<T, unsigned> (0, end, 1u); }

  template <typename T, typename S = unsigned> hb_range_iter_t<T, S>
  operator () (T start, T end, S step = 1u) અચલ
  { પાછા hb_range_iter_t<T, S> (start, end, step); }
}
HB_FUNCOBJ (hb_range);

template <typename T, typename S>
સંરચના hb_iota_iter_t :
  hb_iter_with_fallback_t<hb_iota_iter_t<T, S>, T>
{
  hb_iota_iter_t (T start, S step) : v (start), step (step) {}

  ખાનગી:

  template <typename S2 = S>
  auto
  inc (hb_type_identity<S2> s, hb_priority<1>)
    -> hb_void_t<decltype (hb_invoke (std::forward<hb_type_identity<S2>> (s),
                                      hb_declval<T&> ()))>
  { v = hb_invoke (std::forward<hb_type_identity<S2>> (s), v); }

  શૂન્ય
  inc (S s, hb_priority<0>)
  { v += s; }

  જાહેર:

  પ્રકારવ્યાખ્યા T __item_t__;
  સ્થિર constexpr તાર્કિક is_random_access_iterator = સાચું;
  સ્થિર constexpr તાર્કિક is_sorted_iterator = સાચું;
  __item_t__ __item__ () અચલ { પાછા hb_ridentity (v); }
  તાર્કિક __more__ () અચલ { પાછા સાચું; }
  unsigned __len__ () અચલ { પાછા UINT_MAX; }
  શૂન્ય __next__ () { inc (step, hb_prioritize); }
  શૂન્ય __prev__ () { v -= step; }
  hb_iota_iter_t __end__ () અચલ { પાછા *આ; }
  તાર્કિક operator != (અચલ hb_iota_iter_t& o) અચલ { પાછા સાચું; }

  ખાનગી:
  T v;
  S step;
};
સંરચના
{
  template <typename T = unsigned, typename S = unsigned> hb_iota_iter_t<T, S>
  operator () (T start = 0u, S step = 1u) અચલ
  { પાછા hb_iota_iter_t<T, S> (start, step); }
}
HB_FUNCOBJ (hb_iota);

template <typename T>
સંરચના hb_repeat_iter_t :
  hb_iter_t<hb_repeat_iter_t<T>, T>
{
  hb_repeat_iter_t (T value) : v (value) {}

  પ્રકારવ્યાખ્યા T __item_t__;
  સ્થિર constexpr તાર્કિક is_random_access_iterator = સાચું;
  સ્થિર constexpr તાર્કિક is_sorted_iterator = સાચું;
  __item_t__ __item__ () અચલ { પાછા v; }
  __item_t__ __item_at__ (unsigned j) અચલ { પાછા v; }
  તાર્કિક __more__ () અચલ { પાછા સાચું; }
  unsigned __len__ () અચલ { પાછા UINT_MAX; }
  શૂન્ય __next__ () {}
  શૂન્ય __forward__ (unsigned) {}
  શૂન્ય __prev__ () {}
  શૂન્ય __rewind__ (unsigned) {}
  hb_repeat_iter_t __end__ () અચલ { પાછા *આ; }
  તાર્કિક operator != (અચલ hb_repeat_iter_t& o) અચલ { પાછા સાચું; }

  ખાનગી:
  T v;
};
સંરચના
{
  template <typename T> hb_repeat_iter_t<T>
  operator () (T value) અચલ
  { પાછા hb_repeat_iter_t<T> (value); }
}
HB_FUNCOBJ (hb_repeat);

/* hb_enumerate()/hb_take() */

સંરચના
{
  template <typename Iterable,
	    typename Index = unsigned,
	    hb_requires (hb_is_iterable (Iterable))>
  auto operator () (Iterable&& it, Index start = 0u) અચલ HB_AUTO_RETURN
  ( hb_zip (hb_iota (start), it) )
}
HB_FUNCOBJ (hb_enumerate);

સંરચના
{ HB_PARTIALIZE(2);
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  auto operator () (Iterable&& it, unsigned ગણતરી) અચલ HB_AUTO_RETURN
  ( hb_zip (hb_range (ગણતરી), it) | hb_map_retains_sorting (hb_second) )

  /* Specialization arrays. */

  template <typename Type> અંતઃરેખ hb_array_t<Type>
  operator () (hb_array_t<Type> array, unsigned ગણતરી) અચલ
  { પાછા array.sub_array (0, ગણતરી); }

  template <typename Type> અંતઃરેખ hb_sorted_array_t<Type>
  operator () (hb_sorted_array_t<Type> array, unsigned ગણતરી) અચલ
  { પાછા array.sub_array (0, ગણતરી); }
}
HB_FUNCOBJ (hb_take);

સંરચના
{ HB_PARTIALIZE(2);
  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  auto operator () (Iter it, unsigned ગણતરી) અચલ HB_AUTO_RETURN
  (
    + hb_iota (it, hb_add (ગણતરી))
    | hb_map (hb_take (ગણતરી))
    | hb_take ((hb_len (it) + ગણતરી - 1) / ગણતરી)
  )
}
HB_FUNCOBJ (hb_chop);

/* hb_sink() */

template <typename Sink>
સંરચના hb_sink_t
{
  hb_sink_t (Sink s) : s (s) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  શૂન્ય operator () (Iter it)
  {
    માટે (; it; ++it)
      s << *it;
  }

  ખાનગી:
  Sink s;
};
સંરચના
{
  template <typename Sink> hb_sink_t<Sink>
  operator () (Sink&& s) અચલ
  { પાછા hb_sink_t<Sink> (s); }

  template <typename Sink> hb_sink_t<Sink&>
  operator () (Sink *s) અચલ
  { પાછા hb_sink_t<Sink&> (*s); }
}
HB_FUNCOBJ (hb_sink);

/* hb-drain: hb_sink to void / blackhole / /dev/null. */

સંરચના
{
  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  શૂન્ય operator () (Iter it) અચલ
  {
    માટે (; it; ++it)
      (શૂન્ય) *it;
  }
}
HB_FUNCOBJ (hb_drain);

/* hb_unzip(): unzip and sink to two sinks. */

template <typename Sink1, typename Sink2>
સંરચના hb_unzip_t
{
  hb_unzip_t (Sink1 s1, Sink2 s2) : s1 (s1), s2 (s2) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  શૂન્ય operator () (Iter it)
  {
    માટે (; it; ++it)
    {
      અચલ auto &v = *it;
      s1 << v.first;
      s2 << v.second;
    }
  }

  ખાનગી:
  Sink1 s1;
  Sink2 s2;
};
સંરચના
{
  template <typename Sink1, typename Sink2> hb_unzip_t<Sink1, Sink2>
  operator () (Sink1&& s1, Sink2&& s2) અચલ
  { પાછા hb_unzip_t<Sink1, Sink2> (s1, s2); }

  template <typename Sink1, typename Sink2> hb_unzip_t<Sink1&, Sink2&>
  operator () (Sink1 *s1, Sink2 *s2) અચલ
  { પાછા hb_unzip_t<Sink1&, Sink2&> (*s1, *s2); }
}
HB_FUNCOBJ (hb_unzip);


/* hb-all, hb-any, hb-none. */

સંરચના
{
  template <typename Iterable,
	    typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity)),
	    hb_requires (hb_is_iterable (Iterable))>
  તાર્કિક operator () (Iterable&& c,
		    Pred&& p = hb_identity,
		    Proj&& f = hb_identity) અચલ
  {
    માટે (auto it = hb_iter (c); it; ++it)
      જો (!hb_match (p, hb_get (f, *it)))
	પાછા ખોટું;
    પાછા સાચું;
  }
}
HB_FUNCOBJ (hb_all);
સંરચના
{
  template <typename Iterable,
	    typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity)),
	    hb_requires (hb_is_iterable (Iterable))>
  તાર્કિક operator () (Iterable&& c,
		    Pred&& p = hb_identity,
		    Proj&& f = hb_identity) અચલ
  {
    માટે (auto it = hb_iter (c); it; ++it)
      જો (hb_match (p, hb_get (f, *it)))
	પાછા સાચું;
    પાછા ખોટું;
  }
}
HB_FUNCOBJ (hb_any);
સંરચના
{
  template <typename Iterable,
	    typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity)),
	    hb_requires (hb_is_iterable (Iterable))>
  તાર્કિક operator () (Iterable&& c,
		    Pred&& p = hb_identity,
		    Proj&& f = hb_identity) અચલ
  {
    માટે (auto it = hb_iter (c); it; ++it)
      જો (hb_match (p, hb_get (f, *it)))
	પાછા ખોટું;
    પાછા સાચું;
  }
}
HB_FUNCOBJ (hb_none);

/*
 * Algorithms operating on iterators.
 */

template <typename C, typename V,
	  hb_requires (hb_is_iterable (C))>
અંતઃરેખ શૂન્ય
hb_fill (C&& c, અચલ V &v)
{
  માટે (auto i = hb_iter (c); i; i++)
    *i = v;
}

template <typename S, typename D>
અંતઃરેખ શૂન્ય
hb_copy (S&& is, D&& id)
{
  hb_iter (is) | hb_sink (id);
}


#endif /* HB_ITER_HH */

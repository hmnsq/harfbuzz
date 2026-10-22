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

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-algs.hh"
#അന്തര്ഭാവ "hb-meta.hh"


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
ഘടന hb_iter_t
{
  തരനിർവചനം Item item_t;
  constexpr unsigned get_item_size () സ്ഥിരം { തിരികെ hb_static_size (Item); }
  സ്ഥിതം constexpr തർക്കം is_iterator = സത്യം;
  സ്ഥിതം constexpr തർക്കം is_random_access_iterator = അസത്യം;
  സ്ഥിതം constexpr തർക്കം is_sorted_iterator = അസത്യം;
  സ്ഥിതം constexpr തർക്കം has_fast_len = അസത്യം; // Should be checked in combination with is_random_access_iterator.

  സ്വകാര്യ:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  സ്ഥിരം iter_t* thiz () സ്ഥിരം { തിരികെ static_cast<സ്ഥിരം iter_t *> (ഇത്); }
	iter_t* thiz ()       { തിരികെ static_cast<      iter_t *> (ഇത്); }
  പൊതു:

  /* Operators. */
  iter_t iter () സ്ഥിരം { തിരികെ *thiz(); }
  iter_t operator + () സ്ഥിരം { തിരികെ *thiz(); }
  iter_t _begin () സ്ഥിരം { തിരികെ *thiz(); }
  iter_t begin () സ്ഥിരം { തിരികെ _begin (); }
  iter_t _end () സ്ഥിരം { തിരികെ thiz()->__end__ (); }
  iter_t end () സ്ഥിരം { തിരികെ _end (); }
  explicit operator തർക്കം () സ്ഥിരം { തിരികെ thiz()->__more__ (); }
  unsigned len () സ്ഥിരം { തിരികെ thiz()->__len__ (); }
  /* The following can only be enabled if item_t is reference type.  Otherwise
   * it will be returning pointer to temporary rvalue. */
  template <typename T = item_t,
	    hb_enable_if (std::is_reference<T>::value)>
  hb_remove_reference<item_t>* operator -> () സ്ഥിരം { തിരികെ std::addressof (**thiz()); }
  item_t operator * () സ്ഥിരം { തിരികെ thiz()->__item__ (); }
  item_t operator * () { തിരികെ thiz()->__item__ (); }
  item_t operator [] (unsigned i) സ്ഥിരം { തിരികെ thiz()->__item_at__ (i); }
  item_t operator [] (unsigned i) { തിരികെ thiz()->__item_at__ (i); }
  iter_t& operator += (unsigned എണ്ണം) &  { thiz()->__forward__ (എണ്ണം); തിരികെ *thiz(); }
  iter_t  operator += (unsigned എണ്ണം) && { thiz()->__forward__ (എണ്ണം); തിരികെ *thiz(); }
  iter_t& operator ++ () &  { thiz()->__next__ (); തിരികെ *thiz(); }
  iter_t  operator ++ () && { thiz()->__next__ (); തിരികെ *thiz(); }
  iter_t& operator -= (unsigned എണ്ണം) &  { thiz()->__rewind__ (എണ്ണം); തിരികെ *thiz(); }
  iter_t  operator -= (unsigned എണ്ണം) && { thiz()->__rewind__ (എണ്ണം); തിരികെ *thiz(); }
  iter_t& operator -- () &  { thiz()->__prev__ (); തിരികെ *thiz(); }
  iter_t  operator -- () && { thiz()->__prev__ (); തിരികെ *thiz(); }
  iter_t operator + (unsigned എണ്ണം) സ്ഥിരം { auto c = thiz()->iter (); c += എണ്ണം; തിരികെ c; }
  friend iter_t operator + (unsigned എണ്ണം, സ്ഥിരം iter_t &it) { തിരികെ it + എണ്ണം; }
  iter_t operator ++ (പൂർണ്ണസംഖ്യ) { iter_t c (*thiz()); ++*thiz(); തിരികെ c; }
  iter_t operator - (unsigned എണ്ണം) സ്ഥിരം { auto c = thiz()->iter (); c -= എണ്ണം; തിരികെ c; }
  iter_t operator -- (പൂർണ്ണസംഖ്യ) { iter_t c (*thiz()); --*thiz(); തിരികെ c; }
  template <typename T>
  iter_t& operator >> (T &v) &  { v = **thiz(); ++*thiz(); തിരികെ *thiz(); }
  template <typename T>
  iter_t  operator >> (T &v) && { v = **thiz(); ++*thiz(); തിരികെ *thiz(); }
  template <typename T>
  iter_t& operator << (സ്ഥിരം T v) &  { **thiz() = v; ++*thiz(); തിരികെ *thiz(); }
  template <typename T>
  iter_t  operator << (സ്ഥിരം T v) && { **thiz() = v; ++*thiz(); തിരികെ *thiz(); }

  സംരക്ഷിത:
  hb_iter_t () = സ്ഥിരസ്ഥിതി;
  hb_iter_t (സ്ഥിരം hb_iter_t &o HB_UNUSED) = സ്ഥിരസ്ഥിതി;
  hb_iter_t (hb_iter_t &&o HB_UNUSED) = സ്ഥിരസ്ഥിതി;
  hb_iter_t& operator = (സ്ഥിരം hb_iter_t &o HB_UNUSED) = സ്ഥിരസ്ഥിതി;
  hb_iter_t& operator = (hb_iter_t &&o HB_UNUSED) = സ്ഥിരസ്ഥിതി;
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
  using Name::operator തർക്കം; \
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
  static_assert (സത്യം, "")

/* Returns iterator / item type of a type. */
template <typename Iterable>
using hb_iter_type = decltype (hb_deref (hb_declval (Iterable)).iter ());
template <typename Iterable>
using hb_item_type = decltype (*hb_deref (hb_declval (Iterable)).iter ());


template <typename> ഘടന hb_array_t;
template <typename> ഘടന hb_sorted_array_t;

ഘടന
{
  template <typename T> hb_iter_type<T>
  operator () (T&& c) സ്ഥിരം
  { തിരികെ hb_deref (std::forward<T> (c)).iter (); }

  /* Specialization for C arrays. */

  template <typename Type> വരിയിൽ hb_array_t<Type>
  operator () (Type *array, unsigned പൂർണ്ണസംഖ്യ length) സ്ഥിരം
  { തിരികെ hb_array_t<Type> (array, length); }

  template <typename Type, unsigned പൂർണ്ണസംഖ്യ length> hb_array_t<Type>
  operator () (Type (&array)[length]) സ്ഥിരം
  { തിരികെ hb_array_t<Type> (array, length); }

}
HB_FUNCOBJ (hb_iter);
ഘടന
{
  template <typename T> auto
  impl (T&& c, hb_priority<1>) സ്ഥിരം HB_RETURN (unsigned, c.len ())

  template <typename T> auto
  impl (T&& c, hb_priority<0>) സ്ഥിരം HB_RETURN (unsigned, c.len)

  പൊതു:

  template <typename T> auto
  operator () (T&& c) സ്ഥിരം HB_RETURN (unsigned, impl (std::forward<T> (c), hb_prioritize))
}
HB_FUNCOBJ (hb_len);

/* Mixin to fill in what the subclass doesn't provide. */
template <typename iter_t, typename item_t = typename iter_t::__item_t__>
ഘടന hb_iter_fallback_mixin_t
{
  സ്വകാര്യ:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  സ്ഥിരം iter_t* thiz () സ്ഥിരം { തിരികെ static_cast<സ്ഥിരം iter_t *> (ഇത്); }
	iter_t* thiz ()       { തിരികെ static_cast<      iter_t *> (ഇത്); }
  പൊതു:

  /* Access: Implement __item__(), or __item_at__() if random-access. */
  item_t __item__ () സ്ഥിരം { തിരികെ (*thiz())[0]; }
  item_t __item_at__ (unsigned i) സ്ഥിരം { തിരികെ *(*thiz() + i); }

  /* Termination: Implement __more__(), or __len__() if random-access. */
  തർക്കം __more__ () സ്ഥിരം { തിരികെ തർക്കം (thiz()->len ()); }
  unsigned __len__ () സ്ഥിരം
  { iter_t c (*thiz()); unsigned l = 0; ആകുമ്പോൾ (c) { c++; l++; } തിരികെ l; }

  /* Advancing: Implement __next__(), or __forward__() if random-access. */
  ശൂന്യം __next__ () { *thiz() += 1; }
  ശൂന്യം __forward__ (unsigned n) { ആകുമ്പോൾ (*thiz() && n--) ++*thiz(); }

  /* Rewinding: Implement __prev__() or __rewind__() if bidirectional. */
  ശൂന്യം __prev__ () { *thiz() -= 1; }
  ശൂന്യം __rewind__ (unsigned n) { ആകുമ്പോൾ (*thiz() && n--) --*thiz(); }

  /* Range-based for: Implement __end__() if can be done faster,
   * and operator!=. */
  iter_t __end__ () സ്ഥിരം
  {
    എങ്കിൽ (thiz()->is_random_access_iterator)
      തിരികെ *thiz() + thiz()->len ();
    /* Above expression loops twice. Following loops once. */
    auto it = *thiz();
    ആകുമ്പോൾ (it) ++it;
    തിരികെ it;
  }

  സംരക്ഷിത:
  hb_iter_fallback_mixin_t () = സ്ഥിരസ്ഥിതി;
  hb_iter_fallback_mixin_t (സ്ഥിരം hb_iter_fallback_mixin_t &o HB_UNUSED) = സ്ഥിരസ്ഥിതി;
  hb_iter_fallback_mixin_t (hb_iter_fallback_mixin_t &&o HB_UNUSED) = സ്ഥിരസ്ഥിതി;
  hb_iter_fallback_mixin_t& operator = (സ്ഥിരം hb_iter_fallback_mixin_t &o HB_UNUSED) = സ്ഥിരസ്ഥിതി;
  hb_iter_fallback_mixin_t& operator = (hb_iter_fallback_mixin_t &&o HB_UNUSED) = സ്ഥിരസ്ഥിതി;
};

template <typename iter_t, typename item_t = typename iter_t::__item_t__>
ഘടന hb_iter_with_fallback_t :
  hb_iter_t<iter_t, item_t>,
  hb_iter_fallback_mixin_t<iter_t, item_t>
{
  സംരക്ഷിത:
  hb_iter_with_fallback_t () = സ്ഥിരസ്ഥിതി;
  hb_iter_with_fallback_t (സ്ഥിരം hb_iter_with_fallback_t &o HB_UNUSED) = സ്ഥിരസ്ഥിതി;
  hb_iter_with_fallback_t (hb_iter_with_fallback_t &&o HB_UNUSED) = സ്ഥിരസ്ഥിതി;
  hb_iter_with_fallback_t& operator = (സ്ഥിരം hb_iter_with_fallback_t &o HB_UNUSED) = സ്ഥിരസ്ഥിതി;
  hb_iter_with_fallback_t& operator = (hb_iter_with_fallback_t &&o HB_UNUSED) = സ്ഥിരസ്ഥിതി;
};

/*
 * Meta-programming predicates.
 */

/* hb_is_iterator() / hb_is_iterator_of() */

template<typename Iter, typename Item>
ഘടന hb_is_iterator_of
{
  template <typename Item2 = Item>
  സ്ഥിതം hb_true_type impl (hb_priority<2>, hb_iter_t<Iter, hb_type_identity<Item2>> *);
  സ്ഥിതം hb_false_type impl (hb_priority<0>, സ്ഥിരം ശൂന്യം *);

  പൊതു:
  സ്ഥിതം constexpr തർക്കം value = decltype (impl (hb_prioritize, hb_declval (Iter*)))::value;
};
#define hb_is_iterator_of(Iter, Item) hb_is_iterator_of<Iter, Item>::value
#define hb_is_iterator(Iter) hb_is_iterator_of (Iter, typename Iter::item_t)
#define hb_is_sorted_iterator_of(Iter, Item) (hb_is_iterator_of<Iter, Item>::value && Iter::is_sorted_iterator)
#define hb_is_sorted_iterator(Iter) hb_is_sorted_iterator_of (Iter, typename Iter::item_t)

/* hb_is_iterable() */

template <typename T>
ഘടന hb_is_iterable
{
  സ്വകാര്യ:

  template <typename U>
  സ്ഥിതം auto impl (hb_priority<1>) -> decltype (hb_declval (U).iter (), hb_true_type ());

  template <typename>
  സ്ഥിതം hb_false_type impl (hb_priority<0>);

  പൊതു:
  സ്ഥിതം constexpr തർക്കം value = decltype (impl<T> (hb_prioritize))::value;
};
#define hb_is_iterable(Iterable) hb_is_iterable<Iterable>::value

/* hb_is_source_of() / hb_is_sink_of() */

template<typename Iter, typename Item>
ഘടന hb_is_source_of
{
  സ്വകാര്യ:
  template <typename Iter2 = Iter,
	    hb_enable_if (hb_is_convertible (typename Iter2::item_t, hb_add_lvalue_reference<സ്ഥിരം Item>))>
  സ്ഥിതം hb_true_type impl (hb_priority<2>);
  template <typename Iter2 = Iter>
  സ്ഥിതം auto impl (hb_priority<1>) -> decltype (hb_declval (Iter2) >> hb_declval (Item &), hb_true_type ());
  സ്ഥിതം hb_false_type impl (hb_priority<0>);

  പൊതു:
  സ്ഥിതം constexpr തർക്കം value = decltype (impl (hb_prioritize))::value;
};
#define hb_is_source_of(Iter, Item) hb_is_source_of<Iter, Item>::value

template<typename Iter, typename Item>
ഘടന hb_is_sink_of
{
  സ്വകാര്യ:
  template <typename Iter2 = Iter,
	    hb_enable_if (hb_is_convertible (typename Iter2::item_t, hb_add_lvalue_reference<Item>))>
  സ്ഥിതം hb_true_type impl (hb_priority<2>);
  template <typename Iter2 = Iter>
  സ്ഥിതം auto impl (hb_priority<1>) -> decltype (hb_declval (Iter2) << hb_declval (Item), hb_true_type ());
  സ്ഥിതം hb_false_type impl (hb_priority<0>);

  പൊതു:
  സ്ഥിതം constexpr തർക്കം value = decltype (impl (hb_prioritize))::value;
};
#define hb_is_sink_of(Iter, Item) hb_is_sink_of<Iter, Item>::value

/* This is commonly used, so define: */
#define hb_is_sorted_source_of(Iter, Item) \
	(hb_is_source_of(Iter, Item) && Iter::is_sorted_iterator)


ഘടന
{
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  unsigned operator () (സ്ഥിരം Iterable &_) സ്ഥിരം { തിരികെ hb_len (hb_iter (_)); }

  unsigned operator () (unsigned _) സ്ഥിരം { തിരികെ _; }
}
HB_FUNCOBJ (hb_len_of);

/* Range-based 'for' for iterables. */

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
സ്ഥിതം വരിയിൽ auto begin (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).begin ())

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
സ്ഥിതം വരിയിൽ auto end (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).end ())

/* begin()/end() are NOT looked up non-ADL.  So each namespace must declare them.
 * Do it for namespace OT. */
നാമാകാശ OT {

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
സ്ഥിതം വരിയിൽ auto begin (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).begin ())

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
സ്ഥിതം വരിയിൽ auto end (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).end ())

}


/*
 * Adaptors, combiners, etc.
 */

template <typename Lhs, typename Rhs,
	  hb_requires (hb_is_iterator (Lhs))>
സ്ഥിതം വരിയിൽ auto
operator | (Lhs&& lhs, Rhs&& rhs) HB_AUTO_RETURN (std::forward<Rhs> (rhs) (std::forward<Lhs> (lhs)))

/* hb_map(), hb_filter(), hb_reduce() */

എണ്ണൽ  വർഗ്ഗം hb_function_sortedness_t {
  NOT_SORTED,
  RETAINS_SORTING,
  SORTED,
};

template <typename Iter, typename Proj, hb_function_sortedness_t Sorted,
	 hb_requires (hb_is_iterator (Iter))>
ഘടന hb_map_iter_t :
  hb_iter_t<hb_map_iter_t<Iter, Proj, Sorted>,
	    decltype (hb_get (hb_declval (Proj), *hb_declval (Iter)))>
{
  hb_map_iter_t (സ്ഥിരം Iter& it, Proj f_) : it (it), f (f_) {}

  തരനിർവചനം decltype (hb_get (hb_declval (Proj), *hb_declval (Iter))) __item_t__;
  സ്ഥിതം constexpr തർക്കം is_random_access_iterator = Iter::is_random_access_iterator;
  സ്ഥിതം constexpr തർക്കം is_sorted_iterator =
    Sorted == hb_function_sortedness_t::SORTED ? സത്യം :
    Sorted == hb_function_sortedness_t::RETAINS_SORTING ? Iter::is_sorted_iterator :
    അസത്യം;
  __item_t__ __item__ () സ്ഥിരം { തിരികെ hb_get (f.get (), *it); }
  __item_t__ __item_at__ (unsigned i) സ്ഥിരം { തിരികെ hb_get (f.get (), it[i]); }
  തർക്കം __more__ () സ്ഥിരം { തിരികെ തർക്കം (it); }
  unsigned __len__ () സ്ഥിരം { തിരികെ it.len (); }
  ശൂന്യം __next__ () { ++it; }
  ശൂന്യം __forward__ (unsigned n) { it += n; }
  ശൂന്യം __prev__ () { --it; }
  ശൂന്യം __rewind__ (unsigned n) { it -= n; }
  hb_map_iter_t __end__ () സ്ഥിരം { തിരികെ hb_map_iter_t (it._end (), f); }
  തർക്കം operator != (സ്ഥിരം hb_map_iter_t& o) സ്ഥിരം
  { തിരികെ it != o.it; }

  സ്വകാര്യ:
  Iter it;
  mutable hb_reference_wrapper<Proj> f;
};

template <typename Proj, hb_function_sortedness_t Sorted>
ഘടന hb_map_iter_factory_t
{
  hb_map_iter_factory_t (Proj f) : f (f) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  hb_map_iter_t<Iter, Proj, Sorted>
  operator () (Iter it)
  { തിരികെ hb_map_iter_t<Iter, Proj, Sorted> (it, f); }

  സ്വകാര്യ:
  Proj f;
};
ഘടന
{
  template <typename Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::NOT_SORTED>
  operator () (Proj&& f) സ്ഥിരം
  { തിരികെ hb_map_iter_factory_t<Proj, hb_function_sortedness_t::NOT_SORTED> (f); }
}
HB_FUNCOBJ (hb_map);
ഘടന
{
  template <typename Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::RETAINS_SORTING>
  operator () (Proj&& f) സ്ഥിരം
  { തിരികെ hb_map_iter_factory_t<Proj, hb_function_sortedness_t::RETAINS_SORTING> (f); }
}
HB_FUNCOBJ (hb_map_retains_sorting);
ഘടന
{
  template <typename Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::SORTED>
  operator () (Proj&& f) സ്ഥിരം
  { തിരികെ hb_map_iter_factory_t<Proj, hb_function_sortedness_t::SORTED> (f); }
}
HB_FUNCOBJ (hb_map_sorted);

template <typename Iter, typename Pred, typename Proj,
	 hb_requires (hb_is_iterator (Iter))>
ഘടന hb_filter_iter_t :
  hb_iter_with_fallback_t<hb_filter_iter_t<Iter, Pred, Proj>,
			  typename Iter::item_t>
{
  hb_filter_iter_t (സ്ഥിരം Iter& it_, Pred p_, Proj f_) : it (it_), p (p_), f (f_)
  { ആകുമ്പോൾ (it && !hb_has (p.get (), hb_get (f.get (), *it))) ++it; }

  തരനിർവചനം typename Iter::item_t __item_t__;
  സ്ഥിതം constexpr തർക്കം is_sorted_iterator = Iter::is_sorted_iterator;
  __item_t__ __item__ () സ്ഥിരം { തിരികെ *it; }
  തർക്കം __more__ () സ്ഥിരം { തിരികെ തർക്കം (it); }
  ശൂന്യം __next__ () { ചെയ്യുക ++it; ആകുമ്പോൾ (it && !hb_has (p.get (), hb_get (f.get (), *it))); }
  ശൂന്യം __prev__ ()
  {
    unsigned എണ്ണം = 0;
    ചെയ്യുക
    {
      Iter old = it;
      --it;
      എങ്കിൽ (!(it != old))
      {
	it += എണ്ണം;
	തിരികെ;
      }
      എണ്ണം++;
    }
    ആകുമ്പോൾ (!hb_has (p.get (), hb_get (f.get (), *it)));
  }
  hb_filter_iter_t __end__ () സ്ഥിരം { തിരികെ hb_filter_iter_t (it._end (), p, f); }
  തർക്കം operator != (സ്ഥിരം hb_filter_iter_t& o) സ്ഥിരം
  { തിരികെ it != o.it; }

  സ്വകാര്യ:
  Iter it;
  mutable hb_reference_wrapper<Pred> p;
  mutable hb_reference_wrapper<Proj> f;
};
template <typename Pred, typename Proj>
ഘടന hb_filter_iter_factory_t
{
  hb_filter_iter_factory_t (Pred p, Proj f) : p (p), f (f) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  hb_filter_iter_t<Iter, Pred, Proj>
  operator () (Iter it)
  { തിരികെ hb_filter_iter_t<Iter, Pred, Proj> (it, p, f); }

  സ്വകാര്യ:
  Pred p;
  Proj f;
};
ഘടന
{
  template <typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity))>
  hb_filter_iter_factory_t<Pred, Proj>
  operator () (Pred&& p = hb_identity, Proj&& f = hb_identity) സ്ഥിരം
  { തിരികെ hb_filter_iter_factory_t<Pred, Proj> (p, f); }
}
HB_FUNCOBJ (hb_filter);

template <typename Redu, typename InitT>
ഘടന hb_reduce_t
{
  hb_reduce_t (Redu r, InitT init_മൂല്യം) : r (r), init_മൂല്യം (init_മൂല്യം) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter)),
	    typename AccuT = hb_decay<decltype (hb_declval (Redu) (hb_declval (InitT), hb_declval (typename Iter::item_t)))>>
  AccuT
  operator () (Iter it)
  {
    AccuT value = init_മൂല്യം;
    വേണ്ടി (; it; ++it)
      value = r (value, *it);
    തിരികെ value;
  }

  സ്വകാര്യ:
  Redu r;
  InitT init_മൂല്യം;
};
ഘടന
{
  template <typename Redu, typename InitT>
  hb_reduce_t<Redu, InitT>
  operator () (Redu&& r, InitT init_മൂല്യം) സ്ഥിരം
  { തിരികെ hb_reduce_t<Redu, InitT> (r, init_മൂല്യം); }
}
HB_FUNCOBJ (hb_reduce);


/* hb_zip() */

template <typename A, typename B>
ഘടന hb_zip_iter_t :
  hb_iter_t<hb_zip_iter_t<A, B>,
	    hb_pair_t<typename A::item_t, typename B::item_t>>
{
  hb_zip_iter_t () {}
  hb_zip_iter_t (സ്ഥിരം A& a, സ്ഥിരം B& b) : a (a), b (b) {}

  തരനിർവചനം hb_pair_t<typename A::item_t, typename B::item_t> __item_t__;
  സ്ഥിതം constexpr തർക്കം is_random_access_iterator =
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
  സ്ഥിതം constexpr തർക്കം is_sorted_iterator = A::is_sorted_iterator;

  __item_t__ __item__ () സ്ഥിരം { തിരികെ __item_t__ (*a, *b); }
  __item_t__ __item_at__ (unsigned i) സ്ഥിരം { തിരികെ __item_t__ (a[i], b[i]); }
  തർക്കം __more__ () സ്ഥിരം { തിരികെ തർക്കം (a) && തർക്കം (b); }
  unsigned __len__ () സ്ഥിരം { തിരികെ hb_min (a.len (), b.len ()); }
  ശൂന്യം __next__ () { ++a; ++b; }
  ശൂന്യം __forward__ (unsigned n) { a += n; b += n; }
  ശൂന്യം __prev__ () { --a; --b; }
  ശൂന്യം __rewind__ (unsigned n) { a -= n; b -= n; }
  hb_zip_iter_t __end__ () സ്ഥിരം { തിരികെ hb_zip_iter_t (a._end (), b._end ()); }
  /* Note, we should stop if ANY of the iters reaches end.  As such two compare
   * unequal if both items are unequal, NOT if either is unequal. */
  തർക്കം operator != (സ്ഥിരം hb_zip_iter_t& o) സ്ഥിരം
  { തിരികെ a != o.a && b != o.b; }

  സ്വകാര്യ:
  A a;
  B b;
};
ഘടന
{ HB_PARTIALIZE(2);
  template <typename A, typename B,
	    hb_requires (hb_is_iterable (A) && hb_is_iterable (B))>
  hb_zip_iter_t<hb_iter_type<A>, hb_iter_type<B>>
  operator () (A&& a, B&& b) സ്ഥിരം
  { തിരികെ hb_zip_iter_t<hb_iter_type<A>, hb_iter_type<B>> (hb_iter (a), hb_iter (b)); }
}
HB_FUNCOBJ (hb_zip);

/* hb_concat() */

template <typename A, typename B>
ഘടന hb_concat_iter_t :
    hb_iter_t<hb_concat_iter_t<A, B>, typename A::item_t>
{
  hb_concat_iter_t () {}
  hb_concat_iter_t (A& a, B& b) : a (a), b (b) {}
  hb_concat_iter_t (സ്ഥിരം A& a, സ്ഥിരം B& b) : a (a), b (b) {}


  തരനിർവചനം typename A::item_t __item_t__;
  സ്ഥിതം constexpr തർക്കം is_random_access_iterator =
    A::is_random_access_iterator &&
    B::is_random_access_iterator;
  സ്ഥിതം constexpr തർക്കം is_sorted_iterator = അസത്യം;

  __item_t__ __item__ () സ്ഥിരം
  {
    എങ്കിൽ (!a)
      തിരികെ *b;
    തിരികെ *a;
  }

  __item_t__ __item_at__ (unsigned i) സ്ഥിരം
  {
    unsigned a_len = a.len ();
    എങ്കിൽ (i < a_len)
      തിരികെ a[i];
    തിരികെ b[i - a_len];
  }

  തർക്കം __more__ () സ്ഥിരം { തിരികെ തർക്കം (a) || തർക്കം (b); }

  unsigned __len__ () സ്ഥിരം { തിരികെ a.len () + b.len (); }

  ശൂന്യം __next__ ()
  {
    എങ്കിൽ (a)
      ++a;
    അല്ലെങ്കിൽ
      ++b;
  }

  ശൂന്യം __forward__ (unsigned n)
  {
    എങ്കിൽ (!n) തിരികെ;
    എങ്കിൽ (!is_random_access_iterator) {
      ആകുമ്പോൾ (n-- && *ഇത്) {
        (*ഇത്)++;
      }
      തിരികെ;
    }

    unsigned a_len = a.len ();
    എങ്കിൽ (n > a_len) {
      n -= a_len;
      a.__forward__ (a_len);
      b.__forward__ (n);
    } അല്ലെങ്കിൽ {
      a.__forward__ (n);
    }
  }

  hb_concat_iter_t __end__ () സ്ഥിരം { തിരികെ hb_concat_iter_t (a._end (), b._end ()); }
  തർക്കം operator != (സ്ഥിരം hb_concat_iter_t& o) സ്ഥിരം
  {
    തിരികെ a != o.a
        || b != o.b;
  }

  സ്വകാര്യ:
  A a;
  B b;
};
ഘടന
{ HB_PARTIALIZE(2);
  template <typename A, typename B,
	    hb_requires (hb_is_iterable (A) && hb_is_iterable (B))>
  hb_concat_iter_t<hb_iter_type<A>, hb_iter_type<B>>
  operator () (A&& a, B&& b) സ്ഥിരം
  { തിരികെ hb_concat_iter_t<hb_iter_type<A>, hb_iter_type<B>> (hb_iter (a), hb_iter (b)); }
}
HB_FUNCOBJ (hb_concat);

/* hb_apply() */

template <typename Appl>
ഘടന hb_apply_t
{
  hb_apply_t (Appl a) : a (a) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  ശൂന്യം operator () (Iter it)
  {
    വേണ്ടി (; it; ++it)
      (ശൂന്യം) hb_invoke (a, *it);
  }

  സ്വകാര്യ:
  Appl a;
};
ഘടന
{
  template <typename Appl> hb_apply_t<Appl>
  operator () (Appl&& a) സ്ഥിരം
  { തിരികെ hb_apply_t<Appl> (a); }

  template <typename Appl> hb_apply_t<Appl&>
  operator () (Appl *a) സ്ഥിരം
  { തിരികെ hb_apply_t<Appl&> (*a); }
}
HB_FUNCOBJ (hb_apply);

/* hb_range()/hb_iota()/hb_repeat() */

template <typename T, typename S>
ഘടന hb_range_iter_t :
  hb_iter_t<hb_range_iter_t<T, S>, T>
{
  hb_range_iter_t (T start, T end_, S step) : v (start), end_ (end_for (start, end_, step)), step (step) {}

  തരനിർവചനം T __item_t__;
  സ്ഥിതം constexpr തർക്കം is_random_access_iterator = സത്യം;
  സ്ഥിതം constexpr തർക്കം is_sorted_iterator = സത്യം;
  __item_t__ __item__ () സ്ഥിരം { തിരികെ hb_ridentity (v); }
  __item_t__ __item_at__ (unsigned j) സ്ഥിരം { തിരികെ v + j * step; }
  തർക്കം __more__ () സ്ഥിരം { തിരികെ v != end_; }
  unsigned __len__ () സ്ഥിരം { തിരികെ !step ? UINT_MAX : (end_ - v) / step; }
  ശൂന്യം __next__ () { v += step; }
  ശൂന്യം __forward__ (unsigned n) { v += n * step; }
  ശൂന്യം __prev__ () { v -= step; }
  ശൂന്യം __rewind__ (unsigned n) { v -= n * step; }
  hb_range_iter_t __end__ () സ്ഥിരം { തിരികെ hb_range_iter_t (end_, end_, step); }
  തർക്കം operator != (സ്ഥിരം hb_range_iter_t& o) സ്ഥിരം
  { തിരികെ v != o.v; }

  സ്വകാര്യ:
  സ്ഥിതം വരിയിൽ T end_for (T start, T end_, S step)
  {
    എങ്കിൽ (!step)
      തിരികെ end_;
    auto res = (end_ - start) % step;
    എങ്കിൽ (!res)
      തിരികെ end_;
    end_ += step - res;
    തിരികെ end_;
  }

  സ്വകാര്യ:
  T v;
  T end_;
  S step;
};
ഘടന
{
  template <typename T = unsigned> hb_range_iter_t<T, unsigned>
  operator () (T end = (unsigned) -1) സ്ഥിരം
  { തിരികെ hb_range_iter_t<T, unsigned> (0, end, 1u); }

  template <typename T, typename S = unsigned> hb_range_iter_t<T, S>
  operator () (T start, T end, S step = 1u) സ്ഥിരം
  { തിരികെ hb_range_iter_t<T, S> (start, end, step); }
}
HB_FUNCOBJ (hb_range);

template <typename T, typename S>
ഘടന hb_iota_iter_t :
  hb_iter_with_fallback_t<hb_iota_iter_t<T, S>, T>
{
  hb_iota_iter_t (T start, S step) : v (start), step (step) {}

  സ്വകാര്യ:

  template <typename S2 = S>
  auto
  inc (hb_type_identity<S2> s, hb_priority<1>)
    -> hb_void_t<decltype (hb_invoke (std::forward<hb_type_identity<S2>> (s),
                                      hb_declval<T&> ()))>
  { v = hb_invoke (std::forward<hb_type_identity<S2>> (s), v); }

  ശൂന്യം
  inc (S s, hb_priority<0>)
  { v += s; }

  പൊതു:

  തരനിർവചനം T __item_t__;
  സ്ഥിതം constexpr തർക്കം is_random_access_iterator = സത്യം;
  സ്ഥിതം constexpr തർക്കം is_sorted_iterator = സത്യം;
  __item_t__ __item__ () സ്ഥിരം { തിരികെ hb_ridentity (v); }
  തർക്കം __more__ () സ്ഥിരം { തിരികെ സത്യം; }
  unsigned __len__ () സ്ഥിരം { തിരികെ UINT_MAX; }
  ശൂന്യം __next__ () { inc (step, hb_prioritize); }
  ശൂന്യം __prev__ () { v -= step; }
  hb_iota_iter_t __end__ () സ്ഥിരം { തിരികെ *ഇത്; }
  തർക്കം operator != (സ്ഥിരം hb_iota_iter_t& o) സ്ഥിരം { തിരികെ സത്യം; }

  സ്വകാര്യ:
  T v;
  S step;
};
ഘടന
{
  template <typename T = unsigned, typename S = unsigned> hb_iota_iter_t<T, S>
  operator () (T start = 0u, S step = 1u) സ്ഥിരം
  { തിരികെ hb_iota_iter_t<T, S> (start, step); }
}
HB_FUNCOBJ (hb_iota);

template <typename T>
ഘടന hb_repeat_iter_t :
  hb_iter_t<hb_repeat_iter_t<T>, T>
{
  hb_repeat_iter_t (T value) : v (value) {}

  തരനിർവചനം T __item_t__;
  സ്ഥിതം constexpr തർക്കം is_random_access_iterator = സത്യം;
  സ്ഥിതം constexpr തർക്കം is_sorted_iterator = സത്യം;
  __item_t__ __item__ () സ്ഥിരം { തിരികെ v; }
  __item_t__ __item_at__ (unsigned j) സ്ഥിരം { തിരികെ v; }
  തർക്കം __more__ () സ്ഥിരം { തിരികെ സത്യം; }
  unsigned __len__ () സ്ഥിരം { തിരികെ UINT_MAX; }
  ശൂന്യം __next__ () {}
  ശൂന്യം __forward__ (unsigned) {}
  ശൂന്യം __prev__ () {}
  ശൂന്യം __rewind__ (unsigned) {}
  hb_repeat_iter_t __end__ () സ്ഥിരം { തിരികെ *ഇത്; }
  തർക്കം operator != (സ്ഥിരം hb_repeat_iter_t& o) സ്ഥിരം { തിരികെ സത്യം; }

  സ്വകാര്യ:
  T v;
};
ഘടന
{
  template <typename T> hb_repeat_iter_t<T>
  operator () (T value) സ്ഥിരം
  { തിരികെ hb_repeat_iter_t<T> (value); }
}
HB_FUNCOBJ (hb_repeat);

/* hb_enumerate()/hb_take() */

ഘടന
{
  template <typename Iterable,
	    typename Index = unsigned,
	    hb_requires (hb_is_iterable (Iterable))>
  auto operator () (Iterable&& it, Index start = 0u) സ്ഥിരം HB_AUTO_RETURN
  ( hb_zip (hb_iota (start), it) )
}
HB_FUNCOBJ (hb_enumerate);

ഘടന
{ HB_PARTIALIZE(2);
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  auto operator () (Iterable&& it, unsigned എണ്ണം) സ്ഥിരം HB_AUTO_RETURN
  ( hb_zip (hb_range (എണ്ണം), it) | hb_map_retains_sorting (hb_second) )

  /* Specialization arrays. */

  template <typename Type> വരിയിൽ hb_array_t<Type>
  operator () (hb_array_t<Type> array, unsigned എണ്ണം) സ്ഥിരം
  { തിരികെ array.sub_array (0, എണ്ണം); }

  template <typename Type> വരിയിൽ hb_sorted_array_t<Type>
  operator () (hb_sorted_array_t<Type> array, unsigned എണ്ണം) സ്ഥിരം
  { തിരികെ array.sub_array (0, എണ്ണം); }
}
HB_FUNCOBJ (hb_take);

ഘടന
{ HB_PARTIALIZE(2);
  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  auto operator () (Iter it, unsigned എണ്ണം) സ്ഥിരം HB_AUTO_RETURN
  (
    + hb_iota (it, hb_add (എണ്ണം))
    | hb_map (hb_take (എണ്ണം))
    | hb_take ((hb_len (it) + എണ്ണം - 1) / എണ്ണം)
  )
}
HB_FUNCOBJ (hb_chop);

/* hb_sink() */

template <typename Sink>
ഘടന hb_sink_t
{
  hb_sink_t (Sink s) : s (s) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  ശൂന്യം operator () (Iter it)
  {
    വേണ്ടി (; it; ++it)
      s << *it;
  }

  സ്വകാര്യ:
  Sink s;
};
ഘടന
{
  template <typename Sink> hb_sink_t<Sink>
  operator () (Sink&& s) സ്ഥിരം
  { തിരികെ hb_sink_t<Sink> (s); }

  template <typename Sink> hb_sink_t<Sink&>
  operator () (Sink *s) സ്ഥിരം
  { തിരികെ hb_sink_t<Sink&> (*s); }
}
HB_FUNCOBJ (hb_sink);

/* hb-drain: hb_sink to void / blackhole / /dev/null. */

ഘടന
{
  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  ശൂന്യം operator () (Iter it) സ്ഥിരം
  {
    വേണ്ടി (; it; ++it)
      (ശൂന്യം) *it;
  }
}
HB_FUNCOBJ (hb_drain);

/* hb_unzip(): unzip and sink to two sinks. */

template <typename Sink1, typename Sink2>
ഘടന hb_unzip_t
{
  hb_unzip_t (Sink1 s1, Sink2 s2) : s1 (s1), s2 (s2) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  ശൂന്യം operator () (Iter it)
  {
    വേണ്ടി (; it; ++it)
    {
      സ്ഥിരം auto &v = *it;
      s1 << v.first;
      s2 << v.second;
    }
  }

  സ്വകാര്യ:
  Sink1 s1;
  Sink2 s2;
};
ഘടന
{
  template <typename Sink1, typename Sink2> hb_unzip_t<Sink1, Sink2>
  operator () (Sink1&& s1, Sink2&& s2) സ്ഥിരം
  { തിരികെ hb_unzip_t<Sink1, Sink2> (s1, s2); }

  template <typename Sink1, typename Sink2> hb_unzip_t<Sink1&, Sink2&>
  operator () (Sink1 *s1, Sink2 *s2) സ്ഥിരം
  { തിരികെ hb_unzip_t<Sink1&, Sink2&> (*s1, *s2); }
}
HB_FUNCOBJ (hb_unzip);


/* hb-all, hb-any, hb-none. */

ഘടന
{
  template <typename Iterable,
	    typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity)),
	    hb_requires (hb_is_iterable (Iterable))>
  തർക്കം operator () (Iterable&& c,
		    Pred&& p = hb_identity,
		    Proj&& f = hb_identity) സ്ഥിരം
  {
    വേണ്ടി (auto it = hb_iter (c); it; ++it)
      എങ്കിൽ (!hb_match (p, hb_get (f, *it)))
	തിരികെ അസത്യം;
    തിരികെ സത്യം;
  }
}
HB_FUNCOBJ (hb_all);
ഘടന
{
  template <typename Iterable,
	    typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity)),
	    hb_requires (hb_is_iterable (Iterable))>
  തർക്കം operator () (Iterable&& c,
		    Pred&& p = hb_identity,
		    Proj&& f = hb_identity) സ്ഥിരം
  {
    വേണ്ടി (auto it = hb_iter (c); it; ++it)
      എങ്കിൽ (hb_match (p, hb_get (f, *it)))
	തിരികെ സത്യം;
    തിരികെ അസത്യം;
  }
}
HB_FUNCOBJ (hb_any);
ഘടന
{
  template <typename Iterable,
	    typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity)),
	    hb_requires (hb_is_iterable (Iterable))>
  തർക്കം operator () (Iterable&& c,
		    Pred&& p = hb_identity,
		    Proj&& f = hb_identity) സ്ഥിരം
  {
    വേണ്ടി (auto it = hb_iter (c); it; ++it)
      എങ്കിൽ (hb_match (p, hb_get (f, *it)))
	തിരികെ അസത്യം;
    തിരികെ സത്യം;
  }
}
HB_FUNCOBJ (hb_none);

/*
 * Algorithms operating on iterators.
 */

template <typename C, typename V,
	  hb_requires (hb_is_iterable (C))>
വരിയിൽ ശൂന്യം
hb_fill (C&& c, സ്ഥിരം V &v)
{
  വേണ്ടി (auto i = hb_iter (c); i; i++)
    *i = v;
}

template <typename S, typename D>
വരിയിൽ ശൂന്യം
hb_copy (S&& is, D&& id)
{
  hb_iter (is) | hb_sink (id);
}


#endif /* HB_ITER_HH */

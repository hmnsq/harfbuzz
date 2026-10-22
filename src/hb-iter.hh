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

#অন্তর্ভাৱ "hb.hh"
#অন্তর্ভাৱ "hb-algs.hh"
#অন্তর্ভাৱ "hb-meta.hh"


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
সংরচনা hb_iter_t
{
  প্রকারনির্দেশ Item item_t;
  constexpr unsigned get_item_size () ধ্রুৱ { ঘূৰাই hb_static_size (Item); }
  স্থির constexpr তার্কিক is_iterator = সঁচা;
  স্থির constexpr তার্কিক is_random_access_iterator = মিছা;
  স্থির constexpr তার্কিক is_sorted_iterator = মিছা;
  স্থির constexpr তার্কিক has_fast_len = মিছা; // Should be checked in combination with is_random_access_iterator.

  গুপ্ত:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ধ্রুৱ iter_t* thiz () ধ্রুৱ { ঘূৰাই static_cast<ধ্রুৱ iter_t *> (এতত্); }
	iter_t* thiz ()       { ঘূৰাই static_cast<      iter_t *> (এতত্); }
  সার্ৱজনিক:

  /* Operators. */
  iter_t iter () ধ্রুৱ { ঘূৰাই *thiz(); }
  iter_t operator + () ধ্রুৱ { ঘূৰাই *thiz(); }
  iter_t _begin () ধ্রুৱ { ঘূৰাই *thiz(); }
  iter_t begin () ধ্রুৱ { ঘূৰাই _begin (); }
  iter_t _end () ধ্রুৱ { ঘূৰাই thiz()->__end__ (); }
  iter_t end () ধ্রুৱ { ঘূৰাই _end (); }
  explicit operator তার্কিক () ধ্রুৱ { ঘূৰাই thiz()->__more__ (); }
  unsigned len () ধ্রুৱ { ঘূৰাই thiz()->__len__ (); }
  /* The following can only be enabled if item_t is reference type.  Otherwise
   * it will be returning pointer to temporary rvalue. */
  template <typename T = item_t,
	    hb_enable_if (std::is_reference<T>::value)>
  hb_remove_reference<item_t>* operator -> () ধ্রুৱ { ঘূৰাই std::addressof (**thiz()); }
  item_t operator * () ধ্রুৱ { ঘূৰাই thiz()->__item__ (); }
  item_t operator * () { ঘূৰাই thiz()->__item__ (); }
  item_t operator [] (unsigned i) ধ্রুৱ { ঘূৰাই thiz()->__item_at__ (i); }
  item_t operator [] (unsigned i) { ঘূৰাই thiz()->__item_at__ (i); }
  iter_t& operator += (unsigned গুনতি) &  { thiz()->__forward__ (গুনতি); ঘূৰাই *thiz(); }
  iter_t  operator += (unsigned গুনতি) && { thiz()->__forward__ (গুনতি); ঘূৰাই *thiz(); }
  iter_t& operator ++ () &  { thiz()->__next__ (); ঘূৰাই *thiz(); }
  iter_t  operator ++ () && { thiz()->__next__ (); ঘূৰাই *thiz(); }
  iter_t& operator -= (unsigned গুনতি) &  { thiz()->__rewind__ (গুনতি); ঘূৰাই *thiz(); }
  iter_t  operator -= (unsigned গুনতি) && { thiz()->__rewind__ (গুনতি); ঘূৰাই *thiz(); }
  iter_t& operator -- () &  { thiz()->__prev__ (); ঘূৰাই *thiz(); }
  iter_t  operator -- () && { thiz()->__prev__ (); ঘূৰাই *thiz(); }
  iter_t operator + (unsigned গুনতি) ধ্রুৱ { auto c = thiz()->iter (); c += গুনতি; ঘূৰাই c; }
  friend iter_t operator + (unsigned গুনতি, ধ্রুৱ iter_t &it) { ঘূৰাই it + গুনতি; }
  iter_t operator ++ (পূর্ণাঙ্ক) { iter_t c (*thiz()); ++*thiz(); ঘূৰাই c; }
  iter_t operator - (unsigned গুনতি) ধ্রুৱ { auto c = thiz()->iter (); c -= গুনতি; ঘূৰাই c; }
  iter_t operator -- (পূর্ণাঙ্ক) { iter_t c (*thiz()); --*thiz(); ঘূৰাই c; }
  template <typename T>
  iter_t& operator >> (T &v) &  { v = **thiz(); ++*thiz(); ঘূৰাই *thiz(); }
  template <typename T>
  iter_t  operator >> (T &v) && { v = **thiz(); ++*thiz(); ঘূৰাই *thiz(); }
  template <typename T>
  iter_t& operator << (ধ্রুৱ T v) &  { **thiz() = v; ++*thiz(); ঘূৰাই *thiz(); }
  template <typename T>
  iter_t  operator << (ধ্রুৱ T v) && { **thiz() = v; ++*thiz(); ঘূৰাই *thiz(); }

  রক্ষিত:
  hb_iter_t () = স্ৱভাৱ;
  hb_iter_t (ধ্রুৱ hb_iter_t &o HB_UNUSED) = স্ৱভাৱ;
  hb_iter_t (hb_iter_t &&o HB_UNUSED) = স্ৱভাৱ;
  hb_iter_t& operator = (ধ্রুৱ hb_iter_t &o HB_UNUSED) = স্ৱভাৱ;
  hb_iter_t& operator = (hb_iter_t &&o HB_UNUSED) = স্ৱভাৱ;
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
  using Name::operator তার্কিক; \
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
  static_assert (সঁচা, "")

/* Returns iterator / item type of a type. */
template <typename Iterable>
using hb_iter_type = decltype (hb_deref (hb_declval (Iterable)).iter ());
template <typename Iterable>
using hb_item_type = decltype (*hb_deref (hb_declval (Iterable)).iter ());


template <typename> সংরচনা hb_array_t;
template <typename> সংরচনা hb_sorted_array_t;

সংরচনা
{
  template <typename T> hb_iter_type<T>
  operator () (T&& c) ধ্রুৱ
  { ঘূৰাই hb_deref (std::forward<T> (c)).iter (); }

  /* Specialization for C arrays. */

  template <typename Type> অন্তঃস্থ hb_array_t<Type>
  operator () (Type *array, unsigned পূর্ণাঙ্ক length) ধ্রুৱ
  { ঘূৰাই hb_array_t<Type> (array, length); }

  template <typename Type, unsigned পূর্ণাঙ্ক length> hb_array_t<Type>
  operator () (Type (&array)[length]) ধ্রুৱ
  { ঘূৰাই hb_array_t<Type> (array, length); }

}
HB_FUNCOBJ (hb_iter);
সংরচনা
{
  template <typename T> auto
  impl (T&& c, hb_priority<1>) ধ্রুৱ HB_RETURN (unsigned, c.len ())

  template <typename T> auto
  impl (T&& c, hb_priority<0>) ধ্রুৱ HB_RETURN (unsigned, c.len)

  সার্ৱজনিক:

  template <typename T> auto
  operator () (T&& c) ধ্রুৱ HB_RETURN (unsigned, impl (std::forward<T> (c), hb_prioritize))
}
HB_FUNCOBJ (hb_len);

/* Mixin to fill in what the subclass doesn't provide. */
template <typename iter_t, typename item_t = typename iter_t::__item_t__>
সংরচনা hb_iter_fallback_mixin_t
{
  গুপ্ত:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ধ্রুৱ iter_t* thiz () ধ্রুৱ { ঘূৰাই static_cast<ধ্রুৱ iter_t *> (এতত্); }
	iter_t* thiz ()       { ঘূৰাই static_cast<      iter_t *> (এতত্); }
  সার্ৱজনিক:

  /* Access: Implement __item__(), or __item_at__() if random-access. */
  item_t __item__ () ধ্রুৱ { ঘূৰাই (*thiz())[0]; }
  item_t __item_at__ (unsigned i) ধ্রুৱ { ঘূৰাই *(*thiz() + i); }

  /* Termination: Implement __more__(), or __len__() if random-access. */
  তার্কিক __more__ () ধ্রুৱ { ঘূৰাই তার্কিক (thiz()->len ()); }
  unsigned __len__ () ধ্রুৱ
  { iter_t c (*thiz()); unsigned l = 0; যেতিয়ালৈ (c) { c++; l++; } ঘূৰাই l; }

  /* Advancing: Implement __next__(), or __forward__() if random-access. */
  শূন্য __next__ () { *thiz() += 1; }
  শূন্য __forward__ (unsigned n) { যেতিয়ালৈ (*thiz() && n--) ++*thiz(); }

  /* Rewinding: Implement __prev__() or __rewind__() if bidirectional. */
  শূন্য __prev__ () { *thiz() -= 1; }
  শূন্য __rewind__ (unsigned n) { যেতিয়ালৈ (*thiz() && n--) --*thiz(); }

  /* Range-based for: Implement __end__() if can be done faster,
   * and operator!=. */
  iter_t __end__ () ধ্রুৱ
  {
    যদি (thiz()->is_random_access_iterator)
      ঘূৰাই *thiz() + thiz()->len ();
    /* Above expression loops twice. Following loops once. */
    auto it = *thiz();
    যেতিয়ালৈ (it) ++it;
    ঘূৰাই it;
  }

  রক্ষিত:
  hb_iter_fallback_mixin_t () = স্ৱভাৱ;
  hb_iter_fallback_mixin_t (ধ্রুৱ hb_iter_fallback_mixin_t &o HB_UNUSED) = স্ৱভাৱ;
  hb_iter_fallback_mixin_t (hb_iter_fallback_mixin_t &&o HB_UNUSED) = স্ৱভাৱ;
  hb_iter_fallback_mixin_t& operator = (ধ্রুৱ hb_iter_fallback_mixin_t &o HB_UNUSED) = স্ৱভাৱ;
  hb_iter_fallback_mixin_t& operator = (hb_iter_fallback_mixin_t &&o HB_UNUSED) = স্ৱভাৱ;
};

template <typename iter_t, typename item_t = typename iter_t::__item_t__>
সংরচনা hb_iter_with_fallback_t :
  hb_iter_t<iter_t, item_t>,
  hb_iter_fallback_mixin_t<iter_t, item_t>
{
  রক্ষিত:
  hb_iter_with_fallback_t () = স্ৱভাৱ;
  hb_iter_with_fallback_t (ধ্রুৱ hb_iter_with_fallback_t &o HB_UNUSED) = স্ৱভাৱ;
  hb_iter_with_fallback_t (hb_iter_with_fallback_t &&o HB_UNUSED) = স্ৱভাৱ;
  hb_iter_with_fallback_t& operator = (ধ্রুৱ hb_iter_with_fallback_t &o HB_UNUSED) = স্ৱভাৱ;
  hb_iter_with_fallback_t& operator = (hb_iter_with_fallback_t &&o HB_UNUSED) = স্ৱভাৱ;
};

/*
 * Meta-programming predicates.
 */

/* hb_is_iterator() / hb_is_iterator_of() */

template<typename Iter, typename Item>
সংরচনা hb_is_iterator_of
{
  template <typename Item2 = Item>
  স্থির hb_true_type impl (hb_priority<2>, hb_iter_t<Iter, hb_type_identity<Item2>> *);
  স্থির hb_false_type impl (hb_priority<0>, ধ্রুৱ শূন্য *);

  সার্ৱজনিক:
  স্থির constexpr তার্কিক value = decltype (impl (hb_prioritize, hb_declval (Iter*)))::value;
};
#define hb_is_iterator_of(Iter, Item) hb_is_iterator_of<Iter, Item>::value
#define hb_is_iterator(Iter) hb_is_iterator_of (Iter, typename Iter::item_t)
#define hb_is_sorted_iterator_of(Iter, Item) (hb_is_iterator_of<Iter, Item>::value && Iter::is_sorted_iterator)
#define hb_is_sorted_iterator(Iter) hb_is_sorted_iterator_of (Iter, typename Iter::item_t)

/* hb_is_iterable() */

template <typename T>
সংরচনা hb_is_iterable
{
  গুপ্ত:

  template <typename U>
  স্থির auto impl (hb_priority<1>) -> decltype (hb_declval (U).iter (), hb_true_type ());

  template <typename>
  স্থির hb_false_type impl (hb_priority<0>);

  সার্ৱজনিক:
  স্থির constexpr তার্কিক value = decltype (impl<T> (hb_prioritize))::value;
};
#define hb_is_iterable(Iterable) hb_is_iterable<Iterable>::value

/* hb_is_source_of() / hb_is_sink_of() */

template<typename Iter, typename Item>
সংরচনা hb_is_source_of
{
  গুপ্ত:
  template <typename Iter2 = Iter,
	    hb_enable_if (hb_is_convertible (typename Iter2::item_t, hb_add_lvalue_reference<ধ্রুৱ Item>))>
  স্থির hb_true_type impl (hb_priority<2>);
  template <typename Iter2 = Iter>
  স্থির auto impl (hb_priority<1>) -> decltype (hb_declval (Iter2) >> hb_declval (Item &), hb_true_type ());
  স্থির hb_false_type impl (hb_priority<0>);

  সার্ৱজনিক:
  স্থির constexpr তার্কিক value = decltype (impl (hb_prioritize))::value;
};
#define hb_is_source_of(Iter, Item) hb_is_source_of<Iter, Item>::value

template<typename Iter, typename Item>
সংরচনা hb_is_sink_of
{
  গুপ্ত:
  template <typename Iter2 = Iter,
	    hb_enable_if (hb_is_convertible (typename Iter2::item_t, hb_add_lvalue_reference<Item>))>
  স্থির hb_true_type impl (hb_priority<2>);
  template <typename Iter2 = Iter>
  স্থির auto impl (hb_priority<1>) -> decltype (hb_declval (Iter2) << hb_declval (Item), hb_true_type ());
  স্থির hb_false_type impl (hb_priority<0>);

  সার্ৱজনিক:
  স্থির constexpr তার্কিক value = decltype (impl (hb_prioritize))::value;
};
#define hb_is_sink_of(Iter, Item) hb_is_sink_of<Iter, Item>::value

/* This is commonly used, so define: */
#define hb_is_sorted_source_of(Iter, Item) \
	(hb_is_source_of(Iter, Item) && Iter::is_sorted_iterator)


সংরচনা
{
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  unsigned operator () (ধ্রুৱ Iterable &_) ধ্রুৱ { ঘূৰাই hb_len (hb_iter (_)); }

  unsigned operator () (unsigned _) ধ্রুৱ { ঘূৰাই _; }
}
HB_FUNCOBJ (hb_len_of);

/* Range-based 'for' for iterables. */

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
স্থির অন্তঃস্থ auto begin (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).begin ())

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
স্থির অন্তঃস্থ auto end (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).end ())

/* begin()/end() are NOT looked up non-ADL.  So each namespace must declare them.
 * Do it for namespace OT. */
নামাকাশ OT {

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
স্থির অন্তঃস্থ auto begin (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).begin ())

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
স্থির অন্তঃস্থ auto end (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).end ())

}


/*
 * Adaptors, combiners, etc.
 */

template <typename Lhs, typename Rhs,
	  hb_requires (hb_is_iterator (Lhs))>
স্থির অন্তঃস্থ auto
operator | (Lhs&& lhs, Rhs&& rhs) HB_AUTO_RETURN (std::forward<Rhs> (rhs) (std::forward<Lhs> (lhs)))

/* hb_map(), hb_filter(), hb_reduce() */

গণনা  শ্ৰেণী hb_function_sortedness_t {
  NOT_SORTED,
  RETAINS_SORTING,
  SORTED,
};

template <typename Iter, typename Proj, hb_function_sortedness_t Sorted,
	 hb_requires (hb_is_iterator (Iter))>
সংরচনা hb_map_iter_t :
  hb_iter_t<hb_map_iter_t<Iter, Proj, Sorted>,
	    decltype (hb_get (hb_declval (Proj), *hb_declval (Iter)))>
{
  hb_map_iter_t (ধ্রুৱ Iter& it, Proj f_) : it (it), f (f_) {}

  প্রকারনির্দেশ decltype (hb_get (hb_declval (Proj), *hb_declval (Iter))) __item_t__;
  স্থির constexpr তার্কিক is_random_access_iterator = Iter::is_random_access_iterator;
  স্থির constexpr তার্কিক is_sorted_iterator =
    Sorted == hb_function_sortedness_t::SORTED ? সঁচা :
    Sorted == hb_function_sortedness_t::RETAINS_SORTING ? Iter::is_sorted_iterator :
    মিছা;
  __item_t__ __item__ () ধ্রুৱ { ঘূৰাই hb_get (f.get (), *it); }
  __item_t__ __item_at__ (unsigned i) ধ্রুৱ { ঘূৰাই hb_get (f.get (), it[i]); }
  তার্কিক __more__ () ধ্রুৱ { ঘূৰাই তার্কিক (it); }
  unsigned __len__ () ধ্রুৱ { ঘূৰাই it.len (); }
  শূন্য __next__ () { ++it; }
  শূন্য __forward__ (unsigned n) { it += n; }
  শূন্য __prev__ () { --it; }
  শূন্য __rewind__ (unsigned n) { it -= n; }
  hb_map_iter_t __end__ () ধ্রুৱ { ঘূৰাই hb_map_iter_t (it._end (), f); }
  তার্কিক operator != (ধ্রুৱ hb_map_iter_t& o) ধ্রুৱ
  { ঘূৰাই it != o.it; }

  গুপ্ত:
  Iter it;
  mutable hb_reference_wrapper<Proj> f;
};

template <typename Proj, hb_function_sortedness_t Sorted>
সংরচনা hb_map_iter_factory_t
{
  hb_map_iter_factory_t (Proj f) : f (f) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  hb_map_iter_t<Iter, Proj, Sorted>
  operator () (Iter it)
  { ঘূৰাই hb_map_iter_t<Iter, Proj, Sorted> (it, f); }

  গুপ্ত:
  Proj f;
};
সংরচনা
{
  template <typename Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::NOT_SORTED>
  operator () (Proj&& f) ধ্রুৱ
  { ঘূৰাই hb_map_iter_factory_t<Proj, hb_function_sortedness_t::NOT_SORTED> (f); }
}
HB_FUNCOBJ (hb_map);
সংরচনা
{
  template <typename Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::RETAINS_SORTING>
  operator () (Proj&& f) ধ্রুৱ
  { ঘূৰাই hb_map_iter_factory_t<Proj, hb_function_sortedness_t::RETAINS_SORTING> (f); }
}
HB_FUNCOBJ (hb_map_retains_sorting);
সংরচনা
{
  template <typename Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::SORTED>
  operator () (Proj&& f) ধ্রুৱ
  { ঘূৰাই hb_map_iter_factory_t<Proj, hb_function_sortedness_t::SORTED> (f); }
}
HB_FUNCOBJ (hb_map_sorted);

template <typename Iter, typename Pred, typename Proj,
	 hb_requires (hb_is_iterator (Iter))>
সংরচনা hb_filter_iter_t :
  hb_iter_with_fallback_t<hb_filter_iter_t<Iter, Pred, Proj>,
			  typename Iter::item_t>
{
  hb_filter_iter_t (ধ্রুৱ Iter& it_, Pred p_, Proj f_) : it (it_), p (p_), f (f_)
  { যেতিয়ালৈ (it && !hb_has (p.get (), hb_get (f.get (), *it))) ++it; }

  প্রকারনির্দেশ typename Iter::item_t __item_t__;
  স্থির constexpr তার্কিক is_sorted_iterator = Iter::is_sorted_iterator;
  __item_t__ __item__ () ধ্রুৱ { ঘূৰাই *it; }
  তার্কিক __more__ () ধ্রুৱ { ঘূৰাই তার্কিক (it); }
  শূন্য __next__ () { কৰা ++it; যেতিয়ালৈ (it && !hb_has (p.get (), hb_get (f.get (), *it))); }
  শূন্য __prev__ ()
  {
    unsigned গুনতি = 0;
    কৰা
    {
      Iter old = it;
      --it;
      যদি (!(it != old))
      {
	it += গুনতি;
	ঘূৰাই;
      }
      গুনতি++;
    }
    যেতিয়ালৈ (!hb_has (p.get (), hb_get (f.get (), *it)));
  }
  hb_filter_iter_t __end__ () ধ্রুৱ { ঘূৰাই hb_filter_iter_t (it._end (), p, f); }
  তার্কিক operator != (ধ্রুৱ hb_filter_iter_t& o) ধ্রুৱ
  { ঘূৰাই it != o.it; }

  গুপ্ত:
  Iter it;
  mutable hb_reference_wrapper<Pred> p;
  mutable hb_reference_wrapper<Proj> f;
};
template <typename Pred, typename Proj>
সংরচনা hb_filter_iter_factory_t
{
  hb_filter_iter_factory_t (Pred p, Proj f) : p (p), f (f) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  hb_filter_iter_t<Iter, Pred, Proj>
  operator () (Iter it)
  { ঘূৰাই hb_filter_iter_t<Iter, Pred, Proj> (it, p, f); }

  গুপ্ত:
  Pred p;
  Proj f;
};
সংরচনা
{
  template <typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity))>
  hb_filter_iter_factory_t<Pred, Proj>
  operator () (Pred&& p = hb_identity, Proj&& f = hb_identity) ধ্রুৱ
  { ঘূৰাই hb_filter_iter_factory_t<Pred, Proj> (p, f); }
}
HB_FUNCOBJ (hb_filter);

template <typename Redu, typename InitT>
সংরচনা hb_reduce_t
{
  hb_reduce_t (Redu r, InitT init_মান) : r (r), init_মান (init_মান) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter)),
	    typename AccuT = hb_decay<decltype (hb_declval (Redu) (hb_declval (InitT), hb_declval (typename Iter::item_t)))>>
  AccuT
  operator () (Iter it)
  {
    AccuT value = init_মান;
    বাবে (; it; ++it)
      value = r (value, *it);
    ঘূৰাই value;
  }

  গুপ্ত:
  Redu r;
  InitT init_মান;
};
সংরচনা
{
  template <typename Redu, typename InitT>
  hb_reduce_t<Redu, InitT>
  operator () (Redu&& r, InitT init_মান) ধ্রুৱ
  { ঘূৰাই hb_reduce_t<Redu, InitT> (r, init_মান); }
}
HB_FUNCOBJ (hb_reduce);


/* hb_zip() */

template <typename A, typename B>
সংরচনা hb_zip_iter_t :
  hb_iter_t<hb_zip_iter_t<A, B>,
	    hb_pair_t<typename A::item_t, typename B::item_t>>
{
  hb_zip_iter_t () {}
  hb_zip_iter_t (ধ্রুৱ A& a, ধ্রুৱ B& b) : a (a), b (b) {}

  প্রকারনির্দেশ hb_pair_t<typename A::item_t, typename B::item_t> __item_t__;
  স্থির constexpr তার্কিক is_random_access_iterator =
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
  স্থির constexpr তার্কিক is_sorted_iterator = A::is_sorted_iterator;

  __item_t__ __item__ () ধ্রুৱ { ঘূৰাই __item_t__ (*a, *b); }
  __item_t__ __item_at__ (unsigned i) ধ্রুৱ { ঘূৰাই __item_t__ (a[i], b[i]); }
  তার্কিক __more__ () ধ্রুৱ { ঘূৰাই তার্কিক (a) && তার্কিক (b); }
  unsigned __len__ () ধ্রুৱ { ঘূৰাই hb_min (a.len (), b.len ()); }
  শূন্য __next__ () { ++a; ++b; }
  শূন্য __forward__ (unsigned n) { a += n; b += n; }
  শূন্য __prev__ () { --a; --b; }
  শূন্য __rewind__ (unsigned n) { a -= n; b -= n; }
  hb_zip_iter_t __end__ () ধ্রুৱ { ঘূৰাই hb_zip_iter_t (a._end (), b._end ()); }
  /* Note, we should stop if ANY of the iters reaches end.  As such two compare
   * unequal if both items are unequal, NOT if either is unequal. */
  তার্কিক operator != (ধ্রুৱ hb_zip_iter_t& o) ধ্রুৱ
  { ঘূৰাই a != o.a && b != o.b; }

  গুপ্ত:
  A a;
  B b;
};
সংরচনা
{ HB_PARTIALIZE(2);
  template <typename A, typename B,
	    hb_requires (hb_is_iterable (A) && hb_is_iterable (B))>
  hb_zip_iter_t<hb_iter_type<A>, hb_iter_type<B>>
  operator () (A&& a, B&& b) ধ্রুৱ
  { ঘূৰাই hb_zip_iter_t<hb_iter_type<A>, hb_iter_type<B>> (hb_iter (a), hb_iter (b)); }
}
HB_FUNCOBJ (hb_zip);

/* hb_concat() */

template <typename A, typename B>
সংরচনা hb_concat_iter_t :
    hb_iter_t<hb_concat_iter_t<A, B>, typename A::item_t>
{
  hb_concat_iter_t () {}
  hb_concat_iter_t (A& a, B& b) : a (a), b (b) {}
  hb_concat_iter_t (ধ্রুৱ A& a, ধ্রুৱ B& b) : a (a), b (b) {}


  প্রকারনির্দেশ typename A::item_t __item_t__;
  স্থির constexpr তার্কিক is_random_access_iterator =
    A::is_random_access_iterator &&
    B::is_random_access_iterator;
  স্থির constexpr তার্কিক is_sorted_iterator = মিছা;

  __item_t__ __item__ () ধ্রুৱ
  {
    যদি (!a)
      ঘূৰাই *b;
    ঘূৰাই *a;
  }

  __item_t__ __item_at__ (unsigned i) ধ্রুৱ
  {
    unsigned a_len = a.len ();
    যদি (i < a_len)
      ঘূৰাই a[i];
    ঘূৰাই b[i - a_len];
  }

  তার্কিক __more__ () ধ্রুৱ { ঘূৰাই তার্কিক (a) || তার্কিক (b); }

  unsigned __len__ () ধ্রুৱ { ঘূৰাই a.len () + b.len (); }

  শূন্য __next__ ()
  {
    যদি (a)
      ++a;
    অন্যথা
      ++b;
  }

  শূন্য __forward__ (unsigned n)
  {
    যদি (!n) ঘূৰাই;
    যদি (!is_random_access_iterator) {
      যেতিয়ালৈ (n-- && *এতত্) {
        (*এতত্)++;
      }
      ঘূৰাই;
    }

    unsigned a_len = a.len ();
    যদি (n > a_len) {
      n -= a_len;
      a.__forward__ (a_len);
      b.__forward__ (n);
    } অন্যথা {
      a.__forward__ (n);
    }
  }

  hb_concat_iter_t __end__ () ধ্রুৱ { ঘূৰাই hb_concat_iter_t (a._end (), b._end ()); }
  তার্কিক operator != (ধ্রুৱ hb_concat_iter_t& o) ধ্রুৱ
  {
    ঘূৰাই a != o.a
        || b != o.b;
  }

  গুপ্ত:
  A a;
  B b;
};
সংরচনা
{ HB_PARTIALIZE(2);
  template <typename A, typename B,
	    hb_requires (hb_is_iterable (A) && hb_is_iterable (B))>
  hb_concat_iter_t<hb_iter_type<A>, hb_iter_type<B>>
  operator () (A&& a, B&& b) ধ্রুৱ
  { ঘূৰাই hb_concat_iter_t<hb_iter_type<A>, hb_iter_type<B>> (hb_iter (a), hb_iter (b)); }
}
HB_FUNCOBJ (hb_concat);

/* hb_apply() */

template <typename Appl>
সংরচনা hb_apply_t
{
  hb_apply_t (Appl a) : a (a) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  শূন্য operator () (Iter it)
  {
    বাবে (; it; ++it)
      (শূন্য) hb_invoke (a, *it);
  }

  গুপ্ত:
  Appl a;
};
সংরচনা
{
  template <typename Appl> hb_apply_t<Appl>
  operator () (Appl&& a) ধ্রুৱ
  { ঘূৰাই hb_apply_t<Appl> (a); }

  template <typename Appl> hb_apply_t<Appl&>
  operator () (Appl *a) ধ্রুৱ
  { ঘূৰাই hb_apply_t<Appl&> (*a); }
}
HB_FUNCOBJ (hb_apply);

/* hb_range()/hb_iota()/hb_repeat() */

template <typename T, typename S>
সংরচনা hb_range_iter_t :
  hb_iter_t<hb_range_iter_t<T, S>, T>
{
  hb_range_iter_t (T start, T end_, S step) : v (start), end_ (end_for (start, end_, step)), step (step) {}

  প্রকারনির্দেশ T __item_t__;
  স্থির constexpr তার্কিক is_random_access_iterator = সঁচা;
  স্থির constexpr তার্কিক is_sorted_iterator = সঁচা;
  __item_t__ __item__ () ধ্রুৱ { ঘূৰাই hb_ridentity (v); }
  __item_t__ __item_at__ (unsigned j) ধ্রুৱ { ঘূৰাই v + j * step; }
  তার্কিক __more__ () ধ্রুৱ { ঘূৰাই v != end_; }
  unsigned __len__ () ধ্রুৱ { ঘূৰাই !step ? UINT_MAX : (end_ - v) / step; }
  শূন্য __next__ () { v += step; }
  শূন্য __forward__ (unsigned n) { v += n * step; }
  শূন্য __prev__ () { v -= step; }
  শূন্য __rewind__ (unsigned n) { v -= n * step; }
  hb_range_iter_t __end__ () ধ্রুৱ { ঘূৰাই hb_range_iter_t (end_, end_, step); }
  তার্কিক operator != (ধ্রুৱ hb_range_iter_t& o) ধ্রুৱ
  { ঘূৰাই v != o.v; }

  গুপ্ত:
  স্থির অন্তঃস্থ T end_for (T start, T end_, S step)
  {
    যদি (!step)
      ঘূৰাই end_;
    auto res = (end_ - start) % step;
    যদি (!res)
      ঘূৰাই end_;
    end_ += step - res;
    ঘূৰাই end_;
  }

  গুপ্ত:
  T v;
  T end_;
  S step;
};
সংরচনা
{
  template <typename T = unsigned> hb_range_iter_t<T, unsigned>
  operator () (T end = (unsigned) -1) ধ্রুৱ
  { ঘূৰাই hb_range_iter_t<T, unsigned> (0, end, 1u); }

  template <typename T, typename S = unsigned> hb_range_iter_t<T, S>
  operator () (T start, T end, S step = 1u) ধ্রুৱ
  { ঘূৰাই hb_range_iter_t<T, S> (start, end, step); }
}
HB_FUNCOBJ (hb_range);

template <typename T, typename S>
সংরচনা hb_iota_iter_t :
  hb_iter_with_fallback_t<hb_iota_iter_t<T, S>, T>
{
  hb_iota_iter_t (T start, S step) : v (start), step (step) {}

  গুপ্ত:

  template <typename S2 = S>
  auto
  inc (hb_type_identity<S2> s, hb_priority<1>)
    -> hb_void_t<decltype (hb_invoke (std::forward<hb_type_identity<S2>> (s),
                                      hb_declval<T&> ()))>
  { v = hb_invoke (std::forward<hb_type_identity<S2>> (s), v); }

  শূন্য
  inc (S s, hb_priority<0>)
  { v += s; }

  সার্ৱজনিক:

  প্রকারনির্দেশ T __item_t__;
  স্থির constexpr তার্কিক is_random_access_iterator = সঁচা;
  স্থির constexpr তার্কিক is_sorted_iterator = সঁচা;
  __item_t__ __item__ () ধ্রুৱ { ঘূৰাই hb_ridentity (v); }
  তার্কিক __more__ () ধ্রুৱ { ঘূৰাই সঁচা; }
  unsigned __len__ () ধ্রুৱ { ঘূৰাই UINT_MAX; }
  শূন্য __next__ () { inc (step, hb_prioritize); }
  শূন্য __prev__ () { v -= step; }
  hb_iota_iter_t __end__ () ধ্রুৱ { ঘূৰাই *এতত্; }
  তার্কিক operator != (ধ্রুৱ hb_iota_iter_t& o) ধ্রুৱ { ঘূৰাই সঁচা; }

  গুপ্ত:
  T v;
  S step;
};
সংরচনা
{
  template <typename T = unsigned, typename S = unsigned> hb_iota_iter_t<T, S>
  operator () (T start = 0u, S step = 1u) ধ্রুৱ
  { ঘূৰাই hb_iota_iter_t<T, S> (start, step); }
}
HB_FUNCOBJ (hb_iota);

template <typename T>
সংরচনা hb_repeat_iter_t :
  hb_iter_t<hb_repeat_iter_t<T>, T>
{
  hb_repeat_iter_t (T value) : v (value) {}

  প্রকারনির্দেশ T __item_t__;
  স্থির constexpr তার্কিক is_random_access_iterator = সঁচা;
  স্থির constexpr তার্কিক is_sorted_iterator = সঁচা;
  __item_t__ __item__ () ধ্রুৱ { ঘূৰাই v; }
  __item_t__ __item_at__ (unsigned j) ধ্রুৱ { ঘূৰাই v; }
  তার্কিক __more__ () ধ্রুৱ { ঘূৰাই সঁচা; }
  unsigned __len__ () ধ্রুৱ { ঘূৰাই UINT_MAX; }
  শূন্য __next__ () {}
  শূন্য __forward__ (unsigned) {}
  শূন্য __prev__ () {}
  শূন্য __rewind__ (unsigned) {}
  hb_repeat_iter_t __end__ () ধ্রুৱ { ঘূৰাই *এতত্; }
  তার্কিক operator != (ধ্রুৱ hb_repeat_iter_t& o) ধ্রুৱ { ঘূৰাই সঁচা; }

  গুপ্ত:
  T v;
};
সংরচনা
{
  template <typename T> hb_repeat_iter_t<T>
  operator () (T value) ধ্রুৱ
  { ঘূৰাই hb_repeat_iter_t<T> (value); }
}
HB_FUNCOBJ (hb_repeat);

/* hb_enumerate()/hb_take() */

সংরচনা
{
  template <typename Iterable,
	    typename Index = unsigned,
	    hb_requires (hb_is_iterable (Iterable))>
  auto operator () (Iterable&& it, Index start = 0u) ধ্রুৱ HB_AUTO_RETURN
  ( hb_zip (hb_iota (start), it) )
}
HB_FUNCOBJ (hb_enumerate);

সংরচনা
{ HB_PARTIALIZE(2);
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  auto operator () (Iterable&& it, unsigned গুনতি) ধ্রুৱ HB_AUTO_RETURN
  ( hb_zip (hb_range (গুনতি), it) | hb_map_retains_sorting (hb_second) )

  /* Specialization arrays. */

  template <typename Type> অন্তঃস্থ hb_array_t<Type>
  operator () (hb_array_t<Type> array, unsigned গুনতি) ধ্রুৱ
  { ঘূৰাই array.sub_array (0, গুনতি); }

  template <typename Type> অন্তঃস্থ hb_sorted_array_t<Type>
  operator () (hb_sorted_array_t<Type> array, unsigned গুনতি) ধ্রুৱ
  { ঘূৰাই array.sub_array (0, গুনতি); }
}
HB_FUNCOBJ (hb_take);

সংরচনা
{ HB_PARTIALIZE(2);
  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  auto operator () (Iter it, unsigned গুনতি) ধ্রুৱ HB_AUTO_RETURN
  (
    + hb_iota (it, hb_add (গুনতি))
    | hb_map (hb_take (গুনতি))
    | hb_take ((hb_len (it) + গুনতি - 1) / গুনতি)
  )
}
HB_FUNCOBJ (hb_chop);

/* hb_sink() */

template <typename Sink>
সংরচনা hb_sink_t
{
  hb_sink_t (Sink s) : s (s) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  শূন্য operator () (Iter it)
  {
    বাবে (; it; ++it)
      s << *it;
  }

  গুপ্ত:
  Sink s;
};
সংরচনা
{
  template <typename Sink> hb_sink_t<Sink>
  operator () (Sink&& s) ধ্রুৱ
  { ঘূৰাই hb_sink_t<Sink> (s); }

  template <typename Sink> hb_sink_t<Sink&>
  operator () (Sink *s) ধ্রুৱ
  { ঘূৰাই hb_sink_t<Sink&> (*s); }
}
HB_FUNCOBJ (hb_sink);

/* hb-drain: hb_sink to void / blackhole / /dev/null. */

সংরচনা
{
  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  শূন্য operator () (Iter it) ধ্রুৱ
  {
    বাবে (; it; ++it)
      (শূন্য) *it;
  }
}
HB_FUNCOBJ (hb_drain);

/* hb_unzip(): unzip and sink to two sinks. */

template <typename Sink1, typename Sink2>
সংরচনা hb_unzip_t
{
  hb_unzip_t (Sink1 s1, Sink2 s2) : s1 (s1), s2 (s2) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  শূন্য operator () (Iter it)
  {
    বাবে (; it; ++it)
    {
      ধ্রুৱ auto &v = *it;
      s1 << v.first;
      s2 << v.second;
    }
  }

  গুপ্ত:
  Sink1 s1;
  Sink2 s2;
};
সংরচনা
{
  template <typename Sink1, typename Sink2> hb_unzip_t<Sink1, Sink2>
  operator () (Sink1&& s1, Sink2&& s2) ধ্রুৱ
  { ঘূৰাই hb_unzip_t<Sink1, Sink2> (s1, s2); }

  template <typename Sink1, typename Sink2> hb_unzip_t<Sink1&, Sink2&>
  operator () (Sink1 *s1, Sink2 *s2) ধ্রুৱ
  { ঘূৰাই hb_unzip_t<Sink1&, Sink2&> (*s1, *s2); }
}
HB_FUNCOBJ (hb_unzip);


/* hb-all, hb-any, hb-none. */

সংরচনা
{
  template <typename Iterable,
	    typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity)),
	    hb_requires (hb_is_iterable (Iterable))>
  তার্কিক operator () (Iterable&& c,
		    Pred&& p = hb_identity,
		    Proj&& f = hb_identity) ধ্রুৱ
  {
    বাবে (auto it = hb_iter (c); it; ++it)
      যদি (!hb_match (p, hb_get (f, *it)))
	ঘূৰাই মিছা;
    ঘূৰাই সঁচা;
  }
}
HB_FUNCOBJ (hb_all);
সংরচনা
{
  template <typename Iterable,
	    typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity)),
	    hb_requires (hb_is_iterable (Iterable))>
  তার্কিক operator () (Iterable&& c,
		    Pred&& p = hb_identity,
		    Proj&& f = hb_identity) ধ্রুৱ
  {
    বাবে (auto it = hb_iter (c); it; ++it)
      যদি (hb_match (p, hb_get (f, *it)))
	ঘূৰাই সঁচা;
    ঘূৰাই মিছা;
  }
}
HB_FUNCOBJ (hb_any);
সংরচনা
{
  template <typename Iterable,
	    typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity)),
	    hb_requires (hb_is_iterable (Iterable))>
  তার্কিক operator () (Iterable&& c,
		    Pred&& p = hb_identity,
		    Proj&& f = hb_identity) ধ্রুৱ
  {
    বাবে (auto it = hb_iter (c); it; ++it)
      যদি (hb_match (p, hb_get (f, *it)))
	ঘূৰাই মিছা;
    ঘূৰাই সঁচা;
  }
}
HB_FUNCOBJ (hb_none);

/*
 * Algorithms operating on iterators.
 */

template <typename C, typename V,
	  hb_requires (hb_is_iterable (C))>
অন্তঃস্থ শূন্য
hb_fill (C&& c, ধ্রুৱ V &v)
{
  বাবে (auto i = hb_iter (c); i; i++)
    *i = v;
}

template <typename S, typename D>
অন্তঃস্থ শূন্য
hb_copy (S&& is, D&& id)
{
  hb_iter (is) | hb_sink (id);
}


#endif /* HB_ITER_HH */

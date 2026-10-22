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

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-algs.hh"
#ಅನ್ತರ್ಭಾವ "hb-meta.hh"


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
ಸಂರಚನಾ hb_iter_t
{
  ಪ್ರಕಾರನಿರ್ದೇಶ Item item_t;
  constexpr unsigned get_item_size () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ hb_static_size (Item); }
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ is_iterator = ಸತ್ಯ;
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ is_random_access_iterator = ಅಸತ್ಯ;
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ is_sorted_iterator = ಅಸತ್ಯ;
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ has_fast_len = ಅಸತ್ಯ; // Should be checked in combination with is_random_access_iterator.

  ಗುಪ್ತ:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ಧ್ರುವ iter_t* thiz () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ static_cast<ಧ್ರುವ iter_t *> (ಏತತ್); }
	iter_t* thiz ()       { ಪ್ರತ್ಯಾಗಮ static_cast<      iter_t *> (ಏತತ್); }
  ಸಾರ್ವಜನಿಕ:

  /* Operators. */
  iter_t iter () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ *thiz(); }
  iter_t operator + () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ *thiz(); }
  iter_t _begin () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ *thiz(); }
  iter_t begin () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ _begin (); }
  iter_t _end () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ thiz()->__end__ (); }
  iter_t end () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ _end (); }
  explicit operator ತಾರ್ಕಿಕ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ thiz()->__more__ (); }
  unsigned len () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ thiz()->__len__ (); }
  /* The following can only be enabled if item_t is reference type.  Otherwise
   * it will be returning pointer to temporary rvalue. */
  template <typename T = item_t,
	    hb_enable_if (std::is_reference<T>::value)>
  hb_remove_reference<item_t>* operator -> () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ std::addressof (**thiz()); }
  item_t operator * () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ thiz()->__item__ (); }
  item_t operator * () { ಪ್ರತ್ಯಾಗಮ thiz()->__item__ (); }
  item_t operator [] (unsigned i) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ thiz()->__item_at__ (i); }
  item_t operator [] (unsigned i) { ಪ್ರತ್ಯಾಗಮ thiz()->__item_at__ (i); }
  iter_t& operator += (unsigned ಎಣಿಕೆ) &  { thiz()->__forward__ (ಎಣಿಕೆ); ಪ್ರತ್ಯಾಗಮ *thiz(); }
  iter_t  operator += (unsigned ಎಣಿಕೆ) && { thiz()->__forward__ (ಎಣಿಕೆ); ಪ್ರತ್ಯಾಗಮ *thiz(); }
  iter_t& operator ++ () &  { thiz()->__next__ (); ಪ್ರತ್ಯಾಗಮ *thiz(); }
  iter_t  operator ++ () && { thiz()->__next__ (); ಪ್ರತ್ಯಾಗಮ *thiz(); }
  iter_t& operator -= (unsigned ಎಣಿಕೆ) &  { thiz()->__rewind__ (ಎಣಿಕೆ); ಪ್ರತ್ಯಾಗಮ *thiz(); }
  iter_t  operator -= (unsigned ಎಣಿಕೆ) && { thiz()->__rewind__ (ಎಣಿಕೆ); ಪ್ರತ್ಯಾಗಮ *thiz(); }
  iter_t& operator -- () &  { thiz()->__prev__ (); ಪ್ರತ್ಯಾಗಮ *thiz(); }
  iter_t  operator -- () && { thiz()->__prev__ (); ಪ್ರತ್ಯಾಗಮ *thiz(); }
  iter_t operator + (unsigned ಎಣಿಕೆ) ಧ್ರುವ { auto c = thiz()->iter (); c += ಎಣಿಕೆ; ಪ್ರತ್ಯಾಗಮ c; }
  friend iter_t operator + (unsigned ಎಣಿಕೆ, ಧ್ರುವ iter_t &it) { ಪ್ರತ್ಯಾಗಮ it + ಎಣಿಕೆ; }
  iter_t operator ++ (ಪೂರ್ಣಾಙ್ಕ) { iter_t c (*thiz()); ++*thiz(); ಪ್ರತ್ಯಾಗಮ c; }
  iter_t operator - (unsigned ಎಣಿಕೆ) ಧ್ರುವ { auto c = thiz()->iter (); c -= ಎಣಿಕೆ; ಪ್ರತ್ಯಾಗಮ c; }
  iter_t operator -- (ಪೂರ್ಣಾಙ್ಕ) { iter_t c (*thiz()); --*thiz(); ಪ್ರತ್ಯಾಗಮ c; }
  template <typename T>
  iter_t& operator >> (T &v) &  { v = **thiz(); ++*thiz(); ಪ್ರತ್ಯಾಗಮ *thiz(); }
  template <typename T>
  iter_t  operator >> (T &v) && { v = **thiz(); ++*thiz(); ಪ್ರತ್ಯಾಗಮ *thiz(); }
  template <typename T>
  iter_t& operator << (ಧ್ರುವ T v) &  { **thiz() = v; ++*thiz(); ಪ್ರತ್ಯಾಗಮ *thiz(); }
  template <typename T>
  iter_t  operator << (ಧ್ರುವ T v) && { **thiz() = v; ++*thiz(); ಪ್ರತ್ಯಾಗಮ *thiz(); }

  ರಕ್ಷಿತ:
  hb_iter_t () = ಸ್ವಭಾವ;
  hb_iter_t (ಧ್ರುವ hb_iter_t &o HB_UNUSED) = ಸ್ವಭಾವ;
  hb_iter_t (hb_iter_t &&o HB_UNUSED) = ಸ್ವಭಾವ;
  hb_iter_t& operator = (ಧ್ರುವ hb_iter_t &o HB_UNUSED) = ಸ್ವಭಾವ;
  hb_iter_t& operator = (hb_iter_t &&o HB_UNUSED) = ಸ್ವಭಾವ;
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
  using Name::operator ತಾರ್ಕಿಕ; \
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
  static_assert (ಸತ್ಯ, "")

/* Returns iterator / item type of a type. */
template <typename Iterable>
using hb_iter_type = decltype (hb_deref (hb_declval (Iterable)).iter ());
template <typename Iterable>
using hb_item_type = decltype (*hb_deref (hb_declval (Iterable)).iter ());


template <typename> ಸಂರಚನಾ hb_array_t;
template <typename> ಸಂರಚನಾ hb_sorted_array_t;

ಸಂರಚನಾ
{
  template <typename T> hb_iter_type<T>
  operator () (T&& c) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_deref (std::forward<T> (c)).iter (); }

  /* Specialization for C arrays. */

  template <typename Type> ಅನ್ತಃಸ್ಥ hb_array_t<Type>
  operator () (Type *array, unsigned ಪೂರ್ಣಾಙ್ಕ length) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_array_t<Type> (array, length); }

  template <typename Type, unsigned ಪೂರ್ಣಾಙ್ಕ length> hb_array_t<Type>
  operator () (Type (&array)[length]) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_array_t<Type> (array, length); }

}
HB_FUNCOBJ (hb_iter);
ಸಂರಚನಾ
{
  template <typename T> auto
  impl (T&& c, hb_priority<1>) ಧ್ರುವ HB_RETURN (unsigned, c.len ())

  template <typename T> auto
  impl (T&& c, hb_priority<0>) ಧ್ರುವ HB_RETURN (unsigned, c.len)

  ಸಾರ್ವಜನಿಕ:

  template <typename T> auto
  operator () (T&& c) ಧ್ರುವ HB_RETURN (unsigned, impl (std::forward<T> (c), hb_prioritize))
}
HB_FUNCOBJ (hb_len);

/* Mixin to fill in what the subclass doesn't provide. */
template <typename iter_t, typename item_t = typename iter_t::__item_t__>
ಸಂರಚನಾ hb_iter_fallback_mixin_t
{
  ಗುಪ್ತ:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ಧ್ರುವ iter_t* thiz () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ static_cast<ಧ್ರುವ iter_t *> (ಏತತ್); }
	iter_t* thiz ()       { ಪ್ರತ್ಯಾಗಮ static_cast<      iter_t *> (ಏತತ್); }
  ಸಾರ್ವಜನಿಕ:

  /* Access: Implement __item__(), or __item_at__() if random-access. */
  item_t __item__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ (*thiz())[0]; }
  item_t __item_at__ (unsigned i) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ *(*thiz() + i); }

  /* Termination: Implement __more__(), or __len__() if random-access. */
  ತಾರ್ಕಿಕ __more__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ತಾರ್ಕಿಕ (thiz()->len ()); }
  unsigned __len__ () ಧ್ರುವ
  { iter_t c (*thiz()); unsigned l = 0; ಯಾವತ್ (c) { c++; l++; } ಪ್ರತ್ಯಾಗಮ l; }

  /* Advancing: Implement __next__(), or __forward__() if random-access. */
  ಶೂನ್ಯ __next__ () { *thiz() += 1; }
  ಶೂನ್ಯ __forward__ (unsigned n) { ಯಾವತ್ (*thiz() && n--) ++*thiz(); }

  /* Rewinding: Implement __prev__() or __rewind__() if bidirectional. */
  ಶೂನ್ಯ __prev__ () { *thiz() -= 1; }
  ಶೂನ್ಯ __rewind__ (unsigned n) { ಯಾವತ್ (*thiz() && n--) --*thiz(); }

  /* Range-based for: Implement __end__() if can be done faster,
   * and operator!=. */
  iter_t __end__ () ಧ್ರುವ
  {
    ಯದಿ (thiz()->is_random_access_iterator)
      ಪ್ರತ್ಯಾಗಮ *thiz() + thiz()->len ();
    /* Above expression loops twice. Following loops once. */
    auto it = *thiz();
    ಯಾವತ್ (it) ++it;
    ಪ್ರತ್ಯಾಗಮ it;
  }

  ರಕ್ಷಿತ:
  hb_iter_fallback_mixin_t () = ಸ್ವಭಾವ;
  hb_iter_fallback_mixin_t (ಧ್ರುವ hb_iter_fallback_mixin_t &o HB_UNUSED) = ಸ್ವಭಾವ;
  hb_iter_fallback_mixin_t (hb_iter_fallback_mixin_t &&o HB_UNUSED) = ಸ್ವಭಾವ;
  hb_iter_fallback_mixin_t& operator = (ಧ್ರುವ hb_iter_fallback_mixin_t &o HB_UNUSED) = ಸ್ವಭಾವ;
  hb_iter_fallback_mixin_t& operator = (hb_iter_fallback_mixin_t &&o HB_UNUSED) = ಸ್ವಭಾವ;
};

template <typename iter_t, typename item_t = typename iter_t::__item_t__>
ಸಂರಚನಾ hb_iter_with_fallback_t :
  hb_iter_t<iter_t, item_t>,
  hb_iter_fallback_mixin_t<iter_t, item_t>
{
  ರಕ್ಷಿತ:
  hb_iter_with_fallback_t () = ಸ್ವಭಾವ;
  hb_iter_with_fallback_t (ಧ್ರುವ hb_iter_with_fallback_t &o HB_UNUSED) = ಸ್ವಭಾವ;
  hb_iter_with_fallback_t (hb_iter_with_fallback_t &&o HB_UNUSED) = ಸ್ವಭಾವ;
  hb_iter_with_fallback_t& operator = (ಧ್ರುವ hb_iter_with_fallback_t &o HB_UNUSED) = ಸ್ವಭಾವ;
  hb_iter_with_fallback_t& operator = (hb_iter_with_fallback_t &&o HB_UNUSED) = ಸ್ವಭಾವ;
};

/*
 * Meta-programming predicates.
 */

/* hb_is_iterator() / hb_is_iterator_of() */

template<typename Iter, typename Item>
ಸಂರಚನಾ hb_is_iterator_of
{
  template <typename Item2 = Item>
  ಸ್ಥಿರ hb_true_type impl (hb_priority<2>, hb_iter_t<Iter, hb_type_identity<Item2>> *);
  ಸ್ಥಿರ hb_false_type impl (hb_priority<0>, ಧ್ರುವ ಶೂನ್ಯ *);

  ಸಾರ್ವಜನಿಕ:
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ value = decltype (impl (hb_prioritize, hb_declval (Iter*)))::value;
};
#define hb_is_iterator_of(Iter, Item) hb_is_iterator_of<Iter, Item>::value
#define hb_is_iterator(Iter) hb_is_iterator_of (Iter, typename Iter::item_t)
#define hb_is_sorted_iterator_of(Iter, Item) (hb_is_iterator_of<Iter, Item>::value && Iter::is_sorted_iterator)
#define hb_is_sorted_iterator(Iter) hb_is_sorted_iterator_of (Iter, typename Iter::item_t)

/* hb_is_iterable() */

template <typename T>
ಸಂರಚನಾ hb_is_iterable
{
  ಗುಪ್ತ:

  template <typename U>
  ಸ್ಥಿರ auto impl (hb_priority<1>) -> decltype (hb_declval (U).iter (), hb_true_type ());

  template <typename>
  ಸ್ಥಿರ hb_false_type impl (hb_priority<0>);

  ಸಾರ್ವಜನಿಕ:
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ value = decltype (impl<T> (hb_prioritize))::value;
};
#define hb_is_iterable(Iterable) hb_is_iterable<Iterable>::value

/* hb_is_source_of() / hb_is_sink_of() */

template<typename Iter, typename Item>
ಸಂರಚನಾ hb_is_source_of
{
  ಗುಪ್ತ:
  template <typename Iter2 = Iter,
	    hb_enable_if (hb_is_convertible (typename Iter2::item_t, hb_add_lvalue_reference<ಧ್ರುವ Item>))>
  ಸ್ಥಿರ hb_true_type impl (hb_priority<2>);
  template <typename Iter2 = Iter>
  ಸ್ಥಿರ auto impl (hb_priority<1>) -> decltype (hb_declval (Iter2) >> hb_declval (Item &), hb_true_type ());
  ಸ್ಥಿರ hb_false_type impl (hb_priority<0>);

  ಸಾರ್ವಜನಿಕ:
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ value = decltype (impl (hb_prioritize))::value;
};
#define hb_is_source_of(Iter, Item) hb_is_source_of<Iter, Item>::value

template<typename Iter, typename Item>
ಸಂರಚನಾ hb_is_sink_of
{
  ಗುಪ್ತ:
  template <typename Iter2 = Iter,
	    hb_enable_if (hb_is_convertible (typename Iter2::item_t, hb_add_lvalue_reference<Item>))>
  ಸ್ಥಿರ hb_true_type impl (hb_priority<2>);
  template <typename Iter2 = Iter>
  ಸ್ಥಿರ auto impl (hb_priority<1>) -> decltype (hb_declval (Iter2) << hb_declval (Item), hb_true_type ());
  ಸ್ಥಿರ hb_false_type impl (hb_priority<0>);

  ಸಾರ್ವಜನಿಕ:
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ value = decltype (impl (hb_prioritize))::value;
};
#define hb_is_sink_of(Iter, Item) hb_is_sink_of<Iter, Item>::value

/* This is commonly used, so define: */
#define hb_is_sorted_source_of(Iter, Item) \
	(hb_is_source_of(Iter, Item) && Iter::is_sorted_iterator)


ಸಂರಚನಾ
{
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  unsigned operator () (ಧ್ರುವ Iterable &_) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ hb_len (hb_iter (_)); }

  unsigned operator () (unsigned _) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ _; }
}
HB_FUNCOBJ (hb_len_of);

/* Range-based 'for' for iterables. */

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ auto begin (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).begin ())

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ auto end (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).end ())

/* begin()/end() are NOT looked up non-ADL.  So each namespace must declare them.
 * Do it for namespace OT. */
ನಾಮಾಕಾಶ OT {

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ auto begin (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).begin ())

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ auto end (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).end ())

}


/*
 * Adaptors, combiners, etc.
 */

template <typename Lhs, typename Rhs,
	  hb_requires (hb_is_iterator (Lhs))>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ auto
operator | (Lhs&& lhs, Rhs&& rhs) HB_AUTO_RETURN (std::forward<Rhs> (rhs) (std::forward<Lhs> (lhs)))

/* hb_map(), hb_filter(), hb_reduce() */

ಗಣನಾ  ವರ್ಗ hb_function_sortedness_t {
  NOT_SORTED,
  RETAINS_SORTING,
  SORTED,
};

template <typename Iter, typename Proj, hb_function_sortedness_t Sorted,
	 hb_requires (hb_is_iterator (Iter))>
ಸಂರಚನಾ hb_map_iter_t :
  hb_iter_t<hb_map_iter_t<Iter, Proj, Sorted>,
	    decltype (hb_get (hb_declval (Proj), *hb_declval (Iter)))>
{
  hb_map_iter_t (ಧ್ರುವ Iter& it, Proj f_) : it (it), f (f_) {}

  ಪ್ರಕಾರನಿರ್ದೇಶ decltype (hb_get (hb_declval (Proj), *hb_declval (Iter))) __item_t__;
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ is_random_access_iterator = Iter::is_random_access_iterator;
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ is_sorted_iterator =
    Sorted == hb_function_sortedness_t::SORTED ? ಸತ್ಯ :
    Sorted == hb_function_sortedness_t::RETAINS_SORTING ? Iter::is_sorted_iterator :
    ಅಸತ್ಯ;
  __item_t__ __item__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ hb_get (f.get (), *it); }
  __item_t__ __item_at__ (unsigned i) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ hb_get (f.get (), it[i]); }
  ತಾರ್ಕಿಕ __more__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ತಾರ್ಕಿಕ (it); }
  unsigned __len__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ it.len (); }
  ಶೂನ್ಯ __next__ () { ++it; }
  ಶೂನ್ಯ __forward__ (unsigned n) { it += n; }
  ಶೂನ್ಯ __prev__ () { --it; }
  ಶೂನ್ಯ __rewind__ (unsigned n) { it -= n; }
  hb_map_iter_t __end__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ hb_map_iter_t (it._end (), f); }
  ತಾರ್ಕಿಕ operator != (ಧ್ರುವ hb_map_iter_t& o) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ it != o.it; }

  ಗುಪ್ತ:
  Iter it;
  mutable hb_reference_wrapper<Proj> f;
};

template <typename Proj, hb_function_sortedness_t Sorted>
ಸಂರಚನಾ hb_map_iter_factory_t
{
  hb_map_iter_factory_t (Proj f) : f (f) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  hb_map_iter_t<Iter, Proj, Sorted>
  operator () (Iter it)
  { ಪ್ರತ್ಯಾಗಮ hb_map_iter_t<Iter, Proj, Sorted> (it, f); }

  ಗುಪ್ತ:
  Proj f;
};
ಸಂರಚನಾ
{
  template <typename Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::NOT_SORTED>
  operator () (Proj&& f) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_map_iter_factory_t<Proj, hb_function_sortedness_t::NOT_SORTED> (f); }
}
HB_FUNCOBJ (hb_map);
ಸಂರಚನಾ
{
  template <typename Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::RETAINS_SORTING>
  operator () (Proj&& f) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_map_iter_factory_t<Proj, hb_function_sortedness_t::RETAINS_SORTING> (f); }
}
HB_FUNCOBJ (hb_map_retains_sorting);
ಸಂರಚನಾ
{
  template <typename Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::SORTED>
  operator () (Proj&& f) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_map_iter_factory_t<Proj, hb_function_sortedness_t::SORTED> (f); }
}
HB_FUNCOBJ (hb_map_sorted);

template <typename Iter, typename Pred, typename Proj,
	 hb_requires (hb_is_iterator (Iter))>
ಸಂರಚನಾ hb_filter_iter_t :
  hb_iter_with_fallback_t<hb_filter_iter_t<Iter, Pred, Proj>,
			  typename Iter::item_t>
{
  hb_filter_iter_t (ಧ್ರುವ Iter& it_, Pred p_, Proj f_) : it (it_), p (p_), f (f_)
  { ಯಾವತ್ (it && !hb_has (p.get (), hb_get (f.get (), *it))) ++it; }

  ಪ್ರಕಾರನಿರ್ದೇಶ typename Iter::item_t __item_t__;
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ is_sorted_iterator = Iter::is_sorted_iterator;
  __item_t__ __item__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ *it; }
  ತಾರ್ಕಿಕ __more__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ತಾರ್ಕಿಕ (it); }
  ಶೂನ್ಯ __next__ () { ಕುರು ++it; ಯಾವತ್ (it && !hb_has (p.get (), hb_get (f.get (), *it))); }
  ಶೂನ್ಯ __prev__ ()
  {
    unsigned ಎಣಿಕೆ = 0;
    ಕುರು
    {
      Iter old = it;
      --it;
      ಯದಿ (!(it != old))
      {
	it += ಎಣಿಕೆ;
	ಪ್ರತ್ಯಾಗಮ;
      }
      ಎಣಿಕೆ++;
    }
    ಯಾವತ್ (!hb_has (p.get (), hb_get (f.get (), *it)));
  }
  hb_filter_iter_t __end__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ hb_filter_iter_t (it._end (), p, f); }
  ತಾರ್ಕಿಕ operator != (ಧ್ರುವ hb_filter_iter_t& o) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ it != o.it; }

  ಗುಪ್ತ:
  Iter it;
  mutable hb_reference_wrapper<Pred> p;
  mutable hb_reference_wrapper<Proj> f;
};
template <typename Pred, typename Proj>
ಸಂರಚನಾ hb_filter_iter_factory_t
{
  hb_filter_iter_factory_t (Pred p, Proj f) : p (p), f (f) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  hb_filter_iter_t<Iter, Pred, Proj>
  operator () (Iter it)
  { ಪ್ರತ್ಯಾಗಮ hb_filter_iter_t<Iter, Pred, Proj> (it, p, f); }

  ಗುಪ್ತ:
  Pred p;
  Proj f;
};
ಸಂರಚನಾ
{
  template <typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity))>
  hb_filter_iter_factory_t<Pred, Proj>
  operator () (Pred&& p = hb_identity, Proj&& f = hb_identity) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_filter_iter_factory_t<Pred, Proj> (p, f); }
}
HB_FUNCOBJ (hb_filter);

template <typename Redu, typename InitT>
ಸಂರಚನಾ hb_reduce_t
{
  hb_reduce_t (Redu r, InitT init_ಮೌಲ್ಯ) : r (r), init_ಮೌಲ್ಯ (init_ಮೌಲ್ಯ) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter)),
	    typename AccuT = hb_decay<decltype (hb_declval (Redu) (hb_declval (InitT), hb_declval (typename Iter::item_t)))>>
  AccuT
  operator () (Iter it)
  {
    AccuT value = init_ಮೌಲ್ಯ;
    ಪ್ರತಿ (; it; ++it)
      value = r (value, *it);
    ಪ್ರತ್ಯಾಗಮ value;
  }

  ಗುಪ್ತ:
  Redu r;
  InitT init_ಮೌಲ್ಯ;
};
ಸಂರಚನಾ
{
  template <typename Redu, typename InitT>
  hb_reduce_t<Redu, InitT>
  operator () (Redu&& r, InitT init_ಮೌಲ್ಯ) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_reduce_t<Redu, InitT> (r, init_ಮೌಲ್ಯ); }
}
HB_FUNCOBJ (hb_reduce);


/* hb_zip() */

template <typename A, typename B>
ಸಂರಚನಾ hb_zip_iter_t :
  hb_iter_t<hb_zip_iter_t<A, B>,
	    hb_pair_t<typename A::item_t, typename B::item_t>>
{
  hb_zip_iter_t () {}
  hb_zip_iter_t (ಧ್ರುವ A& a, ಧ್ರುವ B& b) : a (a), b (b) {}

  ಪ್ರಕಾರನಿರ್ದೇಶ hb_pair_t<typename A::item_t, typename B::item_t> __item_t__;
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ is_random_access_iterator =
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
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ is_sorted_iterator = A::is_sorted_iterator;

  __item_t__ __item__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ __item_t__ (*a, *b); }
  __item_t__ __item_at__ (unsigned i) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ __item_t__ (a[i], b[i]); }
  ತಾರ್ಕಿಕ __more__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ತಾರ್ಕಿಕ (a) && ತಾರ್ಕಿಕ (b); }
  unsigned __len__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ hb_min (a.len (), b.len ()); }
  ಶೂನ್ಯ __next__ () { ++a; ++b; }
  ಶೂನ್ಯ __forward__ (unsigned n) { a += n; b += n; }
  ಶೂನ್ಯ __prev__ () { --a; --b; }
  ಶೂನ್ಯ __rewind__ (unsigned n) { a -= n; b -= n; }
  hb_zip_iter_t __end__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ hb_zip_iter_t (a._end (), b._end ()); }
  /* Note, we should stop if ANY of the iters reaches end.  As such two compare
   * unequal if both items are unequal, NOT if either is unequal. */
  ತಾರ್ಕಿಕ operator != (ಧ್ರುವ hb_zip_iter_t& o) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ a != o.a && b != o.b; }

  ಗುಪ್ತ:
  A a;
  B b;
};
ಸಂರಚನಾ
{ HB_PARTIALIZE(2);
  template <typename A, typename B,
	    hb_requires (hb_is_iterable (A) && hb_is_iterable (B))>
  hb_zip_iter_t<hb_iter_type<A>, hb_iter_type<B>>
  operator () (A&& a, B&& b) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_zip_iter_t<hb_iter_type<A>, hb_iter_type<B>> (hb_iter (a), hb_iter (b)); }
}
HB_FUNCOBJ (hb_zip);

/* hb_concat() */

template <typename A, typename B>
ಸಂರಚನಾ hb_concat_iter_t :
    hb_iter_t<hb_concat_iter_t<A, B>, typename A::item_t>
{
  hb_concat_iter_t () {}
  hb_concat_iter_t (A& a, B& b) : a (a), b (b) {}
  hb_concat_iter_t (ಧ್ರುವ A& a, ಧ್ರುವ B& b) : a (a), b (b) {}


  ಪ್ರಕಾರನಿರ್ದೇಶ typename A::item_t __item_t__;
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ is_random_access_iterator =
    A::is_random_access_iterator &&
    B::is_random_access_iterator;
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ is_sorted_iterator = ಅಸತ್ಯ;

  __item_t__ __item__ () ಧ್ರುವ
  {
    ಯದಿ (!a)
      ಪ್ರತ್ಯಾಗಮ *b;
    ಪ್ರತ್ಯಾಗಮ *a;
  }

  __item_t__ __item_at__ (unsigned i) ಧ್ರುವ
  {
    unsigned a_len = a.len ();
    ಯದಿ (i < a_len)
      ಪ್ರತ್ಯಾಗಮ a[i];
    ಪ್ರತ್ಯಾಗಮ b[i - a_len];
  }

  ತಾರ್ಕಿಕ __more__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ತಾರ್ಕಿಕ (a) || ತಾರ್ಕಿಕ (b); }

  unsigned __len__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ a.len () + b.len (); }

  ಶೂನ್ಯ __next__ ()
  {
    ಯದಿ (a)
      ++a;
    ಅನ್ಯಥಾ
      ++b;
  }

  ಶೂನ್ಯ __forward__ (unsigned n)
  {
    ಯದಿ (!n) ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (!is_random_access_iterator) {
      ಯಾವತ್ (n-- && *ಏತತ್) {
        (*ಏತತ್)++;
      }
      ಪ್ರತ್ಯಾಗಮ;
    }

    unsigned a_len = a.len ();
    ಯದಿ (n > a_len) {
      n -= a_len;
      a.__forward__ (a_len);
      b.__forward__ (n);
    } ಅನ್ಯಥಾ {
      a.__forward__ (n);
    }
  }

  hb_concat_iter_t __end__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ hb_concat_iter_t (a._end (), b._end ()); }
  ತಾರ್ಕಿಕ operator != (ಧ್ರುವ hb_concat_iter_t& o) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ a != o.a
        || b != o.b;
  }

  ಗುಪ್ತ:
  A a;
  B b;
};
ಸಂರಚನಾ
{ HB_PARTIALIZE(2);
  template <typename A, typename B,
	    hb_requires (hb_is_iterable (A) && hb_is_iterable (B))>
  hb_concat_iter_t<hb_iter_type<A>, hb_iter_type<B>>
  operator () (A&& a, B&& b) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_concat_iter_t<hb_iter_type<A>, hb_iter_type<B>> (hb_iter (a), hb_iter (b)); }
}
HB_FUNCOBJ (hb_concat);

/* hb_apply() */

template <typename Appl>
ಸಂರಚನಾ hb_apply_t
{
  hb_apply_t (Appl a) : a (a) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  ಶೂನ್ಯ operator () (Iter it)
  {
    ಪ್ರತಿ (; it; ++it)
      (ಶೂನ್ಯ) hb_invoke (a, *it);
  }

  ಗುಪ್ತ:
  Appl a;
};
ಸಂರಚನಾ
{
  template <typename Appl> hb_apply_t<Appl>
  operator () (Appl&& a) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_apply_t<Appl> (a); }

  template <typename Appl> hb_apply_t<Appl&>
  operator () (Appl *a) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_apply_t<Appl&> (*a); }
}
HB_FUNCOBJ (hb_apply);

/* hb_range()/hb_iota()/hb_repeat() */

template <typename T, typename S>
ಸಂರಚನಾ hb_range_iter_t :
  hb_iter_t<hb_range_iter_t<T, S>, T>
{
  hb_range_iter_t (T start, T end_, S step) : v (start), end_ (end_for (start, end_, step)), step (step) {}

  ಪ್ರಕಾರನಿರ್ದೇಶ T __item_t__;
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ is_random_access_iterator = ಸತ್ಯ;
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ is_sorted_iterator = ಸತ್ಯ;
  __item_t__ __item__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ hb_ridentity (v); }
  __item_t__ __item_at__ (unsigned j) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ v + j * step; }
  ತಾರ್ಕಿಕ __more__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ v != end_; }
  unsigned __len__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ !step ? UINT_MAX : (end_ - v) / step; }
  ಶೂನ್ಯ __next__ () { v += step; }
  ಶೂನ್ಯ __forward__ (unsigned n) { v += n * step; }
  ಶೂನ್ಯ __prev__ () { v -= step; }
  ಶೂನ್ಯ __rewind__ (unsigned n) { v -= n * step; }
  hb_range_iter_t __end__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ hb_range_iter_t (end_, end_, step); }
  ತಾರ್ಕಿಕ operator != (ಧ್ರುವ hb_range_iter_t& o) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ v != o.v; }

  ಗುಪ್ತ:
  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ T end_for (T start, T end_, S step)
  {
    ಯದಿ (!step)
      ಪ್ರತ್ಯಾಗಮ end_;
    auto res = (end_ - start) % step;
    ಯದಿ (!res)
      ಪ್ರತ್ಯಾಗಮ end_;
    end_ += step - res;
    ಪ್ರತ್ಯಾಗಮ end_;
  }

  ಗುಪ್ತ:
  T v;
  T end_;
  S step;
};
ಸಂರಚನಾ
{
  template <typename T = unsigned> hb_range_iter_t<T, unsigned>
  operator () (T end = (unsigned) -1) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_range_iter_t<T, unsigned> (0, end, 1u); }

  template <typename T, typename S = unsigned> hb_range_iter_t<T, S>
  operator () (T start, T end, S step = 1u) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_range_iter_t<T, S> (start, end, step); }
}
HB_FUNCOBJ (hb_range);

template <typename T, typename S>
ಸಂರಚನಾ hb_iota_iter_t :
  hb_iter_with_fallback_t<hb_iota_iter_t<T, S>, T>
{
  hb_iota_iter_t (T start, S step) : v (start), step (step) {}

  ಗುಪ್ತ:

  template <typename S2 = S>
  auto
  inc (hb_type_identity<S2> s, hb_priority<1>)
    -> hb_void_t<decltype (hb_invoke (std::forward<hb_type_identity<S2>> (s),
                                      hb_declval<T&> ()))>
  { v = hb_invoke (std::forward<hb_type_identity<S2>> (s), v); }

  ಶೂನ್ಯ
  inc (S s, hb_priority<0>)
  { v += s; }

  ಸಾರ್ವಜನಿಕ:

  ಪ್ರಕಾರನಿರ್ದೇಶ T __item_t__;
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ is_random_access_iterator = ಸತ್ಯ;
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ is_sorted_iterator = ಸತ್ಯ;
  __item_t__ __item__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ hb_ridentity (v); }
  ತಾರ್ಕಿಕ __more__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; }
  unsigned __len__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ UINT_MAX; }
  ಶೂನ್ಯ __next__ () { inc (step, hb_prioritize); }
  ಶೂನ್ಯ __prev__ () { v -= step; }
  hb_iota_iter_t __end__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
  ತಾರ್ಕಿಕ operator != (ಧ್ರುವ hb_iota_iter_t& o) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; }

  ಗುಪ್ತ:
  T v;
  S step;
};
ಸಂರಚನಾ
{
  template <typename T = unsigned, typename S = unsigned> hb_iota_iter_t<T, S>
  operator () (T start = 0u, S step = 1u) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_iota_iter_t<T, S> (start, step); }
}
HB_FUNCOBJ (hb_iota);

template <typename T>
ಸಂರಚನಾ hb_repeat_iter_t :
  hb_iter_t<hb_repeat_iter_t<T>, T>
{
  hb_repeat_iter_t (T value) : v (value) {}

  ಪ್ರಕಾರನಿರ್ದೇಶ T __item_t__;
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ is_random_access_iterator = ಸತ್ಯ;
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ is_sorted_iterator = ಸತ್ಯ;
  __item_t__ __item__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ v; }
  __item_t__ __item_at__ (unsigned j) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ v; }
  ತಾರ್ಕಿಕ __more__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; }
  unsigned __len__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ UINT_MAX; }
  ಶೂನ್ಯ __next__ () {}
  ಶೂನ್ಯ __forward__ (unsigned) {}
  ಶೂನ್ಯ __prev__ () {}
  ಶೂನ್ಯ __rewind__ (unsigned) {}
  hb_repeat_iter_t __end__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
  ತಾರ್ಕಿಕ operator != (ಧ್ರುವ hb_repeat_iter_t& o) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; }

  ಗುಪ್ತ:
  T v;
};
ಸಂರಚನಾ
{
  template <typename T> hb_repeat_iter_t<T>
  operator () (T value) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_repeat_iter_t<T> (value); }
}
HB_FUNCOBJ (hb_repeat);

/* hb_enumerate()/hb_take() */

ಸಂರಚನಾ
{
  template <typename Iterable,
	    typename Index = unsigned,
	    hb_requires (hb_is_iterable (Iterable))>
  auto operator () (Iterable&& it, Index start = 0u) ಧ್ರುವ HB_AUTO_RETURN
  ( hb_zip (hb_iota (start), it) )
}
HB_FUNCOBJ (hb_enumerate);

ಸಂರಚನಾ
{ HB_PARTIALIZE(2);
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  auto operator () (Iterable&& it, unsigned ಎಣಿಕೆ) ಧ್ರುವ HB_AUTO_RETURN
  ( hb_zip (hb_range (ಎಣಿಕೆ), it) | hb_map_retains_sorting (hb_second) )

  /* Specialization arrays. */

  template <typename Type> ಅನ್ತಃಸ್ಥ hb_array_t<Type>
  operator () (hb_array_t<Type> array, unsigned ಎಣಿಕೆ) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ array.sub_array (0, ಎಣಿಕೆ); }

  template <typename Type> ಅನ್ತಃಸ್ಥ hb_sorted_array_t<Type>
  operator () (hb_sorted_array_t<Type> array, unsigned ಎಣಿಕೆ) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ array.sub_array (0, ಎಣಿಕೆ); }
}
HB_FUNCOBJ (hb_take);

ಸಂರಚನಾ
{ HB_PARTIALIZE(2);
  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  auto operator () (Iter it, unsigned ಎಣಿಕೆ) ಧ್ರುವ HB_AUTO_RETURN
  (
    + hb_iota (it, hb_add (ಎಣಿಕೆ))
    | hb_map (hb_take (ಎಣಿಕೆ))
    | hb_take ((hb_len (it) + ಎಣಿಕೆ - 1) / ಎಣಿಕೆ)
  )
}
HB_FUNCOBJ (hb_chop);

/* hb_sink() */

template <typename Sink>
ಸಂರಚನಾ hb_sink_t
{
  hb_sink_t (Sink s) : s (s) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  ಶೂನ್ಯ operator () (Iter it)
  {
    ಪ್ರತಿ (; it; ++it)
      s << *it;
  }

  ಗುಪ್ತ:
  Sink s;
};
ಸಂರಚನಾ
{
  template <typename Sink> hb_sink_t<Sink>
  operator () (Sink&& s) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_sink_t<Sink> (s); }

  template <typename Sink> hb_sink_t<Sink&>
  operator () (Sink *s) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_sink_t<Sink&> (*s); }
}
HB_FUNCOBJ (hb_sink);

/* hb-drain: hb_sink to void / blackhole / /dev/null. */

ಸಂರಚನಾ
{
  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  ಶೂನ್ಯ operator () (Iter it) ಧ್ರುವ
  {
    ಪ್ರತಿ (; it; ++it)
      (ಶೂನ್ಯ) *it;
  }
}
HB_FUNCOBJ (hb_drain);

/* hb_unzip(): unzip and sink to two sinks. */

template <typename Sink1, typename Sink2>
ಸಂರಚನಾ hb_unzip_t
{
  hb_unzip_t (Sink1 s1, Sink2 s2) : s1 (s1), s2 (s2) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  ಶೂನ್ಯ operator () (Iter it)
  {
    ಪ್ರತಿ (; it; ++it)
    {
      ಧ್ರುವ auto &v = *it;
      s1 << v.first;
      s2 << v.second;
    }
  }

  ಗುಪ್ತ:
  Sink1 s1;
  Sink2 s2;
};
ಸಂರಚನಾ
{
  template <typename Sink1, typename Sink2> hb_unzip_t<Sink1, Sink2>
  operator () (Sink1&& s1, Sink2&& s2) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_unzip_t<Sink1, Sink2> (s1, s2); }

  template <typename Sink1, typename Sink2> hb_unzip_t<Sink1&, Sink2&>
  operator () (Sink1 *s1, Sink2 *s2) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_unzip_t<Sink1&, Sink2&> (*s1, *s2); }
}
HB_FUNCOBJ (hb_unzip);


/* hb-all, hb-any, hb-none. */

ಸಂರಚನಾ
{
  template <typename Iterable,
	    typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity)),
	    hb_requires (hb_is_iterable (Iterable))>
  ತಾರ್ಕಿಕ operator () (Iterable&& c,
		    Pred&& p = hb_identity,
		    Proj&& f = hb_identity) ಧ್ರುವ
  {
    ಪ್ರತಿ (auto it = hb_iter (c); it; ++it)
      ಯದಿ (!hb_match (p, hb_get (f, *it)))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }
}
HB_FUNCOBJ (hb_all);
ಸಂರಚನಾ
{
  template <typename Iterable,
	    typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity)),
	    hb_requires (hb_is_iterable (Iterable))>
  ತಾರ್ಕಿಕ operator () (Iterable&& c,
		    Pred&& p = hb_identity,
		    Proj&& f = hb_identity) ಧ್ರುವ
  {
    ಪ್ರತಿ (auto it = hb_iter (c); it; ++it)
      ಯದಿ (hb_match (p, hb_get (f, *it)))
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }
}
HB_FUNCOBJ (hb_any);
ಸಂರಚನಾ
{
  template <typename Iterable,
	    typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity)),
	    hb_requires (hb_is_iterable (Iterable))>
  ತಾರ್ಕಿಕ operator () (Iterable&& c,
		    Pred&& p = hb_identity,
		    Proj&& f = hb_identity) ಧ್ರುವ
  {
    ಪ್ರತಿ (auto it = hb_iter (c); it; ++it)
      ಯದಿ (hb_match (p, hb_get (f, *it)))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }
}
HB_FUNCOBJ (hb_none);

/*
 * Algorithms operating on iterators.
 */

template <typename C, typename V,
	  hb_requires (hb_is_iterable (C))>
ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
hb_fill (C&& c, ಧ್ರುವ V &v)
{
  ಪ್ರತಿ (auto i = hb_iter (c); i; i++)
    *i = v;
}

template <typename S, typename D>
ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
hb_copy (S&& is, D&& id)
{
  hb_iter (is) | hb_sink (id);
}


#endif /* HB_ITER_HH */

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

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-algs.hh"
#ଅନ୍ତର୍ଭାଵ "hb-meta.hh"


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
ସଂରଚନା hb_iter_t
{
  ପ୍ରକାରବ୍ୟାଖ୍ୟା Item item_t;
  constexpr unsigned get_item_size () ଅଚଳ { ଫେରନ୍ତୁ hb_static_size (Item); }
  ସ୍ଥିର constexpr ତାର୍କିକ is_iterator = ସତ୍ୟ;
  ସ୍ଥିର constexpr ତାର୍କିକ is_random_access_iterator = ମିଥ୍ୟା;
  ସ୍ଥିର constexpr ତାର୍କିକ is_sorted_iterator = ମିଥ୍ୟା;
  ସ୍ଥିର constexpr ତାର୍କିକ has_fast_len = ମିଥ୍ୟା; // Should be checked in combination with is_random_access_iterator.

  ବ୍ୟକ୍ତିଗତ:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ଅଚଳ iter_t* thiz () ଅଚଳ { ଫେରନ୍ତୁ static_cast<ଅଚଳ iter_t *> (ଏହି); }
	iter_t* thiz ()       { ଫେରନ୍ତୁ static_cast<      iter_t *> (ଏହି); }
  ସର୍ବସାଧାରଣ:

  /* Operators. */
  iter_t iter () ଅଚଳ { ଫେରନ୍ତୁ *thiz(); }
  iter_t operator + () ଅଚଳ { ଫେରନ୍ତୁ *thiz(); }
  iter_t _begin () ଅଚଳ { ଫେରନ୍ତୁ *thiz(); }
  iter_t begin () ଅଚଳ { ଫେରନ୍ତୁ _begin (); }
  iter_t _end () ଅଚଳ { ଫେରନ୍ତୁ thiz()->__end__ (); }
  iter_t end () ଅଚଳ { ଫେରନ୍ତୁ _end (); }
  explicit operator ତାର୍କିକ () ଅଚଳ { ଫେରନ୍ତୁ thiz()->__more__ (); }
  unsigned len () ଅଚଳ { ଫେରନ୍ତୁ thiz()->__len__ (); }
  /* The following can only be enabled if item_t is reference type.  Otherwise
   * it will be returning pointer to temporary rvalue. */
  template <typename T = item_t,
	    hb_enable_if (std::is_reference<T>::value)>
  hb_remove_reference<item_t>* operator -> () ଅଚଳ { ଫେରନ୍ତୁ std::addressof (**thiz()); }
  item_t operator * () ଅଚଳ { ଫେରନ୍ତୁ thiz()->__item__ (); }
  item_t operator * () { ଫେରନ୍ତୁ thiz()->__item__ (); }
  item_t operator [] (unsigned i) ଅଚଳ { ଫେରନ୍ତୁ thiz()->__item_at__ (i); }
  item_t operator [] (unsigned i) { ଫେରନ୍ତୁ thiz()->__item_at__ (i); }
  iter_t& operator += (unsigned ଗଣନା_2) &  { thiz()->__forward__ (ଗଣନା_2); ଫେରନ୍ତୁ *thiz(); }
  iter_t  operator += (unsigned ଗଣନା_2) && { thiz()->__forward__ (ଗଣନା_2); ଫେରନ୍ତୁ *thiz(); }
  iter_t& operator ++ () &  { thiz()->__next__ (); ଫେରନ୍ତୁ *thiz(); }
  iter_t  operator ++ () && { thiz()->__next__ (); ଫେରନ୍ତୁ *thiz(); }
  iter_t& operator -= (unsigned ଗଣନା_2) &  { thiz()->__rewind__ (ଗଣନା_2); ଫେରନ୍ତୁ *thiz(); }
  iter_t  operator -= (unsigned ଗଣନା_2) && { thiz()->__rewind__ (ଗଣନା_2); ଫେରନ୍ତୁ *thiz(); }
  iter_t& operator -- () &  { thiz()->__prev__ (); ଫେରନ୍ତୁ *thiz(); }
  iter_t  operator -- () && { thiz()->__prev__ (); ଫେରନ୍ତୁ *thiz(); }
  iter_t operator + (unsigned ଗଣନା_2) ଅଚଳ { auto c = thiz()->iter (); c += ଗଣନା_2; ଫେରନ୍ତୁ c; }
  friend iter_t operator + (unsigned ଗଣନା_2, ଅଚଳ iter_t &it) { ଫେରନ୍ତୁ it + ଗଣନା_2; }
  iter_t operator ++ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) { iter_t c (*thiz()); ++*thiz(); ଫେରନ୍ତୁ c; }
  iter_t operator - (unsigned ଗଣନା_2) ଅଚଳ { auto c = thiz()->iter (); c -= ଗଣନା_2; ଫେରନ୍ତୁ c; }
  iter_t operator -- (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) { iter_t c (*thiz()); --*thiz(); ଫେରନ୍ତୁ c; }
  template <typename T>
  iter_t& operator >> (T &v) &  { v = **thiz(); ++*thiz(); ଫେରନ୍ତୁ *thiz(); }
  template <typename T>
  iter_t  operator >> (T &v) && { v = **thiz(); ++*thiz(); ଫେରନ୍ତୁ *thiz(); }
  template <typename T>
  iter_t& operator << (ଅଚଳ T v) &  { **thiz() = v; ++*thiz(); ଫେରନ୍ତୁ *thiz(); }
  template <typename T>
  iter_t  operator << (ଅଚଳ T v) && { **thiz() = v; ++*thiz(); ଫେରନ୍ତୁ *thiz(); }

  ସୁରକ୍ଷିତ:
  hb_iter_t () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  hb_iter_t (ଅଚଳ hb_iter_t &o HB_UNUSED) = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  hb_iter_t (hb_iter_t &&o HB_UNUSED) = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  hb_iter_t& operator = (ଅଚଳ hb_iter_t &o HB_UNUSED) = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  hb_iter_t& operator = (hb_iter_t &&o HB_UNUSED) = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
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
  using Name::operator ତାର୍କିକ; \
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
  static_assert (ସତ୍ୟ, "")

/* Returns iterator / item type of a type. */
template <typename Iterable>
using hb_iter_type = decltype (hb_deref (hb_declval (Iterable)).iter ());
template <typename Iterable>
using hb_item_type = decltype (*hb_deref (hb_declval (Iterable)).iter ());


template <typename> ସଂରଚନା hb_array_t;
template <typename> ସଂରଚନା hb_sorted_array_t;

ସଂରଚନା
{
  template <typename T> hb_iter_type<T>
  operator () (T&& c) ଅଚଳ
  { ଫେରନ୍ତୁ hb_deref (std::forward<T> (c)).iter (); }

  /* Specialization for C arrays. */

  template <typename Type> ଧାଡ଼ିଭିତରେ hb_array_t<Type>
  operator () (Type *array, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା length) ଅଚଳ
  { ଫେରନ୍ତୁ hb_array_t<Type> (array, length); }

  template <typename Type, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା length> hb_array_t<Type>
  operator () (Type (&array)[length]) ଅଚଳ
  { ଫେରନ୍ତୁ hb_array_t<Type> (array, length); }

}
HB_FUNCOBJ (hb_iter);
ସଂରଚନା
{
  template <typename T> auto
  impl (T&& c, hb_priority<1>) ଅଚଳ HB_RETURN (unsigned, c.len ())

  template <typename T> auto
  impl (T&& c, hb_priority<0>) ଅଚଳ HB_RETURN (unsigned, c.len)

  ସର୍ବସାଧାରଣ:

  template <typename T> auto
  operator () (T&& c) ଅଚଳ HB_RETURN (unsigned, impl (std::forward<T> (c), hb_prioritize))
}
HB_FUNCOBJ (hb_len);

/* Mixin to fill in what the subclass doesn't provide. */
template <typename iter_t, typename item_t = typename iter_t::__item_t__>
ସଂରଚନା hb_iter_fallback_mixin_t
{
  ବ୍ୟକ୍ତିଗତ:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ଅଚଳ iter_t* thiz () ଅଚଳ { ଫେରନ୍ତୁ static_cast<ଅଚଳ iter_t *> (ଏହି); }
	iter_t* thiz ()       { ଫେରନ୍ତୁ static_cast<      iter_t *> (ଏହି); }
  ସର୍ବସାଧାରଣ:

  /* Access: Implement __item__(), or __item_at__() if random-access. */
  item_t __item__ () ଅଚଳ { ଫେରନ୍ତୁ (*thiz())[0]; }
  item_t __item_at__ (unsigned i) ଅଚଳ { ଫେରନ୍ତୁ *(*thiz() + i); }

  /* Termination: Implement __more__(), or __len__() if random-access. */
  ତାର୍କିକ __more__ () ଅଚଳ { ଫେରନ୍ତୁ ତାର୍କିକ (thiz()->len ()); }
  unsigned __len__ () ଅଚଳ
  { iter_t c (*thiz()); unsigned l = 0; ଯେତେବେଳେ (c) { c++; l++; } ଫେରନ୍ତୁ l; }

  /* Advancing: Implement __next__(), or __forward__() if random-access. */
  ଶୂନ୍ୟ __next__ () { *thiz() += 1; }
  ଶୂନ୍ୟ __forward__ (unsigned n) { ଯେତେବେଳେ (*thiz() && n--) ++*thiz(); }

  /* Rewinding: Implement __prev__() or __rewind__() if bidirectional. */
  ଶୂନ୍ୟ __prev__ () { *thiz() -= 1; }
  ଶୂନ୍ୟ __rewind__ (unsigned n) { ଯେତେବେଳେ (*thiz() && n--) --*thiz(); }

  /* Range-based for: Implement __end__() if can be done faster,
   * and operator!=. */
  iter_t __end__ () ଅଚଳ
  {
    ଯଦି (thiz()->is_random_access_iterator)
      ଫେରନ୍ତୁ *thiz() + thiz()->len ();
    /* Above expression loops twice. Following loops once. */
    auto it = *thiz();
    ଯେତେବେଳେ (it) ++it;
    ଫେରନ୍ତୁ it;
  }

  ସୁରକ୍ଷିତ:
  hb_iter_fallback_mixin_t () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  hb_iter_fallback_mixin_t (ଅଚଳ hb_iter_fallback_mixin_t &o HB_UNUSED) = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  hb_iter_fallback_mixin_t (hb_iter_fallback_mixin_t &&o HB_UNUSED) = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  hb_iter_fallback_mixin_t& operator = (ଅଚଳ hb_iter_fallback_mixin_t &o HB_UNUSED) = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  hb_iter_fallback_mixin_t& operator = (hb_iter_fallback_mixin_t &&o HB_UNUSED) = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
};

template <typename iter_t, typename item_t = typename iter_t::__item_t__>
ସଂରଚନା hb_iter_with_fallback_t :
  hb_iter_t<iter_t, item_t>,
  hb_iter_fallback_mixin_t<iter_t, item_t>
{
  ସୁରକ୍ଷିତ:
  hb_iter_with_fallback_t () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  hb_iter_with_fallback_t (ଅଚଳ hb_iter_with_fallback_t &o HB_UNUSED) = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  hb_iter_with_fallback_t (hb_iter_with_fallback_t &&o HB_UNUSED) = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  hb_iter_with_fallback_t& operator = (ଅଚଳ hb_iter_with_fallback_t &o HB_UNUSED) = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  hb_iter_with_fallback_t& operator = (hb_iter_with_fallback_t &&o HB_UNUSED) = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
};

/*
 * Meta-programming predicates.
 */

/* hb_is_iterator() / hb_is_iterator_of() */

template<typename Iter, typename Item>
ସଂରଚନା hb_is_iterator_of
{
  template <typename Item2 = Item>
  ସ୍ଥିର hb_true_type impl (hb_priority<2>, hb_iter_t<Iter, hb_type_identity<Item2>> *);
  ସ୍ଥିର hb_false_type impl (hb_priority<0>, ଅଚଳ ଶୂନ୍ୟ *);

  ସର୍ବସାଧାରଣ:
  ସ୍ଥିର constexpr ତାର୍କିକ value = decltype (impl (hb_prioritize, hb_declval (Iter*)))::value;
};
#define hb_is_iterator_of(Iter, Item) hb_is_iterator_of<Iter, Item>::value
#define hb_is_iterator(Iter) hb_is_iterator_of (Iter, typename Iter::item_t)
#define hb_is_sorted_iterator_of(Iter, Item) (hb_is_iterator_of<Iter, Item>::value && Iter::is_sorted_iterator)
#define hb_is_sorted_iterator(Iter) hb_is_sorted_iterator_of (Iter, typename Iter::item_t)

/* hb_is_iterable() */

template <typename T>
ସଂରଚନା hb_is_iterable
{
  ବ୍ୟକ୍ତିଗତ:

  template <typename U>
  ସ୍ଥିର auto impl (hb_priority<1>) -> decltype (hb_declval (U).iter (), hb_true_type ());

  template <typename>
  ସ୍ଥିର hb_false_type impl (hb_priority<0>);

  ସର୍ବସାଧାରଣ:
  ସ୍ଥିର constexpr ତାର୍କିକ value = decltype (impl<T> (hb_prioritize))::value;
};
#define hb_is_iterable(Iterable) hb_is_iterable<Iterable>::value

/* hb_is_source_of() / hb_is_sink_of() */

template<typename Iter, typename Item>
ସଂରଚନା hb_is_source_of
{
  ବ୍ୟକ୍ତିଗତ:
  template <typename Iter2 = Iter,
	    hb_enable_if (hb_is_convertible (typename Iter2::item_t, hb_add_lvalue_reference<ଅଚଳ Item>))>
  ସ୍ଥିର hb_true_type impl (hb_priority<2>);
  template <typename Iter2 = Iter>
  ସ୍ଥିର auto impl (hb_priority<1>) -> decltype (hb_declval (Iter2) >> hb_declval (Item &), hb_true_type ());
  ସ୍ଥିର hb_false_type impl (hb_priority<0>);

  ସର୍ବସାଧାରଣ:
  ସ୍ଥିର constexpr ତାର୍କିକ value = decltype (impl (hb_prioritize))::value;
};
#define hb_is_source_of(Iter, Item) hb_is_source_of<Iter, Item>::value

template<typename Iter, typename Item>
ସଂରଚନା hb_is_sink_of
{
  ବ୍ୟକ୍ତିଗତ:
  template <typename Iter2 = Iter,
	    hb_enable_if (hb_is_convertible (typename Iter2::item_t, hb_add_lvalue_reference<Item>))>
  ସ୍ଥିର hb_true_type impl (hb_priority<2>);
  template <typename Iter2 = Iter>
  ସ୍ଥିର auto impl (hb_priority<1>) -> decltype (hb_declval (Iter2) << hb_declval (Item), hb_true_type ());
  ସ୍ଥିର hb_false_type impl (hb_priority<0>);

  ସର୍ବସାଧାରଣ:
  ସ୍ଥିର constexpr ତାର୍କିକ value = decltype (impl (hb_prioritize))::value;
};
#define hb_is_sink_of(Iter, Item) hb_is_sink_of<Iter, Item>::value

/* This is commonly used, so define: */
#define hb_is_sorted_source_of(Iter, Item) \
	(hb_is_source_of(Iter, Item) && Iter::is_sorted_iterator)


ସଂରଚନା
{
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  unsigned operator () (ଅଚଳ Iterable &_) ଅଚଳ { ଫେରନ୍ତୁ hb_len (hb_iter (_)); }

  unsigned operator () (unsigned _) ଅଚଳ { ଫେରନ୍ତୁ _; }
}
HB_FUNCOBJ (hb_len_of);

/* Range-based 'for' for iterables. */

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ auto begin (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).begin ())

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ auto end (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).end ())

/* begin()/end() are NOT looked up non-ADL.  So each namespace must declare them.
 * Do it for namespace OT. */
ନାମାକାଶ OT {

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ auto begin (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).begin ())

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ auto end (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).end ())

}


/*
 * Adaptors, combiners, etc.
 */

template <typename Lhs, typename Rhs,
	  hb_requires (hb_is_iterator (Lhs))>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ auto
operator | (Lhs&& lhs, Rhs&& rhs) HB_AUTO_RETURN (std::forward<Rhs> (rhs) (std::forward<Lhs> (lhs)))

/* hb_map(), hb_filter(), hb_reduce() */

ଗଣନା  ଶ୍ରେଣୀ hb_function_sortedness_t {
  NOT_SORTED,
  RETAINS_SORTING,
  SORTED,
};

template <typename Iter, typename Proj, hb_function_sortedness_t Sorted,
	 hb_requires (hb_is_iterator (Iter))>
ସଂରଚନା hb_map_iter_t :
  hb_iter_t<hb_map_iter_t<Iter, Proj, Sorted>,
	    decltype (hb_get (hb_declval (Proj), *hb_declval (Iter)))>
{
  hb_map_iter_t (ଅଚଳ Iter& it, Proj f_) : it (it), f (f_) {}

  ପ୍ରକାରବ୍ୟାଖ୍ୟା decltype (hb_get (hb_declval (Proj), *hb_declval (Iter))) __item_t__;
  ସ୍ଥିର constexpr ତାର୍କିକ is_random_access_iterator = Iter::is_random_access_iterator;
  ସ୍ଥିର constexpr ତାର୍କିକ is_sorted_iterator =
    Sorted == hb_function_sortedness_t::SORTED ? ସତ୍ୟ :
    Sorted == hb_function_sortedness_t::RETAINS_SORTING ? Iter::is_sorted_iterator :
    ମିଥ୍ୟା;
  __item_t__ __item__ () ଅଚଳ { ଫେରନ୍ତୁ hb_get (f.get (), *it); }
  __item_t__ __item_at__ (unsigned i) ଅଚଳ { ଫେରନ୍ତୁ hb_get (f.get (), it[i]); }
  ତାର୍କିକ __more__ () ଅଚଳ { ଫେରନ୍ତୁ ତାର୍କିକ (it); }
  unsigned __len__ () ଅଚଳ { ଫେରନ୍ତୁ it.len (); }
  ଶୂନ୍ୟ __next__ () { ++it; }
  ଶୂନ୍ୟ __forward__ (unsigned n) { it += n; }
  ଶୂନ୍ୟ __prev__ () { --it; }
  ଶୂନ୍ୟ __rewind__ (unsigned n) { it -= n; }
  hb_map_iter_t __end__ () ଅଚଳ { ଫେରନ୍ତୁ hb_map_iter_t (it._end (), f); }
  ତାର୍କିକ operator != (ଅଚଳ hb_map_iter_t& o) ଅଚଳ
  { ଫେରନ୍ତୁ it != o.it; }

  ବ୍ୟକ୍ତିଗତ:
  Iter it;
  mutable hb_reference_wrapper<Proj> f;
};

template <typename Proj, hb_function_sortedness_t Sorted>
ସଂରଚନା hb_map_iter_factory_t
{
  hb_map_iter_factory_t (Proj f) : f (f) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  hb_map_iter_t<Iter, Proj, Sorted>
  operator () (Iter it)
  { ଫେରନ୍ତୁ hb_map_iter_t<Iter, Proj, Sorted> (it, f); }

  ବ୍ୟକ୍ତିଗତ:
  Proj f;
};
ସଂରଚନା
{
  template <typename Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::NOT_SORTED>
  operator () (Proj&& f) ଅଚଳ
  { ଫେରନ୍ତୁ hb_map_iter_factory_t<Proj, hb_function_sortedness_t::NOT_SORTED> (f); }
}
HB_FUNCOBJ (hb_map);
ସଂରଚନା
{
  template <typename Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::RETAINS_SORTING>
  operator () (Proj&& f) ଅଚଳ
  { ଫେରନ୍ତୁ hb_map_iter_factory_t<Proj, hb_function_sortedness_t::RETAINS_SORTING> (f); }
}
HB_FUNCOBJ (hb_map_retains_sorting);
ସଂରଚନା
{
  template <typename Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::SORTED>
  operator () (Proj&& f) ଅଚଳ
  { ଫେରନ୍ତୁ hb_map_iter_factory_t<Proj, hb_function_sortedness_t::SORTED> (f); }
}
HB_FUNCOBJ (hb_map_sorted);

template <typename Iter, typename Pred, typename Proj,
	 hb_requires (hb_is_iterator (Iter))>
ସଂରଚନା hb_filter_iter_t :
  hb_iter_with_fallback_t<hb_filter_iter_t<Iter, Pred, Proj>,
			  typename Iter::item_t>
{
  hb_filter_iter_t (ଅଚଳ Iter& it_, Pred p_, Proj f_) : it (it_), p (p_), f (f_)
  { ଯେତେବେଳେ (it && !hb_has (p.get (), hb_get (f.get (), *it))) ++it; }

  ପ୍ରକାରବ୍ୟାଖ୍ୟା typename Iter::item_t __item_t__;
  ସ୍ଥିର constexpr ତାର୍କିକ is_sorted_iterator = Iter::is_sorted_iterator;
  __item_t__ __item__ () ଅଚଳ { ଫେରନ୍ତୁ *it; }
  ତାର୍କିକ __more__ () ଅଚଳ { ଫେରନ୍ତୁ ତାର୍କିକ (it); }
  ଶୂନ୍ୟ __next__ () { କର ++it; ଯେତେବେଳେ (it && !hb_has (p.get (), hb_get (f.get (), *it))); }
  ଶୂନ୍ୟ __prev__ ()
  {
    unsigned ଗଣନା_2 = 0;
    କର
    {
      Iter old = it;
      --it;
      ଯଦି (!(it != old))
      {
	it += ଗଣନା_2;
	ଫେରନ୍ତୁ;
      }
      ଗଣନା_2++;
    }
    ଯେତେବେଳେ (!hb_has (p.get (), hb_get (f.get (), *it)));
  }
  hb_filter_iter_t __end__ () ଅଚଳ { ଫେରନ୍ତୁ hb_filter_iter_t (it._end (), p, f); }
  ତାର୍କିକ operator != (ଅଚଳ hb_filter_iter_t& o) ଅଚଳ
  { ଫେରନ୍ତୁ it != o.it; }

  ବ୍ୟକ୍ତିଗତ:
  Iter it;
  mutable hb_reference_wrapper<Pred> p;
  mutable hb_reference_wrapper<Proj> f;
};
template <typename Pred, typename Proj>
ସଂରଚନା hb_filter_iter_factory_t
{
  hb_filter_iter_factory_t (Pred p, Proj f) : p (p), f (f) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  hb_filter_iter_t<Iter, Pred, Proj>
  operator () (Iter it)
  { ଫେରନ୍ତୁ hb_filter_iter_t<Iter, Pred, Proj> (it, p, f); }

  ବ୍ୟକ୍ତିଗତ:
  Pred p;
  Proj f;
};
ସଂରଚନା
{
  template <typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity))>
  hb_filter_iter_factory_t<Pred, Proj>
  operator () (Pred&& p = hb_identity, Proj&& f = hb_identity) ଅଚଳ
  { ଫେରନ୍ତୁ hb_filter_iter_factory_t<Pred, Proj> (p, f); }
}
HB_FUNCOBJ (hb_filter);

template <typename Redu, typename InitT>
ସଂରଚନା hb_reduce_t
{
  hb_reduce_t (Redu r, InitT init_ମୂଲ୍ୟ) : r (r), init_ମୂଲ୍ୟ (init_ମୂଲ୍ୟ) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter)),
	    typename AccuT = hb_decay<decltype (hb_declval (Redu) (hb_declval (InitT), hb_declval (typename Iter::item_t)))>>
  AccuT
  operator () (Iter it)
  {
    AccuT value = init_ମୂଲ୍ୟ;
    ପାଇଁ (; it; ++it)
      value = r (value, *it);
    ଫେରନ୍ତୁ value;
  }

  ବ୍ୟକ୍ତିଗତ:
  Redu r;
  InitT init_ମୂଲ୍ୟ;
};
ସଂରଚନା
{
  template <typename Redu, typename InitT>
  hb_reduce_t<Redu, InitT>
  operator () (Redu&& r, InitT init_ମୂଲ୍ୟ) ଅଚଳ
  { ଫେରନ୍ତୁ hb_reduce_t<Redu, InitT> (r, init_ମୂଲ୍ୟ); }
}
HB_FUNCOBJ (hb_reduce);


/* hb_zip() */

template <typename A, typename B>
ସଂରଚନା hb_zip_iter_t :
  hb_iter_t<hb_zip_iter_t<A, B>,
	    hb_pair_t<typename A::item_t, typename B::item_t>>
{
  hb_zip_iter_t () {}
  hb_zip_iter_t (ଅଚଳ A& a, ଅଚଳ B& b) : a (a), b (b) {}

  ପ୍ରକାରବ୍ୟାଖ୍ୟା hb_pair_t<typename A::item_t, typename B::item_t> __item_t__;
  ସ୍ଥିର constexpr ତାର୍କିକ is_random_access_iterator =
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
  ସ୍ଥିର constexpr ତାର୍କିକ is_sorted_iterator = A::is_sorted_iterator;

  __item_t__ __item__ () ଅଚଳ { ଫେରନ୍ତୁ __item_t__ (*a, *b); }
  __item_t__ __item_at__ (unsigned i) ଅଚଳ { ଫେରନ୍ତୁ __item_t__ (a[i], b[i]); }
  ତାର୍କିକ __more__ () ଅଚଳ { ଫେରନ୍ତୁ ତାର୍କିକ (a) && ତାର୍କିକ (b); }
  unsigned __len__ () ଅଚଳ { ଫେରନ୍ତୁ hb_min (a.len (), b.len ()); }
  ଶୂନ୍ୟ __next__ () { ++a; ++b; }
  ଶୂନ୍ୟ __forward__ (unsigned n) { a += n; b += n; }
  ଶୂନ୍ୟ __prev__ () { --a; --b; }
  ଶୂନ୍ୟ __rewind__ (unsigned n) { a -= n; b -= n; }
  hb_zip_iter_t __end__ () ଅଚଳ { ଫେରନ୍ତୁ hb_zip_iter_t (a._end (), b._end ()); }
  /* Note, we should stop if ANY of the iters reaches end.  As such two compare
   * unequal if both items are unequal, NOT if either is unequal. */
  ତାର୍କିକ operator != (ଅଚଳ hb_zip_iter_t& o) ଅଚଳ
  { ଫେରନ୍ତୁ a != o.a && b != o.b; }

  ବ୍ୟକ୍ତିଗତ:
  A a;
  B b;
};
ସଂରଚନା
{ HB_PARTIALIZE(2);
  template <typename A, typename B,
	    hb_requires (hb_is_iterable (A) && hb_is_iterable (B))>
  hb_zip_iter_t<hb_iter_type<A>, hb_iter_type<B>>
  operator () (A&& a, B&& b) ଅଚଳ
  { ଫେରନ୍ତୁ hb_zip_iter_t<hb_iter_type<A>, hb_iter_type<B>> (hb_iter (a), hb_iter (b)); }
}
HB_FUNCOBJ (hb_zip);

/* hb_concat() */

template <typename A, typename B>
ସଂରଚନା hb_concat_iter_t :
    hb_iter_t<hb_concat_iter_t<A, B>, typename A::item_t>
{
  hb_concat_iter_t () {}
  hb_concat_iter_t (A& a, B& b) : a (a), b (b) {}
  hb_concat_iter_t (ଅଚଳ A& a, ଅଚଳ B& b) : a (a), b (b) {}


  ପ୍ରକାରବ୍ୟାଖ୍ୟା typename A::item_t __item_t__;
  ସ୍ଥିର constexpr ତାର୍କିକ is_random_access_iterator =
    A::is_random_access_iterator &&
    B::is_random_access_iterator;
  ସ୍ଥିର constexpr ତାର୍କିକ is_sorted_iterator = ମିଥ୍ୟା;

  __item_t__ __item__ () ଅଚଳ
  {
    ଯଦି (!a)
      ଫେରନ୍ତୁ *b;
    ଫେରନ୍ତୁ *a;
  }

  __item_t__ __item_at__ (unsigned i) ଅଚଳ
  {
    unsigned a_len = a.len ();
    ଯଦି (i < a_len)
      ଫେରନ୍ତୁ a[i];
    ଫେରନ୍ତୁ b[i - a_len];
  }

  ତାର୍କିକ __more__ () ଅଚଳ { ଫେରନ୍ତୁ ତାର୍କିକ (a) || ତାର୍କିକ (b); }

  unsigned __len__ () ଅଚଳ { ଫେରନ୍ତୁ a.len () + b.len (); }

  ଶୂନ୍ୟ __next__ ()
  {
    ଯଦି (a)
      ++a;
    ଅନ୍ୟଥା
      ++b;
  }

  ଶୂନ୍ୟ __forward__ (unsigned n)
  {
    ଯଦି (!n) ଫେରନ୍ତୁ;
    ଯଦି (!is_random_access_iterator) {
      ଯେତେବେଳେ (n-- && *ଏହି) {
        (*ଏହି)++;
      }
      ଫେରନ୍ତୁ;
    }

    unsigned a_len = a.len ();
    ଯଦି (n > a_len) {
      n -= a_len;
      a.__forward__ (a_len);
      b.__forward__ (n);
    } ଅନ୍ୟଥା {
      a.__forward__ (n);
    }
  }

  hb_concat_iter_t __end__ () ଅଚଳ { ଫେରନ୍ତୁ hb_concat_iter_t (a._end (), b._end ()); }
  ତାର୍କିକ operator != (ଅଚଳ hb_concat_iter_t& o) ଅଚଳ
  {
    ଫେରନ୍ତୁ a != o.a
        || b != o.b;
  }

  ବ୍ୟକ୍ତିଗତ:
  A a;
  B b;
};
ସଂରଚନା
{ HB_PARTIALIZE(2);
  template <typename A, typename B,
	    hb_requires (hb_is_iterable (A) && hb_is_iterable (B))>
  hb_concat_iter_t<hb_iter_type<A>, hb_iter_type<B>>
  operator () (A&& a, B&& b) ଅଚଳ
  { ଫେରନ୍ତୁ hb_concat_iter_t<hb_iter_type<A>, hb_iter_type<B>> (hb_iter (a), hb_iter (b)); }
}
HB_FUNCOBJ (hb_concat);

/* hb_apply() */

template <typename Appl>
ସଂରଚନା hb_apply_t
{
  hb_apply_t (Appl a) : a (a) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  ଶୂନ୍ୟ operator () (Iter it)
  {
    ପାଇଁ (; it; ++it)
      (ଶୂନ୍ୟ) hb_invoke (a, *it);
  }

  ବ୍ୟକ୍ତିଗତ:
  Appl a;
};
ସଂରଚନା
{
  template <typename Appl> hb_apply_t<Appl>
  operator () (Appl&& a) ଅଚଳ
  { ଫେରନ୍ତୁ hb_apply_t<Appl> (a); }

  template <typename Appl> hb_apply_t<Appl&>
  operator () (Appl *a) ଅଚଳ
  { ଫେରନ୍ତୁ hb_apply_t<Appl&> (*a); }
}
HB_FUNCOBJ (hb_apply);

/* hb_range()/hb_iota()/hb_repeat() */

template <typename T, typename S>
ସଂରଚନା hb_range_iter_t :
  hb_iter_t<hb_range_iter_t<T, S>, T>
{
  hb_range_iter_t (T start, T end_, S step) : v (start), end_ (end_for (start, end_, step)), step (step) {}

  ପ୍ରକାରବ୍ୟାଖ୍ୟା T __item_t__;
  ସ୍ଥିର constexpr ତାର୍କିକ is_random_access_iterator = ସତ୍ୟ;
  ସ୍ଥିର constexpr ତାର୍କିକ is_sorted_iterator = ସତ୍ୟ;
  __item_t__ __item__ () ଅଚଳ { ଫେରନ୍ତୁ hb_ridentity (v); }
  __item_t__ __item_at__ (unsigned j) ଅଚଳ { ଫେରନ୍ତୁ v + j * step; }
  ତାର୍କିକ __more__ () ଅଚଳ { ଫେରନ୍ତୁ v != end_; }
  unsigned __len__ () ଅଚଳ { ଫେରନ୍ତୁ !step ? UINT_MAX : (end_ - v) / step; }
  ଶୂନ୍ୟ __next__ () { v += step; }
  ଶୂନ୍ୟ __forward__ (unsigned n) { v += n * step; }
  ଶୂନ୍ୟ __prev__ () { v -= step; }
  ଶୂନ୍ୟ __rewind__ (unsigned n) { v -= n * step; }
  hb_range_iter_t __end__ () ଅଚଳ { ଫେରନ୍ତୁ hb_range_iter_t (end_, end_, step); }
  ତାର୍କିକ operator != (ଅଚଳ hb_range_iter_t& o) ଅଚଳ
  { ଫେରନ୍ତୁ v != o.v; }

  ବ୍ୟକ୍ତିଗତ:
  ସ୍ଥିର ଧାଡ଼ିଭିତରେ T end_for (T start, T end_, S step)
  {
    ଯଦି (!step)
      ଫେରନ୍ତୁ end_;
    auto res = (end_ - start) % step;
    ଯଦି (!res)
      ଫେରନ୍ତୁ end_;
    end_ += step - res;
    ଫେରନ୍ତୁ end_;
  }

  ବ୍ୟକ୍ତିଗତ:
  T v;
  T end_;
  S step;
};
ସଂରଚନା
{
  template <typename T = unsigned> hb_range_iter_t<T, unsigned>
  operator () (T end = (unsigned) -1) ଅଚଳ
  { ଫେରନ୍ତୁ hb_range_iter_t<T, unsigned> (0, end, 1u); }

  template <typename T, typename S = unsigned> hb_range_iter_t<T, S>
  operator () (T start, T end, S step = 1u) ଅଚଳ
  { ଫେରନ୍ତୁ hb_range_iter_t<T, S> (start, end, step); }
}
HB_FUNCOBJ (hb_range);

template <typename T, typename S>
ସଂରଚନା hb_iota_iter_t :
  hb_iter_with_fallback_t<hb_iota_iter_t<T, S>, T>
{
  hb_iota_iter_t (T start, S step) : v (start), step (step) {}

  ବ୍ୟକ୍ତିଗତ:

  template <typename S2 = S>
  auto
  inc (hb_type_identity<S2> s, hb_priority<1>)
    -> hb_void_t<decltype (hb_invoke (std::forward<hb_type_identity<S2>> (s),
                                      hb_declval<T&> ()))>
  { v = hb_invoke (std::forward<hb_type_identity<S2>> (s), v); }

  ଶୂନ୍ୟ
  inc (S s, hb_priority<0>)
  { v += s; }

  ସର୍ବସାଧାରଣ:

  ପ୍ରକାରବ୍ୟାଖ୍ୟା T __item_t__;
  ସ୍ଥିର constexpr ତାର୍କିକ is_random_access_iterator = ସତ୍ୟ;
  ସ୍ଥିର constexpr ତାର୍କିକ is_sorted_iterator = ସତ୍ୟ;
  __item_t__ __item__ () ଅଚଳ { ଫେରନ୍ତୁ hb_ridentity (v); }
  ତାର୍କିକ __more__ () ଅଚଳ { ଫେରନ୍ତୁ ସତ୍ୟ; }
  unsigned __len__ () ଅଚଳ { ଫେରନ୍ତୁ UINT_MAX; }
  ଶୂନ୍ୟ __next__ () { inc (step, hb_prioritize); }
  ଶୂନ୍ୟ __prev__ () { v -= step; }
  hb_iota_iter_t __end__ () ଅଚଳ { ଫେରନ୍ତୁ *ଏହି; }
  ତାର୍କିକ operator != (ଅଚଳ hb_iota_iter_t& o) ଅଚଳ { ଫେରନ୍ତୁ ସତ୍ୟ; }

  ବ୍ୟକ୍ତିଗତ:
  T v;
  S step;
};
ସଂରଚନା
{
  template <typename T = unsigned, typename S = unsigned> hb_iota_iter_t<T, S>
  operator () (T start = 0u, S step = 1u) ଅଚଳ
  { ଫେରନ୍ତୁ hb_iota_iter_t<T, S> (start, step); }
}
HB_FUNCOBJ (hb_iota);

template <typename T>
ସଂରଚନା hb_repeat_iter_t :
  hb_iter_t<hb_repeat_iter_t<T>, T>
{
  hb_repeat_iter_t (T value) : v (value) {}

  ପ୍ରକାରବ୍ୟାଖ୍ୟା T __item_t__;
  ସ୍ଥିର constexpr ତାର୍କିକ is_random_access_iterator = ସତ୍ୟ;
  ସ୍ଥିର constexpr ତାର୍କିକ is_sorted_iterator = ସତ୍ୟ;
  __item_t__ __item__ () ଅଚଳ { ଫେରନ୍ତୁ v; }
  __item_t__ __item_at__ (unsigned j) ଅଚଳ { ଫେରନ୍ତୁ v; }
  ତାର୍କିକ __more__ () ଅଚଳ { ଫେରନ୍ତୁ ସତ୍ୟ; }
  unsigned __len__ () ଅଚଳ { ଫେରନ୍ତୁ UINT_MAX; }
  ଶୂନ୍ୟ __next__ () {}
  ଶୂନ୍ୟ __forward__ (unsigned) {}
  ଶୂନ୍ୟ __prev__ () {}
  ଶୂନ୍ୟ __rewind__ (unsigned) {}
  hb_repeat_iter_t __end__ () ଅଚଳ { ଫେରନ୍ତୁ *ଏହି; }
  ତାର୍କିକ operator != (ଅଚଳ hb_repeat_iter_t& o) ଅଚଳ { ଫେରନ୍ତୁ ସତ୍ୟ; }

  ବ୍ୟକ୍ତିଗତ:
  T v;
};
ସଂରଚନା
{
  template <typename T> hb_repeat_iter_t<T>
  operator () (T value) ଅଚଳ
  { ଫେରନ୍ତୁ hb_repeat_iter_t<T> (value); }
}
HB_FUNCOBJ (hb_repeat);

/* hb_enumerate()/hb_take() */

ସଂରଚନା
{
  template <typename Iterable,
	    typename Index = unsigned,
	    hb_requires (hb_is_iterable (Iterable))>
  auto operator () (Iterable&& it, Index start = 0u) ଅଚଳ HB_AUTO_RETURN
  ( hb_zip (hb_iota (start), it) )
}
HB_FUNCOBJ (hb_enumerate);

ସଂରଚନା
{ HB_PARTIALIZE(2);
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  auto operator () (Iterable&& it, unsigned ଗଣନା_2) ଅଚଳ HB_AUTO_RETURN
  ( hb_zip (hb_range (ଗଣନା_2), it) | hb_map_retains_sorting (hb_second) )

  /* Specialization arrays. */

  template <typename Type> ଧାଡ଼ିଭିତରେ hb_array_t<Type>
  operator () (hb_array_t<Type> array, unsigned ଗଣନା_2) ଅଚଳ
  { ଫେରନ୍ତୁ array.sub_array (0, ଗଣନା_2); }

  template <typename Type> ଧାଡ଼ିଭିତରେ hb_sorted_array_t<Type>
  operator () (hb_sorted_array_t<Type> array, unsigned ଗଣନା_2) ଅଚଳ
  { ଫେରନ୍ତୁ array.sub_array (0, ଗଣନା_2); }
}
HB_FUNCOBJ (hb_take);

ସଂରଚନା
{ HB_PARTIALIZE(2);
  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  auto operator () (Iter it, unsigned ଗଣନା_2) ଅଚଳ HB_AUTO_RETURN
  (
    + hb_iota (it, hb_add (ଗଣନା_2))
    | hb_map (hb_take (ଗଣନା_2))
    | hb_take ((hb_len (it) + ଗଣନା_2 - 1) / ଗଣନା_2)
  )
}
HB_FUNCOBJ (hb_chop);

/* hb_sink() */

template <typename Sink>
ସଂରଚନା hb_sink_t
{
  hb_sink_t (Sink s) : s (s) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  ଶୂନ୍ୟ operator () (Iter it)
  {
    ପାଇଁ (; it; ++it)
      s << *it;
  }

  ବ୍ୟକ୍ତିଗତ:
  Sink s;
};
ସଂରଚନା
{
  template <typename Sink> hb_sink_t<Sink>
  operator () (Sink&& s) ଅଚଳ
  { ଫେରନ୍ତୁ hb_sink_t<Sink> (s); }

  template <typename Sink> hb_sink_t<Sink&>
  operator () (Sink *s) ଅଚଳ
  { ଫେରନ୍ତୁ hb_sink_t<Sink&> (*s); }
}
HB_FUNCOBJ (hb_sink);

/* hb-drain: hb_sink to void / blackhole / /dev/null. */

ସଂରଚନା
{
  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  ଶୂନ୍ୟ operator () (Iter it) ଅଚଳ
  {
    ପାଇଁ (; it; ++it)
      (ଶୂନ୍ୟ) *it;
  }
}
HB_FUNCOBJ (hb_drain);

/* hb_unzip(): unzip and sink to two sinks. */

template <typename Sink1, typename Sink2>
ସଂରଚନା hb_unzip_t
{
  hb_unzip_t (Sink1 s1, Sink2 s2) : s1 (s1), s2 (s2) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  ଶୂନ୍ୟ operator () (Iter it)
  {
    ପାଇଁ (; it; ++it)
    {
      ଅଚଳ auto &v = *it;
      s1 << v.first;
      s2 << v.second;
    }
  }

  ବ୍ୟକ୍ତିଗତ:
  Sink1 s1;
  Sink2 s2;
};
ସଂରଚନା
{
  template <typename Sink1, typename Sink2> hb_unzip_t<Sink1, Sink2>
  operator () (Sink1&& s1, Sink2&& s2) ଅଚଳ
  { ଫେରନ୍ତୁ hb_unzip_t<Sink1, Sink2> (s1, s2); }

  template <typename Sink1, typename Sink2> hb_unzip_t<Sink1&, Sink2&>
  operator () (Sink1 *s1, Sink2 *s2) ଅଚଳ
  { ଫେରନ୍ତୁ hb_unzip_t<Sink1&, Sink2&> (*s1, *s2); }
}
HB_FUNCOBJ (hb_unzip);


/* hb-all, hb-any, hb-none. */

ସଂରଚନା
{
  template <typename Iterable,
	    typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity)),
	    hb_requires (hb_is_iterable (Iterable))>
  ତାର୍କିକ operator () (Iterable&& c,
		    Pred&& p = hb_identity,
		    Proj&& f = hb_identity) ଅଚଳ
  {
    ପାଇଁ (auto it = hb_iter (c); it; ++it)
      ଯଦି (!hb_match (p, hb_get (f, *it)))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }
}
HB_FUNCOBJ (hb_all);
ସଂରଚନା
{
  template <typename Iterable,
	    typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity)),
	    hb_requires (hb_is_iterable (Iterable))>
  ତାର୍କିକ operator () (Iterable&& c,
		    Pred&& p = hb_identity,
		    Proj&& f = hb_identity) ଅଚଳ
  {
    ପାଇଁ (auto it = hb_iter (c); it; ++it)
      ଯଦି (hb_match (p, hb_get (f, *it)))
	ଫେରନ୍ତୁ ସତ୍ୟ;
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }
}
HB_FUNCOBJ (hb_any);
ସଂରଚନା
{
  template <typename Iterable,
	    typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity)),
	    hb_requires (hb_is_iterable (Iterable))>
  ତାର୍କିକ operator () (Iterable&& c,
		    Pred&& p = hb_identity,
		    Proj&& f = hb_identity) ଅଚଳ
  {
    ପାଇଁ (auto it = hb_iter (c); it; ++it)
      ଯଦି (hb_match (p, hb_get (f, *it)))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }
}
HB_FUNCOBJ (hb_none);

/*
 * Algorithms operating on iterators.
 */

template <typename C, typename V,
	  hb_requires (hb_is_iterable (C))>
ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
hb_fill (C&& c, ଅଚଳ V &v)
{
  ପାଇଁ (auto i = hb_iter (c); i; i++)
    *i = v;
}

template <typename S, typename D>
ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
hb_copy (S&& is, D&& id)
{
  hb_iter (is) | hb_sink (id);
}


#endif /* HB_ITER_HH */

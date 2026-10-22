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

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-algs.hh"
#অন্তর্ভাব "hb-meta.hh"


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
কাঠামো hb_iter_t
{
  প্রকারলেখ Item item_t;
  constexpr unsigned get_item_আকার () ধ্রুবক { ফেরত hb_static_আকার (Item); }
  স্থির constexpr যৌক্তিক is_iterator = সত্য;
  স্থির constexpr যৌক্তিক is_random_access_iterator = মিথ্যা;
  স্থির constexpr যৌক্তিক is_sorted_iterator = মিথ্যা;
  স্থির constexpr যৌক্তিক has_fast_len = মিথ্যা; // Should be checked in combination with is_random_access_iterator.

  ব্যক্তিগত:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ধ্রুবক iter_t* thiz () ধ্রুবক { ফেরত static_cast<ধ্রুবক iter_t *> (এই); }
	iter_t* thiz ()       { ফেরত static_cast<      iter_t *> (এই); }
  সর্বজনীন:

  /* Operators. */
  iter_t iter () ধ্রুবক { ফেরত *thiz(); }
  iter_t operator + () ধ্রুবক { ফেরত *thiz(); }
  iter_t _begin () ধ্রুবক { ফেরত *thiz(); }
  iter_t begin () ধ্রুবক { ফেরত _begin (); }
  iter_t _end () ধ্রুবক { ফেরত thiz()->__end__ (); }
  iter_t end () ধ্রুবক { ফেরত _end (); }
  explicit operator যৌক্তিক () ধ্রুবক { ফেরত thiz()->__more__ (); }
  unsigned len () ধ্রুবক { ফেরত thiz()->__len__ (); }
  /* The following can only be enabled if item_t is reference type.  Otherwise
   * it will be returning pointer to temporary rvalue. */
  template <typename T = item_t,
	    hb_enable_if (std::is_reference<T>::value)>
  hb_remove_reference<item_t>* operator -> () ধ্রুবক { ফেরত std::addressof (**thiz()); }
  item_t operator * () ধ্রুবক { ফেরত thiz()->__item__ (); }
  item_t operator * () { ফেরত thiz()->__item__ (); }
  item_t operator [] (unsigned i) ধ্রুবক { ফেরত thiz()->__item_at__ (i); }
  item_t operator [] (unsigned i) { ফেরত thiz()->__item_at__ (i); }
  iter_t& operator += (unsigned গুনতি) &  { thiz()->__forward__ (গুনতি); ফেরত *thiz(); }
  iter_t  operator += (unsigned গুনতি) && { thiz()->__forward__ (গুনতি); ফেরত *thiz(); }
  iter_t& operator ++ () &  { thiz()->__next__ (); ফেরত *thiz(); }
  iter_t  operator ++ () && { thiz()->__next__ (); ফেরত *thiz(); }
  iter_t& operator -= (unsigned গুনতি) &  { thiz()->__rewind__ (গুনতি); ফেরত *thiz(); }
  iter_t  operator -= (unsigned গুনতি) && { thiz()->__rewind__ (গুনতি); ফেরত *thiz(); }
  iter_t& operator -- () &  { thiz()->__prev__ (); ফেরত *thiz(); }
  iter_t  operator -- () && { thiz()->__prev__ (); ফেরত *thiz(); }
  iter_t operator + (unsigned গুনতি) ধ্রুবক { auto c = thiz()->iter (); c += গুনতি; ফেরত c; }
  friend iter_t operator + (unsigned গুনতি, ধ্রুবক iter_t &it) { ফেরত it + গুনতি; }
  iter_t operator ++ (পূর্ণসংখ্যা) { iter_t c (*thiz()); ++*thiz(); ফেরত c; }
  iter_t operator - (unsigned গুনতি) ধ্রুবক { auto c = thiz()->iter (); c -= গুনতি; ফেরত c; }
  iter_t operator -- (পূর্ণসংখ্যা) { iter_t c (*thiz()); --*thiz(); ফেরত c; }
  template <typename T>
  iter_t& operator >> (T &v) &  { v = **thiz(); ++*thiz(); ফেরত *thiz(); }
  template <typename T>
  iter_t  operator >> (T &v) && { v = **thiz(); ++*thiz(); ফেরত *thiz(); }
  template <typename T>
  iter_t& operator << (ধ্রুবক T v) &  { **thiz() = v; ++*thiz(); ফেরত *thiz(); }
  template <typename T>
  iter_t  operator << (ধ্রুবক T v) && { **thiz() = v; ++*thiz(); ফেরত *thiz(); }

  সুরক্ষিত:
  hb_iter_t () = পূর্বনির্ধারিত;
  hb_iter_t (ধ্রুবক hb_iter_t &o HB_UNUSED) = পূর্বনির্ধারিত;
  hb_iter_t (hb_iter_t &&o HB_UNUSED) = পূর্বনির্ধারিত;
  hb_iter_t& operator = (ধ্রুবক hb_iter_t &o HB_UNUSED) = পূর্বনির্ধারিত;
  hb_iter_t& operator = (hb_iter_t &&o HB_UNUSED) = পূর্বনির্ধারিত;
};

#define HB_ITER_USING(Name) \
  using item_t = typename Name::item_t; \
  using Name::_begin; \
  using Name::begin; \
  using Name::_end; \
  using Name::end; \
  using Name::get_item_আকার; \
  using Name::is_iterator; \
  using Name::iter; \
  using Name::operator যৌক্তিক; \
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
  static_assert (সত্য, "")

/* Returns iterator / item type of a type. */
template <typename Iterable>
using hb_iter_type = decltype (hb_deref (hb_declval (Iterable)).iter ());
template <typename Iterable>
using hb_item_type = decltype (*hb_deref (hb_declval (Iterable)).iter ());


template <typename> কাঠামো hb_array_t;
template <typename> কাঠামো hb_sorted_array_t;

কাঠামো
{
  template <typename T> hb_iter_type<T>
  operator () (T&& c) ধ্রুবক
  { ফেরত hb_deref (std::forward<T> (c)).iter (); }

  /* Specialization for C arrays. */

  template <typename Type> অন্তঃরেখ hb_array_t<Type>
  operator () (Type *array, unsigned পূর্ণসংখ্যা দৈর্ঘ্য) ধ্রুবক
  { ফেরত hb_array_t<Type> (array, দৈর্ঘ্য); }

  template <typename Type, unsigned পূর্ণসংখ্যা দৈর্ঘ্য> hb_array_t<Type>
  operator () (Type (&array)[দৈর্ঘ্য]) ধ্রুবক
  { ফেরত hb_array_t<Type> (array, দৈর্ঘ্য); }

}
HB_FUNCOBJ (hb_iter);
কাঠামো
{
  template <typename T> auto
  impl (T&& c, hb_priority<1>) ধ্রুবক HB_RETURN (unsigned, c.len ())

  template <typename T> auto
  impl (T&& c, hb_priority<0>) ধ্রুবক HB_RETURN (unsigned, c.len)

  সর্বজনীন:

  template <typename T> auto
  operator () (T&& c) ধ্রুবক HB_RETURN (unsigned, impl (std::forward<T> (c), hb_prioritize))
}
HB_FUNCOBJ (hb_len);

/* Mixin to fill in what the subclass doesn't provide. */
template <typename iter_t, typename item_t = typename iter_t::__item_t__>
কাঠামো hb_iter_fallback_mixin_t
{
  ব্যক্তিগত:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ধ্রুবক iter_t* thiz () ধ্রুবক { ফেরত static_cast<ধ্রুবক iter_t *> (এই); }
	iter_t* thiz ()       { ফেরত static_cast<      iter_t *> (এই); }
  সর্বজনীন:

  /* Access: Implement __item__(), or __item_at__() if random-access. */
  item_t __item__ () ধ্রুবক { ফেরত (*thiz())[0]; }
  item_t __item_at__ (unsigned i) ধ্রুবক { ফেরত *(*thiz() + i); }

  /* Termination: Implement __more__(), or __len__() if random-access. */
  যৌক্তিক __more__ () ধ্রুবক { ফেরত যৌক্তিক (thiz()->len ()); }
  unsigned __len__ () ধ্রুবক
  { iter_t c (*thiz()); unsigned l = 0; যখন (c) { c++; l++; } ফেরত l; }

  /* Advancing: Implement __next__(), or __forward__() if random-access. */
  শূন্য __next__ () { *thiz() += 1; }
  শূন্য __forward__ (unsigned n) { যখন (*thiz() && n--) ++*thiz(); }

  /* Rewinding: Implement __prev__() or __rewind__() if bidirectional. */
  শূন্য __prev__ () { *thiz() -= 1; }
  শূন্য __rewind__ (unsigned n) { যখন (*thiz() && n--) --*thiz(); }

  /* Range-based for: Implement __end__() if can be done faster,
   * and operator!=. */
  iter_t __end__ () ধ্রুবক
  {
    যদি (thiz()->is_random_access_iterator)
      ফেরত *thiz() + thiz()->len ();
    /* Above expression loops twice. Following loops once. */
    auto it = *thiz();
    যখন (it) ++it;
    ফেরত it;
  }

  সুরক্ষিত:
  hb_iter_fallback_mixin_t () = পূর্বনির্ধারিত;
  hb_iter_fallback_mixin_t (ধ্রুবক hb_iter_fallback_mixin_t &o HB_UNUSED) = পূর্বনির্ধারিত;
  hb_iter_fallback_mixin_t (hb_iter_fallback_mixin_t &&o HB_UNUSED) = পূর্বনির্ধারিত;
  hb_iter_fallback_mixin_t& operator = (ধ্রুবক hb_iter_fallback_mixin_t &o HB_UNUSED) = পূর্বনির্ধারিত;
  hb_iter_fallback_mixin_t& operator = (hb_iter_fallback_mixin_t &&o HB_UNUSED) = পূর্বনির্ধারিত;
};

template <typename iter_t, typename item_t = typename iter_t::__item_t__>
কাঠামো hb_iter_with_fallback_t :
  hb_iter_t<iter_t, item_t>,
  hb_iter_fallback_mixin_t<iter_t, item_t>
{
  সুরক্ষিত:
  hb_iter_with_fallback_t () = পূর্বনির্ধারিত;
  hb_iter_with_fallback_t (ধ্রুবক hb_iter_with_fallback_t &o HB_UNUSED) = পূর্বনির্ধারিত;
  hb_iter_with_fallback_t (hb_iter_with_fallback_t &&o HB_UNUSED) = পূর্বনির্ধারিত;
  hb_iter_with_fallback_t& operator = (ধ্রুবক hb_iter_with_fallback_t &o HB_UNUSED) = পূর্বনির্ধারিত;
  hb_iter_with_fallback_t& operator = (hb_iter_with_fallback_t &&o HB_UNUSED) = পূর্বনির্ধারিত;
};

/*
 * Meta-programming predicates.
 */

/* hb_is_iterator() / hb_is_iterator_of() */

template<typename Iter, typename Item>
কাঠামো hb_is_iterator_of
{
  template <typename Item2 = Item>
  স্থির hb_true_type impl (hb_priority<2>, hb_iter_t<Iter, hb_type_identity<Item2>> *);
  স্থির hb_false_type impl (hb_priority<0>, ধ্রুবক শূন্য *);

  সর্বজনীন:
  স্থির constexpr যৌক্তিক value = decltype (impl (hb_prioritize, hb_declval (Iter*)))::value;
};
#define hb_is_iterator_of(Iter, Item) hb_is_iterator_of<Iter, Item>::value
#define hb_is_iterator(Iter) hb_is_iterator_of (Iter, typename Iter::item_t)
#define hb_is_sorted_iterator_of(Iter, Item) (hb_is_iterator_of<Iter, Item>::value && Iter::is_sorted_iterator)
#define hb_is_sorted_iterator(Iter) hb_is_sorted_iterator_of (Iter, typename Iter::item_t)

/* hb_is_iterable() */

template <typename T>
কাঠামো hb_is_iterable
{
  ব্যক্তিগত:

  template <typename U>
  স্থির auto impl (hb_priority<1>) -> decltype (hb_declval (U).iter (), hb_true_type ());

  template <typename>
  স্থির hb_false_type impl (hb_priority<0>);

  সর্বজনীন:
  স্থির constexpr যৌক্তিক value = decltype (impl<T> (hb_prioritize))::value;
};
#define hb_is_iterable(Iterable) hb_is_iterable<Iterable>::value

/* hb_is_source_of() / hb_is_sink_of() */

template<typename Iter, typename Item>
কাঠামো hb_is_source_of
{
  ব্যক্তিগত:
  template <typename Iter2 = Iter,
	    hb_enable_if (hb_is_convertible (typename Iter2::item_t, hb_add_lvalue_reference<ধ্রুবক Item>))>
  স্থির hb_true_type impl (hb_priority<2>);
  template <typename Iter2 = Iter>
  স্থির auto impl (hb_priority<1>) -> decltype (hb_declval (Iter2) >> hb_declval (Item &), hb_true_type ());
  স্থির hb_false_type impl (hb_priority<0>);

  সর্বজনীন:
  স্থির constexpr যৌক্তিক value = decltype (impl (hb_prioritize))::value;
};
#define hb_is_source_of(Iter, Item) hb_is_source_of<Iter, Item>::value

template<typename Iter, typename Item>
কাঠামো hb_is_sink_of
{
  ব্যক্তিগত:
  template <typename Iter2 = Iter,
	    hb_enable_if (hb_is_convertible (typename Iter2::item_t, hb_add_lvalue_reference<Item>))>
  স্থির hb_true_type impl (hb_priority<2>);
  template <typename Iter2 = Iter>
  স্থির auto impl (hb_priority<1>) -> decltype (hb_declval (Iter2) << hb_declval (Item), hb_true_type ());
  স্থির hb_false_type impl (hb_priority<0>);

  সর্বজনীন:
  স্থির constexpr যৌক্তিক value = decltype (impl (hb_prioritize))::value;
};
#define hb_is_sink_of(Iter, Item) hb_is_sink_of<Iter, Item>::value

/* This is commonly used, so define: */
#define hb_is_sorted_source_of(Iter, Item) \
	(hb_is_source_of(Iter, Item) && Iter::is_sorted_iterator)


কাঠামো
{
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  unsigned operator () (ধ্রুবক Iterable &_) ধ্রুবক { ফেরত hb_len (hb_iter (_)); }

  unsigned operator () (unsigned _) ধ্রুবক { ফেরত _; }
}
HB_FUNCOBJ (hb_len_of);

/* Range-based 'for' for iterables. */

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
স্থির অন্তঃরেখ auto begin (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).begin ())

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
স্থির অন্তঃরেখ auto end (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).end ())

/* begin()/end() are NOT looked up non-ADL.  So each namespace must declare them.
 * Do it for namespace OT. */
নামাকাশ OT {

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
স্থির অন্তঃরেখ auto begin (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).begin ())

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
স্থির অন্তঃরেখ auto end (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).end ())

}


/*
 * Adaptors, combiners, etc.
 */

template <typename Lhs, typename Rhs,
	  hb_requires (hb_is_iterator (Lhs))>
স্থির অন্তঃরেখ auto
operator | (Lhs&& lhs, Rhs&& rhs) HB_AUTO_RETURN (std::forward<Rhs> (rhs) (std::forward<Lhs> (lhs)))

/* hb_map(), hb_filter(), hb_reduce() */

গণনা  শ্রেণী hb_function_sortedness_t {
  NOT_SORTED,
  RETAINS_SORTING,
  SORTED,
};

template <typename Iter, typename Proj, hb_function_sortedness_t Sorted,
	 hb_requires (hb_is_iterator (Iter))>
কাঠামো hb_map_iter_t :
  hb_iter_t<hb_map_iter_t<Iter, Proj, Sorted>,
	    decltype (hb_get (hb_declval (Proj), *hb_declval (Iter)))>
{
  hb_map_iter_t (ধ্রুবক Iter& it, Proj f_) : it (it), f (f_) {}

  প্রকারলেখ decltype (hb_get (hb_declval (Proj), *hb_declval (Iter))) __item_t__;
  স্থির constexpr যৌক্তিক is_random_access_iterator = Iter::is_random_access_iterator;
  স্থির constexpr যৌক্তিক is_sorted_iterator =
    Sorted == hb_function_sortedness_t::SORTED ? সত্য :
    Sorted == hb_function_sortedness_t::RETAINS_SORTING ? Iter::is_sorted_iterator :
    মিথ্যা;
  __item_t__ __item__ () ধ্রুবক { ফেরত hb_get (f.get (), *it); }
  __item_t__ __item_at__ (unsigned i) ধ্রুবক { ফেরত hb_get (f.get (), it[i]); }
  যৌক্তিক __more__ () ধ্রুবক { ফেরত যৌক্তিক (it); }
  unsigned __len__ () ধ্রুবক { ফেরত it.len (); }
  শূন্য __next__ () { ++it; }
  শূন্য __forward__ (unsigned n) { it += n; }
  শূন্য __prev__ () { --it; }
  শূন্য __rewind__ (unsigned n) { it -= n; }
  hb_map_iter_t __end__ () ধ্রুবক { ফেরত hb_map_iter_t (it._end (), f); }
  যৌক্তিক operator != (ধ্রুবক hb_map_iter_t& o) ধ্রুবক
  { ফেরত it != o.it; }

  ব্যক্তিগত:
  Iter it;
  mutable hb_reference_wrapper<Proj> f;
};

template <typename Proj, hb_function_sortedness_t Sorted>
কাঠামো hb_map_iter_factory_t
{
  hb_map_iter_factory_t (Proj f) : f (f) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  hb_map_iter_t<Iter, Proj, Sorted>
  operator () (Iter it)
  { ফেরত hb_map_iter_t<Iter, Proj, Sorted> (it, f); }

  ব্যক্তিগত:
  Proj f;
};
কাঠামো
{
  template <typename Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::NOT_SORTED>
  operator () (Proj&& f) ধ্রুবক
  { ফেরত hb_map_iter_factory_t<Proj, hb_function_sortedness_t::NOT_SORTED> (f); }
}
HB_FUNCOBJ (hb_map);
কাঠামো
{
  template <typename Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::RETAINS_SORTING>
  operator () (Proj&& f) ধ্রুবক
  { ফেরত hb_map_iter_factory_t<Proj, hb_function_sortedness_t::RETAINS_SORTING> (f); }
}
HB_FUNCOBJ (hb_map_retains_sorting);
কাঠামো
{
  template <typename Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::SORTED>
  operator () (Proj&& f) ধ্রুবক
  { ফেরত hb_map_iter_factory_t<Proj, hb_function_sortedness_t::SORTED> (f); }
}
HB_FUNCOBJ (hb_map_sorted);

template <typename Iter, typename Pred, typename Proj,
	 hb_requires (hb_is_iterator (Iter))>
কাঠামো hb_filter_iter_t :
  hb_iter_with_fallback_t<hb_filter_iter_t<Iter, Pred, Proj>,
			  typename Iter::item_t>
{
  hb_filter_iter_t (ধ্রুবক Iter& it_, Pred p_, Proj f_) : it (it_), p (p_), f (f_)
  { যখন (it && !hb_has (p.get (), hb_get (f.get (), *it))) ++it; }

  প্রকারলেখ typename Iter::item_t __item_t__;
  স্থির constexpr যৌক্তিক is_sorted_iterator = Iter::is_sorted_iterator;
  __item_t__ __item__ () ধ্রুবক { ফেরত *it; }
  যৌক্তিক __more__ () ধ্রুবক { ফেরত যৌক্তিক (it); }
  শূন্য __next__ () { করো ++it; যখন (it && !hb_has (p.get (), hb_get (f.get (), *it))); }
  শূন্য __prev__ ()
  {
    unsigned গুনতি = 0;
    করো
    {
      Iter old = it;
      --it;
      যদি (!(it != old))
      {
	it += গুনতি;
	ফেরত;
      }
      গুনতি++;
    }
    যখন (!hb_has (p.get (), hb_get (f.get (), *it)));
  }
  hb_filter_iter_t __end__ () ধ্রুবক { ফেরত hb_filter_iter_t (it._end (), p, f); }
  যৌক্তিক operator != (ধ্রুবক hb_filter_iter_t& o) ধ্রুবক
  { ফেরত it != o.it; }

  ব্যক্তিগত:
  Iter it;
  mutable hb_reference_wrapper<Pred> p;
  mutable hb_reference_wrapper<Proj> f;
};
template <typename Pred, typename Proj>
কাঠামো hb_filter_iter_factory_t
{
  hb_filter_iter_factory_t (Pred p, Proj f) : p (p), f (f) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  hb_filter_iter_t<Iter, Pred, Proj>
  operator () (Iter it)
  { ফেরত hb_filter_iter_t<Iter, Pred, Proj> (it, p, f); }

  ব্যক্তিগত:
  Pred p;
  Proj f;
};
কাঠামো
{
  template <typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity))>
  hb_filter_iter_factory_t<Pred, Proj>
  operator () (Pred&& p = hb_identity, Proj&& f = hb_identity) ধ্রুবক
  { ফেরত hb_filter_iter_factory_t<Pred, Proj> (p, f); }
}
HB_FUNCOBJ (hb_filter);

template <typename Redu, typename InitT>
কাঠামো hb_reduce_t
{
  hb_reduce_t (Redu r, InitT init_মান) : r (r), init_মান (init_মান) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter)),
	    typename AccuT = hb_decay<decltype (hb_declval (Redu) (hb_declval (InitT), hb_declval (typename Iter::item_t)))>>
  AccuT
  operator () (Iter it)
  {
    AccuT value = init_মান;
    জন্য (; it; ++it)
      value = r (value, *it);
    ফেরত value;
  }

  ব্যক্তিগত:
  Redu r;
  InitT init_মান;
};
কাঠামো
{
  template <typename Redu, typename InitT>
  hb_reduce_t<Redu, InitT>
  operator () (Redu&& r, InitT init_মান) ধ্রুবক
  { ফেরত hb_reduce_t<Redu, InitT> (r, init_মান); }
}
HB_FUNCOBJ (hb_reduce);


/* hb_zip() */

template <typename A, typename B>
কাঠামো hb_zip_iter_t :
  hb_iter_t<hb_zip_iter_t<A, B>,
	    hb_pair_t<typename A::item_t, typename B::item_t>>
{
  hb_zip_iter_t () {}
  hb_zip_iter_t (ধ্রুবক A& a, ধ্রুবক B& b) : a (a), b (b) {}

  প্রকারলেখ hb_pair_t<typename A::item_t, typename B::item_t> __item_t__;
  স্থির constexpr যৌক্তিক is_random_access_iterator =
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
  স্থির constexpr যৌক্তিক is_sorted_iterator = A::is_sorted_iterator;

  __item_t__ __item__ () ধ্রুবক { ফেরত __item_t__ (*a, *b); }
  __item_t__ __item_at__ (unsigned i) ধ্রুবক { ফেরত __item_t__ (a[i], b[i]); }
  যৌক্তিক __more__ () ধ্রুবক { ফেরত যৌক্তিক (a) && যৌক্তিক (b); }
  unsigned __len__ () ধ্রুবক { ফেরত hb_min (a.len (), b.len ()); }
  শূন্য __next__ () { ++a; ++b; }
  শূন্য __forward__ (unsigned n) { a += n; b += n; }
  শূন্য __prev__ () { --a; --b; }
  শূন্য __rewind__ (unsigned n) { a -= n; b -= n; }
  hb_zip_iter_t __end__ () ধ্রুবক { ফেরত hb_zip_iter_t (a._end (), b._end ()); }
  /* Note, we should stop if ANY of the iters reaches end.  As such two compare
   * unequal if both items are unequal, NOT if either is unequal. */
  যৌক্তিক operator != (ধ্রুবক hb_zip_iter_t& o) ধ্রুবক
  { ফেরত a != o.a && b != o.b; }

  ব্যক্তিগত:
  A a;
  B b;
};
কাঠামো
{ HB_PARTIALIZE(2);
  template <typename A, typename B,
	    hb_requires (hb_is_iterable (A) && hb_is_iterable (B))>
  hb_zip_iter_t<hb_iter_type<A>, hb_iter_type<B>>
  operator () (A&& a, B&& b) ধ্রুবক
  { ফেরত hb_zip_iter_t<hb_iter_type<A>, hb_iter_type<B>> (hb_iter (a), hb_iter (b)); }
}
HB_FUNCOBJ (hb_zip);

/* hb_concat() */

template <typename A, typename B>
কাঠামো hb_concat_iter_t :
    hb_iter_t<hb_concat_iter_t<A, B>, typename A::item_t>
{
  hb_concat_iter_t () {}
  hb_concat_iter_t (A& a, B& b) : a (a), b (b) {}
  hb_concat_iter_t (ধ্রুবক A& a, ধ্রুবক B& b) : a (a), b (b) {}


  প্রকারলেখ typename A::item_t __item_t__;
  স্থির constexpr যৌক্তিক is_random_access_iterator =
    A::is_random_access_iterator &&
    B::is_random_access_iterator;
  স্থির constexpr যৌক্তিক is_sorted_iterator = মিথ্যা;

  __item_t__ __item__ () ধ্রুবক
  {
    যদি (!a)
      ফেরত *b;
    ফেরত *a;
  }

  __item_t__ __item_at__ (unsigned i) ধ্রুবক
  {
    unsigned a_len = a.len ();
    যদি (i < a_len)
      ফেরত a[i];
    ফেরত b[i - a_len];
  }

  যৌক্তিক __more__ () ধ্রুবক { ফেরত যৌক্তিক (a) || যৌক্তিক (b); }

  unsigned __len__ () ধ্রুবক { ফেরত a.len () + b.len (); }

  শূন্য __next__ ()
  {
    যদি (a)
      ++a;
    অন্যথা
      ++b;
  }

  শূন্য __forward__ (unsigned n)
  {
    যদি (!n) ফেরত;
    যদি (!is_random_access_iterator) {
      যখন (n-- && *এই) {
        (*এই)++;
      }
      ফেরত;
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

  hb_concat_iter_t __end__ () ধ্রুবক { ফেরত hb_concat_iter_t (a._end (), b._end ()); }
  যৌক্তিক operator != (ধ্রুবক hb_concat_iter_t& o) ধ্রুবক
  {
    ফেরত a != o.a
        || b != o.b;
  }

  ব্যক্তিগত:
  A a;
  B b;
};
কাঠামো
{ HB_PARTIALIZE(2);
  template <typename A, typename B,
	    hb_requires (hb_is_iterable (A) && hb_is_iterable (B))>
  hb_concat_iter_t<hb_iter_type<A>, hb_iter_type<B>>
  operator () (A&& a, B&& b) ধ্রুবক
  { ফেরত hb_concat_iter_t<hb_iter_type<A>, hb_iter_type<B>> (hb_iter (a), hb_iter (b)); }
}
HB_FUNCOBJ (hb_concat);

/* hb_apply() */

template <typename Appl>
কাঠামো hb_apply_t
{
  hb_apply_t (Appl a) : a (a) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  শূন্য operator () (Iter it)
  {
    জন্য (; it; ++it)
      (শূন্য) hb_invoke (a, *it);
  }

  ব্যক্তিগত:
  Appl a;
};
কাঠামো
{
  template <typename Appl> hb_apply_t<Appl>
  operator () (Appl&& a) ধ্রুবক
  { ফেরত hb_apply_t<Appl> (a); }

  template <typename Appl> hb_apply_t<Appl&>
  operator () (Appl *a) ধ্রুবক
  { ফেরত hb_apply_t<Appl&> (*a); }
}
HB_FUNCOBJ (hb_apply);

/* hb_range()/hb_iota()/hb_repeat() */

template <typename T, typename S>
কাঠামো hb_range_iter_t :
  hb_iter_t<hb_range_iter_t<T, S>, T>
{
  hb_range_iter_t (T start, T end_, S step) : v (start), end_ (end_for (start, end_, step)), step (step) {}

  প্রকারলেখ T __item_t__;
  স্থির constexpr যৌক্তিক is_random_access_iterator = সত্য;
  স্থির constexpr যৌক্তিক is_sorted_iterator = সত্য;
  __item_t__ __item__ () ধ্রুবক { ফেরত hb_ridentity (v); }
  __item_t__ __item_at__ (unsigned j) ধ্রুবক { ফেরত v + j * step; }
  যৌক্তিক __more__ () ধ্রুবক { ফেরত v != end_; }
  unsigned __len__ () ধ্রুবক { ফেরত !step ? UINT_MAX : (end_ - v) / step; }
  শূন্য __next__ () { v += step; }
  শূন্য __forward__ (unsigned n) { v += n * step; }
  শূন্য __prev__ () { v -= step; }
  শূন্য __rewind__ (unsigned n) { v -= n * step; }
  hb_range_iter_t __end__ () ধ্রুবক { ফেরত hb_range_iter_t (end_, end_, step); }
  যৌক্তিক operator != (ধ্রুবক hb_range_iter_t& o) ধ্রুবক
  { ফেরত v != o.v; }

  ব্যক্তিগত:
  স্থির অন্তঃরেখ T end_for (T start, T end_, S step)
  {
    যদি (!step)
      ফেরত end_;
    auto res = (end_ - start) % step;
    যদি (!res)
      ফেরত end_;
    end_ += step - res;
    ফেরত end_;
  }

  ব্যক্তিগত:
  T v;
  T end_;
  S step;
};
কাঠামো
{
  template <typename T = unsigned> hb_range_iter_t<T, unsigned>
  operator () (T end = (unsigned) -1) ধ্রুবক
  { ফেরত hb_range_iter_t<T, unsigned> (0, end, 1u); }

  template <typename T, typename S = unsigned> hb_range_iter_t<T, S>
  operator () (T start, T end, S step = 1u) ধ্রুবক
  { ফেরত hb_range_iter_t<T, S> (start, end, step); }
}
HB_FUNCOBJ (hb_range);

template <typename T, typename S>
কাঠামো hb_iota_iter_t :
  hb_iter_with_fallback_t<hb_iota_iter_t<T, S>, T>
{
  hb_iota_iter_t (T start, S step) : v (start), step (step) {}

  ব্যক্তিগত:

  template <typename S2 = S>
  auto
  inc (hb_type_identity<S2> s, hb_priority<1>)
    -> hb_void_t<decltype (hb_invoke (std::forward<hb_type_identity<S2>> (s),
                                      hb_declval<T&> ()))>
  { v = hb_invoke (std::forward<hb_type_identity<S2>> (s), v); }

  শূন্য
  inc (S s, hb_priority<0>)
  { v += s; }

  সর্বজনীন:

  প্রকারলেখ T __item_t__;
  স্থির constexpr যৌক্তিক is_random_access_iterator = সত্য;
  স্থির constexpr যৌক্তিক is_sorted_iterator = সত্য;
  __item_t__ __item__ () ধ্রুবক { ফেরত hb_ridentity (v); }
  যৌক্তিক __more__ () ধ্রুবক { ফেরত সত্য; }
  unsigned __len__ () ধ্রুবক { ফেরত UINT_MAX; }
  শূন্য __next__ () { inc (step, hb_prioritize); }
  শূন্য __prev__ () { v -= step; }
  hb_iota_iter_t __end__ () ধ্রুবক { ফেরত *এই; }
  যৌক্তিক operator != (ধ্রুবক hb_iota_iter_t& o) ধ্রুবক { ফেরত সত্য; }

  ব্যক্তিগত:
  T v;
  S step;
};
কাঠামো
{
  template <typename T = unsigned, typename S = unsigned> hb_iota_iter_t<T, S>
  operator () (T start = 0u, S step = 1u) ধ্রুবক
  { ফেরত hb_iota_iter_t<T, S> (start, step); }
}
HB_FUNCOBJ (hb_iota);

template <typename T>
কাঠামো hb_repeat_iter_t :
  hb_iter_t<hb_repeat_iter_t<T>, T>
{
  hb_repeat_iter_t (T value) : v (value) {}

  প্রকারলেখ T __item_t__;
  স্থির constexpr যৌক্তিক is_random_access_iterator = সত্য;
  স্থির constexpr যৌক্তিক is_sorted_iterator = সত্য;
  __item_t__ __item__ () ধ্রুবক { ফেরত v; }
  __item_t__ __item_at__ (unsigned j) ধ্রুবক { ফেরত v; }
  যৌক্তিক __more__ () ধ্রুবক { ফেরত সত্য; }
  unsigned __len__ () ধ্রুবক { ফেরত UINT_MAX; }
  শূন্য __next__ () {}
  শূন্য __forward__ (unsigned) {}
  শূন্য __prev__ () {}
  শূন্য __rewind__ (unsigned) {}
  hb_repeat_iter_t __end__ () ধ্রুবক { ফেরত *এই; }
  যৌক্তিক operator != (ধ্রুবক hb_repeat_iter_t& o) ধ্রুবক { ফেরত সত্য; }

  ব্যক্তিগত:
  T v;
};
কাঠামো
{
  template <typename T> hb_repeat_iter_t<T>
  operator () (T value) ধ্রুবক
  { ফেরত hb_repeat_iter_t<T> (value); }
}
HB_FUNCOBJ (hb_repeat);

/* hb_enumerate()/hb_take() */

কাঠামো
{
  template <typename Iterable,
	    typename সূচক = unsigned,
	    hb_requires (hb_is_iterable (Iterable))>
  auto operator () (Iterable&& it, সূচক start = 0u) ধ্রুবক HB_AUTO_RETURN
  ( hb_zip (hb_iota (start), it) )
}
HB_FUNCOBJ (hb_enumerate);

কাঠামো
{ HB_PARTIALIZE(2);
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  auto operator () (Iterable&& it, unsigned গুনতি) ধ্রুবক HB_AUTO_RETURN
  ( hb_zip (hb_range (গুনতি), it) | hb_map_retains_sorting (hb_second) )

  /* Specialization arrays. */

  template <typename Type> অন্তঃরেখ hb_array_t<Type>
  operator () (hb_array_t<Type> array, unsigned গুনতি) ধ্রুবক
  { ফেরত array.sub_array (0, গুনতি); }

  template <typename Type> অন্তঃরেখ hb_sorted_array_t<Type>
  operator () (hb_sorted_array_t<Type> array, unsigned গুনতি) ধ্রুবক
  { ফেরত array.sub_array (0, গুনতি); }
}
HB_FUNCOBJ (hb_take);

কাঠামো
{ HB_PARTIALIZE(2);
  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  auto operator () (Iter it, unsigned গুনতি) ধ্রুবক HB_AUTO_RETURN
  (
    + hb_iota (it, hb_add (গুনতি))
    | hb_map (hb_take (গুনতি))
    | hb_take ((hb_len (it) + গুনতি - 1) / গুনতি)
  )
}
HB_FUNCOBJ (hb_chop);

/* hb_sink() */

template <typename Sink>
কাঠামো hb_sink_t
{
  hb_sink_t (Sink s) : s (s) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  শূন্য operator () (Iter it)
  {
    জন্য (; it; ++it)
      s << *it;
  }

  ব্যক্তিগত:
  Sink s;
};
কাঠামো
{
  template <typename Sink> hb_sink_t<Sink>
  operator () (Sink&& s) ধ্রুবক
  { ফেরত hb_sink_t<Sink> (s); }

  template <typename Sink> hb_sink_t<Sink&>
  operator () (Sink *s) ধ্রুবক
  { ফেরত hb_sink_t<Sink&> (*s); }
}
HB_FUNCOBJ (hb_sink);

/* hb-drain: hb_sink to void / blackhole / /dev/null. */

কাঠামো
{
  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  শূন্য operator () (Iter it) ধ্রুবক
  {
    জন্য (; it; ++it)
      (শূন্য) *it;
  }
}
HB_FUNCOBJ (hb_drain);

/* hb_unzip(): unzip and sink to two sinks. */

template <typename Sink1, typename Sink2>
কাঠামো hb_unzip_t
{
  hb_unzip_t (Sink1 s1, Sink2 s2) : s1 (s1), s2 (s2) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  শূন্য operator () (Iter it)
  {
    জন্য (; it; ++it)
    {
      ধ্রুবক auto &v = *it;
      s1 << v.first;
      s2 << v.second;
    }
  }

  ব্যক্তিগত:
  Sink1 s1;
  Sink2 s2;
};
কাঠামো
{
  template <typename Sink1, typename Sink2> hb_unzip_t<Sink1, Sink2>
  operator () (Sink1&& s1, Sink2&& s2) ধ্রুবক
  { ফেরত hb_unzip_t<Sink1, Sink2> (s1, s2); }

  template <typename Sink1, typename Sink2> hb_unzip_t<Sink1&, Sink2&>
  operator () (Sink1 *s1, Sink2 *s2) ধ্রুবক
  { ফেরত hb_unzip_t<Sink1&, Sink2&> (*s1, *s2); }
}
HB_FUNCOBJ (hb_unzip);


/* hb-all, hb-any, hb-none. */

কাঠামো
{
  template <typename Iterable,
	    typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity)),
	    hb_requires (hb_is_iterable (Iterable))>
  যৌক্তিক operator () (Iterable&& c,
		    Pred&& p = hb_identity,
		    Proj&& f = hb_identity) ধ্রুবক
  {
    জন্য (auto it = hb_iter (c); it; ++it)
      যদি (!hb_match (p, hb_get (f, *it)))
	ফেরত মিথ্যা;
    ফেরত সত্য;
  }
}
HB_FUNCOBJ (hb_all);
কাঠামো
{
  template <typename Iterable,
	    typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity)),
	    hb_requires (hb_is_iterable (Iterable))>
  যৌক্তিক operator () (Iterable&& c,
		    Pred&& p = hb_identity,
		    Proj&& f = hb_identity) ধ্রুবক
  {
    জন্য (auto it = hb_iter (c); it; ++it)
      যদি (hb_match (p, hb_get (f, *it)))
	ফেরত সত্য;
    ফেরত মিথ্যা;
  }
}
HB_FUNCOBJ (hb_any);
কাঠামো
{
  template <typename Iterable,
	    typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity)),
	    hb_requires (hb_is_iterable (Iterable))>
  যৌক্তিক operator () (Iterable&& c,
		    Pred&& p = hb_identity,
		    Proj&& f = hb_identity) ধ্রুবক
  {
    জন্য (auto it = hb_iter (c); it; ++it)
      যদি (hb_match (p, hb_get (f, *it)))
	ফেরত মিথ্যা;
    ফেরত সত্য;
  }
}
HB_FUNCOBJ (hb_none);

/*
 * Algorithms operating on iterators.
 */

template <typename C, typename V,
	  hb_requires (hb_is_iterable (C))>
অন্তঃরেখ শূন্য
hb_fill (C&& c, ধ্রুবক V &v)
{
  জন্য (auto i = hb_iter (c); i; i++)
    *i = v;
}

template <typename S, typename D>
অন্তঃরেখ শূন্য
hb_copy (S&& is, D&& id)
{
  hb_iter (is) | hb_sink (id);
}


#endif /* HB_ITER_HH */

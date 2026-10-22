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

#ضمن "hb.hh"
#ضمن "hb-algs.hh"
#ضمن "hb-meta.hh"


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
بنية hb_iter_t
{
  تعريفنوع Item item_t;
  constexpr unsigned get_item_size () ثابت { ܦܢܐ hb_static_size (Item); }
  ساكن constexpr منطقي is_iterator = ܫܪܪܐ;
  ساكن constexpr منطقي is_random_access_iterator = ܕܓܠܐ;
  ساكن constexpr منطقي is_sorted_iterator = ܕܓܠܐ;
  ساكن constexpr منطقي has_fast_len = ܕܓܠܐ; // Should be checked in combination with is_random_access_iterator.

  خاص:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ثابت iter_t* thiz () ثابت { ܦܢܐ static_cast<ثابت iter_t *> (هذا); }
	iter_t* thiz ()       { ܦܢܐ static_cast<      iter_t *> (هذا); }
  عام:

  /* Operators. */
  iter_t iter () ثابت { ܦܢܐ *thiz(); }
  iter_t operator + () ثابت { ܦܢܐ *thiz(); }
  iter_t _begin () ثابت { ܦܢܐ *thiz(); }
  iter_t begin () ثابت { ܦܢܐ _begin (); }
  iter_t _end () ثابت { ܦܢܐ thiz()->__end__ (); }
  iter_t end () ثابت { ܦܢܐ _end (); }
  explicit operator منطقي () ثابت { ܦܢܐ thiz()->__more__ (); }
  unsigned len () ثابت { ܦܢܐ thiz()->__len__ (); }
  /* The following can only be enabled if item_t is reference type.  Otherwise
   * it will be returning pointer to temporary rvalue. */
  template <typename T = item_t,
	    hb_enable_if (std::is_reference<T>::value)>
  hb_remove_reference<item_t>* operator -> () ثابت { ܦܢܐ std::addressof (**thiz()); }
  item_t operator * () ثابت { ܦܢܐ thiz()->__item__ (); }
  item_t operator * () { ܦܢܐ thiz()->__item__ (); }
  item_t operator [] (unsigned i) ثابت { ܦܢܐ thiz()->__item_at__ (i); }
  item_t operator [] (unsigned i) { ܦܢܐ thiz()->__item_at__ (i); }
  iter_t& operator += (unsigned count) &  { thiz()->__forward__ (count); ܦܢܐ *thiz(); }
  iter_t  operator += (unsigned count) && { thiz()->__forward__ (count); ܦܢܐ *thiz(); }
  iter_t& operator ++ () &  { thiz()->__next__ (); ܦܢܐ *thiz(); }
  iter_t  operator ++ () && { thiz()->__next__ (); ܦܢܐ *thiz(); }
  iter_t& operator -= (unsigned count) &  { thiz()->__rewind__ (count); ܦܢܐ *thiz(); }
  iter_t  operator -= (unsigned count) && { thiz()->__rewind__ (count); ܦܢܐ *thiz(); }
  iter_t& operator -- () &  { thiz()->__prev__ (); ܦܢܐ *thiz(); }
  iter_t  operator -- () && { thiz()->__prev__ (); ܦܢܐ *thiz(); }
  iter_t operator + (unsigned count) ثابت { auto c = thiz()->iter (); c += count; ܦܢܐ c; }
  friend iter_t operator + (unsigned count, ثابت iter_t &it) { ܦܢܐ it + count; }
  iter_t operator ++ (صحيح) { iter_t c (*thiz()); ++*thiz(); ܦܢܐ c; }
  iter_t operator - (unsigned count) ثابت { auto c = thiz()->iter (); c -= count; ܦܢܐ c; }
  iter_t operator -- (صحيح) { iter_t c (*thiz()); --*thiz(); ܦܢܐ c; }
  template <typename T>
  iter_t& operator >> (T &v) &  { v = **thiz(); ++*thiz(); ܦܢܐ *thiz(); }
  template <typename T>
  iter_t  operator >> (T &v) && { v = **thiz(); ++*thiz(); ܦܢܐ *thiz(); }
  template <typename T>
  iter_t& operator << (ثابت T v) &  { **thiz() = v; ++*thiz(); ܦܢܐ *thiz(); }
  template <typename T>
  iter_t  operator << (ثابت T v) && { **thiz() = v; ++*thiz(); ܦܢܐ *thiz(); }

  محمي:
  hb_iter_t () = افتراضي;
  hb_iter_t (ثابت hb_iter_t &o HB_UNUSED) = افتراضي;
  hb_iter_t (hb_iter_t &&o HB_UNUSED) = افتراضي;
  hb_iter_t& operator = (ثابت hb_iter_t &o HB_UNUSED) = افتراضي;
  hb_iter_t& operator = (hb_iter_t &&o HB_UNUSED) = افتراضي;
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
  using Name::operator منطقي; \
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
  static_assert (ܫܪܪܐ, "")

/* Returns iterator / item type of a type. */
template <typename Iterable>
using hb_iter_type = decltype (hb_deref (hb_declval (Iterable)).iter ());
template <typename Iterable>
using hb_item_type = decltype (*hb_deref (hb_declval (Iterable)).iter ());


template <typename> بنية hb_array_t;
template <typename> بنية hb_sorted_array_t;

بنية
{
  template <typename T> hb_iter_type<T>
  operator () (T&& c) ثابت
  { ܦܢܐ hb_deref (std::forward<T> (c)).iter (); }

  /* Specialization for C arrays. */

  template <typename Type> مضمن hb_array_t<Type>
  operator () (Type *array, unsigned صحيح length) ثابت
  { ܦܢܐ hb_array_t<Type> (array, length); }

  template <typename Type, unsigned صحيح length> hb_array_t<Type>
  operator () (Type (&array)[length]) ثابت
  { ܦܢܐ hb_array_t<Type> (array, length); }

}
HB_FUNCOBJ (hb_iter);
بنية
{
  template <typename T> auto
  impl (T&& c, hb_priority<1>) ثابت HB_RETURN (unsigned, c.len ())

  template <typename T> auto
  impl (T&& c, hb_priority<0>) ثابت HB_RETURN (unsigned, c.len)

  عام:

  template <typename T> auto
  operator () (T&& c) ثابت HB_RETURN (unsigned, impl (std::forward<T> (c), hb_prioritize))
}
HB_FUNCOBJ (hb_len);

/* Mixin to fill in what the subclass doesn't provide. */
template <typename iter_t, typename item_t = typename iter_t::__item_t__>
بنية hb_iter_fallback_mixin_t
{
  خاص:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ثابت iter_t* thiz () ثابت { ܦܢܐ static_cast<ثابت iter_t *> (هذا); }
	iter_t* thiz ()       { ܦܢܐ static_cast<      iter_t *> (هذا); }
  عام:

  /* Access: Implement __item__(), or __item_at__() if random-access. */
  item_t __item__ () ثابت { ܦܢܐ (*thiz())[0]; }
  item_t __item_at__ (unsigned i) ثابت { ܦܢܐ *(*thiz() + i); }

  /* Termination: Implement __more__(), or __len__() if random-access. */
  منطقي __more__ () ثابت { ܦܢܐ منطقي (thiz()->len ()); }
  unsigned __len__ () ثابت
  { iter_t c (*thiz()); unsigned l = 0; ܟܡܐ (c) { c++; l++; } ܦܢܐ l; }

  /* Advancing: Implement __next__(), or __forward__() if random-access. */
  فراغ __next__ () { *thiz() += 1; }
  فراغ __forward__ (unsigned n) { ܟܡܐ (*thiz() && n--) ++*thiz(); }

  /* Rewinding: Implement __prev__() or __rewind__() if bidirectional. */
  فراغ __prev__ () { *thiz() -= 1; }
  فراغ __rewind__ (unsigned n) { ܟܡܐ (*thiz() && n--) --*thiz(); }

  /* Range-based for: Implement __end__() if can be done faster,
   * and operator!=. */
  iter_t __end__ () ثابت
  {
    ܐܢ (thiz()->is_random_access_iterator)
      ܦܢܐ *thiz() + thiz()->len ();
    /* Above expression loops twice. Following loops once. */
    auto it = *thiz();
    ܟܡܐ (it) ++it;
    ܦܢܐ it;
  }

  محمي:
  hb_iter_fallback_mixin_t () = افتراضي;
  hb_iter_fallback_mixin_t (ثابت hb_iter_fallback_mixin_t &o HB_UNUSED) = افتراضي;
  hb_iter_fallback_mixin_t (hb_iter_fallback_mixin_t &&o HB_UNUSED) = افتراضي;
  hb_iter_fallback_mixin_t& operator = (ثابت hb_iter_fallback_mixin_t &o HB_UNUSED) = افتراضي;
  hb_iter_fallback_mixin_t& operator = (hb_iter_fallback_mixin_t &&o HB_UNUSED) = افتراضي;
};

template <typename iter_t, typename item_t = typename iter_t::__item_t__>
بنية hb_iter_with_fallback_t :
  hb_iter_t<iter_t, item_t>,
  hb_iter_fallback_mixin_t<iter_t, item_t>
{
  محمي:
  hb_iter_with_fallback_t () = افتراضي;
  hb_iter_with_fallback_t (ثابت hb_iter_with_fallback_t &o HB_UNUSED) = افتراضي;
  hb_iter_with_fallback_t (hb_iter_with_fallback_t &&o HB_UNUSED) = افتراضي;
  hb_iter_with_fallback_t& operator = (ثابت hb_iter_with_fallback_t &o HB_UNUSED) = افتراضي;
  hb_iter_with_fallback_t& operator = (hb_iter_with_fallback_t &&o HB_UNUSED) = افتراضي;
};

/*
 * Meta-programming predicates.
 */

/* hb_is_iterator() / hb_is_iterator_of() */

template<typename Iter, typename Item>
بنية hb_is_iterator_of
{
  template <typename Item2 = Item>
  ساكن hb_true_type impl (hb_priority<2>, hb_iter_t<Iter, hb_type_identity<Item2>> *);
  ساكن hb_false_type impl (hb_priority<0>, ثابت فراغ *);

  عام:
  ساكن constexpr منطقي value = decltype (impl (hb_prioritize, hb_declval (Iter*)))::value;
};
#define hb_is_iterator_of(Iter, Item) hb_is_iterator_of<Iter, Item>::value
#define hb_is_iterator(Iter) hb_is_iterator_of (Iter, typename Iter::item_t)
#define hb_is_sorted_iterator_of(Iter, Item) (hb_is_iterator_of<Iter, Item>::value && Iter::is_sorted_iterator)
#define hb_is_sorted_iterator(Iter) hb_is_sorted_iterator_of (Iter, typename Iter::item_t)

/* hb_is_iterable() */

template <typename T>
بنية hb_is_iterable
{
  خاص:

  template <typename U>
  ساكن auto impl (hb_priority<1>) -> decltype (hb_declval (U).iter (), hb_true_type ());

  template <typename>
  ساكن hb_false_type impl (hb_priority<0>);

  عام:
  ساكن constexpr منطقي value = decltype (impl<T> (hb_prioritize))::value;
};
#define hb_is_iterable(Iterable) hb_is_iterable<Iterable>::value

/* hb_is_source_of() / hb_is_sink_of() */

template<typename Iter, typename Item>
بنية hb_is_source_of
{
  خاص:
  template <typename Iter2 = Iter,
	    hb_enable_if (hb_is_convertible (typename Iter2::item_t, hb_add_lvalue_reference<ثابت Item>))>
  ساكن hb_true_type impl (hb_priority<2>);
  template <typename Iter2 = Iter>
  ساكن auto impl (hb_priority<1>) -> decltype (hb_declval (Iter2) >> hb_declval (Item &), hb_true_type ());
  ساكن hb_false_type impl (hb_priority<0>);

  عام:
  ساكن constexpr منطقي value = decltype (impl (hb_prioritize))::value;
};
#define hb_is_source_of(Iter, Item) hb_is_source_of<Iter, Item>::value

template<typename Iter, typename Item>
بنية hb_is_sink_of
{
  خاص:
  template <typename Iter2 = Iter,
	    hb_enable_if (hb_is_convertible (typename Iter2::item_t, hb_add_lvalue_reference<Item>))>
  ساكن hb_true_type impl (hb_priority<2>);
  template <typename Iter2 = Iter>
  ساكن auto impl (hb_priority<1>) -> decltype (hb_declval (Iter2) << hb_declval (Item), hb_true_type ());
  ساكن hb_false_type impl (hb_priority<0>);

  عام:
  ساكن constexpr منطقي value = decltype (impl (hb_prioritize))::value;
};
#define hb_is_sink_of(Iter, Item) hb_is_sink_of<Iter, Item>::value

/* This is commonly used, so define: */
#define hb_is_sorted_source_of(Iter, Item) \
	(hb_is_source_of(Iter, Item) && Iter::is_sorted_iterator)


بنية
{
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  unsigned operator () (ثابت Iterable &_) ثابت { ܦܢܐ hb_len (hb_iter (_)); }

  unsigned operator () (unsigned _) ثابت { ܦܢܐ _; }
}
HB_FUNCOBJ (hb_len_of);

/* Range-based 'for' for iterables. */

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ساكن مضمن auto begin (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).begin ())

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ساكن مضمن auto end (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).end ())

/* begin()/end() are NOT looked up non-ADL.  So each namespace must declare them.
 * Do it for namespace OT. */
نطاق OT {

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ساكن مضمن auto begin (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).begin ())

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ساكن مضمن auto end (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).end ())

}


/*
 * Adaptors, combiners, etc.
 */

template <typename Lhs, typename Rhs,
	  hb_requires (hb_is_iterator (Lhs))>
ساكن مضمن auto
operator | (Lhs&& lhs, Rhs&& rhs) HB_AUTO_RETURN (std::forward<Rhs> (rhs) (std::forward<Lhs> (lhs)))

/* hb_map(), hb_filter(), hb_reduce() */

تعداد  صنف hb_function_sortedness_t {
  NOT_SORTED,
  RETAINS_SORTING,
  SORTED,
};

template <typename Iter, typename Proj, hb_function_sortedness_t Sorted,
	 hb_requires (hb_is_iterator (Iter))>
بنية hb_map_iter_t :
  hb_iter_t<hb_map_iter_t<Iter, Proj, Sorted>,
	    decltype (hb_get (hb_declval (Proj), *hb_declval (Iter)))>
{
  hb_map_iter_t (ثابت Iter& it, Proj f_) : it (it), f (f_) {}

  تعريفنوع decltype (hb_get (hb_declval (Proj), *hb_declval (Iter))) __item_t__;
  ساكن constexpr منطقي is_random_access_iterator = Iter::is_random_access_iterator;
  ساكن constexpr منطقي is_sorted_iterator =
    Sorted == hb_function_sortedness_t::SORTED ? ܫܪܪܐ :
    Sorted == hb_function_sortedness_t::RETAINS_SORTING ? Iter::is_sorted_iterator :
    ܕܓܠܐ;
  __item_t__ __item__ () ثابت { ܦܢܐ hb_get (f.get (), *it); }
  __item_t__ __item_at__ (unsigned i) ثابت { ܦܢܐ hb_get (f.get (), it[i]); }
  منطقي __more__ () ثابت { ܦܢܐ منطقي (it); }
  unsigned __len__ () ثابت { ܦܢܐ it.len (); }
  فراغ __next__ () { ++it; }
  فراغ __forward__ (unsigned n) { it += n; }
  فراغ __prev__ () { --it; }
  فراغ __rewind__ (unsigned n) { it -= n; }
  hb_map_iter_t __end__ () ثابت { ܦܢܐ hb_map_iter_t (it._end (), f); }
  منطقي operator != (ثابت hb_map_iter_t& o) ثابت
  { ܦܢܐ it != o.it; }

  خاص:
  Iter it;
  mutable hb_reference_wrapper<Proj> f;
};

template <typename Proj, hb_function_sortedness_t Sorted>
بنية hb_map_iter_factory_t
{
  hb_map_iter_factory_t (Proj f) : f (f) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  hb_map_iter_t<Iter, Proj, Sorted>
  operator () (Iter it)
  { ܦܢܐ hb_map_iter_t<Iter, Proj, Sorted> (it, f); }

  خاص:
  Proj f;
};
بنية
{
  template <typename Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::NOT_SORTED>
  operator () (Proj&& f) ثابت
  { ܦܢܐ hb_map_iter_factory_t<Proj, hb_function_sortedness_t::NOT_SORTED> (f); }
}
HB_FUNCOBJ (hb_map);
بنية
{
  template <typename Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::RETAINS_SORTING>
  operator () (Proj&& f) ثابت
  { ܦܢܐ hb_map_iter_factory_t<Proj, hb_function_sortedness_t::RETAINS_SORTING> (f); }
}
HB_FUNCOBJ (hb_map_retains_sorting);
بنية
{
  template <typename Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::SORTED>
  operator () (Proj&& f) ثابت
  { ܦܢܐ hb_map_iter_factory_t<Proj, hb_function_sortedness_t::SORTED> (f); }
}
HB_FUNCOBJ (hb_map_sorted);

template <typename Iter, typename Pred, typename Proj,
	 hb_requires (hb_is_iterator (Iter))>
بنية hb_filter_iter_t :
  hb_iter_with_fallback_t<hb_filter_iter_t<Iter, Pred, Proj>,
			  typename Iter::item_t>
{
  hb_filter_iter_t (ثابت Iter& it_, Pred p_, Proj f_) : it (it_), p (p_), f (f_)
  { ܟܡܐ (it && !hb_has (p.get (), hb_get (f.get (), *it))) ++it; }

  تعريفنوع typename Iter::item_t __item_t__;
  ساكن constexpr منطقي is_sorted_iterator = Iter::is_sorted_iterator;
  __item_t__ __item__ () ثابت { ܦܢܐ *it; }
  منطقي __more__ () ثابت { ܦܢܐ منطقي (it); }
  فراغ __next__ () { ܥܒܕ ++it; ܟܡܐ (it && !hb_has (p.get (), hb_get (f.get (), *it))); }
  فراغ __prev__ ()
  {
    unsigned count = 0;
    ܥܒܕ
    {
      Iter old = it;
      --it;
      ܐܢ (!(it != old))
      {
	it += count;
	ܦܢܐ;
      }
      count++;
    }
    ܟܡܐ (!hb_has (p.get (), hb_get (f.get (), *it)));
  }
  hb_filter_iter_t __end__ () ثابت { ܦܢܐ hb_filter_iter_t (it._end (), p, f); }
  منطقي operator != (ثابت hb_filter_iter_t& o) ثابت
  { ܦܢܐ it != o.it; }

  خاص:
  Iter it;
  mutable hb_reference_wrapper<Pred> p;
  mutable hb_reference_wrapper<Proj> f;
};
template <typename Pred, typename Proj>
بنية hb_filter_iter_factory_t
{
  hb_filter_iter_factory_t (Pred p, Proj f) : p (p), f (f) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  hb_filter_iter_t<Iter, Pred, Proj>
  operator () (Iter it)
  { ܦܢܐ hb_filter_iter_t<Iter, Pred, Proj> (it, p, f); }

  خاص:
  Pred p;
  Proj f;
};
بنية
{
  template <typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity))>
  hb_filter_iter_factory_t<Pred, Proj>
  operator () (Pred&& p = hb_identity, Proj&& f = hb_identity) ثابت
  { ܦܢܐ hb_filter_iter_factory_t<Pred, Proj> (p, f); }
}
HB_FUNCOBJ (hb_filter);

template <typename Redu, typename InitT>
بنية hb_reduce_t
{
  hb_reduce_t (Redu r, InitT init_value) : r (r), init_value (init_value) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter)),
	    typename AccuT = hb_decay<decltype (hb_declval (Redu) (hb_declval (InitT), hb_declval (typename Iter::item_t)))>>
  AccuT
  operator () (Iter it)
  {
    AccuT value = init_value;
    لكل (; it; ++it)
      value = r (value, *it);
    ܦܢܐ value;
  }

  خاص:
  Redu r;
  InitT init_value;
};
بنية
{
  template <typename Redu, typename InitT>
  hb_reduce_t<Redu, InitT>
  operator () (Redu&& r, InitT init_value) ثابت
  { ܦܢܐ hb_reduce_t<Redu, InitT> (r, init_value); }
}
HB_FUNCOBJ (hb_reduce);


/* hb_zip() */

template <typename A, typename B>
بنية hb_zip_iter_t :
  hb_iter_t<hb_zip_iter_t<A, B>,
	    hb_pair_t<typename A::item_t, typename B::item_t>>
{
  hb_zip_iter_t () {}
  hb_zip_iter_t (ثابت A& a, ثابت B& b) : a (a), b (b) {}

  تعريفنوع hb_pair_t<typename A::item_t, typename B::item_t> __item_t__;
  ساكن constexpr منطقي is_random_access_iterator =
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
  ساكن constexpr منطقي is_sorted_iterator = A::is_sorted_iterator;

  __item_t__ __item__ () ثابت { ܦܢܐ __item_t__ (*a, *b); }
  __item_t__ __item_at__ (unsigned i) ثابت { ܦܢܐ __item_t__ (a[i], b[i]); }
  منطقي __more__ () ثابت { ܦܢܐ منطقي (a) && منطقي (b); }
  unsigned __len__ () ثابت { ܦܢܐ hb_min (a.len (), b.len ()); }
  فراغ __next__ () { ++a; ++b; }
  فراغ __forward__ (unsigned n) { a += n; b += n; }
  فراغ __prev__ () { --a; --b; }
  فراغ __rewind__ (unsigned n) { a -= n; b -= n; }
  hb_zip_iter_t __end__ () ثابت { ܦܢܐ hb_zip_iter_t (a._end (), b._end ()); }
  /* Note, we should stop if ANY of the iters reaches end.  As such two compare
   * unequal if both items are unequal, NOT if either is unequal. */
  منطقي operator != (ثابت hb_zip_iter_t& o) ثابت
  { ܦܢܐ a != o.a && b != o.b; }

  خاص:
  A a;
  B b;
};
بنية
{ HB_PARTIALIZE(2);
  template <typename A, typename B,
	    hb_requires (hb_is_iterable (A) && hb_is_iterable (B))>
  hb_zip_iter_t<hb_iter_type<A>, hb_iter_type<B>>
  operator () (A&& a, B&& b) ثابت
  { ܦܢܐ hb_zip_iter_t<hb_iter_type<A>, hb_iter_type<B>> (hb_iter (a), hb_iter (b)); }
}
HB_FUNCOBJ (hb_zip);

/* hb_concat() */

template <typename A, typename B>
بنية hb_concat_iter_t :
    hb_iter_t<hb_concat_iter_t<A, B>, typename A::item_t>
{
  hb_concat_iter_t () {}
  hb_concat_iter_t (A& a, B& b) : a (a), b (b) {}
  hb_concat_iter_t (ثابت A& a, ثابت B& b) : a (a), b (b) {}


  تعريفنوع typename A::item_t __item_t__;
  ساكن constexpr منطقي is_random_access_iterator =
    A::is_random_access_iterator &&
    B::is_random_access_iterator;
  ساكن constexpr منطقي is_sorted_iterator = ܕܓܠܐ;

  __item_t__ __item__ () ثابت
  {
    ܐܢ (!a)
      ܦܢܐ *b;
    ܦܢܐ *a;
  }

  __item_t__ __item_at__ (unsigned i) ثابت
  {
    unsigned a_len = a.len ();
    ܐܢ (i < a_len)
      ܦܢܐ a[i];
    ܦܢܐ b[i - a_len];
  }

  منطقي __more__ () ثابت { ܦܢܐ منطقي (a) || منطقي (b); }

  unsigned __len__ () ثابت { ܦܢܐ a.len () + b.len (); }

  فراغ __next__ ()
  {
    ܐܢ (a)
      ++a;
    ܐܚܪܢܐ
      ++b;
  }

  فراغ __forward__ (unsigned n)
  {
    ܐܢ (!n) ܦܢܐ;
    ܐܢ (!is_random_access_iterator) {
      ܟܡܐ (n-- && *هذا) {
        (*هذا)++;
      }
      ܦܢܐ;
    }

    unsigned a_len = a.len ();
    ܐܢ (n > a_len) {
      n -= a_len;
      a.__forward__ (a_len);
      b.__forward__ (n);
    } ܐܚܪܢܐ {
      a.__forward__ (n);
    }
  }

  hb_concat_iter_t __end__ () ثابت { ܦܢܐ hb_concat_iter_t (a._end (), b._end ()); }
  منطقي operator != (ثابت hb_concat_iter_t& o) ثابت
  {
    ܦܢܐ a != o.a
        || b != o.b;
  }

  خاص:
  A a;
  B b;
};
بنية
{ HB_PARTIALIZE(2);
  template <typename A, typename B,
	    hb_requires (hb_is_iterable (A) && hb_is_iterable (B))>
  hb_concat_iter_t<hb_iter_type<A>, hb_iter_type<B>>
  operator () (A&& a, B&& b) ثابت
  { ܦܢܐ hb_concat_iter_t<hb_iter_type<A>, hb_iter_type<B>> (hb_iter (a), hb_iter (b)); }
}
HB_FUNCOBJ (hb_concat);

/* hb_apply() */

template <typename Appl>
بنية hb_apply_t
{
  hb_apply_t (Appl a) : a (a) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  فراغ operator () (Iter it)
  {
    لكل (; it; ++it)
      (فراغ) hb_invoke (a, *it);
  }

  خاص:
  Appl a;
};
بنية
{
  template <typename Appl> hb_apply_t<Appl>
  operator () (Appl&& a) ثابت
  { ܦܢܐ hb_apply_t<Appl> (a); }

  template <typename Appl> hb_apply_t<Appl&>
  operator () (Appl *a) ثابت
  { ܦܢܐ hb_apply_t<Appl&> (*a); }
}
HB_FUNCOBJ (hb_apply);

/* hb_range()/hb_iota()/hb_repeat() */

template <typename T, typename S>
بنية hb_range_iter_t :
  hb_iter_t<hb_range_iter_t<T, S>, T>
{
  hb_range_iter_t (T start, T end_, S step) : v (start), end_ (end_for (start, end_, step)), step (step) {}

  تعريفنوع T __item_t__;
  ساكن constexpr منطقي is_random_access_iterator = ܫܪܪܐ;
  ساكن constexpr منطقي is_sorted_iterator = ܫܪܪܐ;
  __item_t__ __item__ () ثابت { ܦܢܐ hb_ridentity (v); }
  __item_t__ __item_at__ (unsigned j) ثابت { ܦܢܐ v + j * step; }
  منطقي __more__ () ثابت { ܦܢܐ v != end_; }
  unsigned __len__ () ثابت { ܦܢܐ !step ? UINT_MAX : (end_ - v) / step; }
  فراغ __next__ () { v += step; }
  فراغ __forward__ (unsigned n) { v += n * step; }
  فراغ __prev__ () { v -= step; }
  فراغ __rewind__ (unsigned n) { v -= n * step; }
  hb_range_iter_t __end__ () ثابت { ܦܢܐ hb_range_iter_t (end_, end_, step); }
  منطقي operator != (ثابت hb_range_iter_t& o) ثابت
  { ܦܢܐ v != o.v; }

  خاص:
  ساكن مضمن T end_for (T start, T end_, S step)
  {
    ܐܢ (!step)
      ܦܢܐ end_;
    auto res = (end_ - start) % step;
    ܐܢ (!res)
      ܦܢܐ end_;
    end_ += step - res;
    ܦܢܐ end_;
  }

  خاص:
  T v;
  T end_;
  S step;
};
بنية
{
  template <typename T = unsigned> hb_range_iter_t<T, unsigned>
  operator () (T end = (unsigned) -1) ثابت
  { ܦܢܐ hb_range_iter_t<T, unsigned> (0, end, 1u); }

  template <typename T, typename S = unsigned> hb_range_iter_t<T, S>
  operator () (T start, T end, S step = 1u) ثابت
  { ܦܢܐ hb_range_iter_t<T, S> (start, end, step); }
}
HB_FUNCOBJ (hb_range);

template <typename T, typename S>
بنية hb_iota_iter_t :
  hb_iter_with_fallback_t<hb_iota_iter_t<T, S>, T>
{
  hb_iota_iter_t (T start, S step) : v (start), step (step) {}

  خاص:

  template <typename S2 = S>
  auto
  inc (hb_type_identity<S2> s, hb_priority<1>)
    -> hb_void_t<decltype (hb_invoke (std::forward<hb_type_identity<S2>> (s),
                                      hb_declval<T&> ()))>
  { v = hb_invoke (std::forward<hb_type_identity<S2>> (s), v); }

  فراغ
  inc (S s, hb_priority<0>)
  { v += s; }

  عام:

  تعريفنوع T __item_t__;
  ساكن constexpr منطقي is_random_access_iterator = ܫܪܪܐ;
  ساكن constexpr منطقي is_sorted_iterator = ܫܪܪܐ;
  __item_t__ __item__ () ثابت { ܦܢܐ hb_ridentity (v); }
  منطقي __more__ () ثابت { ܦܢܐ ܫܪܪܐ; }
  unsigned __len__ () ثابت { ܦܢܐ UINT_MAX; }
  فراغ __next__ () { inc (step, hb_prioritize); }
  فراغ __prev__ () { v -= step; }
  hb_iota_iter_t __end__ () ثابت { ܦܢܐ *هذا; }
  منطقي operator != (ثابت hb_iota_iter_t& o) ثابت { ܦܢܐ ܫܪܪܐ; }

  خاص:
  T v;
  S step;
};
بنية
{
  template <typename T = unsigned, typename S = unsigned> hb_iota_iter_t<T, S>
  operator () (T start = 0u, S step = 1u) ثابت
  { ܦܢܐ hb_iota_iter_t<T, S> (start, step); }
}
HB_FUNCOBJ (hb_iota);

template <typename T>
بنية hb_repeat_iter_t :
  hb_iter_t<hb_repeat_iter_t<T>, T>
{
  hb_repeat_iter_t (T value) : v (value) {}

  تعريفنوع T __item_t__;
  ساكن constexpr منطقي is_random_access_iterator = ܫܪܪܐ;
  ساكن constexpr منطقي is_sorted_iterator = ܫܪܪܐ;
  __item_t__ __item__ () ثابت { ܦܢܐ v; }
  __item_t__ __item_at__ (unsigned j) ثابت { ܦܢܐ v; }
  منطقي __more__ () ثابت { ܦܢܐ ܫܪܪܐ; }
  unsigned __len__ () ثابت { ܦܢܐ UINT_MAX; }
  فراغ __next__ () {}
  فراغ __forward__ (unsigned) {}
  فراغ __prev__ () {}
  فراغ __rewind__ (unsigned) {}
  hb_repeat_iter_t __end__ () ثابت { ܦܢܐ *هذا; }
  منطقي operator != (ثابت hb_repeat_iter_t& o) ثابت { ܦܢܐ ܫܪܪܐ; }

  خاص:
  T v;
};
بنية
{
  template <typename T> hb_repeat_iter_t<T>
  operator () (T value) ثابت
  { ܦܢܐ hb_repeat_iter_t<T> (value); }
}
HB_FUNCOBJ (hb_repeat);

/* hb_enumerate()/hb_take() */

بنية
{
  template <typename Iterable,
	    typename Index = unsigned,
	    hb_requires (hb_is_iterable (Iterable))>
  auto operator () (Iterable&& it, Index start = 0u) ثابت HB_AUTO_RETURN
  ( hb_zip (hb_iota (start), it) )
}
HB_FUNCOBJ (hb_enumerate);

بنية
{ HB_PARTIALIZE(2);
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  auto operator () (Iterable&& it, unsigned count) ثابت HB_AUTO_RETURN
  ( hb_zip (hb_range (count), it) | hb_map_retains_sorting (hb_second) )

  /* Specialization arrays. */

  template <typename Type> مضمن hb_array_t<Type>
  operator () (hb_array_t<Type> array, unsigned count) ثابت
  { ܦܢܐ array.sub_array (0, count); }

  template <typename Type> مضمن hb_sorted_array_t<Type>
  operator () (hb_sorted_array_t<Type> array, unsigned count) ثابت
  { ܦܢܐ array.sub_array (0, count); }
}
HB_FUNCOBJ (hb_take);

بنية
{ HB_PARTIALIZE(2);
  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  auto operator () (Iter it, unsigned count) ثابت HB_AUTO_RETURN
  (
    + hb_iota (it, hb_add (count))
    | hb_map (hb_take (count))
    | hb_take ((hb_len (it) + count - 1) / count)
  )
}
HB_FUNCOBJ (hb_chop);

/* hb_sink() */

template <typename Sink>
بنية hb_sink_t
{
  hb_sink_t (Sink s) : s (s) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  فراغ operator () (Iter it)
  {
    لكل (; it; ++it)
      s << *it;
  }

  خاص:
  Sink s;
};
بنية
{
  template <typename Sink> hb_sink_t<Sink>
  operator () (Sink&& s) ثابت
  { ܦܢܐ hb_sink_t<Sink> (s); }

  template <typename Sink> hb_sink_t<Sink&>
  operator () (Sink *s) ثابت
  { ܦܢܐ hb_sink_t<Sink&> (*s); }
}
HB_FUNCOBJ (hb_sink);

/* hb-drain: hb_sink to void / blackhole / /dev/null. */

بنية
{
  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  فراغ operator () (Iter it) ثابت
  {
    لكل (; it; ++it)
      (فراغ) *it;
  }
}
HB_FUNCOBJ (hb_drain);

/* hb_unzip(): unzip and sink to two sinks. */

template <typename Sink1, typename Sink2>
بنية hb_unzip_t
{
  hb_unzip_t (Sink1 s1, Sink2 s2) : s1 (s1), s2 (s2) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  فراغ operator () (Iter it)
  {
    لكل (; it; ++it)
    {
      ثابت auto &v = *it;
      s1 << v.first;
      s2 << v.second;
    }
  }

  خاص:
  Sink1 s1;
  Sink2 s2;
};
بنية
{
  template <typename Sink1, typename Sink2> hb_unzip_t<Sink1, Sink2>
  operator () (Sink1&& s1, Sink2&& s2) ثابت
  { ܦܢܐ hb_unzip_t<Sink1, Sink2> (s1, s2); }

  template <typename Sink1, typename Sink2> hb_unzip_t<Sink1&, Sink2&>
  operator () (Sink1 *s1, Sink2 *s2) ثابت
  { ܦܢܐ hb_unzip_t<Sink1&, Sink2&> (*s1, *s2); }
}
HB_FUNCOBJ (hb_unzip);


/* hb-all, hb-any, hb-none. */

بنية
{
  template <typename Iterable,
	    typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity)),
	    hb_requires (hb_is_iterable (Iterable))>
  منطقي operator () (Iterable&& c,
		    Pred&& p = hb_identity,
		    Proj&& f = hb_identity) ثابت
  {
    لكل (auto it = hb_iter (c); it; ++it)
      ܐܢ (!hb_match (p, hb_get (f, *it)))
	ܦܢܐ ܕܓܠܐ;
    ܦܢܐ ܫܪܪܐ;
  }
}
HB_FUNCOBJ (hb_all);
بنية
{
  template <typename Iterable,
	    typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity)),
	    hb_requires (hb_is_iterable (Iterable))>
  منطقي operator () (Iterable&& c,
		    Pred&& p = hb_identity,
		    Proj&& f = hb_identity) ثابت
  {
    لكل (auto it = hb_iter (c); it; ++it)
      ܐܢ (hb_match (p, hb_get (f, *it)))
	ܦܢܐ ܫܪܪܐ;
    ܦܢܐ ܕܓܠܐ;
  }
}
HB_FUNCOBJ (hb_any);
بنية
{
  template <typename Iterable,
	    typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity)),
	    hb_requires (hb_is_iterable (Iterable))>
  منطقي operator () (Iterable&& c,
		    Pred&& p = hb_identity,
		    Proj&& f = hb_identity) ثابت
  {
    لكل (auto it = hb_iter (c); it; ++it)
      ܐܢ (hb_match (p, hb_get (f, *it)))
	ܦܢܐ ܕܓܠܐ;
    ܦܢܐ ܫܪܪܐ;
  }
}
HB_FUNCOBJ (hb_none);

/*
 * Algorithms operating on iterators.
 */

template <typename C, typename V,
	  hb_requires (hb_is_iterable (C))>
مضمن فراغ
hb_fill (C&& c, ثابت V &v)
{
  لكل (auto i = hb_iter (c); i; i++)
    *i = v;
}

template <typename S, typename D>
مضمن فراغ
hb_copy (S&& is, D&& id)
{
  hb_iter (is) | hb_sink (id);
}


#endif /* HB_ITER_HH */

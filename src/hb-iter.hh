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

#شامل "hb.hh"
#شامل "hb-algs.hh"
#شامل "hb-meta.hh"


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
ساختار hb_iter_t
{
  تعریفنوع Item item_t;
  constexpr unsigned get_item_size () ثابت { بازگشت hb_static_size (Item); }
  ایستا constexpr منطقی is_iterator = درست;
  ایستا constexpr منطقی is_random_access_iterator = نادرست;
  ایستا constexpr منطقی is_sorted_iterator = نادرست;
  ایستا constexpr منطقی has_fast_len = نادرست; // Should be checked in combination with is_random_access_iterator.

  خصوصی:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ثابت iter_t* thiz () ثابت { بازگشت static_cast<ثابت iter_t *> (این); }
	iter_t* thiz ()       { بازگشت static_cast<      iter_t *> (این); }
  عمومی:

  /* Operators. */
  iter_t iter () ثابت { بازگشت *thiz(); }
  iter_t operator + () ثابت { بازگشت *thiz(); }
  iter_t _begin () ثابت { بازگشت *thiz(); }
  iter_t begin () ثابت { بازگشت _begin (); }
  iter_t _end () ثابت { بازگشت thiz()->__end__ (); }
  iter_t end () ثابت { بازگشت _end (); }
  explicit operator منطقی () ثابت { بازگشت thiz()->__more__ (); }
  unsigned len () ثابت { بازگشت thiz()->__len__ (); }
  /* The following can only be enabled if item_t is reference type.  Otherwise
   * it will be returning pointer to temporary rvalue. */
  template <typename T = item_t,
	    hb_enable_if (std::is_reference<T>::value)>
  hb_remove_reference<item_t>* operator -> () ثابت { بازگشت std::addressof (**thiz()); }
  item_t operator * () ثابت { بازگشت thiz()->__item__ (); }
  item_t operator * () { بازگشت thiz()->__item__ (); }
  item_t operator [] (unsigned i) ثابت { بازگشت thiz()->__item_at__ (i); }
  item_t operator [] (unsigned i) { بازگشت thiz()->__item_at__ (i); }
  iter_t& operator += (unsigned شمار) &  { thiz()->__forward__ (شمار); بازگشت *thiz(); }
  iter_t  operator += (unsigned شمار) && { thiz()->__forward__ (شمار); بازگشت *thiz(); }
  iter_t& operator ++ () &  { thiz()->__next__ (); بازگشت *thiz(); }
  iter_t  operator ++ () && { thiz()->__next__ (); بازگشت *thiz(); }
  iter_t& operator -= (unsigned شمار) &  { thiz()->__rewind__ (شمار); بازگشت *thiz(); }
  iter_t  operator -= (unsigned شمار) && { thiz()->__rewind__ (شمار); بازگشت *thiz(); }
  iter_t& operator -- () &  { thiz()->__prev__ (); بازگشت *thiz(); }
  iter_t  operator -- () && { thiz()->__prev__ (); بازگشت *thiz(); }
  iter_t operator + (unsigned شمار) ثابت { auto c = thiz()->iter (); c += شمار; بازگشت c; }
  friend iter_t operator + (unsigned شمار, ثابت iter_t &it) { بازگشت it + شمار; }
  iter_t operator ++ (صحیح) { iter_t c (*thiz()); ++*thiz(); بازگشت c; }
  iter_t operator - (unsigned شمار) ثابت { auto c = thiz()->iter (); c -= شمار; بازگشت c; }
  iter_t operator -- (صحیح) { iter_t c (*thiz()); --*thiz(); بازگشت c; }
  template <typename T>
  iter_t& operator >> (T &v) &  { v = **thiz(); ++*thiz(); بازگشت *thiz(); }
  template <typename T>
  iter_t  operator >> (T &v) && { v = **thiz(); ++*thiz(); بازگشت *thiz(); }
  template <typename T>
  iter_t& operator << (ثابت T v) &  { **thiz() = v; ++*thiz(); بازگشت *thiz(); }
  template <typename T>
  iter_t  operator << (ثابت T v) && { **thiz() = v; ++*thiz(); بازگشت *thiz(); }

  محافظتشده:
  hb_iter_t () = پیشفرض;
  hb_iter_t (ثابت hb_iter_t &o HB_UNUSED) = پیشفرض;
  hb_iter_t (hb_iter_t &&o HB_UNUSED) = پیشفرض;
  hb_iter_t& operator = (ثابت hb_iter_t &o HB_UNUSED) = پیشفرض;
  hb_iter_t& operator = (hb_iter_t &&o HB_UNUSED) = پیشفرض;
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
  using Name::operator منطقی; \
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
  static_assert (درست, "")

/* Returns iterator / item type of a type. */
template <typename Iterable>
using hb_iter_type = decltype (hb_deref (hb_declval (Iterable)).iter ());
template <typename Iterable>
using hb_item_type = decltype (*hb_deref (hb_declval (Iterable)).iter ());


template <typename> ساختار hb_array_t;
template <typename> ساختار hb_sorted_array_t;

ساختار
{
  template <typename T> hb_iter_type<T>
  operator () (T&& c) ثابت
  { بازگشت hb_deref (std::forward<T> (c)).iter (); }

  /* Specialization for C arrays. */

  template <typename Type> درونخطی hb_array_t<Type>
  operator () (Type *array, unsigned صحیح length) ثابت
  { بازگشت hb_array_t<Type> (array, length); }

  template <typename Type, unsigned صحیح length> hb_array_t<Type>
  operator () (Type (&array)[length]) ثابت
  { بازگشت hb_array_t<Type> (array, length); }

}
HB_FUNCOBJ (hb_iter);
ساختار
{
  template <typename T> auto
  impl (T&& c, hb_priority<1>) ثابت HB_RETURN (unsigned, c.len ())

  template <typename T> auto
  impl (T&& c, hb_priority<0>) ثابت HB_RETURN (unsigned, c.len)

  عمومی:

  template <typename T> auto
  operator () (T&& c) ثابت HB_RETURN (unsigned, impl (std::forward<T> (c), hb_prioritize))
}
HB_FUNCOBJ (hb_len);

/* Mixin to fill in what the subclass doesn't provide. */
template <typename iter_t, typename item_t = typename iter_t::__item_t__>
ساختار hb_iter_fallback_mixin_t
{
  خصوصی:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ثابت iter_t* thiz () ثابت { بازگشت static_cast<ثابت iter_t *> (این); }
	iter_t* thiz ()       { بازگشت static_cast<      iter_t *> (این); }
  عمومی:

  /* Access: Implement __item__(), or __item_at__() if random-access. */
  item_t __item__ () ثابت { بازگشت (*thiz())[0]; }
  item_t __item_at__ (unsigned i) ثابت { بازگشت *(*thiz() + i); }

  /* Termination: Implement __more__(), or __len__() if random-access. */
  منطقی __more__ () ثابت { بازگشت منطقی (thiz()->len ()); }
  unsigned __len__ () ثابت
  { iter_t c (*thiz()); unsigned l = 0; وقتی (c) { c++; l++; } بازگشت l; }

  /* Advancing: Implement __next__(), or __forward__() if random-access. */
  تهی __next__ () { *thiz() += 1; }
  تهی __forward__ (unsigned n) { وقتی (*thiz() && n--) ++*thiz(); }

  /* Rewinding: Implement __prev__() or __rewind__() if bidirectional. */
  تهی __prev__ () { *thiz() -= 1; }
  تهی __rewind__ (unsigned n) { وقتی (*thiz() && n--) --*thiz(); }

  /* Range-based for: Implement __end__() if can be done faster,
   * and operator!=. */
  iter_t __end__ () ثابت
  {
    اگر (thiz()->is_random_access_iterator)
      بازگشت *thiz() + thiz()->len ();
    /* Above expression loops twice. Following loops once. */
    auto it = *thiz();
    وقتی (it) ++it;
    بازگشت it;
  }

  محافظتشده:
  hb_iter_fallback_mixin_t () = پیشفرض;
  hb_iter_fallback_mixin_t (ثابت hb_iter_fallback_mixin_t &o HB_UNUSED) = پیشفرض;
  hb_iter_fallback_mixin_t (hb_iter_fallback_mixin_t &&o HB_UNUSED) = پیشفرض;
  hb_iter_fallback_mixin_t& operator = (ثابت hb_iter_fallback_mixin_t &o HB_UNUSED) = پیشفرض;
  hb_iter_fallback_mixin_t& operator = (hb_iter_fallback_mixin_t &&o HB_UNUSED) = پیشفرض;
};

template <typename iter_t, typename item_t = typename iter_t::__item_t__>
ساختار hb_iter_with_fallback_t :
  hb_iter_t<iter_t, item_t>,
  hb_iter_fallback_mixin_t<iter_t, item_t>
{
  محافظتشده:
  hb_iter_with_fallback_t () = پیشفرض;
  hb_iter_with_fallback_t (ثابت hb_iter_with_fallback_t &o HB_UNUSED) = پیشفرض;
  hb_iter_with_fallback_t (hb_iter_with_fallback_t &&o HB_UNUSED) = پیشفرض;
  hb_iter_with_fallback_t& operator = (ثابت hb_iter_with_fallback_t &o HB_UNUSED) = پیشفرض;
  hb_iter_with_fallback_t& operator = (hb_iter_with_fallback_t &&o HB_UNUSED) = پیشفرض;
};

/*
 * Meta-programming predicates.
 */

/* hb_is_iterator() / hb_is_iterator_of() */

template<typename Iter, typename Item>
ساختار hb_is_iterator_of
{
  template <typename Item2 = Item>
  ایستا hb_true_type impl (hb_priority<2>, hb_iter_t<Iter, hb_type_identity<Item2>> *);
  ایستا hb_false_type impl (hb_priority<0>, ثابت تهی *);

  عمومی:
  ایستا constexpr منطقی value = decltype (impl (hb_prioritize, hb_declval (Iter*)))::value;
};
#define hb_is_iterator_of(Iter, Item) hb_is_iterator_of<Iter, Item>::value
#define hb_is_iterator(Iter) hb_is_iterator_of (Iter, typename Iter::item_t)
#define hb_is_sorted_iterator_of(Iter, Item) (hb_is_iterator_of<Iter, Item>::value && Iter::is_sorted_iterator)
#define hb_is_sorted_iterator(Iter) hb_is_sorted_iterator_of (Iter, typename Iter::item_t)

/* hb_is_iterable() */

template <typename T>
ساختار hb_is_iterable
{
  خصوصی:

  template <typename U>
  ایستا auto impl (hb_priority<1>) -> decltype (hb_declval (U).iter (), hb_true_type ());

  template <typename>
  ایستا hb_false_type impl (hb_priority<0>);

  عمومی:
  ایستا constexpr منطقی value = decltype (impl<T> (hb_prioritize))::value;
};
#define hb_is_iterable(Iterable) hb_is_iterable<Iterable>::value

/* hb_is_source_of() / hb_is_sink_of() */

template<typename Iter, typename Item>
ساختار hb_is_source_of
{
  خصوصی:
  template <typename Iter2 = Iter,
	    hb_enable_if (hb_is_convertible (typename Iter2::item_t, hb_add_lvalue_reference<ثابت Item>))>
  ایستا hb_true_type impl (hb_priority<2>);
  template <typename Iter2 = Iter>
  ایستا auto impl (hb_priority<1>) -> decltype (hb_declval (Iter2) >> hb_declval (Item &), hb_true_type ());
  ایستا hb_false_type impl (hb_priority<0>);

  عمومی:
  ایستا constexpr منطقی value = decltype (impl (hb_prioritize))::value;
};
#define hb_is_source_of(Iter, Item) hb_is_source_of<Iter, Item>::value

template<typename Iter, typename Item>
ساختار hb_is_sink_of
{
  خصوصی:
  template <typename Iter2 = Iter,
	    hb_enable_if (hb_is_convertible (typename Iter2::item_t, hb_add_lvalue_reference<Item>))>
  ایستا hb_true_type impl (hb_priority<2>);
  template <typename Iter2 = Iter>
  ایستا auto impl (hb_priority<1>) -> decltype (hb_declval (Iter2) << hb_declval (Item), hb_true_type ());
  ایستا hb_false_type impl (hb_priority<0>);

  عمومی:
  ایستا constexpr منطقی value = decltype (impl (hb_prioritize))::value;
};
#define hb_is_sink_of(Iter, Item) hb_is_sink_of<Iter, Item>::value

/* This is commonly used, so define: */
#define hb_is_sorted_source_of(Iter, Item) \
	(hb_is_source_of(Iter, Item) && Iter::is_sorted_iterator)


ساختار
{
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  unsigned operator () (ثابت Iterable &_) ثابت { بازگشت hb_len (hb_iter (_)); }

  unsigned operator () (unsigned _) ثابت { بازگشت _; }
}
HB_FUNCOBJ (hb_len_of);

/* Range-based 'for' for iterables. */

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ایستا درونخطی auto begin (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).begin ())

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ایستا درونخطی auto end (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).end ())

/* begin()/end() are NOT looked up non-ADL.  So each namespace must declare them.
 * Do it for namespace OT. */
فضاینام OT {

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ایستا درونخطی auto begin (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).begin ())

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ایستا درونخطی auto end (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).end ())

}


/*
 * Adaptors, combiners, etc.
 */

template <typename Lhs, typename Rhs,
	  hb_requires (hb_is_iterator (Lhs))>
ایستا درونخطی auto
operator | (Lhs&& lhs, Rhs&& rhs) HB_AUTO_RETURN (std::forward<Rhs> (rhs) (std::forward<Lhs> (lhs)))

/* hb_map(), hb_filter(), hb_reduce() */

شمارش  رده hb_function_sortedness_t {
  NOT_SORTED,
  RETAINS_SORTING,
  SORTED,
};

template <typename Iter, typename Proj, hb_function_sortedness_t Sorted,
	 hb_requires (hb_is_iterator (Iter))>
ساختار hb_map_iter_t :
  hb_iter_t<hb_map_iter_t<Iter, Proj, Sorted>,
	    decltype (hb_get (hb_declval (Proj), *hb_declval (Iter)))>
{
  hb_map_iter_t (ثابت Iter& it, Proj f_) : it (it), f (f_) {}

  تعریفنوع decltype (hb_get (hb_declval (Proj), *hb_declval (Iter))) __item_t__;
  ایستا constexpr منطقی is_random_access_iterator = Iter::is_random_access_iterator;
  ایستا constexpr منطقی is_sorted_iterator =
    Sorted == hb_function_sortedness_t::SORTED ? درست :
    Sorted == hb_function_sortedness_t::RETAINS_SORTING ? Iter::is_sorted_iterator :
    نادرست;
  __item_t__ __item__ () ثابت { بازگشت hb_get (f.get (), *it); }
  __item_t__ __item_at__ (unsigned i) ثابت { بازگشت hb_get (f.get (), it[i]); }
  منطقی __more__ () ثابت { بازگشت منطقی (it); }
  unsigned __len__ () ثابت { بازگشت it.len (); }
  تهی __next__ () { ++it; }
  تهی __forward__ (unsigned n) { it += n; }
  تهی __prev__ () { --it; }
  تهی __rewind__ (unsigned n) { it -= n; }
  hb_map_iter_t __end__ () ثابت { بازگشت hb_map_iter_t (it._end (), f); }
  منطقی operator != (ثابت hb_map_iter_t& o) ثابت
  { بازگشت it != o.it; }

  خصوصی:
  Iter it;
  mutable hb_reference_wrapper<Proj> f;
};

template <typename Proj, hb_function_sortedness_t Sorted>
ساختار hb_map_iter_factory_t
{
  hb_map_iter_factory_t (Proj f) : f (f) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  hb_map_iter_t<Iter, Proj, Sorted>
  operator () (Iter it)
  { بازگشت hb_map_iter_t<Iter, Proj, Sorted> (it, f); }

  خصوصی:
  Proj f;
};
ساختار
{
  template <typename Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::NOT_SORTED>
  operator () (Proj&& f) ثابت
  { بازگشت hb_map_iter_factory_t<Proj, hb_function_sortedness_t::NOT_SORTED> (f); }
}
HB_FUNCOBJ (hb_map);
ساختار
{
  template <typename Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::RETAINS_SORTING>
  operator () (Proj&& f) ثابت
  { بازگشت hb_map_iter_factory_t<Proj, hb_function_sortedness_t::RETAINS_SORTING> (f); }
}
HB_FUNCOBJ (hb_map_retains_sorting);
ساختار
{
  template <typename Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::SORTED>
  operator () (Proj&& f) ثابت
  { بازگشت hb_map_iter_factory_t<Proj, hb_function_sortedness_t::SORTED> (f); }
}
HB_FUNCOBJ (hb_map_sorted);

template <typename Iter, typename Pred, typename Proj,
	 hb_requires (hb_is_iterator (Iter))>
ساختار hb_filter_iter_t :
  hb_iter_with_fallback_t<hb_filter_iter_t<Iter, Pred, Proj>,
			  typename Iter::item_t>
{
  hb_filter_iter_t (ثابت Iter& it_, Pred p_, Proj f_) : it (it_), p (p_), f (f_)
  { وقتی (it && !hb_has (p.get (), hb_get (f.get (), *it))) ++it; }

  تعریفنوع typename Iter::item_t __item_t__;
  ایستا constexpr منطقی is_sorted_iterator = Iter::is_sorted_iterator;
  __item_t__ __item__ () ثابت { بازگشت *it; }
  منطقی __more__ () ثابت { بازگشت منطقی (it); }
  تهی __next__ () { انجامبده ++it; وقتی (it && !hb_has (p.get (), hb_get (f.get (), *it))); }
  تهی __prev__ ()
  {
    unsigned شمار = 0;
    انجامبده
    {
      Iter old = it;
      --it;
      اگر (!(it != old))
      {
	it += شمار;
	بازگشت;
      }
      شمار++;
    }
    وقتی (!hb_has (p.get (), hb_get (f.get (), *it)));
  }
  hb_filter_iter_t __end__ () ثابت { بازگشت hb_filter_iter_t (it._end (), p, f); }
  منطقی operator != (ثابت hb_filter_iter_t& o) ثابت
  { بازگشت it != o.it; }

  خصوصی:
  Iter it;
  mutable hb_reference_wrapper<Pred> p;
  mutable hb_reference_wrapper<Proj> f;
};
template <typename Pred, typename Proj>
ساختار hb_filter_iter_factory_t
{
  hb_filter_iter_factory_t (Pred p, Proj f) : p (p), f (f) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  hb_filter_iter_t<Iter, Pred, Proj>
  operator () (Iter it)
  { بازگشت hb_filter_iter_t<Iter, Pred, Proj> (it, p, f); }

  خصوصی:
  Pred p;
  Proj f;
};
ساختار
{
  template <typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity))>
  hb_filter_iter_factory_t<Pred, Proj>
  operator () (Pred&& p = hb_identity, Proj&& f = hb_identity) ثابت
  { بازگشت hb_filter_iter_factory_t<Pred, Proj> (p, f); }
}
HB_FUNCOBJ (hb_filter);

template <typename Redu, typename InitT>
ساختار hb_reduce_t
{
  hb_reduce_t (Redu r, InitT init_مقدار) : r (r), init_مقدار (init_مقدار) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter)),
	    typename AccuT = hb_decay<decltype (hb_declval (Redu) (hb_declval (InitT), hb_declval (typename Iter::item_t)))>>
  AccuT
  operator () (Iter it)
  {
    AccuT value = init_مقدار;
    برای (; it; ++it)
      value = r (value, *it);
    بازگشت value;
  }

  خصوصی:
  Redu r;
  InitT init_مقدار;
};
ساختار
{
  template <typename Redu, typename InitT>
  hb_reduce_t<Redu, InitT>
  operator () (Redu&& r, InitT init_مقدار) ثابت
  { بازگشت hb_reduce_t<Redu, InitT> (r, init_مقدار); }
}
HB_FUNCOBJ (hb_reduce);


/* hb_zip() */

template <typename A, typename B>
ساختار hb_zip_iter_t :
  hb_iter_t<hb_zip_iter_t<A, B>,
	    hb_pair_t<typename A::item_t, typename B::item_t>>
{
  hb_zip_iter_t () {}
  hb_zip_iter_t (ثابت A& a, ثابت B& b) : a (a), b (b) {}

  تعریفنوع hb_pair_t<typename A::item_t, typename B::item_t> __item_t__;
  ایستا constexpr منطقی is_random_access_iterator =
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
  ایستا constexpr منطقی is_sorted_iterator = A::is_sorted_iterator;

  __item_t__ __item__ () ثابت { بازگشت __item_t__ (*a, *b); }
  __item_t__ __item_at__ (unsigned i) ثابت { بازگشت __item_t__ (a[i], b[i]); }
  منطقی __more__ () ثابت { بازگشت منطقی (a) && منطقی (b); }
  unsigned __len__ () ثابت { بازگشت hb_min (a.len (), b.len ()); }
  تهی __next__ () { ++a; ++b; }
  تهی __forward__ (unsigned n) { a += n; b += n; }
  تهی __prev__ () { --a; --b; }
  تهی __rewind__ (unsigned n) { a -= n; b -= n; }
  hb_zip_iter_t __end__ () ثابت { بازگشت hb_zip_iter_t (a._end (), b._end ()); }
  /* Note, we should stop if ANY of the iters reaches end.  As such two compare
   * unequal if both items are unequal, NOT if either is unequal. */
  منطقی operator != (ثابت hb_zip_iter_t& o) ثابت
  { بازگشت a != o.a && b != o.b; }

  خصوصی:
  A a;
  B b;
};
ساختار
{ HB_PARTIALIZE(2);
  template <typename A, typename B,
	    hb_requires (hb_is_iterable (A) && hb_is_iterable (B))>
  hb_zip_iter_t<hb_iter_type<A>, hb_iter_type<B>>
  operator () (A&& a, B&& b) ثابت
  { بازگشت hb_zip_iter_t<hb_iter_type<A>, hb_iter_type<B>> (hb_iter (a), hb_iter (b)); }
}
HB_FUNCOBJ (hb_zip);

/* hb_concat() */

template <typename A, typename B>
ساختار hb_concat_iter_t :
    hb_iter_t<hb_concat_iter_t<A, B>, typename A::item_t>
{
  hb_concat_iter_t () {}
  hb_concat_iter_t (A& a, B& b) : a (a), b (b) {}
  hb_concat_iter_t (ثابت A& a, ثابت B& b) : a (a), b (b) {}


  تعریفنوع typename A::item_t __item_t__;
  ایستا constexpr منطقی is_random_access_iterator =
    A::is_random_access_iterator &&
    B::is_random_access_iterator;
  ایستا constexpr منطقی is_sorted_iterator = نادرست;

  __item_t__ __item__ () ثابت
  {
    اگر (!a)
      بازگشت *b;
    بازگشت *a;
  }

  __item_t__ __item_at__ (unsigned i) ثابت
  {
    unsigned a_len = a.len ();
    اگر (i < a_len)
      بازگشت a[i];
    بازگشت b[i - a_len];
  }

  منطقی __more__ () ثابت { بازگشت منطقی (a) || منطقی (b); }

  unsigned __len__ () ثابت { بازگشت a.len () + b.len (); }

  تهی __next__ ()
  {
    اگر (a)
      ++a;
    وگرنه
      ++b;
  }

  تهی __forward__ (unsigned n)
  {
    اگر (!n) بازگشت;
    اگر (!is_random_access_iterator) {
      وقتی (n-- && *این) {
        (*این)++;
      }
      بازگشت;
    }

    unsigned a_len = a.len ();
    اگر (n > a_len) {
      n -= a_len;
      a.__forward__ (a_len);
      b.__forward__ (n);
    } وگرنه {
      a.__forward__ (n);
    }
  }

  hb_concat_iter_t __end__ () ثابت { بازگشت hb_concat_iter_t (a._end (), b._end ()); }
  منطقی operator != (ثابت hb_concat_iter_t& o) ثابت
  {
    بازگشت a != o.a
        || b != o.b;
  }

  خصوصی:
  A a;
  B b;
};
ساختار
{ HB_PARTIALIZE(2);
  template <typename A, typename B,
	    hb_requires (hb_is_iterable (A) && hb_is_iterable (B))>
  hb_concat_iter_t<hb_iter_type<A>, hb_iter_type<B>>
  operator () (A&& a, B&& b) ثابت
  { بازگشت hb_concat_iter_t<hb_iter_type<A>, hb_iter_type<B>> (hb_iter (a), hb_iter (b)); }
}
HB_FUNCOBJ (hb_concat);

/* hb_apply() */

template <typename Appl>
ساختار hb_apply_t
{
  hb_apply_t (Appl a) : a (a) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  تهی operator () (Iter it)
  {
    برای (; it; ++it)
      (تهی) hb_invoke (a, *it);
  }

  خصوصی:
  Appl a;
};
ساختار
{
  template <typename Appl> hb_apply_t<Appl>
  operator () (Appl&& a) ثابت
  { بازگشت hb_apply_t<Appl> (a); }

  template <typename Appl> hb_apply_t<Appl&>
  operator () (Appl *a) ثابت
  { بازگشت hb_apply_t<Appl&> (*a); }
}
HB_FUNCOBJ (hb_apply);

/* hb_range()/hb_iota()/hb_repeat() */

template <typename T, typename S>
ساختار hb_range_iter_t :
  hb_iter_t<hb_range_iter_t<T, S>, T>
{
  hb_range_iter_t (T start, T end_, S step) : v (start), end_ (end_for (start, end_, step)), step (step) {}

  تعریفنوع T __item_t__;
  ایستا constexpr منطقی is_random_access_iterator = درست;
  ایستا constexpr منطقی is_sorted_iterator = درست;
  __item_t__ __item__ () ثابت { بازگشت hb_ridentity (v); }
  __item_t__ __item_at__ (unsigned j) ثابت { بازگشت v + j * step; }
  منطقی __more__ () ثابت { بازگشت v != end_; }
  unsigned __len__ () ثابت { بازگشت !step ? UINT_MAX : (end_ - v) / step; }
  تهی __next__ () { v += step; }
  تهی __forward__ (unsigned n) { v += n * step; }
  تهی __prev__ () { v -= step; }
  تهی __rewind__ (unsigned n) { v -= n * step; }
  hb_range_iter_t __end__ () ثابت { بازگشت hb_range_iter_t (end_, end_, step); }
  منطقی operator != (ثابت hb_range_iter_t& o) ثابت
  { بازگشت v != o.v; }

  خصوصی:
  ایستا درونخطی T end_for (T start, T end_, S step)
  {
    اگر (!step)
      بازگشت end_;
    auto res = (end_ - start) % step;
    اگر (!res)
      بازگشت end_;
    end_ += step - res;
    بازگشت end_;
  }

  خصوصی:
  T v;
  T end_;
  S step;
};
ساختار
{
  template <typename T = unsigned> hb_range_iter_t<T, unsigned>
  operator () (T end = (unsigned) -1) ثابت
  { بازگشت hb_range_iter_t<T, unsigned> (0, end, 1u); }

  template <typename T, typename S = unsigned> hb_range_iter_t<T, S>
  operator () (T start, T end, S step = 1u) ثابت
  { بازگشت hb_range_iter_t<T, S> (start, end, step); }
}
HB_FUNCOBJ (hb_range);

template <typename T, typename S>
ساختار hb_iota_iter_t :
  hb_iter_with_fallback_t<hb_iota_iter_t<T, S>, T>
{
  hb_iota_iter_t (T start, S step) : v (start), step (step) {}

  خصوصی:

  template <typename S2 = S>
  auto
  inc (hb_type_identity<S2> s, hb_priority<1>)
    -> hb_void_t<decltype (hb_invoke (std::forward<hb_type_identity<S2>> (s),
                                      hb_declval<T&> ()))>
  { v = hb_invoke (std::forward<hb_type_identity<S2>> (s), v); }

  تهی
  inc (S s, hb_priority<0>)
  { v += s; }

  عمومی:

  تعریفنوع T __item_t__;
  ایستا constexpr منطقی is_random_access_iterator = درست;
  ایستا constexpr منطقی is_sorted_iterator = درست;
  __item_t__ __item__ () ثابت { بازگشت hb_ridentity (v); }
  منطقی __more__ () ثابت { بازگشت درست; }
  unsigned __len__ () ثابت { بازگشت UINT_MAX; }
  تهی __next__ () { inc (step, hb_prioritize); }
  تهی __prev__ () { v -= step; }
  hb_iota_iter_t __end__ () ثابت { بازگشت *این; }
  منطقی operator != (ثابت hb_iota_iter_t& o) ثابت { بازگشت درست; }

  خصوصی:
  T v;
  S step;
};
ساختار
{
  template <typename T = unsigned, typename S = unsigned> hb_iota_iter_t<T, S>
  operator () (T start = 0u, S step = 1u) ثابت
  { بازگشت hb_iota_iter_t<T, S> (start, step); }
}
HB_FUNCOBJ (hb_iota);

template <typename T>
ساختار hb_repeat_iter_t :
  hb_iter_t<hb_repeat_iter_t<T>, T>
{
  hb_repeat_iter_t (T value) : v (value) {}

  تعریفنوع T __item_t__;
  ایستا constexpr منطقی is_random_access_iterator = درست;
  ایستا constexpr منطقی is_sorted_iterator = درست;
  __item_t__ __item__ () ثابت { بازگشت v; }
  __item_t__ __item_at__ (unsigned j) ثابت { بازگشت v; }
  منطقی __more__ () ثابت { بازگشت درست; }
  unsigned __len__ () ثابت { بازگشت UINT_MAX; }
  تهی __next__ () {}
  تهی __forward__ (unsigned) {}
  تهی __prev__ () {}
  تهی __rewind__ (unsigned) {}
  hb_repeat_iter_t __end__ () ثابت { بازگشت *این; }
  منطقی operator != (ثابت hb_repeat_iter_t& o) ثابت { بازگشت درست; }

  خصوصی:
  T v;
};
ساختار
{
  template <typename T> hb_repeat_iter_t<T>
  operator () (T value) ثابت
  { بازگشت hb_repeat_iter_t<T> (value); }
}
HB_FUNCOBJ (hb_repeat);

/* hb_enumerate()/hb_take() */

ساختار
{
  template <typename Iterable,
	    typename Index = unsigned,
	    hb_requires (hb_is_iterable (Iterable))>
  auto operator () (Iterable&& it, Index start = 0u) ثابت HB_AUTO_RETURN
  ( hb_zip (hb_iota (start), it) )
}
HB_FUNCOBJ (hb_enumerate);

ساختار
{ HB_PARTIALIZE(2);
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  auto operator () (Iterable&& it, unsigned شمار) ثابت HB_AUTO_RETURN
  ( hb_zip (hb_range (شمار), it) | hb_map_retains_sorting (hb_second) )

  /* Specialization arrays. */

  template <typename Type> درونخطی hb_array_t<Type>
  operator () (hb_array_t<Type> array, unsigned شمار) ثابت
  { بازگشت array.sub_array (0, شمار); }

  template <typename Type> درونخطی hb_sorted_array_t<Type>
  operator () (hb_sorted_array_t<Type> array, unsigned شمار) ثابت
  { بازگشت array.sub_array (0, شمار); }
}
HB_FUNCOBJ (hb_take);

ساختار
{ HB_PARTIALIZE(2);
  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  auto operator () (Iter it, unsigned شمار) ثابت HB_AUTO_RETURN
  (
    + hb_iota (it, hb_add (شمار))
    | hb_map (hb_take (شمار))
    | hb_take ((hb_len (it) + شمار - 1) / شمار)
  )
}
HB_FUNCOBJ (hb_chop);

/* hb_sink() */

template <typename Sink>
ساختار hb_sink_t
{
  hb_sink_t (Sink s) : s (s) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  تهی operator () (Iter it)
  {
    برای (; it; ++it)
      s << *it;
  }

  خصوصی:
  Sink s;
};
ساختار
{
  template <typename Sink> hb_sink_t<Sink>
  operator () (Sink&& s) ثابت
  { بازگشت hb_sink_t<Sink> (s); }

  template <typename Sink> hb_sink_t<Sink&>
  operator () (Sink *s) ثابت
  { بازگشت hb_sink_t<Sink&> (*s); }
}
HB_FUNCOBJ (hb_sink);

/* hb-drain: hb_sink to void / blackhole / /dev/null. */

ساختار
{
  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  تهی operator () (Iter it) ثابت
  {
    برای (; it; ++it)
      (تهی) *it;
  }
}
HB_FUNCOBJ (hb_drain);

/* hb_unzip(): unzip and sink to two sinks. */

template <typename Sink1, typename Sink2>
ساختار hb_unzip_t
{
  hb_unzip_t (Sink1 s1, Sink2 s2) : s1 (s1), s2 (s2) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  تهی operator () (Iter it)
  {
    برای (; it; ++it)
    {
      ثابت auto &v = *it;
      s1 << v.first;
      s2 << v.second;
    }
  }

  خصوصی:
  Sink1 s1;
  Sink2 s2;
};
ساختار
{
  template <typename Sink1, typename Sink2> hb_unzip_t<Sink1, Sink2>
  operator () (Sink1&& s1, Sink2&& s2) ثابت
  { بازگشت hb_unzip_t<Sink1, Sink2> (s1, s2); }

  template <typename Sink1, typename Sink2> hb_unzip_t<Sink1&, Sink2&>
  operator () (Sink1 *s1, Sink2 *s2) ثابت
  { بازگشت hb_unzip_t<Sink1&, Sink2&> (*s1, *s2); }
}
HB_FUNCOBJ (hb_unzip);


/* hb-all, hb-any, hb-none. */

ساختار
{
  template <typename Iterable,
	    typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity)),
	    hb_requires (hb_is_iterable (Iterable))>
  منطقی operator () (Iterable&& c,
		    Pred&& p = hb_identity,
		    Proj&& f = hb_identity) ثابت
  {
    برای (auto it = hb_iter (c); it; ++it)
      اگر (!hb_match (p, hb_get (f, *it)))
	بازگشت نادرست;
    بازگشت درست;
  }
}
HB_FUNCOBJ (hb_all);
ساختار
{
  template <typename Iterable,
	    typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity)),
	    hb_requires (hb_is_iterable (Iterable))>
  منطقی operator () (Iterable&& c,
		    Pred&& p = hb_identity,
		    Proj&& f = hb_identity) ثابت
  {
    برای (auto it = hb_iter (c); it; ++it)
      اگر (hb_match (p, hb_get (f, *it)))
	بازگشت درست;
    بازگشت نادرست;
  }
}
HB_FUNCOBJ (hb_any);
ساختار
{
  template <typename Iterable,
	    typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity)),
	    hb_requires (hb_is_iterable (Iterable))>
  منطقی operator () (Iterable&& c,
		    Pred&& p = hb_identity,
		    Proj&& f = hb_identity) ثابت
  {
    برای (auto it = hb_iter (c); it; ++it)
      اگر (hb_match (p, hb_get (f, *it)))
	بازگشت نادرست;
    بازگشت درست;
  }
}
HB_FUNCOBJ (hb_none);

/*
 * Algorithms operating on iterators.
 */

template <typename C, typename V,
	  hb_requires (hb_is_iterable (C))>
درونخطی تهی
hb_fill (C&& c, ثابت V &v)
{
  برای (auto i = hb_iter (c); i; i++)
    *i = v;
}

template <typename S, typename D>
درونخطی تهی
hb_copy (S&& is, D&& id)
{
  hb_iter (is) | hb_sink (id);
}


#endif /* HB_ITER_HH */

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

#அந்தர்பாவ "hb.hh"
#அந்தர்பாவ "hb-algs.hh"
#அந்தர்பாவ "hb-meta.hh"


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
கட்டமைப்பு hb_iter_t
{
  வகைவரை Item item_t;
  constexpr unsigned get_item_size () மாறிலி { திரும்பு hb_static_size (Item); }
  நிலையான constexpr தருக்க is_iterator = உண்மை;
  நிலையான constexpr தருக்க is_random_access_iterator = பொய்;
  நிலையான constexpr தருக்க is_sorted_iterator = பொய்;
  நிலையான constexpr தருக்க has_fast_len = பொய்; // Should be checked in combination with is_random_access_iterator.

  தனிப்பட்ட:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  மாறிலி iter_t* thiz () மாறிலி { திரும்பு static_cast<மாறிலி iter_t *> (இது); }
	iter_t* thiz ()       { திரும்பு static_cast<      iter_t *> (இது); }
  பொது:

  /* Operators. */
  iter_t iter () மாறிலி { திரும்பு *thiz(); }
  iter_t operator + () மாறிலி { திரும்பு *thiz(); }
  iter_t _begin () மாறிலி { திரும்பு *thiz(); }
  iter_t begin () மாறிலி { திரும்பு _begin (); }
  iter_t _end () மாறிலி { திரும்பு thiz()->__end__ (); }
  iter_t end () மாறிலி { திரும்பு _end (); }
  explicit operator தருக்க () மாறிலி { திரும்பு thiz()->__more__ (); }
  unsigned len () மாறிலி { திரும்பு thiz()->__len__ (); }
  /* The following can only be enabled if item_t is reference type.  Otherwise
   * it will be returning pointer to temporary rvalue. */
  template <typename T = item_t,
	    hb_enable_if (std::is_reference<T>::value)>
  hb_remove_reference<item_t>* operator -> () மாறிலி { திரும்பு std::addressof (**thiz()); }
  item_t operator * () மாறிலி { திரும்பு thiz()->__item__ (); }
  item_t operator * () { திரும்பு thiz()->__item__ (); }
  item_t operator [] (unsigned i) மாறிலி { திரும்பு thiz()->__item_at__ (i); }
  item_t operator [] (unsigned i) { திரும்பு thiz()->__item_at__ (i); }
  iter_t& operator += (unsigned மொத்தம்) &  { thiz()->__forward__ (மொத்தம்); திரும்பு *thiz(); }
  iter_t  operator += (unsigned மொத்தம்) && { thiz()->__forward__ (மொத்தம்); திரும்பு *thiz(); }
  iter_t& operator ++ () &  { thiz()->__next__ (); திரும்பு *thiz(); }
  iter_t  operator ++ () && { thiz()->__next__ (); திரும்பு *thiz(); }
  iter_t& operator -= (unsigned மொத்தம்) &  { thiz()->__rewind__ (மொத்தம்); திரும்பு *thiz(); }
  iter_t  operator -= (unsigned மொத்தம்) && { thiz()->__rewind__ (மொத்தம்); திரும்பு *thiz(); }
  iter_t& operator -- () &  { thiz()->__prev__ (); திரும்பு *thiz(); }
  iter_t  operator -- () && { thiz()->__prev__ (); திரும்பு *thiz(); }
  iter_t operator + (unsigned மொத்தம்) மாறிலி { auto c = thiz()->iter (); c += மொத்தம்; திரும்பு c; }
  friend iter_t operator + (unsigned மொத்தம், மாறிலி iter_t &it) { திரும்பு it + மொத்தம்; }
  iter_t operator ++ (முழு) { iter_t c (*thiz()); ++*thiz(); திரும்பு c; }
  iter_t operator - (unsigned மொத்தம்) மாறிலி { auto c = thiz()->iter (); c -= மொத்தம்; திரும்பு c; }
  iter_t operator -- (முழு) { iter_t c (*thiz()); --*thiz(); திரும்பு c; }
  template <typename T>
  iter_t& operator >> (T &v) &  { v = **thiz(); ++*thiz(); திரும்பு *thiz(); }
  template <typename T>
  iter_t  operator >> (T &v) && { v = **thiz(); ++*thiz(); திரும்பு *thiz(); }
  template <typename T>
  iter_t& operator << (மாறிலி T v) &  { **thiz() = v; ++*thiz(); திரும்பு *thiz(); }
  template <typename T>
  iter_t  operator << (மாறிலி T v) && { **thiz() = v; ++*thiz(); திரும்பு *thiz(); }

  பாதுகாக்கப்பட்ட:
  hb_iter_t () = இயல்பு;
  hb_iter_t (மாறிலி hb_iter_t &o HB_UNUSED) = இயல்பு;
  hb_iter_t (hb_iter_t &&o HB_UNUSED) = இயல்பு;
  hb_iter_t& operator = (மாறிலி hb_iter_t &o HB_UNUSED) = இயல்பு;
  hb_iter_t& operator = (hb_iter_t &&o HB_UNUSED) = இயல்பு;
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
  using Name::operator தருக்க; \
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
  static_assert (உண்மை, "")

/* Returns iterator / item type of a type. */
template <typename Iterable>
using hb_iter_type = decltype (hb_deref (hb_declval (Iterable)).iter ());
template <typename Iterable>
using hb_item_type = decltype (*hb_deref (hb_declval (Iterable)).iter ());


template <typename> கட்டமைப்பு hb_array_t;
template <typename> கட்டமைப்பு hb_sorted_array_t;

கட்டமைப்பு
{
  template <typename T> hb_iter_type<T>
  operator () (T&& c) மாறிலி
  { திரும்பு hb_deref (std::forward<T> (c)).iter (); }

  /* Specialization for C arrays. */

  template <typename Type> உள்வரி hb_array_t<Type>
  operator () (Type *array, unsigned முழு length) மாறிலி
  { திரும்பு hb_array_t<Type> (array, length); }

  template <typename Type, unsigned முழு length> hb_array_t<Type>
  operator () (Type (&array)[length]) மாறிலி
  { திரும்பு hb_array_t<Type> (array, length); }

}
HB_FUNCOBJ (hb_iter);
கட்டமைப்பு
{
  template <typename T> auto
  impl (T&& c, hb_priority<1>) மாறிலி HB_RETURN (unsigned, c.len ())

  template <typename T> auto
  impl (T&& c, hb_priority<0>) மாறிலி HB_RETURN (unsigned, c.len)

  பொது:

  template <typename T> auto
  operator () (T&& c) மாறிலி HB_RETURN (unsigned, impl (std::forward<T> (c), hb_prioritize))
}
HB_FUNCOBJ (hb_len);

/* Mixin to fill in what the subclass doesn't provide. */
template <typename iter_t, typename item_t = typename iter_t::__item_t__>
கட்டமைப்பு hb_iter_fallback_mixin_t
{
  தனிப்பட்ட:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  மாறிலி iter_t* thiz () மாறிலி { திரும்பு static_cast<மாறிலி iter_t *> (இது); }
	iter_t* thiz ()       { திரும்பு static_cast<      iter_t *> (இது); }
  பொது:

  /* Access: Implement __item__(), or __item_at__() if random-access. */
  item_t __item__ () மாறிலி { திரும்பு (*thiz())[0]; }
  item_t __item_at__ (unsigned i) மாறிலி { திரும்பு *(*thiz() + i); }

  /* Termination: Implement __more__(), or __len__() if random-access. */
  தருக்க __more__ () மாறிலி { திரும்பு தருக்க (thiz()->len ()); }
  unsigned __len__ () மாறிலி
  { iter_t c (*thiz()); unsigned l = 0; போது (c) { c++; l++; } திரும்பு l; }

  /* Advancing: Implement __next__(), or __forward__() if random-access. */
  வெறுமை __next__ () { *thiz() += 1; }
  வெறுமை __forward__ (unsigned n) { போது (*thiz() && n--) ++*thiz(); }

  /* Rewinding: Implement __prev__() or __rewind__() if bidirectional. */
  வெறுமை __prev__ () { *thiz() -= 1; }
  வெறுமை __rewind__ (unsigned n) { போது (*thiz() && n--) --*thiz(); }

  /* Range-based for: Implement __end__() if can be done faster,
   * and operator!=. */
  iter_t __end__ () மாறிலி
  {
    எனில் (thiz()->is_random_access_iterator)
      திரும்பு *thiz() + thiz()->len ();
    /* Above expression loops twice. Following loops once. */
    auto it = *thiz();
    போது (it) ++it;
    திரும்பு it;
  }

  பாதுகாக்கப்பட்ட:
  hb_iter_fallback_mixin_t () = இயல்பு;
  hb_iter_fallback_mixin_t (மாறிலி hb_iter_fallback_mixin_t &o HB_UNUSED) = இயல்பு;
  hb_iter_fallback_mixin_t (hb_iter_fallback_mixin_t &&o HB_UNUSED) = இயல்பு;
  hb_iter_fallback_mixin_t& operator = (மாறிலி hb_iter_fallback_mixin_t &o HB_UNUSED) = இயல்பு;
  hb_iter_fallback_mixin_t& operator = (hb_iter_fallback_mixin_t &&o HB_UNUSED) = இயல்பு;
};

template <typename iter_t, typename item_t = typename iter_t::__item_t__>
கட்டமைப்பு hb_iter_with_fallback_t :
  hb_iter_t<iter_t, item_t>,
  hb_iter_fallback_mixin_t<iter_t, item_t>
{
  பாதுகாக்கப்பட்ட:
  hb_iter_with_fallback_t () = இயல்பு;
  hb_iter_with_fallback_t (மாறிலி hb_iter_with_fallback_t &o HB_UNUSED) = இயல்பு;
  hb_iter_with_fallback_t (hb_iter_with_fallback_t &&o HB_UNUSED) = இயல்பு;
  hb_iter_with_fallback_t& operator = (மாறிலி hb_iter_with_fallback_t &o HB_UNUSED) = இயல்பு;
  hb_iter_with_fallback_t& operator = (hb_iter_with_fallback_t &&o HB_UNUSED) = இயல்பு;
};

/*
 * Meta-programming predicates.
 */

/* hb_is_iterator() / hb_is_iterator_of() */

template<typename Iter, typename Item>
கட்டமைப்பு hb_is_iterator_of
{
  template <typename Item2 = Item>
  நிலையான hb_true_type impl (hb_priority<2>, hb_iter_t<Iter, hb_type_identity<Item2>> *);
  நிலையான hb_false_type impl (hb_priority<0>, மாறிலி வெறுமை *);

  பொது:
  நிலையான constexpr தருக்க value = decltype (impl (hb_prioritize, hb_declval (Iter*)))::value;
};
#define hb_is_iterator_of(Iter, Item) hb_is_iterator_of<Iter, Item>::value
#define hb_is_iterator(Iter) hb_is_iterator_of (Iter, typename Iter::item_t)
#define hb_is_sorted_iterator_of(Iter, Item) (hb_is_iterator_of<Iter, Item>::value && Iter::is_sorted_iterator)
#define hb_is_sorted_iterator(Iter) hb_is_sorted_iterator_of (Iter, typename Iter::item_t)

/* hb_is_iterable() */

template <typename T>
கட்டமைப்பு hb_is_iterable
{
  தனிப்பட்ட:

  template <typename U>
  நிலையான auto impl (hb_priority<1>) -> decltype (hb_declval (U).iter (), hb_true_type ());

  template <typename>
  நிலையான hb_false_type impl (hb_priority<0>);

  பொது:
  நிலையான constexpr தருக்க value = decltype (impl<T> (hb_prioritize))::value;
};
#define hb_is_iterable(Iterable) hb_is_iterable<Iterable>::value

/* hb_is_source_of() / hb_is_sink_of() */

template<typename Iter, typename Item>
கட்டமைப்பு hb_is_source_of
{
  தனிப்பட்ட:
  template <typename Iter2 = Iter,
	    hb_enable_if (hb_is_convertible (typename Iter2::item_t, hb_add_lvalue_reference<மாறிலி Item>))>
  நிலையான hb_true_type impl (hb_priority<2>);
  template <typename Iter2 = Iter>
  நிலையான auto impl (hb_priority<1>) -> decltype (hb_declval (Iter2) >> hb_declval (Item &), hb_true_type ());
  நிலையான hb_false_type impl (hb_priority<0>);

  பொது:
  நிலையான constexpr தருக்க value = decltype (impl (hb_prioritize))::value;
};
#define hb_is_source_of(Iter, Item) hb_is_source_of<Iter, Item>::value

template<typename Iter, typename Item>
கட்டமைப்பு hb_is_sink_of
{
  தனிப்பட்ட:
  template <typename Iter2 = Iter,
	    hb_enable_if (hb_is_convertible (typename Iter2::item_t, hb_add_lvalue_reference<Item>))>
  நிலையான hb_true_type impl (hb_priority<2>);
  template <typename Iter2 = Iter>
  நிலையான auto impl (hb_priority<1>) -> decltype (hb_declval (Iter2) << hb_declval (Item), hb_true_type ());
  நிலையான hb_false_type impl (hb_priority<0>);

  பொது:
  நிலையான constexpr தருக்க value = decltype (impl (hb_prioritize))::value;
};
#define hb_is_sink_of(Iter, Item) hb_is_sink_of<Iter, Item>::value

/* This is commonly used, so define: */
#define hb_is_sorted_source_of(Iter, Item) \
	(hb_is_source_of(Iter, Item) && Iter::is_sorted_iterator)


கட்டமைப்பு
{
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  unsigned operator () (மாறிலி Iterable &_) மாறிலி { திரும்பு hb_len (hb_iter (_)); }

  unsigned operator () (unsigned _) மாறிலி { திரும்பு _; }
}
HB_FUNCOBJ (hb_len_of);

/* Range-based 'for' for iterables. */

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
நிலையான உள்வரி auto begin (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).begin ())

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
நிலையான உள்வரி auto end (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).end ())

/* begin()/end() are NOT looked up non-ADL.  So each namespace must declare them.
 * Do it for namespace OT. */
நாமாகாஶ OT {

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
நிலையான உள்வரி auto begin (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).begin ())

template <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
நிலையான உள்வரி auto end (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).end ())

}


/*
 * Adaptors, combiners, etc.
 */

template <typename Lhs, typename Rhs,
	  hb_requires (hb_is_iterator (Lhs))>
நிலையான உள்வரி auto
operator | (Lhs&& lhs, Rhs&& rhs) HB_AUTO_RETURN (std::forward<Rhs> (rhs) (std::forward<Lhs> (lhs)))

/* hb_map(), hb_filter(), hb_reduce() */

எண்ணிக்கை  வகுப்பு hb_function_sortedness_t {
  NOT_SORTED,
  RETAINS_SORTING,
  SORTED,
};

template <typename Iter, typename Proj, hb_function_sortedness_t Sorted,
	 hb_requires (hb_is_iterator (Iter))>
கட்டமைப்பு hb_map_iter_t :
  hb_iter_t<hb_map_iter_t<Iter, Proj, Sorted>,
	    decltype (hb_get (hb_declval (Proj), *hb_declval (Iter)))>
{
  hb_map_iter_t (மாறிலி Iter& it, Proj f_) : it (it), f (f_) {}

  வகைவரை decltype (hb_get (hb_declval (Proj), *hb_declval (Iter))) __item_t__;
  நிலையான constexpr தருக்க is_random_access_iterator = Iter::is_random_access_iterator;
  நிலையான constexpr தருக்க is_sorted_iterator =
    Sorted == hb_function_sortedness_t::SORTED ? உண்மை :
    Sorted == hb_function_sortedness_t::RETAINS_SORTING ? Iter::is_sorted_iterator :
    பொய்;
  __item_t__ __item__ () மாறிலி { திரும்பு hb_get (f.get (), *it); }
  __item_t__ __item_at__ (unsigned i) மாறிலி { திரும்பு hb_get (f.get (), it[i]); }
  தருக்க __more__ () மாறிலி { திரும்பு தருக்க (it); }
  unsigned __len__ () மாறிலி { திரும்பு it.len (); }
  வெறுமை __next__ () { ++it; }
  வெறுமை __forward__ (unsigned n) { it += n; }
  வெறுமை __prev__ () { --it; }
  வெறுமை __rewind__ (unsigned n) { it -= n; }
  hb_map_iter_t __end__ () மாறிலி { திரும்பு hb_map_iter_t (it._end (), f); }
  தருக்க operator != (மாறிலி hb_map_iter_t& o) மாறிலி
  { திரும்பு it != o.it; }

  தனிப்பட்ட:
  Iter it;
  mutable hb_reference_wrapper<Proj> f;
};

template <typename Proj, hb_function_sortedness_t Sorted>
கட்டமைப்பு hb_map_iter_factory_t
{
  hb_map_iter_factory_t (Proj f) : f (f) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  hb_map_iter_t<Iter, Proj, Sorted>
  operator () (Iter it)
  { திரும்பு hb_map_iter_t<Iter, Proj, Sorted> (it, f); }

  தனிப்பட்ட:
  Proj f;
};
கட்டமைப்பு
{
  template <typename Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::NOT_SORTED>
  operator () (Proj&& f) மாறிலி
  { திரும்பு hb_map_iter_factory_t<Proj, hb_function_sortedness_t::NOT_SORTED> (f); }
}
HB_FUNCOBJ (hb_map);
கட்டமைப்பு
{
  template <typename Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::RETAINS_SORTING>
  operator () (Proj&& f) மாறிலி
  { திரும்பு hb_map_iter_factory_t<Proj, hb_function_sortedness_t::RETAINS_SORTING> (f); }
}
HB_FUNCOBJ (hb_map_retains_sorting);
கட்டமைப்பு
{
  template <typename Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::SORTED>
  operator () (Proj&& f) மாறிலி
  { திரும்பு hb_map_iter_factory_t<Proj, hb_function_sortedness_t::SORTED> (f); }
}
HB_FUNCOBJ (hb_map_sorted);

template <typename Iter, typename Pred, typename Proj,
	 hb_requires (hb_is_iterator (Iter))>
கட்டமைப்பு hb_filter_iter_t :
  hb_iter_with_fallback_t<hb_filter_iter_t<Iter, Pred, Proj>,
			  typename Iter::item_t>
{
  hb_filter_iter_t (மாறிலி Iter& it_, Pred p_, Proj f_) : it (it_), p (p_), f (f_)
  { போது (it && !hb_has (p.get (), hb_get (f.get (), *it))) ++it; }

  வகைவரை typename Iter::item_t __item_t__;
  நிலையான constexpr தருக்க is_sorted_iterator = Iter::is_sorted_iterator;
  __item_t__ __item__ () மாறிலி { திரும்பு *it; }
  தருக்க __more__ () மாறிலி { திரும்பு தருக்க (it); }
  வெறுமை __next__ () { செய் ++it; போது (it && !hb_has (p.get (), hb_get (f.get (), *it))); }
  வெறுமை __prev__ ()
  {
    unsigned மொத்தம் = 0;
    செய்
    {
      Iter old = it;
      --it;
      எனில் (!(it != old))
      {
	it += மொத்தம்;
	திரும்பு;
      }
      மொத்தம்++;
    }
    போது (!hb_has (p.get (), hb_get (f.get (), *it)));
  }
  hb_filter_iter_t __end__ () மாறிலி { திரும்பு hb_filter_iter_t (it._end (), p, f); }
  தருக்க operator != (மாறிலி hb_filter_iter_t& o) மாறிலி
  { திரும்பு it != o.it; }

  தனிப்பட்ட:
  Iter it;
  mutable hb_reference_wrapper<Pred> p;
  mutable hb_reference_wrapper<Proj> f;
};
template <typename Pred, typename Proj>
கட்டமைப்பு hb_filter_iter_factory_t
{
  hb_filter_iter_factory_t (Pred p, Proj f) : p (p), f (f) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  hb_filter_iter_t<Iter, Pred, Proj>
  operator () (Iter it)
  { திரும்பு hb_filter_iter_t<Iter, Pred, Proj> (it, p, f); }

  தனிப்பட்ட:
  Pred p;
  Proj f;
};
கட்டமைப்பு
{
  template <typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity))>
  hb_filter_iter_factory_t<Pred, Proj>
  operator () (Pred&& p = hb_identity, Proj&& f = hb_identity) மாறிலி
  { திரும்பு hb_filter_iter_factory_t<Pred, Proj> (p, f); }
}
HB_FUNCOBJ (hb_filter);

template <typename Redu, typename InitT>
கட்டமைப்பு hb_reduce_t
{
  hb_reduce_t (Redu r, InitT init_மதிப்பு) : r (r), init_மதிப்பு (init_மதிப்பு) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter)),
	    typename AccuT = hb_decay<decltype (hb_declval (Redu) (hb_declval (InitT), hb_declval (typename Iter::item_t)))>>
  AccuT
  operator () (Iter it)
  {
    AccuT value = init_மதிப்பு;
    க்கு (; it; ++it)
      value = r (value, *it);
    திரும்பு value;
  }

  தனிப்பட்ட:
  Redu r;
  InitT init_மதிப்பு;
};
கட்டமைப்பு
{
  template <typename Redu, typename InitT>
  hb_reduce_t<Redu, InitT>
  operator () (Redu&& r, InitT init_மதிப்பு) மாறிலி
  { திரும்பு hb_reduce_t<Redu, InitT> (r, init_மதிப்பு); }
}
HB_FUNCOBJ (hb_reduce);


/* hb_zip() */

template <typename A, typename B>
கட்டமைப்பு hb_zip_iter_t :
  hb_iter_t<hb_zip_iter_t<A, B>,
	    hb_pair_t<typename A::item_t, typename B::item_t>>
{
  hb_zip_iter_t () {}
  hb_zip_iter_t (மாறிலி A& a, மாறிலி B& b) : a (a), b (b) {}

  வகைவரை hb_pair_t<typename A::item_t, typename B::item_t> __item_t__;
  நிலையான constexpr தருக்க is_random_access_iterator =
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
  நிலையான constexpr தருக்க is_sorted_iterator = A::is_sorted_iterator;

  __item_t__ __item__ () மாறிலி { திரும்பு __item_t__ (*a, *b); }
  __item_t__ __item_at__ (unsigned i) மாறிலி { திரும்பு __item_t__ (a[i], b[i]); }
  தருக்க __more__ () மாறிலி { திரும்பு தருக்க (a) && தருக்க (b); }
  unsigned __len__ () மாறிலி { திரும்பு hb_min (a.len (), b.len ()); }
  வெறுமை __next__ () { ++a; ++b; }
  வெறுமை __forward__ (unsigned n) { a += n; b += n; }
  வெறுமை __prev__ () { --a; --b; }
  வெறுமை __rewind__ (unsigned n) { a -= n; b -= n; }
  hb_zip_iter_t __end__ () மாறிலி { திரும்பு hb_zip_iter_t (a._end (), b._end ()); }
  /* Note, we should stop if ANY of the iters reaches end.  As such two compare
   * unequal if both items are unequal, NOT if either is unequal. */
  தருக்க operator != (மாறிலி hb_zip_iter_t& o) மாறிலி
  { திரும்பு a != o.a && b != o.b; }

  தனிப்பட்ட:
  A a;
  B b;
};
கட்டமைப்பு
{ HB_PARTIALIZE(2);
  template <typename A, typename B,
	    hb_requires (hb_is_iterable (A) && hb_is_iterable (B))>
  hb_zip_iter_t<hb_iter_type<A>, hb_iter_type<B>>
  operator () (A&& a, B&& b) மாறிலி
  { திரும்பு hb_zip_iter_t<hb_iter_type<A>, hb_iter_type<B>> (hb_iter (a), hb_iter (b)); }
}
HB_FUNCOBJ (hb_zip);

/* hb_concat() */

template <typename A, typename B>
கட்டமைப்பு hb_concat_iter_t :
    hb_iter_t<hb_concat_iter_t<A, B>, typename A::item_t>
{
  hb_concat_iter_t () {}
  hb_concat_iter_t (A& a, B& b) : a (a), b (b) {}
  hb_concat_iter_t (மாறிலி A& a, மாறிலி B& b) : a (a), b (b) {}


  வகைவரை typename A::item_t __item_t__;
  நிலையான constexpr தருக்க is_random_access_iterator =
    A::is_random_access_iterator &&
    B::is_random_access_iterator;
  நிலையான constexpr தருக்க is_sorted_iterator = பொய்;

  __item_t__ __item__ () மாறிலி
  {
    எனில் (!a)
      திரும்பு *b;
    திரும்பு *a;
  }

  __item_t__ __item_at__ (unsigned i) மாறிலி
  {
    unsigned a_len = a.len ();
    எனில் (i < a_len)
      திரும்பு a[i];
    திரும்பு b[i - a_len];
  }

  தருக்க __more__ () மாறிலி { திரும்பு தருக்க (a) || தருக்க (b); }

  unsigned __len__ () மாறிலி { திரும்பு a.len () + b.len (); }

  வெறுமை __next__ ()
  {
    எனில் (a)
      ++a;
    இல்லையெனில்
      ++b;
  }

  வெறுமை __forward__ (unsigned n)
  {
    எனில் (!n) திரும்பு;
    எனில் (!is_random_access_iterator) {
      போது (n-- && *இது) {
        (*இது)++;
      }
      திரும்பு;
    }

    unsigned a_len = a.len ();
    எனில் (n > a_len) {
      n -= a_len;
      a.__forward__ (a_len);
      b.__forward__ (n);
    } இல்லையெனில் {
      a.__forward__ (n);
    }
  }

  hb_concat_iter_t __end__ () மாறிலி { திரும்பு hb_concat_iter_t (a._end (), b._end ()); }
  தருக்க operator != (மாறிலி hb_concat_iter_t& o) மாறிலி
  {
    திரும்பு a != o.a
        || b != o.b;
  }

  தனிப்பட்ட:
  A a;
  B b;
};
கட்டமைப்பு
{ HB_PARTIALIZE(2);
  template <typename A, typename B,
	    hb_requires (hb_is_iterable (A) && hb_is_iterable (B))>
  hb_concat_iter_t<hb_iter_type<A>, hb_iter_type<B>>
  operator () (A&& a, B&& b) மாறிலி
  { திரும்பு hb_concat_iter_t<hb_iter_type<A>, hb_iter_type<B>> (hb_iter (a), hb_iter (b)); }
}
HB_FUNCOBJ (hb_concat);

/* hb_apply() */

template <typename Appl>
கட்டமைப்பு hb_apply_t
{
  hb_apply_t (Appl a) : a (a) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  வெறுமை operator () (Iter it)
  {
    க்கு (; it; ++it)
      (வெறுமை) hb_invoke (a, *it);
  }

  தனிப்பட்ட:
  Appl a;
};
கட்டமைப்பு
{
  template <typename Appl> hb_apply_t<Appl>
  operator () (Appl&& a) மாறிலி
  { திரும்பு hb_apply_t<Appl> (a); }

  template <typename Appl> hb_apply_t<Appl&>
  operator () (Appl *a) மாறிலி
  { திரும்பு hb_apply_t<Appl&> (*a); }
}
HB_FUNCOBJ (hb_apply);

/* hb_range()/hb_iota()/hb_repeat() */

template <typename T, typename S>
கட்டமைப்பு hb_range_iter_t :
  hb_iter_t<hb_range_iter_t<T, S>, T>
{
  hb_range_iter_t (T start, T end_, S step) : v (start), end_ (end_for (start, end_, step)), step (step) {}

  வகைவரை T __item_t__;
  நிலையான constexpr தருக்க is_random_access_iterator = உண்மை;
  நிலையான constexpr தருக்க is_sorted_iterator = உண்மை;
  __item_t__ __item__ () மாறிலி { திரும்பு hb_ridentity (v); }
  __item_t__ __item_at__ (unsigned j) மாறிலி { திரும்பு v + j * step; }
  தருக்க __more__ () மாறிலி { திரும்பு v != end_; }
  unsigned __len__ () மாறிலி { திரும்பு !step ? UINT_MAX : (end_ - v) / step; }
  வெறுமை __next__ () { v += step; }
  வெறுமை __forward__ (unsigned n) { v += n * step; }
  வெறுமை __prev__ () { v -= step; }
  வெறுமை __rewind__ (unsigned n) { v -= n * step; }
  hb_range_iter_t __end__ () மாறிலி { திரும்பு hb_range_iter_t (end_, end_, step); }
  தருக்க operator != (மாறிலி hb_range_iter_t& o) மாறிலி
  { திரும்பு v != o.v; }

  தனிப்பட்ட:
  நிலையான உள்வரி T end_for (T start, T end_, S step)
  {
    எனில் (!step)
      திரும்பு end_;
    auto res = (end_ - start) % step;
    எனில் (!res)
      திரும்பு end_;
    end_ += step - res;
    திரும்பு end_;
  }

  தனிப்பட்ட:
  T v;
  T end_;
  S step;
};
கட்டமைப்பு
{
  template <typename T = unsigned> hb_range_iter_t<T, unsigned>
  operator () (T end = (unsigned) -1) மாறிலி
  { திரும்பு hb_range_iter_t<T, unsigned> (0, end, 1u); }

  template <typename T, typename S = unsigned> hb_range_iter_t<T, S>
  operator () (T start, T end, S step = 1u) மாறிலி
  { திரும்பு hb_range_iter_t<T, S> (start, end, step); }
}
HB_FUNCOBJ (hb_range);

template <typename T, typename S>
கட்டமைப்பு hb_iota_iter_t :
  hb_iter_with_fallback_t<hb_iota_iter_t<T, S>, T>
{
  hb_iota_iter_t (T start, S step) : v (start), step (step) {}

  தனிப்பட்ட:

  template <typename S2 = S>
  auto
  inc (hb_type_identity<S2> s, hb_priority<1>)
    -> hb_void_t<decltype (hb_invoke (std::forward<hb_type_identity<S2>> (s),
                                      hb_declval<T&> ()))>
  { v = hb_invoke (std::forward<hb_type_identity<S2>> (s), v); }

  வெறுமை
  inc (S s, hb_priority<0>)
  { v += s; }

  பொது:

  வகைவரை T __item_t__;
  நிலையான constexpr தருக்க is_random_access_iterator = உண்மை;
  நிலையான constexpr தருக்க is_sorted_iterator = உண்மை;
  __item_t__ __item__ () மாறிலி { திரும்பு hb_ridentity (v); }
  தருக்க __more__ () மாறிலி { திரும்பு உண்மை; }
  unsigned __len__ () மாறிலி { திரும்பு UINT_MAX; }
  வெறுமை __next__ () { inc (step, hb_prioritize); }
  வெறுமை __prev__ () { v -= step; }
  hb_iota_iter_t __end__ () மாறிலி { திரும்பு *இது; }
  தருக்க operator != (மாறிலி hb_iota_iter_t& o) மாறிலி { திரும்பு உண்மை; }

  தனிப்பட்ட:
  T v;
  S step;
};
கட்டமைப்பு
{
  template <typename T = unsigned, typename S = unsigned> hb_iota_iter_t<T, S>
  operator () (T start = 0u, S step = 1u) மாறிலி
  { திரும்பு hb_iota_iter_t<T, S> (start, step); }
}
HB_FUNCOBJ (hb_iota);

template <typename T>
கட்டமைப்பு hb_repeat_iter_t :
  hb_iter_t<hb_repeat_iter_t<T>, T>
{
  hb_repeat_iter_t (T value) : v (value) {}

  வகைவரை T __item_t__;
  நிலையான constexpr தருக்க is_random_access_iterator = உண்மை;
  நிலையான constexpr தருக்க is_sorted_iterator = உண்மை;
  __item_t__ __item__ () மாறிலி { திரும்பு v; }
  __item_t__ __item_at__ (unsigned j) மாறிலி { திரும்பு v; }
  தருக்க __more__ () மாறிலி { திரும்பு உண்மை; }
  unsigned __len__ () மாறிலி { திரும்பு UINT_MAX; }
  வெறுமை __next__ () {}
  வெறுமை __forward__ (unsigned) {}
  வெறுமை __prev__ () {}
  வெறுமை __rewind__ (unsigned) {}
  hb_repeat_iter_t __end__ () மாறிலி { திரும்பு *இது; }
  தருக்க operator != (மாறிலி hb_repeat_iter_t& o) மாறிலி { திரும்பு உண்மை; }

  தனிப்பட்ட:
  T v;
};
கட்டமைப்பு
{
  template <typename T> hb_repeat_iter_t<T>
  operator () (T value) மாறிலி
  { திரும்பு hb_repeat_iter_t<T> (value); }
}
HB_FUNCOBJ (hb_repeat);

/* hb_enumerate()/hb_take() */

கட்டமைப்பு
{
  template <typename Iterable,
	    typename Index = unsigned,
	    hb_requires (hb_is_iterable (Iterable))>
  auto operator () (Iterable&& it, Index start = 0u) மாறிலி HB_AUTO_RETURN
  ( hb_zip (hb_iota (start), it) )
}
HB_FUNCOBJ (hb_enumerate);

கட்டமைப்பு
{ HB_PARTIALIZE(2);
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  auto operator () (Iterable&& it, unsigned மொத்தம்) மாறிலி HB_AUTO_RETURN
  ( hb_zip (hb_range (மொத்தம்), it) | hb_map_retains_sorting (hb_second) )

  /* Specialization arrays. */

  template <typename Type> உள்வரி hb_array_t<Type>
  operator () (hb_array_t<Type> array, unsigned மொத்தம்) மாறிலி
  { திரும்பு array.sub_array (0, மொத்தம்); }

  template <typename Type> உள்வரி hb_sorted_array_t<Type>
  operator () (hb_sorted_array_t<Type> array, unsigned மொத்தம்) மாறிலி
  { திரும்பு array.sub_array (0, மொத்தம்); }
}
HB_FUNCOBJ (hb_take);

கட்டமைப்பு
{ HB_PARTIALIZE(2);
  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  auto operator () (Iter it, unsigned மொத்தம்) மாறிலி HB_AUTO_RETURN
  (
    + hb_iota (it, hb_add (மொத்தம்))
    | hb_map (hb_take (மொத்தம்))
    | hb_take ((hb_len (it) + மொத்தம் - 1) / மொத்தம்)
  )
}
HB_FUNCOBJ (hb_chop);

/* hb_sink() */

template <typename Sink>
கட்டமைப்பு hb_sink_t
{
  hb_sink_t (Sink s) : s (s) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  வெறுமை operator () (Iter it)
  {
    க்கு (; it; ++it)
      s << *it;
  }

  தனிப்பட்ட:
  Sink s;
};
கட்டமைப்பு
{
  template <typename Sink> hb_sink_t<Sink>
  operator () (Sink&& s) மாறிலி
  { திரும்பு hb_sink_t<Sink> (s); }

  template <typename Sink> hb_sink_t<Sink&>
  operator () (Sink *s) மாறிலி
  { திரும்பு hb_sink_t<Sink&> (*s); }
}
HB_FUNCOBJ (hb_sink);

/* hb-drain: hb_sink to void / blackhole / /dev/null. */

கட்டமைப்பு
{
  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  வெறுமை operator () (Iter it) மாறிலி
  {
    க்கு (; it; ++it)
      (வெறுமை) *it;
  }
}
HB_FUNCOBJ (hb_drain);

/* hb_unzip(): unzip and sink to two sinks. */

template <typename Sink1, typename Sink2>
கட்டமைப்பு hb_unzip_t
{
  hb_unzip_t (Sink1 s1, Sink2 s2) : s1 (s1), s2 (s2) {}

  template <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  வெறுமை operator () (Iter it)
  {
    க்கு (; it; ++it)
    {
      மாறிலி auto &v = *it;
      s1 << v.first;
      s2 << v.second;
    }
  }

  தனிப்பட்ட:
  Sink1 s1;
  Sink2 s2;
};
கட்டமைப்பு
{
  template <typename Sink1, typename Sink2> hb_unzip_t<Sink1, Sink2>
  operator () (Sink1&& s1, Sink2&& s2) மாறிலி
  { திரும்பு hb_unzip_t<Sink1, Sink2> (s1, s2); }

  template <typename Sink1, typename Sink2> hb_unzip_t<Sink1&, Sink2&>
  operator () (Sink1 *s1, Sink2 *s2) மாறிலி
  { திரும்பு hb_unzip_t<Sink1&, Sink2&> (*s1, *s2); }
}
HB_FUNCOBJ (hb_unzip);


/* hb-all, hb-any, hb-none. */

கட்டமைப்பு
{
  template <typename Iterable,
	    typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity)),
	    hb_requires (hb_is_iterable (Iterable))>
  தருக்க operator () (Iterable&& c,
		    Pred&& p = hb_identity,
		    Proj&& f = hb_identity) மாறிலி
  {
    க்கு (auto it = hb_iter (c); it; ++it)
      எனில் (!hb_match (p, hb_get (f, *it)))
	திரும்பு பொய்;
    திரும்பு உண்மை;
  }
}
HB_FUNCOBJ (hb_all);
கட்டமைப்பு
{
  template <typename Iterable,
	    typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity)),
	    hb_requires (hb_is_iterable (Iterable))>
  தருக்க operator () (Iterable&& c,
		    Pred&& p = hb_identity,
		    Proj&& f = hb_identity) மாறிலி
  {
    க்கு (auto it = hb_iter (c); it; ++it)
      எனில் (hb_match (p, hb_get (f, *it)))
	திரும்பு உண்மை;
    திரும்பு பொய்;
  }
}
HB_FUNCOBJ (hb_any);
கட்டமைப்பு
{
  template <typename Iterable,
	    typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity)),
	    hb_requires (hb_is_iterable (Iterable))>
  தருக்க operator () (Iterable&& c,
		    Pred&& p = hb_identity,
		    Proj&& f = hb_identity) மாறிலி
  {
    க்கு (auto it = hb_iter (c); it; ++it)
      எனில் (hb_match (p, hb_get (f, *it)))
	திரும்பு பொய்;
    திரும்பு உண்மை;
  }
}
HB_FUNCOBJ (hb_none);

/*
 * Algorithms operating on iterators.
 */

template <typename C, typename V,
	  hb_requires (hb_is_iterable (C))>
உள்வரி வெறுமை
hb_fill (C&& c, மாறிலி V &v)
{
  க்கு (auto i = hb_iter (c); i; i++)
    *i = v;
}

template <typename S, typename D>
உள்வரி வெறுமை
hb_copy (S&& is, D&& id)
{
  hb_iter (is) | hb_sink (id);
}


#endif /* HB_ITER_HH */

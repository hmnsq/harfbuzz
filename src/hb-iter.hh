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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-algs.hh"
#अन्तर्भाव "hb-meta.hh"


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
ढाँचा <typename iter_t, typename Item = typename iter_t::__item_t__>
संरचना hb_iter_t
{
  प्रकारनिर्देश Item item_t;
  constexpr अचिन्हित get_item_size () अचर { फर्काऊ hb_static_size (Item); }
  स्थिर constexpr तार्किक is_iterator = सत्य;
  स्थिर constexpr तार्किक is_random_access_iterator = असत्य;
  स्थिर constexpr तार्किक is_sorted_iterator = असत्य;
  स्थिर constexpr तार्किक has_fast_len = असत्य; // Should be checked in combination with is_random_access_iterator.

  गुप्त:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  अचर iter_t* thiz () अचर { फर्काऊ static_cast<अचर iter_t *> (एतत्); }
	iter_t* thiz ()       { फर्काऊ static_cast<      iter_t *> (एतत्); }
  सार्वजनिक:

  /* Operators. */
  iter_t iter () अचर { फर्काऊ *thiz(); }
  iter_t चालक + () अचर { फर्काऊ *thiz(); }
  iter_t _begin () अचर { फर्काऊ *thiz(); }
  iter_t begin () अचर { फर्काऊ _begin (); }
  iter_t _end () अचर { फर्काऊ thiz()->__end__ (); }
  iter_t end () अचर { फर्काऊ _end (); }
  explicit चालक तार्किक () अचर { फर्काऊ thiz()->__more__ (); }
  अचिन्हित len () अचर { फर्काऊ thiz()->__len__ (); }
  /* The following can only be enabled if item_t is reference type.  Otherwise
   * it will be returning pointer to temporary rvalue. */
  ढाँचा <typename T = item_t,
	    hb_enable_if (std::is_reference<T>::value)>
  hb_remove_reference<item_t>* चालक -> () अचर { फर्काऊ std::addressof (**thiz()); }
  item_t चालक * () अचर { फर्काऊ thiz()->__item__ (); }
  item_t चालक * () { फर्काऊ thiz()->__item__ (); }
  item_t चालक [] (अचिन्हित i) अचर { फर्काऊ thiz()->__item_at__ (i); }
  item_t चालक [] (अचिन्हित i) { फर्काऊ thiz()->__item_at__ (i); }
  iter_t& चालक += (अचिन्हित गन्ती) &  { thiz()->__forward__ (गन्ती); फर्काऊ *thiz(); }
  iter_t  चालक += (अचिन्हित गन्ती) && { thiz()->__forward__ (गन्ती); फर्काऊ *thiz(); }
  iter_t& चालक ++ () &  { thiz()->__next__ (); फर्काऊ *thiz(); }
  iter_t  चालक ++ () && { thiz()->__next__ (); फर्काऊ *thiz(); }
  iter_t& चालक -= (अचिन्हित गन्ती) &  { thiz()->__rewind__ (गन्ती); फर्काऊ *thiz(); }
  iter_t  चालक -= (अचिन्हित गन्ती) && { thiz()->__rewind__ (गन्ती); फर्काऊ *thiz(); }
  iter_t& चालक -- () &  { thiz()->__prev__ (); फर्काऊ *thiz(); }
  iter_t  चालक -- () && { thiz()->__prev__ (); फर्काऊ *thiz(); }
  iter_t चालक + (अचिन्हित गन्ती) अचर { स्वतः c = thiz()->iter (); c += गन्ती; फर्काऊ c; }
  मित्र iter_t चालक + (अचिन्हित गन्ती, अचर iter_t &it) { फर्काऊ it + गन्ती; }
  iter_t चालक ++ (पूर्णाङ्क) { iter_t c (*thiz()); ++*thiz(); फर्काऊ c; }
  iter_t चालक - (अचिन्हित गन्ती) अचर { स्वतः c = thiz()->iter (); c -= गन्ती; फर्काऊ c; }
  iter_t चालक -- (पूर्णाङ्क) { iter_t c (*thiz()); --*thiz(); फर्काऊ c; }
  ढाँचा <typename T>
  iter_t& चालक >> (T &v) &  { v = **thiz(); ++*thiz(); फर्काऊ *thiz(); }
  ढाँचा <typename T>
  iter_t  चालक >> (T &v) && { v = **thiz(); ++*thiz(); फर्काऊ *thiz(); }
  ढाँचा <typename T>
  iter_t& चालक << (अचर T v) &  { **thiz() = v; ++*thiz(); फर्काऊ *thiz(); }
  ढाँचा <typename T>
  iter_t  चालक << (अचर T v) && { **thiz() = v; ++*thiz(); फर्काऊ *thiz(); }

  रक्षित:
  hb_iter_t () = स्वभाव;
  hb_iter_t (अचर hb_iter_t &o HB_UNUSED) = स्वभाव;
  hb_iter_t (hb_iter_t &&o HB_UNUSED) = स्वभाव;
  hb_iter_t& चालक = (अचर hb_iter_t &o HB_UNUSED) = स्वभाव;
  hb_iter_t& चालक = (hb_iter_t &&o HB_UNUSED) = स्वभाव;
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
  using Name::चालक तार्किक; \
  using Name::len; \
  using Name::चालक ->; \
  using Name::चालक *; \
  using Name::चालक []; \
  using Name::चालक +=; \
  using Name::चालक ++; \
  using Name::चालक -=; \
  using Name::चालक --; \
  using Name::चालक +; \
  using Name::चालक -; \
  using Name::चालक >>; \
  using Name::चालक <<; \
  static_assert (सत्य, "")

/* Returns iterator / item type of a type. */
ढाँचा <typename Iterable>
using hb_iter_type = decltype (hb_deref (hb_declval (Iterable)).iter ());
ढाँचा <typename Iterable>
using hb_item_type = decltype (*hb_deref (hb_declval (Iterable)).iter ());


ढाँचा <typename> संरचना hb_array_t;
ढाँचा <typename> संरचना hb_sorted_array_t;

संरचना
{
  ढाँचा <typename T> hb_iter_type<T>
  चालक () (T&& c) अचर
  { फर्काऊ hb_deref (std::forward<T> (c)).iter (); }

  /* Specialization for C arrays. */

  ढाँचा <typename Type> अन्तःस्थ hb_array_t<Type>
  चालक () (Type *array, अचिन्हित पूर्णाङ्क length) अचर
  { फर्काऊ hb_array_t<Type> (array, length); }

  ढाँचा <typename Type, अचिन्हित पूर्णाङ्क length> hb_array_t<Type>
  चालक () (Type (&array)[length]) अचर
  { फर्काऊ hb_array_t<Type> (array, length); }

}
HB_FUNCOBJ (hb_iter);
संरचना
{
  ढाँचा <typename T> स्वतः
  impl (T&& c, hb_priority<1>) अचर HB_RETURN (अचिन्हित, c.len ())

  ढाँचा <typename T> स्वतः
  impl (T&& c, hb_priority<0>) अचर HB_RETURN (अचिन्हित, c.len)

  सार्वजनिक:

  ढाँचा <typename T> स्वतः
  चालक () (T&& c) अचर HB_RETURN (अचिन्हित, impl (std::forward<T> (c), hb_prioritize))
}
HB_FUNCOBJ (hb_len);

/* Mixin to fill in what the subclass doesn't provide. */
ढाँचा <typename iter_t, typename item_t = typename iter_t::__item_t__>
संरचना hb_iter_fallback_mixin_t
{
  गुप्त:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  अचर iter_t* thiz () अचर { फर्काऊ static_cast<अचर iter_t *> (एतत्); }
	iter_t* thiz ()       { फर्काऊ static_cast<      iter_t *> (एतत्); }
  सार्वजनिक:

  /* Access: Implement __item__(), or __item_at__() if random-access. */
  item_t __item__ () अचर { फर्काऊ (*thiz())[0]; }
  item_t __item_at__ (अचिन्हित i) अचर { फर्काऊ *(*thiz() + i); }

  /* Termination: Implement __more__(), or __len__() if random-access. */
  तार्किक __more__ () अचर { फर्काऊ तार्किक (thiz()->len ()); }
  अचिन्हित __len__ () अचर
  { iter_t c (*thiz()); अचिन्हित l = 0; जबसम्म (c) { c++; l++; } फर्काऊ l; }

  /* Advancing: Implement __next__(), or __forward__() if random-access. */
  शून्य __next__ () { *thiz() += 1; }
  शून्य __forward__ (अचिन्हित n) { जबसम्म (*thiz() && n--) ++*thiz(); }

  /* Rewinding: Implement __prev__() or __rewind__() if bidirectional. */
  शून्य __prev__ () { *thiz() -= 1; }
  शून्य __rewind__ (अचिन्हित n) { जबसम्म (*thiz() && n--) --*thiz(); }

  /* Range-based for: Implement __end__() if can be done faster,
   * and operator!=. */
  iter_t __end__ () अचर
  {
    यदि (thiz()->is_random_access_iterator)
      फर्काऊ *thiz() + thiz()->len ();
    /* Above expression loops twice. Following loops once. */
    स्वतः it = *thiz();
    जबसम्म (it) ++it;
    फर्काऊ it;
  }

  रक्षित:
  hb_iter_fallback_mixin_t () = स्वभाव;
  hb_iter_fallback_mixin_t (अचर hb_iter_fallback_mixin_t &o HB_UNUSED) = स्वभाव;
  hb_iter_fallback_mixin_t (hb_iter_fallback_mixin_t &&o HB_UNUSED) = स्वभाव;
  hb_iter_fallback_mixin_t& चालक = (अचर hb_iter_fallback_mixin_t &o HB_UNUSED) = स्वभाव;
  hb_iter_fallback_mixin_t& चालक = (hb_iter_fallback_mixin_t &&o HB_UNUSED) = स्वभाव;
};

ढाँचा <typename iter_t, typename item_t = typename iter_t::__item_t__>
संरचना hb_iter_with_fallback_t :
  hb_iter_t<iter_t, item_t>,
  hb_iter_fallback_mixin_t<iter_t, item_t>
{
  रक्षित:
  hb_iter_with_fallback_t () = स्वभाव;
  hb_iter_with_fallback_t (अचर hb_iter_with_fallback_t &o HB_UNUSED) = स्वभाव;
  hb_iter_with_fallback_t (hb_iter_with_fallback_t &&o HB_UNUSED) = स्वभाव;
  hb_iter_with_fallback_t& चालक = (अचर hb_iter_with_fallback_t &o HB_UNUSED) = स्वभाव;
  hb_iter_with_fallback_t& चालक = (hb_iter_with_fallback_t &&o HB_UNUSED) = स्वभाव;
};

/*
 * Meta-programming predicates.
 */

/* hb_is_iterator() / hb_is_iterator_of() */

ढाँचा<typename Iter, typename Item>
संरचना hb_is_iterator_of
{
  ढाँचा <typename Item2 = Item>
  स्थिर hb_true_type impl (hb_priority<2>, hb_iter_t<Iter, hb_type_identity<Item2>> *);
  स्थिर hb_false_type impl (hb_priority<0>, अचर शून्य *);

  सार्वजनिक:
  स्थिर constexpr तार्किक value = decltype (impl (hb_prioritize, hb_declval (Iter*)))::value;
};
#define hb_is_iterator_of(Iter, Item) hb_is_iterator_of<Iter, Item>::value
#define hb_is_iterator(Iter) hb_is_iterator_of (Iter, typename Iter::item_t)
#define hb_is_sorted_iterator_of(Iter, Item) (hb_is_iterator_of<Iter, Item>::value && Iter::is_sorted_iterator)
#define hb_is_sorted_iterator(Iter) hb_is_sorted_iterator_of (Iter, typename Iter::item_t)

/* hb_is_iterable() */

ढाँचा <typename T>
संरचना hb_is_iterable
{
  गुप्त:

  ढाँचा <typename U>
  स्थिर स्वतः impl (hb_priority<1>) -> decltype (hb_declval (U).iter (), hb_true_type ());

  ढाँचा <typename>
  स्थिर hb_false_type impl (hb_priority<0>);

  सार्वजनिक:
  स्थिर constexpr तार्किक value = decltype (impl<T> (hb_prioritize))::value;
};
#define hb_is_iterable(Iterable) hb_is_iterable<Iterable>::value

/* hb_is_source_of() / hb_is_sink_of() */

ढाँचा<typename Iter, typename Item>
संरचना hb_is_source_of
{
  गुप्त:
  ढाँचा <typename Iter2 = Iter,
	    hb_enable_if (hb_is_convertible (typename Iter2::item_t, hb_add_lvalue_reference<अचर Item>))>
  स्थिर hb_true_type impl (hb_priority<2>);
  ढाँचा <typename Iter2 = Iter>
  स्थिर स्वतः impl (hb_priority<1>) -> decltype (hb_declval (Iter2) >> hb_declval (Item &), hb_true_type ());
  स्थिर hb_false_type impl (hb_priority<0>);

  सार्वजनिक:
  स्थिर constexpr तार्किक value = decltype (impl (hb_prioritize))::value;
};
#define hb_is_source_of(Iter, Item) hb_is_source_of<Iter, Item>::value

ढाँचा<typename Iter, typename Item>
संरचना hb_is_sink_of
{
  गुप्त:
  ढाँचा <typename Iter2 = Iter,
	    hb_enable_if (hb_is_convertible (typename Iter2::item_t, hb_add_lvalue_reference<Item>))>
  स्थिर hb_true_type impl (hb_priority<2>);
  ढाँचा <typename Iter2 = Iter>
  स्थिर स्वतः impl (hb_priority<1>) -> decltype (hb_declval (Iter2) << hb_declval (Item), hb_true_type ());
  स्थिर hb_false_type impl (hb_priority<0>);

  सार्वजनिक:
  स्थिर constexpr तार्किक value = decltype (impl (hb_prioritize))::value;
};
#define hb_is_sink_of(Iter, Item) hb_is_sink_of<Iter, Item>::value

/* This is commonly used, so define: */
#define hb_is_sorted_source_of(Iter, Item) \
	(hb_is_source_of(Iter, Item) && Iter::is_sorted_iterator)


संरचना
{
  ढाँचा <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  अचिन्हित चालक () (अचर Iterable &_) अचर { फर्काऊ hb_len (hb_iter (_)); }

  अचिन्हित चालक () (अचिन्हित _) अचर { फर्काऊ _; }
}
HB_FUNCOBJ (hb_len_of);

/* Range-based 'for' for iterables. */

ढाँचा <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
स्थिर अन्तःस्थ स्वतः begin (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).begin ())

ढाँचा <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
स्थिर अन्तःस्थ स्वतः end (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).end ())

/* begin()/end() are NOT looked up non-ADL.  So each namespace must declare them.
 * Do it for namespace OT. */
नामाकाश OT {

ढाँचा <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
स्थिर अन्तःस्थ स्वतः begin (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).begin ())

ढाँचा <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
स्थिर अन्तःस्थ स्वतः end (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).end ())

}


/*
 * Adaptors, combiners, etc.
 */

ढाँचा <typename Lhs, typename Rhs,
	  hb_requires (hb_is_iterator (Lhs))>
स्थिर अन्तःस्थ स्वतः
चालक | (Lhs&& lhs, Rhs&& rhs) HB_AUTO_RETURN (std::forward<Rhs> (rhs) (std::forward<Lhs> (lhs)))

/* hb_map(), hb_filter(), hb_reduce() */

गणना  वर्ग hb_function_sortedness_t {
  NOT_SORTED,
  RETAINS_SORTING,
  SORTED,
};

ढाँचा <typename Iter, typename Proj, hb_function_sortedness_t Sorted,
	 hb_requires (hb_is_iterator (Iter))>
संरचना hb_map_iter_t :
  hb_iter_t<hb_map_iter_t<Iter, Proj, Sorted>,
	    decltype (hb_get (hb_declval (Proj), *hb_declval (Iter)))>
{
  hb_map_iter_t (अचर Iter& it, Proj f_) : it (it), f (f_) {}

  प्रकारनिर्देश decltype (hb_get (hb_declval (Proj), *hb_declval (Iter))) __item_t__;
  स्थिर constexpr तार्किक is_random_access_iterator = Iter::is_random_access_iterator;
  स्थिर constexpr तार्किक is_sorted_iterator =
    Sorted == hb_function_sortedness_t::SORTED ? सत्य :
    Sorted == hb_function_sortedness_t::RETAINS_SORTING ? Iter::is_sorted_iterator :
    असत्य;
  __item_t__ __item__ () अचर { फर्काऊ hb_get (f.get (), *it); }
  __item_t__ __item_at__ (अचिन्हित i) अचर { फर्काऊ hb_get (f.get (), it[i]); }
  तार्किक __more__ () अचर { फर्काऊ तार्किक (it); }
  अचिन्हित __len__ () अचर { फर्काऊ it.len (); }
  शून्य __next__ () { ++it; }
  शून्य __forward__ (अचिन्हित n) { it += n; }
  शून्य __prev__ () { --it; }
  शून्य __rewind__ (अचिन्हित n) { it -= n; }
  hb_map_iter_t __end__ () अचर { फर्काऊ hb_map_iter_t (it._end (), f); }
  तार्किक चालक != (अचर hb_map_iter_t& o) अचर
  { फर्काऊ it != o.it; }

  गुप्त:
  Iter it;
  mutable hb_reference_wrapper<Proj> f;
};

ढाँचा <typename Proj, hb_function_sortedness_t Sorted>
संरचना hb_map_iter_factory_t
{
  hb_map_iter_factory_t (Proj f) : f (f) {}

  ढाँचा <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  hb_map_iter_t<Iter, Proj, Sorted>
  चालक () (Iter it)
  { फर्काऊ hb_map_iter_t<Iter, Proj, Sorted> (it, f); }

  गुप्त:
  Proj f;
};
संरचना
{
  ढाँचा <typename Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::NOT_SORTED>
  चालक () (Proj&& f) अचर
  { फर्काऊ hb_map_iter_factory_t<Proj, hb_function_sortedness_t::NOT_SORTED> (f); }
}
HB_FUNCOBJ (hb_map);
संरचना
{
  ढाँचा <typename Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::RETAINS_SORTING>
  चालक () (Proj&& f) अचर
  { फर्काऊ hb_map_iter_factory_t<Proj, hb_function_sortedness_t::RETAINS_SORTING> (f); }
}
HB_FUNCOBJ (hb_map_retains_sorting);
संरचना
{
  ढाँचा <typename Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::SORTED>
  चालक () (Proj&& f) अचर
  { फर्काऊ hb_map_iter_factory_t<Proj, hb_function_sortedness_t::SORTED> (f); }
}
HB_FUNCOBJ (hb_map_sorted);

ढाँचा <typename Iter, typename Pred, typename Proj,
	 hb_requires (hb_is_iterator (Iter))>
संरचना hb_filter_iter_t :
  hb_iter_with_fallback_t<hb_filter_iter_t<Iter, Pred, Proj>,
			  typename Iter::item_t>
{
  hb_filter_iter_t (अचर Iter& it_, Pred p_, Proj f_) : it (it_), p (p_), f (f_)
  { जबसम्म (it && !hb_has (p.get (), hb_get (f.get (), *it))) ++it; }

  प्रकारनिर्देश typename Iter::item_t __item_t__;
  स्थिर constexpr तार्किक is_sorted_iterator = Iter::is_sorted_iterator;
  __item_t__ __item__ () अचर { फर्काऊ *it; }
  तार्किक __more__ () अचर { फर्काऊ तार्किक (it); }
  शून्य __next__ () { गर ++it; जबसम्म (it && !hb_has (p.get (), hb_get (f.get (), *it))); }
  शून्य __prev__ ()
  {
    अचिन्हित गन्ती = 0;
    गर
    {
      Iter old = it;
      --it;
      यदि (!(it != old))
      {
	it += गन्ती;
	फर्काऊ;
      }
      गन्ती++;
    }
    जबसम्म (!hb_has (p.get (), hb_get (f.get (), *it)));
  }
  hb_filter_iter_t __end__ () अचर { फर्काऊ hb_filter_iter_t (it._end (), p, f); }
  तार्किक चालक != (अचर hb_filter_iter_t& o) अचर
  { फर्काऊ it != o.it; }

  गुप्त:
  Iter it;
  mutable hb_reference_wrapper<Pred> p;
  mutable hb_reference_wrapper<Proj> f;
};
ढाँचा <typename Pred, typename Proj>
संरचना hb_filter_iter_factory_t
{
  hb_filter_iter_factory_t (Pred p, Proj f) : p (p), f (f) {}

  ढाँचा <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  hb_filter_iter_t<Iter, Pred, Proj>
  चालक () (Iter it)
  { फर्काऊ hb_filter_iter_t<Iter, Pred, Proj> (it, p, f); }

  गुप्त:
  Pred p;
  Proj f;
};
संरचना
{
  ढाँचा <typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity))>
  hb_filter_iter_factory_t<Pred, Proj>
  चालक () (Pred&& p = hb_identity, Proj&& f = hb_identity) अचर
  { फर्काऊ hb_filter_iter_factory_t<Pred, Proj> (p, f); }
}
HB_FUNCOBJ (hb_filter);

ढाँचा <typename Redu, typename InitT>
संरचना hb_reduce_t
{
  hb_reduce_t (Redu r, InitT init_मान) : r (r), init_मान (init_मान) {}

  ढाँचा <typename Iter,
	    hb_requires (hb_is_iterator (Iter)),
	    typename AccuT = hb_decay<decltype (hb_declval (Redu) (hb_declval (InitT), hb_declval (typename Iter::item_t)))>>
  AccuT
  चालक () (Iter it)
  {
    AccuT value = init_मान;
    लागि (; it; ++it)
      value = r (value, *it);
    फर्काऊ value;
  }

  गुप्त:
  Redu r;
  InitT init_मान;
};
संरचना
{
  ढाँचा <typename Redu, typename InitT>
  hb_reduce_t<Redu, InitT>
  चालक () (Redu&& r, InitT init_मान) अचर
  { फर्काऊ hb_reduce_t<Redu, InitT> (r, init_मान); }
}
HB_FUNCOBJ (hb_reduce);


/* hb_zip() */

ढाँचा <typename A, typename B>
संरचना hb_zip_iter_t :
  hb_iter_t<hb_zip_iter_t<A, B>,
	    hb_pair_t<typename A::item_t, typename B::item_t>>
{
  hb_zip_iter_t () {}
  hb_zip_iter_t (अचर A& a, अचर B& b) : a (a), b (b) {}

  प्रकारनिर्देश hb_pair_t<typename A::item_t, typename B::item_t> __item_t__;
  स्थिर constexpr तार्किक is_random_access_iterator =
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
  स्थिर constexpr तार्किक is_sorted_iterator = A::is_sorted_iterator;

  __item_t__ __item__ () अचर { फर्काऊ __item_t__ (*a, *b); }
  __item_t__ __item_at__ (अचिन्हित i) अचर { फर्काऊ __item_t__ (a[i], b[i]); }
  तार्किक __more__ () अचर { फर्काऊ तार्किक (a) && तार्किक (b); }
  अचिन्हित __len__ () अचर { फर्काऊ hb_min (a.len (), b.len ()); }
  शून्य __next__ () { ++a; ++b; }
  शून्य __forward__ (अचिन्हित n) { a += n; b += n; }
  शून्य __prev__ () { --a; --b; }
  शून्य __rewind__ (अचिन्हित n) { a -= n; b -= n; }
  hb_zip_iter_t __end__ () अचर { फर्काऊ hb_zip_iter_t (a._end (), b._end ()); }
  /* Note, we should stop if ANY of the iters reaches end.  As such two compare
   * unequal if both items are unequal, NOT if either is unequal. */
  तार्किक चालक != (अचर hb_zip_iter_t& o) अचर
  { फर्काऊ a != o.a && b != o.b; }

  गुप्त:
  A a;
  B b;
};
संरचना
{ HB_PARTIALIZE(2);
  ढाँचा <typename A, typename B,
	    hb_requires (hb_is_iterable (A) && hb_is_iterable (B))>
  hb_zip_iter_t<hb_iter_type<A>, hb_iter_type<B>>
  चालक () (A&& a, B&& b) अचर
  { फर्काऊ hb_zip_iter_t<hb_iter_type<A>, hb_iter_type<B>> (hb_iter (a), hb_iter (b)); }
}
HB_FUNCOBJ (hb_zip);

/* hb_concat() */

ढाँचा <typename A, typename B>
संरचना hb_concat_iter_t :
    hb_iter_t<hb_concat_iter_t<A, B>, typename A::item_t>
{
  hb_concat_iter_t () {}
  hb_concat_iter_t (A& a, B& b) : a (a), b (b) {}
  hb_concat_iter_t (अचर A& a, अचर B& b) : a (a), b (b) {}


  प्रकारनिर्देश typename A::item_t __item_t__;
  स्थिर constexpr तार्किक is_random_access_iterator =
    A::is_random_access_iterator &&
    B::is_random_access_iterator;
  स्थिर constexpr तार्किक is_sorted_iterator = असत्य;

  __item_t__ __item__ () अचर
  {
    यदि (!a)
      फर्काऊ *b;
    फर्काऊ *a;
  }

  __item_t__ __item_at__ (अचिन्हित i) अचर
  {
    अचिन्हित a_len = a.len ();
    यदि (i < a_len)
      फर्काऊ a[i];
    फर्काऊ b[i - a_len];
  }

  तार्किक __more__ () अचर { फर्काऊ तार्किक (a) || तार्किक (b); }

  अचिन्हित __len__ () अचर { फर्काऊ a.len () + b.len (); }

  शून्य __next__ ()
  {
    यदि (a)
      ++a;
    अन्यथा
      ++b;
  }

  शून्य __forward__ (अचिन्हित n)
  {
    यदि (!n) फर्काऊ;
    यदि (!is_random_access_iterator) {
      जबसम्म (n-- && *एतत्) {
        (*एतत्)++;
      }
      फर्काऊ;
    }

    अचिन्हित a_len = a.len ();
    यदि (n > a_len) {
      n -= a_len;
      a.__forward__ (a_len);
      b.__forward__ (n);
    } अन्यथा {
      a.__forward__ (n);
    }
  }

  hb_concat_iter_t __end__ () अचर { फर्काऊ hb_concat_iter_t (a._end (), b._end ()); }
  तार्किक चालक != (अचर hb_concat_iter_t& o) अचर
  {
    फर्काऊ a != o.a
        || b != o.b;
  }

  गुप्त:
  A a;
  B b;
};
संरचना
{ HB_PARTIALIZE(2);
  ढाँचा <typename A, typename B,
	    hb_requires (hb_is_iterable (A) && hb_is_iterable (B))>
  hb_concat_iter_t<hb_iter_type<A>, hb_iter_type<B>>
  चालक () (A&& a, B&& b) अचर
  { फर्काऊ hb_concat_iter_t<hb_iter_type<A>, hb_iter_type<B>> (hb_iter (a), hb_iter (b)); }
}
HB_FUNCOBJ (hb_concat);

/* hb_apply() */

ढाँचा <typename Appl>
संरचना hb_apply_t
{
  hb_apply_t (Appl a) : a (a) {}

  ढाँचा <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  शून्य चालक () (Iter it)
  {
    लागि (; it; ++it)
      (शून्य) hb_invoke (a, *it);
  }

  गुप्त:
  Appl a;
};
संरचना
{
  ढाँचा <typename Appl> hb_apply_t<Appl>
  चालक () (Appl&& a) अचर
  { फर्काऊ hb_apply_t<Appl> (a); }

  ढाँचा <typename Appl> hb_apply_t<Appl&>
  चालक () (Appl *a) अचर
  { फर्काऊ hb_apply_t<Appl&> (*a); }
}
HB_FUNCOBJ (hb_apply);

/* hb_range()/hb_iota()/hb_repeat() */

ढाँचा <typename T, typename S>
संरचना hb_range_iter_t :
  hb_iter_t<hb_range_iter_t<T, S>, T>
{
  hb_range_iter_t (T start, T end_, S step) : v (start), end_ (end_for (start, end_, step)), step (step) {}

  प्रकारनिर्देश T __item_t__;
  स्थिर constexpr तार्किक is_random_access_iterator = सत्य;
  स्थिर constexpr तार्किक is_sorted_iterator = सत्य;
  __item_t__ __item__ () अचर { फर्काऊ hb_ridentity (v); }
  __item_t__ __item_at__ (अचिन्हित j) अचर { फर्काऊ v + j * step; }
  तार्किक __more__ () अचर { फर्काऊ v != end_; }
  अचिन्हित __len__ () अचर { फर्काऊ !step ? अच_पूर्णांक_उच्च : (end_ - v) / step; }
  शून्य __next__ () { v += step; }
  शून्य __forward__ (अचिन्हित n) { v += n * step; }
  शून्य __prev__ () { v -= step; }
  शून्य __rewind__ (अचिन्हित n) { v -= n * step; }
  hb_range_iter_t __end__ () अचर { फर्काऊ hb_range_iter_t (end_, end_, step); }
  तार्किक चालक != (अचर hb_range_iter_t& o) अचर
  { फर्काऊ v != o.v; }

  गुप्त:
  स्थिर अन्तःस्थ T end_for (T start, T end_, S step)
  {
    यदि (!step)
      फर्काऊ end_;
    स्वतः res = (end_ - start) % step;
    यदि (!res)
      फर्काऊ end_;
    end_ += step - res;
    फर्काऊ end_;
  }

  गुप्त:
  T v;
  T end_;
  S step;
};
संरचना
{
  ढाँचा <typename T = अचिन्हित> hb_range_iter_t<T, अचिन्हित>
  चालक () (T end = (अचिन्हित) -1) अचर
  { फर्काऊ hb_range_iter_t<T, अचिन्हित> (0, end, 1u); }

  ढाँचा <typename T, typename S = अचिन्हित> hb_range_iter_t<T, S>
  चालक () (T start, T end, S step = 1u) अचर
  { फर्काऊ hb_range_iter_t<T, S> (start, end, step); }
}
HB_FUNCOBJ (hb_range);

ढाँचा <typename T, typename S>
संरचना hb_iota_iter_t :
  hb_iter_with_fallback_t<hb_iota_iter_t<T, S>, T>
{
  hb_iota_iter_t (T start, S step) : v (start), step (step) {}

  गुप्त:

  ढाँचा <typename S2 = S>
  स्वतः
  inc (hb_type_identity<S2> s, hb_priority<1>)
    -> hb_void_t<decltype (hb_invoke (std::forward<hb_type_identity<S2>> (s),
                                      hb_declval<T&> ()))>
  { v = hb_invoke (std::forward<hb_type_identity<S2>> (s), v); }

  शून्य
  inc (S s, hb_priority<0>)
  { v += s; }

  सार्वजनिक:

  प्रकारनिर्देश T __item_t__;
  स्थिर constexpr तार्किक is_random_access_iterator = सत्य;
  स्थिर constexpr तार्किक is_sorted_iterator = सत्य;
  __item_t__ __item__ () अचर { फर्काऊ hb_ridentity (v); }
  तार्किक __more__ () अचर { फर्काऊ सत्य; }
  अचिन्हित __len__ () अचर { फर्काऊ अच_पूर्णांक_उच्च; }
  शून्य __next__ () { inc (step, hb_prioritize); }
  शून्य __prev__ () { v -= step; }
  hb_iota_iter_t __end__ () अचर { फर्काऊ *एतत्; }
  तार्किक चालक != (अचर hb_iota_iter_t& o) अचर { फर्काऊ सत्य; }

  गुप्त:
  T v;
  S step;
};
संरचना
{
  ढाँचा <typename T = अचिन्हित, typename S = अचिन्हित> hb_iota_iter_t<T, S>
  चालक () (T start = 0u, S step = 1u) अचर
  { फर्काऊ hb_iota_iter_t<T, S> (start, step); }
}
HB_FUNCOBJ (hb_iota);

ढाँचा <typename T>
संरचना hb_repeat_iter_t :
  hb_iter_t<hb_repeat_iter_t<T>, T>
{
  hb_repeat_iter_t (T value) : v (value) {}

  प्रकारनिर्देश T __item_t__;
  स्थिर constexpr तार्किक is_random_access_iterator = सत्य;
  स्थिर constexpr तार्किक is_sorted_iterator = सत्य;
  __item_t__ __item__ () अचर { फर्काऊ v; }
  __item_t__ __item_at__ (अचिन्हित j) अचर { फर्काऊ v; }
  तार्किक __more__ () अचर { फर्काऊ सत्य; }
  अचिन्हित __len__ () अचर { फर्काऊ अच_पूर्णांक_उच्च; }
  शून्य __next__ () {}
  शून्य __forward__ (अचिन्हित) {}
  शून्य __prev__ () {}
  शून्य __rewind__ (अचिन्हित) {}
  hb_repeat_iter_t __end__ () अचर { फर्काऊ *एतत्; }
  तार्किक चालक != (अचर hb_repeat_iter_t& o) अचर { फर्काऊ सत्य; }

  गुप्त:
  T v;
};
संरचना
{
  ढाँचा <typename T> hb_repeat_iter_t<T>
  चालक () (T value) अचर
  { फर्काऊ hb_repeat_iter_t<T> (value); }
}
HB_FUNCOBJ (hb_repeat);

/* hb_enumerate()/hb_take() */

संरचना
{
  ढाँचा <typename Iterable,
	    typename Index = अचिन्हित,
	    hb_requires (hb_is_iterable (Iterable))>
  स्वतः चालक () (Iterable&& it, Index start = 0u) अचर HB_AUTO_RETURN
  ( hb_zip (hb_iota (start), it) )
}
HB_FUNCOBJ (hb_enumerate);

संरचना
{ HB_PARTIALIZE(2);
  ढाँचा <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  स्वतः चालक () (Iterable&& it, अचिन्हित गन्ती) अचर HB_AUTO_RETURN
  ( hb_zip (hb_range (गन्ती), it) | hb_map_retains_sorting (hb_second) )

  /* Specialization arrays. */

  ढाँचा <typename Type> अन्तःस्थ hb_array_t<Type>
  चालक () (hb_array_t<Type> array, अचिन्हित गन्ती) अचर
  { फर्काऊ array.sub_array (0, गन्ती); }

  ढाँचा <typename Type> अन्तःस्थ hb_sorted_array_t<Type>
  चालक () (hb_sorted_array_t<Type> array, अचिन्हित गन्ती) अचर
  { फर्काऊ array.sub_array (0, गन्ती); }
}
HB_FUNCOBJ (hb_take);

संरचना
{ HB_PARTIALIZE(2);
  ढाँचा <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  स्वतः चालक () (Iter it, अचिन्हित गन्ती) अचर HB_AUTO_RETURN
  (
    + hb_iota (it, hb_add (गन्ती))
    | hb_map (hb_take (गन्ती))
    | hb_take ((hb_len (it) + गन्ती - 1) / गन्ती)
  )
}
HB_FUNCOBJ (hb_chop);

/* hb_sink() */

ढाँचा <typename Sink>
संरचना hb_sink_t
{
  hb_sink_t (Sink s) : s (s) {}

  ढाँचा <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  शून्य चालक () (Iter it)
  {
    लागि (; it; ++it)
      s << *it;
  }

  गुप्त:
  Sink s;
};
संरचना
{
  ढाँचा <typename Sink> hb_sink_t<Sink>
  चालक () (Sink&& s) अचर
  { फर्काऊ hb_sink_t<Sink> (s); }

  ढाँचा <typename Sink> hb_sink_t<Sink&>
  चालक () (Sink *s) अचर
  { फर्काऊ hb_sink_t<Sink&> (*s); }
}
HB_FUNCOBJ (hb_sink);

/* hb-drain: hb_sink to void / blackhole / /dev/null. */

संरचना
{
  ढाँचा <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  शून्य चालक () (Iter it) अचर
  {
    लागि (; it; ++it)
      (शून्य) *it;
  }
}
HB_FUNCOBJ (hb_drain);

/* hb_unzip(): unzip and sink to two sinks. */

ढाँचा <typename Sink1, typename Sink2>
संरचना hb_unzip_t
{
  hb_unzip_t (Sink1 s1, Sink2 s2) : s1 (s1), s2 (s2) {}

  ढाँचा <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  शून्य चालक () (Iter it)
  {
    लागि (; it; ++it)
    {
      अचर स्वतः &v = *it;
      s1 << v.first;
      s2 << v.second;
    }
  }

  गुप्त:
  Sink1 s1;
  Sink2 s2;
};
संरचना
{
  ढाँचा <typename Sink1, typename Sink2> hb_unzip_t<Sink1, Sink2>
  चालक () (Sink1&& s1, Sink2&& s2) अचर
  { फर्काऊ hb_unzip_t<Sink1, Sink2> (s1, s2); }

  ढाँचा <typename Sink1, typename Sink2> hb_unzip_t<Sink1&, Sink2&>
  चालक () (Sink1 *s1, Sink2 *s2) अचर
  { फर्काऊ hb_unzip_t<Sink1&, Sink2&> (*s1, *s2); }
}
HB_FUNCOBJ (hb_unzip);


/* hb-all, hb-any, hb-none. */

संरचना
{
  ढाँचा <typename Iterable,
	    typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity)),
	    hb_requires (hb_is_iterable (Iterable))>
  तार्किक चालक () (Iterable&& c,
		    Pred&& p = hb_identity,
		    Proj&& f = hb_identity) अचर
  {
    लागि (स्वतः it = hb_iter (c); it; ++it)
      यदि (!hb_match (p, hb_get (f, *it)))
	फर्काऊ असत्य;
    फर्काऊ सत्य;
  }
}
HB_FUNCOBJ (hb_all);
संरचना
{
  ढाँचा <typename Iterable,
	    typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity)),
	    hb_requires (hb_is_iterable (Iterable))>
  तार्किक चालक () (Iterable&& c,
		    Pred&& p = hb_identity,
		    Proj&& f = hb_identity) अचर
  {
    लागि (स्वतः it = hb_iter (c); it; ++it)
      यदि (hb_match (p, hb_get (f, *it)))
	फर्काऊ सत्य;
    फर्काऊ असत्य;
  }
}
HB_FUNCOBJ (hb_any);
संरचना
{
  ढाँचा <typename Iterable,
	    typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity)),
	    hb_requires (hb_is_iterable (Iterable))>
  तार्किक चालक () (Iterable&& c,
		    Pred&& p = hb_identity,
		    Proj&& f = hb_identity) अचर
  {
    लागि (स्वतः it = hb_iter (c); it; ++it)
      यदि (hb_match (p, hb_get (f, *it)))
	फर्काऊ असत्य;
    फर्काऊ सत्य;
  }
}
HB_FUNCOBJ (hb_none);

/*
 * Algorithms operating on iterators.
 */

ढाँचा <typename C, typename V,
	  hb_requires (hb_is_iterable (C))>
अन्तःस्थ शून्य
hb_fill (C&& c, अचर V &v)
{
  लागि (स्वतः i = hb_iter (c); i; i++)
    *i = v;
}

ढाँचा <typename S, typename D>
अन्तःस्थ शून्य
hb_copy (S&& is, D&& id)
{
  hb_iter (is) | hb_sink (id);
}


#endif /* HB_ITER_HH */

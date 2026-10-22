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

#अगर_अघोषित HB_ITER_HH
#घोषणा HB_ITER_HH

#समावेश "hb.hh"
#समावेश "hb-algs.hh"
#समावेश "hb-meta.hh"


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
काष्ठा hb_iter_t
{
  प्रकार Item item_t;
  constexpr अचिन्हित get_item_आकार () स्थिर { वापस hb_static_आकार (Item); }
  अटल constexpr bool is_iterator = true;
  अटल constexpr bool is_random_access_iterator = false;
  अटल constexpr bool is_sorted_iterator = false;
  अटल constexpr bool has_fast_len = false; // Should be checked in combination with is_random_access_iterator.

  निजी:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  स्थिर iter_t* thiz () स्थिर { वापस static_cast<स्थिर iter_t *> (यह); }
	iter_t* thiz ()       { वापस static_cast<      iter_t *> (यह); }
  खुला:

  /* Operators. */
  iter_t iter () स्थिर { वापस *thiz(); }
  iter_t चालक + () स्थिर { वापस *thiz(); }
  iter_t _begin () स्थिर { वापस *thiz(); }
  iter_t begin () स्थिर { वापस _begin (); }
  iter_t _end () स्थिर { वापस thiz()->__end__ (); }
  iter_t end () स्थिर { वापस _end (); }
  explicit चालक bool () स्थिर { वापस thiz()->__more__ (); }
  अचिन्हित len () स्थिर { वापस thiz()->__len__ (); }
  /* The following can only be enabled if item_t is reference type.  Otherwise
   * it will be returning pointer to temporary rvalue. */
  ढाँचा <typename T = item_t,
	    hb_enable_if (std::is_reference<T>::value)>
  hb_remove_reference<item_t>* चालक -> () स्थिर { वापस std::addressof (**thiz()); }
  item_t चालक * () स्थिर { वापस thiz()->__item__ (); }
  item_t चालक * () { वापस thiz()->__item__ (); }
  item_t चालक [] (अचिन्हित i) स्थिर { वापस thiz()->__item_at__ (i); }
  item_t चालक [] (अचिन्हित i) { वापस thiz()->__item_at__ (i); }
  iter_t& चालक += (अचिन्हित गिनती) &  { thiz()->__forward__ (गिनती); वापस *thiz(); }
  iter_t  चालक += (अचिन्हित गिनती) && { thiz()->__forward__ (गिनती); वापस *thiz(); }
  iter_t& चालक ++ () &  { thiz()->__next__ (); वापस *thiz(); }
  iter_t  चालक ++ () && { thiz()->__next__ (); वापस *thiz(); }
  iter_t& चालक -= (अचिन्हित गिनती) &  { thiz()->__rewind__ (गिनती); वापस *thiz(); }
  iter_t  चालक -= (अचिन्हित गिनती) && { thiz()->__rewind__ (गिनती); वापस *thiz(); }
  iter_t& चालक -- () &  { thiz()->__prev__ (); वापस *thiz(); }
  iter_t  चालक -- () && { thiz()->__prev__ (); वापस *thiz(); }
  iter_t चालक + (अचिन्हित गिनती) स्थिर { स्वतः c = thiz()->iter (); c += गिनती; वापस c; }
  मित्र iter_t चालक + (अचिन्हित गिनती, स्थिर iter_t &it) { वापस it + गिनती; }
  iter_t चालक ++ (पूर्णांक) { iter_t c (*thiz()); ++*thiz(); वापस c; }
  iter_t चालक - (अचिन्हित गिनती) स्थिर { स्वतः c = thiz()->iter (); c -= गिनती; वापस c; }
  iter_t चालक -- (पूर्णांक) { iter_t c (*thiz()); --*thiz(); वापस c; }
  ढाँचा <typename T>
  iter_t& चालक >> (T &v) &  { v = **thiz(); ++*thiz(); वापस *thiz(); }
  ढाँचा <typename T>
  iter_t  चालक >> (T &v) && { v = **thiz(); ++*thiz(); वापस *thiz(); }
  ढाँचा <typename T>
  iter_t& चालक << (स्थिर T v) &  { **thiz() = v; ++*thiz(); वापस *thiz(); }
  ढाँचा <typename T>
  iter_t  चालक << (स्थिर T v) && { **thiz() = v; ++*thiz(); वापस *thiz(); }

  रक्षित:
  hb_iter_t () = शेष;
  hb_iter_t (स्थिर hb_iter_t &o HB_UNUSED) = शेष;
  hb_iter_t (hb_iter_t &&o HB_UNUSED) = शेष;
  hb_iter_t& चालक = (स्थिर hb_iter_t &o HB_UNUSED) = शेष;
  hb_iter_t& चालक = (hb_iter_t &&o HB_UNUSED) = शेष;
};

#घोषणा HB_ITER_USING(Name) \
  using item_t = typename Name::item_t; \
  using Name::_begin; \
  using Name::begin; \
  using Name::_end; \
  using Name::end; \
  using Name::get_item_आकार; \
  using Name::is_iterator; \
  using Name::iter; \
  using Name::चालक bool; \
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
  static_assert (true, "")

/* Returns iterator / item type of a type. */
ढाँचा <typename Iterable>
using hb_iter_type = decltype (hb_deref (hb_declval (Iterable)).iter ());
ढाँचा <typename Iterable>
using hb_item_type = decltype (*hb_deref (hb_declval (Iterable)).iter ());


ढाँचा <typename> काष्ठा hb_array_t;
ढाँचा <typename> काष्ठा hb_sorted_array_t;

काष्ठा
{
  ढाँचा <typename T> hb_iter_type<T>
  चालक () (T&& c) स्थिर
  { वापस hb_deref (std::forward<T> (c)).iter (); }

  /* Specialization for C arrays. */

  ढाँचा <typename Type> अंतरभूत hb_array_t<Type>
  चालक () (Type *array, अचिन्हित पूर्णांक लंबाई) स्थिर
  { वापस hb_array_t<Type> (array, लंबाई); }

  ढाँचा <typename Type, अचिन्हित पूर्णांक लंबाई> hb_array_t<Type>
  चालक () (Type (&array)[लंबाई]) स्थिर
  { वापस hb_array_t<Type> (array, लंबाई); }

}
HB_FUNCOBJ (hb_iter);
काष्ठा
{
  ढाँचा <typename T> स्वतः
  impl (T&& c, hb_priority<1>) स्थिर HB_RETURN (अचिन्हित, c.len ())

  ढाँचा <typename T> स्वतः
  impl (T&& c, hb_priority<0>) स्थिर HB_RETURN (अचिन्हित, c.len)

  खुला:

  ढाँचा <typename T> स्वतः
  चालक () (T&& c) स्थिर HB_RETURN (अचिन्हित, impl (std::forward<T> (c), hb_prioritize))
}
HB_FUNCOBJ (hb_len);

/* Mixin to fill in what the subclass doesn't provide. */
ढाँचा <typename iter_t, typename item_t = typename iter_t::__item_t__>
काष्ठा hb_iter_fallback_mixin_t
{
  निजी:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  स्थिर iter_t* thiz () स्थिर { वापस static_cast<स्थिर iter_t *> (यह); }
	iter_t* thiz ()       { वापस static_cast<      iter_t *> (यह); }
  खुला:

  /* Access: Implement __item__(), or __item_at__() if random-access. */
  item_t __item__ () स्थिर { वापस (*thiz())[0]; }
  item_t __item_at__ (अचिन्हित i) स्थिर { वापस *(*thiz() + i); }

  /* Termination: Implement __more__(), or __len__() if random-access. */
  bool __more__ () स्थिर { वापस bool (thiz()->len ()); }
  अचिन्हित __len__ () स्थिर
  { iter_t c (*thiz()); अचिन्हित l = 0; जबतक (c) { c++; l++; } वापस l; }

  /* Advancing: Implement __next__(), or __forward__() if random-access. */
  व्योम __next__ () { *thiz() += 1; }
  व्योम __forward__ (अचिन्हित n) { जबतक (*thiz() && n--) ++*thiz(); }

  /* Rewinding: Implement __prev__() or __rewind__() if bidirectional. */
  व्योम __prev__ () { *thiz() -= 1; }
  व्योम __rewind__ (अचिन्हित n) { जबतक (*thiz() && n--) --*thiz(); }

  /* Range-based for: Implement __end__() if can be done faster,
   * and operator!=. */
  iter_t __end__ () स्थिर
  {
    अगर (thiz()->is_random_access_iterator)
      वापस *thiz() + thiz()->len ();
    /* Above expression loops twice. Following loops once. */
    स्वतः it = *thiz();
    जबतक (it) ++it;
    वापस it;
  }

  रक्षित:
  hb_iter_fallback_mixin_t () = शेष;
  hb_iter_fallback_mixin_t (स्थिर hb_iter_fallback_mixin_t &o HB_UNUSED) = शेष;
  hb_iter_fallback_mixin_t (hb_iter_fallback_mixin_t &&o HB_UNUSED) = शेष;
  hb_iter_fallback_mixin_t& चालक = (स्थिर hb_iter_fallback_mixin_t &o HB_UNUSED) = शेष;
  hb_iter_fallback_mixin_t& चालक = (hb_iter_fallback_mixin_t &&o HB_UNUSED) = शेष;
};

ढाँचा <typename iter_t, typename item_t = typename iter_t::__item_t__>
काष्ठा hb_iter_with_fallback_t :
  hb_iter_t<iter_t, item_t>,
  hb_iter_fallback_mixin_t<iter_t, item_t>
{
  रक्षित:
  hb_iter_with_fallback_t () = शेष;
  hb_iter_with_fallback_t (स्थिर hb_iter_with_fallback_t &o HB_UNUSED) = शेष;
  hb_iter_with_fallback_t (hb_iter_with_fallback_t &&o HB_UNUSED) = शेष;
  hb_iter_with_fallback_t& चालक = (स्थिर hb_iter_with_fallback_t &o HB_UNUSED) = शेष;
  hb_iter_with_fallback_t& चालक = (hb_iter_with_fallback_t &&o HB_UNUSED) = शेष;
};

/*
 * Meta-programming predicates.
 */

/* hb_is_iterator() / hb_is_iterator_of() */

ढाँचा<typename Iter, typename Item>
काष्ठा hb_is_iterator_of
{
  ढाँचा <typename Item2 = Item>
  अटल hb_true_type impl (hb_priority<2>, hb_iter_t<Iter, hb_type_identity<Item2>> *);
  अटल hb_false_type impl (hb_priority<0>, स्थिर व्योम *);

  खुला:
  अटल constexpr bool value = decltype (impl (hb_prioritize, hb_declval (Iter*)))::value;
};
#घोषणा hb_is_iterator_of(Iter, Item) hb_is_iterator_of<Iter, Item>::value
#घोषणा hb_is_iterator(Iter) hb_is_iterator_of (Iter, typename Iter::item_t)
#घोषणा hb_is_sorted_iterator_of(Iter, Item) (hb_is_iterator_of<Iter, Item>::value && Iter::is_sorted_iterator)
#घोषणा hb_is_sorted_iterator(Iter) hb_is_sorted_iterator_of (Iter, typename Iter::item_t)

/* hb_is_iterable() */

ढाँचा <typename T>
काष्ठा hb_is_iterable
{
  निजी:

  ढाँचा <typename U>
  अटल स्वतः impl (hb_priority<1>) -> decltype (hb_declval (U).iter (), hb_true_type ());

  ढाँचा <typename>
  अटल hb_false_type impl (hb_priority<0>);

  खुला:
  अटल constexpr bool value = decltype (impl<T> (hb_prioritize))::value;
};
#घोषणा hb_is_iterable(Iterable) hb_is_iterable<Iterable>::value

/* hb_is_source_of() / hb_is_sink_of() */

ढाँचा<typename Iter, typename Item>
काष्ठा hb_is_source_of
{
  निजी:
  ढाँचा <typename Iter2 = Iter,
	    hb_enable_if (hb_is_convertible (typename Iter2::item_t, hb_add_lvalue_reference<स्थिर Item>))>
  अटल hb_true_type impl (hb_priority<2>);
  ढाँचा <typename Iter2 = Iter>
  अटल स्वतः impl (hb_priority<1>) -> decltype (hb_declval (Iter2) >> hb_declval (Item &), hb_true_type ());
  अटल hb_false_type impl (hb_priority<0>);

  खुला:
  अटल constexpr bool value = decltype (impl (hb_prioritize))::value;
};
#घोषणा hb_is_source_of(Iter, Item) hb_is_source_of<Iter, Item>::value

ढाँचा<typename Iter, typename Item>
काष्ठा hb_is_sink_of
{
  निजी:
  ढाँचा <typename Iter2 = Iter,
	    hb_enable_if (hb_is_convertible (typename Iter2::item_t, hb_add_lvalue_reference<Item>))>
  अटल hb_true_type impl (hb_priority<2>);
  ढाँचा <typename Iter2 = Iter>
  अटल स्वतः impl (hb_priority<1>) -> decltype (hb_declval (Iter2) << hb_declval (Item), hb_true_type ());
  अटल hb_false_type impl (hb_priority<0>);

  खुला:
  अटल constexpr bool value = decltype (impl (hb_prioritize))::value;
};
#घोषणा hb_is_sink_of(Iter, Item) hb_is_sink_of<Iter, Item>::value

/* This is commonly used, so define: */
#घोषणा hb_is_sorted_source_of(Iter, Item) \
	(hb_is_source_of(Iter, Item) && Iter::is_sorted_iterator)


काष्ठा
{
  ढाँचा <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  अचिन्हित चालक () (स्थिर Iterable &_) स्थिर { वापस hb_len (hb_iter (_)); }

  अचिन्हित चालक () (अचिन्हित _) स्थिर { वापस _; }
}
HB_FUNCOBJ (hb_len_of);

/* Range-based 'for' for iterables. */

ढाँचा <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
अटल अंतरभूत स्वतः begin (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).begin ())

ढाँचा <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
अटल अंतरभूत स्वतः end (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).end ())

/* begin()/end() are NOT looked up non-ADL.  So each namespace must declare them.
 * Do it for namespace OT. */
namespace OT {

ढाँचा <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
अटल अंतरभूत स्वतः begin (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).begin ())

ढाँचा <typename Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
अटल अंतरभूत स्वतः end (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).end ())

}


/*
 * Adaptors, combiners, etc.
 */

ढाँचा <typename Lhs, typename Rhs,
	  hb_requires (hb_is_iterator (Lhs))>
अटल अंतरभूत स्वतः
चालक | (Lhs&& lhs, Rhs&& rhs) HB_AUTO_RETURN (std::forward<Rhs> (rhs) (std::forward<Lhs> (lhs)))

/* hb_map(), hb_filter(), hb_reduce() */

क्रमागत  श्रेणी hb_function_sortedness_t {
  NOT_SORTED,
  RETAINS_SORTING,
  SORTED,
};

ढाँचा <typename Iter, typename Proj, hb_function_sortedness_t Sorted,
	 hb_requires (hb_is_iterator (Iter))>
काष्ठा hb_map_iter_t :
  hb_iter_t<hb_map_iter_t<Iter, Proj, Sorted>,
	    decltype (hb_get (hb_declval (Proj), *hb_declval (Iter)))>
{
  hb_map_iter_t (स्थिर Iter& it, Proj f_) : it (it), f (f_) {}

  प्रकार decltype (hb_get (hb_declval (Proj), *hb_declval (Iter))) __item_t__;
  अटल constexpr bool is_random_access_iterator = Iter::is_random_access_iterator;
  अटल constexpr bool is_sorted_iterator =
    Sorted == hb_function_sortedness_t::SORTED ? true :
    Sorted == hb_function_sortedness_t::RETAINS_SORTING ? Iter::is_sorted_iterator :
    false;
  __item_t__ __item__ () स्थिर { वापस hb_get (f.लो (), *it); }
  __item_t__ __item_at__ (अचिन्हित i) स्थिर { वापस hb_get (f.लो (), it[i]); }
  bool __more__ () स्थिर { वापस bool (it); }
  अचिन्हित __len__ () स्थिर { वापस it.len (); }
  व्योम __next__ () { ++it; }
  व्योम __forward__ (अचिन्हित n) { it += n; }
  व्योम __prev__ () { --it; }
  व्योम __rewind__ (अचिन्हित n) { it -= n; }
  hb_map_iter_t __end__ () स्थिर { वापस hb_map_iter_t (it._end (), f); }
  bool चालक != (स्थिर hb_map_iter_t& o) स्थिर
  { वापस it != o.it; }

  निजी:
  Iter it;
  mutable hb_reference_wrapper<Proj> f;
};

ढाँचा <typename Proj, hb_function_sortedness_t Sorted>
काष्ठा hb_map_iter_factory_t
{
  hb_map_iter_factory_t (Proj f) : f (f) {}

  ढाँचा <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  hb_map_iter_t<Iter, Proj, Sorted>
  चालक () (Iter it)
  { वापस hb_map_iter_t<Iter, Proj, Sorted> (it, f); }

  निजी:
  Proj f;
};
काष्ठा
{
  ढाँचा <typename Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::NOT_SORTED>
  चालक () (Proj&& f) स्थिर
  { वापस hb_map_iter_factory_t<Proj, hb_function_sortedness_t::NOT_SORTED> (f); }
}
HB_FUNCOBJ (hb_map);
काष्ठा
{
  ढाँचा <typename Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::RETAINS_SORTING>
  चालक () (Proj&& f) स्थिर
  { वापस hb_map_iter_factory_t<Proj, hb_function_sortedness_t::RETAINS_SORTING> (f); }
}
HB_FUNCOBJ (hb_map_retains_sorting);
काष्ठा
{
  ढाँचा <typename Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::SORTED>
  चालक () (Proj&& f) स्थिर
  { वापस hb_map_iter_factory_t<Proj, hb_function_sortedness_t::SORTED> (f); }
}
HB_FUNCOBJ (hb_map_sorted);

ढाँचा <typename Iter, typename Pred, typename Proj,
	 hb_requires (hb_is_iterator (Iter))>
काष्ठा hb_filter_iter_t :
  hb_iter_with_fallback_t<hb_filter_iter_t<Iter, Pred, Proj>,
			  typename Iter::item_t>
{
  hb_filter_iter_t (स्थिर Iter& it_, Pred p_, Proj f_) : it (it_), p (p_), f (f_)
  { जबतक (it && !hb_has (p.लो (), hb_get (f.लो (), *it))) ++it; }

  प्रकार typename Iter::item_t __item_t__;
  अटल constexpr bool is_sorted_iterator = Iter::is_sorted_iterator;
  __item_t__ __item__ () स्थिर { वापस *it; }
  bool __more__ () स्थिर { वापस bool (it); }
  व्योम __next__ () { करो ++it; जबतक (it && !hb_has (p.लो (), hb_get (f.लो (), *it))); }
  व्योम __prev__ ()
  {
    अचिन्हित गिनती = 0;
    करो
    {
      Iter old = it;
      --it;
      अगर (!(it != old))
      {
	it += गिनती;
	वापस;
      }
      गिनती++;
    }
    जबतक (!hb_has (p.लो (), hb_get (f.लो (), *it)));
  }
  hb_filter_iter_t __end__ () स्थिर { वापस hb_filter_iter_t (it._end (), p, f); }
  bool चालक != (स्थिर hb_filter_iter_t& o) स्थिर
  { वापस it != o.it; }

  निजी:
  Iter it;
  mutable hb_reference_wrapper<Pred> p;
  mutable hb_reference_wrapper<Proj> f;
};
ढाँचा <typename Pred, typename Proj>
काष्ठा hb_filter_iter_factory_t
{
  hb_filter_iter_factory_t (Pred p, Proj f) : p (p), f (f) {}

  ढाँचा <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  hb_filter_iter_t<Iter, Pred, Proj>
  चालक () (Iter it)
  { वापस hb_filter_iter_t<Iter, Pred, Proj> (it, p, f); }

  निजी:
  Pred p;
  Proj f;
};
काष्ठा
{
  ढाँचा <typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity))>
  hb_filter_iter_factory_t<Pred, Proj>
  चालक () (Pred&& p = hb_identity, Proj&& f = hb_identity) स्थिर
  { वापस hb_filter_iter_factory_t<Pred, Proj> (p, f); }
}
HB_FUNCOBJ (hb_filter);

ढाँचा <typename Redu, typename InitT>
काष्ठा hb_reduce_t
{
  hb_reduce_t (Redu r, InitT init_मूल्य) : r (r), init_मूल्य (init_मूल्य) {}

  ढाँचा <typename Iter,
	    hb_requires (hb_is_iterator (Iter)),
	    typename AccuT = hb_decay<decltype (hb_declval (Redu) (hb_declval (InitT), hb_declval (typename Iter::item_t)))>>
  AccuT
  चालक () (Iter it)
  {
    AccuT value = init_मूल्य;
    क्रम (; it; ++it)
      value = r (value, *it);
    वापस value;
  }

  निजी:
  Redu r;
  InitT init_मूल्य;
};
काष्ठा
{
  ढाँचा <typename Redu, typename InitT>
  hb_reduce_t<Redu, InitT>
  चालक () (Redu&& r, InitT init_मूल्य) स्थिर
  { वापस hb_reduce_t<Redu, InitT> (r, init_मूल्य); }
}
HB_FUNCOBJ (hb_reduce);


/* hb_zip() */

ढाँचा <typename A, typename B>
काष्ठा hb_zip_iter_t :
  hb_iter_t<hb_zip_iter_t<A, B>,
	    hb_pair_t<typename A::item_t, typename B::item_t>>
{
  hb_zip_iter_t () {}
  hb_zip_iter_t (स्थिर A& a, स्थिर B& b) : a (a), b (b) {}

  प्रकार hb_pair_t<typename A::item_t, typename B::item_t> __item_t__;
  अटल constexpr bool is_random_access_iterator =
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
  अटल constexpr bool is_sorted_iterator = A::is_sorted_iterator;

  __item_t__ __item__ () स्थिर { वापस __item_t__ (*a, *b); }
  __item_t__ __item_at__ (अचिन्हित i) स्थिर { वापस __item_t__ (a[i], b[i]); }
  bool __more__ () स्थिर { वापस bool (a) && bool (b); }
  अचिन्हित __len__ () स्थिर { वापस hb_min (a.len (), b.len ()); }
  व्योम __next__ () { ++a; ++b; }
  व्योम __forward__ (अचिन्हित n) { a += n; b += n; }
  व्योम __prev__ () { --a; --b; }
  व्योम __rewind__ (अचिन्हित n) { a -= n; b -= n; }
  hb_zip_iter_t __end__ () स्थिर { वापस hb_zip_iter_t (a._end (), b._end ()); }
  /* Note, we should stop if ANY of the iters reaches end.  As such two compare
   * unequal if both items are unequal, NOT if either is unequal. */
  bool चालक != (स्थिर hb_zip_iter_t& o) स्थिर
  { वापस a != o.a && b != o.b; }

  निजी:
  A a;
  B b;
};
काष्ठा
{ HB_PARTIALIZE(2);
  ढाँचा <typename A, typename B,
	    hb_requires (hb_is_iterable (A) && hb_is_iterable (B))>
  hb_zip_iter_t<hb_iter_type<A>, hb_iter_type<B>>
  चालक () (A&& a, B&& b) स्थिर
  { वापस hb_zip_iter_t<hb_iter_type<A>, hb_iter_type<B>> (hb_iter (a), hb_iter (b)); }
}
HB_FUNCOBJ (hb_zip);

/* hb_concat() */

ढाँचा <typename A, typename B>
काष्ठा hb_concat_iter_t :
    hb_iter_t<hb_concat_iter_t<A, B>, typename A::item_t>
{
  hb_concat_iter_t () {}
  hb_concat_iter_t (A& a, B& b) : a (a), b (b) {}
  hb_concat_iter_t (स्थिर A& a, स्थिर B& b) : a (a), b (b) {}


  प्रकार typename A::item_t __item_t__;
  अटल constexpr bool is_random_access_iterator =
    A::is_random_access_iterator &&
    B::is_random_access_iterator;
  अटल constexpr bool is_sorted_iterator = false;

  __item_t__ __item__ () स्थिर
  {
    अगर (!a)
      वापस *b;
    वापस *a;
  }

  __item_t__ __item_at__ (अचिन्हित i) स्थिर
  {
    अचिन्हित a_len = a.len ();
    अगर (i < a_len)
      वापस a[i];
    वापस b[i - a_len];
  }

  bool __more__ () स्थिर { वापस bool (a) || bool (b); }

  अचिन्हित __len__ () स्थिर { वापस a.len () + b.len (); }

  व्योम __next__ ()
  {
    अगर (a)
      ++a;
    अन्यथा
      ++b;
  }

  व्योम __forward__ (अचिन्हित n)
  {
    अगर (!n) वापस;
    अगर (!is_random_access_iterator) {
      जबतक (n-- && *यह) {
        (*यह)++;
      }
      वापस;
    }

    अचिन्हित a_len = a.len ();
    अगर (n > a_len) {
      n -= a_len;
      a.__forward__ (a_len);
      b.__forward__ (n);
    } अन्यथा {
      a.__forward__ (n);
    }
  }

  hb_concat_iter_t __end__ () स्थिर { वापस hb_concat_iter_t (a._end (), b._end ()); }
  bool चालक != (स्थिर hb_concat_iter_t& o) स्थिर
  {
    वापस a != o.a
        || b != o.b;
  }

  निजी:
  A a;
  B b;
};
काष्ठा
{ HB_PARTIALIZE(2);
  ढाँचा <typename A, typename B,
	    hb_requires (hb_is_iterable (A) && hb_is_iterable (B))>
  hb_concat_iter_t<hb_iter_type<A>, hb_iter_type<B>>
  चालक () (A&& a, B&& b) स्थिर
  { वापस hb_concat_iter_t<hb_iter_type<A>, hb_iter_type<B>> (hb_iter (a), hb_iter (b)); }
}
HB_FUNCOBJ (hb_concat);

/* hb_apply() */

ढाँचा <typename Appl>
काष्ठा hb_apply_t
{
  hb_apply_t (Appl a) : a (a) {}

  ढाँचा <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  व्योम चालक () (Iter it)
  {
    क्रम (; it; ++it)
      (व्योम) hb_invoke (a, *it);
  }

  निजी:
  Appl a;
};
काष्ठा
{
  ढाँचा <typename Appl> hb_apply_t<Appl>
  चालक () (Appl&& a) स्थिर
  { वापस hb_apply_t<Appl> (a); }

  ढाँचा <typename Appl> hb_apply_t<Appl&>
  चालक () (Appl *a) स्थिर
  { वापस hb_apply_t<Appl&> (*a); }
}
HB_FUNCOBJ (hb_apply);

/* hb_range()/hb_iota()/hb_repeat() */

ढाँचा <typename T, typename S>
काष्ठा hb_range_iter_t :
  hb_iter_t<hb_range_iter_t<T, S>, T>
{
  hb_range_iter_t (T start, T end_, S step) : v (start), end_ (end_for (start, end_, step)), step (step) {}

  प्रकार T __item_t__;
  अटल constexpr bool is_random_access_iterator = true;
  अटल constexpr bool is_sorted_iterator = true;
  __item_t__ __item__ () स्थिर { वापस hb_ridentity (v); }
  __item_t__ __item_at__ (अचिन्हित j) स्थिर { वापस v + j * step; }
  bool __more__ () स्थिर { वापस v != end_; }
  अचिन्हित __len__ () स्थिर { वापस !step ? अच_पूर्णांक_उच्च : (end_ - v) / step; }
  व्योम __next__ () { v += step; }
  व्योम __forward__ (अचिन्हित n) { v += n * step; }
  व्योम __prev__ () { v -= step; }
  व्योम __rewind__ (अचिन्हित n) { v -= n * step; }
  hb_range_iter_t __end__ () स्थिर { वापस hb_range_iter_t (end_, end_, step); }
  bool चालक != (स्थिर hb_range_iter_t& o) स्थिर
  { वापस v != o.v; }

  निजी:
  अटल अंतरभूत T end_for (T start, T end_, S step)
  {
    अगर (!step)
      वापस end_;
    स्वतः res = (end_ - start) % step;
    अगर (!res)
      वापस end_;
    end_ += step - res;
    वापस end_;
  }

  निजी:
  T v;
  T end_;
  S step;
};
काष्ठा
{
  ढाँचा <typename T = अचिन्हित> hb_range_iter_t<T, अचिन्हित>
  चालक () (T end = (अचिन्हित) -1) स्थिर
  { वापस hb_range_iter_t<T, अचिन्हित> (0, end, 1u); }

  ढाँचा <typename T, typename S = अचिन्हित> hb_range_iter_t<T, S>
  चालक () (T start, T end, S step = 1u) स्थिर
  { वापस hb_range_iter_t<T, S> (start, end, step); }
}
HB_FUNCOBJ (hb_range);

ढाँचा <typename T, typename S>
काष्ठा hb_iota_iter_t :
  hb_iter_with_fallback_t<hb_iota_iter_t<T, S>, T>
{
  hb_iota_iter_t (T start, S step) : v (start), step (step) {}

  निजी:

  ढाँचा <typename S2 = S>
  स्वतः
  inc (hb_type_identity<S2> s, hb_priority<1>)
    -> hb_void_t<decltype (hb_invoke (std::forward<hb_type_identity<S2>> (s),
                                      hb_declval<T&> ()))>
  { v = hb_invoke (std::forward<hb_type_identity<S2>> (s), v); }

  व्योम
  inc (S s, hb_priority<0>)
  { v += s; }

  खुला:

  प्रकार T __item_t__;
  अटल constexpr bool is_random_access_iterator = true;
  अटल constexpr bool is_sorted_iterator = true;
  __item_t__ __item__ () स्थिर { वापस hb_ridentity (v); }
  bool __more__ () स्थिर { वापस true; }
  अचिन्हित __len__ () स्थिर { वापस अच_पूर्णांक_उच्च; }
  व्योम __next__ () { inc (step, hb_prioritize); }
  व्योम __prev__ () { v -= step; }
  hb_iota_iter_t __end__ () स्थिर { वापस *यह; }
  bool चालक != (स्थिर hb_iota_iter_t& o) स्थिर { वापस true; }

  निजी:
  T v;
  S step;
};
काष्ठा
{
  ढाँचा <typename T = अचिन्हित, typename S = अचिन्हित> hb_iota_iter_t<T, S>
  चालक () (T start = 0u, S step = 1u) स्थिर
  { वापस hb_iota_iter_t<T, S> (start, step); }
}
HB_FUNCOBJ (hb_iota);

ढाँचा <typename T>
काष्ठा hb_repeat_iter_t :
  hb_iter_t<hb_repeat_iter_t<T>, T>
{
  hb_repeat_iter_t (T value) : v (value) {}

  प्रकार T __item_t__;
  अटल constexpr bool is_random_access_iterator = true;
  अटल constexpr bool is_sorted_iterator = true;
  __item_t__ __item__ () स्थिर { वापस v; }
  __item_t__ __item_at__ (अचिन्हित j) स्थिर { वापस v; }
  bool __more__ () स्थिर { वापस true; }
  अचिन्हित __len__ () स्थिर { वापस अच_पूर्णांक_उच्च; }
  व्योम __next__ () {}
  व्योम __forward__ (अचिन्हित) {}
  व्योम __prev__ () {}
  व्योम __rewind__ (अचिन्हित) {}
  hb_repeat_iter_t __end__ () स्थिर { वापस *यह; }
  bool चालक != (स्थिर hb_repeat_iter_t& o) स्थिर { वापस true; }

  निजी:
  T v;
};
काष्ठा
{
  ढाँचा <typename T> hb_repeat_iter_t<T>
  चालक () (T value) स्थिर
  { वापस hb_repeat_iter_t<T> (value); }
}
HB_FUNCOBJ (hb_repeat);

/* hb_enumerate()/hb_take() */

काष्ठा
{
  ढाँचा <typename Iterable,
	    typename सूचकांक = अचिन्हित,
	    hb_requires (hb_is_iterable (Iterable))>
  स्वतः चालक () (Iterable&& it, सूचकांक start = 0u) स्थिर HB_AUTO_RETURN
  ( hb_zip (hb_iota (start), it) )
}
HB_FUNCOBJ (hb_enumerate);

काष्ठा
{ HB_PARTIALIZE(2);
  ढाँचा <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  स्वतः चालक () (Iterable&& it, अचिन्हित गिनती) स्थिर HB_AUTO_RETURN
  ( hb_zip (hb_range (गिनती), it) | hb_map_retains_sorting (hb_second) )

  /* Specialization arrays. */

  ढाँचा <typename Type> अंतरभूत hb_array_t<Type>
  चालक () (hb_array_t<Type> array, अचिन्हित गिनती) स्थिर
  { वापस array.sub_array (0, गिनती); }

  ढाँचा <typename Type> अंतरभूत hb_sorted_array_t<Type>
  चालक () (hb_sorted_array_t<Type> array, अचिन्हित गिनती) स्थिर
  { वापस array.sub_array (0, गिनती); }
}
HB_FUNCOBJ (hb_take);

काष्ठा
{ HB_PARTIALIZE(2);
  ढाँचा <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  स्वतः चालक () (Iter it, अचिन्हित गिनती) स्थिर HB_AUTO_RETURN
  (
    + hb_iota (it, hb_add (गिनती))
    | hb_map (hb_take (गिनती))
    | hb_take ((hb_len (it) + गिनती - 1) / गिनती)
  )
}
HB_FUNCOBJ (hb_chop);

/* hb_sink() */

ढाँचा <typename Sink>
काष्ठा hb_sink_t
{
  hb_sink_t (Sink s) : s (s) {}

  ढाँचा <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  व्योम चालक () (Iter it)
  {
    क्रम (; it; ++it)
      s << *it;
  }

  निजी:
  Sink s;
};
काष्ठा
{
  ढाँचा <typename Sink> hb_sink_t<Sink>
  चालक () (Sink&& s) स्थिर
  { वापस hb_sink_t<Sink> (s); }

  ढाँचा <typename Sink> hb_sink_t<Sink&>
  चालक () (Sink *s) स्थिर
  { वापस hb_sink_t<Sink&> (*s); }
}
HB_FUNCOBJ (hb_sink);

/* hb-drain: hb_sink to void / blackhole / /dev/null. */

काष्ठा
{
  ढाँचा <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  व्योम चालक () (Iter it) स्थिर
  {
    क्रम (; it; ++it)
      (व्योम) *it;
  }
}
HB_FUNCOBJ (hb_drain);

/* hb_unzip(): unzip and sink to two sinks. */

ढाँचा <typename Sink1, typename Sink2>
काष्ठा hb_unzip_t
{
  hb_unzip_t (Sink1 s1, Sink2 s2) : s1 (s1), s2 (s2) {}

  ढाँचा <typename Iter,
	    hb_requires (hb_is_iterator (Iter))>
  व्योम चालक () (Iter it)
  {
    क्रम (; it; ++it)
    {
      स्थिर स्वतः &v = *it;
      s1 << v.first;
      s2 << v.second;
    }
  }

  निजी:
  Sink1 s1;
  Sink2 s2;
};
काष्ठा
{
  ढाँचा <typename Sink1, typename Sink2> hb_unzip_t<Sink1, Sink2>
  चालक () (Sink1&& s1, Sink2&& s2) स्थिर
  { वापस hb_unzip_t<Sink1, Sink2> (s1, s2); }

  ढाँचा <typename Sink1, typename Sink2> hb_unzip_t<Sink1&, Sink2&>
  चालक () (Sink1 *s1, Sink2 *s2) स्थिर
  { वापस hb_unzip_t<Sink1&, Sink2&> (*s1, *s2); }
}
HB_FUNCOBJ (hb_unzip);


/* hb-all, hb-any, hb-none. */

काष्ठा
{
  ढाँचा <typename Iterable,
	    typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity)),
	    hb_requires (hb_is_iterable (Iterable))>
  bool चालक () (Iterable&& c,
		    Pred&& p = hb_identity,
		    Proj&& f = hb_identity) स्थिर
  {
    क्रम (स्वतः it = hb_iter (c); it; ++it)
      अगर (!hb_match (p, hb_get (f, *it)))
	वापस false;
    वापस true;
  }
}
HB_FUNCOBJ (hb_all);
काष्ठा
{
  ढाँचा <typename Iterable,
	    typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity)),
	    hb_requires (hb_is_iterable (Iterable))>
  bool चालक () (Iterable&& c,
		    Pred&& p = hb_identity,
		    Proj&& f = hb_identity) स्थिर
  {
    क्रम (स्वतः it = hb_iter (c); it; ++it)
      अगर (hb_match (p, hb_get (f, *it)))
	वापस true;
    वापस false;
  }
}
HB_FUNCOBJ (hb_any);
काष्ठा
{
  ढाँचा <typename Iterable,
	    typename Pred = decltype ((hb_identity)),
	    typename Proj = decltype ((hb_identity)),
	    hb_requires (hb_is_iterable (Iterable))>
  bool चालक () (Iterable&& c,
		    Pred&& p = hb_identity,
		    Proj&& f = hb_identity) स्थिर
  {
    क्रम (स्वतः it = hb_iter (c); it; ++it)
      अगर (hb_match (p, hb_get (f, *it)))
	वापस false;
    वापस true;
  }
}
HB_FUNCOBJ (hb_none);

/*
 * Algorithms operating on iterators.
 */

ढाँचा <typename C, typename V,
	  hb_requires (hb_is_iterable (C))>
अंतरभूत व्योम
hb_fill (C&& c, स्थिर V &v)
{
  क्रम (स्वतः i = hb_iter (c); i; i++)
    *i = v;
}

ढाँचा <typename S, typename D>
अंतरभूत व्योम
hb_copy (S&& is, D&& id)
{
  hb_iter (is) | hb_sink (id);
}


#पूर्ण_अगर /* HB_ITER_HH */

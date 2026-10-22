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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ITER_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ITER_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-algs.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-meta.hh"


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
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ iter_t, ਕਿਸਮ_ਨਾਂ ਵਸਤੂ_3 = ਕਿਸਮ_ਨਾਂ iter_t::__item_t__>
ਬਣਤਰ hb_iter_t
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਵਸਤੂ_3 ਵਸਤੂ_t;
  constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ get_ਵਸਤੂ_ਮਾਪ () ਅਚਲ { ਵਾਪਸ hb_static_ਮਾਪ (ਵਸਤੂ_3); }
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ is_iterator = ਸੱਚ;
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ is_random_access_iterator = ਝੂਠ;
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ is_sorted_iterator = ਝੂਠ;
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ has_fast_len = ਝੂਠ; // Should be checked in combination with is_random_access_iterator.

  ਨਿਜੀ:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ਅਚਲ iter_t* thiz () ਅਚਲ { ਵਾਪਸ static_cast<ਅਚਲ iter_t *> (ਇਹ); }
	iter_t* thiz ()       { ਵਾਪਸ static_cast<      iter_t *> (ਇਹ); }
  ਜਨਤਕ:

  /* Operators. */
  iter_t iter () ਅਚਲ { ਵਾਪਸ *thiz(); }
  iter_t ਸੰਚਾਲਕ + () ਅਚਲ { ਵਾਪਸ *thiz(); }
  iter_t _begin () ਅਚਲ { ਵਾਪਸ *thiz(); }
  iter_t begin () ਅਚਲ { ਵਾਪਸ _begin (); }
  iter_t ਸਿਰਾ_3 () ਅਚਲ { ਵਾਪਸ thiz()->__end__ (); }
  iter_t ਸਿਰਾ () ਅਚਲ { ਵਾਪਸ ਸਿਰਾ_3 (); }
  ਸਪਸ਼ਟ ਸੰਚਾਲਕ ਤਾਰਕਿਕ () ਅਚਲ { ਵਾਪਸ thiz()->__more__ (); }
  ਬਿਨਾਨਿਸ਼ਾਨੀ len () ਅਚਲ { ਵਾਪਸ thiz()->__len__ (); }
  /* The following can only be enabled if item_t is reference type.  Otherwise
   * it will be returning pointer to temporary rvalue. */
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T = ਵਸਤੂ_t,
	    hb_enable_if (std::is_reference<T>::value)>
  hb_remove_reference<ਵਸਤੂ_t>* ਸੰਚਾਲਕ -> () ਅਚਲ { ਵਾਪਸ std::addressof (**thiz()); }
  ਵਸਤੂ_t ਸੰਚਾਲਕ * () ਅਚਲ { ਵਾਪਸ thiz()->__item__ (); }
  ਵਸਤੂ_t ਸੰਚਾਲਕ * () { ਵਾਪਸ thiz()->__item__ (); }
  ਵਸਤੂ_t ਸੰਚਾਲਕ [] (ਬਿਨਾਨਿਸ਼ਾਨੀ i) ਅਚਲ { ਵਾਪਸ thiz()->__item_at__ (i); }
  ਵਸਤੂ_t ਸੰਚਾਲਕ [] (ਬਿਨਾਨਿਸ਼ਾਨੀ i) { ਵਾਪਸ thiz()->__item_at__ (i); }
  iter_t& ਸੰਚਾਲਕ += (ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2) &  { thiz()->__forward__ (ਗਿਣਤੀ_2); ਵਾਪਸ *thiz(); }
  iter_t  ਸੰਚਾਲਕ += (ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2) && { thiz()->__forward__ (ਗਿਣਤੀ_2); ਵਾਪਸ *thiz(); }
  iter_t& ਸੰਚਾਲਕ ++ () &  { thiz()->__next__ (); ਵਾਪਸ *thiz(); }
  iter_t  ਸੰਚਾਲਕ ++ () && { thiz()->__next__ (); ਵਾਪਸ *thiz(); }
  iter_t& ਸੰਚਾਲਕ -= (ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2) &  { thiz()->__rewind__ (ਗਿਣਤੀ_2); ਵਾਪਸ *thiz(); }
  iter_t  ਸੰਚਾਲਕ -= (ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2) && { thiz()->__rewind__ (ਗਿਣਤੀ_2); ਵਾਪਸ *thiz(); }
  iter_t& ਸੰਚਾਲਕ -- () &  { thiz()->__prev__ (); ਵਾਪਸ *thiz(); }
  iter_t  ਸੰਚਾਲਕ -- () && { thiz()->__prev__ (); ਵਾਪਸ *thiz(); }
  iter_t ਸੰਚਾਲਕ + (ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2) ਅਚਲ { ਸਵੈਚਾਲਿਤ c = thiz()->iter (); c += ਗਿਣਤੀ_2; ਵਾਪਸ c; }
  ਮਿੱਤਰ iter_t ਸੰਚਾਲਕ + (ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2, ਅਚਲ iter_t &it) { ਵਾਪਸ it + ਗਿਣਤੀ_2; }
  iter_t ਸੰਚਾਲਕ ++ (ਪੂਰਨ) { iter_t c (*thiz()); ++*thiz(); ਵਾਪਸ c; }
  iter_t ਸੰਚਾਲਕ - (ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2) ਅਚਲ { ਸਵੈਚਾਲਿਤ c = thiz()->iter (); c -= ਗਿਣਤੀ_2; ਵਾਪਸ c; }
  iter_t ਸੰਚਾਲਕ -- (ਪੂਰਨ) { iter_t c (*thiz()); --*thiz(); ਵਾਪਸ c; }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  iter_t& ਸੰਚਾਲਕ >> (T &v) &  { v = **thiz(); ++*thiz(); ਵਾਪਸ *thiz(); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  iter_t  ਸੰਚਾਲਕ >> (T &v) && { v = **thiz(); ++*thiz(); ਵਾਪਸ *thiz(); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  iter_t& ਸੰਚਾਲਕ << (ਅਚਲ T v) &  { **thiz() = v; ++*thiz(); ਵਾਪਸ *thiz(); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  iter_t  ਸੰਚਾਲਕ << (ਅਚਲ T v) && { **thiz() = v; ++*thiz(); ਵਾਪਸ *thiz(); }

  ਸੁਰੱਖਿਅਤ:
  hb_iter_t () = ਮੂਲ;
  hb_iter_t (ਅਚਲ hb_iter_t &o HB_ਅਣਵਰਤਿਆ) = ਮੂਲ;
  hb_iter_t (hb_iter_t &&o HB_ਅਣਵਰਤਿਆ) = ਮੂਲ;
  hb_iter_t& ਸੰਚਾਲਕ = (ਅਚਲ hb_iter_t &o HB_ਅਣਵਰਤਿਆ) = ਮੂਲ;
  hb_iter_t& ਸੰਚਾਲਕ = (hb_iter_t &&o HB_ਅਣਵਰਤਿਆ) = ਮੂਲ;
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ITER_USING(Name) \
  ਵਰਤੋਂ ਵਸਤੂ_t = ਕਿਸਮ_ਨਾਂ Name::ਵਸਤੂ_t; \
  ਵਰਤੋਂ Name::_begin; \
  ਵਰਤੋਂ Name::begin; \
  ਵਰਤੋਂ Name::ਸਿਰਾ_3; \
  ਵਰਤੋਂ Name::ਸਿਰਾ; \
  ਵਰਤੋਂ Name::get_ਵਸਤੂ_ਮਾਪ; \
  ਵਰਤੋਂ Name::is_iterator; \
  ਵਰਤੋਂ Name::iter; \
  ਵਰਤੋਂ Name::ਸੰਚਾਲਕ ਤਾਰਕਿਕ; \
  ਵਰਤੋਂ Name::len; \
  ਵਰਤੋਂ Name::ਸੰਚਾਲਕ ->; \
  ਵਰਤੋਂ Name::ਸੰਚਾਲਕ *; \
  ਵਰਤੋਂ Name::ਸੰਚਾਲਕ []; \
  ਵਰਤੋਂ Name::ਸੰਚਾਲਕ +=; \
  ਵਰਤੋਂ Name::ਸੰਚਾਲਕ ++; \
  ਵਰਤੋਂ Name::ਸੰਚਾਲਕ -=; \
  ਵਰਤੋਂ Name::ਸੰਚਾਲਕ --; \
  ਵਰਤੋਂ Name::ਸੰਚਾਲਕ +; \
  ਵਰਤੋਂ Name::ਸੰਚਾਲਕ -; \
  ਵਰਤੋਂ Name::ਸੰਚਾਲਕ >>; \
  ਵਰਤੋਂ Name::ਸੰਚਾਲਕ <<; \
  static_assert (ਸੱਚ, "")

/* Returns iterator / item type of a type. */
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterable>
ਵਰਤੋਂ hb_iter_type = decltype (hb_deref (hb_declval (Iterable)).iter ());
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterable>
ਵਰਤੋਂ hb_ਵਸਤੂ_type = decltype (*hb_deref (hb_declval (Iterable)).iter ());


ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ> ਬਣਤਰ hb_array_t;
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ> ਬਣਤਰ hb_sorted_array_t;

ਬਣਤਰ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> hb_iter_type<T>
  ਸੰਚਾਲਕ () (T&& c) ਅਚਲ
  { ਵਾਪਸ hb_deref (std::forward<T> (c)).iter (); }

  /* Specialization for C arrays. */

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type> ਸਤਰਅੰਦਰ hb_array_t<Type>
  ਸੰਚਾਲਕ () (Type *array, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਲੰਮਾਈ) ਅਚਲ
  { ਵਾਪਸ hb_array_t<Type> (array, ਲੰਮਾਈ); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਲੰਮਾਈ> hb_array_t<Type>
  ਸੰਚਾਲਕ () (Type (&array)[ਲੰਮਾਈ]) ਅਚਲ
  { ਵਾਪਸ hb_array_t<Type> (array, ਲੰਮਾਈ); }

}
HB_FUNCOBJ (hb_iter);
ਬਣਤਰ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਸਵੈਚਾਲਿਤ
  impl (T&& c, hb_ਤਰਜੀਹ<1>) ਅਚਲ HB_RETURN (ਬਿਨਾਨਿਸ਼ਾਨੀ, c.len ())

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਸਵੈਚਾਲਿਤ
  impl (T&& c, hb_ਤਰਜੀਹ<0>) ਅਚਲ HB_RETURN (ਬਿਨਾਨਿਸ਼ਾਨੀ, c.len)

  ਜਨਤਕ:

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਸਵੈਚਾਲਿਤ
  ਸੰਚਾਲਕ () (T&& c) ਅਚਲ HB_RETURN (ਬਿਨਾਨਿਸ਼ਾਨੀ, impl (std::forward<T> (c), hb_prioritize))
}
HB_FUNCOBJ (hb_len);

/* Mixin to fill in what the subclass doesn't provide. */
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ iter_t, ਕਿਸਮ_ਨਾਂ ਵਸਤੂ_t = ਕਿਸਮ_ਨਾਂ iter_t::__item_t__>
ਬਣਤਰ hb_iter_fallback_mixin_t
{
  ਨਿਜੀ:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ਅਚਲ iter_t* thiz () ਅਚਲ { ਵਾਪਸ static_cast<ਅਚਲ iter_t *> (ਇਹ); }
	iter_t* thiz ()       { ਵਾਪਸ static_cast<      iter_t *> (ਇਹ); }
  ਜਨਤਕ:

  /* Access: Implement __item__(), or __item_at__() if random-access. */
  ਵਸਤੂ_t __item__ () ਅਚਲ { ਵਾਪਸ (*thiz())[0]; }
  ਵਸਤੂ_t __item_at__ (ਬਿਨਾਨਿਸ਼ਾਨੀ i) ਅਚਲ { ਵਾਪਸ *(*thiz() + i); }

  /* Termination: Implement __more__(), or __len__() if random-access. */
  ਤਾਰਕਿਕ __more__ () ਅਚਲ { ਵਾਪਸ ਤਾਰਕਿਕ (thiz()->len ()); }
  ਬਿਨਾਨਿਸ਼ਾਨੀ __len__ () ਅਚਲ
  { iter_t c (*thiz()); ਬਿਨਾਨਿਸ਼ਾਨੀ l = 0; ਜਦੋਂ (c) { c++; l++; } ਵਾਪਸ l; }

  /* Advancing: Implement __next__(), or __forward__() if random-access. */
  ਖਾਲੀ __next__ () { *thiz() += 1; }
  ਖਾਲੀ __forward__ (ਬਿਨਾਨਿਸ਼ਾਨੀ n) { ਜਦੋਂ (*thiz() && n--) ++*thiz(); }

  /* Rewinding: Implement __prev__() or __rewind__() if bidirectional. */
  ਖਾਲੀ __prev__ () { *thiz() -= 1; }
  ਖਾਲੀ __rewind__ (ਬਿਨਾਨਿਸ਼ਾਨੀ n) { ਜਦੋਂ (*thiz() && n--) --*thiz(); }

  /* Range-based for: Implement __end__() if can be done faster,
   * and operator!=. */
  iter_t __end__ () ਅਚਲ
  {
    ਜੇ (thiz()->is_random_access_iterator)
      ਵਾਪਸ *thiz() + thiz()->len ();
    /* Above expression loops twice. Following loops once. */
    ਸਵੈਚਾਲਿਤ it = *thiz();
    ਜਦੋਂ (it) ++it;
    ਵਾਪਸ it;
  }

  ਸੁਰੱਖਿਅਤ:
  hb_iter_fallback_mixin_t () = ਮੂਲ;
  hb_iter_fallback_mixin_t (ਅਚਲ hb_iter_fallback_mixin_t &o HB_ਅਣਵਰਤਿਆ) = ਮੂਲ;
  hb_iter_fallback_mixin_t (hb_iter_fallback_mixin_t &&o HB_ਅਣਵਰਤਿਆ) = ਮੂਲ;
  hb_iter_fallback_mixin_t& ਸੰਚਾਲਕ = (ਅਚਲ hb_iter_fallback_mixin_t &o HB_ਅਣਵਰਤਿਆ) = ਮੂਲ;
  hb_iter_fallback_mixin_t& ਸੰਚਾਲਕ = (hb_iter_fallback_mixin_t &&o HB_ਅਣਵਰਤਿਆ) = ਮੂਲ;
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ iter_t, ਕਿਸਮ_ਨਾਂ ਵਸਤੂ_t = ਕਿਸਮ_ਨਾਂ iter_t::__item_t__>
ਬਣਤਰ hb_iter_with_fallback_t :
  hb_iter_t<iter_t, ਵਸਤੂ_t>,
  hb_iter_fallback_mixin_t<iter_t, ਵਸਤੂ_t>
{
  ਸੁਰੱਖਿਅਤ:
  hb_iter_with_fallback_t () = ਮੂਲ;
  hb_iter_with_fallback_t (ਅਚਲ hb_iter_with_fallback_t &o HB_ਅਣਵਰਤਿਆ) = ਮੂਲ;
  hb_iter_with_fallback_t (hb_iter_with_fallback_t &&o HB_ਅਣਵਰਤਿਆ) = ਮੂਲ;
  hb_iter_with_fallback_t& ਸੰਚਾਲਕ = (ਅਚਲ hb_iter_with_fallback_t &o HB_ਅਣਵਰਤਿਆ) = ਮੂਲ;
  hb_iter_with_fallback_t& ਸੰਚਾਲਕ = (hb_iter_with_fallback_t &&o HB_ਅਣਵਰਤਿਆ) = ਮੂਲ;
};

/*
 * Meta-programming predicates.
 */

/* hb_is_iterator() / hb_is_iterator_of() */

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iter, ਕਿਸਮ_ਨਾਂ ਵਸਤੂ_3>
ਬਣਤਰ hb_is_iterator_ਦਾ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Item2 = ਵਸਤੂ_3>
  ਸਥਿਰ hb_true_type impl (hb_ਤਰਜੀਹ<2>, hb_iter_t<Iter, hb_type_ਪਛਾਣ_ਤੱਤ<Item2>> *);
  ਸਥਿਰ hb_false_type impl (hb_ਤਰਜੀਹ<0>, ਅਚਲ ਖਾਲੀ *);

  ਜਨਤਕ:
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ value = decltype (impl (hb_prioritize, hb_declval (Iter*)))::value;
};
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_is_iterator_ਦਾ(Iter, ਵਸਤੂ_3) hb_is_iterator_ਦਾ<Iter, ਵਸਤੂ_3>::value
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_is_iterator(Iter) hb_is_iterator_ਦਾ (Iter, ਕਿਸਮ_ਨਾਂ Iter::ਵਸਤੂ_t)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_is_sorted_iterator_ਦਾ(Iter, ਵਸਤੂ_3) (hb_is_iterator_ਦਾ<Iter, ਵਸਤੂ_3>::value && Iter::is_sorted_iterator)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_is_sorted_iterator(Iter) hb_is_sorted_iterator_ਦਾ (Iter, ਕਿਸਮ_ਨਾਂ Iter::ਵਸਤੂ_t)

/* hb_is_iterable() */

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ hb_is_iterable
{
  ਨਿਜੀ:

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ U>
  ਸਥਿਰ ਸਵੈਚਾਲਿਤ impl (hb_ਤਰਜੀਹ<1>) -> decltype (hb_declval (U).iter (), hb_true_type ());

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ>
  ਸਥਿਰ hb_false_type impl (hb_ਤਰਜੀਹ<0>);

  ਜਨਤਕ:
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ value = decltype (impl<T> (hb_prioritize))::value;
};
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_is_iterable(Iterable) hb_is_iterable<Iterable>::value

/* hb_is_source_of() / hb_is_sink_of() */

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iter, ਕਿਸਮ_ਨਾਂ ਵਸਤੂ_3>
ਬਣਤਰ hb_is_ਸਰੋਤ_ਦਾ
{
  ਨਿਜੀ:
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iter2 = Iter,
	    hb_enable_if (hb_is_convertible (ਕਿਸਮ_ਨਾਂ Iter2::ਵਸਤੂ_t, hb_ਜੋੜੋ_lvalue_reference<ਅਚਲ ਵਸਤੂ_3>))>
  ਸਥਿਰ hb_true_type impl (hb_ਤਰਜੀਹ<2>);
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iter2 = Iter>
  ਸਥਿਰ ਸਵੈਚਾਲਿਤ impl (hb_ਤਰਜੀਹ<1>) -> decltype (hb_declval (Iter2) >> hb_declval (ਵਸਤੂ_3 &), hb_true_type ());
  ਸਥਿਰ hb_false_type impl (hb_ਤਰਜੀਹ<0>);

  ਜਨਤਕ:
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ value = decltype (impl (hb_prioritize))::value;
};
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_is_ਸਰੋਤ_ਦਾ(Iter, ਵਸਤੂ_3) hb_is_ਸਰੋਤ_ਦਾ<Iter, ਵਸਤੂ_3>::value

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iter, ਕਿਸਮ_ਨਾਂ ਵਸਤੂ_3>
ਬਣਤਰ hb_is_sink_ਦਾ
{
  ਨਿਜੀ:
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iter2 = Iter,
	    hb_enable_if (hb_is_convertible (ਕਿਸਮ_ਨਾਂ Iter2::ਵਸਤੂ_t, hb_ਜੋੜੋ_lvalue_reference<ਵਸਤੂ_3>))>
  ਸਥਿਰ hb_true_type impl (hb_ਤਰਜੀਹ<2>);
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iter2 = Iter>
  ਸਥਿਰ ਸਵੈਚਾਲਿਤ impl (hb_ਤਰਜੀਹ<1>) -> decltype (hb_declval (Iter2) << hb_declval (ਵਸਤੂ_3), hb_true_type ());
  ਸਥਿਰ hb_false_type impl (hb_ਤਰਜੀਹ<0>);

  ਜਨਤਕ:
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ value = decltype (impl (hb_prioritize))::value;
};
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_is_sink_ਦਾ(Iter, ਵਸਤੂ_3) hb_is_sink_ਦਾ<Iter, ਵਸਤੂ_3>::value

/* This is commonly used, so define: */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_is_sorted_ਸਰੋਤ_ਦਾ(Iter, ਵਸਤੂ_3) \
	(hb_is_ਸਰੋਤ_ਦਾ(Iter, ਵਸਤੂ_3) && Iter::is_sorted_iterator)


ਬਣਤਰ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੰਚਾਲਕ () (ਅਚਲ Iterable &_) ਅਚਲ { ਵਾਪਸ hb_len (hb_iter (_)); }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੰਚਾਲਕ () (ਬਿਨਾਨਿਸ਼ਾਨੀ _) ਅਚਲ { ਵਾਪਸ _; }
}
HB_FUNCOBJ (hb_len_ਦਾ);

/* Range-based 'for' for iterables. */

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਸਵੈਚਾਲਿਤ begin (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).begin ())

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਸਵੈਚਾਲਿਤ ਸਿਰਾ (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).ਸਿਰਾ ())

/* begin()/end() are NOT looked up non-ADL.  So each namespace must declare them.
 * Do it for namespace OT. */
ਨਾਮਾਕਾਸ਼ OT {

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਸਵੈਚਾਲਿਤ begin (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).begin ())

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਸਵੈਚਾਲਿਤ ਸਿਰਾ (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).ਸਿਰਾ ())

}


/*
 * Adaptors, combiners, etc.
 */

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Lhs, ਕਿਸਮ_ਨਾਂ Rhs,
	  hb_requires (hb_is_iterator (Lhs))>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਸਵੈਚਾਲਿਤ
ਸੰਚਾਲਕ | (Lhs&& lhs, Rhs&& rhs) HB_AUTO_RETURN (std::forward<Rhs> (rhs) (std::forward<Lhs> (lhs)))

/* hb_map(), hb_filter(), hb_reduce() */

ਗਿਣਤੀ  ਵਰਗ hb_function_sortedness_t {
  NOT_SORTED,
  RETAINS_ਛਾਂਟੀ,
  SORTED,
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iter, ਕਿਸਮ_ਨਾਂ Proj, hb_function_sortedness_t Sorted,
	 hb_requires (hb_is_iterator (Iter))>
ਬਣਤਰ hb_map_iter_t :
  hb_iter_t<hb_map_iter_t<Iter, Proj, Sorted>,
	    decltype (hb_get (hb_declval (Proj), *hb_declval (Iter)))>
{
  hb_map_iter_t (ਅਚਲ Iter& it, Proj f_) : it (it), f (f_) {}

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ decltype (hb_get (hb_declval (Proj), *hb_declval (Iter))) __item_t__;
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ is_random_access_iterator = Iter::is_random_access_iterator;
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ is_sorted_iterator =
    Sorted == hb_function_sortedness_t::SORTED ? ਸੱਚ :
    Sorted == hb_function_sortedness_t::RETAINS_ਛਾਂਟੀ ? Iter::is_sorted_iterator :
    ਝੂਠ;
  __item_t__ __item__ () ਅਚਲ { ਵਾਪਸ hb_get (f.get (), *it); }
  __item_t__ __item_at__ (ਬਿਨਾਨਿਸ਼ਾਨੀ i) ਅਚਲ { ਵਾਪਸ hb_get (f.get (), it[i]); }
  ਤਾਰਕਿਕ __more__ () ਅਚਲ { ਵਾਪਸ ਤਾਰਕਿਕ (it); }
  ਬਿਨਾਨਿਸ਼ਾਨੀ __len__ () ਅਚਲ { ਵਾਪਸ it.len (); }
  ਖਾਲੀ __next__ () { ++it; }
  ਖਾਲੀ __forward__ (ਬਿਨਾਨਿਸ਼ਾਨੀ n) { it += n; }
  ਖਾਲੀ __prev__ () { --it; }
  ਖਾਲੀ __rewind__ (ਬਿਨਾਨਿਸ਼ਾਨੀ n) { it -= n; }
  hb_map_iter_t __end__ () ਅਚਲ { ਵਾਪਸ hb_map_iter_t (it.ਸਿਰਾ_3 (), f); }
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ hb_map_iter_t& o) ਅਚਲ
  { ਵਾਪਸ it != o.it; }

  ਨਿਜੀ:
  Iter it;
  ਬਦਲਣਯੋਗ hb_reference_wrapper<Proj> f;
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Proj, hb_function_sortedness_t Sorted>
ਬਣਤਰ hb_map_iter_factory_t
{
  hb_map_iter_factory_t (Proj f) : f (f) {}

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iter,
	    hb_requires (hb_is_iterator (Iter))>
  hb_map_iter_t<Iter, Proj, Sorted>
  ਸੰਚਾਲਕ () (Iter it)
  { ਵਾਪਸ hb_map_iter_t<Iter, Proj, Sorted> (it, f); }

  ਨਿਜੀ:
  Proj f;
};
ਬਣਤਰ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::NOT_SORTED>
  ਸੰਚਾਲਕ () (Proj&& f) ਅਚਲ
  { ਵਾਪਸ hb_map_iter_factory_t<Proj, hb_function_sortedness_t::NOT_SORTED> (f); }
}
HB_FUNCOBJ (hb_map);
ਬਣਤਰ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::RETAINS_ਛਾਂਟੀ>
  ਸੰਚਾਲਕ () (Proj&& f) ਅਚਲ
  { ਵਾਪਸ hb_map_iter_factory_t<Proj, hb_function_sortedness_t::RETAINS_ਛਾਂਟੀ> (f); }
}
HB_FUNCOBJ (hb_map_retains_ਛਾਂਟੀ);
ਬਣਤਰ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::SORTED>
  ਸੰਚਾਲਕ () (Proj&& f) ਅਚਲ
  { ਵਾਪਸ hb_map_iter_factory_t<Proj, hb_function_sortedness_t::SORTED> (f); }
}
HB_FUNCOBJ (hb_map_sorted);

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iter, ਕਿਸਮ_ਨਾਂ Pred, ਕਿਸਮ_ਨਾਂ Proj,
	 hb_requires (hb_is_iterator (Iter))>
ਬਣਤਰ hb_filter_iter_t :
  hb_iter_with_fallback_t<hb_filter_iter_t<Iter, Pred, Proj>,
			  ਕਿਸਮ_ਨਾਂ Iter::ਵਸਤੂ_t>
{
  hb_filter_iter_t (ਅਚਲ Iter& it_, Pred p_, Proj f_) : it (it_), p (p_), f (f_)
  { ਜਦੋਂ (it && !hb_has (p.get (), hb_get (f.get (), *it))) ++it; }

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ Iter::ਵਸਤੂ_t __item_t__;
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ is_sorted_iterator = Iter::is_sorted_iterator;
  __item_t__ __item__ () ਅਚਲ { ਵਾਪਸ *it; }
  ਤਾਰਕਿਕ __more__ () ਅਚਲ { ਵਾਪਸ ਤਾਰਕਿਕ (it); }
  ਖਾਲੀ __next__ () { ਕਰੋ ++it; ਜਦੋਂ (it && !hb_has (p.get (), hb_get (f.get (), *it))); }
  ਖਾਲੀ __prev__ ()
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = 0;
    ਕਰੋ
    {
      Iter old = it;
      --it;
      ਜੇ (!(it != old))
      {
	it += ਗਿਣਤੀ_2;
	ਵਾਪਸ;
      }
      ਗਿਣਤੀ_2++;
    }
    ਜਦੋਂ (!hb_has (p.get (), hb_get (f.get (), *it)));
  }
  hb_filter_iter_t __end__ () ਅਚਲ { ਵਾਪਸ hb_filter_iter_t (it.ਸਿਰਾ_3 (), p, f); }
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ hb_filter_iter_t& o) ਅਚਲ
  { ਵਾਪਸ it != o.it; }

  ਨਿਜੀ:
  Iter it;
  ਬਦਲਣਯੋਗ hb_reference_wrapper<Pred> p;
  ਬਦਲਣਯੋਗ hb_reference_wrapper<Proj> f;
};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Pred, ਕਿਸਮ_ਨਾਂ Proj>
ਬਣਤਰ hb_filter_iter_factory_t
{
  hb_filter_iter_factory_t (Pred p, Proj f) : p (p), f (f) {}

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iter,
	    hb_requires (hb_is_iterator (Iter))>
  hb_filter_iter_t<Iter, Pred, Proj>
  ਸੰਚਾਲਕ () (Iter it)
  { ਵਾਪਸ hb_filter_iter_t<Iter, Pred, Proj> (it, p, f); }

  ਨਿਜੀ:
  Pred p;
  Proj f;
};
ਬਣਤਰ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Pred = decltype ((hb_ਪਛਾਣ_ਤੱਤ)),
	    ਕਿਸਮ_ਨਾਂ Proj = decltype ((hb_ਪਛਾਣ_ਤੱਤ))>
  hb_filter_iter_factory_t<Pred, Proj>
  ਸੰਚਾਲਕ () (Pred&& p = hb_ਪਛਾਣ_ਤੱਤ, Proj&& f = hb_ਪਛਾਣ_ਤੱਤ) ਅਚਲ
  { ਵਾਪਸ hb_filter_iter_factory_t<Pred, Proj> (p, f); }
}
HB_FUNCOBJ (hb_filter);

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Redu, ਕਿਸਮ_ਨਾਂ InitT>
ਬਣਤਰ hb_reduce_t
{
  hb_reduce_t (Redu r, InitT init_ਮੁੱਲ) : r (r), init_ਮੁੱਲ (init_ਮੁੱਲ) {}

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iter,
	    hb_requires (hb_is_iterator (Iter)),
	    ਕਿਸਮ_ਨਾਂ AccuT = hb_decay<decltype (hb_declval (Redu) (hb_declval (InitT), hb_declval (ਕਿਸਮ_ਨਾਂ Iter::ਵਸਤੂ_t)))>>
  AccuT
  ਸੰਚਾਲਕ () (Iter it)
  {
    AccuT value = init_ਮੁੱਲ;
    ਲਈ (; it; ++it)
      value = r (value, *it);
    ਵਾਪਸ value;
  }

  ਨਿਜੀ:
  Redu r;
  InitT init_ਮੁੱਲ;
};
ਬਣਤਰ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Redu, ਕਿਸਮ_ਨਾਂ InitT>
  hb_reduce_t<Redu, InitT>
  ਸੰਚਾਲਕ () (Redu&& r, InitT init_ਮੁੱਲ) ਅਚਲ
  { ਵਾਪਸ hb_reduce_t<Redu, InitT> (r, init_ਮੁੱਲ); }
}
HB_FUNCOBJ (hb_reduce);


/* hb_zip() */

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ A, ਕਿਸਮ_ਨਾਂ B>
ਬਣਤਰ hb_zip_iter_t :
  hb_iter_t<hb_zip_iter_t<A, B>,
	    hb_pair_t<ਕਿਸਮ_ਨਾਂ A::ਵਸਤੂ_t, ਕਿਸਮ_ਨਾਂ B::ਵਸਤੂ_t>>
{
  hb_zip_iter_t () {}
  hb_zip_iter_t (ਅਚਲ A& a, ਅਚਲ B& b) : a (a), b (b) {}

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ hb_pair_t<ਕਿਸਮ_ਨਾਂ A::ਵਸਤੂ_t, ਕਿਸਮ_ਨਾਂ B::ਵਸਤੂ_t> __item_t__;
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ is_random_access_iterator =
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
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ is_sorted_iterator = A::is_sorted_iterator;

  __item_t__ __item__ () ਅਚਲ { ਵਾਪਸ __item_t__ (*a, *b); }
  __item_t__ __item_at__ (ਬਿਨਾਨਿਸ਼ਾਨੀ i) ਅਚਲ { ਵਾਪਸ __item_t__ (a[i], b[i]); }
  ਤਾਰਕਿਕ __more__ () ਅਚਲ { ਵਾਪਸ ਤਾਰਕਿਕ (a) && ਤਾਰਕਿਕ (b); }
  ਬਿਨਾਨਿਸ਼ਾਨੀ __len__ () ਅਚਲ { ਵਾਪਸ hb_min (a.len (), b.len ()); }
  ਖਾਲੀ __next__ () { ++a; ++b; }
  ਖਾਲੀ __forward__ (ਬਿਨਾਨਿਸ਼ਾਨੀ n) { a += n; b += n; }
  ਖਾਲੀ __prev__ () { --a; --b; }
  ਖਾਲੀ __rewind__ (ਬਿਨਾਨਿਸ਼ਾਨੀ n) { a -= n; b -= n; }
  hb_zip_iter_t __end__ () ਅਚਲ { ਵਾਪਸ hb_zip_iter_t (a.ਸਿਰਾ_3 (), b.ਸਿਰਾ_3 ()); }
  /* Note, we should stop if ANY of the iters reaches end.  As such two compare
   * unequal if both items are unequal, NOT if either is unequal. */
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ hb_zip_iter_t& o) ਅਚਲ
  { ਵਾਪਸ a != o.a && b != o.b; }

  ਨਿਜੀ:
  A a;
  B b;
};
ਬਣਤਰ
{ HB_PARTIALIZE(2);
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ A, ਕਿਸਮ_ਨਾਂ B,
	    hb_requires (hb_is_iterable (A) && hb_is_iterable (B))>
  hb_zip_iter_t<hb_iter_type<A>, hb_iter_type<B>>
  ਸੰਚਾਲਕ () (A&& a, B&& b) ਅਚਲ
  { ਵਾਪਸ hb_zip_iter_t<hb_iter_type<A>, hb_iter_type<B>> (hb_iter (a), hb_iter (b)); }
}
HB_FUNCOBJ (hb_zip);

/* hb_concat() */

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ A, ਕਿਸਮ_ਨਾਂ B>
ਬਣਤਰ hb_concat_iter_t :
    hb_iter_t<hb_concat_iter_t<A, B>, ਕਿਸਮ_ਨਾਂ A::ਵਸਤੂ_t>
{
  hb_concat_iter_t () {}
  hb_concat_iter_t (A& a, B& b) : a (a), b (b) {}
  hb_concat_iter_t (ਅਚਲ A& a, ਅਚਲ B& b) : a (a), b (b) {}


  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ A::ਵਸਤੂ_t __item_t__;
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ is_random_access_iterator =
    A::is_random_access_iterator &&
    B::is_random_access_iterator;
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ is_sorted_iterator = ਝੂਠ;

  __item_t__ __item__ () ਅਚਲ
  {
    ਜੇ (!a)
      ਵਾਪਸ *b;
    ਵਾਪਸ *a;
  }

  __item_t__ __item_at__ (ਬਿਨਾਨਿਸ਼ਾਨੀ i) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ a_len = a.len ();
    ਜੇ (i < a_len)
      ਵਾਪਸ a[i];
    ਵਾਪਸ b[i - a_len];
  }

  ਤਾਰਕਿਕ __more__ () ਅਚਲ { ਵਾਪਸ ਤਾਰਕਿਕ (a) || ਤਾਰਕਿਕ (b); }

  ਬਿਨਾਨਿਸ਼ਾਨੀ __len__ () ਅਚਲ { ਵਾਪਸ a.len () + b.len (); }

  ਖਾਲੀ __next__ ()
  {
    ਜੇ (a)
      ++a;
    ਨਹੀਂਤਾਂ
      ++b;
  }

  ਖਾਲੀ __forward__ (ਬਿਨਾਨਿਸ਼ਾਨੀ n)
  {
    ਜੇ (!n) ਵਾਪਸ;
    ਜੇ (!is_random_access_iterator) {
      ਜਦੋਂ (n-- && *ਇਹ) {
        (*ਇਹ)++;
      }
      ਵਾਪਸ;
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ a_len = a.len ();
    ਜੇ (n > a_len) {
      n -= a_len;
      a.__forward__ (a_len);
      b.__forward__ (n);
    } ਨਹੀਂਤਾਂ {
      a.__forward__ (n);
    }
  }

  hb_concat_iter_t __end__ () ਅਚਲ { ਵਾਪਸ hb_concat_iter_t (a.ਸਿਰਾ_3 (), b.ਸਿਰਾ_3 ()); }
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ hb_concat_iter_t& o) ਅਚਲ
  {
    ਵਾਪਸ a != o.a
        || b != o.b;
  }

  ਨਿਜੀ:
  A a;
  B b;
};
ਬਣਤਰ
{ HB_PARTIALIZE(2);
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ A, ਕਿਸਮ_ਨਾਂ B,
	    hb_requires (hb_is_iterable (A) && hb_is_iterable (B))>
  hb_concat_iter_t<hb_iter_type<A>, hb_iter_type<B>>
  ਸੰਚਾਲਕ () (A&& a, B&& b) ਅਚਲ
  { ਵਾਪਸ hb_concat_iter_t<hb_iter_type<A>, hb_iter_type<B>> (hb_iter (a), hb_iter (b)); }
}
HB_FUNCOBJ (hb_concat);

/* hb_apply() */

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Appl>
ਬਣਤਰ hb_apply_t
{
  hb_apply_t (Appl a) : a (a) {}

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iter,
	    hb_requires (hb_is_iterator (Iter))>
  ਖਾਲੀ ਸੰਚਾਲਕ () (Iter it)
  {
    ਲਈ (; it; ++it)
      (ਖਾਲੀ) hb_invoke (a, *it);
  }

  ਨਿਜੀ:
  Appl a;
};
ਬਣਤਰ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Appl> hb_apply_t<Appl>
  ਸੰਚਾਲਕ () (Appl&& a) ਅਚਲ
  { ਵਾਪਸ hb_apply_t<Appl> (a); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Appl> hb_apply_t<Appl&>
  ਸੰਚਾਲਕ () (Appl *a) ਅਚਲ
  { ਵਾਪਸ hb_apply_t<Appl&> (*a); }
}
HB_FUNCOBJ (hb_apply);

/* hb_range()/hb_iota()/hb_repeat() */

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ S>
ਬਣਤਰ hb_range_iter_t :
  hb_iter_t<hb_range_iter_t<T, S>, T>
{
  hb_range_iter_t (T ਸ਼ੁਰੂ, T ਸਿਰਾ_4, S ਕਦਮ) : v (ਸ਼ੁਰੂ), ਸਿਰਾ_4 (ਸਿਰਾ_for (ਸ਼ੁਰੂ, ਸਿਰਾ_4, ਕਦਮ)), ਕਦਮ (ਕਦਮ) {}

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ T __item_t__;
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ is_random_access_iterator = ਸੱਚ;
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ is_sorted_iterator = ਸੱਚ;
  __item_t__ __item__ () ਅਚਲ { ਵਾਪਸ hb_ridentity (v); }
  __item_t__ __item_at__ (ਬਿਨਾਨਿਸ਼ਾਨੀ j) ਅਚਲ { ਵਾਪਸ v + j * ਕਦਮ; }
  ਤਾਰਕਿਕ __more__ () ਅਚਲ { ਵਾਪਸ v != ਸਿਰਾ_4; }
  ਬਿਨਾਨਿਸ਼ਾਨੀ __len__ () ਅਚਲ { ਵਾਪਸ !ਕਦਮ ? UINT_MAX : (ਸਿਰਾ_4 - v) / ਕਦਮ; }
  ਖਾਲੀ __next__ () { v += ਕਦਮ; }
  ਖਾਲੀ __forward__ (ਬਿਨਾਨਿਸ਼ਾਨੀ n) { v += n * ਕਦਮ; }
  ਖਾਲੀ __prev__ () { v -= ਕਦਮ; }
  ਖਾਲੀ __rewind__ (ਬਿਨਾਨਿਸ਼ਾਨੀ n) { v -= n * ਕਦਮ; }
  hb_range_iter_t __end__ () ਅਚਲ { ਵਾਪਸ hb_range_iter_t (ਸਿਰਾ_4, ਸਿਰਾ_4, ਕਦਮ); }
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ hb_range_iter_t& o) ਅਚਲ
  { ਵਾਪਸ v != o.v; }

  ਨਿਜੀ:
  ਸਥਿਰ ਸਤਰਅੰਦਰ T ਸਿਰਾ_for (T ਸ਼ੁਰੂ, T ਸਿਰਾ_4, S ਕਦਮ)
  {
    ਜੇ (!ਕਦਮ)
      ਵਾਪਸ ਸਿਰਾ_4;
    ਸਵੈਚਾਲਿਤ res = (ਸਿਰਾ_4 - ਸ਼ੁਰੂ) % ਕਦਮ;
    ਜੇ (!res)
      ਵਾਪਸ ਸਿਰਾ_4;
    ਸਿਰਾ_4 += ਕਦਮ - res;
    ਵਾਪਸ ਸਿਰਾ_4;
  }

  ਨਿਜੀ:
  T v;
  T ਸਿਰਾ_4;
  S ਕਦਮ;
};
ਬਣਤਰ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T = ਬਿਨਾਨਿਸ਼ਾਨੀ> hb_range_iter_t<T, ਬਿਨਾਨਿਸ਼ਾਨੀ>
  ਸੰਚਾਲਕ () (T ਸਿਰਾ = (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1) ਅਚਲ
  { ਵਾਪਸ hb_range_iter_t<T, ਬਿਨਾਨਿਸ਼ਾਨੀ> (0, ਸਿਰਾ, 1u); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ S = ਬਿਨਾਨਿਸ਼ਾਨੀ> hb_range_iter_t<T, S>
  ਸੰਚਾਲਕ () (T ਸ਼ੁਰੂ, T ਸਿਰਾ, S ਕਦਮ = 1u) ਅਚਲ
  { ਵਾਪਸ hb_range_iter_t<T, S> (ਸ਼ੁਰੂ, ਸਿਰਾ, ਕਦਮ); }
}
HB_FUNCOBJ (hb_range);

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ S>
ਬਣਤਰ hb_iota_iter_t :
  hb_iter_with_fallback_t<hb_iota_iter_t<T, S>, T>
{
  hb_iota_iter_t (T ਸ਼ੁਰੂ, S ਕਦਮ) : v (ਸ਼ੁਰੂ), ਕਦਮ (ਕਦਮ) {}

  ਨਿਜੀ:

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ S2 = S>
  ਸਵੈਚਾਲਿਤ
  inc (hb_type_ਪਛਾਣ_ਤੱਤ<S2> s, hb_ਤਰਜੀਹ<1>)
    -> hb_void_t<decltype (hb_invoke (std::forward<hb_type_ਪਛਾਣ_ਤੱਤ<S2>> (s),
                                      hb_declval<T&> ()))>
  { v = hb_invoke (std::forward<hb_type_ਪਛਾਣ_ਤੱਤ<S2>> (s), v); }

  ਖਾਲੀ
  inc (S s, hb_ਤਰਜੀਹ<0>)
  { v += s; }

  ਜਨਤਕ:

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ T __item_t__;
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ is_random_access_iterator = ਸੱਚ;
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ is_sorted_iterator = ਸੱਚ;
  __item_t__ __item__ () ਅਚਲ { ਵਾਪਸ hb_ridentity (v); }
  ਤਾਰਕਿਕ __more__ () ਅਚਲ { ਵਾਪਸ ਸੱਚ; }
  ਬਿਨਾਨਿਸ਼ਾਨੀ __len__ () ਅਚਲ { ਵਾਪਸ UINT_MAX; }
  ਖਾਲੀ __next__ () { inc (ਕਦਮ, hb_prioritize); }
  ਖਾਲੀ __prev__ () { v -= ਕਦਮ; }
  hb_iota_iter_t __end__ () ਅਚਲ { ਵਾਪਸ *ਇਹ; }
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ hb_iota_iter_t& o) ਅਚਲ { ਵਾਪਸ ਸੱਚ; }

  ਨਿਜੀ:
  T v;
  S ਕਦਮ;
};
ਬਣਤਰ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T = ਬਿਨਾਨਿਸ਼ਾਨੀ, ਕਿਸਮ_ਨਾਂ S = ਬਿਨਾਨਿਸ਼ਾਨੀ> hb_iota_iter_t<T, S>
  ਸੰਚਾਲਕ () (T ਸ਼ੁਰੂ = 0u, S ਕਦਮ = 1u) ਅਚਲ
  { ਵਾਪਸ hb_iota_iter_t<T, S> (ਸ਼ੁਰੂ, ਕਦਮ); }
}
HB_FUNCOBJ (hb_iota);

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ hb_repeat_iter_t :
  hb_iter_t<hb_repeat_iter_t<T>, T>
{
  hb_repeat_iter_t (T value) : v (value) {}

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ T __item_t__;
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ is_random_access_iterator = ਸੱਚ;
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ is_sorted_iterator = ਸੱਚ;
  __item_t__ __item__ () ਅਚਲ { ਵਾਪਸ v; }
  __item_t__ __item_at__ (ਬਿਨਾਨਿਸ਼ਾਨੀ j) ਅਚਲ { ਵਾਪਸ v; }
  ਤਾਰਕਿਕ __more__ () ਅਚਲ { ਵਾਪਸ ਸੱਚ; }
  ਬਿਨਾਨਿਸ਼ਾਨੀ __len__ () ਅਚਲ { ਵਾਪਸ UINT_MAX; }
  ਖਾਲੀ __next__ () {}
  ਖਾਲੀ __forward__ (ਬਿਨਾਨਿਸ਼ਾਨੀ) {}
  ਖਾਲੀ __prev__ () {}
  ਖਾਲੀ __rewind__ (ਬਿਨਾਨਿਸ਼ਾਨੀ) {}
  hb_repeat_iter_t __end__ () ਅਚਲ { ਵਾਪਸ *ਇਹ; }
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ hb_repeat_iter_t& o) ਅਚਲ { ਵਾਪਸ ਸੱਚ; }

  ਨਿਜੀ:
  T v;
};
ਬਣਤਰ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> hb_repeat_iter_t<T>
  ਸੰਚਾਲਕ () (T value) ਅਚਲ
  { ਵਾਪਸ hb_repeat_iter_t<T> (value); }
}
HB_FUNCOBJ (hb_repeat);

/* hb_enumerate()/hb_take() */

ਬਣਤਰ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterable,
	    ਕਿਸਮ_ਨਾਂ ਸੂਚਕਾਂਕ = ਬਿਨਾਨਿਸ਼ਾਨੀ,
	    hb_requires (hb_is_iterable (Iterable))>
  ਸਵੈਚਾਲਿਤ ਸੰਚਾਲਕ () (Iterable&& it, ਸੂਚਕਾਂਕ ਸ਼ੁਰੂ = 0u) ਅਚਲ HB_AUTO_RETURN
  ( hb_zip (hb_iota (ਸ਼ੁਰੂ), it) )
}
HB_FUNCOBJ (hb_enumerate);

ਬਣਤਰ
{ HB_PARTIALIZE(2);
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  ਸਵੈਚਾਲਿਤ ਸੰਚਾਲਕ () (Iterable&& it, ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2) ਅਚਲ HB_AUTO_RETURN
  ( hb_zip (hb_range (ਗਿਣਤੀ_2), it) | hb_map_retains_ਛਾਂਟੀ (hb_ਦੂਜਾ) )

  /* Specialization arrays. */

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type> ਸਤਰਅੰਦਰ hb_array_t<Type>
  ਸੰਚਾਲਕ () (hb_array_t<Type> array, ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2) ਅਚਲ
  { ਵਾਪਸ array.sub_array (0, ਗਿਣਤੀ_2); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type> ਸਤਰਅੰਦਰ hb_sorted_array_t<Type>
  ਸੰਚਾਲਕ () (hb_sorted_array_t<Type> array, ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2) ਅਚਲ
  { ਵਾਪਸ array.sub_array (0, ਗਿਣਤੀ_2); }
}
HB_FUNCOBJ (hb_take);

ਬਣਤਰ
{ HB_PARTIALIZE(2);
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iter,
	    hb_requires (hb_is_iterator (Iter))>
  ਸਵੈਚਾਲਿਤ ਸੰਚਾਲਕ () (Iter it, ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2) ਅਚਲ HB_AUTO_RETURN
  (
    + hb_iota (it, hb_ਜੋੜੋ (ਗਿਣਤੀ_2))
    | hb_map (hb_take (ਗਿਣਤੀ_2))
    | hb_take ((hb_len (it) + ਗਿਣਤੀ_2 - 1) / ਗਿਣਤੀ_2)
  )
}
HB_FUNCOBJ (hb_chop);

/* hb_sink() */

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Sink>
ਬਣਤਰ hb_sink_t
{
  hb_sink_t (Sink s) : s (s) {}

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iter,
	    hb_requires (hb_is_iterator (Iter))>
  ਖਾਲੀ ਸੰਚਾਲਕ () (Iter it)
  {
    ਲਈ (; it; ++it)
      s << *it;
  }

  ਨਿਜੀ:
  Sink s;
};
ਬਣਤਰ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Sink> hb_sink_t<Sink>
  ਸੰਚਾਲਕ () (Sink&& s) ਅਚਲ
  { ਵਾਪਸ hb_sink_t<Sink> (s); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Sink> hb_sink_t<Sink&>
  ਸੰਚਾਲਕ () (Sink *s) ਅਚਲ
  { ਵਾਪਸ hb_sink_t<Sink&> (*s); }
}
HB_FUNCOBJ (hb_sink);

/* hb-drain: hb_sink to void / blackhole / /dev/null. */

ਬਣਤਰ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iter,
	    hb_requires (hb_is_iterator (Iter))>
  ਖਾਲੀ ਸੰਚਾਲਕ () (Iter it) ਅਚਲ
  {
    ਲਈ (; it; ++it)
      (ਖਾਲੀ) *it;
  }
}
HB_FUNCOBJ (hb_drain);

/* hb_unzip(): unzip and sink to two sinks. */

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Sink1, ਕਿਸਮ_ਨਾਂ Sink2>
ਬਣਤਰ hb_unzip_t
{
  hb_unzip_t (Sink1 s1, Sink2 s2) : s1 (s1), s2 (s2) {}

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iter,
	    hb_requires (hb_is_iterator (Iter))>
  ਖਾਲੀ ਸੰਚਾਲਕ () (Iter it)
  {
    ਲਈ (; it; ++it)
    {
      ਅਚਲ ਸਵੈਚਾਲਿਤ &v = *it;
      s1 << v.ਪਹਿਲਾ;
      s2 << v.second;
    }
  }

  ਨਿਜੀ:
  Sink1 s1;
  Sink2 s2;
};
ਬਣਤਰ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Sink1, ਕਿਸਮ_ਨਾਂ Sink2> hb_unzip_t<Sink1, Sink2>
  ਸੰਚਾਲਕ () (Sink1&& s1, Sink2&& s2) ਅਚਲ
  { ਵਾਪਸ hb_unzip_t<Sink1, Sink2> (s1, s2); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Sink1, ਕਿਸਮ_ਨਾਂ Sink2> hb_unzip_t<Sink1&, Sink2&>
  ਸੰਚਾਲਕ () (Sink1 *s1, Sink2 *s2) ਅਚਲ
  { ਵਾਪਸ hb_unzip_t<Sink1&, Sink2&> (*s1, *s2); }
}
HB_FUNCOBJ (hb_unzip);


/* hb-all, hb-any, hb-none. */

ਬਣਤਰ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterable,
	    ਕਿਸਮ_ਨਾਂ Pred = decltype ((hb_ਪਛਾਣ_ਤੱਤ)),
	    ਕਿਸਮ_ਨਾਂ Proj = decltype ((hb_ਪਛਾਣ_ਤੱਤ)),
	    hb_requires (hb_is_iterable (Iterable))>
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ () (Iterable&& c,
		    Pred&& p = hb_ਪਛਾਣ_ਤੱਤ,
		    Proj&& f = hb_ਪਛਾਣ_ਤੱਤ) ਅਚਲ
  {
    ਲਈ (ਸਵੈਚਾਲਿਤ it = hb_iter (c); it; ++it)
      ਜੇ (!hb_ਮੇਲ (p, hb_get (f, *it)))
	ਵਾਪਸ ਝੂਠ;
    ਵਾਪਸ ਸੱਚ;
  }
}
HB_FUNCOBJ (hb_all);
ਬਣਤਰ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterable,
	    ਕਿਸਮ_ਨਾਂ Pred = decltype ((hb_ਪਛਾਣ_ਤੱਤ)),
	    ਕਿਸਮ_ਨਾਂ Proj = decltype ((hb_ਪਛਾਣ_ਤੱਤ)),
	    hb_requires (hb_is_iterable (Iterable))>
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ () (Iterable&& c,
		    Pred&& p = hb_ਪਛਾਣ_ਤੱਤ,
		    Proj&& f = hb_ਪਛਾਣ_ਤੱਤ) ਅਚਲ
  {
    ਲਈ (ਸਵੈਚਾਲਿਤ it = hb_iter (c); it; ++it)
      ਜੇ (hb_ਮੇਲ (p, hb_get (f, *it)))
	ਵਾਪਸ ਸੱਚ;
    ਵਾਪਸ ਝੂਠ;
  }
}
HB_FUNCOBJ (hb_any);
ਬਣਤਰ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterable,
	    ਕਿਸਮ_ਨਾਂ Pred = decltype ((hb_ਪਛਾਣ_ਤੱਤ)),
	    ਕਿਸਮ_ਨਾਂ Proj = decltype ((hb_ਪਛਾਣ_ਤੱਤ)),
	    hb_requires (hb_is_iterable (Iterable))>
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ () (Iterable&& c,
		    Pred&& p = hb_ਪਛਾਣ_ਤੱਤ,
		    Proj&& f = hb_ਪਛਾਣ_ਤੱਤ) ਅਚਲ
  {
    ਲਈ (ਸਵੈਚਾਲਿਤ it = hb_iter (c); it; ++it)
      ਜੇ (hb_ਮੇਲ (p, hb_get (f, *it)))
	ਵਾਪਸ ਝੂਠ;
    ਵਾਪਸ ਸੱਚ;
  }
}
HB_FUNCOBJ (hb_none);

/*
 * Algorithms operating on iterators.
 */

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ C, ਕਿਸਮ_ਨਾਂ V,
	  hb_requires (hb_is_iterable (C))>
ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_fill (C&& c, ਅਚਲ V &v)
{
  ਲਈ (ਸਵੈਚਾਲਿਤ i = hb_iter (c); i; i++)
    *i = v;
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ S, ਕਿਸਮ_ਨਾਂ D>
ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_copy (S&& is, D&& id)
{
  hb_iter (is) | hb_sink (id);
}


#ਜੇ_ਅੰਤ /* HB_ITER_HH */

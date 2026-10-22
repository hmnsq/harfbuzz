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

#که_نه_تعریف_شوی HB_ITER_HH
#تعریف_کړه HB_ITER_HH

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
کينډۍ <ډول_نوم iter_t, ډول_نوم توکی_3 = ډول_نوم iter_t::__item_t__>
بنية hb_iter_t
{
  تعريفنوع توکی_3 توکی_t;
  constexpr بینښه get_توکی_اندازه () ثابت { أرجع hb_static_اندازه (توکی_3); }
  ساكن constexpr منطقي is_iterator = ریښتیا;
  ساكن constexpr منطقي is_random_access_iterator = خطأ;
  ساكن constexpr منطقي is_sorted_iterator = خطأ;
  ساكن constexpr منطقي has_fast_len = خطأ; // Should be checked in combination with is_random_access_iterator.

  خاص:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ثابت iter_t* thiz () ثابت { أرجع static_cast<ثابت iter_t *> (هذا); }
	iter_t* thiz ()       { أرجع static_cast<      iter_t *> (هذا); }
  عام:

  /* Operators. */
  iter_t iter () ثابت { أرجع *thiz(); }
  iter_t چلوونکی + () ثابت { أرجع *thiz(); }
  iter_t _begin () ثابت { أرجع *thiz(); }
  iter_t begin () ثابت { أرجع _begin (); }
  iter_t پای_ټکی_3 () ثابت { أرجع thiz()->__end__ (); }
  iter_t پای_ټکی () ثابت { أرجع پای_ټکی_3 (); }
  څرګند چلوونکی منطقي () ثابت { أرجع thiz()->__more__ (); }
  بینښه len () ثابت { أرجع thiz()->__len__ (); }
  /* The following can only be enabled if item_t is reference type.  Otherwise
   * it will be returning pointer to temporary rvalue. */
  کينډۍ <ډول_نوم T = توکی_t,
	    hb_enable_if (std::is_reference<T>::value)>
  hb_remove_reference<توکی_t>* چلوونکی -> () ثابت { أرجع std::addressof (**thiz()); }
  توکی_t چلوونکی * () ثابت { أرجع thiz()->__item__ (); }
  توکی_t چلوونکی * () { أرجع thiz()->__item__ (); }
  توکی_t چلوونکی [] (بینښه i) ثابت { أرجع thiz()->__item_at__ (i); }
  توکی_t چلوونکی [] (بینښه i) { أرجع thiz()->__item_at__ (i); }
  iter_t& چلوونکی += (بینښه شمېر) &  { thiz()->__forward__ (شمېر); أرجع *thiz(); }
  iter_t  چلوونکی += (بینښه شمېر) && { thiz()->__forward__ (شمېر); أرجع *thiz(); }
  iter_t& چلوونکی ++ () &  { thiz()->__next__ (); أرجع *thiz(); }
  iter_t  چلوونکی ++ () && { thiz()->__next__ (); أرجع *thiz(); }
  iter_t& چلوونکی -= (بینښه شمېر) &  { thiz()->__rewind__ (شمېر); أرجع *thiz(); }
  iter_t  چلوونکی -= (بینښه شمېر) && { thiz()->__rewind__ (شمېر); أرجع *thiz(); }
  iter_t& چلوونکی -- () &  { thiz()->__prev__ (); أرجع *thiz(); }
  iter_t  چلوونکی -- () && { thiz()->__prev__ (); أرجع *thiz(); }
  iter_t چلوونکی + (بینښه شمېر) ثابت { خپلکاری c = thiz()->iter (); c += شمېر; أرجع c; }
  ملګری iter_t چلوونکی + (بینښه شمېر, ثابت iter_t &it) { أرجع it + شمېر; }
  iter_t چلوونکی ++ (صحيح) { iter_t c (*thiz()); ++*thiz(); أرجع c; }
  iter_t چلوونکی - (بینښه شمېر) ثابت { خپلکاری c = thiz()->iter (); c -= شمېر; أرجع c; }
  iter_t چلوونکی -- (صحيح) { iter_t c (*thiz()); --*thiz(); أرجع c; }
  کينډۍ <ډول_نوم T>
  iter_t& چلوونکی >> (T &v) &  { v = **thiz(); ++*thiz(); أرجع *thiz(); }
  کينډۍ <ډول_نوم T>
  iter_t  چلوونکی >> (T &v) && { v = **thiz(); ++*thiz(); أرجع *thiz(); }
  کينډۍ <ډول_نوم T>
  iter_t& چلوونکی << (ثابت T v) &  { **thiz() = v; ++*thiz(); أرجع *thiz(); }
  کينډۍ <ډول_نوم T>
  iter_t  چلوونکی << (ثابت T v) && { **thiz() = v; ++*thiz(); أرجع *thiz(); }

  محمي:
  hb_iter_t () = افتراضي;
  hb_iter_t (ثابت hb_iter_t &o HB_ناکارول_شوی) = افتراضي;
  hb_iter_t (hb_iter_t &&o HB_ناکارول_شوی) = افتراضي;
  hb_iter_t& چلوونکی = (ثابت hb_iter_t &o HB_ناکارول_شوی) = افتراضي;
  hb_iter_t& چلوونکی = (hb_iter_t &&o HB_ناکارول_شوی) = افتراضي;
};

#تعریف_کړه HB_ITER_USING(Name) \
  کارول توکی_t = ډول_نوم Name::توکی_t; \
  کارول Name::_begin; \
  کارول Name::begin; \
  کارول Name::پای_ټکی_3; \
  کارول Name::پای_ټکی; \
  کارول Name::get_توکی_اندازه; \
  کارول Name::is_iterator; \
  کارول Name::iter; \
  کارول Name::چلوونکی منطقي; \
  کارول Name::len; \
  کارول Name::چلوونکی ->; \
  کارول Name::چلوونکی *; \
  کارول Name::چلوونکی []; \
  کارول Name::چلوونکی +=; \
  کارول Name::چلوونکی ++; \
  کارول Name::چلوونکی -=; \
  کارول Name::چلوونکی --; \
  کارول Name::چلوونکی +; \
  کارول Name::چلوونکی -; \
  کارول Name::چلوونکی >>; \
  کارول Name::چلوونکی <<; \
  static_assert (ریښتیا, "")

/* Returns iterator / item type of a type. */
کينډۍ <ډول_نوم Iterable>
کارول hb_iter_type = decltype (hb_deref (hb_declval (Iterable)).iter ());
کينډۍ <ډول_نوم Iterable>
کارول hb_توکی_type = decltype (*hb_deref (hb_declval (Iterable)).iter ());


کينډۍ <ډول_نوم> بنية hb_array_t;
کينډۍ <ډول_نوم> بنية hb_sorted_array_t;

بنية
{
  کينډۍ <ډول_نوم T> hb_iter_type<T>
  چلوونکی () (T&& c) ثابت
  { أرجع hb_deref (std::forward<T> (c)).iter (); }

  /* Specialization for C arrays. */

  کينډۍ <ډول_نوم Type> مضمن hb_array_t<Type>
  چلوونکی () (Type *array, بینښه صحيح اوږدتیا) ثابت
  { أرجع hb_array_t<Type> (array, اوږدتیا); }

  کينډۍ <ډول_نوم Type, بینښه صحيح اوږدتیا> hb_array_t<Type>
  چلوونکی () (Type (&array)[اوږدتیا]) ثابت
  { أرجع hb_array_t<Type> (array, اوږدتیا); }

}
HB_FUNCOBJ (hb_iter);
بنية
{
  کينډۍ <ډول_نوم T> خپلکاری
  impl (T&& c, hb_لومړیتوب<1>) ثابت HB_RETURN (بینښه, c.len ())

  کينډۍ <ډول_نوم T> خپلکاری
  impl (T&& c, hb_لومړیتوب<0>) ثابت HB_RETURN (بینښه, c.len)

  عام:

  کينډۍ <ډول_نوم T> خپلکاری
  چلوونکی () (T&& c) ثابت HB_RETURN (بینښه, impl (std::forward<T> (c), hb_prioritize))
}
HB_FUNCOBJ (hb_len);

/* Mixin to fill in what the subclass doesn't provide. */
کينډۍ <ډول_نوم iter_t, ډول_نوم توکی_t = ډول_نوم iter_t::__item_t__>
بنية hb_iter_fallback_mixin_t
{
  خاص:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ثابت iter_t* thiz () ثابت { أرجع static_cast<ثابت iter_t *> (هذا); }
	iter_t* thiz ()       { أرجع static_cast<      iter_t *> (هذا); }
  عام:

  /* Access: Implement __item__(), or __item_at__() if random-access. */
  توکی_t __item__ () ثابت { أرجع (*thiz())[0]; }
  توکی_t __item_at__ (بینښه i) ثابت { أرجع *(*thiz() + i); }

  /* Termination: Implement __more__(), or __len__() if random-access. */
  منطقي __more__ () ثابت { أرجع منطقي (thiz()->len ()); }
  بینښه __len__ () ثابت
  { iter_t c (*thiz()); بینښه l = 0; طالما (c) { c++; l++; } أرجع l; }

  /* Advancing: Implement __next__(), or __forward__() if random-access. */
  فراغ __next__ () { *thiz() += 1; }
  فراغ __forward__ (بینښه n) { طالما (*thiz() && n--) ++*thiz(); }

  /* Rewinding: Implement __prev__() or __rewind__() if bidirectional. */
  فراغ __prev__ () { *thiz() -= 1; }
  فراغ __rewind__ (بینښه n) { طالما (*thiz() && n--) --*thiz(); }

  /* Range-based for: Implement __end__() if can be done faster,
   * and operator!=. */
  iter_t __end__ () ثابت
  {
    إذا (thiz()->is_random_access_iterator)
      أرجع *thiz() + thiz()->len ();
    /* Above expression loops twice. Following loops once. */
    خپلکاری it = *thiz();
    طالما (it) ++it;
    أرجع it;
  }

  محمي:
  hb_iter_fallback_mixin_t () = افتراضي;
  hb_iter_fallback_mixin_t (ثابت hb_iter_fallback_mixin_t &o HB_ناکارول_شوی) = افتراضي;
  hb_iter_fallback_mixin_t (hb_iter_fallback_mixin_t &&o HB_ناکارول_شوی) = افتراضي;
  hb_iter_fallback_mixin_t& چلوونکی = (ثابت hb_iter_fallback_mixin_t &o HB_ناکارول_شوی) = افتراضي;
  hb_iter_fallback_mixin_t& چلوونکی = (hb_iter_fallback_mixin_t &&o HB_ناکارول_شوی) = افتراضي;
};

کينډۍ <ډول_نوم iter_t, ډول_نوم توکی_t = ډول_نوم iter_t::__item_t__>
بنية hb_iter_with_fallback_t :
  hb_iter_t<iter_t, توکی_t>,
  hb_iter_fallback_mixin_t<iter_t, توکی_t>
{
  محمي:
  hb_iter_with_fallback_t () = افتراضي;
  hb_iter_with_fallback_t (ثابت hb_iter_with_fallback_t &o HB_ناکارول_شوی) = افتراضي;
  hb_iter_with_fallback_t (hb_iter_with_fallback_t &&o HB_ناکارول_شوی) = افتراضي;
  hb_iter_with_fallback_t& چلوونکی = (ثابت hb_iter_with_fallback_t &o HB_ناکارول_شوی) = افتراضي;
  hb_iter_with_fallback_t& چلوونکی = (hb_iter_with_fallback_t &&o HB_ناکارول_شوی) = افتراضي;
};

/*
 * Meta-programming predicates.
 */

/* hb_is_iterator() / hb_is_iterator_of() */

کينډۍ<ډول_نوم Iter, ډول_نوم توکی_3>
بنية hb_is_iterator_د
{
  کينډۍ <ډول_نوم Item2 = توکی_3>
  ساكن hb_true_type impl (hb_لومړیتوب<2>, hb_iter_t<Iter, hb_type_بې_اغېزه_عنصر<Item2>> *);
  ساكن hb_false_type impl (hb_لومړیتوب<0>, ثابت فراغ *);

  عام:
  ساكن constexpr منطقي value = decltype (impl (hb_prioritize, hb_declval (Iter*)))::value;
};
#تعریف_کړه hb_is_iterator_د(Iter, توکی_3) hb_is_iterator_د<Iter, توکی_3>::value
#تعریف_کړه hb_is_iterator(Iter) hb_is_iterator_د (Iter, ډول_نوم Iter::توکی_t)
#تعریف_کړه hb_is_sorted_iterator_د(Iter, توکی_3) (hb_is_iterator_د<Iter, توکی_3>::value && Iter::is_sorted_iterator)
#تعریف_کړه hb_is_sorted_iterator(Iter) hb_is_sorted_iterator_د (Iter, ډول_نوم Iter::توکی_t)

/* hb_is_iterable() */

کينډۍ <ډول_نوم T>
بنية hb_is_iterable
{
  خاص:

  کينډۍ <ډول_نوم U>
  ساكن خپلکاری impl (hb_لومړیتوب<1>) -> decltype (hb_declval (U).iter (), hb_true_type ());

  کينډۍ <ډول_نوم>
  ساكن hb_false_type impl (hb_لومړیتوب<0>);

  عام:
  ساكن constexpr منطقي value = decltype (impl<T> (hb_prioritize))::value;
};
#تعریف_کړه hb_is_iterable(Iterable) hb_is_iterable<Iterable>::value

/* hb_is_source_of() / hb_is_sink_of() */

کينډۍ<ډول_نوم Iter, ډول_نوم توکی_3>
بنية hb_is_سرچینه_د
{
  خاص:
  کينډۍ <ډول_نوم Iter2 = Iter,
	    hb_enable_if (hb_is_convertible (ډول_نوم Iter2::توکی_t, hb_جمع_کړه_lvalue_reference<ثابت توکی_3>))>
  ساكن hb_true_type impl (hb_لومړیتوب<2>);
  کينډۍ <ډول_نوم Iter2 = Iter>
  ساكن خپلکاری impl (hb_لومړیتوب<1>) -> decltype (hb_declval (Iter2) >> hb_declval (توکی_3 &), hb_true_type ());
  ساكن hb_false_type impl (hb_لومړیتوب<0>);

  عام:
  ساكن constexpr منطقي value = decltype (impl (hb_prioritize))::value;
};
#تعریف_کړه hb_is_سرچینه_د(Iter, توکی_3) hb_is_سرچینه_د<Iter, توکی_3>::value

کينډۍ<ډول_نوم Iter, ډول_نوم توکی_3>
بنية hb_is_sink_د
{
  خاص:
  کينډۍ <ډول_نوم Iter2 = Iter,
	    hb_enable_if (hb_is_convertible (ډول_نوم Iter2::توکی_t, hb_جمع_کړه_lvalue_reference<توکی_3>))>
  ساكن hb_true_type impl (hb_لومړیتوب<2>);
  کينډۍ <ډول_نوم Iter2 = Iter>
  ساكن خپلکاری impl (hb_لومړیتوب<1>) -> decltype (hb_declval (Iter2) << hb_declval (توکی_3), hb_true_type ());
  ساكن hb_false_type impl (hb_لومړیتوب<0>);

  عام:
  ساكن constexpr منطقي value = decltype (impl (hb_prioritize))::value;
};
#تعریف_کړه hb_is_sink_د(Iter, توکی_3) hb_is_sink_د<Iter, توکی_3>::value

/* This is commonly used, so define: */
#تعریف_کړه hb_is_sorted_سرچینه_د(Iter, توکی_3) \
	(hb_is_سرچینه_د(Iter, توکی_3) && Iter::is_sorted_iterator)


بنية
{
  کينډۍ <ډول_نوم Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  بینښه چلوونکی () (ثابت Iterable &_) ثابت { أرجع hb_len (hb_iter (_)); }

  بینښه چلوونکی () (بینښه _) ثابت { أرجع _; }
}
HB_FUNCOBJ (hb_len_د);

/* Range-based 'for' for iterables. */

کينډۍ <ډول_نوم Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ساكن مضمن خپلکاری begin (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).begin ())

کينډۍ <ډول_نوم Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ساكن مضمن خپلکاری پای_ټکی (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).پای_ټکی ())

/* begin()/end() are NOT looked up non-ADL.  So each namespace must declare them.
 * Do it for namespace OT. */
نطاق OT {

کينډۍ <ډول_نوم Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ساكن مضمن خپلکاری begin (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).begin ())

کينډۍ <ډول_نوم Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ساكن مضمن خپلکاری پای_ټکی (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).پای_ټکی ())

}


/*
 * Adaptors, combiners, etc.
 */

کينډۍ <ډول_نوم Lhs, ډول_نوم Rhs,
	  hb_requires (hb_is_iterator (Lhs))>
ساكن مضمن خپلکاری
چلوونکی | (Lhs&& lhs, Rhs&& rhs) HB_AUTO_RETURN (std::forward<Rhs> (rhs) (std::forward<Lhs> (lhs)))

/* hb_map(), hb_filter(), hb_reduce() */

تعداد  صنف hb_function_sortedness_t {
  NOT_SORTED,
  RETAINS_ترتیبونه,
  SORTED,
};

کينډۍ <ډول_نوم Iter, ډول_نوم Proj, hb_function_sortedness_t Sorted,
	 hb_requires (hb_is_iterator (Iter))>
بنية hb_map_iter_t :
  hb_iter_t<hb_map_iter_t<Iter, Proj, Sorted>,
	    decltype (hb_get (hb_declval (Proj), *hb_declval (Iter)))>
{
  hb_map_iter_t (ثابت Iter& it, Proj f_) : it (it), f (f_) {}

  تعريفنوع decltype (hb_get (hb_declval (Proj), *hb_declval (Iter))) __item_t__;
  ساكن constexpr منطقي is_random_access_iterator = Iter::is_random_access_iterator;
  ساكن constexpr منطقي is_sorted_iterator =
    Sorted == hb_function_sortedness_t::SORTED ? ریښتیا :
    Sorted == hb_function_sortedness_t::RETAINS_ترتیبونه ? Iter::is_sorted_iterator :
    خطأ;
  __item_t__ __item__ () ثابت { أرجع hb_get (f.get (), *it); }
  __item_t__ __item_at__ (بینښه i) ثابت { أرجع hb_get (f.get (), it[i]); }
  منطقي __more__ () ثابت { أرجع منطقي (it); }
  بینښه __len__ () ثابت { أرجع it.len (); }
  فراغ __next__ () { ++it; }
  فراغ __forward__ (بینښه n) { it += n; }
  فراغ __prev__ () { --it; }
  فراغ __rewind__ (بینښه n) { it -= n; }
  hb_map_iter_t __end__ () ثابت { أرجع hb_map_iter_t (it.پای_ټکی_3 (), f); }
  منطقي چلوونکی != (ثابت hb_map_iter_t& o) ثابت
  { أرجع it != o.it; }

  خاص:
  Iter it;
  بدلیدونکی hb_reference_wrapper<Proj> f;
};

کينډۍ <ډول_نوم Proj, hb_function_sortedness_t Sorted>
بنية hb_map_iter_factory_t
{
  hb_map_iter_factory_t (Proj f) : f (f) {}

  کينډۍ <ډول_نوم Iter,
	    hb_requires (hb_is_iterator (Iter))>
  hb_map_iter_t<Iter, Proj, Sorted>
  چلوونکی () (Iter it)
  { أرجع hb_map_iter_t<Iter, Proj, Sorted> (it, f); }

  خاص:
  Proj f;
};
بنية
{
  کينډۍ <ډول_نوم Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::NOT_SORTED>
  چلوونکی () (Proj&& f) ثابت
  { أرجع hb_map_iter_factory_t<Proj, hb_function_sortedness_t::NOT_SORTED> (f); }
}
HB_FUNCOBJ (hb_map);
بنية
{
  کينډۍ <ډول_نوم Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::RETAINS_ترتیبونه>
  چلوونکی () (Proj&& f) ثابت
  { أرجع hb_map_iter_factory_t<Proj, hb_function_sortedness_t::RETAINS_ترتیبونه> (f); }
}
HB_FUNCOBJ (hb_map_retains_ترتیبونه);
بنية
{
  کينډۍ <ډول_نوم Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::SORTED>
  چلوونکی () (Proj&& f) ثابت
  { أرجع hb_map_iter_factory_t<Proj, hb_function_sortedness_t::SORTED> (f); }
}
HB_FUNCOBJ (hb_map_sorted);

کينډۍ <ډول_نوم Iter, ډول_نوم Pred, ډول_نوم Proj,
	 hb_requires (hb_is_iterator (Iter))>
بنية hb_filter_iter_t :
  hb_iter_with_fallback_t<hb_filter_iter_t<Iter, Pred, Proj>,
			  ډول_نوم Iter::توکی_t>
{
  hb_filter_iter_t (ثابت Iter& it_, Pred p_, Proj f_) : it (it_), p (p_), f (f_)
  { طالما (it && !hb_has (p.get (), hb_get (f.get (), *it))) ++it; }

  تعريفنوع ډول_نوم Iter::توکی_t __item_t__;
  ساكن constexpr منطقي is_sorted_iterator = Iter::is_sorted_iterator;
  __item_t__ __item__ () ثابت { أرجع *it; }
  منطقي __more__ () ثابت { أرجع منطقي (it); }
  فراغ __next__ () { افعل ++it; طالما (it && !hb_has (p.get (), hb_get (f.get (), *it))); }
  فراغ __prev__ ()
  {
    بینښه شمېر = 0;
    افعل
    {
      Iter old = it;
      --it;
      إذا (!(it != old))
      {
	it += شمېر;
	أرجع;
      }
      شمېر++;
    }
    طالما (!hb_has (p.get (), hb_get (f.get (), *it)));
  }
  hb_filter_iter_t __end__ () ثابت { أرجع hb_filter_iter_t (it.پای_ټکی_3 (), p, f); }
  منطقي چلوونکی != (ثابت hb_filter_iter_t& o) ثابت
  { أرجع it != o.it; }

  خاص:
  Iter it;
  بدلیدونکی hb_reference_wrapper<Pred> p;
  بدلیدونکی hb_reference_wrapper<Proj> f;
};
کينډۍ <ډول_نوم Pred, ډول_نوم Proj>
بنية hb_filter_iter_factory_t
{
  hb_filter_iter_factory_t (Pred p, Proj f) : p (p), f (f) {}

  کينډۍ <ډول_نوم Iter,
	    hb_requires (hb_is_iterator (Iter))>
  hb_filter_iter_t<Iter, Pred, Proj>
  چلوونکی () (Iter it)
  { أرجع hb_filter_iter_t<Iter, Pred, Proj> (it, p, f); }

  خاص:
  Pred p;
  Proj f;
};
بنية
{
  کينډۍ <ډول_نوم Pred = decltype ((hb_بې_اغېزه_عنصر)),
	    ډول_نوم Proj = decltype ((hb_بې_اغېزه_عنصر))>
  hb_filter_iter_factory_t<Pred, Proj>
  چلوونکی () (Pred&& p = hb_بې_اغېزه_عنصر, Proj&& f = hb_بې_اغېزه_عنصر) ثابت
  { أرجع hb_filter_iter_factory_t<Pred, Proj> (p, f); }
}
HB_FUNCOBJ (hb_filter);

کينډۍ <ډول_نوم Redu, ډول_نوم InitT>
بنية hb_reduce_t
{
  hb_reduce_t (Redu r, InitT init_ارزښت) : r (r), init_ارزښت (init_ارزښت) {}

  کينډۍ <ډول_نوم Iter,
	    hb_requires (hb_is_iterator (Iter)),
	    ډول_نوم AccuT = hb_decay<decltype (hb_declval (Redu) (hb_declval (InitT), hb_declval (ډول_نوم Iter::توکی_t)))>>
  AccuT
  چلوونکی () (Iter it)
  {
    AccuT value = init_ارزښت;
    لكل (; it; ++it)
      value = r (value, *it);
    أرجع value;
  }

  خاص:
  Redu r;
  InitT init_ارزښت;
};
بنية
{
  کينډۍ <ډول_نوم Redu, ډول_نوم InitT>
  hb_reduce_t<Redu, InitT>
  چلوونکی () (Redu&& r, InitT init_ارزښت) ثابت
  { أرجع hb_reduce_t<Redu, InitT> (r, init_ارزښت); }
}
HB_FUNCOBJ (hb_reduce);


/* hb_zip() */

کينډۍ <ډول_نوم A, ډول_نوم B>
بنية hb_zip_iter_t :
  hb_iter_t<hb_zip_iter_t<A, B>,
	    hb_pair_t<ډول_نوم A::توکی_t, ډول_نوم B::توکی_t>>
{
  hb_zip_iter_t () {}
  hb_zip_iter_t (ثابت A& a, ثابت B& b) : a (a), b (b) {}

  تعريفنوع hb_pair_t<ډول_نوم A::توکی_t, ډول_نوم B::توکی_t> __item_t__;
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

  __item_t__ __item__ () ثابت { أرجع __item_t__ (*a, *b); }
  __item_t__ __item_at__ (بینښه i) ثابت { أرجع __item_t__ (a[i], b[i]); }
  منطقي __more__ () ثابت { أرجع منطقي (a) && منطقي (b); }
  بینښه __len__ () ثابت { أرجع hb_min (a.len (), b.len ()); }
  فراغ __next__ () { ++a; ++b; }
  فراغ __forward__ (بینښه n) { a += n; b += n; }
  فراغ __prev__ () { --a; --b; }
  فراغ __rewind__ (بینښه n) { a -= n; b -= n; }
  hb_zip_iter_t __end__ () ثابت { أرجع hb_zip_iter_t (a.پای_ټکی_3 (), b.پای_ټکی_3 ()); }
  /* Note, we should stop if ANY of the iters reaches end.  As such two compare
   * unequal if both items are unequal, NOT if either is unequal. */
  منطقي چلوونکی != (ثابت hb_zip_iter_t& o) ثابت
  { أرجع a != o.a && b != o.b; }

  خاص:
  A a;
  B b;
};
بنية
{ HB_PARTIALIZE(2);
  کينډۍ <ډول_نوم A, ډول_نوم B,
	    hb_requires (hb_is_iterable (A) && hb_is_iterable (B))>
  hb_zip_iter_t<hb_iter_type<A>, hb_iter_type<B>>
  چلوونکی () (A&& a, B&& b) ثابت
  { أرجع hb_zip_iter_t<hb_iter_type<A>, hb_iter_type<B>> (hb_iter (a), hb_iter (b)); }
}
HB_FUNCOBJ (hb_zip);

/* hb_concat() */

کينډۍ <ډول_نوم A, ډول_نوم B>
بنية hb_concat_iter_t :
    hb_iter_t<hb_concat_iter_t<A, B>, ډول_نوم A::توکی_t>
{
  hb_concat_iter_t () {}
  hb_concat_iter_t (A& a, B& b) : a (a), b (b) {}
  hb_concat_iter_t (ثابت A& a, ثابت B& b) : a (a), b (b) {}


  تعريفنوع ډول_نوم A::توکی_t __item_t__;
  ساكن constexpr منطقي is_random_access_iterator =
    A::is_random_access_iterator &&
    B::is_random_access_iterator;
  ساكن constexpr منطقي is_sorted_iterator = خطأ;

  __item_t__ __item__ () ثابت
  {
    إذا (!a)
      أرجع *b;
    أرجع *a;
  }

  __item_t__ __item_at__ (بینښه i) ثابت
  {
    بینښه a_len = a.len ();
    إذا (i < a_len)
      أرجع a[i];
    أرجع b[i - a_len];
  }

  منطقي __more__ () ثابت { أرجع منطقي (a) || منطقي (b); }

  بینښه __len__ () ثابت { أرجع a.len () + b.len (); }

  فراغ __next__ ()
  {
    إذا (a)
      ++a;
    وإلا
      ++b;
  }

  فراغ __forward__ (بینښه n)
  {
    إذا (!n) أرجع;
    إذا (!is_random_access_iterator) {
      طالما (n-- && *هذا) {
        (*هذا)++;
      }
      أرجع;
    }

    بینښه a_len = a.len ();
    إذا (n > a_len) {
      n -= a_len;
      a.__forward__ (a_len);
      b.__forward__ (n);
    } وإلا {
      a.__forward__ (n);
    }
  }

  hb_concat_iter_t __end__ () ثابت { أرجع hb_concat_iter_t (a.پای_ټکی_3 (), b.پای_ټکی_3 ()); }
  منطقي چلوونکی != (ثابت hb_concat_iter_t& o) ثابت
  {
    أرجع a != o.a
        || b != o.b;
  }

  خاص:
  A a;
  B b;
};
بنية
{ HB_PARTIALIZE(2);
  کينډۍ <ډول_نوم A, ډول_نوم B,
	    hb_requires (hb_is_iterable (A) && hb_is_iterable (B))>
  hb_concat_iter_t<hb_iter_type<A>, hb_iter_type<B>>
  چلوونکی () (A&& a, B&& b) ثابت
  { أرجع hb_concat_iter_t<hb_iter_type<A>, hb_iter_type<B>> (hb_iter (a), hb_iter (b)); }
}
HB_FUNCOBJ (hb_concat);

/* hb_apply() */

کينډۍ <ډول_نوم Appl>
بنية hb_apply_t
{
  hb_apply_t (Appl a) : a (a) {}

  کينډۍ <ډول_نوم Iter,
	    hb_requires (hb_is_iterator (Iter))>
  فراغ چلوونکی () (Iter it)
  {
    لكل (; it; ++it)
      (فراغ) hb_invoke (a, *it);
  }

  خاص:
  Appl a;
};
بنية
{
  کينډۍ <ډول_نوم Appl> hb_apply_t<Appl>
  چلوونکی () (Appl&& a) ثابت
  { أرجع hb_apply_t<Appl> (a); }

  کينډۍ <ډول_نوم Appl> hb_apply_t<Appl&>
  چلوونکی () (Appl *a) ثابت
  { أرجع hb_apply_t<Appl&> (*a); }
}
HB_FUNCOBJ (hb_apply);

/* hb_range()/hb_iota()/hb_repeat() */

کينډۍ <ډول_نوم T, ډول_نوم S>
بنية hb_range_iter_t :
  hb_iter_t<hb_range_iter_t<T, S>, T>
{
  hb_range_iter_t (T پیل, T پای_ټکی_4, S ګام) : v (پیل), پای_ټکی_4 (پای_ټکی_for (پیل, پای_ټکی_4, ګام)), ګام (ګام) {}

  تعريفنوع T __item_t__;
  ساكن constexpr منطقي is_random_access_iterator = ریښتیا;
  ساكن constexpr منطقي is_sorted_iterator = ریښتیا;
  __item_t__ __item__ () ثابت { أرجع hb_ridentity (v); }
  __item_t__ __item_at__ (بینښه j) ثابت { أرجع v + j * ګام; }
  منطقي __more__ () ثابت { أرجع v != پای_ټکی_4; }
  بینښه __len__ () ثابت { أرجع !ګام ? UINT_MAX : (پای_ټکی_4 - v) / ګام; }
  فراغ __next__ () { v += ګام; }
  فراغ __forward__ (بینښه n) { v += n * ګام; }
  فراغ __prev__ () { v -= ګام; }
  فراغ __rewind__ (بینښه n) { v -= n * ګام; }
  hb_range_iter_t __end__ () ثابت { أرجع hb_range_iter_t (پای_ټکی_4, پای_ټکی_4, ګام); }
  منطقي چلوونکی != (ثابت hb_range_iter_t& o) ثابت
  { أرجع v != o.v; }

  خاص:
  ساكن مضمن T پای_ټکی_for (T پیل, T پای_ټکی_4, S ګام)
  {
    إذا (!ګام)
      أرجع پای_ټکی_4;
    خپلکاری res = (پای_ټکی_4 - پیل) % ګام;
    إذا (!res)
      أرجع پای_ټکی_4;
    پای_ټکی_4 += ګام - res;
    أرجع پای_ټکی_4;
  }

  خاص:
  T v;
  T پای_ټکی_4;
  S ګام;
};
بنية
{
  کينډۍ <ډول_نوم T = بینښه> hb_range_iter_t<T, بینښه>
  چلوونکی () (T پای_ټکی = (بینښه) -1) ثابت
  { أرجع hb_range_iter_t<T, بینښه> (0, پای_ټکی, 1u); }

  کينډۍ <ډول_نوم T, ډول_نوم S = بینښه> hb_range_iter_t<T, S>
  چلوونکی () (T پیل, T پای_ټکی, S ګام = 1u) ثابت
  { أرجع hb_range_iter_t<T, S> (پیل, پای_ټکی, ګام); }
}
HB_FUNCOBJ (hb_range);

کينډۍ <ډول_نوم T, ډول_نوم S>
بنية hb_iota_iter_t :
  hb_iter_with_fallback_t<hb_iota_iter_t<T, S>, T>
{
  hb_iota_iter_t (T پیل, S ګام) : v (پیل), ګام (ګام) {}

  خاص:

  کينډۍ <ډول_نوم S2 = S>
  خپلکاری
  inc (hb_type_بې_اغېزه_عنصر<S2> s, hb_لومړیتوب<1>)
    -> hb_void_t<decltype (hb_invoke (std::forward<hb_type_بې_اغېزه_عنصر<S2>> (s),
                                      hb_declval<T&> ()))>
  { v = hb_invoke (std::forward<hb_type_بې_اغېزه_عنصر<S2>> (s), v); }

  فراغ
  inc (S s, hb_لومړیتوب<0>)
  { v += s; }

  عام:

  تعريفنوع T __item_t__;
  ساكن constexpr منطقي is_random_access_iterator = ریښتیا;
  ساكن constexpr منطقي is_sorted_iterator = ریښتیا;
  __item_t__ __item__ () ثابت { أرجع hb_ridentity (v); }
  منطقي __more__ () ثابت { أرجع ریښتیا; }
  بینښه __len__ () ثابت { أرجع UINT_MAX; }
  فراغ __next__ () { inc (ګام, hb_prioritize); }
  فراغ __prev__ () { v -= ګام; }
  hb_iota_iter_t __end__ () ثابت { أرجع *هذا; }
  منطقي چلوونکی != (ثابت hb_iota_iter_t& o) ثابت { أرجع ریښتیا; }

  خاص:
  T v;
  S ګام;
};
بنية
{
  کينډۍ <ډول_نوم T = بینښه, ډول_نوم S = بینښه> hb_iota_iter_t<T, S>
  چلوونکی () (T پیل = 0u, S ګام = 1u) ثابت
  { أرجع hb_iota_iter_t<T, S> (پیل, ګام); }
}
HB_FUNCOBJ (hb_iota);

کينډۍ <ډول_نوم T>
بنية hb_repeat_iter_t :
  hb_iter_t<hb_repeat_iter_t<T>, T>
{
  hb_repeat_iter_t (T value) : v (value) {}

  تعريفنوع T __item_t__;
  ساكن constexpr منطقي is_random_access_iterator = ریښتیا;
  ساكن constexpr منطقي is_sorted_iterator = ریښتیا;
  __item_t__ __item__ () ثابت { أرجع v; }
  __item_t__ __item_at__ (بینښه j) ثابت { أرجع v; }
  منطقي __more__ () ثابت { أرجع ریښتیا; }
  بینښه __len__ () ثابت { أرجع UINT_MAX; }
  فراغ __next__ () {}
  فراغ __forward__ (بینښه) {}
  فراغ __prev__ () {}
  فراغ __rewind__ (بینښه) {}
  hb_repeat_iter_t __end__ () ثابت { أرجع *هذا; }
  منطقي چلوونکی != (ثابت hb_repeat_iter_t& o) ثابت { أرجع ریښتیا; }

  خاص:
  T v;
};
بنية
{
  کينډۍ <ډول_نوم T> hb_repeat_iter_t<T>
  چلوونکی () (T value) ثابت
  { أرجع hb_repeat_iter_t<T> (value); }
}
HB_FUNCOBJ (hb_repeat);

/* hb_enumerate()/hb_take() */

بنية
{
  کينډۍ <ډول_نوم Iterable,
	    ډول_نوم شاخص = بینښه,
	    hb_requires (hb_is_iterable (Iterable))>
  خپلکاری چلوونکی () (Iterable&& it, شاخص پیل = 0u) ثابت HB_AUTO_RETURN
  ( hb_zip (hb_iota (پیل), it) )
}
HB_FUNCOBJ (hb_enumerate);

بنية
{ HB_PARTIALIZE(2);
  کينډۍ <ډول_نوم Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  خپلکاری چلوونکی () (Iterable&& it, بینښه شمېر) ثابت HB_AUTO_RETURN
  ( hb_zip (hb_range (شمېر), it) | hb_map_retains_ترتیبونه (hb_دوهم) )

  /* Specialization arrays. */

  کينډۍ <ډول_نوم Type> مضمن hb_array_t<Type>
  چلوونکی () (hb_array_t<Type> array, بینښه شمېر) ثابت
  { أرجع array.sub_array (0, شمېر); }

  کينډۍ <ډول_نوم Type> مضمن hb_sorted_array_t<Type>
  چلوونکی () (hb_sorted_array_t<Type> array, بینښه شمېر) ثابت
  { أرجع array.sub_array (0, شمېر); }
}
HB_FUNCOBJ (hb_take);

بنية
{ HB_PARTIALIZE(2);
  کينډۍ <ډول_نوم Iter,
	    hb_requires (hb_is_iterator (Iter))>
  خپلکاری چلوونکی () (Iter it, بینښه شمېر) ثابت HB_AUTO_RETURN
  (
    + hb_iota (it, hb_جمع_کړه (شمېر))
    | hb_map (hb_take (شمېر))
    | hb_take ((hb_len (it) + شمېر - 1) / شمېر)
  )
}
HB_FUNCOBJ (hb_chop);

/* hb_sink() */

کينډۍ <ډول_نوم Sink>
بنية hb_sink_t
{
  hb_sink_t (Sink s) : s (s) {}

  کينډۍ <ډول_نوم Iter,
	    hb_requires (hb_is_iterator (Iter))>
  فراغ چلوونکی () (Iter it)
  {
    لكل (; it; ++it)
      s << *it;
  }

  خاص:
  Sink s;
};
بنية
{
  کينډۍ <ډول_نوم Sink> hb_sink_t<Sink>
  چلوونکی () (Sink&& s) ثابت
  { أرجع hb_sink_t<Sink> (s); }

  کينډۍ <ډول_نوم Sink> hb_sink_t<Sink&>
  چلوونکی () (Sink *s) ثابت
  { أرجع hb_sink_t<Sink&> (*s); }
}
HB_FUNCOBJ (hb_sink);

/* hb-drain: hb_sink to void / blackhole / /dev/null. */

بنية
{
  کينډۍ <ډول_نوم Iter,
	    hb_requires (hb_is_iterator (Iter))>
  فراغ چلوونکی () (Iter it) ثابت
  {
    لكل (; it; ++it)
      (فراغ) *it;
  }
}
HB_FUNCOBJ (hb_drain);

/* hb_unzip(): unzip and sink to two sinks. */

کينډۍ <ډول_نوم Sink1, ډول_نوم Sink2>
بنية hb_unzip_t
{
  hb_unzip_t (Sink1 s1, Sink2 s2) : s1 (s1), s2 (s2) {}

  کينډۍ <ډول_نوم Iter,
	    hb_requires (hb_is_iterator (Iter))>
  فراغ چلوونکی () (Iter it)
  {
    لكل (; it; ++it)
    {
      ثابت خپلکاری &v = *it;
      s1 << v.لومړی;
      s2 << v.second;
    }
  }

  خاص:
  Sink1 s1;
  Sink2 s2;
};
بنية
{
  کينډۍ <ډول_نوم Sink1, ډول_نوم Sink2> hb_unzip_t<Sink1, Sink2>
  چلوونکی () (Sink1&& s1, Sink2&& s2) ثابت
  { أرجع hb_unzip_t<Sink1, Sink2> (s1, s2); }

  کينډۍ <ډول_نوم Sink1, ډول_نوم Sink2> hb_unzip_t<Sink1&, Sink2&>
  چلوونکی () (Sink1 *s1, Sink2 *s2) ثابت
  { أرجع hb_unzip_t<Sink1&, Sink2&> (*s1, *s2); }
}
HB_FUNCOBJ (hb_unzip);


/* hb-all, hb-any, hb-none. */

بنية
{
  کينډۍ <ډول_نوم Iterable,
	    ډول_نوم Pred = decltype ((hb_بې_اغېزه_عنصر)),
	    ډول_نوم Proj = decltype ((hb_بې_اغېزه_عنصر)),
	    hb_requires (hb_is_iterable (Iterable))>
  منطقي چلوونکی () (Iterable&& c,
		    Pred&& p = hb_بې_اغېزه_عنصر,
		    Proj&& f = hb_بې_اغېزه_عنصر) ثابت
  {
    لكل (خپلکاری it = hb_iter (c); it; ++it)
      إذا (!hb_سمون (p, hb_get (f, *it)))
	أرجع خطأ;
    أرجع ریښتیا;
  }
}
HB_FUNCOBJ (hb_all);
بنية
{
  کينډۍ <ډول_نوم Iterable,
	    ډول_نوم Pred = decltype ((hb_بې_اغېزه_عنصر)),
	    ډول_نوم Proj = decltype ((hb_بې_اغېزه_عنصر)),
	    hb_requires (hb_is_iterable (Iterable))>
  منطقي چلوونکی () (Iterable&& c,
		    Pred&& p = hb_بې_اغېزه_عنصر,
		    Proj&& f = hb_بې_اغېزه_عنصر) ثابت
  {
    لكل (خپلکاری it = hb_iter (c); it; ++it)
      إذا (hb_سمون (p, hb_get (f, *it)))
	أرجع ریښتیا;
    أرجع خطأ;
  }
}
HB_FUNCOBJ (hb_any);
بنية
{
  کينډۍ <ډول_نوم Iterable,
	    ډول_نوم Pred = decltype ((hb_بې_اغېزه_عنصر)),
	    ډول_نوم Proj = decltype ((hb_بې_اغېزه_عنصر)),
	    hb_requires (hb_is_iterable (Iterable))>
  منطقي چلوونکی () (Iterable&& c,
		    Pred&& p = hb_بې_اغېزه_عنصر,
		    Proj&& f = hb_بې_اغېزه_عنصر) ثابت
  {
    لكل (خپلکاری it = hb_iter (c); it; ++it)
      إذا (hb_سمون (p, hb_get (f, *it)))
	أرجع خطأ;
    أرجع ریښتیا;
  }
}
HB_FUNCOBJ (hb_none);

/*
 * Algorithms operating on iterators.
 */

کينډۍ <ډول_نوم C, ډول_نوم V,
	  hb_requires (hb_is_iterable (C))>
مضمن فراغ
hb_fill (C&& c, ثابت V &v)
{
  لكل (خپلکاری i = hb_iter (c); i; i++)
    *i = v;
}

کينډۍ <ډول_نوم S, ډول_نوم D>
مضمن فراغ
hb_copy (S&& is, D&& id)
{
  hb_iter (is) | hb_sink (id);
}


#د_که_پای /* HB_ITER_HH */

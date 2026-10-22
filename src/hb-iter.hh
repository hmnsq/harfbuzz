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

#اگر_تعریف_نشده HB_ITER_HH
#تعریف_کن HB_ITER_HH

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
قالب <نام_نوع iter_t, نام_نوع قلم_3 = نام_نوع iter_t::__item_t__>
بنية hb_iter_t
{
  تعريفنوع قلم_3 قلم_t;
  constexpr بیعلامت get_قلم_اندازه () ثابت { أرجع hb_static_اندازه (قلم_3); }
  ساكن constexpr منطقي is_iterator = درست;
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
  iter_t عملگر + () ثابت { أرجع *thiz(); }
  iter_t _begin () ثابت { أرجع *thiz(); }
  iter_t begin () ثابت { أرجع _begin (); }
  iter_t انتها_3 () ثابت { أرجع thiz()->__end__ (); }
  iter_t انتها () ثابت { أرجع انتها_3 (); }
  صریح عملگر منطقي () ثابت { أرجع thiz()->__more__ (); }
  بیعلامت len () ثابت { أرجع thiz()->__len__ (); }
  /* The following can only be enabled if item_t is reference type.  Otherwise
   * it will be returning pointer to temporary rvalue. */
  قالب <نام_نوع T = قلم_t,
	    hb_enable_if (std::is_reference<T>::value)>
  hb_remove_reference<قلم_t>* عملگر -> () ثابت { أرجع std::addressof (**thiz()); }
  قلم_t عملگر * () ثابت { أرجع thiz()->__item__ (); }
  قلم_t عملگر * () { أرجع thiz()->__item__ (); }
  قلم_t عملگر [] (بیعلامت i) ثابت { أرجع thiz()->__item_at__ (i); }
  قلم_t عملگر [] (بیعلامت i) { أرجع thiz()->__item_at__ (i); }
  iter_t& عملگر += (بیعلامت شمار) &  { thiz()->__forward__ (شمار); أرجع *thiz(); }
  iter_t  عملگر += (بیعلامت شمار) && { thiz()->__forward__ (شمار); أرجع *thiz(); }
  iter_t& عملگر ++ () &  { thiz()->__next__ (); أرجع *thiz(); }
  iter_t  عملگر ++ () && { thiz()->__next__ (); أرجع *thiz(); }
  iter_t& عملگر -= (بیعلامت شمار) &  { thiz()->__rewind__ (شمار); أرجع *thiz(); }
  iter_t  عملگر -= (بیعلامت شمار) && { thiz()->__rewind__ (شمار); أرجع *thiz(); }
  iter_t& عملگر -- () &  { thiz()->__prev__ (); أرجع *thiz(); }
  iter_t  عملگر -- () && { thiz()->__prev__ (); أرجع *thiz(); }
  iter_t عملگر + (بیعلامت شمار) ثابت { خودکار c = thiz()->iter (); c += شمار; أرجع c; }
  دوست iter_t عملگر + (بیعلامت شمار, ثابت iter_t &it) { أرجع it + شمار; }
  iter_t عملگر ++ (صحيح) { iter_t c (*thiz()); ++*thiz(); أرجع c; }
  iter_t عملگر - (بیعلامت شمار) ثابت { خودکار c = thiz()->iter (); c -= شمار; أرجع c; }
  iter_t عملگر -- (صحيح) { iter_t c (*thiz()); --*thiz(); أرجع c; }
  قالب <نام_نوع T>
  iter_t& عملگر >> (T &v) &  { v = **thiz(); ++*thiz(); أرجع *thiz(); }
  قالب <نام_نوع T>
  iter_t  عملگر >> (T &v) && { v = **thiz(); ++*thiz(); أرجع *thiz(); }
  قالب <نام_نوع T>
  iter_t& عملگر << (ثابت T v) &  { **thiz() = v; ++*thiz(); أرجع *thiz(); }
  قالب <نام_نوع T>
  iter_t  عملگر << (ثابت T v) && { **thiz() = v; ++*thiz(); أرجع *thiz(); }

  محمي:
  hb_iter_t () = افتراضي;
  hb_iter_t (ثابت hb_iter_t &o HB_استفادهنشده) = افتراضي;
  hb_iter_t (hb_iter_t &&o HB_استفادهنشده) = افتراضي;
  hb_iter_t& عملگر = (ثابت hb_iter_t &o HB_استفادهنشده) = افتراضي;
  hb_iter_t& عملگر = (hb_iter_t &&o HB_استفادهنشده) = افتراضي;
};

#تعریف_کن HB_ITER_USING(Name) \
  استفاده قلم_t = نام_نوع Name::قلم_t; \
  استفاده Name::_begin; \
  استفاده Name::begin; \
  استفاده Name::انتها_3; \
  استفاده Name::انتها; \
  استفاده Name::get_قلم_اندازه; \
  استفاده Name::is_iterator; \
  استفاده Name::iter; \
  استفاده Name::عملگر منطقي; \
  استفاده Name::len; \
  استفاده Name::عملگر ->; \
  استفاده Name::عملگر *; \
  استفاده Name::عملگر []; \
  استفاده Name::عملگر +=; \
  استفاده Name::عملگر ++; \
  استفاده Name::عملگر -=; \
  استفاده Name::عملگر --; \
  استفاده Name::عملگر +; \
  استفاده Name::عملگر -; \
  استفاده Name::عملگر >>; \
  استفاده Name::عملگر <<; \
  static_assert (درست, "")

/* Returns iterator / item type of a type. */
قالب <نام_نوع Iterable>
استفاده hb_iter_type = decltype (hb_deref (hb_declval (Iterable)).iter ());
قالب <نام_نوع Iterable>
استفاده hb_قلم_type = decltype (*hb_deref (hb_declval (Iterable)).iter ());


قالب <نام_نوع> بنية hb_array_t;
قالب <نام_نوع> بنية hb_sorted_array_t;

بنية
{
  قالب <نام_نوع T> hb_iter_type<T>
  عملگر () (T&& c) ثابت
  { أرجع hb_deref (std::forward<T> (c)).iter (); }

  /* Specialization for C arrays. */

  قالب <نام_نوع Type> مضمن hb_array_t<Type>
  عملگر () (Type *array, بیعلامت صحيح درازی) ثابت
  { أرجع hb_array_t<Type> (array, درازی); }

  قالب <نام_نوع Type, بیعلامت صحيح درازی> hb_array_t<Type>
  عملگر () (Type (&array)[درازی]) ثابت
  { أرجع hb_array_t<Type> (array, درازی); }

}
HB_FUNCOBJ (hb_iter);
بنية
{
  قالب <نام_نوع T> خودکار
  impl (T&& c, hb_اولویت<1>) ثابت HB_RETURN (بیعلامت, c.len ())

  قالب <نام_نوع T> خودکار
  impl (T&& c, hb_اولویت<0>) ثابت HB_RETURN (بیعلامت, c.len)

  عام:

  قالب <نام_نوع T> خودکار
  عملگر () (T&& c) ثابت HB_RETURN (بیعلامت, impl (std::forward<T> (c), hb_prioritize))
}
HB_FUNCOBJ (hb_len);

/* Mixin to fill in what the subclass doesn't provide. */
قالب <نام_نوع iter_t, نام_نوع قلم_t = نام_نوع iter_t::__item_t__>
بنية hb_iter_fallback_mixin_t
{
  خاص:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ثابت iter_t* thiz () ثابت { أرجع static_cast<ثابت iter_t *> (هذا); }
	iter_t* thiz ()       { أرجع static_cast<      iter_t *> (هذا); }
  عام:

  /* Access: Implement __item__(), or __item_at__() if random-access. */
  قلم_t __item__ () ثابت { أرجع (*thiz())[0]; }
  قلم_t __item_at__ (بیعلامت i) ثابت { أرجع *(*thiz() + i); }

  /* Termination: Implement __more__(), or __len__() if random-access. */
  منطقي __more__ () ثابت { أرجع منطقي (thiz()->len ()); }
  بیعلامت __len__ () ثابت
  { iter_t c (*thiz()); بیعلامت l = 0; طالما (c) { c++; l++; } أرجع l; }

  /* Advancing: Implement __next__(), or __forward__() if random-access. */
  فراغ __next__ () { *thiz() += 1; }
  فراغ __forward__ (بیعلامت n) { طالما (*thiz() && n--) ++*thiz(); }

  /* Rewinding: Implement __prev__() or __rewind__() if bidirectional. */
  فراغ __prev__ () { *thiz() -= 1; }
  فراغ __rewind__ (بیعلامت n) { طالما (*thiz() && n--) --*thiz(); }

  /* Range-based for: Implement __end__() if can be done faster,
   * and operator!=. */
  iter_t __end__ () ثابت
  {
    إذا (thiz()->is_random_access_iterator)
      أرجع *thiz() + thiz()->len ();
    /* Above expression loops twice. Following loops once. */
    خودکار it = *thiz();
    طالما (it) ++it;
    أرجع it;
  }

  محمي:
  hb_iter_fallback_mixin_t () = افتراضي;
  hb_iter_fallback_mixin_t (ثابت hb_iter_fallback_mixin_t &o HB_استفادهنشده) = افتراضي;
  hb_iter_fallback_mixin_t (hb_iter_fallback_mixin_t &&o HB_استفادهنشده) = افتراضي;
  hb_iter_fallback_mixin_t& عملگر = (ثابت hb_iter_fallback_mixin_t &o HB_استفادهنشده) = افتراضي;
  hb_iter_fallback_mixin_t& عملگر = (hb_iter_fallback_mixin_t &&o HB_استفادهنشده) = افتراضي;
};

قالب <نام_نوع iter_t, نام_نوع قلم_t = نام_نوع iter_t::__item_t__>
بنية hb_iter_with_fallback_t :
  hb_iter_t<iter_t, قلم_t>,
  hb_iter_fallback_mixin_t<iter_t, قلم_t>
{
  محمي:
  hb_iter_with_fallback_t () = افتراضي;
  hb_iter_with_fallback_t (ثابت hb_iter_with_fallback_t &o HB_استفادهنشده) = افتراضي;
  hb_iter_with_fallback_t (hb_iter_with_fallback_t &&o HB_استفادهنشده) = افتراضي;
  hb_iter_with_fallback_t& عملگر = (ثابت hb_iter_with_fallback_t &o HB_استفادهنشده) = افتراضي;
  hb_iter_with_fallback_t& عملگر = (hb_iter_with_fallback_t &&o HB_استفادهنشده) = افتراضي;
};

/*
 * Meta-programming predicates.
 */

/* hb_is_iterator() / hb_is_iterator_of() */

قالب<نام_نوع Iter, نام_نوع قلم_3>
بنية hb_is_iterator_از
{
  قالب <نام_نوع Item2 = قلم_3>
  ساكن hb_true_type impl (hb_اولویت<2>, hb_iter_t<Iter, hb_type_عنصر_خنثی<Item2>> *);
  ساكن hb_false_type impl (hb_اولویت<0>, ثابت فراغ *);

  عام:
  ساكن constexpr منطقي value = decltype (impl (hb_prioritize, hb_declval (Iter*)))::value;
};
#تعریف_کن hb_is_iterator_از(Iter, قلم_3) hb_is_iterator_از<Iter, قلم_3>::value
#تعریف_کن hb_is_iterator(Iter) hb_is_iterator_از (Iter, نام_نوع Iter::قلم_t)
#تعریف_کن hb_is_sorted_iterator_از(Iter, قلم_3) (hb_is_iterator_از<Iter, قلم_3>::value && Iter::is_sorted_iterator)
#تعریف_کن hb_is_sorted_iterator(Iter) hb_is_sorted_iterator_از (Iter, نام_نوع Iter::قلم_t)

/* hb_is_iterable() */

قالب <نام_نوع T>
بنية hb_is_iterable
{
  خاص:

  قالب <نام_نوع U>
  ساكن خودکار impl (hb_اولویت<1>) -> decltype (hb_declval (U).iter (), hb_true_type ());

  قالب <نام_نوع>
  ساكن hb_false_type impl (hb_اولویت<0>);

  عام:
  ساكن constexpr منطقي value = decltype (impl<T> (hb_prioritize))::value;
};
#تعریف_کن hb_is_iterable(Iterable) hb_is_iterable<Iterable>::value

/* hb_is_source_of() / hb_is_sink_of() */

قالب<نام_نوع Iter, نام_نوع قلم_3>
بنية hb_is_منبع_از
{
  خاص:
  قالب <نام_نوع Iter2 = Iter,
	    hb_enable_if (hb_is_convertible (نام_نوع Iter2::قلم_t, hb_جمع_کن_lvalue_reference<ثابت قلم_3>))>
  ساكن hb_true_type impl (hb_اولویت<2>);
  قالب <نام_نوع Iter2 = Iter>
  ساكن خودکار impl (hb_اولویت<1>) -> decltype (hb_declval (Iter2) >> hb_declval (قلم_3 &), hb_true_type ());
  ساكن hb_false_type impl (hb_اولویت<0>);

  عام:
  ساكن constexpr منطقي value = decltype (impl (hb_prioritize))::value;
};
#تعریف_کن hb_is_منبع_از(Iter, قلم_3) hb_is_منبع_از<Iter, قلم_3>::value

قالب<نام_نوع Iter, نام_نوع قلم_3>
بنية hb_is_sink_از
{
  خاص:
  قالب <نام_نوع Iter2 = Iter,
	    hb_enable_if (hb_is_convertible (نام_نوع Iter2::قلم_t, hb_جمع_کن_lvalue_reference<قلم_3>))>
  ساكن hb_true_type impl (hb_اولویت<2>);
  قالب <نام_نوع Iter2 = Iter>
  ساكن خودکار impl (hb_اولویت<1>) -> decltype (hb_declval (Iter2) << hb_declval (قلم_3), hb_true_type ());
  ساكن hb_false_type impl (hb_اولویت<0>);

  عام:
  ساكن constexpr منطقي value = decltype (impl (hb_prioritize))::value;
};
#تعریف_کن hb_is_sink_از(Iter, قلم_3) hb_is_sink_از<Iter, قلم_3>::value

/* This is commonly used, so define: */
#تعریف_کن hb_is_sorted_منبع_از(Iter, قلم_3) \
	(hb_is_منبع_از(Iter, قلم_3) && Iter::is_sorted_iterator)


بنية
{
  قالب <نام_نوع Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  بیعلامت عملگر () (ثابت Iterable &_) ثابت { أرجع hb_len (hb_iter (_)); }

  بیعلامت عملگر () (بیعلامت _) ثابت { أرجع _; }
}
HB_FUNCOBJ (hb_len_از);

/* Range-based 'for' for iterables. */

قالب <نام_نوع Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ساكن مضمن خودکار begin (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).begin ())

قالب <نام_نوع Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ساكن مضمن خودکار انتها (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).انتها ())

/* begin()/end() are NOT looked up non-ADL.  So each namespace must declare them.
 * Do it for namespace OT. */
نطاق OT {

قالب <نام_نوع Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ساكن مضمن خودکار begin (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).begin ())

قالب <نام_نوع Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ساكن مضمن خودکار انتها (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).انتها ())

}


/*
 * Adaptors, combiners, etc.
 */

قالب <نام_نوع Lhs, نام_نوع Rhs,
	  hb_requires (hb_is_iterator (Lhs))>
ساكن مضمن خودکار
عملگر | (Lhs&& lhs, Rhs&& rhs) HB_AUTO_RETURN (std::forward<Rhs> (rhs) (std::forward<Lhs> (lhs)))

/* hb_map(), hb_filter(), hb_reduce() */

تعداد  صنف hb_function_sortedness_t {
  NOT_SORTED,
  RETAINS_ترتیبدهی,
  SORTED,
};

قالب <نام_نوع Iter, نام_نوع Proj, hb_function_sortedness_t Sorted,
	 hb_requires (hb_is_iterator (Iter))>
بنية hb_map_iter_t :
  hb_iter_t<hb_map_iter_t<Iter, Proj, Sorted>,
	    decltype (hb_get (hb_declval (Proj), *hb_declval (Iter)))>
{
  hb_map_iter_t (ثابت Iter& it, Proj f_) : it (it), f (f_) {}

  تعريفنوع decltype (hb_get (hb_declval (Proj), *hb_declval (Iter))) __item_t__;
  ساكن constexpr منطقي is_random_access_iterator = Iter::is_random_access_iterator;
  ساكن constexpr منطقي is_sorted_iterator =
    Sorted == hb_function_sortedness_t::SORTED ? درست :
    Sorted == hb_function_sortedness_t::RETAINS_ترتیبدهی ? Iter::is_sorted_iterator :
    خطأ;
  __item_t__ __item__ () ثابت { أرجع hb_get (f.get (), *it); }
  __item_t__ __item_at__ (بیعلامت i) ثابت { أرجع hb_get (f.get (), it[i]); }
  منطقي __more__ () ثابت { أرجع منطقي (it); }
  بیعلامت __len__ () ثابت { أرجع it.len (); }
  فراغ __next__ () { ++it; }
  فراغ __forward__ (بیعلامت n) { it += n; }
  فراغ __prev__ () { --it; }
  فراغ __rewind__ (بیعلامت n) { it -= n; }
  hb_map_iter_t __end__ () ثابت { أرجع hb_map_iter_t (it.انتها_3 (), f); }
  منطقي عملگر != (ثابت hb_map_iter_t& o) ثابت
  { أرجع it != o.it; }

  خاص:
  Iter it;
  تغییرپذیر hb_reference_wrapper<Proj> f;
};

قالب <نام_نوع Proj, hb_function_sortedness_t Sorted>
بنية hb_map_iter_factory_t
{
  hb_map_iter_factory_t (Proj f) : f (f) {}

  قالب <نام_نوع Iter,
	    hb_requires (hb_is_iterator (Iter))>
  hb_map_iter_t<Iter, Proj, Sorted>
  عملگر () (Iter it)
  { أرجع hb_map_iter_t<Iter, Proj, Sorted> (it, f); }

  خاص:
  Proj f;
};
بنية
{
  قالب <نام_نوع Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::NOT_SORTED>
  عملگر () (Proj&& f) ثابت
  { أرجع hb_map_iter_factory_t<Proj, hb_function_sortedness_t::NOT_SORTED> (f); }
}
HB_FUNCOBJ (hb_map);
بنية
{
  قالب <نام_نوع Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::RETAINS_ترتیبدهی>
  عملگر () (Proj&& f) ثابت
  { أرجع hb_map_iter_factory_t<Proj, hb_function_sortedness_t::RETAINS_ترتیبدهی> (f); }
}
HB_FUNCOBJ (hb_map_retains_ترتیبدهی);
بنية
{
  قالب <نام_نوع Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::SORTED>
  عملگر () (Proj&& f) ثابت
  { أرجع hb_map_iter_factory_t<Proj, hb_function_sortedness_t::SORTED> (f); }
}
HB_FUNCOBJ (hb_map_sorted);

قالب <نام_نوع Iter, نام_نوع Pred, نام_نوع Proj,
	 hb_requires (hb_is_iterator (Iter))>
بنية hb_filter_iter_t :
  hb_iter_with_fallback_t<hb_filter_iter_t<Iter, Pred, Proj>,
			  نام_نوع Iter::قلم_t>
{
  hb_filter_iter_t (ثابت Iter& it_, Pred p_, Proj f_) : it (it_), p (p_), f (f_)
  { طالما (it && !hb_has (p.get (), hb_get (f.get (), *it))) ++it; }

  تعريفنوع نام_نوع Iter::قلم_t __item_t__;
  ساكن constexpr منطقي is_sorted_iterator = Iter::is_sorted_iterator;
  __item_t__ __item__ () ثابت { أرجع *it; }
  منطقي __more__ () ثابت { أرجع منطقي (it); }
  فراغ __next__ () { افعل ++it; طالما (it && !hb_has (p.get (), hb_get (f.get (), *it))); }
  فراغ __prev__ ()
  {
    بیعلامت شمار = 0;
    افعل
    {
      Iter old = it;
      --it;
      إذا (!(it != old))
      {
	it += شمار;
	أرجع;
      }
      شمار++;
    }
    طالما (!hb_has (p.get (), hb_get (f.get (), *it)));
  }
  hb_filter_iter_t __end__ () ثابت { أرجع hb_filter_iter_t (it.انتها_3 (), p, f); }
  منطقي عملگر != (ثابت hb_filter_iter_t& o) ثابت
  { أرجع it != o.it; }

  خاص:
  Iter it;
  تغییرپذیر hb_reference_wrapper<Pred> p;
  تغییرپذیر hb_reference_wrapper<Proj> f;
};
قالب <نام_نوع Pred, نام_نوع Proj>
بنية hb_filter_iter_factory_t
{
  hb_filter_iter_factory_t (Pred p, Proj f) : p (p), f (f) {}

  قالب <نام_نوع Iter,
	    hb_requires (hb_is_iterator (Iter))>
  hb_filter_iter_t<Iter, Pred, Proj>
  عملگر () (Iter it)
  { أرجع hb_filter_iter_t<Iter, Pred, Proj> (it, p, f); }

  خاص:
  Pred p;
  Proj f;
};
بنية
{
  قالب <نام_نوع Pred = decltype ((hb_عنصر_خنثی)),
	    نام_نوع Proj = decltype ((hb_عنصر_خنثی))>
  hb_filter_iter_factory_t<Pred, Proj>
  عملگر () (Pred&& p = hb_عنصر_خنثی, Proj&& f = hb_عنصر_خنثی) ثابت
  { أرجع hb_filter_iter_factory_t<Pred, Proj> (p, f); }
}
HB_FUNCOBJ (hb_filter);

قالب <نام_نوع Redu, نام_نوع InitT>
بنية hb_reduce_t
{
  hb_reduce_t (Redu r, InitT init_مقدار) : r (r), init_مقدار (init_مقدار) {}

  قالب <نام_نوع Iter,
	    hb_requires (hb_is_iterator (Iter)),
	    نام_نوع AccuT = hb_decay<decltype (hb_declval (Redu) (hb_declval (InitT), hb_declval (نام_نوع Iter::قلم_t)))>>
  AccuT
  عملگر () (Iter it)
  {
    AccuT value = init_مقدار;
    لكل (; it; ++it)
      value = r (value, *it);
    أرجع value;
  }

  خاص:
  Redu r;
  InitT init_مقدار;
};
بنية
{
  قالب <نام_نوع Redu, نام_نوع InitT>
  hb_reduce_t<Redu, InitT>
  عملگر () (Redu&& r, InitT init_مقدار) ثابت
  { أرجع hb_reduce_t<Redu, InitT> (r, init_مقدار); }
}
HB_FUNCOBJ (hb_reduce);


/* hb_zip() */

قالب <نام_نوع A, نام_نوع B>
بنية hb_zip_iter_t :
  hb_iter_t<hb_zip_iter_t<A, B>,
	    hb_pair_t<نام_نوع A::قلم_t, نام_نوع B::قلم_t>>
{
  hb_zip_iter_t () {}
  hb_zip_iter_t (ثابت A& a, ثابت B& b) : a (a), b (b) {}

  تعريفنوع hb_pair_t<نام_نوع A::قلم_t, نام_نوع B::قلم_t> __item_t__;
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
  __item_t__ __item_at__ (بیعلامت i) ثابت { أرجع __item_t__ (a[i], b[i]); }
  منطقي __more__ () ثابت { أرجع منطقي (a) && منطقي (b); }
  بیعلامت __len__ () ثابت { أرجع hb_min (a.len (), b.len ()); }
  فراغ __next__ () { ++a; ++b; }
  فراغ __forward__ (بیعلامت n) { a += n; b += n; }
  فراغ __prev__ () { --a; --b; }
  فراغ __rewind__ (بیعلامت n) { a -= n; b -= n; }
  hb_zip_iter_t __end__ () ثابت { أرجع hb_zip_iter_t (a.انتها_3 (), b.انتها_3 ()); }
  /* Note, we should stop if ANY of the iters reaches end.  As such two compare
   * unequal if both items are unequal, NOT if either is unequal. */
  منطقي عملگر != (ثابت hb_zip_iter_t& o) ثابت
  { أرجع a != o.a && b != o.b; }

  خاص:
  A a;
  B b;
};
بنية
{ HB_PARTIALIZE(2);
  قالب <نام_نوع A, نام_نوع B,
	    hb_requires (hb_is_iterable (A) && hb_is_iterable (B))>
  hb_zip_iter_t<hb_iter_type<A>, hb_iter_type<B>>
  عملگر () (A&& a, B&& b) ثابت
  { أرجع hb_zip_iter_t<hb_iter_type<A>, hb_iter_type<B>> (hb_iter (a), hb_iter (b)); }
}
HB_FUNCOBJ (hb_zip);

/* hb_concat() */

قالب <نام_نوع A, نام_نوع B>
بنية hb_concat_iter_t :
    hb_iter_t<hb_concat_iter_t<A, B>, نام_نوع A::قلم_t>
{
  hb_concat_iter_t () {}
  hb_concat_iter_t (A& a, B& b) : a (a), b (b) {}
  hb_concat_iter_t (ثابت A& a, ثابت B& b) : a (a), b (b) {}


  تعريفنوع نام_نوع A::قلم_t __item_t__;
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

  __item_t__ __item_at__ (بیعلامت i) ثابت
  {
    بیعلامت a_len = a.len ();
    إذا (i < a_len)
      أرجع a[i];
    أرجع b[i - a_len];
  }

  منطقي __more__ () ثابت { أرجع منطقي (a) || منطقي (b); }

  بیعلامت __len__ () ثابت { أرجع a.len () + b.len (); }

  فراغ __next__ ()
  {
    إذا (a)
      ++a;
    وإلا
      ++b;
  }

  فراغ __forward__ (بیعلامت n)
  {
    إذا (!n) أرجع;
    إذا (!is_random_access_iterator) {
      طالما (n-- && *هذا) {
        (*هذا)++;
      }
      أرجع;
    }

    بیعلامت a_len = a.len ();
    إذا (n > a_len) {
      n -= a_len;
      a.__forward__ (a_len);
      b.__forward__ (n);
    } وإلا {
      a.__forward__ (n);
    }
  }

  hb_concat_iter_t __end__ () ثابت { أرجع hb_concat_iter_t (a.انتها_3 (), b.انتها_3 ()); }
  منطقي عملگر != (ثابت hb_concat_iter_t& o) ثابت
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
  قالب <نام_نوع A, نام_نوع B,
	    hb_requires (hb_is_iterable (A) && hb_is_iterable (B))>
  hb_concat_iter_t<hb_iter_type<A>, hb_iter_type<B>>
  عملگر () (A&& a, B&& b) ثابت
  { أرجع hb_concat_iter_t<hb_iter_type<A>, hb_iter_type<B>> (hb_iter (a), hb_iter (b)); }
}
HB_FUNCOBJ (hb_concat);

/* hb_apply() */

قالب <نام_نوع Appl>
بنية hb_apply_t
{
  hb_apply_t (Appl a) : a (a) {}

  قالب <نام_نوع Iter,
	    hb_requires (hb_is_iterator (Iter))>
  فراغ عملگر () (Iter it)
  {
    لكل (; it; ++it)
      (فراغ) hb_invoke (a, *it);
  }

  خاص:
  Appl a;
};
بنية
{
  قالب <نام_نوع Appl> hb_apply_t<Appl>
  عملگر () (Appl&& a) ثابت
  { أرجع hb_apply_t<Appl> (a); }

  قالب <نام_نوع Appl> hb_apply_t<Appl&>
  عملگر () (Appl *a) ثابت
  { أرجع hb_apply_t<Appl&> (*a); }
}
HB_FUNCOBJ (hb_apply);

/* hb_range()/hb_iota()/hb_repeat() */

قالب <نام_نوع T, نام_نوع S>
بنية hb_range_iter_t :
  hb_iter_t<hb_range_iter_t<T, S>, T>
{
  hb_range_iter_t (T شروع, T انتها_4, S گام) : v (شروع), انتها_4 (انتها_for (شروع, انتها_4, گام)), گام (گام) {}

  تعريفنوع T __item_t__;
  ساكن constexpr منطقي is_random_access_iterator = درست;
  ساكن constexpr منطقي is_sorted_iterator = درست;
  __item_t__ __item__ () ثابت { أرجع hb_ridentity (v); }
  __item_t__ __item_at__ (بیعلامت j) ثابت { أرجع v + j * گام; }
  منطقي __more__ () ثابت { أرجع v != انتها_4; }
  بیعلامت __len__ () ثابت { أرجع !گام ? UINT_MAX : (انتها_4 - v) / گام; }
  فراغ __next__ () { v += گام; }
  فراغ __forward__ (بیعلامت n) { v += n * گام; }
  فراغ __prev__ () { v -= گام; }
  فراغ __rewind__ (بیعلامت n) { v -= n * گام; }
  hb_range_iter_t __end__ () ثابت { أرجع hb_range_iter_t (انتها_4, انتها_4, گام); }
  منطقي عملگر != (ثابت hb_range_iter_t& o) ثابت
  { أرجع v != o.v; }

  خاص:
  ساكن مضمن T انتها_for (T شروع, T انتها_4, S گام)
  {
    إذا (!گام)
      أرجع انتها_4;
    خودکار res = (انتها_4 - شروع) % گام;
    إذا (!res)
      أرجع انتها_4;
    انتها_4 += گام - res;
    أرجع انتها_4;
  }

  خاص:
  T v;
  T انتها_4;
  S گام;
};
بنية
{
  قالب <نام_نوع T = بیعلامت> hb_range_iter_t<T, بیعلامت>
  عملگر () (T انتها = (بیعلامت) -1) ثابت
  { أرجع hb_range_iter_t<T, بیعلامت> (0, انتها, 1u); }

  قالب <نام_نوع T, نام_نوع S = بیعلامت> hb_range_iter_t<T, S>
  عملگر () (T شروع, T انتها, S گام = 1u) ثابت
  { أرجع hb_range_iter_t<T, S> (شروع, انتها, گام); }
}
HB_FUNCOBJ (hb_range);

قالب <نام_نوع T, نام_نوع S>
بنية hb_iota_iter_t :
  hb_iter_with_fallback_t<hb_iota_iter_t<T, S>, T>
{
  hb_iota_iter_t (T شروع, S گام) : v (شروع), گام (گام) {}

  خاص:

  قالب <نام_نوع S2 = S>
  خودکار
  inc (hb_type_عنصر_خنثی<S2> s, hb_اولویت<1>)
    -> hb_void_t<decltype (hb_invoke (std::forward<hb_type_عنصر_خنثی<S2>> (s),
                                      hb_declval<T&> ()))>
  { v = hb_invoke (std::forward<hb_type_عنصر_خنثی<S2>> (s), v); }

  فراغ
  inc (S s, hb_اولویت<0>)
  { v += s; }

  عام:

  تعريفنوع T __item_t__;
  ساكن constexpr منطقي is_random_access_iterator = درست;
  ساكن constexpr منطقي is_sorted_iterator = درست;
  __item_t__ __item__ () ثابت { أرجع hb_ridentity (v); }
  منطقي __more__ () ثابت { أرجع درست; }
  بیعلامت __len__ () ثابت { أرجع UINT_MAX; }
  فراغ __next__ () { inc (گام, hb_prioritize); }
  فراغ __prev__ () { v -= گام; }
  hb_iota_iter_t __end__ () ثابت { أرجع *هذا; }
  منطقي عملگر != (ثابت hb_iota_iter_t& o) ثابت { أرجع درست; }

  خاص:
  T v;
  S گام;
};
بنية
{
  قالب <نام_نوع T = بیعلامت, نام_نوع S = بیعلامت> hb_iota_iter_t<T, S>
  عملگر () (T شروع = 0u, S گام = 1u) ثابت
  { أرجع hb_iota_iter_t<T, S> (شروع, گام); }
}
HB_FUNCOBJ (hb_iota);

قالب <نام_نوع T>
بنية hb_repeat_iter_t :
  hb_iter_t<hb_repeat_iter_t<T>, T>
{
  hb_repeat_iter_t (T value) : v (value) {}

  تعريفنوع T __item_t__;
  ساكن constexpr منطقي is_random_access_iterator = درست;
  ساكن constexpr منطقي is_sorted_iterator = درست;
  __item_t__ __item__ () ثابت { أرجع v; }
  __item_t__ __item_at__ (بیعلامت j) ثابت { أرجع v; }
  منطقي __more__ () ثابت { أرجع درست; }
  بیعلامت __len__ () ثابت { أرجع UINT_MAX; }
  فراغ __next__ () {}
  فراغ __forward__ (بیعلامت) {}
  فراغ __prev__ () {}
  فراغ __rewind__ (بیعلامت) {}
  hb_repeat_iter_t __end__ () ثابت { أرجع *هذا; }
  منطقي عملگر != (ثابت hb_repeat_iter_t& o) ثابت { أرجع درست; }

  خاص:
  T v;
};
بنية
{
  قالب <نام_نوع T> hb_repeat_iter_t<T>
  عملگر () (T value) ثابت
  { أرجع hb_repeat_iter_t<T> (value); }
}
HB_FUNCOBJ (hb_repeat);

/* hb_enumerate()/hb_take() */

بنية
{
  قالب <نام_نوع Iterable,
	    نام_نوع شاخص = بیعلامت,
	    hb_requires (hb_is_iterable (Iterable))>
  خودکار عملگر () (Iterable&& it, شاخص شروع = 0u) ثابت HB_AUTO_RETURN
  ( hb_zip (hb_iota (شروع), it) )
}
HB_FUNCOBJ (hb_enumerate);

بنية
{ HB_PARTIALIZE(2);
  قالب <نام_نوع Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  خودکار عملگر () (Iterable&& it, بیعلامت شمار) ثابت HB_AUTO_RETURN
  ( hb_zip (hb_range (شمار), it) | hb_map_retains_ترتیبدهی (hb_دومی) )

  /* Specialization arrays. */

  قالب <نام_نوع Type> مضمن hb_array_t<Type>
  عملگر () (hb_array_t<Type> array, بیعلامت شمار) ثابت
  { أرجع array.sub_array (0, شمار); }

  قالب <نام_نوع Type> مضمن hb_sorted_array_t<Type>
  عملگر () (hb_sorted_array_t<Type> array, بیعلامت شمار) ثابت
  { أرجع array.sub_array (0, شمار); }
}
HB_FUNCOBJ (hb_take);

بنية
{ HB_PARTIALIZE(2);
  قالب <نام_نوع Iter,
	    hb_requires (hb_is_iterator (Iter))>
  خودکار عملگر () (Iter it, بیعلامت شمار) ثابت HB_AUTO_RETURN
  (
    + hb_iota (it, hb_جمع_کن (شمار))
    | hb_map (hb_take (شمار))
    | hb_take ((hb_len (it) + شمار - 1) / شمار)
  )
}
HB_FUNCOBJ (hb_chop);

/* hb_sink() */

قالب <نام_نوع Sink>
بنية hb_sink_t
{
  hb_sink_t (Sink s) : s (s) {}

  قالب <نام_نوع Iter,
	    hb_requires (hb_is_iterator (Iter))>
  فراغ عملگر () (Iter it)
  {
    لكل (; it; ++it)
      s << *it;
  }

  خاص:
  Sink s;
};
بنية
{
  قالب <نام_نوع Sink> hb_sink_t<Sink>
  عملگر () (Sink&& s) ثابت
  { أرجع hb_sink_t<Sink> (s); }

  قالب <نام_نوع Sink> hb_sink_t<Sink&>
  عملگر () (Sink *s) ثابت
  { أرجع hb_sink_t<Sink&> (*s); }
}
HB_FUNCOBJ (hb_sink);

/* hb-drain: hb_sink to void / blackhole / /dev/null. */

بنية
{
  قالب <نام_نوع Iter,
	    hb_requires (hb_is_iterator (Iter))>
  فراغ عملگر () (Iter it) ثابت
  {
    لكل (; it; ++it)
      (فراغ) *it;
  }
}
HB_FUNCOBJ (hb_drain);

/* hb_unzip(): unzip and sink to two sinks. */

قالب <نام_نوع Sink1, نام_نوع Sink2>
بنية hb_unzip_t
{
  hb_unzip_t (Sink1 s1, Sink2 s2) : s1 (s1), s2 (s2) {}

  قالب <نام_نوع Iter,
	    hb_requires (hb_is_iterator (Iter))>
  فراغ عملگر () (Iter it)
  {
    لكل (; it; ++it)
    {
      ثابت خودکار &v = *it;
      s1 << v.اولی;
      s2 << v.second;
    }
  }

  خاص:
  Sink1 s1;
  Sink2 s2;
};
بنية
{
  قالب <نام_نوع Sink1, نام_نوع Sink2> hb_unzip_t<Sink1, Sink2>
  عملگر () (Sink1&& s1, Sink2&& s2) ثابت
  { أرجع hb_unzip_t<Sink1, Sink2> (s1, s2); }

  قالب <نام_نوع Sink1, نام_نوع Sink2> hb_unzip_t<Sink1&, Sink2&>
  عملگر () (Sink1 *s1, Sink2 *s2) ثابت
  { أرجع hb_unzip_t<Sink1&, Sink2&> (*s1, *s2); }
}
HB_FUNCOBJ (hb_unzip);


/* hb-all, hb-any, hb-none. */

بنية
{
  قالب <نام_نوع Iterable,
	    نام_نوع Pred = decltype ((hb_عنصر_خنثی)),
	    نام_نوع Proj = decltype ((hb_عنصر_خنثی)),
	    hb_requires (hb_is_iterable (Iterable))>
  منطقي عملگر () (Iterable&& c,
		    Pred&& p = hb_عنصر_خنثی,
		    Proj&& f = hb_عنصر_خنثی) ثابت
  {
    لكل (خودکار it = hb_iter (c); it; ++it)
      إذا (!hb_تطابق (p, hb_get (f, *it)))
	أرجع خطأ;
    أرجع درست;
  }
}
HB_FUNCOBJ (hb_all);
بنية
{
  قالب <نام_نوع Iterable,
	    نام_نوع Pred = decltype ((hb_عنصر_خنثی)),
	    نام_نوع Proj = decltype ((hb_عنصر_خنثی)),
	    hb_requires (hb_is_iterable (Iterable))>
  منطقي عملگر () (Iterable&& c,
		    Pred&& p = hb_عنصر_خنثی,
		    Proj&& f = hb_عنصر_خنثی) ثابت
  {
    لكل (خودکار it = hb_iter (c); it; ++it)
      إذا (hb_تطابق (p, hb_get (f, *it)))
	أرجع درست;
    أرجع خطأ;
  }
}
HB_FUNCOBJ (hb_any);
بنية
{
  قالب <نام_نوع Iterable,
	    نام_نوع Pred = decltype ((hb_عنصر_خنثی)),
	    نام_نوع Proj = decltype ((hb_عنصر_خنثی)),
	    hb_requires (hb_is_iterable (Iterable))>
  منطقي عملگر () (Iterable&& c,
		    Pred&& p = hb_عنصر_خنثی,
		    Proj&& f = hb_عنصر_خنثی) ثابت
  {
    لكل (خودکار it = hb_iter (c); it; ++it)
      إذا (hb_تطابق (p, hb_get (f, *it)))
	أرجع خطأ;
    أرجع درست;
  }
}
HB_FUNCOBJ (hb_none);

/*
 * Algorithms operating on iterators.
 */

قالب <نام_نوع C, نام_نوع V,
	  hb_requires (hb_is_iterable (C))>
مضمن فراغ
hb_fill (C&& c, ثابت V &v)
{
  لكل (خودکار i = hb_iter (c); i; i++)
    *i = v;
}

قالب <نام_نوع S, نام_نوع D>
مضمن فراغ
hb_copy (S&& is, D&& id)
{
  hb_iter (is) | hb_sink (id);
}


#پایان_اگر /* HB_ITER_HH */

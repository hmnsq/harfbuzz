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

#جے_بنا_تعریف HB_ITER_HH
#تعریف_کرو HB_ITER_HH

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
سانچا <قسم_ناں iter_t, قسم_ناں شے_3 = قسم_ناں iter_t::__item_t__>
بنية hb_iter_t
{
  تعريفنوع شے_3 شے_t;
  constexpr بنانشانی get_شے_ناپ () ثابت { أرجع hb_static_ناپ (شے_3); }
  ساكن constexpr منطقي is_iterator = سچ;
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
  iter_t عامل + () ثابت { أرجع *thiz(); }
  iter_t _begin () ثابت { أرجع *thiz(); }
  iter_t begin () ثابت { أرجع _begin (); }
  iter_t سرا_3 () ثابت { أرجع thiz()->__end__ (); }
  iter_t سرا () ثابت { أرجع سرا_3 (); }
  واضح عامل منطقي () ثابت { أرجع thiz()->__more__ (); }
  بنانشانی len () ثابت { أرجع thiz()->__len__ (); }
  /* The following can only be enabled if item_t is reference type.  Otherwise
   * it will be returning pointer to temporary rvalue. */
  سانچا <قسم_ناں T = شے_t,
	    hb_enable_if (std::is_reference<T>::value)>
  hb_remove_reference<شے_t>* عامل -> () ثابت { أرجع std::addressof (**thiz()); }
  شے_t عامل * () ثابت { أرجع thiz()->__item__ (); }
  شے_t عامل * () { أرجع thiz()->__item__ (); }
  شے_t عامل [] (بنانشانی i) ثابت { أرجع thiz()->__item_at__ (i); }
  شے_t عامل [] (بنانشانی i) { أرجع thiz()->__item_at__ (i); }
  iter_t& عامل += (بنانشانی گنتی) &  { thiz()->__forward__ (گنتی); أرجع *thiz(); }
  iter_t  عامل += (بنانشانی گنتی) && { thiz()->__forward__ (گنتی); أرجع *thiz(); }
  iter_t& عامل ++ () &  { thiz()->__next__ (); أرجع *thiz(); }
  iter_t  عامل ++ () && { thiz()->__next__ (); أرجع *thiz(); }
  iter_t& عامل -= (بنانشانی گنتی) &  { thiz()->__rewind__ (گنتی); أرجع *thiz(); }
  iter_t  عامل -= (بنانشانی گنتی) && { thiz()->__rewind__ (گنتی); أرجع *thiz(); }
  iter_t& عامل -- () &  { thiz()->__prev__ (); أرجع *thiz(); }
  iter_t  عامل -- () && { thiz()->__prev__ (); أرجع *thiz(); }
  iter_t عامل + (بنانشانی گنتی) ثابت { خودکار c = thiz()->iter (); c += گنتی; أرجع c; }
  یار iter_t عامل + (بنانشانی گنتی, ثابت iter_t &it) { أرجع it + گنتی; }
  iter_t عامل ++ (صحيح) { iter_t c (*thiz()); ++*thiz(); أرجع c; }
  iter_t عامل - (بنانشانی گنتی) ثابت { خودکار c = thiz()->iter (); c -= گنتی; أرجع c; }
  iter_t عامل -- (صحيح) { iter_t c (*thiz()); --*thiz(); أرجع c; }
  سانچا <قسم_ناں T>
  iter_t& عامل >> (T &v) &  { v = **thiz(); ++*thiz(); أرجع *thiz(); }
  سانچا <قسم_ناں T>
  iter_t  عامل >> (T &v) && { v = **thiz(); ++*thiz(); أرجع *thiz(); }
  سانچا <قسم_ناں T>
  iter_t& عامل << (ثابت T v) &  { **thiz() = v; ++*thiz(); أرجع *thiz(); }
  سانچا <قسم_ناں T>
  iter_t  عامل << (ثابت T v) && { **thiz() = v; ++*thiz(); أرجع *thiz(); }

  محمي:
  hb_iter_t () = افتراضي;
  hb_iter_t (ثابت hb_iter_t &o HB_بن_ورتیا) = افتراضي;
  hb_iter_t (hb_iter_t &&o HB_بن_ورتیا) = افتراضي;
  hb_iter_t& عامل = (ثابت hb_iter_t &o HB_بن_ورتیا) = افتراضي;
  hb_iter_t& عامل = (hb_iter_t &&o HB_بن_ورتیا) = افتراضي;
};

#تعریف_کرو HB_ITER_USING(Name) \
  ورتوں شے_t = قسم_ناں Name::شے_t; \
  ورتوں Name::_begin; \
  ورتوں Name::begin; \
  ورتوں Name::سرا_3; \
  ورتوں Name::سرا; \
  ورتوں Name::get_شے_ناپ; \
  ورتوں Name::is_iterator; \
  ورتوں Name::iter; \
  ورتوں Name::عامل منطقي; \
  ورتوں Name::len; \
  ورتوں Name::عامل ->; \
  ورتوں Name::عامل *; \
  ورتوں Name::عامل []; \
  ورتوں Name::عامل +=; \
  ورتوں Name::عامل ++; \
  ورتوں Name::عامل -=; \
  ورتوں Name::عامل --; \
  ورتوں Name::عامل +; \
  ورتوں Name::عامل -; \
  ورتوں Name::عامل >>; \
  ورتوں Name::عامل <<; \
  static_assert (سچ, "")

/* Returns iterator / item type of a type. */
سانچا <قسم_ناں Iterable>
ورتوں hb_iter_type = decltype (hb_deref (hb_declval (Iterable)).iter ());
سانچا <قسم_ناں Iterable>
ورتوں hb_شے_type = decltype (*hb_deref (hb_declval (Iterable)).iter ());


سانچا <قسم_ناں> بنية hb_array_t;
سانچا <قسم_ناں> بنية hb_sorted_array_t;

بنية
{
  سانچا <قسم_ناں T> hb_iter_type<T>
  عامل () (T&& c) ثابت
  { أرجع hb_deref (std::forward<T> (c)).iter (); }

  /* Specialization for C arrays. */

  سانچا <قسم_ناں Type> مضمن hb_array_t<Type>
  عامل () (Type *array, بنانشانی صحيح لمائی) ثابت
  { أرجع hb_array_t<Type> (array, لمائی); }

  سانچا <قسم_ناں Type, بنانشانی صحيح لمائی> hb_array_t<Type>
  عامل () (Type (&array)[لمائی]) ثابت
  { أرجع hb_array_t<Type> (array, لمائی); }

}
HB_FUNCOBJ (hb_iter);
بنية
{
  سانچا <قسم_ناں T> خودکار
  impl (T&& c, hb_ترجیح<1>) ثابت HB_RETURN (بنانشانی, c.len ())

  سانچا <قسم_ناں T> خودکار
  impl (T&& c, hb_ترجیح<0>) ثابت HB_RETURN (بنانشانی, c.len)

  عام:

  سانچا <قسم_ناں T> خودکار
  عامل () (T&& c) ثابت HB_RETURN (بنانشانی, impl (std::forward<T> (c), hb_prioritize))
}
HB_FUNCOBJ (hb_len);

/* Mixin to fill in what the subclass doesn't provide. */
سانچا <قسم_ناں iter_t, قسم_ناں شے_t = قسم_ناں iter_t::__item_t__>
بنية hb_iter_fallback_mixin_t
{
  خاص:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ثابت iter_t* thiz () ثابت { أرجع static_cast<ثابت iter_t *> (هذا); }
	iter_t* thiz ()       { أرجع static_cast<      iter_t *> (هذا); }
  عام:

  /* Access: Implement __item__(), or __item_at__() if random-access. */
  شے_t __item__ () ثابت { أرجع (*thiz())[0]; }
  شے_t __item_at__ (بنانشانی i) ثابت { أرجع *(*thiz() + i); }

  /* Termination: Implement __more__(), or __len__() if random-access. */
  منطقي __more__ () ثابت { أرجع منطقي (thiz()->len ()); }
  بنانشانی __len__ () ثابت
  { iter_t c (*thiz()); بنانشانی l = 0; طالما (c) { c++; l++; } أرجع l; }

  /* Advancing: Implement __next__(), or __forward__() if random-access. */
  فراغ __next__ () { *thiz() += 1; }
  فراغ __forward__ (بنانشانی n) { طالما (*thiz() && n--) ++*thiz(); }

  /* Rewinding: Implement __prev__() or __rewind__() if bidirectional. */
  فراغ __prev__ () { *thiz() -= 1; }
  فراغ __rewind__ (بنانشانی n) { طالما (*thiz() && n--) --*thiz(); }

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
  hb_iter_fallback_mixin_t (ثابت hb_iter_fallback_mixin_t &o HB_بن_ورتیا) = افتراضي;
  hb_iter_fallback_mixin_t (hb_iter_fallback_mixin_t &&o HB_بن_ورتیا) = افتراضي;
  hb_iter_fallback_mixin_t& عامل = (ثابت hb_iter_fallback_mixin_t &o HB_بن_ورتیا) = افتراضي;
  hb_iter_fallback_mixin_t& عامل = (hb_iter_fallback_mixin_t &&o HB_بن_ورتیا) = افتراضي;
};

سانچا <قسم_ناں iter_t, قسم_ناں شے_t = قسم_ناں iter_t::__item_t__>
بنية hb_iter_with_fallback_t :
  hb_iter_t<iter_t, شے_t>,
  hb_iter_fallback_mixin_t<iter_t, شے_t>
{
  محمي:
  hb_iter_with_fallback_t () = افتراضي;
  hb_iter_with_fallback_t (ثابت hb_iter_with_fallback_t &o HB_بن_ورتیا) = افتراضي;
  hb_iter_with_fallback_t (hb_iter_with_fallback_t &&o HB_بن_ورتیا) = افتراضي;
  hb_iter_with_fallback_t& عامل = (ثابت hb_iter_with_fallback_t &o HB_بن_ورتیا) = افتراضي;
  hb_iter_with_fallback_t& عامل = (hb_iter_with_fallback_t &&o HB_بن_ورتیا) = افتراضي;
};

/*
 * Meta-programming predicates.
 */

/* hb_is_iterator() / hb_is_iterator_of() */

سانچا<قسم_ناں Iter, قسم_ناں شے_3>
بنية hb_is_iterator_دا
{
  سانچا <قسم_ناں Item2 = شے_3>
  ساكن hb_true_type impl (hb_ترجیح<2>, hb_iter_t<Iter, hb_type_پچھان_تت<Item2>> *);
  ساكن hb_false_type impl (hb_ترجیح<0>, ثابت فراغ *);

  عام:
  ساكن constexpr منطقي value = decltype (impl (hb_prioritize, hb_declval (Iter*)))::value;
};
#تعریف_کرو hb_is_iterator_دا(Iter, شے_3) hb_is_iterator_دا<Iter, شے_3>::value
#تعریف_کرو hb_is_iterator(Iter) hb_is_iterator_دا (Iter, قسم_ناں Iter::شے_t)
#تعریف_کرو hb_is_sorted_iterator_دا(Iter, شے_3) (hb_is_iterator_دا<Iter, شے_3>::value && Iter::is_sorted_iterator)
#تعریف_کرو hb_is_sorted_iterator(Iter) hb_is_sorted_iterator_دا (Iter, قسم_ناں Iter::شے_t)

/* hb_is_iterable() */

سانچا <قسم_ناں T>
بنية hb_is_iterable
{
  خاص:

  سانچا <قسم_ناں U>
  ساكن خودکار impl (hb_ترجیح<1>) -> decltype (hb_declval (U).iter (), hb_true_type ());

  سانچا <قسم_ناں>
  ساكن hb_false_type impl (hb_ترجیح<0>);

  عام:
  ساكن constexpr منطقي value = decltype (impl<T> (hb_prioritize))::value;
};
#تعریف_کرو hb_is_iterable(Iterable) hb_is_iterable<Iterable>::value

/* hb_is_source_of() / hb_is_sink_of() */

سانچا<قسم_ناں Iter, قسم_ناں شے_3>
بنية hb_is_سروت_دا
{
  خاص:
  سانچا <قسم_ناں Iter2 = Iter,
	    hb_enable_if (hb_is_convertible (قسم_ناں Iter2::شے_t, hb_جوڑو_lvalue_reference<ثابت شے_3>))>
  ساكن hb_true_type impl (hb_ترجیح<2>);
  سانچا <قسم_ناں Iter2 = Iter>
  ساكن خودکار impl (hb_ترجیح<1>) -> decltype (hb_declval (Iter2) >> hb_declval (شے_3 &), hb_true_type ());
  ساكن hb_false_type impl (hb_ترجیح<0>);

  عام:
  ساكن constexpr منطقي value = decltype (impl (hb_prioritize))::value;
};
#تعریف_کرو hb_is_سروت_دا(Iter, شے_3) hb_is_سروت_دا<Iter, شے_3>::value

سانچا<قسم_ناں Iter, قسم_ناں شے_3>
بنية hb_is_sink_دا
{
  خاص:
  سانچا <قسم_ناں Iter2 = Iter,
	    hb_enable_if (hb_is_convertible (قسم_ناں Iter2::شے_t, hb_جوڑو_lvalue_reference<شے_3>))>
  ساكن hb_true_type impl (hb_ترجیح<2>);
  سانچا <قسم_ناں Iter2 = Iter>
  ساكن خودکار impl (hb_ترجیح<1>) -> decltype (hb_declval (Iter2) << hb_declval (شے_3), hb_true_type ());
  ساكن hb_false_type impl (hb_ترجیح<0>);

  عام:
  ساكن constexpr منطقي value = decltype (impl (hb_prioritize))::value;
};
#تعریف_کرو hb_is_sink_دا(Iter, شے_3) hb_is_sink_دا<Iter, شے_3>::value

/* This is commonly used, so define: */
#تعریف_کرو hb_is_sorted_سروت_دا(Iter, شے_3) \
	(hb_is_سروت_دا(Iter, شے_3) && Iter::is_sorted_iterator)


بنية
{
  سانچا <قسم_ناں Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  بنانشانی عامل () (ثابت Iterable &_) ثابت { أرجع hb_len (hb_iter (_)); }

  بنانشانی عامل () (بنانشانی _) ثابت { أرجع _; }
}
HB_FUNCOBJ (hb_len_دا);

/* Range-based 'for' for iterables. */

سانچا <قسم_ناں Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ساكن مضمن خودکار begin (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).begin ())

سانچا <قسم_ناں Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ساكن مضمن خودکار سرا (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).سرا ())

/* begin()/end() are NOT looked up non-ADL.  So each namespace must declare them.
 * Do it for namespace OT. */
نطاق OT {

سانچا <قسم_ناں Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ساكن مضمن خودکار begin (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).begin ())

سانچا <قسم_ناں Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ساكن مضمن خودکار سرا (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).سرا ())

}


/*
 * Adaptors, combiners, etc.
 */

سانچا <قسم_ناں Lhs, قسم_ناں Rhs,
	  hb_requires (hb_is_iterator (Lhs))>
ساكن مضمن خودکار
عامل | (Lhs&& lhs, Rhs&& rhs) HB_AUTO_RETURN (std::forward<Rhs> (rhs) (std::forward<Lhs> (lhs)))

/* hb_map(), hb_filter(), hb_reduce() */

تعداد  صنف hb_function_sortedness_t {
  NOT_SORTED,
  RETAINS_چھانٹی,
  SORTED,
};

سانچا <قسم_ناں Iter, قسم_ناں Proj, hb_function_sortedness_t Sorted,
	 hb_requires (hb_is_iterator (Iter))>
بنية hb_map_iter_t :
  hb_iter_t<hb_map_iter_t<Iter, Proj, Sorted>,
	    decltype (hb_get (hb_declval (Proj), *hb_declval (Iter)))>
{
  hb_map_iter_t (ثابت Iter& it, Proj f_) : it (it), f (f_) {}

  تعريفنوع decltype (hb_get (hb_declval (Proj), *hb_declval (Iter))) __item_t__;
  ساكن constexpr منطقي is_random_access_iterator = Iter::is_random_access_iterator;
  ساكن constexpr منطقي is_sorted_iterator =
    Sorted == hb_function_sortedness_t::SORTED ? سچ :
    Sorted == hb_function_sortedness_t::RETAINS_چھانٹی ? Iter::is_sorted_iterator :
    خطأ;
  __item_t__ __item__ () ثابت { أرجع hb_get (f.get (), *it); }
  __item_t__ __item_at__ (بنانشانی i) ثابت { أرجع hb_get (f.get (), it[i]); }
  منطقي __more__ () ثابت { أرجع منطقي (it); }
  بنانشانی __len__ () ثابت { أرجع it.len (); }
  فراغ __next__ () { ++it; }
  فراغ __forward__ (بنانشانی n) { it += n; }
  فراغ __prev__ () { --it; }
  فراغ __rewind__ (بنانشانی n) { it -= n; }
  hb_map_iter_t __end__ () ثابت { أرجع hb_map_iter_t (it.سرا_3 (), f); }
  منطقي عامل != (ثابت hb_map_iter_t& o) ثابت
  { أرجع it != o.it; }

  خاص:
  Iter it;
  بدلنجوگ hb_reference_wrapper<Proj> f;
};

سانچا <قسم_ناں Proj, hb_function_sortedness_t Sorted>
بنية hb_map_iter_factory_t
{
  hb_map_iter_factory_t (Proj f) : f (f) {}

  سانچا <قسم_ناں Iter,
	    hb_requires (hb_is_iterator (Iter))>
  hb_map_iter_t<Iter, Proj, Sorted>
  عامل () (Iter it)
  { أرجع hb_map_iter_t<Iter, Proj, Sorted> (it, f); }

  خاص:
  Proj f;
};
بنية
{
  سانچا <قسم_ناں Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::NOT_SORTED>
  عامل () (Proj&& f) ثابت
  { أرجع hb_map_iter_factory_t<Proj, hb_function_sortedness_t::NOT_SORTED> (f); }
}
HB_FUNCOBJ (hb_map);
بنية
{
  سانچا <قسم_ناں Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::RETAINS_چھانٹی>
  عامل () (Proj&& f) ثابت
  { أرجع hb_map_iter_factory_t<Proj, hb_function_sortedness_t::RETAINS_چھانٹی> (f); }
}
HB_FUNCOBJ (hb_map_retains_چھانٹی);
بنية
{
  سانچا <قسم_ناں Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::SORTED>
  عامل () (Proj&& f) ثابت
  { أرجع hb_map_iter_factory_t<Proj, hb_function_sortedness_t::SORTED> (f); }
}
HB_FUNCOBJ (hb_map_sorted);

سانچا <قسم_ناں Iter, قسم_ناں Pred, قسم_ناں Proj,
	 hb_requires (hb_is_iterator (Iter))>
بنية hb_filter_iter_t :
  hb_iter_with_fallback_t<hb_filter_iter_t<Iter, Pred, Proj>,
			  قسم_ناں Iter::شے_t>
{
  hb_filter_iter_t (ثابت Iter& it_, Pred p_, Proj f_) : it (it_), p (p_), f (f_)
  { طالما (it && !hb_has (p.get (), hb_get (f.get (), *it))) ++it; }

  تعريفنوع قسم_ناں Iter::شے_t __item_t__;
  ساكن constexpr منطقي is_sorted_iterator = Iter::is_sorted_iterator;
  __item_t__ __item__ () ثابت { أرجع *it; }
  منطقي __more__ () ثابت { أرجع منطقي (it); }
  فراغ __next__ () { افعل ++it; طالما (it && !hb_has (p.get (), hb_get (f.get (), *it))); }
  فراغ __prev__ ()
  {
    بنانشانی گنتی = 0;
    افعل
    {
      Iter old = it;
      --it;
      إذا (!(it != old))
      {
	it += گنتی;
	أرجع;
      }
      گنتی++;
    }
    طالما (!hb_has (p.get (), hb_get (f.get (), *it)));
  }
  hb_filter_iter_t __end__ () ثابت { أرجع hb_filter_iter_t (it.سرا_3 (), p, f); }
  منطقي عامل != (ثابت hb_filter_iter_t& o) ثابت
  { أرجع it != o.it; }

  خاص:
  Iter it;
  بدلنجوگ hb_reference_wrapper<Pred> p;
  بدلنجوگ hb_reference_wrapper<Proj> f;
};
سانچا <قسم_ناں Pred, قسم_ناں Proj>
بنية hb_filter_iter_factory_t
{
  hb_filter_iter_factory_t (Pred p, Proj f) : p (p), f (f) {}

  سانچا <قسم_ناں Iter,
	    hb_requires (hb_is_iterator (Iter))>
  hb_filter_iter_t<Iter, Pred, Proj>
  عامل () (Iter it)
  { أرجع hb_filter_iter_t<Iter, Pred, Proj> (it, p, f); }

  خاص:
  Pred p;
  Proj f;
};
بنية
{
  سانچا <قسم_ناں Pred = decltype ((hb_پچھان_تت)),
	    قسم_ناں Proj = decltype ((hb_پچھان_تت))>
  hb_filter_iter_factory_t<Pred, Proj>
  عامل () (Pred&& p = hb_پچھان_تت, Proj&& f = hb_پچھان_تت) ثابت
  { أرجع hb_filter_iter_factory_t<Pred, Proj> (p, f); }
}
HB_FUNCOBJ (hb_filter);

سانچا <قسم_ناں Redu, قسم_ناں InitT>
بنية hb_reduce_t
{
  hb_reduce_t (Redu r, InitT init_قدر) : r (r), init_قدر (init_قدر) {}

  سانچا <قسم_ناں Iter,
	    hb_requires (hb_is_iterator (Iter)),
	    قسم_ناں AccuT = hb_decay<decltype (hb_declval (Redu) (hb_declval (InitT), hb_declval (قسم_ناں Iter::شے_t)))>>
  AccuT
  عامل () (Iter it)
  {
    AccuT value = init_قدر;
    لكل (; it; ++it)
      value = r (value, *it);
    أرجع value;
  }

  خاص:
  Redu r;
  InitT init_قدر;
};
بنية
{
  سانچا <قسم_ناں Redu, قسم_ناں InitT>
  hb_reduce_t<Redu, InitT>
  عامل () (Redu&& r, InitT init_قدر) ثابت
  { أرجع hb_reduce_t<Redu, InitT> (r, init_قدر); }
}
HB_FUNCOBJ (hb_reduce);


/* hb_zip() */

سانچا <قسم_ناں A, قسم_ناں B>
بنية hb_zip_iter_t :
  hb_iter_t<hb_zip_iter_t<A, B>,
	    hb_pair_t<قسم_ناں A::شے_t, قسم_ناں B::شے_t>>
{
  hb_zip_iter_t () {}
  hb_zip_iter_t (ثابت A& a, ثابت B& b) : a (a), b (b) {}

  تعريفنوع hb_pair_t<قسم_ناں A::شے_t, قسم_ناں B::شے_t> __item_t__;
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
  __item_t__ __item_at__ (بنانشانی i) ثابت { أرجع __item_t__ (a[i], b[i]); }
  منطقي __more__ () ثابت { أرجع منطقي (a) && منطقي (b); }
  بنانشانی __len__ () ثابت { أرجع hb_min (a.len (), b.len ()); }
  فراغ __next__ () { ++a; ++b; }
  فراغ __forward__ (بنانشانی n) { a += n; b += n; }
  فراغ __prev__ () { --a; --b; }
  فراغ __rewind__ (بنانشانی n) { a -= n; b -= n; }
  hb_zip_iter_t __end__ () ثابت { أرجع hb_zip_iter_t (a.سرا_3 (), b.سرا_3 ()); }
  /* Note, we should stop if ANY of the iters reaches end.  As such two compare
   * unequal if both items are unequal, NOT if either is unequal. */
  منطقي عامل != (ثابت hb_zip_iter_t& o) ثابت
  { أرجع a != o.a && b != o.b; }

  خاص:
  A a;
  B b;
};
بنية
{ HB_PARTIALIZE(2);
  سانچا <قسم_ناں A, قسم_ناں B,
	    hb_requires (hb_is_iterable (A) && hb_is_iterable (B))>
  hb_zip_iter_t<hb_iter_type<A>, hb_iter_type<B>>
  عامل () (A&& a, B&& b) ثابت
  { أرجع hb_zip_iter_t<hb_iter_type<A>, hb_iter_type<B>> (hb_iter (a), hb_iter (b)); }
}
HB_FUNCOBJ (hb_zip);

/* hb_concat() */

سانچا <قسم_ناں A, قسم_ناں B>
بنية hb_concat_iter_t :
    hb_iter_t<hb_concat_iter_t<A, B>, قسم_ناں A::شے_t>
{
  hb_concat_iter_t () {}
  hb_concat_iter_t (A& a, B& b) : a (a), b (b) {}
  hb_concat_iter_t (ثابت A& a, ثابت B& b) : a (a), b (b) {}


  تعريفنوع قسم_ناں A::شے_t __item_t__;
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

  __item_t__ __item_at__ (بنانشانی i) ثابت
  {
    بنانشانی a_len = a.len ();
    إذا (i < a_len)
      أرجع a[i];
    أرجع b[i - a_len];
  }

  منطقي __more__ () ثابت { أرجع منطقي (a) || منطقي (b); }

  بنانشانی __len__ () ثابت { أرجع a.len () + b.len (); }

  فراغ __next__ ()
  {
    إذا (a)
      ++a;
    وإلا
      ++b;
  }

  فراغ __forward__ (بنانشانی n)
  {
    إذا (!n) أرجع;
    إذا (!is_random_access_iterator) {
      طالما (n-- && *هذا) {
        (*هذا)++;
      }
      أرجع;
    }

    بنانشانی a_len = a.len ();
    إذا (n > a_len) {
      n -= a_len;
      a.__forward__ (a_len);
      b.__forward__ (n);
    } وإلا {
      a.__forward__ (n);
    }
  }

  hb_concat_iter_t __end__ () ثابت { أرجع hb_concat_iter_t (a.سرا_3 (), b.سرا_3 ()); }
  منطقي عامل != (ثابت hb_concat_iter_t& o) ثابت
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
  سانچا <قسم_ناں A, قسم_ناں B,
	    hb_requires (hb_is_iterable (A) && hb_is_iterable (B))>
  hb_concat_iter_t<hb_iter_type<A>, hb_iter_type<B>>
  عامل () (A&& a, B&& b) ثابت
  { أرجع hb_concat_iter_t<hb_iter_type<A>, hb_iter_type<B>> (hb_iter (a), hb_iter (b)); }
}
HB_FUNCOBJ (hb_concat);

/* hb_apply() */

سانچا <قسم_ناں Appl>
بنية hb_apply_t
{
  hb_apply_t (Appl a) : a (a) {}

  سانچا <قسم_ناں Iter,
	    hb_requires (hb_is_iterator (Iter))>
  فراغ عامل () (Iter it)
  {
    لكل (; it; ++it)
      (فراغ) hb_invoke (a, *it);
  }

  خاص:
  Appl a;
};
بنية
{
  سانچا <قسم_ناں Appl> hb_apply_t<Appl>
  عامل () (Appl&& a) ثابت
  { أرجع hb_apply_t<Appl> (a); }

  سانچا <قسم_ناں Appl> hb_apply_t<Appl&>
  عامل () (Appl *a) ثابت
  { أرجع hb_apply_t<Appl&> (*a); }
}
HB_FUNCOBJ (hb_apply);

/* hb_range()/hb_iota()/hb_repeat() */

سانچا <قسم_ناں T, قسم_ناں S>
بنية hb_range_iter_t :
  hb_iter_t<hb_range_iter_t<T, S>, T>
{
  hb_range_iter_t (T شروع, T سرا_4, S قدم) : v (شروع), سرا_4 (سرا_for (شروع, سرا_4, قدم)), قدم (قدم) {}

  تعريفنوع T __item_t__;
  ساكن constexpr منطقي is_random_access_iterator = سچ;
  ساكن constexpr منطقي is_sorted_iterator = سچ;
  __item_t__ __item__ () ثابت { أرجع hb_ridentity (v); }
  __item_t__ __item_at__ (بنانشانی j) ثابت { أرجع v + j * قدم; }
  منطقي __more__ () ثابت { أرجع v != سرا_4; }
  بنانشانی __len__ () ثابت { أرجع !قدم ? UINT_MAX : (سرا_4 - v) / قدم; }
  فراغ __next__ () { v += قدم; }
  فراغ __forward__ (بنانشانی n) { v += n * قدم; }
  فراغ __prev__ () { v -= قدم; }
  فراغ __rewind__ (بنانشانی n) { v -= n * قدم; }
  hb_range_iter_t __end__ () ثابت { أرجع hb_range_iter_t (سرا_4, سرا_4, قدم); }
  منطقي عامل != (ثابت hb_range_iter_t& o) ثابت
  { أرجع v != o.v; }

  خاص:
  ساكن مضمن T سرا_for (T شروع, T سرا_4, S قدم)
  {
    إذا (!قدم)
      أرجع سرا_4;
    خودکار res = (سرا_4 - شروع) % قدم;
    إذا (!res)
      أرجع سرا_4;
    سرا_4 += قدم - res;
    أرجع سرا_4;
  }

  خاص:
  T v;
  T سرا_4;
  S قدم;
};
بنية
{
  سانچا <قسم_ناں T = بنانشانی> hb_range_iter_t<T, بنانشانی>
  عامل () (T سرا = (بنانشانی) -1) ثابت
  { أرجع hb_range_iter_t<T, بنانشانی> (0, سرا, 1u); }

  سانچا <قسم_ناں T, قسم_ناں S = بنانشانی> hb_range_iter_t<T, S>
  عامل () (T شروع, T سرا, S قدم = 1u) ثابت
  { أرجع hb_range_iter_t<T, S> (شروع, سرا, قدم); }
}
HB_FUNCOBJ (hb_range);

سانچا <قسم_ناں T, قسم_ناں S>
بنية hb_iota_iter_t :
  hb_iter_with_fallback_t<hb_iota_iter_t<T, S>, T>
{
  hb_iota_iter_t (T شروع, S قدم) : v (شروع), قدم (قدم) {}

  خاص:

  سانچا <قسم_ناں S2 = S>
  خودکار
  inc (hb_type_پچھان_تت<S2> s, hb_ترجیح<1>)
    -> hb_void_t<decltype (hb_invoke (std::forward<hb_type_پچھان_تت<S2>> (s),
                                      hb_declval<T&> ()))>
  { v = hb_invoke (std::forward<hb_type_پچھان_تت<S2>> (s), v); }

  فراغ
  inc (S s, hb_ترجیح<0>)
  { v += s; }

  عام:

  تعريفنوع T __item_t__;
  ساكن constexpr منطقي is_random_access_iterator = سچ;
  ساكن constexpr منطقي is_sorted_iterator = سچ;
  __item_t__ __item__ () ثابت { أرجع hb_ridentity (v); }
  منطقي __more__ () ثابت { أرجع سچ; }
  بنانشانی __len__ () ثابت { أرجع UINT_MAX; }
  فراغ __next__ () { inc (قدم, hb_prioritize); }
  فراغ __prev__ () { v -= قدم; }
  hb_iota_iter_t __end__ () ثابت { أرجع *هذا; }
  منطقي عامل != (ثابت hb_iota_iter_t& o) ثابت { أرجع سچ; }

  خاص:
  T v;
  S قدم;
};
بنية
{
  سانچا <قسم_ناں T = بنانشانی, قسم_ناں S = بنانشانی> hb_iota_iter_t<T, S>
  عامل () (T شروع = 0u, S قدم = 1u) ثابت
  { أرجع hb_iota_iter_t<T, S> (شروع, قدم); }
}
HB_FUNCOBJ (hb_iota);

سانچا <قسم_ناں T>
بنية hb_repeat_iter_t :
  hb_iter_t<hb_repeat_iter_t<T>, T>
{
  hb_repeat_iter_t (T value) : v (value) {}

  تعريفنوع T __item_t__;
  ساكن constexpr منطقي is_random_access_iterator = سچ;
  ساكن constexpr منطقي is_sorted_iterator = سچ;
  __item_t__ __item__ () ثابت { أرجع v; }
  __item_t__ __item_at__ (بنانشانی j) ثابت { أرجع v; }
  منطقي __more__ () ثابت { أرجع سچ; }
  بنانشانی __len__ () ثابت { أرجع UINT_MAX; }
  فراغ __next__ () {}
  فراغ __forward__ (بنانشانی) {}
  فراغ __prev__ () {}
  فراغ __rewind__ (بنانشانی) {}
  hb_repeat_iter_t __end__ () ثابت { أرجع *هذا; }
  منطقي عامل != (ثابت hb_repeat_iter_t& o) ثابت { أرجع سچ; }

  خاص:
  T v;
};
بنية
{
  سانچا <قسم_ناں T> hb_repeat_iter_t<T>
  عامل () (T value) ثابت
  { أرجع hb_repeat_iter_t<T> (value); }
}
HB_FUNCOBJ (hb_repeat);

/* hb_enumerate()/hb_take() */

بنية
{
  سانچا <قسم_ناں Iterable,
	    قسم_ناں اشاریہ = بنانشانی,
	    hb_requires (hb_is_iterable (Iterable))>
  خودکار عامل () (Iterable&& it, اشاریہ شروع = 0u) ثابت HB_AUTO_RETURN
  ( hb_zip (hb_iota (شروع), it) )
}
HB_FUNCOBJ (hb_enumerate);

بنية
{ HB_PARTIALIZE(2);
  سانچا <قسم_ناں Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  خودکار عامل () (Iterable&& it, بنانشانی گنتی) ثابت HB_AUTO_RETURN
  ( hb_zip (hb_range (گنتی), it) | hb_map_retains_چھانٹی (hb_دوجا) )

  /* Specialization arrays. */

  سانچا <قسم_ناں Type> مضمن hb_array_t<Type>
  عامل () (hb_array_t<Type> array, بنانشانی گنتی) ثابت
  { أرجع array.sub_array (0, گنتی); }

  سانچا <قسم_ناں Type> مضمن hb_sorted_array_t<Type>
  عامل () (hb_sorted_array_t<Type> array, بنانشانی گنتی) ثابت
  { أرجع array.sub_array (0, گنتی); }
}
HB_FUNCOBJ (hb_take);

بنية
{ HB_PARTIALIZE(2);
  سانچا <قسم_ناں Iter,
	    hb_requires (hb_is_iterator (Iter))>
  خودکار عامل () (Iter it, بنانشانی گنتی) ثابت HB_AUTO_RETURN
  (
    + hb_iota (it, hb_جوڑو (گنتی))
    | hb_map (hb_take (گنتی))
    | hb_take ((hb_len (it) + گنتی - 1) / گنتی)
  )
}
HB_FUNCOBJ (hb_chop);

/* hb_sink() */

سانچا <قسم_ناں Sink>
بنية hb_sink_t
{
  hb_sink_t (Sink s) : s (s) {}

  سانچا <قسم_ناں Iter,
	    hb_requires (hb_is_iterator (Iter))>
  فراغ عامل () (Iter it)
  {
    لكل (; it; ++it)
      s << *it;
  }

  خاص:
  Sink s;
};
بنية
{
  سانچا <قسم_ناں Sink> hb_sink_t<Sink>
  عامل () (Sink&& s) ثابت
  { أرجع hb_sink_t<Sink> (s); }

  سانچا <قسم_ناں Sink> hb_sink_t<Sink&>
  عامل () (Sink *s) ثابت
  { أرجع hb_sink_t<Sink&> (*s); }
}
HB_FUNCOBJ (hb_sink);

/* hb-drain: hb_sink to void / blackhole / /dev/null. */

بنية
{
  سانچا <قسم_ناں Iter,
	    hb_requires (hb_is_iterator (Iter))>
  فراغ عامل () (Iter it) ثابت
  {
    لكل (; it; ++it)
      (فراغ) *it;
  }
}
HB_FUNCOBJ (hb_drain);

/* hb_unzip(): unzip and sink to two sinks. */

سانچا <قسم_ناں Sink1, قسم_ناں Sink2>
بنية hb_unzip_t
{
  hb_unzip_t (Sink1 s1, Sink2 s2) : s1 (s1), s2 (s2) {}

  سانچا <قسم_ناں Iter,
	    hb_requires (hb_is_iterator (Iter))>
  فراغ عامل () (Iter it)
  {
    لكل (; it; ++it)
    {
      ثابت خودکار &v = *it;
      s1 << v.پہلا;
      s2 << v.second;
    }
  }

  خاص:
  Sink1 s1;
  Sink2 s2;
};
بنية
{
  سانچا <قسم_ناں Sink1, قسم_ناں Sink2> hb_unzip_t<Sink1, Sink2>
  عامل () (Sink1&& s1, Sink2&& s2) ثابت
  { أرجع hb_unzip_t<Sink1, Sink2> (s1, s2); }

  سانچا <قسم_ناں Sink1, قسم_ناں Sink2> hb_unzip_t<Sink1&, Sink2&>
  عامل () (Sink1 *s1, Sink2 *s2) ثابت
  { أرجع hb_unzip_t<Sink1&, Sink2&> (*s1, *s2); }
}
HB_FUNCOBJ (hb_unzip);


/* hb-all, hb-any, hb-none. */

بنية
{
  سانچا <قسم_ناں Iterable,
	    قسم_ناں Pred = decltype ((hb_پچھان_تت)),
	    قسم_ناں Proj = decltype ((hb_پچھان_تت)),
	    hb_requires (hb_is_iterable (Iterable))>
  منطقي عامل () (Iterable&& c,
		    Pred&& p = hb_پچھان_تت,
		    Proj&& f = hb_پچھان_تت) ثابت
  {
    لكل (خودکار it = hb_iter (c); it; ++it)
      إذا (!hb_میل (p, hb_get (f, *it)))
	أرجع خطأ;
    أرجع سچ;
  }
}
HB_FUNCOBJ (hb_all);
بنية
{
  سانچا <قسم_ناں Iterable,
	    قسم_ناں Pred = decltype ((hb_پچھان_تت)),
	    قسم_ناں Proj = decltype ((hb_پچھان_تت)),
	    hb_requires (hb_is_iterable (Iterable))>
  منطقي عامل () (Iterable&& c,
		    Pred&& p = hb_پچھان_تت,
		    Proj&& f = hb_پچھان_تت) ثابت
  {
    لكل (خودکار it = hb_iter (c); it; ++it)
      إذا (hb_میل (p, hb_get (f, *it)))
	أرجع سچ;
    أرجع خطأ;
  }
}
HB_FUNCOBJ (hb_any);
بنية
{
  سانچا <قسم_ناں Iterable,
	    قسم_ناں Pred = decltype ((hb_پچھان_تت)),
	    قسم_ناں Proj = decltype ((hb_پچھان_تت)),
	    hb_requires (hb_is_iterable (Iterable))>
  منطقي عامل () (Iterable&& c,
		    Pred&& p = hb_پچھان_تت,
		    Proj&& f = hb_پچھان_تت) ثابت
  {
    لكل (خودکار it = hb_iter (c); it; ++it)
      إذا (hb_میل (p, hb_get (f, *it)))
	أرجع خطأ;
    أرجع سچ;
  }
}
HB_FUNCOBJ (hb_none);

/*
 * Algorithms operating on iterators.
 */

سانچا <قسم_ناں C, قسم_ناں V,
	  hb_requires (hb_is_iterable (C))>
مضمن فراغ
hb_fill (C&& c, ثابت V &v)
{
  لكل (خودکار i = hb_iter (c); i; i++)
    *i = v;
}

سانچا <قسم_ناں S, قسم_ناں D>
مضمن فراغ
hb_copy (S&& is, D&& id)
{
  hb_iter (is) | hb_sink (id);
}


#جے_انت /* HB_ITER_HH */

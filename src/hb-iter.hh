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

#اگر_غیر_تعریف_شدہ HB_ITER_HH
#تعریف_کرو HB_ITER_HH

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
سانچہ <قسم_نام iter_t, قسم_نام شے_3 = قسم_نام iter_t::__item_t__>
ساخت hb_iter_t
{
  قسمتعریف شے_3 شے_t;
  constexpr غیرعلامتی get_شے_سائز () ثابت { واپس hb_static_سائز (شے_3); }
  ساکن constexpr منطقی is_iterator = سچ;
  ساکن constexpr منطقی is_random_access_iterator = جھوٹ;
  ساکن constexpr منطقی is_sorted_iterator = جھوٹ;
  ساکن constexpr منطقی has_fast_len = جھوٹ; // Should be checked in combination with is_random_access_iterator.

  نجی:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ثابت iter_t* thiz () ثابت { واپس static_cast<ثابت iter_t *> (یہ); }
	iter_t* thiz ()       { واپس static_cast<      iter_t *> (یہ); }
  عوامی:

  /* Operators. */
  iter_t iter () ثابت { واپس *thiz(); }
  iter_t عامل + () ثابت { واپس *thiz(); }
  iter_t _begin () ثابت { واپس *thiz(); }
  iter_t begin () ثابت { واپس _begin (); }
  iter_t انتہا_3 () ثابت { واپس thiz()->__end__ (); }
  iter_t انتہا () ثابت { واپس انتہا_3 (); }
  صریح عامل منطقی () ثابت { واپس thiz()->__more__ (); }
  غیرعلامتی len () ثابت { واپس thiz()->__len__ (); }
  /* The following can only be enabled if item_t is reference type.  Otherwise
   * it will be returning pointer to temporary rvalue. */
  سانچہ <قسم_نام T = شے_t,
	    hb_enable_if (std::is_reference<T>::value)>
  hb_remove_reference<شے_t>* عامل -> () ثابت { واپس std::addressof (**thiz()); }
  شے_t عامل * () ثابت { واپس thiz()->__item__ (); }
  شے_t عامل * () { واپس thiz()->__item__ (); }
  شے_t عامل [] (غیرعلامتی i) ثابت { واپس thiz()->__item_at__ (i); }
  شے_t عامل [] (غیرعلامتی i) { واپس thiz()->__item_at__ (i); }
  iter_t& عامل += (غیرعلامتی گنتی) &  { thiz()->__forward__ (گنتی); واپس *thiz(); }
  iter_t  عامل += (غیرعلامتی گنتی) && { thiz()->__forward__ (گنتی); واپس *thiz(); }
  iter_t& عامل ++ () &  { thiz()->__next__ (); واپس *thiz(); }
  iter_t  عامل ++ () && { thiz()->__next__ (); واپس *thiz(); }
  iter_t& عامل -= (غیرعلامتی گنتی) &  { thiz()->__rewind__ (گنتی); واپس *thiz(); }
  iter_t  عامل -= (غیرعلامتی گنتی) && { thiz()->__rewind__ (گنتی); واپس *thiz(); }
  iter_t& عامل -- () &  { thiz()->__prev__ (); واپس *thiz(); }
  iter_t  عامل -- () && { thiz()->__prev__ (); واپس *thiz(); }
  iter_t عامل + (غیرعلامتی گنتی) ثابت { خودکار c = thiz()->iter (); c += گنتی; واپس c; }
  دوست iter_t عامل + (غیرعلامتی گنتی, ثابت iter_t &it) { واپس it + گنتی; }
  iter_t عامل ++ (صحیح) { iter_t c (*thiz()); ++*thiz(); واپس c; }
  iter_t عامل - (غیرعلامتی گنتی) ثابت { خودکار c = thiz()->iter (); c -= گنتی; واپس c; }
  iter_t عامل -- (صحیح) { iter_t c (*thiz()); --*thiz(); واپس c; }
  سانچہ <قسم_نام T>
  iter_t& عامل >> (T &v) &  { v = **thiz(); ++*thiz(); واپس *thiz(); }
  سانچہ <قسم_نام T>
  iter_t  عامل >> (T &v) && { v = **thiz(); ++*thiz(); واپس *thiz(); }
  سانچہ <قسم_نام T>
  iter_t& عامل << (ثابت T v) &  { **thiz() = v; ++*thiz(); واپس *thiz(); }
  سانچہ <قسم_نام T>
  iter_t  عامل << (ثابت T v) && { **thiz() = v; ++*thiz(); واپس *thiz(); }

  محفوظ:
  hb_iter_t () = طےشدہ;
  hb_iter_t (ثابت hb_iter_t &o HB_غیر_مستعمل) = طےشدہ;
  hb_iter_t (hb_iter_t &&o HB_غیر_مستعمل) = طےشدہ;
  hb_iter_t& عامل = (ثابت hb_iter_t &o HB_غیر_مستعمل) = طےشدہ;
  hb_iter_t& عامل = (hb_iter_t &&o HB_غیر_مستعمل) = طےشدہ;
};

#تعریف_کرو HB_ITER_USING(Name) \
  استعمال شے_t = قسم_نام Name::شے_t; \
  استعمال Name::_begin; \
  استعمال Name::begin; \
  استعمال Name::انتہا_3; \
  استعمال Name::انتہا; \
  استعمال Name::get_شے_سائز; \
  استعمال Name::is_iterator; \
  استعمال Name::iter; \
  استعمال Name::عامل منطقی; \
  استعمال Name::len; \
  استعمال Name::عامل ->; \
  استعمال Name::عامل *; \
  استعمال Name::عامل []; \
  استعمال Name::عامل +=; \
  استعمال Name::عامل ++; \
  استعمال Name::عامل -=; \
  استعمال Name::عامل --; \
  استعمال Name::عامل +; \
  استعمال Name::عامل -; \
  استعمال Name::عامل >>; \
  استعمال Name::عامل <<; \
  static_assert (سچ, "")

/* Returns iterator / item type of a type. */
سانچہ <قسم_نام Iterable>
استعمال hb_iter_type = decltype (hb_deref (hb_declval (Iterable)).iter ());
سانچہ <قسم_نام Iterable>
استعمال hb_شے_type = decltype (*hb_deref (hb_declval (Iterable)).iter ());


سانچہ <قسم_نام> ساخت hb_array_t;
سانچہ <قسم_نام> ساخت hb_sorted_array_t;

ساخت
{
  سانچہ <قسم_نام T> hb_iter_type<T>
  عامل () (T&& c) ثابت
  { واپس hb_deref (std::forward<T> (c)).iter (); }

  /* Specialization for C arrays. */

  سانچہ <قسم_نام Type> اندرونی hb_array_t<Type>
  عامل () (Type *array, غیرعلامتی صحیح لمبائی) ثابت
  { واپس hb_array_t<Type> (array, لمبائی); }

  سانچہ <قسم_نام Type, غیرعلامتی صحیح لمبائی> hb_array_t<Type>
  عامل () (Type (&array)[لمبائی]) ثابت
  { واپس hb_array_t<Type> (array, لمبائی); }

}
HB_FUNCOBJ (hb_iter);
ساخت
{
  سانچہ <قسم_نام T> خودکار
  impl (T&& c, hb_ترجیح<1>) ثابت HB_RETURN (غیرعلامتی, c.len ())

  سانچہ <قسم_نام T> خودکار
  impl (T&& c, hb_ترجیح<0>) ثابت HB_RETURN (غیرعلامتی, c.len)

  عوامی:

  سانچہ <قسم_نام T> خودکار
  عامل () (T&& c) ثابت HB_RETURN (غیرعلامتی, impl (std::forward<T> (c), hb_prioritize))
}
HB_FUNCOBJ (hb_len);

/* Mixin to fill in what the subclass doesn't provide. */
سانچہ <قسم_نام iter_t, قسم_نام شے_t = قسم_نام iter_t::__item_t__>
ساخت hb_iter_fallback_mixin_t
{
  نجی:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ثابت iter_t* thiz () ثابت { واپس static_cast<ثابت iter_t *> (یہ); }
	iter_t* thiz ()       { واپس static_cast<      iter_t *> (یہ); }
  عوامی:

  /* Access: Implement __item__(), or __item_at__() if random-access. */
  شے_t __item__ () ثابت { واپس (*thiz())[0]; }
  شے_t __item_at__ (غیرعلامتی i) ثابت { واپس *(*thiz() + i); }

  /* Termination: Implement __more__(), or __len__() if random-access. */
  منطقی __more__ () ثابت { واپس منطقی (thiz()->len ()); }
  غیرعلامتی __len__ () ثابت
  { iter_t c (*thiz()); غیرعلامتی l = 0; جب (c) { c++; l++; } واپس l; }

  /* Advancing: Implement __next__(), or __forward__() if random-access. */
  خالی __next__ () { *thiz() += 1; }
  خالی __forward__ (غیرعلامتی n) { جب (*thiz() && n--) ++*thiz(); }

  /* Rewinding: Implement __prev__() or __rewind__() if bidirectional. */
  خالی __prev__ () { *thiz() -= 1; }
  خالی __rewind__ (غیرعلامتی n) { جب (*thiz() && n--) --*thiz(); }

  /* Range-based for: Implement __end__() if can be done faster,
   * and operator!=. */
  iter_t __end__ () ثابت
  {
    اگر (thiz()->is_random_access_iterator)
      واپس *thiz() + thiz()->len ();
    /* Above expression loops twice. Following loops once. */
    خودکار it = *thiz();
    جب (it) ++it;
    واپس it;
  }

  محفوظ:
  hb_iter_fallback_mixin_t () = طےشدہ;
  hb_iter_fallback_mixin_t (ثابت hb_iter_fallback_mixin_t &o HB_غیر_مستعمل) = طےشدہ;
  hb_iter_fallback_mixin_t (hb_iter_fallback_mixin_t &&o HB_غیر_مستعمل) = طےشدہ;
  hb_iter_fallback_mixin_t& عامل = (ثابت hb_iter_fallback_mixin_t &o HB_غیر_مستعمل) = طےشدہ;
  hb_iter_fallback_mixin_t& عامل = (hb_iter_fallback_mixin_t &&o HB_غیر_مستعمل) = طےشدہ;
};

سانچہ <قسم_نام iter_t, قسم_نام شے_t = قسم_نام iter_t::__item_t__>
ساخت hb_iter_with_fallback_t :
  hb_iter_t<iter_t, شے_t>,
  hb_iter_fallback_mixin_t<iter_t, شے_t>
{
  محفوظ:
  hb_iter_with_fallback_t () = طےشدہ;
  hb_iter_with_fallback_t (ثابت hb_iter_with_fallback_t &o HB_غیر_مستعمل) = طےشدہ;
  hb_iter_with_fallback_t (hb_iter_with_fallback_t &&o HB_غیر_مستعمل) = طےشدہ;
  hb_iter_with_fallback_t& عامل = (ثابت hb_iter_with_fallback_t &o HB_غیر_مستعمل) = طےشدہ;
  hb_iter_with_fallback_t& عامل = (hb_iter_with_fallback_t &&o HB_غیر_مستعمل) = طےشدہ;
};

/*
 * Meta-programming predicates.
 */

/* hb_is_iterator() / hb_is_iterator_of() */

سانچہ<قسم_نام Iter, قسم_نام شے_3>
ساخت hb_is_iterator_کا
{
  سانچہ <قسم_نام Item2 = شے_3>
  ساکن hb_true_type impl (hb_ترجیح<2>, hb_iter_t<Iter, hb_type_شناختی_عنصر<Item2>> *);
  ساکن hb_false_type impl (hb_ترجیح<0>, ثابت خالی *);

  عوامی:
  ساکن constexpr منطقی value = decltype (impl (hb_prioritize, hb_declval (Iter*)))::value;
};
#تعریف_کرو hb_is_iterator_کا(Iter, شے_3) hb_is_iterator_کا<Iter, شے_3>::value
#تعریف_کرو hb_is_iterator(Iter) hb_is_iterator_کا (Iter, قسم_نام Iter::شے_t)
#تعریف_کرو hb_is_sorted_iterator_کا(Iter, شے_3) (hb_is_iterator_کا<Iter, شے_3>::value && Iter::is_sorted_iterator)
#تعریف_کرو hb_is_sorted_iterator(Iter) hb_is_sorted_iterator_کا (Iter, قسم_نام Iter::شے_t)

/* hb_is_iterable() */

سانچہ <قسم_نام T>
ساخت hb_is_iterable
{
  نجی:

  سانچہ <قسم_نام U>
  ساکن خودکار impl (hb_ترجیح<1>) -> decltype (hb_declval (U).iter (), hb_true_type ());

  سانچہ <قسم_نام>
  ساکن hb_false_type impl (hb_ترجیح<0>);

  عوامی:
  ساکن constexpr منطقی value = decltype (impl<T> (hb_prioritize))::value;
};
#تعریف_کرو hb_is_iterable(Iterable) hb_is_iterable<Iterable>::value

/* hb_is_source_of() / hb_is_sink_of() */

سانچہ<قسم_نام Iter, قسم_نام شے_3>
ساخت hb_is_ماخذ_کا
{
  نجی:
  سانچہ <قسم_نام Iter2 = Iter,
	    hb_enable_if (hb_is_convertible (قسم_نام Iter2::شے_t, hb_جوڑو_lvalue_reference<ثابت شے_3>))>
  ساکن hb_true_type impl (hb_ترجیح<2>);
  سانچہ <قسم_نام Iter2 = Iter>
  ساکن خودکار impl (hb_ترجیح<1>) -> decltype (hb_declval (Iter2) >> hb_declval (شے_3 &), hb_true_type ());
  ساکن hb_false_type impl (hb_ترجیح<0>);

  عوامی:
  ساکن constexpr منطقی value = decltype (impl (hb_prioritize))::value;
};
#تعریف_کرو hb_is_ماخذ_کا(Iter, شے_3) hb_is_ماخذ_کا<Iter, شے_3>::value

سانچہ<قسم_نام Iter, قسم_نام شے_3>
ساخت hb_is_sink_کا
{
  نجی:
  سانچہ <قسم_نام Iter2 = Iter,
	    hb_enable_if (hb_is_convertible (قسم_نام Iter2::شے_t, hb_جوڑو_lvalue_reference<شے_3>))>
  ساکن hb_true_type impl (hb_ترجیح<2>);
  سانچہ <قسم_نام Iter2 = Iter>
  ساکن خودکار impl (hb_ترجیح<1>) -> decltype (hb_declval (Iter2) << hb_declval (شے_3), hb_true_type ());
  ساکن hb_false_type impl (hb_ترجیح<0>);

  عوامی:
  ساکن constexpr منطقی value = decltype (impl (hb_prioritize))::value;
};
#تعریف_کرو hb_is_sink_کا(Iter, شے_3) hb_is_sink_کا<Iter, شے_3>::value

/* This is commonly used, so define: */
#تعریف_کرو hb_is_sorted_ماخذ_کا(Iter, شے_3) \
	(hb_is_ماخذ_کا(Iter, شے_3) && Iter::is_sorted_iterator)


ساخت
{
  سانچہ <قسم_نام Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  غیرعلامتی عامل () (ثابت Iterable &_) ثابت { واپس hb_len (hb_iter (_)); }

  غیرعلامتی عامل () (غیرعلامتی _) ثابت { واپس _; }
}
HB_FUNCOBJ (hb_len_کا);

/* Range-based 'for' for iterables. */

سانچہ <قسم_نام Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ساکن اندرونی خودکار begin (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).begin ())

سانچہ <قسم_نام Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ساکن اندرونی خودکار انتہا (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).انتہا ())

/* begin()/end() are NOT looked up non-ADL.  So each namespace must declare them.
 * Do it for namespace OT. */
نامخلا OT {

سانچہ <قسم_نام Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ساکن اندرونی خودکار begin (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).begin ())

سانچہ <قسم_نام Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
ساکن اندرونی خودکار انتہا (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).انتہا ())

}


/*
 * Adaptors, combiners, etc.
 */

سانچہ <قسم_نام Lhs, قسم_نام Rhs,
	  hb_requires (hb_is_iterator (Lhs))>
ساکن اندرونی خودکار
عامل | (Lhs&& lhs, Rhs&& rhs) HB_AUTO_RETURN (std::forward<Rhs> (rhs) (std::forward<Lhs> (lhs)))

/* hb_map(), hb_filter(), hb_reduce() */

شمار  جماعت hb_function_sortedness_t {
  NOT_SORTED,
  RETAINS_ترتیب_کاری,
  SORTED,
};

سانچہ <قسم_نام Iter, قسم_نام Proj, hb_function_sortedness_t Sorted,
	 hb_requires (hb_is_iterator (Iter))>
ساخت hb_map_iter_t :
  hb_iter_t<hb_map_iter_t<Iter, Proj, Sorted>,
	    decltype (hb_get (hb_declval (Proj), *hb_declval (Iter)))>
{
  hb_map_iter_t (ثابت Iter& it, Proj f_) : it (it), f (f_) {}

  قسمتعریف decltype (hb_get (hb_declval (Proj), *hb_declval (Iter))) __item_t__;
  ساکن constexpr منطقی is_random_access_iterator = Iter::is_random_access_iterator;
  ساکن constexpr منطقی is_sorted_iterator =
    Sorted == hb_function_sortedness_t::SORTED ? سچ :
    Sorted == hb_function_sortedness_t::RETAINS_ترتیب_کاری ? Iter::is_sorted_iterator :
    جھوٹ;
  __item_t__ __item__ () ثابت { واپس hb_get (f.get (), *it); }
  __item_t__ __item_at__ (غیرعلامتی i) ثابت { واپس hb_get (f.get (), it[i]); }
  منطقی __more__ () ثابت { واپس منطقی (it); }
  غیرعلامتی __len__ () ثابت { واپس it.len (); }
  خالی __next__ () { ++it; }
  خالی __forward__ (غیرعلامتی n) { it += n; }
  خالی __prev__ () { --it; }
  خالی __rewind__ (غیرعلامتی n) { it -= n; }
  hb_map_iter_t __end__ () ثابت { واپس hb_map_iter_t (it.انتہا_3 (), f); }
  منطقی عامل != (ثابت hb_map_iter_t& o) ثابت
  { واپس it != o.it; }

  نجی:
  Iter it;
  تغیرپذیر hb_reference_wrapper<Proj> f;
};

سانچہ <قسم_نام Proj, hb_function_sortedness_t Sorted>
ساخت hb_map_iter_factory_t
{
  hb_map_iter_factory_t (Proj f) : f (f) {}

  سانچہ <قسم_نام Iter,
	    hb_requires (hb_is_iterator (Iter))>
  hb_map_iter_t<Iter, Proj, Sorted>
  عامل () (Iter it)
  { واپس hb_map_iter_t<Iter, Proj, Sorted> (it, f); }

  نجی:
  Proj f;
};
ساخت
{
  سانچہ <قسم_نام Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::NOT_SORTED>
  عامل () (Proj&& f) ثابت
  { واپس hb_map_iter_factory_t<Proj, hb_function_sortedness_t::NOT_SORTED> (f); }
}
HB_FUNCOBJ (hb_map);
ساخت
{
  سانچہ <قسم_نام Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::RETAINS_ترتیب_کاری>
  عامل () (Proj&& f) ثابت
  { واپس hb_map_iter_factory_t<Proj, hb_function_sortedness_t::RETAINS_ترتیب_کاری> (f); }
}
HB_FUNCOBJ (hb_map_retains_ترتیب_کاری);
ساخت
{
  سانچہ <قسم_نام Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::SORTED>
  عامل () (Proj&& f) ثابت
  { واپس hb_map_iter_factory_t<Proj, hb_function_sortedness_t::SORTED> (f); }
}
HB_FUNCOBJ (hb_map_sorted);

سانچہ <قسم_نام Iter, قسم_نام Pred, قسم_نام Proj,
	 hb_requires (hb_is_iterator (Iter))>
ساخت hb_filter_iter_t :
  hb_iter_with_fallback_t<hb_filter_iter_t<Iter, Pred, Proj>,
			  قسم_نام Iter::شے_t>
{
  hb_filter_iter_t (ثابت Iter& it_, Pred p_, Proj f_) : it (it_), p (p_), f (f_)
  { جب (it && !hb_has (p.get (), hb_get (f.get (), *it))) ++it; }

  قسمتعریف قسم_نام Iter::شے_t __item_t__;
  ساکن constexpr منطقی is_sorted_iterator = Iter::is_sorted_iterator;
  __item_t__ __item__ () ثابت { واپس *it; }
  منطقی __more__ () ثابت { واپس منطقی (it); }
  خالی __next__ () { کرو ++it; جب (it && !hb_has (p.get (), hb_get (f.get (), *it))); }
  خالی __prev__ ()
  {
    غیرعلامتی گنتی = 0;
    کرو
    {
      Iter old = it;
      --it;
      اگر (!(it != old))
      {
	it += گنتی;
	واپس;
      }
      گنتی++;
    }
    جب (!hb_has (p.get (), hb_get (f.get (), *it)));
  }
  hb_filter_iter_t __end__ () ثابت { واپس hb_filter_iter_t (it.انتہا_3 (), p, f); }
  منطقی عامل != (ثابت hb_filter_iter_t& o) ثابت
  { واپس it != o.it; }

  نجی:
  Iter it;
  تغیرپذیر hb_reference_wrapper<Pred> p;
  تغیرپذیر hb_reference_wrapper<Proj> f;
};
سانچہ <قسم_نام Pred, قسم_نام Proj>
ساخت hb_filter_iter_factory_t
{
  hb_filter_iter_factory_t (Pred p, Proj f) : p (p), f (f) {}

  سانچہ <قسم_نام Iter,
	    hb_requires (hb_is_iterator (Iter))>
  hb_filter_iter_t<Iter, Pred, Proj>
  عامل () (Iter it)
  { واپس hb_filter_iter_t<Iter, Pred, Proj> (it, p, f); }

  نجی:
  Pred p;
  Proj f;
};
ساخت
{
  سانچہ <قسم_نام Pred = decltype ((hb_شناختی_عنصر)),
	    قسم_نام Proj = decltype ((hb_شناختی_عنصر))>
  hb_filter_iter_factory_t<Pred, Proj>
  عامل () (Pred&& p = hb_شناختی_عنصر, Proj&& f = hb_شناختی_عنصر) ثابت
  { واپس hb_filter_iter_factory_t<Pred, Proj> (p, f); }
}
HB_FUNCOBJ (hb_filter);

سانچہ <قسم_نام Redu, قسم_نام InitT>
ساخت hb_reduce_t
{
  hb_reduce_t (Redu r, InitT init_قدر) : r (r), init_قدر (init_قدر) {}

  سانچہ <قسم_نام Iter,
	    hb_requires (hb_is_iterator (Iter)),
	    قسم_نام AccuT = hb_decay<decltype (hb_declval (Redu) (hb_declval (InitT), hb_declval (قسم_نام Iter::شے_t)))>>
  AccuT
  عامل () (Iter it)
  {
    AccuT value = init_قدر;
    برائے (; it; ++it)
      value = r (value, *it);
    واپس value;
  }

  نجی:
  Redu r;
  InitT init_قدر;
};
ساخت
{
  سانچہ <قسم_نام Redu, قسم_نام InitT>
  hb_reduce_t<Redu, InitT>
  عامل () (Redu&& r, InitT init_قدر) ثابت
  { واپس hb_reduce_t<Redu, InitT> (r, init_قدر); }
}
HB_FUNCOBJ (hb_reduce);


/* hb_zip() */

سانچہ <قسم_نام A, قسم_نام B>
ساخت hb_zip_iter_t :
  hb_iter_t<hb_zip_iter_t<A, B>,
	    hb_pair_t<قسم_نام A::شے_t, قسم_نام B::شے_t>>
{
  hb_zip_iter_t () {}
  hb_zip_iter_t (ثابت A& a, ثابت B& b) : a (a), b (b) {}

  قسمتعریف hb_pair_t<قسم_نام A::شے_t, قسم_نام B::شے_t> __item_t__;
  ساکن constexpr منطقی is_random_access_iterator =
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
  ساکن constexpr منطقی is_sorted_iterator = A::is_sorted_iterator;

  __item_t__ __item__ () ثابت { واپس __item_t__ (*a, *b); }
  __item_t__ __item_at__ (غیرعلامتی i) ثابت { واپس __item_t__ (a[i], b[i]); }
  منطقی __more__ () ثابت { واپس منطقی (a) && منطقی (b); }
  غیرعلامتی __len__ () ثابت { واپس hb_min (a.len (), b.len ()); }
  خالی __next__ () { ++a; ++b; }
  خالی __forward__ (غیرعلامتی n) { a += n; b += n; }
  خالی __prev__ () { --a; --b; }
  خالی __rewind__ (غیرعلامتی n) { a -= n; b -= n; }
  hb_zip_iter_t __end__ () ثابت { واپس hb_zip_iter_t (a.انتہا_3 (), b.انتہا_3 ()); }
  /* Note, we should stop if ANY of the iters reaches end.  As such two compare
   * unequal if both items are unequal, NOT if either is unequal. */
  منطقی عامل != (ثابت hb_zip_iter_t& o) ثابت
  { واپس a != o.a && b != o.b; }

  نجی:
  A a;
  B b;
};
ساخت
{ HB_PARTIALIZE(2);
  سانچہ <قسم_نام A, قسم_نام B,
	    hb_requires (hb_is_iterable (A) && hb_is_iterable (B))>
  hb_zip_iter_t<hb_iter_type<A>, hb_iter_type<B>>
  عامل () (A&& a, B&& b) ثابت
  { واپس hb_zip_iter_t<hb_iter_type<A>, hb_iter_type<B>> (hb_iter (a), hb_iter (b)); }
}
HB_FUNCOBJ (hb_zip);

/* hb_concat() */

سانچہ <قسم_نام A, قسم_نام B>
ساخت hb_concat_iter_t :
    hb_iter_t<hb_concat_iter_t<A, B>, قسم_نام A::شے_t>
{
  hb_concat_iter_t () {}
  hb_concat_iter_t (A& a, B& b) : a (a), b (b) {}
  hb_concat_iter_t (ثابت A& a, ثابت B& b) : a (a), b (b) {}


  قسمتعریف قسم_نام A::شے_t __item_t__;
  ساکن constexpr منطقی is_random_access_iterator =
    A::is_random_access_iterator &&
    B::is_random_access_iterator;
  ساکن constexpr منطقی is_sorted_iterator = جھوٹ;

  __item_t__ __item__ () ثابت
  {
    اگر (!a)
      واپس *b;
    واپس *a;
  }

  __item_t__ __item_at__ (غیرعلامتی i) ثابت
  {
    غیرعلامتی a_len = a.len ();
    اگر (i < a_len)
      واپس a[i];
    واپس b[i - a_len];
  }

  منطقی __more__ () ثابت { واپس منطقی (a) || منطقی (b); }

  غیرعلامتی __len__ () ثابت { واپس a.len () + b.len (); }

  خالی __next__ ()
  {
    اگر (a)
      ++a;
    ورنہ
      ++b;
  }

  خالی __forward__ (غیرعلامتی n)
  {
    اگر (!n) واپس;
    اگر (!is_random_access_iterator) {
      جب (n-- && *یہ) {
        (*یہ)++;
      }
      واپس;
    }

    غیرعلامتی a_len = a.len ();
    اگر (n > a_len) {
      n -= a_len;
      a.__forward__ (a_len);
      b.__forward__ (n);
    } ورنہ {
      a.__forward__ (n);
    }
  }

  hb_concat_iter_t __end__ () ثابت { واپس hb_concat_iter_t (a.انتہا_3 (), b.انتہا_3 ()); }
  منطقی عامل != (ثابت hb_concat_iter_t& o) ثابت
  {
    واپس a != o.a
        || b != o.b;
  }

  نجی:
  A a;
  B b;
};
ساخت
{ HB_PARTIALIZE(2);
  سانچہ <قسم_نام A, قسم_نام B,
	    hb_requires (hb_is_iterable (A) && hb_is_iterable (B))>
  hb_concat_iter_t<hb_iter_type<A>, hb_iter_type<B>>
  عامل () (A&& a, B&& b) ثابت
  { واپس hb_concat_iter_t<hb_iter_type<A>, hb_iter_type<B>> (hb_iter (a), hb_iter (b)); }
}
HB_FUNCOBJ (hb_concat);

/* hb_apply() */

سانچہ <قسم_نام Appl>
ساخت hb_apply_t
{
  hb_apply_t (Appl a) : a (a) {}

  سانچہ <قسم_نام Iter,
	    hb_requires (hb_is_iterator (Iter))>
  خالی عامل () (Iter it)
  {
    برائے (; it; ++it)
      (خالی) hb_invoke (a, *it);
  }

  نجی:
  Appl a;
};
ساخت
{
  سانچہ <قسم_نام Appl> hb_apply_t<Appl>
  عامل () (Appl&& a) ثابت
  { واپس hb_apply_t<Appl> (a); }

  سانچہ <قسم_نام Appl> hb_apply_t<Appl&>
  عامل () (Appl *a) ثابت
  { واپس hb_apply_t<Appl&> (*a); }
}
HB_FUNCOBJ (hb_apply);

/* hb_range()/hb_iota()/hb_repeat() */

سانچہ <قسم_نام T, قسم_نام S>
ساخت hb_range_iter_t :
  hb_iter_t<hb_range_iter_t<T, S>, T>
{
  hb_range_iter_t (T آغاز, T انتہا_4, S قدم) : v (آغاز), انتہا_4 (انتہا_for (آغاز, انتہا_4, قدم)), قدم (قدم) {}

  قسمتعریف T __item_t__;
  ساکن constexpr منطقی is_random_access_iterator = سچ;
  ساکن constexpr منطقی is_sorted_iterator = سچ;
  __item_t__ __item__ () ثابت { واپس hb_ridentity (v); }
  __item_t__ __item_at__ (غیرعلامتی j) ثابت { واپس v + j * قدم; }
  منطقی __more__ () ثابت { واپس v != انتہا_4; }
  غیرعلامتی __len__ () ثابت { واپس !قدم ? UINT_MAX : (انتہا_4 - v) / قدم; }
  خالی __next__ () { v += قدم; }
  خالی __forward__ (غیرعلامتی n) { v += n * قدم; }
  خالی __prev__ () { v -= قدم; }
  خالی __rewind__ (غیرعلامتی n) { v -= n * قدم; }
  hb_range_iter_t __end__ () ثابت { واپس hb_range_iter_t (انتہا_4, انتہا_4, قدم); }
  منطقی عامل != (ثابت hb_range_iter_t& o) ثابت
  { واپس v != o.v; }

  نجی:
  ساکن اندرونی T انتہا_for (T آغاز, T انتہا_4, S قدم)
  {
    اگر (!قدم)
      واپس انتہا_4;
    خودکار res = (انتہا_4 - آغاز) % قدم;
    اگر (!res)
      واپس انتہا_4;
    انتہا_4 += قدم - res;
    واپس انتہا_4;
  }

  نجی:
  T v;
  T انتہا_4;
  S قدم;
};
ساخت
{
  سانچہ <قسم_نام T = غیرعلامتی> hb_range_iter_t<T, غیرعلامتی>
  عامل () (T انتہا = (غیرعلامتی) -1) ثابت
  { واپس hb_range_iter_t<T, غیرعلامتی> (0, انتہا, 1u); }

  سانچہ <قسم_نام T, قسم_نام S = غیرعلامتی> hb_range_iter_t<T, S>
  عامل () (T آغاز, T انتہا, S قدم = 1u) ثابت
  { واپس hb_range_iter_t<T, S> (آغاز, انتہا, قدم); }
}
HB_FUNCOBJ (hb_range);

سانچہ <قسم_نام T, قسم_نام S>
ساخت hb_iota_iter_t :
  hb_iter_with_fallback_t<hb_iota_iter_t<T, S>, T>
{
  hb_iota_iter_t (T آغاز, S قدم) : v (آغاز), قدم (قدم) {}

  نجی:

  سانچہ <قسم_نام S2 = S>
  خودکار
  inc (hb_type_شناختی_عنصر<S2> s, hb_ترجیح<1>)
    -> hb_void_t<decltype (hb_invoke (std::forward<hb_type_شناختی_عنصر<S2>> (s),
                                      hb_declval<T&> ()))>
  { v = hb_invoke (std::forward<hb_type_شناختی_عنصر<S2>> (s), v); }

  خالی
  inc (S s, hb_ترجیح<0>)
  { v += s; }

  عوامی:

  قسمتعریف T __item_t__;
  ساکن constexpr منطقی is_random_access_iterator = سچ;
  ساکن constexpr منطقی is_sorted_iterator = سچ;
  __item_t__ __item__ () ثابت { واپس hb_ridentity (v); }
  منطقی __more__ () ثابت { واپس سچ; }
  غیرعلامتی __len__ () ثابت { واپس UINT_MAX; }
  خالی __next__ () { inc (قدم, hb_prioritize); }
  خالی __prev__ () { v -= قدم; }
  hb_iota_iter_t __end__ () ثابت { واپس *یہ; }
  منطقی عامل != (ثابت hb_iota_iter_t& o) ثابت { واپس سچ; }

  نجی:
  T v;
  S قدم;
};
ساخت
{
  سانچہ <قسم_نام T = غیرعلامتی, قسم_نام S = غیرعلامتی> hb_iota_iter_t<T, S>
  عامل () (T آغاز = 0u, S قدم = 1u) ثابت
  { واپس hb_iota_iter_t<T, S> (آغاز, قدم); }
}
HB_FUNCOBJ (hb_iota);

سانچہ <قسم_نام T>
ساخت hb_repeat_iter_t :
  hb_iter_t<hb_repeat_iter_t<T>, T>
{
  hb_repeat_iter_t (T value) : v (value) {}

  قسمتعریف T __item_t__;
  ساکن constexpr منطقی is_random_access_iterator = سچ;
  ساکن constexpr منطقی is_sorted_iterator = سچ;
  __item_t__ __item__ () ثابت { واپس v; }
  __item_t__ __item_at__ (غیرعلامتی j) ثابت { واپس v; }
  منطقی __more__ () ثابت { واپس سچ; }
  غیرعلامتی __len__ () ثابت { واپس UINT_MAX; }
  خالی __next__ () {}
  خالی __forward__ (غیرعلامتی) {}
  خالی __prev__ () {}
  خالی __rewind__ (غیرعلامتی) {}
  hb_repeat_iter_t __end__ () ثابت { واپس *یہ; }
  منطقی عامل != (ثابت hb_repeat_iter_t& o) ثابت { واپس سچ; }

  نجی:
  T v;
};
ساخت
{
  سانچہ <قسم_نام T> hb_repeat_iter_t<T>
  عامل () (T value) ثابت
  { واپس hb_repeat_iter_t<T> (value); }
}
HB_FUNCOBJ (hb_repeat);

/* hb_enumerate()/hb_take() */

ساخت
{
  سانچہ <قسم_نام Iterable,
	    قسم_نام اشاریہ = غیرعلامتی,
	    hb_requires (hb_is_iterable (Iterable))>
  خودکار عامل () (Iterable&& it, اشاریہ آغاز = 0u) ثابت HB_AUTO_RETURN
  ( hb_zip (hb_iota (آغاز), it) )
}
HB_FUNCOBJ (hb_enumerate);

ساخت
{ HB_PARTIALIZE(2);
  سانچہ <قسم_نام Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  خودکار عامل () (Iterable&& it, غیرعلامتی گنتی) ثابت HB_AUTO_RETURN
  ( hb_zip (hb_range (گنتی), it) | hb_map_retains_ترتیب_کاری (hb_دوسرا) )

  /* Specialization arrays. */

  سانچہ <قسم_نام Type> اندرونی hb_array_t<Type>
  عامل () (hb_array_t<Type> array, غیرعلامتی گنتی) ثابت
  { واپس array.sub_array (0, گنتی); }

  سانچہ <قسم_نام Type> اندرونی hb_sorted_array_t<Type>
  عامل () (hb_sorted_array_t<Type> array, غیرعلامتی گنتی) ثابت
  { واپس array.sub_array (0, گنتی); }
}
HB_FUNCOBJ (hb_take);

ساخت
{ HB_PARTIALIZE(2);
  سانچہ <قسم_نام Iter,
	    hb_requires (hb_is_iterator (Iter))>
  خودکار عامل () (Iter it, غیرعلامتی گنتی) ثابت HB_AUTO_RETURN
  (
    + hb_iota (it, hb_جوڑو (گنتی))
    | hb_map (hb_take (گنتی))
    | hb_take ((hb_len (it) + گنتی - 1) / گنتی)
  )
}
HB_FUNCOBJ (hb_chop);

/* hb_sink() */

سانچہ <قسم_نام Sink>
ساخت hb_sink_t
{
  hb_sink_t (Sink s) : s (s) {}

  سانچہ <قسم_نام Iter,
	    hb_requires (hb_is_iterator (Iter))>
  خالی عامل () (Iter it)
  {
    برائے (; it; ++it)
      s << *it;
  }

  نجی:
  Sink s;
};
ساخت
{
  سانچہ <قسم_نام Sink> hb_sink_t<Sink>
  عامل () (Sink&& s) ثابت
  { واپس hb_sink_t<Sink> (s); }

  سانچہ <قسم_نام Sink> hb_sink_t<Sink&>
  عامل () (Sink *s) ثابت
  { واپس hb_sink_t<Sink&> (*s); }
}
HB_FUNCOBJ (hb_sink);

/* hb-drain: hb_sink to void / blackhole / /dev/null. */

ساخت
{
  سانچہ <قسم_نام Iter,
	    hb_requires (hb_is_iterator (Iter))>
  خالی عامل () (Iter it) ثابت
  {
    برائے (; it; ++it)
      (خالی) *it;
  }
}
HB_FUNCOBJ (hb_drain);

/* hb_unzip(): unzip and sink to two sinks. */

سانچہ <قسم_نام Sink1, قسم_نام Sink2>
ساخت hb_unzip_t
{
  hb_unzip_t (Sink1 s1, Sink2 s2) : s1 (s1), s2 (s2) {}

  سانچہ <قسم_نام Iter,
	    hb_requires (hb_is_iterator (Iter))>
  خالی عامل () (Iter it)
  {
    برائے (; it; ++it)
    {
      ثابت خودکار &v = *it;
      s1 << v.پہلا;
      s2 << v.second;
    }
  }

  نجی:
  Sink1 s1;
  Sink2 s2;
};
ساخت
{
  سانچہ <قسم_نام Sink1, قسم_نام Sink2> hb_unzip_t<Sink1, Sink2>
  عامل () (Sink1&& s1, Sink2&& s2) ثابت
  { واپس hb_unzip_t<Sink1, Sink2> (s1, s2); }

  سانچہ <قسم_نام Sink1, قسم_نام Sink2> hb_unzip_t<Sink1&, Sink2&>
  عامل () (Sink1 *s1, Sink2 *s2) ثابت
  { واپس hb_unzip_t<Sink1&, Sink2&> (*s1, *s2); }
}
HB_FUNCOBJ (hb_unzip);


/* hb-all, hb-any, hb-none. */

ساخت
{
  سانچہ <قسم_نام Iterable,
	    قسم_نام Pred = decltype ((hb_شناختی_عنصر)),
	    قسم_نام Proj = decltype ((hb_شناختی_عنصر)),
	    hb_requires (hb_is_iterable (Iterable))>
  منطقی عامل () (Iterable&& c,
		    Pred&& p = hb_شناختی_عنصر,
		    Proj&& f = hb_شناختی_عنصر) ثابت
  {
    برائے (خودکار it = hb_iter (c); it; ++it)
      اگر (!hb_مطابقت (p, hb_get (f, *it)))
	واپس جھوٹ;
    واپس سچ;
  }
}
HB_FUNCOBJ (hb_all);
ساخت
{
  سانچہ <قسم_نام Iterable,
	    قسم_نام Pred = decltype ((hb_شناختی_عنصر)),
	    قسم_نام Proj = decltype ((hb_شناختی_عنصر)),
	    hb_requires (hb_is_iterable (Iterable))>
  منطقی عامل () (Iterable&& c,
		    Pred&& p = hb_شناختی_عنصر,
		    Proj&& f = hb_شناختی_عنصر) ثابت
  {
    برائے (خودکار it = hb_iter (c); it; ++it)
      اگر (hb_مطابقت (p, hb_get (f, *it)))
	واپس سچ;
    واپس جھوٹ;
  }
}
HB_FUNCOBJ (hb_any);
ساخت
{
  سانچہ <قسم_نام Iterable,
	    قسم_نام Pred = decltype ((hb_شناختی_عنصر)),
	    قسم_نام Proj = decltype ((hb_شناختی_عنصر)),
	    hb_requires (hb_is_iterable (Iterable))>
  منطقی عامل () (Iterable&& c,
		    Pred&& p = hb_شناختی_عنصر,
		    Proj&& f = hb_شناختی_عنصر) ثابت
  {
    برائے (خودکار it = hb_iter (c); it; ++it)
      اگر (hb_مطابقت (p, hb_get (f, *it)))
	واپس جھوٹ;
    واپس سچ;
  }
}
HB_FUNCOBJ (hb_none);

/*
 * Algorithms operating on iterators.
 */

سانچہ <قسم_نام C, قسم_نام V,
	  hb_requires (hb_is_iterable (C))>
اندرونی خالی
hb_fill (C&& c, ثابت V &v)
{
  برائے (خودکار i = hb_iter (c); i; i++)
    *i = v;
}

سانچہ <قسم_نام S, قسم_نام D>
اندرونی خالی
hb_copy (S&& is, D&& id)
{
  hb_iter (is) | hb_sink (id);
}


#اگر_ختم /* HB_ITER_HH */

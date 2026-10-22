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

#నిర్వచితం_కాకపోతే HB_ITER_HH
#నిర్వచించు HB_ITER_HH

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-algs.hh"
#అన్తర్భావ "hb-meta.hh"


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
మూస <రకం_పేరు iter_t, రకం_పేరు అంశం_3 = రకం_పేరు iter_t::__item_t__>
నిర్మాణం hb_iter_t
{
  రకనిర్వచనం అంశం_3 అంశం_t;
  constexpr గుర్తులేని get_అంశం_కొలత () స్థిరరాశి { తిరిగి hb_static_కొలత (అంశం_3); }
  స్థిర constexpr తార్కిక is_iterator = నిజం;
  స్థిర constexpr తార్కిక is_random_access_iterator = అబద్ధం;
  స్థిర constexpr తార్కిక is_sorted_iterator = అబద్ధం;
  స్థిర constexpr తార్కిక has_fast_len = అబద్ధం; // Should be checked in combination with is_random_access_iterator.

  అంతరంగ:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  స్థిరరాశి iter_t* thiz () స్థిరరాశి { తిరిగి static_cast<స్థిరరాశి iter_t *> (ఇది); }
	iter_t* thiz ()       { తిరిగి static_cast<      iter_t *> (ఇది); }
  బహిరంగ:

  /* Operators. */
  iter_t iter () స్థిరరాశి { తిరిగి *thiz(); }
  iter_t క్రియాకారకం + () స్థిరరాశి { తిరిగి *thiz(); }
  iter_t _begin () స్థిరరాశి { తిరిగి *thiz(); }
  iter_t begin () స్థిరరాశి { తిరిగి _begin (); }
  iter_t చివర_3 () స్థిరరాశి { తిరిగి thiz()->__end__ (); }
  iter_t చివర () స్థిరరాశి { తిరిగి చివర_3 (); }
  స్పష్టమైన క్రియాకారకం తార్కిక () స్థిరరాశి { తిరిగి thiz()->__more__ (); }
  గుర్తులేని len () స్థిరరాశి { తిరిగి thiz()->__len__ (); }
  /* The following can only be enabled if item_t is reference type.  Otherwise
   * it will be returning pointer to temporary rvalue. */
  మూస <రకం_పేరు T = అంశం_t,
	    hb_enable_if (std::is_reference<T>::value)>
  hb_remove_reference<అంశం_t>* క్రియాకారకం -> () స్థిరరాశి { తిరిగి std::addressof (**thiz()); }
  అంశం_t క్రియాకారకం * () స్థిరరాశి { తిరిగి thiz()->__item__ (); }
  అంశం_t క్రియాకారకం * () { తిరిగి thiz()->__item__ (); }
  అంశం_t క్రియాకారకం [] (గుర్తులేని i) స్థిరరాశి { తిరిగి thiz()->__item_at__ (i); }
  అంశం_t క్రియాకారకం [] (గుర్తులేని i) { తిరిగి thiz()->__item_at__ (i); }
  iter_t& క్రియాకారకం += (గుర్తులేని లెక్క) &  { thiz()->__forward__ (లెక్క); తిరిగి *thiz(); }
  iter_t  క్రియాకారకం += (గుర్తులేని లెక్క) && { thiz()->__forward__ (లెక్క); తిరిగి *thiz(); }
  iter_t& క్రియాకారకం ++ () &  { thiz()->__next__ (); తిరిగి *thiz(); }
  iter_t  క్రియాకారకం ++ () && { thiz()->__next__ (); తిరిగి *thiz(); }
  iter_t& క్రియాకారకం -= (గుర్తులేని లెక్క) &  { thiz()->__rewind__ (లెక్క); తిరిగి *thiz(); }
  iter_t  క్రియాకారకం -= (గుర్తులేని లెక్క) && { thiz()->__rewind__ (లెక్క); తిరిగి *thiz(); }
  iter_t& క్రియాకారకం -- () &  { thiz()->__prev__ (); తిరిగి *thiz(); }
  iter_t  క్రియాకారకం -- () && { thiz()->__prev__ (); తిరిగి *thiz(); }
  iter_t క్రియాకారకం + (గుర్తులేని లెక్క) స్థిరరాశి { స్వయంచాలక c = thiz()->iter (); c += లెక్క; తిరిగి c; }
  మిత్ర iter_t క్రియాకారకం + (గుర్తులేని లెక్క, స్థిరరాశి iter_t &it) { తిరిగి it + లెక్క; }
  iter_t క్రియాకారకం ++ (పూర్ణాంకం) { iter_t c (*thiz()); ++*thiz(); తిరిగి c; }
  iter_t క్రియాకారకం - (గుర్తులేని లెక్క) స్థిరరాశి { స్వయంచాలక c = thiz()->iter (); c -= లెక్క; తిరిగి c; }
  iter_t క్రియాకారకం -- (పూర్ణాంకం) { iter_t c (*thiz()); --*thiz(); తిరిగి c; }
  మూస <రకం_పేరు T>
  iter_t& క్రియాకారకం >> (T &v) &  { v = **thiz(); ++*thiz(); తిరిగి *thiz(); }
  మూస <రకం_పేరు T>
  iter_t  క్రియాకారకం >> (T &v) && { v = **thiz(); ++*thiz(); తిరిగి *thiz(); }
  మూస <రకం_పేరు T>
  iter_t& క్రియాకారకం << (స్థిరరాశి T v) &  { **thiz() = v; ++*thiz(); తిరిగి *thiz(); }
  మూస <రకం_పేరు T>
  iter_t  క్రియాకారకం << (స్థిరరాశి T v) && { **thiz() = v; ++*thiz(); తిరిగి *thiz(); }

  రక్షిత:
  hb_iter_t () = అప్రమేయ;
  hb_iter_t (స్థిరరాశి hb_iter_t &o HB_వాడని) = అప్రమేయ;
  hb_iter_t (hb_iter_t &&o HB_వాడని) = అప్రమేయ;
  hb_iter_t& క్రియాకారకం = (స్థిరరాశి hb_iter_t &o HB_వాడని) = అప్రమేయ;
  hb_iter_t& క్రియాకారకం = (hb_iter_t &&o HB_వాడని) = అప్రమేయ;
};

#నిర్వచించు HB_ITER_USING(Name) \
  ఉపయోగించి అంశం_t = రకం_పేరు Name::అంశం_t; \
  ఉపయోగించి Name::_begin; \
  ఉపయోగించి Name::begin; \
  ఉపయోగించి Name::చివర_3; \
  ఉపయోగించి Name::చివర; \
  ఉపయోగించి Name::get_అంశం_కొలత; \
  ఉపయోగించి Name::is_iterator; \
  ఉపయోగించి Name::iter; \
  ఉపయోగించి Name::క్రియాకారకం తార్కిక; \
  ఉపయోగించి Name::len; \
  ఉపయోగించి Name::క్రియాకారకం ->; \
  ఉపయోగించి Name::క్రియాకారకం *; \
  ఉపయోగించి Name::క్రియాకారకం []; \
  ఉపయోగించి Name::క్రియాకారకం +=; \
  ఉపయోగించి Name::క్రియాకారకం ++; \
  ఉపయోగించి Name::క్రియాకారకం -=; \
  ఉపయోగించి Name::క్రియాకారకం --; \
  ఉపయోగించి Name::క్రియాకారకం +; \
  ఉపయోగించి Name::క్రియాకారకం -; \
  ఉపయోగించి Name::క్రియాకారకం >>; \
  ఉపయోగించి Name::క్రియాకారకం <<; \
  static_assert (నిజం, "")

/* Returns iterator / item type of a type. */
మూస <రకం_పేరు Iterable>
ఉపయోగించి hb_iter_type = decltype (hb_deref (hb_declval (Iterable)).iter ());
మూస <రకం_పేరు Iterable>
ఉపయోగించి hb_అంశం_type = decltype (*hb_deref (hb_declval (Iterable)).iter ());


మూస <రకం_పేరు> నిర్మాణం hb_array_t;
మూస <రకం_పేరు> నిర్మాణం hb_sorted_array_t;

నిర్మాణం
{
  మూస <రకం_పేరు T> hb_iter_type<T>
  క్రియాకారకం () (T&& c) స్థిరరాశి
  { తిరిగి hb_deref (std::forward<T> (c)).iter (); }

  /* Specialization for C arrays. */

  మూస <రకం_పేరు Type> అంతఃవరుస hb_array_t<Type>
  క్రియాకారకం () (Type *array, గుర్తులేని పూర్ణాంకం నిడివి) స్థిరరాశి
  { తిరిగి hb_array_t<Type> (array, నిడివి); }

  మూస <రకం_పేరు Type, గుర్తులేని పూర్ణాంకం నిడివి> hb_array_t<Type>
  క్రియాకారకం () (Type (&array)[నిడివి]) స్థిరరాశి
  { తిరిగి hb_array_t<Type> (array, నిడివి); }

}
HB_FUNCOBJ (hb_iter);
నిర్మాణం
{
  మూస <రకం_పేరు T> స్వయంచాలక
  impl (T&& c, hb_ప్రాధాన్యత<1>) స్థిరరాశి HB_RETURN (గుర్తులేని, c.len ())

  మూస <రకం_పేరు T> స్వయంచాలక
  impl (T&& c, hb_ప్రాధాన్యత<0>) స్థిరరాశి HB_RETURN (గుర్తులేని, c.len)

  బహిరంగ:

  మూస <రకం_పేరు T> స్వయంచాలక
  క్రియాకారకం () (T&& c) స్థిరరాశి HB_RETURN (గుర్తులేని, impl (std::forward<T> (c), hb_prioritize))
}
HB_FUNCOBJ (hb_len);

/* Mixin to fill in what the subclass doesn't provide. */
మూస <రకం_పేరు iter_t, రకం_పేరు అంశం_t = రకం_పేరు iter_t::__item_t__>
నిర్మాణం hb_iter_fallback_mixin_t
{
  అంతరంగ:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  స్థిరరాశి iter_t* thiz () స్థిరరాశి { తిరిగి static_cast<స్థిరరాశి iter_t *> (ఇది); }
	iter_t* thiz ()       { తిరిగి static_cast<      iter_t *> (ఇది); }
  బహిరంగ:

  /* Access: Implement __item__(), or __item_at__() if random-access. */
  అంశం_t __item__ () స్థిరరాశి { తిరిగి (*thiz())[0]; }
  అంశం_t __item_at__ (గుర్తులేని i) స్థిరరాశి { తిరిగి *(*thiz() + i); }

  /* Termination: Implement __more__(), or __len__() if random-access. */
  తార్కిక __more__ () స్థిరరాశి { తిరిగి తార్కిక (thiz()->len ()); }
  గుర్తులేని __len__ () స్థిరరాశి
  { iter_t c (*thiz()); గుర్తులేని l = 0; అయితే (c) { c++; l++; } తిరిగి l; }

  /* Advancing: Implement __next__(), or __forward__() if random-access. */
  శూన్య __next__ () { *thiz() += 1; }
  శూన్య __forward__ (గుర్తులేని n) { అయితే (*thiz() && n--) ++*thiz(); }

  /* Rewinding: Implement __prev__() or __rewind__() if bidirectional. */
  శూన్య __prev__ () { *thiz() -= 1; }
  శూన్య __rewind__ (గుర్తులేని n) { అయితే (*thiz() && n--) --*thiz(); }

  /* Range-based for: Implement __end__() if can be done faster,
   * and operator!=. */
  iter_t __end__ () స్థిరరాశి
  {
    ఒకవేళ (thiz()->is_random_access_iterator)
      తిరిగి *thiz() + thiz()->len ();
    /* Above expression loops twice. Following loops once. */
    స్వయంచాలక it = *thiz();
    అయితే (it) ++it;
    తిరిగి it;
  }

  రక్షిత:
  hb_iter_fallback_mixin_t () = అప్రమేయ;
  hb_iter_fallback_mixin_t (స్థిరరాశి hb_iter_fallback_mixin_t &o HB_వాడని) = అప్రమేయ;
  hb_iter_fallback_mixin_t (hb_iter_fallback_mixin_t &&o HB_వాడని) = అప్రమేయ;
  hb_iter_fallback_mixin_t& క్రియాకారకం = (స్థిరరాశి hb_iter_fallback_mixin_t &o HB_వాడని) = అప్రమేయ;
  hb_iter_fallback_mixin_t& క్రియాకారకం = (hb_iter_fallback_mixin_t &&o HB_వాడని) = అప్రమేయ;
};

మూస <రకం_పేరు iter_t, రకం_పేరు అంశం_t = రకం_పేరు iter_t::__item_t__>
నిర్మాణం hb_iter_with_fallback_t :
  hb_iter_t<iter_t, అంశం_t>,
  hb_iter_fallback_mixin_t<iter_t, అంశం_t>
{
  రక్షిత:
  hb_iter_with_fallback_t () = అప్రమేయ;
  hb_iter_with_fallback_t (స్థిరరాశి hb_iter_with_fallback_t &o HB_వాడని) = అప్రమేయ;
  hb_iter_with_fallback_t (hb_iter_with_fallback_t &&o HB_వాడని) = అప్రమేయ;
  hb_iter_with_fallback_t& క్రియాకారకం = (స్థిరరాశి hb_iter_with_fallback_t &o HB_వాడని) = అప్రమేయ;
  hb_iter_with_fallback_t& క్రియాకారకం = (hb_iter_with_fallback_t &&o HB_వాడని) = అప్రమేయ;
};

/*
 * Meta-programming predicates.
 */

/* hb_is_iterator() / hb_is_iterator_of() */

మూస<రకం_పేరు Iter, రకం_పేరు అంశం_3>
నిర్మాణం hb_is_iterator_యొక్క
{
  మూస <రకం_పేరు Item2 = అంశం_3>
  స్థిర hb_true_type impl (hb_ప్రాధాన్యత<2>, hb_iter_t<Iter, hb_type_తటస్థం<Item2>> *);
  స్థిర hb_false_type impl (hb_ప్రాధాన్యత<0>, స్థిరరాశి శూన్య *);

  బహిరంగ:
  స్థిర constexpr తార్కిక value = decltype (impl (hb_prioritize, hb_declval (Iter*)))::value;
};
#నిర్వచించు hb_is_iterator_యొక్క(Iter, అంశం_3) hb_is_iterator_యొక్క<Iter, అంశం_3>::value
#నిర్వచించు hb_is_iterator(Iter) hb_is_iterator_యొక్క (Iter, రకం_పేరు Iter::అంశం_t)
#నిర్వచించు hb_is_sorted_iterator_యొక్క(Iter, అంశం_3) (hb_is_iterator_యొక్క<Iter, అంశం_3>::value && Iter::is_sorted_iterator)
#నిర్వచించు hb_is_sorted_iterator(Iter) hb_is_sorted_iterator_యొక్క (Iter, రకం_పేరు Iter::అంశం_t)

/* hb_is_iterable() */

మూస <రకం_పేరు T>
నిర్మాణం hb_is_iterable
{
  అంతరంగ:

  మూస <రకం_పేరు U>
  స్థిర స్వయంచాలక impl (hb_ప్రాధాన్యత<1>) -> decltype (hb_declval (U).iter (), hb_true_type ());

  మూస <రకం_పేరు>
  స్థిర hb_false_type impl (hb_ప్రాధాన్యత<0>);

  బహిరంగ:
  స్థిర constexpr తార్కిక value = decltype (impl<T> (hb_prioritize))::value;
};
#నిర్వచించు hb_is_iterable(Iterable) hb_is_iterable<Iterable>::value

/* hb_is_source_of() / hb_is_sink_of() */

మూస<రకం_పేరు Iter, రకం_పేరు అంశం_3>
నిర్మాణం hb_is_ఆధారం_యొక్క
{
  అంతరంగ:
  మూస <రకం_పేరు Iter2 = Iter,
	    hb_enable_if (hb_is_convertible (రకం_పేరు Iter2::అంశం_t, hb_కూడు_lvalue_reference<స్థిరరాశి అంశం_3>))>
  స్థిర hb_true_type impl (hb_ప్రాధాన్యత<2>);
  మూస <రకం_పేరు Iter2 = Iter>
  స్థిర స్వయంచాలక impl (hb_ప్రాధాన్యత<1>) -> decltype (hb_declval (Iter2) >> hb_declval (అంశం_3 &), hb_true_type ());
  స్థిర hb_false_type impl (hb_ప్రాధాన్యత<0>);

  బహిరంగ:
  స్థిర constexpr తార్కిక value = decltype (impl (hb_prioritize))::value;
};
#నిర్వచించు hb_is_ఆధారం_యొక్క(Iter, అంశం_3) hb_is_ఆధారం_యొక్క<Iter, అంశం_3>::value

మూస<రకం_పేరు Iter, రకం_పేరు అంశం_3>
నిర్మాణం hb_is_sink_యొక్క
{
  అంతరంగ:
  మూస <రకం_పేరు Iter2 = Iter,
	    hb_enable_if (hb_is_convertible (రకం_పేరు Iter2::అంశం_t, hb_కూడు_lvalue_reference<అంశం_3>))>
  స్థిర hb_true_type impl (hb_ప్రాధాన్యత<2>);
  మూస <రకం_పేరు Iter2 = Iter>
  స్థిర స్వయంచాలక impl (hb_ప్రాధాన్యత<1>) -> decltype (hb_declval (Iter2) << hb_declval (అంశం_3), hb_true_type ());
  స్థిర hb_false_type impl (hb_ప్రాధాన్యత<0>);

  బహిరంగ:
  స్థిర constexpr తార్కిక value = decltype (impl (hb_prioritize))::value;
};
#నిర్వచించు hb_is_sink_యొక్క(Iter, అంశం_3) hb_is_sink_యొక్క<Iter, అంశం_3>::value

/* This is commonly used, so define: */
#నిర్వచించు hb_is_sorted_ఆధారం_యొక్క(Iter, అంశం_3) \
	(hb_is_ఆధారం_యొక్క(Iter, అంశం_3) && Iter::is_sorted_iterator)


నిర్మాణం
{
  మూస <రకం_పేరు Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  గుర్తులేని క్రియాకారకం () (స్థిరరాశి Iterable &_) స్థిరరాశి { తిరిగి hb_len (hb_iter (_)); }

  గుర్తులేని క్రియాకారకం () (గుర్తులేని _) స్థిరరాశి { తిరిగి _; }
}
HB_FUNCOBJ (hb_len_యొక్క);

/* Range-based 'for' for iterables. */

మూస <రకం_పేరు Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
స్థిర అంతఃవరుస స్వయంచాలక begin (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).begin ())

మూస <రకం_పేరు Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
స్థిర అంతఃవరుస స్వయంచాలక చివర (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).చివర ())

/* begin()/end() are NOT looked up non-ADL.  So each namespace must declare them.
 * Do it for namespace OT. */
నామాకాశ OT {

మూస <రకం_పేరు Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
స్థిర అంతఃవరుస స్వయంచాలక begin (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).begin ())

మూస <రకం_పేరు Iterable,
	  hb_requires (hb_is_iterable (Iterable))>
స్థిర అంతఃవరుస స్వయంచాలక చివర (Iterable&& iterable) HB_AUTO_RETURN (hb_iter (iterable).చివర ())

}


/*
 * Adaptors, combiners, etc.
 */

మూస <రకం_పేరు Lhs, రకం_పేరు Rhs,
	  hb_requires (hb_is_iterator (Lhs))>
స్థిర అంతఃవరుస స్వయంచాలక
క్రియాకారకం | (Lhs&& lhs, Rhs&& rhs) HB_AUTO_RETURN (std::forward<Rhs> (rhs) (std::forward<Lhs> (lhs)))

/* hb_map(), hb_filter(), hb_reduce() */

గణన  తరగతి hb_function_sortedness_t {
  NOT_SORTED,
  RETAINS_క్రమీకరణ,
  SORTED,
};

మూస <రకం_పేరు Iter, రకం_పేరు Proj, hb_function_sortedness_t Sorted,
	 hb_requires (hb_is_iterator (Iter))>
నిర్మాణం hb_map_iter_t :
  hb_iter_t<hb_map_iter_t<Iter, Proj, Sorted>,
	    decltype (hb_get (hb_declval (Proj), *hb_declval (Iter)))>
{
  hb_map_iter_t (స్థిరరాశి Iter& it, Proj f_) : it (it), f (f_) {}

  రకనిర్వచనం decltype (hb_get (hb_declval (Proj), *hb_declval (Iter))) __item_t__;
  స్థిర constexpr తార్కిక is_random_access_iterator = Iter::is_random_access_iterator;
  స్థిర constexpr తార్కిక is_sorted_iterator =
    Sorted == hb_function_sortedness_t::SORTED ? నిజం :
    Sorted == hb_function_sortedness_t::RETAINS_క్రమీకరణ ? Iter::is_sorted_iterator :
    అబద్ధం;
  __item_t__ __item__ () స్థిరరాశి { తిరిగి hb_get (f.get (), *it); }
  __item_t__ __item_at__ (గుర్తులేని i) స్థిరరాశి { తిరిగి hb_get (f.get (), it[i]); }
  తార్కిక __more__ () స్థిరరాశి { తిరిగి తార్కిక (it); }
  గుర్తులేని __len__ () స్థిరరాశి { తిరిగి it.len (); }
  శూన్య __next__ () { ++it; }
  శూన్య __forward__ (గుర్తులేని n) { it += n; }
  శూన్య __prev__ () { --it; }
  శూన్య __rewind__ (గుర్తులేని n) { it -= n; }
  hb_map_iter_t __end__ () స్థిరరాశి { తిరిగి hb_map_iter_t (it.చివర_3 (), f); }
  తార్కిక క్రియాకారకం != (స్థిరరాశి hb_map_iter_t& o) స్థిరరాశి
  { తిరిగి it != o.it; }

  అంతరంగ:
  Iter it;
  మార్చదగిన hb_reference_wrapper<Proj> f;
};

మూస <రకం_పేరు Proj, hb_function_sortedness_t Sorted>
నిర్మాణం hb_map_iter_factory_t
{
  hb_map_iter_factory_t (Proj f) : f (f) {}

  మూస <రకం_పేరు Iter,
	    hb_requires (hb_is_iterator (Iter))>
  hb_map_iter_t<Iter, Proj, Sorted>
  క్రియాకారకం () (Iter it)
  { తిరిగి hb_map_iter_t<Iter, Proj, Sorted> (it, f); }

  అంతరంగ:
  Proj f;
};
నిర్మాణం
{
  మూస <రకం_పేరు Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::NOT_SORTED>
  క్రియాకారకం () (Proj&& f) స్థిరరాశి
  { తిరిగి hb_map_iter_factory_t<Proj, hb_function_sortedness_t::NOT_SORTED> (f); }
}
HB_FUNCOBJ (hb_map);
నిర్మాణం
{
  మూస <రకం_పేరు Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::RETAINS_క్రమీకరణ>
  క్రియాకారకం () (Proj&& f) స్థిరరాశి
  { తిరిగి hb_map_iter_factory_t<Proj, hb_function_sortedness_t::RETAINS_క్రమీకరణ> (f); }
}
HB_FUNCOBJ (hb_map_retains_క్రమీకరణ);
నిర్మాణం
{
  మూస <రకం_పేరు Proj>
  hb_map_iter_factory_t<Proj, hb_function_sortedness_t::SORTED>
  క్రియాకారకం () (Proj&& f) స్థిరరాశి
  { తిరిగి hb_map_iter_factory_t<Proj, hb_function_sortedness_t::SORTED> (f); }
}
HB_FUNCOBJ (hb_map_sorted);

మూస <రకం_పేరు Iter, రకం_పేరు Pred, రకం_పేరు Proj,
	 hb_requires (hb_is_iterator (Iter))>
నిర్మాణం hb_filter_iter_t :
  hb_iter_with_fallback_t<hb_filter_iter_t<Iter, Pred, Proj>,
			  రకం_పేరు Iter::అంశం_t>
{
  hb_filter_iter_t (స్థిరరాశి Iter& it_, Pred p_, Proj f_) : it (it_), p (p_), f (f_)
  { అయితే (it && !hb_has (p.get (), hb_get (f.get (), *it))) ++it; }

  రకనిర్వచనం రకం_పేరు Iter::అంశం_t __item_t__;
  స్థిర constexpr తార్కిక is_sorted_iterator = Iter::is_sorted_iterator;
  __item_t__ __item__ () స్థిరరాశి { తిరిగి *it; }
  తార్కిక __more__ () స్థిరరాశి { తిరిగి తార్కిక (it); }
  శూన్య __next__ () { చేయి ++it; అయితే (it && !hb_has (p.get (), hb_get (f.get (), *it))); }
  శూన్య __prev__ ()
  {
    గుర్తులేని లెక్క = 0;
    చేయి
    {
      Iter old = it;
      --it;
      ఒకవేళ (!(it != old))
      {
	it += లెక్క;
	తిరిగి;
      }
      లెక్క++;
    }
    అయితే (!hb_has (p.get (), hb_get (f.get (), *it)));
  }
  hb_filter_iter_t __end__ () స్థిరరాశి { తిరిగి hb_filter_iter_t (it.చివర_3 (), p, f); }
  తార్కిక క్రియాకారకం != (స్థిరరాశి hb_filter_iter_t& o) స్థిరరాశి
  { తిరిగి it != o.it; }

  అంతరంగ:
  Iter it;
  మార్చదగిన hb_reference_wrapper<Pred> p;
  మార్చదగిన hb_reference_wrapper<Proj> f;
};
మూస <రకం_పేరు Pred, రకం_పేరు Proj>
నిర్మాణం hb_filter_iter_factory_t
{
  hb_filter_iter_factory_t (Pred p, Proj f) : p (p), f (f) {}

  మూస <రకం_పేరు Iter,
	    hb_requires (hb_is_iterator (Iter))>
  hb_filter_iter_t<Iter, Pred, Proj>
  క్రియాకారకం () (Iter it)
  { తిరిగి hb_filter_iter_t<Iter, Pred, Proj> (it, p, f); }

  అంతరంగ:
  Pred p;
  Proj f;
};
నిర్మాణం
{
  మూస <రకం_పేరు Pred = decltype ((hb_తటస్థం)),
	    రకం_పేరు Proj = decltype ((hb_తటస్థం))>
  hb_filter_iter_factory_t<Pred, Proj>
  క్రియాకారకం () (Pred&& p = hb_తటస్థం, Proj&& f = hb_తటస్థం) స్థిరరాశి
  { తిరిగి hb_filter_iter_factory_t<Pred, Proj> (p, f); }
}
HB_FUNCOBJ (hb_filter);

మూస <రకం_పేరు Redu, రకం_పేరు InitT>
నిర్మాణం hb_reduce_t
{
  hb_reduce_t (Redu r, InitT init_విలువ) : r (r), init_విలువ (init_విలువ) {}

  మూస <రకం_పేరు Iter,
	    hb_requires (hb_is_iterator (Iter)),
	    రకం_పేరు AccuT = hb_decay<decltype (hb_declval (Redu) (hb_declval (InitT), hb_declval (రకం_పేరు Iter::అంశం_t)))>>
  AccuT
  క్రియాకారకం () (Iter it)
  {
    AccuT value = init_విలువ;
    కొరకు (; it; ++it)
      value = r (value, *it);
    తిరిగి value;
  }

  అంతరంగ:
  Redu r;
  InitT init_విలువ;
};
నిర్మాణం
{
  మూస <రకం_పేరు Redu, రకం_పేరు InitT>
  hb_reduce_t<Redu, InitT>
  క్రియాకారకం () (Redu&& r, InitT init_విలువ) స్థిరరాశి
  { తిరిగి hb_reduce_t<Redu, InitT> (r, init_విలువ); }
}
HB_FUNCOBJ (hb_reduce);


/* hb_zip() */

మూస <రకం_పేరు A, రకం_పేరు B>
నిర్మాణం hb_zip_iter_t :
  hb_iter_t<hb_zip_iter_t<A, B>,
	    hb_pair_t<రకం_పేరు A::అంశం_t, రకం_పేరు B::అంశం_t>>
{
  hb_zip_iter_t () {}
  hb_zip_iter_t (స్థిరరాశి A& a, స్థిరరాశి B& b) : a (a), b (b) {}

  రకనిర్వచనం hb_pair_t<రకం_పేరు A::అంశం_t, రకం_పేరు B::అంశం_t> __item_t__;
  స్థిర constexpr తార్కిక is_random_access_iterator =
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
  స్థిర constexpr తార్కిక is_sorted_iterator = A::is_sorted_iterator;

  __item_t__ __item__ () స్థిరరాశి { తిరిగి __item_t__ (*a, *b); }
  __item_t__ __item_at__ (గుర్తులేని i) స్థిరరాశి { తిరిగి __item_t__ (a[i], b[i]); }
  తార్కిక __more__ () స్థిరరాశి { తిరిగి తార్కిక (a) && తార్కిక (b); }
  గుర్తులేని __len__ () స్థిరరాశి { తిరిగి hb_min (a.len (), b.len ()); }
  శూన్య __next__ () { ++a; ++b; }
  శూన్య __forward__ (గుర్తులేని n) { a += n; b += n; }
  శూన్య __prev__ () { --a; --b; }
  శూన్య __rewind__ (గుర్తులేని n) { a -= n; b -= n; }
  hb_zip_iter_t __end__ () స్థిరరాశి { తిరిగి hb_zip_iter_t (a.చివర_3 (), b.చివర_3 ()); }
  /* Note, we should stop if ANY of the iters reaches end.  As such two compare
   * unequal if both items are unequal, NOT if either is unequal. */
  తార్కిక క్రియాకారకం != (స్థిరరాశి hb_zip_iter_t& o) స్థిరరాశి
  { తిరిగి a != o.a && b != o.b; }

  అంతరంగ:
  A a;
  B b;
};
నిర్మాణం
{ HB_PARTIALIZE(2);
  మూస <రకం_పేరు A, రకం_పేరు B,
	    hb_requires (hb_is_iterable (A) && hb_is_iterable (B))>
  hb_zip_iter_t<hb_iter_type<A>, hb_iter_type<B>>
  క్రియాకారకం () (A&& a, B&& b) స్థిరరాశి
  { తిరిగి hb_zip_iter_t<hb_iter_type<A>, hb_iter_type<B>> (hb_iter (a), hb_iter (b)); }
}
HB_FUNCOBJ (hb_zip);

/* hb_concat() */

మూస <రకం_పేరు A, రకం_పేరు B>
నిర్మాణం hb_concat_iter_t :
    hb_iter_t<hb_concat_iter_t<A, B>, రకం_పేరు A::అంశం_t>
{
  hb_concat_iter_t () {}
  hb_concat_iter_t (A& a, B& b) : a (a), b (b) {}
  hb_concat_iter_t (స్థిరరాశి A& a, స్థిరరాశి B& b) : a (a), b (b) {}


  రకనిర్వచనం రకం_పేరు A::అంశం_t __item_t__;
  స్థిర constexpr తార్కిక is_random_access_iterator =
    A::is_random_access_iterator &&
    B::is_random_access_iterator;
  స్థిర constexpr తార్కిక is_sorted_iterator = అబద్ధం;

  __item_t__ __item__ () స్థిరరాశి
  {
    ఒకవేళ (!a)
      తిరిగి *b;
    తిరిగి *a;
  }

  __item_t__ __item_at__ (గుర్తులేని i) స్థిరరాశి
  {
    గుర్తులేని a_len = a.len ();
    ఒకవేళ (i < a_len)
      తిరిగి a[i];
    తిరిగి b[i - a_len];
  }

  తార్కిక __more__ () స్థిరరాశి { తిరిగి తార్కిక (a) || తార్కిక (b); }

  గుర్తులేని __len__ () స్థిరరాశి { తిరిగి a.len () + b.len (); }

  శూన్య __next__ ()
  {
    ఒకవేళ (a)
      ++a;
    లేకపోతే
      ++b;
  }

  శూన్య __forward__ (గుర్తులేని n)
  {
    ఒకవేళ (!n) తిరిగి;
    ఒకవేళ (!is_random_access_iterator) {
      అయితే (n-- && *ఇది) {
        (*ఇది)++;
      }
      తిరిగి;
    }

    గుర్తులేని a_len = a.len ();
    ఒకవేళ (n > a_len) {
      n -= a_len;
      a.__forward__ (a_len);
      b.__forward__ (n);
    } లేకపోతే {
      a.__forward__ (n);
    }
  }

  hb_concat_iter_t __end__ () స్థిరరాశి { తిరిగి hb_concat_iter_t (a.చివర_3 (), b.చివర_3 ()); }
  తార్కిక క్రియాకారకం != (స్థిరరాశి hb_concat_iter_t& o) స్థిరరాశి
  {
    తిరిగి a != o.a
        || b != o.b;
  }

  అంతరంగ:
  A a;
  B b;
};
నిర్మాణం
{ HB_PARTIALIZE(2);
  మూస <రకం_పేరు A, రకం_పేరు B,
	    hb_requires (hb_is_iterable (A) && hb_is_iterable (B))>
  hb_concat_iter_t<hb_iter_type<A>, hb_iter_type<B>>
  క్రియాకారకం () (A&& a, B&& b) స్థిరరాశి
  { తిరిగి hb_concat_iter_t<hb_iter_type<A>, hb_iter_type<B>> (hb_iter (a), hb_iter (b)); }
}
HB_FUNCOBJ (hb_concat);

/* hb_apply() */

మూస <రకం_పేరు Appl>
నిర్మాణం hb_apply_t
{
  hb_apply_t (Appl a) : a (a) {}

  మూస <రకం_పేరు Iter,
	    hb_requires (hb_is_iterator (Iter))>
  శూన్య క్రియాకారకం () (Iter it)
  {
    కొరకు (; it; ++it)
      (శూన్య) hb_invoke (a, *it);
  }

  అంతరంగ:
  Appl a;
};
నిర్మాణం
{
  మూస <రకం_పేరు Appl> hb_apply_t<Appl>
  క్రియాకారకం () (Appl&& a) స్థిరరాశి
  { తిరిగి hb_apply_t<Appl> (a); }

  మూస <రకం_పేరు Appl> hb_apply_t<Appl&>
  క్రియాకారకం () (Appl *a) స్థిరరాశి
  { తిరిగి hb_apply_t<Appl&> (*a); }
}
HB_FUNCOBJ (hb_apply);

/* hb_range()/hb_iota()/hb_repeat() */

మూస <రకం_పేరు T, రకం_పేరు S>
నిర్మాణం hb_range_iter_t :
  hb_iter_t<hb_range_iter_t<T, S>, T>
{
  hb_range_iter_t (T ప్రారంభం, T చివర_4, S అడుగు) : v (ప్రారంభం), చివర_4 (చివర_for (ప్రారంభం, చివర_4, అడుగు)), అడుగు (అడుగు) {}

  రకనిర్వచనం T __item_t__;
  స్థిర constexpr తార్కిక is_random_access_iterator = నిజం;
  స్థిర constexpr తార్కిక is_sorted_iterator = నిజం;
  __item_t__ __item__ () స్థిరరాశి { తిరిగి hb_ridentity (v); }
  __item_t__ __item_at__ (గుర్తులేని j) స్థిరరాశి { తిరిగి v + j * అడుగు; }
  తార్కిక __more__ () స్థిరరాశి { తిరిగి v != చివర_4; }
  గుర్తులేని __len__ () స్థిరరాశి { తిరిగి !అడుగు ? UINT_MAX : (చివర_4 - v) / అడుగు; }
  శూన్య __next__ () { v += అడుగు; }
  శూన్య __forward__ (గుర్తులేని n) { v += n * అడుగు; }
  శూన్య __prev__ () { v -= అడుగు; }
  శూన్య __rewind__ (గుర్తులేని n) { v -= n * అడుగు; }
  hb_range_iter_t __end__ () స్థిరరాశి { తిరిగి hb_range_iter_t (చివర_4, చివర_4, అడుగు); }
  తార్కిక క్రియాకారకం != (స్థిరరాశి hb_range_iter_t& o) స్థిరరాశి
  { తిరిగి v != o.v; }

  అంతరంగ:
  స్థిర అంతఃవరుస T చివర_for (T ప్రారంభం, T చివర_4, S అడుగు)
  {
    ఒకవేళ (!అడుగు)
      తిరిగి చివర_4;
    స్వయంచాలక res = (చివర_4 - ప్రారంభం) % అడుగు;
    ఒకవేళ (!res)
      తిరిగి చివర_4;
    చివర_4 += అడుగు - res;
    తిరిగి చివర_4;
  }

  అంతరంగ:
  T v;
  T చివర_4;
  S అడుగు;
};
నిర్మాణం
{
  మూస <రకం_పేరు T = గుర్తులేని> hb_range_iter_t<T, గుర్తులేని>
  క్రియాకారకం () (T చివర = (గుర్తులేని) -1) స్థిరరాశి
  { తిరిగి hb_range_iter_t<T, గుర్తులేని> (0, చివర, 1u); }

  మూస <రకం_పేరు T, రకం_పేరు S = గుర్తులేని> hb_range_iter_t<T, S>
  క్రియాకారకం () (T ప్రారంభం, T చివర, S అడుగు = 1u) స్థిరరాశి
  { తిరిగి hb_range_iter_t<T, S> (ప్రారంభం, చివర, అడుగు); }
}
HB_FUNCOBJ (hb_range);

మూస <రకం_పేరు T, రకం_పేరు S>
నిర్మాణం hb_iota_iter_t :
  hb_iter_with_fallback_t<hb_iota_iter_t<T, S>, T>
{
  hb_iota_iter_t (T ప్రారంభం, S అడుగు) : v (ప్రారంభం), అడుగు (అడుగు) {}

  అంతరంగ:

  మూస <రకం_పేరు S2 = S>
  స్వయంచాలక
  inc (hb_type_తటస్థం<S2> s, hb_ప్రాధాన్యత<1>)
    -> hb_void_t<decltype (hb_invoke (std::forward<hb_type_తటస్థం<S2>> (s),
                                      hb_declval<T&> ()))>
  { v = hb_invoke (std::forward<hb_type_తటస్థం<S2>> (s), v); }

  శూన్య
  inc (S s, hb_ప్రాధాన్యత<0>)
  { v += s; }

  బహిరంగ:

  రకనిర్వచనం T __item_t__;
  స్థిర constexpr తార్కిక is_random_access_iterator = నిజం;
  స్థిర constexpr తార్కిక is_sorted_iterator = నిజం;
  __item_t__ __item__ () స్థిరరాశి { తిరిగి hb_ridentity (v); }
  తార్కిక __more__ () స్థిరరాశి { తిరిగి నిజం; }
  గుర్తులేని __len__ () స్థిరరాశి { తిరిగి UINT_MAX; }
  శూన్య __next__ () { inc (అడుగు, hb_prioritize); }
  శూన్య __prev__ () { v -= అడుగు; }
  hb_iota_iter_t __end__ () స్థిరరాశి { తిరిగి *ఇది; }
  తార్కిక క్రియాకారకం != (స్థిరరాశి hb_iota_iter_t& o) స్థిరరాశి { తిరిగి నిజం; }

  అంతరంగ:
  T v;
  S అడుగు;
};
నిర్మాణం
{
  మూస <రకం_పేరు T = గుర్తులేని, రకం_పేరు S = గుర్తులేని> hb_iota_iter_t<T, S>
  క్రియాకారకం () (T ప్రారంభం = 0u, S అడుగు = 1u) స్థిరరాశి
  { తిరిగి hb_iota_iter_t<T, S> (ప్రారంభం, అడుగు); }
}
HB_FUNCOBJ (hb_iota);

మూస <రకం_పేరు T>
నిర్మాణం hb_repeat_iter_t :
  hb_iter_t<hb_repeat_iter_t<T>, T>
{
  hb_repeat_iter_t (T value) : v (value) {}

  రకనిర్వచనం T __item_t__;
  స్థిర constexpr తార్కిక is_random_access_iterator = నిజం;
  స్థిర constexpr తార్కిక is_sorted_iterator = నిజం;
  __item_t__ __item__ () స్థిరరాశి { తిరిగి v; }
  __item_t__ __item_at__ (గుర్తులేని j) స్థిరరాశి { తిరిగి v; }
  తార్కిక __more__ () స్థిరరాశి { తిరిగి నిజం; }
  గుర్తులేని __len__ () స్థిరరాశి { తిరిగి UINT_MAX; }
  శూన్య __next__ () {}
  శూన్య __forward__ (గుర్తులేని) {}
  శూన్య __prev__ () {}
  శూన్య __rewind__ (గుర్తులేని) {}
  hb_repeat_iter_t __end__ () స్థిరరాశి { తిరిగి *ఇది; }
  తార్కిక క్రియాకారకం != (స్థిరరాశి hb_repeat_iter_t& o) స్థిరరాశి { తిరిగి నిజం; }

  అంతరంగ:
  T v;
};
నిర్మాణం
{
  మూస <రకం_పేరు T> hb_repeat_iter_t<T>
  క్రియాకారకం () (T value) స్థిరరాశి
  { తిరిగి hb_repeat_iter_t<T> (value); }
}
HB_FUNCOBJ (hb_repeat);

/* hb_enumerate()/hb_take() */

నిర్మాణం
{
  మూస <రకం_పేరు Iterable,
	    రకం_పేరు సూచీ = గుర్తులేని,
	    hb_requires (hb_is_iterable (Iterable))>
  స్వయంచాలక క్రియాకారకం () (Iterable&& it, సూచీ ప్రారంభం = 0u) స్థిరరాశి HB_AUTO_RETURN
  ( hb_zip (hb_iota (ప్రారంభం), it) )
}
HB_FUNCOBJ (hb_enumerate);

నిర్మాణం
{ HB_PARTIALIZE(2);
  మూస <రకం_పేరు Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  స్వయంచాలక క్రియాకారకం () (Iterable&& it, గుర్తులేని లెక్క) స్థిరరాశి HB_AUTO_RETURN
  ( hb_zip (hb_range (లెక్క), it) | hb_map_retains_క్రమీకరణ (hb_రెండవ) )

  /* Specialization arrays. */

  మూస <రకం_పేరు Type> అంతఃవరుస hb_array_t<Type>
  క్రియాకారకం () (hb_array_t<Type> array, గుర్తులేని లెక్క) స్థిరరాశి
  { తిరిగి array.sub_array (0, లెక్క); }

  మూస <రకం_పేరు Type> అంతఃవరుస hb_sorted_array_t<Type>
  క్రియాకారకం () (hb_sorted_array_t<Type> array, గుర్తులేని లెక్క) స్థిరరాశి
  { తిరిగి array.sub_array (0, లెక్క); }
}
HB_FUNCOBJ (hb_take);

నిర్మాణం
{ HB_PARTIALIZE(2);
  మూస <రకం_పేరు Iter,
	    hb_requires (hb_is_iterator (Iter))>
  స్వయంచాలక క్రియాకారకం () (Iter it, గుర్తులేని లెక్క) స్థిరరాశి HB_AUTO_RETURN
  (
    + hb_iota (it, hb_కూడు (లెక్క))
    | hb_map (hb_take (లెక్క))
    | hb_take ((hb_len (it) + లెక్క - 1) / లెక్క)
  )
}
HB_FUNCOBJ (hb_chop);

/* hb_sink() */

మూస <రకం_పేరు Sink>
నిర్మాణం hb_sink_t
{
  hb_sink_t (Sink s) : s (s) {}

  మూస <రకం_పేరు Iter,
	    hb_requires (hb_is_iterator (Iter))>
  శూన్య క్రియాకారకం () (Iter it)
  {
    కొరకు (; it; ++it)
      s << *it;
  }

  అంతరంగ:
  Sink s;
};
నిర్మాణం
{
  మూస <రకం_పేరు Sink> hb_sink_t<Sink>
  క్రియాకారకం () (Sink&& s) స్థిరరాశి
  { తిరిగి hb_sink_t<Sink> (s); }

  మూస <రకం_పేరు Sink> hb_sink_t<Sink&>
  క్రియాకారకం () (Sink *s) స్థిరరాశి
  { తిరిగి hb_sink_t<Sink&> (*s); }
}
HB_FUNCOBJ (hb_sink);

/* hb-drain: hb_sink to void / blackhole / /dev/null. */

నిర్మాణం
{
  మూస <రకం_పేరు Iter,
	    hb_requires (hb_is_iterator (Iter))>
  శూన్య క్రియాకారకం () (Iter it) స్థిరరాశి
  {
    కొరకు (; it; ++it)
      (శూన్య) *it;
  }
}
HB_FUNCOBJ (hb_drain);

/* hb_unzip(): unzip and sink to two sinks. */

మూస <రకం_పేరు Sink1, రకం_పేరు Sink2>
నిర్మాణం hb_unzip_t
{
  hb_unzip_t (Sink1 s1, Sink2 s2) : s1 (s1), s2 (s2) {}

  మూస <రకం_పేరు Iter,
	    hb_requires (hb_is_iterator (Iter))>
  శూన్య క్రియాకారకం () (Iter it)
  {
    కొరకు (; it; ++it)
    {
      స్థిరరాశి స్వయంచాలక &v = *it;
      s1 << v.మొదటి;
      s2 << v.second;
    }
  }

  అంతరంగ:
  Sink1 s1;
  Sink2 s2;
};
నిర్మాణం
{
  మూస <రకం_పేరు Sink1, రకం_పేరు Sink2> hb_unzip_t<Sink1, Sink2>
  క్రియాకారకం () (Sink1&& s1, Sink2&& s2) స్థిరరాశి
  { తిరిగి hb_unzip_t<Sink1, Sink2> (s1, s2); }

  మూస <రకం_పేరు Sink1, రకం_పేరు Sink2> hb_unzip_t<Sink1&, Sink2&>
  క్రియాకారకం () (Sink1 *s1, Sink2 *s2) స్థిరరాశి
  { తిరిగి hb_unzip_t<Sink1&, Sink2&> (*s1, *s2); }
}
HB_FUNCOBJ (hb_unzip);


/* hb-all, hb-any, hb-none. */

నిర్మాణం
{
  మూస <రకం_పేరు Iterable,
	    రకం_పేరు Pred = decltype ((hb_తటస్థం)),
	    రకం_పేరు Proj = decltype ((hb_తటస్థం)),
	    hb_requires (hb_is_iterable (Iterable))>
  తార్కిక క్రియాకారకం () (Iterable&& c,
		    Pred&& p = hb_తటస్థం,
		    Proj&& f = hb_తటస్థం) స్థిరరాశి
  {
    కొరకు (స్వయంచాలక it = hb_iter (c); it; ++it)
      ఒకవేళ (!hb_సరిపోలిక (p, hb_get (f, *it)))
	తిరిగి అబద్ధం;
    తిరిగి నిజం;
  }
}
HB_FUNCOBJ (hb_all);
నిర్మాణం
{
  మూస <రకం_పేరు Iterable,
	    రకం_పేరు Pred = decltype ((hb_తటస్థం)),
	    రకం_పేరు Proj = decltype ((hb_తటస్థం)),
	    hb_requires (hb_is_iterable (Iterable))>
  తార్కిక క్రియాకారకం () (Iterable&& c,
		    Pred&& p = hb_తటస్థం,
		    Proj&& f = hb_తటస్థం) స్థిరరాశి
  {
    కొరకు (స్వయంచాలక it = hb_iter (c); it; ++it)
      ఒకవేళ (hb_సరిపోలిక (p, hb_get (f, *it)))
	తిరిగి నిజం;
    తిరిగి అబద్ధం;
  }
}
HB_FUNCOBJ (hb_any);
నిర్మాణం
{
  మూస <రకం_పేరు Iterable,
	    రకం_పేరు Pred = decltype ((hb_తటస్థం)),
	    రకం_పేరు Proj = decltype ((hb_తటస్థం)),
	    hb_requires (hb_is_iterable (Iterable))>
  తార్కిక క్రియాకారకం () (Iterable&& c,
		    Pred&& p = hb_తటస్థం,
		    Proj&& f = hb_తటస్థం) స్థిరరాశి
  {
    కొరకు (స్వయంచాలక it = hb_iter (c); it; ++it)
      ఒకవేళ (hb_సరిపోలిక (p, hb_get (f, *it)))
	తిరిగి అబద్ధం;
    తిరిగి నిజం;
  }
}
HB_FUNCOBJ (hb_none);

/*
 * Algorithms operating on iterators.
 */

మూస <రకం_పేరు C, రకం_పేరు V,
	  hb_requires (hb_is_iterable (C))>
అంతఃవరుస శూన్య
hb_fill (C&& c, స్థిరరాశి V &v)
{
  కొరకు (స్వయంచాలక i = hb_iter (c); i; i++)
    *i = v;
}

మూస <రకం_పేరు S, రకం_పేరు D>
అంతఃవరుస శూన్య
hb_copy (S&& is, D&& id)
{
  hb_iter (is) | hb_sink (id);
}


#అయితే_ముగింపు /* HB_ITER_HH */

/*
 * Copyright © 2018  Google, Inc.
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
 */

#నిర్వచితం_కాకపోతే HB_NULL_HH
#నిర్వచించు HB_NULL_HH

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-meta.hh"


/*
 * Static pools
 */

/* Global nul-content Null pool.  Enlarge as necessary. */

#నిర్వచించు HB_NULL_POOL_కొలత 640

మూస <రకం_పేరు T, రకం_పేరు>
నిర్మాణం hb_has_min_కొలత : hb_false_type {};
మూస <రకం_పేరు T>
నిర్మాణం hb_has_min_కొలత<T, hb_void_t<decltype (T::min_కొలత)>>
	: hb_true_type {};
మూస <రకం_పేరు T>
ఉపయోగించి hb_has_min_కొలత_2 = hb_has_min_కొలత<T, శూన్య>;
#నిర్వచించు hb_has_min_కొలత_2(T) hb_has_min_కొలత_2<T>::value

మూస <రకం_పేరు T, రకం_పేరు>
నిర్మాణం hb_has_null_కొలత : hb_false_type {};
మూస <రకం_పేరు T>
నిర్మాణం hb_has_null_కొలత<T, hb_void_t<decltype (T::null_కొలత)>>
	: hb_true_type {};
మూస <రకం_పేరు T>
ఉపయోగించి hb_has_null_కొలత_2 = hb_has_null_కొలత<T, శూన్య>;
#నిర్వచించు hb_has_null_కొలత_2(T) hb_has_null_కొలత_2<T>::value

/* Use SFINAE to sniff whether T has min_size; in which case return the larger
 * of sizeof(T) and T::null_size, otherwise return sizeof(T).
 *
 * The main purpose of this is to let structs communicate that they are not nullable,
 * by defining min_size but *not* null_size. */

/* The hard way...
 * https://stackoverflow.com/questions/7776448/sfinae-tried-with-bool-gives-compiler-error-template-argument-tvalue-invol
 */

మూస <రకం_పేరు T, రకం_పేరు>
నిర్మాణం hb_null_కొలత : hb_integral_constant<గుర్తులేని, పరిమాణం (T)> {};
మూస <రకం_పేరు T>
నిర్మాణం hb_null_కొలత<T, hb_void_t<decltype (T::min_కొలత)>>
	: hb_integral_constant<గుర్తులేని,
			       (పరిమాణం (T) > T::null_కొలత ? పరిమాణం (T) : T::null_కొలత)> {};
మూస <రకం_పేరు T>
ఉపయోగించి hb_null_కొలత_2 = hb_null_కొలత<T, శూన్య>;
#నిర్వచించు hb_null_కొలత_2(T) hb_null_కొలత_2<T>::value

/* These doesn't belong here, but since is copy/paste from above, put it here. */

/* hb_static_size (T)
 * Returns T::static_size if T::min_size is defined, or sizeof (T) otherwise. */

మూస <రకం_పేరు T, రకం_పేరు>
నిర్మాణం hb_static_కొలత_2 : hb_integral_constant<గుర్తులేని, పరిమాణం (T)> {};
మూస <రకం_పేరు T>
నిర్మాణం hb_static_కొలత_2<T, hb_void_t<decltype (T::static_కొలత)>> : hb_integral_constant<గుర్తులేని, T::static_కొలత> {};
మూస <రకం_పేరు T>
ఉపయోగించి hb_static_కొలత = hb_static_కొలత_2<T, శూన్య>;
#నిర్వచించు hb_static_కొలత(T) hb_static_కొలత<T>::value

మూస <రకం_పేరు T, రకం_పేరు>
నిర్మాణం hb_min_కొలత_2 : hb_integral_constant<గుర్తులేని, పరిమాణం (T)> {};
మూస <రకం_పేరు T>
నిర్మాణం hb_min_కొలత_2<T, hb_void_t<decltype (T::min_కొలత)>> : hb_integral_constant<గుర్తులేని, T::min_కొలత> {};
మూస <రకం_పేరు T>
ఉపయోగించి hb_min_కొలత = hb_min_కొలత_2<T, శూన్య>;
#నిర్వచించు hb_min_కొలత(T) hb_min_కొలత<T>::value


/*
 * Null()
 */

బాహ్య HB_అంతర్గత
uint64_t స్థిరరాశి _hb_NullPool[(HB_NULL_POOL_కొలత + పరిమాణం (uint64_t) - 1) / పరిమాణం (uint64_t)];

/* Generic nul-content Null objects. */
మూస <రకం_పేరు Type>
నిర్మాణం Null {
  స్థిర Type స్థిరరాశి & get_null ()
  {
    static_assert (hb_null_కొలత_2 (Type) <= HB_NULL_POOL_కొలత, "Increase HB_NULL_POOL_SIZE.");
    తిరిగి *reinterpret_cast<Type స్థిరరాశి *> (_hb_NullPool);
  }
};
మూస <రకం_పేరు QType>
నిర్మాణం NullHelper
{
  రకనిర్వచనం hb_remove_const<hb_remove_reference<QType>> Type;
  స్థిర స్థిరరాశి Type & get_null () { తిరిగి Null<Type>::get_null (); }
};
#నిర్వచించు Null(Type) NullHelper<Type>::get_null ()

/* Specializations for arbitrary-content Null objects expressed in bytes. */
#నిర్వచించు DECLARE_NULL_NAMESPACE_బైట్లు(Namespace, Type) \
	} /* Close namespace. */ \
	బాహ్య HB_అంతర్గత స్థిరరాశి గుర్తులేని అక్షరం _hb_Null_##Namespace##_##Type[hb_null_కొలత_2 (Namespace::Type)]; \
	మూస <> \
	నిర్మాణం Null<Namespace::Type> { \
	  స్థిర Namespace::Type స్థిరరాశి & get_null () { \
	    తిరిగి *reinterpret_cast<స్థిరరాశి Namespace::Type *> (_hb_Null_##Namespace##_##Type); \
	  } \
	}; \
	నామాకాశ Namespace { \
	static_assert (నిజం, "") /* Require semicolon after. */
#నిర్వచించు DECLARE_NULL_NAMESPACE_బైట్లు_TEMPLATE1(Namespace, Type, కొలత_2) \
	} /* Close namespace. */ \
	బాహ్య HB_అంతర్గత స్థిరరాశి గుర్తులేని అక్షరం _hb_Null_##Namespace##_##Type[కొలత_2]; \
	మూస <రకం_పేరు Spec> \
	నిర్మాణం Null<Namespace::Type<Spec>> { \
	  స్థిర Namespace::Type<Spec> స్థిరరాశి & get_null () { \
	    తిరిగి *reinterpret_cast<స్థిరరాశి Namespace::Type<Spec> *> (_hb_Null_##Namespace##_##Type); \
	  } \
	}; \
	నామాకాశ Namespace { \
	static_assert (నిజం, "") /* Require semicolon after. */
#నిర్వచించు DEFINE_NULL_NAMESPACE_బైట్లు(Namespace, Type) \
	స్థిరరాశి గుర్తులేని అక్షరం _hb_Null_##Namespace##_##Type[పరిమాణం (_hb_Null_##Namespace##_##Type)]

/* Specializations for arbitrary-content Null objects expressed as struct initializer. */
#నిర్వచించు DECLARE_NULL_INSTANCE(Type) \
	బాహ్య HB_అంతర్గత స్థిరరాశి Type _hb_Null_##Type; \
	మూస <> \
	నిర్మాణం Null<Type> { \
	  స్థిర Type స్థిరరాశి & get_null () { \
	    తిరిగి _hb_Null_##Type; \
	  } \
	}; \
	static_assert (నిజం, "") /* Require semicolon after. */
#నిర్వచించు DEFINE_NULL_INSTANCE(Type) \
	స్థిరరాశి Type _hb_Null_##Type

/* Global writable pool.  Enlarge as necessary. */

/* To be fully correct, CrapPool must be thread_local. However, we do not rely on CrapPool
 * for correct operation. It only exist to catch and divert program logic bugs instead of
 * causing bad memory access. So, races there are not actually introducing incorrectness
 * in the code. Has ~12kb binary size overhead to have it, also clang build fails with it. */
బాహ్య HB_అంతర్గత
/*thread_local*/ uint64_t _hb_CrapPool[(HB_NULL_POOL_కొలత + పరిమాణం (uint64_t) - 1) / పరిమాణం (uint64_t)];

/* CRAP pool: Common Region for Access Protection. */
మూస <రకం_పేరు Type>
స్థిర అంతఃవరుస Type& Crap () {
  static_assert (hb_null_కొలత_2 (Type) <= HB_NULL_POOL_కొలత, "Increase HB_NULL_POOL_SIZE.");
  Type *obj = reinterpret_cast<Type *> (_hb_CrapPool);
  జ్ఞాపకం_నకలు (reinterpret_cast<శూన్య*>(obj), std::addressof (Null (Type)), పరిమాణం (*obj));
  తిరిగి *obj;
}
మూస <రకం_పేరు QType>
నిర్మాణం CrapHelper
{
  రకనిర్వచనం hb_remove_const<hb_remove_reference<QType>> Type;
  స్థిర Type & get_crap () { తిరిగి Crap<Type> (); }
};
#నిర్వచించు Crap(Type) CrapHelper<Type>::get_crap ()

మూస <రకం_పేరు Type>
నిర్మాణం CrapOrNullHelper {
  స్థిర Type & get () { తిరిగి Crap (Type); }
};
మూస <రకం_పేరు Type>
నిర్మాణం CrapOrNullHelper<స్థిరరాశి Type> {
  స్థిర స్థిరరాశి Type & get () { తిరిగి Null (Type); }
};
#నిర్వచించు CrapOrNull(Type) CrapOrNullHelper<Type>::get ()


/*
 * hb_nonnull_ptr_t
 */

మూస <రకం_పేరు P>
నిర్మాణం hb_nonnull_ptr_t
{
  రకనిర్వచనం hb_remove_సూచిక<P> T;

  hb_nonnull_ptr_t (T *v_ = శూన్యసూచిక) : v (v_) {}
  T * క్రియాకారకం = (T *v_)   { తిరిగి v = v_; }
  T * క్రియాకారకం -> () స్థిరరాశి { తిరిగి get (); }
  T & క్రియాకారకం * () స్థిరరాశి  { తిరిగి *get (); }
  T ** క్రియాకారకం & () స్థిరరాశి { తిరిగి std::addressof (v); }
  /* Only auto-cast to const types. */
  మూస <రకం_పేరు C> క్రియాకారకం స్థిరరాశి C * () స్థిరరాశి { తిరిగి get (); }
  క్రియాకారకం స్థిరరాశి అక్షరం * () స్థిరరాశి { తిరిగి (స్థిరరాశి అక్షరం *) get (); }
  T * get () స్థిరరాశి { తిరిగి v ? v : const_cast<T *> (std::addressof (Null (T))); }
  T * get_raw () స్థిరరాశి { తిరిగి v; }

  అంతరంగ:
  T *v;
};


#అయితే_ముగింపు /* HB_NULL_HH */

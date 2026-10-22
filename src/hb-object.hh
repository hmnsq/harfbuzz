/*
 * Copyright © 2007  Chris Wilson
 * Copyright © 2009,2010  Red Hat, Inc.
 * Copyright © 2011,2012  Google, Inc.
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
 * Contributor(s):
 *	Chris Wilson <chris@chris-wilson.co.uk>
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#నిర్వచితం_కాకపోతే HB_OBJECT_HH
#నిర్వచించు HB_OBJECT_HH

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-atomic.hh"
#అన్తర్భావ "hb-mutex.hh"
#అన్తర్భావ "hb-vector.hh"


/*
 * Lockable set
 */

మూస <రకం_పేరు అంశం_t, రకం_పేరు తాళం_t>
నిర్మాణం hb_lockable_సమితి_t
{
  hb_సదిశ_t<అంశం_t> వస్తువులు;

  శూన్య init () { వస్తువులు.init (); }

  మూస <రకం_పేరు T>
  అంశం_t *replace_or_చొప్పించు (T v, తాళం_t &l, తార్కిక replace)
  {
    l.lock ();
    అంశం_t *అంశం = వస్తువులు.lsearch (v);
    ఒకవేళ (అంశం) {
      ఒకవేళ (replace) {
	అంశం_t old = *అంశం;
	*అంశం = v;
	l.unlock ();
	old.fini ();
      }
      లేకపోతే {
	అంశం = శూన్యసూచిక;
	l.unlock ();
      }
    } లేకపోతే {
      అంశం = వస్తువులు.push (v);
      l.unlock ();
    }
    తిరిగి వస్తువులు.లోపల_error () ? శూన్యసూచిక : అంశం;
  }

  మూస <రకం_పేరు T>
  శూన్య remove (T v, తాళం_t &l)
  {
    l.lock ();
    అంశం_t *అంశం = వస్తువులు.lsearch (v);
    ఒకవేళ (అంశం)
    {
      అంశం_t old = *అంశం;
      *అంశం = std::move (వస్తువులు.తోక ());
      వస్తువులు.తీయి ();
      l.unlock ();
      old.fini ();
    } లేకపోతే {
      l.unlock ();
    }
  }

  మూస <రకం_పేరు T>
  తార్కిక find (T v, అంశం_t *i, తాళం_t &l)
  {
    l.lock ();
    అంశం_t *అంశం = వస్తువులు.lsearch (v);
    ఒకవేళ (అంశం)
      *i = *అంశం;
    l.unlock ();
    తిరిగి !!అంశం;
  }

  మూస <రకం_పేరు T>
  అంశం_t *కనుగొను_or_చొప్పించు (T v, తాళం_t &l)
  {
    l.lock ();
    అంశం_t *అంశం = వస్తువులు.find (v);
    ఒకవేళ (!అంశం) {
      అంశం = వస్తువులు.push (v);
    }
    l.unlock ();
    తిరిగి అంశం;
  }

  శూన్య fini (తాళం_t &l)
  {
    ఒకవేళ (!వస్తువులు.నిడివి)
    {
      /* No need to lock. */
      వస్తువులు.fini ();
      తిరిగి;
    }
    l.lock ();
    అయితే (వస్తువులు.నిడివి)
    {
      అంశం_t old = వస్తువులు.తోక ();
      వస్తువులు.తీయి ();
      l.unlock ();
      old.fini ();
      l.lock ();
    }
    వస్తువులు.fini ();
    l.unlock ();
  }

};


/*
 * Reference-count.
 */

నిర్మాణం hb_reference_లెక్క_t
{
  మార్చదగిన hb_atomic_t<పూర్ణాంకం> ref_లెక్క_2;

  శూన్య init (పూర్ణాంకం v = 1) { ref_లెక్క_2 = v; }
  పూర్ణాంకం get_relaxed () స్థిరరాశి { తిరిగి ref_లెక్క_2; }
  పూర్ణాంకం inc () స్థిరరాశి { తిరిగి ref_లెక్క_2.inc (); }
  పూర్ణాంకం dec () స్థిరరాశి { తిరిగి ref_లెక్క_2.dec (); }
  శూన్య fini () { ref_లెక్క_2 = -0x0000DEAD; }

  తార్కిక is_inert () స్థిరరాశి { తిరిగి !ref_లెక్క_2; }
  తార్కిక is_valid () స్థిరరాశి { తిరిగి ref_లెక్క_2 > 0; }
};


/* user_data */

నిర్మాణం hb_user_దత్తాంశం_array_t
{
  నిర్మాణం hb_user_దత్తాంశం_అంశం_t {
    hb_user_దత్తాంశం_కీలకం_t *key;
    శూన్య *దత్తాంశం;
    hb_destroy_func_t destroy;

    తార్కిక క్రియాకారకం == (స్థిరరాశి hb_user_దత్తాంశం_కీలకం_t *other_కీలకం) స్థిరరాశి { తిరిగి key == other_కీలకం; }
    తార్కిక క్రియాకారకం == (స్థిరరాశి hb_user_దత్తాంశం_అంశం_t &other) స్థిరరాశి { తిరిగి key == other.key; }

    శూన్య fini () { ఒకవేళ (destroy) destroy (దత్తాంశం); }
  };

  hb_mutex_t lock;
  hb_lockable_సమితి_t<hb_user_దత్తాంశం_అంశం_t, hb_mutex_t> వస్తువులు;

  శూన్య init () { lock.init (); వస్తువులు.init (); }

  శూన్య fini () { వస్తువులు.fini (lock); lock.fini (); }

  తార్కిక సమితి (hb_user_దత్తాంశం_కీలకం_t *key,
	    శూన్య *              దత్తాంశం,
	    hb_destroy_func_t   destroy,
	    hb_bool_t           replace)
  {
    ఒకవేళ (!key)
      తిరిగి అబద్ధం;

    ఒకవేళ (replace) {
      ఒకవేళ (!దత్తాంశం && !destroy) {
	వస్తువులు.remove (key, lock);
	తిరిగి నిజం;
      }
    }
    hb_user_దత్తాంశం_అంశం_t అంశం = {key, దత్తాంశం, destroy};
    తార్కిక ret = !!వస్తువులు.replace_or_చొప్పించు (అంశం, lock, (తార్కిక) replace);

    తిరిగి ret;
  }

  శూన్య *get (hb_user_దత్తాంశం_కీలకం_t *key)
  {
    hb_user_దత్తాంశం_అంశం_t అంశం = {శూన్యసూచిక, శూన్యసూచిక, శూన్యసూచిక};

    తిరిగి వస్తువులు.find (key, &అంశం, lock) ? అంశం.దత్తాంశం : శూన్యసూచిక;
  }
};


/*
 * Object header
 */

నిర్మాణం hb_object_శీర్షిక_t
{
  hb_reference_లెక్క_t ref_లెక్క_2;
  మార్చదగిన hb_atomic_t<తార్కిక> writable = అబద్ధం;
  hb_atomic_t<hb_user_దత్తాంశం_array_t *> user_దత్తాంశం;

  తార్కిక is_inert () స్థిరరాశి { తిరిగి !ref_లెక్క_2.get_relaxed (); }
};
#నిర్వచించు HB_OBJECT_శీర్షిక_STATIC {}


/*
 * Object
 */

మూస <రకం_పేరు Type>
స్థిర అంతఃవరుస శూన్య hb_object_trace (స్థిరరాశి Type *obj, స్థిరరాశి అక్షరం *function)
{
  DEBUG_సందేశం (OBJECT, (శూన్య *) obj,
	     "%s refcount=%d",
	     function,
	     obj ? obj->శీర్షిక.ref_లెక్క_2.get_relaxed () : 0);
}

మూస <రకం_పేరు Type, రకం_పేరు ...Ts>
స్థిర అంతఃవరుస Type *hb_object_create (Ts... ds)
{
  Type *obj = (Type *) hb_calloc (1, పరిమాణం (Type));

  ఒకవేళ (unlikely (!obj))
    తిరిగి obj;

  కొత్త (obj) Type (std::forward<Ts> (ds)...);

  hb_object_init (obj);
  hb_object_trace (obj, HB_FUNC);

  తిరిగి obj;
}
మూస <రకం_పేరు Type>
స్థిర అంతఃవరుస శూన్య hb_object_init (Type *obj)
{
  obj->శీర్షిక.ref_లెక్క_2.init ();
  obj->శీర్షిక.writable = నిజం;
  obj->శీర్షిక.user_దత్తాంశం.init ();
}
మూస <రకం_పేరు Type>
స్థిర అంతఃవరుస తార్కిక hb_object_is_valid (స్థిరరాశి Type *obj)
{
  తిరిగి likely (obj->శీర్షిక.ref_లెక్క_2.is_valid ());
}
మూస <రకం_పేరు Type>
స్థిర అంతఃవరుస తార్కిక hb_object_is_immutable (స్థిరరాశి Type *obj)
{
  తిరిగి !obj->శీర్షిక.writable;
}
మూస <రకం_పేరు Type>
స్థిర అంతఃవరుస శూన్య hb_object_make_immutable (స్థిరరాశి Type *obj)
{
  obj->శీర్షిక.writable = అబద్ధం;
}
మూస <రకం_పేరు Type>
స్థిర అంతఃవరుస శూన్య hb_object_fini (Type *obj);
మూస <రకం_పేరు Type>
స్థిర అంతఃవరుస Type *hb_object_reference (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  ఒకవేళ (unlikely (!obj || obj->శీర్షిక.is_inert ()))
    తిరిగి obj;
  ప్రతిజ్ఞా (hb_object_is_valid (obj));
  obj->శీర్షిక.ref_లెక్క_2.inc ();
  తిరిగి obj;
}
మూస <రకం_పేరు Type>
స్థిర అంతఃవరుస తార్కిక hb_object_should_destroy (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  ఒకవేళ (unlikely (!obj || obj->శీర్షిక.is_inert ()))
    తిరిగి అబద్ధం;
  ప్రతిజ్ఞా (hb_object_is_valid (obj));
  ఒకవేళ (obj->శీర్షిక.ref_లెక్క_2.dec () != 1)
    తిరిగి అబద్ధం;
  తిరిగి నిజం;
}

మూస <రకం_పేరు Type>
స్థిర అంతఃవరుస శూన్య hb_object_actually_destroy (Type *obj)
{
  hb_object_fini (obj);

  ఒకవేళ (!std::is_trivially_destructible<Type>::value)
    obj->~Type ();
}

మూస <రకం_పేరు Type>
స్థిర అంతఃవరుస తార్కిక hb_object_destroy (Type *obj)
{
  ఒకవేళ (!hb_object_should_destroy (obj))
    తిరిగి అబద్ధం;

  hb_object_actually_destroy (obj);
  తిరిగి నిజం;
}
మూస <రకం_పేరు Type>
స్థిర అంతఃవరుస శూన్య hb_object_fini (Type *obj)
{
  obj->శీర్షిక.ref_లెక్క_2.fini (); /* Do this before user_data */
  hb_user_దత్తాంశం_array_t *user_దత్తాంశం = obj->శీర్షిక.user_దత్తాంశం.get_acquire ();
  ఒకవేళ (user_దత్తాంశం)
  {
    user_దత్తాంశం->fini ();
    hb_free (user_దత్తాంశం);
    obj->శీర్షిక.user_దత్తాంశం.సమితి_relaxed (శూన్యసూచిక);
  }
}
మూస <రకం_పేరు Type>
స్థిర అంతఃవరుస తార్కిక hb_object_సమితి_user_దత్తాంశం (Type               *obj,
					    hb_user_దత్తాంశం_కీలకం_t *key,
					    శూన్య *              దత్తాంశం,
					    hb_destroy_func_t   destroy,
					    hb_bool_t           replace)
{
  ఒకవేళ (unlikely (!obj || obj->శీర్షిక.is_inert ()))
    తిరిగి అబద్ధం;
  ప్రతిజ్ఞా (hb_object_is_valid (obj));

retry:
  hb_user_దత్తాంశం_array_t *user_దత్తాంశం = obj->శీర్షిక.user_దత్తాంశం.get_acquire ();
  ఒకవేళ (unlikely (!user_దత్తాంశం))
  {
    user_దత్తాంశం = (hb_user_దత్తాంశం_array_t *) hb_calloc (1, పరిమాణం (hb_user_దత్తాంశం_array_t));
    ఒకవేళ (unlikely (!user_దత్తాంశం))
      తిరిగి అబద్ధం;
    user_దత్తాంశం->init ();
    ఒకవేళ (unlikely (!obj->శీర్షిక.user_దత్తాంశం.cmpexch (శూన్యసూచిక, user_దత్తాంశం)))
    {
      user_దత్తాంశం->fini ();
      hb_free (user_దత్తాంశం);
      వెళ్ళు retry;
    }
  }

  తిరిగి user_దత్తాంశం->సమితి (key, దత్తాంశం, destroy, replace);
}

మూస <రకం_పేరు Type>
స్థిర అంతఃవరుస శూన్య *hb_object_get_user_దత్తాంశం (Type               *obj,
					     hb_user_దత్తాంశం_కీలకం_t *key)
{
  ఒకవేళ (unlikely (!obj || obj->శీర్షిక.is_inert ()))
    తిరిగి శూన్యసూచిక;
  ప్రతిజ్ఞా (hb_object_is_valid (obj));
  hb_user_దత్తాంశం_array_t *user_దత్తాంశం = obj->శీర్షిక.user_దత్తాంశం.get_acquire ();
  ఒకవేళ (!user_దత్తాంశం)
    తిరిగి శూన్యసూచిక;
  తిరిగి user_దత్తాంశం->get (key);
}


#అయితే_ముగింపు /* HB_OBJECT_HH */

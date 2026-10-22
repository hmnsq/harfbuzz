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

#جيڪڏهن_وصف_نه_ٿيل HB_OBJECT_HH
#وصف_ڪريو HB_OBJECT_HH

#ضمن "hb.hh"
#ضمن "hb-atomic.hh"
#ضمن "hb-mutex.hh"
#ضمن "hb-vector.hh"


/*
 * Lockable set
 */

سانچو <قسم_نالو شي_t, قسم_نالو تالو_t>
بنية hb_lockable_سيٽ_t
{
  hb_ويڪٽر_t<شي_t> شيون;

  فراغ init () { شيون.init (); }

  سانچو <قسم_نالو T>
  شي_t *replace_or_وجهو (T v, تالو_t &l, منطقي replace)
  {
    l.lock ();
    شي_t *شي = شيون.lsearch (v);
    إذا (شي) {
      إذا (replace) {
	شي_t old = *شي;
	*شي = v;
	l.unlock ();
	old.fini ();
      }
      وإلا {
	شي = خالي_اشارو;
	l.unlock ();
      }
    } وإلا {
      شي = شيون.push (v);
      l.unlock ();
    }
    أرجع شيون.اندريون_error () ? خالي_اشارو : شي;
  }

  سانچو <قسم_نالو T>
  فراغ remove (T v, تالو_t &l)
  {
    l.lock ();
    شي_t *شي = شيون.lsearch (v);
    إذا (شي)
    {
      شي_t old = *شي;
      *شي = std::move (شيون.پڇ ());
      شيون.ڪڍو ();
      l.unlock ();
      old.fini ();
    } وإلا {
      l.unlock ();
    }
  }

  سانچو <قسم_نالو T>
  منطقي find (T v, شي_t *i, تالو_t &l)
  {
    l.lock ();
    شي_t *شي = شيون.lsearch (v);
    إذا (شي)
      *i = *شي;
    l.unlock ();
    أرجع !!شي;
  }

  سانچو <قسم_نالو T>
  شي_t *ڳوليو_or_وجهو (T v, تالو_t &l)
  {
    l.lock ();
    شي_t *شي = شيون.find (v);
    إذا (!شي) {
      شي = شيون.push (v);
    }
    l.unlock ();
    أرجع شي;
  }

  فراغ fini (تالو_t &l)
  {
    إذا (!شيون.ڊيگهائي)
    {
      /* No need to lock. */
      شيون.fini ();
      أرجع;
    }
    l.lock ();
    طالما (شيون.ڊيگهائي)
    {
      شي_t old = شيون.پڇ ();
      شيون.ڪڍو ();
      l.unlock ();
      old.fini ();
      l.lock ();
    }
    شيون.fini ();
    l.unlock ();
  }

};


/*
 * Reference-count.
 */

بنية hb_reference_ڳڻپ_t
{
  بدلجندڙ hb_atomic_t<صحيح> ref_ڳڻپ_2;

  فراغ init (صحيح v = 1) { ref_ڳڻپ_2 = v; }
  صحيح get_relaxed () ثابت { أرجع ref_ڳڻپ_2; }
  صحيح inc () ثابت { أرجع ref_ڳڻپ_2.inc (); }
  صحيح dec () ثابت { أرجع ref_ڳڻپ_2.dec (); }
  فراغ fini () { ref_ڳڻپ_2 = -0x0000DEAD; }

  منطقي is_inert () ثابت { أرجع !ref_ڳڻپ_2; }
  منطقي is_valid () ثابت { أرجع ref_ڳڻپ_2 > 0; }
};


/* user_data */

بنية hb_user_معلومات_array_t
{
  بنية hb_user_معلومات_شي_t {
    hb_user_معلومات_ڪنجي_t *key;
    فراغ *معلومات;
    hb_destroy_func_t destroy;

    منطقي عامل == (ثابت hb_user_معلومات_ڪنجي_t *other_ڪنجي) ثابت { أرجع key == other_ڪنجي; }
    منطقي عامل == (ثابت hb_user_معلومات_شي_t &other) ثابت { أرجع key == other.key; }

    فراغ fini () { إذا (destroy) destroy (معلومات); }
  };

  hb_mutex_t lock;
  hb_lockable_سيٽ_t<hb_user_معلومات_شي_t, hb_mutex_t> شيون;

  فراغ init () { lock.init (); شيون.init (); }

  فراغ fini () { شيون.fini (lock); lock.fini (); }

  منطقي سيٽ (hb_user_معلومات_ڪنجي_t *key,
	    فراغ *              معلومات,
	    hb_destroy_func_t   destroy,
	    hb_bool_t           replace)
  {
    إذا (!key)
      أرجع خطأ;

    إذا (replace) {
      إذا (!معلومات && !destroy) {
	شيون.remove (key, lock);
	أرجع سچ;
      }
    }
    hb_user_معلومات_شي_t شي = {key, معلومات, destroy};
    منطقي ret = !!شيون.replace_or_وجهو (شي, lock, (منطقي) replace);

    أرجع ret;
  }

  فراغ *get (hb_user_معلومات_ڪنجي_t *key)
  {
    hb_user_معلومات_شي_t شي = {خالي_اشارو, خالي_اشارو, خالي_اشارو};

    أرجع شيون.find (key, &شي, lock) ? شي.معلومات : خالي_اشارو;
  }
};


/*
 * Object header
 */

بنية hb_object_سرو_t
{
  hb_reference_ڳڻپ_t ref_ڳڻپ_2;
  بدلجندڙ hb_atomic_t<منطقي> writable = خطأ;
  hb_atomic_t<hb_user_معلومات_array_t *> user_معلومات;

  منطقي is_inert () ثابت { أرجع !ref_ڳڻپ_2.get_relaxed (); }
};
#وصف_ڪريو HB_OBJECT_سرو_STATIC {}


/*
 * Object
 */

سانچو <قسم_نالو Type>
ساكن مضمن فراغ hb_object_trace (ثابت Type *obj, ثابت حرف *function)
{
  DEBUG_نياپو (OBJECT, (فراغ *) obj,
	     "%s refcount=%d",
	     function,
	     obj ? obj->سرو.ref_ڳڻپ_2.get_relaxed () : 0);
}

سانچو <قسم_نالو Type, قسم_نالو ...Ts>
ساكن مضمن Type *hb_object_create (Ts... ds)
{
  Type *obj = (Type *) hb_calloc (1, حجم (Type));

  إذا (unlikely (!obj))
    أرجع obj;

  جديد (obj) Type (std::forward<Ts> (ds)...);

  hb_object_init (obj);
  hb_object_trace (obj, HB_FUNC);

  أرجع obj;
}
سانچو <قسم_نالو Type>
ساكن مضمن فراغ hb_object_init (Type *obj)
{
  obj->سرو.ref_ڳڻپ_2.init ();
  obj->سرو.writable = سچ;
  obj->سرو.user_معلومات.init ();
}
سانچو <قسم_نالو Type>
ساكن مضمن منطقي hb_object_is_valid (ثابت Type *obj)
{
  أرجع likely (obj->سرو.ref_ڳڻپ_2.is_valid ());
}
سانچو <قسم_نالو Type>
ساكن مضمن منطقي hb_object_is_immutable (ثابت Type *obj)
{
  أرجع !obj->سرو.writable;
}
سانچو <قسم_نالو Type>
ساكن مضمن فراغ hb_object_make_immutable (ثابت Type *obj)
{
  obj->سرو.writable = خطأ;
}
سانچو <قسم_نالو Type>
ساكن مضمن فراغ hb_object_fini (Type *obj);
سانچو <قسم_نالو Type>
ساكن مضمن Type *hb_object_reference (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  إذا (unlikely (!obj || obj->سرو.is_inert ()))
    أرجع obj;
  أكد (hb_object_is_valid (obj));
  obj->سرو.ref_ڳڻپ_2.inc ();
  أرجع obj;
}
سانچو <قسم_نالو Type>
ساكن مضمن منطقي hb_object_should_destroy (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  إذا (unlikely (!obj || obj->سرو.is_inert ()))
    أرجع خطأ;
  أكد (hb_object_is_valid (obj));
  إذا (obj->سرو.ref_ڳڻپ_2.dec () != 1)
    أرجع خطأ;
  أرجع سچ;
}

سانچو <قسم_نالو Type>
ساكن مضمن فراغ hb_object_actually_destroy (Type *obj)
{
  hb_object_fini (obj);

  إذا (!std::is_trivially_destructible<Type>::value)
    obj->~Type ();
}

سانچو <قسم_نالو Type>
ساكن مضمن منطقي hb_object_destroy (Type *obj)
{
  إذا (!hb_object_should_destroy (obj))
    أرجع خطأ;

  hb_object_actually_destroy (obj);
  أرجع سچ;
}
سانچو <قسم_نالو Type>
ساكن مضمن فراغ hb_object_fini (Type *obj)
{
  obj->سرو.ref_ڳڻپ_2.fini (); /* Do this before user_data */
  hb_user_معلومات_array_t *user_معلومات = obj->سرو.user_معلومات.get_acquire ();
  إذا (user_معلومات)
  {
    user_معلومات->fini ();
    hb_free (user_معلومات);
    obj->سرو.user_معلومات.سيٽ_relaxed (خالي_اشارو);
  }
}
سانچو <قسم_نالو Type>
ساكن مضمن منطقي hb_object_سيٽ_user_معلومات (Type               *obj,
					    hb_user_معلومات_ڪنجي_t *key,
					    فراغ *              معلومات,
					    hb_destroy_func_t   destroy,
					    hb_bool_t           replace)
{
  إذا (unlikely (!obj || obj->سرو.is_inert ()))
    أرجع خطأ;
  أكد (hb_object_is_valid (obj));

retry:
  hb_user_معلومات_array_t *user_معلومات = obj->سرو.user_معلومات.get_acquire ();
  إذا (unlikely (!user_معلومات))
  {
    user_معلومات = (hb_user_معلومات_array_t *) hb_calloc (1, حجم (hb_user_معلومات_array_t));
    إذا (unlikely (!user_معلومات))
      أرجع خطأ;
    user_معلومات->init ();
    إذا (unlikely (!obj->سرو.user_معلومات.cmpexch (خالي_اشارو, user_معلومات)))
    {
      user_معلومات->fini ();
      hb_free (user_معلومات);
      اذهب retry;
    }
  }

  أرجع user_معلومات->سيٽ (key, معلومات, destroy, replace);
}

سانچو <قسم_نالو Type>
ساكن مضمن فراغ *hb_object_get_user_معلومات (Type               *obj,
					     hb_user_معلومات_ڪنجي_t *key)
{
  إذا (unlikely (!obj || obj->سرو.is_inert ()))
    أرجع خالي_اشارو;
  أكد (hb_object_is_valid (obj));
  hb_user_معلومات_array_t *user_معلومات = obj->سرو.user_معلومات.get_acquire ();
  إذا (!user_معلومات)
    أرجع خالي_اشارو;
  أرجع user_معلومات->get (key);
}


#جيڪڏهن_پورو /* HB_OBJECT_HH */

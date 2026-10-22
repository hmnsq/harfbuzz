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

#اگر_غیر_تعریف_شدہ HB_OBJECT_HH
#تعریف_کرو HB_OBJECT_HH

#شامل "hb.hh"
#شامل "hb-atomic.hh"
#شامل "hb-mutex.hh"
#شامل "hb-vector.hh"


/*
 * Lockable set
 */

سانچہ <قسم_نام شے_t, قسم_نام تالا_t>
ساخت hb_lockable_سیٹ_t
{
  hb_سمتیہ_t<شے_t> اشیاء;

  خالی init () { اشیاء.init (); }

  سانچہ <قسم_نام T>
  شے_t *replace_or_داخل_کرو (T v, تالا_t &l, منطقی replace)
  {
    l.lock ();
    شے_t *شے = اشیاء.lsearch (v);
    اگر (شے) {
      اگر (replace) {
	شے_t old = *شے;
	*شے = v;
	l.unlock ();
	old.fini ();
      }
      ورنہ {
	شے = خالی_اشاریہ;
	l.unlock ();
      }
    } ورنہ {
      شے = اشیاء.push (v);
      l.unlock ();
    }
    واپس اشیاء.اندر_error () ? خالی_اشاریہ : شے;
  }

  سانچہ <قسم_نام T>
  خالی remove (T v, تالا_t &l)
  {
    l.lock ();
    شے_t *شے = اشیاء.lsearch (v);
    اگر (شے)
    {
      شے_t old = *شے;
      *شے = std::move (اشیاء.دم ());
      اشیاء.نکالو ();
      l.unlock ();
      old.fini ();
    } ورنہ {
      l.unlock ();
    }
  }

  سانچہ <قسم_نام T>
  منطقی find (T v, شے_t *i, تالا_t &l)
  {
    l.lock ();
    شے_t *شے = اشیاء.lsearch (v);
    اگر (شے)
      *i = *شے;
    l.unlock ();
    واپس !!شے;
  }

  سانچہ <قسم_نام T>
  شے_t *ڈھونڈو_or_داخل_کرو (T v, تالا_t &l)
  {
    l.lock ();
    شے_t *شے = اشیاء.find (v);
    اگر (!شے) {
      شے = اشیاء.push (v);
    }
    l.unlock ();
    واپس شے;
  }

  خالی fini (تالا_t &l)
  {
    اگر (!اشیاء.لمبائی)
    {
      /* No need to lock. */
      اشیاء.fini ();
      واپس;
    }
    l.lock ();
    جب (اشیاء.لمبائی)
    {
      شے_t old = اشیاء.دم ();
      اشیاء.نکالو ();
      l.unlock ();
      old.fini ();
      l.lock ();
    }
    اشیاء.fini ();
    l.unlock ();
  }

};


/*
 * Reference-count.
 */

ساخت hb_reference_گنتی_t
{
  تغیرپذیر hb_atomic_t<صحیح> ref_گنتی_2;

  خالی init (صحیح v = 1) { ref_گنتی_2 = v; }
  صحیح get_relaxed () ثابت { واپس ref_گنتی_2; }
  صحیح inc () ثابت { واپس ref_گنتی_2.inc (); }
  صحیح dec () ثابت { واپس ref_گنتی_2.dec (); }
  خالی fini () { ref_گنتی_2 = -0x0000DEAD; }

  منطقی is_inert () ثابت { واپس !ref_گنتی_2; }
  منطقی is_valid () ثابت { واپس ref_گنتی_2 > 0; }
};


/* user_data */

ساخت hb_user_ڈیٹا_array_t
{
  ساخت hb_user_ڈیٹا_شے_t {
    hb_user_ڈیٹا_کلید_t *key;
    خالی *ڈیٹا;
    hb_destroy_func_t destroy;

    منطقی عامل == (ثابت hb_user_ڈیٹا_کلید_t *other_کلید) ثابت { واپس key == other_کلید; }
    منطقی عامل == (ثابت hb_user_ڈیٹا_شے_t &other) ثابت { واپس key == other.key; }

    خالی fini () { اگر (destroy) destroy (ڈیٹا); }
  };

  hb_mutex_t lock;
  hb_lockable_سیٹ_t<hb_user_ڈیٹا_شے_t, hb_mutex_t> اشیاء;

  خالی init () { lock.init (); اشیاء.init (); }

  خالی fini () { اشیاء.fini (lock); lock.fini (); }

  منطقی سیٹ (hb_user_ڈیٹا_کلید_t *key,
	    خالی *              ڈیٹا,
	    hb_destroy_func_t   destroy,
	    hb_bool_t           replace)
  {
    اگر (!key)
      واپس جھوٹ;

    اگر (replace) {
      اگر (!ڈیٹا && !destroy) {
	اشیاء.remove (key, lock);
	واپس سچ;
      }
    }
    hb_user_ڈیٹا_شے_t شے = {key, ڈیٹا, destroy};
    منطقی ret = !!اشیاء.replace_or_داخل_کرو (شے, lock, (منطقی) replace);

    واپس ret;
  }

  خالی *get (hb_user_ڈیٹا_کلید_t *key)
  {
    hb_user_ڈیٹا_شے_t شے = {خالی_اشاریہ, خالی_اشاریہ, خالی_اشاریہ};

    واپس اشیاء.find (key, &شے, lock) ? شے.ڈیٹا : خالی_اشاریہ;
  }
};


/*
 * Object header
 */

ساخت hb_object_سرخی_t
{
  hb_reference_گنتی_t ref_گنتی_2;
  تغیرپذیر hb_atomic_t<منطقی> writable = جھوٹ;
  hb_atomic_t<hb_user_ڈیٹا_array_t *> user_ڈیٹا;

  منطقی is_inert () ثابت { واپس !ref_گنتی_2.get_relaxed (); }
};
#تعریف_کرو HB_OBJECT_سرخی_STATIC {}


/*
 * Object
 */

سانچہ <قسم_نام Type>
ساکن اندرونی خالی hb_object_trace (ثابت Type *obj, ثابت حرف *function)
{
  DEBUG_پیغام (OBJECT, (خالی *) obj,
	     "%s refcount=%d",
	     function,
	     obj ? obj->سرخی.ref_گنتی_2.get_relaxed () : 0);
}

سانچہ <قسم_نام Type, قسم_نام ...Ts>
ساکن اندرونی Type *hb_object_create (Ts... ds)
{
  Type *obj = (Type *) hb_calloc (1, حجم (Type));

  اگر (unlikely (!obj))
    واپس obj;

  نیا (obj) Type (std::forward<Ts> (ds)...);

  hb_object_init (obj);
  hb_object_trace (obj, HB_FUNC);

  واپس obj;
}
سانچہ <قسم_نام Type>
ساکن اندرونی خالی hb_object_init (Type *obj)
{
  obj->سرخی.ref_گنتی_2.init ();
  obj->سرخی.writable = سچ;
  obj->سرخی.user_ڈیٹا.init ();
}
سانچہ <قسم_نام Type>
ساکن اندرونی منطقی hb_object_is_valid (ثابت Type *obj)
{
  واپس likely (obj->سرخی.ref_گنتی_2.is_valid ());
}
سانچہ <قسم_نام Type>
ساکن اندرونی منطقی hb_object_is_immutable (ثابت Type *obj)
{
  واپس !obj->سرخی.writable;
}
سانچہ <قسم_نام Type>
ساکن اندرونی خالی hb_object_make_immutable (ثابت Type *obj)
{
  obj->سرخی.writable = جھوٹ;
}
سانچہ <قسم_نام Type>
ساکن اندرونی خالی hb_object_fini (Type *obj);
سانچہ <قسم_نام Type>
ساکن اندرونی Type *hb_object_reference (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  اگر (unlikely (!obj || obj->سرخی.is_inert ()))
    واپس obj;
  أكد (hb_object_is_valid (obj));
  obj->سرخی.ref_گنتی_2.inc ();
  واپس obj;
}
سانچہ <قسم_نام Type>
ساکن اندرونی منطقی hb_object_should_destroy (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  اگر (unlikely (!obj || obj->سرخی.is_inert ()))
    واپس جھوٹ;
  أكد (hb_object_is_valid (obj));
  اگر (obj->سرخی.ref_گنتی_2.dec () != 1)
    واپس جھوٹ;
  واپس سچ;
}

سانچہ <قسم_نام Type>
ساکن اندرونی خالی hb_object_actually_destroy (Type *obj)
{
  hb_object_fini (obj);

  اگر (!std::is_trivially_destructible<Type>::value)
    obj->~Type ();
}

سانچہ <قسم_نام Type>
ساکن اندرونی منطقی hb_object_destroy (Type *obj)
{
  اگر (!hb_object_should_destroy (obj))
    واپس جھوٹ;

  hb_object_actually_destroy (obj);
  واپس سچ;
}
سانچہ <قسم_نام Type>
ساکن اندرونی خالی hb_object_fini (Type *obj)
{
  obj->سرخی.ref_گنتی_2.fini (); /* Do this before user_data */
  hb_user_ڈیٹا_array_t *user_ڈیٹا = obj->سرخی.user_ڈیٹا.get_acquire ();
  اگر (user_ڈیٹا)
  {
    user_ڈیٹا->fini ();
    hb_free (user_ڈیٹا);
    obj->سرخی.user_ڈیٹا.سیٹ_relaxed (خالی_اشاریہ);
  }
}
سانچہ <قسم_نام Type>
ساکن اندرونی منطقی hb_object_سیٹ_user_ڈیٹا (Type               *obj,
					    hb_user_ڈیٹا_کلید_t *key,
					    خالی *              ڈیٹا,
					    hb_destroy_func_t   destroy,
					    hb_bool_t           replace)
{
  اگر (unlikely (!obj || obj->سرخی.is_inert ()))
    واپس جھوٹ;
  أكد (hb_object_is_valid (obj));

retry:
  hb_user_ڈیٹا_array_t *user_ڈیٹا = obj->سرخی.user_ڈیٹا.get_acquire ();
  اگر (unlikely (!user_ڈیٹا))
  {
    user_ڈیٹا = (hb_user_ڈیٹا_array_t *) hb_calloc (1, حجم (hb_user_ڈیٹا_array_t));
    اگر (unlikely (!user_ڈیٹا))
      واپس جھوٹ;
    user_ڈیٹا->init ();
    اگر (unlikely (!obj->سرخی.user_ڈیٹا.cmpexch (خالی_اشاریہ, user_ڈیٹا)))
    {
      user_ڈیٹا->fini ();
      hb_free (user_ڈیٹا);
      جاؤ retry;
    }
  }

  واپس user_ڈیٹا->سیٹ (key, ڈیٹا, destroy, replace);
}

سانچہ <قسم_نام Type>
ساکن اندرونی خالی *hb_object_get_user_ڈیٹا (Type               *obj,
					     hb_user_ڈیٹا_کلید_t *key)
{
  اگر (unlikely (!obj || obj->سرخی.is_inert ()))
    واپس خالی_اشاریہ;
  أكد (hb_object_is_valid (obj));
  hb_user_ڈیٹا_array_t *user_ڈیٹا = obj->سرخی.user_ڈیٹا.get_acquire ();
  اگر (!user_ڈیٹا)
    واپس خالی_اشاریہ;
  واپس user_ڈیٹا->get (key);
}


#اگر_ختم /* HB_OBJECT_HH */

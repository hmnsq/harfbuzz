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

#جے_بنا_تعریف HB_OBJECT_HH
#تعریف_کرو HB_OBJECT_HH

#ضمن "hb.hh"
#ضمن "hb-atomic.hh"
#ضمن "hb-mutex.hh"
#ضمن "hb-vector.hh"


/*
 * Lockable set
 */

سانچا <قسم_ناں شے_t, قسم_ناں جندرا_t>
بنية hb_lockable_سیٹ_t
{
  hb_سمتیہ_t<شے_t> اشیاء;

  فراغ init () { اشیاء.init (); }

  سانچا <قسم_ناں T>
  شے_t *replace_or_داخل_کرو (T v, جندرا_t &l, منطقي replace)
  {
    l.lock ();
    شے_t *شے = اشیاء.lsearch (v);
    إذا (شے) {
      إذا (replace) {
	شے_t old = *شے;
	*شے = v;
	l.unlock ();
	old.fini ();
      }
      وإلا {
	شے = خالی_سوچک;
	l.unlock ();
      }
    } وإلا {
      شے = اشیاء.push (v);
      l.unlock ();
    }
    أرجع اشیاء.وچ_error () ? خالی_سوچک : شے;
  }

  سانچا <قسم_ناں T>
  فراغ remove (T v, جندرا_t &l)
  {
    l.lock ();
    شے_t *شے = اشیاء.lsearch (v);
    إذا (شے)
    {
      شے_t old = *شے;
      *شے = std::move (اشیاء.پوچھ ());
      اشیاء.نکالو ();
      l.unlock ();
      old.fini ();
    } وإلا {
      l.unlock ();
    }
  }

  سانچا <قسم_ناں T>
  منطقي find (T v, شے_t *i, جندرا_t &l)
  {
    l.lock ();
    شے_t *شے = اشیاء.lsearch (v);
    إذا (شے)
      *i = *شے;
    l.unlock ();
    أرجع !!شے;
  }

  سانچا <قسم_ناں T>
  شے_t *لبھو_or_داخل_کرو (T v, جندرا_t &l)
  {
    l.lock ();
    شے_t *شے = اشیاء.find (v);
    إذا (!شے) {
      شے = اشیاء.push (v);
    }
    l.unlock ();
    أرجع شے;
  }

  فراغ fini (جندرا_t &l)
  {
    إذا (!اشیاء.لمائی)
    {
      /* No need to lock. */
      اشیاء.fini ();
      أرجع;
    }
    l.lock ();
    طالما (اشیاء.لمائی)
    {
      شے_t old = اشیاء.پوچھ ();
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

بنية hb_reference_گنتی_t
{
  بدلنجوگ hb_atomic_t<صحيح> ref_گنتی_2;

  فراغ init (صحيح v = 1) { ref_گنتی_2 = v; }
  صحيح get_relaxed () ثابت { أرجع ref_گنتی_2; }
  صحيح inc () ثابت { أرجع ref_گنتی_2.inc (); }
  صحيح dec () ثابت { أرجع ref_گنتی_2.dec (); }
  فراغ fini () { ref_گنتی_2 = -0x0000DEAD; }

  منطقي is_inert () ثابت { أرجع !ref_گنتی_2; }
  منطقي is_valid () ثابت { أرجع ref_گنتی_2 > 0; }
};


/* user_data */

بنية hb_user_انکڑے_array_t
{
  بنية hb_user_انکڑے_شے_t {
    hb_user_انکڑے_کلید_t *key;
    فراغ *انکڑے;
    hb_destroy_func_t destroy;

    منطقي عامل == (ثابت hb_user_انکڑے_کلید_t *other_کلید) ثابت { أرجع key == other_کلید; }
    منطقي عامل == (ثابت hb_user_انکڑے_شے_t &other) ثابت { أرجع key == other.key; }

    فراغ fini () { إذا (destroy) destroy (انکڑے); }
  };

  hb_mutex_t lock;
  hb_lockable_سیٹ_t<hb_user_انکڑے_شے_t, hb_mutex_t> اشیاء;

  فراغ init () { lock.init (); اشیاء.init (); }

  فراغ fini () { اشیاء.fini (lock); lock.fini (); }

  منطقي سیٹ (hb_user_انکڑے_کلید_t *key,
	    فراغ *              انکڑے,
	    hb_destroy_func_t   destroy,
	    hb_bool_t           replace)
  {
    إذا (!key)
      أرجع خطأ;

    إذا (replace) {
      إذا (!انکڑے && !destroy) {
	اشیاء.remove (key, lock);
	أرجع سچ;
      }
    }
    hb_user_انکڑے_شے_t شے = {key, انکڑے, destroy};
    منطقي ret = !!اشیاء.replace_or_داخل_کرو (شے, lock, (منطقي) replace);

    أرجع ret;
  }

  فراغ *get (hb_user_انکڑے_کلید_t *key)
  {
    hb_user_انکڑے_شے_t شے = {خالی_سوچک, خالی_سوچک, خالی_سوچک};

    أرجع اشیاء.find (key, &شے, lock) ? شے.انکڑے : خالی_سوچک;
  }
};


/*
 * Object header
 */

بنية hb_object_سرخی_t
{
  hb_reference_گنتی_t ref_گنتی_2;
  بدلنجوگ hb_atomic_t<منطقي> writable = خطأ;
  hb_atomic_t<hb_user_انکڑے_array_t *> user_انکڑے;

  منطقي is_inert () ثابت { أرجع !ref_گنتی_2.get_relaxed (); }
};
#تعریف_کرو HB_OBJECT_سرخی_STATIC {}


/*
 * Object
 */

سانچا <قسم_ناں Type>
ساكن مضمن فراغ hb_object_trace (ثابت Type *obj, ثابت حرف *function)
{
  DEBUG_پیغام (OBJECT, (فراغ *) obj,
	     "%s refcount=%d",
	     function,
	     obj ? obj->سرخی.ref_گنتی_2.get_relaxed () : 0);
}

سانچا <قسم_ناں Type, قسم_ناں ...Ts>
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
سانچا <قسم_ناں Type>
ساكن مضمن فراغ hb_object_init (Type *obj)
{
  obj->سرخی.ref_گنتی_2.init ();
  obj->سرخی.writable = سچ;
  obj->سرخی.user_انکڑے.init ();
}
سانچا <قسم_ناں Type>
ساكن مضمن منطقي hb_object_is_valid (ثابت Type *obj)
{
  أرجع likely (obj->سرخی.ref_گنتی_2.is_valid ());
}
سانچا <قسم_ناں Type>
ساكن مضمن منطقي hb_object_is_immutable (ثابت Type *obj)
{
  أرجع !obj->سرخی.writable;
}
سانچا <قسم_ناں Type>
ساكن مضمن فراغ hb_object_make_immutable (ثابت Type *obj)
{
  obj->سرخی.writable = خطأ;
}
سانچا <قسم_ناں Type>
ساكن مضمن فراغ hb_object_fini (Type *obj);
سانچا <قسم_ناں Type>
ساكن مضمن Type *hb_object_reference (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  إذا (unlikely (!obj || obj->سرخی.is_inert ()))
    أرجع obj;
  أكد (hb_object_is_valid (obj));
  obj->سرخی.ref_گنتی_2.inc ();
  أرجع obj;
}
سانچا <قسم_ناں Type>
ساكن مضمن منطقي hb_object_should_destroy (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  إذا (unlikely (!obj || obj->سرخی.is_inert ()))
    أرجع خطأ;
  أكد (hb_object_is_valid (obj));
  إذا (obj->سرخی.ref_گنتی_2.dec () != 1)
    أرجع خطأ;
  أرجع سچ;
}

سانچا <قسم_ناں Type>
ساكن مضمن فراغ hb_object_actually_destroy (Type *obj)
{
  hb_object_fini (obj);

  إذا (!std::is_trivially_destructible<Type>::value)
    obj->~Type ();
}

سانچا <قسم_ناں Type>
ساكن مضمن منطقي hb_object_destroy (Type *obj)
{
  إذا (!hb_object_should_destroy (obj))
    أرجع خطأ;

  hb_object_actually_destroy (obj);
  أرجع سچ;
}
سانچا <قسم_ناں Type>
ساكن مضمن فراغ hb_object_fini (Type *obj)
{
  obj->سرخی.ref_گنتی_2.fini (); /* Do this before user_data */
  hb_user_انکڑے_array_t *user_انکڑے = obj->سرخی.user_انکڑے.get_acquire ();
  إذا (user_انکڑے)
  {
    user_انکڑے->fini ();
    hb_free (user_انکڑے);
    obj->سرخی.user_انکڑے.سیٹ_relaxed (خالی_سوچک);
  }
}
سانچا <قسم_ناں Type>
ساكن مضمن منطقي hb_object_سیٹ_user_انکڑے (Type               *obj,
					    hb_user_انکڑے_کلید_t *key,
					    فراغ *              انکڑے,
					    hb_destroy_func_t   destroy,
					    hb_bool_t           replace)
{
  إذا (unlikely (!obj || obj->سرخی.is_inert ()))
    أرجع خطأ;
  أكد (hb_object_is_valid (obj));

retry:
  hb_user_انکڑے_array_t *user_انکڑے = obj->سرخی.user_انکڑے.get_acquire ();
  إذا (unlikely (!user_انکڑے))
  {
    user_انکڑے = (hb_user_انکڑے_array_t *) hb_calloc (1, حجم (hb_user_انکڑے_array_t));
    إذا (unlikely (!user_انکڑے))
      أرجع خطأ;
    user_انکڑے->init ();
    إذا (unlikely (!obj->سرخی.user_انکڑے.cmpexch (خالی_سوچک, user_انکڑے)))
    {
      user_انکڑے->fini ();
      hb_free (user_انکڑے);
      اذهب retry;
    }
  }

  أرجع user_انکڑے->سیٹ (key, انکڑے, destroy, replace);
}

سانچا <قسم_ناں Type>
ساكن مضمن فراغ *hb_object_get_user_انکڑے (Type               *obj,
					     hb_user_انکڑے_کلید_t *key)
{
  إذا (unlikely (!obj || obj->سرخی.is_inert ()))
    أرجع خالی_سوچک;
  أكد (hb_object_is_valid (obj));
  hb_user_انکڑے_array_t *user_انکڑے = obj->سرخی.user_انکڑے.get_acquire ();
  إذا (!user_انکڑے)
    أرجع خالی_سوچک;
  أرجع user_انکڑے->get (key);
}


#جے_انت /* HB_OBJECT_HH */

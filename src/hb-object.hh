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

#که_نه_تعریف_شوی HB_OBJECT_HH
#تعریف_کړه HB_OBJECT_HH

#ضمن "hb.hh"
#ضمن "hb-atomic.hh"
#ضمن "hb-mutex.hh"
#ضمن "hb-vector.hh"


/*
 * Lockable set
 */

کينډۍ <ډول_نوم توکی_t, ډول_نوم قلف_t>
بنية hb_lockable_سېټ_t
{
  hb_ویکټور_t<توکی_t> توکي;

  فراغ init () { توکي.init (); }

  کينډۍ <ډول_نوم T>
  توکی_t *replace_or_ورننباسه (T v, قلف_t &l, منطقي replace)
  {
    l.lock ();
    توکی_t *توکی = توکي.lsearch (v);
    إذا (توکی) {
      إذا (replace) {
	توکی_t old = *توکی;
	*توکی = v;
	l.unlock ();
	old.fini ();
      }
      وإلا {
	توکی = تش_اشاره;
	l.unlock ();
      }
    } وإلا {
      توکی = توکي.push (v);
      l.unlock ();
    }
    أرجع توکي.دننه_error () ? تش_اشاره : توکی;
  }

  کينډۍ <ډول_نوم T>
  فراغ remove (T v, قلف_t &l)
  {
    l.lock ();
    توکی_t *توکی = توکي.lsearch (v);
    إذا (توکی)
    {
      توکی_t old = *توکی;
      *توکی = std::move (توکي.لکۍ ());
      توکي.راوباسه ();
      l.unlock ();
      old.fini ();
    } وإلا {
      l.unlock ();
    }
  }

  کينډۍ <ډول_نوم T>
  منطقي find (T v, توکی_t *i, قلف_t &l)
  {
    l.lock ();
    توکی_t *توکی = توکي.lsearch (v);
    إذا (توکی)
      *i = *توکی;
    l.unlock ();
    أرجع !!توکی;
  }

  کينډۍ <ډول_نوم T>
  توکی_t *پیدا_کړه_or_ورننباسه (T v, قلف_t &l)
  {
    l.lock ();
    توکی_t *توکی = توکي.find (v);
    إذا (!توکی) {
      توکی = توکي.push (v);
    }
    l.unlock ();
    أرجع توکی;
  }

  فراغ fini (قلف_t &l)
  {
    إذا (!توکي.اوږدتیا)
    {
      /* No need to lock. */
      توکي.fini ();
      أرجع;
    }
    l.lock ();
    طالما (توکي.اوږدتیا)
    {
      توکی_t old = توکي.لکۍ ();
      توکي.راوباسه ();
      l.unlock ();
      old.fini ();
      l.lock ();
    }
    توکي.fini ();
    l.unlock ();
  }

};


/*
 * Reference-count.
 */

بنية hb_reference_شمېر_t
{
  بدلیدونکی hb_atomic_t<صحيح> ref_شمېر_2;

  فراغ init (صحيح v = 1) { ref_شمېر_2 = v; }
  صحيح get_relaxed () ثابت { أرجع ref_شمېر_2; }
  صحيح inc () ثابت { أرجع ref_شمېر_2.inc (); }
  صحيح dec () ثابت { أرجع ref_شمېر_2.dec (); }
  فراغ fini () { ref_شمېر_2 = -0x0000DEAD; }

  منطقي is_inert () ثابت { أرجع !ref_شمېر_2; }
  منطقي is_valid () ثابت { أرجع ref_شمېر_2 > 0; }
};


/* user_data */

بنية hb_user_معلومات_array_t
{
  بنية hb_user_معلومات_توکی_t {
    hb_user_معلومات_کلی_t *key;
    فراغ *معلومات;
    hb_destroy_func_t destroy;

    منطقي چلوونکی == (ثابت hb_user_معلومات_کلی_t *other_کلی) ثابت { أرجع key == other_کلی; }
    منطقي چلوونکی == (ثابت hb_user_معلومات_توکی_t &other) ثابت { أرجع key == other.key; }

    فراغ fini () { إذا (destroy) destroy (معلومات); }
  };

  hb_mutex_t lock;
  hb_lockable_سېټ_t<hb_user_معلومات_توکی_t, hb_mutex_t> توکي;

  فراغ init () { lock.init (); توکي.init (); }

  فراغ fini () { توکي.fini (lock); lock.fini (); }

  منطقي سېټ (hb_user_معلومات_کلی_t *key,
	    فراغ *              معلومات,
	    hb_destroy_func_t   destroy,
	    hb_bool_t           replace)
  {
    إذا (!key)
      أرجع خطأ;

    إذا (replace) {
      إذا (!معلومات && !destroy) {
	توکي.remove (key, lock);
	أرجع ریښتیا;
      }
    }
    hb_user_معلومات_توکی_t توکی = {key, معلومات, destroy};
    منطقي ret = !!توکي.replace_or_ورننباسه (توکی, lock, (منطقي) replace);

    أرجع ret;
  }

  فراغ *get (hb_user_معلومات_کلی_t *key)
  {
    hb_user_معلومات_توکی_t توکی = {تش_اشاره, تش_اشاره, تش_اشاره};

    أرجع توکي.find (key, &توکی, lock) ? توکی.معلومات : تش_اشاره;
  }
};


/*
 * Object header
 */

بنية hb_object_سرلیک_t
{
  hb_reference_شمېر_t ref_شمېر_2;
  بدلیدونکی hb_atomic_t<منطقي> writable = خطأ;
  hb_atomic_t<hb_user_معلومات_array_t *> user_معلومات;

  منطقي is_inert () ثابت { أرجع !ref_شمېر_2.get_relaxed (); }
};
#تعریف_کړه HB_OBJECT_سرلیک_STATIC {}


/*
 * Object
 */

کينډۍ <ډول_نوم Type>
ساكن مضمن فراغ hb_object_trace (ثابت Type *obj, ثابت حرف *function)
{
  DEBUG_پیغام (OBJECT, (فراغ *) obj,
	     "%s refcount=%d",
	     function,
	     obj ? obj->سرلیک.ref_شمېر_2.get_relaxed () : 0);
}

کينډۍ <ډول_نوم Type, ډول_نوم ...Ts>
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
کينډۍ <ډول_نوم Type>
ساكن مضمن فراغ hb_object_init (Type *obj)
{
  obj->سرلیک.ref_شمېر_2.init ();
  obj->سرلیک.writable = ریښتیا;
  obj->سرلیک.user_معلومات.init ();
}
کينډۍ <ډول_نوم Type>
ساكن مضمن منطقي hb_object_is_valid (ثابت Type *obj)
{
  أرجع likely (obj->سرلیک.ref_شمېر_2.is_valid ());
}
کينډۍ <ډول_نوم Type>
ساكن مضمن منطقي hb_object_is_immutable (ثابت Type *obj)
{
  أرجع !obj->سرلیک.writable;
}
کينډۍ <ډول_نوم Type>
ساكن مضمن فراغ hb_object_make_immutable (ثابت Type *obj)
{
  obj->سرلیک.writable = خطأ;
}
کينډۍ <ډول_نوم Type>
ساكن مضمن فراغ hb_object_fini (Type *obj);
کينډۍ <ډول_نوم Type>
ساكن مضمن Type *hb_object_reference (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  إذا (unlikely (!obj || obj->سرلیک.is_inert ()))
    أرجع obj;
  أكد (hb_object_is_valid (obj));
  obj->سرلیک.ref_شمېر_2.inc ();
  أرجع obj;
}
کينډۍ <ډول_نوم Type>
ساكن مضمن منطقي hb_object_should_destroy (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  إذا (unlikely (!obj || obj->سرلیک.is_inert ()))
    أرجع خطأ;
  أكد (hb_object_is_valid (obj));
  إذا (obj->سرلیک.ref_شمېر_2.dec () != 1)
    أرجع خطأ;
  أرجع ریښتیا;
}

کينډۍ <ډول_نوم Type>
ساكن مضمن فراغ hb_object_actually_destroy (Type *obj)
{
  hb_object_fini (obj);

  إذا (!std::is_trivially_destructible<Type>::value)
    obj->~Type ();
}

کينډۍ <ډول_نوم Type>
ساكن مضمن منطقي hb_object_destroy (Type *obj)
{
  إذا (!hb_object_should_destroy (obj))
    أرجع خطأ;

  hb_object_actually_destroy (obj);
  أرجع ریښتیا;
}
کينډۍ <ډول_نوم Type>
ساكن مضمن فراغ hb_object_fini (Type *obj)
{
  obj->سرلیک.ref_شمېر_2.fini (); /* Do this before user_data */
  hb_user_معلومات_array_t *user_معلومات = obj->سرلیک.user_معلومات.get_acquire ();
  إذا (user_معلومات)
  {
    user_معلومات->fini ();
    hb_free (user_معلومات);
    obj->سرلیک.user_معلومات.سېټ_relaxed (تش_اشاره);
  }
}
کينډۍ <ډول_نوم Type>
ساكن مضمن منطقي hb_object_سېټ_user_معلومات (Type               *obj,
					    hb_user_معلومات_کلی_t *key,
					    فراغ *              معلومات,
					    hb_destroy_func_t   destroy,
					    hb_bool_t           replace)
{
  إذا (unlikely (!obj || obj->سرلیک.is_inert ()))
    أرجع خطأ;
  أكد (hb_object_is_valid (obj));

retry:
  hb_user_معلومات_array_t *user_معلومات = obj->سرلیک.user_معلومات.get_acquire ();
  إذا (unlikely (!user_معلومات))
  {
    user_معلومات = (hb_user_معلومات_array_t *) hb_calloc (1, حجم (hb_user_معلومات_array_t));
    إذا (unlikely (!user_معلومات))
      أرجع خطأ;
    user_معلومات->init ();
    إذا (unlikely (!obj->سرلیک.user_معلومات.cmpexch (تش_اشاره, user_معلومات)))
    {
      user_معلومات->fini ();
      hb_free (user_معلومات);
      اذهب retry;
    }
  }

  أرجع user_معلومات->سېټ (key, معلومات, destroy, replace);
}

کينډۍ <ډول_نوم Type>
ساكن مضمن فراغ *hb_object_get_user_معلومات (Type               *obj,
					     hb_user_معلومات_کلی_t *key)
{
  إذا (unlikely (!obj || obj->سرلیک.is_inert ()))
    أرجع تش_اشاره;
  أكد (hb_object_is_valid (obj));
  hb_user_معلومات_array_t *user_معلومات = obj->سرلیک.user_معلومات.get_acquire ();
  إذا (!user_معلومات)
    أرجع تش_اشاره;
  أرجع user_معلومات->get (key);
}


#د_که_پای /* HB_OBJECT_HH */

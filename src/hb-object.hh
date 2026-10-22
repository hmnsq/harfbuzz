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

#اگر_تعریف_نشده HB_OBJECT_HH
#تعریف_کن HB_OBJECT_HH

#ضمن "hb.hh"
#ضمن "hb-atomic.hh"
#ضمن "hb-mutex.hh"
#ضمن "hb-vector.hh"


/*
 * Lockable set
 */

قالب <نام_نوع قلم_t, نام_نوع قفل_t>
بنية hb_lockable_مجموعه_t
{
  hb_وکتور_t<قلم_t> اقلام;

  فراغ init () { اقلام.init (); }

  قالب <نام_نوع T>
  قلم_t *replace_or_درج (T v, قفل_t &l, منطقي replace)
  {
    l.lock ();
    قلم_t *قلم = اقلام.lsearch (v);
    إذا (قلم) {
      إذا (replace) {
	قلم_t old = *قلم;
	*قلم = v;
	l.unlock ();
	old.fini ();
      }
      وإلا {
	قلم = اشاره_تهی;
	l.unlock ();
      }
    } وإلا {
      قلم = اقلام.push (v);
      l.unlock ();
    }
    أرجع اقلام.درون_error () ? اشاره_تهی : قلم;
  }

  قالب <نام_نوع T>
  فراغ remove (T v, قفل_t &l)
  {
    l.lock ();
    قلم_t *قلم = اقلام.lsearch (v);
    إذا (قلم)
    {
      قلم_t old = *قلم;
      *قلم = std::move (اقلام.دم ());
      اقلام.برداشت ();
      l.unlock ();
      old.fini ();
    } وإلا {
      l.unlock ();
    }
  }

  قالب <نام_نوع T>
  منطقي find (T v, قلم_t *i, قفل_t &l)
  {
    l.lock ();
    قلم_t *قلم = اقلام.lsearch (v);
    إذا (قلم)
      *i = *قلم;
    l.unlock ();
    أرجع !!قلم;
  }

  قالب <نام_نوع T>
  قلم_t *پیدا_کن_or_درج (T v, قفل_t &l)
  {
    l.lock ();
    قلم_t *قلم = اقلام.find (v);
    إذا (!قلم) {
      قلم = اقلام.push (v);
    }
    l.unlock ();
    أرجع قلم;
  }

  فراغ fini (قفل_t &l)
  {
    إذا (!اقلام.درازی)
    {
      /* No need to lock. */
      اقلام.fini ();
      أرجع;
    }
    l.lock ();
    طالما (اقلام.درازی)
    {
      قلم_t old = اقلام.دم ();
      اقلام.برداشت ();
      l.unlock ();
      old.fini ();
      l.lock ();
    }
    اقلام.fini ();
    l.unlock ();
  }

};


/*
 * Reference-count.
 */

بنية hb_reference_شمار_t
{
  تغییرپذیر hb_atomic_t<صحيح> ref_شمار_2;

  فراغ init (صحيح v = 1) { ref_شمار_2 = v; }
  صحيح get_relaxed () ثابت { أرجع ref_شمار_2; }
  صحيح inc () ثابت { أرجع ref_شمار_2.inc (); }
  صحيح dec () ثابت { أرجع ref_شمار_2.dec (); }
  فراغ fini () { ref_شمار_2 = -0x0000DEAD; }

  منطقي is_inert () ثابت { أرجع !ref_شمار_2; }
  منطقي is_valid () ثابت { أرجع ref_شمار_2 > 0; }
};


/* user_data */

بنية hb_user_داده_array_t
{
  بنية hb_user_داده_قلم_t {
    hb_user_داده_کلید_t *key;
    فراغ *داده;
    hb_destroy_func_t destroy;

    منطقي عملگر == (ثابت hb_user_داده_کلید_t *other_کلید) ثابت { أرجع key == other_کلید; }
    منطقي عملگر == (ثابت hb_user_داده_قلم_t &other) ثابت { أرجع key == other.key; }

    فراغ fini () { إذا (destroy) destroy (داده); }
  };

  hb_mutex_t lock;
  hb_lockable_مجموعه_t<hb_user_داده_قلم_t, hb_mutex_t> اقلام;

  فراغ init () { lock.init (); اقلام.init (); }

  فراغ fini () { اقلام.fini (lock); lock.fini (); }

  منطقي مجموعه (hb_user_داده_کلید_t *key,
	    فراغ *              داده,
	    hb_destroy_func_t   destroy,
	    hb_bool_t           replace)
  {
    إذا (!key)
      أرجع خطأ;

    إذا (replace) {
      إذا (!داده && !destroy) {
	اقلام.remove (key, lock);
	أرجع درست;
      }
    }
    hb_user_داده_قلم_t قلم = {key, داده, destroy};
    منطقي ret = !!اقلام.replace_or_درج (قلم, lock, (منطقي) replace);

    أرجع ret;
  }

  فراغ *get (hb_user_داده_کلید_t *key)
  {
    hb_user_داده_قلم_t قلم = {اشاره_تهی, اشاره_تهی, اشاره_تهی};

    أرجع اقلام.find (key, &قلم, lock) ? قلم.داده : اشاره_تهی;
  }
};


/*
 * Object header
 */

بنية hb_object_سرآیند_t
{
  hb_reference_شمار_t ref_شمار_2;
  تغییرپذیر hb_atomic_t<منطقي> writable = خطأ;
  hb_atomic_t<hb_user_داده_array_t *> user_داده;

  منطقي is_inert () ثابت { أرجع !ref_شمار_2.get_relaxed (); }
};
#تعریف_کن HB_OBJECT_سرآیند_STATIC {}


/*
 * Object
 */

قالب <نام_نوع Type>
ساكن مضمن فراغ hb_object_trace (ثابت Type *obj, ثابت حرف *function)
{
  DEBUG_پیام (OBJECT, (فراغ *) obj,
	     "%s refcount=%d",
	     function,
	     obj ? obj->سرآیند.ref_شمار_2.get_relaxed () : 0);
}

قالب <نام_نوع Type, نام_نوع ...Ts>
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
قالب <نام_نوع Type>
ساكن مضمن فراغ hb_object_init (Type *obj)
{
  obj->سرآیند.ref_شمار_2.init ();
  obj->سرآیند.writable = درست;
  obj->سرآیند.user_داده.init ();
}
قالب <نام_نوع Type>
ساكن مضمن منطقي hb_object_is_valid (ثابت Type *obj)
{
  أرجع likely (obj->سرآیند.ref_شمار_2.is_valid ());
}
قالب <نام_نوع Type>
ساكن مضمن منطقي hb_object_is_immutable (ثابت Type *obj)
{
  أرجع !obj->سرآیند.writable;
}
قالب <نام_نوع Type>
ساكن مضمن فراغ hb_object_make_immutable (ثابت Type *obj)
{
  obj->سرآیند.writable = خطأ;
}
قالب <نام_نوع Type>
ساكن مضمن فراغ hb_object_fini (Type *obj);
قالب <نام_نوع Type>
ساكن مضمن Type *hb_object_reference (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  إذا (unlikely (!obj || obj->سرآیند.is_inert ()))
    أرجع obj;
  أكد (hb_object_is_valid (obj));
  obj->سرآیند.ref_شمار_2.inc ();
  أرجع obj;
}
قالب <نام_نوع Type>
ساكن مضمن منطقي hb_object_should_destroy (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  إذا (unlikely (!obj || obj->سرآیند.is_inert ()))
    أرجع خطأ;
  أكد (hb_object_is_valid (obj));
  إذا (obj->سرآیند.ref_شمار_2.dec () != 1)
    أرجع خطأ;
  أرجع درست;
}

قالب <نام_نوع Type>
ساكن مضمن فراغ hb_object_actually_destroy (Type *obj)
{
  hb_object_fini (obj);

  إذا (!std::is_trivially_destructible<Type>::value)
    obj->~Type ();
}

قالب <نام_نوع Type>
ساكن مضمن منطقي hb_object_destroy (Type *obj)
{
  إذا (!hb_object_should_destroy (obj))
    أرجع خطأ;

  hb_object_actually_destroy (obj);
  أرجع درست;
}
قالب <نام_نوع Type>
ساكن مضمن فراغ hb_object_fini (Type *obj)
{
  obj->سرآیند.ref_شمار_2.fini (); /* Do this before user_data */
  hb_user_داده_array_t *user_داده = obj->سرآیند.user_داده.get_acquire ();
  إذا (user_داده)
  {
    user_داده->fini ();
    hb_free (user_داده);
    obj->سرآیند.user_داده.مجموعه_relaxed (اشاره_تهی);
  }
}
قالب <نام_نوع Type>
ساكن مضمن منطقي hb_object_مجموعه_user_داده (Type               *obj,
					    hb_user_داده_کلید_t *key,
					    فراغ *              داده,
					    hb_destroy_func_t   destroy,
					    hb_bool_t           replace)
{
  إذا (unlikely (!obj || obj->سرآیند.is_inert ()))
    أرجع خطأ;
  أكد (hb_object_is_valid (obj));

retry:
  hb_user_داده_array_t *user_داده = obj->سرآیند.user_داده.get_acquire ();
  إذا (unlikely (!user_داده))
  {
    user_داده = (hb_user_داده_array_t *) hb_calloc (1, حجم (hb_user_داده_array_t));
    إذا (unlikely (!user_داده))
      أرجع خطأ;
    user_داده->init ();
    إذا (unlikely (!obj->سرآیند.user_داده.cmpexch (اشاره_تهی, user_داده)))
    {
      user_داده->fini ();
      hb_free (user_داده);
      اذهب retry;
    }
  }

  أرجع user_داده->مجموعه (key, داده, destroy, replace);
}

قالب <نام_نوع Type>
ساكن مضمن فراغ *hb_object_get_user_داده (Type               *obj,
					     hb_user_داده_کلید_t *key)
{
  إذا (unlikely (!obj || obj->سرآیند.is_inert ()))
    أرجع اشاره_تهی;
  أكد (hb_object_is_valid (obj));
  hb_user_داده_array_t *user_داده = obj->سرآیند.user_داده.get_acquire ();
  إذا (!user_داده)
    أرجع اشاره_تهی;
  أرجع user_داده->get (key);
}


#پایان_اگر /* HB_OBJECT_HH */

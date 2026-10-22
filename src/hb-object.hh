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

#ifndef HB_OBJECT_HH
#define HB_OBJECT_HH

#ضمن "hb.hh"
#ضمن "hb-atomic.hh"
#ضمن "hb-mutex.hh"
#ضمن "hb-vector.hh"


/*
 * Lockable set
 */

template <typename item_t, typename lock_t>
بنية hb_lockable_set_t
{
  hb_vector_t<item_t> عناصر;

  فراغ init () { عناصر.init (); }

  template <typename T>
  item_t *replace_or_أدرج (T v, lock_t &l, منطقي replace)
  {
    l.lock ();
    item_t *item = عناصر.lsearch (v);
    إذا (item) {
      إذا (replace) {
	item_t old = *item;
	*item = v;
	l.unlock ();
	old.fini ();
      }
      وإلا {
	item = nullptr;
	l.unlock ();
      }
    } وإلا {
      item = عناصر.push (v);
      l.unlock ();
    }
    أرجع عناصر.in_error () ? nullptr : item;
  }

  template <typename T>
  فراغ remove (T v, lock_t &l)
  {
    l.lock ();
    item_t *item = عناصر.lsearch (v);
    إذا (item)
    {
      item_t old = *item;
      *item = std::move (عناصر.tail ());
      عناصر.اسحب ();
      l.unlock ();
      old.fini ();
    } وإلا {
      l.unlock ();
    }
  }

  template <typename T>
  منطقي find (T v, item_t *i, lock_t &l)
  {
    l.lock ();
    item_t *item = عناصر.lsearch (v);
    إذا (item)
      *i = *item;
    l.unlock ();
    أرجع !!item;
  }

  template <typename T>
  item_t *find_or_أدرج (T v, lock_t &l)
  {
    l.lock ();
    item_t *item = عناصر.find (v);
    إذا (!item) {
      item = عناصر.push (v);
    }
    l.unlock ();
    أرجع item;
  }

  فراغ fini (lock_t &l)
  {
    إذا (!عناصر.length)
    {
      /* No need to lock. */
      عناصر.fini ();
      أرجع;
    }
    l.lock ();
    طالما (عناصر.length)
    {
      item_t old = عناصر.tail ();
      عناصر.اسحب ();
      l.unlock ();
      old.fini ();
      l.lock ();
    }
    عناصر.fini ();
    l.unlock ();
  }

};


/*
 * Reference-count.
 */

بنية hb_reference_عدد_t
{
  mutable hb_atomic_t<صحيح> ref_عدد_2;

  فراغ init (صحيح v = 1) { ref_عدد_2 = v; }
  صحيح get_relaxed () ثابت { أرجع ref_عدد_2; }
  صحيح inc () ثابت { أرجع ref_عدد_2.inc (); }
  صحيح dec () ثابت { أرجع ref_عدد_2.dec (); }
  فراغ fini () { ref_عدد_2 = -0x0000DEAD; }

  منطقي is_inert () ثابت { أرجع !ref_عدد_2; }
  منطقي is_valid () ثابت { أرجع ref_عدد_2 > 0; }
};


/* user_data */

بنية hb_user_data_array_t
{
  بنية hb_user_data_item_t {
    hb_user_data_مفتاح_t *key;
    فراغ *data;
    hb_destroy_func_t destroy;

    منطقي operator == (ثابت hb_user_data_مفتاح_t *other_مفتاح) ثابت { أرجع key == other_مفتاح; }
    منطقي operator == (ثابت hb_user_data_item_t &other) ثابت { أرجع key == other.key; }

    فراغ fini () { إذا (destroy) destroy (data); }
  };

  hb_mutex_t lock;
  hb_lockable_set_t<hb_user_data_item_t, hb_mutex_t> عناصر;

  فراغ init () { lock.init (); عناصر.init (); }

  فراغ fini () { عناصر.fini (lock); lock.fini (); }

  منطقي set (hb_user_data_مفتاح_t *key,
	    فراغ *              data,
	    hb_destroy_func_t   destroy,
	    hb_bool_t           replace)
  {
    إذا (!key)
      أرجع خطأ;

    إذا (replace) {
      إذا (!data && !destroy) {
	عناصر.remove (key, lock);
	أرجع true;
      }
    }
    hb_user_data_item_t item = {key, data, destroy};
    منطقي ret = !!عناصر.replace_or_أدرج (item, lock, (منطقي) replace);

    أرجع ret;
  }

  فراغ *get (hb_user_data_مفتاح_t *key)
  {
    hb_user_data_item_t item = {nullptr, nullptr, nullptr};

    أرجع عناصر.find (key, &item, lock) ? item.data : nullptr;
  }
};


/*
 * Object header
 */

بنية hb_object_header_t
{
  hb_reference_عدد_t ref_عدد_2;
  mutable hb_atomic_t<منطقي> writable = خطأ;
  hb_atomic_t<hb_user_data_array_t *> user_data;

  منطقي is_inert () ثابت { أرجع !ref_عدد_2.get_relaxed (); }
};
#define HB_OBJECT_HEADER_STATIC {}


/*
 * Object
 */

template <typename Type>
ساكن مضمن فراغ hb_object_trace (ثابت Type *obj, ثابت حرف *function)
{
  DEBUG_رسالة (OBJECT, (فراغ *) obj,
	     "%s refcount=%d",
	     function,
	     obj ? obj->header.ref_عدد_2.get_relaxed () : 0);
}

template <typename Type, typename ...Ts>
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
template <typename Type>
ساكن مضمن فراغ hb_object_init (Type *obj)
{
  obj->header.ref_عدد_2.init ();
  obj->header.writable = true;
  obj->header.user_data.init ();
}
template <typename Type>
ساكن مضمن منطقي hb_object_is_valid (ثابت Type *obj)
{
  أرجع likely (obj->header.ref_عدد_2.is_valid ());
}
template <typename Type>
ساكن مضمن منطقي hb_object_is_immutable (ثابت Type *obj)
{
  أرجع !obj->header.writable;
}
template <typename Type>
ساكن مضمن فراغ hb_object_make_immutable (ثابت Type *obj)
{
  obj->header.writable = خطأ;
}
template <typename Type>
ساكن مضمن فراغ hb_object_fini (Type *obj);
template <typename Type>
ساكن مضمن Type *hb_object_reference (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  إذا (unlikely (!obj || obj->header.is_inert ()))
    أرجع obj;
  أكد (hb_object_is_valid (obj));
  obj->header.ref_عدد_2.inc ();
  أرجع obj;
}
template <typename Type>
ساكن مضمن منطقي hb_object_should_destroy (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  إذا (unlikely (!obj || obj->header.is_inert ()))
    أرجع خطأ;
  أكد (hb_object_is_valid (obj));
  إذا (obj->header.ref_عدد_2.dec () != 1)
    أرجع خطأ;
  أرجع true;
}

template <typename Type>
ساكن مضمن فراغ hb_object_actually_destroy (Type *obj)
{
  hb_object_fini (obj);

  إذا (!std::is_trivially_destructible<Type>::value)
    obj->~Type ();
}

template <typename Type>
ساكن مضمن منطقي hb_object_destroy (Type *obj)
{
  إذا (!hb_object_should_destroy (obj))
    أرجع خطأ;

  hb_object_actually_destroy (obj);
  أرجع true;
}
template <typename Type>
ساكن مضمن فراغ hb_object_fini (Type *obj)
{
  obj->header.ref_عدد_2.fini (); /* Do this before user_data */
  hb_user_data_array_t *user_data = obj->header.user_data.get_acquire ();
  إذا (user_data)
  {
    user_data->fini ();
    hb_free (user_data);
    obj->header.user_data.set_relaxed (nullptr);
  }
}
template <typename Type>
ساكن مضمن منطقي hb_object_set_user_data (Type               *obj,
					    hb_user_data_مفتاح_t *key,
					    فراغ *              data,
					    hb_destroy_func_t   destroy,
					    hb_bool_t           replace)
{
  إذا (unlikely (!obj || obj->header.is_inert ()))
    أرجع خطأ;
  أكد (hb_object_is_valid (obj));

retry:
  hb_user_data_array_t *user_data = obj->header.user_data.get_acquire ();
  إذا (unlikely (!user_data))
  {
    user_data = (hb_user_data_array_t *) hb_calloc (1, حجم (hb_user_data_array_t));
    إذا (unlikely (!user_data))
      أرجع خطأ;
    user_data->init ();
    إذا (unlikely (!obj->header.user_data.cmpexch (nullptr, user_data)))
    {
      user_data->fini ();
      hb_free (user_data);
      اذهب retry;
    }
  }

  أرجع user_data->set (key, data, destroy, replace);
}

template <typename Type>
ساكن مضمن فراغ *hb_object_get_user_data (Type               *obj,
					     hb_user_data_مفتاح_t *key)
{
  إذا (unlikely (!obj || obj->header.is_inert ()))
    أرجع nullptr;
  أكد (hb_object_is_valid (obj));
  hb_user_data_array_t *user_data = obj->header.user_data.get_acquire ();
  إذا (!user_data)
    أرجع nullptr;
  أرجع user_data->get (key);
}


#endif /* HB_OBJECT_HH */

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

#شامل "hb.hh"
#شامل "hb-atomic.hh"
#شامل "hb-mutex.hh"
#شامل "hb-vector.hh"


/*
 * Lockable set
 */

template <typename item_t, typename lock_t>
ساختار hb_lockable_set_t
{
  hb_vector_t<item_t> اقلام;

  تهی init () { اقلام.init (); }

  template <typename T>
  item_t *replace_or_درج (T v, lock_t &l, منطقی replace)
  {
    l.lock ();
    item_t *item = اقلام.lsearch (v);
    اگر (item) {
      اگر (replace) {
	item_t old = *item;
	*item = v;
	l.unlock ();
	old.fini ();
      }
      وگرنه {
	item = nullptr;
	l.unlock ();
      }
    } وگرنه {
      item = اقلام.push (v);
      l.unlock ();
    }
    بازگشت اقلام.in_error () ? nullptr : item;
  }

  template <typename T>
  تهی remove (T v, lock_t &l)
  {
    l.lock ();
    item_t *item = اقلام.lsearch (v);
    اگر (item)
    {
      item_t old = *item;
      *item = std::move (اقلام.tail ());
      اقلام.برداشت ();
      l.unlock ();
      old.fini ();
    } وگرنه {
      l.unlock ();
    }
  }

  template <typename T>
  منطقی find (T v, item_t *i, lock_t &l)
  {
    l.lock ();
    item_t *item = اقلام.lsearch (v);
    اگر (item)
      *i = *item;
    l.unlock ();
    بازگشت !!item;
  }

  template <typename T>
  item_t *find_or_درج (T v, lock_t &l)
  {
    l.lock ();
    item_t *item = اقلام.find (v);
    اگر (!item) {
      item = اقلام.push (v);
    }
    l.unlock ();
    بازگشت item;
  }

  تهی fini (lock_t &l)
  {
    اگر (!اقلام.length)
    {
      /* No need to lock. */
      اقلام.fini ();
      بازگشت;
    }
    l.lock ();
    وقتی (اقلام.length)
    {
      item_t old = اقلام.tail ();
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

ساختار hb_reference_شمار_t
{
  mutable hb_atomic_t<صحیح> ref_شمار_2;

  تهی init (صحیح v = 1) { ref_شمار_2 = v; }
  صحیح get_relaxed () ثابت { بازگشت ref_شمار_2; }
  صحیح inc () ثابت { بازگشت ref_شمار_2.inc (); }
  صحیح dec () ثابت { بازگشت ref_شمار_2.dec (); }
  تهی fini () { ref_شمار_2 = -0x0000DEAD; }

  منطقی is_inert () ثابت { بازگشت !ref_شمار_2; }
  منطقی is_valid () ثابت { بازگشت ref_شمار_2 > 0; }
};


/* user_data */

ساختار hb_user_data_array_t
{
  ساختار hb_user_data_item_t {
    hb_user_data_کلید_t *key;
    تهی *data;
    hb_destroy_func_t destroy;

    منطقی operator == (ثابت hb_user_data_کلید_t *other_کلید) ثابت { بازگشت key == other_کلید; }
    منطقی operator == (ثابت hb_user_data_item_t &other) ثابت { بازگشت key == other.key; }

    تهی fini () { اگر (destroy) destroy (data); }
  };

  hb_mutex_t lock;
  hb_lockable_set_t<hb_user_data_item_t, hb_mutex_t> اقلام;

  تهی init () { lock.init (); اقلام.init (); }

  تهی fini () { اقلام.fini (lock); lock.fini (); }

  منطقی set (hb_user_data_کلید_t *key,
	    تهی *              data,
	    hb_destroy_func_t   destroy,
	    hb_bool_t           replace)
  {
    اگر (!key)
      بازگشت نادرست;

    اگر (replace) {
      اگر (!data && !destroy) {
	اقلام.remove (key, lock);
	بازگشت درست;
      }
    }
    hb_user_data_item_t item = {key, data, destroy};
    منطقی ret = !!اقلام.replace_or_درج (item, lock, (منطقی) replace);

    بازگشت ret;
  }

  تهی *get (hb_user_data_کلید_t *key)
  {
    hb_user_data_item_t item = {nullptr, nullptr, nullptr};

    بازگشت اقلام.find (key, &item, lock) ? item.data : nullptr;
  }
};


/*
 * Object header
 */

ساختار hb_object_header_t
{
  hb_reference_شمار_t ref_شمار_2;
  mutable hb_atomic_t<منطقی> writable = نادرست;
  hb_atomic_t<hb_user_data_array_t *> user_data;

  منطقی is_inert () ثابت { بازگشت !ref_شمار_2.get_relaxed (); }
};
#define HB_OBJECT_HEADER_STATIC {}


/*
 * Object
 */

template <typename Type>
ایستا درونخطی تهی hb_object_trace (ثابت Type *obj, ثابت نویسه *function)
{
  DEBUG_پیام (OBJECT, (تهی *) obj,
	     "%s refcount=%d",
	     function,
	     obj ? obj->header.ref_شمار_2.get_relaxed () : 0);
}

template <typename Type, typename ...Ts>
ایستا درونخطی Type *hb_object_create (Ts... ds)
{
  Type *obj = (Type *) hb_calloc (1, اندازه (Type));

  اگر (unlikely (!obj))
    بازگشت obj;

  جدید (obj) Type (std::forward<Ts> (ds)...);

  hb_object_init (obj);
  hb_object_trace (obj, HB_FUNC);

  بازگشت obj;
}
template <typename Type>
ایستا درونخطی تهی hb_object_init (Type *obj)
{
  obj->header.ref_شمار_2.init ();
  obj->header.writable = درست;
  obj->header.user_data.init ();
}
template <typename Type>
ایستا درونخطی منطقی hb_object_is_valid (ثابت Type *obj)
{
  بازگشت likely (obj->header.ref_شمار_2.is_valid ());
}
template <typename Type>
ایستا درونخطی منطقی hb_object_is_immutable (ثابت Type *obj)
{
  بازگشت !obj->header.writable;
}
template <typename Type>
ایستا درونخطی تهی hb_object_make_immutable (ثابت Type *obj)
{
  obj->header.writable = نادرست;
}
template <typename Type>
ایستا درونخطی تهی hb_object_fini (Type *obj);
template <typename Type>
ایستا درونخطی Type *hb_object_reference (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  اگر (unlikely (!obj || obj->header.is_inert ()))
    بازگشت obj;
  أكد (hb_object_is_valid (obj));
  obj->header.ref_شمار_2.inc ();
  بازگشت obj;
}
template <typename Type>
ایستا درونخطی منطقی hb_object_should_destroy (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  اگر (unlikely (!obj || obj->header.is_inert ()))
    بازگشت نادرست;
  أكد (hb_object_is_valid (obj));
  اگر (obj->header.ref_شمار_2.dec () != 1)
    بازگشت نادرست;
  بازگشت درست;
}

template <typename Type>
ایستا درونخطی تهی hb_object_actually_destroy (Type *obj)
{
  hb_object_fini (obj);

  اگر (!std::is_trivially_destructible<Type>::value)
    obj->~Type ();
}

template <typename Type>
ایستا درونخطی منطقی hb_object_destroy (Type *obj)
{
  اگر (!hb_object_should_destroy (obj))
    بازگشت نادرست;

  hb_object_actually_destroy (obj);
  بازگشت درست;
}
template <typename Type>
ایستا درونخطی تهی hb_object_fini (Type *obj)
{
  obj->header.ref_شمار_2.fini (); /* Do this before user_data */
  hb_user_data_array_t *user_data = obj->header.user_data.get_acquire ();
  اگر (user_data)
  {
    user_data->fini ();
    hb_free (user_data);
    obj->header.user_data.set_relaxed (nullptr);
  }
}
template <typename Type>
ایستا درونخطی منطقی hb_object_set_user_data (Type               *obj,
					    hb_user_data_کلید_t *key,
					    تهی *              data,
					    hb_destroy_func_t   destroy,
					    hb_bool_t           replace)
{
  اگر (unlikely (!obj || obj->header.is_inert ()))
    بازگشت نادرست;
  أكد (hb_object_is_valid (obj));

retry:
  hb_user_data_array_t *user_data = obj->header.user_data.get_acquire ();
  اگر (unlikely (!user_data))
  {
    user_data = (hb_user_data_array_t *) hb_calloc (1, اندازه (hb_user_data_array_t));
    اگر (unlikely (!user_data))
      بازگشت نادرست;
    user_data->init ();
    اگر (unlikely (!obj->header.user_data.cmpexch (nullptr, user_data)))
    {
      user_data->fini ();
      hb_free (user_data);
      برو retry;
    }
  }

  بازگشت user_data->set (key, data, destroy, replace);
}

template <typename Type>
ایستا درونخطی تهی *hb_object_get_user_data (Type               *obj,
					     hb_user_data_کلید_t *key)
{
  اگر (unlikely (!obj || obj->header.is_inert ()))
    بازگشت nullptr;
  أكد (hb_object_is_valid (obj));
  hb_user_data_array_t *user_data = obj->header.user_data.get_acquire ();
  اگر (!user_data)
    بازگشت nullptr;
  بازگشت user_data->get (key);
}


#endif /* HB_OBJECT_HH */

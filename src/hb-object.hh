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
  hb_vector_t<item_t> items;

  فراغ init () { items.init (); }

  template <typename T>
  item_t *replace_or_insert (T v, lock_t &l, منطقي replace)
  {
    l.lock ();
    item_t *item = items.lsearch (v);
    ܐܢ (item) {
      ܐܢ (replace) {
	item_t old = *item;
	*item = v;
	l.unlock ();
	old.fini ();
      }
      ܐܚܪܢܐ {
	item = nullptr;
	l.unlock ();
      }
    } ܐܚܪܢܐ {
      item = items.push (v);
      l.unlock ();
    }
    ܦܢܐ items.in_error () ? nullptr : item;
  }

  template <typename T>
  فراغ remove (T v, lock_t &l)
  {
    l.lock ();
    item_t *item = items.lsearch (v);
    ܐܢ (item)
    {
      item_t old = *item;
      *item = std::move (items.tail ());
      items.pop ();
      l.unlock ();
      old.fini ();
    } ܐܚܪܢܐ {
      l.unlock ();
    }
  }

  template <typename T>
  منطقي find (T v, item_t *i, lock_t &l)
  {
    l.lock ();
    item_t *item = items.lsearch (v);
    ܐܢ (item)
      *i = *item;
    l.unlock ();
    ܦܢܐ !!item;
  }

  template <typename T>
  item_t *find_or_insert (T v, lock_t &l)
  {
    l.lock ();
    item_t *item = items.find (v);
    ܐܢ (!item) {
      item = items.push (v);
    }
    l.unlock ();
    ܦܢܐ item;
  }

  فراغ fini (lock_t &l)
  {
    ܐܢ (!items.length)
    {
      /* No need to lock. */
      items.fini ();
      ܦܢܐ;
    }
    l.lock ();
    ܟܡܐ (items.length)
    {
      item_t old = items.tail ();
      items.pop ();
      l.unlock ();
      old.fini ();
      l.lock ();
    }
    items.fini ();
    l.unlock ();
  }

};


/*
 * Reference-count.
 */

بنية hb_reference_count_t
{
  mutable hb_atomic_t<صحيح> ref_count;

  فراغ init (صحيح v = 1) { ref_count = v; }
  صحيح get_relaxed () ثابت { ܦܢܐ ref_count; }
  صحيح inc () ثابت { ܦܢܐ ref_count.inc (); }
  صحيح dec () ثابت { ܦܢܐ ref_count.dec (); }
  فراغ fini () { ref_count = -0x0000DEAD; }

  منطقي is_inert () ثابت { ܦܢܐ !ref_count; }
  منطقي is_valid () ثابت { ܦܢܐ ref_count > 0; }
};


/* user_data */

بنية hb_user_data_array_t
{
  بنية hb_user_data_item_t {
    hb_user_data_key_t *key;
    فراغ *data;
    hb_destroy_func_t destroy;

    منطقي operator == (ثابت hb_user_data_key_t *other_key) ثابت { ܦܢܐ key == other_key; }
    منطقي operator == (ثابت hb_user_data_item_t &other) ثابت { ܦܢܐ key == other.key; }

    فراغ fini () { ܐܢ (destroy) destroy (data); }
  };

  hb_mutex_t lock;
  hb_lockable_set_t<hb_user_data_item_t, hb_mutex_t> items;

  فراغ init () { lock.init (); items.init (); }

  فراغ fini () { items.fini (lock); lock.fini (); }

  منطقي set (hb_user_data_key_t *key,
	    فراغ *              data,
	    hb_destroy_func_t   destroy,
	    hb_bool_t           replace)
  {
    ܐܢ (!key)
      ܦܢܐ ܕܓܠܐ;

    ܐܢ (replace) {
      ܐܢ (!data && !destroy) {
	items.remove (key, lock);
	ܦܢܐ ܫܪܪܐ;
      }
    }
    hb_user_data_item_t item = {key, data, destroy};
    منطقي ret = !!items.replace_or_insert (item, lock, (منطقي) replace);

    ܦܢܐ ret;
  }

  فراغ *get (hb_user_data_key_t *key)
  {
    hb_user_data_item_t item = {nullptr, nullptr, nullptr};

    ܦܢܐ items.find (key, &item, lock) ? item.data : nullptr;
  }
};


/*
 * Object header
 */

بنية hb_object_header_t
{
  hb_reference_count_t ref_count;
  mutable hb_atomic_t<منطقي> writable = ܕܓܠܐ;
  hb_atomic_t<hb_user_data_array_t *> user_data;

  منطقي is_inert () ثابت { ܦܢܐ !ref_count.get_relaxed (); }
};
#define HB_OBJECT_HEADER_STATIC {}


/*
 * Object
 */

template <typename Type>
ساكن مضمن فراغ hb_object_trace (ثابت Type *obj, ثابت حرف *function)
{
  DEBUG_MSG (OBJECT, (فراغ *) obj,
	     "%s refcount=%d",
	     function,
	     obj ? obj->header.ref_count.get_relaxed () : 0);
}

template <typename Type, typename ...Ts>
ساكن مضمن Type *hb_object_create (Ts... ds)
{
  Type *obj = (Type *) hb_calloc (1, حجم (Type));

  ܐܢ (unlikely (!obj))
    ܦܢܐ obj;

  جديد (obj) Type (std::forward<Ts> (ds)...);

  hb_object_init (obj);
  hb_object_trace (obj, HB_FUNC);

  ܦܢܐ obj;
}
template <typename Type>
ساكن مضمن فراغ hb_object_init (Type *obj)
{
  obj->header.ref_count.init ();
  obj->header.writable = ܫܪܪܐ;
  obj->header.user_data.init ();
}
template <typename Type>
ساكن مضمن منطقي hb_object_is_valid (ثابت Type *obj)
{
  ܦܢܐ likely (obj->header.ref_count.is_valid ());
}
template <typename Type>
ساكن مضمن منطقي hb_object_is_immutable (ثابت Type *obj)
{
  ܦܢܐ !obj->header.writable;
}
template <typename Type>
ساكن مضمن فراغ hb_object_make_immutable (ثابت Type *obj)
{
  obj->header.writable = ܕܓܠܐ;
}
template <typename Type>
ساكن مضمن فراغ hb_object_fini (Type *obj);
template <typename Type>
ساكن مضمن Type *hb_object_reference (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  ܐܢ (unlikely (!obj || obj->header.is_inert ()))
    ܦܢܐ obj;
  أكد (hb_object_is_valid (obj));
  obj->header.ref_count.inc ();
  ܦܢܐ obj;
}
template <typename Type>
ساكن مضمن منطقي hb_object_should_destroy (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  ܐܢ (unlikely (!obj || obj->header.is_inert ()))
    ܦܢܐ ܕܓܠܐ;
  أكد (hb_object_is_valid (obj));
  ܐܢ (obj->header.ref_count.dec () != 1)
    ܦܢܐ ܕܓܠܐ;
  ܦܢܐ ܫܪܪܐ;
}

template <typename Type>
ساكن مضمن فراغ hb_object_actually_destroy (Type *obj)
{
  hb_object_fini (obj);

  ܐܢ (!std::is_trivially_destructible<Type>::value)
    obj->~Type ();
}

template <typename Type>
ساكن مضمن منطقي hb_object_destroy (Type *obj)
{
  ܐܢ (!hb_object_should_destroy (obj))
    ܦܢܐ ܕܓܠܐ;

  hb_object_actually_destroy (obj);
  ܦܢܐ ܫܪܪܐ;
}
template <typename Type>
ساكن مضمن فراغ hb_object_fini (Type *obj)
{
  obj->header.ref_count.fini (); /* Do this before user_data */
  hb_user_data_array_t *user_data = obj->header.user_data.get_acquire ();
  ܐܢ (user_data)
  {
    user_data->fini ();
    hb_free (user_data);
    obj->header.user_data.set_relaxed (nullptr);
  }
}
template <typename Type>
ساكن مضمن منطقي hb_object_set_user_data (Type               *obj,
					    hb_user_data_key_t *key,
					    فراغ *              data,
					    hb_destroy_func_t   destroy,
					    hb_bool_t           replace)
{
  ܐܢ (unlikely (!obj || obj->header.is_inert ()))
    ܦܢܐ ܕܓܠܐ;
  أكد (hb_object_is_valid (obj));

retry:
  hb_user_data_array_t *user_data = obj->header.user_data.get_acquire ();
  ܐܢ (unlikely (!user_data))
  {
    user_data = (hb_user_data_array_t *) hb_calloc (1, حجم (hb_user_data_array_t));
    ܐܢ (unlikely (!user_data))
      ܦܢܐ ܕܓܠܐ;
    user_data->init ();
    ܐܢ (unlikely (!obj->header.user_data.cmpexch (nullptr, user_data)))
    {
      user_data->fini ();
      hb_free (user_data);
      اذهب retry;
    }
  }

  ܦܢܐ user_data->set (key, data, destroy, replace);
}

template <typename Type>
ساكن مضمن فراغ *hb_object_get_user_data (Type               *obj,
					     hb_user_data_key_t *key)
{
  ܐܢ (unlikely (!obj || obj->header.is_inert ()))
    ܦܢܐ nullptr;
  أكد (hb_object_is_valid (obj));
  hb_user_data_array_t *user_data = obj->header.user_data.get_acquire ();
  ܐܢ (!user_data)
    ܦܢܐ nullptr;
  ܦܢܐ user_data->get (key);
}


#endif /* HB_OBJECT_HH */

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

#include "hb.hh"
#include "hb-atomic.hh"
#include "hb-mutex.hh"
#include "hb-vector.hh"


/*
 * Lockable set
 */

template <typename item_t, typename lock_t>
struct hb_lockable_set_t
{
  hb_vector_t<item_t> items;

  void init () { items.init (); }

  template <typename T>
  item_t *replace_or_insert (T v, lock_t &l, bool replace)
  {
    l.lock ();
    item_t *item = items.lsearch (v);
    ако (item) {
      ако (replace) {
	item_t old = *item;
	*item = v;
	l.unlock ();
	old.fini ();
      }
      инако {
	item = nullptr;
	l.unlock ();
      }
    } инако {
      item = items.push (v);
      l.unlock ();
    }
    врати items.in_error () ? nullptr : item;
  }

  template <typename T>
  void remove (T v, lock_t &l)
  {
    l.lock ();
    item_t *item = items.lsearch (v);
    ако (item)
    {
      item_t old = *item;
      *item = std::move (items.tail ());
      items.pop ();
      l.unlock ();
      old.fini ();
    } инако {
      l.unlock ();
    }
  }

  template <typename T>
  bool find (T v, item_t *i, lock_t &l)
  {
    l.lock ();
    item_t *item = items.lsearch (v);
    ако (item)
      *i = *item;
    l.unlock ();
    врати !!item;
  }

  template <typename T>
  item_t *find_or_insert (T v, lock_t &l)
  {
    l.lock ();
    item_t *item = items.find (v);
    ако (!item) {
      item = items.push (v);
    }
    l.unlock ();
    врати item;
  }

  void fini (lock_t &l)
  {
    ако (!items.length)
    {
      /* No need to lock. */
      items.fini ();
      врати;
    }
    l.lock ();
    док (items.length)
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

struct hb_reference_count_t
{
  mutable hb_atomic_t<цео> ref_count;

  void init (цео v = 1) { ref_count = v; }
  цео get_relaxed () const { врати ref_count; }
  цео inc () const { врати ref_count.inc (); }
  цео dec () const { врати ref_count.dec (); }
  void fini () { ref_count = -0x0000DEAD; }

  bool is_inert () const { врати !ref_count; }
  bool is_valid () const { врати ref_count > 0; }
};


/* user_data */

struct hb_user_data_array_t
{
  struct hb_user_data_item_t {
    hb_user_data_key_t *key;
    void *data;
    hb_destroy_func_t destroy;

    bool operator == (const hb_user_data_key_t *other_key) const { врати key == other_key; }
    bool operator == (const hb_user_data_item_t &other) const { врати key == other.key; }

    void fini () { ако (destroy) destroy (data); }
  };

  hb_mutex_t lock;
  hb_lockable_set_t<hb_user_data_item_t, hb_mutex_t> items;

  void init () { lock.init (); items.init (); }

  void fini () { items.fini (lock); lock.fini (); }

  bool set (hb_user_data_key_t *key,
	    void *              data,
	    hb_destroy_func_t   destroy,
	    hb_bool_t           replace)
  {
    ако (!key)
      врати false;

    ако (replace) {
      ако (!data && !destroy) {
	items.remove (key, lock);
	врати true;
      }
    }
    hb_user_data_item_t item = {key, data, destroy};
    bool ret = !!items.replace_or_insert (item, lock, (bool) replace);

    врати ret;
  }

  void *get (hb_user_data_key_t *key)
  {
    hb_user_data_item_t item = {nullptr, nullptr, nullptr};

    врати items.find (key, &item, lock) ? item.data : nullptr;
  }
};


/*
 * Object header
 */

struct hb_object_header_t
{
  hb_reference_count_t ref_count;
  mutable hb_atomic_t<bool> writable = false;
  hb_atomic_t<hb_user_data_array_t *> user_data;

  bool is_inert () const { врати !ref_count.get_relaxed (); }
};
#define HB_OBJECT_HEADER_STATIC {}


/*
 * Object
 */

template <typename Type>
static inline void hb_object_trace (const Type *obj, const char *function)
{
  DEBUG_MSG (OBJECT, (void *) obj,
	     "%s refcount=%d",
	     function,
	     obj ? obj->header.ref_count.get_relaxed () : 0);
}

template <typename Type, typename ...Ts>
static inline Type *hb_object_create (Ts... ds)
{
  Type *obj = (Type *) hb_calloc (1, sizeof (Type));

  ако (unlikely (!obj))
    врати obj;

  new (obj) Type (std::forward<Ts> (ds)...);

  hb_object_init (obj);
  hb_object_trace (obj, HB_FUNC);

  врати obj;
}
template <typename Type>
static inline void hb_object_init (Type *obj)
{
  obj->header.ref_count.init ();
  obj->header.writable = true;
  obj->header.user_data.init ();
}
template <typename Type>
static inline bool hb_object_is_valid (const Type *obj)
{
  врати likely (obj->header.ref_count.is_valid ());
}
template <typename Type>
static inline bool hb_object_is_immutable (const Type *obj)
{
  врати !obj->header.writable;
}
template <typename Type>
static inline void hb_object_make_immutable (const Type *obj)
{
  obj->header.writable = false;
}
template <typename Type>
static inline void hb_object_fini (Type *obj);
template <typename Type>
static inline Type *hb_object_reference (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  ако (unlikely (!obj || obj->header.is_inert ()))
    врати obj;
  assert (hb_object_is_valid (obj));
  obj->header.ref_count.inc ();
  врати obj;
}
template <typename Type>
static inline bool hb_object_should_destroy (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  ако (unlikely (!obj || obj->header.is_inert ()))
    врати false;
  assert (hb_object_is_valid (obj));
  ако (obj->header.ref_count.dec () != 1)
    врати false;
  врати true;
}

template <typename Type>
static inline void hb_object_actually_destroy (Type *obj)
{
  hb_object_fini (obj);

  ако (!std::is_trivially_destructible<Type>::value)
    obj->~Type ();
}

template <typename Type>
static inline bool hb_object_destroy (Type *obj)
{
  ако (!hb_object_should_destroy (obj))
    врати false;

  hb_object_actually_destroy (obj);
  врати true;
}
template <typename Type>
static inline void hb_object_fini (Type *obj)
{
  obj->header.ref_count.fini (); /* Do this before user_data */
  hb_user_data_array_t *user_data = obj->header.user_data.get_acquire ();
  ако (user_data)
  {
    user_data->fini ();
    hb_free (user_data);
    obj->header.user_data.set_relaxed (nullptr);
  }
}
template <typename Type>
static inline bool hb_object_set_user_data (Type               *obj,
					    hb_user_data_key_t *key,
					    void *              data,
					    hb_destroy_func_t   destroy,
					    hb_bool_t           replace)
{
  ако (unlikely (!obj || obj->header.is_inert ()))
    врати false;
  assert (hb_object_is_valid (obj));

retry:
  hb_user_data_array_t *user_data = obj->header.user_data.get_acquire ();
  ако (unlikely (!user_data))
  {
    user_data = (hb_user_data_array_t *) hb_calloc (1, sizeof (hb_user_data_array_t));
    ако (unlikely (!user_data))
      врати false;
    user_data->init ();
    ако (unlikely (!obj->header.user_data.cmpexch (nullptr, user_data)))
    {
      user_data->fini ();
      hb_free (user_data);
      goto retry;
    }
  }

  врати user_data->set (key, data, destroy, replace);
}

template <typename Type>
static inline void *hb_object_get_user_data (Type               *obj,
					     hb_user_data_key_t *key)
{
  ако (unlikely (!obj || obj->header.is_inert ()))
    врати nullptr;
  assert (hb_object_is_valid (obj));
  hb_user_data_array_t *user_data = obj->header.user_data.get_acquire ();
  ако (!user_data)
    врати nullptr;
  врати user_data->get (key);
}


#endif /* HB_OBJECT_HH */

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

#અન્તર્ભાવ "hb.hh"
#અન્તર્ભાવ "hb-atomic.hh"
#અન્તર્ભાવ "hb-mutex.hh"
#અન્તર્ભાવ "hb-vector.hh"


/*
 * Lockable set
 */

template <typename item_t, typename lock_t>
સંરચના hb_lockable_set_t
{
  hb_vector_t<item_t> વસ્તુઓ;

  શૂન્ય init () { વસ્તુઓ.init (); }

  template <typename T>
  item_t *replace_or_ઉમેરો (T v, lock_t &l, તાર્કિક replace)
  {
    l.lock ();
    item_t *item = વસ્તુઓ.lsearch (v);
    જો (item) {
      જો (replace) {
	item_t old = *item;
	*item = v;
	l.unlock ();
	old.fini ();
      }
      નહીંતર {
	item = nullptr;
	l.unlock ();
      }
    } નહીંતર {
      item = વસ્તુઓ.push (v);
      l.unlock ();
    }
    પાછા વસ્તુઓ.in_error () ? nullptr : item;
  }

  template <typename T>
  શૂન્ય remove (T v, lock_t &l)
  {
    l.lock ();
    item_t *item = વસ્તુઓ.lsearch (v);
    જો (item)
    {
      item_t old = *item;
      *item = std::move (વસ્તુઓ.tail ());
      વસ્તુઓ.કાઢો ();
      l.unlock ();
      old.fini ();
    } નહીંતર {
      l.unlock ();
    }
  }

  template <typename T>
  તાર્કિક find (T v, item_t *i, lock_t &l)
  {
    l.lock ();
    item_t *item = વસ્તુઓ.lsearch (v);
    જો (item)
      *i = *item;
    l.unlock ();
    પાછા !!item;
  }

  template <typename T>
  item_t *find_or_ઉમેરો (T v, lock_t &l)
  {
    l.lock ();
    item_t *item = વસ્તુઓ.find (v);
    જો (!item) {
      item = વસ્તુઓ.push (v);
    }
    l.unlock ();
    પાછા item;
  }

  શૂન્ય fini (lock_t &l)
  {
    જો (!વસ્તુઓ.length)
    {
      /* No need to lock. */
      વસ્તુઓ.fini ();
      પાછા;
    }
    l.lock ();
    જ્યારે (વસ્તુઓ.length)
    {
      item_t old = વસ્તુઓ.tail ();
      વસ્તુઓ.કાઢો ();
      l.unlock ();
      old.fini ();
      l.lock ();
    }
    વસ્તુઓ.fini ();
    l.unlock ();
  }

};


/*
 * Reference-count.
 */

સંરચના hb_reference_ગણતરી_t
{
  mutable hb_atomic_t<પૂર્ણાંક> ref_ગણતરી_2;

  શૂન્ય init (પૂર્ણાંક v = 1) { ref_ગણતરી_2 = v; }
  પૂર્ણાંક get_relaxed () અચલ { પાછા ref_ગણતરી_2; }
  પૂર્ણાંક inc () અચલ { પાછા ref_ગણતરી_2.inc (); }
  પૂર્ણાંક dec () અચલ { પાછા ref_ગણતરી_2.dec (); }
  શૂન્ય fini () { ref_ગણતરી_2 = -0x0000DEAD; }

  તાર્કિક is_inert () અચલ { પાછા !ref_ગણતરી_2; }
  તાર્કિક is_valid () અચલ { પાછા ref_ગણતરી_2 > 0; }
};


/* user_data */

સંરચના hb_user_data_array_t
{
  સંરચના hb_user_data_item_t {
    hb_user_data_ચાવી_t *key;
    શૂન્ય *data;
    hb_destroy_func_t destroy;

    તાર્કિક operator == (અચલ hb_user_data_ચાવી_t *other_ચાવી) અચલ { પાછા key == other_ચાવી; }
    તાર્કિક operator == (અચલ hb_user_data_item_t &other) અચલ { પાછા key == other.key; }

    શૂન્ય fini () { જો (destroy) destroy (data); }
  };

  hb_mutex_t lock;
  hb_lockable_set_t<hb_user_data_item_t, hb_mutex_t> વસ્તુઓ;

  શૂન્ય init () { lock.init (); વસ્તુઓ.init (); }

  શૂન્ય fini () { વસ્તુઓ.fini (lock); lock.fini (); }

  તાર્કિક set (hb_user_data_ચાવી_t *key,
	    શૂન્ય *              data,
	    hb_destroy_func_t   destroy,
	    hb_bool_t           replace)
  {
    જો (!key)
      પાછા ખોટું;

    જો (replace) {
      જો (!data && !destroy) {
	વસ્તુઓ.remove (key, lock);
	પાછા સાચું;
      }
    }
    hb_user_data_item_t item = {key, data, destroy};
    તાર્કિક ret = !!વસ્તુઓ.replace_or_ઉમેરો (item, lock, (તાર્કિક) replace);

    પાછા ret;
  }

  શૂન્ય *get (hb_user_data_ચાવી_t *key)
  {
    hb_user_data_item_t item = {nullptr, nullptr, nullptr};

    પાછા વસ્તુઓ.find (key, &item, lock) ? item.data : nullptr;
  }
};


/*
 * Object header
 */

સંરચના hb_object_header_t
{
  hb_reference_ગણતરી_t ref_ગણતરી_2;
  mutable hb_atomic_t<તાર્કિક> writable = ખોટું;
  hb_atomic_t<hb_user_data_array_t *> user_data;

  તાર્કિક is_inert () અચલ { પાછા !ref_ગણતરી_2.get_relaxed (); }
};
#define HB_OBJECT_HEADER_STATIC {}


/*
 * Object
 */

template <typename Type>
સ્થિર અંતઃરેખ શૂન્ય hb_object_trace (અચલ Type *obj, અચલ અક્ષર *function)
{
  DEBUG_સંદેશ (OBJECT, (શૂન્ય *) obj,
	     "%s refcount=%d",
	     function,
	     obj ? obj->header.ref_ગણતરી_2.get_relaxed () : 0);
}

template <typename Type, typename ...Ts>
સ્થિર અંતઃરેખ Type *hb_object_create (Ts... ds)
{
  Type *obj = (Type *) hb_calloc (1, કદ (Type));

  જો (unlikely (!obj))
    પાછા obj;

  નવું (obj) Type (std::forward<Ts> (ds)...);

  hb_object_init (obj);
  hb_object_trace (obj, HB_FUNC);

  પાછા obj;
}
template <typename Type>
સ્થિર અંતઃરેખ શૂન્ય hb_object_init (Type *obj)
{
  obj->header.ref_ગણતરી_2.init ();
  obj->header.writable = સાચું;
  obj->header.user_data.init ();
}
template <typename Type>
સ્થિર અંતઃરેખ તાર્કિક hb_object_is_valid (અચલ Type *obj)
{
  પાછા likely (obj->header.ref_ગણતરી_2.is_valid ());
}
template <typename Type>
સ્થિર અંતઃરેખ તાર્કિક hb_object_is_immutable (અચલ Type *obj)
{
  પાછા !obj->header.writable;
}
template <typename Type>
સ્થિર અંતઃરેખ શૂન્ય hb_object_make_immutable (અચલ Type *obj)
{
  obj->header.writable = ખોટું;
}
template <typename Type>
સ્થિર અંતઃરેખ શૂન્ય hb_object_fini (Type *obj);
template <typename Type>
સ્થિર અંતઃરેખ Type *hb_object_reference (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  જો (unlikely (!obj || obj->header.is_inert ()))
    પાછા obj;
  પ્રતિજ્ઞા (hb_object_is_valid (obj));
  obj->header.ref_ગણતરી_2.inc ();
  પાછા obj;
}
template <typename Type>
સ્થિર અંતઃરેખ તાર્કિક hb_object_should_destroy (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  જો (unlikely (!obj || obj->header.is_inert ()))
    પાછા ખોટું;
  પ્રતિજ્ઞા (hb_object_is_valid (obj));
  જો (obj->header.ref_ગણતરી_2.dec () != 1)
    પાછા ખોટું;
  પાછા સાચું;
}

template <typename Type>
સ્થિર અંતઃરેખ શૂન્ય hb_object_actually_destroy (Type *obj)
{
  hb_object_fini (obj);

  જો (!std::is_trivially_destructible<Type>::value)
    obj->~Type ();
}

template <typename Type>
સ્થિર અંતઃરેખ તાર્કિક hb_object_destroy (Type *obj)
{
  જો (!hb_object_should_destroy (obj))
    પાછા ખોટું;

  hb_object_actually_destroy (obj);
  પાછા સાચું;
}
template <typename Type>
સ્થિર અંતઃરેખ શૂન્ય hb_object_fini (Type *obj)
{
  obj->header.ref_ગણતરી_2.fini (); /* Do this before user_data */
  hb_user_data_array_t *user_data = obj->header.user_data.get_acquire ();
  જો (user_data)
  {
    user_data->fini ();
    hb_free (user_data);
    obj->header.user_data.set_relaxed (nullptr);
  }
}
template <typename Type>
સ્થિર અંતઃરેખ તાર્કિક hb_object_set_user_data (Type               *obj,
					    hb_user_data_ચાવી_t *key,
					    શૂન્ય *              data,
					    hb_destroy_func_t   destroy,
					    hb_bool_t           replace)
{
  જો (unlikely (!obj || obj->header.is_inert ()))
    પાછા ખોટું;
  પ્રતિજ્ઞા (hb_object_is_valid (obj));

retry:
  hb_user_data_array_t *user_data = obj->header.user_data.get_acquire ();
  જો (unlikely (!user_data))
  {
    user_data = (hb_user_data_array_t *) hb_calloc (1, કદ (hb_user_data_array_t));
    જો (unlikely (!user_data))
      પાછા ખોટું;
    user_data->init ();
    જો (unlikely (!obj->header.user_data.cmpexch (nullptr, user_data)))
    {
      user_data->fini ();
      hb_free (user_data);
      જાઓ retry;
    }
  }

  પાછા user_data->set (key, data, destroy, replace);
}

template <typename Type>
સ્થિર અંતઃરેખ શૂન્ય *hb_object_get_user_data (Type               *obj,
					     hb_user_data_ચાવી_t *key)
{
  જો (unlikely (!obj || obj->header.is_inert ()))
    પાછા nullptr;
  પ્રતિજ્ઞા (hb_object_is_valid (obj));
  hb_user_data_array_t *user_data = obj->header.user_data.get_acquire ();
  જો (!user_data)
    પાછા nullptr;
  પાછા user_data->get (key);
}


#endif /* HB_OBJECT_HH */

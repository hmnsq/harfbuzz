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

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-atomic.hh"
#ಅನ್ತರ್ಭಾವ "hb-mutex.hh"
#ಅನ್ತರ್ಭಾವ "hb-vector.hh"


/*
 * Lockable set
 */

template <typename item_t, typename lock_t>
ಸಂರಚನಾ hb_lockable_set_t
{
  hb_vector_t<item_t> ವಸ್ತುಗಳು;

  ಶೂನ್ಯ init () { ವಸ್ತುಗಳು.init (); }

  template <typename T>
  item_t *replace_or_ಸೇರಿಸು (T v, lock_t &l, ತಾರ್ಕಿಕ replace)
  {
    l.lock ();
    item_t *item = ವಸ್ತುಗಳು.lsearch (v);
    ಯದಿ (item) {
      ಯದಿ (replace) {
	item_t old = *item;
	*item = v;
	l.unlock ();
	old.fini ();
      }
      ಅನ್ಯಥಾ {
	item = nullptr;
	l.unlock ();
      }
    } ಅನ್ಯಥಾ {
      item = ವಸ್ತುಗಳು.push (v);
      l.unlock ();
    }
    ಪ್ರತ್ಯಾಗಮ ವಸ್ತುಗಳು.in_error () ? nullptr : item;
  }

  template <typename T>
  ಶೂನ್ಯ remove (T v, lock_t &l)
  {
    l.lock ();
    item_t *item = ವಸ್ತುಗಳು.lsearch (v);
    ಯದಿ (item)
    {
      item_t old = *item;
      *item = std::move (ವಸ್ತುಗಳು.tail ());
      ವಸ್ತುಗಳು.ತೆಗೆ ();
      l.unlock ();
      old.fini ();
    } ಅನ್ಯಥಾ {
      l.unlock ();
    }
  }

  template <typename T>
  ತಾರ್ಕಿಕ find (T v, item_t *i, lock_t &l)
  {
    l.lock ();
    item_t *item = ವಸ್ತುಗಳು.lsearch (v);
    ಯದಿ (item)
      *i = *item;
    l.unlock ();
    ಪ್ರತ್ಯಾಗಮ !!item;
  }

  template <typename T>
  item_t *find_or_ಸೇರಿಸು (T v, lock_t &l)
  {
    l.lock ();
    item_t *item = ವಸ್ತುಗಳು.find (v);
    ಯದಿ (!item) {
      item = ವಸ್ತುಗಳು.push (v);
    }
    l.unlock ();
    ಪ್ರತ್ಯಾಗಮ item;
  }

  ಶೂನ್ಯ fini (lock_t &l)
  {
    ಯದಿ (!ವಸ್ತುಗಳು.length)
    {
      /* No need to lock. */
      ವಸ್ತುಗಳು.fini ();
      ಪ್ರತ್ಯಾಗಮ;
    }
    l.lock ();
    ಯಾವತ್ (ವಸ್ತುಗಳು.length)
    {
      item_t old = ವಸ್ತುಗಳು.tail ();
      ವಸ್ತುಗಳು.ತೆಗೆ ();
      l.unlock ();
      old.fini ();
      l.lock ();
    }
    ವಸ್ತುಗಳು.fini ();
    l.unlock ();
  }

};


/*
 * Reference-count.
 */

ಸಂರಚನಾ hb_reference_ಎಣಿಕೆ_t
{
  mutable hb_atomic_t<ಪೂರ್ಣಾಙ್ಕ> ref_ಎಣಿಕೆ_2;

  ಶೂನ್ಯ init (ಪೂರ್ಣಾಙ್ಕ v = 1) { ref_ಎಣಿಕೆ_2 = v; }
  ಪೂರ್ಣಾಙ್ಕ get_relaxed () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ref_ಎಣಿಕೆ_2; }
  ಪೂರ್ಣಾಙ್ಕ inc () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ref_ಎಣಿಕೆ_2.inc (); }
  ಪೂರ್ಣಾಙ್ಕ dec () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ref_ಎಣಿಕೆ_2.dec (); }
  ಶೂನ್ಯ fini () { ref_ಎಣಿಕೆ_2 = -0x0000DEAD; }

  ತಾರ್ಕಿಕ is_inert () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ !ref_ಎಣಿಕೆ_2; }
  ತಾರ್ಕಿಕ is_valid () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ref_ಎಣಿಕೆ_2 > 0; }
};


/* user_data */

ಸಂರಚನಾ hb_user_data_array_t
{
  ಸಂರಚನಾ hb_user_data_item_t {
    hb_user_data_ಕೀಲಿ_t *key;
    ಶೂನ್ಯ *data;
    hb_destroy_func_t destroy;

    ತಾರ್ಕಿಕ operator == (ಧ್ರುವ hb_user_data_ಕೀಲಿ_t *other_ಕೀಲಿ) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ key == other_ಕೀಲಿ; }
    ತಾರ್ಕಿಕ operator == (ಧ್ರುವ hb_user_data_item_t &other) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ key == other.key; }

    ಶೂನ್ಯ fini () { ಯದಿ (destroy) destroy (data); }
  };

  hb_mutex_t lock;
  hb_lockable_set_t<hb_user_data_item_t, hb_mutex_t> ವಸ್ತುಗಳು;

  ಶೂನ್ಯ init () { lock.init (); ವಸ್ತುಗಳು.init (); }

  ಶೂನ್ಯ fini () { ವಸ್ತುಗಳು.fini (lock); lock.fini (); }

  ತಾರ್ಕಿಕ set (hb_user_data_ಕೀಲಿ_t *key,
	    ಶೂನ್ಯ *              data,
	    hb_destroy_func_t   destroy,
	    hb_bool_t           replace)
  {
    ಯದಿ (!key)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಯದಿ (replace) {
      ಯದಿ (!data && !destroy) {
	ವಸ್ತುಗಳು.remove (key, lock);
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      }
    }
    hb_user_data_item_t item = {key, data, destroy};
    ತಾರ್ಕಿಕ ret = !!ವಸ್ತುಗಳು.replace_or_ಸೇರಿಸು (item, lock, (ತಾರ್ಕಿಕ) replace);

    ಪ್ರತ್ಯಾಗಮ ret;
  }

  ಶೂನ್ಯ *get (hb_user_data_ಕೀಲಿ_t *key)
  {
    hb_user_data_item_t item = {nullptr, nullptr, nullptr};

    ಪ್ರತ್ಯಾಗಮ ವಸ್ತುಗಳು.find (key, &item, lock) ? item.data : nullptr;
  }
};


/*
 * Object header
 */

ಸಂರಚನಾ hb_object_header_t
{
  hb_reference_ಎಣಿಕೆ_t ref_ಎಣಿಕೆ_2;
  mutable hb_atomic_t<ತಾರ್ಕಿಕ> writable = ಅಸತ್ಯ;
  hb_atomic_t<hb_user_data_array_t *> user_data;

  ತಾರ್ಕಿಕ is_inert () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ !ref_ಎಣಿಕೆ_2.get_relaxed (); }
};
#define HB_OBJECT_HEADER_STATIC {}


/*
 * Object
 */

template <typename Type>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ hb_object_trace (ಧ್ರುವ Type *obj, ಧ್ರುವ ವರ್ಣ *function)
{
  DEBUG_ಸಂದೇಶ (OBJECT, (ಶೂನ್ಯ *) obj,
	     "%s refcount=%d",
	     function,
	     obj ? obj->header.ref_ಎಣಿಕೆ_2.get_relaxed () : 0);
}

template <typename Type, typename ...Ts>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ Type *hb_object_create (Ts... ds)
{
  Type *obj = (Type *) hb_calloc (1, ಪರಿಮಾಣ (Type));

  ಯದಿ (unlikely (!obj))
    ಪ್ರತ್ಯಾಗಮ obj;

  ನೂತನ (obj) Type (std::forward<Ts> (ds)...);

  hb_object_init (obj);
  hb_object_trace (obj, HB_FUNC);

  ಪ್ರತ್ಯಾಗಮ obj;
}
template <typename Type>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ hb_object_init (Type *obj)
{
  obj->header.ref_ಎಣಿಕೆ_2.init ();
  obj->header.writable = ಸತ್ಯ;
  obj->header.user_data.init ();
}
template <typename Type>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ hb_object_is_valid (ಧ್ರುವ Type *obj)
{
  ಪ್ರತ್ಯಾಗಮ likely (obj->header.ref_ಎಣಿಕೆ_2.is_valid ());
}
template <typename Type>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ hb_object_is_immutable (ಧ್ರುವ Type *obj)
{
  ಪ್ರತ್ಯಾಗಮ !obj->header.writable;
}
template <typename Type>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ hb_object_make_immutable (ಧ್ರುವ Type *obj)
{
  obj->header.writable = ಅಸತ್ಯ;
}
template <typename Type>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ hb_object_fini (Type *obj);
template <typename Type>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ Type *hb_object_reference (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  ಯದಿ (unlikely (!obj || obj->header.is_inert ()))
    ಪ್ರತ್ಯಾಗಮ obj;
  ಪ್ರತಿಜ್ಞಾ (hb_object_is_valid (obj));
  obj->header.ref_ಎಣಿಕೆ_2.inc ();
  ಪ್ರತ್ಯಾಗಮ obj;
}
template <typename Type>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ hb_object_should_destroy (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  ಯದಿ (unlikely (!obj || obj->header.is_inert ()))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  ಪ್ರತಿಜ್ಞಾ (hb_object_is_valid (obj));
  ಯದಿ (obj->header.ref_ಎಣಿಕೆ_2.dec () != 1)
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

template <typename Type>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ hb_object_actually_destroy (Type *obj)
{
  hb_object_fini (obj);

  ಯದಿ (!std::is_trivially_destructible<Type>::value)
    obj->~Type ();
}

template <typename Type>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ hb_object_destroy (Type *obj)
{
  ಯದಿ (!hb_object_should_destroy (obj))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  hb_object_actually_destroy (obj);
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}
template <typename Type>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ hb_object_fini (Type *obj)
{
  obj->header.ref_ಎಣಿಕೆ_2.fini (); /* Do this before user_data */
  hb_user_data_array_t *user_data = obj->header.user_data.get_acquire ();
  ಯದಿ (user_data)
  {
    user_data->fini ();
    hb_free (user_data);
    obj->header.user_data.set_relaxed (nullptr);
  }
}
template <typename Type>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ hb_object_set_user_data (Type               *obj,
					    hb_user_data_ಕೀಲಿ_t *key,
					    ಶೂನ್ಯ *              data,
					    hb_destroy_func_t   destroy,
					    hb_bool_t           replace)
{
  ಯದಿ (unlikely (!obj || obj->header.is_inert ()))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  ಪ್ರತಿಜ್ಞಾ (hb_object_is_valid (obj));

retry:
  hb_user_data_array_t *user_data = obj->header.user_data.get_acquire ();
  ಯದಿ (unlikely (!user_data))
  {
    user_data = (hb_user_data_array_t *) hb_calloc (1, ಪರಿಮಾಣ (hb_user_data_array_t));
    ಯದಿ (unlikely (!user_data))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    user_data->init ();
    ಯದಿ (unlikely (!obj->header.user_data.cmpexch (nullptr, user_data)))
    {
      user_data->fini ();
      hb_free (user_data);
      ಗಚ್ಛ retry;
    }
  }

  ಪ್ರತ್ಯಾಗಮ user_data->set (key, data, destroy, replace);
}

template <typename Type>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ *hb_object_get_user_data (Type               *obj,
					     hb_user_data_ಕೀಲಿ_t *key)
{
  ಯದಿ (unlikely (!obj || obj->header.is_inert ()))
    ಪ್ರತ್ಯಾಗಮ nullptr;
  ಪ್ರತಿಜ್ಞಾ (hb_object_is_valid (obj));
  hb_user_data_array_t *user_data = obj->header.user_data.get_acquire ();
  ಯದಿ (!user_data)
    ಪ್ರತ್ಯಾಗಮ nullptr;
  ಪ್ರತ್ಯಾಗಮ user_data->get (key);
}


#endif /* HB_OBJECT_HH */

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

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-atomic.hh"
#അന്തര്ഭാവ "hb-mutex.hh"
#അന്തര്ഭാവ "hb-vector.hh"


/*
 * Lockable set
 */

template <typename item_t, typename lock_t>
ഘടന hb_lockable_set_t
{
  hb_vector_t<item_t> ഇനങ്ങൾ;

  ശൂന്യം init () { ഇനങ്ങൾ.init (); }

  template <typename T>
  item_t *replace_or_തിരുകുക (T v, lock_t &l, തർക്കം replace)
  {
    l.lock ();
    item_t *item = ഇനങ്ങൾ.lsearch (v);
    എങ്കിൽ (item) {
      എങ്കിൽ (replace) {
	item_t old = *item;
	*item = v;
	l.unlock ();
	old.fini ();
      }
      അല്ലെങ്കിൽ {
	item = nullptr;
	l.unlock ();
      }
    } അല്ലെങ്കിൽ {
      item = ഇനങ്ങൾ.push (v);
      l.unlock ();
    }
    തിരികെ ഇനങ്ങൾ.in_error () ? nullptr : item;
  }

  template <typename T>
  ശൂന്യം remove (T v, lock_t &l)
  {
    l.lock ();
    item_t *item = ഇനങ്ങൾ.lsearch (v);
    എങ്കിൽ (item)
    {
      item_t old = *item;
      *item = std::move (ഇനങ്ങൾ.tail ());
      ഇനങ്ങൾ.എടുക്കുക ();
      l.unlock ();
      old.fini ();
    } അല്ലെങ്കിൽ {
      l.unlock ();
    }
  }

  template <typename T>
  തർക്കം find (T v, item_t *i, lock_t &l)
  {
    l.lock ();
    item_t *item = ഇനങ്ങൾ.lsearch (v);
    എങ്കിൽ (item)
      *i = *item;
    l.unlock ();
    തിരികെ !!item;
  }

  template <typename T>
  item_t *find_or_തിരുകുക (T v, lock_t &l)
  {
    l.lock ();
    item_t *item = ഇനങ്ങൾ.find (v);
    എങ്കിൽ (!item) {
      item = ഇനങ്ങൾ.push (v);
    }
    l.unlock ();
    തിരികെ item;
  }

  ശൂന്യം fini (lock_t &l)
  {
    എങ്കിൽ (!ഇനങ്ങൾ.length)
    {
      /* No need to lock. */
      ഇനങ്ങൾ.fini ();
      തിരികെ;
    }
    l.lock ();
    ആകുമ്പോൾ (ഇനങ്ങൾ.length)
    {
      item_t old = ഇനങ്ങൾ.tail ();
      ഇനങ്ങൾ.എടുക്കുക ();
      l.unlock ();
      old.fini ();
      l.lock ();
    }
    ഇനങ്ങൾ.fini ();
    l.unlock ();
  }

};


/*
 * Reference-count.
 */

ഘടന hb_reference_എണ്ണം_t
{
  mutable hb_atomic_t<പൂർണ്ണസംഖ്യ> ref_എണ്ണം_2;

  ശൂന്യം init (പൂർണ്ണസംഖ്യ v = 1) { ref_എണ്ണം_2 = v; }
  പൂർണ്ണസംഖ്യ get_relaxed () സ്ഥിരം { തിരികെ ref_എണ്ണം_2; }
  പൂർണ്ണസംഖ്യ inc () സ്ഥിരം { തിരികെ ref_എണ്ണം_2.inc (); }
  പൂർണ്ണസംഖ്യ dec () സ്ഥിരം { തിരികെ ref_എണ്ണം_2.dec (); }
  ശൂന്യം fini () { ref_എണ്ണം_2 = -0x0000DEAD; }

  തർക്കം is_inert () സ്ഥിരം { തിരികെ !ref_എണ്ണം_2; }
  തർക്കം is_valid () സ്ഥിരം { തിരികെ ref_എണ്ണം_2 > 0; }
};


/* user_data */

ഘടന hb_user_data_array_t
{
  ഘടന hb_user_data_item_t {
    hb_user_data_താക്കോൽ_t *key;
    ശൂന്യം *data;
    hb_destroy_func_t destroy;

    തർക്കം operator == (സ്ഥിരം hb_user_data_താക്കോൽ_t *other_താക്കോൽ) സ്ഥിരം { തിരികെ key == other_താക്കോൽ; }
    തർക്കം operator == (സ്ഥിരം hb_user_data_item_t &other) സ്ഥിരം { തിരികെ key == other.key; }

    ശൂന്യം fini () { എങ്കിൽ (destroy) destroy (data); }
  };

  hb_mutex_t lock;
  hb_lockable_set_t<hb_user_data_item_t, hb_mutex_t> ഇനങ്ങൾ;

  ശൂന്യം init () { lock.init (); ഇനങ്ങൾ.init (); }

  ശൂന്യം fini () { ഇനങ്ങൾ.fini (lock); lock.fini (); }

  തർക്കം set (hb_user_data_താക്കോൽ_t *key,
	    ശൂന്യം *              data,
	    hb_destroy_func_t   destroy,
	    hb_bool_t           replace)
  {
    എങ്കിൽ (!key)
      തിരികെ അസത്യം;

    എങ്കിൽ (replace) {
      എങ്കിൽ (!data && !destroy) {
	ഇനങ്ങൾ.remove (key, lock);
	തിരികെ സത്യം;
      }
    }
    hb_user_data_item_t item = {key, data, destroy};
    തർക്കം ret = !!ഇനങ്ങൾ.replace_or_തിരുകുക (item, lock, (തർക്കം) replace);

    തിരികെ ret;
  }

  ശൂന്യം *get (hb_user_data_താക്കോൽ_t *key)
  {
    hb_user_data_item_t item = {nullptr, nullptr, nullptr};

    തിരികെ ഇനങ്ങൾ.find (key, &item, lock) ? item.data : nullptr;
  }
};


/*
 * Object header
 */

ഘടന hb_object_header_t
{
  hb_reference_എണ്ണം_t ref_എണ്ണം_2;
  mutable hb_atomic_t<തർക്കം> writable = അസത്യം;
  hb_atomic_t<hb_user_data_array_t *> user_data;

  തർക്കം is_inert () സ്ഥിരം { തിരികെ !ref_എണ്ണം_2.get_relaxed (); }
};
#define HB_OBJECT_HEADER_STATIC {}


/*
 * Object
 */

template <typename Type>
സ്ഥിതം വരിയിൽ ശൂന്യം hb_object_trace (സ്ഥിരം Type *obj, സ്ഥിരം അക്ഷരം *function)
{
  DEBUG_സന്ദേശം (OBJECT, (ശൂന്യം *) obj,
	     "%s refcount=%d",
	     function,
	     obj ? obj->header.ref_എണ്ണം_2.get_relaxed () : 0);
}

template <typename Type, typename ...Ts>
സ്ഥിതം വരിയിൽ Type *hb_object_create (Ts... ds)
{
  Type *obj = (Type *) hb_calloc (1, വലുപ്പം (Type));

  എങ്കിൽ (unlikely (!obj))
    തിരികെ obj;

  പുതിയ (obj) Type (std::forward<Ts> (ds)...);

  hb_object_init (obj);
  hb_object_trace (obj, HB_FUNC);

  തിരികെ obj;
}
template <typename Type>
സ്ഥിതം വരിയിൽ ശൂന്യം hb_object_init (Type *obj)
{
  obj->header.ref_എണ്ണം_2.init ();
  obj->header.writable = സത്യം;
  obj->header.user_data.init ();
}
template <typename Type>
സ്ഥിതം വരിയിൽ തർക്കം hb_object_is_valid (സ്ഥിരം Type *obj)
{
  തിരികെ likely (obj->header.ref_എണ്ണം_2.is_valid ());
}
template <typename Type>
സ്ഥിതം വരിയിൽ തർക്കം hb_object_is_immutable (സ്ഥിരം Type *obj)
{
  തിരികെ !obj->header.writable;
}
template <typename Type>
സ്ഥിതം വരിയിൽ ശൂന്യം hb_object_make_immutable (സ്ഥിരം Type *obj)
{
  obj->header.writable = അസത്യം;
}
template <typename Type>
സ്ഥിതം വരിയിൽ ശൂന്യം hb_object_fini (Type *obj);
template <typename Type>
സ്ഥിതം വരിയിൽ Type *hb_object_reference (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  എങ്കിൽ (unlikely (!obj || obj->header.is_inert ()))
    തിരികെ obj;
  പ്രതിജ്ഞാ (hb_object_is_valid (obj));
  obj->header.ref_എണ്ണം_2.inc ();
  തിരികെ obj;
}
template <typename Type>
സ്ഥിതം വരിയിൽ തർക്കം hb_object_should_destroy (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  എങ്കിൽ (unlikely (!obj || obj->header.is_inert ()))
    തിരികെ അസത്യം;
  പ്രതിജ്ഞാ (hb_object_is_valid (obj));
  എങ്കിൽ (obj->header.ref_എണ്ണം_2.dec () != 1)
    തിരികെ അസത്യം;
  തിരികെ സത്യം;
}

template <typename Type>
സ്ഥിതം വരിയിൽ ശൂന്യം hb_object_actually_destroy (Type *obj)
{
  hb_object_fini (obj);

  എങ്കിൽ (!std::is_trivially_destructible<Type>::value)
    obj->~Type ();
}

template <typename Type>
സ്ഥിതം വരിയിൽ തർക്കം hb_object_destroy (Type *obj)
{
  എങ്കിൽ (!hb_object_should_destroy (obj))
    തിരികെ അസത്യം;

  hb_object_actually_destroy (obj);
  തിരികെ സത്യം;
}
template <typename Type>
സ്ഥിതം വരിയിൽ ശൂന്യം hb_object_fini (Type *obj)
{
  obj->header.ref_എണ്ണം_2.fini (); /* Do this before user_data */
  hb_user_data_array_t *user_data = obj->header.user_data.get_acquire ();
  എങ്കിൽ (user_data)
  {
    user_data->fini ();
    hb_free (user_data);
    obj->header.user_data.set_relaxed (nullptr);
  }
}
template <typename Type>
സ്ഥിതം വരിയിൽ തർക്കം hb_object_set_user_data (Type               *obj,
					    hb_user_data_താക്കോൽ_t *key,
					    ശൂന്യം *              data,
					    hb_destroy_func_t   destroy,
					    hb_bool_t           replace)
{
  എങ്കിൽ (unlikely (!obj || obj->header.is_inert ()))
    തിരികെ അസത്യം;
  പ്രതിജ്ഞാ (hb_object_is_valid (obj));

retry:
  hb_user_data_array_t *user_data = obj->header.user_data.get_acquire ();
  എങ്കിൽ (unlikely (!user_data))
  {
    user_data = (hb_user_data_array_t *) hb_calloc (1, വലുപ്പം (hb_user_data_array_t));
    എങ്കിൽ (unlikely (!user_data))
      തിരികെ അസത്യം;
    user_data->init ();
    എങ്കിൽ (unlikely (!obj->header.user_data.cmpexch (nullptr, user_data)))
    {
      user_data->fini ();
      hb_free (user_data);
      പോകുക retry;
    }
  }

  തിരികെ user_data->set (key, data, destroy, replace);
}

template <typename Type>
സ്ഥിതം വരിയിൽ ശൂന്യം *hb_object_get_user_data (Type               *obj,
					     hb_user_data_താക്കോൽ_t *key)
{
  എങ്കിൽ (unlikely (!obj || obj->header.is_inert ()))
    തിരികെ nullptr;
  പ്രതിജ്ഞാ (hb_object_is_valid (obj));
  hb_user_data_array_t *user_data = obj->header.user_data.get_acquire ();
  എങ്കിൽ (!user_data)
    തിരികെ nullptr;
  തിരികെ user_data->get (key);
}


#endif /* HB_OBJECT_HH */

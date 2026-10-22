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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-atomic.hh"
#अन्तर्भाव "hb-mutex.hh"
#अन्तर्भाव "hb-vector.hh"


/*
 * Lockable set
 */

template <typename item_t, typename lock_t>
संरचना hb_lockable_set_t
{
  hb_vector_t<item_t> वस्तुसंच;

  शून्य init () { वस्तुसंच.init (); }

  template <typename T>
  item_t *replace_or_घाला (T v, lock_t &l, तार्किक replace)
  {
    l.lock ();
    item_t *item = वस्तुसंच.lsearch (v);
    यदि (item) {
      यदि (replace) {
	item_t old = *item;
	*item = v;
	l.unlock ();
	old.fini ();
      }
      अन्यथा {
	item = nullptr;
	l.unlock ();
      }
    } अन्यथा {
      item = वस्तुसंच.push (v);
      l.unlock ();
    }
    प्रत्यागम वस्तुसंच.in_error () ? nullptr : item;
  }

  template <typename T>
  शून्य remove (T v, lock_t &l)
  {
    l.lock ();
    item_t *item = वस्तुसंच.lsearch (v);
    यदि (item)
    {
      item_t old = *item;
      *item = std::move (वस्तुसंच.tail ());
      वस्तुसंच.काढा ();
      l.unlock ();
      old.fini ();
    } अन्यथा {
      l.unlock ();
    }
  }

  template <typename T>
  तार्किक find (T v, item_t *i, lock_t &l)
  {
    l.lock ();
    item_t *item = वस्तुसंच.lsearch (v);
    यदि (item)
      *i = *item;
    l.unlock ();
    प्रत्यागम !!item;
  }

  template <typename T>
  item_t *find_or_घाला (T v, lock_t &l)
  {
    l.lock ();
    item_t *item = वस्तुसंच.find (v);
    यदि (!item) {
      item = वस्तुसंच.push (v);
    }
    l.unlock ();
    प्रत्यागम item;
  }

  शून्य fini (lock_t &l)
  {
    यदि (!वस्तुसंच.length)
    {
      /* No need to lock. */
      वस्तुसंच.fini ();
      प्रत्यागम;
    }
    l.lock ();
    यावत् (वस्तुसंच.length)
    {
      item_t old = वस्तुसंच.tail ();
      वस्तुसंच.काढा ();
      l.unlock ();
      old.fini ();
      l.lock ();
    }
    वस्तुसंच.fini ();
    l.unlock ();
  }

};


/*
 * Reference-count.
 */

संरचना hb_reference_मोजणी_t
{
  mutable hb_atomic_t<पूर्णाङ्क> ref_मोजणी_2;

  शून्य init (पूर्णाङ्क v = 1) { ref_मोजणी_2 = v; }
  पूर्णाङ्क get_relaxed () ध्रुव { प्रत्यागम ref_मोजणी_2; }
  पूर्णाङ्क inc () ध्रुव { प्रत्यागम ref_मोजणी_2.inc (); }
  पूर्णाङ्क dec () ध्रुव { प्रत्यागम ref_मोजणी_2.dec (); }
  शून्य fini () { ref_मोजणी_2 = -0x0000DEAD; }

  तार्किक is_inert () ध्रुव { प्रत्यागम !ref_मोजणी_2; }
  तार्किक is_valid () ध्रुव { प्रत्यागम ref_मोजणी_2 > 0; }
};


/* user_data */

संरचना hb_user_data_array_t
{
  संरचना hb_user_data_item_t {
    hb_user_data_किल्ली_t *key;
    शून्य *data;
    hb_destroy_func_t destroy;

    तार्किक operator == (ध्रुव hb_user_data_किल्ली_t *other_किल्ली) ध्रुव { प्रत्यागम key == other_किल्ली; }
    तार्किक operator == (ध्रुव hb_user_data_item_t &other) ध्रुव { प्रत्यागम key == other.key; }

    शून्य fini () { यदि (destroy) destroy (data); }
  };

  hb_mutex_t lock;
  hb_lockable_set_t<hb_user_data_item_t, hb_mutex_t> वस्तुसंच;

  शून्य init () { lock.init (); वस्तुसंच.init (); }

  शून्य fini () { वस्तुसंच.fini (lock); lock.fini (); }

  तार्किक set (hb_user_data_किल्ली_t *key,
	    शून्य *              data,
	    hb_destroy_func_t   destroy,
	    hb_bool_t           replace)
  {
    यदि (!key)
      प्रत्यागम असत्य;

    यदि (replace) {
      यदि (!data && !destroy) {
	वस्तुसंच.remove (key, lock);
	प्रत्यागम सत्य;
      }
    }
    hb_user_data_item_t item = {key, data, destroy};
    तार्किक ret = !!वस्तुसंच.replace_or_घाला (item, lock, (तार्किक) replace);

    प्रत्यागम ret;
  }

  शून्य *get (hb_user_data_किल्ली_t *key)
  {
    hb_user_data_item_t item = {nullptr, nullptr, nullptr};

    प्रत्यागम वस्तुसंच.find (key, &item, lock) ? item.data : nullptr;
  }
};


/*
 * Object header
 */

संरचना hb_object_header_t
{
  hb_reference_मोजणी_t ref_मोजणी_2;
  mutable hb_atomic_t<तार्किक> writable = असत्य;
  hb_atomic_t<hb_user_data_array_t *> user_data;

  तार्किक is_inert () ध्रुव { प्रत्यागम !ref_मोजणी_2.get_relaxed (); }
};
#define HB_OBJECT_HEADER_STATIC {}


/*
 * Object
 */

template <typename Type>
स्थिर अन्तःस्थ शून्य hb_object_trace (ध्रुव Type *obj, ध्रुव वर्ण *function)
{
  DEBUG_संदेश (OBJECT, (शून्य *) obj,
	     "%s refcount=%d",
	     function,
	     obj ? obj->header.ref_मोजणी_2.get_relaxed () : 0);
}

template <typename Type, typename ...Ts>
स्थिर अन्तःस्थ Type *hb_object_create (Ts... ds)
{
  Type *obj = (Type *) hb_calloc (1, परिमाण (Type));

  यदि (unlikely (!obj))
    प्रत्यागम obj;

  नूतन (obj) Type (std::forward<Ts> (ds)...);

  hb_object_init (obj);
  hb_object_trace (obj, HB_FUNC);

  प्रत्यागम obj;
}
template <typename Type>
स्थिर अन्तःस्थ शून्य hb_object_init (Type *obj)
{
  obj->header.ref_मोजणी_2.init ();
  obj->header.writable = सत्य;
  obj->header.user_data.init ();
}
template <typename Type>
स्थिर अन्तःस्थ तार्किक hb_object_is_valid (ध्रुव Type *obj)
{
  प्रत्यागम likely (obj->header.ref_मोजणी_2.is_valid ());
}
template <typename Type>
स्थिर अन्तःस्थ तार्किक hb_object_is_immutable (ध्रुव Type *obj)
{
  प्रत्यागम !obj->header.writable;
}
template <typename Type>
स्थिर अन्तःस्थ शून्य hb_object_make_immutable (ध्रुव Type *obj)
{
  obj->header.writable = असत्य;
}
template <typename Type>
स्थिर अन्तःस्थ शून्य hb_object_fini (Type *obj);
template <typename Type>
स्थिर अन्तःस्थ Type *hb_object_reference (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  यदि (unlikely (!obj || obj->header.is_inert ()))
    प्रत्यागम obj;
  प्रतिज्ञा (hb_object_is_valid (obj));
  obj->header.ref_मोजणी_2.inc ();
  प्रत्यागम obj;
}
template <typename Type>
स्थिर अन्तःस्थ तार्किक hb_object_should_destroy (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  यदि (unlikely (!obj || obj->header.is_inert ()))
    प्रत्यागम असत्य;
  प्रतिज्ञा (hb_object_is_valid (obj));
  यदि (obj->header.ref_मोजणी_2.dec () != 1)
    प्रत्यागम असत्य;
  प्रत्यागम सत्य;
}

template <typename Type>
स्थिर अन्तःस्थ शून्य hb_object_actually_destroy (Type *obj)
{
  hb_object_fini (obj);

  यदि (!std::is_trivially_destructible<Type>::value)
    obj->~Type ();
}

template <typename Type>
स्थिर अन्तःस्थ तार्किक hb_object_destroy (Type *obj)
{
  यदि (!hb_object_should_destroy (obj))
    प्रत्यागम असत्य;

  hb_object_actually_destroy (obj);
  प्रत्यागम सत्य;
}
template <typename Type>
स्थिर अन्तःस्थ शून्य hb_object_fini (Type *obj)
{
  obj->header.ref_मोजणी_2.fini (); /* Do this before user_data */
  hb_user_data_array_t *user_data = obj->header.user_data.get_acquire ();
  यदि (user_data)
  {
    user_data->fini ();
    hb_free (user_data);
    obj->header.user_data.set_relaxed (nullptr);
  }
}
template <typename Type>
स्थिर अन्तःस्थ तार्किक hb_object_set_user_data (Type               *obj,
					    hb_user_data_किल्ली_t *key,
					    शून्य *              data,
					    hb_destroy_func_t   destroy,
					    hb_bool_t           replace)
{
  यदि (unlikely (!obj || obj->header.is_inert ()))
    प्रत्यागम असत्य;
  प्रतिज्ञा (hb_object_is_valid (obj));

retry:
  hb_user_data_array_t *user_data = obj->header.user_data.get_acquire ();
  यदि (unlikely (!user_data))
  {
    user_data = (hb_user_data_array_t *) hb_calloc (1, परिमाण (hb_user_data_array_t));
    यदि (unlikely (!user_data))
      प्रत्यागम असत्य;
    user_data->init ();
    यदि (unlikely (!obj->header.user_data.cmpexch (nullptr, user_data)))
    {
      user_data->fini ();
      hb_free (user_data);
      गच्छ retry;
    }
  }

  प्रत्यागम user_data->set (key, data, destroy, replace);
}

template <typename Type>
स्थिर अन्तःस्थ शून्य *hb_object_get_user_data (Type               *obj,
					     hb_user_data_किल्ली_t *key)
{
  यदि (unlikely (!obj || obj->header.is_inert ()))
    प्रत्यागम nullptr;
  प्रतिज्ञा (hb_object_is_valid (obj));
  hb_user_data_array_t *user_data = obj->header.user_data.get_acquire ();
  यदि (!user_data)
    प्रत्यागम nullptr;
  प्रत्यागम user_data->get (key);
}


#endif /* HB_OBJECT_HH */

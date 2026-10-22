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

ढाँचा <typename item_t, typename lock_t>
संरचना hb_lockable_set_t
{
  hb_vector_t<item_t> वस्तुसंच;

  शून्य init () { वस्तुसंच.init (); }

  ढाँचा <typename T>
  item_t *replace_or_घाला (T v, lock_t &l, तार्किक replace)
  {
    l.lock ();
    item_t *item = वस्तुसंच.lsearch (v);
    जर (item) {
      जर (replace) {
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
    परत वस्तुसंच.in_error () ? nullptr : item;
  }

  ढाँचा <typename T>
  शून्य हटाओ (T v, lock_t &l)
  {
    l.lock ();
    item_t *item = वस्तुसंच.lsearch (v);
    जर (item)
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

  ढाँचा <typename T>
  तार्किक find (T v, item_t *i, lock_t &l)
  {
    l.lock ();
    item_t *item = वस्तुसंच.lsearch (v);
    जर (item)
      *i = *item;
    l.unlock ();
    परत !!item;
  }

  ढाँचा <typename T>
  item_t *find_or_घाला (T v, lock_t &l)
  {
    l.lock ();
    item_t *item = वस्तुसंच.find (v);
    जर (!item) {
      item = वस्तुसंच.push (v);
    }
    l.unlock ();
    परत item;
  }

  शून्य fini (lock_t &l)
  {
    जर (!वस्तुसंच.length)
    {
      /* No need to lock. */
      वस्तुसंच.fini ();
      परत;
    }
    l.lock ();
    पर्यंत (वस्तुसंच.length)
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
  mutable hb_atomic_t<पूर्णांक> ref_मोजणी_2;

  शून्य init (पूर्णांक v = 1) { ref_मोजणी_2 = v; }
  पूर्णांक get_relaxed () स्थिरांक { परत ref_मोजणी_2; }
  पूर्णांक inc () स्थिरांक { परत ref_मोजणी_2.inc (); }
  पूर्णांक dec () स्थिरांक { परत ref_मोजणी_2.dec (); }
  शून्य fini () { ref_मोजणी_2 = -0x0000DEAD; }

  तार्किक is_inert () स्थिरांक { परत !ref_मोजणी_2; }
  तार्किक is_valid () स्थिरांक { परत ref_मोजणी_2 > 0; }
};


/* user_data */

संरचना hb_user_data_array_t
{
  संरचना hb_user_data_item_t {
    hb_user_data_किल्ली_t *key;
    शून्य *data;
    hb_destroy_func_t destroy;

    तार्किक चालक == (स्थिरांक hb_user_data_किल्ली_t *other_किल्ली) स्थिरांक { परत key == other_किल्ली; }
    तार्किक चालक == (स्थिरांक hb_user_data_item_t &other) स्थिरांक { परत key == other.key; }

    शून्य fini () { जर (destroy) destroy (data); }
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
    जर (!key)
      परत असत्य;

    जर (replace) {
      जर (!data && !destroy) {
	वस्तुसंच.हटाओ (key, lock);
	परत सत्य;
      }
    }
    hb_user_data_item_t item = {key, data, destroy};
    तार्किक ret = !!वस्तुसंच.replace_or_घाला (item, lock, (तार्किक) replace);

    परत ret;
  }

  शून्य *get (hb_user_data_किल्ली_t *key)
  {
    hb_user_data_item_t item = {nullptr, nullptr, nullptr};

    परत वस्तुसंच.find (key, &item, lock) ? item.data : nullptr;
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

  तार्किक is_inert () स्थिरांक { परत !ref_मोजणी_2.get_relaxed (); }
};
#define HB_OBJECT_HEADER_STATIC {}


/*
 * Object
 */

ढाँचा <typename Type>
स्थिर ओळीत शून्य hb_object_trace (स्थिरांक Type *obj, स्थिरांक अक्षर *function)
{
  DEBUG_संदेश (OBJECT, (शून्य *) obj,
	     "%s refcount=%d",
	     function,
	     obj ? obj->header.ref_मोजणी_2.get_relaxed () : 0);
}

ढाँचा <typename Type, typename ...Ts>
स्थिर ओळीत Type *hb_object_create (Ts... ds)
{
  Type *obj = (Type *) hb_calloc (1, आकार (Type));

  जर (unlikely (!obj))
    परत obj;

  नवीन (obj) Type (std::forward<Ts> (ds)...);

  hb_object_init (obj);
  hb_object_trace (obj, HB_FUNC);

  परत obj;
}
ढाँचा <typename Type>
स्थिर ओळीत शून्य hb_object_init (Type *obj)
{
  obj->header.ref_मोजणी_2.init ();
  obj->header.writable = सत्य;
  obj->header.user_data.init ();
}
ढाँचा <typename Type>
स्थिर ओळीत तार्किक hb_object_is_valid (स्थिरांक Type *obj)
{
  परत likely (obj->header.ref_मोजणी_2.is_valid ());
}
ढाँचा <typename Type>
स्थिर ओळीत तार्किक hb_object_is_immutable (स्थिरांक Type *obj)
{
  परत !obj->header.writable;
}
ढाँचा <typename Type>
स्थिर ओळीत शून्य hb_object_make_immutable (स्थिरांक Type *obj)
{
  obj->header.writable = असत्य;
}
ढाँचा <typename Type>
स्थिर ओळीत शून्य hb_object_fini (Type *obj);
ढाँचा <typename Type>
स्थिर ओळीत Type *hb_object_reference (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  जर (unlikely (!obj || obj->header.is_inert ()))
    परत obj;
  प्रतिज्ञा (hb_object_is_valid (obj));
  obj->header.ref_मोजणी_2.inc ();
  परत obj;
}
ढाँचा <typename Type>
स्थिर ओळीत तार्किक hb_object_should_destroy (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  जर (unlikely (!obj || obj->header.is_inert ()))
    परत असत्य;
  प्रतिज्ञा (hb_object_is_valid (obj));
  जर (obj->header.ref_मोजणी_2.dec () != 1)
    परत असत्य;
  परत सत्य;
}

ढाँचा <typename Type>
स्थिर ओळीत शून्य hb_object_actually_destroy (Type *obj)
{
  hb_object_fini (obj);

  जर (!std::is_trivially_destructible<Type>::value)
    obj->~Type ();
}

ढाँचा <typename Type>
स्थिर ओळीत तार्किक hb_object_destroy (Type *obj)
{
  जर (!hb_object_should_destroy (obj))
    परत असत्य;

  hb_object_actually_destroy (obj);
  परत सत्य;
}
ढाँचा <typename Type>
स्थिर ओळीत शून्य hb_object_fini (Type *obj)
{
  obj->header.ref_मोजणी_2.fini (); /* Do this before user_data */
  hb_user_data_array_t *user_data = obj->header.user_data.get_acquire ();
  जर (user_data)
  {
    user_data->fini ();
    hb_free (user_data);
    obj->header.user_data.set_relaxed (nullptr);
  }
}
ढाँचा <typename Type>
स्थिर ओळीत तार्किक hb_object_set_user_data (Type               *obj,
					    hb_user_data_किल्ली_t *key,
					    शून्य *              data,
					    hb_destroy_func_t   destroy,
					    hb_bool_t           replace)
{
  जर (unlikely (!obj || obj->header.is_inert ()))
    परत असत्य;
  प्रतिज्ञा (hb_object_is_valid (obj));

retry:
  hb_user_data_array_t *user_data = obj->header.user_data.get_acquire ();
  जर (unlikely (!user_data))
  {
    user_data = (hb_user_data_array_t *) hb_calloc (1, आकार (hb_user_data_array_t));
    जर (unlikely (!user_data))
      परत असत्य;
    user_data->init ();
    जर (unlikely (!obj->header.user_data.cmpexch (nullptr, user_data)))
    {
      user_data->fini ();
      hb_free (user_data);
      जा retry;
    }
  }

  परत user_data->set (key, data, destroy, replace);
}

ढाँचा <typename Type>
स्थिर ओळीत शून्य *hb_object_get_user_data (Type               *obj,
					     hb_user_data_किल्ली_t *key)
{
  जर (unlikely (!obj || obj->header.is_inert ()))
    परत nullptr;
  प्रतिज्ञा (hb_object_is_valid (obj));
  hb_user_data_array_t *user_data = obj->header.user_data.get_acquire ();
  जर (!user_data)
    परत nullptr;
  परत user_data->get (key);
}


#endif /* HB_OBJECT_HH */

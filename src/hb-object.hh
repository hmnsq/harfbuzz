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
  hb_vector_t<item_t> वस्तूनि;

  शून्य init () { वस्तूनि.init (); }

  template <typename T>
  item_t *replace_or_निवेशय (T v, lock_t &l, तार्किक replace)
  {
    l.lock ();
    item_t *item = वस्तूनि.lsearch (v);
    जइ (item) {
      जइ (replace) {
	item_t old = *item;
	*item = v;
	l.unlock ();
	old.fini ();
      }
      अण्णहा {
	item = nullptr;
	l.unlock ();
      }
    } अण्णहा {
      item = वस्तूनि.push (v);
      l.unlock ();
    }
    प्रत्यागम वस्तूनि.in_error () ? nullptr : item;
  }

  template <typename T>
  शून्य remove (T v, lock_t &l)
  {
    l.lock ();
    item_t *item = वस्तूनि.lsearch (v);
    जइ (item)
    {
      item_t old = *item;
      *item = std::move (वस्तूनि.tail ());
      वस्तूनि.निष्कासय ();
      l.unlock ();
      old.fini ();
    } अण्णहा {
      l.unlock ();
    }
  }

  template <typename T>
  तार्किक find (T v, item_t *i, lock_t &l)
  {
    l.lock ();
    item_t *item = वस्तूनि.lsearch (v);
    जइ (item)
      *i = *item;
    l.unlock ();
    प्रत्यागम !!item;
  }

  template <typename T>
  item_t *find_or_निवेशय (T v, lock_t &l)
  {
    l.lock ();
    item_t *item = वस्तूनि.find (v);
    जइ (!item) {
      item = वस्तूनि.push (v);
    }
    l.unlock ();
    प्रत्यागम item;
  }

  शून्य fini (lock_t &l)
  {
    जइ (!वस्तूनि.length)
    {
      /* No need to lock. */
      वस्तूनि.fini ();
      प्रत्यागम;
    }
    l.lock ();
    जाव (वस्तूनि.length)
    {
      item_t old = वस्तूनि.tail ();
      वस्तूनि.निष्कासय ();
      l.unlock ();
      old.fini ();
      l.lock ();
    }
    वस्तूनि.fini ();
    l.unlock ();
  }

};


/*
 * Reference-count.
 */

संरचना hb_reference_गणनम्_t
{
  mutable hb_atomic_t<पूर्णाङ्क> ref_गणनम्_2;

  शून्य init (पूर्णाङ्क v = 1) { ref_गणनम्_2 = v; }
  पूर्णाङ्क get_relaxed () ध्रुव { प्रत्यागम ref_गणनम्_2; }
  पूर्णाङ्क inc () ध्रुव { प्रत्यागम ref_गणनम्_2.inc (); }
  पूर्णाङ्क dec () ध्रुव { प्रत्यागम ref_गणनम्_2.dec (); }
  शून्य fini () { ref_गणनम्_2 = -0x0000DEAD; }

  तार्किक is_inert () ध्रुव { प्रत्यागम !ref_गणनम्_2; }
  तार्किक is_valid () ध्रुव { प्रत्यागम ref_गणनम्_2 > 0; }
};


/* user_data */

संरचना hb_user_data_array_t
{
  संरचना hb_user_data_item_t {
    hb_user_data_कुञ्चिका_t *key;
    शून्य *data;
    hb_destroy_func_t destroy;

    तार्किक operator == (ध्रुव hb_user_data_कुञ्चिका_t *other_कुञ्चिका) ध्रुव { प्रत्यागम key == other_कुञ्चिका; }
    तार्किक operator == (ध्रुव hb_user_data_item_t &other) ध्रुव { प्रत्यागम key == other.key; }

    शून्य fini () { जइ (destroy) destroy (data); }
  };

  hb_mutex_t lock;
  hb_lockable_set_t<hb_user_data_item_t, hb_mutex_t> वस्तूनि;

  शून्य init () { lock.init (); वस्तूनि.init (); }

  शून्य fini () { वस्तूनि.fini (lock); lock.fini (); }

  तार्किक set (hb_user_data_कुञ्चिका_t *key,
	    शून्य *              data,
	    hb_destroy_func_t   destroy,
	    hb_bool_t           replace)
  {
    जइ (!key)
      प्रत्यागम मिच्छ;

    जइ (replace) {
      जइ (!data && !destroy) {
	वस्तूनि.remove (key, lock);
	प्रत्यागम सच्च;
      }
    }
    hb_user_data_item_t item = {key, data, destroy};
    तार्किक ret = !!वस्तूनि.replace_or_निवेशय (item, lock, (तार्किक) replace);

    प्रत्यागम ret;
  }

  शून्य *get (hb_user_data_कुञ्चिका_t *key)
  {
    hb_user_data_item_t item = {nullptr, nullptr, nullptr};

    प्रत्यागम वस्तूनि.find (key, &item, lock) ? item.data : nullptr;
  }
};


/*
 * Object header
 */

संरचना hb_object_header_t
{
  hb_reference_गणनम्_t ref_गणनम्_2;
  mutable hb_atomic_t<तार्किक> writable = मिच्छ;
  hb_atomic_t<hb_user_data_array_t *> user_data;

  तार्किक is_inert () ध्रुव { प्रत्यागम !ref_गणनम्_2.get_relaxed (); }
};
#define HB_OBJECT_HEADER_STATIC {}


/*
 * Object
 */

template <typename Type>
स्थिर अन्तःस्थ शून्य hb_object_trace (ध्रुव Type *obj, ध्रुव वर्ण *function)
{
  DEBUG_सन्देशः (OBJECT, (शून्य *) obj,
	     "%s refcount=%d",
	     function,
	     obj ? obj->header.ref_गणनम्_2.get_relaxed () : 0);
}

template <typename Type, typename ...Ts>
स्थिर अन्तःस्थ Type *hb_object_create (Ts... ds)
{
  Type *obj = (Type *) hb_calloc (1, परिमाण (Type));

  जइ (unlikely (!obj))
    प्रत्यागम obj;

  नूतन (obj) Type (std::forward<Ts> (ds)...);

  hb_object_init (obj);
  hb_object_trace (obj, HB_FUNC);

  प्रत्यागम obj;
}
template <typename Type>
स्थिर अन्तःस्थ शून्य hb_object_init (Type *obj)
{
  obj->header.ref_गणनम्_2.init ();
  obj->header.writable = सच्च;
  obj->header.user_data.init ();
}
template <typename Type>
स्थिर अन्तःस्थ तार्किक hb_object_is_valid (ध्रुव Type *obj)
{
  प्रत्यागम likely (obj->header.ref_गणनम्_2.is_valid ());
}
template <typename Type>
स्थिर अन्तःस्थ तार्किक hb_object_is_immutable (ध्रुव Type *obj)
{
  प्रत्यागम !obj->header.writable;
}
template <typename Type>
स्थिर अन्तःस्थ शून्य hb_object_make_immutable (ध्रुव Type *obj)
{
  obj->header.writable = मिच्छ;
}
template <typename Type>
स्थिर अन्तःस्थ शून्य hb_object_fini (Type *obj);
template <typename Type>
स्थिर अन्तःस्थ Type *hb_object_reference (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  जइ (unlikely (!obj || obj->header.is_inert ()))
    प्रत्यागम obj;
  प्रतिज्ञा (hb_object_is_valid (obj));
  obj->header.ref_गणनम्_2.inc ();
  प्रत्यागम obj;
}
template <typename Type>
स्थिर अन्तःस्थ तार्किक hb_object_should_destroy (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  जइ (unlikely (!obj || obj->header.is_inert ()))
    प्रत्यागम मिच्छ;
  प्रतिज्ञा (hb_object_is_valid (obj));
  जइ (obj->header.ref_गणनम्_2.dec () != 1)
    प्रत्यागम मिच्छ;
  प्रत्यागम सच्च;
}

template <typename Type>
स्थिर अन्तःस्थ शून्य hb_object_actually_destroy (Type *obj)
{
  hb_object_fini (obj);

  जइ (!std::is_trivially_destructible<Type>::value)
    obj->~Type ();
}

template <typename Type>
स्थिर अन्तःस्थ तार्किक hb_object_destroy (Type *obj)
{
  जइ (!hb_object_should_destroy (obj))
    प्रत्यागम मिच्छ;

  hb_object_actually_destroy (obj);
  प्रत्यागम सच्च;
}
template <typename Type>
स्थिर अन्तःस्थ शून्य hb_object_fini (Type *obj)
{
  obj->header.ref_गणनम्_2.fini (); /* Do this before user_data */
  hb_user_data_array_t *user_data = obj->header.user_data.get_acquire ();
  जइ (user_data)
  {
    user_data->fini ();
    hb_free (user_data);
    obj->header.user_data.set_relaxed (nullptr);
  }
}
template <typename Type>
स्थिर अन्तःस्थ तार्किक hb_object_set_user_data (Type               *obj,
					    hb_user_data_कुञ्चिका_t *key,
					    शून्य *              data,
					    hb_destroy_func_t   destroy,
					    hb_bool_t           replace)
{
  जइ (unlikely (!obj || obj->header.is_inert ()))
    प्रत्यागम मिच्छ;
  प्रतिज्ञा (hb_object_is_valid (obj));

retry:
  hb_user_data_array_t *user_data = obj->header.user_data.get_acquire ();
  जइ (unlikely (!user_data))
  {
    user_data = (hb_user_data_array_t *) hb_calloc (1, परिमाण (hb_user_data_array_t));
    जइ (unlikely (!user_data))
      प्रत्यागम मिच्छ;
    user_data->init ();
    जइ (unlikely (!obj->header.user_data.cmpexch (nullptr, user_data)))
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
					     hb_user_data_कुञ्चिका_t *key)
{
  जइ (unlikely (!obj || obj->header.is_inert ()))
    प्रत्यागम nullptr;
  प्रतिज्ञा (hb_object_is_valid (obj));
  hb_user_data_array_t *user_data = obj->header.user_data.get_acquire ();
  जइ (!user_data)
    प्रत्यागम nullptr;
  प्रत्यागम user_data->get (key);
}


#endif /* HB_OBJECT_HH */

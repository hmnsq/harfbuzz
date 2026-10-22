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

#அந்தர்பாவ "hb.hh"
#அந்தர்பாவ "hb-atomic.hh"
#அந்தர்பாவ "hb-mutex.hh"
#அந்தர்பாவ "hb-vector.hh"


/*
 * Lockable set
 */

template <typename item_t, typename lock_t>
கட்டமைப்பு hb_lockable_set_t
{
  hb_vector_t<item_t> பொருட்கள்;

  வெறுமை init () { பொருட்கள்.init (); }

  template <typename T>
  item_t *replace_or_செருகு (T v, lock_t &l, தருக்க replace)
  {
    l.lock ();
    item_t *item = பொருட்கள்.lsearch (v);
    எனில் (item) {
      எனில் (replace) {
	item_t old = *item;
	*item = v;
	l.unlock ();
	old.fini ();
      }
      இல்லையெனில் {
	item = nullptr;
	l.unlock ();
      }
    } இல்லையெனில் {
      item = பொருட்கள்.push (v);
      l.unlock ();
    }
    திரும்பு பொருட்கள்.in_error () ? nullptr : item;
  }

  template <typename T>
  வெறுமை remove (T v, lock_t &l)
  {
    l.lock ();
    item_t *item = பொருட்கள்.lsearch (v);
    எனில் (item)
    {
      item_t old = *item;
      *item = std::move (பொருட்கள்.tail ());
      பொருட்கள்.எடு ();
      l.unlock ();
      old.fini ();
    } இல்லையெனில் {
      l.unlock ();
    }
  }

  template <typename T>
  தருக்க find (T v, item_t *i, lock_t &l)
  {
    l.lock ();
    item_t *item = பொருட்கள்.lsearch (v);
    எனில் (item)
      *i = *item;
    l.unlock ();
    திரும்பு !!item;
  }

  template <typename T>
  item_t *find_or_செருகு (T v, lock_t &l)
  {
    l.lock ();
    item_t *item = பொருட்கள்.find (v);
    எனில் (!item) {
      item = பொருட்கள்.push (v);
    }
    l.unlock ();
    திரும்பு item;
  }

  வெறுமை fini (lock_t &l)
  {
    எனில் (!பொருட்கள்.length)
    {
      /* No need to lock. */
      பொருட்கள்.fini ();
      திரும்பு;
    }
    l.lock ();
    போது (பொருட்கள்.length)
    {
      item_t old = பொருட்கள்.tail ();
      பொருட்கள்.எடு ();
      l.unlock ();
      old.fini ();
      l.lock ();
    }
    பொருட்கள்.fini ();
    l.unlock ();
  }

};


/*
 * Reference-count.
 */

கட்டமைப்பு hb_reference_மொத்தம்_t
{
  mutable hb_atomic_t<முழு> ref_மொத்தம்_2;

  வெறுமை init (முழு v = 1) { ref_மொத்தம்_2 = v; }
  முழு get_relaxed () மாறிலி { திரும்பு ref_மொத்தம்_2; }
  முழு inc () மாறிலி { திரும்பு ref_மொத்தம்_2.inc (); }
  முழு dec () மாறிலி { திரும்பு ref_மொத்தம்_2.dec (); }
  வெறுமை fini () { ref_மொத்தம்_2 = -0x0000DEAD; }

  தருக்க is_inert () மாறிலி { திரும்பு !ref_மொத்தம்_2; }
  தருக்க is_valid () மாறிலி { திரும்பு ref_மொத்தம்_2 > 0; }
};


/* user_data */

கட்டமைப்பு hb_user_data_array_t
{
  கட்டமைப்பு hb_user_data_item_t {
    hb_user_data_சாவி_t *key;
    வெறுமை *data;
    hb_destroy_func_t destroy;

    தருக்க operator == (மாறிலி hb_user_data_சாவி_t *other_சாவி) மாறிலி { திரும்பு key == other_சாவி; }
    தருக்க operator == (மாறிலி hb_user_data_item_t &other) மாறிலி { திரும்பு key == other.key; }

    வெறுமை fini () { எனில் (destroy) destroy (data); }
  };

  hb_mutex_t lock;
  hb_lockable_set_t<hb_user_data_item_t, hb_mutex_t> பொருட்கள்;

  வெறுமை init () { lock.init (); பொருட்கள்.init (); }

  வெறுமை fini () { பொருட்கள்.fini (lock); lock.fini (); }

  தருக்க set (hb_user_data_சாவி_t *key,
	    வெறுமை *              data,
	    hb_destroy_func_t   destroy,
	    hb_bool_t           replace)
  {
    எனில் (!key)
      திரும்பு பொய்;

    எனில் (replace) {
      எனில் (!data && !destroy) {
	பொருட்கள்.remove (key, lock);
	திரும்பு உண்மை;
      }
    }
    hb_user_data_item_t item = {key, data, destroy};
    தருக்க ret = !!பொருட்கள்.replace_or_செருகு (item, lock, (தருக்க) replace);

    திரும்பு ret;
  }

  வெறுமை *get (hb_user_data_சாவி_t *key)
  {
    hb_user_data_item_t item = {nullptr, nullptr, nullptr};

    திரும்பு பொருட்கள்.find (key, &item, lock) ? item.data : nullptr;
  }
};


/*
 * Object header
 */

கட்டமைப்பு hb_object_header_t
{
  hb_reference_மொத்தம்_t ref_மொத்தம்_2;
  mutable hb_atomic_t<தருக்க> writable = பொய்;
  hb_atomic_t<hb_user_data_array_t *> user_data;

  தருக்க is_inert () மாறிலி { திரும்பு !ref_மொத்தம்_2.get_relaxed (); }
};
#define HB_OBJECT_HEADER_STATIC {}


/*
 * Object
 */

template <typename Type>
நிலையான உள்வரி வெறுமை hb_object_trace (மாறிலி Type *obj, மாறிலி எழுத்து *function)
{
  DEBUG_செய்தி (OBJECT, (வெறுமை *) obj,
	     "%s refcount=%d",
	     function,
	     obj ? obj->header.ref_மொத்தம்_2.get_relaxed () : 0);
}

template <typename Type, typename ...Ts>
நிலையான உள்வரி Type *hb_object_create (Ts... ds)
{
  Type *obj = (Type *) hb_calloc (1, அளவு (Type));

  எனில் (unlikely (!obj))
    திரும்பு obj;

  புதிய (obj) Type (std::forward<Ts> (ds)...);

  hb_object_init (obj);
  hb_object_trace (obj, HB_FUNC);

  திரும்பு obj;
}
template <typename Type>
நிலையான உள்வரி வெறுமை hb_object_init (Type *obj)
{
  obj->header.ref_மொத்தம்_2.init ();
  obj->header.writable = உண்மை;
  obj->header.user_data.init ();
}
template <typename Type>
நிலையான உள்வரி தருக்க hb_object_is_valid (மாறிலி Type *obj)
{
  திரும்பு likely (obj->header.ref_மொத்தம்_2.is_valid ());
}
template <typename Type>
நிலையான உள்வரி தருக்க hb_object_is_immutable (மாறிலி Type *obj)
{
  திரும்பு !obj->header.writable;
}
template <typename Type>
நிலையான உள்வரி வெறுமை hb_object_make_immutable (மாறிலி Type *obj)
{
  obj->header.writable = பொய்;
}
template <typename Type>
நிலையான உள்வரி வெறுமை hb_object_fini (Type *obj);
template <typename Type>
நிலையான உள்வரி Type *hb_object_reference (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  எனில் (unlikely (!obj || obj->header.is_inert ()))
    திரும்பு obj;
  ப்ரதிஜ்ஞா (hb_object_is_valid (obj));
  obj->header.ref_மொத்தம்_2.inc ();
  திரும்பு obj;
}
template <typename Type>
நிலையான உள்வரி தருக்க hb_object_should_destroy (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  எனில் (unlikely (!obj || obj->header.is_inert ()))
    திரும்பு பொய்;
  ப்ரதிஜ்ஞா (hb_object_is_valid (obj));
  எனில் (obj->header.ref_மொத்தம்_2.dec () != 1)
    திரும்பு பொய்;
  திரும்பு உண்மை;
}

template <typename Type>
நிலையான உள்வரி வெறுமை hb_object_actually_destroy (Type *obj)
{
  hb_object_fini (obj);

  எனில் (!std::is_trivially_destructible<Type>::value)
    obj->~Type ();
}

template <typename Type>
நிலையான உள்வரி தருக்க hb_object_destroy (Type *obj)
{
  எனில் (!hb_object_should_destroy (obj))
    திரும்பு பொய்;

  hb_object_actually_destroy (obj);
  திரும்பு உண்மை;
}
template <typename Type>
நிலையான உள்வரி வெறுமை hb_object_fini (Type *obj)
{
  obj->header.ref_மொத்தம்_2.fini (); /* Do this before user_data */
  hb_user_data_array_t *user_data = obj->header.user_data.get_acquire ();
  எனில் (user_data)
  {
    user_data->fini ();
    hb_free (user_data);
    obj->header.user_data.set_relaxed (nullptr);
  }
}
template <typename Type>
நிலையான உள்வரி தருக்க hb_object_set_user_data (Type               *obj,
					    hb_user_data_சாவி_t *key,
					    வெறுமை *              data,
					    hb_destroy_func_t   destroy,
					    hb_bool_t           replace)
{
  எனில் (unlikely (!obj || obj->header.is_inert ()))
    திரும்பு பொய்;
  ப்ரதிஜ்ஞா (hb_object_is_valid (obj));

retry:
  hb_user_data_array_t *user_data = obj->header.user_data.get_acquire ();
  எனில் (unlikely (!user_data))
  {
    user_data = (hb_user_data_array_t *) hb_calloc (1, அளவு (hb_user_data_array_t));
    எனில் (unlikely (!user_data))
      திரும்பு பொய்;
    user_data->init ();
    எனில் (unlikely (!obj->header.user_data.cmpexch (nullptr, user_data)))
    {
      user_data->fini ();
      hb_free (user_data);
      செல் retry;
    }
  }

  திரும்பு user_data->set (key, data, destroy, replace);
}

template <typename Type>
நிலையான உள்வரி வெறுமை *hb_object_get_user_data (Type               *obj,
					     hb_user_data_சாவி_t *key)
{
  எனில் (unlikely (!obj || obj->header.is_inert ()))
    திரும்பு nullptr;
  ப்ரதிஜ்ஞா (hb_object_is_valid (obj));
  hb_user_data_array_t *user_data = obj->header.user_data.get_acquire ();
  எனில் (!user_data)
    திரும்பு nullptr;
  திரும்பு user_data->get (key);
}


#endif /* HB_OBJECT_HH */

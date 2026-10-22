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

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-atomic.hh"
#ଅନ୍ତର୍ଭାଵ "hb-mutex.hh"
#ଅନ୍ତର୍ଭାଵ "hb-vector.hh"


/*
 * Lockable set
 */

template <typename item_t, typename lock_t>
ସଂରଚନା hb_lockable_set_t
{
  hb_vector_t<item_t> ବସ୍ତୁଗୁଡ଼ିକ;

  ଶୂନ୍ୟ init () { ବସ୍ତୁଗୁଡ଼ିକ.init (); }

  template <typename T>
  item_t *replace_or_ଭର୍ତ୍ତି (T v, lock_t &l, ତାର୍କିକ replace)
  {
    l.lock ();
    item_t *item = ବସ୍ତୁଗୁଡ଼ିକ.lsearch (v);
    ଯଦି (item) {
      ଯଦି (replace) {
	item_t old = *item;
	*item = v;
	l.unlock ();
	old.fini ();
      }
      ଅନ୍ୟଥା {
	item = nullptr;
	l.unlock ();
      }
    } ଅନ୍ୟଥା {
      item = ବସ୍ତୁଗୁଡ଼ିକ.push (v);
      l.unlock ();
    }
    ଫେରନ୍ତୁ ବସ୍ତୁଗୁଡ଼ିକ.in_error () ? nullptr : item;
  }

  template <typename T>
  ଶୂନ୍ୟ remove (T v, lock_t &l)
  {
    l.lock ();
    item_t *item = ବସ୍ତୁଗୁଡ଼ିକ.lsearch (v);
    ଯଦି (item)
    {
      item_t old = *item;
      *item = std::move (ବସ୍ତୁଗୁଡ଼ିକ.tail ());
      ବସ୍ତୁଗୁଡ଼ିକ.କାଢ଼ ();
      l.unlock ();
      old.fini ();
    } ଅନ୍ୟଥା {
      l.unlock ();
    }
  }

  template <typename T>
  ତାର୍କିକ find (T v, item_t *i, lock_t &l)
  {
    l.lock ();
    item_t *item = ବସ୍ତୁଗୁଡ଼ିକ.lsearch (v);
    ଯଦି (item)
      *i = *item;
    l.unlock ();
    ଫେରନ୍ତୁ !!item;
  }

  template <typename T>
  item_t *find_or_ଭର୍ତ୍ତି (T v, lock_t &l)
  {
    l.lock ();
    item_t *item = ବସ୍ତୁଗୁଡ଼ିକ.find (v);
    ଯଦି (!item) {
      item = ବସ୍ତୁଗୁଡ଼ିକ.push (v);
    }
    l.unlock ();
    ଫେରନ୍ତୁ item;
  }

  ଶୂନ୍ୟ fini (lock_t &l)
  {
    ଯଦି (!ବସ୍ତୁଗୁଡ଼ିକ.length)
    {
      /* No need to lock. */
      ବସ୍ତୁଗୁଡ଼ିକ.fini ();
      ଫେରନ୍ତୁ;
    }
    l.lock ();
    ଯେତେବେଳେ (ବସ୍ତୁଗୁଡ଼ିକ.length)
    {
      item_t old = ବସ୍ତୁଗୁଡ଼ିକ.tail ();
      ବସ୍ତୁଗୁଡ଼ିକ.କାଢ଼ ();
      l.unlock ();
      old.fini ();
      l.lock ();
    }
    ବସ୍ତୁଗୁଡ଼ିକ.fini ();
    l.unlock ();
  }

};


/*
 * Reference-count.
 */

ସଂରଚନା hb_reference_ଗଣନା_t
{
  mutable hb_atomic_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> ref_ଗଣନା_2;

  ଶୂନ୍ୟ init (ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = 1) { ref_ଗଣନା_2 = v; }
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_relaxed () ଅଚଳ { ଫେରନ୍ତୁ ref_ଗଣନା_2; }
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା inc () ଅଚଳ { ଫେରନ୍ତୁ ref_ଗଣନା_2.inc (); }
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା dec () ଅଚଳ { ଫେରନ୍ତୁ ref_ଗଣନା_2.dec (); }
  ଶୂନ୍ୟ fini () { ref_ଗଣନା_2 = -0x0000DEAD; }

  ତାର୍କିକ is_inert () ଅଚଳ { ଫେରନ୍ତୁ !ref_ଗଣନା_2; }
  ତାର୍କିକ is_valid () ଅଚଳ { ଫେରନ୍ତୁ ref_ଗଣନା_2 > 0; }
};


/* user_data */

ସଂରଚନା hb_user_data_array_t
{
  ସଂରଚନା hb_user_data_item_t {
    hb_user_data_ଚାବି_t *key;
    ଶୂନ୍ୟ *data;
    hb_destroy_func_t destroy;

    ତାର୍କିକ operator == (ଅଚଳ hb_user_data_ଚାବି_t *other_ଚାବି) ଅଚଳ { ଫେରନ୍ତୁ key == other_ଚାବି; }
    ତାର୍କିକ operator == (ଅଚଳ hb_user_data_item_t &other) ଅଚଳ { ଫେରନ୍ତୁ key == other.key; }

    ଶୂନ୍ୟ fini () { ଯଦି (destroy) destroy (data); }
  };

  hb_mutex_t lock;
  hb_lockable_set_t<hb_user_data_item_t, hb_mutex_t> ବସ୍ତୁଗୁଡ଼ିକ;

  ଶୂନ୍ୟ init () { lock.init (); ବସ୍ତୁଗୁଡ଼ିକ.init (); }

  ଶୂନ୍ୟ fini () { ବସ୍ତୁଗୁଡ଼ିକ.fini (lock); lock.fini (); }

  ତାର୍କିକ set (hb_user_data_ଚାବି_t *key,
	    ଶୂନ୍ୟ *              data,
	    hb_destroy_func_t   destroy,
	    hb_bool_t           replace)
  {
    ଯଦି (!key)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଯଦି (replace) {
      ଯଦି (!data && !destroy) {
	ବସ୍ତୁଗୁଡ଼ିକ.remove (key, lock);
	ଫେରନ୍ତୁ ସତ୍ୟ;
      }
    }
    hb_user_data_item_t item = {key, data, destroy};
    ତାର୍କିକ ret = !!ବସ୍ତୁଗୁଡ଼ିକ.replace_or_ଭର୍ତ୍ତି (item, lock, (ତାର୍କିକ) replace);

    ଫେରନ୍ତୁ ret;
  }

  ଶୂନ୍ୟ *get (hb_user_data_ଚାବି_t *key)
  {
    hb_user_data_item_t item = {nullptr, nullptr, nullptr};

    ଫେରନ୍ତୁ ବସ୍ତୁଗୁଡ଼ିକ.find (key, &item, lock) ? item.data : nullptr;
  }
};


/*
 * Object header
 */

ସଂରଚନା hb_object_header_t
{
  hb_reference_ଗଣନା_t ref_ଗଣନା_2;
  mutable hb_atomic_t<ତାର୍କିକ> writable = ମିଥ୍ୟା;
  hb_atomic_t<hb_user_data_array_t *> user_data;

  ତାର୍କିକ is_inert () ଅଚଳ { ଫେରନ୍ତୁ !ref_ଗଣନା_2.get_relaxed (); }
};
#define HB_OBJECT_HEADER_STATIC {}


/*
 * Object
 */

template <typename Type>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ hb_object_trace (ଅଚଳ Type *obj, ଅଚଳ ଅକ୍ଷର *function)
{
  DEBUG_ସନ୍ଦେଶ (OBJECT, (ଶୂନ୍ୟ *) obj,
	     "%s refcount=%d",
	     function,
	     obj ? obj->header.ref_ଗଣନା_2.get_relaxed () : 0);
}

template <typename Type, typename ...Ts>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ Type *hb_object_create (Ts... ds)
{
  Type *obj = (Type *) hb_calloc (1, ଆକାର (Type));

  ଯଦି (unlikely (!obj))
    ଫେରନ୍ତୁ obj;

  ନୂତନ (obj) Type (std::forward<Ts> (ds)...);

  hb_object_init (obj);
  hb_object_trace (obj, HB_FUNC);

  ଫେରନ୍ତୁ obj;
}
template <typename Type>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ hb_object_init (Type *obj)
{
  obj->header.ref_ଗଣନା_2.init ();
  obj->header.writable = ସତ୍ୟ;
  obj->header.user_data.init ();
}
template <typename Type>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ hb_object_is_valid (ଅଚଳ Type *obj)
{
  ଫେରନ୍ତୁ likely (obj->header.ref_ଗଣନା_2.is_valid ());
}
template <typename Type>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ hb_object_is_immutable (ଅଚଳ Type *obj)
{
  ଫେରନ୍ତୁ !obj->header.writable;
}
template <typename Type>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ hb_object_make_immutable (ଅଚଳ Type *obj)
{
  obj->header.writable = ମିଥ୍ୟା;
}
template <typename Type>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ hb_object_fini (Type *obj);
template <typename Type>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ Type *hb_object_reference (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  ଯଦି (unlikely (!obj || obj->header.is_inert ()))
    ଫେରନ୍ତୁ obj;
  ପ୍ରତିଜ୍ଞା (hb_object_is_valid (obj));
  obj->header.ref_ଗଣନା_2.inc ();
  ଫେରନ୍ତୁ obj;
}
template <typename Type>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ hb_object_should_destroy (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  ଯଦି (unlikely (!obj || obj->header.is_inert ()))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  ପ୍ରତିଜ୍ଞା (hb_object_is_valid (obj));
  ଯଦି (obj->header.ref_ଗଣନା_2.dec () != 1)
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

template <typename Type>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ hb_object_actually_destroy (Type *obj)
{
  hb_object_fini (obj);

  ଯଦି (!std::is_trivially_destructible<Type>::value)
    obj->~Type ();
}

template <typename Type>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ hb_object_destroy (Type *obj)
{
  ଯଦି (!hb_object_should_destroy (obj))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  hb_object_actually_destroy (obj);
  ଫେରନ୍ତୁ ସତ୍ୟ;
}
template <typename Type>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ hb_object_fini (Type *obj)
{
  obj->header.ref_ଗଣନା_2.fini (); /* Do this before user_data */
  hb_user_data_array_t *user_data = obj->header.user_data.get_acquire ();
  ଯଦି (user_data)
  {
    user_data->fini ();
    hb_free (user_data);
    obj->header.user_data.set_relaxed (nullptr);
  }
}
template <typename Type>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ hb_object_set_user_data (Type               *obj,
					    hb_user_data_ଚାବି_t *key,
					    ଶୂନ୍ୟ *              data,
					    hb_destroy_func_t   destroy,
					    hb_bool_t           replace)
{
  ଯଦି (unlikely (!obj || obj->header.is_inert ()))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  ପ୍ରତିଜ୍ଞା (hb_object_is_valid (obj));

retry:
  hb_user_data_array_t *user_data = obj->header.user_data.get_acquire ();
  ଯଦି (unlikely (!user_data))
  {
    user_data = (hb_user_data_array_t *) hb_calloc (1, ଆକାର (hb_user_data_array_t));
    ଯଦି (unlikely (!user_data))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    user_data->init ();
    ଯଦି (unlikely (!obj->header.user_data.cmpexch (nullptr, user_data)))
    {
      user_data->fini ();
      hb_free (user_data);
      ଯାଅ retry;
    }
  }

  ଫେରନ୍ତୁ user_data->set (key, data, destroy, replace);
}

template <typename Type>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ *hb_object_get_user_data (Type               *obj,
					     hb_user_data_ଚାବି_t *key)
{
  ଯଦି (unlikely (!obj || obj->header.is_inert ()))
    ଫେରନ୍ତୁ nullptr;
  ପ୍ରତିଜ୍ଞା (hb_object_is_valid (obj));
  hb_user_data_array_t *user_data = obj->header.user_data.get_acquire ();
  ଯଦି (!user_data)
    ଫେରନ୍ତୁ nullptr;
  ଫେରନ୍ତୁ user_data->get (key);
}


#endif /* HB_OBJECT_HH */

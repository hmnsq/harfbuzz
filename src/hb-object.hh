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

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-atomic.hh"
#অন্তর্ভাব "hb-mutex.hh"
#অন্তর্ভাব "hb-vector.hh"


/*
 * Lockable set
 */

template <typename item_t, typename lock_t>
কাঠামো hb_lockable_set_t
{
  hb_vector_t<item_t> বস্তুসমূহ;

  শূন্য init () { বস্তুসমূহ.init (); }

  template <typename T>
  item_t *replace_or_ঢোকাও (T v, lock_t &l, যৌক্তিক replace)
  {
    l.lock ();
    item_t *item = বস্তুসমূহ.lsearch (v);
    যদি (item) {
      যদি (replace) {
	item_t old = *item;
	*item = v;
	l.unlock ();
	old.fini ();
      }
      অন্যথা {
	item = nullptr;
	l.unlock ();
      }
    } অন্যথা {
      item = বস্তুসমূহ.push (v);
      l.unlock ();
    }
    ফেরত বস্তুসমূহ.in_error () ? nullptr : item;
  }

  template <typename T>
  শূন্য remove (T v, lock_t &l)
  {
    l.lock ();
    item_t *item = বস্তুসমূহ.lsearch (v);
    যদি (item)
    {
      item_t old = *item;
      *item = std::move (বস্তুসমূহ.লেজ ());
      বস্তুসমূহ.বের_করো ();
      l.unlock ();
      old.fini ();
    } অন্যথা {
      l.unlock ();
    }
  }

  template <typename T>
  যৌক্তিক find (T v, item_t *i, lock_t &l)
  {
    l.lock ();
    item_t *item = বস্তুসমূহ.lsearch (v);
    যদি (item)
      *i = *item;
    l.unlock ();
    ফেরত !!item;
  }

  template <typename T>
  item_t *find_or_ঢোকাও (T v, lock_t &l)
  {
    l.lock ();
    item_t *item = বস্তুসমূহ.find (v);
    যদি (!item) {
      item = বস্তুসমূহ.push (v);
    }
    l.unlock ();
    ফেরত item;
  }

  শূন্য fini (lock_t &l)
  {
    যদি (!বস্তুসমূহ.দৈর্ঘ্য)
    {
      /* No need to lock. */
      বস্তুসমূহ.fini ();
      ফেরত;
    }
    l.lock ();
    যখন (বস্তুসমূহ.দৈর্ঘ্য)
    {
      item_t old = বস্তুসমূহ.লেজ ();
      বস্তুসমূহ.বের_করো ();
      l.unlock ();
      old.fini ();
      l.lock ();
    }
    বস্তুসমূহ.fini ();
    l.unlock ();
  }

};


/*
 * Reference-count.
 */

কাঠামো hb_reference_গুনতি_t
{
  mutable hb_atomic_t<পূর্ণসংখ্যা> ref_গুনতি_2;

  শূন্য init (পূর্ণসংখ্যা v = 1) { ref_গুনতি_2 = v; }
  পূর্ণসংখ্যা get_relaxed () ধ্রুবক { ফেরত ref_গুনতি_2; }
  পূর্ণসংখ্যা inc () ধ্রুবক { ফেরত ref_গুনতি_2.inc (); }
  পূর্ণসংখ্যা dec () ধ্রুবক { ফেরত ref_গুনতি_2.dec (); }
  শূন্য fini () { ref_গুনতি_2 = -0x0000DEAD; }

  যৌক্তিক is_inert () ধ্রুবক { ফেরত !ref_গুনতি_2; }
  যৌক্তিক is_valid () ধ্রুবক { ফেরত ref_গুনতি_2 > 0; }
};


/* user_data */

কাঠামো hb_user_তথ্য_array_t
{
  কাঠামো hb_user_তথ্য_item_t {
    hb_user_তথ্য_চাবি_t *key;
    শূন্য *তথ্য;
    hb_destroy_func_t destroy;

    যৌক্তিক operator == (ধ্রুবক hb_user_তথ্য_চাবি_t *other_চাবি) ধ্রুবক { ফেরত key == other_চাবি; }
    যৌক্তিক operator == (ধ্রুবক hb_user_তথ্য_item_t &other) ধ্রুবক { ফেরত key == other.key; }

    শূন্য fini () { যদি (destroy) destroy (তথ্য); }
  };

  hb_mutex_t lock;
  hb_lockable_set_t<hb_user_তথ্য_item_t, hb_mutex_t> বস্তুসমূহ;

  শূন্য init () { lock.init (); বস্তুসমূহ.init (); }

  শূন্য fini () { বস্তুসমূহ.fini (lock); lock.fini (); }

  যৌক্তিক set (hb_user_তথ্য_চাবি_t *key,
	    শূন্য *              তথ্য,
	    hb_destroy_func_t   destroy,
	    hb_bool_t           replace)
  {
    যদি (!key)
      ফেরত মিথ্যা;

    যদি (replace) {
      যদি (!তথ্য && !destroy) {
	বস্তুসমূহ.remove (key, lock);
	ফেরত সত্য;
      }
    }
    hb_user_তথ্য_item_t item = {key, তথ্য, destroy};
    যৌক্তিক ret = !!বস্তুসমূহ.replace_or_ঢোকাও (item, lock, (যৌক্তিক) replace);

    ফেরত ret;
  }

  শূন্য *get (hb_user_তথ্য_চাবি_t *key)
  {
    hb_user_তথ্য_item_t item = {nullptr, nullptr, nullptr};

    ফেরত বস্তুসমূহ.find (key, &item, lock) ? item.তথ্য : nullptr;
  }
};


/*
 * Object header
 */

কাঠামো hb_object_header_t
{
  hb_reference_গুনতি_t ref_গুনতি_2;
  mutable hb_atomic_t<যৌক্তিক> writable = মিথ্যা;
  hb_atomic_t<hb_user_তথ্য_array_t *> user_তথ্য;

  যৌক্তিক is_inert () ধ্রুবক { ফেরত !ref_গুনতি_2.get_relaxed (); }
};
#define HB_OBJECT_HEADER_STATIC {}


/*
 * Object
 */

template <typename Type>
স্থির অন্তঃরেখ শূন্য hb_object_trace (ধ্রুবক Type *obj, ধ্রুবক অক্ষর *function)
{
  DEBUG_বার্তা (OBJECT, (শূন্য *) obj,
	     "%s refcount=%d",
	     function,
	     obj ? obj->header.ref_গুনতি_2.get_relaxed () : 0);
}

template <typename Type, typename ...Ts>
স্থির অন্তঃরেখ Type *hb_object_create (Ts... ds)
{
  Type *obj = (Type *) hb_calloc (1, আকার (Type));

  যদি (unlikely (!obj))
    ফেরত obj;

  নতুন (obj) Type (std::forward<Ts> (ds)...);

  hb_object_init (obj);
  hb_object_trace (obj, HB_FUNC);

  ফেরত obj;
}
template <typename Type>
স্থির অন্তঃরেখ শূন্য hb_object_init (Type *obj)
{
  obj->header.ref_গুনতি_2.init ();
  obj->header.writable = সত্য;
  obj->header.user_তথ্য.init ();
}
template <typename Type>
স্থির অন্তঃরেখ যৌক্তিক hb_object_is_valid (ধ্রুবক Type *obj)
{
  ফেরত likely (obj->header.ref_গুনতি_2.is_valid ());
}
template <typename Type>
স্থির অন্তঃরেখ যৌক্তিক hb_object_is_immutable (ধ্রুবক Type *obj)
{
  ফেরত !obj->header.writable;
}
template <typename Type>
স্থির অন্তঃরেখ শূন্য hb_object_make_immutable (ধ্রুবক Type *obj)
{
  obj->header.writable = মিথ্যা;
}
template <typename Type>
স্থির অন্তঃরেখ শূন্য hb_object_fini (Type *obj);
template <typename Type>
স্থির অন্তঃরেখ Type *hb_object_reference (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  যদি (unlikely (!obj || obj->header.is_inert ()))
    ফেরত obj;
  প্রতিজ্ঞা (hb_object_is_valid (obj));
  obj->header.ref_গুনতি_2.inc ();
  ফেরত obj;
}
template <typename Type>
স্থির অন্তঃরেখ যৌক্তিক hb_object_should_destroy (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  যদি (unlikely (!obj || obj->header.is_inert ()))
    ফেরত মিথ্যা;
  প্রতিজ্ঞা (hb_object_is_valid (obj));
  যদি (obj->header.ref_গুনতি_2.dec () != 1)
    ফেরত মিথ্যা;
  ফেরত সত্য;
}

template <typename Type>
স্থির অন্তঃরেখ শূন্য hb_object_actually_destroy (Type *obj)
{
  hb_object_fini (obj);

  যদি (!std::is_trivially_destructible<Type>::value)
    obj->~Type ();
}

template <typename Type>
স্থির অন্তঃরেখ যৌক্তিক hb_object_destroy (Type *obj)
{
  যদি (!hb_object_should_destroy (obj))
    ফেরত মিথ্যা;

  hb_object_actually_destroy (obj);
  ফেরত সত্য;
}
template <typename Type>
স্থির অন্তঃরেখ শূন্য hb_object_fini (Type *obj)
{
  obj->header.ref_গুনতি_2.fini (); /* Do this before user_data */
  hb_user_তথ্য_array_t *user_তথ্য = obj->header.user_তথ্য.get_acquire ();
  যদি (user_তথ্য)
  {
    user_তথ্য->fini ();
    hb_free (user_তথ্য);
    obj->header.user_তথ্য.set_relaxed (nullptr);
  }
}
template <typename Type>
স্থির অন্তঃরেখ যৌক্তিক hb_object_set_user_তথ্য (Type               *obj,
					    hb_user_তথ্য_চাবি_t *key,
					    শূন্য *              তথ্য,
					    hb_destroy_func_t   destroy,
					    hb_bool_t           replace)
{
  যদি (unlikely (!obj || obj->header.is_inert ()))
    ফেরত মিথ্যা;
  প্রতিজ্ঞা (hb_object_is_valid (obj));

retry:
  hb_user_তথ্য_array_t *user_তথ্য = obj->header.user_তথ্য.get_acquire ();
  যদি (unlikely (!user_তথ্য))
  {
    user_তথ্য = (hb_user_তথ্য_array_t *) hb_calloc (1, আকার (hb_user_তথ্য_array_t));
    যদি (unlikely (!user_তথ্য))
      ফেরত মিথ্যা;
    user_তথ্য->init ();
    যদি (unlikely (!obj->header.user_তথ্য.cmpexch (nullptr, user_তথ্য)))
    {
      user_তথ্য->fini ();
      hb_free (user_তথ্য);
      যাও retry;
    }
  }

  ফেরত user_তথ্য->set (key, তথ্য, destroy, replace);
}

template <typename Type>
স্থির অন্তঃরেখ শূন্য *hb_object_get_user_তথ্য (Type               *obj,
					     hb_user_তথ্য_চাবি_t *key)
{
  যদি (unlikely (!obj || obj->header.is_inert ()))
    ফেরত nullptr;
  প্রতিজ্ঞা (hb_object_is_valid (obj));
  hb_user_তথ্য_array_t *user_তথ্য = obj->header.user_তথ্য.get_acquire ();
  যদি (!user_তথ্য)
    ফেরত nullptr;
  ফেরত user_তথ্য->get (key);
}


#endif /* HB_OBJECT_HH */

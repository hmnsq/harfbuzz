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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OBJECT_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OBJECT_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-atomic.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-mutex.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-vector.hh"


/*
 * Lockable set
 */

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਵਸਤੂ_t, ਕਿਸਮ_ਨਾਂ ਤਾਲਾ_t>
ਬਣਤਰ hb_lockable_ਸੈੱਟ_t
{
  hb_ਸਦਿਸ਼_t<ਵਸਤੂ_t> ਚੀਜ਼ਾਂ;

  ਖਾਲੀ init () { ਚੀਜ਼ਾਂ.init (); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਵਸਤੂ_t *replace_or_ਪਾਓ (T v, ਤਾਲਾ_t &l, ਤਾਰਕਿਕ replace)
  {
    l.lock ();
    ਵਸਤੂ_t *ਵਸਤੂ = ਚੀਜ਼ਾਂ.lsearch (v);
    ਜੇ (ਵਸਤੂ) {
      ਜੇ (replace) {
	ਵਸਤੂ_t old = *ਵਸਤੂ;
	*ਵਸਤੂ = v;
	l.unlock ();
	old.fini ();
      }
      ਨਹੀਂਤਾਂ {
	ਵਸਤੂ = ਖਾਲੀ_ਸੂਚਕ;
	l.unlock ();
      }
    } ਨਹੀਂਤਾਂ {
      ਵਸਤੂ = ਚੀਜ਼ਾਂ.push (v);
      l.unlock ();
    }
    ਵਾਪਸ ਚੀਜ਼ਾਂ.ਵਿੱਚ_error () ? ਖਾਲੀ_ਸੂਚਕ : ਵਸਤੂ;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਖਾਲੀ remove (T v, ਤਾਲਾ_t &l)
  {
    l.lock ();
    ਵਸਤੂ_t *ਵਸਤੂ = ਚੀਜ਼ਾਂ.lsearch (v);
    ਜੇ (ਵਸਤੂ)
    {
      ਵਸਤੂ_t old = *ਵਸਤੂ;
      *ਵਸਤੂ = std::move (ਚੀਜ਼ਾਂ.ਪੂਛ ());
      ਚੀਜ਼ਾਂ.ਕੱਢੋ ();
      l.unlock ();
      old.fini ();
    } ਨਹੀਂਤਾਂ {
      l.unlock ();
    }
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਤਾਰਕਿਕ find (T v, ਵਸਤੂ_t *i, ਤਾਲਾ_t &l)
  {
    l.lock ();
    ਵਸਤੂ_t *ਵਸਤੂ = ਚੀਜ਼ਾਂ.lsearch (v);
    ਜੇ (ਵਸਤੂ)
      *i = *ਵਸਤੂ;
    l.unlock ();
    ਵਾਪਸ !!ਵਸਤੂ;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਵਸਤੂ_t *ਲੱਭੋ_or_ਪਾਓ (T v, ਤਾਲਾ_t &l)
  {
    l.lock ();
    ਵਸਤੂ_t *ਵਸਤੂ = ਚੀਜ਼ਾਂ.find (v);
    ਜੇ (!ਵਸਤੂ) {
      ਵਸਤੂ = ਚੀਜ਼ਾਂ.push (v);
    }
    l.unlock ();
    ਵਾਪਸ ਵਸਤੂ;
  }

  ਖਾਲੀ fini (ਤਾਲਾ_t &l)
  {
    ਜੇ (!ਚੀਜ਼ਾਂ.ਲੰਮਾਈ)
    {
      /* No need to lock. */
      ਚੀਜ਼ਾਂ.fini ();
      ਵਾਪਸ;
    }
    l.lock ();
    ਜਦੋਂ (ਚੀਜ਼ਾਂ.ਲੰਮਾਈ)
    {
      ਵਸਤੂ_t old = ਚੀਜ਼ਾਂ.ਪੂਛ ();
      ਚੀਜ਼ਾਂ.ਕੱਢੋ ();
      l.unlock ();
      old.fini ();
      l.lock ();
    }
    ਚੀਜ਼ਾਂ.fini ();
    l.unlock ();
  }

};


/*
 * Reference-count.
 */

ਬਣਤਰ hb_reference_ਗਿਣਤੀ_t
{
  ਬਦਲਣਯੋਗ hb_atomic_t<ਪੂਰਨ> ref_ਗਿਣਤੀ_2;

  ਖਾਲੀ init (ਪੂਰਨ v = 1) { ref_ਗਿਣਤੀ_2 = v; }
  ਪੂਰਨ get_relaxed () ਅਚਲ { ਵਾਪਸ ref_ਗਿਣਤੀ_2; }
  ਪੂਰਨ inc () ਅਚਲ { ਵਾਪਸ ref_ਗਿਣਤੀ_2.inc (); }
  ਪੂਰਨ dec () ਅਚਲ { ਵਾਪਸ ref_ਗਿਣਤੀ_2.dec (); }
  ਖਾਲੀ fini () { ref_ਗਿਣਤੀ_2 = -0x0000DEAD; }

  ਤਾਰਕਿਕ is_inert () ਅਚਲ { ਵਾਪਸ !ref_ਗਿਣਤੀ_2; }
  ਤਾਰਕਿਕ is_valid () ਅਚਲ { ਵਾਪਸ ref_ਗਿਣਤੀ_2 > 0; }
};


/* user_data */

ਬਣਤਰ hb_user_ਅੰਕੜੇ_array_t
{
  ਬਣਤਰ hb_user_ਅੰਕੜੇ_ਵਸਤੂ_t {
    hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *key;
    ਖਾਲੀ *ਅੰਕੜੇ;
    hb_destroy_func_t destroy;

    ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *other_ਕੁੰਜੀ) ਅਚਲ { ਵਾਪਸ key == other_ਕੁੰਜੀ; }
    ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ hb_user_ਅੰਕੜੇ_ਵਸਤੂ_t &other) ਅਚਲ { ਵਾਪਸ key == other.key; }

    ਖਾਲੀ fini () { ਜੇ (destroy) destroy (ਅੰਕੜੇ); }
  };

  hb_mutex_t lock;
  hb_lockable_ਸੈੱਟ_t<hb_user_ਅੰਕੜੇ_ਵਸਤੂ_t, hb_mutex_t> ਚੀਜ਼ਾਂ;

  ਖਾਲੀ init () { lock.init (); ਚੀਜ਼ਾਂ.init (); }

  ਖਾਲੀ fini () { ਚੀਜ਼ਾਂ.fini (lock); lock.fini (); }

  ਤਾਰਕਿਕ ਸੈੱਟ (hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *key,
	    ਖਾਲੀ *              ਅੰਕੜੇ,
	    hb_destroy_func_t   destroy,
	    hb_bool_t           replace)
  {
    ਜੇ (!key)
      ਵਾਪਸ ਝੂਠ;

    ਜੇ (replace) {
      ਜੇ (!ਅੰਕੜੇ && !destroy) {
	ਚੀਜ਼ਾਂ.remove (key, lock);
	ਵਾਪਸ ਸੱਚ;
      }
    }
    hb_user_ਅੰਕੜੇ_ਵਸਤੂ_t ਵਸਤੂ = {key, ਅੰਕੜੇ, destroy};
    ਤਾਰਕਿਕ ret = !!ਚੀਜ਼ਾਂ.replace_or_ਪਾਓ (ਵਸਤੂ, lock, (ਤਾਰਕਿਕ) replace);

    ਵਾਪਸ ret;
  }

  ਖਾਲੀ *get (hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *key)
  {
    hb_user_ਅੰਕੜੇ_ਵਸਤੂ_t ਵਸਤੂ = {ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ};

    ਵਾਪਸ ਚੀਜ਼ਾਂ.find (key, &ਵਸਤੂ, lock) ? ਵਸਤੂ.ਅੰਕੜੇ : ਖਾਲੀ_ਸੂਚਕ;
  }
};


/*
 * Object header
 */

ਬਣਤਰ hb_object_ਸਿਰਲੇਖ_t
{
  hb_reference_ਗਿਣਤੀ_t ref_ਗਿਣਤੀ_2;
  ਬਦਲਣਯੋਗ hb_atomic_t<ਤਾਰਕਿਕ> writable = ਝੂਠ;
  hb_atomic_t<hb_user_ਅੰਕੜੇ_array_t *> user_ਅੰਕੜੇ;

  ਤਾਰਕਿਕ is_inert () ਅਚਲ { ਵਾਪਸ !ref_ਗਿਣਤੀ_2.get_relaxed (); }
};
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OBJECT_ਸਿਰਲੇਖ_STATIC {}


/*
 * Object
 */

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ hb_object_trace (ਅਚਲ Type *obj, ਅਚਲ ਅੱਖਰ *function)
{
  DEBUG_ਸੁਨੇਹਾ (OBJECT, (ਖਾਲੀ *) obj,
	     "%s refcount=%d",
	     function,
	     obj ? obj->ਸਿਰਲੇਖ.ref_ਗਿਣਤੀ_2.get_relaxed () : 0);
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type, ਕਿਸਮ_ਨਾਂ ...Ts>
ਸਥਿਰ ਸਤਰਅੰਦਰ Type *hb_object_create (Ts... ds)
{
  Type *obj = (Type *) hb_calloc (1, ਆਕਾਰ (Type));

  ਜੇ (unlikely (!obj))
    ਵਾਪਸ obj;

  ਨਵਾਂ (obj) Type (std::forward<Ts> (ds)...);

  hb_object_init (obj);
  hb_object_trace (obj, HB_FUNC);

  ਵਾਪਸ obj;
}
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ hb_object_init (Type *obj)
{
  obj->ਸਿਰਲੇਖ.ref_ਗਿਣਤੀ_2.init ();
  obj->ਸਿਰਲੇਖ.writable = ਸੱਚ;
  obj->ਸਿਰਲੇਖ.user_ਅੰਕੜੇ.init ();
}
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ hb_object_is_valid (ਅਚਲ Type *obj)
{
  ਵਾਪਸ likely (obj->ਸਿਰਲੇਖ.ref_ਗਿਣਤੀ_2.is_valid ());
}
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ hb_object_is_immutable (ਅਚਲ Type *obj)
{
  ਵਾਪਸ !obj->ਸਿਰਲੇਖ.writable;
}
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ hb_object_make_immutable (ਅਚਲ Type *obj)
{
  obj->ਸਿਰਲੇਖ.writable = ਝੂਠ;
}
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ hb_object_fini (Type *obj);
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
ਸਥਿਰ ਸਤਰਅੰਦਰ Type *hb_object_reference (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  ਜੇ (unlikely (!obj || obj->ਸਿਰਲੇਖ.is_inert ()))
    ਵਾਪਸ obj;
  ਪ੍ਰਤਿਜ੍ਞਾ (hb_object_is_valid (obj));
  obj->ਸਿਰਲੇਖ.ref_ਗਿਣਤੀ_2.inc ();
  ਵਾਪਸ obj;
}
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ hb_object_should_destroy (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  ਜੇ (unlikely (!obj || obj->ਸਿਰਲੇਖ.is_inert ()))
    ਵਾਪਸ ਝੂਠ;
  ਪ੍ਰਤਿਜ੍ਞਾ (hb_object_is_valid (obj));
  ਜੇ (obj->ਸਿਰਲੇਖ.ref_ਗਿਣਤੀ_2.dec () != 1)
    ਵਾਪਸ ਝੂਠ;
  ਵਾਪਸ ਸੱਚ;
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ hb_object_actually_destroy (Type *obj)
{
  hb_object_fini (obj);

  ਜੇ (!std::is_trivially_destructible<Type>::value)
    obj->~Type ();
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ hb_object_destroy (Type *obj)
{
  ਜੇ (!hb_object_should_destroy (obj))
    ਵਾਪਸ ਝੂਠ;

  hb_object_actually_destroy (obj);
  ਵਾਪਸ ਸੱਚ;
}
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ hb_object_fini (Type *obj)
{
  obj->ਸਿਰਲੇਖ.ref_ਗਿਣਤੀ_2.fini (); /* Do this before user_data */
  hb_user_ਅੰਕੜੇ_array_t *user_ਅੰਕੜੇ = obj->ਸਿਰਲੇਖ.user_ਅੰਕੜੇ.get_acquire ();
  ਜੇ (user_ਅੰਕੜੇ)
  {
    user_ਅੰਕੜੇ->fini ();
    hb_free (user_ਅੰਕੜੇ);
    obj->ਸਿਰਲੇਖ.user_ਅੰਕੜੇ.ਸੈੱਟ_relaxed (ਖਾਲੀ_ਸੂਚਕ);
  }
}
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ hb_object_ਸੈੱਟ_user_ਅੰਕੜੇ (Type               *obj,
					    hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *key,
					    ਖਾਲੀ *              ਅੰਕੜੇ,
					    hb_destroy_func_t   destroy,
					    hb_bool_t           replace)
{
  ਜੇ (unlikely (!obj || obj->ਸਿਰਲੇਖ.is_inert ()))
    ਵਾਪਸ ਝੂਠ;
  ਪ੍ਰਤਿਜ੍ਞਾ (hb_object_is_valid (obj));

retry:
  hb_user_ਅੰਕੜੇ_array_t *user_ਅੰਕੜੇ = obj->ਸਿਰਲੇਖ.user_ਅੰਕੜੇ.get_acquire ();
  ਜੇ (unlikely (!user_ਅੰਕੜੇ))
  {
    user_ਅੰਕੜੇ = (hb_user_ਅੰਕੜੇ_array_t *) hb_calloc (1, ਆਕਾਰ (hb_user_ਅੰਕੜੇ_array_t));
    ਜੇ (unlikely (!user_ਅੰਕੜੇ))
      ਵਾਪਸ ਝੂਠ;
    user_ਅੰਕੜੇ->init ();
    ਜੇ (unlikely (!obj->ਸਿਰਲੇਖ.user_ਅੰਕੜੇ.cmpexch (ਖਾਲੀ_ਸੂਚਕ, user_ਅੰਕੜੇ)))
    {
      user_ਅੰਕੜੇ->fini ();
      hb_free (user_ਅੰਕੜੇ);
      ਜਾਓ retry;
    }
  }

  ਵਾਪਸ user_ਅੰਕੜੇ->ਸੈੱਟ (key, ਅੰਕੜੇ, destroy, replace);
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ *hb_object_get_user_ਅੰਕੜੇ (Type               *obj,
					     hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *key)
{
  ਜੇ (unlikely (!obj || obj->ਸਿਰਲੇਖ.is_inert ()))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  ਪ੍ਰਤਿਜ੍ਞਾ (hb_object_is_valid (obj));
  hb_user_ਅੰਕੜੇ_array_t *user_ਅੰਕੜੇ = obj->ਸਿਰਲੇਖ.user_ਅੰਕੜੇ.get_acquire ();
  ਜੇ (!user_ਅੰਕੜੇ)
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  ਵਾਪਸ user_ਅੰਕੜੇ->get (key);
}


#ਜੇ_ਅੰਤ /* HB_OBJECT_HH */

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

#अगर_अघोषित HB_OBJECT_HH
#घोषणा HB_OBJECT_HH

#समावेश "hb.hh"
#समावेश "hb-atomic.hh"
#समावेश "hb-mutex.hh"
#समावेश "hb-vector.hh"


/*
 * Lockable set
 */

ढाँचा <typename item_t, typename lock_t>
काष्ठा hb_lockable_set_t
{
  hb_vector_t<item_t> वस्तुएँ;

  व्योम init () { वस्तुएँ.init (); }

  ढाँचा <typename T>
  item_t *replace_or_जोड़ो (T v, lock_t &l, bool replace)
  {
    l.lock ();
    item_t *item = वस्तुएँ.lsearch (v);
    अगर (item) {
      अगर (replace) {
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
      item = वस्तुएँ.push (v);
      l.unlock ();
    }
    वापस वस्तुएँ.in_error () ? nullptr : item;
  }

  ढाँचा <typename T>
  व्योम हटाओ (T v, lock_t &l)
  {
    l.lock ();
    item_t *item = वस्तुएँ.lsearch (v);
    अगर (item)
    {
      item_t old = *item;
      *item = std::move (वस्तुएँ.पुच्छ ());
      वस्तुएँ.निकालो ();
      l.unlock ();
      old.fini ();
    } अन्यथा {
      l.unlock ();
    }
  }

  ढाँचा <typename T>
  bool find (T v, item_t *i, lock_t &l)
  {
    l.lock ();
    item_t *item = वस्तुएँ.lsearch (v);
    अगर (item)
      *i = *item;
    l.unlock ();
    वापस !!item;
  }

  ढाँचा <typename T>
  item_t *find_or_जोड़ो (T v, lock_t &l)
  {
    l.lock ();
    item_t *item = वस्तुएँ.find (v);
    अगर (!item) {
      item = वस्तुएँ.push (v);
    }
    l.unlock ();
    वापस item;
  }

  व्योम fini (lock_t &l)
  {
    अगर (!वस्तुएँ.लंबाई)
    {
      /* No need to lock. */
      वस्तुएँ.fini ();
      वापस;
    }
    l.lock ();
    जबतक (वस्तुएँ.लंबाई)
    {
      item_t old = वस्तुएँ.पुच्छ ();
      वस्तुएँ.निकालो ();
      l.unlock ();
      old.fini ();
      l.lock ();
    }
    वस्तुएँ.fini ();
    l.unlock ();
  }

};


/*
 * Reference-count.
 */

काष्ठा hb_reference_गिनती_t
{
  mutable hb_atomic_t<पूर्णांक> ref_गिनती_2;

  व्योम init (पूर्णांक v = 1) { ref_गिनती_2 = v; }
  पूर्णांक get_relaxed () स्थिर { वापस ref_गिनती_2; }
  पूर्णांक inc () स्थिर { वापस ref_गिनती_2.inc (); }
  पूर्णांक डेसी () स्थिर { वापस ref_गिनती_2.डेसी (); }
  व्योम fini () { ref_गिनती_2 = -0x0000DEAD; }

  bool is_inert () स्थिर { वापस !ref_गिनती_2; }
  bool is_valid () स्थिर { वापस ref_गिनती_2 > 0; }
};


/* user_data */

काष्ठा hb_user_तथ्य_array_t
{
  काष्ठा hb_user_तथ्य_item_t {
    hb_user_तथ्य_कुंजी_t *key;
    व्योम *तथ्य;
    hb_destroy_func_t destroy;

    bool चालक == (स्थिर hb_user_तथ्य_कुंजी_t *other_कुंजी) स्थिर { वापस key == other_कुंजी; }
    bool चालक == (स्थिर hb_user_तथ्य_item_t &other) स्थिर { वापस key == other.key; }

    व्योम fini () { अगर (destroy) destroy (तथ्य); }
  };

  hb_mutex_t lock;
  hb_lockable_set_t<hb_user_तथ्य_item_t, hb_mutex_t> वस्तुएँ;

  व्योम init () { lock.init (); वस्तुएँ.init (); }

  व्योम fini () { वस्तुएँ.fini (lock); lock.fini (); }

  bool set (hb_user_तथ्य_कुंजी_t *key,
	    व्योम *              तथ्य,
	    hb_destroy_func_t   destroy,
	    hb_bool_t           replace)
  {
    अगर (!key)
      वापस false;

    अगर (replace) {
      अगर (!तथ्य && !destroy) {
	वस्तुएँ.हटाओ (key, lock);
	वापस true;
      }
    }
    hb_user_तथ्य_item_t item = {key, तथ्य, destroy};
    bool ret = !!वस्तुएँ.replace_or_जोड़ो (item, lock, (bool) replace);

    वापस ret;
  }

  व्योम *लो (hb_user_तथ्य_कुंजी_t *key)
  {
    hb_user_तथ्य_item_t item = {nullptr, nullptr, nullptr};

    वापस वस्तुएँ.find (key, &item, lock) ? item.तथ्य : nullptr;
  }
};


/*
 * Object header
 */

काष्ठा hb_object_header_t
{
  hb_reference_गिनती_t ref_गिनती_2;
  mutable hb_atomic_t<bool> writable = false;
  hb_atomic_t<hb_user_तथ्य_array_t *> user_तथ्य;

  bool is_inert () स्थिर { वापस !ref_गिनती_2.get_relaxed (); }
};
#घोषणा HB_OBJECT_HEADER_STATIC {}


/*
 * Object
 */

ढाँचा <typename Type>
अटल अंतरभूत व्योम hb_object_trace (स्थिर Type *obj, स्थिर अक्षर *function)
{
  DEBUG_संदेश (OBJECT, (व्योम *) obj,
	     "%s refcount=%d",
	     function,
	     obj ? obj->header.ref_गिनती_2.get_relaxed () : 0);
}

ढाँचा <typename Type, typename ...Ts>
अटल अंतरभूत Type *hb_object_create (Ts... ds)
{
  Type *obj = (Type *) hb_calloc (1, माप (Type));

  अगर (unlikely (!obj))
    वापस obj;

  नया (obj) Type (std::forward<Ts> (ds)...);

  hb_object_init (obj);
  hb_object_trace (obj, HB_FUNC);

  वापस obj;
}
ढाँचा <typename Type>
अटल अंतरभूत व्योम hb_object_init (Type *obj)
{
  obj->header.ref_गिनती_2.init ();
  obj->header.writable = true;
  obj->header.user_तथ्य.init ();
}
ढाँचा <typename Type>
अटल अंतरभूत bool hb_object_is_valid (स्थिर Type *obj)
{
  वापस likely (obj->header.ref_गिनती_2.is_valid ());
}
ढाँचा <typename Type>
अटल अंतरभूत bool hb_object_is_immutable (स्थिर Type *obj)
{
  वापस !obj->header.writable;
}
ढाँचा <typename Type>
अटल अंतरभूत व्योम hb_object_make_immutable (स्थिर Type *obj)
{
  obj->header.writable = false;
}
ढाँचा <typename Type>
अटल अंतरभूत व्योम hb_object_fini (Type *obj);
ढाँचा <typename Type>
अटल अंतरभूत Type *hb_object_reference (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  अगर (unlikely (!obj || obj->header.is_inert ()))
    वापस obj;
  निश्चित (hb_object_is_valid (obj));
  obj->header.ref_गिनती_2.inc ();
  वापस obj;
}
ढाँचा <typename Type>
अटल अंतरभूत bool hb_object_should_destroy (Type *obj)
{
  hb_object_trace (obj, HB_FUNC);
  अगर (unlikely (!obj || obj->header.is_inert ()))
    वापस false;
  निश्चित (hb_object_is_valid (obj));
  अगर (obj->header.ref_गिनती_2.डेसी () != 1)
    वापस false;
  वापस true;
}

ढाँचा <typename Type>
अटल अंतरभूत व्योम hb_object_actually_destroy (Type *obj)
{
  hb_object_fini (obj);

  अगर (!std::is_trivially_destructible<Type>::value)
    obj->~Type ();
}

ढाँचा <typename Type>
अटल अंतरभूत bool hb_object_destroy (Type *obj)
{
  अगर (!hb_object_should_destroy (obj))
    वापस false;

  hb_object_actually_destroy (obj);
  वापस true;
}
ढाँचा <typename Type>
अटल अंतरभूत व्योम hb_object_fini (Type *obj)
{
  obj->header.ref_गिनती_2.fini (); /* Do this before user_data */
  hb_user_तथ्य_array_t *user_तथ्य = obj->header.user_तथ्य.get_acquire ();
  अगर (user_तथ्य)
  {
    user_तथ्य->fini ();
    hb_free (user_तथ्य);
    obj->header.user_तथ्य.set_relaxed (nullptr);
  }
}
ढाँचा <typename Type>
अटल अंतरभूत bool hb_object_set_user_तथ्य (Type               *obj,
					    hb_user_तथ्य_कुंजी_t *key,
					    व्योम *              तथ्य,
					    hb_destroy_func_t   destroy,
					    hb_bool_t           replace)
{
  अगर (unlikely (!obj || obj->header.is_inert ()))
    वापस false;
  निश्चित (hb_object_is_valid (obj));

retry:
  hb_user_तथ्य_array_t *user_तथ्य = obj->header.user_तथ्य.get_acquire ();
  अगर (unlikely (!user_तथ्य))
  {
    user_तथ्य = (hb_user_तथ्य_array_t *) hb_calloc (1, माप (hb_user_तथ्य_array_t));
    अगर (unlikely (!user_तथ्य))
      वापस false;
    user_तथ्य->init ();
    अगर (unlikely (!obj->header.user_तथ्य.cmpexch (nullptr, user_तथ्य)))
    {
      user_तथ्य->fini ();
      hb_free (user_तथ्य);
      जाओ retry;
    }
  }

  वापस user_तथ्य->set (key, तथ्य, destroy, replace);
}

ढाँचा <typename Type>
अटल अंतरभूत व्योम *hb_object_get_user_तथ्य (Type               *obj,
					     hb_user_तथ्य_कुंजी_t *key)
{
  अगर (unlikely (!obj || obj->header.is_inert ()))
    वापस nullptr;
  निश्चित (hb_object_is_valid (obj));
  hb_user_तथ्य_array_t *user_तथ्य = obj->header.user_तथ्य.get_acquire ();
  अगर (!user_तथ्य)
    वापस nullptr;
  वापस user_तथ्य->लो (key);
}


#पूर्ण_अगर /* HB_OBJECT_HH */

/*
 * Copyright © 2017,2018  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#अगर_अघोषित HB_VECTOR_HH
#घोषणा HB_VECTOR_HH

#समावेश "hb.hh"
#समावेश "hb-array.hh"
#समावेश "hb-meta.hh"
#समावेश "hb-null.hh"

// Change to 1 to force inline vector allocs, to see callsite in malloc-stats tool.
#अगर 0
#घोषणा HB_ALWAYS_INLINE_VECTOR_ALLOCS HB_ALWAYS_INLINE
#अन्यथा
#घोषणा HB_ALWAYS_INLINE_VECTOR_ALLOCS
#पूर्ण_अगर

ढाँचा <typename Type,
	  bool sorted=false>
काष्ठा hb_vector_t
{
  अटल constexpr bool realloc_move = true;

  प्रकार Type item_t;
  अटल constexpr अचिन्हित item_आकार = hb_static_आकार (Type);
  using array_t = typename std::conditional<sorted, hb_sorted_array_t<Type>, hb_array_t<Type>>::type;
  using c_array_t = typename std::conditional<sorted, hb_sorted_array_t<स्थिर Type>, hb_array_t<स्थिर Type>>::type;

  hb_vector_t () = शेष;
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (std::initializer_list<Type> lst) : hb_vector_t ()
  {
    alloc (lst.आकार (), true);
    क्रम (स्वतः&& item : lst)
      push (item);
  }
  ढाँचा <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  explicit hb_vector_t (स्थिर Iterable &o) : hb_vector_t ()
  {
    extend (o, true);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (स्थिर hb_vector_t &o) : hb_vector_t ()
  {
    alloc_exact (o.लंबाई);
    अगर (unlikely (in_error ())) वापस;
    copy_array (o.as_array ());
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (array_t o) : hb_vector_t ()
  {
    alloc_exact (o.लंबाई);
    अगर (unlikely (in_error ())) वापस;
    copy_array (o);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (c_array_t o) : hb_vector_t ()
  {
    alloc_exact (o.लंबाई);
    अगर (unlikely (in_error ())) वापस;
    copy_array (o);
  }
  hb_vector_t (hb_vector_t &&o) noexcept
  {
    allocated = o.allocated;
    लंबाई = o.लंबाई;
    arrayZ = o.arrayZ;
    o.init ();
  }
  ~hb_vector_t () { fini (); }

  ढाँचा <अचिन्हित n>
  व्योम
  set_storage (Type (&array)[n])
  { set_storage (array, n); }
  व्योम
  set_storage (hb_array_t<Type> array)
  { set_storage (array.arrayZ, array.लंबाई); }
  ढाँचा <typename T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T) &&
			  hb_is_trivially_destructible(T))>
  व्योम
  set_storage (Type *array, अचिन्हित n)
  {
    निश्चित (allocated == 0);
    निश्चित (लंबाई == 0);

    arrayZ = array;
    लंबाई = n;
  }

  ढाँचा <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  व्योम extend (स्थिर Iterable &o, bool exact=false)
  {
    स्वतः iter = hb_iter (o);
    अगर (iter.is_random_access_iterator || iter.has_fast_len)
    {
      अगर (unlikely (!alloc (लंबाई + hb_len (iter), exact)))
	वापस;
      अचिन्हित गिनती = hb_len (iter);
      क्रम (अचिन्हित i = 0; i < गिनती; i++)
	धकेलो_has_room (*iter++);
    }
    जबतक (iter)
    {
      अगर (unlikely (!alloc (लंबाई + 1)))
        वापस;
      अचिन्हित room = allocated - लंबाई;
      क्रम (अचिन्हित i = 0; i < room && iter; i++)
	धकेलो_has_room (*iter++);
    }
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  व्योम extend (array_t o, bool exact=false)
  { extend (c_array_t (o), exact); }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  व्योम extend (c_array_t o, bool exact=false)
  {
    /* o may point into our own storage, eg. v.extend (v.as_array ()).
     * alloc() may move that storage, so find o again afterwards. */
    bool aliased = o.लंबाई && in_storage (*o.arrayZ);
    अचिन्हित offset = aliased ? o.arrayZ - arrayZ : 0;
    alloc (लंबाई + o.लंबाई, exact);
    अगर (unlikely (in_error ())) वापस;
    अगर (unlikely (aliased))
      o = {arrayZ + offset, o.लंबाई};
    copy_array (o);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  व्योम extend (स्थिर hb_vector_t &o, bool exact=false)
  { extend (o.as_array (), exact); }

  खुला:
  पूर्णांक allocated = 0; /* < 0 means allocation failed. */
  अचिन्हित पूर्णांक लंबाई = 0;
  खुला:
  Type *arrayZ = nullptr;

  व्योम init ()
  {
    allocated = लंबाई = 0;
    arrayZ = nullptr;
  }
  व्योम init0 ()
  {
  }

  व्योम fini ()
  {
    अगर (is_owned ())
    {
      shrink_vector (0);
      hb_free (arrayZ);
    }
    init ();
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t &reset ()
  {
    अगर (unlikely (in_error ()))
      reset_error ();
    सफा ();
    वापस *यह;
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t &reset_if_error ()
  {
    अगर (unlikely (in_error ()))
      reset ();
    वापस *यह;
  }

  /* Transfer ownership of the backing storage to caller.
   * Returns nullptr if storage is not owned by this vector. */
  Type *
  steal (अचिन्हित *len = nullptr, पूर्णांक *allocated_out = nullptr)
  {
    अगर (!is_owned ())
      वापस nullptr;
    अगर (len)
      *len = लंबाई;
    अगर (allocated_out)
      *allocated_out = allocated;
    Type *p = arrayZ;
    init ();
    वापस p;
  }

  /* Adopt a previously detached owned buffer. */
  व्योम
  recycle_बफर (Type *बफर,
                  अचिन्हित len,
                  पूर्णांक allocated_len)
  {
    fini ();
    arrayZ = बफर;
    लंबाई = len;
    allocated = allocated_len;
  }

  मित्र व्योम swap (hb_vector_t& a, hb_vector_t& b) noexcept
  {
    hb_swap (a.allocated, b.allocated);
    hb_swap (a.लंबाई, b.लंबाई);
    hb_swap (a.arrayZ, b.arrayZ);
  }

  hb_vector_t& चालक = (स्थिर hb_vector_t &o)
  {
    अगर (यह == &o) वापस *यह;

    reset ();
    alloc_exact (o.लंबाई);
    अगर (unlikely (in_error ())) वापस *यह;

    लंबाई = 0;
    copy_array (o.as_array ());

    वापस *यह;
  }
  hb_vector_t& चालक = (hb_vector_t &&o) noexcept
  {
    hb_swap (*यह, o);
    वापस *यह;
  }

  hb_bytes_t as_bytes () स्थिर
  { वापस hb_bytes_t ((स्थिर अक्षर *) arrayZ, get_आकार ()); }

  bool चालक == (स्थिर hb_vector_t &o) स्थिर { वापस as_array () == o.as_array (); }
  bool चालक != (स्थिर hb_vector_t &o) स्थिर { वापस !(*यह == o); }
  uint32_t hash () स्थिर { वापस as_array ().hash (); }

  Type& चालक [] (पूर्णांक i_)
  {
    अचिन्हित पूर्णांक i = (अचिन्हित पूर्णांक) i_;
    अगर (unlikely (i >= लंबाई))
      वापस Crap (Type);
    वापस arrayZ[i];
  }
  स्थिर Type& चालक [] (पूर्णांक i_) स्थिर
  {
    अचिन्हित पूर्णांक i = (अचिन्हित पूर्णांक) i_;
    अगर (unlikely (i >= लंबाई))
      वापस Null (Type);
    वापस arrayZ[i];
  }

  Type& पुच्छ () { वापस (*यह)[लंबाई - 1]; }
  स्थिर Type& पुच्छ () स्थिर { वापस (*यह)[लंबाई - 1]; }

  explicit चालक bool () स्थिर { वापस लंबाई; }
  माप_प्रकार get_आकार () स्थिर { वापस hb_unsigned_mul_saturate (लंबाई, item_आकार); }

  /* Sink interface. */
  ढाँचा <typename T>
  hb_vector_t& चालक << (T&& v) { push (std::forward<T> (v)); वापस *यह; }

  array_t   as_array ()       { वापस hb_array (arrayZ, लंबाई); }
  c_array_t as_array () स्थिर { वापस hb_array (arrayZ, लंबाई); }

  /* Iterator. */
  प्रकार c_array_t   iter_t;
  प्रकार array_t   writer_t;
    iter_t   iter () स्थिर { वापस as_array (); }
  writer_t writer ()       { वापस as_array (); }
  चालक   iter_t () स्थिर { वापस   iter (); }
  चालक writer_t ()       { वापस writer (); }

  /* Faster range-based for loop. */
  Type *begin () स्थिर { वापस arrayZ; }
  Type *end () स्थिर { वापस arrayZ + लंबाई; }


  hb_sorted_array_t<Type> as_sorted_array ()
  { वापस hb_sorted_array (arrayZ, लंबाई); }
  hb_sorted_array_t<स्थिर Type> as_sorted_array () स्थिर
  { वापस hb_sorted_array (arrayZ, लंबाई); }

  ढाँचा <typename T> explicit चालक T * () { वापस arrayZ; }
  ढाँचा <typename T> explicit चालक स्थिर T * () स्थिर { वापस arrayZ; }

  Type * चालक  + (अचिन्हित पूर्णांक i) { वापस arrayZ + i; }
  स्थिर Type * चालक  + (अचिन्हित पूर्णांक i) स्थिर { वापस arrayZ + i; }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push ()
  {
    अगर (unlikely (!resize (लंबाई + 1)))
      वापस std::addressof (Crap (Type));
    वापस std::addressof (arrayZ[लंबाई - 1]);
  }
  ढाँचा <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push (Args&&... args)
  {
    अगर (unlikely ((पूर्णांक) लंबाई >= allocated))
    {
      अगर (unlikely (in_error ()))
	वापस std::addressof (Crap (Type));

      /* An argument may live in our own storage, eg. v.push (v[i]).
       * alloc() frees that storage, so copy it out first. */
      अगर (unlikely (in_storage (args...)))
      {
	Type copy (std::forward<Args> (args)...);
	अगर (unlikely (!alloc (लंबाई + 1)))
	  वापस std::addressof (Crap (Type));
	वापस धकेलो_has_room (std::move (copy));
      }
      अगर (unlikely (!alloc (लंबाई + 1)))
	// If push failed to allocate then don't copy v, since this may cause
	// the created copy to leak memory since we won't have stored a
	// reference to it.
	वापस std::addressof (Crap (Type));
    }

    वापस धकेलो_has_room (std::forward<Args> (args)...);
  }
  ढाँचा <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  bool धकेलो_or_fail (Args&&... args)
  {
    वापस push (std::forward<Args> (args)...) != std::addressof (Crap (Type));
  }
  ढाँचा <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *धकेलो_has_room (Args&&... args)
  {
    /* Emplace. */
    Type *p = std::addressof (arrayZ[लंबाई++]);
    वापस नया (p) Type (std::forward<Args> (args)...);
  }

  bool in_storage () स्थिर { वापस false; }
  ढाँचा <typename T, typename... Ts>
  bool in_storage (स्थिर T &v, स्थिर Ts &...vs) स्थिर
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    वापस (arrayZ &&
	    (uintptr_t) arrayZ <= p && p < (uintptr_t) (arrayZ + लंबाई)) ||
	   in_storage (vs...);
  }

  bool is_owned () स्थिर
  {
    वापस allocated != 0 && allocated != -1;
  }

  bool in_error () स्थिर { वापस allocated < 0; }
  व्योम set_error ()
  {
    निश्चित (allocated >= 0);
    allocated = -allocated - 1;
  }
  व्योम reset_error ()
  {
    निश्चित (allocated < 0);
    allocated = -(allocated + 1);
  }
  व्योम ensure_error ()
  {
    अगर (!in_error ())
      set_error ();
  }

  Type *
  _realloc (अचिन्हित new_allocated)
  {
    अगर (!new_allocated)
    {
      अगर (is_owned ())
	hb_free (arrayZ);
      वापस nullptr;
    }
    अगर (!allocated && arrayZ)
    {
      /* If we have a non-null arrayZ but allocated is 0, then we are
       * reallocating from a foreign array. */
      Type *new_array = (Type *) hb_malloc (new_allocated * माप (Type));
      अगर (unlikely (!new_array))
	वापस nullptr;
      hb_memcpy ((व्योम *) new_array, (स्थिर व्योम *) arrayZ, लंबाई * माप (Type));
      वापस new_array;
    }
    वापस (Type *) hb_realloc (arrayZ, new_allocated * माप (Type));
  }
  Type *
  _malloc_move (अचिन्हित new_allocated)
  {
    अगर (!new_allocated)
    {
      अगर (is_owned ())
	hb_free (arrayZ);
      वापस nullptr;
    }
    Type *new_array = (Type *) hb_malloc (new_allocated * माप (Type));
    अगर (likely (new_array))
    {
      क्रम (अचिन्हित i = 0; i < लंबाई; i++)
      {
	नया (std::addressof (new_array[i])) Type ();
	new_array[i] = std::move (arrayZ[i]);
	arrayZ[i].~Type ();
      }
      अगर (is_owned ())
	hb_free (arrayZ);
    }
    वापस new_array;
  }

  ढाँचा <typename T = Type,
	    hb_enable_if (hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_vector (अचिन्हित new_allocated, hb_priority<0>)
  {
    वापस _realloc (new_allocated);
  }
  ढाँचा <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_vector (अचिन्हित new_allocated, hb_priority<0>)
  {
    वापस _malloc_move (new_allocated);
  }
  /* Specialization for types that can be moved using realloc(). */
  ढाँचा <typename T = Type,
	    hb_enable_if (T::realloc_move)>
  Type *
  realloc_vector (अचिन्हित new_allocated, hb_priority<1>)
  {
    वापस _realloc (new_allocated);
  }

  ढाँचा <typename T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T))>
  व्योम
  grow_vector (अचिन्हित आकार, hb_priority<0>)
  {
    hb_memset (arrayZ + लंबाई, 0, (आकार - लंबाई) * माप (*arrayZ));
    लंबाई = आकार;
  }
  ढाँचा <typename T = Type,
	    hb_enable_if (!hb_is_trivially_constructible(T))>
  व्योम
  grow_vector (अचिन्हित आकार, hb_priority<0>)
  {
    क्रम (; लंबाई < आकार; लंबाई++)
      नया (std::addressof (arrayZ[लंबाई])) Type ();
  }
  /* Specialization for hb_vector_t<hb_{vector,array}_t<U>> to speed up. */
  ढाँचा <typename T = Type,
	    hb_enable_if (hb_is_same (T, hb_vector_t<typename T::item_t>) ||
			  hb_is_same (T, hb_array_t <typename T::item_t>))>
  व्योम
  grow_vector (अचिन्हित आकार, hb_priority<1>)
  {
    hb_memset (arrayZ + लंबाई, 0, (आकार - लंबाई) * माप (*arrayZ));
    लंबाई = आकार;
  }

  ढाँचा <typename T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  व्योम
  copy_array (hb_array_t<Type> other)
  {
    hb_memcpy ((व्योम *) (arrayZ + लंबाई), (स्थिर व्योम *) other.arrayZ, other.लंबाई * item_आकार);
    लंबाई += other.लंबाई;
  }
  ढाँचा <typename T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  व्योम
  copy_array (hb_array_t<स्थिर Type> other)
  {
    hb_memcpy ((व्योम *) (arrayZ + लंबाई), (स्थिर व्योम *) other.arrayZ, other.लंबाई * item_आकार);
    लंबाई += other.लंबाई;
  }
  ढाँचा <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			   std::is_copy_constructible<T>::value)>
  व्योम
  copy_array (hb_array_t<स्थिर Type> other)
  {
    क्रम (अचिन्हित i = 0; i < other.लंबाई; i++)
      नया (std::addressof (arrayZ[लंबाई + i])) Type (other.arrayZ[i]);
    लंबाई += other.लंबाई;
  }
  ढाँचा <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			  !std::is_copy_constructible<T>::value &&
			  std::is_default_constructible<T>::value &&
			  std::is_copy_assignable<T>::value)>
  व्योम
  copy_array (hb_array_t<स्थिर Type> other)
  {
    क्रम (अचिन्हित i = 0; i < other.लंबाई; i++)
    {
      नया (std::addressof (arrayZ[लंबाई + i])) Type ();
      arrayZ[लंबाई + i] = other.arrayZ[i];
    }
    लंबाई += other.लंबाई;
  }

  व्योम
  shrink_vector (अचिन्हित आकार)
  {
    निश्चित (आकार <= लंबाई);
    अगर (!hb_is_trivially_destructible(Type))
    {
      अचिन्हित गिनती = लंबाई - आकार;
      Type *p = arrayZ + लंबाई;
      जबतक (गिनती--)
        (--p)->~Type ();
    }
    लंबाई = आकार;
  }

  व्योम
  shift_down_vector (अचिन्हित i)
  {
    क्रम (; i < लंबाई; i++)
      arrayZ[i - 1] = std::move (arrayZ[i]);
  }

  /* Allocate for size but don't adjust length. */
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  bool alloc (अचिन्हित पूर्णांक आकार, bool exact=false)
  {
    अगर (unlikely (in_error ()))
      वापस false;

    अचिन्हित पूर्णांक new_allocated;
    अगर (exact)
    {
      /* If exact was specified, we allow shrinking the storage. */
      आकार = hb_max (आकार, लंबाई);
      अगर (आकार <= (अचिन्हित) allocated &&
	  आकार >= (अचिन्हित) allocated >> 2)
	वापस true;

      new_allocated = आकार;
    }
    अन्यथा
    {
      अगर (likely (आकार <= (अचिन्हित) allocated))
	वापस true;

      new_allocated = allocated;
      जबतक (आकार > new_allocated)
	new_allocated += (new_allocated >> 1) + 8;
    }

    /* Reallocate */

    bool overflows =
      (पूर्णांक) in_error () ||
      (new_allocated < आकार) ||
      hb_unsigned_mul_overflows (new_allocated, माप (Type));

    अगर (unlikely (overflows))
    {
      set_error ();
      वापस false;
    }

    Type *new_array = realloc_vector (new_allocated, hb_prioritize);

    अगर (unlikely (new_allocated && !new_array))
    {
      अगर (new_allocated <= (अचिन्हित) allocated)
        वापस true; // shrinking failed; it's okay; happens in our fuzzer

      set_error ();
      वापस false;
    }

    arrayZ = new_array;
    allocated = new_allocated;

    वापस true;
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  bool alloc_exact (अचिन्हित पूर्णांक आकार)
  {
    वापस alloc (आकार, true);
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  व्योम सफा ()
  {
    shrink_vector (0);
  }

  ढाँचा <typename allocator_t>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  bool allocate_from_pool (allocator_t *allocator, अचिन्हित आकार, अचिन्हित पूर्णांक initialize = true)
  {
    अगर (allocator)
    {
      निश्चित (!लंबाई && !allocated);
      arrayZ = (Type *) allocator->alloc (आकार * माप (Type), alignof (Type));
      अगर (unlikely (!arrayZ))
      {
	set_error ();
	वापस false;
      }
      अगर (initialize)
	grow_vector (आकार, hb_prioritize);
      अन्यथा
	लंबाई = आकार;
      वापस true;
    }
    वापस resize_full ((पूर्णांक) आकार, initialize, true);
  }

  ढाँचा <typename allocator_t>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  bool duplicate_vector_from_pool (allocator_t *allocator, स्थिर hb_vector_t &other)
  {
    अगर (unlikely (!allocate_from_pool (allocator, other.लंबाई, false)))
      वापस false;
    लंबाई = 0;
    copy_array (other.as_array ());
    वापस true;
  }

  ढाँचा <typename allocator_t>
  व्योम shrink_back_to_pool (allocator_t *allocator, पूर्णांक आकार)
  {
    अचिन्हित orig_लंबाई = लंबाई;

    shrink (आकार, false);

    अगर (allocator && !is_owned ())
      allocator->discard (arrayZ + लंबाई, (orig_लंबाई - लंबाई) * माप (Type));
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  bool resize_full (पूर्णांक आकार_3, bool initialize, bool exact)
  {
    अगर (unlikely (आकार_3 < 0))
      वापस false;
    अचिन्हित पूर्णांक आकार = (अचिन्हित पूर्णांक) आकार_3;
    अगर (!alloc (आकार, exact))
      वापस false;

    अगर (आकार > लंबाई)
    {
      अगर (initialize)
	grow_vector (आकार, hb_prioritize);
    }
    अन्यथा अगर (आकार < लंबाई)
    {
      अगर (initialize)
	shrink_vector (आकार);
    }

    लंबाई = आकार;
    वापस true;
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  bool resize (पूर्णांक आकार_3)
  {
    वापस resize_full (आकार_3, true, false);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  bool resize_dirty (पूर्णांक आकार_3)
  {
    वापस resize_full (आकार_3, false, false);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  bool resize_exact (पूर्णांक आकार_3)
  {
    वापस resize_full (आकार_3, true, true);
  }

  Type निकालो ()
  {
    अगर (!लंबाई) वापस Null (Type);
    Type v (std::move (arrayZ[लंबाई - 1]));
    arrayZ[लंबाई - 1].~Type ();
    लंबाई--;
    वापस v;
  }

  व्योम remove_ordered (अचिन्हित पूर्णांक i)
  {
    अगर (unlikely (i >= लंबाई))
      वापस;
    shift_down_vector (i + 1);
    arrayZ[लंबाई - 1].~Type ();
    लंबाई--;
  }

  ढाँचा <bool Sorted = sorted,
	    hb_enable_if (!Sorted)>
  व्योम remove_unordered (अचिन्हित पूर्णांक i)
  {
    अगर (unlikely (i >= लंबाई))
      वापस;
    अगर (i != लंबाई - 1)
      arrayZ[i] = std::move (arrayZ[लंबाई - 1]);
    arrayZ[लंबाई - 1].~Type ();
    लंबाई--;
  }

  व्योम shrink (पूर्णांक आकार_3, bool shrink_memory = true)
  {
    अचिन्हित पूर्णांक आकार = आकार_3 < 0 ? 0u : (अचिन्हित पूर्णांक) आकार_3;
    अगर (आकार >= लंबाई)
      वापस;

    shrink_vector (आकार);

    अगर (is_owned () && shrink_memory)
      alloc_exact (आकार); /* To force shrinking memory if needed. */
  }


  /* Sorting API. */
  ढाँचा <typename Compar>
  व्योम क्विक (Compar compar)
  { as_array ().क्विक (compar); }
  व्योम क्विक ()
  { as_array ().क्विक (); }

  /* Unsorted search API. */
  ढाँचा <typename T>
  Type *lsearch (स्थिर T &x, Type *not_found = nullptr)
  { वापस as_array ().lsearch (x, not_found); }
  ढाँचा <typename T>
  स्थिर Type *lsearch (स्थिर T &x, स्थिर Type *not_found = nullptr) स्थिर
  { वापस as_array ().lsearch (x, not_found); }
  ढाँचा <typename T>
  bool lfind (स्थिर T &x, अचिन्हित *pos = nullptr) स्थिर
  { वापस as_array ().lfind (x, pos); }

  /* Sorted search API. */
  ढाँचा <typename T,
	    bool Sorted=sorted, hb_enable_if (Sorted)>
  Type *द्वा_खोज (स्थिर T &x, Type *not_found = nullptr)
  { वापस as_array ().द्वा_खोज (x, not_found); }
  ढाँचा <typename T,
	    bool Sorted=sorted, hb_enable_if (Sorted)>
  स्थिर Type *द्वा_खोज (स्थिर T &x, स्थिर Type *not_found = nullptr) स्थिर
  { वापस as_array ().द्वा_खोज (x, not_found); }
  ढाँचा <typename T,
	    bool Sorted=sorted, hb_enable_if (Sorted)>
  bool bfind (स्थिर T &x, अचिन्हित पूर्णांक *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      अचिन्हित पूर्णांक to_store = (अचिन्हित पूर्णांक) -1) स्थिर
  { वापस as_array ().bfind (x, i, not_found, to_store); }
};

ढाँचा <typename Type>
using hb_sorted_vector_t = hb_vector_t<Type, true>;

#पूर्ण_अगर /* HB_VECTOR_HH */

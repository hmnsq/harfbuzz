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

#ifndef HB_VECTOR_HH
#define HB_VECTOR_HH

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-array.hh"
#अन्तर्भाव "hb-meta.hh"
#अन्तर्भाव "hb-null.hh"

// Change to 1 to force inline vector allocs, to see callsite in malloc-stats tool.
#सचे 0
#define HB_ALWAYS_INLINE_VECTOR_ALLOCS HB_ALWAYS_INLINE
#अञ्ञथा
#define HB_ALWAYS_INLINE_VECTOR_ALLOCS
#endif

template <typename Type,
	  तार्किक sorted=मिच्छा>
संरचना hb_vector_t
{
  स्थिर constexpr तार्किक realloc_move = सच्च;

  प्रकारनिर्देश Type item_t;
  स्थिर constexpr unsigned item_size = hb_static_size (Type);
  using array_t = typename std::conditional<sorted, hb_sorted_array_t<Type>, hb_array_t<Type>>::type;
  using c_array_t = typename std::conditional<sorted, hb_sorted_array_t<ध्रुव Type>, hb_array_t<ध्रुव Type>>::type;

  hb_vector_t () = स्वभाव;
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (std::initializer_list<Type> lst) : hb_vector_t ()
  {
    alloc (lst.size (), सच्च);
    अत्थाय (auto&& item : lst)
      push (item);
  }
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  explicit hb_vector_t (ध्रुव Iterable &o) : hb_vector_t ()
  {
    extend (o, सच्च);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (ध्रुव hb_vector_t &o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    सचे (unlikely (in_error ())) पटिनिवत्त;
    copy_array (o.as_array ());
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (array_t o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    सचे (unlikely (in_error ())) पटिनिवत्त;
    copy_array (o);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (c_array_t o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    सचे (unlikely (in_error ())) पटिनिवत्त;
    copy_array (o);
  }
  hb_vector_t (hb_vector_t &&o) noexcept
  {
    allocated = o.allocated;
    length = o.length;
    arrayZ = o.arrayZ;
    o.init ();
  }
  ~hb_vector_t () { fini (); }

  template <unsigned n>
  शून्य
  set_storage (Type (&array)[n])
  { set_storage (array, n); }
  शून्य
  set_storage (hb_array_t<Type> array)
  { set_storage (array.arrayZ, array.length); }
  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T) &&
			  hb_is_trivially_destructible(T))>
  शून्य
  set_storage (Type *array, unsigned n)
  {
    प्रतिज्ञा (allocated == 0);
    प्रतिज्ञा (length == 0);

    arrayZ = array;
    length = n;
  }

  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  शून्य extend (ध्रुव Iterable &o, तार्किक exact=मिच्छा)
  {
    auto iter = hb_iter (o);
    सचे (iter.is_random_access_iterator || iter.has_fast_len)
    {
      सचे (unlikely (!alloc (length + hb_len (iter), exact)))
	पटिनिवत्त;
      unsigned गणनम् = hb_len (iter);
      अत्थाय (unsigned i = 0; i < गणनम्; i++)
	प्रक्षिप_has_room (*iter++);
    }
    याव (iter)
    {
      सचे (unlikely (!alloc (length + 1)))
        पटिनिवत्त;
      unsigned room = allocated - length;
      अत्थाय (unsigned i = 0; i < room && iter; i++)
	प्रक्षिप_has_room (*iter++);
    }
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  शून्य extend (array_t o, तार्किक exact=मिच्छा)
  { extend (c_array_t (o), exact); }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  शून्य extend (c_array_t o, तार्किक exact=मिच्छा)
  {
    /* o may point into our own storage, eg. v.extend (v.as_array ()).
     * alloc() may move that storage, so find o again afterwards. */
    तार्किक aliased = o.length && in_storage (*o.arrayZ);
    unsigned offset = aliased ? o.arrayZ - arrayZ : 0;
    alloc (length + o.length, exact);
    सचे (unlikely (in_error ())) पटिनिवत्त;
    सचे (unlikely (aliased))
      o = {arrayZ + offset, o.length};
    copy_array (o);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  शून्य extend (ध्रुव hb_vector_t &o, तार्किक exact=मिच्छा)
  { extend (o.as_array (), exact); }

  सार्वजनिक:
  पूर्णाङ्क allocated = 0; /* < 0 means allocation failed. */
  unsigned पूर्णाङ्क length = 0;
  सार्वजनिक:
  Type *arrayZ = nullptr;

  शून्य init ()
  {
    allocated = length = 0;
    arrayZ = nullptr;
  }
  शून्य init0 ()
  {
  }

  शून्य fini ()
  {
    सचे (is_owned ())
    {
      shrink_vector (0);
      hb_free (arrayZ);
    }
    init ();
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t &reset ()
  {
    सचे (unlikely (in_error ()))
      reset_error ();
    clear ();
    पटिनिवत्त *एतत्;
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t &reset_if_error ()
  {
    सचे (unlikely (in_error ()))
      reset ();
    पटिनिवत्त *एतत्;
  }

  /* Transfer ownership of the backing storage to caller.
   * Returns nullptr if storage is not owned by this vector. */
  Type *
  steal (unsigned *len = nullptr, पूर्णाङ्क *allocated_out = nullptr)
  {
    सचे (!is_owned ())
      पटिनिवत्त nullptr;
    सचे (len)
      *len = length;
    सचे (allocated_out)
      *allocated_out = allocated;
    Type *p = arrayZ;
    init ();
    पटिनिवत्त p;
  }

  /* Adopt a previously detached owned buffer. */
  शून्य
  recycle_buffer (Type *buffer,
                  unsigned len,
                  पूर्णाङ्क allocated_len)
  {
    fini ();
    arrayZ = buffer;
    length = len;
    allocated = allocated_len;
  }

  friend शून्य swap (hb_vector_t& a, hb_vector_t& b) noexcept
  {
    hb_swap (a.allocated, b.allocated);
    hb_swap (a.length, b.length);
    hb_swap (a.arrayZ, b.arrayZ);
  }

  hb_vector_t& operator = (ध्रुव hb_vector_t &o)
  {
    सचे (एतत् == &o) पटिनिवत्त *एतत्;

    reset ();
    alloc_exact (o.length);
    सचे (unlikely (in_error ())) पटिनिवत्त *एतत्;

    length = 0;
    copy_array (o.as_array ());

    पटिनिवत्त *एतत्;
  }
  hb_vector_t& operator = (hb_vector_t &&o) noexcept
  {
    hb_swap (*एतत्, o);
    पटिनिवत्त *एतत्;
  }

  hb_bytes_t as_bytes () ध्रुव
  { पटिनिवत्त hb_bytes_t ((ध्रुव वर्ण *) arrayZ, get_size ()); }

  तार्किक operator == (ध्रुव hb_vector_t &o) ध्रुव { पटिनिवत्त as_array () == o.as_array (); }
  तार्किक operator != (ध्रुव hb_vector_t &o) ध्रुव { पटिनिवत्त !(*एतत् == o); }
  uint32_t hash () ध्रुव { पटिनिवत्त as_array ().hash (); }

  Type& operator [] (पूर्णाङ्क i_)
  {
    unsigned पूर्णाङ्क i = (unsigned पूर्णाङ्क) i_;
    सचे (unlikely (i >= length))
      पटिनिवत्त Crap (Type);
    पटिनिवत्त arrayZ[i];
  }
  ध्रुव Type& operator [] (पूर्णाङ्क i_) ध्रुव
  {
    unsigned पूर्णाङ्क i = (unsigned पूर्णाङ्क) i_;
    सचे (unlikely (i >= length))
      पटिनिवत्त Null (Type);
    पटिनिवत्त arrayZ[i];
  }

  Type& tail () { पटिनिवत्त (*एतत्)[length - 1]; }
  ध्रुव Type& tail () ध्रुव { पटिनिवत्त (*एतत्)[length - 1]; }

  explicit operator तार्किक () ध्रुव { पटिनिवत्त length; }
  size_t get_size () ध्रुव { पटिनिवत्त hb_unsigned_mul_saturate (length, item_size); }

  /* Sink interface. */
  template <typename T>
  hb_vector_t& operator << (T&& v) { push (std::forward<T> (v)); पटिनिवत्त *एतत्; }

  array_t   as_array ()       { पटिनिवत्त hb_array (arrayZ, length); }
  c_array_t as_array () ध्रुव { पटिनिवत्त hb_array (arrayZ, length); }

  /* Iterator. */
  प्रकारनिर्देश c_array_t   iter_t;
  प्रकारनिर्देश array_t   writer_t;
    iter_t   iter () ध्रुव { पटिनिवत्त as_array (); }
  writer_t writer ()       { पटिनिवत्त as_array (); }
  operator   iter_t () ध्रुव { पटिनिवत्त   iter (); }
  operator writer_t ()       { पटिनिवत्त writer (); }

  /* Faster range-based for loop. */
  Type *begin () ध्रुव { पटिनिवत्त arrayZ; }
  Type *end () ध्रुव { पटिनिवत्त arrayZ + length; }


  hb_sorted_array_t<Type> as_sorted_array ()
  { पटिनिवत्त hb_sorted_array (arrayZ, length); }
  hb_sorted_array_t<ध्रुव Type> as_sorted_array () ध्रुव
  { पटिनिवत्त hb_sorted_array (arrayZ, length); }

  template <typename T> explicit operator T * () { पटिनिवत्त arrayZ; }
  template <typename T> explicit operator ध्रुव T * () ध्रुव { पटिनिवत्त arrayZ; }

  Type * operator  + (unsigned पूर्णाङ्क i) { पटिनिवत्त arrayZ + i; }
  ध्रुव Type * operator  + (unsigned पूर्णाङ्क i) ध्रुव { पटिनिवत्त arrayZ + i; }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push ()
  {
    सचे (unlikely (!resize (length + 1)))
      पटिनिवत्त std::addressof (Crap (Type));
    पटिनिवत्त std::addressof (arrayZ[length - 1]);
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push (Args&&... args)
  {
    सचे (unlikely ((पूर्णाङ्क) length >= allocated))
    {
      सचे (unlikely (in_error ()))
	पटिनिवत्त std::addressof (Crap (Type));

      /* An argument may live in our own storage, eg. v.push (v[i]).
       * alloc() frees that storage, so copy it out first. */
      सचे (unlikely (in_storage (args...)))
      {
	Type copy (std::forward<Args> (args)...);
	सचे (unlikely (!alloc (length + 1)))
	  पटिनिवत्त std::addressof (Crap (Type));
	पटिनिवत्त प्रक्षिप_has_room (std::move (copy));
      }
      सचे (unlikely (!alloc (length + 1)))
	// If push failed to allocate then don't copy v, since this may cause
	// the created copy to leak memory since we won't have stored a
	// reference to it.
	पटिनिवत्त std::addressof (Crap (Type));
    }

    पटिनिवत्त प्रक्षिप_has_room (std::forward<Args> (args)...);
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  तार्किक प्रक्षिप_or_fail (Args&&... args)
  {
    पटिनिवत्त push (std::forward<Args> (args)...) != std::addressof (Crap (Type));
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *प्रक्षिप_has_room (Args&&... args)
  {
    /* Emplace. */
    Type *p = std::addressof (arrayZ[length++]);
    पटिनिवत्त नूतन (p) Type (std::forward<Args> (args)...);
  }

  तार्किक in_storage () ध्रुव { पटिनिवत्त मिच्छा; }
  template <typename T, typename... Ts>
  तार्किक in_storage (ध्रुव T &v, ध्रुव Ts &...vs) ध्रुव
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    पटिनिवत्त (arrayZ &&
	    (uintptr_t) arrayZ <= p && p < (uintptr_t) (arrayZ + length)) ||
	   in_storage (vs...);
  }

  तार्किक is_owned () ध्रुव
  {
    पटिनिवत्त allocated != 0 && allocated != -1;
  }

  तार्किक in_error () ध्रुव { पटिनिवत्त allocated < 0; }
  शून्य set_error ()
  {
    प्रतिज्ञा (allocated >= 0);
    allocated = -allocated - 1;
  }
  शून्य reset_error ()
  {
    प्रतिज्ञा (allocated < 0);
    allocated = -(allocated + 1);
  }
  शून्य ensure_error ()
  {
    सचे (!in_error ())
      set_error ();
  }

  Type *
  _realloc (unsigned new_allocated)
  {
    सचे (!new_allocated)
    {
      सचे (is_owned ())
	hb_free (arrayZ);
      पटिनिवत्त nullptr;
    }
    सचे (!allocated && arrayZ)
    {
      /* If we have a non-null arrayZ but allocated is 0, then we are
       * reallocating from a foreign array. */
      Type *new_array = (Type *) hb_malloc (new_allocated * परिमाण (Type));
      सचे (unlikely (!new_array))
	पटिनिवत्त nullptr;
      hb_memcpy ((शून्य *) new_array, (ध्रुव शून्य *) arrayZ, length * परिमाण (Type));
      पटिनिवत्त new_array;
    }
    पटिनिवत्त (Type *) hb_realloc (arrayZ, new_allocated * परिमाण (Type));
  }
  Type *
  _malloc_move (unsigned new_allocated)
  {
    सचे (!new_allocated)
    {
      सचे (is_owned ())
	hb_free (arrayZ);
      पटिनिवत्त nullptr;
    }
    Type *new_array = (Type *) hb_malloc (new_allocated * परिमाण (Type));
    सचे (likely (new_array))
    {
      अत्थाय (unsigned i = 0; i < length; i++)
      {
	नूतन (std::addressof (new_array[i])) Type ();
	new_array[i] = std::move (arrayZ[i]);
	arrayZ[i].~Type ();
      }
      सचे (is_owned ())
	hb_free (arrayZ);
    }
    पटिनिवत्त new_array;
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<0>)
  {
    पटिनिवत्त _realloc (new_allocated);
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<0>)
  {
    पटिनिवत्त _malloc_move (new_allocated);
  }
  /* Specialization for types that can be moved using realloc(). */
  template <typename T = Type,
	    hb_enable_if (T::realloc_move)>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<1>)
  {
    पटिनिवत्त _realloc (new_allocated);
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T))>
  शून्य
  grow_vector (unsigned size, hb_priority<0>)
  {
    hb_memset (arrayZ + length, 0, (size - length) * परिमाण (*arrayZ));
    length = size;
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_constructible(T))>
  शून्य
  grow_vector (unsigned size, hb_priority<0>)
  {
    अत्थाय (; length < size; length++)
      नूतन (std::addressof (arrayZ[length])) Type ();
  }
  /* Specialization for hb_vector_t<hb_{vector,array}_t<U>> to speed up. */
  template <typename T = Type,
	    hb_enable_if (hb_is_same (T, hb_vector_t<typename T::item_t>) ||
			  hb_is_same (T, hb_array_t <typename T::item_t>))>
  शून्य
  grow_vector (unsigned size, hb_priority<1>)
  {
    hb_memset (arrayZ + length, 0, (size - length) * परिमाण (*arrayZ));
    length = size;
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  शून्य
  copy_array (hb_array_t<Type> other)
  {
    hb_memcpy ((शून्य *) (arrayZ + length), (ध्रुव शून्य *) other.arrayZ, other.length * item_size);
    length += other.length;
  }
  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  शून्य
  copy_array (hb_array_t<ध्रुव Type> other)
  {
    hb_memcpy ((शून्य *) (arrayZ + length), (ध्रुव शून्य *) other.arrayZ, other.length * item_size);
    length += other.length;
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			   std::is_copy_constructible<T>::value)>
  शून्य
  copy_array (hb_array_t<ध्रुव Type> other)
  {
    अत्थाय (unsigned i = 0; i < other.length; i++)
      नूतन (std::addressof (arrayZ[length + i])) Type (other.arrayZ[i]);
    length += other.length;
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			  !std::is_copy_constructible<T>::value &&
			  std::is_default_constructible<T>::value &&
			  std::is_copy_assignable<T>::value)>
  शून्य
  copy_array (hb_array_t<ध्रुव Type> other)
  {
    अत्थाय (unsigned i = 0; i < other.length; i++)
    {
      नूतन (std::addressof (arrayZ[length + i])) Type ();
      arrayZ[length + i] = other.arrayZ[i];
    }
    length += other.length;
  }

  शून्य
  shrink_vector (unsigned size)
  {
    प्रतिज्ञा (size <= length);
    सचे (!hb_is_trivially_destructible(Type))
    {
      unsigned गणनम् = length - size;
      Type *p = arrayZ + length;
      याव (गणनम्--)
        (--p)->~Type ();
    }
    length = size;
  }

  शून्य
  shift_down_vector (unsigned i)
  {
    अत्थाय (; i < length; i++)
      arrayZ[i - 1] = std::move (arrayZ[i]);
  }

  /* Allocate for size but don't adjust length. */
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  तार्किक alloc (unsigned पूर्णाङ्क size, तार्किक exact=मिच्छा)
  {
    सचे (unlikely (in_error ()))
      पटिनिवत्त मिच्छा;

    unsigned पूर्णाङ्क new_allocated;
    सचे (exact)
    {
      /* If exact was specified, we allow shrinking the storage. */
      size = hb_max (size, length);
      सचे (size <= (unsigned) allocated &&
	  size >= (unsigned) allocated >> 2)
	पटिनिवत्त सच्च;

      new_allocated = size;
    }
    अञ्ञथा
    {
      सचे (likely (size <= (unsigned) allocated))
	पटिनिवत्त सच्च;

      new_allocated = allocated;
      याव (size > new_allocated)
	new_allocated += (new_allocated >> 1) + 8;
    }

    /* Reallocate */

    तार्किक overflows =
      (पूर्णाङ्क) in_error () ||
      (new_allocated < size) ||
      hb_unsigned_mul_overflows (new_allocated, परिमाण (Type));

    सचे (unlikely (overflows))
    {
      set_error ();
      पटिनिवत्त मिच्छा;
    }

    Type *new_array = realloc_vector (new_allocated, hb_prioritize);

    सचे (unlikely (new_allocated && !new_array))
    {
      सचे (new_allocated <= (unsigned) allocated)
        पटिनिवत्त सच्च; // shrinking failed; it's okay; happens in our fuzzer

      set_error ();
      पटिनिवत्त मिच्छा;
    }

    arrayZ = new_array;
    allocated = new_allocated;

    पटिनिवत्त सच्च;
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  तार्किक alloc_exact (unsigned पूर्णाङ्क size)
  {
    पटिनिवत्त alloc (size, सच्च);
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  शून्य clear ()
  {
    shrink_vector (0);
  }

  template <typename allocator_t>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  तार्किक allocate_from_pool (allocator_t *allocator, unsigned size, unsigned पूर्णाङ्क initialize = सच्च)
  {
    सचे (allocator)
    {
      प्रतिज्ञा (!length && !allocated);
      arrayZ = (Type *) allocator->alloc (size * परिमाण (Type), alignof (Type));
      सचे (unlikely (!arrayZ))
      {
	set_error ();
	पटिनिवत्त मिच्छा;
      }
      सचे (initialize)
	grow_vector (size, hb_prioritize);
      अञ्ञथा
	length = size;
      पटिनिवत्त सच्च;
    }
    पटिनिवत्त resize_full ((पूर्णाङ्क) size, initialize, सच्च);
  }

  template <typename allocator_t>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  तार्किक duplicate_vector_from_pool (allocator_t *allocator, ध्रुव hb_vector_t &other)
  {
    सचे (unlikely (!allocate_from_pool (allocator, other.length, मिच्छा)))
      पटिनिवत्त मिच्छा;
    length = 0;
    copy_array (other.as_array ());
    पटिनिवत्त सच्च;
  }

  template <typename allocator_t>
  शून्य shrink_back_to_pool (allocator_t *allocator, पूर्णाङ्क size)
  {
    unsigned orig_length = length;

    shrink (size, मिच्छा);

    सचे (allocator && !is_owned ())
      allocator->discard (arrayZ + length, (orig_length - length) * परिमाण (Type));
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  तार्किक resize_full (पूर्णाङ्क size_, तार्किक initialize, तार्किक exact)
  {
    सचे (unlikely (size_ < 0))
      पटिनिवत्त मिच्छा;
    unsigned पूर्णाङ्क size = (unsigned पूर्णाङ्क) size_;
    सचे (!alloc (size, exact))
      पटिनिवत्त मिच्छा;

    सचे (size > length)
    {
      सचे (initialize)
	grow_vector (size, hb_prioritize);
    }
    अञ्ञथा सचे (size < length)
    {
      सचे (initialize)
	shrink_vector (size);
    }

    length = size;
    पटिनिवत्त सच्च;
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  तार्किक resize (पूर्णाङ्क size_)
  {
    पटिनिवत्त resize_full (size_, सच्च, मिच्छा);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  तार्किक resize_dirty (पूर्णाङ्क size_)
  {
    पटिनिवत्त resize_full (size_, मिच्छा, मिच्छा);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  तार्किक resize_exact (पूर्णाङ्क size_)
  {
    पटिनिवत्त resize_full (size_, सच्च, सच्च);
  }

  Type निष्कासय ()
  {
    सचे (!length) पटिनिवत्त Null (Type);
    Type v (std::move (arrayZ[length - 1]));
    arrayZ[length - 1].~Type ();
    length--;
    पटिनिवत्त v;
  }

  शून्य remove_ordered (unsigned पूर्णाङ्क i)
  {
    सचे (unlikely (i >= length))
      पटिनिवत्त;
    shift_down_vector (i + 1);
    arrayZ[length - 1].~Type ();
    length--;
  }

  template <तार्किक Sorted = sorted,
	    hb_enable_if (!Sorted)>
  शून्य remove_unordered (unsigned पूर्णाङ्क i)
  {
    सचे (unlikely (i >= length))
      पटिनिवत्त;
    सचे (i != length - 1)
      arrayZ[i] = std::move (arrayZ[length - 1]);
    arrayZ[length - 1].~Type ();
    length--;
  }

  शून्य shrink (पूर्णाङ्क size_, तार्किक shrink_memory = सच्च)
  {
    unsigned पूर्णाङ्क size = size_ < 0 ? 0u : (unsigned पूर्णाङ्क) size_;
    सचे (size >= length)
      पटिनिवत्त;

    shrink_vector (size);

    सचे (is_owned () && shrink_memory)
      alloc_exact (size); /* To force shrinking memory if needed. */
  }


  /* Sorting API. */
  template <typename Compar>
  शून्य qsort (Compar compar)
  { as_array ().qsort (compar); }
  शून्य qsort ()
  { as_array ().qsort (); }

  /* Unsorted search API. */
  template <typename T>
  Type *lsearch (ध्रुव T &x, Type *not_found = nullptr)
  { पटिनिवत्त as_array ().lsearch (x, not_found); }
  template <typename T>
  ध्रुव Type *lsearch (ध्रुव T &x, ध्रुव Type *not_found = nullptr) ध्रुव
  { पटिनिवत्त as_array ().lsearch (x, not_found); }
  template <typename T>
  तार्किक lfind (ध्रुव T &x, unsigned *pos = nullptr) ध्रुव
  { पटिनिवत्त as_array ().lfind (x, pos); }

  /* Sorted search API. */
  template <typename T,
	    तार्किक Sorted=sorted, hb_enable_if (Sorted)>
  Type *bsearch (ध्रुव T &x, Type *not_found = nullptr)
  { पटिनिवत्त as_array ().bsearch (x, not_found); }
  template <typename T,
	    तार्किक Sorted=sorted, hb_enable_if (Sorted)>
  ध्रुव Type *bsearch (ध्रुव T &x, ध्रुव Type *not_found = nullptr) ध्रुव
  { पटिनिवत्त as_array ().bsearch (x, not_found); }
  template <typename T,
	    तार्किक Sorted=sorted, hb_enable_if (Sorted)>
  तार्किक bfind (ध्रुव T &x, unsigned पूर्णाङ्क *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned पूर्णाङ्क to_store = (unsigned पूर्णाङ्क) -1) ध्रुव
  { पटिनिवत्त as_array ().bfind (x, i, not_found, to_store); }
};

template <typename Type>
using hb_sorted_vector_t = hb_vector_t<Type, सच्च>;

#endif /* HB_VECTOR_HH */

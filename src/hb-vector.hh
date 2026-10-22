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
#यदि 0
#define HB_ALWAYS_INLINE_VECTOR_ALLOCS HB_ALWAYS_INLINE
#अन्यथा
#define HB_ALWAYS_INLINE_VECTOR_ALLOCS
#endif

template <typename Type,
	  तार्किक sorted=असत्य>
संरचना hb_vector_t
{
  स्थिर constexpr तार्किक realloc_move = सत्य;

  प्रकारनिर्देश Type item_t;
  स्थिर constexpr unsigned item_size = hb_static_size (Type);
  using array_t = typename std::conditional<sorted, hb_sorted_array_t<Type>, hb_array_t<Type>>::type;
  using c_array_t = typename std::conditional<sorted, hb_sorted_array_t<ध्रुव Type>, hb_array_t<ध्रुव Type>>::type;

  hb_vector_t () = स्वभाव;
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (std::initializer_list<Type> lst) : hb_vector_t ()
  {
    alloc (lst.size (), सत्य);
    प्रति (auto&& item : lst)
      push (item);
  }
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  explicit hb_vector_t (ध्रुव Iterable &o) : hb_vector_t ()
  {
    extend (o, सत्य);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (ध्रुव hb_vector_t &o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    यदि (unlikely (in_error ())) प्रत्यागम;
    copy_array (o.as_array ());
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (array_t o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    यदि (unlikely (in_error ())) प्रत्यागम;
    copy_array (o);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (c_array_t o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    यदि (unlikely (in_error ())) प्रत्यागम;
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
  शून्य extend (ध्रुव Iterable &o, तार्किक exact=असत्य)
  {
    auto iter = hb_iter (o);
    यदि (iter.is_random_access_iterator || iter.has_fast_len)
    {
      यदि (unlikely (!alloc (length + hb_len (iter), exact)))
	प्रत्यागम;
      unsigned गिनती = hb_len (iter);
      प्रति (unsigned i = 0; i < गिनती; i++)
	धकेलो_has_room (*iter++);
    }
    यावत् (iter)
    {
      यदि (unlikely (!alloc (length + 1)))
        प्रत्यागम;
      unsigned room = allocated - length;
      प्रति (unsigned i = 0; i < room && iter; i++)
	धकेलो_has_room (*iter++);
    }
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  शून्य extend (array_t o, तार्किक exact=असत्य)
  { extend (c_array_t (o), exact); }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  शून्य extend (c_array_t o, तार्किक exact=असत्य)
  {
    /* o may point into our own storage, eg. v.extend (v.as_array ()).
     * alloc() may move that storage, so find o again afterwards. */
    तार्किक aliased = o.length && in_storage (*o.arrayZ);
    unsigned offset = aliased ? o.arrayZ - arrayZ : 0;
    alloc (length + o.length, exact);
    यदि (unlikely (in_error ())) प्रत्यागम;
    यदि (unlikely (aliased))
      o = {arrayZ + offset, o.length};
    copy_array (o);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  शून्य extend (ध्रुव hb_vector_t &o, तार्किक exact=असत्य)
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
    यदि (is_owned ())
    {
      shrink_vector (0);
      hb_free (arrayZ);
    }
    init ();
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t &reset ()
  {
    यदि (unlikely (in_error ()))
      reset_error ();
    clear ();
    प्रत्यागम *एतत्;
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t &reset_if_error ()
  {
    यदि (unlikely (in_error ()))
      reset ();
    प्रत्यागम *एतत्;
  }

  /* Transfer ownership of the backing storage to caller.
   * Returns nullptr if storage is not owned by this vector. */
  Type *
  steal (unsigned *len = nullptr, पूर्णाङ्क *allocated_out = nullptr)
  {
    यदि (!is_owned ())
      प्रत्यागम nullptr;
    यदि (len)
      *len = length;
    यदि (allocated_out)
      *allocated_out = allocated;
    Type *p = arrayZ;
    init ();
    प्रत्यागम p;
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
    यदि (एतत् == &o) प्रत्यागम *एतत्;

    reset ();
    alloc_exact (o.length);
    यदि (unlikely (in_error ())) प्रत्यागम *एतत्;

    length = 0;
    copy_array (o.as_array ());

    प्रत्यागम *एतत्;
  }
  hb_vector_t& operator = (hb_vector_t &&o) noexcept
  {
    hb_swap (*एतत्, o);
    प्रत्यागम *एतत्;
  }

  hb_bytes_t as_bytes () ध्रुव
  { प्रत्यागम hb_bytes_t ((ध्रुव वर्ण *) arrayZ, get_size ()); }

  तार्किक operator == (ध्रुव hb_vector_t &o) ध्रुव { प्रत्यागम as_array () == o.as_array (); }
  तार्किक operator != (ध्रुव hb_vector_t &o) ध्रुव { प्रत्यागम !(*एतत् == o); }
  uint32_t hash () ध्रुव { प्रत्यागम as_array ().hash (); }

  Type& operator [] (पूर्णाङ्क i_)
  {
    unsigned पूर्णाङ्क i = (unsigned पूर्णाङ्क) i_;
    यदि (unlikely (i >= length))
      प्रत्यागम Crap (Type);
    प्रत्यागम arrayZ[i];
  }
  ध्रुव Type& operator [] (पूर्णाङ्क i_) ध्रुव
  {
    unsigned पूर्णाङ्क i = (unsigned पूर्णाङ्क) i_;
    यदि (unlikely (i >= length))
      प्रत्यागम Null (Type);
    प्रत्यागम arrayZ[i];
  }

  Type& tail () { प्रत्यागम (*एतत्)[length - 1]; }
  ध्रुव Type& tail () ध्रुव { प्रत्यागम (*एतत्)[length - 1]; }

  explicit operator तार्किक () ध्रुव { प्रत्यागम length; }
  size_t get_size () ध्रुव { प्रत्यागम hb_unsigned_mul_saturate (length, item_size); }

  /* Sink interface. */
  template <typename T>
  hb_vector_t& operator << (T&& v) { push (std::forward<T> (v)); प्रत्यागम *एतत्; }

  array_t   as_array ()       { प्रत्यागम hb_array (arrayZ, length); }
  c_array_t as_array () ध्रुव { प्रत्यागम hb_array (arrayZ, length); }

  /* Iterator. */
  प्रकारनिर्देश c_array_t   iter_t;
  प्रकारनिर्देश array_t   writer_t;
    iter_t   iter () ध्रुव { प्रत्यागम as_array (); }
  writer_t writer ()       { प्रत्यागम as_array (); }
  operator   iter_t () ध्रुव { प्रत्यागम   iter (); }
  operator writer_t ()       { प्रत्यागम writer (); }

  /* Faster range-based for loop. */
  Type *begin () ध्रुव { प्रत्यागम arrayZ; }
  Type *end () ध्रुव { प्रत्यागम arrayZ + length; }


  hb_sorted_array_t<Type> as_sorted_array ()
  { प्रत्यागम hb_sorted_array (arrayZ, length); }
  hb_sorted_array_t<ध्रुव Type> as_sorted_array () ध्रुव
  { प्रत्यागम hb_sorted_array (arrayZ, length); }

  template <typename T> explicit operator T * () { प्रत्यागम arrayZ; }
  template <typename T> explicit operator ध्रुव T * () ध्रुव { प्रत्यागम arrayZ; }

  Type * operator  + (unsigned पूर्णाङ्क i) { प्रत्यागम arrayZ + i; }
  ध्रुव Type * operator  + (unsigned पूर्णाङ्क i) ध्रुव { प्रत्यागम arrayZ + i; }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push ()
  {
    यदि (unlikely (!resize (length + 1)))
      प्रत्यागम std::addressof (Crap (Type));
    प्रत्यागम std::addressof (arrayZ[length - 1]);
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push (Args&&... args)
  {
    यदि (unlikely ((पूर्णाङ्क) length >= allocated))
    {
      यदि (unlikely (in_error ()))
	प्रत्यागम std::addressof (Crap (Type));

      /* An argument may live in our own storage, eg. v.push (v[i]).
       * alloc() frees that storage, so copy it out first. */
      यदि (unlikely (in_storage (args...)))
      {
	Type copy (std::forward<Args> (args)...);
	यदि (unlikely (!alloc (length + 1)))
	  प्रत्यागम std::addressof (Crap (Type));
	प्रत्यागम धकेलो_has_room (std::move (copy));
      }
      यदि (unlikely (!alloc (length + 1)))
	// If push failed to allocate then don't copy v, since this may cause
	// the created copy to leak memory since we won't have stored a
	// reference to it.
	प्रत्यागम std::addressof (Crap (Type));
    }

    प्रत्यागम धकेलो_has_room (std::forward<Args> (args)...);
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  तार्किक धकेलो_or_fail (Args&&... args)
  {
    प्रत्यागम push (std::forward<Args> (args)...) != std::addressof (Crap (Type));
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *धकेलो_has_room (Args&&... args)
  {
    /* Emplace. */
    Type *p = std::addressof (arrayZ[length++]);
    प्रत्यागम नूतन (p) Type (std::forward<Args> (args)...);
  }

  तार्किक in_storage () ध्रुव { प्रत्यागम असत्य; }
  template <typename T, typename... Ts>
  तार्किक in_storage (ध्रुव T &v, ध्रुव Ts &...vs) ध्रुव
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    प्रत्यागम (arrayZ &&
	    (uintptr_t) arrayZ <= p && p < (uintptr_t) (arrayZ + length)) ||
	   in_storage (vs...);
  }

  तार्किक is_owned () ध्रुव
  {
    प्रत्यागम allocated != 0 && allocated != -1;
  }

  तार्किक in_error () ध्रुव { प्रत्यागम allocated < 0; }
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
    यदि (!in_error ())
      set_error ();
  }

  Type *
  _realloc (unsigned new_allocated)
  {
    यदि (!new_allocated)
    {
      यदि (is_owned ())
	hb_free (arrayZ);
      प्रत्यागम nullptr;
    }
    यदि (!allocated && arrayZ)
    {
      /* If we have a non-null arrayZ but allocated is 0, then we are
       * reallocating from a foreign array. */
      Type *new_array = (Type *) hb_malloc (new_allocated * परिमाण (Type));
      यदि (unlikely (!new_array))
	प्रत्यागम nullptr;
      hb_memcpy ((शून्य *) new_array, (ध्रुव शून्य *) arrayZ, length * परिमाण (Type));
      प्रत्यागम new_array;
    }
    प्रत्यागम (Type *) hb_realloc (arrayZ, new_allocated * परिमाण (Type));
  }
  Type *
  _malloc_move (unsigned new_allocated)
  {
    यदि (!new_allocated)
    {
      यदि (is_owned ())
	hb_free (arrayZ);
      प्रत्यागम nullptr;
    }
    Type *new_array = (Type *) hb_malloc (new_allocated * परिमाण (Type));
    यदि (likely (new_array))
    {
      प्रति (unsigned i = 0; i < length; i++)
      {
	नूतन (std::addressof (new_array[i])) Type ();
	new_array[i] = std::move (arrayZ[i]);
	arrayZ[i].~Type ();
      }
      यदि (is_owned ())
	hb_free (arrayZ);
    }
    प्रत्यागम new_array;
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<0>)
  {
    प्रत्यागम _realloc (new_allocated);
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<0>)
  {
    प्रत्यागम _malloc_move (new_allocated);
  }
  /* Specialization for types that can be moved using realloc(). */
  template <typename T = Type,
	    hb_enable_if (T::realloc_move)>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<1>)
  {
    प्रत्यागम _realloc (new_allocated);
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
    प्रति (; length < size; length++)
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
    प्रति (unsigned i = 0; i < other.length; i++)
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
    प्रति (unsigned i = 0; i < other.length; i++)
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
    यदि (!hb_is_trivially_destructible(Type))
    {
      unsigned गिनती = length - size;
      Type *p = arrayZ + length;
      यावत् (गिनती--)
        (--p)->~Type ();
    }
    length = size;
  }

  शून्य
  shift_down_vector (unsigned i)
  {
    प्रति (; i < length; i++)
      arrayZ[i - 1] = std::move (arrayZ[i]);
  }

  /* Allocate for size but don't adjust length. */
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  तार्किक alloc (unsigned पूर्णाङ्क size, तार्किक exact=असत्य)
  {
    यदि (unlikely (in_error ()))
      प्रत्यागम असत्य;

    unsigned पूर्णाङ्क new_allocated;
    यदि (exact)
    {
      /* If exact was specified, we allow shrinking the storage. */
      size = hb_max (size, length);
      यदि (size <= (unsigned) allocated &&
	  size >= (unsigned) allocated >> 2)
	प्रत्यागम सत्य;

      new_allocated = size;
    }
    अन्यथा
    {
      यदि (likely (size <= (unsigned) allocated))
	प्रत्यागम सत्य;

      new_allocated = allocated;
      यावत् (size > new_allocated)
	new_allocated += (new_allocated >> 1) + 8;
    }

    /* Reallocate */

    तार्किक overflows =
      (पूर्णाङ्क) in_error () ||
      (new_allocated < size) ||
      hb_unsigned_mul_overflows (new_allocated, परिमाण (Type));

    यदि (unlikely (overflows))
    {
      set_error ();
      प्रत्यागम असत्य;
    }

    Type *new_array = realloc_vector (new_allocated, hb_prioritize);

    यदि (unlikely (new_allocated && !new_array))
    {
      यदि (new_allocated <= (unsigned) allocated)
        प्रत्यागम सत्य; // shrinking failed; it's okay; happens in our fuzzer

      set_error ();
      प्रत्यागम असत्य;
    }

    arrayZ = new_array;
    allocated = new_allocated;

    प्रत्यागम सत्य;
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  तार्किक alloc_exact (unsigned पूर्णाङ्क size)
  {
    प्रत्यागम alloc (size, सत्य);
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  शून्य clear ()
  {
    shrink_vector (0);
  }

  template <typename allocator_t>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  तार्किक allocate_from_pool (allocator_t *allocator, unsigned size, unsigned पूर्णाङ्क initialize = सत्य)
  {
    यदि (allocator)
    {
      प्रतिज्ञा (!length && !allocated);
      arrayZ = (Type *) allocator->alloc (size * परिमाण (Type), alignof (Type));
      यदि (unlikely (!arrayZ))
      {
	set_error ();
	प्रत्यागम असत्य;
      }
      यदि (initialize)
	grow_vector (size, hb_prioritize);
      अन्यथा
	length = size;
      प्रत्यागम सत्य;
    }
    प्रत्यागम resize_full ((पूर्णाङ्क) size, initialize, सत्य);
  }

  template <typename allocator_t>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  तार्किक duplicate_vector_from_pool (allocator_t *allocator, ध्रुव hb_vector_t &other)
  {
    यदि (unlikely (!allocate_from_pool (allocator, other.length, असत्य)))
      प्रत्यागम असत्य;
    length = 0;
    copy_array (other.as_array ());
    प्रत्यागम सत्य;
  }

  template <typename allocator_t>
  शून्य shrink_back_to_pool (allocator_t *allocator, पूर्णाङ्क size)
  {
    unsigned orig_length = length;

    shrink (size, असत्य);

    यदि (allocator && !is_owned ())
      allocator->discard (arrayZ + length, (orig_length - length) * परिमाण (Type));
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  तार्किक resize_full (पूर्णाङ्क size_, तार्किक initialize, तार्किक exact)
  {
    यदि (unlikely (size_ < 0))
      प्रत्यागम असत्य;
    unsigned पूर्णाङ्क size = (unsigned पूर्णाङ्क) size_;
    यदि (!alloc (size, exact))
      प्रत्यागम असत्य;

    यदि (size > length)
    {
      यदि (initialize)
	grow_vector (size, hb_prioritize);
    }
    अन्यथा यदि (size < length)
    {
      यदि (initialize)
	shrink_vector (size);
    }

    length = size;
    प्रत्यागम सत्य;
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  तार्किक resize (पूर्णाङ्क size_)
  {
    प्रत्यागम resize_full (size_, सत्य, असत्य);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  तार्किक resize_dirty (पूर्णाङ्क size_)
  {
    प्रत्यागम resize_full (size_, असत्य, असत्य);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  तार्किक resize_exact (पूर्णाङ्क size_)
  {
    प्रत्यागम resize_full (size_, सत्य, सत्य);
  }

  Type निकालो ()
  {
    यदि (!length) प्रत्यागम Null (Type);
    Type v (std::move (arrayZ[length - 1]));
    arrayZ[length - 1].~Type ();
    length--;
    प्रत्यागम v;
  }

  शून्य remove_ordered (unsigned पूर्णाङ्क i)
  {
    यदि (unlikely (i >= length))
      प्रत्यागम;
    shift_down_vector (i + 1);
    arrayZ[length - 1].~Type ();
    length--;
  }

  template <तार्किक Sorted = sorted,
	    hb_enable_if (!Sorted)>
  शून्य remove_unordered (unsigned पूर्णाङ्क i)
  {
    यदि (unlikely (i >= length))
      प्रत्यागम;
    यदि (i != length - 1)
      arrayZ[i] = std::move (arrayZ[length - 1]);
    arrayZ[length - 1].~Type ();
    length--;
  }

  शून्य shrink (पूर्णाङ्क size_, तार्किक shrink_memory = सत्य)
  {
    unsigned पूर्णाङ्क size = size_ < 0 ? 0u : (unsigned पूर्णाङ्क) size_;
    यदि (size >= length)
      प्रत्यागम;

    shrink_vector (size);

    यदि (is_owned () && shrink_memory)
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
  { प्रत्यागम as_array ().lsearch (x, not_found); }
  template <typename T>
  ध्रुव Type *lsearch (ध्रुव T &x, ध्रुव Type *not_found = nullptr) ध्रुव
  { प्रत्यागम as_array ().lsearch (x, not_found); }
  template <typename T>
  तार्किक lfind (ध्रुव T &x, unsigned *pos = nullptr) ध्रुव
  { प्रत्यागम as_array ().lfind (x, pos); }

  /* Sorted search API. */
  template <typename T,
	    तार्किक Sorted=sorted, hb_enable_if (Sorted)>
  Type *bsearch (ध्रुव T &x, Type *not_found = nullptr)
  { प्रत्यागम as_array ().bsearch (x, not_found); }
  template <typename T,
	    तार्किक Sorted=sorted, hb_enable_if (Sorted)>
  ध्रुव Type *bsearch (ध्रुव T &x, ध्रुव Type *not_found = nullptr) ध्रुव
  { प्रत्यागम as_array ().bsearch (x, not_found); }
  template <typename T,
	    तार्किक Sorted=sorted, hb_enable_if (Sorted)>
  तार्किक bfind (ध्रुव T &x, unsigned पूर्णाङ्क *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned पूर्णाङ्क to_store = (unsigned पूर्णाङ्क) -1) ध्रुव
  { प्रत्यागम as_array ().bfind (x, i, not_found, to_store); }
};

template <typename Type>
using hb_sorted_vector_t = hb_vector_t<Type, सत्य>;

#endif /* HB_VECTOR_HH */

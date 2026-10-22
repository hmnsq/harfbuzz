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

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-array.hh"
#অন্তর্ভাব "hb-meta.hh"
#অন্তর্ভাব "hb-null.hh"

// Change to 1 to force inline vector allocs, to see callsite in malloc-stats tool.
#যদি 0
#define HB_ALWAYS_INLINE_VECTOR_ALLOCS HB_ALWAYS_INLINE
#অন্যথা
#define HB_ALWAYS_INLINE_VECTOR_ALLOCS
#endif

template <typename Type,
	  তার্কিক sorted=অসত্য>
সংরচনা hb_vector_t
{
  স্থির constexpr তার্কিক realloc_move = সত্য;

  প্রকারনির্দেশ Type item_t;
  স্থির constexpr unsigned item_size = hb_static_size (Type);
  using array_t = typename std::conditional<sorted, hb_sorted_array_t<Type>, hb_array_t<Type>>::type;
  using c_array_t = typename std::conditional<sorted, hb_sorted_array_t<ধ্রুব Type>, hb_array_t<ধ্রুব Type>>::type;

  hb_vector_t () = স্বভাব;
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (std::initializer_list<Type> lst) : hb_vector_t ()
  {
    alloc (lst.size (), সত্য);
    প্রতি (auto&& item : lst)
      push (item);
  }
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  explicit hb_vector_t (ধ্রুব Iterable &o) : hb_vector_t ()
  {
    extend (o, সত্য);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (ধ্রুব hb_vector_t &o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    যদি (unlikely (in_error ())) প্রত্যাগম;
    copy_array (o.as_array ());
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (array_t o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    যদি (unlikely (in_error ())) প্রত্যাগম;
    copy_array (o);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (c_array_t o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    যদি (unlikely (in_error ())) প্রত্যাগম;
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
  শূন্য
  set_storage (Type (&array)[n])
  { set_storage (array, n); }
  শূন্য
  set_storage (hb_array_t<Type> array)
  { set_storage (array.arrayZ, array.length); }
  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T) &&
			  hb_is_trivially_destructible(T))>
  শূন্য
  set_storage (Type *array, unsigned n)
  {
    প্রতিজ্ঞা (allocated == 0);
    প্রতিজ্ঞা (length == 0);

    arrayZ = array;
    length = n;
  }

  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  শূন্য extend (ধ্রুব Iterable &o, তার্কিক exact=অসত্য)
  {
    auto iter = hb_iter (o);
    যদি (iter.is_random_access_iterator || iter.has_fast_len)
    {
      যদি (unlikely (!alloc (length + hb_len (iter), exact)))
	প্রত্যাগম;
      unsigned গুনতি = hb_len (iter);
      প্রতি (unsigned i = 0; i < গুনতি; i++)
	ঠেলো_has_room (*iter++);
    }
    যাবত্ (iter)
    {
      যদি (unlikely (!alloc (length + 1)))
        প্রত্যাগম;
      unsigned room = allocated - length;
      প্রতি (unsigned i = 0; i < room && iter; i++)
	ঠেলো_has_room (*iter++);
    }
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  শূন্য extend (array_t o, তার্কিক exact=অসত্য)
  { extend (c_array_t (o), exact); }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  শূন্য extend (c_array_t o, তার্কিক exact=অসত্য)
  {
    /* o may point into our own storage, eg. v.extend (v.as_array ()).
     * alloc() may move that storage, so find o again afterwards. */
    তার্কিক aliased = o.length && in_storage (*o.arrayZ);
    unsigned offset = aliased ? o.arrayZ - arrayZ : 0;
    alloc (length + o.length, exact);
    যদি (unlikely (in_error ())) প্রত্যাগম;
    যদি (unlikely (aliased))
      o = {arrayZ + offset, o.length};
    copy_array (o);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  শূন্য extend (ধ্রুব hb_vector_t &o, তার্কিক exact=অসত্য)
  { extend (o.as_array (), exact); }

  সার্বজনিক:
  পূর্ণাঙ্ক allocated = 0; /* < 0 means allocation failed. */
  unsigned পূর্ণাঙ্ক length = 0;
  সার্বজনিক:
  Type *arrayZ = nullptr;

  শূন্য init ()
  {
    allocated = length = 0;
    arrayZ = nullptr;
  }
  শূন্য init0 ()
  {
  }

  শূন্য fini ()
  {
    যদি (is_owned ())
    {
      shrink_vector (0);
      hb_free (arrayZ);
    }
    init ();
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t &reset ()
  {
    যদি (unlikely (in_error ()))
      reset_error ();
    clear ();
    প্রত্যাগম *এতত্;
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t &reset_if_error ()
  {
    যদি (unlikely (in_error ()))
      reset ();
    প্রত্যাগম *এতত্;
  }

  /* Transfer ownership of the backing storage to caller.
   * Returns nullptr if storage is not owned by this vector. */
  Type *
  steal (unsigned *len = nullptr, পূর্ণাঙ্ক *allocated_out = nullptr)
  {
    যদি (!is_owned ())
      প্রত্যাগম nullptr;
    যদি (len)
      *len = length;
    যদি (allocated_out)
      *allocated_out = allocated;
    Type *p = arrayZ;
    init ();
    প্রত্যাগম p;
  }

  /* Adopt a previously detached owned buffer. */
  শূন্য
  recycle_buffer (Type *buffer,
                  unsigned len,
                  পূর্ণাঙ্ক allocated_len)
  {
    fini ();
    arrayZ = buffer;
    length = len;
    allocated = allocated_len;
  }

  friend শূন্য swap (hb_vector_t& a, hb_vector_t& b) noexcept
  {
    hb_swap (a.allocated, b.allocated);
    hb_swap (a.length, b.length);
    hb_swap (a.arrayZ, b.arrayZ);
  }

  hb_vector_t& operator = (ধ্রুব hb_vector_t &o)
  {
    যদি (এতত্ == &o) প্রত্যাগম *এতত্;

    reset ();
    alloc_exact (o.length);
    যদি (unlikely (in_error ())) প্রত্যাগম *এতত্;

    length = 0;
    copy_array (o.as_array ());

    প্রত্যাগম *এতত্;
  }
  hb_vector_t& operator = (hb_vector_t &&o) noexcept
  {
    hb_swap (*এতত্, o);
    প্রত্যাগম *এতত্;
  }

  hb_bytes_t as_bytes () ধ্রুব
  { প্রত্যাগম hb_bytes_t ((ধ্রুব বর্ণ *) arrayZ, get_size ()); }

  তার্কিক operator == (ধ্রুব hb_vector_t &o) ধ্রুব { প্রত্যাগম as_array () == o.as_array (); }
  তার্কিক operator != (ধ্রুব hb_vector_t &o) ধ্রুব { প্রত্যাগম !(*এতত্ == o); }
  uint32_t hash () ধ্রুব { প্রত্যাগম as_array ().hash (); }

  Type& operator [] (পূর্ণাঙ্ক i_)
  {
    unsigned পূর্ণাঙ্ক i = (unsigned পূর্ণাঙ্ক) i_;
    যদি (unlikely (i >= length))
      প্রত্যাগম Crap (Type);
    প্রত্যাগম arrayZ[i];
  }
  ধ্রুব Type& operator [] (পূর্ণাঙ্ক i_) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক i = (unsigned পূর্ণাঙ্ক) i_;
    যদি (unlikely (i >= length))
      প্রত্যাগম Null (Type);
    প্রত্যাগম arrayZ[i];
  }

  Type& tail () { প্রত্যাগম (*এতত্)[length - 1]; }
  ধ্রুব Type& tail () ধ্রুব { প্রত্যাগম (*এতত্)[length - 1]; }

  explicit operator তার্কিক () ধ্রুব { প্রত্যাগম length; }
  size_t get_size () ধ্রুব { প্রত্যাগম hb_unsigned_mul_saturate (length, item_size); }

  /* Sink interface. */
  template <typename T>
  hb_vector_t& operator << (T&& v) { push (std::forward<T> (v)); প্রত্যাগম *এতত্; }

  array_t   as_array ()       { প্রত্যাগম hb_array (arrayZ, length); }
  c_array_t as_array () ধ্রুব { প্রত্যাগম hb_array (arrayZ, length); }

  /* Iterator. */
  প্রকারনির্দেশ c_array_t   iter_t;
  প্রকারনির্দেশ array_t   writer_t;
    iter_t   iter () ধ্রুব { প্রত্যাগম as_array (); }
  writer_t writer ()       { প্রত্যাগম as_array (); }
  operator   iter_t () ধ্রুব { প্রত্যাগম   iter (); }
  operator writer_t ()       { প্রত্যাগম writer (); }

  /* Faster range-based for loop. */
  Type *begin () ধ্রুব { প্রত্যাগম arrayZ; }
  Type *end () ধ্রুব { প্রত্যাগম arrayZ + length; }


  hb_sorted_array_t<Type> as_sorted_array ()
  { প্রত্যাগম hb_sorted_array (arrayZ, length); }
  hb_sorted_array_t<ধ্রুব Type> as_sorted_array () ধ্রুব
  { প্রত্যাগম hb_sorted_array (arrayZ, length); }

  template <typename T> explicit operator T * () { প্রত্যাগম arrayZ; }
  template <typename T> explicit operator ধ্রুব T * () ধ্রুব { প্রত্যাগম arrayZ; }

  Type * operator  + (unsigned পূর্ণাঙ্ক i) { প্রত্যাগম arrayZ + i; }
  ধ্রুব Type * operator  + (unsigned পূর্ণাঙ্ক i) ধ্রুব { প্রত্যাগম arrayZ + i; }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push ()
  {
    যদি (unlikely (!resize (length + 1)))
      প্রত্যাগম std::addressof (Crap (Type));
    প্রত্যাগম std::addressof (arrayZ[length - 1]);
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push (Args&&... args)
  {
    যদি (unlikely ((পূর্ণাঙ্ক) length >= allocated))
    {
      যদি (unlikely (in_error ()))
	প্রত্যাগম std::addressof (Crap (Type));

      /* An argument may live in our own storage, eg. v.push (v[i]).
       * alloc() frees that storage, so copy it out first. */
      যদি (unlikely (in_storage (args...)))
      {
	Type copy (std::forward<Args> (args)...);
	যদি (unlikely (!alloc (length + 1)))
	  প্রত্যাগম std::addressof (Crap (Type));
	প্রত্যাগম ঠেলো_has_room (std::move (copy));
      }
      যদি (unlikely (!alloc (length + 1)))
	// If push failed to allocate then don't copy v, since this may cause
	// the created copy to leak memory since we won't have stored a
	// reference to it.
	প্রত্যাগম std::addressof (Crap (Type));
    }

    প্রত্যাগম ঠেলো_has_room (std::forward<Args> (args)...);
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  তার্কিক ঠেলো_or_fail (Args&&... args)
  {
    প্রত্যাগম push (std::forward<Args> (args)...) != std::addressof (Crap (Type));
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *ঠেলো_has_room (Args&&... args)
  {
    /* Emplace. */
    Type *p = std::addressof (arrayZ[length++]);
    প্রত্যাগম নূতন (p) Type (std::forward<Args> (args)...);
  }

  তার্কিক in_storage () ধ্রুব { প্রত্যাগম অসত্য; }
  template <typename T, typename... Ts>
  তার্কিক in_storage (ধ্রুব T &v, ধ্রুব Ts &...vs) ধ্রুব
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    প্রত্যাগম (arrayZ &&
	    (uintptr_t) arrayZ <= p && p < (uintptr_t) (arrayZ + length)) ||
	   in_storage (vs...);
  }

  তার্কিক is_owned () ধ্রুব
  {
    প্রত্যাগম allocated != 0 && allocated != -1;
  }

  তার্কিক in_error () ধ্রুব { প্রত্যাগম allocated < 0; }
  শূন্য set_error ()
  {
    প্রতিজ্ঞা (allocated >= 0);
    allocated = -allocated - 1;
  }
  শূন্য reset_error ()
  {
    প্রতিজ্ঞা (allocated < 0);
    allocated = -(allocated + 1);
  }
  শূন্য ensure_error ()
  {
    যদি (!in_error ())
      set_error ();
  }

  Type *
  _realloc (unsigned new_allocated)
  {
    যদি (!new_allocated)
    {
      যদি (is_owned ())
	hb_free (arrayZ);
      প্রত্যাগম nullptr;
    }
    যদি (!allocated && arrayZ)
    {
      /* If we have a non-null arrayZ but allocated is 0, then we are
       * reallocating from a foreign array. */
      Type *new_array = (Type *) hb_malloc (new_allocated * পরিমাণ (Type));
      যদি (unlikely (!new_array))
	প্রত্যাগম nullptr;
      hb_memcpy ((শূন্য *) new_array, (ধ্রুব শূন্য *) arrayZ, length * পরিমাণ (Type));
      প্রত্যাগম new_array;
    }
    প্রত্যাগম (Type *) hb_realloc (arrayZ, new_allocated * পরিমাণ (Type));
  }
  Type *
  _malloc_move (unsigned new_allocated)
  {
    যদি (!new_allocated)
    {
      যদি (is_owned ())
	hb_free (arrayZ);
      প্রত্যাগম nullptr;
    }
    Type *new_array = (Type *) hb_malloc (new_allocated * পরিমাণ (Type));
    যদি (likely (new_array))
    {
      প্রতি (unsigned i = 0; i < length; i++)
      {
	নূতন (std::addressof (new_array[i])) Type ();
	new_array[i] = std::move (arrayZ[i]);
	arrayZ[i].~Type ();
      }
      যদি (is_owned ())
	hb_free (arrayZ);
    }
    প্রত্যাগম new_array;
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<0>)
  {
    প্রত্যাগম _realloc (new_allocated);
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<0>)
  {
    প্রত্যাগম _malloc_move (new_allocated);
  }
  /* Specialization for types that can be moved using realloc(). */
  template <typename T = Type,
	    hb_enable_if (T::realloc_move)>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<1>)
  {
    প্রত্যাগম _realloc (new_allocated);
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T))>
  শূন্য
  grow_vector (unsigned size, hb_priority<0>)
  {
    hb_memset (arrayZ + length, 0, (size - length) * পরিমাণ (*arrayZ));
    length = size;
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_constructible(T))>
  শূন্য
  grow_vector (unsigned size, hb_priority<0>)
  {
    প্রতি (; length < size; length++)
      নূতন (std::addressof (arrayZ[length])) Type ();
  }
  /* Specialization for hb_vector_t<hb_{vector,array}_t<U>> to speed up. */
  template <typename T = Type,
	    hb_enable_if (hb_is_same (T, hb_vector_t<typename T::item_t>) ||
			  hb_is_same (T, hb_array_t <typename T::item_t>))>
  শূন্য
  grow_vector (unsigned size, hb_priority<1>)
  {
    hb_memset (arrayZ + length, 0, (size - length) * পরিমাণ (*arrayZ));
    length = size;
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  শূন্য
  copy_array (hb_array_t<Type> other)
  {
    hb_memcpy ((শূন্য *) (arrayZ + length), (ধ্রুব শূন্য *) other.arrayZ, other.length * item_size);
    length += other.length;
  }
  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  শূন্য
  copy_array (hb_array_t<ধ্রুব Type> other)
  {
    hb_memcpy ((শূন্য *) (arrayZ + length), (ধ্রুব শূন্য *) other.arrayZ, other.length * item_size);
    length += other.length;
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			   std::is_copy_constructible<T>::value)>
  শূন্য
  copy_array (hb_array_t<ধ্রুব Type> other)
  {
    প্রতি (unsigned i = 0; i < other.length; i++)
      নূতন (std::addressof (arrayZ[length + i])) Type (other.arrayZ[i]);
    length += other.length;
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			  !std::is_copy_constructible<T>::value &&
			  std::is_default_constructible<T>::value &&
			  std::is_copy_assignable<T>::value)>
  শূন্য
  copy_array (hb_array_t<ধ্রুব Type> other)
  {
    প্রতি (unsigned i = 0; i < other.length; i++)
    {
      নূতন (std::addressof (arrayZ[length + i])) Type ();
      arrayZ[length + i] = other.arrayZ[i];
    }
    length += other.length;
  }

  শূন্য
  shrink_vector (unsigned size)
  {
    প্রতিজ্ঞা (size <= length);
    যদি (!hb_is_trivially_destructible(Type))
    {
      unsigned গুনতি = length - size;
      Type *p = arrayZ + length;
      যাবত্ (গুনতি--)
        (--p)->~Type ();
    }
    length = size;
  }

  শূন্য
  shift_down_vector (unsigned i)
  {
    প্রতি (; i < length; i++)
      arrayZ[i - 1] = std::move (arrayZ[i]);
  }

  /* Allocate for size but don't adjust length. */
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  তার্কিক alloc (unsigned পূর্ণাঙ্ক size, তার্কিক exact=অসত্য)
  {
    যদি (unlikely (in_error ()))
      প্রত্যাগম অসত্য;

    unsigned পূর্ণাঙ্ক new_allocated;
    যদি (exact)
    {
      /* If exact was specified, we allow shrinking the storage. */
      size = hb_max (size, length);
      যদি (size <= (unsigned) allocated &&
	  size >= (unsigned) allocated >> 2)
	প্রত্যাগম সত্য;

      new_allocated = size;
    }
    অন্যথা
    {
      যদি (likely (size <= (unsigned) allocated))
	প্রত্যাগম সত্য;

      new_allocated = allocated;
      যাবত্ (size > new_allocated)
	new_allocated += (new_allocated >> 1) + 8;
    }

    /* Reallocate */

    তার্কিক overflows =
      (পূর্ণাঙ্ক) in_error () ||
      (new_allocated < size) ||
      hb_unsigned_mul_overflows (new_allocated, পরিমাণ (Type));

    যদি (unlikely (overflows))
    {
      set_error ();
      প্রত্যাগম অসত্য;
    }

    Type *new_array = realloc_vector (new_allocated, hb_prioritize);

    যদি (unlikely (new_allocated && !new_array))
    {
      যদি (new_allocated <= (unsigned) allocated)
        প্রত্যাগম সত্য; // shrinking failed; it's okay; happens in our fuzzer

      set_error ();
      প্রত্যাগম অসত্য;
    }

    arrayZ = new_array;
    allocated = new_allocated;

    প্রত্যাগম সত্য;
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  তার্কিক alloc_exact (unsigned পূর্ণাঙ্ক size)
  {
    প্রত্যাগম alloc (size, সত্য);
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  শূন্য clear ()
  {
    shrink_vector (0);
  }

  template <typename allocator_t>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  তার্কিক allocate_from_pool (allocator_t *allocator, unsigned size, unsigned পূর্ণাঙ্ক initialize = সত্য)
  {
    যদি (allocator)
    {
      প্রতিজ্ঞা (!length && !allocated);
      arrayZ = (Type *) allocator->alloc (size * পরিমাণ (Type), alignof (Type));
      যদি (unlikely (!arrayZ))
      {
	set_error ();
	প্রত্যাগম অসত্য;
      }
      যদি (initialize)
	grow_vector (size, hb_prioritize);
      অন্যথা
	length = size;
      প্রত্যাগম সত্য;
    }
    প্রত্যাগম resize_full ((পূর্ণাঙ্ক) size, initialize, সত্য);
  }

  template <typename allocator_t>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  তার্কিক duplicate_vector_from_pool (allocator_t *allocator, ধ্রুব hb_vector_t &other)
  {
    যদি (unlikely (!allocate_from_pool (allocator, other.length, অসত্য)))
      প্রত্যাগম অসত্য;
    length = 0;
    copy_array (other.as_array ());
    প্রত্যাগম সত্য;
  }

  template <typename allocator_t>
  শূন্য shrink_back_to_pool (allocator_t *allocator, পূর্ণাঙ্ক size)
  {
    unsigned orig_length = length;

    shrink (size, অসত্য);

    যদি (allocator && !is_owned ())
      allocator->discard (arrayZ + length, (orig_length - length) * পরিমাণ (Type));
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  তার্কিক resize_full (পূর্ণাঙ্ক size_, তার্কিক initialize, তার্কিক exact)
  {
    যদি (unlikely (size_ < 0))
      প্রত্যাগম অসত্য;
    unsigned পূর্ণাঙ্ক size = (unsigned পূর্ণাঙ্ক) size_;
    যদি (!alloc (size, exact))
      প্রত্যাগম অসত্য;

    যদি (size > length)
    {
      যদি (initialize)
	grow_vector (size, hb_prioritize);
    }
    অন্যথা যদি (size < length)
    {
      যদি (initialize)
	shrink_vector (size);
    }

    length = size;
    প্রত্যাগম সত্য;
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  তার্কিক resize (পূর্ণাঙ্ক size_)
  {
    প্রত্যাগম resize_full (size_, সত্য, অসত্য);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  তার্কিক resize_dirty (পূর্ণাঙ্ক size_)
  {
    প্রত্যাগম resize_full (size_, অসত্য, অসত্য);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  তার্কিক resize_exact (পূর্ণাঙ্ক size_)
  {
    প্রত্যাগম resize_full (size_, সত্য, সত্য);
  }

  Type বের_করো ()
  {
    যদি (!length) প্রত্যাগম Null (Type);
    Type v (std::move (arrayZ[length - 1]));
    arrayZ[length - 1].~Type ();
    length--;
    প্রত্যাগম v;
  }

  শূন্য remove_ordered (unsigned পূর্ণাঙ্ক i)
  {
    যদি (unlikely (i >= length))
      প্রত্যাগম;
    shift_down_vector (i + 1);
    arrayZ[length - 1].~Type ();
    length--;
  }

  template <তার্কিক Sorted = sorted,
	    hb_enable_if (!Sorted)>
  শূন্য remove_unordered (unsigned পূর্ণাঙ্ক i)
  {
    যদি (unlikely (i >= length))
      প্রত্যাগম;
    যদি (i != length - 1)
      arrayZ[i] = std::move (arrayZ[length - 1]);
    arrayZ[length - 1].~Type ();
    length--;
  }

  শূন্য shrink (পূর্ণাঙ্ক size_, তার্কিক shrink_memory = সত্য)
  {
    unsigned পূর্ণাঙ্ক size = size_ < 0 ? 0u : (unsigned পূর্ণাঙ্ক) size_;
    যদি (size >= length)
      প্রত্যাগম;

    shrink_vector (size);

    যদি (is_owned () && shrink_memory)
      alloc_exact (size); /* To force shrinking memory if needed. */
  }


  /* Sorting API. */
  template <typename Compar>
  শূন্য qsort (Compar compar)
  { as_array ().qsort (compar); }
  শূন্য qsort ()
  { as_array ().qsort (); }

  /* Unsorted search API. */
  template <typename T>
  Type *lsearch (ধ্রুব T &x, Type *not_found = nullptr)
  { প্রত্যাগম as_array ().lsearch (x, not_found); }
  template <typename T>
  ধ্রুব Type *lsearch (ধ্রুব T &x, ধ্রুব Type *not_found = nullptr) ধ্রুব
  { প্রত্যাগম as_array ().lsearch (x, not_found); }
  template <typename T>
  তার্কিক lfind (ধ্রুব T &x, unsigned *pos = nullptr) ধ্রুব
  { প্রত্যাগম as_array ().lfind (x, pos); }

  /* Sorted search API. */
  template <typename T,
	    তার্কিক Sorted=sorted, hb_enable_if (Sorted)>
  Type *bsearch (ধ্রুব T &x, Type *not_found = nullptr)
  { প্রত্যাগম as_array ().bsearch (x, not_found); }
  template <typename T,
	    তার্কিক Sorted=sorted, hb_enable_if (Sorted)>
  ধ্রুব Type *bsearch (ধ্রুব T &x, ধ্রুব Type *not_found = nullptr) ধ্রুব
  { প্রত্যাগম as_array ().bsearch (x, not_found); }
  template <typename T,
	    তার্কিক Sorted=sorted, hb_enable_if (Sorted)>
  তার্কিক bfind (ধ্রুব T &x, unsigned পূর্ণাঙ্ক *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned পূর্ণাঙ্ক to_store = (unsigned পূর্ণাঙ্ক) -1) ধ্রুব
  { প্রত্যাগম as_array ().bfind (x, i, not_found, to_store); }
};

template <typename Type>
using hb_sorted_vector_t = hb_vector_t<Type, সত্য>;

#endif /* HB_VECTOR_HH */

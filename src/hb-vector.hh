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

#அந்தர்பாவ "hb.hh"
#அந்தர்பாவ "hb-array.hh"
#அந்தர்பாவ "hb-meta.hh"
#அந்தர்பாவ "hb-null.hh"

// Change to 1 to force inline vector allocs, to see callsite in malloc-stats tool.
#எனில் 0
#define HB_ALWAYS_INLINE_VECTOR_ALLOCS HB_ALWAYS_INLINE
#இல்லையெனில்
#define HB_ALWAYS_INLINE_VECTOR_ALLOCS
#endif

template <typename Type,
	  தருக்க sorted=பொய்>
கட்டமைப்பு hb_vector_t
{
  நிலையான constexpr தருக்க realloc_move = உண்மை;

  வகைவரை Type item_t;
  நிலையான constexpr unsigned item_size = hb_static_size (Type);
  using array_t = typename std::conditional<sorted, hb_sorted_array_t<Type>, hb_array_t<Type>>::type;
  using c_array_t = typename std::conditional<sorted, hb_sorted_array_t<மாறிலி Type>, hb_array_t<மாறிலி Type>>::type;

  hb_vector_t () = இயல்பு;
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (std::initializer_list<Type> lst) : hb_vector_t ()
  {
    alloc (lst.size (), உண்மை);
    க்கு (auto&& item : lst)
      push (item);
  }
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  explicit hb_vector_t (மாறிலி Iterable &o) : hb_vector_t ()
  {
    extend (o, உண்மை);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (மாறிலி hb_vector_t &o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    எனில் (unlikely (in_error ())) திரும்பு;
    copy_array (o.as_array ());
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (array_t o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    எனில் (unlikely (in_error ())) திரும்பு;
    copy_array (o);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (c_array_t o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    எனில் (unlikely (in_error ())) திரும்பு;
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
  வெறுமை
  set_storage (Type (&array)[n])
  { set_storage (array, n); }
  வெறுமை
  set_storage (hb_array_t<Type> array)
  { set_storage (array.arrayZ, array.length); }
  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T) &&
			  hb_is_trivially_destructible(T))>
  வெறுமை
  set_storage (Type *array, unsigned n)
  {
    ப்ரதிஜ்ஞா (allocated == 0);
    ப்ரதிஜ்ஞா (length == 0);

    arrayZ = array;
    length = n;
  }

  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  வெறுமை extend (மாறிலி Iterable &o, தருக்க exact=பொய்)
  {
    auto iter = hb_iter (o);
    எனில் (iter.is_random_access_iterator || iter.has_fast_len)
    {
      எனில் (unlikely (!alloc (length + hb_len (iter), exact)))
	திரும்பு;
      unsigned மொத்தம் = hb_len (iter);
      க்கு (unsigned i = 0; i < மொத்தம்; i++)
	தள்ளு_has_room (*iter++);
    }
    போது (iter)
    {
      எனில் (unlikely (!alloc (length + 1)))
        திரும்பு;
      unsigned room = allocated - length;
      க்கு (unsigned i = 0; i < room && iter; i++)
	தள்ளு_has_room (*iter++);
    }
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  வெறுமை extend (array_t o, தருக்க exact=பொய்)
  { extend (c_array_t (o), exact); }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  வெறுமை extend (c_array_t o, தருக்க exact=பொய்)
  {
    /* o may point into our own storage, eg. v.extend (v.as_array ()).
     * alloc() may move that storage, so find o again afterwards. */
    தருக்க aliased = o.length && in_storage (*o.arrayZ);
    unsigned offset = aliased ? o.arrayZ - arrayZ : 0;
    alloc (length + o.length, exact);
    எனில் (unlikely (in_error ())) திரும்பு;
    எனில் (unlikely (aliased))
      o = {arrayZ + offset, o.length};
    copy_array (o);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  வெறுமை extend (மாறிலி hb_vector_t &o, தருக்க exact=பொய்)
  { extend (o.as_array (), exact); }

  பொது:
  முழு allocated = 0; /* < 0 means allocation failed. */
  unsigned முழு length = 0;
  பொது:
  Type *arrayZ = nullptr;

  வெறுமை init ()
  {
    allocated = length = 0;
    arrayZ = nullptr;
  }
  வெறுமை init0 ()
  {
  }

  வெறுமை fini ()
  {
    எனில் (is_owned ())
    {
      shrink_vector (0);
      hb_free (arrayZ);
    }
    init ();
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t &reset ()
  {
    எனில் (unlikely (in_error ()))
      reset_error ();
    clear ();
    திரும்பு *இது;
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t &reset_if_error ()
  {
    எனில் (unlikely (in_error ()))
      reset ();
    திரும்பு *இது;
  }

  /* Transfer ownership of the backing storage to caller.
   * Returns nullptr if storage is not owned by this vector. */
  Type *
  steal (unsigned *len = nullptr, முழு *allocated_out = nullptr)
  {
    எனில் (!is_owned ())
      திரும்பு nullptr;
    எனில் (len)
      *len = length;
    எனில் (allocated_out)
      *allocated_out = allocated;
    Type *p = arrayZ;
    init ();
    திரும்பு p;
  }

  /* Adopt a previously detached owned buffer. */
  வெறுமை
  recycle_buffer (Type *buffer,
                  unsigned len,
                  முழு allocated_len)
  {
    fini ();
    arrayZ = buffer;
    length = len;
    allocated = allocated_len;
  }

  friend வெறுமை swap (hb_vector_t& a, hb_vector_t& b) noexcept
  {
    hb_swap (a.allocated, b.allocated);
    hb_swap (a.length, b.length);
    hb_swap (a.arrayZ, b.arrayZ);
  }

  hb_vector_t& operator = (மாறிலி hb_vector_t &o)
  {
    எனில் (இது == &o) திரும்பு *இது;

    reset ();
    alloc_exact (o.length);
    எனில் (unlikely (in_error ())) திரும்பு *இது;

    length = 0;
    copy_array (o.as_array ());

    திரும்பு *இது;
  }
  hb_vector_t& operator = (hb_vector_t &&o) noexcept
  {
    hb_swap (*இது, o);
    திரும்பு *இது;
  }

  hb_bytes_t as_bytes () மாறிலி
  { திரும்பு hb_bytes_t ((மாறிலி எழுத்து *) arrayZ, get_size ()); }

  தருக்க operator == (மாறிலி hb_vector_t &o) மாறிலி { திரும்பு as_array () == o.as_array (); }
  தருக்க operator != (மாறிலி hb_vector_t &o) மாறிலி { திரும்பு !(*இது == o); }
  uint32_t hash () மாறிலி { திரும்பு as_array ().hash (); }

  Type& operator [] (முழு i_)
  {
    unsigned முழு i = (unsigned முழு) i_;
    எனில் (unlikely (i >= length))
      திரும்பு Crap (Type);
    திரும்பு arrayZ[i];
  }
  மாறிலி Type& operator [] (முழு i_) மாறிலி
  {
    unsigned முழு i = (unsigned முழு) i_;
    எனில் (unlikely (i >= length))
      திரும்பு Null (Type);
    திரும்பு arrayZ[i];
  }

  Type& tail () { திரும்பு (*இது)[length - 1]; }
  மாறிலி Type& tail () மாறிலி { திரும்பு (*இது)[length - 1]; }

  explicit operator தருக்க () மாறிலி { திரும்பு length; }
  size_t get_size () மாறிலி { திரும்பு hb_unsigned_mul_saturate (length, item_size); }

  /* Sink interface. */
  template <typename T>
  hb_vector_t& operator << (T&& v) { push (std::forward<T> (v)); திரும்பு *இது; }

  array_t   as_array ()       { திரும்பு hb_array (arrayZ, length); }
  c_array_t as_array () மாறிலி { திரும்பு hb_array (arrayZ, length); }

  /* Iterator. */
  வகைவரை c_array_t   iter_t;
  வகைவரை array_t   writer_t;
    iter_t   iter () மாறிலி { திரும்பு as_array (); }
  writer_t writer ()       { திரும்பு as_array (); }
  operator   iter_t () மாறிலி { திரும்பு   iter (); }
  operator writer_t ()       { திரும்பு writer (); }

  /* Faster range-based for loop. */
  Type *begin () மாறிலி { திரும்பு arrayZ; }
  Type *end () மாறிலி { திரும்பு arrayZ + length; }


  hb_sorted_array_t<Type> as_sorted_array ()
  { திரும்பு hb_sorted_array (arrayZ, length); }
  hb_sorted_array_t<மாறிலி Type> as_sorted_array () மாறிலி
  { திரும்பு hb_sorted_array (arrayZ, length); }

  template <typename T> explicit operator T * () { திரும்பு arrayZ; }
  template <typename T> explicit operator மாறிலி T * () மாறிலி { திரும்பு arrayZ; }

  Type * operator  + (unsigned முழு i) { திரும்பு arrayZ + i; }
  மாறிலி Type * operator  + (unsigned முழு i) மாறிலி { திரும்பு arrayZ + i; }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push ()
  {
    எனில் (unlikely (!resize (length + 1)))
      திரும்பு std::addressof (Crap (Type));
    திரும்பு std::addressof (arrayZ[length - 1]);
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push (Args&&... args)
  {
    எனில் (unlikely ((முழு) length >= allocated))
    {
      எனில் (unlikely (in_error ()))
	திரும்பு std::addressof (Crap (Type));

      /* An argument may live in our own storage, eg. v.push (v[i]).
       * alloc() frees that storage, so copy it out first. */
      எனில் (unlikely (in_storage (args...)))
      {
	Type copy (std::forward<Args> (args)...);
	எனில் (unlikely (!alloc (length + 1)))
	  திரும்பு std::addressof (Crap (Type));
	திரும்பு தள்ளு_has_room (std::move (copy));
      }
      எனில் (unlikely (!alloc (length + 1)))
	// If push failed to allocate then don't copy v, since this may cause
	// the created copy to leak memory since we won't have stored a
	// reference to it.
	திரும்பு std::addressof (Crap (Type));
    }

    திரும்பு தள்ளு_has_room (std::forward<Args> (args)...);
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  தருக்க தள்ளு_or_fail (Args&&... args)
  {
    திரும்பு push (std::forward<Args> (args)...) != std::addressof (Crap (Type));
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *தள்ளு_has_room (Args&&... args)
  {
    /* Emplace. */
    Type *p = std::addressof (arrayZ[length++]);
    திரும்பு புதிய (p) Type (std::forward<Args> (args)...);
  }

  தருக்க in_storage () மாறிலி { திரும்பு பொய்; }
  template <typename T, typename... Ts>
  தருக்க in_storage (மாறிலி T &v, மாறிலி Ts &...vs) மாறிலி
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    திரும்பு (arrayZ &&
	    (uintptr_t) arrayZ <= p && p < (uintptr_t) (arrayZ + length)) ||
	   in_storage (vs...);
  }

  தருக்க is_owned () மாறிலி
  {
    திரும்பு allocated != 0 && allocated != -1;
  }

  தருக்க in_error () மாறிலி { திரும்பு allocated < 0; }
  வெறுமை set_error ()
  {
    ப்ரதிஜ்ஞா (allocated >= 0);
    allocated = -allocated - 1;
  }
  வெறுமை reset_error ()
  {
    ப்ரதிஜ்ஞா (allocated < 0);
    allocated = -(allocated + 1);
  }
  வெறுமை ensure_error ()
  {
    எனில் (!in_error ())
      set_error ();
  }

  Type *
  _realloc (unsigned new_allocated)
  {
    எனில் (!new_allocated)
    {
      எனில் (is_owned ())
	hb_free (arrayZ);
      திரும்பு nullptr;
    }
    எனில் (!allocated && arrayZ)
    {
      /* If we have a non-null arrayZ but allocated is 0, then we are
       * reallocating from a foreign array. */
      Type *new_array = (Type *) hb_malloc (new_allocated * அளவு (Type));
      எனில் (unlikely (!new_array))
	திரும்பு nullptr;
      hb_memcpy ((வெறுமை *) new_array, (மாறிலி வெறுமை *) arrayZ, length * அளவு (Type));
      திரும்பு new_array;
    }
    திரும்பு (Type *) hb_realloc (arrayZ, new_allocated * அளவு (Type));
  }
  Type *
  _malloc_move (unsigned new_allocated)
  {
    எனில் (!new_allocated)
    {
      எனில் (is_owned ())
	hb_free (arrayZ);
      திரும்பு nullptr;
    }
    Type *new_array = (Type *) hb_malloc (new_allocated * அளவு (Type));
    எனில் (likely (new_array))
    {
      க்கு (unsigned i = 0; i < length; i++)
      {
	புதிய (std::addressof (new_array[i])) Type ();
	new_array[i] = std::move (arrayZ[i]);
	arrayZ[i].~Type ();
      }
      எனில் (is_owned ())
	hb_free (arrayZ);
    }
    திரும்பு new_array;
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<0>)
  {
    திரும்பு _realloc (new_allocated);
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<0>)
  {
    திரும்பு _malloc_move (new_allocated);
  }
  /* Specialization for types that can be moved using realloc(). */
  template <typename T = Type,
	    hb_enable_if (T::realloc_move)>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<1>)
  {
    திரும்பு _realloc (new_allocated);
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T))>
  வெறுமை
  grow_vector (unsigned size, hb_priority<0>)
  {
    hb_memset (arrayZ + length, 0, (size - length) * அளவு (*arrayZ));
    length = size;
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_constructible(T))>
  வெறுமை
  grow_vector (unsigned size, hb_priority<0>)
  {
    க்கு (; length < size; length++)
      புதிய (std::addressof (arrayZ[length])) Type ();
  }
  /* Specialization for hb_vector_t<hb_{vector,array}_t<U>> to speed up. */
  template <typename T = Type,
	    hb_enable_if (hb_is_same (T, hb_vector_t<typename T::item_t>) ||
			  hb_is_same (T, hb_array_t <typename T::item_t>))>
  வெறுமை
  grow_vector (unsigned size, hb_priority<1>)
  {
    hb_memset (arrayZ + length, 0, (size - length) * அளவு (*arrayZ));
    length = size;
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  வெறுமை
  copy_array (hb_array_t<Type> other)
  {
    hb_memcpy ((வெறுமை *) (arrayZ + length), (மாறிலி வெறுமை *) other.arrayZ, other.length * item_size);
    length += other.length;
  }
  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  வெறுமை
  copy_array (hb_array_t<மாறிலி Type> other)
  {
    hb_memcpy ((வெறுமை *) (arrayZ + length), (மாறிலி வெறுமை *) other.arrayZ, other.length * item_size);
    length += other.length;
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			   std::is_copy_constructible<T>::value)>
  வெறுமை
  copy_array (hb_array_t<மாறிலி Type> other)
  {
    க்கு (unsigned i = 0; i < other.length; i++)
      புதிய (std::addressof (arrayZ[length + i])) Type (other.arrayZ[i]);
    length += other.length;
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			  !std::is_copy_constructible<T>::value &&
			  std::is_default_constructible<T>::value &&
			  std::is_copy_assignable<T>::value)>
  வெறுமை
  copy_array (hb_array_t<மாறிலி Type> other)
  {
    க்கு (unsigned i = 0; i < other.length; i++)
    {
      புதிய (std::addressof (arrayZ[length + i])) Type ();
      arrayZ[length + i] = other.arrayZ[i];
    }
    length += other.length;
  }

  வெறுமை
  shrink_vector (unsigned size)
  {
    ப்ரதிஜ்ஞா (size <= length);
    எனில் (!hb_is_trivially_destructible(Type))
    {
      unsigned மொத்தம் = length - size;
      Type *p = arrayZ + length;
      போது (மொத்தம்--)
        (--p)->~Type ();
    }
    length = size;
  }

  வெறுமை
  shift_down_vector (unsigned i)
  {
    க்கு (; i < length; i++)
      arrayZ[i - 1] = std::move (arrayZ[i]);
  }

  /* Allocate for size but don't adjust length. */
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  தருக்க alloc (unsigned முழு size, தருக்க exact=பொய்)
  {
    எனில் (unlikely (in_error ()))
      திரும்பு பொய்;

    unsigned முழு new_allocated;
    எனில் (exact)
    {
      /* If exact was specified, we allow shrinking the storage. */
      size = hb_max (size, length);
      எனில் (size <= (unsigned) allocated &&
	  size >= (unsigned) allocated >> 2)
	திரும்பு உண்மை;

      new_allocated = size;
    }
    இல்லையெனில்
    {
      எனில் (likely (size <= (unsigned) allocated))
	திரும்பு உண்மை;

      new_allocated = allocated;
      போது (size > new_allocated)
	new_allocated += (new_allocated >> 1) + 8;
    }

    /* Reallocate */

    தருக்க overflows =
      (முழு) in_error () ||
      (new_allocated < size) ||
      hb_unsigned_mul_overflows (new_allocated, அளவு (Type));

    எனில் (unlikely (overflows))
    {
      set_error ();
      திரும்பு பொய்;
    }

    Type *new_array = realloc_vector (new_allocated, hb_prioritize);

    எனில் (unlikely (new_allocated && !new_array))
    {
      எனில் (new_allocated <= (unsigned) allocated)
        திரும்பு உண்மை; // shrinking failed; it's okay; happens in our fuzzer

      set_error ();
      திரும்பு பொய்;
    }

    arrayZ = new_array;
    allocated = new_allocated;

    திரும்பு உண்மை;
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  தருக்க alloc_exact (unsigned முழு size)
  {
    திரும்பு alloc (size, உண்மை);
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  வெறுமை clear ()
  {
    shrink_vector (0);
  }

  template <typename allocator_t>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  தருக்க allocate_from_pool (allocator_t *allocator, unsigned size, unsigned முழு initialize = உண்மை)
  {
    எனில் (allocator)
    {
      ப்ரதிஜ்ஞா (!length && !allocated);
      arrayZ = (Type *) allocator->alloc (size * அளவு (Type), alignof (Type));
      எனில் (unlikely (!arrayZ))
      {
	set_error ();
	திரும்பு பொய்;
      }
      எனில் (initialize)
	grow_vector (size, hb_prioritize);
      இல்லையெனில்
	length = size;
      திரும்பு உண்மை;
    }
    திரும்பு resize_full ((முழு) size, initialize, உண்மை);
  }

  template <typename allocator_t>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  தருக்க duplicate_vector_from_pool (allocator_t *allocator, மாறிலி hb_vector_t &other)
  {
    எனில் (unlikely (!allocate_from_pool (allocator, other.length, பொய்)))
      திரும்பு பொய்;
    length = 0;
    copy_array (other.as_array ());
    திரும்பு உண்மை;
  }

  template <typename allocator_t>
  வெறுமை shrink_back_to_pool (allocator_t *allocator, முழு size)
  {
    unsigned orig_length = length;

    shrink (size, பொய்);

    எனில் (allocator && !is_owned ())
      allocator->discard (arrayZ + length, (orig_length - length) * அளவு (Type));
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  தருக்க resize_full (முழு size_, தருக்க initialize, தருக்க exact)
  {
    எனில் (unlikely (size_ < 0))
      திரும்பு பொய்;
    unsigned முழு size = (unsigned முழு) size_;
    எனில் (!alloc (size, exact))
      திரும்பு பொய்;

    எனில் (size > length)
    {
      எனில் (initialize)
	grow_vector (size, hb_prioritize);
    }
    இல்லையெனில் எனில் (size < length)
    {
      எனில் (initialize)
	shrink_vector (size);
    }

    length = size;
    திரும்பு உண்மை;
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  தருக்க resize (முழு size_)
  {
    திரும்பு resize_full (size_, உண்மை, பொய்);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  தருக்க resize_dirty (முழு size_)
  {
    திரும்பு resize_full (size_, பொய், பொய்);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  தருக்க resize_exact (முழு size_)
  {
    திரும்பு resize_full (size_, உண்மை, உண்மை);
  }

  Type எடு ()
  {
    எனில் (!length) திரும்பு Null (Type);
    Type v (std::move (arrayZ[length - 1]));
    arrayZ[length - 1].~Type ();
    length--;
    திரும்பு v;
  }

  வெறுமை remove_ordered (unsigned முழு i)
  {
    எனில் (unlikely (i >= length))
      திரும்பு;
    shift_down_vector (i + 1);
    arrayZ[length - 1].~Type ();
    length--;
  }

  template <தருக்க Sorted = sorted,
	    hb_enable_if (!Sorted)>
  வெறுமை remove_unordered (unsigned முழு i)
  {
    எனில் (unlikely (i >= length))
      திரும்பு;
    எனில் (i != length - 1)
      arrayZ[i] = std::move (arrayZ[length - 1]);
    arrayZ[length - 1].~Type ();
    length--;
  }

  வெறுமை shrink (முழு size_, தருக்க shrink_memory = உண்மை)
  {
    unsigned முழு size = size_ < 0 ? 0u : (unsigned முழு) size_;
    எனில் (size >= length)
      திரும்பு;

    shrink_vector (size);

    எனில் (is_owned () && shrink_memory)
      alloc_exact (size); /* To force shrinking memory if needed. */
  }


  /* Sorting API. */
  template <typename Compar>
  வெறுமை qsort (Compar compar)
  { as_array ().qsort (compar); }
  வெறுமை qsort ()
  { as_array ().qsort (); }

  /* Unsorted search API. */
  template <typename T>
  Type *lsearch (மாறிலி T &x, Type *not_found = nullptr)
  { திரும்பு as_array ().lsearch (x, not_found); }
  template <typename T>
  மாறிலி Type *lsearch (மாறிலி T &x, மாறிலி Type *not_found = nullptr) மாறிலி
  { திரும்பு as_array ().lsearch (x, not_found); }
  template <typename T>
  தருக்க lfind (மாறிலி T &x, unsigned *pos = nullptr) மாறிலி
  { திரும்பு as_array ().lfind (x, pos); }

  /* Sorted search API. */
  template <typename T,
	    தருக்க Sorted=sorted, hb_enable_if (Sorted)>
  Type *bsearch (மாறிலி T &x, Type *not_found = nullptr)
  { திரும்பு as_array ().bsearch (x, not_found); }
  template <typename T,
	    தருக்க Sorted=sorted, hb_enable_if (Sorted)>
  மாறிலி Type *bsearch (மாறிலி T &x, மாறிலி Type *not_found = nullptr) மாறிலி
  { திரும்பு as_array ().bsearch (x, not_found); }
  template <typename T,
	    தருக்க Sorted=sorted, hb_enable_if (Sorted)>
  தருக்க bfind (மாறிலி T &x, unsigned முழு *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned முழு to_store = (unsigned முழு) -1) மாறிலி
  { திரும்பு as_array ().bfind (x, i, not_found, to_store); }
};

template <typename Type>
using hb_sorted_vector_t = hb_vector_t<Type, உண்மை>;

#endif /* HB_VECTOR_HH */

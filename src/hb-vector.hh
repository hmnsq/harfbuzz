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

#ضمن "hb.hh"
#ضمن "hb-array.hh"
#ضمن "hb-meta.hh"
#ضمن "hb-null.hh"

// Change to 1 to force inline vector allocs, to see callsite in malloc-stats tool.
#ܐܢ 0
#define HB_ALWAYS_INLINE_VECTOR_ALLOCS HB_ALWAYS_INLINE
#ܐܚܪܢܐ
#define HB_ALWAYS_INLINE_VECTOR_ALLOCS
#endif

template <typename Type,
	  منطقي sorted=ܕܓܠܐ>
بنية hb_vector_t
{
  ساكن constexpr منطقي realloc_move = ܫܪܪܐ;

  تعريفنوع Type item_t;
  ساكن constexpr unsigned item_size = hb_static_size (Type);
  using array_t = typename std::conditional<sorted, hb_sorted_array_t<Type>, hb_array_t<Type>>::type;
  using c_array_t = typename std::conditional<sorted, hb_sorted_array_t<ثابت Type>, hb_array_t<ثابت Type>>::type;

  hb_vector_t () = افتراضي;
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (std::initializer_list<Type> lst) : hb_vector_t ()
  {
    alloc (lst.size (), ܫܪܪܐ);
    لكل (auto&& item : lst)
      push (item);
  }
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  explicit hb_vector_t (ثابت Iterable &o) : hb_vector_t ()
  {
    extend (o, ܫܪܪܐ);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (ثابت hb_vector_t &o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    ܐܢ (unlikely (in_error ())) ܦܢܐ;
    copy_array (o.as_array ());
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (array_t o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    ܐܢ (unlikely (in_error ())) ܦܢܐ;
    copy_array (o);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (c_array_t o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    ܐܢ (unlikely (in_error ())) ܦܢܐ;
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
  فراغ
  set_storage (Type (&array)[n])
  { set_storage (array, n); }
  فراغ
  set_storage (hb_array_t<Type> array)
  { set_storage (array.arrayZ, array.length); }
  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T) &&
			  hb_is_trivially_destructible(T))>
  فراغ
  set_storage (Type *array, unsigned n)
  {
    أكد (allocated == 0);
    أكد (length == 0);

    arrayZ = array;
    length = n;
  }

  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  فراغ extend (ثابت Iterable &o, منطقي exact=ܕܓܠܐ)
  {
    auto iter = hb_iter (o);
    ܐܢ (iter.is_random_access_iterator || iter.has_fast_len)
    {
      ܐܢ (unlikely (!alloc (length + hb_len (iter), exact)))
	ܦܢܐ;
      unsigned count = hb_len (iter);
      لكل (unsigned i = 0; i < count; i++)
	push_has_room (*iter++);
    }
    ܟܡܐ (iter)
    {
      ܐܢ (unlikely (!alloc (length + 1)))
        ܦܢܐ;
      unsigned room = allocated - length;
      لكل (unsigned i = 0; i < room && iter; i++)
	push_has_room (*iter++);
    }
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  فراغ extend (array_t o, منطقي exact=ܕܓܠܐ)
  { extend (c_array_t (o), exact); }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  فراغ extend (c_array_t o, منطقي exact=ܕܓܠܐ)
  {
    /* o may point into our own storage, eg. v.extend (v.as_array ()).
     * alloc() may move that storage, so find o again afterwards. */
    منطقي aliased = o.length && in_storage (*o.arrayZ);
    unsigned offset = aliased ? o.arrayZ - arrayZ : 0;
    alloc (length + o.length, exact);
    ܐܢ (unlikely (in_error ())) ܦܢܐ;
    ܐܢ (unlikely (aliased))
      o = {arrayZ + offset, o.length};
    copy_array (o);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  فراغ extend (ثابت hb_vector_t &o, منطقي exact=ܕܓܠܐ)
  { extend (o.as_array (), exact); }

  عام:
  صحيح allocated = 0; /* < 0 means allocation failed. */
  unsigned صحيح length = 0;
  عام:
  Type *arrayZ = nullptr;

  فراغ init ()
  {
    allocated = length = 0;
    arrayZ = nullptr;
  }
  فراغ init0 ()
  {
  }

  فراغ fini ()
  {
    ܐܢ (is_owned ())
    {
      shrink_vector (0);
      hb_free (arrayZ);
    }
    init ();
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t &reset ()
  {
    ܐܢ (unlikely (in_error ()))
      reset_error ();
    clear ();
    ܦܢܐ *هذا;
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t &reset_if_error ()
  {
    ܐܢ (unlikely (in_error ()))
      reset ();
    ܦܢܐ *هذا;
  }

  /* Transfer ownership of the backing storage to caller.
   * Returns nullptr if storage is not owned by this vector. */
  Type *
  steal (unsigned *len = nullptr, صحيح *allocated_out = nullptr)
  {
    ܐܢ (!is_owned ())
      ܦܢܐ nullptr;
    ܐܢ (len)
      *len = length;
    ܐܢ (allocated_out)
      *allocated_out = allocated;
    Type *p = arrayZ;
    init ();
    ܦܢܐ p;
  }

  /* Adopt a previously detached owned buffer. */
  فراغ
  recycle_buffer (Type *buffer,
                  unsigned len,
                  صحيح allocated_len)
  {
    fini ();
    arrayZ = buffer;
    length = len;
    allocated = allocated_len;
  }

  friend فراغ swap (hb_vector_t& a, hb_vector_t& b) noexcept
  {
    hb_swap (a.allocated, b.allocated);
    hb_swap (a.length, b.length);
    hb_swap (a.arrayZ, b.arrayZ);
  }

  hb_vector_t& operator = (ثابت hb_vector_t &o)
  {
    ܐܢ (هذا == &o) ܦܢܐ *هذا;

    reset ();
    alloc_exact (o.length);
    ܐܢ (unlikely (in_error ())) ܦܢܐ *هذا;

    length = 0;
    copy_array (o.as_array ());

    ܦܢܐ *هذا;
  }
  hb_vector_t& operator = (hb_vector_t &&o) noexcept
  {
    hb_swap (*هذا, o);
    ܦܢܐ *هذا;
  }

  hb_bytes_t as_bytes () ثابت
  { ܦܢܐ hb_bytes_t ((ثابت حرف *) arrayZ, get_size ()); }

  منطقي operator == (ثابت hb_vector_t &o) ثابت { ܦܢܐ as_array () == o.as_array (); }
  منطقي operator != (ثابت hb_vector_t &o) ثابت { ܦܢܐ !(*هذا == o); }
  uint32_t hash () ثابت { ܦܢܐ as_array ().hash (); }

  Type& operator [] (صحيح i_)
  {
    unsigned صحيح i = (unsigned صحيح) i_;
    ܐܢ (unlikely (i >= length))
      ܦܢܐ Crap (Type);
    ܦܢܐ arrayZ[i];
  }
  ثابت Type& operator [] (صحيح i_) ثابت
  {
    unsigned صحيح i = (unsigned صحيح) i_;
    ܐܢ (unlikely (i >= length))
      ܦܢܐ Null (Type);
    ܦܢܐ arrayZ[i];
  }

  Type& tail () { ܦܢܐ (*هذا)[length - 1]; }
  ثابت Type& tail () ثابت { ܦܢܐ (*هذا)[length - 1]; }

  explicit operator منطقي () ثابت { ܦܢܐ length; }
  size_t get_size () ثابت { ܦܢܐ hb_unsigned_mul_saturate (length, item_size); }

  /* Sink interface. */
  template <typename T>
  hb_vector_t& operator << (T&& v) { push (std::forward<T> (v)); ܦܢܐ *هذا; }

  array_t   as_array ()       { ܦܢܐ hb_array (arrayZ, length); }
  c_array_t as_array () ثابت { ܦܢܐ hb_array (arrayZ, length); }

  /* Iterator. */
  تعريفنوع c_array_t   iter_t;
  تعريفنوع array_t   writer_t;
    iter_t   iter () ثابت { ܦܢܐ as_array (); }
  writer_t writer ()       { ܦܢܐ as_array (); }
  operator   iter_t () ثابت { ܦܢܐ   iter (); }
  operator writer_t ()       { ܦܢܐ writer (); }

  /* Faster range-based for loop. */
  Type *begin () ثابت { ܦܢܐ arrayZ; }
  Type *end () ثابت { ܦܢܐ arrayZ + length; }


  hb_sorted_array_t<Type> as_sorted_array ()
  { ܦܢܐ hb_sorted_array (arrayZ, length); }
  hb_sorted_array_t<ثابت Type> as_sorted_array () ثابت
  { ܦܢܐ hb_sorted_array (arrayZ, length); }

  template <typename T> explicit operator T * () { ܦܢܐ arrayZ; }
  template <typename T> explicit operator ثابت T * () ثابت { ܦܢܐ arrayZ; }

  Type * operator  + (unsigned صحيح i) { ܦܢܐ arrayZ + i; }
  ثابت Type * operator  + (unsigned صحيح i) ثابت { ܦܢܐ arrayZ + i; }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push ()
  {
    ܐܢ (unlikely (!resize (length + 1)))
      ܦܢܐ std::addressof (Crap (Type));
    ܦܢܐ std::addressof (arrayZ[length - 1]);
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push (Args&&... args)
  {
    ܐܢ (unlikely ((صحيح) length >= allocated))
    {
      ܐܢ (unlikely (in_error ()))
	ܦܢܐ std::addressof (Crap (Type));

      /* An argument may live in our own storage, eg. v.push (v[i]).
       * alloc() frees that storage, so copy it out first. */
      ܐܢ (unlikely (in_storage (args...)))
      {
	Type copy (std::forward<Args> (args)...);
	ܐܢ (unlikely (!alloc (length + 1)))
	  ܦܢܐ std::addressof (Crap (Type));
	ܦܢܐ push_has_room (std::move (copy));
      }
      ܐܢ (unlikely (!alloc (length + 1)))
	// If push failed to allocate then don't copy v, since this may cause
	// the created copy to leak memory since we won't have stored a
	// reference to it.
	ܦܢܐ std::addressof (Crap (Type));
    }

    ܦܢܐ push_has_room (std::forward<Args> (args)...);
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  منطقي push_or_fail (Args&&... args)
  {
    ܦܢܐ push (std::forward<Args> (args)...) != std::addressof (Crap (Type));
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push_has_room (Args&&... args)
  {
    /* Emplace. */
    Type *p = std::addressof (arrayZ[length++]);
    ܦܢܐ جديد (p) Type (std::forward<Args> (args)...);
  }

  منطقي in_storage () ثابت { ܦܢܐ ܕܓܠܐ; }
  template <typename T, typename... Ts>
  منطقي in_storage (ثابت T &v, ثابت Ts &...vs) ثابت
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    ܦܢܐ (arrayZ &&
	    (uintptr_t) arrayZ <= p && p < (uintptr_t) (arrayZ + length)) ||
	   in_storage (vs...);
  }

  منطقي is_owned () ثابت
  {
    ܦܢܐ allocated != 0 && allocated != -1;
  }

  منطقي in_error () ثابت { ܦܢܐ allocated < 0; }
  فراغ set_error ()
  {
    أكد (allocated >= 0);
    allocated = -allocated - 1;
  }
  فراغ reset_error ()
  {
    أكد (allocated < 0);
    allocated = -(allocated + 1);
  }
  فراغ ensure_error ()
  {
    ܐܢ (!in_error ())
      set_error ();
  }

  Type *
  _realloc (unsigned new_allocated)
  {
    ܐܢ (!new_allocated)
    {
      ܐܢ (is_owned ())
	hb_free (arrayZ);
      ܦܢܐ nullptr;
    }
    ܐܢ (!allocated && arrayZ)
    {
      /* If we have a non-null arrayZ but allocated is 0, then we are
       * reallocating from a foreign array. */
      Type *new_array = (Type *) hb_malloc (new_allocated * حجم (Type));
      ܐܢ (unlikely (!new_array))
	ܦܢܐ nullptr;
      hb_memcpy ((فراغ *) new_array, (ثابت فراغ *) arrayZ, length * حجم (Type));
      ܦܢܐ new_array;
    }
    ܦܢܐ (Type *) hb_realloc (arrayZ, new_allocated * حجم (Type));
  }
  Type *
  _malloc_move (unsigned new_allocated)
  {
    ܐܢ (!new_allocated)
    {
      ܐܢ (is_owned ())
	hb_free (arrayZ);
      ܦܢܐ nullptr;
    }
    Type *new_array = (Type *) hb_malloc (new_allocated * حجم (Type));
    ܐܢ (likely (new_array))
    {
      لكل (unsigned i = 0; i < length; i++)
      {
	جديد (std::addressof (new_array[i])) Type ();
	new_array[i] = std::move (arrayZ[i]);
	arrayZ[i].~Type ();
      }
      ܐܢ (is_owned ())
	hb_free (arrayZ);
    }
    ܦܢܐ new_array;
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<0>)
  {
    ܦܢܐ _realloc (new_allocated);
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<0>)
  {
    ܦܢܐ _malloc_move (new_allocated);
  }
  /* Specialization for types that can be moved using realloc(). */
  template <typename T = Type,
	    hb_enable_if (T::realloc_move)>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<1>)
  {
    ܦܢܐ _realloc (new_allocated);
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T))>
  فراغ
  grow_vector (unsigned size, hb_priority<0>)
  {
    hb_memset (arrayZ + length, 0, (size - length) * حجم (*arrayZ));
    length = size;
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_constructible(T))>
  فراغ
  grow_vector (unsigned size, hb_priority<0>)
  {
    لكل (; length < size; length++)
      جديد (std::addressof (arrayZ[length])) Type ();
  }
  /* Specialization for hb_vector_t<hb_{vector,array}_t<U>> to speed up. */
  template <typename T = Type,
	    hb_enable_if (hb_is_same (T, hb_vector_t<typename T::item_t>) ||
			  hb_is_same (T, hb_array_t <typename T::item_t>))>
  فراغ
  grow_vector (unsigned size, hb_priority<1>)
  {
    hb_memset (arrayZ + length, 0, (size - length) * حجم (*arrayZ));
    length = size;
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  فراغ
  copy_array (hb_array_t<Type> other)
  {
    hb_memcpy ((فراغ *) (arrayZ + length), (ثابت فراغ *) other.arrayZ, other.length * item_size);
    length += other.length;
  }
  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  فراغ
  copy_array (hb_array_t<ثابت Type> other)
  {
    hb_memcpy ((فراغ *) (arrayZ + length), (ثابت فراغ *) other.arrayZ, other.length * item_size);
    length += other.length;
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			   std::is_copy_constructible<T>::value)>
  فراغ
  copy_array (hb_array_t<ثابت Type> other)
  {
    لكل (unsigned i = 0; i < other.length; i++)
      جديد (std::addressof (arrayZ[length + i])) Type (other.arrayZ[i]);
    length += other.length;
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			  !std::is_copy_constructible<T>::value &&
			  std::is_default_constructible<T>::value &&
			  std::is_copy_assignable<T>::value)>
  فراغ
  copy_array (hb_array_t<ثابت Type> other)
  {
    لكل (unsigned i = 0; i < other.length; i++)
    {
      جديد (std::addressof (arrayZ[length + i])) Type ();
      arrayZ[length + i] = other.arrayZ[i];
    }
    length += other.length;
  }

  فراغ
  shrink_vector (unsigned size)
  {
    أكد (size <= length);
    ܐܢ (!hb_is_trivially_destructible(Type))
    {
      unsigned count = length - size;
      Type *p = arrayZ + length;
      ܟܡܐ (count--)
        (--p)->~Type ();
    }
    length = size;
  }

  فراغ
  shift_down_vector (unsigned i)
  {
    لكل (; i < length; i++)
      arrayZ[i - 1] = std::move (arrayZ[i]);
  }

  /* Allocate for size but don't adjust length. */
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  منطقي alloc (unsigned صحيح size, منطقي exact=ܕܓܠܐ)
  {
    ܐܢ (unlikely (in_error ()))
      ܦܢܐ ܕܓܠܐ;

    unsigned صحيح new_allocated;
    ܐܢ (exact)
    {
      /* If exact was specified, we allow shrinking the storage. */
      size = hb_max (size, length);
      ܐܢ (size <= (unsigned) allocated &&
	  size >= (unsigned) allocated >> 2)
	ܦܢܐ ܫܪܪܐ;

      new_allocated = size;
    }
    ܐܚܪܢܐ
    {
      ܐܢ (likely (size <= (unsigned) allocated))
	ܦܢܐ ܫܪܪܐ;

      new_allocated = allocated;
      ܟܡܐ (size > new_allocated)
	new_allocated += (new_allocated >> 1) + 8;
    }

    /* Reallocate */

    منطقي overflows =
      (صحيح) in_error () ||
      (new_allocated < size) ||
      hb_unsigned_mul_overflows (new_allocated, حجم (Type));

    ܐܢ (unlikely (overflows))
    {
      set_error ();
      ܦܢܐ ܕܓܠܐ;
    }

    Type *new_array = realloc_vector (new_allocated, hb_prioritize);

    ܐܢ (unlikely (new_allocated && !new_array))
    {
      ܐܢ (new_allocated <= (unsigned) allocated)
        ܦܢܐ ܫܪܪܐ; // shrinking failed; it's okay; happens in our fuzzer

      set_error ();
      ܦܢܐ ܕܓܠܐ;
    }

    arrayZ = new_array;
    allocated = new_allocated;

    ܦܢܐ ܫܪܪܐ;
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  منطقي alloc_exact (unsigned صحيح size)
  {
    ܦܢܐ alloc (size, ܫܪܪܐ);
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  فراغ clear ()
  {
    shrink_vector (0);
  }

  template <typename allocator_t>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  منطقي allocate_from_pool (allocator_t *allocator, unsigned size, unsigned صحيح initialize = ܫܪܪܐ)
  {
    ܐܢ (allocator)
    {
      أكد (!length && !allocated);
      arrayZ = (Type *) allocator->alloc (size * حجم (Type), alignof (Type));
      ܐܢ (unlikely (!arrayZ))
      {
	set_error ();
	ܦܢܐ ܕܓܠܐ;
      }
      ܐܢ (initialize)
	grow_vector (size, hb_prioritize);
      ܐܚܪܢܐ
	length = size;
      ܦܢܐ ܫܪܪܐ;
    }
    ܦܢܐ resize_full ((صحيح) size, initialize, ܫܪܪܐ);
  }

  template <typename allocator_t>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  منطقي duplicate_vector_from_pool (allocator_t *allocator, ثابت hb_vector_t &other)
  {
    ܐܢ (unlikely (!allocate_from_pool (allocator, other.length, ܕܓܠܐ)))
      ܦܢܐ ܕܓܠܐ;
    length = 0;
    copy_array (other.as_array ());
    ܦܢܐ ܫܪܪܐ;
  }

  template <typename allocator_t>
  فراغ shrink_back_to_pool (allocator_t *allocator, صحيح size)
  {
    unsigned orig_length = length;

    shrink (size, ܕܓܠܐ);

    ܐܢ (allocator && !is_owned ())
      allocator->discard (arrayZ + length, (orig_length - length) * حجم (Type));
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  منطقي resize_full (صحيح size_, منطقي initialize, منطقي exact)
  {
    ܐܢ (unlikely (size_ < 0))
      ܦܢܐ ܕܓܠܐ;
    unsigned صحيح size = (unsigned صحيح) size_;
    ܐܢ (!alloc (size, exact))
      ܦܢܐ ܕܓܠܐ;

    ܐܢ (size > length)
    {
      ܐܢ (initialize)
	grow_vector (size, hb_prioritize);
    }
    ܐܚܪܢܐ ܐܢ (size < length)
    {
      ܐܢ (initialize)
	shrink_vector (size);
    }

    length = size;
    ܦܢܐ ܫܪܪܐ;
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  منطقي resize (صحيح size_)
  {
    ܦܢܐ resize_full (size_, ܫܪܪܐ, ܕܓܠܐ);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  منطقي resize_dirty (صحيح size_)
  {
    ܦܢܐ resize_full (size_, ܕܓܠܐ, ܕܓܠܐ);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  منطقي resize_exact (صحيح size_)
  {
    ܦܢܐ resize_full (size_, ܫܪܪܐ, ܫܪܪܐ);
  }

  Type pop ()
  {
    ܐܢ (!length) ܦܢܐ Null (Type);
    Type v (std::move (arrayZ[length - 1]));
    arrayZ[length - 1].~Type ();
    length--;
    ܦܢܐ v;
  }

  فراغ remove_ordered (unsigned صحيح i)
  {
    ܐܢ (unlikely (i >= length))
      ܦܢܐ;
    shift_down_vector (i + 1);
    arrayZ[length - 1].~Type ();
    length--;
  }

  template <منطقي Sorted = sorted,
	    hb_enable_if (!Sorted)>
  فراغ remove_unordered (unsigned صحيح i)
  {
    ܐܢ (unlikely (i >= length))
      ܦܢܐ;
    ܐܢ (i != length - 1)
      arrayZ[i] = std::move (arrayZ[length - 1]);
    arrayZ[length - 1].~Type ();
    length--;
  }

  فراغ shrink (صحيح size_, منطقي shrink_memory = ܫܪܪܐ)
  {
    unsigned صحيح size = size_ < 0 ? 0u : (unsigned صحيح) size_;
    ܐܢ (size >= length)
      ܦܢܐ;

    shrink_vector (size);

    ܐܢ (is_owned () && shrink_memory)
      alloc_exact (size); /* To force shrinking memory if needed. */
  }


  /* Sorting API. */
  template <typename Compar>
  فراغ qsort (Compar compar)
  { as_array ().qsort (compar); }
  فراغ qsort ()
  { as_array ().qsort (); }

  /* Unsorted search API. */
  template <typename T>
  Type *lsearch (ثابت T &x, Type *not_found = nullptr)
  { ܦܢܐ as_array ().lsearch (x, not_found); }
  template <typename T>
  ثابت Type *lsearch (ثابت T &x, ثابت Type *not_found = nullptr) ثابت
  { ܦܢܐ as_array ().lsearch (x, not_found); }
  template <typename T>
  منطقي lfind (ثابت T &x, unsigned *pos = nullptr) ثابت
  { ܦܢܐ as_array ().lfind (x, pos); }

  /* Sorted search API. */
  template <typename T,
	    منطقي Sorted=sorted, hb_enable_if (Sorted)>
  Type *bsearch (ثابت T &x, Type *not_found = nullptr)
  { ܦܢܐ as_array ().bsearch (x, not_found); }
  template <typename T,
	    منطقي Sorted=sorted, hb_enable_if (Sorted)>
  ثابت Type *bsearch (ثابت T &x, ثابت Type *not_found = nullptr) ثابت
  { ܦܢܐ as_array ().bsearch (x, not_found); }
  template <typename T,
	    منطقي Sorted=sorted, hb_enable_if (Sorted)>
  منطقي bfind (ثابت T &x, unsigned صحيح *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned صحيح to_store = (unsigned صحيح) -1) ثابت
  { ܦܢܐ as_array ().bfind (x, i, not_found, to_store); }
};

template <typename Type>
using hb_sorted_vector_t = hb_vector_t<Type, ܫܪܪܐ>;

#endif /* HB_VECTOR_HH */

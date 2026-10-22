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

#شامل "hb.hh"
#شامل "hb-array.hh"
#شامل "hb-meta.hh"
#شامل "hb-null.hh"

// Change to 1 to force inline vector allocs, to see callsite in malloc-stats tool.
#اگر 0
#define HB_ALWAYS_INLINE_VECTOR_ALLOCS HB_ALWAYS_INLINE
#وگرنه
#define HB_ALWAYS_INLINE_VECTOR_ALLOCS
#endif

template <typename Type,
	  منطقی sorted=نادرست>
ساختار hb_vector_t
{
  ایستا constexpr منطقی realloc_move = درست;

  تعریفنوع Type item_t;
  ایستا constexpr unsigned item_size = hb_static_size (Type);
  using array_t = typename std::conditional<sorted, hb_sorted_array_t<Type>, hb_array_t<Type>>::type;
  using c_array_t = typename std::conditional<sorted, hb_sorted_array_t<ثابت Type>, hb_array_t<ثابت Type>>::type;

  hb_vector_t () = پیشفرض;
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (std::initializer_list<Type> lst) : hb_vector_t ()
  {
    alloc (lst.size (), درست);
    برای (auto&& item : lst)
      push (item);
  }
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  explicit hb_vector_t (ثابت Iterable &o) : hb_vector_t ()
  {
    extend (o, درست);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (ثابت hb_vector_t &o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    اگر (unlikely (in_error ())) بازگشت;
    copy_array (o.as_array ());
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (array_t o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    اگر (unlikely (in_error ())) بازگشت;
    copy_array (o);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (c_array_t o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    اگر (unlikely (in_error ())) بازگشت;
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
  تهی
  set_storage (Type (&array)[n])
  { set_storage (array, n); }
  تهی
  set_storage (hb_array_t<Type> array)
  { set_storage (array.arrayZ, array.length); }
  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T) &&
			  hb_is_trivially_destructible(T))>
  تهی
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
  تهی extend (ثابت Iterable &o, منطقی exact=نادرست)
  {
    auto iter = hb_iter (o);
    اگر (iter.is_random_access_iterator || iter.has_fast_len)
    {
      اگر (unlikely (!alloc (length + hb_len (iter), exact)))
	بازگشت;
      unsigned شمار = hb_len (iter);
      برای (unsigned i = 0; i < شمار; i++)
	افزودن_has_room (*iter++);
    }
    وقتی (iter)
    {
      اگر (unlikely (!alloc (length + 1)))
        بازگشت;
      unsigned room = allocated - length;
      برای (unsigned i = 0; i < room && iter; i++)
	افزودن_has_room (*iter++);
    }
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  تهی extend (array_t o, منطقی exact=نادرست)
  { extend (c_array_t (o), exact); }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  تهی extend (c_array_t o, منطقی exact=نادرست)
  {
    /* o may point into our own storage, eg. v.extend (v.as_array ()).
     * alloc() may move that storage, so find o again afterwards. */
    منطقی aliased = o.length && in_storage (*o.arrayZ);
    unsigned offset = aliased ? o.arrayZ - arrayZ : 0;
    alloc (length + o.length, exact);
    اگر (unlikely (in_error ())) بازگشت;
    اگر (unlikely (aliased))
      o = {arrayZ + offset, o.length};
    copy_array (o);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  تهی extend (ثابت hb_vector_t &o, منطقی exact=نادرست)
  { extend (o.as_array (), exact); }

  عمومی:
  صحیح allocated = 0; /* < 0 means allocation failed. */
  unsigned صحیح length = 0;
  عمومی:
  Type *arrayZ = nullptr;

  تهی init ()
  {
    allocated = length = 0;
    arrayZ = nullptr;
  }
  تهی init0 ()
  {
  }

  تهی fini ()
  {
    اگر (is_owned ())
    {
      shrink_vector (0);
      hb_free (arrayZ);
    }
    init ();
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t &reset ()
  {
    اگر (unlikely (in_error ()))
      reset_error ();
    clear ();
    بازگشت *این;
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t &reset_if_error ()
  {
    اگر (unlikely (in_error ()))
      reset ();
    بازگشت *این;
  }

  /* Transfer ownership of the backing storage to caller.
   * Returns nullptr if storage is not owned by this vector. */
  Type *
  steal (unsigned *len = nullptr, صحیح *allocated_out = nullptr)
  {
    اگر (!is_owned ())
      بازگشت nullptr;
    اگر (len)
      *len = length;
    اگر (allocated_out)
      *allocated_out = allocated;
    Type *p = arrayZ;
    init ();
    بازگشت p;
  }

  /* Adopt a previously detached owned buffer. */
  تهی
  recycle_buffer (Type *buffer,
                  unsigned len,
                  صحیح allocated_len)
  {
    fini ();
    arrayZ = buffer;
    length = len;
    allocated = allocated_len;
  }

  friend تهی swap (hb_vector_t& a, hb_vector_t& b) noexcept
  {
    hb_swap (a.allocated, b.allocated);
    hb_swap (a.length, b.length);
    hb_swap (a.arrayZ, b.arrayZ);
  }

  hb_vector_t& operator = (ثابت hb_vector_t &o)
  {
    اگر (این == &o) بازگشت *این;

    reset ();
    alloc_exact (o.length);
    اگر (unlikely (in_error ())) بازگشت *این;

    length = 0;
    copy_array (o.as_array ());

    بازگشت *این;
  }
  hb_vector_t& operator = (hb_vector_t &&o) noexcept
  {
    hb_swap (*این, o);
    بازگشت *این;
  }

  hb_bytes_t as_bytes () ثابت
  { بازگشت hb_bytes_t ((ثابت نویسه *) arrayZ, get_size ()); }

  منطقی operator == (ثابت hb_vector_t &o) ثابت { بازگشت as_array () == o.as_array (); }
  منطقی operator != (ثابت hb_vector_t &o) ثابت { بازگشت !(*این == o); }
  uint32_t hash () ثابت { بازگشت as_array ().hash (); }

  Type& operator [] (صحیح i_)
  {
    unsigned صحیح i = (unsigned صحیح) i_;
    اگر (unlikely (i >= length))
      بازگشت Crap (Type);
    بازگشت arrayZ[i];
  }
  ثابت Type& operator [] (صحیح i_) ثابت
  {
    unsigned صحیح i = (unsigned صحیح) i_;
    اگر (unlikely (i >= length))
      بازگشت Null (Type);
    بازگشت arrayZ[i];
  }

  Type& tail () { بازگشت (*این)[length - 1]; }
  ثابت Type& tail () ثابت { بازگشت (*این)[length - 1]; }

  explicit operator منطقی () ثابت { بازگشت length; }
  size_t get_size () ثابت { بازگشت hb_unsigned_mul_saturate (length, item_size); }

  /* Sink interface. */
  template <typename T>
  hb_vector_t& operator << (T&& v) { push (std::forward<T> (v)); بازگشت *این; }

  array_t   as_array ()       { بازگشت hb_array (arrayZ, length); }
  c_array_t as_array () ثابت { بازگشت hb_array (arrayZ, length); }

  /* Iterator. */
  تعریفنوع c_array_t   iter_t;
  تعریفنوع array_t   writer_t;
    iter_t   iter () ثابت { بازگشت as_array (); }
  writer_t writer ()       { بازگشت as_array (); }
  operator   iter_t () ثابت { بازگشت   iter (); }
  operator writer_t ()       { بازگشت writer (); }

  /* Faster range-based for loop. */
  Type *begin () ثابت { بازگشت arrayZ; }
  Type *end () ثابت { بازگشت arrayZ + length; }


  hb_sorted_array_t<Type> as_sorted_array ()
  { بازگشت hb_sorted_array (arrayZ, length); }
  hb_sorted_array_t<ثابت Type> as_sorted_array () ثابت
  { بازگشت hb_sorted_array (arrayZ, length); }

  template <typename T> explicit operator T * () { بازگشت arrayZ; }
  template <typename T> explicit operator ثابت T * () ثابت { بازگشت arrayZ; }

  Type * operator  + (unsigned صحیح i) { بازگشت arrayZ + i; }
  ثابت Type * operator  + (unsigned صحیح i) ثابت { بازگشت arrayZ + i; }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push ()
  {
    اگر (unlikely (!resize (length + 1)))
      بازگشت std::addressof (Crap (Type));
    بازگشت std::addressof (arrayZ[length - 1]);
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push (Args&&... args)
  {
    اگر (unlikely ((صحیح) length >= allocated))
    {
      اگر (unlikely (in_error ()))
	بازگشت std::addressof (Crap (Type));

      /* An argument may live in our own storage, eg. v.push (v[i]).
       * alloc() frees that storage, so copy it out first. */
      اگر (unlikely (in_storage (args...)))
      {
	Type copy (std::forward<Args> (args)...);
	اگر (unlikely (!alloc (length + 1)))
	  بازگشت std::addressof (Crap (Type));
	بازگشت افزودن_has_room (std::move (copy));
      }
      اگر (unlikely (!alloc (length + 1)))
	// If push failed to allocate then don't copy v, since this may cause
	// the created copy to leak memory since we won't have stored a
	// reference to it.
	بازگشت std::addressof (Crap (Type));
    }

    بازگشت افزودن_has_room (std::forward<Args> (args)...);
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  منطقی افزودن_or_fail (Args&&... args)
  {
    بازگشت push (std::forward<Args> (args)...) != std::addressof (Crap (Type));
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *افزودن_has_room (Args&&... args)
  {
    /* Emplace. */
    Type *p = std::addressof (arrayZ[length++]);
    بازگشت جدید (p) Type (std::forward<Args> (args)...);
  }

  منطقی in_storage () ثابت { بازگشت نادرست; }
  template <typename T, typename... Ts>
  منطقی in_storage (ثابت T &v, ثابت Ts &...vs) ثابت
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    بازگشت (arrayZ &&
	    (uintptr_t) arrayZ <= p && p < (uintptr_t) (arrayZ + length)) ||
	   in_storage (vs...);
  }

  منطقی is_owned () ثابت
  {
    بازگشت allocated != 0 && allocated != -1;
  }

  منطقی in_error () ثابت { بازگشت allocated < 0; }
  تهی set_error ()
  {
    أكد (allocated >= 0);
    allocated = -allocated - 1;
  }
  تهی reset_error ()
  {
    أكد (allocated < 0);
    allocated = -(allocated + 1);
  }
  تهی ensure_error ()
  {
    اگر (!in_error ())
      set_error ();
  }

  Type *
  _realloc (unsigned new_allocated)
  {
    اگر (!new_allocated)
    {
      اگر (is_owned ())
	hb_free (arrayZ);
      بازگشت nullptr;
    }
    اگر (!allocated && arrayZ)
    {
      /* If we have a non-null arrayZ but allocated is 0, then we are
       * reallocating from a foreign array. */
      Type *new_array = (Type *) hb_malloc (new_allocated * اندازه (Type));
      اگر (unlikely (!new_array))
	بازگشت nullptr;
      hb_memcpy ((تهی *) new_array, (ثابت تهی *) arrayZ, length * اندازه (Type));
      بازگشت new_array;
    }
    بازگشت (Type *) hb_realloc (arrayZ, new_allocated * اندازه (Type));
  }
  Type *
  _malloc_move (unsigned new_allocated)
  {
    اگر (!new_allocated)
    {
      اگر (is_owned ())
	hb_free (arrayZ);
      بازگشت nullptr;
    }
    Type *new_array = (Type *) hb_malloc (new_allocated * اندازه (Type));
    اگر (likely (new_array))
    {
      برای (unsigned i = 0; i < length; i++)
      {
	جدید (std::addressof (new_array[i])) Type ();
	new_array[i] = std::move (arrayZ[i]);
	arrayZ[i].~Type ();
      }
      اگر (is_owned ())
	hb_free (arrayZ);
    }
    بازگشت new_array;
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<0>)
  {
    بازگشت _realloc (new_allocated);
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<0>)
  {
    بازگشت _malloc_move (new_allocated);
  }
  /* Specialization for types that can be moved using realloc(). */
  template <typename T = Type,
	    hb_enable_if (T::realloc_move)>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<1>)
  {
    بازگشت _realloc (new_allocated);
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T))>
  تهی
  grow_vector (unsigned size, hb_priority<0>)
  {
    hb_memset (arrayZ + length, 0, (size - length) * اندازه (*arrayZ));
    length = size;
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_constructible(T))>
  تهی
  grow_vector (unsigned size, hb_priority<0>)
  {
    برای (; length < size; length++)
      جدید (std::addressof (arrayZ[length])) Type ();
  }
  /* Specialization for hb_vector_t<hb_{vector,array}_t<U>> to speed up. */
  template <typename T = Type,
	    hb_enable_if (hb_is_same (T, hb_vector_t<typename T::item_t>) ||
			  hb_is_same (T, hb_array_t <typename T::item_t>))>
  تهی
  grow_vector (unsigned size, hb_priority<1>)
  {
    hb_memset (arrayZ + length, 0, (size - length) * اندازه (*arrayZ));
    length = size;
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  تهی
  copy_array (hb_array_t<Type> other)
  {
    hb_memcpy ((تهی *) (arrayZ + length), (ثابت تهی *) other.arrayZ, other.length * item_size);
    length += other.length;
  }
  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  تهی
  copy_array (hb_array_t<ثابت Type> other)
  {
    hb_memcpy ((تهی *) (arrayZ + length), (ثابت تهی *) other.arrayZ, other.length * item_size);
    length += other.length;
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			   std::is_copy_constructible<T>::value)>
  تهی
  copy_array (hb_array_t<ثابت Type> other)
  {
    برای (unsigned i = 0; i < other.length; i++)
      جدید (std::addressof (arrayZ[length + i])) Type (other.arrayZ[i]);
    length += other.length;
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			  !std::is_copy_constructible<T>::value &&
			  std::is_default_constructible<T>::value &&
			  std::is_copy_assignable<T>::value)>
  تهی
  copy_array (hb_array_t<ثابت Type> other)
  {
    برای (unsigned i = 0; i < other.length; i++)
    {
      جدید (std::addressof (arrayZ[length + i])) Type ();
      arrayZ[length + i] = other.arrayZ[i];
    }
    length += other.length;
  }

  تهی
  shrink_vector (unsigned size)
  {
    أكد (size <= length);
    اگر (!hb_is_trivially_destructible(Type))
    {
      unsigned شمار = length - size;
      Type *p = arrayZ + length;
      وقتی (شمار--)
        (--p)->~Type ();
    }
    length = size;
  }

  تهی
  shift_down_vector (unsigned i)
  {
    برای (; i < length; i++)
      arrayZ[i - 1] = std::move (arrayZ[i]);
  }

  /* Allocate for size but don't adjust length. */
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  منطقی alloc (unsigned صحیح size, منطقی exact=نادرست)
  {
    اگر (unlikely (in_error ()))
      بازگشت نادرست;

    unsigned صحیح new_allocated;
    اگر (exact)
    {
      /* If exact was specified, we allow shrinking the storage. */
      size = hb_max (size, length);
      اگر (size <= (unsigned) allocated &&
	  size >= (unsigned) allocated >> 2)
	بازگشت درست;

      new_allocated = size;
    }
    وگرنه
    {
      اگر (likely (size <= (unsigned) allocated))
	بازگشت درست;

      new_allocated = allocated;
      وقتی (size > new_allocated)
	new_allocated += (new_allocated >> 1) + 8;
    }

    /* Reallocate */

    منطقی overflows =
      (صحیح) in_error () ||
      (new_allocated < size) ||
      hb_unsigned_mul_overflows (new_allocated, اندازه (Type));

    اگر (unlikely (overflows))
    {
      set_error ();
      بازگشت نادرست;
    }

    Type *new_array = realloc_vector (new_allocated, hb_prioritize);

    اگر (unlikely (new_allocated && !new_array))
    {
      اگر (new_allocated <= (unsigned) allocated)
        بازگشت درست; // shrinking failed; it's okay; happens in our fuzzer

      set_error ();
      بازگشت نادرست;
    }

    arrayZ = new_array;
    allocated = new_allocated;

    بازگشت درست;
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  منطقی alloc_exact (unsigned صحیح size)
  {
    بازگشت alloc (size, درست);
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  تهی clear ()
  {
    shrink_vector (0);
  }

  template <typename allocator_t>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  منطقی allocate_from_pool (allocator_t *allocator, unsigned size, unsigned صحیح initialize = درست)
  {
    اگر (allocator)
    {
      أكد (!length && !allocated);
      arrayZ = (Type *) allocator->alloc (size * اندازه (Type), alignof (Type));
      اگر (unlikely (!arrayZ))
      {
	set_error ();
	بازگشت نادرست;
      }
      اگر (initialize)
	grow_vector (size, hb_prioritize);
      وگرنه
	length = size;
      بازگشت درست;
    }
    بازگشت resize_full ((صحیح) size, initialize, درست);
  }

  template <typename allocator_t>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  منطقی duplicate_vector_from_pool (allocator_t *allocator, ثابت hb_vector_t &other)
  {
    اگر (unlikely (!allocate_from_pool (allocator, other.length, نادرست)))
      بازگشت نادرست;
    length = 0;
    copy_array (other.as_array ());
    بازگشت درست;
  }

  template <typename allocator_t>
  تهی shrink_back_to_pool (allocator_t *allocator, صحیح size)
  {
    unsigned orig_length = length;

    shrink (size, نادرست);

    اگر (allocator && !is_owned ())
      allocator->discard (arrayZ + length, (orig_length - length) * اندازه (Type));
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  منطقی resize_full (صحیح size_, منطقی initialize, منطقی exact)
  {
    اگر (unlikely (size_ < 0))
      بازگشت نادرست;
    unsigned صحیح size = (unsigned صحیح) size_;
    اگر (!alloc (size, exact))
      بازگشت نادرست;

    اگر (size > length)
    {
      اگر (initialize)
	grow_vector (size, hb_prioritize);
    }
    وگرنه اگر (size < length)
    {
      اگر (initialize)
	shrink_vector (size);
    }

    length = size;
    بازگشت درست;
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  منطقی resize (صحیح size_)
  {
    بازگشت resize_full (size_, درست, نادرست);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  منطقی resize_dirty (صحیح size_)
  {
    بازگشت resize_full (size_, نادرست, نادرست);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  منطقی resize_exact (صحیح size_)
  {
    بازگشت resize_full (size_, درست, درست);
  }

  Type برداشت ()
  {
    اگر (!length) بازگشت Null (Type);
    Type v (std::move (arrayZ[length - 1]));
    arrayZ[length - 1].~Type ();
    length--;
    بازگشت v;
  }

  تهی remove_ordered (unsigned صحیح i)
  {
    اگر (unlikely (i >= length))
      بازگشت;
    shift_down_vector (i + 1);
    arrayZ[length - 1].~Type ();
    length--;
  }

  template <منطقی Sorted = sorted,
	    hb_enable_if (!Sorted)>
  تهی remove_unordered (unsigned صحیح i)
  {
    اگر (unlikely (i >= length))
      بازگشت;
    اگر (i != length - 1)
      arrayZ[i] = std::move (arrayZ[length - 1]);
    arrayZ[length - 1].~Type ();
    length--;
  }

  تهی shrink (صحیح size_, منطقی shrink_memory = درست)
  {
    unsigned صحیح size = size_ < 0 ? 0u : (unsigned صحیح) size_;
    اگر (size >= length)
      بازگشت;

    shrink_vector (size);

    اگر (is_owned () && shrink_memory)
      alloc_exact (size); /* To force shrinking memory if needed. */
  }


  /* Sorting API. */
  template <typename Compar>
  تهی qsort (Compar compar)
  { as_array ().qsort (compar); }
  تهی qsort ()
  { as_array ().qsort (); }

  /* Unsorted search API. */
  template <typename T>
  Type *lsearch (ثابت T &x, Type *not_found = nullptr)
  { بازگشت as_array ().lsearch (x, not_found); }
  template <typename T>
  ثابت Type *lsearch (ثابت T &x, ثابت Type *not_found = nullptr) ثابت
  { بازگشت as_array ().lsearch (x, not_found); }
  template <typename T>
  منطقی lfind (ثابت T &x, unsigned *pos = nullptr) ثابت
  { بازگشت as_array ().lfind (x, pos); }

  /* Sorted search API. */
  template <typename T,
	    منطقی Sorted=sorted, hb_enable_if (Sorted)>
  Type *bsearch (ثابت T &x, Type *not_found = nullptr)
  { بازگشت as_array ().bsearch (x, not_found); }
  template <typename T,
	    منطقی Sorted=sorted, hb_enable_if (Sorted)>
  ثابت Type *bsearch (ثابت T &x, ثابت Type *not_found = nullptr) ثابت
  { بازگشت as_array ().bsearch (x, not_found); }
  template <typename T,
	    منطقی Sorted=sorted, hb_enable_if (Sorted)>
  منطقی bfind (ثابت T &x, unsigned صحیح *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned صحیح to_store = (unsigned صحیح) -1) ثابت
  { بازگشت as_array ().bfind (x, i, not_found, to_store); }
};

template <typename Type>
using hb_sorted_vector_t = hb_vector_t<Type, درست>;

#endif /* HB_VECTOR_HH */

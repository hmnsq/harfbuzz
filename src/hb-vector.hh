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
#אם 0
#define HB_ALWAYS_INLINE_VECTOR_ALLOCS HB_ALWAYS_INLINE
#אחרת
#define HB_ALWAYS_INLINE_VECTOR_ALLOCS
#endif

template <typename Type,
	  منطقي sorted=שקר>
بنية hb_vector_t
{
  ساكن constexpr منطقي realloc_move = אמת;

  تعريفنوع Type item_t;
  ساكن constexpr unsigned item_size = hb_static_size (Type);
  using array_t = typename std::conditional<sorted, hb_sorted_array_t<Type>, hb_array_t<Type>>::type;
  using c_array_t = typename std::conditional<sorted, hb_sorted_array_t<ثابت Type>, hb_array_t<ثابت Type>>::type;

  hb_vector_t () = افتراضي;
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (std::initializer_list<Type> lst) : hb_vector_t ()
  {
    alloc (lst.size (), אמת);
    עבור (auto&& item : lst)
      push (item);
  }
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  explicit hb_vector_t (ثابت Iterable &o) : hb_vector_t ()
  {
    extend (o, אמת);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (ثابت hb_vector_t &o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    אם (unlikely (in_error ())) החזר;
    copy_array (o.as_array ());
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (array_t o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    אם (unlikely (in_error ())) החזר;
    copy_array (o);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (c_array_t o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    אם (unlikely (in_error ())) החזר;
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
  فراغ extend (ثابت Iterable &o, منطقي exact=שקר)
  {
    auto iter = hb_iter (o);
    אם (iter.is_random_access_iterator || iter.has_fast_len)
    {
      אם (unlikely (!alloc (length + hb_len (iter), exact)))
	החזר;
      unsigned count = hb_len (iter);
      עבור (unsigned i = 0; i < count; i++)
	push_has_room (*iter++);
    }
    כלעוד (iter)
    {
      אם (unlikely (!alloc (length + 1)))
        החזר;
      unsigned room = allocated - length;
      עבור (unsigned i = 0; i < room && iter; i++)
	push_has_room (*iter++);
    }
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  فراغ extend (array_t o, منطقي exact=שקר)
  { extend (c_array_t (o), exact); }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  فراغ extend (c_array_t o, منطقي exact=שקר)
  {
    /* o may point into our own storage, eg. v.extend (v.as_array ()).
     * alloc() may move that storage, so find o again afterwards. */
    منطقي aliased = o.length && in_storage (*o.arrayZ);
    unsigned offset = aliased ? o.arrayZ - arrayZ : 0;
    alloc (length + o.length, exact);
    אם (unlikely (in_error ())) החזר;
    אם (unlikely (aliased))
      o = {arrayZ + offset, o.length};
    copy_array (o);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  فراغ extend (ثابت hb_vector_t &o, منطقي exact=שקר)
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
    אם (is_owned ())
    {
      shrink_vector (0);
      hb_free (arrayZ);
    }
    init ();
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t &reset ()
  {
    אם (unlikely (in_error ()))
      reset_error ();
    clear ();
    החזר *هذا;
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t &reset_if_error ()
  {
    אם (unlikely (in_error ()))
      reset ();
    החזר *هذا;
  }

  /* Transfer ownership of the backing storage to caller.
   * Returns nullptr if storage is not owned by this vector. */
  Type *
  steal (unsigned *len = nullptr, صحيح *allocated_out = nullptr)
  {
    אם (!is_owned ())
      החזר nullptr;
    אם (len)
      *len = length;
    אם (allocated_out)
      *allocated_out = allocated;
    Type *p = arrayZ;
    init ();
    החזר p;
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
    אם (هذا == &o) החזר *هذا;

    reset ();
    alloc_exact (o.length);
    אם (unlikely (in_error ())) החזר *هذا;

    length = 0;
    copy_array (o.as_array ());

    החזר *هذا;
  }
  hb_vector_t& operator = (hb_vector_t &&o) noexcept
  {
    hb_swap (*هذا, o);
    החזר *هذا;
  }

  hb_bytes_t as_bytes () ثابت
  { החזר hb_bytes_t ((ثابت حرف *) arrayZ, get_size ()); }

  منطقي operator == (ثابت hb_vector_t &o) ثابت { החזר as_array () == o.as_array (); }
  منطقي operator != (ثابت hb_vector_t &o) ثابت { החזר !(*هذا == o); }
  uint32_t hash () ثابت { החזר as_array ().hash (); }

  Type& operator [] (صحيح i_)
  {
    unsigned صحيح i = (unsigned صحيح) i_;
    אם (unlikely (i >= length))
      החזר Crap (Type);
    החזר arrayZ[i];
  }
  ثابت Type& operator [] (صحيح i_) ثابت
  {
    unsigned صحيح i = (unsigned صحيح) i_;
    אם (unlikely (i >= length))
      החזר Null (Type);
    החזר arrayZ[i];
  }

  Type& tail () { החזר (*هذا)[length - 1]; }
  ثابت Type& tail () ثابت { החזר (*هذا)[length - 1]; }

  explicit operator منطقي () ثابت { החזר length; }
  size_t get_size () ثابت { החזר hb_unsigned_mul_saturate (length, item_size); }

  /* Sink interface. */
  template <typename T>
  hb_vector_t& operator << (T&& v) { push (std::forward<T> (v)); החזר *هذا; }

  array_t   as_array ()       { החזר hb_array (arrayZ, length); }
  c_array_t as_array () ثابت { החזר hb_array (arrayZ, length); }

  /* Iterator. */
  تعريفنوع c_array_t   iter_t;
  تعريفنوع array_t   writer_t;
    iter_t   iter () ثابت { החזר as_array (); }
  writer_t writer ()       { החזר as_array (); }
  operator   iter_t () ثابت { החזר   iter (); }
  operator writer_t ()       { החזר writer (); }

  /* Faster range-based for loop. */
  Type *begin () ثابت { החזר arrayZ; }
  Type *end () ثابت { החזר arrayZ + length; }


  hb_sorted_array_t<Type> as_sorted_array ()
  { החזר hb_sorted_array (arrayZ, length); }
  hb_sorted_array_t<ثابت Type> as_sorted_array () ثابت
  { החזר hb_sorted_array (arrayZ, length); }

  template <typename T> explicit operator T * () { החזר arrayZ; }
  template <typename T> explicit operator ثابت T * () ثابت { החזר arrayZ; }

  Type * operator  + (unsigned صحيح i) { החזר arrayZ + i; }
  ثابت Type * operator  + (unsigned صحيح i) ثابت { החזר arrayZ + i; }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push ()
  {
    אם (unlikely (!resize (length + 1)))
      החזר std::addressof (Crap (Type));
    החזר std::addressof (arrayZ[length - 1]);
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push (Args&&... args)
  {
    אם (unlikely ((صحيح) length >= allocated))
    {
      אם (unlikely (in_error ()))
	החזר std::addressof (Crap (Type));

      /* An argument may live in our own storage, eg. v.push (v[i]).
       * alloc() frees that storage, so copy it out first. */
      אם (unlikely (in_storage (args...)))
      {
	Type copy (std::forward<Args> (args)...);
	אם (unlikely (!alloc (length + 1)))
	  החזר std::addressof (Crap (Type));
	החזר push_has_room (std::move (copy));
      }
      אם (unlikely (!alloc (length + 1)))
	// If push failed to allocate then don't copy v, since this may cause
	// the created copy to leak memory since we won't have stored a
	// reference to it.
	החזר std::addressof (Crap (Type));
    }

    החזר push_has_room (std::forward<Args> (args)...);
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  منطقي push_or_fail (Args&&... args)
  {
    החזר push (std::forward<Args> (args)...) != std::addressof (Crap (Type));
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push_has_room (Args&&... args)
  {
    /* Emplace. */
    Type *p = std::addressof (arrayZ[length++]);
    החזר جديد (p) Type (std::forward<Args> (args)...);
  }

  منطقي in_storage () ثابت { החזר שקר; }
  template <typename T, typename... Ts>
  منطقي in_storage (ثابت T &v, ثابت Ts &...vs) ثابت
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    החזר (arrayZ &&
	    (uintptr_t) arrayZ <= p && p < (uintptr_t) (arrayZ + length)) ||
	   in_storage (vs...);
  }

  منطقي is_owned () ثابت
  {
    החזר allocated != 0 && allocated != -1;
  }

  منطقي in_error () ثابت { החזר allocated < 0; }
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
    אם (!in_error ())
      set_error ();
  }

  Type *
  _realloc (unsigned new_allocated)
  {
    אם (!new_allocated)
    {
      אם (is_owned ())
	hb_free (arrayZ);
      החזר nullptr;
    }
    אם (!allocated && arrayZ)
    {
      /* If we have a non-null arrayZ but allocated is 0, then we are
       * reallocating from a foreign array. */
      Type *new_array = (Type *) hb_malloc (new_allocated * حجم (Type));
      אם (unlikely (!new_array))
	החזר nullptr;
      hb_memcpy ((فراغ *) new_array, (ثابت فراغ *) arrayZ, length * حجم (Type));
      החזר new_array;
    }
    החזר (Type *) hb_realloc (arrayZ, new_allocated * حجم (Type));
  }
  Type *
  _malloc_move (unsigned new_allocated)
  {
    אם (!new_allocated)
    {
      אם (is_owned ())
	hb_free (arrayZ);
      החזר nullptr;
    }
    Type *new_array = (Type *) hb_malloc (new_allocated * حجم (Type));
    אם (likely (new_array))
    {
      עבור (unsigned i = 0; i < length; i++)
      {
	جديد (std::addressof (new_array[i])) Type ();
	new_array[i] = std::move (arrayZ[i]);
	arrayZ[i].~Type ();
      }
      אם (is_owned ())
	hb_free (arrayZ);
    }
    החזר new_array;
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<0>)
  {
    החזר _realloc (new_allocated);
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<0>)
  {
    החזר _malloc_move (new_allocated);
  }
  /* Specialization for types that can be moved using realloc(). */
  template <typename T = Type,
	    hb_enable_if (T::realloc_move)>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<1>)
  {
    החזר _realloc (new_allocated);
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
    עבור (; length < size; length++)
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
    עבור (unsigned i = 0; i < other.length; i++)
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
    עבור (unsigned i = 0; i < other.length; i++)
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
    אם (!hb_is_trivially_destructible(Type))
    {
      unsigned count = length - size;
      Type *p = arrayZ + length;
      כלעוד (count--)
        (--p)->~Type ();
    }
    length = size;
  }

  فراغ
  shift_down_vector (unsigned i)
  {
    עבור (; i < length; i++)
      arrayZ[i - 1] = std::move (arrayZ[i]);
  }

  /* Allocate for size but don't adjust length. */
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  منطقي alloc (unsigned صحيح size, منطقي exact=שקר)
  {
    אם (unlikely (in_error ()))
      החזר שקר;

    unsigned صحيح new_allocated;
    אם (exact)
    {
      /* If exact was specified, we allow shrinking the storage. */
      size = hb_max (size, length);
      אם (size <= (unsigned) allocated &&
	  size >= (unsigned) allocated >> 2)
	החזר אמת;

      new_allocated = size;
    }
    אחרת
    {
      אם (likely (size <= (unsigned) allocated))
	החזר אמת;

      new_allocated = allocated;
      כלעוד (size > new_allocated)
	new_allocated += (new_allocated >> 1) + 8;
    }

    /* Reallocate */

    منطقي overflows =
      (صحيح) in_error () ||
      (new_allocated < size) ||
      hb_unsigned_mul_overflows (new_allocated, حجم (Type));

    אם (unlikely (overflows))
    {
      set_error ();
      החזר שקר;
    }

    Type *new_array = realloc_vector (new_allocated, hb_prioritize);

    אם (unlikely (new_allocated && !new_array))
    {
      אם (new_allocated <= (unsigned) allocated)
        החזר אמת; // shrinking failed; it's okay; happens in our fuzzer

      set_error ();
      החזר שקר;
    }

    arrayZ = new_array;
    allocated = new_allocated;

    החזר אמת;
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  منطقي alloc_exact (unsigned صحيح size)
  {
    החזר alloc (size, אמת);
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  فراغ clear ()
  {
    shrink_vector (0);
  }

  template <typename allocator_t>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  منطقي allocate_from_pool (allocator_t *allocator, unsigned size, unsigned صحيح initialize = אמת)
  {
    אם (allocator)
    {
      أكد (!length && !allocated);
      arrayZ = (Type *) allocator->alloc (size * حجم (Type), alignof (Type));
      אם (unlikely (!arrayZ))
      {
	set_error ();
	החזר שקר;
      }
      אם (initialize)
	grow_vector (size, hb_prioritize);
      אחרת
	length = size;
      החזר אמת;
    }
    החזר resize_full ((صحيح) size, initialize, אמת);
  }

  template <typename allocator_t>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  منطقي duplicate_vector_from_pool (allocator_t *allocator, ثابت hb_vector_t &other)
  {
    אם (unlikely (!allocate_from_pool (allocator, other.length, שקר)))
      החזר שקר;
    length = 0;
    copy_array (other.as_array ());
    החזר אמת;
  }

  template <typename allocator_t>
  فراغ shrink_back_to_pool (allocator_t *allocator, صحيح size)
  {
    unsigned orig_length = length;

    shrink (size, שקר);

    אם (allocator && !is_owned ())
      allocator->discard (arrayZ + length, (orig_length - length) * حجم (Type));
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  منطقي resize_full (صحيح size_, منطقي initialize, منطقي exact)
  {
    אם (unlikely (size_ < 0))
      החזר שקר;
    unsigned صحيح size = (unsigned صحيح) size_;
    אם (!alloc (size, exact))
      החזר שקר;

    אם (size > length)
    {
      אם (initialize)
	grow_vector (size, hb_prioritize);
    }
    אחרת אם (size < length)
    {
      אם (initialize)
	shrink_vector (size);
    }

    length = size;
    החזר אמת;
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  منطقي resize (صحيح size_)
  {
    החזר resize_full (size_, אמת, שקר);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  منطقي resize_dirty (صحيح size_)
  {
    החזר resize_full (size_, שקר, שקר);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  منطقي resize_exact (صحيح size_)
  {
    החזר resize_full (size_, אמת, אמת);
  }

  Type pop ()
  {
    אם (!length) החזר Null (Type);
    Type v (std::move (arrayZ[length - 1]));
    arrayZ[length - 1].~Type ();
    length--;
    החזר v;
  }

  فراغ remove_ordered (unsigned صحيح i)
  {
    אם (unlikely (i >= length))
      החזר;
    shift_down_vector (i + 1);
    arrayZ[length - 1].~Type ();
    length--;
  }

  template <منطقي Sorted = sorted,
	    hb_enable_if (!Sorted)>
  فراغ remove_unordered (unsigned صحيح i)
  {
    אם (unlikely (i >= length))
      החזר;
    אם (i != length - 1)
      arrayZ[i] = std::move (arrayZ[length - 1]);
    arrayZ[length - 1].~Type ();
    length--;
  }

  فراغ shrink (صحيح size_, منطقي shrink_memory = אמת)
  {
    unsigned صحيح size = size_ < 0 ? 0u : (unsigned صحيح) size_;
    אם (size >= length)
      החזר;

    shrink_vector (size);

    אם (is_owned () && shrink_memory)
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
  { החזר as_array ().lsearch (x, not_found); }
  template <typename T>
  ثابت Type *lsearch (ثابت T &x, ثابت Type *not_found = nullptr) ثابت
  { החזר as_array ().lsearch (x, not_found); }
  template <typename T>
  منطقي lfind (ثابت T &x, unsigned *pos = nullptr) ثابت
  { החזר as_array ().lfind (x, pos); }

  /* Sorted search API. */
  template <typename T,
	    منطقي Sorted=sorted, hb_enable_if (Sorted)>
  Type *bsearch (ثابت T &x, Type *not_found = nullptr)
  { החזר as_array ().bsearch (x, not_found); }
  template <typename T,
	    منطقي Sorted=sorted, hb_enable_if (Sorted)>
  ثابت Type *bsearch (ثابت T &x, ثابت Type *not_found = nullptr) ثابت
  { החזר as_array ().bsearch (x, not_found); }
  template <typename T,
	    منطقي Sorted=sorted, hb_enable_if (Sorted)>
  منطقي bfind (ثابت T &x, unsigned صحيح *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned صحيح to_store = (unsigned صحيح) -1) ثابت
  { החזר as_array ().bfind (x, i, not_found, to_store); }
};

template <typename Type>
using hb_sorted_vector_t = hb_vector_t<Type, אמת>;

#endif /* HB_VECTOR_HH */

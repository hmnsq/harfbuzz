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

#અન્તર્ભાવ "hb.hh"
#અન્તર્ભાવ "hb-array.hh"
#અન્તર્ભાવ "hb-meta.hh"
#અન્તર્ભાવ "hb-null.hh"

// Change to 1 to force inline vector allocs, to see callsite in malloc-stats tool.
#જો 0
#define HB_ALWAYS_INLINE_VECTOR_ALLOCS HB_ALWAYS_INLINE
#નહીંતર
#define HB_ALWAYS_INLINE_VECTOR_ALLOCS
#endif

template <typename Type,
	  તાર્કિક sorted=ખોટું>
સંરચના hb_vector_t
{
  સ્થિર constexpr તાર્કિક realloc_move = સાચું;

  પ્રકારવ્યાખ્યા Type item_t;
  સ્થિર constexpr unsigned item_size = hb_static_size (Type);
  using array_t = typename std::conditional<sorted, hb_sorted_array_t<Type>, hb_array_t<Type>>::type;
  using c_array_t = typename std::conditional<sorted, hb_sorted_array_t<અચલ Type>, hb_array_t<અચલ Type>>::type;

  hb_vector_t () = મૂળભૂત;
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (std::initializer_list<Type> lst) : hb_vector_t ()
  {
    alloc (lst.size (), સાચું);
    માટે (auto&& item : lst)
      push (item);
  }
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  explicit hb_vector_t (અચલ Iterable &o) : hb_vector_t ()
  {
    extend (o, સાચું);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (અચલ hb_vector_t &o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    જો (unlikely (in_error ())) પાછા;
    copy_array (o.as_array ());
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (array_t o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    જો (unlikely (in_error ())) પાછા;
    copy_array (o);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (c_array_t o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    જો (unlikely (in_error ())) પાછા;
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
  શૂન્ય
  set_storage (Type (&array)[n])
  { set_storage (array, n); }
  શૂન્ય
  set_storage (hb_array_t<Type> array)
  { set_storage (array.arrayZ, array.length); }
  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T) &&
			  hb_is_trivially_destructible(T))>
  શૂન્ય
  set_storage (Type *array, unsigned n)
  {
    પ્રતિજ્ઞા (allocated == 0);
    પ્રતિજ્ઞા (length == 0);

    arrayZ = array;
    length = n;
  }

  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  શૂન્ય extend (અચલ Iterable &o, તાર્કિક exact=ખોટું)
  {
    auto iter = hb_iter (o);
    જો (iter.is_random_access_iterator || iter.has_fast_len)
    {
      જો (unlikely (!alloc (length + hb_len (iter), exact)))
	પાછા;
      unsigned ગણતરી = hb_len (iter);
      માટે (unsigned i = 0; i < ગણતરી; i++)
	ધકેલો_has_room (*iter++);
    }
    જ્યારે (iter)
    {
      જો (unlikely (!alloc (length + 1)))
        પાછા;
      unsigned room = allocated - length;
      માટે (unsigned i = 0; i < room && iter; i++)
	ધકેલો_has_room (*iter++);
    }
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  શૂન્ય extend (array_t o, તાર્કિક exact=ખોટું)
  { extend (c_array_t (o), exact); }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  શૂન્ય extend (c_array_t o, તાર્કિક exact=ખોટું)
  {
    /* o may point into our own storage, eg. v.extend (v.as_array ()).
     * alloc() may move that storage, so find o again afterwards. */
    તાર્કિક aliased = o.length && in_storage (*o.arrayZ);
    unsigned offset = aliased ? o.arrayZ - arrayZ : 0;
    alloc (length + o.length, exact);
    જો (unlikely (in_error ())) પાછા;
    જો (unlikely (aliased))
      o = {arrayZ + offset, o.length};
    copy_array (o);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  શૂન્ય extend (અચલ hb_vector_t &o, તાર્કિક exact=ખોટું)
  { extend (o.as_array (), exact); }

  જાહેર:
  પૂર્ણાંક allocated = 0; /* < 0 means allocation failed. */
  unsigned પૂર્ણાંક length = 0;
  જાહેર:
  Type *arrayZ = nullptr;

  શૂન્ય init ()
  {
    allocated = length = 0;
    arrayZ = nullptr;
  }
  શૂન્ય init0 ()
  {
  }

  શૂન્ય fini ()
  {
    જો (is_owned ())
    {
      shrink_vector (0);
      hb_free (arrayZ);
    }
    init ();
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t &reset ()
  {
    જો (unlikely (in_error ()))
      reset_error ();
    clear ();
    પાછા *આ;
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t &reset_if_error ()
  {
    જો (unlikely (in_error ()))
      reset ();
    પાછા *આ;
  }

  /* Transfer ownership of the backing storage to caller.
   * Returns nullptr if storage is not owned by this vector. */
  Type *
  steal (unsigned *len = nullptr, પૂર્ણાંક *allocated_out = nullptr)
  {
    જો (!is_owned ())
      પાછા nullptr;
    જો (len)
      *len = length;
    જો (allocated_out)
      *allocated_out = allocated;
    Type *p = arrayZ;
    init ();
    પાછા p;
  }

  /* Adopt a previously detached owned buffer. */
  શૂન્ય
  recycle_buffer (Type *buffer,
                  unsigned len,
                  પૂર્ણાંક allocated_len)
  {
    fini ();
    arrayZ = buffer;
    length = len;
    allocated = allocated_len;
  }

  friend શૂન્ય swap (hb_vector_t& a, hb_vector_t& b) noexcept
  {
    hb_swap (a.allocated, b.allocated);
    hb_swap (a.length, b.length);
    hb_swap (a.arrayZ, b.arrayZ);
  }

  hb_vector_t& operator = (અચલ hb_vector_t &o)
  {
    જો (આ == &o) પાછા *આ;

    reset ();
    alloc_exact (o.length);
    જો (unlikely (in_error ())) પાછા *આ;

    length = 0;
    copy_array (o.as_array ());

    પાછા *આ;
  }
  hb_vector_t& operator = (hb_vector_t &&o) noexcept
  {
    hb_swap (*આ, o);
    પાછા *આ;
  }

  hb_bytes_t as_bytes () અચલ
  { પાછા hb_bytes_t ((અચલ અક્ષર *) arrayZ, get_size ()); }

  તાર્કિક operator == (અચલ hb_vector_t &o) અચલ { પાછા as_array () == o.as_array (); }
  તાર્કિક operator != (અચલ hb_vector_t &o) અચલ { પાછા !(*આ == o); }
  uint32_t hash () અચલ { પાછા as_array ().hash (); }

  Type& operator [] (પૂર્ણાંક i_)
  {
    unsigned પૂર્ણાંક i = (unsigned પૂર્ણાંક) i_;
    જો (unlikely (i >= length))
      પાછા Crap (Type);
    પાછા arrayZ[i];
  }
  અચલ Type& operator [] (પૂર્ણાંક i_) અચલ
  {
    unsigned પૂર્ણાંક i = (unsigned પૂર્ણાંક) i_;
    જો (unlikely (i >= length))
      પાછા Null (Type);
    પાછા arrayZ[i];
  }

  Type& tail () { પાછા (*આ)[length - 1]; }
  અચલ Type& tail () અચલ { પાછા (*આ)[length - 1]; }

  explicit operator તાર્કિક () અચલ { પાછા length; }
  size_t get_size () અચલ { પાછા hb_unsigned_mul_saturate (length, item_size); }

  /* Sink interface. */
  template <typename T>
  hb_vector_t& operator << (T&& v) { push (std::forward<T> (v)); પાછા *આ; }

  array_t   as_array ()       { પાછા hb_array (arrayZ, length); }
  c_array_t as_array () અચલ { પાછા hb_array (arrayZ, length); }

  /* Iterator. */
  પ્રકારવ્યાખ્યા c_array_t   iter_t;
  પ્રકારવ્યાખ્યા array_t   writer_t;
    iter_t   iter () અચલ { પાછા as_array (); }
  writer_t writer ()       { પાછા as_array (); }
  operator   iter_t () અચલ { પાછા   iter (); }
  operator writer_t ()       { પાછા writer (); }

  /* Faster range-based for loop. */
  Type *begin () અચલ { પાછા arrayZ; }
  Type *end () અચલ { પાછા arrayZ + length; }


  hb_sorted_array_t<Type> as_sorted_array ()
  { પાછા hb_sorted_array (arrayZ, length); }
  hb_sorted_array_t<અચલ Type> as_sorted_array () અચલ
  { પાછા hb_sorted_array (arrayZ, length); }

  template <typename T> explicit operator T * () { પાછા arrayZ; }
  template <typename T> explicit operator અચલ T * () અચલ { પાછા arrayZ; }

  Type * operator  + (unsigned પૂર્ણાંક i) { પાછા arrayZ + i; }
  અચલ Type * operator  + (unsigned પૂર્ણાંક i) અચલ { પાછા arrayZ + i; }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push ()
  {
    જો (unlikely (!resize (length + 1)))
      પાછા std::addressof (Crap (Type));
    પાછા std::addressof (arrayZ[length - 1]);
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push (Args&&... args)
  {
    જો (unlikely ((પૂર્ણાંક) length >= allocated))
    {
      જો (unlikely (in_error ()))
	પાછા std::addressof (Crap (Type));

      /* An argument may live in our own storage, eg. v.push (v[i]).
       * alloc() frees that storage, so copy it out first. */
      જો (unlikely (in_storage (args...)))
      {
	Type copy (std::forward<Args> (args)...);
	જો (unlikely (!alloc (length + 1)))
	  પાછા std::addressof (Crap (Type));
	પાછા ધકેલો_has_room (std::move (copy));
      }
      જો (unlikely (!alloc (length + 1)))
	// If push failed to allocate then don't copy v, since this may cause
	// the created copy to leak memory since we won't have stored a
	// reference to it.
	પાછા std::addressof (Crap (Type));
    }

    પાછા ધકેલો_has_room (std::forward<Args> (args)...);
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  તાર્કિક ધકેલો_or_fail (Args&&... args)
  {
    પાછા push (std::forward<Args> (args)...) != std::addressof (Crap (Type));
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *ધકેલો_has_room (Args&&... args)
  {
    /* Emplace. */
    Type *p = std::addressof (arrayZ[length++]);
    પાછા નવું (p) Type (std::forward<Args> (args)...);
  }

  તાર્કિક in_storage () અચલ { પાછા ખોટું; }
  template <typename T, typename... Ts>
  તાર્કિક in_storage (અચલ T &v, અચલ Ts &...vs) અચલ
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    પાછા (arrayZ &&
	    (uintptr_t) arrayZ <= p && p < (uintptr_t) (arrayZ + length)) ||
	   in_storage (vs...);
  }

  તાર્કિક is_owned () અચલ
  {
    પાછા allocated != 0 && allocated != -1;
  }

  તાર્કિક in_error () અચલ { પાછા allocated < 0; }
  શૂન્ય set_error ()
  {
    પ્રતિજ્ઞા (allocated >= 0);
    allocated = -allocated - 1;
  }
  શૂન્ય reset_error ()
  {
    પ્રતિજ્ઞા (allocated < 0);
    allocated = -(allocated + 1);
  }
  શૂન્ય ensure_error ()
  {
    જો (!in_error ())
      set_error ();
  }

  Type *
  _realloc (unsigned new_allocated)
  {
    જો (!new_allocated)
    {
      જો (is_owned ())
	hb_free (arrayZ);
      પાછા nullptr;
    }
    જો (!allocated && arrayZ)
    {
      /* If we have a non-null arrayZ but allocated is 0, then we are
       * reallocating from a foreign array. */
      Type *new_array = (Type *) hb_malloc (new_allocated * કદ (Type));
      જો (unlikely (!new_array))
	પાછા nullptr;
      hb_memcpy ((શૂન્ય *) new_array, (અચલ શૂન્ય *) arrayZ, length * કદ (Type));
      પાછા new_array;
    }
    પાછા (Type *) hb_realloc (arrayZ, new_allocated * કદ (Type));
  }
  Type *
  _malloc_move (unsigned new_allocated)
  {
    જો (!new_allocated)
    {
      જો (is_owned ())
	hb_free (arrayZ);
      પાછા nullptr;
    }
    Type *new_array = (Type *) hb_malloc (new_allocated * કદ (Type));
    જો (likely (new_array))
    {
      માટે (unsigned i = 0; i < length; i++)
      {
	નવું (std::addressof (new_array[i])) Type ();
	new_array[i] = std::move (arrayZ[i]);
	arrayZ[i].~Type ();
      }
      જો (is_owned ())
	hb_free (arrayZ);
    }
    પાછા new_array;
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<0>)
  {
    પાછા _realloc (new_allocated);
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<0>)
  {
    પાછા _malloc_move (new_allocated);
  }
  /* Specialization for types that can be moved using realloc(). */
  template <typename T = Type,
	    hb_enable_if (T::realloc_move)>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<1>)
  {
    પાછા _realloc (new_allocated);
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T))>
  શૂન્ય
  grow_vector (unsigned size, hb_priority<0>)
  {
    hb_memset (arrayZ + length, 0, (size - length) * કદ (*arrayZ));
    length = size;
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_constructible(T))>
  શૂન્ય
  grow_vector (unsigned size, hb_priority<0>)
  {
    માટે (; length < size; length++)
      નવું (std::addressof (arrayZ[length])) Type ();
  }
  /* Specialization for hb_vector_t<hb_{vector,array}_t<U>> to speed up. */
  template <typename T = Type,
	    hb_enable_if (hb_is_same (T, hb_vector_t<typename T::item_t>) ||
			  hb_is_same (T, hb_array_t <typename T::item_t>))>
  શૂન્ય
  grow_vector (unsigned size, hb_priority<1>)
  {
    hb_memset (arrayZ + length, 0, (size - length) * કદ (*arrayZ));
    length = size;
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  શૂન્ય
  copy_array (hb_array_t<Type> other)
  {
    hb_memcpy ((શૂન્ય *) (arrayZ + length), (અચલ શૂન્ય *) other.arrayZ, other.length * item_size);
    length += other.length;
  }
  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  શૂન્ય
  copy_array (hb_array_t<અચલ Type> other)
  {
    hb_memcpy ((શૂન્ય *) (arrayZ + length), (અચલ શૂન્ય *) other.arrayZ, other.length * item_size);
    length += other.length;
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			   std::is_copy_constructible<T>::value)>
  શૂન્ય
  copy_array (hb_array_t<અચલ Type> other)
  {
    માટે (unsigned i = 0; i < other.length; i++)
      નવું (std::addressof (arrayZ[length + i])) Type (other.arrayZ[i]);
    length += other.length;
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			  !std::is_copy_constructible<T>::value &&
			  std::is_default_constructible<T>::value &&
			  std::is_copy_assignable<T>::value)>
  શૂન્ય
  copy_array (hb_array_t<અચલ Type> other)
  {
    માટે (unsigned i = 0; i < other.length; i++)
    {
      નવું (std::addressof (arrayZ[length + i])) Type ();
      arrayZ[length + i] = other.arrayZ[i];
    }
    length += other.length;
  }

  શૂન્ય
  shrink_vector (unsigned size)
  {
    પ્રતિજ્ઞા (size <= length);
    જો (!hb_is_trivially_destructible(Type))
    {
      unsigned ગણતરી = length - size;
      Type *p = arrayZ + length;
      જ્યારે (ગણતરી--)
        (--p)->~Type ();
    }
    length = size;
  }

  શૂન્ય
  shift_down_vector (unsigned i)
  {
    માટે (; i < length; i++)
      arrayZ[i - 1] = std::move (arrayZ[i]);
  }

  /* Allocate for size but don't adjust length. */
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  તાર્કિક alloc (unsigned પૂર્ણાંક size, તાર્કિક exact=ખોટું)
  {
    જો (unlikely (in_error ()))
      પાછા ખોટું;

    unsigned પૂર્ણાંક new_allocated;
    જો (exact)
    {
      /* If exact was specified, we allow shrinking the storage. */
      size = hb_max (size, length);
      જો (size <= (unsigned) allocated &&
	  size >= (unsigned) allocated >> 2)
	પાછા સાચું;

      new_allocated = size;
    }
    નહીંતર
    {
      જો (likely (size <= (unsigned) allocated))
	પાછા સાચું;

      new_allocated = allocated;
      જ્યારે (size > new_allocated)
	new_allocated += (new_allocated >> 1) + 8;
    }

    /* Reallocate */

    તાર્કિક overflows =
      (પૂર્ણાંક) in_error () ||
      (new_allocated < size) ||
      hb_unsigned_mul_overflows (new_allocated, કદ (Type));

    જો (unlikely (overflows))
    {
      set_error ();
      પાછા ખોટું;
    }

    Type *new_array = realloc_vector (new_allocated, hb_prioritize);

    જો (unlikely (new_allocated && !new_array))
    {
      જો (new_allocated <= (unsigned) allocated)
        પાછા સાચું; // shrinking failed; it's okay; happens in our fuzzer

      set_error ();
      પાછા ખોટું;
    }

    arrayZ = new_array;
    allocated = new_allocated;

    પાછા સાચું;
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  તાર્કિક alloc_exact (unsigned પૂર્ણાંક size)
  {
    પાછા alloc (size, સાચું);
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  શૂન્ય clear ()
  {
    shrink_vector (0);
  }

  template <typename allocator_t>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  તાર્કિક allocate_from_pool (allocator_t *allocator, unsigned size, unsigned પૂર્ણાંક initialize = સાચું)
  {
    જો (allocator)
    {
      પ્રતિજ્ઞા (!length && !allocated);
      arrayZ = (Type *) allocator->alloc (size * કદ (Type), alignof (Type));
      જો (unlikely (!arrayZ))
      {
	set_error ();
	પાછા ખોટું;
      }
      જો (initialize)
	grow_vector (size, hb_prioritize);
      નહીંતર
	length = size;
      પાછા સાચું;
    }
    પાછા resize_full ((પૂર્ણાંક) size, initialize, સાચું);
  }

  template <typename allocator_t>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  તાર્કિક duplicate_vector_from_pool (allocator_t *allocator, અચલ hb_vector_t &other)
  {
    જો (unlikely (!allocate_from_pool (allocator, other.length, ખોટું)))
      પાછા ખોટું;
    length = 0;
    copy_array (other.as_array ());
    પાછા સાચું;
  }

  template <typename allocator_t>
  શૂન્ય shrink_back_to_pool (allocator_t *allocator, પૂર્ણાંક size)
  {
    unsigned orig_length = length;

    shrink (size, ખોટું);

    જો (allocator && !is_owned ())
      allocator->discard (arrayZ + length, (orig_length - length) * કદ (Type));
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  તાર્કિક resize_full (પૂર્ણાંક size_, તાર્કિક initialize, તાર્કિક exact)
  {
    જો (unlikely (size_ < 0))
      પાછા ખોટું;
    unsigned પૂર્ણાંક size = (unsigned પૂર્ણાંક) size_;
    જો (!alloc (size, exact))
      પાછા ખોટું;

    જો (size > length)
    {
      જો (initialize)
	grow_vector (size, hb_prioritize);
    }
    નહીંતર જો (size < length)
    {
      જો (initialize)
	shrink_vector (size);
    }

    length = size;
    પાછા સાચું;
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  તાર્કિક resize (પૂર્ણાંક size_)
  {
    પાછા resize_full (size_, સાચું, ખોટું);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  તાર્કિક resize_dirty (પૂર્ણાંક size_)
  {
    પાછા resize_full (size_, ખોટું, ખોટું);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  તાર્કિક resize_exact (પૂર્ણાંક size_)
  {
    પાછા resize_full (size_, સાચું, સાચું);
  }

  Type કાઢો ()
  {
    જો (!length) પાછા Null (Type);
    Type v (std::move (arrayZ[length - 1]));
    arrayZ[length - 1].~Type ();
    length--;
    પાછા v;
  }

  શૂન્ય remove_ordered (unsigned પૂર્ણાંક i)
  {
    જો (unlikely (i >= length))
      પાછા;
    shift_down_vector (i + 1);
    arrayZ[length - 1].~Type ();
    length--;
  }

  template <તાર્કિક Sorted = sorted,
	    hb_enable_if (!Sorted)>
  શૂન્ય remove_unordered (unsigned પૂર્ણાંક i)
  {
    જો (unlikely (i >= length))
      પાછા;
    જો (i != length - 1)
      arrayZ[i] = std::move (arrayZ[length - 1]);
    arrayZ[length - 1].~Type ();
    length--;
  }

  શૂન્ય shrink (પૂર્ણાંક size_, તાર્કિક shrink_memory = સાચું)
  {
    unsigned પૂર્ણાંક size = size_ < 0 ? 0u : (unsigned પૂર્ણાંક) size_;
    જો (size >= length)
      પાછા;

    shrink_vector (size);

    જો (is_owned () && shrink_memory)
      alloc_exact (size); /* To force shrinking memory if needed. */
  }


  /* Sorting API. */
  template <typename Compar>
  શૂન્ય qsort (Compar compar)
  { as_array ().qsort (compar); }
  શૂન્ય qsort ()
  { as_array ().qsort (); }

  /* Unsorted search API. */
  template <typename T>
  Type *lsearch (અચલ T &x, Type *not_found = nullptr)
  { પાછા as_array ().lsearch (x, not_found); }
  template <typename T>
  અચલ Type *lsearch (અચલ T &x, અચલ Type *not_found = nullptr) અચલ
  { પાછા as_array ().lsearch (x, not_found); }
  template <typename T>
  તાર્કિક lfind (અચલ T &x, unsigned *pos = nullptr) અચલ
  { પાછા as_array ().lfind (x, pos); }

  /* Sorted search API. */
  template <typename T,
	    તાર્કિક Sorted=sorted, hb_enable_if (Sorted)>
  Type *bsearch (અચલ T &x, Type *not_found = nullptr)
  { પાછા as_array ().bsearch (x, not_found); }
  template <typename T,
	    તાર્કિક Sorted=sorted, hb_enable_if (Sorted)>
  અચલ Type *bsearch (અચલ T &x, અચલ Type *not_found = nullptr) અચલ
  { પાછા as_array ().bsearch (x, not_found); }
  template <typename T,
	    તાર્કિક Sorted=sorted, hb_enable_if (Sorted)>
  તાર્કિક bfind (અચલ T &x, unsigned પૂર્ણાંક *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned પૂર્ણાંક to_store = (unsigned પૂર્ણાંક) -1) અચલ
  { પાછા as_array ().bfind (x, i, not_found, to_store); }
};

template <typename Type>
using hb_sorted_vector_t = hb_vector_t<Type, સાચું>;

#endif /* HB_VECTOR_HH */

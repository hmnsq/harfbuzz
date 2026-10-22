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

#include "hb.hh"
#include "hb-array.hh"
#include "hb-meta.hh"
#include "hb-null.hh"

// Change to 1 to force inline vector allocs, to see callsite in malloc-stats tool.
#නම් 0
#define HB_ALWAYS_INLINE_VECTOR_ALLOCS HB_ALWAYS_INLINE
#නැතහොත්
#define HB_ALWAYS_INLINE_VECTOR_ALLOCS
#endif

template <typename Type,
	  bool sorted=false>
sa_nracanaa hb_vector_t
{
  static constexpr bool realloc_move = true;

  typedef Type item_t;
  static constexpr unsigned item_size = hb_static_size (Type);
  using array_t = typename std::conditional<sorted, hb_sorted_array_t<Type>, hb_array_t<Type>>::type;
  using c_array_t = typename std::conditional<sorted, hb_sorted_array_t<const Type>, hb_array_t<const Type>>::type;

  hb_vector_t () = default;
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (std::initializer_list<Type> lst) : hb_vector_t ()
  {
    alloc (lst.size (), true);
    සඳහා (auto&& item : lst)
      push (item);
  }
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  explicit hb_vector_t (const Iterable &o) : hb_vector_t ()
  {
    extend (o, true);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (const hb_vector_t &o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    නම් (unlikely (in_error ())) ආපසු;
    copy_array (o.as_array ());
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (array_t o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    නම් (unlikely (in_error ())) ආපසු;
    copy_array (o);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (c_array_t o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    නම් (unlikely (in_error ())) ආපසු;
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
  void
  set_storage (Type (&array)[n])
  { set_storage (array, n); }
  void
  set_storage (hb_array_t<Type> array)
  { set_storage (array.arrayZ, array.length); }
  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T) &&
			  hb_is_trivially_destructible(T))>
  void
  set_storage (Type *array, unsigned n)
  {
    assert (allocated == 0);
    assert (length == 0);

    arrayZ = array;
    length = n;
  }

  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  void extend (const Iterable &o, bool exact=false)
  {
    auto iter = hb_iter (o);
    නම් (iter.is_random_access_iterator || iter.has_fast_len)
    {
      නම් (unlikely (!alloc (length + hb_len (iter), exact)))
	ආපසු;
      unsigned count = hb_len (iter);
      සඳහා (unsigned i = 0; i < count; i++)
	push_has_room (*iter++);
    }
    අතරතුර (iter)
    {
      නම් (unlikely (!alloc (length + 1)))
        ආපසු;
      unsigned room = allocated - length;
      සඳහා (unsigned i = 0; i < room && iter; i++)
	push_has_room (*iter++);
    }
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  void extend (array_t o, bool exact=false)
  { extend (c_array_t (o), exact); }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  void extend (c_array_t o, bool exact=false)
  {
    /* o may point into our own storage, eg. v.extend (v.as_array ()).
     * alloc() may move that storage, so find o again afterwards. */
    bool aliased = o.length && in_storage (*o.arrayZ);
    unsigned offset = aliased ? o.arrayZ - arrayZ : 0;
    alloc (length + o.length, exact);
    නම් (unlikely (in_error ())) ආපසු;
    නම් (unlikely (aliased))
      o = {arrayZ + offset, o.length};
    copy_array (o);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  void extend (const hb_vector_t &o, bool exact=false)
  { extend (o.as_array (), exact); }

  public:
  පූර්ණ_සංඛ්‍යා allocated = 0; /* < 0 means allocation failed. */
  unsigned පූර්ණ_සංඛ්‍යා length = 0;
  public:
  Type *arrayZ = nullptr;

  void init ()
  {
    allocated = length = 0;
    arrayZ = nullptr;
  }
  void init0 ()
  {
  }

  void fini ()
  {
    නම් (is_owned ())
    {
      shrink_vector (0);
      hb_free (arrayZ);
    }
    init ();
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t &reset ()
  {
    නම් (unlikely (in_error ()))
      reset_error ();
    clear ();
    ආපසු *this;
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t &reset_if_error ()
  {
    නම් (unlikely (in_error ()))
      reset ();
    ආපසු *this;
  }

  /* Transfer ownership of the backing storage to caller.
   * Returns nullptr if storage is not owned by this vector. */
  Type *
  steal (unsigned *len = nullptr, පූර්ණ_සංඛ්‍යා *allocated_out = nullptr)
  {
    නම් (!is_owned ())
      ආපසු nullptr;
    නම් (len)
      *len = length;
    නම් (allocated_out)
      *allocated_out = allocated;
    Type *p = arrayZ;
    init ();
    ආපසු p;
  }

  /* Adopt a previously detached owned buffer. */
  void
  recycle_buffer (Type *buffer,
                  unsigned len,
                  පූර්ණ_සංඛ්‍යා allocated_len)
  {
    fini ();
    arrayZ = buffer;
    length = len;
    allocated = allocated_len;
  }

  friend void swap (hb_vector_t& a, hb_vector_t& b) noexcept
  {
    hb_swap (a.allocated, b.allocated);
    hb_swap (a.length, b.length);
    hb_swap (a.arrayZ, b.arrayZ);
  }

  hb_vector_t& operator = (const hb_vector_t &o)
  {
    නම් (this == &o) ආපසු *this;

    reset ();
    alloc_exact (o.length);
    නම් (unlikely (in_error ())) ආපසු *this;

    length = 0;
    copy_array (o.as_array ());

    ආපසු *this;
  }
  hb_vector_t& operator = (hb_vector_t &&o) noexcept
  {
    hb_swap (*this, o);
    ආපසු *this;
  }

  hb_bytes_t as_bytes () const
  { ආපසු hb_bytes_t ((const var_na *) arrayZ, get_size ()); }

  bool operator == (const hb_vector_t &o) const { ආපසු as_array () == o.as_array (); }
  bool operator != (const hb_vector_t &o) const { ආපසු !(*this == o); }
  uint32_t hash () const { ආපසු as_array ().hash (); }

  Type& operator [] (පූර්ණ_සංඛ්‍යා i_)
  {
    unsigned පූර්ණ_සංඛ්‍යා i = (unsigned පූර්ණ_සංඛ්‍යා) i_;
    නම් (unlikely (i >= length))
      ආපසු Crap (Type);
    ආපසු arrayZ[i];
  }
  const Type& operator [] (පූර්ණ_සංඛ්‍යා i_) const
  {
    unsigned පූර්ණ_සංඛ්‍යා i = (unsigned පූර්ණ_සංඛ්‍යා) i_;
    නම් (unlikely (i >= length))
      ආපසු Null (Type);
    ආපසු arrayZ[i];
  }

  Type& tail () { ආපසු (*this)[length - 1]; }
  const Type& tail () const { ආපසු (*this)[length - 1]; }

  explicit operator bool () const { ආපසු length; }
  size_t get_size () const { ආපසු hb_unsigned_mul_saturate (length, item_size); }

  /* Sink interface. */
  template <typename T>
  hb_vector_t& operator << (T&& v) { push (std::forward<T> (v)); ආපසු *this; }

  array_t   as_array ()       { ආපසු hb_array (arrayZ, length); }
  c_array_t as_array () const { ආපසු hb_array (arrayZ, length); }

  /* Iterator. */
  typedef c_array_t   iter_t;
  typedef array_t   writer_t;
    iter_t   iter () const { ආපසු as_array (); }
  writer_t writer ()       { ආපසු as_array (); }
  operator   iter_t () const { ආපසු   iter (); }
  operator writer_t ()       { ආපසු writer (); }

  /* Faster range-based for loop. */
  Type *begin () const { ආපසු arrayZ; }
  Type *end () const { ආපසු arrayZ + length; }


  hb_sorted_array_t<Type> as_sorted_array ()
  { ආපසු hb_sorted_array (arrayZ, length); }
  hb_sorted_array_t<const Type> as_sorted_array () const
  { ආපසු hb_sorted_array (arrayZ, length); }

  template <typename T> explicit operator T * () { ආපසු arrayZ; }
  template <typename T> explicit operator const T * () const { ආපසු arrayZ; }

  Type * operator  + (unsigned පූර්ණ_සංඛ්‍යා i) { ආපසු arrayZ + i; }
  const Type * operator  + (unsigned පූර්ණ_සංඛ්‍යා i) const { ආපසු arrayZ + i; }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push ()
  {
    නම් (unlikely (!resize (length + 1)))
      ආපසු std::addressof (Crap (Type));
    ආපසු std::addressof (arrayZ[length - 1]);
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push (Args&&... args)
  {
    නම් (unlikely ((පූර්ණ_සංඛ්‍යා) length >= allocated))
    {
      නම් (unlikely (in_error ()))
	ආපසු std::addressof (Crap (Type));

      /* An argument may live in our own storage, eg. v.push (v[i]).
       * alloc() frees that storage, so copy it out first. */
      නම් (unlikely (in_storage (args...)))
      {
	Type copy (std::forward<Args> (args)...);
	නම් (unlikely (!alloc (length + 1)))
	  ආපසු std::addressof (Crap (Type));
	ආපසු push_has_room (std::move (copy));
      }
      නම් (unlikely (!alloc (length + 1)))
	// If push failed to allocate then don't copy v, since this may cause
	// the created copy to leak memory since we won't have stored a
	// reference to it.
	ආපසු std::addressof (Crap (Type));
    }

    ආපසු push_has_room (std::forward<Args> (args)...);
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  bool push_or_fail (Args&&... args)
  {
    ආපසු push (std::forward<Args> (args)...) != std::addressof (Crap (Type));
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push_has_room (Args&&... args)
  {
    /* Emplace. */
    Type *p = std::addressof (arrayZ[length++]);
    ආපසු new (p) Type (std::forward<Args> (args)...);
  }

  bool in_storage () const { ආපසු false; }
  template <typename T, typename... Ts>
  bool in_storage (const T &v, const Ts &...vs) const
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    ආපසු (arrayZ &&
	    (uintptr_t) arrayZ <= p && p < (uintptr_t) (arrayZ + length)) ||
	   in_storage (vs...);
  }

  bool is_owned () const
  {
    ආපසු allocated != 0 && allocated != -1;
  }

  bool in_error () const { ආපසු allocated < 0; }
  void set_error ()
  {
    assert (allocated >= 0);
    allocated = -allocated - 1;
  }
  void reset_error ()
  {
    assert (allocated < 0);
    allocated = -(allocated + 1);
  }
  void ensure_error ()
  {
    නම් (!in_error ())
      set_error ();
  }

  Type *
  _realloc (unsigned new_allocated)
  {
    නම් (!new_allocated)
    {
      නම් (is_owned ())
	hb_free (arrayZ);
      ආපසු nullptr;
    }
    නම් (!allocated && arrayZ)
    {
      /* If we have a non-null arrayZ but allocated is 0, then we are
       * reallocating from a foreign array. */
      Type *new_array = (Type *) hb_malloc (new_allocated * parimaa_na (Type));
      නම් (unlikely (!new_array))
	ආපසු nullptr;
      hb_memcpy ((void *) new_array, (const void *) arrayZ, length * parimaa_na (Type));
      ආපසු new_array;
    }
    ආපසු (Type *) hb_realloc (arrayZ, new_allocated * parimaa_na (Type));
  }
  Type *
  _malloc_move (unsigned new_allocated)
  {
    නම් (!new_allocated)
    {
      නම් (is_owned ())
	hb_free (arrayZ);
      ආපසු nullptr;
    }
    Type *new_array = (Type *) hb_malloc (new_allocated * parimaa_na (Type));
    නම් (likely (new_array))
    {
      සඳහා (unsigned i = 0; i < length; i++)
      {
	new (std::addressof (new_array[i])) Type ();
	new_array[i] = std::move (arrayZ[i]);
	arrayZ[i].~Type ();
      }
      නම් (is_owned ())
	hb_free (arrayZ);
    }
    ආපසු new_array;
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<0>)
  {
    ආපසු _realloc (new_allocated);
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<0>)
  {
    ආපසු _malloc_move (new_allocated);
  }
  /* Specialization for types that can be moved using realloc(). */
  template <typename T = Type,
	    hb_enable_if (T::realloc_move)>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<1>)
  {
    ආපසු _realloc (new_allocated);
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T))>
  void
  grow_vector (unsigned size, hb_priority<0>)
  {
    hb_memset (arrayZ + length, 0, (size - length) * parimaa_na (*arrayZ));
    length = size;
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_constructible(T))>
  void
  grow_vector (unsigned size, hb_priority<0>)
  {
    සඳහා (; length < size; length++)
      new (std::addressof (arrayZ[length])) Type ();
  }
  /* Specialization for hb_vector_t<hb_{vector,array}_t<U>> to speed up. */
  template <typename T = Type,
	    hb_enable_if (hb_is_same (T, hb_vector_t<typename T::item_t>) ||
			  hb_is_same (T, hb_array_t <typename T::item_t>))>
  void
  grow_vector (unsigned size, hb_priority<1>)
  {
    hb_memset (arrayZ + length, 0, (size - length) * parimaa_na (*arrayZ));
    length = size;
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  void
  copy_array (hb_array_t<Type> other)
  {
    hb_memcpy ((void *) (arrayZ + length), (const void *) other.arrayZ, other.length * item_size);
    length += other.length;
  }
  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  void
  copy_array (hb_array_t<const Type> other)
  {
    hb_memcpy ((void *) (arrayZ + length), (const void *) other.arrayZ, other.length * item_size);
    length += other.length;
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			   std::is_copy_constructible<T>::value)>
  void
  copy_array (hb_array_t<const Type> other)
  {
    සඳහා (unsigned i = 0; i < other.length; i++)
      new (std::addressof (arrayZ[length + i])) Type (other.arrayZ[i]);
    length += other.length;
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			  !std::is_copy_constructible<T>::value &&
			  std::is_default_constructible<T>::value &&
			  std::is_copy_assignable<T>::value)>
  void
  copy_array (hb_array_t<const Type> other)
  {
    සඳහා (unsigned i = 0; i < other.length; i++)
    {
      new (std::addressof (arrayZ[length + i])) Type ();
      arrayZ[length + i] = other.arrayZ[i];
    }
    length += other.length;
  }

  void
  shrink_vector (unsigned size)
  {
    assert (size <= length);
    නම් (!hb_is_trivially_destructible(Type))
    {
      unsigned count = length - size;
      Type *p = arrayZ + length;
      අතරතුර (count--)
        (--p)->~Type ();
    }
    length = size;
  }

  void
  shift_down_vector (unsigned i)
  {
    සඳහා (; i < length; i++)
      arrayZ[i - 1] = std::move (arrayZ[i]);
  }

  /* Allocate for size but don't adjust length. */
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  bool alloc (unsigned පූර්ණ_සංඛ්‍යා size, bool exact=false)
  {
    නම් (unlikely (in_error ()))
      ආපසු false;

    unsigned පූර්ණ_සංඛ්‍යා new_allocated;
    නම් (exact)
    {
      /* If exact was specified, we allow shrinking the storage. */
      size = hb_max (size, length);
      නම් (size <= (unsigned) allocated &&
	  size >= (unsigned) allocated >> 2)
	ආපසු true;

      new_allocated = size;
    }
    නැතහොත්
    {
      නම් (likely (size <= (unsigned) allocated))
	ආපසු true;

      new_allocated = allocated;
      අතරතුර (size > new_allocated)
	new_allocated += (new_allocated >> 1) + 8;
    }

    /* Reallocate */

    bool overflows =
      (පූර්ණ_සංඛ්‍යා) in_error () ||
      (new_allocated < size) ||
      hb_unsigned_mul_overflows (new_allocated, parimaa_na (Type));

    නම් (unlikely (overflows))
    {
      set_error ();
      ආපසු false;
    }

    Type *new_array = realloc_vector (new_allocated, hb_prioritize);

    නම් (unlikely (new_allocated && !new_array))
    {
      නම් (new_allocated <= (unsigned) allocated)
        ආපසු true; // shrinking failed; it's okay; happens in our fuzzer

      set_error ();
      ආපසු false;
    }

    arrayZ = new_array;
    allocated = new_allocated;

    ආපසු true;
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  bool alloc_exact (unsigned පූර්ණ_සංඛ්‍යා size)
  {
    ආපසු alloc (size, true);
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  void clear ()
  {
    shrink_vector (0);
  }

  template <typename allocator_t>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  bool allocate_from_pool (allocator_t *allocator, unsigned size, unsigned පූර්ණ_සංඛ්‍යා initialize = true)
  {
    නම් (allocator)
    {
      assert (!length && !allocated);
      arrayZ = (Type *) allocator->alloc (size * parimaa_na (Type), alignof (Type));
      නම් (unlikely (!arrayZ))
      {
	set_error ();
	ආපසු false;
      }
      නම් (initialize)
	grow_vector (size, hb_prioritize);
      නැතහොත්
	length = size;
      ආපසු true;
    }
    ආපසු resize_full ((පූර්ණ_සංඛ්‍යා) size, initialize, true);
  }

  template <typename allocator_t>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  bool duplicate_vector_from_pool (allocator_t *allocator, const hb_vector_t &other)
  {
    නම් (unlikely (!allocate_from_pool (allocator, other.length, false)))
      ආපසු false;
    length = 0;
    copy_array (other.as_array ());
    ආපසු true;
  }

  template <typename allocator_t>
  void shrink_back_to_pool (allocator_t *allocator, පූර්ණ_සංඛ්‍යා size)
  {
    unsigned orig_length = length;

    shrink (size, false);

    නම් (allocator && !is_owned ())
      allocator->discard (arrayZ + length, (orig_length - length) * parimaa_na (Type));
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  bool resize_full (පූර්ණ_සංඛ්‍යා size_, bool initialize, bool exact)
  {
    නම් (unlikely (size_ < 0))
      ආපසු false;
    unsigned පූර්ණ_සංඛ්‍යා size = (unsigned පූර්ණ_සංඛ්‍යා) size_;
    නම් (!alloc (size, exact))
      ආපසු false;

    නම් (size > length)
    {
      නම් (initialize)
	grow_vector (size, hb_prioritize);
    }
    නැතහොත් නම් (size < length)
    {
      නම් (initialize)
	shrink_vector (size);
    }

    length = size;
    ආපසු true;
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  bool resize (පූර්ණ_සංඛ්‍යා size_)
  {
    ආපසු resize_full (size_, true, false);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  bool resize_dirty (පූර්ණ_සංඛ්‍යා size_)
  {
    ආපසු resize_full (size_, false, false);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  bool resize_exact (පූර්ණ_සංඛ්‍යා size_)
  {
    ආපසු resize_full (size_, true, true);
  }

  Type pop ()
  {
    නම් (!length) ආපසු Null (Type);
    Type v (std::move (arrayZ[length - 1]));
    arrayZ[length - 1].~Type ();
    length--;
    ආපසු v;
  }

  void remove_ordered (unsigned පූර්ණ_සංඛ්‍යා i)
  {
    නම් (unlikely (i >= length))
      ආපසු;
    shift_down_vector (i + 1);
    arrayZ[length - 1].~Type ();
    length--;
  }

  template <bool Sorted = sorted,
	    hb_enable_if (!Sorted)>
  void remove_unordered (unsigned පූර්ණ_සංඛ්‍යා i)
  {
    නම් (unlikely (i >= length))
      ආපසු;
    නම් (i != length - 1)
      arrayZ[i] = std::move (arrayZ[length - 1]);
    arrayZ[length - 1].~Type ();
    length--;
  }

  void shrink (පූර්ණ_සංඛ්‍යා size_, bool shrink_memory = true)
  {
    unsigned පූර්ණ_සංඛ්‍යා size = size_ < 0 ? 0u : (unsigned පූර්ණ_සංඛ්‍යා) size_;
    නම් (size >= length)
      ආපසු;

    shrink_vector (size);

    නම් (is_owned () && shrink_memory)
      alloc_exact (size); /* To force shrinking memory if needed. */
  }


  /* Sorting API. */
  template <typename Compar>
  void qsort (Compar compar)
  { as_array ().qsort (compar); }
  void qsort ()
  { as_array ().qsort (); }

  /* Unsorted search API. */
  template <typename T>
  Type *lsearch (const T &x, Type *not_found = nullptr)
  { ආපසු as_array ().lsearch (x, not_found); }
  template <typename T>
  const Type *lsearch (const T &x, const Type *not_found = nullptr) const
  { ආපසු as_array ().lsearch (x, not_found); }
  template <typename T>
  bool lfind (const T &x, unsigned *pos = nullptr) const
  { ආපසු as_array ().lfind (x, pos); }

  /* Sorted search API. */
  template <typename T,
	    bool Sorted=sorted, hb_enable_if (Sorted)>
  Type *bsearch (const T &x, Type *not_found = nullptr)
  { ආපසු as_array ().bsearch (x, not_found); }
  template <typename T,
	    bool Sorted=sorted, hb_enable_if (Sorted)>
  const Type *bsearch (const T &x, const Type *not_found = nullptr) const
  { ආපසු as_array ().bsearch (x, not_found); }
  template <typename T,
	    bool Sorted=sorted, hb_enable_if (Sorted)>
  bool bfind (const T &x, unsigned පූර්ණ_සංඛ්‍යා *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned පූර්ණ_සංඛ්‍යා to_store = (unsigned පූර්ණ_සංඛ්‍යා) -1) const
  { ආපසු as_array ().bfind (x, i, not_found, to_store); }
};

template <typename Type>
using hb_sorted_vector_t = hb_vector_t<Type, true>;

#endif /* HB_VECTOR_HH */

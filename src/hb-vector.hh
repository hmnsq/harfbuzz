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
#ꦪꦼꦤ꧀ 0
#define HB_ALWAYS_INLINE_VECTOR_ALLOCS HB_ALWAYS_INLINE
#ꦭꦶꦪ
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
    ꦏꦁꦒꦺꦴ (auto&& item : lst)
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
    ꦪꦼꦤ꧀ (unlikely (in_error ())) ꦮꦭꦶ;
    copy_array (o.as_array ());
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (array_t o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    ꦪꦼꦤ꧀ (unlikely (in_error ())) ꦮꦭꦶ;
    copy_array (o);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (c_array_t o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    ꦪꦼꦤ꧀ (unlikely (in_error ())) ꦮꦭꦶ;
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
    ꦪꦼꦤ꧀ (iter.is_random_access_iterator || iter.has_fast_len)
    {
      ꦪꦼꦤ꧀ (unlikely (!alloc (length + hb_len (iter), exact)))
	ꦮꦭꦶ;
      unsigned count = hb_len (iter);
      ꦏꦁꦒꦺꦴ (unsigned i = 0; i < count; i++)
	push_has_room (*iter++);
    }
    ꦱꦼꦢꦼꦭ (iter)
    {
      ꦪꦼꦤ꧀ (unlikely (!alloc (length + 1)))
        ꦮꦭꦶ;
      unsigned room = allocated - length;
      ꦏꦁꦒꦺꦴ (unsigned i = 0; i < room && iter; i++)
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
    ꦪꦼꦤ꧀ (unlikely (in_error ())) ꦮꦭꦶ;
    ꦪꦼꦤ꧀ (unlikely (aliased))
      o = {arrayZ + offset, o.length};
    copy_array (o);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  void extend (const hb_vector_t &o, bool exact=false)
  { extend (o.as_array (), exact); }

  public:
  puur_naa_nka allocated = 0; /* < 0 means allocation failed. */
  unsigned puur_naa_nka length = 0;
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
    ꦪꦼꦤ꧀ (is_owned ())
    {
      shrink_vector (0);
      hb_free (arrayZ);
    }
    init ();
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t &reset ()
  {
    ꦪꦼꦤ꧀ (unlikely (in_error ()))
      reset_error ();
    clear ();
    ꦮꦭꦶ *this;
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t &reset_if_error ()
  {
    ꦪꦼꦤ꧀ (unlikely (in_error ()))
      reset ();
    ꦮꦭꦶ *this;
  }

  /* Transfer ownership of the backing storage to caller.
   * Returns nullptr if storage is not owned by this vector. */
  Type *
  steal (unsigned *len = nullptr, puur_naa_nka *allocated_out = nullptr)
  {
    ꦪꦼꦤ꧀ (!is_owned ())
      ꦮꦭꦶ nullptr;
    ꦪꦼꦤ꧀ (len)
      *len = length;
    ꦪꦼꦤ꧀ (allocated_out)
      *allocated_out = allocated;
    Type *p = arrayZ;
    init ();
    ꦮꦭꦶ p;
  }

  /* Adopt a previously detached owned buffer. */
  void
  recycle_buffer (Type *buffer,
                  unsigned len,
                  puur_naa_nka allocated_len)
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
    ꦪꦼꦤ꧀ (this == &o) ꦮꦭꦶ *this;

    reset ();
    alloc_exact (o.length);
    ꦪꦼꦤ꧀ (unlikely (in_error ())) ꦮꦭꦶ *this;

    length = 0;
    copy_array (o.as_array ());

    ꦮꦭꦶ *this;
  }
  hb_vector_t& operator = (hb_vector_t &&o) noexcept
  {
    hb_swap (*this, o);
    ꦮꦭꦶ *this;
  }

  hb_bytes_t as_bytes () const
  { ꦮꦭꦶ hb_bytes_t ((const var_na *) arrayZ, get_size ()); }

  bool operator == (const hb_vector_t &o) const { ꦮꦭꦶ as_array () == o.as_array (); }
  bool operator != (const hb_vector_t &o) const { ꦮꦭꦶ !(*this == o); }
  uint32_t hash () const { ꦮꦭꦶ as_array ().hash (); }

  Type& operator [] (puur_naa_nka i_)
  {
    unsigned puur_naa_nka i = (unsigned puur_naa_nka) i_;
    ꦪꦼꦤ꧀ (unlikely (i >= length))
      ꦮꦭꦶ Crap (Type);
    ꦮꦭꦶ arrayZ[i];
  }
  const Type& operator [] (puur_naa_nka i_) const
  {
    unsigned puur_naa_nka i = (unsigned puur_naa_nka) i_;
    ꦪꦼꦤ꧀ (unlikely (i >= length))
      ꦮꦭꦶ Null (Type);
    ꦮꦭꦶ arrayZ[i];
  }

  Type& tail () { ꦮꦭꦶ (*this)[length - 1]; }
  const Type& tail () const { ꦮꦭꦶ (*this)[length - 1]; }

  explicit operator bool () const { ꦮꦭꦶ length; }
  size_t get_size () const { ꦮꦭꦶ hb_unsigned_mul_saturate (length, item_size); }

  /* Sink interface. */
  template <typename T>
  hb_vector_t& operator << (T&& v) { push (std::forward<T> (v)); ꦮꦭꦶ *this; }

  array_t   as_array ()       { ꦮꦭꦶ hb_array (arrayZ, length); }
  c_array_t as_array () const { ꦮꦭꦶ hb_array (arrayZ, length); }

  /* Iterator. */
  typedef c_array_t   iter_t;
  typedef array_t   writer_t;
    iter_t   iter () const { ꦮꦭꦶ as_array (); }
  writer_t writer ()       { ꦮꦭꦶ as_array (); }
  operator   iter_t () const { ꦮꦭꦶ   iter (); }
  operator writer_t ()       { ꦮꦭꦶ writer (); }

  /* Faster range-based for loop. */
  Type *begin () const { ꦮꦭꦶ arrayZ; }
  Type *end () const { ꦮꦭꦶ arrayZ + length; }


  hb_sorted_array_t<Type> as_sorted_array ()
  { ꦮꦭꦶ hb_sorted_array (arrayZ, length); }
  hb_sorted_array_t<const Type> as_sorted_array () const
  { ꦮꦭꦶ hb_sorted_array (arrayZ, length); }

  template <typename T> explicit operator T * () { ꦮꦭꦶ arrayZ; }
  template <typename T> explicit operator const T * () const { ꦮꦭꦶ arrayZ; }

  Type * operator  + (unsigned puur_naa_nka i) { ꦮꦭꦶ arrayZ + i; }
  const Type * operator  + (unsigned puur_naa_nka i) const { ꦮꦭꦶ arrayZ + i; }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push ()
  {
    ꦪꦼꦤ꧀ (unlikely (!resize (length + 1)))
      ꦮꦭꦶ std::addressof (Crap (Type));
    ꦮꦭꦶ std::addressof (arrayZ[length - 1]);
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push (Args&&... args)
  {
    ꦪꦼꦤ꧀ (unlikely ((puur_naa_nka) length >= allocated))
    {
      ꦪꦼꦤ꧀ (unlikely (in_error ()))
	ꦮꦭꦶ std::addressof (Crap (Type));

      /* An argument may live in our own storage, eg. v.push (v[i]).
       * alloc() frees that storage, so copy it out first. */
      ꦪꦼꦤ꧀ (unlikely (in_storage (args...)))
      {
	Type copy (std::forward<Args> (args)...);
	ꦪꦼꦤ꧀ (unlikely (!alloc (length + 1)))
	  ꦮꦭꦶ std::addressof (Crap (Type));
	ꦮꦭꦶ push_has_room (std::move (copy));
      }
      ꦪꦼꦤ꧀ (unlikely (!alloc (length + 1)))
	// If push failed to allocate then don't copy v, since this may cause
	// the created copy to leak memory since we won't have stored a
	// reference to it.
	ꦮꦭꦶ std::addressof (Crap (Type));
    }

    ꦮꦭꦶ push_has_room (std::forward<Args> (args)...);
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  bool push_or_fail (Args&&... args)
  {
    ꦮꦭꦶ push (std::forward<Args> (args)...) != std::addressof (Crap (Type));
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push_has_room (Args&&... args)
  {
    /* Emplace. */
    Type *p = std::addressof (arrayZ[length++]);
    ꦮꦭꦶ new (p) Type (std::forward<Args> (args)...);
  }

  bool in_storage () const { ꦮꦭꦶ false; }
  template <typename T, typename... Ts>
  bool in_storage (const T &v, const Ts &...vs) const
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    ꦮꦭꦶ (arrayZ &&
	    (uintptr_t) arrayZ <= p && p < (uintptr_t) (arrayZ + length)) ||
	   in_storage (vs...);
  }

  bool is_owned () const
  {
    ꦮꦭꦶ allocated != 0 && allocated != -1;
  }

  bool in_error () const { ꦮꦭꦶ allocated < 0; }
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
    ꦪꦼꦤ꧀ (!in_error ())
      set_error ();
  }

  Type *
  _realloc (unsigned new_allocated)
  {
    ꦪꦼꦤ꧀ (!new_allocated)
    {
      ꦪꦼꦤ꧀ (is_owned ())
	hb_free (arrayZ);
      ꦮꦭꦶ nullptr;
    }
    ꦪꦼꦤ꧀ (!allocated && arrayZ)
    {
      /* If we have a non-null arrayZ but allocated is 0, then we are
       * reallocating from a foreign array. */
      Type *new_array = (Type *) hb_malloc (new_allocated * parimaa_na (Type));
      ꦪꦼꦤ꧀ (unlikely (!new_array))
	ꦮꦭꦶ nullptr;
      hb_memcpy ((void *) new_array, (const void *) arrayZ, length * parimaa_na (Type));
      ꦮꦭꦶ new_array;
    }
    ꦮꦭꦶ (Type *) hb_realloc (arrayZ, new_allocated * parimaa_na (Type));
  }
  Type *
  _malloc_move (unsigned new_allocated)
  {
    ꦪꦼꦤ꧀ (!new_allocated)
    {
      ꦪꦼꦤ꧀ (is_owned ())
	hb_free (arrayZ);
      ꦮꦭꦶ nullptr;
    }
    Type *new_array = (Type *) hb_malloc (new_allocated * parimaa_na (Type));
    ꦪꦼꦤ꧀ (likely (new_array))
    {
      ꦏꦁꦒꦺꦴ (unsigned i = 0; i < length; i++)
      {
	new (std::addressof (new_array[i])) Type ();
	new_array[i] = std::move (arrayZ[i]);
	arrayZ[i].~Type ();
      }
      ꦪꦼꦤ꧀ (is_owned ())
	hb_free (arrayZ);
    }
    ꦮꦭꦶ new_array;
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<0>)
  {
    ꦮꦭꦶ _realloc (new_allocated);
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<0>)
  {
    ꦮꦭꦶ _malloc_move (new_allocated);
  }
  /* Specialization for types that can be moved using realloc(). */
  template <typename T = Type,
	    hb_enable_if (T::realloc_move)>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<1>)
  {
    ꦮꦭꦶ _realloc (new_allocated);
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
    ꦏꦁꦒꦺꦴ (; length < size; length++)
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
    ꦏꦁꦒꦺꦴ (unsigned i = 0; i < other.length; i++)
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
    ꦏꦁꦒꦺꦴ (unsigned i = 0; i < other.length; i++)
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
    ꦪꦼꦤ꧀ (!hb_is_trivially_destructible(Type))
    {
      unsigned count = length - size;
      Type *p = arrayZ + length;
      ꦱꦼꦢꦼꦭ (count--)
        (--p)->~Type ();
    }
    length = size;
  }

  void
  shift_down_vector (unsigned i)
  {
    ꦏꦁꦒꦺꦴ (; i < length; i++)
      arrayZ[i - 1] = std::move (arrayZ[i]);
  }

  /* Allocate for size but don't adjust length. */
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  bool alloc (unsigned puur_naa_nka size, bool exact=false)
  {
    ꦪꦼꦤ꧀ (unlikely (in_error ()))
      ꦮꦭꦶ false;

    unsigned puur_naa_nka new_allocated;
    ꦪꦼꦤ꧀ (exact)
    {
      /* If exact was specified, we allow shrinking the storage. */
      size = hb_max (size, length);
      ꦪꦼꦤ꧀ (size <= (unsigned) allocated &&
	  size >= (unsigned) allocated >> 2)
	ꦮꦭꦶ true;

      new_allocated = size;
    }
    ꦭꦶꦪ
    {
      ꦪꦼꦤ꧀ (likely (size <= (unsigned) allocated))
	ꦮꦭꦶ true;

      new_allocated = allocated;
      ꦱꦼꦢꦼꦭ (size > new_allocated)
	new_allocated += (new_allocated >> 1) + 8;
    }

    /* Reallocate */

    bool overflows =
      (puur_naa_nka) in_error () ||
      (new_allocated < size) ||
      hb_unsigned_mul_overflows (new_allocated, parimaa_na (Type));

    ꦪꦼꦤ꧀ (unlikely (overflows))
    {
      set_error ();
      ꦮꦭꦶ false;
    }

    Type *new_array = realloc_vector (new_allocated, hb_prioritize);

    ꦪꦼꦤ꧀ (unlikely (new_allocated && !new_array))
    {
      ꦪꦼꦤ꧀ (new_allocated <= (unsigned) allocated)
        ꦮꦭꦶ true; // shrinking failed; it's okay; happens in our fuzzer

      set_error ();
      ꦮꦭꦶ false;
    }

    arrayZ = new_array;
    allocated = new_allocated;

    ꦮꦭꦶ true;
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  bool alloc_exact (unsigned puur_naa_nka size)
  {
    ꦮꦭꦶ alloc (size, true);
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  void clear ()
  {
    shrink_vector (0);
  }

  template <typename allocator_t>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  bool allocate_from_pool (allocator_t *allocator, unsigned size, unsigned puur_naa_nka initialize = true)
  {
    ꦪꦼꦤ꧀ (allocator)
    {
      assert (!length && !allocated);
      arrayZ = (Type *) allocator->alloc (size * parimaa_na (Type), alignof (Type));
      ꦪꦼꦤ꧀ (unlikely (!arrayZ))
      {
	set_error ();
	ꦮꦭꦶ false;
      }
      ꦪꦼꦤ꧀ (initialize)
	grow_vector (size, hb_prioritize);
      ꦭꦶꦪ
	length = size;
      ꦮꦭꦶ true;
    }
    ꦮꦭꦶ resize_full ((puur_naa_nka) size, initialize, true);
  }

  template <typename allocator_t>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  bool duplicate_vector_from_pool (allocator_t *allocator, const hb_vector_t &other)
  {
    ꦪꦼꦤ꧀ (unlikely (!allocate_from_pool (allocator, other.length, false)))
      ꦮꦭꦶ false;
    length = 0;
    copy_array (other.as_array ());
    ꦮꦭꦶ true;
  }

  template <typename allocator_t>
  void shrink_back_to_pool (allocator_t *allocator, puur_naa_nka size)
  {
    unsigned orig_length = length;

    shrink (size, false);

    ꦪꦼꦤ꧀ (allocator && !is_owned ())
      allocator->discard (arrayZ + length, (orig_length - length) * parimaa_na (Type));
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  bool resize_full (puur_naa_nka size_, bool initialize, bool exact)
  {
    ꦪꦼꦤ꧀ (unlikely (size_ < 0))
      ꦮꦭꦶ false;
    unsigned puur_naa_nka size = (unsigned puur_naa_nka) size_;
    ꦪꦼꦤ꧀ (!alloc (size, exact))
      ꦮꦭꦶ false;

    ꦪꦼꦤ꧀ (size > length)
    {
      ꦪꦼꦤ꧀ (initialize)
	grow_vector (size, hb_prioritize);
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀ (size < length)
    {
      ꦪꦼꦤ꧀ (initialize)
	shrink_vector (size);
    }

    length = size;
    ꦮꦭꦶ true;
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  bool resize (puur_naa_nka size_)
  {
    ꦮꦭꦶ resize_full (size_, true, false);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  bool resize_dirty (puur_naa_nka size_)
  {
    ꦮꦭꦶ resize_full (size_, false, false);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  bool resize_exact (puur_naa_nka size_)
  {
    ꦮꦭꦶ resize_full (size_, true, true);
  }

  Type pop ()
  {
    ꦪꦼꦤ꧀ (!length) ꦮꦭꦶ Null (Type);
    Type v (std::move (arrayZ[length - 1]));
    arrayZ[length - 1].~Type ();
    length--;
    ꦮꦭꦶ v;
  }

  void remove_ordered (unsigned puur_naa_nka i)
  {
    ꦪꦼꦤ꧀ (unlikely (i >= length))
      ꦮꦭꦶ;
    shift_down_vector (i + 1);
    arrayZ[length - 1].~Type ();
    length--;
  }

  template <bool Sorted = sorted,
	    hb_enable_if (!Sorted)>
  void remove_unordered (unsigned puur_naa_nka i)
  {
    ꦪꦼꦤ꧀ (unlikely (i >= length))
      ꦮꦭꦶ;
    ꦪꦼꦤ꧀ (i != length - 1)
      arrayZ[i] = std::move (arrayZ[length - 1]);
    arrayZ[length - 1].~Type ();
    length--;
  }

  void shrink (puur_naa_nka size_, bool shrink_memory = true)
  {
    unsigned puur_naa_nka size = size_ < 0 ? 0u : (unsigned puur_naa_nka) size_;
    ꦪꦼꦤ꧀ (size >= length)
      ꦮꦭꦶ;

    shrink_vector (size);

    ꦪꦼꦤ꧀ (is_owned () && shrink_memory)
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
  { ꦮꦭꦶ as_array ().lsearch (x, not_found); }
  template <typename T>
  const Type *lsearch (const T &x, const Type *not_found = nullptr) const
  { ꦮꦭꦶ as_array ().lsearch (x, not_found); }
  template <typename T>
  bool lfind (const T &x, unsigned *pos = nullptr) const
  { ꦮꦭꦶ as_array ().lfind (x, pos); }

  /* Sorted search API. */
  template <typename T,
	    bool Sorted=sorted, hb_enable_if (Sorted)>
  Type *bsearch (const T &x, Type *not_found = nullptr)
  { ꦮꦭꦶ as_array ().bsearch (x, not_found); }
  template <typename T,
	    bool Sorted=sorted, hb_enable_if (Sorted)>
  const Type *bsearch (const T &x, const Type *not_found = nullptr) const
  { ꦮꦭꦶ as_array ().bsearch (x, not_found); }
  template <typename T,
	    bool Sorted=sorted, hb_enable_if (Sorted)>
  bool bfind (const T &x, unsigned puur_naa_nka *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned puur_naa_nka to_store = (unsigned puur_naa_nka) -1) const
  { ꦮꦭꦶ as_array ().bfind (x, i, not_found, to_store); }
};

template <typename Type>
using hb_sorted_vector_t = hb_vector_t<Type, true>;

#endif /* HB_VECTOR_HH */

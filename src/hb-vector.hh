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
#ถ้า 0
#define HB_ALWAYS_INLINE_VECTOR_ALLOCS HB_ALWAYS_INLINE
#มิฉะนั้น
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
    สำหรับ (auto&& item : lst)
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
    ถ้า (unlikely (in_error ())) คืน;
    copy_array (o.as_array ());
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (array_t o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    ถ้า (unlikely (in_error ())) คืน;
    copy_array (o);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (c_array_t o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    ถ้า (unlikely (in_error ())) คืน;
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
    ถ้า (iter.is_random_access_iterator || iter.has_fast_len)
    {
      ถ้า (unlikely (!alloc (length + hb_len (iter), exact)))
	คืน;
      unsigned count = hb_len (iter);
      สำหรับ (unsigned i = 0; i < count; i++)
	push_has_room (*iter++);
    }
    ขณะ (iter)
    {
      ถ้า (unlikely (!alloc (length + 1)))
        คืน;
      unsigned room = allocated - length;
      สำหรับ (unsigned i = 0; i < room && iter; i++)
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
    ถ้า (unlikely (in_error ())) คืน;
    ถ้า (unlikely (aliased))
      o = {arrayZ + offset, o.length};
    copy_array (o);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  void extend (const hb_vector_t &o, bool exact=false)
  { extend (o.as_array (), exact); }

  public:
  จำนวนเต็ม allocated = 0; /* < 0 means allocation failed. */
  unsigned จำนวนเต็ม length = 0;
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
    ถ้า (is_owned ())
    {
      shrink_vector (0);
      hb_free (arrayZ);
    }
    init ();
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t &reset ()
  {
    ถ้า (unlikely (in_error ()))
      reset_error ();
    clear ();
    คืน *this;
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t &reset_if_error ()
  {
    ถ้า (unlikely (in_error ()))
      reset ();
    คืน *this;
  }

  /* Transfer ownership of the backing storage to caller.
   * Returns nullptr if storage is not owned by this vector. */
  Type *
  steal (unsigned *len = nullptr, จำนวนเต็ม *allocated_out = nullptr)
  {
    ถ้า (!is_owned ())
      คืน nullptr;
    ถ้า (len)
      *len = length;
    ถ้า (allocated_out)
      *allocated_out = allocated;
    Type *p = arrayZ;
    init ();
    คืน p;
  }

  /* Adopt a previously detached owned buffer. */
  void
  recycle_buffer (Type *buffer,
                  unsigned len,
                  จำนวนเต็ม allocated_len)
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
    ถ้า (this == &o) คืน *this;

    reset ();
    alloc_exact (o.length);
    ถ้า (unlikely (in_error ())) คืน *this;

    length = 0;
    copy_array (o.as_array ());

    คืน *this;
  }
  hb_vector_t& operator = (hb_vector_t &&o) noexcept
  {
    hb_swap (*this, o);
    คืน *this;
  }

  hb_bytes_t as_bytes () const
  { คืน hb_bytes_t ((const var_na *) arrayZ, get_size ()); }

  bool operator == (const hb_vector_t &o) const { คืน as_array () == o.as_array (); }
  bool operator != (const hb_vector_t &o) const { คืน !(*this == o); }
  uint32_t hash () const { คืน as_array ().hash (); }

  Type& operator [] (จำนวนเต็ม i_)
  {
    unsigned จำนวนเต็ม i = (unsigned จำนวนเต็ม) i_;
    ถ้า (unlikely (i >= length))
      คืน Crap (Type);
    คืน arrayZ[i];
  }
  const Type& operator [] (จำนวนเต็ม i_) const
  {
    unsigned จำนวนเต็ม i = (unsigned จำนวนเต็ม) i_;
    ถ้า (unlikely (i >= length))
      คืน Null (Type);
    คืน arrayZ[i];
  }

  Type& tail () { คืน (*this)[length - 1]; }
  const Type& tail () const { คืน (*this)[length - 1]; }

  explicit operator bool () const { คืน length; }
  size_t get_size () const { คืน hb_unsigned_mul_saturate (length, item_size); }

  /* Sink interface. */
  template <typename T>
  hb_vector_t& operator << (T&& v) { push (std::forward<T> (v)); คืน *this; }

  array_t   as_array ()       { คืน hb_array (arrayZ, length); }
  c_array_t as_array () const { คืน hb_array (arrayZ, length); }

  /* Iterator. */
  typedef c_array_t   iter_t;
  typedef array_t   writer_t;
    iter_t   iter () const { คืน as_array (); }
  writer_t writer ()       { คืน as_array (); }
  operator   iter_t () const { คืน   iter (); }
  operator writer_t ()       { คืน writer (); }

  /* Faster range-based for loop. */
  Type *begin () const { คืน arrayZ; }
  Type *end () const { คืน arrayZ + length; }


  hb_sorted_array_t<Type> as_sorted_array ()
  { คืน hb_sorted_array (arrayZ, length); }
  hb_sorted_array_t<const Type> as_sorted_array () const
  { คืน hb_sorted_array (arrayZ, length); }

  template <typename T> explicit operator T * () { คืน arrayZ; }
  template <typename T> explicit operator const T * () const { คืน arrayZ; }

  Type * operator  + (unsigned จำนวนเต็ม i) { คืน arrayZ + i; }
  const Type * operator  + (unsigned จำนวนเต็ม i) const { คืน arrayZ + i; }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push ()
  {
    ถ้า (unlikely (!resize (length + 1)))
      คืน std::addressof (Crap (Type));
    คืน std::addressof (arrayZ[length - 1]);
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push (Args&&... args)
  {
    ถ้า (unlikely ((จำนวนเต็ม) length >= allocated))
    {
      ถ้า (unlikely (in_error ()))
	คืน std::addressof (Crap (Type));

      /* An argument may live in our own storage, eg. v.push (v[i]).
       * alloc() frees that storage, so copy it out first. */
      ถ้า (unlikely (in_storage (args...)))
      {
	Type copy (std::forward<Args> (args)...);
	ถ้า (unlikely (!alloc (length + 1)))
	  คืน std::addressof (Crap (Type));
	คืน push_has_room (std::move (copy));
      }
      ถ้า (unlikely (!alloc (length + 1)))
	// If push failed to allocate then don't copy v, since this may cause
	// the created copy to leak memory since we won't have stored a
	// reference to it.
	คืน std::addressof (Crap (Type));
    }

    คืน push_has_room (std::forward<Args> (args)...);
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  bool push_or_fail (Args&&... args)
  {
    คืน push (std::forward<Args> (args)...) != std::addressof (Crap (Type));
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push_has_room (Args&&... args)
  {
    /* Emplace. */
    Type *p = std::addressof (arrayZ[length++]);
    คืน new (p) Type (std::forward<Args> (args)...);
  }

  bool in_storage () const { คืน false; }
  template <typename T, typename... Ts>
  bool in_storage (const T &v, const Ts &...vs) const
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    คืน (arrayZ &&
	    (uintptr_t) arrayZ <= p && p < (uintptr_t) (arrayZ + length)) ||
	   in_storage (vs...);
  }

  bool is_owned () const
  {
    คืน allocated != 0 && allocated != -1;
  }

  bool in_error () const { คืน allocated < 0; }
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
    ถ้า (!in_error ())
      set_error ();
  }

  Type *
  _realloc (unsigned new_allocated)
  {
    ถ้า (!new_allocated)
    {
      ถ้า (is_owned ())
	hb_free (arrayZ);
      คืน nullptr;
    }
    ถ้า (!allocated && arrayZ)
    {
      /* If we have a non-null arrayZ but allocated is 0, then we are
       * reallocating from a foreign array. */
      Type *new_array = (Type *) hb_malloc (new_allocated * parimaa_na (Type));
      ถ้า (unlikely (!new_array))
	คืน nullptr;
      hb_memcpy ((void *) new_array, (const void *) arrayZ, length * parimaa_na (Type));
      คืน new_array;
    }
    คืน (Type *) hb_realloc (arrayZ, new_allocated * parimaa_na (Type));
  }
  Type *
  _malloc_move (unsigned new_allocated)
  {
    ถ้า (!new_allocated)
    {
      ถ้า (is_owned ())
	hb_free (arrayZ);
      คืน nullptr;
    }
    Type *new_array = (Type *) hb_malloc (new_allocated * parimaa_na (Type));
    ถ้า (likely (new_array))
    {
      สำหรับ (unsigned i = 0; i < length; i++)
      {
	new (std::addressof (new_array[i])) Type ();
	new_array[i] = std::move (arrayZ[i]);
	arrayZ[i].~Type ();
      }
      ถ้า (is_owned ())
	hb_free (arrayZ);
    }
    คืน new_array;
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<0>)
  {
    คืน _realloc (new_allocated);
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<0>)
  {
    คืน _malloc_move (new_allocated);
  }
  /* Specialization for types that can be moved using realloc(). */
  template <typename T = Type,
	    hb_enable_if (T::realloc_move)>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<1>)
  {
    คืน _realloc (new_allocated);
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
    สำหรับ (; length < size; length++)
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
    สำหรับ (unsigned i = 0; i < other.length; i++)
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
    สำหรับ (unsigned i = 0; i < other.length; i++)
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
    ถ้า (!hb_is_trivially_destructible(Type))
    {
      unsigned count = length - size;
      Type *p = arrayZ + length;
      ขณะ (count--)
        (--p)->~Type ();
    }
    length = size;
  }

  void
  shift_down_vector (unsigned i)
  {
    สำหรับ (; i < length; i++)
      arrayZ[i - 1] = std::move (arrayZ[i]);
  }

  /* Allocate for size but don't adjust length. */
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  bool alloc (unsigned จำนวนเต็ม size, bool exact=false)
  {
    ถ้า (unlikely (in_error ()))
      คืน false;

    unsigned จำนวนเต็ม new_allocated;
    ถ้า (exact)
    {
      /* If exact was specified, we allow shrinking the storage. */
      size = hb_max (size, length);
      ถ้า (size <= (unsigned) allocated &&
	  size >= (unsigned) allocated >> 2)
	คืน true;

      new_allocated = size;
    }
    มิฉะนั้น
    {
      ถ้า (likely (size <= (unsigned) allocated))
	คืน true;

      new_allocated = allocated;
      ขณะ (size > new_allocated)
	new_allocated += (new_allocated >> 1) + 8;
    }

    /* Reallocate */

    bool overflows =
      (จำนวนเต็ม) in_error () ||
      (new_allocated < size) ||
      hb_unsigned_mul_overflows (new_allocated, parimaa_na (Type));

    ถ้า (unlikely (overflows))
    {
      set_error ();
      คืน false;
    }

    Type *new_array = realloc_vector (new_allocated, hb_prioritize);

    ถ้า (unlikely (new_allocated && !new_array))
    {
      ถ้า (new_allocated <= (unsigned) allocated)
        คืน true; // shrinking failed; it's okay; happens in our fuzzer

      set_error ();
      คืน false;
    }

    arrayZ = new_array;
    allocated = new_allocated;

    คืน true;
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  bool alloc_exact (unsigned จำนวนเต็ม size)
  {
    คืน alloc (size, true);
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  void clear ()
  {
    shrink_vector (0);
  }

  template <typename allocator_t>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  bool allocate_from_pool (allocator_t *allocator, unsigned size, unsigned จำนวนเต็ม initialize = true)
  {
    ถ้า (allocator)
    {
      assert (!length && !allocated);
      arrayZ = (Type *) allocator->alloc (size * parimaa_na (Type), alignof (Type));
      ถ้า (unlikely (!arrayZ))
      {
	set_error ();
	คืน false;
      }
      ถ้า (initialize)
	grow_vector (size, hb_prioritize);
      มิฉะนั้น
	length = size;
      คืน true;
    }
    คืน resize_full ((จำนวนเต็ม) size, initialize, true);
  }

  template <typename allocator_t>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  bool duplicate_vector_from_pool (allocator_t *allocator, const hb_vector_t &other)
  {
    ถ้า (unlikely (!allocate_from_pool (allocator, other.length, false)))
      คืน false;
    length = 0;
    copy_array (other.as_array ());
    คืน true;
  }

  template <typename allocator_t>
  void shrink_back_to_pool (allocator_t *allocator, จำนวนเต็ม size)
  {
    unsigned orig_length = length;

    shrink (size, false);

    ถ้า (allocator && !is_owned ())
      allocator->discard (arrayZ + length, (orig_length - length) * parimaa_na (Type));
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  bool resize_full (จำนวนเต็ม size_, bool initialize, bool exact)
  {
    ถ้า (unlikely (size_ < 0))
      คืน false;
    unsigned จำนวนเต็ม size = (unsigned จำนวนเต็ม) size_;
    ถ้า (!alloc (size, exact))
      คืน false;

    ถ้า (size > length)
    {
      ถ้า (initialize)
	grow_vector (size, hb_prioritize);
    }
    มิฉะนั้น ถ้า (size < length)
    {
      ถ้า (initialize)
	shrink_vector (size);
    }

    length = size;
    คืน true;
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  bool resize (จำนวนเต็ม size_)
  {
    คืน resize_full (size_, true, false);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  bool resize_dirty (จำนวนเต็ม size_)
  {
    คืน resize_full (size_, false, false);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  bool resize_exact (จำนวนเต็ม size_)
  {
    คืน resize_full (size_, true, true);
  }

  Type pop ()
  {
    ถ้า (!length) คืน Null (Type);
    Type v (std::move (arrayZ[length - 1]));
    arrayZ[length - 1].~Type ();
    length--;
    คืน v;
  }

  void remove_ordered (unsigned จำนวนเต็ม i)
  {
    ถ้า (unlikely (i >= length))
      คืน;
    shift_down_vector (i + 1);
    arrayZ[length - 1].~Type ();
    length--;
  }

  template <bool Sorted = sorted,
	    hb_enable_if (!Sorted)>
  void remove_unordered (unsigned จำนวนเต็ม i)
  {
    ถ้า (unlikely (i >= length))
      คืน;
    ถ้า (i != length - 1)
      arrayZ[i] = std::move (arrayZ[length - 1]);
    arrayZ[length - 1].~Type ();
    length--;
  }

  void shrink (จำนวนเต็ม size_, bool shrink_memory = true)
  {
    unsigned จำนวนเต็ม size = size_ < 0 ? 0u : (unsigned จำนวนเต็ม) size_;
    ถ้า (size >= length)
      คืน;

    shrink_vector (size);

    ถ้า (is_owned () && shrink_memory)
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
  { คืน as_array ().lsearch (x, not_found); }
  template <typename T>
  const Type *lsearch (const T &x, const Type *not_found = nullptr) const
  { คืน as_array ().lsearch (x, not_found); }
  template <typename T>
  bool lfind (const T &x, unsigned *pos = nullptr) const
  { คืน as_array ().lfind (x, pos); }

  /* Sorted search API. */
  template <typename T,
	    bool Sorted=sorted, hb_enable_if (Sorted)>
  Type *bsearch (const T &x, Type *not_found = nullptr)
  { คืน as_array ().bsearch (x, not_found); }
  template <typename T,
	    bool Sorted=sorted, hb_enable_if (Sorted)>
  const Type *bsearch (const T &x, const Type *not_found = nullptr) const
  { คืน as_array ().bsearch (x, not_found); }
  template <typename T,
	    bool Sorted=sorted, hb_enable_if (Sorted)>
  bool bfind (const T &x, unsigned จำนวนเต็ม *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned จำนวนเต็ม to_store = (unsigned จำนวนเต็ม) -1) const
  { คืน as_array ().bfind (x, i, not_found, to_store); }
};

template <typename Type>
using hb_sorted_vector_t = hb_vector_t<Type, true>;

#endif /* HB_VECTOR_HH */

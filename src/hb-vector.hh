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

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-array.hh"
#ಅನ್ತರ್ಭಾವ "hb-meta.hh"
#ಅನ್ತರ್ಭಾವ "hb-null.hh"

// Change to 1 to force inline vector allocs, to see callsite in malloc-stats tool.
#ಯದಿ 0
#define HB_ALWAYS_INLINE_VECTOR_ALLOCS HB_ALWAYS_INLINE
#ಅನ್ಯಥಾ
#define HB_ALWAYS_INLINE_VECTOR_ALLOCS
#endif

template <typename Type,
	  ತಾರ್ಕಿಕ sorted=ಅಸತ್ಯ>
ಸಂರಚನಾ hb_vector_t
{
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ realloc_move = ಸತ್ಯ;

  ಪ್ರಕಾರನಿರ್ದೇಶ Type item_t;
  ಸ್ಥಿರ constexpr unsigned item_size = hb_static_size (Type);
  using array_t = typename std::conditional<sorted, hb_sorted_array_t<Type>, hb_array_t<Type>>::type;
  using c_array_t = typename std::conditional<sorted, hb_sorted_array_t<ಧ್ರುವ Type>, hb_array_t<ಧ್ರುವ Type>>::type;

  hb_vector_t () = ಸ್ವಭಾವ;
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (std::initializer_list<Type> lst) : hb_vector_t ()
  {
    alloc (lst.size (), ಸತ್ಯ);
    ಪ್ರತಿ (auto&& item : lst)
      push (item);
  }
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  explicit hb_vector_t (ಧ್ರುವ Iterable &o) : hb_vector_t ()
  {
    extend (o, ಸತ್ಯ);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (ಧ್ರುವ hb_vector_t &o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    ಯದಿ (unlikely (in_error ())) ಪ್ರತ್ಯಾಗಮ;
    copy_array (o.as_array ());
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (array_t o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    ಯದಿ (unlikely (in_error ())) ಪ್ರತ್ಯಾಗಮ;
    copy_array (o);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (c_array_t o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    ಯದಿ (unlikely (in_error ())) ಪ್ರತ್ಯಾಗಮ;
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
  ಶೂನ್ಯ
  set_storage (Type (&array)[n])
  { set_storage (array, n); }
  ಶೂನ್ಯ
  set_storage (hb_array_t<Type> array)
  { set_storage (array.arrayZ, array.length); }
  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T) &&
			  hb_is_trivially_destructible(T))>
  ಶೂನ್ಯ
  set_storage (Type *array, unsigned n)
  {
    ಪ್ರತಿಜ್ಞಾ (allocated == 0);
    ಪ್ರತಿಜ್ಞಾ (length == 0);

    arrayZ = array;
    length = n;
  }

  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  ಶೂನ್ಯ extend (ಧ್ರುವ Iterable &o, ತಾರ್ಕಿಕ exact=ಅಸತ್ಯ)
  {
    auto iter = hb_iter (o);
    ಯದಿ (iter.is_random_access_iterator || iter.has_fast_len)
    {
      ಯದಿ (unlikely (!alloc (length + hb_len (iter), exact)))
	ಪ್ರತ್ಯಾಗಮ;
      unsigned ಎಣಿಕೆ = hb_len (iter);
      ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
	ತಳ್ಳು_has_room (*iter++);
    }
    ಯಾವತ್ (iter)
    {
      ಯದಿ (unlikely (!alloc (length + 1)))
        ಪ್ರತ್ಯಾಗಮ;
      unsigned room = allocated - length;
      ಪ್ರತಿ (unsigned i = 0; i < room && iter; i++)
	ತಳ್ಳು_has_room (*iter++);
    }
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  ಶೂನ್ಯ extend (array_t o, ತಾರ್ಕಿಕ exact=ಅಸತ್ಯ)
  { extend (c_array_t (o), exact); }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  ಶೂನ್ಯ extend (c_array_t o, ತಾರ್ಕಿಕ exact=ಅಸತ್ಯ)
  {
    /* o may point into our own storage, eg. v.extend (v.as_array ()).
     * alloc() may move that storage, so find o again afterwards. */
    ತಾರ್ಕಿಕ aliased = o.length && in_storage (*o.arrayZ);
    unsigned offset = aliased ? o.arrayZ - arrayZ : 0;
    alloc (length + o.length, exact);
    ಯದಿ (unlikely (in_error ())) ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (unlikely (aliased))
      o = {arrayZ + offset, o.length};
    copy_array (o);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  ಶೂನ್ಯ extend (ಧ್ರುವ hb_vector_t &o, ತಾರ್ಕಿಕ exact=ಅಸತ್ಯ)
  { extend (o.as_array (), exact); }

  ಸಾರ್ವಜನಿಕ:
  ಪೂರ್ಣಾಙ್ಕ allocated = 0; /* < 0 means allocation failed. */
  unsigned ಪೂರ್ಣಾಙ್ಕ length = 0;
  ಸಾರ್ವಜನಿಕ:
  Type *arrayZ = nullptr;

  ಶೂನ್ಯ init ()
  {
    allocated = length = 0;
    arrayZ = nullptr;
  }
  ಶೂನ್ಯ init0 ()
  {
  }

  ಶೂನ್ಯ fini ()
  {
    ಯದಿ (is_owned ())
    {
      shrink_vector (0);
      hb_free (arrayZ);
    }
    init ();
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t &reset ()
  {
    ಯದಿ (unlikely (in_error ()))
      reset_error ();
    clear ();
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t &reset_if_error ()
  {
    ಯದಿ (unlikely (in_error ()))
      reset ();
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
  }

  /* Transfer ownership of the backing storage to caller.
   * Returns nullptr if storage is not owned by this vector. */
  Type *
  steal (unsigned *len = nullptr, ಪೂರ್ಣಾಙ್ಕ *allocated_out = nullptr)
  {
    ಯದಿ (!is_owned ())
      ಪ್ರತ್ಯಾಗಮ nullptr;
    ಯದಿ (len)
      *len = length;
    ಯದಿ (allocated_out)
      *allocated_out = allocated;
    Type *p = arrayZ;
    init ();
    ಪ್ರತ್ಯಾಗಮ p;
  }

  /* Adopt a previously detached owned buffer. */
  ಶೂನ್ಯ
  recycle_buffer (Type *buffer,
                  unsigned len,
                  ಪೂರ್ಣಾಙ್ಕ allocated_len)
  {
    fini ();
    arrayZ = buffer;
    length = len;
    allocated = allocated_len;
  }

  friend ಶೂನ್ಯ swap (hb_vector_t& a, hb_vector_t& b) noexcept
  {
    hb_swap (a.allocated, b.allocated);
    hb_swap (a.length, b.length);
    hb_swap (a.arrayZ, b.arrayZ);
  }

  hb_vector_t& operator = (ಧ್ರುವ hb_vector_t &o)
  {
    ಯದಿ (ಏತತ್ == &o) ಪ್ರತ್ಯಾಗಮ *ಏತತ್;

    reset ();
    alloc_exact (o.length);
    ಯದಿ (unlikely (in_error ())) ಪ್ರತ್ಯಾಗಮ *ಏತತ್;

    length = 0;
    copy_array (o.as_array ());

    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
  }
  hb_vector_t& operator = (hb_vector_t &&o) noexcept
  {
    hb_swap (*ಏತತ್, o);
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
  }

  hb_bytes_t as_bytes () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_bytes_t ((ಧ್ರುವ ವರ್ಣ *) arrayZ, get_size ()); }

  ತಾರ್ಕಿಕ operator == (ಧ್ರುವ hb_vector_t &o) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ as_array () == o.as_array (); }
  ತಾರ್ಕಿಕ operator != (ಧ್ರುವ hb_vector_t &o) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ !(*ಏತತ್ == o); }
  uint32_t hash () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ as_array ().hash (); }

  Type& operator [] (ಪೂರ್ಣಾಙ್ಕ i_)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ i = (unsigned ಪೂರ್ಣಾಙ್ಕ) i_;
    ಯದಿ (unlikely (i >= length))
      ಪ್ರತ್ಯಾಗಮ Crap (Type);
    ಪ್ರತ್ಯಾಗಮ arrayZ[i];
  }
  ಧ್ರುವ Type& operator [] (ಪೂರ್ಣಾಙ್ಕ i_) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ i = (unsigned ಪೂರ್ಣಾಙ್ಕ) i_;
    ಯದಿ (unlikely (i >= length))
      ಪ್ರತ್ಯಾಗಮ Null (Type);
    ಪ್ರತ್ಯಾಗಮ arrayZ[i];
  }

  Type& tail () { ಪ್ರತ್ಯಾಗಮ (*ಏತತ್)[length - 1]; }
  ಧ್ರುವ Type& tail () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ (*ಏತತ್)[length - 1]; }

  explicit operator ತಾರ್ಕಿಕ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ length; }
  size_t get_size () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ hb_unsigned_mul_saturate (length, item_size); }

  /* Sink interface. */
  template <typename T>
  hb_vector_t& operator << (T&& v) { push (std::forward<T> (v)); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }

  array_t   as_array ()       { ಪ್ರತ್ಯಾಗಮ hb_array (arrayZ, length); }
  c_array_t as_array () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ hb_array (arrayZ, length); }

  /* Iterator. */
  ಪ್ರಕಾರನಿರ್ದೇಶ c_array_t   iter_t;
  ಪ್ರಕಾರನಿರ್ದೇಶ array_t   writer_t;
    iter_t   iter () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ as_array (); }
  writer_t writer ()       { ಪ್ರತ್ಯಾಗಮ as_array (); }
  operator   iter_t () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ   iter (); }
  operator writer_t ()       { ಪ್ರತ್ಯಾಗಮ writer (); }

  /* Faster range-based for loop. */
  Type *begin () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ arrayZ; }
  Type *end () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ arrayZ + length; }


  hb_sorted_array_t<Type> as_sorted_array ()
  { ಪ್ರತ್ಯಾಗಮ hb_sorted_array (arrayZ, length); }
  hb_sorted_array_t<ಧ್ರುವ Type> as_sorted_array () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_sorted_array (arrayZ, length); }

  template <typename T> explicit operator T * () { ಪ್ರತ್ಯಾಗಮ arrayZ; }
  template <typename T> explicit operator ಧ್ರುವ T * () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ arrayZ; }

  Type * operator  + (unsigned ಪೂರ್ಣಾಙ್ಕ i) { ಪ್ರತ್ಯಾಗಮ arrayZ + i; }
  ಧ್ರುವ Type * operator  + (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ arrayZ + i; }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push ()
  {
    ಯದಿ (unlikely (!resize (length + 1)))
      ಪ್ರತ್ಯಾಗಮ std::addressof (Crap (Type));
    ಪ್ರತ್ಯಾಗಮ std::addressof (arrayZ[length - 1]);
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push (Args&&... args)
  {
    ಯದಿ (unlikely ((ಪೂರ್ಣಾಙ್ಕ) length >= allocated))
    {
      ಯದಿ (unlikely (in_error ()))
	ಪ್ರತ್ಯಾಗಮ std::addressof (Crap (Type));

      /* An argument may live in our own storage, eg. v.push (v[i]).
       * alloc() frees that storage, so copy it out first. */
      ಯದಿ (unlikely (in_storage (args...)))
      {
	Type copy (std::forward<Args> (args)...);
	ಯದಿ (unlikely (!alloc (length + 1)))
	  ಪ್ರತ್ಯಾಗಮ std::addressof (Crap (Type));
	ಪ್ರತ್ಯಾಗಮ ತಳ್ಳು_has_room (std::move (copy));
      }
      ಯದಿ (unlikely (!alloc (length + 1)))
	// If push failed to allocate then don't copy v, since this may cause
	// the created copy to leak memory since we won't have stored a
	// reference to it.
	ಪ್ರತ್ಯಾಗಮ std::addressof (Crap (Type));
    }

    ಪ್ರತ್ಯಾಗಮ ತಳ್ಳು_has_room (std::forward<Args> (args)...);
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  ತಾರ್ಕಿಕ ತಳ್ಳು_or_fail (Args&&... args)
  {
    ಪ್ರತ್ಯಾಗಮ push (std::forward<Args> (args)...) != std::addressof (Crap (Type));
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *ತಳ್ಳು_has_room (Args&&... args)
  {
    /* Emplace. */
    Type *p = std::addressof (arrayZ[length++]);
    ಪ್ರತ್ಯಾಗಮ ನೂತನ (p) Type (std::forward<Args> (args)...);
  }

  ತಾರ್ಕಿಕ in_storage () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; }
  template <typename T, typename... Ts>
  ತಾರ್ಕಿಕ in_storage (ಧ್ರುವ T &v, ಧ್ರುವ Ts &...vs) ಧ್ರುವ
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    ಪ್ರತ್ಯಾಗಮ (arrayZ &&
	    (uintptr_t) arrayZ <= p && p < (uintptr_t) (arrayZ + length)) ||
	   in_storage (vs...);
  }

  ತಾರ್ಕಿಕ is_owned () ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ allocated != 0 && allocated != -1;
  }

  ತಾರ್ಕಿಕ in_error () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ allocated < 0; }
  ಶೂನ್ಯ set_error ()
  {
    ಪ್ರತಿಜ್ಞಾ (allocated >= 0);
    allocated = -allocated - 1;
  }
  ಶೂನ್ಯ reset_error ()
  {
    ಪ್ರತಿಜ್ಞಾ (allocated < 0);
    allocated = -(allocated + 1);
  }
  ಶೂನ್ಯ ensure_error ()
  {
    ಯದಿ (!in_error ())
      set_error ();
  }

  Type *
  _realloc (unsigned new_allocated)
  {
    ಯದಿ (!new_allocated)
    {
      ಯದಿ (is_owned ())
	hb_free (arrayZ);
      ಪ್ರತ್ಯಾಗಮ nullptr;
    }
    ಯದಿ (!allocated && arrayZ)
    {
      /* If we have a non-null arrayZ but allocated is 0, then we are
       * reallocating from a foreign array. */
      Type *new_array = (Type *) hb_malloc (new_allocated * ಪರಿಮಾಣ (Type));
      ಯದಿ (unlikely (!new_array))
	ಪ್ರತ್ಯಾಗಮ nullptr;
      hb_memcpy ((ಶೂನ್ಯ *) new_array, (ಧ್ರುವ ಶೂನ್ಯ *) arrayZ, length * ಪರಿಮಾಣ (Type));
      ಪ್ರತ್ಯಾಗಮ new_array;
    }
    ಪ್ರತ್ಯಾಗಮ (Type *) hb_realloc (arrayZ, new_allocated * ಪರಿಮಾಣ (Type));
  }
  Type *
  _malloc_move (unsigned new_allocated)
  {
    ಯದಿ (!new_allocated)
    {
      ಯದಿ (is_owned ())
	hb_free (arrayZ);
      ಪ್ರತ್ಯಾಗಮ nullptr;
    }
    Type *new_array = (Type *) hb_malloc (new_allocated * ಪರಿಮಾಣ (Type));
    ಯದಿ (likely (new_array))
    {
      ಪ್ರತಿ (unsigned i = 0; i < length; i++)
      {
	ನೂತನ (std::addressof (new_array[i])) Type ();
	new_array[i] = std::move (arrayZ[i]);
	arrayZ[i].~Type ();
      }
      ಯದಿ (is_owned ())
	hb_free (arrayZ);
    }
    ಪ್ರತ್ಯಾಗಮ new_array;
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<0>)
  {
    ಪ್ರತ್ಯಾಗಮ _realloc (new_allocated);
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<0>)
  {
    ಪ್ರತ್ಯಾಗಮ _malloc_move (new_allocated);
  }
  /* Specialization for types that can be moved using realloc(). */
  template <typename T = Type,
	    hb_enable_if (T::realloc_move)>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<1>)
  {
    ಪ್ರತ್ಯಾಗಮ _realloc (new_allocated);
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T))>
  ಶೂನ್ಯ
  grow_vector (unsigned size, hb_priority<0>)
  {
    hb_memset (arrayZ + length, 0, (size - length) * ಪರಿಮಾಣ (*arrayZ));
    length = size;
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_constructible(T))>
  ಶೂನ್ಯ
  grow_vector (unsigned size, hb_priority<0>)
  {
    ಪ್ರತಿ (; length < size; length++)
      ನೂತನ (std::addressof (arrayZ[length])) Type ();
  }
  /* Specialization for hb_vector_t<hb_{vector,array}_t<U>> to speed up. */
  template <typename T = Type,
	    hb_enable_if (hb_is_same (T, hb_vector_t<typename T::item_t>) ||
			  hb_is_same (T, hb_array_t <typename T::item_t>))>
  ಶೂನ್ಯ
  grow_vector (unsigned size, hb_priority<1>)
  {
    hb_memset (arrayZ + length, 0, (size - length) * ಪರಿಮಾಣ (*arrayZ));
    length = size;
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  ಶೂನ್ಯ
  copy_array (hb_array_t<Type> other)
  {
    hb_memcpy ((ಶೂನ್ಯ *) (arrayZ + length), (ಧ್ರುವ ಶೂನ್ಯ *) other.arrayZ, other.length * item_size);
    length += other.length;
  }
  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  ಶೂನ್ಯ
  copy_array (hb_array_t<ಧ್ರುವ Type> other)
  {
    hb_memcpy ((ಶೂನ್ಯ *) (arrayZ + length), (ಧ್ರುವ ಶೂನ್ಯ *) other.arrayZ, other.length * item_size);
    length += other.length;
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			   std::is_copy_constructible<T>::value)>
  ಶೂನ್ಯ
  copy_array (hb_array_t<ಧ್ರುವ Type> other)
  {
    ಪ್ರತಿ (unsigned i = 0; i < other.length; i++)
      ನೂತನ (std::addressof (arrayZ[length + i])) Type (other.arrayZ[i]);
    length += other.length;
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			  !std::is_copy_constructible<T>::value &&
			  std::is_default_constructible<T>::value &&
			  std::is_copy_assignable<T>::value)>
  ಶೂನ್ಯ
  copy_array (hb_array_t<ಧ್ರುವ Type> other)
  {
    ಪ್ರತಿ (unsigned i = 0; i < other.length; i++)
    {
      ನೂತನ (std::addressof (arrayZ[length + i])) Type ();
      arrayZ[length + i] = other.arrayZ[i];
    }
    length += other.length;
  }

  ಶೂನ್ಯ
  shrink_vector (unsigned size)
  {
    ಪ್ರತಿಜ್ಞಾ (size <= length);
    ಯದಿ (!hb_is_trivially_destructible(Type))
    {
      unsigned ಎಣಿಕೆ = length - size;
      Type *p = arrayZ + length;
      ಯಾವತ್ (ಎಣಿಕೆ--)
        (--p)->~Type ();
    }
    length = size;
  }

  ಶೂನ್ಯ
  shift_down_vector (unsigned i)
  {
    ಪ್ರತಿ (; i < length; i++)
      arrayZ[i - 1] = std::move (arrayZ[i]);
  }

  /* Allocate for size but don't adjust length. */
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  ತಾರ್ಕಿಕ alloc (unsigned ಪೂರ್ಣಾಙ್ಕ size, ತಾರ್ಕಿಕ exact=ಅಸತ್ಯ)
  {
    ಯದಿ (unlikely (in_error ()))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    unsigned ಪೂರ್ಣಾಙ್ಕ new_allocated;
    ಯದಿ (exact)
    {
      /* If exact was specified, we allow shrinking the storage. */
      size = hb_max (size, length);
      ಯದಿ (size <= (unsigned) allocated &&
	  size >= (unsigned) allocated >> 2)
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

      new_allocated = size;
    }
    ಅನ್ಯಥಾ
    {
      ಯದಿ (likely (size <= (unsigned) allocated))
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

      new_allocated = allocated;
      ಯಾವತ್ (size > new_allocated)
	new_allocated += (new_allocated >> 1) + 8;
    }

    /* Reallocate */

    ತಾರ್ಕಿಕ overflows =
      (ಪೂರ್ಣಾಙ್ಕ) in_error () ||
      (new_allocated < size) ||
      hb_unsigned_mul_overflows (new_allocated, ಪರಿಮಾಣ (Type));

    ಯದಿ (unlikely (overflows))
    {
      set_error ();
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    Type *new_array = realloc_vector (new_allocated, hb_prioritize);

    ಯದಿ (unlikely (new_allocated && !new_array))
    {
      ಯದಿ (new_allocated <= (unsigned) allocated)
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; // shrinking failed; it's okay; happens in our fuzzer

      set_error ();
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    arrayZ = new_array;
    allocated = new_allocated;

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  ತಾರ್ಕಿಕ alloc_exact (unsigned ಪೂರ್ಣಾಙ್ಕ size)
  {
    ಪ್ರತ್ಯಾಗಮ alloc (size, ಸತ್ಯ);
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  ಶೂನ್ಯ clear ()
  {
    shrink_vector (0);
  }

  template <typename allocator_t>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  ತಾರ್ಕಿಕ allocate_from_pool (allocator_t *allocator, unsigned size, unsigned ಪೂರ್ಣಾಙ್ಕ initialize = ಸತ್ಯ)
  {
    ಯದಿ (allocator)
    {
      ಪ್ರತಿಜ್ಞಾ (!length && !allocated);
      arrayZ = (Type *) allocator->alloc (size * ಪರಿಮಾಣ (Type), alignof (Type));
      ಯದಿ (unlikely (!arrayZ))
      {
	set_error ();
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      }
      ಯದಿ (initialize)
	grow_vector (size, hb_prioritize);
      ಅನ್ಯಥಾ
	length = size;
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ resize_full ((ಪೂರ್ಣಾಙ್ಕ) size, initialize, ಸತ್ಯ);
  }

  template <typename allocator_t>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  ತಾರ್ಕಿಕ duplicate_vector_from_pool (allocator_t *allocator, ಧ್ರುವ hb_vector_t &other)
  {
    ಯದಿ (unlikely (!allocate_from_pool (allocator, other.length, ಅಸತ್ಯ)))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    length = 0;
    copy_array (other.as_array ());
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  template <typename allocator_t>
  ಶೂನ್ಯ shrink_back_to_pool (allocator_t *allocator, ಪೂರ್ಣಾಙ್ಕ size)
  {
    unsigned orig_length = length;

    shrink (size, ಅಸತ್ಯ);

    ಯದಿ (allocator && !is_owned ())
      allocator->discard (arrayZ + length, (orig_length - length) * ಪರಿಮಾಣ (Type));
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  ತಾರ್ಕಿಕ resize_full (ಪೂರ್ಣಾಙ್ಕ size_, ತಾರ್ಕಿಕ initialize, ತಾರ್ಕಿಕ exact)
  {
    ಯದಿ (unlikely (size_ < 0))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    unsigned ಪೂರ್ಣಾಙ್ಕ size = (unsigned ಪೂರ್ಣಾಙ್ಕ) size_;
    ಯದಿ (!alloc (size, exact))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಯದಿ (size > length)
    {
      ಯದಿ (initialize)
	grow_vector (size, hb_prioritize);
    }
    ಅನ್ಯಥಾ ಯದಿ (size < length)
    {
      ಯದಿ (initialize)
	shrink_vector (size);
    }

    length = size;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  ತಾರ್ಕಿಕ resize (ಪೂರ್ಣಾಙ್ಕ size_)
  {
    ಪ್ರತ್ಯಾಗಮ resize_full (size_, ಸತ್ಯ, ಅಸತ್ಯ);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  ತಾರ್ಕಿಕ resize_dirty (ಪೂರ್ಣಾಙ್ಕ size_)
  {
    ಪ್ರತ್ಯಾಗಮ resize_full (size_, ಅಸತ್ಯ, ಅಸತ್ಯ);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  ತಾರ್ಕಿಕ resize_exact (ಪೂರ್ಣಾಙ್ಕ size_)
  {
    ಪ್ರತ್ಯಾಗಮ resize_full (size_, ಸತ್ಯ, ಸತ್ಯ);
  }

  Type ತೆಗೆ ()
  {
    ಯದಿ (!length) ಪ್ರತ್ಯಾಗಮ Null (Type);
    Type v (std::move (arrayZ[length - 1]));
    arrayZ[length - 1].~Type ();
    length--;
    ಪ್ರತ್ಯಾಗಮ v;
  }

  ಶೂನ್ಯ remove_ordered (unsigned ಪೂರ್ಣಾಙ್ಕ i)
  {
    ಯದಿ (unlikely (i >= length))
      ಪ್ರತ್ಯಾಗಮ;
    shift_down_vector (i + 1);
    arrayZ[length - 1].~Type ();
    length--;
  }

  template <ತಾರ್ಕಿಕ Sorted = sorted,
	    hb_enable_if (!Sorted)>
  ಶೂನ್ಯ remove_unordered (unsigned ಪೂರ್ಣಾಙ್ಕ i)
  {
    ಯದಿ (unlikely (i >= length))
      ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (i != length - 1)
      arrayZ[i] = std::move (arrayZ[length - 1]);
    arrayZ[length - 1].~Type ();
    length--;
  }

  ಶೂನ್ಯ shrink (ಪೂರ್ಣಾಙ್ಕ size_, ತಾರ್ಕಿಕ shrink_memory = ಸತ್ಯ)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ size = size_ < 0 ? 0u : (unsigned ಪೂರ್ಣಾಙ್ಕ) size_;
    ಯದಿ (size >= length)
      ಪ್ರತ್ಯಾಗಮ;

    shrink_vector (size);

    ಯದಿ (is_owned () && shrink_memory)
      alloc_exact (size); /* To force shrinking memory if needed. */
  }


  /* Sorting API. */
  template <typename Compar>
  ಶೂನ್ಯ qsort (Compar compar)
  { as_array ().qsort (compar); }
  ಶೂನ್ಯ qsort ()
  { as_array ().qsort (); }

  /* Unsorted search API. */
  template <typename T>
  Type *lsearch (ಧ್ರುವ T &x, Type *not_found = nullptr)
  { ಪ್ರತ್ಯಾಗಮ as_array ().lsearch (x, not_found); }
  template <typename T>
  ಧ್ರುವ Type *lsearch (ಧ್ರುವ T &x, ಧ್ರುವ Type *not_found = nullptr) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ as_array ().lsearch (x, not_found); }
  template <typename T>
  ತಾರ್ಕಿಕ lfind (ಧ್ರುವ T &x, unsigned *pos = nullptr) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ as_array ().lfind (x, pos); }

  /* Sorted search API. */
  template <typename T,
	    ತಾರ್ಕಿಕ Sorted=sorted, hb_enable_if (Sorted)>
  Type *bsearch (ಧ್ರುವ T &x, Type *not_found = nullptr)
  { ಪ್ರತ್ಯಾಗಮ as_array ().bsearch (x, not_found); }
  template <typename T,
	    ತಾರ್ಕಿಕ Sorted=sorted, hb_enable_if (Sorted)>
  ಧ್ರುವ Type *bsearch (ಧ್ರುವ T &x, ಧ್ರುವ Type *not_found = nullptr) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ as_array ().bsearch (x, not_found); }
  template <typename T,
	    ತಾರ್ಕಿಕ Sorted=sorted, hb_enable_if (Sorted)>
  ತಾರ್ಕಿಕ bfind (ಧ್ರುವ T &x, unsigned ಪೂರ್ಣಾಙ್ಕ *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned ಪೂರ್ಣಾಙ್ಕ to_store = (unsigned ಪೂರ್ಣಾಙ್ಕ) -1) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ as_array ().bfind (x, i, not_found, to_store); }
};

template <typename Type>
using hb_sorted_vector_t = hb_vector_t<Type, ಸತ್ಯ>;

#endif /* HB_VECTOR_HH */

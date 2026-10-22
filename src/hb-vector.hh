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

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-array.hh"
#ଅନ୍ତର୍ଭାଵ "hb-meta.hh"
#ଅନ୍ତର୍ଭାଵ "hb-null.hh"

// Change to 1 to force inline vector allocs, to see callsite in malloc-stats tool.
#ଯଦି 0
#define HB_ALWAYS_INLINE_VECTOR_ALLOCS HB_ALWAYS_INLINE
#ଅନ୍ୟଥା
#define HB_ALWAYS_INLINE_VECTOR_ALLOCS
#endif

template <typename Type,
	  ତାର୍କିକ sorted=ମିଥ୍ୟା>
ସଂରଚନା hb_vector_t
{
  ସ୍ଥିର constexpr ତାର୍କିକ realloc_move = ସତ୍ୟ;

  ପ୍ରକାରବ୍ୟାଖ୍ୟା Type item_t;
  ସ୍ଥିର constexpr unsigned item_size = hb_static_size (Type);
  using array_t = typename std::conditional<sorted, hb_sorted_array_t<Type>, hb_array_t<Type>>::type;
  using c_array_t = typename std::conditional<sorted, hb_sorted_array_t<ଅଚଳ Type>, hb_array_t<ଅଚଳ Type>>::type;

  hb_vector_t () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (std::initializer_list<Type> lst) : hb_vector_t ()
  {
    alloc (lst.size (), ସତ୍ୟ);
    ପାଇଁ (auto&& item : lst)
      push (item);
  }
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  explicit hb_vector_t (ଅଚଳ Iterable &o) : hb_vector_t ()
  {
    extend (o, ସତ୍ୟ);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (ଅଚଳ hb_vector_t &o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    ଯଦି (unlikely (in_error ())) ଫେରନ୍ତୁ;
    copy_array (o.as_array ());
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (array_t o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    ଯଦି (unlikely (in_error ())) ଫେରନ୍ତୁ;
    copy_array (o);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (c_array_t o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    ଯଦି (unlikely (in_error ())) ଫେରନ୍ତୁ;
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
  ଶୂନ୍ୟ
  set_storage (Type (&array)[n])
  { set_storage (array, n); }
  ଶୂନ୍ୟ
  set_storage (hb_array_t<Type> array)
  { set_storage (array.arrayZ, array.length); }
  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T) &&
			  hb_is_trivially_destructible(T))>
  ଶୂନ୍ୟ
  set_storage (Type *array, unsigned n)
  {
    ପ୍ରତିଜ୍ଞା (allocated == 0);
    ପ୍ରତିଜ୍ଞା (length == 0);

    arrayZ = array;
    length = n;
  }

  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  ଶୂନ୍ୟ extend (ଅଚଳ Iterable &o, ତାର୍କିକ exact=ମିଥ୍ୟା)
  {
    auto iter = hb_iter (o);
    ଯଦି (iter.is_random_access_iterator || iter.has_fast_len)
    {
      ଯଦି (unlikely (!alloc (length + hb_len (iter), exact)))
	ଫେରନ୍ତୁ;
      unsigned ଗଣନା_2 = hb_len (iter);
      ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
	ଠେଲ_has_room (*iter++);
    }
    ଯେତେବେଳେ (iter)
    {
      ଯଦି (unlikely (!alloc (length + 1)))
        ଫେରନ୍ତୁ;
      unsigned room = allocated - length;
      ପାଇଁ (unsigned i = 0; i < room && iter; i++)
	ଠେଲ_has_room (*iter++);
    }
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  ଶୂନ୍ୟ extend (array_t o, ତାର୍କିକ exact=ମିଥ୍ୟା)
  { extend (c_array_t (o), exact); }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  ଶୂନ୍ୟ extend (c_array_t o, ତାର୍କିକ exact=ମିଥ୍ୟା)
  {
    /* o may point into our own storage, eg. v.extend (v.as_array ()).
     * alloc() may move that storage, so find o again afterwards. */
    ତାର୍କିକ aliased = o.length && in_storage (*o.arrayZ);
    unsigned offset = aliased ? o.arrayZ - arrayZ : 0;
    alloc (length + o.length, exact);
    ଯଦି (unlikely (in_error ())) ଫେରନ୍ତୁ;
    ଯଦି (unlikely (aliased))
      o = {arrayZ + offset, o.length};
    copy_array (o);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  ଶୂନ୍ୟ extend (ଅଚଳ hb_vector_t &o, ତାର୍କିକ exact=ମିଥ୍ୟା)
  { extend (o.as_array (), exact); }

  ସର୍ବସାଧାରଣ:
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା allocated = 0; /* < 0 means allocation failed. */
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା length = 0;
  ସର୍ବସାଧାରଣ:
  Type *arrayZ = nullptr;

  ଶୂନ୍ୟ init ()
  {
    allocated = length = 0;
    arrayZ = nullptr;
  }
  ଶୂନ୍ୟ init0 ()
  {
  }

  ଶୂନ୍ୟ fini ()
  {
    ଯଦି (is_owned ())
    {
      shrink_vector (0);
      hb_free (arrayZ);
    }
    init ();
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t &reset ()
  {
    ଯଦି (unlikely (in_error ()))
      reset_error ();
    clear ();
    ଫେରନ୍ତୁ *ଏହି;
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t &reset_if_error ()
  {
    ଯଦି (unlikely (in_error ()))
      reset ();
    ଫେରନ୍ତୁ *ଏହି;
  }

  /* Transfer ownership of the backing storage to caller.
   * Returns nullptr if storage is not owned by this vector. */
  Type *
  steal (unsigned *len = nullptr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *allocated_out = nullptr)
  {
    ଯଦି (!is_owned ())
      ଫେରନ୍ତୁ nullptr;
    ଯଦି (len)
      *len = length;
    ଯଦି (allocated_out)
      *allocated_out = allocated;
    Type *p = arrayZ;
    init ();
    ଫେରନ୍ତୁ p;
  }

  /* Adopt a previously detached owned buffer. */
  ଶୂନ୍ୟ
  recycle_buffer (Type *buffer,
                  unsigned len,
                  ପୂର୍ଣ୍ଣସଂଖ୍ୟା allocated_len)
  {
    fini ();
    arrayZ = buffer;
    length = len;
    allocated = allocated_len;
  }

  friend ଶୂନ୍ୟ swap (hb_vector_t& a, hb_vector_t& b) noexcept
  {
    hb_swap (a.allocated, b.allocated);
    hb_swap (a.length, b.length);
    hb_swap (a.arrayZ, b.arrayZ);
  }

  hb_vector_t& operator = (ଅଚଳ hb_vector_t &o)
  {
    ଯଦି (ଏହି == &o) ଫେରନ୍ତୁ *ଏହି;

    reset ();
    alloc_exact (o.length);
    ଯଦି (unlikely (in_error ())) ଫେରନ୍ତୁ *ଏହି;

    length = 0;
    copy_array (o.as_array ());

    ଫେରନ୍ତୁ *ଏହି;
  }
  hb_vector_t& operator = (hb_vector_t &&o) noexcept
  {
    hb_swap (*ଏହି, o);
    ଫେରନ୍ତୁ *ଏହି;
  }

  hb_bytes_t as_bytes () ଅଚଳ
  { ଫେରନ୍ତୁ hb_bytes_t ((ଅଚଳ ଅକ୍ଷର *) arrayZ, get_size ()); }

  ତାର୍କିକ operator == (ଅଚଳ hb_vector_t &o) ଅଚଳ { ଫେରନ୍ତୁ as_array () == o.as_array (); }
  ତାର୍କିକ operator != (ଅଚଳ hb_vector_t &o) ଅଚଳ { ଫେରନ୍ତୁ !(*ଏହି == o); }
  uint32_t hash () ଅଚଳ { ଫେରନ୍ତୁ as_array ().hash (); }

  Type& operator [] (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i_)
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) i_;
    ଯଦି (unlikely (i >= length))
      ଫେରନ୍ତୁ Crap (Type);
    ଫେରନ୍ତୁ arrayZ[i];
  }
  ଅଚଳ Type& operator [] (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i_) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) i_;
    ଯଦି (unlikely (i >= length))
      ଫେରନ୍ତୁ Null (Type);
    ଫେରନ୍ତୁ arrayZ[i];
  }

  Type& tail () { ଫେରନ୍ତୁ (*ଏହି)[length - 1]; }
  ଅଚଳ Type& tail () ଅଚଳ { ଫେରନ୍ତୁ (*ଏହି)[length - 1]; }

  explicit operator ତାର୍କିକ () ଅଚଳ { ଫେରନ୍ତୁ length; }
  size_t get_size () ଅଚଳ { ଫେରନ୍ତୁ hb_unsigned_mul_saturate (length, item_size); }

  /* Sink interface. */
  template <typename T>
  hb_vector_t& operator << (T&& v) { push (std::forward<T> (v)); ଫେରନ୍ତୁ *ଏହି; }

  array_t   as_array ()       { ଫେରନ୍ତୁ hb_array (arrayZ, length); }
  c_array_t as_array () ଅଚଳ { ଫେରନ୍ତୁ hb_array (arrayZ, length); }

  /* Iterator. */
  ପ୍ରକାରବ୍ୟାଖ୍ୟା c_array_t   iter_t;
  ପ୍ରକାରବ୍ୟାଖ୍ୟା array_t   writer_t;
    iter_t   iter () ଅଚଳ { ଫେରନ୍ତୁ as_array (); }
  writer_t writer ()       { ଫେରନ୍ତୁ as_array (); }
  operator   iter_t () ଅଚଳ { ଫେରନ୍ତୁ   iter (); }
  operator writer_t ()       { ଫେରନ୍ତୁ writer (); }

  /* Faster range-based for loop. */
  Type *begin () ଅଚଳ { ଫେରନ୍ତୁ arrayZ; }
  Type *end () ଅଚଳ { ଫେରନ୍ତୁ arrayZ + length; }


  hb_sorted_array_t<Type> as_sorted_array ()
  { ଫେରନ୍ତୁ hb_sorted_array (arrayZ, length); }
  hb_sorted_array_t<ଅଚଳ Type> as_sorted_array () ଅଚଳ
  { ଫେରନ୍ତୁ hb_sorted_array (arrayZ, length); }

  template <typename T> explicit operator T * () { ଫେରନ୍ତୁ arrayZ; }
  template <typename T> explicit operator ଅଚଳ T * () ଅଚଳ { ଫେରନ୍ତୁ arrayZ; }

  Type * operator  + (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) { ଫେରନ୍ତୁ arrayZ + i; }
  ଅଚଳ Type * operator  + (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ { ଫେରନ୍ତୁ arrayZ + i; }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push ()
  {
    ଯଦି (unlikely (!resize (length + 1)))
      ଫେରନ୍ତୁ std::addressof (Crap (Type));
    ଫେରନ୍ତୁ std::addressof (arrayZ[length - 1]);
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push (Args&&... args)
  {
    ଯଦି (unlikely ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) length >= allocated))
    {
      ଯଦି (unlikely (in_error ()))
	ଫେରନ୍ତୁ std::addressof (Crap (Type));

      /* An argument may live in our own storage, eg. v.push (v[i]).
       * alloc() frees that storage, so copy it out first. */
      ଯଦି (unlikely (in_storage (args...)))
      {
	Type copy (std::forward<Args> (args)...);
	ଯଦି (unlikely (!alloc (length + 1)))
	  ଫେରନ୍ତୁ std::addressof (Crap (Type));
	ଫେରନ୍ତୁ ଠେଲ_has_room (std::move (copy));
      }
      ଯଦି (unlikely (!alloc (length + 1)))
	// If push failed to allocate then don't copy v, since this may cause
	// the created copy to leak memory since we won't have stored a
	// reference to it.
	ଫେରନ୍ତୁ std::addressof (Crap (Type));
    }

    ଫେରନ୍ତୁ ଠେଲ_has_room (std::forward<Args> (args)...);
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  ତାର୍କିକ ଠେଲ_or_fail (Args&&... args)
  {
    ଫେରନ୍ତୁ push (std::forward<Args> (args)...) != std::addressof (Crap (Type));
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *ଠେଲ_has_room (Args&&... args)
  {
    /* Emplace. */
    Type *p = std::addressof (arrayZ[length++]);
    ଫେରନ୍ତୁ ନୂତନ (p) Type (std::forward<Args> (args)...);
  }

  ତାର୍କିକ in_storage () ଅଚଳ { ଫେରନ୍ତୁ ମିଥ୍ୟା; }
  template <typename T, typename... Ts>
  ତାର୍କିକ in_storage (ଅଚଳ T &v, ଅଚଳ Ts &...vs) ଅଚଳ
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    ଫେରନ୍ତୁ (arrayZ &&
	    (uintptr_t) arrayZ <= p && p < (uintptr_t) (arrayZ + length)) ||
	   in_storage (vs...);
  }

  ତାର୍କିକ is_owned () ଅଚଳ
  {
    ଫେରନ୍ତୁ allocated != 0 && allocated != -1;
  }

  ତାର୍କିକ in_error () ଅଚଳ { ଫେରନ୍ତୁ allocated < 0; }
  ଶୂନ୍ୟ set_error ()
  {
    ପ୍ରତିଜ୍ଞା (allocated >= 0);
    allocated = -allocated - 1;
  }
  ଶୂନ୍ୟ reset_error ()
  {
    ପ୍ରତିଜ୍ଞା (allocated < 0);
    allocated = -(allocated + 1);
  }
  ଶୂନ୍ୟ ensure_error ()
  {
    ଯଦି (!in_error ())
      set_error ();
  }

  Type *
  _realloc (unsigned new_allocated)
  {
    ଯଦି (!new_allocated)
    {
      ଯଦି (is_owned ())
	hb_free (arrayZ);
      ଫେରନ୍ତୁ nullptr;
    }
    ଯଦି (!allocated && arrayZ)
    {
      /* If we have a non-null arrayZ but allocated is 0, then we are
       * reallocating from a foreign array. */
      Type *new_array = (Type *) hb_malloc (new_allocated * ଆକାର (Type));
      ଯଦି (unlikely (!new_array))
	ଫେରନ୍ତୁ nullptr;
      hb_memcpy ((ଶୂନ୍ୟ *) new_array, (ଅଚଳ ଶୂନ୍ୟ *) arrayZ, length * ଆକାର (Type));
      ଫେରନ୍ତୁ new_array;
    }
    ଫେରନ୍ତୁ (Type *) hb_realloc (arrayZ, new_allocated * ଆକାର (Type));
  }
  Type *
  _malloc_move (unsigned new_allocated)
  {
    ଯଦି (!new_allocated)
    {
      ଯଦି (is_owned ())
	hb_free (arrayZ);
      ଫେରନ୍ତୁ nullptr;
    }
    Type *new_array = (Type *) hb_malloc (new_allocated * ଆକାର (Type));
    ଯଦି (likely (new_array))
    {
      ପାଇଁ (unsigned i = 0; i < length; i++)
      {
	ନୂତନ (std::addressof (new_array[i])) Type ();
	new_array[i] = std::move (arrayZ[i]);
	arrayZ[i].~Type ();
      }
      ଯଦି (is_owned ())
	hb_free (arrayZ);
    }
    ଫେରନ୍ତୁ new_array;
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<0>)
  {
    ଫେରନ୍ତୁ _realloc (new_allocated);
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<0>)
  {
    ଫେରନ୍ତୁ _malloc_move (new_allocated);
  }
  /* Specialization for types that can be moved using realloc(). */
  template <typename T = Type,
	    hb_enable_if (T::realloc_move)>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<1>)
  {
    ଫେରନ୍ତୁ _realloc (new_allocated);
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T))>
  ଶୂନ୍ୟ
  grow_vector (unsigned size, hb_priority<0>)
  {
    hb_memset (arrayZ + length, 0, (size - length) * ଆକାର (*arrayZ));
    length = size;
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_constructible(T))>
  ଶୂନ୍ୟ
  grow_vector (unsigned size, hb_priority<0>)
  {
    ପାଇଁ (; length < size; length++)
      ନୂତନ (std::addressof (arrayZ[length])) Type ();
  }
  /* Specialization for hb_vector_t<hb_{vector,array}_t<U>> to speed up. */
  template <typename T = Type,
	    hb_enable_if (hb_is_same (T, hb_vector_t<typename T::item_t>) ||
			  hb_is_same (T, hb_array_t <typename T::item_t>))>
  ଶୂନ୍ୟ
  grow_vector (unsigned size, hb_priority<1>)
  {
    hb_memset (arrayZ + length, 0, (size - length) * ଆକାର (*arrayZ));
    length = size;
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  ଶୂନ୍ୟ
  copy_array (hb_array_t<Type> other)
  {
    hb_memcpy ((ଶୂନ୍ୟ *) (arrayZ + length), (ଅଚଳ ଶୂନ୍ୟ *) other.arrayZ, other.length * item_size);
    length += other.length;
  }
  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  ଶୂନ୍ୟ
  copy_array (hb_array_t<ଅଚଳ Type> other)
  {
    hb_memcpy ((ଶୂନ୍ୟ *) (arrayZ + length), (ଅଚଳ ଶୂନ୍ୟ *) other.arrayZ, other.length * item_size);
    length += other.length;
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			   std::is_copy_constructible<T>::value)>
  ଶୂନ୍ୟ
  copy_array (hb_array_t<ଅଚଳ Type> other)
  {
    ପାଇଁ (unsigned i = 0; i < other.length; i++)
      ନୂତନ (std::addressof (arrayZ[length + i])) Type (other.arrayZ[i]);
    length += other.length;
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			  !std::is_copy_constructible<T>::value &&
			  std::is_default_constructible<T>::value &&
			  std::is_copy_assignable<T>::value)>
  ଶୂନ୍ୟ
  copy_array (hb_array_t<ଅଚଳ Type> other)
  {
    ପାଇଁ (unsigned i = 0; i < other.length; i++)
    {
      ନୂତନ (std::addressof (arrayZ[length + i])) Type ();
      arrayZ[length + i] = other.arrayZ[i];
    }
    length += other.length;
  }

  ଶୂନ୍ୟ
  shrink_vector (unsigned size)
  {
    ପ୍ରତିଜ୍ଞା (size <= length);
    ଯଦି (!hb_is_trivially_destructible(Type))
    {
      unsigned ଗଣନା_2 = length - size;
      Type *p = arrayZ + length;
      ଯେତେବେଳେ (ଗଣନା_2--)
        (--p)->~Type ();
    }
    length = size;
  }

  ଶୂନ୍ୟ
  shift_down_vector (unsigned i)
  {
    ପାଇଁ (; i < length; i++)
      arrayZ[i - 1] = std::move (arrayZ[i]);
  }

  /* Allocate for size but don't adjust length. */
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  ତାର୍କିକ alloc (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ତାର୍କିକ exact=ମିଥ୍ୟା)
  {
    ଯଦି (unlikely (in_error ()))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_allocated;
    ଯଦି (exact)
    {
      /* If exact was specified, we allow shrinking the storage. */
      size = hb_max (size, length);
      ଯଦି (size <= (unsigned) allocated &&
	  size >= (unsigned) allocated >> 2)
	ଫେରନ୍ତୁ ସତ୍ୟ;

      new_allocated = size;
    }
    ଅନ୍ୟଥା
    {
      ଯଦି (likely (size <= (unsigned) allocated))
	ଫେରନ୍ତୁ ସତ୍ୟ;

      new_allocated = allocated;
      ଯେତେବେଳେ (size > new_allocated)
	new_allocated += (new_allocated >> 1) + 8;
    }

    /* Reallocate */

    ତାର୍କିକ overflows =
      (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) in_error () ||
      (new_allocated < size) ||
      hb_unsigned_mul_overflows (new_allocated, ଆକାର (Type));

    ଯଦି (unlikely (overflows))
    {
      set_error ();
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    Type *new_array = realloc_vector (new_allocated, hb_prioritize);

    ଯଦି (unlikely (new_allocated && !new_array))
    {
      ଯଦି (new_allocated <= (unsigned) allocated)
        ଫେରନ୍ତୁ ସତ୍ୟ; // shrinking failed; it's okay; happens in our fuzzer

      set_error ();
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    arrayZ = new_array;
    allocated = new_allocated;

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  ତାର୍କିକ alloc_exact (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା size)
  {
    ଫେରନ୍ତୁ alloc (size, ସତ୍ୟ);
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  ଶୂନ୍ୟ clear ()
  {
    shrink_vector (0);
  }

  template <typename allocator_t>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  ତାର୍କିକ allocate_from_pool (allocator_t *allocator, unsigned size, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା initialize = ସତ୍ୟ)
  {
    ଯଦି (allocator)
    {
      ପ୍ରତିଜ୍ଞା (!length && !allocated);
      arrayZ = (Type *) allocator->alloc (size * ଆକାର (Type), alignof (Type));
      ଯଦି (unlikely (!arrayZ))
      {
	set_error ();
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }
      ଯଦି (initialize)
	grow_vector (size, hb_prioritize);
      ଅନ୍ୟଥା
	length = size;
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }
    ଫେରନ୍ତୁ resize_full ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) size, initialize, ସତ୍ୟ);
  }

  template <typename allocator_t>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  ତାର୍କିକ duplicate_vector_from_pool (allocator_t *allocator, ଅଚଳ hb_vector_t &other)
  {
    ଯଦି (unlikely (!allocate_from_pool (allocator, other.length, ମିଥ୍ୟା)))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    length = 0;
    copy_array (other.as_array ());
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  template <typename allocator_t>
  ଶୂନ୍ୟ shrink_back_to_pool (allocator_t *allocator, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size)
  {
    unsigned orig_length = length;

    shrink (size, ମିଥ୍ୟା);

    ଯଦି (allocator && !is_owned ())
      allocator->discard (arrayZ + length, (orig_length - length) * ଆକାର (Type));
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  ତାର୍କିକ resize_full (ପୂର୍ଣ୍ଣସଂଖ୍ୟା size_, ତାର୍କିକ initialize, ତାର୍କିକ exact)
  {
    ଯଦି (unlikely (size_ < 0))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା size = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) size_;
    ଯଦି (!alloc (size, exact))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଯଦି (size > length)
    {
      ଯଦି (initialize)
	grow_vector (size, hb_prioritize);
    }
    ଅନ୍ୟଥା ଯଦି (size < length)
    {
      ଯଦି (initialize)
	shrink_vector (size);
    }

    length = size;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  ତାର୍କିକ resize (ପୂର୍ଣ୍ଣସଂଖ୍ୟା size_)
  {
    ଫେରନ୍ତୁ resize_full (size_, ସତ୍ୟ, ମିଥ୍ୟା);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  ତାର୍କିକ resize_dirty (ପୂର୍ଣ୍ଣସଂଖ୍ୟା size_)
  {
    ଫେରନ୍ତୁ resize_full (size_, ମିଥ୍ୟା, ମିଥ୍ୟା);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  ତାର୍କିକ resize_exact (ପୂର୍ଣ୍ଣସଂଖ୍ୟା size_)
  {
    ଫେରନ୍ତୁ resize_full (size_, ସତ୍ୟ, ସତ୍ୟ);
  }

  Type କାଢ଼ ()
  {
    ଯଦି (!length) ଫେରନ୍ତୁ Null (Type);
    Type v (std::move (arrayZ[length - 1]));
    arrayZ[length - 1].~Type ();
    length--;
    ଫେରନ୍ତୁ v;
  }

  ଶୂନ୍ୟ remove_ordered (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i)
  {
    ଯଦି (unlikely (i >= length))
      ଫେରନ୍ତୁ;
    shift_down_vector (i + 1);
    arrayZ[length - 1].~Type ();
    length--;
  }

  template <ତାର୍କିକ Sorted = sorted,
	    hb_enable_if (!Sorted)>
  ଶୂନ୍ୟ remove_unordered (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i)
  {
    ଯଦି (unlikely (i >= length))
      ଫେରନ୍ତୁ;
    ଯଦି (i != length - 1)
      arrayZ[i] = std::move (arrayZ[length - 1]);
    arrayZ[length - 1].~Type ();
    length--;
  }

  ଶୂନ୍ୟ shrink (ପୂର୍ଣ୍ଣସଂଖ୍ୟା size_, ତାର୍କିକ shrink_memory = ସତ୍ୟ)
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା size = size_ < 0 ? 0u : (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) size_;
    ଯଦି (size >= length)
      ଫେରନ୍ତୁ;

    shrink_vector (size);

    ଯଦି (is_owned () && shrink_memory)
      alloc_exact (size); /* To force shrinking memory if needed. */
  }


  /* Sorting API. */
  template <typename Compar>
  ଶୂନ୍ୟ qsort (Compar compar)
  { as_array ().qsort (compar); }
  ଶୂନ୍ୟ qsort ()
  { as_array ().qsort (); }

  /* Unsorted search API. */
  template <typename T>
  Type *lsearch (ଅଚଳ T &x, Type *not_found = nullptr)
  { ଫେରନ୍ତୁ as_array ().lsearch (x, not_found); }
  template <typename T>
  ଅଚଳ Type *lsearch (ଅଚଳ T &x, ଅଚଳ Type *not_found = nullptr) ଅଚଳ
  { ଫେରନ୍ତୁ as_array ().lsearch (x, not_found); }
  template <typename T>
  ତାର୍କିକ lfind (ଅଚଳ T &x, unsigned *pos = nullptr) ଅଚଳ
  { ଫେରନ୍ତୁ as_array ().lfind (x, pos); }

  /* Sorted search API. */
  template <typename T,
	    ତାର୍କିକ Sorted=sorted, hb_enable_if (Sorted)>
  Type *bsearch (ଅଚଳ T &x, Type *not_found = nullptr)
  { ଫେରନ୍ତୁ as_array ().bsearch (x, not_found); }
  template <typename T,
	    ତାର୍କିକ Sorted=sorted, hb_enable_if (Sorted)>
  ଅଚଳ Type *bsearch (ଅଚଳ T &x, ଅଚଳ Type *not_found = nullptr) ଅଚଳ
  { ଫେରନ୍ତୁ as_array ().bsearch (x, not_found); }
  template <typename T,
	    ତାର୍କିକ Sorted=sorted, hb_enable_if (Sorted)>
  ତାର୍କିକ bfind (ଅଚଳ T &x, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା to_store = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) -1) ଅଚଳ
  { ଫେରନ୍ତୁ as_array ().bfind (x, i, not_found, to_store); }
};

template <typename Type>
using hb_sorted_vector_t = hb_vector_t<Type, ସତ୍ୟ>;

#endif /* HB_VECTOR_HH */

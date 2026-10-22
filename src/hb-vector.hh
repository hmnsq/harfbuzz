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

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-array.hh"
#അന്തര്ഭാവ "hb-meta.hh"
#അന്തര്ഭാവ "hb-null.hh"

// Change to 1 to force inline vector allocs, to see callsite in malloc-stats tool.
#എങ്കിൽ 0
#define HB_ALWAYS_INLINE_VECTOR_ALLOCS HB_ALWAYS_INLINE
#അല്ലെങ്കിൽ
#define HB_ALWAYS_INLINE_VECTOR_ALLOCS
#endif

template <typename Type,
	  തർക്കം sorted=അസത്യം>
ഘടന hb_vector_t
{
  സ്ഥിതം constexpr തർക്കം realloc_move = സത്യം;

  തരനിർവചനം Type item_t;
  സ്ഥിതം constexpr unsigned item_size = hb_static_size (Type);
  using array_t = typename std::conditional<sorted, hb_sorted_array_t<Type>, hb_array_t<Type>>::type;
  using c_array_t = typename std::conditional<sorted, hb_sorted_array_t<സ്ഥിരം Type>, hb_array_t<സ്ഥിരം Type>>::type;

  hb_vector_t () = സ്ഥിരസ്ഥിതി;
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (std::initializer_list<Type> lst) : hb_vector_t ()
  {
    alloc (lst.size (), സത്യം);
    വേണ്ടി (auto&& item : lst)
      push (item);
  }
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  explicit hb_vector_t (സ്ഥിരം Iterable &o) : hb_vector_t ()
  {
    extend (o, സത്യം);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (സ്ഥിരം hb_vector_t &o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    എങ്കിൽ (unlikely (in_error ())) തിരികെ;
    copy_array (o.as_array ());
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (array_t o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    എങ്കിൽ (unlikely (in_error ())) തിരികെ;
    copy_array (o);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (c_array_t o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    എങ്കിൽ (unlikely (in_error ())) തിരികെ;
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
  ശൂന്യം
  set_storage (Type (&array)[n])
  { set_storage (array, n); }
  ശൂന്യം
  set_storage (hb_array_t<Type> array)
  { set_storage (array.arrayZ, array.length); }
  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T) &&
			  hb_is_trivially_destructible(T))>
  ശൂന്യം
  set_storage (Type *array, unsigned n)
  {
    പ്രതിജ്ഞാ (allocated == 0);
    പ്രതിജ്ഞാ (length == 0);

    arrayZ = array;
    length = n;
  }

  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  ശൂന്യം extend (സ്ഥിരം Iterable &o, തർക്കം exact=അസത്യം)
  {
    auto iter = hb_iter (o);
    എങ്കിൽ (iter.is_random_access_iterator || iter.has_fast_len)
    {
      എങ്കിൽ (unlikely (!alloc (length + hb_len (iter), exact)))
	തിരികെ;
      unsigned എണ്ണം = hb_len (iter);
      വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
	തള്ളുക_has_room (*iter++);
    }
    ആകുമ്പോൾ (iter)
    {
      എങ്കിൽ (unlikely (!alloc (length + 1)))
        തിരികെ;
      unsigned room = allocated - length;
      വേണ്ടി (unsigned i = 0; i < room && iter; i++)
	തള്ളുക_has_room (*iter++);
    }
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  ശൂന്യം extend (array_t o, തർക്കം exact=അസത്യം)
  { extend (c_array_t (o), exact); }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  ശൂന്യം extend (c_array_t o, തർക്കം exact=അസത്യം)
  {
    /* o may point into our own storage, eg. v.extend (v.as_array ()).
     * alloc() may move that storage, so find o again afterwards. */
    തർക്കം aliased = o.length && in_storage (*o.arrayZ);
    unsigned offset = aliased ? o.arrayZ - arrayZ : 0;
    alloc (length + o.length, exact);
    എങ്കിൽ (unlikely (in_error ())) തിരികെ;
    എങ്കിൽ (unlikely (aliased))
      o = {arrayZ + offset, o.length};
    copy_array (o);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  ശൂന്യം extend (സ്ഥിരം hb_vector_t &o, തർക്കം exact=അസത്യം)
  { extend (o.as_array (), exact); }

  പൊതു:
  പൂർണ്ണസംഖ്യ allocated = 0; /* < 0 means allocation failed. */
  unsigned പൂർണ്ണസംഖ്യ length = 0;
  പൊതു:
  Type *arrayZ = nullptr;

  ശൂന്യം init ()
  {
    allocated = length = 0;
    arrayZ = nullptr;
  }
  ശൂന്യം init0 ()
  {
  }

  ശൂന്യം fini ()
  {
    എങ്കിൽ (is_owned ())
    {
      shrink_vector (0);
      hb_free (arrayZ);
    }
    init ();
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t &reset ()
  {
    എങ്കിൽ (unlikely (in_error ()))
      reset_error ();
    clear ();
    തിരികെ *ഇത്;
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t &reset_if_error ()
  {
    എങ്കിൽ (unlikely (in_error ()))
      reset ();
    തിരികെ *ഇത്;
  }

  /* Transfer ownership of the backing storage to caller.
   * Returns nullptr if storage is not owned by this vector. */
  Type *
  steal (unsigned *len = nullptr, പൂർണ്ണസംഖ്യ *allocated_out = nullptr)
  {
    എങ്കിൽ (!is_owned ())
      തിരികെ nullptr;
    എങ്കിൽ (len)
      *len = length;
    എങ്കിൽ (allocated_out)
      *allocated_out = allocated;
    Type *p = arrayZ;
    init ();
    തിരികെ p;
  }

  /* Adopt a previously detached owned buffer. */
  ശൂന്യം
  recycle_buffer (Type *buffer,
                  unsigned len,
                  പൂർണ്ണസംഖ്യ allocated_len)
  {
    fini ();
    arrayZ = buffer;
    length = len;
    allocated = allocated_len;
  }

  friend ശൂന്യം swap (hb_vector_t& a, hb_vector_t& b) noexcept
  {
    hb_swap (a.allocated, b.allocated);
    hb_swap (a.length, b.length);
    hb_swap (a.arrayZ, b.arrayZ);
  }

  hb_vector_t& operator = (സ്ഥിരം hb_vector_t &o)
  {
    എങ്കിൽ (ഇത് == &o) തിരികെ *ഇത്;

    reset ();
    alloc_exact (o.length);
    എങ്കിൽ (unlikely (in_error ())) തിരികെ *ഇത്;

    length = 0;
    copy_array (o.as_array ());

    തിരികെ *ഇത്;
  }
  hb_vector_t& operator = (hb_vector_t &&o) noexcept
  {
    hb_swap (*ഇത്, o);
    തിരികെ *ഇത്;
  }

  hb_bytes_t as_bytes () സ്ഥിരം
  { തിരികെ hb_bytes_t ((സ്ഥിരം അക്ഷരം *) arrayZ, get_size ()); }

  തർക്കം operator == (സ്ഥിരം hb_vector_t &o) സ്ഥിരം { തിരികെ as_array () == o.as_array (); }
  തർക്കം operator != (സ്ഥിരം hb_vector_t &o) സ്ഥിരം { തിരികെ !(*ഇത് == o); }
  uint32_t hash () സ്ഥിരം { തിരികെ as_array ().hash (); }

  Type& operator [] (പൂർണ്ണസംഖ്യ i_)
  {
    unsigned പൂർണ്ണസംഖ്യ i = (unsigned പൂർണ്ണസംഖ്യ) i_;
    എങ്കിൽ (unlikely (i >= length))
      തിരികെ Crap (Type);
    തിരികെ arrayZ[i];
  }
  സ്ഥിരം Type& operator [] (പൂർണ്ണസംഖ്യ i_) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ i = (unsigned പൂർണ്ണസംഖ്യ) i_;
    എങ്കിൽ (unlikely (i >= length))
      തിരികെ Null (Type);
    തിരികെ arrayZ[i];
  }

  Type& tail () { തിരികെ (*ഇത്)[length - 1]; }
  സ്ഥിരം Type& tail () സ്ഥിരം { തിരികെ (*ഇത്)[length - 1]; }

  explicit operator തർക്കം () സ്ഥിരം { തിരികെ length; }
  size_t get_size () സ്ഥിരം { തിരികെ hb_unsigned_mul_saturate (length, item_size); }

  /* Sink interface. */
  template <typename T>
  hb_vector_t& operator << (T&& v) { push (std::forward<T> (v)); തിരികെ *ഇത്; }

  array_t   as_array ()       { തിരികെ hb_array (arrayZ, length); }
  c_array_t as_array () സ്ഥിരം { തിരികെ hb_array (arrayZ, length); }

  /* Iterator. */
  തരനിർവചനം c_array_t   iter_t;
  തരനിർവചനം array_t   writer_t;
    iter_t   iter () സ്ഥിരം { തിരികെ as_array (); }
  writer_t writer ()       { തിരികെ as_array (); }
  operator   iter_t () സ്ഥിരം { തിരികെ   iter (); }
  operator writer_t ()       { തിരികെ writer (); }

  /* Faster range-based for loop. */
  Type *begin () സ്ഥിരം { തിരികെ arrayZ; }
  Type *end () സ്ഥിരം { തിരികെ arrayZ + length; }


  hb_sorted_array_t<Type> as_sorted_array ()
  { തിരികെ hb_sorted_array (arrayZ, length); }
  hb_sorted_array_t<സ്ഥിരം Type> as_sorted_array () സ്ഥിരം
  { തിരികെ hb_sorted_array (arrayZ, length); }

  template <typename T> explicit operator T * () { തിരികെ arrayZ; }
  template <typename T> explicit operator സ്ഥിരം T * () സ്ഥിരം { തിരികെ arrayZ; }

  Type * operator  + (unsigned പൂർണ്ണസംഖ്യ i) { തിരികെ arrayZ + i; }
  സ്ഥിരം Type * operator  + (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം { തിരികെ arrayZ + i; }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push ()
  {
    എങ്കിൽ (unlikely (!resize (length + 1)))
      തിരികെ std::addressof (Crap (Type));
    തിരികെ std::addressof (arrayZ[length - 1]);
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push (Args&&... args)
  {
    എങ്കിൽ (unlikely ((പൂർണ്ണസംഖ്യ) length >= allocated))
    {
      എങ്കിൽ (unlikely (in_error ()))
	തിരികെ std::addressof (Crap (Type));

      /* An argument may live in our own storage, eg. v.push (v[i]).
       * alloc() frees that storage, so copy it out first. */
      എങ്കിൽ (unlikely (in_storage (args...)))
      {
	Type copy (std::forward<Args> (args)...);
	എങ്കിൽ (unlikely (!alloc (length + 1)))
	  തിരികെ std::addressof (Crap (Type));
	തിരികെ തള്ളുക_has_room (std::move (copy));
      }
      എങ്കിൽ (unlikely (!alloc (length + 1)))
	// If push failed to allocate then don't copy v, since this may cause
	// the created copy to leak memory since we won't have stored a
	// reference to it.
	തിരികെ std::addressof (Crap (Type));
    }

    തിരികെ തള്ളുക_has_room (std::forward<Args> (args)...);
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  തർക്കം തള്ളുക_or_fail (Args&&... args)
  {
    തിരികെ push (std::forward<Args> (args)...) != std::addressof (Crap (Type));
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *തള്ളുക_has_room (Args&&... args)
  {
    /* Emplace. */
    Type *p = std::addressof (arrayZ[length++]);
    തിരികെ പുതിയ (p) Type (std::forward<Args> (args)...);
  }

  തർക്കം in_storage () സ്ഥിരം { തിരികെ അസത്യം; }
  template <typename T, typename... Ts>
  തർക്കം in_storage (സ്ഥിരം T &v, സ്ഥിരം Ts &...vs) സ്ഥിരം
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    തിരികെ (arrayZ &&
	    (uintptr_t) arrayZ <= p && p < (uintptr_t) (arrayZ + length)) ||
	   in_storage (vs...);
  }

  തർക്കം is_owned () സ്ഥിരം
  {
    തിരികെ allocated != 0 && allocated != -1;
  }

  തർക്കം in_error () സ്ഥിരം { തിരികെ allocated < 0; }
  ശൂന്യം set_error ()
  {
    പ്രതിജ്ഞാ (allocated >= 0);
    allocated = -allocated - 1;
  }
  ശൂന്യം reset_error ()
  {
    പ്രതിജ്ഞാ (allocated < 0);
    allocated = -(allocated + 1);
  }
  ശൂന്യം ensure_error ()
  {
    എങ്കിൽ (!in_error ())
      set_error ();
  }

  Type *
  _realloc (unsigned new_allocated)
  {
    എങ്കിൽ (!new_allocated)
    {
      എങ്കിൽ (is_owned ())
	hb_free (arrayZ);
      തിരികെ nullptr;
    }
    എങ്കിൽ (!allocated && arrayZ)
    {
      /* If we have a non-null arrayZ but allocated is 0, then we are
       * reallocating from a foreign array. */
      Type *new_array = (Type *) hb_malloc (new_allocated * വലുപ്പം (Type));
      എങ്കിൽ (unlikely (!new_array))
	തിരികെ nullptr;
      hb_memcpy ((ശൂന്യം *) new_array, (സ്ഥിരം ശൂന്യം *) arrayZ, length * വലുപ്പം (Type));
      തിരികെ new_array;
    }
    തിരികെ (Type *) hb_realloc (arrayZ, new_allocated * വലുപ്പം (Type));
  }
  Type *
  _malloc_move (unsigned new_allocated)
  {
    എങ്കിൽ (!new_allocated)
    {
      എങ്കിൽ (is_owned ())
	hb_free (arrayZ);
      തിരികെ nullptr;
    }
    Type *new_array = (Type *) hb_malloc (new_allocated * വലുപ്പം (Type));
    എങ്കിൽ (likely (new_array))
    {
      വേണ്ടി (unsigned i = 0; i < length; i++)
      {
	പുതിയ (std::addressof (new_array[i])) Type ();
	new_array[i] = std::move (arrayZ[i]);
	arrayZ[i].~Type ();
      }
      എങ്കിൽ (is_owned ())
	hb_free (arrayZ);
    }
    തിരികെ new_array;
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<0>)
  {
    തിരികെ _realloc (new_allocated);
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<0>)
  {
    തിരികെ _malloc_move (new_allocated);
  }
  /* Specialization for types that can be moved using realloc(). */
  template <typename T = Type,
	    hb_enable_if (T::realloc_move)>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<1>)
  {
    തിരികെ _realloc (new_allocated);
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T))>
  ശൂന്യം
  grow_vector (unsigned size, hb_priority<0>)
  {
    hb_memset (arrayZ + length, 0, (size - length) * വലുപ്പം (*arrayZ));
    length = size;
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_constructible(T))>
  ശൂന്യം
  grow_vector (unsigned size, hb_priority<0>)
  {
    വേണ്ടി (; length < size; length++)
      പുതിയ (std::addressof (arrayZ[length])) Type ();
  }
  /* Specialization for hb_vector_t<hb_{vector,array}_t<U>> to speed up. */
  template <typename T = Type,
	    hb_enable_if (hb_is_same (T, hb_vector_t<typename T::item_t>) ||
			  hb_is_same (T, hb_array_t <typename T::item_t>))>
  ശൂന്യം
  grow_vector (unsigned size, hb_priority<1>)
  {
    hb_memset (arrayZ + length, 0, (size - length) * വലുപ്പം (*arrayZ));
    length = size;
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  ശൂന്യം
  copy_array (hb_array_t<Type> other)
  {
    hb_memcpy ((ശൂന്യം *) (arrayZ + length), (സ്ഥിരം ശൂന്യം *) other.arrayZ, other.length * item_size);
    length += other.length;
  }
  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  ശൂന്യം
  copy_array (hb_array_t<സ്ഥിരം Type> other)
  {
    hb_memcpy ((ശൂന്യം *) (arrayZ + length), (സ്ഥിരം ശൂന്യം *) other.arrayZ, other.length * item_size);
    length += other.length;
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			   std::is_copy_constructible<T>::value)>
  ശൂന്യം
  copy_array (hb_array_t<സ്ഥിരം Type> other)
  {
    വേണ്ടി (unsigned i = 0; i < other.length; i++)
      പുതിയ (std::addressof (arrayZ[length + i])) Type (other.arrayZ[i]);
    length += other.length;
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			  !std::is_copy_constructible<T>::value &&
			  std::is_default_constructible<T>::value &&
			  std::is_copy_assignable<T>::value)>
  ശൂന്യം
  copy_array (hb_array_t<സ്ഥിരം Type> other)
  {
    വേണ്ടി (unsigned i = 0; i < other.length; i++)
    {
      പുതിയ (std::addressof (arrayZ[length + i])) Type ();
      arrayZ[length + i] = other.arrayZ[i];
    }
    length += other.length;
  }

  ശൂന്യം
  shrink_vector (unsigned size)
  {
    പ്രതിജ്ഞാ (size <= length);
    എങ്കിൽ (!hb_is_trivially_destructible(Type))
    {
      unsigned എണ്ണം = length - size;
      Type *p = arrayZ + length;
      ആകുമ്പോൾ (എണ്ണം--)
        (--p)->~Type ();
    }
    length = size;
  }

  ശൂന്യം
  shift_down_vector (unsigned i)
  {
    വേണ്ടി (; i < length; i++)
      arrayZ[i - 1] = std::move (arrayZ[i]);
  }

  /* Allocate for size but don't adjust length. */
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  തർക്കം alloc (unsigned പൂർണ്ണസംഖ്യ size, തർക്കം exact=അസത്യം)
  {
    എങ്കിൽ (unlikely (in_error ()))
      തിരികെ അസത്യം;

    unsigned പൂർണ്ണസംഖ്യ new_allocated;
    എങ്കിൽ (exact)
    {
      /* If exact was specified, we allow shrinking the storage. */
      size = hb_max (size, length);
      എങ്കിൽ (size <= (unsigned) allocated &&
	  size >= (unsigned) allocated >> 2)
	തിരികെ സത്യം;

      new_allocated = size;
    }
    അല്ലെങ്കിൽ
    {
      എങ്കിൽ (likely (size <= (unsigned) allocated))
	തിരികെ സത്യം;

      new_allocated = allocated;
      ആകുമ്പോൾ (size > new_allocated)
	new_allocated += (new_allocated >> 1) + 8;
    }

    /* Reallocate */

    തർക്കം overflows =
      (പൂർണ്ണസംഖ്യ) in_error () ||
      (new_allocated < size) ||
      hb_unsigned_mul_overflows (new_allocated, വലുപ്പം (Type));

    എങ്കിൽ (unlikely (overflows))
    {
      set_error ();
      തിരികെ അസത്യം;
    }

    Type *new_array = realloc_vector (new_allocated, hb_prioritize);

    എങ്കിൽ (unlikely (new_allocated && !new_array))
    {
      എങ്കിൽ (new_allocated <= (unsigned) allocated)
        തിരികെ സത്യം; // shrinking failed; it's okay; happens in our fuzzer

      set_error ();
      തിരികെ അസത്യം;
    }

    arrayZ = new_array;
    allocated = new_allocated;

    തിരികെ സത്യം;
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  തർക്കം alloc_exact (unsigned പൂർണ്ണസംഖ്യ size)
  {
    തിരികെ alloc (size, സത്യം);
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  ശൂന്യം clear ()
  {
    shrink_vector (0);
  }

  template <typename allocator_t>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  തർക്കം allocate_from_pool (allocator_t *allocator, unsigned size, unsigned പൂർണ്ണസംഖ്യ initialize = സത്യം)
  {
    എങ്കിൽ (allocator)
    {
      പ്രതിജ്ഞാ (!length && !allocated);
      arrayZ = (Type *) allocator->alloc (size * വലുപ്പം (Type), alignof (Type));
      എങ്കിൽ (unlikely (!arrayZ))
      {
	set_error ();
	തിരികെ അസത്യം;
      }
      എങ്കിൽ (initialize)
	grow_vector (size, hb_prioritize);
      അല്ലെങ്കിൽ
	length = size;
      തിരികെ സത്യം;
    }
    തിരികെ resize_full ((പൂർണ്ണസംഖ്യ) size, initialize, സത്യം);
  }

  template <typename allocator_t>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  തർക്കം duplicate_vector_from_pool (allocator_t *allocator, സ്ഥിരം hb_vector_t &other)
  {
    എങ്കിൽ (unlikely (!allocate_from_pool (allocator, other.length, അസത്യം)))
      തിരികെ അസത്യം;
    length = 0;
    copy_array (other.as_array ());
    തിരികെ സത്യം;
  }

  template <typename allocator_t>
  ശൂന്യം shrink_back_to_pool (allocator_t *allocator, പൂർണ്ണസംഖ്യ size)
  {
    unsigned orig_length = length;

    shrink (size, അസത്യം);

    എങ്കിൽ (allocator && !is_owned ())
      allocator->discard (arrayZ + length, (orig_length - length) * വലുപ്പം (Type));
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  തർക്കം resize_full (പൂർണ്ണസംഖ്യ size_, തർക്കം initialize, തർക്കം exact)
  {
    എങ്കിൽ (unlikely (size_ < 0))
      തിരികെ അസത്യം;
    unsigned പൂർണ്ണസംഖ്യ size = (unsigned പൂർണ്ണസംഖ്യ) size_;
    എങ്കിൽ (!alloc (size, exact))
      തിരികെ അസത്യം;

    എങ്കിൽ (size > length)
    {
      എങ്കിൽ (initialize)
	grow_vector (size, hb_prioritize);
    }
    അല്ലെങ്കിൽ എങ്കിൽ (size < length)
    {
      എങ്കിൽ (initialize)
	shrink_vector (size);
    }

    length = size;
    തിരികെ സത്യം;
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  തർക്കം resize (പൂർണ്ണസംഖ്യ size_)
  {
    തിരികെ resize_full (size_, സത്യം, അസത്യം);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  തർക്കം resize_dirty (പൂർണ്ണസംഖ്യ size_)
  {
    തിരികെ resize_full (size_, അസത്യം, അസത്യം);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  തർക്കം resize_exact (പൂർണ്ണസംഖ്യ size_)
  {
    തിരികെ resize_full (size_, സത്യം, സത്യം);
  }

  Type എടുക്കുക ()
  {
    എങ്കിൽ (!length) തിരികെ Null (Type);
    Type v (std::move (arrayZ[length - 1]));
    arrayZ[length - 1].~Type ();
    length--;
    തിരികെ v;
  }

  ശൂന്യം remove_ordered (unsigned പൂർണ്ണസംഖ്യ i)
  {
    എങ്കിൽ (unlikely (i >= length))
      തിരികെ;
    shift_down_vector (i + 1);
    arrayZ[length - 1].~Type ();
    length--;
  }

  template <തർക്കം Sorted = sorted,
	    hb_enable_if (!Sorted)>
  ശൂന്യം remove_unordered (unsigned പൂർണ്ണസംഖ്യ i)
  {
    എങ്കിൽ (unlikely (i >= length))
      തിരികെ;
    എങ്കിൽ (i != length - 1)
      arrayZ[i] = std::move (arrayZ[length - 1]);
    arrayZ[length - 1].~Type ();
    length--;
  }

  ശൂന്യം shrink (പൂർണ്ണസംഖ്യ size_, തർക്കം shrink_memory = സത്യം)
  {
    unsigned പൂർണ്ണസംഖ്യ size = size_ < 0 ? 0u : (unsigned പൂർണ്ണസംഖ്യ) size_;
    എങ്കിൽ (size >= length)
      തിരികെ;

    shrink_vector (size);

    എങ്കിൽ (is_owned () && shrink_memory)
      alloc_exact (size); /* To force shrinking memory if needed. */
  }


  /* Sorting API. */
  template <typename Compar>
  ശൂന്യം qsort (Compar compar)
  { as_array ().qsort (compar); }
  ശൂന്യം qsort ()
  { as_array ().qsort (); }

  /* Unsorted search API. */
  template <typename T>
  Type *lsearch (സ്ഥിരം T &x, Type *not_found = nullptr)
  { തിരികെ as_array ().lsearch (x, not_found); }
  template <typename T>
  സ്ഥിരം Type *lsearch (സ്ഥിരം T &x, സ്ഥിരം Type *not_found = nullptr) സ്ഥിരം
  { തിരികെ as_array ().lsearch (x, not_found); }
  template <typename T>
  തർക്കം lfind (സ്ഥിരം T &x, unsigned *pos = nullptr) സ്ഥിരം
  { തിരികെ as_array ().lfind (x, pos); }

  /* Sorted search API. */
  template <typename T,
	    തർക്കം Sorted=sorted, hb_enable_if (Sorted)>
  Type *bsearch (സ്ഥിരം T &x, Type *not_found = nullptr)
  { തിരികെ as_array ().bsearch (x, not_found); }
  template <typename T,
	    തർക്കം Sorted=sorted, hb_enable_if (Sorted)>
  സ്ഥിരം Type *bsearch (സ്ഥിരം T &x, സ്ഥിരം Type *not_found = nullptr) സ്ഥിരം
  { തിരികെ as_array ().bsearch (x, not_found); }
  template <typename T,
	    തർക്കം Sorted=sorted, hb_enable_if (Sorted)>
  തർക്കം bfind (സ്ഥിരം T &x, unsigned പൂർണ്ണസംഖ്യ *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned പൂർണ്ണസംഖ്യ to_store = (unsigned പൂർണ്ണസംഖ്യ) -1) സ്ഥിരം
  { തിരികെ as_array ().bfind (x, i, not_found, to_store); }
};

template <typename Type>
using hb_sorted_vector_t = hb_vector_t<Type, സത്യം>;

#endif /* HB_VECTOR_HH */

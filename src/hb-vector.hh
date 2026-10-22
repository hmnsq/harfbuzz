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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-array.hh"
#अन्तर्भाव "hb-meta.hh"
#अन्तर्भाव "hb-null.hh"

// Change to 1 to force inline vector allocs, to see callsite in malloc-stats tool.
#जर 0
#define HB_ALWAYS_INLINE_VECTOR_ALLOCS HB_ALWAYS_INLINE
#अन्यथा
#define HB_ALWAYS_INLINE_VECTOR_ALLOCS
#endif

ढाँचा <typename Type,
	  तार्किक sorted=असत्य>
संरचना hb_vector_t
{
  स्थिर constexpr तार्किक realloc_move = सत्य;

  प्रकारव्याख्या Type item_t;
  स्थिर constexpr अचिन्हित item_size = hb_static_size (Type);
  using array_t = typename std::conditional<sorted, hb_sorted_array_t<Type>, hb_array_t<Type>>::type;
  using c_array_t = typename std::conditional<sorted, hb_sorted_array_t<स्थिरांक Type>, hb_array_t<स्थिरांक Type>>::type;

  hb_vector_t () = पूर्वनिर्धारित;
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (std::initializer_list<Type> lst) : hb_vector_t ()
  {
    alloc (lst.size (), सत्य);
    साठी (स्वतः&& item : lst)
      push (item);
  }
  ढाँचा <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  explicit hb_vector_t (स्थिरांक Iterable &o) : hb_vector_t ()
  {
    extend (o, सत्य);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (स्थिरांक hb_vector_t &o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    जर (unlikely (in_error ())) परत;
    copy_array (o.as_array ());
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (array_t o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    जर (unlikely (in_error ())) परत;
    copy_array (o);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (c_array_t o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    जर (unlikely (in_error ())) परत;
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

  ढाँचा <अचिन्हित n>
  शून्य
  set_storage (Type (&array)[n])
  { set_storage (array, n); }
  शून्य
  set_storage (hb_array_t<Type> array)
  { set_storage (array.arrayZ, array.length); }
  ढाँचा <typename T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T) &&
			  hb_is_trivially_destructible(T))>
  शून्य
  set_storage (Type *array, अचिन्हित n)
  {
    प्रतिज्ञा (allocated == 0);
    प्रतिज्ञा (length == 0);

    arrayZ = array;
    length = n;
  }

  ढाँचा <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  शून्य extend (स्थिरांक Iterable &o, तार्किक exact=असत्य)
  {
    स्वतः iter = hb_iter (o);
    जर (iter.is_random_access_iterator || iter.has_fast_len)
    {
      जर (unlikely (!alloc (length + hb_len (iter), exact)))
	परत;
      अचिन्हित मोजणी = hb_len (iter);
      साठी (अचिन्हित i = 0; i < मोजणी; i++)
	ढकला_has_room (*iter++);
    }
    पर्यंत (iter)
    {
      जर (unlikely (!alloc (length + 1)))
        परत;
      अचिन्हित room = allocated - length;
      साठी (अचिन्हित i = 0; i < room && iter; i++)
	ढकला_has_room (*iter++);
    }
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  शून्य extend (array_t o, तार्किक exact=असत्य)
  { extend (c_array_t (o), exact); }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  शून्य extend (c_array_t o, तार्किक exact=असत्य)
  {
    /* o may point into our own storage, eg. v.extend (v.as_array ()).
     * alloc() may move that storage, so find o again afterwards. */
    तार्किक aliased = o.length && in_storage (*o.arrayZ);
    अचिन्हित offset = aliased ? o.arrayZ - arrayZ : 0;
    alloc (length + o.length, exact);
    जर (unlikely (in_error ())) परत;
    जर (unlikely (aliased))
      o = {arrayZ + offset, o.length};
    copy_array (o);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  शून्य extend (स्थिरांक hb_vector_t &o, तार्किक exact=असत्य)
  { extend (o.as_array (), exact); }

  सार्वजनिक:
  पूर्णांक allocated = 0; /* < 0 means allocation failed. */
  अचिन्हित पूर्णांक length = 0;
  सार्वजनिक:
  Type *arrayZ = nullptr;

  शून्य init ()
  {
    allocated = length = 0;
    arrayZ = nullptr;
  }
  शून्य init0 ()
  {
  }

  शून्य fini ()
  {
    जर (is_owned ())
    {
      shrink_vector (0);
      hb_free (arrayZ);
    }
    init ();
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t &reset ()
  {
    जर (unlikely (in_error ()))
      reset_error ();
    clear ();
    परत *हे;
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t &reset_if_error ()
  {
    जर (unlikely (in_error ()))
      reset ();
    परत *हे;
  }

  /* Transfer ownership of the backing storage to caller.
   * Returns nullptr if storage is not owned by this vector. */
  Type *
  steal (अचिन्हित *len = nullptr, पूर्णांक *allocated_out = nullptr)
  {
    जर (!is_owned ())
      परत nullptr;
    जर (len)
      *len = length;
    जर (allocated_out)
      *allocated_out = allocated;
    Type *p = arrayZ;
    init ();
    परत p;
  }

  /* Adopt a previously detached owned buffer. */
  शून्य
  recycle_buffer (Type *buffer,
                  अचिन्हित len,
                  पूर्णांक allocated_len)
  {
    fini ();
    arrayZ = buffer;
    length = len;
    allocated = allocated_len;
  }

  मित्र शून्य swap (hb_vector_t& a, hb_vector_t& b) noexcept
  {
    hb_swap (a.allocated, b.allocated);
    hb_swap (a.length, b.length);
    hb_swap (a.arrayZ, b.arrayZ);
  }

  hb_vector_t& चालक = (स्थिरांक hb_vector_t &o)
  {
    जर (हे == &o) परत *हे;

    reset ();
    alloc_exact (o.length);
    जर (unlikely (in_error ())) परत *हे;

    length = 0;
    copy_array (o.as_array ());

    परत *हे;
  }
  hb_vector_t& चालक = (hb_vector_t &&o) noexcept
  {
    hb_swap (*हे, o);
    परत *हे;
  }

  hb_bytes_t as_bytes () स्थिरांक
  { परत hb_bytes_t ((स्थिरांक अक्षर *) arrayZ, get_size ()); }

  तार्किक चालक == (स्थिरांक hb_vector_t &o) स्थिरांक { परत as_array () == o.as_array (); }
  तार्किक चालक != (स्थिरांक hb_vector_t &o) स्थिरांक { परत !(*हे == o); }
  uint32_t hash () स्थिरांक { परत as_array ().hash (); }

  Type& चालक [] (पूर्णांक i_)
  {
    अचिन्हित पूर्णांक i = (अचिन्हित पूर्णांक) i_;
    जर (unlikely (i >= length))
      परत Crap (Type);
    परत arrayZ[i];
  }
  स्थिरांक Type& चालक [] (पूर्णांक i_) स्थिरांक
  {
    अचिन्हित पूर्णांक i = (अचिन्हित पूर्णांक) i_;
    जर (unlikely (i >= length))
      परत Null (Type);
    परत arrayZ[i];
  }

  Type& tail () { परत (*हे)[length - 1]; }
  स्थिरांक Type& tail () स्थिरांक { परत (*हे)[length - 1]; }

  explicit चालक तार्किक () स्थिरांक { परत length; }
  माप_प्रकार get_size () स्थिरांक { परत hb_unsigned_mul_saturate (length, item_size); }

  /* Sink interface. */
  ढाँचा <typename T>
  hb_vector_t& चालक << (T&& v) { push (std::forward<T> (v)); परत *हे; }

  array_t   as_array ()       { परत hb_array (arrayZ, length); }
  c_array_t as_array () स्थिरांक { परत hb_array (arrayZ, length); }

  /* Iterator. */
  प्रकारव्याख्या c_array_t   iter_t;
  प्रकारव्याख्या array_t   writer_t;
    iter_t   iter () स्थिरांक { परत as_array (); }
  writer_t writer ()       { परत as_array (); }
  चालक   iter_t () स्थिरांक { परत   iter (); }
  चालक writer_t ()       { परत writer (); }

  /* Faster range-based for loop. */
  Type *begin () स्थिरांक { परत arrayZ; }
  Type *end () स्थिरांक { परत arrayZ + length; }


  hb_sorted_array_t<Type> as_sorted_array ()
  { परत hb_sorted_array (arrayZ, length); }
  hb_sorted_array_t<स्थिरांक Type> as_sorted_array () स्थिरांक
  { परत hb_sorted_array (arrayZ, length); }

  ढाँचा <typename T> explicit चालक T * () { परत arrayZ; }
  ढाँचा <typename T> explicit चालक स्थिरांक T * () स्थिरांक { परत arrayZ; }

  Type * चालक  + (अचिन्हित पूर्णांक i) { परत arrayZ + i; }
  स्थिरांक Type * चालक  + (अचिन्हित पूर्णांक i) स्थिरांक { परत arrayZ + i; }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push ()
  {
    जर (unlikely (!resize (length + 1)))
      परत std::addressof (Crap (Type));
    परत std::addressof (arrayZ[length - 1]);
  }
  ढाँचा <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push (Args&&... args)
  {
    जर (unlikely ((पूर्णांक) length >= allocated))
    {
      जर (unlikely (in_error ()))
	परत std::addressof (Crap (Type));

      /* An argument may live in our own storage, eg. v.push (v[i]).
       * alloc() frees that storage, so copy it out first. */
      जर (unlikely (in_storage (args...)))
      {
	Type copy (std::forward<Args> (args)...);
	जर (unlikely (!alloc (length + 1)))
	  परत std::addressof (Crap (Type));
	परत ढकला_has_room (std::move (copy));
      }
      जर (unlikely (!alloc (length + 1)))
	// If push failed to allocate then don't copy v, since this may cause
	// the created copy to leak memory since we won't have stored a
	// reference to it.
	परत std::addressof (Crap (Type));
    }

    परत ढकला_has_room (std::forward<Args> (args)...);
  }
  ढाँचा <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  तार्किक ढकला_or_fail (Args&&... args)
  {
    परत push (std::forward<Args> (args)...) != std::addressof (Crap (Type));
  }
  ढाँचा <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *ढकला_has_room (Args&&... args)
  {
    /* Emplace. */
    Type *p = std::addressof (arrayZ[length++]);
    परत नवीन (p) Type (std::forward<Args> (args)...);
  }

  तार्किक in_storage () स्थिरांक { परत असत्य; }
  ढाँचा <typename T, typename... Ts>
  तार्किक in_storage (स्थिरांक T &v, स्थिरांक Ts &...vs) स्थिरांक
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    परत (arrayZ &&
	    (uintptr_t) arrayZ <= p && p < (uintptr_t) (arrayZ + length)) ||
	   in_storage (vs...);
  }

  तार्किक is_owned () स्थिरांक
  {
    परत allocated != 0 && allocated != -1;
  }

  तार्किक in_error () स्थिरांक { परत allocated < 0; }
  शून्य set_error ()
  {
    प्रतिज्ञा (allocated >= 0);
    allocated = -allocated - 1;
  }
  शून्य reset_error ()
  {
    प्रतिज्ञा (allocated < 0);
    allocated = -(allocated + 1);
  }
  शून्य ensure_error ()
  {
    जर (!in_error ())
      set_error ();
  }

  Type *
  _realloc (अचिन्हित new_allocated)
  {
    जर (!new_allocated)
    {
      जर (is_owned ())
	hb_free (arrayZ);
      परत nullptr;
    }
    जर (!allocated && arrayZ)
    {
      /* If we have a non-null arrayZ but allocated is 0, then we are
       * reallocating from a foreign array. */
      Type *new_array = (Type *) hb_malloc (new_allocated * आकार (Type));
      जर (unlikely (!new_array))
	परत nullptr;
      hb_memcpy ((शून्य *) new_array, (स्थिरांक शून्य *) arrayZ, length * आकार (Type));
      परत new_array;
    }
    परत (Type *) hb_realloc (arrayZ, new_allocated * आकार (Type));
  }
  Type *
  _malloc_move (अचिन्हित new_allocated)
  {
    जर (!new_allocated)
    {
      जर (is_owned ())
	hb_free (arrayZ);
      परत nullptr;
    }
    Type *new_array = (Type *) hb_malloc (new_allocated * आकार (Type));
    जर (likely (new_array))
    {
      साठी (अचिन्हित i = 0; i < length; i++)
      {
	नवीन (std::addressof (new_array[i])) Type ();
	new_array[i] = std::move (arrayZ[i]);
	arrayZ[i].~Type ();
      }
      जर (is_owned ())
	hb_free (arrayZ);
    }
    परत new_array;
  }

  ढाँचा <typename T = Type,
	    hb_enable_if (hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_vector (अचिन्हित new_allocated, hb_priority<0>)
  {
    परत _realloc (new_allocated);
  }
  ढाँचा <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_vector (अचिन्हित new_allocated, hb_priority<0>)
  {
    परत _malloc_move (new_allocated);
  }
  /* Specialization for types that can be moved using realloc(). */
  ढाँचा <typename T = Type,
	    hb_enable_if (T::realloc_move)>
  Type *
  realloc_vector (अचिन्हित new_allocated, hb_priority<1>)
  {
    परत _realloc (new_allocated);
  }

  ढाँचा <typename T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T))>
  शून्य
  grow_vector (अचिन्हित size, hb_priority<0>)
  {
    hb_memset (arrayZ + length, 0, (size - length) * आकार (*arrayZ));
    length = size;
  }
  ढाँचा <typename T = Type,
	    hb_enable_if (!hb_is_trivially_constructible(T))>
  शून्य
  grow_vector (अचिन्हित size, hb_priority<0>)
  {
    साठी (; length < size; length++)
      नवीन (std::addressof (arrayZ[length])) Type ();
  }
  /* Specialization for hb_vector_t<hb_{vector,array}_t<U>> to speed up. */
  ढाँचा <typename T = Type,
	    hb_enable_if (hb_is_same (T, hb_vector_t<typename T::item_t>) ||
			  hb_is_same (T, hb_array_t <typename T::item_t>))>
  शून्य
  grow_vector (अचिन्हित size, hb_priority<1>)
  {
    hb_memset (arrayZ + length, 0, (size - length) * आकार (*arrayZ));
    length = size;
  }

  ढाँचा <typename T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  शून्य
  copy_array (hb_array_t<Type> other)
  {
    hb_memcpy ((शून्य *) (arrayZ + length), (स्थिरांक शून्य *) other.arrayZ, other.length * item_size);
    length += other.length;
  }
  ढाँचा <typename T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  शून्य
  copy_array (hb_array_t<स्थिरांक Type> other)
  {
    hb_memcpy ((शून्य *) (arrayZ + length), (स्थिरांक शून्य *) other.arrayZ, other.length * item_size);
    length += other.length;
  }
  ढाँचा <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			   std::is_copy_constructible<T>::value)>
  शून्य
  copy_array (hb_array_t<स्थिरांक Type> other)
  {
    साठी (अचिन्हित i = 0; i < other.length; i++)
      नवीन (std::addressof (arrayZ[length + i])) Type (other.arrayZ[i]);
    length += other.length;
  }
  ढाँचा <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			  !std::is_copy_constructible<T>::value &&
			  std::is_default_constructible<T>::value &&
			  std::is_copy_assignable<T>::value)>
  शून्य
  copy_array (hb_array_t<स्थिरांक Type> other)
  {
    साठी (अचिन्हित i = 0; i < other.length; i++)
    {
      नवीन (std::addressof (arrayZ[length + i])) Type ();
      arrayZ[length + i] = other.arrayZ[i];
    }
    length += other.length;
  }

  शून्य
  shrink_vector (अचिन्हित size)
  {
    प्रतिज्ञा (size <= length);
    जर (!hb_is_trivially_destructible(Type))
    {
      अचिन्हित मोजणी = length - size;
      Type *p = arrayZ + length;
      पर्यंत (मोजणी--)
        (--p)->~Type ();
    }
    length = size;
  }

  शून्य
  shift_down_vector (अचिन्हित i)
  {
    साठी (; i < length; i++)
      arrayZ[i - 1] = std::move (arrayZ[i]);
  }

  /* Allocate for size but don't adjust length. */
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  तार्किक alloc (अचिन्हित पूर्णांक size, तार्किक exact=असत्य)
  {
    जर (unlikely (in_error ()))
      परत असत्य;

    अचिन्हित पूर्णांक new_allocated;
    जर (exact)
    {
      /* If exact was specified, we allow shrinking the storage. */
      size = hb_max (size, length);
      जर (size <= (अचिन्हित) allocated &&
	  size >= (अचिन्हित) allocated >> 2)
	परत सत्य;

      new_allocated = size;
    }
    अन्यथा
    {
      जर (likely (size <= (अचिन्हित) allocated))
	परत सत्य;

      new_allocated = allocated;
      पर्यंत (size > new_allocated)
	new_allocated += (new_allocated >> 1) + 8;
    }

    /* Reallocate */

    तार्किक overflows =
      (पूर्णांक) in_error () ||
      (new_allocated < size) ||
      hb_unsigned_mul_overflows (new_allocated, आकार (Type));

    जर (unlikely (overflows))
    {
      set_error ();
      परत असत्य;
    }

    Type *new_array = realloc_vector (new_allocated, hb_prioritize);

    जर (unlikely (new_allocated && !new_array))
    {
      जर (new_allocated <= (अचिन्हित) allocated)
        परत सत्य; // shrinking failed; it's okay; happens in our fuzzer

      set_error ();
      परत असत्य;
    }

    arrayZ = new_array;
    allocated = new_allocated;

    परत सत्य;
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  तार्किक alloc_exact (अचिन्हित पूर्णांक size)
  {
    परत alloc (size, सत्य);
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  शून्य clear ()
  {
    shrink_vector (0);
  }

  ढाँचा <typename allocator_t>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  तार्किक allocate_from_pool (allocator_t *allocator, अचिन्हित size, अचिन्हित पूर्णांक initialize = सत्य)
  {
    जर (allocator)
    {
      प्रतिज्ञा (!length && !allocated);
      arrayZ = (Type *) allocator->alloc (size * आकार (Type), alignof (Type));
      जर (unlikely (!arrayZ))
      {
	set_error ();
	परत असत्य;
      }
      जर (initialize)
	grow_vector (size, hb_prioritize);
      अन्यथा
	length = size;
      परत सत्य;
    }
    परत resize_full ((पूर्णांक) size, initialize, सत्य);
  }

  ढाँचा <typename allocator_t>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  तार्किक duplicate_vector_from_pool (allocator_t *allocator, स्थिरांक hb_vector_t &other)
  {
    जर (unlikely (!allocate_from_pool (allocator, other.length, असत्य)))
      परत असत्य;
    length = 0;
    copy_array (other.as_array ());
    परत सत्य;
  }

  ढाँचा <typename allocator_t>
  शून्य shrink_back_to_pool (allocator_t *allocator, पूर्णांक size)
  {
    अचिन्हित orig_length = length;

    shrink (size, असत्य);

    जर (allocator && !is_owned ())
      allocator->discard (arrayZ + length, (orig_length - length) * आकार (Type));
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  तार्किक resize_full (पूर्णांक size_, तार्किक initialize, तार्किक exact)
  {
    जर (unlikely (size_ < 0))
      परत असत्य;
    अचिन्हित पूर्णांक size = (अचिन्हित पूर्णांक) size_;
    जर (!alloc (size, exact))
      परत असत्य;

    जर (size > length)
    {
      जर (initialize)
	grow_vector (size, hb_prioritize);
    }
    अन्यथा जर (size < length)
    {
      जर (initialize)
	shrink_vector (size);
    }

    length = size;
    परत सत्य;
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  तार्किक resize (पूर्णांक size_)
  {
    परत resize_full (size_, सत्य, असत्य);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  तार्किक resize_dirty (पूर्णांक size_)
  {
    परत resize_full (size_, असत्य, असत्य);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  तार्किक resize_exact (पूर्णांक size_)
  {
    परत resize_full (size_, सत्य, सत्य);
  }

  Type काढा ()
  {
    जर (!length) परत Null (Type);
    Type v (std::move (arrayZ[length - 1]));
    arrayZ[length - 1].~Type ();
    length--;
    परत v;
  }

  शून्य remove_ordered (अचिन्हित पूर्णांक i)
  {
    जर (unlikely (i >= length))
      परत;
    shift_down_vector (i + 1);
    arrayZ[length - 1].~Type ();
    length--;
  }

  ढाँचा <तार्किक Sorted = sorted,
	    hb_enable_if (!Sorted)>
  शून्य remove_unordered (अचिन्हित पूर्णांक i)
  {
    जर (unlikely (i >= length))
      परत;
    जर (i != length - 1)
      arrayZ[i] = std::move (arrayZ[length - 1]);
    arrayZ[length - 1].~Type ();
    length--;
  }

  शून्य shrink (पूर्णांक size_, तार्किक shrink_memory = सत्य)
  {
    अचिन्हित पूर्णांक size = size_ < 0 ? 0u : (अचिन्हित पूर्णांक) size_;
    जर (size >= length)
      परत;

    shrink_vector (size);

    जर (is_owned () && shrink_memory)
      alloc_exact (size); /* To force shrinking memory if needed. */
  }


  /* Sorting API. */
  ढाँचा <typename Compar>
  शून्य क्विक (Compar compar)
  { as_array ().क्विक (compar); }
  शून्य क्विक ()
  { as_array ().क्विक (); }

  /* Unsorted search API. */
  ढाँचा <typename T>
  Type *lsearch (स्थिरांक T &x, Type *not_found = nullptr)
  { परत as_array ().lsearch (x, not_found); }
  ढाँचा <typename T>
  स्थिरांक Type *lsearch (स्थिरांक T &x, स्थिरांक Type *not_found = nullptr) स्थिरांक
  { परत as_array ().lsearch (x, not_found); }
  ढाँचा <typename T>
  तार्किक lfind (स्थिरांक T &x, अचिन्हित *pos = nullptr) स्थिरांक
  { परत as_array ().lfind (x, pos); }

  /* Sorted search API. */
  ढाँचा <typename T,
	    तार्किक Sorted=sorted, hb_enable_if (Sorted)>
  Type *द्वा_खोज (स्थिरांक T &x, Type *not_found = nullptr)
  { परत as_array ().द्वा_खोज (x, not_found); }
  ढाँचा <typename T,
	    तार्किक Sorted=sorted, hb_enable_if (Sorted)>
  स्थिरांक Type *द्वा_खोज (स्थिरांक T &x, स्थिरांक Type *not_found = nullptr) स्थिरांक
  { परत as_array ().द्वा_खोज (x, not_found); }
  ढाँचा <typename T,
	    तार्किक Sorted=sorted, hb_enable_if (Sorted)>
  तार्किक bfind (स्थिरांक T &x, अचिन्हित पूर्णांक *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      अचिन्हित पूर्णांक to_store = (अचिन्हित पूर्णांक) -1) स्थिरांक
  { परत as_array ().bfind (x, i, not_found, to_store); }
};

ढाँचा <typename Type>
using hb_sorted_vector_t = hb_vector_t<Type, सत्य>;

#endif /* HB_VECTOR_HH */

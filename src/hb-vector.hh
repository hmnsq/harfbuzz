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
#यदि 0
#define HB_ALWAYS_INLINE_VECTOR_ALLOCS HB_ALWAYS_INLINE
#अन्यथा
#define HB_ALWAYS_INLINE_VECTOR_ALLOCS
#endif

ढाँचा <typename Type,
	  तार्किक sorted=असत्य>
संरचना hb_vector_t
{
  स्थिर constexpr तार्किक realloc_move = सत्य;

  प्रकारनिर्देश Type item_t;
  स्थिर constexpr अचिन्हित item_size = hb_static_size (Type);
  using array_t = typename std::conditional<sorted, hb_sorted_array_t<Type>, hb_array_t<Type>>::type;
  using c_array_t = typename std::conditional<sorted, hb_sorted_array_t<अचर Type>, hb_array_t<अचर Type>>::type;

  hb_vector_t () = स्वभाव;
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (std::initializer_list<Type> lst) : hb_vector_t ()
  {
    alloc (lst.size (), सत्य);
    लागि (स्वतः&& item : lst)
      push (item);
  }
  ढाँचा <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  explicit hb_vector_t (अचर Iterable &o) : hb_vector_t ()
  {
    extend (o, सत्य);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (अचर hb_vector_t &o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    यदि (unlikely (in_error ())) फर्काऊ;
    copy_array (o.as_array ());
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (array_t o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    यदि (unlikely (in_error ())) फर्काऊ;
    copy_array (o);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (c_array_t o) : hb_vector_t ()
  {
    alloc_exact (o.length);
    यदि (unlikely (in_error ())) फर्काऊ;
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
  शून्य extend (अचर Iterable &o, तार्किक exact=असत्य)
  {
    स्वतः iter = hb_iter (o);
    यदि (iter.is_random_access_iterator || iter.has_fast_len)
    {
      यदि (unlikely (!alloc (length + hb_len (iter), exact)))
	फर्काऊ;
      अचिन्हित गन्ती = hb_len (iter);
      लागि (अचिन्हित i = 0; i < गन्ती; i++)
	थप_has_room (*iter++);
    }
    जबसम्म (iter)
    {
      यदि (unlikely (!alloc (length + 1)))
        फर्काऊ;
      अचिन्हित room = allocated - length;
      लागि (अचिन्हित i = 0; i < room && iter; i++)
	थप_has_room (*iter++);
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
    यदि (unlikely (in_error ())) फर्काऊ;
    यदि (unlikely (aliased))
      o = {arrayZ + offset, o.length};
    copy_array (o);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  शून्य extend (अचर hb_vector_t &o, तार्किक exact=असत्य)
  { extend (o.as_array (), exact); }

  सार्वजनिक:
  पूर्णाङ्क allocated = 0; /* < 0 means allocation failed. */
  अचिन्हित पूर्णाङ्क length = 0;
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
    यदि (is_owned ())
    {
      shrink_vector (0);
      hb_free (arrayZ);
    }
    init ();
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t &reset ()
  {
    यदि (unlikely (in_error ()))
      reset_error ();
    clear ();
    फर्काऊ *एतत्;
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t &reset_if_error ()
  {
    यदि (unlikely (in_error ()))
      reset ();
    फर्काऊ *एतत्;
  }

  /* Transfer ownership of the backing storage to caller.
   * Returns nullptr if storage is not owned by this vector. */
  Type *
  steal (अचिन्हित *len = nullptr, पूर्णाङ्क *allocated_out = nullptr)
  {
    यदि (!is_owned ())
      फर्काऊ nullptr;
    यदि (len)
      *len = length;
    यदि (allocated_out)
      *allocated_out = allocated;
    Type *p = arrayZ;
    init ();
    फर्काऊ p;
  }

  /* Adopt a previously detached owned buffer. */
  शून्य
  recycle_buffer (Type *buffer,
                  अचिन्हित len,
                  पूर्णाङ्क allocated_len)
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

  hb_vector_t& चालक = (अचर hb_vector_t &o)
  {
    यदि (एतत् == &o) फर्काऊ *एतत्;

    reset ();
    alloc_exact (o.length);
    यदि (unlikely (in_error ())) फर्काऊ *एतत्;

    length = 0;
    copy_array (o.as_array ());

    फर्काऊ *एतत्;
  }
  hb_vector_t& चालक = (hb_vector_t &&o) noexcept
  {
    hb_swap (*एतत्, o);
    फर्काऊ *एतत्;
  }

  hb_bytes_t as_bytes () अचर
  { फर्काऊ hb_bytes_t ((अचर वर्ण *) arrayZ, get_size ()); }

  तार्किक चालक == (अचर hb_vector_t &o) अचर { फर्काऊ as_array () == o.as_array (); }
  तार्किक चालक != (अचर hb_vector_t &o) अचर { फर्काऊ !(*एतत् == o); }
  uint32_t hash () अचर { फर्काऊ as_array ().hash (); }

  Type& चालक [] (पूर्णाङ्क i_)
  {
    अचिन्हित पूर्णाङ्क i = (अचिन्हित पूर्णाङ्क) i_;
    यदि (unlikely (i >= length))
      फर्काऊ Crap (Type);
    फर्काऊ arrayZ[i];
  }
  अचर Type& चालक [] (पूर्णाङ्क i_) अचर
  {
    अचिन्हित पूर्णाङ्क i = (अचिन्हित पूर्णाङ्क) i_;
    यदि (unlikely (i >= length))
      फर्काऊ Null (Type);
    फर्काऊ arrayZ[i];
  }

  Type& tail () { फर्काऊ (*एतत्)[length - 1]; }
  अचर Type& tail () अचर { फर्काऊ (*एतत्)[length - 1]; }

  explicit चालक तार्किक () अचर { फर्काऊ length; }
  माप_प्रकार get_size () अचर { फर्काऊ hb_unsigned_mul_saturate (length, item_size); }

  /* Sink interface. */
  ढाँचा <typename T>
  hb_vector_t& चालक << (T&& v) { push (std::forward<T> (v)); फर्काऊ *एतत्; }

  array_t   as_array ()       { फर्काऊ hb_array (arrayZ, length); }
  c_array_t as_array () अचर { फर्काऊ hb_array (arrayZ, length); }

  /* Iterator. */
  प्रकारनिर्देश c_array_t   iter_t;
  प्रकारनिर्देश array_t   writer_t;
    iter_t   iter () अचर { फर्काऊ as_array (); }
  writer_t writer ()       { फर्काऊ as_array (); }
  चालक   iter_t () अचर { फर्काऊ   iter (); }
  चालक writer_t ()       { फर्काऊ writer (); }

  /* Faster range-based for loop. */
  Type *begin () अचर { फर्काऊ arrayZ; }
  Type *end () अचर { फर्काऊ arrayZ + length; }


  hb_sorted_array_t<Type> as_sorted_array ()
  { फर्काऊ hb_sorted_array (arrayZ, length); }
  hb_sorted_array_t<अचर Type> as_sorted_array () अचर
  { फर्काऊ hb_sorted_array (arrayZ, length); }

  ढाँचा <typename T> explicit चालक T * () { फर्काऊ arrayZ; }
  ढाँचा <typename T> explicit चालक अचर T * () अचर { फर्काऊ arrayZ; }

  Type * चालक  + (अचिन्हित पूर्णाङ्क i) { फर्काऊ arrayZ + i; }
  अचर Type * चालक  + (अचिन्हित पूर्णाङ्क i) अचर { फर्काऊ arrayZ + i; }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push ()
  {
    यदि (unlikely (!resize (length + 1)))
      फर्काऊ std::addressof (Crap (Type));
    फर्काऊ std::addressof (arrayZ[length - 1]);
  }
  ढाँचा <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push (Args&&... args)
  {
    यदि (unlikely ((पूर्णाङ्क) length >= allocated))
    {
      यदि (unlikely (in_error ()))
	फर्काऊ std::addressof (Crap (Type));

      /* An argument may live in our own storage, eg. v.push (v[i]).
       * alloc() frees that storage, so copy it out first. */
      यदि (unlikely (in_storage (args...)))
      {
	Type copy (std::forward<Args> (args)...);
	यदि (unlikely (!alloc (length + 1)))
	  फर्काऊ std::addressof (Crap (Type));
	फर्काऊ थप_has_room (std::move (copy));
      }
      यदि (unlikely (!alloc (length + 1)))
	// If push failed to allocate then don't copy v, since this may cause
	// the created copy to leak memory since we won't have stored a
	// reference to it.
	फर्काऊ std::addressof (Crap (Type));
    }

    फर्काऊ थप_has_room (std::forward<Args> (args)...);
  }
  ढाँचा <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  तार्किक थप_or_fail (Args&&... args)
  {
    फर्काऊ push (std::forward<Args> (args)...) != std::addressof (Crap (Type));
  }
  ढाँचा <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *थप_has_room (Args&&... args)
  {
    /* Emplace. */
    Type *p = std::addressof (arrayZ[length++]);
    फर्काऊ नूतन (p) Type (std::forward<Args> (args)...);
  }

  तार्किक in_storage () अचर { फर्काऊ असत्य; }
  ढाँचा <typename T, typename... Ts>
  तार्किक in_storage (अचर T &v, अचर Ts &...vs) अचर
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    फर्काऊ (arrayZ &&
	    (uintptr_t) arrayZ <= p && p < (uintptr_t) (arrayZ + length)) ||
	   in_storage (vs...);
  }

  तार्किक is_owned () अचर
  {
    फर्काऊ allocated != 0 && allocated != -1;
  }

  तार्किक in_error () अचर { फर्काऊ allocated < 0; }
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
    यदि (!in_error ())
      set_error ();
  }

  Type *
  _realloc (अचिन्हित new_allocated)
  {
    यदि (!new_allocated)
    {
      यदि (is_owned ())
	hb_free (arrayZ);
      फर्काऊ nullptr;
    }
    यदि (!allocated && arrayZ)
    {
      /* If we have a non-null arrayZ but allocated is 0, then we are
       * reallocating from a foreign array. */
      Type *new_array = (Type *) hb_malloc (new_allocated * परिमाण (Type));
      यदि (unlikely (!new_array))
	फर्काऊ nullptr;
      hb_memcpy ((शून्य *) new_array, (अचर शून्य *) arrayZ, length * परिमाण (Type));
      फर्काऊ new_array;
    }
    फर्काऊ (Type *) hb_realloc (arrayZ, new_allocated * परिमाण (Type));
  }
  Type *
  _malloc_move (अचिन्हित new_allocated)
  {
    यदि (!new_allocated)
    {
      यदि (is_owned ())
	hb_free (arrayZ);
      फर्काऊ nullptr;
    }
    Type *new_array = (Type *) hb_malloc (new_allocated * परिमाण (Type));
    यदि (likely (new_array))
    {
      लागि (अचिन्हित i = 0; i < length; i++)
      {
	नूतन (std::addressof (new_array[i])) Type ();
	new_array[i] = std::move (arrayZ[i]);
	arrayZ[i].~Type ();
      }
      यदि (is_owned ())
	hb_free (arrayZ);
    }
    फर्काऊ new_array;
  }

  ढाँचा <typename T = Type,
	    hb_enable_if (hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_vector (अचिन्हित new_allocated, hb_priority<0>)
  {
    फर्काऊ _realloc (new_allocated);
  }
  ढाँचा <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_vector (अचिन्हित new_allocated, hb_priority<0>)
  {
    फर्काऊ _malloc_move (new_allocated);
  }
  /* Specialization for types that can be moved using realloc(). */
  ढाँचा <typename T = Type,
	    hb_enable_if (T::realloc_move)>
  Type *
  realloc_vector (अचिन्हित new_allocated, hb_priority<1>)
  {
    फर्काऊ _realloc (new_allocated);
  }

  ढाँचा <typename T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T))>
  शून्य
  grow_vector (अचिन्हित size, hb_priority<0>)
  {
    hb_memset (arrayZ + length, 0, (size - length) * परिमाण (*arrayZ));
    length = size;
  }
  ढाँचा <typename T = Type,
	    hb_enable_if (!hb_is_trivially_constructible(T))>
  शून्य
  grow_vector (अचिन्हित size, hb_priority<0>)
  {
    लागि (; length < size; length++)
      नूतन (std::addressof (arrayZ[length])) Type ();
  }
  /* Specialization for hb_vector_t<hb_{vector,array}_t<U>> to speed up. */
  ढाँचा <typename T = Type,
	    hb_enable_if (hb_is_same (T, hb_vector_t<typename T::item_t>) ||
			  hb_is_same (T, hb_array_t <typename T::item_t>))>
  शून्य
  grow_vector (अचिन्हित size, hb_priority<1>)
  {
    hb_memset (arrayZ + length, 0, (size - length) * परिमाण (*arrayZ));
    length = size;
  }

  ढाँचा <typename T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  शून्य
  copy_array (hb_array_t<Type> other)
  {
    hb_memcpy ((शून्य *) (arrayZ + length), (अचर शून्य *) other.arrayZ, other.length * item_size);
    length += other.length;
  }
  ढाँचा <typename T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  शून्य
  copy_array (hb_array_t<अचर Type> other)
  {
    hb_memcpy ((शून्य *) (arrayZ + length), (अचर शून्य *) other.arrayZ, other.length * item_size);
    length += other.length;
  }
  ढाँचा <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			   std::is_copy_constructible<T>::value)>
  शून्य
  copy_array (hb_array_t<अचर Type> other)
  {
    लागि (अचिन्हित i = 0; i < other.length; i++)
      नूतन (std::addressof (arrayZ[length + i])) Type (other.arrayZ[i]);
    length += other.length;
  }
  ढाँचा <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			  !std::is_copy_constructible<T>::value &&
			  std::is_default_constructible<T>::value &&
			  std::is_copy_assignable<T>::value)>
  शून्य
  copy_array (hb_array_t<अचर Type> other)
  {
    लागि (अचिन्हित i = 0; i < other.length; i++)
    {
      नूतन (std::addressof (arrayZ[length + i])) Type ();
      arrayZ[length + i] = other.arrayZ[i];
    }
    length += other.length;
  }

  शून्य
  shrink_vector (अचिन्हित size)
  {
    प्रतिज्ञा (size <= length);
    यदि (!hb_is_trivially_destructible(Type))
    {
      अचिन्हित गन्ती = length - size;
      Type *p = arrayZ + length;
      जबसम्म (गन्ती--)
        (--p)->~Type ();
    }
    length = size;
  }

  शून्य
  shift_down_vector (अचिन्हित i)
  {
    लागि (; i < length; i++)
      arrayZ[i - 1] = std::move (arrayZ[i]);
  }

  /* Allocate for size but don't adjust length. */
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  तार्किक alloc (अचिन्हित पूर्णाङ्क size, तार्किक exact=असत्य)
  {
    यदि (unlikely (in_error ()))
      फर्काऊ असत्य;

    अचिन्हित पूर्णाङ्क new_allocated;
    यदि (exact)
    {
      /* If exact was specified, we allow shrinking the storage. */
      size = hb_max (size, length);
      यदि (size <= (अचिन्हित) allocated &&
	  size >= (अचिन्हित) allocated >> 2)
	फर्काऊ सत्य;

      new_allocated = size;
    }
    अन्यथा
    {
      यदि (likely (size <= (अचिन्हित) allocated))
	फर्काऊ सत्य;

      new_allocated = allocated;
      जबसम्म (size > new_allocated)
	new_allocated += (new_allocated >> 1) + 8;
    }

    /* Reallocate */

    तार्किक overflows =
      (पूर्णाङ्क) in_error () ||
      (new_allocated < size) ||
      hb_unsigned_mul_overflows (new_allocated, परिमाण (Type));

    यदि (unlikely (overflows))
    {
      set_error ();
      फर्काऊ असत्य;
    }

    Type *new_array = realloc_vector (new_allocated, hb_prioritize);

    यदि (unlikely (new_allocated && !new_array))
    {
      यदि (new_allocated <= (अचिन्हित) allocated)
        फर्काऊ सत्य; // shrinking failed; it's okay; happens in our fuzzer

      set_error ();
      फर्काऊ असत्य;
    }

    arrayZ = new_array;
    allocated = new_allocated;

    फर्काऊ सत्य;
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  तार्किक alloc_exact (अचिन्हित पूर्णाङ्क size)
  {
    फर्काऊ alloc (size, सत्य);
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  शून्य clear ()
  {
    shrink_vector (0);
  }

  ढाँचा <typename allocator_t>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  तार्किक allocate_from_pool (allocator_t *allocator, अचिन्हित size, अचिन्हित पूर्णाङ्क initialize = सत्य)
  {
    यदि (allocator)
    {
      प्रतिज्ञा (!length && !allocated);
      arrayZ = (Type *) allocator->alloc (size * परिमाण (Type), alignof (Type));
      यदि (unlikely (!arrayZ))
      {
	set_error ();
	फर्काऊ असत्य;
      }
      यदि (initialize)
	grow_vector (size, hb_prioritize);
      अन्यथा
	length = size;
      फर्काऊ सत्य;
    }
    फर्काऊ resize_full ((पूर्णाङ्क) size, initialize, सत्य);
  }

  ढाँचा <typename allocator_t>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  तार्किक duplicate_vector_from_pool (allocator_t *allocator, अचर hb_vector_t &other)
  {
    यदि (unlikely (!allocate_from_pool (allocator, other.length, असत्य)))
      फर्काऊ असत्य;
    length = 0;
    copy_array (other.as_array ());
    फर्काऊ सत्य;
  }

  ढाँचा <typename allocator_t>
  शून्य shrink_back_to_pool (allocator_t *allocator, पूर्णाङ्क size)
  {
    अचिन्हित orig_length = length;

    shrink (size, असत्य);

    यदि (allocator && !is_owned ())
      allocator->discard (arrayZ + length, (orig_length - length) * परिमाण (Type));
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  तार्किक resize_full (पूर्णाङ्क size_, तार्किक initialize, तार्किक exact)
  {
    यदि (unlikely (size_ < 0))
      फर्काऊ असत्य;
    अचिन्हित पूर्णाङ्क size = (अचिन्हित पूर्णाङ्क) size_;
    यदि (!alloc (size, exact))
      फर्काऊ असत्य;

    यदि (size > length)
    {
      यदि (initialize)
	grow_vector (size, hb_prioritize);
    }
    अन्यथा यदि (size < length)
    {
      यदि (initialize)
	shrink_vector (size);
    }

    length = size;
    फर्काऊ सत्य;
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  तार्किक resize (पूर्णाङ्क size_)
  {
    फर्काऊ resize_full (size_, सत्य, असत्य);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  तार्किक resize_dirty (पूर्णाङ्क size_)
  {
    फर्काऊ resize_full (size_, असत्य, असत्य);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  तार्किक resize_exact (पूर्णाङ्क size_)
  {
    फर्काऊ resize_full (size_, सत्य, सत्य);
  }

  Type झिक ()
  {
    यदि (!length) फर्काऊ Null (Type);
    Type v (std::move (arrayZ[length - 1]));
    arrayZ[length - 1].~Type ();
    length--;
    फर्काऊ v;
  }

  शून्य remove_ordered (अचिन्हित पूर्णाङ्क i)
  {
    यदि (unlikely (i >= length))
      फर्काऊ;
    shift_down_vector (i + 1);
    arrayZ[length - 1].~Type ();
    length--;
  }

  ढाँचा <तार्किक Sorted = sorted,
	    hb_enable_if (!Sorted)>
  शून्य remove_unordered (अचिन्हित पूर्णाङ्क i)
  {
    यदि (unlikely (i >= length))
      फर्काऊ;
    यदि (i != length - 1)
      arrayZ[i] = std::move (arrayZ[length - 1]);
    arrayZ[length - 1].~Type ();
    length--;
  }

  शून्य shrink (पूर्णाङ्क size_, तार्किक shrink_memory = सत्य)
  {
    अचिन्हित पूर्णाङ्क size = size_ < 0 ? 0u : (अचिन्हित पूर्णाङ्क) size_;
    यदि (size >= length)
      फर्काऊ;

    shrink_vector (size);

    यदि (is_owned () && shrink_memory)
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
  Type *lsearch (अचर T &x, Type *not_found = nullptr)
  { फर्काऊ as_array ().lsearch (x, not_found); }
  ढाँचा <typename T>
  अचर Type *lsearch (अचर T &x, अचर Type *not_found = nullptr) अचर
  { फर्काऊ as_array ().lsearch (x, not_found); }
  ढाँचा <typename T>
  तार्किक lfind (अचर T &x, अचिन्हित *pos = nullptr) अचर
  { फर्काऊ as_array ().lfind (x, pos); }

  /* Sorted search API. */
  ढाँचा <typename T,
	    तार्किक Sorted=sorted, hb_enable_if (Sorted)>
  Type *द्वा_खोज (अचर T &x, Type *not_found = nullptr)
  { फर्काऊ as_array ().द्वा_खोज (x, not_found); }
  ढाँचा <typename T,
	    तार्किक Sorted=sorted, hb_enable_if (Sorted)>
  अचर Type *द्वा_खोज (अचर T &x, अचर Type *not_found = nullptr) अचर
  { फर्काऊ as_array ().द्वा_खोज (x, not_found); }
  ढाँचा <typename T,
	    तार्किक Sorted=sorted, hb_enable_if (Sorted)>
  तार्किक bfind (अचर T &x, अचिन्हित पूर्णाङ्क *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      अचिन्हित पूर्णाङ्क to_store = (अचिन्हित पूर्णाङ्क) -1) अचर
  { फर्काऊ as_array ().bfind (x, i, not_found, to_store); }
};

ढाँचा <typename Type>
using hb_sorted_vector_t = hb_vector_t<Type, सत्य>;

#endif /* HB_VECTOR_HH */

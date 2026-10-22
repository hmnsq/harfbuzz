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

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-array.hh"
#অন্তর্ভাব "hb-meta.hh"
#অন্তর্ভাব "hb-null.hh"

// Change to 1 to force inline vector allocs, to see callsite in malloc-stats tool.
#যদি 0
#define HB_ALWAYS_INLINE_VECTOR_ALLOCS HB_ALWAYS_INLINE
#অন্যথা
#define HB_ALWAYS_INLINE_VECTOR_ALLOCS
#endif

template <typename Type,
	  যৌক্তিক sorted=মিথ্যা>
কাঠামো hb_vector_t
{
  স্থির constexpr যৌক্তিক realloc_move = সত্য;

  প্রকারলেখ Type item_t;
  স্থির constexpr unsigned item_আকার = hb_static_আকার (Type);
  using array_t = typename std::conditional<sorted, hb_sorted_array_t<Type>, hb_array_t<Type>>::type;
  using c_array_t = typename std::conditional<sorted, hb_sorted_array_t<ধ্রুবক Type>, hb_array_t<ধ্রুবক Type>>::type;

  hb_vector_t () = পূর্বনির্ধারিত;
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (std::initializer_list<Type> lst) : hb_vector_t ()
  {
    alloc (lst.আকার_2 (), সত্য);
    জন্য (auto&& item : lst)
      push (item);
  }
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  explicit hb_vector_t (ধ্রুবক Iterable &o) : hb_vector_t ()
  {
    extend (o, সত্য);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (ধ্রুবক hb_vector_t &o) : hb_vector_t ()
  {
    alloc_exact (o.দৈর্ঘ্য);
    যদি (unlikely (in_error ())) ফেরত;
    copy_array (o.as_array ());
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (array_t o) : hb_vector_t ()
  {
    alloc_exact (o.দৈর্ঘ্য);
    যদি (unlikely (in_error ())) ফেরত;
    copy_array (o);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t (c_array_t o) : hb_vector_t ()
  {
    alloc_exact (o.দৈর্ঘ্য);
    যদি (unlikely (in_error ())) ফেরত;
    copy_array (o);
  }
  hb_vector_t (hb_vector_t &&o) noexcept
  {
    allocated = o.allocated;
    দৈর্ঘ্য = o.দৈর্ঘ্য;
    arrayZ = o.arrayZ;
    o.init ();
  }
  ~hb_vector_t () { fini (); }

  template <unsigned n>
  শূন্য
  set_storage (Type (&array)[n])
  { set_storage (array, n); }
  শূন্য
  set_storage (hb_array_t<Type> array)
  { set_storage (array.arrayZ, array.দৈর্ঘ্য); }
  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T) &&
			  hb_is_trivially_destructible(T))>
  শূন্য
  set_storage (Type *array, unsigned n)
  {
    প্রতিজ্ঞা (allocated == 0);
    প্রতিজ্ঞা (দৈর্ঘ্য == 0);

    arrayZ = array;
    দৈর্ঘ্য = n;
  }

  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  শূন্য extend (ধ্রুবক Iterable &o, যৌক্তিক exact=মিথ্যা)
  {
    auto iter = hb_iter (o);
    যদি (iter.is_random_access_iterator || iter.has_fast_len)
    {
      যদি (unlikely (!alloc (দৈর্ঘ্য + hb_len (iter), exact)))
	ফেরত;
      unsigned গুনতি = hb_len (iter);
      জন্য (unsigned i = 0; i < গুনতি; i++)
	ঠেলো_has_room (*iter++);
    }
    যখন (iter)
    {
      যদি (unlikely (!alloc (দৈর্ঘ্য + 1)))
        ফেরত;
      unsigned room = allocated - দৈর্ঘ্য;
      জন্য (unsigned i = 0; i < room && iter; i++)
	ঠেলো_has_room (*iter++);
    }
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  শূন্য extend (array_t o, যৌক্তিক exact=মিথ্যা)
  { extend (c_array_t (o), exact); }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  শূন্য extend (c_array_t o, যৌক্তিক exact=মিথ্যা)
  {
    /* o may point into our own storage, eg. v.extend (v.as_array ()).
     * alloc() may move that storage, so find o again afterwards. */
    যৌক্তিক aliased = o.দৈর্ঘ্য && in_storage (*o.arrayZ);
    unsigned offset = aliased ? o.arrayZ - arrayZ : 0;
    alloc (দৈর্ঘ্য + o.দৈর্ঘ্য, exact);
    যদি (unlikely (in_error ())) ফেরত;
    যদি (unlikely (aliased))
      o = {arrayZ + offset, o.দৈর্ঘ্য};
    copy_array (o);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  শূন্য extend (ধ্রুবক hb_vector_t &o, যৌক্তিক exact=মিথ্যা)
  { extend (o.as_array (), exact); }

  সর্বজনীন:
  পূর্ণসংখ্যা allocated = 0; /* < 0 means allocation failed. */
  unsigned পূর্ণসংখ্যা দৈর্ঘ্য = 0;
  সর্বজনীন:
  Type *arrayZ = nullptr;

  শূন্য init ()
  {
    allocated = দৈর্ঘ্য = 0;
    arrayZ = nullptr;
  }
  শূন্য init0 ()
  {
  }

  শূন্য fini ()
  {
    যদি (is_owned ())
    {
      shrink_vector (0);
      hb_free (arrayZ);
    }
    init ();
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t &reset ()
  {
    যদি (unlikely (in_error ()))
      reset_error ();
    clear ();
    ফেরত *এই;
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  hb_vector_t &reset_if_error ()
  {
    যদি (unlikely (in_error ()))
      reset ();
    ফেরত *এই;
  }

  /* Transfer ownership of the backing storage to caller.
   * Returns nullptr if storage is not owned by this vector. */
  Type *
  steal (unsigned *len = nullptr, পূর্ণসংখ্যা *allocated_out = nullptr)
  {
    যদি (!is_owned ())
      ফেরত nullptr;
    যদি (len)
      *len = দৈর্ঘ্য;
    যদি (allocated_out)
      *allocated_out = allocated;
    Type *p = arrayZ;
    init ();
    ফেরত p;
  }

  /* Adopt a previously detached owned buffer. */
  শূন্য
  recycle_বাফার (Type *বাফার,
                  unsigned len,
                  পূর্ণসংখ্যা allocated_len)
  {
    fini ();
    arrayZ = বাফার;
    দৈর্ঘ্য = len;
    allocated = allocated_len;
  }

  friend শূন্য swap (hb_vector_t& a, hb_vector_t& b) noexcept
  {
    hb_swap (a.allocated, b.allocated);
    hb_swap (a.দৈর্ঘ্য, b.দৈর্ঘ্য);
    hb_swap (a.arrayZ, b.arrayZ);
  }

  hb_vector_t& operator = (ধ্রুবক hb_vector_t &o)
  {
    যদি (এই == &o) ফেরত *এই;

    reset ();
    alloc_exact (o.দৈর্ঘ্য);
    যদি (unlikely (in_error ())) ফেরত *এই;

    দৈর্ঘ্য = 0;
    copy_array (o.as_array ());

    ফেরত *এই;
  }
  hb_vector_t& operator = (hb_vector_t &&o) noexcept
  {
    hb_swap (*এই, o);
    ফেরত *এই;
  }

  hb_bytes_t as_bytes () ধ্রুবক
  { ফেরত hb_bytes_t ((ধ্রুবক অক্ষর *) arrayZ, get_আকার ()); }

  যৌক্তিক operator == (ধ্রুবক hb_vector_t &o) ধ্রুবক { ফেরত as_array () == o.as_array (); }
  যৌক্তিক operator != (ধ্রুবক hb_vector_t &o) ধ্রুবক { ফেরত !(*এই == o); }
  uint32_t hash () ধ্রুবক { ফেরত as_array ().hash (); }

  Type& operator [] (পূর্ণসংখ্যা i_)
  {
    unsigned পূর্ণসংখ্যা i = (unsigned পূর্ণসংখ্যা) i_;
    যদি (unlikely (i >= দৈর্ঘ্য))
      ফেরত Crap (Type);
    ফেরত arrayZ[i];
  }
  ধ্রুবক Type& operator [] (পূর্ণসংখ্যা i_) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা i = (unsigned পূর্ণসংখ্যা) i_;
    যদি (unlikely (i >= দৈর্ঘ্য))
      ফেরত Null (Type);
    ফেরত arrayZ[i];
  }

  Type& লেজ () { ফেরত (*এই)[দৈর্ঘ্য - 1]; }
  ধ্রুবক Type& লেজ () ধ্রুবক { ফেরত (*এই)[দৈর্ঘ্য - 1]; }

  explicit operator যৌক্তিক () ধ্রুবক { ফেরত দৈর্ঘ্য; }
  size_t get_আকার () ধ্রুবক { ফেরত hb_unsigned_mul_saturate (দৈর্ঘ্য, item_আকার); }

  /* Sink interface. */
  template <typename T>
  hb_vector_t& operator << (T&& v) { push (std::forward<T> (v)); ফেরত *এই; }

  array_t   as_array ()       { ফেরত hb_array (arrayZ, দৈর্ঘ্য); }
  c_array_t as_array () ধ্রুবক { ফেরত hb_array (arrayZ, দৈর্ঘ্য); }

  /* Iterator. */
  প্রকারলেখ c_array_t   iter_t;
  প্রকারলেখ array_t   writer_t;
    iter_t   iter () ধ্রুবক { ফেরত as_array (); }
  writer_t writer ()       { ফেরত as_array (); }
  operator   iter_t () ধ্রুবক { ফেরত   iter (); }
  operator writer_t ()       { ফেরত writer (); }

  /* Faster range-based for loop. */
  Type *begin () ধ্রুবক { ফেরত arrayZ; }
  Type *end () ধ্রুবক { ফেরত arrayZ + দৈর্ঘ্য; }


  hb_sorted_array_t<Type> as_sorted_array ()
  { ফেরত hb_sorted_array (arrayZ, দৈর্ঘ্য); }
  hb_sorted_array_t<ধ্রুবক Type> as_sorted_array () ধ্রুবক
  { ফেরত hb_sorted_array (arrayZ, দৈর্ঘ্য); }

  template <typename T> explicit operator T * () { ফেরত arrayZ; }
  template <typename T> explicit operator ধ্রুবক T * () ধ্রুবক { ফেরত arrayZ; }

  Type * operator  + (unsigned পূর্ণসংখ্যা i) { ফেরত arrayZ + i; }
  ধ্রুবক Type * operator  + (unsigned পূর্ণসংখ্যা i) ধ্রুবক { ফেরত arrayZ + i; }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push ()
  {
    যদি (unlikely (!resize (দৈর্ঘ্য + 1)))
      ফেরত std::addressof (Crap (Type));
    ফেরত std::addressof (arrayZ[দৈর্ঘ্য - 1]);
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *push (Args&&... args)
  {
    যদি (unlikely ((পূর্ণসংখ্যা) দৈর্ঘ্য >= allocated))
    {
      যদি (unlikely (in_error ()))
	ফেরত std::addressof (Crap (Type));

      /* An argument may live in our own storage, eg. v.push (v[i]).
       * alloc() frees that storage, so copy it out first. */
      যদি (unlikely (in_storage (args...)))
      {
	Type copy (std::forward<Args> (args)...);
	যদি (unlikely (!alloc (দৈর্ঘ্য + 1)))
	  ফেরত std::addressof (Crap (Type));
	ফেরত ঠেলো_has_room (std::move (copy));
      }
      যদি (unlikely (!alloc (দৈর্ঘ্য + 1)))
	// If push failed to allocate then don't copy v, since this may cause
	// the created copy to leak memory since we won't have stored a
	// reference to it.
	ফেরত std::addressof (Crap (Type));
    }

    ফেরত ঠেলো_has_room (std::forward<Args> (args)...);
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  যৌক্তিক ঠেলো_or_fail (Args&&... args)
  {
    ফেরত push (std::forward<Args> (args)...) != std::addressof (Crap (Type));
  }
  template <typename... Args>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  Type *ঠেলো_has_room (Args&&... args)
  {
    /* Emplace. */
    Type *p = std::addressof (arrayZ[দৈর্ঘ্য++]);
    ফেরত নতুন (p) Type (std::forward<Args> (args)...);
  }

  যৌক্তিক in_storage () ধ্রুবক { ফেরত মিথ্যা; }
  template <typename T, typename... Ts>
  যৌক্তিক in_storage (ধ্রুবক T &v, ধ্রুবক Ts &...vs) ধ্রুবক
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    ফেরত (arrayZ &&
	    (uintptr_t) arrayZ <= p && p < (uintptr_t) (arrayZ + দৈর্ঘ্য)) ||
	   in_storage (vs...);
  }

  যৌক্তিক is_owned () ধ্রুবক
  {
    ফেরত allocated != 0 && allocated != -1;
  }

  যৌক্তিক in_error () ধ্রুবক { ফেরত allocated < 0; }
  শূন্য set_error ()
  {
    প্রতিজ্ঞা (allocated >= 0);
    allocated = -allocated - 1;
  }
  শূন্য reset_error ()
  {
    প্রতিজ্ঞা (allocated < 0);
    allocated = -(allocated + 1);
  }
  শূন্য ensure_error ()
  {
    যদি (!in_error ())
      set_error ();
  }

  Type *
  _realloc (unsigned new_allocated)
  {
    যদি (!new_allocated)
    {
      যদি (is_owned ())
	hb_free (arrayZ);
      ফেরত nullptr;
    }
    যদি (!allocated && arrayZ)
    {
      /* If we have a non-null arrayZ but allocated is 0, then we are
       * reallocating from a foreign array. */
      Type *new_array = (Type *) hb_malloc (new_allocated * আকার (Type));
      যদি (unlikely (!new_array))
	ফেরত nullptr;
      hb_memcpy ((শূন্য *) new_array, (ধ্রুবক শূন্য *) arrayZ, দৈর্ঘ্য * আকার (Type));
      ফেরত new_array;
    }
    ফেরত (Type *) hb_realloc (arrayZ, new_allocated * আকার (Type));
  }
  Type *
  _malloc_move (unsigned new_allocated)
  {
    যদি (!new_allocated)
    {
      যদি (is_owned ())
	hb_free (arrayZ);
      ফেরত nullptr;
    }
    Type *new_array = (Type *) hb_malloc (new_allocated * আকার (Type));
    যদি (likely (new_array))
    {
      জন্য (unsigned i = 0; i < দৈর্ঘ্য; i++)
      {
	নতুন (std::addressof (new_array[i])) Type ();
	new_array[i] = std::move (arrayZ[i]);
	arrayZ[i].~Type ();
      }
      যদি (is_owned ())
	hb_free (arrayZ);
    }
    ফেরত new_array;
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<0>)
  {
    ফেরত _realloc (new_allocated);
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<0>)
  {
    ফেরত _malloc_move (new_allocated);
  }
  /* Specialization for types that can be moved using realloc(). */
  template <typename T = Type,
	    hb_enable_if (T::realloc_move)>
  Type *
  realloc_vector (unsigned new_allocated, hb_priority<1>)
  {
    ফেরত _realloc (new_allocated);
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T))>
  শূন্য
  grow_vector (unsigned আকার_2, hb_priority<0>)
  {
    hb_memset (arrayZ + দৈর্ঘ্য, 0, (আকার_2 - দৈর্ঘ্য) * আকার (*arrayZ));
    দৈর্ঘ্য = আকার_2;
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_constructible(T))>
  শূন্য
  grow_vector (unsigned আকার_2, hb_priority<0>)
  {
    জন্য (; দৈর্ঘ্য < আকার_2; দৈর্ঘ্য++)
      নতুন (std::addressof (arrayZ[দৈর্ঘ্য])) Type ();
  }
  /* Specialization for hb_vector_t<hb_{vector,array}_t<U>> to speed up. */
  template <typename T = Type,
	    hb_enable_if (hb_is_same (T, hb_vector_t<typename T::item_t>) ||
			  hb_is_same (T, hb_array_t <typename T::item_t>))>
  শূন্য
  grow_vector (unsigned আকার_2, hb_priority<1>)
  {
    hb_memset (arrayZ + দৈর্ঘ্য, 0, (আকার_2 - দৈর্ঘ্য) * আকার (*arrayZ));
    দৈর্ঘ্য = আকার_2;
  }

  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  শূন্য
  copy_array (hb_array_t<Type> other)
  {
    hb_memcpy ((শূন্য *) (arrayZ + দৈর্ঘ্য), (ধ্রুবক শূন্য *) other.arrayZ, other.দৈর্ঘ্য * item_আকার);
    দৈর্ঘ্য += other.দৈর্ঘ্য;
  }
  template <typename T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  শূন্য
  copy_array (hb_array_t<ধ্রুবক Type> other)
  {
    hb_memcpy ((শূন্য *) (arrayZ + দৈর্ঘ্য), (ধ্রুবক শূন্য *) other.arrayZ, other.দৈর্ঘ্য * item_আকার);
    দৈর্ঘ্য += other.দৈর্ঘ্য;
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			   std::is_copy_constructible<T>::value)>
  শূন্য
  copy_array (hb_array_t<ধ্রুবক Type> other)
  {
    জন্য (unsigned i = 0; i < other.দৈর্ঘ্য; i++)
      নতুন (std::addressof (arrayZ[দৈর্ঘ্য + i])) Type (other.arrayZ[i]);
    দৈর্ঘ্য += other.দৈর্ঘ্য;
  }
  template <typename T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			  !std::is_copy_constructible<T>::value &&
			  std::is_default_constructible<T>::value &&
			  std::is_copy_assignable<T>::value)>
  শূন্য
  copy_array (hb_array_t<ধ্রুবক Type> other)
  {
    জন্য (unsigned i = 0; i < other.দৈর্ঘ্য; i++)
    {
      নতুন (std::addressof (arrayZ[দৈর্ঘ্য + i])) Type ();
      arrayZ[দৈর্ঘ্য + i] = other.arrayZ[i];
    }
    দৈর্ঘ্য += other.দৈর্ঘ্য;
  }

  শূন্য
  shrink_vector (unsigned আকার_2)
  {
    প্রতিজ্ঞা (আকার_2 <= দৈর্ঘ্য);
    যদি (!hb_is_trivially_destructible(Type))
    {
      unsigned গুনতি = দৈর্ঘ্য - আকার_2;
      Type *p = arrayZ + দৈর্ঘ্য;
      যখন (গুনতি--)
        (--p)->~Type ();
    }
    দৈর্ঘ্য = আকার_2;
  }

  শূন্য
  shift_down_vector (unsigned i)
  {
    জন্য (; i < দৈর্ঘ্য; i++)
      arrayZ[i - 1] = std::move (arrayZ[i]);
  }

  /* Allocate for size but don't adjust length. */
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  যৌক্তিক alloc (unsigned পূর্ণসংখ্যা আকার_2, যৌক্তিক exact=মিথ্যা)
  {
    যদি (unlikely (in_error ()))
      ফেরত মিথ্যা;

    unsigned পূর্ণসংখ্যা new_allocated;
    যদি (exact)
    {
      /* If exact was specified, we allow shrinking the storage. */
      আকার_2 = hb_max (আকার_2, দৈর্ঘ্য);
      যদি (আকার_2 <= (unsigned) allocated &&
	  আকার_2 >= (unsigned) allocated >> 2)
	ফেরত সত্য;

      new_allocated = আকার_2;
    }
    অন্যথা
    {
      যদি (likely (আকার_2 <= (unsigned) allocated))
	ফেরত সত্য;

      new_allocated = allocated;
      যখন (আকার_2 > new_allocated)
	new_allocated += (new_allocated >> 1) + 8;
    }

    /* Reallocate */

    যৌক্তিক overflows =
      (পূর্ণসংখ্যা) in_error () ||
      (new_allocated < আকার_2) ||
      hb_unsigned_mul_overflows (new_allocated, আকার (Type));

    যদি (unlikely (overflows))
    {
      set_error ();
      ফেরত মিথ্যা;
    }

    Type *new_array = realloc_vector (new_allocated, hb_prioritize);

    যদি (unlikely (new_allocated && !new_array))
    {
      যদি (new_allocated <= (unsigned) allocated)
        ফেরত সত্য; // shrinking failed; it's okay; happens in our fuzzer

      set_error ();
      ফেরত মিথ্যা;
    }

    arrayZ = new_array;
    allocated = new_allocated;

    ফেরত সত্য;
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  যৌক্তিক alloc_exact (unsigned পূর্ণসংখ্যা আকার_2)
  {
    ফেরত alloc (আকার_2, সত্য);
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  শূন্য clear ()
  {
    shrink_vector (0);
  }

  template <typename allocator_t>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  যৌক্তিক allocate_from_pool (allocator_t *allocator, unsigned আকার_2, unsigned পূর্ণসংখ্যা initialize = সত্য)
  {
    যদি (allocator)
    {
      প্রতিজ্ঞা (!দৈর্ঘ্য && !allocated);
      arrayZ = (Type *) allocator->alloc (আকার_2 * আকার (Type), alignof (Type));
      যদি (unlikely (!arrayZ))
      {
	set_error ();
	ফেরত মিথ্যা;
      }
      যদি (initialize)
	grow_vector (আকার_2, hb_prioritize);
      অন্যথা
	দৈর্ঘ্য = আকার_2;
      ফেরত সত্য;
    }
    ফেরত resize_full ((পূর্ণসংখ্যা) আকার_2, initialize, সত্য);
  }

  template <typename allocator_t>
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  যৌক্তিক duplicate_vector_from_pool (allocator_t *allocator, ধ্রুবক hb_vector_t &other)
  {
    যদি (unlikely (!allocate_from_pool (allocator, other.দৈর্ঘ্য, মিথ্যা)))
      ফেরত মিথ্যা;
    দৈর্ঘ্য = 0;
    copy_array (other.as_array ());
    ফেরত সত্য;
  }

  template <typename allocator_t>
  শূন্য shrink_back_to_pool (allocator_t *allocator, পূর্ণসংখ্যা আকার_2)
  {
    unsigned orig_দৈর্ঘ্য = দৈর্ঘ্য;

    shrink (আকার_2, মিথ্যা);

    যদি (allocator && !is_owned ())
      allocator->discard (arrayZ + দৈর্ঘ্য, (orig_দৈর্ঘ্য - দৈর্ঘ্য) * আকার (Type));
  }

  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  যৌক্তিক resize_full (পূর্ণসংখ্যা আকার_4, যৌক্তিক initialize, যৌক্তিক exact)
  {
    যদি (unlikely (আকার_4 < 0))
      ফেরত মিথ্যা;
    unsigned পূর্ণসংখ্যা আকার_2 = (unsigned পূর্ণসংখ্যা) আকার_4;
    যদি (!alloc (আকার_2, exact))
      ফেরত মিথ্যা;

    যদি (আকার_2 > দৈর্ঘ্য)
    {
      যদি (initialize)
	grow_vector (আকার_2, hb_prioritize);
    }
    অন্যথা যদি (আকার_2 < দৈর্ঘ্য)
    {
      যদি (initialize)
	shrink_vector (আকার_2);
    }

    দৈর্ঘ্য = আকার_2;
    ফেরত সত্য;
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  যৌক্তিক resize (পূর্ণসংখ্যা আকার_4)
  {
    ফেরত resize_full (আকার_4, সত্য, মিথ্যা);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  যৌক্তিক resize_dirty (পূর্ণসংখ্যা আকার_4)
  {
    ফেরত resize_full (আকার_4, মিথ্যা, মিথ্যা);
  }
  HB_ALWAYS_INLINE_VECTOR_ALLOCS
  যৌক্তিক resize_exact (পূর্ণসংখ্যা আকার_4)
  {
    ফেরত resize_full (আকার_4, সত্য, সত্য);
  }

  Type বের_করো ()
  {
    যদি (!দৈর্ঘ্য) ফেরত Null (Type);
    Type v (std::move (arrayZ[দৈর্ঘ্য - 1]));
    arrayZ[দৈর্ঘ্য - 1].~Type ();
    দৈর্ঘ্য--;
    ফেরত v;
  }

  শূন্য remove_ordered (unsigned পূর্ণসংখ্যা i)
  {
    যদি (unlikely (i >= দৈর্ঘ্য))
      ফেরত;
    shift_down_vector (i + 1);
    arrayZ[দৈর্ঘ্য - 1].~Type ();
    দৈর্ঘ্য--;
  }

  template <যৌক্তিক Sorted = sorted,
	    hb_enable_if (!Sorted)>
  শূন্য remove_unordered (unsigned পূর্ণসংখ্যা i)
  {
    যদি (unlikely (i >= দৈর্ঘ্য))
      ফেরত;
    যদি (i != দৈর্ঘ্য - 1)
      arrayZ[i] = std::move (arrayZ[দৈর্ঘ্য - 1]);
    arrayZ[দৈর্ঘ্য - 1].~Type ();
    দৈর্ঘ্য--;
  }

  শূন্য shrink (পূর্ণসংখ্যা আকার_4, যৌক্তিক shrink_memory = সত্য)
  {
    unsigned পূর্ণসংখ্যা আকার_2 = আকার_4 < 0 ? 0u : (unsigned পূর্ণসংখ্যা) আকার_4;
    যদি (আকার_2 >= দৈর্ঘ্য)
      ফেরত;

    shrink_vector (আকার_2);

    যদি (is_owned () && shrink_memory)
      alloc_exact (আকার_2); /* To force shrinking memory if needed. */
  }


  /* Sorting API. */
  template <typename Compar>
  শূন্য qsort (Compar compar)
  { as_array ().qsort (compar); }
  শূন্য qsort ()
  { as_array ().qsort (); }

  /* Unsorted search API. */
  template <typename T>
  Type *lsearch (ধ্রুবক T &x, Type *not_found = nullptr)
  { ফেরত as_array ().lsearch (x, not_found); }
  template <typename T>
  ধ্রুবক Type *lsearch (ধ্রুবক T &x, ধ্রুবক Type *not_found = nullptr) ধ্রুবক
  { ফেরত as_array ().lsearch (x, not_found); }
  template <typename T>
  যৌক্তিক lfind (ধ্রুবক T &x, unsigned *pos = nullptr) ধ্রুবক
  { ফেরত as_array ().lfind (x, pos); }

  /* Sorted search API. */
  template <typename T,
	    যৌক্তিক Sorted=sorted, hb_enable_if (Sorted)>
  Type *bsearch (ধ্রুবক T &x, Type *not_found = nullptr)
  { ফেরত as_array ().bsearch (x, not_found); }
  template <typename T,
	    যৌক্তিক Sorted=sorted, hb_enable_if (Sorted)>
  ধ্রুবক Type *bsearch (ধ্রুবক T &x, ধ্রুবক Type *not_found = nullptr) ধ্রুবক
  { ফেরত as_array ().bsearch (x, not_found); }
  template <typename T,
	    যৌক্তিক Sorted=sorted, hb_enable_if (Sorted)>
  যৌক্তিক bfind (ধ্রুবক T &x, unsigned পূর্ণসংখ্যা *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned পূর্ণসংখ্যা to_store = (unsigned পূর্ণসংখ্যা) -1) ধ্রুবক
  { ফেরত as_array ().bfind (x, i, not_found, to_store); }
};

template <typename Type>
using hb_sorted_vector_t = hb_vector_t<Type, সত্য>;

#endif /* HB_VECTOR_HH */

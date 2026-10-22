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

#که_نه_تعریف_شوی HB_ویکټور_HH
#تعریف_کړه HB_ویکټور_HH

#ضمن "hb.hh"
#ضمن "hb-array.hh"
#ضمن "hb-meta.hh"
#ضمن "hb-null.hh"

// Change to 1 to force inline vector allocs, to see callsite in malloc-stats tool.
#إذا 0
#تعریف_کړه HB_ALWAYS_INLINE_ویکټور_ALLOCS HB_ALWAYS_INLINE
#وإلا
#تعریف_کړه HB_ALWAYS_INLINE_ویکټور_ALLOCS
#د_که_پای

کينډۍ <ډول_نوم Type,
	  منطقي sorted=خطأ>
بنية hb_ویکټور_t
{
  ساكن constexpr منطقي realloc_move = ریښتیا;

  تعريفنوع Type توکی_t;
  ساكن constexpr بینښه توکی_اندازه = hb_static_اندازه (Type);
  کارول array_t = ډول_نوم std::conditional<sorted, hb_sorted_array_t<Type>, hb_array_t<Type>>::type;
  کارول c_array_t = ډول_نوم std::conditional<sorted, hb_sorted_array_t<ثابت Type>, hb_array_t<ثابت Type>>::type;

  hb_ویکټور_t () = افتراضي;
  HB_ALWAYS_INLINE_ویکټور_ALLOCS
  hb_ویکټور_t (std::initializer_list<Type> lst) : hb_ویکټور_t ()
  {
    alloc (lst.اندازه (), ریښتیا);
    لكل (خپلکاری&& توکی : lst)
      push (توکی);
  }
  کينډۍ <ډول_نوم Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  څرګند hb_ویکټور_t (ثابت Iterable &o) : hb_ویکټور_t ()
  {
    extend (o, ریښتیا);
  }
  HB_ALWAYS_INLINE_ویکټور_ALLOCS
  hb_ویکټور_t (ثابت hb_ویکټور_t &o) : hb_ویکټور_t ()
  {
    alloc_exact (o.اوږدتیا);
    إذا (unlikely (دننه_error ())) أرجع;
    copy_array (o.as_array ());
  }
  HB_ALWAYS_INLINE_ویکټور_ALLOCS
  hb_ویکټور_t (array_t o) : hb_ویکټور_t ()
  {
    alloc_exact (o.اوږدتیا);
    إذا (unlikely (دننه_error ())) أرجع;
    copy_array (o);
  }
  HB_ALWAYS_INLINE_ویکټور_ALLOCS
  hb_ویکټور_t (c_array_t o) : hb_ویکټور_t ()
  {
    alloc_exact (o.اوږدتیا);
    إذا (unlikely (دننه_error ())) أرجع;
    copy_array (o);
  }
  hb_ویکټور_t (hb_ویکټور_t &&o) noexcept
  {
    allocated = o.allocated;
    اوږدتیا = o.اوږدتیا;
    arrayZ = o.arrayZ;
    o.init ();
  }
  ~hb_ویکټور_t () { fini (); }

  کينډۍ <بینښه n>
  فراغ
  سېټ_storage (Type (&array)[n])
  { سېټ_storage (array, n); }
  فراغ
  سېټ_storage (hb_array_t<Type> array)
  { سېټ_storage (array.arrayZ, array.اوږدتیا); }
  کينډۍ <ډول_نوم T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T) &&
			  hb_is_trivially_destructible(T))>
  فراغ
  سېټ_storage (Type *array, بینښه n)
  {
    أكد (allocated == 0);
    أكد (اوږدتیا == 0);

    arrayZ = array;
    اوږدتیا = n;
  }

  کينډۍ <ډول_نوم Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  HB_ALWAYS_INLINE_ویکټور_ALLOCS
  فراغ extend (ثابت Iterable &o, منطقي exact=خطأ)
  {
    خپلکاری iter = hb_iter (o);
    إذا (iter.is_random_access_iterator || iter.has_fast_len)
    {
      إذا (unlikely (!alloc (اوږدتیا + hb_len (iter), exact)))
	أرجع;
      بینښه شمېر = hb_len (iter);
      لكل (بینښه i = 0; i < شمېر; i++)
	ټېل_has_ځای (*iter++);
    }
    طالما (iter)
    {
      إذا (unlikely (!alloc (اوږدتیا + 1)))
        أرجع;
      بینښه ځای = allocated - اوږدتیا;
      لكل (بینښه i = 0; i < ځای && iter; i++)
	ټېل_has_ځای (*iter++);
    }
  }
  HB_ALWAYS_INLINE_ویکټور_ALLOCS
  فراغ extend (array_t o, منطقي exact=خطأ)
  { extend (c_array_t (o), exact); }
  HB_ALWAYS_INLINE_ویکټور_ALLOCS
  فراغ extend (c_array_t o, منطقي exact=خطأ)
  {
    /* o may point into our own storage, eg. v.extend (v.as_array ()).
     * alloc() may move that storage, so find o again afterwards. */
    منطقي aliased = o.اوږدتیا && دننه_storage (*o.arrayZ);
    بینښه دننه_واټن = aliased ? o.arrayZ - arrayZ : 0;
    alloc (اوږدتیا + o.اوږدتیا, exact);
    إذا (unlikely (دننه_error ())) أرجع;
    إذا (unlikely (aliased))
      o = {arrayZ + دننه_واټن, o.اوږدتیا};
    copy_array (o);
  }
  HB_ALWAYS_INLINE_ویکټور_ALLOCS
  فراغ extend (ثابت hb_ویکټور_t &o, منطقي exact=خطأ)
  { extend (o.as_array (), exact); }

  عام:
  صحيح allocated = 0; /* < 0 means allocation failed. */
  بینښه صحيح اوږدتیا = 0;
  عام:
  Type *arrayZ = تش_اشاره;

  فراغ init ()
  {
    allocated = اوږدتیا = 0;
    arrayZ = تش_اشاره;
  }
  فراغ init0 ()
  {
  }

  فراغ fini ()
  {
    إذا (is_owned ())
    {
      shrink_ویکټور (0);
      hb_free (arrayZ);
    }
    init ();
  }

  HB_ALWAYS_INLINE_ویکټور_ALLOCS
  hb_ویکټور_t &reset ()
  {
    إذا (unlikely (دننه_error ()))
      reset_error ();
    clear ();
    أرجع *هذا;
  }

  HB_ALWAYS_INLINE_ویکټور_ALLOCS
  hb_ویکټور_t &reset_if_error ()
  {
    إذا (unlikely (دننه_error ()))
      reset ();
    أرجع *هذا;
  }

  /* Transfer ownership of the backing storage to caller.
   * Returns nullptr if storage is not owned by this vector. */
  Type *
  steal (بینښه *len = تش_اشاره, صحيح *allocated_بهر = تش_اشاره)
  {
    إذا (!is_owned ())
      أرجع تش_اشاره;
    إذا (len)
      *len = اوږدتیا;
    إذا (allocated_بهر)
      *allocated_بهر = allocated;
    Type *p = arrayZ;
    init ();
    أرجع p;
  }

  /* Adopt a previously detached owned buffer. */
  فراغ
  recycle_بفر (Type *بفر,
                  بینښه len,
                  صحيح allocated_len)
  {
    fini ();
    arrayZ = بفر;
    اوږدتیا = len;
    allocated = allocated_len;
  }

  ملګری فراغ swap (hb_ویکټور_t& a, hb_ویکټور_t& b) noexcept
  {
    hb_بدلون (a.allocated, b.allocated);
    hb_بدلون (a.اوږدتیا, b.اوږدتیا);
    hb_بدلون (a.arrayZ, b.arrayZ);
  }

  hb_ویکټور_t& چلوونکی = (ثابت hb_ویکټور_t &o)
  {
    إذا (هذا == &o) أرجع *هذا;

    reset ();
    alloc_exact (o.اوږدتیا);
    إذا (unlikely (دننه_error ())) أرجع *هذا;

    اوږدتیا = 0;
    copy_array (o.as_array ());

    أرجع *هذا;
  }
  hb_ویکټور_t& چلوونکی = (hb_ویکټور_t &&o) noexcept
  {
    hb_بدلون (*هذا, o);
    أرجع *هذا;
  }

  hb_بایټونه_t as_بایټونه () ثابت
  { أرجع hb_بایټونه_t ((ثابت حرف *) arrayZ, get_اندازه ()); }

  منطقي چلوونکی == (ثابت hb_ویکټور_t &o) ثابت { أرجع as_array () == o.as_array (); }
  منطقي چلوونکی != (ثابت hb_ویکټور_t &o) ثابت { أرجع !(*هذا == o); }
  uint32_t hash () ثابت { أرجع as_array ().hash (); }

  Type& چلوونکی [] (صحيح i_)
  {
    بینښه صحيح i = (بینښه صحيح) i_;
    إذا (unlikely (i >= اوږدتیا))
      أرجع Crap (Type);
    أرجع arrayZ[i];
  }
  ثابت Type& چلوونکی [] (صحيح i_) ثابت
  {
    بینښه صحيح i = (بینښه صحيح) i_;
    إذا (unlikely (i >= اوږدتیا))
      أرجع Null (Type);
    أرجع arrayZ[i];
  }

  Type& لکۍ () { أرجع (*هذا)[اوږدتیا - 1]; }
  ثابت Type& لکۍ () ثابت { أرجع (*هذا)[اوږدتیا - 1]; }

  څرګند چلوونکی منطقي () ثابت { أرجع اوږدتیا; }
  size_t get_اندازه () ثابت { أرجع hb_unsigned_mul_saturate (اوږدتیا, توکی_اندازه); }

  /* Sink interface. */
  کينډۍ <ډول_نوم T>
  hb_ویکټور_t& چلوونکی << (T&& v) { push (std::forward<T> (v)); أرجع *هذا; }

  array_t   as_array ()       { أرجع hb_array (arrayZ, اوږدتیا); }
  c_array_t as_array () ثابت { أرجع hb_array (arrayZ, اوږدتیا); }

  /* Iterator. */
  تعريفنوع c_array_t   iter_t;
  تعريفنوع array_t   writer_t;
    iter_t   iter () ثابت { أرجع as_array (); }
  writer_t writer ()       { أرجع as_array (); }
  چلوونکی   iter_t () ثابت { أرجع   iter (); }
  چلوونکی writer_t ()       { أرجع writer (); }

  /* Faster range-based for loop. */
  Type *begin () ثابت { أرجع arrayZ; }
  Type *پای_ټکی () ثابت { أرجع arrayZ + اوږدتیا; }


  hb_sorted_array_t<Type> as_sorted_array ()
  { أرجع hb_sorted_array (arrayZ, اوږدتیا); }
  hb_sorted_array_t<ثابت Type> as_sorted_array () ثابت
  { أرجع hb_sorted_array (arrayZ, اوږدتیا); }

  کينډۍ <ډول_نوم T> څرګند چلوونکی T * () { أرجع arrayZ; }
  کينډۍ <ډول_نوم T> څرګند چلوونکی ثابت T * () ثابت { أرجع arrayZ; }

  Type * چلوونکی  + (بینښه صحيح i) { أرجع arrayZ + i; }
  ثابت Type * چلوونکی  + (بینښه صحيح i) ثابت { أرجع arrayZ + i; }

  HB_ALWAYS_INLINE_ویکټور_ALLOCS
  Type *push ()
  {
    إذا (unlikely (!resize (اوږدتیا + 1)))
      أرجع std::addressof (Crap (Type));
    أرجع std::addressof (arrayZ[اوږدتیا - 1]);
  }
  کينډۍ <ډول_نوم... دلیلونه_2>
  HB_ALWAYS_INLINE_ویکټور_ALLOCS
  Type *push (دلیلونه_2&&... دلیلونه)
  {
    إذا (unlikely ((صحيح) اوږدتیا >= allocated))
    {
      إذا (unlikely (دننه_error ()))
	أرجع std::addressof (Crap (Type));

      /* An argument may live in our own storage, eg. v.push (v[i]).
       * alloc() frees that storage, so copy it out first. */
      إذا (unlikely (دننه_storage (دلیلونه...)))
      {
	Type copy (std::forward<دلیلونه_2> (دلیلونه)...);
	إذا (unlikely (!alloc (اوږدتیا + 1)))
	  أرجع std::addressof (Crap (Type));
	أرجع ټېل_has_ځای (std::move (copy));
      }
      إذا (unlikely (!alloc (اوږدتیا + 1)))
	// If push failed to allocate then don't copy v, since this may cause
	// the created copy to leak memory since we won't have stored a
	// reference to it.
	أرجع std::addressof (Crap (Type));
    }

    أرجع ټېل_has_ځای (std::forward<دلیلونه_2> (دلیلونه)...);
  }
  کينډۍ <ډول_نوم... دلیلونه_2>
  HB_ALWAYS_INLINE_ویکټور_ALLOCS
  منطقي ټېل_or_fail (دلیلونه_2&&... دلیلونه)
  {
    أرجع push (std::forward<دلیلونه_2> (دلیلونه)...) != std::addressof (Crap (Type));
  }
  کينډۍ <ډول_نوم... دلیلونه_2>
  HB_ALWAYS_INLINE_ویکټور_ALLOCS
  Type *ټېل_has_ځای (دلیلونه_2&&... دلیلونه)
  {
    /* Emplace. */
    Type *p = std::addressof (arrayZ[اوږدتیا++]);
    أرجع جديد (p) Type (std::forward<دلیلونه_2> (دلیلونه)...);
  }

  منطقي دننه_storage () ثابت { أرجع خطأ; }
  کينډۍ <ډول_نوم T, ډول_نوم... Ts>
  منطقي دننه_storage (ثابت T &v, ثابت Ts &...vs) ثابت
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    أرجع (arrayZ &&
	    (uintptr_t) arrayZ <= p && p < (uintptr_t) (arrayZ + اوږدتیا)) ||
	   دننه_storage (vs...);
  }

  منطقي is_owned () ثابت
  {
    أرجع allocated != 0 && allocated != -1;
  }

  منطقي دننه_error () ثابت { أرجع allocated < 0; }
  فراغ سېټ_error ()
  {
    أكد (allocated >= 0);
    allocated = -allocated - 1;
  }
  فراغ reset_error ()
  {
    أكد (allocated < 0);
    allocated = -(allocated + 1);
  }
  فراغ ensure_error ()
  {
    إذا (!دننه_error ())
      سېټ_error ();
  }

  Type *
  _realloc (بینښه new_allocated)
  {
    إذا (!new_allocated)
    {
      إذا (is_owned ())
	hb_free (arrayZ);
      أرجع تش_اشاره;
    }
    إذا (!allocated && arrayZ)
    {
      /* If we have a non-null arrayZ but allocated is 0, then we are
       * reallocating from a foreign array. */
      Type *new_array = (Type *) hb_malloc (new_allocated * حجم (Type));
      إذا (unlikely (!new_array))
	أرجع تش_اشاره;
      hb_memcpy ((فراغ *) new_array, (ثابت فراغ *) arrayZ, اوږدتیا * حجم (Type));
      أرجع new_array;
    }
    أرجع (Type *) hb_realloc (arrayZ, new_allocated * حجم (Type));
  }
  Type *
  _malloc_move (بینښه new_allocated)
  {
    إذا (!new_allocated)
    {
      إذا (is_owned ())
	hb_free (arrayZ);
      أرجع تش_اشاره;
    }
    Type *new_array = (Type *) hb_malloc (new_allocated * حجم (Type));
    إذا (likely (new_array))
    {
      لكل (بینښه i = 0; i < اوږدتیا; i++)
      {
	جديد (std::addressof (new_array[i])) Type ();
	new_array[i] = std::move (arrayZ[i]);
	arrayZ[i].~Type ();
      }
      إذا (is_owned ())
	hb_free (arrayZ);
    }
    أرجع new_array;
  }

  کينډۍ <ډول_نوم T = Type,
	    hb_enable_if (hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_ویکټور (بینښه new_allocated, hb_لومړیتوب<0>)
  {
    أرجع _realloc (new_allocated);
  }
  کينډۍ <ډول_نوم T = Type,
	    hb_enable_if (!hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_ویکټور (بینښه new_allocated, hb_لومړیتوب<0>)
  {
    أرجع _malloc_move (new_allocated);
  }
  /* Specialization for types that can be moved using realloc(). */
  کينډۍ <ډول_نوم T = Type,
	    hb_enable_if (T::realloc_move)>
  Type *
  realloc_ویکټور (بینښه new_allocated, hb_لومړیتوب<1>)
  {
    أرجع _realloc (new_allocated);
  }

  کينډۍ <ډول_نوم T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T))>
  فراغ
  grow_ویکټور (بینښه اندازه, hb_لومړیتوب<0>)
  {
    hb_memset (arrayZ + اوږدتیا, 0, (اندازه - اوږدتیا) * حجم (*arrayZ));
    اوږدتیا = اندازه;
  }
  کينډۍ <ډول_نوم T = Type,
	    hb_enable_if (!hb_is_trivially_constructible(T))>
  فراغ
  grow_ویکټور (بینښه اندازه, hb_لومړیتوب<0>)
  {
    لكل (; اوږدتیا < اندازه; اوږدتیا++)
      جديد (std::addressof (arrayZ[اوږدتیا])) Type ();
  }
  /* Specialization for hb_vector_t<hb_{vector,array}_t<U>> to speed up. */
  کينډۍ <ډول_نوم T = Type,
	    hb_enable_if (hb_is_same (T, hb_ویکټور_t<ډول_نوم T::توکی_t>) ||
			  hb_is_same (T, hb_array_t <ډول_نوم T::توکی_t>))>
  فراغ
  grow_ویکټور (بینښه اندازه, hb_لومړیتوب<1>)
  {
    hb_memset (arrayZ + اوږدتیا, 0, (اندازه - اوږدتیا) * حجم (*arrayZ));
    اوږدتیا = اندازه;
  }

  کينډۍ <ډول_نوم T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  فراغ
  copy_array (hb_array_t<Type> other)
  {
    hb_memcpy ((فراغ *) (arrayZ + اوږدتیا), (ثابت فراغ *) other.arrayZ, other.اوږدتیا * توکی_اندازه);
    اوږدتیا += other.اوږدتیا;
  }
  کينډۍ <ډول_نوم T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  فراغ
  copy_array (hb_array_t<ثابت Type> other)
  {
    hb_memcpy ((فراغ *) (arrayZ + اوږدتیا), (ثابت فراغ *) other.arrayZ, other.اوږدتیا * توکی_اندازه);
    اوږدتیا += other.اوږدتیا;
  }
  کينډۍ <ډول_نوم T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			   std::is_copy_constructible<T>::value)>
  فراغ
  copy_array (hb_array_t<ثابت Type> other)
  {
    لكل (بینښه i = 0; i < other.اوږدتیا; i++)
      جديد (std::addressof (arrayZ[اوږدتیا + i])) Type (other.arrayZ[i]);
    اوږدتیا += other.اوږدتیا;
  }
  کينډۍ <ډول_نوم T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			  !std::is_copy_constructible<T>::value &&
			  std::is_default_constructible<T>::value &&
			  std::is_copy_assignable<T>::value)>
  فراغ
  copy_array (hb_array_t<ثابت Type> other)
  {
    لكل (بینښه i = 0; i < other.اوږدتیا; i++)
    {
      جديد (std::addressof (arrayZ[اوږدتیا + i])) Type ();
      arrayZ[اوږدتیا + i] = other.arrayZ[i];
    }
    اوږدتیا += other.اوږدتیا;
  }

  فراغ
  shrink_ویکټور (بینښه اندازه)
  {
    أكد (اندازه <= اوږدتیا);
    إذا (!hb_is_trivially_destructible(Type))
    {
      بینښه شمېر = اوږدتیا - اندازه;
      Type *p = arrayZ + اوږدتیا;
      طالما (شمېر--)
        (--p)->~Type ();
    }
    اوږدتیا = اندازه;
  }

  فراغ
  ښويدنه_ښکته_ویکټور (بینښه i)
  {
    لكل (; i < اوږدتیا; i++)
      arrayZ[i - 1] = std::move (arrayZ[i]);
  }

  /* Allocate for size but don't adjust length. */
  HB_ALWAYS_INLINE_ویکټور_ALLOCS
  منطقي alloc (بینښه صحيح اندازه, منطقي exact=خطأ)
  {
    إذا (unlikely (دننه_error ()))
      أرجع خطأ;

    بینښه صحيح new_allocated;
    إذا (exact)
    {
      /* If exact was specified, we allow shrinking the storage. */
      اندازه = hb_max (اندازه, اوږدتیا);
      إذا (اندازه <= (بینښه) allocated &&
	  اندازه >= (بینښه) allocated >> 2)
	أرجع ریښتیا;

      new_allocated = اندازه;
    }
    وإلا
    {
      إذا (likely (اندازه <= (بینښه) allocated))
	أرجع ریښتیا;

      new_allocated = allocated;
      طالما (اندازه > new_allocated)
	new_allocated += (new_allocated >> 1) + 8;
    }

    /* Reallocate */

    منطقي overflows =
      (صحيح) دننه_error () ||
      (new_allocated < اندازه) ||
      hb_unsigned_mul_overflows (new_allocated, حجم (Type));

    إذا (unlikely (overflows))
    {
      سېټ_error ();
      أرجع خطأ;
    }

    Type *new_array = realloc_ویکټور (new_allocated, hb_prioritize);

    إذا (unlikely (new_allocated && !new_array))
    {
      إذا (new_allocated <= (بینښه) allocated)
        أرجع ریښتیا; // shrinking failed; it's okay; happens in our fuzzer

      سېټ_error ();
      أرجع خطأ;
    }

    arrayZ = new_array;
    allocated = new_allocated;

    أرجع ریښتیا;
  }
  HB_ALWAYS_INLINE_ویکټور_ALLOCS
  منطقي alloc_exact (بینښه صحيح اندازه)
  {
    أرجع alloc (اندازه, ریښتیا);
  }

  HB_ALWAYS_INLINE_ویکټور_ALLOCS
  فراغ clear ()
  {
    shrink_ویکټور (0);
  }

  کينډۍ <ډول_نوم allocator_t>
  HB_ALWAYS_INLINE_ویکټور_ALLOCS
  منطقي allocate_from_pool (allocator_t *allocator, بینښه اندازه, بینښه صحيح initialize = ریښتیا)
  {
    إذا (allocator)
    {
      أكد (!اوږدتیا && !allocated);
      arrayZ = (Type *) allocator->alloc (اندازه * حجم (Type), alignof (Type));
      إذا (unlikely (!arrayZ))
      {
	سېټ_error ();
	أرجع خطأ;
      }
      إذا (initialize)
	grow_ویکټور (اندازه, hb_prioritize);
      وإلا
	اوږدتیا = اندازه;
      أرجع ریښتیا;
    }
    أرجع resize_ډک ((صحيح) اندازه, initialize, ریښتیا);
  }

  کينډۍ <ډول_نوم allocator_t>
  HB_ALWAYS_INLINE_ویکټور_ALLOCS
  منطقي duplicate_ویکټور_from_pool (allocator_t *allocator, ثابت hb_ویکټور_t &other)
  {
    إذا (unlikely (!allocate_from_pool (allocator, other.اوږدتیا, خطأ)))
      أرجع خطأ;
    اوږدتیا = 0;
    copy_array (other.as_array ());
    أرجع ریښتیا;
  }

  کينډۍ <ډول_نوم allocator_t>
  فراغ shrink_شا_منزل_pool (allocator_t *allocator, صحيح اندازه)
  {
    بینښه orig_اوږدتیا = اوږدتیا;

    shrink (اندازه, خطأ);

    إذا (allocator && !is_owned ())
      allocator->discard (arrayZ + اوږدتیا, (orig_اوږدتیا - اوږدتیا) * حجم (Type));
  }

  HB_ALWAYS_INLINE_ویکټور_ALLOCS
  منطقي resize_ډک (صحيح اندازه_3, منطقي initialize, منطقي exact)
  {
    إذا (unlikely (اندازه_3 < 0))
      أرجع خطأ;
    بینښه صحيح اندازه = (بینښه صحيح) اندازه_3;
    إذا (!alloc (اندازه, exact))
      أرجع خطأ;

    إذا (اندازه > اوږدتیا)
    {
      إذا (initialize)
	grow_ویکټور (اندازه, hb_prioritize);
    }
    وإلا إذا (اندازه < اوږدتیا)
    {
      إذا (initialize)
	shrink_ویکټور (اندازه);
    }

    اوږدتیا = اندازه;
    أرجع ریښتیا;
  }
  HB_ALWAYS_INLINE_ویکټور_ALLOCS
  منطقي resize (صحيح اندازه_3)
  {
    أرجع resize_ډک (اندازه_3, ریښتیا, خطأ);
  }
  HB_ALWAYS_INLINE_ویکټور_ALLOCS
  منطقي resize_dirty (صحيح اندازه_3)
  {
    أرجع resize_ډک (اندازه_3, خطأ, خطأ);
  }
  HB_ALWAYS_INLINE_ویکټور_ALLOCS
  منطقي resize_exact (صحيح اندازه_3)
  {
    أرجع resize_ډک (اندازه_3, ریښتیا, ریښتیا);
  }

  Type راوباسه ()
  {
    إذا (!اوږدتیا) أرجع Null (Type);
    Type v (std::move (arrayZ[اوږدتیا - 1]));
    arrayZ[اوږدتیا - 1].~Type ();
    اوږدتیا--;
    أرجع v;
  }

  فراغ remove_ordered (بینښه صحيح i)
  {
    إذا (unlikely (i >= اوږدتیا))
      أرجع;
    ښويدنه_ښکته_ویکټور (i + 1);
    arrayZ[اوږدتیا - 1].~Type ();
    اوږدتیا--;
  }

  کينډۍ <منطقي Sorted = sorted,
	    hb_enable_if (!Sorted)>
  فراغ remove_unordered (بینښه صحيح i)
  {
    إذا (unlikely (i >= اوږدتیا))
      أرجع;
    إذا (i != اوږدتیا - 1)
      arrayZ[i] = std::move (arrayZ[اوږدتیا - 1]);
    arrayZ[اوږدتیا - 1].~Type ();
    اوږدتیا--;
  }

  فراغ shrink (صحيح اندازه_3, منطقي shrink_حافظه = ریښتیا)
  {
    بینښه صحيح اندازه = اندازه_3 < 0 ? 0u : (بینښه صحيح) اندازه_3;
    إذا (اندازه >= اوږدتیا)
      أرجع;

    shrink_ویکټور (اندازه);

    إذا (is_owned () && shrink_حافظه)
      alloc_exact (اندازه); /* To force shrinking memory if needed. */
  }


  /* Sorting API. */
  کينډۍ <ډول_نوم Compar>
  فراغ qsort (Compar compar)
  { as_array ().qsort (compar); }
  فراغ qsort ()
  { as_array ().qsort (); }

  /* Unsorted search API. */
  کينډۍ <ډول_نوم T>
  Type *lsearch (ثابت T &x, Type *not_وموندل = تش_اشاره)
  { أرجع as_array ().lsearch (x, not_وموندل); }
  کينډۍ <ډول_نوم T>
  ثابت Type *lsearch (ثابت T &x, ثابت Type *not_وموندل = تش_اشاره) ثابت
  { أرجع as_array ().lsearch (x, not_وموندل); }
  کينډۍ <ډول_نوم T>
  منطقي lfind (ثابت T &x, بینښه *pos = تش_اشاره) ثابت
  { أرجع as_array ().lfind (x, pos); }

  /* Sorted search API. */
  کينډۍ <ډول_نوم T,
	    منطقي Sorted=sorted, hb_enable_if (Sorted)>
  Type *bsearch (ثابت T &x, Type *not_وموندل = تش_اشاره)
  { أرجع as_array ().bsearch (x, not_وموندل); }
  کينډۍ <ډول_نوم T,
	    منطقي Sorted=sorted, hb_enable_if (Sorted)>
  ثابت Type *bsearch (ثابت T &x, ثابت Type *not_وموندل = تش_اشاره) ثابت
  { أرجع as_array ().bsearch (x, not_وموندل); }
  کينډۍ <ډول_نوم T,
	    منطقي Sorted=sorted, hb_enable_if (Sorted)>
  منطقي bfind (ثابت T &x, بینښه صحيح *i = تش_اشاره,
	      hb_not_وموندل_t not_وموندل = HB_NOT_وموندل_DONT_STORE,
	      بینښه صحيح منزل_store = (بینښه صحيح) -1) ثابت
  { أرجع as_array ().bfind (x, i, not_وموندل, منزل_store); }
};

کينډۍ <ډول_نوم Type>
کارول hb_sorted_ویکټور_t = hb_ویکټور_t<Type, ریښتیا>;

#د_که_پای /* HB_VECTOR_HH */

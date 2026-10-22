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

#اگر_تعریف_نشده HB_وکتور_HH
#تعریف_کن HB_وکتور_HH

#ضمن "hb.hh"
#ضمن "hb-array.hh"
#ضمن "hb-meta.hh"
#ضمن "hb-null.hh"

// Change to 1 to force inline vector allocs, to see callsite in malloc-stats tool.
#إذا 0
#تعریف_کن HB_ALWAYS_INLINE_وکتور_ALLOCS HB_ALWAYS_INLINE
#وإلا
#تعریف_کن HB_ALWAYS_INLINE_وکتور_ALLOCS
#پایان_اگر

قالب <نام_نوع Type,
	  منطقي sorted=خطأ>
بنية hb_وکتور_t
{
  ساكن constexpr منطقي realloc_move = درست;

  تعريفنوع Type قلم_t;
  ساكن constexpr بیعلامت قلم_اندازه = hb_static_اندازه (Type);
  استفاده array_t = نام_نوع std::conditional<sorted, hb_sorted_array_t<Type>, hb_array_t<Type>>::type;
  استفاده c_array_t = نام_نوع std::conditional<sorted, hb_sorted_array_t<ثابت Type>, hb_array_t<ثابت Type>>::type;

  hb_وکتور_t () = افتراضي;
  HB_ALWAYS_INLINE_وکتور_ALLOCS
  hb_وکتور_t (std::initializer_list<Type> lst) : hb_وکتور_t ()
  {
    alloc (lst.اندازه (), درست);
    لكل (خودکار&& قلم : lst)
      push (قلم);
  }
  قالب <نام_نوع Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  صریح hb_وکتور_t (ثابت Iterable &o) : hb_وکتور_t ()
  {
    extend (o, درست);
  }
  HB_ALWAYS_INLINE_وکتور_ALLOCS
  hb_وکتور_t (ثابت hb_وکتور_t &o) : hb_وکتور_t ()
  {
    alloc_exact (o.درازی);
    إذا (unlikely (درون_error ())) أرجع;
    copy_array (o.as_array ());
  }
  HB_ALWAYS_INLINE_وکتور_ALLOCS
  hb_وکتور_t (array_t o) : hb_وکتور_t ()
  {
    alloc_exact (o.درازی);
    إذا (unlikely (درون_error ())) أرجع;
    copy_array (o);
  }
  HB_ALWAYS_INLINE_وکتور_ALLOCS
  hb_وکتور_t (c_array_t o) : hb_وکتور_t ()
  {
    alloc_exact (o.درازی);
    إذا (unlikely (درون_error ())) أرجع;
    copy_array (o);
  }
  hb_وکتور_t (hb_وکتور_t &&o) noexcept
  {
    allocated = o.allocated;
    درازی = o.درازی;
    arrayZ = o.arrayZ;
    o.init ();
  }
  ~hb_وکتور_t () { fini (); }

  قالب <بیعلامت n>
  فراغ
  مجموعه_storage (Type (&array)[n])
  { مجموعه_storage (array, n); }
  فراغ
  مجموعه_storage (hb_array_t<Type> array)
  { مجموعه_storage (array.arrayZ, array.درازی); }
  قالب <نام_نوع T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T) &&
			  hb_is_trivially_destructible(T))>
  فراغ
  مجموعه_storage (Type *array, بیعلامت n)
  {
    أكد (allocated == 0);
    أكد (درازی == 0);

    arrayZ = array;
    درازی = n;
  }

  قالب <نام_نوع Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  HB_ALWAYS_INLINE_وکتور_ALLOCS
  فراغ extend (ثابت Iterable &o, منطقي exact=خطأ)
  {
    خودکار iter = hb_iter (o);
    إذا (iter.is_random_access_iterator || iter.has_fast_len)
    {
      إذا (unlikely (!alloc (درازی + hb_len (iter), exact)))
	أرجع;
      بیعلامت شمار = hb_len (iter);
      لكل (بیعلامت i = 0; i < شمار; i++)
	افزودن_has_جا (*iter++);
    }
    طالما (iter)
    {
      إذا (unlikely (!alloc (درازی + 1)))
        أرجع;
      بیعلامت جا = allocated - درازی;
      لكل (بیعلامت i = 0; i < جا && iter; i++)
	افزودن_has_جا (*iter++);
    }
  }
  HB_ALWAYS_INLINE_وکتور_ALLOCS
  فراغ extend (array_t o, منطقي exact=خطأ)
  { extend (c_array_t (o), exact); }
  HB_ALWAYS_INLINE_وکتور_ALLOCS
  فراغ extend (c_array_t o, منطقي exact=خطأ)
  {
    /* o may point into our own storage, eg. v.extend (v.as_array ()).
     * alloc() may move that storage, so find o again afterwards. */
    منطقي aliased = o.درازی && درون_storage (*o.arrayZ);
    بیعلامت جابه_جایی = aliased ? o.arrayZ - arrayZ : 0;
    alloc (درازی + o.درازی, exact);
    إذا (unlikely (درون_error ())) أرجع;
    إذا (unlikely (aliased))
      o = {arrayZ + جابه_جایی, o.درازی};
    copy_array (o);
  }
  HB_ALWAYS_INLINE_وکتور_ALLOCS
  فراغ extend (ثابت hb_وکتور_t &o, منطقي exact=خطأ)
  { extend (o.as_array (), exact); }

  عام:
  صحيح allocated = 0; /* < 0 means allocation failed. */
  بیعلامت صحيح درازی = 0;
  عام:
  Type *arrayZ = اشاره_تهی;

  فراغ init ()
  {
    allocated = درازی = 0;
    arrayZ = اشاره_تهی;
  }
  فراغ init0 ()
  {
  }

  فراغ fini ()
  {
    إذا (is_owned ())
    {
      shrink_وکتور (0);
      hb_free (arrayZ);
    }
    init ();
  }

  HB_ALWAYS_INLINE_وکتور_ALLOCS
  hb_وکتور_t &reset ()
  {
    إذا (unlikely (درون_error ()))
      reset_error ();
    clear ();
    أرجع *هذا;
  }

  HB_ALWAYS_INLINE_وکتور_ALLOCS
  hb_وکتور_t &reset_if_error ()
  {
    إذا (unlikely (درون_error ()))
      reset ();
    أرجع *هذا;
  }

  /* Transfer ownership of the backing storage to caller.
   * Returns nullptr if storage is not owned by this vector. */
  Type *
  steal (بیعلامت *len = اشاره_تهی, صحيح *allocated_بیرون = اشاره_تهی)
  {
    إذا (!is_owned ())
      أرجع اشاره_تهی;
    إذا (len)
      *len = درازی;
    إذا (allocated_بیرون)
      *allocated_بیرون = allocated;
    Type *p = arrayZ;
    init ();
    أرجع p;
  }

  /* Adopt a previously detached owned buffer. */
  فراغ
  recycle_میانگیر (Type *میانگیر,
                  بیعلامت len,
                  صحيح allocated_len)
  {
    fini ();
    arrayZ = میانگیر;
    درازی = len;
    allocated = allocated_len;
  }

  دوست فراغ swap (hb_وکتور_t& a, hb_وکتور_t& b) noexcept
  {
    hb_جابجا (a.allocated, b.allocated);
    hb_جابجا (a.درازی, b.درازی);
    hb_جابجا (a.arrayZ, b.arrayZ);
  }

  hb_وکتور_t& عملگر = (ثابت hb_وکتور_t &o)
  {
    إذا (هذا == &o) أرجع *هذا;

    reset ();
    alloc_exact (o.درازی);
    إذا (unlikely (درون_error ())) أرجع *هذا;

    درازی = 0;
    copy_array (o.as_array ());

    أرجع *هذا;
  }
  hb_وکتور_t& عملگر = (hb_وکتور_t &&o) noexcept
  {
    hb_جابجا (*هذا, o);
    أرجع *هذا;
  }

  hb_بایتها_t as_بایتها () ثابت
  { أرجع hb_بایتها_t ((ثابت حرف *) arrayZ, get_اندازه ()); }

  منطقي عملگر == (ثابت hb_وکتور_t &o) ثابت { أرجع as_array () == o.as_array (); }
  منطقي عملگر != (ثابت hb_وکتور_t &o) ثابت { أرجع !(*هذا == o); }
  uint32_t hash () ثابت { أرجع as_array ().hash (); }

  Type& عملگر [] (صحيح i_)
  {
    بیعلامت صحيح i = (بیعلامت صحيح) i_;
    إذا (unlikely (i >= درازی))
      أرجع Crap (Type);
    أرجع arrayZ[i];
  }
  ثابت Type& عملگر [] (صحيح i_) ثابت
  {
    بیعلامت صحيح i = (بیعلامت صحيح) i_;
    إذا (unlikely (i >= درازی))
      أرجع Null (Type);
    أرجع arrayZ[i];
  }

  Type& دم () { أرجع (*هذا)[درازی - 1]; }
  ثابت Type& دم () ثابت { أرجع (*هذا)[درازی - 1]; }

  صریح عملگر منطقي () ثابت { أرجع درازی; }
  size_t get_اندازه () ثابت { أرجع hb_unsigned_mul_saturate (درازی, قلم_اندازه); }

  /* Sink interface. */
  قالب <نام_نوع T>
  hb_وکتور_t& عملگر << (T&& v) { push (std::forward<T> (v)); أرجع *هذا; }

  array_t   as_array ()       { أرجع hb_array (arrayZ, درازی); }
  c_array_t as_array () ثابت { أرجع hb_array (arrayZ, درازی); }

  /* Iterator. */
  تعريفنوع c_array_t   iter_t;
  تعريفنوع array_t   writer_t;
    iter_t   iter () ثابت { أرجع as_array (); }
  writer_t writer ()       { أرجع as_array (); }
  عملگر   iter_t () ثابت { أرجع   iter (); }
  عملگر writer_t ()       { أرجع writer (); }

  /* Faster range-based for loop. */
  Type *begin () ثابت { أرجع arrayZ; }
  Type *انتها () ثابت { أرجع arrayZ + درازی; }


  hb_sorted_array_t<Type> as_sorted_array ()
  { أرجع hb_sorted_array (arrayZ, درازی); }
  hb_sorted_array_t<ثابت Type> as_sorted_array () ثابت
  { أرجع hb_sorted_array (arrayZ, درازی); }

  قالب <نام_نوع T> صریح عملگر T * () { أرجع arrayZ; }
  قالب <نام_نوع T> صریح عملگر ثابت T * () ثابت { أرجع arrayZ; }

  Type * عملگر  + (بیعلامت صحيح i) { أرجع arrayZ + i; }
  ثابت Type * عملگر  + (بیعلامت صحيح i) ثابت { أرجع arrayZ + i; }

  HB_ALWAYS_INLINE_وکتور_ALLOCS
  Type *push ()
  {
    إذا (unlikely (!resize (درازی + 1)))
      أرجع std::addressof (Crap (Type));
    أرجع std::addressof (arrayZ[درازی - 1]);
  }
  قالب <نام_نوع... آرگومانها_2>
  HB_ALWAYS_INLINE_وکتور_ALLOCS
  Type *push (آرگومانها_2&&... آرگومانها)
  {
    إذا (unlikely ((صحيح) درازی >= allocated))
    {
      إذا (unlikely (درون_error ()))
	أرجع std::addressof (Crap (Type));

      /* An argument may live in our own storage, eg. v.push (v[i]).
       * alloc() frees that storage, so copy it out first. */
      إذا (unlikely (درون_storage (آرگومانها...)))
      {
	Type copy (std::forward<آرگومانها_2> (آرگومانها)...);
	إذا (unlikely (!alloc (درازی + 1)))
	  أرجع std::addressof (Crap (Type));
	أرجع افزودن_has_جا (std::move (copy));
      }
      إذا (unlikely (!alloc (درازی + 1)))
	// If push failed to allocate then don't copy v, since this may cause
	// the created copy to leak memory since we won't have stored a
	// reference to it.
	أرجع std::addressof (Crap (Type));
    }

    أرجع افزودن_has_جا (std::forward<آرگومانها_2> (آرگومانها)...);
  }
  قالب <نام_نوع... آرگومانها_2>
  HB_ALWAYS_INLINE_وکتور_ALLOCS
  منطقي افزودن_or_fail (آرگومانها_2&&... آرگومانها)
  {
    أرجع push (std::forward<آرگومانها_2> (آرگومانها)...) != std::addressof (Crap (Type));
  }
  قالب <نام_نوع... آرگومانها_2>
  HB_ALWAYS_INLINE_وکتور_ALLOCS
  Type *افزودن_has_جا (آرگومانها_2&&... آرگومانها)
  {
    /* Emplace. */
    Type *p = std::addressof (arrayZ[درازی++]);
    أرجع جديد (p) Type (std::forward<آرگومانها_2> (آرگومانها)...);
  }

  منطقي درون_storage () ثابت { أرجع خطأ; }
  قالب <نام_نوع T, نام_نوع... Ts>
  منطقي درون_storage (ثابت T &v, ثابت Ts &...vs) ثابت
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    أرجع (arrayZ &&
	    (uintptr_t) arrayZ <= p && p < (uintptr_t) (arrayZ + درازی)) ||
	   درون_storage (vs...);
  }

  منطقي is_owned () ثابت
  {
    أرجع allocated != 0 && allocated != -1;
  }

  منطقي درون_error () ثابت { أرجع allocated < 0; }
  فراغ مجموعه_error ()
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
    إذا (!درون_error ())
      مجموعه_error ();
  }

  Type *
  _realloc (بیعلامت new_allocated)
  {
    إذا (!new_allocated)
    {
      إذا (is_owned ())
	hb_free (arrayZ);
      أرجع اشاره_تهی;
    }
    إذا (!allocated && arrayZ)
    {
      /* If we have a non-null arrayZ but allocated is 0, then we are
       * reallocating from a foreign array. */
      Type *new_array = (Type *) hb_malloc (new_allocated * حجم (Type));
      إذا (unlikely (!new_array))
	أرجع اشاره_تهی;
      hb_memcpy ((فراغ *) new_array, (ثابت فراغ *) arrayZ, درازی * حجم (Type));
      أرجع new_array;
    }
    أرجع (Type *) hb_realloc (arrayZ, new_allocated * حجم (Type));
  }
  Type *
  _malloc_move (بیعلامت new_allocated)
  {
    إذا (!new_allocated)
    {
      إذا (is_owned ())
	hb_free (arrayZ);
      أرجع اشاره_تهی;
    }
    Type *new_array = (Type *) hb_malloc (new_allocated * حجم (Type));
    إذا (likely (new_array))
    {
      لكل (بیعلامت i = 0; i < درازی; i++)
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

  قالب <نام_نوع T = Type,
	    hb_enable_if (hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_وکتور (بیعلامت new_allocated, hb_اولویت<0>)
  {
    أرجع _realloc (new_allocated);
  }
  قالب <نام_نوع T = Type,
	    hb_enable_if (!hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_وکتور (بیعلامت new_allocated, hb_اولویت<0>)
  {
    أرجع _malloc_move (new_allocated);
  }
  /* Specialization for types that can be moved using realloc(). */
  قالب <نام_نوع T = Type,
	    hb_enable_if (T::realloc_move)>
  Type *
  realloc_وکتور (بیعلامت new_allocated, hb_اولویت<1>)
  {
    أرجع _realloc (new_allocated);
  }

  قالب <نام_نوع T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T))>
  فراغ
  grow_وکتور (بیعلامت اندازه, hb_اولویت<0>)
  {
    hb_memset (arrayZ + درازی, 0, (اندازه - درازی) * حجم (*arrayZ));
    درازی = اندازه;
  }
  قالب <نام_نوع T = Type,
	    hb_enable_if (!hb_is_trivially_constructible(T))>
  فراغ
  grow_وکتور (بیعلامت اندازه, hb_اولویت<0>)
  {
    لكل (; درازی < اندازه; درازی++)
      جديد (std::addressof (arrayZ[درازی])) Type ();
  }
  /* Specialization for hb_vector_t<hb_{vector,array}_t<U>> to speed up. */
  قالب <نام_نوع T = Type,
	    hb_enable_if (hb_is_same (T, hb_وکتور_t<نام_نوع T::قلم_t>) ||
			  hb_is_same (T, hb_array_t <نام_نوع T::قلم_t>))>
  فراغ
  grow_وکتور (بیعلامت اندازه, hb_اولویت<1>)
  {
    hb_memset (arrayZ + درازی, 0, (اندازه - درازی) * حجم (*arrayZ));
    درازی = اندازه;
  }

  قالب <نام_نوع T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  فراغ
  copy_array (hb_array_t<Type> other)
  {
    hb_memcpy ((فراغ *) (arrayZ + درازی), (ثابت فراغ *) other.arrayZ, other.درازی * قلم_اندازه);
    درازی += other.درازی;
  }
  قالب <نام_نوع T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  فراغ
  copy_array (hb_array_t<ثابت Type> other)
  {
    hb_memcpy ((فراغ *) (arrayZ + درازی), (ثابت فراغ *) other.arrayZ, other.درازی * قلم_اندازه);
    درازی += other.درازی;
  }
  قالب <نام_نوع T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			   std::is_copy_constructible<T>::value)>
  فراغ
  copy_array (hb_array_t<ثابت Type> other)
  {
    لكل (بیعلامت i = 0; i < other.درازی; i++)
      جديد (std::addressof (arrayZ[درازی + i])) Type (other.arrayZ[i]);
    درازی += other.درازی;
  }
  قالب <نام_نوع T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			  !std::is_copy_constructible<T>::value &&
			  std::is_default_constructible<T>::value &&
			  std::is_copy_assignable<T>::value)>
  فراغ
  copy_array (hb_array_t<ثابت Type> other)
  {
    لكل (بیعلامت i = 0; i < other.درازی; i++)
    {
      جديد (std::addressof (arrayZ[درازی + i])) Type ();
      arrayZ[درازی + i] = other.arrayZ[i];
    }
    درازی += other.درازی;
  }

  فراغ
  shrink_وکتور (بیعلامت اندازه)
  {
    أكد (اندازه <= درازی);
    إذا (!hb_is_trivially_destructible(Type))
    {
      بیعلامت شمار = درازی - اندازه;
      Type *p = arrayZ + درازی;
      طالما (شمار--)
        (--p)->~Type ();
    }
    درازی = اندازه;
  }

  فراغ
  لغزش_پایین_وکتور (بیعلامت i)
  {
    لكل (; i < درازی; i++)
      arrayZ[i - 1] = std::move (arrayZ[i]);
  }

  /* Allocate for size but don't adjust length. */
  HB_ALWAYS_INLINE_وکتور_ALLOCS
  منطقي alloc (بیعلامت صحيح اندازه, منطقي exact=خطأ)
  {
    إذا (unlikely (درون_error ()))
      أرجع خطأ;

    بیعلامت صحيح new_allocated;
    إذا (exact)
    {
      /* If exact was specified, we allow shrinking the storage. */
      اندازه = hb_max (اندازه, درازی);
      إذا (اندازه <= (بیعلامت) allocated &&
	  اندازه >= (بیعلامت) allocated >> 2)
	أرجع درست;

      new_allocated = اندازه;
    }
    وإلا
    {
      إذا (likely (اندازه <= (بیعلامت) allocated))
	أرجع درست;

      new_allocated = allocated;
      طالما (اندازه > new_allocated)
	new_allocated += (new_allocated >> 1) + 8;
    }

    /* Reallocate */

    منطقي overflows =
      (صحيح) درون_error () ||
      (new_allocated < اندازه) ||
      hb_unsigned_mul_overflows (new_allocated, حجم (Type));

    إذا (unlikely (overflows))
    {
      مجموعه_error ();
      أرجع خطأ;
    }

    Type *new_array = realloc_وکتور (new_allocated, hb_prioritize);

    إذا (unlikely (new_allocated && !new_array))
    {
      إذا (new_allocated <= (بیعلامت) allocated)
        أرجع درست; // shrinking failed; it's okay; happens in our fuzzer

      مجموعه_error ();
      أرجع خطأ;
    }

    arrayZ = new_array;
    allocated = new_allocated;

    أرجع درست;
  }
  HB_ALWAYS_INLINE_وکتور_ALLOCS
  منطقي alloc_exact (بیعلامت صحيح اندازه)
  {
    أرجع alloc (اندازه, درست);
  }

  HB_ALWAYS_INLINE_وکتور_ALLOCS
  فراغ clear ()
  {
    shrink_وکتور (0);
  }

  قالب <نام_نوع allocator_t>
  HB_ALWAYS_INLINE_وکتور_ALLOCS
  منطقي allocate_from_pool (allocator_t *allocator, بیعلامت اندازه, بیعلامت صحيح initialize = درست)
  {
    إذا (allocator)
    {
      أكد (!درازی && !allocated);
      arrayZ = (Type *) allocator->alloc (اندازه * حجم (Type), alignof (Type));
      إذا (unlikely (!arrayZ))
      {
	مجموعه_error ();
	أرجع خطأ;
      }
      إذا (initialize)
	grow_وکتور (اندازه, hb_prioritize);
      وإلا
	درازی = اندازه;
      أرجع درست;
    }
    أرجع resize_پر ((صحيح) اندازه, initialize, درست);
  }

  قالب <نام_نوع allocator_t>
  HB_ALWAYS_INLINE_وکتور_ALLOCS
  منطقي duplicate_وکتور_from_pool (allocator_t *allocator, ثابت hb_وکتور_t &other)
  {
    إذا (unlikely (!allocate_from_pool (allocator, other.درازی, خطأ)))
      أرجع خطأ;
    درازی = 0;
    copy_array (other.as_array ());
    أرجع درست;
  }

  قالب <نام_نوع allocator_t>
  فراغ shrink_پشت_مقصد_pool (allocator_t *allocator, صحيح اندازه)
  {
    بیعلامت orig_درازی = درازی;

    shrink (اندازه, خطأ);

    إذا (allocator && !is_owned ())
      allocator->discard (arrayZ + درازی, (orig_درازی - درازی) * حجم (Type));
  }

  HB_ALWAYS_INLINE_وکتور_ALLOCS
  منطقي resize_پر (صحيح اندازه_3, منطقي initialize, منطقي exact)
  {
    إذا (unlikely (اندازه_3 < 0))
      أرجع خطأ;
    بیعلامت صحيح اندازه = (بیعلامت صحيح) اندازه_3;
    إذا (!alloc (اندازه, exact))
      أرجع خطأ;

    إذا (اندازه > درازی)
    {
      إذا (initialize)
	grow_وکتور (اندازه, hb_prioritize);
    }
    وإلا إذا (اندازه < درازی)
    {
      إذا (initialize)
	shrink_وکتور (اندازه);
    }

    درازی = اندازه;
    أرجع درست;
  }
  HB_ALWAYS_INLINE_وکتور_ALLOCS
  منطقي resize (صحيح اندازه_3)
  {
    أرجع resize_پر (اندازه_3, درست, خطأ);
  }
  HB_ALWAYS_INLINE_وکتور_ALLOCS
  منطقي resize_dirty (صحيح اندازه_3)
  {
    أرجع resize_پر (اندازه_3, خطأ, خطأ);
  }
  HB_ALWAYS_INLINE_وکتور_ALLOCS
  منطقي resize_exact (صحيح اندازه_3)
  {
    أرجع resize_پر (اندازه_3, درست, درست);
  }

  Type برداشت ()
  {
    إذا (!درازی) أرجع Null (Type);
    Type v (std::move (arrayZ[درازی - 1]));
    arrayZ[درازی - 1].~Type ();
    درازی--;
    أرجع v;
  }

  فراغ remove_ordered (بیعلامت صحيح i)
  {
    إذا (unlikely (i >= درازی))
      أرجع;
    لغزش_پایین_وکتور (i + 1);
    arrayZ[درازی - 1].~Type ();
    درازی--;
  }

  قالب <منطقي Sorted = sorted,
	    hb_enable_if (!Sorted)>
  فراغ remove_unordered (بیعلامت صحيح i)
  {
    إذا (unlikely (i >= درازی))
      أرجع;
    إذا (i != درازی - 1)
      arrayZ[i] = std::move (arrayZ[درازی - 1]);
    arrayZ[درازی - 1].~Type ();
    درازی--;
  }

  فراغ shrink (صحيح اندازه_3, منطقي shrink_حافظه = درست)
  {
    بیعلامت صحيح اندازه = اندازه_3 < 0 ? 0u : (بیعلامت صحيح) اندازه_3;
    إذا (اندازه >= درازی)
      أرجع;

    shrink_وکتور (اندازه);

    إذا (is_owned () && shrink_حافظه)
      alloc_exact (اندازه); /* To force shrinking memory if needed. */
  }


  /* Sorting API. */
  قالب <نام_نوع Compar>
  فراغ qsort (Compar compar)
  { as_array ().qsort (compar); }
  فراغ qsort ()
  { as_array ().qsort (); }

  /* Unsorted search API. */
  قالب <نام_نوع T>
  Type *lsearch (ثابت T &x, Type *not_یافت = اشاره_تهی)
  { أرجع as_array ().lsearch (x, not_یافت); }
  قالب <نام_نوع T>
  ثابت Type *lsearch (ثابت T &x, ثابت Type *not_یافت = اشاره_تهی) ثابت
  { أرجع as_array ().lsearch (x, not_یافت); }
  قالب <نام_نوع T>
  منطقي lfind (ثابت T &x, بیعلامت *pos = اشاره_تهی) ثابت
  { أرجع as_array ().lfind (x, pos); }

  /* Sorted search API. */
  قالب <نام_نوع T,
	    منطقي Sorted=sorted, hb_enable_if (Sorted)>
  Type *bsearch (ثابت T &x, Type *not_یافت = اشاره_تهی)
  { أرجع as_array ().bsearch (x, not_یافت); }
  قالب <نام_نوع T,
	    منطقي Sorted=sorted, hb_enable_if (Sorted)>
  ثابت Type *bsearch (ثابت T &x, ثابت Type *not_یافت = اشاره_تهی) ثابت
  { أرجع as_array ().bsearch (x, not_یافت); }
  قالب <نام_نوع T,
	    منطقي Sorted=sorted, hb_enable_if (Sorted)>
  منطقي bfind (ثابت T &x, بیعلامت صحيح *i = اشاره_تهی,
	      hb_not_یافت_t not_یافت = HB_NOT_یافت_DONT_STORE,
	      بیعلامت صحيح مقصد_store = (بیعلامت صحيح) -1) ثابت
  { أرجع as_array ().bfind (x, i, not_یافت, مقصد_store); }
};

قالب <نام_نوع Type>
استفاده hb_sorted_وکتور_t = hb_وکتور_t<Type, درست>;

#پایان_اگر /* HB_VECTOR_HH */

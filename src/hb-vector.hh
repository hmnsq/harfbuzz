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

#اگر_غیر_تعریف_شدہ HB_سمتیہ_HH
#تعریف_کرو HB_سمتیہ_HH

#شامل "hb.hh"
#شامل "hb-array.hh"
#شامل "hb-meta.hh"
#شامل "hb-null.hh"

// Change to 1 to force inline vector allocs, to see callsite in malloc-stats tool.
#اگر 0
#تعریف_کرو HB_ALWAYS_INLINE_سمتیہ_ALLOCS HB_ALWAYS_INLINE
#ورنہ
#تعریف_کرو HB_ALWAYS_INLINE_سمتیہ_ALLOCS
#اگر_ختم

سانچہ <قسم_نام Type,
	  منطقی sorted=جھوٹ>
ساخت hb_سمتیہ_t
{
  ساکن constexpr منطقی realloc_move = سچ;

  قسمتعریف Type شے_t;
  ساکن constexpr غیرعلامتی شے_سائز = hb_static_سائز (Type);
  استعمال array_t = قسم_نام std::conditional<sorted, hb_sorted_array_t<Type>, hb_array_t<Type>>::type;
  استعمال c_array_t = قسم_نام std::conditional<sorted, hb_sorted_array_t<ثابت Type>, hb_array_t<ثابت Type>>::type;

  hb_سمتیہ_t () = طےشدہ;
  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  hb_سمتیہ_t (std::initializer_list<Type> lst) : hb_سمتیہ_t ()
  {
    alloc (lst.سائز (), سچ);
    برائے (خودکار&& شے : lst)
      push (شے);
  }
  سانچہ <قسم_نام Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  صریح hb_سمتیہ_t (ثابت Iterable &o) : hb_سمتیہ_t ()
  {
    extend (o, سچ);
  }
  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  hb_سمتیہ_t (ثابت hb_سمتیہ_t &o) : hb_سمتیہ_t ()
  {
    alloc_exact (o.لمبائی);
    اگر (unlikely (اندر_error ())) واپس;
    copy_array (o.as_array ());
  }
  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  hb_سمتیہ_t (array_t o) : hb_سمتیہ_t ()
  {
    alloc_exact (o.لمبائی);
    اگر (unlikely (اندر_error ())) واپس;
    copy_array (o);
  }
  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  hb_سمتیہ_t (c_array_t o) : hb_سمتیہ_t ()
  {
    alloc_exact (o.لمبائی);
    اگر (unlikely (اندر_error ())) واپس;
    copy_array (o);
  }
  hb_سمتیہ_t (hb_سمتیہ_t &&o) noexcept
  {
    allocated = o.allocated;
    لمبائی = o.لمبائی;
    arrayZ = o.arrayZ;
    o.init ();
  }
  ~hb_سمتیہ_t () { fini (); }

  سانچہ <غیرعلامتی n>
  خالی
  سیٹ_storage (Type (&array)[n])
  { سیٹ_storage (array, n); }
  خالی
  سیٹ_storage (hb_array_t<Type> array)
  { سیٹ_storage (array.arrayZ, array.لمبائی); }
  سانچہ <قسم_نام T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T) &&
			  hb_is_trivially_destructible(T))>
  خالی
  سیٹ_storage (Type *array, غیرعلامتی n)
  {
    أكد (allocated == 0);
    أكد (لمبائی == 0);

    arrayZ = array;
    لمبائی = n;
  }

  سانچہ <قسم_نام Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  خالی extend (ثابت Iterable &o, منطقی exact=جھوٹ)
  {
    خودکار iter = hb_iter (o);
    اگر (iter.is_random_access_iterator || iter.has_fast_len)
    {
      اگر (unlikely (!alloc (لمبائی + hb_len (iter), exact)))
	واپس;
      غیرعلامتی گنتی = hb_len (iter);
      برائے (غیرعلامتی i = 0; i < گنتی; i++)
	ڈالو_has_گنجائش (*iter++);
    }
    جب (iter)
    {
      اگر (unlikely (!alloc (لمبائی + 1)))
        واپس;
      غیرعلامتی گنجائش = allocated - لمبائی;
      برائے (غیرعلامتی i = 0; i < گنجائش && iter; i++)
	ڈالو_has_گنجائش (*iter++);
    }
  }
  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  خالی extend (array_t o, منطقی exact=جھوٹ)
  { extend (c_array_t (o), exact); }
  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  خالی extend (c_array_t o, منطقی exact=جھوٹ)
  {
    /* o may point into our own storage, eg. v.extend (v.as_array ()).
     * alloc() may move that storage, so find o again afterwards. */
    منطقی aliased = o.لمبائی && اندر_storage (*o.arrayZ);
    غیرعلامتی انحراف = aliased ? o.arrayZ - arrayZ : 0;
    alloc (لمبائی + o.لمبائی, exact);
    اگر (unlikely (اندر_error ())) واپس;
    اگر (unlikely (aliased))
      o = {arrayZ + انحراف, o.لمبائی};
    copy_array (o);
  }
  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  خالی extend (ثابت hb_سمتیہ_t &o, منطقی exact=جھوٹ)
  { extend (o.as_array (), exact); }

  عوامی:
  صحیح allocated = 0; /* < 0 means allocation failed. */
  غیرعلامتی صحیح لمبائی = 0;
  عوامی:
  Type *arrayZ = خالی_اشاریہ;

  خالی init ()
  {
    allocated = لمبائی = 0;
    arrayZ = خالی_اشاریہ;
  }
  خالی init0 ()
  {
  }

  خالی fini ()
  {
    اگر (is_owned ())
    {
      shrink_سمتیہ (0);
      hb_free (arrayZ);
    }
    init ();
  }

  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  hb_سمتیہ_t &reset ()
  {
    اگر (unlikely (اندر_error ()))
      reset_error ();
    clear ();
    واپس *یہ;
  }

  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  hb_سمتیہ_t &reset_if_error ()
  {
    اگر (unlikely (اندر_error ()))
      reset ();
    واپس *یہ;
  }

  /* Transfer ownership of the backing storage to caller.
   * Returns nullptr if storage is not owned by this vector. */
  Type *
  steal (غیرعلامتی *len = خالی_اشاریہ, صحیح *allocated_برآمد = خالی_اشاریہ)
  {
    اگر (!is_owned ())
      واپس خالی_اشاریہ;
    اگر (len)
      *len = لمبائی;
    اگر (allocated_برآمد)
      *allocated_برآمد = allocated;
    Type *p = arrayZ;
    init ();
    واپس p;
  }

  /* Adopt a previously detached owned buffer. */
  خالی
  recycle_بفر (Type *بفر,
                  غیرعلامتی len,
                  صحیح allocated_len)
  {
    fini ();
    arrayZ = بفر;
    لمبائی = len;
    allocated = allocated_len;
  }

  دوست خالی swap (hb_سمتیہ_t& a, hb_سمتیہ_t& b) noexcept
  {
    hb_تبادلہ (a.allocated, b.allocated);
    hb_تبادلہ (a.لمبائی, b.لمبائی);
    hb_تبادلہ (a.arrayZ, b.arrayZ);
  }

  hb_سمتیہ_t& عامل = (ثابت hb_سمتیہ_t &o)
  {
    اگر (یہ == &o) واپس *یہ;

    reset ();
    alloc_exact (o.لمبائی);
    اگر (unlikely (اندر_error ())) واپس *یہ;

    لمبائی = 0;
    copy_array (o.as_array ());

    واپس *یہ;
  }
  hb_سمتیہ_t& عامل = (hb_سمتیہ_t &&o) noexcept
  {
    hb_تبادلہ (*یہ, o);
    واپس *یہ;
  }

  hb_بائٹیں_t as_بائٹیں () ثابت
  { واپس hb_بائٹیں_t ((ثابت حرف *) arrayZ, get_سائز ()); }

  منطقی عامل == (ثابت hb_سمتیہ_t &o) ثابت { واپس as_array () == o.as_array (); }
  منطقی عامل != (ثابت hb_سمتیہ_t &o) ثابت { واپس !(*یہ == o); }
  uint32_t hash () ثابت { واپس as_array ().hash (); }

  Type& عامل [] (صحیح i_)
  {
    غیرعلامتی صحیح i = (غیرعلامتی صحیح) i_;
    اگر (unlikely (i >= لمبائی))
      واپس Crap (Type);
    واپس arrayZ[i];
  }
  ثابت Type& عامل [] (صحیح i_) ثابت
  {
    غیرعلامتی صحیح i = (غیرعلامتی صحیح) i_;
    اگر (unlikely (i >= لمبائی))
      واپس Null (Type);
    واپس arrayZ[i];
  }

  Type& دم () { واپس (*یہ)[لمبائی - 1]; }
  ثابت Type& دم () ثابت { واپس (*یہ)[لمبائی - 1]; }

  صریح عامل منطقی () ثابت { واپس لمبائی; }
  size_t get_سائز () ثابت { واپس hb_unsigned_mul_saturate (لمبائی, شے_سائز); }

  /* Sink interface. */
  سانچہ <قسم_نام T>
  hb_سمتیہ_t& عامل << (T&& v) { push (std::forward<T> (v)); واپس *یہ; }

  array_t   as_array ()       { واپس hb_array (arrayZ, لمبائی); }
  c_array_t as_array () ثابت { واپس hb_array (arrayZ, لمبائی); }

  /* Iterator. */
  قسمتعریف c_array_t   iter_t;
  قسمتعریف array_t   writer_t;
    iter_t   iter () ثابت { واپس as_array (); }
  writer_t writer ()       { واپس as_array (); }
  عامل   iter_t () ثابت { واپس   iter (); }
  عامل writer_t ()       { واپس writer (); }

  /* Faster range-based for loop. */
  Type *begin () ثابت { واپس arrayZ; }
  Type *انتہا () ثابت { واپس arrayZ + لمبائی; }


  hb_sorted_array_t<Type> as_sorted_array ()
  { واپس hb_sorted_array (arrayZ, لمبائی); }
  hb_sorted_array_t<ثابت Type> as_sorted_array () ثابت
  { واپس hb_sorted_array (arrayZ, لمبائی); }

  سانچہ <قسم_نام T> صریح عامل T * () { واپس arrayZ; }
  سانچہ <قسم_نام T> صریح عامل ثابت T * () ثابت { واپس arrayZ; }

  Type * عامل  + (غیرعلامتی صحیح i) { واپس arrayZ + i; }
  ثابت Type * عامل  + (غیرعلامتی صحیح i) ثابت { واپس arrayZ + i; }

  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  Type *push ()
  {
    اگر (unlikely (!resize (لمبائی + 1)))
      واپس std::addressof (Crap (Type));
    واپس std::addressof (arrayZ[لمبائی - 1]);
  }
  سانچہ <قسم_نام... دلائل_2>
  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  Type *push (دلائل_2&&... دلائل)
  {
    اگر (unlikely ((صحیح) لمبائی >= allocated))
    {
      اگر (unlikely (اندر_error ()))
	واپس std::addressof (Crap (Type));

      /* An argument may live in our own storage, eg. v.push (v[i]).
       * alloc() frees that storage, so copy it out first. */
      اگر (unlikely (اندر_storage (دلائل...)))
      {
	Type copy (std::forward<دلائل_2> (دلائل)...);
	اگر (unlikely (!alloc (لمبائی + 1)))
	  واپس std::addressof (Crap (Type));
	واپس ڈالو_has_گنجائش (std::move (copy));
      }
      اگر (unlikely (!alloc (لمبائی + 1)))
	// If push failed to allocate then don't copy v, since this may cause
	// the created copy to leak memory since we won't have stored a
	// reference to it.
	واپس std::addressof (Crap (Type));
    }

    واپس ڈالو_has_گنجائش (std::forward<دلائل_2> (دلائل)...);
  }
  سانچہ <قسم_نام... دلائل_2>
  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  منطقی ڈالو_or_fail (دلائل_2&&... دلائل)
  {
    واپس push (std::forward<دلائل_2> (دلائل)...) != std::addressof (Crap (Type));
  }
  سانچہ <قسم_نام... دلائل_2>
  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  Type *ڈالو_has_گنجائش (دلائل_2&&... دلائل)
  {
    /* Emplace. */
    Type *p = std::addressof (arrayZ[لمبائی++]);
    واپس نیا (p) Type (std::forward<دلائل_2> (دلائل)...);
  }

  منطقی اندر_storage () ثابت { واپس جھوٹ; }
  سانچہ <قسم_نام T, قسم_نام... Ts>
  منطقی اندر_storage (ثابت T &v, ثابت Ts &...vs) ثابت
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    واپس (arrayZ &&
	    (uintptr_t) arrayZ <= p && p < (uintptr_t) (arrayZ + لمبائی)) ||
	   اندر_storage (vs...);
  }

  منطقی is_owned () ثابت
  {
    واپس allocated != 0 && allocated != -1;
  }

  منطقی اندر_error () ثابت { واپس allocated < 0; }
  خالی سیٹ_error ()
  {
    أكد (allocated >= 0);
    allocated = -allocated - 1;
  }
  خالی reset_error ()
  {
    أكد (allocated < 0);
    allocated = -(allocated + 1);
  }
  خالی ensure_error ()
  {
    اگر (!اندر_error ())
      سیٹ_error ();
  }

  Type *
  _realloc (غیرعلامتی new_allocated)
  {
    اگر (!new_allocated)
    {
      اگر (is_owned ())
	hb_free (arrayZ);
      واپس خالی_اشاریہ;
    }
    اگر (!allocated && arrayZ)
    {
      /* If we have a non-null arrayZ but allocated is 0, then we are
       * reallocating from a foreign array. */
      Type *new_array = (Type *) hb_malloc (new_allocated * حجم (Type));
      اگر (unlikely (!new_array))
	واپس خالی_اشاریہ;
      hb_memcpy ((خالی *) new_array, (ثابت خالی *) arrayZ, لمبائی * حجم (Type));
      واپس new_array;
    }
    واپس (Type *) hb_realloc (arrayZ, new_allocated * حجم (Type));
  }
  Type *
  _malloc_move (غیرعلامتی new_allocated)
  {
    اگر (!new_allocated)
    {
      اگر (is_owned ())
	hb_free (arrayZ);
      واپس خالی_اشاریہ;
    }
    Type *new_array = (Type *) hb_malloc (new_allocated * حجم (Type));
    اگر (likely (new_array))
    {
      برائے (غیرعلامتی i = 0; i < لمبائی; i++)
      {
	نیا (std::addressof (new_array[i])) Type ();
	new_array[i] = std::move (arrayZ[i]);
	arrayZ[i].~Type ();
      }
      اگر (is_owned ())
	hb_free (arrayZ);
    }
    واپس new_array;
  }

  سانچہ <قسم_نام T = Type,
	    hb_enable_if (hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_سمتیہ (غیرعلامتی new_allocated, hb_ترجیح<0>)
  {
    واپس _realloc (new_allocated);
  }
  سانچہ <قسم_نام T = Type,
	    hb_enable_if (!hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_سمتیہ (غیرعلامتی new_allocated, hb_ترجیح<0>)
  {
    واپس _malloc_move (new_allocated);
  }
  /* Specialization for types that can be moved using realloc(). */
  سانچہ <قسم_نام T = Type,
	    hb_enable_if (T::realloc_move)>
  Type *
  realloc_سمتیہ (غیرعلامتی new_allocated, hb_ترجیح<1>)
  {
    واپس _realloc (new_allocated);
  }

  سانچہ <قسم_نام T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T))>
  خالی
  grow_سمتیہ (غیرعلامتی سائز, hb_ترجیح<0>)
  {
    hb_memset (arrayZ + لمبائی, 0, (سائز - لمبائی) * حجم (*arrayZ));
    لمبائی = سائز;
  }
  سانچہ <قسم_نام T = Type,
	    hb_enable_if (!hb_is_trivially_constructible(T))>
  خالی
  grow_سمتیہ (غیرعلامتی سائز, hb_ترجیح<0>)
  {
    برائے (; لمبائی < سائز; لمبائی++)
      نیا (std::addressof (arrayZ[لمبائی])) Type ();
  }
  /* Specialization for hb_vector_t<hb_{vector,array}_t<U>> to speed up. */
  سانچہ <قسم_نام T = Type,
	    hb_enable_if (hb_is_same (T, hb_سمتیہ_t<قسم_نام T::شے_t>) ||
			  hb_is_same (T, hb_array_t <قسم_نام T::شے_t>))>
  خالی
  grow_سمتیہ (غیرعلامتی سائز, hb_ترجیح<1>)
  {
    hb_memset (arrayZ + لمبائی, 0, (سائز - لمبائی) * حجم (*arrayZ));
    لمبائی = سائز;
  }

  سانچہ <قسم_نام T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  خالی
  copy_array (hb_array_t<Type> other)
  {
    hb_memcpy ((خالی *) (arrayZ + لمبائی), (ثابت خالی *) other.arrayZ, other.لمبائی * شے_سائز);
    لمبائی += other.لمبائی;
  }
  سانچہ <قسم_نام T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  خالی
  copy_array (hb_array_t<ثابت Type> other)
  {
    hb_memcpy ((خالی *) (arrayZ + لمبائی), (ثابت خالی *) other.arrayZ, other.لمبائی * شے_سائز);
    لمبائی += other.لمبائی;
  }
  سانچہ <قسم_نام T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			   std::is_copy_constructible<T>::value)>
  خالی
  copy_array (hb_array_t<ثابت Type> other)
  {
    برائے (غیرعلامتی i = 0; i < other.لمبائی; i++)
      نیا (std::addressof (arrayZ[لمبائی + i])) Type (other.arrayZ[i]);
    لمبائی += other.لمبائی;
  }
  سانچہ <قسم_نام T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			  !std::is_copy_constructible<T>::value &&
			  std::is_default_constructible<T>::value &&
			  std::is_copy_assignable<T>::value)>
  خالی
  copy_array (hb_array_t<ثابت Type> other)
  {
    برائے (غیرعلامتی i = 0; i < other.لمبائی; i++)
    {
      نیا (std::addressof (arrayZ[لمبائی + i])) Type ();
      arrayZ[لمبائی + i] = other.arrayZ[i];
    }
    لمبائی += other.لمبائی;
  }

  خالی
  shrink_سمتیہ (غیرعلامتی سائز)
  {
    أكد (سائز <= لمبائی);
    اگر (!hb_is_trivially_destructible(Type))
    {
      غیرعلامتی گنتی = لمبائی - سائز;
      Type *p = arrayZ + لمبائی;
      جب (گنتی--)
        (--p)->~Type ();
    }
    لمبائی = سائز;
  }

  خالی
  سرکاؤ_نیچے_کو_سمتیہ (غیرعلامتی i)
  {
    برائے (; i < لمبائی; i++)
      arrayZ[i - 1] = std::move (arrayZ[i]);
  }

  /* Allocate for size but don't adjust length. */
  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  منطقی alloc (غیرعلامتی صحیح سائز, منطقی exact=جھوٹ)
  {
    اگر (unlikely (اندر_error ()))
      واپس جھوٹ;

    غیرعلامتی صحیح new_allocated;
    اگر (exact)
    {
      /* If exact was specified, we allow shrinking the storage. */
      سائز = hb_max (سائز, لمبائی);
      اگر (سائز <= (غیرعلامتی) allocated &&
	  سائز >= (غیرعلامتی) allocated >> 2)
	واپس سچ;

      new_allocated = سائز;
    }
    ورنہ
    {
      اگر (likely (سائز <= (غیرعلامتی) allocated))
	واپس سچ;

      new_allocated = allocated;
      جب (سائز > new_allocated)
	new_allocated += (new_allocated >> 1) + 8;
    }

    /* Reallocate */

    منطقی overflows =
      (صحیح) اندر_error () ||
      (new_allocated < سائز) ||
      hb_unsigned_mul_overflows (new_allocated, حجم (Type));

    اگر (unlikely (overflows))
    {
      سیٹ_error ();
      واپس جھوٹ;
    }

    Type *new_array = realloc_سمتیہ (new_allocated, hb_prioritize);

    اگر (unlikely (new_allocated && !new_array))
    {
      اگر (new_allocated <= (غیرعلامتی) allocated)
        واپس سچ; // shrinking failed; it's okay; happens in our fuzzer

      سیٹ_error ();
      واپس جھوٹ;
    }

    arrayZ = new_array;
    allocated = new_allocated;

    واپس سچ;
  }
  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  منطقی alloc_exact (غیرعلامتی صحیح سائز)
  {
    واپس alloc (سائز, سچ);
  }

  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  خالی clear ()
  {
    shrink_سمتیہ (0);
  }

  سانچہ <قسم_نام allocator_t>
  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  منطقی allocate_from_pool (allocator_t *allocator, غیرعلامتی سائز, غیرعلامتی صحیح initialize = سچ)
  {
    اگر (allocator)
    {
      أكد (!لمبائی && !allocated);
      arrayZ = (Type *) allocator->alloc (سائز * حجم (Type), alignof (Type));
      اگر (unlikely (!arrayZ))
      {
	سیٹ_error ();
	واپس جھوٹ;
      }
      اگر (initialize)
	grow_سمتیہ (سائز, hb_prioritize);
      ورنہ
	لمبائی = سائز;
      واپس سچ;
    }
    واپس resize_بھرا ((صحیح) سائز, initialize, سچ);
  }

  سانچہ <قسم_نام allocator_t>
  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  منطقی duplicate_سمتیہ_from_pool (allocator_t *allocator, ثابت hb_سمتیہ_t &other)
  {
    اگر (unlikely (!allocate_from_pool (allocator, other.لمبائی, جھوٹ)))
      واپس جھوٹ;
    لمبائی = 0;
    copy_array (other.as_array ());
    واپس سچ;
  }

  سانچہ <قسم_نام allocator_t>
  خالی shrink_عقب_مقصد_pool (allocator_t *allocator, صحیح سائز)
  {
    غیرعلامتی orig_لمبائی = لمبائی;

    shrink (سائز, جھوٹ);

    اگر (allocator && !is_owned ())
      allocator->discard (arrayZ + لمبائی, (orig_لمبائی - لمبائی) * حجم (Type));
  }

  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  منطقی resize_بھرا (صحیح سائز_3, منطقی initialize, منطقی exact)
  {
    اگر (unlikely (سائز_3 < 0))
      واپس جھوٹ;
    غیرعلامتی صحیح سائز = (غیرعلامتی صحیح) سائز_3;
    اگر (!alloc (سائز, exact))
      واپس جھوٹ;

    اگر (سائز > لمبائی)
    {
      اگر (initialize)
	grow_سمتیہ (سائز, hb_prioritize);
    }
    ورنہ اگر (سائز < لمبائی)
    {
      اگر (initialize)
	shrink_سمتیہ (سائز);
    }

    لمبائی = سائز;
    واپس سچ;
  }
  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  منطقی resize (صحیح سائز_3)
  {
    واپس resize_بھرا (سائز_3, سچ, جھوٹ);
  }
  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  منطقی resize_dirty (صحیح سائز_3)
  {
    واپس resize_بھرا (سائز_3, جھوٹ, جھوٹ);
  }
  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  منطقی resize_exact (صحیح سائز_3)
  {
    واپس resize_بھرا (سائز_3, سچ, سچ);
  }

  Type نکالو ()
  {
    اگر (!لمبائی) واپس Null (Type);
    Type v (std::move (arrayZ[لمبائی - 1]));
    arrayZ[لمبائی - 1].~Type ();
    لمبائی--;
    واپس v;
  }

  خالی remove_ordered (غیرعلامتی صحیح i)
  {
    اگر (unlikely (i >= لمبائی))
      واپس;
    سرکاؤ_نیچے_کو_سمتیہ (i + 1);
    arrayZ[لمبائی - 1].~Type ();
    لمبائی--;
  }

  سانچہ <منطقی Sorted = sorted,
	    hb_enable_if (!Sorted)>
  خالی remove_unordered (غیرعلامتی صحیح i)
  {
    اگر (unlikely (i >= لمبائی))
      واپس;
    اگر (i != لمبائی - 1)
      arrayZ[i] = std::move (arrayZ[لمبائی - 1]);
    arrayZ[لمبائی - 1].~Type ();
    لمبائی--;
  }

  خالی shrink (صحیح سائز_3, منطقی shrink_حافظہ = سچ)
  {
    غیرعلامتی صحیح سائز = سائز_3 < 0 ? 0u : (غیرعلامتی صحیح) سائز_3;
    اگر (سائز >= لمبائی)
      واپس;

    shrink_سمتیہ (سائز);

    اگر (is_owned () && shrink_حافظہ)
      alloc_exact (سائز); /* To force shrinking memory if needed. */
  }


  /* Sorting API. */
  سانچہ <قسم_نام Compar>
  خالی qsort (Compar compar)
  { as_array ().qsort (compar); }
  خالی qsort ()
  { as_array ().qsort (); }

  /* Unsorted search API. */
  سانچہ <قسم_نام T>
  Type *lsearch (ثابت T &x, Type *not_ملا = خالی_اشاریہ)
  { واپس as_array ().lsearch (x, not_ملا); }
  سانچہ <قسم_نام T>
  ثابت Type *lsearch (ثابت T &x, ثابت Type *not_ملا = خالی_اشاریہ) ثابت
  { واپس as_array ().lsearch (x, not_ملا); }
  سانچہ <قسم_نام T>
  منطقی lfind (ثابت T &x, غیرعلامتی *pos = خالی_اشاریہ) ثابت
  { واپس as_array ().lfind (x, pos); }

  /* Sorted search API. */
  سانچہ <قسم_نام T,
	    منطقی Sorted=sorted, hb_enable_if (Sorted)>
  Type *bsearch (ثابت T &x, Type *not_ملا = خالی_اشاریہ)
  { واپس as_array ().bsearch (x, not_ملا); }
  سانچہ <قسم_نام T,
	    منطقی Sorted=sorted, hb_enable_if (Sorted)>
  ثابت Type *bsearch (ثابت T &x, ثابت Type *not_ملا = خالی_اشاریہ) ثابت
  { واپس as_array ().bsearch (x, not_ملا); }
  سانچہ <قسم_نام T,
	    منطقی Sorted=sorted, hb_enable_if (Sorted)>
  منطقی bfind (ثابت T &x, غیرعلامتی صحیح *i = خالی_اشاریہ,
	      hb_not_ملا_t not_ملا = HB_NOT_ملا_DONT_STORE,
	      غیرعلامتی صحیح مقصد_store = (غیرعلامتی صحیح) -1) ثابت
  { واپس as_array ().bfind (x, i, not_ملا, مقصد_store); }
};

سانچہ <قسم_نام Type>
استعمال hb_sorted_سمتیہ_t = hb_سمتیہ_t<Type, سچ>;

#اگر_ختم /* HB_VECTOR_HH */

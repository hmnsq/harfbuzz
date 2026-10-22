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

#جے_بنا_تعریف HB_سمتیہ_HH
#تعریف_کرو HB_سمتیہ_HH

#ضمن "hb.hh"
#ضمن "hb-array.hh"
#ضمن "hb-meta.hh"
#ضمن "hb-null.hh"

// Change to 1 to force inline vector allocs, to see callsite in malloc-stats tool.
#إذا 0
#تعریف_کرو HB_ALWAYS_INLINE_سمتیہ_ALLOCS HB_ALWAYS_INLINE
#وإلا
#تعریف_کرو HB_ALWAYS_INLINE_سمتیہ_ALLOCS
#جے_انت

سانچا <قسم_ناں Type,
	  منطقي sorted=خطأ>
بنية hb_سمتیہ_t
{
  ساكن constexpr منطقي realloc_move = سچ;

  تعريفنوع Type شے_t;
  ساكن constexpr بنانشانی شے_ناپ = hb_static_ناپ (Type);
  ورتوں array_t = قسم_ناں std::conditional<sorted, hb_sorted_array_t<Type>, hb_array_t<Type>>::type;
  ورتوں c_array_t = قسم_ناں std::conditional<sorted, hb_sorted_array_t<ثابت Type>, hb_array_t<ثابت Type>>::type;

  hb_سمتیہ_t () = افتراضي;
  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  hb_سمتیہ_t (std::initializer_list<Type> lst) : hb_سمتیہ_t ()
  {
    alloc (lst.ناپ (), سچ);
    لكل (خودکار&& شے : lst)
      push (شے);
  }
  سانچا <قسم_ناں Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  واضح hb_سمتیہ_t (ثابت Iterable &o) : hb_سمتیہ_t ()
  {
    extend (o, سچ);
  }
  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  hb_سمتیہ_t (ثابت hb_سمتیہ_t &o) : hb_سمتیہ_t ()
  {
    alloc_exact (o.لمائی);
    إذا (unlikely (وچ_error ())) أرجع;
    copy_array (o.as_array ());
  }
  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  hb_سمتیہ_t (array_t o) : hb_سمتیہ_t ()
  {
    alloc_exact (o.لمائی);
    إذا (unlikely (وچ_error ())) أرجع;
    copy_array (o);
  }
  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  hb_سمتیہ_t (c_array_t o) : hb_سمتیہ_t ()
  {
    alloc_exact (o.لمائی);
    إذا (unlikely (وچ_error ())) أرجع;
    copy_array (o);
  }
  hb_سمتیہ_t (hb_سمتیہ_t &&o) noexcept
  {
    allocated = o.allocated;
    لمائی = o.لمائی;
    arrayZ = o.arrayZ;
    o.init ();
  }
  ~hb_سمتیہ_t () { fini (); }

  سانچا <بنانشانی n>
  فراغ
  سیٹ_storage (Type (&array)[n])
  { سیٹ_storage (array, n); }
  فراغ
  سیٹ_storage (hb_array_t<Type> array)
  { سیٹ_storage (array.arrayZ, array.لمائی); }
  سانچا <قسم_ناں T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T) &&
			  hb_is_trivially_destructible(T))>
  فراغ
  سیٹ_storage (Type *array, بنانشانی n)
  {
    أكد (allocated == 0);
    أكد (لمائی == 0);

    arrayZ = array;
    لمائی = n;
  }

  سانچا <قسم_ناں Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  فراغ extend (ثابت Iterable &o, منطقي exact=خطأ)
  {
    خودکار iter = hb_iter (o);
    إذا (iter.is_random_access_iterator || iter.has_fast_len)
    {
      إذا (unlikely (!alloc (لمائی + hb_len (iter), exact)))
	أرجع;
      بنانشانی گنتی = hb_len (iter);
      لكل (بنانشانی i = 0; i < گنتی; i++)
	ڈالو_has_گنجائش (*iter++);
    }
    طالما (iter)
    {
      إذا (unlikely (!alloc (لمائی + 1)))
        أرجع;
      بنانشانی گنجائش = allocated - لمائی;
      لكل (بنانشانی i = 0; i < گنجائش && iter; i++)
	ڈالو_has_گنجائش (*iter++);
    }
  }
  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  فراغ extend (array_t o, منطقي exact=خطأ)
  { extend (c_array_t (o), exact); }
  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  فراغ extend (c_array_t o, منطقي exact=خطأ)
  {
    /* o may point into our own storage, eg. v.extend (v.as_array ()).
     * alloc() may move that storage, so find o again afterwards. */
    منطقي aliased = o.لمائی && وچ_storage (*o.arrayZ);
    بنانشانی وتھ = aliased ? o.arrayZ - arrayZ : 0;
    alloc (لمائی + o.لمائی, exact);
    إذا (unlikely (وچ_error ())) أرجع;
    إذا (unlikely (aliased))
      o = {arrayZ + وتھ, o.لمائی};
    copy_array (o);
  }
  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  فراغ extend (ثابت hb_سمتیہ_t &o, منطقي exact=خطأ)
  { extend (o.as_array (), exact); }

  عام:
  صحيح allocated = 0; /* < 0 means allocation failed. */
  بنانشانی صحيح لمائی = 0;
  عام:
  Type *arrayZ = خالی_سوچک;

  فراغ init ()
  {
    allocated = لمائی = 0;
    arrayZ = خالی_سوچک;
  }
  فراغ init0 ()
  {
  }

  فراغ fini ()
  {
    إذا (is_owned ())
    {
      shrink_سمتیہ (0);
      hb_free (arrayZ);
    }
    init ();
  }

  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  hb_سمتیہ_t &reset ()
  {
    إذا (unlikely (وچ_error ()))
      reset_error ();
    clear ();
    أرجع *هذا;
  }

  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  hb_سمتیہ_t &reset_if_error ()
  {
    إذا (unlikely (وچ_error ()))
      reset ();
    أرجع *هذا;
  }

  /* Transfer ownership of the backing storage to caller.
   * Returns nullptr if storage is not owned by this vector. */
  Type *
  steal (بنانشانی *len = خالی_سوچک, صحيح *allocated_باہرلا = خالی_سوچک)
  {
    إذا (!is_owned ())
      أرجع خالی_سوچک;
    إذا (len)
      *len = لمائی;
    إذا (allocated_باہرلا)
      *allocated_باہرلا = allocated;
    Type *p = arrayZ;
    init ();
    أرجع p;
  }

  /* Adopt a previously detached owned buffer. */
  فراغ
  recycle_بفر (Type *بفر,
                  بنانشانی len,
                  صحيح allocated_len)
  {
    fini ();
    arrayZ = بفر;
    لمائی = len;
    allocated = allocated_len;
  }

  یار فراغ swap (hb_سمتیہ_t& a, hb_سمتیہ_t& b) noexcept
  {
    hb_وٹاندرا (a.allocated, b.allocated);
    hb_وٹاندرا (a.لمائی, b.لمائی);
    hb_وٹاندرا (a.arrayZ, b.arrayZ);
  }

  hb_سمتیہ_t& عامل = (ثابت hb_سمتیہ_t &o)
  {
    إذا (هذا == &o) أرجع *هذا;

    reset ();
    alloc_exact (o.لمائی);
    إذا (unlikely (وچ_error ())) أرجع *هذا;

    لمائی = 0;
    copy_array (o.as_array ());

    أرجع *هذا;
  }
  hb_سمتیہ_t& عامل = (hb_سمتیہ_t &&o) noexcept
  {
    hb_وٹاندرا (*هذا, o);
    أرجع *هذا;
  }

  hb_بائٹاں_t as_بائٹاں () ثابت
  { أرجع hb_بائٹاں_t ((ثابت حرف *) arrayZ, get_ناپ ()); }

  منطقي عامل == (ثابت hb_سمتیہ_t &o) ثابت { أرجع as_array () == o.as_array (); }
  منطقي عامل != (ثابت hb_سمتیہ_t &o) ثابت { أرجع !(*هذا == o); }
  uint32_t hash () ثابت { أرجع as_array ().hash (); }

  Type& عامل [] (صحيح i_)
  {
    بنانشانی صحيح i = (بنانشانی صحيح) i_;
    إذا (unlikely (i >= لمائی))
      أرجع Crap (Type);
    أرجع arrayZ[i];
  }
  ثابت Type& عامل [] (صحيح i_) ثابت
  {
    بنانشانی صحيح i = (بنانشانی صحيح) i_;
    إذا (unlikely (i >= لمائی))
      أرجع Null (Type);
    أرجع arrayZ[i];
  }

  Type& پوچھ () { أرجع (*هذا)[لمائی - 1]; }
  ثابت Type& پوچھ () ثابت { أرجع (*هذا)[لمائی - 1]; }

  واضح عامل منطقي () ثابت { أرجع لمائی; }
  size_t get_ناپ () ثابت { أرجع hb_unsigned_mul_saturate (لمائی, شے_ناپ); }

  /* Sink interface. */
  سانچا <قسم_ناں T>
  hb_سمتیہ_t& عامل << (T&& v) { push (std::forward<T> (v)); أرجع *هذا; }

  array_t   as_array ()       { أرجع hb_array (arrayZ, لمائی); }
  c_array_t as_array () ثابت { أرجع hb_array (arrayZ, لمائی); }

  /* Iterator. */
  تعريفنوع c_array_t   iter_t;
  تعريفنوع array_t   writer_t;
    iter_t   iter () ثابت { أرجع as_array (); }
  writer_t writer ()       { أرجع as_array (); }
  عامل   iter_t () ثابت { أرجع   iter (); }
  عامل writer_t ()       { أرجع writer (); }

  /* Faster range-based for loop. */
  Type *begin () ثابت { أرجع arrayZ; }
  Type *سرا () ثابت { أرجع arrayZ + لمائی; }


  hb_sorted_array_t<Type> as_sorted_array ()
  { أرجع hb_sorted_array (arrayZ, لمائی); }
  hb_sorted_array_t<ثابت Type> as_sorted_array () ثابت
  { أرجع hb_sorted_array (arrayZ, لمائی); }

  سانچا <قسم_ناں T> واضح عامل T * () { أرجع arrayZ; }
  سانچا <قسم_ناں T> واضح عامل ثابت T * () ثابت { أرجع arrayZ; }

  Type * عامل  + (بنانشانی صحيح i) { أرجع arrayZ + i; }
  ثابت Type * عامل  + (بنانشانی صحيح i) ثابت { أرجع arrayZ + i; }

  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  Type *push ()
  {
    إذا (unlikely (!resize (لمائی + 1)))
      أرجع std::addressof (Crap (Type));
    أرجع std::addressof (arrayZ[لمائی - 1]);
  }
  سانچا <قسم_ناں... دلیلاں_2>
  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  Type *push (دلیلاں_2&&... دلیلاں)
  {
    إذا (unlikely ((صحيح) لمائی >= allocated))
    {
      إذا (unlikely (وچ_error ()))
	أرجع std::addressof (Crap (Type));

      /* An argument may live in our own storage, eg. v.push (v[i]).
       * alloc() frees that storage, so copy it out first. */
      إذا (unlikely (وچ_storage (دلیلاں...)))
      {
	Type copy (std::forward<دلیلاں_2> (دلیلاں)...);
	إذا (unlikely (!alloc (لمائی + 1)))
	  أرجع std::addressof (Crap (Type));
	أرجع ڈالو_has_گنجائش (std::move (copy));
      }
      إذا (unlikely (!alloc (لمائی + 1)))
	// If push failed to allocate then don't copy v, since this may cause
	// the created copy to leak memory since we won't have stored a
	// reference to it.
	أرجع std::addressof (Crap (Type));
    }

    أرجع ڈالو_has_گنجائش (std::forward<دلیلاں_2> (دلیلاں)...);
  }
  سانچا <قسم_ناں... دلیلاں_2>
  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  منطقي ڈالو_or_fail (دلیلاں_2&&... دلیلاں)
  {
    أرجع push (std::forward<دلیلاں_2> (دلیلاں)...) != std::addressof (Crap (Type));
  }
  سانچا <قسم_ناں... دلیلاں_2>
  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  Type *ڈالو_has_گنجائش (دلیلاں_2&&... دلیلاں)
  {
    /* Emplace. */
    Type *p = std::addressof (arrayZ[لمائی++]);
    أرجع جديد (p) Type (std::forward<دلیلاں_2> (دلیلاں)...);
  }

  منطقي وچ_storage () ثابت { أرجع خطأ; }
  سانچا <قسم_ناں T, قسم_ناں... Ts>
  منطقي وچ_storage (ثابت T &v, ثابت Ts &...vs) ثابت
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    أرجع (arrayZ &&
	    (uintptr_t) arrayZ <= p && p < (uintptr_t) (arrayZ + لمائی)) ||
	   وچ_storage (vs...);
  }

  منطقي is_owned () ثابت
  {
    أرجع allocated != 0 && allocated != -1;
  }

  منطقي وچ_error () ثابت { أرجع allocated < 0; }
  فراغ سیٹ_error ()
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
    إذا (!وچ_error ())
      سیٹ_error ();
  }

  Type *
  _realloc (بنانشانی new_allocated)
  {
    إذا (!new_allocated)
    {
      إذا (is_owned ())
	hb_free (arrayZ);
      أرجع خالی_سوچک;
    }
    إذا (!allocated && arrayZ)
    {
      /* If we have a non-null arrayZ but allocated is 0, then we are
       * reallocating from a foreign array. */
      Type *new_array = (Type *) hb_malloc (new_allocated * حجم (Type));
      إذا (unlikely (!new_array))
	أرجع خالی_سوچک;
      hb_memcpy ((فراغ *) new_array, (ثابت فراغ *) arrayZ, لمائی * حجم (Type));
      أرجع new_array;
    }
    أرجع (Type *) hb_realloc (arrayZ, new_allocated * حجم (Type));
  }
  Type *
  _malloc_move (بنانشانی new_allocated)
  {
    إذا (!new_allocated)
    {
      إذا (is_owned ())
	hb_free (arrayZ);
      أرجع خالی_سوچک;
    }
    Type *new_array = (Type *) hb_malloc (new_allocated * حجم (Type));
    إذا (likely (new_array))
    {
      لكل (بنانشانی i = 0; i < لمائی; i++)
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

  سانچا <قسم_ناں T = Type,
	    hb_enable_if (hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_سمتیہ (بنانشانی new_allocated, hb_ترجیح<0>)
  {
    أرجع _realloc (new_allocated);
  }
  سانچا <قسم_ناں T = Type,
	    hb_enable_if (!hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_سمتیہ (بنانشانی new_allocated, hb_ترجیح<0>)
  {
    أرجع _malloc_move (new_allocated);
  }
  /* Specialization for types that can be moved using realloc(). */
  سانچا <قسم_ناں T = Type,
	    hb_enable_if (T::realloc_move)>
  Type *
  realloc_سمتیہ (بنانشانی new_allocated, hb_ترجیح<1>)
  {
    أرجع _realloc (new_allocated);
  }

  سانچا <قسم_ناں T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T))>
  فراغ
  grow_سمتیہ (بنانشانی ناپ, hb_ترجیح<0>)
  {
    hb_memset (arrayZ + لمائی, 0, (ناپ - لمائی) * حجم (*arrayZ));
    لمائی = ناپ;
  }
  سانچا <قسم_ناں T = Type,
	    hb_enable_if (!hb_is_trivially_constructible(T))>
  فراغ
  grow_سمتیہ (بنانشانی ناپ, hb_ترجیح<0>)
  {
    لكل (; لمائی < ناپ; لمائی++)
      جديد (std::addressof (arrayZ[لمائی])) Type ();
  }
  /* Specialization for hb_vector_t<hb_{vector,array}_t<U>> to speed up. */
  سانچا <قسم_ناں T = Type,
	    hb_enable_if (hb_is_same (T, hb_سمتیہ_t<قسم_ناں T::شے_t>) ||
			  hb_is_same (T, hb_array_t <قسم_ناں T::شے_t>))>
  فراغ
  grow_سمتیہ (بنانشانی ناپ, hb_ترجیح<1>)
  {
    hb_memset (arrayZ + لمائی, 0, (ناپ - لمائی) * حجم (*arrayZ));
    لمائی = ناپ;
  }

  سانچا <قسم_ناں T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  فراغ
  copy_array (hb_array_t<Type> other)
  {
    hb_memcpy ((فراغ *) (arrayZ + لمائی), (ثابت فراغ *) other.arrayZ, other.لمائی * شے_ناپ);
    لمائی += other.لمائی;
  }
  سانچا <قسم_ناں T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  فراغ
  copy_array (hb_array_t<ثابت Type> other)
  {
    hb_memcpy ((فراغ *) (arrayZ + لمائی), (ثابت فراغ *) other.arrayZ, other.لمائی * شے_ناپ);
    لمائی += other.لمائی;
  }
  سانچا <قسم_ناں T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			   std::is_copy_constructible<T>::value)>
  فراغ
  copy_array (hb_array_t<ثابت Type> other)
  {
    لكل (بنانشانی i = 0; i < other.لمائی; i++)
      جديد (std::addressof (arrayZ[لمائی + i])) Type (other.arrayZ[i]);
    لمائی += other.لمائی;
  }
  سانچا <قسم_ناں T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			  !std::is_copy_constructible<T>::value &&
			  std::is_default_constructible<T>::value &&
			  std::is_copy_assignable<T>::value)>
  فراغ
  copy_array (hb_array_t<ثابت Type> other)
  {
    لكل (بنانشانی i = 0; i < other.لمائی; i++)
    {
      جديد (std::addressof (arrayZ[لمائی + i])) Type ();
      arrayZ[لمائی + i] = other.arrayZ[i];
    }
    لمائی += other.لمائی;
  }

  فراغ
  shrink_سمتیہ (بنانشانی ناپ)
  {
    أكد (ناپ <= لمائی);
    إذا (!hb_is_trivially_destructible(Type))
    {
      بنانشانی گنتی = لمائی - ناپ;
      Type *p = arrayZ + لمائی;
      طالما (گنتی--)
        (--p)->~Type ();
    }
    لمائی = ناپ;
  }

  فراغ
  کھسکاؤ_تھلے_سمتیہ (بنانشانی i)
  {
    لكل (; i < لمائی; i++)
      arrayZ[i - 1] = std::move (arrayZ[i]);
  }

  /* Allocate for size but don't adjust length. */
  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  منطقي alloc (بنانشانی صحيح ناپ, منطقي exact=خطأ)
  {
    إذا (unlikely (وچ_error ()))
      أرجع خطأ;

    بنانشانی صحيح new_allocated;
    إذا (exact)
    {
      /* If exact was specified, we allow shrinking the storage. */
      ناپ = hb_max (ناپ, لمائی);
      إذا (ناپ <= (بنانشانی) allocated &&
	  ناپ >= (بنانشانی) allocated >> 2)
	أرجع سچ;

      new_allocated = ناپ;
    }
    وإلا
    {
      إذا (likely (ناپ <= (بنانشانی) allocated))
	أرجع سچ;

      new_allocated = allocated;
      طالما (ناپ > new_allocated)
	new_allocated += (new_allocated >> 1) + 8;
    }

    /* Reallocate */

    منطقي overflows =
      (صحيح) وچ_error () ||
      (new_allocated < ناپ) ||
      hb_unsigned_mul_overflows (new_allocated, حجم (Type));

    إذا (unlikely (overflows))
    {
      سیٹ_error ();
      أرجع خطأ;
    }

    Type *new_array = realloc_سمتیہ (new_allocated, hb_prioritize);

    إذا (unlikely (new_allocated && !new_array))
    {
      إذا (new_allocated <= (بنانشانی) allocated)
        أرجع سچ; // shrinking failed; it's okay; happens in our fuzzer

      سیٹ_error ();
      أرجع خطأ;
    }

    arrayZ = new_array;
    allocated = new_allocated;

    أرجع سچ;
  }
  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  منطقي alloc_exact (بنانشانی صحيح ناپ)
  {
    أرجع alloc (ناپ, سچ);
  }

  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  فراغ clear ()
  {
    shrink_سمتیہ (0);
  }

  سانچا <قسم_ناں allocator_t>
  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  منطقي allocate_from_pool (allocator_t *allocator, بنانشانی ناپ, بنانشانی صحيح initialize = سچ)
  {
    إذا (allocator)
    {
      أكد (!لمائی && !allocated);
      arrayZ = (Type *) allocator->alloc (ناپ * حجم (Type), alignof (Type));
      إذا (unlikely (!arrayZ))
      {
	سیٹ_error ();
	أرجع خطأ;
      }
      إذا (initialize)
	grow_سمتیہ (ناپ, hb_prioritize);
      وإلا
	لمائی = ناپ;
      أرجع سچ;
    }
    أرجع resize_بھریا ((صحيح) ناپ, initialize, سچ);
  }

  سانچا <قسم_ناں allocator_t>
  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  منطقي duplicate_سمتیہ_from_pool (allocator_t *allocator, ثابت hb_سمتیہ_t &other)
  {
    إذا (unlikely (!allocate_from_pool (allocator, other.لمائی, خطأ)))
      أرجع خطأ;
    لمائی = 0;
    copy_array (other.as_array ());
    أرجع سچ;
  }

  سانچا <قسم_ناں allocator_t>
  فراغ shrink_پٹھ_منزل_pool (allocator_t *allocator, صحيح ناپ)
  {
    بنانشانی orig_لمائی = لمائی;

    shrink (ناپ, خطأ);

    إذا (allocator && !is_owned ())
      allocator->discard (arrayZ + لمائی, (orig_لمائی - لمائی) * حجم (Type));
  }

  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  منطقي resize_بھریا (صحيح ناپ_3, منطقي initialize, منطقي exact)
  {
    إذا (unlikely (ناپ_3 < 0))
      أرجع خطأ;
    بنانشانی صحيح ناپ = (بنانشانی صحيح) ناپ_3;
    إذا (!alloc (ناپ, exact))
      أرجع خطأ;

    إذا (ناپ > لمائی)
    {
      إذا (initialize)
	grow_سمتیہ (ناپ, hb_prioritize);
    }
    وإلا إذا (ناپ < لمائی)
    {
      إذا (initialize)
	shrink_سمتیہ (ناپ);
    }

    لمائی = ناپ;
    أرجع سچ;
  }
  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  منطقي resize (صحيح ناپ_3)
  {
    أرجع resize_بھریا (ناپ_3, سچ, خطأ);
  }
  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  منطقي resize_dirty (صحيح ناپ_3)
  {
    أرجع resize_بھریا (ناپ_3, خطأ, خطأ);
  }
  HB_ALWAYS_INLINE_سمتیہ_ALLOCS
  منطقي resize_exact (صحيح ناپ_3)
  {
    أرجع resize_بھریا (ناپ_3, سچ, سچ);
  }

  Type نکالو ()
  {
    إذا (!لمائی) أرجع Null (Type);
    Type v (std::move (arrayZ[لمائی - 1]));
    arrayZ[لمائی - 1].~Type ();
    لمائی--;
    أرجع v;
  }

  فراغ remove_ordered (بنانشانی صحيح i)
  {
    إذا (unlikely (i >= لمائی))
      أرجع;
    کھسکاؤ_تھلے_سمتیہ (i + 1);
    arrayZ[لمائی - 1].~Type ();
    لمائی--;
  }

  سانچا <منطقي Sorted = sorted,
	    hb_enable_if (!Sorted)>
  فراغ remove_unordered (بنانشانی صحيح i)
  {
    إذا (unlikely (i >= لمائی))
      أرجع;
    إذا (i != لمائی - 1)
      arrayZ[i] = std::move (arrayZ[لمائی - 1]);
    arrayZ[لمائی - 1].~Type ();
    لمائی--;
  }

  فراغ shrink (صحيح ناپ_3, منطقي shrink_حافظہ = سچ)
  {
    بنانشانی صحيح ناپ = ناپ_3 < 0 ? 0u : (بنانشانی صحيح) ناپ_3;
    إذا (ناپ >= لمائی)
      أرجع;

    shrink_سمتیہ (ناپ);

    إذا (is_owned () && shrink_حافظہ)
      alloc_exact (ناپ); /* To force shrinking memory if needed. */
  }


  /* Sorting API. */
  سانچا <قسم_ناں Compar>
  فراغ qsort (Compar compar)
  { as_array ().qsort (compar); }
  فراغ qsort ()
  { as_array ().qsort (); }

  /* Unsorted search API. */
  سانچا <قسم_ناں T>
  Type *lsearch (ثابت T &x, Type *not_لبھیا = خالی_سوچک)
  { أرجع as_array ().lsearch (x, not_لبھیا); }
  سانچا <قسم_ناں T>
  ثابت Type *lsearch (ثابت T &x, ثابت Type *not_لبھیا = خالی_سوچک) ثابت
  { أرجع as_array ().lsearch (x, not_لبھیا); }
  سانچا <قسم_ناں T>
  منطقي lfind (ثابت T &x, بنانشانی *pos = خالی_سوچک) ثابت
  { أرجع as_array ().lfind (x, pos); }

  /* Sorted search API. */
  سانچا <قسم_ناں T,
	    منطقي Sorted=sorted, hb_enable_if (Sorted)>
  Type *bsearch (ثابت T &x, Type *not_لبھیا = خالی_سوچک)
  { أرجع as_array ().bsearch (x, not_لبھیا); }
  سانچا <قسم_ناں T,
	    منطقي Sorted=sorted, hb_enable_if (Sorted)>
  ثابت Type *bsearch (ثابت T &x, ثابت Type *not_لبھیا = خالی_سوچک) ثابت
  { أرجع as_array ().bsearch (x, not_لبھیا); }
  سانچا <قسم_ناں T,
	    منطقي Sorted=sorted, hb_enable_if (Sorted)>
  منطقي bfind (ثابت T &x, بنانشانی صحيح *i = خالی_سوچک,
	      hb_not_لبھیا_t not_لبھیا = HB_NOT_لبھیا_DONT_STORE,
	      بنانشانی صحيح منزل_store = (بنانشانی صحيح) -1) ثابت
  { أرجع as_array ().bfind (x, i, not_لبھیا, منزل_store); }
};

سانچا <قسم_ناں Type>
ورتوں hb_sorted_سمتیہ_t = hb_سمتیہ_t<Type, سچ>;

#جے_انت /* HB_VECTOR_HH */

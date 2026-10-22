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

#جيڪڏهن_وصف_نه_ٿيل HB_ويڪٽر_HH
#وصف_ڪريو HB_ويڪٽر_HH

#ضمن "hb.hh"
#ضمن "hb-array.hh"
#ضمن "hb-meta.hh"
#ضمن "hb-null.hh"

// Change to 1 to force inline vector allocs, to see callsite in malloc-stats tool.
#إذا 0
#وصف_ڪريو HB_ALWAYS_INLINE_ويڪٽر_ALLOCS HB_ALWAYS_INLINE
#وإلا
#وصف_ڪريو HB_ALWAYS_INLINE_ويڪٽر_ALLOCS
#جيڪڏهن_پورو

سانچو <قسم_نالو Type,
	  منطقي sorted=خطأ>
بنية hb_ويڪٽر_t
{
  ساكن constexpr منطقي realloc_move = سچ;

  تعريفنوع Type شي_t;
  ساكن constexpr بنانشان شي_سائيز = hb_static_سائيز (Type);
  استعمال array_t = قسم_نالو std::conditional<sorted, hb_sorted_array_t<Type>, hb_array_t<Type>>::type;
  استعمال c_array_t = قسم_نالو std::conditional<sorted, hb_sorted_array_t<ثابت Type>, hb_array_t<ثابت Type>>::type;

  hb_ويڪٽر_t () = افتراضي;
  HB_ALWAYS_INLINE_ويڪٽر_ALLOCS
  hb_ويڪٽر_t (std::initializer_list<Type> lst) : hb_ويڪٽر_t ()
  {
    alloc (lst.سائيز (), سچ);
    لكل (خودڪار&& شي : lst)
      push (شي);
  }
  سانچو <قسم_نالو Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  واضح hb_ويڪٽر_t (ثابت Iterable &o) : hb_ويڪٽر_t ()
  {
    extend (o, سچ);
  }
  HB_ALWAYS_INLINE_ويڪٽر_ALLOCS
  hb_ويڪٽر_t (ثابت hb_ويڪٽر_t &o) : hb_ويڪٽر_t ()
  {
    alloc_exact (o.ڊيگهائي);
    إذا (unlikely (اندريون_error ())) أرجع;
    copy_array (o.as_array ());
  }
  HB_ALWAYS_INLINE_ويڪٽر_ALLOCS
  hb_ويڪٽر_t (array_t o) : hb_ويڪٽر_t ()
  {
    alloc_exact (o.ڊيگهائي);
    إذا (unlikely (اندريون_error ())) أرجع;
    copy_array (o);
  }
  HB_ALWAYS_INLINE_ويڪٽر_ALLOCS
  hb_ويڪٽر_t (c_array_t o) : hb_ويڪٽر_t ()
  {
    alloc_exact (o.ڊيگهائي);
    إذا (unlikely (اندريون_error ())) أرجع;
    copy_array (o);
  }
  hb_ويڪٽر_t (hb_ويڪٽر_t &&o) noexcept
  {
    allocated = o.allocated;
    ڊيگهائي = o.ڊيگهائي;
    arrayZ = o.arrayZ;
    o.init ();
  }
  ~hb_ويڪٽر_t () { fini (); }

  سانچو <بنانشان n>
  فراغ
  سيٽ_storage (Type (&array)[n])
  { سيٽ_storage (array, n); }
  فراغ
  سيٽ_storage (hb_array_t<Type> array)
  { سيٽ_storage (array.arrayZ, array.ڊيگهائي); }
  سانچو <قسم_نالو T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T) &&
			  hb_is_trivially_destructible(T))>
  فراغ
  سيٽ_storage (Type *array, بنانشان n)
  {
    أكد (allocated == 0);
    أكد (ڊيگهائي == 0);

    arrayZ = array;
    ڊيگهائي = n;
  }

  سانچو <قسم_نالو Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  HB_ALWAYS_INLINE_ويڪٽر_ALLOCS
  فراغ extend (ثابت Iterable &o, منطقي exact=خطأ)
  {
    خودڪار iter = hb_iter (o);
    إذا (iter.is_random_access_iterator || iter.has_fast_len)
    {
      إذا (unlikely (!alloc (ڊيگهائي + hb_len (iter), exact)))
	أرجع;
      بنانشان ڳڻپ = hb_len (iter);
      لكل (بنانشان i = 0; i < ڳڻپ; i++)
	ڌڪو_has_گنجائش (*iter++);
    }
    طالما (iter)
    {
      إذا (unlikely (!alloc (ڊيگهائي + 1)))
        أرجع;
      بنانشان گنجائش = allocated - ڊيگهائي;
      لكل (بنانشان i = 0; i < گنجائش && iter; i++)
	ڌڪو_has_گنجائش (*iter++);
    }
  }
  HB_ALWAYS_INLINE_ويڪٽر_ALLOCS
  فراغ extend (array_t o, منطقي exact=خطأ)
  { extend (c_array_t (o), exact); }
  HB_ALWAYS_INLINE_ويڪٽر_ALLOCS
  فراغ extend (c_array_t o, منطقي exact=خطأ)
  {
    /* o may point into our own storage, eg. v.extend (v.as_array ()).
     * alloc() may move that storage, so find o again afterwards. */
    منطقي aliased = o.ڊيگهائي && اندريون_storage (*o.arrayZ);
    بنانشان وٿي = aliased ? o.arrayZ - arrayZ : 0;
    alloc (ڊيگهائي + o.ڊيگهائي, exact);
    إذا (unlikely (اندريون_error ())) أرجع;
    إذا (unlikely (aliased))
      o = {arrayZ + وٿي, o.ڊيگهائي};
    copy_array (o);
  }
  HB_ALWAYS_INLINE_ويڪٽر_ALLOCS
  فراغ extend (ثابت hb_ويڪٽر_t &o, منطقي exact=خطأ)
  { extend (o.as_array (), exact); }

  عام:
  صحيح allocated = 0; /* < 0 means allocation failed. */
  بنانشان صحيح ڊيگهائي = 0;
  عام:
  Type *arrayZ = خالي_اشارو;

  فراغ init ()
  {
    allocated = ڊيگهائي = 0;
    arrayZ = خالي_اشارو;
  }
  فراغ init0 ()
  {
  }

  فراغ fini ()
  {
    إذا (is_owned ())
    {
      shrink_ويڪٽر (0);
      hb_free (arrayZ);
    }
    init ();
  }

  HB_ALWAYS_INLINE_ويڪٽر_ALLOCS
  hb_ويڪٽر_t &reset ()
  {
    إذا (unlikely (اندريون_error ()))
      reset_error ();
    clear ();
    أرجع *هذا;
  }

  HB_ALWAYS_INLINE_ويڪٽر_ALLOCS
  hb_ويڪٽر_t &reset_if_error ()
  {
    إذا (unlikely (اندريون_error ()))
      reset ();
    أرجع *هذا;
  }

  /* Transfer ownership of the backing storage to caller.
   * Returns nullptr if storage is not owned by this vector. */
  Type *
  steal (بنانشان *len = خالي_اشارو, صحيح *allocated_ٻاهريون = خالي_اشارو)
  {
    إذا (!is_owned ())
      أرجع خالي_اشارو;
    إذا (len)
      *len = ڊيگهائي;
    إذا (allocated_ٻاهريون)
      *allocated_ٻاهريون = allocated;
    Type *p = arrayZ;
    init ();
    أرجع p;
  }

  /* Adopt a previously detached owned buffer. */
  فراغ
  recycle_بفر (Type *بفر,
                  بنانشان len,
                  صحيح allocated_len)
  {
    fini ();
    arrayZ = بفر;
    ڊيگهائي = len;
    allocated = allocated_len;
  }

  دوست فراغ swap (hb_ويڪٽر_t& a, hb_ويڪٽر_t& b) noexcept
  {
    hb_مٽا_سٽا (a.allocated, b.allocated);
    hb_مٽا_سٽا (a.ڊيگهائي, b.ڊيگهائي);
    hb_مٽا_سٽا (a.arrayZ, b.arrayZ);
  }

  hb_ويڪٽر_t& عامل = (ثابت hb_ويڪٽر_t &o)
  {
    إذا (هذا == &o) أرجع *هذا;

    reset ();
    alloc_exact (o.ڊيگهائي);
    إذا (unlikely (اندريون_error ())) أرجع *هذا;

    ڊيگهائي = 0;
    copy_array (o.as_array ());

    أرجع *هذا;
  }
  hb_ويڪٽر_t& عامل = (hb_ويڪٽر_t &&o) noexcept
  {
    hb_مٽا_سٽا (*هذا, o);
    أرجع *هذا;
  }

  hb_بائيٽون_t as_بائيٽون () ثابت
  { أرجع hb_بائيٽون_t ((ثابت حرف *) arrayZ, get_سائيز ()); }

  منطقي عامل == (ثابت hb_ويڪٽر_t &o) ثابت { أرجع as_array () == o.as_array (); }
  منطقي عامل != (ثابت hb_ويڪٽر_t &o) ثابت { أرجع !(*هذا == o); }
  uint32_t hash () ثابت { أرجع as_array ().hash (); }

  Type& عامل [] (صحيح i_)
  {
    بنانشان صحيح i = (بنانشان صحيح) i_;
    إذا (unlikely (i >= ڊيگهائي))
      أرجع Crap (Type);
    أرجع arrayZ[i];
  }
  ثابت Type& عامل [] (صحيح i_) ثابت
  {
    بنانشان صحيح i = (بنانشان صحيح) i_;
    إذا (unlikely (i >= ڊيگهائي))
      أرجع Null (Type);
    أرجع arrayZ[i];
  }

  Type& پڇ () { أرجع (*هذا)[ڊيگهائي - 1]; }
  ثابت Type& پڇ () ثابت { أرجع (*هذا)[ڊيگهائي - 1]; }

  واضح عامل منطقي () ثابت { أرجع ڊيگهائي; }
  size_t get_سائيز () ثابت { أرجع hb_unsigned_mul_saturate (ڊيگهائي, شي_سائيز); }

  /* Sink interface. */
  سانچو <قسم_نالو T>
  hb_ويڪٽر_t& عامل << (T&& v) { push (std::forward<T> (v)); أرجع *هذا; }

  array_t   as_array ()       { أرجع hb_array (arrayZ, ڊيگهائي); }
  c_array_t as_array () ثابت { أرجع hb_array (arrayZ, ڊيگهائي); }

  /* Iterator. */
  تعريفنوع c_array_t   iter_t;
  تعريفنوع array_t   writer_t;
    iter_t   iter () ثابت { أرجع as_array (); }
  writer_t writer ()       { أرجع as_array (); }
  عامل   iter_t () ثابت { أرجع   iter (); }
  عامل writer_t ()       { أرجع writer (); }

  /* Faster range-based for loop. */
  Type *begin () ثابت { أرجع arrayZ; }
  Type *پڇاڙي () ثابت { أرجع arrayZ + ڊيگهائي; }


  hb_sorted_array_t<Type> as_sorted_array ()
  { أرجع hb_sorted_array (arrayZ, ڊيگهائي); }
  hb_sorted_array_t<ثابت Type> as_sorted_array () ثابت
  { أرجع hb_sorted_array (arrayZ, ڊيگهائي); }

  سانچو <قسم_نالو T> واضح عامل T * () { أرجع arrayZ; }
  سانچو <قسم_نالو T> واضح عامل ثابت T * () ثابت { أرجع arrayZ; }

  Type * عامل  + (بنانشان صحيح i) { أرجع arrayZ + i; }
  ثابت Type * عامل  + (بنانشان صحيح i) ثابت { أرجع arrayZ + i; }

  HB_ALWAYS_INLINE_ويڪٽر_ALLOCS
  Type *push ()
  {
    إذا (unlikely (!resize (ڊيگهائي + 1)))
      أرجع std::addressof (Crap (Type));
    أرجع std::addressof (arrayZ[ڊيگهائي - 1]);
  }
  سانچو <قسم_نالو... دليلون_2>
  HB_ALWAYS_INLINE_ويڪٽر_ALLOCS
  Type *push (دليلون_2&&... دليلون)
  {
    إذا (unlikely ((صحيح) ڊيگهائي >= allocated))
    {
      إذا (unlikely (اندريون_error ()))
	أرجع std::addressof (Crap (Type));

      /* An argument may live in our own storage, eg. v.push (v[i]).
       * alloc() frees that storage, so copy it out first. */
      إذا (unlikely (اندريون_storage (دليلون...)))
      {
	Type copy (std::forward<دليلون_2> (دليلون)...);
	إذا (unlikely (!alloc (ڊيگهائي + 1)))
	  أرجع std::addressof (Crap (Type));
	أرجع ڌڪو_has_گنجائش (std::move (copy));
      }
      إذا (unlikely (!alloc (ڊيگهائي + 1)))
	// If push failed to allocate then don't copy v, since this may cause
	// the created copy to leak memory since we won't have stored a
	// reference to it.
	أرجع std::addressof (Crap (Type));
    }

    أرجع ڌڪو_has_گنجائش (std::forward<دليلون_2> (دليلون)...);
  }
  سانچو <قسم_نالو... دليلون_2>
  HB_ALWAYS_INLINE_ويڪٽر_ALLOCS
  منطقي ڌڪو_or_fail (دليلون_2&&... دليلون)
  {
    أرجع push (std::forward<دليلون_2> (دليلون)...) != std::addressof (Crap (Type));
  }
  سانچو <قسم_نالو... دليلون_2>
  HB_ALWAYS_INLINE_ويڪٽر_ALLOCS
  Type *ڌڪو_has_گنجائش (دليلون_2&&... دليلون)
  {
    /* Emplace. */
    Type *p = std::addressof (arrayZ[ڊيگهائي++]);
    أرجع جديد (p) Type (std::forward<دليلون_2> (دليلون)...);
  }

  منطقي اندريون_storage () ثابت { أرجع خطأ; }
  سانچو <قسم_نالو T, قسم_نالو... Ts>
  منطقي اندريون_storage (ثابت T &v, ثابت Ts &...vs) ثابت
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    أرجع (arrayZ &&
	    (uintptr_t) arrayZ <= p && p < (uintptr_t) (arrayZ + ڊيگهائي)) ||
	   اندريون_storage (vs...);
  }

  منطقي is_owned () ثابت
  {
    أرجع allocated != 0 && allocated != -1;
  }

  منطقي اندريون_error () ثابت { أرجع allocated < 0; }
  فراغ سيٽ_error ()
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
    إذا (!اندريون_error ())
      سيٽ_error ();
  }

  Type *
  _realloc (بنانشان new_allocated)
  {
    إذا (!new_allocated)
    {
      إذا (is_owned ())
	hb_free (arrayZ);
      أرجع خالي_اشارو;
    }
    إذا (!allocated && arrayZ)
    {
      /* If we have a non-null arrayZ but allocated is 0, then we are
       * reallocating from a foreign array. */
      Type *new_array = (Type *) hb_malloc (new_allocated * حجم (Type));
      إذا (unlikely (!new_array))
	أرجع خالي_اشارو;
      hb_memcpy ((فراغ *) new_array, (ثابت فراغ *) arrayZ, ڊيگهائي * حجم (Type));
      أرجع new_array;
    }
    أرجع (Type *) hb_realloc (arrayZ, new_allocated * حجم (Type));
  }
  Type *
  _malloc_move (بنانشان new_allocated)
  {
    إذا (!new_allocated)
    {
      إذا (is_owned ())
	hb_free (arrayZ);
      أرجع خالي_اشارو;
    }
    Type *new_array = (Type *) hb_malloc (new_allocated * حجم (Type));
    إذا (likely (new_array))
    {
      لكل (بنانشان i = 0; i < ڊيگهائي; i++)
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

  سانچو <قسم_نالو T = Type,
	    hb_enable_if (hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_ويڪٽر (بنانشان new_allocated, hb_ترجيح<0>)
  {
    أرجع _realloc (new_allocated);
  }
  سانچو <قسم_نالو T = Type,
	    hb_enable_if (!hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_ويڪٽر (بنانشان new_allocated, hb_ترجيح<0>)
  {
    أرجع _malloc_move (new_allocated);
  }
  /* Specialization for types that can be moved using realloc(). */
  سانچو <قسم_نالو T = Type,
	    hb_enable_if (T::realloc_move)>
  Type *
  realloc_ويڪٽر (بنانشان new_allocated, hb_ترجيح<1>)
  {
    أرجع _realloc (new_allocated);
  }

  سانچو <قسم_نالو T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T))>
  فراغ
  grow_ويڪٽر (بنانشان سائيز, hb_ترجيح<0>)
  {
    hb_memset (arrayZ + ڊيگهائي, 0, (سائيز - ڊيگهائي) * حجم (*arrayZ));
    ڊيگهائي = سائيز;
  }
  سانچو <قسم_نالو T = Type,
	    hb_enable_if (!hb_is_trivially_constructible(T))>
  فراغ
  grow_ويڪٽر (بنانشان سائيز, hb_ترجيح<0>)
  {
    لكل (; ڊيگهائي < سائيز; ڊيگهائي++)
      جديد (std::addressof (arrayZ[ڊيگهائي])) Type ();
  }
  /* Specialization for hb_vector_t<hb_{vector,array}_t<U>> to speed up. */
  سانچو <قسم_نالو T = Type,
	    hb_enable_if (hb_is_same (T, hb_ويڪٽر_t<قسم_نالو T::شي_t>) ||
			  hb_is_same (T, hb_array_t <قسم_نالو T::شي_t>))>
  فراغ
  grow_ويڪٽر (بنانشان سائيز, hb_ترجيح<1>)
  {
    hb_memset (arrayZ + ڊيگهائي, 0, (سائيز - ڊيگهائي) * حجم (*arrayZ));
    ڊيگهائي = سائيز;
  }

  سانچو <قسم_نالو T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  فراغ
  copy_array (hb_array_t<Type> other)
  {
    hb_memcpy ((فراغ *) (arrayZ + ڊيگهائي), (ثابت فراغ *) other.arrayZ, other.ڊيگهائي * شي_سائيز);
    ڊيگهائي += other.ڊيگهائي;
  }
  سانچو <قسم_نالو T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  فراغ
  copy_array (hb_array_t<ثابت Type> other)
  {
    hb_memcpy ((فراغ *) (arrayZ + ڊيگهائي), (ثابت فراغ *) other.arrayZ, other.ڊيگهائي * شي_سائيز);
    ڊيگهائي += other.ڊيگهائي;
  }
  سانچو <قسم_نالو T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			   std::is_copy_constructible<T>::value)>
  فراغ
  copy_array (hb_array_t<ثابت Type> other)
  {
    لكل (بنانشان i = 0; i < other.ڊيگهائي; i++)
      جديد (std::addressof (arrayZ[ڊيگهائي + i])) Type (other.arrayZ[i]);
    ڊيگهائي += other.ڊيگهائي;
  }
  سانچو <قسم_نالو T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			  !std::is_copy_constructible<T>::value &&
			  std::is_default_constructible<T>::value &&
			  std::is_copy_assignable<T>::value)>
  فراغ
  copy_array (hb_array_t<ثابت Type> other)
  {
    لكل (بنانشان i = 0; i < other.ڊيگهائي; i++)
    {
      جديد (std::addressof (arrayZ[ڊيگهائي + i])) Type ();
      arrayZ[ڊيگهائي + i] = other.arrayZ[i];
    }
    ڊيگهائي += other.ڊيگهائي;
  }

  فراغ
  shrink_ويڪٽر (بنانشان سائيز)
  {
    أكد (سائيز <= ڊيگهائي);
    إذا (!hb_is_trivially_destructible(Type))
    {
      بنانشان ڳڻپ = ڊيگهائي - سائيز;
      Type *p = arrayZ + ڊيگهائي;
      طالما (ڳڻپ--)
        (--p)->~Type ();
    }
    ڊيگهائي = سائيز;
  }

  فراغ
  سرڪايو_هيٺ_ڏانهن_ويڪٽر (بنانشان i)
  {
    لكل (; i < ڊيگهائي; i++)
      arrayZ[i - 1] = std::move (arrayZ[i]);
  }

  /* Allocate for size but don't adjust length. */
  HB_ALWAYS_INLINE_ويڪٽر_ALLOCS
  منطقي alloc (بنانشان صحيح سائيز, منطقي exact=خطأ)
  {
    إذا (unlikely (اندريون_error ()))
      أرجع خطأ;

    بنانشان صحيح new_allocated;
    إذا (exact)
    {
      /* If exact was specified, we allow shrinking the storage. */
      سائيز = hb_max (سائيز, ڊيگهائي);
      إذا (سائيز <= (بنانشان) allocated &&
	  سائيز >= (بنانشان) allocated >> 2)
	أرجع سچ;

      new_allocated = سائيز;
    }
    وإلا
    {
      إذا (likely (سائيز <= (بنانشان) allocated))
	أرجع سچ;

      new_allocated = allocated;
      طالما (سائيز > new_allocated)
	new_allocated += (new_allocated >> 1) + 8;
    }

    /* Reallocate */

    منطقي overflows =
      (صحيح) اندريون_error () ||
      (new_allocated < سائيز) ||
      hb_unsigned_mul_overflows (new_allocated, حجم (Type));

    إذا (unlikely (overflows))
    {
      سيٽ_error ();
      أرجع خطأ;
    }

    Type *new_array = realloc_ويڪٽر (new_allocated, hb_prioritize);

    إذا (unlikely (new_allocated && !new_array))
    {
      إذا (new_allocated <= (بنانشان) allocated)
        أرجع سچ; // shrinking failed; it's okay; happens in our fuzzer

      سيٽ_error ();
      أرجع خطأ;
    }

    arrayZ = new_array;
    allocated = new_allocated;

    أرجع سچ;
  }
  HB_ALWAYS_INLINE_ويڪٽر_ALLOCS
  منطقي alloc_exact (بنانشان صحيح سائيز)
  {
    أرجع alloc (سائيز, سچ);
  }

  HB_ALWAYS_INLINE_ويڪٽر_ALLOCS
  فراغ clear ()
  {
    shrink_ويڪٽر (0);
  }

  سانچو <قسم_نالو allocator_t>
  HB_ALWAYS_INLINE_ويڪٽر_ALLOCS
  منطقي allocate_from_pool (allocator_t *allocator, بنانشان سائيز, بنانشان صحيح initialize = سچ)
  {
    إذا (allocator)
    {
      أكد (!ڊيگهائي && !allocated);
      arrayZ = (Type *) allocator->alloc (سائيز * حجم (Type), alignof (Type));
      إذا (unlikely (!arrayZ))
      {
	سيٽ_error ();
	أرجع خطأ;
      }
      إذا (initialize)
	grow_ويڪٽر (سائيز, hb_prioritize);
      وإلا
	ڊيگهائي = سائيز;
      أرجع سچ;
    }
    أرجع resize_ڀريل ((صحيح) سائيز, initialize, سچ);
  }

  سانچو <قسم_نالو allocator_t>
  HB_ALWAYS_INLINE_ويڪٽر_ALLOCS
  منطقي duplicate_ويڪٽر_from_pool (allocator_t *allocator, ثابت hb_ويڪٽر_t &other)
  {
    إذا (unlikely (!allocate_from_pool (allocator, other.ڊيگهائي, خطأ)))
      أرجع خطأ;
    ڊيگهائي = 0;
    copy_array (other.as_array ());
    أرجع سچ;
  }

  سانچو <قسم_نالو allocator_t>
  فراغ shrink_پٺ_منزل_pool (allocator_t *allocator, صحيح سائيز)
  {
    بنانشان orig_ڊيگهائي = ڊيگهائي;

    shrink (سائيز, خطأ);

    إذا (allocator && !is_owned ())
      allocator->discard (arrayZ + ڊيگهائي, (orig_ڊيگهائي - ڊيگهائي) * حجم (Type));
  }

  HB_ALWAYS_INLINE_ويڪٽر_ALLOCS
  منطقي resize_ڀريل (صحيح سائيز_3, منطقي initialize, منطقي exact)
  {
    إذا (unlikely (سائيز_3 < 0))
      أرجع خطأ;
    بنانشان صحيح سائيز = (بنانشان صحيح) سائيز_3;
    إذا (!alloc (سائيز, exact))
      أرجع خطأ;

    إذا (سائيز > ڊيگهائي)
    {
      إذا (initialize)
	grow_ويڪٽر (سائيز, hb_prioritize);
    }
    وإلا إذا (سائيز < ڊيگهائي)
    {
      إذا (initialize)
	shrink_ويڪٽر (سائيز);
    }

    ڊيگهائي = سائيز;
    أرجع سچ;
  }
  HB_ALWAYS_INLINE_ويڪٽر_ALLOCS
  منطقي resize (صحيح سائيز_3)
  {
    أرجع resize_ڀريل (سائيز_3, سچ, خطأ);
  }
  HB_ALWAYS_INLINE_ويڪٽر_ALLOCS
  منطقي resize_dirty (صحيح سائيز_3)
  {
    أرجع resize_ڀريل (سائيز_3, خطأ, خطأ);
  }
  HB_ALWAYS_INLINE_ويڪٽر_ALLOCS
  منطقي resize_exact (صحيح سائيز_3)
  {
    أرجع resize_ڀريل (سائيز_3, سچ, سچ);
  }

  Type ڪڍو ()
  {
    إذا (!ڊيگهائي) أرجع Null (Type);
    Type v (std::move (arrayZ[ڊيگهائي - 1]));
    arrayZ[ڊيگهائي - 1].~Type ();
    ڊيگهائي--;
    أرجع v;
  }

  فراغ remove_ordered (بنانشان صحيح i)
  {
    إذا (unlikely (i >= ڊيگهائي))
      أرجع;
    سرڪايو_هيٺ_ڏانهن_ويڪٽر (i + 1);
    arrayZ[ڊيگهائي - 1].~Type ();
    ڊيگهائي--;
  }

  سانچو <منطقي Sorted = sorted,
	    hb_enable_if (!Sorted)>
  فراغ remove_unordered (بنانشان صحيح i)
  {
    إذا (unlikely (i >= ڊيگهائي))
      أرجع;
    إذا (i != ڊيگهائي - 1)
      arrayZ[i] = std::move (arrayZ[ڊيگهائي - 1]);
    arrayZ[ڊيگهائي - 1].~Type ();
    ڊيگهائي--;
  }

  فراغ shrink (صحيح سائيز_3, منطقي shrink_يادگيري = سچ)
  {
    بنانشان صحيح سائيز = سائيز_3 < 0 ? 0u : (بنانشان صحيح) سائيز_3;
    إذا (سائيز >= ڊيگهائي)
      أرجع;

    shrink_ويڪٽر (سائيز);

    إذا (is_owned () && shrink_يادگيري)
      alloc_exact (سائيز); /* To force shrinking memory if needed. */
  }


  /* Sorting API. */
  سانچو <قسم_نالو Compar>
  فراغ qsort (Compar compar)
  { as_array ().qsort (compar); }
  فراغ qsort ()
  { as_array ().qsort (); }

  /* Unsorted search API. */
  سانچو <قسم_نالو T>
  Type *lsearch (ثابت T &x, Type *not_مليو = خالي_اشارو)
  { أرجع as_array ().lsearch (x, not_مليو); }
  سانچو <قسم_نالو T>
  ثابت Type *lsearch (ثابت T &x, ثابت Type *not_مليو = خالي_اشارو) ثابت
  { أرجع as_array ().lsearch (x, not_مليو); }
  سانچو <قسم_نالو T>
  منطقي lfind (ثابت T &x, بنانشان *pos = خالي_اشارو) ثابت
  { أرجع as_array ().lfind (x, pos); }

  /* Sorted search API. */
  سانچو <قسم_نالو T,
	    منطقي Sorted=sorted, hb_enable_if (Sorted)>
  Type *bsearch (ثابت T &x, Type *not_مليو = خالي_اشارو)
  { أرجع as_array ().bsearch (x, not_مليو); }
  سانچو <قسم_نالو T,
	    منطقي Sorted=sorted, hb_enable_if (Sorted)>
  ثابت Type *bsearch (ثابت T &x, ثابت Type *not_مليو = خالي_اشارو) ثابت
  { أرجع as_array ().bsearch (x, not_مليو); }
  سانچو <قسم_نالو T,
	    منطقي Sorted=sorted, hb_enable_if (Sorted)>
  منطقي bfind (ثابت T &x, بنانشان صحيح *i = خالي_اشارو,
	      hb_not_مليو_t not_مليو = HB_NOT_مليو_DONT_STORE,
	      بنانشان صحيح منزل_store = (بنانشان صحيح) -1) ثابت
  { أرجع as_array ().bfind (x, i, not_مليو, منزل_store); }
};

سانچو <قسم_نالو Type>
استعمال hb_sorted_ويڪٽر_t = hb_ويڪٽر_t<Type, سچ>;

#جيڪڏهن_پورو /* HB_VECTOR_HH */

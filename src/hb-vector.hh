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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ਸਦਿਸ਼_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਸਦਿਸ਼_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-array.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-meta.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-null.hh"

// Change to 1 to force inline vector allocs, to see callsite in malloc-stats tool.
#ਜੇ 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ALWAYS_INLINE_ਸਦਿਸ਼_ALLOCS HB_ALWAYS_INLINE
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ALWAYS_INLINE_ਸਦਿਸ਼_ALLOCS
#ਜੇ_ਅੰਤ

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type,
	  ਤਾਰਕਿਕ sorted=ਝੂਠ>
ਬਣਤਰ hb_ਸਦਿਸ਼_t
{
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ realloc_move = ਸੱਚ;

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ Type ਵਸਤੂ_t;
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਸਤੂ_ਮਾਪ = hb_static_ਮਾਪ (Type);
  ਵਰਤੋਂ array_t = ਕਿਸਮ_ਨਾਂ std::conditional<sorted, hb_sorted_array_t<Type>, hb_array_t<Type>>::type;
  ਵਰਤੋਂ c_array_t = ਕਿਸਮ_ਨਾਂ std::conditional<sorted, hb_sorted_array_t<ਅਚਲ Type>, hb_array_t<ਅਚਲ Type>>::type;

  hb_ਸਦਿਸ਼_t () = ਮੂਲ;
  HB_ALWAYS_INLINE_ਸਦਿਸ਼_ALLOCS
  hb_ਸਦਿਸ਼_t (std::initializer_list<Type> lst) : hb_ਸਦਿਸ਼_t ()
  {
    alloc (lst.ਮਾਪ (), ਸੱਚ);
    ਲਈ (ਸਵੈਚਾਲਿਤ&& ਵਸਤੂ : lst)
      push (ਵਸਤੂ);
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  ਸਪਸ਼ਟ hb_ਸਦਿਸ਼_t (ਅਚਲ Iterable &o) : hb_ਸਦਿਸ਼_t ()
  {
    extend (o, ਸੱਚ);
  }
  HB_ALWAYS_INLINE_ਸਦਿਸ਼_ALLOCS
  hb_ਸਦਿਸ਼_t (ਅਚਲ hb_ਸਦਿਸ਼_t &o) : hb_ਸਦਿਸ਼_t ()
  {
    alloc_exact (o.ਲੰਮਾਈ);
    ਜੇ (unlikely (ਵਿੱਚ_error ())) ਵਾਪਸ;
    copy_array (o.as_array ());
  }
  HB_ALWAYS_INLINE_ਸਦਿਸ਼_ALLOCS
  hb_ਸਦਿਸ਼_t (array_t o) : hb_ਸਦਿਸ਼_t ()
  {
    alloc_exact (o.ਲੰਮਾਈ);
    ਜੇ (unlikely (ਵਿੱਚ_error ())) ਵਾਪਸ;
    copy_array (o);
  }
  HB_ALWAYS_INLINE_ਸਦਿਸ਼_ALLOCS
  hb_ਸਦਿਸ਼_t (c_array_t o) : hb_ਸਦਿਸ਼_t ()
  {
    alloc_exact (o.ਲੰਮਾਈ);
    ਜੇ (unlikely (ਵਿੱਚ_error ())) ਵਾਪਸ;
    copy_array (o);
  }
  hb_ਸਦਿਸ਼_t (hb_ਸਦਿਸ਼_t &&o) noexcept
  {
    allocated = o.allocated;
    ਲੰਮਾਈ = o.ਲੰਮਾਈ;
    arrayZ = o.arrayZ;
    o.init ();
  }
  ~hb_ਸਦਿਸ਼_t () { fini (); }

  ਸਾਂਚਾ <ਬਿਨਾਨਿਸ਼ਾਨੀ n>
  ਖਾਲੀ
  ਸੈੱਟ_storage (Type (&array)[n])
  { ਸੈੱਟ_storage (array, n); }
  ਖਾਲੀ
  ਸੈੱਟ_storage (hb_array_t<Type> array)
  { ਸੈੱਟ_storage (array.arrayZ, array.ਲੰਮਾਈ); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T) &&
			  hb_is_trivially_destructible(T))>
  ਖਾਲੀ
  ਸੈੱਟ_storage (Type *array, ਬਿਨਾਨਿਸ਼ਾਨੀ n)
  {
    ਪ੍ਰਤਿਜ੍ਞਾ (allocated == 0);
    ਪ੍ਰਤਿਜ੍ਞਾ (ਲੰਮਾਈ == 0);

    arrayZ = array;
    ਲੰਮਾਈ = n;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  HB_ALWAYS_INLINE_ਸਦਿਸ਼_ALLOCS
  ਖਾਲੀ extend (ਅਚਲ Iterable &o, ਤਾਰਕਿਕ exact=ਝੂਠ)
  {
    ਸਵੈਚਾਲਿਤ iter = hb_iter (o);
    ਜੇ (iter.is_random_access_iterator || iter.has_fast_len)
    {
      ਜੇ (unlikely (!alloc (ਲੰਮਾਈ + hb_len (iter), exact)))
	ਵਾਪਸ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = hb_len (iter);
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
	ਧੱਕੋ_has_ਗੁੰਜਾਇਸ਼ (*iter++);
    }
    ਜਦੋਂ (iter)
    {
      ਜੇ (unlikely (!alloc (ਲੰਮਾਈ + 1)))
        ਵਾਪਸ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਗੁੰਜਾਇਸ਼ = allocated - ਲੰਮਾਈ;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗੁੰਜਾਇਸ਼ && iter; i++)
	ਧੱਕੋ_has_ਗੁੰਜਾਇਸ਼ (*iter++);
    }
  }
  HB_ALWAYS_INLINE_ਸਦਿਸ਼_ALLOCS
  ਖਾਲੀ extend (array_t o, ਤਾਰਕਿਕ exact=ਝੂਠ)
  { extend (c_array_t (o), exact); }
  HB_ALWAYS_INLINE_ਸਦਿਸ਼_ALLOCS
  ਖਾਲੀ extend (c_array_t o, ਤਾਰਕਿਕ exact=ਝੂਠ)
  {
    /* o may point into our own storage, eg. v.extend (v.as_array ()).
     * alloc() may move that storage, so find o again afterwards. */
    ਤਾਰਕਿਕ aliased = o.ਲੰਮਾਈ && ਵਿੱਚ_storage (*o.arrayZ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਿੱਥ = aliased ? o.arrayZ - arrayZ : 0;
    alloc (ਲੰਮਾਈ + o.ਲੰਮਾਈ, exact);
    ਜੇ (unlikely (ਵਿੱਚ_error ())) ਵਾਪਸ;
    ਜੇ (unlikely (aliased))
      o = {arrayZ + ਵਿੱਥ, o.ਲੰਮਾਈ};
    copy_array (o);
  }
  HB_ALWAYS_INLINE_ਸਦਿਸ਼_ALLOCS
  ਖਾਲੀ extend (ਅਚਲ hb_ਸਦਿਸ਼_t &o, ਤਾਰਕਿਕ exact=ਝੂਠ)
  { extend (o.as_array (), exact); }

  ਜਨਤਕ:
  ਪੂਰਨ allocated = 0; /* < 0 means allocation failed. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਲੰਮਾਈ = 0;
  ਜਨਤਕ:
  Type *arrayZ = ਖਾਲੀ_ਸੂਚਕ;

  ਖਾਲੀ init ()
  {
    allocated = ਲੰਮਾਈ = 0;
    arrayZ = ਖਾਲੀ_ਸੂਚਕ;
  }
  ਖਾਲੀ init0 ()
  {
  }

  ਖਾਲੀ fini ()
  {
    ਜੇ (is_owned ())
    {
      shrink_ਸਦਿਸ਼ (0);
      hb_free (arrayZ);
    }
    init ();
  }

  HB_ALWAYS_INLINE_ਸਦਿਸ਼_ALLOCS
  hb_ਸਦਿਸ਼_t &reset ()
  {
    ਜੇ (unlikely (ਵਿੱਚ_error ()))
      reset_error ();
    clear ();
    ਵਾਪਸ *ਇਹ;
  }

  HB_ALWAYS_INLINE_ਸਦਿਸ਼_ALLOCS
  hb_ਸਦਿਸ਼_t &reset_if_error ()
  {
    ਜੇ (unlikely (ਵਿੱਚ_error ()))
      reset ();
    ਵਾਪਸ *ਇਹ;
  }

  /* Transfer ownership of the backing storage to caller.
   * Returns nullptr if storage is not owned by this vector. */
  Type *
  steal (ਬਿਨਾਨਿਸ਼ਾਨੀ *len = ਖਾਲੀ_ਸੂਚਕ, ਪੂਰਨ *allocated_ਬਾਹਰਲਾ = ਖਾਲੀ_ਸੂਚਕ)
  {
    ਜੇ (!is_owned ())
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    ਜੇ (len)
      *len = ਲੰਮਾਈ;
    ਜੇ (allocated_ਬਾਹਰਲਾ)
      *allocated_ਬਾਹਰਲਾ = allocated;
    Type *p = arrayZ;
    init ();
    ਵਾਪਸ p;
  }

  /* Adopt a previously detached owned buffer. */
  ਖਾਲੀ
  recycle_ਬਫਰ (Type *ਬਫਰ,
                  ਬਿਨਾਨਿਸ਼ਾਨੀ len,
                  ਪੂਰਨ allocated_len)
  {
    fini ();
    arrayZ = ਬਫਰ;
    ਲੰਮਾਈ = len;
    allocated = allocated_len;
  }

  ਮਿੱਤਰ ਖਾਲੀ swap (hb_ਸਦਿਸ਼_t& a, hb_ਸਦਿਸ਼_t& b) noexcept
  {
    hb_ਅਦਲਾ_ਬਦਲੀ (a.allocated, b.allocated);
    hb_ਅਦਲਾ_ਬਦਲੀ (a.ਲੰਮਾਈ, b.ਲੰਮਾਈ);
    hb_ਅਦਲਾ_ਬਦਲੀ (a.arrayZ, b.arrayZ);
  }

  hb_ਸਦਿਸ਼_t& ਸੰਚਾਲਕ = (ਅਚਲ hb_ਸਦਿਸ਼_t &o)
  {
    ਜੇ (ਇਹ == &o) ਵਾਪਸ *ਇਹ;

    reset ();
    alloc_exact (o.ਲੰਮਾਈ);
    ਜੇ (unlikely (ਵਿੱਚ_error ())) ਵਾਪਸ *ਇਹ;

    ਲੰਮਾਈ = 0;
    copy_array (o.as_array ());

    ਵਾਪਸ *ਇਹ;
  }
  hb_ਸਦਿਸ਼_t& ਸੰਚਾਲਕ = (hb_ਸਦਿਸ਼_t &&o) noexcept
  {
    hb_ਅਦਲਾ_ਬਦਲੀ (*ਇਹ, o);
    ਵਾਪਸ *ਇਹ;
  }

  hb_ਬਾਈਟਾਂ_t as_ਬਾਈਟਾਂ () ਅਚਲ
  { ਵਾਪਸ hb_ਬਾਈਟਾਂ_t ((ਅਚਲ ਅੱਖਰ *) arrayZ, get_ਮਾਪ ()); }

  ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ hb_ਸਦਿਸ਼_t &o) ਅਚਲ { ਵਾਪਸ as_array () == o.as_array (); }
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ hb_ਸਦਿਸ਼_t &o) ਅਚਲ { ਵਾਪਸ !(*ਇਹ == o); }
  uint32_t hash () ਅਚਲ { ਵਾਪਸ as_array ().hash (); }

  Type& ਸੰਚਾਲਕ [] (ਪੂਰਨ i_)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) i_;
    ਜੇ (unlikely (i >= ਲੰਮਾਈ))
      ਵਾਪਸ Crap (Type);
    ਵਾਪਸ arrayZ[i];
  }
  ਅਚਲ Type& ਸੰਚਾਲਕ [] (ਪੂਰਨ i_) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) i_;
    ਜੇ (unlikely (i >= ਲੰਮਾਈ))
      ਵਾਪਸ Null (Type);
    ਵਾਪਸ arrayZ[i];
  }

  Type& ਪੂਛ () { ਵਾਪਸ (*ਇਹ)[ਲੰਮਾਈ - 1]; }
  ਅਚਲ Type& ਪੂਛ () ਅਚਲ { ਵਾਪਸ (*ਇਹ)[ਲੰਮਾਈ - 1]; }

  ਸਪਸ਼ਟ ਸੰਚਾਲਕ ਤਾਰਕਿਕ () ਅਚਲ { ਵਾਪਸ ਲੰਮਾਈ; }
  size_t get_ਮਾਪ () ਅਚਲ { ਵਾਪਸ hb_unsigned_mul_saturate (ਲੰਮਾਈ, ਵਸਤੂ_ਮਾਪ); }

  /* Sink interface. */
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  hb_ਸਦਿਸ਼_t& ਸੰਚਾਲਕ << (T&& v) { push (std::forward<T> (v)); ਵਾਪਸ *ਇਹ; }

  array_t   as_array ()       { ਵਾਪਸ hb_array (arrayZ, ਲੰਮਾਈ); }
  c_array_t as_array () ਅਚਲ { ਵਾਪਸ hb_array (arrayZ, ਲੰਮਾਈ); }

  /* Iterator. */
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ c_array_t   iter_t;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ array_t   writer_t;
    iter_t   iter () ਅਚਲ { ਵਾਪਸ as_array (); }
  writer_t writer ()       { ਵਾਪਸ as_array (); }
  ਸੰਚਾਲਕ   iter_t () ਅਚਲ { ਵਾਪਸ   iter (); }
  ਸੰਚਾਲਕ writer_t ()       { ਵਾਪਸ writer (); }

  /* Faster range-based for loop. */
  Type *begin () ਅਚਲ { ਵਾਪਸ arrayZ; }
  Type *ਸਿਰਾ () ਅਚਲ { ਵਾਪਸ arrayZ + ਲੰਮਾਈ; }


  hb_sorted_array_t<Type> as_sorted_array ()
  { ਵਾਪਸ hb_sorted_array (arrayZ, ਲੰਮਾਈ); }
  hb_sorted_array_t<ਅਚਲ Type> as_sorted_array () ਅਚਲ
  { ਵਾਪਸ hb_sorted_array (arrayZ, ਲੰਮਾਈ); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਸਪਸ਼ਟ ਸੰਚਾਲਕ T * () { ਵਾਪਸ arrayZ; }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਸਪਸ਼ਟ ਸੰਚਾਲਕ ਅਚਲ T * () ਅਚਲ { ਵਾਪਸ arrayZ; }

  Type * ਸੰਚਾਲਕ  + (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) { ਵਾਪਸ arrayZ + i; }
  ਅਚਲ Type * ਸੰਚਾਲਕ  + (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ { ਵਾਪਸ arrayZ + i; }

  HB_ALWAYS_INLINE_ਸਦਿਸ਼_ALLOCS
  Type *push ()
  {
    ਜੇ (unlikely (!resize (ਲੰਮਾਈ + 1)))
      ਵਾਪਸ std::addressof (Crap (Type));
    ਵਾਪਸ std::addressof (arrayZ[ਲੰਮਾਈ - 1]);
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ... ਦਲੀਲਾਂ_2>
  HB_ALWAYS_INLINE_ਸਦਿਸ਼_ALLOCS
  Type *push (ਦਲੀਲਾਂ_2&&... ਦਲੀਲਾਂ)
  {
    ਜੇ (unlikely ((ਪੂਰਨ) ਲੰਮਾਈ >= allocated))
    {
      ਜੇ (unlikely (ਵਿੱਚ_error ()))
	ਵਾਪਸ std::addressof (Crap (Type));

      /* An argument may live in our own storage, eg. v.push (v[i]).
       * alloc() frees that storage, so copy it out first. */
      ਜੇ (unlikely (ਵਿੱਚ_storage (ਦਲੀਲਾਂ...)))
      {
	Type copy (std::forward<ਦਲੀਲਾਂ_2> (ਦਲੀਲਾਂ)...);
	ਜੇ (unlikely (!alloc (ਲੰਮਾਈ + 1)))
	  ਵਾਪਸ std::addressof (Crap (Type));
	ਵਾਪਸ ਧੱਕੋ_has_ਗੁੰਜਾਇਸ਼ (std::move (copy));
      }
      ਜੇ (unlikely (!alloc (ਲੰਮਾਈ + 1)))
	// If push failed to allocate then don't copy v, since this may cause
	// the created copy to leak memory since we won't have stored a
	// reference to it.
	ਵਾਪਸ std::addressof (Crap (Type));
    }

    ਵਾਪਸ ਧੱਕੋ_has_ਗੁੰਜਾਇਸ਼ (std::forward<ਦਲੀਲਾਂ_2> (ਦਲੀਲਾਂ)...);
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ... ਦਲੀਲਾਂ_2>
  HB_ALWAYS_INLINE_ਸਦਿਸ਼_ALLOCS
  ਤਾਰਕਿਕ ਧੱਕੋ_or_fail (ਦਲੀਲਾਂ_2&&... ਦਲੀਲਾਂ)
  {
    ਵਾਪਸ push (std::forward<ਦਲੀਲਾਂ_2> (ਦਲੀਲਾਂ)...) != std::addressof (Crap (Type));
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ... ਦਲੀਲਾਂ_2>
  HB_ALWAYS_INLINE_ਸਦਿਸ਼_ALLOCS
  Type *ਧੱਕੋ_has_ਗੁੰਜਾਇਸ਼ (ਦਲੀਲਾਂ_2&&... ਦਲੀਲਾਂ)
  {
    /* Emplace. */
    Type *p = std::addressof (arrayZ[ਲੰਮਾਈ++]);
    ਵਾਪਸ ਨਵਾਂ (p) Type (std::forward<ਦਲੀਲਾਂ_2> (ਦਲੀਲਾਂ)...);
  }

  ਤਾਰਕਿਕ ਵਿੱਚ_storage () ਅਚਲ { ਵਾਪਸ ਝੂਠ; }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ... Ts>
  ਤਾਰਕਿਕ ਵਿੱਚ_storage (ਅਚਲ T &v, ਅਚਲ Ts &...vs) ਅਚਲ
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    ਵਾਪਸ (arrayZ &&
	    (uintptr_t) arrayZ <= p && p < (uintptr_t) (arrayZ + ਲੰਮਾਈ)) ||
	   ਵਿੱਚ_storage (vs...);
  }

  ਤਾਰਕਿਕ is_owned () ਅਚਲ
  {
    ਵਾਪਸ allocated != 0 && allocated != -1;
  }

  ਤਾਰਕਿਕ ਵਿੱਚ_error () ਅਚਲ { ਵਾਪਸ allocated < 0; }
  ਖਾਲੀ ਸੈੱਟ_error ()
  {
    ਪ੍ਰਤਿਜ੍ਞਾ (allocated >= 0);
    allocated = -allocated - 1;
  }
  ਖਾਲੀ reset_error ()
  {
    ਪ੍ਰਤਿਜ੍ਞਾ (allocated < 0);
    allocated = -(allocated + 1);
  }
  ਖਾਲੀ ensure_error ()
  {
    ਜੇ (!ਵਿੱਚ_error ())
      ਸੈੱਟ_error ();
  }

  Type *
  _realloc (ਬਿਨਾਨਿਸ਼ਾਨੀ new_allocated)
  {
    ਜੇ (!new_allocated)
    {
      ਜੇ (is_owned ())
	hb_free (arrayZ);
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    }
    ਜੇ (!allocated && arrayZ)
    {
      /* If we have a non-null arrayZ but allocated is 0, then we are
       * reallocating from a foreign array. */
      Type *new_array = (Type *) hb_malloc (new_allocated * ਆਕਾਰ (Type));
      ਜੇ (unlikely (!new_array))
	ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
      hb_memcpy ((ਖਾਲੀ *) new_array, (ਅਚਲ ਖਾਲੀ *) arrayZ, ਲੰਮਾਈ * ਆਕਾਰ (Type));
      ਵਾਪਸ new_array;
    }
    ਵਾਪਸ (Type *) hb_realloc (arrayZ, new_allocated * ਆਕਾਰ (Type));
  }
  Type *
  _malloc_move (ਬਿਨਾਨਿਸ਼ਾਨੀ new_allocated)
  {
    ਜੇ (!new_allocated)
    {
      ਜੇ (is_owned ())
	hb_free (arrayZ);
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    }
    Type *new_array = (Type *) hb_malloc (new_allocated * ਆਕਾਰ (Type));
    ਜੇ (likely (new_array))
    {
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਲੰਮਾਈ; i++)
      {
	ਨਵਾਂ (std::addressof (new_array[i])) Type ();
	new_array[i] = std::move (arrayZ[i]);
	arrayZ[i].~Type ();
      }
      ਜੇ (is_owned ())
	hb_free (arrayZ);
    }
    ਵਾਪਸ new_array;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T = Type,
	    hb_enable_if (hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_ਸਦਿਸ਼ (ਬਿਨਾਨਿਸ਼ਾਨੀ new_allocated, hb_ਤਰਜੀਹ<0>)
  {
    ਵਾਪਸ _realloc (new_allocated);
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T = Type,
	    hb_enable_if (!hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_ਸਦਿਸ਼ (ਬਿਨਾਨਿਸ਼ਾਨੀ new_allocated, hb_ਤਰਜੀਹ<0>)
  {
    ਵਾਪਸ _malloc_move (new_allocated);
  }
  /* Specialization for types that can be moved using realloc(). */
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T = Type,
	    hb_enable_if (T::realloc_move)>
  Type *
  realloc_ਸਦਿਸ਼ (ਬਿਨਾਨਿਸ਼ਾਨੀ new_allocated, hb_ਤਰਜੀਹ<1>)
  {
    ਵਾਪਸ _realloc (new_allocated);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T))>
  ਖਾਲੀ
  grow_ਸਦਿਸ਼ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ, hb_ਤਰਜੀਹ<0>)
  {
    hb_memset (arrayZ + ਲੰਮਾਈ, 0, (ਮਾਪ - ਲੰਮਾਈ) * ਆਕਾਰ (*arrayZ));
    ਲੰਮਾਈ = ਮਾਪ;
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T = Type,
	    hb_enable_if (!hb_is_trivially_constructible(T))>
  ਖਾਲੀ
  grow_ਸਦਿਸ਼ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ, hb_ਤਰਜੀਹ<0>)
  {
    ਲਈ (; ਲੰਮਾਈ < ਮਾਪ; ਲੰਮਾਈ++)
      ਨਵਾਂ (std::addressof (arrayZ[ਲੰਮਾਈ])) Type ();
  }
  /* Specialization for hb_vector_t<hb_{vector,array}_t<U>> to speed up. */
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T = Type,
	    hb_enable_if (hb_is_same (T, hb_ਸਦਿਸ਼_t<ਕਿਸਮ_ਨਾਂ T::ਵਸਤੂ_t>) ||
			  hb_is_same (T, hb_array_t <ਕਿਸਮ_ਨਾਂ T::ਵਸਤੂ_t>))>
  ਖਾਲੀ
  grow_ਸਦਿਸ਼ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ, hb_ਤਰਜੀਹ<1>)
  {
    hb_memset (arrayZ + ਲੰਮਾਈ, 0, (ਮਾਪ - ਲੰਮਾਈ) * ਆਕਾਰ (*arrayZ));
    ਲੰਮਾਈ = ਮਾਪ;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  ਖਾਲੀ
  copy_array (hb_array_t<Type> other)
  {
    hb_memcpy ((ਖਾਲੀ *) (arrayZ + ਲੰਮਾਈ), (ਅਚਲ ਖਾਲੀ *) other.arrayZ, other.ਲੰਮਾਈ * ਵਸਤੂ_ਮਾਪ);
    ਲੰਮਾਈ += other.ਲੰਮਾਈ;
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  ਖਾਲੀ
  copy_array (hb_array_t<ਅਚਲ Type> other)
  {
    hb_memcpy ((ਖਾਲੀ *) (arrayZ + ਲੰਮਾਈ), (ਅਚਲ ਖਾਲੀ *) other.arrayZ, other.ਲੰਮਾਈ * ਵਸਤੂ_ਮਾਪ);
    ਲੰਮਾਈ += other.ਲੰਮਾਈ;
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			   std::is_copy_constructible<T>::value)>
  ਖਾਲੀ
  copy_array (hb_array_t<ਅਚਲ Type> other)
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < other.ਲੰਮਾਈ; i++)
      ਨਵਾਂ (std::addressof (arrayZ[ਲੰਮਾਈ + i])) Type (other.arrayZ[i]);
    ਲੰਮਾਈ += other.ਲੰਮਾਈ;
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			  !std::is_copy_constructible<T>::value &&
			  std::is_default_constructible<T>::value &&
			  std::is_copy_assignable<T>::value)>
  ਖਾਲੀ
  copy_array (hb_array_t<ਅਚਲ Type> other)
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < other.ਲੰਮਾਈ; i++)
    {
      ਨਵਾਂ (std::addressof (arrayZ[ਲੰਮਾਈ + i])) Type ();
      arrayZ[ਲੰਮਾਈ + i] = other.arrayZ[i];
    }
    ਲੰਮਾਈ += other.ਲੰਮਾਈ;
  }

  ਖਾਲੀ
  shrink_ਸਦਿਸ਼ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ)
  {
    ਪ੍ਰਤਿਜ੍ਞਾ (ਮਾਪ <= ਲੰਮਾਈ);
    ਜੇ (!hb_is_trivially_destructible(Type))
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = ਲੰਮਾਈ - ਮਾਪ;
      Type *p = arrayZ + ਲੰਮਾਈ;
      ਜਦੋਂ (ਗਿਣਤੀ_2--)
        (--p)->~Type ();
    }
    ਲੰਮਾਈ = ਮਾਪ;
  }

  ਖਾਲੀ
  ਖਿਸਕਾਓ_ਥੱਲੇ_ਸਦਿਸ਼ (ਬਿਨਾਨਿਸ਼ਾਨੀ i)
  {
    ਲਈ (; i < ਲੰਮਾਈ; i++)
      arrayZ[i - 1] = std::move (arrayZ[i]);
  }

  /* Allocate for size but don't adjust length. */
  HB_ALWAYS_INLINE_ਸਦਿਸ਼_ALLOCS
  ਤਾਰਕਿਕ alloc (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ, ਤਾਰਕਿਕ exact=ਝੂਠ)
  {
    ਜੇ (unlikely (ਵਿੱਚ_error ()))
      ਵਾਪਸ ਝੂਠ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ new_allocated;
    ਜੇ (exact)
    {
      /* If exact was specified, we allow shrinking the storage. */
      ਮਾਪ = hb_max (ਮਾਪ, ਲੰਮਾਈ);
      ਜੇ (ਮਾਪ <= (ਬਿਨਾਨਿਸ਼ਾਨੀ) allocated &&
	  ਮਾਪ >= (ਬਿਨਾਨਿਸ਼ਾਨੀ) allocated >> 2)
	ਵਾਪਸ ਸੱਚ;

      new_allocated = ਮਾਪ;
    }
    ਨਹੀਂਤਾਂ
    {
      ਜੇ (likely (ਮਾਪ <= (ਬਿਨਾਨਿਸ਼ਾਨੀ) allocated))
	ਵਾਪਸ ਸੱਚ;

      new_allocated = allocated;
      ਜਦੋਂ (ਮਾਪ > new_allocated)
	new_allocated += (new_allocated >> 1) + 8;
    }

    /* Reallocate */

    ਤਾਰਕਿਕ overflows =
      (ਪੂਰਨ) ਵਿੱਚ_error () ||
      (new_allocated < ਮਾਪ) ||
      hb_unsigned_mul_overflows (new_allocated, ਆਕਾਰ (Type));

    ਜੇ (unlikely (overflows))
    {
      ਸੈੱਟ_error ();
      ਵਾਪਸ ਝੂਠ;
    }

    Type *new_array = realloc_ਸਦਿਸ਼ (new_allocated, hb_prioritize);

    ਜੇ (unlikely (new_allocated && !new_array))
    {
      ਜੇ (new_allocated <= (ਬਿਨਾਨਿਸ਼ਾਨੀ) allocated)
        ਵਾਪਸ ਸੱਚ; // shrinking failed; it's okay; happens in our fuzzer

      ਸੈੱਟ_error ();
      ਵਾਪਸ ਝੂਠ;
    }

    arrayZ = new_array;
    allocated = new_allocated;

    ਵਾਪਸ ਸੱਚ;
  }
  HB_ALWAYS_INLINE_ਸਦਿਸ਼_ALLOCS
  ਤਾਰਕਿਕ alloc_exact (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ)
  {
    ਵਾਪਸ alloc (ਮਾਪ, ਸੱਚ);
  }

  HB_ALWAYS_INLINE_ਸਦਿਸ਼_ALLOCS
  ਖਾਲੀ clear ()
  {
    shrink_ਸਦਿਸ਼ (0);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ allocator_t>
  HB_ALWAYS_INLINE_ਸਦਿਸ਼_ALLOCS
  ਤਾਰਕਿਕ allocate_from_pool (allocator_t *allocator, ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ initialize = ਸੱਚ)
  {
    ਜੇ (allocator)
    {
      ਪ੍ਰਤਿਜ੍ਞਾ (!ਲੰਮਾਈ && !allocated);
      arrayZ = (Type *) allocator->alloc (ਮਾਪ * ਆਕਾਰ (Type), alignof (Type));
      ਜੇ (unlikely (!arrayZ))
      {
	ਸੈੱਟ_error ();
	ਵਾਪਸ ਝੂਠ;
      }
      ਜੇ (initialize)
	grow_ਸਦਿਸ਼ (ਮਾਪ, hb_prioritize);
      ਨਹੀਂਤਾਂ
	ਲੰਮਾਈ = ਮਾਪ;
      ਵਾਪਸ ਸੱਚ;
    }
    ਵਾਪਸ resize_ਭਰਿਆ ((ਪੂਰਨ) ਮਾਪ, initialize, ਸੱਚ);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ allocator_t>
  HB_ALWAYS_INLINE_ਸਦਿਸ਼_ALLOCS
  ਤਾਰਕਿਕ duplicate_ਸਦਿਸ਼_from_pool (allocator_t *allocator, ਅਚਲ hb_ਸਦਿਸ਼_t &other)
  {
    ਜੇ (unlikely (!allocate_from_pool (allocator, other.ਲੰਮਾਈ, ਝੂਠ)))
      ਵਾਪਸ ਝੂਠ;
    ਲੰਮਾਈ = 0;
    copy_array (other.as_array ());
    ਵਾਪਸ ਸੱਚ;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ allocator_t>
  ਖਾਲੀ shrink_ਪਿੱਠ_ਮੰਜ਼ਿਲ_pool (allocator_t *allocator, ਪੂਰਨ ਮਾਪ)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ orig_ਲੰਮਾਈ = ਲੰਮਾਈ;

    shrink (ਮਾਪ, ਝੂਠ);

    ਜੇ (allocator && !is_owned ())
      allocator->discard (arrayZ + ਲੰਮਾਈ, (orig_ਲੰਮਾਈ - ਲੰਮਾਈ) * ਆਕਾਰ (Type));
  }

  HB_ALWAYS_INLINE_ਸਦਿਸ਼_ALLOCS
  ਤਾਰਕਿਕ resize_ਭਰਿਆ (ਪੂਰਨ ਮਾਪ_3, ਤਾਰਕਿਕ initialize, ਤਾਰਕਿਕ exact)
  {
    ਜੇ (unlikely (ਮਾਪ_3 < 0))
      ਵਾਪਸ ਝੂਠ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) ਮਾਪ_3;
    ਜੇ (!alloc (ਮਾਪ, exact))
      ਵਾਪਸ ਝੂਠ;

    ਜੇ (ਮਾਪ > ਲੰਮਾਈ)
    {
      ਜੇ (initialize)
	grow_ਸਦਿਸ਼ (ਮਾਪ, hb_prioritize);
    }
    ਨਹੀਂਤਾਂ ਜੇ (ਮਾਪ < ਲੰਮਾਈ)
    {
      ਜੇ (initialize)
	shrink_ਸਦਿਸ਼ (ਮਾਪ);
    }

    ਲੰਮਾਈ = ਮਾਪ;
    ਵਾਪਸ ਸੱਚ;
  }
  HB_ALWAYS_INLINE_ਸਦਿਸ਼_ALLOCS
  ਤਾਰਕਿਕ resize (ਪੂਰਨ ਮਾਪ_3)
  {
    ਵਾਪਸ resize_ਭਰਿਆ (ਮਾਪ_3, ਸੱਚ, ਝੂਠ);
  }
  HB_ALWAYS_INLINE_ਸਦਿਸ਼_ALLOCS
  ਤਾਰਕਿਕ resize_dirty (ਪੂਰਨ ਮਾਪ_3)
  {
    ਵਾਪਸ resize_ਭਰਿਆ (ਮਾਪ_3, ਝੂਠ, ਝੂਠ);
  }
  HB_ALWAYS_INLINE_ਸਦਿਸ਼_ALLOCS
  ਤਾਰਕਿਕ resize_exact (ਪੂਰਨ ਮਾਪ_3)
  {
    ਵਾਪਸ resize_ਭਰਿਆ (ਮਾਪ_3, ਸੱਚ, ਸੱਚ);
  }

  Type ਕੱਢੋ ()
  {
    ਜੇ (!ਲੰਮਾਈ) ਵਾਪਸ Null (Type);
    Type v (std::move (arrayZ[ਲੰਮਾਈ - 1]));
    arrayZ[ਲੰਮਾਈ - 1].~Type ();
    ਲੰਮਾਈ--;
    ਵਾਪਸ v;
  }

  ਖਾਲੀ remove_ordered (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i)
  {
    ਜੇ (unlikely (i >= ਲੰਮਾਈ))
      ਵਾਪਸ;
    ਖਿਸਕਾਓ_ਥੱਲੇ_ਸਦਿਸ਼ (i + 1);
    arrayZ[ਲੰਮਾਈ - 1].~Type ();
    ਲੰਮਾਈ--;
  }

  ਸਾਂਚਾ <ਤਾਰਕਿਕ Sorted = sorted,
	    hb_enable_if (!Sorted)>
  ਖਾਲੀ remove_unordered (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i)
  {
    ਜੇ (unlikely (i >= ਲੰਮਾਈ))
      ਵਾਪਸ;
    ਜੇ (i != ਲੰਮਾਈ - 1)
      arrayZ[i] = std::move (arrayZ[ਲੰਮਾਈ - 1]);
    arrayZ[ਲੰਮਾਈ - 1].~Type ();
    ਲੰਮਾਈ--;
  }

  ਖਾਲੀ shrink (ਪੂਰਨ ਮਾਪ_3, ਤਾਰਕਿਕ shrink_ਸਮ੍ਰਿਤੀ = ਸੱਚ)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ = ਮਾਪ_3 < 0 ? 0u : (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) ਮਾਪ_3;
    ਜੇ (ਮਾਪ >= ਲੰਮਾਈ)
      ਵਾਪਸ;

    shrink_ਸਦਿਸ਼ (ਮਾਪ);

    ਜੇ (is_owned () && shrink_ਸਮ੍ਰਿਤੀ)
      alloc_exact (ਮਾਪ); /* To force shrinking memory if needed. */
  }


  /* Sorting API. */
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Compar>
  ਖਾਲੀ qsort (Compar compar)
  { as_array ().qsort (compar); }
  ਖਾਲੀ qsort ()
  { as_array ().qsort (); }

  /* Unsorted search API. */
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  Type *lsearch (ਅਚਲ T &x, Type *not_ਮਿਲਿਆ = ਖਾਲੀ_ਸੂਚਕ)
  { ਵਾਪਸ as_array ().lsearch (x, not_ਮਿਲਿਆ); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਅਚਲ Type *lsearch (ਅਚਲ T &x, ਅਚਲ Type *not_ਮਿਲਿਆ = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  { ਵਾਪਸ as_array ().lsearch (x, not_ਮਿਲਿਆ); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਤਾਰਕਿਕ lfind (ਅਚਲ T &x, ਬਿਨਾਨਿਸ਼ਾਨੀ *pos = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  { ਵਾਪਸ as_array ().lfind (x, pos); }

  /* Sorted search API. */
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T,
	    ਤਾਰਕਿਕ Sorted=sorted, hb_enable_if (Sorted)>
  Type *bsearch (ਅਚਲ T &x, Type *not_ਮਿਲਿਆ = ਖਾਲੀ_ਸੂਚਕ)
  { ਵਾਪਸ as_array ().bsearch (x, not_ਮਿਲਿਆ); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T,
	    ਤਾਰਕਿਕ Sorted=sorted, hb_enable_if (Sorted)>
  ਅਚਲ Type *bsearch (ਅਚਲ T &x, ਅਚਲ Type *not_ਮਿਲਿਆ = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  { ਵਾਪਸ as_array ().bsearch (x, not_ਮਿਲਿਆ); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T,
	    ਤਾਰਕਿਕ Sorted=sorted, hb_enable_if (Sorted)>
  ਤਾਰਕਿਕ bfind (ਅਚਲ T &x, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *i = ਖਾਲੀ_ਸੂਚਕ,
	      hb_not_ਮਿਲਿਆ_t not_ਮਿਲਿਆ = HB_NOT_ਮਿਲਿਆ_DONT_STORE,
	      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮੰਜ਼ਿਲ_store = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) -1) ਅਚਲ
  { ਵਾਪਸ as_array ().bfind (x, i, not_ਮਿਲਿਆ, ਮੰਜ਼ਿਲ_store); }
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
ਵਰਤੋਂ hb_sorted_ਸਦਿਸ਼_t = hb_ਸਦਿਸ਼_t<Type, ਸੱਚ>;

#ਜੇ_ਅੰਤ /* HB_VECTOR_HH */

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

#నిర్వచితం_కాకపోతే HB_సదిశ_HH
#నిర్వచించు HB_సదిశ_HH

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-array.hh"
#అన్తర్భావ "hb-meta.hh"
#అన్తర్భావ "hb-null.hh"

// Change to 1 to force inline vector allocs, to see callsite in malloc-stats tool.
#ఒకవేళ 0
#నిర్వచించు HB_ALWAYS_INLINE_సదిశ_ALLOCS HB_ALWAYS_INLINE
#లేకపోతే
#నిర్వచించు HB_ALWAYS_INLINE_సదిశ_ALLOCS
#అయితే_ముగింపు

మూస <రకం_పేరు Type,
	  తార్కిక sorted=అబద్ధం>
నిర్మాణం hb_సదిశ_t
{
  స్థిర constexpr తార్కిక realloc_move = నిజం;

  రకనిర్వచనం Type అంశం_t;
  స్థిర constexpr గుర్తులేని అంశం_కొలత = hb_static_కొలత (Type);
  ఉపయోగించి array_t = రకం_పేరు std::conditional<sorted, hb_sorted_array_t<Type>, hb_array_t<Type>>::type;
  ఉపయోగించి c_array_t = రకం_పేరు std::conditional<sorted, hb_sorted_array_t<స్థిరరాశి Type>, hb_array_t<స్థిరరాశి Type>>::type;

  hb_సదిశ_t () = అప్రమేయ;
  HB_ALWAYS_INLINE_సదిశ_ALLOCS
  hb_సదిశ_t (std::initializer_list<Type> lst) : hb_సదిశ_t ()
  {
    alloc (lst.కొలత (), నిజం);
    కొరకు (స్వయంచాలక&& అంశం : lst)
      push (అంశం);
  }
  మూస <రకం_పేరు Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  స్పష్టమైన hb_సదిశ_t (స్థిరరాశి Iterable &o) : hb_సదిశ_t ()
  {
    extend (o, నిజం);
  }
  HB_ALWAYS_INLINE_సదిశ_ALLOCS
  hb_సదిశ_t (స్థిరరాశి hb_సదిశ_t &o) : hb_సదిశ_t ()
  {
    alloc_exact (o.నిడివి);
    ఒకవేళ (unlikely (లోపల_error ())) తిరిగి;
    copy_array (o.as_array ());
  }
  HB_ALWAYS_INLINE_సదిశ_ALLOCS
  hb_సదిశ_t (array_t o) : hb_సదిశ_t ()
  {
    alloc_exact (o.నిడివి);
    ఒకవేళ (unlikely (లోపల_error ())) తిరిగి;
    copy_array (o);
  }
  HB_ALWAYS_INLINE_సదిశ_ALLOCS
  hb_సదిశ_t (c_array_t o) : hb_సదిశ_t ()
  {
    alloc_exact (o.నిడివి);
    ఒకవేళ (unlikely (లోపల_error ())) తిరిగి;
    copy_array (o);
  }
  hb_సదిశ_t (hb_సదిశ_t &&o) noexcept
  {
    allocated = o.allocated;
    నిడివి = o.నిడివి;
    arrayZ = o.arrayZ;
    o.init ();
  }
  ~hb_సదిశ_t () { fini (); }

  మూస <గుర్తులేని n>
  శూన్య
  సమితి_storage (Type (&array)[n])
  { సమితి_storage (array, n); }
  శూన్య
  సమితి_storage (hb_array_t<Type> array)
  { సమితి_storage (array.arrayZ, array.నిడివి); }
  మూస <రకం_పేరు T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T) &&
			  hb_is_trivially_destructible(T))>
  శూన్య
  సమితి_storage (Type *array, గుర్తులేని n)
  {
    ప్రతిజ్ఞా (allocated == 0);
    ప్రతిజ్ఞా (నిడివి == 0);

    arrayZ = array;
    నిడివి = n;
  }

  మూస <రకం_పేరు Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  HB_ALWAYS_INLINE_సదిశ_ALLOCS
  శూన్య extend (స్థిరరాశి Iterable &o, తార్కిక exact=అబద్ధం)
  {
    స్వయంచాలక iter = hb_iter (o);
    ఒకవేళ (iter.is_random_access_iterator || iter.has_fast_len)
    {
      ఒకవేళ (unlikely (!alloc (నిడివి + hb_len (iter), exact)))
	తిరిగి;
      గుర్తులేని లెక్క = hb_len (iter);
      కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
	నెట్టు_has_స్థలం (*iter++);
    }
    అయితే (iter)
    {
      ఒకవేళ (unlikely (!alloc (నిడివి + 1)))
        తిరిగి;
      గుర్తులేని స్థలం = allocated - నిడివి;
      కొరకు (గుర్తులేని i = 0; i < స్థలం && iter; i++)
	నెట్టు_has_స్థలం (*iter++);
    }
  }
  HB_ALWAYS_INLINE_సదిశ_ALLOCS
  శూన్య extend (array_t o, తార్కిక exact=అబద్ధం)
  { extend (c_array_t (o), exact); }
  HB_ALWAYS_INLINE_సదిశ_ALLOCS
  శూన్య extend (c_array_t o, తార్కిక exact=అబద్ధం)
  {
    /* o may point into our own storage, eg. v.extend (v.as_array ()).
     * alloc() may move that storage, so find o again afterwards. */
    తార్కిక aliased = o.నిడివి && లోపల_storage (*o.arrayZ);
    గుర్తులేని స్థానభ్రంశం = aliased ? o.arrayZ - arrayZ : 0;
    alloc (నిడివి + o.నిడివి, exact);
    ఒకవేళ (unlikely (లోపల_error ())) తిరిగి;
    ఒకవేళ (unlikely (aliased))
      o = {arrayZ + స్థానభ్రంశం, o.నిడివి};
    copy_array (o);
  }
  HB_ALWAYS_INLINE_సదిశ_ALLOCS
  శూన్య extend (స్థిరరాశి hb_సదిశ_t &o, తార్కిక exact=అబద్ధం)
  { extend (o.as_array (), exact); }

  బహిరంగ:
  పూర్ణాంకం allocated = 0; /* < 0 means allocation failed. */
  గుర్తులేని పూర్ణాంకం నిడివి = 0;
  బహిరంగ:
  Type *arrayZ = శూన్యసూచిక;

  శూన్య init ()
  {
    allocated = నిడివి = 0;
    arrayZ = శూన్యసూచిక;
  }
  శూన్య init0 ()
  {
  }

  శూన్య fini ()
  {
    ఒకవేళ (is_owned ())
    {
      shrink_సదిశ (0);
      hb_free (arrayZ);
    }
    init ();
  }

  HB_ALWAYS_INLINE_సదిశ_ALLOCS
  hb_సదిశ_t &reset ()
  {
    ఒకవేళ (unlikely (లోపల_error ()))
      reset_error ();
    clear ();
    తిరిగి *ఇది;
  }

  HB_ALWAYS_INLINE_సదిశ_ALLOCS
  hb_సదిశ_t &reset_if_error ()
  {
    ఒకవేళ (unlikely (లోపల_error ()))
      reset ();
    తిరిగి *ఇది;
  }

  /* Transfer ownership of the backing storage to caller.
   * Returns nullptr if storage is not owned by this vector. */
  Type *
  steal (గుర్తులేని *len = శూన్యసూచిక, పూర్ణాంకం *allocated_బహిర్గతం = శూన్యసూచిక)
  {
    ఒకవేళ (!is_owned ())
      తిరిగి శూన్యసూచిక;
    ఒకవేళ (len)
      *len = నిడివి;
    ఒకవేళ (allocated_బహిర్గతం)
      *allocated_బహిర్గతం = allocated;
    Type *p = arrayZ;
    init ();
    తిరిగి p;
  }

  /* Adopt a previously detached owned buffer. */
  శూన్య
  recycle_బఫరు (Type *బఫరు,
                  గుర్తులేని len,
                  పూర్ణాంకం allocated_len)
  {
    fini ();
    arrayZ = బఫరు;
    నిడివి = len;
    allocated = allocated_len;
  }

  మిత్ర శూన్య swap (hb_సదిశ_t& a, hb_సదిశ_t& b) noexcept
  {
    hb_మార్పిడి (a.allocated, b.allocated);
    hb_మార్పిడి (a.నిడివి, b.నిడివి);
    hb_మార్పిడి (a.arrayZ, b.arrayZ);
  }

  hb_సదిశ_t& క్రియాకారకం = (స్థిరరాశి hb_సదిశ_t &o)
  {
    ఒకవేళ (ఇది == &o) తిరిగి *ఇది;

    reset ();
    alloc_exact (o.నిడివి);
    ఒకవేళ (unlikely (లోపల_error ())) తిరిగి *ఇది;

    నిడివి = 0;
    copy_array (o.as_array ());

    తిరిగి *ఇది;
  }
  hb_సదిశ_t& క్రియాకారకం = (hb_సదిశ_t &&o) noexcept
  {
    hb_మార్పిడి (*ఇది, o);
    తిరిగి *ఇది;
  }

  hb_బైట్లు_t as_బైట్లు () స్థిరరాశి
  { తిరిగి hb_బైట్లు_t ((స్థిరరాశి అక్షరం *) arrayZ, get_కొలత ()); }

  తార్కిక క్రియాకారకం == (స్థిరరాశి hb_సదిశ_t &o) స్థిరరాశి { తిరిగి as_array () == o.as_array (); }
  తార్కిక క్రియాకారకం != (స్థిరరాశి hb_సదిశ_t &o) స్థిరరాశి { తిరిగి !(*ఇది == o); }
  uint32_t hash () స్థిరరాశి { తిరిగి as_array ().hash (); }

  Type& క్రియాకారకం [] (పూర్ణాంకం i_)
  {
    గుర్తులేని పూర్ణాంకం i = (గుర్తులేని పూర్ణాంకం) i_;
    ఒకవేళ (unlikely (i >= నిడివి))
      తిరిగి Crap (Type);
    తిరిగి arrayZ[i];
  }
  స్థిరరాశి Type& క్రియాకారకం [] (పూర్ణాంకం i_) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం i = (గుర్తులేని పూర్ణాంకం) i_;
    ఒకవేళ (unlikely (i >= నిడివి))
      తిరిగి Null (Type);
    తిరిగి arrayZ[i];
  }

  Type& తోక () { తిరిగి (*ఇది)[నిడివి - 1]; }
  స్థిరరాశి Type& తోక () స్థిరరాశి { తిరిగి (*ఇది)[నిడివి - 1]; }

  స్పష్టమైన క్రియాకారకం తార్కిక () స్థిరరాశి { తిరిగి నిడివి; }
  size_t get_కొలత () స్థిరరాశి { తిరిగి hb_unsigned_mul_saturate (నిడివి, అంశం_కొలత); }

  /* Sink interface. */
  మూస <రకం_పేరు T>
  hb_సదిశ_t& క్రియాకారకం << (T&& v) { push (std::forward<T> (v)); తిరిగి *ఇది; }

  array_t   as_array ()       { తిరిగి hb_array (arrayZ, నిడివి); }
  c_array_t as_array () స్థిరరాశి { తిరిగి hb_array (arrayZ, నిడివి); }

  /* Iterator. */
  రకనిర్వచనం c_array_t   iter_t;
  రకనిర్వచనం array_t   writer_t;
    iter_t   iter () స్థిరరాశి { తిరిగి as_array (); }
  writer_t writer ()       { తిరిగి as_array (); }
  క్రియాకారకం   iter_t () స్థిరరాశి { తిరిగి   iter (); }
  క్రియాకారకం writer_t ()       { తిరిగి writer (); }

  /* Faster range-based for loop. */
  Type *begin () స్థిరరాశి { తిరిగి arrayZ; }
  Type *చివర () స్థిరరాశి { తిరిగి arrayZ + నిడివి; }


  hb_sorted_array_t<Type> as_sorted_array ()
  { తిరిగి hb_sorted_array (arrayZ, నిడివి); }
  hb_sorted_array_t<స్థిరరాశి Type> as_sorted_array () స్థిరరాశి
  { తిరిగి hb_sorted_array (arrayZ, నిడివి); }

  మూస <రకం_పేరు T> స్పష్టమైన క్రియాకారకం T * () { తిరిగి arrayZ; }
  మూస <రకం_పేరు T> స్పష్టమైన క్రియాకారకం స్థిరరాశి T * () స్థిరరాశి { తిరిగి arrayZ; }

  Type * క్రియాకారకం  + (గుర్తులేని పూర్ణాంకం i) { తిరిగి arrayZ + i; }
  స్థిరరాశి Type * క్రియాకారకం  + (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి { తిరిగి arrayZ + i; }

  HB_ALWAYS_INLINE_సదిశ_ALLOCS
  Type *push ()
  {
    ఒకవేళ (unlikely (!resize (నిడివి + 1)))
      తిరిగి std::addressof (Crap (Type));
    తిరిగి std::addressof (arrayZ[నిడివి - 1]);
  }
  మూస <రకం_పేరు... వాదనలు_2>
  HB_ALWAYS_INLINE_సదిశ_ALLOCS
  Type *push (వాదనలు_2&&... వాదనలు)
  {
    ఒకవేళ (unlikely ((పూర్ణాంకం) నిడివి >= allocated))
    {
      ఒకవేళ (unlikely (లోపల_error ()))
	తిరిగి std::addressof (Crap (Type));

      /* An argument may live in our own storage, eg. v.push (v[i]).
       * alloc() frees that storage, so copy it out first. */
      ఒకవేళ (unlikely (లోపల_storage (వాదనలు...)))
      {
	Type copy (std::forward<వాదనలు_2> (వాదనలు)...);
	ఒకవేళ (unlikely (!alloc (నిడివి + 1)))
	  తిరిగి std::addressof (Crap (Type));
	తిరిగి నెట్టు_has_స్థలం (std::move (copy));
      }
      ఒకవేళ (unlikely (!alloc (నిడివి + 1)))
	// If push failed to allocate then don't copy v, since this may cause
	// the created copy to leak memory since we won't have stored a
	// reference to it.
	తిరిగి std::addressof (Crap (Type));
    }

    తిరిగి నెట్టు_has_స్థలం (std::forward<వాదనలు_2> (వాదనలు)...);
  }
  మూస <రకం_పేరు... వాదనలు_2>
  HB_ALWAYS_INLINE_సదిశ_ALLOCS
  తార్కిక నెట్టు_or_fail (వాదనలు_2&&... వాదనలు)
  {
    తిరిగి push (std::forward<వాదనలు_2> (వాదనలు)...) != std::addressof (Crap (Type));
  }
  మూస <రకం_పేరు... వాదనలు_2>
  HB_ALWAYS_INLINE_సదిశ_ALLOCS
  Type *నెట్టు_has_స్థలం (వాదనలు_2&&... వాదనలు)
  {
    /* Emplace. */
    Type *p = std::addressof (arrayZ[నిడివి++]);
    తిరిగి కొత్త (p) Type (std::forward<వాదనలు_2> (వాదనలు)...);
  }

  తార్కిక లోపల_storage () స్థిరరాశి { తిరిగి అబద్ధం; }
  మూస <రకం_పేరు T, రకం_పేరు... Ts>
  తార్కిక లోపల_storage (స్థిరరాశి T &v, స్థిరరాశి Ts &...vs) స్థిరరాశి
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    తిరిగి (arrayZ &&
	    (uintptr_t) arrayZ <= p && p < (uintptr_t) (arrayZ + నిడివి)) ||
	   లోపల_storage (vs...);
  }

  తార్కిక is_owned () స్థిరరాశి
  {
    తిరిగి allocated != 0 && allocated != -1;
  }

  తార్కిక లోపల_error () స్థిరరాశి { తిరిగి allocated < 0; }
  శూన్య సమితి_error ()
  {
    ప్రతిజ్ఞా (allocated >= 0);
    allocated = -allocated - 1;
  }
  శూన్య reset_error ()
  {
    ప్రతిజ్ఞా (allocated < 0);
    allocated = -(allocated + 1);
  }
  శూన్య ensure_error ()
  {
    ఒకవేళ (!లోపల_error ())
      సమితి_error ();
  }

  Type *
  _realloc (గుర్తులేని new_allocated)
  {
    ఒకవేళ (!new_allocated)
    {
      ఒకవేళ (is_owned ())
	hb_free (arrayZ);
      తిరిగి శూన్యసూచిక;
    }
    ఒకవేళ (!allocated && arrayZ)
    {
      /* If we have a non-null arrayZ but allocated is 0, then we are
       * reallocating from a foreign array. */
      Type *new_array = (Type *) hb_malloc (new_allocated * పరిమాణం (Type));
      ఒకవేళ (unlikely (!new_array))
	తిరిగి శూన్యసూచిక;
      hb_memcpy ((శూన్య *) new_array, (స్థిరరాశి శూన్య *) arrayZ, నిడివి * పరిమాణం (Type));
      తిరిగి new_array;
    }
    తిరిగి (Type *) hb_realloc (arrayZ, new_allocated * పరిమాణం (Type));
  }
  Type *
  _malloc_move (గుర్తులేని new_allocated)
  {
    ఒకవేళ (!new_allocated)
    {
      ఒకవేళ (is_owned ())
	hb_free (arrayZ);
      తిరిగి శూన్యసూచిక;
    }
    Type *new_array = (Type *) hb_malloc (new_allocated * పరిమాణం (Type));
    ఒకవేళ (likely (new_array))
    {
      కొరకు (గుర్తులేని i = 0; i < నిడివి; i++)
      {
	కొత్త (std::addressof (new_array[i])) Type ();
	new_array[i] = std::move (arrayZ[i]);
	arrayZ[i].~Type ();
      }
      ఒకవేళ (is_owned ())
	hb_free (arrayZ);
    }
    తిరిగి new_array;
  }

  మూస <రకం_పేరు T = Type,
	    hb_enable_if (hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_సదిశ (గుర్తులేని new_allocated, hb_ప్రాధాన్యత<0>)
  {
    తిరిగి _realloc (new_allocated);
  }
  మూస <రకం_పేరు T = Type,
	    hb_enable_if (!hb_is_trivially_copy_assignable(T))>
  Type *
  realloc_సదిశ (గుర్తులేని new_allocated, hb_ప్రాధాన్యత<0>)
  {
    తిరిగి _malloc_move (new_allocated);
  }
  /* Specialization for types that can be moved using realloc(). */
  మూస <రకం_పేరు T = Type,
	    hb_enable_if (T::realloc_move)>
  Type *
  realloc_సదిశ (గుర్తులేని new_allocated, hb_ప్రాధాన్యత<1>)
  {
    తిరిగి _realloc (new_allocated);
  }

  మూస <రకం_పేరు T = Type,
	    hb_enable_if (hb_is_trivially_constructible(T))>
  శూన్య
  grow_సదిశ (గుర్తులేని కొలత, hb_ప్రాధాన్యత<0>)
  {
    hb_memset (arrayZ + నిడివి, 0, (కొలత - నిడివి) * పరిమాణం (*arrayZ));
    నిడివి = కొలత;
  }
  మూస <రకం_పేరు T = Type,
	    hb_enable_if (!hb_is_trivially_constructible(T))>
  శూన్య
  grow_సదిశ (గుర్తులేని కొలత, hb_ప్రాధాన్యత<0>)
  {
    కొరకు (; నిడివి < కొలత; నిడివి++)
      కొత్త (std::addressof (arrayZ[నిడివి])) Type ();
  }
  /* Specialization for hb_vector_t<hb_{vector,array}_t<U>> to speed up. */
  మూస <రకం_పేరు T = Type,
	    hb_enable_if (hb_is_same (T, hb_సదిశ_t<రకం_పేరు T::అంశం_t>) ||
			  hb_is_same (T, hb_array_t <రకం_పేరు T::అంశం_t>))>
  శూన్య
  grow_సదిశ (గుర్తులేని కొలత, hb_ప్రాధాన్యత<1>)
  {
    hb_memset (arrayZ + నిడివి, 0, (కొలత - నిడివి) * పరిమాణం (*arrayZ));
    నిడివి = కొలత;
  }

  మూస <రకం_పేరు T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  శూన్య
  copy_array (hb_array_t<Type> other)
  {
    hb_memcpy ((శూన్య *) (arrayZ + నిడివి), (స్థిరరాశి శూన్య *) other.arrayZ, other.నిడివి * అంశం_కొలత);
    నిడివి += other.నిడివి;
  }
  మూస <రకం_పేరు T = Type,
	    hb_enable_if (hb_is_trivially_copyable (T))>
  శూన్య
  copy_array (hb_array_t<స్థిరరాశి Type> other)
  {
    hb_memcpy ((శూన్య *) (arrayZ + నిడివి), (స్థిరరాశి శూన్య *) other.arrayZ, other.నిడివి * అంశం_కొలత);
    నిడివి += other.నిడివి;
  }
  మూస <రకం_పేరు T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			   std::is_copy_constructible<T>::value)>
  శూన్య
  copy_array (hb_array_t<స్థిరరాశి Type> other)
  {
    కొరకు (గుర్తులేని i = 0; i < other.నిడివి; i++)
      కొత్త (std::addressof (arrayZ[నిడివి + i])) Type (other.arrayZ[i]);
    నిడివి += other.నిడివి;
  }
  మూస <రకం_పేరు T = Type,
	    hb_enable_if (!hb_is_trivially_copyable (T) &&
			  !std::is_copy_constructible<T>::value &&
			  std::is_default_constructible<T>::value &&
			  std::is_copy_assignable<T>::value)>
  శూన్య
  copy_array (hb_array_t<స్థిరరాశి Type> other)
  {
    కొరకు (గుర్తులేని i = 0; i < other.నిడివి; i++)
    {
      కొత్త (std::addressof (arrayZ[నిడివి + i])) Type ();
      arrayZ[నిడివి + i] = other.arrayZ[i];
    }
    నిడివి += other.నిడివి;
  }

  శూన్య
  shrink_సదిశ (గుర్తులేని కొలత)
  {
    ప్రతిజ్ఞా (కొలత <= నిడివి);
    ఒకవేళ (!hb_is_trivially_destructible(Type))
    {
      గుర్తులేని లెక్క = నిడివి - కొలత;
      Type *p = arrayZ + నిడివి;
      అయితే (లెక్క--)
        (--p)->~Type ();
    }
    నిడివి = కొలత;
  }

  శూన్య
  జరుపు_కిందికి_సదిశ (గుర్తులేని i)
  {
    కొరకు (; i < నిడివి; i++)
      arrayZ[i - 1] = std::move (arrayZ[i]);
  }

  /* Allocate for size but don't adjust length. */
  HB_ALWAYS_INLINE_సదిశ_ALLOCS
  తార్కిక alloc (గుర్తులేని పూర్ణాంకం కొలత, తార్కిక exact=అబద్ధం)
  {
    ఒకవేళ (unlikely (లోపల_error ()))
      తిరిగి అబద్ధం;

    గుర్తులేని పూర్ణాంకం new_allocated;
    ఒకవేళ (exact)
    {
      /* If exact was specified, we allow shrinking the storage. */
      కొలత = hb_max (కొలత, నిడివి);
      ఒకవేళ (కొలత <= (గుర్తులేని) allocated &&
	  కొలత >= (గుర్తులేని) allocated >> 2)
	తిరిగి నిజం;

      new_allocated = కొలత;
    }
    లేకపోతే
    {
      ఒకవేళ (likely (కొలత <= (గుర్తులేని) allocated))
	తిరిగి నిజం;

      new_allocated = allocated;
      అయితే (కొలత > new_allocated)
	new_allocated += (new_allocated >> 1) + 8;
    }

    /* Reallocate */

    తార్కిక overflows =
      (పూర్ణాంకం) లోపల_error () ||
      (new_allocated < కొలత) ||
      hb_unsigned_mul_overflows (new_allocated, పరిమాణం (Type));

    ఒకవేళ (unlikely (overflows))
    {
      సమితి_error ();
      తిరిగి అబద్ధం;
    }

    Type *new_array = realloc_సదిశ (new_allocated, hb_prioritize);

    ఒకవేళ (unlikely (new_allocated && !new_array))
    {
      ఒకవేళ (new_allocated <= (గుర్తులేని) allocated)
        తిరిగి నిజం; // shrinking failed; it's okay; happens in our fuzzer

      సమితి_error ();
      తిరిగి అబద్ధం;
    }

    arrayZ = new_array;
    allocated = new_allocated;

    తిరిగి నిజం;
  }
  HB_ALWAYS_INLINE_సదిశ_ALLOCS
  తార్కిక alloc_exact (గుర్తులేని పూర్ణాంకం కొలత)
  {
    తిరిగి alloc (కొలత, నిజం);
  }

  HB_ALWAYS_INLINE_సదిశ_ALLOCS
  శూన్య clear ()
  {
    shrink_సదిశ (0);
  }

  మూస <రకం_పేరు allocator_t>
  HB_ALWAYS_INLINE_సదిశ_ALLOCS
  తార్కిక allocate_from_pool (allocator_t *allocator, గుర్తులేని కొలత, గుర్తులేని పూర్ణాంకం initialize = నిజం)
  {
    ఒకవేళ (allocator)
    {
      ప్రతిజ్ఞా (!నిడివి && !allocated);
      arrayZ = (Type *) allocator->alloc (కొలత * పరిమాణం (Type), alignof (Type));
      ఒకవేళ (unlikely (!arrayZ))
      {
	సమితి_error ();
	తిరిగి అబద్ధం;
      }
      ఒకవేళ (initialize)
	grow_సదిశ (కొలత, hb_prioritize);
      లేకపోతే
	నిడివి = కొలత;
      తిరిగి నిజం;
    }
    తిరిగి resize_నిండిన ((పూర్ణాంకం) కొలత, initialize, నిజం);
  }

  మూస <రకం_పేరు allocator_t>
  HB_ALWAYS_INLINE_సదిశ_ALLOCS
  తార్కిక duplicate_సదిశ_from_pool (allocator_t *allocator, స్థిరరాశి hb_సదిశ_t &other)
  {
    ఒకవేళ (unlikely (!allocate_from_pool (allocator, other.నిడివి, అబద్ధం)))
      తిరిగి అబద్ధం;
    నిడివి = 0;
    copy_array (other.as_array ());
    తిరిగి నిజం;
  }

  మూస <రకం_పేరు allocator_t>
  శూన్య shrink_వెనుక_గమ్యం_pool (allocator_t *allocator, పూర్ణాంకం కొలత)
  {
    గుర్తులేని orig_నిడివి = నిడివి;

    shrink (కొలత, అబద్ధం);

    ఒకవేళ (allocator && !is_owned ())
      allocator->discard (arrayZ + నిడివి, (orig_నిడివి - నిడివి) * పరిమాణం (Type));
  }

  HB_ALWAYS_INLINE_సదిశ_ALLOCS
  తార్కిక resize_నిండిన (పూర్ణాంకం కొలత_3, తార్కిక initialize, తార్కిక exact)
  {
    ఒకవేళ (unlikely (కొలత_3 < 0))
      తిరిగి అబద్ధం;
    గుర్తులేని పూర్ణాంకం కొలత = (గుర్తులేని పూర్ణాంకం) కొలత_3;
    ఒకవేళ (!alloc (కొలత, exact))
      తిరిగి అబద్ధం;

    ఒకవేళ (కొలత > నిడివి)
    {
      ఒకవేళ (initialize)
	grow_సదిశ (కొలత, hb_prioritize);
    }
    లేకపోతే ఒకవేళ (కొలత < నిడివి)
    {
      ఒకవేళ (initialize)
	shrink_సదిశ (కొలత);
    }

    నిడివి = కొలత;
    తిరిగి నిజం;
  }
  HB_ALWAYS_INLINE_సదిశ_ALLOCS
  తార్కిక resize (పూర్ణాంకం కొలత_3)
  {
    తిరిగి resize_నిండిన (కొలత_3, నిజం, అబద్ధం);
  }
  HB_ALWAYS_INLINE_సదిశ_ALLOCS
  తార్కిక resize_dirty (పూర్ణాంకం కొలత_3)
  {
    తిరిగి resize_నిండిన (కొలత_3, అబద్ధం, అబద్ధం);
  }
  HB_ALWAYS_INLINE_సదిశ_ALLOCS
  తార్కిక resize_exact (పూర్ణాంకం కొలత_3)
  {
    తిరిగి resize_నిండిన (కొలత_3, నిజం, నిజం);
  }

  Type తీయి ()
  {
    ఒకవేళ (!నిడివి) తిరిగి Null (Type);
    Type v (std::move (arrayZ[నిడివి - 1]));
    arrayZ[నిడివి - 1].~Type ();
    నిడివి--;
    తిరిగి v;
  }

  శూన్య remove_ordered (గుర్తులేని పూర్ణాంకం i)
  {
    ఒకవేళ (unlikely (i >= నిడివి))
      తిరిగి;
    జరుపు_కిందికి_సదిశ (i + 1);
    arrayZ[నిడివి - 1].~Type ();
    నిడివి--;
  }

  మూస <తార్కిక Sorted = sorted,
	    hb_enable_if (!Sorted)>
  శూన్య remove_unordered (గుర్తులేని పూర్ణాంకం i)
  {
    ఒకవేళ (unlikely (i >= నిడివి))
      తిరిగి;
    ఒకవేళ (i != నిడివి - 1)
      arrayZ[i] = std::move (arrayZ[నిడివి - 1]);
    arrayZ[నిడివి - 1].~Type ();
    నిడివి--;
  }

  శూన్య shrink (పూర్ణాంకం కొలత_3, తార్కిక shrink_జ్ఞాపకం = నిజం)
  {
    గుర్తులేని పూర్ణాంకం కొలత = కొలత_3 < 0 ? 0u : (గుర్తులేని పూర్ణాంకం) కొలత_3;
    ఒకవేళ (కొలత >= నిడివి)
      తిరిగి;

    shrink_సదిశ (కొలత);

    ఒకవేళ (is_owned () && shrink_జ్ఞాపకం)
      alloc_exact (కొలత); /* To force shrinking memory if needed. */
  }


  /* Sorting API. */
  మూస <రకం_పేరు Compar>
  శూన్య qsort (Compar compar)
  { as_array ().qsort (compar); }
  శూన్య qsort ()
  { as_array ().qsort (); }

  /* Unsorted search API. */
  మూస <రకం_పేరు T>
  Type *lsearch (స్థిరరాశి T &x, Type *not_దొరికింది = శూన్యసూచిక)
  { తిరిగి as_array ().lsearch (x, not_దొరికింది); }
  మూస <రకం_పేరు T>
  స్థిరరాశి Type *lsearch (స్థిరరాశి T &x, స్థిరరాశి Type *not_దొరికింది = శూన్యసూచిక) స్థిరరాశి
  { తిరిగి as_array ().lsearch (x, not_దొరికింది); }
  మూస <రకం_పేరు T>
  తార్కిక lfind (స్థిరరాశి T &x, గుర్తులేని *pos = శూన్యసూచిక) స్థిరరాశి
  { తిరిగి as_array ().lfind (x, pos); }

  /* Sorted search API. */
  మూస <రకం_పేరు T,
	    తార్కిక Sorted=sorted, hb_enable_if (Sorted)>
  Type *bsearch (స్థిరరాశి T &x, Type *not_దొరికింది = శూన్యసూచిక)
  { తిరిగి as_array ().bsearch (x, not_దొరికింది); }
  మూస <రకం_పేరు T,
	    తార్కిక Sorted=sorted, hb_enable_if (Sorted)>
  స్థిరరాశి Type *bsearch (స్థిరరాశి T &x, స్థిరరాశి Type *not_దొరికింది = శూన్యసూచిక) స్థిరరాశి
  { తిరిగి as_array ().bsearch (x, not_దొరికింది); }
  మూస <రకం_పేరు T,
	    తార్కిక Sorted=sorted, hb_enable_if (Sorted)>
  తార్కిక bfind (స్థిరరాశి T &x, గుర్తులేని పూర్ణాంకం *i = శూన్యసూచిక,
	      hb_not_దొరికింది_t not_దొరికింది = HB_NOT_దొరికింది_DONT_STORE,
	      గుర్తులేని పూర్ణాంకం గమ్యం_store = (గుర్తులేని పూర్ణాంకం) -1) స్థిరరాశి
  { తిరిగి as_array ().bfind (x, i, not_దొరికింది, గమ్యం_store); }
};

మూస <రకం_పేరు Type>
ఉపయోగించి hb_sorted_సదిశ_t = hb_సదిశ_t<Type, నిజం>;

#అయితే_ముగింపు /* HB_VECTOR_HH */

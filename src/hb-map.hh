/*
 * Copyright © 2018  Google, Inc.
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

#جيڪڏهن_وصف_نه_ٿيل HB_MAP_HH
#وصف_ڪريو HB_MAP_HH

#ضمن "hb.hh"

#ضمن "hb-set.hh"


/*
 * hb_hashmap_t
 */

خارجي HB_اندروني ثابت hb_codepoint_t minus_1;

سانچو <قسم_نالو K, قسم_نالو V,
	  منطقي minus_one = خطأ>
بنية hb_hashmap_t
{
  ساكن constexpr منطقي realloc_move = سچ;

  hb_hashmap_t ()  { init (); }
  ~hb_hashmap_t () { fini (); }

  فراغ _copy (ثابت hb_hashmap_t& o)
  {
    إذا (unlikely (!o.successful))
    {
      successful = خطأ;
      أرجع;
    }
    إذا (unlikely (!o.نقاب)) أرجع;

    إذا (hb_is_trivially_copy_assignable (شي_t))
    {
      شيون = (شي_t *) hb_malloc2 ((o.نقاب + 1), حجم (شي_t));
      إذا (unlikely (!شيون))
      {
	successful = خطأ;
	أرجع;
      }
      population = o.population;
      occupancy = o.occupancy;
      نقاب = o.نقاب;
      مفرد = o.مفرد;
      max_chain_ڊيگهائي = o.max_chain_ڊيگهائي;
      يادگيري_نقل (شيون, o.شيون, حجم (شي_t) * (نقاب + 1));
      أرجع;
    }

    alloc (o.population); hb_copy (o, *هذا);
  }

  hb_hashmap_t (ثابت hb_hashmap_t& o) : hb_hashmap_t () { _copy (o); }
  hb_hashmap_t& عامل= (ثابت hb_hashmap_t& o)
  {
    reset ();
    إذا (!شيون) { _copy (o); أرجع *هذا; }
    alloc (o.population); hb_copy (o, *هذا); أرجع *هذا;
  }

  hb_hashmap_t (hb_hashmap_t&& o)  noexcept : hb_hashmap_t () { hb_مٽا_سٽا (*هذا, o); }
  hb_hashmap_t& عامل= (hb_hashmap_t&& o)   noexcept { hb_مٽا_سٽا (*هذا, o); أرجع *هذا; }

  hb_hashmap_t (std::initializer_list<hb_pair_t<K, V>> lst) : hb_hashmap_t ()
  {
    لكل (خودڪار&& شي : lst)
      سيٽ (شي.پهريون, شي.second);
  }
  سانچو <قسم_نالو Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_hashmap_t (ثابت Iterable &o) : hb_hashmap_t ()
  {
    خودڪار iter = hb_iter (o);
    إذا (iter.is_random_access_iterator || iter.has_fast_len)
      alloc (hb_len (iter));
    hb_copy (iter, *هذا);
  }

  بنية شي_t
  {
    K key;
    uint32_t is_real_ : 1;
    uint32_t is_استعمال_ٿيل : 1;
    uint32_t hash : 30;
    V value;

    شي_t () : key (),
		is_real_ (خطأ), is_استعمال_ٿيل (خطأ),
		hash (0),
		value () {}

    // Needed for https://github.com/harfbuzz/harfbuzz/issues/4138
    K& get_ڪنجي () { أرجع key; }
    V& get_قدر () { أرجع value; }

    منطقي is_استعمال_ٿيل_2 () ثابت { أرجع is_استعمال_ٿيل; }
    فراغ سيٽ_استعمال_ٿيل (منطقي is_استعمال_ٿيل_2) { is_استعمال_ٿيل = is_استعمال_ٿيل_2; }
    فراغ سيٽ_real (منطقي is_real) { is_real_ = is_real; }
    منطقي is_real () ثابت { أرجع is_real_; }

    سانچو <منطقي v = minus_one,
	      hb_enable_if (v == خطأ)>
    ساكن مضمن ثابت V& default_قدر () { أرجع Null(V); };
    سانچو <منطقي v = minus_one,
	      hb_enable_if (v == سچ)>
    ساكن مضمن ثابت V& default_قدر ()
    {
      static_assert (hb_is_same (V, hb_codepoint_t), "");
      أرجع minus_1;
    };

    منطقي عامل == (ثابت K &o) ثابت { أرجع hb_deref (key) == hb_deref (o); }
    منطقي عامل == (ثابت شي_t &o) ثابت { أرجع *هذا == o.key; }
    hb_pair_t<K, V> get_pair() ثابت { أرجع hb_pair_t<K, V> (key, value); }
    hb_pair_t<ثابت K &, V &> get_pair_ref() { أرجع hb_pair_t<ثابت K &, V &> (key, value); }

    uint32_t ڪل_hash () ثابت
    { أرجع (hash * 31u) + hb_hash (value); }

    ساكن constexpr منطقي is_trivially_constructible = (hb_is_trivially_constructible(K) && hb_is_trivially_constructible(V));
  };

  hb_object_سرو_t سرو;
  منطقي successful; /* Allocations successful */
  بنانشان ننڍو max_chain_ڊيگهائي;
  بنانشان صحيح population; /* Not including tombstones. */
  بنانشان صحيح occupancy; /* Including tombstones. */
  بنانشان صحيح نقاب;
  بنانشان صحيح مفرد;
  شي_t *شيون;

  دوست فراغ swap (hb_hashmap_t& a, hb_hashmap_t& b) noexcept
  {
    إذا (unlikely (!a.successful || !b.successful))
      أرجع;
    hb_مٽا_سٽا (a.max_chain_ڊيگهائي, b.max_chain_ڊيگهائي);
    hb_مٽا_سٽا (a.population, b.population);
    hb_مٽا_سٽا (a.occupancy, b.occupancy);
    hb_مٽا_سٽا (a.نقاب, b.نقاب);
    hb_مٽا_سٽا (a.مفرد, b.مفرد);
    hb_مٽا_سٽا (a.شيون, b.شيون);
  }
  فراغ init ()
  {
    hb_object_init (هذا);

    successful = سچ;
    max_chain_ڊيگهائي = 0;
    population = occupancy = 0;
    نقاب = 0;
    مفرد = 0;
    شيون = خالي_اشارو;
  }
  فراغ fini ()
  {
    hb_object_fini (هذا);

    إذا (likely (شيون))
    {
      بنانشان سائيز = نقاب + 1;
      لكل (بنانشان i = 0; i < سائيز; i++)
	شيون[i].~شي_t ();
      hb_free (شيون);
      شيون = خالي_اشارو;
    }
    population = occupancy = 0;
  }

  hb_hashmap_t& reset ()
  {
    successful = سچ;
    clear ();
    أرجع *هذا;
  }

  منطقي اندريون_error () ثابت { أرجع !successful; }

  منطقي alloc (بنانشان new_population = 0)
  {
    إذا (unlikely (!successful)) أرجع خطأ;
    إذا (unlikely (population > 0x3FFFFFFFu || new_population > 0x3FFFFFFFu))
    {
      // Population sizes >0x3FFFFFFF will result in power
      // being larger than 31, which in turn leads to a new size
      // that overflows u32.
      successful = خطأ;
      أرجع خطأ;
    }

    إذا (new_population != 0 && (new_population + new_population / 2) < نقاب) أرجع سچ;

    بنانشان صحيح power = hb_ٻٽو_انگ_storage (hb_max (hb_max ((بنانشان) population, new_population) * 2, 4u));
    بنانشان صحيح new_سائيز = 1u << power;
    شي_t *new_شيون = (شي_t *) hb_malloc2 ((size_t) new_سائيز, حجم (شي_t));
    إذا (unlikely (!new_شيون))
    {
      successful = خطأ;
      أرجع خطأ;
    }
    إذا (!شي_t::is_trivially_constructible)
      لكل (خودڪار &_ : hb_iter (new_شيون, new_سائيز))
	جديد (&_) شي_t ();
    وإلا
      hb_memset (new_شيون, 0, (size_t) new_سائيز * حجم (شي_t));

    بنانشان صحيح old_سائيز = سائيز ();
    شي_t *old_شيون = شيون;

    /* Switch to new, empty, array. */
    population = occupancy = 0;
    نقاب = new_سائيز - 1;
    مفرد = مفرد_for (power);
    max_chain_ڊيگهائي = power * 2;
    شيون = new_شيون;

    /* Insert back old items. */
    لكل (بنانشان صحيح i = 0; i < old_سائيز; i++)
    {
      إذا (old_شيون[i].is_real ())
      {
	سيٽ_with_hash (std::move (old_شيون[i].key),
		       old_شيون[i].hash,
		       std::move (old_شيون[i].value));
      }
    }
    لكل (بنانشان صحيح i = 0; i < old_سائيز; i++)
      old_شيون[i].~شي_t ();

    hb_free (old_شيون);

    أرجع سچ;
  }

  سانچو <قسم_نالو T>
  منطقي اندريون_storage (ثابت T &v) ثابت
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    أرجع شيون &&
	   (uintptr_t) شيون <= p && p < (uintptr_t) (شيون + سائيز ());
  }

  سانچو <قسم_نالو KK, قسم_نالو VV>
  منطقي سيٽ_with_hash (KK&& key, uint32_t hash, VV&& value, منطقي overwrite = سچ)
  {
    إذا (unlikely (!successful)) أرجع خطأ;
    إذا (unlikely ((occupancy + occupancy / 2) >= نقاب))
    {
      /* key or value may live in our own storage, eg. m.set (k, m.get (j)).
       * alloc() frees that storage, so copy them out first. */
      إذا (unlikely (اندريون_storage (key) || اندريون_storage (value)))
      {
	K ڪنجي_copy;
	ڪنجي_copy = std::forward<KK> (key);
	V قدر_copy;
	قدر_copy = std::forward<VV> (value);
	أرجع سيٽ_with_hash (std::move (ڪنجي_copy), hash, std::move (قدر_copy), overwrite);
      }
      إذا (unlikely (!alloc ())) أرجع خطأ;
    }

    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    بنانشان صحيح tombstone = (بنانشان صحيح) -1;
    بنانشان صحيح i = hash % مفرد;
    بنانشان ڊيگهائي = 0;
    بنانشان قدم = 0;
    طالما (شيون[i].is_استعمال_ٿيل_2 ())
    {
      إذا ((std::is_integral<K>::value || شيون[i].hash == hash) &&
	  شيون[i] == key)
      {
        إذا (!overwrite)
	  أرجع خطأ;
        وإلا
	  اكسر;
      }
      إذا (!شيون[i].is_real () && tombstone == (بنانشان) -1)
        tombstone = i;
      i = (i + ++قدم) & نقاب;
      ڊيگهائي++;
    }

    شي_t &شي = شيون[tombstone == (بنانشان) -1 ? i : tombstone];

    إذا (شي.is_استعمال_ٿيل_2 ())
    {
      occupancy--;
      population -= شي.is_real ();
    }

    شي.key = std::forward<KK> (key);
    شي.value = std::forward<VV> (value);
    شي.hash = hash;
    شي.سيٽ_استعمال_ٿيل (سچ);
    شي.سيٽ_real (سچ);

    occupancy++;
    population++;

    إذا (unlikely (ڊيگهائي > max_chain_ڊيگهائي) && occupancy > نقاب / 8)
      alloc (نقاب - 8); // This ensures we jump to next larger size

    أرجع سچ;
  }

  سانچو <قسم_نالو VV>
  منطقي سيٽ (ثابت K &key, VV&& value, منطقي overwrite = سچ) { أرجع سيٽ_with_hash (key, hb_hash (key), std::forward<VV> (value), overwrite); }
  سانچو <قسم_نالو VV>
  منطقي سيٽ (K &&key, VV&& value, منطقي overwrite = سچ)
  {
    uint32_t hash = hb_hash (key);
    أرجع سيٽ_with_hash (std::move (key), hash, std::forward<VV> (value), overwrite);
  }
  منطقي جوڙيو (ثابت K &key)
  {
    uint32_t hash = hb_hash (key);
    أرجع سيٽ_with_hash (key, hash, شي_t::default_قدر ());
  }

  ثابت V& get_with_hash (ثابت K &key, uint32_t hash) ثابت
  {
    إذا (!شيون) أرجع شي_t::default_قدر ();
    خودڪار *شي = fetch_شي (key, hash);
    إذا (شي)
      أرجع شي->value;
    أرجع شي_t::default_قدر ();
  }
  ثابت V& get (ثابت K &key) ثابت
  {
    إذا (!شيون) أرجع شي_t::default_قدر ();
    أرجع get_with_hash (key, hb_hash (key));
  }

  فراغ del (ثابت K &key)
  {
    إذا (!شيون) أرجع;
    خودڪار *شي = fetch_شي (key, hb_hash (key));
    إذا (شي)
    {
      شي->سيٽ_real (خطأ);
      population--;
    }
  }

  /* Has interface. */
  ثابت V& عامل [] (K k) ثابت { أرجع get (k); }
  سانچو <قسم_نالو VV=V>
  منطقي has (ثابت K &key, VV **vp = خالي_اشارو) ثابت
  {
    إذا (!شيون) أرجع خطأ;
    أرجع has_with_hash (key, hb_hash (key), vp);
  }
  سانچو <قسم_نالو VV=V>
  منطقي has_with_hash (ثابت K &key, uint32_t hash, VV **vp = خالي_اشارو) ثابت
  {
    إذا (!شيون) أرجع خطأ;
    خودڪار *شي = fetch_شي (key, hash);
    إذا (شي)
    {
      إذا (vp) *vp = std::addressof (شي->value);
      أرجع سچ;
    }
    أرجع خطأ;
  }
  شي_t *fetch_شي (ثابت K &key, uint32_t hash) ثابت
  {
    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    بنانشان صحيح i = hash % مفرد;
    بنانشان قدم = 0;
    طالما (شيون[i].is_استعمال_ٿيل_2 ())
    {
      إذا ((std::is_integral<K>::value || شيون[i].hash == hash) &&
	  شيون[i] == key)
      {
	إذا (شيون[i].is_real ())
	  أرجع &شيون[i];
	وإلا
	  أرجع خالي_اشارو;
      }
      i = (i + ++قدم) & نقاب;
    }
    أرجع خالي_اشارو;
  }
  /* Projection. */
  ثابت V& عامل () (K k) ثابت { أرجع get (k); }

  بنانشان سائيز () ثابت { أرجع نقاب ? نقاب + 1 : 0; }

  فراغ clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (zero-initialized) from any writes.  Any non-empty hashmap
     * is a real heap instance with writable items, so clearing
     * under !successful is safe. */
    إذا (!population && !occupancy) أرجع;

    لكل (خودڪار &_ : hb_iter (شيون, سائيز ()))
    {
      /* Reconstruct items. */
      _.~شي_t ();
      جديد (&_) شي_t ();
    }

    population = occupancy = 0;
  }

  منطقي is_empty () ثابت { أرجع population == 0; }
  واضح عامل منطقي () ثابت { أرجع !is_empty (); }

  uint32_t hash () ثابت
  {
    أرجع
    + iter_شيون ()
    | hb_reduce ([] (uint32_t h, ثابت شي_t &_) { أرجع h ^ _.ڪل_hash (); }, (uint32_t) 0u)
    ;
  }

  منطقي is_equal (ثابت hb_hashmap_t &other) ثابت
  {
    إذا (population != other.population) أرجع خطأ;

    لكل (خودڪار pair : iter ())
      إذا (other.get (pair.پهريون) != pair.second)
        أرجع خطأ;

    أرجع سچ;
  }
  منطقي عامل == (ثابت hb_hashmap_t &other) ثابت { أرجع is_equal (other); }
  منطقي عامل != (ثابت hb_hashmap_t &other) ثابت { أرجع !is_equal (other); }

  بنانشان صحيح get_population () ثابت { أرجع population; }

  فراغ update (ثابت hb_hashmap_t &other)
  {
    إذا (unlikely (!successful)) أرجع;

    hb_copy (other, *هذا);
  }

  /*
   * Iterator
   */

  خودڪار iter_شيون () ثابت HB_AUTO_RETURN
  (
    + hb_iter (شيون, هذا->سائيز ())
    | hb_filter (&شي_t::is_real)
  )
  خودڪار iter_ref () ثابت HB_AUTO_RETURN
  (
    + هذا->iter_شيون ()
    | hb_map (&شي_t::get_pair_ref)
  )
  خودڪار iter () ثابت HB_AUTO_RETURN
  (
    + هذا->iter_شيون ()
    | hb_map (&شي_t::get_pair)
  )
  خودڪار ڪنجيون_ref () ثابت HB_AUTO_RETURN
  (
    + هذا->iter_شيون ()
    | hb_map (&شي_t::get_ڪنجي)
  )
  خودڪار ڪنجيون () ثابت HB_AUTO_RETURN
  (
    + هذا->ڪنجيون_ref ()
    | hb_map (hb_ridentity)
  )
  خودڪار قدرون_ref () ثابت HB_AUTO_RETURN
  (
    + هذا->iter_شيون ()
    | hb_map (&شي_t::get_قدر)
  )
  خودڪار values () ثابت HB_AUTO_RETURN
  (
    + هذا->قدرون_ref ()
    | hb_map (hb_ridentity)
  )

  /* C iterator. */
  منطقي next (صحيح *idx,
	     K *key,
	     V *value) ثابت
  {
    بنانشان i = (بنانشان) (*idx + 1);

    بنانشان ڳڻپ = سائيز ();
    طالما (i < ڳڻپ && !شيون[i].is_real ())
      i++;

    إذا (i >= ڳڻپ)
    {
      *idx = -1;
      أرجع خطأ;
    }

    *key = شيون[i].key;
    *value = شيون[i].value;

    *idx = (نشانوارو) i;
    أرجع سچ;
  }

  /* Sink interface. */
  hb_hashmap_t& عامل << (ثابت hb_pair_t<K, V>& v)
  { سيٽ (v.پهريون, v.second); أرجع *هذا; }
  سانچو <قسم_نالو V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (V2))>
  hb_hashmap_t& عامل << (ثابت hb_pair_t<K, V&&>& v)
  { سيٽ (v.پهريون, std::move (v.second)); أرجع *هذا; }
  سانچو <قسم_نالو K2 = K,
	    hb_enable_if (!hb_is_trivially_copyable (K2))>
  hb_hashmap_t& عامل << (ثابت hb_pair_t<K&&, V>& v)
  { سيٽ (std::move (v.پهريون), v.second); أرجع *هذا; }
  سانچو <قسم_نالو K2 = K, قسم_نالو V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (K2) &&
			  !hb_is_trivially_copyable (V2))>
  hb_hashmap_t& عامل << (ثابت hb_pair_t<K&&, V&&>& v)
  { سيٽ (std::move (v.پهريون), std::move (v.second)); أرجع *هذا; }

  ساكن بنانشان صحيح مفرد_for (بنانشان صحيح سرڪايو)
  {
    /* Following comment and table copied from glib. */
    /* Each table size has an associated prime modulo (the first prime
     * lower than the table size) used to find the initial bucket. Probing
     * then works modulo 2^n. The prime modulo is necessary to get a
     * good distribution with poor hash functions.
     */
    /* Not declaring static to make all kinds of compilers happy... */
    /*static*/ ثابت بنانشان صحيح مفرد_mod [32] =
    {
      1,          /* For 1 << 0 */
      2,
      3,
      7,
      13,
      31,
      61,
      127,
      251,
      509,
      1021,
      2039,
      4093,
      8191,
      16381,
      32749,
      65521,      /* For 1 << 16 */
      131071,
      262139,
      524287,
      1048573,
      2097143,
      4194301,
      8388593,
      16777213,
      33554393,
      67108859,
      134217689,
      268435399,
      536870909,
      1073741789,
      2147483647  /* For 1 << 31 */
    };

    إذا (unlikely (سرڪايو >= ARRAY_ڊيگهائي (مفرد_mod)))
      أرجع مفرد_mod[ARRAY_ڊيگهائي (مفرد_mod) - 1];

    أرجع مفرد_mod[سرڪايو];
  }
};

/*
 * hb_map_t
 */

بنية hb_map_t : hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       سچ>
{
  استعمال hashmap = hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       سچ>;

  ~hb_map_t () = افتراضي;
  hb_map_t () : hashmap () {}
  hb_map_t (ثابت hb_map_t &o) : hashmap ((hashmap &) o) {}
  hb_map_t (hb_map_t &&o)  noexcept : hashmap (std::move ((hashmap &) o)) {}
  hb_map_t& عامل= (ثابت hb_map_t&) = افتراضي;
  hb_map_t& عامل= (hb_map_t&&) = افتراضي;
  hb_map_t (std::initializer_list<hb_codepoint_pair_t> lst) : hashmap (lst) {}
  سانچو <قسم_نالو Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_map_t (ثابت Iterable &o) : hashmap (o) {}
};


#جيڪڏهن_پورو /* HB_MAP_HH */

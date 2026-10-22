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

#که_نه_تعریف_شوی HB_MAP_HH
#تعریف_کړه HB_MAP_HH

#ضمن "hb.hh"

#ضمن "hb-set.hh"


/*
 * hb_hashmap_t
 */

خارجي HB_دننی ثابت hb_codepoint_t minus_1;

کينډۍ <ډول_نوم K, ډول_نوم V,
	  منطقي minus_one = خطأ>
بنية hb_hashmap_t
{
  ساكن constexpr منطقي realloc_move = ریښتیا;

  hb_hashmap_t ()  { init (); }
  ~hb_hashmap_t () { fini (); }

  فراغ _copy (ثابت hb_hashmap_t& o)
  {
    إذا (unlikely (!o.successful))
    {
      successful = خطأ;
      أرجع;
    }
    إذا (unlikely (!o.ماسک)) أرجع;

    إذا (hb_is_trivially_copy_assignable (توکی_t))
    {
      توکي = (توکی_t *) hb_malloc2 ((o.ماسک + 1), حجم (توکی_t));
      إذا (unlikely (!توکي))
      {
	successful = خطأ;
	أرجع;
      }
      population = o.population;
      occupancy = o.occupancy;
      ماسک = o.ماسک;
      اولي = o.اولي;
      max_chain_اوږدتیا = o.max_chain_اوږدتیا;
      حافظه_نقل (توکي, o.توکي, حجم (توکی_t) * (ماسک + 1));
      أرجع;
    }

    alloc (o.population); hb_copy (o, *هذا);
  }

  hb_hashmap_t (ثابت hb_hashmap_t& o) : hb_hashmap_t () { _copy (o); }
  hb_hashmap_t& چلوونکی= (ثابت hb_hashmap_t& o)
  {
    reset ();
    إذا (!توکي) { _copy (o); أرجع *هذا; }
    alloc (o.population); hb_copy (o, *هذا); أرجع *هذا;
  }

  hb_hashmap_t (hb_hashmap_t&& o)  noexcept : hb_hashmap_t () { hb_بدلون (*هذا, o); }
  hb_hashmap_t& چلوونکی= (hb_hashmap_t&& o)   noexcept { hb_بدلون (*هذا, o); أرجع *هذا; }

  hb_hashmap_t (std::initializer_list<hb_pair_t<K, V>> lst) : hb_hashmap_t ()
  {
    لكل (خپلکاری&& توکی : lst)
      سېټ (توکی.لومړی, توکی.second);
  }
  کينډۍ <ډول_نوم Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_hashmap_t (ثابت Iterable &o) : hb_hashmap_t ()
  {
    خپلکاری iter = hb_iter (o);
    إذا (iter.is_random_access_iterator || iter.has_fast_len)
      alloc (hb_len (iter));
    hb_copy (iter, *هذا);
  }

  بنية توکی_t
  {
    K key;
    uint32_t is_real_ : 1;
    uint32_t is_کارول_شوی : 1;
    uint32_t hash : 30;
    V value;

    توکی_t () : key (),
		is_real_ (خطأ), is_کارول_شوی (خطأ),
		hash (0),
		value () {}

    // Needed for https://github.com/harfbuzz/harfbuzz/issues/4138
    K& get_کلی () { أرجع key; }
    V& get_ارزښت () { أرجع value; }

    منطقي is_کارول_شوی_2 () ثابت { أرجع is_کارول_شوی; }
    فراغ سېټ_کارول_شوی (منطقي is_کارول_شوی_2) { is_کارول_شوی = is_کارول_شوی_2; }
    فراغ سېټ_real (منطقي is_real) { is_real_ = is_real; }
    منطقي is_real () ثابت { أرجع is_real_; }

    کينډۍ <منطقي v = minus_one,
	      hb_enable_if (v == خطأ)>
    ساكن مضمن ثابت V& default_ارزښت () { أرجع Null(V); };
    کينډۍ <منطقي v = minus_one,
	      hb_enable_if (v == ریښتیا)>
    ساكن مضمن ثابت V& default_ارزښت ()
    {
      static_assert (hb_is_same (V, hb_codepoint_t), "");
      أرجع minus_1;
    };

    منطقي چلوونکی == (ثابت K &o) ثابت { أرجع hb_deref (key) == hb_deref (o); }
    منطقي چلوونکی == (ثابت توکی_t &o) ثابت { أرجع *هذا == o.key; }
    hb_pair_t<K, V> get_pair() ثابت { أرجع hb_pair_t<K, V> (key, value); }
    hb_pair_t<ثابت K &, V &> get_pair_ref() { أرجع hb_pair_t<ثابت K &, V &> (key, value); }

    uint32_t ټول_hash () ثابت
    { أرجع (hash * 31u) + hb_hash (value); }

    ساكن constexpr منطقي is_trivially_constructible = (hb_is_trivially_constructible(K) && hb_is_trivially_constructible(V));
  };

  hb_object_سرلیک_t سرلیک;
  منطقي successful; /* Allocations successful */
  بینښه لنډ max_chain_اوږدتیا;
  بینښه صحيح population; /* Not including tombstones. */
  بینښه صحيح occupancy; /* Including tombstones. */
  بینښه صحيح ماسک;
  بینښه صحيح اولي;
  توکی_t *توکي;

  ملګری فراغ swap (hb_hashmap_t& a, hb_hashmap_t& b) noexcept
  {
    إذا (unlikely (!a.successful || !b.successful))
      أرجع;
    hb_بدلون (a.max_chain_اوږدتیا, b.max_chain_اوږدتیا);
    hb_بدلون (a.population, b.population);
    hb_بدلون (a.occupancy, b.occupancy);
    hb_بدلون (a.ماسک, b.ماسک);
    hb_بدلون (a.اولي, b.اولي);
    hb_بدلون (a.توکي, b.توکي);
  }
  فراغ init ()
  {
    hb_object_init (هذا);

    successful = ریښتیا;
    max_chain_اوږدتیا = 0;
    population = occupancy = 0;
    ماسک = 0;
    اولي = 0;
    توکي = تش_اشاره;
  }
  فراغ fini ()
  {
    hb_object_fini (هذا);

    إذا (likely (توکي))
    {
      بینښه اندازه = ماسک + 1;
      لكل (بینښه i = 0; i < اندازه; i++)
	توکي[i].~توکی_t ();
      hb_free (توکي);
      توکي = تش_اشاره;
    }
    population = occupancy = 0;
  }

  hb_hashmap_t& reset ()
  {
    successful = ریښتیا;
    clear ();
    أرجع *هذا;
  }

  منطقي دننه_error () ثابت { أرجع !successful; }

  منطقي alloc (بینښه new_population = 0)
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

    إذا (new_population != 0 && (new_population + new_population / 2) < ماسک) أرجع ریښتیا;

    بینښه صحيح power = hb_بټ_storage (hb_max (hb_max ((بینښه) population, new_population) * 2, 4u));
    بینښه صحيح new_اندازه = 1u << power;
    توکی_t *new_توکي = (توکی_t *) hb_malloc2 ((size_t) new_اندازه, حجم (توکی_t));
    إذا (unlikely (!new_توکي))
    {
      successful = خطأ;
      أرجع خطأ;
    }
    إذا (!توکی_t::is_trivially_constructible)
      لكل (خپلکاری &_ : hb_iter (new_توکي, new_اندازه))
	جديد (&_) توکی_t ();
    وإلا
      hb_memset (new_توکي, 0, (size_t) new_اندازه * حجم (توکی_t));

    بینښه صحيح old_اندازه = اندازه ();
    توکی_t *old_توکي = توکي;

    /* Switch to new, empty, array. */
    population = occupancy = 0;
    ماسک = new_اندازه - 1;
    اولي = اولي_for (power);
    max_chain_اوږدتیا = power * 2;
    توکي = new_توکي;

    /* Insert back old items. */
    لكل (بینښه صحيح i = 0; i < old_اندازه; i++)
    {
      إذا (old_توکي[i].is_real ())
      {
	سېټ_with_hash (std::move (old_توکي[i].key),
		       old_توکي[i].hash,
		       std::move (old_توکي[i].value));
      }
    }
    لكل (بینښه صحيح i = 0; i < old_اندازه; i++)
      old_توکي[i].~توکی_t ();

    hb_free (old_توکي);

    أرجع ریښتیا;
  }

  کينډۍ <ډول_نوم T>
  منطقي دننه_storage (ثابت T &v) ثابت
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    أرجع توکي &&
	   (uintptr_t) توکي <= p && p < (uintptr_t) (توکي + اندازه ());
  }

  کينډۍ <ډول_نوم KK, ډول_نوم VV>
  منطقي سېټ_with_hash (KK&& key, uint32_t hash, VV&& value, منطقي overwrite = ریښتیا)
  {
    إذا (unlikely (!successful)) أرجع خطأ;
    إذا (unlikely ((occupancy + occupancy / 2) >= ماسک))
    {
      /* key or value may live in our own storage, eg. m.set (k, m.get (j)).
       * alloc() frees that storage, so copy them out first. */
      إذا (unlikely (دننه_storage (key) || دننه_storage (value)))
      {
	K کلی_copy;
	کلی_copy = std::forward<KK> (key);
	V ارزښت_copy;
	ارزښت_copy = std::forward<VV> (value);
	أرجع سېټ_with_hash (std::move (کلی_copy), hash, std::move (ارزښت_copy), overwrite);
      }
      إذا (unlikely (!alloc ())) أرجع خطأ;
    }

    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    بینښه صحيح tombstone = (بینښه صحيح) -1;
    بینښه صحيح i = hash % اولي;
    بینښه اوږدتیا = 0;
    بینښه ګام = 0;
    طالما (توکي[i].is_کارول_شوی_2 ())
    {
      إذا ((std::is_integral<K>::value || توکي[i].hash == hash) &&
	  توکي[i] == key)
      {
        إذا (!overwrite)
	  أرجع خطأ;
        وإلا
	  اكسر;
      }
      إذا (!توکي[i].is_real () && tombstone == (بینښه) -1)
        tombstone = i;
      i = (i + ++ګام) & ماسک;
      اوږدتیا++;
    }

    توکی_t &توکی = توکي[tombstone == (بینښه) -1 ? i : tombstone];

    إذا (توکی.is_کارول_شوی_2 ())
    {
      occupancy--;
      population -= توکی.is_real ();
    }

    توکی.key = std::forward<KK> (key);
    توکی.value = std::forward<VV> (value);
    توکی.hash = hash;
    توکی.سېټ_کارول_شوی (ریښتیا);
    توکی.سېټ_real (ریښتیا);

    occupancy++;
    population++;

    إذا (unlikely (اوږدتیا > max_chain_اوږدتیا) && occupancy > ماسک / 8)
      alloc (ماسک - 8); // This ensures we jump to next larger size

    أرجع ریښتیا;
  }

  کينډۍ <ډول_نوم VV>
  منطقي سېټ (ثابت K &key, VV&& value, منطقي overwrite = ریښتیا) { أرجع سېټ_with_hash (key, hb_hash (key), std::forward<VV> (value), overwrite); }
  کينډۍ <ډول_نوم VV>
  منطقي سېټ (K &&key, VV&& value, منطقي overwrite = ریښتیا)
  {
    uint32_t hash = hb_hash (key);
    أرجع سېټ_with_hash (std::move (key), hash, std::forward<VV> (value), overwrite);
  }
  منطقي جمع_کړه (ثابت K &key)
  {
    uint32_t hash = hb_hash (key);
    أرجع سېټ_with_hash (key, hash, توکی_t::default_ارزښت ());
  }

  ثابت V& get_with_hash (ثابت K &key, uint32_t hash) ثابت
  {
    إذا (!توکي) أرجع توکی_t::default_ارزښت ();
    خپلکاری *توکی = fetch_توکی (key, hash);
    إذا (توکی)
      أرجع توکی->value;
    أرجع توکی_t::default_ارزښت ();
  }
  ثابت V& get (ثابت K &key) ثابت
  {
    إذا (!توکي) أرجع توکی_t::default_ارزښت ();
    أرجع get_with_hash (key, hb_hash (key));
  }

  فراغ del (ثابت K &key)
  {
    إذا (!توکي) أرجع;
    خپلکاری *توکی = fetch_توکی (key, hb_hash (key));
    إذا (توکی)
    {
      توکی->سېټ_real (خطأ);
      population--;
    }
  }

  /* Has interface. */
  ثابت V& چلوونکی [] (K k) ثابت { أرجع get (k); }
  کينډۍ <ډول_نوم VV=V>
  منطقي has (ثابت K &key, VV **vp = تش_اشاره) ثابت
  {
    إذا (!توکي) أرجع خطأ;
    أرجع has_with_hash (key, hb_hash (key), vp);
  }
  کينډۍ <ډول_نوم VV=V>
  منطقي has_with_hash (ثابت K &key, uint32_t hash, VV **vp = تش_اشاره) ثابت
  {
    إذا (!توکي) أرجع خطأ;
    خپلکاری *توکی = fetch_توکی (key, hash);
    إذا (توکی)
    {
      إذا (vp) *vp = std::addressof (توکی->value);
      أرجع ریښتیا;
    }
    أرجع خطأ;
  }
  توکی_t *fetch_توکی (ثابت K &key, uint32_t hash) ثابت
  {
    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    بینښه صحيح i = hash % اولي;
    بینښه ګام = 0;
    طالما (توکي[i].is_کارول_شوی_2 ())
    {
      إذا ((std::is_integral<K>::value || توکي[i].hash == hash) &&
	  توکي[i] == key)
      {
	إذا (توکي[i].is_real ())
	  أرجع &توکي[i];
	وإلا
	  أرجع تش_اشاره;
      }
      i = (i + ++ګام) & ماسک;
    }
    أرجع تش_اشاره;
  }
  /* Projection. */
  ثابت V& چلوونکی () (K k) ثابت { أرجع get (k); }

  بینښه اندازه () ثابت { أرجع ماسک ? ماسک + 1 : 0; }

  فراغ clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (zero-initialized) from any writes.  Any non-empty hashmap
     * is a real heap instance with writable items, so clearing
     * under !successful is safe. */
    إذا (!population && !occupancy) أرجع;

    لكل (خپلکاری &_ : hb_iter (توکي, اندازه ()))
    {
      /* Reconstruct items. */
      _.~توکی_t ();
      جديد (&_) توکی_t ();
    }

    population = occupancy = 0;
  }

  منطقي is_empty () ثابت { أرجع population == 0; }
  څرګند چلوونکی منطقي () ثابت { أرجع !is_empty (); }

  uint32_t hash () ثابت
  {
    أرجع
    + iter_توکي ()
    | hb_reduce ([] (uint32_t h, ثابت توکی_t &_) { أرجع h ^ _.ټول_hash (); }, (uint32_t) 0u)
    ;
  }

  منطقي is_equal (ثابت hb_hashmap_t &other) ثابت
  {
    إذا (population != other.population) أرجع خطأ;

    لكل (خپلکاری pair : iter ())
      إذا (other.get (pair.لومړی) != pair.second)
        أرجع خطأ;

    أرجع ریښتیا;
  }
  منطقي چلوونکی == (ثابت hb_hashmap_t &other) ثابت { أرجع is_equal (other); }
  منطقي چلوونکی != (ثابت hb_hashmap_t &other) ثابت { أرجع !is_equal (other); }

  بینښه صحيح get_population () ثابت { أرجع population; }

  فراغ update (ثابت hb_hashmap_t &other)
  {
    إذا (unlikely (!successful)) أرجع;

    hb_copy (other, *هذا);
  }

  /*
   * Iterator
   */

  خپلکاری iter_توکي () ثابت HB_AUTO_RETURN
  (
    + hb_iter (توکي, هذا->اندازه ())
    | hb_filter (&توکی_t::is_real)
  )
  خپلکاری iter_ref () ثابت HB_AUTO_RETURN
  (
    + هذا->iter_توکي ()
    | hb_map (&توکی_t::get_pair_ref)
  )
  خپلکاری iter () ثابت HB_AUTO_RETURN
  (
    + هذا->iter_توکي ()
    | hb_map (&توکی_t::get_pair)
  )
  خپلکاری کلیګانې_ref () ثابت HB_AUTO_RETURN
  (
    + هذا->iter_توکي ()
    | hb_map (&توکی_t::get_کلی)
  )
  خپلکاری کلیګانې () ثابت HB_AUTO_RETURN
  (
    + هذا->کلیګانې_ref ()
    | hb_map (hb_ridentity)
  )
  خپلکاری ارزښتونه_ref () ثابت HB_AUTO_RETURN
  (
    + هذا->iter_توکي ()
    | hb_map (&توکی_t::get_ارزښت)
  )
  خپلکاری values () ثابت HB_AUTO_RETURN
  (
    + هذا->ارزښتونه_ref ()
    | hb_map (hb_ridentity)
  )

  /* C iterator. */
  منطقي next (صحيح *idx,
	     K *key,
	     V *value) ثابت
  {
    بینښه i = (بینښه) (*idx + 1);

    بینښه شمېر = اندازه ();
    طالما (i < شمېر && !توکي[i].is_real ())
      i++;

    إذا (i >= شمېر)
    {
      *idx = -1;
      أرجع خطأ;
    }

    *key = توکي[i].key;
    *value = توکي[i].value;

    *idx = (نښهدار) i;
    أرجع ریښتیا;
  }

  /* Sink interface. */
  hb_hashmap_t& چلوونکی << (ثابت hb_pair_t<K, V>& v)
  { سېټ (v.لومړی, v.second); أرجع *هذا; }
  کينډۍ <ډول_نوم V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (V2))>
  hb_hashmap_t& چلوونکی << (ثابت hb_pair_t<K, V&&>& v)
  { سېټ (v.لومړی, std::move (v.second)); أرجع *هذا; }
  کينډۍ <ډول_نوم K2 = K,
	    hb_enable_if (!hb_is_trivially_copyable (K2))>
  hb_hashmap_t& چلوونکی << (ثابت hb_pair_t<K&&, V>& v)
  { سېټ (std::move (v.لومړی), v.second); أرجع *هذا; }
  کينډۍ <ډول_نوم K2 = K, ډول_نوم V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (K2) &&
			  !hb_is_trivially_copyable (V2))>
  hb_hashmap_t& چلوونکی << (ثابت hb_pair_t<K&&, V&&>& v)
  { سېټ (std::move (v.لومړی), std::move (v.second)); أرجع *هذا; }

  ساكن بینښه صحيح اولي_for (بینښه صحيح ښويدنه)
  {
    /* Following comment and table copied from glib. */
    /* Each table size has an associated prime modulo (the first prime
     * lower than the table size) used to find the initial bucket. Probing
     * then works modulo 2^n. The prime modulo is necessary to get a
     * good distribution with poor hash functions.
     */
    /* Not declaring static to make all kinds of compilers happy... */
    /*static*/ ثابت بینښه صحيح اولي_mod [32] =
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

    إذا (unlikely (ښويدنه >= ARRAY_اوږدتیا (اولي_mod)))
      أرجع اولي_mod[ARRAY_اوږدتیا (اولي_mod) - 1];

    أرجع اولي_mod[ښويدنه];
  }
};

/*
 * hb_map_t
 */

بنية hb_map_t : hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       ریښتیا>
{
  کارول hashmap = hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       ریښتیا>;

  ~hb_map_t () = افتراضي;
  hb_map_t () : hashmap () {}
  hb_map_t (ثابت hb_map_t &o) : hashmap ((hashmap &) o) {}
  hb_map_t (hb_map_t &&o)  noexcept : hashmap (std::move ((hashmap &) o)) {}
  hb_map_t& چلوونکی= (ثابت hb_map_t&) = افتراضي;
  hb_map_t& چلوونکی= (hb_map_t&&) = افتراضي;
  hb_map_t (std::initializer_list<hb_codepoint_pair_t> lst) : hashmap (lst) {}
  کينډۍ <ډول_نوم Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_map_t (ثابت Iterable &o) : hashmap (o) {}
};


#د_که_پای /* HB_MAP_HH */

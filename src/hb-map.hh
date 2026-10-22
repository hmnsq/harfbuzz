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

#اگر_تعریف_نشده HB_MAP_HH
#تعریف_کن HB_MAP_HH

#ضمن "hb.hh"

#ضمن "hb-set.hh"


/*
 * hb_hashmap_t
 */

خارجي HB_داخلی ثابت hb_codepoint_t minus_1;

قالب <نام_نوع K, نام_نوع V,
	  منطقي minus_one = خطأ>
بنية hb_hashmap_t
{
  ساكن constexpr منطقي realloc_move = درست;

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

    إذا (hb_is_trivially_copy_assignable (قلم_t))
    {
      اقلام = (قلم_t *) hb_malloc2 ((o.ماسک + 1), حجم (قلم_t));
      إذا (unlikely (!اقلام))
      {
	successful = خطأ;
	أرجع;
      }
      population = o.population;
      occupancy = o.occupancy;
      ماسک = o.ماسک;
      اول = o.اول;
      max_chain_درازی = o.max_chain_درازی;
      کپی_حافظه (اقلام, o.اقلام, حجم (قلم_t) * (ماسک + 1));
      أرجع;
    }

    alloc (o.population); hb_copy (o, *هذا);
  }

  hb_hashmap_t (ثابت hb_hashmap_t& o) : hb_hashmap_t () { _copy (o); }
  hb_hashmap_t& عملگر= (ثابت hb_hashmap_t& o)
  {
    reset ();
    إذا (!اقلام) { _copy (o); أرجع *هذا; }
    alloc (o.population); hb_copy (o, *هذا); أرجع *هذا;
  }

  hb_hashmap_t (hb_hashmap_t&& o)  noexcept : hb_hashmap_t () { hb_جابجا (*هذا, o); }
  hb_hashmap_t& عملگر= (hb_hashmap_t&& o)   noexcept { hb_جابجا (*هذا, o); أرجع *هذا; }

  hb_hashmap_t (std::initializer_list<hb_pair_t<K, V>> lst) : hb_hashmap_t ()
  {
    لكل (خودکار&& قلم : lst)
      مجموعه (قلم.اولی, قلم.second);
  }
  قالب <نام_نوع Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_hashmap_t (ثابت Iterable &o) : hb_hashmap_t ()
  {
    خودکار iter = hb_iter (o);
    إذا (iter.is_random_access_iterator || iter.has_fast_len)
      alloc (hb_len (iter));
    hb_copy (iter, *هذا);
  }

  بنية قلم_t
  {
    K key;
    uint32_t is_real_ : 1;
    uint32_t is_استفاده_شده : 1;
    uint32_t hash : 30;
    V value;

    قلم_t () : key (),
		is_real_ (خطأ), is_استفاده_شده (خطأ),
		hash (0),
		value () {}

    // Needed for https://github.com/harfbuzz/harfbuzz/issues/4138
    K& get_کلید () { أرجع key; }
    V& get_مقدار () { أرجع value; }

    منطقي is_استفاده_شده_2 () ثابت { أرجع is_استفاده_شده; }
    فراغ مجموعه_استفاده_شده (منطقي is_استفاده_شده_2) { is_استفاده_شده = is_استفاده_شده_2; }
    فراغ مجموعه_real (منطقي is_real) { is_real_ = is_real; }
    منطقي is_real () ثابت { أرجع is_real_; }

    قالب <منطقي v = minus_one,
	      hb_enable_if (v == خطأ)>
    ساكن مضمن ثابت V& default_مقدار () { أرجع Null(V); };
    قالب <منطقي v = minus_one,
	      hb_enable_if (v == درست)>
    ساكن مضمن ثابت V& default_مقدار ()
    {
      static_assert (hb_is_same (V, hb_codepoint_t), "");
      أرجع minus_1;
    };

    منطقي عملگر == (ثابت K &o) ثابت { أرجع hb_deref (key) == hb_deref (o); }
    منطقي عملگر == (ثابت قلم_t &o) ثابت { أرجع *هذا == o.key; }
    hb_pair_t<K, V> get_pair() ثابت { أرجع hb_pair_t<K, V> (key, value); }
    hb_pair_t<ثابت K &, V &> get_pair_ref() { أرجع hb_pair_t<ثابت K &, V &> (key, value); }

    uint32_t مجموع_hash () ثابت
    { أرجع (hash * 31u) + hb_hash (value); }

    ساكن constexpr منطقي is_trivially_constructible = (hb_is_trivially_constructible(K) && hb_is_trivially_constructible(V));
  };

  hb_object_سرآیند_t سرآیند;
  منطقي successful; /* Allocations successful */
  بیعلامت کوتاه max_chain_درازی;
  بیعلامت صحيح population; /* Not including tombstones. */
  بیعلامت صحيح occupancy; /* Including tombstones. */
  بیعلامت صحيح ماسک;
  بیعلامت صحيح اول;
  قلم_t *اقلام;

  دوست فراغ swap (hb_hashmap_t& a, hb_hashmap_t& b) noexcept
  {
    إذا (unlikely (!a.successful || !b.successful))
      أرجع;
    hb_جابجا (a.max_chain_درازی, b.max_chain_درازی);
    hb_جابجا (a.population, b.population);
    hb_جابجا (a.occupancy, b.occupancy);
    hb_جابجا (a.ماسک, b.ماسک);
    hb_جابجا (a.اول, b.اول);
    hb_جابجا (a.اقلام, b.اقلام);
  }
  فراغ init ()
  {
    hb_object_init (هذا);

    successful = درست;
    max_chain_درازی = 0;
    population = occupancy = 0;
    ماسک = 0;
    اول = 0;
    اقلام = اشاره_تهی;
  }
  فراغ fini ()
  {
    hb_object_fini (هذا);

    إذا (likely (اقلام))
    {
      بیعلامت اندازه = ماسک + 1;
      لكل (بیعلامت i = 0; i < اندازه; i++)
	اقلام[i].~قلم_t ();
      hb_free (اقلام);
      اقلام = اشاره_تهی;
    }
    population = occupancy = 0;
  }

  hb_hashmap_t& reset ()
  {
    successful = درست;
    clear ();
    أرجع *هذا;
  }

  منطقي درون_error () ثابت { أرجع !successful; }

  منطقي alloc (بیعلامت new_population = 0)
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

    إذا (new_population != 0 && (new_population + new_population / 2) < ماسک) أرجع درست;

    بیعلامت صحيح power = hb_بیت_storage (hb_max (hb_max ((بیعلامت) population, new_population) * 2, 4u));
    بیعلامت صحيح new_اندازه = 1u << power;
    قلم_t *new_اقلام = (قلم_t *) hb_malloc2 ((size_t) new_اندازه, حجم (قلم_t));
    إذا (unlikely (!new_اقلام))
    {
      successful = خطأ;
      أرجع خطأ;
    }
    إذا (!قلم_t::is_trivially_constructible)
      لكل (خودکار &_ : hb_iter (new_اقلام, new_اندازه))
	جديد (&_) قلم_t ();
    وإلا
      hb_memset (new_اقلام, 0, (size_t) new_اندازه * حجم (قلم_t));

    بیعلامت صحيح old_اندازه = اندازه ();
    قلم_t *old_اقلام = اقلام;

    /* Switch to new, empty, array. */
    population = occupancy = 0;
    ماسک = new_اندازه - 1;
    اول = اول_for (power);
    max_chain_درازی = power * 2;
    اقلام = new_اقلام;

    /* Insert back old items. */
    لكل (بیعلامت صحيح i = 0; i < old_اندازه; i++)
    {
      إذا (old_اقلام[i].is_real ())
      {
	مجموعه_with_hash (std::move (old_اقلام[i].key),
		       old_اقلام[i].hash,
		       std::move (old_اقلام[i].value));
      }
    }
    لكل (بیعلامت صحيح i = 0; i < old_اندازه; i++)
      old_اقلام[i].~قلم_t ();

    hb_free (old_اقلام);

    أرجع درست;
  }

  قالب <نام_نوع T>
  منطقي درون_storage (ثابت T &v) ثابت
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    أرجع اقلام &&
	   (uintptr_t) اقلام <= p && p < (uintptr_t) (اقلام + اندازه ());
  }

  قالب <نام_نوع KK, نام_نوع VV>
  منطقي مجموعه_with_hash (KK&& key, uint32_t hash, VV&& value, منطقي overwrite = درست)
  {
    إذا (unlikely (!successful)) أرجع خطأ;
    إذا (unlikely ((occupancy + occupancy / 2) >= ماسک))
    {
      /* key or value may live in our own storage, eg. m.set (k, m.get (j)).
       * alloc() frees that storage, so copy them out first. */
      إذا (unlikely (درون_storage (key) || درون_storage (value)))
      {
	K کلید_copy;
	کلید_copy = std::forward<KK> (key);
	V مقدار_copy;
	مقدار_copy = std::forward<VV> (value);
	أرجع مجموعه_with_hash (std::move (کلید_copy), hash, std::move (مقدار_copy), overwrite);
      }
      إذا (unlikely (!alloc ())) أرجع خطأ;
    }

    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    بیعلامت صحيح tombstone = (بیعلامت صحيح) -1;
    بیعلامت صحيح i = hash % اول;
    بیعلامت درازی = 0;
    بیعلامت گام = 0;
    طالما (اقلام[i].is_استفاده_شده_2 ())
    {
      إذا ((std::is_integral<K>::value || اقلام[i].hash == hash) &&
	  اقلام[i] == key)
      {
        إذا (!overwrite)
	  أرجع خطأ;
        وإلا
	  اكسر;
      }
      إذا (!اقلام[i].is_real () && tombstone == (بیعلامت) -1)
        tombstone = i;
      i = (i + ++گام) & ماسک;
      درازی++;
    }

    قلم_t &قلم = اقلام[tombstone == (بیعلامت) -1 ? i : tombstone];

    إذا (قلم.is_استفاده_شده_2 ())
    {
      occupancy--;
      population -= قلم.is_real ();
    }

    قلم.key = std::forward<KK> (key);
    قلم.value = std::forward<VV> (value);
    قلم.hash = hash;
    قلم.مجموعه_استفاده_شده (درست);
    قلم.مجموعه_real (درست);

    occupancy++;
    population++;

    إذا (unlikely (درازی > max_chain_درازی) && occupancy > ماسک / 8)
      alloc (ماسک - 8); // This ensures we jump to next larger size

    أرجع درست;
  }

  قالب <نام_نوع VV>
  منطقي مجموعه (ثابت K &key, VV&& value, منطقي overwrite = درست) { أرجع مجموعه_with_hash (key, hb_hash (key), std::forward<VV> (value), overwrite); }
  قالب <نام_نوع VV>
  منطقي مجموعه (K &&key, VV&& value, منطقي overwrite = درست)
  {
    uint32_t hash = hb_hash (key);
    أرجع مجموعه_with_hash (std::move (key), hash, std::forward<VV> (value), overwrite);
  }
  منطقي جمع_کن (ثابت K &key)
  {
    uint32_t hash = hb_hash (key);
    أرجع مجموعه_with_hash (key, hash, قلم_t::default_مقدار ());
  }

  ثابت V& get_with_hash (ثابت K &key, uint32_t hash) ثابت
  {
    إذا (!اقلام) أرجع قلم_t::default_مقدار ();
    خودکار *قلم = fetch_قلم (key, hash);
    إذا (قلم)
      أرجع قلم->value;
    أرجع قلم_t::default_مقدار ();
  }
  ثابت V& get (ثابت K &key) ثابت
  {
    إذا (!اقلام) أرجع قلم_t::default_مقدار ();
    أرجع get_with_hash (key, hb_hash (key));
  }

  فراغ del (ثابت K &key)
  {
    إذا (!اقلام) أرجع;
    خودکار *قلم = fetch_قلم (key, hb_hash (key));
    إذا (قلم)
    {
      قلم->مجموعه_real (خطأ);
      population--;
    }
  }

  /* Has interface. */
  ثابت V& عملگر [] (K k) ثابت { أرجع get (k); }
  قالب <نام_نوع VV=V>
  منطقي has (ثابت K &key, VV **vp = اشاره_تهی) ثابت
  {
    إذا (!اقلام) أرجع خطأ;
    أرجع has_with_hash (key, hb_hash (key), vp);
  }
  قالب <نام_نوع VV=V>
  منطقي has_with_hash (ثابت K &key, uint32_t hash, VV **vp = اشاره_تهی) ثابت
  {
    إذا (!اقلام) أرجع خطأ;
    خودکار *قلم = fetch_قلم (key, hash);
    إذا (قلم)
    {
      إذا (vp) *vp = std::addressof (قلم->value);
      أرجع درست;
    }
    أرجع خطأ;
  }
  قلم_t *fetch_قلم (ثابت K &key, uint32_t hash) ثابت
  {
    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    بیعلامت صحيح i = hash % اول;
    بیعلامت گام = 0;
    طالما (اقلام[i].is_استفاده_شده_2 ())
    {
      إذا ((std::is_integral<K>::value || اقلام[i].hash == hash) &&
	  اقلام[i] == key)
      {
	إذا (اقلام[i].is_real ())
	  أرجع &اقلام[i];
	وإلا
	  أرجع اشاره_تهی;
      }
      i = (i + ++گام) & ماسک;
    }
    أرجع اشاره_تهی;
  }
  /* Projection. */
  ثابت V& عملگر () (K k) ثابت { أرجع get (k); }

  بیعلامت اندازه () ثابت { أرجع ماسک ? ماسک + 1 : 0; }

  فراغ clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (zero-initialized) from any writes.  Any non-empty hashmap
     * is a real heap instance with writable items, so clearing
     * under !successful is safe. */
    إذا (!population && !occupancy) أرجع;

    لكل (خودکار &_ : hb_iter (اقلام, اندازه ()))
    {
      /* Reconstruct items. */
      _.~قلم_t ();
      جديد (&_) قلم_t ();
    }

    population = occupancy = 0;
  }

  منطقي is_empty () ثابت { أرجع population == 0; }
  صریح عملگر منطقي () ثابت { أرجع !is_empty (); }

  uint32_t hash () ثابت
  {
    أرجع
    + iter_اقلام ()
    | hb_reduce ([] (uint32_t h, ثابت قلم_t &_) { أرجع h ^ _.مجموع_hash (); }, (uint32_t) 0u)
    ;
  }

  منطقي is_equal (ثابت hb_hashmap_t &other) ثابت
  {
    إذا (population != other.population) أرجع خطأ;

    لكل (خودکار pair : iter ())
      إذا (other.get (pair.اولی) != pair.second)
        أرجع خطأ;

    أرجع درست;
  }
  منطقي عملگر == (ثابت hb_hashmap_t &other) ثابت { أرجع is_equal (other); }
  منطقي عملگر != (ثابت hb_hashmap_t &other) ثابت { أرجع !is_equal (other); }

  بیعلامت صحيح get_population () ثابت { أرجع population; }

  فراغ update (ثابت hb_hashmap_t &other)
  {
    إذا (unlikely (!successful)) أرجع;

    hb_copy (other, *هذا);
  }

  /*
   * Iterator
   */

  خودکار iter_اقلام () ثابت HB_AUTO_RETURN
  (
    + hb_iter (اقلام, هذا->اندازه ())
    | hb_filter (&قلم_t::is_real)
  )
  خودکار iter_ref () ثابت HB_AUTO_RETURN
  (
    + هذا->iter_اقلام ()
    | hb_map (&قلم_t::get_pair_ref)
  )
  خودکار iter () ثابت HB_AUTO_RETURN
  (
    + هذا->iter_اقلام ()
    | hb_map (&قلم_t::get_pair)
  )
  خودکار کلیدها_ref () ثابت HB_AUTO_RETURN
  (
    + هذا->iter_اقلام ()
    | hb_map (&قلم_t::get_کلید)
  )
  خودکار کلیدها () ثابت HB_AUTO_RETURN
  (
    + هذا->کلیدها_ref ()
    | hb_map (hb_ridentity)
  )
  خودکار مقادیر_ref () ثابت HB_AUTO_RETURN
  (
    + هذا->iter_اقلام ()
    | hb_map (&قلم_t::get_مقدار)
  )
  خودکار values () ثابت HB_AUTO_RETURN
  (
    + هذا->مقادیر_ref ()
    | hb_map (hb_ridentity)
  )

  /* C iterator. */
  منطقي next (صحيح *idx,
	     K *key,
	     V *value) ثابت
  {
    بیعلامت i = (بیعلامت) (*idx + 1);

    بیعلامت شمار = اندازه ();
    طالما (i < شمار && !اقلام[i].is_real ())
      i++;

    إذا (i >= شمار)
    {
      *idx = -1;
      أرجع خطأ;
    }

    *key = اقلام[i].key;
    *value = اقلام[i].value;

    *idx = (علامتدار) i;
    أرجع درست;
  }

  /* Sink interface. */
  hb_hashmap_t& عملگر << (ثابت hb_pair_t<K, V>& v)
  { مجموعه (v.اولی, v.second); أرجع *هذا; }
  قالب <نام_نوع V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (V2))>
  hb_hashmap_t& عملگر << (ثابت hb_pair_t<K, V&&>& v)
  { مجموعه (v.اولی, std::move (v.second)); أرجع *هذا; }
  قالب <نام_نوع K2 = K,
	    hb_enable_if (!hb_is_trivially_copyable (K2))>
  hb_hashmap_t& عملگر << (ثابت hb_pair_t<K&&, V>& v)
  { مجموعه (std::move (v.اولی), v.second); أرجع *هذا; }
  قالب <نام_نوع K2 = K, نام_نوع V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (K2) &&
			  !hb_is_trivially_copyable (V2))>
  hb_hashmap_t& عملگر << (ثابت hb_pair_t<K&&, V&&>& v)
  { مجموعه (std::move (v.اولی), std::move (v.second)); أرجع *هذا; }

  ساكن بیعلامت صحيح اول_for (بیعلامت صحيح لغزش)
  {
    /* Following comment and table copied from glib. */
    /* Each table size has an associated prime modulo (the first prime
     * lower than the table size) used to find the initial bucket. Probing
     * then works modulo 2^n. The prime modulo is necessary to get a
     * good distribution with poor hash functions.
     */
    /* Not declaring static to make all kinds of compilers happy... */
    /*static*/ ثابت بیعلامت صحيح اول_mod [32] =
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

    إذا (unlikely (لغزش >= ARRAY_درازی (اول_mod)))
      أرجع اول_mod[ARRAY_درازی (اول_mod) - 1];

    أرجع اول_mod[لغزش];
  }
};

/*
 * hb_map_t
 */

بنية hb_map_t : hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       درست>
{
  استفاده hashmap = hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       درست>;

  ~hb_map_t () = افتراضي;
  hb_map_t () : hashmap () {}
  hb_map_t (ثابت hb_map_t &o) : hashmap ((hashmap &) o) {}
  hb_map_t (hb_map_t &&o)  noexcept : hashmap (std::move ((hashmap &) o)) {}
  hb_map_t& عملگر= (ثابت hb_map_t&) = افتراضي;
  hb_map_t& عملگر= (hb_map_t&&) = افتراضي;
  hb_map_t (std::initializer_list<hb_codepoint_pair_t> lst) : hashmap (lst) {}
  قالب <نام_نوع Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_map_t (ثابت Iterable &o) : hashmap (o) {}
};


#پایان_اگر /* HB_MAP_HH */

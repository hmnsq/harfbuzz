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

#جے_بنا_تعریف HB_MAP_HH
#تعریف_کرو HB_MAP_HH

#ضمن "hb.hh"

#ضمن "hb-set.hh"


/*
 * hb_hashmap_t
 */

خارجي HB_اندرونی ثابت hb_codepoint_t minus_1;

سانچا <قسم_ناں K, قسم_ناں V,
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

    إذا (hb_is_trivially_copy_assignable (شے_t))
    {
      اشیاء = (شے_t *) hb_malloc2 ((o.نقاب + 1), حجم (شے_t));
      إذا (unlikely (!اشیاء))
      {
	successful = خطأ;
	أرجع;
      }
      population = o.population;
      occupancy = o.occupancy;
      نقاب = o.نقاب;
      مفرد = o.مفرد;
      max_chain_لمائی = o.max_chain_لمائی;
      حافظہ_نقل (اشیاء, o.اشیاء, حجم (شے_t) * (نقاب + 1));
      أرجع;
    }

    alloc (o.population); hb_copy (o, *هذا);
  }

  hb_hashmap_t (ثابت hb_hashmap_t& o) : hb_hashmap_t () { _copy (o); }
  hb_hashmap_t& عامل= (ثابت hb_hashmap_t& o)
  {
    reset ();
    إذا (!اشیاء) { _copy (o); أرجع *هذا; }
    alloc (o.population); hb_copy (o, *هذا); أرجع *هذا;
  }

  hb_hashmap_t (hb_hashmap_t&& o)  noexcept : hb_hashmap_t () { hb_وٹاندرا (*هذا, o); }
  hb_hashmap_t& عامل= (hb_hashmap_t&& o)   noexcept { hb_وٹاندرا (*هذا, o); أرجع *هذا; }

  hb_hashmap_t (std::initializer_list<hb_pair_t<K, V>> lst) : hb_hashmap_t ()
  {
    لكل (خودکار&& شے : lst)
      سیٹ (شے.پہلا, شے.second);
  }
  سانچا <قسم_ناں Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_hashmap_t (ثابت Iterable &o) : hb_hashmap_t ()
  {
    خودکار iter = hb_iter (o);
    إذا (iter.is_random_access_iterator || iter.has_fast_len)
      alloc (hb_len (iter));
    hb_copy (iter, *هذا);
  }

  بنية شے_t
  {
    K key;
    uint32_t is_real_ : 1;
    uint32_t is_ورتیا : 1;
    uint32_t hash : 30;
    V value;

    شے_t () : key (),
		is_real_ (خطأ), is_ورتیا (خطأ),
		hash (0),
		value () {}

    // Needed for https://github.com/harfbuzz/harfbuzz/issues/4138
    K& get_کلید () { أرجع key; }
    V& get_قدر () { أرجع value; }

    منطقي is_ورتیا_2 () ثابت { أرجع is_ورتیا; }
    فراغ سیٹ_ورتیا (منطقي is_ورتیا_2) { is_ورتیا = is_ورتیا_2; }
    فراغ سیٹ_real (منطقي is_real) { is_real_ = is_real; }
    منطقي is_real () ثابت { أرجع is_real_; }

    سانچا <منطقي v = minus_one,
	      hb_enable_if (v == خطأ)>
    ساكن مضمن ثابت V& default_قدر () { أرجع Null(V); };
    سانچا <منطقي v = minus_one,
	      hb_enable_if (v == سچ)>
    ساكن مضمن ثابت V& default_قدر ()
    {
      static_assert (hb_is_same (V, hb_codepoint_t), "");
      أرجع minus_1;
    };

    منطقي عامل == (ثابت K &o) ثابت { أرجع hb_deref (key) == hb_deref (o); }
    منطقي عامل == (ثابت شے_t &o) ثابت { أرجع *هذا == o.key; }
    hb_pair_t<K, V> get_pair() ثابت { أرجع hb_pair_t<K, V> (key, value); }
    hb_pair_t<ثابت K &, V &> get_pair_ref() { أرجع hb_pair_t<ثابت K &, V &> (key, value); }

    uint32_t کل_hash () ثابت
    { أرجع (hash * 31u) + hb_hash (value); }

    ساكن constexpr منطقي is_trivially_constructible = (hb_is_trivially_constructible(K) && hb_is_trivially_constructible(V));
  };

  hb_object_سرخی_t سرخی;
  منطقي successful; /* Allocations successful */
  بنانشانی چھوٹا max_chain_لمائی;
  بنانشانی صحيح population; /* Not including tombstones. */
  بنانشانی صحيح occupancy; /* Including tombstones. */
  بنانشانی صحيح نقاب;
  بنانشانی صحيح مفرد;
  شے_t *اشیاء;

  یار فراغ swap (hb_hashmap_t& a, hb_hashmap_t& b) noexcept
  {
    إذا (unlikely (!a.successful || !b.successful))
      أرجع;
    hb_وٹاندرا (a.max_chain_لمائی, b.max_chain_لمائی);
    hb_وٹاندرا (a.population, b.population);
    hb_وٹاندرا (a.occupancy, b.occupancy);
    hb_وٹاندرا (a.نقاب, b.نقاب);
    hb_وٹاندرا (a.مفرد, b.مفرد);
    hb_وٹاندرا (a.اشیاء, b.اشیاء);
  }
  فراغ init ()
  {
    hb_object_init (هذا);

    successful = سچ;
    max_chain_لمائی = 0;
    population = occupancy = 0;
    نقاب = 0;
    مفرد = 0;
    اشیاء = خالی_سوچک;
  }
  فراغ fini ()
  {
    hb_object_fini (هذا);

    إذا (likely (اشیاء))
    {
      بنانشانی ناپ = نقاب + 1;
      لكل (بنانشانی i = 0; i < ناپ; i++)
	اشیاء[i].~شے_t ();
      hb_free (اشیاء);
      اشیاء = خالی_سوچک;
    }
    population = occupancy = 0;
  }

  hb_hashmap_t& reset ()
  {
    successful = سچ;
    clear ();
    أرجع *هذا;
  }

  منطقي وچ_error () ثابت { أرجع !successful; }

  منطقي alloc (بنانشانی new_population = 0)
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

    بنانشانی صحيح power = hb_ثنائی_ہندسہ_storage (hb_max (hb_max ((بنانشانی) population, new_population) * 2, 4u));
    بنانشانی صحيح new_ناپ = 1u << power;
    شے_t *new_اشیاء = (شے_t *) hb_malloc2 ((size_t) new_ناپ, حجم (شے_t));
    إذا (unlikely (!new_اشیاء))
    {
      successful = خطأ;
      أرجع خطأ;
    }
    إذا (!شے_t::is_trivially_constructible)
      لكل (خودکار &_ : hb_iter (new_اشیاء, new_ناپ))
	جديد (&_) شے_t ();
    وإلا
      hb_memset (new_اشیاء, 0, (size_t) new_ناپ * حجم (شے_t));

    بنانشانی صحيح old_ناپ = ناپ ();
    شے_t *old_اشیاء = اشیاء;

    /* Switch to new, empty, array. */
    population = occupancy = 0;
    نقاب = new_ناپ - 1;
    مفرد = مفرد_for (power);
    max_chain_لمائی = power * 2;
    اشیاء = new_اشیاء;

    /* Insert back old items. */
    لكل (بنانشانی صحيح i = 0; i < old_ناپ; i++)
    {
      إذا (old_اشیاء[i].is_real ())
      {
	سیٹ_with_hash (std::move (old_اشیاء[i].key),
		       old_اشیاء[i].hash,
		       std::move (old_اشیاء[i].value));
      }
    }
    لكل (بنانشانی صحيح i = 0; i < old_ناپ; i++)
      old_اشیاء[i].~شے_t ();

    hb_free (old_اشیاء);

    أرجع سچ;
  }

  سانچا <قسم_ناں T>
  منطقي وچ_storage (ثابت T &v) ثابت
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    أرجع اشیاء &&
	   (uintptr_t) اشیاء <= p && p < (uintptr_t) (اشیاء + ناپ ());
  }

  سانچا <قسم_ناں KK, قسم_ناں VV>
  منطقي سیٹ_with_hash (KK&& key, uint32_t hash, VV&& value, منطقي overwrite = سچ)
  {
    إذا (unlikely (!successful)) أرجع خطأ;
    إذا (unlikely ((occupancy + occupancy / 2) >= نقاب))
    {
      /* key or value may live in our own storage, eg. m.set (k, m.get (j)).
       * alloc() frees that storage, so copy them out first. */
      إذا (unlikely (وچ_storage (key) || وچ_storage (value)))
      {
	K کلید_copy;
	کلید_copy = std::forward<KK> (key);
	V قدر_copy;
	قدر_copy = std::forward<VV> (value);
	أرجع سیٹ_with_hash (std::move (کلید_copy), hash, std::move (قدر_copy), overwrite);
      }
      إذا (unlikely (!alloc ())) أرجع خطأ;
    }

    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    بنانشانی صحيح tombstone = (بنانشانی صحيح) -1;
    بنانشانی صحيح i = hash % مفرد;
    بنانشانی لمائی = 0;
    بنانشانی قدم = 0;
    طالما (اشیاء[i].is_ورتیا_2 ())
    {
      إذا ((std::is_integral<K>::value || اشیاء[i].hash == hash) &&
	  اشیاء[i] == key)
      {
        إذا (!overwrite)
	  أرجع خطأ;
        وإلا
	  اكسر;
      }
      إذا (!اشیاء[i].is_real () && tombstone == (بنانشانی) -1)
        tombstone = i;
      i = (i + ++قدم) & نقاب;
      لمائی++;
    }

    شے_t &شے = اشیاء[tombstone == (بنانشانی) -1 ? i : tombstone];

    إذا (شے.is_ورتیا_2 ())
    {
      occupancy--;
      population -= شے.is_real ();
    }

    شے.key = std::forward<KK> (key);
    شے.value = std::forward<VV> (value);
    شے.hash = hash;
    شے.سیٹ_ورتیا (سچ);
    شے.سیٹ_real (سچ);

    occupancy++;
    population++;

    إذا (unlikely (لمائی > max_chain_لمائی) && occupancy > نقاب / 8)
      alloc (نقاب - 8); // This ensures we jump to next larger size

    أرجع سچ;
  }

  سانچا <قسم_ناں VV>
  منطقي سیٹ (ثابت K &key, VV&& value, منطقي overwrite = سچ) { أرجع سیٹ_with_hash (key, hb_hash (key), std::forward<VV> (value), overwrite); }
  سانچا <قسم_ناں VV>
  منطقي سیٹ (K &&key, VV&& value, منطقي overwrite = سچ)
  {
    uint32_t hash = hb_hash (key);
    أرجع سیٹ_with_hash (std::move (key), hash, std::forward<VV> (value), overwrite);
  }
  منطقي جوڑو (ثابت K &key)
  {
    uint32_t hash = hb_hash (key);
    أرجع سیٹ_with_hash (key, hash, شے_t::default_قدر ());
  }

  ثابت V& get_with_hash (ثابت K &key, uint32_t hash) ثابت
  {
    إذا (!اشیاء) أرجع شے_t::default_قدر ();
    خودکار *شے = fetch_شے (key, hash);
    إذا (شے)
      أرجع شے->value;
    أرجع شے_t::default_قدر ();
  }
  ثابت V& get (ثابت K &key) ثابت
  {
    إذا (!اشیاء) أرجع شے_t::default_قدر ();
    أرجع get_with_hash (key, hb_hash (key));
  }

  فراغ del (ثابت K &key)
  {
    إذا (!اشیاء) أرجع;
    خودکار *شے = fetch_شے (key, hb_hash (key));
    إذا (شے)
    {
      شے->سیٹ_real (خطأ);
      population--;
    }
  }

  /* Has interface. */
  ثابت V& عامل [] (K k) ثابت { أرجع get (k); }
  سانچا <قسم_ناں VV=V>
  منطقي has (ثابت K &key, VV **vp = خالی_سوچک) ثابت
  {
    إذا (!اشیاء) أرجع خطأ;
    أرجع has_with_hash (key, hb_hash (key), vp);
  }
  سانچا <قسم_ناں VV=V>
  منطقي has_with_hash (ثابت K &key, uint32_t hash, VV **vp = خالی_سوچک) ثابت
  {
    إذا (!اشیاء) أرجع خطأ;
    خودکار *شے = fetch_شے (key, hash);
    إذا (شے)
    {
      إذا (vp) *vp = std::addressof (شے->value);
      أرجع سچ;
    }
    أرجع خطأ;
  }
  شے_t *fetch_شے (ثابت K &key, uint32_t hash) ثابت
  {
    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    بنانشانی صحيح i = hash % مفرد;
    بنانشانی قدم = 0;
    طالما (اشیاء[i].is_ورتیا_2 ())
    {
      إذا ((std::is_integral<K>::value || اشیاء[i].hash == hash) &&
	  اشیاء[i] == key)
      {
	إذا (اشیاء[i].is_real ())
	  أرجع &اشیاء[i];
	وإلا
	  أرجع خالی_سوچک;
      }
      i = (i + ++قدم) & نقاب;
    }
    أرجع خالی_سوچک;
  }
  /* Projection. */
  ثابت V& عامل () (K k) ثابت { أرجع get (k); }

  بنانشانی ناپ () ثابت { أرجع نقاب ? نقاب + 1 : 0; }

  فراغ clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (zero-initialized) from any writes.  Any non-empty hashmap
     * is a real heap instance with writable items, so clearing
     * under !successful is safe. */
    إذا (!population && !occupancy) أرجع;

    لكل (خودکار &_ : hb_iter (اشیاء, ناپ ()))
    {
      /* Reconstruct items. */
      _.~شے_t ();
      جديد (&_) شے_t ();
    }

    population = occupancy = 0;
  }

  منطقي is_empty () ثابت { أرجع population == 0; }
  واضح عامل منطقي () ثابت { أرجع !is_empty (); }

  uint32_t hash () ثابت
  {
    أرجع
    + iter_اشیاء ()
    | hb_reduce ([] (uint32_t h, ثابت شے_t &_) { أرجع h ^ _.کل_hash (); }, (uint32_t) 0u)
    ;
  }

  منطقي is_equal (ثابت hb_hashmap_t &other) ثابت
  {
    إذا (population != other.population) أرجع خطأ;

    لكل (خودکار pair : iter ())
      إذا (other.get (pair.پہلا) != pair.second)
        أرجع خطأ;

    أرجع سچ;
  }
  منطقي عامل == (ثابت hb_hashmap_t &other) ثابت { أرجع is_equal (other); }
  منطقي عامل != (ثابت hb_hashmap_t &other) ثابت { أرجع !is_equal (other); }

  بنانشانی صحيح get_population () ثابت { أرجع population; }

  فراغ update (ثابت hb_hashmap_t &other)
  {
    إذا (unlikely (!successful)) أرجع;

    hb_copy (other, *هذا);
  }

  /*
   * Iterator
   */

  خودکار iter_اشیاء () ثابت HB_AUTO_RETURN
  (
    + hb_iter (اشیاء, هذا->ناپ ())
    | hb_filter (&شے_t::is_real)
  )
  خودکار iter_ref () ثابت HB_AUTO_RETURN
  (
    + هذا->iter_اشیاء ()
    | hb_map (&شے_t::get_pair_ref)
  )
  خودکار iter () ثابت HB_AUTO_RETURN
  (
    + هذا->iter_اشیاء ()
    | hb_map (&شے_t::get_pair)
  )
  خودکار کلیدیں_ref () ثابت HB_AUTO_RETURN
  (
    + هذا->iter_اشیاء ()
    | hb_map (&شے_t::get_کلید)
  )
  خودکار کلیدیں () ثابت HB_AUTO_RETURN
  (
    + هذا->کلیدیں_ref ()
    | hb_map (hb_ridentity)
  )
  خودکار اقدار_ref () ثابت HB_AUTO_RETURN
  (
    + هذا->iter_اشیاء ()
    | hb_map (&شے_t::get_قدر)
  )
  خودکار values () ثابت HB_AUTO_RETURN
  (
    + هذا->اقدار_ref ()
    | hb_map (hb_ridentity)
  )

  /* C iterator. */
  منطقي next (صحيح *idx,
	     K *key,
	     V *value) ثابت
  {
    بنانشانی i = (بنانشانی) (*idx + 1);

    بنانشانی گنتی = ناپ ();
    طالما (i < گنتی && !اشیاء[i].is_real ())
      i++;

    إذا (i >= گنتی)
    {
      *idx = -1;
      أرجع خطأ;
    }

    *key = اشیاء[i].key;
    *value = اشیاء[i].value;

    *idx = (نشانیوالا) i;
    أرجع سچ;
  }

  /* Sink interface. */
  hb_hashmap_t& عامل << (ثابت hb_pair_t<K, V>& v)
  { سیٹ (v.پہلا, v.second); أرجع *هذا; }
  سانچا <قسم_ناں V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (V2))>
  hb_hashmap_t& عامل << (ثابت hb_pair_t<K, V&&>& v)
  { سیٹ (v.پہلا, std::move (v.second)); أرجع *هذا; }
  سانچا <قسم_ناں K2 = K,
	    hb_enable_if (!hb_is_trivially_copyable (K2))>
  hb_hashmap_t& عامل << (ثابت hb_pair_t<K&&, V>& v)
  { سیٹ (std::move (v.پہلا), v.second); أرجع *هذا; }
  سانچا <قسم_ناں K2 = K, قسم_ناں V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (K2) &&
			  !hb_is_trivially_copyable (V2))>
  hb_hashmap_t& عامل << (ثابت hb_pair_t<K&&, V&&>& v)
  { سیٹ (std::move (v.پہلا), std::move (v.second)); أرجع *هذا; }

  ساكن بنانشانی صحيح مفرد_for (بنانشانی صحيح کھسکاؤ)
  {
    /* Following comment and table copied from glib. */
    /* Each table size has an associated prime modulo (the first prime
     * lower than the table size) used to find the initial bucket. Probing
     * then works modulo 2^n. The prime modulo is necessary to get a
     * good distribution with poor hash functions.
     */
    /* Not declaring static to make all kinds of compilers happy... */
    /*static*/ ثابت بنانشانی صحيح مفرد_mod [32] =
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

    إذا (unlikely (کھسکاؤ >= ARRAY_لمائی (مفرد_mod)))
      أرجع مفرد_mod[ARRAY_لمائی (مفرد_mod) - 1];

    أرجع مفرد_mod[کھسکاؤ];
  }
};

/*
 * hb_map_t
 */

بنية hb_map_t : hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       سچ>
{
  ورتوں hashmap = hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       سچ>;

  ~hb_map_t () = افتراضي;
  hb_map_t () : hashmap () {}
  hb_map_t (ثابت hb_map_t &o) : hashmap ((hashmap &) o) {}
  hb_map_t (hb_map_t &&o)  noexcept : hashmap (std::move ((hashmap &) o)) {}
  hb_map_t& عامل= (ثابت hb_map_t&) = افتراضي;
  hb_map_t& عامل= (hb_map_t&&) = افتراضي;
  hb_map_t (std::initializer_list<hb_codepoint_pair_t> lst) : hashmap (lst) {}
  سانچا <قسم_ناں Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_map_t (ثابت Iterable &o) : hashmap (o) {}
};


#جے_انت /* HB_MAP_HH */

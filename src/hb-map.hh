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

#اگر_غیر_تعریف_شدہ HB_MAP_HH
#تعریف_کرو HB_MAP_HH

#شامل "hb.hh"

#شامل "hb-set.hh"


/*
 * hb_hashmap_t
 */

خارجی HB_اندرونی ثابت hb_codepoint_t minus_1;

سانچہ <قسم_نام K, قسم_نام V,
	  منطقی minus_one = جھوٹ>
ساخت hb_hashmap_t
{
  ساکن constexpr منطقی realloc_move = سچ;

  hb_hashmap_t ()  { init (); }
  ~hb_hashmap_t () { fini (); }

  خالی _copy (ثابت hb_hashmap_t& o)
  {
    اگر (unlikely (!o.successful))
    {
      successful = جھوٹ;
      واپس;
    }
    اگر (unlikely (!o.نقاب)) واپس;

    اگر (hb_is_trivially_copy_assignable (شے_t))
    {
      اشیاء = (شے_t *) hb_malloc2 ((o.نقاب + 1), حجم (شے_t));
      اگر (unlikely (!اشیاء))
      {
	successful = جھوٹ;
	واپس;
      }
      population = o.population;
      occupancy = o.occupancy;
      نقاب = o.نقاب;
      مفرد = o.مفرد;
      max_chain_لمبائی = o.max_chain_لمبائی;
      حافظہ_نقل (اشیاء, o.اشیاء, حجم (شے_t) * (نقاب + 1));
      واپس;
    }

    alloc (o.population); hb_copy (o, *یہ);
  }

  hb_hashmap_t (ثابت hb_hashmap_t& o) : hb_hashmap_t () { _copy (o); }
  hb_hashmap_t& عامل= (ثابت hb_hashmap_t& o)
  {
    reset ();
    اگر (!اشیاء) { _copy (o); واپس *یہ; }
    alloc (o.population); hb_copy (o, *یہ); واپس *یہ;
  }

  hb_hashmap_t (hb_hashmap_t&& o)  noexcept : hb_hashmap_t () { hb_تبادلہ (*یہ, o); }
  hb_hashmap_t& عامل= (hb_hashmap_t&& o)   noexcept { hb_تبادلہ (*یہ, o); واپس *یہ; }

  hb_hashmap_t (std::initializer_list<hb_pair_t<K, V>> lst) : hb_hashmap_t ()
  {
    برائے (خودکار&& شے : lst)
      سیٹ (شے.پہلا, شے.second);
  }
  سانچہ <قسم_نام Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_hashmap_t (ثابت Iterable &o) : hb_hashmap_t ()
  {
    خودکار iter = hb_iter (o);
    اگر (iter.is_random_access_iterator || iter.has_fast_len)
      alloc (hb_len (iter));
    hb_copy (iter, *یہ);
  }

  ساخت شے_t
  {
    K key;
    uint32_t is_real_ : 1;
    uint32_t is_مستعمل : 1;
    uint32_t hash : 30;
    V value;

    شے_t () : key (),
		is_real_ (جھوٹ), is_مستعمل (جھوٹ),
		hash (0),
		value () {}

    // Needed for https://github.com/harfbuzz/harfbuzz/issues/4138
    K& get_کلید () { واپس key; }
    V& get_قدر () { واپس value; }

    منطقی is_مستعمل_2 () ثابت { واپس is_مستعمل; }
    خالی سیٹ_مستعمل (منطقی is_مستعمل_2) { is_مستعمل = is_مستعمل_2; }
    خالی سیٹ_real (منطقی is_real) { is_real_ = is_real; }
    منطقی is_real () ثابت { واپس is_real_; }

    سانچہ <منطقی v = minus_one,
	      hb_enable_if (v == جھوٹ)>
    ساکن اندرونی ثابت V& default_قدر () { واپس Null(V); };
    سانچہ <منطقی v = minus_one,
	      hb_enable_if (v == سچ)>
    ساکن اندرونی ثابت V& default_قدر ()
    {
      static_assert (hb_is_same (V, hb_codepoint_t), "");
      واپس minus_1;
    };

    منطقی عامل == (ثابت K &o) ثابت { واپس hb_deref (key) == hb_deref (o); }
    منطقی عامل == (ثابت شے_t &o) ثابت { واپس *یہ == o.key; }
    hb_pair_t<K, V> get_pair() ثابت { واپس hb_pair_t<K, V> (key, value); }
    hb_pair_t<ثابت K &, V &> get_pair_ref() { واپس hb_pair_t<ثابت K &, V &> (key, value); }

    uint32_t کل_hash () ثابت
    { واپس (hash * 31u) + hb_hash (value); }

    ساکن constexpr منطقی is_trivially_constructible = (hb_is_trivially_constructible(K) && hb_is_trivially_constructible(V));
  };

  hb_object_سرخی_t سرخی;
  منطقی successful; /* Allocations successful */
  غیرعلامتی مختصر max_chain_لمبائی;
  غیرعلامتی صحیح population; /* Not including tombstones. */
  غیرعلامتی صحیح occupancy; /* Including tombstones. */
  غیرعلامتی صحیح نقاب;
  غیرعلامتی صحیح مفرد;
  شے_t *اشیاء;

  دوست خالی swap (hb_hashmap_t& a, hb_hashmap_t& b) noexcept
  {
    اگر (unlikely (!a.successful || !b.successful))
      واپس;
    hb_تبادلہ (a.max_chain_لمبائی, b.max_chain_لمبائی);
    hb_تبادلہ (a.population, b.population);
    hb_تبادلہ (a.occupancy, b.occupancy);
    hb_تبادلہ (a.نقاب, b.نقاب);
    hb_تبادلہ (a.مفرد, b.مفرد);
    hb_تبادلہ (a.اشیاء, b.اشیاء);
  }
  خالی init ()
  {
    hb_object_init (یہ);

    successful = سچ;
    max_chain_لمبائی = 0;
    population = occupancy = 0;
    نقاب = 0;
    مفرد = 0;
    اشیاء = خالی_اشاریہ;
  }
  خالی fini ()
  {
    hb_object_fini (یہ);

    اگر (likely (اشیاء))
    {
      غیرعلامتی سائز = نقاب + 1;
      برائے (غیرعلامتی i = 0; i < سائز; i++)
	اشیاء[i].~شے_t ();
      hb_free (اشیاء);
      اشیاء = خالی_اشاریہ;
    }
    population = occupancy = 0;
  }

  hb_hashmap_t& reset ()
  {
    successful = سچ;
    clear ();
    واپس *یہ;
  }

  منطقی اندر_error () ثابت { واپس !successful; }

  منطقی alloc (غیرعلامتی new_population = 0)
  {
    اگر (unlikely (!successful)) واپس جھوٹ;
    اگر (unlikely (population > 0x3FFFFFFFu || new_population > 0x3FFFFFFFu))
    {
      // Population sizes >0x3FFFFFFF will result in power
      // being larger than 31, which in turn leads to a new size
      // that overflows u32.
      successful = جھوٹ;
      واپس جھوٹ;
    }

    اگر (new_population != 0 && (new_population + new_population / 2) < نقاب) واپس سچ;

    غیرعلامتی صحیح power = hb_ثنائی_ہندسہ_storage (hb_max (hb_max ((غیرعلامتی) population, new_population) * 2, 4u));
    غیرعلامتی صحیح new_سائز = 1u << power;
    شے_t *new_اشیاء = (شے_t *) hb_malloc2 ((size_t) new_سائز, حجم (شے_t));
    اگر (unlikely (!new_اشیاء))
    {
      successful = جھوٹ;
      واپس جھوٹ;
    }
    اگر (!شے_t::is_trivially_constructible)
      برائے (خودکار &_ : hb_iter (new_اشیاء, new_سائز))
	نیا (&_) شے_t ();
    ورنہ
      hb_memset (new_اشیاء, 0, (size_t) new_سائز * حجم (شے_t));

    غیرعلامتی صحیح old_سائز = سائز ();
    شے_t *old_اشیاء = اشیاء;

    /* Switch to new, empty, array. */
    population = occupancy = 0;
    نقاب = new_سائز - 1;
    مفرد = مفرد_for (power);
    max_chain_لمبائی = power * 2;
    اشیاء = new_اشیاء;

    /* Insert back old items. */
    برائے (غیرعلامتی صحیح i = 0; i < old_سائز; i++)
    {
      اگر (old_اشیاء[i].is_real ())
      {
	سیٹ_with_hash (std::move (old_اشیاء[i].key),
		       old_اشیاء[i].hash,
		       std::move (old_اشیاء[i].value));
      }
    }
    برائے (غیرعلامتی صحیح i = 0; i < old_سائز; i++)
      old_اشیاء[i].~شے_t ();

    hb_free (old_اشیاء);

    واپس سچ;
  }

  سانچہ <قسم_نام T>
  منطقی اندر_storage (ثابت T &v) ثابت
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    واپس اشیاء &&
	   (uintptr_t) اشیاء <= p && p < (uintptr_t) (اشیاء + سائز ());
  }

  سانچہ <قسم_نام KK, قسم_نام VV>
  منطقی سیٹ_with_hash (KK&& key, uint32_t hash, VV&& value, منطقی overwrite = سچ)
  {
    اگر (unlikely (!successful)) واپس جھوٹ;
    اگر (unlikely ((occupancy + occupancy / 2) >= نقاب))
    {
      /* key or value may live in our own storage, eg. m.set (k, m.get (j)).
       * alloc() frees that storage, so copy them out first. */
      اگر (unlikely (اندر_storage (key) || اندر_storage (value)))
      {
	K کلید_copy;
	کلید_copy = std::forward<KK> (key);
	V قدر_copy;
	قدر_copy = std::forward<VV> (value);
	واپس سیٹ_with_hash (std::move (کلید_copy), hash, std::move (قدر_copy), overwrite);
      }
      اگر (unlikely (!alloc ())) واپس جھوٹ;
    }

    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    غیرعلامتی صحیح tombstone = (غیرعلامتی صحیح) -1;
    غیرعلامتی صحیح i = hash % مفرد;
    غیرعلامتی لمبائی = 0;
    غیرعلامتی قدم = 0;
    جب (اشیاء[i].is_مستعمل_2 ())
    {
      اگر ((std::is_integral<K>::value || اشیاء[i].hash == hash) &&
	  اشیاء[i] == key)
      {
        اگر (!overwrite)
	  واپس جھوٹ;
        ورنہ
	  توڑو;
      }
      اگر (!اشیاء[i].is_real () && tombstone == (غیرعلامتی) -1)
        tombstone = i;
      i = (i + ++قدم) & نقاب;
      لمبائی++;
    }

    شے_t &شے = اشیاء[tombstone == (غیرعلامتی) -1 ? i : tombstone];

    اگر (شے.is_مستعمل_2 ())
    {
      occupancy--;
      population -= شے.is_real ();
    }

    شے.key = std::forward<KK> (key);
    شے.value = std::forward<VV> (value);
    شے.hash = hash;
    شے.سیٹ_مستعمل (سچ);
    شے.سیٹ_real (سچ);

    occupancy++;
    population++;

    اگر (unlikely (لمبائی > max_chain_لمبائی) && occupancy > نقاب / 8)
      alloc (نقاب - 8); // This ensures we jump to next larger size

    واپس سچ;
  }

  سانچہ <قسم_نام VV>
  منطقی سیٹ (ثابت K &key, VV&& value, منطقی overwrite = سچ) { واپس سیٹ_with_hash (key, hb_hash (key), std::forward<VV> (value), overwrite); }
  سانچہ <قسم_نام VV>
  منطقی سیٹ (K &&key, VV&& value, منطقی overwrite = سچ)
  {
    uint32_t hash = hb_hash (key);
    واپس سیٹ_with_hash (std::move (key), hash, std::forward<VV> (value), overwrite);
  }
  منطقی جوڑو (ثابت K &key)
  {
    uint32_t hash = hb_hash (key);
    واپس سیٹ_with_hash (key, hash, شے_t::default_قدر ());
  }

  ثابت V& get_with_hash (ثابت K &key, uint32_t hash) ثابت
  {
    اگر (!اشیاء) واپس شے_t::default_قدر ();
    خودکار *شے = fetch_شے (key, hash);
    اگر (شے)
      واپس شے->value;
    واپس شے_t::default_قدر ();
  }
  ثابت V& get (ثابت K &key) ثابت
  {
    اگر (!اشیاء) واپس شے_t::default_قدر ();
    واپس get_with_hash (key, hb_hash (key));
  }

  خالی del (ثابت K &key)
  {
    اگر (!اشیاء) واپس;
    خودکار *شے = fetch_شے (key, hb_hash (key));
    اگر (شے)
    {
      شے->سیٹ_real (جھوٹ);
      population--;
    }
  }

  /* Has interface. */
  ثابت V& عامل [] (K k) ثابت { واپس get (k); }
  سانچہ <قسم_نام VV=V>
  منطقی has (ثابت K &key, VV **vp = خالی_اشاریہ) ثابت
  {
    اگر (!اشیاء) واپس جھوٹ;
    واپس has_with_hash (key, hb_hash (key), vp);
  }
  سانچہ <قسم_نام VV=V>
  منطقی has_with_hash (ثابت K &key, uint32_t hash, VV **vp = خالی_اشاریہ) ثابت
  {
    اگر (!اشیاء) واپس جھوٹ;
    خودکار *شے = fetch_شے (key, hash);
    اگر (شے)
    {
      اگر (vp) *vp = std::addressof (شے->value);
      واپس سچ;
    }
    واپس جھوٹ;
  }
  شے_t *fetch_شے (ثابت K &key, uint32_t hash) ثابت
  {
    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    غیرعلامتی صحیح i = hash % مفرد;
    غیرعلامتی قدم = 0;
    جب (اشیاء[i].is_مستعمل_2 ())
    {
      اگر ((std::is_integral<K>::value || اشیاء[i].hash == hash) &&
	  اشیاء[i] == key)
      {
	اگر (اشیاء[i].is_real ())
	  واپس &اشیاء[i];
	ورنہ
	  واپس خالی_اشاریہ;
      }
      i = (i + ++قدم) & نقاب;
    }
    واپس خالی_اشاریہ;
  }
  /* Projection. */
  ثابت V& عامل () (K k) ثابت { واپس get (k); }

  غیرعلامتی سائز () ثابت { واپس نقاب ? نقاب + 1 : 0; }

  خالی clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (zero-initialized) from any writes.  Any non-empty hashmap
     * is a real heap instance with writable items, so clearing
     * under !successful is safe. */
    اگر (!population && !occupancy) واپس;

    برائے (خودکار &_ : hb_iter (اشیاء, سائز ()))
    {
      /* Reconstruct items. */
      _.~شے_t ();
      نیا (&_) شے_t ();
    }

    population = occupancy = 0;
  }

  منطقی is_empty () ثابت { واپس population == 0; }
  صریح عامل منطقی () ثابت { واپس !is_empty (); }

  uint32_t hash () ثابت
  {
    واپس
    + iter_اشیاء ()
    | hb_reduce ([] (uint32_t h, ثابت شے_t &_) { واپس h ^ _.کل_hash (); }, (uint32_t) 0u)
    ;
  }

  منطقی is_equal (ثابت hb_hashmap_t &other) ثابت
  {
    اگر (population != other.population) واپس جھوٹ;

    برائے (خودکار pair : iter ())
      اگر (other.get (pair.پہلا) != pair.second)
        واپس جھوٹ;

    واپس سچ;
  }
  منطقی عامل == (ثابت hb_hashmap_t &other) ثابت { واپس is_equal (other); }
  منطقی عامل != (ثابت hb_hashmap_t &other) ثابت { واپس !is_equal (other); }

  غیرعلامتی صحیح get_population () ثابت { واپس population; }

  خالی update (ثابت hb_hashmap_t &other)
  {
    اگر (unlikely (!successful)) واپس;

    hb_copy (other, *یہ);
  }

  /*
   * Iterator
   */

  خودکار iter_اشیاء () ثابت HB_AUTO_RETURN
  (
    + hb_iter (اشیاء, یہ->سائز ())
    | hb_filter (&شے_t::is_real)
  )
  خودکار iter_ref () ثابت HB_AUTO_RETURN
  (
    + یہ->iter_اشیاء ()
    | hb_map (&شے_t::get_pair_ref)
  )
  خودکار iter () ثابت HB_AUTO_RETURN
  (
    + یہ->iter_اشیاء ()
    | hb_map (&شے_t::get_pair)
  )
  خودکار کلیدیں_ref () ثابت HB_AUTO_RETURN
  (
    + یہ->iter_اشیاء ()
    | hb_map (&شے_t::get_کلید)
  )
  خودکار کلیدیں () ثابت HB_AUTO_RETURN
  (
    + یہ->کلیدیں_ref ()
    | hb_map (hb_ridentity)
  )
  خودکار اقدار_ref () ثابت HB_AUTO_RETURN
  (
    + یہ->iter_اشیاء ()
    | hb_map (&شے_t::get_قدر)
  )
  خودکار values () ثابت HB_AUTO_RETURN
  (
    + یہ->اقدار_ref ()
    | hb_map (hb_ridentity)
  )

  /* C iterator. */
  منطقی next (صحیح *idx,
	     K *key,
	     V *value) ثابت
  {
    غیرعلامتی i = (غیرعلامتی) (*idx + 1);

    غیرعلامتی گنتی = سائز ();
    جب (i < گنتی && !اشیاء[i].is_real ())
      i++;

    اگر (i >= گنتی)
    {
      *idx = -1;
      واپس جھوٹ;
    }

    *key = اشیاء[i].key;
    *value = اشیاء[i].value;

    *idx = (علامتی) i;
    واپس سچ;
  }

  /* Sink interface. */
  hb_hashmap_t& عامل << (ثابت hb_pair_t<K, V>& v)
  { سیٹ (v.پہلا, v.second); واپس *یہ; }
  سانچہ <قسم_نام V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (V2))>
  hb_hashmap_t& عامل << (ثابت hb_pair_t<K, V&&>& v)
  { سیٹ (v.پہلا, std::move (v.second)); واپس *یہ; }
  سانچہ <قسم_نام K2 = K,
	    hb_enable_if (!hb_is_trivially_copyable (K2))>
  hb_hashmap_t& عامل << (ثابت hb_pair_t<K&&, V>& v)
  { سیٹ (std::move (v.پہلا), v.second); واپس *یہ; }
  سانچہ <قسم_نام K2 = K, قسم_نام V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (K2) &&
			  !hb_is_trivially_copyable (V2))>
  hb_hashmap_t& عامل << (ثابت hb_pair_t<K&&, V&&>& v)
  { سیٹ (std::move (v.پہلا), std::move (v.second)); واپس *یہ; }

  ساکن غیرعلامتی صحیح مفرد_for (غیرعلامتی صحیح سرکاؤ)
  {
    /* Following comment and table copied from glib. */
    /* Each table size has an associated prime modulo (the first prime
     * lower than the table size) used to find the initial bucket. Probing
     * then works modulo 2^n. The prime modulo is necessary to get a
     * good distribution with poor hash functions.
     */
    /* Not declaring static to make all kinds of compilers happy... */
    /*static*/ ثابت غیرعلامتی صحیح مفرد_mod [32] =
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

    اگر (unlikely (سرکاؤ >= ARRAY_لمبائی (مفرد_mod)))
      واپس مفرد_mod[ARRAY_لمبائی (مفرد_mod) - 1];

    واپس مفرد_mod[سرکاؤ];
  }
};

/*
 * hb_map_t
 */

ساخت hb_map_t : hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       سچ>
{
  استعمال hashmap = hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       سچ>;

  ~hb_map_t () = طےشدہ;
  hb_map_t () : hashmap () {}
  hb_map_t (ثابت hb_map_t &o) : hashmap ((hashmap &) o) {}
  hb_map_t (hb_map_t &&o)  noexcept : hashmap (std::move ((hashmap &) o)) {}
  hb_map_t& عامل= (ثابت hb_map_t&) = طےشدہ;
  hb_map_t& عامل= (hb_map_t&&) = طےشدہ;
  hb_map_t (std::initializer_list<hb_codepoint_pair_t> lst) : hashmap (lst) {}
  سانچہ <قسم_نام Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_map_t (ثابت Iterable &o) : hashmap (o) {}
};


#اگر_ختم /* HB_MAP_HH */

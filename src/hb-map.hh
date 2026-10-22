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

#ifndef HB_MAP_HH
#define HB_MAP_HH

#شامل "hb.hh"

#شامل "hb-set.hh"


/*
 * hb_hashmap_t
 */

بیرونی HB_INTERNAL ثابت hb_codepoint_t minus_1;

template <typename K, typename V,
	  منطقی minus_one = نادرست>
ساختار hb_hashmap_t
{
  ایستا constexpr منطقی realloc_move = درست;

  hb_hashmap_t ()  { init (); }
  ~hb_hashmap_t () { fini (); }

  تهی _copy (ثابت hb_hashmap_t& o)
  {
    اگر (unlikely (!o.successful))
    {
      successful = نادرست;
      بازگشت;
    }
    اگر (unlikely (!o.mask)) بازگشت;

    اگر (hb_is_trivially_copy_assignable (item_t))
    {
      اقلام = (item_t *) hb_malloc2 ((o.mask + 1), اندازه (item_t));
      اگر (unlikely (!اقلام))
      {
	successful = نادرست;
	بازگشت;
      }
      population = o.population;
      occupancy = o.occupancy;
      mask = o.mask;
      اول = o.اول;
      max_chain_length = o.max_chain_length;
      memcpy (اقلام, o.اقلام, اندازه (item_t) * (mask + 1));
      بازگشت;
    }

    alloc (o.population); hb_copy (o, *این);
  }

  hb_hashmap_t (ثابت hb_hashmap_t& o) : hb_hashmap_t () { _copy (o); }
  hb_hashmap_t& operator= (ثابت hb_hashmap_t& o)
  {
    reset ();
    اگر (!اقلام) { _copy (o); بازگشت *این; }
    alloc (o.population); hb_copy (o, *این); بازگشت *این;
  }

  hb_hashmap_t (hb_hashmap_t&& o)  noexcept : hb_hashmap_t () { hb_swap (*این, o); }
  hb_hashmap_t& operator= (hb_hashmap_t&& o)   noexcept { hb_swap (*این, o); بازگشت *این; }

  hb_hashmap_t (std::initializer_list<hb_pair_t<K, V>> lst) : hb_hashmap_t ()
  {
    برای (auto&& item : lst)
      set (item.first, item.second);
  }
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_hashmap_t (ثابت Iterable &o) : hb_hashmap_t ()
  {
    auto iter = hb_iter (o);
    اگر (iter.is_random_access_iterator || iter.has_fast_len)
      alloc (hb_len (iter));
    hb_copy (iter, *این);
  }

  ساختار item_t
  {
    K key;
    uint32_t is_real_ : 1;
    uint32_t is_used_ : 1;
    uint32_t hash : 30;
    V value;

    item_t () : key (),
		is_real_ (نادرست), is_used_ (نادرست),
		hash (0),
		value () {}

    // Needed for https://github.com/harfbuzz/harfbuzz/issues/4138
    K& get_کلید () { بازگشت key; }
    V& get_مقدار () { بازگشت value; }

    منطقی is_used () ثابت { بازگشت is_used_; }
    تهی set_used (منطقی is_used) { is_used_ = is_used; }
    تهی set_real (منطقی is_real) { is_real_ = is_real; }
    منطقی is_real () ثابت { بازگشت is_real_; }

    template <منطقی v = minus_one,
	      hb_enable_if (v == نادرست)>
    ایستا درونخطی ثابت V& default_مقدار () { بازگشت Null(V); };
    template <منطقی v = minus_one,
	      hb_enable_if (v == درست)>
    ایستا درونخطی ثابت V& default_مقدار ()
    {
      static_assert (hb_is_same (V, hb_codepoint_t), "");
      بازگشت minus_1;
    };

    منطقی operator == (ثابت K &o) ثابت { بازگشت hb_deref (key) == hb_deref (o); }
    منطقی operator == (ثابت item_t &o) ثابت { بازگشت *این == o.key; }
    hb_pair_t<K, V> get_pair() ثابت { بازگشت hb_pair_t<K, V> (key, value); }
    hb_pair_t<ثابت K &, V &> get_pair_ref() { بازگشت hb_pair_t<ثابت K &, V &> (key, value); }

    uint32_t total_hash () ثابت
    { بازگشت (hash * 31u) + hb_hash (value); }

    ایستا constexpr منطقی is_trivially_constructible = (hb_is_trivially_constructible(K) && hb_is_trivially_constructible(V));
  };

  hb_object_header_t header;
  منطقی successful; /* Allocations successful */
  unsigned short max_chain_length;
  unsigned صحیح population; /* Not including tombstones. */
  unsigned صحیح occupancy; /* Including tombstones. */
  unsigned صحیح mask;
  unsigned صحیح اول;
  item_t *اقلام;

  friend تهی swap (hb_hashmap_t& a, hb_hashmap_t& b) noexcept
  {
    اگر (unlikely (!a.successful || !b.successful))
      بازگشت;
    hb_swap (a.max_chain_length, b.max_chain_length);
    hb_swap (a.population, b.population);
    hb_swap (a.occupancy, b.occupancy);
    hb_swap (a.mask, b.mask);
    hb_swap (a.اول, b.اول);
    hb_swap (a.اقلام, b.اقلام);
  }
  تهی init ()
  {
    hb_object_init (این);

    successful = درست;
    max_chain_length = 0;
    population = occupancy = 0;
    mask = 0;
    اول = 0;
    اقلام = nullptr;
  }
  تهی fini ()
  {
    hb_object_fini (این);

    اگر (likely (اقلام))
    {
      unsigned size = mask + 1;
      برای (unsigned i = 0; i < size; i++)
	اقلام[i].~item_t ();
      hb_free (اقلام);
      اقلام = nullptr;
    }
    population = occupancy = 0;
  }

  hb_hashmap_t& reset ()
  {
    successful = درست;
    clear ();
    بازگشت *این;
  }

  منطقی in_error () ثابت { بازگشت !successful; }

  منطقی alloc (unsigned new_population = 0)
  {
    اگر (unlikely (!successful)) بازگشت نادرست;
    اگر (unlikely (population > 0x3FFFFFFFu || new_population > 0x3FFFFFFFu))
    {
      // Population sizes >0x3FFFFFFF will result in power
      // being larger than 31, which in turn leads to a new size
      // that overflows u32.
      successful = نادرست;
      بازگشت نادرست;
    }

    اگر (new_population != 0 && (new_population + new_population / 2) < mask) بازگشت درست;

    unsigned صحیح power = hb_bit_storage (hb_max (hb_max ((unsigned) population, new_population) * 2, 4u));
    unsigned صحیح new_size = 1u << power;
    item_t *new_اقلام = (item_t *) hb_malloc2 ((size_t) new_size, اندازه (item_t));
    اگر (unlikely (!new_اقلام))
    {
      successful = نادرست;
      بازگشت نادرست;
    }
    اگر (!item_t::is_trivially_constructible)
      برای (auto &_ : hb_iter (new_اقلام, new_size))
	جدید (&_) item_t ();
    وگرنه
      hb_memset (new_اقلام, 0, (size_t) new_size * اندازه (item_t));

    unsigned صحیح old_size = size ();
    item_t *old_اقلام = اقلام;

    /* Switch to new, empty, array. */
    population = occupancy = 0;
    mask = new_size - 1;
    اول = اول_for (power);
    max_chain_length = power * 2;
    اقلام = new_اقلام;

    /* Insert back old items. */
    برای (unsigned صحیح i = 0; i < old_size; i++)
    {
      اگر (old_اقلام[i].is_real ())
      {
	set_with_hash (std::move (old_اقلام[i].key),
		       old_اقلام[i].hash,
		       std::move (old_اقلام[i].value));
      }
    }
    برای (unsigned صحیح i = 0; i < old_size; i++)
      old_اقلام[i].~item_t ();

    hb_free (old_اقلام);

    بازگشت درست;
  }

  template <typename T>
  منطقی in_storage (ثابت T &v) ثابت
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    بازگشت اقلام &&
	   (uintptr_t) اقلام <= p && p < (uintptr_t) (اقلام + size ());
  }

  template <typename KK, typename VV>
  منطقی set_with_hash (KK&& key, uint32_t hash, VV&& value, منطقی overwrite = درست)
  {
    اگر (unlikely (!successful)) بازگشت نادرست;
    اگر (unlikely ((occupancy + occupancy / 2) >= mask))
    {
      /* key or value may live in our own storage, eg. m.set (k, m.get (j)).
       * alloc() frees that storage, so copy them out first. */
      اگر (unlikely (in_storage (key) || in_storage (value)))
      {
	K کلید_copy;
	کلید_copy = std::forward<KK> (key);
	V مقدار_copy;
	مقدار_copy = std::forward<VV> (value);
	بازگشت set_with_hash (std::move (کلید_copy), hash, std::move (مقدار_copy), overwrite);
      }
      اگر (unlikely (!alloc ())) بازگشت نادرست;
    }

    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    unsigned صحیح tombstone = (unsigned صحیح) -1;
    unsigned صحیح i = hash % اول;
    unsigned length = 0;
    unsigned step = 0;
    وقتی (اقلام[i].is_used ())
    {
      اگر ((std::is_integral<K>::value || اقلام[i].hash == hash) &&
	  اقلام[i] == key)
      {
        اگر (!overwrite)
	  بازگشت نادرست;
        وگرنه
	  بشکن;
      }
      اگر (!اقلام[i].is_real () && tombstone == (unsigned) -1)
        tombstone = i;
      i = (i + ++step) & mask;
      length++;
    }

    item_t &item = اقلام[tombstone == (unsigned) -1 ? i : tombstone];

    اگر (item.is_used ())
    {
      occupancy--;
      population -= item.is_real ();
    }

    item.key = std::forward<KK> (key);
    item.value = std::forward<VV> (value);
    item.hash = hash;
    item.set_used (درست);
    item.set_real (درست);

    occupancy++;
    population++;

    اگر (unlikely (length > max_chain_length) && occupancy > mask / 8)
      alloc (mask - 8); // This ensures we jump to next larger size

    بازگشت درست;
  }

  template <typename VV>
  منطقی set (ثابت K &key, VV&& value, منطقی overwrite = درست) { بازگشت set_with_hash (key, hb_hash (key), std::forward<VV> (value), overwrite); }
  template <typename VV>
  منطقی set (K &&key, VV&& value, منطقی overwrite = درست)
  {
    uint32_t hash = hb_hash (key);
    بازگشت set_with_hash (std::move (key), hash, std::forward<VV> (value), overwrite);
  }
  منطقی add (ثابت K &key)
  {
    uint32_t hash = hb_hash (key);
    بازگشت set_with_hash (key, hash, item_t::default_مقدار ());
  }

  ثابت V& get_with_hash (ثابت K &key, uint32_t hash) ثابت
  {
    اگر (!اقلام) بازگشت item_t::default_مقدار ();
    auto *item = fetch_item (key, hash);
    اگر (item)
      بازگشت item->value;
    بازگشت item_t::default_مقدار ();
  }
  ثابت V& get (ثابت K &key) ثابت
  {
    اگر (!اقلام) بازگشت item_t::default_مقدار ();
    بازگشت get_with_hash (key, hb_hash (key));
  }

  تهی del (ثابت K &key)
  {
    اگر (!اقلام) بازگشت;
    auto *item = fetch_item (key, hb_hash (key));
    اگر (item)
    {
      item->set_real (نادرست);
      population--;
    }
  }

  /* Has interface. */
  ثابت V& operator [] (K k) ثابت { بازگشت get (k); }
  template <typename VV=V>
  منطقی has (ثابت K &key, VV **vp = nullptr) ثابت
  {
    اگر (!اقلام) بازگشت نادرست;
    بازگشت has_with_hash (key, hb_hash (key), vp);
  }
  template <typename VV=V>
  منطقی has_with_hash (ثابت K &key, uint32_t hash, VV **vp = nullptr) ثابت
  {
    اگر (!اقلام) بازگشت نادرست;
    auto *item = fetch_item (key, hash);
    اگر (item)
    {
      اگر (vp) *vp = std::addressof (item->value);
      بازگشت درست;
    }
    بازگشت نادرست;
  }
  item_t *fetch_item (ثابت K &key, uint32_t hash) ثابت
  {
    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    unsigned صحیح i = hash % اول;
    unsigned step = 0;
    وقتی (اقلام[i].is_used ())
    {
      اگر ((std::is_integral<K>::value || اقلام[i].hash == hash) &&
	  اقلام[i] == key)
      {
	اگر (اقلام[i].is_real ())
	  بازگشت &اقلام[i];
	وگرنه
	  بازگشت nullptr;
      }
      i = (i + ++step) & mask;
    }
    بازگشت nullptr;
  }
  /* Projection. */
  ثابت V& operator () (K k) ثابت { بازگشت get (k); }

  unsigned size () ثابت { بازگشت mask ? mask + 1 : 0; }

  تهی clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (zero-initialized) from any writes.  Any non-empty hashmap
     * is a real heap instance with writable items, so clearing
     * under !successful is safe. */
    اگر (!population && !occupancy) بازگشت;

    برای (auto &_ : hb_iter (اقلام, size ()))
    {
      /* Reconstruct items. */
      _.~item_t ();
      جدید (&_) item_t ();
    }

    population = occupancy = 0;
  }

  منطقی is_empty () ثابت { بازگشت population == 0; }
  explicit operator منطقی () ثابت { بازگشت !is_empty (); }

  uint32_t hash () ثابت
  {
    بازگشت
    + iter_اقلام ()
    | hb_reduce ([] (uint32_t h, ثابت item_t &_) { بازگشت h ^ _.total_hash (); }, (uint32_t) 0u)
    ;
  }

  منطقی is_equal (ثابت hb_hashmap_t &other) ثابت
  {
    اگر (population != other.population) بازگشت نادرست;

    برای (auto pair : iter ())
      اگر (other.get (pair.first) != pair.second)
        بازگشت نادرست;

    بازگشت درست;
  }
  منطقی operator == (ثابت hb_hashmap_t &other) ثابت { بازگشت is_equal (other); }
  منطقی operator != (ثابت hb_hashmap_t &other) ثابت { بازگشت !is_equal (other); }

  unsigned صحیح get_population () ثابت { بازگشت population; }

  تهی update (ثابت hb_hashmap_t &other)
  {
    اگر (unlikely (!successful)) بازگشت;

    hb_copy (other, *این);
  }

  /*
   * Iterator
   */

  auto iter_اقلام () ثابت HB_AUTO_RETURN
  (
    + hb_iter (اقلام, این->size ())
    | hb_filter (&item_t::is_real)
  )
  auto iter_ref () ثابت HB_AUTO_RETURN
  (
    + این->iter_اقلام ()
    | hb_map (&item_t::get_pair_ref)
  )
  auto iter () ثابت HB_AUTO_RETURN
  (
    + این->iter_اقلام ()
    | hb_map (&item_t::get_pair)
  )
  auto کلیدها_ref () ثابت HB_AUTO_RETURN
  (
    + این->iter_اقلام ()
    | hb_map (&item_t::get_کلید)
  )
  auto کلیدها () ثابت HB_AUTO_RETURN
  (
    + این->کلیدها_ref ()
    | hb_map (hb_ridentity)
  )
  auto مقادیر_ref () ثابت HB_AUTO_RETURN
  (
    + این->iter_اقلام ()
    | hb_map (&item_t::get_مقدار)
  )
  auto values () ثابت HB_AUTO_RETURN
  (
    + این->مقادیر_ref ()
    | hb_map (hb_ridentity)
  )

  /* C iterator. */
  منطقی next (صحیح *idx,
	     K *key,
	     V *value) ثابت
  {
    unsigned i = (unsigned) (*idx + 1);

    unsigned شمار = size ();
    وقتی (i < شمار && !اقلام[i].is_real ())
      i++;

    اگر (i >= شمار)
    {
      *idx = -1;
      بازگشت نادرست;
    }

    *key = اقلام[i].key;
    *value = اقلام[i].value;

    *idx = (signed) i;
    بازگشت درست;
  }

  /* Sink interface. */
  hb_hashmap_t& operator << (ثابت hb_pair_t<K, V>& v)
  { set (v.first, v.second); بازگشت *این; }
  template <typename V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (V2))>
  hb_hashmap_t& operator << (ثابت hb_pair_t<K, V&&>& v)
  { set (v.first, std::move (v.second)); بازگشت *این; }
  template <typename K2 = K,
	    hb_enable_if (!hb_is_trivially_copyable (K2))>
  hb_hashmap_t& operator << (ثابت hb_pair_t<K&&, V>& v)
  { set (std::move (v.first), v.second); بازگشت *این; }
  template <typename K2 = K, typename V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (K2) &&
			  !hb_is_trivially_copyable (V2))>
  hb_hashmap_t& operator << (ثابت hb_pair_t<K&&, V&&>& v)
  { set (std::move (v.first), std::move (v.second)); بازگشت *این; }

  ایستا unsigned صحیح اول_for (unsigned صحیح shift)
  {
    /* Following comment and table copied from glib. */
    /* Each table size has an associated prime modulo (the first prime
     * lower than the table size) used to find the initial bucket. Probing
     * then works modulo 2^n. The prime modulo is necessary to get a
     * good distribution with poor hash functions.
     */
    /* Not declaring static to make all kinds of compilers happy... */
    /*static*/ ثابت unsigned صحیح اول_mod [32] =
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

    اگر (unlikely (shift >= ARRAY_LENGTH (اول_mod)))
      بازگشت اول_mod[ARRAY_LENGTH (اول_mod) - 1];

    بازگشت اول_mod[shift];
  }
};

/*
 * hb_map_t
 */

ساختار hb_map_t : hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       درست>
{
  using hashmap = hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       درست>;

  ~hb_map_t () = پیشفرض;
  hb_map_t () : hashmap () {}
  hb_map_t (ثابت hb_map_t &o) : hashmap ((hashmap &) o) {}
  hb_map_t (hb_map_t &&o)  noexcept : hashmap (std::move ((hashmap &) o)) {}
  hb_map_t& operator= (ثابت hb_map_t&) = پیشفرض;
  hb_map_t& operator= (hb_map_t&&) = پیشفرض;
  hb_map_t (std::initializer_list<hb_codepoint_pair_t> lst) : hashmap (lst) {}
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_map_t (ثابت Iterable &o) : hashmap (o) {}
};


#endif /* HB_MAP_HH */

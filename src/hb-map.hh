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

#அந்தர்பாவ "hb.hh"

#அந்தர்பாவ "hb-set.hh"


/*
 * hb_hashmap_t
 */

வெளி HB_INTERNAL மாறிலி hb_codepoint_t minus_1;

template <typename K, typename V,
	  தருக்க minus_one = பொய்>
கட்டமைப்பு hb_hashmap_t
{
  நிலையான constexpr தருக்க realloc_move = உண்மை;

  hb_hashmap_t ()  { init (); }
  ~hb_hashmap_t () { fini (); }

  வெறுமை _copy (மாறிலி hb_hashmap_t& o)
  {
    எனில் (unlikely (!o.successful))
    {
      successful = பொய்;
      திரும்பு;
    }
    எனில் (unlikely (!o.mask)) திரும்பு;

    எனில் (hb_is_trivially_copy_assignable (item_t))
    {
      பொருட்கள் = (item_t *) hb_malloc2 ((o.mask + 1), அளவு (item_t));
      எனில் (unlikely (!பொருட்கள்))
      {
	successful = பொய்;
	திரும்பு;
      }
      population = o.population;
      occupancy = o.occupancy;
      mask = o.mask;
      பகாஎண் = o.பகாஎண்;
      max_chain_length = o.max_chain_length;
      memcpy (பொருட்கள், o.பொருட்கள், அளவு (item_t) * (mask + 1));
      திரும்பு;
    }

    alloc (o.population); hb_copy (o, *இது);
  }

  hb_hashmap_t (மாறிலி hb_hashmap_t& o) : hb_hashmap_t () { _copy (o); }
  hb_hashmap_t& operator= (மாறிலி hb_hashmap_t& o)
  {
    reset ();
    எனில் (!பொருட்கள்) { _copy (o); திரும்பு *இது; }
    alloc (o.population); hb_copy (o, *இது); திரும்பு *இது;
  }

  hb_hashmap_t (hb_hashmap_t&& o)  noexcept : hb_hashmap_t () { hb_swap (*இது, o); }
  hb_hashmap_t& operator= (hb_hashmap_t&& o)   noexcept { hb_swap (*இது, o); திரும்பு *இது; }

  hb_hashmap_t (std::initializer_list<hb_pair_t<K, V>> lst) : hb_hashmap_t ()
  {
    க்கு (auto&& item : lst)
      set (item.first, item.second);
  }
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_hashmap_t (மாறிலி Iterable &o) : hb_hashmap_t ()
  {
    auto iter = hb_iter (o);
    எனில் (iter.is_random_access_iterator || iter.has_fast_len)
      alloc (hb_len (iter));
    hb_copy (iter, *இது);
  }

  கட்டமைப்பு item_t
  {
    K key;
    uint32_t is_real_ : 1;
    uint32_t is_used_ : 1;
    uint32_t hash : 30;
    V value;

    item_t () : key (),
		is_real_ (பொய்), is_used_ (பொய்),
		hash (0),
		value () {}

    // Needed for https://github.com/harfbuzz/harfbuzz/issues/4138
    K& get_சாவி () { திரும்பு key; }
    V& get_மதிப்பு () { திரும்பு value; }

    தருக்க is_used () மாறிலி { திரும்பு is_used_; }
    வெறுமை set_used (தருக்க is_used) { is_used_ = is_used; }
    வெறுமை set_real (தருக்க is_real) { is_real_ = is_real; }
    தருக்க is_real () மாறிலி { திரும்பு is_real_; }

    template <தருக்க v = minus_one,
	      hb_enable_if (v == பொய்)>
    நிலையான உள்வரி மாறிலி V& default_மதிப்பு () { திரும்பு Null(V); };
    template <தருக்க v = minus_one,
	      hb_enable_if (v == உண்மை)>
    நிலையான உள்வரி மாறிலி V& default_மதிப்பு ()
    {
      static_assert (hb_is_same (V, hb_codepoint_t), "");
      திரும்பு minus_1;
    };

    தருக்க operator == (மாறிலி K &o) மாறிலி { திரும்பு hb_deref (key) == hb_deref (o); }
    தருக்க operator == (மாறிலி item_t &o) மாறிலி { திரும்பு *இது == o.key; }
    hb_pair_t<K, V> get_pair() மாறிலி { திரும்பு hb_pair_t<K, V> (key, value); }
    hb_pair_t<மாறிலி K &, V &> get_pair_ref() { திரும்பு hb_pair_t<மாறிலி K &, V &> (key, value); }

    uint32_t total_hash () மாறிலி
    { திரும்பு (hash * 31u) + hb_hash (value); }

    நிலையான constexpr தருக்க is_trivially_constructible = (hb_is_trivially_constructible(K) && hb_is_trivially_constructible(V));
  };

  hb_object_header_t header;
  தருக்க successful; /* Allocations successful */
  unsigned short max_chain_length;
  unsigned முழு population; /* Not including tombstones. */
  unsigned முழு occupancy; /* Including tombstones. */
  unsigned முழு mask;
  unsigned முழு பகாஎண்;
  item_t *பொருட்கள்;

  friend வெறுமை swap (hb_hashmap_t& a, hb_hashmap_t& b) noexcept
  {
    எனில் (unlikely (!a.successful || !b.successful))
      திரும்பு;
    hb_swap (a.max_chain_length, b.max_chain_length);
    hb_swap (a.population, b.population);
    hb_swap (a.occupancy, b.occupancy);
    hb_swap (a.mask, b.mask);
    hb_swap (a.பகாஎண், b.பகாஎண்);
    hb_swap (a.பொருட்கள், b.பொருட்கள்);
  }
  வெறுமை init ()
  {
    hb_object_init (இது);

    successful = உண்மை;
    max_chain_length = 0;
    population = occupancy = 0;
    mask = 0;
    பகாஎண் = 0;
    பொருட்கள் = nullptr;
  }
  வெறுமை fini ()
  {
    hb_object_fini (இது);

    எனில் (likely (பொருட்கள்))
    {
      unsigned size = mask + 1;
      க்கு (unsigned i = 0; i < size; i++)
	பொருட்கள்[i].~item_t ();
      hb_free (பொருட்கள்);
      பொருட்கள் = nullptr;
    }
    population = occupancy = 0;
  }

  hb_hashmap_t& reset ()
  {
    successful = உண்மை;
    clear ();
    திரும்பு *இது;
  }

  தருக்க in_error () மாறிலி { திரும்பு !successful; }

  தருக்க alloc (unsigned new_population = 0)
  {
    எனில் (unlikely (!successful)) திரும்பு பொய்;
    எனில் (unlikely (population > 0x3FFFFFFFu || new_population > 0x3FFFFFFFu))
    {
      // Population sizes >0x3FFFFFFF will result in power
      // being larger than 31, which in turn leads to a new size
      // that overflows u32.
      successful = பொய்;
      திரும்பு பொய்;
    }

    எனில் (new_population != 0 && (new_population + new_population / 2) < mask) திரும்பு உண்மை;

    unsigned முழு power = hb_bit_storage (hb_max (hb_max ((unsigned) population, new_population) * 2, 4u));
    unsigned முழு new_size = 1u << power;
    item_t *new_பொருட்கள் = (item_t *) hb_malloc2 ((size_t) new_size, அளவு (item_t));
    எனில் (unlikely (!new_பொருட்கள்))
    {
      successful = பொய்;
      திரும்பு பொய்;
    }
    எனில் (!item_t::is_trivially_constructible)
      க்கு (auto &_ : hb_iter (new_பொருட்கள், new_size))
	புதிய (&_) item_t ();
    இல்லையெனில்
      hb_memset (new_பொருட்கள், 0, (size_t) new_size * அளவு (item_t));

    unsigned முழு old_size = size ();
    item_t *old_பொருட்கள் = பொருட்கள்;

    /* Switch to new, empty, array. */
    population = occupancy = 0;
    mask = new_size - 1;
    பகாஎண் = பகாஎண்_for (power);
    max_chain_length = power * 2;
    பொருட்கள் = new_பொருட்கள்;

    /* Insert back old items. */
    க்கு (unsigned முழு i = 0; i < old_size; i++)
    {
      எனில் (old_பொருட்கள்[i].is_real ())
      {
	set_with_hash (std::move (old_பொருட்கள்[i].key),
		       old_பொருட்கள்[i].hash,
		       std::move (old_பொருட்கள்[i].value));
      }
    }
    க்கு (unsigned முழு i = 0; i < old_size; i++)
      old_பொருட்கள்[i].~item_t ();

    hb_free (old_பொருட்கள்);

    திரும்பு உண்மை;
  }

  template <typename T>
  தருக்க in_storage (மாறிலி T &v) மாறிலி
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    திரும்பு பொருட்கள் &&
	   (uintptr_t) பொருட்கள் <= p && p < (uintptr_t) (பொருட்கள் + size ());
  }

  template <typename KK, typename VV>
  தருக்க set_with_hash (KK&& key, uint32_t hash, VV&& value, தருக்க overwrite = உண்மை)
  {
    எனில் (unlikely (!successful)) திரும்பு பொய்;
    எனில் (unlikely ((occupancy + occupancy / 2) >= mask))
    {
      /* key or value may live in our own storage, eg. m.set (k, m.get (j)).
       * alloc() frees that storage, so copy them out first. */
      எனில் (unlikely (in_storage (key) || in_storage (value)))
      {
	K சாவி_copy;
	சாவி_copy = std::forward<KK> (key);
	V மதிப்பு_copy;
	மதிப்பு_copy = std::forward<VV> (value);
	திரும்பு set_with_hash (std::move (சாவி_copy), hash, std::move (மதிப்பு_copy), overwrite);
      }
      எனில் (unlikely (!alloc ())) திரும்பு பொய்;
    }

    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    unsigned முழு tombstone = (unsigned முழு) -1;
    unsigned முழு i = hash % பகாஎண்;
    unsigned length = 0;
    unsigned step = 0;
    போது (பொருட்கள்[i].is_used ())
    {
      எனில் ((std::is_integral<K>::value || பொருட்கள்[i].hash == hash) &&
	  பொருட்கள்[i] == key)
      {
        எனில் (!overwrite)
	  திரும்பு பொய்;
        இல்லையெனில்
	  நிறுத்து;
      }
      எனில் (!பொருட்கள்[i].is_real () && tombstone == (unsigned) -1)
        tombstone = i;
      i = (i + ++step) & mask;
      length++;
    }

    item_t &item = பொருட்கள்[tombstone == (unsigned) -1 ? i : tombstone];

    எனில் (item.is_used ())
    {
      occupancy--;
      population -= item.is_real ();
    }

    item.key = std::forward<KK> (key);
    item.value = std::forward<VV> (value);
    item.hash = hash;
    item.set_used (உண்மை);
    item.set_real (உண்மை);

    occupancy++;
    population++;

    எனில் (unlikely (length > max_chain_length) && occupancy > mask / 8)
      alloc (mask - 8); // This ensures we jump to next larger size

    திரும்பு உண்மை;
  }

  template <typename VV>
  தருக்க set (மாறிலி K &key, VV&& value, தருக்க overwrite = உண்மை) { திரும்பு set_with_hash (key, hb_hash (key), std::forward<VV> (value), overwrite); }
  template <typename VV>
  தருக்க set (K &&key, VV&& value, தருக்க overwrite = உண்மை)
  {
    uint32_t hash = hb_hash (key);
    திரும்பு set_with_hash (std::move (key), hash, std::forward<VV> (value), overwrite);
  }
  தருக்க add (மாறிலி K &key)
  {
    uint32_t hash = hb_hash (key);
    திரும்பு set_with_hash (key, hash, item_t::default_மதிப்பு ());
  }

  மாறிலி V& get_with_hash (மாறிலி K &key, uint32_t hash) மாறிலி
  {
    எனில் (!பொருட்கள்) திரும்பு item_t::default_மதிப்பு ();
    auto *item = fetch_item (key, hash);
    எனில் (item)
      திரும்பு item->value;
    திரும்பு item_t::default_மதிப்பு ();
  }
  மாறிலி V& get (மாறிலி K &key) மாறிலி
  {
    எனில் (!பொருட்கள்) திரும்பு item_t::default_மதிப்பு ();
    திரும்பு get_with_hash (key, hb_hash (key));
  }

  வெறுமை del (மாறிலி K &key)
  {
    எனில் (!பொருட்கள்) திரும்பு;
    auto *item = fetch_item (key, hb_hash (key));
    எனில் (item)
    {
      item->set_real (பொய்);
      population--;
    }
  }

  /* Has interface. */
  மாறிலி V& operator [] (K k) மாறிலி { திரும்பு get (k); }
  template <typename VV=V>
  தருக்க has (மாறிலி K &key, VV **vp = nullptr) மாறிலி
  {
    எனில் (!பொருட்கள்) திரும்பு பொய்;
    திரும்பு has_with_hash (key, hb_hash (key), vp);
  }
  template <typename VV=V>
  தருக்க has_with_hash (மாறிலி K &key, uint32_t hash, VV **vp = nullptr) மாறிலி
  {
    எனில் (!பொருட்கள்) திரும்பு பொய்;
    auto *item = fetch_item (key, hash);
    எனில் (item)
    {
      எனில் (vp) *vp = std::addressof (item->value);
      திரும்பு உண்மை;
    }
    திரும்பு பொய்;
  }
  item_t *fetch_item (மாறிலி K &key, uint32_t hash) மாறிலி
  {
    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    unsigned முழு i = hash % பகாஎண்;
    unsigned step = 0;
    போது (பொருட்கள்[i].is_used ())
    {
      எனில் ((std::is_integral<K>::value || பொருட்கள்[i].hash == hash) &&
	  பொருட்கள்[i] == key)
      {
	எனில் (பொருட்கள்[i].is_real ())
	  திரும்பு &பொருட்கள்[i];
	இல்லையெனில்
	  திரும்பு nullptr;
      }
      i = (i + ++step) & mask;
    }
    திரும்பு nullptr;
  }
  /* Projection. */
  மாறிலி V& operator () (K k) மாறிலி { திரும்பு get (k); }

  unsigned size () மாறிலி { திரும்பு mask ? mask + 1 : 0; }

  வெறுமை clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (zero-initialized) from any writes.  Any non-empty hashmap
     * is a real heap instance with writable items, so clearing
     * under !successful is safe. */
    எனில் (!population && !occupancy) திரும்பு;

    க்கு (auto &_ : hb_iter (பொருட்கள், size ()))
    {
      /* Reconstruct items. */
      _.~item_t ();
      புதிய (&_) item_t ();
    }

    population = occupancy = 0;
  }

  தருக்க is_empty () மாறிலி { திரும்பு population == 0; }
  explicit operator தருக்க () மாறிலி { திரும்பு !is_empty (); }

  uint32_t hash () மாறிலி
  {
    திரும்பு
    + iter_பொருட்கள் ()
    | hb_reduce ([] (uint32_t h, மாறிலி item_t &_) { திரும்பு h ^ _.total_hash (); }, (uint32_t) 0u)
    ;
  }

  தருக்க is_equal (மாறிலி hb_hashmap_t &other) மாறிலி
  {
    எனில் (population != other.population) திரும்பு பொய்;

    க்கு (auto pair : iter ())
      எனில் (other.get (pair.first) != pair.second)
        திரும்பு பொய்;

    திரும்பு உண்மை;
  }
  தருக்க operator == (மாறிலி hb_hashmap_t &other) மாறிலி { திரும்பு is_equal (other); }
  தருக்க operator != (மாறிலி hb_hashmap_t &other) மாறிலி { திரும்பு !is_equal (other); }

  unsigned முழு get_population () மாறிலி { திரும்பு population; }

  வெறுமை update (மாறிலி hb_hashmap_t &other)
  {
    எனில் (unlikely (!successful)) திரும்பு;

    hb_copy (other, *இது);
  }

  /*
   * Iterator
   */

  auto iter_பொருட்கள் () மாறிலி HB_AUTO_RETURN
  (
    + hb_iter (பொருட்கள், இது->size ())
    | hb_filter (&item_t::is_real)
  )
  auto iter_ref () மாறிலி HB_AUTO_RETURN
  (
    + இது->iter_பொருட்கள் ()
    | hb_map (&item_t::get_pair_ref)
  )
  auto iter () மாறிலி HB_AUTO_RETURN
  (
    + இது->iter_பொருட்கள் ()
    | hb_map (&item_t::get_pair)
  )
  auto சாவிகள்_ref () மாறிலி HB_AUTO_RETURN
  (
    + இது->iter_பொருட்கள் ()
    | hb_map (&item_t::get_சாவி)
  )
  auto சாவிகள் () மாறிலி HB_AUTO_RETURN
  (
    + இது->சாவிகள்_ref ()
    | hb_map (hb_ridentity)
  )
  auto மதிப்புகள்_ref () மாறிலி HB_AUTO_RETURN
  (
    + இது->iter_பொருட்கள் ()
    | hb_map (&item_t::get_மதிப்பு)
  )
  auto values () மாறிலி HB_AUTO_RETURN
  (
    + இது->மதிப்புகள்_ref ()
    | hb_map (hb_ridentity)
  )

  /* C iterator. */
  தருக்க next (முழு *idx,
	     K *key,
	     V *value) மாறிலி
  {
    unsigned i = (unsigned) (*idx + 1);

    unsigned மொத்தம் = size ();
    போது (i < மொத்தம் && !பொருட்கள்[i].is_real ())
      i++;

    எனில் (i >= மொத்தம்)
    {
      *idx = -1;
      திரும்பு பொய்;
    }

    *key = பொருட்கள்[i].key;
    *value = பொருட்கள்[i].value;

    *idx = (signed) i;
    திரும்பு உண்மை;
  }

  /* Sink interface. */
  hb_hashmap_t& operator << (மாறிலி hb_pair_t<K, V>& v)
  { set (v.first, v.second); திரும்பு *இது; }
  template <typename V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (V2))>
  hb_hashmap_t& operator << (மாறிலி hb_pair_t<K, V&&>& v)
  { set (v.first, std::move (v.second)); திரும்பு *இது; }
  template <typename K2 = K,
	    hb_enable_if (!hb_is_trivially_copyable (K2))>
  hb_hashmap_t& operator << (மாறிலி hb_pair_t<K&&, V>& v)
  { set (std::move (v.first), v.second); திரும்பு *இது; }
  template <typename K2 = K, typename V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (K2) &&
			  !hb_is_trivially_copyable (V2))>
  hb_hashmap_t& operator << (மாறிலி hb_pair_t<K&&, V&&>& v)
  { set (std::move (v.first), std::move (v.second)); திரும்பு *இது; }

  நிலையான unsigned முழு பகாஎண்_for (unsigned முழு shift)
  {
    /* Following comment and table copied from glib. */
    /* Each table size has an associated prime modulo (the first prime
     * lower than the table size) used to find the initial bucket. Probing
     * then works modulo 2^n. The prime modulo is necessary to get a
     * good distribution with poor hash functions.
     */
    /* Not declaring static to make all kinds of compilers happy... */
    /*static*/ மாறிலி unsigned முழு பகாஎண்_mod [32] =
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

    எனில் (unlikely (shift >= ARRAY_LENGTH (பகாஎண்_mod)))
      திரும்பு பகாஎண்_mod[ARRAY_LENGTH (பகாஎண்_mod) - 1];

    திரும்பு பகாஎண்_mod[shift];
  }
};

/*
 * hb_map_t
 */

கட்டமைப்பு hb_map_t : hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       உண்மை>
{
  using hashmap = hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       உண்மை>;

  ~hb_map_t () = இயல்பு;
  hb_map_t () : hashmap () {}
  hb_map_t (மாறிலி hb_map_t &o) : hashmap ((hashmap &) o) {}
  hb_map_t (hb_map_t &&o)  noexcept : hashmap (std::move ((hashmap &) o)) {}
  hb_map_t& operator= (மாறிலி hb_map_t&) = இயல்பு;
  hb_map_t& operator= (hb_map_t&&) = இயல்பு;
  hb_map_t (std::initializer_list<hb_codepoint_pair_t> lst) : hashmap (lst) {}
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_map_t (மாறிலி Iterable &o) : hashmap (o) {}
};


#endif /* HB_MAP_HH */

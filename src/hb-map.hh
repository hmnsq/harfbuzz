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

#include "hb.hh"

#include "hb-set.hh"


/*
 * hb_hashmap_t
 */

extern HB_INTERNAL const hb_codepoint_t minus_1;

template <typename K, typename V,
	  bool minus_one = false>
sa_nracanaa hb_hashmap_t
{
  static constexpr bool realloc_move = true;

  hb_hashmap_t ()  { init (); }
  ~hb_hashmap_t () { fini (); }

  void _copy (const hb_hashmap_t& o)
  {
    ပေႃး (unlikely (!o.successful))
    {
      successful = false;
      ပွၵ်ႈ;
    }
    ပေႃး (unlikely (!o.mask)) ပွၵ်ႈ;

    ပေႃး (hb_is_trivially_copy_assignable (item_t))
    {
      items = (item_t *) hb_malloc2 ((o.mask + 1), parimaa_na (item_t));
      ပေႃး (unlikely (!items))
      {
	successful = false;
	ပွၵ်ႈ;
      }
      population = o.population;
      occupancy = o.occupancy;
      mask = o.mask;
      prime = o.prime;
      max_chain_length = o.max_chain_length;
      memcpy (items, o.items, parimaa_na (item_t) * (mask + 1));
      ပွၵ်ႈ;
    }

    alloc (o.population); hb_copy (o, *this);
  }

  hb_hashmap_t (const hb_hashmap_t& o) : hb_hashmap_t () { _copy (o); }
  hb_hashmap_t& operator= (const hb_hashmap_t& o)
  {
    reset ();
    ပေႃး (!items) { _copy (o); ပွၵ်ႈ *this; }
    alloc (o.population); hb_copy (o, *this); ပွၵ်ႈ *this;
  }

  hb_hashmap_t (hb_hashmap_t&& o)  noexcept : hb_hashmap_t () { hb_swap (*this, o); }
  hb_hashmap_t& operator= (hb_hashmap_t&& o)   noexcept { hb_swap (*this, o); ပွၵ်ႈ *this; }

  hb_hashmap_t (std::initializer_list<hb_pair_t<K, V>> lst) : hb_hashmap_t ()
  {
    for (auto&& item : lst)
      set (item.first, item.second);
  }
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_hashmap_t (const Iterable &o) : hb_hashmap_t ()
  {
    auto iter = hb_iter (o);
    ပေႃး (iter.is_random_access_iterator || iter.has_fast_len)
      alloc (hb_len (iter));
    hb_copy (iter, *this);
  }

  sa_nracanaa item_t
  {
    K key;
    uint32_t is_real_ : 1;
    uint32_t is_used_ : 1;
    uint32_t hash : 30;
    V value;

    item_t () : key (),
		is_real_ (false), is_used_ (false),
		hash (0),
		value () {}

    // Needed for https://github.com/harfbuzz/harfbuzz/issues/4138
    K& get_key () { ပွၵ်ႈ key; }
    V& get_value () { ပွၵ်ႈ value; }

    bool is_used () const { ပွၵ်ႈ is_used_; }
    void set_used (bool is_used) { is_used_ = is_used; }
    void set_real (bool is_real) { is_real_ = is_real; }
    bool is_real () const { ပွၵ်ႈ is_real_; }

    template <bool v = minus_one,
	      hb_enable_if (v == false)>
    static anta_hstha const V& default_value () { ပွၵ်ႈ Null(V); };
    template <bool v = minus_one,
	      hb_enable_if (v == true)>
    static anta_hstha const V& default_value ()
    {
      static_assert (hb_is_same (V, hb_codepoint_t), "");
      ပွၵ်ႈ minus_1;
    };

    bool operator == (const K &o) const { ပွၵ်ႈ hb_deref (key) == hb_deref (o); }
    bool operator == (const item_t &o) const { ပွၵ်ႈ *this == o.key; }
    hb_pair_t<K, V> get_pair() const { ပွၵ်ႈ hb_pair_t<K, V> (key, value); }
    hb_pair_t<const K &, V &> get_pair_ref() { ပွၵ်ႈ hb_pair_t<const K &, V &> (key, value); }

    uint32_t total_hash () const
    { ပွၵ်ႈ (hash * 31u) + hb_hash (value); }

    static constexpr bool is_trivially_constructible = (hb_is_trivially_constructible(K) && hb_is_trivially_constructible(V));
  };

  hb_object_header_t header;
  bool successful; /* Allocations successful */
  unsigned short max_chain_length;
  unsigned puur_naa_nka population; /* Not including tombstones. */
  unsigned puur_naa_nka occupancy; /* Including tombstones. */
  unsigned puur_naa_nka mask;
  unsigned puur_naa_nka prime;
  item_t *items;

  friend void swap (hb_hashmap_t& a, hb_hashmap_t& b) noexcept
  {
    ပေႃး (unlikely (!a.successful || !b.successful))
      ပွၵ်ႈ;
    hb_swap (a.max_chain_length, b.max_chain_length);
    hb_swap (a.population, b.population);
    hb_swap (a.occupancy, b.occupancy);
    hb_swap (a.mask, b.mask);
    hb_swap (a.prime, b.prime);
    hb_swap (a.items, b.items);
  }
  void init ()
  {
    hb_object_init (this);

    successful = true;
    max_chain_length = 0;
    population = occupancy = 0;
    mask = 0;
    prime = 0;
    items = nullptr;
  }
  void fini ()
  {
    hb_object_fini (this);

    ပေႃး (likely (items))
    {
      unsigned size = mask + 1;
      for (unsigned i = 0; i < size; i++)
	items[i].~item_t ();
      hb_free (items);
      items = nullptr;
    }
    population = occupancy = 0;
  }

  hb_hashmap_t& reset ()
  {
    successful = true;
    clear ();
    ပွၵ်ႈ *this;
  }

  bool in_error () const { ပွၵ်ႈ !successful; }

  bool alloc (unsigned new_population = 0)
  {
    ပေႃး (unlikely (!successful)) ပွၵ်ႈ false;
    ပေႃး (unlikely (population > 0x3FFFFFFFu || new_population > 0x3FFFFFFFu))
    {
      // Population sizes >0x3FFFFFFF will result in power
      // being larger than 31, which in turn leads to a new size
      // that overflows u32.
      successful = false;
      ပွၵ်ႈ false;
    }

    ပေႃး (new_population != 0 && (new_population + new_population / 2) < mask) ပွၵ်ႈ true;

    unsigned puur_naa_nka power = hb_bit_storage (hb_max (hb_max ((unsigned) population, new_population) * 2, 4u));
    unsigned puur_naa_nka new_size = 1u << power;
    item_t *new_items = (item_t *) hb_malloc2 ((size_t) new_size, parimaa_na (item_t));
    ပေႃး (unlikely (!new_items))
    {
      successful = false;
      ပွၵ်ႈ false;
    }
    ပေႃး (!item_t::is_trivially_constructible)
      for (auto &_ : hb_iter (new_items, new_size))
	new (&_) item_t ();
    ပေႃးဢမ်ႇ
      hb_memset (new_items, 0, (size_t) new_size * parimaa_na (item_t));

    unsigned puur_naa_nka old_size = size ();
    item_t *old_items = items;

    /* Switch to new, empty, array. */
    population = occupancy = 0;
    mask = new_size - 1;
    prime = prime_for (power);
    max_chain_length = power * 2;
    items = new_items;

    /* Insert back old items. */
    for (unsigned puur_naa_nka i = 0; i < old_size; i++)
    {
      ပေႃး (old_items[i].is_real ())
      {
	set_with_hash (std::move (old_items[i].key),
		       old_items[i].hash,
		       std::move (old_items[i].value));
      }
    }
    for (unsigned puur_naa_nka i = 0; i < old_size; i++)
      old_items[i].~item_t ();

    hb_free (old_items);

    ပွၵ်ႈ true;
  }

  template <typename T>
  bool in_storage (const T &v) const
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    ပွၵ်ႈ items &&
	   (uintptr_t) items <= p && p < (uintptr_t) (items + size ());
  }

  template <typename KK, typename VV>
  bool set_with_hash (KK&& key, uint32_t hash, VV&& value, bool overwrite = true)
  {
    ပေႃး (unlikely (!successful)) ပွၵ်ႈ false;
    ပေႃး (unlikely ((occupancy + occupancy / 2) >= mask))
    {
      /* key or value may live in our own storage, eg. m.set (k, m.get (j)).
       * alloc() frees that storage, so copy them out first. */
      ပေႃး (unlikely (in_storage (key) || in_storage (value)))
      {
	K key_copy;
	key_copy = std::forward<KK> (key);
	V value_copy;
	value_copy = std::forward<VV> (value);
	ပွၵ်ႈ set_with_hash (std::move (key_copy), hash, std::move (value_copy), overwrite);
      }
      ပေႃး (unlikely (!alloc ())) ပွၵ်ႈ false;
    }

    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    unsigned puur_naa_nka tombstone = (unsigned puur_naa_nka) -1;
    unsigned puur_naa_nka i = hash % prime;
    unsigned length = 0;
    unsigned step = 0;
    ၶၢဝ်း (items[i].is_used ())
    {
      ပေႃး ((std::is_integral<K>::value || items[i].hash == hash) &&
	  items[i] == key)
      {
        ပေႃး (!overwrite)
	  ပွၵ်ႈ false;
        ပေႃးဢမ်ႇ
	  bha_nga;
      }
      ပေႃး (!items[i].is_real () && tombstone == (unsigned) -1)
        tombstone = i;
      i = (i + ++step) & mask;
      length++;
    }

    item_t &item = items[tombstone == (unsigned) -1 ? i : tombstone];

    ပေႃး (item.is_used ())
    {
      occupancy--;
      population -= item.is_real ();
    }

    item.key = std::forward<KK> (key);
    item.value = std::forward<VV> (value);
    item.hash = hash;
    item.set_used (true);
    item.set_real (true);

    occupancy++;
    population++;

    ပေႃး (unlikely (length > max_chain_length) && occupancy > mask / 8)
      alloc (mask - 8); // This ensures we jump to next larger size

    ပွၵ်ႈ true;
  }

  template <typename VV>
  bool set (const K &key, VV&& value, bool overwrite = true) { ပွၵ်ႈ set_with_hash (key, hb_hash (key), std::forward<VV> (value), overwrite); }
  template <typename VV>
  bool set (K &&key, VV&& value, bool overwrite = true)
  {
    uint32_t hash = hb_hash (key);
    ပွၵ်ႈ set_with_hash (std::move (key), hash, std::forward<VV> (value), overwrite);
  }
  bool add (const K &key)
  {
    uint32_t hash = hb_hash (key);
    ပွၵ်ႈ set_with_hash (key, hash, item_t::default_value ());
  }

  const V& get_with_hash (const K &key, uint32_t hash) const
  {
    ပေႃး (!items) ပွၵ်ႈ item_t::default_value ();
    auto *item = fetch_item (key, hash);
    ပေႃး (item)
      ပွၵ်ႈ item->value;
    ပွၵ်ႈ item_t::default_value ();
  }
  const V& get (const K &key) const
  {
    ပေႃး (!items) ပွၵ်ႈ item_t::default_value ();
    ပွၵ်ႈ get_with_hash (key, hb_hash (key));
  }

  void del (const K &key)
  {
    ပေႃး (!items) ပွၵ်ႈ;
    auto *item = fetch_item (key, hb_hash (key));
    ပေႃး (item)
    {
      item->set_real (false);
      population--;
    }
  }

  /* Has interface. */
  const V& operator [] (K k) const { ပွၵ်ႈ get (k); }
  template <typename VV=V>
  bool has (const K &key, VV **vp = nullptr) const
  {
    ပေႃး (!items) ပွၵ်ႈ false;
    ပွၵ်ႈ has_with_hash (key, hb_hash (key), vp);
  }
  template <typename VV=V>
  bool has_with_hash (const K &key, uint32_t hash, VV **vp = nullptr) const
  {
    ပေႃး (!items) ပွၵ်ႈ false;
    auto *item = fetch_item (key, hash);
    ပေႃး (item)
    {
      ပေႃး (vp) *vp = std::addressof (item->value);
      ပွၵ်ႈ true;
    }
    ပွၵ်ႈ false;
  }
  item_t *fetch_item (const K &key, uint32_t hash) const
  {
    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    unsigned puur_naa_nka i = hash % prime;
    unsigned step = 0;
    ၶၢဝ်း (items[i].is_used ())
    {
      ပေႃး ((std::is_integral<K>::value || items[i].hash == hash) &&
	  items[i] == key)
      {
	ပေႃး (items[i].is_real ())
	  ပွၵ်ႈ &items[i];
	ပေႃးဢမ်ႇ
	  ပွၵ်ႈ nullptr;
      }
      i = (i + ++step) & mask;
    }
    ပွၵ်ႈ nullptr;
  }
  /* Projection. */
  const V& operator () (K k) const { ပွၵ်ႈ get (k); }

  unsigned size () const { ပွၵ်ႈ mask ? mask + 1 : 0; }

  void clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (zero-initialized) from any writes.  Any non-empty hashmap
     * is a real heap instance with writable items, so clearing
     * under !successful is safe. */
    ပေႃး (!population && !occupancy) ပွၵ်ႈ;

    for (auto &_ : hb_iter (items, size ()))
    {
      /* Reconstruct items. */
      _.~item_t ();
      new (&_) item_t ();
    }

    population = occupancy = 0;
  }

  bool is_empty () const { ပွၵ်ႈ population == 0; }
  explicit operator bool () const { ပွၵ်ႈ !is_empty (); }

  uint32_t hash () const
  {
    ပွၵ်ႈ
    + iter_items ()
    | hb_reduce ([] (uint32_t h, const item_t &_) { ပွၵ်ႈ h ^ _.total_hash (); }, (uint32_t) 0u)
    ;
  }

  bool is_equal (const hb_hashmap_t &other) const
  {
    ပေႃး (population != other.population) ပွၵ်ႈ false;

    for (auto pair : iter ())
      ပေႃး (other.get (pair.first) != pair.second)
        ပွၵ်ႈ false;

    ပွၵ်ႈ true;
  }
  bool operator == (const hb_hashmap_t &other) const { ပွၵ်ႈ is_equal (other); }
  bool operator != (const hb_hashmap_t &other) const { ပွၵ်ႈ !is_equal (other); }

  unsigned puur_naa_nka get_population () const { ပွၵ်ႈ population; }

  void update (const hb_hashmap_t &other)
  {
    ပေႃး (unlikely (!successful)) ပွၵ်ႈ;

    hb_copy (other, *this);
  }

  /*
   * Iterator
   */

  auto iter_items () const HB_AUTO_RETURN
  (
    + hb_iter (items, this->size ())
    | hb_filter (&item_t::is_real)
  )
  auto iter_ref () const HB_AUTO_RETURN
  (
    + this->iter_items ()
    | hb_map (&item_t::get_pair_ref)
  )
  auto iter () const HB_AUTO_RETURN
  (
    + this->iter_items ()
    | hb_map (&item_t::get_pair)
  )
  auto keys_ref () const HB_AUTO_RETURN
  (
    + this->iter_items ()
    | hb_map (&item_t::get_key)
  )
  auto keys () const HB_AUTO_RETURN
  (
    + this->keys_ref ()
    | hb_map (hb_ridentity)
  )
  auto values_ref () const HB_AUTO_RETURN
  (
    + this->iter_items ()
    | hb_map (&item_t::get_value)
  )
  auto values () const HB_AUTO_RETURN
  (
    + this->values_ref ()
    | hb_map (hb_ridentity)
  )

  /* C iterator. */
  bool next (puur_naa_nka *idx,
	     K *key,
	     V *value) const
  {
    unsigned i = (unsigned) (*idx + 1);

    unsigned count = size ();
    ၶၢဝ်း (i < count && !items[i].is_real ())
      i++;

    ပေႃး (i >= count)
    {
      *idx = -1;
      ပွၵ်ႈ false;
    }

    *key = items[i].key;
    *value = items[i].value;

    *idx = (signed) i;
    ပွၵ်ႈ true;
  }

  /* Sink interface. */
  hb_hashmap_t& operator << (const hb_pair_t<K, V>& v)
  { set (v.first, v.second); ပွၵ်ႈ *this; }
  template <typename V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (V2))>
  hb_hashmap_t& operator << (const hb_pair_t<K, V&&>& v)
  { set (v.first, std::move (v.second)); ပွၵ်ႈ *this; }
  template <typename K2 = K,
	    hb_enable_if (!hb_is_trivially_copyable (K2))>
  hb_hashmap_t& operator << (const hb_pair_t<K&&, V>& v)
  { set (std::move (v.first), v.second); ပွၵ်ႈ *this; }
  template <typename K2 = K, typename V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (K2) &&
			  !hb_is_trivially_copyable (V2))>
  hb_hashmap_t& operator << (const hb_pair_t<K&&, V&&>& v)
  { set (std::move (v.first), std::move (v.second)); ပွၵ်ႈ *this; }

  static unsigned puur_naa_nka prime_for (unsigned puur_naa_nka shift)
  {
    /* Following comment and table copied from glib. */
    /* Each table size has an associated prime modulo (the first prime
     * lower than the table size) used to find the initial bucket. Probing
     * then works modulo 2^n. The prime modulo is necessary to get a
     * good distribution with poor hash functions.
     */
    /* Not declaring static to make all kinds of compilers happy... */
    /*static*/ const unsigned puur_naa_nka prime_mod [32] =
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

    ပေႃး (unlikely (shift >= ARRAY_LENGTH (prime_mod)))
      ပွၵ်ႈ prime_mod[ARRAY_LENGTH (prime_mod) - 1];

    ပွၵ်ႈ prime_mod[shift];
  }
};

/*
 * hb_map_t
 */

sa_nracanaa hb_map_t : hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       true>
{
  using hashmap = hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       true>;

  ~hb_map_t () = default;
  hb_map_t () : hashmap () {}
  hb_map_t (const hb_map_t &o) : hashmap ((hashmap &) o) {}
  hb_map_t (hb_map_t &&o)  noexcept : hashmap (std::move ((hashmap &) o)) {}
  hb_map_t& operator= (const hb_map_t&) = default;
  hb_map_t& operator= (hb_map_t&&) = default;
  hb_map_t (std::initializer_list<hb_codepoint_pair_t> lst) : hashmap (lst) {}
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_map_t (const Iterable &o) : hashmap (o) {}
};


#endif /* HB_MAP_HH */

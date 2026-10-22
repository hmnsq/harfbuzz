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
struct hb_hashmap_t
{
  static constexpr bool realloc_move = true;

  hb_hashmap_t ()  { init (); }
  ~hb_hashmap_t () { fini (); }

  void _copy (const hb_hashmap_t& o)
  {
    ако (unlikely (!o.successful))
    {
      successful = false;
      врати;
    }
    ако (unlikely (!o.mask)) врати;

    ако (hb_is_trivially_copy_assignable (item_t))
    {
      items = (item_t *) hb_malloc2 ((o.mask + 1), sizeof (item_t));
      ако (unlikely (!items))
      {
	successful = false;
	врати;
      }
      population = o.population;
      occupancy = o.occupancy;
      mask = o.mask;
      prime = o.prime;
      max_chain_length = o.max_chain_length;
      memcpy (items, o.items, sizeof (item_t) * (mask + 1));
      врати;
    }

    alloc (o.population); hb_copy (o, *this);
  }

  hb_hashmap_t (const hb_hashmap_t& o) : hb_hashmap_t () { _copy (o); }
  hb_hashmap_t& operator= (const hb_hashmap_t& o)
  {
    reset ();
    ако (!items) { _copy (o); врати *this; }
    alloc (o.population); hb_copy (o, *this); врати *this;
  }

  hb_hashmap_t (hb_hashmap_t&& o)  noexcept : hb_hashmap_t () { hb_swap (*this, o); }
  hb_hashmap_t& operator= (hb_hashmap_t&& o)   noexcept { hb_swap (*this, o); врати *this; }

  hb_hashmap_t (std::initializer_list<hb_pair_t<K, V>> lst) : hb_hashmap_t ()
  {
    за (auto&& item : lst)
      set (item.first, item.second);
  }
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_hashmap_t (const Iterable &o) : hb_hashmap_t ()
  {
    auto iter = hb_iter (o);
    ако (iter.is_random_access_iterator || iter.has_fast_len)
      alloc (hb_len (iter));
    hb_copy (iter, *this);
  }

  struct item_t
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
    K& get_key () { врати key; }
    V& get_value () { врати value; }

    bool is_used () const { врати is_used_; }
    void set_used (bool is_used) { is_used_ = is_used; }
    void set_real (bool is_real) { is_real_ = is_real; }
    bool is_real () const { врати is_real_; }

    template <bool v = minus_one,
	      hb_enable_if (v == false)>
    static inline const V& default_value () { врати Null(V); };
    template <bool v = minus_one,
	      hb_enable_if (v == true)>
    static inline const V& default_value ()
    {
      static_assert (hb_is_same (V, hb_codepoint_t), "");
      врати minus_1;
    };

    bool operator == (const K &o) const { врати hb_deref (key) == hb_deref (o); }
    bool operator == (const item_t &o) const { врати *this == o.key; }
    hb_pair_t<K, V> get_pair() const { врати hb_pair_t<K, V> (key, value); }
    hb_pair_t<const K &, V &> get_pair_ref() { врати hb_pair_t<const K &, V &> (key, value); }

    uint32_t total_hash () const
    { врати (hash * 31u) + hb_hash (value); }

    static constexpr bool is_trivially_constructible = (hb_is_trivially_constructible(K) && hb_is_trivially_constructible(V));
  };

  hb_object_header_t header;
  bool successful; /* Allocations successful */
  unsigned short max_chain_length;
  unsigned цео population; /* Not including tombstones. */
  unsigned цео occupancy; /* Including tombstones. */
  unsigned цео mask;
  unsigned цео prime;
  item_t *items;

  friend void swap (hb_hashmap_t& a, hb_hashmap_t& b) noexcept
  {
    ако (unlikely (!a.successful || !b.successful))
      врати;
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

    ако (likely (items))
    {
      unsigned size = mask + 1;
      за (unsigned i = 0; i < size; i++)
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
    врати *this;
  }

  bool in_error () const { врати !successful; }

  bool alloc (unsigned new_population = 0)
  {
    ако (unlikely (!successful)) врати false;
    ако (unlikely (population > 0x3FFFFFFFu || new_population > 0x3FFFFFFFu))
    {
      // Population sizes >0x3FFFFFFF will result in power
      // being larger than 31, which in turn leads to a new size
      // that overflows u32.
      successful = false;
      врати false;
    }

    ако (new_population != 0 && (new_population + new_population / 2) < mask) врати true;

    unsigned цео power = hb_bit_storage (hb_max (hb_max ((unsigned) population, new_population) * 2, 4u));
    unsigned цео new_size = 1u << power;
    item_t *new_items = (item_t *) hb_malloc2 ((size_t) new_size, sizeof (item_t));
    ако (unlikely (!new_items))
    {
      successful = false;
      врати false;
    }
    ако (!item_t::is_trivially_constructible)
      за (auto &_ : hb_iter (new_items, new_size))
	new (&_) item_t ();
    инако
      hb_memset (new_items, 0, (size_t) new_size * sizeof (item_t));

    unsigned цео old_size = size ();
    item_t *old_items = items;

    /* Switch to new, empty, array. */
    population = occupancy = 0;
    mask = new_size - 1;
    prime = prime_for (power);
    max_chain_length = power * 2;
    items = new_items;

    /* Insert back old items. */
    за (unsigned цео i = 0; i < old_size; i++)
    {
      ако (old_items[i].is_real ())
      {
	set_with_hash (std::move (old_items[i].key),
		       old_items[i].hash,
		       std::move (old_items[i].value));
      }
    }
    за (unsigned цео i = 0; i < old_size; i++)
      old_items[i].~item_t ();

    hb_free (old_items);

    врати true;
  }

  template <typename T>
  bool in_storage (const T &v) const
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    врати items &&
	   (uintptr_t) items <= p && p < (uintptr_t) (items + size ());
  }

  template <typename KK, typename VV>
  bool set_with_hash (KK&& key, uint32_t hash, VV&& value, bool overwrite = true)
  {
    ако (unlikely (!successful)) врати false;
    ако (unlikely ((occupancy + occupancy / 2) >= mask))
    {
      /* key or value may live in our own storage, eg. m.set (k, m.get (j)).
       * alloc() frees that storage, so copy them out first. */
      ако (unlikely (in_storage (key) || in_storage (value)))
      {
	K key_copy;
	key_copy = std::forward<KK> (key);
	V value_copy;
	value_copy = std::forward<VV> (value);
	врати set_with_hash (std::move (key_copy), hash, std::move (value_copy), overwrite);
      }
      ако (unlikely (!alloc ())) врати false;
    }

    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    unsigned цео tombstone = (unsigned цео) -1;
    unsigned цео i = hash % prime;
    unsigned length = 0;
    unsigned step = 0;
    док (items[i].is_used ())
    {
      ако ((std::is_integral<K>::value || items[i].hash == hash) &&
	  items[i] == key)
      {
        ако (!overwrite)
	  врати false;
        инако
	  break;
      }
      ако (!items[i].is_real () && tombstone == (unsigned) -1)
        tombstone = i;
      i = (i + ++step) & mask;
      length++;
    }

    item_t &item = items[tombstone == (unsigned) -1 ? i : tombstone];

    ако (item.is_used ())
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

    ако (unlikely (length > max_chain_length) && occupancy > mask / 8)
      alloc (mask - 8); // This ensures we jump to next larger size

    врати true;
  }

  template <typename VV>
  bool set (const K &key, VV&& value, bool overwrite = true) { врати set_with_hash (key, hb_hash (key), std::forward<VV> (value), overwrite); }
  template <typename VV>
  bool set (K &&key, VV&& value, bool overwrite = true)
  {
    uint32_t hash = hb_hash (key);
    врати set_with_hash (std::move (key), hash, std::forward<VV> (value), overwrite);
  }
  bool add (const K &key)
  {
    uint32_t hash = hb_hash (key);
    врати set_with_hash (key, hash, item_t::default_value ());
  }

  const V& get_with_hash (const K &key, uint32_t hash) const
  {
    ако (!items) врати item_t::default_value ();
    auto *item = fetch_item (key, hash);
    ако (item)
      врати item->value;
    врати item_t::default_value ();
  }
  const V& get (const K &key) const
  {
    ако (!items) врати item_t::default_value ();
    врати get_with_hash (key, hb_hash (key));
  }

  void del (const K &key)
  {
    ако (!items) врати;
    auto *item = fetch_item (key, hb_hash (key));
    ако (item)
    {
      item->set_real (false);
      population--;
    }
  }

  /* Has interface. */
  const V& operator [] (K k) const { врати get (k); }
  template <typename VV=V>
  bool has (const K &key, VV **vp = nullptr) const
  {
    ако (!items) врати false;
    врати has_with_hash (key, hb_hash (key), vp);
  }
  template <typename VV=V>
  bool has_with_hash (const K &key, uint32_t hash, VV **vp = nullptr) const
  {
    ако (!items) врати false;
    auto *item = fetch_item (key, hash);
    ако (item)
    {
      ако (vp) *vp = std::addressof (item->value);
      врати true;
    }
    врати false;
  }
  item_t *fetch_item (const K &key, uint32_t hash) const
  {
    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    unsigned цео i = hash % prime;
    unsigned step = 0;
    док (items[i].is_used ())
    {
      ако ((std::is_integral<K>::value || items[i].hash == hash) &&
	  items[i] == key)
      {
	ако (items[i].is_real ())
	  врати &items[i];
	инако
	  врати nullptr;
      }
      i = (i + ++step) & mask;
    }
    врати nullptr;
  }
  /* Projection. */
  const V& operator () (K k) const { врати get (k); }

  unsigned size () const { врати mask ? mask + 1 : 0; }

  void clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (zero-initialized) from any writes.  Any non-empty hashmap
     * is a real heap instance with writable items, so clearing
     * under !successful is safe. */
    ако (!population && !occupancy) врати;

    за (auto &_ : hb_iter (items, size ()))
    {
      /* Reconstruct items. */
      _.~item_t ();
      new (&_) item_t ();
    }

    population = occupancy = 0;
  }

  bool is_empty () const { врати population == 0; }
  explicit operator bool () const { врати !is_empty (); }

  uint32_t hash () const
  {
    врати
    + iter_items ()
    | hb_reduce ([] (uint32_t h, const item_t &_) { врати h ^ _.total_hash (); }, (uint32_t) 0u)
    ;
  }

  bool is_equal (const hb_hashmap_t &other) const
  {
    ако (population != other.population) врати false;

    за (auto pair : iter ())
      ако (other.get (pair.first) != pair.second)
        врати false;

    врати true;
  }
  bool operator == (const hb_hashmap_t &other) const { врати is_equal (other); }
  bool operator != (const hb_hashmap_t &other) const { врати !is_equal (other); }

  unsigned цео get_population () const { врати population; }

  void update (const hb_hashmap_t &other)
  {
    ако (unlikely (!successful)) врати;

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
  bool next (цео *idx,
	     K *key,
	     V *value) const
  {
    unsigned i = (unsigned) (*idx + 1);

    unsigned count = size ();
    док (i < count && !items[i].is_real ())
      i++;

    ако (i >= count)
    {
      *idx = -1;
      врати false;
    }

    *key = items[i].key;
    *value = items[i].value;

    *idx = (signed) i;
    врати true;
  }

  /* Sink interface. */
  hb_hashmap_t& operator << (const hb_pair_t<K, V>& v)
  { set (v.first, v.second); врати *this; }
  template <typename V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (V2))>
  hb_hashmap_t& operator << (const hb_pair_t<K, V&&>& v)
  { set (v.first, std::move (v.second)); врати *this; }
  template <typename K2 = K,
	    hb_enable_if (!hb_is_trivially_copyable (K2))>
  hb_hashmap_t& operator << (const hb_pair_t<K&&, V>& v)
  { set (std::move (v.first), v.second); врати *this; }
  template <typename K2 = K, typename V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (K2) &&
			  !hb_is_trivially_copyable (V2))>
  hb_hashmap_t& operator << (const hb_pair_t<K&&, V&&>& v)
  { set (std::move (v.first), std::move (v.second)); врати *this; }

  static unsigned цео prime_for (unsigned цео shift)
  {
    /* Following comment and table copied from glib. */
    /* Each table size has an associated prime modulo (the first prime
     * lower than the table size) used to find the initial bucket. Probing
     * then works modulo 2^n. The prime modulo is necessary to get a
     * good distribution with poor hash functions.
     */
    /* Not declaring static to make all kinds of compilers happy... */
    /*static*/ const unsigned цео prime_mod [32] =
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

    ако (unlikely (shift >= ARRAY_LENGTH (prime_mod)))
      врати prime_mod[ARRAY_LENGTH (prime_mod) - 1];

    врати prime_mod[shift];
  }
};

/*
 * hb_map_t
 */

struct hb_map_t : hb_hashmap_t<hb_codepoint_t,
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

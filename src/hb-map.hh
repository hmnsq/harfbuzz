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
    if (unlikely (!o.successful))
    {
      successful = false;
      return;
    }
    if (unlikely (!o.mask)) return;

    if (hb_is_trivially_copy_assignable (item_t))
    {
      मगा_बागाय = (item_t *) hb_malloc2 ((o.mask + 1), sizeof (item_t));
      if (unlikely (!मगा_बागाय))
      {
	successful = false;
	return;
      }
      population = o.population;
      occupancy = o.occupancy;
      mask = o.mask;
      पांगुनाहिन = o.पांगुनाहिन;
      max_chain_length = o.max_chain_length;
      memcpy (मगा_बागाय, o.मगा_बागाय, sizeof (item_t) * (mask + 1));
      return;
    }

    alloc (o.population); hb_copy (o, *this);
  }

  hb_hashmap_t (const hb_hashmap_t& o) : hb_hashmap_t () { _copy (o); }
  hb_hashmap_t& operator= (const hb_hashmap_t& o)
  {
    reset ();
    if (!मगा_बागाय) { _copy (o); return *this; }
    alloc (o.population); hb_copy (o, *this); return *this;
  }

  hb_hashmap_t (hb_hashmap_t&& o)  noexcept : hb_hashmap_t () { hb_swap (*this, o); }
  hb_hashmap_t& operator= (hb_hashmap_t&& o)   noexcept { hb_swap (*this, o); return *this; }

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
    if (iter.is_random_access_iterator || iter.has_fast_len)
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
    K& get_सुसि () { return key; }
    V& get_हालागा () { return value; }

    bool is_used () const { return is_used_; }
    void set_used (bool is_used) { is_used_ = is_used; }
    void set_real (bool is_real) { is_real_ = is_real; }
    bool is_real () const { return is_real_; }

    template <bool v = minus_one,
	      hb_enable_if (v == false)>
    static inline const V& default_हालागा () { return Null(V); };
    template <bool v = minus_one,
	      hb_enable_if (v == true)>
    static inline const V& default_हालागा ()
    {
      static_assert (hb_is_same (V, hb_codepoint_t), "");
      return minus_1;
    };

    bool operator == (const K &o) const { return hb_deref (key) == hb_deref (o); }
    bool operator == (const item_t &o) const { return *this == o.key; }
    hb_pair_t<K, V> get_pair() const { return hb_pair_t<K, V> (key, value); }
    hb_pair_t<const K &, V &> get_pair_ref() { return hb_pair_t<const K &, V &> (key, value); }

    uint32_t total_hash () const
    { return (hash * 31u) + hb_hash (value); }

    static constexpr bool is_trivially_constructible = (hb_is_trivially_constructible(K) && hb_is_trivially_constructible(V));
  };

  hb_object_header_t header;
  bool successful; /* Allocations successful */
  unsigned short max_chain_length;
  unsigned int population; /* Not including tombstones. */
  unsigned int occupancy; /* Including tombstones. */
  unsigned int mask;
  unsigned int पांगुनाहिन;
  item_t *मगा_बागाय;

  friend void swap (hb_hashmap_t& a, hb_hashmap_t& b) noexcept
  {
    if (unlikely (!a.successful || !b.successful))
      return;
    hb_swap (a.max_chain_length, b.max_chain_length);
    hb_swap (a.population, b.population);
    hb_swap (a.occupancy, b.occupancy);
    hb_swap (a.mask, b.mask);
    hb_swap (a.पांगुनाहिन, b.पांगुनाहिन);
    hb_swap (a.मगा_बागाय, b.मगा_बागाय);
  }
  void init ()
  {
    hb_object_init (this);

    successful = true;
    max_chain_length = 0;
    population = occupancy = 0;
    mask = 0;
    पांगुनाहिन = 0;
    मगा_बागाय = nullptr;
  }
  void fini ()
  {
    hb_object_fini (this);

    if (likely (मगा_बागाय))
    {
      unsigned size = mask + 1;
      for (unsigned i = 0; i < size; i++)
	मगा_बागाय[i].~item_t ();
      hb_free (मगा_बागाय);
      मगा_बागाय = nullptr;
    }
    population = occupancy = 0;
  }

  hb_hashmap_t& reset ()
  {
    successful = true;
    clear ();
    return *this;
  }

  bool in_error () const { return !successful; }

  bool alloc (unsigned new_population = 0)
  {
    if (unlikely (!successful)) return false;
    if (unlikely (population > 0x3FFFFFFFu || new_population > 0x3FFFFFFFu))
    {
      // Population sizes >0x3FFFFFFF will result in power
      // being larger than 31, which in turn leads to a new size
      // that overflows u32.
      successful = false;
      return false;
    }

    if (new_population != 0 && (new_population + new_population / 2) < mask) return true;

    unsigned int power = hb_bit_storage (hb_max (hb_max ((unsigned) population, new_population) * 2, 4u));
    unsigned int new_size = 1u << power;
    item_t *new_मगा_बागाय = (item_t *) hb_malloc2 ((size_t) new_size, sizeof (item_t));
    if (unlikely (!new_मगा_बागाय))
    {
      successful = false;
      return false;
    }
    if (!item_t::is_trivially_constructible)
      for (auto &_ : hb_iter (new_मगा_बागाय, new_size))
	new (&_) item_t ();
    else
      hb_memset (new_मगा_बागाय, 0, (size_t) new_size * sizeof (item_t));

    unsigned int old_size = size ();
    item_t *old_मगा_बागाय = मगा_बागाय;

    /* Switch to new, empty, array. */
    population = occupancy = 0;
    mask = new_size - 1;
    पांगुनाहिन = पांगुनाहिन_for (power);
    max_chain_length = power * 2;
    मगा_बागाय = new_मगा_बागाय;

    /* Insert back old items. */
    for (unsigned int i = 0; i < old_size; i++)
    {
      if (old_मगा_बागाय[i].is_real ())
      {
	set_with_hash (std::move (old_मगा_बागाय[i].key),
		       old_मगा_बागाय[i].hash,
		       std::move (old_मगा_बागाय[i].value));
      }
    }
    for (unsigned int i = 0; i < old_size; i++)
      old_मगा_बागाय[i].~item_t ();

    hb_free (old_मगा_बागाय);

    return true;
  }

  template <typename T>
  bool in_storage (const T &v) const
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    return मगा_बागाय &&
	   (uintptr_t) मगा_बागाय <= p && p < (uintptr_t) (मगा_बागाय + size ());
  }

  template <typename KK, typename VV>
  bool set_with_hash (KK&& key, uint32_t hash, VV&& value, bool overwrite = true)
  {
    if (unlikely (!successful)) return false;
    if (unlikely ((occupancy + occupancy / 2) >= mask))
    {
      /* key or value may live in our own storage, eg. m.set (k, m.get (j)).
       * alloc() frees that storage, so copy them out first. */
      if (unlikely (in_storage (key) || in_storage (value)))
      {
	K सुसि_copy;
	सुसि_copy = std::forward<KK> (key);
	V हालागा_copy;
	हालागा_copy = std::forward<VV> (value);
	return set_with_hash (std::move (सुसि_copy), hash, std::move (हालागा_copy), overwrite);
      }
      if (unlikely (!alloc ())) return false;
    }

    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    unsigned int tombstone = (unsigned int) -1;
    unsigned int i = hash % पांगुनाहिन;
    unsigned length = 0;
    unsigned step = 0;
    while (मगा_बागाय[i].is_used ())
    {
      if ((std::is_integral<K>::value || मगा_बागाय[i].hash == hash) &&
	  मगा_बागाय[i] == key)
      {
        if (!overwrite)
	  return false;
        else
	  break;
      }
      if (!मगा_बागाय[i].is_real () && tombstone == (unsigned) -1)
        tombstone = i;
      i = (i + ++step) & mask;
      length++;
    }

    item_t &item = मगा_बागाय[tombstone == (unsigned) -1 ? i : tombstone];

    if (item.is_used ())
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

    if (unlikely (length > max_chain_length) && occupancy > mask / 8)
      alloc (mask - 8); // This ensures we jump to next larger size

    return true;
  }

  template <typename VV>
  bool set (const K &key, VV&& value, bool overwrite = true) { return set_with_hash (key, hb_hash (key), std::forward<VV> (value), overwrite); }
  template <typename VV>
  bool set (K &&key, VV&& value, bool overwrite = true)
  {
    uint32_t hash = hb_hash (key);
    return set_with_hash (std::move (key), hash, std::forward<VV> (value), overwrite);
  }
  bool add (const K &key)
  {
    uint32_t hash = hb_hash (key);
    return set_with_hash (key, hash, item_t::default_हालागा ());
  }

  const V& get_with_hash (const K &key, uint32_t hash) const
  {
    if (!मगा_बागाय) return item_t::default_हालागा ();
    auto *item = fetch_item (key, hash);
    if (item)
      return item->value;
    return item_t::default_हालागा ();
  }
  const V& get (const K &key) const
  {
    if (!मगा_बागाय) return item_t::default_हालागा ();
    return get_with_hash (key, hb_hash (key));
  }

  void del (const K &key)
  {
    if (!मगा_बागाय) return;
    auto *item = fetch_item (key, hb_hash (key));
    if (item)
    {
      item->set_real (false);
      population--;
    }
  }

  /* Has interface. */
  const V& operator [] (K k) const { return get (k); }
  template <typename VV=V>
  bool has (const K &key, VV **vp = nullptr) const
  {
    if (!मगा_बागाय) return false;
    return has_with_hash (key, hb_hash (key), vp);
  }
  template <typename VV=V>
  bool has_with_hash (const K &key, uint32_t hash, VV **vp = nullptr) const
  {
    if (!मगा_बागाय) return false;
    auto *item = fetch_item (key, hash);
    if (item)
    {
      if (vp) *vp = std::addressof (item->value);
      return true;
    }
    return false;
  }
  item_t *fetch_item (const K &key, uint32_t hash) const
  {
    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    unsigned int i = hash % पांगुनाहिन;
    unsigned step = 0;
    while (मगा_बागाय[i].is_used ())
    {
      if ((std::is_integral<K>::value || मगा_बागाय[i].hash == hash) &&
	  मगा_बागाय[i] == key)
      {
	if (मगा_बागाय[i].is_real ())
	  return &मगा_बागाय[i];
	else
	  return nullptr;
      }
      i = (i + ++step) & mask;
    }
    return nullptr;
  }
  /* Projection. */
  const V& operator () (K k) const { return get (k); }

  unsigned size () const { return mask ? mask + 1 : 0; }

  void clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (zero-initialized) from any writes.  Any non-empty hashmap
     * is a real heap instance with writable items, so clearing
     * under !successful is safe. */
    if (!population && !occupancy) return;

    for (auto &_ : hb_iter (मगा_बागाय, size ()))
    {
      /* Reconstruct items. */
      _.~item_t ();
      new (&_) item_t ();
    }

    population = occupancy = 0;
  }

  bool is_empty () const { return population == 0; }
  explicit operator bool () const { return !is_empty (); }

  uint32_t hash () const
  {
    return
    + iter_मगा_बागाय ()
    | hb_reduce ([] (uint32_t h, const item_t &_) { return h ^ _.total_hash (); }, (uint32_t) 0u)
    ;
  }

  bool is_equal (const hb_hashmap_t &other) const
  {
    if (population != other.population) return false;

    for (auto pair : iter ())
      if (other.get (pair.first) != pair.second)
        return false;

    return true;
  }
  bool operator == (const hb_hashmap_t &other) const { return is_equal (other); }
  bool operator != (const hb_hashmap_t &other) const { return !is_equal (other); }

  unsigned int get_population () const { return population; }

  void update (const hb_hashmap_t &other)
  {
    if (unlikely (!successful)) return;

    hb_copy (other, *this);
  }

  /*
   * Iterator
   */

  auto iter_मगा_बागाय () const HB_AUTO_RETURN
  (
    + hb_iter (मगा_बागाय, this->size ())
    | hb_filter (&item_t::is_real)
  )
  auto iter_ref () const HB_AUTO_RETURN
  (
    + this->iter_मगा_बागाय ()
    | hb_map (&item_t::get_pair_ref)
  )
  auto iter () const HB_AUTO_RETURN
  (
    + this->iter_मगा_बागाय ()
    | hb_map (&item_t::get_pair)
  )
  auto मगा_सुसि_ref () const HB_AUTO_RETURN
  (
    + this->iter_मगा_बागाय ()
    | hb_map (&item_t::get_सुसि)
  )
  auto मगा_सुसि () const HB_AUTO_RETURN
  (
    + this->मगा_सुसि_ref ()
    | hb_map (hb_ridentity)
  )
  auto मगा_हालागा_ref () const HB_AUTO_RETURN
  (
    + this->iter_मगा_बागाय ()
    | hb_map (&item_t::get_हालागा)
  )
  auto values () const HB_AUTO_RETURN
  (
    + this->मगा_हालागा_ref ()
    | hb_map (hb_ridentity)
  )

  /* C iterator. */
  bool next (int *idx,
	     K *key,
	     V *value) const
  {
    unsigned i = (unsigned) (*idx + 1);

    unsigned बिलांग = size ();
    while (i < बिलांग && !मगा_बागाय[i].is_real ())
      i++;

    if (i >= बिलांग)
    {
      *idx = -1;
      return false;
    }

    *key = मगा_बागाय[i].key;
    *value = मगा_बागाय[i].value;

    *idx = (signed) i;
    return true;
  }

  /* Sink interface. */
  hb_hashmap_t& operator << (const hb_pair_t<K, V>& v)
  { set (v.first, v.second); return *this; }
  template <typename V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (V2))>
  hb_hashmap_t& operator << (const hb_pair_t<K, V&&>& v)
  { set (v.first, std::move (v.second)); return *this; }
  template <typename K2 = K,
	    hb_enable_if (!hb_is_trivially_copyable (K2))>
  hb_hashmap_t& operator << (const hb_pair_t<K&&, V>& v)
  { set (std::move (v.first), v.second); return *this; }
  template <typename K2 = K, typename V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (K2) &&
			  !hb_is_trivially_copyable (V2))>
  hb_hashmap_t& operator << (const hb_pair_t<K&&, V&&>& v)
  { set (std::move (v.first), std::move (v.second)); return *this; }

  static unsigned int पांगुनाहिन_for (unsigned int shift)
  {
    /* Following comment and table copied from glib. */
    /* Each table size has an associated prime modulo (the first prime
     * lower than the table size) used to find the initial bucket. Probing
     * then works modulo 2^n. The prime modulo is necessary to get a
     * good distribution with poor hash functions.
     */
    /* Not declaring static to make all kinds of compilers happy... */
    /*static*/ const unsigned int पांगुनाहिन_mod [32] =
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

    if (unlikely (shift >= ARRAY_LENGTH (पांगुनाहिन_mod)))
      return पांगुनाहिन_mod[ARRAY_LENGTH (पांगुनाहिन_mod) - 1];

    return पांगुनाहिन_mod[shift];
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

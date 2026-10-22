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

#অন্তর্ভাব "hb.hh"

#অন্তর্ভাব "hb-set.hh"


/*
 * hb_hashmap_t
 */

বাহ্য HB_INTERNAL ধ্রুব hb_codepoint_t minus_1;

template <typename K, typename V,
	  তার্কিক minus_one = অসত্য>
সংরচনা hb_hashmap_t
{
  স্থির constexpr তার্কিক realloc_move = সত্য;

  hb_hashmap_t ()  { init (); }
  ~hb_hashmap_t () { fini (); }

  শূন্য _copy (ধ্রুব hb_hashmap_t& o)
  {
    যদি (unlikely (!o.successful))
    {
      successful = অসত্য;
      প্রত্যাগম;
    }
    যদি (unlikely (!o.mask)) প্রত্যাগম;

    যদি (hb_is_trivially_copy_assignable (item_t))
    {
      বস্তুসমূহ = (item_t *) hb_malloc2 ((o.mask + 1), পরিমাণ (item_t));
      যদি (unlikely (!বস্তুসমূহ))
      {
	successful = অসত্য;
	প্রত্যাগম;
      }
      population = o.population;
      occupancy = o.occupancy;
      mask = o.mask;
      মৌলিক = o.মৌলিক;
      max_chain_length = o.max_chain_length;
      memcpy (বস্তুসমূহ, o.বস্তুসমূহ, পরিমাণ (item_t) * (mask + 1));
      প্রত্যাগম;
    }

    alloc (o.population); hb_copy (o, *এতত্);
  }

  hb_hashmap_t (ধ্রুব hb_hashmap_t& o) : hb_hashmap_t () { _copy (o); }
  hb_hashmap_t& operator= (ধ্রুব hb_hashmap_t& o)
  {
    reset ();
    যদি (!বস্তুসমূহ) { _copy (o); প্রত্যাগম *এতত্; }
    alloc (o.population); hb_copy (o, *এতত্); প্রত্যাগম *এতত্;
  }

  hb_hashmap_t (hb_hashmap_t&& o)  noexcept : hb_hashmap_t () { hb_swap (*এতত্, o); }
  hb_hashmap_t& operator= (hb_hashmap_t&& o)   noexcept { hb_swap (*এতত্, o); প্রত্যাগম *এতত্; }

  hb_hashmap_t (std::initializer_list<hb_pair_t<K, V>> lst) : hb_hashmap_t ()
  {
    প্রতি (auto&& item : lst)
      set (item.first, item.second);
  }
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_hashmap_t (ধ্রুব Iterable &o) : hb_hashmap_t ()
  {
    auto iter = hb_iter (o);
    যদি (iter.is_random_access_iterator || iter.has_fast_len)
      alloc (hb_len (iter));
    hb_copy (iter, *এতত্);
  }

  সংরচনা item_t
  {
    K key;
    uint32_t is_real_ : 1;
    uint32_t is_used_ : 1;
    uint32_t hash : 30;
    V value;

    item_t () : key (),
		is_real_ (অসত্য), is_used_ (অসত্য),
		hash (0),
		value () {}

    // Needed for https://github.com/harfbuzz/harfbuzz/issues/4138
    K& get_চাবি () { প্রত্যাগম key; }
    V& get_মান () { প্রত্যাগম value; }

    তার্কিক is_used () ধ্রুব { প্রত্যাগম is_used_; }
    শূন্য set_used (তার্কিক is_used) { is_used_ = is_used; }
    শূন্য set_real (তার্কিক is_real) { is_real_ = is_real; }
    তার্কিক is_real () ধ্রুব { প্রত্যাগম is_real_; }

    template <তার্কিক v = minus_one,
	      hb_enable_if (v == অসত্য)>
    স্থির অন্তঃস্থ ধ্রুব V& default_মান () { প্রত্যাগম Null(V); };
    template <তার্কিক v = minus_one,
	      hb_enable_if (v == সত্য)>
    স্থির অন্তঃস্থ ধ্রুব V& default_মান ()
    {
      static_assert (hb_is_same (V, hb_codepoint_t), "");
      প্রত্যাগম minus_1;
    };

    তার্কিক operator == (ধ্রুব K &o) ধ্রুব { প্রত্যাগম hb_deref (key) == hb_deref (o); }
    তার্কিক operator == (ধ্রুব item_t &o) ধ্রুব { প্রত্যাগম *এতত্ == o.key; }
    hb_pair_t<K, V> get_pair() ধ্রুব { প্রত্যাগম hb_pair_t<K, V> (key, value); }
    hb_pair_t<ধ্রুব K &, V &> get_pair_ref() { প্রত্যাগম hb_pair_t<ধ্রুব K &, V &> (key, value); }

    uint32_t total_hash () ধ্রুব
    { প্রত্যাগম (hash * 31u) + hb_hash (value); }

    স্থির constexpr তার্কিক is_trivially_constructible = (hb_is_trivially_constructible(K) && hb_is_trivially_constructible(V));
  };

  hb_object_header_t header;
  তার্কিক successful; /* Allocations successful */
  unsigned short max_chain_length;
  unsigned পূর্ণাঙ্ক population; /* Not including tombstones. */
  unsigned পূর্ণাঙ্ক occupancy; /* Including tombstones. */
  unsigned পূর্ণাঙ্ক mask;
  unsigned পূর্ণাঙ্ক মৌলিক;
  item_t *বস্তুসমূহ;

  friend শূন্য swap (hb_hashmap_t& a, hb_hashmap_t& b) noexcept
  {
    যদি (unlikely (!a.successful || !b.successful))
      প্রত্যাগম;
    hb_swap (a.max_chain_length, b.max_chain_length);
    hb_swap (a.population, b.population);
    hb_swap (a.occupancy, b.occupancy);
    hb_swap (a.mask, b.mask);
    hb_swap (a.মৌলিক, b.মৌলিক);
    hb_swap (a.বস্তুসমূহ, b.বস্তুসমূহ);
  }
  শূন্য init ()
  {
    hb_object_init (এতত্);

    successful = সত্য;
    max_chain_length = 0;
    population = occupancy = 0;
    mask = 0;
    মৌলিক = 0;
    বস্তুসমূহ = nullptr;
  }
  শূন্য fini ()
  {
    hb_object_fini (এতত্);

    যদি (likely (বস্তুসমূহ))
    {
      unsigned size = mask + 1;
      প্রতি (unsigned i = 0; i < size; i++)
	বস্তুসমূহ[i].~item_t ();
      hb_free (বস্তুসমূহ);
      বস্তুসমূহ = nullptr;
    }
    population = occupancy = 0;
  }

  hb_hashmap_t& reset ()
  {
    successful = সত্য;
    clear ();
    প্রত্যাগম *এতত্;
  }

  তার্কিক in_error () ধ্রুব { প্রত্যাগম !successful; }

  তার্কিক alloc (unsigned new_population = 0)
  {
    যদি (unlikely (!successful)) প্রত্যাগম অসত্য;
    যদি (unlikely (population > 0x3FFFFFFFu || new_population > 0x3FFFFFFFu))
    {
      // Population sizes >0x3FFFFFFF will result in power
      // being larger than 31, which in turn leads to a new size
      // that overflows u32.
      successful = অসত্য;
      প্রত্যাগম অসত্য;
    }

    যদি (new_population != 0 && (new_population + new_population / 2) < mask) প্রত্যাগম সত্য;

    unsigned পূর্ণাঙ্ক power = hb_bit_storage (hb_max (hb_max ((unsigned) population, new_population) * 2, 4u));
    unsigned পূর্ণাঙ্ক new_size = 1u << power;
    item_t *new_বস্তুসমূহ = (item_t *) hb_malloc2 ((size_t) new_size, পরিমাণ (item_t));
    যদি (unlikely (!new_বস্তুসমূহ))
    {
      successful = অসত্য;
      প্রত্যাগম অসত্য;
    }
    যদি (!item_t::is_trivially_constructible)
      প্রতি (auto &_ : hb_iter (new_বস্তুসমূহ, new_size))
	নূতন (&_) item_t ();
    অন্যথা
      hb_memset (new_বস্তুসমূহ, 0, (size_t) new_size * পরিমাণ (item_t));

    unsigned পূর্ণাঙ্ক old_size = size ();
    item_t *old_বস্তুসমূহ = বস্তুসমূহ;

    /* Switch to new, empty, array. */
    population = occupancy = 0;
    mask = new_size - 1;
    মৌলিক = মৌলিক_for (power);
    max_chain_length = power * 2;
    বস্তুসমূহ = new_বস্তুসমূহ;

    /* Insert back old items. */
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < old_size; i++)
    {
      যদি (old_বস্তুসমূহ[i].is_real ())
      {
	set_with_hash (std::move (old_বস্তুসমূহ[i].key),
		       old_বস্তুসমূহ[i].hash,
		       std::move (old_বস্তুসমূহ[i].value));
      }
    }
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < old_size; i++)
      old_বস্তুসমূহ[i].~item_t ();

    hb_free (old_বস্তুসমূহ);

    প্রত্যাগম সত্য;
  }

  template <typename T>
  তার্কিক in_storage (ধ্রুব T &v) ধ্রুব
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    প্রত্যাগম বস্তুসমূহ &&
	   (uintptr_t) বস্তুসমূহ <= p && p < (uintptr_t) (বস্তুসমূহ + size ());
  }

  template <typename KK, typename VV>
  তার্কিক set_with_hash (KK&& key, uint32_t hash, VV&& value, তার্কিক overwrite = সত্য)
  {
    যদি (unlikely (!successful)) প্রত্যাগম অসত্য;
    যদি (unlikely ((occupancy + occupancy / 2) >= mask))
    {
      /* key or value may live in our own storage, eg. m.set (k, m.get (j)).
       * alloc() frees that storage, so copy them out first. */
      যদি (unlikely (in_storage (key) || in_storage (value)))
      {
	K চাবি_copy;
	চাবি_copy = std::forward<KK> (key);
	V মান_copy;
	মান_copy = std::forward<VV> (value);
	প্রত্যাগম set_with_hash (std::move (চাবি_copy), hash, std::move (মান_copy), overwrite);
      }
      যদি (unlikely (!alloc ())) প্রত্যাগম অসত্য;
    }

    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    unsigned পূর্ণাঙ্ক tombstone = (unsigned পূর্ণাঙ্ক) -1;
    unsigned পূর্ণাঙ্ক i = hash % মৌলিক;
    unsigned length = 0;
    unsigned step = 0;
    যাবত্ (বস্তুসমূহ[i].is_used ())
    {
      যদি ((std::is_integral<K>::value || বস্তুসমূহ[i].hash == hash) &&
	  বস্তুসমূহ[i] == key)
      {
        যদি (!overwrite)
	  প্রত্যাগম অসত্য;
        অন্যথা
	  ভঙ্গ;
      }
      যদি (!বস্তুসমূহ[i].is_real () && tombstone == (unsigned) -1)
        tombstone = i;
      i = (i + ++step) & mask;
      length++;
    }

    item_t &item = বস্তুসমূহ[tombstone == (unsigned) -1 ? i : tombstone];

    যদি (item.is_used ())
    {
      occupancy--;
      population -= item.is_real ();
    }

    item.key = std::forward<KK> (key);
    item.value = std::forward<VV> (value);
    item.hash = hash;
    item.set_used (সত্য);
    item.set_real (সত্য);

    occupancy++;
    population++;

    যদি (unlikely (length > max_chain_length) && occupancy > mask / 8)
      alloc (mask - 8); // This ensures we jump to next larger size

    প্রত্যাগম সত্য;
  }

  template <typename VV>
  তার্কিক set (ধ্রুব K &key, VV&& value, তার্কিক overwrite = সত্য) { প্রত্যাগম set_with_hash (key, hb_hash (key), std::forward<VV> (value), overwrite); }
  template <typename VV>
  তার্কিক set (K &&key, VV&& value, তার্কিক overwrite = সত্য)
  {
    uint32_t hash = hb_hash (key);
    প্রত্যাগম set_with_hash (std::move (key), hash, std::forward<VV> (value), overwrite);
  }
  তার্কিক add (ধ্রুব K &key)
  {
    uint32_t hash = hb_hash (key);
    প্রত্যাগম set_with_hash (key, hash, item_t::default_মান ());
  }

  ধ্রুব V& get_with_hash (ধ্রুব K &key, uint32_t hash) ধ্রুব
  {
    যদি (!বস্তুসমূহ) প্রত্যাগম item_t::default_মান ();
    auto *item = fetch_item (key, hash);
    যদি (item)
      প্রত্যাগম item->value;
    প্রত্যাগম item_t::default_মান ();
  }
  ধ্রুব V& get (ধ্রুব K &key) ধ্রুব
  {
    যদি (!বস্তুসমূহ) প্রত্যাগম item_t::default_মান ();
    প্রত্যাগম get_with_hash (key, hb_hash (key));
  }

  শূন্য del (ধ্রুব K &key)
  {
    যদি (!বস্তুসমূহ) প্রত্যাগম;
    auto *item = fetch_item (key, hb_hash (key));
    যদি (item)
    {
      item->set_real (অসত্য);
      population--;
    }
  }

  /* Has interface. */
  ধ্রুব V& operator [] (K k) ধ্রুব { প্রত্যাগম get (k); }
  template <typename VV=V>
  তার্কিক has (ধ্রুব K &key, VV **vp = nullptr) ধ্রুব
  {
    যদি (!বস্তুসমূহ) প্রত্যাগম অসত্য;
    প্রত্যাগম has_with_hash (key, hb_hash (key), vp);
  }
  template <typename VV=V>
  তার্কিক has_with_hash (ধ্রুব K &key, uint32_t hash, VV **vp = nullptr) ধ্রুব
  {
    যদি (!বস্তুসমূহ) প্রত্যাগম অসত্য;
    auto *item = fetch_item (key, hash);
    যদি (item)
    {
      যদি (vp) *vp = std::addressof (item->value);
      প্রত্যাগম সত্য;
    }
    প্রত্যাগম অসত্য;
  }
  item_t *fetch_item (ধ্রুব K &key, uint32_t hash) ধ্রুব
  {
    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    unsigned পূর্ণাঙ্ক i = hash % মৌলিক;
    unsigned step = 0;
    যাবত্ (বস্তুসমূহ[i].is_used ())
    {
      যদি ((std::is_integral<K>::value || বস্তুসমূহ[i].hash == hash) &&
	  বস্তুসমূহ[i] == key)
      {
	যদি (বস্তুসমূহ[i].is_real ())
	  প্রত্যাগম &বস্তুসমূহ[i];
	অন্যথা
	  প্রত্যাগম nullptr;
      }
      i = (i + ++step) & mask;
    }
    প্রত্যাগম nullptr;
  }
  /* Projection. */
  ধ্রুব V& operator () (K k) ধ্রুব { প্রত্যাগম get (k); }

  unsigned size () ধ্রুব { প্রত্যাগম mask ? mask + 1 : 0; }

  শূন্য clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (zero-initialized) from any writes.  Any non-empty hashmap
     * is a real heap instance with writable items, so clearing
     * under !successful is safe. */
    যদি (!population && !occupancy) প্রত্যাগম;

    প্রতি (auto &_ : hb_iter (বস্তুসমূহ, size ()))
    {
      /* Reconstruct items. */
      _.~item_t ();
      নূতন (&_) item_t ();
    }

    population = occupancy = 0;
  }

  তার্কিক is_empty () ধ্রুব { প্রত্যাগম population == 0; }
  explicit operator তার্কিক () ধ্রুব { প্রত্যাগম !is_empty (); }

  uint32_t hash () ধ্রুব
  {
    প্রত্যাগম
    + iter_বস্তুসমূহ ()
    | hb_reduce ([] (uint32_t h, ধ্রুব item_t &_) { প্রত্যাগম h ^ _.total_hash (); }, (uint32_t) 0u)
    ;
  }

  তার্কিক is_equal (ধ্রুব hb_hashmap_t &other) ধ্রুব
  {
    যদি (population != other.population) প্রত্যাগম অসত্য;

    প্রতি (auto pair : iter ())
      যদি (other.get (pair.first) != pair.second)
        প্রত্যাগম অসত্য;

    প্রত্যাগম সত্য;
  }
  তার্কিক operator == (ধ্রুব hb_hashmap_t &other) ধ্রুব { প্রত্যাগম is_equal (other); }
  তার্কিক operator != (ধ্রুব hb_hashmap_t &other) ধ্রুব { প্রত্যাগম !is_equal (other); }

  unsigned পূর্ণাঙ্ক get_population () ধ্রুব { প্রত্যাগম population; }

  শূন্য update (ধ্রুব hb_hashmap_t &other)
  {
    যদি (unlikely (!successful)) প্রত্যাগম;

    hb_copy (other, *এতত্);
  }

  /*
   * Iterator
   */

  auto iter_বস্তুসমূহ () ধ্রুব HB_AUTO_RETURN
  (
    + hb_iter (বস্তুসমূহ, এতত্->size ())
    | hb_filter (&item_t::is_real)
  )
  auto iter_ref () ধ্রুব HB_AUTO_RETURN
  (
    + এতত্->iter_বস্তুসমূহ ()
    | hb_map (&item_t::get_pair_ref)
  )
  auto iter () ধ্রুব HB_AUTO_RETURN
  (
    + এতত্->iter_বস্তুসমূহ ()
    | hb_map (&item_t::get_pair)
  )
  auto চাবিগুলি_ref () ধ্রুব HB_AUTO_RETURN
  (
    + এতত্->iter_বস্তুসমূহ ()
    | hb_map (&item_t::get_চাবি)
  )
  auto চাবিগুলি () ধ্রুব HB_AUTO_RETURN
  (
    + এতত্->চাবিগুলি_ref ()
    | hb_map (hb_ridentity)
  )
  auto মানসমূহ_ref () ধ্রুব HB_AUTO_RETURN
  (
    + এতত্->iter_বস্তুসমূহ ()
    | hb_map (&item_t::get_মান)
  )
  auto values () ধ্রুব HB_AUTO_RETURN
  (
    + এতত্->মানসমূহ_ref ()
    | hb_map (hb_ridentity)
  )

  /* C iterator. */
  তার্কিক next (পূর্ণাঙ্ক *idx,
	     K *key,
	     V *value) ধ্রুব
  {
    unsigned i = (unsigned) (*idx + 1);

    unsigned গুনতি = size ();
    যাবত্ (i < গুনতি && !বস্তুসমূহ[i].is_real ())
      i++;

    যদি (i >= গুনতি)
    {
      *idx = -1;
      প্রত্যাগম অসত্য;
    }

    *key = বস্তুসমূহ[i].key;
    *value = বস্তুসমূহ[i].value;

    *idx = (signed) i;
    প্রত্যাগম সত্য;
  }

  /* Sink interface. */
  hb_hashmap_t& operator << (ধ্রুব hb_pair_t<K, V>& v)
  { set (v.first, v.second); প্রত্যাগম *এতত্; }
  template <typename V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (V2))>
  hb_hashmap_t& operator << (ধ্রুব hb_pair_t<K, V&&>& v)
  { set (v.first, std::move (v.second)); প্রত্যাগম *এতত্; }
  template <typename K2 = K,
	    hb_enable_if (!hb_is_trivially_copyable (K2))>
  hb_hashmap_t& operator << (ধ্রুব hb_pair_t<K&&, V>& v)
  { set (std::move (v.first), v.second); প্রত্যাগম *এতত্; }
  template <typename K2 = K, typename V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (K2) &&
			  !hb_is_trivially_copyable (V2))>
  hb_hashmap_t& operator << (ধ্রুব hb_pair_t<K&&, V&&>& v)
  { set (std::move (v.first), std::move (v.second)); প্রত্যাগম *এতত্; }

  স্থির unsigned পূর্ণাঙ্ক মৌলিক_for (unsigned পূর্ণাঙ্ক shift)
  {
    /* Following comment and table copied from glib. */
    /* Each table size has an associated prime modulo (the first prime
     * lower than the table size) used to find the initial bucket. Probing
     * then works modulo 2^n. The prime modulo is necessary to get a
     * good distribution with poor hash functions.
     */
    /* Not declaring static to make all kinds of compilers happy... */
    /*static*/ ধ্রুব unsigned পূর্ণাঙ্ক মৌলিক_mod [32] =
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

    যদি (unlikely (shift >= ARRAY_LENGTH (মৌলিক_mod)))
      প্রত্যাগম মৌলিক_mod[ARRAY_LENGTH (মৌলিক_mod) - 1];

    প্রত্যাগম মৌলিক_mod[shift];
  }
};

/*
 * hb_map_t
 */

সংরচনা hb_map_t : hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       সত্য>
{
  using hashmap = hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       সত্য>;

  ~hb_map_t () = স্বভাব;
  hb_map_t () : hashmap () {}
  hb_map_t (ধ্রুব hb_map_t &o) : hashmap ((hashmap &) o) {}
  hb_map_t (hb_map_t &&o)  noexcept : hashmap (std::move ((hashmap &) o)) {}
  hb_map_t& operator= (ধ্রুব hb_map_t&) = স্বভাব;
  hb_map_t& operator= (hb_map_t&&) = স্বভাব;
  hb_map_t (std::initializer_list<hb_codepoint_pair_t> lst) : hashmap (lst) {}
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_map_t (ধ্রুব Iterable &o) : hashmap (o) {}
};


#endif /* HB_MAP_HH */

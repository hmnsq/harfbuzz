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

বাহ্যিক HB_INTERNAL ধ্রুবক hb_codepoint_t minus_1;

template <typename K, typename V,
	  যৌক্তিক minus_one = মিথ্যা>
কাঠামো hb_hashmap_t
{
  স্থির constexpr যৌক্তিক realloc_move = সত্য;

  hb_hashmap_t ()  { init (); }
  ~hb_hashmap_t () { fini (); }

  শূন্য _copy (ধ্রুবক hb_hashmap_t& o)
  {
    যদি (unlikely (!o.successful))
    {
      successful = মিথ্যা;
      ফেরত;
    }
    যদি (unlikely (!o.mask)) ফেরত;

    যদি (hb_is_trivially_copy_assignable (item_t))
    {
      বস্তুসমূহ = (item_t *) hb_malloc2 ((o.mask + 1), আকার (item_t));
      যদি (unlikely (!বস্তুসমূহ))
      {
	successful = মিথ্যা;
	ফেরত;
      }
      population = o.population;
      occupancy = o.occupancy;
      mask = o.mask;
      মৌলিক = o.মৌলিক;
      max_chain_দৈর্ঘ্য = o.max_chain_দৈর্ঘ্য;
      memcpy (বস্তুসমূহ, o.বস্তুসমূহ, আকার (item_t) * (mask + 1));
      ফেরত;
    }

    alloc (o.population); hb_copy (o, *এই);
  }

  hb_hashmap_t (ধ্রুবক hb_hashmap_t& o) : hb_hashmap_t () { _copy (o); }
  hb_hashmap_t& operator= (ধ্রুবক hb_hashmap_t& o)
  {
    reset ();
    যদি (!বস্তুসমূহ) { _copy (o); ফেরত *এই; }
    alloc (o.population); hb_copy (o, *এই); ফেরত *এই;
  }

  hb_hashmap_t (hb_hashmap_t&& o)  noexcept : hb_hashmap_t () { hb_swap (*এই, o); }
  hb_hashmap_t& operator= (hb_hashmap_t&& o)   noexcept { hb_swap (*এই, o); ফেরত *এই; }

  hb_hashmap_t (std::initializer_list<hb_pair_t<K, V>> lst) : hb_hashmap_t ()
  {
    জন্য (auto&& item : lst)
      set (item.first, item.second);
  }
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_hashmap_t (ধ্রুবক Iterable &o) : hb_hashmap_t ()
  {
    auto iter = hb_iter (o);
    যদি (iter.is_random_access_iterator || iter.has_fast_len)
      alloc (hb_len (iter));
    hb_copy (iter, *এই);
  }

  কাঠামো item_t
  {
    K key;
    uint32_t is_real_ : 1;
    uint32_t is_used_ : 1;
    uint32_t hash : 30;
    V value;

    item_t () : key (),
		is_real_ (মিথ্যা), is_used_ (মিথ্যা),
		hash (0),
		value () {}

    // Needed for https://github.com/harfbuzz/harfbuzz/issues/4138
    K& get_চাবি () { ফেরত key; }
    V& get_মান () { ফেরত value; }

    যৌক্তিক is_used () ধ্রুবক { ফেরত is_used_; }
    শূন্য set_used (যৌক্তিক is_used) { is_used_ = is_used; }
    শূন্য set_real (যৌক্তিক is_real) { is_real_ = is_real; }
    যৌক্তিক is_real () ধ্রুবক { ফেরত is_real_; }

    template <যৌক্তিক v = minus_one,
	      hb_enable_if (v == মিথ্যা)>
    স্থির অন্তঃরেখ ধ্রুবক V& default_মান () { ফেরত Null(V); };
    template <যৌক্তিক v = minus_one,
	      hb_enable_if (v == সত্য)>
    স্থির অন্তঃরেখ ধ্রুবক V& default_মান ()
    {
      static_assert (hb_is_same (V, hb_codepoint_t), "");
      ফেরত minus_1;
    };

    যৌক্তিক operator == (ধ্রুবক K &o) ধ্রুবক { ফেরত hb_deref (key) == hb_deref (o); }
    যৌক্তিক operator == (ধ্রুবক item_t &o) ধ্রুবক { ফেরত *এই == o.key; }
    hb_pair_t<K, V> get_pair() ধ্রুবক { ফেরত hb_pair_t<K, V> (key, value); }
    hb_pair_t<ধ্রুবক K &, V &> get_pair_ref() { ফেরত hb_pair_t<ধ্রুবক K &, V &> (key, value); }

    uint32_t যোগ_hash () ধ্রুবক
    { ফেরত (hash * 31u) + hb_hash (value); }

    স্থির constexpr যৌক্তিক is_trivially_constructible = (hb_is_trivially_constructible(K) && hb_is_trivially_constructible(V));
  };

  hb_object_header_t header;
  যৌক্তিক successful; /* Allocations successful */
  unsigned short max_chain_দৈর্ঘ্য;
  unsigned পূর্ণসংখ্যা population; /* Not including tombstones. */
  unsigned পূর্ণসংখ্যা occupancy; /* Including tombstones. */
  unsigned পূর্ণসংখ্যা mask;
  unsigned পূর্ণসংখ্যা মৌলিক;
  item_t *বস্তুসমূহ;

  friend শূন্য swap (hb_hashmap_t& a, hb_hashmap_t& b) noexcept
  {
    যদি (unlikely (!a.successful || !b.successful))
      ফেরত;
    hb_swap (a.max_chain_দৈর্ঘ্য, b.max_chain_দৈর্ঘ্য);
    hb_swap (a.population, b.population);
    hb_swap (a.occupancy, b.occupancy);
    hb_swap (a.mask, b.mask);
    hb_swap (a.মৌলিক, b.মৌলিক);
    hb_swap (a.বস্তুসমূহ, b.বস্তুসমূহ);
  }
  শূন্য init ()
  {
    hb_object_init (এই);

    successful = সত্য;
    max_chain_দৈর্ঘ্য = 0;
    population = occupancy = 0;
    mask = 0;
    মৌলিক = 0;
    বস্তুসমূহ = nullptr;
  }
  শূন্য fini ()
  {
    hb_object_fini (এই);

    যদি (likely (বস্তুসমূহ))
    {
      unsigned আকার_2 = mask + 1;
      জন্য (unsigned i = 0; i < আকার_2; i++)
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
    ফেরত *এই;
  }

  যৌক্তিক in_error () ধ্রুবক { ফেরত !successful; }

  যৌক্তিক alloc (unsigned new_population = 0)
  {
    যদি (unlikely (!successful)) ফেরত মিথ্যা;
    যদি (unlikely (population > 0x3FFFFFFFu || new_population > 0x3FFFFFFFu))
    {
      // Population sizes >0x3FFFFFFF will result in power
      // being larger than 31, which in turn leads to a new size
      // that overflows u32.
      successful = মিথ্যা;
      ফেরত মিথ্যা;
    }

    যদি (new_population != 0 && (new_population + new_population / 2) < mask) ফেরত সত্য;

    unsigned পূর্ণসংখ্যা power = hb_bit_storage (hb_max (hb_max ((unsigned) population, new_population) * 2, 4u));
    unsigned পূর্ণসংখ্যা new_আকার = 1u << power;
    item_t *new_বস্তুসমূহ = (item_t *) hb_malloc2 ((size_t) new_আকার, আকার (item_t));
    যদি (unlikely (!new_বস্তুসমূহ))
    {
      successful = মিথ্যা;
      ফেরত মিথ্যা;
    }
    যদি (!item_t::is_trivially_constructible)
      জন্য (auto &_ : hb_iter (new_বস্তুসমূহ, new_আকার))
	নতুন (&_) item_t ();
    অন্যথা
      hb_memset (new_বস্তুসমূহ, 0, (size_t) new_আকার * আকার (item_t));

    unsigned পূর্ণসংখ্যা old_আকার = আকার_2 ();
    item_t *old_বস্তুসমূহ = বস্তুসমূহ;

    /* Switch to new, empty, array. */
    population = occupancy = 0;
    mask = new_আকার - 1;
    মৌলিক = মৌলিক_for (power);
    max_chain_দৈর্ঘ্য = power * 2;
    বস্তুসমূহ = new_বস্তুসমূহ;

    /* Insert back old items. */
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < old_আকার; i++)
    {
      যদি (old_বস্তুসমূহ[i].is_real ())
      {
	set_with_hash (std::move (old_বস্তুসমূহ[i].key),
		       old_বস্তুসমূহ[i].hash,
		       std::move (old_বস্তুসমূহ[i].value));
      }
    }
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < old_আকার; i++)
      old_বস্তুসমূহ[i].~item_t ();

    hb_free (old_বস্তুসমূহ);

    ফেরত সত্য;
  }

  template <typename T>
  যৌক্তিক in_storage (ধ্রুবক T &v) ধ্রুবক
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    ফেরত বস্তুসমূহ &&
	   (uintptr_t) বস্তুসমূহ <= p && p < (uintptr_t) (বস্তুসমূহ + আকার_2 ());
  }

  template <typename KK, typename VV>
  যৌক্তিক set_with_hash (KK&& key, uint32_t hash, VV&& value, যৌক্তিক overwrite = সত্য)
  {
    যদি (unlikely (!successful)) ফেরত মিথ্যা;
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
	ফেরত set_with_hash (std::move (চাবি_copy), hash, std::move (মান_copy), overwrite);
      }
      যদি (unlikely (!alloc ())) ফেরত মিথ্যা;
    }

    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    unsigned পূর্ণসংখ্যা tombstone = (unsigned পূর্ণসংখ্যা) -1;
    unsigned পূর্ণসংখ্যা i = hash % মৌলিক;
    unsigned দৈর্ঘ্য = 0;
    unsigned step = 0;
    যখন (বস্তুসমূহ[i].is_used ())
    {
      যদি ((std::is_integral<K>::value || বস্তুসমূহ[i].hash == hash) &&
	  বস্তুসমূহ[i] == key)
      {
        যদি (!overwrite)
	  ফেরত মিথ্যা;
        অন্যথা
	  বিরতি;
      }
      যদি (!বস্তুসমূহ[i].is_real () && tombstone == (unsigned) -1)
        tombstone = i;
      i = (i + ++step) & mask;
      দৈর্ঘ্য++;
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

    যদি (unlikely (দৈর্ঘ্য > max_chain_দৈর্ঘ্য) && occupancy > mask / 8)
      alloc (mask - 8); // This ensures we jump to next larger size

    ফেরত সত্য;
  }

  template <typename VV>
  যৌক্তিক set (ধ্রুবক K &key, VV&& value, যৌক্তিক overwrite = সত্য) { ফেরত set_with_hash (key, hb_hash (key), std::forward<VV> (value), overwrite); }
  template <typename VV>
  যৌক্তিক set (K &&key, VV&& value, যৌক্তিক overwrite = সত্য)
  {
    uint32_t hash = hb_hash (key);
    ফেরত set_with_hash (std::move (key), hash, std::forward<VV> (value), overwrite);
  }
  যৌক্তিক add (ধ্রুবক K &key)
  {
    uint32_t hash = hb_hash (key);
    ফেরত set_with_hash (key, hash, item_t::default_মান ());
  }

  ধ্রুবক V& get_with_hash (ধ্রুবক K &key, uint32_t hash) ধ্রুবক
  {
    যদি (!বস্তুসমূহ) ফেরত item_t::default_মান ();
    auto *item = fetch_item (key, hash);
    যদি (item)
      ফেরত item->value;
    ফেরত item_t::default_মান ();
  }
  ধ্রুবক V& get (ধ্রুবক K &key) ধ্রুবক
  {
    যদি (!বস্তুসমূহ) ফেরত item_t::default_মান ();
    ফেরত get_with_hash (key, hb_hash (key));
  }

  শূন্য del (ধ্রুবক K &key)
  {
    যদি (!বস্তুসমূহ) ফেরত;
    auto *item = fetch_item (key, hb_hash (key));
    যদি (item)
    {
      item->set_real (মিথ্যা);
      population--;
    }
  }

  /* Has interface. */
  ধ্রুবক V& operator [] (K k) ধ্রুবক { ফেরত get (k); }
  template <typename VV=V>
  যৌক্তিক has (ধ্রুবক K &key, VV **vp = nullptr) ধ্রুবক
  {
    যদি (!বস্তুসমূহ) ফেরত মিথ্যা;
    ফেরত has_with_hash (key, hb_hash (key), vp);
  }
  template <typename VV=V>
  যৌক্তিক has_with_hash (ধ্রুবক K &key, uint32_t hash, VV **vp = nullptr) ধ্রুবক
  {
    যদি (!বস্তুসমূহ) ফেরত মিথ্যা;
    auto *item = fetch_item (key, hash);
    যদি (item)
    {
      যদি (vp) *vp = std::addressof (item->value);
      ফেরত সত্য;
    }
    ফেরত মিথ্যা;
  }
  item_t *fetch_item (ধ্রুবক K &key, uint32_t hash) ধ্রুবক
  {
    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    unsigned পূর্ণসংখ্যা i = hash % মৌলিক;
    unsigned step = 0;
    যখন (বস্তুসমূহ[i].is_used ())
    {
      যদি ((std::is_integral<K>::value || বস্তুসমূহ[i].hash == hash) &&
	  বস্তুসমূহ[i] == key)
      {
	যদি (বস্তুসমূহ[i].is_real ())
	  ফেরত &বস্তুসমূহ[i];
	অন্যথা
	  ফেরত nullptr;
      }
      i = (i + ++step) & mask;
    }
    ফেরত nullptr;
  }
  /* Projection. */
  ধ্রুবক V& operator () (K k) ধ্রুবক { ফেরত get (k); }

  unsigned আকার_2 () ধ্রুবক { ফেরত mask ? mask + 1 : 0; }

  শূন্য clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (zero-initialized) from any writes.  Any non-empty hashmap
     * is a real heap instance with writable items, so clearing
     * under !successful is safe. */
    যদি (!population && !occupancy) ফেরত;

    জন্য (auto &_ : hb_iter (বস্তুসমূহ, আকার_2 ()))
    {
      /* Reconstruct items. */
      _.~item_t ();
      নতুন (&_) item_t ();
    }

    population = occupancy = 0;
  }

  যৌক্তিক is_empty () ধ্রুবক { ফেরত population == 0; }
  explicit operator যৌক্তিক () ধ্রুবক { ফেরত !is_empty (); }

  uint32_t hash () ধ্রুবক
  {
    ফেরত
    + iter_বস্তুসমূহ ()
    | hb_reduce ([] (uint32_t h, ধ্রুবক item_t &_) { ফেরত h ^ _.যোগ_hash (); }, (uint32_t) 0u)
    ;
  }

  যৌক্তিক is_equal (ধ্রুবক hb_hashmap_t &other) ধ্রুবক
  {
    যদি (population != other.population) ফেরত মিথ্যা;

    জন্য (auto pair : iter ())
      যদি (other.get (pair.first) != pair.second)
        ফেরত মিথ্যা;

    ফেরত সত্য;
  }
  যৌক্তিক operator == (ধ্রুবক hb_hashmap_t &other) ধ্রুবক { ফেরত is_equal (other); }
  যৌক্তিক operator != (ধ্রুবক hb_hashmap_t &other) ধ্রুবক { ফেরত !is_equal (other); }

  unsigned পূর্ণসংখ্যা get_population () ধ্রুবক { ফেরত population; }

  শূন্য update (ধ্রুবক hb_hashmap_t &other)
  {
    যদি (unlikely (!successful)) ফেরত;

    hb_copy (other, *এই);
  }

  /*
   * Iterator
   */

  auto iter_বস্তুসমূহ () ধ্রুবক HB_AUTO_RETURN
  (
    + hb_iter (বস্তুসমূহ, এই->আকার_2 ())
    | hb_filter (&item_t::is_real)
  )
  auto iter_ref () ধ্রুবক HB_AUTO_RETURN
  (
    + এই->iter_বস্তুসমূহ ()
    | hb_map (&item_t::get_pair_ref)
  )
  auto iter () ধ্রুবক HB_AUTO_RETURN
  (
    + এই->iter_বস্তুসমূহ ()
    | hb_map (&item_t::get_pair)
  )
  auto চাবিগুলি_ref () ধ্রুবক HB_AUTO_RETURN
  (
    + এই->iter_বস্তুসমূহ ()
    | hb_map (&item_t::get_চাবি)
  )
  auto চাবিগুলি () ধ্রুবক HB_AUTO_RETURN
  (
    + এই->চাবিগুলি_ref ()
    | hb_map (hb_ridentity)
  )
  auto মানসমূহ_ref () ধ্রুবক HB_AUTO_RETURN
  (
    + এই->iter_বস্তুসমূহ ()
    | hb_map (&item_t::get_মান)
  )
  auto values () ধ্রুবক HB_AUTO_RETURN
  (
    + এই->মানসমূহ_ref ()
    | hb_map (hb_ridentity)
  )

  /* C iterator. */
  যৌক্তিক next (পূর্ণসংখ্যা *idx,
	     K *key,
	     V *value) ধ্রুবক
  {
    unsigned i = (unsigned) (*idx + 1);

    unsigned গুনতি = আকার_2 ();
    যখন (i < গুনতি && !বস্তুসমূহ[i].is_real ())
      i++;

    যদি (i >= গুনতি)
    {
      *idx = -1;
      ফেরত মিথ্যা;
    }

    *key = বস্তুসমূহ[i].key;
    *value = বস্তুসমূহ[i].value;

    *idx = (signed) i;
    ফেরত সত্য;
  }

  /* Sink interface. */
  hb_hashmap_t& operator << (ধ্রুবক hb_pair_t<K, V>& v)
  { set (v.first, v.second); ফেরত *এই; }
  template <typename V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (V2))>
  hb_hashmap_t& operator << (ধ্রুবক hb_pair_t<K, V&&>& v)
  { set (v.first, std::move (v.second)); ফেরত *এই; }
  template <typename K2 = K,
	    hb_enable_if (!hb_is_trivially_copyable (K2))>
  hb_hashmap_t& operator << (ধ্রুবক hb_pair_t<K&&, V>& v)
  { set (std::move (v.first), v.second); ফেরত *এই; }
  template <typename K2 = K, typename V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (K2) &&
			  !hb_is_trivially_copyable (V2))>
  hb_hashmap_t& operator << (ধ্রুবক hb_pair_t<K&&, V&&>& v)
  { set (std::move (v.first), std::move (v.second)); ফেরত *এই; }

  স্থির unsigned পূর্ণসংখ্যা মৌলিক_for (unsigned পূর্ণসংখ্যা shift)
  {
    /* Following comment and table copied from glib. */
    /* Each table size has an associated prime modulo (the first prime
     * lower than the table size) used to find the initial bucket. Probing
     * then works modulo 2^n. The prime modulo is necessary to get a
     * good distribution with poor hash functions.
     */
    /* Not declaring static to make all kinds of compilers happy... */
    /*static*/ ধ্রুবক unsigned পূর্ণসংখ্যা মৌলিক_mod [32] =
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

    যদি (unlikely (shift >= ARRAY_দৈর্ঘ্য (মৌলিক_mod)))
      ফেরত মৌলিক_mod[ARRAY_দৈর্ঘ্য (মৌলিক_mod) - 1];

    ফেরত মৌলিক_mod[shift];
  }
};

/*
 * hb_map_t
 */

কাঠামো hb_map_t : hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       সত্য>
{
  using hashmap = hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       সত্য>;

  ~hb_map_t () = পূর্বনির্ধারিত;
  hb_map_t () : hashmap () {}
  hb_map_t (ধ্রুবক hb_map_t &o) : hashmap ((hashmap &) o) {}
  hb_map_t (hb_map_t &&o)  noexcept : hashmap (std::move ((hashmap &) o)) {}
  hb_map_t& operator= (ধ্রুবক hb_map_t&) = পূর্বনির্ধারিত;
  hb_map_t& operator= (hb_map_t&&) = পূর্বনির্ধারিত;
  hb_map_t (std::initializer_list<hb_codepoint_pair_t> lst) : hashmap (lst) {}
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_map_t (ধ্রুবক Iterable &o) : hashmap (o) {}
};


#endif /* HB_MAP_HH */

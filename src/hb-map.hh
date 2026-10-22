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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ଅନ୍ତର୍ଭାଵ "hb-set.hh"


/*
 * hb_hashmap_t
 */

ବାହ୍ୟ HB_INTERNAL ଅଚଳ hb_codepoint_t minus_1;

template <typename K, typename V,
	  ତାର୍କିକ minus_one = ମିଥ୍ୟା>
ସଂରଚନା hb_hashmap_t
{
  ସ୍ଥିର constexpr ତାର୍କିକ realloc_move = ସତ୍ୟ;

  hb_hashmap_t ()  { init (); }
  ~hb_hashmap_t () { fini (); }

  ଶୂନ୍ୟ _copy (ଅଚଳ hb_hashmap_t& o)
  {
    ଯଦି (unlikely (!o.successful))
    {
      successful = ମିଥ୍ୟା;
      ଫେରନ୍ତୁ;
    }
    ଯଦି (unlikely (!o.mask)) ଫେରନ୍ତୁ;

    ଯଦି (hb_is_trivially_copy_assignable (item_t))
    {
      ବସ୍ତୁଗୁଡ଼ିକ = (item_t *) hb_malloc2 ((o.mask + 1), ଆକାର (item_t));
      ଯଦି (unlikely (!ବସ୍ତୁଗୁଡ଼ିକ))
      {
	successful = ମିଥ୍ୟା;
	ଫେରନ୍ତୁ;
      }
      population = o.population;
      occupancy = o.occupancy;
      mask = o.mask;
      ମୌଳିକ = o.ମୌଳିକ;
      max_chain_length = o.max_chain_length;
      memcpy (ବସ୍ତୁଗୁଡ଼ିକ, o.ବସ୍ତୁଗୁଡ଼ିକ, ଆକାର (item_t) * (mask + 1));
      ଫେରନ୍ତୁ;
    }

    alloc (o.population); hb_copy (o, *ଏହି);
  }

  hb_hashmap_t (ଅଚଳ hb_hashmap_t& o) : hb_hashmap_t () { _copy (o); }
  hb_hashmap_t& operator= (ଅଚଳ hb_hashmap_t& o)
  {
    reset ();
    ଯଦି (!ବସ୍ତୁଗୁଡ଼ିକ) { _copy (o); ଫେରନ୍ତୁ *ଏହି; }
    alloc (o.population); hb_copy (o, *ଏହି); ଫେରନ୍ତୁ *ଏହି;
  }

  hb_hashmap_t (hb_hashmap_t&& o)  noexcept : hb_hashmap_t () { hb_swap (*ଏହି, o); }
  hb_hashmap_t& operator= (hb_hashmap_t&& o)   noexcept { hb_swap (*ଏହି, o); ଫେରନ୍ତୁ *ଏହି; }

  hb_hashmap_t (std::initializer_list<hb_pair_t<K, V>> lst) : hb_hashmap_t ()
  {
    ପାଇଁ (auto&& item : lst)
      set (item.first, item.second);
  }
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_hashmap_t (ଅଚଳ Iterable &o) : hb_hashmap_t ()
  {
    auto iter = hb_iter (o);
    ଯଦି (iter.is_random_access_iterator || iter.has_fast_len)
      alloc (hb_len (iter));
    hb_copy (iter, *ଏହି);
  }

  ସଂରଚନା item_t
  {
    K key;
    uint32_t is_real_ : 1;
    uint32_t is_used_ : 1;
    uint32_t hash : 30;
    V value;

    item_t () : key (),
		is_real_ (ମିଥ୍ୟା), is_used_ (ମିଥ୍ୟା),
		hash (0),
		value () {}

    // Needed for https://github.com/harfbuzz/harfbuzz/issues/4138
    K& get_ଚାବି () { ଫେରନ୍ତୁ key; }
    V& get_ମୂଲ୍ୟ () { ଫେରନ୍ତୁ value; }

    ତାର୍କିକ is_used () ଅଚଳ { ଫେରନ୍ତୁ is_used_; }
    ଶୂନ୍ୟ set_used (ତାର୍କିକ is_used) { is_used_ = is_used; }
    ଶୂନ୍ୟ set_real (ତାର୍କିକ is_real) { is_real_ = is_real; }
    ତାର୍କିକ is_real () ଅଚଳ { ଫେରନ୍ତୁ is_real_; }

    template <ତାର୍କିକ v = minus_one,
	      hb_enable_if (v == ମିଥ୍ୟା)>
    ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅଚଳ V& default_ମୂଲ୍ୟ () { ଫେରନ୍ତୁ Null(V); };
    template <ତାର୍କିକ v = minus_one,
	      hb_enable_if (v == ସତ୍ୟ)>
    ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅଚଳ V& default_ମୂଲ୍ୟ ()
    {
      static_assert (hb_is_same (V, hb_codepoint_t), "");
      ଫେରନ୍ତୁ minus_1;
    };

    ତାର୍କିକ operator == (ଅଚଳ K &o) ଅଚଳ { ଫେରନ୍ତୁ hb_deref (key) == hb_deref (o); }
    ତାର୍କିକ operator == (ଅଚଳ item_t &o) ଅଚଳ { ଫେରନ୍ତୁ *ଏହି == o.key; }
    hb_pair_t<K, V> get_pair() ଅଚଳ { ଫେରନ୍ତୁ hb_pair_t<K, V> (key, value); }
    hb_pair_t<ଅଚଳ K &, V &> get_pair_ref() { ଫେରନ୍ତୁ hb_pair_t<ଅଚଳ K &, V &> (key, value); }

    uint32_t total_hash () ଅଚଳ
    { ଫେରନ୍ତୁ (hash * 31u) + hb_hash (value); }

    ସ୍ଥିର constexpr ତାର୍କିକ is_trivially_constructible = (hb_is_trivially_constructible(K) && hb_is_trivially_constructible(V));
  };

  hb_object_header_t header;
  ତାର୍କିକ successful; /* Allocations successful */
  unsigned short max_chain_length;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା population; /* Not including tombstones. */
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା occupancy; /* Including tombstones. */
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mask;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୌଳିକ;
  item_t *ବସ୍ତୁଗୁଡ଼ିକ;

  friend ଶୂନ୍ୟ swap (hb_hashmap_t& a, hb_hashmap_t& b) noexcept
  {
    ଯଦି (unlikely (!a.successful || !b.successful))
      ଫେରନ୍ତୁ;
    hb_swap (a.max_chain_length, b.max_chain_length);
    hb_swap (a.population, b.population);
    hb_swap (a.occupancy, b.occupancy);
    hb_swap (a.mask, b.mask);
    hb_swap (a.ମୌଳିକ, b.ମୌଳିକ);
    hb_swap (a.ବସ୍ତୁଗୁଡ଼ିକ, b.ବସ୍ତୁଗୁଡ଼ିକ);
  }
  ଶୂନ୍ୟ init ()
  {
    hb_object_init (ଏହି);

    successful = ସତ୍ୟ;
    max_chain_length = 0;
    population = occupancy = 0;
    mask = 0;
    ମୌଳିକ = 0;
    ବସ୍ତୁଗୁଡ଼ିକ = nullptr;
  }
  ଶୂନ୍ୟ fini ()
  {
    hb_object_fini (ଏହି);

    ଯଦି (likely (ବସ୍ତୁଗୁଡ଼ିକ))
    {
      unsigned size = mask + 1;
      ପାଇଁ (unsigned i = 0; i < size; i++)
	ବସ୍ତୁଗୁଡ଼ିକ[i].~item_t ();
      hb_free (ବସ୍ତୁଗୁଡ଼ିକ);
      ବସ୍ତୁଗୁଡ଼ିକ = nullptr;
    }
    population = occupancy = 0;
  }

  hb_hashmap_t& reset ()
  {
    successful = ସତ୍ୟ;
    clear ();
    ଫେରନ୍ତୁ *ଏହି;
  }

  ତାର୍କିକ in_error () ଅଚଳ { ଫେରନ୍ତୁ !successful; }

  ତାର୍କିକ alloc (unsigned new_population = 0)
  {
    ଯଦି (unlikely (!successful)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଯଦି (unlikely (population > 0x3FFFFFFFu || new_population > 0x3FFFFFFFu))
    {
      // Population sizes >0x3FFFFFFF will result in power
      // being larger than 31, which in turn leads to a new size
      // that overflows u32.
      successful = ମିଥ୍ୟା;
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    ଯଦି (new_population != 0 && (new_population + new_population / 2) < mask) ଫେରନ୍ତୁ ସତ୍ୟ;

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା power = hb_bit_storage (hb_max (hb_max ((unsigned) population, new_population) * 2, 4u));
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_size = 1u << power;
    item_t *new_ବସ୍ତୁଗୁଡ଼ିକ = (item_t *) hb_malloc2 ((size_t) new_size, ଆକାର (item_t));
    ଯଦି (unlikely (!new_ବସ୍ତୁଗୁଡ଼ିକ))
    {
      successful = ମିଥ୍ୟା;
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    ଯଦି (!item_t::is_trivially_constructible)
      ପାଇଁ (auto &_ : hb_iter (new_ବସ୍ତୁଗୁଡ଼ିକ, new_size))
	ନୂତନ (&_) item_t ();
    ଅନ୍ୟଥା
      hb_memset (new_ବସ୍ତୁଗୁଡ଼ିକ, 0, (size_t) new_size * ଆକାର (item_t));

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା old_size = size ();
    item_t *old_ବସ୍ତୁଗୁଡ଼ିକ = ବସ୍ତୁଗୁଡ଼ିକ;

    /* Switch to new, empty, array. */
    population = occupancy = 0;
    mask = new_size - 1;
    ମୌଳିକ = ମୌଳିକ_for (power);
    max_chain_length = power * 2;
    ବସ୍ତୁଗୁଡ଼ିକ = new_ବସ୍ତୁଗୁଡ଼ିକ;

    /* Insert back old items. */
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < old_size; i++)
    {
      ଯଦି (old_ବସ୍ତୁଗୁଡ଼ିକ[i].is_real ())
      {
	set_with_hash (std::move (old_ବସ୍ତୁଗୁଡ଼ିକ[i].key),
		       old_ବସ୍ତୁଗୁଡ଼ିକ[i].hash,
		       std::move (old_ବସ୍ତୁଗୁଡ଼ିକ[i].value));
      }
    }
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < old_size; i++)
      old_ବସ୍ତୁଗୁଡ଼ିକ[i].~item_t ();

    hb_free (old_ବସ୍ତୁଗୁଡ଼ିକ);

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  template <typename T>
  ତାର୍କିକ in_storage (ଅଚଳ T &v) ଅଚଳ
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    ଫେରନ୍ତୁ ବସ୍ତୁଗୁଡ଼ିକ &&
	   (uintptr_t) ବସ୍ତୁଗୁଡ଼ିକ <= p && p < (uintptr_t) (ବସ୍ତୁଗୁଡ଼ିକ + size ());
  }

  template <typename KK, typename VV>
  ତାର୍କିକ set_with_hash (KK&& key, uint32_t hash, VV&& value, ତାର୍କିକ overwrite = ସତ୍ୟ)
  {
    ଯଦି (unlikely (!successful)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଯଦି (unlikely ((occupancy + occupancy / 2) >= mask))
    {
      /* key or value may live in our own storage, eg. m.set (k, m.get (j)).
       * alloc() frees that storage, so copy them out first. */
      ଯଦି (unlikely (in_storage (key) || in_storage (value)))
      {
	K ଚାବି_copy;
	ଚାବି_copy = std::forward<KK> (key);
	V ମୂଲ୍ୟ_copy;
	ମୂଲ୍ୟ_copy = std::forward<VV> (value);
	ଫେରନ୍ତୁ set_with_hash (std::move (ଚାବି_copy), hash, std::move (ମୂଲ୍ୟ_copy), overwrite);
      }
      ଯଦି (unlikely (!alloc ())) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା tombstone = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) -1;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = hash % ମୌଳିକ;
    unsigned length = 0;
    unsigned step = 0;
    ଯେତେବେଳେ (ବସ୍ତୁଗୁଡ଼ିକ[i].is_used ())
    {
      ଯଦି ((std::is_integral<K>::value || ବସ୍ତୁଗୁଡ଼ିକ[i].hash == hash) &&
	  ବସ୍ତୁଗୁଡ଼ିକ[i] == key)
      {
        ଯଦି (!overwrite)
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;
        ଅନ୍ୟଥା
	  ଭାଙ୍ଗ;
      }
      ଯଦି (!ବସ୍ତୁଗୁଡ଼ିକ[i].is_real () && tombstone == (unsigned) -1)
        tombstone = i;
      i = (i + ++step) & mask;
      length++;
    }

    item_t &item = ବସ୍ତୁଗୁଡ଼ିକ[tombstone == (unsigned) -1 ? i : tombstone];

    ଯଦି (item.is_used ())
    {
      occupancy--;
      population -= item.is_real ();
    }

    item.key = std::forward<KK> (key);
    item.value = std::forward<VV> (value);
    item.hash = hash;
    item.set_used (ସତ୍ୟ);
    item.set_real (ସତ୍ୟ);

    occupancy++;
    population++;

    ଯଦି (unlikely (length > max_chain_length) && occupancy > mask / 8)
      alloc (mask - 8); // This ensures we jump to next larger size

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  template <typename VV>
  ତାର୍କିକ set (ଅଚଳ K &key, VV&& value, ତାର୍କିକ overwrite = ସତ୍ୟ) { ଫେରନ୍ତୁ set_with_hash (key, hb_hash (key), std::forward<VV> (value), overwrite); }
  template <typename VV>
  ତାର୍କିକ set (K &&key, VV&& value, ତାର୍କିକ overwrite = ସତ୍ୟ)
  {
    uint32_t hash = hb_hash (key);
    ଫେରନ୍ତୁ set_with_hash (std::move (key), hash, std::forward<VV> (value), overwrite);
  }
  ତାର୍କିକ add (ଅଚଳ K &key)
  {
    uint32_t hash = hb_hash (key);
    ଫେରନ୍ତୁ set_with_hash (key, hash, item_t::default_ମୂଲ୍ୟ ());
  }

  ଅଚଳ V& get_with_hash (ଅଚଳ K &key, uint32_t hash) ଅଚଳ
  {
    ଯଦି (!ବସ୍ତୁଗୁଡ଼ିକ) ଫେରନ୍ତୁ item_t::default_ମୂଲ୍ୟ ();
    auto *item = fetch_item (key, hash);
    ଯଦି (item)
      ଫେରନ୍ତୁ item->value;
    ଫେରନ୍ତୁ item_t::default_ମୂଲ୍ୟ ();
  }
  ଅଚଳ V& get (ଅଚଳ K &key) ଅଚଳ
  {
    ଯଦି (!ବସ୍ତୁଗୁଡ଼ିକ) ଫେରନ୍ତୁ item_t::default_ମୂଲ୍ୟ ();
    ଫେରନ୍ତୁ get_with_hash (key, hb_hash (key));
  }

  ଶୂନ୍ୟ del (ଅଚଳ K &key)
  {
    ଯଦି (!ବସ୍ତୁଗୁଡ଼ିକ) ଫେରନ୍ତୁ;
    auto *item = fetch_item (key, hb_hash (key));
    ଯଦି (item)
    {
      item->set_real (ମିଥ୍ୟା);
      population--;
    }
  }

  /* Has interface. */
  ଅଚଳ V& operator [] (K k) ଅଚଳ { ଫେରନ୍ତୁ get (k); }
  template <typename VV=V>
  ତାର୍କିକ has (ଅଚଳ K &key, VV **vp = nullptr) ଅଚଳ
  {
    ଯଦି (!ବସ୍ତୁଗୁଡ଼ିକ) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ has_with_hash (key, hb_hash (key), vp);
  }
  template <typename VV=V>
  ତାର୍କିକ has_with_hash (ଅଚଳ K &key, uint32_t hash, VV **vp = nullptr) ଅଚଳ
  {
    ଯଦି (!ବସ୍ତୁଗୁଡ଼ିକ) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    auto *item = fetch_item (key, hash);
    ଯଦି (item)
    {
      ଯଦି (vp) *vp = std::addressof (item->value);
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }
  item_t *fetch_item (ଅଚଳ K &key, uint32_t hash) ଅଚଳ
  {
    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = hash % ମୌଳିକ;
    unsigned step = 0;
    ଯେତେବେଳେ (ବସ୍ତୁଗୁଡ଼ିକ[i].is_used ())
    {
      ଯଦି ((std::is_integral<K>::value || ବସ୍ତୁଗୁଡ଼ିକ[i].hash == hash) &&
	  ବସ୍ତୁଗୁଡ଼ିକ[i] == key)
      {
	ଯଦି (ବସ୍ତୁଗୁଡ଼ିକ[i].is_real ())
	  ଫେରନ୍ତୁ &ବସ୍ତୁଗୁଡ଼ିକ[i];
	ଅନ୍ୟଥା
	  ଫେରନ୍ତୁ nullptr;
      }
      i = (i + ++step) & mask;
    }
    ଫେରନ୍ତୁ nullptr;
  }
  /* Projection. */
  ଅଚଳ V& operator () (K k) ଅଚଳ { ଫେରନ୍ତୁ get (k); }

  unsigned size () ଅଚଳ { ଫେରନ୍ତୁ mask ? mask + 1 : 0; }

  ଶୂନ୍ୟ clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (zero-initialized) from any writes.  Any non-empty hashmap
     * is a real heap instance with writable items, so clearing
     * under !successful is safe. */
    ଯଦି (!population && !occupancy) ଫେରନ୍ତୁ;

    ପାଇଁ (auto &_ : hb_iter (ବସ୍ତୁଗୁଡ଼ିକ, size ()))
    {
      /* Reconstruct items. */
      _.~item_t ();
      ନୂତନ (&_) item_t ();
    }

    population = occupancy = 0;
  }

  ତାର୍କିକ is_empty () ଅଚଳ { ଫେରନ୍ତୁ population == 0; }
  explicit operator ତାର୍କିକ () ଅଚଳ { ଫେରନ୍ତୁ !is_empty (); }

  uint32_t hash () ଅଚଳ
  {
    ଫେରନ୍ତୁ
    + iter_ବସ୍ତୁଗୁଡ଼ିକ ()
    | hb_reduce ([] (uint32_t h, ଅଚଳ item_t &_) { ଫେରନ୍ତୁ h ^ _.total_hash (); }, (uint32_t) 0u)
    ;
  }

  ତାର୍କିକ is_equal (ଅଚଳ hb_hashmap_t &other) ଅଚଳ
  {
    ଯଦି (population != other.population) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ପାଇଁ (auto pair : iter ())
      ଯଦି (other.get (pair.first) != pair.second)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }
  ତାର୍କିକ operator == (ଅଚଳ hb_hashmap_t &other) ଅଚଳ { ଫେରନ୍ତୁ is_equal (other); }
  ତାର୍କିକ operator != (ଅଚଳ hb_hashmap_t &other) ଅଚଳ { ଫେରନ୍ତୁ !is_equal (other); }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_population () ଅଚଳ { ଫେରନ୍ତୁ population; }

  ଶୂନ୍ୟ update (ଅଚଳ hb_hashmap_t &other)
  {
    ଯଦି (unlikely (!successful)) ଫେରନ୍ତୁ;

    hb_copy (other, *ଏହି);
  }

  /*
   * Iterator
   */

  auto iter_ବସ୍ତୁଗୁଡ଼ିକ () ଅଚଳ HB_AUTO_RETURN
  (
    + hb_iter (ବସ୍ତୁଗୁଡ଼ିକ, ଏହି->size ())
    | hb_filter (&item_t::is_real)
  )
  auto iter_ref () ଅଚଳ HB_AUTO_RETURN
  (
    + ଏହି->iter_ବସ୍ତୁଗୁଡ଼ିକ ()
    | hb_map (&item_t::get_pair_ref)
  )
  auto iter () ଅଚଳ HB_AUTO_RETURN
  (
    + ଏହି->iter_ବସ୍ତୁଗୁଡ଼ିକ ()
    | hb_map (&item_t::get_pair)
  )
  auto ଚାବିଗୁଡ଼ିକ_ref () ଅଚଳ HB_AUTO_RETURN
  (
    + ଏହି->iter_ବସ୍ତୁଗୁଡ଼ିକ ()
    | hb_map (&item_t::get_ଚାବି)
  )
  auto ଚାବିଗୁଡ଼ିକ () ଅଚଳ HB_AUTO_RETURN
  (
    + ଏହି->ଚାବିଗୁଡ଼ିକ_ref ()
    | hb_map (hb_ridentity)
  )
  auto ମୂଲ୍ୟଗୁଡ଼ିକ_ref () ଅଚଳ HB_AUTO_RETURN
  (
    + ଏହି->iter_ବସ୍ତୁଗୁଡ଼ିକ ()
    | hb_map (&item_t::get_ମୂଲ୍ୟ)
  )
  auto values () ଅଚଳ HB_AUTO_RETURN
  (
    + ଏହି->ମୂଲ୍ୟଗୁଡ଼ିକ_ref ()
    | hb_map (hb_ridentity)
  )

  /* C iterator. */
  ତାର୍କିକ next (ପୂର୍ଣ୍ଣସଂଖ୍ୟା *idx,
	     K *key,
	     V *value) ଅଚଳ
  {
    unsigned i = (unsigned) (*idx + 1);

    unsigned ଗଣନା_2 = size ();
    ଯେତେବେଳେ (i < ଗଣନା_2 && !ବସ୍ତୁଗୁଡ଼ିକ[i].is_real ())
      i++;

    ଯଦି (i >= ଗଣନା_2)
    {
      *idx = -1;
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    *key = ବସ୍ତୁଗୁଡ଼ିକ[i].key;
    *value = ବସ୍ତୁଗୁଡ଼ିକ[i].value;

    *idx = (signed) i;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  /* Sink interface. */
  hb_hashmap_t& operator << (ଅଚଳ hb_pair_t<K, V>& v)
  { set (v.first, v.second); ଫେରନ୍ତୁ *ଏହି; }
  template <typename V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (V2))>
  hb_hashmap_t& operator << (ଅଚଳ hb_pair_t<K, V&&>& v)
  { set (v.first, std::move (v.second)); ଫେରନ୍ତୁ *ଏହି; }
  template <typename K2 = K,
	    hb_enable_if (!hb_is_trivially_copyable (K2))>
  hb_hashmap_t& operator << (ଅଚଳ hb_pair_t<K&&, V>& v)
  { set (std::move (v.first), v.second); ଫେରନ୍ତୁ *ଏହି; }
  template <typename K2 = K, typename V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (K2) &&
			  !hb_is_trivially_copyable (V2))>
  hb_hashmap_t& operator << (ଅଚଳ hb_pair_t<K&&, V&&>& v)
  { set (std::move (v.first), std::move (v.second)); ଫେରନ୍ତୁ *ଏହି; }

  ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୌଳିକ_for (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା shift)
  {
    /* Following comment and table copied from glib. */
    /* Each table size has an associated prime modulo (the first prime
     * lower than the table size) used to find the initial bucket. Probing
     * then works modulo 2^n. The prime modulo is necessary to get a
     * good distribution with poor hash functions.
     */
    /* Not declaring static to make all kinds of compilers happy... */
    /*static*/ ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୌଳିକ_mod [32] =
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

    ଯଦି (unlikely (shift >= ARRAY_LENGTH (ମୌଳିକ_mod)))
      ଫେରନ୍ତୁ ମୌଳିକ_mod[ARRAY_LENGTH (ମୌଳିକ_mod) - 1];

    ଫେରନ୍ତୁ ମୌଳିକ_mod[shift];
  }
};

/*
 * hb_map_t
 */

ସଂରଚନା hb_map_t : hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       ସତ୍ୟ>
{
  using hashmap = hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       ସତ୍ୟ>;

  ~hb_map_t () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  hb_map_t () : hashmap () {}
  hb_map_t (ଅଚଳ hb_map_t &o) : hashmap ((hashmap &) o) {}
  hb_map_t (hb_map_t &&o)  noexcept : hashmap (std::move ((hashmap &) o)) {}
  hb_map_t& operator= (ଅଚଳ hb_map_t&) = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  hb_map_t& operator= (hb_map_t&&) = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  hb_map_t (std::initializer_list<hb_codepoint_pair_t> lst) : hashmap (lst) {}
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_map_t (ଅଚଳ Iterable &o) : hashmap (o) {}
};


#endif /* HB_MAP_HH */

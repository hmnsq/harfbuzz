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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ಅನ್ತರ್ಭಾವ "hb-set.hh"


/*
 * hb_hashmap_t
 */

ಬಾಹ್ಯ HB_INTERNAL ಧ್ರುವ hb_codepoint_t minus_1;

template <typename K, typename V,
	  ತಾರ್ಕಿಕ minus_one = ಅಸತ್ಯ>
ಸಂರಚನಾ hb_hashmap_t
{
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ realloc_move = ಸತ್ಯ;

  hb_hashmap_t ()  { init (); }
  ~hb_hashmap_t () { fini (); }

  ಶೂನ್ಯ _copy (ಧ್ರುವ hb_hashmap_t& o)
  {
    ಯದಿ (unlikely (!o.successful))
    {
      successful = ಅಸತ್ಯ;
      ಪ್ರತ್ಯಾಗಮ;
    }
    ಯದಿ (unlikely (!o.mask)) ಪ್ರತ್ಯಾಗಮ;

    ಯದಿ (hb_is_trivially_copy_assignable (item_t))
    {
      ವಸ್ತುಗಳು = (item_t *) hb_malloc2 ((o.mask + 1), ಪರಿಮಾಣ (item_t));
      ಯದಿ (unlikely (!ವಸ್ತುಗಳು))
      {
	successful = ಅಸತ್ಯ;
	ಪ್ರತ್ಯಾಗಮ;
      }
      population = o.population;
      occupancy = o.occupancy;
      mask = o.mask;
      ಅವಿಭಾಜ್ಯ = o.ಅವಿಭಾಜ್ಯ;
      max_chain_length = o.max_chain_length;
      memcpy (ವಸ್ತುಗಳು, o.ವಸ್ತುಗಳು, ಪರಿಮಾಣ (item_t) * (mask + 1));
      ಪ್ರತ್ಯಾಗಮ;
    }

    alloc (o.population); hb_copy (o, *ಏತತ್);
  }

  hb_hashmap_t (ಧ್ರುವ hb_hashmap_t& o) : hb_hashmap_t () { _copy (o); }
  hb_hashmap_t& operator= (ಧ್ರುವ hb_hashmap_t& o)
  {
    reset ();
    ಯದಿ (!ವಸ್ತುಗಳು) { _copy (o); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
    alloc (o.population); hb_copy (o, *ಏತತ್); ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
  }

  hb_hashmap_t (hb_hashmap_t&& o)  noexcept : hb_hashmap_t () { hb_swap (*ಏತತ್, o); }
  hb_hashmap_t& operator= (hb_hashmap_t&& o)   noexcept { hb_swap (*ಏತತ್, o); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }

  hb_hashmap_t (std::initializer_list<hb_pair_t<K, V>> lst) : hb_hashmap_t ()
  {
    ಪ್ರತಿ (auto&& item : lst)
      set (item.first, item.second);
  }
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_hashmap_t (ಧ್ರುವ Iterable &o) : hb_hashmap_t ()
  {
    auto iter = hb_iter (o);
    ಯದಿ (iter.is_random_access_iterator || iter.has_fast_len)
      alloc (hb_len (iter));
    hb_copy (iter, *ಏತತ್);
  }

  ಸಂರಚನಾ item_t
  {
    K key;
    uint32_t is_real_ : 1;
    uint32_t is_used_ : 1;
    uint32_t hash : 30;
    V value;

    item_t () : key (),
		is_real_ (ಅಸತ್ಯ), is_used_ (ಅಸತ್ಯ),
		hash (0),
		value () {}

    // Needed for https://github.com/harfbuzz/harfbuzz/issues/4138
    K& get_ಕೀಲಿ () { ಪ್ರತ್ಯಾಗಮ key; }
    V& get_ಮೌಲ್ಯ () { ಪ್ರತ್ಯಾಗಮ value; }

    ತಾರ್ಕಿಕ is_used () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ is_used_; }
    ಶೂನ್ಯ set_used (ತಾರ್ಕಿಕ is_used) { is_used_ = is_used; }
    ಶೂನ್ಯ set_real (ತಾರ್ಕಿಕ is_real) { is_real_ = is_real; }
    ತಾರ್ಕಿಕ is_real () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ is_real_; }

    template <ತಾರ್ಕಿಕ v = minus_one,
	      hb_enable_if (v == ಅಸತ್ಯ)>
    ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ V& default_ಮೌಲ್ಯ () { ಪ್ರತ್ಯಾಗಮ Null(V); };
    template <ತಾರ್ಕಿಕ v = minus_one,
	      hb_enable_if (v == ಸತ್ಯ)>
    ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ V& default_ಮೌಲ್ಯ ()
    {
      static_assert (hb_is_same (V, hb_codepoint_t), "");
      ಪ್ರತ್ಯಾಗಮ minus_1;
    };

    ತಾರ್ಕಿಕ operator == (ಧ್ರುವ K &o) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ hb_deref (key) == hb_deref (o); }
    ತಾರ್ಕಿಕ operator == (ಧ್ರುವ item_t &o) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ *ಏತತ್ == o.key; }
    hb_pair_t<K, V> get_pair() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ hb_pair_t<K, V> (key, value); }
    hb_pair_t<ಧ್ರುವ K &, V &> get_pair_ref() { ಪ್ರತ್ಯಾಗಮ hb_pair_t<ಧ್ರುವ K &, V &> (key, value); }

    uint32_t total_hash () ಧ್ರುವ
    { ಪ್ರತ್ಯಾಗಮ (hash * 31u) + hb_hash (value); }

    ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ is_trivially_constructible = (hb_is_trivially_constructible(K) && hb_is_trivially_constructible(V));
  };

  hb_object_header_t header;
  ತಾರ್ಕಿಕ successful; /* Allocations successful */
  unsigned short max_chain_length;
  unsigned ಪೂರ್ಣಾಙ್ಕ population; /* Not including tombstones. */
  unsigned ಪೂರ್ಣಾಙ್ಕ occupancy; /* Including tombstones. */
  unsigned ಪೂರ್ಣಾಙ್ಕ mask;
  unsigned ಪೂರ್ಣಾಙ್ಕ ಅವಿಭಾಜ್ಯ;
  item_t *ವಸ್ತುಗಳು;

  friend ಶೂನ್ಯ swap (hb_hashmap_t& a, hb_hashmap_t& b) noexcept
  {
    ಯದಿ (unlikely (!a.successful || !b.successful))
      ಪ್ರತ್ಯಾಗಮ;
    hb_swap (a.max_chain_length, b.max_chain_length);
    hb_swap (a.population, b.population);
    hb_swap (a.occupancy, b.occupancy);
    hb_swap (a.mask, b.mask);
    hb_swap (a.ಅವಿಭಾಜ್ಯ, b.ಅವಿಭಾಜ್ಯ);
    hb_swap (a.ವಸ್ತುಗಳು, b.ವಸ್ತುಗಳು);
  }
  ಶೂನ್ಯ init ()
  {
    hb_object_init (ಏತತ್);

    successful = ಸತ್ಯ;
    max_chain_length = 0;
    population = occupancy = 0;
    mask = 0;
    ಅವಿಭಾಜ್ಯ = 0;
    ವಸ್ತುಗಳು = nullptr;
  }
  ಶೂನ್ಯ fini ()
  {
    hb_object_fini (ಏತತ್);

    ಯದಿ (likely (ವಸ್ತುಗಳು))
    {
      unsigned size = mask + 1;
      ಪ್ರತಿ (unsigned i = 0; i < size; i++)
	ವಸ್ತುಗಳು[i].~item_t ();
      hb_free (ವಸ್ತುಗಳು);
      ವಸ್ತುಗಳು = nullptr;
    }
    population = occupancy = 0;
  }

  hb_hashmap_t& reset ()
  {
    successful = ಸತ್ಯ;
    clear ();
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
  }

  ತಾರ್ಕಿಕ in_error () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ !successful; }

  ತಾರ್ಕಿಕ alloc (unsigned new_population = 0)
  {
    ಯದಿ (unlikely (!successful)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಯದಿ (unlikely (population > 0x3FFFFFFFu || new_population > 0x3FFFFFFFu))
    {
      // Population sizes >0x3FFFFFFF will result in power
      // being larger than 31, which in turn leads to a new size
      // that overflows u32.
      successful = ಅಸತ್ಯ;
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    ಯದಿ (new_population != 0 && (new_population + new_population / 2) < mask) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

    unsigned ಪೂರ್ಣಾಙ್ಕ power = hb_bit_storage (hb_max (hb_max ((unsigned) population, new_population) * 2, 4u));
    unsigned ಪೂರ್ಣಾಙ್ಕ new_size = 1u << power;
    item_t *new_ವಸ್ತುಗಳು = (item_t *) hb_malloc2 ((size_t) new_size, ಪರಿಮಾಣ (item_t));
    ಯದಿ (unlikely (!new_ವಸ್ತುಗಳು))
    {
      successful = ಅಸತ್ಯ;
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    ಯದಿ (!item_t::is_trivially_constructible)
      ಪ್ರತಿ (auto &_ : hb_iter (new_ವಸ್ತುಗಳು, new_size))
	ನೂತನ (&_) item_t ();
    ಅನ್ಯಥಾ
      hb_memset (new_ವಸ್ತುಗಳು, 0, (size_t) new_size * ಪರಿಮಾಣ (item_t));

    unsigned ಪೂರ್ಣಾಙ್ಕ old_size = size ();
    item_t *old_ವಸ್ತುಗಳು = ವಸ್ತುಗಳು;

    /* Switch to new, empty, array. */
    population = occupancy = 0;
    mask = new_size - 1;
    ಅವಿಭಾಜ್ಯ = ಅವಿಭಾಜ್ಯ_for (power);
    max_chain_length = power * 2;
    ವಸ್ತುಗಳು = new_ವಸ್ತುಗಳು;

    /* Insert back old items. */
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < old_size; i++)
    {
      ಯದಿ (old_ವಸ್ತುಗಳು[i].is_real ())
      {
	set_with_hash (std::move (old_ವಸ್ತುಗಳು[i].key),
		       old_ವಸ್ತುಗಳು[i].hash,
		       std::move (old_ವಸ್ತುಗಳು[i].value));
      }
    }
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < old_size; i++)
      old_ವಸ್ತುಗಳು[i].~item_t ();

    hb_free (old_ವಸ್ತುಗಳು);

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  template <typename T>
  ತಾರ್ಕಿಕ in_storage (ಧ್ರುವ T &v) ಧ್ರುವ
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    ಪ್ರತ್ಯಾಗಮ ವಸ್ತುಗಳು &&
	   (uintptr_t) ವಸ್ತುಗಳು <= p && p < (uintptr_t) (ವಸ್ತುಗಳು + size ());
  }

  template <typename KK, typename VV>
  ತಾರ್ಕಿಕ set_with_hash (KK&& key, uint32_t hash, VV&& value, ತಾರ್ಕಿಕ overwrite = ಸತ್ಯ)
  {
    ಯದಿ (unlikely (!successful)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಯದಿ (unlikely ((occupancy + occupancy / 2) >= mask))
    {
      /* key or value may live in our own storage, eg. m.set (k, m.get (j)).
       * alloc() frees that storage, so copy them out first. */
      ಯದಿ (unlikely (in_storage (key) || in_storage (value)))
      {
	K ಕೀಲಿ_copy;
	ಕೀಲಿ_copy = std::forward<KK> (key);
	V ಮೌಲ್ಯ_copy;
	ಮೌಲ್ಯ_copy = std::forward<VV> (value);
	ಪ್ರತ್ಯಾಗಮ set_with_hash (std::move (ಕೀಲಿ_copy), hash, std::move (ಮೌಲ್ಯ_copy), overwrite);
      }
      ಯದಿ (unlikely (!alloc ())) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    unsigned ಪೂರ್ಣಾಙ್ಕ tombstone = (unsigned ಪೂರ್ಣಾಙ್ಕ) -1;
    unsigned ಪೂರ್ಣಾಙ್ಕ i = hash % ಅವಿಭಾಜ್ಯ;
    unsigned length = 0;
    unsigned step = 0;
    ಯಾವತ್ (ವಸ್ತುಗಳು[i].is_used ())
    {
      ಯದಿ ((std::is_integral<K>::value || ವಸ್ತುಗಳು[i].hash == hash) &&
	  ವಸ್ತುಗಳು[i] == key)
      {
        ಯದಿ (!overwrite)
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        ಅನ್ಯಥಾ
	  ಭಙ್ಗ;
      }
      ಯದಿ (!ವಸ್ತುಗಳು[i].is_real () && tombstone == (unsigned) -1)
        tombstone = i;
      i = (i + ++step) & mask;
      length++;
    }

    item_t &item = ವಸ್ತುಗಳು[tombstone == (unsigned) -1 ? i : tombstone];

    ಯದಿ (item.is_used ())
    {
      occupancy--;
      population -= item.is_real ();
    }

    item.key = std::forward<KK> (key);
    item.value = std::forward<VV> (value);
    item.hash = hash;
    item.set_used (ಸತ್ಯ);
    item.set_real (ಸತ್ಯ);

    occupancy++;
    population++;

    ಯದಿ (unlikely (length > max_chain_length) && occupancy > mask / 8)
      alloc (mask - 8); // This ensures we jump to next larger size

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  template <typename VV>
  ತಾರ್ಕಿಕ set (ಧ್ರುವ K &key, VV&& value, ತಾರ್ಕಿಕ overwrite = ಸತ್ಯ) { ಪ್ರತ್ಯಾಗಮ set_with_hash (key, hb_hash (key), std::forward<VV> (value), overwrite); }
  template <typename VV>
  ತಾರ್ಕಿಕ set (K &&key, VV&& value, ತಾರ್ಕಿಕ overwrite = ಸತ್ಯ)
  {
    uint32_t hash = hb_hash (key);
    ಪ್ರತ್ಯಾಗಮ set_with_hash (std::move (key), hash, std::forward<VV> (value), overwrite);
  }
  ತಾರ್ಕಿಕ add (ಧ್ರುವ K &key)
  {
    uint32_t hash = hb_hash (key);
    ಪ್ರತ್ಯಾಗಮ set_with_hash (key, hash, item_t::default_ಮೌಲ್ಯ ());
  }

  ಧ್ರುವ V& get_with_hash (ಧ್ರುವ K &key, uint32_t hash) ಧ್ರುವ
  {
    ಯದಿ (!ವಸ್ತುಗಳು) ಪ್ರತ್ಯಾಗಮ item_t::default_ಮೌಲ್ಯ ();
    auto *item = fetch_item (key, hash);
    ಯದಿ (item)
      ಪ್ರತ್ಯಾಗಮ item->value;
    ಪ್ರತ್ಯಾಗಮ item_t::default_ಮೌಲ್ಯ ();
  }
  ಧ್ರುವ V& get (ಧ್ರುವ K &key) ಧ್ರುವ
  {
    ಯದಿ (!ವಸ್ತುಗಳು) ಪ್ರತ್ಯಾಗಮ item_t::default_ಮೌಲ್ಯ ();
    ಪ್ರತ್ಯಾಗಮ get_with_hash (key, hb_hash (key));
  }

  ಶೂನ್ಯ del (ಧ್ರುವ K &key)
  {
    ಯದಿ (!ವಸ್ತುಗಳು) ಪ್ರತ್ಯಾಗಮ;
    auto *item = fetch_item (key, hb_hash (key));
    ಯದಿ (item)
    {
      item->set_real (ಅಸತ್ಯ);
      population--;
    }
  }

  /* Has interface. */
  ಧ್ರುವ V& operator [] (K k) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get (k); }
  template <typename VV=V>
  ತಾರ್ಕಿಕ has (ಧ್ರುವ K &key, VV **vp = nullptr) ಧ್ರುವ
  {
    ಯದಿ (!ವಸ್ತುಗಳು) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ has_with_hash (key, hb_hash (key), vp);
  }
  template <typename VV=V>
  ತಾರ್ಕಿಕ has_with_hash (ಧ್ರುವ K &key, uint32_t hash, VV **vp = nullptr) ಧ್ರುವ
  {
    ಯದಿ (!ವಸ್ತುಗಳು) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    auto *item = fetch_item (key, hash);
    ಯದಿ (item)
    {
      ಯದಿ (vp) *vp = std::addressof (item->value);
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }
  item_t *fetch_item (ಧ್ರುವ K &key, uint32_t hash) ಧ್ರುವ
  {
    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    unsigned ಪೂರ್ಣಾಙ್ಕ i = hash % ಅವಿಭಾಜ್ಯ;
    unsigned step = 0;
    ಯಾವತ್ (ವಸ್ತುಗಳು[i].is_used ())
    {
      ಯದಿ ((std::is_integral<K>::value || ವಸ್ತುಗಳು[i].hash == hash) &&
	  ವಸ್ತುಗಳು[i] == key)
      {
	ಯದಿ (ವಸ್ತುಗಳು[i].is_real ())
	  ಪ್ರತ್ಯಾಗಮ &ವಸ್ತುಗಳು[i];
	ಅನ್ಯಥಾ
	  ಪ್ರತ್ಯಾಗಮ nullptr;
      }
      i = (i + ++step) & mask;
    }
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }
  /* Projection. */
  ಧ್ರುವ V& operator () (K k) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get (k); }

  unsigned size () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ mask ? mask + 1 : 0; }

  ಶೂನ್ಯ clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (zero-initialized) from any writes.  Any non-empty hashmap
     * is a real heap instance with writable items, so clearing
     * under !successful is safe. */
    ಯದಿ (!population && !occupancy) ಪ್ರತ್ಯಾಗಮ;

    ಪ್ರತಿ (auto &_ : hb_iter (ವಸ್ತುಗಳು, size ()))
    {
      /* Reconstruct items. */
      _.~item_t ();
      ನೂತನ (&_) item_t ();
    }

    population = occupancy = 0;
  }

  ತಾರ್ಕಿಕ is_empty () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ population == 0; }
  explicit operator ತಾರ್ಕಿಕ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ !is_empty (); }

  uint32_t hash () ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ
    + iter_ವಸ್ತುಗಳು ()
    | hb_reduce ([] (uint32_t h, ಧ್ರುವ item_t &_) { ಪ್ರತ್ಯಾಗಮ h ^ _.total_hash (); }, (uint32_t) 0u)
    ;
  }

  ತಾರ್ಕಿಕ is_equal (ಧ್ರುವ hb_hashmap_t &other) ಧ್ರುವ
  {
    ಯದಿ (population != other.population) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪ್ರತಿ (auto pair : iter ())
      ಯದಿ (other.get (pair.first) != pair.second)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }
  ತಾರ್ಕಿಕ operator == (ಧ್ರುವ hb_hashmap_t &other) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ is_equal (other); }
  ತಾರ್ಕಿಕ operator != (ಧ್ರುವ hb_hashmap_t &other) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ !is_equal (other); }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_population () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ population; }

  ಶೂನ್ಯ update (ಧ್ರುವ hb_hashmap_t &other)
  {
    ಯದಿ (unlikely (!successful)) ಪ್ರತ್ಯಾಗಮ;

    hb_copy (other, *ಏತತ್);
  }

  /*
   * Iterator
   */

  auto iter_ವಸ್ತುಗಳು () ಧ್ರುವ HB_AUTO_RETURN
  (
    + hb_iter (ವಸ್ತುಗಳು, ಏತತ್->size ())
    | hb_filter (&item_t::is_real)
  )
  auto iter_ref () ಧ್ರುವ HB_AUTO_RETURN
  (
    + ಏತತ್->iter_ವಸ್ತುಗಳು ()
    | hb_map (&item_t::get_pair_ref)
  )
  auto iter () ಧ್ರುವ HB_AUTO_RETURN
  (
    + ಏತತ್->iter_ವಸ್ತುಗಳು ()
    | hb_map (&item_t::get_pair)
  )
  auto ಕೀಲಿಗಳು_ref () ಧ್ರುವ HB_AUTO_RETURN
  (
    + ಏತತ್->iter_ವಸ್ತುಗಳು ()
    | hb_map (&item_t::get_ಕೀಲಿ)
  )
  auto ಕೀಲಿಗಳು () ಧ್ರುವ HB_AUTO_RETURN
  (
    + ಏತತ್->ಕೀಲಿಗಳು_ref ()
    | hb_map (hb_ridentity)
  )
  auto ಮೌಲ್ಯಗಳು_ref () ಧ್ರುವ HB_AUTO_RETURN
  (
    + ಏತತ್->iter_ವಸ್ತುಗಳು ()
    | hb_map (&item_t::get_ಮೌಲ್ಯ)
  )
  auto values () ಧ್ರುವ HB_AUTO_RETURN
  (
    + ಏತತ್->ಮೌಲ್ಯಗಳು_ref ()
    | hb_map (hb_ridentity)
  )

  /* C iterator. */
  ತಾರ್ಕಿಕ next (ಪೂರ್ಣಾಙ್ಕ *idx,
	     K *key,
	     V *value) ಧ್ರುವ
  {
    unsigned i = (unsigned) (*idx + 1);

    unsigned ಎಣಿಕೆ = size ();
    ಯಾವತ್ (i < ಎಣಿಕೆ && !ವಸ್ತುಗಳು[i].is_real ())
      i++;

    ಯದಿ (i >= ಎಣಿಕೆ)
    {
      *idx = -1;
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    *key = ವಸ್ತುಗಳು[i].key;
    *value = ವಸ್ತುಗಳು[i].value;

    *idx = (signed) i;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  /* Sink interface. */
  hb_hashmap_t& operator << (ಧ್ರುವ hb_pair_t<K, V>& v)
  { set (v.first, v.second); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
  template <typename V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (V2))>
  hb_hashmap_t& operator << (ಧ್ರುವ hb_pair_t<K, V&&>& v)
  { set (v.first, std::move (v.second)); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
  template <typename K2 = K,
	    hb_enable_if (!hb_is_trivially_copyable (K2))>
  hb_hashmap_t& operator << (ಧ್ರುವ hb_pair_t<K&&, V>& v)
  { set (std::move (v.first), v.second); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
  template <typename K2 = K, typename V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (K2) &&
			  !hb_is_trivially_copyable (V2))>
  hb_hashmap_t& operator << (ಧ್ರುವ hb_pair_t<K&&, V&&>& v)
  { set (std::move (v.first), std::move (v.second)); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }

  ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ ಅವಿಭಾಜ್ಯ_for (unsigned ಪೂರ್ಣಾಙ್ಕ shift)
  {
    /* Following comment and table copied from glib. */
    /* Each table size has an associated prime modulo (the first prime
     * lower than the table size) used to find the initial bucket. Probing
     * then works modulo 2^n. The prime modulo is necessary to get a
     * good distribution with poor hash functions.
     */
    /* Not declaring static to make all kinds of compilers happy... */
    /*static*/ ಧ್ರುವ unsigned ಪೂರ್ಣಾಙ್ಕ ಅವಿಭಾಜ್ಯ_mod [32] =
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

    ಯದಿ (unlikely (shift >= ARRAY_LENGTH (ಅವಿಭಾಜ್ಯ_mod)))
      ಪ್ರತ್ಯಾಗಮ ಅವಿಭಾಜ್ಯ_mod[ARRAY_LENGTH (ಅವಿಭಾಜ್ಯ_mod) - 1];

    ಪ್ರತ್ಯಾಗಮ ಅವಿಭಾಜ್ಯ_mod[shift];
  }
};

/*
 * hb_map_t
 */

ಸಂರಚನಾ hb_map_t : hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       ಸತ್ಯ>
{
  using hashmap = hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       ಸತ್ಯ>;

  ~hb_map_t () = ಸ್ವಭಾವ;
  hb_map_t () : hashmap () {}
  hb_map_t (ಧ್ರುವ hb_map_t &o) : hashmap ((hashmap &) o) {}
  hb_map_t (hb_map_t &&o)  noexcept : hashmap (std::move ((hashmap &) o)) {}
  hb_map_t& operator= (ಧ್ರುವ hb_map_t&) = ಸ್ವಭಾವ;
  hb_map_t& operator= (hb_map_t&&) = ಸ್ವಭಾವ;
  hb_map_t (std::initializer_list<hb_codepoint_pair_t> lst) : hashmap (lst) {}
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_map_t (ಧ್ರುವ Iterable &o) : hashmap (o) {}
};


#endif /* HB_MAP_HH */

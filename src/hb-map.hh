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

#അന്തര്ഭാവ "hb.hh"

#അന്തര്ഭാവ "hb-set.hh"


/*
 * hb_hashmap_t
 */

ബാഹ്യ HB_INTERNAL സ്ഥിരം hb_codepoint_t minus_1;

template <typename K, typename V,
	  തർക്കം minus_one = അസത്യം>
ഘടന hb_hashmap_t
{
  സ്ഥിതം constexpr തർക്കം realloc_move = സത്യം;

  hb_hashmap_t ()  { init (); }
  ~hb_hashmap_t () { fini (); }

  ശൂന്യം _copy (സ്ഥിരം hb_hashmap_t& o)
  {
    എങ്കിൽ (unlikely (!o.successful))
    {
      successful = അസത്യം;
      തിരികെ;
    }
    എങ്കിൽ (unlikely (!o.mask)) തിരികെ;

    എങ്കിൽ (hb_is_trivially_copy_assignable (item_t))
    {
      ഇനങ്ങൾ = (item_t *) hb_malloc2 ((o.mask + 1), വലുപ്പം (item_t));
      എങ്കിൽ (unlikely (!ഇനങ്ങൾ))
      {
	successful = അസത്യം;
	തിരികെ;
      }
      population = o.population;
      occupancy = o.occupancy;
      mask = o.mask;
      അഭാജ്യം = o.അഭാജ്യം;
      max_chain_length = o.max_chain_length;
      memcpy (ഇനങ്ങൾ, o.ഇനങ്ങൾ, വലുപ്പം (item_t) * (mask + 1));
      തിരികെ;
    }

    alloc (o.population); hb_copy (o, *ഇത്);
  }

  hb_hashmap_t (സ്ഥിരം hb_hashmap_t& o) : hb_hashmap_t () { _copy (o); }
  hb_hashmap_t& operator= (സ്ഥിരം hb_hashmap_t& o)
  {
    reset ();
    എങ്കിൽ (!ഇനങ്ങൾ) { _copy (o); തിരികെ *ഇത്; }
    alloc (o.population); hb_copy (o, *ഇത്); തിരികെ *ഇത്;
  }

  hb_hashmap_t (hb_hashmap_t&& o)  noexcept : hb_hashmap_t () { hb_swap (*ഇത്, o); }
  hb_hashmap_t& operator= (hb_hashmap_t&& o)   noexcept { hb_swap (*ഇത്, o); തിരികെ *ഇത്; }

  hb_hashmap_t (std::initializer_list<hb_pair_t<K, V>> lst) : hb_hashmap_t ()
  {
    വേണ്ടി (auto&& item : lst)
      set (item.first, item.second);
  }
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_hashmap_t (സ്ഥിരം Iterable &o) : hb_hashmap_t ()
  {
    auto iter = hb_iter (o);
    എങ്കിൽ (iter.is_random_access_iterator || iter.has_fast_len)
      alloc (hb_len (iter));
    hb_copy (iter, *ഇത്);
  }

  ഘടന item_t
  {
    K key;
    uint32_t is_real_ : 1;
    uint32_t is_used_ : 1;
    uint32_t hash : 30;
    V value;

    item_t () : key (),
		is_real_ (അസത്യം), is_used_ (അസത്യം),
		hash (0),
		value () {}

    // Needed for https://github.com/harfbuzz/harfbuzz/issues/4138
    K& get_താക്കോൽ () { തിരികെ key; }
    V& get_മൂല്യം () { തിരികെ value; }

    തർക്കം is_used () സ്ഥിരം { തിരികെ is_used_; }
    ശൂന്യം set_used (തർക്കം is_used) { is_used_ = is_used; }
    ശൂന്യം set_real (തർക്കം is_real) { is_real_ = is_real; }
    തർക്കം is_real () സ്ഥിരം { തിരികെ is_real_; }

    template <തർക്കം v = minus_one,
	      hb_enable_if (v == അസത്യം)>
    സ്ഥിതം വരിയിൽ സ്ഥിരം V& default_മൂല്യം () { തിരികെ Null(V); };
    template <തർക്കം v = minus_one,
	      hb_enable_if (v == സത്യം)>
    സ്ഥിതം വരിയിൽ സ്ഥിരം V& default_മൂല്യം ()
    {
      static_assert (hb_is_same (V, hb_codepoint_t), "");
      തിരികെ minus_1;
    };

    തർക്കം operator == (സ്ഥിരം K &o) സ്ഥിരം { തിരികെ hb_deref (key) == hb_deref (o); }
    തർക്കം operator == (സ്ഥിരം item_t &o) സ്ഥിരം { തിരികെ *ഇത് == o.key; }
    hb_pair_t<K, V> get_pair() സ്ഥിരം { തിരികെ hb_pair_t<K, V> (key, value); }
    hb_pair_t<സ്ഥിരം K &, V &> get_pair_ref() { തിരികെ hb_pair_t<സ്ഥിരം K &, V &> (key, value); }

    uint32_t total_hash () സ്ഥിരം
    { തിരികെ (hash * 31u) + hb_hash (value); }

    സ്ഥിതം constexpr തർക്കം is_trivially_constructible = (hb_is_trivially_constructible(K) && hb_is_trivially_constructible(V));
  };

  hb_object_header_t header;
  തർക്കം successful; /* Allocations successful */
  unsigned short max_chain_length;
  unsigned പൂർണ്ണസംഖ്യ population; /* Not including tombstones. */
  unsigned പൂർണ്ണസംഖ്യ occupancy; /* Including tombstones. */
  unsigned പൂർണ്ണസംഖ്യ mask;
  unsigned പൂർണ്ണസംഖ്യ അഭാജ്യം;
  item_t *ഇനങ്ങൾ;

  friend ശൂന്യം swap (hb_hashmap_t& a, hb_hashmap_t& b) noexcept
  {
    എങ്കിൽ (unlikely (!a.successful || !b.successful))
      തിരികെ;
    hb_swap (a.max_chain_length, b.max_chain_length);
    hb_swap (a.population, b.population);
    hb_swap (a.occupancy, b.occupancy);
    hb_swap (a.mask, b.mask);
    hb_swap (a.അഭാജ്യം, b.അഭാജ്യം);
    hb_swap (a.ഇനങ്ങൾ, b.ഇനങ്ങൾ);
  }
  ശൂന്യം init ()
  {
    hb_object_init (ഇത്);

    successful = സത്യം;
    max_chain_length = 0;
    population = occupancy = 0;
    mask = 0;
    അഭാജ്യം = 0;
    ഇനങ്ങൾ = nullptr;
  }
  ശൂന്യം fini ()
  {
    hb_object_fini (ഇത്);

    എങ്കിൽ (likely (ഇനങ്ങൾ))
    {
      unsigned size = mask + 1;
      വേണ്ടി (unsigned i = 0; i < size; i++)
	ഇനങ്ങൾ[i].~item_t ();
      hb_free (ഇനങ്ങൾ);
      ഇനങ്ങൾ = nullptr;
    }
    population = occupancy = 0;
  }

  hb_hashmap_t& reset ()
  {
    successful = സത്യം;
    clear ();
    തിരികെ *ഇത്;
  }

  തർക്കം in_error () സ്ഥിരം { തിരികെ !successful; }

  തർക്കം alloc (unsigned new_population = 0)
  {
    എങ്കിൽ (unlikely (!successful)) തിരികെ അസത്യം;
    എങ്കിൽ (unlikely (population > 0x3FFFFFFFu || new_population > 0x3FFFFFFFu))
    {
      // Population sizes >0x3FFFFFFF will result in power
      // being larger than 31, which in turn leads to a new size
      // that overflows u32.
      successful = അസത്യം;
      തിരികെ അസത്യം;
    }

    എങ്കിൽ (new_population != 0 && (new_population + new_population / 2) < mask) തിരികെ സത്യം;

    unsigned പൂർണ്ണസംഖ്യ power = hb_bit_storage (hb_max (hb_max ((unsigned) population, new_population) * 2, 4u));
    unsigned പൂർണ്ണസംഖ്യ new_size = 1u << power;
    item_t *new_ഇനങ്ങൾ = (item_t *) hb_malloc2 ((size_t) new_size, വലുപ്പം (item_t));
    എങ്കിൽ (unlikely (!new_ഇനങ്ങൾ))
    {
      successful = അസത്യം;
      തിരികെ അസത്യം;
    }
    എങ്കിൽ (!item_t::is_trivially_constructible)
      വേണ്ടി (auto &_ : hb_iter (new_ഇനങ്ങൾ, new_size))
	പുതിയ (&_) item_t ();
    അല്ലെങ്കിൽ
      hb_memset (new_ഇനങ്ങൾ, 0, (size_t) new_size * വലുപ്പം (item_t));

    unsigned പൂർണ്ണസംഖ്യ old_size = size ();
    item_t *old_ഇനങ്ങൾ = ഇനങ്ങൾ;

    /* Switch to new, empty, array. */
    population = occupancy = 0;
    mask = new_size - 1;
    അഭാജ്യം = അഭാജ്യം_for (power);
    max_chain_length = power * 2;
    ഇനങ്ങൾ = new_ഇനങ്ങൾ;

    /* Insert back old items. */
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < old_size; i++)
    {
      എങ്കിൽ (old_ഇനങ്ങൾ[i].is_real ())
      {
	set_with_hash (std::move (old_ഇനങ്ങൾ[i].key),
		       old_ഇനങ്ങൾ[i].hash,
		       std::move (old_ഇനങ്ങൾ[i].value));
      }
    }
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < old_size; i++)
      old_ഇനങ്ങൾ[i].~item_t ();

    hb_free (old_ഇനങ്ങൾ);

    തിരികെ സത്യം;
  }

  template <typename T>
  തർക്കം in_storage (സ്ഥിരം T &v) സ്ഥിരം
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    തിരികെ ഇനങ്ങൾ &&
	   (uintptr_t) ഇനങ്ങൾ <= p && p < (uintptr_t) (ഇനങ്ങൾ + size ());
  }

  template <typename KK, typename VV>
  തർക്കം set_with_hash (KK&& key, uint32_t hash, VV&& value, തർക്കം overwrite = സത്യം)
  {
    എങ്കിൽ (unlikely (!successful)) തിരികെ അസത്യം;
    എങ്കിൽ (unlikely ((occupancy + occupancy / 2) >= mask))
    {
      /* key or value may live in our own storage, eg. m.set (k, m.get (j)).
       * alloc() frees that storage, so copy them out first. */
      എങ്കിൽ (unlikely (in_storage (key) || in_storage (value)))
      {
	K താക്കോൽ_copy;
	താക്കോൽ_copy = std::forward<KK> (key);
	V മൂല്യം_copy;
	മൂല്യം_copy = std::forward<VV> (value);
	തിരികെ set_with_hash (std::move (താക്കോൽ_copy), hash, std::move (മൂല്യം_copy), overwrite);
      }
      എങ്കിൽ (unlikely (!alloc ())) തിരികെ അസത്യം;
    }

    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    unsigned പൂർണ്ണസംഖ്യ tombstone = (unsigned പൂർണ്ണസംഖ്യ) -1;
    unsigned പൂർണ്ണസംഖ്യ i = hash % അഭാജ്യം;
    unsigned length = 0;
    unsigned step = 0;
    ആകുമ്പോൾ (ഇനങ്ങൾ[i].is_used ())
    {
      എങ്കിൽ ((std::is_integral<K>::value || ഇനങ്ങൾ[i].hash == hash) &&
	  ഇനങ്ങൾ[i] == key)
      {
        എങ്കിൽ (!overwrite)
	  തിരികെ അസത്യം;
        അല്ലെങ്കിൽ
	  നിർത്തുക;
      }
      എങ്കിൽ (!ഇനങ്ങൾ[i].is_real () && tombstone == (unsigned) -1)
        tombstone = i;
      i = (i + ++step) & mask;
      length++;
    }

    item_t &item = ഇനങ്ങൾ[tombstone == (unsigned) -1 ? i : tombstone];

    എങ്കിൽ (item.is_used ())
    {
      occupancy--;
      population -= item.is_real ();
    }

    item.key = std::forward<KK> (key);
    item.value = std::forward<VV> (value);
    item.hash = hash;
    item.set_used (സത്യം);
    item.set_real (സത്യം);

    occupancy++;
    population++;

    എങ്കിൽ (unlikely (length > max_chain_length) && occupancy > mask / 8)
      alloc (mask - 8); // This ensures we jump to next larger size

    തിരികെ സത്യം;
  }

  template <typename VV>
  തർക്കം set (സ്ഥിരം K &key, VV&& value, തർക്കം overwrite = സത്യം) { തിരികെ set_with_hash (key, hb_hash (key), std::forward<VV> (value), overwrite); }
  template <typename VV>
  തർക്കം set (K &&key, VV&& value, തർക്കം overwrite = സത്യം)
  {
    uint32_t hash = hb_hash (key);
    തിരികെ set_with_hash (std::move (key), hash, std::forward<VV> (value), overwrite);
  }
  തർക്കം add (സ്ഥിരം K &key)
  {
    uint32_t hash = hb_hash (key);
    തിരികെ set_with_hash (key, hash, item_t::default_മൂല്യം ());
  }

  സ്ഥിരം V& get_with_hash (സ്ഥിരം K &key, uint32_t hash) സ്ഥിരം
  {
    എങ്കിൽ (!ഇനങ്ങൾ) തിരികെ item_t::default_മൂല്യം ();
    auto *item = fetch_item (key, hash);
    എങ്കിൽ (item)
      തിരികെ item->value;
    തിരികെ item_t::default_മൂല്യം ();
  }
  സ്ഥിരം V& get (സ്ഥിരം K &key) സ്ഥിരം
  {
    എങ്കിൽ (!ഇനങ്ങൾ) തിരികെ item_t::default_മൂല്യം ();
    തിരികെ get_with_hash (key, hb_hash (key));
  }

  ശൂന്യം del (സ്ഥിരം K &key)
  {
    എങ്കിൽ (!ഇനങ്ങൾ) തിരികെ;
    auto *item = fetch_item (key, hb_hash (key));
    എങ്കിൽ (item)
    {
      item->set_real (അസത്യം);
      population--;
    }
  }

  /* Has interface. */
  സ്ഥിരം V& operator [] (K k) സ്ഥിരം { തിരികെ get (k); }
  template <typename VV=V>
  തർക്കം has (സ്ഥിരം K &key, VV **vp = nullptr) സ്ഥിരം
  {
    എങ്കിൽ (!ഇനങ്ങൾ) തിരികെ അസത്യം;
    തിരികെ has_with_hash (key, hb_hash (key), vp);
  }
  template <typename VV=V>
  തർക്കം has_with_hash (സ്ഥിരം K &key, uint32_t hash, VV **vp = nullptr) സ്ഥിരം
  {
    എങ്കിൽ (!ഇനങ്ങൾ) തിരികെ അസത്യം;
    auto *item = fetch_item (key, hash);
    എങ്കിൽ (item)
    {
      എങ്കിൽ (vp) *vp = std::addressof (item->value);
      തിരികെ സത്യം;
    }
    തിരികെ അസത്യം;
  }
  item_t *fetch_item (സ്ഥിരം K &key, uint32_t hash) സ്ഥിരം
  {
    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    unsigned പൂർണ്ണസംഖ്യ i = hash % അഭാജ്യം;
    unsigned step = 0;
    ആകുമ്പോൾ (ഇനങ്ങൾ[i].is_used ())
    {
      എങ്കിൽ ((std::is_integral<K>::value || ഇനങ്ങൾ[i].hash == hash) &&
	  ഇനങ്ങൾ[i] == key)
      {
	എങ്കിൽ (ഇനങ്ങൾ[i].is_real ())
	  തിരികെ &ഇനങ്ങൾ[i];
	അല്ലെങ്കിൽ
	  തിരികെ nullptr;
      }
      i = (i + ++step) & mask;
    }
    തിരികെ nullptr;
  }
  /* Projection. */
  സ്ഥിരം V& operator () (K k) സ്ഥിരം { തിരികെ get (k); }

  unsigned size () സ്ഥിരം { തിരികെ mask ? mask + 1 : 0; }

  ശൂന്യം clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (zero-initialized) from any writes.  Any non-empty hashmap
     * is a real heap instance with writable items, so clearing
     * under !successful is safe. */
    എങ്കിൽ (!population && !occupancy) തിരികെ;

    വേണ്ടി (auto &_ : hb_iter (ഇനങ്ങൾ, size ()))
    {
      /* Reconstruct items. */
      _.~item_t ();
      പുതിയ (&_) item_t ();
    }

    population = occupancy = 0;
  }

  തർക്കം is_empty () സ്ഥിരം { തിരികെ population == 0; }
  explicit operator തർക്കം () സ്ഥിരം { തിരികെ !is_empty (); }

  uint32_t hash () സ്ഥിരം
  {
    തിരികെ
    + iter_ഇനങ്ങൾ ()
    | hb_reduce ([] (uint32_t h, സ്ഥിരം item_t &_) { തിരികെ h ^ _.total_hash (); }, (uint32_t) 0u)
    ;
  }

  തർക്കം is_equal (സ്ഥിരം hb_hashmap_t &other) സ്ഥിരം
  {
    എങ്കിൽ (population != other.population) തിരികെ അസത്യം;

    വേണ്ടി (auto pair : iter ())
      എങ്കിൽ (other.get (pair.first) != pair.second)
        തിരികെ അസത്യം;

    തിരികെ സത്യം;
  }
  തർക്കം operator == (സ്ഥിരം hb_hashmap_t &other) സ്ഥിരം { തിരികെ is_equal (other); }
  തർക്കം operator != (സ്ഥിരം hb_hashmap_t &other) സ്ഥിരം { തിരികെ !is_equal (other); }

  unsigned പൂർണ്ണസംഖ്യ get_population () സ്ഥിരം { തിരികെ population; }

  ശൂന്യം update (സ്ഥിരം hb_hashmap_t &other)
  {
    എങ്കിൽ (unlikely (!successful)) തിരികെ;

    hb_copy (other, *ഇത്);
  }

  /*
   * Iterator
   */

  auto iter_ഇനങ്ങൾ () സ്ഥിരം HB_AUTO_RETURN
  (
    + hb_iter (ഇനങ്ങൾ, ഇത്->size ())
    | hb_filter (&item_t::is_real)
  )
  auto iter_ref () സ്ഥിരം HB_AUTO_RETURN
  (
    + ഇത്->iter_ഇനങ്ങൾ ()
    | hb_map (&item_t::get_pair_ref)
  )
  auto iter () സ്ഥിരം HB_AUTO_RETURN
  (
    + ഇത്->iter_ഇനങ്ങൾ ()
    | hb_map (&item_t::get_pair)
  )
  auto താക്കോലുകൾ_ref () സ്ഥിരം HB_AUTO_RETURN
  (
    + ഇത്->iter_ഇനങ്ങൾ ()
    | hb_map (&item_t::get_താക്കോൽ)
  )
  auto താക്കോലുകൾ () സ്ഥിരം HB_AUTO_RETURN
  (
    + ഇത്->താക്കോലുകൾ_ref ()
    | hb_map (hb_ridentity)
  )
  auto മൂല്യങ്ങൾ_ref () സ്ഥിരം HB_AUTO_RETURN
  (
    + ഇത്->iter_ഇനങ്ങൾ ()
    | hb_map (&item_t::get_മൂല്യം)
  )
  auto values () സ്ഥിരം HB_AUTO_RETURN
  (
    + ഇത്->മൂല്യങ്ങൾ_ref ()
    | hb_map (hb_ridentity)
  )

  /* C iterator. */
  തർക്കം next (പൂർണ്ണസംഖ്യ *idx,
	     K *key,
	     V *value) സ്ഥിരം
  {
    unsigned i = (unsigned) (*idx + 1);

    unsigned എണ്ണം = size ();
    ആകുമ്പോൾ (i < എണ്ണം && !ഇനങ്ങൾ[i].is_real ())
      i++;

    എങ്കിൽ (i >= എണ്ണം)
    {
      *idx = -1;
      തിരികെ അസത്യം;
    }

    *key = ഇനങ്ങൾ[i].key;
    *value = ഇനങ്ങൾ[i].value;

    *idx = (signed) i;
    തിരികെ സത്യം;
  }

  /* Sink interface. */
  hb_hashmap_t& operator << (സ്ഥിരം hb_pair_t<K, V>& v)
  { set (v.first, v.second); തിരികെ *ഇത്; }
  template <typename V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (V2))>
  hb_hashmap_t& operator << (സ്ഥിരം hb_pair_t<K, V&&>& v)
  { set (v.first, std::move (v.second)); തിരികെ *ഇത്; }
  template <typename K2 = K,
	    hb_enable_if (!hb_is_trivially_copyable (K2))>
  hb_hashmap_t& operator << (സ്ഥിരം hb_pair_t<K&&, V>& v)
  { set (std::move (v.first), v.second); തിരികെ *ഇത്; }
  template <typename K2 = K, typename V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (K2) &&
			  !hb_is_trivially_copyable (V2))>
  hb_hashmap_t& operator << (സ്ഥിരം hb_pair_t<K&&, V&&>& v)
  { set (std::move (v.first), std::move (v.second)); തിരികെ *ഇത്; }

  സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ അഭാജ്യം_for (unsigned പൂർണ്ണസംഖ്യ shift)
  {
    /* Following comment and table copied from glib. */
    /* Each table size has an associated prime modulo (the first prime
     * lower than the table size) used to find the initial bucket. Probing
     * then works modulo 2^n. The prime modulo is necessary to get a
     * good distribution with poor hash functions.
     */
    /* Not declaring static to make all kinds of compilers happy... */
    /*static*/ സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ അഭാജ്യം_mod [32] =
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

    എങ്കിൽ (unlikely (shift >= ARRAY_LENGTH (അഭാജ്യം_mod)))
      തിരികെ അഭാജ്യം_mod[ARRAY_LENGTH (അഭാജ്യം_mod) - 1];

    തിരികെ അഭാജ്യം_mod[shift];
  }
};

/*
 * hb_map_t
 */

ഘടന hb_map_t : hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       സത്യം>
{
  using hashmap = hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       സത്യം>;

  ~hb_map_t () = സ്ഥിരസ്ഥിതി;
  hb_map_t () : hashmap () {}
  hb_map_t (സ്ഥിരം hb_map_t &o) : hashmap ((hashmap &) o) {}
  hb_map_t (hb_map_t &&o)  noexcept : hashmap (std::move ((hashmap &) o)) {}
  hb_map_t& operator= (സ്ഥിരം hb_map_t&) = സ്ഥിരസ്ഥിതി;
  hb_map_t& operator= (hb_map_t&&) = സ്ഥിരസ്ഥിതി;
  hb_map_t (std::initializer_list<hb_codepoint_pair_t> lst) : hashmap (lst) {}
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_map_t (സ്ഥിരം Iterable &o) : hashmap (o) {}
};


#endif /* HB_MAP_HH */

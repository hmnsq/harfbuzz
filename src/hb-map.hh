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

#અન્તર્ભાવ "hb.hh"

#અન્તર્ભાવ "hb-set.hh"


/*
 * hb_hashmap_t
 */

બાહ્ય HB_INTERNAL અચલ hb_codepoint_t minus_1;

template <typename K, typename V,
	  તાર્કિક minus_one = ખોટું>
સંરચના hb_hashmap_t
{
  સ્થિર constexpr તાર્કિક realloc_move = સાચું;

  hb_hashmap_t ()  { init (); }
  ~hb_hashmap_t () { fini (); }

  શૂન્ય _copy (અચલ hb_hashmap_t& o)
  {
    જો (unlikely (!o.successful))
    {
      successful = ખોટું;
      પાછા;
    }
    જો (unlikely (!o.mask)) પાછા;

    જો (hb_is_trivially_copy_assignable (item_t))
    {
      વસ્તુઓ = (item_t *) hb_malloc2 ((o.mask + 1), કદ (item_t));
      જો (unlikely (!વસ્તુઓ))
      {
	successful = ખોટું;
	પાછા;
      }
      population = o.population;
      occupancy = o.occupancy;
      mask = o.mask;
      અવિભાજ્ય = o.અવિભાજ્ય;
      max_chain_length = o.max_chain_length;
      memcpy (વસ્તુઓ, o.વસ્તુઓ, કદ (item_t) * (mask + 1));
      પાછા;
    }

    alloc (o.population); hb_copy (o, *આ);
  }

  hb_hashmap_t (અચલ hb_hashmap_t& o) : hb_hashmap_t () { _copy (o); }
  hb_hashmap_t& operator= (અચલ hb_hashmap_t& o)
  {
    reset ();
    જો (!વસ્તુઓ) { _copy (o); પાછા *આ; }
    alloc (o.population); hb_copy (o, *આ); પાછા *આ;
  }

  hb_hashmap_t (hb_hashmap_t&& o)  noexcept : hb_hashmap_t () { hb_swap (*આ, o); }
  hb_hashmap_t& operator= (hb_hashmap_t&& o)   noexcept { hb_swap (*આ, o); પાછા *આ; }

  hb_hashmap_t (std::initializer_list<hb_pair_t<K, V>> lst) : hb_hashmap_t ()
  {
    માટે (auto&& item : lst)
      set (item.first, item.second);
  }
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_hashmap_t (અચલ Iterable &o) : hb_hashmap_t ()
  {
    auto iter = hb_iter (o);
    જો (iter.is_random_access_iterator || iter.has_fast_len)
      alloc (hb_len (iter));
    hb_copy (iter, *આ);
  }

  સંરચના item_t
  {
    K key;
    uint32_t is_real_ : 1;
    uint32_t is_used_ : 1;
    uint32_t hash : 30;
    V value;

    item_t () : key (),
		is_real_ (ખોટું), is_used_ (ખોટું),
		hash (0),
		value () {}

    // Needed for https://github.com/harfbuzz/harfbuzz/issues/4138
    K& get_ચાવી () { પાછા key; }
    V& get_કિંમત () { પાછા value; }

    તાર્કિક is_used () અચલ { પાછા is_used_; }
    શૂન્ય set_used (તાર્કિક is_used) { is_used_ = is_used; }
    શૂન્ય set_real (તાર્કિક is_real) { is_real_ = is_real; }
    તાર્કિક is_real () અચલ { પાછા is_real_; }

    template <તાર્કિક v = minus_one,
	      hb_enable_if (v == ખોટું)>
    સ્થિર અંતઃરેખ અચલ V& default_કિંમત () { પાછા Null(V); };
    template <તાર્કિક v = minus_one,
	      hb_enable_if (v == સાચું)>
    સ્થિર અંતઃરેખ અચલ V& default_કિંમત ()
    {
      static_assert (hb_is_same (V, hb_codepoint_t), "");
      પાછા minus_1;
    };

    તાર્કિક operator == (અચલ K &o) અચલ { પાછા hb_deref (key) == hb_deref (o); }
    તાર્કિક operator == (અચલ item_t &o) અચલ { પાછા *આ == o.key; }
    hb_pair_t<K, V> get_pair() અચલ { પાછા hb_pair_t<K, V> (key, value); }
    hb_pair_t<અચલ K &, V &> get_pair_ref() { પાછા hb_pair_t<અચલ K &, V &> (key, value); }

    uint32_t total_hash () અચલ
    { પાછા (hash * 31u) + hb_hash (value); }

    સ્થિર constexpr તાર્કિક is_trivially_constructible = (hb_is_trivially_constructible(K) && hb_is_trivially_constructible(V));
  };

  hb_object_header_t header;
  તાર્કિક successful; /* Allocations successful */
  unsigned short max_chain_length;
  unsigned પૂર્ણાંક population; /* Not including tombstones. */
  unsigned પૂર્ણાંક occupancy; /* Including tombstones. */
  unsigned પૂર્ણાંક mask;
  unsigned પૂર્ણાંક અવિભાજ્ય;
  item_t *વસ્તુઓ;

  friend શૂન્ય swap (hb_hashmap_t& a, hb_hashmap_t& b) noexcept
  {
    જો (unlikely (!a.successful || !b.successful))
      પાછા;
    hb_swap (a.max_chain_length, b.max_chain_length);
    hb_swap (a.population, b.population);
    hb_swap (a.occupancy, b.occupancy);
    hb_swap (a.mask, b.mask);
    hb_swap (a.અવિભાજ્ય, b.અવિભાજ્ય);
    hb_swap (a.વસ્તુઓ, b.વસ્તુઓ);
  }
  શૂન્ય init ()
  {
    hb_object_init (આ);

    successful = સાચું;
    max_chain_length = 0;
    population = occupancy = 0;
    mask = 0;
    અવિભાજ્ય = 0;
    વસ્તુઓ = nullptr;
  }
  શૂન્ય fini ()
  {
    hb_object_fini (આ);

    જો (likely (વસ્તુઓ))
    {
      unsigned size = mask + 1;
      માટે (unsigned i = 0; i < size; i++)
	વસ્તુઓ[i].~item_t ();
      hb_free (વસ્તુઓ);
      વસ્તુઓ = nullptr;
    }
    population = occupancy = 0;
  }

  hb_hashmap_t& reset ()
  {
    successful = સાચું;
    clear ();
    પાછા *આ;
  }

  તાર્કિક in_error () અચલ { પાછા !successful; }

  તાર્કિક alloc (unsigned new_population = 0)
  {
    જો (unlikely (!successful)) પાછા ખોટું;
    જો (unlikely (population > 0x3FFFFFFFu || new_population > 0x3FFFFFFFu))
    {
      // Population sizes >0x3FFFFFFF will result in power
      // being larger than 31, which in turn leads to a new size
      // that overflows u32.
      successful = ખોટું;
      પાછા ખોટું;
    }

    જો (new_population != 0 && (new_population + new_population / 2) < mask) પાછા સાચું;

    unsigned પૂર્ણાંક power = hb_bit_storage (hb_max (hb_max ((unsigned) population, new_population) * 2, 4u));
    unsigned પૂર્ણાંક new_size = 1u << power;
    item_t *new_વસ્તુઓ = (item_t *) hb_malloc2 ((size_t) new_size, કદ (item_t));
    જો (unlikely (!new_વસ્તુઓ))
    {
      successful = ખોટું;
      પાછા ખોટું;
    }
    જો (!item_t::is_trivially_constructible)
      માટે (auto &_ : hb_iter (new_વસ્તુઓ, new_size))
	નવું (&_) item_t ();
    નહીંતર
      hb_memset (new_વસ્તુઓ, 0, (size_t) new_size * કદ (item_t));

    unsigned પૂર્ણાંક old_size = size ();
    item_t *old_વસ્તુઓ = વસ્તુઓ;

    /* Switch to new, empty, array. */
    population = occupancy = 0;
    mask = new_size - 1;
    અવિભાજ્ય = અવિભાજ્ય_for (power);
    max_chain_length = power * 2;
    વસ્તુઓ = new_વસ્તુઓ;

    /* Insert back old items. */
    માટે (unsigned પૂર્ણાંક i = 0; i < old_size; i++)
    {
      જો (old_વસ્તુઓ[i].is_real ())
      {
	set_with_hash (std::move (old_વસ્તુઓ[i].key),
		       old_વસ્તુઓ[i].hash,
		       std::move (old_વસ્તુઓ[i].value));
      }
    }
    માટે (unsigned પૂર્ણાંક i = 0; i < old_size; i++)
      old_વસ્તુઓ[i].~item_t ();

    hb_free (old_વસ્તુઓ);

    પાછા સાચું;
  }

  template <typename T>
  તાર્કિક in_storage (અચલ T &v) અચલ
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    પાછા વસ્તુઓ &&
	   (uintptr_t) વસ્તુઓ <= p && p < (uintptr_t) (વસ્તુઓ + size ());
  }

  template <typename KK, typename VV>
  તાર્કિક set_with_hash (KK&& key, uint32_t hash, VV&& value, તાર્કિક overwrite = સાચું)
  {
    જો (unlikely (!successful)) પાછા ખોટું;
    જો (unlikely ((occupancy + occupancy / 2) >= mask))
    {
      /* key or value may live in our own storage, eg. m.set (k, m.get (j)).
       * alloc() frees that storage, so copy them out first. */
      જો (unlikely (in_storage (key) || in_storage (value)))
      {
	K ચાવી_copy;
	ચાવી_copy = std::forward<KK> (key);
	V કિંમત_copy;
	કિંમત_copy = std::forward<VV> (value);
	પાછા set_with_hash (std::move (ચાવી_copy), hash, std::move (કિંમત_copy), overwrite);
      }
      જો (unlikely (!alloc ())) પાછા ખોટું;
    }

    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    unsigned પૂર્ણાંક tombstone = (unsigned પૂર્ણાંક) -1;
    unsigned પૂર્ણાંક i = hash % અવિભાજ્ય;
    unsigned length = 0;
    unsigned step = 0;
    જ્યારે (વસ્તુઓ[i].is_used ())
    {
      જો ((std::is_integral<K>::value || વસ્તુઓ[i].hash == hash) &&
	  વસ્તુઓ[i] == key)
      {
        જો (!overwrite)
	  પાછા ખોટું;
        નહીંતર
	  તોડો;
      }
      જો (!વસ્તુઓ[i].is_real () && tombstone == (unsigned) -1)
        tombstone = i;
      i = (i + ++step) & mask;
      length++;
    }

    item_t &item = વસ્તુઓ[tombstone == (unsigned) -1 ? i : tombstone];

    જો (item.is_used ())
    {
      occupancy--;
      population -= item.is_real ();
    }

    item.key = std::forward<KK> (key);
    item.value = std::forward<VV> (value);
    item.hash = hash;
    item.set_used (સાચું);
    item.set_real (સાચું);

    occupancy++;
    population++;

    જો (unlikely (length > max_chain_length) && occupancy > mask / 8)
      alloc (mask - 8); // This ensures we jump to next larger size

    પાછા સાચું;
  }

  template <typename VV>
  તાર્કિક set (અચલ K &key, VV&& value, તાર્કિક overwrite = સાચું) { પાછા set_with_hash (key, hb_hash (key), std::forward<VV> (value), overwrite); }
  template <typename VV>
  તાર્કિક set (K &&key, VV&& value, તાર્કિક overwrite = સાચું)
  {
    uint32_t hash = hb_hash (key);
    પાછા set_with_hash (std::move (key), hash, std::forward<VV> (value), overwrite);
  }
  તાર્કિક add (અચલ K &key)
  {
    uint32_t hash = hb_hash (key);
    પાછા set_with_hash (key, hash, item_t::default_કિંમત ());
  }

  અચલ V& get_with_hash (અચલ K &key, uint32_t hash) અચલ
  {
    જો (!વસ્તુઓ) પાછા item_t::default_કિંમત ();
    auto *item = fetch_item (key, hash);
    જો (item)
      પાછા item->value;
    પાછા item_t::default_કિંમત ();
  }
  અચલ V& get (અચલ K &key) અચલ
  {
    જો (!વસ્તુઓ) પાછા item_t::default_કિંમત ();
    પાછા get_with_hash (key, hb_hash (key));
  }

  શૂન્ય del (અચલ K &key)
  {
    જો (!વસ્તુઓ) પાછા;
    auto *item = fetch_item (key, hb_hash (key));
    જો (item)
    {
      item->set_real (ખોટું);
      population--;
    }
  }

  /* Has interface. */
  અચલ V& operator [] (K k) અચલ { પાછા get (k); }
  template <typename VV=V>
  તાર્કિક has (અચલ K &key, VV **vp = nullptr) અચલ
  {
    જો (!વસ્તુઓ) પાછા ખોટું;
    પાછા has_with_hash (key, hb_hash (key), vp);
  }
  template <typename VV=V>
  તાર્કિક has_with_hash (અચલ K &key, uint32_t hash, VV **vp = nullptr) અચલ
  {
    જો (!વસ્તુઓ) પાછા ખોટું;
    auto *item = fetch_item (key, hash);
    જો (item)
    {
      જો (vp) *vp = std::addressof (item->value);
      પાછા સાચું;
    }
    પાછા ખોટું;
  }
  item_t *fetch_item (અચલ K &key, uint32_t hash) અચલ
  {
    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    unsigned પૂર્ણાંક i = hash % અવિભાજ્ય;
    unsigned step = 0;
    જ્યારે (વસ્તુઓ[i].is_used ())
    {
      જો ((std::is_integral<K>::value || વસ્તુઓ[i].hash == hash) &&
	  વસ્તુઓ[i] == key)
      {
	જો (વસ્તુઓ[i].is_real ())
	  પાછા &વસ્તુઓ[i];
	નહીંતર
	  પાછા nullptr;
      }
      i = (i + ++step) & mask;
    }
    પાછા nullptr;
  }
  /* Projection. */
  અચલ V& operator () (K k) અચલ { પાછા get (k); }

  unsigned size () અચલ { પાછા mask ? mask + 1 : 0; }

  શૂન્ય clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (zero-initialized) from any writes.  Any non-empty hashmap
     * is a real heap instance with writable items, so clearing
     * under !successful is safe. */
    જો (!population && !occupancy) પાછા;

    માટે (auto &_ : hb_iter (વસ્તુઓ, size ()))
    {
      /* Reconstruct items. */
      _.~item_t ();
      નવું (&_) item_t ();
    }

    population = occupancy = 0;
  }

  તાર્કિક is_empty () અચલ { પાછા population == 0; }
  explicit operator તાર્કિક () અચલ { પાછા !is_empty (); }

  uint32_t hash () અચલ
  {
    પાછા
    + iter_વસ્તુઓ ()
    | hb_reduce ([] (uint32_t h, અચલ item_t &_) { પાછા h ^ _.total_hash (); }, (uint32_t) 0u)
    ;
  }

  તાર્કિક is_equal (અચલ hb_hashmap_t &other) અચલ
  {
    જો (population != other.population) પાછા ખોટું;

    માટે (auto pair : iter ())
      જો (other.get (pair.first) != pair.second)
        પાછા ખોટું;

    પાછા સાચું;
  }
  તાર્કિક operator == (અચલ hb_hashmap_t &other) અચલ { પાછા is_equal (other); }
  તાર્કિક operator != (અચલ hb_hashmap_t &other) અચલ { પાછા !is_equal (other); }

  unsigned પૂર્ણાંક get_population () અચલ { પાછા population; }

  શૂન્ય update (અચલ hb_hashmap_t &other)
  {
    જો (unlikely (!successful)) પાછા;

    hb_copy (other, *આ);
  }

  /*
   * Iterator
   */

  auto iter_વસ્તુઓ () અચલ HB_AUTO_RETURN
  (
    + hb_iter (વસ્તુઓ, આ->size ())
    | hb_filter (&item_t::is_real)
  )
  auto iter_ref () અચલ HB_AUTO_RETURN
  (
    + આ->iter_વસ્તુઓ ()
    | hb_map (&item_t::get_pair_ref)
  )
  auto iter () અચલ HB_AUTO_RETURN
  (
    + આ->iter_વસ્તુઓ ()
    | hb_map (&item_t::get_pair)
  )
  auto ચાવીઓ_ref () અચલ HB_AUTO_RETURN
  (
    + આ->iter_વસ્તુઓ ()
    | hb_map (&item_t::get_ચાવી)
  )
  auto ચાવીઓ () અચલ HB_AUTO_RETURN
  (
    + આ->ચાવીઓ_ref ()
    | hb_map (hb_ridentity)
  )
  auto કિંમતો_ref () અચલ HB_AUTO_RETURN
  (
    + આ->iter_વસ્તુઓ ()
    | hb_map (&item_t::get_કિંમત)
  )
  auto values () અચલ HB_AUTO_RETURN
  (
    + આ->કિંમતો_ref ()
    | hb_map (hb_ridentity)
  )

  /* C iterator. */
  તાર્કિક next (પૂર્ણાંક *idx,
	     K *key,
	     V *value) અચલ
  {
    unsigned i = (unsigned) (*idx + 1);

    unsigned ગણતરી = size ();
    જ્યારે (i < ગણતરી && !વસ્તુઓ[i].is_real ())
      i++;

    જો (i >= ગણતરી)
    {
      *idx = -1;
      પાછા ખોટું;
    }

    *key = વસ્તુઓ[i].key;
    *value = વસ્તુઓ[i].value;

    *idx = (signed) i;
    પાછા સાચું;
  }

  /* Sink interface. */
  hb_hashmap_t& operator << (અચલ hb_pair_t<K, V>& v)
  { set (v.first, v.second); પાછા *આ; }
  template <typename V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (V2))>
  hb_hashmap_t& operator << (અચલ hb_pair_t<K, V&&>& v)
  { set (v.first, std::move (v.second)); પાછા *આ; }
  template <typename K2 = K,
	    hb_enable_if (!hb_is_trivially_copyable (K2))>
  hb_hashmap_t& operator << (અચલ hb_pair_t<K&&, V>& v)
  { set (std::move (v.first), v.second); પાછા *આ; }
  template <typename K2 = K, typename V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (K2) &&
			  !hb_is_trivially_copyable (V2))>
  hb_hashmap_t& operator << (અચલ hb_pair_t<K&&, V&&>& v)
  { set (std::move (v.first), std::move (v.second)); પાછા *આ; }

  સ્થિર unsigned પૂર્ણાંક અવિભાજ્ય_for (unsigned પૂર્ણાંક shift)
  {
    /* Following comment and table copied from glib. */
    /* Each table size has an associated prime modulo (the first prime
     * lower than the table size) used to find the initial bucket. Probing
     * then works modulo 2^n. The prime modulo is necessary to get a
     * good distribution with poor hash functions.
     */
    /* Not declaring static to make all kinds of compilers happy... */
    /*static*/ અચલ unsigned પૂર્ણાંક અવિભાજ્ય_mod [32] =
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

    જો (unlikely (shift >= ARRAY_LENGTH (અવિભાજ્ય_mod)))
      પાછા અવિભાજ્ય_mod[ARRAY_LENGTH (અવિભાજ્ય_mod) - 1];

    પાછા અવિભાજ્ય_mod[shift];
  }
};

/*
 * hb_map_t
 */

સંરચના hb_map_t : hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       સાચું>
{
  using hashmap = hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       સાચું>;

  ~hb_map_t () = મૂળભૂત;
  hb_map_t () : hashmap () {}
  hb_map_t (અચલ hb_map_t &o) : hashmap ((hashmap &) o) {}
  hb_map_t (hb_map_t &&o)  noexcept : hashmap (std::move ((hashmap &) o)) {}
  hb_map_t& operator= (અચલ hb_map_t&) = મૂળભૂત;
  hb_map_t& operator= (hb_map_t&&) = મૂળભૂત;
  hb_map_t (std::initializer_list<hb_codepoint_pair_t> lst) : hashmap (lst) {}
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_map_t (અચલ Iterable &o) : hashmap (o) {}
};


#endif /* HB_MAP_HH */

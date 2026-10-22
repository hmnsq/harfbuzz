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

#अन्तर्भाव "hb.hh"

#अन्तर्भाव "hb-set.hh"


/*
 * hb_hashmap_t
 */

बाह्य HB_INTERNAL ध्रुव hb_codepoint_t minus_1;

template <typename K, typename V,
	  तार्किक minus_one = मिच्छ>
संरचना hb_hashmap_t
{
  स्थिर constexpr तार्किक realloc_move = सच्च;

  hb_hashmap_t ()  { init (); }
  ~hb_hashmap_t () { fini (); }

  शून्य _copy (ध्रुव hb_hashmap_t& o)
  {
    जइ (unlikely (!o.successful))
    {
      successful = मिच्छ;
      प्रत्यागम;
    }
    जइ (unlikely (!o.mask)) प्रत्यागम;

    जइ (hb_is_trivially_copy_assignable (item_t))
    {
      वस्तूनि = (item_t *) hb_malloc2 ((o.mask + 1), परिमाण (item_t));
      जइ (unlikely (!वस्तूनि))
      {
	successful = मिच्छ;
	प्रत्यागम;
      }
      population = o.population;
      occupancy = o.occupancy;
      mask = o.mask;
      अविभाज्यः = o.अविभाज्यः;
      max_chain_length = o.max_chain_length;
      memcpy (वस्तूनि, o.वस्तूनि, परिमाण (item_t) * (mask + 1));
      प्रत्यागम;
    }

    alloc (o.population); hb_copy (o, *एतत्);
  }

  hb_hashmap_t (ध्रुव hb_hashmap_t& o) : hb_hashmap_t () { _copy (o); }
  hb_hashmap_t& operator= (ध्रुव hb_hashmap_t& o)
  {
    reset ();
    जइ (!वस्तूनि) { _copy (o); प्रत्यागम *एतत्; }
    alloc (o.population); hb_copy (o, *एतत्); प्रत्यागम *एतत्;
  }

  hb_hashmap_t (hb_hashmap_t&& o)  noexcept : hb_hashmap_t () { hb_swap (*एतत्, o); }
  hb_hashmap_t& operator= (hb_hashmap_t&& o)   noexcept { hb_swap (*एतत्, o); प्रत्यागम *एतत्; }

  hb_hashmap_t (std::initializer_list<hb_pair_t<K, V>> lst) : hb_hashmap_t ()
  {
    प्रति (auto&& item : lst)
      set (item.first, item.second);
  }
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_hashmap_t (ध्रुव Iterable &o) : hb_hashmap_t ()
  {
    auto iter = hb_iter (o);
    जइ (iter.is_random_access_iterator || iter.has_fast_len)
      alloc (hb_len (iter));
    hb_copy (iter, *एतत्);
  }

  संरचना item_t
  {
    K key;
    uint32_t is_real_ : 1;
    uint32_t is_used_ : 1;
    uint32_t hash : 30;
    V value;

    item_t () : key (),
		is_real_ (मिच्छ), is_used_ (मिच्छ),
		hash (0),
		value () {}

    // Needed for https://github.com/harfbuzz/harfbuzz/issues/4138
    K& get_कुञ्चिका () { प्रत्यागम key; }
    V& get_मानम् () { प्रत्यागम value; }

    तार्किक is_used () ध्रुव { प्रत्यागम is_used_; }
    शून्य set_used (तार्किक is_used) { is_used_ = is_used; }
    शून्य set_real (तार्किक is_real) { is_real_ = is_real; }
    तार्किक is_real () ध्रुव { प्रत्यागम is_real_; }

    template <तार्किक v = minus_one,
	      hb_enable_if (v == मिच्छ)>
    स्थिर अन्तःस्थ ध्रुव V& default_मानम् () { प्रत्यागम Null(V); };
    template <तार्किक v = minus_one,
	      hb_enable_if (v == सच्च)>
    स्थिर अन्तःस्थ ध्रुव V& default_मानम् ()
    {
      static_assert (hb_is_same (V, hb_codepoint_t), "");
      प्रत्यागम minus_1;
    };

    तार्किक operator == (ध्रुव K &o) ध्रुव { प्रत्यागम hb_deref (key) == hb_deref (o); }
    तार्किक operator == (ध्रुव item_t &o) ध्रुव { प्रत्यागम *एतत् == o.key; }
    hb_pair_t<K, V> get_pair() ध्रुव { प्रत्यागम hb_pair_t<K, V> (key, value); }
    hb_pair_t<ध्रुव K &, V &> get_pair_ref() { प्रत्यागम hb_pair_t<ध्रुव K &, V &> (key, value); }

    uint32_t total_hash () ध्रुव
    { प्रत्यागम (hash * 31u) + hb_hash (value); }

    स्थिर constexpr तार्किक is_trivially_constructible = (hb_is_trivially_constructible(K) && hb_is_trivially_constructible(V));
  };

  hb_object_header_t header;
  तार्किक successful; /* Allocations successful */
  unsigned short max_chain_length;
  unsigned पूर्णाङ्क population; /* Not including tombstones. */
  unsigned पूर्णाङ्क occupancy; /* Including tombstones. */
  unsigned पूर्णाङ्क mask;
  unsigned पूर्णाङ्क अविभाज्यः;
  item_t *वस्तूनि;

  friend शून्य swap (hb_hashmap_t& a, hb_hashmap_t& b) noexcept
  {
    जइ (unlikely (!a.successful || !b.successful))
      प्रत्यागम;
    hb_swap (a.max_chain_length, b.max_chain_length);
    hb_swap (a.population, b.population);
    hb_swap (a.occupancy, b.occupancy);
    hb_swap (a.mask, b.mask);
    hb_swap (a.अविभाज्यः, b.अविभाज्यः);
    hb_swap (a.वस्तूनि, b.वस्तूनि);
  }
  शून्य init ()
  {
    hb_object_init (एतत्);

    successful = सच्च;
    max_chain_length = 0;
    population = occupancy = 0;
    mask = 0;
    अविभाज्यः = 0;
    वस्तूनि = nullptr;
  }
  शून्य fini ()
  {
    hb_object_fini (एतत्);

    जइ (likely (वस्तूनि))
    {
      unsigned size = mask + 1;
      प्रति (unsigned i = 0; i < size; i++)
	वस्तूनि[i].~item_t ();
      hb_free (वस्तूनि);
      वस्तूनि = nullptr;
    }
    population = occupancy = 0;
  }

  hb_hashmap_t& reset ()
  {
    successful = सच्च;
    clear ();
    प्रत्यागम *एतत्;
  }

  तार्किक in_error () ध्रुव { प्रत्यागम !successful; }

  तार्किक alloc (unsigned new_population = 0)
  {
    जइ (unlikely (!successful)) प्रत्यागम मिच्छ;
    जइ (unlikely (population > 0x3FFFFFFFu || new_population > 0x3FFFFFFFu))
    {
      // Population sizes >0x3FFFFFFF will result in power
      // being larger than 31, which in turn leads to a new size
      // that overflows u32.
      successful = मिच्छ;
      प्रत्यागम मिच्छ;
    }

    जइ (new_population != 0 && (new_population + new_population / 2) < mask) प्रत्यागम सच्च;

    unsigned पूर्णाङ्क power = hb_bit_storage (hb_max (hb_max ((unsigned) population, new_population) * 2, 4u));
    unsigned पूर्णाङ्क new_size = 1u << power;
    item_t *new_वस्तूनि = (item_t *) hb_malloc2 ((size_t) new_size, परिमाण (item_t));
    जइ (unlikely (!new_वस्तूनि))
    {
      successful = मिच्छ;
      प्रत्यागम मिच्छ;
    }
    जइ (!item_t::is_trivially_constructible)
      प्रति (auto &_ : hb_iter (new_वस्तूनि, new_size))
	नूतन (&_) item_t ();
    अण्णहा
      hb_memset (new_वस्तूनि, 0, (size_t) new_size * परिमाण (item_t));

    unsigned पूर्णाङ्क old_size = size ();
    item_t *old_वस्तूनि = वस्तूनि;

    /* Switch to new, empty, array. */
    population = occupancy = 0;
    mask = new_size - 1;
    अविभाज्यः = अविभाज्यः_for (power);
    max_chain_length = power * 2;
    वस्तूनि = new_वस्तूनि;

    /* Insert back old items. */
    प्रति (unsigned पूर्णाङ्क i = 0; i < old_size; i++)
    {
      जइ (old_वस्तूनि[i].is_real ())
      {
	set_with_hash (std::move (old_वस्तूनि[i].key),
		       old_वस्तूनि[i].hash,
		       std::move (old_वस्तूनि[i].value));
      }
    }
    प्रति (unsigned पूर्णाङ्क i = 0; i < old_size; i++)
      old_वस्तूनि[i].~item_t ();

    hb_free (old_वस्तूनि);

    प्रत्यागम सच्च;
  }

  template <typename T>
  तार्किक in_storage (ध्रुव T &v) ध्रुव
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    प्रत्यागम वस्तूनि &&
	   (uintptr_t) वस्तूनि <= p && p < (uintptr_t) (वस्तूनि + size ());
  }

  template <typename KK, typename VV>
  तार्किक set_with_hash (KK&& key, uint32_t hash, VV&& value, तार्किक overwrite = सच्च)
  {
    जइ (unlikely (!successful)) प्रत्यागम मिच्छ;
    जइ (unlikely ((occupancy + occupancy / 2) >= mask))
    {
      /* key or value may live in our own storage, eg. m.set (k, m.get (j)).
       * alloc() frees that storage, so copy them out first. */
      जइ (unlikely (in_storage (key) || in_storage (value)))
      {
	K कुञ्चिका_copy;
	कुञ्चिका_copy = std::forward<KK> (key);
	V मानम्_copy;
	मानम्_copy = std::forward<VV> (value);
	प्रत्यागम set_with_hash (std::move (कुञ्चिका_copy), hash, std::move (मानम्_copy), overwrite);
      }
      जइ (unlikely (!alloc ())) प्रत्यागम मिच्छ;
    }

    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    unsigned पूर्णाङ्क tombstone = (unsigned पूर्णाङ्क) -1;
    unsigned पूर्णाङ्क i = hash % अविभाज्यः;
    unsigned length = 0;
    unsigned step = 0;
    जाव (वस्तूनि[i].is_used ())
    {
      जइ ((std::is_integral<K>::value || वस्तूनि[i].hash == hash) &&
	  वस्तूनि[i] == key)
      {
        जइ (!overwrite)
	  प्रत्यागम मिच्छ;
        अण्णहा
	  भङ्ग;
      }
      जइ (!वस्तूनि[i].is_real () && tombstone == (unsigned) -1)
        tombstone = i;
      i = (i + ++step) & mask;
      length++;
    }

    item_t &item = वस्तूनि[tombstone == (unsigned) -1 ? i : tombstone];

    जइ (item.is_used ())
    {
      occupancy--;
      population -= item.is_real ();
    }

    item.key = std::forward<KK> (key);
    item.value = std::forward<VV> (value);
    item.hash = hash;
    item.set_used (सच्च);
    item.set_real (सच्च);

    occupancy++;
    population++;

    जइ (unlikely (length > max_chain_length) && occupancy > mask / 8)
      alloc (mask - 8); // This ensures we jump to next larger size

    प्रत्यागम सच्च;
  }

  template <typename VV>
  तार्किक set (ध्रुव K &key, VV&& value, तार्किक overwrite = सच्च) { प्रत्यागम set_with_hash (key, hb_hash (key), std::forward<VV> (value), overwrite); }
  template <typename VV>
  तार्किक set (K &&key, VV&& value, तार्किक overwrite = सच्च)
  {
    uint32_t hash = hb_hash (key);
    प्रत्यागम set_with_hash (std::move (key), hash, std::forward<VV> (value), overwrite);
  }
  तार्किक add (ध्रुव K &key)
  {
    uint32_t hash = hb_hash (key);
    प्रत्यागम set_with_hash (key, hash, item_t::default_मानम् ());
  }

  ध्रुव V& get_with_hash (ध्रुव K &key, uint32_t hash) ध्रुव
  {
    जइ (!वस्तूनि) प्रत्यागम item_t::default_मानम् ();
    auto *item = fetch_item (key, hash);
    जइ (item)
      प्रत्यागम item->value;
    प्रत्यागम item_t::default_मानम् ();
  }
  ध्रुव V& get (ध्रुव K &key) ध्रुव
  {
    जइ (!वस्तूनि) प्रत्यागम item_t::default_मानम् ();
    प्रत्यागम get_with_hash (key, hb_hash (key));
  }

  शून्य del (ध्रुव K &key)
  {
    जइ (!वस्तूनि) प्रत्यागम;
    auto *item = fetch_item (key, hb_hash (key));
    जइ (item)
    {
      item->set_real (मिच्छ);
      population--;
    }
  }

  /* Has interface. */
  ध्रुव V& operator [] (K k) ध्रुव { प्रत्यागम get (k); }
  template <typename VV=V>
  तार्किक has (ध्रुव K &key, VV **vp = nullptr) ध्रुव
  {
    जइ (!वस्तूनि) प्रत्यागम मिच्छ;
    प्रत्यागम has_with_hash (key, hb_hash (key), vp);
  }
  template <typename VV=V>
  तार्किक has_with_hash (ध्रुव K &key, uint32_t hash, VV **vp = nullptr) ध्रुव
  {
    जइ (!वस्तूनि) प्रत्यागम मिच्छ;
    auto *item = fetch_item (key, hash);
    जइ (item)
    {
      जइ (vp) *vp = std::addressof (item->value);
      प्रत्यागम सच्च;
    }
    प्रत्यागम मिच्छ;
  }
  item_t *fetch_item (ध्रुव K &key, uint32_t hash) ध्रुव
  {
    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    unsigned पूर्णाङ्क i = hash % अविभाज्यः;
    unsigned step = 0;
    जाव (वस्तूनि[i].is_used ())
    {
      जइ ((std::is_integral<K>::value || वस्तूनि[i].hash == hash) &&
	  वस्तूनि[i] == key)
      {
	जइ (वस्तूनि[i].is_real ())
	  प्रत्यागम &वस्तूनि[i];
	अण्णहा
	  प्रत्यागम nullptr;
      }
      i = (i + ++step) & mask;
    }
    प्रत्यागम nullptr;
  }
  /* Projection. */
  ध्रुव V& operator () (K k) ध्रुव { प्रत्यागम get (k); }

  unsigned size () ध्रुव { प्रत्यागम mask ? mask + 1 : 0; }

  शून्य clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (zero-initialized) from any writes.  Any non-empty hashmap
     * is a real heap instance with writable items, so clearing
     * under !successful is safe. */
    जइ (!population && !occupancy) प्रत्यागम;

    प्रति (auto &_ : hb_iter (वस्तूनि, size ()))
    {
      /* Reconstruct items. */
      _.~item_t ();
      नूतन (&_) item_t ();
    }

    population = occupancy = 0;
  }

  तार्किक is_empty () ध्रुव { प्रत्यागम population == 0; }
  explicit operator तार्किक () ध्रुव { प्रत्यागम !is_empty (); }

  uint32_t hash () ध्रुव
  {
    प्रत्यागम
    + iter_वस्तूनि ()
    | hb_reduce ([] (uint32_t h, ध्रुव item_t &_) { प्रत्यागम h ^ _.total_hash (); }, (uint32_t) 0u)
    ;
  }

  तार्किक is_equal (ध्रुव hb_hashmap_t &other) ध्रुव
  {
    जइ (population != other.population) प्रत्यागम मिच्छ;

    प्रति (auto pair : iter ())
      जइ (other.get (pair.first) != pair.second)
        प्रत्यागम मिच्छ;

    प्रत्यागम सच्च;
  }
  तार्किक operator == (ध्रुव hb_hashmap_t &other) ध्रुव { प्रत्यागम is_equal (other); }
  तार्किक operator != (ध्रुव hb_hashmap_t &other) ध्रुव { प्रत्यागम !is_equal (other); }

  unsigned पूर्णाङ्क get_population () ध्रुव { प्रत्यागम population; }

  शून्य update (ध्रुव hb_hashmap_t &other)
  {
    जइ (unlikely (!successful)) प्रत्यागम;

    hb_copy (other, *एतत्);
  }

  /*
   * Iterator
   */

  auto iter_वस्तूनि () ध्रुव HB_AUTO_RETURN
  (
    + hb_iter (वस्तूनि, एतत्->size ())
    | hb_filter (&item_t::is_real)
  )
  auto iter_ref () ध्रुव HB_AUTO_RETURN
  (
    + एतत्->iter_वस्तूनि ()
    | hb_map (&item_t::get_pair_ref)
  )
  auto iter () ध्रुव HB_AUTO_RETURN
  (
    + एतत्->iter_वस्तूनि ()
    | hb_map (&item_t::get_pair)
  )
  auto कुञ्चिकाः_ref () ध्रुव HB_AUTO_RETURN
  (
    + एतत्->iter_वस्तूनि ()
    | hb_map (&item_t::get_कुञ्चिका)
  )
  auto कुञ्चिकाः () ध्रुव HB_AUTO_RETURN
  (
    + एतत्->कुञ्चिकाः_ref ()
    | hb_map (hb_ridentity)
  )
  auto मानानि_ref () ध्रुव HB_AUTO_RETURN
  (
    + एतत्->iter_वस्तूनि ()
    | hb_map (&item_t::get_मानम्)
  )
  auto values () ध्रुव HB_AUTO_RETURN
  (
    + एतत्->मानानि_ref ()
    | hb_map (hb_ridentity)
  )

  /* C iterator. */
  तार्किक next (पूर्णाङ्क *idx,
	     K *key,
	     V *value) ध्रुव
  {
    unsigned i = (unsigned) (*idx + 1);

    unsigned गणनम् = size ();
    जाव (i < गणनम् && !वस्तूनि[i].is_real ())
      i++;

    जइ (i >= गणनम्)
    {
      *idx = -1;
      प्रत्यागम मिच्छ;
    }

    *key = वस्तूनि[i].key;
    *value = वस्तूनि[i].value;

    *idx = (signed) i;
    प्रत्यागम सच्च;
  }

  /* Sink interface. */
  hb_hashmap_t& operator << (ध्रुव hb_pair_t<K, V>& v)
  { set (v.first, v.second); प्रत्यागम *एतत्; }
  template <typename V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (V2))>
  hb_hashmap_t& operator << (ध्रुव hb_pair_t<K, V&&>& v)
  { set (v.first, std::move (v.second)); प्रत्यागम *एतत्; }
  template <typename K2 = K,
	    hb_enable_if (!hb_is_trivially_copyable (K2))>
  hb_hashmap_t& operator << (ध्रुव hb_pair_t<K&&, V>& v)
  { set (std::move (v.first), v.second); प्रत्यागम *एतत्; }
  template <typename K2 = K, typename V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (K2) &&
			  !hb_is_trivially_copyable (V2))>
  hb_hashmap_t& operator << (ध्रुव hb_pair_t<K&&, V&&>& v)
  { set (std::move (v.first), std::move (v.second)); प्रत्यागम *एतत्; }

  स्थिर unsigned पूर्णाङ्क अविभाज्यः_for (unsigned पूर्णाङ्क shift)
  {
    /* Following comment and table copied from glib. */
    /* Each table size has an associated prime modulo (the first prime
     * lower than the table size) used to find the initial bucket. Probing
     * then works modulo 2^n. The prime modulo is necessary to get a
     * good distribution with poor hash functions.
     */
    /* Not declaring static to make all kinds of compilers happy... */
    /*static*/ ध्रुव unsigned पूर्णाङ्क अविभाज्यः_mod [32] =
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

    जइ (unlikely (shift >= ARRAY_LENGTH (अविभाज्यः_mod)))
      प्रत्यागम अविभाज्यः_mod[ARRAY_LENGTH (अविभाज्यः_mod) - 1];

    प्रत्यागम अविभाज्यः_mod[shift];
  }
};

/*
 * hb_map_t
 */

संरचना hb_map_t : hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       सच्च>
{
  using hashmap = hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       सच्च>;

  ~hb_map_t () = स्वभाव;
  hb_map_t () : hashmap () {}
  hb_map_t (ध्रुव hb_map_t &o) : hashmap ((hashmap &) o) {}
  hb_map_t (hb_map_t &&o)  noexcept : hashmap (std::move ((hashmap &) o)) {}
  hb_map_t& operator= (ध्रुव hb_map_t&) = स्वभाव;
  hb_map_t& operator= (hb_map_t&&) = स्वभाव;
  hb_map_t (std::initializer_list<hb_codepoint_pair_t> lst) : hashmap (lst) {}
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_map_t (ध्रुव Iterable &o) : hashmap (o) {}
};


#endif /* HB_MAP_HH */

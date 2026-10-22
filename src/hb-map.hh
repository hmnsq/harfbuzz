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
	  तार्किक minus_one = असत्य>
संरचना hb_hashmap_t
{
  स्थिर constexpr तार्किक realloc_move = सत्य;

  hb_hashmap_t ()  { init (); }
  ~hb_hashmap_t () { fini (); }

  शून्य _copy (ध्रुव hb_hashmap_t& o)
  {
    यदि (unlikely (!o.successful))
    {
      successful = असत्य;
      प्रत्यागम;
    }
    यदि (unlikely (!o.mask)) प्रत्यागम;

    यदि (hb_is_trivially_copy_assignable (item_t))
    {
      वस्तुएँ = (item_t *) hb_malloc2 ((o.mask + 1), परिमाण (item_t));
      यदि (unlikely (!वस्तुएँ))
      {
	successful = असत्य;
	प्रत्यागम;
      }
      population = o.population;
      occupancy = o.occupancy;
      mask = o.mask;
      अभाज्य = o.अभाज्य;
      max_chain_length = o.max_chain_length;
      memcpy (वस्तुएँ, o.वस्तुएँ, परिमाण (item_t) * (mask + 1));
      प्रत्यागम;
    }

    alloc (o.population); hb_copy (o, *एतत्);
  }

  hb_hashmap_t (ध्रुव hb_hashmap_t& o) : hb_hashmap_t () { _copy (o); }
  hb_hashmap_t& operator= (ध्रुव hb_hashmap_t& o)
  {
    reset ();
    यदि (!वस्तुएँ) { _copy (o); प्रत्यागम *एतत्; }
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
    यदि (iter.is_random_access_iterator || iter.has_fast_len)
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
		is_real_ (असत्य), is_used_ (असत्य),
		hash (0),
		value () {}

    // Needed for https://github.com/harfbuzz/harfbuzz/issues/4138
    K& get_कुंजी () { प्रत्यागम key; }
    V& get_मूल्य () { प्रत्यागम value; }

    तार्किक is_used () ध्रुव { प्रत्यागम is_used_; }
    शून्य set_used (तार्किक is_used) { is_used_ = is_used; }
    शून्य set_real (तार्किक is_real) { is_real_ = is_real; }
    तार्किक is_real () ध्रुव { प्रत्यागम is_real_; }

    template <तार्किक v = minus_one,
	      hb_enable_if (v == असत्य)>
    स्थिर अन्तःस्थ ध्रुव V& default_मूल्य () { प्रत्यागम Null(V); };
    template <तार्किक v = minus_one,
	      hb_enable_if (v == सत्य)>
    स्थिर अन्तःस्थ ध्रुव V& default_मूल्य ()
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
  unsigned पूर्णाङ्क अभाज्य;
  item_t *वस्तुएँ;

  friend शून्य swap (hb_hashmap_t& a, hb_hashmap_t& b) noexcept
  {
    यदि (unlikely (!a.successful || !b.successful))
      प्रत्यागम;
    hb_swap (a.max_chain_length, b.max_chain_length);
    hb_swap (a.population, b.population);
    hb_swap (a.occupancy, b.occupancy);
    hb_swap (a.mask, b.mask);
    hb_swap (a.अभाज्य, b.अभाज्य);
    hb_swap (a.वस्तुएँ, b.वस्तुएँ);
  }
  शून्य init ()
  {
    hb_object_init (एतत्);

    successful = सत्य;
    max_chain_length = 0;
    population = occupancy = 0;
    mask = 0;
    अभाज्य = 0;
    वस्तुएँ = nullptr;
  }
  शून्य fini ()
  {
    hb_object_fini (एतत्);

    यदि (likely (वस्तुएँ))
    {
      unsigned size = mask + 1;
      प्रति (unsigned i = 0; i < size; i++)
	वस्तुएँ[i].~item_t ();
      hb_free (वस्तुएँ);
      वस्तुएँ = nullptr;
    }
    population = occupancy = 0;
  }

  hb_hashmap_t& reset ()
  {
    successful = सत्य;
    clear ();
    प्रत्यागम *एतत्;
  }

  तार्किक in_error () ध्रुव { प्रत्यागम !successful; }

  तार्किक alloc (unsigned new_population = 0)
  {
    यदि (unlikely (!successful)) प्रत्यागम असत्य;
    यदि (unlikely (population > 0x3FFFFFFFu || new_population > 0x3FFFFFFFu))
    {
      // Population sizes >0x3FFFFFFF will result in power
      // being larger than 31, which in turn leads to a new size
      // that overflows u32.
      successful = असत्य;
      प्रत्यागम असत्य;
    }

    यदि (new_population != 0 && (new_population + new_population / 2) < mask) प्रत्यागम सत्य;

    unsigned पूर्णाङ्क power = hb_bit_storage (hb_max (hb_max ((unsigned) population, new_population) * 2, 4u));
    unsigned पूर्णाङ्क new_size = 1u << power;
    item_t *new_वस्तुएँ = (item_t *) hb_malloc2 ((size_t) new_size, परिमाण (item_t));
    यदि (unlikely (!new_वस्तुएँ))
    {
      successful = असत्य;
      प्रत्यागम असत्य;
    }
    यदि (!item_t::is_trivially_constructible)
      प्रति (auto &_ : hb_iter (new_वस्तुएँ, new_size))
	नूतन (&_) item_t ();
    अन्यथा
      hb_memset (new_वस्तुएँ, 0, (size_t) new_size * परिमाण (item_t));

    unsigned पूर्णाङ्क old_size = size ();
    item_t *old_वस्तुएँ = वस्तुएँ;

    /* Switch to new, empty, array. */
    population = occupancy = 0;
    mask = new_size - 1;
    अभाज्य = अभाज्य_for (power);
    max_chain_length = power * 2;
    वस्तुएँ = new_वस्तुएँ;

    /* Insert back old items. */
    प्रति (unsigned पूर्णाङ्क i = 0; i < old_size; i++)
    {
      यदि (old_वस्तुएँ[i].is_real ())
      {
	set_with_hash (std::move (old_वस्तुएँ[i].key),
		       old_वस्तुएँ[i].hash,
		       std::move (old_वस्तुएँ[i].value));
      }
    }
    प्रति (unsigned पूर्णाङ्क i = 0; i < old_size; i++)
      old_वस्तुएँ[i].~item_t ();

    hb_free (old_वस्तुएँ);

    प्रत्यागम सत्य;
  }

  template <typename T>
  तार्किक in_storage (ध्रुव T &v) ध्रुव
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    प्रत्यागम वस्तुएँ &&
	   (uintptr_t) वस्तुएँ <= p && p < (uintptr_t) (वस्तुएँ + size ());
  }

  template <typename KK, typename VV>
  तार्किक set_with_hash (KK&& key, uint32_t hash, VV&& value, तार्किक overwrite = सत्य)
  {
    यदि (unlikely (!successful)) प्रत्यागम असत्य;
    यदि (unlikely ((occupancy + occupancy / 2) >= mask))
    {
      /* key or value may live in our own storage, eg. m.set (k, m.get (j)).
       * alloc() frees that storage, so copy them out first. */
      यदि (unlikely (in_storage (key) || in_storage (value)))
      {
	K कुंजी_copy;
	कुंजी_copy = std::forward<KK> (key);
	V मूल्य_copy;
	मूल्य_copy = std::forward<VV> (value);
	प्रत्यागम set_with_hash (std::move (कुंजी_copy), hash, std::move (मूल्य_copy), overwrite);
      }
      यदि (unlikely (!alloc ())) प्रत्यागम असत्य;
    }

    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    unsigned पूर्णाङ्क tombstone = (unsigned पूर्णाङ्क) -1;
    unsigned पूर्णाङ्क i = hash % अभाज्य;
    unsigned length = 0;
    unsigned step = 0;
    यावत् (वस्तुएँ[i].is_used ())
    {
      यदि ((std::is_integral<K>::value || वस्तुएँ[i].hash == hash) &&
	  वस्तुएँ[i] == key)
      {
        यदि (!overwrite)
	  प्रत्यागम असत्य;
        अन्यथा
	  भङ्ग;
      }
      यदि (!वस्तुएँ[i].is_real () && tombstone == (unsigned) -1)
        tombstone = i;
      i = (i + ++step) & mask;
      length++;
    }

    item_t &item = वस्तुएँ[tombstone == (unsigned) -1 ? i : tombstone];

    यदि (item.is_used ())
    {
      occupancy--;
      population -= item.is_real ();
    }

    item.key = std::forward<KK> (key);
    item.value = std::forward<VV> (value);
    item.hash = hash;
    item.set_used (सत्य);
    item.set_real (सत्य);

    occupancy++;
    population++;

    यदि (unlikely (length > max_chain_length) && occupancy > mask / 8)
      alloc (mask - 8); // This ensures we jump to next larger size

    प्रत्यागम सत्य;
  }

  template <typename VV>
  तार्किक set (ध्रुव K &key, VV&& value, तार्किक overwrite = सत्य) { प्रत्यागम set_with_hash (key, hb_hash (key), std::forward<VV> (value), overwrite); }
  template <typename VV>
  तार्किक set (K &&key, VV&& value, तार्किक overwrite = सत्य)
  {
    uint32_t hash = hb_hash (key);
    प्रत्यागम set_with_hash (std::move (key), hash, std::forward<VV> (value), overwrite);
  }
  तार्किक add (ध्रुव K &key)
  {
    uint32_t hash = hb_hash (key);
    प्रत्यागम set_with_hash (key, hash, item_t::default_मूल्य ());
  }

  ध्रुव V& get_with_hash (ध्रुव K &key, uint32_t hash) ध्रुव
  {
    यदि (!वस्तुएँ) प्रत्यागम item_t::default_मूल्य ();
    auto *item = fetch_item (key, hash);
    यदि (item)
      प्रत्यागम item->value;
    प्रत्यागम item_t::default_मूल्य ();
  }
  ध्रुव V& get (ध्रुव K &key) ध्रुव
  {
    यदि (!वस्तुएँ) प्रत्यागम item_t::default_मूल्य ();
    प्रत्यागम get_with_hash (key, hb_hash (key));
  }

  शून्य del (ध्रुव K &key)
  {
    यदि (!वस्तुएँ) प्रत्यागम;
    auto *item = fetch_item (key, hb_hash (key));
    यदि (item)
    {
      item->set_real (असत्य);
      population--;
    }
  }

  /* Has interface. */
  ध्रुव V& operator [] (K k) ध्रुव { प्रत्यागम get (k); }
  template <typename VV=V>
  तार्किक has (ध्रुव K &key, VV **vp = nullptr) ध्रुव
  {
    यदि (!वस्तुएँ) प्रत्यागम असत्य;
    प्रत्यागम has_with_hash (key, hb_hash (key), vp);
  }
  template <typename VV=V>
  तार्किक has_with_hash (ध्रुव K &key, uint32_t hash, VV **vp = nullptr) ध्रुव
  {
    यदि (!वस्तुएँ) प्रत्यागम असत्य;
    auto *item = fetch_item (key, hash);
    यदि (item)
    {
      यदि (vp) *vp = std::addressof (item->value);
      प्रत्यागम सत्य;
    }
    प्रत्यागम असत्य;
  }
  item_t *fetch_item (ध्रुव K &key, uint32_t hash) ध्रुव
  {
    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    unsigned पूर्णाङ्क i = hash % अभाज्य;
    unsigned step = 0;
    यावत् (वस्तुएँ[i].is_used ())
    {
      यदि ((std::is_integral<K>::value || वस्तुएँ[i].hash == hash) &&
	  वस्तुएँ[i] == key)
      {
	यदि (वस्तुएँ[i].is_real ())
	  प्रत्यागम &वस्तुएँ[i];
	अन्यथा
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
    यदि (!population && !occupancy) प्रत्यागम;

    प्रति (auto &_ : hb_iter (वस्तुएँ, size ()))
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
    + iter_वस्तुएँ ()
    | hb_reduce ([] (uint32_t h, ध्रुव item_t &_) { प्रत्यागम h ^ _.total_hash (); }, (uint32_t) 0u)
    ;
  }

  तार्किक is_equal (ध्रुव hb_hashmap_t &other) ध्रुव
  {
    यदि (population != other.population) प्रत्यागम असत्य;

    प्रति (auto pair : iter ())
      यदि (other.get (pair.first) != pair.second)
        प्रत्यागम असत्य;

    प्रत्यागम सत्य;
  }
  तार्किक operator == (ध्रुव hb_hashmap_t &other) ध्रुव { प्रत्यागम is_equal (other); }
  तार्किक operator != (ध्रुव hb_hashmap_t &other) ध्रुव { प्रत्यागम !is_equal (other); }

  unsigned पूर्णाङ्क get_population () ध्रुव { प्रत्यागम population; }

  शून्य update (ध्रुव hb_hashmap_t &other)
  {
    यदि (unlikely (!successful)) प्रत्यागम;

    hb_copy (other, *एतत्);
  }

  /*
   * Iterator
   */

  auto iter_वस्तुएँ () ध्रुव HB_AUTO_RETURN
  (
    + hb_iter (वस्तुएँ, एतत्->size ())
    | hb_filter (&item_t::is_real)
  )
  auto iter_ref () ध्रुव HB_AUTO_RETURN
  (
    + एतत्->iter_वस्तुएँ ()
    | hb_map (&item_t::get_pair_ref)
  )
  auto iter () ध्रुव HB_AUTO_RETURN
  (
    + एतत्->iter_वस्तुएँ ()
    | hb_map (&item_t::get_pair)
  )
  auto कुंजियाँ_ref () ध्रुव HB_AUTO_RETURN
  (
    + एतत्->iter_वस्तुएँ ()
    | hb_map (&item_t::get_कुंजी)
  )
  auto कुंजियाँ () ध्रुव HB_AUTO_RETURN
  (
    + एतत्->कुंजियाँ_ref ()
    | hb_map (hb_ridentity)
  )
  auto मूल्यावली_ref () ध्रुव HB_AUTO_RETURN
  (
    + एतत्->iter_वस्तुएँ ()
    | hb_map (&item_t::get_मूल्य)
  )
  auto values () ध्रुव HB_AUTO_RETURN
  (
    + एतत्->मूल्यावली_ref ()
    | hb_map (hb_ridentity)
  )

  /* C iterator. */
  तार्किक next (पूर्णाङ्क *idx,
	     K *key,
	     V *value) ध्रुव
  {
    unsigned i = (unsigned) (*idx + 1);

    unsigned गिनती = size ();
    यावत् (i < गिनती && !वस्तुएँ[i].is_real ())
      i++;

    यदि (i >= गिनती)
    {
      *idx = -1;
      प्रत्यागम असत्य;
    }

    *key = वस्तुएँ[i].key;
    *value = वस्तुएँ[i].value;

    *idx = (signed) i;
    प्रत्यागम सत्य;
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

  स्थिर unsigned पूर्णाङ्क अभाज्य_for (unsigned पूर्णाङ्क shift)
  {
    /* Following comment and table copied from glib. */
    /* Each table size has an associated prime modulo (the first prime
     * lower than the table size) used to find the initial bucket. Probing
     * then works modulo 2^n. The prime modulo is necessary to get a
     * good distribution with poor hash functions.
     */
    /* Not declaring static to make all kinds of compilers happy... */
    /*static*/ ध्रुव unsigned पूर्णाङ्क अभाज्य_mod [32] =
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

    यदि (unlikely (shift >= ARRAY_LENGTH (अभाज्य_mod)))
      प्रत्यागम अभाज्य_mod[ARRAY_LENGTH (अभाज्य_mod) - 1];

    प्रत्यागम अभाज्य_mod[shift];
  }
};

/*
 * hb_map_t
 */

संरचना hb_map_t : hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       सत्य>
{
  using hashmap = hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       सत्य>;

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

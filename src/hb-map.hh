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

बाह्य HB_INTERNAL अचर hb_codepoint_t minus_1;

ढाँचा <typename K, typename V,
	  तार्किक minus_one = असत्य>
संरचना hb_hashmap_t
{
  स्थिर constexpr तार्किक realloc_move = सत्य;

  hb_hashmap_t ()  { init (); }
  ~hb_hashmap_t () { fini (); }

  शून्य _copy (अचर hb_hashmap_t& o)
  {
    यदि (unlikely (!o.successful))
    {
      successful = असत्य;
      फर्काऊ;
    }
    यदि (unlikely (!o.mask)) फर्काऊ;

    यदि (hb_is_trivially_copy_assignable (item_t))
    {
      वस्तुहरू = (item_t *) hb_malloc2 ((o.mask + 1), परिमाण (item_t));
      यदि (unlikely (!वस्तुहरू))
      {
	successful = असत्य;
	फर्काऊ;
      }
      population = o.population;
      occupancy = o.occupancy;
      mask = o.mask;
      अविभाज्य = o.अविभाज्य;
      max_chain_length = o.max_chain_length;
      स_नकल (वस्तुहरू, o.वस्तुहरू, परिमाण (item_t) * (mask + 1));
      फर्काऊ;
    }

    alloc (o.population); hb_copy (o, *एतत्);
  }

  hb_hashmap_t (अचर hb_hashmap_t& o) : hb_hashmap_t () { _copy (o); }
  hb_hashmap_t& चालक= (अचर hb_hashmap_t& o)
  {
    reset ();
    यदि (!वस्तुहरू) { _copy (o); फर्काऊ *एतत्; }
    alloc (o.population); hb_copy (o, *एतत्); फर्काऊ *एतत्;
  }

  hb_hashmap_t (hb_hashmap_t&& o)  noexcept : hb_hashmap_t () { hb_swap (*एतत्, o); }
  hb_hashmap_t& चालक= (hb_hashmap_t&& o)   noexcept { hb_swap (*एतत्, o); फर्काऊ *एतत्; }

  hb_hashmap_t (std::initializer_list<hb_pair_t<K, V>> lst) : hb_hashmap_t ()
  {
    लागि (स्वतः&& item : lst)
      set (item.first, item.second);
  }
  ढाँचा <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_hashmap_t (अचर Iterable &o) : hb_hashmap_t ()
  {
    स्वतः iter = hb_iter (o);
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
    K& get_साँचो () { फर्काऊ key; }
    V& get_मान () { फर्काऊ value; }

    तार्किक is_used () अचर { फर्काऊ is_used_; }
    शून्य set_used (तार्किक is_used) { is_used_ = is_used; }
    शून्य set_real (तार्किक is_real) { is_real_ = is_real; }
    तार्किक is_real () अचर { फर्काऊ is_real_; }

    ढाँचा <तार्किक v = minus_one,
	      hb_enable_if (v == असत्य)>
    स्थिर अन्तःस्थ अचर V& default_मान () { फर्काऊ Null(V); };
    ढाँचा <तार्किक v = minus_one,
	      hb_enable_if (v == सत्य)>
    स्थिर अन्तःस्थ अचर V& default_मान ()
    {
      static_assert (hb_is_same (V, hb_codepoint_t), "");
      फर्काऊ minus_1;
    };

    तार्किक चालक == (अचर K &o) अचर { फर्काऊ hb_deref (key) == hb_deref (o); }
    तार्किक चालक == (अचर item_t &o) अचर { फर्काऊ *एतत् == o.key; }
    hb_pair_t<K, V> get_pair() अचर { फर्काऊ hb_pair_t<K, V> (key, value); }
    hb_pair_t<अचर K &, V &> get_pair_ref() { फर्काऊ hb_pair_t<अचर K &, V &> (key, value); }

    uint32_t total_hash () अचर
    { फर्काऊ (hash * 31u) + hb_hash (value); }

    स्थिर constexpr तार्किक is_trivially_constructible = (hb_is_trivially_constructible(K) && hb_is_trivially_constructible(V));
  };

  hb_object_header_t header;
  तार्किक successful; /* Allocations successful */
  अचिन्हित लघु max_chain_length;
  अचिन्हित पूर्णाङ्क population; /* Not including tombstones. */
  अचिन्हित पूर्णाङ्क occupancy; /* Including tombstones. */
  अचिन्हित पूर्णाङ्क mask;
  अचिन्हित पूर्णाङ्क अविभाज्य;
  item_t *वस्तुहरू;

  मित्र शून्य swap (hb_hashmap_t& a, hb_hashmap_t& b) noexcept
  {
    यदि (unlikely (!a.successful || !b.successful))
      फर्काऊ;
    hb_swap (a.max_chain_length, b.max_chain_length);
    hb_swap (a.population, b.population);
    hb_swap (a.occupancy, b.occupancy);
    hb_swap (a.mask, b.mask);
    hb_swap (a.अविभाज्य, b.अविभाज्य);
    hb_swap (a.वस्तुहरू, b.वस्तुहरू);
  }
  शून्य init ()
  {
    hb_object_init (एतत्);

    successful = सत्य;
    max_chain_length = 0;
    population = occupancy = 0;
    mask = 0;
    अविभाज्य = 0;
    वस्तुहरू = nullptr;
  }
  शून्य fini ()
  {
    hb_object_fini (एतत्);

    यदि (likely (वस्तुहरू))
    {
      अचिन्हित size = mask + 1;
      लागि (अचिन्हित i = 0; i < size; i++)
	वस्तुहरू[i].~item_t ();
      hb_free (वस्तुहरू);
      वस्तुहरू = nullptr;
    }
    population = occupancy = 0;
  }

  hb_hashmap_t& reset ()
  {
    successful = सत्य;
    clear ();
    फर्काऊ *एतत्;
  }

  तार्किक in_error () अचर { फर्काऊ !successful; }

  तार्किक alloc (अचिन्हित new_population = 0)
  {
    यदि (unlikely (!successful)) फर्काऊ असत्य;
    यदि (unlikely (population > 0x3FFFFFFFu || new_population > 0x3FFFFFFFu))
    {
      // Population sizes >0x3FFFFFFF will result in power
      // being larger than 31, which in turn leads to a new size
      // that overflows u32.
      successful = असत्य;
      फर्काऊ असत्य;
    }

    यदि (new_population != 0 && (new_population + new_population / 2) < mask) फर्काऊ सत्य;

    अचिन्हित पूर्णाङ्क power = hb_bit_storage (hb_max (hb_max ((अचिन्हित) population, new_population) * 2, 4u));
    अचिन्हित पूर्णाङ्क new_size = 1u << power;
    item_t *new_वस्तुहरू = (item_t *) hb_malloc2 ((माप_प्रकार) new_size, परिमाण (item_t));
    यदि (unlikely (!new_वस्तुहरू))
    {
      successful = असत्य;
      फर्काऊ असत्य;
    }
    यदि (!item_t::is_trivially_constructible)
      लागि (स्वतः &_ : hb_iter (new_वस्तुहरू, new_size))
	नूतन (&_) item_t ();
    अन्यथा
      hb_memset (new_वस्तुहरू, 0, (माप_प्रकार) new_size * परिमाण (item_t));

    अचिन्हित पूर्णाङ्क old_size = size ();
    item_t *old_वस्तुहरू = वस्तुहरू;

    /* Switch to new, empty, array. */
    population = occupancy = 0;
    mask = new_size - 1;
    अविभाज्य = अविभाज्य_for (power);
    max_chain_length = power * 2;
    वस्तुहरू = new_वस्तुहरू;

    /* Insert back old items. */
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < old_size; i++)
    {
      यदि (old_वस्तुहरू[i].is_real ())
      {
	set_with_hash (std::move (old_वस्तुहरू[i].key),
		       old_वस्तुहरू[i].hash,
		       std::move (old_वस्तुहरू[i].value));
      }
    }
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < old_size; i++)
      old_वस्तुहरू[i].~item_t ();

    hb_free (old_वस्तुहरू);

    फर्काऊ सत्य;
  }

  ढाँचा <typename T>
  तार्किक in_storage (अचर T &v) अचर
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    फर्काऊ वस्तुहरू &&
	   (uintptr_t) वस्तुहरू <= p && p < (uintptr_t) (वस्तुहरू + size ());
  }

  ढाँचा <typename KK, typename VV>
  तार्किक set_with_hash (KK&& key, uint32_t hash, VV&& value, तार्किक overwrite = सत्य)
  {
    यदि (unlikely (!successful)) फर्काऊ असत्य;
    यदि (unlikely ((occupancy + occupancy / 2) >= mask))
    {
      /* key or value may live in our own storage, eg. m.set (k, m.get (j)).
       * alloc() frees that storage, so copy them out first. */
      यदि (unlikely (in_storage (key) || in_storage (value)))
      {
	K साँचो_copy;
	साँचो_copy = std::forward<KK> (key);
	V मान_copy;
	मान_copy = std::forward<VV> (value);
	फर्काऊ set_with_hash (std::move (साँचो_copy), hash, std::move (मान_copy), overwrite);
      }
      यदि (unlikely (!alloc ())) फर्काऊ असत्य;
    }

    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    अचिन्हित पूर्णाङ्क tombstone = (अचिन्हित पूर्णाङ्क) -1;
    अचिन्हित पूर्णाङ्क i = hash % अविभाज्य;
    अचिन्हित length = 0;
    अचिन्हित step = 0;
    जबसम्म (वस्तुहरू[i].is_used ())
    {
      यदि ((std::is_integral<K>::value || वस्तुहरू[i].hash == hash) &&
	  वस्तुहरू[i] == key)
      {
        यदि (!overwrite)
	  फर्काऊ असत्य;
        अन्यथा
	  रोक;
      }
      यदि (!वस्तुहरू[i].is_real () && tombstone == (अचिन्हित) -1)
        tombstone = i;
      i = (i + ++step) & mask;
      length++;
    }

    item_t &item = वस्तुहरू[tombstone == (अचिन्हित) -1 ? i : tombstone];

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

    फर्काऊ सत्य;
  }

  ढाँचा <typename VV>
  तार्किक set (अचर K &key, VV&& value, तार्किक overwrite = सत्य) { फर्काऊ set_with_hash (key, hb_hash (key), std::forward<VV> (value), overwrite); }
  ढाँचा <typename VV>
  तार्किक set (K &&key, VV&& value, तार्किक overwrite = सत्य)
  {
    uint32_t hash = hb_hash (key);
    फर्काऊ set_with_hash (std::move (key), hash, std::forward<VV> (value), overwrite);
  }
  तार्किक add (अचर K &key)
  {
    uint32_t hash = hb_hash (key);
    फर्काऊ set_with_hash (key, hash, item_t::default_मान ());
  }

  अचर V& get_with_hash (अचर K &key, uint32_t hash) अचर
  {
    यदि (!वस्तुहरू) फर्काऊ item_t::default_मान ();
    स्वतः *item = fetch_item (key, hash);
    यदि (item)
      फर्काऊ item->value;
    फर्काऊ item_t::default_मान ();
  }
  अचर V& get (अचर K &key) अचर
  {
    यदि (!वस्तुहरू) फर्काऊ item_t::default_मान ();
    फर्काऊ get_with_hash (key, hb_hash (key));
  }

  शून्य del (अचर K &key)
  {
    यदि (!वस्तुहरू) फर्काऊ;
    स्वतः *item = fetch_item (key, hb_hash (key));
    यदि (item)
    {
      item->set_real (असत्य);
      population--;
    }
  }

  /* Has interface. */
  अचर V& चालक [] (K k) अचर { फर्काऊ get (k); }
  ढाँचा <typename VV=V>
  तार्किक has (अचर K &key, VV **vp = nullptr) अचर
  {
    यदि (!वस्तुहरू) फर्काऊ असत्य;
    फर्काऊ has_with_hash (key, hb_hash (key), vp);
  }
  ढाँचा <typename VV=V>
  तार्किक has_with_hash (अचर K &key, uint32_t hash, VV **vp = nullptr) अचर
  {
    यदि (!वस्तुहरू) फर्काऊ असत्य;
    स्वतः *item = fetch_item (key, hash);
    यदि (item)
    {
      यदि (vp) *vp = std::addressof (item->value);
      फर्काऊ सत्य;
    }
    फर्काऊ असत्य;
  }
  item_t *fetch_item (अचर K &key, uint32_t hash) अचर
  {
    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    अचिन्हित पूर्णाङ्क i = hash % अविभाज्य;
    अचिन्हित step = 0;
    जबसम्म (वस्तुहरू[i].is_used ())
    {
      यदि ((std::is_integral<K>::value || वस्तुहरू[i].hash == hash) &&
	  वस्तुहरू[i] == key)
      {
	यदि (वस्तुहरू[i].is_real ())
	  फर्काऊ &वस्तुहरू[i];
	अन्यथा
	  फर्काऊ nullptr;
      }
      i = (i + ++step) & mask;
    }
    फर्काऊ nullptr;
  }
  /* Projection. */
  अचर V& चालक () (K k) अचर { फर्काऊ get (k); }

  अचिन्हित size () अचर { फर्काऊ mask ? mask + 1 : 0; }

  शून्य clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (zero-initialized) from any writes.  Any non-empty hashmap
     * is a real heap instance with writable items, so clearing
     * under !successful is safe. */
    यदि (!population && !occupancy) फर्काऊ;

    लागि (स्वतः &_ : hb_iter (वस्तुहरू, size ()))
    {
      /* Reconstruct items. */
      _.~item_t ();
      नूतन (&_) item_t ();
    }

    population = occupancy = 0;
  }

  तार्किक is_empty () अचर { फर्काऊ population == 0; }
  explicit चालक तार्किक () अचर { फर्काऊ !is_empty (); }

  uint32_t hash () अचर
  {
    फर्काऊ
    + iter_वस्तुहरू ()
    | hb_reduce ([] (uint32_t h, अचर item_t &_) { फर्काऊ h ^ _.total_hash (); }, (uint32_t) 0u)
    ;
  }

  तार्किक is_equal (अचर hb_hashmap_t &other) अचर
  {
    यदि (population != other.population) फर्काऊ असत्य;

    लागि (स्वतः pair : iter ())
      यदि (other.get (pair.first) != pair.second)
        फर्काऊ असत्य;

    फर्काऊ सत्य;
  }
  तार्किक चालक == (अचर hb_hashmap_t &other) अचर { फर्काऊ is_equal (other); }
  तार्किक चालक != (अचर hb_hashmap_t &other) अचर { फर्काऊ !is_equal (other); }

  अचिन्हित पूर्णाङ्क get_population () अचर { फर्काऊ population; }

  शून्य update (अचर hb_hashmap_t &other)
  {
    यदि (unlikely (!successful)) फर्काऊ;

    hb_copy (other, *एतत्);
  }

  /*
   * Iterator
   */

  स्वतः iter_वस्तुहरू () अचर HB_AUTO_RETURN
  (
    + hb_iter (वस्तुहरू, एतत्->size ())
    | hb_filter (&item_t::is_real)
  )
  स्वतः iter_ref () अचर HB_AUTO_RETURN
  (
    + एतत्->iter_वस्तुहरू ()
    | hb_map (&item_t::get_pair_ref)
  )
  स्वतः iter () अचर HB_AUTO_RETURN
  (
    + एतत्->iter_वस्तुहरू ()
    | hb_map (&item_t::get_pair)
  )
  स्वतः साँचोहरू_ref () अचर HB_AUTO_RETURN
  (
    + एतत्->iter_वस्तुहरू ()
    | hb_map (&item_t::get_साँचो)
  )
  स्वतः साँचोहरू () अचर HB_AUTO_RETURN
  (
    + एतत्->साँचोहरू_ref ()
    | hb_map (hb_ridentity)
  )
  स्वतः मानहरू_ref () अचर HB_AUTO_RETURN
  (
    + एतत्->iter_वस्तुहरू ()
    | hb_map (&item_t::get_मान)
  )
  स्वतः values () अचर HB_AUTO_RETURN
  (
    + एतत्->मानहरू_ref ()
    | hb_map (hb_ridentity)
  )

  /* C iterator. */
  तार्किक next (पूर्णाङ्क *idx,
	     K *key,
	     V *value) अचर
  {
    अचिन्हित i = (अचिन्हित) (*idx + 1);

    अचिन्हित गन्ती = size ();
    जबसम्म (i < गन्ती && !वस्तुहरू[i].is_real ())
      i++;

    यदि (i >= गन्ती)
    {
      *idx = -1;
      फर्काऊ असत्य;
    }

    *key = वस्तुहरू[i].key;
    *value = वस्तुहरू[i].value;

    *idx = (चिन्हित) i;
    फर्काऊ सत्य;
  }

  /* Sink interface. */
  hb_hashmap_t& चालक << (अचर hb_pair_t<K, V>& v)
  { set (v.first, v.second); फर्काऊ *एतत्; }
  ढाँचा <typename V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (V2))>
  hb_hashmap_t& चालक << (अचर hb_pair_t<K, V&&>& v)
  { set (v.first, std::move (v.second)); फर्काऊ *एतत्; }
  ढाँचा <typename K2 = K,
	    hb_enable_if (!hb_is_trivially_copyable (K2))>
  hb_hashmap_t& चालक << (अचर hb_pair_t<K&&, V>& v)
  { set (std::move (v.first), v.second); फर्काऊ *एतत्; }
  ढाँचा <typename K2 = K, typename V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (K2) &&
			  !hb_is_trivially_copyable (V2))>
  hb_hashmap_t& चालक << (अचर hb_pair_t<K&&, V&&>& v)
  { set (std::move (v.first), std::move (v.second)); फर्काऊ *एतत्; }

  स्थिर अचिन्हित पूर्णाङ्क अविभाज्य_for (अचिन्हित पूर्णाङ्क shift)
  {
    /* Following comment and table copied from glib. */
    /* Each table size has an associated prime modulo (the first prime
     * lower than the table size) used to find the initial bucket. Probing
     * then works modulo 2^n. The prime modulo is necessary to get a
     * good distribution with poor hash functions.
     */
    /* Not declaring static to make all kinds of compilers happy... */
    /*static*/ अचर अचिन्हित पूर्णाङ्क अविभाज्य_mod [32] =
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

    यदि (unlikely (shift >= ARRAY_LENGTH (अविभाज्य_mod)))
      फर्काऊ अविभाज्य_mod[ARRAY_LENGTH (अविभाज्य_mod) - 1];

    फर्काऊ अविभाज्य_mod[shift];
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
  hb_map_t (अचर hb_map_t &o) : hashmap ((hashmap &) o) {}
  hb_map_t (hb_map_t &&o)  noexcept : hashmap (std::move ((hashmap &) o)) {}
  hb_map_t& चालक= (अचर hb_map_t&) = स्वभाव;
  hb_map_t& चालक= (hb_map_t&&) = स्वभाव;
  hb_map_t (std::initializer_list<hb_codepoint_pair_t> lst) : hashmap (lst) {}
  ढाँचा <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_map_t (अचर Iterable &o) : hashmap (o) {}
};


#endif /* HB_MAP_HH */

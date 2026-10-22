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

बाह्य HB_INTERNAL स्थिरांक hb_codepoint_t minus_1;

ढाँचा <typename K, typename V,
	  तार्किक minus_one = असत्य>
संरचना hb_hashmap_t
{
  स्थिर constexpr तार्किक realloc_move = सत्य;

  hb_hashmap_t ()  { init (); }
  ~hb_hashmap_t () { fini (); }

  शून्य _copy (स्थिरांक hb_hashmap_t& o)
  {
    जर (unlikely (!o.successful))
    {
      successful = असत्य;
      परत;
    }
    जर (unlikely (!o.mask)) परत;

    जर (hb_is_trivially_copy_assignable (item_t))
    {
      वस्तुसंच = (item_t *) hb_malloc2 ((o.mask + 1), आकार (item_t));
      जर (unlikely (!वस्तुसंच))
      {
	successful = असत्य;
	परत;
      }
      population = o.population;
      occupancy = o.occupancy;
      mask = o.mask;
      मूळसंख्या = o.मूळसंख्या;
      max_chain_length = o.max_chain_length;
      स_नकल (वस्तुसंच, o.वस्तुसंच, आकार (item_t) * (mask + 1));
      परत;
    }

    alloc (o.population); hb_copy (o, *हे);
  }

  hb_hashmap_t (स्थिरांक hb_hashmap_t& o) : hb_hashmap_t () { _copy (o); }
  hb_hashmap_t& चालक= (स्थिरांक hb_hashmap_t& o)
  {
    reset ();
    जर (!वस्तुसंच) { _copy (o); परत *हे; }
    alloc (o.population); hb_copy (o, *हे); परत *हे;
  }

  hb_hashmap_t (hb_hashmap_t&& o)  noexcept : hb_hashmap_t () { hb_swap (*हे, o); }
  hb_hashmap_t& चालक= (hb_hashmap_t&& o)   noexcept { hb_swap (*हे, o); परत *हे; }

  hb_hashmap_t (std::initializer_list<hb_pair_t<K, V>> lst) : hb_hashmap_t ()
  {
    साठी (स्वतः&& item : lst)
      set (item.first, item.second);
  }
  ढाँचा <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_hashmap_t (स्थिरांक Iterable &o) : hb_hashmap_t ()
  {
    स्वतः iter = hb_iter (o);
    जर (iter.is_random_access_iterator || iter.has_fast_len)
      alloc (hb_len (iter));
    hb_copy (iter, *हे);
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
    K& get_किल्ली () { परत key; }
    V& get_मूल्य () { परत value; }

    तार्किक is_used () स्थिरांक { परत is_used_; }
    शून्य set_used (तार्किक is_used) { is_used_ = is_used; }
    शून्य set_real (तार्किक is_real) { is_real_ = is_real; }
    तार्किक is_real () स्थिरांक { परत is_real_; }

    ढाँचा <तार्किक v = minus_one,
	      hb_enable_if (v == असत्य)>
    स्थिर ओळीत स्थिरांक V& default_मूल्य () { परत Null(V); };
    ढाँचा <तार्किक v = minus_one,
	      hb_enable_if (v == सत्य)>
    स्थिर ओळीत स्थिरांक V& default_मूल्य ()
    {
      static_assert (hb_is_same (V, hb_codepoint_t), "");
      परत minus_1;
    };

    तार्किक चालक == (स्थिरांक K &o) स्थिरांक { परत hb_deref (key) == hb_deref (o); }
    तार्किक चालक == (स्थिरांक item_t &o) स्थिरांक { परत *हे == o.key; }
    hb_pair_t<K, V> get_pair() स्थिरांक { परत hb_pair_t<K, V> (key, value); }
    hb_pair_t<स्थिरांक K &, V &> get_pair_ref() { परत hb_pair_t<स्थिरांक K &, V &> (key, value); }

    uint32_t total_hash () स्थिरांक
    { परत (hash * 31u) + hb_hash (value); }

    स्थिर constexpr तार्किक is_trivially_constructible = (hb_is_trivially_constructible(K) && hb_is_trivially_constructible(V));
  };

  hb_object_header_t header;
  तार्किक successful; /* Allocations successful */
  अचिन्हित लघु max_chain_length;
  अचिन्हित पूर्णांक population; /* Not including tombstones. */
  अचिन्हित पूर्णांक occupancy; /* Including tombstones. */
  अचिन्हित पूर्णांक mask;
  अचिन्हित पूर्णांक मूळसंख्या;
  item_t *वस्तुसंच;

  मित्र शून्य swap (hb_hashmap_t& a, hb_hashmap_t& b) noexcept
  {
    जर (unlikely (!a.successful || !b.successful))
      परत;
    hb_swap (a.max_chain_length, b.max_chain_length);
    hb_swap (a.population, b.population);
    hb_swap (a.occupancy, b.occupancy);
    hb_swap (a.mask, b.mask);
    hb_swap (a.मूळसंख्या, b.मूळसंख्या);
    hb_swap (a.वस्तुसंच, b.वस्तुसंच);
  }
  शून्य init ()
  {
    hb_object_init (हे);

    successful = सत्य;
    max_chain_length = 0;
    population = occupancy = 0;
    mask = 0;
    मूळसंख्या = 0;
    वस्तुसंच = nullptr;
  }
  शून्य fini ()
  {
    hb_object_fini (हे);

    जर (likely (वस्तुसंच))
    {
      अचिन्हित size = mask + 1;
      साठी (अचिन्हित i = 0; i < size; i++)
	वस्तुसंच[i].~item_t ();
      hb_free (वस्तुसंच);
      वस्तुसंच = nullptr;
    }
    population = occupancy = 0;
  }

  hb_hashmap_t& reset ()
  {
    successful = सत्य;
    clear ();
    परत *हे;
  }

  तार्किक in_error () स्थिरांक { परत !successful; }

  तार्किक alloc (अचिन्हित new_population = 0)
  {
    जर (unlikely (!successful)) परत असत्य;
    जर (unlikely (population > 0x3FFFFFFFu || new_population > 0x3FFFFFFFu))
    {
      // Population sizes >0x3FFFFFFF will result in power
      // being larger than 31, which in turn leads to a new size
      // that overflows u32.
      successful = असत्य;
      परत असत्य;
    }

    जर (new_population != 0 && (new_population + new_population / 2) < mask) परत सत्य;

    अचिन्हित पूर्णांक power = hb_bit_storage (hb_max (hb_max ((अचिन्हित) population, new_population) * 2, 4u));
    अचिन्हित पूर्णांक new_size = 1u << power;
    item_t *new_वस्तुसंच = (item_t *) hb_malloc2 ((माप_प्रकार) new_size, आकार (item_t));
    जर (unlikely (!new_वस्तुसंच))
    {
      successful = असत्य;
      परत असत्य;
    }
    जर (!item_t::is_trivially_constructible)
      साठी (स्वतः &_ : hb_iter (new_वस्तुसंच, new_size))
	नवीन (&_) item_t ();
    अन्यथा
      hb_memset (new_वस्तुसंच, 0, (माप_प्रकार) new_size * आकार (item_t));

    अचिन्हित पूर्णांक old_size = size ();
    item_t *old_वस्तुसंच = वस्तुसंच;

    /* Switch to new, empty, array. */
    population = occupancy = 0;
    mask = new_size - 1;
    मूळसंख्या = मूळसंख्या_for (power);
    max_chain_length = power * 2;
    वस्तुसंच = new_वस्तुसंच;

    /* Insert back old items. */
    साठी (अचिन्हित पूर्णांक i = 0; i < old_size; i++)
    {
      जर (old_वस्तुसंच[i].is_real ())
      {
	set_with_hash (std::move (old_वस्तुसंच[i].key),
		       old_वस्तुसंच[i].hash,
		       std::move (old_वस्तुसंच[i].value));
      }
    }
    साठी (अचिन्हित पूर्णांक i = 0; i < old_size; i++)
      old_वस्तुसंच[i].~item_t ();

    hb_free (old_वस्तुसंच);

    परत सत्य;
  }

  ढाँचा <typename T>
  तार्किक in_storage (स्थिरांक T &v) स्थिरांक
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    परत वस्तुसंच &&
	   (uintptr_t) वस्तुसंच <= p && p < (uintptr_t) (वस्तुसंच + size ());
  }

  ढाँचा <typename KK, typename VV>
  तार्किक set_with_hash (KK&& key, uint32_t hash, VV&& value, तार्किक overwrite = सत्य)
  {
    जर (unlikely (!successful)) परत असत्य;
    जर (unlikely ((occupancy + occupancy / 2) >= mask))
    {
      /* key or value may live in our own storage, eg. m.set (k, m.get (j)).
       * alloc() frees that storage, so copy them out first. */
      जर (unlikely (in_storage (key) || in_storage (value)))
      {
	K किल्ली_copy;
	किल्ली_copy = std::forward<KK> (key);
	V मूल्य_copy;
	मूल्य_copy = std::forward<VV> (value);
	परत set_with_hash (std::move (किल्ली_copy), hash, std::move (मूल्य_copy), overwrite);
      }
      जर (unlikely (!alloc ())) परत असत्य;
    }

    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    अचिन्हित पूर्णांक tombstone = (अचिन्हित पूर्णांक) -1;
    अचिन्हित पूर्णांक i = hash % मूळसंख्या;
    अचिन्हित length = 0;
    अचिन्हित step = 0;
    पर्यंत (वस्तुसंच[i].is_used ())
    {
      जर ((std::is_integral<K>::value || वस्तुसंच[i].hash == hash) &&
	  वस्तुसंच[i] == key)
      {
        जर (!overwrite)
	  परत असत्य;
        अन्यथा
	  थांबा;
      }
      जर (!वस्तुसंच[i].is_real () && tombstone == (अचिन्हित) -1)
        tombstone = i;
      i = (i + ++step) & mask;
      length++;
    }

    item_t &item = वस्तुसंच[tombstone == (अचिन्हित) -1 ? i : tombstone];

    जर (item.is_used ())
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

    जर (unlikely (length > max_chain_length) && occupancy > mask / 8)
      alloc (mask - 8); // This ensures we jump to next larger size

    परत सत्य;
  }

  ढाँचा <typename VV>
  तार्किक set (स्थिरांक K &key, VV&& value, तार्किक overwrite = सत्य) { परत set_with_hash (key, hb_hash (key), std::forward<VV> (value), overwrite); }
  ढाँचा <typename VV>
  तार्किक set (K &&key, VV&& value, तार्किक overwrite = सत्य)
  {
    uint32_t hash = hb_hash (key);
    परत set_with_hash (std::move (key), hash, std::forward<VV> (value), overwrite);
  }
  तार्किक add (स्थिरांक K &key)
  {
    uint32_t hash = hb_hash (key);
    परत set_with_hash (key, hash, item_t::default_मूल्य ());
  }

  स्थिरांक V& get_with_hash (स्थिरांक K &key, uint32_t hash) स्थिरांक
  {
    जर (!वस्तुसंच) परत item_t::default_मूल्य ();
    स्वतः *item = fetch_item (key, hash);
    जर (item)
      परत item->value;
    परत item_t::default_मूल्य ();
  }
  स्थिरांक V& get (स्थिरांक K &key) स्थिरांक
  {
    जर (!वस्तुसंच) परत item_t::default_मूल्य ();
    परत get_with_hash (key, hb_hash (key));
  }

  शून्य del (स्थिरांक K &key)
  {
    जर (!वस्तुसंच) परत;
    स्वतः *item = fetch_item (key, hb_hash (key));
    जर (item)
    {
      item->set_real (असत्य);
      population--;
    }
  }

  /* Has interface. */
  स्थिरांक V& चालक [] (K k) स्थिरांक { परत get (k); }
  ढाँचा <typename VV=V>
  तार्किक has (स्थिरांक K &key, VV **vp = nullptr) स्थिरांक
  {
    जर (!वस्तुसंच) परत असत्य;
    परत has_with_hash (key, hb_hash (key), vp);
  }
  ढाँचा <typename VV=V>
  तार्किक has_with_hash (स्थिरांक K &key, uint32_t hash, VV **vp = nullptr) स्थिरांक
  {
    जर (!वस्तुसंच) परत असत्य;
    स्वतः *item = fetch_item (key, hash);
    जर (item)
    {
      जर (vp) *vp = std::addressof (item->value);
      परत सत्य;
    }
    परत असत्य;
  }
  item_t *fetch_item (स्थिरांक K &key, uint32_t hash) स्थिरांक
  {
    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    अचिन्हित पूर्णांक i = hash % मूळसंख्या;
    अचिन्हित step = 0;
    पर्यंत (वस्तुसंच[i].is_used ())
    {
      जर ((std::is_integral<K>::value || वस्तुसंच[i].hash == hash) &&
	  वस्तुसंच[i] == key)
      {
	जर (वस्तुसंच[i].is_real ())
	  परत &वस्तुसंच[i];
	अन्यथा
	  परत nullptr;
      }
      i = (i + ++step) & mask;
    }
    परत nullptr;
  }
  /* Projection. */
  स्थिरांक V& चालक () (K k) स्थिरांक { परत get (k); }

  अचिन्हित size () स्थिरांक { परत mask ? mask + 1 : 0; }

  शून्य clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (zero-initialized) from any writes.  Any non-empty hashmap
     * is a real heap instance with writable items, so clearing
     * under !successful is safe. */
    जर (!population && !occupancy) परत;

    साठी (स्वतः &_ : hb_iter (वस्तुसंच, size ()))
    {
      /* Reconstruct items. */
      _.~item_t ();
      नवीन (&_) item_t ();
    }

    population = occupancy = 0;
  }

  तार्किक is_empty () स्थिरांक { परत population == 0; }
  explicit चालक तार्किक () स्थिरांक { परत !is_empty (); }

  uint32_t hash () स्थिरांक
  {
    परत
    + iter_वस्तुसंच ()
    | hb_reduce ([] (uint32_t h, स्थिरांक item_t &_) { परत h ^ _.total_hash (); }, (uint32_t) 0u)
    ;
  }

  तार्किक is_equal (स्थिरांक hb_hashmap_t &other) स्थिरांक
  {
    जर (population != other.population) परत असत्य;

    साठी (स्वतः pair : iter ())
      जर (other.get (pair.first) != pair.second)
        परत असत्य;

    परत सत्य;
  }
  तार्किक चालक == (स्थिरांक hb_hashmap_t &other) स्थिरांक { परत is_equal (other); }
  तार्किक चालक != (स्थिरांक hb_hashmap_t &other) स्थिरांक { परत !is_equal (other); }

  अचिन्हित पूर्णांक get_population () स्थिरांक { परत population; }

  शून्य update (स्थिरांक hb_hashmap_t &other)
  {
    जर (unlikely (!successful)) परत;

    hb_copy (other, *हे);
  }

  /*
   * Iterator
   */

  स्वतः iter_वस्तुसंच () स्थिरांक HB_AUTO_RETURN
  (
    + hb_iter (वस्तुसंच, हे->size ())
    | hb_filter (&item_t::is_real)
  )
  स्वतः iter_ref () स्थिरांक HB_AUTO_RETURN
  (
    + हे->iter_वस्तुसंच ()
    | hb_map (&item_t::get_pair_ref)
  )
  स्वतः iter () स्थिरांक HB_AUTO_RETURN
  (
    + हे->iter_वस्तुसंच ()
    | hb_map (&item_t::get_pair)
  )
  स्वतः किल्ल्या_ref () स्थिरांक HB_AUTO_RETURN
  (
    + हे->iter_वस्तुसंच ()
    | hb_map (&item_t::get_किल्ली)
  )
  स्वतः किल्ल्या () स्थिरांक HB_AUTO_RETURN
  (
    + हे->किल्ल्या_ref ()
    | hb_map (hb_ridentity)
  )
  स्वतः मूल्ये_ref () स्थिरांक HB_AUTO_RETURN
  (
    + हे->iter_वस्तुसंच ()
    | hb_map (&item_t::get_मूल्य)
  )
  स्वतः values () स्थिरांक HB_AUTO_RETURN
  (
    + हे->मूल्ये_ref ()
    | hb_map (hb_ridentity)
  )

  /* C iterator. */
  तार्किक next (पूर्णांक *idx,
	     K *key,
	     V *value) स्थिरांक
  {
    अचिन्हित i = (अचिन्हित) (*idx + 1);

    अचिन्हित मोजणी = size ();
    पर्यंत (i < मोजणी && !वस्तुसंच[i].is_real ())
      i++;

    जर (i >= मोजणी)
    {
      *idx = -1;
      परत असत्य;
    }

    *key = वस्तुसंच[i].key;
    *value = वस्तुसंच[i].value;

    *idx = (चिन्हित) i;
    परत सत्य;
  }

  /* Sink interface. */
  hb_hashmap_t& चालक << (स्थिरांक hb_pair_t<K, V>& v)
  { set (v.first, v.second); परत *हे; }
  ढाँचा <typename V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (V2))>
  hb_hashmap_t& चालक << (स्थिरांक hb_pair_t<K, V&&>& v)
  { set (v.first, std::move (v.second)); परत *हे; }
  ढाँचा <typename K2 = K,
	    hb_enable_if (!hb_is_trivially_copyable (K2))>
  hb_hashmap_t& चालक << (स्थिरांक hb_pair_t<K&&, V>& v)
  { set (std::move (v.first), v.second); परत *हे; }
  ढाँचा <typename K2 = K, typename V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (K2) &&
			  !hb_is_trivially_copyable (V2))>
  hb_hashmap_t& चालक << (स्थिरांक hb_pair_t<K&&, V&&>& v)
  { set (std::move (v.first), std::move (v.second)); परत *हे; }

  स्थिर अचिन्हित पूर्णांक मूळसंख्या_for (अचिन्हित पूर्णांक shift)
  {
    /* Following comment and table copied from glib. */
    /* Each table size has an associated prime modulo (the first prime
     * lower than the table size) used to find the initial bucket. Probing
     * then works modulo 2^n. The prime modulo is necessary to get a
     * good distribution with poor hash functions.
     */
    /* Not declaring static to make all kinds of compilers happy... */
    /*static*/ स्थिरांक अचिन्हित पूर्णांक मूळसंख्या_mod [32] =
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

    जर (unlikely (shift >= ARRAY_LENGTH (मूळसंख्या_mod)))
      परत मूळसंख्या_mod[ARRAY_LENGTH (मूळसंख्या_mod) - 1];

    परत मूळसंख्या_mod[shift];
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

  ~hb_map_t () = पूर्वनिर्धारित;
  hb_map_t () : hashmap () {}
  hb_map_t (स्थिरांक hb_map_t &o) : hashmap ((hashmap &) o) {}
  hb_map_t (hb_map_t &&o)  noexcept : hashmap (std::move ((hashmap &) o)) {}
  hb_map_t& चालक= (स्थिरांक hb_map_t&) = पूर्वनिर्धारित;
  hb_map_t& चालक= (hb_map_t&&) = पूर्वनिर्धारित;
  hb_map_t (std::initializer_list<hb_codepoint_pair_t> lst) : hashmap (lst) {}
  ढाँचा <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_map_t (स्थिरांक Iterable &o) : hashmap (o) {}
};


#endif /* HB_MAP_HH */

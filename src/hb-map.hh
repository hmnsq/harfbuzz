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

#अगर_अघोषित HB_MAP_HH
#घोषणा HB_MAP_HH

#समावेश "hb.hh"

#समावेश "hb-set.hh"


/*
 * hb_hashmap_t
 */

बाह्य HB_INTERNAL स्थिर hb_codepoint_t minus_1;

ढाँचा <typename K, typename V,
	  bool minus_one = false>
काष्ठा hb_hashmap_t
{
  अटल constexpr bool realloc_move = true;

  hb_hashmap_t ()  { init (); }
  ~hb_hashmap_t () { fini (); }

  व्योम _copy (स्थिर hb_hashmap_t& o)
  {
    अगर (unlikely (!o.successful))
    {
      successful = false;
      वापस;
    }
    अगर (unlikely (!o.mask)) वापस;

    अगर (hb_is_trivially_copy_assignable (item_t))
    {
      वस्तुएँ = (item_t *) hb_malloc2 ((o.mask + 1), माप (item_t));
      अगर (unlikely (!वस्तुएँ))
      {
	successful = false;
	वापस;
      }
      population = o.population;
      occupancy = o.occupancy;
      mask = o.mask;
      अभाज्य = o.अभाज्य;
      max_chain_लंबाई = o.max_chain_लंबाई;
      स_नकल (वस्तुएँ, o.वस्तुएँ, माप (item_t) * (mask + 1));
      वापस;
    }

    alloc (o.population); hb_copy (o, *यह);
  }

  hb_hashmap_t (स्थिर hb_hashmap_t& o) : hb_hashmap_t () { _copy (o); }
  hb_hashmap_t& चालक= (स्थिर hb_hashmap_t& o)
  {
    reset ();
    अगर (!वस्तुएँ) { _copy (o); वापस *यह; }
    alloc (o.population); hb_copy (o, *यह); वापस *यह;
  }

  hb_hashmap_t (hb_hashmap_t&& o)  noexcept : hb_hashmap_t () { hb_swap (*यह, o); }
  hb_hashmap_t& चालक= (hb_hashmap_t&& o)   noexcept { hb_swap (*यह, o); वापस *यह; }

  hb_hashmap_t (std::initializer_list<hb_pair_t<K, V>> lst) : hb_hashmap_t ()
  {
    क्रम (स्वतः&& item : lst)
      set (item.first, item.second);
  }
  ढाँचा <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_hashmap_t (स्थिर Iterable &o) : hb_hashmap_t ()
  {
    स्वतः iter = hb_iter (o);
    अगर (iter.is_random_access_iterator || iter.has_fast_len)
      alloc (hb_len (iter));
    hb_copy (iter, *यह);
  }

  काष्ठा item_t
  {
    K key;
    uint32_t is_real_ : 1;
    uint32_t is_used_ : 1;
    uint32_t hash : 30;
    V value;

    item_t () : key (),
		is_real_ (false), is_used_ (false),
		hash (0),
		value () {}

    // Needed for https://github.com/harfbuzz/harfbuzz/issues/4138
    K& get_कुंजी () { वापस key; }
    V& get_मूल्य () { वापस value; }

    bool is_used () स्थिर { वापस is_used_; }
    व्योम set_used (bool is_used) { is_used_ = is_used; }
    व्योम set_real (bool is_real) { is_real_ = is_real; }
    bool is_real () स्थिर { वापस is_real_; }

    ढाँचा <bool v = minus_one,
	      hb_enable_if (v == false)>
    अटल अंतरभूत स्थिर V& default_मूल्य () { वापस Null(V); };
    ढाँचा <bool v = minus_one,
	      hb_enable_if (v == true)>
    अटल अंतरभूत स्थिर V& default_मूल्य ()
    {
      static_assert (hb_is_same (V, hb_codepoint_t), "");
      वापस minus_1;
    };

    bool चालक == (स्थिर K &o) स्थिर { वापस hb_deref (key) == hb_deref (o); }
    bool चालक == (स्थिर item_t &o) स्थिर { वापस *यह == o.key; }
    hb_pair_t<K, V> get_pair() स्थिर { वापस hb_pair_t<K, V> (key, value); }
    hb_pair_t<स्थिर K &, V &> get_pair_ref() { वापस hb_pair_t<स्थिर K &, V &> (key, value); }

    uint32_t योग_hash () स्थिर
    { वापस (hash * 31u) + hb_hash (value); }

    अटल constexpr bool is_trivially_constructible = (hb_is_trivially_constructible(K) && hb_is_trivially_constructible(V));
  };

  hb_object_header_t header;
  bool successful; /* Allocations successful */
  अचिन्हित लघु max_chain_लंबाई;
  अचिन्हित पूर्णांक population; /* Not including tombstones. */
  अचिन्हित पूर्णांक occupancy; /* Including tombstones. */
  अचिन्हित पूर्णांक mask;
  अचिन्हित पूर्णांक अभाज्य;
  item_t *वस्तुएँ;

  मित्र व्योम swap (hb_hashmap_t& a, hb_hashmap_t& b) noexcept
  {
    अगर (unlikely (!a.successful || !b.successful))
      वापस;
    hb_swap (a.max_chain_लंबाई, b.max_chain_लंबाई);
    hb_swap (a.population, b.population);
    hb_swap (a.occupancy, b.occupancy);
    hb_swap (a.mask, b.mask);
    hb_swap (a.अभाज्य, b.अभाज्य);
    hb_swap (a.वस्तुएँ, b.वस्तुएँ);
  }
  व्योम init ()
  {
    hb_object_init (यह);

    successful = true;
    max_chain_लंबाई = 0;
    population = occupancy = 0;
    mask = 0;
    अभाज्य = 0;
    वस्तुएँ = nullptr;
  }
  व्योम fini ()
  {
    hb_object_fini (यह);

    अगर (likely (वस्तुएँ))
    {
      अचिन्हित आकार = mask + 1;
      क्रम (अचिन्हित i = 0; i < आकार; i++)
	वस्तुएँ[i].~item_t ();
      hb_free (वस्तुएँ);
      वस्तुएँ = nullptr;
    }
    population = occupancy = 0;
  }

  hb_hashmap_t& reset ()
  {
    successful = true;
    सफा ();
    वापस *यह;
  }

  bool in_error () स्थिर { वापस !successful; }

  bool alloc (अचिन्हित new_population = 0)
  {
    अगर (unlikely (!successful)) वापस false;
    अगर (unlikely (population > 0x3FFFFFFFu || new_population > 0x3FFFFFFFu))
    {
      // Population sizes >0x3FFFFFFF will result in power
      // being larger than 31, which in turn leads to a new size
      // that overflows u32.
      successful = false;
      वापस false;
    }

    अगर (new_population != 0 && (new_population + new_population / 2) < mask) वापस true;

    अचिन्हित पूर्णांक power = hb_bit_storage (hb_max (hb_max ((अचिन्हित) population, new_population) * 2, 4u));
    अचिन्हित पूर्णांक new_आकार = 1u << power;
    item_t *new_वस्तुएँ = (item_t *) hb_malloc2 ((माप_प्रकार) new_आकार, माप (item_t));
    अगर (unlikely (!new_वस्तुएँ))
    {
      successful = false;
      वापस false;
    }
    अगर (!item_t::is_trivially_constructible)
      क्रम (स्वतः &_ : hb_iter (new_वस्तुएँ, new_आकार))
	नया (&_) item_t ();
    अन्यथा
      hb_memset (new_वस्तुएँ, 0, (माप_प्रकार) new_आकार * माप (item_t));

    अचिन्हित पूर्णांक old_आकार = आकार ();
    item_t *old_वस्तुएँ = वस्तुएँ;

    /* Switch to new, empty, array. */
    population = occupancy = 0;
    mask = new_आकार - 1;
    अभाज्य = अभाज्य_for (power);
    max_chain_लंबाई = power * 2;
    वस्तुएँ = new_वस्तुएँ;

    /* Insert back old items. */
    क्रम (अचिन्हित पूर्णांक i = 0; i < old_आकार; i++)
    {
      अगर (old_वस्तुएँ[i].is_real ())
      {
	set_with_hash (std::move (old_वस्तुएँ[i].key),
		       old_वस्तुएँ[i].hash,
		       std::move (old_वस्तुएँ[i].value));
      }
    }
    क्रम (अचिन्हित पूर्णांक i = 0; i < old_आकार; i++)
      old_वस्तुएँ[i].~item_t ();

    hb_free (old_वस्तुएँ);

    वापस true;
  }

  ढाँचा <typename T>
  bool in_storage (स्थिर T &v) स्थिर
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    वापस वस्तुएँ &&
	   (uintptr_t) वस्तुएँ <= p && p < (uintptr_t) (वस्तुएँ + आकार ());
  }

  ढाँचा <typename KK, typename VV>
  bool set_with_hash (KK&& key, uint32_t hash, VV&& value, bool overwrite = true)
  {
    अगर (unlikely (!successful)) वापस false;
    अगर (unlikely ((occupancy + occupancy / 2) >= mask))
    {
      /* key or value may live in our own storage, eg. m.set (k, m.get (j)).
       * alloc() frees that storage, so copy them out first. */
      अगर (unlikely (in_storage (key) || in_storage (value)))
      {
	K कुंजी_copy;
	कुंजी_copy = std::forward<KK> (key);
	V मूल्य_copy;
	मूल्य_copy = std::forward<VV> (value);
	वापस set_with_hash (std::move (कुंजी_copy), hash, std::move (मूल्य_copy), overwrite);
      }
      अगर (unlikely (!alloc ())) वापस false;
    }

    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    अचिन्हित पूर्णांक tombstone = (अचिन्हित पूर्णांक) -1;
    अचिन्हित पूर्णांक i = hash % अभाज्य;
    अचिन्हित लंबाई = 0;
    अचिन्हित step = 0;
    जबतक (वस्तुएँ[i].is_used ())
    {
      अगर ((std::is_integral<K>::value || वस्तुएँ[i].hash == hash) &&
	  वस्तुएँ[i] == key)
      {
        अगर (!overwrite)
	  वापस false;
        अन्यथा
	  अवरोध;
      }
      अगर (!वस्तुएँ[i].is_real () && tombstone == (अचिन्हित) -1)
        tombstone = i;
      i = (i + ++step) & mask;
      लंबाई++;
    }

    item_t &item = वस्तुएँ[tombstone == (अचिन्हित) -1 ? i : tombstone];

    अगर (item.is_used ())
    {
      occupancy--;
      population -= item.is_real ();
    }

    item.key = std::forward<KK> (key);
    item.value = std::forward<VV> (value);
    item.hash = hash;
    item.set_used (true);
    item.set_real (true);

    occupancy++;
    population++;

    अगर (unlikely (लंबाई > max_chain_लंबाई) && occupancy > mask / 8)
      alloc (mask - 8); // This ensures we jump to next larger size

    वापस true;
  }

  ढाँचा <typename VV>
  bool set (स्थिर K &key, VV&& value, bool overwrite = true) { वापस set_with_hash (key, hb_hash (key), std::forward<VV> (value), overwrite); }
  ढाँचा <typename VV>
  bool set (K &&key, VV&& value, bool overwrite = true)
  {
    uint32_t hash = hb_hash (key);
    वापस set_with_hash (std::move (key), hash, std::forward<VV> (value), overwrite);
  }
  bool add (स्थिर K &key)
  {
    uint32_t hash = hb_hash (key);
    वापस set_with_hash (key, hash, item_t::default_मूल्य ());
  }

  स्थिर V& get_with_hash (स्थिर K &key, uint32_t hash) स्थिर
  {
    अगर (!वस्तुएँ) वापस item_t::default_मूल्य ();
    स्वतः *item = fetch_item (key, hash);
    अगर (item)
      वापस item->value;
    वापस item_t::default_मूल्य ();
  }
  स्थिर V& लो (स्थिर K &key) स्थिर
  {
    अगर (!वस्तुएँ) वापस item_t::default_मूल्य ();
    वापस get_with_hash (key, hb_hash (key));
  }

  व्योम del (स्थिर K &key)
  {
    अगर (!वस्तुएँ) वापस;
    स्वतः *item = fetch_item (key, hb_hash (key));
    अगर (item)
    {
      item->set_real (false);
      population--;
    }
  }

  /* Has interface. */
  स्थिर V& चालक [] (K k) स्थिर { वापस लो (k); }
  ढाँचा <typename VV=V>
  bool has (स्थिर K &key, VV **vp = nullptr) स्थिर
  {
    अगर (!वस्तुएँ) वापस false;
    वापस has_with_hash (key, hb_hash (key), vp);
  }
  ढाँचा <typename VV=V>
  bool has_with_hash (स्थिर K &key, uint32_t hash, VV **vp = nullptr) स्थिर
  {
    अगर (!वस्तुएँ) वापस false;
    स्वतः *item = fetch_item (key, hash);
    अगर (item)
    {
      अगर (vp) *vp = std::addressof (item->value);
      वापस true;
    }
    वापस false;
  }
  item_t *fetch_item (स्थिर K &key, uint32_t hash) स्थिर
  {
    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    अचिन्हित पूर्णांक i = hash % अभाज्य;
    अचिन्हित step = 0;
    जबतक (वस्तुएँ[i].is_used ())
    {
      अगर ((std::is_integral<K>::value || वस्तुएँ[i].hash == hash) &&
	  वस्तुएँ[i] == key)
      {
	अगर (वस्तुएँ[i].is_real ())
	  वापस &वस्तुएँ[i];
	अन्यथा
	  वापस nullptr;
      }
      i = (i + ++step) & mask;
    }
    वापस nullptr;
  }
  /* Projection. */
  स्थिर V& चालक () (K k) स्थिर { वापस लो (k); }

  अचिन्हित आकार () स्थिर { वापस mask ? mask + 1 : 0; }

  व्योम सफा ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (zero-initialized) from any writes.  Any non-empty hashmap
     * is a real heap instance with writable items, so clearing
     * under !successful is safe. */
    अगर (!population && !occupancy) वापस;

    क्रम (स्वतः &_ : hb_iter (वस्तुएँ, आकार ()))
    {
      /* Reconstruct items. */
      _.~item_t ();
      नया (&_) item_t ();
    }

    population = occupancy = 0;
  }

  bool is_empty () स्थिर { वापस population == 0; }
  explicit चालक bool () स्थिर { वापस !is_empty (); }

  uint32_t hash () स्थिर
  {
    वापस
    + iter_वस्तुएँ ()
    | hb_reduce ([] (uint32_t h, स्थिर item_t &_) { वापस h ^ _.योग_hash (); }, (uint32_t) 0u)
    ;
  }

  bool is_equal (स्थिर hb_hashmap_t &other) स्थिर
  {
    अगर (population != other.population) वापस false;

    क्रम (स्वतः pair : iter ())
      अगर (other.लो (pair.first) != pair.second)
        वापस false;

    वापस true;
  }
  bool चालक == (स्थिर hb_hashmap_t &other) स्थिर { वापस is_equal (other); }
  bool चालक != (स्थिर hb_hashmap_t &other) स्थिर { वापस !is_equal (other); }

  अचिन्हित पूर्णांक get_population () स्थिर { वापस population; }

  व्योम update (स्थिर hb_hashmap_t &other)
  {
    अगर (unlikely (!successful)) वापस;

    hb_copy (other, *यह);
  }

  /*
   * Iterator
   */

  स्वतः iter_वस्तुएँ () स्थिर HB_AUTO_RETURN
  (
    + hb_iter (वस्तुएँ, यह->आकार ())
    | hb_filter (&item_t::is_real)
  )
  स्वतः iter_ref () स्थिर HB_AUTO_RETURN
  (
    + यह->iter_वस्तुएँ ()
    | hb_map (&item_t::get_pair_ref)
  )
  स्वतः iter () स्थिर HB_AUTO_RETURN
  (
    + यह->iter_वस्तुएँ ()
    | hb_map (&item_t::get_pair)
  )
  स्वतः कुंजियाँ_ref () स्थिर HB_AUTO_RETURN
  (
    + यह->iter_वस्तुएँ ()
    | hb_map (&item_t::get_कुंजी)
  )
  स्वतः कुंजियाँ () स्थिर HB_AUTO_RETURN
  (
    + यह->कुंजियाँ_ref ()
    | hb_map (hb_ridentity)
  )
  स्वतः मूल्यावली_ref () स्थिर HB_AUTO_RETURN
  (
    + यह->iter_वस्तुएँ ()
    | hb_map (&item_t::get_मूल्य)
  )
  स्वतः values () स्थिर HB_AUTO_RETURN
  (
    + यह->मूल्यावली_ref ()
    | hb_map (hb_ridentity)
  )

  /* C iterator. */
  bool next (पूर्णांक *idx,
	     K *key,
	     V *value) स्थिर
  {
    अचिन्हित i = (अचिन्हित) (*idx + 1);

    अचिन्हित गिनती = आकार ();
    जबतक (i < गिनती && !वस्तुएँ[i].is_real ())
      i++;

    अगर (i >= गिनती)
    {
      *idx = -1;
      वापस false;
    }

    *key = वस्तुएँ[i].key;
    *value = वस्तुएँ[i].value;

    *idx = (चिन्हित) i;
    वापस true;
  }

  /* Sink interface. */
  hb_hashmap_t& चालक << (स्थिर hb_pair_t<K, V>& v)
  { set (v.first, v.second); वापस *यह; }
  ढाँचा <typename V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (V2))>
  hb_hashmap_t& चालक << (स्थिर hb_pair_t<K, V&&>& v)
  { set (v.first, std::move (v.second)); वापस *यह; }
  ढाँचा <typename K2 = K,
	    hb_enable_if (!hb_is_trivially_copyable (K2))>
  hb_hashmap_t& चालक << (स्थिर hb_pair_t<K&&, V>& v)
  { set (std::move (v.first), v.second); वापस *यह; }
  ढाँचा <typename K2 = K, typename V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (K2) &&
			  !hb_is_trivially_copyable (V2))>
  hb_hashmap_t& चालक << (स्थिर hb_pair_t<K&&, V&&>& v)
  { set (std::move (v.first), std::move (v.second)); वापस *यह; }

  अटल अचिन्हित पूर्णांक अभाज्य_for (अचिन्हित पूर्णांक shift)
  {
    /* Following comment and table copied from glib. */
    /* Each table size has an associated prime modulo (the first prime
     * lower than the table size) used to find the initial bucket. Probing
     * then works modulo 2^n. The prime modulo is necessary to get a
     * good distribution with poor hash functions.
     */
    /* Not declaring static to make all kinds of compilers happy... */
    /*static*/ स्थिर अचिन्हित पूर्णांक अभाज्य_mod [32] =
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

    अगर (unlikely (shift >= ARRAY_लंबाई (अभाज्य_mod)))
      वापस अभाज्य_mod[ARRAY_लंबाई (अभाज्य_mod) - 1];

    वापस अभाज्य_mod[shift];
  }
};

/*
 * hb_map_t
 */

काष्ठा hb_map_t : hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       true>
{
  using hashmap = hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       true>;

  ~hb_map_t () = शेष;
  hb_map_t () : hashmap () {}
  hb_map_t (स्थिर hb_map_t &o) : hashmap ((hashmap &) o) {}
  hb_map_t (hb_map_t &&o)  noexcept : hashmap (std::move ((hashmap &) o)) {}
  hb_map_t& चालक= (स्थिर hb_map_t&) = शेष;
  hb_map_t& चालक= (hb_map_t&&) = शेष;
  hb_map_t (std::initializer_list<hb_codepoint_pair_t> lst) : hashmap (lst) {}
  ढाँचा <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_map_t (स्थिर Iterable &o) : hashmap (o) {}
};


#पूर्ण_अगर /* HB_MAP_HH */

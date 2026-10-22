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

#ضمن "hb.hh"

#ضمن "hb-set.hh"


/*
 * hb_hashmap_t
 */

خارجي HB_INTERNAL ثابت hb_codepoint_t minus_1;

template <typename K, typename V,
	  منطقي minus_one = خطأ>
بنية hb_hashmap_t
{
  ساكن constexpr منطقي realloc_move = true;

  hb_hashmap_t ()  { init (); }
  ~hb_hashmap_t () { fini (); }

  فراغ _copy (ثابت hb_hashmap_t& o)
  {
    إذا (unlikely (!o.successful))
    {
      successful = خطأ;
      أرجع;
    }
    إذا (unlikely (!o.mask)) أرجع;

    إذا (hb_is_trivially_copy_assignable (item_t))
    {
      بارڠن = (item_t *) hb_malloc2 ((o.mask + 1), حجم (item_t));
      إذا (unlikely (!بارڠن))
      {
	successful = خطأ;
	أرجع;
      }
      population = o.population;
      occupancy = o.occupancy;
      mask = o.mask;
      ڤردان = o.ڤردان;
      max_chain_length = o.max_chain_length;
      memcpy (بارڠن, o.بارڠن, حجم (item_t) * (mask + 1));
      أرجع;
    }

    alloc (o.population); hb_copy (o, *هذا);
  }

  hb_hashmap_t (ثابت hb_hashmap_t& o) : hb_hashmap_t () { _copy (o); }
  hb_hashmap_t& operator= (ثابت hb_hashmap_t& o)
  {
    reset ();
    إذا (!بارڠن) { _copy (o); أرجع *هذا; }
    alloc (o.population); hb_copy (o, *هذا); أرجع *هذا;
  }

  hb_hashmap_t (hb_hashmap_t&& o)  noexcept : hb_hashmap_t () { hb_swap (*هذا, o); }
  hb_hashmap_t& operator= (hb_hashmap_t&& o)   noexcept { hb_swap (*هذا, o); أرجع *هذا; }

  hb_hashmap_t (std::initializer_list<hb_pair_t<K, V>> lst) : hb_hashmap_t ()
  {
    لكل (auto&& item : lst)
      set (item.first, item.second);
  }
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_hashmap_t (ثابت Iterable &o) : hb_hashmap_t ()
  {
    auto iter = hb_iter (o);
    إذا (iter.is_random_access_iterator || iter.has_fast_len)
      alloc (hb_len (iter));
    hb_copy (iter, *هذا);
  }

  بنية item_t
  {
    K key;
    uint32_t is_real_ : 1;
    uint32_t is_used_ : 1;
    uint32_t hash : 30;
    V value;

    item_t () : key (),
		is_real_ (خطأ), is_used_ (خطأ),
		hash (0),
		value () {}

    // Needed for https://github.com/harfbuzz/harfbuzz/issues/4138
    K& get_کونچي () { أرجع key; }
    V& get_نيلاي () { أرجع value; }

    منطقي is_used () ثابت { أرجع is_used_; }
    فراغ set_used (منطقي is_used) { is_used_ = is_used; }
    فراغ set_real (منطقي is_real) { is_real_ = is_real; }
    منطقي is_real () ثابت { أرجع is_real_; }

    template <منطقي v = minus_one,
	      hb_enable_if (v == خطأ)>
    ساكن مضمن ثابت V& default_نيلاي () { أرجع Null(V); };
    template <منطقي v = minus_one,
	      hb_enable_if (v == true)>
    ساكن مضمن ثابت V& default_نيلاي ()
    {
      static_assert (hb_is_same (V, hb_codepoint_t), "");
      أرجع minus_1;
    };

    منطقي operator == (ثابت K &o) ثابت { أرجع hb_deref (key) == hb_deref (o); }
    منطقي operator == (ثابت item_t &o) ثابت { أرجع *هذا == o.key; }
    hb_pair_t<K, V> get_pair() ثابت { أرجع hb_pair_t<K, V> (key, value); }
    hb_pair_t<ثابت K &, V &> get_pair_ref() { أرجع hb_pair_t<ثابت K &, V &> (key, value); }

    uint32_t total_hash () ثابت
    { أرجع (hash * 31u) + hb_hash (value); }

    ساكن constexpr منطقي is_trivially_constructible = (hb_is_trivially_constructible(K) && hb_is_trivially_constructible(V));
  };

  hb_object_header_t header;
  منطقي successful; /* Allocations successful */
  unsigned short max_chain_length;
  unsigned صحيح population; /* Not including tombstones. */
  unsigned صحيح occupancy; /* Including tombstones. */
  unsigned صحيح mask;
  unsigned صحيح ڤردان;
  item_t *بارڠن;

  friend فراغ swap (hb_hashmap_t& a, hb_hashmap_t& b) noexcept
  {
    إذا (unlikely (!a.successful || !b.successful))
      أرجع;
    hb_swap (a.max_chain_length, b.max_chain_length);
    hb_swap (a.population, b.population);
    hb_swap (a.occupancy, b.occupancy);
    hb_swap (a.mask, b.mask);
    hb_swap (a.ڤردان, b.ڤردان);
    hb_swap (a.بارڠن, b.بارڠن);
  }
  فراغ init ()
  {
    hb_object_init (هذا);

    successful = true;
    max_chain_length = 0;
    population = occupancy = 0;
    mask = 0;
    ڤردان = 0;
    بارڠن = nullptr;
  }
  فراغ fini ()
  {
    hb_object_fini (هذا);

    إذا (likely (بارڠن))
    {
      unsigned size = mask + 1;
      لكل (unsigned i = 0; i < size; i++)
	بارڠن[i].~item_t ();
      hb_free (بارڠن);
      بارڠن = nullptr;
    }
    population = occupancy = 0;
  }

  hb_hashmap_t& reset ()
  {
    successful = true;
    clear ();
    أرجع *هذا;
  }

  منطقي in_error () ثابت { أرجع !successful; }

  منطقي alloc (unsigned new_population = 0)
  {
    إذا (unlikely (!successful)) أرجع خطأ;
    إذا (unlikely (population > 0x3FFFFFFFu || new_population > 0x3FFFFFFFu))
    {
      // Population sizes >0x3FFFFFFF will result in power
      // being larger than 31, which in turn leads to a new size
      // that overflows u32.
      successful = خطأ;
      أرجع خطأ;
    }

    إذا (new_population != 0 && (new_population + new_population / 2) < mask) أرجع true;

    unsigned صحيح power = hb_bit_storage (hb_max (hb_max ((unsigned) population, new_population) * 2, 4u));
    unsigned صحيح new_size = 1u << power;
    item_t *new_بارڠن = (item_t *) hb_malloc2 ((size_t) new_size, حجم (item_t));
    إذا (unlikely (!new_بارڠن))
    {
      successful = خطأ;
      أرجع خطأ;
    }
    إذا (!item_t::is_trivially_constructible)
      لكل (auto &_ : hb_iter (new_بارڠن, new_size))
	جديد (&_) item_t ();
    وإلا
      hb_memset (new_بارڠن, 0, (size_t) new_size * حجم (item_t));

    unsigned صحيح old_size = size ();
    item_t *old_بارڠن = بارڠن;

    /* Switch to new, empty, array. */
    population = occupancy = 0;
    mask = new_size - 1;
    ڤردان = ڤردان_for (power);
    max_chain_length = power * 2;
    بارڠن = new_بارڠن;

    /* Insert back old items. */
    لكل (unsigned صحيح i = 0; i < old_size; i++)
    {
      إذا (old_بارڠن[i].is_real ())
      {
	set_with_hash (std::move (old_بارڠن[i].key),
		       old_بارڠن[i].hash,
		       std::move (old_بارڠن[i].value));
      }
    }
    لكل (unsigned صحيح i = 0; i < old_size; i++)
      old_بارڠن[i].~item_t ();

    hb_free (old_بارڠن);

    أرجع true;
  }

  template <typename T>
  منطقي in_storage (ثابت T &v) ثابت
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    أرجع بارڠن &&
	   (uintptr_t) بارڠن <= p && p < (uintptr_t) (بارڠن + size ());
  }

  template <typename KK, typename VV>
  منطقي set_with_hash (KK&& key, uint32_t hash, VV&& value, منطقي overwrite = true)
  {
    إذا (unlikely (!successful)) أرجع خطأ;
    إذا (unlikely ((occupancy + occupancy / 2) >= mask))
    {
      /* key or value may live in our own storage, eg. m.set (k, m.get (j)).
       * alloc() frees that storage, so copy them out first. */
      إذا (unlikely (in_storage (key) || in_storage (value)))
      {
	K کونچي_copy;
	کونچي_copy = std::forward<KK> (key);
	V نيلاي_copy;
	نيلاي_copy = std::forward<VV> (value);
	أرجع set_with_hash (std::move (کونچي_copy), hash, std::move (نيلاي_copy), overwrite);
      }
      إذا (unlikely (!alloc ())) أرجع خطأ;
    }

    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    unsigned صحيح tombstone = (unsigned صحيح) -1;
    unsigned صحيح i = hash % ڤردان;
    unsigned length = 0;
    unsigned step = 0;
    طالما (بارڠن[i].is_used ())
    {
      إذا ((std::is_integral<K>::value || بارڠن[i].hash == hash) &&
	  بارڠن[i] == key)
      {
        إذا (!overwrite)
	  أرجع خطأ;
        وإلا
	  اكسر;
      }
      إذا (!بارڠن[i].is_real () && tombstone == (unsigned) -1)
        tombstone = i;
      i = (i + ++step) & mask;
      length++;
    }

    item_t &item = بارڠن[tombstone == (unsigned) -1 ? i : tombstone];

    إذا (item.is_used ())
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

    إذا (unlikely (length > max_chain_length) && occupancy > mask / 8)
      alloc (mask - 8); // This ensures we jump to next larger size

    أرجع true;
  }

  template <typename VV>
  منطقي set (ثابت K &key, VV&& value, منطقي overwrite = true) { أرجع set_with_hash (key, hb_hash (key), std::forward<VV> (value), overwrite); }
  template <typename VV>
  منطقي set (K &&key, VV&& value, منطقي overwrite = true)
  {
    uint32_t hash = hb_hash (key);
    أرجع set_with_hash (std::move (key), hash, std::forward<VV> (value), overwrite);
  }
  منطقي add (ثابت K &key)
  {
    uint32_t hash = hb_hash (key);
    أرجع set_with_hash (key, hash, item_t::default_نيلاي ());
  }

  ثابت V& get_with_hash (ثابت K &key, uint32_t hash) ثابت
  {
    إذا (!بارڠن) أرجع item_t::default_نيلاي ();
    auto *item = fetch_item (key, hash);
    إذا (item)
      أرجع item->value;
    أرجع item_t::default_نيلاي ();
  }
  ثابت V& get (ثابت K &key) ثابت
  {
    إذا (!بارڠن) أرجع item_t::default_نيلاي ();
    أرجع get_with_hash (key, hb_hash (key));
  }

  فراغ del (ثابت K &key)
  {
    إذا (!بارڠن) أرجع;
    auto *item = fetch_item (key, hb_hash (key));
    إذا (item)
    {
      item->set_real (خطأ);
      population--;
    }
  }

  /* Has interface. */
  ثابت V& operator [] (K k) ثابت { أرجع get (k); }
  template <typename VV=V>
  منطقي has (ثابت K &key, VV **vp = nullptr) ثابت
  {
    إذا (!بارڠن) أرجع خطأ;
    أرجع has_with_hash (key, hb_hash (key), vp);
  }
  template <typename VV=V>
  منطقي has_with_hash (ثابت K &key, uint32_t hash, VV **vp = nullptr) ثابت
  {
    إذا (!بارڠن) أرجع خطأ;
    auto *item = fetch_item (key, hash);
    إذا (item)
    {
      إذا (vp) *vp = std::addressof (item->value);
      أرجع true;
    }
    أرجع خطأ;
  }
  item_t *fetch_item (ثابت K &key, uint32_t hash) ثابت
  {
    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    unsigned صحيح i = hash % ڤردان;
    unsigned step = 0;
    طالما (بارڠن[i].is_used ())
    {
      إذا ((std::is_integral<K>::value || بارڠن[i].hash == hash) &&
	  بارڠن[i] == key)
      {
	إذا (بارڠن[i].is_real ())
	  أرجع &بارڠن[i];
	وإلا
	  أرجع nullptr;
      }
      i = (i + ++step) & mask;
    }
    أرجع nullptr;
  }
  /* Projection. */
  ثابت V& operator () (K k) ثابت { أرجع get (k); }

  unsigned size () ثابت { أرجع mask ? mask + 1 : 0; }

  فراغ clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (zero-initialized) from any writes.  Any non-empty hashmap
     * is a real heap instance with writable items, so clearing
     * under !successful is safe. */
    إذا (!population && !occupancy) أرجع;

    لكل (auto &_ : hb_iter (بارڠن, size ()))
    {
      /* Reconstruct items. */
      _.~item_t ();
      جديد (&_) item_t ();
    }

    population = occupancy = 0;
  }

  منطقي is_empty () ثابت { أرجع population == 0; }
  explicit operator منطقي () ثابت { أرجع !is_empty (); }

  uint32_t hash () ثابت
  {
    أرجع
    + iter_بارڠن ()
    | hb_reduce ([] (uint32_t h, ثابت item_t &_) { أرجع h ^ _.total_hash (); }, (uint32_t) 0u)
    ;
  }

  منطقي is_equal (ثابت hb_hashmap_t &other) ثابت
  {
    إذا (population != other.population) أرجع خطأ;

    لكل (auto pair : iter ())
      إذا (other.get (pair.first) != pair.second)
        أرجع خطأ;

    أرجع true;
  }
  منطقي operator == (ثابت hb_hashmap_t &other) ثابت { أرجع is_equal (other); }
  منطقي operator != (ثابت hb_hashmap_t &other) ثابت { أرجع !is_equal (other); }

  unsigned صحيح get_population () ثابت { أرجع population; }

  فراغ update (ثابت hb_hashmap_t &other)
  {
    إذا (unlikely (!successful)) أرجع;

    hb_copy (other, *هذا);
  }

  /*
   * Iterator
   */

  auto iter_بارڠن () ثابت HB_AUTO_RETURN
  (
    + hb_iter (بارڠن, هذا->size ())
    | hb_filter (&item_t::is_real)
  )
  auto iter_ref () ثابت HB_AUTO_RETURN
  (
    + هذا->iter_بارڠن ()
    | hb_map (&item_t::get_pair_ref)
  )
  auto iter () ثابت HB_AUTO_RETURN
  (
    + هذا->iter_بارڠن ()
    | hb_map (&item_t::get_pair)
  )
  auto کونچي_سموا_ref () ثابت HB_AUTO_RETURN
  (
    + هذا->iter_بارڠن ()
    | hb_map (&item_t::get_کونچي)
  )
  auto کونچي_سموا () ثابت HB_AUTO_RETURN
  (
    + هذا->کونچي_سموا_ref ()
    | hb_map (hb_ridentity)
  )
  auto نيلاي_سموا_ref () ثابت HB_AUTO_RETURN
  (
    + هذا->iter_بارڠن ()
    | hb_map (&item_t::get_نيلاي)
  )
  auto values () ثابت HB_AUTO_RETURN
  (
    + هذا->نيلاي_سموا_ref ()
    | hb_map (hb_ridentity)
  )

  /* C iterator. */
  منطقي next (صحيح *idx,
	     K *key,
	     V *value) ثابت
  {
    unsigned i = (unsigned) (*idx + 1);

    unsigned کيراءن = size ();
    طالما (i < کيراءن && !بارڠن[i].is_real ())
      i++;

    إذا (i >= کيراءن)
    {
      *idx = -1;
      أرجع خطأ;
    }

    *key = بارڠن[i].key;
    *value = بارڠن[i].value;

    *idx = (signed) i;
    أرجع true;
  }

  /* Sink interface. */
  hb_hashmap_t& operator << (ثابت hb_pair_t<K, V>& v)
  { set (v.first, v.second); أرجع *هذا; }
  template <typename V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (V2))>
  hb_hashmap_t& operator << (ثابت hb_pair_t<K, V&&>& v)
  { set (v.first, std::move (v.second)); أرجع *هذا; }
  template <typename K2 = K,
	    hb_enable_if (!hb_is_trivially_copyable (K2))>
  hb_hashmap_t& operator << (ثابت hb_pair_t<K&&, V>& v)
  { set (std::move (v.first), v.second); أرجع *هذا; }
  template <typename K2 = K, typename V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (K2) &&
			  !hb_is_trivially_copyable (V2))>
  hb_hashmap_t& operator << (ثابت hb_pair_t<K&&, V&&>& v)
  { set (std::move (v.first), std::move (v.second)); أرجع *هذا; }

  ساكن unsigned صحيح ڤردان_for (unsigned صحيح shift)
  {
    /* Following comment and table copied from glib. */
    /* Each table size has an associated prime modulo (the first prime
     * lower than the table size) used to find the initial bucket. Probing
     * then works modulo 2^n. The prime modulo is necessary to get a
     * good distribution with poor hash functions.
     */
    /* Not declaring static to make all kinds of compilers happy... */
    /*static*/ ثابت unsigned صحيح ڤردان_mod [32] =
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

    إذا (unlikely (shift >= ARRAY_LENGTH (ڤردان_mod)))
      أرجع ڤردان_mod[ARRAY_LENGTH (ڤردان_mod) - 1];

    أرجع ڤردان_mod[shift];
  }
};

/*
 * hb_map_t
 */

بنية hb_map_t : hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       true>
{
  using hashmap = hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       true>;

  ~hb_map_t () = افتراضي;
  hb_map_t () : hashmap () {}
  hb_map_t (ثابت hb_map_t &o) : hashmap ((hashmap &) o) {}
  hb_map_t (hb_map_t &&o)  noexcept : hashmap (std::move ((hashmap &) o)) {}
  hb_map_t& operator= (ثابت hb_map_t&) = افتراضي;
  hb_map_t& operator= (hb_map_t&&) = افتراضي;
  hb_map_t (std::initializer_list<hb_codepoint_pair_t> lst) : hashmap (lst) {}
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_map_t (ثابت Iterable &o) : hashmap (o) {}
};


#endif /* HB_MAP_HH */

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

#నిర్వచితం_కాకపోతే HB_MAP_HH
#నిర్వచించు HB_MAP_HH

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-set.hh"


/*
 * hb_hashmap_t
 */

బాహ్య HB_అంతర్గత స్థిరరాశి hb_codepoint_t minus_1;

మూస <రకం_పేరు K, రకం_పేరు V,
	  తార్కిక minus_one = అబద్ధం>
నిర్మాణం hb_hashmap_t
{
  స్థిర constexpr తార్కిక realloc_move = నిజం;

  hb_hashmap_t ()  { init (); }
  ~hb_hashmap_t () { fini (); }

  శూన్య _copy (స్థిరరాశి hb_hashmap_t& o)
  {
    ఒకవేళ (unlikely (!o.successful))
    {
      successful = అబద్ధం;
      తిరిగి;
    }
    ఒకవేళ (unlikely (!o.ముసుగు)) తిరిగి;

    ఒకవేళ (hb_is_trivially_copy_assignable (అంశం_t))
    {
      వస్తువులు = (అంశం_t *) hb_malloc2 ((o.ముసుగు + 1), పరిమాణం (అంశం_t));
      ఒకవేళ (unlikely (!వస్తువులు))
      {
	successful = అబద్ధం;
	తిరిగి;
      }
      population = o.population;
      occupancy = o.occupancy;
      ముసుగు = o.ముసుగు;
      ప్రధానసంఖ్య = o.ప్రధానసంఖ్య;
      max_chain_నిడివి = o.max_chain_నిడివి;
      జ్ఞాపకం_నకలు (వస్తువులు, o.వస్తువులు, పరిమాణం (అంశం_t) * (ముసుగు + 1));
      తిరిగి;
    }

    alloc (o.population); hb_copy (o, *ఇది);
  }

  hb_hashmap_t (స్థిరరాశి hb_hashmap_t& o) : hb_hashmap_t () { _copy (o); }
  hb_hashmap_t& క్రియాకారకం= (స్థిరరాశి hb_hashmap_t& o)
  {
    reset ();
    ఒకవేళ (!వస్తువులు) { _copy (o); తిరిగి *ఇది; }
    alloc (o.population); hb_copy (o, *ఇది); తిరిగి *ఇది;
  }

  hb_hashmap_t (hb_hashmap_t&& o)  noexcept : hb_hashmap_t () { hb_మార్పిడి (*ఇది, o); }
  hb_hashmap_t& క్రియాకారకం= (hb_hashmap_t&& o)   noexcept { hb_మార్పిడి (*ఇది, o); తిరిగి *ఇది; }

  hb_hashmap_t (std::initializer_list<hb_pair_t<K, V>> lst) : hb_hashmap_t ()
  {
    కొరకు (స్వయంచాలక&& అంశం : lst)
      సమితి (అంశం.మొదటి, అంశం.second);
  }
  మూస <రకం_పేరు Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_hashmap_t (స్థిరరాశి Iterable &o) : hb_hashmap_t ()
  {
    స్వయంచాలక iter = hb_iter (o);
    ఒకవేళ (iter.is_random_access_iterator || iter.has_fast_len)
      alloc (hb_len (iter));
    hb_copy (iter, *ఇది);
  }

  నిర్మాణం అంశం_t
  {
    K key;
    uint32_t is_real_ : 1;
    uint32_t is_వాడిన : 1;
    uint32_t hash : 30;
    V value;

    అంశం_t () : key (),
		is_real_ (అబద్ధం), is_వాడిన (అబద్ధం),
		hash (0),
		value () {}

    // Needed for https://github.com/harfbuzz/harfbuzz/issues/4138
    K& get_కీలకం () { తిరిగి key; }
    V& get_విలువ () { తిరిగి value; }

    తార్కిక is_వాడిన_2 () స్థిరరాశి { తిరిగి is_వాడిన; }
    శూన్య సమితి_వాడిన (తార్కిక is_వాడిన_2) { is_వాడిన = is_వాడిన_2; }
    శూన్య సమితి_real (తార్కిక is_real) { is_real_ = is_real; }
    తార్కిక is_real () స్థిరరాశి { తిరిగి is_real_; }

    మూస <తార్కిక v = minus_one,
	      hb_enable_if (v == అబద్ధం)>
    స్థిర అంతఃవరుస స్థిరరాశి V& default_విలువ () { తిరిగి Null(V); };
    మూస <తార్కిక v = minus_one,
	      hb_enable_if (v == నిజం)>
    స్థిర అంతఃవరుస స్థిరరాశి V& default_విలువ ()
    {
      static_assert (hb_is_same (V, hb_codepoint_t), "");
      తిరిగి minus_1;
    };

    తార్కిక క్రియాకారకం == (స్థిరరాశి K &o) స్థిరరాశి { తిరిగి hb_deref (key) == hb_deref (o); }
    తార్కిక క్రియాకారకం == (స్థిరరాశి అంశం_t &o) స్థిరరాశి { తిరిగి *ఇది == o.key; }
    hb_pair_t<K, V> get_pair() స్థిరరాశి { తిరిగి hb_pair_t<K, V> (key, value); }
    hb_pair_t<స్థిరరాశి K &, V &> get_pair_ref() { తిరిగి hb_pair_t<స్థిరరాశి K &, V &> (key, value); }

    uint32_t మొత్తం_hash () స్థిరరాశి
    { తిరిగి (hash * 31u) + hb_hash (value); }

    స్థిర constexpr తార్కిక is_trivially_constructible = (hb_is_trivially_constructible(K) && hb_is_trivially_constructible(V));
  };

  hb_object_శీర్షిక_t శీర్షిక;
  తార్కిక successful; /* Allocations successful */
  గుర్తులేని పొట్టి max_chain_నిడివి;
  గుర్తులేని పూర్ణాంకం population; /* Not including tombstones. */
  గుర్తులేని పూర్ణాంకం occupancy; /* Including tombstones. */
  గుర్తులేని పూర్ణాంకం ముసుగు;
  గుర్తులేని పూర్ణాంకం ప్రధానసంఖ్య;
  అంశం_t *వస్తువులు;

  మిత్ర శూన్య swap (hb_hashmap_t& a, hb_hashmap_t& b) noexcept
  {
    ఒకవేళ (unlikely (!a.successful || !b.successful))
      తిరిగి;
    hb_మార్పిడి (a.max_chain_నిడివి, b.max_chain_నిడివి);
    hb_మార్పిడి (a.population, b.population);
    hb_మార్పిడి (a.occupancy, b.occupancy);
    hb_మార్పిడి (a.ముసుగు, b.ముసుగు);
    hb_మార్పిడి (a.ప్రధానసంఖ్య, b.ప్రధానసంఖ్య);
    hb_మార్పిడి (a.వస్తువులు, b.వస్తువులు);
  }
  శూన్య init ()
  {
    hb_object_init (ఇది);

    successful = నిజం;
    max_chain_నిడివి = 0;
    population = occupancy = 0;
    ముసుగు = 0;
    ప్రధానసంఖ్య = 0;
    వస్తువులు = శూన్యసూచిక;
  }
  శూన్య fini ()
  {
    hb_object_fini (ఇది);

    ఒకవేళ (likely (వస్తువులు))
    {
      గుర్తులేని కొలత = ముసుగు + 1;
      కొరకు (గుర్తులేని i = 0; i < కొలత; i++)
	వస్తువులు[i].~అంశం_t ();
      hb_free (వస్తువులు);
      వస్తువులు = శూన్యసూచిక;
    }
    population = occupancy = 0;
  }

  hb_hashmap_t& reset ()
  {
    successful = నిజం;
    clear ();
    తిరిగి *ఇది;
  }

  తార్కిక లోపల_error () స్థిరరాశి { తిరిగి !successful; }

  తార్కిక alloc (గుర్తులేని new_population = 0)
  {
    ఒకవేళ (unlikely (!successful)) తిరిగి అబద్ధం;
    ఒకవేళ (unlikely (population > 0x3FFFFFFFu || new_population > 0x3FFFFFFFu))
    {
      // Population sizes >0x3FFFFFFF will result in power
      // being larger than 31, which in turn leads to a new size
      // that overflows u32.
      successful = అబద్ధం;
      తిరిగి అబద్ధం;
    }

    ఒకవేళ (new_population != 0 && (new_population + new_population / 2) < ముసుగు) తిరిగి నిజం;

    గుర్తులేని పూర్ణాంకం power = hb_బిట్టు_storage (hb_max (hb_max ((గుర్తులేని) population, new_population) * 2, 4u));
    గుర్తులేని పూర్ణాంకం new_కొలత = 1u << power;
    అంశం_t *new_వస్తువులు = (అంశం_t *) hb_malloc2 ((size_t) new_కొలత, పరిమాణం (అంశం_t));
    ఒకవేళ (unlikely (!new_వస్తువులు))
    {
      successful = అబద్ధం;
      తిరిగి అబద్ధం;
    }
    ఒకవేళ (!అంశం_t::is_trivially_constructible)
      కొరకు (స్వయంచాలక &_ : hb_iter (new_వస్తువులు, new_కొలత))
	కొత్త (&_) అంశం_t ();
    లేకపోతే
      hb_memset (new_వస్తువులు, 0, (size_t) new_కొలత * పరిమాణం (అంశం_t));

    గుర్తులేని పూర్ణాంకం old_కొలత = కొలత ();
    అంశం_t *old_వస్తువులు = వస్తువులు;

    /* Switch to new, empty, array. */
    population = occupancy = 0;
    ముసుగు = new_కొలత - 1;
    ప్రధానసంఖ్య = ప్రధానసంఖ్య_for (power);
    max_chain_నిడివి = power * 2;
    వస్తువులు = new_వస్తువులు;

    /* Insert back old items. */
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < old_కొలత; i++)
    {
      ఒకవేళ (old_వస్తువులు[i].is_real ())
      {
	సమితి_with_hash (std::move (old_వస్తువులు[i].key),
		       old_వస్తువులు[i].hash,
		       std::move (old_వస్తువులు[i].value));
      }
    }
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < old_కొలత; i++)
      old_వస్తువులు[i].~అంశం_t ();

    hb_free (old_వస్తువులు);

    తిరిగి నిజం;
  }

  మూస <రకం_పేరు T>
  తార్కిక లోపల_storage (స్థిరరాశి T &v) స్థిరరాశి
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    తిరిగి వస్తువులు &&
	   (uintptr_t) వస్తువులు <= p && p < (uintptr_t) (వస్తువులు + కొలత ());
  }

  మూస <రకం_పేరు KK, రకం_పేరు VV>
  తార్కిక సమితి_with_hash (KK&& key, uint32_t hash, VV&& value, తార్కిక overwrite = నిజం)
  {
    ఒకవేళ (unlikely (!successful)) తిరిగి అబద్ధం;
    ఒకవేళ (unlikely ((occupancy + occupancy / 2) >= ముసుగు))
    {
      /* key or value may live in our own storage, eg. m.set (k, m.get (j)).
       * alloc() frees that storage, so copy them out first. */
      ఒకవేళ (unlikely (లోపల_storage (key) || లోపల_storage (value)))
      {
	K కీలకం_copy;
	కీలకం_copy = std::forward<KK> (key);
	V విలువ_copy;
	విలువ_copy = std::forward<VV> (value);
	తిరిగి సమితి_with_hash (std::move (కీలకం_copy), hash, std::move (విలువ_copy), overwrite);
      }
      ఒకవేళ (unlikely (!alloc ())) తిరిగి అబద్ధం;
    }

    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    గుర్తులేని పూర్ణాంకం tombstone = (గుర్తులేని పూర్ణాంకం) -1;
    గుర్తులేని పూర్ణాంకం i = hash % ప్రధానసంఖ్య;
    గుర్తులేని నిడివి = 0;
    గుర్తులేని అడుగు = 0;
    అయితే (వస్తువులు[i].is_వాడిన_2 ())
    {
      ఒకవేళ ((std::is_integral<K>::value || వస్తువులు[i].hash == hash) &&
	  వస్తువులు[i] == key)
      {
        ఒకవేళ (!overwrite)
	  తిరిగి అబద్ధం;
        లేకపోతే
	  ఆపు;
      }
      ఒకవేళ (!వస్తువులు[i].is_real () && tombstone == (గుర్తులేని) -1)
        tombstone = i;
      i = (i + ++అడుగు) & ముసుగు;
      నిడివి++;
    }

    అంశం_t &అంశం = వస్తువులు[tombstone == (గుర్తులేని) -1 ? i : tombstone];

    ఒకవేళ (అంశం.is_వాడిన_2 ())
    {
      occupancy--;
      population -= అంశం.is_real ();
    }

    అంశం.key = std::forward<KK> (key);
    అంశం.value = std::forward<VV> (value);
    అంశం.hash = hash;
    అంశం.సమితి_వాడిన (నిజం);
    అంశం.సమితి_real (నిజం);

    occupancy++;
    population++;

    ఒకవేళ (unlikely (నిడివి > max_chain_నిడివి) && occupancy > ముసుగు / 8)
      alloc (ముసుగు - 8); // This ensures we jump to next larger size

    తిరిగి నిజం;
  }

  మూస <రకం_పేరు VV>
  తార్కిక సమితి (స్థిరరాశి K &key, VV&& value, తార్కిక overwrite = నిజం) { తిరిగి సమితి_with_hash (key, hb_hash (key), std::forward<VV> (value), overwrite); }
  మూస <రకం_పేరు VV>
  తార్కిక సమితి (K &&key, VV&& value, తార్కిక overwrite = నిజం)
  {
    uint32_t hash = hb_hash (key);
    తిరిగి సమితి_with_hash (std::move (key), hash, std::forward<VV> (value), overwrite);
  }
  తార్కిక కూడు (స్థిరరాశి K &key)
  {
    uint32_t hash = hb_hash (key);
    తిరిగి సమితి_with_hash (key, hash, అంశం_t::default_విలువ ());
  }

  స్థిరరాశి V& get_with_hash (స్థిరరాశి K &key, uint32_t hash) స్థిరరాశి
  {
    ఒకవేళ (!వస్తువులు) తిరిగి అంశం_t::default_విలువ ();
    స్వయంచాలక *అంశం = fetch_అంశం (key, hash);
    ఒకవేళ (అంశం)
      తిరిగి అంశం->value;
    తిరిగి అంశం_t::default_విలువ ();
  }
  స్థిరరాశి V& get (స్థిరరాశి K &key) స్థిరరాశి
  {
    ఒకవేళ (!వస్తువులు) తిరిగి అంశం_t::default_విలువ ();
    తిరిగి get_with_hash (key, hb_hash (key));
  }

  శూన్య del (స్థిరరాశి K &key)
  {
    ఒకవేళ (!వస్తువులు) తిరిగి;
    స్వయంచాలక *అంశం = fetch_అంశం (key, hb_hash (key));
    ఒకవేళ (అంశం)
    {
      అంశం->సమితి_real (అబద్ధం);
      population--;
    }
  }

  /* Has interface. */
  స్థిరరాశి V& క్రియాకారకం [] (K k) స్థిరరాశి { తిరిగి get (k); }
  మూస <రకం_పేరు VV=V>
  తార్కిక has (స్థిరరాశి K &key, VV **vp = శూన్యసూచిక) స్థిరరాశి
  {
    ఒకవేళ (!వస్తువులు) తిరిగి అబద్ధం;
    తిరిగి has_with_hash (key, hb_hash (key), vp);
  }
  మూస <రకం_పేరు VV=V>
  తార్కిక has_with_hash (స్థిరరాశి K &key, uint32_t hash, VV **vp = శూన్యసూచిక) స్థిరరాశి
  {
    ఒకవేళ (!వస్తువులు) తిరిగి అబద్ధం;
    స్వయంచాలక *అంశం = fetch_అంశం (key, hash);
    ఒకవేళ (అంశం)
    {
      ఒకవేళ (vp) *vp = std::addressof (అంశం->value);
      తిరిగి నిజం;
    }
    తిరిగి అబద్ధం;
  }
  అంశం_t *fetch_అంశం (స్థిరరాశి K &key, uint32_t hash) స్థిరరాశి
  {
    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    గుర్తులేని పూర్ణాంకం i = hash % ప్రధానసంఖ్య;
    గుర్తులేని అడుగు = 0;
    అయితే (వస్తువులు[i].is_వాడిన_2 ())
    {
      ఒకవేళ ((std::is_integral<K>::value || వస్తువులు[i].hash == hash) &&
	  వస్తువులు[i] == key)
      {
	ఒకవేళ (వస్తువులు[i].is_real ())
	  తిరిగి &వస్తువులు[i];
	లేకపోతే
	  తిరిగి శూన్యసూచిక;
      }
      i = (i + ++అడుగు) & ముసుగు;
    }
    తిరిగి శూన్యసూచిక;
  }
  /* Projection. */
  స్థిరరాశి V& క్రియాకారకం () (K k) స్థిరరాశి { తిరిగి get (k); }

  గుర్తులేని కొలత () స్థిరరాశి { తిరిగి ముసుగు ? ముసుగు + 1 : 0; }

  శూన్య clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (zero-initialized) from any writes.  Any non-empty hashmap
     * is a real heap instance with writable items, so clearing
     * under !successful is safe. */
    ఒకవేళ (!population && !occupancy) తిరిగి;

    కొరకు (స్వయంచాలక &_ : hb_iter (వస్తువులు, కొలత ()))
    {
      /* Reconstruct items. */
      _.~అంశం_t ();
      కొత్త (&_) అంశం_t ();
    }

    population = occupancy = 0;
  }

  తార్కిక is_empty () స్థిరరాశి { తిరిగి population == 0; }
  స్పష్టమైన క్రియాకారకం తార్కిక () స్థిరరాశి { తిరిగి !is_empty (); }

  uint32_t hash () స్థిరరాశి
  {
    తిరిగి
    + iter_వస్తువులు ()
    | hb_reduce ([] (uint32_t h, స్థిరరాశి అంశం_t &_) { తిరిగి h ^ _.మొత్తం_hash (); }, (uint32_t) 0u)
    ;
  }

  తార్కిక is_equal (స్థిరరాశి hb_hashmap_t &other) స్థిరరాశి
  {
    ఒకవేళ (population != other.population) తిరిగి అబద్ధం;

    కొరకు (స్వయంచాలక pair : iter ())
      ఒకవేళ (other.get (pair.మొదటి) != pair.second)
        తిరిగి అబద్ధం;

    తిరిగి నిజం;
  }
  తార్కిక క్రియాకారకం == (స్థిరరాశి hb_hashmap_t &other) స్థిరరాశి { తిరిగి is_equal (other); }
  తార్కిక క్రియాకారకం != (స్థిరరాశి hb_hashmap_t &other) స్థిరరాశి { తిరిగి !is_equal (other); }

  గుర్తులేని పూర్ణాంకం get_population () స్థిరరాశి { తిరిగి population; }

  శూన్య update (స్థిరరాశి hb_hashmap_t &other)
  {
    ఒకవేళ (unlikely (!successful)) తిరిగి;

    hb_copy (other, *ఇది);
  }

  /*
   * Iterator
   */

  స్వయంచాలక iter_వస్తువులు () స్థిరరాశి HB_AUTO_RETURN
  (
    + hb_iter (వస్తువులు, ఇది->కొలత ())
    | hb_filter (&అంశం_t::is_real)
  )
  స్వయంచాలక iter_ref () స్థిరరాశి HB_AUTO_RETURN
  (
    + ఇది->iter_వస్తువులు ()
    | hb_map (&అంశం_t::get_pair_ref)
  )
  స్వయంచాలక iter () స్థిరరాశి HB_AUTO_RETURN
  (
    + ఇది->iter_వస్తువులు ()
    | hb_map (&అంశం_t::get_pair)
  )
  స్వయంచాలక కీలకాలు_ref () స్థిరరాశి HB_AUTO_RETURN
  (
    + ఇది->iter_వస్తువులు ()
    | hb_map (&అంశం_t::get_కీలకం)
  )
  స్వయంచాలక కీలకాలు () స్థిరరాశి HB_AUTO_RETURN
  (
    + ఇది->కీలకాలు_ref ()
    | hb_map (hb_ridentity)
  )
  స్వయంచాలక విలువలు_ref () స్థిరరాశి HB_AUTO_RETURN
  (
    + ఇది->iter_వస్తువులు ()
    | hb_map (&అంశం_t::get_విలువ)
  )
  స్వయంచాలక values () స్థిరరాశి HB_AUTO_RETURN
  (
    + ఇది->విలువలు_ref ()
    | hb_map (hb_ridentity)
  )

  /* C iterator. */
  తార్కిక next (పూర్ణాంకం *idx,
	     K *key,
	     V *value) స్థిరరాశి
  {
    గుర్తులేని i = (గుర్తులేని) (*idx + 1);

    గుర్తులేని లెక్క = కొలత ();
    అయితే (i < లెక్క && !వస్తువులు[i].is_real ())
      i++;

    ఒకవేళ (i >= లెక్క)
    {
      *idx = -1;
      తిరిగి అబద్ధం;
    }

    *key = వస్తువులు[i].key;
    *value = వస్తువులు[i].value;

    *idx = (గుర్తుగల) i;
    తిరిగి నిజం;
  }

  /* Sink interface. */
  hb_hashmap_t& క్రియాకారకం << (స్థిరరాశి hb_pair_t<K, V>& v)
  { సమితి (v.మొదటి, v.second); తిరిగి *ఇది; }
  మూస <రకం_పేరు V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (V2))>
  hb_hashmap_t& క్రియాకారకం << (స్థిరరాశి hb_pair_t<K, V&&>& v)
  { సమితి (v.మొదటి, std::move (v.second)); తిరిగి *ఇది; }
  మూస <రకం_పేరు K2 = K,
	    hb_enable_if (!hb_is_trivially_copyable (K2))>
  hb_hashmap_t& క్రియాకారకం << (స్థిరరాశి hb_pair_t<K&&, V>& v)
  { సమితి (std::move (v.మొదటి), v.second); తిరిగి *ఇది; }
  మూస <రకం_పేరు K2 = K, రకం_పేరు V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (K2) &&
			  !hb_is_trivially_copyable (V2))>
  hb_hashmap_t& క్రియాకారకం << (స్థిరరాశి hb_pair_t<K&&, V&&>& v)
  { సమితి (std::move (v.మొదటి), std::move (v.second)); తిరిగి *ఇది; }

  స్థిర గుర్తులేని పూర్ణాంకం ప్రధానసంఖ్య_for (గుర్తులేని పూర్ణాంకం జరుపు)
  {
    /* Following comment and table copied from glib. */
    /* Each table size has an associated prime modulo (the first prime
     * lower than the table size) used to find the initial bucket. Probing
     * then works modulo 2^n. The prime modulo is necessary to get a
     * good distribution with poor hash functions.
     */
    /* Not declaring static to make all kinds of compilers happy... */
    /*static*/ స్థిరరాశి గుర్తులేని పూర్ణాంకం ప్రధానసంఖ్య_mod [32] =
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

    ఒకవేళ (unlikely (జరుపు >= ARRAY_నిడివి (ప్రధానసంఖ్య_mod)))
      తిరిగి ప్రధానసంఖ్య_mod[ARRAY_నిడివి (ప్రధానసంఖ్య_mod) - 1];

    తిరిగి ప్రధానసంఖ్య_mod[జరుపు];
  }
};

/*
 * hb_map_t
 */

నిర్మాణం hb_map_t : hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       నిజం>
{
  ఉపయోగించి hashmap = hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       నిజం>;

  ~hb_map_t () = అప్రమేయ;
  hb_map_t () : hashmap () {}
  hb_map_t (స్థిరరాశి hb_map_t &o) : hashmap ((hashmap &) o) {}
  hb_map_t (hb_map_t &&o)  noexcept : hashmap (std::move ((hashmap &) o)) {}
  hb_map_t& క్రియాకారకం= (స్థిరరాశి hb_map_t&) = అప్రమేయ;
  hb_map_t& క్రియాకారకం= (hb_map_t&&) = అప్రమేయ;
  hb_map_t (std::initializer_list<hb_codepoint_pair_t> lst) : hashmap (lst) {}
  మూస <రకం_పేరు Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_map_t (స్థిరరాశి Iterable &o) : hashmap (o) {}
};


#అయితే_ముగింపు /* HB_MAP_HH */

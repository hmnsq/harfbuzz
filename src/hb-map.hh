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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_MAP_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MAP_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-set.hh"


/*
 * hb_hashmap_t
 */

ਬਾਹਰੀ HB_ਅੰਦਰੂਨੀ ਅਚਲ hb_codepoint_t minus_1;

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ K, ਕਿਸਮ_ਨਾਂ V,
	  ਤਾਰਕਿਕ minus_one = ਝੂਠ>
ਬਣਤਰ hb_hashmap_t
{
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ realloc_move = ਸੱਚ;

  hb_hashmap_t ()  { init (); }
  ~hb_hashmap_t () { fini (); }

  ਖਾਲੀ _copy (ਅਚਲ hb_hashmap_t& o)
  {
    ਜੇ (unlikely (!o.successful))
    {
      successful = ਝੂਠ;
      ਵਾਪਸ;
    }
    ਜੇ (unlikely (!o.ਨਕਾਬ)) ਵਾਪਸ;

    ਜੇ (hb_is_trivially_copy_assignable (ਵਸਤੂ_t))
    {
      ਚੀਜ਼ਾਂ = (ਵਸਤੂ_t *) hb_malloc2 ((o.ਨਕਾਬ + 1), ਆਕਾਰ (ਵਸਤੂ_t));
      ਜੇ (unlikely (!ਚੀਜ਼ਾਂ))
      {
	successful = ਝੂਠ;
	ਵਾਪਸ;
      }
      population = o.population;
      occupancy = o.occupancy;
      ਨਕਾਬ = o.ਨਕਾਬ;
      ਅਭਾਜ = o.ਅਭਾਜ;
      max_chain_ਲੰਮਾਈ = o.max_chain_ਲੰਮਾਈ;
      ਸਮ੍ਰਿਤੀ_ਨਕਲ (ਚੀਜ਼ਾਂ, o.ਚੀਜ਼ਾਂ, ਆਕਾਰ (ਵਸਤੂ_t) * (ਨਕਾਬ + 1));
      ਵਾਪਸ;
    }

    alloc (o.population); hb_copy (o, *ਇਹ);
  }

  hb_hashmap_t (ਅਚਲ hb_hashmap_t& o) : hb_hashmap_t () { _copy (o); }
  hb_hashmap_t& ਸੰਚਾਲਕ= (ਅਚਲ hb_hashmap_t& o)
  {
    reset ();
    ਜੇ (!ਚੀਜ਼ਾਂ) { _copy (o); ਵਾਪਸ *ਇਹ; }
    alloc (o.population); hb_copy (o, *ਇਹ); ਵਾਪਸ *ਇਹ;
  }

  hb_hashmap_t (hb_hashmap_t&& o)  noexcept : hb_hashmap_t () { hb_ਅਦਲਾ_ਬਦਲੀ (*ਇਹ, o); }
  hb_hashmap_t& ਸੰਚਾਲਕ= (hb_hashmap_t&& o)   noexcept { hb_ਅਦਲਾ_ਬਦਲੀ (*ਇਹ, o); ਵਾਪਸ *ਇਹ; }

  hb_hashmap_t (std::initializer_list<hb_pair_t<K, V>> lst) : hb_hashmap_t ()
  {
    ਲਈ (ਸਵੈਚਾਲਿਤ&& ਵਸਤੂ : lst)
      ਸੈੱਟ (ਵਸਤੂ.ਪਹਿਲਾ, ਵਸਤੂ.second);
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_hashmap_t (ਅਚਲ Iterable &o) : hb_hashmap_t ()
  {
    ਸਵੈਚਾਲਿਤ iter = hb_iter (o);
    ਜੇ (iter.is_random_access_iterator || iter.has_fast_len)
      alloc (hb_len (iter));
    hb_copy (iter, *ਇਹ);
  }

  ਬਣਤਰ ਵਸਤੂ_t
  {
    K key;
    uint32_t is_real_ : 1;
    uint32_t is_ਵਰਤਿਆ : 1;
    uint32_t hash : 30;
    V value;

    ਵਸਤੂ_t () : key (),
		is_real_ (ਝੂਠ), is_ਵਰਤਿਆ (ਝੂਠ),
		hash (0),
		value () {}

    // Needed for https://github.com/harfbuzz/harfbuzz/issues/4138
    K& get_ਕੁੰਜੀ () { ਵਾਪਸ key; }
    V& get_ਮੁੱਲ () { ਵਾਪਸ value; }

    ਤਾਰਕਿਕ is_ਵਰਤਿਆ_2 () ਅਚਲ { ਵਾਪਸ is_ਵਰਤਿਆ; }
    ਖਾਲੀ ਸੈੱਟ_ਵਰਤਿਆ (ਤਾਰਕਿਕ is_ਵਰਤਿਆ_2) { is_ਵਰਤਿਆ = is_ਵਰਤਿਆ_2; }
    ਖਾਲੀ ਸੈੱਟ_real (ਤਾਰਕਿਕ is_real) { is_real_ = is_real; }
    ਤਾਰਕਿਕ is_real () ਅਚਲ { ਵਾਪਸ is_real_; }

    ਸਾਂਚਾ <ਤਾਰਕਿਕ v = minus_one,
	      hb_enable_if (v == ਝੂਠ)>
    ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ V& default_ਮੁੱਲ () { ਵਾਪਸ Null(V); };
    ਸਾਂਚਾ <ਤਾਰਕਿਕ v = minus_one,
	      hb_enable_if (v == ਸੱਚ)>
    ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ V& default_ਮੁੱਲ ()
    {
      static_assert (hb_is_same (V, hb_codepoint_t), "");
      ਵਾਪਸ minus_1;
    };

    ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ K &o) ਅਚਲ { ਵਾਪਸ hb_deref (key) == hb_deref (o); }
    ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ ਵਸਤੂ_t &o) ਅਚਲ { ਵਾਪਸ *ਇਹ == o.key; }
    hb_pair_t<K, V> get_pair() ਅਚਲ { ਵਾਪਸ hb_pair_t<K, V> (key, value); }
    hb_pair_t<ਅਚਲ K &, V &> get_pair_ref() { ਵਾਪਸ hb_pair_t<ਅਚਲ K &, V &> (key, value); }

    uint32_t ਕੁੱਲ_hash () ਅਚਲ
    { ਵਾਪਸ (hash * 31u) + hb_hash (value); }

    ਸਥਿਰ constexpr ਤਾਰਕਿਕ is_trivially_constructible = (hb_is_trivially_constructible(K) && hb_is_trivially_constructible(V));
  };

  hb_object_ਸਿਰਲੇਖ_t ਸਿਰਲੇਖ;
  ਤਾਰਕਿਕ successful; /* Allocations successful */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ max_chain_ਲੰਮਾਈ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ population; /* Not including tombstones. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ occupancy; /* Including tombstones. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਨਕਾਬ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅਭਾਜ;
  ਵਸਤੂ_t *ਚੀਜ਼ਾਂ;

  ਮਿੱਤਰ ਖਾਲੀ swap (hb_hashmap_t& a, hb_hashmap_t& b) noexcept
  {
    ਜੇ (unlikely (!a.successful || !b.successful))
      ਵਾਪਸ;
    hb_ਅਦਲਾ_ਬਦਲੀ (a.max_chain_ਲੰਮਾਈ, b.max_chain_ਲੰਮਾਈ);
    hb_ਅਦਲਾ_ਬਦਲੀ (a.population, b.population);
    hb_ਅਦਲਾ_ਬਦਲੀ (a.occupancy, b.occupancy);
    hb_ਅਦਲਾ_ਬਦਲੀ (a.ਨਕਾਬ, b.ਨਕਾਬ);
    hb_ਅਦਲਾ_ਬਦਲੀ (a.ਅਭਾਜ, b.ਅਭਾਜ);
    hb_ਅਦਲਾ_ਬਦਲੀ (a.ਚੀਜ਼ਾਂ, b.ਚੀਜ਼ਾਂ);
  }
  ਖਾਲੀ init ()
  {
    hb_object_init (ਇਹ);

    successful = ਸੱਚ;
    max_chain_ਲੰਮਾਈ = 0;
    population = occupancy = 0;
    ਨਕਾਬ = 0;
    ਅਭਾਜ = 0;
    ਚੀਜ਼ਾਂ = ਖਾਲੀ_ਸੂਚਕ;
  }
  ਖਾਲੀ fini ()
  {
    hb_object_fini (ਇਹ);

    ਜੇ (likely (ਚੀਜ਼ਾਂ))
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ = ਨਕਾਬ + 1;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਮਾਪ; i++)
	ਚੀਜ਼ਾਂ[i].~ਵਸਤੂ_t ();
      hb_free (ਚੀਜ਼ਾਂ);
      ਚੀਜ਼ਾਂ = ਖਾਲੀ_ਸੂਚਕ;
    }
    population = occupancy = 0;
  }

  hb_hashmap_t& reset ()
  {
    successful = ਸੱਚ;
    clear ();
    ਵਾਪਸ *ਇਹ;
  }

  ਤਾਰਕਿਕ ਵਿੱਚ_error () ਅਚਲ { ਵਾਪਸ !successful; }

  ਤਾਰਕਿਕ alloc (ਬਿਨਾਨਿਸ਼ਾਨੀ new_population = 0)
  {
    ਜੇ (unlikely (!successful)) ਵਾਪਸ ਝੂਠ;
    ਜੇ (unlikely (population > 0x3FFFFFFFu || new_population > 0x3FFFFFFFu))
    {
      // Population sizes >0x3FFFFFFF will result in power
      // being larger than 31, which in turn leads to a new size
      // that overflows u32.
      successful = ਝੂਠ;
      ਵਾਪਸ ਝੂਠ;
    }

    ਜੇ (new_population != 0 && (new_population + new_population / 2) < ਨਕਾਬ) ਵਾਪਸ ਸੱਚ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ power = hb_ਦੋਆਧਾਰੀ_ਅੰਕ_storage (hb_max (hb_max ((ਬਿਨਾਨਿਸ਼ਾਨੀ) population, new_population) * 2, 4u));
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ new_ਮਾਪ = 1u << power;
    ਵਸਤੂ_t *new_ਚੀਜ਼ਾਂ = (ਵਸਤੂ_t *) hb_malloc2 ((size_t) new_ਮਾਪ, ਆਕਾਰ (ਵਸਤੂ_t));
    ਜੇ (unlikely (!new_ਚੀਜ਼ਾਂ))
    {
      successful = ਝੂਠ;
      ਵਾਪਸ ਝੂਠ;
    }
    ਜੇ (!ਵਸਤੂ_t::is_trivially_constructible)
      ਲਈ (ਸਵੈਚਾਲਿਤ &_ : hb_iter (new_ਚੀਜ਼ਾਂ, new_ਮਾਪ))
	ਨਵਾਂ (&_) ਵਸਤੂ_t ();
    ਨਹੀਂਤਾਂ
      hb_memset (new_ਚੀਜ਼ਾਂ, 0, (size_t) new_ਮਾਪ * ਆਕਾਰ (ਵਸਤੂ_t));

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ old_ਮਾਪ = ਮਾਪ ();
    ਵਸਤੂ_t *old_ਚੀਜ਼ਾਂ = ਚੀਜ਼ਾਂ;

    /* Switch to new, empty, array. */
    population = occupancy = 0;
    ਨਕਾਬ = new_ਮਾਪ - 1;
    ਅਭਾਜ = ਅਭਾਜ_for (power);
    max_chain_ਲੰਮਾਈ = power * 2;
    ਚੀਜ਼ਾਂ = new_ਚੀਜ਼ਾਂ;

    /* Insert back old items. */
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < old_ਮਾਪ; i++)
    {
      ਜੇ (old_ਚੀਜ਼ਾਂ[i].is_real ())
      {
	ਸੈੱਟ_with_hash (std::move (old_ਚੀਜ਼ਾਂ[i].key),
		       old_ਚੀਜ਼ਾਂ[i].hash,
		       std::move (old_ਚੀਜ਼ਾਂ[i].value));
      }
    }
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < old_ਮਾਪ; i++)
      old_ਚੀਜ਼ਾਂ[i].~ਵਸਤੂ_t ();

    hb_free (old_ਚੀਜ਼ਾਂ);

    ਵਾਪਸ ਸੱਚ;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਤਾਰਕਿਕ ਵਿੱਚ_storage (ਅਚਲ T &v) ਅਚਲ
  {
    uintptr_t p = (uintptr_t) std::addressof (v);
    ਵਾਪਸ ਚੀਜ਼ਾਂ &&
	   (uintptr_t) ਚੀਜ਼ਾਂ <= p && p < (uintptr_t) (ਚੀਜ਼ਾਂ + ਮਾਪ ());
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ KK, ਕਿਸਮ_ਨਾਂ VV>
  ਤਾਰਕਿਕ ਸੈੱਟ_with_hash (KK&& key, uint32_t hash, VV&& value, ਤਾਰਕਿਕ overwrite = ਸੱਚ)
  {
    ਜੇ (unlikely (!successful)) ਵਾਪਸ ਝੂਠ;
    ਜੇ (unlikely ((occupancy + occupancy / 2) >= ਨਕਾਬ))
    {
      /* key or value may live in our own storage, eg. m.set (k, m.get (j)).
       * alloc() frees that storage, so copy them out first. */
      ਜੇ (unlikely (ਵਿੱਚ_storage (key) || ਵਿੱਚ_storage (value)))
      {
	K ਕੁੰਜੀ_copy;
	ਕੁੰਜੀ_copy = std::forward<KK> (key);
	V ਮੁੱਲ_copy;
	ਮੁੱਲ_copy = std::forward<VV> (value);
	ਵਾਪਸ ਸੈੱਟ_with_hash (std::move (ਕੁੰਜੀ_copy), hash, std::move (ਮੁੱਲ_copy), overwrite);
      }
      ਜੇ (unlikely (!alloc ())) ਵਾਪਸ ਝੂਠ;
    }

    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ tombstone = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) -1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = hash % ਅਭਾਜ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾਈ = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਕਦਮ = 0;
    ਜਦੋਂ (ਚੀਜ਼ਾਂ[i].is_ਵਰਤਿਆ_2 ())
    {
      ਜੇ ((std::is_integral<K>::value || ਚੀਜ਼ਾਂ[i].hash == hash) &&
	  ਚੀਜ਼ਾਂ[i] == key)
      {
        ਜੇ (!overwrite)
	  ਵਾਪਸ ਝੂਠ;
        ਨਹੀਂਤਾਂ
	  ਰੋਕੋ;
      }
      ਜੇ (!ਚੀਜ਼ਾਂ[i].is_real () && tombstone == (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1)
        tombstone = i;
      i = (i + ++ਕਦਮ) & ਨਕਾਬ;
      ਲੰਮਾਈ++;
    }

    ਵਸਤੂ_t &ਵਸਤੂ = ਚੀਜ਼ਾਂ[tombstone == (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1 ? i : tombstone];

    ਜੇ (ਵਸਤੂ.is_ਵਰਤਿਆ_2 ())
    {
      occupancy--;
      population -= ਵਸਤੂ.is_real ();
    }

    ਵਸਤੂ.key = std::forward<KK> (key);
    ਵਸਤੂ.value = std::forward<VV> (value);
    ਵਸਤੂ.hash = hash;
    ਵਸਤੂ.ਸੈੱਟ_ਵਰਤਿਆ (ਸੱਚ);
    ਵਸਤੂ.ਸੈੱਟ_real (ਸੱਚ);

    occupancy++;
    population++;

    ਜੇ (unlikely (ਲੰਮਾਈ > max_chain_ਲੰਮਾਈ) && occupancy > ਨਕਾਬ / 8)
      alloc (ਨਕਾਬ - 8); // This ensures we jump to next larger size

    ਵਾਪਸ ਸੱਚ;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ VV>
  ਤਾਰਕਿਕ ਸੈੱਟ (ਅਚਲ K &key, VV&& value, ਤਾਰਕਿਕ overwrite = ਸੱਚ) { ਵਾਪਸ ਸੈੱਟ_with_hash (key, hb_hash (key), std::forward<VV> (value), overwrite); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ VV>
  ਤਾਰਕਿਕ ਸੈੱਟ (K &&key, VV&& value, ਤਾਰਕਿਕ overwrite = ਸੱਚ)
  {
    uint32_t hash = hb_hash (key);
    ਵਾਪਸ ਸੈੱਟ_with_hash (std::move (key), hash, std::forward<VV> (value), overwrite);
  }
  ਤਾਰਕਿਕ ਜੋੜੋ (ਅਚਲ K &key)
  {
    uint32_t hash = hb_hash (key);
    ਵਾਪਸ ਸੈੱਟ_with_hash (key, hash, ਵਸਤੂ_t::default_ਮੁੱਲ ());
  }

  ਅਚਲ V& get_with_hash (ਅਚਲ K &key, uint32_t hash) ਅਚਲ
  {
    ਜੇ (!ਚੀਜ਼ਾਂ) ਵਾਪਸ ਵਸਤੂ_t::default_ਮੁੱਲ ();
    ਸਵੈਚਾਲਿਤ *ਵਸਤੂ = fetch_ਵਸਤੂ (key, hash);
    ਜੇ (ਵਸਤੂ)
      ਵਾਪਸ ਵਸਤੂ->value;
    ਵਾਪਸ ਵਸਤੂ_t::default_ਮੁੱਲ ();
  }
  ਅਚਲ V& get (ਅਚਲ K &key) ਅਚਲ
  {
    ਜੇ (!ਚੀਜ਼ਾਂ) ਵਾਪਸ ਵਸਤੂ_t::default_ਮੁੱਲ ();
    ਵਾਪਸ get_with_hash (key, hb_hash (key));
  }

  ਖਾਲੀ del (ਅਚਲ K &key)
  {
    ਜੇ (!ਚੀਜ਼ਾਂ) ਵਾਪਸ;
    ਸਵੈਚਾਲਿਤ *ਵਸਤੂ = fetch_ਵਸਤੂ (key, hb_hash (key));
    ਜੇ (ਵਸਤੂ)
    {
      ਵਸਤੂ->ਸੈੱਟ_real (ਝੂਠ);
      population--;
    }
  }

  /* Has interface. */
  ਅਚਲ V& ਸੰਚਾਲਕ [] (K k) ਅਚਲ { ਵਾਪਸ get (k); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ VV=V>
  ਤਾਰਕਿਕ has (ਅਚਲ K &key, VV **vp = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਜੇ (!ਚੀਜ਼ਾਂ) ਵਾਪਸ ਝੂਠ;
    ਵਾਪਸ has_with_hash (key, hb_hash (key), vp);
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ VV=V>
  ਤਾਰਕਿਕ has_with_hash (ਅਚਲ K &key, uint32_t hash, VV **vp = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਜੇ (!ਚੀਜ਼ਾਂ) ਵਾਪਸ ਝੂਠ;
    ਸਵੈਚਾਲਿਤ *ਵਸਤੂ = fetch_ਵਸਤੂ (key, hash);
    ਜੇ (ਵਸਤੂ)
    {
      ਜੇ (vp) *vp = std::addressof (ਵਸਤੂ->value);
      ਵਾਪਸ ਸੱਚ;
    }
    ਵਾਪਸ ਝੂਠ;
  }
  ਵਸਤੂ_t *fetch_ਵਸਤੂ (ਅਚਲ K &key, uint32_t hash) ਅਚਲ
  {
    hash &= 0x3FFFFFFF; // We only store lower 30bit of hash
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = hash % ਅਭਾਜ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਕਦਮ = 0;
    ਜਦੋਂ (ਚੀਜ਼ਾਂ[i].is_ਵਰਤਿਆ_2 ())
    {
      ਜੇ ((std::is_integral<K>::value || ਚੀਜ਼ਾਂ[i].hash == hash) &&
	  ਚੀਜ਼ਾਂ[i] == key)
      {
	ਜੇ (ਚੀਜ਼ਾਂ[i].is_real ())
	  ਵਾਪਸ &ਚੀਜ਼ਾਂ[i];
	ਨਹੀਂਤਾਂ
	  ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
      }
      i = (i + ++ਕਦਮ) & ਨਕਾਬ;
    }
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }
  /* Projection. */
  ਅਚਲ V& ਸੰਚਾਲਕ () (K k) ਅਚਲ { ਵਾਪਸ get (k); }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ () ਅਚਲ { ਵਾਪਸ ਨਕਾਬ ? ਨਕਾਬ + 1 : 0; }

  ਖਾਲੀ clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (zero-initialized) from any writes.  Any non-empty hashmap
     * is a real heap instance with writable items, so clearing
     * under !successful is safe. */
    ਜੇ (!population && !occupancy) ਵਾਪਸ;

    ਲਈ (ਸਵੈਚਾਲਿਤ &_ : hb_iter (ਚੀਜ਼ਾਂ, ਮਾਪ ()))
    {
      /* Reconstruct items. */
      _.~ਵਸਤੂ_t ();
      ਨਵਾਂ (&_) ਵਸਤੂ_t ();
    }

    population = occupancy = 0;
  }

  ਤਾਰਕਿਕ is_empty () ਅਚਲ { ਵਾਪਸ population == 0; }
  ਸਪਸ਼ਟ ਸੰਚਾਲਕ ਤਾਰਕਿਕ () ਅਚਲ { ਵਾਪਸ !is_empty (); }

  uint32_t hash () ਅਚਲ
  {
    ਵਾਪਸ
    + iter_ਚੀਜ਼ਾਂ ()
    | hb_reduce ([] (uint32_t h, ਅਚਲ ਵਸਤੂ_t &_) { ਵਾਪਸ h ^ _.ਕੁੱਲ_hash (); }, (uint32_t) 0u)
    ;
  }

  ਤਾਰਕਿਕ is_equal (ਅਚਲ hb_hashmap_t &other) ਅਚਲ
  {
    ਜੇ (population != other.population) ਵਾਪਸ ਝੂਠ;

    ਲਈ (ਸਵੈਚਾਲਿਤ pair : iter ())
      ਜੇ (other.get (pair.ਪਹਿਲਾ) != pair.second)
        ਵਾਪਸ ਝੂਠ;

    ਵਾਪਸ ਸੱਚ;
  }
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ hb_hashmap_t &other) ਅਚਲ { ਵਾਪਸ is_equal (other); }
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ hb_hashmap_t &other) ਅਚਲ { ਵਾਪਸ !is_equal (other); }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_population () ਅਚਲ { ਵਾਪਸ population; }

  ਖਾਲੀ update (ਅਚਲ hb_hashmap_t &other)
  {
    ਜੇ (unlikely (!successful)) ਵਾਪਸ;

    hb_copy (other, *ਇਹ);
  }

  /*
   * Iterator
   */

  ਸਵੈਚਾਲਿਤ iter_ਚੀਜ਼ਾਂ () ਅਚਲ HB_AUTO_RETURN
  (
    + hb_iter (ਚੀਜ਼ਾਂ, ਇਹ->ਮਾਪ ())
    | hb_filter (&ਵਸਤੂ_t::is_real)
  )
  ਸਵੈਚਾਲਿਤ iter_ref () ਅਚਲ HB_AUTO_RETURN
  (
    + ਇਹ->iter_ਚੀਜ਼ਾਂ ()
    | hb_map (&ਵਸਤੂ_t::get_pair_ref)
  )
  ਸਵੈਚਾਲਿਤ iter () ਅਚਲ HB_AUTO_RETURN
  (
    + ਇਹ->iter_ਚੀਜ਼ਾਂ ()
    | hb_map (&ਵਸਤੂ_t::get_pair)
  )
  ਸਵੈਚਾਲਿਤ ਕੁੰਜੀਆਂ_ref () ਅਚਲ HB_AUTO_RETURN
  (
    + ਇਹ->iter_ਚੀਜ਼ਾਂ ()
    | hb_map (&ਵਸਤੂ_t::get_ਕੁੰਜੀ)
  )
  ਸਵੈਚਾਲਿਤ ਕੁੰਜੀਆਂ () ਅਚਲ HB_AUTO_RETURN
  (
    + ਇਹ->ਕੁੰਜੀਆਂ_ref ()
    | hb_map (hb_ridentity)
  )
  ਸਵੈਚਾਲਿਤ ਮੁੱਲਾਂ_ref () ਅਚਲ HB_AUTO_RETURN
  (
    + ਇਹ->iter_ਚੀਜ਼ਾਂ ()
    | hb_map (&ਵਸਤੂ_t::get_ਮੁੱਲ)
  )
  ਸਵੈਚਾਲਿਤ values () ਅਚਲ HB_AUTO_RETURN
  (
    + ਇਹ->ਮੁੱਲਾਂ_ref ()
    | hb_map (hb_ridentity)
  )

  /* C iterator. */
  ਤਾਰਕਿਕ next (ਪੂਰਨ *idx,
	     K *key,
	     V *value) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ i = (ਬਿਨਾਨਿਸ਼ਾਨੀ) (*idx + 1);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = ਮਾਪ ();
    ਜਦੋਂ (i < ਗਿਣਤੀ_2 && !ਚੀਜ਼ਾਂ[i].is_real ())
      i++;

    ਜੇ (i >= ਗਿਣਤੀ_2)
    {
      *idx = -1;
      ਵਾਪਸ ਝੂਠ;
    }

    *key = ਚੀਜ਼ਾਂ[i].key;
    *value = ਚੀਜ਼ਾਂ[i].value;

    *idx = (ਨਿਸ਼ਾਨੀਵਾਲਾ) i;
    ਵਾਪਸ ਸੱਚ;
  }

  /* Sink interface. */
  hb_hashmap_t& ਸੰਚਾਲਕ << (ਅਚਲ hb_pair_t<K, V>& v)
  { ਸੈੱਟ (v.ਪਹਿਲਾ, v.second); ਵਾਪਸ *ਇਹ; }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (V2))>
  hb_hashmap_t& ਸੰਚਾਲਕ << (ਅਚਲ hb_pair_t<K, V&&>& v)
  { ਸੈੱਟ (v.ਪਹਿਲਾ, std::move (v.second)); ਵਾਪਸ *ਇਹ; }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ K2 = K,
	    hb_enable_if (!hb_is_trivially_copyable (K2))>
  hb_hashmap_t& ਸੰਚਾਲਕ << (ਅਚਲ hb_pair_t<K&&, V>& v)
  { ਸੈੱਟ (std::move (v.ਪਹਿਲਾ), v.second); ਵਾਪਸ *ਇਹ; }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ K2 = K, ਕਿਸਮ_ਨਾਂ V2 = V,
	    hb_enable_if (!hb_is_trivially_copyable (K2) &&
			  !hb_is_trivially_copyable (V2))>
  hb_hashmap_t& ਸੰਚਾਲਕ << (ਅਚਲ hb_pair_t<K&&, V&&>& v)
  { ਸੈੱਟ (std::move (v.ਪਹਿਲਾ), std::move (v.second)); ਵਾਪਸ *ਇਹ; }

  ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅਭਾਜ_for (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਖਿਸਕਾਓ)
  {
    /* Following comment and table copied from glib. */
    /* Each table size has an associated prime modulo (the first prime
     * lower than the table size) used to find the initial bucket. Probing
     * then works modulo 2^n. The prime modulo is necessary to get a
     * good distribution with poor hash functions.
     */
    /* Not declaring static to make all kinds of compilers happy... */
    /*static*/ ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅਭਾਜ_mod [32] =
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

    ਜੇ (unlikely (ਖਿਸਕਾਓ >= ARRAY_ਲੰਮਾਈ (ਅਭਾਜ_mod)))
      ਵਾਪਸ ਅਭਾਜ_mod[ARRAY_ਲੰਮਾਈ (ਅਭਾਜ_mod) - 1];

    ਵਾਪਸ ਅਭਾਜ_mod[ਖਿਸਕਾਓ];
  }
};

/*
 * hb_map_t
 */

ਬਣਤਰ hb_map_t : hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       ਸੱਚ>
{
  ਵਰਤੋਂ hashmap = hb_hashmap_t<hb_codepoint_t,
			       hb_codepoint_t,
			       ਸੱਚ>;

  ~hb_map_t () = ਮੂਲ;
  hb_map_t () : hashmap () {}
  hb_map_t (ਅਚਲ hb_map_t &o) : hashmap ((hashmap &) o) {}
  hb_map_t (hb_map_t &&o)  noexcept : hashmap (std::move ((hashmap &) o)) {}
  hb_map_t& ਸੰਚਾਲਕ= (ਅਚਲ hb_map_t&) = ਮੂਲ;
  hb_map_t& ਸੰਚਾਲਕ= (hb_map_t&&) = ਮੂਲ;
  hb_map_t (std::initializer_list<hb_codepoint_pair_t> lst) : hashmap (lst) {}
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_map_t (ਅਚਲ Iterable &o) : hashmap (o) {}
};


#ਜੇ_ਅੰਤ /* HB_MAP_HH */

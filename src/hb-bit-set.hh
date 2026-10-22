/*
 * Copyright © 2012,2017  Google, Inc.
 * Copyright © 2021 Behdad Esfahbod
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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-bit-page.hh"

ਬਣਤਰ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t
{
  hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t () = ਮੂਲ;
  ~hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t () = ਮੂਲ;

  hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t (ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t& other) : hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t () { ਸੈੱਟ (other, ਸੱਚ); }
  hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t ( hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t&& other)  noexcept : hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t () { hb_ਅਦਲਾ_ਬਦਲੀ (*ਇਹ, other); }
  hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t& ਸੰਚਾਲਕ= (ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t& other) { ਸੈੱਟ (other); ਵਾਪਸ *ਇਹ; }
  hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t& ਸੰਚਾਲਕ= (hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t&& other)  noexcept { hb_ਅਦਲਾ_ਬਦਲੀ (*ਇਹ, other); ਵਾਪਸ *ਇਹ; }
  ਮਿੱਤਰ ਖਾਲੀ swap (hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t &a, hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t &b) noexcept
  {
    ਜੇ (likely (!a.successful || !b.successful))
      ਵਾਪਸ;
    hb_ਅਦਲਾ_ਬਦਲੀ (a.population, b.population);
    hb_ਅਦਲਾ_ਬਦਲੀ (a.ਆਖਰੀ_ਪੰਨਾ_lookup, b.ਆਖਰੀ_ਪੰਨਾ_lookup);
    hb_ਅਦਲਾ_ਬਦਲੀ (a.ਪੰਨਾ_map, b.ਪੰਨਾ_map);
    hb_ਅਦਲਾ_ਬਦਲੀ (a.ਪੰਨੇ, b.ਪੰਨੇ);
  }

  ਖਾਲੀ init ()
  {
    successful = ਸੱਚ;
    population = 0;
    ਆਖਰੀ_ਪੰਨਾ_lookup = 0;
    ਪੰਨਾ_map.init ();
    ਪੰਨੇ.init ();
  }
  ਖਾਲੀ fini ()
  {
    ਪੰਨਾ_map.fini ();
    ਪੰਨੇ.fini ();
  }

  ਵਰਤੋਂ ਪੰਨਾ_t = hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t;
  ਬਣਤਰ ਪੰਨਾ_map_t
  {
    ਪੂਰਨ cmp (ਅਚਲ ਪੰਨਾ_map_t &o) ਅਚਲ { ਵਾਪਸ cmp (o.major); }
    ਪੂਰਨ cmp (uint32_t o_major) ਅਚਲ { ਵਾਪਸ (ਪੂਰਨ) o_major - (ਪੂਰਨ) major; }

    uint32_t major;
    uint32_t ਸੂਚਕਾਂਕ_2;
  };

  ਤਾਰਕਿਕ successful = ਸੱਚ; /* Allocations successful */
  ਬਦਲਣਯੋਗ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ population = 0;
  ਬਦਲਣਯੋਗ hb_atomic_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> ਆਖਰੀ_ਪੰਨਾ_lookup = 0;
  hb_sorted_ਸਦਿਸ਼_t<ਪੰਨਾ_map_t> ਪੰਨਾ_map;
  hb_ਸਦਿਸ਼_t<ਪੰਨਾ_t> ਪੰਨੇ;

  ਖਾਲੀ err () { ਜੇ (successful) successful = ਝੂਠ; } /* TODO Remove */
  ਤਾਰਕਿਕ ਵਿੱਚ_error () ਅਚਲ { ਵਾਪਸ !successful; }

  ਤਾਰਕਿਕ resize (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2, ਤਾਰਕਿਕ clear = ਸੱਚ, ਤਾਰਕਿਕ exact_ਮਾਪ = ਝੂਠ)
  {
    ਜੇ (unlikely (!successful)) ਵਾਪਸ ਝੂਠ;

    ਜੇ (ਪੰਨੇ.ਲੰਮਾਈ < ਗਿਣਤੀ_2 && (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਪੰਨੇ.allocated < ਗਿਣਤੀ_2 && ਗਿਣਤੀ_2 <= 2)
      exact_ਮਾਪ = ਸੱਚ; // Most sets are small and local

    ਜੇ (unlikely (!ਪੰਨੇ.resize_ਭਰਿਆ (ਗਿਣਤੀ_2, clear, exact_ਮਾਪ) ||
	!ਪੰਨਾ_map.resize_ਭਰਿਆ (ਗਿਣਤੀ_2, clear, ਝੂਠ)))
    {
      ਪੰਨੇ.resize_ਭਰਿਆ (ਪੰਨਾ_map.ਲੰਮਾਈ, clear, exact_ਮਾਪ);
      successful = ਝੂਠ;
      ਵਾਪਸ ਝੂਠ;
    }
    ਵਾਪਸ ਸੱਚ;
  }

  ਖਾਲੀ alloc (ਬਿਨਾਨਿਸ਼ਾਨੀ sz)
  {
    sz >>= (ਪੰਨਾ_t::ਪੰਨਾ_ਬਿੱਟ_LOG_2 - 1);
    ਪੰਨੇ.alloc (sz);
    ਪੰਨਾ_map.alloc (sz);
  }

  hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t& reset ()
  {
    successful = ਸੱਚ;
    clear ();
    ਵਾਪਸ *ਇਹ;
  }

  ਖਾਲੀ clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (which is zero-initialized) from any writes.  Any non-empty
     * instance is a real heap object with writable storage, so
     * clearing through the vector's always-safe clear() is fine
     * even if we entered error state. */
    ਜੇ (!ਪੰਨੇ.ਲੰਮਾਈ && !population) ਵਾਪਸ;
    ਪੰਨੇ.clear ();
    ਪੰਨਾ_map.clear ();
    population = 0;
  }
  ਤਾਰਕਿਕ is_empty () ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਪੰਨੇ.ਲੰਮਾਈ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      ਜੇ (!ਪੰਨੇ[i].is_empty ())
	ਵਾਪਸ ਝੂਠ;
    ਵਾਪਸ ਸੱਚ;
  }
  ਸਪਸ਼ਟ ਸੰਚਾਲਕ ਤਾਰਕਿਕ () ਅਚਲ { ਵਾਪਸ !is_empty (); }

  uint32_t hash () ਅਚਲ
  {
    uint32_t h = 0;
    ਲਈ (ਸਵੈਚਾਲਿਤ &map : ਪੰਨਾ_map)
    {
      ਸਵੈਚਾਲਿਤ &ਪੰਨਾ = ਪੰਨੇ.arrayZ[map.ਸੂਚਕਾਂਕ_2];
      ਜੇ (unlikely (ਪੰਨਾ.is_empty ())) ਜਾਰੀ;
      h = h * 31 + hb_hash (map.major) + hb_hash (ਪੰਨਾ);
    }
    ਵਾਪਸ h;
  }

  ਨਿਜੀ:
  ਖਾਲੀ dirty () { population = UINT_MAX; }
  ਜਨਤਕ:

  ਖਾਲੀ ਜੋੜੋ (hb_codepoint_t g)
  {
    ਜੇ (unlikely (!successful)) ਵਾਪਸ;
    ਜੇ (unlikely (g == INVALID)) ਵਾਪਸ;
    dirty ();
    ਪੰਨਾ_t *ਪੰਨਾ = ਪੰਨਾ_for (g, ਸੱਚ); ਜੇ (unlikely (!ਪੰਨਾ)) ਵਾਪਸ;
    ਪੰਨਾ->ਜੋੜੋ (g);
  }
  ਖਾਲੀ ਜੋੜੋ_ਬਿੱਟ (hb_codepoint_t g, uint64_t ਬਿੱਟ)
  {
    ਜੇ (unlikely (!successful) || unlikely (!ਬਿੱਟ)) ਵਾਪਸ;
    ਪ੍ਰਤਿਜ੍ਞਾ (!(g & (ਪੰਨਾ_t::ELT_ਬਿੱਟ - 1)));
    dirty ();
    ਪੰਨਾ_t *ਪੰਨਾ = ਪੰਨਾ_for (g, ਸੱਚ); ਜੇ (unlikely (!ਪੰਨਾ)) ਵਾਪਸ;
    ਪੰਨਾ->ਜੋੜੋ_ਬਿੱਟ (g, ਬਿੱਟ);
  }
  ਤਾਰਕਿਕ ਜੋੜੋ_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    ਜੇ (unlikely (!successful)) ਵਾਪਸ ਸੱਚ; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    ਜੇ (unlikely (a > b || a == INVALID || b == INVALID)) ਵਾਪਸ ਝੂਠ;
    dirty ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ma = get_major (a);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ mb = get_major (b);
    ਜੇ (ma == mb)
    {
      ਪੰਨਾ_t *ਪੰਨਾ = ਪੰਨਾ_for (a, ਸੱਚ); ਜੇ (unlikely (!ਪੰਨਾ)) ਵਾਪਸ ਝੂਠ;
      ਪੰਨਾ->ਜੋੜੋ_range (a, b);
    }
    ਨਹੀਂਤਾਂ
    {
      ਪੰਨਾ_t *ਪੰਨਾ = ਪੰਨਾ_for (a, ਸੱਚ); ਜੇ (unlikely (!ਪੰਨਾ)) ਵਾਪਸ ਝੂਠ;
      ਪੰਨਾ->ਜੋੜੋ_range (a, major_ਸ਼ੁਰੂ (ma + 1) - 1);

      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ m = ma + 1; m < mb; m++)
      {
	ਪੰਨਾ = ਪੰਨਾ_for (major_ਸ਼ੁਰੂ (m), ਸੱਚ); ਜੇ (unlikely (!ਪੰਨਾ)) ਵਾਪਸ ਝੂਠ;
	ਪੰਨਾ->init1 ();
      }

      ਪੰਨਾ = ਪੰਨਾ_for (b, ਸੱਚ); ਜੇ (unlikely (!ਪੰਨਾ)) ਵਾਪਸ ਝੂਠ;
      ਪੰਨਾ->ਜੋੜੋ_range (major_ਸ਼ੁਰੂ (mb), b);
    }
    ਵਾਪਸ ਸੱਚ;
  }

  /* Duplicated here from hb-machinery.hh to avoid including it. */
  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Type>
  ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ Type& Struct_At_ਵਿੱਥ_Unaligned(ਅਚਲ ਖਾਲੀ *P, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ)
  {
#ਹਦਾਇਤ GCC diagnostic push
#ਹਦਾਇਤ GCC diagnostic ignored "-Wcast-align"
    ਵਾਪਸ * reinterpret_cast<ਅਚਲ Type*> ((ਅਚਲ ਅੱਖਰ *) P + ਵਿੱਥ);
#ਹਦਾਇਤ GCC diagnostic ਕੱਢੋ
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਖਾਲੀ ਸੈੱਟ_array (ਤਾਰਕਿਕ v, ਅਚਲ T *array, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ stride=ਆਕਾਰ(T))
  {
    ਜੇ (unlikely (!successful)) ਵਾਪਸ;
    ਜੇ (!ਗਿਣਤੀ_2) ਵਾਪਸ;
    dirty ();
    hb_codepoint_t g = *array;
    ਜਦੋਂ (ਗਿਣਤੀ_2)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ m = get_major (g);
      ਪੰਨਾ_t *ਪੰਨਾ = ਪੰਨਾ_for (g, v); ਜੇ (unlikely (v && !ਪੰਨਾ)) ਵਾਪਸ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ = major_ਸ਼ੁਰੂ (m);
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ = major_ਸ਼ੁਰੂ (m + 1);
      ਕਰੋ
      {
        ਜੇ (g != INVALID && (v || ਪੰਨਾ)) /* The v check is to optimize out the page check if v is true. */
	  ਪੰਨਾ->ਸੈੱਟ (g, v);

	array = &Struct_At_ਵਿੱਥ_Unaligned<T> (array, stride);
	ਗਿਣਤੀ_2--;
      }
      ਜਦੋਂ (ਗਿਣਤੀ_2 && (g = *array, ਸ਼ੁਰੂ <= g && g < ਸਿਰਾ));
    }
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਖਾਲੀ ਜੋੜੋ_array (ਅਚਲ T *array, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ stride=ਆਕਾਰ(T))
  { ਸੈੱਟ_array (ਸੱਚ, array, ਗਿਣਤੀ_2, stride); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਖਾਲੀ ਜੋੜੋ_array (ਅਚਲ hb_array_t<ਅਚਲ T>& arr) { ਜੋੜੋ_array (&arr, arr.len ()); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਖਾਲੀ del_array (ਅਚਲ T *array, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ stride=ਆਕਾਰ(T))
  { ਸੈੱਟ_array (ਝੂਠ, array, ਗਿਣਤੀ_2, stride); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਖਾਲੀ del_array (ਅਚਲ hb_array_t<ਅਚਲ T>& arr) { del_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਤਾਰਕਿਕ ਸੈੱਟ_sorted_array (ਤਾਰਕਿਕ v, ਅਚਲ T *array, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ stride=ਆਕਾਰ(T))
  {
    ਜੇ (unlikely (!successful)) ਵਾਪਸ ਸੱਚ; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    ਜੇ (unlikely (!ਗਿਣਤੀ_2)) ਵਾਪਸ ਸੱਚ;
    dirty ();
    hb_codepoint_t g = *array;
    hb_codepoint_t ਆਖਰੀ_g = g;
    ਜਦੋਂ (ਗਿਣਤੀ_2)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ m = get_major (g);
      ਪੰਨਾ_t *ਪੰਨਾ = ਪੰਨਾ_for (g, v); ਜੇ (unlikely (v && !ਪੰਨਾ)) ਵਾਪਸ ਝੂਠ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ = major_ਸ਼ੁਰੂ (m + 1);
      ਕਰੋ
      {
	/* If we try harder we can change the following comparison to <=;
	 * Not sure if it's worth it. */
	ਜੇ (g < ਆਖਰੀ_g) ਵਾਪਸ ਝੂਠ;
	ਆਖਰੀ_g = g;

        ਜੇ (g != INVALID && (v || ਪੰਨਾ)) /* The v check is to optimize out the page check if v is true. */
	  ਪੰਨਾ->ਸੈੱਟ (g, v);

	array = &Struct_At_ਵਿੱਥ_Unaligned<T> (array, stride);
	ਗਿਣਤੀ_2--;
      }
      ਜਦੋਂ (ਗਿਣਤੀ_2 && (g = *array, g < ਸਿਰਾ));
    }
    ਵਾਪਸ ਸੱਚ;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਤਾਰਕਿਕ ਜੋੜੋ_sorted_array (ਅਚਲ T *array, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ stride=ਆਕਾਰ(T))
  { ਵਾਪਸ ਸੈੱਟ_sorted_array (ਸੱਚ, array, ਗਿਣਤੀ_2, stride); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਤਾਰਕਿਕ ਜੋੜੋ_sorted_array (ਅਚਲ hb_sorted_array_t<ਅਚਲ T>& arr) { ਵਾਪਸ ਜੋੜੋ_sorted_array (&arr, arr.len ()); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਤਾਰਕਿਕ del_sorted_array (ਅਚਲ T *array, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ stride=ਆਕਾਰ(T))
  { ਵਾਪਸ ਸੈੱਟ_sorted_array (ਝੂਠ, array, ਗਿਣਤੀ_2, stride); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਤਾਰਕਿਕ del_sorted_array (ਅਚਲ hb_sorted_array_t<ਅਚਲ T>& arr) { ਵਾਪਸ del_sorted_array (&arr, arr.len ()); }

  ਖਾਲੀ del (hb_codepoint_t g)
  {
    ਜੇ (unlikely (!successful)) ਵਾਪਸ;
    ਪੰਨਾ_t *ਪੰਨਾ = ਪੰਨਾ_for (g);
    ਜੇ (!ਪੰਨਾ)
      ਵਾਪਸ;
    dirty ();
    ਪੰਨਾ->del (g);
  }
  ਖਾਲੀ del_ਬਿੱਟ (hb_codepoint_t g, uint64_t ਬਿੱਟ)
  {
    ਜੇ (unlikely (!successful) || unlikely (!ਬਿੱਟ)) ਵਾਪਸ;
    ਪ੍ਰਤਿਜ੍ਞਾ (!(g & (ਪੰਨਾ_t::ELT_ਬਿੱਟ - 1)));
    ਪੰਨਾ_t *ਪੰਨਾ = ਪੰਨਾ_for (g);
    ਜੇ (!ਪੰਨਾ)
      ਵਾਪਸ;
    dirty ();
    ਪੰਨਾ->del_ਬਿੱਟ (g, ਬਿੱਟ);
  }

  ਨਿਜੀ:
  ਖਾਲੀ del_ਪੰਨੇ (ਪੂਰਨ ds, ਪੂਰਨ de)
  {
    ਜੇ (ds <= de)
    {
      // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
      // before attempting to rewrite the page map.
      hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> compact_workspace;
      ਜੇ (unlikely (!allocate_compact_workspace (compact_workspace))) ਵਾਪਸ;

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ write_ਸੂਚਕਾਂਕ = 0;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਪੰਨਾ_map.ਲੰਮਾਈ; i++)
      {
	ਪੂਰਨ m = (ਪੂਰਨ) ਪੰਨਾ_map.arrayZ[i].major;
	ਜੇ (m < ds || de < m)
	  ਪੰਨਾ_map.arrayZ[write_ਸੂਚਕਾਂਕ++] = ਪੰਨਾ_map.arrayZ[i];
      }
      compact (compact_workspace, write_ਸੂਚਕਾਂਕ);
      resize (write_ਸੂਚਕਾਂਕ);
    }
  }


  ਜਨਤਕ:
  ਖਾਲੀ del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    ਜੇ (unlikely (!successful)) ਵਾਪਸ;
    ਜੇ (unlikely (a > b || a == INVALID)) ਵਾਪਸ;
    dirty ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ma = get_major (a);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ mb = get_major (b);
    /* Delete pages from ds through de if ds <= de. */
    ਪੂਰਨ ds = (a == major_ਸ਼ੁਰੂ (ma))? (ਪੂਰਨ) ma: (ਪੂਰਨ) (ma + 1);
    ਪੂਰਨ de = (b + 1 == major_ਸ਼ੁਰੂ (mb + 1))? (ਪੂਰਨ) mb: ((ਪੂਰਨ) mb - 1);
    ਜੇ (ds > de || (ਪੂਰਨ) ma < ds)
    {
      ਪੰਨਾ_t *ਪੰਨਾ = ਪੰਨਾ_for (a);
      ਜੇ (ਪੰਨਾ)
      {
	ਜੇ (ma == mb)
	  ਪੰਨਾ->del_range (a, b);
	ਨਹੀਂਤਾਂ
	  ਪੰਨਾ->del_range (a, major_ਸ਼ੁਰੂ (ma + 1) - 1);
      }
    }
    ਜੇ (de < (ਪੂਰਨ) mb && ma != mb)
    {
      ਪੰਨਾ_t *ਪੰਨਾ = ਪੰਨਾ_for (b);
      ਜੇ (ਪੰਨਾ)
	ਪੰਨਾ->del_range (major_ਸ਼ੁਰੂ (mb), b);
    }
    del_ਪੰਨੇ (ds, de);
  }

  ਤਾਰਕਿਕ get (hb_codepoint_t g) ਅਚਲ
  {
    ਅਚਲ ਪੰਨਾ_t *ਪੰਨਾ = ਪੰਨਾ_for (g);
    ਜੇ (!ਪੰਨਾ)
      ਵਾਪਸ ਝੂਠ;
    ਵਾਪਸ ਪੰਨਾ->get (g);
  }
  ਤਾਰਕਿਕ may_have (hb_codepoint_t g) ਅਚਲ { ਵਾਪਸ get (g); }

  /* Has interface. */
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ [] (hb_codepoint_t k) ਅਚਲ { ਵਾਪਸ get (k); }
  ਤਾਰਕਿਕ has (hb_codepoint_t k) ਅਚਲ { ਵਾਪਸ (*ਇਹ)[k]; }
  /* Predicate. */
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ () (hb_codepoint_t k) ਅਚਲ { ਵਾਪਸ has (k); }

  /* Sink interface. */
  hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t& ਸੰਚਾਲਕ << (hb_codepoint_t v)
  { ਜੋੜੋ (v); ਵਾਪਸ *ਇਹ; }
  hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t& ਸੰਚਾਲਕ << (ਅਚਲ hb_codepoint_pair_t& range)
  { ਜੋੜੋ_range (range.ਪਹਿਲਾ, range.second); ਵਾਪਸ *ਇਹ; }

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t &other) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ na = ਪੰਨੇ.ਲੰਮਾਈ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nb = other.ਪੰਨੇ.ਲੰਮਾਈ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ a = 0, b = 0;
    ਲਈ (; a < na && b < nb; )
    {
      ਜੇ (ਪੰਨਾ_map.arrayZ[a].major == other.ਪੰਨਾ_map.arrayZ[b].major)
      {
	ਜੇ (ਪੰਨਾ_at (a).intersects (other.ਪੰਨਾ_at (b)))
	  ਵਾਪਸ ਸੱਚ;
	a++;
	b++;
      }
      ਨਹੀਂਤਾਂ ਜੇ (ਪੰਨਾ_map.arrayZ[a].major < other.ਪੰਨਾ_map.arrayZ[b].major)
	a++;
      ਨਹੀਂਤਾਂ
	b++;
    }
    ਵਾਪਸ ਝੂਠ;
  }
  ਤਾਰਕਿਕ may_intersect (ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t &other) ਅਚਲ
  { ਵਾਪਸ intersects (other); }

  ਤਾਰਕਿਕ intersects (hb_codepoint_t ਪਹਿਲਾ, hb_codepoint_t ਆਖਰੀ) ਅਚਲ
  {
    hb_codepoint_t c = ਪਹਿਲਾ - 1;
    ਵਾਪਸ next (&c) && c <= ਆਖਰੀ;
  }
  ਖਾਲੀ ਸੈੱਟ (ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t &other, ਤਾਰਕਿਕ exact_ਮਾਪ = ਝੂਠ)
  {
    ਜੇ (unlikely (!successful)) ਵਾਪਸ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = other.ਪੰਨੇ.ਲੰਮਾਈ;
    ਜੇ (unlikely (!resize  (ਗਿਣਤੀ_2, ਝੂਠ, exact_ਮਾਪ)))
      ਵਾਪਸ;
    population = other.population;

    ਪੰਨਾ_map = other.ਪੰਨਾ_map;
    ਪੰਨੇ = other.ਪੰਨੇ;
  }

  ਤਾਰਕਿਕ is_equal (ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t &other) ਅਚਲ
  {
    ਜੇ (has_population () && other.has_population () &&
	population != other.population)
      ਵਾਪਸ ਝੂਠ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ na = ਪੰਨੇ.ਲੰਮਾਈ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nb = other.ਪੰਨੇ.ਲੰਮਾਈ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ a = 0, b = 0;
    ਲਈ (; a < na && b < nb; )
    {
      ਜੇ (ਪੰਨਾ_at (a).is_empty ()) { a++; ਜਾਰੀ; }
      ਜੇ (other.ਪੰਨਾ_at (b).is_empty ()) { b++; ਜਾਰੀ; }
      ਜੇ (ਪੰਨਾ_map.arrayZ[a].major != other.ਪੰਨਾ_map.arrayZ[b].major ||
	  !ਪੰਨਾ_at (a).is_equal (other.ਪੰਨਾ_at (b)))
	ਵਾਪਸ ਝੂਠ;
      a++;
      b++;
    }
    ਲਈ (; a < na; a++)
      ਜੇ (!ਪੰਨਾ_at (a).is_empty ()) { ਵਾਪਸ ਝੂਠ; }
    ਲਈ (; b < nb; b++)
      ਜੇ (!other.ਪੰਨਾ_at (b).is_empty ()) { ਵਾਪਸ ਝੂਠ; }

    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ is_subset (ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t &larger_ਸੈੱਟ) ਅਚਲ
  {
    ਜੇ (has_population () && larger_ਸੈੱਟ.has_population () &&
	population > larger_ਸੈੱਟ.population)
      ਵਾਪਸ ਝੂਠ;

    uint32_t spi = 0;
    uint32_t lpi = 0;
    ਜਦੋਂ (spi < ਪੰਨਾ_map.ਲੰਮਾਈ && lpi < larger_ਸੈੱਟ.ਪੰਨਾ_map.ਲੰਮਾਈ)
    {
      uint32_t spm = ਪੰਨਾ_map.arrayZ[spi].major;
      uint32_t lpm = larger_ਸੈੱਟ.ਪੰਨਾ_map.arrayZ[lpi].major;
      ਸਵੈਚਾਲਿਤ sp = ਪੰਨਾ_at (spi);

      ਜੇ (spm < lpm) {
        ਜੇ (!sp.is_empty ())
          ਵਾਪਸ ਝੂਠ;
        spi++;
        ਜਾਰੀ;
      }

      ਜੇ (lpm < spm) {
        lpi++;
        ਜਾਰੀ;
      }

      ਸਵੈਚਾਲਿਤ lp = larger_ਸੈੱਟ.ਪੰਨਾ_at (lpi);
      ਜੇ (!sp.is_subset (lp))
        ਵਾਪਸ ਝੂਠ;

      spi++;
      lpi++;
    }

    ਜਦੋਂ (spi < ਪੰਨਾ_map.ਲੰਮਾਈ)
      ਜੇ (!ਪੰਨਾ_at (spi++).is_empty ())
        ਵਾਪਸ ਝੂਠ;

    ਵਾਪਸ ਸੱਚ;
  }

  ਨਿਜੀ:
  ਤਾਰਕਿਕ allocate_compact_workspace (hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ>& workspace)
  {
    ਜੇ (unlikely (!workspace.resize_exact (ਪੰਨੇ.ਲੰਮਾਈ)))
    {
      successful = ਝੂਠ;
      ਵਾਪਸ ਝੂਠ;
    }

    ਵਾਪਸ ਸੱਚ;
  }

  /*
   * workspace should be a pre-sized vector allocated to hold at exactly pages.length
   * elements.
   */
  ਖਾਲੀ compact (hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ>& workspace,
                ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਲੰਮਾਈ)
  {
    ਪ੍ਰਤਿਜ੍ਞਾ(workspace.ਲੰਮਾਈ == ਪੰਨੇ.ਲੰਮਾਈ);
    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ>& old_ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਨਾ_map_ਸੂਚਕਾਂਕ = workspace;

    hb_fill (old_ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਨਾ_map_ਸੂਚਕਾਂਕ.writer(), 0xFFFFFFFF);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਲੰਮਾਈ; i++)
      old_ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਨਾ_map_ਸੂਚਕਾਂਕ[ਪੰਨਾ_map[i].ਸੂਚਕਾਂਕ_2] =  i;

    compact_ਪੰਨੇ (old_ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਨਾ_map_ਸੂਚਕਾਂਕ);
  }
  ਖਾਲੀ compact_ਪੰਨੇ (ਅਚਲ hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ>& old_ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਨਾ_map_ਸੂਚਕਾਂਕ)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ write_ਸੂਚਕਾਂਕ = 0;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਪੰਨੇ.ਲੰਮਾਈ; i++)
    {
      ਜੇ (old_ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਨਾ_map_ਸੂਚਕਾਂਕ[i] == 0xFFFFFFFF) ਜਾਰੀ;

      ਜੇ (write_ਸੂਚਕਾਂਕ < i)
	ਪੰਨੇ[write_ਸੂਚਕਾਂਕ] = ਪੰਨੇ[i];

      ਪੰਨਾ_map[old_ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਨਾ_map_ਸੂਚਕਾਂਕ[i]].ਸੂਚਕਾਂਕ_2 = write_ਸੂਚਕਾਂਕ;
      write_ਸੂਚਕਾਂਕ++;
    }
  }
  ਜਨਤਕ:

  ਖਾਲੀ process_ (hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t::ਸਦਿਸ਼_t (*op) (ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t::ਸਦਿਸ਼_t &, ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t::ਸਦਿਸ਼_t &),
		 ਤਾਰਕਿਕ passthru_ਖੱਬਾ, ਤਾਰਕਿਕ passthru_ਸੱਜਾ,
		 ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t &other,
		 hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> *workspace = ਖਾਲੀ_ਸੂਚਕ)
  {
    ਜੇ (unlikely (!successful)) ਵਾਪਸ;

    dirty ();

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ na = ਪੰਨੇ.ਲੰਮਾਈ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nb = other.ਪੰਨੇ.ਲੰਮਾਈ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅਗਲਾ_ਪੰਨਾ = na;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = 0, new_ਗਿਣਤੀ = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ a = 0, b = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ write_ਸੂਚਕਾਂਕ = 0;

    // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
    // before attempting to rewrite the page map.
    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> local_workspace;
    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> &compact_workspace = workspace ? *workspace : local_workspace;
    ਜੇ (!passthru_ਖੱਬਾ)
    {
      ਤਾਰਕਿਕ allocated = workspace ? compact_workspace.resize (ਪੰਨੇ.ਲੰਮਾਈ)
				 : allocate_compact_workspace (compact_workspace);
      ਜੇ (unlikely (!allocated))
      {
	successful = ਝੂਠ;
	ਵਾਪਸ;
      }
    }

    ਲਈ (; a < na && b < nb; )
    {
      ਜੇ (ਪੰਨਾ_map.arrayZ[a].major == other.ਪੰਨਾ_map.arrayZ[b].major)
      {
	ਜੇ (!passthru_ਖੱਬਾ)
	{
	  // Move page_map entries that we're keeping from the left side set
	  // to the front of the page_map vector. This isn't necessary if
	  // passthru_left is set since no left side pages will be removed
	  // in that case.
	  ਜੇ (write_ਸੂਚਕਾਂਕ < a)
	    ਪੰਨਾ_map.arrayZ[write_ਸੂਚਕਾਂਕ] = ਪੰਨਾ_map.arrayZ[a];
	  write_ਸੂਚਕਾਂਕ++;
	}

	ਗਿਣਤੀ_2++;
	a++;
	b++;
      }
      ਨਹੀਂਤਾਂ ਜੇ (ਪੰਨਾ_map.arrayZ[a].major < other.ਪੰਨਾ_map.arrayZ[b].major)
      {
	ਜੇ (passthru_ਖੱਬਾ)
	  ਗਿਣਤੀ_2++;
	a++;
      }
      ਨਹੀਂਤਾਂ
      {
	ਜੇ (passthru_ਸੱਜਾ)
	  ਗਿਣਤੀ_2++;
	b++;
      }
    }
    ਜੇ (passthru_ਖੱਬਾ)
      ਗਿਣਤੀ_2 += na - a;
    ਜੇ (passthru_ਸੱਜਾ)
      ਗਿਣਤੀ_2 += nb - b;

    ਜੇ (!passthru_ਖੱਬਾ)
    {
      na  = write_ਸੂਚਕਾਂਕ;
      ਅਗਲਾ_ਪੰਨਾ = write_ਸੂਚਕਾਂਕ;
      compact (compact_workspace, write_ਸੂਚਕਾਂਕ);
    }

    ਜੇ (unlikely (!resize (ਗਿਣਤੀ_2)))
      ਵਾਪਸ;

    new_ਗਿਣਤੀ = ਗਿਣਤੀ_2;

    /* Process in-place backward. */
    a = na;
    b = nb;
    ਲਈ (; a && b; )
    {
      ਜੇ (ਪੰਨਾ_map.arrayZ[a - 1].major == other.ਪੰਨਾ_map.arrayZ[b - 1].major)
      {
	a--;
	b--;
	ਗਿਣਤੀ_2--;
	ਪੰਨਾ_map.arrayZ[ਗਿਣਤੀ_2] = ਪੰਨਾ_map.arrayZ[a];
	ਪੰਨਾ_at (ਗਿਣਤੀ_2).v = op (ਪੰਨਾ_at (a).v, other.ਪੰਨਾ_at (b).v);
	ਪੰਨਾ_at (ਗਿਣਤੀ_2).dirty ();
      }
      ਨਹੀਂਤਾਂ ਜੇ (ਪੰਨਾ_map.arrayZ[a - 1].major > other.ਪੰਨਾ_map.arrayZ[b - 1].major)
      {
	a--;
	ਜੇ (passthru_ਖੱਬਾ)
	{
	  ਗਿਣਤੀ_2--;
	  ਪੰਨਾ_map.arrayZ[ਗਿਣਤੀ_2] = ਪੰਨਾ_map.arrayZ[a];
	}
      }
      ਨਹੀਂਤਾਂ
      {
	b--;
	ਜੇ (passthru_ਸੱਜਾ)
	{
	  ਗਿਣਤੀ_2--;
	  ਪੰਨਾ_map.arrayZ[ਗਿਣਤੀ_2].major = other.ਪੰਨਾ_map.arrayZ[b].major;
	  ਪੰਨਾ_map.arrayZ[ਗਿਣਤੀ_2].ਸੂਚਕਾਂਕ_2 = ਅਗਲਾ_ਪੰਨਾ++;
	  ਪੰਨਾ_at (ਗਿਣਤੀ_2) = other.ਪੰਨਾ_at (b);
	}
      }
    }
    ਜੇ (passthru_ਖੱਬਾ)
      ਜਦੋਂ (a)
      {
	a--;
	ਗਿਣਤੀ_2--;
	ਪੰਨਾ_map.arrayZ[ਗਿਣਤੀ_2] = ਪੰਨਾ_map.arrayZ[a];
      }
    ਜੇ (passthru_ਸੱਜਾ)
      ਜਦੋਂ (b)
      {
	b--;
	ਗਿਣਤੀ_2--;
	ਪੰਨਾ_map.arrayZ[ਗਿਣਤੀ_2].major = other.ਪੰਨਾ_map.arrayZ[b].major;
	ਪੰਨਾ_map.arrayZ[ਗਿਣਤੀ_2].ਸੂਚਕਾਂਕ_2 = ਅਗਲਾ_ਪੰਨਾ++;
	ਪੰਨਾ_at (ਗਿਣਤੀ_2) = other.ਪੰਨਾ_at (b);
      }
    ਪ੍ਰਤਿਜ੍ਞਾ (!ਗਿਣਤੀ_2);
    resize (new_ਗਿਣਤੀ);
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Op>
  ਸਥਿਰ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t::ਸਦਿਸ਼_t
  op_ (ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t::ਸਦਿਸ਼_t &a, ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t::ਸਦਿਸ਼_t &b)
  { ਵਾਪਸ Op{} (a, b); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Op>
  ਖਾਲੀ process (ਅਚਲ Op& op,
		ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t &other,
		hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> *workspace = ਖਾਲੀ_ਸੂਚਕ)
  {
    process_ (op_<Op>, op (1, 0), op (0, 1), other, workspace);
  }

  ਖਾਲੀ union_ (ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t &other) { process (hb_bitwise_or, other); }
  ਖਾਲੀ intersect (ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t &other,
		  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> *workspace = ਖਾਲੀ_ਸੂਚਕ)
  { process (hb_bitwise_and, other, workspace); }
  ਖਾਲੀ ਘਟਾਓ (ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t &other) { process (hb_bitwise_gt, other); }
  ਖਾਲੀ ਸਮਮਿਤ_difference (ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t &other) { process (hb_bitwise_xor, other); }

  ਤਾਰਕਿਕ next (hb_codepoint_t *codepoint) ਅਚਲ
  {
    ਜੇ (unlikely (*codepoint == INVALID)) {
      *codepoint = get_min ();
      ਵਾਪਸ *codepoint != INVALID;
    }

    ਅਚਲ ਸਵੈਚਾਲਿਤ* ਪੰਨਾ_map_array = ਪੰਨਾ_map.arrayZ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ major = get_major (*codepoint);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਆਖਰੀ_ਪੰਨਾ_lookup;

    ਜੇ (unlikely (i >= ਪੰਨਾ_map.ਲੰਮਾਈ || ਪੰਨਾ_map_array[i].major != major))
    {
      ਪੰਨਾ_map.bfind (major, &i, HB_NOT_ਮਿਲਿਆ_STORE_CLOSEST);
      ਜੇ (i >= ਪੰਨਾ_map.ਲੰਮਾਈ) {
        *codepoint = INVALID;
        ਵਾਪਸ ਝੂਠ;
      }
      ਆਖਰੀ_ਪੰਨਾ_lookup = i;
    }

    ਅਚਲ ਸਵੈਚਾਲਿਤ* ਪੰਨੇ_array = ਪੰਨੇ.arrayZ;
    ਅਚਲ ਪੰਨਾ_map_t &current = ਪੰਨਾ_map_array[i];
    ਜੇ (likely (current.major == major))
    {
      ਜੇ (ਪੰਨੇ_array[current.ਸੂਚਕਾਂਕ_2].next (codepoint))
      {
        *codepoint += current.major * ਪੰਨਾ_t::ਪੰਨਾ_ਬਿੱਟ;
        ਵਾਪਸ ਸੱਚ;
      }
      i++;
    }

    ਲਈ (; i < ਪੰਨਾ_map.ਲੰਮਾਈ; i++)
    {
      ਅਚਲ ਪੰਨਾ_map_t &current = ਪੰਨਾ_map_array[i];
      hb_codepoint_t m = ਪੰਨੇ_array[current.ਸੂਚਕਾਂਕ_2].get_min ();
      ਜੇ (m != INVALID)
      {
	*codepoint = current.major * ਪੰਨਾ_t::ਪੰਨਾ_ਬਿੱਟ + m;
        ਆਖਰੀ_ਪੰਨਾ_lookup = i;
	ਵਾਪਸ ਸੱਚ;
      }
    }
    *codepoint = INVALID;
    ਵਾਪਸ ਝੂਠ;
  }
  ਤਾਰਕਿਕ ਅਗਲਾ_ਬਿੱਟ (hb_codepoint_t *codepoint, uint64_t *ਬਿੱਟ) ਅਚਲ
  {
    static_assert (ਪੰਨਾ_t::ELT_ਬਿੱਟ == 64, "");

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ elt = 0;
    ਜੇ (likely (*codepoint != INVALID))
    {
      hb_codepoint_t ਅਧਾਰ = *codepoint & ~ਪੰਨਾ_t::ELT_ਨਕਾਬ;
      ਜੇ (unlikely (ਅਧਾਰ >= INVALID - ਪੰਨਾ_t::ELT_ਬਿੱਟ + 1))
        ਜਾਓ ਪੂਰਾ;
      ਅਧਾਰ += ਪੰਨਾ_t::ELT_ਬਿੱਟ;

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ major = get_major (ਅਧਾਰ);
      i = ਆਖਰੀ_ਪੰਨਾ_lookup;
      ਜੇ (unlikely (i >= ਪੰਨਾ_map.ਲੰਮਾਈ || ਪੰਨਾ_map.arrayZ[i].major != major))
      {
	ਪੰਨਾ_map.bfind (major, &i, HB_NOT_ਮਿਲਿਆ_STORE_CLOSEST);
	ਜੇ (i >= ਪੰਨਾ_map.ਲੰਮਾਈ)
	  ਜਾਓ ਪੂਰਾ;
      }
      ਜੇ (ਪੰਨਾ_map.arrayZ[i].major == major)
	elt = ਪੰਨਾ_remainder (ਅਧਾਰ) / ਪੰਨਾ_t::ELT_ਬਿੱਟ;
    }

    ਲਈ (; i < ਪੰਨਾ_map.ਲੰਮਾਈ; i++, elt = 0)
    {
      ਅਚਲ ਪੰਨਾ_map_t &map = ਪੰਨਾ_map.arrayZ[i];
      ਅਚਲ ਪੰਨਾ_t &ਪੰਨਾ = ਪੰਨੇ.arrayZ[map.ਸੂਚਕਾਂਕ_2];
      ਲਈ (; elt < ਪੰਨਾ_t::len (); elt++)
	ਜੇ (ਪੰਨਾ.v[elt])
	{
	  *codepoint = major_ਸ਼ੁਰੂ (map.major) + elt * ਪੰਨਾ_t::ELT_ਬਿੱਟ;
	  *ਬਿੱਟ = ਪੰਨਾ.v[elt];
	  ਆਖਰੀ_ਪੰਨਾ_lookup = i;
	  ਵਾਪਸ ਸੱਚ;
	}
    }

  ਪੂਰਾ:
    *codepoint = INVALID;
    *ਬਿੱਟ = 0;
    ਵਾਪਸ ਝੂਠ;
  }
  ਤਾਰਕਿਕ ਪਿਛਲਾ (hb_codepoint_t *codepoint) ਅਚਲ
  {
    ਜੇ (unlikely (*codepoint == INVALID)) {
      *codepoint = get_max ();
      ਵਾਪਸ *codepoint != INVALID;
    }

    ਪੰਨਾ_map_t map = {get_major (*codepoint), 0};
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i;
    ਪੰਨਾ_map.bfind (map, &i, HB_NOT_ਮਿਲਿਆ_STORE_CLOSEST);
    ਜੇ (i < ਪੰਨਾ_map.ਲੰਮਾਈ && ਪੰਨਾ_map.arrayZ[i].major == map.major)
    {
      ਜੇ (ਪੰਨੇ[ਪੰਨਾ_map.arrayZ[i].ਸੂਚਕਾਂਕ_2].ਪਿਛਲਾ (codepoint))
      {
	*codepoint += ਪੰਨਾ_map.arrayZ[i].major * ਪੰਨਾ_t::ਪੰਨਾ_ਬਿੱਟ;
	ਵਾਪਸ ਸੱਚ;
      }
    }
    i--;
    ਲਈ (; (ਪੂਰਨ) i >= 0; i--)
    {
      hb_codepoint_t m = ਪੰਨੇ.arrayZ[ਪੰਨਾ_map.arrayZ[i].ਸੂਚਕਾਂਕ_2].get_max ();
      ਜੇ (m != INVALID)
      {
	*codepoint = ਪੰਨਾ_map.arrayZ[i].major * ਪੰਨਾ_t::ਪੰਨਾ_ਬਿੱਟ + m;
	ਵਾਪਸ ਸੱਚ;
      }
    }
    *codepoint = INVALID;
    ਵਾਪਸ ਝੂਠ;
  }
  ਤਾਰਕਿਕ ਅਗਲਾ_range (hb_codepoint_t *ਪਹਿਲਾ, hb_codepoint_t *ਆਖਰੀ) ਅਚਲ
  {
    hb_codepoint_t i;

    i = *ਆਖਰੀ;
    ਜੇ (!next (&i))
    {
      *ਆਖਰੀ = *ਪਹਿਲਾ = INVALID;
      ਵਾਪਸ ਝੂਠ;
    }

    *ਆਖਰੀ = *ਪਹਿਲਾ = i;

    ਅਚਲ ਸਵੈਚਾਲਿਤ* ਪੰਨਾ_map_array = ਪੰਨਾ_map.arrayZ;
    ਅਚਲ ਸਵੈਚਾਲਿਤ* ਪੰਨੇ_array = ਪੰਨੇ.arrayZ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ major = get_major (*ਆਖਰੀ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ p_ਸੂਚਕਾਂਕ = ਆਖਰੀ_ਪੰਨਾ_lookup;

    ਜੇ (unlikely (p_ਸੂਚਕਾਂਕ >= ਪੰਨਾ_map.ਲੰਮਾਈ || ਪੰਨਾ_map_array[p_ਸੂਚਕਾਂਕ].major != major))
    {
      ਪੰਨਾ_map.bfind (major, &p_ਸੂਚਕਾਂਕ, HB_NOT_ਮਿਲਿਆ_STORE_CLOSEST);
      // The above next() call gaurantees the page we're looking for exists.
      ਪ੍ਰਤਿਜ੍ਞਾ(p_ਸੂਚਕਾਂਕ < ਪੰਨਾ_map.ਲੰਮਾਈ && ਪੰਨਾ_map_array[p_ਸੂਚਕਾਂਕ].major == major);
      ਆਖਰੀ_ਪੰਨਾ_lookup = p_ਸੂਚਕਾਂਕ;
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ rem = ਪੰਨਾ_remainder (*ਆਖਰੀ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ elt = rem / ਪੰਨਾ_t::ELT_ਬਿੱਟ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ bit = rem & ਪੰਨਾ_t::ELT_ਨਕਾਬ;

    ਅਚਲ ਪੰਨਾ_t *ਪੰਨਾ = &ਪੰਨੇ_array[ਪੰਨਾ_map_array[p_ਸੂਚਕਾਂਕ].ਸੂਚਕਾਂਕ_2];
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਲੜੀਵਾਰ = hb_ctz (~(ਪੰਨਾ->v[elt] >> bit));
    *ਆਖਰੀ += ਲੜੀਵਾਰ - 1;
    ਜੇ (ਲੜੀਵਾਰ < ਪੰਨਾ_t::ELT_ਬਿੱਟ - bit)
      ਵਾਪਸ ਸੱਚ;

    elt++;

    ਜਦੋਂ (ਸੱਚ)
    {
      ਲਈ (; elt < ਪੰਨਾ_t::len (); elt++)
      {
        uint64_t w = ਪੰਨਾ->v[elt];
        ਜੇ (w == (uint64_t) -1)
        {
          *ਆਖਰੀ += ਪੰਨਾ_t::ELT_ਬਿੱਟ;
          ਜਾਰੀ;
        }
        uint64_t inv = ~w;
        *ਆਖਰੀ += hb_ctz (inv);
        ਵਾਪਸ ਸੱਚ;
      }

      p_ਸੂਚਕਾਂਕ++;


      ਜੇ (p_ਸੂਚਕਾਂਕ >= ਪੰਨਾ_map.ਲੰਮਾਈ || ਪੰਨਾ_map_array[p_ਸੂਚਕਾਂਕ].major != major + 1)
        ਵਾਪਸ ਸੱਚ;

      ਅਚਲ ਸਵੈਚਾਲਿਤ& ਇੰਦਰਾਜ = ਪੰਨਾ_map_array[p_ਸੂਚਕਾਂਕ];
      major = ਇੰਦਰਾਜ.major;
      ਆਖਰੀ_ਪੰਨਾ_lookup = p_ਸੂਚਕਾਂਕ;
      ਪੰਨਾ = &ਪੰਨੇ_array[ਇੰਦਰਾਜ.ਸੂਚਕਾਂਕ_2];
      elt = 0;
    }
  }

  ਤਾਰਕਿਕ ਪਿਛਲਾ_range (hb_codepoint_t *ਪਹਿਲਾ, hb_codepoint_t *ਆਖਰੀ) ਅਚਲ
  {
    hb_codepoint_t i;

    i = *ਪਹਿਲਾ;
    ਜੇ (!ਪਿਛਲਾ (&i))
    {
      *ਆਖਰੀ = *ਪਹਿਲਾ = INVALID;
      ਵਾਪਸ ਝੂਠ;
    }

    *ਆਖਰੀ = *ਪਹਿਲਾ = i;

    ਅਚਲ ਸਵੈਚਾਲਿਤ* ਪੰਨਾ_map_array = ਪੰਨਾ_map.arrayZ;
    ਅਚਲ ਸਵੈਚਾਲਿਤ* ਪੰਨੇ_array = ਪੰਨੇ.arrayZ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ major = get_major (*ਪਹਿਲਾ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ p_ਸੂਚਕਾਂਕ = ਆਖਰੀ_ਪੰਨਾ_lookup;

    ਜੇ (unlikely (p_ਸੂਚਕਾਂਕ >= ਪੰਨਾ_map.ਲੰਮਾਈ || ਪੰਨਾ_map_array[p_ਸੂਚਕਾਂਕ].major != major))
    {
      ਪੰਨਾ_map.bfind (major, &p_ਸੂਚਕਾਂਕ, HB_NOT_ਮਿਲਿਆ_STORE_CLOSEST);
      // The above previous() call gaurantees the page we're looking for exists.
      ਪ੍ਰਤਿਜ੍ਞਾ(p_ਸੂਚਕਾਂਕ < ਪੰਨਾ_map.ਲੰਮਾਈ && ਪੰਨਾ_map_array[p_ਸੂਚਕਾਂਕ].major == major);
      ਆਖਰੀ_ਪੰਨਾ_lookup = p_ਸੂਚਕਾਂਕ;
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ rem = ਪੰਨਾ_remainder (*ਪਹਿਲਾ);
    ਪੂਰਨ elt = rem / ਪੰਨਾ_t::ELT_ਬਿੱਟ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ bit = rem & ਪੰਨਾ_t::ELT_ਨਕਾਬ;

    ਅਚਲ ਪੰਨਾ_t *ਪੰਨਾ = &ਪੰਨੇ_array[ਪੰਨਾ_map_array[p_ਸੂਚਕਾਂਕ].ਸੂਚਕਾਂਕ_2];
    uint64_t inv = ~(ਪੰਨਾ->v[elt] << (63 - bit));
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਲੜੀਵਾਰ = hb_ਦੋਆਧਾਰੀ_ਅੰਕ_storage (inv) ? (ਪੰਨਾ_t::ELT_ਬਿੱਟ - hb_ਦੋਆਧਾਰੀ_ਅੰਕ_storage (inv)) : ਪੰਨਾ_t::ELT_ਬਿੱਟ;
    *ਪਹਿਲਾ -= ਲੜੀਵਾਰ - 1;
    ਜੇ (ਲੜੀਵਾਰ < bit + 1)
      ਵਾਪਸ ਸੱਚ;

    elt--;

    ਜਦੋਂ (ਸੱਚ)
    {
      ਲਈ (; elt >= 0; elt--)
      {
        uint64_t w = ਪੰਨਾ->v[elt];
        ਜੇ (w == (uint64_t) -1)
        {
          *ਪਹਿਲਾ -= ਪੰਨਾ_t::ELT_ਬਿੱਟ;
          ਜਾਰੀ;
        }
        uint64_t inv = ~w;
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ zeros = hb_ਦੋਆਧਾਰੀ_ਅੰਕ_storage (inv) ? (ਪੰਨਾ_t::ELT_ਬਿੱਟ - hb_ਦੋਆਧਾਰੀ_ਅੰਕ_storage (inv)) : ਪੰਨਾ_t::ELT_ਬਿੱਟ;
        *ਪਹਿਲਾ -= zeros;
        ਵਾਪਸ ਸੱਚ;
      }


      ਜੇ (p_ਸੂਚਕਾਂਕ == 0 || ਪੰਨਾ_map_array[p_ਸੂਚਕਾਂਕ - 1].major != major - 1)
        ਵਾਪਸ ਸੱਚ;

      p_ਸੂਚਕਾਂਕ--;
      ਅਚਲ ਸਵੈਚਾਲਿਤ& ਇੰਦਰਾਜ = ਪੰਨਾ_map_array[p_ਸੂਚਕਾਂਕ];

      major = ਇੰਦਰਾਜ.major;
      ਆਖਰੀ_ਪੰਨਾ_lookup = p_ਸੂਚਕਾਂਕ;
      ਪੰਨਾ = &ਪੰਨੇ_array[ਇੰਦਰਾਜ.ਸੂਚਕਾਂਕ_2];
      elt = ਪੰਨਾ_t::len () - 1;
    }
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅਗਲਾ_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *ਬਾਹਰਲਾ,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    ਮਾਪ) ਅਚਲ
  {
    // By default, start at the first bit of the first page of values.
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਪੰਨਾ = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਪੰਨਾ_ਮੁੱਲ = 0;
    ਜੇ (unlikely (codepoint != INVALID))
    {
      ਅਚਲ ਸਵੈਚਾਲਿਤ* ਪੰਨਾ_map_array = ਪੰਨਾ_map.arrayZ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ major = get_major (codepoint);
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਆਖਰੀ_ਪੰਨਾ_lookup;
      ਜੇ (unlikely (i >= ਪੰਨਾ_map.ਲੰਮਾਈ || ਪੰਨਾ_map_array[i].major != major))
      {
	ਪੰਨਾ_map.bfind (major, &i, HB_NOT_ਮਿਲਿਆ_STORE_CLOSEST);
	ਜੇ (i >= ਪੰਨਾ_map.ਲੰਮਾਈ)
	  ਵਾਪਸ 0;  // codepoint is greater than our max element.
      }
      ਸ਼ੁਰੂ_ਪੰਨਾ = i;
      ਜੇ (ਪੰਨਾ_map_array[i].major == major)
      {
        ਸ਼ੁਰੂ_ਪੰਨਾ_ਮੁੱਲ = ਪੰਨਾ_remainder (codepoint + 1);
        ਜੇ (unlikely (ਸ਼ੁਰੂ_ਪੰਨਾ_ਮੁੱਲ == 0))
        {
          // The export-after value was last in the page. Start on next page.
          ਸ਼ੁਰੂ_ਪੰਨਾ++;
          ਸ਼ੁਰੂ_ਪੰਨਾ_ਮੁੱਲ = 0;
        }
      }
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ initial_ਮਾਪ = ਮਾਪ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ_ਪੰਨਾ; i < ਪੰਨਾ_map.ਲੰਮਾਈ && ਮਾਪ; i++)
    {
      uint32_t ਅਧਾਰ = major_ਸ਼ੁਰੂ (ਪੰਨਾ_map.arrayZ[i].major);
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ n = ਪੰਨੇ[ਪੰਨਾ_map.arrayZ[i].ਸੂਚਕਾਂਕ_2].ਲਿਖ (ਅਧਾਰ, ਸ਼ੁਰੂ_ਪੰਨਾ_ਮੁੱਲ, ਬਾਹਰਲਾ, ਮਾਪ);
      ਬਾਹਰਲਾ += n;
      ਮਾਪ -= n;
      ਸ਼ੁਰੂ_ਪੰਨਾ_ਮੁੱਲ = 0;
    }
    ਵਾਪਸ initial_ਮਾਪ - ਮਾਪ;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅਗਲਾ_many_inverted (hb_codepoint_t  codepoint,
				   hb_codepoint_t *ਬਾਹਰਲਾ,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    ਮਾਪ) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ initial_ਮਾਪ = ਮਾਪ;
    // By default, start at the first bit of the first page of values.
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਪੰਨਾ = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਪੰਨਾ_ਮੁੱਲ = 0;
    ਜੇ (unlikely (codepoint != INVALID))
    {
      ਅਚਲ ਸਵੈਚਾਲਿਤ* ਪੰਨਾ_map_array = ਪੰਨਾ_map.arrayZ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ major = get_major (codepoint);
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਆਖਰੀ_ਪੰਨਾ_lookup;
      ਜੇ (unlikely (i >= ਪੰਨਾ_map.ਲੰਮਾਈ || ਪੰਨਾ_map_array[i].major != major))
      {
        ਪੰਨਾ_map.bfind(major, &i, HB_NOT_ਮਿਲਿਆ_STORE_CLOSEST);
        ਜੇ (unlikely (i >= ਪੰਨਾ_map.ਲੰਮਾਈ))
        {
          // codepoint is greater than our max element.
          ਜਦੋਂ (++codepoint != INVALID && ਮਾਪ)
          {
            *ਬਾਹਰਲਾ++ = codepoint;
            ਮਾਪ--;
          }
          ਵਾਪਸ initial_ਮਾਪ - ਮਾਪ;
        }
      }
      ਸ਼ੁਰੂ_ਪੰਨਾ = i;
      ਜੇ (ਪੰਨਾ_map_array[i].major == major)
      {
        ਸ਼ੁਰੂ_ਪੰਨਾ_ਮੁੱਲ = ਪੰਨਾ_remainder (codepoint + 1);
        ਜੇ (unlikely (ਸ਼ੁਰੂ_ਪੰਨਾ_ਮੁੱਲ == 0))
        {
          // The export-after value was last in the page. Start on next page.
          ਸ਼ੁਰੂ_ਪੰਨਾ++;
          ਸ਼ੁਰੂ_ਪੰਨਾ_ਮੁੱਲ = 0;
        }
      }
    }

    hb_codepoint_t ਅਗਲਾ_ਮੁੱਲ = codepoint + 1;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i=ਸ਼ੁਰੂ_ਪੰਨਾ; i<ਪੰਨਾ_map.ਲੰਮਾਈ && ਮਾਪ; i++)
    {
      uint32_t ਅਧਾਰ = major_ਸ਼ੁਰੂ (ਪੰਨਾ_map.arrayZ[i].major);
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ n = ਪੰਨੇ[ਪੰਨਾ_map.arrayZ[i].ਸੂਚਕਾਂਕ_2].write_inverted (ਅਧਾਰ, ਸ਼ੁਰੂ_ਪੰਨਾ_ਮੁੱਲ, ਬਾਹਰਲਾ, ਮਾਪ, &ਅਗਲਾ_ਮੁੱਲ);
      ਬਾਹਰਲਾ += n;
      ਮਾਪ -= n;
      ਸ਼ੁਰੂ_ਪੰਨਾ_ਮੁੱਲ = 0;
    }
    ਜਦੋਂ (ਅਗਲਾ_ਮੁੱਲ < HB_ਸੈੱਟ_ਮੁੱਲ_INVALID && ਮਾਪ) {
      *ਬਾਹਰਲਾ++ = ਅਗਲਾ_ਮੁੱਲ++;
      ਮਾਪ--;
    }
    ਵਾਪਸ initial_ਮਾਪ - ਮਾਪ;
  }

  ਤਾਰਕਿਕ has_population () ਅਚਲ { ਵਾਪਸ population != UINT_MAX; }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_population () ਅਚਲ
  {
    ਜੇ (has_population ())
      ਵਾਪਸ population;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਕੱਢੋ = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਪੰਨੇ.ਲੰਮਾਈ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      ਕੱਢੋ += ਪੰਨੇ[i].get_population ();

    population = ਕੱਢੋ;
    ਵਾਪਸ ਕੱਢੋ;
  }
  ਤਾਰਕਿਕ get_singleton (hb_codepoint_t *codepoint) ਅਚਲ
  {
    ਜੇ (has_population ())
    {
      ਜੇ (population != 1) ਵਾਪਸ ਝੂਠ;
      *codepoint = get_min ();
      ਵਾਪਸ ਸੱਚ;
    }

    hb_codepoint_t singleton = INVALID;
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &map : ਪੰਨਾ_map)
    {
      ਅਚਲ ਸਵੈਚਾਲਿਤ &ਪੰਨਾ = ਪੰਨੇ.arrayZ[map.ਸੂਚਕਾਂਕ_2];
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਪੰਨਾ_t::len (); i++)
      {
	ਪੰਨਾ_t::elt_t ਬਿੱਟ = ਪੰਨਾ.v[i];
	ਜੇ (!ਬਿੱਟ) ਜਾਰੀ;
	ਜੇ (singleton != INVALID || (ਬਿੱਟ & (ਬਿੱਟ - 1)))
	  ਵਾਪਸ ਝੂਠ;
	singleton = map.major * ਪੰਨਾ_t::ਪੰਨਾ_ਬਿੱਟ +
		    i * ਪੰਨਾ_t::ELT_ਬਿੱਟ + hb_ctz (ਬਿੱਟ);
      }
    }

    ਜੇ (singleton == INVALID)
    {
      population = 0;
      ਵਾਪਸ ਝੂਠ;
    }
    population = 1;
    *codepoint = singleton;
    ਵਾਪਸ ਸੱਚ;
  }
  hb_codepoint_t get_min () ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = ਪੰਨੇ.ਲੰਮਾਈ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਅਚਲ ਸਵੈਚਾਲਿਤ& map = ਪੰਨਾ_map.arrayZ[i];
      ਅਚਲ ਸਵੈਚਾਲਿਤ& ਪੰਨਾ = ਪੰਨੇ.arrayZ[map.ਸੂਚਕਾਂਕ_2];

      ਜੇ (!ਪੰਨਾ.is_empty ())
	ਵਾਪਸ map.major * ਪੰਨਾ_t::ਪੰਨਾ_ਬਿੱਟ + ਪੰਨਾ.get_min ();
    }
    ਵਾਪਸ INVALID;
  }
  hb_codepoint_t get_max () ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = ਪੰਨੇ.ਲੰਮਾਈ;
    ਲਈ (ਨਿਸ਼ਾਨੀਵਾਲਾ i = ਗਿਣਤੀ_2 - 1; i >= 0; i--)
    {
      ਅਚਲ ਸਵੈਚਾਲਿਤ& map = ਪੰਨਾ_map.arrayZ[(ਬਿਨਾਨਿਸ਼ਾਨੀ) i];
      ਅਚਲ ਸਵੈਚਾਲਿਤ& ਪੰਨਾ = ਪੰਨੇ.arrayZ[map.ਸੂਚਕਾਂਕ_2];

      ਜੇ (!ਪੰਨਾ.is_empty ())
	ਵਾਪਸ map.major * ਪੰਨਾ_t::ਪੰਨਾ_ਬਿੱਟ + ਪੰਨਾ.get_max ();
    }
    ਵਾਪਸ INVALID;
  }

  ਸਥਿਰ constexpr hb_codepoint_t INVALID = ਪੰਨਾ_t::INVALID;

  /*
   * Iterator implementation.
   */
  ਬਣਤਰ iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ਸਥਿਰ constexpr ਤਾਰਕਿਕ is_sorted_iterator = ਸੱਚ;
    ਸਥਿਰ constexpr ਤਾਰਕਿਕ has_fast_len = ਸੱਚ;
    iter_t (ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t &s_ = Null (hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t),
	    ਤਾਰਕਿਕ init = ਸੱਚ) : s (&s_), v (INVALID), l(0)
    {
      ਜੇ (init)
      {
	l = s->get_population () + 1;
	__next__ ();
      }
    }

    ਕਿਸਮਪਰਿਭਾਸ਼ਾ hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ਅਚਲ { ਵਾਪਸ v; }
    ਤਾਰਕਿਕ __more__ () ਅਚਲ { ਵਾਪਸ v != INVALID; }
    ਖਾਲੀ __next__ () { s->next (&v); ਜੇ (l) l--; }
    ਖਾਲੀ __prev__ () { s->ਪਿਛਲਾ (&v); }
    ਬਿਨਾਨਿਸ਼ਾਨੀ __len__ () ਅਚਲ { ਵਾਪਸ l; }
    iter_t ਸਿਰਾ () ਅਚਲ { ਵਾਪਸ iter_t (*s, ਝੂਠ); }
    ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ iter_t& o) ਅਚਲ
    { ਵਾਪਸ v != o.v; }

    ਸੁਰੱਖਿਅਤ:
    ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t *s;
    hb_codepoint_t v;
    ਬਿਨਾਨਿਸ਼ਾਨੀ l;
  };
  iter_t iter () ਅਚਲ { ਵਾਪਸ iter_t (*ਇਹ); }
  ਸੰਚਾਲਕ iter_t () ਅਚਲ { ਵਾਪਸ iter (); }

  ਸੁਰੱਖਿਅਤ:

  ਪੰਨਾ_t *ਪੰਨਾ_for (hb_codepoint_t g, ਤਾਰਕਿਕ insert = ਝੂਠ)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    ਬਿਨਾਨਿਸ਼ਾਨੀ i = ਆਖਰੀ_ਪੰਨਾ_lookup;
    ਜੇ (likely (i < ਪੰਨਾ_map.ਲੰਮਾਈ))
    {
      ਸਵੈਚਾਲਿਤ &cached_ਪੰਨਾ = ਪੰਨਾ_map.arrayZ[i];
      ਜੇ (cached_ਪੰਨਾ.major == major)
	ਵਾਪਸ &ਪੰਨੇ.arrayZ[cached_ਪੰਨਾ.ਸੂਚਕਾਂਕ_2];
    }

    ਪੰਨਾ_map_t map = {major, ਪੰਨੇ.ਲੰਮਾਈ};
    ਜੇ (!ਪੰਨਾ_map.bfind (map, &i, HB_NOT_ਮਿਲਿਆ_STORE_CLOSEST))
    {
      ਜੇ (!insert)
        ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

      ਜੇ (unlikely (!resize (ਪੰਨੇ.ਲੰਮਾਈ + 1)))
	ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

      ਪੰਨੇ.arrayZ[map.ਸੂਚਕਾਂਕ_2].init0 ();
      memmove (ਪੰਨਾ_map.arrayZ + i + 1,
	       ਪੰਨਾ_map.arrayZ + i,
	       (ਪੰਨਾ_map.ਲੰਮਾਈ - 1 - i) * ਪੰਨਾ_map.ਵਸਤੂ_ਮਾਪ);
      ਪੰਨਾ_map.arrayZ[i] = map;
    }

    ਆਖਰੀ_ਪੰਨਾ_lookup = i;
    ਵਾਪਸ &ਪੰਨੇ.arrayZ[ਪੰਨਾ_map.arrayZ[i].ਸੂਚਕਾਂਕ_2];
  }
  ਅਚਲ ਪੰਨਾ_t *ਪੰਨਾ_for (hb_codepoint_t g) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    ਬਿਨਾਨਿਸ਼ਾਨੀ i = ਆਖਰੀ_ਪੰਨਾ_lookup;
    ਜੇ (likely (i < ਪੰਨਾ_map.ਲੰਮਾਈ))
    {
      ਸਵੈਚਾਲਿਤ &cached_ਪੰਨਾ = ਪੰਨਾ_map.arrayZ[i];
      ਜੇ (cached_ਪੰਨਾ.major == major)
	ਵਾਪਸ &ਪੰਨੇ.arrayZ[cached_ਪੰਨਾ.ਸੂਚਕਾਂਕ_2];
    }

    ਪੰਨਾ_map_t key = {major};
    ਜੇ (!ਪੰਨਾ_map.bfind (key, &i))
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

    ਆਖਰੀ_ਪੰਨਾ_lookup = i;
    ਵਾਪਸ &ਪੰਨੇ.arrayZ[ਪੰਨਾ_map.arrayZ[i].ਸੂਚਕਾਂਕ_2];
  }
  ਪੰਨਾ_t &ਪੰਨਾ_at (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i)
  {
    ਪ੍ਰਤਿਜ੍ਞਾ (i < ਪੰਨਾ_map.ਲੰਮਾਈ);
    ਵਾਪਸ ਪੰਨੇ.arrayZ[ਪੰਨਾ_map.arrayZ[i].ਸੂਚਕਾਂਕ_2];
  }
  ਅਚਲ ਪੰਨਾ_t &ਪੰਨਾ_at (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ
  {
    ਪ੍ਰਤਿਜ੍ਞਾ (i < ਪੰਨਾ_map.ਲੰਮਾਈ);
    ਵਾਪਸ ਪੰਨੇ.arrayZ[ਪੰਨਾ_map.arrayZ[i].ਸੂਚਕਾਂਕ_2];
  }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_major (hb_codepoint_t g) ਅਚਲ { ਵਾਪਸ g >> ਪੰਨਾ_t::ਪੰਨਾ_ਬਿੱਟ_LOG_2; }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਪੰਨਾ_remainder (hb_codepoint_t g) ਅਚਲ { ਵਾਪਸ g & ਪੰਨਾ_t::ਪੰਨਾ_BITMASK; }
  hb_codepoint_t major_ਸ਼ੁਰੂ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ major) ਅਚਲ { ਵਾਪਸ major << ਪੰਨਾ_t::ਪੰਨਾ_ਬਿੱਟ_LOG_2; }
};


#ਜੇ_ਅੰਤ /* HB_BIT_SET_HH */

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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_INVERTIBLE_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_INVERTIBLE_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-bit-set.hh"


ਬਣਤਰ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_invertible_t
{
  hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t s;
  ਤਾਰਕਿਕ inverted = ਝੂਠ;

  hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_invertible_t () = ਮੂਲ;
  hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_invertible_t (ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_invertible_t& o) = ਮੂਲ;
  hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_invertible_t (hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_invertible_t&& other)  noexcept : hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_invertible_t () { hb_ਅਦਲਾ_ਬਦਲੀ (*ਇਹ, other); }
  hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_invertible_t& ਸੰਚਾਲਕ= (ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_invertible_t& o) = ਮੂਲ;
  hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_invertible_t& ਸੰਚਾਲਕ= (hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_invertible_t&& other)  noexcept { hb_ਅਦਲਾ_ਬਦਲੀ (*ਇਹ, other); ਵਾਪਸ *ਇਹ; }
  ਮਿੱਤਰ ਖਾਲੀ swap (hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_invertible_t &a, hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_invertible_t &b) noexcept
  {
    ਜੇ (likely (!a.s.successful || !b.s.successful))
      ਵਾਪਸ;
    hb_ਅਦਲਾ_ਬਦਲੀ (a.inverted, b.inverted);
    hb_ਅਦਲਾ_ਬਦਲੀ (a.s, b.s);
  }

  ਖਾਲੀ init () { s.init (); inverted = ਝੂਠ; }
  ਖਾਲੀ fini () { s.fini (); }
  ਖਾਲੀ err () { s.err (); }
  ਤਾਰਕਿਕ ਵਿੱਚ_error () ਅਚਲ { ਵਾਪਸ s.ਵਿੱਚ_error (); }
  ਸਪਸ਼ਟ ਸੰਚਾਲਕ ਤਾਰਕਿਕ () ਅਚਲ { ਵਾਪਸ !is_empty (); }

  ਖਾਲੀ alloc (ਬਿਨਾਨਿਸ਼ਾਨੀ sz) { s.alloc (sz); }
  ਖਾਲੀ reset ()
  {
    s.reset ();
    inverted = ਝੂਠ;
  }
  ਖਾਲੀ clear ()
  {
    s.clear ();
    ਜੇ (likely (s.successful))
      inverted = ਝੂਠ;
  }
  ਖਾਲੀ invert ()
  {
    ਜੇ (likely (s.successful))
      inverted = !inverted;
  }

  ਤਾਰਕਿਕ is_inverted () ਅਚਲ
  {
    ਵਾਪਸ inverted;
  }

  ਤਾਰਕਿਕ is_empty () ਅਚਲ
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    ਵਾਪਸ v == INVALID;
  }
  uint32_t hash () ਅਚਲ { ਵਾਪਸ s.hash () ^ (uint32_t) inverted; }

  hb_codepoint_t get_min () ਅਚਲ
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    ਵਾਪਸ v;
  }
  hb_codepoint_t get_max () ਅਚਲ
  {
    hb_codepoint_t v = INVALID;
    ਪਿਛਲਾ (&v);
    ਵਾਪਸ v;
  }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_population () ਅਚਲ
  { ਵਾਪਸ inverted ? INVALID - s.get_population () : s.get_population (); }
  ਤਾਰਕਿਕ get_singleton (hb_codepoint_t *codepoint) ਅਚਲ
  {
    ਜੇ (likely (!inverted))
      ਵਾਪਸ s.get_singleton (codepoint);
    ਜੇ (s.get_population () != INVALID - 1)
      ਵਾਪਸ ਝੂਠ;
    *codepoint = get_min ();
    ਵਾਪਸ *codepoint != INVALID;
  }


  ਖਾਲੀ ਜੋੜੋ (hb_codepoint_t g) { unlikely (inverted) ? s.del (g) : s.ਜੋੜੋ (g); }
  ਖਾਲੀ ਜੋੜੋ_ਬਿੱਟ (hb_codepoint_t g, uint64_t ਬਿੱਟ)
  { unlikely (inverted) ? s.del_ਬਿੱਟ (g, ਬਿੱਟ) : s.ਜੋੜੋ_ਬਿੱਟ (g, ਬਿੱਟ); }
  ਤਾਰਕਿਕ ਜੋੜੋ_range (hb_codepoint_t a, hb_codepoint_t b)
  { ਵਾਪਸ unlikely (inverted) ? ((ਖਾਲੀ) s.del_range (a, b), ਸੱਚ) : s.ਜੋੜੋ_range (a, b); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਖਾਲੀ ਜੋੜੋ_array (ਅਚਲ T *array, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ stride=ਆਕਾਰ(T))
  { inverted ? s.del_array (array, ਗਿਣਤੀ_2, stride) : s.ਜੋੜੋ_array (array, ਗਿਣਤੀ_2, stride); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਖਾਲੀ ਜੋੜੋ_array (ਅਚਲ hb_array_t<ਅਚਲ T>& arr) { ਜੋੜੋ_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਤਾਰਕਿਕ ਜੋੜੋ_sorted_array (ਅਚਲ T *array, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ stride=ਆਕਾਰ(T))
  { ਵਾਪਸ inverted ? s.del_sorted_array (array, ਗਿਣਤੀ_2, stride) : s.ਜੋੜੋ_sorted_array (array, ਗਿਣਤੀ_2, stride); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਤਾਰਕਿਕ ਜੋੜੋ_sorted_array (ਅਚਲ hb_sorted_array_t<ਅਚਲ T>& arr) { ਵਾਪਸ ਜੋੜੋ_sorted_array (&arr, arr.len ()); }

  ਖਾਲੀ del (hb_codepoint_t g) { unlikely (inverted) ? s.ਜੋੜੋ (g) : s.del (g); }
  ਖਾਲੀ del_range (hb_codepoint_t a, hb_codepoint_t b)
  { unlikely (inverted) ? (ਖਾਲੀ) s.ਜੋੜੋ_range (a, b) : s.del_range (a, b); }

  ਤਾਰਕਿਕ get (hb_codepoint_t g) ਅਚਲ { ਵਾਪਸ s.get (g) ^ inverted; }
  ਤਾਰਕਿਕ may_have (hb_codepoint_t g) ਅਚਲ { ਵਾਪਸ get (g); }

  /* Has interface. */
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ [] (hb_codepoint_t k) ਅਚਲ { ਵਾਪਸ get (k); }
  ਤਾਰਕਿਕ has (hb_codepoint_t k) ਅਚਲ { ਵਾਪਸ (*ਇਹ)[k]; }
  /* Predicate. */
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ () (hb_codepoint_t k) ਅਚਲ { ਵਾਪਸ has (k); }

  /* Sink interface. */
  hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_invertible_t& ਸੰਚਾਲਕ << (hb_codepoint_t v)
  { ਜੋੜੋ (v); ਵਾਪਸ *ਇਹ; }
  hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_invertible_t& ਸੰਚਾਲਕ << (ਅਚਲ hb_codepoint_pair_t& range)
  { ਜੋੜੋ_range (range.ਪਹਿਲਾ, range.second); ਵਾਪਸ *ਇਹ; }

  ਤਾਰਕਿਕ may_intersect (ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_invertible_t &other) ਅਚਲ
  { ਵਾਪਸ inverted || other.inverted || s.intersects (other.s); }

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_invertible_t &other) ਅਚਲ
  {
    ਜੇ (likely (!inverted))
      ਵਾਪਸ unlikely (other.inverted) ? !s.is_subset (other.s)
				       : s.intersects (other.s);
    ਜੇ (likely (!other.inverted))
      ਵਾਪਸ !other.s.is_subset (s);

    ਬਿਨਾਨਿਸ਼ਾਨੀ population = s.get_population ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ other_population = other.s.get_population ();
    ਜੇ ((uint64_t) population + other_population < INVALID)
      ਵਾਪਸ ਸੱਚ;

    ਵਾਪਸ population >= other_population
	 ? !hb_all (iter (), other.s)
	 : !hb_all (other.iter (), s);
  }

  ਤਾਰਕਿਕ intersects (hb_codepoint_t ਪਹਿਲਾ, hb_codepoint_t ਆਖਰੀ) ਅਚਲ
  {
    hb_codepoint_t c = ਪਹਿਲਾ - 1;
    ਵਾਪਸ next (&c) && c <= ਆਖਰੀ;
  }

  ਖਾਲੀ ਸੈੱਟ (ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_invertible_t &other)
  {
    s.ਸੈੱਟ (other.s);
    ਜੇ (likely (s.successful))
      inverted = other.inverted;
  }

  ਤਾਰਕਿਕ is_equal (ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_invertible_t &other) ਅਚਲ
  {
    ਜੇ (likely (inverted == other.inverted))
      ਵਾਪਸ s.is_equal (other.s);
    ਨਹੀਂਤਾਂ
    {
      ਜੇ (get_population () != other.get_population ())
	ਵਾਪਸ ਝੂਠ;

      /* TODO Add iter_ranges() and use here. */
      ਸਵੈਚਾਲਿਤ it1 = iter ();
      ਸਵੈਚਾਲਿਤ it2 = other.iter ();
      ਵਾਪਸ hb_all (+ hb_zip (it1, it2)
		     | hb_map ([](hb_codepoint_pair_t _) { ਵਾਪਸ _.ਪਹਿਲਾ == _.second; }));
    }
  }

  ਤਾਰਕਿਕ is_subset (ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_invertible_t &larger_ਸੈੱਟ) ਅਚਲ
  {
    ਜੇ (unlikely (inverted != larger_ਸੈੱਟ.inverted))
    {
      ਜੇ (inverted)
	ਵਾਪਸ hb_all (iter (), larger_ਸੈੱਟ.s);
      ਨਹੀਂਤਾਂ
        // larger set is inverted so larger_set.s is the set of things that are not present
        // in larger_set, therefore if s has any of those it can't be a subset.
	ਵਾਪਸ !s.intersects (larger_ਸੈੱਟ.s);
    }
    ਨਹੀਂਤਾਂ
      ਵਾਪਸ unlikely (inverted) ? larger_ਸੈੱਟ.s.is_subset (s) : s.is_subset (larger_ਸੈੱਟ.s);
  }

  ਸੁਰੱਖਿਅਤ:
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Op>
  ਖਾਲੀ process (ਅਚਲ Op& op,
		ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_invertible_t &other,
		hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> *workspace = ਖਾਲੀ_ਸੂਚਕ)
  { s.process (op, other.s, workspace); }
  ਜਨਤਕ:
  ਖਾਲੀ union_ (ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_invertible_t &other)
  {
    ਜੇ (likely (inverted == other.inverted))
    {
      ਜੇ (unlikely (inverted))
	process (hb_bitwise_and, other);
      ਨਹੀਂਤਾਂ
	process (hb_bitwise_or, other); /* Main branch. */
    }
    ਨਹੀਂਤਾਂ
    {
      ਜੇ (unlikely (inverted))
	process (hb_bitwise_gt, other);
      ਨਹੀਂਤਾਂ
	process (hb_bitwise_lt, other);
    }
    ਜੇ (likely (s.successful))
      inverted = inverted || other.inverted;
  }
  ਖਾਲੀ intersect (ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_invertible_t &other,
		  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> *workspace = ਖਾਲੀ_ਸੂਚਕ)
  {
    ਜੇ (likely (inverted == other.inverted))
    {
      ਜੇ (unlikely (inverted))
	process (hb_bitwise_or, other, workspace);
      ਨਹੀਂਤਾਂ
	process (hb_bitwise_and, other, workspace); /* Main branch. */
    }
    ਨਹੀਂਤਾਂ
    {
      ਜੇ (unlikely (inverted))
	process (hb_bitwise_lt, other, workspace);
      ਨਹੀਂਤਾਂ
	process (hb_bitwise_gt, other, workspace);
    }
    ਜੇ (likely (s.successful))
      inverted = inverted && other.inverted;
  }
  ਖਾਲੀ ਘਟਾਓ (ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_invertible_t &other)
  {
    ਜੇ (likely (inverted == other.inverted))
    {
      ਜੇ (unlikely (inverted))
	process (hb_bitwise_lt, other);
      ਨਹੀਂਤਾਂ
	process (hb_bitwise_gt, other); /* Main branch. */
    }
    ਨਹੀਂਤਾਂ
    {
      ਜੇ (unlikely (inverted))
	process (hb_bitwise_or, other);
      ਨਹੀਂਤਾਂ
	process (hb_bitwise_and, other);
    }
    ਜੇ (likely (s.successful))
      inverted = inverted && !other.inverted;
  }
  ਖਾਲੀ ਸਮਮਿਤ_difference (ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_invertible_t &other)
  {
    process (hb_bitwise_xor, other);
    ਜੇ (likely (s.successful))
      inverted = inverted ^ other.inverted;
  }

  ਤਾਰਕਿਕ next (hb_codepoint_t *codepoint) ਅਚਲ
  {
    ਜੇ (likely (!inverted))
      ਵਾਪਸ s.next (codepoint);

    ਸਵੈਚਾਲਿਤ old = *codepoint;
    ਜੇ (unlikely (old + 1 == INVALID))
    {
      *codepoint = INVALID;
      ਵਾਪਸ ਝੂਠ;
    }

    ਸਵੈਚਾਲਿਤ v = old;
    s.next (&v);
    ਜੇ (old + 1 < v)
    {
      *codepoint = old + 1;
      ਵਾਪਸ ਸੱਚ;
    }

    v = old;
    s.ਅਗਲਾ_range (&old, &v);

    *codepoint = v + 1;
    ਵਾਪਸ *codepoint != INVALID;
  }
  ਤਾਰਕਿਕ ਅਗਲਾ_ਬਿੱਟ (hb_codepoint_t *codepoint, uint64_t *ਬਿੱਟ) ਅਚਲ
  {
    ਜੇ (likely (!inverted))
      ਵਾਪਸ s.ਅਗਲਾ_ਬਿੱਟ (codepoint, ਬਿੱਟ);

    hb_codepoint_t ਅਧਾਰ;
    ਜੇ (unlikely (*codepoint == INVALID))
      ਅਧਾਰ = 0;
    ਨਹੀਂਤਾਂ
    {
      ਅਧਾਰ = *codepoint & ~hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t::ELT_ਨਕਾਬ;
      ਜੇ (unlikely (ਅਧਾਰ >= INVALID - hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t::ELT_ਬਿੱਟ + 1))
	ਜਾਓ ਪੂਰਾ;
      ਅਧਾਰ += hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t::ELT_ਬਿੱਟ;
    }

    ਜਦੋਂ (ਸੱਚ)
    {
      hb_codepoint_t cursor = ਅਧਾਰ ? ਅਧਾਰ - hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t::ELT_ਬਿੱਟ : INVALID;
      uint64_t excluded = 0;
      ਜੇ (!s.ਅਗਲਾ_ਬਿੱਟ (&cursor, &excluded) || cursor != ਅਧਾਰ)
	excluded = 0;
      *ਬਿੱਟ = ~excluded;
      ਤਾਰਕਿਕ ਆਖਰੀ = unlikely (ਅਧਾਰ >= INVALID - hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t::ELT_ਬਿੱਟ + 1);
      ਜੇ (ਆਖਰੀ)
	*ਬਿੱਟ &= UINT64_MAX >> 1;
      ਜੇ (*ਬਿੱਟ)
      {
	*codepoint = ਅਧਾਰ;
	ਵਾਪਸ ਸੱਚ;
      }
      ਜੇ (unlikely (ਆਖਰੀ))
	ਰੋਕੋ;
      ਅਧਾਰ += hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t::ELT_ਬਿੱਟ;
    }

  ਪੂਰਾ:
    *codepoint = INVALID;
    *ਬਿੱਟ = 0;
    ਵਾਪਸ ਝੂਠ;
  }
  ਤਾਰਕਿਕ ਪਿਛਲਾ (hb_codepoint_t *codepoint) ਅਚਲ
  {
    ਜੇ (likely (!inverted))
      ਵਾਪਸ s.ਪਿਛਲਾ (codepoint);

    ਸਵੈਚਾਲਿਤ old = *codepoint;
    ਜੇ (unlikely (old - 1 == INVALID))
    {
      *codepoint = INVALID;
      ਵਾਪਸ ਝੂਠ;
    }

    ਸਵੈਚਾਲਿਤ v = old;
    s.ਪਿਛਲਾ (&v);

    ਜੇ (old - 1 > v || v == INVALID)
    {
      *codepoint = old - 1;
      ਵਾਪਸ ਸੱਚ;
    }

    v = old;
    s.ਪਿਛਲਾ_range (&v, &old);

    *codepoint = v - 1;
    ਵਾਪਸ *codepoint != INVALID;
  }
  ਤਾਰਕਿਕ ਅਗਲਾ_range (hb_codepoint_t *ਪਹਿਲਾ, hb_codepoint_t *ਆਖਰੀ) ਅਚਲ
  {
    ਜੇ (likely (!inverted))
      ਵਾਪਸ s.ਅਗਲਾ_range (ਪਹਿਲਾ, ਆਖਰੀ);

    ਜੇ (!next (ਆਖਰੀ))
    {
      *ਆਖਰੀ = *ਪਹਿਲਾ = INVALID;
      ਵਾਪਸ ਝੂਠ;
    }

    *ਪਹਿਲਾ = *ਆਖਰੀ;
    s.next (ਆਖਰੀ);
    --*ਆਖਰੀ;
    ਵਾਪਸ ਸੱਚ;
  }
  ਤਾਰਕਿਕ ਪਿਛਲਾ_range (hb_codepoint_t *ਪਹਿਲਾ, hb_codepoint_t *ਆਖਰੀ) ਅਚਲ
  {
    ਜੇ (likely (!inverted))
      ਵਾਪਸ s.ਪਿਛਲਾ_range (ਪਹਿਲਾ, ਆਖਰੀ);

    ਜੇ (!ਪਿਛਲਾ (ਪਹਿਲਾ))
    {
      *ਆਖਰੀ = *ਪਹਿਲਾ = INVALID;
      ਵਾਪਸ ਝੂਠ;
    }

    *ਆਖਰੀ = *ਪਹਿਲਾ;
    s.ਪਿਛਲਾ (ਪਹਿਲਾ);
    ++*ਪਹਿਲਾ;
    ਵਾਪਸ ਸੱਚ;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅਗਲਾ_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *ਬਾਹਰਲਾ,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    ਮਾਪ) ਅਚਲ
  {
    ਵਾਪਸ inverted ? s.ਅਗਲਾ_many_inverted (codepoint, ਬਾਹਰਲਾ, ਮਾਪ)
		    : s.ਅਗਲਾ_many (codepoint, ਬਾਹਰਲਾ, ਮਾਪ);
  }

  ਸਥਿਰ constexpr hb_codepoint_t INVALID = hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t::INVALID;

  /*
   * Iterator implementation.
   * Mutating the set invalidates its iterators.
   */
  ਬਣਤਰ iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ਸਥਿਰ constexpr ਤਾਰਕਿਕ is_sorted_iterator = ਸੱਚ;
    ਸਥਿਰ constexpr ਤਾਰਕਿਕ has_fast_len = ਸੱਚ;
    iter_t (ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_invertible_t &s_ = Null (hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_invertible_t),
    ਤਾਰਕਿਕ init = ਸੱਚ) : s (&s_), v (INVALID), ਬਚਿਆ (0),
				     have_len (!init), ਅਧਾਰ (INVALID), ਬਿੱਟ (0)
    {
      ਜੇ (init)
	advance ();
    }

    ਕਿਸਮਪਰਿਭਾਸ਼ਾ hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ਅਚਲ { ਵਾਪਸ v; }
    ਤਾਰਕਿਕ __more__ () ਅਚਲ { ਵਾਪਸ v != INVALID; }
    ਖਾਲੀ __next__ ()
    {
      ਬਚਿਆ--;
      advance ();
    }
    ਖਾਲੀ __prev__ ()
    {
      ਜੇ (s->ਪਿਛਲਾ (&v))
      {
	ਸਮਕਾਲੀ ();
	ਬਚਿਆ++;
      }
      ਨਹੀਂਤਾਂ
      {
	ਅਧਾਰ = INVALID;
	ਬਿੱਟ = 0;
      }
    }
    ਬਿਨਾਨਿਸ਼ਾਨੀ __len__ () ਅਚਲ
    {
      ਜੇ (unlikely (!have_len))
      {
	ਬਚਿਆ += s->get_population ();
	have_len = ਸੱਚ;
      }
      ਵਾਪਸ ਬਚਿਆ;
    }
    iter_t ਸਿਰਾ () ਅਚਲ { ਵਾਪਸ iter_t (*s, ਝੂਠ); }
    ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ iter_t& o) ਅਚਲ
    { ਵਾਪਸ v != o.v; }

    ਸੁਰੱਖਿਅਤ:
    ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_invertible_t *s;
    hb_codepoint_t v;
    ਬਦਲਣਯੋਗ ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਚਿਆ;
    ਬਦਲਣਯੋਗ ਤਾਰਕਿਕ have_len;
    hb_codepoint_t ਅਧਾਰ;
    uint64_t ਬਿੱਟ;

    ਖਾਲੀ advance ()
    {
      ਜੇ (likely (ਬਿੱਟ))
      {
	v = ਅਧਾਰ + hb_ctz (ਬਿੱਟ);
	ਬਿੱਟ &= ਬਿੱਟ - 1;
      }
      ਨਹੀਂਤਾਂ ਜੇ (s->ਅਗਲਾ_ਬਿੱਟ (&ਅਧਾਰ, &ਬਿੱਟ))
      {
	v = ਅਧਾਰ + hb_ctz (ਬਿੱਟ);
	ਬਿੱਟ &= ਬਿੱਟ - 1;
      }
      ਨਹੀਂਤਾਂ
	v = INVALID;
    }

    ਖਾਲੀ ਸਮਕਾਲੀ ()
    {
      ਅਧਾਰ = v & ~hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t::ELT_ਨਕਾਬ;
      hb_codepoint_t cursor = ਅਧਾਰ ? ਅਧਾਰ - hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t::ELT_ਬਿੱਟ : INVALID;
      s->ਅਗਲਾ_ਬਿੱਟ (&cursor, &ਬਿੱਟ);
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ bit = v & (hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t::ELT_ਬਿੱਟ - 1);
      ਬਿੱਟ = bit == hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t::ELT_ਬਿੱਟ - 1 ? 0 : ਬਿੱਟ >> (bit + 1) << (bit + 1);
    }
  };
  iter_t iter () ਅਚਲ { ਵਾਪਸ iter_t (*ਇਹ); }
  ਸੰਚਾਲਕ iter_t () ਅਚਲ { ਵਾਪਸ iter (); }
};


#ਜੇ_ਅੰਤ /* HB_BIT_SET_INVERTIBLE_HH */

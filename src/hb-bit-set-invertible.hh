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

#ifndef HB_BIT_SET_INVERTIBLE_HH
#define HB_BIT_SET_INVERTIBLE_HH

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-bit-set.hh"


ಸಂರಚನಾ hb_bit_set_invertible_t
{
  hb_bit_set_t s;
  ತಾರ್ಕಿಕ inverted = ಅಸತ್ಯ;

  hb_bit_set_invertible_t () = ಸ್ವಭಾವ;
  hb_bit_set_invertible_t (ಧ್ರುವ hb_bit_set_invertible_t& o) = ಸ್ವಭಾವ;
  hb_bit_set_invertible_t (hb_bit_set_invertible_t&& other)  noexcept : hb_bit_set_invertible_t () { hb_swap (*ಏತತ್, other); }
  hb_bit_set_invertible_t& operator= (ಧ್ರುವ hb_bit_set_invertible_t& o) = ಸ್ವಭಾವ;
  hb_bit_set_invertible_t& operator= (hb_bit_set_invertible_t&& other)  noexcept { hb_swap (*ಏತತ್, other); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
  friend ಶೂನ್ಯ swap (hb_bit_set_invertible_t &a, hb_bit_set_invertible_t &b) noexcept
  {
    ಯದಿ (likely (!a.s.successful || !b.s.successful))
      ಪ್ರತ್ಯಾಗಮ;
    hb_swap (a.inverted, b.inverted);
    hb_swap (a.s, b.s);
  }

  ಶೂನ್ಯ init () { s.init (); inverted = ಅಸತ್ಯ; }
  ಶೂನ್ಯ fini () { s.fini (); }
  ಶೂನ್ಯ err () { s.err (); }
  ತಾರ್ಕಿಕ in_error () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ s.in_error (); }
  explicit operator ತಾರ್ಕಿಕ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ !is_empty (); }

  ಶೂನ್ಯ alloc (unsigned sz) { s.alloc (sz); }
  ಶೂನ್ಯ reset ()
  {
    s.reset ();
    inverted = ಅಸತ್ಯ;
  }
  ಶೂನ್ಯ clear ()
  {
    s.clear ();
    ಯದಿ (likely (s.successful))
      inverted = ಅಸತ್ಯ;
  }
  ಶೂನ್ಯ invert ()
  {
    ಯದಿ (likely (s.successful))
      inverted = !inverted;
  }

  ತಾರ್ಕಿಕ is_inverted () ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ inverted;
  }

  ತಾರ್ಕಿಕ is_empty () ಧ್ರುವ
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    ಪ್ರತ್ಯಾಗಮ v == INVALID;
  }
  uint32_t hash () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ s.hash () ^ (uint32_t) inverted; }

  hb_codepoint_t get_min () ಧ್ರುವ
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    ಪ್ರತ್ಯಾಗಮ v;
  }
  hb_codepoint_t get_max () ಧ್ರುವ
  {
    hb_codepoint_t v = INVALID;
    previous (&v);
    ಪ್ರತ್ಯಾಗಮ v;
  }
  unsigned ಪೂರ್ಣಾಙ್ಕ get_population () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ inverted ? INVALID - s.get_population () : s.get_population (); }
  ತಾರ್ಕಿಕ get_singleton (hb_codepoint_t *codepoint) ಧ್ರುವ
  {
    ಯದಿ (likely (!inverted))
      ಪ್ರತ್ಯಾಗಮ s.get_singleton (codepoint);
    ಯದಿ (s.get_population () != INVALID - 1)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    *codepoint = get_min ();
    ಪ್ರತ್ಯಾಗಮ *codepoint != INVALID;
  }


  ಶೂನ್ಯ add (hb_codepoint_t g) { unlikely (inverted) ? s.del (g) : s.add (g); }
  ಶೂನ್ಯ add_bits (hb_codepoint_t g, uint64_t bits)
  { unlikely (inverted) ? s.del_bits (g, bits) : s.add_bits (g, bits); }
  ತಾರ್ಕಿಕ add_range (hb_codepoint_t a, hb_codepoint_t b)
  { ಪ್ರತ್ಯಾಗಮ unlikely (inverted) ? ((ಶೂನ್ಯ) s.del_range (a, b), ಸತ್ಯ) : s.add_range (a, b); }

  template <typename T>
  ಶೂನ್ಯ add_array (ಧ್ರುವ T *array, unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, unsigned ಪೂರ್ಣಾಙ್ಕ stride=ಪರಿಮಾಣ(T))
  { inverted ? s.del_array (array, ಎಣಿಕೆ, stride) : s.add_array (array, ಎಣಿಕೆ, stride); }
  template <typename T>
  ಶೂನ್ಯ add_array (ಧ್ರುವ hb_array_t<ಧ್ರುವ T>& arr) { add_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  ತಾರ್ಕಿಕ add_sorted_array (ಧ್ರುವ T *array, unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, unsigned ಪೂರ್ಣಾಙ್ಕ stride=ಪರಿಮಾಣ(T))
  { ಪ್ರತ್ಯಾಗಮ inverted ? s.del_sorted_array (array, ಎಣಿಕೆ, stride) : s.add_sorted_array (array, ಎಣಿಕೆ, stride); }
  template <typename T>
  ತಾರ್ಕಿಕ add_sorted_array (ಧ್ರುವ hb_sorted_array_t<ಧ್ರುವ T>& arr) { ಪ್ರತ್ಯಾಗಮ add_sorted_array (&arr, arr.len ()); }

  ಶೂನ್ಯ del (hb_codepoint_t g) { unlikely (inverted) ? s.add (g) : s.del (g); }
  ಶೂನ್ಯ del_range (hb_codepoint_t a, hb_codepoint_t b)
  { unlikely (inverted) ? (ಶೂನ್ಯ) s.add_range (a, b) : s.del_range (a, b); }

  ತಾರ್ಕಿಕ get (hb_codepoint_t g) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ s.get (g) ^ inverted; }
  ತಾರ್ಕಿಕ may_have (hb_codepoint_t g) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get (g); }

  /* Has interface. */
  ತಾರ್ಕಿಕ operator [] (hb_codepoint_t k) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get (k); }
  ತಾರ್ಕಿಕ has (hb_codepoint_t k) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ (*ಏತತ್)[k]; }
  /* Predicate. */
  ತಾರ್ಕಿಕ operator () (hb_codepoint_t k) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ has (k); }

  /* Sink interface. */
  hb_bit_set_invertible_t& operator << (hb_codepoint_t v)
  { add (v); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
  hb_bit_set_invertible_t& operator << (ಧ್ರುವ hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }

  ತಾರ್ಕಿಕ may_intersect (ಧ್ರುವ hb_bit_set_invertible_t &other) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ inverted || other.inverted || s.intersects (other.s); }

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_bit_set_invertible_t &other) ಧ್ರುವ
  {
    ಯದಿ (likely (!inverted))
      ಪ್ರತ್ಯಾಗಮ unlikely (other.inverted) ? !s.is_subset (other.s)
				       : s.intersects (other.s);
    ಯದಿ (likely (!other.inverted))
      ಪ್ರತ್ಯಾಗಮ !other.s.is_subset (s);

    unsigned population = s.get_population ();
    unsigned other_population = other.s.get_population ();
    ಯದಿ ((uint64_t) population + other_population < INVALID)
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

    ಪ್ರತ್ಯಾಗಮ population >= other_population
	 ? !hb_all (iter (), other.s)
	 : !hb_all (other.iter (), s);
  }

  ತಾರ್ಕಿಕ intersects (hb_codepoint_t first, hb_codepoint_t last) ಧ್ರುವ
  {
    hb_codepoint_t c = first - 1;
    ಪ್ರತ್ಯಾಗಮ next (&c) && c <= last;
  }

  ಶೂನ್ಯ set (ಧ್ರುವ hb_bit_set_invertible_t &other)
  {
    s.set (other.s);
    ಯದಿ (likely (s.successful))
      inverted = other.inverted;
  }

  ತಾರ್ಕಿಕ is_equal (ಧ್ರುವ hb_bit_set_invertible_t &other) ಧ್ರುವ
  {
    ಯದಿ (likely (inverted == other.inverted))
      ಪ್ರತ್ಯಾಗಮ s.is_equal (other.s);
    ಅನ್ಯಥಾ
    {
      ಯದಿ (get_population () != other.get_population ())
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      /* TODO Add iter_ranges() and use here. */
      auto it1 = iter ();
      auto it2 = other.iter ();
      ಪ್ರತ್ಯಾಗಮ hb_all (+ hb_zip (it1, it2)
		     | hb_map ([](hb_codepoint_pair_t _) { ಪ್ರತ್ಯಾಗಮ _.first == _.second; }));
    }
  }

  ತಾರ್ಕಿಕ is_subset (ಧ್ರುವ hb_bit_set_invertible_t &larger_set) ಧ್ರುವ
  {
    ಯದಿ (unlikely (inverted != larger_set.inverted))
    {
      ಯದಿ (inverted)
	ಪ್ರತ್ಯಾಗಮ hb_all (iter (), larger_set.s);
      ಅನ್ಯಥಾ
        // larger set is inverted so larger_set.s is the set of things that are not present
        // in larger_set, therefore if s has any of those it can't be a subset.
	ಪ್ರತ್ಯಾಗಮ !s.intersects (larger_set.s);
    }
    ಅನ್ಯಥಾ
      ಪ್ರತ್ಯಾಗಮ unlikely (inverted) ? larger_set.s.is_subset (s) : s.is_subset (larger_set.s);
  }

  ರಕ್ಷಿತ:
  template <typename Op>
  ಶೂನ್ಯ process (ಧ್ರುವ Op& op,
		ಧ್ರುವ hb_bit_set_invertible_t &other,
		hb_vector_t<unsigned> *workspace = nullptr)
  { s.process (op, other.s, workspace); }
  ಸಾರ್ವಜನಿಕ:
  ಶೂನ್ಯ union_ (ಧ್ರುವ hb_bit_set_invertible_t &other)
  {
    ಯದಿ (likely (inverted == other.inverted))
    {
      ಯದಿ (unlikely (inverted))
	process (hb_bitwise_and, other);
      ಅನ್ಯಥಾ
	process (hb_bitwise_or, other); /* Main branch. */
    }
    ಅನ್ಯಥಾ
    {
      ಯದಿ (unlikely (inverted))
	process (hb_bitwise_gt, other);
      ಅನ್ಯಥಾ
	process (hb_bitwise_lt, other);
    }
    ಯದಿ (likely (s.successful))
      inverted = inverted || other.inverted;
  }
  ಶೂನ್ಯ intersect (ಧ್ರುವ hb_bit_set_invertible_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  {
    ಯದಿ (likely (inverted == other.inverted))
    {
      ಯದಿ (unlikely (inverted))
	process (hb_bitwise_or, other, workspace);
      ಅನ್ಯಥಾ
	process (hb_bitwise_and, other, workspace); /* Main branch. */
    }
    ಅನ್ಯಥಾ
    {
      ಯದಿ (unlikely (inverted))
	process (hb_bitwise_lt, other, workspace);
      ಅನ್ಯಥಾ
	process (hb_bitwise_gt, other, workspace);
    }
    ಯದಿ (likely (s.successful))
      inverted = inverted && other.inverted;
  }
  ಶೂನ್ಯ subtract (ಧ್ರುವ hb_bit_set_invertible_t &other)
  {
    ಯದಿ (likely (inverted == other.inverted))
    {
      ಯದಿ (unlikely (inverted))
	process (hb_bitwise_lt, other);
      ಅನ್ಯಥಾ
	process (hb_bitwise_gt, other); /* Main branch. */
    }
    ಅನ್ಯಥಾ
    {
      ಯದಿ (unlikely (inverted))
	process (hb_bitwise_or, other);
      ಅನ್ಯಥಾ
	process (hb_bitwise_and, other);
    }
    ಯದಿ (likely (s.successful))
      inverted = inverted && !other.inverted;
  }
  ಶೂನ್ಯ symmetric_difference (ಧ್ರುವ hb_bit_set_invertible_t &other)
  {
    process (hb_bitwise_xor, other);
    ಯದಿ (likely (s.successful))
      inverted = inverted ^ other.inverted;
  }

  ತಾರ್ಕಿಕ next (hb_codepoint_t *codepoint) ಧ್ರುವ
  {
    ಯದಿ (likely (!inverted))
      ಪ್ರತ್ಯಾಗಮ s.next (codepoint);

    auto old = *codepoint;
    ಯದಿ (unlikely (old + 1 == INVALID))
    {
      *codepoint = INVALID;
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    auto v = old;
    s.next (&v);
    ಯದಿ (old + 1 < v)
    {
      *codepoint = old + 1;
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    v = old;
    s.next_range (&old, &v);

    *codepoint = v + 1;
    ಪ್ರತ್ಯಾಗಮ *codepoint != INVALID;
  }
  ತಾರ್ಕಿಕ next_bits (hb_codepoint_t *codepoint, uint64_t *bits) ಧ್ರುವ
  {
    ಯದಿ (likely (!inverted))
      ಪ್ರತ್ಯಾಗಮ s.next_bits (codepoint, bits);

    hb_codepoint_t base;
    ಯದಿ (unlikely (*codepoint == INVALID))
      base = 0;
    ಅನ್ಯಥಾ
    {
      base = *codepoint & ~hb_bit_page_t::ELT_MASK;
      ಯದಿ (unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1))
	ಗಚ್ಛ done;
      base += hb_bit_page_t::ELT_BITS;
    }

    ಯಾವತ್ (ಸತ್ಯ)
    {
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      uint64_t excluded = 0;
      ಯದಿ (!s.next_bits (&cursor, &excluded) || cursor != base)
	excluded = 0;
      *bits = ~excluded;
      ತಾರ್ಕಿಕ last = unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1);
      ಯದಿ (last)
	*bits &= UINT64_MAX >> 1;
      ಯದಿ (*bits)
      {
	*codepoint = base;
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      }
      ಯದಿ (unlikely (last))
	ಭಙ್ಗ;
      base += hb_bit_page_t::ELT_BITS;
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }
  ತಾರ್ಕಿಕ previous (hb_codepoint_t *codepoint) ಧ್ರುವ
  {
    ಯದಿ (likely (!inverted))
      ಪ್ರತ್ಯಾಗಮ s.previous (codepoint);

    auto old = *codepoint;
    ಯದಿ (unlikely (old - 1 == INVALID))
    {
      *codepoint = INVALID;
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    auto v = old;
    s.previous (&v);

    ಯದಿ (old - 1 > v || v == INVALID)
    {
      *codepoint = old - 1;
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    v = old;
    s.previous_range (&v, &old);

    *codepoint = v - 1;
    ಪ್ರತ್ಯಾಗಮ *codepoint != INVALID;
  }
  ತಾರ್ಕಿಕ next_range (hb_codepoint_t *first, hb_codepoint_t *last) ಧ್ರುವ
  {
    ಯದಿ (likely (!inverted))
      ಪ್ರತ್ಯಾಗಮ s.next_range (first, last);

    ಯದಿ (!next (last))
    {
      *last = *first = INVALID;
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    *first = *last;
    s.next (last);
    --*last;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }
  ತಾರ್ಕಿಕ previous_range (hb_codepoint_t *first, hb_codepoint_t *last) ಧ್ರುವ
  {
    ಯದಿ (likely (!inverted))
      ಪ್ರತ್ಯಾಗಮ s.previous_range (first, last);

    ಯದಿ (!previous (first))
    {
      *last = *first = INVALID;
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    *last = *first;
    s.previous (first);
    ++*first;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  unsigned ಪೂರ್ಣಾಙ್ಕ    size) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ inverted ? s.next_many_inverted (codepoint, out, size)
		    : s.next_many (codepoint, out, size);
  }

  ಸ್ಥಿರ constexpr hb_codepoint_t INVALID = hb_bit_set_t::INVALID;

  /*
   * Iterator implementation.
   * Mutating the set invalidates its iterators.
   */
  ಸಂರಚನಾ iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ is_sorted_iterator = ಸತ್ಯ;
    ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ has_fast_len = ಸತ್ಯ;
    iter_t (ಧ್ರುವ hb_bit_set_invertible_t &s_ = Null (hb_bit_set_invertible_t),
    ತಾರ್ಕಿಕ init = ಸತ್ಯ) : s (&s_), v (INVALID), remaining (0),
				     have_len (!init), base (INVALID), bits (0)
    {
      ಯದಿ (init)
	advance ();
    }

    ಪ್ರಕಾರನಿರ್ದೇಶ hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ v; }
    ತಾರ್ಕಿಕ __more__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ v != INVALID; }
    ಶೂನ್ಯ __next__ ()
    {
      remaining--;
      advance ();
    }
    ಶೂನ್ಯ __prev__ ()
    {
      ಯದಿ (s->previous (&v))
      {
	ಸಮಕಾಲ ();
	remaining++;
      }
      ಅನ್ಯಥಾ
      {
	base = INVALID;
	bits = 0;
      }
    }
    unsigned __len__ () ಧ್ರುವ
    {
      ಯದಿ (unlikely (!have_len))
      {
	remaining += s->get_population ();
	have_len = ಸತ್ಯ;
      }
      ಪ್ರತ್ಯಾಗಮ remaining;
    }
    iter_t end () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ iter_t (*s, ಅಸತ್ಯ); }
    ತಾರ್ಕಿಕ operator != (ಧ್ರುವ iter_t& o) ಧ್ರುವ
    { ಪ್ರತ್ಯಾಗಮ v != o.v; }

    ರಕ್ಷಿತ:
    ಧ್ರುವ hb_bit_set_invertible_t *s;
    hb_codepoint_t v;
    mutable unsigned remaining;
    mutable ತಾರ್ಕಿಕ have_len;
    hb_codepoint_t base;
    uint64_t bits;

    ಶೂನ್ಯ advance ()
    {
      ಯದಿ (likely (bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      ಅನ್ಯಥಾ ಯದಿ (s->next_bits (&base, &bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      ಅನ್ಯಥಾ
	v = INVALID;
    }

    ಶೂನ್ಯ ಸಮಕಾಲ ()
    {
      base = v & ~hb_bit_page_t::ELT_MASK;
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      s->next_bits (&cursor, &bits);
      unsigned ಪೂರ್ಣಾಙ್ಕ bit = v & (hb_bit_page_t::ELT_BITS - 1);
      bits = bit == hb_bit_page_t::ELT_BITS - 1 ? 0 : bits >> (bit + 1) << (bit + 1);
    }
  };
  iter_t iter () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ iter_t (*ಏತತ್); }
  operator iter_t () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ iter (); }
};


#endif /* HB_BIT_SET_INVERTIBLE_HH */

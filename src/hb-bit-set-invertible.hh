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

#અન્તર્ભાવ "hb.hh"
#અન્તર્ભાવ "hb-bit-set.hh"


સંરચના hb_bit_set_invertible_t
{
  hb_bit_set_t s;
  તાર્કિક inverted = ખોટું;

  hb_bit_set_invertible_t () = મૂળભૂત;
  hb_bit_set_invertible_t (અચલ hb_bit_set_invertible_t& o) = મૂળભૂત;
  hb_bit_set_invertible_t (hb_bit_set_invertible_t&& other)  noexcept : hb_bit_set_invertible_t () { hb_swap (*આ, other); }
  hb_bit_set_invertible_t& operator= (અચલ hb_bit_set_invertible_t& o) = મૂળભૂત;
  hb_bit_set_invertible_t& operator= (hb_bit_set_invertible_t&& other)  noexcept { hb_swap (*આ, other); પાછા *આ; }
  friend શૂન્ય swap (hb_bit_set_invertible_t &a, hb_bit_set_invertible_t &b) noexcept
  {
    જો (likely (!a.s.successful || !b.s.successful))
      પાછા;
    hb_swap (a.inverted, b.inverted);
    hb_swap (a.s, b.s);
  }

  શૂન્ય init () { s.init (); inverted = ખોટું; }
  શૂન્ય fini () { s.fini (); }
  શૂન્ય err () { s.err (); }
  તાર્કિક in_error () અચલ { પાછા s.in_error (); }
  explicit operator તાર્કિક () અચલ { પાછા !is_empty (); }

  શૂન્ય alloc (unsigned sz) { s.alloc (sz); }
  શૂન્ય reset ()
  {
    s.reset ();
    inverted = ખોટું;
  }
  શૂન્ય clear ()
  {
    s.clear ();
    જો (likely (s.successful))
      inverted = ખોટું;
  }
  શૂન્ય invert ()
  {
    જો (likely (s.successful))
      inverted = !inverted;
  }

  તાર્કિક is_inverted () અચલ
  {
    પાછા inverted;
  }

  તાર્કિક is_empty () અચલ
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    પાછા v == INVALID;
  }
  uint32_t hash () અચલ { પાછા s.hash () ^ (uint32_t) inverted; }

  hb_codepoint_t get_min () અચલ
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    પાછા v;
  }
  hb_codepoint_t get_max () અચલ
  {
    hb_codepoint_t v = INVALID;
    previous (&v);
    પાછા v;
  }
  unsigned પૂર્ણાંક get_population () અચલ
  { પાછા inverted ? INVALID - s.get_population () : s.get_population (); }
  તાર્કિક get_singleton (hb_codepoint_t *codepoint) અચલ
  {
    જો (likely (!inverted))
      પાછા s.get_singleton (codepoint);
    જો (s.get_population () != INVALID - 1)
      પાછા ખોટું;
    *codepoint = get_min ();
    પાછા *codepoint != INVALID;
  }


  શૂન્ય add (hb_codepoint_t g) { unlikely (inverted) ? s.del (g) : s.add (g); }
  શૂન્ય add_bits (hb_codepoint_t g, uint64_t bits)
  { unlikely (inverted) ? s.del_bits (g, bits) : s.add_bits (g, bits); }
  તાર્કિક add_range (hb_codepoint_t a, hb_codepoint_t b)
  { પાછા unlikely (inverted) ? ((શૂન્ય) s.del_range (a, b), સાચું) : s.add_range (a, b); }

  template <typename T>
  શૂન્ય add_array (અચલ T *array, unsigned પૂર્ણાંક ગણતરી, unsigned પૂર્ણાંક stride=કદ(T))
  { inverted ? s.del_array (array, ગણતરી, stride) : s.add_array (array, ગણતરી, stride); }
  template <typename T>
  શૂન્ય add_array (અચલ hb_array_t<અચલ T>& arr) { add_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  તાર્કિક add_sorted_array (અચલ T *array, unsigned પૂર્ણાંક ગણતરી, unsigned પૂર્ણાંક stride=કદ(T))
  { પાછા inverted ? s.del_sorted_array (array, ગણતરી, stride) : s.add_sorted_array (array, ગણતરી, stride); }
  template <typename T>
  તાર્કિક add_sorted_array (અચલ hb_sorted_array_t<અચલ T>& arr) { પાછા add_sorted_array (&arr, arr.len ()); }

  શૂન્ય del (hb_codepoint_t g) { unlikely (inverted) ? s.add (g) : s.del (g); }
  શૂન્ય del_range (hb_codepoint_t a, hb_codepoint_t b)
  { unlikely (inverted) ? (શૂન્ય) s.add_range (a, b) : s.del_range (a, b); }

  તાર્કિક get (hb_codepoint_t g) અચલ { પાછા s.get (g) ^ inverted; }
  તાર્કિક may_have (hb_codepoint_t g) અચલ { પાછા get (g); }

  /* Has interface. */
  તાર્કિક operator [] (hb_codepoint_t k) અચલ { પાછા get (k); }
  તાર્કિક has (hb_codepoint_t k) અચલ { પાછા (*આ)[k]; }
  /* Predicate. */
  તાર્કિક operator () (hb_codepoint_t k) અચલ { પાછા has (k); }

  /* Sink interface. */
  hb_bit_set_invertible_t& operator << (hb_codepoint_t v)
  { add (v); પાછા *આ; }
  hb_bit_set_invertible_t& operator << (અચલ hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); પાછા *આ; }

  તાર્કિક may_intersect (અચલ hb_bit_set_invertible_t &other) અચલ
  { પાછા inverted || other.inverted || s.intersects (other.s); }

  તાર્કિક intersects (અચલ hb_bit_set_invertible_t &other) અચલ
  {
    જો (likely (!inverted))
      પાછા unlikely (other.inverted) ? !s.is_subset (other.s)
				       : s.intersects (other.s);
    જો (likely (!other.inverted))
      પાછા !other.s.is_subset (s);

    unsigned population = s.get_population ();
    unsigned other_population = other.s.get_population ();
    જો ((uint64_t) population + other_population < INVALID)
      પાછા સાચું;

    પાછા population >= other_population
	 ? !hb_all (iter (), other.s)
	 : !hb_all (other.iter (), s);
  }

  તાર્કિક intersects (hb_codepoint_t first, hb_codepoint_t last) અચલ
  {
    hb_codepoint_t c = first - 1;
    પાછા next (&c) && c <= last;
  }

  શૂન્ય set (અચલ hb_bit_set_invertible_t &other)
  {
    s.set (other.s);
    જો (likely (s.successful))
      inverted = other.inverted;
  }

  તાર્કિક is_equal (અચલ hb_bit_set_invertible_t &other) અચલ
  {
    જો (likely (inverted == other.inverted))
      પાછા s.is_equal (other.s);
    નહીંતર
    {
      જો (get_population () != other.get_population ())
	પાછા ખોટું;

      /* TODO Add iter_ranges() and use here. */
      auto it1 = iter ();
      auto it2 = other.iter ();
      પાછા hb_all (+ hb_zip (it1, it2)
		     | hb_map ([](hb_codepoint_pair_t _) { પાછા _.first == _.second; }));
    }
  }

  તાર્કિક is_subset (અચલ hb_bit_set_invertible_t &larger_set) અચલ
  {
    જો (unlikely (inverted != larger_set.inverted))
    {
      જો (inverted)
	પાછા hb_all (iter (), larger_set.s);
      નહીંતર
        // larger set is inverted so larger_set.s is the set of things that are not present
        // in larger_set, therefore if s has any of those it can't be a subset.
	પાછા !s.intersects (larger_set.s);
    }
    નહીંતર
      પાછા unlikely (inverted) ? larger_set.s.is_subset (s) : s.is_subset (larger_set.s);
  }

  સુરક્ષિત:
  template <typename Op>
  શૂન્ય process (અચલ Op& op,
		અચલ hb_bit_set_invertible_t &other,
		hb_vector_t<unsigned> *workspace = nullptr)
  { s.process (op, other.s, workspace); }
  જાહેર:
  શૂન્ય union_ (અચલ hb_bit_set_invertible_t &other)
  {
    જો (likely (inverted == other.inverted))
    {
      જો (unlikely (inverted))
	process (hb_bitwise_and, other);
      નહીંતર
	process (hb_bitwise_or, other); /* Main branch. */
    }
    નહીંતર
    {
      જો (unlikely (inverted))
	process (hb_bitwise_gt, other);
      નહીંતર
	process (hb_bitwise_lt, other);
    }
    જો (likely (s.successful))
      inverted = inverted || other.inverted;
  }
  શૂન્ય intersect (અચલ hb_bit_set_invertible_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  {
    જો (likely (inverted == other.inverted))
    {
      જો (unlikely (inverted))
	process (hb_bitwise_or, other, workspace);
      નહીંતર
	process (hb_bitwise_and, other, workspace); /* Main branch. */
    }
    નહીંતર
    {
      જો (unlikely (inverted))
	process (hb_bitwise_lt, other, workspace);
      નહીંતર
	process (hb_bitwise_gt, other, workspace);
    }
    જો (likely (s.successful))
      inverted = inverted && other.inverted;
  }
  શૂન્ય subtract (અચલ hb_bit_set_invertible_t &other)
  {
    જો (likely (inverted == other.inverted))
    {
      જો (unlikely (inverted))
	process (hb_bitwise_lt, other);
      નહીંતર
	process (hb_bitwise_gt, other); /* Main branch. */
    }
    નહીંતર
    {
      જો (unlikely (inverted))
	process (hb_bitwise_or, other);
      નહીંતર
	process (hb_bitwise_and, other);
    }
    જો (likely (s.successful))
      inverted = inverted && !other.inverted;
  }
  શૂન્ય symmetric_difference (અચલ hb_bit_set_invertible_t &other)
  {
    process (hb_bitwise_xor, other);
    જો (likely (s.successful))
      inverted = inverted ^ other.inverted;
  }

  તાર્કિક next (hb_codepoint_t *codepoint) અચલ
  {
    જો (likely (!inverted))
      પાછા s.next (codepoint);

    auto old = *codepoint;
    જો (unlikely (old + 1 == INVALID))
    {
      *codepoint = INVALID;
      પાછા ખોટું;
    }

    auto v = old;
    s.next (&v);
    જો (old + 1 < v)
    {
      *codepoint = old + 1;
      પાછા સાચું;
    }

    v = old;
    s.next_range (&old, &v);

    *codepoint = v + 1;
    પાછા *codepoint != INVALID;
  }
  તાર્કિક next_bits (hb_codepoint_t *codepoint, uint64_t *bits) અચલ
  {
    જો (likely (!inverted))
      પાછા s.next_bits (codepoint, bits);

    hb_codepoint_t base;
    જો (unlikely (*codepoint == INVALID))
      base = 0;
    નહીંતર
    {
      base = *codepoint & ~hb_bit_page_t::ELT_MASK;
      જો (unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1))
	જાઓ done;
      base += hb_bit_page_t::ELT_BITS;
    }

    જ્યારે (સાચું)
    {
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      uint64_t excluded = 0;
      જો (!s.next_bits (&cursor, &excluded) || cursor != base)
	excluded = 0;
      *bits = ~excluded;
      તાર્કિક last = unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1);
      જો (last)
	*bits &= UINT64_MAX >> 1;
      જો (*bits)
      {
	*codepoint = base;
	પાછા સાચું;
      }
      જો (unlikely (last))
	તોડો;
      base += hb_bit_page_t::ELT_BITS;
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    પાછા ખોટું;
  }
  તાર્કિક previous (hb_codepoint_t *codepoint) અચલ
  {
    જો (likely (!inverted))
      પાછા s.previous (codepoint);

    auto old = *codepoint;
    જો (unlikely (old - 1 == INVALID))
    {
      *codepoint = INVALID;
      પાછા ખોટું;
    }

    auto v = old;
    s.previous (&v);

    જો (old - 1 > v || v == INVALID)
    {
      *codepoint = old - 1;
      પાછા સાચું;
    }

    v = old;
    s.previous_range (&v, &old);

    *codepoint = v - 1;
    પાછા *codepoint != INVALID;
  }
  તાર્કિક next_range (hb_codepoint_t *first, hb_codepoint_t *last) અચલ
  {
    જો (likely (!inverted))
      પાછા s.next_range (first, last);

    જો (!next (last))
    {
      *last = *first = INVALID;
      પાછા ખોટું;
    }

    *first = *last;
    s.next (last);
    --*last;
    પાછા સાચું;
  }
  તાર્કિક previous_range (hb_codepoint_t *first, hb_codepoint_t *last) અચલ
  {
    જો (likely (!inverted))
      પાછા s.previous_range (first, last);

    જો (!previous (first))
    {
      *last = *first = INVALID;
      પાછા ખોટું;
    }

    *last = *first;
    s.previous (first);
    ++*first;
    પાછા સાચું;
  }

  unsigned પૂર્ણાંક next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  unsigned પૂર્ણાંક    size) અચલ
  {
    પાછા inverted ? s.next_many_inverted (codepoint, out, size)
		    : s.next_many (codepoint, out, size);
  }

  સ્થિર constexpr hb_codepoint_t INVALID = hb_bit_set_t::INVALID;

  /*
   * Iterator implementation.
   * Mutating the set invalidates its iterators.
   */
  સંરચના iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    સ્થિર constexpr તાર્કિક is_sorted_iterator = સાચું;
    સ્થિર constexpr તાર્કિક has_fast_len = સાચું;
    iter_t (અચલ hb_bit_set_invertible_t &s_ = Null (hb_bit_set_invertible_t),
    તાર્કિક init = સાચું) : s (&s_), v (INVALID), remaining (0),
				     have_len (!init), base (INVALID), bits (0)
    {
      જો (init)
	advance ();
    }

    પ્રકારવ્યાખ્યા hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () અચલ { પાછા v; }
    તાર્કિક __more__ () અચલ { પાછા v != INVALID; }
    શૂન્ય __next__ ()
    {
      remaining--;
      advance ();
    }
    શૂન્ય __prev__ ()
    {
      જો (s->previous (&v))
      {
	સમકાલિક ();
	remaining++;
      }
      નહીંતર
      {
	base = INVALID;
	bits = 0;
      }
    }
    unsigned __len__ () અચલ
    {
      જો (unlikely (!have_len))
      {
	remaining += s->get_population ();
	have_len = સાચું;
      }
      પાછા remaining;
    }
    iter_t end () અચલ { પાછા iter_t (*s, ખોટું); }
    તાર્કિક operator != (અચલ iter_t& o) અચલ
    { પાછા v != o.v; }

    સુરક્ષિત:
    અચલ hb_bit_set_invertible_t *s;
    hb_codepoint_t v;
    mutable unsigned remaining;
    mutable તાર્કિક have_len;
    hb_codepoint_t base;
    uint64_t bits;

    શૂન્ય advance ()
    {
      જો (likely (bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      નહીંતર જો (s->next_bits (&base, &bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      નહીંતર
	v = INVALID;
    }

    શૂન્ય સમકાલિક ()
    {
      base = v & ~hb_bit_page_t::ELT_MASK;
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      s->next_bits (&cursor, &bits);
      unsigned પૂર્ણાંક bit = v & (hb_bit_page_t::ELT_BITS - 1);
      bits = bit == hb_bit_page_t::ELT_BITS - 1 ? 0 : bits >> (bit + 1) << (bit + 1);
    }
  };
  iter_t iter () અચલ { પાછા iter_t (*આ); }
  operator iter_t () અચલ { પાછા iter (); }
};


#endif /* HB_BIT_SET_INVERTIBLE_HH */

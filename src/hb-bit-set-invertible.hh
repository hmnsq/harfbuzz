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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-bit-set.hh"


संरचना hb_bit_set_invertible_t
{
  hb_bit_set_t s;
  तार्किक inverted = मिच्छ;

  hb_bit_set_invertible_t () = स्वभाव;
  hb_bit_set_invertible_t (ध्रुव hb_bit_set_invertible_t& o) = स्वभाव;
  hb_bit_set_invertible_t (hb_bit_set_invertible_t&& other)  noexcept : hb_bit_set_invertible_t () { hb_swap (*एतत्, other); }
  hb_bit_set_invertible_t& operator= (ध्रुव hb_bit_set_invertible_t& o) = स्वभाव;
  hb_bit_set_invertible_t& operator= (hb_bit_set_invertible_t&& other)  noexcept { hb_swap (*एतत्, other); प्रत्यागम *एतत्; }
  friend शून्य swap (hb_bit_set_invertible_t &a, hb_bit_set_invertible_t &b) noexcept
  {
    जइ (likely (!a.s.successful || !b.s.successful))
      प्रत्यागम;
    hb_swap (a.inverted, b.inverted);
    hb_swap (a.s, b.s);
  }

  शून्य init () { s.init (); inverted = मिच्छ; }
  शून्य fini () { s.fini (); }
  शून्य err () { s.err (); }
  तार्किक in_error () ध्रुव { प्रत्यागम s.in_error (); }
  explicit operator तार्किक () ध्रुव { प्रत्यागम !is_empty (); }

  शून्य alloc (unsigned sz) { s.alloc (sz); }
  शून्य reset ()
  {
    s.reset ();
    inverted = मिच्छ;
  }
  शून्य clear ()
  {
    s.clear ();
    जइ (likely (s.successful))
      inverted = मिच्छ;
  }
  शून्य invert ()
  {
    जइ (likely (s.successful))
      inverted = !inverted;
  }

  तार्किक is_inverted () ध्रुव
  {
    प्रत्यागम inverted;
  }

  तार्किक is_empty () ध्रुव
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    प्रत्यागम v == INVALID;
  }
  uint32_t hash () ध्रुव { प्रत्यागम s.hash () ^ (uint32_t) inverted; }

  hb_codepoint_t get_min () ध्रुव
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    प्रत्यागम v;
  }
  hb_codepoint_t get_max () ध्रुव
  {
    hb_codepoint_t v = INVALID;
    previous (&v);
    प्रत्यागम v;
  }
  unsigned पूर्णाङ्क get_population () ध्रुव
  { प्रत्यागम inverted ? INVALID - s.get_population () : s.get_population (); }
  तार्किक get_singleton (hb_codepoint_t *codepoint) ध्रुव
  {
    जइ (likely (!inverted))
      प्रत्यागम s.get_singleton (codepoint);
    जइ (s.get_population () != INVALID - 1)
      प्रत्यागम मिच्छ;
    *codepoint = get_min ();
    प्रत्यागम *codepoint != INVALID;
  }


  शून्य add (hb_codepoint_t g) { unlikely (inverted) ? s.del (g) : s.add (g); }
  शून्य add_bits (hb_codepoint_t g, uint64_t bits)
  { unlikely (inverted) ? s.del_bits (g, bits) : s.add_bits (g, bits); }
  तार्किक add_range (hb_codepoint_t a, hb_codepoint_t b)
  { प्रत्यागम unlikely (inverted) ? ((शून्य) s.del_range (a, b), सच्च) : s.add_range (a, b); }

  template <typename T>
  शून्य add_array (ध्रुव T *array, unsigned पूर्णाङ्क गणनम्, unsigned पूर्णाङ्क stride=परिमाण(T))
  { inverted ? s.del_array (array, गणनम्, stride) : s.add_array (array, गणनम्, stride); }
  template <typename T>
  शून्य add_array (ध्रुव hb_array_t<ध्रुव T>& arr) { add_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  तार्किक add_sorted_array (ध्रुव T *array, unsigned पूर्णाङ्क गणनम्, unsigned पूर्णाङ्क stride=परिमाण(T))
  { प्रत्यागम inverted ? s.del_sorted_array (array, गणनम्, stride) : s.add_sorted_array (array, गणनम्, stride); }
  template <typename T>
  तार्किक add_sorted_array (ध्रुव hb_sorted_array_t<ध्रुव T>& arr) { प्रत्यागम add_sorted_array (&arr, arr.len ()); }

  शून्य del (hb_codepoint_t g) { unlikely (inverted) ? s.add (g) : s.del (g); }
  शून्य del_range (hb_codepoint_t a, hb_codepoint_t b)
  { unlikely (inverted) ? (शून्य) s.add_range (a, b) : s.del_range (a, b); }

  तार्किक get (hb_codepoint_t g) ध्रुव { प्रत्यागम s.get (g) ^ inverted; }
  तार्किक may_have (hb_codepoint_t g) ध्रुव { प्रत्यागम get (g); }

  /* Has interface. */
  तार्किक operator [] (hb_codepoint_t k) ध्रुव { प्रत्यागम get (k); }
  तार्किक has (hb_codepoint_t k) ध्रुव { प्रत्यागम (*एतत्)[k]; }
  /* Predicate. */
  तार्किक operator () (hb_codepoint_t k) ध्रुव { प्रत्यागम has (k); }

  /* Sink interface. */
  hb_bit_set_invertible_t& operator << (hb_codepoint_t v)
  { add (v); प्रत्यागम *एतत्; }
  hb_bit_set_invertible_t& operator << (ध्रुव hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); प्रत्यागम *एतत्; }

  तार्किक may_intersect (ध्रुव hb_bit_set_invertible_t &other) ध्रुव
  { प्रत्यागम inverted || other.inverted || s.intersects (other.s); }

  तार्किक intersects (ध्रुव hb_bit_set_invertible_t &other) ध्रुव
  {
    जइ (likely (!inverted))
      प्रत्यागम unlikely (other.inverted) ? !s.is_subset (other.s)
				       : s.intersects (other.s);
    जइ (likely (!other.inverted))
      प्रत्यागम !other.s.is_subset (s);

    unsigned population = s.get_population ();
    unsigned other_population = other.s.get_population ();
    जइ ((uint64_t) population + other_population < INVALID)
      प्रत्यागम सच्च;

    प्रत्यागम population >= other_population
	 ? !hb_all (iter (), other.s)
	 : !hb_all (other.iter (), s);
  }

  तार्किक intersects (hb_codepoint_t first, hb_codepoint_t last) ध्रुव
  {
    hb_codepoint_t c = first - 1;
    प्रत्यागम next (&c) && c <= last;
  }

  शून्य set (ध्रुव hb_bit_set_invertible_t &other)
  {
    s.set (other.s);
    जइ (likely (s.successful))
      inverted = other.inverted;
  }

  तार्किक is_equal (ध्रुव hb_bit_set_invertible_t &other) ध्रुव
  {
    जइ (likely (inverted == other.inverted))
      प्रत्यागम s.is_equal (other.s);
    अण्णहा
    {
      जइ (get_population () != other.get_population ())
	प्रत्यागम मिच्छ;

      /* TODO Add iter_ranges() and use here. */
      auto it1 = iter ();
      auto it2 = other.iter ();
      प्रत्यागम hb_all (+ hb_zip (it1, it2)
		     | hb_map ([](hb_codepoint_pair_t _) { प्रत्यागम _.first == _.second; }));
    }
  }

  तार्किक is_subset (ध्रुव hb_bit_set_invertible_t &larger_set) ध्रुव
  {
    जइ (unlikely (inverted != larger_set.inverted))
    {
      जइ (inverted)
	प्रत्यागम hb_all (iter (), larger_set.s);
      अण्णहा
        // larger set is inverted so larger_set.s is the set of things that are not present
        // in larger_set, therefore if s has any of those it can't be a subset.
	प्रत्यागम !s.intersects (larger_set.s);
    }
    अण्णहा
      प्रत्यागम unlikely (inverted) ? larger_set.s.is_subset (s) : s.is_subset (larger_set.s);
  }

  रक्षित:
  template <typename Op>
  शून्य process (ध्रुव Op& op,
		ध्रुव hb_bit_set_invertible_t &other,
		hb_vector_t<unsigned> *workspace = nullptr)
  { s.process (op, other.s, workspace); }
  सार्वजनिक:
  शून्य union_ (ध्रुव hb_bit_set_invertible_t &other)
  {
    जइ (likely (inverted == other.inverted))
    {
      जइ (unlikely (inverted))
	process (hb_bitwise_and, other);
      अण्णहा
	process (hb_bitwise_or, other); /* Main branch. */
    }
    अण्णहा
    {
      जइ (unlikely (inverted))
	process (hb_bitwise_gt, other);
      अण्णहा
	process (hb_bitwise_lt, other);
    }
    जइ (likely (s.successful))
      inverted = inverted || other.inverted;
  }
  शून्य intersect (ध्रुव hb_bit_set_invertible_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  {
    जइ (likely (inverted == other.inverted))
    {
      जइ (unlikely (inverted))
	process (hb_bitwise_or, other, workspace);
      अण्णहा
	process (hb_bitwise_and, other, workspace); /* Main branch. */
    }
    अण्णहा
    {
      जइ (unlikely (inverted))
	process (hb_bitwise_lt, other, workspace);
      अण्णहा
	process (hb_bitwise_gt, other, workspace);
    }
    जइ (likely (s.successful))
      inverted = inverted && other.inverted;
  }
  शून्य subtract (ध्रुव hb_bit_set_invertible_t &other)
  {
    जइ (likely (inverted == other.inverted))
    {
      जइ (unlikely (inverted))
	process (hb_bitwise_lt, other);
      अण्णहा
	process (hb_bitwise_gt, other); /* Main branch. */
    }
    अण्णहा
    {
      जइ (unlikely (inverted))
	process (hb_bitwise_or, other);
      अण्णहा
	process (hb_bitwise_and, other);
    }
    जइ (likely (s.successful))
      inverted = inverted && !other.inverted;
  }
  शून्य symmetric_difference (ध्रुव hb_bit_set_invertible_t &other)
  {
    process (hb_bitwise_xor, other);
    जइ (likely (s.successful))
      inverted = inverted ^ other.inverted;
  }

  तार्किक next (hb_codepoint_t *codepoint) ध्रुव
  {
    जइ (likely (!inverted))
      प्रत्यागम s.next (codepoint);

    auto old = *codepoint;
    जइ (unlikely (old + 1 == INVALID))
    {
      *codepoint = INVALID;
      प्रत्यागम मिच्छ;
    }

    auto v = old;
    s.next (&v);
    जइ (old + 1 < v)
    {
      *codepoint = old + 1;
      प्रत्यागम सच्च;
    }

    v = old;
    s.next_range (&old, &v);

    *codepoint = v + 1;
    प्रत्यागम *codepoint != INVALID;
  }
  तार्किक next_bits (hb_codepoint_t *codepoint, uint64_t *bits) ध्रुव
  {
    जइ (likely (!inverted))
      प्रत्यागम s.next_bits (codepoint, bits);

    hb_codepoint_t base;
    जइ (unlikely (*codepoint == INVALID))
      base = 0;
    अण्णहा
    {
      base = *codepoint & ~hb_bit_page_t::ELT_MASK;
      जइ (unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1))
	गच्छ done;
      base += hb_bit_page_t::ELT_BITS;
    }

    जाव (सच्च)
    {
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      uint64_t excluded = 0;
      जइ (!s.next_bits (&cursor, &excluded) || cursor != base)
	excluded = 0;
      *bits = ~excluded;
      तार्किक last = unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1);
      जइ (last)
	*bits &= UINT64_MAX >> 1;
      जइ (*bits)
      {
	*codepoint = base;
	प्रत्यागम सच्च;
      }
      जइ (unlikely (last))
	भङ्ग;
      base += hb_bit_page_t::ELT_BITS;
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    प्रत्यागम मिच्छ;
  }
  तार्किक previous (hb_codepoint_t *codepoint) ध्रुव
  {
    जइ (likely (!inverted))
      प्रत्यागम s.previous (codepoint);

    auto old = *codepoint;
    जइ (unlikely (old - 1 == INVALID))
    {
      *codepoint = INVALID;
      प्रत्यागम मिच्छ;
    }

    auto v = old;
    s.previous (&v);

    जइ (old - 1 > v || v == INVALID)
    {
      *codepoint = old - 1;
      प्रत्यागम सच्च;
    }

    v = old;
    s.previous_range (&v, &old);

    *codepoint = v - 1;
    प्रत्यागम *codepoint != INVALID;
  }
  तार्किक next_range (hb_codepoint_t *first, hb_codepoint_t *last) ध्रुव
  {
    जइ (likely (!inverted))
      प्रत्यागम s.next_range (first, last);

    जइ (!next (last))
    {
      *last = *first = INVALID;
      प्रत्यागम मिच्छ;
    }

    *first = *last;
    s.next (last);
    --*last;
    प्रत्यागम सच्च;
  }
  तार्किक previous_range (hb_codepoint_t *first, hb_codepoint_t *last) ध्रुव
  {
    जइ (likely (!inverted))
      प्रत्यागम s.previous_range (first, last);

    जइ (!previous (first))
    {
      *last = *first = INVALID;
      प्रत्यागम मिच्छ;
    }

    *last = *first;
    s.previous (first);
    ++*first;
    प्रत्यागम सच्च;
  }

  unsigned पूर्णाङ्क next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  unsigned पूर्णाङ्क    size) ध्रुव
  {
    प्रत्यागम inverted ? s.next_many_inverted (codepoint, out, size)
		    : s.next_many (codepoint, out, size);
  }

  स्थिर constexpr hb_codepoint_t INVALID = hb_bit_set_t::INVALID;

  /*
   * Iterator implementation.
   * Mutating the set invalidates its iterators.
   */
  संरचना iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    स्थिर constexpr तार्किक is_sorted_iterator = सच्च;
    स्थिर constexpr तार्किक has_fast_len = सच्च;
    iter_t (ध्रुव hb_bit_set_invertible_t &s_ = Null (hb_bit_set_invertible_t),
    तार्किक init = सच्च) : s (&s_), v (INVALID), remaining (0),
				     have_len (!init), base (INVALID), bits (0)
    {
      जइ (init)
	advance ();
    }

    प्रकारनिर्देश hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ध्रुव { प्रत्यागम v; }
    तार्किक __more__ () ध्रुव { प्रत्यागम v != INVALID; }
    शून्य __next__ ()
    {
      remaining--;
      advance ();
    }
    शून्य __prev__ ()
    {
      जइ (s->previous (&v))
      {
	समकाल ();
	remaining++;
      }
      अण्णहा
      {
	base = INVALID;
	bits = 0;
      }
    }
    unsigned __len__ () ध्रुव
    {
      जइ (unlikely (!have_len))
      {
	remaining += s->get_population ();
	have_len = सच्च;
      }
      प्रत्यागम remaining;
    }
    iter_t end () ध्रुव { प्रत्यागम iter_t (*s, मिच्छ); }
    तार्किक operator != (ध्रुव iter_t& o) ध्रुव
    { प्रत्यागम v != o.v; }

    रक्षित:
    ध्रुव hb_bit_set_invertible_t *s;
    hb_codepoint_t v;
    mutable unsigned remaining;
    mutable तार्किक have_len;
    hb_codepoint_t base;
    uint64_t bits;

    शून्य advance ()
    {
      जइ (likely (bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      अण्णहा जइ (s->next_bits (&base, &bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      अण्णहा
	v = INVALID;
    }

    शून्य समकाल ()
    {
      base = v & ~hb_bit_page_t::ELT_MASK;
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      s->next_bits (&cursor, &bits);
      unsigned पूर्णाङ्क bit = v & (hb_bit_page_t::ELT_BITS - 1);
      bits = bit == hb_bit_page_t::ELT_BITS - 1 ? 0 : bits >> (bit + 1) << (bit + 1);
    }
  };
  iter_t iter () ध्रुव { प्रत्यागम iter_t (*एतत्); }
  operator iter_t () ध्रुव { प्रत्यागम iter (); }
};


#endif /* HB_BIT_SET_INVERTIBLE_HH */

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
  तार्किक inverted = मिच्छा;

  hb_bit_set_invertible_t () = स्वभाव;
  hb_bit_set_invertible_t (ध्रुव hb_bit_set_invertible_t& o) = स्वभाव;
  hb_bit_set_invertible_t (hb_bit_set_invertible_t&& other)  noexcept : hb_bit_set_invertible_t () { hb_swap (*एतत्, other); }
  hb_bit_set_invertible_t& operator= (ध्रुव hb_bit_set_invertible_t& o) = स्वभाव;
  hb_bit_set_invertible_t& operator= (hb_bit_set_invertible_t&& other)  noexcept { hb_swap (*एतत्, other); पटिनिवत्त *एतत्; }
  friend शून्य swap (hb_bit_set_invertible_t &a, hb_bit_set_invertible_t &b) noexcept
  {
    सचे (likely (!a.s.successful || !b.s.successful))
      पटिनिवत्त;
    hb_swap (a.inverted, b.inverted);
    hb_swap (a.s, b.s);
  }

  शून्य init () { s.init (); inverted = मिच्छा; }
  शून्य fini () { s.fini (); }
  शून्य err () { s.err (); }
  तार्किक in_error () ध्रुव { पटिनिवत्त s.in_error (); }
  explicit operator तार्किक () ध्रुव { पटिनिवत्त !is_empty (); }

  शून्य alloc (unsigned sz) { s.alloc (sz); }
  शून्य reset ()
  {
    s.reset ();
    inverted = मिच्छा;
  }
  शून्य clear ()
  {
    s.clear ();
    सचे (likely (s.successful))
      inverted = मिच्छा;
  }
  शून्य invert ()
  {
    सचे (likely (s.successful))
      inverted = !inverted;
  }

  तार्किक is_inverted () ध्रुव
  {
    पटिनिवत्त inverted;
  }

  तार्किक is_empty () ध्रुव
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    पटिनिवत्त v == INVALID;
  }
  uint32_t hash () ध्रुव { पटिनिवत्त s.hash () ^ (uint32_t) inverted; }

  hb_codepoint_t get_min () ध्रुव
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    पटिनिवत्त v;
  }
  hb_codepoint_t get_max () ध्रुव
  {
    hb_codepoint_t v = INVALID;
    previous (&v);
    पटिनिवत्त v;
  }
  unsigned पूर्णाङ्क get_population () ध्रुव
  { पटिनिवत्त inverted ? INVALID - s.get_population () : s.get_population (); }
  तार्किक get_singleton (hb_codepoint_t *codepoint) ध्रुव
  {
    सचे (likely (!inverted))
      पटिनिवत्त s.get_singleton (codepoint);
    सचे (s.get_population () != INVALID - 1)
      पटिनिवत्त मिच्छा;
    *codepoint = get_min ();
    पटिनिवत्त *codepoint != INVALID;
  }


  शून्य add (hb_codepoint_t g) { unlikely (inverted) ? s.del (g) : s.add (g); }
  शून्य add_bits (hb_codepoint_t g, uint64_t bits)
  { unlikely (inverted) ? s.del_bits (g, bits) : s.add_bits (g, bits); }
  तार्किक add_range (hb_codepoint_t a, hb_codepoint_t b)
  { पटिनिवत्त unlikely (inverted) ? ((शून्य) s.del_range (a, b), सच्च) : s.add_range (a, b); }

  template <typename T>
  शून्य add_array (ध्रुव T *array, unsigned पूर्णाङ्क गणनम्, unsigned पूर्णाङ्क stride=परिमाण(T))
  { inverted ? s.del_array (array, गणनम्, stride) : s.add_array (array, गणनम्, stride); }
  template <typename T>
  शून्य add_array (ध्रुव hb_array_t<ध्रुव T>& arr) { add_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  तार्किक add_sorted_array (ध्रुव T *array, unsigned पूर्णाङ्क गणनम्, unsigned पूर्णाङ्क stride=परिमाण(T))
  { पटिनिवत्त inverted ? s.del_sorted_array (array, गणनम्, stride) : s.add_sorted_array (array, गणनम्, stride); }
  template <typename T>
  तार्किक add_sorted_array (ध्रुव hb_sorted_array_t<ध्रुव T>& arr) { पटिनिवत्त add_sorted_array (&arr, arr.len ()); }

  शून्य del (hb_codepoint_t g) { unlikely (inverted) ? s.add (g) : s.del (g); }
  शून्य del_range (hb_codepoint_t a, hb_codepoint_t b)
  { unlikely (inverted) ? (शून्य) s.add_range (a, b) : s.del_range (a, b); }

  तार्किक get (hb_codepoint_t g) ध्रुव { पटिनिवत्त s.get (g) ^ inverted; }
  तार्किक may_have (hb_codepoint_t g) ध्रुव { पटिनिवत्त get (g); }

  /* Has interface. */
  तार्किक operator [] (hb_codepoint_t k) ध्रुव { पटिनिवत्त get (k); }
  तार्किक has (hb_codepoint_t k) ध्रुव { पटिनिवत्त (*एतत्)[k]; }
  /* Predicate. */
  तार्किक operator () (hb_codepoint_t k) ध्रुव { पटिनिवत्त has (k); }

  /* Sink interface. */
  hb_bit_set_invertible_t& operator << (hb_codepoint_t v)
  { add (v); पटिनिवत्त *एतत्; }
  hb_bit_set_invertible_t& operator << (ध्रुव hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); पटिनिवत्त *एतत्; }

  तार्किक may_intersect (ध्रुव hb_bit_set_invertible_t &other) ध्रुव
  { पटिनिवत्त inverted || other.inverted || s.intersects (other.s); }

  तार्किक intersects (ध्रुव hb_bit_set_invertible_t &other) ध्रुव
  {
    सचे (likely (!inverted))
      पटिनिवत्त unlikely (other.inverted) ? !s.is_subset (other.s)
				       : s.intersects (other.s);
    सचे (likely (!other.inverted))
      पटिनिवत्त !other.s.is_subset (s);

    unsigned population = s.get_population ();
    unsigned other_population = other.s.get_population ();
    सचे ((uint64_t) population + other_population < INVALID)
      पटिनिवत्त सच्च;

    पटिनिवत्त population >= other_population
	 ? !hb_all (iter (), other.s)
	 : !hb_all (other.iter (), s);
  }

  तार्किक intersects (hb_codepoint_t first, hb_codepoint_t last) ध्रुव
  {
    hb_codepoint_t c = first - 1;
    पटिनिवत्त next (&c) && c <= last;
  }

  शून्य set (ध्रुव hb_bit_set_invertible_t &other)
  {
    s.set (other.s);
    सचे (likely (s.successful))
      inverted = other.inverted;
  }

  तार्किक is_equal (ध्रुव hb_bit_set_invertible_t &other) ध्रुव
  {
    सचे (likely (inverted == other.inverted))
      पटिनिवत्त s.is_equal (other.s);
    अञ्ञथा
    {
      सचे (get_population () != other.get_population ())
	पटिनिवत्त मिच्छा;

      /* TODO Add iter_ranges() and use here. */
      auto it1 = iter ();
      auto it2 = other.iter ();
      पटिनिवत्त hb_all (+ hb_zip (it1, it2)
		     | hb_map ([](hb_codepoint_pair_t _) { पटिनिवत्त _.first == _.second; }));
    }
  }

  तार्किक is_subset (ध्रुव hb_bit_set_invertible_t &larger_set) ध्रुव
  {
    सचे (unlikely (inverted != larger_set.inverted))
    {
      सचे (inverted)
	पटिनिवत्त hb_all (iter (), larger_set.s);
      अञ्ञथा
        // larger set is inverted so larger_set.s is the set of things that are not present
        // in larger_set, therefore if s has any of those it can't be a subset.
	पटिनिवत्त !s.intersects (larger_set.s);
    }
    अञ्ञथा
      पटिनिवत्त unlikely (inverted) ? larger_set.s.is_subset (s) : s.is_subset (larger_set.s);
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
    सचे (likely (inverted == other.inverted))
    {
      सचे (unlikely (inverted))
	process (hb_bitwise_and, other);
      अञ्ञथा
	process (hb_bitwise_or, other); /* Main branch. */
    }
    अञ्ञथा
    {
      सचे (unlikely (inverted))
	process (hb_bitwise_gt, other);
      अञ्ञथा
	process (hb_bitwise_lt, other);
    }
    सचे (likely (s.successful))
      inverted = inverted || other.inverted;
  }
  शून्य intersect (ध्रुव hb_bit_set_invertible_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  {
    सचे (likely (inverted == other.inverted))
    {
      सचे (unlikely (inverted))
	process (hb_bitwise_or, other, workspace);
      अञ्ञथा
	process (hb_bitwise_and, other, workspace); /* Main branch. */
    }
    अञ्ञथा
    {
      सचे (unlikely (inverted))
	process (hb_bitwise_lt, other, workspace);
      अञ्ञथा
	process (hb_bitwise_gt, other, workspace);
    }
    सचे (likely (s.successful))
      inverted = inverted && other.inverted;
  }
  शून्य subtract (ध्रुव hb_bit_set_invertible_t &other)
  {
    सचे (likely (inverted == other.inverted))
    {
      सचे (unlikely (inverted))
	process (hb_bitwise_lt, other);
      अञ्ञथा
	process (hb_bitwise_gt, other); /* Main branch. */
    }
    अञ्ञथा
    {
      सचे (unlikely (inverted))
	process (hb_bitwise_or, other);
      अञ्ञथा
	process (hb_bitwise_and, other);
    }
    सचे (likely (s.successful))
      inverted = inverted && !other.inverted;
  }
  शून्य symmetric_difference (ध्रुव hb_bit_set_invertible_t &other)
  {
    process (hb_bitwise_xor, other);
    सचे (likely (s.successful))
      inverted = inverted ^ other.inverted;
  }

  तार्किक next (hb_codepoint_t *codepoint) ध्रुव
  {
    सचे (likely (!inverted))
      पटिनिवत्त s.next (codepoint);

    auto old = *codepoint;
    सचे (unlikely (old + 1 == INVALID))
    {
      *codepoint = INVALID;
      पटिनिवत्त मिच्छा;
    }

    auto v = old;
    s.next (&v);
    सचे (old + 1 < v)
    {
      *codepoint = old + 1;
      पटिनिवत्त सच्च;
    }

    v = old;
    s.next_range (&old, &v);

    *codepoint = v + 1;
    पटिनिवत्त *codepoint != INVALID;
  }
  तार्किक next_bits (hb_codepoint_t *codepoint, uint64_t *bits) ध्रुव
  {
    सचे (likely (!inverted))
      पटिनिवत्त s.next_bits (codepoint, bits);

    hb_codepoint_t base;
    सचे (unlikely (*codepoint == INVALID))
      base = 0;
    अञ्ञथा
    {
      base = *codepoint & ~hb_bit_page_t::ELT_MASK;
      सचे (unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1))
	गच्छ done;
      base += hb_bit_page_t::ELT_BITS;
    }

    याव (सच्च)
    {
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      uint64_t excluded = 0;
      सचे (!s.next_bits (&cursor, &excluded) || cursor != base)
	excluded = 0;
      *bits = ~excluded;
      तार्किक last = unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1);
      सचे (last)
	*bits &= UINT64_MAX >> 1;
      सचे (*bits)
      {
	*codepoint = base;
	पटिनिवत्त सच्च;
      }
      सचे (unlikely (last))
	भङ्ग;
      base += hb_bit_page_t::ELT_BITS;
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    पटिनिवत्त मिच्छा;
  }
  तार्किक previous (hb_codepoint_t *codepoint) ध्रुव
  {
    सचे (likely (!inverted))
      पटिनिवत्त s.previous (codepoint);

    auto old = *codepoint;
    सचे (unlikely (old - 1 == INVALID))
    {
      *codepoint = INVALID;
      पटिनिवत्त मिच्छा;
    }

    auto v = old;
    s.previous (&v);

    सचे (old - 1 > v || v == INVALID)
    {
      *codepoint = old - 1;
      पटिनिवत्त सच्च;
    }

    v = old;
    s.previous_range (&v, &old);

    *codepoint = v - 1;
    पटिनिवत्त *codepoint != INVALID;
  }
  तार्किक next_range (hb_codepoint_t *first, hb_codepoint_t *last) ध्रुव
  {
    सचे (likely (!inverted))
      पटिनिवत्त s.next_range (first, last);

    सचे (!next (last))
    {
      *last = *first = INVALID;
      पटिनिवत्त मिच्छा;
    }

    *first = *last;
    s.next (last);
    --*last;
    पटिनिवत्त सच्च;
  }
  तार्किक previous_range (hb_codepoint_t *first, hb_codepoint_t *last) ध्रुव
  {
    सचे (likely (!inverted))
      पटिनिवत्त s.previous_range (first, last);

    सचे (!previous (first))
    {
      *last = *first = INVALID;
      पटिनिवत्त मिच्छा;
    }

    *last = *first;
    s.previous (first);
    ++*first;
    पटिनिवत्त सच्च;
  }

  unsigned पूर्णाङ्क next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  unsigned पूर्णाङ्क    size) ध्रुव
  {
    पटिनिवत्त inverted ? s.next_many_inverted (codepoint, out, size)
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
      सचे (init)
	advance ();
    }

    प्रकारनिर्देश hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ध्रुव { पटिनिवत्त v; }
    तार्किक __more__ () ध्रुव { पटिनिवत्त v != INVALID; }
    शून्य __next__ ()
    {
      remaining--;
      advance ();
    }
    शून्य __prev__ ()
    {
      सचे (s->previous (&v))
      {
	समकाल ();
	remaining++;
      }
      अञ्ञथा
      {
	base = INVALID;
	bits = 0;
      }
    }
    unsigned __len__ () ध्रुव
    {
      सचे (unlikely (!have_len))
      {
	remaining += s->get_population ();
	have_len = सच्च;
      }
      पटिनिवत्त remaining;
    }
    iter_t end () ध्रुव { पटिनिवत्त iter_t (*s, मिच्छा); }
    तार्किक operator != (ध्रुव iter_t& o) ध्रुव
    { पटिनिवत्त v != o.v; }

    रक्षित:
    ध्रुव hb_bit_set_invertible_t *s;
    hb_codepoint_t v;
    mutable unsigned remaining;
    mutable तार्किक have_len;
    hb_codepoint_t base;
    uint64_t bits;

    शून्य advance ()
    {
      सचे (likely (bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      अञ्ञथा सचे (s->next_bits (&base, &bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      अञ्ञथा
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
  iter_t iter () ध्रुव { पटिनिवत्त iter_t (*एतत्); }
  operator iter_t () ध्रुव { पटिनिवत्त iter (); }
};


#endif /* HB_BIT_SET_INVERTIBLE_HH */

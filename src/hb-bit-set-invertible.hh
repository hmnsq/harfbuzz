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
  तार्किक inverted = असत्य;

  hb_bit_set_invertible_t () = स्वभाव;
  hb_bit_set_invertible_t (अचर hb_bit_set_invertible_t& o) = स्वभाव;
  hb_bit_set_invertible_t (hb_bit_set_invertible_t&& other)  noexcept : hb_bit_set_invertible_t () { hb_swap (*एतत्, other); }
  hb_bit_set_invertible_t& चालक= (अचर hb_bit_set_invertible_t& o) = स्वभाव;
  hb_bit_set_invertible_t& चालक= (hb_bit_set_invertible_t&& other)  noexcept { hb_swap (*एतत्, other); फर्काऊ *एतत्; }
  मित्र शून्य swap (hb_bit_set_invertible_t &a, hb_bit_set_invertible_t &b) noexcept
  {
    यदि (likely (!a.s.successful || !b.s.successful))
      फर्काऊ;
    hb_swap (a.inverted, b.inverted);
    hb_swap (a.s, b.s);
  }

  शून्य init () { s.init (); inverted = असत्य; }
  शून्य fini () { s.fini (); }
  शून्य err () { s.err (); }
  तार्किक in_error () अचर { फर्काऊ s.in_error (); }
  explicit चालक तार्किक () अचर { फर्काऊ !is_empty (); }

  शून्य alloc (अचिन्हित sz) { s.alloc (sz); }
  शून्य reset ()
  {
    s.reset ();
    inverted = असत्य;
  }
  शून्य clear ()
  {
    s.clear ();
    यदि (likely (s.successful))
      inverted = असत्य;
  }
  शून्य invert ()
  {
    यदि (likely (s.successful))
      inverted = !inverted;
  }

  तार्किक is_inverted () अचर
  {
    फर्काऊ inverted;
  }

  तार्किक is_empty () अचर
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    फर्काऊ v == INVALID;
  }
  uint32_t hash () अचर { फर्काऊ s.hash () ^ (uint32_t) inverted; }

  hb_codepoint_t get_min () अचर
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    फर्काऊ v;
  }
  hb_codepoint_t get_max () अचर
  {
    hb_codepoint_t v = INVALID;
    previous (&v);
    फर्काऊ v;
  }
  अचिन्हित पूर्णाङ्क get_population () अचर
  { फर्काऊ inverted ? INVALID - s.get_population () : s.get_population (); }
  तार्किक get_singleton (hb_codepoint_t *codepoint) अचर
  {
    यदि (likely (!inverted))
      फर्काऊ s.get_singleton (codepoint);
    यदि (s.get_population () != INVALID - 1)
      फर्काऊ असत्य;
    *codepoint = get_min ();
    फर्काऊ *codepoint != INVALID;
  }


  शून्य add (hb_codepoint_t g) { unlikely (inverted) ? s.del (g) : s.add (g); }
  शून्य add_bits (hb_codepoint_t g, uint64_t bits)
  { unlikely (inverted) ? s.del_bits (g, bits) : s.add_bits (g, bits); }
  तार्किक add_range (hb_codepoint_t a, hb_codepoint_t b)
  { फर्काऊ unlikely (inverted) ? ((शून्य) s.del_range (a, b), सत्य) : s.add_range (a, b); }

  ढाँचा <typename T>
  शून्य add_array (अचर T *array, अचिन्हित पूर्णाङ्क गन्ती, अचिन्हित पूर्णाङ्क stride=परिमाण(T))
  { inverted ? s.del_array (array, गन्ती, stride) : s.add_array (array, गन्ती, stride); }
  ढाँचा <typename T>
  शून्य add_array (अचर hb_array_t<अचर T>& arr) { add_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  ढाँचा <typename T>
  तार्किक add_sorted_array (अचर T *array, अचिन्हित पूर्णाङ्क गन्ती, अचिन्हित पूर्णाङ्क stride=परिमाण(T))
  { फर्काऊ inverted ? s.del_sorted_array (array, गन्ती, stride) : s.add_sorted_array (array, गन्ती, stride); }
  ढाँचा <typename T>
  तार्किक add_sorted_array (अचर hb_sorted_array_t<अचर T>& arr) { फर्काऊ add_sorted_array (&arr, arr.len ()); }

  शून्य del (hb_codepoint_t g) { unlikely (inverted) ? s.add (g) : s.del (g); }
  शून्य del_range (hb_codepoint_t a, hb_codepoint_t b)
  { unlikely (inverted) ? (शून्य) s.add_range (a, b) : s.del_range (a, b); }

  तार्किक get (hb_codepoint_t g) अचर { फर्काऊ s.get (g) ^ inverted; }
  तार्किक may_have (hb_codepoint_t g) अचर { फर्काऊ get (g); }

  /* Has interface. */
  तार्किक चालक [] (hb_codepoint_t k) अचर { फर्काऊ get (k); }
  तार्किक has (hb_codepoint_t k) अचर { फर्काऊ (*एतत्)[k]; }
  /* Predicate. */
  तार्किक चालक () (hb_codepoint_t k) अचर { फर्काऊ has (k); }

  /* Sink interface. */
  hb_bit_set_invertible_t& चालक << (hb_codepoint_t v)
  { add (v); फर्काऊ *एतत्; }
  hb_bit_set_invertible_t& चालक << (अचर hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); फर्काऊ *एतत्; }

  तार्किक may_intersect (अचर hb_bit_set_invertible_t &other) अचर
  { फर्काऊ inverted || other.inverted || s.intersects (other.s); }

  तार्किक intersects (अचर hb_bit_set_invertible_t &other) अचर
  {
    यदि (likely (!inverted))
      फर्काऊ unlikely (other.inverted) ? !s.is_subset (other.s)
				       : s.intersects (other.s);
    यदि (likely (!other.inverted))
      फर्काऊ !other.s.is_subset (s);

    अचिन्हित population = s.get_population ();
    अचिन्हित other_population = other.s.get_population ();
    यदि ((uint64_t) population + other_population < INVALID)
      फर्काऊ सत्य;

    फर्काऊ population >= other_population
	 ? !hb_all (iter (), other.s)
	 : !hb_all (other.iter (), s);
  }

  तार्किक intersects (hb_codepoint_t first, hb_codepoint_t last) अचर
  {
    hb_codepoint_t c = first - 1;
    फर्काऊ next (&c) && c <= last;
  }

  शून्य set (अचर hb_bit_set_invertible_t &other)
  {
    s.set (other.s);
    यदि (likely (s.successful))
      inverted = other.inverted;
  }

  तार्किक is_equal (अचर hb_bit_set_invertible_t &other) अचर
  {
    यदि (likely (inverted == other.inverted))
      फर्काऊ s.is_equal (other.s);
    अन्यथा
    {
      यदि (get_population () != other.get_population ())
	फर्काऊ असत्य;

      /* TODO Add iter_ranges() and use here. */
      स्वतः it1 = iter ();
      स्वतः it2 = other.iter ();
      फर्काऊ hb_all (+ hb_zip (it1, it2)
		     | hb_map ([](hb_codepoint_pair_t _) { फर्काऊ _.first == _.second; }));
    }
  }

  तार्किक is_subset (अचर hb_bit_set_invertible_t &larger_set) अचर
  {
    यदि (unlikely (inverted != larger_set.inverted))
    {
      यदि (inverted)
	फर्काऊ hb_all (iter (), larger_set.s);
      अन्यथा
        // larger set is inverted so larger_set.s is the set of things that are not present
        // in larger_set, therefore if s has any of those it can't be a subset.
	फर्काऊ !s.intersects (larger_set.s);
    }
    अन्यथा
      फर्काऊ unlikely (inverted) ? larger_set.s.is_subset (s) : s.is_subset (larger_set.s);
  }

  रक्षित:
  ढाँचा <typename Op>
  शून्य process (अचर Op& op,
		अचर hb_bit_set_invertible_t &other,
		hb_vector_t<अचिन्हित> *workspace = nullptr)
  { s.process (op, other.s, workspace); }
  सार्वजनिक:
  शून्य union_ (अचर hb_bit_set_invertible_t &other)
  {
    यदि (likely (inverted == other.inverted))
    {
      यदि (unlikely (inverted))
	process (hb_bitwise_and, other);
      अन्यथा
	process (hb_bitwise_or, other); /* Main branch. */
    }
    अन्यथा
    {
      यदि (unlikely (inverted))
	process (hb_bitwise_gt, other);
      अन्यथा
	process (hb_bitwise_lt, other);
    }
    यदि (likely (s.successful))
      inverted = inverted || other.inverted;
  }
  शून्य intersect (अचर hb_bit_set_invertible_t &other,
		  hb_vector_t<अचिन्हित> *workspace = nullptr)
  {
    यदि (likely (inverted == other.inverted))
    {
      यदि (unlikely (inverted))
	process (hb_bitwise_or, other, workspace);
      अन्यथा
	process (hb_bitwise_and, other, workspace); /* Main branch. */
    }
    अन्यथा
    {
      यदि (unlikely (inverted))
	process (hb_bitwise_lt, other, workspace);
      अन्यथा
	process (hb_bitwise_gt, other, workspace);
    }
    यदि (likely (s.successful))
      inverted = inverted && other.inverted;
  }
  शून्य subtract (अचर hb_bit_set_invertible_t &other)
  {
    यदि (likely (inverted == other.inverted))
    {
      यदि (unlikely (inverted))
	process (hb_bitwise_lt, other);
      अन्यथा
	process (hb_bitwise_gt, other); /* Main branch. */
    }
    अन्यथा
    {
      यदि (unlikely (inverted))
	process (hb_bitwise_or, other);
      अन्यथा
	process (hb_bitwise_and, other);
    }
    यदि (likely (s.successful))
      inverted = inverted && !other.inverted;
  }
  शून्य symmetric_difference (अचर hb_bit_set_invertible_t &other)
  {
    process (hb_bitwise_xor, other);
    यदि (likely (s.successful))
      inverted = inverted ^ other.inverted;
  }

  तार्किक next (hb_codepoint_t *codepoint) अचर
  {
    यदि (likely (!inverted))
      फर्काऊ s.next (codepoint);

    स्वतः old = *codepoint;
    यदि (unlikely (old + 1 == INVALID))
    {
      *codepoint = INVALID;
      फर्काऊ असत्य;
    }

    स्वतः v = old;
    s.next (&v);
    यदि (old + 1 < v)
    {
      *codepoint = old + 1;
      फर्काऊ सत्य;
    }

    v = old;
    s.next_range (&old, &v);

    *codepoint = v + 1;
    फर्काऊ *codepoint != INVALID;
  }
  तार्किक next_bits (hb_codepoint_t *codepoint, uint64_t *bits) अचर
  {
    यदि (likely (!inverted))
      फर्काऊ s.next_bits (codepoint, bits);

    hb_codepoint_t base;
    यदि (unlikely (*codepoint == INVALID))
      base = 0;
    अन्यथा
    {
      base = *codepoint & ~hb_bit_page_t::ELT_MASK;
      यदि (unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1))
	गच्छ done;
      base += hb_bit_page_t::ELT_BITS;
    }

    जबसम्म (सत्य)
    {
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      uint64_t excluded = 0;
      यदि (!s.next_bits (&cursor, &excluded) || cursor != base)
	excluded = 0;
      *bits = ~excluded;
      तार्किक last = unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1);
      यदि (last)
	*bits &= UINT64_MAX >> 1;
      यदि (*bits)
      {
	*codepoint = base;
	फर्काऊ सत्य;
      }
      यदि (unlikely (last))
	रोक;
      base += hb_bit_page_t::ELT_BITS;
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    फर्काऊ असत्य;
  }
  तार्किक previous (hb_codepoint_t *codepoint) अचर
  {
    यदि (likely (!inverted))
      फर्काऊ s.previous (codepoint);

    स्वतः old = *codepoint;
    यदि (unlikely (old - 1 == INVALID))
    {
      *codepoint = INVALID;
      फर्काऊ असत्य;
    }

    स्वतः v = old;
    s.previous (&v);

    यदि (old - 1 > v || v == INVALID)
    {
      *codepoint = old - 1;
      फर्काऊ सत्य;
    }

    v = old;
    s.previous_range (&v, &old);

    *codepoint = v - 1;
    फर्काऊ *codepoint != INVALID;
  }
  तार्किक next_range (hb_codepoint_t *first, hb_codepoint_t *last) अचर
  {
    यदि (likely (!inverted))
      फर्काऊ s.next_range (first, last);

    यदि (!next (last))
    {
      *last = *first = INVALID;
      फर्काऊ असत्य;
    }

    *first = *last;
    s.next (last);
    --*last;
    फर्काऊ सत्य;
  }
  तार्किक previous_range (hb_codepoint_t *first, hb_codepoint_t *last) अचर
  {
    यदि (likely (!inverted))
      फर्काऊ s.previous_range (first, last);

    यदि (!previous (first))
    {
      *last = *first = INVALID;
      फर्काऊ असत्य;
    }

    *last = *first;
    s.previous (first);
    ++*first;
    फर्काऊ सत्य;
  }

  अचिन्हित पूर्णाङ्क next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  अचिन्हित पूर्णाङ्क    size) अचर
  {
    फर्काऊ inverted ? s.next_many_inverted (codepoint, out, size)
		    : s.next_many (codepoint, out, size);
  }

  स्थिर constexpr hb_codepoint_t INVALID = hb_bit_set_t::INVALID;

  /*
   * Iterator implementation.
   * Mutating the set invalidates its iterators.
   */
  संरचना iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    स्थिर constexpr तार्किक is_sorted_iterator = सत्य;
    स्थिर constexpr तार्किक has_fast_len = सत्य;
    iter_t (अचर hb_bit_set_invertible_t &s_ = Null (hb_bit_set_invertible_t),
    तार्किक init = सत्य) : s (&s_), v (INVALID), remaining (0),
				     have_len (!init), base (INVALID), bits (0)
    {
      यदि (init)
	advance ();
    }

    प्रकारनिर्देश hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () अचर { फर्काऊ v; }
    तार्किक __more__ () अचर { फर्काऊ v != INVALID; }
    शून्य __next__ ()
    {
      remaining--;
      advance ();
    }
    शून्य __prev__ ()
    {
      यदि (s->previous (&v))
      {
	समकाल ();
	remaining++;
      }
      अन्यथा
      {
	base = INVALID;
	bits = 0;
      }
    }
    अचिन्हित __len__ () अचर
    {
      यदि (unlikely (!have_len))
      {
	remaining += s->get_population ();
	have_len = सत्य;
      }
      फर्काऊ remaining;
    }
    iter_t end () अचर { फर्काऊ iter_t (*s, असत्य); }
    तार्किक चालक != (अचर iter_t& o) अचर
    { फर्काऊ v != o.v; }

    रक्षित:
    अचर hb_bit_set_invertible_t *s;
    hb_codepoint_t v;
    mutable अचिन्हित remaining;
    mutable तार्किक have_len;
    hb_codepoint_t base;
    uint64_t bits;

    शून्य advance ()
    {
      यदि (likely (bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      अन्यथा यदि (s->next_bits (&base, &bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      अन्यथा
	v = INVALID;
    }

    शून्य समकाल ()
    {
      base = v & ~hb_bit_page_t::ELT_MASK;
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      s->next_bits (&cursor, &bits);
      अचिन्हित पूर्णाङ्क bit = v & (hb_bit_page_t::ELT_BITS - 1);
      bits = bit == hb_bit_page_t::ELT_BITS - 1 ? 0 : bits >> (bit + 1) << (bit + 1);
    }
  };
  iter_t iter () अचर { फर्काऊ iter_t (*एतत्); }
  चालक iter_t () अचर { फर्काऊ iter (); }
};


#endif /* HB_BIT_SET_INVERTIBLE_HH */

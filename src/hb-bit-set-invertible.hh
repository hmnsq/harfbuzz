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

  hb_bit_set_invertible_t () = पूर्वनिर्धारित;
  hb_bit_set_invertible_t (स्थिरांक hb_bit_set_invertible_t& o) = पूर्वनिर्धारित;
  hb_bit_set_invertible_t (hb_bit_set_invertible_t&& other)  noexcept : hb_bit_set_invertible_t () { hb_swap (*हे, other); }
  hb_bit_set_invertible_t& चालक= (स्थिरांक hb_bit_set_invertible_t& o) = पूर्वनिर्धारित;
  hb_bit_set_invertible_t& चालक= (hb_bit_set_invertible_t&& other)  noexcept { hb_swap (*हे, other); परत *हे; }
  मित्र शून्य swap (hb_bit_set_invertible_t &a, hb_bit_set_invertible_t &b) noexcept
  {
    जर (likely (!a.s.successful || !b.s.successful))
      परत;
    hb_swap (a.inverted, b.inverted);
    hb_swap (a.s, b.s);
  }

  शून्य init () { s.init (); inverted = असत्य; }
  शून्य fini () { s.fini (); }
  शून्य err () { s.err (); }
  तार्किक in_error () स्थिरांक { परत s.in_error (); }
  explicit चालक तार्किक () स्थिरांक { परत !is_empty (); }

  शून्य alloc (अचिन्हित sz) { s.alloc (sz); }
  शून्य reset ()
  {
    s.reset ();
    inverted = असत्य;
  }
  शून्य clear ()
  {
    s.clear ();
    जर (likely (s.successful))
      inverted = असत्य;
  }
  शून्य invert ()
  {
    जर (likely (s.successful))
      inverted = !inverted;
  }

  तार्किक is_inverted () स्थिरांक
  {
    परत inverted;
  }

  तार्किक is_empty () स्थिरांक
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    परत v == INVALID;
  }
  uint32_t hash () स्थिरांक { परत s.hash () ^ (uint32_t) inverted; }

  hb_codepoint_t get_min () स्थिरांक
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    परत v;
  }
  hb_codepoint_t get_max () स्थिरांक
  {
    hb_codepoint_t v = INVALID;
    previous (&v);
    परत v;
  }
  अचिन्हित पूर्णांक get_population () स्थिरांक
  { परत inverted ? INVALID - s.get_population () : s.get_population (); }
  तार्किक get_singleton (hb_codepoint_t *codepoint) स्थिरांक
  {
    जर (likely (!inverted))
      परत s.get_singleton (codepoint);
    जर (s.get_population () != INVALID - 1)
      परत असत्य;
    *codepoint = get_min ();
    परत *codepoint != INVALID;
  }


  शून्य add (hb_codepoint_t g) { unlikely (inverted) ? s.del (g) : s.add (g); }
  शून्य add_bits (hb_codepoint_t g, uint64_t bits)
  { unlikely (inverted) ? s.del_bits (g, bits) : s.add_bits (g, bits); }
  तार्किक add_range (hb_codepoint_t a, hb_codepoint_t b)
  { परत unlikely (inverted) ? ((शून्य) s.del_range (a, b), सत्य) : s.add_range (a, b); }

  ढाँचा <typename T>
  शून्य add_array (स्थिरांक T *array, अचिन्हित पूर्णांक मोजणी, अचिन्हित पूर्णांक stride=आकार(T))
  { inverted ? s.del_array (array, मोजणी, stride) : s.add_array (array, मोजणी, stride); }
  ढाँचा <typename T>
  शून्य add_array (स्थिरांक hb_array_t<स्थिरांक T>& arr) { add_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  ढाँचा <typename T>
  तार्किक add_sorted_array (स्थिरांक T *array, अचिन्हित पूर्णांक मोजणी, अचिन्हित पूर्णांक stride=आकार(T))
  { परत inverted ? s.del_sorted_array (array, मोजणी, stride) : s.add_sorted_array (array, मोजणी, stride); }
  ढाँचा <typename T>
  तार्किक add_sorted_array (स्थिरांक hb_sorted_array_t<स्थिरांक T>& arr) { परत add_sorted_array (&arr, arr.len ()); }

  शून्य del (hb_codepoint_t g) { unlikely (inverted) ? s.add (g) : s.del (g); }
  शून्य del_range (hb_codepoint_t a, hb_codepoint_t b)
  { unlikely (inverted) ? (शून्य) s.add_range (a, b) : s.del_range (a, b); }

  तार्किक get (hb_codepoint_t g) स्थिरांक { परत s.get (g) ^ inverted; }
  तार्किक may_have (hb_codepoint_t g) स्थिरांक { परत get (g); }

  /* Has interface. */
  तार्किक चालक [] (hb_codepoint_t k) स्थिरांक { परत get (k); }
  तार्किक has (hb_codepoint_t k) स्थिरांक { परत (*हे)[k]; }
  /* Predicate. */
  तार्किक चालक () (hb_codepoint_t k) स्थिरांक { परत has (k); }

  /* Sink interface. */
  hb_bit_set_invertible_t& चालक << (hb_codepoint_t v)
  { add (v); परत *हे; }
  hb_bit_set_invertible_t& चालक << (स्थिरांक hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); परत *हे; }

  तार्किक may_intersect (स्थिरांक hb_bit_set_invertible_t &other) स्थिरांक
  { परत inverted || other.inverted || s.intersects (other.s); }

  तार्किक intersects (स्थिरांक hb_bit_set_invertible_t &other) स्थिरांक
  {
    जर (likely (!inverted))
      परत unlikely (other.inverted) ? !s.is_subset (other.s)
				       : s.intersects (other.s);
    जर (likely (!other.inverted))
      परत !other.s.is_subset (s);

    अचिन्हित population = s.get_population ();
    अचिन्हित other_population = other.s.get_population ();
    जर ((uint64_t) population + other_population < INVALID)
      परत सत्य;

    परत population >= other_population
	 ? !hb_all (iter (), other.s)
	 : !hb_all (other.iter (), s);
  }

  तार्किक intersects (hb_codepoint_t first, hb_codepoint_t last) स्थिरांक
  {
    hb_codepoint_t c = first - 1;
    परत next (&c) && c <= last;
  }

  शून्य set (स्थिरांक hb_bit_set_invertible_t &other)
  {
    s.set (other.s);
    जर (likely (s.successful))
      inverted = other.inverted;
  }

  तार्किक is_equal (स्थिरांक hb_bit_set_invertible_t &other) स्थिरांक
  {
    जर (likely (inverted == other.inverted))
      परत s.is_equal (other.s);
    अन्यथा
    {
      जर (get_population () != other.get_population ())
	परत असत्य;

      /* TODO Add iter_ranges() and use here. */
      स्वतः it1 = iter ();
      स्वतः it2 = other.iter ();
      परत hb_all (+ hb_zip (it1, it2)
		     | hb_map ([](hb_codepoint_pair_t _) { परत _.first == _.second; }));
    }
  }

  तार्किक is_subset (स्थिरांक hb_bit_set_invertible_t &larger_set) स्थिरांक
  {
    जर (unlikely (inverted != larger_set.inverted))
    {
      जर (inverted)
	परत hb_all (iter (), larger_set.s);
      अन्यथा
        // larger set is inverted so larger_set.s is the set of things that are not present
        // in larger_set, therefore if s has any of those it can't be a subset.
	परत !s.intersects (larger_set.s);
    }
    अन्यथा
      परत unlikely (inverted) ? larger_set.s.is_subset (s) : s.is_subset (larger_set.s);
  }

  संरक्षित:
  ढाँचा <typename Op>
  शून्य process (स्थिरांक Op& op,
		स्थिरांक hb_bit_set_invertible_t &other,
		hb_vector_t<अचिन्हित> *workspace = nullptr)
  { s.process (op, other.s, workspace); }
  सार्वजनिक:
  शून्य union_ (स्थिरांक hb_bit_set_invertible_t &other)
  {
    जर (likely (inverted == other.inverted))
    {
      जर (unlikely (inverted))
	process (hb_bitwise_and, other);
      अन्यथा
	process (hb_bitwise_or, other); /* Main branch. */
    }
    अन्यथा
    {
      जर (unlikely (inverted))
	process (hb_bitwise_gt, other);
      अन्यथा
	process (hb_bitwise_lt, other);
    }
    जर (likely (s.successful))
      inverted = inverted || other.inverted;
  }
  शून्य intersect (स्थिरांक hb_bit_set_invertible_t &other,
		  hb_vector_t<अचिन्हित> *workspace = nullptr)
  {
    जर (likely (inverted == other.inverted))
    {
      जर (unlikely (inverted))
	process (hb_bitwise_or, other, workspace);
      अन्यथा
	process (hb_bitwise_and, other, workspace); /* Main branch. */
    }
    अन्यथा
    {
      जर (unlikely (inverted))
	process (hb_bitwise_lt, other, workspace);
      अन्यथा
	process (hb_bitwise_gt, other, workspace);
    }
    जर (likely (s.successful))
      inverted = inverted && other.inverted;
  }
  शून्य subtract (स्थिरांक hb_bit_set_invertible_t &other)
  {
    जर (likely (inverted == other.inverted))
    {
      जर (unlikely (inverted))
	process (hb_bitwise_lt, other);
      अन्यथा
	process (hb_bitwise_gt, other); /* Main branch. */
    }
    अन्यथा
    {
      जर (unlikely (inverted))
	process (hb_bitwise_or, other);
      अन्यथा
	process (hb_bitwise_and, other);
    }
    जर (likely (s.successful))
      inverted = inverted && !other.inverted;
  }
  शून्य symmetric_difference (स्थिरांक hb_bit_set_invertible_t &other)
  {
    process (hb_bitwise_xor, other);
    जर (likely (s.successful))
      inverted = inverted ^ other.inverted;
  }

  तार्किक next (hb_codepoint_t *codepoint) स्थिरांक
  {
    जर (likely (!inverted))
      परत s.next (codepoint);

    स्वतः old = *codepoint;
    जर (unlikely (old + 1 == INVALID))
    {
      *codepoint = INVALID;
      परत असत्य;
    }

    स्वतः v = old;
    s.next (&v);
    जर (old + 1 < v)
    {
      *codepoint = old + 1;
      परत सत्य;
    }

    v = old;
    s.next_range (&old, &v);

    *codepoint = v + 1;
    परत *codepoint != INVALID;
  }
  तार्किक next_bits (hb_codepoint_t *codepoint, uint64_t *bits) स्थिरांक
  {
    जर (likely (!inverted))
      परत s.next_bits (codepoint, bits);

    hb_codepoint_t base;
    जर (unlikely (*codepoint == INVALID))
      base = 0;
    अन्यथा
    {
      base = *codepoint & ~hb_bit_page_t::ELT_MASK;
      जर (unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1))
	जा done;
      base += hb_bit_page_t::ELT_BITS;
    }

    पर्यंत (सत्य)
    {
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      uint64_t excluded = 0;
      जर (!s.next_bits (&cursor, &excluded) || cursor != base)
	excluded = 0;
      *bits = ~excluded;
      तार्किक last = unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1);
      जर (last)
	*bits &= UINT64_MAX >> 1;
      जर (*bits)
      {
	*codepoint = base;
	परत सत्य;
      }
      जर (unlikely (last))
	थांबा;
      base += hb_bit_page_t::ELT_BITS;
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    परत असत्य;
  }
  तार्किक previous (hb_codepoint_t *codepoint) स्थिरांक
  {
    जर (likely (!inverted))
      परत s.previous (codepoint);

    स्वतः old = *codepoint;
    जर (unlikely (old - 1 == INVALID))
    {
      *codepoint = INVALID;
      परत असत्य;
    }

    स्वतः v = old;
    s.previous (&v);

    जर (old - 1 > v || v == INVALID)
    {
      *codepoint = old - 1;
      परत सत्य;
    }

    v = old;
    s.previous_range (&v, &old);

    *codepoint = v - 1;
    परत *codepoint != INVALID;
  }
  तार्किक next_range (hb_codepoint_t *first, hb_codepoint_t *last) स्थिरांक
  {
    जर (likely (!inverted))
      परत s.next_range (first, last);

    जर (!next (last))
    {
      *last = *first = INVALID;
      परत असत्य;
    }

    *first = *last;
    s.next (last);
    --*last;
    परत सत्य;
  }
  तार्किक previous_range (hb_codepoint_t *first, hb_codepoint_t *last) स्थिरांक
  {
    जर (likely (!inverted))
      परत s.previous_range (first, last);

    जर (!previous (first))
    {
      *last = *first = INVALID;
      परत असत्य;
    }

    *last = *first;
    s.previous (first);
    ++*first;
    परत सत्य;
  }

  अचिन्हित पूर्णांक next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  अचिन्हित पूर्णांक    size) स्थिरांक
  {
    परत inverted ? s.next_many_inverted (codepoint, out, size)
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
    iter_t (स्थिरांक hb_bit_set_invertible_t &s_ = Null (hb_bit_set_invertible_t),
    तार्किक init = सत्य) : s (&s_), v (INVALID), remaining (0),
				     have_len (!init), base (INVALID), bits (0)
    {
      जर (init)
	advance ();
    }

    प्रकारव्याख्या hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () स्थिरांक { परत v; }
    तार्किक __more__ () स्थिरांक { परत v != INVALID; }
    शून्य __next__ ()
    {
      remaining--;
      advance ();
    }
    शून्य __prev__ ()
    {
      जर (s->previous (&v))
      {
	समकालिक ();
	remaining++;
      }
      अन्यथा
      {
	base = INVALID;
	bits = 0;
      }
    }
    अचिन्हित __len__ () स्थिरांक
    {
      जर (unlikely (!have_len))
      {
	remaining += s->get_population ();
	have_len = सत्य;
      }
      परत remaining;
    }
    iter_t end () स्थिरांक { परत iter_t (*s, असत्य); }
    तार्किक चालक != (स्थिरांक iter_t& o) स्थिरांक
    { परत v != o.v; }

    संरक्षित:
    स्थिरांक hb_bit_set_invertible_t *s;
    hb_codepoint_t v;
    mutable अचिन्हित remaining;
    mutable तार्किक have_len;
    hb_codepoint_t base;
    uint64_t bits;

    शून्य advance ()
    {
      जर (likely (bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      अन्यथा जर (s->next_bits (&base, &bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      अन्यथा
	v = INVALID;
    }

    शून्य समकालिक ()
    {
      base = v & ~hb_bit_page_t::ELT_MASK;
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      s->next_bits (&cursor, &bits);
      अचिन्हित पूर्णांक bit = v & (hb_bit_page_t::ELT_BITS - 1);
      bits = bit == hb_bit_page_t::ELT_BITS - 1 ? 0 : bits >> (bit + 1) << (bit + 1);
    }
  };
  iter_t iter () स्थिरांक { परत iter_t (*हे); }
  चालक iter_t () स्थिरांक { परत iter (); }
};


#endif /* HB_BIT_SET_INVERTIBLE_HH */

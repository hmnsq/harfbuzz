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

#अगर_अघोषित HB_BIT_SET_INVERTIBLE_HH
#घोषणा HB_BIT_SET_INVERTIBLE_HH

#समावेश "hb.hh"
#समावेश "hb-bit-set.hh"


काष्ठा hb_bit_set_invertible_t
{
  hb_bit_set_t s;
  bool inverted = false;

  hb_bit_set_invertible_t () = शेष;
  hb_bit_set_invertible_t (स्थिर hb_bit_set_invertible_t& o) = शेष;
  hb_bit_set_invertible_t (hb_bit_set_invertible_t&& other)  noexcept : hb_bit_set_invertible_t () { hb_swap (*यह, other); }
  hb_bit_set_invertible_t& चालक= (स्थिर hb_bit_set_invertible_t& o) = शेष;
  hb_bit_set_invertible_t& चालक= (hb_bit_set_invertible_t&& other)  noexcept { hb_swap (*यह, other); वापस *यह; }
  मित्र व्योम swap (hb_bit_set_invertible_t &a, hb_bit_set_invertible_t &b) noexcept
  {
    अगर (likely (!a.s.successful || !b.s.successful))
      वापस;
    hb_swap (a.inverted, b.inverted);
    hb_swap (a.s, b.s);
  }

  व्योम init () { s.init (); inverted = false; }
  व्योम fini () { s.fini (); }
  व्योम err () { s.err (); }
  bool in_error () स्थिर { वापस s.in_error (); }
  explicit चालक bool () स्थिर { वापस !is_empty (); }

  व्योम alloc (अचिन्हित sz) { s.alloc (sz); }
  व्योम reset ()
  {
    s.reset ();
    inverted = false;
  }
  व्योम सफा ()
  {
    s.सफा ();
    अगर (likely (s.successful))
      inverted = false;
  }
  व्योम invert ()
  {
    अगर (likely (s.successful))
      inverted = !inverted;
  }

  bool is_inverted () स्थिर
  {
    वापस inverted;
  }

  bool is_empty () स्थिर
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    वापस v == INVALID;
  }
  uint32_t hash () स्थिर { वापस s.hash () ^ (uint32_t) inverted; }

  hb_codepoint_t get_min () स्थिर
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    वापस v;
  }
  hb_codepoint_t get_max () स्थिर
  {
    hb_codepoint_t v = INVALID;
    previous (&v);
    वापस v;
  }
  अचिन्हित पूर्णांक get_population () स्थिर
  { वापस inverted ? INVALID - s.get_population () : s.get_population (); }
  bool get_singleton (hb_codepoint_t *codepoint) स्थिर
  {
    अगर (likely (!inverted))
      वापस s.get_singleton (codepoint);
    अगर (s.get_population () != INVALID - 1)
      वापस false;
    *codepoint = get_min ();
    वापस *codepoint != INVALID;
  }


  व्योम add (hb_codepoint_t g) { unlikely (inverted) ? s.del (g) : s.add (g); }
  व्योम add_bits (hb_codepoint_t g, uint64_t bits)
  { unlikely (inverted) ? s.del_bits (g, bits) : s.add_bits (g, bits); }
  bool add_range (hb_codepoint_t a, hb_codepoint_t b)
  { वापस unlikely (inverted) ? ((व्योम) s.del_range (a, b), true) : s.add_range (a, b); }

  ढाँचा <typename T>
  व्योम add_array (स्थिर T *array, अचिन्हित पूर्णांक गिनती, अचिन्हित पूर्णांक stride=माप(T))
  { inverted ? s.del_array (array, गिनती, stride) : s.add_array (array, गिनती, stride); }
  ढाँचा <typename T>
  व्योम add_array (स्थिर hb_array_t<स्थिर T>& arr) { add_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  ढाँचा <typename T>
  bool add_sorted_array (स्थिर T *array, अचिन्हित पूर्णांक गिनती, अचिन्हित पूर्णांक stride=माप(T))
  { वापस inverted ? s.del_sorted_array (array, गिनती, stride) : s.add_sorted_array (array, गिनती, stride); }
  ढाँचा <typename T>
  bool add_sorted_array (स्थिर hb_sorted_array_t<स्थिर T>& arr) { वापस add_sorted_array (&arr, arr.len ()); }

  व्योम del (hb_codepoint_t g) { unlikely (inverted) ? s.add (g) : s.del (g); }
  व्योम del_range (hb_codepoint_t a, hb_codepoint_t b)
  { unlikely (inverted) ? (व्योम) s.add_range (a, b) : s.del_range (a, b); }

  bool लो (hb_codepoint_t g) स्थिर { वापस s.लो (g) ^ inverted; }
  bool may_have (hb_codepoint_t g) स्थिर { वापस लो (g); }

  /* Has interface. */
  bool चालक [] (hb_codepoint_t k) स्थिर { वापस लो (k); }
  bool has (hb_codepoint_t k) स्थिर { वापस (*यह)[k]; }
  /* Predicate. */
  bool चालक () (hb_codepoint_t k) स्थिर { वापस has (k); }

  /* Sink interface. */
  hb_bit_set_invertible_t& चालक << (hb_codepoint_t v)
  { add (v); वापस *यह; }
  hb_bit_set_invertible_t& चालक << (स्थिर hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); वापस *यह; }

  bool may_intersect (स्थिर hb_bit_set_invertible_t &other) स्थिर
  { वापस inverted || other.inverted || s.intersects (other.s); }

  bool intersects (स्थिर hb_bit_set_invertible_t &other) स्थिर
  {
    अगर (likely (!inverted))
      वापस unlikely (other.inverted) ? !s.is_subset (other.s)
				       : s.intersects (other.s);
    अगर (likely (!other.inverted))
      वापस !other.s.is_subset (s);

    अचिन्हित population = s.get_population ();
    अचिन्हित other_population = other.s.get_population ();
    अगर ((uint64_t) population + other_population < INVALID)
      वापस true;

    वापस population >= other_population
	 ? !hb_all (iter (), other.s)
	 : !hb_all (other.iter (), s);
  }

  bool intersects (hb_codepoint_t first, hb_codepoint_t last) स्थिर
  {
    hb_codepoint_t c = first - 1;
    वापस next (&c) && c <= last;
  }

  व्योम set (स्थिर hb_bit_set_invertible_t &other)
  {
    s.set (other.s);
    अगर (likely (s.successful))
      inverted = other.inverted;
  }

  bool is_equal (स्थिर hb_bit_set_invertible_t &other) स्थिर
  {
    अगर (likely (inverted == other.inverted))
      वापस s.is_equal (other.s);
    अन्यथा
    {
      अगर (get_population () != other.get_population ())
	वापस false;

      /* TODO Add iter_ranges() and use here. */
      स्वतः it1 = iter ();
      स्वतः it2 = other.iter ();
      वापस hb_all (+ hb_zip (it1, it2)
		     | hb_map ([](hb_codepoint_pair_t _) { वापस _.first == _.second; }));
    }
  }

  bool is_subset (स्थिर hb_bit_set_invertible_t &larger_set) स्थिर
  {
    अगर (unlikely (inverted != larger_set.inverted))
    {
      अगर (inverted)
	वापस hb_all (iter (), larger_set.s);
      अन्यथा
        // larger set is inverted so larger_set.s is the set of things that are not present
        // in larger_set, therefore if s has any of those it can't be a subset.
	वापस !s.intersects (larger_set.s);
    }
    अन्यथा
      वापस unlikely (inverted) ? larger_set.s.is_subset (s) : s.is_subset (larger_set.s);
  }

  रक्षित:
  ढाँचा <typename Op>
  व्योम process (स्थिर Op& op,
		स्थिर hb_bit_set_invertible_t &other,
		hb_vector_t<अचिन्हित> *workspace = nullptr)
  { s.process (op, other.s, workspace); }
  खुला:
  व्योम union_ (स्थिर hb_bit_set_invertible_t &other)
  {
    अगर (likely (inverted == other.inverted))
    {
      अगर (unlikely (inverted))
	process (hb_bitwise_and, other);
      अन्यथा
	process (hb_bitwise_or, other); /* Main branch. */
    }
    अन्यथा
    {
      अगर (unlikely (inverted))
	process (hb_bitwise_gt, other);
      अन्यथा
	process (hb_bitwise_lt, other);
    }
    अगर (likely (s.successful))
      inverted = inverted || other.inverted;
  }
  व्योम intersect (स्थिर hb_bit_set_invertible_t &other,
		  hb_vector_t<अचिन्हित> *workspace = nullptr)
  {
    अगर (likely (inverted == other.inverted))
    {
      अगर (unlikely (inverted))
	process (hb_bitwise_or, other, workspace);
      अन्यथा
	process (hb_bitwise_and, other, workspace); /* Main branch. */
    }
    अन्यथा
    {
      अगर (unlikely (inverted))
	process (hb_bitwise_lt, other, workspace);
      अन्यथा
	process (hb_bitwise_gt, other, workspace);
    }
    अगर (likely (s.successful))
      inverted = inverted && other.inverted;
  }
  व्योम subtract (स्थिर hb_bit_set_invertible_t &other)
  {
    अगर (likely (inverted == other.inverted))
    {
      अगर (unlikely (inverted))
	process (hb_bitwise_lt, other);
      अन्यथा
	process (hb_bitwise_gt, other); /* Main branch. */
    }
    अन्यथा
    {
      अगर (unlikely (inverted))
	process (hb_bitwise_or, other);
      अन्यथा
	process (hb_bitwise_and, other);
    }
    अगर (likely (s.successful))
      inverted = inverted && !other.inverted;
  }
  व्योम symmetric_difference (स्थिर hb_bit_set_invertible_t &other)
  {
    process (hb_bitwise_xor, other);
    अगर (likely (s.successful))
      inverted = inverted ^ other.inverted;
  }

  bool next (hb_codepoint_t *codepoint) स्थिर
  {
    अगर (likely (!inverted))
      वापस s.next (codepoint);

    स्वतः old = *codepoint;
    अगर (unlikely (old + 1 == INVALID))
    {
      *codepoint = INVALID;
      वापस false;
    }

    स्वतः v = old;
    s.next (&v);
    अगर (old + 1 < v)
    {
      *codepoint = old + 1;
      वापस true;
    }

    v = old;
    s.अग्र_range (&old, &v);

    *codepoint = v + 1;
    वापस *codepoint != INVALID;
  }
  bool अग्र_bits (hb_codepoint_t *codepoint, uint64_t *bits) स्थिर
  {
    अगर (likely (!inverted))
      वापस s.अग्र_bits (codepoint, bits);

    hb_codepoint_t base;
    अगर (unlikely (*codepoint == INVALID))
      base = 0;
    अन्यथा
    {
      base = *codepoint & ~hb_bit_page_t::ELT_MASK;
      अगर (unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1))
	जाओ done;
      base += hb_bit_page_t::ELT_BITS;
    }

    जबतक (true)
    {
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      uint64_t excluded = 0;
      अगर (!s.अग्र_bits (&cursor, &excluded) || cursor != base)
	excluded = 0;
      *bits = ~excluded;
      bool last = unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1);
      अगर (last)
	*bits &= UINT64_MAX >> 1;
      अगर (*bits)
      {
	*codepoint = base;
	वापस true;
      }
      अगर (unlikely (last))
	अवरोध;
      base += hb_bit_page_t::ELT_BITS;
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    वापस false;
  }
  bool previous (hb_codepoint_t *codepoint) स्थिर
  {
    अगर (likely (!inverted))
      वापस s.previous (codepoint);

    स्वतः old = *codepoint;
    अगर (unlikely (old - 1 == INVALID))
    {
      *codepoint = INVALID;
      वापस false;
    }

    स्वतः v = old;
    s.previous (&v);

    अगर (old - 1 > v || v == INVALID)
    {
      *codepoint = old - 1;
      वापस true;
    }

    v = old;
    s.previous_range (&v, &old);

    *codepoint = v - 1;
    वापस *codepoint != INVALID;
  }
  bool अग्र_range (hb_codepoint_t *first, hb_codepoint_t *last) स्थिर
  {
    अगर (likely (!inverted))
      वापस s.अग्र_range (first, last);

    अगर (!next (last))
    {
      *last = *first = INVALID;
      वापस false;
    }

    *first = *last;
    s.next (last);
    --*last;
    वापस true;
  }
  bool previous_range (hb_codepoint_t *first, hb_codepoint_t *last) स्थिर
  {
    अगर (likely (!inverted))
      वापस s.previous_range (first, last);

    अगर (!previous (first))
    {
      *last = *first = INVALID;
      वापस false;
    }

    *last = *first;
    s.previous (first);
    ++*first;
    वापस true;
  }

  अचिन्हित पूर्णांक अग्र_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  अचिन्हित पूर्णांक    आकार) स्थिर
  {
    वापस inverted ? s.अग्र_many_inverted (codepoint, out, आकार)
		    : s.अग्र_many (codepoint, out, आकार);
  }

  अटल constexpr hb_codepoint_t INVALID = hb_bit_set_t::INVALID;

  /*
   * Iterator implementation.
   * Mutating the set invalidates its iterators.
   */
  काष्ठा iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    अटल constexpr bool is_sorted_iterator = true;
    अटल constexpr bool has_fast_len = true;
    iter_t (स्थिर hb_bit_set_invertible_t &s_ = Null (hb_bit_set_invertible_t),
    bool init = true) : s (&s_), v (INVALID), remaining (0),
				     have_len (!init), base (INVALID), bits (0)
    {
      अगर (init)
	advance ();
    }

    प्रकार hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () स्थिर { वापस v; }
    bool __more__ () स्थिर { वापस v != INVALID; }
    व्योम __next__ ()
    {
      remaining--;
      advance ();
    }
    व्योम __prev__ ()
    {
      अगर (s->previous (&v))
      {
	sync ();
	remaining++;
      }
      अन्यथा
      {
	base = INVALID;
	bits = 0;
      }
    }
    अचिन्हित __len__ () स्थिर
    {
      अगर (unlikely (!have_len))
      {
	remaining += s->get_population ();
	have_len = true;
      }
      वापस remaining;
    }
    iter_t end () स्थिर { वापस iter_t (*s, false); }
    bool चालक != (स्थिर iter_t& o) स्थिर
    { वापस v != o.v; }

    रक्षित:
    स्थिर hb_bit_set_invertible_t *s;
    hb_codepoint_t v;
    mutable अचिन्हित remaining;
    mutable bool have_len;
    hb_codepoint_t base;
    uint64_t bits;

    व्योम advance ()
    {
      अगर (likely (bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      अन्यथा अगर (s->अग्र_bits (&base, &bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      अन्यथा
	v = INVALID;
    }

    व्योम sync ()
    {
      base = v & ~hb_bit_page_t::ELT_MASK;
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      s->अग्र_bits (&cursor, &bits);
      अचिन्हित पूर्णांक bit = v & (hb_bit_page_t::ELT_BITS - 1);
      bits = bit == hb_bit_page_t::ELT_BITS - 1 ? 0 : bits >> (bit + 1) << (bit + 1);
    }
  };
  iter_t iter () स्थिर { वापस iter_t (*यह); }
  चालक iter_t () स्थिर { वापस iter (); }
};


#पूर्ण_अगर /* HB_BIT_SET_INVERTIBLE_HH */

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

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-bit-set.hh"


ସଂରଚନା hb_bit_set_invertible_t
{
  hb_bit_set_t s;
  ତାର୍କିକ inverted = ମିଥ୍ୟା;

  hb_bit_set_invertible_t () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  hb_bit_set_invertible_t (ଅଚଳ hb_bit_set_invertible_t& o) = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  hb_bit_set_invertible_t (hb_bit_set_invertible_t&& other)  noexcept : hb_bit_set_invertible_t () { hb_swap (*ଏହି, other); }
  hb_bit_set_invertible_t& operator= (ଅଚଳ hb_bit_set_invertible_t& o) = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  hb_bit_set_invertible_t& operator= (hb_bit_set_invertible_t&& other)  noexcept { hb_swap (*ଏହି, other); ଫେରନ୍ତୁ *ଏହି; }
  friend ଶୂନ୍ୟ swap (hb_bit_set_invertible_t &a, hb_bit_set_invertible_t &b) noexcept
  {
    ଯଦି (likely (!a.s.successful || !b.s.successful))
      ଫେରନ୍ତୁ;
    hb_swap (a.inverted, b.inverted);
    hb_swap (a.s, b.s);
  }

  ଶୂନ୍ୟ init () { s.init (); inverted = ମିଥ୍ୟା; }
  ଶୂନ୍ୟ fini () { s.fini (); }
  ଶୂନ୍ୟ err () { s.err (); }
  ତାର୍କିକ in_error () ଅଚଳ { ଫେରନ୍ତୁ s.in_error (); }
  explicit operator ତାର୍କିକ () ଅଚଳ { ଫେରନ୍ତୁ !is_empty (); }

  ଶୂନ୍ୟ alloc (unsigned sz) { s.alloc (sz); }
  ଶୂନ୍ୟ reset ()
  {
    s.reset ();
    inverted = ମିଥ୍ୟା;
  }
  ଶୂନ୍ୟ clear ()
  {
    s.clear ();
    ଯଦି (likely (s.successful))
      inverted = ମିଥ୍ୟା;
  }
  ଶୂନ୍ୟ invert ()
  {
    ଯଦି (likely (s.successful))
      inverted = !inverted;
  }

  ତାର୍କିକ is_inverted () ଅଚଳ
  {
    ଫେରନ୍ତୁ inverted;
  }

  ତାର୍କିକ is_empty () ଅଚଳ
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    ଫେରନ୍ତୁ v == INVALID;
  }
  uint32_t hash () ଅଚଳ { ଫେରନ୍ତୁ s.hash () ^ (uint32_t) inverted; }

  hb_codepoint_t get_min () ଅଚଳ
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    ଫେରନ୍ତୁ v;
  }
  hb_codepoint_t get_max () ଅଚଳ
  {
    hb_codepoint_t v = INVALID;
    previous (&v);
    ଫେରନ୍ତୁ v;
  }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_population () ଅଚଳ
  { ଫେରନ୍ତୁ inverted ? INVALID - s.get_population () : s.get_population (); }
  ତାର୍କିକ get_singleton (hb_codepoint_t *codepoint) ଅଚଳ
  {
    ଯଦି (likely (!inverted))
      ଫେରନ୍ତୁ s.get_singleton (codepoint);
    ଯଦି (s.get_population () != INVALID - 1)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    *codepoint = get_min ();
    ଫେରନ୍ତୁ *codepoint != INVALID;
  }


  ଶୂନ୍ୟ add (hb_codepoint_t g) { unlikely (inverted) ? s.del (g) : s.add (g); }
  ଶୂନ୍ୟ add_bits (hb_codepoint_t g, uint64_t bits)
  { unlikely (inverted) ? s.del_bits (g, bits) : s.add_bits (g, bits); }
  ତାର୍କିକ add_range (hb_codepoint_t a, hb_codepoint_t b)
  { ଫେରନ୍ତୁ unlikely (inverted) ? ((ଶୂନ୍ୟ) s.del_range (a, b), ସତ୍ୟ) : s.add_range (a, b); }

  template <typename T>
  ଶୂନ୍ୟ add_array (ଅଚଳ T *array, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride=ଆକାର(T))
  { inverted ? s.del_array (array, ଗଣନା_2, stride) : s.add_array (array, ଗଣନା_2, stride); }
  template <typename T>
  ଶୂନ୍ୟ add_array (ଅଚଳ hb_array_t<ଅଚଳ T>& arr) { add_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  ତାର୍କିକ add_sorted_array (ଅଚଳ T *array, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride=ଆକାର(T))
  { ଫେରନ୍ତୁ inverted ? s.del_sorted_array (array, ଗଣନା_2, stride) : s.add_sorted_array (array, ଗଣନା_2, stride); }
  template <typename T>
  ତାର୍କିକ add_sorted_array (ଅଚଳ hb_sorted_array_t<ଅଚଳ T>& arr) { ଫେରନ୍ତୁ add_sorted_array (&arr, arr.len ()); }

  ଶୂନ୍ୟ del (hb_codepoint_t g) { unlikely (inverted) ? s.add (g) : s.del (g); }
  ଶୂନ୍ୟ del_range (hb_codepoint_t a, hb_codepoint_t b)
  { unlikely (inverted) ? (ଶୂନ୍ୟ) s.add_range (a, b) : s.del_range (a, b); }

  ତାର୍କିକ get (hb_codepoint_t g) ଅଚଳ { ଫେରନ୍ତୁ s.get (g) ^ inverted; }
  ତାର୍କିକ may_have (hb_codepoint_t g) ଅଚଳ { ଫେରନ୍ତୁ get (g); }

  /* Has interface. */
  ତାର୍କିକ operator [] (hb_codepoint_t k) ଅଚଳ { ଫେରନ୍ତୁ get (k); }
  ତାର୍କିକ has (hb_codepoint_t k) ଅଚଳ { ଫେରନ୍ତୁ (*ଏହି)[k]; }
  /* Predicate. */
  ତାର୍କିକ operator () (hb_codepoint_t k) ଅଚଳ { ଫେରନ୍ତୁ has (k); }

  /* Sink interface. */
  hb_bit_set_invertible_t& operator << (hb_codepoint_t v)
  { add (v); ଫେରନ୍ତୁ *ଏହି; }
  hb_bit_set_invertible_t& operator << (ଅଚଳ hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); ଫେରନ୍ତୁ *ଏହି; }

  ତାର୍କିକ may_intersect (ଅଚଳ hb_bit_set_invertible_t &other) ଅଚଳ
  { ଫେରନ୍ତୁ inverted || other.inverted || s.intersects (other.s); }

  ତାର୍କିକ intersects (ଅଚଳ hb_bit_set_invertible_t &other) ଅଚଳ
  {
    ଯଦି (likely (!inverted))
      ଫେରନ୍ତୁ unlikely (other.inverted) ? !s.is_subset (other.s)
				       : s.intersects (other.s);
    ଯଦି (likely (!other.inverted))
      ଫେରନ୍ତୁ !other.s.is_subset (s);

    unsigned population = s.get_population ();
    unsigned other_population = other.s.get_population ();
    ଯଦି ((uint64_t) population + other_population < INVALID)
      ଫେରନ୍ତୁ ସତ୍ୟ;

    ଫେରନ୍ତୁ population >= other_population
	 ? !hb_all (iter (), other.s)
	 : !hb_all (other.iter (), s);
  }

  ତାର୍କିକ intersects (hb_codepoint_t first, hb_codepoint_t last) ଅଚଳ
  {
    hb_codepoint_t c = first - 1;
    ଫେରନ୍ତୁ next (&c) && c <= last;
  }

  ଶୂନ୍ୟ set (ଅଚଳ hb_bit_set_invertible_t &other)
  {
    s.set (other.s);
    ଯଦି (likely (s.successful))
      inverted = other.inverted;
  }

  ତାର୍କିକ is_equal (ଅଚଳ hb_bit_set_invertible_t &other) ଅଚଳ
  {
    ଯଦି (likely (inverted == other.inverted))
      ଫେରନ୍ତୁ s.is_equal (other.s);
    ଅନ୍ୟଥା
    {
      ଯଦି (get_population () != other.get_population ())
	ଫେରନ୍ତୁ ମିଥ୍ୟା;

      /* TODO Add iter_ranges() and use here. */
      auto it1 = iter ();
      auto it2 = other.iter ();
      ଫେରନ୍ତୁ hb_all (+ hb_zip (it1, it2)
		     | hb_map ([](hb_codepoint_pair_t _) { ଫେରନ୍ତୁ _.first == _.second; }));
    }
  }

  ତାର୍କିକ is_subset (ଅଚଳ hb_bit_set_invertible_t &larger_set) ଅଚଳ
  {
    ଯଦି (unlikely (inverted != larger_set.inverted))
    {
      ଯଦି (inverted)
	ଫେରନ୍ତୁ hb_all (iter (), larger_set.s);
      ଅନ୍ୟଥା
        // larger set is inverted so larger_set.s is the set of things that are not present
        // in larger_set, therefore if s has any of those it can't be a subset.
	ଫେରନ୍ତୁ !s.intersects (larger_set.s);
    }
    ଅନ୍ୟଥା
      ଫେରନ୍ତୁ unlikely (inverted) ? larger_set.s.is_subset (s) : s.is_subset (larger_set.s);
  }

  ସୁରକ୍ଷିତ:
  template <typename Op>
  ଶୂନ୍ୟ process (ଅଚଳ Op& op,
		ଅଚଳ hb_bit_set_invertible_t &other,
		hb_vector_t<unsigned> *workspace = nullptr)
  { s.process (op, other.s, workspace); }
  ସର୍ବସାଧାରଣ:
  ଶୂନ୍ୟ union_ (ଅଚଳ hb_bit_set_invertible_t &other)
  {
    ଯଦି (likely (inverted == other.inverted))
    {
      ଯଦି (unlikely (inverted))
	process (hb_bitwise_and, other);
      ଅନ୍ୟଥା
	process (hb_bitwise_or, other); /* Main branch. */
    }
    ଅନ୍ୟଥା
    {
      ଯଦି (unlikely (inverted))
	process (hb_bitwise_gt, other);
      ଅନ୍ୟଥା
	process (hb_bitwise_lt, other);
    }
    ଯଦି (likely (s.successful))
      inverted = inverted || other.inverted;
  }
  ଶୂନ୍ୟ intersect (ଅଚଳ hb_bit_set_invertible_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  {
    ଯଦି (likely (inverted == other.inverted))
    {
      ଯଦି (unlikely (inverted))
	process (hb_bitwise_or, other, workspace);
      ଅନ୍ୟଥା
	process (hb_bitwise_and, other, workspace); /* Main branch. */
    }
    ଅନ୍ୟଥା
    {
      ଯଦି (unlikely (inverted))
	process (hb_bitwise_lt, other, workspace);
      ଅନ୍ୟଥା
	process (hb_bitwise_gt, other, workspace);
    }
    ଯଦି (likely (s.successful))
      inverted = inverted && other.inverted;
  }
  ଶୂନ୍ୟ subtract (ଅଚଳ hb_bit_set_invertible_t &other)
  {
    ଯଦି (likely (inverted == other.inverted))
    {
      ଯଦି (unlikely (inverted))
	process (hb_bitwise_lt, other);
      ଅନ୍ୟଥା
	process (hb_bitwise_gt, other); /* Main branch. */
    }
    ଅନ୍ୟଥା
    {
      ଯଦି (unlikely (inverted))
	process (hb_bitwise_or, other);
      ଅନ୍ୟଥା
	process (hb_bitwise_and, other);
    }
    ଯଦି (likely (s.successful))
      inverted = inverted && !other.inverted;
  }
  ଶୂନ୍ୟ symmetric_difference (ଅଚଳ hb_bit_set_invertible_t &other)
  {
    process (hb_bitwise_xor, other);
    ଯଦି (likely (s.successful))
      inverted = inverted ^ other.inverted;
  }

  ତାର୍କିକ next (hb_codepoint_t *codepoint) ଅଚଳ
  {
    ଯଦି (likely (!inverted))
      ଫେରନ୍ତୁ s.next (codepoint);

    auto old = *codepoint;
    ଯଦି (unlikely (old + 1 == INVALID))
    {
      *codepoint = INVALID;
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    auto v = old;
    s.next (&v);
    ଯଦି (old + 1 < v)
    {
      *codepoint = old + 1;
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    v = old;
    s.next_range (&old, &v);

    *codepoint = v + 1;
    ଫେରନ୍ତୁ *codepoint != INVALID;
  }
  ତାର୍କିକ next_bits (hb_codepoint_t *codepoint, uint64_t *bits) ଅଚଳ
  {
    ଯଦି (likely (!inverted))
      ଫେରନ୍ତୁ s.next_bits (codepoint, bits);

    hb_codepoint_t base;
    ଯଦି (unlikely (*codepoint == INVALID))
      base = 0;
    ଅନ୍ୟଥା
    {
      base = *codepoint & ~hb_bit_page_t::ELT_MASK;
      ଯଦି (unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1))
	ଯାଅ done;
      base += hb_bit_page_t::ELT_BITS;
    }

    ଯେତେବେଳେ (ସତ୍ୟ)
    {
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      uint64_t excluded = 0;
      ଯଦି (!s.next_bits (&cursor, &excluded) || cursor != base)
	excluded = 0;
      *bits = ~excluded;
      ତାର୍କିକ last = unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1);
      ଯଦି (last)
	*bits &= UINT64_MAX >> 1;
      ଯଦି (*bits)
      {
	*codepoint = base;
	ଫେରନ୍ତୁ ସତ୍ୟ;
      }
      ଯଦି (unlikely (last))
	ଭାଙ୍ଗ;
      base += hb_bit_page_t::ELT_BITS;
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }
  ତାର୍କିକ previous (hb_codepoint_t *codepoint) ଅଚଳ
  {
    ଯଦି (likely (!inverted))
      ଫେରନ୍ତୁ s.previous (codepoint);

    auto old = *codepoint;
    ଯଦି (unlikely (old - 1 == INVALID))
    {
      *codepoint = INVALID;
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    auto v = old;
    s.previous (&v);

    ଯଦି (old - 1 > v || v == INVALID)
    {
      *codepoint = old - 1;
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    v = old;
    s.previous_range (&v, &old);

    *codepoint = v - 1;
    ଫେରନ୍ତୁ *codepoint != INVALID;
  }
  ତାର୍କିକ next_range (hb_codepoint_t *first, hb_codepoint_t *last) ଅଚଳ
  {
    ଯଦି (likely (!inverted))
      ଫେରନ୍ତୁ s.next_range (first, last);

    ଯଦି (!next (last))
    {
      *last = *first = INVALID;
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    *first = *last;
    s.next (last);
    --*last;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }
  ତାର୍କିକ previous_range (hb_codepoint_t *first, hb_codepoint_t *last) ଅଚଳ
  {
    ଯଦି (likely (!inverted))
      ଫେରନ୍ତୁ s.previous_range (first, last);

    ଯଦି (!previous (first))
    {
      *last = *first = INVALID;
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    *last = *first;
    s.previous (first);
    ++*first;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା    size) ଅଚଳ
  {
    ଫେରନ୍ତୁ inverted ? s.next_many_inverted (codepoint, out, size)
		    : s.next_many (codepoint, out, size);
  }

  ସ୍ଥିର constexpr hb_codepoint_t INVALID = hb_bit_set_t::INVALID;

  /*
   * Iterator implementation.
   * Mutating the set invalidates its iterators.
   */
  ସଂରଚନା iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ସ୍ଥିର constexpr ତାର୍କିକ is_sorted_iterator = ସତ୍ୟ;
    ସ୍ଥିର constexpr ତାର୍କିକ has_fast_len = ସତ୍ୟ;
    iter_t (ଅଚଳ hb_bit_set_invertible_t &s_ = Null (hb_bit_set_invertible_t),
    ତାର୍କିକ init = ସତ୍ୟ) : s (&s_), v (INVALID), remaining (0),
				     have_len (!init), base (INVALID), bits (0)
    {
      ଯଦି (init)
	advance ();
    }

    ପ୍ରକାରବ୍ୟାଖ୍ୟା hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ଅଚଳ { ଫେରନ୍ତୁ v; }
    ତାର୍କିକ __more__ () ଅଚଳ { ଫେରନ୍ତୁ v != INVALID; }
    ଶୂନ୍ୟ __next__ ()
    {
      remaining--;
      advance ();
    }
    ଶୂନ୍ୟ __prev__ ()
    {
      ଯଦି (s->previous (&v))
      {
	ସମକାଳ ();
	remaining++;
      }
      ଅନ୍ୟଥା
      {
	base = INVALID;
	bits = 0;
      }
    }
    unsigned __len__ () ଅଚଳ
    {
      ଯଦି (unlikely (!have_len))
      {
	remaining += s->get_population ();
	have_len = ସତ୍ୟ;
      }
      ଫେରନ୍ତୁ remaining;
    }
    iter_t end () ଅଚଳ { ଫେରନ୍ତୁ iter_t (*s, ମିଥ୍ୟା); }
    ତାର୍କିକ operator != (ଅଚଳ iter_t& o) ଅଚଳ
    { ଫେରନ୍ତୁ v != o.v; }

    ସୁରକ୍ଷିତ:
    ଅଚଳ hb_bit_set_invertible_t *s;
    hb_codepoint_t v;
    mutable unsigned remaining;
    mutable ତାର୍କିକ have_len;
    hb_codepoint_t base;
    uint64_t bits;

    ଶୂନ୍ୟ advance ()
    {
      ଯଦି (likely (bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      ଅନ୍ୟଥା ଯଦି (s->next_bits (&base, &bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      ଅନ୍ୟଥା
	v = INVALID;
    }

    ଶୂନ୍ୟ ସମକାଳ ()
    {
      base = v & ~hb_bit_page_t::ELT_MASK;
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      s->next_bits (&cursor, &bits);
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା bit = v & (hb_bit_page_t::ELT_BITS - 1);
      bits = bit == hb_bit_page_t::ELT_BITS - 1 ? 0 : bits >> (bit + 1) << (bit + 1);
    }
  };
  iter_t iter () ଅଚଳ { ଫେରନ୍ତୁ iter_t (*ଏହି); }
  operator iter_t () ଅଚଳ { ଫେରନ୍ତୁ iter (); }
};


#endif /* HB_BIT_SET_INVERTIBLE_HH */

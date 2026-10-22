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

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-bit-set.hh"


সংরচনা hb_bit_set_invertible_t
{
  hb_bit_set_t s;
  তার্কিক inverted = অসত্য;

  hb_bit_set_invertible_t () = স্বভাব;
  hb_bit_set_invertible_t (ধ্রুব hb_bit_set_invertible_t& o) = স্বভাব;
  hb_bit_set_invertible_t (hb_bit_set_invertible_t&& other)  noexcept : hb_bit_set_invertible_t () { hb_swap (*এতত্, other); }
  hb_bit_set_invertible_t& operator= (ধ্রুব hb_bit_set_invertible_t& o) = স্বভাব;
  hb_bit_set_invertible_t& operator= (hb_bit_set_invertible_t&& other)  noexcept { hb_swap (*এতত্, other); প্রত্যাগম *এতত্; }
  friend শূন্য swap (hb_bit_set_invertible_t &a, hb_bit_set_invertible_t &b) noexcept
  {
    যদি (likely (!a.s.successful || !b.s.successful))
      প্রত্যাগম;
    hb_swap (a.inverted, b.inverted);
    hb_swap (a.s, b.s);
  }

  শূন্য init () { s.init (); inverted = অসত্য; }
  শূন্য fini () { s.fini (); }
  শূন্য err () { s.err (); }
  তার্কিক in_error () ধ্রুব { প্রত্যাগম s.in_error (); }
  explicit operator তার্কিক () ধ্রুব { প্রত্যাগম !is_empty (); }

  শূন্য alloc (unsigned sz) { s.alloc (sz); }
  শূন্য reset ()
  {
    s.reset ();
    inverted = অসত্য;
  }
  শূন্য clear ()
  {
    s.clear ();
    যদি (likely (s.successful))
      inverted = অসত্য;
  }
  শূন্য invert ()
  {
    যদি (likely (s.successful))
      inverted = !inverted;
  }

  তার্কিক is_inverted () ধ্রুব
  {
    প্রত্যাগম inverted;
  }

  তার্কিক is_empty () ধ্রুব
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    প্রত্যাগম v == INVALID;
  }
  uint32_t hash () ধ্রুব { প্রত্যাগম s.hash () ^ (uint32_t) inverted; }

  hb_codepoint_t get_min () ধ্রুব
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    প্রত্যাগম v;
  }
  hb_codepoint_t get_max () ধ্রুব
  {
    hb_codepoint_t v = INVALID;
    previous (&v);
    প্রত্যাগম v;
  }
  unsigned পূর্ণাঙ্ক get_population () ধ্রুব
  { প্রত্যাগম inverted ? INVALID - s.get_population () : s.get_population (); }
  তার্কিক get_singleton (hb_codepoint_t *codepoint) ধ্রুব
  {
    যদি (likely (!inverted))
      প্রত্যাগম s.get_singleton (codepoint);
    যদি (s.get_population () != INVALID - 1)
      প্রত্যাগম অসত্য;
    *codepoint = get_min ();
    প্রত্যাগম *codepoint != INVALID;
  }


  শূন্য add (hb_codepoint_t g) { unlikely (inverted) ? s.del (g) : s.add (g); }
  শূন্য add_bits (hb_codepoint_t g, uint64_t bits)
  { unlikely (inverted) ? s.del_bits (g, bits) : s.add_bits (g, bits); }
  তার্কিক add_range (hb_codepoint_t a, hb_codepoint_t b)
  { প্রত্যাগম unlikely (inverted) ? ((শূন্য) s.del_range (a, b), সত্য) : s.add_range (a, b); }

  template <typename T>
  শূন্য add_array (ধ্রুব T *array, unsigned পূর্ণাঙ্ক গুনতি, unsigned পূর্ণাঙ্ক stride=পরিমাণ(T))
  { inverted ? s.del_array (array, গুনতি, stride) : s.add_array (array, গুনতি, stride); }
  template <typename T>
  শূন্য add_array (ধ্রুব hb_array_t<ধ্রুব T>& arr) { add_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  তার্কিক add_sorted_array (ধ্রুব T *array, unsigned পূর্ণাঙ্ক গুনতি, unsigned পূর্ণাঙ্ক stride=পরিমাণ(T))
  { প্রত্যাগম inverted ? s.del_sorted_array (array, গুনতি, stride) : s.add_sorted_array (array, গুনতি, stride); }
  template <typename T>
  তার্কিক add_sorted_array (ধ্রুব hb_sorted_array_t<ধ্রুব T>& arr) { প্রত্যাগম add_sorted_array (&arr, arr.len ()); }

  শূন্য del (hb_codepoint_t g) { unlikely (inverted) ? s.add (g) : s.del (g); }
  শূন্য del_range (hb_codepoint_t a, hb_codepoint_t b)
  { unlikely (inverted) ? (শূন্য) s.add_range (a, b) : s.del_range (a, b); }

  তার্কিক get (hb_codepoint_t g) ধ্রুব { প্রত্যাগম s.get (g) ^ inverted; }
  তার্কিক may_have (hb_codepoint_t g) ধ্রুব { প্রত্যাগম get (g); }

  /* Has interface. */
  তার্কিক operator [] (hb_codepoint_t k) ধ্রুব { প্রত্যাগম get (k); }
  তার্কিক has (hb_codepoint_t k) ধ্রুব { প্রত্যাগম (*এতত্)[k]; }
  /* Predicate. */
  তার্কিক operator () (hb_codepoint_t k) ধ্রুব { প্রত্যাগম has (k); }

  /* Sink interface. */
  hb_bit_set_invertible_t& operator << (hb_codepoint_t v)
  { add (v); প্রত্যাগম *এতত্; }
  hb_bit_set_invertible_t& operator << (ধ্রুব hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); প্রত্যাগম *এতত্; }

  তার্কিক may_intersect (ধ্রুব hb_bit_set_invertible_t &other) ধ্রুব
  { প্রত্যাগম inverted || other.inverted || s.intersects (other.s); }

  তার্কিক intersects (ধ্রুব hb_bit_set_invertible_t &other) ধ্রুব
  {
    যদি (likely (!inverted))
      প্রত্যাগম unlikely (other.inverted) ? !s.is_subset (other.s)
				       : s.intersects (other.s);
    যদি (likely (!other.inverted))
      প্রত্যাগম !other.s.is_subset (s);

    unsigned population = s.get_population ();
    unsigned other_population = other.s.get_population ();
    যদি ((uint64_t) population + other_population < INVALID)
      প্রত্যাগম সত্য;

    প্রত্যাগম population >= other_population
	 ? !hb_all (iter (), other.s)
	 : !hb_all (other.iter (), s);
  }

  তার্কিক intersects (hb_codepoint_t first, hb_codepoint_t last) ধ্রুব
  {
    hb_codepoint_t c = first - 1;
    প্রত্যাগম next (&c) && c <= last;
  }

  শূন্য set (ধ্রুব hb_bit_set_invertible_t &other)
  {
    s.set (other.s);
    যদি (likely (s.successful))
      inverted = other.inverted;
  }

  তার্কিক is_equal (ধ্রুব hb_bit_set_invertible_t &other) ধ্রুব
  {
    যদি (likely (inverted == other.inverted))
      প্রত্যাগম s.is_equal (other.s);
    অন্যথা
    {
      যদি (get_population () != other.get_population ())
	প্রত্যাগম অসত্য;

      /* TODO Add iter_ranges() and use here. */
      auto it1 = iter ();
      auto it2 = other.iter ();
      প্রত্যাগম hb_all (+ hb_zip (it1, it2)
		     | hb_map ([](hb_codepoint_pair_t _) { প্রত্যাগম _.first == _.second; }));
    }
  }

  তার্কিক is_subset (ধ্রুব hb_bit_set_invertible_t &larger_set) ধ্রুব
  {
    যদি (unlikely (inverted != larger_set.inverted))
    {
      যদি (inverted)
	প্রত্যাগম hb_all (iter (), larger_set.s);
      অন্যথা
        // larger set is inverted so larger_set.s is the set of things that are not present
        // in larger_set, therefore if s has any of those it can't be a subset.
	প্রত্যাগম !s.intersects (larger_set.s);
    }
    অন্যথা
      প্রত্যাগম unlikely (inverted) ? larger_set.s.is_subset (s) : s.is_subset (larger_set.s);
  }

  রক্ষিত:
  template <typename Op>
  শূন্য process (ধ্রুব Op& op,
		ধ্রুব hb_bit_set_invertible_t &other,
		hb_vector_t<unsigned> *workspace = nullptr)
  { s.process (op, other.s, workspace); }
  সার্বজনিক:
  শূন্য union_ (ধ্রুব hb_bit_set_invertible_t &other)
  {
    যদি (likely (inverted == other.inverted))
    {
      যদি (unlikely (inverted))
	process (hb_bitwise_and, other);
      অন্যথা
	process (hb_bitwise_or, other); /* Main branch. */
    }
    অন্যথা
    {
      যদি (unlikely (inverted))
	process (hb_bitwise_gt, other);
      অন্যথা
	process (hb_bitwise_lt, other);
    }
    যদি (likely (s.successful))
      inverted = inverted || other.inverted;
  }
  শূন্য intersect (ধ্রুব hb_bit_set_invertible_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  {
    যদি (likely (inverted == other.inverted))
    {
      যদি (unlikely (inverted))
	process (hb_bitwise_or, other, workspace);
      অন্যথা
	process (hb_bitwise_and, other, workspace); /* Main branch. */
    }
    অন্যথা
    {
      যদি (unlikely (inverted))
	process (hb_bitwise_lt, other, workspace);
      অন্যথা
	process (hb_bitwise_gt, other, workspace);
    }
    যদি (likely (s.successful))
      inverted = inverted && other.inverted;
  }
  শূন্য subtract (ধ্রুব hb_bit_set_invertible_t &other)
  {
    যদি (likely (inverted == other.inverted))
    {
      যদি (unlikely (inverted))
	process (hb_bitwise_lt, other);
      অন্যথা
	process (hb_bitwise_gt, other); /* Main branch. */
    }
    অন্যথা
    {
      যদি (unlikely (inverted))
	process (hb_bitwise_or, other);
      অন্যথা
	process (hb_bitwise_and, other);
    }
    যদি (likely (s.successful))
      inverted = inverted && !other.inverted;
  }
  শূন্য symmetric_difference (ধ্রুব hb_bit_set_invertible_t &other)
  {
    process (hb_bitwise_xor, other);
    যদি (likely (s.successful))
      inverted = inverted ^ other.inverted;
  }

  তার্কিক next (hb_codepoint_t *codepoint) ধ্রুব
  {
    যদি (likely (!inverted))
      প্রত্যাগম s.next (codepoint);

    auto old = *codepoint;
    যদি (unlikely (old + 1 == INVALID))
    {
      *codepoint = INVALID;
      প্রত্যাগম অসত্য;
    }

    auto v = old;
    s.next (&v);
    যদি (old + 1 < v)
    {
      *codepoint = old + 1;
      প্রত্যাগম সত্য;
    }

    v = old;
    s.next_range (&old, &v);

    *codepoint = v + 1;
    প্রত্যাগম *codepoint != INVALID;
  }
  তার্কিক next_bits (hb_codepoint_t *codepoint, uint64_t *bits) ধ্রুব
  {
    যদি (likely (!inverted))
      প্রত্যাগম s.next_bits (codepoint, bits);

    hb_codepoint_t base;
    যদি (unlikely (*codepoint == INVALID))
      base = 0;
    অন্যথা
    {
      base = *codepoint & ~hb_bit_page_t::ELT_MASK;
      যদি (unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1))
	গচ্ছ done;
      base += hb_bit_page_t::ELT_BITS;
    }

    যাবত্ (সত্য)
    {
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      uint64_t excluded = 0;
      যদি (!s.next_bits (&cursor, &excluded) || cursor != base)
	excluded = 0;
      *bits = ~excluded;
      তার্কিক last = unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1);
      যদি (last)
	*bits &= UINT64_MAX >> 1;
      যদি (*bits)
      {
	*codepoint = base;
	প্রত্যাগম সত্য;
      }
      যদি (unlikely (last))
	ভঙ্গ;
      base += hb_bit_page_t::ELT_BITS;
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    প্রত্যাগম অসত্য;
  }
  তার্কিক previous (hb_codepoint_t *codepoint) ধ্রুব
  {
    যদি (likely (!inverted))
      প্রত্যাগম s.previous (codepoint);

    auto old = *codepoint;
    যদি (unlikely (old - 1 == INVALID))
    {
      *codepoint = INVALID;
      প্রত্যাগম অসত্য;
    }

    auto v = old;
    s.previous (&v);

    যদি (old - 1 > v || v == INVALID)
    {
      *codepoint = old - 1;
      প্রত্যাগম সত্য;
    }

    v = old;
    s.previous_range (&v, &old);

    *codepoint = v - 1;
    প্রত্যাগম *codepoint != INVALID;
  }
  তার্কিক next_range (hb_codepoint_t *first, hb_codepoint_t *last) ধ্রুব
  {
    যদি (likely (!inverted))
      প্রত্যাগম s.next_range (first, last);

    যদি (!next (last))
    {
      *last = *first = INVALID;
      প্রত্যাগম অসত্য;
    }

    *first = *last;
    s.next (last);
    --*last;
    প্রত্যাগম সত্য;
  }
  তার্কিক previous_range (hb_codepoint_t *first, hb_codepoint_t *last) ধ্রুব
  {
    যদি (likely (!inverted))
      প্রত্যাগম s.previous_range (first, last);

    যদি (!previous (first))
    {
      *last = *first = INVALID;
      প্রত্যাগম অসত্য;
    }

    *last = *first;
    s.previous (first);
    ++*first;
    প্রত্যাগম সত্য;
  }

  unsigned পূর্ণাঙ্ক next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  unsigned পূর্ণাঙ্ক    size) ধ্রুব
  {
    প্রত্যাগম inverted ? s.next_many_inverted (codepoint, out, size)
		    : s.next_many (codepoint, out, size);
  }

  স্থির constexpr hb_codepoint_t INVALID = hb_bit_set_t::INVALID;

  /*
   * Iterator implementation.
   * Mutating the set invalidates its iterators.
   */
  সংরচনা iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    স্থির constexpr তার্কিক is_sorted_iterator = সত্য;
    স্থির constexpr তার্কিক has_fast_len = সত্য;
    iter_t (ধ্রুব hb_bit_set_invertible_t &s_ = Null (hb_bit_set_invertible_t),
    তার্কিক init = সত্য) : s (&s_), v (INVALID), remaining (0),
				     have_len (!init), base (INVALID), bits (0)
    {
      যদি (init)
	advance ();
    }

    প্রকারনির্দেশ hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ধ্রুব { প্রত্যাগম v; }
    তার্কিক __more__ () ধ্রুব { প্রত্যাগম v != INVALID; }
    শূন্য __next__ ()
    {
      remaining--;
      advance ();
    }
    শূন্য __prev__ ()
    {
      যদি (s->previous (&v))
      {
	সমকাল ();
	remaining++;
      }
      অন্যথা
      {
	base = INVALID;
	bits = 0;
      }
    }
    unsigned __len__ () ধ্রুব
    {
      যদি (unlikely (!have_len))
      {
	remaining += s->get_population ();
	have_len = সত্য;
      }
      প্রত্যাগম remaining;
    }
    iter_t end () ধ্রুব { প্রত্যাগম iter_t (*s, অসত্য); }
    তার্কিক operator != (ধ্রুব iter_t& o) ধ্রুব
    { প্রত্যাগম v != o.v; }

    রক্ষিত:
    ধ্রুব hb_bit_set_invertible_t *s;
    hb_codepoint_t v;
    mutable unsigned remaining;
    mutable তার্কিক have_len;
    hb_codepoint_t base;
    uint64_t bits;

    শূন্য advance ()
    {
      যদি (likely (bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      অন্যথা যদি (s->next_bits (&base, &bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      অন্যথা
	v = INVALID;
    }

    শূন্য সমকাল ()
    {
      base = v & ~hb_bit_page_t::ELT_MASK;
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      s->next_bits (&cursor, &bits);
      unsigned পূর্ণাঙ্ক bit = v & (hb_bit_page_t::ELT_BITS - 1);
      bits = bit == hb_bit_page_t::ELT_BITS - 1 ? 0 : bits >> (bit + 1) << (bit + 1);
    }
  };
  iter_t iter () ধ্রুব { প্রত্যাগম iter_t (*এতত্); }
  operator iter_t () ধ্রুব { প্রত্যাগম iter (); }
};


#endif /* HB_BIT_SET_INVERTIBLE_HH */

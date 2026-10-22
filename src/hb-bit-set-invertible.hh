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


কাঠামো hb_bit_set_invertible_t
{
  hb_bit_set_t s;
  যৌক্তিক inverted = মিথ্যা;

  hb_bit_set_invertible_t () = পূর্বনির্ধারিত;
  hb_bit_set_invertible_t (ধ্রুবক hb_bit_set_invertible_t& o) = পূর্বনির্ধারিত;
  hb_bit_set_invertible_t (hb_bit_set_invertible_t&& other)  noexcept : hb_bit_set_invertible_t () { hb_swap (*এই, other); }
  hb_bit_set_invertible_t& operator= (ধ্রুবক hb_bit_set_invertible_t& o) = পূর্বনির্ধারিত;
  hb_bit_set_invertible_t& operator= (hb_bit_set_invertible_t&& other)  noexcept { hb_swap (*এই, other); ফেরত *এই; }
  friend শূন্য swap (hb_bit_set_invertible_t &a, hb_bit_set_invertible_t &b) noexcept
  {
    যদি (likely (!a.s.successful || !b.s.successful))
      ফেরত;
    hb_swap (a.inverted, b.inverted);
    hb_swap (a.s, b.s);
  }

  শূন্য init () { s.init (); inverted = মিথ্যা; }
  শূন্য fini () { s.fini (); }
  শূন্য err () { s.err (); }
  যৌক্তিক in_error () ধ্রুবক { ফেরত s.in_error (); }
  explicit operator যৌক্তিক () ধ্রুবক { ফেরত !is_empty (); }

  শূন্য alloc (unsigned sz) { s.alloc (sz); }
  শূন্য reset ()
  {
    s.reset ();
    inverted = মিথ্যা;
  }
  শূন্য clear ()
  {
    s.clear ();
    যদি (likely (s.successful))
      inverted = মিথ্যা;
  }
  শূন্য invert ()
  {
    যদি (likely (s.successful))
      inverted = !inverted;
  }

  যৌক্তিক is_inverted () ধ্রুবক
  {
    ফেরত inverted;
  }

  যৌক্তিক is_empty () ধ্রুবক
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    ফেরত v == INVALID;
  }
  uint32_t hash () ধ্রুবক { ফেরত s.hash () ^ (uint32_t) inverted; }

  hb_codepoint_t get_min () ধ্রুবক
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    ফেরত v;
  }
  hb_codepoint_t get_max () ধ্রুবক
  {
    hb_codepoint_t v = INVALID;
    previous (&v);
    ফেরত v;
  }
  unsigned পূর্ণসংখ্যা get_population () ধ্রুবক
  { ফেরত inverted ? INVALID - s.get_population () : s.get_population (); }
  যৌক্তিক get_singleton (hb_codepoint_t *codepoint) ধ্রুবক
  {
    যদি (likely (!inverted))
      ফেরত s.get_singleton (codepoint);
    যদি (s.get_population () != INVALID - 1)
      ফেরত মিথ্যা;
    *codepoint = get_min ();
    ফেরত *codepoint != INVALID;
  }


  শূন্য add (hb_codepoint_t g) { unlikely (inverted) ? s.del (g) : s.add (g); }
  শূন্য add_bits (hb_codepoint_t g, uint64_t bits)
  { unlikely (inverted) ? s.del_bits (g, bits) : s.add_bits (g, bits); }
  যৌক্তিক add_range (hb_codepoint_t a, hb_codepoint_t b)
  { ফেরত unlikely (inverted) ? ((শূন্য) s.del_range (a, b), সত্য) : s.add_range (a, b); }

  template <typename T>
  শূন্য add_array (ধ্রুবক T *array, unsigned পূর্ণসংখ্যা গুনতি, unsigned পূর্ণসংখ্যা stride=আকার(T))
  { inverted ? s.del_array (array, গুনতি, stride) : s.add_array (array, গুনতি, stride); }
  template <typename T>
  শূন্য add_array (ধ্রুবক hb_array_t<ধ্রুবক T>& arr) { add_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  যৌক্তিক add_sorted_array (ধ্রুবক T *array, unsigned পূর্ণসংখ্যা গুনতি, unsigned পূর্ণসংখ্যা stride=আকার(T))
  { ফেরত inverted ? s.del_sorted_array (array, গুনতি, stride) : s.add_sorted_array (array, গুনতি, stride); }
  template <typename T>
  যৌক্তিক add_sorted_array (ধ্রুবক hb_sorted_array_t<ধ্রুবক T>& arr) { ফেরত add_sorted_array (&arr, arr.len ()); }

  শূন্য del (hb_codepoint_t g) { unlikely (inverted) ? s.add (g) : s.del (g); }
  শূন্য del_range (hb_codepoint_t a, hb_codepoint_t b)
  { unlikely (inverted) ? (শূন্য) s.add_range (a, b) : s.del_range (a, b); }

  যৌক্তিক get (hb_codepoint_t g) ধ্রুবক { ফেরত s.get (g) ^ inverted; }
  যৌক্তিক may_have (hb_codepoint_t g) ধ্রুবক { ফেরত get (g); }

  /* Has interface. */
  যৌক্তিক operator [] (hb_codepoint_t k) ধ্রুবক { ফেরত get (k); }
  যৌক্তিক has (hb_codepoint_t k) ধ্রুবক { ফেরত (*এই)[k]; }
  /* Predicate. */
  যৌক্তিক operator () (hb_codepoint_t k) ধ্রুবক { ফেরত has (k); }

  /* Sink interface. */
  hb_bit_set_invertible_t& operator << (hb_codepoint_t v)
  { add (v); ফেরত *এই; }
  hb_bit_set_invertible_t& operator << (ধ্রুবক hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); ফেরত *এই; }

  যৌক্তিক may_intersect (ধ্রুবক hb_bit_set_invertible_t &other) ধ্রুবক
  { ফেরত inverted || other.inverted || s.intersects (other.s); }

  যৌক্তিক intersects (ধ্রুবক hb_bit_set_invertible_t &other) ধ্রুবক
  {
    যদি (likely (!inverted))
      ফেরত unlikely (other.inverted) ? !s.is_subset (other.s)
				       : s.intersects (other.s);
    যদি (likely (!other.inverted))
      ফেরত !other.s.is_subset (s);

    unsigned population = s.get_population ();
    unsigned other_population = other.s.get_population ();
    যদি ((uint64_t) population + other_population < INVALID)
      ফেরত সত্য;

    ফেরত population >= other_population
	 ? !hb_all (iter (), other.s)
	 : !hb_all (other.iter (), s);
  }

  যৌক্তিক intersects (hb_codepoint_t first, hb_codepoint_t last) ধ্রুবক
  {
    hb_codepoint_t c = first - 1;
    ফেরত next (&c) && c <= last;
  }

  শূন্য set (ধ্রুবক hb_bit_set_invertible_t &other)
  {
    s.set (other.s);
    যদি (likely (s.successful))
      inverted = other.inverted;
  }

  যৌক্তিক is_equal (ধ্রুবক hb_bit_set_invertible_t &other) ধ্রুবক
  {
    যদি (likely (inverted == other.inverted))
      ফেরত s.is_equal (other.s);
    অন্যথা
    {
      যদি (get_population () != other.get_population ())
	ফেরত মিথ্যা;

      /* TODO Add iter_ranges() and use here. */
      auto it1 = iter ();
      auto it2 = other.iter ();
      ফেরত hb_all (+ hb_zip (it1, it2)
		     | hb_map ([](hb_codepoint_pair_t _) { ফেরত _.first == _.second; }));
    }
  }

  যৌক্তিক is_subset (ধ্রুবক hb_bit_set_invertible_t &larger_set) ধ্রুবক
  {
    যদি (unlikely (inverted != larger_set.inverted))
    {
      যদি (inverted)
	ফেরত hb_all (iter (), larger_set.s);
      অন্যথা
        // larger set is inverted so larger_set.s is the set of things that are not present
        // in larger_set, therefore if s has any of those it can't be a subset.
	ফেরত !s.intersects (larger_set.s);
    }
    অন্যথা
      ফেরত unlikely (inverted) ? larger_set.s.is_subset (s) : s.is_subset (larger_set.s);
  }

  সুরক্ষিত:
  template <typename Op>
  শূন্য process (ধ্রুবক Op& op,
		ধ্রুবক hb_bit_set_invertible_t &other,
		hb_vector_t<unsigned> *workspace = nullptr)
  { s.process (op, other.s, workspace); }
  সর্বজনীন:
  শূন্য union_ (ধ্রুবক hb_bit_set_invertible_t &other)
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
  শূন্য intersect (ধ্রুবক hb_bit_set_invertible_t &other,
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
  শূন্য subtract (ধ্রুবক hb_bit_set_invertible_t &other)
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
  শূন্য symmetric_difference (ধ্রুবক hb_bit_set_invertible_t &other)
  {
    process (hb_bitwise_xor, other);
    যদি (likely (s.successful))
      inverted = inverted ^ other.inverted;
  }

  যৌক্তিক next (hb_codepoint_t *codepoint) ধ্রুবক
  {
    যদি (likely (!inverted))
      ফেরত s.next (codepoint);

    auto old = *codepoint;
    যদি (unlikely (old + 1 == INVALID))
    {
      *codepoint = INVALID;
      ফেরত মিথ্যা;
    }

    auto v = old;
    s.next (&v);
    যদি (old + 1 < v)
    {
      *codepoint = old + 1;
      ফেরত সত্য;
    }

    v = old;
    s.পরবর্তী_range (&old, &v);

    *codepoint = v + 1;
    ফেরত *codepoint != INVALID;
  }
  যৌক্তিক পরবর্তী_bits (hb_codepoint_t *codepoint, uint64_t *bits) ধ্রুবক
  {
    যদি (likely (!inverted))
      ফেরত s.পরবর্তী_bits (codepoint, bits);

    hb_codepoint_t base;
    যদি (unlikely (*codepoint == INVALID))
      base = 0;
    অন্যথা
    {
      base = *codepoint & ~hb_bit_page_t::ELT_MASK;
      যদি (unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1))
	যাও done;
      base += hb_bit_page_t::ELT_BITS;
    }

    যখন (সত্য)
    {
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      uint64_t excluded = 0;
      যদি (!s.পরবর্তী_bits (&cursor, &excluded) || cursor != base)
	excluded = 0;
      *bits = ~excluded;
      যৌক্তিক last = unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1);
      যদি (last)
	*bits &= UINT64_MAX >> 1;
      যদি (*bits)
      {
	*codepoint = base;
	ফেরত সত্য;
      }
      যদি (unlikely (last))
	বিরতি;
      base += hb_bit_page_t::ELT_BITS;
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    ফেরত মিথ্যা;
  }
  যৌক্তিক previous (hb_codepoint_t *codepoint) ধ্রুবক
  {
    যদি (likely (!inverted))
      ফেরত s.previous (codepoint);

    auto old = *codepoint;
    যদি (unlikely (old - 1 == INVALID))
    {
      *codepoint = INVALID;
      ফেরত মিথ্যা;
    }

    auto v = old;
    s.previous (&v);

    যদি (old - 1 > v || v == INVALID)
    {
      *codepoint = old - 1;
      ফেরত সত্য;
    }

    v = old;
    s.previous_range (&v, &old);

    *codepoint = v - 1;
    ফেরত *codepoint != INVALID;
  }
  যৌক্তিক পরবর্তী_range (hb_codepoint_t *first, hb_codepoint_t *last) ধ্রুবক
  {
    যদি (likely (!inverted))
      ফেরত s.পরবর্তী_range (first, last);

    যদি (!next (last))
    {
      *last = *first = INVALID;
      ফেরত মিথ্যা;
    }

    *first = *last;
    s.next (last);
    --*last;
    ফেরত সত্য;
  }
  যৌক্তিক previous_range (hb_codepoint_t *first, hb_codepoint_t *last) ধ্রুবক
  {
    যদি (likely (!inverted))
      ফেরত s.previous_range (first, last);

    যদি (!previous (first))
    {
      *last = *first = INVALID;
      ফেরত মিথ্যা;
    }

    *last = *first;
    s.previous (first);
    ++*first;
    ফেরত সত্য;
  }

  unsigned পূর্ণসংখ্যা পরবর্তী_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  unsigned পূর্ণসংখ্যা    আকার_2) ধ্রুবক
  {
    ফেরত inverted ? s.পরবর্তী_many_inverted (codepoint, out, আকার_2)
		    : s.পরবর্তী_many (codepoint, out, আকার_2);
  }

  স্থির constexpr hb_codepoint_t INVALID = hb_bit_set_t::INVALID;

  /*
   * Iterator implementation.
   * Mutating the set invalidates its iterators.
   */
  কাঠামো iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    স্থির constexpr যৌক্তিক is_sorted_iterator = সত্য;
    স্থির constexpr যৌক্তিক has_fast_len = সত্য;
    iter_t (ধ্রুবক hb_bit_set_invertible_t &s_ = Null (hb_bit_set_invertible_t),
    যৌক্তিক init = সত্য) : s (&s_), v (INVALID), remaining (0),
				     have_len (!init), base (INVALID), bits (0)
    {
      যদি (init)
	advance ();
    }

    প্রকারলেখ hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ধ্রুবক { ফেরত v; }
    যৌক্তিক __more__ () ধ্রুবক { ফেরত v != INVALID; }
    শূন্য __next__ ()
    {
      remaining--;
      advance ();
    }
    শূন্য __prev__ ()
    {
      যদি (s->previous (&v))
      {
	সমলয় ();
	remaining++;
      }
      অন্যথা
      {
	base = INVALID;
	bits = 0;
      }
    }
    unsigned __len__ () ধ্রুবক
    {
      যদি (unlikely (!have_len))
      {
	remaining += s->get_population ();
	have_len = সত্য;
      }
      ফেরত remaining;
    }
    iter_t end () ধ্রুবক { ফেরত iter_t (*s, মিথ্যা); }
    যৌক্তিক operator != (ধ্রুবক iter_t& o) ধ্রুবক
    { ফেরত v != o.v; }

    সুরক্ষিত:
    ধ্রুবক hb_bit_set_invertible_t *s;
    hb_codepoint_t v;
    mutable unsigned remaining;
    mutable যৌক্তিক have_len;
    hb_codepoint_t base;
    uint64_t bits;

    শূন্য advance ()
    {
      যদি (likely (bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      অন্যথা যদি (s->পরবর্তী_bits (&base, &bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      অন্যথা
	v = INVALID;
    }

    শূন্য সমলয় ()
    {
      base = v & ~hb_bit_page_t::ELT_MASK;
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      s->পরবর্তী_bits (&cursor, &bits);
      unsigned পূর্ণসংখ্যা bit = v & (hb_bit_page_t::ELT_BITS - 1);
      bits = bit == hb_bit_page_t::ELT_BITS - 1 ? 0 : bits >> (bit + 1) << (bit + 1);
    }
  };
  iter_t iter () ধ্রুবক { ফেরত iter_t (*এই); }
  operator iter_t () ধ্রুবক { ফেরত iter (); }
};


#endif /* HB_BIT_SET_INVERTIBLE_HH */

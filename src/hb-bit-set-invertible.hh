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

#அந்தர்பாவ "hb.hh"
#அந்தர்பாவ "hb-bit-set.hh"


கட்டமைப்பு hb_bit_set_invertible_t
{
  hb_bit_set_t s;
  தருக்க inverted = பொய்;

  hb_bit_set_invertible_t () = இயல்பு;
  hb_bit_set_invertible_t (மாறிலி hb_bit_set_invertible_t& o) = இயல்பு;
  hb_bit_set_invertible_t (hb_bit_set_invertible_t&& other)  noexcept : hb_bit_set_invertible_t () { hb_swap (*இது, other); }
  hb_bit_set_invertible_t& operator= (மாறிலி hb_bit_set_invertible_t& o) = இயல்பு;
  hb_bit_set_invertible_t& operator= (hb_bit_set_invertible_t&& other)  noexcept { hb_swap (*இது, other); திரும்பு *இது; }
  friend வெறுமை swap (hb_bit_set_invertible_t &a, hb_bit_set_invertible_t &b) noexcept
  {
    எனில் (likely (!a.s.successful || !b.s.successful))
      திரும்பு;
    hb_swap (a.inverted, b.inverted);
    hb_swap (a.s, b.s);
  }

  வெறுமை init () { s.init (); inverted = பொய்; }
  வெறுமை fini () { s.fini (); }
  வெறுமை err () { s.err (); }
  தருக்க in_error () மாறிலி { திரும்பு s.in_error (); }
  explicit operator தருக்க () மாறிலி { திரும்பு !is_empty (); }

  வெறுமை alloc (unsigned sz) { s.alloc (sz); }
  வெறுமை reset ()
  {
    s.reset ();
    inverted = பொய்;
  }
  வெறுமை clear ()
  {
    s.clear ();
    எனில் (likely (s.successful))
      inverted = பொய்;
  }
  வெறுமை invert ()
  {
    எனில் (likely (s.successful))
      inverted = !inverted;
  }

  தருக்க is_inverted () மாறிலி
  {
    திரும்பு inverted;
  }

  தருக்க is_empty () மாறிலி
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    திரும்பு v == INVALID;
  }
  uint32_t hash () மாறிலி { திரும்பு s.hash () ^ (uint32_t) inverted; }

  hb_codepoint_t get_min () மாறிலி
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    திரும்பு v;
  }
  hb_codepoint_t get_max () மாறிலி
  {
    hb_codepoint_t v = INVALID;
    previous (&v);
    திரும்பு v;
  }
  unsigned முழு get_population () மாறிலி
  { திரும்பு inverted ? INVALID - s.get_population () : s.get_population (); }
  தருக்க get_singleton (hb_codepoint_t *codepoint) மாறிலி
  {
    எனில் (likely (!inverted))
      திரும்பு s.get_singleton (codepoint);
    எனில் (s.get_population () != INVALID - 1)
      திரும்பு பொய்;
    *codepoint = get_min ();
    திரும்பு *codepoint != INVALID;
  }


  வெறுமை add (hb_codepoint_t g) { unlikely (inverted) ? s.del (g) : s.add (g); }
  வெறுமை add_bits (hb_codepoint_t g, uint64_t bits)
  { unlikely (inverted) ? s.del_bits (g, bits) : s.add_bits (g, bits); }
  தருக்க add_range (hb_codepoint_t a, hb_codepoint_t b)
  { திரும்பு unlikely (inverted) ? ((வெறுமை) s.del_range (a, b), உண்மை) : s.add_range (a, b); }

  template <typename T>
  வெறுமை add_array (மாறிலி T *array, unsigned முழு மொத்தம், unsigned முழு stride=அளவு(T))
  { inverted ? s.del_array (array, மொத்தம், stride) : s.add_array (array, மொத்தம், stride); }
  template <typename T>
  வெறுமை add_array (மாறிலி hb_array_t<மாறிலி T>& arr) { add_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  தருக்க add_sorted_array (மாறிலி T *array, unsigned முழு மொத்தம், unsigned முழு stride=அளவு(T))
  { திரும்பு inverted ? s.del_sorted_array (array, மொத்தம், stride) : s.add_sorted_array (array, மொத்தம், stride); }
  template <typename T>
  தருக்க add_sorted_array (மாறிலி hb_sorted_array_t<மாறிலி T>& arr) { திரும்பு add_sorted_array (&arr, arr.len ()); }

  வெறுமை del (hb_codepoint_t g) { unlikely (inverted) ? s.add (g) : s.del (g); }
  வெறுமை del_range (hb_codepoint_t a, hb_codepoint_t b)
  { unlikely (inverted) ? (வெறுமை) s.add_range (a, b) : s.del_range (a, b); }

  தருக்க get (hb_codepoint_t g) மாறிலி { திரும்பு s.get (g) ^ inverted; }
  தருக்க may_have (hb_codepoint_t g) மாறிலி { திரும்பு get (g); }

  /* Has interface. */
  தருக்க operator [] (hb_codepoint_t k) மாறிலி { திரும்பு get (k); }
  தருக்க has (hb_codepoint_t k) மாறிலி { திரும்பு (*இது)[k]; }
  /* Predicate. */
  தருக்க operator () (hb_codepoint_t k) மாறிலி { திரும்பு has (k); }

  /* Sink interface. */
  hb_bit_set_invertible_t& operator << (hb_codepoint_t v)
  { add (v); திரும்பு *இது; }
  hb_bit_set_invertible_t& operator << (மாறிலி hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); திரும்பு *இது; }

  தருக்க may_intersect (மாறிலி hb_bit_set_invertible_t &other) மாறிலி
  { திரும்பு inverted || other.inverted || s.intersects (other.s); }

  தருக்க intersects (மாறிலி hb_bit_set_invertible_t &other) மாறிலி
  {
    எனில் (likely (!inverted))
      திரும்பு unlikely (other.inverted) ? !s.is_subset (other.s)
				       : s.intersects (other.s);
    எனில் (likely (!other.inverted))
      திரும்பு !other.s.is_subset (s);

    unsigned population = s.get_population ();
    unsigned other_population = other.s.get_population ();
    எனில் ((uint64_t) population + other_population < INVALID)
      திரும்பு உண்மை;

    திரும்பு population >= other_population
	 ? !hb_all (iter (), other.s)
	 : !hb_all (other.iter (), s);
  }

  தருக்க intersects (hb_codepoint_t first, hb_codepoint_t last) மாறிலி
  {
    hb_codepoint_t c = first - 1;
    திரும்பு next (&c) && c <= last;
  }

  வெறுமை set (மாறிலி hb_bit_set_invertible_t &other)
  {
    s.set (other.s);
    எனில் (likely (s.successful))
      inverted = other.inverted;
  }

  தருக்க is_equal (மாறிலி hb_bit_set_invertible_t &other) மாறிலி
  {
    எனில் (likely (inverted == other.inverted))
      திரும்பு s.is_equal (other.s);
    இல்லையெனில்
    {
      எனில் (get_population () != other.get_population ())
	திரும்பு பொய்;

      /* TODO Add iter_ranges() and use here. */
      auto it1 = iter ();
      auto it2 = other.iter ();
      திரும்பு hb_all (+ hb_zip (it1, it2)
		     | hb_map ([](hb_codepoint_pair_t _) { திரும்பு _.first == _.second; }));
    }
  }

  தருக்க is_subset (மாறிலி hb_bit_set_invertible_t &larger_set) மாறிலி
  {
    எனில் (unlikely (inverted != larger_set.inverted))
    {
      எனில் (inverted)
	திரும்பு hb_all (iter (), larger_set.s);
      இல்லையெனில்
        // larger set is inverted so larger_set.s is the set of things that are not present
        // in larger_set, therefore if s has any of those it can't be a subset.
	திரும்பு !s.intersects (larger_set.s);
    }
    இல்லையெனில்
      திரும்பு unlikely (inverted) ? larger_set.s.is_subset (s) : s.is_subset (larger_set.s);
  }

  பாதுகாக்கப்பட்ட:
  template <typename Op>
  வெறுமை process (மாறிலி Op& op,
		மாறிலி hb_bit_set_invertible_t &other,
		hb_vector_t<unsigned> *workspace = nullptr)
  { s.process (op, other.s, workspace); }
  பொது:
  வெறுமை union_ (மாறிலி hb_bit_set_invertible_t &other)
  {
    எனில் (likely (inverted == other.inverted))
    {
      எனில் (unlikely (inverted))
	process (hb_bitwise_and, other);
      இல்லையெனில்
	process (hb_bitwise_or, other); /* Main branch. */
    }
    இல்லையெனில்
    {
      எனில் (unlikely (inverted))
	process (hb_bitwise_gt, other);
      இல்லையெனில்
	process (hb_bitwise_lt, other);
    }
    எனில் (likely (s.successful))
      inverted = inverted || other.inverted;
  }
  வெறுமை intersect (மாறிலி hb_bit_set_invertible_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  {
    எனில் (likely (inverted == other.inverted))
    {
      எனில் (unlikely (inverted))
	process (hb_bitwise_or, other, workspace);
      இல்லையெனில்
	process (hb_bitwise_and, other, workspace); /* Main branch. */
    }
    இல்லையெனில்
    {
      எனில் (unlikely (inverted))
	process (hb_bitwise_lt, other, workspace);
      இல்லையெனில்
	process (hb_bitwise_gt, other, workspace);
    }
    எனில் (likely (s.successful))
      inverted = inverted && other.inverted;
  }
  வெறுமை subtract (மாறிலி hb_bit_set_invertible_t &other)
  {
    எனில் (likely (inverted == other.inverted))
    {
      எனில் (unlikely (inverted))
	process (hb_bitwise_lt, other);
      இல்லையெனில்
	process (hb_bitwise_gt, other); /* Main branch. */
    }
    இல்லையெனில்
    {
      எனில் (unlikely (inverted))
	process (hb_bitwise_or, other);
      இல்லையெனில்
	process (hb_bitwise_and, other);
    }
    எனில் (likely (s.successful))
      inverted = inverted && !other.inverted;
  }
  வெறுமை symmetric_difference (மாறிலி hb_bit_set_invertible_t &other)
  {
    process (hb_bitwise_xor, other);
    எனில் (likely (s.successful))
      inverted = inverted ^ other.inverted;
  }

  தருக்க next (hb_codepoint_t *codepoint) மாறிலி
  {
    எனில் (likely (!inverted))
      திரும்பு s.next (codepoint);

    auto old = *codepoint;
    எனில் (unlikely (old + 1 == INVALID))
    {
      *codepoint = INVALID;
      திரும்பு பொய்;
    }

    auto v = old;
    s.next (&v);
    எனில் (old + 1 < v)
    {
      *codepoint = old + 1;
      திரும்பு உண்மை;
    }

    v = old;
    s.next_range (&old, &v);

    *codepoint = v + 1;
    திரும்பு *codepoint != INVALID;
  }
  தருக்க next_bits (hb_codepoint_t *codepoint, uint64_t *bits) மாறிலி
  {
    எனில் (likely (!inverted))
      திரும்பு s.next_bits (codepoint, bits);

    hb_codepoint_t base;
    எனில் (unlikely (*codepoint == INVALID))
      base = 0;
    இல்லையெனில்
    {
      base = *codepoint & ~hb_bit_page_t::ELT_MASK;
      எனில் (unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1))
	செல் done;
      base += hb_bit_page_t::ELT_BITS;
    }

    போது (உண்மை)
    {
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      uint64_t excluded = 0;
      எனில் (!s.next_bits (&cursor, &excluded) || cursor != base)
	excluded = 0;
      *bits = ~excluded;
      தருக்க last = unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1);
      எனில் (last)
	*bits &= UINT64_MAX >> 1;
      எனில் (*bits)
      {
	*codepoint = base;
	திரும்பு உண்மை;
      }
      எனில் (unlikely (last))
	நிறுத்து;
      base += hb_bit_page_t::ELT_BITS;
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    திரும்பு பொய்;
  }
  தருக்க previous (hb_codepoint_t *codepoint) மாறிலி
  {
    எனில் (likely (!inverted))
      திரும்பு s.previous (codepoint);

    auto old = *codepoint;
    எனில் (unlikely (old - 1 == INVALID))
    {
      *codepoint = INVALID;
      திரும்பு பொய்;
    }

    auto v = old;
    s.previous (&v);

    எனில் (old - 1 > v || v == INVALID)
    {
      *codepoint = old - 1;
      திரும்பு உண்மை;
    }

    v = old;
    s.previous_range (&v, &old);

    *codepoint = v - 1;
    திரும்பு *codepoint != INVALID;
  }
  தருக்க next_range (hb_codepoint_t *first, hb_codepoint_t *last) மாறிலி
  {
    எனில் (likely (!inverted))
      திரும்பு s.next_range (first, last);

    எனில் (!next (last))
    {
      *last = *first = INVALID;
      திரும்பு பொய்;
    }

    *first = *last;
    s.next (last);
    --*last;
    திரும்பு உண்மை;
  }
  தருக்க previous_range (hb_codepoint_t *first, hb_codepoint_t *last) மாறிலி
  {
    எனில் (likely (!inverted))
      திரும்பு s.previous_range (first, last);

    எனில் (!previous (first))
    {
      *last = *first = INVALID;
      திரும்பு பொய்;
    }

    *last = *first;
    s.previous (first);
    ++*first;
    திரும்பு உண்மை;
  }

  unsigned முழு next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  unsigned முழு    size) மாறிலி
  {
    திரும்பு inverted ? s.next_many_inverted (codepoint, out, size)
		    : s.next_many (codepoint, out, size);
  }

  நிலையான constexpr hb_codepoint_t INVALID = hb_bit_set_t::INVALID;

  /*
   * Iterator implementation.
   * Mutating the set invalidates its iterators.
   */
  கட்டமைப்பு iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    நிலையான constexpr தருக்க is_sorted_iterator = உண்மை;
    நிலையான constexpr தருக்க has_fast_len = உண்மை;
    iter_t (மாறிலி hb_bit_set_invertible_t &s_ = Null (hb_bit_set_invertible_t),
    தருக்க init = உண்மை) : s (&s_), v (INVALID), remaining (0),
				     have_len (!init), base (INVALID), bits (0)
    {
      எனில் (init)
	advance ();
    }

    வகைவரை hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () மாறிலி { திரும்பு v; }
    தருக்க __more__ () மாறிலி { திரும்பு v != INVALID; }
    வெறுமை __next__ ()
    {
      remaining--;
      advance ();
    }
    வெறுமை __prev__ ()
    {
      எனில் (s->previous (&v))
      {
	ஒத்திசை ();
	remaining++;
      }
      இல்லையெனில்
      {
	base = INVALID;
	bits = 0;
      }
    }
    unsigned __len__ () மாறிலி
    {
      எனில் (unlikely (!have_len))
      {
	remaining += s->get_population ();
	have_len = உண்மை;
      }
      திரும்பு remaining;
    }
    iter_t end () மாறிலி { திரும்பு iter_t (*s, பொய்); }
    தருக்க operator != (மாறிலி iter_t& o) மாறிலி
    { திரும்பு v != o.v; }

    பாதுகாக்கப்பட்ட:
    மாறிலி hb_bit_set_invertible_t *s;
    hb_codepoint_t v;
    mutable unsigned remaining;
    mutable தருக்க have_len;
    hb_codepoint_t base;
    uint64_t bits;

    வெறுமை advance ()
    {
      எனில் (likely (bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      இல்லையெனில் எனில் (s->next_bits (&base, &bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      இல்லையெனில்
	v = INVALID;
    }

    வெறுமை ஒத்திசை ()
    {
      base = v & ~hb_bit_page_t::ELT_MASK;
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      s->next_bits (&cursor, &bits);
      unsigned முழு bit = v & (hb_bit_page_t::ELT_BITS - 1);
      bits = bit == hb_bit_page_t::ELT_BITS - 1 ? 0 : bits >> (bit + 1) << (bit + 1);
    }
  };
  iter_t iter () மாறிலி { திரும்பு iter_t (*இது); }
  operator iter_t () மாறிலி { திரும்பு iter (); }
};


#endif /* HB_BIT_SET_INVERTIBLE_HH */

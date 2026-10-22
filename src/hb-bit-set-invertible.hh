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

#include "hb.hh"
#include "hb-bit-set.hh"


sa_nracanaa hb_bit_set_invertible_t
{
  hb_bit_set_t s;
  bool inverted = false;

  hb_bit_set_invertible_t () = default;
  hb_bit_set_invertible_t (const hb_bit_set_invertible_t& o) = default;
  hb_bit_set_invertible_t (hb_bit_set_invertible_t&& other)  noexcept : hb_bit_set_invertible_t () { hb_swap (*this, other); }
  hb_bit_set_invertible_t& operator= (const hb_bit_set_invertible_t& o) = default;
  hb_bit_set_invertible_t& operator= (hb_bit_set_invertible_t&& other)  noexcept { hb_swap (*this, other); ආපසු *this; }
  friend void swap (hb_bit_set_invertible_t &a, hb_bit_set_invertible_t &b) noexcept
  {
    නම් (likely (!a.s.successful || !b.s.successful))
      ආපසු;
    hb_swap (a.inverted, b.inverted);
    hb_swap (a.s, b.s);
  }

  void init () { s.init (); inverted = false; }
  void fini () { s.fini (); }
  void err () { s.err (); }
  bool in_error () const { ආපසු s.in_error (); }
  explicit operator bool () const { ආපසු !is_empty (); }

  void alloc (unsigned sz) { s.alloc (sz); }
  void reset ()
  {
    s.reset ();
    inverted = false;
  }
  void clear ()
  {
    s.clear ();
    නම් (likely (s.successful))
      inverted = false;
  }
  void invert ()
  {
    නම් (likely (s.successful))
      inverted = !inverted;
  }

  bool is_inverted () const
  {
    ආපසු inverted;
  }

  bool is_empty () const
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    ආපසු v == INVALID;
  }
  uint32_t hash () const { ආපසු s.hash () ^ (uint32_t) inverted; }

  hb_codepoint_t get_min () const
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    ආපසු v;
  }
  hb_codepoint_t get_max () const
  {
    hb_codepoint_t v = INVALID;
    previous (&v);
    ආපසු v;
  }
  unsigned පූර්ණ_සංඛ්‍යා get_population () const
  { ආපසු inverted ? INVALID - s.get_population () : s.get_population (); }
  bool get_singleton (hb_codepoint_t *codepoint) const
  {
    නම් (likely (!inverted))
      ආපසු s.get_singleton (codepoint);
    නම් (s.get_population () != INVALID - 1)
      ආපසු false;
    *codepoint = get_min ();
    ආපසු *codepoint != INVALID;
  }


  void add (hb_codepoint_t g) { unlikely (inverted) ? s.del (g) : s.add (g); }
  void add_bits (hb_codepoint_t g, uint64_t bits)
  { unlikely (inverted) ? s.del_bits (g, bits) : s.add_bits (g, bits); }
  bool add_range (hb_codepoint_t a, hb_codepoint_t b)
  { ආපසු unlikely (inverted) ? ((void) s.del_range (a, b), true) : s.add_range (a, b); }

  template <typename T>
  void add_array (const T *array, unsigned පූර්ණ_සංඛ්‍යා count, unsigned පූර්ණ_සංඛ්‍යා stride=parimaa_na(T))
  { inverted ? s.del_array (array, count, stride) : s.add_array (array, count, stride); }
  template <typename T>
  void add_array (const hb_array_t<const T>& arr) { add_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  bool add_sorted_array (const T *array, unsigned පූර්ණ_සංඛ්‍යා count, unsigned පූර්ණ_සංඛ්‍යා stride=parimaa_na(T))
  { ආපසු inverted ? s.del_sorted_array (array, count, stride) : s.add_sorted_array (array, count, stride); }
  template <typename T>
  bool add_sorted_array (const hb_sorted_array_t<const T>& arr) { ආපසු add_sorted_array (&arr, arr.len ()); }

  void del (hb_codepoint_t g) { unlikely (inverted) ? s.add (g) : s.del (g); }
  void del_range (hb_codepoint_t a, hb_codepoint_t b)
  { unlikely (inverted) ? (void) s.add_range (a, b) : s.del_range (a, b); }

  bool get (hb_codepoint_t g) const { ආපසු s.get (g) ^ inverted; }
  bool may_have (hb_codepoint_t g) const { ආපසු get (g); }

  /* Has interface. */
  bool operator [] (hb_codepoint_t k) const { ආපසු get (k); }
  bool has (hb_codepoint_t k) const { ආපසු (*this)[k]; }
  /* Predicate. */
  bool operator () (hb_codepoint_t k) const { ආපසු has (k); }

  /* Sink interface. */
  hb_bit_set_invertible_t& operator << (hb_codepoint_t v)
  { add (v); ආපසු *this; }
  hb_bit_set_invertible_t& operator << (const hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); ආපසු *this; }

  bool may_intersect (const hb_bit_set_invertible_t &other) const
  { ආපසු inverted || other.inverted || s.intersects (other.s); }

  bool intersects (const hb_bit_set_invertible_t &other) const
  {
    නම් (likely (!inverted))
      ආපසු unlikely (other.inverted) ? !s.is_subset (other.s)
				       : s.intersects (other.s);
    නම් (likely (!other.inverted))
      ආපසු !other.s.is_subset (s);

    unsigned population = s.get_population ();
    unsigned other_population = other.s.get_population ();
    නම් ((uint64_t) population + other_population < INVALID)
      ආපසු true;

    ආපසු population >= other_population
	 ? !hb_all (iter (), other.s)
	 : !hb_all (other.iter (), s);
  }

  bool intersects (hb_codepoint_t first, hb_codepoint_t last) const
  {
    hb_codepoint_t c = first - 1;
    ආපසු next (&c) && c <= last;
  }

  void set (const hb_bit_set_invertible_t &other)
  {
    s.set (other.s);
    නම් (likely (s.successful))
      inverted = other.inverted;
  }

  bool is_equal (const hb_bit_set_invertible_t &other) const
  {
    නම් (likely (inverted == other.inverted))
      ආපසු s.is_equal (other.s);
    නැතහොත්
    {
      නම් (get_population () != other.get_population ())
	ආපසු false;

      /* TODO Add iter_ranges() and use here. */
      auto it1 = iter ();
      auto it2 = other.iter ();
      ආපසු hb_all (+ hb_zip (it1, it2)
		     | hb_map ([](hb_codepoint_pair_t _) { ආපසු _.first == _.second; }));
    }
  }

  bool is_subset (const hb_bit_set_invertible_t &larger_set) const
  {
    නම් (unlikely (inverted != larger_set.inverted))
    {
      නම් (inverted)
	ආපසු hb_all (iter (), larger_set.s);
      නැතහොත්
        // larger set is inverted so larger_set.s is the set of things that are not present
        // in larger_set, therefore if s has any of those it can't be a subset.
	ආපසු !s.intersects (larger_set.s);
    }
    නැතහොත්
      ආපසු unlikely (inverted) ? larger_set.s.is_subset (s) : s.is_subset (larger_set.s);
  }

  rak_sita:
  template <typename Op>
  void process (const Op& op,
		const hb_bit_set_invertible_t &other,
		hb_vector_t<unsigned> *workspace = nullptr)
  { s.process (op, other.s, workspace); }
  public:
  void union_ (const hb_bit_set_invertible_t &other)
  {
    නම් (likely (inverted == other.inverted))
    {
      නම් (unlikely (inverted))
	process (hb_bitwise_and, other);
      නැතහොත්
	process (hb_bitwise_or, other); /* Main branch. */
    }
    නැතහොත්
    {
      නම් (unlikely (inverted))
	process (hb_bitwise_gt, other);
      නැතහොත්
	process (hb_bitwise_lt, other);
    }
    නම් (likely (s.successful))
      inverted = inverted || other.inverted;
  }
  void intersect (const hb_bit_set_invertible_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  {
    නම් (likely (inverted == other.inverted))
    {
      නම් (unlikely (inverted))
	process (hb_bitwise_or, other, workspace);
      නැතහොත්
	process (hb_bitwise_and, other, workspace); /* Main branch. */
    }
    නැතහොත්
    {
      නම් (unlikely (inverted))
	process (hb_bitwise_lt, other, workspace);
      නැතහොත්
	process (hb_bitwise_gt, other, workspace);
    }
    නම් (likely (s.successful))
      inverted = inverted && other.inverted;
  }
  void subtract (const hb_bit_set_invertible_t &other)
  {
    නම් (likely (inverted == other.inverted))
    {
      නම් (unlikely (inverted))
	process (hb_bitwise_lt, other);
      නැතහොත්
	process (hb_bitwise_gt, other); /* Main branch. */
    }
    නැතහොත්
    {
      නම් (unlikely (inverted))
	process (hb_bitwise_or, other);
      නැතහොත්
	process (hb_bitwise_and, other);
    }
    නම් (likely (s.successful))
      inverted = inverted && !other.inverted;
  }
  void symmetric_difference (const hb_bit_set_invertible_t &other)
  {
    process (hb_bitwise_xor, other);
    නම් (likely (s.successful))
      inverted = inverted ^ other.inverted;
  }

  bool next (hb_codepoint_t *codepoint) const
  {
    නම් (likely (!inverted))
      ආපසු s.next (codepoint);

    auto old = *codepoint;
    නම් (unlikely (old + 1 == INVALID))
    {
      *codepoint = INVALID;
      ආපසු false;
    }

    auto v = old;
    s.next (&v);
    නම් (old + 1 < v)
    {
      *codepoint = old + 1;
      ආපසු true;
    }

    v = old;
    s.next_range (&old, &v);

    *codepoint = v + 1;
    ආපසු *codepoint != INVALID;
  }
  bool next_bits (hb_codepoint_t *codepoint, uint64_t *bits) const
  {
    නම් (likely (!inverted))
      ආපසු s.next_bits (codepoint, bits);

    hb_codepoint_t base;
    නම් (unlikely (*codepoint == INVALID))
      base = 0;
    නැතහොත්
    {
      base = *codepoint & ~hb_bit_page_t::ELT_MASK;
      නම් (unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1))
	goto done;
      base += hb_bit_page_t::ELT_BITS;
    }

    අතරතුර (true)
    {
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      uint64_t excluded = 0;
      නම් (!s.next_bits (&cursor, &excluded) || cursor != base)
	excluded = 0;
      *bits = ~excluded;
      bool last = unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1);
      නම් (last)
	*bits &= UINT64_MAX >> 1;
      නම් (*bits)
      {
	*codepoint = base;
	ආපසු true;
      }
      නම් (unlikely (last))
	bha_nga;
      base += hb_bit_page_t::ELT_BITS;
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    ආපසු false;
  }
  bool previous (hb_codepoint_t *codepoint) const
  {
    නම් (likely (!inverted))
      ආපසු s.previous (codepoint);

    auto old = *codepoint;
    නම් (unlikely (old - 1 == INVALID))
    {
      *codepoint = INVALID;
      ආපසු false;
    }

    auto v = old;
    s.previous (&v);

    නම් (old - 1 > v || v == INVALID)
    {
      *codepoint = old - 1;
      ආපසු true;
    }

    v = old;
    s.previous_range (&v, &old);

    *codepoint = v - 1;
    ආපසු *codepoint != INVALID;
  }
  bool next_range (hb_codepoint_t *first, hb_codepoint_t *last) const
  {
    නම් (likely (!inverted))
      ආපසු s.next_range (first, last);

    නම් (!next (last))
    {
      *last = *first = INVALID;
      ආපසු false;
    }

    *first = *last;
    s.next (last);
    --*last;
    ආපසු true;
  }
  bool previous_range (hb_codepoint_t *first, hb_codepoint_t *last) const
  {
    නම් (likely (!inverted))
      ආපසු s.previous_range (first, last);

    නම් (!previous (first))
    {
      *last = *first = INVALID;
      ආපසු false;
    }

    *last = *first;
    s.previous (first);
    ++*first;
    ආපසු true;
  }

  unsigned පූර්ණ_සංඛ්‍යා next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  unsigned පූර්ණ_සංඛ්‍යා    size) const
  {
    ආපසු inverted ? s.next_many_inverted (codepoint, out, size)
		    : s.next_many (codepoint, out, size);
  }

  static constexpr hb_codepoint_t INVALID = hb_bit_set_t::INVALID;

  /*
   * Iterator implementation.
   * Mutating the set invalidates its iterators.
   */
  sa_nracanaa iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    static constexpr bool is_sorted_iterator = true;
    static constexpr bool has_fast_len = true;
    iter_t (const hb_bit_set_invertible_t &s_ = Null (hb_bit_set_invertible_t),
    bool init = true) : s (&s_), v (INVALID), remaining (0),
				     have_len (!init), base (INVALID), bits (0)
    {
      නම් (init)
	advance ();
    }

    typedef hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () const { ආපසු v; }
    bool __more__ () const { ආපසු v != INVALID; }
    void __next__ ()
    {
      remaining--;
      advance ();
    }
    void __prev__ ()
    {
      නම් (s->previous (&v))
      {
	sync ();
	remaining++;
      }
      නැතහොත්
      {
	base = INVALID;
	bits = 0;
      }
    }
    unsigned __len__ () const
    {
      නම් (unlikely (!have_len))
      {
	remaining += s->get_population ();
	have_len = true;
      }
      ආපසු remaining;
    }
    iter_t end () const { ආපසු iter_t (*s, false); }
    bool operator != (const iter_t& o) const
    { ආපසු v != o.v; }

    rak_sita:
    const hb_bit_set_invertible_t *s;
    hb_codepoint_t v;
    mutable unsigned remaining;
    mutable bool have_len;
    hb_codepoint_t base;
    uint64_t bits;

    void advance ()
    {
      නම් (likely (bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      නැතහොත් නම් (s->next_bits (&base, &bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      නැතහොත්
	v = INVALID;
    }

    void sync ()
    {
      base = v & ~hb_bit_page_t::ELT_MASK;
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      s->next_bits (&cursor, &bits);
      unsigned පූර්ණ_සංඛ්‍යා bit = v & (hb_bit_page_t::ELT_BITS - 1);
      bits = bit == hb_bit_page_t::ELT_BITS - 1 ? 0 : bits >> (bit + 1) << (bit + 1);
    }
  };
  iter_t iter () const { ආපසු iter_t (*this); }
  operator iter_t () const { ආපසු iter (); }
};


#endif /* HB_BIT_SET_INVERTIBLE_HH */

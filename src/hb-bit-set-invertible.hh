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


struct hb_bit_set_invertible_t
{
  hb_bit_set_t s;
  bool inverted = false;

  hb_bit_set_invertible_t () = default;
  hb_bit_set_invertible_t (const hb_bit_set_invertible_t& o) = default;
  hb_bit_set_invertible_t (hb_bit_set_invertible_t&& other)  noexcept : hb_bit_set_invertible_t () { hb_swap (*this, other); }
  hb_bit_set_invertible_t& operator= (const hb_bit_set_invertible_t& o) = default;
  hb_bit_set_invertible_t& operator= (hb_bit_set_invertible_t&& other)  noexcept { hb_swap (*this, other); врати *this; }
  friend void swap (hb_bit_set_invertible_t &a, hb_bit_set_invertible_t &b) noexcept
  {
    ако (likely (!a.s.successful || !b.s.successful))
      врати;
    hb_swap (a.inverted, b.inverted);
    hb_swap (a.s, b.s);
  }

  void init () { s.init (); inverted = false; }
  void fini () { s.fini (); }
  void err () { s.err (); }
  bool in_error () const { врати s.in_error (); }
  explicit operator bool () const { врати !is_empty (); }

  void alloc (unsigned sz) { s.alloc (sz); }
  void reset ()
  {
    s.reset ();
    inverted = false;
  }
  void clear ()
  {
    s.clear ();
    ако (likely (s.successful))
      inverted = false;
  }
  void invert ()
  {
    ако (likely (s.successful))
      inverted = !inverted;
  }

  bool is_inverted () const
  {
    врати inverted;
  }

  bool is_empty () const
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    врати v == INVALID;
  }
  uint32_t hash () const { врати s.hash () ^ (uint32_t) inverted; }

  hb_codepoint_t get_min () const
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    врати v;
  }
  hb_codepoint_t get_max () const
  {
    hb_codepoint_t v = INVALID;
    previous (&v);
    врати v;
  }
  unsigned цео get_population () const
  { врати inverted ? INVALID - s.get_population () : s.get_population (); }
  bool get_singleton (hb_codepoint_t *codepoint) const
  {
    ако (likely (!inverted))
      врати s.get_singleton (codepoint);
    ако (s.get_population () != INVALID - 1)
      врати false;
    *codepoint = get_min ();
    врати *codepoint != INVALID;
  }


  void add (hb_codepoint_t g) { unlikely (inverted) ? s.del (g) : s.add (g); }
  void add_bits (hb_codepoint_t g, uint64_t bits)
  { unlikely (inverted) ? s.del_bits (g, bits) : s.add_bits (g, bits); }
  bool add_range (hb_codepoint_t a, hb_codepoint_t b)
  { врати unlikely (inverted) ? ((void) s.del_range (a, b), true) : s.add_range (a, b); }

  template <typename T>
  void add_array (const T *array, unsigned цео count, unsigned цео stride=sizeof(T))
  { inverted ? s.del_array (array, count, stride) : s.add_array (array, count, stride); }
  template <typename T>
  void add_array (const hb_array_t<const T>& arr) { add_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  bool add_sorted_array (const T *array, unsigned цео count, unsigned цео stride=sizeof(T))
  { врати inverted ? s.del_sorted_array (array, count, stride) : s.add_sorted_array (array, count, stride); }
  template <typename T>
  bool add_sorted_array (const hb_sorted_array_t<const T>& arr) { врати add_sorted_array (&arr, arr.len ()); }

  void del (hb_codepoint_t g) { unlikely (inverted) ? s.add (g) : s.del (g); }
  void del_range (hb_codepoint_t a, hb_codepoint_t b)
  { unlikely (inverted) ? (void) s.add_range (a, b) : s.del_range (a, b); }

  bool get (hb_codepoint_t g) const { врати s.get (g) ^ inverted; }
  bool may_have (hb_codepoint_t g) const { врати get (g); }

  /* Has interface. */
  bool operator [] (hb_codepoint_t k) const { врати get (k); }
  bool has (hb_codepoint_t k) const { врати (*this)[k]; }
  /* Predicate. */
  bool operator () (hb_codepoint_t k) const { врати has (k); }

  /* Sink interface. */
  hb_bit_set_invertible_t& operator << (hb_codepoint_t v)
  { add (v); врати *this; }
  hb_bit_set_invertible_t& operator << (const hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); врати *this; }

  bool may_intersect (const hb_bit_set_invertible_t &other) const
  { врати inverted || other.inverted || s.intersects (other.s); }

  bool intersects (const hb_bit_set_invertible_t &other) const
  {
    ако (likely (!inverted))
      врати unlikely (other.inverted) ? !s.is_subset (other.s)
				       : s.intersects (other.s);
    ако (likely (!other.inverted))
      врати !other.s.is_subset (s);

    unsigned population = s.get_population ();
    unsigned other_population = other.s.get_population ();
    ако ((uint64_t) population + other_population < INVALID)
      врати true;

    врати population >= other_population
	 ? !hb_all (iter (), other.s)
	 : !hb_all (other.iter (), s);
  }

  bool intersects (hb_codepoint_t first, hb_codepoint_t last) const
  {
    hb_codepoint_t c = first - 1;
    врати next (&c) && c <= last;
  }

  void set (const hb_bit_set_invertible_t &other)
  {
    s.set (other.s);
    ако (likely (s.successful))
      inverted = other.inverted;
  }

  bool is_equal (const hb_bit_set_invertible_t &other) const
  {
    ако (likely (inverted == other.inverted))
      врати s.is_equal (other.s);
    инако
    {
      ако (get_population () != other.get_population ())
	врати false;

      /* TODO Add iter_ranges() and use here. */
      auto it1 = iter ();
      auto it2 = other.iter ();
      врати hb_all (+ hb_zip (it1, it2)
		     | hb_map ([](hb_codepoint_pair_t _) { врати _.first == _.second; }));
    }
  }

  bool is_subset (const hb_bit_set_invertible_t &larger_set) const
  {
    ако (unlikely (inverted != larger_set.inverted))
    {
      ако (inverted)
	врати hb_all (iter (), larger_set.s);
      инако
        // larger set is inverted so larger_set.s is the set of things that are not present
        // in larger_set, therefore if s has any of those it can't be a subset.
	врати !s.intersects (larger_set.s);
    }
    инако
      врати unlikely (inverted) ? larger_set.s.is_subset (s) : s.is_subset (larger_set.s);
  }

  protected:
  template <typename Op>
  void process (const Op& op,
		const hb_bit_set_invertible_t &other,
		hb_vector_t<unsigned> *workspace = nullptr)
  { s.process (op, other.s, workspace); }
  public:
  void union_ (const hb_bit_set_invertible_t &other)
  {
    ако (likely (inverted == other.inverted))
    {
      ако (unlikely (inverted))
	process (hb_bitwise_and, other);
      инако
	process (hb_bitwise_or, other); /* Main branch. */
    }
    инако
    {
      ако (unlikely (inverted))
	process (hb_bitwise_gt, other);
      инако
	process (hb_bitwise_lt, other);
    }
    ако (likely (s.successful))
      inverted = inverted || other.inverted;
  }
  void intersect (const hb_bit_set_invertible_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  {
    ако (likely (inverted == other.inverted))
    {
      ако (unlikely (inverted))
	process (hb_bitwise_or, other, workspace);
      инако
	process (hb_bitwise_and, other, workspace); /* Main branch. */
    }
    инако
    {
      ако (unlikely (inverted))
	process (hb_bitwise_lt, other, workspace);
      инако
	process (hb_bitwise_gt, other, workspace);
    }
    ако (likely (s.successful))
      inverted = inverted && other.inverted;
  }
  void subtract (const hb_bit_set_invertible_t &other)
  {
    ако (likely (inverted == other.inverted))
    {
      ако (unlikely (inverted))
	process (hb_bitwise_lt, other);
      инако
	process (hb_bitwise_gt, other); /* Main branch. */
    }
    инако
    {
      ако (unlikely (inverted))
	process (hb_bitwise_or, other);
      инако
	process (hb_bitwise_and, other);
    }
    ако (likely (s.successful))
      inverted = inverted && !other.inverted;
  }
  void symmetric_difference (const hb_bit_set_invertible_t &other)
  {
    process (hb_bitwise_xor, other);
    ако (likely (s.successful))
      inverted = inverted ^ other.inverted;
  }

  bool next (hb_codepoint_t *codepoint) const
  {
    ако (likely (!inverted))
      врати s.next (codepoint);

    auto old = *codepoint;
    ако (unlikely (old + 1 == INVALID))
    {
      *codepoint = INVALID;
      врати false;
    }

    auto v = old;
    s.next (&v);
    ако (old + 1 < v)
    {
      *codepoint = old + 1;
      врати true;
    }

    v = old;
    s.next_range (&old, &v);

    *codepoint = v + 1;
    врати *codepoint != INVALID;
  }
  bool next_bits (hb_codepoint_t *codepoint, uint64_t *bits) const
  {
    ако (likely (!inverted))
      врати s.next_bits (codepoint, bits);

    hb_codepoint_t base;
    ако (unlikely (*codepoint == INVALID))
      base = 0;
    инако
    {
      base = *codepoint & ~hb_bit_page_t::ELT_MASK;
      ако (unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1))
	goto done;
      base += hb_bit_page_t::ELT_BITS;
    }

    док (true)
    {
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      uint64_t excluded = 0;
      ако (!s.next_bits (&cursor, &excluded) || cursor != base)
	excluded = 0;
      *bits = ~excluded;
      bool last = unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1);
      ако (last)
	*bits &= UINT64_MAX >> 1;
      ако (*bits)
      {
	*codepoint = base;
	врати true;
      }
      ако (unlikely (last))
	break;
      base += hb_bit_page_t::ELT_BITS;
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    врати false;
  }
  bool previous (hb_codepoint_t *codepoint) const
  {
    ако (likely (!inverted))
      врати s.previous (codepoint);

    auto old = *codepoint;
    ако (unlikely (old - 1 == INVALID))
    {
      *codepoint = INVALID;
      врати false;
    }

    auto v = old;
    s.previous (&v);

    ако (old - 1 > v || v == INVALID)
    {
      *codepoint = old - 1;
      врати true;
    }

    v = old;
    s.previous_range (&v, &old);

    *codepoint = v - 1;
    врати *codepoint != INVALID;
  }
  bool next_range (hb_codepoint_t *first, hb_codepoint_t *last) const
  {
    ако (likely (!inverted))
      врати s.next_range (first, last);

    ако (!next (last))
    {
      *last = *first = INVALID;
      врати false;
    }

    *first = *last;
    s.next (last);
    --*last;
    врати true;
  }
  bool previous_range (hb_codepoint_t *first, hb_codepoint_t *last) const
  {
    ако (likely (!inverted))
      врати s.previous_range (first, last);

    ако (!previous (first))
    {
      *last = *first = INVALID;
      врати false;
    }

    *last = *first;
    s.previous (first);
    ++*first;
    врати true;
  }

  unsigned цео next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  unsigned цео    size) const
  {
    врати inverted ? s.next_many_inverted (codepoint, out, size)
		    : s.next_many (codepoint, out, size);
  }

  static constexpr hb_codepoint_t INVALID = hb_bit_set_t::INVALID;

  /*
   * Iterator implementation.
   * Mutating the set invalidates its iterators.
   */
  struct iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    static constexpr bool is_sorted_iterator = true;
    static constexpr bool has_fast_len = true;
    iter_t (const hb_bit_set_invertible_t &s_ = Null (hb_bit_set_invertible_t),
    bool init = true) : s (&s_), v (INVALID), remaining (0),
				     have_len (!init), base (INVALID), bits (0)
    {
      ако (init)
	advance ();
    }

    typedef hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () const { врати v; }
    bool __more__ () const { врати v != INVALID; }
    void __next__ ()
    {
      remaining--;
      advance ();
    }
    void __prev__ ()
    {
      ако (s->previous (&v))
      {
	sync ();
	remaining++;
      }
      инако
      {
	base = INVALID;
	bits = 0;
      }
    }
    unsigned __len__ () const
    {
      ако (unlikely (!have_len))
      {
	remaining += s->get_population ();
	have_len = true;
      }
      врати remaining;
    }
    iter_t end () const { врати iter_t (*s, false); }
    bool operator != (const iter_t& o) const
    { врати v != o.v; }

    protected:
    const hb_bit_set_invertible_t *s;
    hb_codepoint_t v;
    mutable unsigned remaining;
    mutable bool have_len;
    hb_codepoint_t base;
    uint64_t bits;

    void advance ()
    {
      ако (likely (bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      инако ако (s->next_bits (&base, &bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      инако
	v = INVALID;
    }

    void sync ()
    {
      base = v & ~hb_bit_page_t::ELT_MASK;
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      s->next_bits (&cursor, &bits);
      unsigned цео bit = v & (hb_bit_page_t::ELT_BITS - 1);
      bits = bit == hb_bit_page_t::ELT_BITS - 1 ? 0 : bits >> (bit + 1) << (bit + 1);
    }
  };
  iter_t iter () const { врати iter_t (*this); }
  operator iter_t () const { врати iter (); }
};


#endif /* HB_BIT_SET_INVERTIBLE_HH */

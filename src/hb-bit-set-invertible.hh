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
  hb_bit_set_invertible_t& operator= (hb_bit_set_invertible_t&& other)  noexcept { hb_swap (*this, other); 반환 *this; }
  friend void swap (hb_bit_set_invertible_t &a, hb_bit_set_invertible_t &b) noexcept
  {
    만약 (likely (!a.s.successful || !b.s.successful))
      반환;
    hb_swap (a.inverted, b.inverted);
    hb_swap (a.s, b.s);
  }

  void init () { s.init (); inverted = false; }
  void fini () { s.fini (); }
  void err () { s.err (); }
  bool in_error () const { 반환 s.in_error (); }
  explicit operator bool () const { 반환 !is_empty (); }

  void alloc (unsigned sz) { s.alloc (sz); }
  void reset ()
  {
    s.reset ();
    inverted = false;
  }
  void clear ()
  {
    s.clear ();
    만약 (likely (s.successful))
      inverted = false;
  }
  void invert ()
  {
    만약 (likely (s.successful))
      inverted = !inverted;
  }

  bool is_inverted () const
  {
    반환 inverted;
  }

  bool is_empty () const
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    반환 v == INVALID;
  }
  uint32_t hash () const { 반환 s.hash () ^ (uint32_t) inverted; }

  hb_codepoint_t get_min () const
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    반환 v;
  }
  hb_codepoint_t get_max () const
  {
    hb_codepoint_t v = INVALID;
    previous (&v);
    반환 v;
  }
  unsigned 정수 get_population () const
  { 반환 inverted ? INVALID - s.get_population () : s.get_population (); }
  bool get_singleton (hb_codepoint_t *codepoint) const
  {
    만약 (likely (!inverted))
      반환 s.get_singleton (codepoint);
    만약 (s.get_population () != INVALID - 1)
      반환 false;
    *codepoint = get_min ();
    반환 *codepoint != INVALID;
  }


  void add (hb_codepoint_t g) { unlikely (inverted) ? s.del (g) : s.add (g); }
  void add_bits (hb_codepoint_t g, uint64_t bits)
  { unlikely (inverted) ? s.del_bits (g, bits) : s.add_bits (g, bits); }
  bool add_range (hb_codepoint_t a, hb_codepoint_t b)
  { 반환 unlikely (inverted) ? ((void) s.del_range (a, b), true) : s.add_range (a, b); }

  template <typename T>
  void add_array (const T *array, unsigned 정수 count, unsigned 정수 stride=sizeof(T))
  { inverted ? s.del_array (array, count, stride) : s.add_array (array, count, stride); }
  template <typename T>
  void add_array (const hb_array_t<const T>& arr) { add_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  bool add_sorted_array (const T *array, unsigned 정수 count, unsigned 정수 stride=sizeof(T))
  { 반환 inverted ? s.del_sorted_array (array, count, stride) : s.add_sorted_array (array, count, stride); }
  template <typename T>
  bool add_sorted_array (const hb_sorted_array_t<const T>& arr) { 반환 add_sorted_array (&arr, arr.len ()); }

  void del (hb_codepoint_t g) { unlikely (inverted) ? s.add (g) : s.del (g); }
  void del_range (hb_codepoint_t a, hb_codepoint_t b)
  { unlikely (inverted) ? (void) s.add_range (a, b) : s.del_range (a, b); }

  bool get (hb_codepoint_t g) const { 반환 s.get (g) ^ inverted; }
  bool may_have (hb_codepoint_t g) const { 반환 get (g); }

  /* Has interface. */
  bool operator [] (hb_codepoint_t k) const { 반환 get (k); }
  bool has (hb_codepoint_t k) const { 반환 (*this)[k]; }
  /* Predicate. */
  bool operator () (hb_codepoint_t k) const { 반환 has (k); }

  /* Sink interface. */
  hb_bit_set_invertible_t& operator << (hb_codepoint_t v)
  { add (v); 반환 *this; }
  hb_bit_set_invertible_t& operator << (const hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); 반환 *this; }

  bool may_intersect (const hb_bit_set_invertible_t &other) const
  { 반환 inverted || other.inverted || s.intersects (other.s); }

  bool intersects (const hb_bit_set_invertible_t &other) const
  {
    만약 (likely (!inverted))
      반환 unlikely (other.inverted) ? !s.is_subset (other.s)
				       : s.intersects (other.s);
    만약 (likely (!other.inverted))
      반환 !other.s.is_subset (s);

    unsigned population = s.get_population ();
    unsigned other_population = other.s.get_population ();
    만약 ((uint64_t) population + other_population < INVALID)
      반환 true;

    반환 population >= other_population
	 ? !hb_all (iter (), other.s)
	 : !hb_all (other.iter (), s);
  }

  bool intersects (hb_codepoint_t first, hb_codepoint_t last) const
  {
    hb_codepoint_t c = first - 1;
    반환 next (&c) && c <= last;
  }

  void set (const hb_bit_set_invertible_t &other)
  {
    s.set (other.s);
    만약 (likely (s.successful))
      inverted = other.inverted;
  }

  bool is_equal (const hb_bit_set_invertible_t &other) const
  {
    만약 (likely (inverted == other.inverted))
      반환 s.is_equal (other.s);
    아니면
    {
      만약 (get_population () != other.get_population ())
	반환 false;

      /* TODO Add iter_ranges() and use here. */
      auto it1 = iter ();
      auto it2 = other.iter ();
      반환 hb_all (+ hb_zip (it1, it2)
		     | hb_map ([](hb_codepoint_pair_t _) { 반환 _.first == _.second; }));
    }
  }

  bool is_subset (const hb_bit_set_invertible_t &larger_set) const
  {
    만약 (unlikely (inverted != larger_set.inverted))
    {
      만약 (inverted)
	반환 hb_all (iter (), larger_set.s);
      아니면
        // larger set is inverted so larger_set.s is the set of things that are not present
        // in larger_set, therefore if s has any of those it can't be a subset.
	반환 !s.intersects (larger_set.s);
    }
    아니면
      반환 unlikely (inverted) ? larger_set.s.is_subset (s) : s.is_subset (larger_set.s);
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
    만약 (likely (inverted == other.inverted))
    {
      만약 (unlikely (inverted))
	process (hb_bitwise_and, other);
      아니면
	process (hb_bitwise_or, other); /* Main branch. */
    }
    아니면
    {
      만약 (unlikely (inverted))
	process (hb_bitwise_gt, other);
      아니면
	process (hb_bitwise_lt, other);
    }
    만약 (likely (s.successful))
      inverted = inverted || other.inverted;
  }
  void intersect (const hb_bit_set_invertible_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  {
    만약 (likely (inverted == other.inverted))
    {
      만약 (unlikely (inverted))
	process (hb_bitwise_or, other, workspace);
      아니면
	process (hb_bitwise_and, other, workspace); /* Main branch. */
    }
    아니면
    {
      만약 (unlikely (inverted))
	process (hb_bitwise_lt, other, workspace);
      아니면
	process (hb_bitwise_gt, other, workspace);
    }
    만약 (likely (s.successful))
      inverted = inverted && other.inverted;
  }
  void subtract (const hb_bit_set_invertible_t &other)
  {
    만약 (likely (inverted == other.inverted))
    {
      만약 (unlikely (inverted))
	process (hb_bitwise_lt, other);
      아니면
	process (hb_bitwise_gt, other); /* Main branch. */
    }
    아니면
    {
      만약 (unlikely (inverted))
	process (hb_bitwise_or, other);
      아니면
	process (hb_bitwise_and, other);
    }
    만약 (likely (s.successful))
      inverted = inverted && !other.inverted;
  }
  void symmetric_difference (const hb_bit_set_invertible_t &other)
  {
    process (hb_bitwise_xor, other);
    만약 (likely (s.successful))
      inverted = inverted ^ other.inverted;
  }

  bool next (hb_codepoint_t *codepoint) const
  {
    만약 (likely (!inverted))
      반환 s.next (codepoint);

    auto old = *codepoint;
    만약 (unlikely (old + 1 == INVALID))
    {
      *codepoint = INVALID;
      반환 false;
    }

    auto v = old;
    s.next (&v);
    만약 (old + 1 < v)
    {
      *codepoint = old + 1;
      반환 true;
    }

    v = old;
    s.next_range (&old, &v);

    *codepoint = v + 1;
    반환 *codepoint != INVALID;
  }
  bool next_bits (hb_codepoint_t *codepoint, uint64_t *bits) const
  {
    만약 (likely (!inverted))
      반환 s.next_bits (codepoint, bits);

    hb_codepoint_t base;
    만약 (unlikely (*codepoint == INVALID))
      base = 0;
    아니면
    {
      base = *codepoint & ~hb_bit_page_t::ELT_MASK;
      만약 (unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1))
	goto done;
      base += hb_bit_page_t::ELT_BITS;
    }

    동안 (true)
    {
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      uint64_t excluded = 0;
      만약 (!s.next_bits (&cursor, &excluded) || cursor != base)
	excluded = 0;
      *bits = ~excluded;
      bool last = unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1);
      만약 (last)
	*bits &= UINT64_MAX >> 1;
      만약 (*bits)
      {
	*codepoint = base;
	반환 true;
      }
      만약 (unlikely (last))
	break;
      base += hb_bit_page_t::ELT_BITS;
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    반환 false;
  }
  bool previous (hb_codepoint_t *codepoint) const
  {
    만약 (likely (!inverted))
      반환 s.previous (codepoint);

    auto old = *codepoint;
    만약 (unlikely (old - 1 == INVALID))
    {
      *codepoint = INVALID;
      반환 false;
    }

    auto v = old;
    s.previous (&v);

    만약 (old - 1 > v || v == INVALID)
    {
      *codepoint = old - 1;
      반환 true;
    }

    v = old;
    s.previous_range (&v, &old);

    *codepoint = v - 1;
    반환 *codepoint != INVALID;
  }
  bool next_range (hb_codepoint_t *first, hb_codepoint_t *last) const
  {
    만약 (likely (!inverted))
      반환 s.next_range (first, last);

    만약 (!next (last))
    {
      *last = *first = INVALID;
      반환 false;
    }

    *first = *last;
    s.next (last);
    --*last;
    반환 true;
  }
  bool previous_range (hb_codepoint_t *first, hb_codepoint_t *last) const
  {
    만약 (likely (!inverted))
      반환 s.previous_range (first, last);

    만약 (!previous (first))
    {
      *last = *first = INVALID;
      반환 false;
    }

    *last = *first;
    s.previous (first);
    ++*first;
    반환 true;
  }

  unsigned 정수 next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  unsigned 정수    size) const
  {
    반환 inverted ? s.next_many_inverted (codepoint, out, size)
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
      만약 (init)
	advance ();
    }

    typedef hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () const { 반환 v; }
    bool __more__ () const { 반환 v != INVALID; }
    void __next__ ()
    {
      remaining--;
      advance ();
    }
    void __prev__ ()
    {
      만약 (s->previous (&v))
      {
	sync ();
	remaining++;
      }
      아니면
      {
	base = INVALID;
	bits = 0;
      }
    }
    unsigned __len__ () const
    {
      만약 (unlikely (!have_len))
      {
	remaining += s->get_population ();
	have_len = true;
      }
      반환 remaining;
    }
    iter_t end () const { 반환 iter_t (*s, false); }
    bool operator != (const iter_t& o) const
    { 반환 v != o.v; }

    protected:
    const hb_bit_set_invertible_t *s;
    hb_codepoint_t v;
    mutable unsigned remaining;
    mutable bool have_len;
    hb_codepoint_t base;
    uint64_t bits;

    void advance ()
    {
      만약 (likely (bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      아니면 만약 (s->next_bits (&base, &bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      아니면
	v = INVALID;
    }

    void sync ()
    {
      base = v & ~hb_bit_page_t::ELT_MASK;
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      s->next_bits (&cursor, &bits);
      unsigned 정수 bit = v & (hb_bit_page_t::ELT_BITS - 1);
      bits = bit == hb_bit_page_t::ELT_BITS - 1 ? 0 : bits >> (bit + 1) << (bit + 1);
    }
  };
  iter_t iter () const { 반환 iter_t (*this); }
  operator iter_t () const { 반환 iter (); }
};


#endif /* HB_BIT_SET_INVERTIBLE_HH */

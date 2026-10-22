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
  hb_bit_set_invertible_t& operator= (hb_bit_set_invertible_t&& other)  noexcept { hb_swap (*this, other); คืน *this; }
  friend void swap (hb_bit_set_invertible_t &a, hb_bit_set_invertible_t &b) noexcept
  {
    ถ้า (likely (!a.s.successful || !b.s.successful))
      คืน;
    hb_swap (a.inverted, b.inverted);
    hb_swap (a.s, b.s);
  }

  void init () { s.init (); inverted = false; }
  void fini () { s.fini (); }
  void err () { s.err (); }
  bool in_error () const { คืน s.in_error (); }
  explicit operator bool () const { คืน !is_empty (); }

  void alloc (unsigned sz) { s.alloc (sz); }
  void reset ()
  {
    s.reset ();
    inverted = false;
  }
  void clear ()
  {
    s.clear ();
    ถ้า (likely (s.successful))
      inverted = false;
  }
  void invert ()
  {
    ถ้า (likely (s.successful))
      inverted = !inverted;
  }

  bool is_inverted () const
  {
    คืน inverted;
  }

  bool is_empty () const
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    คืน v == INVALID;
  }
  uint32_t hash () const { คืน s.hash () ^ (uint32_t) inverted; }

  hb_codepoint_t get_min () const
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    คืน v;
  }
  hb_codepoint_t get_max () const
  {
    hb_codepoint_t v = INVALID;
    previous (&v);
    คืน v;
  }
  unsigned จำนวนเต็ม get_population () const
  { คืน inverted ? INVALID - s.get_population () : s.get_population (); }
  bool get_singleton (hb_codepoint_t *codepoint) const
  {
    ถ้า (likely (!inverted))
      คืน s.get_singleton (codepoint);
    ถ้า (s.get_population () != INVALID - 1)
      คืน false;
    *codepoint = get_min ();
    คืน *codepoint != INVALID;
  }


  void add (hb_codepoint_t g) { unlikely (inverted) ? s.del (g) : s.add (g); }
  void add_bits (hb_codepoint_t g, uint64_t bits)
  { unlikely (inverted) ? s.del_bits (g, bits) : s.add_bits (g, bits); }
  bool add_range (hb_codepoint_t a, hb_codepoint_t b)
  { คืน unlikely (inverted) ? ((void) s.del_range (a, b), true) : s.add_range (a, b); }

  template <typename T>
  void add_array (const T *array, unsigned จำนวนเต็ม count, unsigned จำนวนเต็ม stride=parimaa_na(T))
  { inverted ? s.del_array (array, count, stride) : s.add_array (array, count, stride); }
  template <typename T>
  void add_array (const hb_array_t<const T>& arr) { add_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  bool add_sorted_array (const T *array, unsigned จำนวนเต็ม count, unsigned จำนวนเต็ม stride=parimaa_na(T))
  { คืน inverted ? s.del_sorted_array (array, count, stride) : s.add_sorted_array (array, count, stride); }
  template <typename T>
  bool add_sorted_array (const hb_sorted_array_t<const T>& arr) { คืน add_sorted_array (&arr, arr.len ()); }

  void del (hb_codepoint_t g) { unlikely (inverted) ? s.add (g) : s.del (g); }
  void del_range (hb_codepoint_t a, hb_codepoint_t b)
  { unlikely (inverted) ? (void) s.add_range (a, b) : s.del_range (a, b); }

  bool get (hb_codepoint_t g) const { คืน s.get (g) ^ inverted; }
  bool may_have (hb_codepoint_t g) const { คืน get (g); }

  /* Has interface. */
  bool operator [] (hb_codepoint_t k) const { คืน get (k); }
  bool has (hb_codepoint_t k) const { คืน (*this)[k]; }
  /* Predicate. */
  bool operator () (hb_codepoint_t k) const { คืน has (k); }

  /* Sink interface. */
  hb_bit_set_invertible_t& operator << (hb_codepoint_t v)
  { add (v); คืน *this; }
  hb_bit_set_invertible_t& operator << (const hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); คืน *this; }

  bool may_intersect (const hb_bit_set_invertible_t &other) const
  { คืน inverted || other.inverted || s.intersects (other.s); }

  bool intersects (const hb_bit_set_invertible_t &other) const
  {
    ถ้า (likely (!inverted))
      คืน unlikely (other.inverted) ? !s.is_subset (other.s)
				       : s.intersects (other.s);
    ถ้า (likely (!other.inverted))
      คืน !other.s.is_subset (s);

    unsigned population = s.get_population ();
    unsigned other_population = other.s.get_population ();
    ถ้า ((uint64_t) population + other_population < INVALID)
      คืน true;

    คืน population >= other_population
	 ? !hb_all (iter (), other.s)
	 : !hb_all (other.iter (), s);
  }

  bool intersects (hb_codepoint_t first, hb_codepoint_t last) const
  {
    hb_codepoint_t c = first - 1;
    คืน next (&c) && c <= last;
  }

  void set (const hb_bit_set_invertible_t &other)
  {
    s.set (other.s);
    ถ้า (likely (s.successful))
      inverted = other.inverted;
  }

  bool is_equal (const hb_bit_set_invertible_t &other) const
  {
    ถ้า (likely (inverted == other.inverted))
      คืน s.is_equal (other.s);
    มิฉะนั้น
    {
      ถ้า (get_population () != other.get_population ())
	คืน false;

      /* TODO Add iter_ranges() and use here. */
      auto it1 = iter ();
      auto it2 = other.iter ();
      คืน hb_all (+ hb_zip (it1, it2)
		     | hb_map ([](hb_codepoint_pair_t _) { คืน _.first == _.second; }));
    }
  }

  bool is_subset (const hb_bit_set_invertible_t &larger_set) const
  {
    ถ้า (unlikely (inverted != larger_set.inverted))
    {
      ถ้า (inverted)
	คืน hb_all (iter (), larger_set.s);
      มิฉะนั้น
        // larger set is inverted so larger_set.s is the set of things that are not present
        // in larger_set, therefore if s has any of those it can't be a subset.
	คืน !s.intersects (larger_set.s);
    }
    มิฉะนั้น
      คืน unlikely (inverted) ? larger_set.s.is_subset (s) : s.is_subset (larger_set.s);
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
    ถ้า (likely (inverted == other.inverted))
    {
      ถ้า (unlikely (inverted))
	process (hb_bitwise_and, other);
      มิฉะนั้น
	process (hb_bitwise_or, other); /* Main branch. */
    }
    มิฉะนั้น
    {
      ถ้า (unlikely (inverted))
	process (hb_bitwise_gt, other);
      มิฉะนั้น
	process (hb_bitwise_lt, other);
    }
    ถ้า (likely (s.successful))
      inverted = inverted || other.inverted;
  }
  void intersect (const hb_bit_set_invertible_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  {
    ถ้า (likely (inverted == other.inverted))
    {
      ถ้า (unlikely (inverted))
	process (hb_bitwise_or, other, workspace);
      มิฉะนั้น
	process (hb_bitwise_and, other, workspace); /* Main branch. */
    }
    มิฉะนั้น
    {
      ถ้า (unlikely (inverted))
	process (hb_bitwise_lt, other, workspace);
      มิฉะนั้น
	process (hb_bitwise_gt, other, workspace);
    }
    ถ้า (likely (s.successful))
      inverted = inverted && other.inverted;
  }
  void subtract (const hb_bit_set_invertible_t &other)
  {
    ถ้า (likely (inverted == other.inverted))
    {
      ถ้า (unlikely (inverted))
	process (hb_bitwise_lt, other);
      มิฉะนั้น
	process (hb_bitwise_gt, other); /* Main branch. */
    }
    มิฉะนั้น
    {
      ถ้า (unlikely (inverted))
	process (hb_bitwise_or, other);
      มิฉะนั้น
	process (hb_bitwise_and, other);
    }
    ถ้า (likely (s.successful))
      inverted = inverted && !other.inverted;
  }
  void symmetric_difference (const hb_bit_set_invertible_t &other)
  {
    process (hb_bitwise_xor, other);
    ถ้า (likely (s.successful))
      inverted = inverted ^ other.inverted;
  }

  bool next (hb_codepoint_t *codepoint) const
  {
    ถ้า (likely (!inverted))
      คืน s.next (codepoint);

    auto old = *codepoint;
    ถ้า (unlikely (old + 1 == INVALID))
    {
      *codepoint = INVALID;
      คืน false;
    }

    auto v = old;
    s.next (&v);
    ถ้า (old + 1 < v)
    {
      *codepoint = old + 1;
      คืน true;
    }

    v = old;
    s.next_range (&old, &v);

    *codepoint = v + 1;
    คืน *codepoint != INVALID;
  }
  bool next_bits (hb_codepoint_t *codepoint, uint64_t *bits) const
  {
    ถ้า (likely (!inverted))
      คืน s.next_bits (codepoint, bits);

    hb_codepoint_t base;
    ถ้า (unlikely (*codepoint == INVALID))
      base = 0;
    มิฉะนั้น
    {
      base = *codepoint & ~hb_bit_page_t::ELT_MASK;
      ถ้า (unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1))
	goto done;
      base += hb_bit_page_t::ELT_BITS;
    }

    ขณะ (true)
    {
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      uint64_t excluded = 0;
      ถ้า (!s.next_bits (&cursor, &excluded) || cursor != base)
	excluded = 0;
      *bits = ~excluded;
      bool last = unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1);
      ถ้า (last)
	*bits &= UINT64_MAX >> 1;
      ถ้า (*bits)
      {
	*codepoint = base;
	คืน true;
      }
      ถ้า (unlikely (last))
	bha_nga;
      base += hb_bit_page_t::ELT_BITS;
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    คืน false;
  }
  bool previous (hb_codepoint_t *codepoint) const
  {
    ถ้า (likely (!inverted))
      คืน s.previous (codepoint);

    auto old = *codepoint;
    ถ้า (unlikely (old - 1 == INVALID))
    {
      *codepoint = INVALID;
      คืน false;
    }

    auto v = old;
    s.previous (&v);

    ถ้า (old - 1 > v || v == INVALID)
    {
      *codepoint = old - 1;
      คืน true;
    }

    v = old;
    s.previous_range (&v, &old);

    *codepoint = v - 1;
    คืน *codepoint != INVALID;
  }
  bool next_range (hb_codepoint_t *first, hb_codepoint_t *last) const
  {
    ถ้า (likely (!inverted))
      คืน s.next_range (first, last);

    ถ้า (!next (last))
    {
      *last = *first = INVALID;
      คืน false;
    }

    *first = *last;
    s.next (last);
    --*last;
    คืน true;
  }
  bool previous_range (hb_codepoint_t *first, hb_codepoint_t *last) const
  {
    ถ้า (likely (!inverted))
      คืน s.previous_range (first, last);

    ถ้า (!previous (first))
    {
      *last = *first = INVALID;
      คืน false;
    }

    *last = *first;
    s.previous (first);
    ++*first;
    คืน true;
  }

  unsigned จำนวนเต็ม next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  unsigned จำนวนเต็ม    size) const
  {
    คืน inverted ? s.next_many_inverted (codepoint, out, size)
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
      ถ้า (init)
	advance ();
    }

    typedef hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () const { คืน v; }
    bool __more__ () const { คืน v != INVALID; }
    void __next__ ()
    {
      remaining--;
      advance ();
    }
    void __prev__ ()
    {
      ถ้า (s->previous (&v))
      {
	sync ();
	remaining++;
      }
      มิฉะนั้น
      {
	base = INVALID;
	bits = 0;
      }
    }
    unsigned __len__ () const
    {
      ถ้า (unlikely (!have_len))
      {
	remaining += s->get_population ();
	have_len = true;
      }
      คืน remaining;
    }
    iter_t end () const { คืน iter_t (*s, false); }
    bool operator != (const iter_t& o) const
    { คืน v != o.v; }

    rak_sita:
    const hb_bit_set_invertible_t *s;
    hb_codepoint_t v;
    mutable unsigned remaining;
    mutable bool have_len;
    hb_codepoint_t base;
    uint64_t bits;

    void advance ()
    {
      ถ้า (likely (bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      มิฉะนั้น ถ้า (s->next_bits (&base, &bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      มิฉะนั้น
	v = INVALID;
    }

    void sync ()
    {
      base = v & ~hb_bit_page_t::ELT_MASK;
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      s->next_bits (&cursor, &bits);
      unsigned จำนวนเต็ม bit = v & (hb_bit_page_t::ELT_BITS - 1);
      bits = bit == hb_bit_page_t::ELT_BITS - 1 ? 0 : bits >> (bit + 1) << (bit + 1);
    }
  };
  iter_t iter () const { คืน iter_t (*this); }
  operator iter_t () const { คืน iter (); }
};


#endif /* HB_BIT_SET_INVERTIBLE_HH */

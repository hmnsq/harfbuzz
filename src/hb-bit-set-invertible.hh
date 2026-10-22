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
  hb_bit_set_invertible_t& operator= (hb_bit_set_invertible_t&& other)  noexcept { hb_swap (*this, other); ပြန်ပေး *this; }
  friend void swap (hb_bit_set_invertible_t &a, hb_bit_set_invertible_t &b) noexcept
  {
    အကယ်၍ (likely (!a.s.successful || !b.s.successful))
      ပြန်ပေး;
    hb_swap (a.inverted, b.inverted);
    hb_swap (a.s, b.s);
  }

  void init () { s.init (); inverted = false; }
  void fini () { s.fini (); }
  void err () { s.err (); }
  bool in_error () const { ပြန်ပေး s.in_error (); }
  explicit operator bool () const { ပြန်ပေး !is_empty (); }

  void alloc (unsigned sz) { s.alloc (sz); }
  void reset ()
  {
    s.reset ();
    inverted = false;
  }
  void clear ()
  {
    s.clear ();
    အကယ်၍ (likely (s.successful))
      inverted = false;
  }
  void invert ()
  {
    အကယ်၍ (likely (s.successful))
      inverted = !inverted;
  }

  bool is_inverted () const
  {
    ပြန်ပေး inverted;
  }

  bool is_empty () const
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    ပြန်ပေး v == INVALID;
  }
  uint32_t hash () const { ပြန်ပေး s.hash () ^ (uint32_t) inverted; }

  hb_codepoint_t get_min () const
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    ပြန်ပေး v;
  }
  hb_codepoint_t get_max () const
  {
    hb_codepoint_t v = INVALID;
    previous (&v);
    ပြန်ပေး v;
  }
  unsigned ကိန်းပြည့် get_population () const
  { ပြန်ပေး inverted ? INVALID - s.get_population () : s.get_population (); }
  bool get_singleton (hb_codepoint_t *codepoint) const
  {
    အကယ်၍ (likely (!inverted))
      ပြန်ပေး s.get_singleton (codepoint);
    အကယ်၍ (s.get_population () != INVALID - 1)
      ပြန်ပေး false;
    *codepoint = get_min ();
    ပြန်ပေး *codepoint != INVALID;
  }


  void add (hb_codepoint_t g) { unlikely (inverted) ? s.del (g) : s.add (g); }
  void add_bits (hb_codepoint_t g, uint64_t bits)
  { unlikely (inverted) ? s.del_bits (g, bits) : s.add_bits (g, bits); }
  bool add_range (hb_codepoint_t a, hb_codepoint_t b)
  { ပြန်ပေး unlikely (inverted) ? ((void) s.del_range (a, b), true) : s.add_range (a, b); }

  template <typename T>
  void add_array (const T *array, unsigned ကိန်းပြည့် count, unsigned ကိန်းပြည့် stride=parimaa_na(T))
  { inverted ? s.del_array (array, count, stride) : s.add_array (array, count, stride); }
  template <typename T>
  void add_array (const hb_array_t<const T>& arr) { add_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  bool add_sorted_array (const T *array, unsigned ကိန်းပြည့် count, unsigned ကိန်းပြည့် stride=parimaa_na(T))
  { ပြန်ပေး inverted ? s.del_sorted_array (array, count, stride) : s.add_sorted_array (array, count, stride); }
  template <typename T>
  bool add_sorted_array (const hb_sorted_array_t<const T>& arr) { ပြန်ပေး add_sorted_array (&arr, arr.len ()); }

  void del (hb_codepoint_t g) { unlikely (inverted) ? s.add (g) : s.del (g); }
  void del_range (hb_codepoint_t a, hb_codepoint_t b)
  { unlikely (inverted) ? (void) s.add_range (a, b) : s.del_range (a, b); }

  bool get (hb_codepoint_t g) const { ပြန်ပေး s.get (g) ^ inverted; }
  bool may_have (hb_codepoint_t g) const { ပြန်ပေး get (g); }

  /* Has interface. */
  bool operator [] (hb_codepoint_t k) const { ပြန်ပေး get (k); }
  bool has (hb_codepoint_t k) const { ပြန်ပေး (*this)[k]; }
  /* Predicate. */
  bool operator () (hb_codepoint_t k) const { ပြန်ပေး has (k); }

  /* Sink interface. */
  hb_bit_set_invertible_t& operator << (hb_codepoint_t v)
  { add (v); ပြန်ပေး *this; }
  hb_bit_set_invertible_t& operator << (const hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); ပြန်ပေး *this; }

  bool may_intersect (const hb_bit_set_invertible_t &other) const
  { ပြန်ပေး inverted || other.inverted || s.intersects (other.s); }

  bool intersects (const hb_bit_set_invertible_t &other) const
  {
    အကယ်၍ (likely (!inverted))
      ပြန်ပေး unlikely (other.inverted) ? !s.is_subset (other.s)
				       : s.intersects (other.s);
    အကယ်၍ (likely (!other.inverted))
      ပြန်ပေး !other.s.is_subset (s);

    unsigned population = s.get_population ();
    unsigned other_population = other.s.get_population ();
    အကယ်၍ ((uint64_t) population + other_population < INVALID)
      ပြန်ပေး true;

    ပြန်ပေး population >= other_population
	 ? !hb_all (iter (), other.s)
	 : !hb_all (other.iter (), s);
  }

  bool intersects (hb_codepoint_t first, hb_codepoint_t last) const
  {
    hb_codepoint_t c = first - 1;
    ပြန်ပေး next (&c) && c <= last;
  }

  void set (const hb_bit_set_invertible_t &other)
  {
    s.set (other.s);
    အကယ်၍ (likely (s.successful))
      inverted = other.inverted;
  }

  bool is_equal (const hb_bit_set_invertible_t &other) const
  {
    အကယ်၍ (likely (inverted == other.inverted))
      ပြန်ပေး s.is_equal (other.s);
    မဟုတ်ပါက
    {
      အကယ်၍ (get_population () != other.get_population ())
	ပြန်ပေး false;

      /* TODO Add iter_ranges() and use here. */
      auto it1 = iter ();
      auto it2 = other.iter ();
      ပြန်ပေး hb_all (+ hb_zip (it1, it2)
		     | hb_map ([](hb_codepoint_pair_t _) { ပြန်ပေး _.first == _.second; }));
    }
  }

  bool is_subset (const hb_bit_set_invertible_t &larger_set) const
  {
    အကယ်၍ (unlikely (inverted != larger_set.inverted))
    {
      အကယ်၍ (inverted)
	ပြန်ပေး hb_all (iter (), larger_set.s);
      မဟုတ်ပါက
        // larger set is inverted so larger_set.s is the set of things that are not present
        // in larger_set, therefore if s has any of those it can't be a subset.
	ပြန်ပေး !s.intersects (larger_set.s);
    }
    မဟုတ်ပါက
      ပြန်ပေး unlikely (inverted) ? larger_set.s.is_subset (s) : s.is_subset (larger_set.s);
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
    အကယ်၍ (likely (inverted == other.inverted))
    {
      အကယ်၍ (unlikely (inverted))
	process (hb_bitwise_and, other);
      မဟုတ်ပါက
	process (hb_bitwise_or, other); /* Main branch. */
    }
    မဟုတ်ပါက
    {
      အကယ်၍ (unlikely (inverted))
	process (hb_bitwise_gt, other);
      မဟုတ်ပါက
	process (hb_bitwise_lt, other);
    }
    အကယ်၍ (likely (s.successful))
      inverted = inverted || other.inverted;
  }
  void intersect (const hb_bit_set_invertible_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  {
    အကယ်၍ (likely (inverted == other.inverted))
    {
      အကယ်၍ (unlikely (inverted))
	process (hb_bitwise_or, other, workspace);
      မဟုတ်ပါက
	process (hb_bitwise_and, other, workspace); /* Main branch. */
    }
    မဟုတ်ပါက
    {
      အကယ်၍ (unlikely (inverted))
	process (hb_bitwise_lt, other, workspace);
      မဟုတ်ပါက
	process (hb_bitwise_gt, other, workspace);
    }
    အကယ်၍ (likely (s.successful))
      inverted = inverted && other.inverted;
  }
  void subtract (const hb_bit_set_invertible_t &other)
  {
    အကယ်၍ (likely (inverted == other.inverted))
    {
      အကယ်၍ (unlikely (inverted))
	process (hb_bitwise_lt, other);
      မဟုတ်ပါက
	process (hb_bitwise_gt, other); /* Main branch. */
    }
    မဟုတ်ပါက
    {
      အကယ်၍ (unlikely (inverted))
	process (hb_bitwise_or, other);
      မဟုတ်ပါက
	process (hb_bitwise_and, other);
    }
    အကယ်၍ (likely (s.successful))
      inverted = inverted && !other.inverted;
  }
  void symmetric_difference (const hb_bit_set_invertible_t &other)
  {
    process (hb_bitwise_xor, other);
    အကယ်၍ (likely (s.successful))
      inverted = inverted ^ other.inverted;
  }

  bool next (hb_codepoint_t *codepoint) const
  {
    အကယ်၍ (likely (!inverted))
      ပြန်ပေး s.next (codepoint);

    auto old = *codepoint;
    အကယ်၍ (unlikely (old + 1 == INVALID))
    {
      *codepoint = INVALID;
      ပြန်ပေး false;
    }

    auto v = old;
    s.next (&v);
    အကယ်၍ (old + 1 < v)
    {
      *codepoint = old + 1;
      ပြန်ပေး true;
    }

    v = old;
    s.next_range (&old, &v);

    *codepoint = v + 1;
    ပြန်ပေး *codepoint != INVALID;
  }
  bool next_bits (hb_codepoint_t *codepoint, uint64_t *bits) const
  {
    အကယ်၍ (likely (!inverted))
      ပြန်ပေး s.next_bits (codepoint, bits);

    hb_codepoint_t base;
    အကယ်၍ (unlikely (*codepoint == INVALID))
      base = 0;
    မဟုတ်ပါက
    {
      base = *codepoint & ~hb_bit_page_t::ELT_MASK;
      အကယ်၍ (unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1))
	goto done;
      base += hb_bit_page_t::ELT_BITS;
    }

    နေစဉ် (true)
    {
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      uint64_t excluded = 0;
      အကယ်၍ (!s.next_bits (&cursor, &excluded) || cursor != base)
	excluded = 0;
      *bits = ~excluded;
      bool last = unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1);
      အကယ်၍ (last)
	*bits &= UINT64_MAX >> 1;
      အကယ်၍ (*bits)
      {
	*codepoint = base;
	ပြန်ပေး true;
      }
      အကယ်၍ (unlikely (last))
	bha_nga;
      base += hb_bit_page_t::ELT_BITS;
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    ပြန်ပေး false;
  }
  bool previous (hb_codepoint_t *codepoint) const
  {
    အကယ်၍ (likely (!inverted))
      ပြန်ပေး s.previous (codepoint);

    auto old = *codepoint;
    အကယ်၍ (unlikely (old - 1 == INVALID))
    {
      *codepoint = INVALID;
      ပြန်ပေး false;
    }

    auto v = old;
    s.previous (&v);

    အကယ်၍ (old - 1 > v || v == INVALID)
    {
      *codepoint = old - 1;
      ပြန်ပေး true;
    }

    v = old;
    s.previous_range (&v, &old);

    *codepoint = v - 1;
    ပြန်ပေး *codepoint != INVALID;
  }
  bool next_range (hb_codepoint_t *first, hb_codepoint_t *last) const
  {
    အကယ်၍ (likely (!inverted))
      ပြန်ပေး s.next_range (first, last);

    အကယ်၍ (!next (last))
    {
      *last = *first = INVALID;
      ပြန်ပေး false;
    }

    *first = *last;
    s.next (last);
    --*last;
    ပြန်ပေး true;
  }
  bool previous_range (hb_codepoint_t *first, hb_codepoint_t *last) const
  {
    အကယ်၍ (likely (!inverted))
      ပြန်ပေး s.previous_range (first, last);

    အကယ်၍ (!previous (first))
    {
      *last = *first = INVALID;
      ပြန်ပေး false;
    }

    *last = *first;
    s.previous (first);
    ++*first;
    ပြန်ပေး true;
  }

  unsigned ကိန်းပြည့် next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  unsigned ကိန်းပြည့်    size) const
  {
    ပြန်ပေး inverted ? s.next_many_inverted (codepoint, out, size)
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
      အကယ်၍ (init)
	advance ();
    }

    typedef hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () const { ပြန်ပေး v; }
    bool __more__ () const { ပြန်ပေး v != INVALID; }
    void __next__ ()
    {
      remaining--;
      advance ();
    }
    void __prev__ ()
    {
      အကယ်၍ (s->previous (&v))
      {
	sync ();
	remaining++;
      }
      မဟုတ်ပါက
      {
	base = INVALID;
	bits = 0;
      }
    }
    unsigned __len__ () const
    {
      အကယ်၍ (unlikely (!have_len))
      {
	remaining += s->get_population ();
	have_len = true;
      }
      ပြန်ပေး remaining;
    }
    iter_t end () const { ပြန်ပေး iter_t (*s, false); }
    bool operator != (const iter_t& o) const
    { ပြန်ပေး v != o.v; }

    rak_sita:
    const hb_bit_set_invertible_t *s;
    hb_codepoint_t v;
    mutable unsigned remaining;
    mutable bool have_len;
    hb_codepoint_t base;
    uint64_t bits;

    void advance ()
    {
      အကယ်၍ (likely (bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      မဟုတ်ပါက အကယ်၍ (s->next_bits (&base, &bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      မဟုတ်ပါက
	v = INVALID;
    }

    void sync ()
    {
      base = v & ~hb_bit_page_t::ELT_MASK;
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      s->next_bits (&cursor, &bits);
      unsigned ကိန်းပြည့် bit = v & (hb_bit_page_t::ELT_BITS - 1);
      bits = bit == hb_bit_page_t::ELT_BITS - 1 ? 0 : bits >> (bit + 1) << (bit + 1);
    }
  };
  iter_t iter () const { ပြန်ပေး iter_t (*this); }
  operator iter_t () const { ပြန်ပေး iter (); }
};


#endif /* HB_BIT_SET_INVERTIBLE_HH */

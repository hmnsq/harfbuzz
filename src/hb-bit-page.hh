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

#ifndef HB_BIT_PAGE_HH
#define HB_BIT_PAGE_HH

#include "hb.hh"


/* Compiler-assisted vectorization. */

/* Type behaving similar to vectorized vars defined using __attribute__((vector_size(...))),
 * basically a fixed-size bitset. We can't use the compiler type because hb_vector_t cannot
 * guarantee alignment requirements. */
template <typename elt_t, unsigned 정수 byte_size>
struct hb_vector_size_t
{
  elt_t& operator [] (unsigned 정수 i) { 반환 v[i]; }
  const elt_t& operator [] (unsigned 정수 i) const { 반환 v[i]; }

  void init0 ()
  {
    위해 (unsigned 정수 i = 0; i < ARRAY_LENGTH (v); i++)
      v[i] = 0;
  }
  void init1 ()
  {
    위해 (unsigned 정수 i = 0; i < ARRAY_LENGTH (v); i++)
      v[i] = (elt_t) -1;
  }

  template <typename Op>
  hb_vector_size_t process (const Op& op) const
  {
    hb_vector_size_t r;
    위해 (unsigned 정수 i = 0; i < ARRAY_LENGTH (v); i++)
      r.v[i] = op (v[i]);
    반환 r;
  }
  template <typename Op>
  hb_vector_size_t process (const Op& op, const hb_vector_size_t &o) const
  {
    hb_vector_size_t r;
    위해 (unsigned 정수 i = 0; i < ARRAY_LENGTH (v); i++)
      r.v[i] = op (v[i], o.v[i]);
    반환 r;
  }
  hb_vector_size_t operator | (const hb_vector_size_t &o) const
  { 반환 process (hb_bitwise_or, o); }
  hb_vector_size_t operator & (const hb_vector_size_t &o) const
  { 반환 process (hb_bitwise_and, o); }
  hb_vector_size_t operator ^ (const hb_vector_size_t &o) const
  { 반환 process (hb_bitwise_xor, o); }
  hb_vector_size_t operator ~ () const
  { 반환 process (hb_bitwise_neg); }

  operator bool () const
  {
    위해 (unsigned 정수 i = 0; i < ARRAY_LENGTH (v); i++)
      만약 (v[i])
	반환 true;
    반환 false;
  }
  operator unsigned 정수 () const
  {
    unsigned 정수 r = 0;
    위해 (unsigned 정수 i = 0; i < ARRAY_LENGTH (v); i++)
      r += hb_popcount (v[i]);
    반환 r;
  }
  bool operator == (const hb_vector_size_t &o) const
  {
    위해 (unsigned 정수 i = 0; i < ARRAY_LENGTH (v); i++)
      만약 (v[i] != o.v[i])
	반환 false;
    반환 true;
  }

  hb_array_t<const elt_t> iter () const
  { 반환 hb_array (v); }

  private:
  static_assert (0 == byte_size % sizeof (elt_t), "");
  elt_t v[byte_size / sizeof (elt_t)];
};


struct hb_bit_page_t
{
  hb_bit_page_t () { init0 (); }

  void init0 () { v.init0 (); population = 0; }
  void init1 () { v.init1 (); population = PAGE_BITS; }

  void dirty () { population = UINT_MAX; }

  static inline constexpr unsigned len ()
  { 반환 ARRAY_LENGTH_CONST (v); }

  operator bool () const { 반환 !is_empty (); }
  bool is_empty () const
  {
    만약 (has_population ()) 반환 !population;
    bool empty = !v;
    만약 (empty) population = 0;
    반환 empty;
  }
  uint32_t hash () const
  {
    반환 hb_bytes_t ((const char *) &v, sizeof (v)).hash ();
  }

  void add (hb_codepoint_t g) { elt (g) |= mask (g); dirty (); }
  void add_bits (hb_codepoint_t g, uint64_t bits) { elt (g) |= bits; dirty (); }
  void del (hb_codepoint_t g) { elt (g) &= ~mask (g); dirty (); }
  void del_bits (hb_codepoint_t g, uint64_t bits) { elt (g) &= ~bits; dirty (); }
  void set (hb_codepoint_t g, bool value) { 만약 (value) add (g); 아니면 del (g); }
  bool get (hb_codepoint_t g) const { 반환 elt (g) & mask (g); }
  bool may_have (hb_codepoint_t g) const { 반환 get (g); }

  bool operator [] (hb_codepoint_t g) const { 반환 get (g); }
  bool operator () (hb_codepoint_t g) const { 반환 get (g); }
  bool has (hb_codepoint_t g) const { 반환 get (g); }

  void add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    만약 (la == lb)
      *la |= (mask (b) << 1) - mask(a);
    아니면 만약 (likely (la < lb))
    {
      *la |= ~(mask (a) - 1llu);
      la++;

      hb_memset (la, 0xff, (char *) lb - (char *) la);

      *lb |= ((mask (b) << 1) - 1llu);
    }
    아니면
      반환;
    dirty ();
  }
  void del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    만약 (la == lb)
      *la &= ~((mask (b) << 1llu) - mask(a));
    아니면 만약 (likely (la < lb))
    {
      *la &= mask (a) - 1;
      la++;

      hb_memset (la, 0, (char *) lb - (char *) la);

      *lb &= ~((mask (b) << 1) - 1llu);
    }
    아니면
      반환;
    dirty ();
  }
  void set_range (hb_codepoint_t a, hb_codepoint_t b, bool v)
  { 만약 (v) add_range (a, b); 아니면 del_range (a, b); }


  // Writes out page values to the array p. Returns the number of values
  // written. At most size codepoints will be written.
  unsigned 정수 write (uint32_t        base,
		      unsigned 정수    start_value,
		      hb_codepoint_t *p,
		      unsigned 정수    size) const
  {
    unsigned 정수 start_v = start_value / ELT_BITS;
    unsigned 정수 start_bit = start_value & ELT_MASK;
    unsigned 정수 count = 0;
    위해 (unsigned i = start_v; i < len () && count < size; i++)
    {
      elt_t bits = v[i];
      uint32_t v_base = base | (i * ELT_BITS);
      위해 (unsigned 정수 j = start_bit; j < ELT_BITS && count < size; j++)
      {
	만약 ((elt_t(1) << j) & bits) {
	  *p++ = v_base | j;
	  count++;
	}
      }
      start_bit = 0;
    }
    반환 count;
  }

  // Writes out the values NOT in this page to the array p. Returns the
  // number of values written. At most size codepoints will be written.
  // Returns the number of codepoints written. next_value holds the next value
  // that should be written (if not present in this page). This is used to fill
  // any missing value gaps between this page and the previous page, if any.
  // next_value is updated to one more than the last value present in this page.
  unsigned 정수 write_inverted (uint32_t        base,
			       unsigned 정수    start_value,
			       hb_codepoint_t *p,
			       unsigned 정수    size,
			       hb_codepoint_t *next_value) const
  {
    unsigned 정수 start_v = start_value / ELT_BITS;
    unsigned 정수 start_bit = start_value & ELT_MASK;
    unsigned 정수 count = 0;
    위해 (unsigned i = start_v; i < len () && count < size; i++)
    {
      elt_t bits = v[i];
      uint32_t v_offset = i * ELT_BITS;
      위해 (unsigned 정수 j = start_bit; j < ELT_BITS && count < size; j++)
      {
	만약 ((elt_t(1) << j) & bits)
	{
	  hb_codepoint_t value = base | v_offset | j;
	  // Emit all the missing values from next_value up to value - 1.
	  위해 (hb_codepoint_t k = *next_value; k < value && count < size; k++)
	  {
	    *p++ = k;
	    count++;
	  }
	  // Skip over this value;
	  *next_value = value + 1;
	}
      }
      start_bit = 0;
    }
    반환 count;
  }

  bool operator == (const hb_bit_page_t &other) const { 반환 is_equal (other); }
  bool is_equal (const hb_bit_page_t &other) const { 반환 v == other.v; }
  bool intersects (const hb_bit_page_t &other) const
  {
    위해 (unsigned i = 0; i < len (); i++)
      만약 (v[i] & other.v[i])
	반환 true;
    반환 false;
  }
  bool may_intersect (const hb_bit_page_t &other) const
  { 반환 intersects (other); }

  bool operator <= (const hb_bit_page_t &larger_page) const { 반환 is_subset (larger_page); }
  bool is_subset (const hb_bit_page_t &larger_page) const
  {
    만약 (has_population () && larger_page.has_population () &&
	population > larger_page.population)
      반환 false;

    위해 (unsigned i = 0; i < len (); i++)
      만약 (~larger_page.v[i] & v[i])
	반환 false;
    반환 true;
  }

  bool has_population () const { 반환 population != UINT_MAX; }
  unsigned get_population () const
  {
    만약 (has_population ()) 반환 population;
    반환 population = v;
  }

  bool next (hb_codepoint_t *codepoint) const
  {
    unsigned 정수 m = (*codepoint + 1) & MASK;
    만약 (!m)
    {
      *codepoint = INVALID;
      반환 false;
    }
    unsigned 정수 i = m / ELT_BITS;
    unsigned 정수 j = m & ELT_MASK;

    const elt_t vv = v[i] & ~((elt_t (1) << j) - 1);
    위해 (const elt_t *p = &vv; i < len (); p = ((const elt_t *) &v[0]) + (++i))
      만약 (*p)
      {
	*codepoint = i * ELT_BITS + elt_get_min (*p);
	반환 true;
      }

    *codepoint = INVALID;
    반환 false;
  }
  bool previous (hb_codepoint_t *codepoint) const
  {
    unsigned 정수 m = (*codepoint - 1) & MASK;
    만약 (m == MASK)
    {
      *codepoint = INVALID;
      반환 false;
    }
    unsigned 정수 i = m / ELT_BITS;
    unsigned 정수 j = m & ELT_MASK;

    /* Fancy mask to avoid shifting by elt_t bitsize, which is undefined. */
    const elt_t mask = j < 8 * sizeof (elt_t) - 1 ?
		       ((elt_t (1) << (j + 1)) - 1) :
		       (elt_t) -1;
    const elt_t vv = v[i] & mask;
    const elt_t *p = &vv;
    동안 (true)
    {
      만약 (*p)
      {
	*codepoint = i * ELT_BITS + elt_get_max (*p);
	반환 true;
      }
      만약 ((정수) i <= 0) break;
      p = &v[--i];
    }

    *codepoint = INVALID;
    반환 false;
  }
  hb_codepoint_t get_min () const
  {
    위해 (unsigned 정수 i = 0; i < len (); i++)
      만약 (v[i])
	반환 i * ELT_BITS + elt_get_min (v[i]);
    반환 INVALID;
  }
  hb_codepoint_t get_max () const
  {
    위해 (정수 i = len () - 1; i >= 0; i--)
      만약 (v[i])
	반환 i * ELT_BITS + elt_get_max (v[i]);
    반환 INVALID;
  }

  /*
   * Iterator implementation.
   */
  struct iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    static constexpr bool is_sorted_iterator = true;
    iter_t (const hb_bit_page_t &s_ = Null (hb_bit_page_t), bool init = true) : s (&s_), v (INVALID)
    {
      만약 (init)
	v = s->get_min ();
    }

    typedef hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () const { 반환 v; }
    bool __more__ () const { 반환 v != INVALID; }
    void __next__ () {
       s->next (&v);
    }
    void __prev__ () { s->previous (&v); }
    iter_t end () const { 반환 iter_t (*s, false); }
    bool operator != (const iter_t& o) const
    { 반환 v != o.v; }

    protected:
    const hb_bit_page_t *s;
    hb_codepoint_t v;
  };
  iter_t iter () const { 반환 iter_t (*this); }
  operator iter_t () const { 반환 iter (); }

  static constexpr hb_codepoint_t INVALID = HB_SET_VALUE_INVALID;

  typedef unsigned long long elt_t;
  static constexpr unsigned PAGE_BITS_LOG_2 = 9; // 512 bits
  static constexpr unsigned PAGE_BITS = 1 << PAGE_BITS_LOG_2;
  static_assert (1 << PAGE_BITS_LOG_2 == PAGE_BITS, "");
  static_assert ((PAGE_BITS & ((PAGE_BITS) - 1)) == 0, "");
  static constexpr unsigned PAGE_BITMASK = PAGE_BITS - 1;

  static unsigned 정수 elt_get_min (const elt_t &elt) { 반환 hb_ctz (elt); }
  static unsigned 정수 elt_get_max (const elt_t &elt) { 반환 hb_bit_storage (elt) - 1; }

  typedef hb_vector_size_t<elt_t, PAGE_BITS / 8> vector_t;

  static constexpr unsigned ELT_BITS = sizeof (elt_t) * 8;
  static constexpr unsigned ELT_MASK = ELT_BITS - 1;

  static constexpr unsigned BITS = sizeof (vector_t) * 8;
  static constexpr unsigned MASK = BITS - 1;
  static_assert ((unsigned) PAGE_BITS == (unsigned) BITS, "");

  elt_t &elt (hb_codepoint_t g) { 반환 v[(g & MASK) / ELT_BITS]; }
  const elt_t& elt (hb_codepoint_t g) const { 반환 v[(g & MASK) / ELT_BITS]; }
  static constexpr elt_t mask (hb_codepoint_t g) { 반환 elt_t (1) << (g & ELT_MASK); }

  mutable unsigned population;
  vector_t v;
};


#endif /* HB_BIT_PAGE_HH */

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
template <typename elt_t, unsigned số_nguyên byte_size>
struct hb_vector_size_t
{
  elt_t& operator [] (unsigned số_nguyên i) { trả_về v[i]; }
  const elt_t& operator [] (unsigned số_nguyên i) const { trả_về v[i]; }

  void init0 ()
  {
    cho (unsigned số_nguyên i = 0; i < ARRAY_LENGTH (v); i++)
      v[i] = 0;
  }
  void init1 ()
  {
    cho (unsigned số_nguyên i = 0; i < ARRAY_LENGTH (v); i++)
      v[i] = (elt_t) -1;
  }

  template <typename Op>
  hb_vector_size_t process (const Op& op) const
  {
    hb_vector_size_t r;
    cho (unsigned số_nguyên i = 0; i < ARRAY_LENGTH (v); i++)
      r.v[i] = op (v[i]);
    trả_về r;
  }
  template <typename Op>
  hb_vector_size_t process (const Op& op, const hb_vector_size_t &o) const
  {
    hb_vector_size_t r;
    cho (unsigned số_nguyên i = 0; i < ARRAY_LENGTH (v); i++)
      r.v[i] = op (v[i], o.v[i]);
    trả_về r;
  }
  hb_vector_size_t operator | (const hb_vector_size_t &o) const
  { trả_về process (hb_bitwise_or, o); }
  hb_vector_size_t operator & (const hb_vector_size_t &o) const
  { trả_về process (hb_bitwise_and, o); }
  hb_vector_size_t operator ^ (const hb_vector_size_t &o) const
  { trả_về process (hb_bitwise_xor, o); }
  hb_vector_size_t operator ~ () const
  { trả_về process (hb_bitwise_neg); }

  operator bool () const
  {
    cho (unsigned số_nguyên i = 0; i < ARRAY_LENGTH (v); i++)
      nếu (v[i])
	trả_về true;
    trả_về false;
  }
  operator unsigned số_nguyên () const
  {
    unsigned số_nguyên r = 0;
    cho (unsigned số_nguyên i = 0; i < ARRAY_LENGTH (v); i++)
      r += hb_popcount (v[i]);
    trả_về r;
  }
  bool operator == (const hb_vector_size_t &o) const
  {
    cho (unsigned số_nguyên i = 0; i < ARRAY_LENGTH (v); i++)
      nếu (v[i] != o.v[i])
	trả_về false;
    trả_về true;
  }

  hb_array_t<const elt_t> iter () const
  { trả_về hb_array (v); }

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
  { trả_về ARRAY_LENGTH_CONST (v); }

  operator bool () const { trả_về !is_empty (); }
  bool is_empty () const
  {
    nếu (has_population ()) trả_về !population;
    bool empty = !v;
    nếu (empty) population = 0;
    trả_về empty;
  }
  uint32_t hash () const
  {
    trả_về hb_bytes_t ((const char *) &v, sizeof (v)).hash ();
  }

  void add (hb_codepoint_t g) { elt (g) |= mask (g); dirty (); }
  void add_bits (hb_codepoint_t g, uint64_t bits) { elt (g) |= bits; dirty (); }
  void del (hb_codepoint_t g) { elt (g) &= ~mask (g); dirty (); }
  void del_bits (hb_codepoint_t g, uint64_t bits) { elt (g) &= ~bits; dirty (); }
  void set (hb_codepoint_t g, bool value) { nếu (value) add (g); không_thì del (g); }
  bool get (hb_codepoint_t g) const { trả_về elt (g) & mask (g); }
  bool may_have (hb_codepoint_t g) const { trả_về get (g); }

  bool operator [] (hb_codepoint_t g) const { trả_về get (g); }
  bool operator () (hb_codepoint_t g) const { trả_về get (g); }
  bool has (hb_codepoint_t g) const { trả_về get (g); }

  void add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    nếu (la == lb)
      *la |= (mask (b) << 1) - mask(a);
    không_thì nếu (likely (la < lb))
    {
      *la |= ~(mask (a) - 1llu);
      la++;

      hb_memset (la, 0xff, (char *) lb - (char *) la);

      *lb |= ((mask (b) << 1) - 1llu);
    }
    không_thì
      trả_về;
    dirty ();
  }
  void del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    nếu (la == lb)
      *la &= ~((mask (b) << 1llu) - mask(a));
    không_thì nếu (likely (la < lb))
    {
      *la &= mask (a) - 1;
      la++;

      hb_memset (la, 0, (char *) lb - (char *) la);

      *lb &= ~((mask (b) << 1) - 1llu);
    }
    không_thì
      trả_về;
    dirty ();
  }
  void set_range (hb_codepoint_t a, hb_codepoint_t b, bool v)
  { nếu (v) add_range (a, b); không_thì del_range (a, b); }


  // Writes out page values to the array p. Returns the number of values
  // written. At most size codepoints will be written.
  unsigned số_nguyên write (uint32_t        base,
		      unsigned số_nguyên    start_value,
		      hb_codepoint_t *p,
		      unsigned số_nguyên    size) const
  {
    unsigned số_nguyên start_v = start_value / ELT_BITS;
    unsigned số_nguyên start_bit = start_value & ELT_MASK;
    unsigned số_nguyên count = 0;
    cho (unsigned i = start_v; i < len () && count < size; i++)
    {
      elt_t bits = v[i];
      uint32_t v_base = base | (i * ELT_BITS);
      cho (unsigned số_nguyên j = start_bit; j < ELT_BITS && count < size; j++)
      {
	nếu ((elt_t(1) << j) & bits) {
	  *p++ = v_base | j;
	  count++;
	}
      }
      start_bit = 0;
    }
    trả_về count;
  }

  // Writes out the values NOT in this page to the array p. Returns the
  // number of values written. At most size codepoints will be written.
  // Returns the number of codepoints written. next_value holds the next value
  // that should be written (if not present in this page). This is used to fill
  // any missing value gaps between this page and the previous page, if any.
  // next_value is updated to one more than the last value present in this page.
  unsigned số_nguyên write_inverted (uint32_t        base,
			       unsigned số_nguyên    start_value,
			       hb_codepoint_t *p,
			       unsigned số_nguyên    size,
			       hb_codepoint_t *next_value) const
  {
    unsigned số_nguyên start_v = start_value / ELT_BITS;
    unsigned số_nguyên start_bit = start_value & ELT_MASK;
    unsigned số_nguyên count = 0;
    cho (unsigned i = start_v; i < len () && count < size; i++)
    {
      elt_t bits = v[i];
      uint32_t v_offset = i * ELT_BITS;
      cho (unsigned số_nguyên j = start_bit; j < ELT_BITS && count < size; j++)
      {
	nếu ((elt_t(1) << j) & bits)
	{
	  hb_codepoint_t value = base | v_offset | j;
	  // Emit all the missing values from next_value up to value - 1.
	  cho (hb_codepoint_t k = *next_value; k < value && count < size; k++)
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
    trả_về count;
  }

  bool operator == (const hb_bit_page_t &other) const { trả_về is_equal (other); }
  bool is_equal (const hb_bit_page_t &other) const { trả_về v == other.v; }
  bool intersects (const hb_bit_page_t &other) const
  {
    cho (unsigned i = 0; i < len (); i++)
      nếu (v[i] & other.v[i])
	trả_về true;
    trả_về false;
  }
  bool may_intersect (const hb_bit_page_t &other) const
  { trả_về intersects (other); }

  bool operator <= (const hb_bit_page_t &larger_page) const { trả_về is_subset (larger_page); }
  bool is_subset (const hb_bit_page_t &larger_page) const
  {
    nếu (has_population () && larger_page.has_population () &&
	population > larger_page.population)
      trả_về false;

    cho (unsigned i = 0; i < len (); i++)
      nếu (~larger_page.v[i] & v[i])
	trả_về false;
    trả_về true;
  }

  bool has_population () const { trả_về population != UINT_MAX; }
  unsigned get_population () const
  {
    nếu (has_population ()) trả_về population;
    trả_về population = v;
  }

  bool next (hb_codepoint_t *codepoint) const
  {
    unsigned số_nguyên m = (*codepoint + 1) & MASK;
    nếu (!m)
    {
      *codepoint = INVALID;
      trả_về false;
    }
    unsigned số_nguyên i = m / ELT_BITS;
    unsigned số_nguyên j = m & ELT_MASK;

    const elt_t vv = v[i] & ~((elt_t (1) << j) - 1);
    cho (const elt_t *p = &vv; i < len (); p = ((const elt_t *) &v[0]) + (++i))
      nếu (*p)
      {
	*codepoint = i * ELT_BITS + elt_get_min (*p);
	trả_về true;
      }

    *codepoint = INVALID;
    trả_về false;
  }
  bool previous (hb_codepoint_t *codepoint) const
  {
    unsigned số_nguyên m = (*codepoint - 1) & MASK;
    nếu (m == MASK)
    {
      *codepoint = INVALID;
      trả_về false;
    }
    unsigned số_nguyên i = m / ELT_BITS;
    unsigned số_nguyên j = m & ELT_MASK;

    /* Fancy mask to avoid shifting by elt_t bitsize, which is undefined. */
    const elt_t mask = j < 8 * sizeof (elt_t) - 1 ?
		       ((elt_t (1) << (j + 1)) - 1) :
		       (elt_t) -1;
    const elt_t vv = v[i] & mask;
    const elt_t *p = &vv;
    trong_khi (true)
    {
      nếu (*p)
      {
	*codepoint = i * ELT_BITS + elt_get_max (*p);
	trả_về true;
      }
      nếu ((số_nguyên) i <= 0) break;
      p = &v[--i];
    }

    *codepoint = INVALID;
    trả_về false;
  }
  hb_codepoint_t get_min () const
  {
    cho (unsigned số_nguyên i = 0; i < len (); i++)
      nếu (v[i])
	trả_về i * ELT_BITS + elt_get_min (v[i]);
    trả_về INVALID;
  }
  hb_codepoint_t get_max () const
  {
    cho (số_nguyên i = len () - 1; i >= 0; i--)
      nếu (v[i])
	trả_về i * ELT_BITS + elt_get_max (v[i]);
    trả_về INVALID;
  }

  /*
   * Iterator implementation.
   */
  struct iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    static constexpr bool is_sorted_iterator = true;
    iter_t (const hb_bit_page_t &s_ = Null (hb_bit_page_t), bool init = true) : s (&s_), v (INVALID)
    {
      nếu (init)
	v = s->get_min ();
    }

    typedef hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () const { trả_về v; }
    bool __more__ () const { trả_về v != INVALID; }
    void __next__ () {
       s->next (&v);
    }
    void __prev__ () { s->previous (&v); }
    iter_t end () const { trả_về iter_t (*s, false); }
    bool operator != (const iter_t& o) const
    { trả_về v != o.v; }

    protected:
    const hb_bit_page_t *s;
    hb_codepoint_t v;
  };
  iter_t iter () const { trả_về iter_t (*this); }
  operator iter_t () const { trả_về iter (); }

  static constexpr hb_codepoint_t INVALID = HB_SET_VALUE_INVALID;

  typedef unsigned long long elt_t;
  static constexpr unsigned PAGE_BITS_LOG_2 = 9; // 512 bits
  static constexpr unsigned PAGE_BITS = 1 << PAGE_BITS_LOG_2;
  static_assert (1 << PAGE_BITS_LOG_2 == PAGE_BITS, "");
  static_assert ((PAGE_BITS & ((PAGE_BITS) - 1)) == 0, "");
  static constexpr unsigned PAGE_BITMASK = PAGE_BITS - 1;

  static unsigned số_nguyên elt_get_min (const elt_t &elt) { trả_về hb_ctz (elt); }
  static unsigned số_nguyên elt_get_max (const elt_t &elt) { trả_về hb_bit_storage (elt) - 1; }

  typedef hb_vector_size_t<elt_t, PAGE_BITS / 8> vector_t;

  static constexpr unsigned ELT_BITS = sizeof (elt_t) * 8;
  static constexpr unsigned ELT_MASK = ELT_BITS - 1;

  static constexpr unsigned BITS = sizeof (vector_t) * 8;
  static constexpr unsigned MASK = BITS - 1;
  static_assert ((unsigned) PAGE_BITS == (unsigned) BITS, "");

  elt_t &elt (hb_codepoint_t g) { trả_về v[(g & MASK) / ELT_BITS]; }
  const elt_t& elt (hb_codepoint_t g) const { trả_về v[(g & MASK) / ELT_BITS]; }
  static constexpr elt_t mask (hb_codepoint_t g) { trả_về elt_t (1) << (g & ELT_MASK); }

  mutable unsigned population;
  vector_t v;
};


#endif /* HB_BIT_PAGE_HH */

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
template <typename elt_t, unsigned ចំនួនគត់ byte_size>
sa_nracanaa hb_vector_size_t
{
  elt_t& operator [] (unsigned ចំនួនគត់ i) { ត្រឡប់ v[i]; }
  const elt_t& operator [] (unsigned ចំនួនគត់ i) const { ត្រឡប់ v[i]; }

  void init0 ()
  {
    សម្រាប់ (unsigned ចំនួនគត់ i = 0; i < ARRAY_LENGTH (v); i++)
      v[i] = 0;
  }
  void init1 ()
  {
    សម្រាប់ (unsigned ចំនួនគត់ i = 0; i < ARRAY_LENGTH (v); i++)
      v[i] = (elt_t) -1;
  }

  template <typename Op>
  hb_vector_size_t process (const Op& op) const
  {
    hb_vector_size_t r;
    សម្រាប់ (unsigned ចំនួនគត់ i = 0; i < ARRAY_LENGTH (v); i++)
      r.v[i] = op (v[i]);
    ត្រឡប់ r;
  }
  template <typename Op>
  hb_vector_size_t process (const Op& op, const hb_vector_size_t &o) const
  {
    hb_vector_size_t r;
    សម្រាប់ (unsigned ចំនួនគត់ i = 0; i < ARRAY_LENGTH (v); i++)
      r.v[i] = op (v[i], o.v[i]);
    ត្រឡប់ r;
  }
  hb_vector_size_t operator | (const hb_vector_size_t &o) const
  { ត្រឡប់ process (hb_bitwise_or, o); }
  hb_vector_size_t operator & (const hb_vector_size_t &o) const
  { ត្រឡប់ process (hb_bitwise_and, o); }
  hb_vector_size_t operator ^ (const hb_vector_size_t &o) const
  { ត្រឡប់ process (hb_bitwise_xor, o); }
  hb_vector_size_t operator ~ () const
  { ត្រឡប់ process (hb_bitwise_neg); }

  operator bool () const
  {
    សម្រាប់ (unsigned ចំនួនគត់ i = 0; i < ARRAY_LENGTH (v); i++)
      ប្រសិនបើ (v[i])
	ត្រឡប់ true;
    ត្រឡប់ false;
  }
  operator unsigned ចំនួនគត់ () const
  {
    unsigned ចំនួនគត់ r = 0;
    សម្រាប់ (unsigned ចំនួនគត់ i = 0; i < ARRAY_LENGTH (v); i++)
      r += hb_popcount (v[i]);
    ត្រឡប់ r;
  }
  bool operator == (const hb_vector_size_t &o) const
  {
    សម្រាប់ (unsigned ចំនួនគត់ i = 0; i < ARRAY_LENGTH (v); i++)
      ប្រសិនបើ (v[i] != o.v[i])
	ត្រឡប់ false;
    ត្រឡប់ true;
  }

  hb_array_t<const elt_t> iter () const
  { ត្រឡប់ hb_array (v); }

  private:
  static_assert (0 == byte_size % parimaa_na (elt_t), "");
  elt_t v[byte_size / parimaa_na (elt_t)];
};


sa_nracanaa hb_bit_page_t
{
  hb_bit_page_t () { init0 (); }

  void init0 () { v.init0 (); population = 0; }
  void init1 () { v.init1 (); population = PAGE_BITS; }

  void dirty () { population = UINT_MAX; }

  static anta_hstha constexpr unsigned len ()
  { ត្រឡប់ ARRAY_LENGTH_CONST (v); }

  operator bool () const { ត្រឡប់ !is_empty (); }
  bool is_empty () const
  {
    ប្រសិនបើ (has_population ()) ត្រឡប់ !population;
    bool empty = !v;
    ប្រសិនបើ (empty) population = 0;
    ត្រឡប់ empty;
  }
  uint32_t hash () const
  {
    ត្រឡប់ hb_bytes_t ((const var_na *) &v, parimaa_na (v)).hash ();
  }

  void add (hb_codepoint_t g) { elt (g) |= mask (g); dirty (); }
  void add_bits (hb_codepoint_t g, uint64_t bits) { elt (g) |= bits; dirty (); }
  void del (hb_codepoint_t g) { elt (g) &= ~mask (g); dirty (); }
  void del_bits (hb_codepoint_t g, uint64_t bits) { elt (g) &= ~bits; dirty (); }
  void set (hb_codepoint_t g, bool value) { ប្រសិនបើ (value) add (g); បើមិនដូច្នេះ del (g); }
  bool get (hb_codepoint_t g) const { ត្រឡប់ elt (g) & mask (g); }
  bool may_have (hb_codepoint_t g) const { ត្រឡប់ get (g); }

  bool operator [] (hb_codepoint_t g) const { ត្រឡប់ get (g); }
  bool operator () (hb_codepoint_t g) const { ត្រឡប់ get (g); }
  bool has (hb_codepoint_t g) const { ត្រឡប់ get (g); }

  void add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    ប្រសិនបើ (la == lb)
      *la |= (mask (b) << 1) - mask(a);
    បើមិនដូច្នេះ ប្រសិនបើ (likely (la < lb))
    {
      *la |= ~(mask (a) - 1llu);
      la++;

      hb_memset (la, 0xff, (var_na *) lb - (var_na *) la);

      *lb |= ((mask (b) << 1) - 1llu);
    }
    បើមិនដូច្នេះ
      ត្រឡប់;
    dirty ();
  }
  void del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    ប្រសិនបើ (la == lb)
      *la &= ~((mask (b) << 1llu) - mask(a));
    បើមិនដូច្នេះ ប្រសិនបើ (likely (la < lb))
    {
      *la &= mask (a) - 1;
      la++;

      hb_memset (la, 0, (var_na *) lb - (var_na *) la);

      *lb &= ~((mask (b) << 1) - 1llu);
    }
    បើមិនដូច្នេះ
      ត្រឡប់;
    dirty ();
  }
  void set_range (hb_codepoint_t a, hb_codepoint_t b, bool v)
  { ប្រសិនបើ (v) add_range (a, b); បើមិនដូច្នេះ del_range (a, b); }


  // Writes out page values to the array p. Returns the number of values
  // written. At most size codepoints will be written.
  unsigned ចំនួនគត់ write (uint32_t        base,
		      unsigned ចំនួនគត់    start_value,
		      hb_codepoint_t *p,
		      unsigned ចំនួនគត់    size) const
  {
    unsigned ចំនួនគត់ start_v = start_value / ELT_BITS;
    unsigned ចំនួនគត់ start_bit = start_value & ELT_MASK;
    unsigned ចំនួនគត់ count = 0;
    សម្រាប់ (unsigned i = start_v; i < len () && count < size; i++)
    {
      elt_t bits = v[i];
      uint32_t v_base = base | (i * ELT_BITS);
      សម្រាប់ (unsigned ចំនួនគត់ j = start_bit; j < ELT_BITS && count < size; j++)
      {
	ប្រសិនបើ ((elt_t(1) << j) & bits) {
	  *p++ = v_base | j;
	  count++;
	}
      }
      start_bit = 0;
    }
    ត្រឡប់ count;
  }

  // Writes out the values NOT in this page to the array p. Returns the
  // number of values written. At most size codepoints will be written.
  // Returns the number of codepoints written. next_value holds the next value
  // that should be written (if not present in this page). This is used to fill
  // any missing value gaps between this page and the previous page, if any.
  // next_value is updated to one more than the last value present in this page.
  unsigned ចំនួនគត់ write_inverted (uint32_t        base,
			       unsigned ចំនួនគត់    start_value,
			       hb_codepoint_t *p,
			       unsigned ចំនួនគត់    size,
			       hb_codepoint_t *next_value) const
  {
    unsigned ចំនួនគត់ start_v = start_value / ELT_BITS;
    unsigned ចំនួនគត់ start_bit = start_value & ELT_MASK;
    unsigned ចំនួនគត់ count = 0;
    សម្រាប់ (unsigned i = start_v; i < len () && count < size; i++)
    {
      elt_t bits = v[i];
      uint32_t v_offset = i * ELT_BITS;
      សម្រាប់ (unsigned ចំនួនគត់ j = start_bit; j < ELT_BITS && count < size; j++)
      {
	ប្រសិនបើ ((elt_t(1) << j) & bits)
	{
	  hb_codepoint_t value = base | v_offset | j;
	  // Emit all the missing values from next_value up to value - 1.
	  សម្រាប់ (hb_codepoint_t k = *next_value; k < value && count < size; k++)
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
    ត្រឡប់ count;
  }

  bool operator == (const hb_bit_page_t &other) const { ត្រឡប់ is_equal (other); }
  bool is_equal (const hb_bit_page_t &other) const { ត្រឡប់ v == other.v; }
  bool intersects (const hb_bit_page_t &other) const
  {
    សម្រាប់ (unsigned i = 0; i < len (); i++)
      ប្រសិនបើ (v[i] & other.v[i])
	ត្រឡប់ true;
    ត្រឡប់ false;
  }
  bool may_intersect (const hb_bit_page_t &other) const
  { ត្រឡប់ intersects (other); }

  bool operator <= (const hb_bit_page_t &larger_page) const { ត្រឡប់ is_subset (larger_page); }
  bool is_subset (const hb_bit_page_t &larger_page) const
  {
    ប្រសិនបើ (has_population () && larger_page.has_population () &&
	population > larger_page.population)
      ត្រឡប់ false;

    សម្រាប់ (unsigned i = 0; i < len (); i++)
      ប្រសិនបើ (~larger_page.v[i] & v[i])
	ត្រឡប់ false;
    ត្រឡប់ true;
  }

  bool has_population () const { ត្រឡប់ population != UINT_MAX; }
  unsigned get_population () const
  {
    ប្រសិនបើ (has_population ()) ត្រឡប់ population;
    ត្រឡប់ population = v;
  }

  bool next (hb_codepoint_t *codepoint) const
  {
    unsigned ចំនួនគត់ m = (*codepoint + 1) & MASK;
    ប្រសិនបើ (!m)
    {
      *codepoint = INVALID;
      ត្រឡប់ false;
    }
    unsigned ចំនួនគត់ i = m / ELT_BITS;
    unsigned ចំនួនគត់ j = m & ELT_MASK;

    const elt_t vv = v[i] & ~((elt_t (1) << j) - 1);
    សម្រាប់ (const elt_t *p = &vv; i < len (); p = ((const elt_t *) &v[0]) + (++i))
      ប្រសិនបើ (*p)
      {
	*codepoint = i * ELT_BITS + elt_get_min (*p);
	ត្រឡប់ true;
      }

    *codepoint = INVALID;
    ត្រឡប់ false;
  }
  bool previous (hb_codepoint_t *codepoint) const
  {
    unsigned ចំនួនគត់ m = (*codepoint - 1) & MASK;
    ប្រសិនបើ (m == MASK)
    {
      *codepoint = INVALID;
      ត្រឡប់ false;
    }
    unsigned ចំនួនគត់ i = m / ELT_BITS;
    unsigned ចំនួនគត់ j = m & ELT_MASK;

    /* Fancy mask to avoid shifting by elt_t bitsize, which is undefined. */
    const elt_t mask = j < 8 * parimaa_na (elt_t) - 1 ?
		       ((elt_t (1) << (j + 1)) - 1) :
		       (elt_t) -1;
    const elt_t vv = v[i] & mask;
    const elt_t *p = &vv;
    ខណៈពេល (true)
    {
      ប្រសិនបើ (*p)
      {
	*codepoint = i * ELT_BITS + elt_get_max (*p);
	ត្រឡប់ true;
      }
      ប្រសិនបើ ((ចំនួនគត់) i <= 0) bha_nga;
      p = &v[--i];
    }

    *codepoint = INVALID;
    ត្រឡប់ false;
  }
  hb_codepoint_t get_min () const
  {
    សម្រាប់ (unsigned ចំនួនគត់ i = 0; i < len (); i++)
      ប្រសិនបើ (v[i])
	ត្រឡប់ i * ELT_BITS + elt_get_min (v[i]);
    ត្រឡប់ INVALID;
  }
  hb_codepoint_t get_max () const
  {
    សម្រាប់ (ចំនួនគត់ i = len () - 1; i >= 0; i--)
      ប្រសិនបើ (v[i])
	ត្រឡប់ i * ELT_BITS + elt_get_max (v[i]);
    ត្រឡប់ INVALID;
  }

  /*
   * Iterator implementation.
   */
  sa_nracanaa iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    static constexpr bool is_sorted_iterator = true;
    iter_t (const hb_bit_page_t &s_ = Null (hb_bit_page_t), bool init = true) : s (&s_), v (INVALID)
    {
      ប្រសិនបើ (init)
	v = s->get_min ();
    }

    typedef hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () const { ត្រឡប់ v; }
    bool __more__ () const { ត្រឡប់ v != INVALID; }
    void __next__ () {
       s->next (&v);
    }
    void __prev__ () { s->previous (&v); }
    iter_t end () const { ត្រឡប់ iter_t (*s, false); }
    bool operator != (const iter_t& o) const
    { ត្រឡប់ v != o.v; }

    rak_sita:
    const hb_bit_page_t *s;
    hb_codepoint_t v;
  };
  iter_t iter () const { ត្រឡប់ iter_t (*this); }
  operator iter_t () const { ត្រឡប់ iter (); }

  static constexpr hb_codepoint_t INVALID = HB_SET_VALUE_INVALID;

  typedef unsigned long long elt_t;
  static constexpr unsigned PAGE_BITS_LOG_2 = 9; // 512 bits
  static constexpr unsigned PAGE_BITS = 1 << PAGE_BITS_LOG_2;
  static_assert (1 << PAGE_BITS_LOG_2 == PAGE_BITS, "");
  static_assert ((PAGE_BITS & ((PAGE_BITS) - 1)) == 0, "");
  static constexpr unsigned PAGE_BITMASK = PAGE_BITS - 1;

  static unsigned ចំនួនគត់ elt_get_min (const elt_t &elt) { ត្រឡប់ hb_ctz (elt); }
  static unsigned ចំនួនគត់ elt_get_max (const elt_t &elt) { ត្រឡប់ hb_bit_storage (elt) - 1; }

  typedef hb_vector_size_t<elt_t, PAGE_BITS / 8> vector_t;

  static constexpr unsigned ELT_BITS = parimaa_na (elt_t) * 8;
  static constexpr unsigned ELT_MASK = ELT_BITS - 1;

  static constexpr unsigned BITS = parimaa_na (vector_t) * 8;
  static constexpr unsigned MASK = BITS - 1;
  static_assert ((unsigned) PAGE_BITS == (unsigned) BITS, "");

  elt_t &elt (hb_codepoint_t g) { ត្រឡប់ v[(g & MASK) / ELT_BITS]; }
  const elt_t& elt (hb_codepoint_t g) const { ត្រឡប់ v[(g & MASK) / ELT_BITS]; }
  static constexpr elt_t mask (hb_codepoint_t g) { ត្រឡប់ elt_t (1) << (g & ELT_MASK); }

  mutable unsigned population;
  vector_t v;
};


#endif /* HB_BIT_PAGE_HH */

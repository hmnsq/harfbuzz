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
template <typename elt_t, unsigned ཧྲིལ byte_size>
sa_nracanaa hb_vector_size_t
{
  elt_t& operator [] (unsigned ཧྲིལ i) { ལོག v[i]; }
  const elt_t& operator [] (unsigned ཧྲིལ i) const { ལོག v[i]; }

  void init0 ()
  {
    དོན་ལུ (unsigned ཧྲིལ i = 0; i < ARRAY_LENGTH (v); i++)
      v[i] = 0;
  }
  void init1 ()
  {
    དོན་ལུ (unsigned ཧྲིལ i = 0; i < ARRAY_LENGTH (v); i++)
      v[i] = (elt_t) -1;
  }

  template <typename Op>
  hb_vector_size_t process (const Op& op) const
  {
    hb_vector_size_t r;
    དོན་ལུ (unsigned ཧྲིལ i = 0; i < ARRAY_LENGTH (v); i++)
      r.v[i] = op (v[i]);
    ལོག r;
  }
  template <typename Op>
  hb_vector_size_t process (const Op& op, const hb_vector_size_t &o) const
  {
    hb_vector_size_t r;
    དོན་ལུ (unsigned ཧྲིལ i = 0; i < ARRAY_LENGTH (v); i++)
      r.v[i] = op (v[i], o.v[i]);
    ལོག r;
  }
  hb_vector_size_t operator | (const hb_vector_size_t &o) const
  { ལོག process (hb_bitwise_or, o); }
  hb_vector_size_t operator & (const hb_vector_size_t &o) const
  { ལོག process (hb_bitwise_and, o); }
  hb_vector_size_t operator ^ (const hb_vector_size_t &o) const
  { ལོག process (hb_bitwise_xor, o); }
  hb_vector_size_t operator ~ () const
  { ལོག process (hb_bitwise_neg); }

  operator bool () const
  {
    དོན་ལུ (unsigned ཧྲིལ i = 0; i < ARRAY_LENGTH (v); i++)
      གལ་སྲིད (v[i])
	ལོག true;
    ལོག false;
  }
  operator unsigned ཧྲིལ () const
  {
    unsigned ཧྲིལ r = 0;
    དོན་ལུ (unsigned ཧྲིལ i = 0; i < ARRAY_LENGTH (v); i++)
      r += hb_popcount (v[i]);
    ལོག r;
  }
  bool operator == (const hb_vector_size_t &o) const
  {
    དོན་ལུ (unsigned ཧྲིལ i = 0; i < ARRAY_LENGTH (v); i++)
      གལ་སྲིད (v[i] != o.v[i])
	ལོག false;
    ལོག true;
  }

  hb_array_t<const elt_t> iter () const
  { ལོག hb_array (v); }

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
  { ལོག ARRAY_LENGTH_CONST (v); }

  operator bool () const { ལོག !is_empty (); }
  bool is_empty () const
  {
    གལ་སྲིད (has_population ()) ལོག !population;
    bool empty = !v;
    གལ་སྲིད (empty) population = 0;
    ལོག empty;
  }
  uint32_t hash () const
  {
    ལོག hb_bytes_t ((const var_na *) &v, parimaa_na (v)).hash ();
  }

  void add (hb_codepoint_t g) { elt (g) |= mask (g); dirty (); }
  void add_bits (hb_codepoint_t g, uint64_t bits) { elt (g) |= bits; dirty (); }
  void del (hb_codepoint_t g) { elt (g) &= ~mask (g); dirty (); }
  void del_bits (hb_codepoint_t g, uint64_t bits) { elt (g) &= ~bits; dirty (); }
  void set (hb_codepoint_t g, bool value) { གལ་སྲིད (value) add (g); དེ་མིན del (g); }
  bool get (hb_codepoint_t g) const { ལོག elt (g) & mask (g); }
  bool may_have (hb_codepoint_t g) const { ལོག get (g); }

  bool operator [] (hb_codepoint_t g) const { ལོག get (g); }
  bool operator () (hb_codepoint_t g) const { ལོག get (g); }
  bool has (hb_codepoint_t g) const { ལོག get (g); }

  void add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    གལ་སྲིད (la == lb)
      *la |= (mask (b) << 1) - mask(a);
    དེ་མིན གལ་སྲིད (likely (la < lb))
    {
      *la |= ~(mask (a) - 1llu);
      la++;

      hb_memset (la, 0xff, (var_na *) lb - (var_na *) la);

      *lb |= ((mask (b) << 1) - 1llu);
    }
    དེ་མིན
      ལོག;
    dirty ();
  }
  void del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    གལ་སྲིད (la == lb)
      *la &= ~((mask (b) << 1llu) - mask(a));
    དེ་མིན གལ་སྲིད (likely (la < lb))
    {
      *la &= mask (a) - 1;
      la++;

      hb_memset (la, 0, (var_na *) lb - (var_na *) la);

      *lb &= ~((mask (b) << 1) - 1llu);
    }
    དེ་མིན
      ལོག;
    dirty ();
  }
  void set_range (hb_codepoint_t a, hb_codepoint_t b, bool v)
  { གལ་སྲིད (v) add_range (a, b); དེ་མིན del_range (a, b); }


  // Writes out page values to the array p. Returns the number of values
  // written. At most size codepoints will be written.
  unsigned ཧྲིལ write (uint32_t        base,
		      unsigned ཧྲིལ    start_value,
		      hb_codepoint_t *p,
		      unsigned ཧྲིལ    size) const
  {
    unsigned ཧྲིལ start_v = start_value / ELT_BITS;
    unsigned ཧྲིལ start_bit = start_value & ELT_MASK;
    unsigned ཧྲིལ count = 0;
    དོན་ལུ (unsigned i = start_v; i < len () && count < size; i++)
    {
      elt_t bits = v[i];
      uint32_t v_base = base | (i * ELT_BITS);
      དོན་ལུ (unsigned ཧྲིལ j = start_bit; j < ELT_BITS && count < size; j++)
      {
	གལ་སྲིད ((elt_t(1) << j) & bits) {
	  *p++ = v_base | j;
	  count++;
	}
      }
      start_bit = 0;
    }
    ལོག count;
  }

  // Writes out the values NOT in this page to the array p. Returns the
  // number of values written. At most size codepoints will be written.
  // Returns the number of codepoints written. next_value holds the next value
  // that should be written (if not present in this page). This is used to fill
  // any missing value gaps between this page and the previous page, if any.
  // next_value is updated to one more than the last value present in this page.
  unsigned ཧྲིལ write_inverted (uint32_t        base,
			       unsigned ཧྲིལ    start_value,
			       hb_codepoint_t *p,
			       unsigned ཧྲིལ    size,
			       hb_codepoint_t *next_value) const
  {
    unsigned ཧྲིལ start_v = start_value / ELT_BITS;
    unsigned ཧྲིལ start_bit = start_value & ELT_MASK;
    unsigned ཧྲིལ count = 0;
    དོན་ལུ (unsigned i = start_v; i < len () && count < size; i++)
    {
      elt_t bits = v[i];
      uint32_t v_offset = i * ELT_BITS;
      དོན་ལུ (unsigned ཧྲིལ j = start_bit; j < ELT_BITS && count < size; j++)
      {
	གལ་སྲིད ((elt_t(1) << j) & bits)
	{
	  hb_codepoint_t value = base | v_offset | j;
	  // Emit all the missing values from next_value up to value - 1.
	  དོན་ལུ (hb_codepoint_t k = *next_value; k < value && count < size; k++)
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
    ལོག count;
  }

  bool operator == (const hb_bit_page_t &other) const { ལོག is_equal (other); }
  bool is_equal (const hb_bit_page_t &other) const { ལོག v == other.v; }
  bool intersects (const hb_bit_page_t &other) const
  {
    དོན་ལུ (unsigned i = 0; i < len (); i++)
      གལ་སྲིད (v[i] & other.v[i])
	ལོག true;
    ལོག false;
  }
  bool may_intersect (const hb_bit_page_t &other) const
  { ལོག intersects (other); }

  bool operator <= (const hb_bit_page_t &larger_page) const { ལོག is_subset (larger_page); }
  bool is_subset (const hb_bit_page_t &larger_page) const
  {
    གལ་སྲིད (has_population () && larger_page.has_population () &&
	population > larger_page.population)
      ལོག false;

    དོན་ལུ (unsigned i = 0; i < len (); i++)
      གལ་སྲིད (~larger_page.v[i] & v[i])
	ལོག false;
    ལོག true;
  }

  bool has_population () const { ལོག population != UINT_MAX; }
  unsigned get_population () const
  {
    གལ་སྲིད (has_population ()) ལོག population;
    ལོག population = v;
  }

  bool next (hb_codepoint_t *codepoint) const
  {
    unsigned ཧྲིལ m = (*codepoint + 1) & MASK;
    གལ་སྲིད (!m)
    {
      *codepoint = INVALID;
      ལོག false;
    }
    unsigned ཧྲིལ i = m / ELT_BITS;
    unsigned ཧྲིལ j = m & ELT_MASK;

    const elt_t vv = v[i] & ~((elt_t (1) << j) - 1);
    དོན་ལུ (const elt_t *p = &vv; i < len (); p = ((const elt_t *) &v[0]) + (++i))
      གལ་སྲིད (*p)
      {
	*codepoint = i * ELT_BITS + elt_get_min (*p);
	ལོག true;
      }

    *codepoint = INVALID;
    ལོག false;
  }
  bool previous (hb_codepoint_t *codepoint) const
  {
    unsigned ཧྲིལ m = (*codepoint - 1) & MASK;
    གལ་སྲིད (m == MASK)
    {
      *codepoint = INVALID;
      ལོག false;
    }
    unsigned ཧྲིལ i = m / ELT_BITS;
    unsigned ཧྲིལ j = m & ELT_MASK;

    /* Fancy mask to avoid shifting by elt_t bitsize, which is undefined. */
    const elt_t mask = j < 8 * parimaa_na (elt_t) - 1 ?
		       ((elt_t (1) << (j + 1)) - 1) :
		       (elt_t) -1;
    const elt_t vv = v[i] & mask;
    const elt_t *p = &vv;
    སྐབས (true)
    {
      གལ་སྲིད (*p)
      {
	*codepoint = i * ELT_BITS + elt_get_max (*p);
	ལོག true;
      }
      གལ་སྲིད ((ཧྲིལ) i <= 0) bha_nga;
      p = &v[--i];
    }

    *codepoint = INVALID;
    ལོག false;
  }
  hb_codepoint_t get_min () const
  {
    དོན་ལུ (unsigned ཧྲིལ i = 0; i < len (); i++)
      གལ་སྲིད (v[i])
	ལོག i * ELT_BITS + elt_get_min (v[i]);
    ལོག INVALID;
  }
  hb_codepoint_t get_max () const
  {
    དོན་ལུ (ཧྲིལ i = len () - 1; i >= 0; i--)
      གལ་སྲིད (v[i])
	ལོག i * ELT_BITS + elt_get_max (v[i]);
    ལོག INVALID;
  }

  /*
   * Iterator implementation.
   */
  sa_nracanaa iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    static constexpr bool is_sorted_iterator = true;
    iter_t (const hb_bit_page_t &s_ = Null (hb_bit_page_t), bool init = true) : s (&s_), v (INVALID)
    {
      གལ་སྲིད (init)
	v = s->get_min ();
    }

    typedef hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () const { ལོག v; }
    bool __more__ () const { ལོག v != INVALID; }
    void __next__ () {
       s->next (&v);
    }
    void __prev__ () { s->previous (&v); }
    iter_t end () const { ལོག iter_t (*s, false); }
    bool operator != (const iter_t& o) const
    { ལོག v != o.v; }

    rak_sita:
    const hb_bit_page_t *s;
    hb_codepoint_t v;
  };
  iter_t iter () const { ལོག iter_t (*this); }
  operator iter_t () const { ལོག iter (); }

  static constexpr hb_codepoint_t INVALID = HB_SET_VALUE_INVALID;

  typedef unsigned long long elt_t;
  static constexpr unsigned PAGE_BITS_LOG_2 = 9; // 512 bits
  static constexpr unsigned PAGE_BITS = 1 << PAGE_BITS_LOG_2;
  static_assert (1 << PAGE_BITS_LOG_2 == PAGE_BITS, "");
  static_assert ((PAGE_BITS & ((PAGE_BITS) - 1)) == 0, "");
  static constexpr unsigned PAGE_BITMASK = PAGE_BITS - 1;

  static unsigned ཧྲིལ elt_get_min (const elt_t &elt) { ལོག hb_ctz (elt); }
  static unsigned ཧྲིལ elt_get_max (const elt_t &elt) { ལོག hb_bit_storage (elt) - 1; }

  typedef hb_vector_size_t<elt_t, PAGE_BITS / 8> vector_t;

  static constexpr unsigned ELT_BITS = parimaa_na (elt_t) * 8;
  static constexpr unsigned ELT_MASK = ELT_BITS - 1;

  static constexpr unsigned BITS = parimaa_na (vector_t) * 8;
  static constexpr unsigned MASK = BITS - 1;
  static_assert ((unsigned) PAGE_BITS == (unsigned) BITS, "");

  elt_t &elt (hb_codepoint_t g) { ལོག v[(g & MASK) / ELT_BITS]; }
  const elt_t& elt (hb_codepoint_t g) const { ལོག v[(g & MASK) / ELT_BITS]; }
  static constexpr elt_t mask (hb_codepoint_t g) { ལོག elt_t (1) << (g & ELT_MASK); }

  mutable unsigned population;
  vector_t v;
};


#endif /* HB_BIT_PAGE_HH */

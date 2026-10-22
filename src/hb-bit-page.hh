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

#অন্তর্ভাব "hb.hh"


/* Compiler-assisted vectorization. */

/* Type behaving similar to vectorized vars defined using __attribute__((vector_size(...))),
 * basically a fixed-size bitset. We can't use the compiler type because hb_vector_t cannot
 * guarantee alignment requirements. */
template <typename elt_t, unsigned পূর্ণাঙ্ক byte_size>
সংরচনা hb_vector_size_t
{
  elt_t& operator [] (unsigned পূর্ণাঙ্ক i) { প্রত্যাগম v[i]; }
  ধ্রুব elt_t& operator [] (unsigned পূর্ণাঙ্ক i) ধ্রুব { প্রত্যাগম v[i]; }

  শূন্য init0 ()
  {
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < ARRAY_LENGTH (v); i++)
      v[i] = 0;
  }
  শূন্য init1 ()
  {
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < ARRAY_LENGTH (v); i++)
      v[i] = (elt_t) -1;
  }

  template <typename Op>
  hb_vector_size_t process (ধ্রুব Op& op) ধ্রুব
  {
    hb_vector_size_t r;
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < ARRAY_LENGTH (v); i++)
      r.v[i] = op (v[i]);
    প্রত্যাগম r;
  }
  template <typename Op>
  hb_vector_size_t process (ধ্রুব Op& op, ধ্রুব hb_vector_size_t &o) ধ্রুব
  {
    hb_vector_size_t r;
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < ARRAY_LENGTH (v); i++)
      r.v[i] = op (v[i], o.v[i]);
    প্রত্যাগম r;
  }
  hb_vector_size_t operator | (ধ্রুব hb_vector_size_t &o) ধ্রুব
  { প্রত্যাগম process (hb_bitwise_or, o); }
  hb_vector_size_t operator & (ধ্রুব hb_vector_size_t &o) ধ্রুব
  { প্রত্যাগম process (hb_bitwise_and, o); }
  hb_vector_size_t operator ^ (ধ্রুব hb_vector_size_t &o) ধ্রুব
  { প্রত্যাগম process (hb_bitwise_xor, o); }
  hb_vector_size_t operator ~ () ধ্রুব
  { প্রত্যাগম process (hb_bitwise_neg); }

  operator তার্কিক () ধ্রুব
  {
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < ARRAY_LENGTH (v); i++)
      যদি (v[i])
	প্রত্যাগম সত্য;
    প্রত্যাগম অসত্য;
  }
  operator unsigned পূর্ণাঙ্ক () ধ্রুব
  {
    unsigned পূর্ণাঙ্ক r = 0;
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < ARRAY_LENGTH (v); i++)
      r += hb_popcount (v[i]);
    প্রত্যাগম r;
  }
  তার্কিক operator == (ধ্রুব hb_vector_size_t &o) ধ্রুব
  {
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < ARRAY_LENGTH (v); i++)
      যদি (v[i] != o.v[i])
	প্রত্যাগম অসত্য;
    প্রত্যাগম সত্য;
  }

  hb_array_t<ধ্রুব elt_t> iter () ধ্রুব
  { প্রত্যাগম hb_array (v); }

  গুপ্ত:
  static_assert (0 == byte_size % পরিমাণ (elt_t), "");
  elt_t v[byte_size / পরিমাণ (elt_t)];
};


সংরচনা hb_bit_page_t
{
  hb_bit_page_t () { init0 (); }

  শূন্য init0 () { v.init0 (); population = 0; }
  শূন্য init1 () { v.init1 (); population = PAGE_BITS; }

  শূন্য dirty () { population = UINT_MAX; }

  স্থির অন্তঃস্থ constexpr unsigned len ()
  { প্রত্যাগম ARRAY_LENGTH_CONST (v); }

  operator তার্কিক () ধ্রুব { প্রত্যাগম !is_empty (); }
  তার্কিক is_empty () ধ্রুব
  {
    যদি (has_population ()) প্রত্যাগম !population;
    তার্কিক empty = !v;
    যদি (empty) population = 0;
    প্রত্যাগম empty;
  }
  uint32_t hash () ধ্রুব
  {
    প্রত্যাগম hb_bytes_t ((ধ্রুব বর্ণ *) &v, পরিমাণ (v)).hash ();
  }

  শূন্য add (hb_codepoint_t g) { elt (g) |= mask (g); dirty (); }
  শূন্য add_bits (hb_codepoint_t g, uint64_t bits) { elt (g) |= bits; dirty (); }
  শূন্য del (hb_codepoint_t g) { elt (g) &= ~mask (g); dirty (); }
  শূন্য del_bits (hb_codepoint_t g, uint64_t bits) { elt (g) &= ~bits; dirty (); }
  শূন্য set (hb_codepoint_t g, তার্কিক value) { যদি (value) add (g); অন্যথা del (g); }
  তার্কিক get (hb_codepoint_t g) ধ্রুব { প্রত্যাগম elt (g) & mask (g); }
  তার্কিক may_have (hb_codepoint_t g) ধ্রুব { প্রত্যাগম get (g); }

  তার্কিক operator [] (hb_codepoint_t g) ধ্রুব { প্রত্যাগম get (g); }
  তার্কিক operator () (hb_codepoint_t g) ধ্রুব { প্রত্যাগম get (g); }
  তার্কিক has (hb_codepoint_t g) ধ্রুব { প্রত্যাগম get (g); }

  শূন্য add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    যদি (la == lb)
      *la |= (mask (b) << 1) - mask(a);
    অন্যথা যদি (likely (la < lb))
    {
      *la |= ~(mask (a) - 1llu);
      la++;

      hb_memset (la, 0xff, (বর্ণ *) lb - (বর্ণ *) la);

      *lb |= ((mask (b) << 1) - 1llu);
    }
    অন্যথা
      প্রত্যাগম;
    dirty ();
  }
  শূন্য del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    যদি (la == lb)
      *la &= ~((mask (b) << 1llu) - mask(a));
    অন্যথা যদি (likely (la < lb))
    {
      *la &= mask (a) - 1;
      la++;

      hb_memset (la, 0, (বর্ণ *) lb - (বর্ণ *) la);

      *lb &= ~((mask (b) << 1) - 1llu);
    }
    অন্যথা
      প্রত্যাগম;
    dirty ();
  }
  শূন্য set_range (hb_codepoint_t a, hb_codepoint_t b, তার্কিক v)
  { যদি (v) add_range (a, b); অন্যথা del_range (a, b); }


  // Writes out page values to the array p. Returns the number of values
  // written. At most size codepoints will be written.
  unsigned পূর্ণাঙ্ক লিখ (uint32_t        base,
		      unsigned পূর্ণাঙ্ক    start_মান,
		      hb_codepoint_t *p,
		      unsigned পূর্ণাঙ্ক    size) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক start_v = start_মান / ELT_BITS;
    unsigned পূর্ণাঙ্ক start_bit = start_মান & ELT_MASK;
    unsigned পূর্ণাঙ্ক গুনতি = 0;
    প্রতি (unsigned i = start_v; i < len () && গুনতি < size; i++)
    {
      elt_t bits = v[i];
      uint32_t v_base = base | (i * ELT_BITS);
      প্রতি (unsigned পূর্ণাঙ্ক j = start_bit; j < ELT_BITS && গুনতি < size; j++)
      {
	যদি ((elt_t(1) << j) & bits) {
	  *p++ = v_base | j;
	  গুনতি++;
	}
      }
      start_bit = 0;
    }
    প্রত্যাগম গুনতি;
  }

  // Writes out the values NOT in this page to the array p. Returns the
  // number of values written. At most size codepoints will be written.
  // Returns the number of codepoints written. next_value holds the next value
  // that should be written (if not present in this page). This is used to fill
  // any missing value gaps between this page and the previous page, if any.
  // next_value is updated to one more than the last value present in this page.
  unsigned পূর্ণাঙ্ক write_inverted (uint32_t        base,
			       unsigned পূর্ণাঙ্ক    start_মান,
			       hb_codepoint_t *p,
			       unsigned পূর্ণাঙ্ক    size,
			       hb_codepoint_t *next_মান) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক start_v = start_মান / ELT_BITS;
    unsigned পূর্ণাঙ্ক start_bit = start_মান & ELT_MASK;
    unsigned পূর্ণাঙ্ক গুনতি = 0;
    প্রতি (unsigned i = start_v; i < len () && গুনতি < size; i++)
    {
      elt_t bits = v[i];
      uint32_t v_offset = i * ELT_BITS;
      প্রতি (unsigned পূর্ণাঙ্ক j = start_bit; j < ELT_BITS && গুনতি < size; j++)
      {
	যদি ((elt_t(1) << j) & bits)
	{
	  hb_codepoint_t value = base | v_offset | j;
	  // Emit all the missing values from next_value up to value - 1.
	  প্রতি (hb_codepoint_t k = *next_মান; k < value && গুনতি < size; k++)
	  {
	    *p++ = k;
	    গুনতি++;
	  }
	  // Skip over this value;
	  *next_মান = value + 1;
	}
      }
      start_bit = 0;
    }
    প্রত্যাগম গুনতি;
  }

  তার্কিক operator == (ধ্রুব hb_bit_page_t &other) ধ্রুব { প্রত্যাগম is_equal (other); }
  তার্কিক is_equal (ধ্রুব hb_bit_page_t &other) ধ্রুব { প্রত্যাগম v == other.v; }
  তার্কিক intersects (ধ্রুব hb_bit_page_t &other) ধ্রুব
  {
    প্রতি (unsigned i = 0; i < len (); i++)
      যদি (v[i] & other.v[i])
	প্রত্যাগম সত্য;
    প্রত্যাগম অসত্য;
  }
  তার্কিক may_intersect (ধ্রুব hb_bit_page_t &other) ধ্রুব
  { প্রত্যাগম intersects (other); }

  তার্কিক operator <= (ধ্রুব hb_bit_page_t &larger_page) ধ্রুব { প্রত্যাগম is_subset (larger_page); }
  তার্কিক is_subset (ধ্রুব hb_bit_page_t &larger_page) ধ্রুব
  {
    যদি (has_population () && larger_page.has_population () &&
	population > larger_page.population)
      প্রত্যাগম অসত্য;

    প্রতি (unsigned i = 0; i < len (); i++)
      যদি (~larger_page.v[i] & v[i])
	প্রত্যাগম অসত্য;
    প্রত্যাগম সত্য;
  }

  তার্কিক has_population () ধ্রুব { প্রত্যাগম population != UINT_MAX; }
  unsigned get_population () ধ্রুব
  {
    যদি (has_population ()) প্রত্যাগম population;
    প্রত্যাগম population = v;
  }

  তার্কিক next (hb_codepoint_t *codepoint) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক m = (*codepoint + 1) & MASK;
    যদি (!m)
    {
      *codepoint = INVALID;
      প্রত্যাগম অসত্য;
    }
    unsigned পূর্ণাঙ্ক i = m / ELT_BITS;
    unsigned পূর্ণাঙ্ক j = m & ELT_MASK;

    ধ্রুব elt_t vv = v[i] & ~((elt_t (1) << j) - 1);
    প্রতি (ধ্রুব elt_t *p = &vv; i < len (); p = ((ধ্রুব elt_t *) &v[0]) + (++i))
      যদি (*p)
      {
	*codepoint = i * ELT_BITS + elt_get_min (*p);
	প্রত্যাগম সত্য;
      }

    *codepoint = INVALID;
    প্রত্যাগম অসত্য;
  }
  তার্কিক previous (hb_codepoint_t *codepoint) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক m = (*codepoint - 1) & MASK;
    যদি (m == MASK)
    {
      *codepoint = INVALID;
      প্রত্যাগম অসত্য;
    }
    unsigned পূর্ণাঙ্ক i = m / ELT_BITS;
    unsigned পূর্ণাঙ্ক j = m & ELT_MASK;

    /* Fancy mask to avoid shifting by elt_t bitsize, which is undefined. */
    ধ্রুব elt_t mask = j < 8 * পরিমাণ (elt_t) - 1 ?
		       ((elt_t (1) << (j + 1)) - 1) :
		       (elt_t) -1;
    ধ্রুব elt_t vv = v[i] & mask;
    ধ্রুব elt_t *p = &vv;
    যাবত্ (সত্য)
    {
      যদি (*p)
      {
	*codepoint = i * ELT_BITS + elt_get_max (*p);
	প্রত্যাগম সত্য;
      }
      যদি ((পূর্ণাঙ্ক) i <= 0) ভঙ্গ;
      p = &v[--i];
    }

    *codepoint = INVALID;
    প্রত্যাগম অসত্য;
  }
  hb_codepoint_t get_min () ধ্রুব
  {
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < len (); i++)
      যদি (v[i])
	প্রত্যাগম i * ELT_BITS + elt_get_min (v[i]);
    প্রত্যাগম INVALID;
  }
  hb_codepoint_t get_max () ধ্রুব
  {
    প্রতি (পূর্ণাঙ্ক i = len () - 1; i >= 0; i--)
      যদি (v[i])
	প্রত্যাগম i * ELT_BITS + elt_get_max (v[i]);
    প্রত্যাগম INVALID;
  }

  /*
   * Iterator implementation.
   */
  সংরচনা iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    স্থির constexpr তার্কিক is_sorted_iterator = সত্য;
    iter_t (ধ্রুব hb_bit_page_t &s_ = Null (hb_bit_page_t), তার্কিক init = সত্য) : s (&s_), v (INVALID)
    {
      যদি (init)
	v = s->get_min ();
    }

    প্রকারনির্দেশ hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ধ্রুব { প্রত্যাগম v; }
    তার্কিক __more__ () ধ্রুব { প্রত্যাগম v != INVALID; }
    শূন্য __next__ () {
       s->next (&v);
    }
    শূন্য __prev__ () { s->previous (&v); }
    iter_t end () ধ্রুব { প্রত্যাগম iter_t (*s, অসত্য); }
    তার্কিক operator != (ধ্রুব iter_t& o) ধ্রুব
    { প্রত্যাগম v != o.v; }

    রক্ষিত:
    ধ্রুব hb_bit_page_t *s;
    hb_codepoint_t v;
  };
  iter_t iter () ধ্রুব { প্রত্যাগম iter_t (*এতত্); }
  operator iter_t () ধ্রুব { প্রত্যাগম iter (); }

  স্থির constexpr hb_codepoint_t INVALID = HB_SET_মান_INVALID;

  প্রকারনির্দেশ unsigned long long elt_t;
  স্থির constexpr unsigned PAGE_BITS_LOG_2 = 9; // 512 bits
  স্থির constexpr unsigned PAGE_BITS = 1 << PAGE_BITS_LOG_2;
  static_assert (1 << PAGE_BITS_LOG_2 == PAGE_BITS, "");
  static_assert ((PAGE_BITS & ((PAGE_BITS) - 1)) == 0, "");
  স্থির constexpr unsigned PAGE_BITMASK = PAGE_BITS - 1;

  স্থির unsigned পূর্ণাঙ্ক elt_get_min (ধ্রুব elt_t &elt) { প্রত্যাগম hb_ctz (elt); }
  স্থির unsigned পূর্ণাঙ্ক elt_get_max (ধ্রুব elt_t &elt) { প্রত্যাগম hb_bit_storage (elt) - 1; }

  প্রকারনির্দেশ hb_vector_size_t<elt_t, PAGE_BITS / 8> vector_t;

  স্থির constexpr unsigned ELT_BITS = পরিমাণ (elt_t) * 8;
  স্থির constexpr unsigned ELT_MASK = ELT_BITS - 1;

  স্থির constexpr unsigned BITS = পরিমাণ (vector_t) * 8;
  স্থির constexpr unsigned MASK = BITS - 1;
  static_assert ((unsigned) PAGE_BITS == (unsigned) BITS, "");

  elt_t &elt (hb_codepoint_t g) { প্রত্যাগম v[(g & MASK) / ELT_BITS]; }
  ধ্রুব elt_t& elt (hb_codepoint_t g) ধ্রুব { প্রত্যাগম v[(g & MASK) / ELT_BITS]; }
  স্থির constexpr elt_t mask (hb_codepoint_t g) { প্রত্যাগম elt_t (1) << (g & ELT_MASK); }

  mutable unsigned population;
  vector_t v;
};


#endif /* HB_BIT_PAGE_HH */

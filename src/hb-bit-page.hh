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
template <typename elt_t, unsigned পূর্ণসংখ্যা byte_আকার>
কাঠামো hb_vector_আকার_t
{
  elt_t& operator [] (unsigned পূর্ণসংখ্যা i) { ফেরত v[i]; }
  ধ্রুবক elt_t& operator [] (unsigned পূর্ণসংখ্যা i) ধ্রুবক { ফেরত v[i]; }

  শূন্য init0 ()
  {
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < ARRAY_দৈর্ঘ্য (v); i++)
      v[i] = 0;
  }
  শূন্য init1 ()
  {
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < ARRAY_দৈর্ঘ্য (v); i++)
      v[i] = (elt_t) -1;
  }

  template <typename Op>
  hb_vector_আকার_t process (ধ্রুবক Op& op) ধ্রুবক
  {
    hb_vector_আকার_t r;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < ARRAY_দৈর্ঘ্য (v); i++)
      r.v[i] = op (v[i]);
    ফেরত r;
  }
  template <typename Op>
  hb_vector_আকার_t process (ধ্রুবক Op& op, ধ্রুবক hb_vector_আকার_t &o) ধ্রুবক
  {
    hb_vector_আকার_t r;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < ARRAY_দৈর্ঘ্য (v); i++)
      r.v[i] = op (v[i], o.v[i]);
    ফেরত r;
  }
  hb_vector_আকার_t operator | (ধ্রুবক hb_vector_আকার_t &o) ধ্রুবক
  { ফেরত process (hb_bitwise_or, o); }
  hb_vector_আকার_t operator & (ধ্রুবক hb_vector_আকার_t &o) ধ্রুবক
  { ফেরত process (hb_bitwise_and, o); }
  hb_vector_আকার_t operator ^ (ধ্রুবক hb_vector_আকার_t &o) ধ্রুবক
  { ফেরত process (hb_bitwise_xor, o); }
  hb_vector_আকার_t operator ~ () ধ্রুবক
  { ফেরত process (hb_bitwise_neg); }

  operator যৌক্তিক () ধ্রুবক
  {
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < ARRAY_দৈর্ঘ্য (v); i++)
      যদি (v[i])
	ফেরত সত্য;
    ফেরত মিথ্যা;
  }
  operator unsigned পূর্ণসংখ্যা () ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা r = 0;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < ARRAY_দৈর্ঘ্য (v); i++)
      r += hb_popcount (v[i]);
    ফেরত r;
  }
  যৌক্তিক operator == (ধ্রুবক hb_vector_আকার_t &o) ধ্রুবক
  {
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < ARRAY_দৈর্ঘ্য (v); i++)
      যদি (v[i] != o.v[i])
	ফেরত মিথ্যা;
    ফেরত সত্য;
  }

  hb_array_t<ধ্রুবক elt_t> iter () ধ্রুবক
  { ফেরত hb_array (v); }

  ব্যক্তিগত:
  static_assert (0 == byte_আকার % আকার (elt_t), "");
  elt_t v[byte_আকার / আকার (elt_t)];
};


কাঠামো hb_bit_page_t
{
  hb_bit_page_t () { init0 (); }

  শূন্য init0 () { v.init0 (); population = 0; }
  শূন্য init1 () { v.init1 (); population = PAGE_BITS; }

  শূন্য dirty () { population = UINT_MAX; }

  স্থির অন্তঃরেখ constexpr unsigned len ()
  { ফেরত ARRAY_দৈর্ঘ্য_CONST (v); }

  operator যৌক্তিক () ধ্রুবক { ফেরত !is_empty (); }
  যৌক্তিক is_empty () ধ্রুবক
  {
    যদি (has_population ()) ফেরত !population;
    যৌক্তিক empty = !v;
    যদি (empty) population = 0;
    ফেরত empty;
  }
  uint32_t hash () ধ্রুবক
  {
    ফেরত hb_bytes_t ((ধ্রুবক অক্ষর *) &v, আকার (v)).hash ();
  }

  শূন্য add (hb_codepoint_t g) { elt (g) |= mask (g); dirty (); }
  শূন্য add_bits (hb_codepoint_t g, uint64_t bits) { elt (g) |= bits; dirty (); }
  শূন্য del (hb_codepoint_t g) { elt (g) &= ~mask (g); dirty (); }
  শূন্য del_bits (hb_codepoint_t g, uint64_t bits) { elt (g) &= ~bits; dirty (); }
  শূন্য set (hb_codepoint_t g, যৌক্তিক value) { যদি (value) add (g); অন্যথা del (g); }
  যৌক্তিক get (hb_codepoint_t g) ধ্রুবক { ফেরত elt (g) & mask (g); }
  যৌক্তিক may_have (hb_codepoint_t g) ধ্রুবক { ফেরত get (g); }

  যৌক্তিক operator [] (hb_codepoint_t g) ধ্রুবক { ফেরত get (g); }
  যৌক্তিক operator () (hb_codepoint_t g) ধ্রুবক { ফেরত get (g); }
  যৌক্তিক has (hb_codepoint_t g) ধ্রুবক { ফেরত get (g); }

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

      hb_memset (la, 0xff, (অক্ষর *) lb - (অক্ষর *) la);

      *lb |= ((mask (b) << 1) - 1llu);
    }
    অন্যথা
      ফেরত;
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

      hb_memset (la, 0, (অক্ষর *) lb - (অক্ষর *) la);

      *lb &= ~((mask (b) << 1) - 1llu);
    }
    অন্যথা
      ফেরত;
    dirty ();
  }
  শূন্য set_range (hb_codepoint_t a, hb_codepoint_t b, যৌক্তিক v)
  { যদি (v) add_range (a, b); অন্যথা del_range (a, b); }


  // Writes out page values to the array p. Returns the number of values
  // written. At most size codepoints will be written.
  unsigned পূর্ণসংখ্যা লিখ (uint32_t        base,
		      unsigned পূর্ণসংখ্যা    start_মান,
		      hb_codepoint_t *p,
		      unsigned পূর্ণসংখ্যা    আকার_2) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা start_v = start_মান / ELT_BITS;
    unsigned পূর্ণসংখ্যা start_bit = start_মান & ELT_MASK;
    unsigned পূর্ণসংখ্যা গুনতি = 0;
    জন্য (unsigned i = start_v; i < len () && গুনতি < আকার_2; i++)
    {
      elt_t bits = v[i];
      uint32_t v_base = base | (i * ELT_BITS);
      জন্য (unsigned পূর্ণসংখ্যা j = start_bit; j < ELT_BITS && গুনতি < আকার_2; j++)
      {
	যদি ((elt_t(1) << j) & bits) {
	  *p++ = v_base | j;
	  গুনতি++;
	}
      }
      start_bit = 0;
    }
    ফেরত গুনতি;
  }

  // Writes out the values NOT in this page to the array p. Returns the
  // number of values written. At most size codepoints will be written.
  // Returns the number of codepoints written. next_value holds the next value
  // that should be written (if not present in this page). This is used to fill
  // any missing value gaps between this page and the previous page, if any.
  // next_value is updated to one more than the last value present in this page.
  unsigned পূর্ণসংখ্যা write_inverted (uint32_t        base,
			       unsigned পূর্ণসংখ্যা    start_মান,
			       hb_codepoint_t *p,
			       unsigned পূর্ণসংখ্যা    আকার_2,
			       hb_codepoint_t *পরবর্তী_মান) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা start_v = start_মান / ELT_BITS;
    unsigned পূর্ণসংখ্যা start_bit = start_মান & ELT_MASK;
    unsigned পূর্ণসংখ্যা গুনতি = 0;
    জন্য (unsigned i = start_v; i < len () && গুনতি < আকার_2; i++)
    {
      elt_t bits = v[i];
      uint32_t v_offset = i * ELT_BITS;
      জন্য (unsigned পূর্ণসংখ্যা j = start_bit; j < ELT_BITS && গুনতি < আকার_2; j++)
      {
	যদি ((elt_t(1) << j) & bits)
	{
	  hb_codepoint_t value = base | v_offset | j;
	  // Emit all the missing values from next_value up to value - 1.
	  জন্য (hb_codepoint_t k = *পরবর্তী_মান; k < value && গুনতি < আকার_2; k++)
	  {
	    *p++ = k;
	    গুনতি++;
	  }
	  // Skip over this value;
	  *পরবর্তী_মান = value + 1;
	}
      }
      start_bit = 0;
    }
    ফেরত গুনতি;
  }

  যৌক্তিক operator == (ধ্রুবক hb_bit_page_t &other) ধ্রুবক { ফেরত is_equal (other); }
  যৌক্তিক is_equal (ধ্রুবক hb_bit_page_t &other) ধ্রুবক { ফেরত v == other.v; }
  যৌক্তিক intersects (ধ্রুবক hb_bit_page_t &other) ধ্রুবক
  {
    জন্য (unsigned i = 0; i < len (); i++)
      যদি (v[i] & other.v[i])
	ফেরত সত্য;
    ফেরত মিথ্যা;
  }
  যৌক্তিক may_intersect (ধ্রুবক hb_bit_page_t &other) ধ্রুবক
  { ফেরত intersects (other); }

  যৌক্তিক operator <= (ধ্রুবক hb_bit_page_t &larger_page) ধ্রুবক { ফেরত is_subset (larger_page); }
  যৌক্তিক is_subset (ধ্রুবক hb_bit_page_t &larger_page) ধ্রুবক
  {
    যদি (has_population () && larger_page.has_population () &&
	population > larger_page.population)
      ফেরত মিথ্যা;

    জন্য (unsigned i = 0; i < len (); i++)
      যদি (~larger_page.v[i] & v[i])
	ফেরত মিথ্যা;
    ফেরত সত্য;
  }

  যৌক্তিক has_population () ধ্রুবক { ফেরত population != UINT_MAX; }
  unsigned get_population () ধ্রুবক
  {
    যদি (has_population ()) ফেরত population;
    ফেরত population = v;
  }

  যৌক্তিক next (hb_codepoint_t *codepoint) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা m = (*codepoint + 1) & MASK;
    যদি (!m)
    {
      *codepoint = INVALID;
      ফেরত মিথ্যা;
    }
    unsigned পূর্ণসংখ্যা i = m / ELT_BITS;
    unsigned পূর্ণসংখ্যা j = m & ELT_MASK;

    ধ্রুবক elt_t vv = v[i] & ~((elt_t (1) << j) - 1);
    জন্য (ধ্রুবক elt_t *p = &vv; i < len (); p = ((ধ্রুবক elt_t *) &v[0]) + (++i))
      যদি (*p)
      {
	*codepoint = i * ELT_BITS + elt_get_min (*p);
	ফেরত সত্য;
      }

    *codepoint = INVALID;
    ফেরত মিথ্যা;
  }
  যৌক্তিক previous (hb_codepoint_t *codepoint) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা m = (*codepoint - 1) & MASK;
    যদি (m == MASK)
    {
      *codepoint = INVALID;
      ফেরত মিথ্যা;
    }
    unsigned পূর্ণসংখ্যা i = m / ELT_BITS;
    unsigned পূর্ণসংখ্যা j = m & ELT_MASK;

    /* Fancy mask to avoid shifting by elt_t bitsize, which is undefined. */
    ধ্রুবক elt_t mask = j < 8 * আকার (elt_t) - 1 ?
		       ((elt_t (1) << (j + 1)) - 1) :
		       (elt_t) -1;
    ধ্রুবক elt_t vv = v[i] & mask;
    ধ্রুবক elt_t *p = &vv;
    যখন (সত্য)
    {
      যদি (*p)
      {
	*codepoint = i * ELT_BITS + elt_get_max (*p);
	ফেরত সত্য;
      }
      যদি ((পূর্ণসংখ্যা) i <= 0) বিরতি;
      p = &v[--i];
    }

    *codepoint = INVALID;
    ফেরত মিথ্যা;
  }
  hb_codepoint_t get_min () ধ্রুবক
  {
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < len (); i++)
      যদি (v[i])
	ফেরত i * ELT_BITS + elt_get_min (v[i]);
    ফেরত INVALID;
  }
  hb_codepoint_t get_max () ধ্রুবক
  {
    জন্য (পূর্ণসংখ্যা i = len () - 1; i >= 0; i--)
      যদি (v[i])
	ফেরত i * ELT_BITS + elt_get_max (v[i]);
    ফেরত INVALID;
  }

  /*
   * Iterator implementation.
   */
  কাঠামো iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    স্থির constexpr যৌক্তিক is_sorted_iterator = সত্য;
    iter_t (ধ্রুবক hb_bit_page_t &s_ = Null (hb_bit_page_t), যৌক্তিক init = সত্য) : s (&s_), v (INVALID)
    {
      যদি (init)
	v = s->get_min ();
    }

    প্রকারলেখ hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ধ্রুবক { ফেরত v; }
    যৌক্তিক __more__ () ধ্রুবক { ফেরত v != INVALID; }
    শূন্য __next__ () {
       s->next (&v);
    }
    শূন্য __prev__ () { s->previous (&v); }
    iter_t end () ধ্রুবক { ফেরত iter_t (*s, মিথ্যা); }
    যৌক্তিক operator != (ধ্রুবক iter_t& o) ধ্রুবক
    { ফেরত v != o.v; }

    সুরক্ষিত:
    ধ্রুবক hb_bit_page_t *s;
    hb_codepoint_t v;
  };
  iter_t iter () ধ্রুবক { ফেরত iter_t (*এই); }
  operator iter_t () ধ্রুবক { ফেরত iter (); }

  স্থির constexpr hb_codepoint_t INVALID = HB_SET_মান_INVALID;

  প্রকারলেখ unsigned long long elt_t;
  স্থির constexpr unsigned PAGE_BITS_LOG_2 = 9; // 512 bits
  স্থির constexpr unsigned PAGE_BITS = 1 << PAGE_BITS_LOG_2;
  static_assert (1 << PAGE_BITS_LOG_2 == PAGE_BITS, "");
  static_assert ((PAGE_BITS & ((PAGE_BITS) - 1)) == 0, "");
  স্থির constexpr unsigned PAGE_BITMASK = PAGE_BITS - 1;

  স্থির unsigned পূর্ণসংখ্যা elt_get_min (ধ্রুবক elt_t &elt) { ফেরত hb_ctz (elt); }
  স্থির unsigned পূর্ণসংখ্যা elt_get_max (ধ্রুবক elt_t &elt) { ফেরত hb_bit_storage (elt) - 1; }

  প্রকারলেখ hb_vector_আকার_t<elt_t, PAGE_BITS / 8> vector_t;

  স্থির constexpr unsigned ELT_BITS = আকার (elt_t) * 8;
  স্থির constexpr unsigned ELT_MASK = ELT_BITS - 1;

  স্থির constexpr unsigned BITS = আকার (vector_t) * 8;
  স্থির constexpr unsigned MASK = BITS - 1;
  static_assert ((unsigned) PAGE_BITS == (unsigned) BITS, "");

  elt_t &elt (hb_codepoint_t g) { ফেরত v[(g & MASK) / ELT_BITS]; }
  ধ্রুবক elt_t& elt (hb_codepoint_t g) ধ্রুবক { ফেরত v[(g & MASK) / ELT_BITS]; }
  স্থির constexpr elt_t mask (hb_codepoint_t g) { ফেরত elt_t (1) << (g & ELT_MASK); }

  mutable unsigned population;
  vector_t v;
};


#endif /* HB_BIT_PAGE_HH */

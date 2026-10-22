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

#ଅନ୍ତର୍ଭାଵ "hb.hh"


/* Compiler-assisted vectorization. */

/* Type behaving similar to vectorized vars defined using __attribute__((vector_size(...))),
 * basically a fixed-size bitset. We can't use the compiler type because hb_vector_t cannot
 * guarantee alignment requirements. */
template <typename elt_t, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା byte_size>
ସଂରଚନା hb_vector_size_t
{
  elt_t& operator [] (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) { ଫେରନ୍ତୁ v[i]; }
  ଅଚଳ elt_t& operator [] (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ { ଫେରନ୍ତୁ v[i]; }

  ଶୂନ୍ୟ init0 ()
  {
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ARRAY_LENGTH (v); i++)
      v[i] = 0;
  }
  ଶୂନ୍ୟ init1 ()
  {
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ARRAY_LENGTH (v); i++)
      v[i] = (elt_t) -1;
  }

  template <typename Op>
  hb_vector_size_t process (ଅଚଳ Op& op) ଅଚଳ
  {
    hb_vector_size_t r;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ARRAY_LENGTH (v); i++)
      r.v[i] = op (v[i]);
    ଫେରନ୍ତୁ r;
  }
  template <typename Op>
  hb_vector_size_t process (ଅଚଳ Op& op, ଅଚଳ hb_vector_size_t &o) ଅଚଳ
  {
    hb_vector_size_t r;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ARRAY_LENGTH (v); i++)
      r.v[i] = op (v[i], o.v[i]);
    ଫେରନ୍ତୁ r;
  }
  hb_vector_size_t operator | (ଅଚଳ hb_vector_size_t &o) ଅଚଳ
  { ଫେରନ୍ତୁ process (hb_bitwise_or, o); }
  hb_vector_size_t operator & (ଅଚଳ hb_vector_size_t &o) ଅଚଳ
  { ଫେରନ୍ତୁ process (hb_bitwise_and, o); }
  hb_vector_size_t operator ^ (ଅଚଳ hb_vector_size_t &o) ଅଚଳ
  { ଫେରନ୍ତୁ process (hb_bitwise_xor, o); }
  hb_vector_size_t operator ~ () ଅଚଳ
  { ଫେରନ୍ତୁ process (hb_bitwise_neg); }

  operator ତାର୍କିକ () ଅଚଳ
  {
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ARRAY_LENGTH (v); i++)
      ଯଦି (v[i])
	ଫେରନ୍ତୁ ସତ୍ୟ;
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }
  operator unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା () ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା r = 0;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ARRAY_LENGTH (v); i++)
      r += hb_popcount (v[i]);
    ଫେରନ୍ତୁ r;
  }
  ତାର୍କିକ operator == (ଅଚଳ hb_vector_size_t &o) ଅଚଳ
  {
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ARRAY_LENGTH (v); i++)
      ଯଦି (v[i] != o.v[i])
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  hb_array_t<ଅଚଳ elt_t> iter () ଅଚଳ
  { ଫେରନ୍ତୁ hb_array (v); }

  ବ୍ୟକ୍ତିଗତ:
  static_assert (0 == byte_size % ଆକାର (elt_t), "");
  elt_t v[byte_size / ଆକାର (elt_t)];
};


ସଂରଚନା hb_bit_page_t
{
  hb_bit_page_t () { init0 (); }

  ଶୂନ୍ୟ init0 () { v.init0 (); population = 0; }
  ଶୂନ୍ୟ init1 () { v.init1 (); population = PAGE_BITS; }

  ଶୂନ୍ୟ dirty () { population = UINT_MAX; }

  ସ୍ଥିର ଧାଡ଼ିଭିତରେ constexpr unsigned len ()
  { ଫେରନ୍ତୁ ARRAY_LENGTH_CONST (v); }

  operator ତାର୍କିକ () ଅଚଳ { ଫେରନ୍ତୁ !is_empty (); }
  ତାର୍କିକ is_empty () ଅଚଳ
  {
    ଯଦି (has_population ()) ଫେରନ୍ତୁ !population;
    ତାର୍କିକ empty = !v;
    ଯଦି (empty) population = 0;
    ଫେରନ୍ତୁ empty;
  }
  uint32_t hash () ଅଚଳ
  {
    ଫେରନ୍ତୁ hb_bytes_t ((ଅଚଳ ଅକ୍ଷର *) &v, ଆକାର (v)).hash ();
  }

  ଶୂନ୍ୟ add (hb_codepoint_t g) { elt (g) |= mask (g); dirty (); }
  ଶୂନ୍ୟ add_bits (hb_codepoint_t g, uint64_t bits) { elt (g) |= bits; dirty (); }
  ଶୂନ୍ୟ del (hb_codepoint_t g) { elt (g) &= ~mask (g); dirty (); }
  ଶୂନ୍ୟ del_bits (hb_codepoint_t g, uint64_t bits) { elt (g) &= ~bits; dirty (); }
  ଶୂନ୍ୟ set (hb_codepoint_t g, ତାର୍କିକ value) { ଯଦି (value) add (g); ଅନ୍ୟଥା del (g); }
  ତାର୍କିକ get (hb_codepoint_t g) ଅଚଳ { ଫେରନ୍ତୁ elt (g) & mask (g); }
  ତାର୍କିକ may_have (hb_codepoint_t g) ଅଚଳ { ଫେରନ୍ତୁ get (g); }

  ତାର୍କିକ operator [] (hb_codepoint_t g) ଅଚଳ { ଫେରନ୍ତୁ get (g); }
  ତାର୍କିକ operator () (hb_codepoint_t g) ଅଚଳ { ଫେରନ୍ତୁ get (g); }
  ତାର୍କିକ has (hb_codepoint_t g) ଅଚଳ { ଫେରନ୍ତୁ get (g); }

  ଶୂନ୍ୟ add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    ଯଦି (la == lb)
      *la |= (mask (b) << 1) - mask(a);
    ଅନ୍ୟଥା ଯଦି (likely (la < lb))
    {
      *la |= ~(mask (a) - 1llu);
      la++;

      hb_memset (la, 0xff, (ଅକ୍ଷର *) lb - (ଅକ୍ଷର *) la);

      *lb |= ((mask (b) << 1) - 1llu);
    }
    ଅନ୍ୟଥା
      ଫେରନ୍ତୁ;
    dirty ();
  }
  ଶୂନ୍ୟ del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    ଯଦି (la == lb)
      *la &= ~((mask (b) << 1llu) - mask(a));
    ଅନ୍ୟଥା ଯଦି (likely (la < lb))
    {
      *la &= mask (a) - 1;
      la++;

      hb_memset (la, 0, (ଅକ୍ଷର *) lb - (ଅକ୍ଷର *) la);

      *lb &= ~((mask (b) << 1) - 1llu);
    }
    ଅନ୍ୟଥା
      ଫେରନ୍ତୁ;
    dirty ();
  }
  ଶୂନ୍ୟ set_range (hb_codepoint_t a, hb_codepoint_t b, ତାର୍କିକ v)
  { ଯଦି (v) add_range (a, b); ଅନ୍ୟଥା del_range (a, b); }


  // Writes out page values to the array p. Returns the number of values
  // written. At most size codepoints will be written.
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଲିଖ (uint32_t        base,
		      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା    start_ମୂଲ୍ୟ,
		      hb_codepoint_t *p,
		      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା    size) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_v = start_ମୂଲ୍ୟ / ELT_BITS;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_bit = start_ମୂଲ୍ୟ & ELT_MASK;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;
    ପାଇଁ (unsigned i = start_v; i < len () && ଗଣନା_2 < size; i++)
    {
      elt_t bits = v[i];
      uint32_t v_base = base | (i * ELT_BITS);
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = start_bit; j < ELT_BITS && ଗଣନା_2 < size; j++)
      {
	ଯଦି ((elt_t(1) << j) & bits) {
	  *p++ = v_base | j;
	  ଗଣନା_2++;
	}
      }
      start_bit = 0;
    }
    ଫେରନ୍ତୁ ଗଣନା_2;
  }

  // Writes out the values NOT in this page to the array p. Returns the
  // number of values written. At most size codepoints will be written.
  // Returns the number of codepoints written. next_value holds the next value
  // that should be written (if not present in this page). This is used to fill
  // any missing value gaps between this page and the previous page, if any.
  // next_value is updated to one more than the last value present in this page.
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା write_inverted (uint32_t        base,
			       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା    start_ମୂଲ୍ୟ,
			       hb_codepoint_t *p,
			       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା    size,
			       hb_codepoint_t *next_ମୂଲ୍ୟ) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_v = start_ମୂଲ୍ୟ / ELT_BITS;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_bit = start_ମୂଲ୍ୟ & ELT_MASK;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;
    ପାଇଁ (unsigned i = start_v; i < len () && ଗଣନା_2 < size; i++)
    {
      elt_t bits = v[i];
      uint32_t v_offset = i * ELT_BITS;
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = start_bit; j < ELT_BITS && ଗଣନା_2 < size; j++)
      {
	ଯଦି ((elt_t(1) << j) & bits)
	{
	  hb_codepoint_t value = base | v_offset | j;
	  // Emit all the missing values from next_value up to value - 1.
	  ପାଇଁ (hb_codepoint_t k = *next_ମୂଲ୍ୟ; k < value && ଗଣନା_2 < size; k++)
	  {
	    *p++ = k;
	    ଗଣନା_2++;
	  }
	  // Skip over this value;
	  *next_ମୂଲ୍ୟ = value + 1;
	}
      }
      start_bit = 0;
    }
    ଫେରନ୍ତୁ ଗଣନା_2;
  }

  ତାର୍କିକ operator == (ଅଚଳ hb_bit_page_t &other) ଅଚଳ { ଫେରନ୍ତୁ is_equal (other); }
  ତାର୍କିକ is_equal (ଅଚଳ hb_bit_page_t &other) ଅଚଳ { ଫେରନ୍ତୁ v == other.v; }
  ତାର୍କିକ intersects (ଅଚଳ hb_bit_page_t &other) ଅଚଳ
  {
    ପାଇଁ (unsigned i = 0; i < len (); i++)
      ଯଦି (v[i] & other.v[i])
	ଫେରନ୍ତୁ ସତ୍ୟ;
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }
  ତାର୍କିକ may_intersect (ଅଚଳ hb_bit_page_t &other) ଅଚଳ
  { ଫେରନ୍ତୁ intersects (other); }

  ତାର୍କିକ operator <= (ଅଚଳ hb_bit_page_t &larger_page) ଅଚଳ { ଫେରନ୍ତୁ is_subset (larger_page); }
  ତାର୍କିକ is_subset (ଅଚଳ hb_bit_page_t &larger_page) ଅଚଳ
  {
    ଯଦି (has_population () && larger_page.has_population () &&
	population > larger_page.population)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ପାଇଁ (unsigned i = 0; i < len (); i++)
      ଯଦି (~larger_page.v[i] & v[i])
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ତାର୍କିକ has_population () ଅଚଳ { ଫେରନ୍ତୁ population != UINT_MAX; }
  unsigned get_population () ଅଚଳ
  {
    ଯଦି (has_population ()) ଫେରନ୍ତୁ population;
    ଫେରନ୍ତୁ population = v;
  }

  ତାର୍କିକ next (hb_codepoint_t *codepoint) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା m = (*codepoint + 1) & MASK;
    ଯଦି (!m)
    {
      *codepoint = INVALID;
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = m / ELT_BITS;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = m & ELT_MASK;

    ଅଚଳ elt_t vv = v[i] & ~((elt_t (1) << j) - 1);
    ପାଇଁ (ଅଚଳ elt_t *p = &vv; i < len (); p = ((ଅଚଳ elt_t *) &v[0]) + (++i))
      ଯଦି (*p)
      {
	*codepoint = i * ELT_BITS + elt_get_min (*p);
	ଫେରନ୍ତୁ ସତ୍ୟ;
      }

    *codepoint = INVALID;
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }
  ତାର୍କିକ previous (hb_codepoint_t *codepoint) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା m = (*codepoint - 1) & MASK;
    ଯଦି (m == MASK)
    {
      *codepoint = INVALID;
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = m / ELT_BITS;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = m & ELT_MASK;

    /* Fancy mask to avoid shifting by elt_t bitsize, which is undefined. */
    ଅଚଳ elt_t mask = j < 8 * ଆକାର (elt_t) - 1 ?
		       ((elt_t (1) << (j + 1)) - 1) :
		       (elt_t) -1;
    ଅଚଳ elt_t vv = v[i] & mask;
    ଅଚଳ elt_t *p = &vv;
    ଯେତେବେଳେ (ସତ୍ୟ)
    {
      ଯଦି (*p)
      {
	*codepoint = i * ELT_BITS + elt_get_max (*p);
	ଫେରନ୍ତୁ ସତ୍ୟ;
      }
      ଯଦି ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) i <= 0) ଭାଙ୍ଗ;
      p = &v[--i];
    }

    *codepoint = INVALID;
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }
  hb_codepoint_t get_min () ଅଚଳ
  {
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len (); i++)
      ଯଦି (v[i])
	ଫେରନ୍ତୁ i * ELT_BITS + elt_get_min (v[i]);
    ଫେରନ୍ତୁ INVALID;
  }
  hb_codepoint_t get_max () ଅଚଳ
  {
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = len () - 1; i >= 0; i--)
      ଯଦି (v[i])
	ଫେରନ୍ତୁ i * ELT_BITS + elt_get_max (v[i]);
    ଫେରନ୍ତୁ INVALID;
  }

  /*
   * Iterator implementation.
   */
  ସଂରଚନା iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ସ୍ଥିର constexpr ତାର୍କିକ is_sorted_iterator = ସତ୍ୟ;
    iter_t (ଅଚଳ hb_bit_page_t &s_ = Null (hb_bit_page_t), ତାର୍କିକ init = ସତ୍ୟ) : s (&s_), v (INVALID)
    {
      ଯଦି (init)
	v = s->get_min ();
    }

    ପ୍ରକାରବ୍ୟାଖ୍ୟା hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ଅଚଳ { ଫେରନ୍ତୁ v; }
    ତାର୍କିକ __more__ () ଅଚଳ { ଫେରନ୍ତୁ v != INVALID; }
    ଶୂନ୍ୟ __next__ () {
       s->next (&v);
    }
    ଶୂନ୍ୟ __prev__ () { s->previous (&v); }
    iter_t end () ଅଚଳ { ଫେରନ୍ତୁ iter_t (*s, ମିଥ୍ୟା); }
    ତାର୍କିକ operator != (ଅଚଳ iter_t& o) ଅଚଳ
    { ଫେରନ୍ତୁ v != o.v; }

    ସୁରକ୍ଷିତ:
    ଅଚଳ hb_bit_page_t *s;
    hb_codepoint_t v;
  };
  iter_t iter () ଅଚଳ { ଫେରନ୍ତୁ iter_t (*ଏହି); }
  operator iter_t () ଅଚଳ { ଫେରନ୍ତୁ iter (); }

  ସ୍ଥିର constexpr hb_codepoint_t INVALID = HB_SET_ମୂଲ୍ୟ_INVALID;

  ପ୍ରକାରବ୍ୟାଖ୍ୟା unsigned long long elt_t;
  ସ୍ଥିର constexpr unsigned PAGE_BITS_LOG_2 = 9; // 512 bits
  ସ୍ଥିର constexpr unsigned PAGE_BITS = 1 << PAGE_BITS_LOG_2;
  static_assert (1 << PAGE_BITS_LOG_2 == PAGE_BITS, "");
  static_assert ((PAGE_BITS & ((PAGE_BITS) - 1)) == 0, "");
  ସ୍ଥିର constexpr unsigned PAGE_BITMASK = PAGE_BITS - 1;

  ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା elt_get_min (ଅଚଳ elt_t &elt) { ଫେରନ୍ତୁ hb_ctz (elt); }
  ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା elt_get_max (ଅଚଳ elt_t &elt) { ଫେରନ୍ତୁ hb_bit_storage (elt) - 1; }

  ପ୍ରକାରବ୍ୟାଖ୍ୟା hb_vector_size_t<elt_t, PAGE_BITS / 8> vector_t;

  ସ୍ଥିର constexpr unsigned ELT_BITS = ଆକାର (elt_t) * 8;
  ସ୍ଥିର constexpr unsigned ELT_MASK = ELT_BITS - 1;

  ସ୍ଥିର constexpr unsigned BITS = ଆକାର (vector_t) * 8;
  ସ୍ଥିର constexpr unsigned MASK = BITS - 1;
  static_assert ((unsigned) PAGE_BITS == (unsigned) BITS, "");

  elt_t &elt (hb_codepoint_t g) { ଫେରନ୍ତୁ v[(g & MASK) / ELT_BITS]; }
  ଅଚଳ elt_t& elt (hb_codepoint_t g) ଅଚଳ { ଫେରନ୍ତୁ v[(g & MASK) / ELT_BITS]; }
  ସ୍ଥିର constexpr elt_t mask (hb_codepoint_t g) { ଫେରନ୍ତୁ elt_t (1) << (g & ELT_MASK); }

  mutable unsigned population;
  vector_t v;
};


#endif /* HB_BIT_PAGE_HH */

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

#ಅನ್ತರ್ಭಾವ "hb.hh"


/* Compiler-assisted vectorization. */

/* Type behaving similar to vectorized vars defined using __attribute__((vector_size(...))),
 * basically a fixed-size bitset. We can't use the compiler type because hb_vector_t cannot
 * guarantee alignment requirements. */
template <typename elt_t, unsigned ಪೂರ್ಣಾಙ್ಕ byte_size>
ಸಂರಚನಾ hb_vector_size_t
{
  elt_t& operator [] (unsigned ಪೂರ್ಣಾಙ್ಕ i) { ಪ್ರತ್ಯಾಗಮ v[i]; }
  ಧ್ರುವ elt_t& operator [] (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ v[i]; }

  ಶೂನ್ಯ init0 ()
  {
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ARRAY_LENGTH (v); i++)
      v[i] = 0;
  }
  ಶೂನ್ಯ init1 ()
  {
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ARRAY_LENGTH (v); i++)
      v[i] = (elt_t) -1;
  }

  template <typename Op>
  hb_vector_size_t process (ಧ್ರುವ Op& op) ಧ್ರುವ
  {
    hb_vector_size_t r;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ARRAY_LENGTH (v); i++)
      r.v[i] = op (v[i]);
    ಪ್ರತ್ಯಾಗಮ r;
  }
  template <typename Op>
  hb_vector_size_t process (ಧ್ರುವ Op& op, ಧ್ರುವ hb_vector_size_t &o) ಧ್ರುವ
  {
    hb_vector_size_t r;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ARRAY_LENGTH (v); i++)
      r.v[i] = op (v[i], o.v[i]);
    ಪ್ರತ್ಯಾಗಮ r;
  }
  hb_vector_size_t operator | (ಧ್ರುವ hb_vector_size_t &o) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ process (hb_bitwise_or, o); }
  hb_vector_size_t operator & (ಧ್ರುವ hb_vector_size_t &o) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ process (hb_bitwise_and, o); }
  hb_vector_size_t operator ^ (ಧ್ರುವ hb_vector_size_t &o) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ process (hb_bitwise_xor, o); }
  hb_vector_size_t operator ~ () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ process (hb_bitwise_neg); }

  operator ತಾರ್ಕಿಕ () ಧ್ರುವ
  {
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ARRAY_LENGTH (v); i++)
      ಯದಿ (v[i])
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }
  operator unsigned ಪೂರ್ಣಾಙ್ಕ () ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ r = 0;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ARRAY_LENGTH (v); i++)
      r += hb_popcount (v[i]);
    ಪ್ರತ್ಯಾಗಮ r;
  }
  ತಾರ್ಕಿಕ operator == (ಧ್ರುವ hb_vector_size_t &o) ಧ್ರುವ
  {
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ARRAY_LENGTH (v); i++)
      ಯದಿ (v[i] != o.v[i])
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  hb_array_t<ಧ್ರುವ elt_t> iter () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_array (v); }

  ಗುಪ್ತ:
  static_assert (0 == byte_size % ಪರಿಮಾಣ (elt_t), "");
  elt_t v[byte_size / ಪರಿಮಾಣ (elt_t)];
};


ಸಂರಚನಾ hb_bit_page_t
{
  hb_bit_page_t () { init0 (); }

  ಶೂನ್ಯ init0 () { v.init0 (); population = 0; }
  ಶೂನ್ಯ init1 () { v.init1 (); population = PAGE_BITS; }

  ಶೂನ್ಯ dirty () { population = UINT_MAX; }

  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ constexpr unsigned len ()
  { ಪ್ರತ್ಯಾಗಮ ARRAY_LENGTH_CONST (v); }

  operator ತಾರ್ಕಿಕ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ !is_empty (); }
  ತಾರ್ಕಿಕ is_empty () ಧ್ರುವ
  {
    ಯದಿ (has_population ()) ಪ್ರತ್ಯಾಗಮ !population;
    ತಾರ್ಕಿಕ empty = !v;
    ಯದಿ (empty) population = 0;
    ಪ್ರತ್ಯಾಗಮ empty;
  }
  uint32_t hash () ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ hb_bytes_t ((ಧ್ರುವ ವರ್ಣ *) &v, ಪರಿಮಾಣ (v)).hash ();
  }

  ಶೂನ್ಯ add (hb_codepoint_t g) { elt (g) |= mask (g); dirty (); }
  ಶೂನ್ಯ add_bits (hb_codepoint_t g, uint64_t bits) { elt (g) |= bits; dirty (); }
  ಶೂನ್ಯ del (hb_codepoint_t g) { elt (g) &= ~mask (g); dirty (); }
  ಶೂನ್ಯ del_bits (hb_codepoint_t g, uint64_t bits) { elt (g) &= ~bits; dirty (); }
  ಶೂನ್ಯ set (hb_codepoint_t g, ತಾರ್ಕಿಕ value) { ಯದಿ (value) add (g); ಅನ್ಯಥಾ del (g); }
  ತಾರ್ಕಿಕ get (hb_codepoint_t g) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ elt (g) & mask (g); }
  ತಾರ್ಕಿಕ may_have (hb_codepoint_t g) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get (g); }

  ತಾರ್ಕಿಕ operator [] (hb_codepoint_t g) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get (g); }
  ತಾರ್ಕಿಕ operator () (hb_codepoint_t g) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get (g); }
  ತಾರ್ಕಿಕ has (hb_codepoint_t g) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get (g); }

  ಶೂನ್ಯ add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    ಯದಿ (la == lb)
      *la |= (mask (b) << 1) - mask(a);
    ಅನ್ಯಥಾ ಯದಿ (likely (la < lb))
    {
      *la |= ~(mask (a) - 1llu);
      la++;

      hb_memset (la, 0xff, (ವರ್ಣ *) lb - (ವರ್ಣ *) la);

      *lb |= ((mask (b) << 1) - 1llu);
    }
    ಅನ್ಯಥಾ
      ಪ್ರತ್ಯಾಗಮ;
    dirty ();
  }
  ಶೂನ್ಯ del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    ಯದಿ (la == lb)
      *la &= ~((mask (b) << 1llu) - mask(a));
    ಅನ್ಯಥಾ ಯದಿ (likely (la < lb))
    {
      *la &= mask (a) - 1;
      la++;

      hb_memset (la, 0, (ವರ್ಣ *) lb - (ವರ್ಣ *) la);

      *lb &= ~((mask (b) << 1) - 1llu);
    }
    ಅನ್ಯಥಾ
      ಪ್ರತ್ಯಾಗಮ;
    dirty ();
  }
  ಶೂನ್ಯ set_range (hb_codepoint_t a, hb_codepoint_t b, ತಾರ್ಕಿಕ v)
  { ಯದಿ (v) add_range (a, b); ಅನ್ಯಥಾ del_range (a, b); }


  // Writes out page values to the array p. Returns the number of values
  // written. At most size codepoints will be written.
  unsigned ಪೂರ್ಣಾಙ್ಕ ಲಿಖ (uint32_t        base,
		      unsigned ಪೂರ್ಣಾಙ್ಕ    start_ಮೌಲ್ಯ,
		      hb_codepoint_t *p,
		      unsigned ಪೂರ್ಣಾಙ್ಕ    size) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ start_v = start_ಮೌಲ್ಯ / ELT_BITS;
    unsigned ಪೂರ್ಣಾಙ್ಕ start_bit = start_ಮೌಲ್ಯ & ELT_MASK;
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 0;
    ಪ್ರತಿ (unsigned i = start_v; i < len () && ಎಣಿಕೆ < size; i++)
    {
      elt_t bits = v[i];
      uint32_t v_base = base | (i * ELT_BITS);
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ j = start_bit; j < ELT_BITS && ಎಣಿಕೆ < size; j++)
      {
	ಯದಿ ((elt_t(1) << j) & bits) {
	  *p++ = v_base | j;
	  ಎಣಿಕೆ++;
	}
      }
      start_bit = 0;
    }
    ಪ್ರತ್ಯಾಗಮ ಎಣಿಕೆ;
  }

  // Writes out the values NOT in this page to the array p. Returns the
  // number of values written. At most size codepoints will be written.
  // Returns the number of codepoints written. next_value holds the next value
  // that should be written (if not present in this page). This is used to fill
  // any missing value gaps between this page and the previous page, if any.
  // next_value is updated to one more than the last value present in this page.
  unsigned ಪೂರ್ಣಾಙ್ಕ write_inverted (uint32_t        base,
			       unsigned ಪೂರ್ಣಾಙ್ಕ    start_ಮೌಲ್ಯ,
			       hb_codepoint_t *p,
			       unsigned ಪೂರ್ಣಾಙ್ಕ    size,
			       hb_codepoint_t *next_ಮೌಲ್ಯ) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ start_v = start_ಮೌಲ್ಯ / ELT_BITS;
    unsigned ಪೂರ್ಣಾಙ್ಕ start_bit = start_ಮೌಲ್ಯ & ELT_MASK;
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 0;
    ಪ್ರತಿ (unsigned i = start_v; i < len () && ಎಣಿಕೆ < size; i++)
    {
      elt_t bits = v[i];
      uint32_t v_offset = i * ELT_BITS;
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ j = start_bit; j < ELT_BITS && ಎಣಿಕೆ < size; j++)
      {
	ಯದಿ ((elt_t(1) << j) & bits)
	{
	  hb_codepoint_t value = base | v_offset | j;
	  // Emit all the missing values from next_value up to value - 1.
	  ಪ್ರತಿ (hb_codepoint_t k = *next_ಮೌಲ್ಯ; k < value && ಎಣಿಕೆ < size; k++)
	  {
	    *p++ = k;
	    ಎಣಿಕೆ++;
	  }
	  // Skip over this value;
	  *next_ಮೌಲ್ಯ = value + 1;
	}
      }
      start_bit = 0;
    }
    ಪ್ರತ್ಯಾಗಮ ಎಣಿಕೆ;
  }

  ತಾರ್ಕಿಕ operator == (ಧ್ರುವ hb_bit_page_t &other) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ is_equal (other); }
  ತಾರ್ಕಿಕ is_equal (ಧ್ರುವ hb_bit_page_t &other) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ v == other.v; }
  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_bit_page_t &other) ಧ್ರುವ
  {
    ಪ್ರತಿ (unsigned i = 0; i < len (); i++)
      ಯದಿ (v[i] & other.v[i])
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }
  ತಾರ್ಕಿಕ may_intersect (ಧ್ರುವ hb_bit_page_t &other) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ intersects (other); }

  ತಾರ್ಕಿಕ operator <= (ಧ್ರುವ hb_bit_page_t &larger_page) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ is_subset (larger_page); }
  ತಾರ್ಕಿಕ is_subset (ಧ್ರುವ hb_bit_page_t &larger_page) ಧ್ರುವ
  {
    ಯದಿ (has_population () && larger_page.has_population () &&
	population > larger_page.population)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪ್ರತಿ (unsigned i = 0; i < len (); i++)
      ಯದಿ (~larger_page.v[i] & v[i])
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ has_population () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ population != UINT_MAX; }
  unsigned get_population () ಧ್ರುವ
  {
    ಯದಿ (has_population ()) ಪ್ರತ್ಯಾಗಮ population;
    ಪ್ರತ್ಯಾಗಮ population = v;
  }

  ತಾರ್ಕಿಕ next (hb_codepoint_t *codepoint) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ m = (*codepoint + 1) & MASK;
    ಯದಿ (!m)
    {
      *codepoint = INVALID;
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    unsigned ಪೂರ್ಣಾಙ್ಕ i = m / ELT_BITS;
    unsigned ಪೂರ್ಣಾಙ್ಕ j = m & ELT_MASK;

    ಧ್ರುವ elt_t vv = v[i] & ~((elt_t (1) << j) - 1);
    ಪ್ರತಿ (ಧ್ರುವ elt_t *p = &vv; i < len (); p = ((ಧ್ರುವ elt_t *) &v[0]) + (++i))
      ಯದಿ (*p)
      {
	*codepoint = i * ELT_BITS + elt_get_min (*p);
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      }

    *codepoint = INVALID;
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }
  ತಾರ್ಕಿಕ previous (hb_codepoint_t *codepoint) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ m = (*codepoint - 1) & MASK;
    ಯದಿ (m == MASK)
    {
      *codepoint = INVALID;
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    unsigned ಪೂರ್ಣಾಙ್ಕ i = m / ELT_BITS;
    unsigned ಪೂರ್ಣಾಙ್ಕ j = m & ELT_MASK;

    /* Fancy mask to avoid shifting by elt_t bitsize, which is undefined. */
    ಧ್ರುವ elt_t mask = j < 8 * ಪರಿಮಾಣ (elt_t) - 1 ?
		       ((elt_t (1) << (j + 1)) - 1) :
		       (elt_t) -1;
    ಧ್ರುವ elt_t vv = v[i] & mask;
    ಧ್ರುವ elt_t *p = &vv;
    ಯಾವತ್ (ಸತ್ಯ)
    {
      ಯದಿ (*p)
      {
	*codepoint = i * ELT_BITS + elt_get_max (*p);
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      }
      ಯದಿ ((ಪೂರ್ಣಾಙ್ಕ) i <= 0) ಭಙ್ಗ;
      p = &v[--i];
    }

    *codepoint = INVALID;
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }
  hb_codepoint_t get_min () ಧ್ರುವ
  {
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < len (); i++)
      ಯದಿ (v[i])
	ಪ್ರತ್ಯಾಗಮ i * ELT_BITS + elt_get_min (v[i]);
    ಪ್ರತ್ಯಾಗಮ INVALID;
  }
  hb_codepoint_t get_max () ಧ್ರುವ
  {
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = len () - 1; i >= 0; i--)
      ಯದಿ (v[i])
	ಪ್ರತ್ಯಾಗಮ i * ELT_BITS + elt_get_max (v[i]);
    ಪ್ರತ್ಯಾಗಮ INVALID;
  }

  /*
   * Iterator implementation.
   */
  ಸಂರಚನಾ iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ is_sorted_iterator = ಸತ್ಯ;
    iter_t (ಧ್ರುವ hb_bit_page_t &s_ = Null (hb_bit_page_t), ತಾರ್ಕಿಕ init = ಸತ್ಯ) : s (&s_), v (INVALID)
    {
      ಯದಿ (init)
	v = s->get_min ();
    }

    ಪ್ರಕಾರನಿರ್ದೇಶ hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ v; }
    ತಾರ್ಕಿಕ __more__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ v != INVALID; }
    ಶೂನ್ಯ __next__ () {
       s->next (&v);
    }
    ಶೂನ್ಯ __prev__ () { s->previous (&v); }
    iter_t end () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ iter_t (*s, ಅಸತ್ಯ); }
    ತಾರ್ಕಿಕ operator != (ಧ್ರುವ iter_t& o) ಧ್ರುವ
    { ಪ್ರತ್ಯಾಗಮ v != o.v; }

    ರಕ್ಷಿತ:
    ಧ್ರುವ hb_bit_page_t *s;
    hb_codepoint_t v;
  };
  iter_t iter () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ iter_t (*ಏತತ್); }
  operator iter_t () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ iter (); }

  ಸ್ಥಿರ constexpr hb_codepoint_t INVALID = HB_SET_ಮೌಲ್ಯ_INVALID;

  ಪ್ರಕಾರನಿರ್ದೇಶ unsigned long long elt_t;
  ಸ್ಥಿರ constexpr unsigned PAGE_BITS_LOG_2 = 9; // 512 bits
  ಸ್ಥಿರ constexpr unsigned PAGE_BITS = 1 << PAGE_BITS_LOG_2;
  static_assert (1 << PAGE_BITS_LOG_2 == PAGE_BITS, "");
  static_assert ((PAGE_BITS & ((PAGE_BITS) - 1)) == 0, "");
  ಸ್ಥಿರ constexpr unsigned PAGE_BITMASK = PAGE_BITS - 1;

  ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ elt_get_min (ಧ್ರುವ elt_t &elt) { ಪ್ರತ್ಯಾಗಮ hb_ctz (elt); }
  ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ elt_get_max (ಧ್ರುವ elt_t &elt) { ಪ್ರತ್ಯಾಗಮ hb_bit_storage (elt) - 1; }

  ಪ್ರಕಾರನಿರ್ದೇಶ hb_vector_size_t<elt_t, PAGE_BITS / 8> vector_t;

  ಸ್ಥಿರ constexpr unsigned ELT_BITS = ಪರಿಮಾಣ (elt_t) * 8;
  ಸ್ಥಿರ constexpr unsigned ELT_MASK = ELT_BITS - 1;

  ಸ್ಥಿರ constexpr unsigned BITS = ಪರಿಮಾಣ (vector_t) * 8;
  ಸ್ಥಿರ constexpr unsigned MASK = BITS - 1;
  static_assert ((unsigned) PAGE_BITS == (unsigned) BITS, "");

  elt_t &elt (hb_codepoint_t g) { ಪ್ರತ್ಯಾಗಮ v[(g & MASK) / ELT_BITS]; }
  ಧ್ರುವ elt_t& elt (hb_codepoint_t g) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ v[(g & MASK) / ELT_BITS]; }
  ಸ್ಥಿರ constexpr elt_t mask (hb_codepoint_t g) { ಪ್ರತ್ಯಾಗಮ elt_t (1) << (g & ELT_MASK); }

  mutable unsigned population;
  vector_t v;
};


#endif /* HB_BIT_PAGE_HH */

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

#ضمن "hb.hh"


/* Compiler-assisted vectorization. */

/* Type behaving similar to vectorized vars defined using __attribute__((vector_size(...))),
 * basically a fixed-size bitset. We can't use the compiler type because hb_vector_t cannot
 * guarantee alignment requirements. */
template <typename elt_t, unsigned صحيح byte_size>
بنية hb_vector_size_t
{
  elt_t& operator [] (unsigned صحيح i) { ܦܢܐ v[i]; }
  ثابت elt_t& operator [] (unsigned صحيح i) ثابت { ܦܢܐ v[i]; }

  فراغ init0 ()
  {
    لكل (unsigned صحيح i = 0; i < ARRAY_LENGTH (v); i++)
      v[i] = 0;
  }
  فراغ init1 ()
  {
    لكل (unsigned صحيح i = 0; i < ARRAY_LENGTH (v); i++)
      v[i] = (elt_t) -1;
  }

  template <typename Op>
  hb_vector_size_t process (ثابت Op& op) ثابت
  {
    hb_vector_size_t r;
    لكل (unsigned صحيح i = 0; i < ARRAY_LENGTH (v); i++)
      r.v[i] = op (v[i]);
    ܦܢܐ r;
  }
  template <typename Op>
  hb_vector_size_t process (ثابت Op& op, ثابت hb_vector_size_t &o) ثابت
  {
    hb_vector_size_t r;
    لكل (unsigned صحيح i = 0; i < ARRAY_LENGTH (v); i++)
      r.v[i] = op (v[i], o.v[i]);
    ܦܢܐ r;
  }
  hb_vector_size_t operator | (ثابت hb_vector_size_t &o) ثابت
  { ܦܢܐ process (hb_bitwise_or, o); }
  hb_vector_size_t operator & (ثابت hb_vector_size_t &o) ثابت
  { ܦܢܐ process (hb_bitwise_and, o); }
  hb_vector_size_t operator ^ (ثابت hb_vector_size_t &o) ثابت
  { ܦܢܐ process (hb_bitwise_xor, o); }
  hb_vector_size_t operator ~ () ثابت
  { ܦܢܐ process (hb_bitwise_neg); }

  operator منطقي () ثابت
  {
    لكل (unsigned صحيح i = 0; i < ARRAY_LENGTH (v); i++)
      ܐܢ (v[i])
	ܦܢܐ ܫܪܪܐ;
    ܦܢܐ ܕܓܠܐ;
  }
  operator unsigned صحيح () ثابت
  {
    unsigned صحيح r = 0;
    لكل (unsigned صحيح i = 0; i < ARRAY_LENGTH (v); i++)
      r += hb_popcount (v[i]);
    ܦܢܐ r;
  }
  منطقي operator == (ثابت hb_vector_size_t &o) ثابت
  {
    لكل (unsigned صحيح i = 0; i < ARRAY_LENGTH (v); i++)
      ܐܢ (v[i] != o.v[i])
	ܦܢܐ ܕܓܠܐ;
    ܦܢܐ ܫܪܪܐ;
  }

  hb_array_t<ثابت elt_t> iter () ثابت
  { ܦܢܐ hb_array (v); }

  خاص:
  static_assert (0 == byte_size % حجم (elt_t), "");
  elt_t v[byte_size / حجم (elt_t)];
};


بنية hb_bit_page_t
{
  hb_bit_page_t () { init0 (); }

  فراغ init0 () { v.init0 (); population = 0; }
  فراغ init1 () { v.init1 (); population = PAGE_BITS; }

  فراغ dirty () { population = UINT_MAX; }

  ساكن مضمن constexpr unsigned len ()
  { ܦܢܐ ARRAY_LENGTH_CONST (v); }

  operator منطقي () ثابت { ܦܢܐ !is_empty (); }
  منطقي is_empty () ثابت
  {
    ܐܢ (has_population ()) ܦܢܐ !population;
    منطقي empty = !v;
    ܐܢ (empty) population = 0;
    ܦܢܐ empty;
  }
  uint32_t hash () ثابت
  {
    ܦܢܐ hb_bytes_t ((ثابت حرف *) &v, حجم (v)).hash ();
  }

  فراغ add (hb_codepoint_t g) { elt (g) |= mask (g); dirty (); }
  فراغ add_bits (hb_codepoint_t g, uint64_t bits) { elt (g) |= bits; dirty (); }
  فراغ del (hb_codepoint_t g) { elt (g) &= ~mask (g); dirty (); }
  فراغ del_bits (hb_codepoint_t g, uint64_t bits) { elt (g) &= ~bits; dirty (); }
  فراغ set (hb_codepoint_t g, منطقي value) { ܐܢ (value) add (g); ܐܚܪܢܐ del (g); }
  منطقي get (hb_codepoint_t g) ثابت { ܦܢܐ elt (g) & mask (g); }
  منطقي may_have (hb_codepoint_t g) ثابت { ܦܢܐ get (g); }

  منطقي operator [] (hb_codepoint_t g) ثابت { ܦܢܐ get (g); }
  منطقي operator () (hb_codepoint_t g) ثابت { ܦܢܐ get (g); }
  منطقي has (hb_codepoint_t g) ثابت { ܦܢܐ get (g); }

  فراغ add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    ܐܢ (la == lb)
      *la |= (mask (b) << 1) - mask(a);
    ܐܚܪܢܐ ܐܢ (likely (la < lb))
    {
      *la |= ~(mask (a) - 1llu);
      la++;

      hb_memset (la, 0xff, (حرف *) lb - (حرف *) la);

      *lb |= ((mask (b) << 1) - 1llu);
    }
    ܐܚܪܢܐ
      ܦܢܐ;
    dirty ();
  }
  فراغ del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    ܐܢ (la == lb)
      *la &= ~((mask (b) << 1llu) - mask(a));
    ܐܚܪܢܐ ܐܢ (likely (la < lb))
    {
      *la &= mask (a) - 1;
      la++;

      hb_memset (la, 0, (حرف *) lb - (حرف *) la);

      *lb &= ~((mask (b) << 1) - 1llu);
    }
    ܐܚܪܢܐ
      ܦܢܐ;
    dirty ();
  }
  فراغ set_range (hb_codepoint_t a, hb_codepoint_t b, منطقي v)
  { ܐܢ (v) add_range (a, b); ܐܚܪܢܐ del_range (a, b); }


  // Writes out page values to the array p. Returns the number of values
  // written. At most size codepoints will be written.
  unsigned صحيح اكتب (uint32_t        base,
		      unsigned صحيح    start_value,
		      hb_codepoint_t *p,
		      unsigned صحيح    size) ثابت
  {
    unsigned صحيح start_v = start_value / ELT_BITS;
    unsigned صحيح start_bit = start_value & ELT_MASK;
    unsigned صحيح count = 0;
    لكل (unsigned i = start_v; i < len () && count < size; i++)
    {
      elt_t bits = v[i];
      uint32_t v_base = base | (i * ELT_BITS);
      لكل (unsigned صحيح j = start_bit; j < ELT_BITS && count < size; j++)
      {
	ܐܢ ((elt_t(1) << j) & bits) {
	  *p++ = v_base | j;
	  count++;
	}
      }
      start_bit = 0;
    }
    ܦܢܐ count;
  }

  // Writes out the values NOT in this page to the array p. Returns the
  // number of values written. At most size codepoints will be written.
  // Returns the number of codepoints written. next_value holds the next value
  // that should be written (if not present in this page). This is used to fill
  // any missing value gaps between this page and the previous page, if any.
  // next_value is updated to one more than the last value present in this page.
  unsigned صحيح write_inverted (uint32_t        base,
			       unsigned صحيح    start_value,
			       hb_codepoint_t *p,
			       unsigned صحيح    size,
			       hb_codepoint_t *next_value) ثابت
  {
    unsigned صحيح start_v = start_value / ELT_BITS;
    unsigned صحيح start_bit = start_value & ELT_MASK;
    unsigned صحيح count = 0;
    لكل (unsigned i = start_v; i < len () && count < size; i++)
    {
      elt_t bits = v[i];
      uint32_t v_offset = i * ELT_BITS;
      لكل (unsigned صحيح j = start_bit; j < ELT_BITS && count < size; j++)
      {
	ܐܢ ((elt_t(1) << j) & bits)
	{
	  hb_codepoint_t value = base | v_offset | j;
	  // Emit all the missing values from next_value up to value - 1.
	  لكل (hb_codepoint_t k = *next_value; k < value && count < size; k++)
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
    ܦܢܐ count;
  }

  منطقي operator == (ثابت hb_bit_page_t &other) ثابت { ܦܢܐ is_equal (other); }
  منطقي is_equal (ثابت hb_bit_page_t &other) ثابت { ܦܢܐ v == other.v; }
  منطقي intersects (ثابت hb_bit_page_t &other) ثابت
  {
    لكل (unsigned i = 0; i < len (); i++)
      ܐܢ (v[i] & other.v[i])
	ܦܢܐ ܫܪܪܐ;
    ܦܢܐ ܕܓܠܐ;
  }
  منطقي may_intersect (ثابت hb_bit_page_t &other) ثابت
  { ܦܢܐ intersects (other); }

  منطقي operator <= (ثابت hb_bit_page_t &larger_page) ثابت { ܦܢܐ is_subset (larger_page); }
  منطقي is_subset (ثابت hb_bit_page_t &larger_page) ثابت
  {
    ܐܢ (has_population () && larger_page.has_population () &&
	population > larger_page.population)
      ܦܢܐ ܕܓܠܐ;

    لكل (unsigned i = 0; i < len (); i++)
      ܐܢ (~larger_page.v[i] & v[i])
	ܦܢܐ ܕܓܠܐ;
    ܦܢܐ ܫܪܪܐ;
  }

  منطقي has_population () ثابت { ܦܢܐ population != UINT_MAX; }
  unsigned get_population () ثابت
  {
    ܐܢ (has_population ()) ܦܢܐ population;
    ܦܢܐ population = v;
  }

  منطقي next (hb_codepoint_t *codepoint) ثابت
  {
    unsigned صحيح m = (*codepoint + 1) & MASK;
    ܐܢ (!m)
    {
      *codepoint = INVALID;
      ܦܢܐ ܕܓܠܐ;
    }
    unsigned صحيح i = m / ELT_BITS;
    unsigned صحيح j = m & ELT_MASK;

    ثابت elt_t vv = v[i] & ~((elt_t (1) << j) - 1);
    لكل (ثابت elt_t *p = &vv; i < len (); p = ((ثابت elt_t *) &v[0]) + (++i))
      ܐܢ (*p)
      {
	*codepoint = i * ELT_BITS + elt_get_min (*p);
	ܦܢܐ ܫܪܪܐ;
      }

    *codepoint = INVALID;
    ܦܢܐ ܕܓܠܐ;
  }
  منطقي previous (hb_codepoint_t *codepoint) ثابت
  {
    unsigned صحيح m = (*codepoint - 1) & MASK;
    ܐܢ (m == MASK)
    {
      *codepoint = INVALID;
      ܦܢܐ ܕܓܠܐ;
    }
    unsigned صحيح i = m / ELT_BITS;
    unsigned صحيح j = m & ELT_MASK;

    /* Fancy mask to avoid shifting by elt_t bitsize, which is undefined. */
    ثابت elt_t mask = j < 8 * حجم (elt_t) - 1 ?
		       ((elt_t (1) << (j + 1)) - 1) :
		       (elt_t) -1;
    ثابت elt_t vv = v[i] & mask;
    ثابت elt_t *p = &vv;
    ܟܡܐ (ܫܪܪܐ)
    {
      ܐܢ (*p)
      {
	*codepoint = i * ELT_BITS + elt_get_max (*p);
	ܦܢܐ ܫܪܪܐ;
      }
      ܐܢ ((صحيح) i <= 0) اكسر;
      p = &v[--i];
    }

    *codepoint = INVALID;
    ܦܢܐ ܕܓܠܐ;
  }
  hb_codepoint_t get_min () ثابت
  {
    لكل (unsigned صحيح i = 0; i < len (); i++)
      ܐܢ (v[i])
	ܦܢܐ i * ELT_BITS + elt_get_min (v[i]);
    ܦܢܐ INVALID;
  }
  hb_codepoint_t get_max () ثابت
  {
    لكل (صحيح i = len () - 1; i >= 0; i--)
      ܐܢ (v[i])
	ܦܢܐ i * ELT_BITS + elt_get_max (v[i]);
    ܦܢܐ INVALID;
  }

  /*
   * Iterator implementation.
   */
  بنية iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ساكن constexpr منطقي is_sorted_iterator = ܫܪܪܐ;
    iter_t (ثابت hb_bit_page_t &s_ = Null (hb_bit_page_t), منطقي init = ܫܪܪܐ) : s (&s_), v (INVALID)
    {
      ܐܢ (init)
	v = s->get_min ();
    }

    تعريفنوع hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ثابت { ܦܢܐ v; }
    منطقي __more__ () ثابت { ܦܢܐ v != INVALID; }
    فراغ __next__ () {
       s->next (&v);
    }
    فراغ __prev__ () { s->previous (&v); }
    iter_t end () ثابت { ܦܢܐ iter_t (*s, ܕܓܠܐ); }
    منطقي operator != (ثابت iter_t& o) ثابت
    { ܦܢܐ v != o.v; }

    محمي:
    ثابت hb_bit_page_t *s;
    hb_codepoint_t v;
  };
  iter_t iter () ثابت { ܦܢܐ iter_t (*هذا); }
  operator iter_t () ثابت { ܦܢܐ iter (); }

  ساكن constexpr hb_codepoint_t INVALID = HB_SET_VALUE_INVALID;

  تعريفنوع unsigned long long elt_t;
  ساكن constexpr unsigned PAGE_BITS_LOG_2 = 9; // 512 bits
  ساكن constexpr unsigned PAGE_BITS = 1 << PAGE_BITS_LOG_2;
  static_assert (1 << PAGE_BITS_LOG_2 == PAGE_BITS, "");
  static_assert ((PAGE_BITS & ((PAGE_BITS) - 1)) == 0, "");
  ساكن constexpr unsigned PAGE_BITMASK = PAGE_BITS - 1;

  ساكن unsigned صحيح elt_get_min (ثابت elt_t &elt) { ܦܢܐ hb_ctz (elt); }
  ساكن unsigned صحيح elt_get_max (ثابت elt_t &elt) { ܦܢܐ hb_bit_storage (elt) - 1; }

  تعريفنوع hb_vector_size_t<elt_t, PAGE_BITS / 8> vector_t;

  ساكن constexpr unsigned ELT_BITS = حجم (elt_t) * 8;
  ساكن constexpr unsigned ELT_MASK = ELT_BITS - 1;

  ساكن constexpr unsigned BITS = حجم (vector_t) * 8;
  ساكن constexpr unsigned MASK = BITS - 1;
  static_assert ((unsigned) PAGE_BITS == (unsigned) BITS, "");

  elt_t &elt (hb_codepoint_t g) { ܦܢܐ v[(g & MASK) / ELT_BITS]; }
  ثابت elt_t& elt (hb_codepoint_t g) ثابت { ܦܢܐ v[(g & MASK) / ELT_BITS]; }
  ساكن constexpr elt_t mask (hb_codepoint_t g) { ܦܢܐ elt_t (1) << (g & ELT_MASK); }

  mutable unsigned population;
  vector_t v;
};


#endif /* HB_BIT_PAGE_HH */

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

#شامل "hb.hh"


/* Compiler-assisted vectorization. */

/* Type behaving similar to vectorized vars defined using __attribute__((vector_size(...))),
 * basically a fixed-size bitset. We can't use the compiler type because hb_vector_t cannot
 * guarantee alignment requirements. */
template <typename elt_t, unsigned صحیح byte_size>
ساختار hb_vector_size_t
{
  elt_t& operator [] (unsigned صحیح i) { بازگشت v[i]; }
  ثابت elt_t& operator [] (unsigned صحیح i) ثابت { بازگشت v[i]; }

  تهی init0 ()
  {
    برای (unsigned صحیح i = 0; i < ARRAY_LENGTH (v); i++)
      v[i] = 0;
  }
  تهی init1 ()
  {
    برای (unsigned صحیح i = 0; i < ARRAY_LENGTH (v); i++)
      v[i] = (elt_t) -1;
  }

  template <typename Op>
  hb_vector_size_t process (ثابت Op& op) ثابت
  {
    hb_vector_size_t r;
    برای (unsigned صحیح i = 0; i < ARRAY_LENGTH (v); i++)
      r.v[i] = op (v[i]);
    بازگشت r;
  }
  template <typename Op>
  hb_vector_size_t process (ثابت Op& op, ثابت hb_vector_size_t &o) ثابت
  {
    hb_vector_size_t r;
    برای (unsigned صحیح i = 0; i < ARRAY_LENGTH (v); i++)
      r.v[i] = op (v[i], o.v[i]);
    بازگشت r;
  }
  hb_vector_size_t operator | (ثابت hb_vector_size_t &o) ثابت
  { بازگشت process (hb_bitwise_or, o); }
  hb_vector_size_t operator & (ثابت hb_vector_size_t &o) ثابت
  { بازگشت process (hb_bitwise_and, o); }
  hb_vector_size_t operator ^ (ثابت hb_vector_size_t &o) ثابت
  { بازگشت process (hb_bitwise_xor, o); }
  hb_vector_size_t operator ~ () ثابت
  { بازگشت process (hb_bitwise_neg); }

  operator منطقی () ثابت
  {
    برای (unsigned صحیح i = 0; i < ARRAY_LENGTH (v); i++)
      اگر (v[i])
	بازگشت درست;
    بازگشت نادرست;
  }
  operator unsigned صحیح () ثابت
  {
    unsigned صحیح r = 0;
    برای (unsigned صحیح i = 0; i < ARRAY_LENGTH (v); i++)
      r += hb_popcount (v[i]);
    بازگشت r;
  }
  منطقی operator == (ثابت hb_vector_size_t &o) ثابت
  {
    برای (unsigned صحیح i = 0; i < ARRAY_LENGTH (v); i++)
      اگر (v[i] != o.v[i])
	بازگشت نادرست;
    بازگشت درست;
  }

  hb_array_t<ثابت elt_t> iter () ثابت
  { بازگشت hb_array (v); }

  خصوصی:
  static_assert (0 == byte_size % اندازه (elt_t), "");
  elt_t v[byte_size / اندازه (elt_t)];
};


ساختار hb_bit_page_t
{
  hb_bit_page_t () { init0 (); }

  تهی init0 () { v.init0 (); population = 0; }
  تهی init1 () { v.init1 (); population = PAGE_BITS; }

  تهی dirty () { population = UINT_MAX; }

  ایستا درونخطی constexpr unsigned len ()
  { بازگشت ARRAY_LENGTH_CONST (v); }

  operator منطقی () ثابت { بازگشت !is_empty (); }
  منطقی is_empty () ثابت
  {
    اگر (has_population ()) بازگشت !population;
    منطقی empty = !v;
    اگر (empty) population = 0;
    بازگشت empty;
  }
  uint32_t hash () ثابت
  {
    بازگشت hb_bytes_t ((ثابت نویسه *) &v, اندازه (v)).hash ();
  }

  تهی add (hb_codepoint_t g) { elt (g) |= mask (g); dirty (); }
  تهی add_bits (hb_codepoint_t g, uint64_t bits) { elt (g) |= bits; dirty (); }
  تهی del (hb_codepoint_t g) { elt (g) &= ~mask (g); dirty (); }
  تهی del_bits (hb_codepoint_t g, uint64_t bits) { elt (g) &= ~bits; dirty (); }
  تهی set (hb_codepoint_t g, منطقی value) { اگر (value) add (g); وگرنه del (g); }
  منطقی get (hb_codepoint_t g) ثابت { بازگشت elt (g) & mask (g); }
  منطقی may_have (hb_codepoint_t g) ثابت { بازگشت get (g); }

  منطقی operator [] (hb_codepoint_t g) ثابت { بازگشت get (g); }
  منطقی operator () (hb_codepoint_t g) ثابت { بازگشت get (g); }
  منطقی has (hb_codepoint_t g) ثابت { بازگشت get (g); }

  تهی add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    اگر (la == lb)
      *la |= (mask (b) << 1) - mask(a);
    وگرنه اگر (likely (la < lb))
    {
      *la |= ~(mask (a) - 1llu);
      la++;

      hb_memset (la, 0xff, (نویسه *) lb - (نویسه *) la);

      *lb |= ((mask (b) << 1) - 1llu);
    }
    وگرنه
      بازگشت;
    dirty ();
  }
  تهی del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    اگر (la == lb)
      *la &= ~((mask (b) << 1llu) - mask(a));
    وگرنه اگر (likely (la < lb))
    {
      *la &= mask (a) - 1;
      la++;

      hb_memset (la, 0, (نویسه *) lb - (نویسه *) la);

      *lb &= ~((mask (b) << 1) - 1llu);
    }
    وگرنه
      بازگشت;
    dirty ();
  }
  تهی set_range (hb_codepoint_t a, hb_codepoint_t b, منطقی v)
  { اگر (v) add_range (a, b); وگرنه del_range (a, b); }


  // Writes out page values to the array p. Returns the number of values
  // written. At most size codepoints will be written.
  unsigned صحیح اكتب (uint32_t        base,
		      unsigned صحیح    start_مقدار,
		      hb_codepoint_t *p,
		      unsigned صحیح    size) ثابت
  {
    unsigned صحیح start_v = start_مقدار / ELT_BITS;
    unsigned صحیح start_bit = start_مقدار & ELT_MASK;
    unsigned صحیح شمار = 0;
    برای (unsigned i = start_v; i < len () && شمار < size; i++)
    {
      elt_t bits = v[i];
      uint32_t v_base = base | (i * ELT_BITS);
      برای (unsigned صحیح j = start_bit; j < ELT_BITS && شمار < size; j++)
      {
	اگر ((elt_t(1) << j) & bits) {
	  *p++ = v_base | j;
	  شمار++;
	}
      }
      start_bit = 0;
    }
    بازگشت شمار;
  }

  // Writes out the values NOT in this page to the array p. Returns the
  // number of values written. At most size codepoints will be written.
  // Returns the number of codepoints written. next_value holds the next value
  // that should be written (if not present in this page). This is used to fill
  // any missing value gaps between this page and the previous page, if any.
  // next_value is updated to one more than the last value present in this page.
  unsigned صحیح write_inverted (uint32_t        base,
			       unsigned صحیح    start_مقدار,
			       hb_codepoint_t *p,
			       unsigned صحیح    size,
			       hb_codepoint_t *next_مقدار) ثابت
  {
    unsigned صحیح start_v = start_مقدار / ELT_BITS;
    unsigned صحیح start_bit = start_مقدار & ELT_MASK;
    unsigned صحیح شمار = 0;
    برای (unsigned i = start_v; i < len () && شمار < size; i++)
    {
      elt_t bits = v[i];
      uint32_t v_offset = i * ELT_BITS;
      برای (unsigned صحیح j = start_bit; j < ELT_BITS && شمار < size; j++)
      {
	اگر ((elt_t(1) << j) & bits)
	{
	  hb_codepoint_t value = base | v_offset | j;
	  // Emit all the missing values from next_value up to value - 1.
	  برای (hb_codepoint_t k = *next_مقدار; k < value && شمار < size; k++)
	  {
	    *p++ = k;
	    شمار++;
	  }
	  // Skip over this value;
	  *next_مقدار = value + 1;
	}
      }
      start_bit = 0;
    }
    بازگشت شمار;
  }

  منطقی operator == (ثابت hb_bit_page_t &other) ثابت { بازگشت is_equal (other); }
  منطقی is_equal (ثابت hb_bit_page_t &other) ثابت { بازگشت v == other.v; }
  منطقی intersects (ثابت hb_bit_page_t &other) ثابت
  {
    برای (unsigned i = 0; i < len (); i++)
      اگر (v[i] & other.v[i])
	بازگشت درست;
    بازگشت نادرست;
  }
  منطقی may_intersect (ثابت hb_bit_page_t &other) ثابت
  { بازگشت intersects (other); }

  منطقی operator <= (ثابت hb_bit_page_t &larger_page) ثابت { بازگشت is_subset (larger_page); }
  منطقی is_subset (ثابت hb_bit_page_t &larger_page) ثابت
  {
    اگر (has_population () && larger_page.has_population () &&
	population > larger_page.population)
      بازگشت نادرست;

    برای (unsigned i = 0; i < len (); i++)
      اگر (~larger_page.v[i] & v[i])
	بازگشت نادرست;
    بازگشت درست;
  }

  منطقی has_population () ثابت { بازگشت population != UINT_MAX; }
  unsigned get_population () ثابت
  {
    اگر (has_population ()) بازگشت population;
    بازگشت population = v;
  }

  منطقی next (hb_codepoint_t *codepoint) ثابت
  {
    unsigned صحیح m = (*codepoint + 1) & MASK;
    اگر (!m)
    {
      *codepoint = INVALID;
      بازگشت نادرست;
    }
    unsigned صحیح i = m / ELT_BITS;
    unsigned صحیح j = m & ELT_MASK;

    ثابت elt_t vv = v[i] & ~((elt_t (1) << j) - 1);
    برای (ثابت elt_t *p = &vv; i < len (); p = ((ثابت elt_t *) &v[0]) + (++i))
      اگر (*p)
      {
	*codepoint = i * ELT_BITS + elt_get_min (*p);
	بازگشت درست;
      }

    *codepoint = INVALID;
    بازگشت نادرست;
  }
  منطقی previous (hb_codepoint_t *codepoint) ثابت
  {
    unsigned صحیح m = (*codepoint - 1) & MASK;
    اگر (m == MASK)
    {
      *codepoint = INVALID;
      بازگشت نادرست;
    }
    unsigned صحیح i = m / ELT_BITS;
    unsigned صحیح j = m & ELT_MASK;

    /* Fancy mask to avoid shifting by elt_t bitsize, which is undefined. */
    ثابت elt_t mask = j < 8 * اندازه (elt_t) - 1 ?
		       ((elt_t (1) << (j + 1)) - 1) :
		       (elt_t) -1;
    ثابت elt_t vv = v[i] & mask;
    ثابت elt_t *p = &vv;
    وقتی (درست)
    {
      اگر (*p)
      {
	*codepoint = i * ELT_BITS + elt_get_max (*p);
	بازگشت درست;
      }
      اگر ((صحیح) i <= 0) بشکن;
      p = &v[--i];
    }

    *codepoint = INVALID;
    بازگشت نادرست;
  }
  hb_codepoint_t get_min () ثابت
  {
    برای (unsigned صحیح i = 0; i < len (); i++)
      اگر (v[i])
	بازگشت i * ELT_BITS + elt_get_min (v[i]);
    بازگشت INVALID;
  }
  hb_codepoint_t get_max () ثابت
  {
    برای (صحیح i = len () - 1; i >= 0; i--)
      اگر (v[i])
	بازگشت i * ELT_BITS + elt_get_max (v[i]);
    بازگشت INVALID;
  }

  /*
   * Iterator implementation.
   */
  ساختار iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ایستا constexpr منطقی is_sorted_iterator = درست;
    iter_t (ثابت hb_bit_page_t &s_ = Null (hb_bit_page_t), منطقی init = درست) : s (&s_), v (INVALID)
    {
      اگر (init)
	v = s->get_min ();
    }

    تعریفنوع hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ثابت { بازگشت v; }
    منطقی __more__ () ثابت { بازگشت v != INVALID; }
    تهی __next__ () {
       s->next (&v);
    }
    تهی __prev__ () { s->previous (&v); }
    iter_t end () ثابت { بازگشت iter_t (*s, نادرست); }
    منطقی operator != (ثابت iter_t& o) ثابت
    { بازگشت v != o.v; }

    محافظتشده:
    ثابت hb_bit_page_t *s;
    hb_codepoint_t v;
  };
  iter_t iter () ثابت { بازگشت iter_t (*این); }
  operator iter_t () ثابت { بازگشت iter (); }

  ایستا constexpr hb_codepoint_t INVALID = HB_SET_مقدار_INVALID;

  تعریفنوع unsigned long long elt_t;
  ایستا constexpr unsigned PAGE_BITS_LOG_2 = 9; // 512 bits
  ایستا constexpr unsigned PAGE_BITS = 1 << PAGE_BITS_LOG_2;
  static_assert (1 << PAGE_BITS_LOG_2 == PAGE_BITS, "");
  static_assert ((PAGE_BITS & ((PAGE_BITS) - 1)) == 0, "");
  ایستا constexpr unsigned PAGE_BITMASK = PAGE_BITS - 1;

  ایستا unsigned صحیح elt_get_min (ثابت elt_t &elt) { بازگشت hb_ctz (elt); }
  ایستا unsigned صحیح elt_get_max (ثابت elt_t &elt) { بازگشت hb_bit_storage (elt) - 1; }

  تعریفنوع hb_vector_size_t<elt_t, PAGE_BITS / 8> vector_t;

  ایستا constexpr unsigned ELT_BITS = اندازه (elt_t) * 8;
  ایستا constexpr unsigned ELT_MASK = ELT_BITS - 1;

  ایستا constexpr unsigned BITS = اندازه (vector_t) * 8;
  ایستا constexpr unsigned MASK = BITS - 1;
  static_assert ((unsigned) PAGE_BITS == (unsigned) BITS, "");

  elt_t &elt (hb_codepoint_t g) { بازگشت v[(g & MASK) / ELT_BITS]; }
  ثابت elt_t& elt (hb_codepoint_t g) ثابت { بازگشت v[(g & MASK) / ELT_BITS]; }
  ایستا constexpr elt_t mask (hb_codepoint_t g) { بازگشت elt_t (1) << (g & ELT_MASK); }

  mutable unsigned population;
  vector_t v;
};


#endif /* HB_BIT_PAGE_HH */

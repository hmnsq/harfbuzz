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

#அந்தர்பாவ "hb.hh"


/* Compiler-assisted vectorization. */

/* Type behaving similar to vectorized vars defined using __attribute__((vector_size(...))),
 * basically a fixed-size bitset. We can't use the compiler type because hb_vector_t cannot
 * guarantee alignment requirements. */
template <typename elt_t, unsigned முழு byte_size>
கட்டமைப்பு hb_vector_size_t
{
  elt_t& operator [] (unsigned முழு i) { திரும்பு v[i]; }
  மாறிலி elt_t& operator [] (unsigned முழு i) மாறிலி { திரும்பு v[i]; }

  வெறுமை init0 ()
  {
    க்கு (unsigned முழு i = 0; i < ARRAY_LENGTH (v); i++)
      v[i] = 0;
  }
  வெறுமை init1 ()
  {
    க்கு (unsigned முழு i = 0; i < ARRAY_LENGTH (v); i++)
      v[i] = (elt_t) -1;
  }

  template <typename Op>
  hb_vector_size_t process (மாறிலி Op& op) மாறிலி
  {
    hb_vector_size_t r;
    க்கு (unsigned முழு i = 0; i < ARRAY_LENGTH (v); i++)
      r.v[i] = op (v[i]);
    திரும்பு r;
  }
  template <typename Op>
  hb_vector_size_t process (மாறிலி Op& op, மாறிலி hb_vector_size_t &o) மாறிலி
  {
    hb_vector_size_t r;
    க்கு (unsigned முழு i = 0; i < ARRAY_LENGTH (v); i++)
      r.v[i] = op (v[i], o.v[i]);
    திரும்பு r;
  }
  hb_vector_size_t operator | (மாறிலி hb_vector_size_t &o) மாறிலி
  { திரும்பு process (hb_bitwise_or, o); }
  hb_vector_size_t operator & (மாறிலி hb_vector_size_t &o) மாறிலி
  { திரும்பு process (hb_bitwise_and, o); }
  hb_vector_size_t operator ^ (மாறிலி hb_vector_size_t &o) மாறிலி
  { திரும்பு process (hb_bitwise_xor, o); }
  hb_vector_size_t operator ~ () மாறிலி
  { திரும்பு process (hb_bitwise_neg); }

  operator தருக்க () மாறிலி
  {
    க்கு (unsigned முழு i = 0; i < ARRAY_LENGTH (v); i++)
      எனில் (v[i])
	திரும்பு உண்மை;
    திரும்பு பொய்;
  }
  operator unsigned முழு () மாறிலி
  {
    unsigned முழு r = 0;
    க்கு (unsigned முழு i = 0; i < ARRAY_LENGTH (v); i++)
      r += hb_popcount (v[i]);
    திரும்பு r;
  }
  தருக்க operator == (மாறிலி hb_vector_size_t &o) மாறிலி
  {
    க்கு (unsigned முழு i = 0; i < ARRAY_LENGTH (v); i++)
      எனில் (v[i] != o.v[i])
	திரும்பு பொய்;
    திரும்பு உண்மை;
  }

  hb_array_t<மாறிலி elt_t> iter () மாறிலி
  { திரும்பு hb_array (v); }

  தனிப்பட்ட:
  static_assert (0 == byte_size % அளவு (elt_t), "");
  elt_t v[byte_size / அளவு (elt_t)];
};


கட்டமைப்பு hb_bit_page_t
{
  hb_bit_page_t () { init0 (); }

  வெறுமை init0 () { v.init0 (); population = 0; }
  வெறுமை init1 () { v.init1 (); population = PAGE_BITS; }

  வெறுமை dirty () { population = UINT_MAX; }

  நிலையான உள்வரி constexpr unsigned len ()
  { திரும்பு ARRAY_LENGTH_CONST (v); }

  operator தருக்க () மாறிலி { திரும்பு !is_empty (); }
  தருக்க is_empty () மாறிலி
  {
    எனில் (has_population ()) திரும்பு !population;
    தருக்க empty = !v;
    எனில் (empty) population = 0;
    திரும்பு empty;
  }
  uint32_t hash () மாறிலி
  {
    திரும்பு hb_bytes_t ((மாறிலி எழுத்து *) &v, அளவு (v)).hash ();
  }

  வெறுமை add (hb_codepoint_t g) { elt (g) |= mask (g); dirty (); }
  வெறுமை add_bits (hb_codepoint_t g, uint64_t bits) { elt (g) |= bits; dirty (); }
  வெறுமை del (hb_codepoint_t g) { elt (g) &= ~mask (g); dirty (); }
  வெறுமை del_bits (hb_codepoint_t g, uint64_t bits) { elt (g) &= ~bits; dirty (); }
  வெறுமை set (hb_codepoint_t g, தருக்க value) { எனில் (value) add (g); இல்லையெனில் del (g); }
  தருக்க get (hb_codepoint_t g) மாறிலி { திரும்பு elt (g) & mask (g); }
  தருக்க may_have (hb_codepoint_t g) மாறிலி { திரும்பு get (g); }

  தருக்க operator [] (hb_codepoint_t g) மாறிலி { திரும்பு get (g); }
  தருக்க operator () (hb_codepoint_t g) மாறிலி { திரும்பு get (g); }
  தருக்க has (hb_codepoint_t g) மாறிலி { திரும்பு get (g); }

  வெறுமை add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    எனில் (la == lb)
      *la |= (mask (b) << 1) - mask(a);
    இல்லையெனில் எனில் (likely (la < lb))
    {
      *la |= ~(mask (a) - 1llu);
      la++;

      hb_memset (la, 0xff, (எழுத்து *) lb - (எழுத்து *) la);

      *lb |= ((mask (b) << 1) - 1llu);
    }
    இல்லையெனில்
      திரும்பு;
    dirty ();
  }
  வெறுமை del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    எனில் (la == lb)
      *la &= ~((mask (b) << 1llu) - mask(a));
    இல்லையெனில் எனில் (likely (la < lb))
    {
      *la &= mask (a) - 1;
      la++;

      hb_memset (la, 0, (எழுத்து *) lb - (எழுத்து *) la);

      *lb &= ~((mask (b) << 1) - 1llu);
    }
    இல்லையெனில்
      திரும்பு;
    dirty ();
  }
  வெறுமை set_range (hb_codepoint_t a, hb_codepoint_t b, தருக்க v)
  { எனில் (v) add_range (a, b); இல்லையெனில் del_range (a, b); }


  // Writes out page values to the array p. Returns the number of values
  // written. At most size codepoints will be written.
  unsigned முழு லிக (uint32_t        base,
		      unsigned முழு    start_மதிப்பு,
		      hb_codepoint_t *p,
		      unsigned முழு    size) மாறிலி
  {
    unsigned முழு start_v = start_மதிப்பு / ELT_BITS;
    unsigned முழு start_bit = start_மதிப்பு & ELT_MASK;
    unsigned முழு மொத்தம் = 0;
    க்கு (unsigned i = start_v; i < len () && மொத்தம் < size; i++)
    {
      elt_t bits = v[i];
      uint32_t v_base = base | (i * ELT_BITS);
      க்கு (unsigned முழு j = start_bit; j < ELT_BITS && மொத்தம் < size; j++)
      {
	எனில் ((elt_t(1) << j) & bits) {
	  *p++ = v_base | j;
	  மொத்தம்++;
	}
      }
      start_bit = 0;
    }
    திரும்பு மொத்தம்;
  }

  // Writes out the values NOT in this page to the array p. Returns the
  // number of values written. At most size codepoints will be written.
  // Returns the number of codepoints written. next_value holds the next value
  // that should be written (if not present in this page). This is used to fill
  // any missing value gaps between this page and the previous page, if any.
  // next_value is updated to one more than the last value present in this page.
  unsigned முழு write_inverted (uint32_t        base,
			       unsigned முழு    start_மதிப்பு,
			       hb_codepoint_t *p,
			       unsigned முழு    size,
			       hb_codepoint_t *next_மதிப்பு) மாறிலி
  {
    unsigned முழு start_v = start_மதிப்பு / ELT_BITS;
    unsigned முழு start_bit = start_மதிப்பு & ELT_MASK;
    unsigned முழு மொத்தம் = 0;
    க்கு (unsigned i = start_v; i < len () && மொத்தம் < size; i++)
    {
      elt_t bits = v[i];
      uint32_t v_offset = i * ELT_BITS;
      க்கு (unsigned முழு j = start_bit; j < ELT_BITS && மொத்தம் < size; j++)
      {
	எனில் ((elt_t(1) << j) & bits)
	{
	  hb_codepoint_t value = base | v_offset | j;
	  // Emit all the missing values from next_value up to value - 1.
	  க்கு (hb_codepoint_t k = *next_மதிப்பு; k < value && மொத்தம் < size; k++)
	  {
	    *p++ = k;
	    மொத்தம்++;
	  }
	  // Skip over this value;
	  *next_மதிப்பு = value + 1;
	}
      }
      start_bit = 0;
    }
    திரும்பு மொத்தம்;
  }

  தருக்க operator == (மாறிலி hb_bit_page_t &other) மாறிலி { திரும்பு is_equal (other); }
  தருக்க is_equal (மாறிலி hb_bit_page_t &other) மாறிலி { திரும்பு v == other.v; }
  தருக்க intersects (மாறிலி hb_bit_page_t &other) மாறிலி
  {
    க்கு (unsigned i = 0; i < len (); i++)
      எனில் (v[i] & other.v[i])
	திரும்பு உண்மை;
    திரும்பு பொய்;
  }
  தருக்க may_intersect (மாறிலி hb_bit_page_t &other) மாறிலி
  { திரும்பு intersects (other); }

  தருக்க operator <= (மாறிலி hb_bit_page_t &larger_page) மாறிலி { திரும்பு is_subset (larger_page); }
  தருக்க is_subset (மாறிலி hb_bit_page_t &larger_page) மாறிலி
  {
    எனில் (has_population () && larger_page.has_population () &&
	population > larger_page.population)
      திரும்பு பொய்;

    க்கு (unsigned i = 0; i < len (); i++)
      எனில் (~larger_page.v[i] & v[i])
	திரும்பு பொய்;
    திரும்பு உண்மை;
  }

  தருக்க has_population () மாறிலி { திரும்பு population != UINT_MAX; }
  unsigned get_population () மாறிலி
  {
    எனில் (has_population ()) திரும்பு population;
    திரும்பு population = v;
  }

  தருக்க next (hb_codepoint_t *codepoint) மாறிலி
  {
    unsigned முழு m = (*codepoint + 1) & MASK;
    எனில் (!m)
    {
      *codepoint = INVALID;
      திரும்பு பொய்;
    }
    unsigned முழு i = m / ELT_BITS;
    unsigned முழு j = m & ELT_MASK;

    மாறிலி elt_t vv = v[i] & ~((elt_t (1) << j) - 1);
    க்கு (மாறிலி elt_t *p = &vv; i < len (); p = ((மாறிலி elt_t *) &v[0]) + (++i))
      எனில் (*p)
      {
	*codepoint = i * ELT_BITS + elt_get_min (*p);
	திரும்பு உண்மை;
      }

    *codepoint = INVALID;
    திரும்பு பொய்;
  }
  தருக்க previous (hb_codepoint_t *codepoint) மாறிலி
  {
    unsigned முழு m = (*codepoint - 1) & MASK;
    எனில் (m == MASK)
    {
      *codepoint = INVALID;
      திரும்பு பொய்;
    }
    unsigned முழு i = m / ELT_BITS;
    unsigned முழு j = m & ELT_MASK;

    /* Fancy mask to avoid shifting by elt_t bitsize, which is undefined. */
    மாறிலி elt_t mask = j < 8 * அளவு (elt_t) - 1 ?
		       ((elt_t (1) << (j + 1)) - 1) :
		       (elt_t) -1;
    மாறிலி elt_t vv = v[i] & mask;
    மாறிலி elt_t *p = &vv;
    போது (உண்மை)
    {
      எனில் (*p)
      {
	*codepoint = i * ELT_BITS + elt_get_max (*p);
	திரும்பு உண்மை;
      }
      எனில் ((முழு) i <= 0) நிறுத்து;
      p = &v[--i];
    }

    *codepoint = INVALID;
    திரும்பு பொய்;
  }
  hb_codepoint_t get_min () மாறிலி
  {
    க்கு (unsigned முழு i = 0; i < len (); i++)
      எனில் (v[i])
	திரும்பு i * ELT_BITS + elt_get_min (v[i]);
    திரும்பு INVALID;
  }
  hb_codepoint_t get_max () மாறிலி
  {
    க்கு (முழு i = len () - 1; i >= 0; i--)
      எனில் (v[i])
	திரும்பு i * ELT_BITS + elt_get_max (v[i]);
    திரும்பு INVALID;
  }

  /*
   * Iterator implementation.
   */
  கட்டமைப்பு iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    நிலையான constexpr தருக்க is_sorted_iterator = உண்மை;
    iter_t (மாறிலி hb_bit_page_t &s_ = Null (hb_bit_page_t), தருக்க init = உண்மை) : s (&s_), v (INVALID)
    {
      எனில் (init)
	v = s->get_min ();
    }

    வகைவரை hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () மாறிலி { திரும்பு v; }
    தருக்க __more__ () மாறிலி { திரும்பு v != INVALID; }
    வெறுமை __next__ () {
       s->next (&v);
    }
    வெறுமை __prev__ () { s->previous (&v); }
    iter_t end () மாறிலி { திரும்பு iter_t (*s, பொய்); }
    தருக்க operator != (மாறிலி iter_t& o) மாறிலி
    { திரும்பு v != o.v; }

    பாதுகாக்கப்பட்ட:
    மாறிலி hb_bit_page_t *s;
    hb_codepoint_t v;
  };
  iter_t iter () மாறிலி { திரும்பு iter_t (*இது); }
  operator iter_t () மாறிலி { திரும்பு iter (); }

  நிலையான constexpr hb_codepoint_t INVALID = HB_SET_மதிப்பு_INVALID;

  வகைவரை unsigned long long elt_t;
  நிலையான constexpr unsigned PAGE_BITS_LOG_2 = 9; // 512 bits
  நிலையான constexpr unsigned PAGE_BITS = 1 << PAGE_BITS_LOG_2;
  static_assert (1 << PAGE_BITS_LOG_2 == PAGE_BITS, "");
  static_assert ((PAGE_BITS & ((PAGE_BITS) - 1)) == 0, "");
  நிலையான constexpr unsigned PAGE_BITMASK = PAGE_BITS - 1;

  நிலையான unsigned முழு elt_get_min (மாறிலி elt_t &elt) { திரும்பு hb_ctz (elt); }
  நிலையான unsigned முழு elt_get_max (மாறிலி elt_t &elt) { திரும்பு hb_bit_storage (elt) - 1; }

  வகைவரை hb_vector_size_t<elt_t, PAGE_BITS / 8> vector_t;

  நிலையான constexpr unsigned ELT_BITS = அளவு (elt_t) * 8;
  நிலையான constexpr unsigned ELT_MASK = ELT_BITS - 1;

  நிலையான constexpr unsigned BITS = அளவு (vector_t) * 8;
  நிலையான constexpr unsigned MASK = BITS - 1;
  static_assert ((unsigned) PAGE_BITS == (unsigned) BITS, "");

  elt_t &elt (hb_codepoint_t g) { திரும்பு v[(g & MASK) / ELT_BITS]; }
  மாறிலி elt_t& elt (hb_codepoint_t g) மாறிலி { திரும்பு v[(g & MASK) / ELT_BITS]; }
  நிலையான constexpr elt_t mask (hb_codepoint_t g) { திரும்பு elt_t (1) << (g & ELT_MASK); }

  mutable unsigned population;
  vector_t v;
};


#endif /* HB_BIT_PAGE_HH */

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

#अन्तर्भाव "hb.hh"


/* Compiler-assisted vectorization. */

/* Type behaving similar to vectorized vars defined using __attribute__((vector_size(...))),
 * basically a fixed-size bitset. We can't use the compiler type because hb_vector_t cannot
 * guarantee alignment requirements. */
template <typename elt_t, unsigned पूर्णाङ्क byte_size>
संरचना hb_vector_size_t
{
  elt_t& operator [] (unsigned पूर्णाङ्क i) { प्रत्यागम v[i]; }
  ध्रुव elt_t& operator [] (unsigned पूर्णाङ्क i) ध्रुव { प्रत्यागम v[i]; }

  शून्य init0 ()
  {
    प्रति (unsigned पूर्णाङ्क i = 0; i < ARRAY_LENGTH (v); i++)
      v[i] = 0;
  }
  शून्य init1 ()
  {
    प्रति (unsigned पूर्णाङ्क i = 0; i < ARRAY_LENGTH (v); i++)
      v[i] = (elt_t) -1;
  }

  template <typename Op>
  hb_vector_size_t process (ध्रुव Op& op) ध्रुव
  {
    hb_vector_size_t r;
    प्रति (unsigned पूर्णाङ्क i = 0; i < ARRAY_LENGTH (v); i++)
      r.v[i] = op (v[i]);
    प्रत्यागम r;
  }
  template <typename Op>
  hb_vector_size_t process (ध्रुव Op& op, ध्रुव hb_vector_size_t &o) ध्रुव
  {
    hb_vector_size_t r;
    प्रति (unsigned पूर्णाङ्क i = 0; i < ARRAY_LENGTH (v); i++)
      r.v[i] = op (v[i], o.v[i]);
    प्रत्यागम r;
  }
  hb_vector_size_t operator | (ध्रुव hb_vector_size_t &o) ध्रुव
  { प्रत्यागम process (hb_bitwise_or, o); }
  hb_vector_size_t operator & (ध्रुव hb_vector_size_t &o) ध्रुव
  { प्रत्यागम process (hb_bitwise_and, o); }
  hb_vector_size_t operator ^ (ध्रुव hb_vector_size_t &o) ध्रुव
  { प्रत्यागम process (hb_bitwise_xor, o); }
  hb_vector_size_t operator ~ () ध्रुव
  { प्रत्यागम process (hb_bitwise_neg); }

  operator तार्किक () ध्रुव
  {
    प्रति (unsigned पूर्णाङ्क i = 0; i < ARRAY_LENGTH (v); i++)
      यदि (v[i])
	प्रत्यागम सत्य;
    प्रत्यागम असत्य;
  }
  operator unsigned पूर्णाङ्क () ध्रुव
  {
    unsigned पूर्णाङ्क r = 0;
    प्रति (unsigned पूर्णाङ्क i = 0; i < ARRAY_LENGTH (v); i++)
      r += hb_popcount (v[i]);
    प्रत्यागम r;
  }
  तार्किक operator == (ध्रुव hb_vector_size_t &o) ध्रुव
  {
    प्रति (unsigned पूर्णाङ्क i = 0; i < ARRAY_LENGTH (v); i++)
      यदि (v[i] != o.v[i])
	प्रत्यागम असत्य;
    प्रत्यागम सत्य;
  }

  hb_array_t<ध्रुव elt_t> iter () ध्रुव
  { प्रत्यागम hb_array (v); }

  गुप्त:
  static_assert (0 == byte_size % परिमाण (elt_t), "");
  elt_t v[byte_size / परिमाण (elt_t)];
};


संरचना hb_bit_page_t
{
  hb_bit_page_t () { init0 (); }

  शून्य init0 () { v.init0 (); population = 0; }
  शून्य init1 () { v.init1 (); population = PAGE_BITS; }

  शून्य dirty () { population = UINT_MAX; }

  स्थिर अन्तःस्थ constexpr unsigned len ()
  { प्रत्यागम ARRAY_LENGTH_CONST (v); }

  operator तार्किक () ध्रुव { प्रत्यागम !is_empty (); }
  तार्किक is_empty () ध्रुव
  {
    यदि (has_population ()) प्रत्यागम !population;
    तार्किक empty = !v;
    यदि (empty) population = 0;
    प्रत्यागम empty;
  }
  uint32_t hash () ध्रुव
  {
    प्रत्यागम hb_bytes_t ((ध्रुव वर्ण *) &v, परिमाण (v)).hash ();
  }

  शून्य add (hb_codepoint_t g) { elt (g) |= mask (g); dirty (); }
  शून्य add_bits (hb_codepoint_t g, uint64_t bits) { elt (g) |= bits; dirty (); }
  शून्य del (hb_codepoint_t g) { elt (g) &= ~mask (g); dirty (); }
  शून्य del_bits (hb_codepoint_t g, uint64_t bits) { elt (g) &= ~bits; dirty (); }
  शून्य set (hb_codepoint_t g, तार्किक value) { यदि (value) add (g); अन्यथा del (g); }
  तार्किक get (hb_codepoint_t g) ध्रुव { प्रत्यागम elt (g) & mask (g); }
  तार्किक may_have (hb_codepoint_t g) ध्रुव { प्रत्यागम get (g); }

  तार्किक operator [] (hb_codepoint_t g) ध्रुव { प्रत्यागम get (g); }
  तार्किक operator () (hb_codepoint_t g) ध्रुव { प्रत्यागम get (g); }
  तार्किक has (hb_codepoint_t g) ध्रुव { प्रत्यागम get (g); }

  शून्य add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    यदि (la == lb)
      *la |= (mask (b) << 1) - mask(a);
    अन्यथा यदि (likely (la < lb))
    {
      *la |= ~(mask (a) - 1llu);
      la++;

      hb_memset (la, 0xff, (वर्ण *) lb - (वर्ण *) la);

      *lb |= ((mask (b) << 1) - 1llu);
    }
    अन्यथा
      प्रत्यागम;
    dirty ();
  }
  शून्य del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    यदि (la == lb)
      *la &= ~((mask (b) << 1llu) - mask(a));
    अन्यथा यदि (likely (la < lb))
    {
      *la &= mask (a) - 1;
      la++;

      hb_memset (la, 0, (वर्ण *) lb - (वर्ण *) la);

      *lb &= ~((mask (b) << 1) - 1llu);
    }
    अन्यथा
      प्रत्यागम;
    dirty ();
  }
  शून्य set_range (hb_codepoint_t a, hb_codepoint_t b, तार्किक v)
  { यदि (v) add_range (a, b); अन्यथा del_range (a, b); }


  // Writes out page values to the array p. Returns the number of values
  // written. At most size codepoints will be written.
  unsigned पूर्णाङ्क लिख (uint32_t        base,
		      unsigned पूर्णाङ्क    start_मूल्य,
		      hb_codepoint_t *p,
		      unsigned पूर्णाङ्क    size) ध्रुव
  {
    unsigned पूर्णाङ्क start_v = start_मूल्य / ELT_BITS;
    unsigned पूर्णाङ्क start_bit = start_मूल्य & ELT_MASK;
    unsigned पूर्णाङ्क मोजणी = 0;
    प्रति (unsigned i = start_v; i < len () && मोजणी < size; i++)
    {
      elt_t bits = v[i];
      uint32_t v_base = base | (i * ELT_BITS);
      प्रति (unsigned पूर्णाङ्क j = start_bit; j < ELT_BITS && मोजणी < size; j++)
      {
	यदि ((elt_t(1) << j) & bits) {
	  *p++ = v_base | j;
	  मोजणी++;
	}
      }
      start_bit = 0;
    }
    प्रत्यागम मोजणी;
  }

  // Writes out the values NOT in this page to the array p. Returns the
  // number of values written. At most size codepoints will be written.
  // Returns the number of codepoints written. next_value holds the next value
  // that should be written (if not present in this page). This is used to fill
  // any missing value gaps between this page and the previous page, if any.
  // next_value is updated to one more than the last value present in this page.
  unsigned पूर्णाङ्क write_inverted (uint32_t        base,
			       unsigned पूर्णाङ्क    start_मूल्य,
			       hb_codepoint_t *p,
			       unsigned पूर्णाङ्क    size,
			       hb_codepoint_t *next_मूल्य) ध्रुव
  {
    unsigned पूर्णाङ्क start_v = start_मूल्य / ELT_BITS;
    unsigned पूर्णाङ्क start_bit = start_मूल्य & ELT_MASK;
    unsigned पूर्णाङ्क मोजणी = 0;
    प्रति (unsigned i = start_v; i < len () && मोजणी < size; i++)
    {
      elt_t bits = v[i];
      uint32_t v_offset = i * ELT_BITS;
      प्रति (unsigned पूर्णाङ्क j = start_bit; j < ELT_BITS && मोजणी < size; j++)
      {
	यदि ((elt_t(1) << j) & bits)
	{
	  hb_codepoint_t value = base | v_offset | j;
	  // Emit all the missing values from next_value up to value - 1.
	  प्रति (hb_codepoint_t k = *next_मूल्य; k < value && मोजणी < size; k++)
	  {
	    *p++ = k;
	    मोजणी++;
	  }
	  // Skip over this value;
	  *next_मूल्य = value + 1;
	}
      }
      start_bit = 0;
    }
    प्रत्यागम मोजणी;
  }

  तार्किक operator == (ध्रुव hb_bit_page_t &other) ध्रुव { प्रत्यागम is_equal (other); }
  तार्किक is_equal (ध्रुव hb_bit_page_t &other) ध्रुव { प्रत्यागम v == other.v; }
  तार्किक intersects (ध्रुव hb_bit_page_t &other) ध्रुव
  {
    प्रति (unsigned i = 0; i < len (); i++)
      यदि (v[i] & other.v[i])
	प्रत्यागम सत्य;
    प्रत्यागम असत्य;
  }
  तार्किक may_intersect (ध्रुव hb_bit_page_t &other) ध्रुव
  { प्रत्यागम intersects (other); }

  तार्किक operator <= (ध्रुव hb_bit_page_t &larger_page) ध्रुव { प्रत्यागम is_subset (larger_page); }
  तार्किक is_subset (ध्रुव hb_bit_page_t &larger_page) ध्रुव
  {
    यदि (has_population () && larger_page.has_population () &&
	population > larger_page.population)
      प्रत्यागम असत्य;

    प्रति (unsigned i = 0; i < len (); i++)
      यदि (~larger_page.v[i] & v[i])
	प्रत्यागम असत्य;
    प्रत्यागम सत्य;
  }

  तार्किक has_population () ध्रुव { प्रत्यागम population != UINT_MAX; }
  unsigned get_population () ध्रुव
  {
    यदि (has_population ()) प्रत्यागम population;
    प्रत्यागम population = v;
  }

  तार्किक next (hb_codepoint_t *codepoint) ध्रुव
  {
    unsigned पूर्णाङ्क m = (*codepoint + 1) & MASK;
    यदि (!m)
    {
      *codepoint = INVALID;
      प्रत्यागम असत्य;
    }
    unsigned पूर्णाङ्क i = m / ELT_BITS;
    unsigned पूर्णाङ्क j = m & ELT_MASK;

    ध्रुव elt_t vv = v[i] & ~((elt_t (1) << j) - 1);
    प्रति (ध्रुव elt_t *p = &vv; i < len (); p = ((ध्रुव elt_t *) &v[0]) + (++i))
      यदि (*p)
      {
	*codepoint = i * ELT_BITS + elt_get_min (*p);
	प्रत्यागम सत्य;
      }

    *codepoint = INVALID;
    प्रत्यागम असत्य;
  }
  तार्किक previous (hb_codepoint_t *codepoint) ध्रुव
  {
    unsigned पूर्णाङ्क m = (*codepoint - 1) & MASK;
    यदि (m == MASK)
    {
      *codepoint = INVALID;
      प्रत्यागम असत्य;
    }
    unsigned पूर्णाङ्क i = m / ELT_BITS;
    unsigned पूर्णाङ्क j = m & ELT_MASK;

    /* Fancy mask to avoid shifting by elt_t bitsize, which is undefined. */
    ध्रुव elt_t mask = j < 8 * परिमाण (elt_t) - 1 ?
		       ((elt_t (1) << (j + 1)) - 1) :
		       (elt_t) -1;
    ध्रुव elt_t vv = v[i] & mask;
    ध्रुव elt_t *p = &vv;
    यावत् (सत्य)
    {
      यदि (*p)
      {
	*codepoint = i * ELT_BITS + elt_get_max (*p);
	प्रत्यागम सत्य;
      }
      यदि ((पूर्णाङ्क) i <= 0) भङ्ग;
      p = &v[--i];
    }

    *codepoint = INVALID;
    प्रत्यागम असत्य;
  }
  hb_codepoint_t get_min () ध्रुव
  {
    प्रति (unsigned पूर्णाङ्क i = 0; i < len (); i++)
      यदि (v[i])
	प्रत्यागम i * ELT_BITS + elt_get_min (v[i]);
    प्रत्यागम INVALID;
  }
  hb_codepoint_t get_max () ध्रुव
  {
    प्रति (पूर्णाङ्क i = len () - 1; i >= 0; i--)
      यदि (v[i])
	प्रत्यागम i * ELT_BITS + elt_get_max (v[i]);
    प्रत्यागम INVALID;
  }

  /*
   * Iterator implementation.
   */
  संरचना iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    स्थिर constexpr तार्किक is_sorted_iterator = सत्य;
    iter_t (ध्रुव hb_bit_page_t &s_ = Null (hb_bit_page_t), तार्किक init = सत्य) : s (&s_), v (INVALID)
    {
      यदि (init)
	v = s->get_min ();
    }

    प्रकारनिर्देश hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ध्रुव { प्रत्यागम v; }
    तार्किक __more__ () ध्रुव { प्रत्यागम v != INVALID; }
    शून्य __next__ () {
       s->next (&v);
    }
    शून्य __prev__ () { s->previous (&v); }
    iter_t end () ध्रुव { प्रत्यागम iter_t (*s, असत्य); }
    तार्किक operator != (ध्रुव iter_t& o) ध्रुव
    { प्रत्यागम v != o.v; }

    रक्षित:
    ध्रुव hb_bit_page_t *s;
    hb_codepoint_t v;
  };
  iter_t iter () ध्रुव { प्रत्यागम iter_t (*एतत्); }
  operator iter_t () ध्रुव { प्रत्यागम iter (); }

  स्थिर constexpr hb_codepoint_t INVALID = HB_SET_मूल्य_INVALID;

  प्रकारनिर्देश unsigned long long elt_t;
  स्थिर constexpr unsigned PAGE_BITS_LOG_2 = 9; // 512 bits
  स्थिर constexpr unsigned PAGE_BITS = 1 << PAGE_BITS_LOG_2;
  static_assert (1 << PAGE_BITS_LOG_2 == PAGE_BITS, "");
  static_assert ((PAGE_BITS & ((PAGE_BITS) - 1)) == 0, "");
  स्थिर constexpr unsigned PAGE_BITMASK = PAGE_BITS - 1;

  स्थिर unsigned पूर्णाङ्क elt_get_min (ध्रुव elt_t &elt) { प्रत्यागम hb_ctz (elt); }
  स्थिर unsigned पूर्णाङ्क elt_get_max (ध्रुव elt_t &elt) { प्रत्यागम hb_bit_storage (elt) - 1; }

  प्रकारनिर्देश hb_vector_size_t<elt_t, PAGE_BITS / 8> vector_t;

  स्थिर constexpr unsigned ELT_BITS = परिमाण (elt_t) * 8;
  स्थिर constexpr unsigned ELT_MASK = ELT_BITS - 1;

  स्थिर constexpr unsigned BITS = परिमाण (vector_t) * 8;
  स्थिर constexpr unsigned MASK = BITS - 1;
  static_assert ((unsigned) PAGE_BITS == (unsigned) BITS, "");

  elt_t &elt (hb_codepoint_t g) { प्रत्यागम v[(g & MASK) / ELT_BITS]; }
  ध्रुव elt_t& elt (hb_codepoint_t g) ध्रुव { प्रत्यागम v[(g & MASK) / ELT_BITS]; }
  स्थिर constexpr elt_t mask (hb_codepoint_t g) { प्रत्यागम elt_t (1) << (g & ELT_MASK); }

  mutable unsigned population;
  vector_t v;
};


#endif /* HB_BIT_PAGE_HH */

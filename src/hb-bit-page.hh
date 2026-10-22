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
ढाँचा <typename elt_t, अचिन्हित पूर्णाङ्क byte_size>
संरचना hb_vector_size_t
{
  elt_t& चालक [] (अचिन्हित पूर्णाङ्क i) { फर्काऊ v[i]; }
  अचर elt_t& चालक [] (अचिन्हित पूर्णाङ्क i) अचर { फर्काऊ v[i]; }

  शून्य init0 ()
  {
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < ARRAY_LENGTH (v); i++)
      v[i] = 0;
  }
  शून्य init1 ()
  {
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < ARRAY_LENGTH (v); i++)
      v[i] = (elt_t) -1;
  }

  ढाँचा <typename Op>
  hb_vector_size_t process (अचर Op& op) अचर
  {
    hb_vector_size_t r;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < ARRAY_LENGTH (v); i++)
      r.v[i] = op (v[i]);
    फर्काऊ r;
  }
  ढाँचा <typename Op>
  hb_vector_size_t process (अचर Op& op, अचर hb_vector_size_t &o) अचर
  {
    hb_vector_size_t r;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < ARRAY_LENGTH (v); i++)
      r.v[i] = op (v[i], o.v[i]);
    फर्काऊ r;
  }
  hb_vector_size_t चालक | (अचर hb_vector_size_t &o) अचर
  { फर्काऊ process (hb_bitwise_or, o); }
  hb_vector_size_t चालक & (अचर hb_vector_size_t &o) अचर
  { फर्काऊ process (hb_bitwise_and, o); }
  hb_vector_size_t चालक ^ (अचर hb_vector_size_t &o) अचर
  { फर्काऊ process (hb_bitwise_xor, o); }
  hb_vector_size_t चालक ~ () अचर
  { फर्काऊ process (hb_bitwise_neg); }

  चालक तार्किक () अचर
  {
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < ARRAY_LENGTH (v); i++)
      यदि (v[i])
	फर्काऊ सत्य;
    फर्काऊ असत्य;
  }
  चालक अचिन्हित पूर्णाङ्क () अचर
  {
    अचिन्हित पूर्णाङ्क r = 0;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < ARRAY_LENGTH (v); i++)
      r += hb_popcount (v[i]);
    फर्काऊ r;
  }
  तार्किक चालक == (अचर hb_vector_size_t &o) अचर
  {
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < ARRAY_LENGTH (v); i++)
      यदि (v[i] != o.v[i])
	फर्काऊ असत्य;
    फर्काऊ सत्य;
  }

  hb_array_t<अचर elt_t> iter () अचर
  { फर्काऊ hb_array (v); }

  गुप्त:
  static_assert (0 == byte_size % परिमाण (elt_t), "");
  elt_t v[byte_size / परिमाण (elt_t)];
};


संरचना hb_bit_page_t
{
  hb_bit_page_t () { init0 (); }

  शून्य init0 () { v.init0 (); population = 0; }
  शून्य init1 () { v.init1 (); population = PAGE_BITS; }

  शून्य dirty () { population = अच_पूर्णांक_उच्च; }

  स्थिर अन्तःस्थ constexpr अचिन्हित len ()
  { फर्काऊ ARRAY_LENGTH_CONST (v); }

  चालक तार्किक () अचर { फर्काऊ !is_empty (); }
  तार्किक is_empty () अचर
  {
    यदि (has_population ()) फर्काऊ !population;
    तार्किक empty = !v;
    यदि (empty) population = 0;
    फर्काऊ empty;
  }
  uint32_t hash () अचर
  {
    फर्काऊ hb_bytes_t ((अचर वर्ण *) &v, परिमाण (v)).hash ();
  }

  शून्य add (hb_codepoint_t g) { elt (g) |= mask (g); dirty (); }
  शून्य add_bits (hb_codepoint_t g, uint64_t bits) { elt (g) |= bits; dirty (); }
  शून्य del (hb_codepoint_t g) { elt (g) &= ~mask (g); dirty (); }
  शून्य del_bits (hb_codepoint_t g, uint64_t bits) { elt (g) &= ~bits; dirty (); }
  शून्य set (hb_codepoint_t g, तार्किक value) { यदि (value) add (g); अन्यथा del (g); }
  तार्किक get (hb_codepoint_t g) अचर { फर्काऊ elt (g) & mask (g); }
  तार्किक may_have (hb_codepoint_t g) अचर { फर्काऊ get (g); }

  तार्किक चालक [] (hb_codepoint_t g) अचर { फर्काऊ get (g); }
  तार्किक चालक () (hb_codepoint_t g) अचर { फर्काऊ get (g); }
  तार्किक has (hb_codepoint_t g) अचर { फर्काऊ get (g); }

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
      फर्काऊ;
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
      फर्काऊ;
    dirty ();
  }
  शून्य set_range (hb_codepoint_t a, hb_codepoint_t b, तार्किक v)
  { यदि (v) add_range (a, b); अन्यथा del_range (a, b); }


  // Writes out page values to the array p. Returns the number of values
  // written. At most size codepoints will be written.
  अचिन्हित पूर्णाङ्क लिख (uint32_t        base,
		      अचिन्हित पूर्णाङ्क    start_मान,
		      hb_codepoint_t *p,
		      अचिन्हित पूर्णाङ्क    size) अचर
  {
    अचिन्हित पूर्णाङ्क start_v = start_मान / ELT_BITS;
    अचिन्हित पूर्णाङ्क start_bit = start_मान & ELT_MASK;
    अचिन्हित पूर्णाङ्क गन्ती = 0;
    लागि (अचिन्हित i = start_v; i < len () && गन्ती < size; i++)
    {
      elt_t bits = v[i];
      uint32_t v_base = base | (i * ELT_BITS);
      लागि (अचिन्हित पूर्णाङ्क j = start_bit; j < ELT_BITS && गन्ती < size; j++)
      {
	यदि ((elt_t(1) << j) & bits) {
	  *p++ = v_base | j;
	  गन्ती++;
	}
      }
      start_bit = 0;
    }
    फर्काऊ गन्ती;
  }

  // Writes out the values NOT in this page to the array p. Returns the
  // number of values written. At most size codepoints will be written.
  // Returns the number of codepoints written. next_value holds the next value
  // that should be written (if not present in this page). This is used to fill
  // any missing value gaps between this page and the previous page, if any.
  // next_value is updated to one more than the last value present in this page.
  अचिन्हित पूर्णाङ्क write_inverted (uint32_t        base,
			       अचिन्हित पूर्णाङ्क    start_मान,
			       hb_codepoint_t *p,
			       अचिन्हित पूर्णाङ्क    size,
			       hb_codepoint_t *next_मान) अचर
  {
    अचिन्हित पूर्णाङ्क start_v = start_मान / ELT_BITS;
    अचिन्हित पूर्णाङ्क start_bit = start_मान & ELT_MASK;
    अचिन्हित पूर्णाङ्क गन्ती = 0;
    लागि (अचिन्हित i = start_v; i < len () && गन्ती < size; i++)
    {
      elt_t bits = v[i];
      uint32_t v_offset = i * ELT_BITS;
      लागि (अचिन्हित पूर्णाङ्क j = start_bit; j < ELT_BITS && गन्ती < size; j++)
      {
	यदि ((elt_t(1) << j) & bits)
	{
	  hb_codepoint_t value = base | v_offset | j;
	  // Emit all the missing values from next_value up to value - 1.
	  लागि (hb_codepoint_t k = *next_मान; k < value && गन्ती < size; k++)
	  {
	    *p++ = k;
	    गन्ती++;
	  }
	  // Skip over this value;
	  *next_मान = value + 1;
	}
      }
      start_bit = 0;
    }
    फर्काऊ गन्ती;
  }

  तार्किक चालक == (अचर hb_bit_page_t &other) अचर { फर्काऊ is_equal (other); }
  तार्किक is_equal (अचर hb_bit_page_t &other) अचर { फर्काऊ v == other.v; }
  तार्किक intersects (अचर hb_bit_page_t &other) अचर
  {
    लागि (अचिन्हित i = 0; i < len (); i++)
      यदि (v[i] & other.v[i])
	फर्काऊ सत्य;
    फर्काऊ असत्य;
  }
  तार्किक may_intersect (अचर hb_bit_page_t &other) अचर
  { फर्काऊ intersects (other); }

  तार्किक चालक <= (अचर hb_bit_page_t &larger_page) अचर { फर्काऊ is_subset (larger_page); }
  तार्किक is_subset (अचर hb_bit_page_t &larger_page) अचर
  {
    यदि (has_population () && larger_page.has_population () &&
	population > larger_page.population)
      फर्काऊ असत्य;

    लागि (अचिन्हित i = 0; i < len (); i++)
      यदि (~larger_page.v[i] & v[i])
	फर्काऊ असत्य;
    फर्काऊ सत्य;
  }

  तार्किक has_population () अचर { फर्काऊ population != अच_पूर्णांक_उच्च; }
  अचिन्हित get_population () अचर
  {
    यदि (has_population ()) फर्काऊ population;
    फर्काऊ population = v;
  }

  तार्किक next (hb_codepoint_t *codepoint) अचर
  {
    अचिन्हित पूर्णाङ्क m = (*codepoint + 1) & MASK;
    यदि (!m)
    {
      *codepoint = INVALID;
      फर्काऊ असत्य;
    }
    अचिन्हित पूर्णाङ्क i = m / ELT_BITS;
    अचिन्हित पूर्णाङ्क j = m & ELT_MASK;

    अचर elt_t vv = v[i] & ~((elt_t (1) << j) - 1);
    लागि (अचर elt_t *p = &vv; i < len (); p = ((अचर elt_t *) &v[0]) + (++i))
      यदि (*p)
      {
	*codepoint = i * ELT_BITS + elt_get_min (*p);
	फर्काऊ सत्य;
      }

    *codepoint = INVALID;
    फर्काऊ असत्य;
  }
  तार्किक previous (hb_codepoint_t *codepoint) अचर
  {
    अचिन्हित पूर्णाङ्क m = (*codepoint - 1) & MASK;
    यदि (m == MASK)
    {
      *codepoint = INVALID;
      फर्काऊ असत्य;
    }
    अचिन्हित पूर्णाङ्क i = m / ELT_BITS;
    अचिन्हित पूर्णाङ्क j = m & ELT_MASK;

    /* Fancy mask to avoid shifting by elt_t bitsize, which is undefined. */
    अचर elt_t mask = j < 8 * परिमाण (elt_t) - 1 ?
		       ((elt_t (1) << (j + 1)) - 1) :
		       (elt_t) -1;
    अचर elt_t vv = v[i] & mask;
    अचर elt_t *p = &vv;
    जबसम्म (सत्य)
    {
      यदि (*p)
      {
	*codepoint = i * ELT_BITS + elt_get_max (*p);
	फर्काऊ सत्य;
      }
      यदि ((पूर्णाङ्क) i <= 0) रोक;
      p = &v[--i];
    }

    *codepoint = INVALID;
    फर्काऊ असत्य;
  }
  hb_codepoint_t get_min () अचर
  {
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < len (); i++)
      यदि (v[i])
	फर्काऊ i * ELT_BITS + elt_get_min (v[i]);
    फर्काऊ INVALID;
  }
  hb_codepoint_t get_max () अचर
  {
    लागि (पूर्णाङ्क i = len () - 1; i >= 0; i--)
      यदि (v[i])
	फर्काऊ i * ELT_BITS + elt_get_max (v[i]);
    फर्काऊ INVALID;
  }

  /*
   * Iterator implementation.
   */
  संरचना iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    स्थिर constexpr तार्किक is_sorted_iterator = सत्य;
    iter_t (अचर hb_bit_page_t &s_ = Null (hb_bit_page_t), तार्किक init = सत्य) : s (&s_), v (INVALID)
    {
      यदि (init)
	v = s->get_min ();
    }

    प्रकारनिर्देश hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () अचर { फर्काऊ v; }
    तार्किक __more__ () अचर { फर्काऊ v != INVALID; }
    शून्य __next__ () {
       s->next (&v);
    }
    शून्य __prev__ () { s->previous (&v); }
    iter_t end () अचर { फर्काऊ iter_t (*s, असत्य); }
    तार्किक चालक != (अचर iter_t& o) अचर
    { फर्काऊ v != o.v; }

    रक्षित:
    अचर hb_bit_page_t *s;
    hb_codepoint_t v;
  };
  iter_t iter () अचर { फर्काऊ iter_t (*एतत्); }
  चालक iter_t () अचर { फर्काऊ iter (); }

  स्थिर constexpr hb_codepoint_t INVALID = HB_SET_मान_INVALID;

  प्रकारनिर्देश अचिन्हित दीर्घ दीर्घ elt_t;
  स्थिर constexpr अचिन्हित PAGE_BITS_LOG_2 = 9; // 512 bits
  स्थिर constexpr अचिन्हित PAGE_BITS = 1 << PAGE_BITS_LOG_2;
  static_assert (1 << PAGE_BITS_LOG_2 == PAGE_BITS, "");
  static_assert ((PAGE_BITS & ((PAGE_BITS) - 1)) == 0, "");
  स्थिर constexpr अचिन्हित PAGE_BITMASK = PAGE_BITS - 1;

  स्थिर अचिन्हित पूर्णाङ्क elt_get_min (अचर elt_t &elt) { फर्काऊ hb_ctz (elt); }
  स्थिर अचिन्हित पूर्णाङ्क elt_get_max (अचर elt_t &elt) { फर्काऊ hb_bit_storage (elt) - 1; }

  प्रकारनिर्देश hb_vector_size_t<elt_t, PAGE_BITS / 8> vector_t;

  स्थिर constexpr अचिन्हित ELT_BITS = परिमाण (elt_t) * 8;
  स्थिर constexpr अचिन्हित ELT_MASK = ELT_BITS - 1;

  स्थिर constexpr अचिन्हित BITS = परिमाण (vector_t) * 8;
  स्थिर constexpr अचिन्हित MASK = BITS - 1;
  static_assert ((अचिन्हित) PAGE_BITS == (अचिन्हित) BITS, "");

  elt_t &elt (hb_codepoint_t g) { फर्काऊ v[(g & MASK) / ELT_BITS]; }
  अचर elt_t& elt (hb_codepoint_t g) अचर { फर्काऊ v[(g & MASK) / ELT_BITS]; }
  स्थिर constexpr elt_t mask (hb_codepoint_t g) { फर्काऊ elt_t (1) << (g & ELT_MASK); }

  mutable अचिन्हित population;
  vector_t v;
};


#endif /* HB_BIT_PAGE_HH */

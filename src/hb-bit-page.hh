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
ढाँचा <typename elt_t, अचिन्हित पूर्णांक byte_size>
संरचना hb_vector_size_t
{
  elt_t& चालक [] (अचिन्हित पूर्णांक i) { परत v[i]; }
  स्थिरांक elt_t& चालक [] (अचिन्हित पूर्णांक i) स्थिरांक { परत v[i]; }

  शून्य init0 ()
  {
    साठी (अचिन्हित पूर्णांक i = 0; i < ARRAY_LENGTH (v); i++)
      v[i] = 0;
  }
  शून्य init1 ()
  {
    साठी (अचिन्हित पूर्णांक i = 0; i < ARRAY_LENGTH (v); i++)
      v[i] = (elt_t) -1;
  }

  ढाँचा <typename Op>
  hb_vector_size_t process (स्थिरांक Op& op) स्थिरांक
  {
    hb_vector_size_t r;
    साठी (अचिन्हित पूर्णांक i = 0; i < ARRAY_LENGTH (v); i++)
      r.v[i] = op (v[i]);
    परत r;
  }
  ढाँचा <typename Op>
  hb_vector_size_t process (स्थिरांक Op& op, स्थिरांक hb_vector_size_t &o) स्थिरांक
  {
    hb_vector_size_t r;
    साठी (अचिन्हित पूर्णांक i = 0; i < ARRAY_LENGTH (v); i++)
      r.v[i] = op (v[i], o.v[i]);
    परत r;
  }
  hb_vector_size_t चालक | (स्थिरांक hb_vector_size_t &o) स्थिरांक
  { परत process (hb_bitwise_or, o); }
  hb_vector_size_t चालक & (स्थिरांक hb_vector_size_t &o) स्थिरांक
  { परत process (hb_bitwise_and, o); }
  hb_vector_size_t चालक ^ (स्थिरांक hb_vector_size_t &o) स्थिरांक
  { परत process (hb_bitwise_xor, o); }
  hb_vector_size_t चालक ~ () स्थिरांक
  { परत process (hb_bitwise_neg); }

  चालक तार्किक () स्थिरांक
  {
    साठी (अचिन्हित पूर्णांक i = 0; i < ARRAY_LENGTH (v); i++)
      जर (v[i])
	परत सत्य;
    परत असत्य;
  }
  चालक अचिन्हित पूर्णांक () स्थिरांक
  {
    अचिन्हित पूर्णांक r = 0;
    साठी (अचिन्हित पूर्णांक i = 0; i < ARRAY_LENGTH (v); i++)
      r += hb_popcount (v[i]);
    परत r;
  }
  तार्किक चालक == (स्थिरांक hb_vector_size_t &o) स्थिरांक
  {
    साठी (अचिन्हित पूर्णांक i = 0; i < ARRAY_LENGTH (v); i++)
      जर (v[i] != o.v[i])
	परत असत्य;
    परत सत्य;
  }

  hb_array_t<स्थिरांक elt_t> iter () स्थिरांक
  { परत hb_array (v); }

  खाजगी:
  static_assert (0 == byte_size % आकार (elt_t), "");
  elt_t v[byte_size / आकार (elt_t)];
};


संरचना hb_bit_page_t
{
  hb_bit_page_t () { init0 (); }

  शून्य init0 () { v.init0 (); population = 0; }
  शून्य init1 () { v.init1 (); population = PAGE_BITS; }

  शून्य dirty () { population = अच_पूर्णांक_उच्च; }

  स्थिर ओळीत constexpr अचिन्हित len ()
  { परत ARRAY_LENGTH_CONST (v); }

  चालक तार्किक () स्थिरांक { परत !is_empty (); }
  तार्किक is_empty () स्थिरांक
  {
    जर (has_population ()) परत !population;
    तार्किक empty = !v;
    जर (empty) population = 0;
    परत empty;
  }
  uint32_t hash () स्थिरांक
  {
    परत hb_bytes_t ((स्थिरांक अक्षर *) &v, आकार (v)).hash ();
  }

  शून्य add (hb_codepoint_t g) { elt (g) |= mask (g); dirty (); }
  शून्य add_bits (hb_codepoint_t g, uint64_t bits) { elt (g) |= bits; dirty (); }
  शून्य del (hb_codepoint_t g) { elt (g) &= ~mask (g); dirty (); }
  शून्य del_bits (hb_codepoint_t g, uint64_t bits) { elt (g) &= ~bits; dirty (); }
  शून्य set (hb_codepoint_t g, तार्किक value) { जर (value) add (g); अन्यथा del (g); }
  तार्किक get (hb_codepoint_t g) स्थिरांक { परत elt (g) & mask (g); }
  तार्किक may_have (hb_codepoint_t g) स्थिरांक { परत get (g); }

  तार्किक चालक [] (hb_codepoint_t g) स्थिरांक { परत get (g); }
  तार्किक चालक () (hb_codepoint_t g) स्थिरांक { परत get (g); }
  तार्किक has (hb_codepoint_t g) स्थिरांक { परत get (g); }

  शून्य add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    जर (la == lb)
      *la |= (mask (b) << 1) - mask(a);
    अन्यथा जर (likely (la < lb))
    {
      *la |= ~(mask (a) - 1llu);
      la++;

      hb_memset (la, 0xff, (अक्षर *) lb - (अक्षर *) la);

      *lb |= ((mask (b) << 1) - 1llu);
    }
    अन्यथा
      परत;
    dirty ();
  }
  शून्य del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    जर (la == lb)
      *la &= ~((mask (b) << 1llu) - mask(a));
    अन्यथा जर (likely (la < lb))
    {
      *la &= mask (a) - 1;
      la++;

      hb_memset (la, 0, (अक्षर *) lb - (अक्षर *) la);

      *lb &= ~((mask (b) << 1) - 1llu);
    }
    अन्यथा
      परत;
    dirty ();
  }
  शून्य set_range (hb_codepoint_t a, hb_codepoint_t b, तार्किक v)
  { जर (v) add_range (a, b); अन्यथा del_range (a, b); }


  // Writes out page values to the array p. Returns the number of values
  // written. At most size codepoints will be written.
  अचिन्हित पूर्णांक लिख (uint32_t        base,
		      अचिन्हित पूर्णांक    start_मूल्य,
		      hb_codepoint_t *p,
		      अचिन्हित पूर्णांक    size) स्थिरांक
  {
    अचिन्हित पूर्णांक start_v = start_मूल्य / ELT_BITS;
    अचिन्हित पूर्णांक start_bit = start_मूल्य & ELT_MASK;
    अचिन्हित पूर्णांक मोजणी = 0;
    साठी (अचिन्हित i = start_v; i < len () && मोजणी < size; i++)
    {
      elt_t bits = v[i];
      uint32_t v_base = base | (i * ELT_BITS);
      साठी (अचिन्हित पूर्णांक j = start_bit; j < ELT_BITS && मोजणी < size; j++)
      {
	जर ((elt_t(1) << j) & bits) {
	  *p++ = v_base | j;
	  मोजणी++;
	}
      }
      start_bit = 0;
    }
    परत मोजणी;
  }

  // Writes out the values NOT in this page to the array p. Returns the
  // number of values written. At most size codepoints will be written.
  // Returns the number of codepoints written. next_value holds the next value
  // that should be written (if not present in this page). This is used to fill
  // any missing value gaps between this page and the previous page, if any.
  // next_value is updated to one more than the last value present in this page.
  अचिन्हित पूर्णांक write_inverted (uint32_t        base,
			       अचिन्हित पूर्णांक    start_मूल्य,
			       hb_codepoint_t *p,
			       अचिन्हित पूर्णांक    size,
			       hb_codepoint_t *next_मूल्य) स्थिरांक
  {
    अचिन्हित पूर्णांक start_v = start_मूल्य / ELT_BITS;
    अचिन्हित पूर्णांक start_bit = start_मूल्य & ELT_MASK;
    अचिन्हित पूर्णांक मोजणी = 0;
    साठी (अचिन्हित i = start_v; i < len () && मोजणी < size; i++)
    {
      elt_t bits = v[i];
      uint32_t v_offset = i * ELT_BITS;
      साठी (अचिन्हित पूर्णांक j = start_bit; j < ELT_BITS && मोजणी < size; j++)
      {
	जर ((elt_t(1) << j) & bits)
	{
	  hb_codepoint_t value = base | v_offset | j;
	  // Emit all the missing values from next_value up to value - 1.
	  साठी (hb_codepoint_t k = *next_मूल्य; k < value && मोजणी < size; k++)
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
    परत मोजणी;
  }

  तार्किक चालक == (स्थिरांक hb_bit_page_t &other) स्थिरांक { परत is_equal (other); }
  तार्किक is_equal (स्थिरांक hb_bit_page_t &other) स्थिरांक { परत v == other.v; }
  तार्किक intersects (स्थिरांक hb_bit_page_t &other) स्थिरांक
  {
    साठी (अचिन्हित i = 0; i < len (); i++)
      जर (v[i] & other.v[i])
	परत सत्य;
    परत असत्य;
  }
  तार्किक may_intersect (स्थिरांक hb_bit_page_t &other) स्थिरांक
  { परत intersects (other); }

  तार्किक चालक <= (स्थिरांक hb_bit_page_t &larger_page) स्थिरांक { परत is_subset (larger_page); }
  तार्किक is_subset (स्थिरांक hb_bit_page_t &larger_page) स्थिरांक
  {
    जर (has_population () && larger_page.has_population () &&
	population > larger_page.population)
      परत असत्य;

    साठी (अचिन्हित i = 0; i < len (); i++)
      जर (~larger_page.v[i] & v[i])
	परत असत्य;
    परत सत्य;
  }

  तार्किक has_population () स्थिरांक { परत population != अच_पूर्णांक_उच्च; }
  अचिन्हित get_population () स्थिरांक
  {
    जर (has_population ()) परत population;
    परत population = v;
  }

  तार्किक next (hb_codepoint_t *codepoint) स्थिरांक
  {
    अचिन्हित पूर्णांक m = (*codepoint + 1) & MASK;
    जर (!m)
    {
      *codepoint = INVALID;
      परत असत्य;
    }
    अचिन्हित पूर्णांक i = m / ELT_BITS;
    अचिन्हित पूर्णांक j = m & ELT_MASK;

    स्थिरांक elt_t vv = v[i] & ~((elt_t (1) << j) - 1);
    साठी (स्थिरांक elt_t *p = &vv; i < len (); p = ((स्थिरांक elt_t *) &v[0]) + (++i))
      जर (*p)
      {
	*codepoint = i * ELT_BITS + elt_get_min (*p);
	परत सत्य;
      }

    *codepoint = INVALID;
    परत असत्य;
  }
  तार्किक previous (hb_codepoint_t *codepoint) स्थिरांक
  {
    अचिन्हित पूर्णांक m = (*codepoint - 1) & MASK;
    जर (m == MASK)
    {
      *codepoint = INVALID;
      परत असत्य;
    }
    अचिन्हित पूर्णांक i = m / ELT_BITS;
    अचिन्हित पूर्णांक j = m & ELT_MASK;

    /* Fancy mask to avoid shifting by elt_t bitsize, which is undefined. */
    स्थिरांक elt_t mask = j < 8 * आकार (elt_t) - 1 ?
		       ((elt_t (1) << (j + 1)) - 1) :
		       (elt_t) -1;
    स्थिरांक elt_t vv = v[i] & mask;
    स्थिरांक elt_t *p = &vv;
    पर्यंत (सत्य)
    {
      जर (*p)
      {
	*codepoint = i * ELT_BITS + elt_get_max (*p);
	परत सत्य;
      }
      जर ((पूर्णांक) i <= 0) थांबा;
      p = &v[--i];
    }

    *codepoint = INVALID;
    परत असत्य;
  }
  hb_codepoint_t get_min () स्थिरांक
  {
    साठी (अचिन्हित पूर्णांक i = 0; i < len (); i++)
      जर (v[i])
	परत i * ELT_BITS + elt_get_min (v[i]);
    परत INVALID;
  }
  hb_codepoint_t get_max () स्थिरांक
  {
    साठी (पूर्णांक i = len () - 1; i >= 0; i--)
      जर (v[i])
	परत i * ELT_BITS + elt_get_max (v[i]);
    परत INVALID;
  }

  /*
   * Iterator implementation.
   */
  संरचना iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    स्थिर constexpr तार्किक is_sorted_iterator = सत्य;
    iter_t (स्थिरांक hb_bit_page_t &s_ = Null (hb_bit_page_t), तार्किक init = सत्य) : s (&s_), v (INVALID)
    {
      जर (init)
	v = s->get_min ();
    }

    प्रकारव्याख्या hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () स्थिरांक { परत v; }
    तार्किक __more__ () स्थिरांक { परत v != INVALID; }
    शून्य __next__ () {
       s->next (&v);
    }
    शून्य __prev__ () { s->previous (&v); }
    iter_t end () स्थिरांक { परत iter_t (*s, असत्य); }
    तार्किक चालक != (स्थिरांक iter_t& o) स्थिरांक
    { परत v != o.v; }

    संरक्षित:
    स्थिरांक hb_bit_page_t *s;
    hb_codepoint_t v;
  };
  iter_t iter () स्थिरांक { परत iter_t (*हे); }
  चालक iter_t () स्थिरांक { परत iter (); }

  स्थिर constexpr hb_codepoint_t INVALID = HB_SET_मूल्य_INVALID;

  प्रकारव्याख्या अचिन्हित दीर्घ दीर्घ elt_t;
  स्थिर constexpr अचिन्हित PAGE_BITS_LOG_2 = 9; // 512 bits
  स्थिर constexpr अचिन्हित PAGE_BITS = 1 << PAGE_BITS_LOG_2;
  static_assert (1 << PAGE_BITS_LOG_2 == PAGE_BITS, "");
  static_assert ((PAGE_BITS & ((PAGE_BITS) - 1)) == 0, "");
  स्थिर constexpr अचिन्हित PAGE_BITMASK = PAGE_BITS - 1;

  स्थिर अचिन्हित पूर्णांक elt_get_min (स्थिरांक elt_t &elt) { परत hb_ctz (elt); }
  स्थिर अचिन्हित पूर्णांक elt_get_max (स्थिरांक elt_t &elt) { परत hb_bit_storage (elt) - 1; }

  प्रकारव्याख्या hb_vector_size_t<elt_t, PAGE_BITS / 8> vector_t;

  स्थिर constexpr अचिन्हित ELT_BITS = आकार (elt_t) * 8;
  स्थिर constexpr अचिन्हित ELT_MASK = ELT_BITS - 1;

  स्थिर constexpr अचिन्हित BITS = आकार (vector_t) * 8;
  स्थिर constexpr अचिन्हित MASK = BITS - 1;
  static_assert ((अचिन्हित) PAGE_BITS == (अचिन्हित) BITS, "");

  elt_t &elt (hb_codepoint_t g) { परत v[(g & MASK) / ELT_BITS]; }
  स्थिरांक elt_t& elt (hb_codepoint_t g) स्थिरांक { परत v[(g & MASK) / ELT_BITS]; }
  स्थिर constexpr elt_t mask (hb_codepoint_t g) { परत elt_t (1) << (g & ELT_MASK); }

  mutable अचिन्हित population;
  vector_t v;
};


#endif /* HB_BIT_PAGE_HH */

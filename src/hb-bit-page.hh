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

#અન્તર્ભાવ "hb.hh"


/* Compiler-assisted vectorization. */

/* Type behaving similar to vectorized vars defined using __attribute__((vector_size(...))),
 * basically a fixed-size bitset. We can't use the compiler type because hb_vector_t cannot
 * guarantee alignment requirements. */
template <typename elt_t, unsigned પૂર્ણાંક byte_size>
સંરચના hb_vector_size_t
{
  elt_t& operator [] (unsigned પૂર્ણાંક i) { પાછા v[i]; }
  અચલ elt_t& operator [] (unsigned પૂર્ણાંક i) અચલ { પાછા v[i]; }

  શૂન્ય init0 ()
  {
    માટે (unsigned પૂર્ણાંક i = 0; i < ARRAY_LENGTH (v); i++)
      v[i] = 0;
  }
  શૂન્ય init1 ()
  {
    માટે (unsigned પૂર્ણાંક i = 0; i < ARRAY_LENGTH (v); i++)
      v[i] = (elt_t) -1;
  }

  template <typename Op>
  hb_vector_size_t process (અચલ Op& op) અચલ
  {
    hb_vector_size_t r;
    માટે (unsigned પૂર્ણાંક i = 0; i < ARRAY_LENGTH (v); i++)
      r.v[i] = op (v[i]);
    પાછા r;
  }
  template <typename Op>
  hb_vector_size_t process (અચલ Op& op, અચલ hb_vector_size_t &o) અચલ
  {
    hb_vector_size_t r;
    માટે (unsigned પૂર્ણાંક i = 0; i < ARRAY_LENGTH (v); i++)
      r.v[i] = op (v[i], o.v[i]);
    પાછા r;
  }
  hb_vector_size_t operator | (અચલ hb_vector_size_t &o) અચલ
  { પાછા process (hb_bitwise_or, o); }
  hb_vector_size_t operator & (અચલ hb_vector_size_t &o) અચલ
  { પાછા process (hb_bitwise_and, o); }
  hb_vector_size_t operator ^ (અચલ hb_vector_size_t &o) અચલ
  { પાછા process (hb_bitwise_xor, o); }
  hb_vector_size_t operator ~ () અચલ
  { પાછા process (hb_bitwise_neg); }

  operator તાર્કિક () અચલ
  {
    માટે (unsigned પૂર્ણાંક i = 0; i < ARRAY_LENGTH (v); i++)
      જો (v[i])
	પાછા સાચું;
    પાછા ખોટું;
  }
  operator unsigned પૂર્ણાંક () અચલ
  {
    unsigned પૂર્ણાંક r = 0;
    માટે (unsigned પૂર્ણાંક i = 0; i < ARRAY_LENGTH (v); i++)
      r += hb_popcount (v[i]);
    પાછા r;
  }
  તાર્કિક operator == (અચલ hb_vector_size_t &o) અચલ
  {
    માટે (unsigned પૂર્ણાંક i = 0; i < ARRAY_LENGTH (v); i++)
      જો (v[i] != o.v[i])
	પાછા ખોટું;
    પાછા સાચું;
  }

  hb_array_t<અચલ elt_t> iter () અચલ
  { પાછા hb_array (v); }

  ખાનગી:
  static_assert (0 == byte_size % કદ (elt_t), "");
  elt_t v[byte_size / કદ (elt_t)];
};


સંરચના hb_bit_page_t
{
  hb_bit_page_t () { init0 (); }

  શૂન્ય init0 () { v.init0 (); population = 0; }
  શૂન્ય init1 () { v.init1 (); population = PAGE_BITS; }

  શૂન્ય dirty () { population = UINT_MAX; }

  સ્થિર અંતઃરેખ constexpr unsigned len ()
  { પાછા ARRAY_LENGTH_CONST (v); }

  operator તાર્કિક () અચલ { પાછા !is_empty (); }
  તાર્કિક is_empty () અચલ
  {
    જો (has_population ()) પાછા !population;
    તાર્કિક empty = !v;
    જો (empty) population = 0;
    પાછા empty;
  }
  uint32_t hash () અચલ
  {
    પાછા hb_bytes_t ((અચલ અક્ષર *) &v, કદ (v)).hash ();
  }

  શૂન્ય add (hb_codepoint_t g) { elt (g) |= mask (g); dirty (); }
  શૂન્ય add_bits (hb_codepoint_t g, uint64_t bits) { elt (g) |= bits; dirty (); }
  શૂન્ય del (hb_codepoint_t g) { elt (g) &= ~mask (g); dirty (); }
  શૂન્ય del_bits (hb_codepoint_t g, uint64_t bits) { elt (g) &= ~bits; dirty (); }
  શૂન્ય set (hb_codepoint_t g, તાર્કિક value) { જો (value) add (g); નહીંતર del (g); }
  તાર્કિક get (hb_codepoint_t g) અચલ { પાછા elt (g) & mask (g); }
  તાર્કિક may_have (hb_codepoint_t g) અચલ { પાછા get (g); }

  તાર્કિક operator [] (hb_codepoint_t g) અચલ { પાછા get (g); }
  તાર્કિક operator () (hb_codepoint_t g) અચલ { પાછા get (g); }
  તાર્કિક has (hb_codepoint_t g) અચલ { પાછા get (g); }

  શૂન્ય add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    જો (la == lb)
      *la |= (mask (b) << 1) - mask(a);
    નહીંતર જો (likely (la < lb))
    {
      *la |= ~(mask (a) - 1llu);
      la++;

      hb_memset (la, 0xff, (અક્ષર *) lb - (અક્ષર *) la);

      *lb |= ((mask (b) << 1) - 1llu);
    }
    નહીંતર
      પાછા;
    dirty ();
  }
  શૂન્ય del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    જો (la == lb)
      *la &= ~((mask (b) << 1llu) - mask(a));
    નહીંતર જો (likely (la < lb))
    {
      *la &= mask (a) - 1;
      la++;

      hb_memset (la, 0, (અક્ષર *) lb - (અક્ષર *) la);

      *lb &= ~((mask (b) << 1) - 1llu);
    }
    નહીંતર
      પાછા;
    dirty ();
  }
  શૂન્ય set_range (hb_codepoint_t a, hb_codepoint_t b, તાર્કિક v)
  { જો (v) add_range (a, b); નહીંતર del_range (a, b); }


  // Writes out page values to the array p. Returns the number of values
  // written. At most size codepoints will be written.
  unsigned પૂર્ણાંક લિખ (uint32_t        base,
		      unsigned પૂર્ણાંક    start_કિંમત,
		      hb_codepoint_t *p,
		      unsigned પૂર્ણાંક    size) અચલ
  {
    unsigned પૂર્ણાંક start_v = start_કિંમત / ELT_BITS;
    unsigned પૂર્ણાંક start_bit = start_કિંમત & ELT_MASK;
    unsigned પૂર્ણાંક ગણતરી = 0;
    માટે (unsigned i = start_v; i < len () && ગણતરી < size; i++)
    {
      elt_t bits = v[i];
      uint32_t v_base = base | (i * ELT_BITS);
      માટે (unsigned પૂર્ણાંક j = start_bit; j < ELT_BITS && ગણતરી < size; j++)
      {
	જો ((elt_t(1) << j) & bits) {
	  *p++ = v_base | j;
	  ગણતરી++;
	}
      }
      start_bit = 0;
    }
    પાછા ગણતરી;
  }

  // Writes out the values NOT in this page to the array p. Returns the
  // number of values written. At most size codepoints will be written.
  // Returns the number of codepoints written. next_value holds the next value
  // that should be written (if not present in this page). This is used to fill
  // any missing value gaps between this page and the previous page, if any.
  // next_value is updated to one more than the last value present in this page.
  unsigned પૂર્ણાંક write_inverted (uint32_t        base,
			       unsigned પૂર્ણાંક    start_કિંમત,
			       hb_codepoint_t *p,
			       unsigned પૂર્ણાંક    size,
			       hb_codepoint_t *next_કિંમત) અચલ
  {
    unsigned પૂર્ણાંક start_v = start_કિંમત / ELT_BITS;
    unsigned પૂર્ણાંક start_bit = start_કિંમત & ELT_MASK;
    unsigned પૂર્ણાંક ગણતરી = 0;
    માટે (unsigned i = start_v; i < len () && ગણતરી < size; i++)
    {
      elt_t bits = v[i];
      uint32_t v_offset = i * ELT_BITS;
      માટે (unsigned પૂર્ણાંક j = start_bit; j < ELT_BITS && ગણતરી < size; j++)
      {
	જો ((elt_t(1) << j) & bits)
	{
	  hb_codepoint_t value = base | v_offset | j;
	  // Emit all the missing values from next_value up to value - 1.
	  માટે (hb_codepoint_t k = *next_કિંમત; k < value && ગણતરી < size; k++)
	  {
	    *p++ = k;
	    ગણતરી++;
	  }
	  // Skip over this value;
	  *next_કિંમત = value + 1;
	}
      }
      start_bit = 0;
    }
    પાછા ગણતરી;
  }

  તાર્કિક operator == (અચલ hb_bit_page_t &other) અચલ { પાછા is_equal (other); }
  તાર્કિક is_equal (અચલ hb_bit_page_t &other) અચલ { પાછા v == other.v; }
  તાર્કિક intersects (અચલ hb_bit_page_t &other) અચલ
  {
    માટે (unsigned i = 0; i < len (); i++)
      જો (v[i] & other.v[i])
	પાછા સાચું;
    પાછા ખોટું;
  }
  તાર્કિક may_intersect (અચલ hb_bit_page_t &other) અચલ
  { પાછા intersects (other); }

  તાર્કિક operator <= (અચલ hb_bit_page_t &larger_page) અચલ { પાછા is_subset (larger_page); }
  તાર્કિક is_subset (અચલ hb_bit_page_t &larger_page) અચલ
  {
    જો (has_population () && larger_page.has_population () &&
	population > larger_page.population)
      પાછા ખોટું;

    માટે (unsigned i = 0; i < len (); i++)
      જો (~larger_page.v[i] & v[i])
	પાછા ખોટું;
    પાછા સાચું;
  }

  તાર્કિક has_population () અચલ { પાછા population != UINT_MAX; }
  unsigned get_population () અચલ
  {
    જો (has_population ()) પાછા population;
    પાછા population = v;
  }

  તાર્કિક next (hb_codepoint_t *codepoint) અચલ
  {
    unsigned પૂર્ણાંક m = (*codepoint + 1) & MASK;
    જો (!m)
    {
      *codepoint = INVALID;
      પાછા ખોટું;
    }
    unsigned પૂર્ણાંક i = m / ELT_BITS;
    unsigned પૂર્ણાંક j = m & ELT_MASK;

    અચલ elt_t vv = v[i] & ~((elt_t (1) << j) - 1);
    માટે (અચલ elt_t *p = &vv; i < len (); p = ((અચલ elt_t *) &v[0]) + (++i))
      જો (*p)
      {
	*codepoint = i * ELT_BITS + elt_get_min (*p);
	પાછા સાચું;
      }

    *codepoint = INVALID;
    પાછા ખોટું;
  }
  તાર્કિક previous (hb_codepoint_t *codepoint) અચલ
  {
    unsigned પૂર્ણાંક m = (*codepoint - 1) & MASK;
    જો (m == MASK)
    {
      *codepoint = INVALID;
      પાછા ખોટું;
    }
    unsigned પૂર્ણાંક i = m / ELT_BITS;
    unsigned પૂર્ણાંક j = m & ELT_MASK;

    /* Fancy mask to avoid shifting by elt_t bitsize, which is undefined. */
    અચલ elt_t mask = j < 8 * કદ (elt_t) - 1 ?
		       ((elt_t (1) << (j + 1)) - 1) :
		       (elt_t) -1;
    અચલ elt_t vv = v[i] & mask;
    અચલ elt_t *p = &vv;
    જ્યારે (સાચું)
    {
      જો (*p)
      {
	*codepoint = i * ELT_BITS + elt_get_max (*p);
	પાછા સાચું;
      }
      જો ((પૂર્ણાંક) i <= 0) તોડો;
      p = &v[--i];
    }

    *codepoint = INVALID;
    પાછા ખોટું;
  }
  hb_codepoint_t get_min () અચલ
  {
    માટે (unsigned પૂર્ણાંક i = 0; i < len (); i++)
      જો (v[i])
	પાછા i * ELT_BITS + elt_get_min (v[i]);
    પાછા INVALID;
  }
  hb_codepoint_t get_max () અચલ
  {
    માટે (પૂર્ણાંક i = len () - 1; i >= 0; i--)
      જો (v[i])
	પાછા i * ELT_BITS + elt_get_max (v[i]);
    પાછા INVALID;
  }

  /*
   * Iterator implementation.
   */
  સંરચના iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    સ્થિર constexpr તાર્કિક is_sorted_iterator = સાચું;
    iter_t (અચલ hb_bit_page_t &s_ = Null (hb_bit_page_t), તાર્કિક init = સાચું) : s (&s_), v (INVALID)
    {
      જો (init)
	v = s->get_min ();
    }

    પ્રકારવ્યાખ્યા hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () અચલ { પાછા v; }
    તાર્કિક __more__ () અચલ { પાછા v != INVALID; }
    શૂન્ય __next__ () {
       s->next (&v);
    }
    શૂન્ય __prev__ () { s->previous (&v); }
    iter_t end () અચલ { પાછા iter_t (*s, ખોટું); }
    તાર્કિક operator != (અચલ iter_t& o) અચલ
    { પાછા v != o.v; }

    સુરક્ષિત:
    અચલ hb_bit_page_t *s;
    hb_codepoint_t v;
  };
  iter_t iter () અચલ { પાછા iter_t (*આ); }
  operator iter_t () અચલ { પાછા iter (); }

  સ્થિર constexpr hb_codepoint_t INVALID = HB_SET_કિંમત_INVALID;

  પ્રકારવ્યાખ્યા unsigned long long elt_t;
  સ્થિર constexpr unsigned PAGE_BITS_LOG_2 = 9; // 512 bits
  સ્થિર constexpr unsigned PAGE_BITS = 1 << PAGE_BITS_LOG_2;
  static_assert (1 << PAGE_BITS_LOG_2 == PAGE_BITS, "");
  static_assert ((PAGE_BITS & ((PAGE_BITS) - 1)) == 0, "");
  સ્થિર constexpr unsigned PAGE_BITMASK = PAGE_BITS - 1;

  સ્થિર unsigned પૂર્ણાંક elt_get_min (અચલ elt_t &elt) { પાછા hb_ctz (elt); }
  સ્થિર unsigned પૂર્ણાંક elt_get_max (અચલ elt_t &elt) { પાછા hb_bit_storage (elt) - 1; }

  પ્રકારવ્યાખ્યા hb_vector_size_t<elt_t, PAGE_BITS / 8> vector_t;

  સ્થિર constexpr unsigned ELT_BITS = કદ (elt_t) * 8;
  સ્થિર constexpr unsigned ELT_MASK = ELT_BITS - 1;

  સ્થિર constexpr unsigned BITS = કદ (vector_t) * 8;
  સ્થિર constexpr unsigned MASK = BITS - 1;
  static_assert ((unsigned) PAGE_BITS == (unsigned) BITS, "");

  elt_t &elt (hb_codepoint_t g) { પાછા v[(g & MASK) / ELT_BITS]; }
  અચલ elt_t& elt (hb_codepoint_t g) અચલ { પાછા v[(g & MASK) / ELT_BITS]; }
  સ્થિર constexpr elt_t mask (hb_codepoint_t g) { પાછા elt_t (1) << (g & ELT_MASK); }

  mutable unsigned population;
  vector_t v;
};


#endif /* HB_BIT_PAGE_HH */

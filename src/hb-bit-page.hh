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

#അന്തര്ഭാവ "hb.hh"


/* Compiler-assisted vectorization. */

/* Type behaving similar to vectorized vars defined using __attribute__((vector_size(...))),
 * basically a fixed-size bitset. We can't use the compiler type because hb_vector_t cannot
 * guarantee alignment requirements. */
template <typename elt_t, unsigned പൂർണ്ണസംഖ്യ byte_size>
ഘടന hb_vector_size_t
{
  elt_t& operator [] (unsigned പൂർണ്ണസംഖ്യ i) { തിരികെ v[i]; }
  സ്ഥിരം elt_t& operator [] (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം { തിരികെ v[i]; }

  ശൂന്യം init0 ()
  {
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < ARRAY_LENGTH (v); i++)
      v[i] = 0;
  }
  ശൂന്യം init1 ()
  {
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < ARRAY_LENGTH (v); i++)
      v[i] = (elt_t) -1;
  }

  template <typename Op>
  hb_vector_size_t process (സ്ഥിരം Op& op) സ്ഥിരം
  {
    hb_vector_size_t r;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < ARRAY_LENGTH (v); i++)
      r.v[i] = op (v[i]);
    തിരികെ r;
  }
  template <typename Op>
  hb_vector_size_t process (സ്ഥിരം Op& op, സ്ഥിരം hb_vector_size_t &o) സ്ഥിരം
  {
    hb_vector_size_t r;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < ARRAY_LENGTH (v); i++)
      r.v[i] = op (v[i], o.v[i]);
    തിരികെ r;
  }
  hb_vector_size_t operator | (സ്ഥിരം hb_vector_size_t &o) സ്ഥിരം
  { തിരികെ process (hb_bitwise_or, o); }
  hb_vector_size_t operator & (സ്ഥിരം hb_vector_size_t &o) സ്ഥിരം
  { തിരികെ process (hb_bitwise_and, o); }
  hb_vector_size_t operator ^ (സ്ഥിരം hb_vector_size_t &o) സ്ഥിരം
  { തിരികെ process (hb_bitwise_xor, o); }
  hb_vector_size_t operator ~ () സ്ഥിരം
  { തിരികെ process (hb_bitwise_neg); }

  operator തർക്കം () സ്ഥിരം
  {
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < ARRAY_LENGTH (v); i++)
      എങ്കിൽ (v[i])
	തിരികെ സത്യം;
    തിരികെ അസത്യം;
  }
  operator unsigned പൂർണ്ണസംഖ്യ () സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ r = 0;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < ARRAY_LENGTH (v); i++)
      r += hb_popcount (v[i]);
    തിരികെ r;
  }
  തർക്കം operator == (സ്ഥിരം hb_vector_size_t &o) സ്ഥിരം
  {
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < ARRAY_LENGTH (v); i++)
      എങ്കിൽ (v[i] != o.v[i])
	തിരികെ അസത്യം;
    തിരികെ സത്യം;
  }

  hb_array_t<സ്ഥിരം elt_t> iter () സ്ഥിരം
  { തിരികെ hb_array (v); }

  സ്വകാര്യ:
  static_assert (0 == byte_size % വലുപ്പം (elt_t), "");
  elt_t v[byte_size / വലുപ്പം (elt_t)];
};


ഘടന hb_bit_page_t
{
  hb_bit_page_t () { init0 (); }

  ശൂന്യം init0 () { v.init0 (); population = 0; }
  ശൂന്യം init1 () { v.init1 (); population = PAGE_BITS; }

  ശൂന്യം dirty () { population = UINT_MAX; }

  സ്ഥിതം വരിയിൽ constexpr unsigned len ()
  { തിരികെ ARRAY_LENGTH_CONST (v); }

  operator തർക്കം () സ്ഥിരം { തിരികെ !is_empty (); }
  തർക്കം is_empty () സ്ഥിരം
  {
    എങ്കിൽ (has_population ()) തിരികെ !population;
    തർക്കം empty = !v;
    എങ്കിൽ (empty) population = 0;
    തിരികെ empty;
  }
  uint32_t hash () സ്ഥിരം
  {
    തിരികെ hb_bytes_t ((സ്ഥിരം അക്ഷരം *) &v, വലുപ്പം (v)).hash ();
  }

  ശൂന്യം add (hb_codepoint_t g) { elt (g) |= mask (g); dirty (); }
  ശൂന്യം add_bits (hb_codepoint_t g, uint64_t bits) { elt (g) |= bits; dirty (); }
  ശൂന്യം del (hb_codepoint_t g) { elt (g) &= ~mask (g); dirty (); }
  ശൂന്യം del_bits (hb_codepoint_t g, uint64_t bits) { elt (g) &= ~bits; dirty (); }
  ശൂന്യം set (hb_codepoint_t g, തർക്കം value) { എങ്കിൽ (value) add (g); അല്ലെങ്കിൽ del (g); }
  തർക്കം get (hb_codepoint_t g) സ്ഥിരം { തിരികെ elt (g) & mask (g); }
  തർക്കം may_have (hb_codepoint_t g) സ്ഥിരം { തിരികെ get (g); }

  തർക്കം operator [] (hb_codepoint_t g) സ്ഥിരം { തിരികെ get (g); }
  തർക്കം operator () (hb_codepoint_t g) സ്ഥിരം { തിരികെ get (g); }
  തർക്കം has (hb_codepoint_t g) സ്ഥിരം { തിരികെ get (g); }

  ശൂന്യം add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    എങ്കിൽ (la == lb)
      *la |= (mask (b) << 1) - mask(a);
    അല്ലെങ്കിൽ എങ്കിൽ (likely (la < lb))
    {
      *la |= ~(mask (a) - 1llu);
      la++;

      hb_memset (la, 0xff, (അക്ഷരം *) lb - (അക്ഷരം *) la);

      *lb |= ((mask (b) << 1) - 1llu);
    }
    അല്ലെങ്കിൽ
      തിരികെ;
    dirty ();
  }
  ശൂന്യം del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    എങ്കിൽ (la == lb)
      *la &= ~((mask (b) << 1llu) - mask(a));
    അല്ലെങ്കിൽ എങ്കിൽ (likely (la < lb))
    {
      *la &= mask (a) - 1;
      la++;

      hb_memset (la, 0, (അക്ഷരം *) lb - (അക്ഷരം *) la);

      *lb &= ~((mask (b) << 1) - 1llu);
    }
    അല്ലെങ്കിൽ
      തിരികെ;
    dirty ();
  }
  ശൂന്യം set_range (hb_codepoint_t a, hb_codepoint_t b, തർക്കം v)
  { എങ്കിൽ (v) add_range (a, b); അല്ലെങ്കിൽ del_range (a, b); }


  // Writes out page values to the array p. Returns the number of values
  // written. At most size codepoints will be written.
  unsigned പൂർണ്ണസംഖ്യ ലിഖ (uint32_t        base,
		      unsigned പൂർണ്ണസംഖ്യ    start_മൂല്യം,
		      hb_codepoint_t *p,
		      unsigned പൂർണ്ണസംഖ്യ    size) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ start_v = start_മൂല്യം / ELT_BITS;
    unsigned പൂർണ്ണസംഖ്യ start_bit = start_മൂല്യം & ELT_MASK;
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = 0;
    വേണ്ടി (unsigned i = start_v; i < len () && എണ്ണം < size; i++)
    {
      elt_t bits = v[i];
      uint32_t v_base = base | (i * ELT_BITS);
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ j = start_bit; j < ELT_BITS && എണ്ണം < size; j++)
      {
	എങ്കിൽ ((elt_t(1) << j) & bits) {
	  *p++ = v_base | j;
	  എണ്ണം++;
	}
      }
      start_bit = 0;
    }
    തിരികെ എണ്ണം;
  }

  // Writes out the values NOT in this page to the array p. Returns the
  // number of values written. At most size codepoints will be written.
  // Returns the number of codepoints written. next_value holds the next value
  // that should be written (if not present in this page). This is used to fill
  // any missing value gaps between this page and the previous page, if any.
  // next_value is updated to one more than the last value present in this page.
  unsigned പൂർണ്ണസംഖ്യ write_inverted (uint32_t        base,
			       unsigned പൂർണ്ണസംഖ്യ    start_മൂല്യം,
			       hb_codepoint_t *p,
			       unsigned പൂർണ്ണസംഖ്യ    size,
			       hb_codepoint_t *next_മൂല്യം) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ start_v = start_മൂല്യം / ELT_BITS;
    unsigned പൂർണ്ണസംഖ്യ start_bit = start_മൂല്യം & ELT_MASK;
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = 0;
    വേണ്ടി (unsigned i = start_v; i < len () && എണ്ണം < size; i++)
    {
      elt_t bits = v[i];
      uint32_t v_offset = i * ELT_BITS;
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ j = start_bit; j < ELT_BITS && എണ്ണം < size; j++)
      {
	എങ്കിൽ ((elt_t(1) << j) & bits)
	{
	  hb_codepoint_t value = base | v_offset | j;
	  // Emit all the missing values from next_value up to value - 1.
	  വേണ്ടി (hb_codepoint_t k = *next_മൂല്യം; k < value && എണ്ണം < size; k++)
	  {
	    *p++ = k;
	    എണ്ണം++;
	  }
	  // Skip over this value;
	  *next_മൂല്യം = value + 1;
	}
      }
      start_bit = 0;
    }
    തിരികെ എണ്ണം;
  }

  തർക്കം operator == (സ്ഥിരം hb_bit_page_t &other) സ്ഥിരം { തിരികെ is_equal (other); }
  തർക്കം is_equal (സ്ഥിരം hb_bit_page_t &other) സ്ഥിരം { തിരികെ v == other.v; }
  തർക്കം intersects (സ്ഥിരം hb_bit_page_t &other) സ്ഥിരം
  {
    വേണ്ടി (unsigned i = 0; i < len (); i++)
      എങ്കിൽ (v[i] & other.v[i])
	തിരികെ സത്യം;
    തിരികെ അസത്യം;
  }
  തർക്കം may_intersect (സ്ഥിരം hb_bit_page_t &other) സ്ഥിരം
  { തിരികെ intersects (other); }

  തർക്കം operator <= (സ്ഥിരം hb_bit_page_t &larger_page) സ്ഥിരം { തിരികെ is_subset (larger_page); }
  തർക്കം is_subset (സ്ഥിരം hb_bit_page_t &larger_page) സ്ഥിരം
  {
    എങ്കിൽ (has_population () && larger_page.has_population () &&
	population > larger_page.population)
      തിരികെ അസത്യം;

    വേണ്ടി (unsigned i = 0; i < len (); i++)
      എങ്കിൽ (~larger_page.v[i] & v[i])
	തിരികെ അസത്യം;
    തിരികെ സത്യം;
  }

  തർക്കം has_population () സ്ഥിരം { തിരികെ population != UINT_MAX; }
  unsigned get_population () സ്ഥിരം
  {
    എങ്കിൽ (has_population ()) തിരികെ population;
    തിരികെ population = v;
  }

  തർക്കം next (hb_codepoint_t *codepoint) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ m = (*codepoint + 1) & MASK;
    എങ്കിൽ (!m)
    {
      *codepoint = INVALID;
      തിരികെ അസത്യം;
    }
    unsigned പൂർണ്ണസംഖ്യ i = m / ELT_BITS;
    unsigned പൂർണ്ണസംഖ്യ j = m & ELT_MASK;

    സ്ഥിരം elt_t vv = v[i] & ~((elt_t (1) << j) - 1);
    വേണ്ടി (സ്ഥിരം elt_t *p = &vv; i < len (); p = ((സ്ഥിരം elt_t *) &v[0]) + (++i))
      എങ്കിൽ (*p)
      {
	*codepoint = i * ELT_BITS + elt_get_min (*p);
	തിരികെ സത്യം;
      }

    *codepoint = INVALID;
    തിരികെ അസത്യം;
  }
  തർക്കം previous (hb_codepoint_t *codepoint) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ m = (*codepoint - 1) & MASK;
    എങ്കിൽ (m == MASK)
    {
      *codepoint = INVALID;
      തിരികെ അസത്യം;
    }
    unsigned പൂർണ്ണസംഖ്യ i = m / ELT_BITS;
    unsigned പൂർണ്ണസംഖ്യ j = m & ELT_MASK;

    /* Fancy mask to avoid shifting by elt_t bitsize, which is undefined. */
    സ്ഥിരം elt_t mask = j < 8 * വലുപ്പം (elt_t) - 1 ?
		       ((elt_t (1) << (j + 1)) - 1) :
		       (elt_t) -1;
    സ്ഥിരം elt_t vv = v[i] & mask;
    സ്ഥിരം elt_t *p = &vv;
    ആകുമ്പോൾ (സത്യം)
    {
      എങ്കിൽ (*p)
      {
	*codepoint = i * ELT_BITS + elt_get_max (*p);
	തിരികെ സത്യം;
      }
      എങ്കിൽ ((പൂർണ്ണസംഖ്യ) i <= 0) നിർത്തുക;
      p = &v[--i];
    }

    *codepoint = INVALID;
    തിരികെ അസത്യം;
  }
  hb_codepoint_t get_min () സ്ഥിരം
  {
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < len (); i++)
      എങ്കിൽ (v[i])
	തിരികെ i * ELT_BITS + elt_get_min (v[i]);
    തിരികെ INVALID;
  }
  hb_codepoint_t get_max () സ്ഥിരം
  {
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = len () - 1; i >= 0; i--)
      എങ്കിൽ (v[i])
	തിരികെ i * ELT_BITS + elt_get_max (v[i]);
    തിരികെ INVALID;
  }

  /*
   * Iterator implementation.
   */
  ഘടന iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    സ്ഥിതം constexpr തർക്കം is_sorted_iterator = സത്യം;
    iter_t (സ്ഥിരം hb_bit_page_t &s_ = Null (hb_bit_page_t), തർക്കം init = സത്യം) : s (&s_), v (INVALID)
    {
      എങ്കിൽ (init)
	v = s->get_min ();
    }

    തരനിർവചനം hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () സ്ഥിരം { തിരികെ v; }
    തർക്കം __more__ () സ്ഥിരം { തിരികെ v != INVALID; }
    ശൂന്യം __next__ () {
       s->next (&v);
    }
    ശൂന്യം __prev__ () { s->previous (&v); }
    iter_t end () സ്ഥിരം { തിരികെ iter_t (*s, അസത്യം); }
    തർക്കം operator != (സ്ഥിരം iter_t& o) സ്ഥിരം
    { തിരികെ v != o.v; }

    സംരക്ഷിത:
    സ്ഥിരം hb_bit_page_t *s;
    hb_codepoint_t v;
  };
  iter_t iter () സ്ഥിരം { തിരികെ iter_t (*ഇത്); }
  operator iter_t () സ്ഥിരം { തിരികെ iter (); }

  സ്ഥിതം constexpr hb_codepoint_t INVALID = HB_SET_മൂല്യം_INVALID;

  തരനിർവചനം unsigned long long elt_t;
  സ്ഥിതം constexpr unsigned PAGE_BITS_LOG_2 = 9; // 512 bits
  സ്ഥിതം constexpr unsigned PAGE_BITS = 1 << PAGE_BITS_LOG_2;
  static_assert (1 << PAGE_BITS_LOG_2 == PAGE_BITS, "");
  static_assert ((PAGE_BITS & ((PAGE_BITS) - 1)) == 0, "");
  സ്ഥിതം constexpr unsigned PAGE_BITMASK = PAGE_BITS - 1;

  സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ elt_get_min (സ്ഥിരം elt_t &elt) { തിരികെ hb_ctz (elt); }
  സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ elt_get_max (സ്ഥിരം elt_t &elt) { തിരികെ hb_bit_storage (elt) - 1; }

  തരനിർവചനം hb_vector_size_t<elt_t, PAGE_BITS / 8> vector_t;

  സ്ഥിതം constexpr unsigned ELT_BITS = വലുപ്പം (elt_t) * 8;
  സ്ഥിതം constexpr unsigned ELT_MASK = ELT_BITS - 1;

  സ്ഥിതം constexpr unsigned BITS = വലുപ്പം (vector_t) * 8;
  സ്ഥിതം constexpr unsigned MASK = BITS - 1;
  static_assert ((unsigned) PAGE_BITS == (unsigned) BITS, "");

  elt_t &elt (hb_codepoint_t g) { തിരികെ v[(g & MASK) / ELT_BITS]; }
  സ്ഥിരം elt_t& elt (hb_codepoint_t g) സ്ഥിരം { തിരികെ v[(g & MASK) / ELT_BITS]; }
  സ്ഥിതം constexpr elt_t mask (hb_codepoint_t g) { തിരികെ elt_t (1) << (g & ELT_MASK); }

  mutable unsigned population;
  vector_t v;
};


#endif /* HB_BIT_PAGE_HH */

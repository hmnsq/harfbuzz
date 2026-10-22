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

#अगर_अघोषित HB_BIT_PAGE_HH
#घोषणा HB_BIT_PAGE_HH

#समावेश "hb.hh"


/* Compiler-assisted vectorization. */

/* Type behaving similar to vectorized vars defined using __attribute__((vector_size(...))),
 * basically a fixed-size bitset. We can't use the compiler type because hb_vector_t cannot
 * guarantee alignment requirements. */
ढाँचा <typename elt_t, अचिन्हित पूर्णांक byte_आकार>
काष्ठा hb_vector_आकार_t
{
  elt_t& चालक [] (अचिन्हित पूर्णांक i) { वापस v[i]; }
  स्थिर elt_t& चालक [] (अचिन्हित पूर्णांक i) स्थिर { वापस v[i]; }

  व्योम init0 ()
  {
    क्रम (अचिन्हित पूर्णांक i = 0; i < ARRAY_लंबाई (v); i++)
      v[i] = 0;
  }
  व्योम init1 ()
  {
    क्रम (अचिन्हित पूर्णांक i = 0; i < ARRAY_लंबाई (v); i++)
      v[i] = (elt_t) -1;
  }

  ढाँचा <typename Op>
  hb_vector_आकार_t process (स्थिर Op& op) स्थिर
  {
    hb_vector_आकार_t r;
    क्रम (अचिन्हित पूर्णांक i = 0; i < ARRAY_लंबाई (v); i++)
      r.v[i] = op (v[i]);
    वापस r;
  }
  ढाँचा <typename Op>
  hb_vector_आकार_t process (स्थिर Op& op, स्थिर hb_vector_आकार_t &o) स्थिर
  {
    hb_vector_आकार_t r;
    क्रम (अचिन्हित पूर्णांक i = 0; i < ARRAY_लंबाई (v); i++)
      r.v[i] = op (v[i], o.v[i]);
    वापस r;
  }
  hb_vector_आकार_t चालक | (स्थिर hb_vector_आकार_t &o) स्थिर
  { वापस process (hb_bitwise_or, o); }
  hb_vector_आकार_t चालक & (स्थिर hb_vector_आकार_t &o) स्थिर
  { वापस process (hb_bitwise_and, o); }
  hb_vector_आकार_t चालक ^ (स्थिर hb_vector_आकार_t &o) स्थिर
  { वापस process (hb_bitwise_xor, o); }
  hb_vector_आकार_t चालक ~ () स्थिर
  { वापस process (hb_bitwise_neg); }

  चालक bool () स्थिर
  {
    क्रम (अचिन्हित पूर्णांक i = 0; i < ARRAY_लंबाई (v); i++)
      अगर (v[i])
	वापस true;
    वापस false;
  }
  चालक अचिन्हित पूर्णांक () स्थिर
  {
    अचिन्हित पूर्णांक r = 0;
    क्रम (अचिन्हित पूर्णांक i = 0; i < ARRAY_लंबाई (v); i++)
      r += hb_popcount (v[i]);
    वापस r;
  }
  bool चालक == (स्थिर hb_vector_आकार_t &o) स्थिर
  {
    क्रम (अचिन्हित पूर्णांक i = 0; i < ARRAY_लंबाई (v); i++)
      अगर (v[i] != o.v[i])
	वापस false;
    वापस true;
  }

  hb_array_t<स्थिर elt_t> iter () स्थिर
  { वापस hb_array (v); }

  निजी:
  static_assert (0 == byte_आकार % माप (elt_t), "");
  elt_t v[byte_आकार / माप (elt_t)];
};


काष्ठा hb_bit_page_t
{
  hb_bit_page_t () { init0 (); }

  व्योम init0 () { v.init0 (); population = 0; }
  व्योम init1 () { v.init1 (); population = PAGE_BITS; }

  व्योम dirty () { population = अच_पूर्णांक_उच्च; }

  अटल अंतरभूत constexpr अचिन्हित len ()
  { वापस ARRAY_लंबाई_CONST (v); }

  चालक bool () स्थिर { वापस !is_empty (); }
  bool is_empty () स्थिर
  {
    अगर (has_population ()) वापस !population;
    bool empty = !v;
    अगर (empty) population = 0;
    वापस empty;
  }
  uint32_t hash () स्थिर
  {
    वापस hb_bytes_t ((स्थिर अक्षर *) &v, माप (v)).hash ();
  }

  व्योम add (hb_codepoint_t g) { elt (g) |= mask (g); dirty (); }
  व्योम add_bits (hb_codepoint_t g, uint64_t bits) { elt (g) |= bits; dirty (); }
  व्योम del (hb_codepoint_t g) { elt (g) &= ~mask (g); dirty (); }
  व्योम del_bits (hb_codepoint_t g, uint64_t bits) { elt (g) &= ~bits; dirty (); }
  व्योम set (hb_codepoint_t g, bool value) { अगर (value) add (g); अन्यथा del (g); }
  bool लो (hb_codepoint_t g) स्थिर { वापस elt (g) & mask (g); }
  bool may_have (hb_codepoint_t g) स्थिर { वापस लो (g); }

  bool चालक [] (hb_codepoint_t g) स्थिर { वापस लो (g); }
  bool चालक () (hb_codepoint_t g) स्थिर { वापस लो (g); }
  bool has (hb_codepoint_t g) स्थिर { वापस लो (g); }

  व्योम add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    अगर (la == lb)
      *la |= (mask (b) << 1) - mask(a);
    अन्यथा अगर (likely (la < lb))
    {
      *la |= ~(mask (a) - 1llu);
      la++;

      hb_memset (la, 0xff, (अक्षर *) lb - (अक्षर *) la);

      *lb |= ((mask (b) << 1) - 1llu);
    }
    अन्यथा
      वापस;
    dirty ();
  }
  व्योम del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    अगर (la == lb)
      *la &= ~((mask (b) << 1llu) - mask(a));
    अन्यथा अगर (likely (la < lb))
    {
      *la &= mask (a) - 1;
      la++;

      hb_memset (la, 0, (अक्षर *) lb - (अक्षर *) la);

      *lb &= ~((mask (b) << 1) - 1llu);
    }
    अन्यथा
      वापस;
    dirty ();
  }
  व्योम set_range (hb_codepoint_t a, hb_codepoint_t b, bool v)
  { अगर (v) add_range (a, b); अन्यथा del_range (a, b); }


  // Writes out page values to the array p. Returns the number of values
  // written. At most size codepoints will be written.
  अचिन्हित पूर्णांक ग_लिखो (uint32_t        base,
		      अचिन्हित पूर्णांक    start_मूल्य,
		      hb_codepoint_t *p,
		      अचिन्हित पूर्णांक    आकार) स्थिर
  {
    अचिन्हित पूर्णांक start_v = start_मूल्य / ELT_BITS;
    अचिन्हित पूर्णांक start_bit = start_मूल्य & ELT_MASK;
    अचिन्हित पूर्णांक गिनती = 0;
    क्रम (अचिन्हित i = start_v; i < len () && गिनती < आकार; i++)
    {
      elt_t bits = v[i];
      uint32_t v_base = base | (i * ELT_BITS);
      क्रम (अचिन्हित पूर्णांक j = start_bit; j < ELT_BITS && गिनती < आकार; j++)
      {
	अगर ((elt_t(1) << j) & bits) {
	  *p++ = v_base | j;
	  गिनती++;
	}
      }
      start_bit = 0;
    }
    वापस गिनती;
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
			       अचिन्हित पूर्णांक    आकार,
			       hb_codepoint_t *अग्र_मूल्य) स्थिर
  {
    अचिन्हित पूर्णांक start_v = start_मूल्य / ELT_BITS;
    अचिन्हित पूर्णांक start_bit = start_मूल्य & ELT_MASK;
    अचिन्हित पूर्णांक गिनती = 0;
    क्रम (अचिन्हित i = start_v; i < len () && गिनती < आकार; i++)
    {
      elt_t bits = v[i];
      uint32_t v_offset = i * ELT_BITS;
      क्रम (अचिन्हित पूर्णांक j = start_bit; j < ELT_BITS && गिनती < आकार; j++)
      {
	अगर ((elt_t(1) << j) & bits)
	{
	  hb_codepoint_t value = base | v_offset | j;
	  // Emit all the missing values from next_value up to value - 1.
	  क्रम (hb_codepoint_t k = *अग्र_मूल्य; k < value && गिनती < आकार; k++)
	  {
	    *p++ = k;
	    गिनती++;
	  }
	  // Skip over this value;
	  *अग्र_मूल्य = value + 1;
	}
      }
      start_bit = 0;
    }
    वापस गिनती;
  }

  bool चालक == (स्थिर hb_bit_page_t &other) स्थिर { वापस is_equal (other); }
  bool is_equal (स्थिर hb_bit_page_t &other) स्थिर { वापस v == other.v; }
  bool intersects (स्थिर hb_bit_page_t &other) स्थिर
  {
    क्रम (अचिन्हित i = 0; i < len (); i++)
      अगर (v[i] & other.v[i])
	वापस true;
    वापस false;
  }
  bool may_intersect (स्थिर hb_bit_page_t &other) स्थिर
  { वापस intersects (other); }

  bool चालक <= (स्थिर hb_bit_page_t &larger_page) स्थिर { वापस is_subset (larger_page); }
  bool is_subset (स्थिर hb_bit_page_t &larger_page) स्थिर
  {
    अगर (has_population () && larger_page.has_population () &&
	population > larger_page.population)
      वापस false;

    क्रम (अचिन्हित i = 0; i < len (); i++)
      अगर (~larger_page.v[i] & v[i])
	वापस false;
    वापस true;
  }

  bool has_population () स्थिर { वापस population != अच_पूर्णांक_उच्च; }
  अचिन्हित get_population () स्थिर
  {
    अगर (has_population ()) वापस population;
    वापस population = v;
  }

  bool next (hb_codepoint_t *codepoint) स्थिर
  {
    अचिन्हित पूर्णांक m = (*codepoint + 1) & MASK;
    अगर (!m)
    {
      *codepoint = INVALID;
      वापस false;
    }
    अचिन्हित पूर्णांक i = m / ELT_BITS;
    अचिन्हित पूर्णांक j = m & ELT_MASK;

    स्थिर elt_t vv = v[i] & ~((elt_t (1) << j) - 1);
    क्रम (स्थिर elt_t *p = &vv; i < len (); p = ((स्थिर elt_t *) &v[0]) + (++i))
      अगर (*p)
      {
	*codepoint = i * ELT_BITS + elt_get_min (*p);
	वापस true;
      }

    *codepoint = INVALID;
    वापस false;
  }
  bool previous (hb_codepoint_t *codepoint) स्थिर
  {
    अचिन्हित पूर्णांक m = (*codepoint - 1) & MASK;
    अगर (m == MASK)
    {
      *codepoint = INVALID;
      वापस false;
    }
    अचिन्हित पूर्णांक i = m / ELT_BITS;
    अचिन्हित पूर्णांक j = m & ELT_MASK;

    /* Fancy mask to avoid shifting by elt_t bitsize, which is undefined. */
    स्थिर elt_t mask = j < 8 * माप (elt_t) - 1 ?
		       ((elt_t (1) << (j + 1)) - 1) :
		       (elt_t) -1;
    स्थिर elt_t vv = v[i] & mask;
    स्थिर elt_t *p = &vv;
    जबतक (true)
    {
      अगर (*p)
      {
	*codepoint = i * ELT_BITS + elt_get_max (*p);
	वापस true;
      }
      अगर ((पूर्णांक) i <= 0) अवरोध;
      p = &v[--i];
    }

    *codepoint = INVALID;
    वापस false;
  }
  hb_codepoint_t get_min () स्थिर
  {
    क्रम (अचिन्हित पूर्णांक i = 0; i < len (); i++)
      अगर (v[i])
	वापस i * ELT_BITS + elt_get_min (v[i]);
    वापस INVALID;
  }
  hb_codepoint_t get_max () स्थिर
  {
    क्रम (पूर्णांक i = len () - 1; i >= 0; i--)
      अगर (v[i])
	वापस i * ELT_BITS + elt_get_max (v[i]);
    वापस INVALID;
  }

  /*
   * Iterator implementation.
   */
  काष्ठा iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    अटल constexpr bool is_sorted_iterator = true;
    iter_t (स्थिर hb_bit_page_t &s_ = Null (hb_bit_page_t), bool init = true) : s (&s_), v (INVALID)
    {
      अगर (init)
	v = s->get_min ();
    }

    प्रकार hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () स्थिर { वापस v; }
    bool __more__ () स्थिर { वापस v != INVALID; }
    व्योम __next__ () {
       s->next (&v);
    }
    व्योम __prev__ () { s->previous (&v); }
    iter_t end () स्थिर { वापस iter_t (*s, false); }
    bool चालक != (स्थिर iter_t& o) स्थिर
    { वापस v != o.v; }

    रक्षित:
    स्थिर hb_bit_page_t *s;
    hb_codepoint_t v;
  };
  iter_t iter () स्थिर { वापस iter_t (*यह); }
  चालक iter_t () स्थिर { वापस iter (); }

  अटल constexpr hb_codepoint_t INVALID = HB_SET_मूल्य_INVALID;

  प्रकार अचिन्हित दीर्घ दीर्घ elt_t;
  अटल constexpr अचिन्हित PAGE_BITS_LOG_2 = 9; // 512 bits
  अटल constexpr अचिन्हित PAGE_BITS = 1 << PAGE_BITS_LOG_2;
  static_assert (1 << PAGE_BITS_LOG_2 == PAGE_BITS, "");
  static_assert ((PAGE_BITS & ((PAGE_BITS) - 1)) == 0, "");
  अटल constexpr अचिन्हित PAGE_BITMASK = PAGE_BITS - 1;

  अटल अचिन्हित पूर्णांक elt_get_min (स्थिर elt_t &elt) { वापस hb_ctz (elt); }
  अटल अचिन्हित पूर्णांक elt_get_max (स्थिर elt_t &elt) { वापस hb_bit_storage (elt) - 1; }

  प्रकार hb_vector_आकार_t<elt_t, PAGE_BITS / 8> vector_t;

  अटल constexpr अचिन्हित ELT_BITS = माप (elt_t) * 8;
  अटल constexpr अचिन्हित ELT_MASK = ELT_BITS - 1;

  अटल constexpr अचिन्हित BITS = माप (vector_t) * 8;
  अटल constexpr अचिन्हित MASK = BITS - 1;
  static_assert ((अचिन्हित) PAGE_BITS == (अचिन्हित) BITS, "");

  elt_t &elt (hb_codepoint_t g) { वापस v[(g & MASK) / ELT_BITS]; }
  स्थिर elt_t& elt (hb_codepoint_t g) स्थिर { वापस v[(g & MASK) / ELT_BITS]; }
  अटल constexpr elt_t mask (hb_codepoint_t g) { वापस elt_t (1) << (g & ELT_MASK); }

  mutable अचिन्हित population;
  vector_t v;
};


#पूर्ण_अगर /* HB_BIT_PAGE_HH */

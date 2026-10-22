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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"


/* Compiler-assisted vectorization. */

/* Type behaving similar to vectorized vars defined using __attribute__((vector_size(...))),
 * basically a fixed-size bitset. We can't use the compiler type because hb_vector_t cannot
 * guarantee alignment requirements. */
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ elt_t, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ byte_ਮਾਪ>
ਬਣਤਰ hb_ਸਦਿਸ਼_ਮਾਪ_t
{
  elt_t& ਸੰਚਾਲਕ [] (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) { ਵਾਪਸ v[i]; }
  ਅਚਲ elt_t& ਸੰਚਾਲਕ [] (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ { ਵਾਪਸ v[i]; }

  ਖਾਲੀ init0 ()
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ARRAY_ਲੰਮਾਈ (v); i++)
      v[i] = 0;
  }
  ਖਾਲੀ init1 ()
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ARRAY_ਲੰਮਾਈ (v); i++)
      v[i] = (elt_t) -1;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Op>
  hb_ਸਦਿਸ਼_ਮਾਪ_t process (ਅਚਲ Op& op) ਅਚਲ
  {
    hb_ਸਦਿਸ਼_ਮਾਪ_t r;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ARRAY_ਲੰਮਾਈ (v); i++)
      r.v[i] = op (v[i]);
    ਵਾਪਸ r;
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Op>
  hb_ਸਦਿਸ਼_ਮਾਪ_t process (ਅਚਲ Op& op, ਅਚਲ hb_ਸਦਿਸ਼_ਮਾਪ_t &o) ਅਚਲ
  {
    hb_ਸਦਿਸ਼_ਮਾਪ_t r;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ARRAY_ਲੰਮਾਈ (v); i++)
      r.v[i] = op (v[i], o.v[i]);
    ਵਾਪਸ r;
  }
  hb_ਸਦਿਸ਼_ਮਾਪ_t ਸੰਚਾਲਕ | (ਅਚਲ hb_ਸਦਿਸ਼_ਮਾਪ_t &o) ਅਚਲ
  { ਵਾਪਸ process (hb_bitwise_or, o); }
  hb_ਸਦਿਸ਼_ਮਾਪ_t ਸੰਚਾਲਕ & (ਅਚਲ hb_ਸਦਿਸ਼_ਮਾਪ_t &o) ਅਚਲ
  { ਵਾਪਸ process (hb_bitwise_and, o); }
  hb_ਸਦਿਸ਼_ਮਾਪ_t ਸੰਚਾਲਕ ^ (ਅਚਲ hb_ਸਦਿਸ਼_ਮਾਪ_t &o) ਅਚਲ
  { ਵਾਪਸ process (hb_bitwise_xor, o); }
  hb_ਸਦਿਸ਼_ਮਾਪ_t ਸੰਚਾਲਕ ~ () ਅਚਲ
  { ਵਾਪਸ process (hb_bitwise_neg); }

  ਸੰਚਾਲਕ ਤਾਰਕਿਕ () ਅਚਲ
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ARRAY_ਲੰਮਾਈ (v); i++)
      ਜੇ (v[i])
	ਵਾਪਸ ਸੱਚ;
    ਵਾਪਸ ਝੂਠ;
  }
  ਸੰਚਾਲਕ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ () ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ r = 0;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ARRAY_ਲੰਮਾਈ (v); i++)
      r += hb_popcount (v[i]);
    ਵਾਪਸ r;
  }
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ hb_ਸਦਿਸ਼_ਮਾਪ_t &o) ਅਚਲ
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ARRAY_ਲੰਮਾਈ (v); i++)
      ਜੇ (v[i] != o.v[i])
	ਵਾਪਸ ਝੂਠ;
    ਵਾਪਸ ਸੱਚ;
  }

  hb_array_t<ਅਚਲ elt_t> iter () ਅਚਲ
  { ਵਾਪਸ hb_array (v); }

  ਨਿਜੀ:
  static_assert (0 == byte_ਮਾਪ % ਆਕਾਰ (elt_t), "");
  elt_t v[byte_ਮਾਪ / ਆਕਾਰ (elt_t)];
};


ਬਣਤਰ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t
{
  hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t () { init0 (); }

  ਖਾਲੀ init0 () { v.init0 (); population = 0; }
  ਖਾਲੀ init1 () { v.init1 (); population = ਪੰਨਾ_ਬਿੱਟ; }

  ਖਾਲੀ dirty () { population = UINT_MAX; }

  ਸਥਿਰ ਸਤਰਅੰਦਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ len ()
  { ਵਾਪਸ ARRAY_ਲੰਮਾਈ_CONST (v); }

  ਸੰਚਾਲਕ ਤਾਰਕਿਕ () ਅਚਲ { ਵਾਪਸ !is_empty (); }
  ਤਾਰਕਿਕ is_empty () ਅਚਲ
  {
    ਜੇ (has_population ()) ਵਾਪਸ !population;
    ਤਾਰਕਿਕ ਖਾਲੀ_ਥਾਵਾਂ_2 = !v;
    ਜੇ (ਖਾਲੀ_ਥਾਵਾਂ_2) population = 0;
    ਵਾਪਸ ਖਾਲੀ_ਥਾਵਾਂ_2;
  }
  uint32_t hash () ਅਚਲ
  {
    ਵਾਪਸ hb_ਬਾਈਟਾਂ_t ((ਅਚਲ ਅੱਖਰ *) &v, ਆਕਾਰ (v)).hash ();
  }

  ਖਾਲੀ ਜੋੜੋ (hb_codepoint_t g) { elt (g) |= ਨਕਾਬ (g); dirty (); }
  ਖਾਲੀ ਜੋੜੋ_ਬਿੱਟ (hb_codepoint_t g, uint64_t ਬਿੱਟ) { elt (g) |= ਬਿੱਟ; dirty (); }
  ਖਾਲੀ del (hb_codepoint_t g) { elt (g) &= ~ਨਕਾਬ (g); dirty (); }
  ਖਾਲੀ del_ਬਿੱਟ (hb_codepoint_t g, uint64_t ਬਿੱਟ) { elt (g) &= ~ਬਿੱਟ; dirty (); }
  ਖਾਲੀ ਸੈੱਟ (hb_codepoint_t g, ਤਾਰਕਿਕ value) { ਜੇ (value) ਜੋੜੋ (g); ਨਹੀਂਤਾਂ del (g); }
  ਤਾਰਕਿਕ get (hb_codepoint_t g) ਅਚਲ { ਵਾਪਸ elt (g) & ਨਕਾਬ (g); }
  ਤਾਰਕਿਕ may_have (hb_codepoint_t g) ਅਚਲ { ਵਾਪਸ get (g); }

  ਤਾਰਕਿਕ ਸੰਚਾਲਕ [] (hb_codepoint_t g) ਅਚਲ { ਵਾਪਸ get (g); }
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ () (hb_codepoint_t g) ਅਚਲ { ਵਾਪਸ get (g); }
  ਤਾਰਕਿਕ has (hb_codepoint_t g) ਅਚਲ { ਵਾਪਸ get (g); }

  ਖਾਲੀ ਜੋੜੋ_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    ਜੇ (la == lb)
      *la |= (ਨਕਾਬ (b) << 1) - ਨਕਾਬ(a);
    ਨਹੀਂਤਾਂ ਜੇ (likely (la < lb))
    {
      *la |= ~(ਨਕਾਬ (a) - 1llu);
      la++;

      hb_memset (la, 0xff, (ਅੱਖਰ *) lb - (ਅੱਖਰ *) la);

      *lb |= ((ਨਕਾਬ (b) << 1) - 1llu);
    }
    ਨਹੀਂਤਾਂ
      ਵਾਪਸ;
    dirty ();
  }
  ਖਾਲੀ del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    ਜੇ (la == lb)
      *la &= ~((ਨਕਾਬ (b) << 1llu) - ਨਕਾਬ(a));
    ਨਹੀਂਤਾਂ ਜੇ (likely (la < lb))
    {
      *la &= ਨਕਾਬ (a) - 1;
      la++;

      hb_memset (la, 0, (ਅੱਖਰ *) lb - (ਅੱਖਰ *) la);

      *lb &= ~((ਨਕਾਬ (b) << 1) - 1llu);
    }
    ਨਹੀਂਤਾਂ
      ਵਾਪਸ;
    dirty ();
  }
  ਖਾਲੀ ਸੈੱਟ_range (hb_codepoint_t a, hb_codepoint_t b, ਤਾਰਕਿਕ v)
  { ਜੇ (v) ਜੋੜੋ_range (a, b); ਨਹੀਂਤਾਂ del_range (a, b); }


  // Writes out page values to the array p. Returns the number of values
  // written. At most size codepoints will be written.
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਲਿਖ (uint32_t        ਅਧਾਰ,
		      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    ਸ਼ੁਰੂ_ਮੁੱਲ,
		      hb_codepoint_t *p,
		      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    ਮਾਪ) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_v = ਸ਼ੁਰੂ_ਮੁੱਲ / ELT_ਬਿੱਟ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਦੋਆਧਾਰੀ_ਅੰਕ = ਸ਼ੁਰੂ_ਮੁੱਲ & ELT_ਨਕਾਬ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = 0;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = ਸ਼ੁਰੂ_v; i < len () && ਗਿਣਤੀ_2 < ਮਾਪ; i++)
    {
      elt_t ਬਿੱਟ = v[i];
      uint32_t v_ਅਧਾਰ = ਅਧਾਰ | (i * ELT_ਬਿੱਟ);
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = ਸ਼ੁਰੂ_ਦੋਆਧਾਰੀ_ਅੰਕ; j < ELT_ਬਿੱਟ && ਗਿਣਤੀ_2 < ਮਾਪ; j++)
      {
	ਜੇ ((elt_t(1) << j) & ਬਿੱਟ) {
	  *p++ = v_ਅਧਾਰ | j;
	  ਗਿਣਤੀ_2++;
	}
      }
      ਸ਼ੁਰੂ_ਦੋਆਧਾਰੀ_ਅੰਕ = 0;
    }
    ਵਾਪਸ ਗਿਣਤੀ_2;
  }

  // Writes out the values NOT in this page to the array p. Returns the
  // number of values written. At most size codepoints will be written.
  // Returns the number of codepoints written. next_value holds the next value
  // that should be written (if not present in this page). This is used to fill
  // any missing value gaps between this page and the previous page, if any.
  // next_value is updated to one more than the last value present in this page.
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ write_inverted (uint32_t        ਅਧਾਰ,
			       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    ਸ਼ੁਰੂ_ਮੁੱਲ,
			       hb_codepoint_t *p,
			       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    ਮਾਪ,
			       hb_codepoint_t *ਅਗਲਾ_ਮੁੱਲ) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_v = ਸ਼ੁਰੂ_ਮੁੱਲ / ELT_ਬਿੱਟ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਦੋਆਧਾਰੀ_ਅੰਕ = ਸ਼ੁਰੂ_ਮੁੱਲ & ELT_ਨਕਾਬ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = 0;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = ਸ਼ੁਰੂ_v; i < len () && ਗਿਣਤੀ_2 < ਮਾਪ; i++)
    {
      elt_t ਬਿੱਟ = v[i];
      uint32_t v_ਵਿੱਥ = i * ELT_ਬਿੱਟ;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = ਸ਼ੁਰੂ_ਦੋਆਧਾਰੀ_ਅੰਕ; j < ELT_ਬਿੱਟ && ਗਿਣਤੀ_2 < ਮਾਪ; j++)
      {
	ਜੇ ((elt_t(1) << j) & ਬਿੱਟ)
	{
	  hb_codepoint_t value = ਅਧਾਰ | v_ਵਿੱਥ | j;
	  // Emit all the missing values from next_value up to value - 1.
	  ਲਈ (hb_codepoint_t k = *ਅਗਲਾ_ਮੁੱਲ; k < value && ਗਿਣਤੀ_2 < ਮਾਪ; k++)
	  {
	    *p++ = k;
	    ਗਿਣਤੀ_2++;
	  }
	  // Skip over this value;
	  *ਅਗਲਾ_ਮੁੱਲ = value + 1;
	}
      }
      ਸ਼ੁਰੂ_ਦੋਆਧਾਰੀ_ਅੰਕ = 0;
    }
    ਵਾਪਸ ਗਿਣਤੀ_2;
  }

  ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t &other) ਅਚਲ { ਵਾਪਸ is_equal (other); }
  ਤਾਰਕਿਕ is_equal (ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t &other) ਅਚਲ { ਵਾਪਸ v == other.v; }
  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t &other) ਅਚਲ
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < len (); i++)
      ਜੇ (v[i] & other.v[i])
	ਵਾਪਸ ਸੱਚ;
    ਵਾਪਸ ਝੂਠ;
  }
  ਤਾਰਕਿਕ may_intersect (ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t &other) ਅਚਲ
  { ਵਾਪਸ intersects (other); }

  ਤਾਰਕਿਕ ਸੰਚਾਲਕ <= (ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t &larger_ਪੰਨਾ) ਅਚਲ { ਵਾਪਸ is_subset (larger_ਪੰਨਾ); }
  ਤਾਰਕਿਕ is_subset (ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t &larger_ਪੰਨਾ) ਅਚਲ
  {
    ਜੇ (has_population () && larger_ਪੰਨਾ.has_population () &&
	population > larger_ਪੰਨਾ.population)
      ਵਾਪਸ ਝੂਠ;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < len (); i++)
      ਜੇ (~larger_ਪੰਨਾ.v[i] & v[i])
	ਵਾਪਸ ਝੂਠ;
    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ has_population () ਅਚਲ { ਵਾਪਸ population != UINT_MAX; }
  ਬਿਨਾਨਿਸ਼ਾਨੀ get_population () ਅਚਲ
  {
    ਜੇ (has_population ()) ਵਾਪਸ population;
    ਵਾਪਸ population = v;
  }

  ਤਾਰਕਿਕ next (hb_codepoint_t *codepoint) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ m = (*codepoint + 1) & ਨਕਾਬ_2;
    ਜੇ (!m)
    {
      *codepoint = INVALID;
      ਵਾਪਸ ਝੂਠ;
    }
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = m / ELT_ਬਿੱਟ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = m & ELT_ਨਕਾਬ;

    ਅਚਲ elt_t vv = v[i] & ~((elt_t (1) << j) - 1);
    ਲਈ (ਅਚਲ elt_t *p = &vv; i < len (); p = ((ਅਚਲ elt_t *) &v[0]) + (++i))
      ਜੇ (*p)
      {
	*codepoint = i * ELT_ਬਿੱਟ + elt_get_min (*p);
	ਵਾਪਸ ਸੱਚ;
      }

    *codepoint = INVALID;
    ਵਾਪਸ ਝੂਠ;
  }
  ਤਾਰਕਿਕ ਪਿਛਲਾ (hb_codepoint_t *codepoint) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ m = (*codepoint - 1) & ਨਕਾਬ_2;
    ਜੇ (m == ਨਕਾਬ_2)
    {
      *codepoint = INVALID;
      ਵਾਪਸ ਝੂਠ;
    }
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = m / ELT_ਬਿੱਟ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = m & ELT_ਨਕਾਬ;

    /* Fancy mask to avoid shifting by elt_t bitsize, which is undefined. */
    ਅਚਲ elt_t ਨਕਾਬ = j < 8 * ਆਕਾਰ (elt_t) - 1 ?
		       ((elt_t (1) << (j + 1)) - 1) :
		       (elt_t) -1;
    ਅਚਲ elt_t vv = v[i] & ਨਕਾਬ;
    ਅਚਲ elt_t *p = &vv;
    ਜਦੋਂ (ਸੱਚ)
    {
      ਜੇ (*p)
      {
	*codepoint = i * ELT_ਬਿੱਟ + elt_get_max (*p);
	ਵਾਪਸ ਸੱਚ;
      }
      ਜੇ ((ਪੂਰਨ) i <= 0) ਰੋਕੋ;
      p = &v[--i];
    }

    *codepoint = INVALID;
    ਵਾਪਸ ਝੂਠ;
  }
  hb_codepoint_t get_min () ਅਚਲ
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < len (); i++)
      ਜੇ (v[i])
	ਵਾਪਸ i * ELT_ਬਿੱਟ + elt_get_min (v[i]);
    ਵਾਪਸ INVALID;
  }
  hb_codepoint_t get_max () ਅਚਲ
  {
    ਲਈ (ਪੂਰਨ i = len () - 1; i >= 0; i--)
      ਜੇ (v[i])
	ਵਾਪਸ i * ELT_ਬਿੱਟ + elt_get_max (v[i]);
    ਵਾਪਸ INVALID;
  }

  /*
   * Iterator implementation.
   */
  ਬਣਤਰ iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ਸਥਿਰ constexpr ਤਾਰਕਿਕ is_sorted_iterator = ਸੱਚ;
    iter_t (ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t &s_ = Null (hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t), ਤਾਰਕਿਕ init = ਸੱਚ) : s (&s_), v (INVALID)
    {
      ਜੇ (init)
	v = s->get_min ();
    }

    ਕਿਸਮਪਰਿਭਾਸ਼ਾ hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ਅਚਲ { ਵਾਪਸ v; }
    ਤਾਰਕਿਕ __more__ () ਅਚਲ { ਵਾਪਸ v != INVALID; }
    ਖਾਲੀ __next__ () {
       s->next (&v);
    }
    ਖਾਲੀ __prev__ () { s->ਪਿਛਲਾ (&v); }
    iter_t ਸਿਰਾ () ਅਚਲ { ਵਾਪਸ iter_t (*s, ਝੂਠ); }
    ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ iter_t& o) ਅਚਲ
    { ਵਾਪਸ v != o.v; }

    ਸੁਰੱਖਿਅਤ:
    ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t *s;
    hb_codepoint_t v;
  };
  iter_t iter () ਅਚਲ { ਵਾਪਸ iter_t (*ਇਹ); }
  ਸੰਚਾਲਕ iter_t () ਅਚਲ { ਵਾਪਸ iter (); }

  ਸਥਿਰ constexpr hb_codepoint_t INVALID = HB_ਸੈੱਟ_ਮੁੱਲ_INVALID;

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ elt_t;
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੰਨਾ_ਬਿੱਟ_LOG_2 = 9; // 512 bits
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੰਨਾ_ਬਿੱਟ = 1 << ਪੰਨਾ_ਬਿੱਟ_LOG_2;
  static_assert (1 << ਪੰਨਾ_ਬਿੱਟ_LOG_2 == ਪੰਨਾ_ਬਿੱਟ, "");
  static_assert ((ਪੰਨਾ_ਬਿੱਟ & ((ਪੰਨਾ_ਬਿੱਟ) - 1)) == 0, "");
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੰਨਾ_BITMASK = ਪੰਨਾ_ਬਿੱਟ - 1;

  ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ elt_get_min (ਅਚਲ elt_t &elt) { ਵਾਪਸ hb_ctz (elt); }
  ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ elt_get_max (ਅਚਲ elt_t &elt) { ਵਾਪਸ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_storage (elt) - 1; }

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ hb_ਸਦਿਸ਼_ਮਾਪ_t<elt_t, ਪੰਨਾ_ਬਿੱਟ / 8> ਸਦਿਸ਼_t;

  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ ELT_ਬਿੱਟ = ਆਕਾਰ (elt_t) * 8;
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ ELT_ਨਕਾਬ = ELT_ਬਿੱਟ - 1;

  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਿੱਟ_2 = ਆਕਾਰ (ਸਦਿਸ਼_t) * 8;
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ ਨਕਾਬ_2 = ਬਿੱਟ_2 - 1;
  static_assert ((ਬਿਨਾਨਿਸ਼ਾਨੀ) ਪੰਨਾ_ਬਿੱਟ == (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਬਿੱਟ_2, "");

  elt_t &elt (hb_codepoint_t g) { ਵਾਪਸ v[(g & ਨਕਾਬ_2) / ELT_ਬਿੱਟ]; }
  ਅਚਲ elt_t& elt (hb_codepoint_t g) ਅਚਲ { ਵਾਪਸ v[(g & ਨਕਾਬ_2) / ELT_ਬਿੱਟ]; }
  ਸਥਿਰ constexpr elt_t ਨਕਾਬ (hb_codepoint_t g) { ਵਾਪਸ elt_t (1) << (g & ELT_ਨਕਾਬ); }

  ਬਦਲਣਯੋਗ ਬਿਨਾਨਿਸ਼ਾਨੀ population;
  ਸਦਿਸ਼_t v;
};


#ਜੇ_ਅੰਤ /* HB_BIT_PAGE_HH */

/*
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
 * Author(s): Behdad Esfahbod
 */

#ifndef HB_BIT_VECTOR_HH
#define HB_BIT_VECTOR_HH

#അന്തര്ഭാവ "hb.hh"

#അന്തര്ഭാവ "hb-atomic.hh"

ഘടന hb_min_max_t
{
  ശൂന്യം add (hb_codepoint_t v) { min_v = hb_min (min_v, v); max_v = hb_max (max_v, v); }
  ശൂന്യം add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    min_v = hb_min (min_v, a);
    max_v = hb_max (max_v, b);
  }

  template <typename set_t>
  ശൂന്യം union_ (സ്ഥിരം set_t &set)
  {
    hb_codepoint_t set_min = set.get_min ();
    എങ്കിൽ (unlikely (set_min == HB_CODEPOINT_INVALID))
      തിരികെ;
    hb_codepoint_t set_max = set.get_max ();
    min_v = hb_min (min_v, set_min);
    max_v = hb_max (max_v, set_max);
  }

  hb_codepoint_t get_min () സ്ഥിരം { തിരികെ min_v; }
  hb_codepoint_t get_max () സ്ഥിരം { തിരികെ max_v; }

  സ്വകാര്യ:
  hb_codepoint_t min_v = HB_CODEPOINT_INVALID;
  hb_codepoint_t max_v = 0;
};

template <തർക്കം atomic = അസത്യം>
ഘടന hb_bit_vector_t
{
  using int_t = uint64_t;
  using elt_t = typename std::conditional<atomic, hb_atomic_t<int_t>, int_t>::type;

  hb_bit_vector_t () = delete;
  hb_bit_vector_t (സ്ഥിരം hb_bit_vector_t &other) = delete;
  hb_bit_vector_t &operator= (സ്ഥിരം hb_bit_vector_t &other) = delete;

  // Move
  hb_bit_vector_t (hb_bit_vector_t &&other)
		: min_v (other.min_v), max_v (other.max_v), എണ്ണം (other.എണ്ണം), elts (other.elts)
  {
    other.min_v = other.max_v = other.എണ്ണം = 0;
    other.elts = nullptr;
  }
  hb_bit_vector_t &operator= (hb_bit_vector_t &&other)
  {
    hb_swap (min_v, other.min_v);
    hb_swap (max_v, other.max_v);
    hb_swap (എണ്ണം, other.എണ്ണം);
    hb_swap (elts, other.elts);
    തിരികെ *ഇത്;
  }

  hb_bit_vector_t (unsigned min_v, unsigned max_v)
    : min_v (min_v), max_v (max_v)
  {
    എങ്കിൽ (unlikely (min_v >= max_v))
    {
      min_v = max_v = എണ്ണം = 0;
      തിരികെ;
    }

    unsigned അക്കം = (max_v - min_v + വലുപ്പം (int_t) * 8) / (വലുപ്പം (int_t) * 8);
    elts = (elt_t *) hb_calloc (അക്കം, വലുപ്പം (int_t));
    എങ്കിൽ (unlikely (!elts))
    {
      min_v = max_v = എണ്ണം = 0;
      തിരികെ;
    }

    എണ്ണം = max_v - min_v + 1;
  }
  ~hb_bit_vector_t ()
  {
    hb_free (elts);
  }

  ശൂന്യം add (hb_codepoint_t g) { elt (g) |= mask (g); }
  ശൂന്യം del (hb_codepoint_t g) { elt (g) &= ~mask (g); }
  ശൂന്യം set (hb_codepoint_t g, തർക്കം value) { എങ്കിൽ (value) add (g); അല്ലെങ്കിൽ del (g); }
  തർക്കം get (hb_codepoint_t g) സ്ഥിരം { തിരികെ elt (g) & mask (g); }
  തർക്കം has (hb_codepoint_t g) സ്ഥിരം { തിരികെ get (g); }
  തർക്കം may_have (hb_codepoint_t g) സ്ഥിരം { തിരികെ get (g); }

  തർക്കം operator [] (hb_codepoint_t g) സ്ഥിരം { തിരികെ get (g); }
  തർക്കം operator () (hb_codepoint_t g) സ്ഥിരം { തിരികെ get (g); }

  ശൂന്യം add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    എങ്കിൽ (unlikely (!എണ്ണം || a > b || a < min_v || b > max_v))
      തിരികെ;

    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    എങ്കിൽ (la == lb)
      *la |= (mask (b) << 1) - mask(a);
    അല്ലെങ്കിൽ
    {
      *la |= ~(mask (a) - 1llu);
      la++;

      hb_memset (la, 0xff, (അക്ഷരം *) lb - (അക്ഷരം *) la);

      *lb |= ((mask (b) << 1) - 1llu);
    }
  }
  ശൂന്യം del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    എങ്കിൽ (unlikely (!എണ്ണം || a > b || a < min_v || b > max_v))
      തിരികെ;

    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    എങ്കിൽ (la == lb)
      *la &= ~((mask (b) << 1llu) - mask(a));
    അല്ലെങ്കിൽ
    {
      *la &= mask (a) - 1;
      la++;

      hb_memset (la, 0, (അക്ഷരം *) lb - (അക്ഷരം *) la);

      *lb &= ~((mask (b) << 1) - 1llu);
    }
  }
  ശൂന്യം set_range (hb_codepoint_t a, hb_codepoint_t b, തർക്കം v)
  { എങ്കിൽ (v) add_range (a, b); അല്ലെങ്കിൽ del_range (a, b); }

  template <typename set_t>
  ശൂന്യം union_ (സ്ഥിരം set_t &set)
  {
    വേണ്ടി (hb_codepoint_t g : set)
      add (g);
  }

  സ്ഥിതം സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ ELT_BITS = വലുപ്പം (elt_t) * 8;
  സ്ഥിതം constexpr unsigned ELT_MASK = ELT_BITS - 1;

  സ്ഥിതം constexpr elt_t zero = 0;

  elt_t &elt (hb_codepoint_t g)
  {
    g -= min_v;
    എങ്കിൽ (unlikely (g >= എണ്ണം))
      തിരികെ Crap(elt_t);
    തിരികെ elts[g / ELT_BITS];
  }
  സ്ഥിരം elt_t& elt (hb_codepoint_t g) സ്ഥിരം
  {
    g -= min_v;
    എങ്കിൽ (unlikely (g >= എണ്ണം))
      തിരികെ Null(elt_t);
    തിരികെ elts[g / ELT_BITS];
  }

  സ്ഥിതം constexpr int_t mask (hb_codepoint_t g) { തിരികെ elt_t (1) << (g & ELT_MASK); }

  hb_codepoint_t min_v = 0, max_v = 0, എണ്ണം = 0;
  elt_t *elts = nullptr;
};


#endif /* HB_BIT_VECTOR_HH */

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

#અન્તર્ભાવ "hb.hh"

#અન્તર્ભાવ "hb-atomic.hh"

સંરચના hb_min_max_t
{
  શૂન્ય add (hb_codepoint_t v) { min_v = hb_min (min_v, v); max_v = hb_max (max_v, v); }
  શૂન્ય add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    min_v = hb_min (min_v, a);
    max_v = hb_max (max_v, b);
  }

  template <typename set_t>
  શૂન્ય union_ (અચલ set_t &set)
  {
    hb_codepoint_t set_min = set.get_min ();
    જો (unlikely (set_min == HB_CODEPOINT_INVALID))
      પાછા;
    hb_codepoint_t set_max = set.get_max ();
    min_v = hb_min (min_v, set_min);
    max_v = hb_max (max_v, set_max);
  }

  hb_codepoint_t get_min () અચલ { પાછા min_v; }
  hb_codepoint_t get_max () અચલ { પાછા max_v; }

  ખાનગી:
  hb_codepoint_t min_v = HB_CODEPOINT_INVALID;
  hb_codepoint_t max_v = 0;
};

template <તાર્કિક atomic = ખોટું>
સંરચના hb_bit_vector_t
{
  using int_t = uint64_t;
  using elt_t = typename std::conditional<atomic, hb_atomic_t<int_t>, int_t>::type;

  hb_bit_vector_t () = delete;
  hb_bit_vector_t (અચલ hb_bit_vector_t &other) = delete;
  hb_bit_vector_t &operator= (અચલ hb_bit_vector_t &other) = delete;

  // Move
  hb_bit_vector_t (hb_bit_vector_t &&other)
		: min_v (other.min_v), max_v (other.max_v), ગણતરી (other.ગણતરી), elts (other.elts)
  {
    other.min_v = other.max_v = other.ગણતરી = 0;
    other.elts = nullptr;
  }
  hb_bit_vector_t &operator= (hb_bit_vector_t &&other)
  {
    hb_swap (min_v, other.min_v);
    hb_swap (max_v, other.max_v);
    hb_swap (ગણતરી, other.ગણતરી);
    hb_swap (elts, other.elts);
    પાછા *આ;
  }

  hb_bit_vector_t (unsigned min_v, unsigned max_v)
    : min_v (min_v), max_v (max_v)
  {
    જો (unlikely (min_v >= max_v))
    {
      min_v = max_v = ગણતરી = 0;
      પાછા;
    }

    unsigned અંક = (max_v - min_v + કદ (int_t) * 8) / (કદ (int_t) * 8);
    elts = (elt_t *) hb_calloc (અંક, કદ (int_t));
    જો (unlikely (!elts))
    {
      min_v = max_v = ગણતરી = 0;
      પાછા;
    }

    ગણતરી = max_v - min_v + 1;
  }
  ~hb_bit_vector_t ()
  {
    hb_free (elts);
  }

  શૂન્ય add (hb_codepoint_t g) { elt (g) |= mask (g); }
  શૂન્ય del (hb_codepoint_t g) { elt (g) &= ~mask (g); }
  શૂન્ય set (hb_codepoint_t g, તાર્કિક value) { જો (value) add (g); નહીંતર del (g); }
  તાર્કિક get (hb_codepoint_t g) અચલ { પાછા elt (g) & mask (g); }
  તાર્કિક has (hb_codepoint_t g) અચલ { પાછા get (g); }
  તાર્કિક may_have (hb_codepoint_t g) અચલ { પાછા get (g); }

  તાર્કિક operator [] (hb_codepoint_t g) અચલ { પાછા get (g); }
  તાર્કિક operator () (hb_codepoint_t g) અચલ { પાછા get (g); }

  શૂન્ય add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    જો (unlikely (!ગણતરી || a > b || a < min_v || b > max_v))
      પાછા;

    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    જો (la == lb)
      *la |= (mask (b) << 1) - mask(a);
    નહીંતર
    {
      *la |= ~(mask (a) - 1llu);
      la++;

      hb_memset (la, 0xff, (અક્ષર *) lb - (અક્ષર *) la);

      *lb |= ((mask (b) << 1) - 1llu);
    }
  }
  શૂન્ય del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    જો (unlikely (!ગણતરી || a > b || a < min_v || b > max_v))
      પાછા;

    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    જો (la == lb)
      *la &= ~((mask (b) << 1llu) - mask(a));
    નહીંતર
    {
      *la &= mask (a) - 1;
      la++;

      hb_memset (la, 0, (અક્ષર *) lb - (અક્ષર *) la);

      *lb &= ~((mask (b) << 1) - 1llu);
    }
  }
  શૂન્ય set_range (hb_codepoint_t a, hb_codepoint_t b, તાર્કિક v)
  { જો (v) add_range (a, b); નહીંતર del_range (a, b); }

  template <typename set_t>
  શૂન્ય union_ (અચલ set_t &set)
  {
    માટે (hb_codepoint_t g : set)
      add (g);
  }

  સ્થિર અચલ unsigned પૂર્ણાંક ELT_BITS = કદ (elt_t) * 8;
  સ્થિર constexpr unsigned ELT_MASK = ELT_BITS - 1;

  સ્થિર constexpr elt_t zero = 0;

  elt_t &elt (hb_codepoint_t g)
  {
    g -= min_v;
    જો (unlikely (g >= ગણતરી))
      પાછા Crap(elt_t);
    પાછા elts[g / ELT_BITS];
  }
  અચલ elt_t& elt (hb_codepoint_t g) અચલ
  {
    g -= min_v;
    જો (unlikely (g >= ગણતરી))
      પાછા Null(elt_t);
    પાછા elts[g / ELT_BITS];
  }

  સ્થિર constexpr int_t mask (hb_codepoint_t g) { પાછા elt_t (1) << (g & ELT_MASK); }

  hb_codepoint_t min_v = 0, max_v = 0, ગણતરી = 0;
  elt_t *elts = nullptr;
};


#endif /* HB_BIT_VECTOR_HH */

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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ಅನ್ತರ್ಭಾವ "hb-atomic.hh"

ಸಂರಚನಾ hb_min_max_t
{
  ಶೂನ್ಯ add (hb_codepoint_t v) { min_v = hb_min (min_v, v); max_v = hb_max (max_v, v); }
  ಶೂನ್ಯ add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    min_v = hb_min (min_v, a);
    max_v = hb_max (max_v, b);
  }

  template <typename set_t>
  ಶೂನ್ಯ union_ (ಧ್ರುವ set_t &set)
  {
    hb_codepoint_t set_min = set.get_min ();
    ಯದಿ (unlikely (set_min == HB_CODEPOINT_INVALID))
      ಪ್ರತ್ಯಾಗಮ;
    hb_codepoint_t set_max = set.get_max ();
    min_v = hb_min (min_v, set_min);
    max_v = hb_max (max_v, set_max);
  }

  hb_codepoint_t get_min () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ min_v; }
  hb_codepoint_t get_max () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ max_v; }

  ಗುಪ್ತ:
  hb_codepoint_t min_v = HB_CODEPOINT_INVALID;
  hb_codepoint_t max_v = 0;
};

template <ತಾರ್ಕಿಕ atomic = ಅಸತ್ಯ>
ಸಂರಚನಾ hb_bit_vector_t
{
  using int_t = uint64_t;
  using elt_t = typename std::conditional<atomic, hb_atomic_t<int_t>, int_t>::type;

  hb_bit_vector_t () = delete;
  hb_bit_vector_t (ಧ್ರುವ hb_bit_vector_t &other) = delete;
  hb_bit_vector_t &operator= (ಧ್ರುವ hb_bit_vector_t &other) = delete;

  // Move
  hb_bit_vector_t (hb_bit_vector_t &&other)
		: min_v (other.min_v), max_v (other.max_v), ಎಣಿಕೆ (other.ಎಣಿಕೆ), elts (other.elts)
  {
    other.min_v = other.max_v = other.ಎಣಿಕೆ = 0;
    other.elts = nullptr;
  }
  hb_bit_vector_t &operator= (hb_bit_vector_t &&other)
  {
    hb_swap (min_v, other.min_v);
    hb_swap (max_v, other.max_v);
    hb_swap (ಎಣಿಕೆ, other.ಎಣಿಕೆ);
    hb_swap (elts, other.elts);
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
  }

  hb_bit_vector_t (unsigned min_v, unsigned max_v)
    : min_v (min_v), max_v (max_v)
  {
    ಯದಿ (unlikely (min_v >= max_v))
    {
      min_v = max_v = ಎಣಿಕೆ = 0;
      ಪ್ರತ್ಯಾಗಮ;
    }

    unsigned ಅಂಕೆ = (max_v - min_v + ಪರಿಮಾಣ (int_t) * 8) / (ಪರಿಮಾಣ (int_t) * 8);
    elts = (elt_t *) hb_calloc (ಅಂಕೆ, ಪರಿಮಾಣ (int_t));
    ಯದಿ (unlikely (!elts))
    {
      min_v = max_v = ಎಣಿಕೆ = 0;
      ಪ್ರತ್ಯಾಗಮ;
    }

    ಎಣಿಕೆ = max_v - min_v + 1;
  }
  ~hb_bit_vector_t ()
  {
    hb_free (elts);
  }

  ಶೂನ್ಯ add (hb_codepoint_t g) { elt (g) |= mask (g); }
  ಶೂನ್ಯ del (hb_codepoint_t g) { elt (g) &= ~mask (g); }
  ಶೂನ್ಯ set (hb_codepoint_t g, ತಾರ್ಕಿಕ value) { ಯದಿ (value) add (g); ಅನ್ಯಥಾ del (g); }
  ತಾರ್ಕಿಕ get (hb_codepoint_t g) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ elt (g) & mask (g); }
  ತಾರ್ಕಿಕ has (hb_codepoint_t g) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get (g); }
  ತಾರ್ಕಿಕ may_have (hb_codepoint_t g) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get (g); }

  ತಾರ್ಕಿಕ operator [] (hb_codepoint_t g) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get (g); }
  ತಾರ್ಕಿಕ operator () (hb_codepoint_t g) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get (g); }

  ಶೂನ್ಯ add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    ಯದಿ (unlikely (!ಎಣಿಕೆ || a > b || a < min_v || b > max_v))
      ಪ್ರತ್ಯಾಗಮ;

    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    ಯದಿ (la == lb)
      *la |= (mask (b) << 1) - mask(a);
    ಅನ್ಯಥಾ
    {
      *la |= ~(mask (a) - 1llu);
      la++;

      hb_memset (la, 0xff, (ವರ್ಣ *) lb - (ವರ್ಣ *) la);

      *lb |= ((mask (b) << 1) - 1llu);
    }
  }
  ಶೂನ್ಯ del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    ಯದಿ (unlikely (!ಎಣಿಕೆ || a > b || a < min_v || b > max_v))
      ಪ್ರತ್ಯಾಗಮ;

    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    ಯದಿ (la == lb)
      *la &= ~((mask (b) << 1llu) - mask(a));
    ಅನ್ಯಥಾ
    {
      *la &= mask (a) - 1;
      la++;

      hb_memset (la, 0, (ವರ್ಣ *) lb - (ವರ್ಣ *) la);

      *lb &= ~((mask (b) << 1) - 1llu);
    }
  }
  ಶೂನ್ಯ set_range (hb_codepoint_t a, hb_codepoint_t b, ತಾರ್ಕಿಕ v)
  { ಯದಿ (v) add_range (a, b); ಅನ್ಯಥಾ del_range (a, b); }

  template <typename set_t>
  ಶೂನ್ಯ union_ (ಧ್ರುವ set_t &set)
  {
    ಪ್ರತಿ (hb_codepoint_t g : set)
      add (g);
  }

  ಸ್ಥಿರ ಧ್ರುವ unsigned ಪೂರ್ಣಾಙ್ಕ ELT_BITS = ಪರಿಮಾಣ (elt_t) * 8;
  ಸ್ಥಿರ constexpr unsigned ELT_MASK = ELT_BITS - 1;

  ಸ್ಥಿರ constexpr elt_t zero = 0;

  elt_t &elt (hb_codepoint_t g)
  {
    g -= min_v;
    ಯದಿ (unlikely (g >= ಎಣಿಕೆ))
      ಪ್ರತ್ಯಾಗಮ Crap(elt_t);
    ಪ್ರತ್ಯಾಗಮ elts[g / ELT_BITS];
  }
  ಧ್ರುವ elt_t& elt (hb_codepoint_t g) ಧ್ರುವ
  {
    g -= min_v;
    ಯದಿ (unlikely (g >= ಎಣಿಕೆ))
      ಪ್ರತ್ಯಾಗಮ Null(elt_t);
    ಪ್ರತ್ಯಾಗಮ elts[g / ELT_BITS];
  }

  ಸ್ಥಿರ constexpr int_t mask (hb_codepoint_t g) { ಪ್ರತ್ಯಾಗಮ elt_t (1) << (g & ELT_MASK); }

  hb_codepoint_t min_v = 0, max_v = 0, ಎಣಿಕೆ = 0;
  elt_t *elts = nullptr;
};


#endif /* HB_BIT_VECTOR_HH */

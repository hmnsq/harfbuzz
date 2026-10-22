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

#অন্তর্ভাৱ "hb.hh"

#অন্তর্ভাৱ "hb-atomic.hh"

সংরচনা hb_min_max_t
{
  শূন্য add (hb_codepoint_t v) { min_v = hb_min (min_v, v); max_v = hb_max (max_v, v); }
  শূন্য add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    min_v = hb_min (min_v, a);
    max_v = hb_max (max_v, b);
  }

  template <typename set_t>
  শূন্য union_ (ধ্রুৱ set_t &set)
  {
    hb_codepoint_t set_min = set.get_min ();
    যদি (unlikely (set_min == HB_CODEPOINT_INVALID))
      ঘূৰাই;
    hb_codepoint_t set_max = set.get_max ();
    min_v = hb_min (min_v, set_min);
    max_v = hb_max (max_v, set_max);
  }

  hb_codepoint_t get_min () ধ্রুৱ { ঘূৰাই min_v; }
  hb_codepoint_t get_max () ধ্রুৱ { ঘূৰাই max_v; }

  গুপ্ত:
  hb_codepoint_t min_v = HB_CODEPOINT_INVALID;
  hb_codepoint_t max_v = 0;
};

template <তার্কিক atomic = মিছা>
সংরচনা hb_bit_vector_t
{
  using int_t = uint64_t;
  using elt_t = typename std::conditional<atomic, hb_atomic_t<int_t>, int_t>::type;

  hb_bit_vector_t () = delete;
  hb_bit_vector_t (ধ্রুৱ hb_bit_vector_t &other) = delete;
  hb_bit_vector_t &operator= (ধ্রুৱ hb_bit_vector_t &other) = delete;

  // Move
  hb_bit_vector_t (hb_bit_vector_t &&other)
		: min_v (other.min_v), max_v (other.max_v), গুনতি (other.গুনতি), elts (other.elts)
  {
    other.min_v = other.max_v = other.গুনতি = 0;
    other.elts = nullptr;
  }
  hb_bit_vector_t &operator= (hb_bit_vector_t &&other)
  {
    hb_swap (min_v, other.min_v);
    hb_swap (max_v, other.max_v);
    hb_swap (গুনতি, other.গুনতি);
    hb_swap (elts, other.elts);
    ঘূৰাই *এতত্;
  }

  hb_bit_vector_t (unsigned min_v, unsigned max_v)
    : min_v (min_v), max_v (max_v)
  {
    যদি (unlikely (min_v >= max_v))
    {
      min_v = max_v = গুনতি = 0;
      ঘূৰাই;
    }

    unsigned অঙ্ক = (max_v - min_v + পরিমাণ (int_t) * 8) / (পরিমাণ (int_t) * 8);
    elts = (elt_t *) hb_calloc (অঙ্ক, পরিমাণ (int_t));
    যদি (unlikely (!elts))
    {
      min_v = max_v = গুনতি = 0;
      ঘূৰাই;
    }

    গুনতি = max_v - min_v + 1;
  }
  ~hb_bit_vector_t ()
  {
    hb_free (elts);
  }

  শূন্য add (hb_codepoint_t g) { elt (g) |= mask (g); }
  শূন্য del (hb_codepoint_t g) { elt (g) &= ~mask (g); }
  শূন্য set (hb_codepoint_t g, তার্কিক value) { যদি (value) add (g); অন্যথা del (g); }
  তার্কিক get (hb_codepoint_t g) ধ্রুৱ { ঘূৰাই elt (g) & mask (g); }
  তার্কিক has (hb_codepoint_t g) ধ্রুৱ { ঘূৰাই get (g); }
  তার্কিক may_have (hb_codepoint_t g) ধ্রুৱ { ঘূৰাই get (g); }

  তার্কিক operator [] (hb_codepoint_t g) ধ্রুৱ { ঘূৰাই get (g); }
  তার্কিক operator () (hb_codepoint_t g) ধ্রুৱ { ঘূৰাই get (g); }

  শূন্য add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    যদি (unlikely (!গুনতি || a > b || a < min_v || b > max_v))
      ঘূৰাই;

    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    যদি (la == lb)
      *la |= (mask (b) << 1) - mask(a);
    অন্যথা
    {
      *la |= ~(mask (a) - 1llu);
      la++;

      hb_memset (la, 0xff, (ৱর্ণ *) lb - (ৱর্ণ *) la);

      *lb |= ((mask (b) << 1) - 1llu);
    }
  }
  শূন্য del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    যদি (unlikely (!গুনতি || a > b || a < min_v || b > max_v))
      ঘূৰাই;

    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    যদি (la == lb)
      *la &= ~((mask (b) << 1llu) - mask(a));
    অন্যথা
    {
      *la &= mask (a) - 1;
      la++;

      hb_memset (la, 0, (ৱর্ণ *) lb - (ৱর্ণ *) la);

      *lb &= ~((mask (b) << 1) - 1llu);
    }
  }
  শূন্য set_range (hb_codepoint_t a, hb_codepoint_t b, তার্কিক v)
  { যদি (v) add_range (a, b); অন্যথা del_range (a, b); }

  template <typename set_t>
  শূন্য union_ (ধ্রুৱ set_t &set)
  {
    বাবে (hb_codepoint_t g : set)
      add (g);
  }

  স্থির ধ্রুৱ unsigned পূর্ণাঙ্ক ELT_BITS = পরিমাণ (elt_t) * 8;
  স্থির constexpr unsigned ELT_MASK = ELT_BITS - 1;

  স্থির constexpr elt_t zero = 0;

  elt_t &elt (hb_codepoint_t g)
  {
    g -= min_v;
    যদি (unlikely (g >= গুনতি))
      ঘূৰাই Crap(elt_t);
    ঘূৰাই elts[g / ELT_BITS];
  }
  ধ্রুৱ elt_t& elt (hb_codepoint_t g) ধ্রুৱ
  {
    g -= min_v;
    যদি (unlikely (g >= গুনতি))
      ঘূৰাই Null(elt_t);
    ঘূৰাই elts[g / ELT_BITS];
  }

  স্থির constexpr int_t mask (hb_codepoint_t g) { ঘূৰাই elt_t (1) << (g & ELT_MASK); }

  hb_codepoint_t min_v = 0, max_v = 0, গুনতি = 0;
  elt_t *elts = nullptr;
};


#endif /* HB_BIT_VECTOR_HH */

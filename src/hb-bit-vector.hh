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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ଅନ୍ତର୍ଭାଵ "hb-atomic.hh"

ସଂରଚନା hb_min_max_t
{
  ଶୂନ୍ୟ add (hb_codepoint_t v) { min_v = hb_min (min_v, v); max_v = hb_max (max_v, v); }
  ଶୂନ୍ୟ add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    min_v = hb_min (min_v, a);
    max_v = hb_max (max_v, b);
  }

  template <typename set_t>
  ଶୂନ୍ୟ union_ (ଅଚଳ set_t &set)
  {
    hb_codepoint_t set_min = set.get_min ();
    ଯଦି (unlikely (set_min == HB_CODEPOINT_INVALID))
      ଫେରନ୍ତୁ;
    hb_codepoint_t set_max = set.get_max ();
    min_v = hb_min (min_v, set_min);
    max_v = hb_max (max_v, set_max);
  }

  hb_codepoint_t get_min () ଅଚଳ { ଫେରନ୍ତୁ min_v; }
  hb_codepoint_t get_max () ଅଚଳ { ଫେରନ୍ତୁ max_v; }

  ବ୍ୟକ୍ତିଗତ:
  hb_codepoint_t min_v = HB_CODEPOINT_INVALID;
  hb_codepoint_t max_v = 0;
};

template <ତାର୍କିକ atomic = ମିଥ୍ୟା>
ସଂରଚନା hb_bit_vector_t
{
  using int_t = uint64_t;
  using elt_t = typename std::conditional<atomic, hb_atomic_t<int_t>, int_t>::type;

  hb_bit_vector_t () = delete;
  hb_bit_vector_t (ଅଚଳ hb_bit_vector_t &other) = delete;
  hb_bit_vector_t &operator= (ଅଚଳ hb_bit_vector_t &other) = delete;

  // Move
  hb_bit_vector_t (hb_bit_vector_t &&other)
		: min_v (other.min_v), max_v (other.max_v), ଗଣନା_2 (other.ଗଣନା_2), elts (other.elts)
  {
    other.min_v = other.max_v = other.ଗଣନା_2 = 0;
    other.elts = nullptr;
  }
  hb_bit_vector_t &operator= (hb_bit_vector_t &&other)
  {
    hb_swap (min_v, other.min_v);
    hb_swap (max_v, other.max_v);
    hb_swap (ଗଣନା_2, other.ଗଣନା_2);
    hb_swap (elts, other.elts);
    ଫେରନ୍ତୁ *ଏହି;
  }

  hb_bit_vector_t (unsigned min_v, unsigned max_v)
    : min_v (min_v), max_v (max_v)
  {
    ଯଦି (unlikely (min_v >= max_v))
    {
      min_v = max_v = ଗଣନା_2 = 0;
      ଫେରନ୍ତୁ;
    }

    unsigned ଅଙ୍କ = (max_v - min_v + ଆକାର (int_t) * 8) / (ଆକାର (int_t) * 8);
    elts = (elt_t *) hb_calloc (ଅଙ୍କ, ଆକାର (int_t));
    ଯଦି (unlikely (!elts))
    {
      min_v = max_v = ଗଣନା_2 = 0;
      ଫେରନ୍ତୁ;
    }

    ଗଣନା_2 = max_v - min_v + 1;
  }
  ~hb_bit_vector_t ()
  {
    hb_free (elts);
  }

  ଶୂନ୍ୟ add (hb_codepoint_t g) { elt (g) |= mask (g); }
  ଶୂନ୍ୟ del (hb_codepoint_t g) { elt (g) &= ~mask (g); }
  ଶୂନ୍ୟ set (hb_codepoint_t g, ତାର୍କିକ value) { ଯଦି (value) add (g); ଅନ୍ୟଥା del (g); }
  ତାର୍କିକ get (hb_codepoint_t g) ଅଚଳ { ଫେରନ୍ତୁ elt (g) & mask (g); }
  ତାର୍କିକ has (hb_codepoint_t g) ଅଚଳ { ଫେରନ୍ତୁ get (g); }
  ତାର୍କିକ may_have (hb_codepoint_t g) ଅଚଳ { ଫେରନ୍ତୁ get (g); }

  ତାର୍କିକ operator [] (hb_codepoint_t g) ଅଚଳ { ଫେରନ୍ତୁ get (g); }
  ତାର୍କିକ operator () (hb_codepoint_t g) ଅଚଳ { ଫେରନ୍ତୁ get (g); }

  ଶୂନ୍ୟ add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    ଯଦି (unlikely (!ଗଣନା_2 || a > b || a < min_v || b > max_v))
      ଫେରନ୍ତୁ;

    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    ଯଦି (la == lb)
      *la |= (mask (b) << 1) - mask(a);
    ଅନ୍ୟଥା
    {
      *la |= ~(mask (a) - 1llu);
      la++;

      hb_memset (la, 0xff, (ଅକ୍ଷର *) lb - (ଅକ୍ଷର *) la);

      *lb |= ((mask (b) << 1) - 1llu);
    }
  }
  ଶୂନ୍ୟ del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    ଯଦି (unlikely (!ଗଣନା_2 || a > b || a < min_v || b > max_v))
      ଫେରନ୍ତୁ;

    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    ଯଦି (la == lb)
      *la &= ~((mask (b) << 1llu) - mask(a));
    ଅନ୍ୟଥା
    {
      *la &= mask (a) - 1;
      la++;

      hb_memset (la, 0, (ଅକ୍ଷର *) lb - (ଅକ୍ଷର *) la);

      *lb &= ~((mask (b) << 1) - 1llu);
    }
  }
  ଶୂନ୍ୟ set_range (hb_codepoint_t a, hb_codepoint_t b, ତାର୍କିକ v)
  { ଯଦି (v) add_range (a, b); ଅନ୍ୟଥା del_range (a, b); }

  template <typename set_t>
  ଶୂନ୍ୟ union_ (ଅଚଳ set_t &set)
  {
    ପାଇଁ (hb_codepoint_t g : set)
      add (g);
  }

  ସ୍ଥିର ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ELT_BITS = ଆକାର (elt_t) * 8;
  ସ୍ଥିର constexpr unsigned ELT_MASK = ELT_BITS - 1;

  ସ୍ଥିର constexpr elt_t zero = 0;

  elt_t &elt (hb_codepoint_t g)
  {
    g -= min_v;
    ଯଦି (unlikely (g >= ଗଣନା_2))
      ଫେରନ୍ତୁ Crap(elt_t);
    ଫେରନ୍ତୁ elts[g / ELT_BITS];
  }
  ଅଚଳ elt_t& elt (hb_codepoint_t g) ଅଚଳ
  {
    g -= min_v;
    ଯଦି (unlikely (g >= ଗଣନା_2))
      ଫେରନ୍ତୁ Null(elt_t);
    ଫେରନ୍ତୁ elts[g / ELT_BITS];
  }

  ସ୍ଥିର constexpr int_t mask (hb_codepoint_t g) { ଫେରନ୍ତୁ elt_t (1) << (g & ELT_MASK); }

  hb_codepoint_t min_v = 0, max_v = 0, ଗଣନା_2 = 0;
  elt_t *elts = nullptr;
};


#endif /* HB_BIT_VECTOR_HH */

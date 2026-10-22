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

#அந்தர்பாவ "hb.hh"

#அந்தர்பாவ "hb-atomic.hh"

கட்டமைப்பு hb_min_max_t
{
  வெறுமை add (hb_codepoint_t v) { min_v = hb_min (min_v, v); max_v = hb_max (max_v, v); }
  வெறுமை add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    min_v = hb_min (min_v, a);
    max_v = hb_max (max_v, b);
  }

  template <typename set_t>
  வெறுமை union_ (மாறிலி set_t &set)
  {
    hb_codepoint_t set_min = set.get_min ();
    எனில் (unlikely (set_min == HB_CODEPOINT_INVALID))
      திரும்பு;
    hb_codepoint_t set_max = set.get_max ();
    min_v = hb_min (min_v, set_min);
    max_v = hb_max (max_v, set_max);
  }

  hb_codepoint_t get_min () மாறிலி { திரும்பு min_v; }
  hb_codepoint_t get_max () மாறிலி { திரும்பு max_v; }

  தனிப்பட்ட:
  hb_codepoint_t min_v = HB_CODEPOINT_INVALID;
  hb_codepoint_t max_v = 0;
};

template <தருக்க atomic = பொய்>
கட்டமைப்பு hb_bit_vector_t
{
  using int_t = uint64_t;
  using elt_t = typename std::conditional<atomic, hb_atomic_t<int_t>, int_t>::type;

  hb_bit_vector_t () = delete;
  hb_bit_vector_t (மாறிலி hb_bit_vector_t &other) = delete;
  hb_bit_vector_t &operator= (மாறிலி hb_bit_vector_t &other) = delete;

  // Move
  hb_bit_vector_t (hb_bit_vector_t &&other)
		: min_v (other.min_v), max_v (other.max_v), மொத்தம் (other.மொத்தம்), elts (other.elts)
  {
    other.min_v = other.max_v = other.மொத்தம் = 0;
    other.elts = nullptr;
  }
  hb_bit_vector_t &operator= (hb_bit_vector_t &&other)
  {
    hb_swap (min_v, other.min_v);
    hb_swap (max_v, other.max_v);
    hb_swap (மொத்தம், other.மொத்தம்);
    hb_swap (elts, other.elts);
    திரும்பு *இது;
  }

  hb_bit_vector_t (unsigned min_v, unsigned max_v)
    : min_v (min_v), max_v (max_v)
  {
    எனில் (unlikely (min_v >= max_v))
    {
      min_v = max_v = மொத்தம் = 0;
      திரும்பு;
    }

    unsigned இலக்கம் = (max_v - min_v + அளவு (int_t) * 8) / (அளவு (int_t) * 8);
    elts = (elt_t *) hb_calloc (இலக்கம், அளவு (int_t));
    எனில் (unlikely (!elts))
    {
      min_v = max_v = மொத்தம் = 0;
      திரும்பு;
    }

    மொத்தம் = max_v - min_v + 1;
  }
  ~hb_bit_vector_t ()
  {
    hb_free (elts);
  }

  வெறுமை add (hb_codepoint_t g) { elt (g) |= mask (g); }
  வெறுமை del (hb_codepoint_t g) { elt (g) &= ~mask (g); }
  வெறுமை set (hb_codepoint_t g, தருக்க value) { எனில் (value) add (g); இல்லையெனில் del (g); }
  தருக்க get (hb_codepoint_t g) மாறிலி { திரும்பு elt (g) & mask (g); }
  தருக்க has (hb_codepoint_t g) மாறிலி { திரும்பு get (g); }
  தருக்க may_have (hb_codepoint_t g) மாறிலி { திரும்பு get (g); }

  தருக்க operator [] (hb_codepoint_t g) மாறிலி { திரும்பு get (g); }
  தருக்க operator () (hb_codepoint_t g) மாறிலி { திரும்பு get (g); }

  வெறுமை add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    எனில் (unlikely (!மொத்தம் || a > b || a < min_v || b > max_v))
      திரும்பு;

    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    எனில் (la == lb)
      *la |= (mask (b) << 1) - mask(a);
    இல்லையெனில்
    {
      *la |= ~(mask (a) - 1llu);
      la++;

      hb_memset (la, 0xff, (எழுத்து *) lb - (எழுத்து *) la);

      *lb |= ((mask (b) << 1) - 1llu);
    }
  }
  வெறுமை del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    எனில் (unlikely (!மொத்தம் || a > b || a < min_v || b > max_v))
      திரும்பு;

    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    எனில் (la == lb)
      *la &= ~((mask (b) << 1llu) - mask(a));
    இல்லையெனில்
    {
      *la &= mask (a) - 1;
      la++;

      hb_memset (la, 0, (எழுத்து *) lb - (எழுத்து *) la);

      *lb &= ~((mask (b) << 1) - 1llu);
    }
  }
  வெறுமை set_range (hb_codepoint_t a, hb_codepoint_t b, தருக்க v)
  { எனில் (v) add_range (a, b); இல்லையெனில் del_range (a, b); }

  template <typename set_t>
  வெறுமை union_ (மாறிலி set_t &set)
  {
    க்கு (hb_codepoint_t g : set)
      add (g);
  }

  நிலையான மாறிலி unsigned முழு ELT_BITS = அளவு (elt_t) * 8;
  நிலையான constexpr unsigned ELT_MASK = ELT_BITS - 1;

  நிலையான constexpr elt_t zero = 0;

  elt_t &elt (hb_codepoint_t g)
  {
    g -= min_v;
    எனில் (unlikely (g >= மொத்தம்))
      திரும்பு Crap(elt_t);
    திரும்பு elts[g / ELT_BITS];
  }
  மாறிலி elt_t& elt (hb_codepoint_t g) மாறிலி
  {
    g -= min_v;
    எனில் (unlikely (g >= மொத்தம்))
      திரும்பு Null(elt_t);
    திரும்பு elts[g / ELT_BITS];
  }

  நிலையான constexpr int_t mask (hb_codepoint_t g) { திரும்பு elt_t (1) << (g & ELT_MASK); }

  hb_codepoint_t min_v = 0, max_v = 0, மொத்தம் = 0;
  elt_t *elts = nullptr;
};


#endif /* HB_BIT_VECTOR_HH */

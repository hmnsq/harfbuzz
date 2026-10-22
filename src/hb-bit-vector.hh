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

#شامل "hb.hh"

#شامل "hb-atomic.hh"

ساختار hb_min_max_t
{
  تهی add (hb_codepoint_t v) { min_v = hb_min (min_v, v); max_v = hb_max (max_v, v); }
  تهی add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    min_v = hb_min (min_v, a);
    max_v = hb_max (max_v, b);
  }

  template <typename set_t>
  تهی union_ (ثابت set_t &set)
  {
    hb_codepoint_t set_min = set.get_min ();
    اگر (unlikely (set_min == HB_CODEPOINT_INVALID))
      بازگشت;
    hb_codepoint_t set_max = set.get_max ();
    min_v = hb_min (min_v, set_min);
    max_v = hb_max (max_v, set_max);
  }

  hb_codepoint_t get_min () ثابت { بازگشت min_v; }
  hb_codepoint_t get_max () ثابت { بازگشت max_v; }

  خصوصی:
  hb_codepoint_t min_v = HB_CODEPOINT_INVALID;
  hb_codepoint_t max_v = 0;
};

template <منطقی atomic = نادرست>
ساختار hb_bit_vector_t
{
  using int_t = uint64_t;
  using elt_t = typename std::conditional<atomic, hb_atomic_t<int_t>, int_t>::type;

  hb_bit_vector_t () = delete;
  hb_bit_vector_t (ثابت hb_bit_vector_t &other) = delete;
  hb_bit_vector_t &operator= (ثابت hb_bit_vector_t &other) = delete;

  // Move
  hb_bit_vector_t (hb_bit_vector_t &&other)
		: min_v (other.min_v), max_v (other.max_v), شمار (other.شمار), elts (other.elts)
  {
    other.min_v = other.max_v = other.شمار = 0;
    other.elts = nullptr;
  }
  hb_bit_vector_t &operator= (hb_bit_vector_t &&other)
  {
    hb_swap (min_v, other.min_v);
    hb_swap (max_v, other.max_v);
    hb_swap (شمار, other.شمار);
    hb_swap (elts, other.elts);
    بازگشت *این;
  }

  hb_bit_vector_t (unsigned min_v, unsigned max_v)
    : min_v (min_v), max_v (max_v)
  {
    اگر (unlikely (min_v >= max_v))
    {
      min_v = max_v = شمار = 0;
      بازگشت;
    }

    unsigned رقم = (max_v - min_v + اندازه (int_t) * 8) / (اندازه (int_t) * 8);
    elts = (elt_t *) hb_calloc (رقم, اندازه (int_t));
    اگر (unlikely (!elts))
    {
      min_v = max_v = شمار = 0;
      بازگشت;
    }

    شمار = max_v - min_v + 1;
  }
  ~hb_bit_vector_t ()
  {
    hb_free (elts);
  }

  تهی add (hb_codepoint_t g) { elt (g) |= mask (g); }
  تهی del (hb_codepoint_t g) { elt (g) &= ~mask (g); }
  تهی set (hb_codepoint_t g, منطقی value) { اگر (value) add (g); وگرنه del (g); }
  منطقی get (hb_codepoint_t g) ثابت { بازگشت elt (g) & mask (g); }
  منطقی has (hb_codepoint_t g) ثابت { بازگشت get (g); }
  منطقی may_have (hb_codepoint_t g) ثابت { بازگشت get (g); }

  منطقی operator [] (hb_codepoint_t g) ثابت { بازگشت get (g); }
  منطقی operator () (hb_codepoint_t g) ثابت { بازگشت get (g); }

  تهی add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    اگر (unlikely (!شمار || a > b || a < min_v || b > max_v))
      بازگشت;

    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    اگر (la == lb)
      *la |= (mask (b) << 1) - mask(a);
    وگرنه
    {
      *la |= ~(mask (a) - 1llu);
      la++;

      hb_memset (la, 0xff, (نویسه *) lb - (نویسه *) la);

      *lb |= ((mask (b) << 1) - 1llu);
    }
  }
  تهی del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    اگر (unlikely (!شمار || a > b || a < min_v || b > max_v))
      بازگشت;

    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    اگر (la == lb)
      *la &= ~((mask (b) << 1llu) - mask(a));
    وگرنه
    {
      *la &= mask (a) - 1;
      la++;

      hb_memset (la, 0, (نویسه *) lb - (نویسه *) la);

      *lb &= ~((mask (b) << 1) - 1llu);
    }
  }
  تهی set_range (hb_codepoint_t a, hb_codepoint_t b, منطقی v)
  { اگر (v) add_range (a, b); وگرنه del_range (a, b); }

  template <typename set_t>
  تهی union_ (ثابت set_t &set)
  {
    برای (hb_codepoint_t g : set)
      add (g);
  }

  ایستا ثابت unsigned صحیح ELT_BITS = اندازه (elt_t) * 8;
  ایستا constexpr unsigned ELT_MASK = ELT_BITS - 1;

  ایستا constexpr elt_t zero = 0;

  elt_t &elt (hb_codepoint_t g)
  {
    g -= min_v;
    اگر (unlikely (g >= شمار))
      بازگشت Crap(elt_t);
    بازگشت elts[g / ELT_BITS];
  }
  ثابت elt_t& elt (hb_codepoint_t g) ثابت
  {
    g -= min_v;
    اگر (unlikely (g >= شمار))
      بازگشت Null(elt_t);
    بازگشت elts[g / ELT_BITS];
  }

  ایستا constexpr int_t mask (hb_codepoint_t g) { بازگشت elt_t (1) << (g & ELT_MASK); }

  hb_codepoint_t min_v = 0, max_v = 0, شمار = 0;
  elt_t *elts = nullptr;
};


#endif /* HB_BIT_VECTOR_HH */

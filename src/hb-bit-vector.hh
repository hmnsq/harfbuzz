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

#ضمن "hb.hh"

#ضمن "hb-atomic.hh"

بنية hb_min_max_t
{
  فراغ add (hb_codepoint_t v) { min_v = hb_min (min_v, v); max_v = hb_max (max_v, v); }
  فراغ add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    min_v = hb_min (min_v, a);
    max_v = hb_max (max_v, b);
  }

  template <typename set_t>
  فراغ union_ (ثابت set_t &set)
  {
    hb_codepoint_t set_min = set.get_min ();
    إذا (unlikely (set_min == HB_CODEPOINT_INVALID))
      أرجع;
    hb_codepoint_t set_max = set.get_max ();
    min_v = hb_min (min_v, set_min);
    max_v = hb_max (max_v, set_max);
  }

  hb_codepoint_t get_min () ثابت { أرجع min_v; }
  hb_codepoint_t get_max () ثابت { أرجع max_v; }

  خاص:
  hb_codepoint_t min_v = HB_CODEPOINT_INVALID;
  hb_codepoint_t max_v = 0;
};

template <منطقي atomic = خطأ>
بنية hb_bit_vector_t
{
  using int_t = uint64_t;
  using elt_t = typename std::conditional<atomic, hb_atomic_t<int_t>, int_t>::type;

  hb_bit_vector_t () = delete;
  hb_bit_vector_t (ثابت hb_bit_vector_t &other) = delete;
  hb_bit_vector_t &operator= (ثابت hb_bit_vector_t &other) = delete;

  // Move
  hb_bit_vector_t (hb_bit_vector_t &&other)
		: min_v (other.min_v), max_v (other.max_v), گنتی (other.گنتی), elts (other.elts)
  {
    other.min_v = other.max_v = other.گنتی = 0;
    other.elts = nullptr;
  }
  hb_bit_vector_t &operator= (hb_bit_vector_t &&other)
  {
    hb_swap (min_v, other.min_v);
    hb_swap (max_v, other.max_v);
    hb_swap (گنتی, other.گنتی);
    hb_swap (elts, other.elts);
    أرجع *هذا;
  }

  hb_bit_vector_t (unsigned min_v, unsigned max_v)
    : min_v (min_v), max_v (max_v)
  {
    إذا (unlikely (min_v >= max_v))
    {
      min_v = max_v = گنتی = 0;
      أرجع;
    }

    unsigned عدد = (max_v - min_v + حجم (int_t) * 8) / (حجم (int_t) * 8);
    elts = (elt_t *) hb_calloc (عدد, حجم (int_t));
    إذا (unlikely (!elts))
    {
      min_v = max_v = گنتی = 0;
      أرجع;
    }

    گنتی = max_v - min_v + 1;
  }
  ~hb_bit_vector_t ()
  {
    hb_free (elts);
  }

  فراغ add (hb_codepoint_t g) { elt (g) |= mask (g); }
  فراغ del (hb_codepoint_t g) { elt (g) &= ~mask (g); }
  فراغ set (hb_codepoint_t g, منطقي value) { إذا (value) add (g); وإلا del (g); }
  منطقي get (hb_codepoint_t g) ثابت { أرجع elt (g) & mask (g); }
  منطقي has (hb_codepoint_t g) ثابت { أرجع get (g); }
  منطقي may_have (hb_codepoint_t g) ثابت { أرجع get (g); }

  منطقي operator [] (hb_codepoint_t g) ثابت { أرجع get (g); }
  منطقي operator () (hb_codepoint_t g) ثابت { أرجع get (g); }

  فراغ add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    إذا (unlikely (!گنتی || a > b || a < min_v || b > max_v))
      أرجع;

    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    إذا (la == lb)
      *la |= (mask (b) << 1) - mask(a);
    وإلا
    {
      *la |= ~(mask (a) - 1llu);
      la++;

      hb_memset (la, 0xff, (حرف *) lb - (حرف *) la);

      *lb |= ((mask (b) << 1) - 1llu);
    }
  }
  فراغ del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    إذا (unlikely (!گنتی || a > b || a < min_v || b > max_v))
      أرجع;

    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    إذا (la == lb)
      *la &= ~((mask (b) << 1llu) - mask(a));
    وإلا
    {
      *la &= mask (a) - 1;
      la++;

      hb_memset (la, 0, (حرف *) lb - (حرف *) la);

      *lb &= ~((mask (b) << 1) - 1llu);
    }
  }
  فراغ set_range (hb_codepoint_t a, hb_codepoint_t b, منطقي v)
  { إذا (v) add_range (a, b); وإلا del_range (a, b); }

  template <typename set_t>
  فراغ union_ (ثابت set_t &set)
  {
    لكل (hb_codepoint_t g : set)
      add (g);
  }

  ساكن ثابت unsigned صحيح ELT_BITS = حجم (elt_t) * 8;
  ساكن constexpr unsigned ELT_MASK = ELT_BITS - 1;

  ساكن constexpr elt_t zero = 0;

  elt_t &elt (hb_codepoint_t g)
  {
    g -= min_v;
    إذا (unlikely (g >= گنتی))
      أرجع Crap(elt_t);
    أرجع elts[g / ELT_BITS];
  }
  ثابت elt_t& elt (hb_codepoint_t g) ثابت
  {
    g -= min_v;
    إذا (unlikely (g >= گنتی))
      أرجع Null(elt_t);
    أرجع elts[g / ELT_BITS];
  }

  ساكن constexpr int_t mask (hb_codepoint_t g) { أرجع elt_t (1) << (g & ELT_MASK); }

  hb_codepoint_t min_v = 0, max_v = 0, گنتی = 0;
  elt_t *elts = nullptr;
};


#endif /* HB_BIT_VECTOR_HH */

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

#अन्तर्भाव "hb.hh"

#अन्तर्भाव "hb-atomic.hh"

संरचना hb_min_max_t
{
  शून्य add (hb_codepoint_t v) { min_v = hb_min (min_v, v); max_v = hb_max (max_v, v); }
  शून्य add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    min_v = hb_min (min_v, a);
    max_v = hb_max (max_v, b);
  }

  ढाँचा <typename set_t>
  शून्य union_ (ध्रुव set_t &set)
  {
    hb_codepoint_t set_min = set.get_min ();
    यदि (unlikely (set_min == HB_CODEPOINT_INVALID))
      प्रत्यागम;
    hb_codepoint_t set_max = set.get_max ();
    min_v = hb_min (min_v, set_min);
    max_v = hb_max (max_v, set_max);
  }

  hb_codepoint_t get_min () ध्रुव { प्रत्यागम min_v; }
  hb_codepoint_t get_max () ध्रुव { प्रत्यागम max_v; }

  गुप्त:
  hb_codepoint_t min_v = HB_CODEPOINT_INVALID;
  hb_codepoint_t max_v = 0;
};

ढाँचा <तार्किक atomic = असत्य>
संरचना hb_bit_vector_t
{
  using int_t = uint64_t;
  using elt_t = typename std::conditional<atomic, hb_atomic_t<int_t>, int_t>::type;

  hb_bit_vector_t () = delete;
  hb_bit_vector_t (ध्रुव hb_bit_vector_t &other) = delete;
  hb_bit_vector_t &चालक= (ध्रुव hb_bit_vector_t &other) = delete;

  // Move
  hb_bit_vector_t (hb_bit_vector_t &&other)
		: min_v (other.min_v), max_v (other.max_v), गिनती (other.गिनती), elts (other.elts)
  {
    other.min_v = other.max_v = other.गिनती = 0;
    other.elts = nullptr;
  }
  hb_bit_vector_t &चालक= (hb_bit_vector_t &&other)
  {
    hb_swap (min_v, other.min_v);
    hb_swap (max_v, other.max_v);
    hb_swap (गिनती, other.गिनती);
    hb_swap (elts, other.elts);
    प्रत्यागम *एतत्;
  }

  hb_bit_vector_t (अचिन्हित min_v, अचिन्हित max_v)
    : min_v (min_v), max_v (max_v)
  {
    यदि (unlikely (min_v >= max_v))
    {
      min_v = max_v = गिनती = 0;
      प्रत्यागम;
    }

    अचिन्हित अंक = (max_v - min_v + परिमाण (int_t) * 8) / (परिमाण (int_t) * 8);
    elts = (elt_t *) hb_calloc (अंक, परिमाण (int_t));
    यदि (unlikely (!elts))
    {
      min_v = max_v = गिनती = 0;
      प्रत्यागम;
    }

    गिनती = max_v - min_v + 1;
  }
  ~hb_bit_vector_t ()
  {
    hb_free (elts);
  }

  शून्य add (hb_codepoint_t g) { elt (g) |= mask (g); }
  शून्य del (hb_codepoint_t g) { elt (g) &= ~mask (g); }
  शून्य set (hb_codepoint_t g, तार्किक value) { यदि (value) add (g); अन्यथा del (g); }
  तार्किक get (hb_codepoint_t g) ध्रुव { प्रत्यागम elt (g) & mask (g); }
  तार्किक has (hb_codepoint_t g) ध्रुव { प्रत्यागम get (g); }
  तार्किक may_have (hb_codepoint_t g) ध्रुव { प्रत्यागम get (g); }

  तार्किक चालक [] (hb_codepoint_t g) ध्रुव { प्रत्यागम get (g); }
  तार्किक चालक () (hb_codepoint_t g) ध्रुव { प्रत्यागम get (g); }

  शून्य add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    यदि (unlikely (!गिनती || a > b || a < min_v || b > max_v))
      प्रत्यागम;

    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    यदि (la == lb)
      *la |= (mask (b) << 1) - mask(a);
    अन्यथा
    {
      *la |= ~(mask (a) - 1llu);
      la++;

      hb_memset (la, 0xff, (वर्ण *) lb - (वर्ण *) la);

      *lb |= ((mask (b) << 1) - 1llu);
    }
  }
  शून्य del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    यदि (unlikely (!गिनती || a > b || a < min_v || b > max_v))
      प्रत्यागम;

    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    यदि (la == lb)
      *la &= ~((mask (b) << 1llu) - mask(a));
    अन्यथा
    {
      *la &= mask (a) - 1;
      la++;

      hb_memset (la, 0, (वर्ण *) lb - (वर्ण *) la);

      *lb &= ~((mask (b) << 1) - 1llu);
    }
  }
  शून्य set_range (hb_codepoint_t a, hb_codepoint_t b, तार्किक v)
  { यदि (v) add_range (a, b); अन्यथा del_range (a, b); }

  ढाँचा <typename set_t>
  शून्य union_ (ध्रुव set_t &set)
  {
    प्रति (hb_codepoint_t g : set)
      add (g);
  }

  स्थिर ध्रुव अचिन्हित पूर्णाङ्क ELT_BITS = परिमाण (elt_t) * 8;
  स्थिर constexpr अचिन्हित ELT_MASK = ELT_BITS - 1;

  स्थिर constexpr elt_t zero = 0;

  elt_t &elt (hb_codepoint_t g)
  {
    g -= min_v;
    यदि (unlikely (g >= गिनती))
      प्रत्यागम Crap(elt_t);
    प्रत्यागम elts[g / ELT_BITS];
  }
  ध्रुव elt_t& elt (hb_codepoint_t g) ध्रुव
  {
    g -= min_v;
    यदि (unlikely (g >= गिनती))
      प्रत्यागम Null(elt_t);
    प्रत्यागम elts[g / ELT_BITS];
  }

  स्थिर constexpr int_t mask (hb_codepoint_t g) { प्रत्यागम elt_t (1) << (g & ELT_MASK); }

  hb_codepoint_t min_v = 0, max_v = 0, गिनती = 0;
  elt_t *elts = nullptr;
};


#endif /* HB_BIT_VECTOR_HH */

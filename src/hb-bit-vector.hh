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

#अगर_अघोषित HB_BIT_VECTOR_HH
#घोषणा HB_BIT_VECTOR_HH

#समावेश "hb.hh"

#समावेश "hb-atomic.hh"

काष्ठा hb_min_max_t
{
  व्योम add (hb_codepoint_t v) { min_v = hb_min (min_v, v); max_v = hb_max (max_v, v); }
  व्योम add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    min_v = hb_min (min_v, a);
    max_v = hb_max (max_v, b);
  }

  ढाँचा <typename set_t>
  व्योम union_ (स्थिर set_t &set)
  {
    hb_codepoint_t set_min = set.get_min ();
    अगर (unlikely (set_min == HB_CODEPOINT_INVALID))
      वापस;
    hb_codepoint_t set_max = set.get_max ();
    min_v = hb_min (min_v, set_min);
    max_v = hb_max (max_v, set_max);
  }

  hb_codepoint_t get_min () स्थिर { वापस min_v; }
  hb_codepoint_t get_max () स्थिर { वापस max_v; }

  निजी:
  hb_codepoint_t min_v = HB_CODEPOINT_INVALID;
  hb_codepoint_t max_v = 0;
};

ढाँचा <bool atomic = false>
काष्ठा hb_bit_vector_t
{
  using int_t = uint64_t;
  using elt_t = typename std::conditional<atomic, hb_atomic_t<int_t>, int_t>::type;

  hb_bit_vector_t () = मिटाओ;
  hb_bit_vector_t (स्थिर hb_bit_vector_t &other) = मिटाओ;
  hb_bit_vector_t &चालक= (स्थिर hb_bit_vector_t &other) = मिटाओ;

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
    वापस *यह;
  }

  hb_bit_vector_t (अचिन्हित min_v, अचिन्हित max_v)
    : min_v (min_v), max_v (max_v)
  {
    अगर (unlikely (min_v >= max_v))
    {
      min_v = max_v = गिनती = 0;
      वापस;
    }

    अचिन्हित अंक = (max_v - min_v + माप (int_t) * 8) / (माप (int_t) * 8);
    elts = (elt_t *) hb_calloc (अंक, माप (int_t));
    अगर (unlikely (!elts))
    {
      min_v = max_v = गिनती = 0;
      वापस;
    }

    गिनती = max_v - min_v + 1;
  }
  ~hb_bit_vector_t ()
  {
    hb_free (elts);
  }

  व्योम add (hb_codepoint_t g) { elt (g) |= mask (g); }
  व्योम del (hb_codepoint_t g) { elt (g) &= ~mask (g); }
  व्योम set (hb_codepoint_t g, bool value) { अगर (value) add (g); अन्यथा del (g); }
  bool लो (hb_codepoint_t g) स्थिर { वापस elt (g) & mask (g); }
  bool has (hb_codepoint_t g) स्थिर { वापस लो (g); }
  bool may_have (hb_codepoint_t g) स्थिर { वापस लो (g); }

  bool चालक [] (hb_codepoint_t g) स्थिर { वापस लो (g); }
  bool चालक () (hb_codepoint_t g) स्थिर { वापस लो (g); }

  व्योम add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    अगर (unlikely (!गिनती || a > b || a < min_v || b > max_v))
      वापस;

    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    अगर (la == lb)
      *la |= (mask (b) << 1) - mask(a);
    अन्यथा
    {
      *la |= ~(mask (a) - 1llu);
      la++;

      hb_memset (la, 0xff, (अक्षर *) lb - (अक्षर *) la);

      *lb |= ((mask (b) << 1) - 1llu);
    }
  }
  व्योम del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    अगर (unlikely (!गिनती || a > b || a < min_v || b > max_v))
      वापस;

    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    अगर (la == lb)
      *la &= ~((mask (b) << 1llu) - mask(a));
    अन्यथा
    {
      *la &= mask (a) - 1;
      la++;

      hb_memset (la, 0, (अक्षर *) lb - (अक्षर *) la);

      *lb &= ~((mask (b) << 1) - 1llu);
    }
  }
  व्योम set_range (hb_codepoint_t a, hb_codepoint_t b, bool v)
  { अगर (v) add_range (a, b); अन्यथा del_range (a, b); }

  ढाँचा <typename set_t>
  व्योम union_ (स्थिर set_t &set)
  {
    क्रम (hb_codepoint_t g : set)
      add (g);
  }

  अटल स्थिर अचिन्हित पूर्णांक ELT_BITS = माप (elt_t) * 8;
  अटल constexpr अचिन्हित ELT_MASK = ELT_BITS - 1;

  अटल constexpr elt_t zero = 0;

  elt_t &elt (hb_codepoint_t g)
  {
    g -= min_v;
    अगर (unlikely (g >= गिनती))
      वापस Crap(elt_t);
    वापस elts[g / ELT_BITS];
  }
  स्थिर elt_t& elt (hb_codepoint_t g) स्थिर
  {
    g -= min_v;
    अगर (unlikely (g >= गिनती))
      वापस Null(elt_t);
    वापस elts[g / ELT_BITS];
  }

  अटल constexpr int_t mask (hb_codepoint_t g) { वापस elt_t (1) << (g & ELT_MASK); }

  hb_codepoint_t min_v = 0, max_v = 0, गिनती = 0;
  elt_t *elts = nullptr;
};


#पूर्ण_अगर /* HB_BIT_VECTOR_HH */

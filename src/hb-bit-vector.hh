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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸਦਿਸ਼_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸਦਿਸ਼_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-atomic.hh"

ਬਣਤਰ hb_min_max_t
{
  ਖਾਲੀ ਜੋੜੋ (hb_codepoint_t v) { min_v = hb_min (min_v, v); max_v = hb_max (max_v, v); }
  ਖਾਲੀ ਜੋੜੋ_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    min_v = hb_min (min_v, a);
    max_v = hb_max (max_v, b);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਖਾਲੀ union_ (ਅਚਲ ਸੈੱਟ_t &ਸੈੱਟ)
  {
    hb_codepoint_t ਸੈੱਟ_min = ਸੈੱਟ.get_min ();
    ਜੇ (unlikely (ਸੈੱਟ_min == HB_CODEPOINT_INVALID))
      ਵਾਪਸ;
    hb_codepoint_t ਸੈੱਟ_max = ਸੈੱਟ.get_max ();
    min_v = hb_min (min_v, ਸੈੱਟ_min);
    max_v = hb_max (max_v, ਸੈੱਟ_max);
  }

  hb_codepoint_t get_min () ਅਚਲ { ਵਾਪਸ min_v; }
  hb_codepoint_t get_max () ਅਚਲ { ਵਾਪਸ max_v; }

  ਨਿਜੀ:
  hb_codepoint_t min_v = HB_CODEPOINT_INVALID;
  hb_codepoint_t max_v = 0;
};

ਸਾਂਚਾ <ਤਾਰਕਿਕ atomic = ਝੂਠ>
ਬਣਤਰ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸਦਿਸ਼_t
{
  ਵਰਤੋਂ int_t = uint64_t;
  ਵਰਤੋਂ elt_t = ਕਿਸਮ_ਨਾਂ std::conditional<atomic, hb_atomic_t<int_t>, int_t>::type;

  hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸਦਿਸ਼_t () = ਮਿਟਾਓ;
  hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸਦਿਸ਼_t (ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸਦਿਸ਼_t &other) = ਮਿਟਾਓ;
  hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸਦਿਸ਼_t &ਸੰਚਾਲਕ= (ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸਦਿਸ਼_t &other) = ਮਿਟਾਓ;

  // Move
  hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸਦਿਸ਼_t (hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸਦਿਸ਼_t &&other)
		: min_v (other.min_v), max_v (other.max_v), ਗਿਣਤੀ_2 (other.ਗਿਣਤੀ_2), elts (other.elts)
  {
    other.min_v = other.max_v = other.ਗਿਣਤੀ_2 = 0;
    other.elts = ਖਾਲੀ_ਸੂਚਕ;
  }
  hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸਦਿਸ਼_t &ਸੰਚਾਲਕ= (hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸਦਿਸ਼_t &&other)
  {
    hb_ਅਦਲਾ_ਬਦਲੀ (min_v, other.min_v);
    hb_ਅਦਲਾ_ਬਦਲੀ (max_v, other.max_v);
    hb_ਅਦਲਾ_ਬਦਲੀ (ਗਿਣਤੀ_2, other.ਗਿਣਤੀ_2);
    hb_ਅਦਲਾ_ਬਦਲੀ (elts, other.elts);
    ਵਾਪਸ *ਇਹ;
  }

  hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸਦਿਸ਼_t (ਬਿਨਾਨਿਸ਼ਾਨੀ min_v, ਬਿਨਾਨਿਸ਼ਾਨੀ max_v)
    : min_v (min_v), max_v (max_v)
  {
    ਜੇ (unlikely (min_v >= max_v))
    {
      min_v = max_v = ਗਿਣਤੀ_2 = 0;
      ਵਾਪਸ;
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ = (max_v - min_v + ਆਕਾਰ (int_t) * 8) / (ਆਕਾਰ (int_t) * 8);
    elts = (elt_t *) hb_calloc (ਅੰਕ, ਆਕਾਰ (int_t));
    ਜੇ (unlikely (!elts))
    {
      min_v = max_v = ਗਿਣਤੀ_2 = 0;
      ਵਾਪਸ;
    }

    ਗਿਣਤੀ_2 = max_v - min_v + 1;
  }
  ~hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸਦਿਸ਼_t ()
  {
    hb_free (elts);
  }

  ਖਾਲੀ ਜੋੜੋ (hb_codepoint_t g) { elt (g) |= ਨਕਾਬ (g); }
  ਖਾਲੀ del (hb_codepoint_t g) { elt (g) &= ~ਨਕਾਬ (g); }
  ਖਾਲੀ ਸੈੱਟ (hb_codepoint_t g, ਤਾਰਕਿਕ value) { ਜੇ (value) ਜੋੜੋ (g); ਨਹੀਂਤਾਂ del (g); }
  ਤਾਰਕਿਕ get (hb_codepoint_t g) ਅਚਲ { ਵਾਪਸ elt (g) & ਨਕਾਬ (g); }
  ਤਾਰਕਿਕ has (hb_codepoint_t g) ਅਚਲ { ਵਾਪਸ get (g); }
  ਤਾਰਕਿਕ may_have (hb_codepoint_t g) ਅਚਲ { ਵਾਪਸ get (g); }

  ਤਾਰਕਿਕ ਸੰਚਾਲਕ [] (hb_codepoint_t g) ਅਚਲ { ਵਾਪਸ get (g); }
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ () (hb_codepoint_t g) ਅਚਲ { ਵਾਪਸ get (g); }

  ਖਾਲੀ ਜੋੜੋ_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    ਜੇ (unlikely (!ਗਿਣਤੀ_2 || a > b || a < min_v || b > max_v))
      ਵਾਪਸ;

    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    ਜੇ (la == lb)
      *la |= (ਨਕਾਬ (b) << 1) - ਨਕਾਬ(a);
    ਨਹੀਂਤਾਂ
    {
      *la |= ~(ਨਕਾਬ (a) - 1llu);
      la++;

      hb_memset (la, 0xff, (ਅੱਖਰ *) lb - (ਅੱਖਰ *) la);

      *lb |= ((ਨਕਾਬ (b) << 1) - 1llu);
    }
  }
  ਖਾਲੀ del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    ਜੇ (unlikely (!ਗਿਣਤੀ_2 || a > b || a < min_v || b > max_v))
      ਵਾਪਸ;

    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    ਜੇ (la == lb)
      *la &= ~((ਨਕਾਬ (b) << 1llu) - ਨਕਾਬ(a));
    ਨਹੀਂਤਾਂ
    {
      *la &= ਨਕਾਬ (a) - 1;
      la++;

      hb_memset (la, 0, (ਅੱਖਰ *) lb - (ਅੱਖਰ *) la);

      *lb &= ~((ਨਕਾਬ (b) << 1) - 1llu);
    }
  }
  ਖਾਲੀ ਸੈੱਟ_range (hb_codepoint_t a, hb_codepoint_t b, ਤਾਰਕਿਕ v)
  { ਜੇ (v) ਜੋੜੋ_range (a, b); ਨਹੀਂਤਾਂ del_range (a, b); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਖਾਲੀ union_ (ਅਚਲ ਸੈੱਟ_t &ਸੈੱਟ)
  {
    ਲਈ (hb_codepoint_t g : ਸੈੱਟ)
      ਜੋੜੋ (g);
  }

  ਸਥਿਰ ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ELT_ਬਿੱਟ = ਆਕਾਰ (elt_t) * 8;
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ ELT_ਨਕਾਬ = ELT_ਬਿੱਟ - 1;

  ਸਥਿਰ constexpr elt_t zero = 0;

  elt_t &elt (hb_codepoint_t g)
  {
    g -= min_v;
    ਜੇ (unlikely (g >= ਗਿਣਤੀ_2))
      ਵਾਪਸ Crap(elt_t);
    ਵਾਪਸ elts[g / ELT_ਬਿੱਟ];
  }
  ਅਚਲ elt_t& elt (hb_codepoint_t g) ਅਚਲ
  {
    g -= min_v;
    ਜੇ (unlikely (g >= ਗਿਣਤੀ_2))
      ਵਾਪਸ Null(elt_t);
    ਵਾਪਸ elts[g / ELT_ਬਿੱਟ];
  }

  ਸਥਿਰ constexpr int_t ਨਕਾਬ (hb_codepoint_t g) { ਵਾਪਸ elt_t (1) << (g & ELT_ਨਕਾਬ); }

  hb_codepoint_t min_v = 0, max_v = 0, ਗਿਣਤੀ_2 = 0;
  elt_t *elts = ਖਾਲੀ_ਸੂਚਕ;
};


#ਜੇ_ਅੰਤ /* HB_BIT_VECTOR_HH */

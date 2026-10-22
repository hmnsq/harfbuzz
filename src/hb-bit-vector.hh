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

#جے_بنا_تعریف HB_ثنائی_ہندسہ_سمتیہ_HH
#تعریف_کرو HB_ثنائی_ہندسہ_سمتیہ_HH

#ضمن "hb.hh"

#ضمن "hb-atomic.hh"

بنية hb_min_max_t
{
  فراغ جوڑو (hb_codepoint_t v) { min_v = hb_min (min_v, v); max_v = hb_max (max_v, v); }
  فراغ جوڑو_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    min_v = hb_min (min_v, a);
    max_v = hb_max (max_v, b);
  }

  سانچا <قسم_ناں سیٹ_t>
  فراغ union_ (ثابت سیٹ_t &سیٹ)
  {
    hb_codepoint_t سیٹ_min = سیٹ.get_min ();
    إذا (unlikely (سیٹ_min == HB_CODEPOINT_INVALID))
      أرجع;
    hb_codepoint_t سیٹ_max = سیٹ.get_max ();
    min_v = hb_min (min_v, سیٹ_min);
    max_v = hb_max (max_v, سیٹ_max);
  }

  hb_codepoint_t get_min () ثابت { أرجع min_v; }
  hb_codepoint_t get_max () ثابت { أرجع max_v; }

  خاص:
  hb_codepoint_t min_v = HB_CODEPOINT_INVALID;
  hb_codepoint_t max_v = 0;
};

سانچا <منطقي atomic = خطأ>
بنية hb_ثنائی_ہندسہ_سمتیہ_t
{
  ورتوں int_t = uint64_t;
  ورتوں elt_t = قسم_ناں std::conditional<atomic, hb_atomic_t<int_t>, int_t>::type;

  hb_ثنائی_ہندسہ_سمتیہ_t () = مٹاؤ;
  hb_ثنائی_ہندسہ_سمتیہ_t (ثابت hb_ثنائی_ہندسہ_سمتیہ_t &other) = مٹاؤ;
  hb_ثنائی_ہندسہ_سمتیہ_t &عامل= (ثابت hb_ثنائی_ہندسہ_سمتیہ_t &other) = مٹاؤ;

  // Move
  hb_ثنائی_ہندسہ_سمتیہ_t (hb_ثنائی_ہندسہ_سمتیہ_t &&other)
		: min_v (other.min_v), max_v (other.max_v), گنتی (other.گنتی), elts (other.elts)
  {
    other.min_v = other.max_v = other.گنتی = 0;
    other.elts = خالی_سوچک;
  }
  hb_ثنائی_ہندسہ_سمتیہ_t &عامل= (hb_ثنائی_ہندسہ_سمتیہ_t &&other)
  {
    hb_وٹاندرا (min_v, other.min_v);
    hb_وٹاندرا (max_v, other.max_v);
    hb_وٹاندرا (گنتی, other.گنتی);
    hb_وٹاندرا (elts, other.elts);
    أرجع *هذا;
  }

  hb_ثنائی_ہندسہ_سمتیہ_t (بنانشانی min_v, بنانشانی max_v)
    : min_v (min_v), max_v (max_v)
  {
    إذا (unlikely (min_v >= max_v))
    {
      min_v = max_v = گنتی = 0;
      أرجع;
    }

    بنانشانی عدد = (max_v - min_v + حجم (int_t) * 8) / (حجم (int_t) * 8);
    elts = (elt_t *) hb_calloc (عدد, حجم (int_t));
    إذا (unlikely (!elts))
    {
      min_v = max_v = گنتی = 0;
      أرجع;
    }

    گنتی = max_v - min_v + 1;
  }
  ~hb_ثنائی_ہندسہ_سمتیہ_t ()
  {
    hb_free (elts);
  }

  فراغ جوڑو (hb_codepoint_t g) { elt (g) |= نقاب (g); }
  فراغ del (hb_codepoint_t g) { elt (g) &= ~نقاب (g); }
  فراغ سیٹ (hb_codepoint_t g, منطقي value) { إذا (value) جوڑو (g); وإلا del (g); }
  منطقي get (hb_codepoint_t g) ثابت { أرجع elt (g) & نقاب (g); }
  منطقي has (hb_codepoint_t g) ثابت { أرجع get (g); }
  منطقي may_have (hb_codepoint_t g) ثابت { أرجع get (g); }

  منطقي عامل [] (hb_codepoint_t g) ثابت { أرجع get (g); }
  منطقي عامل () (hb_codepoint_t g) ثابت { أرجع get (g); }

  فراغ جوڑو_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    إذا (unlikely (!گنتی || a > b || a < min_v || b > max_v))
      أرجع;

    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    إذا (la == lb)
      *la |= (نقاب (b) << 1) - نقاب(a);
    وإلا
    {
      *la |= ~(نقاب (a) - 1llu);
      la++;

      hb_memset (la, 0xff, (حرف *) lb - (حرف *) la);

      *lb |= ((نقاب (b) << 1) - 1llu);
    }
  }
  فراغ del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    إذا (unlikely (!گنتی || a > b || a < min_v || b > max_v))
      أرجع;

    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    إذا (la == lb)
      *la &= ~((نقاب (b) << 1llu) - نقاب(a));
    وإلا
    {
      *la &= نقاب (a) - 1;
      la++;

      hb_memset (la, 0, (حرف *) lb - (حرف *) la);

      *lb &= ~((نقاب (b) << 1) - 1llu);
    }
  }
  فراغ سیٹ_range (hb_codepoint_t a, hb_codepoint_t b, منطقي v)
  { إذا (v) جوڑو_range (a, b); وإلا del_range (a, b); }

  سانچا <قسم_ناں سیٹ_t>
  فراغ union_ (ثابت سیٹ_t &سیٹ)
  {
    لكل (hb_codepoint_t g : سیٹ)
      جوڑو (g);
  }

  ساكن ثابت بنانشانی صحيح ELT_بٹ = حجم (elt_t) * 8;
  ساكن constexpr بنانشانی ELT_نقاب = ELT_بٹ - 1;

  ساكن constexpr elt_t zero = 0;

  elt_t &elt (hb_codepoint_t g)
  {
    g -= min_v;
    إذا (unlikely (g >= گنتی))
      أرجع Crap(elt_t);
    أرجع elts[g / ELT_بٹ];
  }
  ثابت elt_t& elt (hb_codepoint_t g) ثابت
  {
    g -= min_v;
    إذا (unlikely (g >= گنتی))
      أرجع Null(elt_t);
    أرجع elts[g / ELT_بٹ];
  }

  ساكن constexpr int_t نقاب (hb_codepoint_t g) { أرجع elt_t (1) << (g & ELT_نقاب); }

  hb_codepoint_t min_v = 0, max_v = 0, گنتی = 0;
  elt_t *elts = خالی_سوچک;
};


#جے_انت /* HB_BIT_VECTOR_HH */

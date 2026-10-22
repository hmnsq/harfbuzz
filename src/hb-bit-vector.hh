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

#اگر_غیر_تعریف_شدہ HB_ثنائی_ہندسہ_سمتیہ_HH
#تعریف_کرو HB_ثنائی_ہندسہ_سمتیہ_HH

#شامل "hb.hh"

#شامل "hb-atomic.hh"

ساخت hb_min_max_t
{
  خالی جوڑو (hb_codepoint_t v) { min_v = hb_min (min_v, v); max_v = hb_max (max_v, v); }
  خالی جوڑو_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    min_v = hb_min (min_v, a);
    max_v = hb_max (max_v, b);
  }

  سانچہ <قسم_نام سیٹ_t>
  خالی union_ (ثابت سیٹ_t &سیٹ)
  {
    hb_codepoint_t سیٹ_min = سیٹ.get_min ();
    اگر (unlikely (سیٹ_min == HB_CODEPOINT_INVALID))
      واپس;
    hb_codepoint_t سیٹ_max = سیٹ.get_max ();
    min_v = hb_min (min_v, سیٹ_min);
    max_v = hb_max (max_v, سیٹ_max);
  }

  hb_codepoint_t get_min () ثابت { واپس min_v; }
  hb_codepoint_t get_max () ثابت { واپس max_v; }

  نجی:
  hb_codepoint_t min_v = HB_CODEPOINT_INVALID;
  hb_codepoint_t max_v = 0;
};

سانچہ <منطقی atomic = جھوٹ>
ساخت hb_ثنائی_ہندسہ_سمتیہ_t
{
  استعمال int_t = uint64_t;
  استعمال elt_t = قسم_نام std::conditional<atomic, hb_atomic_t<int_t>, int_t>::type;

  hb_ثنائی_ہندسہ_سمتیہ_t () = حذف;
  hb_ثنائی_ہندسہ_سمتیہ_t (ثابت hb_ثنائی_ہندسہ_سمتیہ_t &other) = حذف;
  hb_ثنائی_ہندسہ_سمتیہ_t &عامل= (ثابت hb_ثنائی_ہندسہ_سمتیہ_t &other) = حذف;

  // Move
  hb_ثنائی_ہندسہ_سمتیہ_t (hb_ثنائی_ہندسہ_سمتیہ_t &&other)
		: min_v (other.min_v), max_v (other.max_v), گنتی (other.گنتی), elts (other.elts)
  {
    other.min_v = other.max_v = other.گنتی = 0;
    other.elts = خالی_اشاریہ;
  }
  hb_ثنائی_ہندسہ_سمتیہ_t &عامل= (hb_ثنائی_ہندسہ_سمتیہ_t &&other)
  {
    hb_تبادلہ (min_v, other.min_v);
    hb_تبادلہ (max_v, other.max_v);
    hb_تبادلہ (گنتی, other.گنتی);
    hb_تبادلہ (elts, other.elts);
    واپس *یہ;
  }

  hb_ثنائی_ہندسہ_سمتیہ_t (غیرعلامتی min_v, غیرعلامتی max_v)
    : min_v (min_v), max_v (max_v)
  {
    اگر (unlikely (min_v >= max_v))
    {
      min_v = max_v = گنتی = 0;
      واپس;
    }

    غیرعلامتی عدد = (max_v - min_v + حجم (int_t) * 8) / (حجم (int_t) * 8);
    elts = (elt_t *) hb_calloc (عدد, حجم (int_t));
    اگر (unlikely (!elts))
    {
      min_v = max_v = گنتی = 0;
      واپس;
    }

    گنتی = max_v - min_v + 1;
  }
  ~hb_ثنائی_ہندسہ_سمتیہ_t ()
  {
    hb_free (elts);
  }

  خالی جوڑو (hb_codepoint_t g) { elt (g) |= نقاب (g); }
  خالی del (hb_codepoint_t g) { elt (g) &= ~نقاب (g); }
  خالی سیٹ (hb_codepoint_t g, منطقی value) { اگر (value) جوڑو (g); ورنہ del (g); }
  منطقی get (hb_codepoint_t g) ثابت { واپس elt (g) & نقاب (g); }
  منطقی has (hb_codepoint_t g) ثابت { واپس get (g); }
  منطقی may_have (hb_codepoint_t g) ثابت { واپس get (g); }

  منطقی عامل [] (hb_codepoint_t g) ثابت { واپس get (g); }
  منطقی عامل () (hb_codepoint_t g) ثابت { واپس get (g); }

  خالی جوڑو_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    اگر (unlikely (!گنتی || a > b || a < min_v || b > max_v))
      واپس;

    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    اگر (la == lb)
      *la |= (نقاب (b) << 1) - نقاب(a);
    ورنہ
    {
      *la |= ~(نقاب (a) - 1llu);
      la++;

      hb_memset (la, 0xff, (حرف *) lb - (حرف *) la);

      *lb |= ((نقاب (b) << 1) - 1llu);
    }
  }
  خالی del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    اگر (unlikely (!گنتی || a > b || a < min_v || b > max_v))
      واپس;

    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    اگر (la == lb)
      *la &= ~((نقاب (b) << 1llu) - نقاب(a));
    ورنہ
    {
      *la &= نقاب (a) - 1;
      la++;

      hb_memset (la, 0, (حرف *) lb - (حرف *) la);

      *lb &= ~((نقاب (b) << 1) - 1llu);
    }
  }
  خالی سیٹ_range (hb_codepoint_t a, hb_codepoint_t b, منطقی v)
  { اگر (v) جوڑو_range (a, b); ورنہ del_range (a, b); }

  سانچہ <قسم_نام سیٹ_t>
  خالی union_ (ثابت سیٹ_t &سیٹ)
  {
    برائے (hb_codepoint_t g : سیٹ)
      جوڑو (g);
  }

  ساکن ثابت غیرعلامتی صحیح ELT_بٹ = حجم (elt_t) * 8;
  ساکن constexpr غیرعلامتی ELT_نقاب = ELT_بٹ - 1;

  ساکن constexpr elt_t zero = 0;

  elt_t &elt (hb_codepoint_t g)
  {
    g -= min_v;
    اگر (unlikely (g >= گنتی))
      واپس Crap(elt_t);
    واپس elts[g / ELT_بٹ];
  }
  ثابت elt_t& elt (hb_codepoint_t g) ثابت
  {
    g -= min_v;
    اگر (unlikely (g >= گنتی))
      واپس Null(elt_t);
    واپس elts[g / ELT_بٹ];
  }

  ساکن constexpr int_t نقاب (hb_codepoint_t g) { واپس elt_t (1) << (g & ELT_نقاب); }

  hb_codepoint_t min_v = 0, max_v = 0, گنتی = 0;
  elt_t *elts = خالی_اشاریہ;
};


#اگر_ختم /* HB_BIT_VECTOR_HH */

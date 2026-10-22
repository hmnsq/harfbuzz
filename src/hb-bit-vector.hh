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

#اگر_تعریف_نشده HB_بیت_وکتور_HH
#تعریف_کن HB_بیت_وکتور_HH

#ضمن "hb.hh"

#ضمن "hb-atomic.hh"

بنية hb_min_max_t
{
  فراغ جمع_کن (hb_codepoint_t v) { min_v = hb_min (min_v, v); max_v = hb_max (max_v, v); }
  فراغ جمع_کن_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    min_v = hb_min (min_v, a);
    max_v = hb_max (max_v, b);
  }

  قالب <نام_نوع مجموعه_t>
  فراغ union_ (ثابت مجموعه_t &مجموعه)
  {
    hb_codepoint_t مجموعه_min = مجموعه.get_min ();
    إذا (unlikely (مجموعه_min == HB_CODEPOINT_INVALID))
      أرجع;
    hb_codepoint_t مجموعه_max = مجموعه.get_max ();
    min_v = hb_min (min_v, مجموعه_min);
    max_v = hb_max (max_v, مجموعه_max);
  }

  hb_codepoint_t get_min () ثابت { أرجع min_v; }
  hb_codepoint_t get_max () ثابت { أرجع max_v; }

  خاص:
  hb_codepoint_t min_v = HB_CODEPOINT_INVALID;
  hb_codepoint_t max_v = 0;
};

قالب <منطقي atomic = خطأ>
بنية hb_بیت_وکتور_t
{
  استفاده int_t = uint64_t;
  استفاده elt_t = نام_نوع std::conditional<atomic, hb_atomic_t<int_t>, int_t>::type;

  hb_بیت_وکتور_t () = حذف;
  hb_بیت_وکتور_t (ثابت hb_بیت_وکتور_t &other) = حذف;
  hb_بیت_وکتور_t &عملگر= (ثابت hb_بیت_وکتور_t &other) = حذف;

  // Move
  hb_بیت_وکتور_t (hb_بیت_وکتور_t &&other)
		: min_v (other.min_v), max_v (other.max_v), شمار (other.شمار), elts (other.elts)
  {
    other.min_v = other.max_v = other.شمار = 0;
    other.elts = اشاره_تهی;
  }
  hb_بیت_وکتور_t &عملگر= (hb_بیت_وکتور_t &&other)
  {
    hb_جابجا (min_v, other.min_v);
    hb_جابجا (max_v, other.max_v);
    hb_جابجا (شمار, other.شمار);
    hb_جابجا (elts, other.elts);
    أرجع *هذا;
  }

  hb_بیت_وکتور_t (بیعلامت min_v, بیعلامت max_v)
    : min_v (min_v), max_v (max_v)
  {
    إذا (unlikely (min_v >= max_v))
    {
      min_v = max_v = شمار = 0;
      أرجع;
    }

    بیعلامت رقم = (max_v - min_v + حجم (int_t) * 8) / (حجم (int_t) * 8);
    elts = (elt_t *) hb_calloc (رقم, حجم (int_t));
    إذا (unlikely (!elts))
    {
      min_v = max_v = شمار = 0;
      أرجع;
    }

    شمار = max_v - min_v + 1;
  }
  ~hb_بیت_وکتور_t ()
  {
    hb_free (elts);
  }

  فراغ جمع_کن (hb_codepoint_t g) { elt (g) |= ماسک (g); }
  فراغ del (hb_codepoint_t g) { elt (g) &= ~ماسک (g); }
  فراغ مجموعه (hb_codepoint_t g, منطقي value) { إذا (value) جمع_کن (g); وإلا del (g); }
  منطقي get (hb_codepoint_t g) ثابت { أرجع elt (g) & ماسک (g); }
  منطقي has (hb_codepoint_t g) ثابت { أرجع get (g); }
  منطقي may_have (hb_codepoint_t g) ثابت { أرجع get (g); }

  منطقي عملگر [] (hb_codepoint_t g) ثابت { أرجع get (g); }
  منطقي عملگر () (hb_codepoint_t g) ثابت { أرجع get (g); }

  فراغ جمع_کن_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    إذا (unlikely (!شمار || a > b || a < min_v || b > max_v))
      أرجع;

    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    إذا (la == lb)
      *la |= (ماسک (b) << 1) - ماسک(a);
    وإلا
    {
      *la |= ~(ماسک (a) - 1llu);
      la++;

      hb_memset (la, 0xff, (حرف *) lb - (حرف *) la);

      *lb |= ((ماسک (b) << 1) - 1llu);
    }
  }
  فراغ del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    إذا (unlikely (!شمار || a > b || a < min_v || b > max_v))
      أرجع;

    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    إذا (la == lb)
      *la &= ~((ماسک (b) << 1llu) - ماسک(a));
    وإلا
    {
      *la &= ماسک (a) - 1;
      la++;

      hb_memset (la, 0, (حرف *) lb - (حرف *) la);

      *lb &= ~((ماسک (b) << 1) - 1llu);
    }
  }
  فراغ مجموعه_range (hb_codepoint_t a, hb_codepoint_t b, منطقي v)
  { إذا (v) جمع_کن_range (a, b); وإلا del_range (a, b); }

  قالب <نام_نوع مجموعه_t>
  فراغ union_ (ثابت مجموعه_t &مجموعه)
  {
    لكل (hb_codepoint_t g : مجموعه)
      جمع_کن (g);
  }

  ساكن ثابت بیعلامت صحيح ELT_بیتها = حجم (elt_t) * 8;
  ساكن constexpr بیعلامت ELT_ماسک = ELT_بیتها - 1;

  ساكن constexpr elt_t zero = 0;

  elt_t &elt (hb_codepoint_t g)
  {
    g -= min_v;
    إذا (unlikely (g >= شمار))
      أرجع Crap(elt_t);
    أرجع elts[g / ELT_بیتها];
  }
  ثابت elt_t& elt (hb_codepoint_t g) ثابت
  {
    g -= min_v;
    إذا (unlikely (g >= شمار))
      أرجع Null(elt_t);
    أرجع elts[g / ELT_بیتها];
  }

  ساكن constexpr int_t ماسک (hb_codepoint_t g) { أرجع elt_t (1) << (g & ELT_ماسک); }

  hb_codepoint_t min_v = 0, max_v = 0, شمار = 0;
  elt_t *elts = اشاره_تهی;
};


#پایان_اگر /* HB_BIT_VECTOR_HH */

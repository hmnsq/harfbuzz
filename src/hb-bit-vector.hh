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

#که_نه_تعریف_شوی HB_بټ_ویکټور_HH
#تعریف_کړه HB_بټ_ویکټور_HH

#ضمن "hb.hh"

#ضمن "hb-atomic.hh"

بنية hb_min_max_t
{
  فراغ جمع_کړه (hb_codepoint_t v) { min_v = hb_min (min_v, v); max_v = hb_max (max_v, v); }
  فراغ جمع_کړه_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    min_v = hb_min (min_v, a);
    max_v = hb_max (max_v, b);
  }

  کينډۍ <ډول_نوم سېټ_t>
  فراغ union_ (ثابت سېټ_t &سېټ)
  {
    hb_codepoint_t سېټ_min = سېټ.get_min ();
    إذا (unlikely (سېټ_min == HB_CODEPOINT_INVALID))
      أرجع;
    hb_codepoint_t سېټ_max = سېټ.get_max ();
    min_v = hb_min (min_v, سېټ_min);
    max_v = hb_max (max_v, سېټ_max);
  }

  hb_codepoint_t get_min () ثابت { أرجع min_v; }
  hb_codepoint_t get_max () ثابت { أرجع max_v; }

  خاص:
  hb_codepoint_t min_v = HB_CODEPOINT_INVALID;
  hb_codepoint_t max_v = 0;
};

کينډۍ <منطقي atomic = خطأ>
بنية hb_بټ_ویکټور_t
{
  کارول int_t = uint64_t;
  کارول elt_t = ډول_نوم std::conditional<atomic, hb_atomic_t<int_t>, int_t>::type;

  hb_بټ_ویکټور_t () = ړنګول;
  hb_بټ_ویکټور_t (ثابت hb_بټ_ویکټور_t &other) = ړنګول;
  hb_بټ_ویکټور_t &چلوونکی= (ثابت hb_بټ_ویکټور_t &other) = ړنګول;

  // Move
  hb_بټ_ویکټور_t (hb_بټ_ویکټور_t &&other)
		: min_v (other.min_v), max_v (other.max_v), شمېر (other.شمېر), elts (other.elts)
  {
    other.min_v = other.max_v = other.شمېر = 0;
    other.elts = تش_اشاره;
  }
  hb_بټ_ویکټور_t &چلوونکی= (hb_بټ_ویکټور_t &&other)
  {
    hb_بدلون (min_v, other.min_v);
    hb_بدلون (max_v, other.max_v);
    hb_بدلون (شمېر, other.شمېر);
    hb_بدلون (elts, other.elts);
    أرجع *هذا;
  }

  hb_بټ_ویکټور_t (بینښه min_v, بینښه max_v)
    : min_v (min_v), max_v (max_v)
  {
    إذا (unlikely (min_v >= max_v))
    {
      min_v = max_v = شمېر = 0;
      أرجع;
    }

    بینښه عدد = (max_v - min_v + حجم (int_t) * 8) / (حجم (int_t) * 8);
    elts = (elt_t *) hb_calloc (عدد, حجم (int_t));
    إذا (unlikely (!elts))
    {
      min_v = max_v = شمېر = 0;
      أرجع;
    }

    شمېر = max_v - min_v + 1;
  }
  ~hb_بټ_ویکټور_t ()
  {
    hb_free (elts);
  }

  فراغ جمع_کړه (hb_codepoint_t g) { elt (g) |= ماسک (g); }
  فراغ del (hb_codepoint_t g) { elt (g) &= ~ماسک (g); }
  فراغ سېټ (hb_codepoint_t g, منطقي value) { إذا (value) جمع_کړه (g); وإلا del (g); }
  منطقي get (hb_codepoint_t g) ثابت { أرجع elt (g) & ماسک (g); }
  منطقي has (hb_codepoint_t g) ثابت { أرجع get (g); }
  منطقي may_have (hb_codepoint_t g) ثابت { أرجع get (g); }

  منطقي چلوونکی [] (hb_codepoint_t g) ثابت { أرجع get (g); }
  منطقي چلوونکی () (hb_codepoint_t g) ثابت { أرجع get (g); }

  فراغ جمع_کړه_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    إذا (unlikely (!شمېر || a > b || a < min_v || b > max_v))
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
    إذا (unlikely (!شمېر || a > b || a < min_v || b > max_v))
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
  فراغ سېټ_range (hb_codepoint_t a, hb_codepoint_t b, منطقي v)
  { إذا (v) جمع_کړه_range (a, b); وإلا del_range (a, b); }

  کينډۍ <ډول_نوم سېټ_t>
  فراغ union_ (ثابت سېټ_t &سېټ)
  {
    لكل (hb_codepoint_t g : سېټ)
      جمع_کړه (g);
  }

  ساكن ثابت بینښه صحيح ELT_بټونه = حجم (elt_t) * 8;
  ساكن constexpr بینښه ELT_ماسک = ELT_بټونه - 1;

  ساكن constexpr elt_t zero = 0;

  elt_t &elt (hb_codepoint_t g)
  {
    g -= min_v;
    إذا (unlikely (g >= شمېر))
      أرجع Crap(elt_t);
    أرجع elts[g / ELT_بټونه];
  }
  ثابت elt_t& elt (hb_codepoint_t g) ثابت
  {
    g -= min_v;
    إذا (unlikely (g >= شمېر))
      أرجع Null(elt_t);
    أرجع elts[g / ELT_بټونه];
  }

  ساكن constexpr int_t ماسک (hb_codepoint_t g) { أرجع elt_t (1) << (g & ELT_ماسک); }

  hb_codepoint_t min_v = 0, max_v = 0, شمېر = 0;
  elt_t *elts = تش_اشاره;
};


#د_که_پای /* HB_BIT_VECTOR_HH */

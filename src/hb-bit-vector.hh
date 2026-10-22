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

#جيڪڏهن_وصف_نه_ٿيل HB_ٻٽو_انگ_ويڪٽر_HH
#وصف_ڪريو HB_ٻٽو_انگ_ويڪٽر_HH

#ضمن "hb.hh"

#ضمن "hb-atomic.hh"

بنية hb_min_max_t
{
  فراغ جوڙيو (hb_codepoint_t v) { min_v = hb_min (min_v, v); max_v = hb_max (max_v, v); }
  فراغ جوڙيو_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    min_v = hb_min (min_v, a);
    max_v = hb_max (max_v, b);
  }

  سانچو <قسم_نالو سيٽ_t>
  فراغ union_ (ثابت سيٽ_t &سيٽ)
  {
    hb_codepoint_t سيٽ_min = سيٽ.get_min ();
    إذا (unlikely (سيٽ_min == HB_CODEPOINT_INVALID))
      أرجع;
    hb_codepoint_t سيٽ_max = سيٽ.get_max ();
    min_v = hb_min (min_v, سيٽ_min);
    max_v = hb_max (max_v, سيٽ_max);
  }

  hb_codepoint_t get_min () ثابت { أرجع min_v; }
  hb_codepoint_t get_max () ثابت { أرجع max_v; }

  خاص:
  hb_codepoint_t min_v = HB_CODEPOINT_INVALID;
  hb_codepoint_t max_v = 0;
};

سانچو <منطقي atomic = خطأ>
بنية hb_ٻٽو_انگ_ويڪٽر_t
{
  استعمال int_t = uint64_t;
  استعمال elt_t = قسم_نالو std::conditional<atomic, hb_atomic_t<int_t>, int_t>::type;

  hb_ٻٽو_انگ_ويڪٽر_t () = ميٽيو;
  hb_ٻٽو_انگ_ويڪٽر_t (ثابت hb_ٻٽو_انگ_ويڪٽر_t &other) = ميٽيو;
  hb_ٻٽو_انگ_ويڪٽر_t &عامل= (ثابت hb_ٻٽو_انگ_ويڪٽر_t &other) = ميٽيو;

  // Move
  hb_ٻٽو_انگ_ويڪٽر_t (hb_ٻٽو_انگ_ويڪٽر_t &&other)
		: min_v (other.min_v), max_v (other.max_v), ڳڻپ (other.ڳڻپ), elts (other.elts)
  {
    other.min_v = other.max_v = other.ڳڻپ = 0;
    other.elts = خالي_اشارو;
  }
  hb_ٻٽو_انگ_ويڪٽر_t &عامل= (hb_ٻٽو_انگ_ويڪٽر_t &&other)
  {
    hb_مٽا_سٽا (min_v, other.min_v);
    hb_مٽا_سٽا (max_v, other.max_v);
    hb_مٽا_سٽا (ڳڻپ, other.ڳڻپ);
    hb_مٽا_سٽا (elts, other.elts);
    أرجع *هذا;
  }

  hb_ٻٽو_انگ_ويڪٽر_t (بنانشان min_v, بنانشان max_v)
    : min_v (min_v), max_v (max_v)
  {
    إذا (unlikely (min_v >= max_v))
    {
      min_v = max_v = ڳڻپ = 0;
      أرجع;
    }

    بنانشان انگ = (max_v - min_v + حجم (int_t) * 8) / (حجم (int_t) * 8);
    elts = (elt_t *) hb_calloc (انگ, حجم (int_t));
    إذا (unlikely (!elts))
    {
      min_v = max_v = ڳڻپ = 0;
      أرجع;
    }

    ڳڻپ = max_v - min_v + 1;
  }
  ~hb_ٻٽو_انگ_ويڪٽر_t ()
  {
    hb_free (elts);
  }

  فراغ جوڙيو (hb_codepoint_t g) { elt (g) |= نقاب (g); }
  فراغ del (hb_codepoint_t g) { elt (g) &= ~نقاب (g); }
  فراغ سيٽ (hb_codepoint_t g, منطقي value) { إذا (value) جوڙيو (g); وإلا del (g); }
  منطقي get (hb_codepoint_t g) ثابت { أرجع elt (g) & نقاب (g); }
  منطقي has (hb_codepoint_t g) ثابت { أرجع get (g); }
  منطقي may_have (hb_codepoint_t g) ثابت { أرجع get (g); }

  منطقي عامل [] (hb_codepoint_t g) ثابت { أرجع get (g); }
  منطقي عامل () (hb_codepoint_t g) ثابت { أرجع get (g); }

  فراغ جوڙيو_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    إذا (unlikely (!ڳڻپ || a > b || a < min_v || b > max_v))
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
    إذا (unlikely (!ڳڻپ || a > b || a < min_v || b > max_v))
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
  فراغ سيٽ_range (hb_codepoint_t a, hb_codepoint_t b, منطقي v)
  { إذا (v) جوڙيو_range (a, b); وإلا del_range (a, b); }

  سانچو <قسم_نالو سيٽ_t>
  فراغ union_ (ثابت سيٽ_t &سيٽ)
  {
    لكل (hb_codepoint_t g : سيٽ)
      جوڙيو (g);
  }

  ساكن ثابت بنانشان صحيح ELT_بٽ = حجم (elt_t) * 8;
  ساكن constexpr بنانشان ELT_نقاب = ELT_بٽ - 1;

  ساكن constexpr elt_t zero = 0;

  elt_t &elt (hb_codepoint_t g)
  {
    g -= min_v;
    إذا (unlikely (g >= ڳڻپ))
      أرجع Crap(elt_t);
    أرجع elts[g / ELT_بٽ];
  }
  ثابت elt_t& elt (hb_codepoint_t g) ثابت
  {
    g -= min_v;
    إذا (unlikely (g >= ڳڻپ))
      أرجع Null(elt_t);
    أرجع elts[g / ELT_بٽ];
  }

  ساكن constexpr int_t نقاب (hb_codepoint_t g) { أرجع elt_t (1) << (g & ELT_نقاب); }

  hb_codepoint_t min_v = 0, max_v = 0, ڳڻپ = 0;
  elt_t *elts = خالي_اشارو;
};


#جيڪڏهن_پورو /* HB_BIT_VECTOR_HH */

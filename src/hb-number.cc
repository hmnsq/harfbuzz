/*
 * Copyright © 2019-2026  Ebrahim Byagowi
 *
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
 */

#جے_بنا_تعریف HB_نمبر_CC
#تعریف_کرو HB_نمبر_CC
#جے_تعریف_شدہ HB_نمبر_CC /* Pacify -Wunused-macros. */

#ضمن "hb.hh"
#ضمن "hb-number.hh"
#ضمن "hb-number-parser.hh"

منطقي
hb_parse_double (ثابت حرف **pp, ثابت حرف *سرا, مزدوج *pv, منطقي سارا_بفر)
{
  ثابت حرف *pend = سرا;
  مزدوج value = hb_strtod (*pp, &pend);
  إذا (unlikely (*pp == pend)) أرجع خطأ;
  إذا (unlikely (سارا_بفر && سرا != pend)) أرجع خطأ;
  *pv = value;
  *pp = pend;
  أرجع سچ;
}

/* Returns the digit value of c in the given base (2..36), or -1 if c
 * isn't a digit in that base. */
ساكن صحيح
hb_digit_قدر (حرف c, صحيح ادھار)
{
  صحيح d;
  إذا ('0' <= c && c <= '9')      d = c - '0';
  وإلا إذا ('a' <= c && c <= 'z') d = c - 'a' + 10;
  وإلا إذا ('A' <= c && c <= 'Z') d = c - 'A' + 10;
  وإلا أرجع -1;
  أرجع d < ادھار ? d : -1;
}

/* Consumes digits in [p, pe) into *value (magnitude).  Sets *overflow
 * if the value would exceed UINT64_MAX.  Sets *any_digits if at least
 * one digit was consumed.  Returns the position past the last digit. */
ساكن ثابت حرف *
hb_جانچ_digits (ثابت حرف *p, ثابت حرف *pe, صحيح ادھار,
                 uint64_t *value, منطقي *حد_توں_ودھ, منطقي *any_digits)
{
  ثابت uint64_t cutoff = UINT64_MAX / (بنانشانی) ادھار;
  ثابت uint64_t cutlim = UINT64_MAX % (بنانشانی) ادھار;
  uint64_t v = 0;
  منطقي ov = خطأ, any = خطأ;

  لكل (; p < pe; p++)
  {
    صحيح d = hb_digit_قدر (*p, ادھار);
    إذا (d < 0) اكسر;
    any = سچ;
    إذا (v > cutoff || (v == cutoff && (uint64_t) d > cutlim))
      ov = سچ;
    وإلا
      v = v * (بنانشانی) ادھار + (uint64_t) d;
  }

  *value = v;
  *حد_توں_ودھ = ov;
  *any_digits = any;
  أرجع p;
}

سانچا<قسم_ناں T, قسم_ناں Postprocess>
ساكن منطقي
parse_نمبر (ثابت حرف **pp, ثابت حرف *سرا, T *pv,
               منطقي سارا_بفر, صحيح ادھار, Postprocess post)
{
  ثابت حرف *pend = سرا;
  ثابت حرف *p = *pp;
  منطقي neg = خطأ, any = خطأ, حد_توں_ودھ = خطأ;

  طالما (p < pend && ISSPACE (*p)) p++;
  إذا (p < pend && (*p == '+' || *p == '-')) { neg = (*p == '-'); p++; }

  /* Skip an optional "0x"/"0X" prefix for base 16, but only when a hex
   * digit follows — "0x" alone parses as the digit 0, matching strtoul. */
  إذا (ادھار == 16 && p + 2 < pend && p[0] == '0' &&
      (p[1] == 'x' || p[1] == 'X') && hb_digit_قدر (p[2], 16) >= 0)
    p += 2;

  uint64_t v = 0;
  p = hb_جانچ_digits (p, pend, ادھار, &v, &حد_توں_ودھ, &any);
  إذا (unlikely (!any || حد_توں_ودھ)) أرجع خطأ;

  T باہرلا;
  إذا (unlikely (!post (v, neg, &باہرلا))) أرجع خطأ;
  إذا (unlikely (سارا_بفر && p != pend)) أرجع خطأ;

  *pv = باہرلا;
  *pp = p;
  أرجع سچ;
}

منطقي
hb_parse_int (ثابت حرف **pp, ثابت حرف *سرا, صحيح *pv, منطقي سارا_بفر)
{
  أرجع parse_نمبر (pp, سرا, pv, سارا_بفر, 10,
                        [] (uint64_t v, منطقي neg, صحيح *باہرلا) {
    /* INT_MAX = 2^31-1, INT_MIN magnitude = 2^31 */
    uint64_t حد_بندی = neg ? (uint64_t) ودھ_توں_ودھ_عدد + 1 : (uint64_t) ودھ_توں_ودھ_عدد;
    إذا (unlikely (v > حد_بندی)) أرجع خطأ;
    *باہرلا = (صحيح) (neg ? -(int64_t) v : (int64_t) v);
    أرجع سچ;
  });
}

منطقي
hb_parse_uint (ثابت حرف **pp, ثابت حرف *سرا, بنانشانی *pv,
               منطقي سارا_بفر, صحيح ادھار)
{
  أرجع parse_نمبر (pp, سرا, pv, سارا_بفر, ادھار,
                        [] (uint64_t v, منطقي neg, بنانشانی *باہرلا) {
    إذا (unlikely (v > UINT_MAX)) أرجع خطأ;
    /* Negate in unsigned so -1 wraps to UINT_MAX, matching strtoul
     * truncated to unsigned. */
    بنانشانی u = (بنانشانی) v;
    *باہرلا = neg ? (بنانشانی) (0u - u) : u;
    أرجع سچ;
  });
}

#جے_انت /* HB_NUMBER_CC pacify */
#جے_انت /* HB_NUMBER_CC guard */

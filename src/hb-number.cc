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

#که_نه_تعریف_شوی HB_شمېره_CC
#تعریف_کړه HB_شمېره_CC
#که_تعریف_شوی HB_شمېره_CC /* Pacify -Wunused-macros. */

#ضمن "hb.hh"
#ضمن "hb-number.hh"
#ضمن "hb-number-parser.hh"

منطقي
hb_parse_double (ثابت حرف **pp, ثابت حرف *پای_ټکی, مزدوج *pv, منطقي ټولیز_بفر)
{
  ثابت حرف *pend = پای_ټکی;
  مزدوج value = hb_strtod (*pp, &pend);
  إذا (unlikely (*pp == pend)) أرجع خطأ;
  إذا (unlikely (ټولیز_بفر && پای_ټکی != pend)) أرجع خطأ;
  *pv = value;
  *pp = pend;
  أرجع ریښتیا;
}

/* Returns the digit value of c in the given base (2..36), or -1 if c
 * isn't a digit in that base. */
ساكن صحيح
hb_digit_ارزښت (حرف c, صحيح بنسټ)
{
  صحيح d;
  إذا ('0' <= c && c <= '9')      d = c - '0';
  وإلا إذا ('a' <= c && c <= 'z') d = c - 'a' + 10;
  وإلا إذا ('A' <= c && c <= 'Z') d = c - 'A' + 10;
  وإلا أرجع -1;
  أرجع d < بنسټ ? d : -1;
}

/* Consumes digits in [p, pe) into *value (magnitude).  Sets *overflow
 * if the value would exceed UINT64_MAX.  Sets *any_digits if at least
 * one digit was consumed.  Returns the position past the last digit. */
ساكن ثابت حرف *
hb_لټون_digits (ثابت حرف *p, ثابت حرف *pe, صحيح بنسټ,
                 uint64_t *value, منطقي *ډکېدنه, منطقي *any_digits)
{
  ثابت uint64_t cutoff = UINT64_MAX / (بینښه) بنسټ;
  ثابت uint64_t cutlim = UINT64_MAX % (بینښه) بنسټ;
  uint64_t v = 0;
  منطقي ov = خطأ, any = خطأ;

  لكل (; p < pe; p++)
  {
    صحيح d = hb_digit_ارزښت (*p, بنسټ);
    إذا (d < 0) اكسر;
    any = ریښتیا;
    إذا (v > cutoff || (v == cutoff && (uint64_t) d > cutlim))
      ov = ریښتیا;
    وإلا
      v = v * (بینښه) بنسټ + (uint64_t) d;
  }

  *value = v;
  *ډکېدنه = ov;
  *any_digits = any;
  أرجع p;
}

کينډۍ<ډول_نوم T, ډول_نوم Postprocess>
ساكن منطقي
parse_شمېره (ثابت حرف **pp, ثابت حرف *پای_ټکی, T *pv,
               منطقي ټولیز_بفر, صحيح بنسټ, Postprocess post)
{
  ثابت حرف *pend = پای_ټکی;
  ثابت حرف *p = *pp;
  منطقي neg = خطأ, any = خطأ, ډکېدنه = خطأ;

  طالما (p < pend && ISSPACE (*p)) p++;
  إذا (p < pend && (*p == '+' || *p == '-')) { neg = (*p == '-'); p++; }

  /* Skip an optional "0x"/"0X" prefix for base 16, but only when a hex
   * digit follows — "0x" alone parses as the digit 0, matching strtoul. */
  إذا (بنسټ == 16 && p + 2 < pend && p[0] == '0' &&
      (p[1] == 'x' || p[1] == 'X') && hb_digit_ارزښت (p[2], 16) >= 0)
    p += 2;

  uint64_t v = 0;
  p = hb_لټون_digits (p, pend, بنسټ, &v, &ډکېدنه, &any);
  إذا (unlikely (!any || ډکېدنه)) أرجع خطأ;

  T بهر;
  إذا (unlikely (!post (v, neg, &بهر))) أرجع خطأ;
  إذا (unlikely (ټولیز_بفر && p != pend)) أرجع خطأ;

  *pv = بهر;
  *pp = p;
  أرجع ریښتیا;
}

منطقي
hb_parse_int (ثابت حرف **pp, ثابت حرف *پای_ټکی, صحيح *pv, منطقي ټولیز_بفر)
{
  أرجع parse_شمېره (pp, پای_ټکی, pv, ټولیز_بفر, 10,
                        [] (uint64_t v, منطقي neg, صحيح *بهر) {
    /* INT_MAX = 2^31-1, INT_MIN magnitude = 2^31 */
    uint64_t حد = neg ? (uint64_t) لوی_عدد + 1 : (uint64_t) لوی_عدد;
    إذا (unlikely (v > حد)) أرجع خطأ;
    *بهر = (صحيح) (neg ? -(int64_t) v : (int64_t) v);
    أرجع ریښتیا;
  });
}

منطقي
hb_parse_uint (ثابت حرف **pp, ثابت حرف *پای_ټکی, بینښه *pv,
               منطقي ټولیز_بفر, صحيح بنسټ)
{
  أرجع parse_شمېره (pp, پای_ټکی, pv, ټولیز_بفر, بنسټ,
                        [] (uint64_t v, منطقي neg, بینښه *بهر) {
    إذا (unlikely (v > UINT_MAX)) أرجع خطأ;
    /* Negate in unsigned so -1 wraps to UINT_MAX, matching strtoul
     * truncated to unsigned. */
    بینښه u = (بینښه) v;
    *بهر = neg ? (بینښه) (0u - u) : u;
    أرجع ریښتیا;
  });
}

#د_که_پای /* HB_NUMBER_CC pacify */
#د_که_پای /* HB_NUMBER_CC guard */

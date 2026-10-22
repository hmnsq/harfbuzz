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

#جيڪڏهن_وصف_نه_ٿيل HB_عدد_CC
#وصف_ڪريو HB_عدد_CC
#جيڪڏهن_وصف_ٿيل HB_عدد_CC /* Pacify -Wunused-macros. */

#ضمن "hb.hh"
#ضمن "hb-number.hh"
#ضمن "hb-number-parser.hh"

منطقي
hb_parse_double (ثابت حرف **pp, ثابت حرف *پڇاڙي, مزدوج *pv, منطقي سڄو_بفر)
{
  ثابت حرف *pend = پڇاڙي;
  مزدوج value = hb_strtod (*pp, &pend);
  إذا (unlikely (*pp == pend)) أرجع خطأ;
  إذا (unlikely (سڄو_بفر && پڇاڙي != pend)) أرجع خطأ;
  *pv = value;
  *pp = pend;
  أرجع سچ;
}

/* Returns the digit value of c in the given base (2..36), or -1 if c
 * isn't a digit in that base. */
ساكن صحيح
hb_digit_قدر (حرف c, صحيح بنياد)
{
  صحيح d;
  إذا ('0' <= c && c <= '9')      d = c - '0';
  وإلا إذا ('a' <= c && c <= 'z') d = c - 'a' + 10;
  وإلا إذا ('A' <= c && c <= 'Z') d = c - 'A' + 10;
  وإلا أرجع -1;
  أرجع d < بنياد ? d : -1;
}

/* Consumes digits in [p, pe) into *value (magnitude).  Sets *overflow
 * if the value would exceed UINT64_MAX.  Sets *any_digits if at least
 * one digit was consumed.  Returns the position past the last digit. */
ساكن ثابت حرف *
hb_جاچ_digits (ثابت حرف *p, ثابت حرف *pe, صحيح بنياد,
                 uint64_t *value, منطقي *حد_کان_مٿي, منطقي *any_digits)
{
  ثابت uint64_t cutoff = UINT64_MAX / (بنانشان) بنياد;
  ثابت uint64_t cutlim = UINT64_MAX % (بنانشان) بنياد;
  uint64_t v = 0;
  منطقي ov = خطأ, any = خطأ;

  لكل (; p < pe; p++)
  {
    صحيح d = hb_digit_قدر (*p, بنياد);
    إذا (d < 0) اكسر;
    any = سچ;
    إذا (v > cutoff || (v == cutoff && (uint64_t) d > cutlim))
      ov = سچ;
    وإلا
      v = v * (بنانشان) بنياد + (uint64_t) d;
  }

  *value = v;
  *حد_کان_مٿي = ov;
  *any_digits = any;
  أرجع p;
}

سانچو<قسم_نالو T, قسم_نالو Postprocess>
ساكن منطقي
parse_عدد (ثابت حرف **pp, ثابت حرف *پڇاڙي, T *pv,
               منطقي سڄو_بفر, صحيح بنياد, Postprocess post)
{
  ثابت حرف *pend = پڇاڙي;
  ثابت حرف *p = *pp;
  منطقي neg = خطأ, any = خطأ, حد_کان_مٿي = خطأ;

  طالما (p < pend && ISSPACE (*p)) p++;
  إذا (p < pend && (*p == '+' || *p == '-')) { neg = (*p == '-'); p++; }

  /* Skip an optional "0x"/"0X" prefix for base 16, but only when a hex
   * digit follows — "0x" alone parses as the digit 0, matching strtoul. */
  إذا (بنياد == 16 && p + 2 < pend && p[0] == '0' &&
      (p[1] == 'x' || p[1] == 'X') && hb_digit_قدر (p[2], 16) >= 0)
    p += 2;

  uint64_t v = 0;
  p = hb_جاچ_digits (p, pend, بنياد, &v, &حد_کان_مٿي, &any);
  إذا (unlikely (!any || حد_کان_مٿي)) أرجع خطأ;

  T ٻاهريون;
  إذا (unlikely (!post (v, neg, &ٻاهريون))) أرجع خطأ;
  إذا (unlikely (سڄو_بفر && p != pend)) أرجع خطأ;

  *pv = ٻاهريون;
  *pp = p;
  أرجع سچ;
}

منطقي
hb_parse_int (ثابت حرف **pp, ثابت حرف *پڇاڙي, صحيح *pv, منطقي سڄو_بفر)
{
  أرجع parse_عدد (pp, پڇاڙي, pv, سڄو_بفر, 10,
                        [] (uint64_t v, منطقي neg, صحيح *ٻاهريون) {
    /* INT_MAX = 2^31-1, INT_MIN magnitude = 2^31 */
    uint64_t حد_بندي = neg ? (uint64_t) وڌ_کان_وڌ_عدد + 1 : (uint64_t) وڌ_کان_وڌ_عدد;
    إذا (unlikely (v > حد_بندي)) أرجع خطأ;
    *ٻاهريون = (صحيح) (neg ? -(int64_t) v : (int64_t) v);
    أرجع سچ;
  });
}

منطقي
hb_parse_uint (ثابت حرف **pp, ثابت حرف *پڇاڙي, بنانشان *pv,
               منطقي سڄو_بفر, صحيح بنياد)
{
  أرجع parse_عدد (pp, پڇاڙي, pv, سڄو_بفر, بنياد,
                        [] (uint64_t v, منطقي neg, بنانشان *ٻاهريون) {
    إذا (unlikely (v > UINT_MAX)) أرجع خطأ;
    /* Negate in unsigned so -1 wraps to UINT_MAX, matching strtoul
     * truncated to unsigned. */
    بنانشان u = (بنانشان) v;
    *ٻاهريون = neg ? (بنانشان) (0u - u) : u;
    أرجع سچ;
  });
}

#جيڪڏهن_پورو /* HB_NUMBER_CC pacify */
#جيڪڏهن_پورو /* HB_NUMBER_CC guard */

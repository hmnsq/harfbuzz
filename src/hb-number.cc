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

#اگر_غیر_تعریف_شدہ HB_نمبر_CC
#تعریف_کرو HB_نمبر_CC
#اگر_تعریف_شدہ HB_نمبر_CC /* Pacify -Wunused-macros. */

#شامل "hb.hh"
#شامل "hb-number.hh"
#شامل "hb-number-parser.hh"

منطقی
hb_parse_double (ثابت حرف **pp, ثابت حرف *انتہا, دوہرا *pv, منطقی پورا_بفر)
{
  ثابت حرف *pend = انتہا;
  دوہرا value = hb_strtod (*pp, &pend);
  اگر (unlikely (*pp == pend)) واپس جھوٹ;
  اگر (unlikely (پورا_بفر && انتہا != pend)) واپس جھوٹ;
  *pv = value;
  *pp = pend;
  واپس سچ;
}

/* Returns the digit value of c in the given base (2..36), or -1 if c
 * isn't a digit in that base. */
ساکن صحیح
hb_digit_قدر (حرف c, صحیح بنیاد)
{
  صحیح d;
  اگر ('0' <= c && c <= '9')      d = c - '0';
  ورنہ اگر ('a' <= c && c <= 'z') d = c - 'a' + 10;
  ورنہ اگر ('A' <= c && c <= 'Z') d = c - 'A' + 10;
  ورنہ واپس -1;
  واپس d < بنیاد ? d : -1;
}

/* Consumes digits in [p, pe) into *value (magnitude).  Sets *overflow
 * if the value would exceed UINT64_MAX.  Sets *any_digits if at least
 * one digit was consumed.  Returns the position past the last digit. */
ساکن ثابت حرف *
hb_جائزہ_digits (ثابت حرف *p, ثابت حرف *pe, صحیح بنیاد,
                 uint64_t *value, منطقی *حد_سے_تجاوز, منطقی *any_digits)
{
  ثابت uint64_t cutoff = UINT64_MAX / (غیرعلامتی) بنیاد;
  ثابت uint64_t cutlim = UINT64_MAX % (غیرعلامتی) بنیاد;
  uint64_t v = 0;
  منطقی ov = جھوٹ, any = جھوٹ;

  برائے (; p < pe; p++)
  {
    صحیح d = hb_digit_قدر (*p, بنیاد);
    اگر (d < 0) توڑو;
    any = سچ;
    اگر (v > cutoff || (v == cutoff && (uint64_t) d > cutlim))
      ov = سچ;
    ورنہ
      v = v * (غیرعلامتی) بنیاد + (uint64_t) d;
  }

  *value = v;
  *حد_سے_تجاوز = ov;
  *any_digits = any;
  واپس p;
}

سانچہ<قسم_نام T, قسم_نام Postprocess>
ساکن منطقی
parse_نمبر (ثابت حرف **pp, ثابت حرف *انتہا, T *pv,
               منطقی پورا_بفر, صحیح بنیاد, Postprocess post)
{
  ثابت حرف *pend = انتہا;
  ثابت حرف *p = *pp;
  منطقی neg = جھوٹ, any = جھوٹ, حد_سے_تجاوز = جھوٹ;

  جب (p < pend && ISSPACE (*p)) p++;
  اگر (p < pend && (*p == '+' || *p == '-')) { neg = (*p == '-'); p++; }

  /* Skip an optional "0x"/"0X" prefix for base 16, but only when a hex
   * digit follows — "0x" alone parses as the digit 0, matching strtoul. */
  اگر (بنیاد == 16 && p + 2 < pend && p[0] == '0' &&
      (p[1] == 'x' || p[1] == 'X') && hb_digit_قدر (p[2], 16) >= 0)
    p += 2;

  uint64_t v = 0;
  p = hb_جائزہ_digits (p, pend, بنیاد, &v, &حد_سے_تجاوز, &any);
  اگر (unlikely (!any || حد_سے_تجاوز)) واپس جھوٹ;

  T برآمد;
  اگر (unlikely (!post (v, neg, &برآمد))) واپس جھوٹ;
  اگر (unlikely (پورا_بفر && p != pend)) واپس جھوٹ;

  *pv = برآمد;
  *pp = p;
  واپس سچ;
}

منطقی
hb_parse_int (ثابت حرف **pp, ثابت حرف *انتہا, صحیح *pv, منطقی پورا_بفر)
{
  واپس parse_نمبر (pp, انتہا, pv, پورا_بفر, 10,
                        [] (uint64_t v, منطقی neg, صحیح *برآمد) {
    /* INT_MAX = 2^31-1, INT_MIN magnitude = 2^31 */
    uint64_t حد_بندی = neg ? (uint64_t) زیادہ_سے_زیادہ_عدد + 1 : (uint64_t) زیادہ_سے_زیادہ_عدد;
    اگر (unlikely (v > حد_بندی)) واپس جھوٹ;
    *برآمد = (صحیح) (neg ? -(int64_t) v : (int64_t) v);
    واپس سچ;
  });
}

منطقی
hb_parse_uint (ثابت حرف **pp, ثابت حرف *انتہا, غیرعلامتی *pv,
               منطقی پورا_بفر, صحیح بنیاد)
{
  واپس parse_نمبر (pp, انتہا, pv, پورا_بفر, بنیاد,
                        [] (uint64_t v, منطقی neg, غیرعلامتی *برآمد) {
    اگر (unlikely (v > UINT_MAX)) واپس جھوٹ;
    /* Negate in unsigned so -1 wraps to UINT_MAX, matching strtoul
     * truncated to unsigned. */
    غیرعلامتی u = (غیرعلامتی) v;
    *برآمد = neg ? (غیرعلامتی) (0u - u) : u;
    واپس سچ;
  });
}

#اگر_ختم /* HB_NUMBER_CC pacify */
#اگر_ختم /* HB_NUMBER_CC guard */

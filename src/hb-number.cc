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

#ifndef HB_عدد_CC
#define HB_عدد_CC
#ifdef HB_عدد_CC /* Pacify -Wunused-macros. */

#شامل "hb.hh"
#شامل "hb-number.hh"
#شامل "hb-number-parser.hh"

منطقی
hb_parse_double (ثابت نویسه **pp, ثابت نویسه *end, دوگانه *pv, منطقی whole_buffer)
{
  ثابت نویسه *pend = end;
  دوگانه value = hb_strtod (*pp, &pend);
  اگر (unlikely (*pp == pend)) بازگشت نادرست;
  اگر (unlikely (whole_buffer && end != pend)) بازگشت نادرست;
  *pv = value;
  *pp = pend;
  بازگشت درست;
}

/* Returns the digit value of c in the given base (2..36), or -1 if c
 * isn't a digit in that base. */
ایستا صحیح
hb_digit_مقدار (نویسه c, صحیح base)
{
  صحیح d;
  اگر ('0' <= c && c <= '9')      d = c - '0';
  وگرنه اگر ('a' <= c && c <= 'z') d = c - 'a' + 10;
  وگرنه اگر ('A' <= c && c <= 'Z') d = c - 'A' + 10;
  وگرنه بازگشت -1;
  بازگشت d < base ? d : -1;
}

/* Consumes digits in [p, pe) into *value (magnitude).  Sets *overflow
 * if the value would exceed UINT64_MAX.  Sets *any_digits if at least
 * one digit was consumed.  Returns the position past the last digit. */
ایستا ثابت نویسه *
_hb_scan_digits (ثابت نویسه *p, ثابت نویسه *pe, صحیح base,
                 uint64_t *value, منطقی *overflow, منطقی *any_digits)
{
  ثابت uint64_t cutoff = UINT64_MAX / (unsigned) base;
  ثابت uint64_t cutlim = UINT64_MAX % (unsigned) base;
  uint64_t v = 0;
  منطقی ov = نادرست, any = نادرست;

  برای (; p < pe; p++)
  {
    صحیح d = hb_digit_مقدار (*p, base);
    اگر (d < 0) بشکن;
    any = درست;
    اگر (v > cutoff || (v == cutoff && (uint64_t) d > cutlim))
      ov = درست;
    وگرنه
      v = v * (unsigned) base + (uint64_t) d;
  }

  *value = v;
  *overflow = ov;
  *any_digits = any;
  بازگشت p;
}

template<typename T, typename Postprocess>
ایستا منطقی
parse_عدد (ثابت نویسه **pp, ثابت نویسه *end, T *pv,
               منطقی whole_buffer, صحیح base, Postprocess post)
{
  ثابت نویسه *pend = end;
  ثابت نویسه *p = *pp;
  منطقی neg = نادرست, any = نادرست, overflow = نادرست;

  وقتی (p < pend && ISSPACE (*p)) p++;
  اگر (p < pend && (*p == '+' || *p == '-')) { neg = (*p == '-'); p++; }

  /* Skip an optional "0x"/"0X" prefix for base 16, but only when a hex
   * digit follows — "0x" alone parses as the digit 0, matching strtoul. */
  اگر (base == 16 && p + 2 < pend && p[0] == '0' &&
      (p[1] == 'x' || p[1] == 'X') && hb_digit_مقدار (p[2], 16) >= 0)
    p += 2;

  uint64_t v = 0;
  p = _hb_scan_digits (p, pend, base, &v, &overflow, &any);
  اگر (unlikely (!any || overflow)) بازگشت نادرست;

  T out;
  اگر (unlikely (!post (v, neg, &out))) بازگشت نادرست;
  اگر (unlikely (whole_buffer && p != pend)) بازگشت نادرست;

  *pv = out;
  *pp = p;
  بازگشت درست;
}

منطقی
hb_parse_int (ثابت نویسه **pp, ثابت نویسه *end, صحیح *pv, منطقی whole_buffer)
{
  بازگشت parse_عدد (pp, end, pv, whole_buffer, 10,
                        [] (uint64_t v, منطقی neg, صحیح *out) {
    /* INT_MAX = 2^31-1, INT_MIN magnitude = 2^31 */
    uint64_t limit = neg ? (uint64_t) INT_MAX + 1 : (uint64_t) INT_MAX;
    اگر (unlikely (v > limit)) بازگشت نادرست;
    *out = (صحیح) (neg ? -(int64_t) v : (int64_t) v);
    بازگشت درست;
  });
}

منطقی
hb_parse_uint (ثابت نویسه **pp, ثابت نویسه *end, unsigned *pv,
               منطقی whole_buffer, صحیح base)
{
  بازگشت parse_عدد (pp, end, pv, whole_buffer, base,
                        [] (uint64_t v, منطقی neg, unsigned *out) {
    اگر (unlikely (v > UINT_MAX)) بازگشت نادرست;
    /* Negate in unsigned so -1 wraps to UINT_MAX, matching strtoul
     * truncated to unsigned. */
    unsigned u = (unsigned) v;
    *out = neg ? (unsigned) (0u - u) : u;
    بازگشت درست;
  });
}

#endif /* HB_NUMBER_CC pacify */
#endif /* HB_NUMBER_CC guard */

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

#ifndef HB_NUMBER_CC
#define HB_NUMBER_CC
#ifdef HB_NUMBER_CC /* Pacify -Wunused-macros. */

#ضمن "hb.hh"
#ضمن "hb-number.hh"
#ضمن "hb-number-parser.hh"

منطقي
hb_parse_double (ثابت حرف **pp, ثابت حرف *end, مزدوج *pv, منطقي whole_buffer)
{
  ثابت حرف *pend = end;
  مزدوج value = hb_strtod (*pp, &pend);
  אם (unlikely (*pp == pend)) החזר שקר;
  אם (unlikely (whole_buffer && end != pend)) החזר שקר;
  *pv = value;
  *pp = pend;
  החזר אמת;
}

/* Returns the digit value of c in the given base (2..36), or -1 if c
 * isn't a digit in that base. */
ساكن صحيح
_hb_digit_value (حرف c, صحيح base)
{
  صحيح d;
  אם ('0' <= c && c <= '9')      d = c - '0';
  אחרת אם ('a' <= c && c <= 'z') d = c - 'a' + 10;
  אחרת אם ('A' <= c && c <= 'Z') d = c - 'A' + 10;
  אחרת החזר -1;
  החזר d < base ? d : -1;
}

/* Consumes digits in [p, pe) into *value (magnitude).  Sets *overflow
 * if the value would exceed UINT64_MAX.  Sets *any_digits if at least
 * one digit was consumed.  Returns the position past the last digit. */
ساكن ثابت حرف *
_hb_scan_digits (ثابت حرف *p, ثابت حرف *pe, صحيح base,
                 uint64_t *value, منطقي *overflow, منطقي *any_digits)
{
  ثابت uint64_t cutoff = UINT64_MAX / (unsigned) base;
  ثابت uint64_t cutlim = UINT64_MAX % (unsigned) base;
  uint64_t v = 0;
  منطقي ov = שקר, any = שקר;

  עבור (; p < pe; p++)
  {
    صحيح d = _hb_digit_value (*p, base);
    אם (d < 0) اكسر;
    any = אמת;
    אם (v > cutoff || (v == cutoff && (uint64_t) d > cutlim))
      ov = אמת;
    אחרת
      v = v * (unsigned) base + (uint64_t) d;
  }

  *value = v;
  *overflow = ov;
  *any_digits = any;
  החזר p;
}

template<typename T, typename Postprocess>
ساكن منطقي
_parse_number (ثابت حرف **pp, ثابت حرف *end, T *pv,
               منطقي whole_buffer, صحيح base, Postprocess post)
{
  ثابت حرف *pend = end;
  ثابت حرف *p = *pp;
  منطقي neg = שקר, any = שקר, overflow = שקר;

  כלעוד (p < pend && ISSPACE (*p)) p++;
  אם (p < pend && (*p == '+' || *p == '-')) { neg = (*p == '-'); p++; }

  /* Skip an optional "0x"/"0X" prefix for base 16, but only when a hex
   * digit follows — "0x" alone parses as the digit 0, matching strtoul. */
  אם (base == 16 && p + 2 < pend && p[0] == '0' &&
      (p[1] == 'x' || p[1] == 'X') && _hb_digit_value (p[2], 16) >= 0)
    p += 2;

  uint64_t v = 0;
  p = _hb_scan_digits (p, pend, base, &v, &overflow, &any);
  אם (unlikely (!any || overflow)) החזר שקר;

  T out;
  אם (unlikely (!post (v, neg, &out))) החזר שקר;
  אם (unlikely (whole_buffer && p != pend)) החזר שקר;

  *pv = out;
  *pp = p;
  החזר אמת;
}

منطقي
hb_parse_int (ثابت حرف **pp, ثابت حرف *end, صحيح *pv, منطقي whole_buffer)
{
  החזר _parse_number (pp, end, pv, whole_buffer, 10,
                        [] (uint64_t v, منطقي neg, صحيح *out) {
    /* INT_MAX = 2^31-1, INT_MIN magnitude = 2^31 */
    uint64_t limit = neg ? (uint64_t) INT_MAX + 1 : (uint64_t) INT_MAX;
    אם (unlikely (v > limit)) החזר שקר;
    *out = (صحيح) (neg ? -(int64_t) v : (int64_t) v);
    החזר אמת;
  });
}

منطقي
hb_parse_uint (ثابت حرف **pp, ثابت حرف *end, unsigned *pv,
               منطقي whole_buffer, صحيح base)
{
  החזר _parse_number (pp, end, pv, whole_buffer, base,
                        [] (uint64_t v, منطقي neg, unsigned *out) {
    אם (unlikely (v > UINT_MAX)) החזר שקר;
    /* Negate in unsigned so -1 wraps to UINT_MAX, matching strtoul
     * truncated to unsigned. */
    unsigned u = (unsigned) v;
    *out = neg ? (unsigned) (0u - u) : u;
    החזר אמת;
  });
}

#endif /* HB_NUMBER_CC pacify */
#endif /* HB_NUMBER_CC guard */

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

#ifndef HB_لمب_CC
#define HB_لمب_CC
#ifdef HB_لمب_CC /* Pacify -Wunused-macros. */

#ضمن "hb.hh"
#ضمن "hb-number.hh"
#ضمن "hb-number-parser.hh"

منطقي
hb_parse_double (ثابت حرف **pp, ثابت حرف *end, مزدوج *pv, منطقي whole_buffer)
{
  ثابت حرف *pend = end;
  مزدوج value = hb_strtod (*pp, &pend);
  إذا (unlikely (*pp == pend)) أرجع خطأ;
  إذا (unlikely (whole_buffer && end != pend)) أرجع خطأ;
  *pv = value;
  *pp = pend;
  أرجع true;
}

/* Returns the digit value of c in the given base (2..36), or -1 if c
 * isn't a digit in that base. */
ساكن صحيح
hb_digit_درج (حرف c, صحيح base)
{
  صحيح d;
  إذا ('0' <= c && c <= '9')      d = c - '0';
  وإلا إذا ('a' <= c && c <= 'z') d = c - 'a' + 10;
  وإلا إذا ('A' <= c && c <= 'Z') d = c - 'A' + 10;
  وإلا أرجع -1;
  أرجع d < base ? d : -1;
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
  منطقي ov = خطأ, any = خطأ;

  لكل (; p < pe; p++)
  {
    صحيح d = hb_digit_درج (*p, base);
    إذا (d < 0) اكسر;
    any = true;
    إذا (v > cutoff || (v == cutoff && (uint64_t) d > cutlim))
      ov = true;
    وإلا
      v = v * (unsigned) base + (uint64_t) d;
  }

  *value = v;
  *overflow = ov;
  *any_digits = any;
  أرجع p;
}

template<typename T, typename Postprocess>
ساكن منطقي
parse_لمب (ثابت حرف **pp, ثابت حرف *end, T *pv,
               منطقي whole_buffer, صحيح base, Postprocess post)
{
  ثابت حرف *pend = end;
  ثابت حرف *p = *pp;
  منطقي neg = خطأ, any = خطأ, overflow = خطأ;

  طالما (p < pend && ISSPACE (*p)) p++;
  إذا (p < pend && (*p == '+' || *p == '-')) { neg = (*p == '-'); p++; }

  /* Skip an optional "0x"/"0X" prefix for base 16, but only when a hex
   * digit follows — "0x" alone parses as the digit 0, matching strtoul. */
  إذا (base == 16 && p + 2 < pend && p[0] == '0' &&
      (p[1] == 'x' || p[1] == 'X') && hb_digit_درج (p[2], 16) >= 0)
    p += 2;

  uint64_t v = 0;
  p = _hb_scan_digits (p, pend, base, &v, &overflow, &any);
  إذا (unlikely (!any || overflow)) أرجع خطأ;

  T out;
  إذا (unlikely (!post (v, neg, &out))) أرجع خطأ;
  إذا (unlikely (whole_buffer && p != pend)) أرجع خطأ;

  *pv = out;
  *pp = p;
  أرجع true;
}

منطقي
hb_parse_int (ثابت حرف **pp, ثابت حرف *end, صحيح *pv, منطقي whole_buffer)
{
  أرجع parse_لمب (pp, end, pv, whole_buffer, 10,
                        [] (uint64_t v, منطقي neg, صحيح *out) {
    /* INT_MAX = 2^31-1, INT_MIN magnitude = 2^31 */
    uint64_t limit = neg ? (uint64_t) INT_MAX + 1 : (uint64_t) INT_MAX;
    إذا (unlikely (v > limit)) أرجع خطأ;
    *out = (صحيح) (neg ? -(int64_t) v : (int64_t) v);
    أرجع true;
  });
}

منطقي
hb_parse_uint (ثابت حرف **pp, ثابت حرف *end, unsigned *pv,
               منطقي whole_buffer, صحيح base)
{
  أرجع parse_لمب (pp, end, pv, whole_buffer, base,
                        [] (uint64_t v, منطقي neg, unsigned *out) {
    إذا (unlikely (v > UINT_MAX)) أرجع خطأ;
    /* Negate in unsigned so -1 wraps to UINT_MAX, matching strtoul
     * truncated to unsigned. */
    unsigned u = (unsigned) v;
    *out = neg ? (unsigned) (0u - u) : u;
    أرجع true;
  });
}

#endif /* HB_NUMBER_CC pacify */
#endif /* HB_NUMBER_CC guard */

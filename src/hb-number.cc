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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ਸੰਖਿਆ_CC
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਸੰਖਿਆ_CC
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_ਸੰਖਿਆ_CC /* Pacify -Wunused-macros. */

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-number.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-number-parser.hh"

ਤਾਰਕਿਕ
hb_parse_double (ਅਚਲ ਅੱਖਰ **pp, ਅਚਲ ਅੱਖਰ *ਸਿਰਾ, ਦੁੱਗਣਾ *pv, ਤਾਰਕਿਕ ਸਾਰਾ_ਬਫਰ)
{
  ਅਚਲ ਅੱਖਰ *pend = ਸਿਰਾ;
  ਦੁੱਗਣਾ value = hb_strtod (*pp, &pend);
  ਜੇ (unlikely (*pp == pend)) ਵਾਪਸ ਝੂਠ;
  ਜੇ (unlikely (ਸਾਰਾ_ਬਫਰ && ਸਿਰਾ != pend)) ਵਾਪਸ ਝੂਠ;
  *pv = value;
  *pp = pend;
  ਵਾਪਸ ਸੱਚ;
}

/* Returns the digit value of c in the given base (2..36), or -1 if c
 * isn't a digit in that base. */
ਸਥਿਰ ਪੂਰਨ
hb_digit_ਮੁੱਲ (ਅੱਖਰ c, ਪੂਰਨ ਅਧਾਰ)
{
  ਪੂਰਨ d;
  ਜੇ ('0' <= c && c <= '9')      d = c - '0';
  ਨਹੀਂਤਾਂ ਜੇ ('a' <= c && c <= 'z') d = c - 'a' + 10;
  ਨਹੀਂਤਾਂ ਜੇ ('A' <= c && c <= 'Z') d = c - 'A' + 10;
  ਨਹੀਂਤਾਂ ਵਾਪਸ -1;
  ਵਾਪਸ d < ਅਧਾਰ ? d : -1;
}

/* Consumes digits in [p, pe) into *value (magnitude).  Sets *overflow
 * if the value would exceed UINT64_MAX.  Sets *any_digits if at least
 * one digit was consumed.  Returns the position past the last digit. */
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *
hb_ਜਾਂਚ_digits (ਅਚਲ ਅੱਖਰ *p, ਅਚਲ ਅੱਖਰ *pe, ਪੂਰਨ ਅਧਾਰ,
                 uint64_t *value, ਤਾਰਕਿਕ *ਉਛਾਲ, ਤਾਰਕਿਕ *any_digits)
{
  ਅਚਲ uint64_t cutoff = UINT64_MAX / (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਅਧਾਰ;
  ਅਚਲ uint64_t cutlim = UINT64_MAX % (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਅਧਾਰ;
  uint64_t v = 0;
  ਤਾਰਕਿਕ ov = ਝੂਠ, any = ਝੂਠ;

  ਲਈ (; p < pe; p++)
  {
    ਪੂਰਨ d = hb_digit_ਮੁੱਲ (*p, ਅਧਾਰ);
    ਜੇ (d < 0) ਰੋਕੋ;
    any = ਸੱਚ;
    ਜੇ (v > cutoff || (v == cutoff && (uint64_t) d > cutlim))
      ov = ਸੱਚ;
    ਨਹੀਂਤਾਂ
      v = v * (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਅਧਾਰ + (uint64_t) d;
  }

  *value = v;
  *ਉਛਾਲ = ov;
  *any_digits = any;
  ਵਾਪਸ p;
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ Postprocess>
ਸਥਿਰ ਤਾਰਕਿਕ
parse_ਸੰਖਿਆ (ਅਚਲ ਅੱਖਰ **pp, ਅਚਲ ਅੱਖਰ *ਸਿਰਾ, T *pv,
               ਤਾਰਕਿਕ ਸਾਰਾ_ਬਫਰ, ਪੂਰਨ ਅਧਾਰ, Postprocess post)
{
  ਅਚਲ ਅੱਖਰ *pend = ਸਿਰਾ;
  ਅਚਲ ਅੱਖਰ *p = *pp;
  ਤਾਰਕਿਕ neg = ਝੂਠ, any = ਝੂਠ, ਉਛਾਲ = ਝੂਠ;

  ਜਦੋਂ (p < pend && ISSPACE (*p)) p++;
  ਜੇ (p < pend && (*p == '+' || *p == '-')) { neg = (*p == '-'); p++; }

  /* Skip an optional "0x"/"0X" prefix for base 16, but only when a hex
   * digit follows — "0x" alone parses as the digit 0, matching strtoul. */
  ਜੇ (ਅਧਾਰ == 16 && p + 2 < pend && p[0] == '0' &&
      (p[1] == 'x' || p[1] == 'X') && hb_digit_ਮੁੱਲ (p[2], 16) >= 0)
    p += 2;

  uint64_t v = 0;
  p = hb_ਜਾਂਚ_digits (p, pend, ਅਧਾਰ, &v, &ਉਛਾਲ, &any);
  ਜੇ (unlikely (!any || ਉਛਾਲ)) ਵਾਪਸ ਝੂਠ;

  T ਬਾਹਰਲਾ;
  ਜੇ (unlikely (!post (v, neg, &ਬਾਹਰਲਾ))) ਵਾਪਸ ਝੂਠ;
  ਜੇ (unlikely (ਸਾਰਾ_ਬਫਰ && p != pend)) ਵਾਪਸ ਝੂਠ;

  *pv = ਬਾਹਰਲਾ;
  *pp = p;
  ਵਾਪਸ ਸੱਚ;
}

ਤਾਰਕਿਕ
hb_parse_int (ਅਚਲ ਅੱਖਰ **pp, ਅਚਲ ਅੱਖਰ *ਸਿਰਾ, ਪੂਰਨ *pv, ਤਾਰਕਿਕ ਸਾਰਾ_ਬਫਰ)
{
  ਵਾਪਸ parse_ਸੰਖਿਆ (pp, ਸਿਰਾ, pv, ਸਾਰਾ_ਬਫਰ, 10,
                        [] (uint64_t v, ਤਾਰਕਿਕ neg, ਪੂਰਨ *ਬਾਹਰਲਾ) {
    /* INT_MAX = 2^31-1, INT_MIN magnitude = 2^31 */
    uint64_t ਹੱਦਬੰਦੀ = neg ? (uint64_t) ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ + 1 : (uint64_t) ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ;
    ਜੇ (unlikely (v > ਹੱਦਬੰਦੀ)) ਵਾਪਸ ਝੂਠ;
    *ਬਾਹਰਲਾ = (ਪੂਰਨ) (neg ? -(int64_t) v : (int64_t) v);
    ਵਾਪਸ ਸੱਚ;
  });
}

ਤਾਰਕਿਕ
hb_parse_uint (ਅਚਲ ਅੱਖਰ **pp, ਅਚਲ ਅੱਖਰ *ਸਿਰਾ, ਬਿਨਾਨਿਸ਼ਾਨੀ *pv,
               ਤਾਰਕਿਕ ਸਾਰਾ_ਬਫਰ, ਪੂਰਨ ਅਧਾਰ)
{
  ਵਾਪਸ parse_ਸੰਖਿਆ (pp, ਸਿਰਾ, pv, ਸਾਰਾ_ਬਫਰ, ਅਧਾਰ,
                        [] (uint64_t v, ਤਾਰਕਿਕ neg, ਬਿਨਾਨਿਸ਼ਾਨੀ *ਬਾਹਰਲਾ) {
    ਜੇ (unlikely (v > UINT_MAX)) ਵਾਪਸ ਝੂਠ;
    /* Negate in unsigned so -1 wraps to UINT_MAX, matching strtoul
     * truncated to unsigned. */
    ਬਿਨਾਨਿਸ਼ਾਨੀ u = (ਬਿਨਾਨਿਸ਼ਾਨੀ) v;
    *ਬਾਹਰਲਾ = neg ? (ਬਿਨਾਨਿਸ਼ਾਨੀ) (0u - u) : u;
    ਵਾਪਸ ਸੱਚ;
  });
}

#ਜੇ_ਅੰਤ /* HB_NUMBER_CC pacify */
#ਜੇ_ਅੰਤ /* HB_NUMBER_CC guard */

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

#ifndef HB_ಸಂಖ್ಯೆ_CC
#define HB_ಸಂಖ್ಯೆ_CC
#ifdef HB_ಸಂಖ್ಯೆ_CC /* Pacify -Wunused-macros. */

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-number.hh"
#ಅನ್ತರ್ಭಾವ "hb-number-parser.hh"

ತಾರ್ಕಿಕ
hb_parse_double (ಧ್ರುವ ವರ್ಣ **pp, ಧ್ರುವ ವರ್ಣ *end, ದ್ವಿಗುಣ *pv, ತಾರ್ಕಿಕ whole_buffer)
{
  ಧ್ರುವ ವರ್ಣ *pend = end;
  ದ್ವಿಗುಣ value = hb_strtod (*pp, &pend);
  ಯದಿ (unlikely (*pp == pend)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  ಯದಿ (unlikely (whole_buffer && end != pend)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  *pv = value;
  *pp = pend;
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

/* Returns the digit value of c in the given base (2..36), or -1 if c
 * isn't a digit in that base. */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
hb_digit_ಮೌಲ್ಯ (ವರ್ಣ c, ಪೂರ್ಣಾಙ್ಕ base)
{
  ಪೂರ್ಣಾಙ್ಕ d;
  ಯದಿ ('0' <= c && c <= '9')      d = c - '0';
  ಅನ್ಯಥಾ ಯದಿ ('a' <= c && c <= 'z') d = c - 'a' + 10;
  ಅನ್ಯಥಾ ಯದಿ ('A' <= c && c <= 'Z') d = c - 'A' + 10;
  ಅನ್ಯಥಾ ಪ್ರತ್ಯಾಗಮ -1;
  ಪ್ರತ್ಯಾಗಮ d < base ? d : -1;
}

/* Consumes digits in [p, pe) into *value (magnitude).  Sets *overflow
 * if the value would exceed UINT64_MAX.  Sets *any_digits if at least
 * one digit was consumed.  Returns the position past the last digit. */
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *
_hb_scan_digits (ಧ್ರುವ ವರ್ಣ *p, ಧ್ರುವ ವರ್ಣ *pe, ಪೂರ್ಣಾಙ್ಕ base,
                 uint64_t *value, ತಾರ್ಕಿಕ *overflow, ತಾರ್ಕಿಕ *any_digits)
{
  ಧ್ರುವ uint64_t cutoff = UINT64_MAX / (unsigned) base;
  ಧ್ರುವ uint64_t cutlim = UINT64_MAX % (unsigned) base;
  uint64_t v = 0;
  ತಾರ್ಕಿಕ ov = ಅಸತ್ಯ, any = ಅಸತ್ಯ;

  ಪ್ರತಿ (; p < pe; p++)
  {
    ಪೂರ್ಣಾಙ್ಕ d = hb_digit_ಮೌಲ್ಯ (*p, base);
    ಯದಿ (d < 0) ಭಙ್ಗ;
    any = ಸತ್ಯ;
    ಯದಿ (v > cutoff || (v == cutoff && (uint64_t) d > cutlim))
      ov = ಸತ್ಯ;
    ಅನ್ಯಥಾ
      v = v * (unsigned) base + (uint64_t) d;
  }

  *value = v;
  *overflow = ov;
  *any_digits = any;
  ಪ್ರತ್ಯಾಗಮ p;
}

template<typename T, typename Postprocess>
ಸ್ಥಿರ ತಾರ್ಕಿಕ
parse_ಸಂಖ್ಯೆ (ಧ್ರುವ ವರ್ಣ **pp, ಧ್ರುವ ವರ್ಣ *end, T *pv,
               ತಾರ್ಕಿಕ whole_buffer, ಪೂರ್ಣಾಙ್ಕ base, Postprocess post)
{
  ಧ್ರುವ ವರ್ಣ *pend = end;
  ಧ್ರುವ ವರ್ಣ *p = *pp;
  ತಾರ್ಕಿಕ neg = ಅಸತ್ಯ, any = ಅಸತ್ಯ, overflow = ಅಸತ್ಯ;

  ಯಾವತ್ (p < pend && ISSPACE (*p)) p++;
  ಯದಿ (p < pend && (*p == '+' || *p == '-')) { neg = (*p == '-'); p++; }

  /* Skip an optional "0x"/"0X" prefix for base 16, but only when a hex
   * digit follows — "0x" alone parses as the digit 0, matching strtoul. */
  ಯದಿ (base == 16 && p + 2 < pend && p[0] == '0' &&
      (p[1] == 'x' || p[1] == 'X') && hb_digit_ಮೌಲ್ಯ (p[2], 16) >= 0)
    p += 2;

  uint64_t v = 0;
  p = _hb_scan_digits (p, pend, base, &v, &overflow, &any);
  ಯದಿ (unlikely (!any || overflow)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  T out;
  ಯದಿ (unlikely (!post (v, neg, &out))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  ಯದಿ (unlikely (whole_buffer && p != pend)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  *pv = out;
  *pp = p;
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ತಾರ್ಕಿಕ
hb_parse_int (ಧ್ರುವ ವರ್ಣ **pp, ಧ್ರುವ ವರ್ಣ *end, ಪೂರ್ಣಾಙ್ಕ *pv, ತಾರ್ಕಿಕ whole_buffer)
{
  ಪ್ರತ್ಯಾಗಮ parse_ಸಂಖ್ಯೆ (pp, end, pv, whole_buffer, 10,
                        [] (uint64_t v, ತಾರ್ಕಿಕ neg, ಪೂರ್ಣಾಙ್ಕ *out) {
    /* INT_MAX = 2^31-1, INT_MIN magnitude = 2^31 */
    uint64_t limit = neg ? (uint64_t) INT_MAX + 1 : (uint64_t) INT_MAX;
    ಯದಿ (unlikely (v > limit)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    *out = (ಪೂರ್ಣಾಙ್ಕ) (neg ? -(int64_t) v : (int64_t) v);
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  });
}

ತಾರ್ಕಿಕ
hb_parse_uint (ಧ್ರುವ ವರ್ಣ **pp, ಧ್ರುವ ವರ್ಣ *end, unsigned *pv,
               ತಾರ್ಕಿಕ whole_buffer, ಪೂರ್ಣಾಙ್ಕ base)
{
  ಪ್ರತ್ಯಾಗಮ parse_ಸಂಖ್ಯೆ (pp, end, pv, whole_buffer, base,
                        [] (uint64_t v, ತಾರ್ಕಿಕ neg, unsigned *out) {
    ಯದಿ (unlikely (v > UINT_MAX)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    /* Negate in unsigned so -1 wraps to UINT_MAX, matching strtoul
     * truncated to unsigned. */
    unsigned u = (unsigned) v;
    *out = neg ? (unsigned) (0u - u) : u;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  });
}

#endif /* HB_NUMBER_CC pacify */
#endif /* HB_NUMBER_CC guard */

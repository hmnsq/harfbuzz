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

#ifndef HB_सङ्ख्या_CC
#define HB_सङ्ख्या_CC
#ifdef HB_सङ्ख्या_CC /* Pacify -Wunused-macros. */

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-number.hh"
#अन्तर्भाव "hb-number-parser.hh"

तार्किक
hb_parse_double (ध्रुव वर्ण **pp, ध्रुव वर्ण *end, द्विगुण *pv, तार्किक whole_buffer)
{
  ध्रुव वर्ण *pend = end;
  द्विगुण value = hb_strtod (*pp, &pend);
  सचे (unlikely (*pp == pend)) पटिनिवत्त मिच्छा;
  सचे (unlikely (whole_buffer && end != pend)) पटिनिवत्त मिच्छा;
  *pv = value;
  *pp = pend;
  पटिनिवत्त सच्च;
}

/* Returns the digit value of c in the given base (2..36), or -1 if c
 * isn't a digit in that base. */
स्थिर पूर्णाङ्क
hb_digit_मानम् (वर्ण c, पूर्णाङ्क base)
{
  पूर्णाङ्क d;
  सचे ('0' <= c && c <= '9')      d = c - '0';
  अञ्ञथा सचे ('a' <= c && c <= 'z') d = c - 'a' + 10;
  अञ्ञथा सचे ('A' <= c && c <= 'Z') d = c - 'A' + 10;
  अञ्ञथा पटिनिवत्त -1;
  पटिनिवत्त d < base ? d : -1;
}

/* Consumes digits in [p, pe) into *value (magnitude).  Sets *overflow
 * if the value would exceed UINT64_MAX.  Sets *any_digits if at least
 * one digit was consumed.  Returns the position past the last digit. */
स्थिर ध्रुव वर्ण *
_hb_scan_digits (ध्रुव वर्ण *p, ध्रुव वर्ण *pe, पूर्णाङ्क base,
                 uint64_t *value, तार्किक *overflow, तार्किक *any_digits)
{
  ध्रुव uint64_t cutoff = UINT64_MAX / (unsigned) base;
  ध्रुव uint64_t cutlim = UINT64_MAX % (unsigned) base;
  uint64_t v = 0;
  तार्किक ov = मिच्छा, any = मिच्छा;

  अत्थाय (; p < pe; p++)
  {
    पूर्णाङ्क d = hb_digit_मानम् (*p, base);
    सचे (d < 0) भङ्ग;
    any = सच्च;
    सचे (v > cutoff || (v == cutoff && (uint64_t) d > cutlim))
      ov = सच्च;
    अञ्ञथा
      v = v * (unsigned) base + (uint64_t) d;
  }

  *value = v;
  *overflow = ov;
  *any_digits = any;
  पटिनिवत्त p;
}

template<typename T, typename Postprocess>
स्थिर तार्किक
parse_सङ्ख्या (ध्रुव वर्ण **pp, ध्रुव वर्ण *end, T *pv,
               तार्किक whole_buffer, पूर्णाङ्क base, Postprocess post)
{
  ध्रुव वर्ण *pend = end;
  ध्रुव वर्ण *p = *pp;
  तार्किक neg = मिच्छा, any = मिच्छा, overflow = मिच्छा;

  याव (p < pend && ISSPACE (*p)) p++;
  सचे (p < pend && (*p == '+' || *p == '-')) { neg = (*p == '-'); p++; }

  /* Skip an optional "0x"/"0X" prefix for base 16, but only when a hex
   * digit follows — "0x" alone parses as the digit 0, matching strtoul. */
  सचे (base == 16 && p + 2 < pend && p[0] == '0' &&
      (p[1] == 'x' || p[1] == 'X') && hb_digit_मानम् (p[2], 16) >= 0)
    p += 2;

  uint64_t v = 0;
  p = _hb_scan_digits (p, pend, base, &v, &overflow, &any);
  सचे (unlikely (!any || overflow)) पटिनिवत्त मिच्छा;

  T out;
  सचे (unlikely (!post (v, neg, &out))) पटिनिवत्त मिच्छा;
  सचे (unlikely (whole_buffer && p != pend)) पटिनिवत्त मिच्छा;

  *pv = out;
  *pp = p;
  पटिनिवत्त सच्च;
}

तार्किक
hb_parse_int (ध्रुव वर्ण **pp, ध्रुव वर्ण *end, पूर्णाङ्क *pv, तार्किक whole_buffer)
{
  पटिनिवत्त parse_सङ्ख्या (pp, end, pv, whole_buffer, 10,
                        [] (uint64_t v, तार्किक neg, पूर्णाङ्क *out) {
    /* INT_MAX = 2^31-1, INT_MIN magnitude = 2^31 */
    uint64_t limit = neg ? (uint64_t) INT_MAX + 1 : (uint64_t) INT_MAX;
    सचे (unlikely (v > limit)) पटिनिवत्त मिच्छा;
    *out = (पूर्णाङ्क) (neg ? -(int64_t) v : (int64_t) v);
    पटिनिवत्त सच्च;
  });
}

तार्किक
hb_parse_uint (ध्रुव वर्ण **pp, ध्रुव वर्ण *end, unsigned *pv,
               तार्किक whole_buffer, पूर्णाङ्क base)
{
  पटिनिवत्त parse_सङ्ख्या (pp, end, pv, whole_buffer, base,
                        [] (uint64_t v, तार्किक neg, unsigned *out) {
    सचे (unlikely (v > UINT_MAX)) पटिनिवत्त मिच्छा;
    /* Negate in unsigned so -1 wraps to UINT_MAX, matching strtoul
     * truncated to unsigned. */
    unsigned u = (unsigned) v;
    *out = neg ? (unsigned) (0u - u) : u;
    पटिनिवत्त सच्च;
  });
}

#endif /* HB_NUMBER_CC pacify */
#endif /* HB_NUMBER_CC guard */

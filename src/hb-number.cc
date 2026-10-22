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
  जइ (unlikely (*pp == pend)) प्रत्यागम मिच्छ;
  जइ (unlikely (whole_buffer && end != pend)) प्रत्यागम मिच्छ;
  *pv = value;
  *pp = pend;
  प्रत्यागम सच्च;
}

/* Returns the digit value of c in the given base (2..36), or -1 if c
 * isn't a digit in that base. */
स्थिर पूर्णाङ्क
hb_digit_मानम् (वर्ण c, पूर्णाङ्क base)
{
  पूर्णाङ्क d;
  जइ ('0' <= c && c <= '9')      d = c - '0';
  अण्णहा जइ ('a' <= c && c <= 'z') d = c - 'a' + 10;
  अण्णहा जइ ('A' <= c && c <= 'Z') d = c - 'A' + 10;
  अण्णहा प्रत्यागम -1;
  प्रत्यागम d < base ? d : -1;
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
  तार्किक ov = मिच्छ, any = मिच्छ;

  प्रति (; p < pe; p++)
  {
    पूर्णाङ्क d = hb_digit_मानम् (*p, base);
    जइ (d < 0) भङ्ग;
    any = सच्च;
    जइ (v > cutoff || (v == cutoff && (uint64_t) d > cutlim))
      ov = सच्च;
    अण्णहा
      v = v * (unsigned) base + (uint64_t) d;
  }

  *value = v;
  *overflow = ov;
  *any_digits = any;
  प्रत्यागम p;
}

template<typename T, typename Postprocess>
स्थिर तार्किक
parse_सङ्ख्या (ध्रुव वर्ण **pp, ध्रुव वर्ण *end, T *pv,
               तार्किक whole_buffer, पूर्णाङ्क base, Postprocess post)
{
  ध्रुव वर्ण *pend = end;
  ध्रुव वर्ण *p = *pp;
  तार्किक neg = मिच्छ, any = मिच्छ, overflow = मिच्छ;

  जाव (p < pend && ISSPACE (*p)) p++;
  जइ (p < pend && (*p == '+' || *p == '-')) { neg = (*p == '-'); p++; }

  /* Skip an optional "0x"/"0X" prefix for base 16, but only when a hex
   * digit follows — "0x" alone parses as the digit 0, matching strtoul. */
  जइ (base == 16 && p + 2 < pend && p[0] == '0' &&
      (p[1] == 'x' || p[1] == 'X') && hb_digit_मानम् (p[2], 16) >= 0)
    p += 2;

  uint64_t v = 0;
  p = _hb_scan_digits (p, pend, base, &v, &overflow, &any);
  जइ (unlikely (!any || overflow)) प्रत्यागम मिच्छ;

  T out;
  जइ (unlikely (!post (v, neg, &out))) प्रत्यागम मिच्छ;
  जइ (unlikely (whole_buffer && p != pend)) प्रत्यागम मिच्छ;

  *pv = out;
  *pp = p;
  प्रत्यागम सच्च;
}

तार्किक
hb_parse_int (ध्रुव वर्ण **pp, ध्रुव वर्ण *end, पूर्णाङ्क *pv, तार्किक whole_buffer)
{
  प्रत्यागम parse_सङ्ख्या (pp, end, pv, whole_buffer, 10,
                        [] (uint64_t v, तार्किक neg, पूर्णाङ्क *out) {
    /* INT_MAX = 2^31-1, INT_MIN magnitude = 2^31 */
    uint64_t limit = neg ? (uint64_t) INT_MAX + 1 : (uint64_t) INT_MAX;
    जइ (unlikely (v > limit)) प्रत्यागम मिच्छ;
    *out = (पूर्णाङ्क) (neg ? -(int64_t) v : (int64_t) v);
    प्रत्यागम सच्च;
  });
}

तार्किक
hb_parse_uint (ध्रुव वर्ण **pp, ध्रुव वर्ण *end, unsigned *pv,
               तार्किक whole_buffer, पूर्णाङ्क base)
{
  प्रत्यागम parse_सङ्ख्या (pp, end, pv, whole_buffer, base,
                        [] (uint64_t v, तार्किक neg, unsigned *out) {
    जइ (unlikely (v > UINT_MAX)) प्रत्यागम मिच्छ;
    /* Negate in unsigned so -1 wraps to UINT_MAX, matching strtoul
     * truncated to unsigned. */
    unsigned u = (unsigned) v;
    *out = neg ? (unsigned) (0u - u) : u;
    प्रत्यागम सच्च;
  });
}

#endif /* HB_NUMBER_CC pacify */
#endif /* HB_NUMBER_CC guard */

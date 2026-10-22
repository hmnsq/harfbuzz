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

#ifndef HB_ସଂଖ୍ୟା_CC
#define HB_ସଂଖ୍ୟା_CC
#ifdef HB_ସଂଖ୍ୟା_CC /* Pacify -Wunused-macros. */

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-number.hh"
#ଅନ୍ତର୍ଭାଵ "hb-number-parser.hh"

ତାର୍କିକ
hb_parse_double (ଅଚଳ ଅକ୍ଷର **pp, ଅଚଳ ଅକ୍ଷର *end, ଦ୍ୱିଗୁଣ *pv, ତାର୍କିକ whole_buffer)
{
  ଅଚଳ ଅକ୍ଷର *pend = end;
  ଦ୍ୱିଗୁଣ value = hb_strtod (*pp, &pend);
  ଯଦି (unlikely (*pp == pend)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
  ଯଦି (unlikely (whole_buffer && end != pend)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
  *pv = value;
  *pp = pend;
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

/* Returns the digit value of c in the given base (2..36), or -1 if c
 * isn't a digit in that base. */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
hb_digit_ମୂଲ୍ୟ (ଅକ୍ଷର c, ପୂର୍ଣ୍ଣସଂଖ୍ୟା base)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା d;
  ଯଦି ('0' <= c && c <= '9')      d = c - '0';
  ଅନ୍ୟଥା ଯଦି ('a' <= c && c <= 'z') d = c - 'a' + 10;
  ଅନ୍ୟଥା ଯଦି ('A' <= c && c <= 'Z') d = c - 'A' + 10;
  ଅନ୍ୟଥା ଫେରନ୍ତୁ -1;
  ଫେରନ୍ତୁ d < base ? d : -1;
}

/* Consumes digits in [p, pe) into *value (magnitude).  Sets *overflow
 * if the value would exceed UINT64_MAX.  Sets *any_digits if at least
 * one digit was consumed.  Returns the position past the last digit. */
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *
_hb_scan_digits (ଅଚଳ ଅକ୍ଷର *p, ଅଚଳ ଅକ୍ଷର *pe, ପୂର୍ଣ୍ଣସଂଖ୍ୟା base,
                 uint64_t *value, ତାର୍କିକ *overflow, ତାର୍କିକ *any_digits)
{
  ଅଚଳ uint64_t cutoff = UINT64_MAX / (unsigned) base;
  ଅଚଳ uint64_t cutlim = UINT64_MAX % (unsigned) base;
  uint64_t v = 0;
  ତାର୍କିକ ov = ମିଥ୍ୟା, any = ମିଥ୍ୟା;

  ପାଇଁ (; p < pe; p++)
  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା d = hb_digit_ମୂଲ୍ୟ (*p, base);
    ଯଦି (d < 0) ଭାଙ୍ଗ;
    any = ସତ୍ୟ;
    ଯଦି (v > cutoff || (v == cutoff && (uint64_t) d > cutlim))
      ov = ସତ୍ୟ;
    ଅନ୍ୟଥା
      v = v * (unsigned) base + (uint64_t) d;
  }

  *value = v;
  *overflow = ov;
  *any_digits = any;
  ଫେରନ୍ତୁ p;
}

template<typename T, typename Postprocess>
ସ୍ଥିର ତାର୍କିକ
parse_ସଂଖ୍ୟା (ଅଚଳ ଅକ୍ଷର **pp, ଅଚଳ ଅକ୍ଷର *end, T *pv,
               ତାର୍କିକ whole_buffer, ପୂର୍ଣ୍ଣସଂଖ୍ୟା base, Postprocess post)
{
  ଅଚଳ ଅକ୍ଷର *pend = end;
  ଅଚଳ ଅକ୍ଷର *p = *pp;
  ତାର୍କିକ neg = ମିଥ୍ୟା, any = ମିଥ୍ୟା, overflow = ମିଥ୍ୟା;

  ଯେତେବେଳେ (p < pend && ISSPACE (*p)) p++;
  ଯଦି (p < pend && (*p == '+' || *p == '-')) { neg = (*p == '-'); p++; }

  /* Skip an optional "0x"/"0X" prefix for base 16, but only when a hex
   * digit follows — "0x" alone parses as the digit 0, matching strtoul. */
  ଯଦି (base == 16 && p + 2 < pend && p[0] == '0' &&
      (p[1] == 'x' || p[1] == 'X') && hb_digit_ମୂଲ୍ୟ (p[2], 16) >= 0)
    p += 2;

  uint64_t v = 0;
  p = _hb_scan_digits (p, pend, base, &v, &overflow, &any);
  ଯଦି (unlikely (!any || overflow)) ଫେରନ୍ତୁ ମିଥ୍ୟା;

  T out;
  ଯଦି (unlikely (!post (v, neg, &out))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
  ଯଦି (unlikely (whole_buffer && p != pend)) ଫେରନ୍ତୁ ମିଥ୍ୟା;

  *pv = out;
  *pp = p;
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

ତାର୍କିକ
hb_parse_int (ଅଚଳ ଅକ୍ଷର **pp, ଅଚଳ ଅକ୍ଷର *end, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *pv, ତାର୍କିକ whole_buffer)
{
  ଫେରନ୍ତୁ parse_ସଂଖ୍ୟା (pp, end, pv, whole_buffer, 10,
                        [] (uint64_t v, ତାର୍କିକ neg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *out) {
    /* INT_MAX = 2^31-1, INT_MIN magnitude = 2^31 */
    uint64_t limit = neg ? (uint64_t) INT_MAX + 1 : (uint64_t) INT_MAX;
    ଯଦି (unlikely (v > limit)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    *out = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (neg ? -(int64_t) v : (int64_t) v);
    ଫେରନ୍ତୁ ସତ୍ୟ;
  });
}

ତାର୍କିକ
hb_parse_uint (ଅଚଳ ଅକ୍ଷର **pp, ଅଚଳ ଅକ୍ଷର *end, unsigned *pv,
               ତାର୍କିକ whole_buffer, ପୂର୍ଣ୍ଣସଂଖ୍ୟା base)
{
  ଫେରନ୍ତୁ parse_ସଂଖ୍ୟା (pp, end, pv, whole_buffer, base,
                        [] (uint64_t v, ତାର୍କିକ neg, unsigned *out) {
    ଯଦି (unlikely (v > UINT_MAX)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    /* Negate in unsigned so -1 wraps to UINT_MAX, matching strtoul
     * truncated to unsigned. */
    unsigned u = (unsigned) v;
    *out = neg ? (unsigned) (0u - u) : u;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  });
}

#endif /* HB_NUMBER_CC pacify */
#endif /* HB_NUMBER_CC guard */

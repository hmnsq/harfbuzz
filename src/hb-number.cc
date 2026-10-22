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

#ifndef HB_সংখ্যা_CC
#define HB_সংখ্যা_CC
#ifdef HB_সংখ্যা_CC /* Pacify -Wunused-macros. */

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-number.hh"
#অন্তর্ভাব "hb-number-parser.hh"

যৌক্তিক
hb_parse_double (ধ্রুবক অক্ষর **pp, ধ্রুবক অক্ষর *end, দ্বিগুণ *pv, যৌক্তিক whole_বাফার)
{
  ধ্রুবক অক্ষর *pend = end;
  দ্বিগুণ value = hb_strtod (*pp, &pend);
  যদি (unlikely (*pp == pend)) ফেরত মিথ্যা;
  যদি (unlikely (whole_বাফার && end != pend)) ফেরত মিথ্যা;
  *pv = value;
  *pp = pend;
  ফেরত সত্য;
}

/* Returns the digit value of c in the given base (2..36), or -1 if c
 * isn't a digit in that base. */
স্থির পূর্ণসংখ্যা
hb_digit_মান (অক্ষর c, পূর্ণসংখ্যা base)
{
  পূর্ণসংখ্যা d;
  যদি ('0' <= c && c <= '9')      d = c - '0';
  অন্যথা যদি ('a' <= c && c <= 'z') d = c - 'a' + 10;
  অন্যথা যদি ('A' <= c && c <= 'Z') d = c - 'A' + 10;
  অন্যথা ফেরত -1;
  ফেরত d < base ? d : -1;
}

/* Consumes digits in [p, pe) into *value (magnitude).  Sets *overflow
 * if the value would exceed UINT64_MAX.  Sets *any_digits if at least
 * one digit was consumed.  Returns the position past the last digit. */
স্থির ধ্রুবক অক্ষর *
_hb_scan_digits (ধ্রুবক অক্ষর *p, ধ্রুবক অক্ষর *pe, পূর্ণসংখ্যা base,
                 uint64_t *value, যৌক্তিক *overflow, যৌক্তিক *any_digits)
{
  ধ্রুবক uint64_t cutoff = UINT64_MAX / (unsigned) base;
  ধ্রুবক uint64_t cutlim = UINT64_MAX % (unsigned) base;
  uint64_t v = 0;
  যৌক্তিক ov = মিথ্যা, any = মিথ্যা;

  জন্য (; p < pe; p++)
  {
    পূর্ণসংখ্যা d = hb_digit_মান (*p, base);
    যদি (d < 0) বিরতি;
    any = সত্য;
    যদি (v > cutoff || (v == cutoff && (uint64_t) d > cutlim))
      ov = সত্য;
    অন্যথা
      v = v * (unsigned) base + (uint64_t) d;
  }

  *value = v;
  *overflow = ov;
  *any_digits = any;
  ফেরত p;
}

template<typename T, typename Postprocess>
স্থির যৌক্তিক
parse_সংখ্যা (ধ্রুবক অক্ষর **pp, ধ্রুবক অক্ষর *end, T *pv,
               যৌক্তিক whole_বাফার, পূর্ণসংখ্যা base, Postprocess post)
{
  ধ্রুবক অক্ষর *pend = end;
  ধ্রুবক অক্ষর *p = *pp;
  যৌক্তিক neg = মিথ্যা, any = মিথ্যা, overflow = মিথ্যা;

  যখন (p < pend && ISSPACE (*p)) p++;
  যদি (p < pend && (*p == '+' || *p == '-')) { neg = (*p == '-'); p++; }

  /* Skip an optional "0x"/"0X" prefix for base 16, but only when a hex
   * digit follows — "0x" alone parses as the digit 0, matching strtoul. */
  যদি (base == 16 && p + 2 < pend && p[0] == '0' &&
      (p[1] == 'x' || p[1] == 'X') && hb_digit_মান (p[2], 16) >= 0)
    p += 2;

  uint64_t v = 0;
  p = _hb_scan_digits (p, pend, base, &v, &overflow, &any);
  যদি (unlikely (!any || overflow)) ফেরত মিথ্যা;

  T out;
  যদি (unlikely (!post (v, neg, &out))) ফেরত মিথ্যা;
  যদি (unlikely (whole_বাফার && p != pend)) ফেরত মিথ্যা;

  *pv = out;
  *pp = p;
  ফেরত সত্য;
}

যৌক্তিক
hb_parse_int (ধ্রুবক অক্ষর **pp, ধ্রুবক অক্ষর *end, পূর্ণসংখ্যা *pv, যৌক্তিক whole_বাফার)
{
  ফেরত parse_সংখ্যা (pp, end, pv, whole_বাফার, 10,
                        [] (uint64_t v, যৌক্তিক neg, পূর্ণসংখ্যা *out) {
    /* INT_MAX = 2^31-1, INT_MIN magnitude = 2^31 */
    uint64_t limit = neg ? (uint64_t) INT_MAX + 1 : (uint64_t) INT_MAX;
    যদি (unlikely (v > limit)) ফেরত মিথ্যা;
    *out = (পূর্ণসংখ্যা) (neg ? -(int64_t) v : (int64_t) v);
    ফেরত সত্য;
  });
}

যৌক্তিক
hb_parse_uint (ধ্রুবক অক্ষর **pp, ধ্রুবক অক্ষর *end, unsigned *pv,
               যৌক্তিক whole_বাফার, পূর্ণসংখ্যা base)
{
  ফেরত parse_সংখ্যা (pp, end, pv, whole_বাফার, base,
                        [] (uint64_t v, যৌক্তিক neg, unsigned *out) {
    যদি (unlikely (v > UINT_MAX)) ফেরত মিথ্যা;
    /* Negate in unsigned so -1 wraps to UINT_MAX, matching strtoul
     * truncated to unsigned. */
    unsigned u = (unsigned) v;
    *out = neg ? (unsigned) (0u - u) : u;
    ফেরত সত্য;
  });
}

#endif /* HB_NUMBER_CC pacify */
#endif /* HB_NUMBER_CC guard */

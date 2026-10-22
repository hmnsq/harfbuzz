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

#నిర్వచితం_కాకపోతే HB_సంఖ్య_CC
#నిర్వచించు HB_సంఖ్య_CC
#నిర్వచితమైతే HB_సంఖ్య_CC /* Pacify -Wunused-macros. */

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-number.hh"
#అన్తర్భావ "hb-number-parser.hh"

తార్కిక
hb_parse_double (స్థిరరాశి అక్షరం **pp, స్థిరరాశి అక్షరం *చివర, ద్విగుణ *pv, తార్కిక సంపూర్ణ_బఫరు)
{
  స్థిరరాశి అక్షరం *pend = చివర;
  ద్విగుణ value = hb_strtod (*pp, &pend);
  ఒకవేళ (unlikely (*pp == pend)) తిరిగి అబద్ధం;
  ఒకవేళ (unlikely (సంపూర్ణ_బఫరు && చివర != pend)) తిరిగి అబద్ధం;
  *pv = value;
  *pp = pend;
  తిరిగి నిజం;
}

/* Returns the digit value of c in the given base (2..36), or -1 if c
 * isn't a digit in that base. */
స్థిర పూర్ణాంకం
hb_digit_విలువ (అక్షరం c, పూర్ణాంకం పునాది)
{
  పూర్ణాంకం d;
  ఒకవేళ ('0' <= c && c <= '9')      d = c - '0';
  లేకపోతే ఒకవేళ ('a' <= c && c <= 'z') d = c - 'a' + 10;
  లేకపోతే ఒకవేళ ('A' <= c && c <= 'Z') d = c - 'A' + 10;
  లేకపోతే తిరిగి -1;
  తిరిగి d < పునాది ? d : -1;
}

/* Consumes digits in [p, pe) into *value (magnitude).  Sets *overflow
 * if the value would exceed UINT64_MAX.  Sets *any_digits if at least
 * one digit was consumed.  Returns the position past the last digit. */
స్థిర స్థిరరాశి అక్షరం *
hb_పరిశీలన_digits (స్థిరరాశి అక్షరం *p, స్థిరరాశి అక్షరం *pe, పూర్ణాంకం పునాది,
                 uint64_t *value, తార్కిక *అధికప్రవాహం, తార్కిక *any_digits)
{
  స్థిరరాశి uint64_t cutoff = UINT64_MAX / (గుర్తులేని) పునాది;
  స్థిరరాశి uint64_t cutlim = UINT64_MAX % (గుర్తులేని) పునాది;
  uint64_t v = 0;
  తార్కిక ov = అబద్ధం, any = అబద్ధం;

  కొరకు (; p < pe; p++)
  {
    పూర్ణాంకం d = hb_digit_విలువ (*p, పునాది);
    ఒకవేళ (d < 0) ఆపు;
    any = నిజం;
    ఒకవేళ (v > cutoff || (v == cutoff && (uint64_t) d > cutlim))
      ov = నిజం;
    లేకపోతే
      v = v * (గుర్తులేని) పునాది + (uint64_t) d;
  }

  *value = v;
  *అధికప్రవాహం = ov;
  *any_digits = any;
  తిరిగి p;
}

మూస<రకం_పేరు T, రకం_పేరు Postprocess>
స్థిర తార్కిక
parse_సంఖ్య (స్థిరరాశి అక్షరం **pp, స్థిరరాశి అక్షరం *చివర, T *pv,
               తార్కిక సంపూర్ణ_బఫరు, పూర్ణాంకం పునాది, Postprocess post)
{
  స్థిరరాశి అక్షరం *pend = చివర;
  స్థిరరాశి అక్షరం *p = *pp;
  తార్కిక neg = అబద్ధం, any = అబద్ధం, అధికప్రవాహం = అబద్ధం;

  అయితే (p < pend && ISSPACE (*p)) p++;
  ఒకవేళ (p < pend && (*p == '+' || *p == '-')) { neg = (*p == '-'); p++; }

  /* Skip an optional "0x"/"0X" prefix for base 16, but only when a hex
   * digit follows — "0x" alone parses as the digit 0, matching strtoul. */
  ఒకవేళ (పునాది == 16 && p + 2 < pend && p[0] == '0' &&
      (p[1] == 'x' || p[1] == 'X') && hb_digit_విలువ (p[2], 16) >= 0)
    p += 2;

  uint64_t v = 0;
  p = hb_పరిశీలన_digits (p, pend, పునాది, &v, &అధికప్రవాహం, &any);
  ఒకవేళ (unlikely (!any || అధికప్రవాహం)) తిరిగి అబద్ధం;

  T బహిర్గతం;
  ఒకవేళ (unlikely (!post (v, neg, &బహిర్గతం))) తిరిగి అబద్ధం;
  ఒకవేళ (unlikely (సంపూర్ణ_బఫరు && p != pend)) తిరిగి అబద్ధం;

  *pv = బహిర్గతం;
  *pp = p;
  తిరిగి నిజం;
}

తార్కిక
hb_parse_int (స్థిరరాశి అక్షరం **pp, స్థిరరాశి అక్షరం *చివర, పూర్ణాంకం *pv, తార్కిక సంపూర్ణ_బఫరు)
{
  తిరిగి parse_సంఖ్య (pp, చివర, pv, సంపూర్ణ_బఫరు, 10,
                        [] (uint64_t v, తార్కిక neg, పూర్ణాంకం *బహిర్గతం) {
    /* INT_MAX = 2^31-1, INT_MIN magnitude = 2^31 */
    uint64_t హద్దు = neg ? (uint64_t) గరిష్ఠ_పూర్ణాంకం + 1 : (uint64_t) గరిష్ఠ_పూర్ణాంకం;
    ఒకవేళ (unlikely (v > హద్దు)) తిరిగి అబద్ధం;
    *బహిర్గతం = (పూర్ణాంకం) (neg ? -(int64_t) v : (int64_t) v);
    తిరిగి నిజం;
  });
}

తార్కిక
hb_parse_uint (స్థిరరాశి అక్షరం **pp, స్థిరరాశి అక్షరం *చివర, గుర్తులేని *pv,
               తార్కిక సంపూర్ణ_బఫరు, పూర్ణాంకం పునాది)
{
  తిరిగి parse_సంఖ్య (pp, చివర, pv, సంపూర్ణ_బఫరు, పునాది,
                        [] (uint64_t v, తార్కిక neg, గుర్తులేని *బహిర్గతం) {
    ఒకవేళ (unlikely (v > UINT_MAX)) తిరిగి అబద్ధం;
    /* Negate in unsigned so -1 wraps to UINT_MAX, matching strtoul
     * truncated to unsigned. */
    గుర్తులేని u = (గుర్తులేని) v;
    *బహిర్గతం = neg ? (గుర్తులేని) (0u - u) : u;
    తిరిగి నిజం;
  });
}

#అయితే_ముగింపు /* HB_NUMBER_CC pacify */
#అయితే_ముగింపు /* HB_NUMBER_CC guard */

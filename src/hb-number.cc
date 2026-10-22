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

#ifndef HB_எண்_CC
#define HB_எண்_CC
#ifdef HB_எண்_CC /* Pacify -Wunused-macros. */

#அந்தர்பாவ "hb.hh"
#அந்தர்பாவ "hb-number.hh"
#அந்தர்பாவ "hb-number-parser.hh"

தருக்க
hb_parse_double (மாறிலி எழுத்து **pp, மாறிலி எழுத்து *end, இரட்டை *pv, தருக்க whole_buffer)
{
  மாறிலி எழுத்து *pend = end;
  இரட்டை value = hb_strtod (*pp, &pend);
  எனில் (unlikely (*pp == pend)) திரும்பு பொய்;
  எனில் (unlikely (whole_buffer && end != pend)) திரும்பு பொய்;
  *pv = value;
  *pp = pend;
  திரும்பு உண்மை;
}

/* Returns the digit value of c in the given base (2..36), or -1 if c
 * isn't a digit in that base. */
நிலையான முழு
hb_digit_மதிப்பு (எழுத்து c, முழு base)
{
  முழு d;
  எனில் ('0' <= c && c <= '9')      d = c - '0';
  இல்லையெனில் எனில் ('a' <= c && c <= 'z') d = c - 'a' + 10;
  இல்லையெனில் எனில் ('A' <= c && c <= 'Z') d = c - 'A' + 10;
  இல்லையெனில் திரும்பு -1;
  திரும்பு d < base ? d : -1;
}

/* Consumes digits in [p, pe) into *value (magnitude).  Sets *overflow
 * if the value would exceed UINT64_MAX.  Sets *any_digits if at least
 * one digit was consumed.  Returns the position past the last digit. */
நிலையான மாறிலி எழுத்து *
_hb_scan_digits (மாறிலி எழுத்து *p, மாறிலி எழுத்து *pe, முழு base,
                 uint64_t *value, தருக்க *overflow, தருக்க *any_digits)
{
  மாறிலி uint64_t cutoff = UINT64_MAX / (unsigned) base;
  மாறிலி uint64_t cutlim = UINT64_MAX % (unsigned) base;
  uint64_t v = 0;
  தருக்க ov = பொய், any = பொய்;

  க்கு (; p < pe; p++)
  {
    முழு d = hb_digit_மதிப்பு (*p, base);
    எனில் (d < 0) நிறுத்து;
    any = உண்மை;
    எனில் (v > cutoff || (v == cutoff && (uint64_t) d > cutlim))
      ov = உண்மை;
    இல்லையெனில்
      v = v * (unsigned) base + (uint64_t) d;
  }

  *value = v;
  *overflow = ov;
  *any_digits = any;
  திரும்பு p;
}

template<typename T, typename Postprocess>
நிலையான தருக்க
parse_எண் (மாறிலி எழுத்து **pp, மாறிலி எழுத்து *end, T *pv,
               தருக்க whole_buffer, முழு base, Postprocess post)
{
  மாறிலி எழுத்து *pend = end;
  மாறிலி எழுத்து *p = *pp;
  தருக்க neg = பொய், any = பொய், overflow = பொய்;

  போது (p < pend && ISSPACE (*p)) p++;
  எனில் (p < pend && (*p == '+' || *p == '-')) { neg = (*p == '-'); p++; }

  /* Skip an optional "0x"/"0X" prefix for base 16, but only when a hex
   * digit follows — "0x" alone parses as the digit 0, matching strtoul. */
  எனில் (base == 16 && p + 2 < pend && p[0] == '0' &&
      (p[1] == 'x' || p[1] == 'X') && hb_digit_மதிப்பு (p[2], 16) >= 0)
    p += 2;

  uint64_t v = 0;
  p = _hb_scan_digits (p, pend, base, &v, &overflow, &any);
  எனில் (unlikely (!any || overflow)) திரும்பு பொய்;

  T out;
  எனில் (unlikely (!post (v, neg, &out))) திரும்பு பொய்;
  எனில் (unlikely (whole_buffer && p != pend)) திரும்பு பொய்;

  *pv = out;
  *pp = p;
  திரும்பு உண்மை;
}

தருக்க
hb_parse_int (மாறிலி எழுத்து **pp, மாறிலி எழுத்து *end, முழு *pv, தருக்க whole_buffer)
{
  திரும்பு parse_எண் (pp, end, pv, whole_buffer, 10,
                        [] (uint64_t v, தருக்க neg, முழு *out) {
    /* INT_MAX = 2^31-1, INT_MIN magnitude = 2^31 */
    uint64_t limit = neg ? (uint64_t) INT_MAX + 1 : (uint64_t) INT_MAX;
    எனில் (unlikely (v > limit)) திரும்பு பொய்;
    *out = (முழு) (neg ? -(int64_t) v : (int64_t) v);
    திரும்பு உண்மை;
  });
}

தருக்க
hb_parse_uint (மாறிலி எழுத்து **pp, மாறிலி எழுத்து *end, unsigned *pv,
               தருக்க whole_buffer, முழு base)
{
  திரும்பு parse_எண் (pp, end, pv, whole_buffer, base,
                        [] (uint64_t v, தருக்க neg, unsigned *out) {
    எனில் (unlikely (v > UINT_MAX)) திரும்பு பொய்;
    /* Negate in unsigned so -1 wraps to UINT_MAX, matching strtoul
     * truncated to unsigned. */
    unsigned u = (unsigned) v;
    *out = neg ? (unsigned) (0u - u) : u;
    திரும்பு உண்மை;
  });
}

#endif /* HB_NUMBER_CC pacify */
#endif /* HB_NUMBER_CC guard */

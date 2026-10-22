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

#ifndef HB_સંખ્યા_CC
#define HB_સંખ્યા_CC
#ifdef HB_સંખ્યા_CC /* Pacify -Wunused-macros. */

#અન્તર્ભાવ "hb.hh"
#અન્તર્ભાવ "hb-number.hh"
#અન્તર્ભાવ "hb-number-parser.hh"

તાર્કિક
hb_parse_double (અચલ અક્ષર **pp, અચલ અક્ષર *end, દ્વિગુણ *pv, તાર્કિક whole_buffer)
{
  અચલ અક્ષર *pend = end;
  દ્વિગુણ value = hb_strtod (*pp, &pend);
  જો (unlikely (*pp == pend)) પાછા ખોટું;
  જો (unlikely (whole_buffer && end != pend)) પાછા ખોટું;
  *pv = value;
  *pp = pend;
  પાછા સાચું;
}

/* Returns the digit value of c in the given base (2..36), or -1 if c
 * isn't a digit in that base. */
સ્થિર પૂર્ણાંક
hb_digit_કિંમત (અક્ષર c, પૂર્ણાંક base)
{
  પૂર્ણાંક d;
  જો ('0' <= c && c <= '9')      d = c - '0';
  નહીંતર જો ('a' <= c && c <= 'z') d = c - 'a' + 10;
  નહીંતર જો ('A' <= c && c <= 'Z') d = c - 'A' + 10;
  નહીંતર પાછા -1;
  પાછા d < base ? d : -1;
}

/* Consumes digits in [p, pe) into *value (magnitude).  Sets *overflow
 * if the value would exceed UINT64_MAX.  Sets *any_digits if at least
 * one digit was consumed.  Returns the position past the last digit. */
સ્થિર અચલ અક્ષર *
_hb_scan_digits (અચલ અક્ષર *p, અચલ અક્ષર *pe, પૂર્ણાંક base,
                 uint64_t *value, તાર્કિક *overflow, તાર્કિક *any_digits)
{
  અચલ uint64_t cutoff = UINT64_MAX / (unsigned) base;
  અચલ uint64_t cutlim = UINT64_MAX % (unsigned) base;
  uint64_t v = 0;
  તાર્કિક ov = ખોટું, any = ખોટું;

  માટે (; p < pe; p++)
  {
    પૂર્ણાંક d = hb_digit_કિંમત (*p, base);
    જો (d < 0) તોડો;
    any = સાચું;
    જો (v > cutoff || (v == cutoff && (uint64_t) d > cutlim))
      ov = સાચું;
    નહીંતર
      v = v * (unsigned) base + (uint64_t) d;
  }

  *value = v;
  *overflow = ov;
  *any_digits = any;
  પાછા p;
}

template<typename T, typename Postprocess>
સ્થિર તાર્કિક
parse_સંખ્યા (અચલ અક્ષર **pp, અચલ અક્ષર *end, T *pv,
               તાર્કિક whole_buffer, પૂર્ણાંક base, Postprocess post)
{
  અચલ અક્ષર *pend = end;
  અચલ અક્ષર *p = *pp;
  તાર્કિક neg = ખોટું, any = ખોટું, overflow = ખોટું;

  જ્યારે (p < pend && ISSPACE (*p)) p++;
  જો (p < pend && (*p == '+' || *p == '-')) { neg = (*p == '-'); p++; }

  /* Skip an optional "0x"/"0X" prefix for base 16, but only when a hex
   * digit follows — "0x" alone parses as the digit 0, matching strtoul. */
  જો (base == 16 && p + 2 < pend && p[0] == '0' &&
      (p[1] == 'x' || p[1] == 'X') && hb_digit_કિંમત (p[2], 16) >= 0)
    p += 2;

  uint64_t v = 0;
  p = _hb_scan_digits (p, pend, base, &v, &overflow, &any);
  જો (unlikely (!any || overflow)) પાછા ખોટું;

  T out;
  જો (unlikely (!post (v, neg, &out))) પાછા ખોટું;
  જો (unlikely (whole_buffer && p != pend)) પાછા ખોટું;

  *pv = out;
  *pp = p;
  પાછા સાચું;
}

તાર્કિક
hb_parse_int (અચલ અક્ષર **pp, અચલ અક્ષર *end, પૂર્ણાંક *pv, તાર્કિક whole_buffer)
{
  પાછા parse_સંખ્યા (pp, end, pv, whole_buffer, 10,
                        [] (uint64_t v, તાર્કિક neg, પૂર્ણાંક *out) {
    /* INT_MAX = 2^31-1, INT_MIN magnitude = 2^31 */
    uint64_t limit = neg ? (uint64_t) INT_MAX + 1 : (uint64_t) INT_MAX;
    જો (unlikely (v > limit)) પાછા ખોટું;
    *out = (પૂર્ણાંક) (neg ? -(int64_t) v : (int64_t) v);
    પાછા સાચું;
  });
}

તાર્કિક
hb_parse_uint (અચલ અક્ષર **pp, અચલ અક્ષર *end, unsigned *pv,
               તાર્કિક whole_buffer, પૂર્ણાંક base)
{
  પાછા parse_સંખ્યા (pp, end, pv, whole_buffer, base,
                        [] (uint64_t v, તાર્કિક neg, unsigned *out) {
    જો (unlikely (v > UINT_MAX)) પાછા ખોટું;
    /* Negate in unsigned so -1 wraps to UINT_MAX, matching strtoul
     * truncated to unsigned. */
    unsigned u = (unsigned) v;
    *out = neg ? (unsigned) (0u - u) : u;
    પાછા સાચું;
  });
}

#endif /* HB_NUMBER_CC pacify */
#endif /* HB_NUMBER_CC guard */

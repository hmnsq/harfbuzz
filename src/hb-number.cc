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

#ifndef HB_NUMBER_CC
#define HB_NUMBER_CC
#ifdef HB_NUMBER_CC /* Pacify -Wunused-macros. */

#include "hb.hh"
#include "hb-number.hh"
#include "hb-number-parser.hh"

bool
hb_parse_double (const var_na **pp, const var_na *end, dviguu_na *pv, bool whole_buffer)
{
  const var_na *pend = end;
  dviguu_na value = hb_strtod (*pp, &pend);
  ᮚᮦᮔ᮪ (unlikely (*pp == pend)) ᮝᮜᮤ false;
  ᮚᮦᮔ᮪ (unlikely (whole_buffer && end != pend)) ᮝᮜᮤ false;
  *pv = value;
  *pp = pend;
  ᮝᮜᮤ true;
}

/* Returns the digit value of c in the given base (2..36), or -1 if c
 * isn't a digit in that base. */
static puur_naa_nka
_hb_digit_value (var_na c, puur_naa_nka base)
{
  puur_naa_nka d;
  ᮚᮦᮔ᮪ ('0' <= c && c <= '9')      d = c - '0';
  ᮜᮤᮚ ᮚᮦᮔ᮪ ('a' <= c && c <= 'z') d = c - 'a' + 10;
  ᮜᮤᮚ ᮚᮦᮔ᮪ ('A' <= c && c <= 'Z') d = c - 'A' + 10;
  ᮜᮤᮚ ᮝᮜᮤ -1;
  ᮝᮜᮤ d < base ? d : -1;
}

/* Consumes digits in [p, pe) into *value (magnitude).  Sets *overflow
 * if the value would exceed UINT64_MAX.  Sets *any_digits if at least
 * one digit was consumed.  Returns the position past the last digit. */
static const var_na *
_hb_scan_digits (const var_na *p, const var_na *pe, puur_naa_nka base,
                 uint64_t *value, bool *overflow, bool *any_digits)
{
  const uint64_t cutoff = UINT64_MAX / (unsigned) base;
  const uint64_t cutlim = UINT64_MAX % (unsigned) base;
  uint64_t v = 0;
  bool ov = false, any = false;

  for (; p < pe; p++)
  {
    puur_naa_nka d = _hb_digit_value (*p, base);
    ᮚᮦᮔ᮪ (d < 0) bha_nga;
    any = true;
    ᮚᮦᮔ᮪ (v > cutoff || (v == cutoff && (uint64_t) d > cutlim))
      ov = true;
    ᮜᮤᮚ
      v = v * (unsigned) base + (uint64_t) d;
  }

  *value = v;
  *overflow = ov;
  *any_digits = any;
  ᮝᮜᮤ p;
}

template<typename T, typename Postprocess>
static bool
_parse_number (const var_na **pp, const var_na *end, T *pv,
               bool whole_buffer, puur_naa_nka base, Postprocess post)
{
  const var_na *pend = end;
  const var_na *p = *pp;
  bool neg = false, any = false, overflow = false;

  ᮞᮨᮓᮨᮜ (p < pend && ISSPACE (*p)) p++;
  ᮚᮦᮔ᮪ (p < pend && (*p == '+' || *p == '-')) { neg = (*p == '-'); p++; }

  /* Skip an optional "0x"/"0X" prefix for base 16, but only when a hex
   * digit follows — "0x" alone parses as the digit 0, matching strtoul. */
  ᮚᮦᮔ᮪ (base == 16 && p + 2 < pend && p[0] == '0' &&
      (p[1] == 'x' || p[1] == 'X') && _hb_digit_value (p[2], 16) >= 0)
    p += 2;

  uint64_t v = 0;
  p = _hb_scan_digits (p, pend, base, &v, &overflow, &any);
  ᮚᮦᮔ᮪ (unlikely (!any || overflow)) ᮝᮜᮤ false;

  T out;
  ᮚᮦᮔ᮪ (unlikely (!post (v, neg, &out))) ᮝᮜᮤ false;
  ᮚᮦᮔ᮪ (unlikely (whole_buffer && p != pend)) ᮝᮜᮤ false;

  *pv = out;
  *pp = p;
  ᮝᮜᮤ true;
}

bool
hb_parse_int (const var_na **pp, const var_na *end, puur_naa_nka *pv, bool whole_buffer)
{
  ᮝᮜᮤ _parse_number (pp, end, pv, whole_buffer, 10,
                        [] (uint64_t v, bool neg, puur_naa_nka *out) {
    /* INT_MAX = 2^31-1, INT_MIN magnitude = 2^31 */
    uint64_t limit = neg ? (uint64_t) INT_MAX + 1 : (uint64_t) INT_MAX;
    ᮚᮦᮔ᮪ (unlikely (v > limit)) ᮝᮜᮤ false;
    *out = (puur_naa_nka) (neg ? -(int64_t) v : (int64_t) v);
    ᮝᮜᮤ true;
  });
}

bool
hb_parse_uint (const var_na **pp, const var_na *end, unsigned *pv,
               bool whole_buffer, puur_naa_nka base)
{
  ᮝᮜᮤ _parse_number (pp, end, pv, whole_buffer, base,
                        [] (uint64_t v, bool neg, unsigned *out) {
    ᮚᮦᮔ᮪ (unlikely (v > UINT_MAX)) ᮝᮜᮤ false;
    /* Negate in unsigned so -1 wraps to UINT_MAX, matching strtoul
     * truncated to unsigned. */
    unsigned u = (unsigned) v;
    *out = neg ? (unsigned) (0u - u) : u;
    ᮝᮜᮤ true;
  });
}

#endif /* HB_NUMBER_CC pacify */
#endif /* HB_NUMBER_CC guard */

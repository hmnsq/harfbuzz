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

#ifndef HB_സംഖ്യ_CC
#define HB_സംഖ്യ_CC
#ifdef HB_സംഖ്യ_CC /* Pacify -Wunused-macros. */

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-number.hh"
#അന്തര്ഭാവ "hb-number-parser.hh"

തർക്കം
hb_parse_double (സ്ഥിരം അക്ഷരം **pp, സ്ഥിരം അക്ഷരം *end, ഇരട്ട *pv, തർക്കം whole_buffer)
{
  സ്ഥിരം അക്ഷരം *pend = end;
  ഇരട്ട value = hb_strtod (*pp, &pend);
  എങ്കിൽ (unlikely (*pp == pend)) തിരികെ അസത്യം;
  എങ്കിൽ (unlikely (whole_buffer && end != pend)) തിരികെ അസത്യം;
  *pv = value;
  *pp = pend;
  തിരികെ സത്യം;
}

/* Returns the digit value of c in the given base (2..36), or -1 if c
 * isn't a digit in that base. */
സ്ഥിതം പൂർണ്ണസംഖ്യ
hb_digit_മൂല്യം (അക്ഷരം c, പൂർണ്ണസംഖ്യ base)
{
  പൂർണ്ണസംഖ്യ d;
  എങ്കിൽ ('0' <= c && c <= '9')      d = c - '0';
  അല്ലെങ്കിൽ എങ്കിൽ ('a' <= c && c <= 'z') d = c - 'a' + 10;
  അല്ലെങ്കിൽ എങ്കിൽ ('A' <= c && c <= 'Z') d = c - 'A' + 10;
  അല്ലെങ്കിൽ തിരികെ -1;
  തിരികെ d < base ? d : -1;
}

/* Consumes digits in [p, pe) into *value (magnitude).  Sets *overflow
 * if the value would exceed UINT64_MAX.  Sets *any_digits if at least
 * one digit was consumed.  Returns the position past the last digit. */
സ്ഥിതം സ്ഥിരം അക്ഷരം *
_hb_scan_digits (സ്ഥിരം അക്ഷരം *p, സ്ഥിരം അക്ഷരം *pe, പൂർണ്ണസംഖ്യ base,
                 uint64_t *value, തർക്കം *overflow, തർക്കം *any_digits)
{
  സ്ഥിരം uint64_t cutoff = UINT64_MAX / (unsigned) base;
  സ്ഥിരം uint64_t cutlim = UINT64_MAX % (unsigned) base;
  uint64_t v = 0;
  തർക്കം ov = അസത്യം, any = അസത്യം;

  വേണ്ടി (; p < pe; p++)
  {
    പൂർണ്ണസംഖ്യ d = hb_digit_മൂല്യം (*p, base);
    എങ്കിൽ (d < 0) നിർത്തുക;
    any = സത്യം;
    എങ്കിൽ (v > cutoff || (v == cutoff && (uint64_t) d > cutlim))
      ov = സത്യം;
    അല്ലെങ്കിൽ
      v = v * (unsigned) base + (uint64_t) d;
  }

  *value = v;
  *overflow = ov;
  *any_digits = any;
  തിരികെ p;
}

template<typename T, typename Postprocess>
സ്ഥിതം തർക്കം
parse_സംഖ്യ (സ്ഥിരം അക്ഷരം **pp, സ്ഥിരം അക്ഷരം *end, T *pv,
               തർക്കം whole_buffer, പൂർണ്ണസംഖ്യ base, Postprocess post)
{
  സ്ഥിരം അക്ഷരം *pend = end;
  സ്ഥിരം അക്ഷരം *p = *pp;
  തർക്കം neg = അസത്യം, any = അസത്യം, overflow = അസത്യം;

  ആകുമ്പോൾ (p < pend && ISSPACE (*p)) p++;
  എങ്കിൽ (p < pend && (*p == '+' || *p == '-')) { neg = (*p == '-'); p++; }

  /* Skip an optional "0x"/"0X" prefix for base 16, but only when a hex
   * digit follows — "0x" alone parses as the digit 0, matching strtoul. */
  എങ്കിൽ (base == 16 && p + 2 < pend && p[0] == '0' &&
      (p[1] == 'x' || p[1] == 'X') && hb_digit_മൂല്യം (p[2], 16) >= 0)
    p += 2;

  uint64_t v = 0;
  p = _hb_scan_digits (p, pend, base, &v, &overflow, &any);
  എങ്കിൽ (unlikely (!any || overflow)) തിരികെ അസത്യം;

  T out;
  എങ്കിൽ (unlikely (!post (v, neg, &out))) തിരികെ അസത്യം;
  എങ്കിൽ (unlikely (whole_buffer && p != pend)) തിരികെ അസത്യം;

  *pv = out;
  *pp = p;
  തിരികെ സത്യം;
}

തർക്കം
hb_parse_int (സ്ഥിരം അക്ഷരം **pp, സ്ഥിരം അക്ഷരം *end, പൂർണ്ണസംഖ്യ *pv, തർക്കം whole_buffer)
{
  തിരികെ parse_സംഖ്യ (pp, end, pv, whole_buffer, 10,
                        [] (uint64_t v, തർക്കം neg, പൂർണ്ണസംഖ്യ *out) {
    /* INT_MAX = 2^31-1, INT_MIN magnitude = 2^31 */
    uint64_t limit = neg ? (uint64_t) INT_MAX + 1 : (uint64_t) INT_MAX;
    എങ്കിൽ (unlikely (v > limit)) തിരികെ അസത്യം;
    *out = (പൂർണ്ണസംഖ്യ) (neg ? -(int64_t) v : (int64_t) v);
    തിരികെ സത്യം;
  });
}

തർക്കം
hb_parse_uint (സ്ഥിരം അക്ഷരം **pp, സ്ഥിരം അക്ഷരം *end, unsigned *pv,
               തർക്കം whole_buffer, പൂർണ്ണസംഖ്യ base)
{
  തിരികെ parse_സംഖ്യ (pp, end, pv, whole_buffer, base,
                        [] (uint64_t v, തർക്കം neg, unsigned *out) {
    എങ്കിൽ (unlikely (v > UINT_MAX)) തിരികെ അസത്യം;
    /* Negate in unsigned so -1 wraps to UINT_MAX, matching strtoul
     * truncated to unsigned. */
    unsigned u = (unsigned) v;
    *out = neg ? (unsigned) (0u - u) : u;
    തിരികെ സത്യം;
  });
}

#endif /* HB_NUMBER_CC pacify */
#endif /* HB_NUMBER_CC guard */

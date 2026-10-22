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

#अगर_अघोषित HB_संख्या_CC
#घोषणा HB_संख्या_CC
#अगर_घोषित HB_संख्या_CC /* Pacify -Wunused-macros. */

#समावेश "hb.hh"
#समावेश "hb-number.hh"
#समावेश "hb-number-parser.hh"

bool
hb_parse_double (स्थिर अक्षर **pp, स्थिर अक्षर *end, द्विगुन *pv, bool whole_बफर)
{
  स्थिर अक्षर *pend = end;
  द्विगुन value = hb_strtod (*pp, &pend);
  अगर (unlikely (*pp == pend)) वापस false;
  अगर (unlikely (whole_बफर && end != pend)) वापस false;
  *pv = value;
  *pp = pend;
  वापस true;
}

/* Returns the digit value of c in the given base (2..36), or -1 if c
 * isn't a digit in that base. */
अटल पूर्णांक
hb_digit_मूल्य (अक्षर c, पूर्णांक base)
{
  पूर्णांक d;
  अगर ('0' <= c && c <= '9')      d = c - '0';
  अन्यथा अगर ('a' <= c && c <= 'z') d = c - 'a' + 10;
  अन्यथा अगर ('A' <= c && c <= 'Z') d = c - 'A' + 10;
  अन्यथा वापस -1;
  वापस d < base ? d : -1;
}

/* Consumes digits in [p, pe) into *value (magnitude).  Sets *overflow
 * if the value would exceed UINT64_MAX.  Sets *any_digits if at least
 * one digit was consumed.  Returns the position past the last digit. */
अटल स्थिर अक्षर *
_hb_scan_digits (स्थिर अक्षर *p, स्थिर अक्षर *pe, पूर्णांक base,
                 uint64_t *value, bool *overflow, bool *any_digits)
{
  स्थिर uint64_t cutoff = UINT64_MAX / (अचिन्हित) base;
  स्थिर uint64_t cutlim = UINT64_MAX % (अचिन्हित) base;
  uint64_t v = 0;
  bool ov = false, any = false;

  क्रम (; p < pe; p++)
  {
    पूर्णांक d = hb_digit_मूल्य (*p, base);
    अगर (d < 0) अवरोध;
    any = true;
    अगर (v > cutoff || (v == cutoff && (uint64_t) d > cutlim))
      ov = true;
    अन्यथा
      v = v * (अचिन्हित) base + (uint64_t) d;
  }

  *value = v;
  *overflow = ov;
  *any_digits = any;
  वापस p;
}

ढाँचा<typename T, typename Postprocess>
अटल bool
parse_संख्या (स्थिर अक्षर **pp, स्थिर अक्षर *end, T *pv,
               bool whole_बफर, पूर्णांक base, Postprocess post)
{
  स्थिर अक्षर *pend = end;
  स्थिर अक्षर *p = *pp;
  bool neg = false, any = false, overflow = false;

  जबतक (p < pend && ISSPACE (*p)) p++;
  अगर (p < pend && (*p == '+' || *p == '-')) { neg = (*p == '-'); p++; }

  /* Skip an optional "0x"/"0X" prefix for base 16, but only when a hex
   * digit follows — "0x" alone parses as the digit 0, matching strtoul. */
  अगर (base == 16 && p + 2 < pend && p[0] == '0' &&
      (p[1] == 'x' || p[1] == 'X') && hb_digit_मूल्य (p[2], 16) >= 0)
    p += 2;

  uint64_t v = 0;
  p = _hb_scan_digits (p, pend, base, &v, &overflow, &any);
  अगर (unlikely (!any || overflow)) वापस false;

  T out;
  अगर (unlikely (!post (v, neg, &out))) वापस false;
  अगर (unlikely (whole_बफर && p != pend)) वापस false;

  *pv = out;
  *pp = p;
  वापस true;
}

bool
hb_parse_int (स्थिर अक्षर **pp, स्थिर अक्षर *end, पूर्णांक *pv, bool whole_बफर)
{
  वापस parse_संख्या (pp, end, pv, whole_बफर, 10,
                        [] (uint64_t v, bool neg, पूर्णांक *out) {
    /* INT_MAX = 2^31-1, INT_MIN magnitude = 2^31 */
    uint64_t limit = neg ? (uint64_t) पूर्णांक_उच्च + 1 : (uint64_t) पूर्णांक_उच्च;
    अगर (unlikely (v > limit)) वापस false;
    *out = (पूर्णांक) (neg ? -(int64_t) v : (int64_t) v);
    वापस true;
  });
}

bool
hb_parse_uint (स्थिर अक्षर **pp, स्थिर अक्षर *end, अचिन्हित *pv,
               bool whole_बफर, पूर्णांक base)
{
  वापस parse_संख्या (pp, end, pv, whole_बफर, base,
                        [] (uint64_t v, bool neg, अचिन्हित *out) {
    अगर (unlikely (v > अच_पूर्णांक_उच्च)) वापस false;
    /* Negate in unsigned so -1 wraps to UINT_MAX, matching strtoul
     * truncated to unsigned. */
    अचिन्हित u = (अचिन्हित) v;
    *out = neg ? (अचिन्हित) (0u - u) : u;
    वापस true;
  });
}

#पूर्ण_अगर /* HB_NUMBER_CC pacify */
#पूर्ण_अगर /* HB_NUMBER_CC guard */

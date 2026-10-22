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

#অন্তর্ভাৱ "hb.hh"
#অন্তর্ভাৱ "hb-number.hh"
#অন্তর্ভাৱ "hb-number-parser.hh"

তার্কিক
hb_parse_double (ধ্রুৱ ৱর্ণ **pp, ধ্রুৱ ৱর্ণ *end, দ্ৱিগুণ *pv, তার্কিক whole_buffer)
{
  ধ্রুৱ ৱর্ণ *pend = end;
  দ্ৱিগুণ value = hb_strtod (*pp, &pend);
  যদি (unlikely (*pp == pend)) ঘূৰাই মিছা;
  যদি (unlikely (whole_buffer && end != pend)) ঘূৰাই মিছা;
  *pv = value;
  *pp = pend;
  ঘূৰাই সঁচা;
}

/* Returns the digit value of c in the given base (2..36), or -1 if c
 * isn't a digit in that base. */
স্থির পূর্ণাঙ্ক
hb_digit_মান (ৱর্ণ c, পূর্ণাঙ্ক base)
{
  পূর্ণাঙ্ক d;
  যদি ('0' <= c && c <= '9')      d = c - '0';
  অন্যথা যদি ('a' <= c && c <= 'z') d = c - 'a' + 10;
  অন্যথা যদি ('A' <= c && c <= 'Z') d = c - 'A' + 10;
  অন্যথা ঘূৰাই -1;
  ঘূৰাই d < base ? d : -1;
}

/* Consumes digits in [p, pe) into *value (magnitude).  Sets *overflow
 * if the value would exceed UINT64_MAX.  Sets *any_digits if at least
 * one digit was consumed.  Returns the position past the last digit. */
স্থির ধ্রুৱ ৱর্ণ *
_hb_scan_digits (ধ্রুৱ ৱর্ণ *p, ধ্রুৱ ৱর্ণ *pe, পূর্ণাঙ্ক base,
                 uint64_t *value, তার্কিক *overflow, তার্কিক *any_digits)
{
  ধ্রুৱ uint64_t cutoff = UINT64_MAX / (unsigned) base;
  ধ্রুৱ uint64_t cutlim = UINT64_MAX % (unsigned) base;
  uint64_t v = 0;
  তার্কিক ov = মিছা, any = মিছা;

  বাবে (; p < pe; p++)
  {
    পূর্ণাঙ্ক d = hb_digit_মান (*p, base);
    যদি (d < 0) ভঙ্গ;
    any = সঁচা;
    যদি (v > cutoff || (v == cutoff && (uint64_t) d > cutlim))
      ov = সঁচা;
    অন্যথা
      v = v * (unsigned) base + (uint64_t) d;
  }

  *value = v;
  *overflow = ov;
  *any_digits = any;
  ঘূৰাই p;
}

template<typename T, typename Postprocess>
স্থির তার্কিক
parse_সংখ্যা (ধ্রুৱ ৱর্ণ **pp, ধ্রুৱ ৱর্ণ *end, T *pv,
               তার্কিক whole_buffer, পূর্ণাঙ্ক base, Postprocess post)
{
  ধ্রুৱ ৱর্ণ *pend = end;
  ধ্রুৱ ৱর্ণ *p = *pp;
  তার্কিক neg = মিছা, any = মিছা, overflow = মিছা;

  যেতিয়ালৈ (p < pend && ISSPACE (*p)) p++;
  যদি (p < pend && (*p == '+' || *p == '-')) { neg = (*p == '-'); p++; }

  /* Skip an optional "0x"/"0X" prefix for base 16, but only when a hex
   * digit follows — "0x" alone parses as the digit 0, matching strtoul. */
  যদি (base == 16 && p + 2 < pend && p[0] == '0' &&
      (p[1] == 'x' || p[1] == 'X') && hb_digit_মান (p[2], 16) >= 0)
    p += 2;

  uint64_t v = 0;
  p = _hb_scan_digits (p, pend, base, &v, &overflow, &any);
  যদি (unlikely (!any || overflow)) ঘূৰাই মিছা;

  T out;
  যদি (unlikely (!post (v, neg, &out))) ঘূৰাই মিছা;
  যদি (unlikely (whole_buffer && p != pend)) ঘূৰাই মিছা;

  *pv = out;
  *pp = p;
  ঘূৰাই সঁচা;
}

তার্কিক
hb_parse_int (ধ্রুৱ ৱর্ণ **pp, ধ্রুৱ ৱর্ণ *end, পূর্ণাঙ্ক *pv, তার্কিক whole_buffer)
{
  ঘূৰাই parse_সংখ্যা (pp, end, pv, whole_buffer, 10,
                        [] (uint64_t v, তার্কিক neg, পূর্ণাঙ্ক *out) {
    /* INT_MAX = 2^31-1, INT_MIN magnitude = 2^31 */
    uint64_t limit = neg ? (uint64_t) INT_MAX + 1 : (uint64_t) INT_MAX;
    যদি (unlikely (v > limit)) ঘূৰাই মিছা;
    *out = (পূর্ণাঙ্ক) (neg ? -(int64_t) v : (int64_t) v);
    ঘূৰাই সঁচা;
  });
}

তার্কিক
hb_parse_uint (ধ্রুৱ ৱর্ণ **pp, ধ্রুৱ ৱর্ণ *end, unsigned *pv,
               তার্কিক whole_buffer, পূর্ণাঙ্ক base)
{
  ঘূৰাই parse_সংখ্যা (pp, end, pv, whole_buffer, base,
                        [] (uint64_t v, তার্কিক neg, unsigned *out) {
    যদি (unlikely (v > UINT_MAX)) ঘূৰাই মিছা;
    /* Negate in unsigned so -1 wraps to UINT_MAX, matching strtoul
     * truncated to unsigned. */
    unsigned u = (unsigned) v;
    *out = neg ? (unsigned) (0u - u) : u;
    ঘূৰাই সঁচা;
  });
}

#endif /* HB_NUMBER_CC pacify */
#endif /* HB_NUMBER_CC guard */

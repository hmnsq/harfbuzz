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

#اگر_تعریف_نشده HB_عدد_CC
#تعریف_کن HB_عدد_CC
#اگر_تعریف_شده HB_عدد_CC /* Pacify -Wunused-macros. */

#ضمن "hb.hh"
#ضمن "hb-number.hh"
#ضمن "hb-number-parser.hh"

منطقي
hb_parse_double (ثابت حرف **pp, ثابت حرف *انتها, مزدوج *pv, منطقي کامل_میانگیر)
{
  ثابت حرف *pend = انتها;
  مزدوج value = hb_strtod (*pp, &pend);
  إذا (unlikely (*pp == pend)) أرجع خطأ;
  إذا (unlikely (کامل_میانگیر && انتها != pend)) أرجع خطأ;
  *pv = value;
  *pp = pend;
  أرجع درست;
}

/* Returns the digit value of c in the given base (2..36), or -1 if c
 * isn't a digit in that base. */
ساكن صحيح
hb_digit_مقدار (حرف c, صحيح پایه)
{
  صحيح d;
  إذا ('0' <= c && c <= '9')      d = c - '0';
  وإلا إذا ('a' <= c && c <= 'z') d = c - 'a' + 10;
  وإلا إذا ('A' <= c && c <= 'Z') d = c - 'A' + 10;
  وإلا أرجع -1;
  أرجع d < پایه ? d : -1;
}

/* Consumes digits in [p, pe) into *value (magnitude).  Sets *overflow
 * if the value would exceed UINT64_MAX.  Sets *any_digits if at least
 * one digit was consumed.  Returns the position past the last digit. */
ساكن ثابت حرف *
hb_پیمایش_digits (ثابت حرف *p, ثابت حرف *pe, صحيح پایه,
                 uint64_t *value, منطقي *سرریز, منطقي *any_digits)
{
  ثابت uint64_t cutoff = UINT64_MAX / (بیعلامت) پایه;
  ثابت uint64_t cutlim = UINT64_MAX % (بیعلامت) پایه;
  uint64_t v = 0;
  منطقي ov = خطأ, any = خطأ;

  لكل (; p < pe; p++)
  {
    صحيح d = hb_digit_مقدار (*p, پایه);
    إذا (d < 0) اكسر;
    any = درست;
    إذا (v > cutoff || (v == cutoff && (uint64_t) d > cutlim))
      ov = درست;
    وإلا
      v = v * (بیعلامت) پایه + (uint64_t) d;
  }

  *value = v;
  *سرریز = ov;
  *any_digits = any;
  أرجع p;
}

قالب<نام_نوع T, نام_نوع Postprocess>
ساكن منطقي
parse_عدد (ثابت حرف **pp, ثابت حرف *انتها, T *pv,
               منطقي کامل_میانگیر, صحيح پایه, Postprocess post)
{
  ثابت حرف *pend = انتها;
  ثابت حرف *p = *pp;
  منطقي neg = خطأ, any = خطأ, سرریز = خطأ;

  طالما (p < pend && ISSPACE (*p)) p++;
  إذا (p < pend && (*p == '+' || *p == '-')) { neg = (*p == '-'); p++; }

  /* Skip an optional "0x"/"0X" prefix for base 16, but only when a hex
   * digit follows — "0x" alone parses as the digit 0, matching strtoul. */
  إذا (پایه == 16 && p + 2 < pend && p[0] == '0' &&
      (p[1] == 'x' || p[1] == 'X') && hb_digit_مقدار (p[2], 16) >= 0)
    p += 2;

  uint64_t v = 0;
  p = hb_پیمایش_digits (p, pend, پایه, &v, &سرریز, &any);
  إذا (unlikely (!any || سرریز)) أرجع خطأ;

  T بیرون;
  إذا (unlikely (!post (v, neg, &بیرون))) أرجع خطأ;
  إذا (unlikely (کامل_میانگیر && p != pend)) أرجع خطأ;

  *pv = بیرون;
  *pp = p;
  أرجع درست;
}

منطقي
hb_parse_int (ثابت حرف **pp, ثابت حرف *انتها, صحيح *pv, منطقي کامل_میانگیر)
{
  أرجع parse_عدد (pp, انتها, pv, کامل_میانگیر, 10,
                        [] (uint64_t v, منطقي neg, صحيح *بیرون) {
    /* INT_MAX = 2^31-1, INT_MIN magnitude = 2^31 */
    uint64_t حد = neg ? (uint64_t) بیشترین_صحیح + 1 : (uint64_t) بیشترین_صحیح;
    إذا (unlikely (v > حد)) أرجع خطأ;
    *بیرون = (صحيح) (neg ? -(int64_t) v : (int64_t) v);
    أرجع درست;
  });
}

منطقي
hb_parse_uint (ثابت حرف **pp, ثابت حرف *انتها, بیعلامت *pv,
               منطقي کامل_میانگیر, صحيح پایه)
{
  أرجع parse_عدد (pp, انتها, pv, کامل_میانگیر, پایه,
                        [] (uint64_t v, منطقي neg, بیعلامت *بیرون) {
    إذا (unlikely (v > UINT_MAX)) أرجع خطأ;
    /* Negate in unsigned so -1 wraps to UINT_MAX, matching strtoul
     * truncated to unsigned. */
    بیعلامت u = (بیعلامت) v;
    *بیرون = neg ? (بیعلامت) (0u - u) : u;
    أرجع درست;
  });
}

#پایان_اگر /* HB_NUMBER_CC pacify */
#پایان_اگر /* HB_NUMBER_CC guard */

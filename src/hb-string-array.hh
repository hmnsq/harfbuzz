/*
 * Copyright © 2017  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#اگر_غیر_تعریف_شدہ HB_STRING_ARRAY_HH
#اگر 0 /* Make checks happy. */
#تعریف_کرو HB_STRING_ARRAY_HH
#اگر_ختم

#شامل "hb.hh"

/* Based on Bruno Haible's code in Appendix B of Ulrich Drepper's dsohowto.pdf:
 * https://software.intel.com/sites/default/files/m/a/1/e/dsohowto.pdf */

#تعریف_کرو HB_STRING_ARRAY_TYPE_NAME	HB_PASTE(HB_STRING_ARRAY_NAME, _msgstr_t)
#تعریف_کرو HB_STRING_ARRAY_POOL_NAME	HB_PASTE(HB_STRING_ARRAY_NAME, _msgstr)
#تعریف_کرو HB_STRING_ARRAY_OFFS_NAME	HB_PASTE(HB_STRING_ARRAY_NAME, _msgidx)
#تعریف_کرو HB_STRING_ARRAY_LENG_NAME	HB_PASTE(HB_STRING_ARRAY_NAME, لمبائی_4)

ساکن ثابت اتحاد HB_STRING_ARRAY_TYPE_NAME {
  ساخت {
/* I like to avoid storing the nul-termination byte since we don't need it,
 * but C++ does not allow that.
 * https://stackoverflow.com/q/28433862
 */
#تعریف_کرو HB_STR(s) حرف HB_PASTE (str, __LINE__)[حجم (s)];
#شامل HB_STRING_ARRAY_LIST
#تعریف_منسوخ HB_STR
  } st;
  حرف str[HB_VAR_ARRAY];
}
HB_STRING_ARRAY_POOL_NAME =
{
  {
#تعریف_کرو HB_STR(s) s,
#شامل HB_STRING_ARRAY_LIST
#تعریف_منسوخ HB_STR
  }
};
ساکن ثابت غیرعلامتی صحیح HB_STRING_ARRAY_OFFS_NAME[] =
{
#تعریف_کرو HB_STR(s) offsetof (اتحاد HB_STRING_ARRAY_TYPE_NAME, st.HB_PASTE(str, __LINE__)),
#شامل HB_STRING_ARRAY_LIST
#تعریف_منسوخ HB_STR
  حجم (HB_STRING_ARRAY_TYPE_NAME)
};

ساکن ثابت غیرعلامتی صحیح HB_STRING_ARRAY_LENG_NAME = ARRAY_لمبائی_CONST (HB_STRING_ARRAY_OFFS_NAME) - 1;

ساکن اندرونی hb_بائٹیں_t
HB_STRING_ARRAY_NAME (غیرعلامتی صحیح i)
{
  أكد (i < ARRAY_لمبائی (HB_STRING_ARRAY_OFFS_NAME) - 1);
  واپس hb_بائٹیں_t (HB_STRING_ARRAY_POOL_NAME.str + HB_STRING_ARRAY_OFFS_NAME[i],
		     HB_STRING_ARRAY_OFFS_NAME[i + 1] - HB_STRING_ARRAY_OFFS_NAME[i] - 1);
}

#تعریف_منسوخ HB_STRING_ARRAY_TYPE_NAME
#تعریف_منسوخ HB_STRING_ARRAY_POOL_NAME
#تعریف_منسوخ HB_STRING_ARRAY_OFFS_NAME
#تعریف_منسوخ HB_STRING_ARRAY_LENG_NAME

#اگر_ختم /* HB_STRING_ARRAY_HH */

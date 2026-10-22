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

#که_نه_تعریف_شوی HB_STRING_ARRAY_HH
#إذا 0 /* Make checks happy. */
#تعریف_کړه HB_STRING_ARRAY_HH
#د_که_پای

#ضمن "hb.hh"

/* Based on Bruno Haible's code in Appendix B of Ulrich Drepper's dsohowto.pdf:
 * https://software.intel.com/sites/default/files/m/a/1/e/dsohowto.pdf */

#تعریف_کړه HB_STRING_ARRAY_TYPE_NAME	HB_PASTE(HB_STRING_ARRAY_NAME, _msgstr_t)
#تعریف_کړه HB_STRING_ARRAY_POOL_NAME	HB_PASTE(HB_STRING_ARRAY_NAME, _msgstr)
#تعریف_کړه HB_STRING_ARRAY_OFFS_NAME	HB_PASTE(HB_STRING_ARRAY_NAME, _msgidx)
#تعریف_کړه HB_STRING_ARRAY_LENG_NAME	HB_PASTE(HB_STRING_ARRAY_NAME, اوږدتیا_4)

ساكن ثابت اتحاد HB_STRING_ARRAY_TYPE_NAME {
  بنية {
/* I like to avoid storing the nul-termination byte since we don't need it,
 * but C++ does not allow that.
 * https://stackoverflow.com/q/28433862
 */
#تعریف_کړه HB_STR(s) حرف HB_PASTE (str, __LINE__)[حجم (s)];
#ضمن HB_STRING_ARRAY_LIST
#تعریف_لغوه HB_STR
  } st;
  حرف str[HB_VAR_ARRAY];
}
HB_STRING_ARRAY_POOL_NAME =
{
  {
#تعریف_کړه HB_STR(s) s,
#ضمن HB_STRING_ARRAY_LIST
#تعریف_لغوه HB_STR
  }
};
ساكن ثابت بینښه صحيح HB_STRING_ARRAY_OFFS_NAME[] =
{
#تعریف_کړه HB_STR(s) offsetof (اتحاد HB_STRING_ARRAY_TYPE_NAME, st.HB_PASTE(str, __LINE__)),
#ضمن HB_STRING_ARRAY_LIST
#تعریف_لغوه HB_STR
  حجم (HB_STRING_ARRAY_TYPE_NAME)
};

ساكن ثابت بینښه صحيح HB_STRING_ARRAY_LENG_NAME = ARRAY_اوږدتیا_CONST (HB_STRING_ARRAY_OFFS_NAME) - 1;

ساكن مضمن hb_بایټونه_t
HB_STRING_ARRAY_NAME (بینښه صحيح i)
{
  أكد (i < ARRAY_اوږدتیا (HB_STRING_ARRAY_OFFS_NAME) - 1);
  أرجع hb_بایټونه_t (HB_STRING_ARRAY_POOL_NAME.str + HB_STRING_ARRAY_OFFS_NAME[i],
		     HB_STRING_ARRAY_OFFS_NAME[i + 1] - HB_STRING_ARRAY_OFFS_NAME[i] - 1);
}

#تعریف_لغوه HB_STRING_ARRAY_TYPE_NAME
#تعریف_لغوه HB_STRING_ARRAY_POOL_NAME
#تعریف_لغوه HB_STRING_ARRAY_OFFS_NAME
#تعریف_لغوه HB_STRING_ARRAY_LENG_NAME

#د_که_پای /* HB_STRING_ARRAY_HH */

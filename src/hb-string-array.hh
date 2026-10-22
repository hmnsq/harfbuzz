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

#جيڪڏهن_وصف_نه_ٿيل HB_STRING_ARRAY_HH
#إذا 0 /* Make checks happy. */
#وصف_ڪريو HB_STRING_ARRAY_HH
#جيڪڏهن_پورو

#ضمن "hb.hh"

/* Based on Bruno Haible's code in Appendix B of Ulrich Drepper's dsohowto.pdf:
 * https://software.intel.com/sites/default/files/m/a/1/e/dsohowto.pdf */

#وصف_ڪريو HB_STRING_ARRAY_TYPE_NAME	HB_PASTE(HB_STRING_ARRAY_NAME, _msgstr_t)
#وصف_ڪريو HB_STRING_ARRAY_POOL_NAME	HB_PASTE(HB_STRING_ARRAY_NAME, _msgstr)
#وصف_ڪريو HB_STRING_ARRAY_OFFS_NAME	HB_PASTE(HB_STRING_ARRAY_NAME, _msgidx)
#وصف_ڪريو HB_STRING_ARRAY_LENG_NAME	HB_PASTE(HB_STRING_ARRAY_NAME, ڊيگهائي_4)

ساكن ثابت اتحاد HB_STRING_ARRAY_TYPE_NAME {
  بنية {
/* I like to avoid storing the nul-termination byte since we don't need it,
 * but C++ does not allow that.
 * https://stackoverflow.com/q/28433862
 */
#وصف_ڪريو HB_STR(s) حرف HB_PASTE (str, __LINE__)[حجم (s)];
#ضمن HB_STRING_ARRAY_LIST
#وصف_رد HB_STR
  } st;
  حرف str[HB_VAR_ARRAY];
}
HB_STRING_ARRAY_POOL_NAME =
{
  {
#وصف_ڪريو HB_STR(s) s,
#ضمن HB_STRING_ARRAY_LIST
#وصف_رد HB_STR
  }
};
ساكن ثابت بنانشان صحيح HB_STRING_ARRAY_OFFS_NAME[] =
{
#وصف_ڪريو HB_STR(s) offsetof (اتحاد HB_STRING_ARRAY_TYPE_NAME, st.HB_PASTE(str, __LINE__)),
#ضمن HB_STRING_ARRAY_LIST
#وصف_رد HB_STR
  حجم (HB_STRING_ARRAY_TYPE_NAME)
};

ساكن ثابت بنانشان صحيح HB_STRING_ARRAY_LENG_NAME = ARRAY_ڊيگهائي_CONST (HB_STRING_ARRAY_OFFS_NAME) - 1;

ساكن مضمن hb_بائيٽون_t
HB_STRING_ARRAY_NAME (بنانشان صحيح i)
{
  أكد (i < ARRAY_ڊيگهائي (HB_STRING_ARRAY_OFFS_NAME) - 1);
  أرجع hb_بائيٽون_t (HB_STRING_ARRAY_POOL_NAME.str + HB_STRING_ARRAY_OFFS_NAME[i],
		     HB_STRING_ARRAY_OFFS_NAME[i + 1] - HB_STRING_ARRAY_OFFS_NAME[i] - 1);
}

#وصف_رد HB_STRING_ARRAY_TYPE_NAME
#وصف_رد HB_STRING_ARRAY_POOL_NAME
#وصف_رد HB_STRING_ARRAY_OFFS_NAME
#وصف_رد HB_STRING_ARRAY_LENG_NAME

#جيڪڏهن_پورو /* HB_STRING_ARRAY_HH */

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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_STRING_ARRAY_HH
#ਜੇ 0 /* Make checks happy. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_STRING_ARRAY_HH
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

/* Based on Bruno Haible's code in Appendix B of Ulrich Drepper's dsohowto.pdf:
 * https://software.intel.com/sites/default/files/m/a/1/e/dsohowto.pdf */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_STRING_ARRAY_TYPE_NAME	HB_PASTE(HB_STRING_ARRAY_NAME, _msgstr_t)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_STRING_ARRAY_POOL_NAME	HB_PASTE(HB_STRING_ARRAY_NAME, _msgstr)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_STRING_ARRAY_OFFS_NAME	HB_PASTE(HB_STRING_ARRAY_NAME, _msgidx)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_STRING_ARRAY_LENG_NAME	HB_PASTE(HB_STRING_ARRAY_NAME, ਲੰਮਾਈ_4)

ਸਥਿਰ ਅਚਲ ਸੰਘ HB_STRING_ARRAY_TYPE_NAME {
  ਬਣਤਰ {
/* I like to avoid storing the nul-termination byte since we don't need it,
 * but C++ does not allow that.
 * https://stackoverflow.com/q/28433862
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_STR(s) ਅੱਖਰ HB_PASTE (str, __LINE__)[ਆਕਾਰ (s)];
#ਅਨ੍ਤਰ੍ਭਾਵ HB_STRING_ARRAY_LIST
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_STR
  } st;
  ਅੱਖਰ str[HB_VAR_ARRAY];
}
HB_STRING_ARRAY_POOL_NAME =
{
  {
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_STR(s) s,
#ਅਨ੍ਤਰ੍ਭਾਵ HB_STRING_ARRAY_LIST
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_STR
  }
};
ਸਥਿਰ ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ HB_STRING_ARRAY_OFFS_NAME[] =
{
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_STR(s) offsetof (ਸੰਘ HB_STRING_ARRAY_TYPE_NAME, st.HB_PASTE(str, __LINE__)),
#ਅਨ੍ਤਰ੍ਭਾਵ HB_STRING_ARRAY_LIST
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_STR
  ਆਕਾਰ (HB_STRING_ARRAY_TYPE_NAME)
};

ਸਥਿਰ ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ HB_STRING_ARRAY_LENG_NAME = ARRAY_ਲੰਮਾਈ_CONST (HB_STRING_ARRAY_OFFS_NAME) - 1;

ਸਥਿਰ ਸਤਰਅੰਦਰ hb_ਬਾਈਟਾਂ_t
HB_STRING_ARRAY_NAME (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i)
{
  ਪ੍ਰਤਿਜ੍ਞਾ (i < ARRAY_ਲੰਮਾਈ (HB_STRING_ARRAY_OFFS_NAME) - 1);
  ਵਾਪਸ hb_ਬਾਈਟਾਂ_t (HB_STRING_ARRAY_POOL_NAME.str + HB_STRING_ARRAY_OFFS_NAME[i],
		     HB_STRING_ARRAY_OFFS_NAME[i + 1] - HB_STRING_ARRAY_OFFS_NAME[i] - 1);
}

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_STRING_ARRAY_TYPE_NAME
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_STRING_ARRAY_POOL_NAME
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_STRING_ARRAY_OFFS_NAME
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_STRING_ARRAY_LENG_NAME

#ਜੇ_ਅੰਤ /* HB_STRING_ARRAY_HH */

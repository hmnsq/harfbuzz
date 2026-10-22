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

#ifndef HB_STRING_ARRAY_HH
#যদি 0 /* Make checks happy. */
#define HB_STRING_ARRAY_HH
#endif

#অন্তর্ভাব "hb.hh"

/* Based on Bruno Haible's code in Appendix B of Ulrich Drepper's dsohowto.pdf:
 * https://software.intel.com/sites/default/files/m/a/1/e/dsohowto.pdf */

#define HB_STRING_ARRAY_TYPE_NAME	HB_PASTE(HB_STRING_ARRAY_NAME, _msgstr_t)
#define HB_STRING_ARRAY_POOL_NAME	HB_PASTE(HB_STRING_ARRAY_NAME, _msgstr)
#define HB_STRING_ARRAY_OFFS_NAME	HB_PASTE(HB_STRING_ARRAY_NAME, _msgidx)
#define HB_STRING_ARRAY_LENG_NAME	HB_PASTE(HB_STRING_ARRAY_NAME, _length)

স্থির ধ্রুব সঙ্ঘ HB_STRING_ARRAY_TYPE_NAME {
  সংরচনা {
/* I like to avoid storing the nul-termination byte since we don't need it,
 * but C++ does not allow that.
 * https://stackoverflow.com/q/28433862
 */
#define HB_STR(s) বর্ণ HB_PASTE (str, __LINE__)[পরিমাণ (s)];
#অন্তর্ভাব HB_STRING_ARRAY_LIST
#undef HB_STR
  } st;
  বর্ণ str[HB_VAR_ARRAY];
}
HB_STRING_ARRAY_POOL_NAME =
{
  {
#define HB_STR(s) s,
#অন্তর্ভাব HB_STRING_ARRAY_LIST
#undef HB_STR
  }
};
স্থির ধ্রুব unsigned পূর্ণাঙ্ক HB_STRING_ARRAY_OFFS_NAME[] =
{
#define HB_STR(s) offsetof (সঙ্ঘ HB_STRING_ARRAY_TYPE_NAME, st.HB_PASTE(str, __LINE__)),
#অন্তর্ভাব HB_STRING_ARRAY_LIST
#undef HB_STR
  পরিমাণ (HB_STRING_ARRAY_TYPE_NAME)
};

স্থির ধ্রুব unsigned পূর্ণাঙ্ক HB_STRING_ARRAY_LENG_NAME = ARRAY_LENGTH_CONST (HB_STRING_ARRAY_OFFS_NAME) - 1;

স্থির অন্তঃস্থ hb_bytes_t
HB_STRING_ARRAY_NAME (unsigned পূর্ণাঙ্ক i)
{
  প্রতিজ্ঞা (i < ARRAY_LENGTH (HB_STRING_ARRAY_OFFS_NAME) - 1);
  প্রত্যাগম hb_bytes_t (HB_STRING_ARRAY_POOL_NAME.str + HB_STRING_ARRAY_OFFS_NAME[i],
		     HB_STRING_ARRAY_OFFS_NAME[i + 1] - HB_STRING_ARRAY_OFFS_NAME[i] - 1);
}

#undef HB_STRING_ARRAY_TYPE_NAME
#undef HB_STRING_ARRAY_POOL_NAME
#undef HB_STRING_ARRAY_OFFS_NAME
#undef HB_STRING_ARRAY_LENG_NAME

#endif /* HB_STRING_ARRAY_HH */

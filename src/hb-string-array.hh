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

#अगर_अघोषित HB_STRING_ARRAY_HH
#अगर 0 /* Make checks happy. */
#घोषणा HB_STRING_ARRAY_HH
#पूर्ण_अगर

#समावेश "hb.hh"

/* Based on Bruno Haible's code in Appendix B of Ulrich Drepper's dsohowto.pdf:
 * https://software.intel.com/sites/default/files/m/a/1/e/dsohowto.pdf */

#घोषणा HB_STRING_ARRAY_TYPE_NAME	HB_PASTE(HB_STRING_ARRAY_NAME, _msgstr_t)
#घोषणा HB_STRING_ARRAY_POOL_NAME	HB_PASTE(HB_STRING_ARRAY_NAME, _msgstr)
#घोषणा HB_STRING_ARRAY_OFFS_NAME	HB_PASTE(HB_STRING_ARRAY_NAME, _msgidx)
#घोषणा HB_STRING_ARRAY_LENG_NAME	HB_PASTE(HB_STRING_ARRAY_NAME, लंबाई_4)

अटल स्थिर जोड़ HB_STRING_ARRAY_TYPE_NAME {
  काष्ठा {
/* I like to avoid storing the nul-termination byte since we don't need it,
 * but C++ does not allow that.
 * https://stackoverflow.com/q/28433862
 */
#घोषणा HB_STR(s) अक्षर HB_PASTE (str, __LINE__)[माप (s)];
#समावेश HB_STRING_ARRAY_LIST
#अघोषित HB_STR
  } st;
  अक्षर str[HB_VAR_ARRAY];
}
HB_STRING_ARRAY_POOL_NAME =
{
  {
#घोषणा HB_STR(s) s,
#समावेश HB_STRING_ARRAY_LIST
#अघोषित HB_STR
  }
};
अटल स्थिर अचिन्हित पूर्णांक HB_STRING_ARRAY_OFFS_NAME[] =
{
#घोषणा HB_STR(s) दुरत्व (जोड़ HB_STRING_ARRAY_TYPE_NAME, st.HB_PASTE(str, __LINE__)),
#समावेश HB_STRING_ARRAY_LIST
#अघोषित HB_STR
  माप (HB_STRING_ARRAY_TYPE_NAME)
};

अटल स्थिर अचिन्हित पूर्णांक HB_STRING_ARRAY_LENG_NAME = ARRAY_लंबाई_CONST (HB_STRING_ARRAY_OFFS_NAME) - 1;

अटल अंतरभूत hb_bytes_t
HB_STRING_ARRAY_NAME (अचिन्हित पूर्णांक i)
{
  निश्चित (i < ARRAY_लंबाई (HB_STRING_ARRAY_OFFS_NAME) - 1);
  वापस hb_bytes_t (HB_STRING_ARRAY_POOL_NAME.str + HB_STRING_ARRAY_OFFS_NAME[i],
		     HB_STRING_ARRAY_OFFS_NAME[i + 1] - HB_STRING_ARRAY_OFFS_NAME[i] - 1);
}

#अघोषित HB_STRING_ARRAY_TYPE_NAME
#अघोषित HB_STRING_ARRAY_POOL_NAME
#अघोषित HB_STRING_ARRAY_OFFS_NAME
#अघोषित HB_STRING_ARRAY_LENG_NAME

#पूर्ण_अगर /* HB_STRING_ARRAY_HH */

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

#నిర్వచితం_కాకపోతే HB_STRING_ARRAY_HH
#ఒకవేళ 0 /* Make checks happy. */
#నిర్వచించు HB_STRING_ARRAY_HH
#అయితే_ముగింపు

#అన్తర్భావ "hb.hh"

/* Based on Bruno Haible's code in Appendix B of Ulrich Drepper's dsohowto.pdf:
 * https://software.intel.com/sites/default/files/m/a/1/e/dsohowto.pdf */

#నిర్వచించు HB_STRING_ARRAY_TYPE_NAME	HB_PASTE(HB_STRING_ARRAY_NAME, _msgstr_t)
#నిర్వచించు HB_STRING_ARRAY_POOL_NAME	HB_PASTE(HB_STRING_ARRAY_NAME, _msgstr)
#నిర్వచించు HB_STRING_ARRAY_OFFS_NAME	HB_PASTE(HB_STRING_ARRAY_NAME, _msgidx)
#నిర్వచించు HB_STRING_ARRAY_LENG_NAME	HB_PASTE(HB_STRING_ARRAY_NAME, నిడివి_4)

స్థిర స్థిరరాశి సంఘం HB_STRING_ARRAY_TYPE_NAME {
  నిర్మాణం {
/* I like to avoid storing the nul-termination byte since we don't need it,
 * but C++ does not allow that.
 * https://stackoverflow.com/q/28433862
 */
#నిర్వచించు HB_STR(s) అక్షరం HB_PASTE (str, __LINE__)[పరిమాణం (s)];
#అన్తర్భావ HB_STRING_ARRAY_LIST
#నిర్వచనం_రద్దు HB_STR
  } st;
  అక్షరం str[HB_VAR_ARRAY];
}
HB_STRING_ARRAY_POOL_NAME =
{
  {
#నిర్వచించు HB_STR(s) s,
#అన్తర్భావ HB_STRING_ARRAY_LIST
#నిర్వచనం_రద్దు HB_STR
  }
};
స్థిర స్థిరరాశి గుర్తులేని పూర్ణాంకం HB_STRING_ARRAY_OFFS_NAME[] =
{
#నిర్వచించు HB_STR(s) offsetof (సంఘం HB_STRING_ARRAY_TYPE_NAME, st.HB_PASTE(str, __LINE__)),
#అన్తర్భావ HB_STRING_ARRAY_LIST
#నిర్వచనం_రద్దు HB_STR
  పరిమాణం (HB_STRING_ARRAY_TYPE_NAME)
};

స్థిర స్థిరరాశి గుర్తులేని పూర్ణాంకం HB_STRING_ARRAY_LENG_NAME = ARRAY_నిడివి_CONST (HB_STRING_ARRAY_OFFS_NAME) - 1;

స్థిర అంతఃవరుస hb_బైట్లు_t
HB_STRING_ARRAY_NAME (గుర్తులేని పూర్ణాంకం i)
{
  ప్రతిజ్ఞా (i < ARRAY_నిడివి (HB_STRING_ARRAY_OFFS_NAME) - 1);
  తిరిగి hb_బైట్లు_t (HB_STRING_ARRAY_POOL_NAME.str + HB_STRING_ARRAY_OFFS_NAME[i],
		     HB_STRING_ARRAY_OFFS_NAME[i + 1] - HB_STRING_ARRAY_OFFS_NAME[i] - 1);
}

#నిర్వచనం_రద్దు HB_STRING_ARRAY_TYPE_NAME
#నిర్వచనం_రద్దు HB_STRING_ARRAY_POOL_NAME
#నిర్వచనం_రద్దు HB_STRING_ARRAY_OFFS_NAME
#నిర్వచనం_రద్దు HB_STRING_ARRAY_LENG_NAME

#అయితే_ముగింపు /* HB_STRING_ARRAY_HH */

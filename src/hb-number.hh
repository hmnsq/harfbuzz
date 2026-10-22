/*
 * Copyright © 2019  Ebrahim Byagowi
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

#اگر_غیر_تعریف_شدہ HB_نمبر_HH
#تعریف_کرو HB_نمبر_HH

HB_اندرونی منطقی
hb_parse_int (ثابت حرف **pp, ثابت حرف *انتہا, صحیح *pv,
	      منطقی پورا_بفر = جھوٹ);

HB_اندرونی منطقی
hb_parse_uint (ثابت حرف **pp, ثابت حرف *انتہا, غیرعلامتی صحیح *pv,
	       منطقی پورا_بفر = جھوٹ, صحیح بنیاد = 10);

HB_اندرونی منطقی
hb_parse_double (ثابت حرف **pp, ثابت حرف *انتہا, دوہرا *pv,
		 منطقی پورا_بفر = جھوٹ);

#اگر_ختم /* HB_NUMBER_HH */

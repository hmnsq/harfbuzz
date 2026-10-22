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

#اگر_تعریف_نشده HB_عدد_HH
#تعریف_کن HB_عدد_HH

HB_داخلی منطقي
hb_parse_int (ثابت حرف **pp, ثابت حرف *انتها, صحيح *pv,
	      منطقي کامل_میانگیر = خطأ);

HB_داخلی منطقي
hb_parse_uint (ثابت حرف **pp, ثابت حرف *انتها, بیعلامت صحيح *pv,
	       منطقي کامل_میانگیر = خطأ, صحيح پایه = 10);

HB_داخلی منطقي
hb_parse_double (ثابت حرف **pp, ثابت حرف *انتها, مزدوج *pv,
		 منطقي کامل_میانگیر = خطأ);

#پایان_اگر /* HB_NUMBER_HH */

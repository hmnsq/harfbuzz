/*
 * Copyright © 2018  Ebrahim Byagowi
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
 */

#اگر_تعریف_نشده HB_AAT_LTAG_جدول_HH
#تعریف_کن HB_AAT_LTAG_جدول_HH

#ضمن "hb-open-type.hh"

/*
 * ltag -- Language Tag
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6ltag.html
 */
#تعریف_کن HB_AAT_برچسب_ltag HB_برچسب('l','t','a','g')


نطاق AAT {

استفاده نطاق OT;


بنية FTStringRange
{
  دوست بنية ltag;

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  (پایه+برچسب_2).sanitize (c, درازی));
  }

  محمي:
  NNOffset16_مقصد<Unsized_Array_از<HBUINT8>>
		برچسب_2;		/* Offset from the start of the table to
				 * the beginning of the string */
  HBUINT16	درازی;		/* String length (in bytes) */
  عام:
  DEFINE_اندازه_STATIC (4);
};

بنية ltag
{
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_AAT_برچسب_ltag;

  hb_language_t get_language (بیعلامت صحيح i) ثابت
  {
    ثابت FTStringRange &range = برچسب_Ranges[i];
    أرجع hb_language_from_string ((ثابت حرف *) (هذا+range.برچسب_2).arrayZ,
				    range.درازی);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  نسخه >= 1 &&
			  برچسب_Ranges.sanitize (c, هذا)));
  }

  محمي:
  HBUINT32	نسخه;	/* Table version; currently 1 */
  HBUINT32	flags;		/* Table flags; currently none defined */
  Array32_از<FTStringRange>
		برچسب_Ranges;	/* Range for each tag's string */
  عام:
  DEFINE_اندازه_ARRAY (12, برچسب_Ranges);
};

} /* namespace AAT */


#پایان_اگر /* HB_AAT_LTAG_TABLE_HH */

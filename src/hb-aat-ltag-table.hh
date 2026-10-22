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

#اگر_غیر_تعریف_شدہ HB_AAT_LTAG_جدول_HH
#تعریف_کرو HB_AAT_LTAG_جدول_HH

#شامل "hb-open-type.hh"

/*
 * ltag -- Language Tag
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6ltag.html
 */
#تعریف_کرو HB_AAT_لیبل_ltag HB_لیبل('l','t','a','g')


نامخلا AAT {

استعمال نامخلا OT;


ساخت FTStringRange
{
  دوست ساخت ltag;

  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  hb_barrier () &&
		  (بنیاد+لیبل_2).sanitize (c, لمبائی));
  }

  محفوظ:
  NNOffset16_مقصد<Unsized_Array_کا<HBUINT8>>
		لیبل_2;		/* Offset from the start of the table to
				 * the beginning of the string */
  HBUINT16	لمبائی;		/* String length (in bytes) */
  عوامی:
  DEFINE_سائز_STATIC (4);
};

ساخت ltag
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_AAT_لیبل_ltag;

  hb_language_t get_language (غیرعلامتی صحیح i) ثابت
  {
    ثابت FTStringRange &range = لیبل_Ranges[i];
    واپس hb_language_from_string ((ثابت حرف *) (یہ+range.لیبل_2).arrayZ,
				    range.لمبائی);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (likely (c->check_struct (یہ) &&
			  hb_barrier () &&
			  نسخہ >= 1 &&
			  لیبل_Ranges.sanitize (c, یہ)));
  }

  محفوظ:
  HBUINT32	نسخہ;	/* Table version; currently 1 */
  HBUINT32	flags;		/* Table flags; currently none defined */
  Array32_کا<FTStringRange>
		لیبل_Ranges;	/* Range for each tag's string */
  عوامی:
  DEFINE_سائز_ARRAY (12, لیبل_Ranges);
};

} /* namespace AAT */


#اگر_ختم /* HB_AAT_LTAG_TABLE_HH */

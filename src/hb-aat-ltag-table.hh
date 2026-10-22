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

#که_نه_تعریف_شوی HB_AAT_LTAG_جدول_HH
#تعریف_کړه HB_AAT_LTAG_جدول_HH

#ضمن "hb-open-type.hh"

/*
 * ltag -- Language Tag
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6ltag.html
 */
#تعریف_کړه HB_AAT_نښان_ltag HB_نښان('l','t','a','g')


نطاق AAT {

کارول نطاق OT;


بنية FTStringRange
{
  ملګری بنية ltag;

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  (بنسټ+نښان_2).sanitize (c, اوږدتیا));
  }

  محمي:
  NNOffset16_منزل<Unsized_Array_د<HBUINT8>>
		نښان_2;		/* Offset from the start of the table to
				 * the beginning of the string */
  HBUINT16	اوږدتیا;		/* String length (in bytes) */
  عام:
  DEFINE_اندازه_STATIC (4);
};

بنية ltag
{
  ساكن constexpr hb_نښان_t جدول_نښان = HB_AAT_نښان_ltag;

  hb_language_t get_language (بینښه صحيح i) ثابت
  {
    ثابت FTStringRange &range = نښان_Ranges[i];
    أرجع hb_language_from_string ((ثابت حرف *) (هذا+range.نښان_2).arrayZ,
				    range.اوږدتیا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  نسخه >= 1 &&
			  نښان_Ranges.sanitize (c, هذا)));
  }

  محمي:
  HBUINT32	نسخه;	/* Table version; currently 1 */
  HBUINT32	flags;		/* Table flags; currently none defined */
  Array32_د<FTStringRange>
		نښان_Ranges;	/* Range for each tag's string */
  عام:
  DEFINE_اندازه_ARRAY (12, نښان_Ranges);
};

} /* namespace AAT */


#د_که_پای /* HB_AAT_LTAG_TABLE_HH */

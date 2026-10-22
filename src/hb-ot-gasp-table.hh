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

#اگر_غیر_تعریف_شدہ HB_OT_GASP_جدول_HH
#تعریف_کرو HB_OT_GASP_جدول_HH

#شامل "hb-open-type.hh"
#شامل "hb-ot-hhea-table.hh"
#شامل "hb-ot-os2-table.hh"
#شامل "hb-ot-var-hvar-table.hh"

/*
 * gasp -- Grid-fitting and Scan-conversion Procedure
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gasp
 */
#تعریف_کرو HB_OT_لیبل_gasp HB_لیبل('g','a','s','p')


نامخلا OT {

ساخت GaspRange
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  عوامی:
  HBUINT16	rangeMaxPPEM;	/* Upper limit of range, in PPEM */
  HBUINT16	rangeGaspBehavior;
				/* Flags describing desired rasterizer behavior. */
  عوامی:
  DEFINE_سائز_STATIC (4);
};

ساخت gasp
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_gasp;

  ثابت GaspRange &get_gasp_range (غیرعلامتی صحیح i) ثابت
  { واپس gaspRanges[i]; }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  gaspRanges.sanitize (c));
  }

  محفوظ:
  HBUINT16	نسخہ;	/* Version number (set to 1) */
  Array16_کا<GaspRange>
		gaspRanges;	/* Number of records to follow
				 * Sorted by ppem */
  عوامی:
  DEFINE_سائز_ARRAY (4, gaspRanges);
};

} /* namespace OT */


#اگر_ختم /* HB_OT_GASP_TABLE_HH */

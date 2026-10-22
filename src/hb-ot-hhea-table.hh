/*
 * Copyright © 2011,2012  Google, Inc.
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

#که_نه_تعریف_شوی HB_OT_HHEA_جدول_HH
#تعریف_کړه HB_OT_HHEA_جدول_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-dual.hh"

/*
 * hhea -- Horizontal Header
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hhea
 * vhea -- Vertical Header
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vhea
 */
#تعریف_کړه HB_OT_نښان_hhea HB_نښان('h','h','e','a')
#تعریف_کړه HB_OT_نښان_HHEA HB_نښان('H','H','E','A')
#تعریف_کړه HB_OT_نښان_vhea HB_نښان('v','h','e','a')
#تعریف_کړه HB_OT_نښان_VHEA HB_نښان('V','H','E','A')


نطاق OT {


کينډۍ <ډول_نوم T, ډول_نوم Types>
بنية _hea
{
  کارول Long_Metric_شمېر = ډول_نوم Types::HBLUINT;

  ساكن constexpr بینښه long_metric_شمېر_max ()
  { أرجع Long_Metric_شمېر::static_اندازه == 4 ? 0xFFFFFFFFu : 0xFFFFu; }

  منطقي has_معلومات () ثابت { أرجع نسخه.major; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  likely (نسخه.major == 1));
  }

  عام:
  Fixed_نسخه<>نسخه;	/* 0x00010000u for version 1.0. */
  FWORD		ascender;	/* Typographic ascent. */
  FWORD		descender;	/* Typographic descent. */
  FWORD		کرښه_Gap;	/* Typographic line gap. */
  UFWORD	advanceMax;	/* Maximum advance width/height value in
				 * metrics table. */
  FWORD		minLeadingBearing;
				/* Minimum left/top sidebearing value in
				 * metrics table. */
  FWORD		minTrailingBearing;
				/* Minimum right/bottom sidebearing value;
				 * calculated as Min(aw - lsb -
				 * (xMax - xMin)) for horizontal. */
  FWORD		maxExtent;	/* horizontal: Max(lsb + (xMax - xMin)),
				 * vertical: minLeadingBearing+(yMax-yMin). */
  HBINT16	caret_میلان_Rise;	/* Used to calculate the slope of the
				 * cursor (rise/run); 1 for vertical caret,
				 * 0 for horizontal.*/
  HBINT16	caret_میلان_پرلپسې;	/* 0 for vertical caret, 1 for horizontal. */
  HBINT16	caret_دننه_واټن;	/* The amount by which a slanted
				 * highlight on a glyph needs
				 * to be shifted to produce the
				 * best appearance. Set to 0 for
				 * non-slanted fonts. */
  HBINT16	reserved1;	/* Set to 0. */
  HBINT16	reserved2;	/* Set to 0. */
  HBINT16	reserved3;	/* Set to 0. */
  HBINT16	reserved4;	/* Set to 0. */
  HBINT16	metric_معلومات_Format;/* 0 for current format. */
  Long_Metric_شمېر
		شمېره_د_Long_Metrics;
				/* Number of LongMetric entries in metric
				 * table. */
  عام:
  DEFINE_اندازه_STATIC (34 + Types::HBLUINT::static_اندازه);
};

بنية hhea : _hea<hhea, Layout::SmallTypes> {
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_hhea;
};
بنية HHEA : _hea<HHEA, Layout::MediumTypes> {
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_HHEA;
};
بنية vhea : _hea<vhea, Layout::SmallTypes> {
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_vhea;
};
بنية VHEA : _hea<VHEA, Layout::MediumTypes> {
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_VHEA;
};

بنية hhea_accelerator_t : hb_dual_جدول_t<hhea, HHEA>
{
  hhea_accelerator_t (hb_face_t *face) :
    hb_dual_جدول_t<hhea, HHEA> (face) {}
};

بنية vhea_accelerator_t : hb_dual_جدول_t<vhea, VHEA>
{
  vhea_accelerator_t (hb_face_t *face) :
    hb_dual_جدول_t<vhea, VHEA> (face) {}
};


} /* namespace OT */


#د_که_پای /* HB_OT_HHEA_TABLE_HH */

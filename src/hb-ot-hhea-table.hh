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

#جے_بنا_تعریف HB_OT_HHEA_جدول_HH
#تعریف_کرو HB_OT_HHEA_جدول_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-dual.hh"

/*
 * hhea -- Horizontal Header
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hhea
 * vhea -- Vertical Header
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vhea
 */
#تعریف_کرو HB_OT_ٹیگ_hhea HB_ٹیگ('h','h','e','a')
#تعریف_کرو HB_OT_ٹیگ_HHEA HB_ٹیگ('H','H','E','A')
#تعریف_کرو HB_OT_ٹیگ_vhea HB_ٹیگ('v','h','e','a')
#تعریف_کرو HB_OT_ٹیگ_VHEA HB_ٹیگ('V','H','E','A')


نطاق OT {


سانچا <قسم_ناں T, قسم_ناں Types>
بنية _hea
{
  ورتوں Long_Metric_گنتی = قسم_ناں Types::HBLUINT;

  ساكن constexpr بنانشانی long_metric_گنتی_max ()
  { أرجع Long_Metric_گنتی::static_ناپ == 4 ? 0xFFFFFFFFu : 0xFFFFu; }

  منطقي has_انکڑے () ثابت { أرجع نسخہ.major; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  likely (نسخہ.major == 1));
  }

  عام:
  Fixed_نسخہ<>نسخہ;	/* 0x00010000u for version 1.0. */
  FWORD		ascender;	/* Typographic ascent. */
  FWORD		descender;	/* Typographic descent. */
  FWORD		سطر_Gap;	/* Typographic line gap. */
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
  HBINT16	caret_ڈھلوان_Rise;	/* Used to calculate the slope of the
				 * cursor (rise/run); 1 for vertical caret,
				 * 0 for horizontal.*/
  HBINT16	caret_ڈھلوان_لگاتار;	/* 0 for vertical caret, 1 for horizontal. */
  HBINT16	caret_وتھ;	/* The amount by which a slanted
				 * highlight on a glyph needs
				 * to be shifted to produce the
				 * best appearance. Set to 0 for
				 * non-slanted fonts. */
  HBINT16	reserved1;	/* Set to 0. */
  HBINT16	reserved2;	/* Set to 0. */
  HBINT16	reserved3;	/* Set to 0. */
  HBINT16	reserved4;	/* Set to 0. */
  HBINT16	metric_انکڑے_Format;/* 0 for current format. */
  Long_Metric_گنتی
		نمبر_دا_Long_Metrics;
				/* Number of LongMetric entries in metric
				 * table. */
  عام:
  DEFINE_ناپ_STATIC (34 + Types::HBLUINT::static_ناپ);
};

بنية hhea : _hea<hhea, Layout::SmallTypes> {
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_hhea;
};
بنية HHEA : _hea<HHEA, Layout::MediumTypes> {
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_HHEA;
};
بنية vhea : _hea<vhea, Layout::SmallTypes> {
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_vhea;
};
بنية VHEA : _hea<VHEA, Layout::MediumTypes> {
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_VHEA;
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


#جے_انت /* HB_OT_HHEA_TABLE_HH */

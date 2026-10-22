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

#اگر_غیر_تعریف_شدہ HB_OT_HHEA_جدول_HH
#تعریف_کرو HB_OT_HHEA_جدول_HH

#شامل "hb-open-type.hh"
#شامل "hb-ot-dual.hh"

/*
 * hhea -- Horizontal Header
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hhea
 * vhea -- Vertical Header
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vhea
 */
#تعریف_کرو HB_OT_لیبل_hhea HB_لیبل('h','h','e','a')
#تعریف_کرو HB_OT_لیبل_HHEA HB_لیبل('H','H','E','A')
#تعریف_کرو HB_OT_لیبل_vhea HB_لیبل('v','h','e','a')
#تعریف_کرو HB_OT_لیبل_VHEA HB_لیبل('V','H','E','A')


نامخلا OT {


سانچہ <قسم_نام T, قسم_نام Types>
ساخت _hea
{
  استعمال Long_Metric_گنتی = قسم_نام Types::HBLUINT;

  ساکن constexpr غیرعلامتی long_metric_گنتی_max ()
  { واپس Long_Metric_گنتی::static_سائز == 4 ? 0xFFFFFFFFu : 0xFFFFu; }

  منطقی has_ڈیٹا () ثابت { واپس نسخہ.major; }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  hb_barrier () &&
		  likely (نسخہ.major == 1));
  }

  عوامی:
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
  HBINT16	caret_ڈھلان_Rise;	/* Used to calculate the slope of the
				 * cursor (rise/run); 1 for vertical caret,
				 * 0 for horizontal.*/
  HBINT16	caret_ڈھلان_تسلسل;	/* 0 for vertical caret, 1 for horizontal. */
  HBINT16	caret_انحراف;	/* The amount by which a slanted
				 * highlight on a glyph needs
				 * to be shifted to produce the
				 * best appearance. Set to 0 for
				 * non-slanted fonts. */
  HBINT16	reserved1;	/* Set to 0. */
  HBINT16	reserved2;	/* Set to 0. */
  HBINT16	reserved3;	/* Set to 0. */
  HBINT16	reserved4;	/* Set to 0. */
  HBINT16	metric_ڈیٹا_Format;/* 0 for current format. */
  Long_Metric_گنتی
		نمبر_کا_Long_Metrics;
				/* Number of LongMetric entries in metric
				 * table. */
  عوامی:
  DEFINE_سائز_STATIC (34 + Types::HBLUINT::static_سائز);
};

ساخت hhea : _hea<hhea, Layout::SmallTypes> {
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_hhea;
};
ساخت HHEA : _hea<HHEA, Layout::MediumTypes> {
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_HHEA;
};
ساخت vhea : _hea<vhea, Layout::SmallTypes> {
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_vhea;
};
ساخت VHEA : _hea<VHEA, Layout::MediumTypes> {
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_VHEA;
};

ساخت hhea_accelerator_t : hb_dual_جدول_t<hhea, HHEA>
{
  hhea_accelerator_t (hb_face_t *face) :
    hb_dual_جدول_t<hhea, HHEA> (face) {}
};

ساخت vhea_accelerator_t : hb_dual_جدول_t<vhea, VHEA>
{
  vhea_accelerator_t (hb_face_t *face) :
    hb_dual_جدول_t<vhea, VHEA> (face) {}
};


} /* namespace OT */


#اگر_ختم /* HB_OT_HHEA_TABLE_HH */

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

#که_نه_تعریف_شوی HB_AAT_LAYOUT_BSLN_جدول_HH
#تعریف_کړه HB_AAT_LAYOUT_BSLN_جدول_HH

#ضمن "hb-aat-layout-common.hh"

/*
 * bsln -- Baseline
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6bsln.html
 */
#تعریف_کړه HB_AAT_نښان_bsln HB_نښان('b','s','l','n')


نطاق AAT {


بنية Baseline_جدول_Format0_Part
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  // Roman, Ideographic centered, Ideographic low, Hanging and Math
  // are the default defined ones, but any other maybe accessed also.
  HBINT16	deltas[32];	/* These are the FUnit distance deltas from
				 * the font's natural baseline to the other
				 * baselines used in the font. */
  عام:
  DEFINE_اندازه_STATIC (64);
};

بنية Baseline_جدول_Format1_Part
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  lookup_جدول.sanitize (c)));
  }

  محمي:
  HBINT16	deltas[32];	/* ditto */
  Lookup<HBUINT16>
		lookup_جدول;	/* Lookup table that maps glyphs to their
				 * baseline values. */
  عام:
  DEFINE_اندازه_MIN (66);
};

بنية Baseline_جدول_Format2_Part
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  HBGlyphID16	stdGlyph;	/* The specific glyph index number in this
				 * font that is used to set the baseline values.
				 * This is the standard glyph.
				 * This glyph must contain a set of control points
				 * (whose numbers are contained in the ctlPoints field)
				 * that are used to determine baseline distances. */
  HBUINT16	ctlPoints[32];	/* Set of control point numbers,
				 * associated with the standard glyph.
				 * A value of 0xFFFF means there is no corresponding
				 * control point in the standard glyph. */
  عام:
  DEFINE_اندازه_STATIC (66);
};

بنية Baseline_جدول_Format3_Part
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) && lookup_جدول.sanitize (c)));
  }

  محمي:
  HBGlyphID16	stdGlyph;	/* ditto */
  HBUINT16	ctlPoints[32];	/* ditto */
  Lookup<HBUINT16>
		lookup_جدول;	/* Lookup table that maps glyphs to their
				 * baseline values. */
  عام:
  DEFINE_اندازه_MIN (68);
};

بنية bsln
{
  ساكن constexpr hb_نښان_t جدول_نښان = HB_AAT_نښان_bsln;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!(c->check_struct (هذا) && defaultBaseline < 32)))
      return_trace (خطأ);
    hb_barrier ();

    بدل (format)
    {
    حالة 0: hb_barrier (); return_trace (parts.format0.sanitize (c));
    حالة 1: hb_barrier (); return_trace (parts.format1.sanitize (c));
    حالة 2: hb_barrier (); return_trace (parts.format2.sanitize (c));
    حالة 3: hb_barrier (); return_trace (parts.format3.sanitize (c));
    افتراضي:return_trace (ریښتیا);
    }
  }

  محمي:
  Fixed_نسخه<>نسخه;	/* Version number of the Baseline table. */
  HBUINT16	format;		/* Format of the baseline table. Only one baseline
				 * format may be selected for the font. */
  HBUINT16	defaultBaseline;/* Default baseline value for all glyphs.
				 * This value can be from 0 through 31. */
  اتحاد {
  // Distance-Based Formats
  Baseline_جدول_Format0_Part	format0;
  Baseline_جدول_Format1_Part	format1;
  // Control Point-based Formats
  Baseline_جدول_Format2_Part	format2;
  Baseline_جدول_Format3_Part	format3;
  } parts;
  عام:
  DEFINE_اندازه_MIN (8);
};

} /* namespace AAT */


#د_که_پای /* HB_AAT_LAYOUT_BSLN_TABLE_HH */

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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_AAT_LAYOUT_BSLN_ਸਾਰਣੀ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_AAT_LAYOUT_BSLN_ਸਾਰਣੀ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-layout-common.hh"

/*
 * bsln -- Baseline
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6bsln.html
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_AAT_ਟੈਗ_bsln HB_ਟੈਗ('b','s','l','n')


ਨਾਮਾਕਾਸ਼ AAT {


ਬਣਤਰ Baseline_ਸਾਰਣੀ_Format0_Part
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  // Roman, Ideographic centered, Ideographic low, Hanging and Math
  // are the default defined ones, but any other maybe accessed also.
  HBINT16	deltas[32];	/* These are the FUnit distance deltas from
				 * the font's natural baseline to the other
				 * baselines used in the font. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (64);
};

ਬਣਤਰ Baseline_ਸਾਰਣੀ_Format1_Part
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (likely (c->check_struct (ਇਹ) &&
			  lookup_ਸਾਰਣੀ.sanitize (c)));
  }

  ਸੁਰੱਖਿਅਤ:
  HBINT16	deltas[32];	/* ditto */
  Lookup<HBUINT16>
		lookup_ਸਾਰਣੀ;	/* Lookup table that maps glyphs to their
				 * baseline values. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (66);
};

ਬਣਤਰ Baseline_ਸਾਰਣੀ_Format2_Part
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
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
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (66);
};

ਬਣਤਰ Baseline_ਸਾਰਣੀ_Format3_Part
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (likely (c->check_struct (ਇਹ) && lookup_ਸਾਰਣੀ.sanitize (c)));
  }

  ਸੁਰੱਖਿਅਤ:
  HBGlyphID16	stdGlyph;	/* ditto */
  HBUINT16	ctlPoints[32];	/* ditto */
  Lookup<HBUINT16>
		lookup_ਸਾਰਣੀ;	/* Lookup table that maps glyphs to their
				 * baseline values. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (68);
};

ਬਣਤਰ bsln
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_AAT_ਟੈਗ_bsln;

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (unlikely (!(c->check_struct (ਇਹ) && defaultBaseline < 32)))
      return_trace (ਝੂਠ);
    hb_barrier ();

    ਚੋਣ (format)
    {
    ਹਾਲਤ 0: hb_barrier (); return_trace (parts.format0.sanitize (c));
    ਹਾਲਤ 1: hb_barrier (); return_trace (parts.format1.sanitize (c));
    ਹਾਲਤ 2: hb_barrier (); return_trace (parts.format2.sanitize (c));
    ਹਾਲਤ 3: hb_barrier (); return_trace (parts.format3.sanitize (c));
    ਮੂਲ:return_trace (ਸੱਚ);
    }
  }

  ਸੁਰੱਖਿਅਤ:
  Fixed_ਸੰਸਕਰਣ<>ਸੰਸਕਰਣ;	/* Version number of the Baseline table. */
  HBUINT16	format;		/* Format of the baseline table. Only one baseline
				 * format may be selected for the font. */
  HBUINT16	defaultBaseline;/* Default baseline value for all glyphs.
				 * This value can be from 0 through 31. */
  ਸੰਘ {
  // Distance-Based Formats
  Baseline_ਸਾਰਣੀ_Format0_Part	format0;
  Baseline_ਸਾਰਣੀ_Format1_Part	format1;
  // Control Point-based Formats
  Baseline_ਸਾਰਣੀ_Format2_Part	format2;
  Baseline_ਸਾਰਣੀ_Format3_Part	format3;
  } parts;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (8);
};

} /* namespace AAT */


#ਜੇ_ਅੰਤ /* HB_AAT_LAYOUT_BSLN_TABLE_HH */

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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_HHEA_ਸਾਰਣੀ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_HHEA_ਸਾਰਣੀ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-dual.hh"

/*
 * hhea -- Horizontal Header
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hhea
 * vhea -- Vertical Header
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vhea
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_hhea HB_ਟੈਗ('h','h','e','a')
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_HHEA HB_ਟੈਗ('H','H','E','A')
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_vhea HB_ਟੈਗ('v','h','e','a')
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_VHEA HB_ਟੈਗ('V','H','E','A')


ਨਾਮਾਕਾਸ਼ OT {


ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ _hea
{
  ਵਰਤੋਂ Long_Metric_ਗਿਣਤੀ = ਕਿਸਮ_ਨਾਂ Types::HBLUINT;

  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ long_metric_ਗਿਣਤੀ_max ()
  { ਵਾਪਸ Long_Metric_ਗਿਣਤੀ::static_ਮਾਪ == 4 ? 0xFFFFFFFFu : 0xFFFFu; }

  ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ ਸੰਸਕਰਣ.major; }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  hb_barrier () &&
		  likely (ਸੰਸਕਰਣ.major == 1));
  }

  ਜਨਤਕ:
  Fixed_ਸੰਸਕਰਣ<>ਸੰਸਕਰਣ;	/* 0x00010000u for version 1.0. */
  FWORD		ascender;	/* Typographic ascent. */
  FWORD		descender;	/* Typographic descent. */
  FWORD		ਸਤਰ_Gap;	/* Typographic line gap. */
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
  HBINT16	caret_ਢਲਾਨ_Rise;	/* Used to calculate the slope of the
				 * cursor (rise/run); 1 for vertical caret,
				 * 0 for horizontal.*/
  HBINT16	caret_ਢਲਾਨ_ਲੜੀਵਾਰ;	/* 0 for vertical caret, 1 for horizontal. */
  HBINT16	caret_ਵਿੱਥ;	/* The amount by which a slanted
				 * highlight on a glyph needs
				 * to be shifted to produce the
				 * best appearance. Set to 0 for
				 * non-slanted fonts. */
  HBINT16	reserved1;	/* Set to 0. */
  HBINT16	reserved2;	/* Set to 0. */
  HBINT16	reserved3;	/* Set to 0. */
  HBINT16	reserved4;	/* Set to 0. */
  HBINT16	metric_ਅੰਕੜੇ_Format;/* 0 for current format. */
  Long_Metric_ਗਿਣਤੀ
		ਸੰਖਿਆ_ਦਾ_Long_Metrics;
				/* Number of LongMetric entries in metric
				 * table. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (34 + Types::HBLUINT::static_ਮਾਪ);
};

ਬਣਤਰ hhea : _hea<hhea, Layout::SmallTypes> {
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_hhea;
};
ਬਣਤਰ HHEA : _hea<HHEA, Layout::MediumTypes> {
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_HHEA;
};
ਬਣਤਰ vhea : _hea<vhea, Layout::SmallTypes> {
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_vhea;
};
ਬਣਤਰ VHEA : _hea<VHEA, Layout::MediumTypes> {
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_VHEA;
};

ਬਣਤਰ hhea_accelerator_t : hb_dual_ਸਾਰਣੀ_t<hhea, HHEA>
{
  hhea_accelerator_t (hb_face_t *face) :
    hb_dual_ਸਾਰਣੀ_t<hhea, HHEA> (face) {}
};

ਬਣਤਰ vhea_accelerator_t : hb_dual_ਸਾਰਣੀ_t<vhea, VHEA>
{
  vhea_accelerator_t (hb_face_t *face) :
    hb_dual_ਸਾਰਣੀ_t<vhea, VHEA> (face) {}
};


} /* namespace OT */


#ਜੇ_ਅੰਤ /* HB_OT_HHEA_TABLE_HH */

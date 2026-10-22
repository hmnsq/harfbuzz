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

#ifndef HB_OT_HHEA_TABLE_HH
#define HB_OT_HHEA_TABLE_HH

#अन्तर्भाव "hb-open-type.hh"
#अन्तर्भाव "hb-ot-dual.hh"

/*
 * hhea -- Horizontal Header
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hhea
 * vhea -- Vertical Header
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vhea
 */
#define HB_OT_TAG_hhea HB_TAG('h','h','e','a')
#define HB_OT_TAG_HHEA HB_TAG('H','H','E','A')
#define HB_OT_TAG_vhea HB_TAG('v','h','e','a')
#define HB_OT_TAG_VHEA HB_TAG('V','H','E','A')


नामाकाश OT {


template <typename T, typename Types>
संरचना _hea
{
  using Long_Metric_गणनम् = typename Types::HBLUINT;

  स्थिर constexpr unsigned long_metric_गणनम्_max ()
  { प्रत्यागम Long_Metric_गणनम्::static_size == 4 ? 0xFFFFFFFFu : 0xFFFFu; }

  तार्किक has_data () ध्रुव { प्रत्यागम version.major; }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  likely (version.major == 1));
  }

  सार्वजनिक:
  FixedVersion<>version;	/* 0x00010000u for version 1.0. */
  FWORD		ascender;	/* Typographic ascent. */
  FWORD		descender;	/* Typographic descent. */
  FWORD		पङ्क्तिः_Gap;	/* Typographic line gap. */
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
  HBINT16	caretSlopeRise;	/* Used to calculate the slope of the
				 * cursor (rise/run); 1 for vertical caret,
				 * 0 for horizontal.*/
  HBINT16	caretSlopeRun;	/* 0 for vertical caret, 1 for horizontal. */
  HBINT16	caretOffset;	/* The amount by which a slanted
				 * highlight on a glyph needs
				 * to be shifted to produce the
				 * best appearance. Set to 0 for
				 * non-slanted fonts. */
  HBINT16	reserved1;	/* Set to 0. */
  HBINT16	reserved2;	/* Set to 0. */
  HBINT16	reserved3;	/* Set to 0. */
  HBINT16	reserved4;	/* Set to 0. */
  HBINT16	metricDataFormat;/* 0 for current format. */
  Long_Metric_गणनम्
		सङ्ख्या_Of_Long_Metrics;
				/* Number of LongMetric entries in metric
				 * table. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (34 + Types::HBLUINT::static_size);
};

संरचना hhea : _hea<hhea, Layout::SmallTypes> {
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_hhea;
};
संरचना HHEA : _hea<HHEA, Layout::MediumTypes> {
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_HHEA;
};
संरचना vhea : _hea<vhea, Layout::SmallTypes> {
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_vhea;
};
संरचना VHEA : _hea<VHEA, Layout::MediumTypes> {
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_VHEA;
};

संरचना hhea_accelerator_t : hb_dual_table_t<hhea, HHEA>
{
  hhea_accelerator_t (hb_face_t *face) :
    hb_dual_table_t<hhea, HHEA> (face) {}
};

संरचना vhea_accelerator_t : hb_dual_table_t<vhea, VHEA>
{
  vhea_accelerator_t (hb_face_t *face) :
    hb_dual_table_t<vhea, VHEA> (face) {}
};


} /* namespace OT */


#endif /* HB_OT_HHEA_TABLE_HH */

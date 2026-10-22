/*
 * Copyright © 2010  Red Hat, Inc.
 * Copyright © 2012  Google, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#నిర్వచితం_కాకపోతే HB_OT_తల_పట్టిక_HH
#నిర్వచించు HB_OT_తల_పట్టిక_HH

#అన్తర్భావ "hb-open-type.hh"

/*
 * head -- Font Header
 * https://docs.microsoft.com/en-us/typography/opentype/spec/head
 */
#నిర్వచించు HB_OT_ట్యాగ్_తల HB_ట్యాగ్('h','e','a','d')


నామాకాశ OT {


నిర్మాణం తల_2
{
  మిత్ర నిర్మాణం Open_Type_స్థానభ్రంశం_పట్టిక;

  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_తల;

  గుర్తులేని పూర్ణాంకం get_upem () స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం upem = units_చొప్పున_Em;
    /* If no valid head table found, assume 1000, which matches typical Type1 usage. */
    తిరిగి 16 <= upem && upem <= 16384 ? upem : 1000;
  }

  తార్కిక serialize (hb_serialize_context_t *c) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    return_trace ((తార్కిక) c->embed (ఇది));
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    తల_2 *బహిర్గతం = c->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    ఒకవేళ (c->plan->normalized_coords)
    {
      ఒకవేళ (unlikely (!c->serializer->check_కేటాయించు (బహిర్గతం->xMin, c->plan->తల_maxp_info.xMin,
                                                  HB_SERIALIZE_ERROR_INT_అధికప్రవాహం)))
        return_trace (అబద్ధం);
      ఒకవేళ (unlikely (!c->serializer->check_కేటాయించు (బహిర్గతం->xMax, c->plan->తల_maxp_info.xMax,
                                                  HB_SERIALIZE_ERROR_INT_అధికప్రవాహం)))
        return_trace (అబద్ధం);
      ఒకవేళ (unlikely (!c->serializer->check_కేటాయించు (బహిర్గతం->yMin, c->plan->తల_maxp_info.yMin,
                                                  HB_SERIALIZE_ERROR_INT_అధికప్రవాహం)))
        return_trace (అబద్ధం);
      ఒకవేళ (unlikely (!c->serializer->check_కేటాయించు (బహిర్గతం->yMax, c->plan->తల_maxp_info.yMax,
                                                  HB_SERIALIZE_ERROR_INT_అధికప్రవాహం)))
        return_trace (అబద్ధం);
    }
    return_trace (నిజం);
  }

  గణన mac_style_flag_t {
    BOLD	= 1u<<0,
    ITALIC	= 1u<<1,
    UNDERLINE	= 1u<<2,
    OUTLINE	= 1u<<3,
    SHADOW	= 1u<<4,
    CONDENSED	= 1u<<5,
    EXPANDED	= 1u<<6,
  };

  తార్కిక is_bold () స్థిరరాశి      { తిరిగి macStyle & BOLD; }
  తార్కిక is_italic () స్థిరరాశి    { తిరిగి macStyle & ITALIC; }
  తార్కిక is_condensed () స్థిరరాశి { తిరిగి macStyle & CONDENSED; }
  తార్కిక is_expanded () స్థిరరాశి  { తిరిగి macStyle & EXPANDED; }

  గుర్తులేని get_mac_style () స్థిరరాశి { తిరిగి macStyle; }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
		  సంస్కరణ.major == 1 &&
		  magic_సంఖ్య == 0x5F0F3CF5u);
  }

  రక్షిత:
  Fixed_సంస్కరణ<>సంస్కరణ;		/* Version of the head table--currently
					 * 0x00010000u for version 1.0. */
  Fixed_సంస్కరణ<>fontRevision;		/* Set by font manufacturer. */
  HBUINT32	check_కూడిక_Adjustment;	/* To compute: set it to 0, sum the
					 * entire font as HBUINT32, then store
					 * 0xB1B0AFBAu - sum. */
  HBUINT32	magic_సంఖ్య;		/* Set to 0x5F0F3CF5u. */
  బహిరంగ:
  HBUINT16	flags;			/* Bit 0: Baseline for font at y=0;
					 * Bit 1: Left sidebearing point at x=0;
					 * Bit 2: Instructions may depend on point size;
					 * Bit 3: Force ppem to integer values for all
					 *   internal scaler math; may use fractional
					 *   ppem sizes if this bit is clear;
					 * Bit 4: Instructions may alter advance width
					 *   (the advance widths might not scale linearly);
					 * Bits 5-10: These should be set according to
					 *   Apple's specification. However, they are not
					 *   implemented in OpenType.
					 * Bit 5: This bit should be set in fonts that are
					 *   intended to e laid out vertically, and in
					 *   which the glyphs have been drawn such that an
					 *   x-coordinate of 0 corresponds to the desired
					 *   vertical baseline.
					 * Bit 6: This bit must be set to zero.
					 * Bit 7: This bit should be set if the font
					 *   requires layout for correct linguistic
					 *   rendering (e.g. Arabic fonts).
					 * Bit 8: This bit should be set for a GX font
					 *   which has one or more metamorphosis effects
					 *   designated as happening by default.
					 * Bit 9: This bit should be set if the font
					 *   contains any strong right-to-left glyphs.
					 * Bit 10: This bit should be set if the font
					 *   contains Indic-style rearrangement effects.
					 * Bit 11: Font data is 'lossless,' as a result
					 *   of having been compressed and decompressed
					 *   with the Agfa MicroType Express engine.
					 * Bit 12: Font converted (produce compatible metrics)
					 * Bit 13: Font optimized for ClearType™.
					 *   Note, fonts that rely on embedded bitmaps (EBDT)
					 *   for rendering should not be considered optimized
					 *   for ClearType, and therefore should keep this bit
					 *   cleared.
					 * Bit 14: Last Resort font. If set, indicates that
					 * the glyphs encoded in the cmap subtables are simply
					 * generic symbolic representations of code point
					 * ranges and don’t truly represent support for those
					 * code points. If unset, indicates that the glyphs
					 * encoded in the cmap subtables represent proper
					 * support for those code points.
					 * Bit 15: Reserved, set to 0. */
  రక్షిత:
  HBUINT16	units_చొప్పున_Em;		/* Valid range is from 16 to 16384. This value
					 * should be a power of 2 for fonts that have
					 * TrueType outlines. */
  LONGDATETIME	created;		/* Number of seconds since 12:00 midnight,
					   January 1, 1904. 64-bit integer */
  LONGDATETIME	modified;		/* Number of seconds since 12:00 midnight,
					   January 1, 1904. 64-bit integer */
  బహిరంగ:
  HBINT16	xMin;			/* For all glyph bounding boxes. */
  HBINT16	yMin;			/* For all glyph bounding boxes. */
  HBINT16	xMax;			/* For all glyph bounding boxes. */
  HBINT16	yMax;			/* For all glyph bounding boxes. */
  రక్షిత:
  HBUINT16	macStyle;		/* Bit 0: Bold (if set to 1);
					 * Bit 1: Italic (if set to 1)
					 * Bit 2: Underline (if set to 1)
					 * Bit 3: Outline (if set to 1)
					 * Bit 4: Shadow (if set to 1)
					 * Bit 5: Condensed (if set to 1)
					 * Bit 6: Extended (if set to 1)
					 * Bits 7-15: Reserved (set to 0). */
  HBUINT16	అత్యల్ప_Rec_PPEM;		/* Smallest readable size in pixels. */
  HBINT16	fontDirectionHint;	/* Deprecated (Set to 2).
					 * 0: Fully mixed directional glyphs;
					 * 1: Only strongly left to right;
					 * 2: Like 1 but also contains neutrals;
					 * -1: Only strongly right to left;
					 * -2: Like -1 but also contains neutrals. */
  బహిరంగ:
  HBUINT16	సూచీ_గమ్యం_Loc_Format;	/* 0 for short offsets, 1 for long. */
  HBUINT16	glyph_దత్తాంశం_Format;	/* 0 for current format. */

  DEFINE_కొలత_STATIC (54);
};


} /* namespace OT */


#అయితే_ముగింపు /* HB_OT_HEAD_TABLE_HH */

/*
 * Copyright © 2016  Igalia S.L.
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
 * Igalia Author(s): Frédéric Wang
 */

#నిర్వచితం_కాకపోతే HB_OT_MATH_పట్టిక_HH
#నిర్వచించు HB_OT_MATH_పట్టిక_HH

#అన్తర్భావ "hb-open-type.hh"
#అన్తర్భావ "hb-ot-layout-common.hh"
#అన్తర్భావ "hb-ot-layout-gdef-table.hh"
#అన్తర్భావ "hb-ot-math.h"
#అన్తర్భావ "hb-depend-data.hh"

నామాకాశ OT {


నిర్మాణం Math_విలువ_రికార్డు
{
  hb_position_t get_x_విలువ (hb_font_t *font, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  { తిరిగి hb_saturate_కూడు (font->em_scale_x (value), (పునాది+device_పట్టిక).get_x_delta (font, get_var_store (font, పునాది))); }
  hb_position_t get_y_విలువ (hb_font_t *font, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  { తిరిగి hb_saturate_కూడు (font->em_scale_y (value), (పునాది+device_పట్టిక).get_y_delta (font, get_var_store (font, పునాది))); }

  స్థిరరాశి అంశం_Variation_Store &get_var_store (hb_font_t *font, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
#ఒకవేళ !defined(HB_NO_VAR) && !defined(HB_NO_OT_LAYOUT)
    ఒకవేళ ((పునాది+device_పట్టిక).is_variation_device ())
      తిరిగి font->face->పట్టిక.GDEF->పట్టిక->get_var_store ();
#అయితే_ముగింపు
    తిరిగి Null (అంశం_Variation_Store);
  }

  Math_విలువ_రికార్డు* copy (hb_serialize_context_t *c, స్థిరరాశి శూన్య *పునాది,
                         స్థిరరాశి hb_hashmap_t<గుర్తులేని, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> *variation_map) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    స్వయంచాలక *బహిర్గతం = c->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (శూన్యసూచిక);
    hb_pair_t<గుర్తులేని, పూర్ణాంకం> *varidx_delta;
    ఒకవేళ (variation_map->has ((పునాది+device_పట్టిక).get_variation_సూచీ (), &varidx_delta) &&
        !c->check_కేటాయించు (బహిర్గతం->value, (int64_t) value + hb_రెండవ (*varidx_delta),
                          HB_SERIALIZE_ERROR_INT_అధికప్రవాహం))
      return_trace (శూన్యసూచిక);
    బహిర్గతం->device_పట్టిక.serialize_copy (c, device_పట్టిక, పునాది, 0, hb_serialize_context_t::తల, variation_map);

    return_trace (బహిర్గతం);
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  { (పునాది+device_పట్టిక).collect_variation_indices (c); }

  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) && device_పట్టిక.sanitize (c, పునాది));
  }

  రక్షిత:
  HBINT16		value;		/* The X or Y value in design units */
  Offset16_గమ్యం<Device>	device_పట్టిక;	/* Offset to the device table - from the
					 * beginning of parent table.  May be NULL.
					 * Suggested format for device table is 1. */

  బహిరంగ:
  DEFINE_కొలత_STATIC (4);
};

నిర్మాణం MathConstants
{
  మిత్ర నిర్మాణం MATH;

  MathConstants* copy (hb_serialize_context_t *c,
                      స్థిరరాశి hb_hashmap_t<గుర్తులేని, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> *variation_map) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    స్వయంచాలక *బహిర్గతం = c->ప్రారంభం_embed (ఇది);

    HBINT16 *p = c->allocate_కొలత<HBINT16> (HBINT16::static_కొలత * 2);
    ఒకవేళ (unlikely (!p)) return_trace (శూన్యసూచిక);
    hb_memcpy (p, percent_Scale_కిందికి, HBINT16::static_కొలత * 2);

    HBUINT16 *m = c->allocate_కొలత<HBUINT16> (HBUINT16::static_కొలత * 2);
    ఒకవేళ (unlikely (!m)) return_trace (శూన్యసూచిక);
    hb_memcpy (m, min_ఎత్తు, HBUINT16::static_కొలత * 2);

    గుర్తులేని లెక్క = ARRAY_నిడివి (math_విలువ_రికార్డులు);
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
      ఒకవేళ (!c->copy (math_విలువ_రికార్డులు[i], ఇది, variation_map))
        return_trace (శూన్యసూచిక);

    ఒకవేళ (!c->embed (radicalDegreeBottomRaisePercent)) return_trace (శూన్యసూచిక);
    return_trace (బహిర్గతం);
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి స్వయంచాలక &రికార్డు : math_విలువ_రికార్డులు)
      రికార్డు.collect_variation_indices (c, ఇది);
  }

  తార్కిక sanitize_math_విలువ_రికార్డులు (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);

    గుర్తులేని పూర్ణాంకం లెక్క = ARRAY_నిడివి (math_విలువ_రికార్డులు);
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      ఒకవేళ (!math_విలువ_రికార్డులు[i].sanitize (c, ఇది))
	return_trace (అబద్ధం);

    return_trace (నిజం);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) && sanitize_math_విలువ_రికార్డులు (c));
  }

  hb_position_t get_విలువ (hb_ot_math_constant_t constant,
			   hb_font_t *font) స్థిరరాశి
  {
    ఎంపిక (constant) {

    స్థితి HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_కిందికి:
    స్థితి HB_OT_MATH_CONSTANT_SCRIPT_SCRIPT_PERCENT_SCALE_కిందికి:
      తిరిగి percent_Scale_కిందికి[constant - HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_కిందికి];

    స్థితి HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_ఎత్తు:
    స్థితి HB_OT_MATH_CONSTANT_DISPLAY_OPERATOR_MIN_ఎత్తు:
      తిరిగి font->em_scale_y (min_ఎత్తు[constant - HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_ఎత్తు]);

    స్థితి HB_OT_MATH_CONSTANT_RADICAL_KERN_AFTER_DEGREE:
    స్థితి HB_OT_MATH_CONSTANT_RADICAL_KERN_BEFORE_DEGREE:
    స్థితి HB_OT_MATH_CONSTANT_SKEWED_FRACTION_HORIZONTAL_GAP:
    స్థితి HB_OT_MATH_CONSTANT_SPACE_AFTER_SCRIPT:
      తిరిగి math_విలువ_రికార్డులు[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_x_విలువ (font, ఇది);

    స్థితి HB_OT_MATH_CONSTANT_ACCENT_పునాది_ఎత్తు:
    స్థితి HB_OT_MATH_CONSTANT_AXIS_ఎత్తు:
    స్థితి HB_OT_MATH_CONSTANT_FLATTENED_ACCENT_పునాది_ఎత్తు:
    స్థితి HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_DISPLAY_STYLE_జరుపు_కిందికి:
    స్థితి HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_GAP_MIN:
    స్థితి HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_జరుపు_కిందికి:
    స్థితి HB_OT_MATH_CONSTANT_FRACTION_DENOM_DISPLAY_STYLE_GAP_MIN:
    స్థితి HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_DISPLAY_STYLE_జరుపు_పైకి:
    స్థితి HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_GAP_MIN:
    స్థితి HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_జరుపు_పైకి:
    స్థితి HB_OT_MATH_CONSTANT_FRACTION_అంకె_DISPLAY_STYLE_GAP_MIN:
    స్థితి HB_OT_MATH_CONSTANT_FRACTION_నియమం_THICKNESS:
    స్థితి HB_OT_MATH_CONSTANT_క్రింది_హద్దు_BASELINE_DROP_MIN:
    స్థితి HB_OT_MATH_CONSTANT_క్రింది_హద్దు_GAP_MIN:
    స్థితి HB_OT_MATH_CONSTANT_MATH_LEADING:
    స్థితి HB_OT_MATH_CONSTANT_OVERBAR_EXTRA_ASCENDER:
    స్థితి HB_OT_MATH_CONSTANT_OVERBAR_నియమం_THICKNESS:
    స్థితి HB_OT_MATH_CONSTANT_OVERBAR_VERTICAL_GAP:
    స్థితి HB_OT_MATH_CONSTANT_RADICAL_DISPLAY_STYLE_VERTICAL_GAP:
    స్థితి HB_OT_MATH_CONSTANT_RADICAL_EXTRA_ASCENDER:
    స్థితి HB_OT_MATH_CONSTANT_RADICAL_నియమం_THICKNESS:
    స్థితి HB_OT_MATH_CONSTANT_RADICAL_VERTICAL_GAP:
    స్థితి HB_OT_MATH_CONSTANT_SKEWED_FRACTION_VERTICAL_GAP:
    స్థితి HB_OT_MATH_CONSTANT_దొంతర_BOTTOM_DISPLAY_STYLE_జరుపు_కిందికి:
    స్థితి HB_OT_MATH_CONSTANT_దొంతర_BOTTOM_జరుపు_కిందికి:
    స్థితి HB_OT_MATH_CONSTANT_దొంతర_DISPLAY_STYLE_GAP_MIN:
    స్థితి HB_OT_MATH_CONSTANT_దొంతర_GAP_MIN:
    స్థితి HB_OT_MATH_CONSTANT_దొంతర_శిఖరం_DISPLAY_STYLE_జరుపు_పైకి:
    స్థితి HB_OT_MATH_CONSTANT_దొంతర_శిఖరం_జరుపు_పైకి:
    స్థితి HB_OT_MATH_CONSTANT_STRETCH_దొంతర_BOTTOM_జరుపు_కిందికి:
    స్థితి HB_OT_MATH_CONSTANT_STRETCH_దొంతర_GAP_ABOVE_MIN:
    స్థితి HB_OT_MATH_CONSTANT_STRETCH_దొంతర_GAP_క్రింద_MIN:
    స్థితి HB_OT_MATH_CONSTANT_STRETCH_దొంతర_శిఖరం_జరుపు_పైకి:
    స్థితి HB_OT_MATH_CONSTANT_SUBSCRIPT_BASELINE_DROP_MIN:
    స్థితి HB_OT_MATH_CONSTANT_SUBSCRIPT_జరుపు_కిందికి:
    స్థితి HB_OT_MATH_CONSTANT_SUBSCRIPT_శిఖరం_MAX:
    స్థితి HB_OT_MATH_CONSTANT_SUB_SUPERSCRIPT_GAP_MIN:
    స్థితి HB_OT_MATH_CONSTANT_SUPERSCRIPT_BASELINE_DROP_MAX:
    స్థితి HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MAX_WITH_SUBSCRIPT:
    స్థితి HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MIN:
    స్థితి HB_OT_MATH_CONSTANT_SUPERSCRIPT_జరుపు_పైకి:
    స్థితి HB_OT_MATH_CONSTANT_SUPERSCRIPT_జరుపు_పైకి_CRAMPED:
    స్థితి HB_OT_MATH_CONSTANT_UNDERBAR_EXTRA_DESCENDER:
    స్థితి HB_OT_MATH_CONSTANT_UNDERBAR_నియమం_THICKNESS:
    స్థితి HB_OT_MATH_CONSTANT_UNDERBAR_VERTICAL_GAP:
    స్థితి HB_OT_MATH_CONSTANT_పై_హద్దు_BASELINE_RISE_MIN:
    స్థితి HB_OT_MATH_CONSTANT_పై_హద్దు_GAP_MIN:
      తిరిగి math_విలువ_రికార్డులు[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_y_విలువ (font, ఇది);

    స్థితి HB_OT_MATH_CONSTANT_RADICAL_DEGREE_BOTTOM_RAISE_PERCENT:
      తిరిగి radicalDegreeBottomRaisePercent;

    అప్రమేయ:
      తిరిగి 0;
    }
  }

  రక్షిత:
  HBINT16 percent_Scale_కిందికి[2];
  HBUINT16 min_ఎత్తు[2];
  Math_విలువ_రికార్డు math_విలువ_రికార్డులు[51];
  HBINT16 radicalDegreeBottomRaisePercent;

  బహిరంగ:
  DEFINE_కొలత_STATIC (214);
};

నిర్మాణం MathItalicsCorrectionInfo
{
  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్థిరరాశి hb_సమితి_t &glyphset = c->plan->_glyphset_mathed;
    స్థిరరాశి hb_map_t &glyph_map = *c->plan->glyph_map;

    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);

    hb_sorted_సదిశ_t<hb_codepoint_t> new_coverage;
    + hb_zip (ఇది+coverage, italicsCorrection)
    | hb_filter (glyphset, hb_మొదటి)
    | hb_filter (serialize_math_రికార్డు_array (c->serializer, బహిర్గతం->italicsCorrection, ఇది, &c->plan->layout_variation_idx_delta_map), hb_రెండవ)
    | hb_map (hb_మొదటి)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    బహిర్గతం->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (నిజం);
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి స్వయంచాలక &నమోదు_2 : hb_zip (ఇది+coverage, italicsCorrection))
      ఒకవేళ (c->glyph_సమితి->has (hb_మొదటి (నమోదు_2)))
        hb_రెండవ (నమోదు_2).collect_variation_indices (c, ఇది);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  coverage.sanitize (c, ఇది) &&
		  italicsCorrection.sanitize (c, ఇది));
  }

  hb_position_t get_విలువ (hb_codepoint_t glyph,
			   hb_font_t *font) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం సూచీ_2 = (ఇది+coverage).get_coverage (glyph);
    తిరిగి italicsCorrection[సూచీ_2].get_x_విలువ (font, ఇది);
  }

  రక్షిత:
  Offset16_గమ్యం<Coverage>       coverage;		/* Offset to Coverage table -
						 * from the beginning of
						 * MathItalicsCorrectionInfo
						 * table. */
  Array16_యొక్క<Math_విలువ_రికార్డు> italicsCorrection;	/* Array of MathValueRecords
						 * defining italics correction
						 * values for each
						 * covered glyph. */

  బహిరంగ:
  DEFINE_కొలత_ARRAY (4, italicsCorrection);
};

నిర్మాణం Math_శిఖరం_Accent_Attachment
{
  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్థిరరాశి hb_సమితి_t &glyphset = c->plan->_glyphset_mathed;
    స్థిరరాశి hb_map_t &glyph_map = *c->plan->glyph_map;

    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);

    hb_sorted_సదిశ_t<hb_codepoint_t> new_coverage;
    + hb_zip (ఇది+శిఖరం_Accent_Coverage, శిఖరం_Accent_Attachment)
    | hb_filter (glyphset, hb_మొదటి)
    | hb_filter (serialize_math_రికార్డు_array (c->serializer, బహిర్గతం->శిఖరం_Accent_Attachment, ఇది, &c->plan->layout_variation_idx_delta_map), hb_రెండవ)
    | hb_map (hb_మొదటి)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    బహిర్గతం->శిఖరం_Accent_Coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (నిజం);
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి స్వయంచాలక &నమోదు_2 : hb_zip (ఇది+శిఖరం_Accent_Coverage, శిఖరం_Accent_Attachment))
      ఒకవేళ (c->glyph_సమితి->has (hb_మొదటి (నమోదు_2)))
        hb_రెండవ (నమోదు_2).collect_variation_indices (c, ఇది);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  శిఖరం_Accent_Coverage.sanitize (c, ఇది) &&
		  శిఖరం_Accent_Attachment.sanitize (c, ఇది));
  }

  hb_position_t get_విలువ (hb_codepoint_t glyph,
			   hb_font_t *font) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం సూచీ_2 = (ఇది+శిఖరం_Accent_Coverage).get_coverage (glyph);
    ఒకవేళ (సూచీ_2 == NOT_COVERED)
      తిరిగి font->get_glyph_h_advance (glyph) / 2;
    తిరిగి శిఖరం_Accent_Attachment[సూచీ_2].get_x_విలువ (font, ఇది);
  }

  రక్షిత:
  Offset16_గమ్యం<Coverage>       శిఖరం_Accent_Coverage;   /* Offset to Coverage table -
						 * from the beginning of
						 * MathTopAccentAttachment
						 * table. */
  Array16_యొక్క<Math_విలువ_రికార్డు> శిఖరం_Accent_Attachment; /* Array of MathValueRecords
						 * defining top accent
						 * attachment points for each
						 * covered glyph. */

  బహిరంగ:
  DEFINE_కొలత_ARRAY (2 + 2, శిఖరం_Accent_Attachment);
};

నిర్మాణం MathKern
{
  MathKern* copy (hb_serialize_context_t *c,
                      స్థిరరాశి hb_hashmap_t<గుర్తులేని, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> *variation_map) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    స్వయంచాలక *బహిర్గతం = c->ప్రారంభం_embed (ఇది);

    ఒకవేళ (unlikely (!c->embed (ఎత్తు_లెక్క))) return_trace (శూన్యసూచిక);

    గుర్తులేని లెక్క = 2 * ఎత్తు_లెక్క + 1;
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
      ఒకవేళ (!c->copy (math_విలువ_రికార్డులు_Z.arrayZ[i], ఇది, variation_map))
        return_trace (శూన్యసూచిక);

    return_trace (బహిర్గతం);
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి స్వయంచాలక &రికార్డు : math_విలువ_రికార్డులు_Z.as_array (2 * ఎత్తు_లెక్క + 1))
      రికార్డు.collect_variation_indices (c, ఇది);
  }

  తార్కిక sanitize_math_విలువ_రికార్డులు (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    గుర్తులేని పూర్ణాంకం లెక్క = 2 * ఎత్తు_లెక్క + 1;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      ఒకవేళ (!math_విలువ_రికార్డులు_Z.arrayZ[i].sanitize (c, ఇది)) return_trace (అబద్ధం);
    return_trace (నిజం);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
		  c->check_array (math_విలువ_రికార్డులు_Z.arrayZ, 2 * ఎత్తు_లెక్క + 1) &&
		  sanitize_math_విలువ_రికార్డులు (c));
  }

  hb_position_t get_విలువ (hb_position_t correction_ఎత్తు, hb_font_t *font) స్థిరరాశి
  {
    స్థిరరాశి Math_విలువ_రికార్డు* correction_ఎత్తు_2 = math_విలువ_రికార్డులు_Z.arrayZ;
    స్థిరరాశి Math_విలువ_రికార్డు* kern_విలువ = math_విలువ_రికార్డులు_Z.arrayZ + ఎత్తు_లెక్క;
    పూర్ణాంకం sign = font->y_scale < 0 ? -1 : +1;

    /* According to OpenType spec (v1.9), except for the boundary cases, the index
     * chosen for kern value should be i such that
     *    correctionHeight[i-1] <= correction_height < correctionHeight[i]
     * We can use the binary search algorithm of std::upper_bound(). Or, we can
     * use the internal hb_bsearch_impl.
     */
    గుర్తులేని పూర్ణాంకం pos;
    స్వయంచాలక cmp = +[](స్థిరరాశి శూన్య* key, స్థిరరాశి శూన్య* p,
                   పూర్ణాంకం sign, hb_font_t* font, స్థిరరాశి MathKern* mathKern) -> పూర్ణాంకం {
      hb_position_t a = *(hb_position_t *) key;
      hb_position_t b = ((Math_విలువ_రికార్డు *) p)->get_y_విలువ (font, mathKern);
      ఒకవేళ (a == b) తిరిగి 0;
      తిరిగి sign > 0 ? (a < b ? -1 : +1) : (b < a ? -1 : +1);
    };
    గుర్తులేని పూర్ణాంకం i = hb_bsearch_impl(&pos, correction_ఎత్తు, correction_ఎత్తు_2,
                                     ఎత్తు_లెక్క, Math_విలువ_రికార్డు::static_కొలత,
                                     cmp, sign, font, ఇది) ? pos + 1 : pos;
    తిరిగి kern_విలువ[i].get_x_విలువ (font, ఇది);
  }

  గుర్తులేని పూర్ణాంకం get_entries (గుర్తులేని పూర్ణాంకం ప్రారంభం_స్థానభ్రంశం,
			    గుర్తులేని పూర్ణాంకం *entries_లెక్క, /* IN/OUT */
			    hb_ot_math_kern_నమోదు_t *kern_entries, /*బహిర్గతం*/
			    hb_font_t *font) స్థిరరాశి
  {
    స్థిరరాశి Math_విలువ_రికార్డు* correction_ఎత్తు_2 = math_విలువ_రికార్డులు_Z.arrayZ;
    స్థిరరాశి Math_విలువ_రికార్డు* kern_విలువ = math_విలువ_రికార్డులు_Z.arrayZ + ఎత్తు_లెక్క;
    స్థిరరాశి గుర్తులేని పూర్ణాంకం entries_లెక్క_2 = ఎత్తు_లెక్క + 1;

    ఒకవేళ (entries_లెక్క && kern_entries)
    {
      గుర్తులేని పూర్ణాంకం ప్రారంభం = hb_min (ప్రారంభం_స్థానభ్రంశం, entries_లెక్క_2);
      గుర్తులేని పూర్ణాంకం చివర = hb_min (ప్రారంభం + *entries_లెక్క, entries_లెక్క_2);
      *entries_లెక్క = చివర - ప్రారంభం;

      కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < *entries_లెక్క; i++) {
	గుర్తులేని పూర్ణాంకం j = ప్రారంభం + i;

	hb_position_t max_ఎత్తు;
	ఒకవేళ (j == ఎత్తు_లెక్క) {
	  max_ఎత్తు = INT32_MAX;
	} లేకపోతే {
	  max_ఎత్తు = correction_ఎత్తు_2[j].get_y_విలువ (font, ఇది);
	}

	kern_entries[i] = {max_ఎత్తు, kern_విలువ[j].get_x_విలువ (font, ఇది)};
      }
    }
    తిరిగి entries_లెక్క_2;
  }

  రక్షిత:
  HBUINT16	ఎత్తు_లెక్క;
  Unsized_Array_యొక్క<Math_విలువ_రికార్డు>
		math_విలువ_రికార్డులు_Z;
				/* Array of correction heights at
				 * which the kern value changes.
				 * Sorted by the height value in
				 * design units (heightCount entries),
				 * Followed by:
				 * Array of kern values corresponding
				 * to heights. (heightCount+1 entries).
				 */

  బహిరంగ:
  DEFINE_కొలత_ARRAY (2, math_విలువ_రికార్డులు_Z);
};

నిర్మాణం Math_Kern_Info_రికార్డు
{
  Math_Kern_Info_రికార్డు* copy (hb_serialize_context_t *c, స్థిరరాశి శూన్య *పునాది,
                            స్థిరరాశి hb_hashmap_t<గుర్తులేని, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> *variation_map) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    స్వయంచాలక *బహిర్గతం = c->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (శూన్యసూచిక);

    గుర్తులేని లెక్క = ARRAY_నిడివి (mathKern);
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
      బహిర్గతం->mathKern[i].serialize_copy (c, mathKern[i], పునాది, 0, hb_serialize_context_t::తల, variation_map);

    return_trace (బహిర్గతం);
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి స్వయంచాలక &స్థానభ్రంశం : mathKern)
      (పునాది+స్థానభ్రంశం).collect_variation_indices (c);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);

    గుర్తులేని పూర్ణాంకం లెక్క = ARRAY_నిడివి (mathKern);
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      ఒకవేళ (unlikely (!mathKern[i].sanitize (c, పునాది)))
	return_trace (అబద్ధం);

    return_trace (నిజం);
  }

  hb_position_t get_kerning (hb_ot_math_kern_t kern,
			     hb_position_t correction_ఎత్తు,
			     hb_font_t *font,
			     స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం idx = kern;
    ఒకవేళ (unlikely (idx >= ARRAY_నిడివి (mathKern))) తిరిగి 0;
    తిరిగి (పునాది+mathKern[idx]).get_విలువ (correction_ఎత్తు, font);
  }

  గుర్తులేని పూర్ణాంకం get_kernings (hb_ot_math_kern_t kern,
			     గుర్తులేని పూర్ణాంకం ప్రారంభం_స్థానభ్రంశం,
			     గుర్తులేని పూర్ణాంకం *entries_లెక్క, /* IN/OUT */
			     hb_ot_math_kern_నమోదు_t *kern_entries, /*బహిర్గతం*/
			     hb_font_t *font,
			     స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం idx = kern;
    ఒకవేళ (unlikely (idx >= ARRAY_నిడివి (mathKern)) || !mathKern[idx]) {
      ఒకవేళ (entries_లెక్క) *entries_లెక్క = 0;
      తిరిగి 0;
    }
    తిరిగి (పునాది+mathKern[idx]).get_entries (ప్రారంభం_స్థానభ్రంశం,
					     entries_లెక్క,
					     kern_entries,
					     font);
  }

  రక్షిత:
  /* Offset to MathKern table for each corner -
   * from the beginning of MathKernInfo table.  May be NULL. */
  Offset16_గమ్యం<MathKern> mathKern[4];

  బహిరంగ:
  DEFINE_కొలత_STATIC (8);
};

నిర్మాణం MathKernInfo
{
  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్థిరరాశి hb_సమితి_t &glyphset = c->plan->_glyphset_mathed;
    స్థిరరాశి hb_map_t &glyph_map = *c->plan->glyph_map;

    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);

    hb_sorted_సదిశ_t<hb_codepoint_t> new_coverage;
    + hb_zip (ఇది+mathKernCoverage, math_Kern_Info_రికార్డులు)
    | hb_filter (glyphset, hb_మొదటి)
    | hb_filter (serialize_math_రికార్డు_array (c->serializer, బహిర్గతం->math_Kern_Info_రికార్డులు, ఇది, &c->plan->layout_variation_idx_delta_map), hb_రెండవ)
    | hb_map (hb_మొదటి)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    బహిర్గతం->mathKernCoverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (నిజం);
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి స్వయంచాలక &నమోదు_2 : hb_zip (ఇది+mathKernCoverage, math_Kern_Info_రికార్డులు))
      ఒకవేళ (c->glyph_సమితి->has (hb_మొదటి (నమోదు_2)))
        hb_రెండవ (నమోదు_2).collect_variation_indices (c, ఇది);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  mathKernCoverage.sanitize (c, ఇది) &&
		  math_Kern_Info_రికార్డులు.sanitize (c, ఇది));
  }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_ఎత్తు,
			     hb_font_t *font) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం సూచీ_2 = (ఇది+mathKernCoverage).get_coverage (glyph);
    తిరిగి math_Kern_Info_రికార్డులు[సూచీ_2].get_kerning (kern, correction_ఎత్తు, font, ఇది);
  }

  గుర్తులేని పూర్ణాంకం get_kernings (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     గుర్తులేని పూర్ణాంకం ప్రారంభం_స్థానభ్రంశం,
			     గుర్తులేని పూర్ణాంకం *entries_లెక్క, /* IN/OUT */
			     hb_ot_math_kern_నమోదు_t *kern_entries, /*బహిర్గతం*/
			     hb_font_t *font) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం సూచీ_2 = (ఇది+mathKernCoverage).get_coverage (glyph);
    తిరిగి math_Kern_Info_రికార్డులు[సూచీ_2].get_kernings (kern,
						    ప్రారంభం_స్థానభ్రంశం,
						    entries_లెక్క,
						    kern_entries,
						    font,
						    ఇది);
  }

  రక్షిత:
  Offset16_గమ్యం<Coverage>
		mathKernCoverage;
				/* Offset to Coverage table -
				 * from the beginning of the
				 * MathKernInfo table. */
  Array16_యొక్క<Math_Kern_Info_రికార్డు>
		math_Kern_Info_రికార్డులు;
				/* Array of MathKernInfoRecords,
				 * per-glyph information for
				 * mathematical positioning
				 * of subscripts and
				 * superscripts. */

  బహిరంగ:
  DEFINE_కొలత_ARRAY (4, math_Kern_Info_రికార్డులు);
};

నిర్మాణం MathGlyphInfo
{
  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (*ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    బహిర్గతం->mathItalicsCorrectionInfo.serialize_subset (c, mathItalicsCorrectionInfo, ఇది);
    బహిర్గతం->math_శిఖరం_Accent_Attachment.serialize_subset (c, math_శిఖరం_Accent_Attachment, ఇది);

    స్థిరరాశి hb_సమితి_t &glyphset = c->plan->_glyphset_mathed;
    స్థిరరాశి hb_map_t &glyph_map = *c->plan->glyph_map;

    స్వయంచాలక it =
    + hb_iter (ఇది+extendedShapeCoverage)
    | hb_take (c->plan->ఆధారం->get_అంకె_glyphs ())
    | hb_filter (glyphset)
    | hb_map_retains_క్రమీకరణ (glyph_map)
    ;

    ఒకవేళ (it) బహిర్గతం->extendedShapeCoverage.serialize_serialize (c->serializer, it);
    లేకపోతే బహిర్గతం->extendedShapeCoverage = 0;

    బహిర్గతం->mathKernInfo.serialize_subset (c, mathKernInfo, ఇది);
    return_trace (నిజం);
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి
  {
    (ఇది+mathItalicsCorrectionInfo).collect_variation_indices (c);
    (ఇది+math_శిఖరం_Accent_Attachment).collect_variation_indices (c);
    (ఇది+mathKernInfo).collect_variation_indices (c);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  mathItalicsCorrectionInfo.sanitize (c, ఇది) &&
		  math_శిఖరం_Accent_Attachment.sanitize (c, ఇది) &&
		  extendedShapeCoverage.sanitize (c, ఇది) &&
		  mathKernInfo.sanitize (c, ఇది));
  }

  hb_position_t
  get_italics_correction (hb_codepoint_t  glyph, hb_font_t *font) స్థిరరాశి
  { తిరిగి (ఇది+mathItalicsCorrectionInfo).get_విలువ (glyph, font); }

  hb_position_t
  get_శిఖరం_accent_attachment (hb_codepoint_t  glyph, hb_font_t *font) స్థిరరాశి
  { తిరిగి (ఇది+math_శిఖరం_Accent_Attachment).get_విలువ (glyph, font); }

  తార్కిక is_extended_shape (hb_codepoint_t glyph) స్థిరరాశి
  { తిరిగి (ఇది+extendedShapeCoverage).get_coverage (glyph) != NOT_COVERED; }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_ఎత్తు,
			     hb_font_t *font) స్థిరరాశి
  { తిరిగి (ఇది+mathKernInfo).get_kerning (glyph, kern, correction_ఎత్తు, font); }

  hb_position_t get_kernings (hb_codepoint_t glyph,
			      hb_ot_math_kern_t kern,
			      గుర్తులేని పూర్ణాంకం ప్రారంభం_స్థానభ్రంశం,
			      గుర్తులేని పూర్ణాంకం *entries_లెక్క, /* IN/OUT */
			      hb_ot_math_kern_నమోదు_t *kern_entries, /*బహిర్గతం*/
			      hb_font_t *font) స్థిరరాశి
  { తిరిగి (ఇది+mathKernInfo).get_kernings (glyph,
					     kern,
					     ప్రారంభం_స్థానభ్రంశం,
					     entries_లెక్క,
					     kern_entries,
					     font); }

  రక్షిత:
  /* Offset to MathItalicsCorrectionInfo table -
   * from the beginning of MathGlyphInfo table. */
  Offset16_గమ్యం<MathItalicsCorrectionInfo> mathItalicsCorrectionInfo;

  /* Offset to MathTopAccentAttachment table -
   * from the beginning of MathGlyphInfo table. */
  Offset16_గమ్యం<Math_శిఖరం_Accent_Attachment> math_శిఖరం_Accent_Attachment;

  /* Offset to coverage table for Extended Shape glyphs -
   * from the beginning of MathGlyphInfo table. When the left or right glyph of
   * a box is an extended shape variant, the (ink) box (and not the default
   * position defined by values in MathConstants table) should be used for
   * vertical positioning purposes.  May be NULL.. */
  Offset16_గమ్యం<Coverage> extendedShapeCoverage;

   /* Offset to MathKernInfo table -
    * from the beginning of MathGlyphInfo table. */
  Offset16_గమ్యం<MathKernInfo> mathKernInfo;

  బహిరంగ:
  DEFINE_కొలత_STATIC (8);
};

నిర్మాణం Math_Glyph_Variant_రికార్డు
{
  మిత్ర నిర్మాణం MathGlyphConstruction;

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    స్థిరరాశి hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_కేటాయించు (బహిర్గతం->variantGlyph, glyph_map.get (variantGlyph), HB_SERIALIZE_ERROR_INT_అధికప్రవాహం));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  శూన్య depend (hb_depend_దత్తాంశం_builder_t *depend_దత్తాంశం, గుర్తులేని ఆధారం) స్థిరరాశి
  { depend_దత్తాంశం->కూడు_depend(ఆధారం, HB_OT_ట్యాగ్_MATH, variantGlyph); }

  శూన్య సంవృతం_glyphs (hb_సమితి_t *variant_glyphs) స్థిరరాశి
  { variant_glyphs->కూడు (variantGlyph); }

  రక్షిత:
  HBGlyphID16 variantGlyph;       /* Glyph ID for the variant. */
  HBUINT16  advanceMeasurement; /* Advance width/height, in design units, of the
				 * variant, in the direction of requested
				 * glyph extension. */

  బహిరంగ:
  DEFINE_కొలత_STATIC (4);
};

నిర్మాణం PartFlags : HBUINT16
{
  గణన Flags {
    Extender	= 0x0001u, /* If set, the part can be skipped or repeated. */

    Defined	= 0x0001u, /* All defined flags. */
  };

  బహిరంగ:
  DEFINE_కొలత_STATIC (2);
};

నిర్మాణం Math_Glyph_Part_రికార్డు
{
  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    స్థిరరాశి hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_కేటాయించు (బహిర్గతం->glyph, glyph_map.get (glyph), HB_SERIALIZE_ERROR_INT_అధికప్రవాహం));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  శూన్య extract (hb_ot_math_glyph_part_t &బహిర్గతం,
		int64_t mult,
		hb_font_t *font) స్థిరరాశి
  {
    బహిర్గతం.glyph			= glyph;

    బహిర్గతం.ప్రారంభం_connector_నిడివి	= font->em_mult (ప్రారంభం_Connector_నిడివి, mult);
    బహిర్గతం.చివర_connector_నిడివి	= font->em_mult (చివర_Connector_నిడివి, mult);
    బహిర్గతం.నిండిన_advance		= font->em_mult (నిండిన_Advance, mult);

    static_assert ((గుర్తులేని పూర్ణాంకం) HB_OT_MATH_GLYPH_PART_FLAG_EXTENDER ==
		   (గుర్తులేని పూర్ణాంకం) PartFlags::Extender, "");

    బహిర్గతం.flags = (hb_ot_math_glyph_part_flags_t)
		(గుర్తులేని పూర్ణాంకం)
		(partFlags & PartFlags::Defined);
  }

  శూన్య depend (hb_depend_దత్తాంశం_builder_t *depend_దత్తాంశం, గుర్తులేని ఆధారం) స్థిరరాశి
  { depend_దత్తాంశం->కూడు_depend(ఆధారం, HB_OT_ట్యాగ్_MATH, glyph); }

  శూన్య సంవృతం_glyphs (hb_సమితి_t *variant_glyphs) స్థిరరాశి
  { variant_glyphs->కూడు (glyph); }

  రక్షిత:
  HBGlyphID16	glyph;		/* Glyph ID for the part. */
  HBUINT16	ప్రారంభం_Connector_నిడివి;
				/* Advance width/ height of the straight bar
				 * connector material, in design units, is at
				 * the beginning of the glyph, in the
				 * direction of the extension. */
  HBUINT16	చివర_Connector_నిడివి;
				/* Advance width/ height of the straight bar
				 * connector material, in design units, is at
				 * the end of the glyph, in the direction of
				 * the extension. */
  HBUINT16	నిండిన_Advance;	/* Full advance width/height for this part,
				 * in the direction of the extension.
				 * In design units. */
  PartFlags	partFlags;	/* Part qualifiers. */

  బహిరంగ:
  DEFINE_కొలత_STATIC (10);
};

నిర్మాణం MathGlyphAssembly
{
  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);

    ఒకవేళ (!c->serializer->copy (italicsCorrection, ఇది, &c->plan->layout_variation_idx_delta_map)) return_trace (అబద్ధం);
    ఒకవేళ (!c->serializer->copy<HBUINT16> (part_రికార్డులు.len)) return_trace (అబద్ధం);

    కొరకు (స్థిరరాశి స్వయంచాలక& రికార్డు : part_రికార్డులు.iter ())
      ఒకవేళ (!రికార్డు.subset (c)) return_trace (అబద్ధం);
    return_trace (నిజం);
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి
  {
    italicsCorrection.collect_variation_indices (c, ఇది);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  italicsCorrection.sanitize (c, ఇది) &&
		  part_రికార్డులు.sanitize (c));
  }

  గుర్తులేని పూర్ణాంకం get_parts (hb_direction_t direction,
			  hb_font_t *font,
			  గుర్తులేని పూర్ణాంకం ప్రారంభం_స్థానభ్రంశం,
			  గుర్తులేని పూర్ణాంకం *parts_లెక్క, /* IN/OUT */
			  hb_ot_math_glyph_part_t *parts /*బహిర్గతం*/,
			  hb_position_t *italics_correction /*బహిర్గతం*/) స్థిరరాశి
  {
    ఒకవేళ (parts_లెక్క && parts)
    {
      int64_t mult = font->dir_mult (direction);
      కొరకు (స్వయంచాలక _ : hb_zip (part_రికార్డులు.as_array ().sub_array (ప్రారంభం_స్థానభ్రంశం, parts_లెక్క),
			    hb_array (parts, *parts_లెక్క)))
	_.మొదటి.extract (_.second, mult, font);
    }

    ఒకవేళ (italics_correction)
      *italics_correction = italicsCorrection.get_x_విలువ (font, ఇది);

    తిరిగి part_రికార్డులు.len;
  }

  శూన్య depend (hb_depend_దత్తాంశం_builder_t *depend_దత్తాంశం, గుర్తులేని ఆధారం) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి స్వయంచాలక& _ : part_రికార్డులు.iter ())
      _.depend (depend_దత్తాంశం, ఆధారం);
  }

  శూన్య సంవృతం_glyphs (hb_సమితి_t *variant_glyphs) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి స్వయంచాలక& _ : part_రికార్డులు.iter ())
      _.సంవృతం_glyphs (variant_glyphs);
  }

  రక్షిత:
  Math_విలువ_రికార్డు
		italicsCorrection;
				/* Italics correction of this
				 * MathGlyphAssembly. Should not
				 * depend on the assembly size. */
  Array16_యొక్క<Math_Glyph_Part_రికార్డు>
		part_రికార్డులు;	/* Array of part records, from
				 * left to right and bottom to
				 * top. */

  బహిరంగ:
  DEFINE_కొలత_ARRAY (6, part_రికార్డులు);
};

నిర్మాణం MathGlyphConstruction
{
  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);

    బహిర్గతం->glyphAssembly.serialize_subset (c, glyphAssembly, ఇది);

    ఒకవేళ (!c->serializer->check_కేటాయించు (బహిర్గతం->math_Glyph_Variant_రికార్డు.len, math_Glyph_Variant_రికార్డు.len, HB_SERIALIZE_ERROR_INT_అధికప్రవాహం))
      return_trace (అబద్ధం);
    కొరకు (స్థిరరాశి స్వయంచాలక& రికార్డు : math_Glyph_Variant_రికార్డు.iter ())
      ఒకవేళ (!రికార్డు.subset (c)) return_trace (అబద్ధం);

    return_trace (నిజం);
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి
  {
    (ఇది+glyphAssembly).collect_variation_indices (c);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  glyphAssembly.sanitize (c, ఇది) &&
		  math_Glyph_Variant_రికార్డు.sanitize (c));
  }

  స్థిరరాశి MathGlyphAssembly &get_assembly () స్థిరరాశి { తిరిగి ఇది+glyphAssembly; }

  గుర్తులేని పూర్ణాంకం get_variants (hb_direction_t direction,
			     hb_font_t *font,
			     గుర్తులేని పూర్ణాంకం ప్రారంభం_స్థానభ్రంశం,
			     గుర్తులేని పూర్ణాంకం *variants_లెక్క, /* IN/OUT */
			     hb_ot_math_glyph_variant_t *variants /*బహిర్గతం*/) స్థిరరాశి
  {
    ఒకవేళ (variants_లెక్క && variants)
    {
      int64_t mult = font->dir_mult (direction);
      కొరకు (స్వయంచాలక _ : hb_zip (math_Glyph_Variant_రికార్డు.as_array ().sub_array (ప్రారంభం_స్థానభ్రంశం, variants_లెక్క),
			    hb_array (variants, *variants_లెక్క)))
	_.second = {_.మొదటి.variantGlyph, font->em_mult (_.మొదటి.advanceMeasurement, mult)};
    }
    తిరిగి math_Glyph_Variant_రికార్డు.len;
  }

  శూన్య depend (hb_depend_దత్తాంశం_builder_t *depend_దత్తాంశం, గుర్తులేని ఆధారం) స్థిరరాశి
  {
    (ఇది+glyphAssembly).depend (depend_దత్తాంశం, ఆధారం);

    కొరకు (స్థిరరాశి స్వయంచాలక& _ : math_Glyph_Variant_రికార్డు.iter ())
      _.depend (depend_దత్తాంశం, ఆధారం);
  }

  శూన్య సంవృతం_glyphs (hb_సమితి_t *variant_glyphs) స్థిరరాశి
  {
    (ఇది+glyphAssembly).సంవృతం_glyphs (variant_glyphs);

    కొరకు (స్థిరరాశి స్వయంచాలక& _ : math_Glyph_Variant_రికార్డు.iter ())
      _.సంవృతం_glyphs (variant_glyphs);
  }

  రక్షిత:
  /* Offset to MathGlyphAssembly table for this shape - from the beginning of
     MathGlyphConstruction table.  May be NULL. */
  Offset16_గమ్యం<MathGlyphAssembly>	  glyphAssembly;

  /* MathGlyphVariantRecords for alternative variants of the glyphs. */
  Array16_యొక్క<Math_Glyph_Variant_రికార్డు> math_Glyph_Variant_రికార్డు;

  బహిరంగ:
  DEFINE_కొలత_ARRAY (4, math_Glyph_Variant_రికార్డు);
};

నిర్మాణం MathVariants
{
  శూన్య depend (hb_depend_దత్తాంశం_builder_t *depend_దత్తాంశం) స్థిరరాశి
  {
    స్థిరరాశి hb_array_t<స్థిరరాశి Offset16_గమ్యం<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vert_Glyph_లెక్క + horiz_Glyph_లెక్క);

    ఒకవేళ (vertGlyphCoverage)
    {
      స్థిరరాశి స్వయంచాలక vert_offsets = glyph_construction_offsets.sub_array (0, vert_Glyph_లెక్క);
      + hb_zip (ఇది+vertGlyphCoverage, vert_offsets)
      | hb_apply ([&] (స్థిరరాశి hb_pair_t<hb_codepoint_t, స్థిరరాశి Offset16_గమ్యం<MathGlyphConstruction>&> &_)
                  {
                    స్థిరరాశి MathGlyphConstruction &mgc = ఇది+_.second;
                    mgc.depend (depend_దత్తాంశం, _.మొదటి);
                  })
      ;
    }
    ఒకవేళ (horizGlyphCoverage)
    {
      స్థిరరాశి స్వయంచాలక hori_offsets = glyph_construction_offsets.sub_array (vert_Glyph_లెక్క, horiz_Glyph_లెక్క);
      + hb_zip (ఇది+horizGlyphCoverage, hori_offsets)
      | hb_apply ([&] (స్థిరరాశి hb_pair_t<hb_codepoint_t, స్థిరరాశి Offset16_గమ్యం<MathGlyphConstruction>&> &_)
                  {
                    స్థిరరాశి MathGlyphConstruction &mgc = ఇది+_.second;
                    mgc.depend (depend_దత్తాంశం, _.మొదటి);
                  })
      ;
    }
  }

  శూన్య సంవృతం_glyphs (స్థిరరాశి hb_సమితి_t *glyph_సమితి,
                       hb_సమితి_t *variant_glyphs) స్థిరరాశి
  {
    స్థిరరాశి hb_array_t<స్థిరరాశి Offset16_గమ్యం<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vert_Glyph_లెక్క + horiz_Glyph_లెక్క);

    ఒకవేళ (vertGlyphCoverage)
    {
      స్థిరరాశి స్వయంచాలక vert_offsets = glyph_construction_offsets.sub_array (0, vert_Glyph_లెక్క);
      + hb_zip (ఇది+vertGlyphCoverage, vert_offsets)
      | hb_filter (glyph_సమితి, hb_మొదటి)
      | hb_map (hb_రెండవ)
      | hb_map (hb_కూడు (ఇది))
      | hb_apply ([=] (స్థిరరాశి MathGlyphConstruction &_) { _.సంవృతం_glyphs (variant_glyphs); })
      ;
    }

    ఒకవేళ (horizGlyphCoverage)
    {
      స్థిరరాశి స్వయంచాలక hori_offsets = glyph_construction_offsets.sub_array (vert_Glyph_లెక్క, horiz_Glyph_లెక్క);
      + hb_zip (ఇది+horizGlyphCoverage, hori_offsets)
      | hb_filter (glyph_సమితి, hb_మొదటి)
      | hb_map (hb_రెండవ)
      | hb_map (hb_కూడు (ఇది))
      | hb_apply ([=] (స్థిరరాశి MathGlyphConstruction &_) { _.సంవృతం_glyphs (variant_glyphs); })
      ;
    }
  }

  శూన్య collect_coverage_and_indices (hb_sorted_సదిశ_t<hb_codepoint_t>& new_coverage,
                                     స్థిరరాశి Offset16_గమ్యం<Coverage>& coverage,
                                     గుర్తులేని i,
                                     గుర్తులేని చివర_సూచీ,
                                     hb_సమితి_t& indices,
                                     స్థిరరాశి hb_సమితి_t& glyphset,
                                     స్థిరరాశి hb_map_t& glyph_map) స్థిరరాశి
  {
    ఒకవేళ (!coverage) తిరిగి;

    కొరకు (స్థిరరాశి స్వయంచాలక _ : (ఇది+coverage).iter ())
    {
      ఒకవేళ (i >= చివర_సూచీ) తిరిగి;
      ఒకవేళ (glyphset.has (_))
      {
        గుర్తులేని new_gid = glyph_map.get (_);
        new_coverage.push (new_gid);
        indices.కూడు (i);
      }
      i++;
    }
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్థిరరాశి hb_సమితి_t &glyphset = c->plan->_glyphset_cmaped;
    స్థిరరాశి hb_map_t &glyph_map = *c->plan->glyph_map;

    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);
    ఒకవేళ (!c->serializer->check_కేటాయించు (బహిర్గతం->minConnectorOverlap, minConnectorOverlap, HB_SERIALIZE_ERROR_INT_అధికప్రవాహం))
      return_trace (అబద్ధం);

    hb_sorted_సదిశ_t<hb_codepoint_t> new_vert_coverage;
    hb_sorted_సదిశ_t<hb_codepoint_t> new_hori_coverage;
    hb_సమితి_t indices;
    collect_coverage_and_indices (new_vert_coverage, vertGlyphCoverage, 0, vert_Glyph_లెక్క, indices, glyphset, glyph_map);
    collect_coverage_and_indices (new_hori_coverage, horizGlyphCoverage, vert_Glyph_లెక్క, vert_Glyph_లెక్క + horiz_Glyph_లెక్క, indices, glyphset, glyph_map);

    ఒకవేళ (!c->serializer->check_కేటాయించు (బహిర్గతం->vert_Glyph_లెక్క, new_vert_coverage.నిడివి, HB_SERIALIZE_ERROR_INT_అధికప్రవాహం))
      return_trace (అబద్ధం);
    ఒకవేళ (!c->serializer->check_కేటాయించు (బహిర్గతం->horiz_Glyph_లెక్క, new_hori_coverage.నిడివి, HB_SERIALIZE_ERROR_INT_అధికప్రవాహం))
      return_trace (అబద్ధం);

    కొరకు (గుర్తులేని i : indices.iter ())
    {
      స్వయంచాలక *o = c->serializer->embed (glyphConstruction[i]);
      ఒకవేళ (!o) return_trace (అబద్ధం);
      o->serialize_subset (c, glyphConstruction[i], ఇది);
    }

    ఒకవేళ (new_vert_coverage)
      బహిర్గతం->vertGlyphCoverage.serialize_serialize (c->serializer, new_vert_coverage.iter ());

    ఒకవేళ (new_hori_coverage)
    బహిర్గతం->horizGlyphCoverage.serialize_serialize (c->serializer, new_hori_coverage.iter ());
    return_trace (నిజం);
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి స్వయంచాలక &నమోదు_2 : hb_zip (ఇది+vertGlyphCoverage, glyphConstruction.as_array (vert_Glyph_లెక్క)))
      ఒకవేళ (c->glyph_సమితి->has (hb_మొదటి (నమోదు_2)))
        (ఇది+hb_రెండవ (నమోదు_2)).collect_variation_indices (c);
    కొరకు (స్థిరరాశి స్వయంచాలక &నమోదు_2 : hb_zip (ఇది+horizGlyphCoverage, glyphConstruction.as_array (vert_Glyph_లెక్క + horiz_Glyph_లెక్క).sub_array (vert_Glyph_లెక్క)))
      ఒకవేళ (c->glyph_సమితి->has (hb_మొదటి (నమోదు_2)))
        (ఇది+hb_రెండవ (నమోదు_2)).collect_variation_indices (c);
  }

  తార్కిక sanitize_offsets (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    గుర్తులేని పూర్ణాంకం లెక్క = vert_Glyph_లెక్క + horiz_Glyph_లెక్క;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      ఒకవేళ (!glyphConstruction.arrayZ[i].sanitize (c, ఇది)) return_trace (అబద్ధం);
    return_trace (నిజం);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  vertGlyphCoverage.sanitize (c, ఇది) &&
		  horizGlyphCoverage.sanitize (c, ఇది) &&
		  hb_barrier () &&
		  c->check_array (glyphConstruction.arrayZ, vert_Glyph_లెక్క + horiz_Glyph_లెక్క) &&
		  sanitize_offsets (c));
  }

  hb_position_t get_min_connector_overlap (hb_direction_t direction,
						  hb_font_t *font) స్థిరరాశి
  { తిరిగి font->em_scale_dir (minConnectorOverlap, direction); }

  గుర్తులేని పూర్ణాంకం get_glyph_variants (hb_codepoint_t glyph,
				   hb_direction_t direction,
				   hb_font_t *font,
				   గుర్తులేని పూర్ణాంకం ప్రారంభం_స్థానభ్రంశం,
				   గుర్తులేని పూర్ణాంకం *variants_లెక్క, /* IN/OUT */
				   hb_ot_math_glyph_variant_t *variants /*బహిర్గతం*/) స్థిరరాశి
  { తిరిగి get_glyph_construction (glyph, direction, font)
	   .get_variants (direction, font, ప్రారంభం_స్థానభ్రంశం, variants_లెక్క, variants); }

  గుర్తులేని పూర్ణాంకం get_glyph_parts (hb_codepoint_t glyph,
				hb_direction_t direction,
				hb_font_t *font,
				గుర్తులేని పూర్ణాంకం ప్రారంభం_స్థానభ్రంశం,
				గుర్తులేని పూర్ణాంకం *parts_లెక్క, /* IN/OUT */
				hb_ot_math_glyph_part_t *parts /*బహిర్గతం*/,
				hb_position_t *italics_correction /*బహిర్గతం*/) స్థిరరాశి
  { తిరిగి get_glyph_construction (glyph, direction, font)
	   .get_assembly ()
	   .get_parts (direction, font,
		       ప్రారంభం_స్థానభ్రంశం, parts_లెక్క, parts,
		       italics_correction); }

  అంతరంగ:
  స్థిరరాశి MathGlyphConstruction &
  get_glyph_construction (hb_codepoint_t glyph,
			  hb_direction_t direction,
			  hb_font_t *font HB_వాడని) స్థిరరాశి
  {
    తార్కిక vertical = HB_DIRECTION_IS_VERTICAL (direction);
    గుర్తులేని పూర్ణాంకం లెక్క = vertical ? vert_Glyph_లెక్క : horiz_Glyph_లెక్క;
    స్థిరరాశి Offset16_గమ్యం<Coverage> &coverage = vertical ? vertGlyphCoverage
						  : horizGlyphCoverage;

    గుర్తులేని పూర్ణాంకం సూచీ_2 = (ఇది+coverage).get_coverage (glyph);
    ఒకవేళ (unlikely (సూచీ_2 >= లెక్క)) తిరిగి Null (MathGlyphConstruction);

    ఒకవేళ (!vertical)
      సూచీ_2 += vert_Glyph_లెక్క;

    తిరిగి ఇది+glyphConstruction[సూచీ_2];
  }

  రక్షిత:
  HBUINT16	minConnectorOverlap;
				/* Minimum overlap of connecting
				 * glyphs during glyph construction,
				 * in design units. */
  Offset16_గమ్యం<Coverage> vertGlyphCoverage;
				/* Offset to Coverage table -
				 * from the beginning of MathVariants
				 * table. */
  Offset16_గమ్యం<Coverage> horizGlyphCoverage;
				/* Offset to Coverage table -
				 * from the beginning of MathVariants
				 * table. */
  HBUINT16	vert_Glyph_లెక్క;	/* Number of glyphs for which
				 * information is provided for
				 * vertically growing variants. */
  HBUINT16	horiz_Glyph_లెక్క;/* Number of glyphs for which
				 * information is provided for
				 * horizontally growing variants. */

  /* Array of offsets to MathGlyphConstruction tables - from the beginning of
     the MathVariants table, for shapes growing in vertical/horizontal
     direction. */
  Unsized_Array_యొక్క<Offset16_గమ్యం<MathGlyphConstruction>>
			glyphConstruction;

  బహిరంగ:
  DEFINE_కొలత_ARRAY (10, glyphConstruction);
};


/*
 * MATH -- Mathematical typesetting
 * https://docs.microsoft.com/en-us/typography/opentype/spec/math
 */

నిర్మాణం MATH
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_MATH;

  తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి సంస్కరణ.గమ్యం_int (); }

  శూన్య depend (hb_depend_దత్తాంశం_builder_t *depend_దత్తాంశం) స్థిరరాశి
  {
    ఒకవేళ (mathVariants)
      (ఇది+mathVariants).depend (depend_దత్తాంశం);
  }

  శూన్య సంవృతం_glyphs (hb_సమితి_t *glyph_సమితి) స్థిరరాశి
  {
    ఒకవేళ (mathVariants)
    {
      hb_సమితి_t variant_glyphs;
      (ఇది+mathVariants).సంవృతం_glyphs (glyph_సమితి, &variant_glyphs);
      hb_సమితి_union (glyph_సమితి, &variant_glyphs);
    }
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (*ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    బహిర్గతం->mathConstants.serialize_copy (c->serializer, mathConstants, ఇది, 0, hb_serialize_context_t::తల, &c->plan->layout_variation_idx_delta_map);
    బహిర్గతం->mathGlyphInfo.serialize_subset (c, mathGlyphInfo, ఇది);
    బహిర్గతం->mathVariants.serialize_subset (c, mathVariants, ఇది);
    return_trace (నిజం);
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c, స్థిరరాశి hb_సమితి_t *variant_glyph_సమితి) స్థిరరాశి
  {
    (ఇది+mathConstants).collect_variation_indices (c);
    (ఇది+mathGlyphInfo).collect_variation_indices (c);
    స్వయంచాలక *glyph_సమితి = c->glyph_సమితి;
    c->glyph_సమితి = variant_glyph_సమితి;
    (ఇది+mathVariants).collect_variation_indices (c);
    c->glyph_సమితి = glyph_సమితి;
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (సంస్కరణ.sanitize (c) &&
		  likely (సంస్కరణ.major == 1) &&
		  hb_barrier () &&
		  mathConstants.sanitize (c, ఇది) &&
		  mathGlyphInfo.sanitize (c, ఇది) &&
		  mathVariants.sanitize (c, ఇది));
  }

  // https://github.com/harfbuzz/harfbuzz/issues/4653
  HB_అంతర్గత తార్కిక is_bad_cambria (hb_font_t *font) స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_MATH
    ఎంపిక HB_CODEPOINT_ENCODE3 (font->face->పట్టిక.MATH.get_blob ()->నిడివి,
                                 (ఇది+mathConstants).min_ఎత్తు[1], // displayOperatorMinHeight
                                 (ఇది+mathConstants).min_ఎత్తు[0]) // delimitedSubFormulaMinHeight
    {
      /* sha1sum:ab4a4fe054d23061f3c039493d6f665cfda2ecf5  cambria.ttc
       * sha1sum:086855301bff644f9d8827b88491fcf73a6d4cb9  cambria.ttc
       * sha1sum:b1e5a3feaca2ea3dfcf79ccb377de749ecf60343  cambria.ttc */
      స్థితి HB_CODEPOINT_ENCODE3 (25722, 2500, 3000):
        తిరిగి నిజం;
    }
#అయితే_ముగింపు
    తిరిగి అబద్ధం;
  }

  hb_position_t get_constant (hb_ot_math_constant_t  constant,
			      hb_font_t		   *font) స్థిరరాశి
  { తిరిగి (ఇది+mathConstants).get_విలువ (constant, font); }

  స్థిరరాశి MathGlyphInfo &get_glyph_info () స్థిరరాశి { తిరిగి ఇది+mathGlyphInfo; }

  స్థిరరాశి MathVariants &get_variants () స్థిరరాశి    { తిరిగి ఇది+mathVariants; }

  రక్షిత:
  Fixed_సంస్కరణ<>సంస్కరణ;	/* Version of the MATH table
				 * initially set to 0x00010000u */
  Offset16_గమ్యం<MathConstants>
		mathConstants;	/* MathConstants table */
  Offset16_గమ్యం<MathGlyphInfo>
		mathGlyphInfo;	/* MathGlyphInfo table */
  Offset16_గమ్యం<MathVariants>
		mathVariants;	/* MathVariants table */

  బహిరంగ:
  DEFINE_కొలత_STATIC (10);
};

} /* namespace OT */


#అయితే_ముగింపు /* HB_OT_MATH_TABLE_HH */

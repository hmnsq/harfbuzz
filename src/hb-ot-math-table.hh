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

#ifndef HB_OT_MATH_TABLE_HH
#define HB_OT_MATH_TABLE_HH

#ಅನ್ತರ್ಭಾವ "hb-open-type.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-layout-common.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-layout-gdef-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-math.h"
#ಅನ್ತರ್ಭಾವ "hb-depend-data.hh"

ನಾಮಾಕಾಶ OT {


ಸಂರಚನಾ Math_ಮೌಲ್ಯ_Record
{
  hb_position_t get_x_ಮೌಲ್ಯ (hb_font_t *font, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_saturate_add (font->em_scale_x (value), (base+deviceTable).get_x_delta (font, get_var_store (font, base))); }
  hb_position_t get_y_ಮೌಲ್ಯ (hb_font_t *font, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_saturate_add (font->em_scale_y (value), (base+deviceTable).get_y_delta (font, get_var_store (font, base))); }

  ಧ್ರುವ ItemVariationStore &get_var_store (hb_font_t *font, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
#ಯದಿ !defined(HB_NO_VAR) && !defined(HB_NO_OT_LAYOUT)
    ಯದಿ ((base+deviceTable).is_variation_device ())
      ಪ್ರತ್ಯಾಗಮ font->face->table.GDEF->table->get_var_store ();
#endif
    ಪ್ರತ್ಯಾಗಮ Null (ItemVariationStore);
  }

  Math_ಮೌಲ್ಯ_Record* copy (hb_serialize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base,
                         ಧ್ರುವ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ>> *variation_map) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    auto *out = c->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (nullptr);
    hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ> *varidx_delta;
    ಯದಿ (variation_map->has ((base+deviceTable).get_variation_index (), &varidx_delta) &&
        !c->check_assign (out->value, (int64_t) value + hb_second (*varidx_delta),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (nullptr);
    out->deviceTable.serialize_copy (c, deviceTable, base, 0, hb_serialize_context_t::Head, variation_map);

    return_trace (out);
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  { (base+deviceTable).collect_variation_indices (c); }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) && deviceTable.sanitize (c, base));
  }

  ರಕ್ಷಿತ:
  HBINT16		value;		/* The X or Y value in design units */
  Offset16To<Device>	deviceTable;	/* Offset to the device table - from the
					 * beginning of parent table.  May be NULL.
					 * Suggested format for device table is 1. */

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4);
};

ಸಂರಚನಾ MathConstants
{
  friend ಸಂರಚನಾ MATH;

  MathConstants* copy (hb_serialize_context_t *c,
                      ಧ್ರುವ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ>> *variation_map) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    auto *out = c->start_embed (ಏತತ್);

    HBINT16 *p = c->allocate_size<HBINT16> (HBINT16::static_size * 2);
    ಯದಿ (unlikely (!p)) return_trace (nullptr);
    hb_memcpy (p, percentScaleDown, HBINT16::static_size * 2);

    HBUINT16 *m = c->allocate_size<HBUINT16> (HBUINT16::static_size * 2);
    ಯದಿ (unlikely (!m)) return_trace (nullptr);
    hb_memcpy (m, minHeight, HBUINT16::static_size * 2);

    unsigned ಎಣಿಕೆ = ARRAY_LENGTH (math_ಮೌಲ್ಯ_Records);
    ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
      ಯದಿ (!c->copy (math_ಮೌಲ್ಯ_Records[i], ಏತತ್, variation_map))
        return_trace (nullptr);

    ಯದಿ (!c->embed (radicalDegreeBottomRaisePercent)) return_trace (nullptr);
    return_trace (out);
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ auto &record : math_ಮೌಲ್ಯ_Records)
      record.collect_variation_indices (c, ಏತತ್);
  }

  ತಾರ್ಕಿಕ sanitize_math_ಮೌಲ್ಯ_records (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);

    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = ARRAY_LENGTH (math_ಮೌಲ್ಯ_Records);
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
      ಯದಿ (!math_ಮೌಲ್ಯ_Records[i].sanitize (c, ಏತತ್))
	return_trace (ಅಸತ್ಯ);

    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) && sanitize_math_ಮೌಲ್ಯ_records (c));
  }

  hb_position_t get_ಮೌಲ್ಯ (hb_ot_math_constant_t constant,
			   hb_font_t *font) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (constant) {

    ಪಕ್ಷ HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_DOWN:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_SCRIPT_SCRIPT_PERCENT_SCALE_DOWN:
      ಪ್ರತ್ಯಾಗಮ percentScaleDown[constant - HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_DOWN];

    ಪಕ್ಷ HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_HEIGHT:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_DISPLAY_OPERATOR_MIN_HEIGHT:
      ಪ್ರತ್ಯಾಗಮ font->em_scale_y (minHeight[constant - HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_HEIGHT]);

    ಪಕ್ಷ HB_OT_MATH_CONSTANT_RADICAL_KERN_AFTER_DEGREE:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_RADICAL_KERN_BEFORE_DEGREE:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_SKEWED_FRACTION_HORIZONTAL_GAP:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_SPACE_AFTER_SCRIPT:
      ಪ್ರತ್ಯಾಗಮ math_ಮೌಲ್ಯ_Records[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_x_ಮೌಲ್ಯ (font, ಏತತ್);

    ಪಕ್ಷ HB_OT_MATH_CONSTANT_ACCENT_BASE_HEIGHT:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_AXIS_HEIGHT:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_FLATTENED_ACCENT_BASE_HEIGHT:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_DISPLAY_STYLE_SHIFT_DOWN:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_GAP_MIN:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_SHIFT_DOWN:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_FRACTION_DENOM_DISPLAY_STYLE_GAP_MIN:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_DISPLAY_STYLE_SHIFT_UP:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_GAP_MIN:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_SHIFT_UP:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_FRACTION_ಅಂಕೆ_DISPLAY_STYLE_GAP_MIN:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_FRACTION_RULE_THICKNESS:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_LOWER_LIMIT_BASELINE_DROP_MIN:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_LOWER_LIMIT_GAP_MIN:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_MATH_LEADING:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_OVERBAR_EXTRA_ASCENDER:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_OVERBAR_RULE_THICKNESS:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_OVERBAR_VERTICAL_GAP:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_RADICAL_DISPLAY_STYLE_VERTICAL_GAP:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_RADICAL_EXTRA_ASCENDER:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_RADICAL_RULE_THICKNESS:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_RADICAL_VERTICAL_GAP:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_SKEWED_FRACTION_VERTICAL_GAP:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_ರಾಶಿ_BOTTOM_DISPLAY_STYLE_SHIFT_DOWN:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_ರಾಶಿ_BOTTOM_SHIFT_DOWN:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_ರಾಶಿ_DISPLAY_STYLE_GAP_MIN:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_ರಾಶಿ_GAP_MIN:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_ರಾಶಿ_ತುದಿ_DISPLAY_STYLE_SHIFT_UP:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_ರಾಶಿ_ತುದಿ_SHIFT_UP:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_STRETCH_ರಾಶಿ_BOTTOM_SHIFT_DOWN:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_STRETCH_ರಾಶಿ_GAP_ABOVE_MIN:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_STRETCH_ರಾಶಿ_GAP_BELOW_MIN:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_STRETCH_ರಾಶಿ_ತುದಿ_SHIFT_UP:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_SUBSCRIPT_BASELINE_DROP_MIN:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_SUBSCRIPT_SHIFT_DOWN:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_SUBSCRIPT_ತುದಿ_MAX:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_SUB_SUPERSCRIPT_GAP_MIN:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_SUPERSCRIPT_BASELINE_DROP_MAX:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MAX_WITH_SUBSCRIPT:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MIN:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_SUPERSCRIPT_SHIFT_UP:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_SUPERSCRIPT_SHIFT_UP_CRAMPED:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_UNDERBAR_EXTRA_DESCENDER:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_UNDERBAR_RULE_THICKNESS:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_UNDERBAR_VERTICAL_GAP:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_UPPER_LIMIT_BASELINE_RISE_MIN:
    ಪಕ್ಷ HB_OT_MATH_CONSTANT_UPPER_LIMIT_GAP_MIN:
      ಪ್ರತ್ಯಾಗಮ math_ಮೌಲ್ಯ_Records[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_y_ಮೌಲ್ಯ (font, ಏತತ್);

    ಪಕ್ಷ HB_OT_MATH_CONSTANT_RADICAL_DEGREE_BOTTOM_RAISE_PERCENT:
      ಪ್ರತ್ಯಾಗಮ radicalDegreeBottomRaisePercent;

    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ 0;
    }
  }

  ರಕ್ಷಿತ:
  HBINT16 percentScaleDown[2];
  HBUINT16 minHeight[2];
  Math_ಮೌಲ್ಯ_Record math_ಮೌಲ್ಯ_Records[51];
  HBINT16 radicalDegreeBottomRaisePercent;

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (214);
};

ಸಂರಚನಾ MathItalicsCorrectionInfo
{
  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಧ್ರುವ hb_set_t &glyphset = c->plan->_glyphset_mathed;
    ಧ್ರುವ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (ಏತತ್+coverage, italicsCorrection)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->italicsCorrection, ಏತತ್, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (ಸತ್ಯ);
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ auto &entry : hb_zip (ಏತತ್+coverage, italicsCorrection))
      ಯದಿ (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, ಏತತ್);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  coverage.sanitize (c, ಏತತ್) &&
		  italicsCorrection.sanitize (c, ಏತತ್));
  }

  hb_position_t get_ಮೌಲ್ಯ (hb_codepoint_t glyph,
			   hb_font_t *font) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ index = (ಏತತ್+coverage).get_coverage (glyph);
    ಪ್ರತ್ಯಾಗಮ italicsCorrection[index].get_x_ಮೌಲ್ಯ (font, ಏತತ್);
  }

  ರಕ್ಷಿತ:
  Offset16To<Coverage>       coverage;		/* Offset to Coverage table -
						 * from the beginning of
						 * MathItalicsCorrectionInfo
						 * table. */
  Array16Of<Math_ಮೌಲ್ಯ_Record> italicsCorrection;	/* Array of MathValueRecords
						 * defining italics correction
						 * values for each
						 * covered glyph. */

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (4, italicsCorrection);
};

ಸಂರಚನಾ Math_ತುದಿ_Accent_Attachment
{
  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಧ್ರುವ hb_set_t &glyphset = c->plan->_glyphset_mathed;
    ಧ್ರುವ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (ಏತತ್+ತುದಿ_Accent_Coverage, ತುದಿ_Accent_Attachment)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->ತುದಿ_Accent_Attachment, ಏತತ್, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->ತುದಿ_Accent_Coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (ಸತ್ಯ);
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ auto &entry : hb_zip (ಏತತ್+ತುದಿ_Accent_Coverage, ತುದಿ_Accent_Attachment))
      ಯದಿ (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, ಏತತ್);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  ತುದಿ_Accent_Coverage.sanitize (c, ಏತತ್) &&
		  ತುದಿ_Accent_Attachment.sanitize (c, ಏತತ್));
  }

  hb_position_t get_ಮೌಲ್ಯ (hb_codepoint_t glyph,
			   hb_font_t *font) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ index = (ಏತತ್+ತುದಿ_Accent_Coverage).get_coverage (glyph);
    ಯದಿ (index == NOT_COVERED)
      ಪ್ರತ್ಯಾಗಮ font->get_glyph_h_advance (glyph) / 2;
    ಪ್ರತ್ಯಾಗಮ ತುದಿ_Accent_Attachment[index].get_x_ಮೌಲ್ಯ (font, ಏತತ್);
  }

  ರಕ್ಷಿತ:
  Offset16To<Coverage>       ತುದಿ_Accent_Coverage;   /* Offset to Coverage table -
						 * from the beginning of
						 * MathTopAccentAttachment
						 * table. */
  Array16Of<Math_ಮೌಲ್ಯ_Record> ತುದಿ_Accent_Attachment; /* Array of MathValueRecords
						 * defining top accent
						 * attachment points for each
						 * covered glyph. */

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (2 + 2, ತುದಿ_Accent_Attachment);
};

ಸಂರಚನಾ MathKern
{
  MathKern* copy (hb_serialize_context_t *c,
                      ಧ್ರುವ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ>> *variation_map) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    auto *out = c->start_embed (ಏತತ್);

    ಯದಿ (unlikely (!c->embed (height_ಎಣಿಕೆ))) return_trace (nullptr);

    unsigned ಎಣಿಕೆ = 2 * height_ಎಣಿಕೆ + 1;
    ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
      ಯದಿ (!c->copy (math_ಮೌಲ್ಯ_Records_Z.arrayZ[i], ಏತತ್, variation_map))
        return_trace (nullptr);

    return_trace (out);
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ auto &record : math_ಮೌಲ್ಯ_Records_Z.as_array (2 * height_ಎಣಿಕೆ + 1))
      record.collect_variation_indices (c, ಏತತ್);
  }

  ತಾರ್ಕಿಕ sanitize_math_ಮೌಲ್ಯ_records (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 2 * height_ಎಣಿಕೆ + 1;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
      ಯದಿ (!math_ಮೌಲ್ಯ_Records_Z.arrayZ[i].sanitize (c, ಏತತ್)) return_trace (ಅಸತ್ಯ);
    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  hb_barrier () &&
		  c->check_array (math_ಮೌಲ್ಯ_Records_Z.arrayZ, 2 * height_ಎಣಿಕೆ + 1) &&
		  sanitize_math_ಮೌಲ್ಯ_records (c));
  }

  hb_position_t get_ಮೌಲ್ಯ (hb_position_t correction_height, hb_font_t *font) ಧ್ರುವ
  {
    ಧ್ರುವ Math_ಮೌಲ್ಯ_Record* correctionHeight = math_ಮೌಲ್ಯ_Records_Z.arrayZ;
    ಧ್ರುವ Math_ಮೌಲ್ಯ_Record* kern_ಮೌಲ್ಯ = math_ಮೌಲ್ಯ_Records_Z.arrayZ + height_ಎಣಿಕೆ;
    ಪೂರ್ಣಾಙ್ಕ sign = font->y_scale < 0 ? -1 : +1;

    /* According to OpenType spec (v1.9), except for the boundary cases, the index
     * chosen for kern value should be i such that
     *    correctionHeight[i-1] <= correction_height < correctionHeight[i]
     * We can use the binary search algorithm of std::upper_bound(). Or, we can
     * use the internal hb_bsearch_impl.
     */
    unsigned ಪೂರ್ಣಾಙ್ಕ pos;
    auto cmp = +[](ಧ್ರುವ ಶೂನ್ಯ* key, ಧ್ರುವ ಶೂನ್ಯ* p,
                   ಪೂರ್ಣಾಙ್ಕ sign, hb_font_t* font, ಧ್ರುವ MathKern* mathKern) -> ಪೂರ್ಣಾಙ್ಕ {
      hb_position_t a = *(hb_position_t *) key;
      hb_position_t b = ((Math_ಮೌಲ್ಯ_Record *) p)->get_y_ಮೌಲ್ಯ (font, mathKern);
      ಯದಿ (a == b) ಪ್ರತ್ಯಾಗಮ 0;
      ಪ್ರತ್ಯಾಗಮ sign > 0 ? (a < b ? -1 : +1) : (b < a ? -1 : +1);
    };
    unsigned ಪೂರ್ಣಾಙ್ಕ i = hb_bsearch_impl(&pos, correction_height, correctionHeight,
                                     height_ಎಣಿಕೆ, Math_ಮೌಲ್ಯ_Record::static_size,
                                     cmp, sign, font, ಏತತ್) ? pos + 1 : pos;
    ಪ್ರತ್ಯಾಗಮ kern_ಮೌಲ್ಯ[i].get_x_ಮೌಲ್ಯ (font, ಏತತ್);
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_entries (unsigned ಪೂರ್ಣಾಙ್ಕ start_offset,
			    unsigned ಪೂರ್ಣಾಙ್ಕ *entries_ಎಣಿಕೆ, /* IN/OUT */
			    hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			    hb_font_t *font) ಧ್ರುವ
  {
    ಧ್ರುವ Math_ಮೌಲ್ಯ_Record* correctionHeight = math_ಮೌಲ್ಯ_Records_Z.arrayZ;
    ಧ್ರುವ Math_ಮೌಲ್ಯ_Record* kern_ಮೌಲ್ಯ = math_ಮೌಲ್ಯ_Records_Z.arrayZ + height_ಎಣಿಕೆ;
    ಧ್ರುವ unsigned ಪೂರ್ಣಾಙ್ಕ entries_ಎಣಿಕೆ_2 = height_ಎಣಿಕೆ + 1;

    ಯದಿ (entries_ಎಣಿಕೆ && kern_entries)
    {
      unsigned ಪೂರ್ಣಾಙ್ಕ start = hb_min (start_offset, entries_ಎಣಿಕೆ_2);
      unsigned ಪೂರ್ಣಾಙ್ಕ end = hb_min (start + *entries_ಎಣಿಕೆ, entries_ಎಣಿಕೆ_2);
      *entries_ಎಣಿಕೆ = end - start;

      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < *entries_ಎಣಿಕೆ; i++) {
	unsigned ಪೂರ್ಣಾಙ್ಕ j = start + i;

	hb_position_t max_height;
	ಯದಿ (j == height_ಎಣಿಕೆ) {
	  max_height = INT32_MAX;
	} ಅನ್ಯಥಾ {
	  max_height = correctionHeight[j].get_y_ಮೌಲ್ಯ (font, ಏತತ್);
	}

	kern_entries[i] = {max_height, kern_ಮೌಲ್ಯ[j].get_x_ಮೌಲ್ಯ (font, ಏತತ್)};
      }
    }
    ಪ್ರತ್ಯಾಗಮ entries_ಎಣಿಕೆ_2;
  }

  ರಕ್ಷಿತ:
  HBUINT16	height_ಎಣಿಕೆ;
  UnsizedArrayOf<Math_ಮೌಲ್ಯ_Record>
		math_ಮೌಲ್ಯ_Records_Z;
				/* Array of correction heights at
				 * which the kern value changes.
				 * Sorted by the height value in
				 * design units (heightCount entries),
				 * Followed by:
				 * Array of kern values corresponding
				 * to heights. (heightCount+1 entries).
				 */

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (2, math_ಮೌಲ್ಯ_Records_Z);
};

ಸಂರಚನಾ MathKernInfoRecord
{
  MathKernInfoRecord* copy (hb_serialize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base,
                            ಧ್ರುವ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ>> *variation_map) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    auto *out = c->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (nullptr);

    unsigned ಎಣಿಕೆ = ARRAY_LENGTH (mathKern);
    ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
      out->mathKern[i].serialize_copy (c, mathKern[i], base, 0, hb_serialize_context_t::Head, variation_map);

    return_trace (out);
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ auto &offset : mathKern)
      (base+offset).collect_variation_indices (c);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);

    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = ARRAY_LENGTH (mathKern);
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
      ಯದಿ (unlikely (!mathKern[i].sanitize (c, base)))
	return_trace (ಅಸತ್ಯ);

    return_trace (ಸತ್ಯ);
  }

  hb_position_t get_kerning (hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font,
			     ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ idx = kern;
    ಯದಿ (unlikely (idx >= ARRAY_LENGTH (mathKern))) ಪ್ರತ್ಯಾಗಮ 0;
    ಪ್ರತ್ಯಾಗಮ (base+mathKern[idx]).get_ಮೌಲ್ಯ (correction_height, font);
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_kernings (hb_ot_math_kern_t kern,
			     unsigned ಪೂರ್ಣಾಙ್ಕ start_offset,
			     unsigned ಪೂರ್ಣಾಙ್ಕ *entries_ಎಣಿಕೆ, /* IN/OUT */
			     hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			     hb_font_t *font,
			     ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ idx = kern;
    ಯದಿ (unlikely (idx >= ARRAY_LENGTH (mathKern)) || !mathKern[idx]) {
      ಯದಿ (entries_ಎಣಿಕೆ) *entries_ಎಣಿಕೆ = 0;
      ಪ್ರತ್ಯಾಗಮ 0;
    }
    ಪ್ರತ್ಯಾಗಮ (base+mathKern[idx]).get_entries (start_offset,
					     entries_ಎಣಿಕೆ,
					     kern_entries,
					     font);
  }

  ರಕ್ಷಿತ:
  /* Offset to MathKern table for each corner -
   * from the beginning of MathKernInfo table.  May be NULL. */
  Offset16To<MathKern> mathKern[4];

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (8);
};

ಸಂರಚನಾ MathKernInfo
{
  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಧ್ರುವ hb_set_t &glyphset = c->plan->_glyphset_mathed;
    ಧ್ರುವ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (ಏತತ್+mathKernCoverage, mathKernInfoRecords)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->mathKernInfoRecords, ಏತತ್, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->mathKernCoverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (ಸತ್ಯ);
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ auto &entry : hb_zip (ಏತತ್+mathKernCoverage, mathKernInfoRecords))
      ಯದಿ (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, ಏತತ್);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  mathKernCoverage.sanitize (c, ಏತತ್) &&
		  mathKernInfoRecords.sanitize (c, ಏತತ್));
  }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ index = (ಏತತ್+mathKernCoverage).get_coverage (glyph);
    ಪ್ರತ್ಯಾಗಮ mathKernInfoRecords[index].get_kerning (kern, correction_height, font, ಏತತ್);
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_kernings (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     unsigned ಪೂರ್ಣಾಙ್ಕ start_offset,
			     unsigned ಪೂರ್ಣಾಙ್ಕ *entries_ಎಣಿಕೆ, /* IN/OUT */
			     hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			     hb_font_t *font) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ index = (ಏತತ್+mathKernCoverage).get_coverage (glyph);
    ಪ್ರತ್ಯಾಗಮ mathKernInfoRecords[index].get_kernings (kern,
						    start_offset,
						    entries_ಎಣಿಕೆ,
						    kern_entries,
						    font,
						    ಏತತ್);
  }

  ರಕ್ಷಿತ:
  Offset16To<Coverage>
		mathKernCoverage;
				/* Offset to Coverage table -
				 * from the beginning of the
				 * MathKernInfo table. */
  Array16Of<MathKernInfoRecord>
		mathKernInfoRecords;
				/* Array of MathKernInfoRecords,
				 * per-glyph information for
				 * mathematical positioning
				 * of subscripts and
				 * superscripts. */

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (4, mathKernInfoRecords);
};

ಸಂರಚನಾ MathGlyphInfo
{
  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->embed (*ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    out->mathItalicsCorrectionInfo.serialize_subset (c, mathItalicsCorrectionInfo, ಏತತ್);
    out->math_ತುದಿ_Accent_Attachment.serialize_subset (c, math_ತುದಿ_Accent_Attachment, ಏತತ್);

    ಧ್ರುವ hb_set_t &glyphset = c->plan->_glyphset_mathed;
    ಧ್ರುವ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto it =
    + hb_iter (ಏತತ್+extendedShapeCoverage)
    | hb_take (c->plan->source->get_ಅಂಕೆ_glyphs ())
    | hb_filter (glyphset)
    | hb_map_retains_sorting (glyph_map)
    ;

    ಯದಿ (it) out->extendedShapeCoverage.serialize_serialize (c->serializer, it);
    ಅನ್ಯಥಾ out->extendedShapeCoverage = 0;

    out->mathKernInfo.serialize_subset (c, mathKernInfo, ಏತತ್);
    return_trace (ಸತ್ಯ);
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ
  {
    (ಏತತ್+mathItalicsCorrectionInfo).collect_variation_indices (c);
    (ಏತತ್+math_ತುದಿ_Accent_Attachment).collect_variation_indices (c);
    (ಏತತ್+mathKernInfo).collect_variation_indices (c);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  mathItalicsCorrectionInfo.sanitize (c, ಏತತ್) &&
		  math_ತುದಿ_Accent_Attachment.sanitize (c, ಏತತ್) &&
		  extendedShapeCoverage.sanitize (c, ಏತತ್) &&
		  mathKernInfo.sanitize (c, ಏತತ್));
  }

  hb_position_t
  get_italics_correction (hb_codepoint_t  glyph, hb_font_t *font) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (ಏತತ್+mathItalicsCorrectionInfo).get_ಮೌಲ್ಯ (glyph, font); }

  hb_position_t
  get_ತುದಿ_accent_attachment (hb_codepoint_t  glyph, hb_font_t *font) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (ಏತತ್+math_ತುದಿ_Accent_Attachment).get_ಮೌಲ್ಯ (glyph, font); }

  ತಾರ್ಕಿಕ is_extended_shape (hb_codepoint_t glyph) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (ಏತತ್+extendedShapeCoverage).get_coverage (glyph) != NOT_COVERED; }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (ಏತತ್+mathKernInfo).get_kerning (glyph, kern, correction_height, font); }

  hb_position_t get_kernings (hb_codepoint_t glyph,
			      hb_ot_math_kern_t kern,
			      unsigned ಪೂರ್ಣಾಙ್ಕ start_offset,
			      unsigned ಪೂರ್ಣಾಙ್ಕ *entries_ಎಣಿಕೆ, /* IN/OUT */
			      hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			      hb_font_t *font) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (ಏತತ್+mathKernInfo).get_kernings (glyph,
					     kern,
					     start_offset,
					     entries_ಎಣಿಕೆ,
					     kern_entries,
					     font); }

  ರಕ್ಷಿತ:
  /* Offset to MathItalicsCorrectionInfo table -
   * from the beginning of MathGlyphInfo table. */
  Offset16To<MathItalicsCorrectionInfo> mathItalicsCorrectionInfo;

  /* Offset to MathTopAccentAttachment table -
   * from the beginning of MathGlyphInfo table. */
  Offset16To<Math_ತುದಿ_Accent_Attachment> math_ತುದಿ_Accent_Attachment;

  /* Offset to coverage table for Extended Shape glyphs -
   * from the beginning of MathGlyphInfo table. When the left or right glyph of
   * a box is an extended shape variant, the (ink) box (and not the default
   * position defined by values in MathConstants table) should be used for
   * vertical positioning purposes.  May be NULL.. */
  Offset16To<Coverage> extendedShapeCoverage;

   /* Offset to MathKernInfo table -
    * from the beginning of MathGlyphInfo table. */
  Offset16To<MathKernInfo> mathKernInfo;

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (8);
};

ಸಂರಚನಾ MathGlyphVariantRecord
{
  friend ಸಂರಚನಾ MathGlyphConstruction;

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    ಧ್ರುವ hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_assign (out->variantGlyph, glyph_map.get (variantGlyph), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ಶೂನ್ಯ depend (hb_depend_data_builder_t *depend_data, unsigned source) ಧ್ರುವ
  { depend_data->add_depend(source, HB_OT_TAG_MATH, variantGlyph); }

  ಶೂನ್ಯ closure_glyphs (hb_set_t *variant_glyphs) ಧ್ರುವ
  { variant_glyphs->add (variantGlyph); }

  ರಕ್ಷಿತ:
  HBGlyphID16 variantGlyph;       /* Glyph ID for the variant. */
  HBUINT16  advanceMeasurement; /* Advance width/height, in design units, of the
				 * variant, in the direction of requested
				 * glyph extension. */

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4);
};

ಸಂರಚನಾ PartFlags : HBUINT16
{
  ಗಣನಾ Flags {
    Extender	= 0x0001u, /* If set, the part can be skipped or repeated. */

    Defined	= 0x0001u, /* All defined flags. */
  };

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (2);
};

ಸಂರಚನಾ MathGlyphPartRecord
{
  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    ಧ್ರುವ hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_assign (out->glyph, glyph_map.get (glyph), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ಶೂನ್ಯ extract (hb_ot_math_glyph_part_t &out,
		int64_t mult,
		hb_font_t *font) ಧ್ರುವ
  {
    out.glyph			= glyph;

    out.start_connector_length	= font->em_mult (startConnectorLength, mult);
    out.end_connector_length	= font->em_mult (endConnectorLength, mult);
    out.full_advance		= font->em_mult (fullAdvance, mult);

    static_assert ((unsigned ಪೂರ್ಣಾಙ್ಕ) HB_OT_MATH_GLYPH_PART_FLAG_EXTENDER ==
		   (unsigned ಪೂರ್ಣಾಙ್ಕ) PartFlags::Extender, "");

    out.flags = (hb_ot_math_glyph_part_flags_t)
		(unsigned ಪೂರ್ಣಾಙ್ಕ)
		(partFlags & PartFlags::Defined);
  }

  ಶೂನ್ಯ depend (hb_depend_data_builder_t *depend_data, unsigned source) ಧ್ರುವ
  { depend_data->add_depend(source, HB_OT_TAG_MATH, glyph); }

  ಶೂನ್ಯ closure_glyphs (hb_set_t *variant_glyphs) ಧ್ರುವ
  { variant_glyphs->add (glyph); }

  ರಕ್ಷಿತ:
  HBGlyphID16	glyph;		/* Glyph ID for the part. */
  HBUINT16	startConnectorLength;
				/* Advance width/ height of the straight bar
				 * connector material, in design units, is at
				 * the beginning of the glyph, in the
				 * direction of the extension. */
  HBUINT16	endConnectorLength;
				/* Advance width/ height of the straight bar
				 * connector material, in design units, is at
				 * the end of the glyph, in the direction of
				 * the extension. */
  HBUINT16	fullAdvance;	/* Full advance width/height for this part,
				 * in the direction of the extension.
				 * In design units. */
  PartFlags	partFlags;	/* Part qualifiers. */

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (10);
};

ಸಂರಚನಾ MathGlyphAssembly
{
  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);

    ಯದಿ (!c->serializer->copy (italicsCorrection, ಏತತ್, &c->plan->layout_variation_idx_delta_map)) return_trace (ಅಸತ್ಯ);
    ಯದಿ (!c->serializer->copy<HBUINT16> (partRecords.len)) return_trace (ಅಸತ್ಯ);

    ಪ್ರತಿ (ಧ್ರುವ auto& record : partRecords.iter ())
      ಯದಿ (!record.subset (c)) return_trace (ಅಸತ್ಯ);
    return_trace (ಸತ್ಯ);
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ
  {
    italicsCorrection.collect_variation_indices (c, ಏತತ್);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  italicsCorrection.sanitize (c, ಏತತ್) &&
		  partRecords.sanitize (c));
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_parts (hb_direction_t direction,
			  hb_font_t *font,
			  unsigned ಪೂರ್ಣಾಙ್ಕ start_offset,
			  unsigned ಪೂರ್ಣಾಙ್ಕ *parts_ಎಣಿಕೆ, /* IN/OUT */
			  hb_ot_math_glyph_part_t *parts /* OUT */,
			  hb_position_t *italics_correction /* OUT */) ಧ್ರುವ
  {
    ಯದಿ (parts_ಎಣಿಕೆ && parts)
    {
      int64_t mult = font->dir_mult (direction);
      ಪ್ರತಿ (auto _ : hb_zip (partRecords.as_array ().sub_array (start_offset, parts_ಎಣಿಕೆ),
			    hb_array (parts, *parts_ಎಣಿಕೆ)))
	_.first.extract (_.second, mult, font);
    }

    ಯದಿ (italics_correction)
      *italics_correction = italicsCorrection.get_x_ಮೌಲ್ಯ (font, ಏತತ್);

    ಪ್ರತ್ಯಾಗಮ partRecords.len;
  }

  ಶೂನ್ಯ depend (hb_depend_data_builder_t *depend_data, unsigned source) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ auto& _ : partRecords.iter ())
      _.depend (depend_data, source);
  }

  ಶೂನ್ಯ closure_glyphs (hb_set_t *variant_glyphs) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ auto& _ : partRecords.iter ())
      _.closure_glyphs (variant_glyphs);
  }

  ರಕ್ಷಿತ:
  Math_ಮೌಲ್ಯ_Record
		italicsCorrection;
				/* Italics correction of this
				 * MathGlyphAssembly. Should not
				 * depend on the assembly size. */
  Array16Of<MathGlyphPartRecord>
		partRecords;	/* Array of part records, from
				 * left to right and bottom to
				 * top. */

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (6, partRecords);
};

ಸಂರಚನಾ MathGlyphConstruction
{
  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);

    out->glyphAssembly.serialize_subset (c, glyphAssembly, ಏತತ್);

    ಯದಿ (!c->serializer->check_assign (out->mathGlyphVariantRecord.len, mathGlyphVariantRecord.len, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (ಅಸತ್ಯ);
    ಪ್ರತಿ (ಧ್ರುವ auto& record : mathGlyphVariantRecord.iter ())
      ಯದಿ (!record.subset (c)) return_trace (ಅಸತ್ಯ);

    return_trace (ಸತ್ಯ);
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ
  {
    (ಏತತ್+glyphAssembly).collect_variation_indices (c);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  glyphAssembly.sanitize (c, ಏತತ್) &&
		  mathGlyphVariantRecord.sanitize (c));
  }

  ಧ್ರುವ MathGlyphAssembly &get_assembly () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಏತತ್+glyphAssembly; }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_variants (hb_direction_t direction,
			     hb_font_t *font,
			     unsigned ಪೂರ್ಣಾಙ್ಕ start_offset,
			     unsigned ಪೂರ್ಣಾಙ್ಕ *variants_ಎಣಿಕೆ, /* IN/OUT */
			     hb_ot_math_glyph_variant_t *variants /* OUT */) ಧ್ರುವ
  {
    ಯದಿ (variants_ಎಣಿಕೆ && variants)
    {
      int64_t mult = font->dir_mult (direction);
      ಪ್ರತಿ (auto _ : hb_zip (mathGlyphVariantRecord.as_array ().sub_array (start_offset, variants_ಎಣಿಕೆ),
			    hb_array (variants, *variants_ಎಣಿಕೆ)))
	_.second = {_.first.variantGlyph, font->em_mult (_.first.advanceMeasurement, mult)};
    }
    ಪ್ರತ್ಯಾಗಮ mathGlyphVariantRecord.len;
  }

  ಶೂನ್ಯ depend (hb_depend_data_builder_t *depend_data, unsigned source) ಧ್ರುವ
  {
    (ಏತತ್+glyphAssembly).depend (depend_data, source);

    ಪ್ರತಿ (ಧ್ರುವ auto& _ : mathGlyphVariantRecord.iter ())
      _.depend (depend_data, source);
  }

  ಶೂನ್ಯ closure_glyphs (hb_set_t *variant_glyphs) ಧ್ರುವ
  {
    (ಏತತ್+glyphAssembly).closure_glyphs (variant_glyphs);

    ಪ್ರತಿ (ಧ್ರುವ auto& _ : mathGlyphVariantRecord.iter ())
      _.closure_glyphs (variant_glyphs);
  }

  ರಕ್ಷಿತ:
  /* Offset to MathGlyphAssembly table for this shape - from the beginning of
     MathGlyphConstruction table.  May be NULL. */
  Offset16To<MathGlyphAssembly>	  glyphAssembly;

  /* MathGlyphVariantRecords for alternative variants of the glyphs. */
  Array16Of<MathGlyphVariantRecord> mathGlyphVariantRecord;

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (4, mathGlyphVariantRecord);
};

ಸಂರಚನಾ MathVariants
{
  ಶೂನ್ಯ depend (hb_depend_data_builder_t *depend_data) ಧ್ರುವ
  {
    ಧ್ರುವ hb_array_t<ಧ್ರುವ Offset16To<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vert_Glyph_ಎಣಿಕೆ + horiz_Glyph_ಎಣಿಕೆ);

    ಯದಿ (vertGlyphCoverage)
    {
      ಧ್ರುವ auto vert_offsets = glyph_construction_offsets.sub_array (0, vert_Glyph_ಎಣಿಕೆ);
      + hb_zip (ಏತತ್+vertGlyphCoverage, vert_offsets)
      | hb_apply ([&] (ಧ್ರುವ hb_pair_t<hb_codepoint_t, ಧ್ರುವ Offset16To<MathGlyphConstruction>&> &_)
                  {
                    ಧ್ರುವ MathGlyphConstruction &mgc = ಏತತ್+_.second;
                    mgc.depend (depend_data, _.first);
                  })
      ;
    }
    ಯದಿ (horizGlyphCoverage)
    {
      ಧ್ರುವ auto hori_offsets = glyph_construction_offsets.sub_array (vert_Glyph_ಎಣಿಕೆ, horiz_Glyph_ಎಣಿಕೆ);
      + hb_zip (ಏತತ್+horizGlyphCoverage, hori_offsets)
      | hb_apply ([&] (ಧ್ರುವ hb_pair_t<hb_codepoint_t, ಧ್ರುವ Offset16To<MathGlyphConstruction>&> &_)
                  {
                    ಧ್ರುವ MathGlyphConstruction &mgc = ಏತತ್+_.second;
                    mgc.depend (depend_data, _.first);
                  })
      ;
    }
  }

  ಶೂನ್ಯ closure_glyphs (ಧ್ರುವ hb_set_t *glyph_set,
                       hb_set_t *variant_glyphs) ಧ್ರುವ
  {
    ಧ್ರುವ hb_array_t<ಧ್ರುವ Offset16To<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vert_Glyph_ಎಣಿಕೆ + horiz_Glyph_ಎಣಿಕೆ);

    ಯದಿ (vertGlyphCoverage)
    {
      ಧ್ರುವ auto vert_offsets = glyph_construction_offsets.sub_array (0, vert_Glyph_ಎಣಿಕೆ);
      + hb_zip (ಏತತ್+vertGlyphCoverage, vert_offsets)
      | hb_filter (glyph_set, hb_first)
      | hb_map (hb_second)
      | hb_map (hb_add (ಏತತ್))
      | hb_apply ([=] (ಧ್ರುವ MathGlyphConstruction &_) { _.closure_glyphs (variant_glyphs); })
      ;
    }

    ಯದಿ (horizGlyphCoverage)
    {
      ಧ್ರುವ auto hori_offsets = glyph_construction_offsets.sub_array (vert_Glyph_ಎಣಿಕೆ, horiz_Glyph_ಎಣಿಕೆ);
      + hb_zip (ಏತತ್+horizGlyphCoverage, hori_offsets)
      | hb_filter (glyph_set, hb_first)
      | hb_map (hb_second)
      | hb_map (hb_add (ಏತತ್))
      | hb_apply ([=] (ಧ್ರುವ MathGlyphConstruction &_) { _.closure_glyphs (variant_glyphs); })
      ;
    }
  }

  ಶೂನ್ಯ collect_coverage_and_indices (hb_sorted_vector_t<hb_codepoint_t>& new_coverage,
                                     ಧ್ರುವ Offset16To<Coverage>& coverage,
                                     unsigned i,
                                     unsigned end_index,
                                     hb_set_t& indices,
                                     ಧ್ರುವ hb_set_t& glyphset,
                                     ಧ್ರುವ hb_map_t& glyph_map) ಧ್ರುವ
  {
    ಯದಿ (!coverage) ಪ್ರತ್ಯಾಗಮ;

    ಪ್ರತಿ (ಧ್ರುವ auto _ : (ಏತತ್+coverage).iter ())
    {
      ಯದಿ (i >= end_index) ಪ್ರತ್ಯಾಗಮ;
      ಯದಿ (glyphset.has (_))
      {
        unsigned new_gid = glyph_map.get (_);
        new_coverage.push (new_gid);
        indices.add (i);
      }
      i++;
    }
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಧ್ರುವ hb_set_t &glyphset = c->plan->_glyphset_cmaped;
    ಧ್ರುವ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);
    ಯದಿ (!c->serializer->check_assign (out->minConnectorOverlap, minConnectorOverlap, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (ಅಸತ್ಯ);

    hb_sorted_vector_t<hb_codepoint_t> new_vert_coverage;
    hb_sorted_vector_t<hb_codepoint_t> new_hori_coverage;
    hb_set_t indices;
    collect_coverage_and_indices (new_vert_coverage, vertGlyphCoverage, 0, vert_Glyph_ಎಣಿಕೆ, indices, glyphset, glyph_map);
    collect_coverage_and_indices (new_hori_coverage, horizGlyphCoverage, vert_Glyph_ಎಣಿಕೆ, vert_Glyph_ಎಣಿಕೆ + horiz_Glyph_ಎಣಿಕೆ, indices, glyphset, glyph_map);

    ಯದಿ (!c->serializer->check_assign (out->vert_Glyph_ಎಣಿಕೆ, new_vert_coverage.length, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (ಅಸತ್ಯ);
    ಯದಿ (!c->serializer->check_assign (out->horiz_Glyph_ಎಣಿಕೆ, new_hori_coverage.length, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (ಅಸತ್ಯ);

    ಪ್ರತಿ (unsigned i : indices.iter ())
    {
      auto *o = c->serializer->embed (glyphConstruction[i]);
      ಯದಿ (!o) return_trace (ಅಸತ್ಯ);
      o->serialize_subset (c, glyphConstruction[i], ಏತತ್);
    }

    ಯದಿ (new_vert_coverage)
      out->vertGlyphCoverage.serialize_serialize (c->serializer, new_vert_coverage.iter ());

    ಯದಿ (new_hori_coverage)
    out->horizGlyphCoverage.serialize_serialize (c->serializer, new_hori_coverage.iter ());
    return_trace (ಸತ್ಯ);
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ auto &entry : hb_zip (ಏತತ್+vertGlyphCoverage, glyphConstruction.as_array (vert_Glyph_ಎಣಿಕೆ)))
      ಯದಿ (c->glyph_set->has (hb_first (entry)))
        (ಏತತ್+hb_second (entry)).collect_variation_indices (c);
    ಪ್ರತಿ (ಧ್ರುವ auto &entry : hb_zip (ಏತತ್+horizGlyphCoverage, glyphConstruction.as_array (vert_Glyph_ಎಣಿಕೆ + horiz_Glyph_ಎಣಿಕೆ).sub_array (vert_Glyph_ಎಣಿಕೆ)))
      ಯದಿ (c->glyph_set->has (hb_first (entry)))
        (ಏತತ್+hb_second (entry)).collect_variation_indices (c);
  }

  ತಾರ್ಕಿಕ sanitize_offsets (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = vert_Glyph_ಎಣಿಕೆ + horiz_Glyph_ಎಣಿಕೆ;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
      ಯದಿ (!glyphConstruction.arrayZ[i].sanitize (c, ಏತತ್)) return_trace (ಅಸತ್ಯ);
    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  vertGlyphCoverage.sanitize (c, ಏತತ್) &&
		  horizGlyphCoverage.sanitize (c, ಏತತ್) &&
		  hb_barrier () &&
		  c->check_array (glyphConstruction.arrayZ, vert_Glyph_ಎಣಿಕೆ + horiz_Glyph_ಎಣಿಕೆ) &&
		  sanitize_offsets (c));
  }

  hb_position_t get_min_connector_overlap (hb_direction_t direction,
						  hb_font_t *font) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ font->em_scale_dir (minConnectorOverlap, direction); }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_glyph_variants (hb_codepoint_t glyph,
				   hb_direction_t direction,
				   hb_font_t *font,
				   unsigned ಪೂರ್ಣಾಙ್ಕ start_offset,
				   unsigned ಪೂರ್ಣಾಙ್ಕ *variants_ಎಣಿಕೆ, /* IN/OUT */
				   hb_ot_math_glyph_variant_t *variants /* OUT */) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ get_glyph_construction (glyph, direction, font)
	   .get_variants (direction, font, start_offset, variants_ಎಣಿಕೆ, variants); }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_glyph_parts (hb_codepoint_t glyph,
				hb_direction_t direction,
				hb_font_t *font,
				unsigned ಪೂರ್ಣಾಙ್ಕ start_offset,
				unsigned ಪೂರ್ಣಾಙ್ಕ *parts_ಎಣಿಕೆ, /* IN/OUT */
				hb_ot_math_glyph_part_t *parts /* OUT */,
				hb_position_t *italics_correction /* OUT */) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ get_glyph_construction (glyph, direction, font)
	   .get_assembly ()
	   .get_parts (direction, font,
		       start_offset, parts_ಎಣಿಕೆ, parts,
		       italics_correction); }

  ಗುಪ್ತ:
  ಧ್ರುವ MathGlyphConstruction &
  get_glyph_construction (hb_codepoint_t glyph,
			  hb_direction_t direction,
			  hb_font_t *font HB_UNUSED) ಧ್ರುವ
  {
    ತಾರ್ಕಿಕ vertical = HB_DIRECTION_IS_VERTICAL (direction);
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = vertical ? vert_Glyph_ಎಣಿಕೆ : horiz_Glyph_ಎಣಿಕೆ;
    ಧ್ರುವ Offset16To<Coverage> &coverage = vertical ? vertGlyphCoverage
						  : horizGlyphCoverage;

    unsigned ಪೂರ್ಣಾಙ್ಕ index = (ಏತತ್+coverage).get_coverage (glyph);
    ಯದಿ (unlikely (index >= ಎಣಿಕೆ)) ಪ್ರತ್ಯಾಗಮ Null (MathGlyphConstruction);

    ಯದಿ (!vertical)
      index += vert_Glyph_ಎಣಿಕೆ;

    ಪ್ರತ್ಯಾಗಮ ಏತತ್+glyphConstruction[index];
  }

  ರಕ್ಷಿತ:
  HBUINT16	minConnectorOverlap;
				/* Minimum overlap of connecting
				 * glyphs during glyph construction,
				 * in design units. */
  Offset16To<Coverage> vertGlyphCoverage;
				/* Offset to Coverage table -
				 * from the beginning of MathVariants
				 * table. */
  Offset16To<Coverage> horizGlyphCoverage;
				/* Offset to Coverage table -
				 * from the beginning of MathVariants
				 * table. */
  HBUINT16	vert_Glyph_ಎಣಿಕೆ;	/* Number of glyphs for which
				 * information is provided for
				 * vertically growing variants. */
  HBUINT16	horiz_Glyph_ಎಣಿಕೆ;/* Number of glyphs for which
				 * information is provided for
				 * horizontally growing variants. */

  /* Array of offsets to MathGlyphConstruction tables - from the beginning of
     the MathVariants table, for shapes growing in vertical/horizontal
     direction. */
  UnsizedArrayOf<Offset16To<MathGlyphConstruction>>
			glyphConstruction;

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (10, glyphConstruction);
};


/*
 * MATH -- Mathematical typesetting
 * https://docs.microsoft.com/en-us/typography/opentype/spec/math
 */

ಸಂರಚನಾ MATH
{
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_MATH;

  ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ version.to_int (); }

  ಶೂನ್ಯ depend (hb_depend_data_builder_t *depend_data) ಧ್ರುವ
  {
    ಯದಿ (mathVariants)
      (ಏತತ್+mathVariants).depend (depend_data);
  }

  ಶೂನ್ಯ closure_glyphs (hb_set_t *glyph_set) ಧ್ರುವ
  {
    ಯದಿ (mathVariants)
    {
      hb_set_t variant_glyphs;
      (ಏತತ್+mathVariants).closure_glyphs (glyph_set, &variant_glyphs);
      hb_set_union (glyph_set, &variant_glyphs);
    }
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->embed (*ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    out->mathConstants.serialize_copy (c->serializer, mathConstants, ಏತತ್, 0, hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map);
    out->mathGlyphInfo.serialize_subset (c, mathGlyphInfo, ಏತತ್);
    out->mathVariants.serialize_subset (c, mathVariants, ಏತತ್);
    return_trace (ಸತ್ಯ);
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c, ಧ್ರುವ hb_set_t *variant_glyph_set) ಧ್ರುವ
  {
    (ಏತತ್+mathConstants).collect_variation_indices (c);
    (ಏತತ್+mathGlyphInfo).collect_variation_indices (c);
    auto *glyph_set = c->glyph_set;
    c->glyph_set = variant_glyph_set;
    (ಏತತ್+mathVariants).collect_variation_indices (c);
    c->glyph_set = glyph_set;
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (version.sanitize (c) &&
		  likely (version.major == 1) &&
		  hb_barrier () &&
		  mathConstants.sanitize (c, ಏತತ್) &&
		  mathGlyphInfo.sanitize (c, ಏತತ್) &&
		  mathVariants.sanitize (c, ಏತತ್));
  }

  // https://github.com/harfbuzz/harfbuzz/issues/4653
  HB_INTERNAL ತಾರ್ಕಿಕ is_bad_cambria (hb_font_t *font) ಧ್ರುವ
  {
#ifndef HB_NO_MATH
    ವಿಕಲ್ಪ HB_CODEPOINT_ENCODE3 (font->face->table.MATH.get_blob ()->length,
                                 (ಏತತ್+mathConstants).minHeight[1], // displayOperatorMinHeight
                                 (ಏತತ್+mathConstants).minHeight[0]) // delimitedSubFormulaMinHeight
    {
      /* sha1sum:ab4a4fe054d23061f3c039493d6f665cfda2ecf5  cambria.ttc
       * sha1sum:086855301bff644f9d8827b88491fcf73a6d4cb9  cambria.ttc
       * sha1sum:b1e5a3feaca2ea3dfcf79ccb377de749ecf60343  cambria.ttc */
      ಪಕ್ಷ HB_CODEPOINT_ENCODE3 (25722, 2500, 3000):
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
#endif
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  hb_position_t get_constant (hb_ot_math_constant_t  constant,
			      hb_font_t		   *font) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (ಏತತ್+mathConstants).get_ಮೌಲ್ಯ (constant, font); }

  ಧ್ರುವ MathGlyphInfo &get_glyph_info () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಏತತ್+mathGlyphInfo; }

  ಧ್ರುವ MathVariants &get_variants () ಧ್ರುವ    { ಪ್ರತ್ಯಾಗಮ ಏತತ್+mathVariants; }

  ರಕ್ಷಿತ:
  FixedVersion<>version;	/* Version of the MATH table
				 * initially set to 0x00010000u */
  Offset16To<MathConstants>
		mathConstants;	/* MathConstants table */
  Offset16To<MathGlyphInfo>
		mathGlyphInfo;	/* MathGlyphInfo table */
  Offset16To<MathVariants>
		mathVariants;	/* MathVariants table */

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (10);
};

} /* namespace OT */


#endif /* HB_OT_MATH_TABLE_HH */

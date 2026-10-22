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

#ఒకవేళ !defined(HB_OT_H_లోపల) && !defined(HB_NO_SINGLE_శీర్షిక_ERROR)
#దోషం "Include <hb-ot.h> instead."
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_OT_MATH_H
#నిర్వచించు HB_OT_MATH_H

#అన్తర్భావ "hb.h"

HB_BEGIN_DECLS


/*
 * MATH
 */

/**
 * HB_OT_TAG_MATH:
 *
 * OpenType [Mathematical Typesetting Table](https://docs.microsoft.com/en-us/typography/opentype/spec/math).
 *
 * Since: 1.3.3
 */
#నిర్వచించు HB_OT_ట్యాగ్_MATH HB_ట్యాగ్('M','A','T','H')

/**
 * HB_OT_TAG_MATH_SCRIPT:
 *
 * OpenType script tag, `math`, for features specific to math shaping.
 *
 * <note>#HB_OT_TAG_MATH_SCRIPT is not a valid #hb_script_t and should only be
 * used with functions that accept raw OpenType script tags, such as
 * #hb_ot_layout_collect_features. In other cases, #HB_SCRIPT_MATH should be
 * used instead.</note>
 *
 * Since: 3.4.0
 */
#నిర్వచించు HB_OT_ట్యాగ్_MATH_SCRIPT HB_ట్యాగ్('m','a','t','h')

/* Types */

/**
 * hb_ot_math_constant_t:
 * @HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_DOWN: scriptPercentScaleDown
 * @HB_OT_MATH_CONSTANT_SCRIPT_SCRIPT_PERCENT_SCALE_DOWN: scriptScriptPercentScaleDown
 * @HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_HEIGHT: delimitedSubFormulaMinHeight
 * @HB_OT_MATH_CONSTANT_DISPLAY_OPERATOR_MIN_HEIGHT: displayOperatorMinHeight
 * @HB_OT_MATH_CONSTANT_MATH_LEADING: mathLeading
 * @HB_OT_MATH_CONSTANT_AXIS_HEIGHT: axisHeight
 * @HB_OT_MATH_CONSTANT_ACCENT_BASE_HEIGHT: accentBaseHeight
 * @HB_OT_MATH_CONSTANT_FLATTENED_ACCENT_BASE_HEIGHT: flattenedAccentBaseHeight
 * @HB_OT_MATH_CONSTANT_SUBSCRIPT_SHIFT_DOWN: subscriptShiftDown
 * @HB_OT_MATH_CONSTANT_SUBSCRIPT_TOP_MAX: subscriptTopMax
 * @HB_OT_MATH_CONSTANT_SUBSCRIPT_BASELINE_DROP_MIN: subscriptBaselineDropMin
 * @HB_OT_MATH_CONSTANT_SUPERSCRIPT_SHIFT_UP: superscriptShiftUp
 * @HB_OT_MATH_CONSTANT_SUPERSCRIPT_SHIFT_UP_CRAMPED: superscriptShiftUpCramped
 * @HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MIN: superscriptBottomMin
 * @HB_OT_MATH_CONSTANT_SUPERSCRIPT_BASELINE_DROP_MAX: superscriptBaselineDropMax
 * @HB_OT_MATH_CONSTANT_SUB_SUPERSCRIPT_GAP_MIN: subSuperscriptGapMin
 * @HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MAX_WITH_SUBSCRIPT: superscriptBottomMaxWithSubscript
 * @HB_OT_MATH_CONSTANT_SPACE_AFTER_SCRIPT: spaceAfterScript
 * @HB_OT_MATH_CONSTANT_UPPER_LIMIT_GAP_MIN: upperLimitGapMin
 * @HB_OT_MATH_CONSTANT_UPPER_LIMIT_BASELINE_RISE_MIN: upperLimitBaselineRiseMin
 * @HB_OT_MATH_CONSTANT_LOWER_LIMIT_GAP_MIN: lowerLimitGapMin
 * @HB_OT_MATH_CONSTANT_LOWER_LIMIT_BASELINE_DROP_MIN: lowerLimitBaselineDropMin
 * @HB_OT_MATH_CONSTANT_STACK_TOP_SHIFT_UP: stackTopShiftUp
 * @HB_OT_MATH_CONSTANT_STACK_TOP_DISPLAY_STYLE_SHIFT_UP: stackTopDisplayStyleShiftUp
 * @HB_OT_MATH_CONSTANT_STACK_BOTTOM_SHIFT_DOWN: stackBottomShiftDown
 * @HB_OT_MATH_CONSTANT_STACK_BOTTOM_DISPLAY_STYLE_SHIFT_DOWN: stackBottomDisplayStyleShiftDown
 * @HB_OT_MATH_CONSTANT_STACK_GAP_MIN: stackGapMin
 * @HB_OT_MATH_CONSTANT_STACK_DISPLAY_STYLE_GAP_MIN: stackDisplayStyleGapMin
 * @HB_OT_MATH_CONSTANT_STRETCH_STACK_TOP_SHIFT_UP: stretchStackTopShiftUp
 * @HB_OT_MATH_CONSTANT_STRETCH_STACK_BOTTOM_SHIFT_DOWN: stretchStackBottomShiftDown
 * @HB_OT_MATH_CONSTANT_STRETCH_STACK_GAP_ABOVE_MIN: stretchStackGapAboveMin
 * @HB_OT_MATH_CONSTANT_STRETCH_STACK_GAP_BELOW_MIN: stretchStackGapBelowMin
 * @HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_SHIFT_UP: fractionNumeratorShiftUp
 * @HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_DISPLAY_STYLE_SHIFT_UP: fractionNumeratorDisplayStyleShiftUp
 * @HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_SHIFT_DOWN: fractionDenominatorShiftDown
 * @HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_DISPLAY_STYLE_SHIFT_DOWN: fractionDenominatorDisplayStyleShiftDown
 * @HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_GAP_MIN: fractionNumeratorGapMin
 * @HB_OT_MATH_CONSTANT_FRACTION_NUM_DISPLAY_STYLE_GAP_MIN: fractionNumDisplayStyleGapMin
 * @HB_OT_MATH_CONSTANT_FRACTION_RULE_THICKNESS: fractionRuleThickness
 * @HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_GAP_MIN: fractionDenominatorGapMin
 * @HB_OT_MATH_CONSTANT_FRACTION_DENOM_DISPLAY_STYLE_GAP_MIN: fractionDenomDisplayStyleGapMin
 * @HB_OT_MATH_CONSTANT_SKEWED_FRACTION_HORIZONTAL_GAP: skewedFractionHorizontalGap
 * @HB_OT_MATH_CONSTANT_SKEWED_FRACTION_VERTICAL_GAP: skewedFractionVerticalGap
 * @HB_OT_MATH_CONSTANT_OVERBAR_VERTICAL_GAP: overbarVerticalGap
 * @HB_OT_MATH_CONSTANT_OVERBAR_RULE_THICKNESS: overbarRuleThickness
 * @HB_OT_MATH_CONSTANT_OVERBAR_EXTRA_ASCENDER: overbarExtraAscender
 * @HB_OT_MATH_CONSTANT_UNDERBAR_VERTICAL_GAP: underbarVerticalGap
 * @HB_OT_MATH_CONSTANT_UNDERBAR_RULE_THICKNESS: underbarRuleThickness
 * @HB_OT_MATH_CONSTANT_UNDERBAR_EXTRA_DESCENDER: underbarExtraDescender
 * @HB_OT_MATH_CONSTANT_RADICAL_VERTICAL_GAP: radicalVerticalGap
 * @HB_OT_MATH_CONSTANT_RADICAL_DISPLAY_STYLE_VERTICAL_GAP: radicalDisplayStyleVerticalGap
 * @HB_OT_MATH_CONSTANT_RADICAL_RULE_THICKNESS: radicalRuleThickness
 * @HB_OT_MATH_CONSTANT_RADICAL_EXTRA_ASCENDER: radicalExtraAscender
 * @HB_OT_MATH_CONSTANT_RADICAL_KERN_BEFORE_DEGREE: radicalKernBeforeDegree
 * @HB_OT_MATH_CONSTANT_RADICAL_KERN_AFTER_DEGREE: radicalKernAfterDegree
 * @HB_OT_MATH_CONSTANT_RADICAL_DEGREE_BOTTOM_RAISE_PERCENT: radicalDegreeBottomRaisePercent
 *
 * The 'MATH' table constants, refer to
 * [OpenType documentation](https://docs.microsoft.com/en-us/typography/opentype/spec/math#mathconstants-table)
 * For more explanations.
 *
 * Since: 1.3.3
 */
రకనిర్వచనం గణన {
  HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_కిందికి = 0,
  HB_OT_MATH_CONSTANT_SCRIPT_SCRIPT_PERCENT_SCALE_కిందికి = 1,
  HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_ఎత్తు = 2,
  HB_OT_MATH_CONSTANT_DISPLAY_OPERATOR_MIN_ఎత్తు = 3,
  HB_OT_MATH_CONSTANT_MATH_LEADING = 4,
  HB_OT_MATH_CONSTANT_AXIS_ఎత్తు = 5,
  HB_OT_MATH_CONSTANT_ACCENT_పునాది_ఎత్తు = 6,
  HB_OT_MATH_CONSTANT_FLATTENED_ACCENT_పునాది_ఎత్తు = 7,
  HB_OT_MATH_CONSTANT_SUBSCRIPT_జరుపు_కిందికి = 8,
  HB_OT_MATH_CONSTANT_SUBSCRIPT_శిఖరం_MAX = 9,
  HB_OT_MATH_CONSTANT_SUBSCRIPT_BASELINE_DROP_MIN = 10,
  HB_OT_MATH_CONSTANT_SUPERSCRIPT_జరుపు_పైకి = 11,
  HB_OT_MATH_CONSTANT_SUPERSCRIPT_జరుపు_పైకి_CRAMPED = 12,
  HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MIN = 13,
  HB_OT_MATH_CONSTANT_SUPERSCRIPT_BASELINE_DROP_MAX = 14,
  HB_OT_MATH_CONSTANT_SUB_SUPERSCRIPT_GAP_MIN = 15,
  HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MAX_WITH_SUBSCRIPT = 16,
  HB_OT_MATH_CONSTANT_SPACE_AFTER_SCRIPT = 17,
  HB_OT_MATH_CONSTANT_పై_హద్దు_GAP_MIN = 18,
  HB_OT_MATH_CONSTANT_పై_హద్దు_BASELINE_RISE_MIN = 19,
  HB_OT_MATH_CONSTANT_క్రింది_హద్దు_GAP_MIN = 20,
  HB_OT_MATH_CONSTANT_క్రింది_హద్దు_BASELINE_DROP_MIN = 21,
  HB_OT_MATH_CONSTANT_దొంతర_శిఖరం_జరుపు_పైకి = 22,
  HB_OT_MATH_CONSTANT_దొంతర_శిఖరం_DISPLAY_STYLE_జరుపు_పైకి = 23,
  HB_OT_MATH_CONSTANT_దొంతర_BOTTOM_జరుపు_కిందికి = 24,
  HB_OT_MATH_CONSTANT_దొంతర_BOTTOM_DISPLAY_STYLE_జరుపు_కిందికి = 25,
  HB_OT_MATH_CONSTANT_దొంతర_GAP_MIN = 26,
  HB_OT_MATH_CONSTANT_దొంతర_DISPLAY_STYLE_GAP_MIN = 27,
  HB_OT_MATH_CONSTANT_STRETCH_దొంతర_శిఖరం_జరుపు_పైకి = 28,
  HB_OT_MATH_CONSTANT_STRETCH_దొంతర_BOTTOM_జరుపు_కిందికి = 29,
  HB_OT_MATH_CONSTANT_STRETCH_దొంతర_GAP_ABOVE_MIN = 30,
  HB_OT_MATH_CONSTANT_STRETCH_దొంతర_GAP_క్రింద_MIN = 31,
  HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_జరుపు_పైకి = 32,
  HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_DISPLAY_STYLE_జరుపు_పైకి = 33,
  HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_జరుపు_కిందికి = 34,
  HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_DISPLAY_STYLE_జరుపు_కిందికి = 35,
  HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_GAP_MIN = 36,
  HB_OT_MATH_CONSTANT_FRACTION_అంకె_DISPLAY_STYLE_GAP_MIN = 37,
  HB_OT_MATH_CONSTANT_FRACTION_నియమం_THICKNESS = 38,
  HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_GAP_MIN = 39,
  HB_OT_MATH_CONSTANT_FRACTION_DENOM_DISPLAY_STYLE_GAP_MIN = 40,
  HB_OT_MATH_CONSTANT_SKEWED_FRACTION_HORIZONTAL_GAP = 41,
  HB_OT_MATH_CONSTANT_SKEWED_FRACTION_VERTICAL_GAP = 42,
  HB_OT_MATH_CONSTANT_OVERBAR_VERTICAL_GAP = 43,
  HB_OT_MATH_CONSTANT_OVERBAR_నియమం_THICKNESS = 44,
  HB_OT_MATH_CONSTANT_OVERBAR_EXTRA_ASCENDER = 45,
  HB_OT_MATH_CONSTANT_UNDERBAR_VERTICAL_GAP = 46,
  HB_OT_MATH_CONSTANT_UNDERBAR_నియమం_THICKNESS = 47,
  HB_OT_MATH_CONSTANT_UNDERBAR_EXTRA_DESCENDER = 48,
  HB_OT_MATH_CONSTANT_RADICAL_VERTICAL_GAP = 49,
  HB_OT_MATH_CONSTANT_RADICAL_DISPLAY_STYLE_VERTICAL_GAP = 50,
  HB_OT_MATH_CONSTANT_RADICAL_నియమం_THICKNESS = 51,
  HB_OT_MATH_CONSTANT_RADICAL_EXTRA_ASCENDER = 52,
  HB_OT_MATH_CONSTANT_RADICAL_KERN_BEFORE_DEGREE = 53,
  HB_OT_MATH_CONSTANT_RADICAL_KERN_AFTER_DEGREE = 54,
  HB_OT_MATH_CONSTANT_RADICAL_DEGREE_BOTTOM_RAISE_PERCENT = 55
} hb_ot_math_constant_t;

/**
 * hb_ot_math_kern_t:
 * @HB_OT_MATH_KERN_TOP_RIGHT: The top right corner of the glyph.
 * @HB_OT_MATH_KERN_TOP_LEFT: The top left corner of the glyph.
 * @HB_OT_MATH_KERN_BOTTOM_RIGHT: The bottom right corner of the glyph.
 * @HB_OT_MATH_KERN_BOTTOM_LEFT: The bottom left corner of the glyph.
 *
 * The math kerning-table types defined for the four corners
 * of a glyph.
 *
 * Since: 1.3.3
 */
రకనిర్వచనం గణన {
  HB_OT_MATH_KERN_శిఖరం_కుడి = 0,
  HB_OT_MATH_KERN_శిఖరం_ఎడమ = 1,
  HB_OT_MATH_KERN_BOTTOM_కుడి = 2,
  HB_OT_MATH_KERN_BOTTOM_ఎడమ = 3
} hb_ot_math_kern_t;

/**
 * hb_ot_math_kern_entry_t:
 * @max_correction_height: The maximum height at which this entry should be used
 * @kern_value: The kern value of the entry
 *
 * Data type to hold math kerning (cut-in) information for a glyph.
 *
 * Since: 3.4.0
 */
రకనిర్వచనం నిర్మాణం {
  hb_position_t max_correction_ఎత్తు;
  hb_position_t kern_విలువ_2;
} hb_ot_math_kern_నమోదు_t;

/**
 * hb_ot_math_glyph_variant_t:
 * @glyph: The glyph index of the variant
 * @advance: The advance width of the variant
 *
 * Data type to hold math-variant information for a glyph.
 *
 * Since: 1.3.3
 */
రకనిర్వచనం నిర్మాణం hb_ot_math_glyph_variant_t {
  hb_codepoint_t glyph;
  hb_position_t advance;
} hb_ot_math_glyph_variant_t;

/**
 * hb_ot_math_glyph_part_flags_t:
 * @HB_OT_MATH_GLYPH_PART_FLAG_EXTENDER: This is an extender glyph part that
 * can be repeated to reach the desired length.
 *
 * Flags for math glyph parts.
 *
 * Since: 1.3.3
 */
రకనిర్వచనం గణన { /*< flags >*/
  HB_OT_MATH_GLYPH_PART_FLAG_EXTENDER	= 0x00000001u  /* Extender glyph */
} hb_ot_math_glyph_part_flags_t;

/**
 * hb_ot_math_glyph_part_t:
 * @glyph: The glyph index of the variant part
 * @start_connector_length: The length of the connector on the starting side of the variant part
 * @end_connector_length: The length of the connector on the ending side of the variant part
 * @full_advance: The total advance of the part
 * @flags: #hb_ot_math_glyph_part_flags_t flags for the part
 * 
 * Data type to hold information for a "part" component of a math-variant glyph.
 * Large variants for stretchable math glyphs (such as parentheses) can be constructed
 * on the fly from parts.
 *
 * Since: 1.3.3
 */
రకనిర్వచనం నిర్మాణం hb_ot_math_glyph_part_t {
  hb_codepoint_t glyph;
  hb_position_t ప్రారంభం_connector_నిడివి;
  hb_position_t చివర_connector_నిడివి;
  hb_position_t నిండిన_advance;
  hb_ot_math_glyph_part_flags_t flags;
} hb_ot_math_glyph_part_t;

/* Methods */

HB_EXTERN hb_bool_t
hb_ot_math_has_దత్తాంశం (hb_face_t *face);

HB_EXTERN hb_position_t
hb_ot_math_get_constant (hb_font_t *font,
			 hb_ot_math_constant_t constant);

HB_EXTERN hb_position_t
hb_ot_math_get_glyph_italics_correction (hb_font_t *font,
					 hb_codepoint_t glyph);

HB_EXTERN hb_position_t
hb_ot_math_get_glyph_శిఖరం_accent_attachment (hb_font_t *font,
					    hb_codepoint_t glyph);

HB_EXTERN hb_bool_t
hb_ot_math_is_glyph_extended_shape (hb_face_t *face,
				    hb_codepoint_t glyph);

HB_EXTERN hb_position_t
hb_ot_math_get_glyph_kerning (hb_font_t *font,
			      hb_codepoint_t glyph,
			      hb_ot_math_kern_t kern,
			      hb_position_t correction_ఎత్తు);

HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_ot_math_get_glyph_kernings (hb_font_t *font,
			       hb_codepoint_t glyph,
			       hb_ot_math_kern_t kern,
			       గుర్తులేని పూర్ణాంకం ప్రారంభం_స్థానభ్రంశం,
			       గుర్తులేని పూర్ణాంకం *entries_లెక్క, /* IN/OUT */
			       hb_ot_math_kern_నమోదు_t *kern_entries /*బహిర్గతం*/);

HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_ot_math_get_glyph_variants (hb_font_t *font,
			       hb_codepoint_t glyph,
			       hb_direction_t direction,
			       గుర్తులేని పూర్ణాంకం ప్రారంభం_స్థానభ్రంశం,
			       గుర్తులేని పూర్ణాంకం *variants_లెక్క, /* IN/OUT */
			       hb_ot_math_glyph_variant_t *variants /*బహిర్గతం*/);

HB_EXTERN hb_position_t
hb_ot_math_get_min_connector_overlap (hb_font_t *font,
				      hb_direction_t direction);

HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_ot_math_get_glyph_assembly (hb_font_t *font,
			       hb_codepoint_t glyph,
			       hb_direction_t direction,
			       గుర్తులేని పూర్ణాంకం ప్రారంభం_స్థానభ్రంశం,
			       గుర్తులేని పూర్ణాంకం *parts_లెక్క, /* IN/OUT */
			       hb_ot_math_glyph_part_t *parts, /*బహిర్గతం*/
			       hb_position_t *italics_correction /*బహిర్గతం*/);


HB_చివర_DECLS

#అయితే_ముగింపు /* HB_OT_MATH_H */

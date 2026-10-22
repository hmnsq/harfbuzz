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

#ଅନ୍ତର୍ଭାଵ "hb-open-type.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-layout-common.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-layout-gdef-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-math.h"
#ଅନ୍ତର୍ଭାଵ "hb-depend-data.hh"

ନାମାକାଶ OT {


ସଂରଚନା Math_ମୂଲ୍ୟ_Record
{
  hb_position_t get_x_ମୂଲ୍ୟ (hb_font_t *font, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  { ଫେରନ୍ତୁ hb_saturate_add (font->em_scale_x (value), (base+deviceTable).get_x_delta (font, get_var_store (font, base))); }
  hb_position_t get_y_ମୂଲ୍ୟ (hb_font_t *font, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  { ଫେରନ୍ତୁ hb_saturate_add (font->em_scale_y (value), (base+deviceTable).get_y_delta (font, get_var_store (font, base))); }

  ଅଚଳ ItemVariationStore &get_var_store (hb_font_t *font, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
#ଯଦି !defined(HB_NO_VAR) && !defined(HB_NO_OT_LAYOUT)
    ଯଦି ((base+deviceTable).is_variation_device ())
      ଫେରନ୍ତୁ font->face->table.GDEF->table->get_var_store ();
#endif
    ଫେରନ୍ତୁ Null (ItemVariationStore);
  }

  Math_ମୂଲ୍ୟ_Record* copy (hb_serialize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base,
                         ଅଚଳ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> *variation_map) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    auto *out = c->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (nullptr);
    hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> *varidx_delta;
    ଯଦି (variation_map->has ((base+deviceTable).get_variation_index (), &varidx_delta) &&
        !c->check_assign (out->value, (int64_t) value + hb_second (*varidx_delta),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (nullptr);
    out->deviceTable.serialize_copy (c, deviceTable, base, 0, hb_serialize_context_t::Head, variation_map);

    return_trace (out);
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  { (base+deviceTable).collect_variation_indices (c); }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) && deviceTable.sanitize (c, base));
  }

  ସୁରକ୍ଷିତ:
  HBINT16		value;		/* The X or Y value in design units */
  Offset16To<Device>	deviceTable;	/* Offset to the device table - from the
					 * beginning of parent table.  May be NULL.
					 * Suggested format for device table is 1. */

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4);
};

ସଂରଚନା MathConstants
{
  friend ସଂରଚନା MATH;

  MathConstants* copy (hb_serialize_context_t *c,
                      ଅଚଳ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> *variation_map) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    auto *out = c->start_embed (ଏହି);

    HBINT16 *p = c->allocate_size<HBINT16> (HBINT16::static_size * 2);
    ଯଦି (unlikely (!p)) return_trace (nullptr);
    hb_memcpy (p, percentScaleDown, HBINT16::static_size * 2);

    HBUINT16 *m = c->allocate_size<HBUINT16> (HBUINT16::static_size * 2);
    ଯଦି (unlikely (!m)) return_trace (nullptr);
    hb_memcpy (m, minHeight, HBUINT16::static_size * 2);

    unsigned ଗଣନା_2 = ARRAY_LENGTH (math_ମୂଲ୍ୟ_Records);
    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
      ଯଦି (!c->copy (math_ମୂଲ୍ୟ_Records[i], ଏହି, variation_map))
        return_trace (nullptr);

    ଯଦି (!c->embed (radicalDegreeBottomRaisePercent)) return_trace (nullptr);
    return_trace (out);
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ auto &record : math_ମୂଲ୍ୟ_Records)
      record.collect_variation_indices (c, ଏହି);
  }

  ତାର୍କିକ sanitize_math_ମୂଲ୍ୟ_records (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = ARRAY_LENGTH (math_ମୂଲ୍ୟ_Records);
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
      ଯଦି (!math_ମୂଲ୍ୟ_Records[i].sanitize (c, ଏହି))
	return_trace (ମିଥ୍ୟା);

    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) && sanitize_math_ମୂଲ୍ୟ_records (c));
  }

  hb_position_t get_ମୂଲ୍ୟ (hb_ot_math_constant_t constant,
			   hb_font_t *font) ଅଚଳ
  {
    ବାଛ (constant) {

    ସ୍ଥିତି HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_DOWN:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_SCRIPT_SCRIPT_PERCENT_SCALE_DOWN:
      ଫେରନ୍ତୁ percentScaleDown[constant - HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_DOWN];

    ସ୍ଥିତି HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_HEIGHT:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_DISPLAY_OPERATOR_MIN_HEIGHT:
      ଫେରନ୍ତୁ font->em_scale_y (minHeight[constant - HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_HEIGHT]);

    ସ୍ଥିତି HB_OT_MATH_CONSTANT_RADICAL_KERN_AFTER_DEGREE:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_RADICAL_KERN_BEFORE_DEGREE:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_SKEWED_FRACTION_HORIZONTAL_GAP:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_SPACE_AFTER_SCRIPT:
      ଫେରନ୍ତୁ math_ମୂଲ୍ୟ_Records[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_x_ମୂଲ୍ୟ (font, ଏହି);

    ସ୍ଥିତି HB_OT_MATH_CONSTANT_ACCENT_BASE_HEIGHT:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_AXIS_HEIGHT:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_FLATTENED_ACCENT_BASE_HEIGHT:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_DISPLAY_STYLE_SHIFT_DOWN:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_GAP_MIN:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_SHIFT_DOWN:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_FRACTION_DENOM_DISPLAY_STYLE_GAP_MIN:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_DISPLAY_STYLE_SHIFT_UP:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_GAP_MIN:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_SHIFT_UP:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_FRACTION_ଅଙ୍କ_DISPLAY_STYLE_GAP_MIN:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_FRACTION_RULE_THICKNESS:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_LOWER_LIMIT_BASELINE_DROP_MIN:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_LOWER_LIMIT_GAP_MIN:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_MATH_LEADING:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_OVERBAR_EXTRA_ASCENDER:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_OVERBAR_RULE_THICKNESS:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_OVERBAR_VERTICAL_GAP:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_RADICAL_DISPLAY_STYLE_VERTICAL_GAP:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_RADICAL_EXTRA_ASCENDER:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_RADICAL_RULE_THICKNESS:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_RADICAL_VERTICAL_GAP:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_SKEWED_FRACTION_VERTICAL_GAP:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_ଗଦା_BOTTOM_DISPLAY_STYLE_SHIFT_DOWN:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_ଗଦା_BOTTOM_SHIFT_DOWN:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_ଗଦା_DISPLAY_STYLE_GAP_MIN:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_ଗଦା_GAP_MIN:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_ଗଦା_ଶୀର୍ଷ_DISPLAY_STYLE_SHIFT_UP:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_ଗଦା_ଶୀର୍ଷ_SHIFT_UP:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_STRETCH_ଗଦା_BOTTOM_SHIFT_DOWN:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_STRETCH_ଗଦା_GAP_ABOVE_MIN:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_STRETCH_ଗଦା_GAP_BELOW_MIN:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_STRETCH_ଗଦା_ଶୀର୍ଷ_SHIFT_UP:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_SUBSCRIPT_BASELINE_DROP_MIN:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_SUBSCRIPT_SHIFT_DOWN:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_SUBSCRIPT_ଶୀର୍ଷ_MAX:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_SUB_SUPERSCRIPT_GAP_MIN:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_SUPERSCRIPT_BASELINE_DROP_MAX:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MAX_WITH_SUBSCRIPT:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MIN:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_SUPERSCRIPT_SHIFT_UP:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_SUPERSCRIPT_SHIFT_UP_CRAMPED:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_UNDERBAR_EXTRA_DESCENDER:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_UNDERBAR_RULE_THICKNESS:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_UNDERBAR_VERTICAL_GAP:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_UPPER_LIMIT_BASELINE_RISE_MIN:
    ସ୍ଥିତି HB_OT_MATH_CONSTANT_UPPER_LIMIT_GAP_MIN:
      ଫେରନ୍ତୁ math_ମୂଲ୍ୟ_Records[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_y_ମୂଲ୍ୟ (font, ଏହି);

    ସ୍ଥିତି HB_OT_MATH_CONSTANT_RADICAL_DEGREE_BOTTOM_RAISE_PERCENT:
      ଫେରନ୍ତୁ radicalDegreeBottomRaisePercent;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ 0;
    }
  }

  ସୁରକ୍ଷିତ:
  HBINT16 percentScaleDown[2];
  HBUINT16 minHeight[2];
  Math_ମୂଲ୍ୟ_Record math_ମୂଲ୍ୟ_Records[51];
  HBINT16 radicalDegreeBottomRaisePercent;

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (214);
};

ସଂରଚନା MathItalicsCorrectionInfo
{
  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଅଚଳ hb_set_t &glyphset = c->plan->_glyphset_mathed;
    ଅଚଳ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (ଏହି+coverage, italicsCorrection)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->italicsCorrection, ଏହି, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (ସତ୍ୟ);
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ auto &entry : hb_zip (ଏହି+coverage, italicsCorrection))
      ଯଦି (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, ଏହି);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  coverage.sanitize (c, ଏହି) &&
		  italicsCorrection.sanitize (c, ଏହି));
  }

  hb_position_t get_ମୂଲ୍ୟ (hb_codepoint_t glyph,
			   hb_font_t *font) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = (ଏହି+coverage).get_coverage (glyph);
    ଫେରନ୍ତୁ italicsCorrection[index].get_x_ମୂଲ୍ୟ (font, ଏହି);
  }

  ସୁରକ୍ଷିତ:
  Offset16To<Coverage>       coverage;		/* Offset to Coverage table -
						 * from the beginning of
						 * MathItalicsCorrectionInfo
						 * table. */
  Array16Of<Math_ମୂଲ୍ୟ_Record> italicsCorrection;	/* Array of MathValueRecords
						 * defining italics correction
						 * values for each
						 * covered glyph. */

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (4, italicsCorrection);
};

ସଂରଚନା Math_ଶୀର୍ଷ_Accent_Attachment
{
  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଅଚଳ hb_set_t &glyphset = c->plan->_glyphset_mathed;
    ଅଚଳ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (ଏହି+ଶୀର୍ଷ_Accent_Coverage, ଶୀର୍ଷ_Accent_Attachment)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->ଶୀର୍ଷ_Accent_Attachment, ଏହି, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->ଶୀର୍ଷ_Accent_Coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (ସତ୍ୟ);
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ auto &entry : hb_zip (ଏହି+ଶୀର୍ଷ_Accent_Coverage, ଶୀର୍ଷ_Accent_Attachment))
      ଯଦି (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, ଏହି);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  ଶୀର୍ଷ_Accent_Coverage.sanitize (c, ଏହି) &&
		  ଶୀର୍ଷ_Accent_Attachment.sanitize (c, ଏହି));
  }

  hb_position_t get_ମୂଲ୍ୟ (hb_codepoint_t glyph,
			   hb_font_t *font) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = (ଏହି+ଶୀର୍ଷ_Accent_Coverage).get_coverage (glyph);
    ଯଦି (index == NOT_COVERED)
      ଫେରନ୍ତୁ font->get_glyph_h_advance (glyph) / 2;
    ଫେରନ୍ତୁ ଶୀର୍ଷ_Accent_Attachment[index].get_x_ମୂଲ୍ୟ (font, ଏହି);
  }

  ସୁରକ୍ଷିତ:
  Offset16To<Coverage>       ଶୀର୍ଷ_Accent_Coverage;   /* Offset to Coverage table -
						 * from the beginning of
						 * MathTopAccentAttachment
						 * table. */
  Array16Of<Math_ମୂଲ୍ୟ_Record> ଶୀର୍ଷ_Accent_Attachment; /* Array of MathValueRecords
						 * defining top accent
						 * attachment points for each
						 * covered glyph. */

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (2 + 2, ଶୀର୍ଷ_Accent_Attachment);
};

ସଂରଚନା MathKern
{
  MathKern* copy (hb_serialize_context_t *c,
                      ଅଚଳ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> *variation_map) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    auto *out = c->start_embed (ଏହି);

    ଯଦି (unlikely (!c->embed (height_ଗଣନା))) return_trace (nullptr);

    unsigned ଗଣନା_2 = 2 * height_ଗଣନା + 1;
    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
      ଯଦି (!c->copy (math_ମୂଲ୍ୟ_Records_Z.arrayZ[i], ଏହି, variation_map))
        return_trace (nullptr);

    return_trace (out);
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ auto &record : math_ମୂଲ୍ୟ_Records_Z.as_array (2 * height_ଗଣନା + 1))
      record.collect_variation_indices (c, ଏହି);
  }

  ତାର୍କିକ sanitize_math_ମୂଲ୍ୟ_records (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 2 * height_ଗଣନା + 1;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
      ଯଦି (!math_ମୂଲ୍ୟ_Records_Z.arrayZ[i].sanitize (c, ଏହି)) return_trace (ମିଥ୍ୟା);
    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  hb_barrier () &&
		  c->check_array (math_ମୂଲ୍ୟ_Records_Z.arrayZ, 2 * height_ଗଣନା + 1) &&
		  sanitize_math_ମୂଲ୍ୟ_records (c));
  }

  hb_position_t get_ମୂଲ୍ୟ (hb_position_t correction_height, hb_font_t *font) ଅଚଳ
  {
    ଅଚଳ Math_ମୂଲ୍ୟ_Record* correctionHeight = math_ମୂଲ୍ୟ_Records_Z.arrayZ;
    ଅଚଳ Math_ମୂଲ୍ୟ_Record* kern_ମୂଲ୍ୟ = math_ମୂଲ୍ୟ_Records_Z.arrayZ + height_ଗଣନା;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sign = font->y_scale < 0 ? -1 : +1;

    /* According to OpenType spec (v1.9), except for the boundary cases, the index
     * chosen for kern value should be i such that
     *    correctionHeight[i-1] <= correction_height < correctionHeight[i]
     * We can use the binary search algorithm of std::upper_bound(). Or, we can
     * use the internal hb_bsearch_impl.
     */
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos;
    auto cmp = +[](ଅଚଳ ଶୂନ୍ୟ* key, ଅଚଳ ଶୂନ୍ୟ* p,
                   ପୂର୍ଣ୍ଣସଂଖ୍ୟା sign, hb_font_t* font, ଅଚଳ MathKern* mathKern) -> ପୂର୍ଣ୍ଣସଂଖ୍ୟା {
      hb_position_t a = *(hb_position_t *) key;
      hb_position_t b = ((Math_ମୂଲ୍ୟ_Record *) p)->get_y_ମୂଲ୍ୟ (font, mathKern);
      ଯଦି (a == b) ଫେରନ୍ତୁ 0;
      ଫେରନ୍ତୁ sign > 0 ? (a < b ? -1 : +1) : (b < a ? -1 : +1);
    };
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = hb_bsearch_impl(&pos, correction_height, correctionHeight,
                                     height_ଗଣନା, Math_ମୂଲ୍ୟ_Record::static_size,
                                     cmp, sign, font, ଏହି) ? pos + 1 : pos;
    ଫେରନ୍ତୁ kern_ମୂଲ୍ୟ[i].get_x_ମୂଲ୍ୟ (font, ଏହି);
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_entries (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_offset,
			    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *entries_ଗଣନା, /* IN/OUT */
			    hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			    hb_font_t *font) ଅଚଳ
  {
    ଅଚଳ Math_ମୂଲ୍ୟ_Record* correctionHeight = math_ମୂଲ୍ୟ_Records_Z.arrayZ;
    ଅଚଳ Math_ମୂଲ୍ୟ_Record* kern_ମୂଲ୍ୟ = math_ମୂଲ୍ୟ_Records_Z.arrayZ + height_ଗଣନା;
    ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା entries_ଗଣନା_2 = height_ଗଣନା + 1;

    ଯଦି (entries_ଗଣନା && kern_entries)
    {
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start = hb_min (start_offset, entries_ଗଣନା_2);
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end = hb_min (start + *entries_ଗଣନା, entries_ଗଣନା_2);
      *entries_ଗଣନା = end - start;

      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < *entries_ଗଣନା; i++) {
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = start + i;

	hb_position_t max_height;
	ଯଦି (j == height_ଗଣନା) {
	  max_height = INT32_MAX;
	} ଅନ୍ୟଥା {
	  max_height = correctionHeight[j].get_y_ମୂଲ୍ୟ (font, ଏହି);
	}

	kern_entries[i] = {max_height, kern_ମୂଲ୍ୟ[j].get_x_ମୂଲ୍ୟ (font, ଏହି)};
      }
    }
    ଫେରନ୍ତୁ entries_ଗଣନା_2;
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	height_ଗଣନା;
  UnsizedArrayOf<Math_ମୂଲ୍ୟ_Record>
		math_ମୂଲ୍ୟ_Records_Z;
				/* Array of correction heights at
				 * which the kern value changes.
				 * Sorted by the height value in
				 * design units (heightCount entries),
				 * Followed by:
				 * Array of kern values corresponding
				 * to heights. (heightCount+1 entries).
				 */

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (2, math_ମୂଲ୍ୟ_Records_Z);
};

ସଂରଚନା MathKernInfoRecord
{
  MathKernInfoRecord* copy (hb_serialize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base,
                            ଅଚଳ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> *variation_map) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    auto *out = c->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (nullptr);

    unsigned ଗଣନା_2 = ARRAY_LENGTH (mathKern);
    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
      out->mathKern[i].serialize_copy (c, mathKern[i], base, 0, hb_serialize_context_t::Head, variation_map);

    return_trace (out);
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ auto &offset : mathKern)
      (base+offset).collect_variation_indices (c);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = ARRAY_LENGTH (mathKern);
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
      ଯଦି (unlikely (!mathKern[i].sanitize (c, base)))
	return_trace (ମିଥ୍ୟା);

    return_trace (ସତ୍ୟ);
  }

  hb_position_t get_kerning (hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font,
			     ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx = kern;
    ଯଦି (unlikely (idx >= ARRAY_LENGTH (mathKern))) ଫେରନ୍ତୁ 0;
    ଫେରନ୍ତୁ (base+mathKern[idx]).get_ମୂଲ୍ୟ (correction_height, font);
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_kernings (hb_ot_math_kern_t kern,
			     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_offset,
			     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *entries_ଗଣନା, /* IN/OUT */
			     hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			     hb_font_t *font,
			     ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx = kern;
    ଯଦି (unlikely (idx >= ARRAY_LENGTH (mathKern)) || !mathKern[idx]) {
      ଯଦି (entries_ଗଣନା) *entries_ଗଣନା = 0;
      ଫେରନ୍ତୁ 0;
    }
    ଫେରନ୍ତୁ (base+mathKern[idx]).get_entries (start_offset,
					     entries_ଗଣନା,
					     kern_entries,
					     font);
  }

  ସୁରକ୍ଷିତ:
  /* Offset to MathKern table for each corner -
   * from the beginning of MathKernInfo table.  May be NULL. */
  Offset16To<MathKern> mathKern[4];

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (8);
};

ସଂରଚନା MathKernInfo
{
  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଅଚଳ hb_set_t &glyphset = c->plan->_glyphset_mathed;
    ଅଚଳ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (ଏହି+mathKernCoverage, mathKernInfoRecords)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->mathKernInfoRecords, ଏହି, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->mathKernCoverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (ସତ୍ୟ);
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ auto &entry : hb_zip (ଏହି+mathKernCoverage, mathKernInfoRecords))
      ଯଦି (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, ଏହି);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  mathKernCoverage.sanitize (c, ଏହି) &&
		  mathKernInfoRecords.sanitize (c, ଏହି));
  }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = (ଏହି+mathKernCoverage).get_coverage (glyph);
    ଫେରନ୍ତୁ mathKernInfoRecords[index].get_kerning (kern, correction_height, font, ଏହି);
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_kernings (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_offset,
			     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *entries_ଗଣନା, /* IN/OUT */
			     hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			     hb_font_t *font) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = (ଏହି+mathKernCoverage).get_coverage (glyph);
    ଫେରନ୍ତୁ mathKernInfoRecords[index].get_kernings (kern,
						    start_offset,
						    entries_ଗଣନା,
						    kern_entries,
						    font,
						    ଏହି);
  }

  ସୁରକ୍ଷିତ:
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

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (4, mathKernInfoRecords);
};

ସଂରଚନା MathGlyphInfo
{
  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->embed (*ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    out->mathItalicsCorrectionInfo.serialize_subset (c, mathItalicsCorrectionInfo, ଏହି);
    out->math_ଶୀର୍ଷ_Accent_Attachment.serialize_subset (c, math_ଶୀର୍ଷ_Accent_Attachment, ଏହି);

    ଅଚଳ hb_set_t &glyphset = c->plan->_glyphset_mathed;
    ଅଚଳ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto it =
    + hb_iter (ଏହି+extendedShapeCoverage)
    | hb_take (c->plan->source->get_ଅଙ୍କ_glyphs ())
    | hb_filter (glyphset)
    | hb_map_retains_sorting (glyph_map)
    ;

    ଯଦି (it) out->extendedShapeCoverage.serialize_serialize (c->serializer, it);
    ଅନ୍ୟଥା out->extendedShapeCoverage = 0;

    out->mathKernInfo.serialize_subset (c, mathKernInfo, ଏହି);
    return_trace (ସତ୍ୟ);
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ
  {
    (ଏହି+mathItalicsCorrectionInfo).collect_variation_indices (c);
    (ଏହି+math_ଶୀର୍ଷ_Accent_Attachment).collect_variation_indices (c);
    (ଏହି+mathKernInfo).collect_variation_indices (c);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  mathItalicsCorrectionInfo.sanitize (c, ଏହି) &&
		  math_ଶୀର୍ଷ_Accent_Attachment.sanitize (c, ଏହି) &&
		  extendedShapeCoverage.sanitize (c, ଏହି) &&
		  mathKernInfo.sanitize (c, ଏହି));
  }

  hb_position_t
  get_italics_correction (hb_codepoint_t  glyph, hb_font_t *font) ଅଚଳ
  { ଫେରନ୍ତୁ (ଏହି+mathItalicsCorrectionInfo).get_ମୂଲ୍ୟ (glyph, font); }

  hb_position_t
  get_ଶୀର୍ଷ_accent_attachment (hb_codepoint_t  glyph, hb_font_t *font) ଅଚଳ
  { ଫେରନ୍ତୁ (ଏହି+math_ଶୀର୍ଷ_Accent_Attachment).get_ମୂଲ୍ୟ (glyph, font); }

  ତାର୍କିକ is_extended_shape (hb_codepoint_t glyph) ଅଚଳ
  { ଫେରନ୍ତୁ (ଏହି+extendedShapeCoverage).get_coverage (glyph) != NOT_COVERED; }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font) ଅଚଳ
  { ଫେରନ୍ତୁ (ଏହି+mathKernInfo).get_kerning (glyph, kern, correction_height, font); }

  hb_position_t get_kernings (hb_codepoint_t glyph,
			      hb_ot_math_kern_t kern,
			      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_offset,
			      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *entries_ଗଣନା, /* IN/OUT */
			      hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			      hb_font_t *font) ଅଚଳ
  { ଫେରନ୍ତୁ (ଏହି+mathKernInfo).get_kernings (glyph,
					     kern,
					     start_offset,
					     entries_ଗଣନା,
					     kern_entries,
					     font); }

  ସୁରକ୍ଷିତ:
  /* Offset to MathItalicsCorrectionInfo table -
   * from the beginning of MathGlyphInfo table. */
  Offset16To<MathItalicsCorrectionInfo> mathItalicsCorrectionInfo;

  /* Offset to MathTopAccentAttachment table -
   * from the beginning of MathGlyphInfo table. */
  Offset16To<Math_ଶୀର୍ଷ_Accent_Attachment> math_ଶୀର୍ଷ_Accent_Attachment;

  /* Offset to coverage table for Extended Shape glyphs -
   * from the beginning of MathGlyphInfo table. When the left or right glyph of
   * a box is an extended shape variant, the (ink) box (and not the default
   * position defined by values in MathConstants table) should be used for
   * vertical positioning purposes.  May be NULL.. */
  Offset16To<Coverage> extendedShapeCoverage;

   /* Offset to MathKernInfo table -
    * from the beginning of MathGlyphInfo table. */
  Offset16To<MathKernInfo> mathKernInfo;

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (8);
};

ସଂରଚନା MathGlyphVariantRecord
{
  friend ସଂରଚନା MathGlyphConstruction;

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    ଅଚଳ hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_assign (out->variantGlyph, glyph_map.get (variantGlyph), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ଶୂନ୍ୟ depend (hb_depend_data_builder_t *depend_data, unsigned source) ଅଚଳ
  { depend_data->add_depend(source, HB_OT_TAG_MATH, variantGlyph); }

  ଶୂନ୍ୟ closure_glyphs (hb_set_t *variant_glyphs) ଅଚଳ
  { variant_glyphs->add (variantGlyph); }

  ସୁରକ୍ଷିତ:
  HBGlyphID16 variantGlyph;       /* Glyph ID for the variant. */
  HBUINT16  advanceMeasurement; /* Advance width/height, in design units, of the
				 * variant, in the direction of requested
				 * glyph extension. */

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4);
};

ସଂରଚନା PartFlags : HBUINT16
{
  ଗଣନା Flags {
    Extender	= 0x0001u, /* If set, the part can be skipped or repeated. */

    Defined	= 0x0001u, /* All defined flags. */
  };

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (2);
};

ସଂରଚନା MathGlyphPartRecord
{
  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    ଅଚଳ hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_assign (out->glyph, glyph_map.get (glyph), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ଶୂନ୍ୟ extract (hb_ot_math_glyph_part_t &out,
		int64_t mult,
		hb_font_t *font) ଅଚଳ
  {
    out.glyph			= glyph;

    out.start_connector_length	= font->em_mult (startConnectorLength, mult);
    out.end_connector_length	= font->em_mult (endConnectorLength, mult);
    out.full_advance		= font->em_mult (fullAdvance, mult);

    static_assert ((unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) HB_OT_MATH_GLYPH_PART_FLAG_EXTENDER ==
		   (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) PartFlags::Extender, "");

    out.flags = (hb_ot_math_glyph_part_flags_t)
		(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)
		(partFlags & PartFlags::Defined);
  }

  ଶୂନ୍ୟ depend (hb_depend_data_builder_t *depend_data, unsigned source) ଅଚଳ
  { depend_data->add_depend(source, HB_OT_TAG_MATH, glyph); }

  ଶୂନ୍ୟ closure_glyphs (hb_set_t *variant_glyphs) ଅଚଳ
  { variant_glyphs->add (glyph); }

  ସୁରକ୍ଷିତ:
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

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (10);
};

ସଂରଚନା MathGlyphAssembly
{
  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);

    ଯଦି (!c->serializer->copy (italicsCorrection, ଏହି, &c->plan->layout_variation_idx_delta_map)) return_trace (ମିଥ୍ୟା);
    ଯଦି (!c->serializer->copy<HBUINT16> (partRecords.len)) return_trace (ମିଥ୍ୟା);

    ପାଇଁ (ଅଚଳ auto& record : partRecords.iter ())
      ଯଦି (!record.subset (c)) return_trace (ମିଥ୍ୟା);
    return_trace (ସତ୍ୟ);
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ
  {
    italicsCorrection.collect_variation_indices (c, ଏହି);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  italicsCorrection.sanitize (c, ଏହି) &&
		  partRecords.sanitize (c));
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_parts (hb_direction_t direction,
			  hb_font_t *font,
			  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_offset,
			  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *parts_ଗଣନା, /* IN/OUT */
			  hb_ot_math_glyph_part_t *parts /* OUT */,
			  hb_position_t *italics_correction /* OUT */) ଅଚଳ
  {
    ଯଦି (parts_ଗଣନା && parts)
    {
      int64_t mult = font->dir_mult (direction);
      ପାଇଁ (auto _ : hb_zip (partRecords.as_array ().sub_array (start_offset, parts_ଗଣନା),
			    hb_array (parts, *parts_ଗଣନା)))
	_.first.extract (_.second, mult, font);
    }

    ଯଦି (italics_correction)
      *italics_correction = italicsCorrection.get_x_ମୂଲ୍ୟ (font, ଏହି);

    ଫେରନ୍ତୁ partRecords.len;
  }

  ଶୂନ୍ୟ depend (hb_depend_data_builder_t *depend_data, unsigned source) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ auto& _ : partRecords.iter ())
      _.depend (depend_data, source);
  }

  ଶୂନ୍ୟ closure_glyphs (hb_set_t *variant_glyphs) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ auto& _ : partRecords.iter ())
      _.closure_glyphs (variant_glyphs);
  }

  ସୁରକ୍ଷିତ:
  Math_ମୂଲ୍ୟ_Record
		italicsCorrection;
				/* Italics correction of this
				 * MathGlyphAssembly. Should not
				 * depend on the assembly size. */
  Array16Of<MathGlyphPartRecord>
		partRecords;	/* Array of part records, from
				 * left to right and bottom to
				 * top. */

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (6, partRecords);
};

ସଂରଚନା MathGlyphConstruction
{
  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);

    out->glyphAssembly.serialize_subset (c, glyphAssembly, ଏହି);

    ଯଦି (!c->serializer->check_assign (out->mathGlyphVariantRecord.len, mathGlyphVariantRecord.len, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (ମିଥ୍ୟା);
    ପାଇଁ (ଅଚଳ auto& record : mathGlyphVariantRecord.iter ())
      ଯଦି (!record.subset (c)) return_trace (ମିଥ୍ୟା);

    return_trace (ସତ୍ୟ);
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ
  {
    (ଏହି+glyphAssembly).collect_variation_indices (c);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  glyphAssembly.sanitize (c, ଏହି) &&
		  mathGlyphVariantRecord.sanitize (c));
  }

  ଅଚଳ MathGlyphAssembly &get_assembly () ଅଚଳ { ଫେରନ୍ତୁ ଏହି+glyphAssembly; }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_variants (hb_direction_t direction,
			     hb_font_t *font,
			     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_offset,
			     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *variants_ଗଣନା, /* IN/OUT */
			     hb_ot_math_glyph_variant_t *variants /* OUT */) ଅଚଳ
  {
    ଯଦି (variants_ଗଣନା && variants)
    {
      int64_t mult = font->dir_mult (direction);
      ପାଇଁ (auto _ : hb_zip (mathGlyphVariantRecord.as_array ().sub_array (start_offset, variants_ଗଣନା),
			    hb_array (variants, *variants_ଗଣନା)))
	_.second = {_.first.variantGlyph, font->em_mult (_.first.advanceMeasurement, mult)};
    }
    ଫେରନ୍ତୁ mathGlyphVariantRecord.len;
  }

  ଶୂନ୍ୟ depend (hb_depend_data_builder_t *depend_data, unsigned source) ଅଚଳ
  {
    (ଏହି+glyphAssembly).depend (depend_data, source);

    ପାଇଁ (ଅଚଳ auto& _ : mathGlyphVariantRecord.iter ())
      _.depend (depend_data, source);
  }

  ଶୂନ୍ୟ closure_glyphs (hb_set_t *variant_glyphs) ଅଚଳ
  {
    (ଏହି+glyphAssembly).closure_glyphs (variant_glyphs);

    ପାଇଁ (ଅଚଳ auto& _ : mathGlyphVariantRecord.iter ())
      _.closure_glyphs (variant_glyphs);
  }

  ସୁରକ୍ଷିତ:
  /* Offset to MathGlyphAssembly table for this shape - from the beginning of
     MathGlyphConstruction table.  May be NULL. */
  Offset16To<MathGlyphAssembly>	  glyphAssembly;

  /* MathGlyphVariantRecords for alternative variants of the glyphs. */
  Array16Of<MathGlyphVariantRecord> mathGlyphVariantRecord;

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (4, mathGlyphVariantRecord);
};

ସଂରଚନା MathVariants
{
  ଶୂନ୍ୟ depend (hb_depend_data_builder_t *depend_data) ଅଚଳ
  {
    ଅଚଳ hb_array_t<ଅଚଳ Offset16To<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vert_Glyph_ଗଣନା + horiz_Glyph_ଗଣନା);

    ଯଦି (vertGlyphCoverage)
    {
      ଅଚଳ auto vert_offsets = glyph_construction_offsets.sub_array (0, vert_Glyph_ଗଣନା);
      + hb_zip (ଏହି+vertGlyphCoverage, vert_offsets)
      | hb_apply ([&] (ଅଚଳ hb_pair_t<hb_codepoint_t, ଅଚଳ Offset16To<MathGlyphConstruction>&> &_)
                  {
                    ଅଚଳ MathGlyphConstruction &mgc = ଏହି+_.second;
                    mgc.depend (depend_data, _.first);
                  })
      ;
    }
    ଯଦି (horizGlyphCoverage)
    {
      ଅଚଳ auto hori_offsets = glyph_construction_offsets.sub_array (vert_Glyph_ଗଣନା, horiz_Glyph_ଗଣନା);
      + hb_zip (ଏହି+horizGlyphCoverage, hori_offsets)
      | hb_apply ([&] (ଅଚଳ hb_pair_t<hb_codepoint_t, ଅଚଳ Offset16To<MathGlyphConstruction>&> &_)
                  {
                    ଅଚଳ MathGlyphConstruction &mgc = ଏହି+_.second;
                    mgc.depend (depend_data, _.first);
                  })
      ;
    }
  }

  ଶୂନ୍ୟ closure_glyphs (ଅଚଳ hb_set_t *glyph_set,
                       hb_set_t *variant_glyphs) ଅଚଳ
  {
    ଅଚଳ hb_array_t<ଅଚଳ Offset16To<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vert_Glyph_ଗଣନା + horiz_Glyph_ଗଣନା);

    ଯଦି (vertGlyphCoverage)
    {
      ଅଚଳ auto vert_offsets = glyph_construction_offsets.sub_array (0, vert_Glyph_ଗଣନା);
      + hb_zip (ଏହି+vertGlyphCoverage, vert_offsets)
      | hb_filter (glyph_set, hb_first)
      | hb_map (hb_second)
      | hb_map (hb_add (ଏହି))
      | hb_apply ([=] (ଅଚଳ MathGlyphConstruction &_) { _.closure_glyphs (variant_glyphs); })
      ;
    }

    ଯଦି (horizGlyphCoverage)
    {
      ଅଚଳ auto hori_offsets = glyph_construction_offsets.sub_array (vert_Glyph_ଗଣନା, horiz_Glyph_ଗଣନା);
      + hb_zip (ଏହି+horizGlyphCoverage, hori_offsets)
      | hb_filter (glyph_set, hb_first)
      | hb_map (hb_second)
      | hb_map (hb_add (ଏହି))
      | hb_apply ([=] (ଅଚଳ MathGlyphConstruction &_) { _.closure_glyphs (variant_glyphs); })
      ;
    }
  }

  ଶୂନ୍ୟ collect_coverage_and_indices (hb_sorted_vector_t<hb_codepoint_t>& new_coverage,
                                     ଅଚଳ Offset16To<Coverage>& coverage,
                                     unsigned i,
                                     unsigned end_index,
                                     hb_set_t& indices,
                                     ଅଚଳ hb_set_t& glyphset,
                                     ଅଚଳ hb_map_t& glyph_map) ଅଚଳ
  {
    ଯଦି (!coverage) ଫେରନ୍ତୁ;

    ପାଇଁ (ଅଚଳ auto _ : (ଏହି+coverage).iter ())
    {
      ଯଦି (i >= end_index) ଫେରନ୍ତୁ;
      ଯଦି (glyphset.has (_))
      {
        unsigned new_gid = glyph_map.get (_);
        new_coverage.push (new_gid);
        indices.add (i);
      }
      i++;
    }
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଅଚଳ hb_set_t &glyphset = c->plan->_glyphset_cmaped;
    ଅଚଳ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);
    ଯଦି (!c->serializer->check_assign (out->minConnectorOverlap, minConnectorOverlap, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (ମିଥ୍ୟା);

    hb_sorted_vector_t<hb_codepoint_t> new_vert_coverage;
    hb_sorted_vector_t<hb_codepoint_t> new_hori_coverage;
    hb_set_t indices;
    collect_coverage_and_indices (new_vert_coverage, vertGlyphCoverage, 0, vert_Glyph_ଗଣନା, indices, glyphset, glyph_map);
    collect_coverage_and_indices (new_hori_coverage, horizGlyphCoverage, vert_Glyph_ଗଣନା, vert_Glyph_ଗଣନା + horiz_Glyph_ଗଣନା, indices, glyphset, glyph_map);

    ଯଦି (!c->serializer->check_assign (out->vert_Glyph_ଗଣନା, new_vert_coverage.length, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (ମିଥ୍ୟା);
    ଯଦି (!c->serializer->check_assign (out->horiz_Glyph_ଗଣନା, new_hori_coverage.length, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (ମିଥ୍ୟା);

    ପାଇଁ (unsigned i : indices.iter ())
    {
      auto *o = c->serializer->embed (glyphConstruction[i]);
      ଯଦି (!o) return_trace (ମିଥ୍ୟା);
      o->serialize_subset (c, glyphConstruction[i], ଏହି);
    }

    ଯଦି (new_vert_coverage)
      out->vertGlyphCoverage.serialize_serialize (c->serializer, new_vert_coverage.iter ());

    ଯଦି (new_hori_coverage)
    out->horizGlyphCoverage.serialize_serialize (c->serializer, new_hori_coverage.iter ());
    return_trace (ସତ୍ୟ);
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ auto &entry : hb_zip (ଏହି+vertGlyphCoverage, glyphConstruction.as_array (vert_Glyph_ଗଣନା)))
      ଯଦି (c->glyph_set->has (hb_first (entry)))
        (ଏହି+hb_second (entry)).collect_variation_indices (c);
    ପାଇଁ (ଅଚଳ auto &entry : hb_zip (ଏହି+horizGlyphCoverage, glyphConstruction.as_array (vert_Glyph_ଗଣନା + horiz_Glyph_ଗଣନା).sub_array (vert_Glyph_ଗଣନା)))
      ଯଦି (c->glyph_set->has (hb_first (entry)))
        (ଏହି+hb_second (entry)).collect_variation_indices (c);
  }

  ତାର୍କିକ sanitize_offsets (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = vert_Glyph_ଗଣନା + horiz_Glyph_ଗଣନା;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
      ଯଦି (!glyphConstruction.arrayZ[i].sanitize (c, ଏହି)) return_trace (ମିଥ୍ୟା);
    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  vertGlyphCoverage.sanitize (c, ଏହି) &&
		  horizGlyphCoverage.sanitize (c, ଏହି) &&
		  hb_barrier () &&
		  c->check_array (glyphConstruction.arrayZ, vert_Glyph_ଗଣନା + horiz_Glyph_ଗଣନା) &&
		  sanitize_offsets (c));
  }

  hb_position_t get_min_connector_overlap (hb_direction_t direction,
						  hb_font_t *font) ଅଚଳ
  { ଫେରନ୍ତୁ font->em_scale_dir (minConnectorOverlap, direction); }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_glyph_variants (hb_codepoint_t glyph,
				   hb_direction_t direction,
				   hb_font_t *font,
				   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_offset,
				   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *variants_ଗଣନା, /* IN/OUT */
				   hb_ot_math_glyph_variant_t *variants /* OUT */) ଅଚଳ
  { ଫେରନ୍ତୁ get_glyph_construction (glyph, direction, font)
	   .get_variants (direction, font, start_offset, variants_ଗଣନା, variants); }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_glyph_parts (hb_codepoint_t glyph,
				hb_direction_t direction,
				hb_font_t *font,
				unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_offset,
				unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *parts_ଗଣନା, /* IN/OUT */
				hb_ot_math_glyph_part_t *parts /* OUT */,
				hb_position_t *italics_correction /* OUT */) ଅଚଳ
  { ଫେରନ୍ତୁ get_glyph_construction (glyph, direction, font)
	   .get_assembly ()
	   .get_parts (direction, font,
		       start_offset, parts_ଗଣନା, parts,
		       italics_correction); }

  ବ୍ୟକ୍ତିଗତ:
  ଅଚଳ MathGlyphConstruction &
  get_glyph_construction (hb_codepoint_t glyph,
			  hb_direction_t direction,
			  hb_font_t *font HB_UNUSED) ଅଚଳ
  {
    ତାର୍କିକ vertical = HB_DIRECTION_IS_VERTICAL (direction);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = vertical ? vert_Glyph_ଗଣନା : horiz_Glyph_ଗଣନା;
    ଅଚଳ Offset16To<Coverage> &coverage = vertical ? vertGlyphCoverage
						  : horizGlyphCoverage;

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = (ଏହି+coverage).get_coverage (glyph);
    ଯଦି (unlikely (index >= ଗଣନା_2)) ଫେରନ୍ତୁ Null (MathGlyphConstruction);

    ଯଦି (!vertical)
      index += vert_Glyph_ଗଣନା;

    ଫେରନ୍ତୁ ଏହି+glyphConstruction[index];
  }

  ସୁରକ୍ଷିତ:
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
  HBUINT16	vert_Glyph_ଗଣନା;	/* Number of glyphs for which
				 * information is provided for
				 * vertically growing variants. */
  HBUINT16	horiz_Glyph_ଗଣନା;/* Number of glyphs for which
				 * information is provided for
				 * horizontally growing variants. */

  /* Array of offsets to MathGlyphConstruction tables - from the beginning of
     the MathVariants table, for shapes growing in vertical/horizontal
     direction. */
  UnsizedArrayOf<Offset16To<MathGlyphConstruction>>
			glyphConstruction;

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (10, glyphConstruction);
};


/*
 * MATH -- Mathematical typesetting
 * https://docs.microsoft.com/en-us/typography/opentype/spec/math
 */

ସଂରଚନା MATH
{
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_MATH;

  ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ version.to_int (); }

  ଶୂନ୍ୟ depend (hb_depend_data_builder_t *depend_data) ଅଚଳ
  {
    ଯଦି (mathVariants)
      (ଏହି+mathVariants).depend (depend_data);
  }

  ଶୂନ୍ୟ closure_glyphs (hb_set_t *glyph_set) ଅଚଳ
  {
    ଯଦି (mathVariants)
    {
      hb_set_t variant_glyphs;
      (ଏହି+mathVariants).closure_glyphs (glyph_set, &variant_glyphs);
      hb_set_union (glyph_set, &variant_glyphs);
    }
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->embed (*ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    out->mathConstants.serialize_copy (c->serializer, mathConstants, ଏହି, 0, hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map);
    out->mathGlyphInfo.serialize_subset (c, mathGlyphInfo, ଏହି);
    out->mathVariants.serialize_subset (c, mathVariants, ଏହି);
    return_trace (ସତ୍ୟ);
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c, ଅଚଳ hb_set_t *variant_glyph_set) ଅଚଳ
  {
    (ଏହି+mathConstants).collect_variation_indices (c);
    (ଏହି+mathGlyphInfo).collect_variation_indices (c);
    auto *glyph_set = c->glyph_set;
    c->glyph_set = variant_glyph_set;
    (ଏହି+mathVariants).collect_variation_indices (c);
    c->glyph_set = glyph_set;
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (version.sanitize (c) &&
		  likely (version.major == 1) &&
		  hb_barrier () &&
		  mathConstants.sanitize (c, ଏହି) &&
		  mathGlyphInfo.sanitize (c, ଏହି) &&
		  mathVariants.sanitize (c, ଏହି));
  }

  // https://github.com/harfbuzz/harfbuzz/issues/4653
  HB_INTERNAL ତାର୍କିକ is_bad_cambria (hb_font_t *font) ଅଚଳ
  {
#ifndef HB_NO_MATH
    ବାଛ HB_CODEPOINT_ENCODE3 (font->face->table.MATH.get_blob ()->length,
                                 (ଏହି+mathConstants).minHeight[1], // displayOperatorMinHeight
                                 (ଏହି+mathConstants).minHeight[0]) // delimitedSubFormulaMinHeight
    {
      /* sha1sum:ab4a4fe054d23061f3c039493d6f665cfda2ecf5  cambria.ttc
       * sha1sum:086855301bff644f9d8827b88491fcf73a6d4cb9  cambria.ttc
       * sha1sum:b1e5a3feaca2ea3dfcf79ccb377de749ecf60343  cambria.ttc */
      ସ୍ଥିତି HB_CODEPOINT_ENCODE3 (25722, 2500, 3000):
        ଫେରନ୍ତୁ ସତ୍ୟ;
    }
#endif
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  hb_position_t get_constant (hb_ot_math_constant_t  constant,
			      hb_font_t		   *font) ଅଚଳ
  { ଫେରନ୍ତୁ (ଏହି+mathConstants).get_ମୂଲ୍ୟ (constant, font); }

  ଅଚଳ MathGlyphInfo &get_glyph_info () ଅଚଳ { ଫେରନ୍ତୁ ଏହି+mathGlyphInfo; }

  ଅଚଳ MathVariants &get_variants () ଅଚଳ    { ଫେରନ୍ତୁ ଏହି+mathVariants; }

  ସୁରକ୍ଷିତ:
  FixedVersion<>version;	/* Version of the MATH table
				 * initially set to 0x00010000u */
  Offset16To<MathConstants>
		mathConstants;	/* MathConstants table */
  Offset16To<MathGlyphInfo>
		mathGlyphInfo;	/* MathGlyphInfo table */
  Offset16To<MathVariants>
		mathVariants;	/* MathVariants table */

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (10);
};

} /* namespace OT */


#endif /* HB_OT_MATH_TABLE_HH */

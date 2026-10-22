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

#অন্তর্ভাব "hb-open-type.hh"
#অন্তর্ভাব "hb-ot-layout-common.hh"
#অন্তর্ভাব "hb-ot-layout-gdef-table.hh"
#অন্তর্ভাব "hb-ot-math.h"
#অন্তর্ভাব "hb-depend-data.hh"

নামাকাশ OT {


কাঠামো Math_মান_Record
{
  hb_position_t get_x_মান (hb_font_t *font, ধ্রুবক শূন্য *base) ধ্রুবক
  { ফেরত hb_saturate_add (font->em_scale_x (value), (base+deviceTable).get_x_delta (font, get_var_store (font, base))); }
  hb_position_t get_y_মান (hb_font_t *font, ধ্রুবক শূন্য *base) ধ্রুবক
  { ফেরত hb_saturate_add (font->em_scale_y (value), (base+deviceTable).get_y_delta (font, get_var_store (font, base))); }

  ধ্রুবক ItemVariationStore &get_var_store (hb_font_t *font, ধ্রুবক শূন্য *base) ধ্রুবক
  {
#যদি !defined(HB_NO_VAR) && !defined(HB_NO_OT_LAYOUT)
    যদি ((base+deviceTable).is_variation_device ())
      ফেরত font->face->table.GDEF->table->get_var_store ();
#endif
    ফেরত Null (ItemVariationStore);
  }

  Math_মান_Record* copy (hb_serialize_context_t *c, ধ্রুবক শূন্য *base,
                         ধ্রুবক hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণসংখ্যা>> *variation_map) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    auto *out = c->embed (এই);
    যদি (unlikely (!out)) return_trace (nullptr);
    hb_pair_t<unsigned, পূর্ণসংখ্যা> *varidx_delta;
    যদি (variation_map->has ((base+deviceTable).get_variation_সূচক (), &varidx_delta) &&
        !c->check_assign (out->value, (int64_t) value + hb_second (*varidx_delta),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (nullptr);
    out->deviceTable.serialize_copy (c, deviceTable, base, 0, hb_serialize_context_t::শীর্ষ, variation_map);

    return_trace (out);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c, ধ্রুবক শূন্য *base) ধ্রুবক
  { (base+deviceTable).collect_variation_indices (c); }

  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক শূন্য *base) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) && deviceTable.sanitize (c, base));
  }

  সুরক্ষিত:
  HBINT16		value;		/* The X or Y value in design units */
  Offset16To<Device>	deviceTable;	/* Offset to the device table - from the
					 * beginning of parent table.  May be NULL.
					 * Suggested format for device table is 1. */

  সর্বজনীন:
  DEFINE_আকার_STATIC (4);
};

কাঠামো MathConstants
{
  friend কাঠামো MATH;

  MathConstants* copy (hb_serialize_context_t *c,
                      ধ্রুবক hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণসংখ্যা>> *variation_map) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    auto *out = c->start_embed (এই);

    HBINT16 *p = c->allocate_আকার<HBINT16> (HBINT16::static_আকার * 2);
    যদি (unlikely (!p)) return_trace (nullptr);
    hb_memcpy (p, percentScaleDown, HBINT16::static_আকার * 2);

    HBUINT16 *m = c->allocate_আকার<HBUINT16> (HBUINT16::static_আকার * 2);
    যদি (unlikely (!m)) return_trace (nullptr);
    hb_memcpy (m, minHeight, HBUINT16::static_আকার * 2);

    unsigned গুনতি = ARRAY_দৈর্ঘ্য (math_মান_Records);
    জন্য (unsigned i = 0; i < গুনতি; i++)
      যদি (!c->copy (math_মান_Records[i], এই, variation_map))
        return_trace (nullptr);

    যদি (!c->embed (radicalDegreeBottomRaisePercent)) return_trace (nullptr);
    return_trace (out);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুবক
  {
    জন্য (ধ্রুবক auto &record : math_মান_Records)
      record.collect_variation_indices (c, এই);
  }

  যৌক্তিক sanitize_math_মান_records (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);

    unsigned পূর্ণসংখ্যা গুনতি = ARRAY_দৈর্ঘ্য (math_মান_Records);
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
      যদি (!math_মান_Records[i].sanitize (c, এই))
	return_trace (মিথ্যা);

    return_trace (সত্য);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) && sanitize_math_মান_records (c));
  }

  hb_position_t get_মান (hb_ot_math_constant_t constant,
			   hb_font_t *font) ধ্রুবক
  {
    নির্বাচন (constant) {

    ক্ষেত্র HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_DOWN:
    ক্ষেত্র HB_OT_MATH_CONSTANT_SCRIPT_SCRIPT_PERCENT_SCALE_DOWN:
      ফেরত percentScaleDown[constant - HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_DOWN];

    ক্ষেত্র HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_HEIGHT:
    ক্ষেত্র HB_OT_MATH_CONSTANT_DISPLAY_OPERATOR_MIN_HEIGHT:
      ফেরত font->em_scale_y (minHeight[constant - HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_HEIGHT]);

    ক্ষেত্র HB_OT_MATH_CONSTANT_RADICAL_KERN_AFTER_DEGREE:
    ক্ষেত্র HB_OT_MATH_CONSTANT_RADICAL_KERN_BEFORE_DEGREE:
    ক্ষেত্র HB_OT_MATH_CONSTANT_SKEWED_FRACTION_HORIZONTAL_GAP:
    ক্ষেত্র HB_OT_MATH_CONSTANT_SPACE_AFTER_SCRIPT:
      ফেরত math_মান_Records[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_x_মান (font, এই);

    ক্ষেত্র HB_OT_MATH_CONSTANT_ACCENT_BASE_HEIGHT:
    ক্ষেত্র HB_OT_MATH_CONSTANT_AXIS_HEIGHT:
    ক্ষেত্র HB_OT_MATH_CONSTANT_FLATTENED_ACCENT_BASE_HEIGHT:
    ক্ষেত্র HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_DISPLAY_STYLE_SHIFT_DOWN:
    ক্ষেত্র HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_GAP_MIN:
    ক্ষেত্র HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_SHIFT_DOWN:
    ক্ষেত্র HB_OT_MATH_CONSTANT_FRACTION_DENOM_DISPLAY_STYLE_GAP_MIN:
    ক্ষেত্র HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_DISPLAY_STYLE_SHIFT_UP:
    ক্ষেত্র HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_GAP_MIN:
    ক্ষেত্র HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_SHIFT_UP:
    ক্ষেত্র HB_OT_MATH_CONSTANT_FRACTION_অঙ্ক_DISPLAY_STYLE_GAP_MIN:
    ক্ষেত্র HB_OT_MATH_CONSTANT_FRACTION_RULE_THICKNESS:
    ক্ষেত্র HB_OT_MATH_CONSTANT_LOWER_LIMIT_BASELINE_DROP_MIN:
    ক্ষেত্র HB_OT_MATH_CONSTANT_LOWER_LIMIT_GAP_MIN:
    ক্ষেত্র HB_OT_MATH_CONSTANT_MATH_LEADING:
    ক্ষেত্র HB_OT_MATH_CONSTANT_OVERBAR_EXTRA_ASCENDER:
    ক্ষেত্র HB_OT_MATH_CONSTANT_OVERBAR_RULE_THICKNESS:
    ক্ষেত্র HB_OT_MATH_CONSTANT_OVERBAR_VERTICAL_GAP:
    ক্ষেত্র HB_OT_MATH_CONSTANT_RADICAL_DISPLAY_STYLE_VERTICAL_GAP:
    ক্ষেত্র HB_OT_MATH_CONSTANT_RADICAL_EXTRA_ASCENDER:
    ক্ষেত্র HB_OT_MATH_CONSTANT_RADICAL_RULE_THICKNESS:
    ক্ষেত্র HB_OT_MATH_CONSTANT_RADICAL_VERTICAL_GAP:
    ক্ষেত্র HB_OT_MATH_CONSTANT_SKEWED_FRACTION_VERTICAL_GAP:
    ক্ষেত্র HB_OT_MATH_CONSTANT_স্তূপ_BOTTOM_DISPLAY_STYLE_SHIFT_DOWN:
    ক্ষেত্র HB_OT_MATH_CONSTANT_স্তূপ_BOTTOM_SHIFT_DOWN:
    ক্ষেত্র HB_OT_MATH_CONSTANT_স্তূপ_DISPLAY_STYLE_GAP_MIN:
    ক্ষেত্র HB_OT_MATH_CONSTANT_স্তূপ_GAP_MIN:
    ক্ষেত্র HB_OT_MATH_CONSTANT_স্তূপ_শীর্ষ_DISPLAY_STYLE_SHIFT_UP:
    ক্ষেত্র HB_OT_MATH_CONSTANT_স্তূপ_শীর্ষ_SHIFT_UP:
    ক্ষেত্র HB_OT_MATH_CONSTANT_STRETCH_স্তূপ_BOTTOM_SHIFT_DOWN:
    ক্ষেত্র HB_OT_MATH_CONSTANT_STRETCH_স্তূপ_GAP_ABOVE_MIN:
    ক্ষেত্র HB_OT_MATH_CONSTANT_STRETCH_স্তূপ_GAP_BELOW_MIN:
    ক্ষেত্র HB_OT_MATH_CONSTANT_STRETCH_স্তূপ_শীর্ষ_SHIFT_UP:
    ক্ষেত্র HB_OT_MATH_CONSTANT_SUBSCRIPT_BASELINE_DROP_MIN:
    ক্ষেত্র HB_OT_MATH_CONSTANT_SUBSCRIPT_SHIFT_DOWN:
    ক্ষেত্র HB_OT_MATH_CONSTANT_SUBSCRIPT_শীর্ষ_MAX:
    ক্ষেত্র HB_OT_MATH_CONSTANT_SUB_SUPERSCRIPT_GAP_MIN:
    ক্ষেত্র HB_OT_MATH_CONSTANT_SUPERSCRIPT_BASELINE_DROP_MAX:
    ক্ষেত্র HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MAX_WITH_SUBSCRIPT:
    ক্ষেত্র HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MIN:
    ক্ষেত্র HB_OT_MATH_CONSTANT_SUPERSCRIPT_SHIFT_UP:
    ক্ষেত্র HB_OT_MATH_CONSTANT_SUPERSCRIPT_SHIFT_UP_CRAMPED:
    ক্ষেত্র HB_OT_MATH_CONSTANT_UNDERBAR_EXTRA_DESCENDER:
    ক্ষেত্র HB_OT_MATH_CONSTANT_UNDERBAR_RULE_THICKNESS:
    ক্ষেত্র HB_OT_MATH_CONSTANT_UNDERBAR_VERTICAL_GAP:
    ক্ষেত্র HB_OT_MATH_CONSTANT_UPPER_LIMIT_BASELINE_RISE_MIN:
    ক্ষেত্র HB_OT_MATH_CONSTANT_UPPER_LIMIT_GAP_MIN:
      ফেরত math_মান_Records[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_y_মান (font, এই);

    ক্ষেত্র HB_OT_MATH_CONSTANT_RADICAL_DEGREE_BOTTOM_RAISE_PERCENT:
      ফেরত radicalDegreeBottomRaisePercent;

    পূর্বনির্ধারিত:
      ফেরত 0;
    }
  }

  সুরক্ষিত:
  HBINT16 percentScaleDown[2];
  HBUINT16 minHeight[2];
  Math_মান_Record math_মান_Records[51];
  HBINT16 radicalDegreeBottomRaisePercent;

  সর্বজনীন:
  DEFINE_আকার_STATIC (214);
};

কাঠামো MathItalicsCorrectionInfo
{
  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    ধ্রুবক hb_set_t &glyphset = c->plan->_glyphset_mathed;
    ধ্রুবক hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (এই+coverage, italicsCorrection)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->italicsCorrection, এই, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (সত্য);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুবক
  {
    জন্য (ধ্রুবক auto &entry : hb_zip (এই+coverage, italicsCorrection))
      যদি (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, এই);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  coverage.sanitize (c, এই) &&
		  italicsCorrection.sanitize (c, এই));
  }

  hb_position_t get_মান (hb_codepoint_t glyph,
			   hb_font_t *font) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা সূচক_2 = (এই+coverage).get_coverage (glyph);
    ফেরত italicsCorrection[সূচক_2].get_x_মান (font, এই);
  }

  সুরক্ষিত:
  Offset16To<Coverage>       coverage;		/* Offset to Coverage table -
						 * from the beginning of
						 * MathItalicsCorrectionInfo
						 * table. */
  Array16Of<Math_মান_Record> italicsCorrection;	/* Array of MathValueRecords
						 * defining italics correction
						 * values for each
						 * covered glyph. */

  সর্বজনীন:
  DEFINE_আকার_ARRAY (4, italicsCorrection);
};

কাঠামো Math_শীর্ষ_Accent_Attachment
{
  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    ধ্রুবক hb_set_t &glyphset = c->plan->_glyphset_mathed;
    ধ্রুবক hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (এই+শীর্ষ_Accent_Coverage, শীর্ষ_Accent_Attachment)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->শীর্ষ_Accent_Attachment, এই, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->শীর্ষ_Accent_Coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (সত্য);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুবক
  {
    জন্য (ধ্রুবক auto &entry : hb_zip (এই+শীর্ষ_Accent_Coverage, শীর্ষ_Accent_Attachment))
      যদি (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, এই);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  শীর্ষ_Accent_Coverage.sanitize (c, এই) &&
		  শীর্ষ_Accent_Attachment.sanitize (c, এই));
  }

  hb_position_t get_মান (hb_codepoint_t glyph,
			   hb_font_t *font) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা সূচক_2 = (এই+শীর্ষ_Accent_Coverage).get_coverage (glyph);
    যদি (সূচক_2 == NOT_COVERED)
      ফেরত font->get_glyph_h_advance (glyph) / 2;
    ফেরত শীর্ষ_Accent_Attachment[সূচক_2].get_x_মান (font, এই);
  }

  সুরক্ষিত:
  Offset16To<Coverage>       শীর্ষ_Accent_Coverage;   /* Offset to Coverage table -
						 * from the beginning of
						 * MathTopAccentAttachment
						 * table. */
  Array16Of<Math_মান_Record> শীর্ষ_Accent_Attachment; /* Array of MathValueRecords
						 * defining top accent
						 * attachment points for each
						 * covered glyph. */

  সর্বজনীন:
  DEFINE_আকার_ARRAY (2 + 2, শীর্ষ_Accent_Attachment);
};

কাঠামো MathKern
{
  MathKern* copy (hb_serialize_context_t *c,
                      ধ্রুবক hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণসংখ্যা>> *variation_map) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    auto *out = c->start_embed (এই);

    যদি (unlikely (!c->embed (height_গুনতি))) return_trace (nullptr);

    unsigned গুনতি = 2 * height_গুনতি + 1;
    জন্য (unsigned i = 0; i < গুনতি; i++)
      যদি (!c->copy (math_মান_Records_Z.arrayZ[i], এই, variation_map))
        return_trace (nullptr);

    return_trace (out);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুবক
  {
    জন্য (ধ্রুবক auto &record : math_মান_Records_Z.as_array (2 * height_গুনতি + 1))
      record.collect_variation_indices (c, এই);
  }

  যৌক্তিক sanitize_math_মান_records (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    unsigned পূর্ণসংখ্যা গুনতি = 2 * height_গুনতি + 1;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
      যদি (!math_মান_Records_Z.arrayZ[i].sanitize (c, এই)) return_trace (মিথ্যা);
    return_trace (সত্য);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  hb_barrier () &&
		  c->check_array (math_মান_Records_Z.arrayZ, 2 * height_গুনতি + 1) &&
		  sanitize_math_মান_records (c));
  }

  hb_position_t get_মান (hb_position_t correction_height, hb_font_t *font) ধ্রুবক
  {
    ধ্রুবক Math_মান_Record* correctionHeight = math_মান_Records_Z.arrayZ;
    ধ্রুবক Math_মান_Record* kern_মান = math_মান_Records_Z.arrayZ + height_গুনতি;
    পূর্ণসংখ্যা sign = font->y_scale < 0 ? -1 : +1;

    /* According to OpenType spec (v1.9), except for the boundary cases, the index
     * chosen for kern value should be i such that
     *    correctionHeight[i-1] <= correction_height < correctionHeight[i]
     * We can use the binary search algorithm of std::upper_bound(). Or, we can
     * use the internal hb_bsearch_impl.
     */
    unsigned পূর্ণসংখ্যা pos;
    auto cmp = +[](ধ্রুবক শূন্য* key, ধ্রুবক শূন্য* p,
                   পূর্ণসংখ্যা sign, hb_font_t* font, ধ্রুবক MathKern* mathKern) -> পূর্ণসংখ্যা {
      hb_position_t a = *(hb_position_t *) key;
      hb_position_t b = ((Math_মান_Record *) p)->get_y_মান (font, mathKern);
      যদি (a == b) ফেরত 0;
      ফেরত sign > 0 ? (a < b ? -1 : +1) : (b < a ? -1 : +1);
    };
    unsigned পূর্ণসংখ্যা i = hb_bsearch_impl(&pos, correction_height, correctionHeight,
                                     height_গুনতি, Math_মান_Record::static_আকার,
                                     cmp, sign, font, এই) ? pos + 1 : pos;
    ফেরত kern_মান[i].get_x_মান (font, এই);
  }

  unsigned পূর্ণসংখ্যা get_entries (unsigned পূর্ণসংখ্যা start_offset,
			    unsigned পূর্ণসংখ্যা *entries_গুনতি, /* IN/OUT */
			    hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			    hb_font_t *font) ধ্রুবক
  {
    ধ্রুবক Math_মান_Record* correctionHeight = math_মান_Records_Z.arrayZ;
    ধ্রুবক Math_মান_Record* kern_মান = math_মান_Records_Z.arrayZ + height_গুনতি;
    ধ্রুবক unsigned পূর্ণসংখ্যা entries_গুনতি_2 = height_গুনতি + 1;

    যদি (entries_গুনতি && kern_entries)
    {
      unsigned পূর্ণসংখ্যা start = hb_min (start_offset, entries_গুনতি_2);
      unsigned পূর্ণসংখ্যা end = hb_min (start + *entries_গুনতি, entries_গুনতি_2);
      *entries_গুনতি = end - start;

      জন্য (unsigned পূর্ণসংখ্যা i = 0; i < *entries_গুনতি; i++) {
	unsigned পূর্ণসংখ্যা j = start + i;

	hb_position_t max_height;
	যদি (j == height_গুনতি) {
	  max_height = INT32_MAX;
	} অন্যথা {
	  max_height = correctionHeight[j].get_y_মান (font, এই);
	}

	kern_entries[i] = {max_height, kern_মান[j].get_x_মান (font, এই)};
      }
    }
    ফেরত entries_গুনতি_2;
  }

  সুরক্ষিত:
  HBUINT16	height_গুনতি;
  UnsizedArrayOf<Math_মান_Record>
		math_মান_Records_Z;
				/* Array of correction heights at
				 * which the kern value changes.
				 * Sorted by the height value in
				 * design units (heightCount entries),
				 * Followed by:
				 * Array of kern values corresponding
				 * to heights. (heightCount+1 entries).
				 */

  সর্বজনীন:
  DEFINE_আকার_ARRAY (2, math_মান_Records_Z);
};

কাঠামো MathKernInfoRecord
{
  MathKernInfoRecord* copy (hb_serialize_context_t *c, ধ্রুবক শূন্য *base,
                            ধ্রুবক hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণসংখ্যা>> *variation_map) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    auto *out = c->embed (এই);
    যদি (unlikely (!out)) return_trace (nullptr);

    unsigned গুনতি = ARRAY_দৈর্ঘ্য (mathKern);
    জন্য (unsigned i = 0; i < গুনতি; i++)
      out->mathKern[i].serialize_copy (c, mathKern[i], base, 0, hb_serialize_context_t::শীর্ষ, variation_map);

    return_trace (out);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c, ধ্রুবক শূন্য *base) ধ্রুবক
  {
    জন্য (ধ্রুবক auto &offset : mathKern)
      (base+offset).collect_variation_indices (c);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক শূন্য *base) ধ্রুবক
  {
    TRACE_SANITIZE (এই);

    unsigned পূর্ণসংখ্যা গুনতি = ARRAY_দৈর্ঘ্য (mathKern);
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
      যদি (unlikely (!mathKern[i].sanitize (c, base)))
	return_trace (মিথ্যা);

    return_trace (সত্য);
  }

  hb_position_t get_kerning (hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font,
			     ধ্রুবক শূন্য *base) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা idx = kern;
    যদি (unlikely (idx >= ARRAY_দৈর্ঘ্য (mathKern))) ফেরত 0;
    ফেরত (base+mathKern[idx]).get_মান (correction_height, font);
  }

  unsigned পূর্ণসংখ্যা get_kernings (hb_ot_math_kern_t kern,
			     unsigned পূর্ণসংখ্যা start_offset,
			     unsigned পূর্ণসংখ্যা *entries_গুনতি, /* IN/OUT */
			     hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			     hb_font_t *font,
			     ধ্রুবক শূন্য *base) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা idx = kern;
    যদি (unlikely (idx >= ARRAY_দৈর্ঘ্য (mathKern)) || !mathKern[idx]) {
      যদি (entries_গুনতি) *entries_গুনতি = 0;
      ফেরত 0;
    }
    ফেরত (base+mathKern[idx]).get_entries (start_offset,
					     entries_গুনতি,
					     kern_entries,
					     font);
  }

  সুরক্ষিত:
  /* Offset to MathKern table for each corner -
   * from the beginning of MathKernInfo table.  May be NULL. */
  Offset16To<MathKern> mathKern[4];

  সর্বজনীন:
  DEFINE_আকার_STATIC (8);
};

কাঠামো MathKernInfo
{
  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    ধ্রুবক hb_set_t &glyphset = c->plan->_glyphset_mathed;
    ধ্রুবক hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (এই+mathKernCoverage, mathKernInfoRecords)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->mathKernInfoRecords, এই, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->mathKernCoverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (সত্য);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুবক
  {
    জন্য (ধ্রুবক auto &entry : hb_zip (এই+mathKernCoverage, mathKernInfoRecords))
      যদি (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, এই);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  mathKernCoverage.sanitize (c, এই) &&
		  mathKernInfoRecords.sanitize (c, এই));
  }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা সূচক_2 = (এই+mathKernCoverage).get_coverage (glyph);
    ফেরত mathKernInfoRecords[সূচক_2].get_kerning (kern, correction_height, font, এই);
  }

  unsigned পূর্ণসংখ্যা get_kernings (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     unsigned পূর্ণসংখ্যা start_offset,
			     unsigned পূর্ণসংখ্যা *entries_গুনতি, /* IN/OUT */
			     hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			     hb_font_t *font) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা সূচক_2 = (এই+mathKernCoverage).get_coverage (glyph);
    ফেরত mathKernInfoRecords[সূচক_2].get_kernings (kern,
						    start_offset,
						    entries_গুনতি,
						    kern_entries,
						    font,
						    এই);
  }

  সুরক্ষিত:
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

  সর্বজনীন:
  DEFINE_আকার_ARRAY (4, mathKernInfoRecords);
};

কাঠামো MathGlyphInfo
{
  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->embed (*এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    out->mathItalicsCorrectionInfo.serialize_subset (c, mathItalicsCorrectionInfo, এই);
    out->math_শীর্ষ_Accent_Attachment.serialize_subset (c, math_শীর্ষ_Accent_Attachment, এই);

    ধ্রুবক hb_set_t &glyphset = c->plan->_glyphset_mathed;
    ধ্রুবক hb_map_t &glyph_map = *c->plan->glyph_map;

    auto it =
    + hb_iter (এই+extendedShapeCoverage)
    | hb_take (c->plan->source->get_অঙ্ক_glyphs ())
    | hb_filter (glyphset)
    | hb_map_retains_sorting (glyph_map)
    ;

    যদি (it) out->extendedShapeCoverage.serialize_serialize (c->serializer, it);
    অন্যথা out->extendedShapeCoverage = 0;

    out->mathKernInfo.serialize_subset (c, mathKernInfo, এই);
    return_trace (সত্য);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুবক
  {
    (এই+mathItalicsCorrectionInfo).collect_variation_indices (c);
    (এই+math_শীর্ষ_Accent_Attachment).collect_variation_indices (c);
    (এই+mathKernInfo).collect_variation_indices (c);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  mathItalicsCorrectionInfo.sanitize (c, এই) &&
		  math_শীর্ষ_Accent_Attachment.sanitize (c, এই) &&
		  extendedShapeCoverage.sanitize (c, এই) &&
		  mathKernInfo.sanitize (c, এই));
  }

  hb_position_t
  get_italics_correction (hb_codepoint_t  glyph, hb_font_t *font) ধ্রুবক
  { ফেরত (এই+mathItalicsCorrectionInfo).get_মান (glyph, font); }

  hb_position_t
  get_শীর্ষ_accent_attachment (hb_codepoint_t  glyph, hb_font_t *font) ধ্রুবক
  { ফেরত (এই+math_শীর্ষ_Accent_Attachment).get_মান (glyph, font); }

  যৌক্তিক is_extended_shape (hb_codepoint_t glyph) ধ্রুবক
  { ফেরত (এই+extendedShapeCoverage).get_coverage (glyph) != NOT_COVERED; }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font) ধ্রুবক
  { ফেরত (এই+mathKernInfo).get_kerning (glyph, kern, correction_height, font); }

  hb_position_t get_kernings (hb_codepoint_t glyph,
			      hb_ot_math_kern_t kern,
			      unsigned পূর্ণসংখ্যা start_offset,
			      unsigned পূর্ণসংখ্যা *entries_গুনতি, /* IN/OUT */
			      hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			      hb_font_t *font) ধ্রুবক
  { ফেরত (এই+mathKernInfo).get_kernings (glyph,
					     kern,
					     start_offset,
					     entries_গুনতি,
					     kern_entries,
					     font); }

  সুরক্ষিত:
  /* Offset to MathItalicsCorrectionInfo table -
   * from the beginning of MathGlyphInfo table. */
  Offset16To<MathItalicsCorrectionInfo> mathItalicsCorrectionInfo;

  /* Offset to MathTopAccentAttachment table -
   * from the beginning of MathGlyphInfo table. */
  Offset16To<Math_শীর্ষ_Accent_Attachment> math_শীর্ষ_Accent_Attachment;

  /* Offset to coverage table for Extended Shape glyphs -
   * from the beginning of MathGlyphInfo table. When the left or right glyph of
   * a box is an extended shape variant, the (ink) box (and not the default
   * position defined by values in MathConstants table) should be used for
   * vertical positioning purposes.  May be NULL.. */
  Offset16To<Coverage> extendedShapeCoverage;

   /* Offset to MathKernInfo table -
    * from the beginning of MathGlyphInfo table. */
  Offset16To<MathKernInfo> mathKernInfo;

  সর্বজনীন:
  DEFINE_আকার_STATIC (8);
};

কাঠামো MathGlyphVariantRecord
{
  friend কাঠামো MathGlyphConstruction;

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->embed (এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    ধ্রুবক hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_assign (out->variantGlyph, glyph_map.get (variantGlyph), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  শূন্য depend (hb_depend_তথ্য_builder_t *depend_তথ্য, unsigned source) ধ্রুবক
  { depend_তথ্য->add_depend(source, HB_OT_TAG_MATH, variantGlyph); }

  শূন্য closure_glyphs (hb_set_t *variant_glyphs) ধ্রুবক
  { variant_glyphs->add (variantGlyph); }

  সুরক্ষিত:
  HBGlyphID16 variantGlyph;       /* Glyph ID for the variant. */
  HBUINT16  advanceMeasurement; /* Advance width/height, in design units, of the
				 * variant, in the direction of requested
				 * glyph extension. */

  সর্বজনীন:
  DEFINE_আকার_STATIC (4);
};

কাঠামো PartFlags : HBUINT16
{
  গণনা Flags {
    Extender	= 0x0001u, /* If set, the part can be skipped or repeated. */

    Defined	= 0x0001u, /* All defined flags. */
  };

  সর্বজনীন:
  DEFINE_আকার_STATIC (2);
};

কাঠামো MathGlyphPartRecord
{
  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->embed (এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    ধ্রুবক hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_assign (out->glyph, glyph_map.get (glyph), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  শূন্য extract (hb_ot_math_glyph_part_t &out,
		int64_t mult,
		hb_font_t *font) ধ্রুবক
  {
    out.glyph			= glyph;

    out.start_connector_দৈর্ঘ্য	= font->em_mult (start_Connector_দৈর্ঘ্য, mult);
    out.end_connector_দৈর্ঘ্য	= font->em_mult (end_Connector_দৈর্ঘ্য, mult);
    out.full_advance		= font->em_mult (fullAdvance, mult);

    static_assert ((unsigned পূর্ণসংখ্যা) HB_OT_MATH_GLYPH_PART_FLAG_EXTENDER ==
		   (unsigned পূর্ণসংখ্যা) PartFlags::Extender, "");

    out.flags = (hb_ot_math_glyph_part_flags_t)
		(unsigned পূর্ণসংখ্যা)
		(partFlags & PartFlags::Defined);
  }

  শূন্য depend (hb_depend_তথ্য_builder_t *depend_তথ্য, unsigned source) ধ্রুবক
  { depend_তথ্য->add_depend(source, HB_OT_TAG_MATH, glyph); }

  শূন্য closure_glyphs (hb_set_t *variant_glyphs) ধ্রুবক
  { variant_glyphs->add (glyph); }

  সুরক্ষিত:
  HBGlyphID16	glyph;		/* Glyph ID for the part. */
  HBUINT16	start_Connector_দৈর্ঘ্য;
				/* Advance width/ height of the straight bar
				 * connector material, in design units, is at
				 * the beginning of the glyph, in the
				 * direction of the extension. */
  HBUINT16	end_Connector_দৈর্ঘ্য;
				/* Advance width/ height of the straight bar
				 * connector material, in design units, is at
				 * the end of the glyph, in the direction of
				 * the extension. */
  HBUINT16	fullAdvance;	/* Full advance width/height for this part,
				 * in the direction of the extension.
				 * In design units. */
  PartFlags	partFlags;	/* Part qualifiers. */

  সর্বজনীন:
  DEFINE_আকার_STATIC (10);
};

কাঠামো MathGlyphAssembly
{
  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);

    যদি (!c->serializer->copy (italicsCorrection, এই, &c->plan->layout_variation_idx_delta_map)) return_trace (মিথ্যা);
    যদি (!c->serializer->copy<HBUINT16> (partRecords.len)) return_trace (মিথ্যা);

    জন্য (ধ্রুবক auto& record : partRecords.iter ())
      যদি (!record.subset (c)) return_trace (মিথ্যা);
    return_trace (সত্য);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুবক
  {
    italicsCorrection.collect_variation_indices (c, এই);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  italicsCorrection.sanitize (c, এই) &&
		  partRecords.sanitize (c));
  }

  unsigned পূর্ণসংখ্যা get_parts (hb_direction_t direction,
			  hb_font_t *font,
			  unsigned পূর্ণসংখ্যা start_offset,
			  unsigned পূর্ণসংখ্যা *parts_গুনতি, /* IN/OUT */
			  hb_ot_math_glyph_part_t *parts /* OUT */,
			  hb_position_t *italics_correction /* OUT */) ধ্রুবক
  {
    যদি (parts_গুনতি && parts)
    {
      int64_t mult = font->dir_mult (direction);
      জন্য (auto _ : hb_zip (partRecords.as_array ().sub_array (start_offset, parts_গুনতি),
			    hb_array (parts, *parts_গুনতি)))
	_.first.extract (_.second, mult, font);
    }

    যদি (italics_correction)
      *italics_correction = italicsCorrection.get_x_মান (font, এই);

    ফেরত partRecords.len;
  }

  শূন্য depend (hb_depend_তথ্য_builder_t *depend_তথ্য, unsigned source) ধ্রুবক
  {
    জন্য (ধ্রুবক auto& _ : partRecords.iter ())
      _.depend (depend_তথ্য, source);
  }

  শূন্য closure_glyphs (hb_set_t *variant_glyphs) ধ্রুবক
  {
    জন্য (ধ্রুবক auto& _ : partRecords.iter ())
      _.closure_glyphs (variant_glyphs);
  }

  সুরক্ষিত:
  Math_মান_Record
		italicsCorrection;
				/* Italics correction of this
				 * MathGlyphAssembly. Should not
				 * depend on the assembly size. */
  Array16Of<MathGlyphPartRecord>
		partRecords;	/* Array of part records, from
				 * left to right and bottom to
				 * top. */

  সর্বজনীন:
  DEFINE_আকার_ARRAY (6, partRecords);
};

কাঠামো MathGlyphConstruction
{
  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);

    out->glyphAssembly.serialize_subset (c, glyphAssembly, এই);

    যদি (!c->serializer->check_assign (out->mathGlyphVariantRecord.len, mathGlyphVariantRecord.len, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (মিথ্যা);
    জন্য (ধ্রুবক auto& record : mathGlyphVariantRecord.iter ())
      যদি (!record.subset (c)) return_trace (মিথ্যা);

    return_trace (সত্য);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুবক
  {
    (এই+glyphAssembly).collect_variation_indices (c);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  glyphAssembly.sanitize (c, এই) &&
		  mathGlyphVariantRecord.sanitize (c));
  }

  ধ্রুবক MathGlyphAssembly &get_assembly () ধ্রুবক { ফেরত এই+glyphAssembly; }

  unsigned পূর্ণসংখ্যা get_variants (hb_direction_t direction,
			     hb_font_t *font,
			     unsigned পূর্ণসংখ্যা start_offset,
			     unsigned পূর্ণসংখ্যা *variants_গুনতি, /* IN/OUT */
			     hb_ot_math_glyph_variant_t *variants /* OUT */) ধ্রুবক
  {
    যদি (variants_গুনতি && variants)
    {
      int64_t mult = font->dir_mult (direction);
      জন্য (auto _ : hb_zip (mathGlyphVariantRecord.as_array ().sub_array (start_offset, variants_গুনতি),
			    hb_array (variants, *variants_গুনতি)))
	_.second = {_.first.variantGlyph, font->em_mult (_.first.advanceMeasurement, mult)};
    }
    ফেরত mathGlyphVariantRecord.len;
  }

  শূন্য depend (hb_depend_তথ্য_builder_t *depend_তথ্য, unsigned source) ধ্রুবক
  {
    (এই+glyphAssembly).depend (depend_তথ্য, source);

    জন্য (ধ্রুবক auto& _ : mathGlyphVariantRecord.iter ())
      _.depend (depend_তথ্য, source);
  }

  শূন্য closure_glyphs (hb_set_t *variant_glyphs) ধ্রুবক
  {
    (এই+glyphAssembly).closure_glyphs (variant_glyphs);

    জন্য (ধ্রুবক auto& _ : mathGlyphVariantRecord.iter ())
      _.closure_glyphs (variant_glyphs);
  }

  সুরক্ষিত:
  /* Offset to MathGlyphAssembly table for this shape - from the beginning of
     MathGlyphConstruction table.  May be NULL. */
  Offset16To<MathGlyphAssembly>	  glyphAssembly;

  /* MathGlyphVariantRecords for alternative variants of the glyphs. */
  Array16Of<MathGlyphVariantRecord> mathGlyphVariantRecord;

  সর্বজনীন:
  DEFINE_আকার_ARRAY (4, mathGlyphVariantRecord);
};

কাঠামো MathVariants
{
  শূন্য depend (hb_depend_তথ্য_builder_t *depend_তথ্য) ধ্রুবক
  {
    ধ্রুবক hb_array_t<ধ্রুবক Offset16To<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vert_Glyph_গুনতি + horiz_Glyph_গুনতি);

    যদি (vertGlyphCoverage)
    {
      ধ্রুবক auto vert_offsets = glyph_construction_offsets.sub_array (0, vert_Glyph_গুনতি);
      + hb_zip (এই+vertGlyphCoverage, vert_offsets)
      | hb_apply ([&] (ধ্রুবক hb_pair_t<hb_codepoint_t, ধ্রুবক Offset16To<MathGlyphConstruction>&> &_)
                  {
                    ধ্রুবক MathGlyphConstruction &mgc = এই+_.second;
                    mgc.depend (depend_তথ্য, _.first);
                  })
      ;
    }
    যদি (horizGlyphCoverage)
    {
      ধ্রুবক auto hori_offsets = glyph_construction_offsets.sub_array (vert_Glyph_গুনতি, horiz_Glyph_গুনতি);
      + hb_zip (এই+horizGlyphCoverage, hori_offsets)
      | hb_apply ([&] (ধ্রুবক hb_pair_t<hb_codepoint_t, ধ্রুবক Offset16To<MathGlyphConstruction>&> &_)
                  {
                    ধ্রুবক MathGlyphConstruction &mgc = এই+_.second;
                    mgc.depend (depend_তথ্য, _.first);
                  })
      ;
    }
  }

  শূন্য closure_glyphs (ধ্রুবক hb_set_t *glyph_set,
                       hb_set_t *variant_glyphs) ধ্রুবক
  {
    ধ্রুবক hb_array_t<ধ্রুবক Offset16To<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vert_Glyph_গুনতি + horiz_Glyph_গুনতি);

    যদি (vertGlyphCoverage)
    {
      ধ্রুবক auto vert_offsets = glyph_construction_offsets.sub_array (0, vert_Glyph_গুনতি);
      + hb_zip (এই+vertGlyphCoverage, vert_offsets)
      | hb_filter (glyph_set, hb_first)
      | hb_map (hb_second)
      | hb_map (hb_add (এই))
      | hb_apply ([=] (ধ্রুবক MathGlyphConstruction &_) { _.closure_glyphs (variant_glyphs); })
      ;
    }

    যদি (horizGlyphCoverage)
    {
      ধ্রুবক auto hori_offsets = glyph_construction_offsets.sub_array (vert_Glyph_গুনতি, horiz_Glyph_গুনতি);
      + hb_zip (এই+horizGlyphCoverage, hori_offsets)
      | hb_filter (glyph_set, hb_first)
      | hb_map (hb_second)
      | hb_map (hb_add (এই))
      | hb_apply ([=] (ধ্রুবক MathGlyphConstruction &_) { _.closure_glyphs (variant_glyphs); })
      ;
    }
  }

  শূন্য collect_coverage_and_indices (hb_sorted_vector_t<hb_codepoint_t>& new_coverage,
                                     ধ্রুবক Offset16To<Coverage>& coverage,
                                     unsigned i,
                                     unsigned end_সূচক,
                                     hb_set_t& indices,
                                     ধ্রুবক hb_set_t& glyphset,
                                     ধ্রুবক hb_map_t& glyph_map) ধ্রুবক
  {
    যদি (!coverage) ফেরত;

    জন্য (ধ্রুবক auto _ : (এই+coverage).iter ())
    {
      যদি (i >= end_সূচক) ফেরত;
      যদি (glyphset.has (_))
      {
        unsigned new_gid = glyph_map.get (_);
        new_coverage.push (new_gid);
        indices.add (i);
      }
      i++;
    }
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    ধ্রুবক hb_set_t &glyphset = c->plan->_glyphset_cmaped;
    ধ্রুবক hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);
    যদি (!c->serializer->check_assign (out->minConnectorOverlap, minConnectorOverlap, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (মিথ্যা);

    hb_sorted_vector_t<hb_codepoint_t> new_vert_coverage;
    hb_sorted_vector_t<hb_codepoint_t> new_hori_coverage;
    hb_set_t indices;
    collect_coverage_and_indices (new_vert_coverage, vertGlyphCoverage, 0, vert_Glyph_গুনতি, indices, glyphset, glyph_map);
    collect_coverage_and_indices (new_hori_coverage, horizGlyphCoverage, vert_Glyph_গুনতি, vert_Glyph_গুনতি + horiz_Glyph_গুনতি, indices, glyphset, glyph_map);

    যদি (!c->serializer->check_assign (out->vert_Glyph_গুনতি, new_vert_coverage.দৈর্ঘ্য, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (মিথ্যা);
    যদি (!c->serializer->check_assign (out->horiz_Glyph_গুনতি, new_hori_coverage.দৈর্ঘ্য, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (মিথ্যা);

    জন্য (unsigned i : indices.iter ())
    {
      auto *o = c->serializer->embed (glyphConstruction[i]);
      যদি (!o) return_trace (মিথ্যা);
      o->serialize_subset (c, glyphConstruction[i], এই);
    }

    যদি (new_vert_coverage)
      out->vertGlyphCoverage.serialize_serialize (c->serializer, new_vert_coverage.iter ());

    যদি (new_hori_coverage)
    out->horizGlyphCoverage.serialize_serialize (c->serializer, new_hori_coverage.iter ());
    return_trace (সত্য);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুবক
  {
    জন্য (ধ্রুবক auto &entry : hb_zip (এই+vertGlyphCoverage, glyphConstruction.as_array (vert_Glyph_গুনতি)))
      যদি (c->glyph_set->has (hb_first (entry)))
        (এই+hb_second (entry)).collect_variation_indices (c);
    জন্য (ধ্রুবক auto &entry : hb_zip (এই+horizGlyphCoverage, glyphConstruction.as_array (vert_Glyph_গুনতি + horiz_Glyph_গুনতি).sub_array (vert_Glyph_গুনতি)))
      যদি (c->glyph_set->has (hb_first (entry)))
        (এই+hb_second (entry)).collect_variation_indices (c);
  }

  যৌক্তিক sanitize_offsets (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    unsigned পূর্ণসংখ্যা গুনতি = vert_Glyph_গুনতি + horiz_Glyph_গুনতি;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
      যদি (!glyphConstruction.arrayZ[i].sanitize (c, এই)) return_trace (মিথ্যা);
    return_trace (সত্য);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  vertGlyphCoverage.sanitize (c, এই) &&
		  horizGlyphCoverage.sanitize (c, এই) &&
		  hb_barrier () &&
		  c->check_array (glyphConstruction.arrayZ, vert_Glyph_গুনতি + horiz_Glyph_গুনতি) &&
		  sanitize_offsets (c));
  }

  hb_position_t get_min_connector_overlap (hb_direction_t direction,
						  hb_font_t *font) ধ্রুবক
  { ফেরত font->em_scale_dir (minConnectorOverlap, direction); }

  unsigned পূর্ণসংখ্যা get_glyph_variants (hb_codepoint_t glyph,
				   hb_direction_t direction,
				   hb_font_t *font,
				   unsigned পূর্ণসংখ্যা start_offset,
				   unsigned পূর্ণসংখ্যা *variants_গুনতি, /* IN/OUT */
				   hb_ot_math_glyph_variant_t *variants /* OUT */) ধ্রুবক
  { ফেরত get_glyph_construction (glyph, direction, font)
	   .get_variants (direction, font, start_offset, variants_গুনতি, variants); }

  unsigned পূর্ণসংখ্যা get_glyph_parts (hb_codepoint_t glyph,
				hb_direction_t direction,
				hb_font_t *font,
				unsigned পূর্ণসংখ্যা start_offset,
				unsigned পূর্ণসংখ্যা *parts_গুনতি, /* IN/OUT */
				hb_ot_math_glyph_part_t *parts /* OUT */,
				hb_position_t *italics_correction /* OUT */) ধ্রুবক
  { ফেরত get_glyph_construction (glyph, direction, font)
	   .get_assembly ()
	   .get_parts (direction, font,
		       start_offset, parts_গুনতি, parts,
		       italics_correction); }

  ব্যক্তিগত:
  ধ্রুবক MathGlyphConstruction &
  get_glyph_construction (hb_codepoint_t glyph,
			  hb_direction_t direction,
			  hb_font_t *font HB_UNUSED) ধ্রুবক
  {
    যৌক্তিক vertical = HB_DIRECTION_IS_VERTICAL (direction);
    unsigned পূর্ণসংখ্যা গুনতি = vertical ? vert_Glyph_গুনতি : horiz_Glyph_গুনতি;
    ধ্রুবক Offset16To<Coverage> &coverage = vertical ? vertGlyphCoverage
						  : horizGlyphCoverage;

    unsigned পূর্ণসংখ্যা সূচক_2 = (এই+coverage).get_coverage (glyph);
    যদি (unlikely (সূচক_2 >= গুনতি)) ফেরত Null (MathGlyphConstruction);

    যদি (!vertical)
      সূচক_2 += vert_Glyph_গুনতি;

    ফেরত এই+glyphConstruction[সূচক_2];
  }

  সুরক্ষিত:
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
  HBUINT16	vert_Glyph_গুনতি;	/* Number of glyphs for which
				 * information is provided for
				 * vertically growing variants. */
  HBUINT16	horiz_Glyph_গুনতি;/* Number of glyphs for which
				 * information is provided for
				 * horizontally growing variants. */

  /* Array of offsets to MathGlyphConstruction tables - from the beginning of
     the MathVariants table, for shapes growing in vertical/horizontal
     direction. */
  UnsizedArrayOf<Offset16To<MathGlyphConstruction>>
			glyphConstruction;

  সর্বজনীন:
  DEFINE_আকার_ARRAY (10, glyphConstruction);
};


/*
 * MATH -- Mathematical typesetting
 * https://docs.microsoft.com/en-us/typography/opentype/spec/math
 */

কাঠামো MATH
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_MATH;

  যৌক্তিক has_তথ্য () ধ্রুবক { ফেরত version.to_int (); }

  শূন্য depend (hb_depend_তথ্য_builder_t *depend_তথ্য) ধ্রুবক
  {
    যদি (mathVariants)
      (এই+mathVariants).depend (depend_তথ্য);
  }

  শূন্য closure_glyphs (hb_set_t *glyph_set) ধ্রুবক
  {
    যদি (mathVariants)
    {
      hb_set_t variant_glyphs;
      (এই+mathVariants).closure_glyphs (glyph_set, &variant_glyphs);
      hb_set_union (glyph_set, &variant_glyphs);
    }
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->embed (*এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    out->mathConstants.serialize_copy (c->serializer, mathConstants, এই, 0, hb_serialize_context_t::শীর্ষ, &c->plan->layout_variation_idx_delta_map);
    out->mathGlyphInfo.serialize_subset (c, mathGlyphInfo, এই);
    out->mathVariants.serialize_subset (c, mathVariants, এই);
    return_trace (সত্য);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c, ধ্রুবক hb_set_t *variant_glyph_set) ধ্রুবক
  {
    (এই+mathConstants).collect_variation_indices (c);
    (এই+mathGlyphInfo).collect_variation_indices (c);
    auto *glyph_set = c->glyph_set;
    c->glyph_set = variant_glyph_set;
    (এই+mathVariants).collect_variation_indices (c);
    c->glyph_set = glyph_set;
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (version.sanitize (c) &&
		  likely (version.major == 1) &&
		  hb_barrier () &&
		  mathConstants.sanitize (c, এই) &&
		  mathGlyphInfo.sanitize (c, এই) &&
		  mathVariants.sanitize (c, এই));
  }

  // https://github.com/harfbuzz/harfbuzz/issues/4653
  HB_INTERNAL যৌক্তিক is_bad_cambria (hb_font_t *font) ধ্রুবক
  {
#ifndef HB_NO_MATH
    নির্বাচন HB_CODEPOINT_ENCODE3 (font->face->table.MATH.get_blob ()->দৈর্ঘ্য,
                                 (এই+mathConstants).minHeight[1], // displayOperatorMinHeight
                                 (এই+mathConstants).minHeight[0]) // delimitedSubFormulaMinHeight
    {
      /* sha1sum:ab4a4fe054d23061f3c039493d6f665cfda2ecf5  cambria.ttc
       * sha1sum:086855301bff644f9d8827b88491fcf73a6d4cb9  cambria.ttc
       * sha1sum:b1e5a3feaca2ea3dfcf79ccb377de749ecf60343  cambria.ttc */
      ক্ষেত্র HB_CODEPOINT_ENCODE3 (25722, 2500, 3000):
        ফেরত সত্য;
    }
#endif
    ফেরত মিথ্যা;
  }

  hb_position_t get_constant (hb_ot_math_constant_t  constant,
			      hb_font_t		   *font) ধ্রুবক
  { ফেরত (এই+mathConstants).get_মান (constant, font); }

  ধ্রুবক MathGlyphInfo &get_glyph_info () ধ্রুবক { ফেরত এই+mathGlyphInfo; }

  ধ্রুবক MathVariants &get_variants () ধ্রুবক    { ফেরত এই+mathVariants; }

  সুরক্ষিত:
  FixedVersion<>version;	/* Version of the MATH table
				 * initially set to 0x00010000u */
  Offset16To<MathConstants>
		mathConstants;	/* MathConstants table */
  Offset16To<MathGlyphInfo>
		mathGlyphInfo;	/* MathGlyphInfo table */
  Offset16To<MathVariants>
		mathVariants;	/* MathVariants table */

  সর্বজনীন:
  DEFINE_আকার_STATIC (10);
};

} /* namespace OT */


#endif /* HB_OT_MATH_TABLE_HH */

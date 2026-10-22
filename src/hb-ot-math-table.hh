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

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-layout-common.hh"
#ضمن "hb-ot-layout-gdef-table.hh"
#ضمن "hb-ot-math.h"
#ضمن "hb-depend-data.hh"

نطاق OT {


بنية MathValueRecord
{
  hb_position_t get_x_value (hb_font_t *font, ثابت فراغ *base) ثابت
  { החזר hb_saturate_add (font->em_scale_x (value), (base+deviceTable).get_x_delta (font, get_var_store (font, base))); }
  hb_position_t get_y_value (hb_font_t *font, ثابت فراغ *base) ثابت
  { החזר hb_saturate_add (font->em_scale_y (value), (base+deviceTable).get_y_delta (font, get_var_store (font, base))); }

  ثابت ItemVariationStore &get_var_store (hb_font_t *font, ثابت فراغ *base) ثابت
  {
#אם !defined(HB_NO_VAR) && !defined(HB_NO_OT_LAYOUT)
    אם ((base+deviceTable).is_variation_device ())
      החזר font->face->table.GDEF->table->get_var_store ();
#endif
    החזר Null (ItemVariationStore);
  }

  MathValueRecord* copy (hb_serialize_context_t *c, ثابت فراغ *base,
                         ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> *variation_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    auto *out = c->embed (هذا);
    אם (unlikely (!out)) return_trace (nullptr);
    hb_pair_t<unsigned, صحيح> *varidx_delta;
    אם (variation_map->has ((base+deviceTable).get_variation_index (), &varidx_delta) &&
        !c->check_assign (out->value, (int64_t) value + hb_second (*varidx_delta),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (nullptr);
    out->deviceTable.serialize_copy (c, deviceTable, base, 0, hb_serialize_context_t::Head, variation_map);

    return_trace (out);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c, ثابت فراغ *base) ثابت
  { (base+deviceTable).collect_variation_indices (c); }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && deviceTable.sanitize (c, base));
  }

  محمي:
  HBINT16		value;		/* The X or Y value in design units */
  Offset16To<Device>	deviceTable;	/* Offset to the device table - from the
					 * beginning of parent table.  May be NULL.
					 * Suggested format for device table is 1. */

  عام:
  DEFINE_SIZE_STATIC (4);
};

بنية MathConstants
{
  friend بنية MATH;

  MathConstants* copy (hb_serialize_context_t *c,
                      ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> *variation_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    auto *out = c->start_embed (هذا);

    HBINT16 *p = c->allocate_size<HBINT16> (HBINT16::static_size * 2);
    אם (unlikely (!p)) return_trace (nullptr);
    hb_memcpy (p, percentScaleDown, HBINT16::static_size * 2);

    HBUINT16 *m = c->allocate_size<HBUINT16> (HBUINT16::static_size * 2);
    אם (unlikely (!m)) return_trace (nullptr);
    hb_memcpy (m, minHeight, HBUINT16::static_size * 2);

    unsigned count = ARRAY_LENGTH (mathValueRecords);
    עבור (unsigned i = 0; i < count; i++)
      אם (!c->copy (mathValueRecords[i], هذا, variation_map))
        return_trace (nullptr);

    אם (!c->embed (radicalDegreeBottomRaisePercent)) return_trace (nullptr);
    return_trace (out);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    עבור (ثابت auto &record : mathValueRecords)
      record.collect_variation_indices (c, هذا);
  }

  منطقي sanitize_math_value_records (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);

    unsigned صحيح count = ARRAY_LENGTH (mathValueRecords);
    עבור (unsigned صحيح i = 0; i < count; i++)
      אם (!mathValueRecords[i].sanitize (c, هذا))
	return_trace (שקר);

    return_trace (אמת);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && sanitize_math_value_records (c));
  }

  hb_position_t get_value (hb_ot_math_constant_t constant,
			   hb_font_t *font) ثابت
  {
    بدل (constant) {

    حالة HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_DOWN:
    حالة HB_OT_MATH_CONSTANT_SCRIPT_SCRIPT_PERCENT_SCALE_DOWN:
      החזר percentScaleDown[constant - HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_DOWN];

    حالة HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_HEIGHT:
    حالة HB_OT_MATH_CONSTANT_DISPLAY_OPERATOR_MIN_HEIGHT:
      החזר font->em_scale_y (minHeight[constant - HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_HEIGHT]);

    حالة HB_OT_MATH_CONSTANT_RADICAL_KERN_AFTER_DEGREE:
    حالة HB_OT_MATH_CONSTANT_RADICAL_KERN_BEFORE_DEGREE:
    حالة HB_OT_MATH_CONSTANT_SKEWED_FRACTION_HORIZONTAL_GAP:
    حالة HB_OT_MATH_CONSTANT_SPACE_AFTER_SCRIPT:
      החזר mathValueRecords[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_x_value (font, هذا);

    حالة HB_OT_MATH_CONSTANT_ACCENT_BASE_HEIGHT:
    حالة HB_OT_MATH_CONSTANT_AXIS_HEIGHT:
    حالة HB_OT_MATH_CONSTANT_FLATTENED_ACCENT_BASE_HEIGHT:
    حالة HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_DISPLAY_STYLE_SHIFT_DOWN:
    حالة HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_SHIFT_DOWN:
    حالة HB_OT_MATH_CONSTANT_FRACTION_DENOM_DISPLAY_STYLE_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_DISPLAY_STYLE_SHIFT_UP:
    حالة HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_SHIFT_UP:
    حالة HB_OT_MATH_CONSTANT_FRACTION_NUM_DISPLAY_STYLE_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_FRACTION_RULE_THICKNESS:
    حالة HB_OT_MATH_CONSTANT_LOWER_LIMIT_BASELINE_DROP_MIN:
    حالة HB_OT_MATH_CONSTANT_LOWER_LIMIT_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_MATH_LEADING:
    حالة HB_OT_MATH_CONSTANT_OVERBAR_EXTRA_ASCENDER:
    حالة HB_OT_MATH_CONSTANT_OVERBAR_RULE_THICKNESS:
    حالة HB_OT_MATH_CONSTANT_OVERBAR_VERTICAL_GAP:
    حالة HB_OT_MATH_CONSTANT_RADICAL_DISPLAY_STYLE_VERTICAL_GAP:
    حالة HB_OT_MATH_CONSTANT_RADICAL_EXTRA_ASCENDER:
    حالة HB_OT_MATH_CONSTANT_RADICAL_RULE_THICKNESS:
    حالة HB_OT_MATH_CONSTANT_RADICAL_VERTICAL_GAP:
    حالة HB_OT_MATH_CONSTANT_SKEWED_FRACTION_VERTICAL_GAP:
    حالة HB_OT_MATH_CONSTANT_STACK_BOTTOM_DISPLAY_STYLE_SHIFT_DOWN:
    حالة HB_OT_MATH_CONSTANT_STACK_BOTTOM_SHIFT_DOWN:
    حالة HB_OT_MATH_CONSTANT_STACK_DISPLAY_STYLE_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_STACK_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_STACK_TOP_DISPLAY_STYLE_SHIFT_UP:
    حالة HB_OT_MATH_CONSTANT_STACK_TOP_SHIFT_UP:
    حالة HB_OT_MATH_CONSTANT_STRETCH_STACK_BOTTOM_SHIFT_DOWN:
    حالة HB_OT_MATH_CONSTANT_STRETCH_STACK_GAP_ABOVE_MIN:
    حالة HB_OT_MATH_CONSTANT_STRETCH_STACK_GAP_BELOW_MIN:
    حالة HB_OT_MATH_CONSTANT_STRETCH_STACK_TOP_SHIFT_UP:
    حالة HB_OT_MATH_CONSTANT_SUBSCRIPT_BASELINE_DROP_MIN:
    حالة HB_OT_MATH_CONSTANT_SUBSCRIPT_SHIFT_DOWN:
    حالة HB_OT_MATH_CONSTANT_SUBSCRIPT_TOP_MAX:
    حالة HB_OT_MATH_CONSTANT_SUB_SUPERSCRIPT_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_SUPERSCRIPT_BASELINE_DROP_MAX:
    حالة HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MAX_WITH_SUBSCRIPT:
    حالة HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MIN:
    حالة HB_OT_MATH_CONSTANT_SUPERSCRIPT_SHIFT_UP:
    حالة HB_OT_MATH_CONSTANT_SUPERSCRIPT_SHIFT_UP_CRAMPED:
    حالة HB_OT_MATH_CONSTANT_UNDERBAR_EXTRA_DESCENDER:
    حالة HB_OT_MATH_CONSTANT_UNDERBAR_RULE_THICKNESS:
    حالة HB_OT_MATH_CONSTANT_UNDERBAR_VERTICAL_GAP:
    حالة HB_OT_MATH_CONSTANT_UPPER_LIMIT_BASELINE_RISE_MIN:
    حالة HB_OT_MATH_CONSTANT_UPPER_LIMIT_GAP_MIN:
      החזר mathValueRecords[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_y_value (font, هذا);

    حالة HB_OT_MATH_CONSTANT_RADICAL_DEGREE_BOTTOM_RAISE_PERCENT:
      החזר radicalDegreeBottomRaisePercent;

    افتراضي:
      החזר 0;
    }
  }

  محمي:
  HBINT16 percentScaleDown[2];
  HBUINT16 minHeight[2];
  MathValueRecord mathValueRecords[51];
  HBINT16 radicalDegreeBottomRaisePercent;

  عام:
  DEFINE_SIZE_STATIC (214);
};

بنية MathItalicsCorrectionInfo
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_set_t &glyphset = c->plan->_glyphset_mathed;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*هذا);
    אם (unlikely (!c->serializer->extend_min (out))) return_trace (שקר);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+coverage, italicsCorrection)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->italicsCorrection, هذا, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (אמת);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    עבור (ثابت auto &entry : hb_zip (هذا+coverage, italicsCorrection))
      אם (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, هذا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  coverage.sanitize (c, هذا) &&
		  italicsCorrection.sanitize (c, هذا));
  }

  hb_position_t get_value (hb_codepoint_t glyph,
			   hb_font_t *font) ثابت
  {
    unsigned صحيح index = (هذا+coverage).get_coverage (glyph);
    החזר italicsCorrection[index].get_x_value (font, هذا);
  }

  محمي:
  Offset16To<Coverage>       coverage;		/* Offset to Coverage table -
						 * from the beginning of
						 * MathItalicsCorrectionInfo
						 * table. */
  Array16Of<MathValueRecord> italicsCorrection;	/* Array of MathValueRecords
						 * defining italics correction
						 * values for each
						 * covered glyph. */

  عام:
  DEFINE_SIZE_ARRAY (4, italicsCorrection);
};

بنية MathTopAccentAttachment
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_set_t &glyphset = c->plan->_glyphset_mathed;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*هذا);
    אם (unlikely (!c->serializer->extend_min (out))) return_trace (שקר);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+topAccentCoverage, topAccentAttachment)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->topAccentAttachment, هذا, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->topAccentCoverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (אמת);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    עבור (ثابت auto &entry : hb_zip (هذا+topAccentCoverage, topAccentAttachment))
      אם (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, هذا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  topAccentCoverage.sanitize (c, هذا) &&
		  topAccentAttachment.sanitize (c, هذا));
  }

  hb_position_t get_value (hb_codepoint_t glyph,
			   hb_font_t *font) ثابت
  {
    unsigned صحيح index = (هذا+topAccentCoverage).get_coverage (glyph);
    אם (index == NOT_COVERED)
      החזר font->get_glyph_h_advance (glyph) / 2;
    החזר topAccentAttachment[index].get_x_value (font, هذا);
  }

  محمي:
  Offset16To<Coverage>       topAccentCoverage;   /* Offset to Coverage table -
						 * from the beginning of
						 * MathTopAccentAttachment
						 * table. */
  Array16Of<MathValueRecord> topAccentAttachment; /* Array of MathValueRecords
						 * defining top accent
						 * attachment points for each
						 * covered glyph. */

  عام:
  DEFINE_SIZE_ARRAY (2 + 2, topAccentAttachment);
};

بنية MathKern
{
  MathKern* copy (hb_serialize_context_t *c,
                      ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> *variation_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    auto *out = c->start_embed (هذا);

    אם (unlikely (!c->embed (heightCount))) return_trace (nullptr);

    unsigned count = 2 * heightCount + 1;
    עבור (unsigned i = 0; i < count; i++)
      אם (!c->copy (mathValueRecordsZ.arrayZ[i], هذا, variation_map))
        return_trace (nullptr);

    return_trace (out);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    עבור (ثابت auto &record : mathValueRecordsZ.as_array (2 * heightCount + 1))
      record.collect_variation_indices (c, هذا);
  }

  منطقي sanitize_math_value_records (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    unsigned صحيح count = 2 * heightCount + 1;
    עבור (unsigned صحيح i = 0; i < count; i++)
      אם (!mathValueRecordsZ.arrayZ[i].sanitize (c, هذا)) return_trace (שקר);
    return_trace (אמת);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  c->check_array (mathValueRecordsZ.arrayZ, 2 * heightCount + 1) &&
		  sanitize_math_value_records (c));
  }

  hb_position_t get_value (hb_position_t correction_height, hb_font_t *font) ثابت
  {
    ثابت MathValueRecord* correctionHeight = mathValueRecordsZ.arrayZ;
    ثابت MathValueRecord* kernValue = mathValueRecordsZ.arrayZ + heightCount;
    صحيح sign = font->y_scale < 0 ? -1 : +1;

    /* According to OpenType spec (v1.9), except for the boundary cases, the index
     * chosen for kern value should be i such that
     *    correctionHeight[i-1] <= correction_height < correctionHeight[i]
     * We can use the binary search algorithm of std::upper_bound(). Or, we can
     * use the internal hb_bsearch_impl.
     */
    unsigned صحيح pos;
    auto cmp = +[](ثابت فراغ* key, ثابت فراغ* p,
                   صحيح sign, hb_font_t* font, ثابت MathKern* mathKern) -> صحيح {
      hb_position_t a = *(hb_position_t *) key;
      hb_position_t b = ((MathValueRecord *) p)->get_y_value (font, mathKern);
      אם (a == b) החזר 0;
      החזר sign > 0 ? (a < b ? -1 : +1) : (b < a ? -1 : +1);
    };
    unsigned صحيح i = hb_bsearch_impl(&pos, correction_height, correctionHeight,
                                     heightCount, MathValueRecord::static_size,
                                     cmp, sign, font, هذا) ? pos + 1 : pos;
    החזר kernValue[i].get_x_value (font, هذا);
  }

  unsigned صحيح get_entries (unsigned صحيح start_offset,
			    unsigned صحيح *entries_count, /* IN/OUT */
			    hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			    hb_font_t *font) ثابت
  {
    ثابت MathValueRecord* correctionHeight = mathValueRecordsZ.arrayZ;
    ثابت MathValueRecord* kernValue = mathValueRecordsZ.arrayZ + heightCount;
    ثابت unsigned صحيح entriesCount = heightCount + 1;

    אם (entries_count && kern_entries)
    {
      unsigned صحيح start = hb_min (start_offset, entriesCount);
      unsigned صحيح end = hb_min (start + *entries_count, entriesCount);
      *entries_count = end - start;

      עבור (unsigned صحيح i = 0; i < *entries_count; i++) {
	unsigned صحيح j = start + i;

	hb_position_t max_height;
	אם (j == heightCount) {
	  max_height = INT32_MAX;
	} אחרת {
	  max_height = correctionHeight[j].get_y_value (font, هذا);
	}

	kern_entries[i] = {max_height, kernValue[j].get_x_value (font, هذا)};
      }
    }
    החזר entriesCount;
  }

  محمي:
  HBUINT16	heightCount;
  UnsizedArrayOf<MathValueRecord>
		mathValueRecordsZ;
				/* Array of correction heights at
				 * which the kern value changes.
				 * Sorted by the height value in
				 * design units (heightCount entries),
				 * Followed by:
				 * Array of kern values corresponding
				 * to heights. (heightCount+1 entries).
				 */

  عام:
  DEFINE_SIZE_ARRAY (2, mathValueRecordsZ);
};

بنية MathKernInfoRecord
{
  MathKernInfoRecord* copy (hb_serialize_context_t *c, ثابت فراغ *base,
                            ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> *variation_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    auto *out = c->embed (هذا);
    אם (unlikely (!out)) return_trace (nullptr);

    unsigned count = ARRAY_LENGTH (mathKern);
    עבור (unsigned i = 0; i < count; i++)
      out->mathKern[i].serialize_copy (c, mathKern[i], base, 0, hb_serialize_context_t::Head, variation_map);

    return_trace (out);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c, ثابت فراغ *base) ثابت
  {
    עבור (ثابت auto &offset : mathKern)
      (base+offset).collect_variation_indices (c);
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base) ثابت
  {
    TRACE_SANITIZE (هذا);

    unsigned صحيح count = ARRAY_LENGTH (mathKern);
    עבור (unsigned صحيح i = 0; i < count; i++)
      אם (unlikely (!mathKern[i].sanitize (c, base)))
	return_trace (שקר);

    return_trace (אמת);
  }

  hb_position_t get_kerning (hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font,
			     ثابت فراغ *base) ثابت
  {
    unsigned صحيح idx = kern;
    אם (unlikely (idx >= ARRAY_LENGTH (mathKern))) החזר 0;
    החזר (base+mathKern[idx]).get_value (correction_height, font);
  }

  unsigned صحيح get_kernings (hb_ot_math_kern_t kern,
			     unsigned صحيح start_offset,
			     unsigned صحيح *entries_count, /* IN/OUT */
			     hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			     hb_font_t *font,
			     ثابت فراغ *base) ثابت
  {
    unsigned صحيح idx = kern;
    אם (unlikely (idx >= ARRAY_LENGTH (mathKern)) || !mathKern[idx]) {
      אם (entries_count) *entries_count = 0;
      החזר 0;
    }
    החזר (base+mathKern[idx]).get_entries (start_offset,
					     entries_count,
					     kern_entries,
					     font);
  }

  محمي:
  /* Offset to MathKern table for each corner -
   * from the beginning of MathKernInfo table.  May be NULL. */
  Offset16To<MathKern> mathKern[4];

  عام:
  DEFINE_SIZE_STATIC (8);
};

بنية MathKernInfo
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_set_t &glyphset = c->plan->_glyphset_mathed;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*هذا);
    אם (unlikely (!c->serializer->extend_min (out))) return_trace (שקר);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+mathKernCoverage, mathKernInfoRecords)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->mathKernInfoRecords, هذا, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->mathKernCoverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (אמת);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    עבור (ثابت auto &entry : hb_zip (هذا+mathKernCoverage, mathKernInfoRecords))
      אם (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, هذا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  mathKernCoverage.sanitize (c, هذا) &&
		  mathKernInfoRecords.sanitize (c, هذا));
  }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font) ثابت
  {
    unsigned صحيح index = (هذا+mathKernCoverage).get_coverage (glyph);
    החזר mathKernInfoRecords[index].get_kerning (kern, correction_height, font, هذا);
  }

  unsigned صحيح get_kernings (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     unsigned صحيح start_offset,
			     unsigned صحيح *entries_count, /* IN/OUT */
			     hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			     hb_font_t *font) ثابت
  {
    unsigned صحيح index = (هذا+mathKernCoverage).get_coverage (glyph);
    החזר mathKernInfoRecords[index].get_kernings (kern,
						    start_offset,
						    entries_count,
						    kern_entries,
						    font,
						    هذا);
  }

  محمي:
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

  عام:
  DEFINE_SIZE_ARRAY (4, mathKernInfoRecords);
};

بنية MathGlyphInfo
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (*هذا);
    אם (unlikely (!out)) return_trace (שקר);

    out->mathItalicsCorrectionInfo.serialize_subset (c, mathItalicsCorrectionInfo, هذا);
    out->mathTopAccentAttachment.serialize_subset (c, mathTopAccentAttachment, هذا);

    ثابت hb_set_t &glyphset = c->plan->_glyphset_mathed;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    auto it =
    + hb_iter (هذا+extendedShapeCoverage)
    | hb_take (c->plan->source->get_num_glyphs ())
    | hb_filter (glyphset)
    | hb_map_retains_sorting (glyph_map)
    ;

    אם (it) out->extendedShapeCoverage.serialize_serialize (c->serializer, it);
    אחרת out->extendedShapeCoverage = 0;

    out->mathKernInfo.serialize_subset (c, mathKernInfo, هذا);
    return_trace (אמת);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    (هذا+mathItalicsCorrectionInfo).collect_variation_indices (c);
    (هذا+mathTopAccentAttachment).collect_variation_indices (c);
    (هذا+mathKernInfo).collect_variation_indices (c);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  mathItalicsCorrectionInfo.sanitize (c, هذا) &&
		  mathTopAccentAttachment.sanitize (c, هذا) &&
		  extendedShapeCoverage.sanitize (c, هذا) &&
		  mathKernInfo.sanitize (c, هذا));
  }

  hb_position_t
  get_italics_correction (hb_codepoint_t  glyph, hb_font_t *font) ثابت
  { החזר (هذا+mathItalicsCorrectionInfo).get_value (glyph, font); }

  hb_position_t
  get_top_accent_attachment (hb_codepoint_t  glyph, hb_font_t *font) ثابت
  { החזר (هذا+mathTopAccentAttachment).get_value (glyph, font); }

  منطقي is_extended_shape (hb_codepoint_t glyph) ثابت
  { החזר (هذا+extendedShapeCoverage).get_coverage (glyph) != NOT_COVERED; }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font) ثابت
  { החזר (هذا+mathKernInfo).get_kerning (glyph, kern, correction_height, font); }

  hb_position_t get_kernings (hb_codepoint_t glyph,
			      hb_ot_math_kern_t kern,
			      unsigned صحيح start_offset,
			      unsigned صحيح *entries_count, /* IN/OUT */
			      hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			      hb_font_t *font) ثابت
  { החזר (هذا+mathKernInfo).get_kernings (glyph,
					     kern,
					     start_offset,
					     entries_count,
					     kern_entries,
					     font); }

  محمي:
  /* Offset to MathItalicsCorrectionInfo table -
   * from the beginning of MathGlyphInfo table. */
  Offset16To<MathItalicsCorrectionInfo> mathItalicsCorrectionInfo;

  /* Offset to MathTopAccentAttachment table -
   * from the beginning of MathGlyphInfo table. */
  Offset16To<MathTopAccentAttachment> mathTopAccentAttachment;

  /* Offset to coverage table for Extended Shape glyphs -
   * from the beginning of MathGlyphInfo table. When the left or right glyph of
   * a box is an extended shape variant, the (ink) box (and not the default
   * position defined by values in MathConstants table) should be used for
   * vertical positioning purposes.  May be NULL.. */
  Offset16To<Coverage> extendedShapeCoverage;

   /* Offset to MathKernInfo table -
    * from the beginning of MathGlyphInfo table. */
  Offset16To<MathKernInfo> mathKernInfo;

  عام:
  DEFINE_SIZE_STATIC (8);
};

بنية MathGlyphVariantRecord
{
  friend بنية MathGlyphConstruction;

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (هذا);
    אם (unlikely (!out)) return_trace (שקר);

    ثابت hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_assign (out->variantGlyph, glyph_map.get (variantGlyph), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  فراغ depend (hb_depend_data_builder_t *depend_data, unsigned source) ثابت
  { depend_data->add_depend(source, HB_OT_TAG_MATH, variantGlyph); }

  فراغ closure_glyphs (hb_set_t *variant_glyphs) ثابت
  { variant_glyphs->add (variantGlyph); }

  محمي:
  HBGlyphID16 variantGlyph;       /* Glyph ID for the variant. */
  HBUINT16  advanceMeasurement; /* Advance width/height, in design units, of the
				 * variant, in the direction of requested
				 * glyph extension. */

  عام:
  DEFINE_SIZE_STATIC (4);
};

بنية PartFlags : HBUINT16
{
  تعداد Flags {
    Extender	= 0x0001u, /* If set, the part can be skipped or repeated. */

    Defined	= 0x0001u, /* All defined flags. */
  };

  عام:
  DEFINE_SIZE_STATIC (2);
};

بنية MathGlyphPartRecord
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (هذا);
    אם (unlikely (!out)) return_trace (שקר);

    ثابت hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_assign (out->glyph, glyph_map.get (glyph), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  فراغ extract (hb_ot_math_glyph_part_t &out,
		int64_t mult,
		hb_font_t *font) ثابت
  {
    out.glyph			= glyph;

    out.start_connector_length	= font->em_mult (startConnectorLength, mult);
    out.end_connector_length	= font->em_mult (endConnectorLength, mult);
    out.full_advance		= font->em_mult (fullAdvance, mult);

    static_assert ((unsigned صحيح) HB_OT_MATH_GLYPH_PART_FLAG_EXTENDER ==
		   (unsigned صحيح) PartFlags::Extender, "");

    out.flags = (hb_ot_math_glyph_part_flags_t)
		(unsigned صحيح)
		(partFlags & PartFlags::Defined);
  }

  فراغ depend (hb_depend_data_builder_t *depend_data, unsigned source) ثابت
  { depend_data->add_depend(source, HB_OT_TAG_MATH, glyph); }

  فراغ closure_glyphs (hb_set_t *variant_glyphs) ثابت
  { variant_glyphs->add (glyph); }

  محمي:
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

  عام:
  DEFINE_SIZE_STATIC (10);
};

بنية MathGlyphAssembly
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);

    אם (!c->serializer->copy (italicsCorrection, هذا, &c->plan->layout_variation_idx_delta_map)) return_trace (שקר);
    אם (!c->serializer->copy<HBUINT16> (partRecords.len)) return_trace (שקר);

    עבור (ثابت auto& record : partRecords.iter ())
      אם (!record.subset (c)) return_trace (שקר);
    return_trace (אמת);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    italicsCorrection.collect_variation_indices (c, هذا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  italicsCorrection.sanitize (c, هذا) &&
		  partRecords.sanitize (c));
  }

  unsigned صحيح get_parts (hb_direction_t direction,
			  hb_font_t *font,
			  unsigned صحيح start_offset,
			  unsigned صحيح *parts_count, /* IN/OUT */
			  hb_ot_math_glyph_part_t *parts /* OUT */,
			  hb_position_t *italics_correction /* OUT */) ثابت
  {
    אם (parts_count && parts)
    {
      int64_t mult = font->dir_mult (direction);
      עבור (auto _ : hb_zip (partRecords.as_array ().sub_array (start_offset, parts_count),
			    hb_array (parts, *parts_count)))
	_.first.extract (_.second, mult, font);
    }

    אם (italics_correction)
      *italics_correction = italicsCorrection.get_x_value (font, هذا);

    החזר partRecords.len;
  }

  فراغ depend (hb_depend_data_builder_t *depend_data, unsigned source) ثابت
  {
    עבור (ثابت auto& _ : partRecords.iter ())
      _.depend (depend_data, source);
  }

  فراغ closure_glyphs (hb_set_t *variant_glyphs) ثابت
  {
    עבור (ثابت auto& _ : partRecords.iter ())
      _.closure_glyphs (variant_glyphs);
  }

  محمي:
  MathValueRecord
		italicsCorrection;
				/* Italics correction of this
				 * MathGlyphAssembly. Should not
				 * depend on the assembly size. */
  Array16Of<MathGlyphPartRecord>
		partRecords;	/* Array of part records, from
				 * left to right and bottom to
				 * top. */

  عام:
  DEFINE_SIZE_ARRAY (6, partRecords);
};

بنية MathGlyphConstruction
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->start_embed (*هذا);
    אם (unlikely (!c->serializer->extend_min (out))) return_trace (שקר);

    out->glyphAssembly.serialize_subset (c, glyphAssembly, هذا);

    אם (!c->serializer->check_assign (out->mathGlyphVariantRecord.len, mathGlyphVariantRecord.len, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (שקר);
    עבור (ثابت auto& record : mathGlyphVariantRecord.iter ())
      אם (!record.subset (c)) return_trace (שקר);

    return_trace (אמת);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    (هذا+glyphAssembly).collect_variation_indices (c);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  glyphAssembly.sanitize (c, هذا) &&
		  mathGlyphVariantRecord.sanitize (c));
  }

  ثابت MathGlyphAssembly &get_assembly () ثابت { החזר هذا+glyphAssembly; }

  unsigned صحيح get_variants (hb_direction_t direction,
			     hb_font_t *font,
			     unsigned صحيح start_offset,
			     unsigned صحيح *variants_count, /* IN/OUT */
			     hb_ot_math_glyph_variant_t *variants /* OUT */) ثابت
  {
    אם (variants_count && variants)
    {
      int64_t mult = font->dir_mult (direction);
      עבור (auto _ : hb_zip (mathGlyphVariantRecord.as_array ().sub_array (start_offset, variants_count),
			    hb_array (variants, *variants_count)))
	_.second = {_.first.variantGlyph, font->em_mult (_.first.advanceMeasurement, mult)};
    }
    החזר mathGlyphVariantRecord.len;
  }

  فراغ depend (hb_depend_data_builder_t *depend_data, unsigned source) ثابت
  {
    (هذا+glyphAssembly).depend (depend_data, source);

    עבור (ثابت auto& _ : mathGlyphVariantRecord.iter ())
      _.depend (depend_data, source);
  }

  فراغ closure_glyphs (hb_set_t *variant_glyphs) ثابت
  {
    (هذا+glyphAssembly).closure_glyphs (variant_glyphs);

    עבור (ثابت auto& _ : mathGlyphVariantRecord.iter ())
      _.closure_glyphs (variant_glyphs);
  }

  محمي:
  /* Offset to MathGlyphAssembly table for this shape - from the beginning of
     MathGlyphConstruction table.  May be NULL. */
  Offset16To<MathGlyphAssembly>	  glyphAssembly;

  /* MathGlyphVariantRecords for alternative variants of the glyphs. */
  Array16Of<MathGlyphVariantRecord> mathGlyphVariantRecord;

  عام:
  DEFINE_SIZE_ARRAY (4, mathGlyphVariantRecord);
};

بنية MathVariants
{
  فراغ depend (hb_depend_data_builder_t *depend_data) ثابت
  {
    ثابت hb_array_t<ثابت Offset16To<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vertGlyphCount + horizGlyphCount);

    אם (vertGlyphCoverage)
    {
      ثابت auto vert_offsets = glyph_construction_offsets.sub_array (0, vertGlyphCount);
      + hb_zip (هذا+vertGlyphCoverage, vert_offsets)
      | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, ثابت Offset16To<MathGlyphConstruction>&> &_)
                  {
                    ثابت MathGlyphConstruction &mgc = هذا+_.second;
                    mgc.depend (depend_data, _.first);
                  })
      ;
    }
    אם (horizGlyphCoverage)
    {
      ثابت auto hori_offsets = glyph_construction_offsets.sub_array (vertGlyphCount, horizGlyphCount);
      + hb_zip (هذا+horizGlyphCoverage, hori_offsets)
      | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, ثابت Offset16To<MathGlyphConstruction>&> &_)
                  {
                    ثابت MathGlyphConstruction &mgc = هذا+_.second;
                    mgc.depend (depend_data, _.first);
                  })
      ;
    }
  }

  فراغ closure_glyphs (ثابت hb_set_t *glyph_set,
                       hb_set_t *variant_glyphs) ثابت
  {
    ثابت hb_array_t<ثابت Offset16To<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vertGlyphCount + horizGlyphCount);

    אם (vertGlyphCoverage)
    {
      ثابت auto vert_offsets = glyph_construction_offsets.sub_array (0, vertGlyphCount);
      + hb_zip (هذا+vertGlyphCoverage, vert_offsets)
      | hb_filter (glyph_set, hb_first)
      | hb_map (hb_second)
      | hb_map (hb_add (هذا))
      | hb_apply ([=] (ثابت MathGlyphConstruction &_) { _.closure_glyphs (variant_glyphs); })
      ;
    }

    אם (horizGlyphCoverage)
    {
      ثابت auto hori_offsets = glyph_construction_offsets.sub_array (vertGlyphCount, horizGlyphCount);
      + hb_zip (هذا+horizGlyphCoverage, hori_offsets)
      | hb_filter (glyph_set, hb_first)
      | hb_map (hb_second)
      | hb_map (hb_add (هذا))
      | hb_apply ([=] (ثابت MathGlyphConstruction &_) { _.closure_glyphs (variant_glyphs); })
      ;
    }
  }

  فراغ collect_coverage_and_indices (hb_sorted_vector_t<hb_codepoint_t>& new_coverage,
                                     ثابت Offset16To<Coverage>& coverage,
                                     unsigned i,
                                     unsigned end_index,
                                     hb_set_t& indices,
                                     ثابت hb_set_t& glyphset,
                                     ثابت hb_map_t& glyph_map) ثابت
  {
    אם (!coverage) החזר;

    עבור (ثابت auto _ : (هذا+coverage).iter ())
    {
      אם (i >= end_index) החזר;
      אם (glyphset.has (_))
      {
        unsigned new_gid = glyph_map.get (_);
        new_coverage.push (new_gid);
        indices.add (i);
      }
      i++;
    }
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_set_t &glyphset = c->plan->_glyphset_cmaped;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*هذا);
    אם (unlikely (!c->serializer->extend_min (out))) return_trace (שקר);
    אם (!c->serializer->check_assign (out->minConnectorOverlap, minConnectorOverlap, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (שקר);

    hb_sorted_vector_t<hb_codepoint_t> new_vert_coverage;
    hb_sorted_vector_t<hb_codepoint_t> new_hori_coverage;
    hb_set_t indices;
    collect_coverage_and_indices (new_vert_coverage, vertGlyphCoverage, 0, vertGlyphCount, indices, glyphset, glyph_map);
    collect_coverage_and_indices (new_hori_coverage, horizGlyphCoverage, vertGlyphCount, vertGlyphCount + horizGlyphCount, indices, glyphset, glyph_map);

    אם (!c->serializer->check_assign (out->vertGlyphCount, new_vert_coverage.length, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (שקר);
    אם (!c->serializer->check_assign (out->horizGlyphCount, new_hori_coverage.length, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (שקר);

    עבור (unsigned i : indices.iter ())
    {
      auto *o = c->serializer->embed (glyphConstruction[i]);
      אם (!o) return_trace (שקר);
      o->serialize_subset (c, glyphConstruction[i], هذا);
    }

    אם (new_vert_coverage)
      out->vertGlyphCoverage.serialize_serialize (c->serializer, new_vert_coverage.iter ());

    אם (new_hori_coverage)
    out->horizGlyphCoverage.serialize_serialize (c->serializer, new_hori_coverage.iter ());
    return_trace (אמת);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    עבור (ثابت auto &entry : hb_zip (هذا+vertGlyphCoverage, glyphConstruction.as_array (vertGlyphCount)))
      אם (c->glyph_set->has (hb_first (entry)))
        (هذا+hb_second (entry)).collect_variation_indices (c);
    עבור (ثابت auto &entry : hb_zip (هذا+horizGlyphCoverage, glyphConstruction.as_array (vertGlyphCount + horizGlyphCount).sub_array (vertGlyphCount)))
      אם (c->glyph_set->has (hb_first (entry)))
        (هذا+hb_second (entry)).collect_variation_indices (c);
  }

  منطقي sanitize_offsets (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    unsigned صحيح count = vertGlyphCount + horizGlyphCount;
    עבור (unsigned صحيح i = 0; i < count; i++)
      אם (!glyphConstruction.arrayZ[i].sanitize (c, هذا)) return_trace (שקר);
    return_trace (אמת);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  vertGlyphCoverage.sanitize (c, هذا) &&
		  horizGlyphCoverage.sanitize (c, هذا) &&
		  hb_barrier () &&
		  c->check_array (glyphConstruction.arrayZ, vertGlyphCount + horizGlyphCount) &&
		  sanitize_offsets (c));
  }

  hb_position_t get_min_connector_overlap (hb_direction_t direction,
						  hb_font_t *font) ثابت
  { החזר font->em_scale_dir (minConnectorOverlap, direction); }

  unsigned صحيح get_glyph_variants (hb_codepoint_t glyph,
				   hb_direction_t direction,
				   hb_font_t *font,
				   unsigned صحيح start_offset,
				   unsigned صحيح *variants_count, /* IN/OUT */
				   hb_ot_math_glyph_variant_t *variants /* OUT */) ثابت
  { החזר get_glyph_construction (glyph, direction, font)
	   .get_variants (direction, font, start_offset, variants_count, variants); }

  unsigned صحيح get_glyph_parts (hb_codepoint_t glyph,
				hb_direction_t direction,
				hb_font_t *font,
				unsigned صحيح start_offset,
				unsigned صحيح *parts_count, /* IN/OUT */
				hb_ot_math_glyph_part_t *parts /* OUT */,
				hb_position_t *italics_correction /* OUT */) ثابت
  { החזר get_glyph_construction (glyph, direction, font)
	   .get_assembly ()
	   .get_parts (direction, font,
		       start_offset, parts_count, parts,
		       italics_correction); }

  خاص:
  ثابت MathGlyphConstruction &
  get_glyph_construction (hb_codepoint_t glyph,
			  hb_direction_t direction,
			  hb_font_t *font HB_UNUSED) ثابت
  {
    منطقي vertical = HB_DIRECTION_IS_VERTICAL (direction);
    unsigned صحيح count = vertical ? vertGlyphCount : horizGlyphCount;
    ثابت Offset16To<Coverage> &coverage = vertical ? vertGlyphCoverage
						  : horizGlyphCoverage;

    unsigned صحيح index = (هذا+coverage).get_coverage (glyph);
    אם (unlikely (index >= count)) החזר Null (MathGlyphConstruction);

    אם (!vertical)
      index += vertGlyphCount;

    החזר هذا+glyphConstruction[index];
  }

  محمي:
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
  HBUINT16	vertGlyphCount;	/* Number of glyphs for which
				 * information is provided for
				 * vertically growing variants. */
  HBUINT16	horizGlyphCount;/* Number of glyphs for which
				 * information is provided for
				 * horizontally growing variants. */

  /* Array of offsets to MathGlyphConstruction tables - from the beginning of
     the MathVariants table, for shapes growing in vertical/horizontal
     direction. */
  UnsizedArrayOf<Offset16To<MathGlyphConstruction>>
			glyphConstruction;

  عام:
  DEFINE_SIZE_ARRAY (10, glyphConstruction);
};


/*
 * MATH -- Mathematical typesetting
 * https://docs.microsoft.com/en-us/typography/opentype/spec/math
 */

بنية MATH
{
  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_MATH;

  منطقي has_data () ثابت { החזר version.to_int (); }

  فراغ depend (hb_depend_data_builder_t *depend_data) ثابت
  {
    אם (mathVariants)
      (هذا+mathVariants).depend (depend_data);
  }

  فراغ closure_glyphs (hb_set_t *glyph_set) ثابت
  {
    אם (mathVariants)
    {
      hb_set_t variant_glyphs;
      (هذا+mathVariants).closure_glyphs (glyph_set, &variant_glyphs);
      hb_set_union (glyph_set, &variant_glyphs);
    }
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (*هذا);
    אם (unlikely (!out)) return_trace (שקר);

    out->mathConstants.serialize_copy (c->serializer, mathConstants, هذا, 0, hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map);
    out->mathGlyphInfo.serialize_subset (c, mathGlyphInfo, هذا);
    out->mathVariants.serialize_subset (c, mathVariants, هذا);
    return_trace (אמת);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c, ثابت hb_set_t *variant_glyph_set) ثابت
  {
    (هذا+mathConstants).collect_variation_indices (c);
    (هذا+mathGlyphInfo).collect_variation_indices (c);
    auto *glyph_set = c->glyph_set;
    c->glyph_set = variant_glyph_set;
    (هذا+mathVariants).collect_variation_indices (c);
    c->glyph_set = glyph_set;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (version.sanitize (c) &&
		  likely (version.major == 1) &&
		  hb_barrier () &&
		  mathConstants.sanitize (c, هذا) &&
		  mathGlyphInfo.sanitize (c, هذا) &&
		  mathVariants.sanitize (c, هذا));
  }

  // https://github.com/harfbuzz/harfbuzz/issues/4653
  HB_INTERNAL منطقي is_bad_cambria (hb_font_t *font) ثابت
  {
#ifndef HB_NO_MATH
    بدل HB_CODEPOINT_ENCODE3 (font->face->table.MATH.get_blob ()->length,
                                 (هذا+mathConstants).minHeight[1], // displayOperatorMinHeight
                                 (هذا+mathConstants).minHeight[0]) // delimitedSubFormulaMinHeight
    {
      /* sha1sum:ab4a4fe054d23061f3c039493d6f665cfda2ecf5  cambria.ttc
       * sha1sum:086855301bff644f9d8827b88491fcf73a6d4cb9  cambria.ttc
       * sha1sum:b1e5a3feaca2ea3dfcf79ccb377de749ecf60343  cambria.ttc */
      حالة HB_CODEPOINT_ENCODE3 (25722, 2500, 3000):
        החזר אמת;
    }
#endif
    החזר שקר;
  }

  hb_position_t get_constant (hb_ot_math_constant_t  constant,
			      hb_font_t		   *font) ثابت
  { החזר (هذا+mathConstants).get_value (constant, font); }

  ثابت MathGlyphInfo &get_glyph_info () ثابت { החזר هذا+mathGlyphInfo; }

  ثابت MathVariants &get_variants () ثابت    { החזר هذا+mathVariants; }

  محمي:
  FixedVersion<>version;	/* Version of the MATH table
				 * initially set to 0x00010000u */
  Offset16To<MathConstants>
		mathConstants;	/* MathConstants table */
  Offset16To<MathGlyphInfo>
		mathGlyphInfo;	/* MathGlyphInfo table */
  Offset16To<MathVariants>
		mathVariants;	/* MathVariants table */

  عام:
  DEFINE_SIZE_STATIC (10);
};

} /* namespace OT */


#endif /* HB_OT_MATH_TABLE_HH */

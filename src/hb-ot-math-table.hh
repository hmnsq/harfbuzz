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


সংরচনা Math_মান_Record
{
  hb_position_t get_x_মান (hb_font_t *font, ধ্রুব শূন্য *base) ধ্রুব
  { প্রত্যাগম hb_saturate_add (font->em_scale_x (value), (base+deviceTable).get_x_delta (font, get_var_store (font, base))); }
  hb_position_t get_y_মান (hb_font_t *font, ধ্রুব শূন্য *base) ধ্রুব
  { প্রত্যাগম hb_saturate_add (font->em_scale_y (value), (base+deviceTable).get_y_delta (font, get_var_store (font, base))); }

  ধ্রুব ItemVariationStore &get_var_store (hb_font_t *font, ধ্রুব শূন্য *base) ধ্রুব
  {
#যদি !defined(HB_NO_VAR) && !defined(HB_NO_OT_LAYOUT)
    যদি ((base+deviceTable).is_variation_device ())
      প্রত্যাগম font->face->table.GDEF->table->get_var_store ();
#endif
    প্রত্যাগম Null (ItemVariationStore);
  }

  Math_মান_Record* copy (hb_serialize_context_t *c, ধ্রুব শূন্য *base,
                         ধ্রুব hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণাঙ্ক>> *variation_map) ধ্রুব
  {
    TRACE_SERIALIZE (এতত্);
    auto *out = c->embed (এতত্);
    যদি (unlikely (!out)) return_trace (nullptr);
    hb_pair_t<unsigned, পূর্ণাঙ্ক> *varidx_delta;
    যদি (variation_map->has ((base+deviceTable).get_variation_index (), &varidx_delta) &&
        !c->check_assign (out->value, (int64_t) value + hb_second (*varidx_delta),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (nullptr);
    out->deviceTable.serialize_copy (c, deviceTable, base, 0, hb_serialize_context_t::Head, variation_map);

    return_trace (out);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c, ধ্রুব শূন্য *base) ধ্রুব
  { (base+deviceTable).collect_variation_indices (c); }

  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুব শূন্য *base) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) && deviceTable.sanitize (c, base));
  }

  রক্ষিত:
  HBINT16		value;		/* The X or Y value in design units */
  Offset16To<Device>	deviceTable;	/* Offset to the device table - from the
					 * beginning of parent table.  May be NULL.
					 * Suggested format for device table is 1. */

  সার্বজনিক:
  DEFINE_SIZE_STATIC (4);
};

সংরচনা MathConstants
{
  friend সংরচনা MATH;

  MathConstants* copy (hb_serialize_context_t *c,
                      ধ্রুব hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণাঙ্ক>> *variation_map) ধ্রুব
  {
    TRACE_SERIALIZE (এতত্);
    auto *out = c->start_embed (এতত্);

    HBINT16 *p = c->allocate_size<HBINT16> (HBINT16::static_size * 2);
    যদি (unlikely (!p)) return_trace (nullptr);
    hb_memcpy (p, percentScaleDown, HBINT16::static_size * 2);

    HBUINT16 *m = c->allocate_size<HBUINT16> (HBUINT16::static_size * 2);
    যদি (unlikely (!m)) return_trace (nullptr);
    hb_memcpy (m, minHeight, HBUINT16::static_size * 2);

    unsigned গুনতি = ARRAY_LENGTH (math_মান_Records);
    প্রতি (unsigned i = 0; i < গুনতি; i++)
      যদি (!c->copy (math_মান_Records[i], এতত্, variation_map))
        return_trace (nullptr);

    যদি (!c->embed (radicalDegreeBottomRaisePercent)) return_trace (nullptr);
    return_trace (out);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুব
  {
    প্রতি (ধ্রুব auto &record : math_মান_Records)
      record.collect_variation_indices (c, এতত্);
  }

  তার্কিক sanitize_math_মান_records (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);

    unsigned পূর্ণাঙ্ক গুনতি = ARRAY_LENGTH (math_মান_Records);
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
      যদি (!math_মান_Records[i].sanitize (c, এতত্))
	return_trace (অসত্য);

    return_trace (সত্য);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) && sanitize_math_মান_records (c));
  }

  hb_position_t get_মান (hb_ot_math_constant_t constant,
			   hb_font_t *font) ধ্রুব
  {
    বিকল্প (constant) {

    পক্ষ HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_DOWN:
    পক্ষ HB_OT_MATH_CONSTANT_SCRIPT_SCRIPT_PERCENT_SCALE_DOWN:
      প্রত্যাগম percentScaleDown[constant - HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_DOWN];

    পক্ষ HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_HEIGHT:
    পক্ষ HB_OT_MATH_CONSTANT_DISPLAY_OPERATOR_MIN_HEIGHT:
      প্রত্যাগম font->em_scale_y (minHeight[constant - HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_HEIGHT]);

    পক্ষ HB_OT_MATH_CONSTANT_RADICAL_KERN_AFTER_DEGREE:
    পক্ষ HB_OT_MATH_CONSTANT_RADICAL_KERN_BEFORE_DEGREE:
    পক্ষ HB_OT_MATH_CONSTANT_SKEWED_FRACTION_HORIZONTAL_GAP:
    পক্ষ HB_OT_MATH_CONSTANT_SPACE_AFTER_SCRIPT:
      প্রত্যাগম math_মান_Records[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_x_মান (font, এতত্);

    পক্ষ HB_OT_MATH_CONSTANT_ACCENT_BASE_HEIGHT:
    পক্ষ HB_OT_MATH_CONSTANT_AXIS_HEIGHT:
    পক্ষ HB_OT_MATH_CONSTANT_FLATTENED_ACCENT_BASE_HEIGHT:
    পক্ষ HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_DISPLAY_STYLE_SHIFT_DOWN:
    পক্ষ HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_GAP_MIN:
    পক্ষ HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_SHIFT_DOWN:
    পক্ষ HB_OT_MATH_CONSTANT_FRACTION_DENOM_DISPLAY_STYLE_GAP_MIN:
    পক্ষ HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_DISPLAY_STYLE_SHIFT_UP:
    পক্ষ HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_GAP_MIN:
    পক্ষ HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_SHIFT_UP:
    পক্ষ HB_OT_MATH_CONSTANT_FRACTION_অঙ্ক_DISPLAY_STYLE_GAP_MIN:
    পক্ষ HB_OT_MATH_CONSTANT_FRACTION_RULE_THICKNESS:
    পক্ষ HB_OT_MATH_CONSTANT_LOWER_LIMIT_BASELINE_DROP_MIN:
    পক্ষ HB_OT_MATH_CONSTANT_LOWER_LIMIT_GAP_MIN:
    পক্ষ HB_OT_MATH_CONSTANT_MATH_LEADING:
    পক্ষ HB_OT_MATH_CONSTANT_OVERBAR_EXTRA_ASCENDER:
    পক্ষ HB_OT_MATH_CONSTANT_OVERBAR_RULE_THICKNESS:
    পক্ষ HB_OT_MATH_CONSTANT_OVERBAR_VERTICAL_GAP:
    পক্ষ HB_OT_MATH_CONSTANT_RADICAL_DISPLAY_STYLE_VERTICAL_GAP:
    পক্ষ HB_OT_MATH_CONSTANT_RADICAL_EXTRA_ASCENDER:
    পক্ষ HB_OT_MATH_CONSTANT_RADICAL_RULE_THICKNESS:
    পক্ষ HB_OT_MATH_CONSTANT_RADICAL_VERTICAL_GAP:
    পক্ষ HB_OT_MATH_CONSTANT_SKEWED_FRACTION_VERTICAL_GAP:
    পক্ষ HB_OT_MATH_CONSTANT_স্তূপ_BOTTOM_DISPLAY_STYLE_SHIFT_DOWN:
    পক্ষ HB_OT_MATH_CONSTANT_স্তূপ_BOTTOM_SHIFT_DOWN:
    পক্ষ HB_OT_MATH_CONSTANT_স্তূপ_DISPLAY_STYLE_GAP_MIN:
    পক্ষ HB_OT_MATH_CONSTANT_স্তূপ_GAP_MIN:
    পক্ষ HB_OT_MATH_CONSTANT_স্তূপ_শীর্ষ_DISPLAY_STYLE_SHIFT_UP:
    পক্ষ HB_OT_MATH_CONSTANT_স্তূপ_শীর্ষ_SHIFT_UP:
    পক্ষ HB_OT_MATH_CONSTANT_STRETCH_স্তূপ_BOTTOM_SHIFT_DOWN:
    পক্ষ HB_OT_MATH_CONSTANT_STRETCH_স্তূপ_GAP_ABOVE_MIN:
    পক্ষ HB_OT_MATH_CONSTANT_STRETCH_স্তূপ_GAP_BELOW_MIN:
    পক্ষ HB_OT_MATH_CONSTANT_STRETCH_স্তূপ_শীর্ষ_SHIFT_UP:
    পক্ষ HB_OT_MATH_CONSTANT_SUBSCRIPT_BASELINE_DROP_MIN:
    পক্ষ HB_OT_MATH_CONSTANT_SUBSCRIPT_SHIFT_DOWN:
    পক্ষ HB_OT_MATH_CONSTANT_SUBSCRIPT_শীর্ষ_MAX:
    পক্ষ HB_OT_MATH_CONSTANT_SUB_SUPERSCRIPT_GAP_MIN:
    পক্ষ HB_OT_MATH_CONSTANT_SUPERSCRIPT_BASELINE_DROP_MAX:
    পক্ষ HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MAX_WITH_SUBSCRIPT:
    পক্ষ HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MIN:
    পক্ষ HB_OT_MATH_CONSTANT_SUPERSCRIPT_SHIFT_UP:
    পক্ষ HB_OT_MATH_CONSTANT_SUPERSCRIPT_SHIFT_UP_CRAMPED:
    পক্ষ HB_OT_MATH_CONSTANT_UNDERBAR_EXTRA_DESCENDER:
    পক্ষ HB_OT_MATH_CONSTANT_UNDERBAR_RULE_THICKNESS:
    পক্ষ HB_OT_MATH_CONSTANT_UNDERBAR_VERTICAL_GAP:
    পক্ষ HB_OT_MATH_CONSTANT_UPPER_LIMIT_BASELINE_RISE_MIN:
    পক্ষ HB_OT_MATH_CONSTANT_UPPER_LIMIT_GAP_MIN:
      প্রত্যাগম math_মান_Records[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_y_মান (font, এতত্);

    পক্ষ HB_OT_MATH_CONSTANT_RADICAL_DEGREE_BOTTOM_RAISE_PERCENT:
      প্রত্যাগম radicalDegreeBottomRaisePercent;

    স্বভাব:
      প্রত্যাগম 0;
    }
  }

  রক্ষিত:
  HBINT16 percentScaleDown[2];
  HBUINT16 minHeight[2];
  Math_মান_Record math_মান_Records[51];
  HBINT16 radicalDegreeBottomRaisePercent;

  সার্বজনিক:
  DEFINE_SIZE_STATIC (214);
};

সংরচনা MathItalicsCorrectionInfo
{
  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    ধ্রুব hb_set_t &glyphset = c->plan->_glyphset_mathed;
    ধ্রুব hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (অসত্য);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (এতত্+coverage, italicsCorrection)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->italicsCorrection, এতত্, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (সত্য);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুব
  {
    প্রতি (ধ্রুব auto &entry : hb_zip (এতত্+coverage, italicsCorrection))
      যদি (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, এতত্);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  coverage.sanitize (c, এতত্) &&
		  italicsCorrection.sanitize (c, এতত্));
  }

  hb_position_t get_মান (hb_codepoint_t glyph,
			   hb_font_t *font) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক index = (এতত্+coverage).get_coverage (glyph);
    প্রত্যাগম italicsCorrection[index].get_x_মান (font, এতত্);
  }

  রক্ষিত:
  Offset16To<Coverage>       coverage;		/* Offset to Coverage table -
						 * from the beginning of
						 * MathItalicsCorrectionInfo
						 * table. */
  Array16Of<Math_মান_Record> italicsCorrection;	/* Array of MathValueRecords
						 * defining italics correction
						 * values for each
						 * covered glyph. */

  সার্বজনিক:
  DEFINE_SIZE_ARRAY (4, italicsCorrection);
};

সংরচনা Math_শীর্ষ_Accent_Attachment
{
  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    ধ্রুব hb_set_t &glyphset = c->plan->_glyphset_mathed;
    ধ্রুব hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (অসত্য);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (এতত্+শীর্ষ_Accent_Coverage, শীর্ষ_Accent_Attachment)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->শীর্ষ_Accent_Attachment, এতত্, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->শীর্ষ_Accent_Coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (সত্য);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুব
  {
    প্রতি (ধ্রুব auto &entry : hb_zip (এতত্+শীর্ষ_Accent_Coverage, শীর্ষ_Accent_Attachment))
      যদি (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, এতত্);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  শীর্ষ_Accent_Coverage.sanitize (c, এতত্) &&
		  শীর্ষ_Accent_Attachment.sanitize (c, এতত্));
  }

  hb_position_t get_মান (hb_codepoint_t glyph,
			   hb_font_t *font) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক index = (এতত্+শীর্ষ_Accent_Coverage).get_coverage (glyph);
    যদি (index == NOT_COVERED)
      প্রত্যাগম font->get_glyph_h_advance (glyph) / 2;
    প্রত্যাগম শীর্ষ_Accent_Attachment[index].get_x_মান (font, এতত্);
  }

  রক্ষিত:
  Offset16To<Coverage>       শীর্ষ_Accent_Coverage;   /* Offset to Coverage table -
						 * from the beginning of
						 * MathTopAccentAttachment
						 * table. */
  Array16Of<Math_মান_Record> শীর্ষ_Accent_Attachment; /* Array of MathValueRecords
						 * defining top accent
						 * attachment points for each
						 * covered glyph. */

  সার্বজনিক:
  DEFINE_SIZE_ARRAY (2 + 2, শীর্ষ_Accent_Attachment);
};

সংরচনা MathKern
{
  MathKern* copy (hb_serialize_context_t *c,
                      ধ্রুব hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণাঙ্ক>> *variation_map) ধ্রুব
  {
    TRACE_SERIALIZE (এতত্);
    auto *out = c->start_embed (এতত্);

    যদি (unlikely (!c->embed (height_গুনতি))) return_trace (nullptr);

    unsigned গুনতি = 2 * height_গুনতি + 1;
    প্রতি (unsigned i = 0; i < গুনতি; i++)
      যদি (!c->copy (math_মান_Records_Z.arrayZ[i], এতত্, variation_map))
        return_trace (nullptr);

    return_trace (out);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুব
  {
    প্রতি (ধ্রুব auto &record : math_মান_Records_Z.as_array (2 * height_গুনতি + 1))
      record.collect_variation_indices (c, এতত্);
  }

  তার্কিক sanitize_math_মান_records (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    unsigned পূর্ণাঙ্ক গুনতি = 2 * height_গুনতি + 1;
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
      যদি (!math_মান_Records_Z.arrayZ[i].sanitize (c, এতত্)) return_trace (অসত্য);
    return_trace (সত্য);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  hb_barrier () &&
		  c->check_array (math_মান_Records_Z.arrayZ, 2 * height_গুনতি + 1) &&
		  sanitize_math_মান_records (c));
  }

  hb_position_t get_মান (hb_position_t correction_height, hb_font_t *font) ধ্রুব
  {
    ধ্রুব Math_মান_Record* correctionHeight = math_মান_Records_Z.arrayZ;
    ধ্রুব Math_মান_Record* kern_মান = math_মান_Records_Z.arrayZ + height_গুনতি;
    পূর্ণাঙ্ক sign = font->y_scale < 0 ? -1 : +1;

    /* According to OpenType spec (v1.9), except for the boundary cases, the index
     * chosen for kern value should be i such that
     *    correctionHeight[i-1] <= correction_height < correctionHeight[i]
     * We can use the binary search algorithm of std::upper_bound(). Or, we can
     * use the internal hb_bsearch_impl.
     */
    unsigned পূর্ণাঙ্ক pos;
    auto cmp = +[](ধ্রুব শূন্য* key, ধ্রুব শূন্য* p,
                   পূর্ণাঙ্ক sign, hb_font_t* font, ধ্রুব MathKern* mathKern) -> পূর্ণাঙ্ক {
      hb_position_t a = *(hb_position_t *) key;
      hb_position_t b = ((Math_মান_Record *) p)->get_y_মান (font, mathKern);
      যদি (a == b) প্রত্যাগম 0;
      প্রত্যাগম sign > 0 ? (a < b ? -1 : +1) : (b < a ? -1 : +1);
    };
    unsigned পূর্ণাঙ্ক i = hb_bsearch_impl(&pos, correction_height, correctionHeight,
                                     height_গুনতি, Math_মান_Record::static_size,
                                     cmp, sign, font, এতত্) ? pos + 1 : pos;
    প্রত্যাগম kern_মান[i].get_x_মান (font, এতত্);
  }

  unsigned পূর্ণাঙ্ক get_entries (unsigned পূর্ণাঙ্ক start_offset,
			    unsigned পূর্ণাঙ্ক *entries_গুনতি, /* IN/OUT */
			    hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			    hb_font_t *font) ধ্রুব
  {
    ধ্রুব Math_মান_Record* correctionHeight = math_মান_Records_Z.arrayZ;
    ধ্রুব Math_মান_Record* kern_মান = math_মান_Records_Z.arrayZ + height_গুনতি;
    ধ্রুব unsigned পূর্ণাঙ্ক entries_গুনতি_2 = height_গুনতি + 1;

    যদি (entries_গুনতি && kern_entries)
    {
      unsigned পূর্ণাঙ্ক start = hb_min (start_offset, entries_গুনতি_2);
      unsigned পূর্ণাঙ্ক end = hb_min (start + *entries_গুনতি, entries_গুনতি_2);
      *entries_গুনতি = end - start;

      প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < *entries_গুনতি; i++) {
	unsigned পূর্ণাঙ্ক j = start + i;

	hb_position_t max_height;
	যদি (j == height_গুনতি) {
	  max_height = INT32_MAX;
	} অন্যথা {
	  max_height = correctionHeight[j].get_y_মান (font, এতত্);
	}

	kern_entries[i] = {max_height, kern_মান[j].get_x_মান (font, এতত্)};
      }
    }
    প্রত্যাগম entries_গুনতি_2;
  }

  রক্ষিত:
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

  সার্বজনিক:
  DEFINE_SIZE_ARRAY (2, math_মান_Records_Z);
};

সংরচনা MathKernInfoRecord
{
  MathKernInfoRecord* copy (hb_serialize_context_t *c, ধ্রুব শূন্য *base,
                            ধ্রুব hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণাঙ্ক>> *variation_map) ধ্রুব
  {
    TRACE_SERIALIZE (এতত্);
    auto *out = c->embed (এতত্);
    যদি (unlikely (!out)) return_trace (nullptr);

    unsigned গুনতি = ARRAY_LENGTH (mathKern);
    প্রতি (unsigned i = 0; i < গুনতি; i++)
      out->mathKern[i].serialize_copy (c, mathKern[i], base, 0, hb_serialize_context_t::Head, variation_map);

    return_trace (out);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c, ধ্রুব শূন্য *base) ধ্রুব
  {
    প্রতি (ধ্রুব auto &offset : mathKern)
      (base+offset).collect_variation_indices (c);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুব শূন্য *base) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);

    unsigned পূর্ণাঙ্ক গুনতি = ARRAY_LENGTH (mathKern);
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
      যদি (unlikely (!mathKern[i].sanitize (c, base)))
	return_trace (অসত্য);

    return_trace (সত্য);
  }

  hb_position_t get_kerning (hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font,
			     ধ্রুব শূন্য *base) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক idx = kern;
    যদি (unlikely (idx >= ARRAY_LENGTH (mathKern))) প্রত্যাগম 0;
    প্রত্যাগম (base+mathKern[idx]).get_মান (correction_height, font);
  }

  unsigned পূর্ণাঙ্ক get_kernings (hb_ot_math_kern_t kern,
			     unsigned পূর্ণাঙ্ক start_offset,
			     unsigned পূর্ণাঙ্ক *entries_গুনতি, /* IN/OUT */
			     hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			     hb_font_t *font,
			     ধ্রুব শূন্য *base) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক idx = kern;
    যদি (unlikely (idx >= ARRAY_LENGTH (mathKern)) || !mathKern[idx]) {
      যদি (entries_গুনতি) *entries_গুনতি = 0;
      প্রত্যাগম 0;
    }
    প্রত্যাগম (base+mathKern[idx]).get_entries (start_offset,
					     entries_গুনতি,
					     kern_entries,
					     font);
  }

  রক্ষিত:
  /* Offset to MathKern table for each corner -
   * from the beginning of MathKernInfo table.  May be NULL. */
  Offset16To<MathKern> mathKern[4];

  সার্বজনিক:
  DEFINE_SIZE_STATIC (8);
};

সংরচনা MathKernInfo
{
  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    ধ্রুব hb_set_t &glyphset = c->plan->_glyphset_mathed;
    ধ্রুব hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (অসত্য);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (এতত্+mathKernCoverage, mathKernInfoRecords)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->mathKernInfoRecords, এতত্, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->mathKernCoverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (সত্য);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুব
  {
    প্রতি (ধ্রুব auto &entry : hb_zip (এতত্+mathKernCoverage, mathKernInfoRecords))
      যদি (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, এতত্);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  mathKernCoverage.sanitize (c, এতত্) &&
		  mathKernInfoRecords.sanitize (c, এতত্));
  }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক index = (এতত্+mathKernCoverage).get_coverage (glyph);
    প্রত্যাগম mathKernInfoRecords[index].get_kerning (kern, correction_height, font, এতত্);
  }

  unsigned পূর্ণাঙ্ক get_kernings (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     unsigned পূর্ণাঙ্ক start_offset,
			     unsigned পূর্ণাঙ্ক *entries_গুনতি, /* IN/OUT */
			     hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			     hb_font_t *font) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক index = (এতত্+mathKernCoverage).get_coverage (glyph);
    প্রত্যাগম mathKernInfoRecords[index].get_kernings (kern,
						    start_offset,
						    entries_গুনতি,
						    kern_entries,
						    font,
						    এতত্);
  }

  রক্ষিত:
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

  সার্বজনিক:
  DEFINE_SIZE_ARRAY (4, mathKernInfoRecords);
};

সংরচনা MathGlyphInfo
{
  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (*এতত্);
    যদি (unlikely (!out)) return_trace (অসত্য);

    out->mathItalicsCorrectionInfo.serialize_subset (c, mathItalicsCorrectionInfo, এতত্);
    out->math_শীর্ষ_Accent_Attachment.serialize_subset (c, math_শীর্ষ_Accent_Attachment, এতত্);

    ধ্রুব hb_set_t &glyphset = c->plan->_glyphset_mathed;
    ধ্রুব hb_map_t &glyph_map = *c->plan->glyph_map;

    auto it =
    + hb_iter (এতত্+extendedShapeCoverage)
    | hb_take (c->plan->source->get_অঙ্ক_glyphs ())
    | hb_filter (glyphset)
    | hb_map_retains_sorting (glyph_map)
    ;

    যদি (it) out->extendedShapeCoverage.serialize_serialize (c->serializer, it);
    অন্যথা out->extendedShapeCoverage = 0;

    out->mathKernInfo.serialize_subset (c, mathKernInfo, এতত্);
    return_trace (সত্য);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুব
  {
    (এতত্+mathItalicsCorrectionInfo).collect_variation_indices (c);
    (এতত্+math_শীর্ষ_Accent_Attachment).collect_variation_indices (c);
    (এতত্+mathKernInfo).collect_variation_indices (c);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  mathItalicsCorrectionInfo.sanitize (c, এতত্) &&
		  math_শীর্ষ_Accent_Attachment.sanitize (c, এতত্) &&
		  extendedShapeCoverage.sanitize (c, এতত্) &&
		  mathKernInfo.sanitize (c, এতত্));
  }

  hb_position_t
  get_italics_correction (hb_codepoint_t  glyph, hb_font_t *font) ধ্রুব
  { প্রত্যাগম (এতত্+mathItalicsCorrectionInfo).get_মান (glyph, font); }

  hb_position_t
  get_শীর্ষ_accent_attachment (hb_codepoint_t  glyph, hb_font_t *font) ধ্রুব
  { প্রত্যাগম (এতত্+math_শীর্ষ_Accent_Attachment).get_মান (glyph, font); }

  তার্কিক is_extended_shape (hb_codepoint_t glyph) ধ্রুব
  { প্রত্যাগম (এতত্+extendedShapeCoverage).get_coverage (glyph) != NOT_COVERED; }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font) ধ্রুব
  { প্রত্যাগম (এতত্+mathKernInfo).get_kerning (glyph, kern, correction_height, font); }

  hb_position_t get_kernings (hb_codepoint_t glyph,
			      hb_ot_math_kern_t kern,
			      unsigned পূর্ণাঙ্ক start_offset,
			      unsigned পূর্ণাঙ্ক *entries_গুনতি, /* IN/OUT */
			      hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			      hb_font_t *font) ধ্রুব
  { প্রত্যাগম (এতত্+mathKernInfo).get_kernings (glyph,
					     kern,
					     start_offset,
					     entries_গুনতি,
					     kern_entries,
					     font); }

  রক্ষিত:
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

  সার্বজনিক:
  DEFINE_SIZE_STATIC (8);
};

সংরচনা MathGlyphVariantRecord
{
  friend সংরচনা MathGlyphConstruction;

  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (এতত্);
    যদি (unlikely (!out)) return_trace (অসত্য);

    ধ্রুব hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_assign (out->variantGlyph, glyph_map.get (variantGlyph), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  শূন্য depend (hb_depend_data_builder_t *depend_data, unsigned source) ধ্রুব
  { depend_data->add_depend(source, HB_OT_TAG_MATH, variantGlyph); }

  শূন্য closure_glyphs (hb_set_t *variant_glyphs) ধ্রুব
  { variant_glyphs->add (variantGlyph); }

  রক্ষিত:
  HBGlyphID16 variantGlyph;       /* Glyph ID for the variant. */
  HBUINT16  advanceMeasurement; /* Advance width/height, in design units, of the
				 * variant, in the direction of requested
				 * glyph extension. */

  সার্বজনিক:
  DEFINE_SIZE_STATIC (4);
};

সংরচনা PartFlags : HBUINT16
{
  গণনা Flags {
    Extender	= 0x0001u, /* If set, the part can be skipped or repeated. */

    Defined	= 0x0001u, /* All defined flags. */
  };

  সার্বজনিক:
  DEFINE_SIZE_STATIC (2);
};

সংরচনা MathGlyphPartRecord
{
  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (এতত্);
    যদি (unlikely (!out)) return_trace (অসত্য);

    ধ্রুব hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_assign (out->glyph, glyph_map.get (glyph), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  শূন্য extract (hb_ot_math_glyph_part_t &out,
		int64_t mult,
		hb_font_t *font) ধ্রুব
  {
    out.glyph			= glyph;

    out.start_connector_length	= font->em_mult (startConnectorLength, mult);
    out.end_connector_length	= font->em_mult (endConnectorLength, mult);
    out.full_advance		= font->em_mult (fullAdvance, mult);

    static_assert ((unsigned পূর্ণাঙ্ক) HB_OT_MATH_GLYPH_PART_FLAG_EXTENDER ==
		   (unsigned পূর্ণাঙ্ক) PartFlags::Extender, "");

    out.flags = (hb_ot_math_glyph_part_flags_t)
		(unsigned পূর্ণাঙ্ক)
		(partFlags & PartFlags::Defined);
  }

  শূন্য depend (hb_depend_data_builder_t *depend_data, unsigned source) ধ্রুব
  { depend_data->add_depend(source, HB_OT_TAG_MATH, glyph); }

  শূন্য closure_glyphs (hb_set_t *variant_glyphs) ধ্রুব
  { variant_glyphs->add (glyph); }

  রক্ষিত:
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

  সার্বজনিক:
  DEFINE_SIZE_STATIC (10);
};

সংরচনা MathGlyphAssembly
{
  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);

    যদি (!c->serializer->copy (italicsCorrection, এতত্, &c->plan->layout_variation_idx_delta_map)) return_trace (অসত্য);
    যদি (!c->serializer->copy<HBUINT16> (partRecords.len)) return_trace (অসত্য);

    প্রতি (ধ্রুব auto& record : partRecords.iter ())
      যদি (!record.subset (c)) return_trace (অসত্য);
    return_trace (সত্য);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুব
  {
    italicsCorrection.collect_variation_indices (c, এতত্);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  italicsCorrection.sanitize (c, এতত্) &&
		  partRecords.sanitize (c));
  }

  unsigned পূর্ণাঙ্ক get_parts (hb_direction_t direction,
			  hb_font_t *font,
			  unsigned পূর্ণাঙ্ক start_offset,
			  unsigned পূর্ণাঙ্ক *parts_গুনতি, /* IN/OUT */
			  hb_ot_math_glyph_part_t *parts /* OUT */,
			  hb_position_t *italics_correction /* OUT */) ধ্রুব
  {
    যদি (parts_গুনতি && parts)
    {
      int64_t mult = font->dir_mult (direction);
      প্রতি (auto _ : hb_zip (partRecords.as_array ().sub_array (start_offset, parts_গুনতি),
			    hb_array (parts, *parts_গুনতি)))
	_.first.extract (_.second, mult, font);
    }

    যদি (italics_correction)
      *italics_correction = italicsCorrection.get_x_মান (font, এতত্);

    প্রত্যাগম partRecords.len;
  }

  শূন্য depend (hb_depend_data_builder_t *depend_data, unsigned source) ধ্রুব
  {
    প্রতি (ধ্রুব auto& _ : partRecords.iter ())
      _.depend (depend_data, source);
  }

  শূন্য closure_glyphs (hb_set_t *variant_glyphs) ধ্রুব
  {
    প্রতি (ধ্রুব auto& _ : partRecords.iter ())
      _.closure_glyphs (variant_glyphs);
  }

  রক্ষিত:
  Math_মান_Record
		italicsCorrection;
				/* Italics correction of this
				 * MathGlyphAssembly. Should not
				 * depend on the assembly size. */
  Array16Of<MathGlyphPartRecord>
		partRecords;	/* Array of part records, from
				 * left to right and bottom to
				 * top. */

  সার্বজনিক:
  DEFINE_SIZE_ARRAY (6, partRecords);
};

সংরচনা MathGlyphConstruction
{
  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (অসত্য);

    out->glyphAssembly.serialize_subset (c, glyphAssembly, এতত্);

    যদি (!c->serializer->check_assign (out->mathGlyphVariantRecord.len, mathGlyphVariantRecord.len, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (অসত্য);
    প্রতি (ধ্রুব auto& record : mathGlyphVariantRecord.iter ())
      যদি (!record.subset (c)) return_trace (অসত্য);

    return_trace (সত্য);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুব
  {
    (এতত্+glyphAssembly).collect_variation_indices (c);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  glyphAssembly.sanitize (c, এতত্) &&
		  mathGlyphVariantRecord.sanitize (c));
  }

  ধ্রুব MathGlyphAssembly &get_assembly () ধ্রুব { প্রত্যাগম এতত্+glyphAssembly; }

  unsigned পূর্ণাঙ্ক get_variants (hb_direction_t direction,
			     hb_font_t *font,
			     unsigned পূর্ণাঙ্ক start_offset,
			     unsigned পূর্ণাঙ্ক *variants_গুনতি, /* IN/OUT */
			     hb_ot_math_glyph_variant_t *variants /* OUT */) ধ্রুব
  {
    যদি (variants_গুনতি && variants)
    {
      int64_t mult = font->dir_mult (direction);
      প্রতি (auto _ : hb_zip (mathGlyphVariantRecord.as_array ().sub_array (start_offset, variants_গুনতি),
			    hb_array (variants, *variants_গুনতি)))
	_.second = {_.first.variantGlyph, font->em_mult (_.first.advanceMeasurement, mult)};
    }
    প্রত্যাগম mathGlyphVariantRecord.len;
  }

  শূন্য depend (hb_depend_data_builder_t *depend_data, unsigned source) ধ্রুব
  {
    (এতত্+glyphAssembly).depend (depend_data, source);

    প্রতি (ধ্রুব auto& _ : mathGlyphVariantRecord.iter ())
      _.depend (depend_data, source);
  }

  শূন্য closure_glyphs (hb_set_t *variant_glyphs) ধ্রুব
  {
    (এতত্+glyphAssembly).closure_glyphs (variant_glyphs);

    প্রতি (ধ্রুব auto& _ : mathGlyphVariantRecord.iter ())
      _.closure_glyphs (variant_glyphs);
  }

  রক্ষিত:
  /* Offset to MathGlyphAssembly table for this shape - from the beginning of
     MathGlyphConstruction table.  May be NULL. */
  Offset16To<MathGlyphAssembly>	  glyphAssembly;

  /* MathGlyphVariantRecords for alternative variants of the glyphs. */
  Array16Of<MathGlyphVariantRecord> mathGlyphVariantRecord;

  সার্বজনিক:
  DEFINE_SIZE_ARRAY (4, mathGlyphVariantRecord);
};

সংরচনা MathVariants
{
  শূন্য depend (hb_depend_data_builder_t *depend_data) ধ্রুব
  {
    ধ্রুব hb_array_t<ধ্রুব Offset16To<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vert_Glyph_গুনতি + horiz_Glyph_গুনতি);

    যদি (vertGlyphCoverage)
    {
      ধ্রুব auto vert_offsets = glyph_construction_offsets.sub_array (0, vert_Glyph_গুনতি);
      + hb_zip (এতত্+vertGlyphCoverage, vert_offsets)
      | hb_apply ([&] (ধ্রুব hb_pair_t<hb_codepoint_t, ধ্রুব Offset16To<MathGlyphConstruction>&> &_)
                  {
                    ধ্রুব MathGlyphConstruction &mgc = এতত্+_.second;
                    mgc.depend (depend_data, _.first);
                  })
      ;
    }
    যদি (horizGlyphCoverage)
    {
      ধ্রুব auto hori_offsets = glyph_construction_offsets.sub_array (vert_Glyph_গুনতি, horiz_Glyph_গুনতি);
      + hb_zip (এতত্+horizGlyphCoverage, hori_offsets)
      | hb_apply ([&] (ধ্রুব hb_pair_t<hb_codepoint_t, ধ্রুব Offset16To<MathGlyphConstruction>&> &_)
                  {
                    ধ্রুব MathGlyphConstruction &mgc = এতত্+_.second;
                    mgc.depend (depend_data, _.first);
                  })
      ;
    }
  }

  শূন্য closure_glyphs (ধ্রুব hb_set_t *glyph_set,
                       hb_set_t *variant_glyphs) ধ্রুব
  {
    ধ্রুব hb_array_t<ধ্রুব Offset16To<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vert_Glyph_গুনতি + horiz_Glyph_গুনতি);

    যদি (vertGlyphCoverage)
    {
      ধ্রুব auto vert_offsets = glyph_construction_offsets.sub_array (0, vert_Glyph_গুনতি);
      + hb_zip (এতত্+vertGlyphCoverage, vert_offsets)
      | hb_filter (glyph_set, hb_first)
      | hb_map (hb_second)
      | hb_map (hb_add (এতত্))
      | hb_apply ([=] (ধ্রুব MathGlyphConstruction &_) { _.closure_glyphs (variant_glyphs); })
      ;
    }

    যদি (horizGlyphCoverage)
    {
      ধ্রুব auto hori_offsets = glyph_construction_offsets.sub_array (vert_Glyph_গুনতি, horiz_Glyph_গুনতি);
      + hb_zip (এতত্+horizGlyphCoverage, hori_offsets)
      | hb_filter (glyph_set, hb_first)
      | hb_map (hb_second)
      | hb_map (hb_add (এতত্))
      | hb_apply ([=] (ধ্রুব MathGlyphConstruction &_) { _.closure_glyphs (variant_glyphs); })
      ;
    }
  }

  শূন্য collect_coverage_and_indices (hb_sorted_vector_t<hb_codepoint_t>& new_coverage,
                                     ধ্রুব Offset16To<Coverage>& coverage,
                                     unsigned i,
                                     unsigned end_index,
                                     hb_set_t& indices,
                                     ধ্রুব hb_set_t& glyphset,
                                     ধ্রুব hb_map_t& glyph_map) ধ্রুব
  {
    যদি (!coverage) প্রত্যাগম;

    প্রতি (ধ্রুব auto _ : (এতত্+coverage).iter ())
    {
      যদি (i >= end_index) প্রত্যাগম;
      যদি (glyphset.has (_))
      {
        unsigned new_gid = glyph_map.get (_);
        new_coverage.push (new_gid);
        indices.add (i);
      }
      i++;
    }
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    ধ্রুব hb_set_t &glyphset = c->plan->_glyphset_cmaped;
    ধ্রুব hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (অসত্য);
    যদি (!c->serializer->check_assign (out->minConnectorOverlap, minConnectorOverlap, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (অসত্য);

    hb_sorted_vector_t<hb_codepoint_t> new_vert_coverage;
    hb_sorted_vector_t<hb_codepoint_t> new_hori_coverage;
    hb_set_t indices;
    collect_coverage_and_indices (new_vert_coverage, vertGlyphCoverage, 0, vert_Glyph_গুনতি, indices, glyphset, glyph_map);
    collect_coverage_and_indices (new_hori_coverage, horizGlyphCoverage, vert_Glyph_গুনতি, vert_Glyph_গুনতি + horiz_Glyph_গুনতি, indices, glyphset, glyph_map);

    যদি (!c->serializer->check_assign (out->vert_Glyph_গুনতি, new_vert_coverage.length, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (অসত্য);
    যদি (!c->serializer->check_assign (out->horiz_Glyph_গুনতি, new_hori_coverage.length, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (অসত্য);

    প্রতি (unsigned i : indices.iter ())
    {
      auto *o = c->serializer->embed (glyphConstruction[i]);
      যদি (!o) return_trace (অসত্য);
      o->serialize_subset (c, glyphConstruction[i], এতত্);
    }

    যদি (new_vert_coverage)
      out->vertGlyphCoverage.serialize_serialize (c->serializer, new_vert_coverage.iter ());

    যদি (new_hori_coverage)
    out->horizGlyphCoverage.serialize_serialize (c->serializer, new_hori_coverage.iter ());
    return_trace (সত্য);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুব
  {
    প্রতি (ধ্রুব auto &entry : hb_zip (এতত্+vertGlyphCoverage, glyphConstruction.as_array (vert_Glyph_গুনতি)))
      যদি (c->glyph_set->has (hb_first (entry)))
        (এতত্+hb_second (entry)).collect_variation_indices (c);
    প্রতি (ধ্রুব auto &entry : hb_zip (এতত্+horizGlyphCoverage, glyphConstruction.as_array (vert_Glyph_গুনতি + horiz_Glyph_গুনতি).sub_array (vert_Glyph_গুনতি)))
      যদি (c->glyph_set->has (hb_first (entry)))
        (এতত্+hb_second (entry)).collect_variation_indices (c);
  }

  তার্কিক sanitize_offsets (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    unsigned পূর্ণাঙ্ক গুনতি = vert_Glyph_গুনতি + horiz_Glyph_গুনতি;
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
      যদি (!glyphConstruction.arrayZ[i].sanitize (c, এতত্)) return_trace (অসত্য);
    return_trace (সত্য);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  vertGlyphCoverage.sanitize (c, এতত্) &&
		  horizGlyphCoverage.sanitize (c, এতত্) &&
		  hb_barrier () &&
		  c->check_array (glyphConstruction.arrayZ, vert_Glyph_গুনতি + horiz_Glyph_গুনতি) &&
		  sanitize_offsets (c));
  }

  hb_position_t get_min_connector_overlap (hb_direction_t direction,
						  hb_font_t *font) ধ্রুব
  { প্রত্যাগম font->em_scale_dir (minConnectorOverlap, direction); }

  unsigned পূর্ণাঙ্ক get_glyph_variants (hb_codepoint_t glyph,
				   hb_direction_t direction,
				   hb_font_t *font,
				   unsigned পূর্ণাঙ্ক start_offset,
				   unsigned পূর্ণাঙ্ক *variants_গুনতি, /* IN/OUT */
				   hb_ot_math_glyph_variant_t *variants /* OUT */) ধ্রুব
  { প্রত্যাগম get_glyph_construction (glyph, direction, font)
	   .get_variants (direction, font, start_offset, variants_গুনতি, variants); }

  unsigned পূর্ণাঙ্ক get_glyph_parts (hb_codepoint_t glyph,
				hb_direction_t direction,
				hb_font_t *font,
				unsigned পূর্ণাঙ্ক start_offset,
				unsigned পূর্ণাঙ্ক *parts_গুনতি, /* IN/OUT */
				hb_ot_math_glyph_part_t *parts /* OUT */,
				hb_position_t *italics_correction /* OUT */) ধ্রুব
  { প্রত্যাগম get_glyph_construction (glyph, direction, font)
	   .get_assembly ()
	   .get_parts (direction, font,
		       start_offset, parts_গুনতি, parts,
		       italics_correction); }

  গুপ্ত:
  ধ্রুব MathGlyphConstruction &
  get_glyph_construction (hb_codepoint_t glyph,
			  hb_direction_t direction,
			  hb_font_t *font HB_UNUSED) ধ্রুব
  {
    তার্কিক vertical = HB_DIRECTION_IS_VERTICAL (direction);
    unsigned পূর্ণাঙ্ক গুনতি = vertical ? vert_Glyph_গুনতি : horiz_Glyph_গুনতি;
    ধ্রুব Offset16To<Coverage> &coverage = vertical ? vertGlyphCoverage
						  : horizGlyphCoverage;

    unsigned পূর্ণাঙ্ক index = (এতত্+coverage).get_coverage (glyph);
    যদি (unlikely (index >= গুনতি)) প্রত্যাগম Null (MathGlyphConstruction);

    যদি (!vertical)
      index += vert_Glyph_গুনতি;

    প্রত্যাগম এতত্+glyphConstruction[index];
  }

  রক্ষিত:
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

  সার্বজনিক:
  DEFINE_SIZE_ARRAY (10, glyphConstruction);
};


/*
 * MATH -- Mathematical typesetting
 * https://docs.microsoft.com/en-us/typography/opentype/spec/math
 */

সংরচনা MATH
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_MATH;

  তার্কিক has_data () ধ্রুব { প্রত্যাগম version.to_int (); }

  শূন্য depend (hb_depend_data_builder_t *depend_data) ধ্রুব
  {
    যদি (mathVariants)
      (এতত্+mathVariants).depend (depend_data);
  }

  শূন্য closure_glyphs (hb_set_t *glyph_set) ধ্রুব
  {
    যদি (mathVariants)
    {
      hb_set_t variant_glyphs;
      (এতত্+mathVariants).closure_glyphs (glyph_set, &variant_glyphs);
      hb_set_union (glyph_set, &variant_glyphs);
    }
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (*এতত্);
    যদি (unlikely (!out)) return_trace (অসত্য);

    out->mathConstants.serialize_copy (c->serializer, mathConstants, এতত্, 0, hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map);
    out->mathGlyphInfo.serialize_subset (c, mathGlyphInfo, এতত্);
    out->mathVariants.serialize_subset (c, mathVariants, এতত্);
    return_trace (সত্য);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c, ধ্রুব hb_set_t *variant_glyph_set) ধ্রুব
  {
    (এতত্+mathConstants).collect_variation_indices (c);
    (এতত্+mathGlyphInfo).collect_variation_indices (c);
    auto *glyph_set = c->glyph_set;
    c->glyph_set = variant_glyph_set;
    (এতত্+mathVariants).collect_variation_indices (c);
    c->glyph_set = glyph_set;
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (version.sanitize (c) &&
		  likely (version.major == 1) &&
		  hb_barrier () &&
		  mathConstants.sanitize (c, এতত্) &&
		  mathGlyphInfo.sanitize (c, এতত্) &&
		  mathVariants.sanitize (c, এতত্));
  }

  // https://github.com/harfbuzz/harfbuzz/issues/4653
  HB_INTERNAL তার্কিক is_bad_cambria (hb_font_t *font) ধ্রুব
  {
#ifndef HB_NO_MATH
    বিকল্প HB_CODEPOINT_ENCODE3 (font->face->table.MATH.get_blob ()->length,
                                 (এতত্+mathConstants).minHeight[1], // displayOperatorMinHeight
                                 (এতত্+mathConstants).minHeight[0]) // delimitedSubFormulaMinHeight
    {
      /* sha1sum:ab4a4fe054d23061f3c039493d6f665cfda2ecf5  cambria.ttc
       * sha1sum:086855301bff644f9d8827b88491fcf73a6d4cb9  cambria.ttc
       * sha1sum:b1e5a3feaca2ea3dfcf79ccb377de749ecf60343  cambria.ttc */
      পক্ষ HB_CODEPOINT_ENCODE3 (25722, 2500, 3000):
        প্রত্যাগম সত্য;
    }
#endif
    প্রত্যাগম অসত্য;
  }

  hb_position_t get_constant (hb_ot_math_constant_t  constant,
			      hb_font_t		   *font) ধ্রুব
  { প্রত্যাগম (এতত্+mathConstants).get_মান (constant, font); }

  ধ্রুব MathGlyphInfo &get_glyph_info () ধ্রুব { প্রত্যাগম এতত্+mathGlyphInfo; }

  ধ্রুব MathVariants &get_variants () ধ্রুব    { প্রত্যাগম এতত্+mathVariants; }

  রক্ষিত:
  FixedVersion<>version;	/* Version of the MATH table
				 * initially set to 0x00010000u */
  Offset16To<MathConstants>
		mathConstants;	/* MathConstants table */
  Offset16To<MathGlyphInfo>
		mathGlyphInfo;	/* MathGlyphInfo table */
  Offset16To<MathVariants>
		mathVariants;	/* MathVariants table */

  সার্বজনিক:
  DEFINE_SIZE_STATIC (10);
};

} /* namespace OT */


#endif /* HB_OT_MATH_TABLE_HH */

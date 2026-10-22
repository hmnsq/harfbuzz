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

#شامل "hb-open-type.hh"
#شامل "hb-ot-layout-common.hh"
#شامل "hb-ot-layout-gdef-table.hh"
#شامل "hb-ot-math.h"
#شامل "hb-depend-data.hh"

فضاینام OT {


ساختار Math_مقدار_Record
{
  hb_position_t get_x_مقدار (hb_font_t *font, ثابت تهی *base) ثابت
  { بازگشت hb_saturate_add (font->em_scale_x (value), (base+deviceTable).get_x_delta (font, get_var_store (font, base))); }
  hb_position_t get_y_مقدار (hb_font_t *font, ثابت تهی *base) ثابت
  { بازگشت hb_saturate_add (font->em_scale_y (value), (base+deviceTable).get_y_delta (font, get_var_store (font, base))); }

  ثابت ItemVariationStore &get_var_store (hb_font_t *font, ثابت تهی *base) ثابت
  {
#اگر !defined(HB_NO_VAR) && !defined(HB_NO_OT_LAYOUT)
    اگر ((base+deviceTable).is_variation_device ())
      بازگشت font->face->table.GDEF->table->get_var_store ();
#endif
    بازگشت Null (ItemVariationStore);
  }

  Math_مقدار_Record* copy (hb_serialize_context_t *c, ثابت تهی *base,
                         ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحیح>> *variation_map) ثابت
  {
    TRACE_SERIALIZE (این);
    auto *out = c->embed (این);
    اگر (unlikely (!out)) return_trace (nullptr);
    hb_pair_t<unsigned, صحیح> *varidx_delta;
    اگر (variation_map->has ((base+deviceTable).get_variation_index (), &varidx_delta) &&
        !c->check_assign (out->value, (int64_t) value + hb_second (*varidx_delta),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (nullptr);
    out->deviceTable.serialize_copy (c, deviceTable, base, 0, hb_serialize_context_t::Head, variation_map);

    return_trace (out);
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c, ثابت تهی *base) ثابت
  { (base+deviceTable).collect_variation_indices (c); }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت تهی *base) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) && deviceTable.sanitize (c, base));
  }

  محافظتشده:
  HBINT16		value;		/* The X or Y value in design units */
  Offset16To<Device>	deviceTable;	/* Offset to the device table - from the
					 * beginning of parent table.  May be NULL.
					 * Suggested format for device table is 1. */

  عمومی:
  DEFINE_SIZE_STATIC (4);
};

ساختار MathConstants
{
  friend ساختار MATH;

  MathConstants* copy (hb_serialize_context_t *c,
                      ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحیح>> *variation_map) ثابت
  {
    TRACE_SERIALIZE (این);
    auto *out = c->start_embed (این);

    HBINT16 *p = c->allocate_size<HBINT16> (HBINT16::static_size * 2);
    اگر (unlikely (!p)) return_trace (nullptr);
    hb_memcpy (p, percentScaleDown, HBINT16::static_size * 2);

    HBUINT16 *m = c->allocate_size<HBUINT16> (HBUINT16::static_size * 2);
    اگر (unlikely (!m)) return_trace (nullptr);
    hb_memcpy (m, minHeight, HBUINT16::static_size * 2);

    unsigned شمار = ARRAY_LENGTH (math_مقدار_Records);
    برای (unsigned i = 0; i < شمار; i++)
      اگر (!c->copy (math_مقدار_Records[i], این, variation_map))
        return_trace (nullptr);

    اگر (!c->embed (radicalDegreeBottomRaisePercent)) return_trace (nullptr);
    return_trace (out);
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    برای (ثابت auto &record : math_مقدار_Records)
      record.collect_variation_indices (c, این);
  }

  منطقی sanitize_math_مقدار_records (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);

    unsigned صحیح شمار = ARRAY_LENGTH (math_مقدار_Records);
    برای (unsigned صحیح i = 0; i < شمار; i++)
      اگر (!math_مقدار_Records[i].sanitize (c, این))
	return_trace (نادرست);

    return_trace (درست);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) && sanitize_math_مقدار_records (c));
  }

  hb_position_t get_مقدار (hb_ot_math_constant_t constant,
			   hb_font_t *font) ثابت
  {
    انتخاب (constant) {

    حالت HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_DOWN:
    حالت HB_OT_MATH_CONSTANT_SCRIPT_SCRIPT_PERCENT_SCALE_DOWN:
      بازگشت percentScaleDown[constant - HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_DOWN];

    حالت HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_HEIGHT:
    حالت HB_OT_MATH_CONSTANT_DISPLAY_OPERATOR_MIN_HEIGHT:
      بازگشت font->em_scale_y (minHeight[constant - HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_HEIGHT]);

    حالت HB_OT_MATH_CONSTANT_RADICAL_KERN_AFTER_DEGREE:
    حالت HB_OT_MATH_CONSTANT_RADICAL_KERN_BEFORE_DEGREE:
    حالت HB_OT_MATH_CONSTANT_SKEWED_FRACTION_HORIZONTAL_GAP:
    حالت HB_OT_MATH_CONSTANT_SPACE_AFTER_SCRIPT:
      بازگشت math_مقدار_Records[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_x_مقدار (font, این);

    حالت HB_OT_MATH_CONSTANT_ACCENT_BASE_HEIGHT:
    حالت HB_OT_MATH_CONSTANT_AXIS_HEIGHT:
    حالت HB_OT_MATH_CONSTANT_FLATTENED_ACCENT_BASE_HEIGHT:
    حالت HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_DISPLAY_STYLE_SHIFT_DOWN:
    حالت HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_GAP_MIN:
    حالت HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_SHIFT_DOWN:
    حالت HB_OT_MATH_CONSTANT_FRACTION_DENOM_DISPLAY_STYLE_GAP_MIN:
    حالت HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_DISPLAY_STYLE_SHIFT_UP:
    حالت HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_GAP_MIN:
    حالت HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_SHIFT_UP:
    حالت HB_OT_MATH_CONSTANT_FRACTION_رقم_DISPLAY_STYLE_GAP_MIN:
    حالت HB_OT_MATH_CONSTANT_FRACTION_RULE_THICKNESS:
    حالت HB_OT_MATH_CONSTANT_LOWER_LIMIT_BASELINE_DROP_MIN:
    حالت HB_OT_MATH_CONSTANT_LOWER_LIMIT_GAP_MIN:
    حالت HB_OT_MATH_CONSTANT_MATH_LEADING:
    حالت HB_OT_MATH_CONSTANT_OVERBAR_EXTRA_ASCENDER:
    حالت HB_OT_MATH_CONSTANT_OVERBAR_RULE_THICKNESS:
    حالت HB_OT_MATH_CONSTANT_OVERBAR_VERTICAL_GAP:
    حالت HB_OT_MATH_CONSTANT_RADICAL_DISPLAY_STYLE_VERTICAL_GAP:
    حالت HB_OT_MATH_CONSTANT_RADICAL_EXTRA_ASCENDER:
    حالت HB_OT_MATH_CONSTANT_RADICAL_RULE_THICKNESS:
    حالت HB_OT_MATH_CONSTANT_RADICAL_VERTICAL_GAP:
    حالت HB_OT_MATH_CONSTANT_SKEWED_FRACTION_VERTICAL_GAP:
    حالت HB_OT_MATH_CONSTANT_پشته_BOTTOM_DISPLAY_STYLE_SHIFT_DOWN:
    حالت HB_OT_MATH_CONSTANT_پشته_BOTTOM_SHIFT_DOWN:
    حالت HB_OT_MATH_CONSTANT_پشته_DISPLAY_STYLE_GAP_MIN:
    حالت HB_OT_MATH_CONSTANT_پشته_GAP_MIN:
    حالت HB_OT_MATH_CONSTANT_پشته_بالاترین_DISPLAY_STYLE_SHIFT_UP:
    حالت HB_OT_MATH_CONSTANT_پشته_بالاترین_SHIFT_UP:
    حالت HB_OT_MATH_CONSTANT_STRETCH_پشته_BOTTOM_SHIFT_DOWN:
    حالت HB_OT_MATH_CONSTANT_STRETCH_پشته_GAP_ABOVE_MIN:
    حالت HB_OT_MATH_CONSTANT_STRETCH_پشته_GAP_BELOW_MIN:
    حالت HB_OT_MATH_CONSTANT_STRETCH_پشته_بالاترین_SHIFT_UP:
    حالت HB_OT_MATH_CONSTANT_SUBSCRIPT_BASELINE_DROP_MIN:
    حالت HB_OT_MATH_CONSTANT_SUBSCRIPT_SHIFT_DOWN:
    حالت HB_OT_MATH_CONSTANT_SUBSCRIPT_بالاترین_MAX:
    حالت HB_OT_MATH_CONSTANT_SUB_SUPERSCRIPT_GAP_MIN:
    حالت HB_OT_MATH_CONSTANT_SUPERSCRIPT_BASELINE_DROP_MAX:
    حالت HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MAX_WITH_SUBSCRIPT:
    حالت HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MIN:
    حالت HB_OT_MATH_CONSTANT_SUPERSCRIPT_SHIFT_UP:
    حالت HB_OT_MATH_CONSTANT_SUPERSCRIPT_SHIFT_UP_CRAMPED:
    حالت HB_OT_MATH_CONSTANT_UNDERBAR_EXTRA_DESCENDER:
    حالت HB_OT_MATH_CONSTANT_UNDERBAR_RULE_THICKNESS:
    حالت HB_OT_MATH_CONSTANT_UNDERBAR_VERTICAL_GAP:
    حالت HB_OT_MATH_CONSTANT_UPPER_LIMIT_BASELINE_RISE_MIN:
    حالت HB_OT_MATH_CONSTANT_UPPER_LIMIT_GAP_MIN:
      بازگشت math_مقدار_Records[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_y_مقدار (font, این);

    حالت HB_OT_MATH_CONSTANT_RADICAL_DEGREE_BOTTOM_RAISE_PERCENT:
      بازگشت radicalDegreeBottomRaisePercent;

    پیشفرض:
      بازگشت 0;
    }
  }

  محافظتشده:
  HBINT16 percentScaleDown[2];
  HBUINT16 minHeight[2];
  Math_مقدار_Record math_مقدار_Records[51];
  HBINT16 radicalDegreeBottomRaisePercent;

  عمومی:
  DEFINE_SIZE_STATIC (214);
};

ساختار MathItalicsCorrectionInfo
{
  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    ثابت hb_set_t &glyphset = c->plan->_glyphset_mathed;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (این+coverage, italicsCorrection)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->italicsCorrection, این, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (درست);
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    برای (ثابت auto &entry : hb_zip (این+coverage, italicsCorrection))
      اگر (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, این);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  coverage.sanitize (c, این) &&
		  italicsCorrection.sanitize (c, این));
  }

  hb_position_t get_مقدار (hb_codepoint_t glyph,
			   hb_font_t *font) ثابت
  {
    unsigned صحیح index = (این+coverage).get_coverage (glyph);
    بازگشت italicsCorrection[index].get_x_مقدار (font, این);
  }

  محافظتشده:
  Offset16To<Coverage>       coverage;		/* Offset to Coverage table -
						 * from the beginning of
						 * MathItalicsCorrectionInfo
						 * table. */
  Array16Of<Math_مقدار_Record> italicsCorrection;	/* Array of MathValueRecords
						 * defining italics correction
						 * values for each
						 * covered glyph. */

  عمومی:
  DEFINE_SIZE_ARRAY (4, italicsCorrection);
};

ساختار Math_بالاترین_Accent_Attachment
{
  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    ثابت hb_set_t &glyphset = c->plan->_glyphset_mathed;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (این+بالاترین_Accent_Coverage, بالاترین_Accent_Attachment)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->بالاترین_Accent_Attachment, این, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->بالاترین_Accent_Coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (درست);
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    برای (ثابت auto &entry : hb_zip (این+بالاترین_Accent_Coverage, بالاترین_Accent_Attachment))
      اگر (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, این);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  بالاترین_Accent_Coverage.sanitize (c, این) &&
		  بالاترین_Accent_Attachment.sanitize (c, این));
  }

  hb_position_t get_مقدار (hb_codepoint_t glyph,
			   hb_font_t *font) ثابت
  {
    unsigned صحیح index = (این+بالاترین_Accent_Coverage).get_coverage (glyph);
    اگر (index == NOT_COVERED)
      بازگشت font->get_glyph_h_advance (glyph) / 2;
    بازگشت بالاترین_Accent_Attachment[index].get_x_مقدار (font, این);
  }

  محافظتشده:
  Offset16To<Coverage>       بالاترین_Accent_Coverage;   /* Offset to Coverage table -
						 * from the beginning of
						 * MathTopAccentAttachment
						 * table. */
  Array16Of<Math_مقدار_Record> بالاترین_Accent_Attachment; /* Array of MathValueRecords
						 * defining top accent
						 * attachment points for each
						 * covered glyph. */

  عمومی:
  DEFINE_SIZE_ARRAY (2 + 2, بالاترین_Accent_Attachment);
};

ساختار MathKern
{
  MathKern* copy (hb_serialize_context_t *c,
                      ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحیح>> *variation_map) ثابت
  {
    TRACE_SERIALIZE (این);
    auto *out = c->start_embed (این);

    اگر (unlikely (!c->embed (height_شمار))) return_trace (nullptr);

    unsigned شمار = 2 * height_شمار + 1;
    برای (unsigned i = 0; i < شمار; i++)
      اگر (!c->copy (math_مقدار_Records_Z.arrayZ[i], این, variation_map))
        return_trace (nullptr);

    return_trace (out);
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    برای (ثابت auto &record : math_مقدار_Records_Z.as_array (2 * height_شمار + 1))
      record.collect_variation_indices (c, این);
  }

  منطقی sanitize_math_مقدار_records (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    unsigned صحیح شمار = 2 * height_شمار + 1;
    برای (unsigned صحیح i = 0; i < شمار; i++)
      اگر (!math_مقدار_Records_Z.arrayZ[i].sanitize (c, این)) return_trace (نادرست);
    return_trace (درست);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  hb_barrier () &&
		  c->check_array (math_مقدار_Records_Z.arrayZ, 2 * height_شمار + 1) &&
		  sanitize_math_مقدار_records (c));
  }

  hb_position_t get_مقدار (hb_position_t correction_height, hb_font_t *font) ثابت
  {
    ثابت Math_مقدار_Record* correctionHeight = math_مقدار_Records_Z.arrayZ;
    ثابت Math_مقدار_Record* kern_مقدار = math_مقدار_Records_Z.arrayZ + height_شمار;
    صحیح sign = font->y_scale < 0 ? -1 : +1;

    /* According to OpenType spec (v1.9), except for the boundary cases, the index
     * chosen for kern value should be i such that
     *    correctionHeight[i-1] <= correction_height < correctionHeight[i]
     * We can use the binary search algorithm of std::upper_bound(). Or, we can
     * use the internal hb_bsearch_impl.
     */
    unsigned صحیح pos;
    auto cmp = +[](ثابت تهی* key, ثابت تهی* p,
                   صحیح sign, hb_font_t* font, ثابت MathKern* mathKern) -> صحیح {
      hb_position_t a = *(hb_position_t *) key;
      hb_position_t b = ((Math_مقدار_Record *) p)->get_y_مقدار (font, mathKern);
      اگر (a == b) بازگشت 0;
      بازگشت sign > 0 ? (a < b ? -1 : +1) : (b < a ? -1 : +1);
    };
    unsigned صحیح i = hb_bsearch_impl(&pos, correction_height, correctionHeight,
                                     height_شمار, Math_مقدار_Record::static_size,
                                     cmp, sign, font, این) ? pos + 1 : pos;
    بازگشت kern_مقدار[i].get_x_مقدار (font, این);
  }

  unsigned صحیح get_entries (unsigned صحیح start_offset,
			    unsigned صحیح *entries_شمار, /* IN/OUT */
			    hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			    hb_font_t *font) ثابت
  {
    ثابت Math_مقدار_Record* correctionHeight = math_مقدار_Records_Z.arrayZ;
    ثابت Math_مقدار_Record* kern_مقدار = math_مقدار_Records_Z.arrayZ + height_شمار;
    ثابت unsigned صحیح entries_شمار_2 = height_شمار + 1;

    اگر (entries_شمار && kern_entries)
    {
      unsigned صحیح start = hb_min (start_offset, entries_شمار_2);
      unsigned صحیح end = hb_min (start + *entries_شمار, entries_شمار_2);
      *entries_شمار = end - start;

      برای (unsigned صحیح i = 0; i < *entries_شمار; i++) {
	unsigned صحیح j = start + i;

	hb_position_t max_height;
	اگر (j == height_شمار) {
	  max_height = INT32_MAX;
	} وگرنه {
	  max_height = correctionHeight[j].get_y_مقدار (font, این);
	}

	kern_entries[i] = {max_height, kern_مقدار[j].get_x_مقدار (font, این)};
      }
    }
    بازگشت entries_شمار_2;
  }

  محافظتشده:
  HBUINT16	height_شمار;
  UnsizedArrayOf<Math_مقدار_Record>
		math_مقدار_Records_Z;
				/* Array of correction heights at
				 * which the kern value changes.
				 * Sorted by the height value in
				 * design units (heightCount entries),
				 * Followed by:
				 * Array of kern values corresponding
				 * to heights. (heightCount+1 entries).
				 */

  عمومی:
  DEFINE_SIZE_ARRAY (2, math_مقدار_Records_Z);
};

ساختار MathKernInfoRecord
{
  MathKernInfoRecord* copy (hb_serialize_context_t *c, ثابت تهی *base,
                            ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحیح>> *variation_map) ثابت
  {
    TRACE_SERIALIZE (این);
    auto *out = c->embed (این);
    اگر (unlikely (!out)) return_trace (nullptr);

    unsigned شمار = ARRAY_LENGTH (mathKern);
    برای (unsigned i = 0; i < شمار; i++)
      out->mathKern[i].serialize_copy (c, mathKern[i], base, 0, hb_serialize_context_t::Head, variation_map);

    return_trace (out);
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c, ثابت تهی *base) ثابت
  {
    برای (ثابت auto &offset : mathKern)
      (base+offset).collect_variation_indices (c);
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت تهی *base) ثابت
  {
    TRACE_SANITIZE (این);

    unsigned صحیح شمار = ARRAY_LENGTH (mathKern);
    برای (unsigned صحیح i = 0; i < شمار; i++)
      اگر (unlikely (!mathKern[i].sanitize (c, base)))
	return_trace (نادرست);

    return_trace (درست);
  }

  hb_position_t get_kerning (hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font,
			     ثابت تهی *base) ثابت
  {
    unsigned صحیح idx = kern;
    اگر (unlikely (idx >= ARRAY_LENGTH (mathKern))) بازگشت 0;
    بازگشت (base+mathKern[idx]).get_مقدار (correction_height, font);
  }

  unsigned صحیح get_kernings (hb_ot_math_kern_t kern,
			     unsigned صحیح start_offset,
			     unsigned صحیح *entries_شمار, /* IN/OUT */
			     hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			     hb_font_t *font,
			     ثابت تهی *base) ثابت
  {
    unsigned صحیح idx = kern;
    اگر (unlikely (idx >= ARRAY_LENGTH (mathKern)) || !mathKern[idx]) {
      اگر (entries_شمار) *entries_شمار = 0;
      بازگشت 0;
    }
    بازگشت (base+mathKern[idx]).get_entries (start_offset,
					     entries_شمار,
					     kern_entries,
					     font);
  }

  محافظتشده:
  /* Offset to MathKern table for each corner -
   * from the beginning of MathKernInfo table.  May be NULL. */
  Offset16To<MathKern> mathKern[4];

  عمومی:
  DEFINE_SIZE_STATIC (8);
};

ساختار MathKernInfo
{
  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    ثابت hb_set_t &glyphset = c->plan->_glyphset_mathed;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (این+mathKernCoverage, mathKernInfoRecords)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->mathKernInfoRecords, این, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->mathKernCoverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (درست);
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    برای (ثابت auto &entry : hb_zip (این+mathKernCoverage, mathKernInfoRecords))
      اگر (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, این);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  mathKernCoverage.sanitize (c, این) &&
		  mathKernInfoRecords.sanitize (c, این));
  }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font) ثابت
  {
    unsigned صحیح index = (این+mathKernCoverage).get_coverage (glyph);
    بازگشت mathKernInfoRecords[index].get_kerning (kern, correction_height, font, این);
  }

  unsigned صحیح get_kernings (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     unsigned صحیح start_offset,
			     unsigned صحیح *entries_شمار, /* IN/OUT */
			     hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			     hb_font_t *font) ثابت
  {
    unsigned صحیح index = (این+mathKernCoverage).get_coverage (glyph);
    بازگشت mathKernInfoRecords[index].get_kernings (kern,
						    start_offset,
						    entries_شمار,
						    kern_entries,
						    font,
						    این);
  }

  محافظتشده:
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

  عمومی:
  DEFINE_SIZE_ARRAY (4, mathKernInfoRecords);
};

ساختار MathGlyphInfo
{
  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->embed (*این);
    اگر (unlikely (!out)) return_trace (نادرست);

    out->mathItalicsCorrectionInfo.serialize_subset (c, mathItalicsCorrectionInfo, این);
    out->math_بالاترین_Accent_Attachment.serialize_subset (c, math_بالاترین_Accent_Attachment, این);

    ثابت hb_set_t &glyphset = c->plan->_glyphset_mathed;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    auto it =
    + hb_iter (این+extendedShapeCoverage)
    | hb_take (c->plan->source->get_رقم_glyphs ())
    | hb_filter (glyphset)
    | hb_map_retains_sorting (glyph_map)
    ;

    اگر (it) out->extendedShapeCoverage.serialize_serialize (c->serializer, it);
    وگرنه out->extendedShapeCoverage = 0;

    out->mathKernInfo.serialize_subset (c, mathKernInfo, این);
    return_trace (درست);
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    (این+mathItalicsCorrectionInfo).collect_variation_indices (c);
    (این+math_بالاترین_Accent_Attachment).collect_variation_indices (c);
    (این+mathKernInfo).collect_variation_indices (c);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  mathItalicsCorrectionInfo.sanitize (c, این) &&
		  math_بالاترین_Accent_Attachment.sanitize (c, این) &&
		  extendedShapeCoverage.sanitize (c, این) &&
		  mathKernInfo.sanitize (c, این));
  }

  hb_position_t
  get_italics_correction (hb_codepoint_t  glyph, hb_font_t *font) ثابت
  { بازگشت (این+mathItalicsCorrectionInfo).get_مقدار (glyph, font); }

  hb_position_t
  get_بالاترین_accent_attachment (hb_codepoint_t  glyph, hb_font_t *font) ثابت
  { بازگشت (این+math_بالاترین_Accent_Attachment).get_مقدار (glyph, font); }

  منطقی is_extended_shape (hb_codepoint_t glyph) ثابت
  { بازگشت (این+extendedShapeCoverage).get_coverage (glyph) != NOT_COVERED; }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font) ثابت
  { بازگشت (این+mathKernInfo).get_kerning (glyph, kern, correction_height, font); }

  hb_position_t get_kernings (hb_codepoint_t glyph,
			      hb_ot_math_kern_t kern,
			      unsigned صحیح start_offset,
			      unsigned صحیح *entries_شمار, /* IN/OUT */
			      hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			      hb_font_t *font) ثابت
  { بازگشت (این+mathKernInfo).get_kernings (glyph,
					     kern,
					     start_offset,
					     entries_شمار,
					     kern_entries,
					     font); }

  محافظتشده:
  /* Offset to MathItalicsCorrectionInfo table -
   * from the beginning of MathGlyphInfo table. */
  Offset16To<MathItalicsCorrectionInfo> mathItalicsCorrectionInfo;

  /* Offset to MathTopAccentAttachment table -
   * from the beginning of MathGlyphInfo table. */
  Offset16To<Math_بالاترین_Accent_Attachment> math_بالاترین_Accent_Attachment;

  /* Offset to coverage table for Extended Shape glyphs -
   * from the beginning of MathGlyphInfo table. When the left or right glyph of
   * a box is an extended shape variant, the (ink) box (and not the default
   * position defined by values in MathConstants table) should be used for
   * vertical positioning purposes.  May be NULL.. */
  Offset16To<Coverage> extendedShapeCoverage;

   /* Offset to MathKernInfo table -
    * from the beginning of MathGlyphInfo table. */
  Offset16To<MathKernInfo> mathKernInfo;

  عمومی:
  DEFINE_SIZE_STATIC (8);
};

ساختار MathGlyphVariantRecord
{
  friend ساختار MathGlyphConstruction;

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->embed (این);
    اگر (unlikely (!out)) return_trace (نادرست);

    ثابت hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_assign (out->variantGlyph, glyph_map.get (variantGlyph), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  تهی depend (hb_depend_data_builder_t *depend_data, unsigned source) ثابت
  { depend_data->add_depend(source, HB_OT_TAG_MATH, variantGlyph); }

  تهی closure_glyphs (hb_set_t *variant_glyphs) ثابت
  { variant_glyphs->add (variantGlyph); }

  محافظتشده:
  HBGlyphID16 variantGlyph;       /* Glyph ID for the variant. */
  HBUINT16  advanceMeasurement; /* Advance width/height, in design units, of the
				 * variant, in the direction of requested
				 * glyph extension. */

  عمومی:
  DEFINE_SIZE_STATIC (4);
};

ساختار PartFlags : HBUINT16
{
  شمارش Flags {
    Extender	= 0x0001u, /* If set, the part can be skipped or repeated. */

    Defined	= 0x0001u, /* All defined flags. */
  };

  عمومی:
  DEFINE_SIZE_STATIC (2);
};

ساختار MathGlyphPartRecord
{
  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->embed (این);
    اگر (unlikely (!out)) return_trace (نادرست);

    ثابت hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_assign (out->glyph, glyph_map.get (glyph), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  تهی extract (hb_ot_math_glyph_part_t &out,
		int64_t mult,
		hb_font_t *font) ثابت
  {
    out.glyph			= glyph;

    out.start_connector_length	= font->em_mult (startConnectorLength, mult);
    out.end_connector_length	= font->em_mult (endConnectorLength, mult);
    out.full_advance		= font->em_mult (fullAdvance, mult);

    static_assert ((unsigned صحیح) HB_OT_MATH_GLYPH_PART_FLAG_EXTENDER ==
		   (unsigned صحیح) PartFlags::Extender, "");

    out.flags = (hb_ot_math_glyph_part_flags_t)
		(unsigned صحیح)
		(partFlags & PartFlags::Defined);
  }

  تهی depend (hb_depend_data_builder_t *depend_data, unsigned source) ثابت
  { depend_data->add_depend(source, HB_OT_TAG_MATH, glyph); }

  تهی closure_glyphs (hb_set_t *variant_glyphs) ثابت
  { variant_glyphs->add (glyph); }

  محافظتشده:
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

  عمومی:
  DEFINE_SIZE_STATIC (10);
};

ساختار MathGlyphAssembly
{
  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);

    اگر (!c->serializer->copy (italicsCorrection, این, &c->plan->layout_variation_idx_delta_map)) return_trace (نادرست);
    اگر (!c->serializer->copy<HBUINT16> (partRecords.len)) return_trace (نادرست);

    برای (ثابت auto& record : partRecords.iter ())
      اگر (!record.subset (c)) return_trace (نادرست);
    return_trace (درست);
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    italicsCorrection.collect_variation_indices (c, این);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  italicsCorrection.sanitize (c, این) &&
		  partRecords.sanitize (c));
  }

  unsigned صحیح get_parts (hb_direction_t direction,
			  hb_font_t *font,
			  unsigned صحیح start_offset,
			  unsigned صحیح *parts_شمار, /* IN/OUT */
			  hb_ot_math_glyph_part_t *parts /* OUT */,
			  hb_position_t *italics_correction /* OUT */) ثابت
  {
    اگر (parts_شمار && parts)
    {
      int64_t mult = font->dir_mult (direction);
      برای (auto _ : hb_zip (partRecords.as_array ().sub_array (start_offset, parts_شمار),
			    hb_array (parts, *parts_شمار)))
	_.first.extract (_.second, mult, font);
    }

    اگر (italics_correction)
      *italics_correction = italicsCorrection.get_x_مقدار (font, این);

    بازگشت partRecords.len;
  }

  تهی depend (hb_depend_data_builder_t *depend_data, unsigned source) ثابت
  {
    برای (ثابت auto& _ : partRecords.iter ())
      _.depend (depend_data, source);
  }

  تهی closure_glyphs (hb_set_t *variant_glyphs) ثابت
  {
    برای (ثابت auto& _ : partRecords.iter ())
      _.closure_glyphs (variant_glyphs);
  }

  محافظتشده:
  Math_مقدار_Record
		italicsCorrection;
				/* Italics correction of this
				 * MathGlyphAssembly. Should not
				 * depend on the assembly size. */
  Array16Of<MathGlyphPartRecord>
		partRecords;	/* Array of part records, from
				 * left to right and bottom to
				 * top. */

  عمومی:
  DEFINE_SIZE_ARRAY (6, partRecords);
};

ساختار MathGlyphConstruction
{
  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);

    out->glyphAssembly.serialize_subset (c, glyphAssembly, این);

    اگر (!c->serializer->check_assign (out->mathGlyphVariantRecord.len, mathGlyphVariantRecord.len, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (نادرست);
    برای (ثابت auto& record : mathGlyphVariantRecord.iter ())
      اگر (!record.subset (c)) return_trace (نادرست);

    return_trace (درست);
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    (این+glyphAssembly).collect_variation_indices (c);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  glyphAssembly.sanitize (c, این) &&
		  mathGlyphVariantRecord.sanitize (c));
  }

  ثابت MathGlyphAssembly &get_assembly () ثابت { بازگشت این+glyphAssembly; }

  unsigned صحیح get_variants (hb_direction_t direction,
			     hb_font_t *font,
			     unsigned صحیح start_offset,
			     unsigned صحیح *variants_شمار, /* IN/OUT */
			     hb_ot_math_glyph_variant_t *variants /* OUT */) ثابت
  {
    اگر (variants_شمار && variants)
    {
      int64_t mult = font->dir_mult (direction);
      برای (auto _ : hb_zip (mathGlyphVariantRecord.as_array ().sub_array (start_offset, variants_شمار),
			    hb_array (variants, *variants_شمار)))
	_.second = {_.first.variantGlyph, font->em_mult (_.first.advanceMeasurement, mult)};
    }
    بازگشت mathGlyphVariantRecord.len;
  }

  تهی depend (hb_depend_data_builder_t *depend_data, unsigned source) ثابت
  {
    (این+glyphAssembly).depend (depend_data, source);

    برای (ثابت auto& _ : mathGlyphVariantRecord.iter ())
      _.depend (depend_data, source);
  }

  تهی closure_glyphs (hb_set_t *variant_glyphs) ثابت
  {
    (این+glyphAssembly).closure_glyphs (variant_glyphs);

    برای (ثابت auto& _ : mathGlyphVariantRecord.iter ())
      _.closure_glyphs (variant_glyphs);
  }

  محافظتشده:
  /* Offset to MathGlyphAssembly table for this shape - from the beginning of
     MathGlyphConstruction table.  May be NULL. */
  Offset16To<MathGlyphAssembly>	  glyphAssembly;

  /* MathGlyphVariantRecords for alternative variants of the glyphs. */
  Array16Of<MathGlyphVariantRecord> mathGlyphVariantRecord;

  عمومی:
  DEFINE_SIZE_ARRAY (4, mathGlyphVariantRecord);
};

ساختار MathVariants
{
  تهی depend (hb_depend_data_builder_t *depend_data) ثابت
  {
    ثابت hb_array_t<ثابت Offset16To<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vert_Glyph_شمار + horiz_Glyph_شمار);

    اگر (vertGlyphCoverage)
    {
      ثابت auto vert_offsets = glyph_construction_offsets.sub_array (0, vert_Glyph_شمار);
      + hb_zip (این+vertGlyphCoverage, vert_offsets)
      | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, ثابت Offset16To<MathGlyphConstruction>&> &_)
                  {
                    ثابت MathGlyphConstruction &mgc = این+_.second;
                    mgc.depend (depend_data, _.first);
                  })
      ;
    }
    اگر (horizGlyphCoverage)
    {
      ثابت auto hori_offsets = glyph_construction_offsets.sub_array (vert_Glyph_شمار, horiz_Glyph_شمار);
      + hb_zip (این+horizGlyphCoverage, hori_offsets)
      | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, ثابت Offset16To<MathGlyphConstruction>&> &_)
                  {
                    ثابت MathGlyphConstruction &mgc = این+_.second;
                    mgc.depend (depend_data, _.first);
                  })
      ;
    }
  }

  تهی closure_glyphs (ثابت hb_set_t *glyph_set,
                       hb_set_t *variant_glyphs) ثابت
  {
    ثابت hb_array_t<ثابت Offset16To<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vert_Glyph_شمار + horiz_Glyph_شمار);

    اگر (vertGlyphCoverage)
    {
      ثابت auto vert_offsets = glyph_construction_offsets.sub_array (0, vert_Glyph_شمار);
      + hb_zip (این+vertGlyphCoverage, vert_offsets)
      | hb_filter (glyph_set, hb_first)
      | hb_map (hb_second)
      | hb_map (hb_add (این))
      | hb_apply ([=] (ثابت MathGlyphConstruction &_) { _.closure_glyphs (variant_glyphs); })
      ;
    }

    اگر (horizGlyphCoverage)
    {
      ثابت auto hori_offsets = glyph_construction_offsets.sub_array (vert_Glyph_شمار, horiz_Glyph_شمار);
      + hb_zip (این+horizGlyphCoverage, hori_offsets)
      | hb_filter (glyph_set, hb_first)
      | hb_map (hb_second)
      | hb_map (hb_add (این))
      | hb_apply ([=] (ثابت MathGlyphConstruction &_) { _.closure_glyphs (variant_glyphs); })
      ;
    }
  }

  تهی collect_coverage_and_indices (hb_sorted_vector_t<hb_codepoint_t>& new_coverage,
                                     ثابت Offset16To<Coverage>& coverage,
                                     unsigned i,
                                     unsigned end_index,
                                     hb_set_t& indices,
                                     ثابت hb_set_t& glyphset,
                                     ثابت hb_map_t& glyph_map) ثابت
  {
    اگر (!coverage) بازگشت;

    برای (ثابت auto _ : (این+coverage).iter ())
    {
      اگر (i >= end_index) بازگشت;
      اگر (glyphset.has (_))
      {
        unsigned new_gid = glyph_map.get (_);
        new_coverage.push (new_gid);
        indices.add (i);
      }
      i++;
    }
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    ثابت hb_set_t &glyphset = c->plan->_glyphset_cmaped;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);
    اگر (!c->serializer->check_assign (out->minConnectorOverlap, minConnectorOverlap, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (نادرست);

    hb_sorted_vector_t<hb_codepoint_t> new_vert_coverage;
    hb_sorted_vector_t<hb_codepoint_t> new_hori_coverage;
    hb_set_t indices;
    collect_coverage_and_indices (new_vert_coverage, vertGlyphCoverage, 0, vert_Glyph_شمار, indices, glyphset, glyph_map);
    collect_coverage_and_indices (new_hori_coverage, horizGlyphCoverage, vert_Glyph_شمار, vert_Glyph_شمار + horiz_Glyph_شمار, indices, glyphset, glyph_map);

    اگر (!c->serializer->check_assign (out->vert_Glyph_شمار, new_vert_coverage.length, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (نادرست);
    اگر (!c->serializer->check_assign (out->horiz_Glyph_شمار, new_hori_coverage.length, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (نادرست);

    برای (unsigned i : indices.iter ())
    {
      auto *o = c->serializer->embed (glyphConstruction[i]);
      اگر (!o) return_trace (نادرست);
      o->serialize_subset (c, glyphConstruction[i], این);
    }

    اگر (new_vert_coverage)
      out->vertGlyphCoverage.serialize_serialize (c->serializer, new_vert_coverage.iter ());

    اگر (new_hori_coverage)
    out->horizGlyphCoverage.serialize_serialize (c->serializer, new_hori_coverage.iter ());
    return_trace (درست);
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    برای (ثابت auto &entry : hb_zip (این+vertGlyphCoverage, glyphConstruction.as_array (vert_Glyph_شمار)))
      اگر (c->glyph_set->has (hb_first (entry)))
        (این+hb_second (entry)).collect_variation_indices (c);
    برای (ثابت auto &entry : hb_zip (این+horizGlyphCoverage, glyphConstruction.as_array (vert_Glyph_شمار + horiz_Glyph_شمار).sub_array (vert_Glyph_شمار)))
      اگر (c->glyph_set->has (hb_first (entry)))
        (این+hb_second (entry)).collect_variation_indices (c);
  }

  منطقی sanitize_offsets (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    unsigned صحیح شمار = vert_Glyph_شمار + horiz_Glyph_شمار;
    برای (unsigned صحیح i = 0; i < شمار; i++)
      اگر (!glyphConstruction.arrayZ[i].sanitize (c, این)) return_trace (نادرست);
    return_trace (درست);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  vertGlyphCoverage.sanitize (c, این) &&
		  horizGlyphCoverage.sanitize (c, این) &&
		  hb_barrier () &&
		  c->check_array (glyphConstruction.arrayZ, vert_Glyph_شمار + horiz_Glyph_شمار) &&
		  sanitize_offsets (c));
  }

  hb_position_t get_min_connector_overlap (hb_direction_t direction,
						  hb_font_t *font) ثابت
  { بازگشت font->em_scale_dir (minConnectorOverlap, direction); }

  unsigned صحیح get_glyph_variants (hb_codepoint_t glyph,
				   hb_direction_t direction,
				   hb_font_t *font,
				   unsigned صحیح start_offset,
				   unsigned صحیح *variants_شمار, /* IN/OUT */
				   hb_ot_math_glyph_variant_t *variants /* OUT */) ثابت
  { بازگشت get_glyph_construction (glyph, direction, font)
	   .get_variants (direction, font, start_offset, variants_شمار, variants); }

  unsigned صحیح get_glyph_parts (hb_codepoint_t glyph,
				hb_direction_t direction,
				hb_font_t *font,
				unsigned صحیح start_offset,
				unsigned صحیح *parts_شمار, /* IN/OUT */
				hb_ot_math_glyph_part_t *parts /* OUT */,
				hb_position_t *italics_correction /* OUT */) ثابت
  { بازگشت get_glyph_construction (glyph, direction, font)
	   .get_assembly ()
	   .get_parts (direction, font,
		       start_offset, parts_شمار, parts,
		       italics_correction); }

  خصوصی:
  ثابت MathGlyphConstruction &
  get_glyph_construction (hb_codepoint_t glyph,
			  hb_direction_t direction,
			  hb_font_t *font HB_UNUSED) ثابت
  {
    منطقی vertical = HB_DIRECTION_IS_VERTICAL (direction);
    unsigned صحیح شمار = vertical ? vert_Glyph_شمار : horiz_Glyph_شمار;
    ثابت Offset16To<Coverage> &coverage = vertical ? vertGlyphCoverage
						  : horizGlyphCoverage;

    unsigned صحیح index = (این+coverage).get_coverage (glyph);
    اگر (unlikely (index >= شمار)) بازگشت Null (MathGlyphConstruction);

    اگر (!vertical)
      index += vert_Glyph_شمار;

    بازگشت این+glyphConstruction[index];
  }

  محافظتشده:
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
  HBUINT16	vert_Glyph_شمار;	/* Number of glyphs for which
				 * information is provided for
				 * vertically growing variants. */
  HBUINT16	horiz_Glyph_شمار;/* Number of glyphs for which
				 * information is provided for
				 * horizontally growing variants. */

  /* Array of offsets to MathGlyphConstruction tables - from the beginning of
     the MathVariants table, for shapes growing in vertical/horizontal
     direction. */
  UnsizedArrayOf<Offset16To<MathGlyphConstruction>>
			glyphConstruction;

  عمومی:
  DEFINE_SIZE_ARRAY (10, glyphConstruction);
};


/*
 * MATH -- Mathematical typesetting
 * https://docs.microsoft.com/en-us/typography/opentype/spec/math
 */

ساختار MATH
{
  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_MATH;

  منطقی has_data () ثابت { بازگشت version.to_int (); }

  تهی depend (hb_depend_data_builder_t *depend_data) ثابت
  {
    اگر (mathVariants)
      (این+mathVariants).depend (depend_data);
  }

  تهی closure_glyphs (hb_set_t *glyph_set) ثابت
  {
    اگر (mathVariants)
    {
      hb_set_t variant_glyphs;
      (این+mathVariants).closure_glyphs (glyph_set, &variant_glyphs);
      hb_set_union (glyph_set, &variant_glyphs);
    }
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->embed (*این);
    اگر (unlikely (!out)) return_trace (نادرست);

    out->mathConstants.serialize_copy (c->serializer, mathConstants, این, 0, hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map);
    out->mathGlyphInfo.serialize_subset (c, mathGlyphInfo, این);
    out->mathVariants.serialize_subset (c, mathVariants, این);
    return_trace (درست);
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c, ثابت hb_set_t *variant_glyph_set) ثابت
  {
    (این+mathConstants).collect_variation_indices (c);
    (این+mathGlyphInfo).collect_variation_indices (c);
    auto *glyph_set = c->glyph_set;
    c->glyph_set = variant_glyph_set;
    (این+mathVariants).collect_variation_indices (c);
    c->glyph_set = glyph_set;
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (version.sanitize (c) &&
		  likely (version.major == 1) &&
		  hb_barrier () &&
		  mathConstants.sanitize (c, این) &&
		  mathGlyphInfo.sanitize (c, این) &&
		  mathVariants.sanitize (c, این));
  }

  // https://github.com/harfbuzz/harfbuzz/issues/4653
  HB_INTERNAL منطقی is_bad_cambria (hb_font_t *font) ثابت
  {
#ifndef HB_NO_MATH
    انتخاب HB_CODEPOINT_ENCODE3 (font->face->table.MATH.get_blob ()->length,
                                 (این+mathConstants).minHeight[1], // displayOperatorMinHeight
                                 (این+mathConstants).minHeight[0]) // delimitedSubFormulaMinHeight
    {
      /* sha1sum:ab4a4fe054d23061f3c039493d6f665cfda2ecf5  cambria.ttc
       * sha1sum:086855301bff644f9d8827b88491fcf73a6d4cb9  cambria.ttc
       * sha1sum:b1e5a3feaca2ea3dfcf79ccb377de749ecf60343  cambria.ttc */
      حالت HB_CODEPOINT_ENCODE3 (25722, 2500, 3000):
        بازگشت درست;
    }
#endif
    بازگشت نادرست;
  }

  hb_position_t get_constant (hb_ot_math_constant_t  constant,
			      hb_font_t		   *font) ثابت
  { بازگشت (این+mathConstants).get_مقدار (constant, font); }

  ثابت MathGlyphInfo &get_glyph_info () ثابت { بازگشت این+mathGlyphInfo; }

  ثابت MathVariants &get_variants () ثابت    { بازگشت این+mathVariants; }

  محافظتشده:
  FixedVersion<>version;	/* Version of the MATH table
				 * initially set to 0x00010000u */
  Offset16To<MathConstants>
		mathConstants;	/* MathConstants table */
  Offset16To<MathGlyphInfo>
		mathGlyphInfo;	/* MathGlyphInfo table */
  Offset16To<MathVariants>
		mathVariants;	/* MathVariants table */

  عمومی:
  DEFINE_SIZE_STATIC (10);
};

} /* namespace OT */


#endif /* HB_OT_MATH_TABLE_HH */

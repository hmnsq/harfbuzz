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

#அந்தர்பாவ "hb-open-type.hh"
#அந்தர்பாவ "hb-ot-layout-common.hh"
#அந்தர்பாவ "hb-ot-layout-gdef-table.hh"
#அந்தர்பாவ "hb-ot-math.h"
#அந்தர்பாவ "hb-depend-data.hh"

நாமாகாஶ OT {


கட்டமைப்பு Math_மதிப்பு_Record
{
  hb_position_t get_x_மதிப்பு (hb_font_t *font, மாறிலி வெறுமை *base) மாறிலி
  { திரும்பு hb_saturate_add (font->em_scale_x (value), (base+deviceTable).get_x_delta (font, get_var_store (font, base))); }
  hb_position_t get_y_மதிப்பு (hb_font_t *font, மாறிலி வெறுமை *base) மாறிலி
  { திரும்பு hb_saturate_add (font->em_scale_y (value), (base+deviceTable).get_y_delta (font, get_var_store (font, base))); }

  மாறிலி ItemVariationStore &get_var_store (hb_font_t *font, மாறிலி வெறுமை *base) மாறிலி
  {
#எனில் !defined(HB_NO_VAR) && !defined(HB_NO_OT_LAYOUT)
    எனில் ((base+deviceTable).is_variation_device ())
      திரும்பு font->face->table.GDEF->table->get_var_store ();
#endif
    திரும்பு Null (ItemVariationStore);
  }

  Math_மதிப்பு_Record* copy (hb_serialize_context_t *c, மாறிலி வெறுமை *base,
                         மாறிலி hb_hashmap_t<unsigned, hb_pair_t<unsigned, முழு>> *variation_map) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    auto *out = c->embed (இது);
    எனில் (unlikely (!out)) return_trace (nullptr);
    hb_pair_t<unsigned, முழு> *varidx_delta;
    எனில் (variation_map->has ((base+deviceTable).get_variation_index (), &varidx_delta) &&
        !c->check_assign (out->value, (int64_t) value + hb_second (*varidx_delta),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (nullptr);
    out->deviceTable.serialize_copy (c, deviceTable, base, 0, hb_serialize_context_t::Head, variation_map);

    return_trace (out);
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c, மாறிலி வெறுமை *base) மாறிலி
  { (base+deviceTable).collect_variation_indices (c); }

  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) && deviceTable.sanitize (c, base));
  }

  பாதுகாக்கப்பட்ட:
  HBINT16		value;		/* The X or Y value in design units */
  Offset16To<Device>	deviceTable;	/* Offset to the device table - from the
					 * beginning of parent table.  May be NULL.
					 * Suggested format for device table is 1. */

  பொது:
  DEFINE_SIZE_STATIC (4);
};

கட்டமைப்பு MathConstants
{
  friend கட்டமைப்பு MATH;

  MathConstants* copy (hb_serialize_context_t *c,
                      மாறிலி hb_hashmap_t<unsigned, hb_pair_t<unsigned, முழு>> *variation_map) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    auto *out = c->start_embed (இது);

    HBINT16 *p = c->allocate_size<HBINT16> (HBINT16::static_size * 2);
    எனில் (unlikely (!p)) return_trace (nullptr);
    hb_memcpy (p, percentScaleDown, HBINT16::static_size * 2);

    HBUINT16 *m = c->allocate_size<HBUINT16> (HBUINT16::static_size * 2);
    எனில் (unlikely (!m)) return_trace (nullptr);
    hb_memcpy (m, minHeight, HBUINT16::static_size * 2);

    unsigned மொத்தம் = ARRAY_LENGTH (math_மதிப்பு_Records);
    க்கு (unsigned i = 0; i < மொத்தம்; i++)
      எனில் (!c->copy (math_மதிப்பு_Records[i], இது, variation_map))
        return_trace (nullptr);

    எனில் (!c->embed (radicalDegreeBottomRaisePercent)) return_trace (nullptr);
    return_trace (out);
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி
  {
    க்கு (மாறிலி auto &record : math_மதிப்பு_Records)
      record.collect_variation_indices (c, இது);
  }

  தருக்க sanitize_math_மதிப்பு_records (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);

    unsigned முழு மொத்தம் = ARRAY_LENGTH (math_மதிப்பு_Records);
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
      எனில் (!math_மதிப்பு_Records[i].sanitize (c, இது))
	return_trace (பொய்);

    return_trace (உண்மை);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) && sanitize_math_மதிப்பு_records (c));
  }

  hb_position_t get_மதிப்பு (hb_ot_math_constant_t constant,
			   hb_font_t *font) மாறிலி
  {
    தேர்வு (constant) {

    நிலை HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_DOWN:
    நிலை HB_OT_MATH_CONSTANT_SCRIPT_SCRIPT_PERCENT_SCALE_DOWN:
      திரும்பு percentScaleDown[constant - HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_DOWN];

    நிலை HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_HEIGHT:
    நிலை HB_OT_MATH_CONSTANT_DISPLAY_OPERATOR_MIN_HEIGHT:
      திரும்பு font->em_scale_y (minHeight[constant - HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_HEIGHT]);

    நிலை HB_OT_MATH_CONSTANT_RADICAL_KERN_AFTER_DEGREE:
    நிலை HB_OT_MATH_CONSTANT_RADICAL_KERN_BEFORE_DEGREE:
    நிலை HB_OT_MATH_CONSTANT_SKEWED_FRACTION_HORIZONTAL_GAP:
    நிலை HB_OT_MATH_CONSTANT_SPACE_AFTER_SCRIPT:
      திரும்பு math_மதிப்பு_Records[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_x_மதிப்பு (font, இது);

    நிலை HB_OT_MATH_CONSTANT_ACCENT_BASE_HEIGHT:
    நிலை HB_OT_MATH_CONSTANT_AXIS_HEIGHT:
    நிலை HB_OT_MATH_CONSTANT_FLATTENED_ACCENT_BASE_HEIGHT:
    நிலை HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_DISPLAY_STYLE_SHIFT_DOWN:
    நிலை HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_GAP_MIN:
    நிலை HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_SHIFT_DOWN:
    நிலை HB_OT_MATH_CONSTANT_FRACTION_DENOM_DISPLAY_STYLE_GAP_MIN:
    நிலை HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_DISPLAY_STYLE_SHIFT_UP:
    நிலை HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_GAP_MIN:
    நிலை HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_SHIFT_UP:
    நிலை HB_OT_MATH_CONSTANT_FRACTION_இலக்கம்_DISPLAY_STYLE_GAP_MIN:
    நிலை HB_OT_MATH_CONSTANT_FRACTION_RULE_THICKNESS:
    நிலை HB_OT_MATH_CONSTANT_LOWER_LIMIT_BASELINE_DROP_MIN:
    நிலை HB_OT_MATH_CONSTANT_LOWER_LIMIT_GAP_MIN:
    நிலை HB_OT_MATH_CONSTANT_MATH_LEADING:
    நிலை HB_OT_MATH_CONSTANT_OVERBAR_EXTRA_ASCENDER:
    நிலை HB_OT_MATH_CONSTANT_OVERBAR_RULE_THICKNESS:
    நிலை HB_OT_MATH_CONSTANT_OVERBAR_VERTICAL_GAP:
    நிலை HB_OT_MATH_CONSTANT_RADICAL_DISPLAY_STYLE_VERTICAL_GAP:
    நிலை HB_OT_MATH_CONSTANT_RADICAL_EXTRA_ASCENDER:
    நிலை HB_OT_MATH_CONSTANT_RADICAL_RULE_THICKNESS:
    நிலை HB_OT_MATH_CONSTANT_RADICAL_VERTICAL_GAP:
    நிலை HB_OT_MATH_CONSTANT_SKEWED_FRACTION_VERTICAL_GAP:
    நிலை HB_OT_MATH_CONSTANT_அடுக்கு_BOTTOM_DISPLAY_STYLE_SHIFT_DOWN:
    நிலை HB_OT_MATH_CONSTANT_அடுக்கு_BOTTOM_SHIFT_DOWN:
    நிலை HB_OT_MATH_CONSTANT_அடுக்கு_DISPLAY_STYLE_GAP_MIN:
    நிலை HB_OT_MATH_CONSTANT_அடுக்கு_GAP_MIN:
    நிலை HB_OT_MATH_CONSTANT_அடுக்கு_உச்சி_DISPLAY_STYLE_SHIFT_UP:
    நிலை HB_OT_MATH_CONSTANT_அடுக்கு_உச்சி_SHIFT_UP:
    நிலை HB_OT_MATH_CONSTANT_STRETCH_அடுக்கு_BOTTOM_SHIFT_DOWN:
    நிலை HB_OT_MATH_CONSTANT_STRETCH_அடுக்கு_GAP_ABOVE_MIN:
    நிலை HB_OT_MATH_CONSTANT_STRETCH_அடுக்கு_GAP_BELOW_MIN:
    நிலை HB_OT_MATH_CONSTANT_STRETCH_அடுக்கு_உச்சி_SHIFT_UP:
    நிலை HB_OT_MATH_CONSTANT_SUBSCRIPT_BASELINE_DROP_MIN:
    நிலை HB_OT_MATH_CONSTANT_SUBSCRIPT_SHIFT_DOWN:
    நிலை HB_OT_MATH_CONSTANT_SUBSCRIPT_உச்சி_MAX:
    நிலை HB_OT_MATH_CONSTANT_SUB_SUPERSCRIPT_GAP_MIN:
    நிலை HB_OT_MATH_CONSTANT_SUPERSCRIPT_BASELINE_DROP_MAX:
    நிலை HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MAX_WITH_SUBSCRIPT:
    நிலை HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MIN:
    நிலை HB_OT_MATH_CONSTANT_SUPERSCRIPT_SHIFT_UP:
    நிலை HB_OT_MATH_CONSTANT_SUPERSCRIPT_SHIFT_UP_CRAMPED:
    நிலை HB_OT_MATH_CONSTANT_UNDERBAR_EXTRA_DESCENDER:
    நிலை HB_OT_MATH_CONSTANT_UNDERBAR_RULE_THICKNESS:
    நிலை HB_OT_MATH_CONSTANT_UNDERBAR_VERTICAL_GAP:
    நிலை HB_OT_MATH_CONSTANT_UPPER_LIMIT_BASELINE_RISE_MIN:
    நிலை HB_OT_MATH_CONSTANT_UPPER_LIMIT_GAP_MIN:
      திரும்பு math_மதிப்பு_Records[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_y_மதிப்பு (font, இது);

    நிலை HB_OT_MATH_CONSTANT_RADICAL_DEGREE_BOTTOM_RAISE_PERCENT:
      திரும்பு radicalDegreeBottomRaisePercent;

    இயல்பு:
      திரும்பு 0;
    }
  }

  பாதுகாக்கப்பட்ட:
  HBINT16 percentScaleDown[2];
  HBUINT16 minHeight[2];
  Math_மதிப்பு_Record math_மதிப்பு_Records[51];
  HBINT16 radicalDegreeBottomRaisePercent;

  பொது:
  DEFINE_SIZE_STATIC (214);
};

கட்டமைப்பு MathItalicsCorrectionInfo
{
  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    மாறிலி hb_set_t &glyphset = c->plan->_glyphset_mathed;
    மாறிலி hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (இது+coverage, italicsCorrection)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->italicsCorrection, இது, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (உண்மை);
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி
  {
    க்கு (மாறிலி auto &entry : hb_zip (இது+coverage, italicsCorrection))
      எனில் (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, இது);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  coverage.sanitize (c, இது) &&
		  italicsCorrection.sanitize (c, இது));
  }

  hb_position_t get_மதிப்பு (hb_codepoint_t glyph,
			   hb_font_t *font) மாறிலி
  {
    unsigned முழு index = (இது+coverage).get_coverage (glyph);
    திரும்பு italicsCorrection[index].get_x_மதிப்பு (font, இது);
  }

  பாதுகாக்கப்பட்ட:
  Offset16To<Coverage>       coverage;		/* Offset to Coverage table -
						 * from the beginning of
						 * MathItalicsCorrectionInfo
						 * table. */
  Array16Of<Math_மதிப்பு_Record> italicsCorrection;	/* Array of MathValueRecords
						 * defining italics correction
						 * values for each
						 * covered glyph. */

  பொது:
  DEFINE_SIZE_ARRAY (4, italicsCorrection);
};

கட்டமைப்பு Math_உச்சி_Accent_Attachment
{
  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    மாறிலி hb_set_t &glyphset = c->plan->_glyphset_mathed;
    மாறிலி hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (இது+உச்சி_Accent_Coverage, உச்சி_Accent_Attachment)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->உச்சி_Accent_Attachment, இது, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->உச்சி_Accent_Coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (உண்மை);
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி
  {
    க்கு (மாறிலி auto &entry : hb_zip (இது+உச்சி_Accent_Coverage, உச்சி_Accent_Attachment))
      எனில் (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, இது);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  உச்சி_Accent_Coverage.sanitize (c, இது) &&
		  உச்சி_Accent_Attachment.sanitize (c, இது));
  }

  hb_position_t get_மதிப்பு (hb_codepoint_t glyph,
			   hb_font_t *font) மாறிலி
  {
    unsigned முழு index = (இது+உச்சி_Accent_Coverage).get_coverage (glyph);
    எனில் (index == NOT_COVERED)
      திரும்பு font->get_glyph_h_advance (glyph) / 2;
    திரும்பு உச்சி_Accent_Attachment[index].get_x_மதிப்பு (font, இது);
  }

  பாதுகாக்கப்பட்ட:
  Offset16To<Coverage>       உச்சி_Accent_Coverage;   /* Offset to Coverage table -
						 * from the beginning of
						 * MathTopAccentAttachment
						 * table. */
  Array16Of<Math_மதிப்பு_Record> உச்சி_Accent_Attachment; /* Array of MathValueRecords
						 * defining top accent
						 * attachment points for each
						 * covered glyph. */

  பொது:
  DEFINE_SIZE_ARRAY (2 + 2, உச்சி_Accent_Attachment);
};

கட்டமைப்பு MathKern
{
  MathKern* copy (hb_serialize_context_t *c,
                      மாறிலி hb_hashmap_t<unsigned, hb_pair_t<unsigned, முழு>> *variation_map) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    auto *out = c->start_embed (இது);

    எனில் (unlikely (!c->embed (height_மொத்தம்))) return_trace (nullptr);

    unsigned மொத்தம் = 2 * height_மொத்தம் + 1;
    க்கு (unsigned i = 0; i < மொத்தம்; i++)
      எனில் (!c->copy (math_மதிப்பு_Records_Z.arrayZ[i], இது, variation_map))
        return_trace (nullptr);

    return_trace (out);
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி
  {
    க்கு (மாறிலி auto &record : math_மதிப்பு_Records_Z.as_array (2 * height_மொத்தம் + 1))
      record.collect_variation_indices (c, இது);
  }

  தருக்க sanitize_math_மதிப்பு_records (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    unsigned முழு மொத்தம் = 2 * height_மொத்தம் + 1;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
      எனில் (!math_மதிப்பு_Records_Z.arrayZ[i].sanitize (c, இது)) return_trace (பொய்);
    return_trace (உண்மை);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  hb_barrier () &&
		  c->check_array (math_மதிப்பு_Records_Z.arrayZ, 2 * height_மொத்தம் + 1) &&
		  sanitize_math_மதிப்பு_records (c));
  }

  hb_position_t get_மதிப்பு (hb_position_t correction_height, hb_font_t *font) மாறிலி
  {
    மாறிலி Math_மதிப்பு_Record* correctionHeight = math_மதிப்பு_Records_Z.arrayZ;
    மாறிலி Math_மதிப்பு_Record* kern_மதிப்பு = math_மதிப்பு_Records_Z.arrayZ + height_மொத்தம்;
    முழு sign = font->y_scale < 0 ? -1 : +1;

    /* According to OpenType spec (v1.9), except for the boundary cases, the index
     * chosen for kern value should be i such that
     *    correctionHeight[i-1] <= correction_height < correctionHeight[i]
     * We can use the binary search algorithm of std::upper_bound(). Or, we can
     * use the internal hb_bsearch_impl.
     */
    unsigned முழு pos;
    auto cmp = +[](மாறிலி வெறுமை* key, மாறிலி வெறுமை* p,
                   முழு sign, hb_font_t* font, மாறிலி MathKern* mathKern) -> முழு {
      hb_position_t a = *(hb_position_t *) key;
      hb_position_t b = ((Math_மதிப்பு_Record *) p)->get_y_மதிப்பு (font, mathKern);
      எனில் (a == b) திரும்பு 0;
      திரும்பு sign > 0 ? (a < b ? -1 : +1) : (b < a ? -1 : +1);
    };
    unsigned முழு i = hb_bsearch_impl(&pos, correction_height, correctionHeight,
                                     height_மொத்தம், Math_மதிப்பு_Record::static_size,
                                     cmp, sign, font, இது) ? pos + 1 : pos;
    திரும்பு kern_மதிப்பு[i].get_x_மதிப்பு (font, இது);
  }

  unsigned முழு get_entries (unsigned முழு start_offset,
			    unsigned முழு *entries_மொத்தம், /* IN/OUT */
			    hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			    hb_font_t *font) மாறிலி
  {
    மாறிலி Math_மதிப்பு_Record* correctionHeight = math_மதிப்பு_Records_Z.arrayZ;
    மாறிலி Math_மதிப்பு_Record* kern_மதிப்பு = math_மதிப்பு_Records_Z.arrayZ + height_மொத்தம்;
    மாறிலி unsigned முழு entries_மொத்தம்_2 = height_மொத்தம் + 1;

    எனில் (entries_மொத்தம் && kern_entries)
    {
      unsigned முழு start = hb_min (start_offset, entries_மொத்தம்_2);
      unsigned முழு end = hb_min (start + *entries_மொத்தம், entries_மொத்தம்_2);
      *entries_மொத்தம் = end - start;

      க்கு (unsigned முழு i = 0; i < *entries_மொத்தம்; i++) {
	unsigned முழு j = start + i;

	hb_position_t max_height;
	எனில் (j == height_மொத்தம்) {
	  max_height = INT32_MAX;
	} இல்லையெனில் {
	  max_height = correctionHeight[j].get_y_மதிப்பு (font, இது);
	}

	kern_entries[i] = {max_height, kern_மதிப்பு[j].get_x_மதிப்பு (font, இது)};
      }
    }
    திரும்பு entries_மொத்தம்_2;
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	height_மொத்தம்;
  UnsizedArrayOf<Math_மதிப்பு_Record>
		math_மதிப்பு_Records_Z;
				/* Array of correction heights at
				 * which the kern value changes.
				 * Sorted by the height value in
				 * design units (heightCount entries),
				 * Followed by:
				 * Array of kern values corresponding
				 * to heights. (heightCount+1 entries).
				 */

  பொது:
  DEFINE_SIZE_ARRAY (2, math_மதிப்பு_Records_Z);
};

கட்டமைப்பு MathKernInfoRecord
{
  MathKernInfoRecord* copy (hb_serialize_context_t *c, மாறிலி வெறுமை *base,
                            மாறிலி hb_hashmap_t<unsigned, hb_pair_t<unsigned, முழு>> *variation_map) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    auto *out = c->embed (இது);
    எனில் (unlikely (!out)) return_trace (nullptr);

    unsigned மொத்தம் = ARRAY_LENGTH (mathKern);
    க்கு (unsigned i = 0; i < மொத்தம்; i++)
      out->mathKern[i].serialize_copy (c, mathKern[i], base, 0, hb_serialize_context_t::Head, variation_map);

    return_trace (out);
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c, மாறிலி வெறுமை *base) மாறிலி
  {
    க்கு (மாறிலி auto &offset : mathKern)
      (base+offset).collect_variation_indices (c);
  }

  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SANITIZE (இது);

    unsigned முழு மொத்தம் = ARRAY_LENGTH (mathKern);
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
      எனில் (unlikely (!mathKern[i].sanitize (c, base)))
	return_trace (பொய்);

    return_trace (உண்மை);
  }

  hb_position_t get_kerning (hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font,
			     மாறிலி வெறுமை *base) மாறிலி
  {
    unsigned முழு idx = kern;
    எனில் (unlikely (idx >= ARRAY_LENGTH (mathKern))) திரும்பு 0;
    திரும்பு (base+mathKern[idx]).get_மதிப்பு (correction_height, font);
  }

  unsigned முழு get_kernings (hb_ot_math_kern_t kern,
			     unsigned முழு start_offset,
			     unsigned முழு *entries_மொத்தம், /* IN/OUT */
			     hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			     hb_font_t *font,
			     மாறிலி வெறுமை *base) மாறிலி
  {
    unsigned முழு idx = kern;
    எனில் (unlikely (idx >= ARRAY_LENGTH (mathKern)) || !mathKern[idx]) {
      எனில் (entries_மொத்தம்) *entries_மொத்தம் = 0;
      திரும்பு 0;
    }
    திரும்பு (base+mathKern[idx]).get_entries (start_offset,
					     entries_மொத்தம்,
					     kern_entries,
					     font);
  }

  பாதுகாக்கப்பட்ட:
  /* Offset to MathKern table for each corner -
   * from the beginning of MathKernInfo table.  May be NULL. */
  Offset16To<MathKern> mathKern[4];

  பொது:
  DEFINE_SIZE_STATIC (8);
};

கட்டமைப்பு MathKernInfo
{
  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    மாறிலி hb_set_t &glyphset = c->plan->_glyphset_mathed;
    மாறிலி hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (இது+mathKernCoverage, mathKernInfoRecords)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->mathKernInfoRecords, இது, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->mathKernCoverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (உண்மை);
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி
  {
    க்கு (மாறிலி auto &entry : hb_zip (இது+mathKernCoverage, mathKernInfoRecords))
      எனில் (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, இது);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  mathKernCoverage.sanitize (c, இது) &&
		  mathKernInfoRecords.sanitize (c, இது));
  }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font) மாறிலி
  {
    unsigned முழு index = (இது+mathKernCoverage).get_coverage (glyph);
    திரும்பு mathKernInfoRecords[index].get_kerning (kern, correction_height, font, இது);
  }

  unsigned முழு get_kernings (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     unsigned முழு start_offset,
			     unsigned முழு *entries_மொத்தம், /* IN/OUT */
			     hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			     hb_font_t *font) மாறிலி
  {
    unsigned முழு index = (இது+mathKernCoverage).get_coverage (glyph);
    திரும்பு mathKernInfoRecords[index].get_kernings (kern,
						    start_offset,
						    entries_மொத்தம்,
						    kern_entries,
						    font,
						    இது);
  }

  பாதுகாக்கப்பட்ட:
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

  பொது:
  DEFINE_SIZE_ARRAY (4, mathKernInfoRecords);
};

கட்டமைப்பு MathGlyphInfo
{
  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->embed (*இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    out->mathItalicsCorrectionInfo.serialize_subset (c, mathItalicsCorrectionInfo, இது);
    out->math_உச்சி_Accent_Attachment.serialize_subset (c, math_உச்சி_Accent_Attachment, இது);

    மாறிலி hb_set_t &glyphset = c->plan->_glyphset_mathed;
    மாறிலி hb_map_t &glyph_map = *c->plan->glyph_map;

    auto it =
    + hb_iter (இது+extendedShapeCoverage)
    | hb_take (c->plan->source->get_இலக்கம்_glyphs ())
    | hb_filter (glyphset)
    | hb_map_retains_sorting (glyph_map)
    ;

    எனில் (it) out->extendedShapeCoverage.serialize_serialize (c->serializer, it);
    இல்லையெனில் out->extendedShapeCoverage = 0;

    out->mathKernInfo.serialize_subset (c, mathKernInfo, இது);
    return_trace (உண்மை);
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி
  {
    (இது+mathItalicsCorrectionInfo).collect_variation_indices (c);
    (இது+math_உச்சி_Accent_Attachment).collect_variation_indices (c);
    (இது+mathKernInfo).collect_variation_indices (c);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  mathItalicsCorrectionInfo.sanitize (c, இது) &&
		  math_உச்சி_Accent_Attachment.sanitize (c, இது) &&
		  extendedShapeCoverage.sanitize (c, இது) &&
		  mathKernInfo.sanitize (c, இது));
  }

  hb_position_t
  get_italics_correction (hb_codepoint_t  glyph, hb_font_t *font) மாறிலி
  { திரும்பு (இது+mathItalicsCorrectionInfo).get_மதிப்பு (glyph, font); }

  hb_position_t
  get_உச்சி_accent_attachment (hb_codepoint_t  glyph, hb_font_t *font) மாறிலி
  { திரும்பு (இது+math_உச்சி_Accent_Attachment).get_மதிப்பு (glyph, font); }

  தருக்க is_extended_shape (hb_codepoint_t glyph) மாறிலி
  { திரும்பு (இது+extendedShapeCoverage).get_coverage (glyph) != NOT_COVERED; }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font) மாறிலி
  { திரும்பு (இது+mathKernInfo).get_kerning (glyph, kern, correction_height, font); }

  hb_position_t get_kernings (hb_codepoint_t glyph,
			      hb_ot_math_kern_t kern,
			      unsigned முழு start_offset,
			      unsigned முழு *entries_மொத்தம், /* IN/OUT */
			      hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			      hb_font_t *font) மாறிலி
  { திரும்பு (இது+mathKernInfo).get_kernings (glyph,
					     kern,
					     start_offset,
					     entries_மொத்தம்,
					     kern_entries,
					     font); }

  பாதுகாக்கப்பட்ட:
  /* Offset to MathItalicsCorrectionInfo table -
   * from the beginning of MathGlyphInfo table. */
  Offset16To<MathItalicsCorrectionInfo> mathItalicsCorrectionInfo;

  /* Offset to MathTopAccentAttachment table -
   * from the beginning of MathGlyphInfo table. */
  Offset16To<Math_உச்சி_Accent_Attachment> math_உச்சி_Accent_Attachment;

  /* Offset to coverage table for Extended Shape glyphs -
   * from the beginning of MathGlyphInfo table. When the left or right glyph of
   * a box is an extended shape variant, the (ink) box (and not the default
   * position defined by values in MathConstants table) should be used for
   * vertical positioning purposes.  May be NULL.. */
  Offset16To<Coverage> extendedShapeCoverage;

   /* Offset to MathKernInfo table -
    * from the beginning of MathGlyphInfo table. */
  Offset16To<MathKernInfo> mathKernInfo;

  பொது:
  DEFINE_SIZE_STATIC (8);
};

கட்டமைப்பு MathGlyphVariantRecord
{
  friend கட்டமைப்பு MathGlyphConstruction;

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->embed (இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    மாறிலி hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_assign (out->variantGlyph, glyph_map.get (variantGlyph), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  வெறுமை depend (hb_depend_data_builder_t *depend_data, unsigned source) மாறிலி
  { depend_data->add_depend(source, HB_OT_TAG_MATH, variantGlyph); }

  வெறுமை closure_glyphs (hb_set_t *variant_glyphs) மாறிலி
  { variant_glyphs->add (variantGlyph); }

  பாதுகாக்கப்பட்ட:
  HBGlyphID16 variantGlyph;       /* Glyph ID for the variant. */
  HBUINT16  advanceMeasurement; /* Advance width/height, in design units, of the
				 * variant, in the direction of requested
				 * glyph extension. */

  பொது:
  DEFINE_SIZE_STATIC (4);
};

கட்டமைப்பு PartFlags : HBUINT16
{
  எண்ணிக்கை Flags {
    Extender	= 0x0001u, /* If set, the part can be skipped or repeated. */

    Defined	= 0x0001u, /* All defined flags. */
  };

  பொது:
  DEFINE_SIZE_STATIC (2);
};

கட்டமைப்பு MathGlyphPartRecord
{
  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->embed (இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    மாறிலி hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_assign (out->glyph, glyph_map.get (glyph), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  வெறுமை extract (hb_ot_math_glyph_part_t &out,
		int64_t mult,
		hb_font_t *font) மாறிலி
  {
    out.glyph			= glyph;

    out.start_connector_length	= font->em_mult (startConnectorLength, mult);
    out.end_connector_length	= font->em_mult (endConnectorLength, mult);
    out.full_advance		= font->em_mult (fullAdvance, mult);

    static_assert ((unsigned முழு) HB_OT_MATH_GLYPH_PART_FLAG_EXTENDER ==
		   (unsigned முழு) PartFlags::Extender, "");

    out.flags = (hb_ot_math_glyph_part_flags_t)
		(unsigned முழு)
		(partFlags & PartFlags::Defined);
  }

  வெறுமை depend (hb_depend_data_builder_t *depend_data, unsigned source) மாறிலி
  { depend_data->add_depend(source, HB_OT_TAG_MATH, glyph); }

  வெறுமை closure_glyphs (hb_set_t *variant_glyphs) மாறிலி
  { variant_glyphs->add (glyph); }

  பாதுகாக்கப்பட்ட:
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

  பொது:
  DEFINE_SIZE_STATIC (10);
};

கட்டமைப்பு MathGlyphAssembly
{
  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);

    எனில் (!c->serializer->copy (italicsCorrection, இது, &c->plan->layout_variation_idx_delta_map)) return_trace (பொய்);
    எனில் (!c->serializer->copy<HBUINT16> (partRecords.len)) return_trace (பொய்);

    க்கு (மாறிலி auto& record : partRecords.iter ())
      எனில் (!record.subset (c)) return_trace (பொய்);
    return_trace (உண்மை);
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி
  {
    italicsCorrection.collect_variation_indices (c, இது);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  italicsCorrection.sanitize (c, இது) &&
		  partRecords.sanitize (c));
  }

  unsigned முழு get_parts (hb_direction_t direction,
			  hb_font_t *font,
			  unsigned முழு start_offset,
			  unsigned முழு *parts_மொத்தம், /* IN/OUT */
			  hb_ot_math_glyph_part_t *parts /* OUT */,
			  hb_position_t *italics_correction /* OUT */) மாறிலி
  {
    எனில் (parts_மொத்தம் && parts)
    {
      int64_t mult = font->dir_mult (direction);
      க்கு (auto _ : hb_zip (partRecords.as_array ().sub_array (start_offset, parts_மொத்தம்),
			    hb_array (parts, *parts_மொத்தம்)))
	_.first.extract (_.second, mult, font);
    }

    எனில் (italics_correction)
      *italics_correction = italicsCorrection.get_x_மதிப்பு (font, இது);

    திரும்பு partRecords.len;
  }

  வெறுமை depend (hb_depend_data_builder_t *depend_data, unsigned source) மாறிலி
  {
    க்கு (மாறிலி auto& _ : partRecords.iter ())
      _.depend (depend_data, source);
  }

  வெறுமை closure_glyphs (hb_set_t *variant_glyphs) மாறிலி
  {
    க்கு (மாறிலி auto& _ : partRecords.iter ())
      _.closure_glyphs (variant_glyphs);
  }

  பாதுகாக்கப்பட்ட:
  Math_மதிப்பு_Record
		italicsCorrection;
				/* Italics correction of this
				 * MathGlyphAssembly. Should not
				 * depend on the assembly size. */
  Array16Of<MathGlyphPartRecord>
		partRecords;	/* Array of part records, from
				 * left to right and bottom to
				 * top. */

  பொது:
  DEFINE_SIZE_ARRAY (6, partRecords);
};

கட்டமைப்பு MathGlyphConstruction
{
  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);

    out->glyphAssembly.serialize_subset (c, glyphAssembly, இது);

    எனில் (!c->serializer->check_assign (out->mathGlyphVariantRecord.len, mathGlyphVariantRecord.len, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (பொய்);
    க்கு (மாறிலி auto& record : mathGlyphVariantRecord.iter ())
      எனில் (!record.subset (c)) return_trace (பொய்);

    return_trace (உண்மை);
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி
  {
    (இது+glyphAssembly).collect_variation_indices (c);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  glyphAssembly.sanitize (c, இது) &&
		  mathGlyphVariantRecord.sanitize (c));
  }

  மாறிலி MathGlyphAssembly &get_assembly () மாறிலி { திரும்பு இது+glyphAssembly; }

  unsigned முழு get_variants (hb_direction_t direction,
			     hb_font_t *font,
			     unsigned முழு start_offset,
			     unsigned முழு *variants_மொத்தம், /* IN/OUT */
			     hb_ot_math_glyph_variant_t *variants /* OUT */) மாறிலி
  {
    எனில் (variants_மொத்தம் && variants)
    {
      int64_t mult = font->dir_mult (direction);
      க்கு (auto _ : hb_zip (mathGlyphVariantRecord.as_array ().sub_array (start_offset, variants_மொத்தம்),
			    hb_array (variants, *variants_மொத்தம்)))
	_.second = {_.first.variantGlyph, font->em_mult (_.first.advanceMeasurement, mult)};
    }
    திரும்பு mathGlyphVariantRecord.len;
  }

  வெறுமை depend (hb_depend_data_builder_t *depend_data, unsigned source) மாறிலி
  {
    (இது+glyphAssembly).depend (depend_data, source);

    க்கு (மாறிலி auto& _ : mathGlyphVariantRecord.iter ())
      _.depend (depend_data, source);
  }

  வெறுமை closure_glyphs (hb_set_t *variant_glyphs) மாறிலி
  {
    (இது+glyphAssembly).closure_glyphs (variant_glyphs);

    க்கு (மாறிலி auto& _ : mathGlyphVariantRecord.iter ())
      _.closure_glyphs (variant_glyphs);
  }

  பாதுகாக்கப்பட்ட:
  /* Offset to MathGlyphAssembly table for this shape - from the beginning of
     MathGlyphConstruction table.  May be NULL. */
  Offset16To<MathGlyphAssembly>	  glyphAssembly;

  /* MathGlyphVariantRecords for alternative variants of the glyphs. */
  Array16Of<MathGlyphVariantRecord> mathGlyphVariantRecord;

  பொது:
  DEFINE_SIZE_ARRAY (4, mathGlyphVariantRecord);
};

கட்டமைப்பு MathVariants
{
  வெறுமை depend (hb_depend_data_builder_t *depend_data) மாறிலி
  {
    மாறிலி hb_array_t<மாறிலி Offset16To<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vert_Glyph_மொத்தம் + horiz_Glyph_மொத்தம்);

    எனில் (vertGlyphCoverage)
    {
      மாறிலி auto vert_offsets = glyph_construction_offsets.sub_array (0, vert_Glyph_மொத்தம்);
      + hb_zip (இது+vertGlyphCoverage, vert_offsets)
      | hb_apply ([&] (மாறிலி hb_pair_t<hb_codepoint_t, மாறிலி Offset16To<MathGlyphConstruction>&> &_)
                  {
                    மாறிலி MathGlyphConstruction &mgc = இது+_.second;
                    mgc.depend (depend_data, _.first);
                  })
      ;
    }
    எனில் (horizGlyphCoverage)
    {
      மாறிலி auto hori_offsets = glyph_construction_offsets.sub_array (vert_Glyph_மொத்தம், horiz_Glyph_மொத்தம்);
      + hb_zip (இது+horizGlyphCoverage, hori_offsets)
      | hb_apply ([&] (மாறிலி hb_pair_t<hb_codepoint_t, மாறிலி Offset16To<MathGlyphConstruction>&> &_)
                  {
                    மாறிலி MathGlyphConstruction &mgc = இது+_.second;
                    mgc.depend (depend_data, _.first);
                  })
      ;
    }
  }

  வெறுமை closure_glyphs (மாறிலி hb_set_t *glyph_set,
                       hb_set_t *variant_glyphs) மாறிலி
  {
    மாறிலி hb_array_t<மாறிலி Offset16To<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vert_Glyph_மொத்தம் + horiz_Glyph_மொத்தம்);

    எனில் (vertGlyphCoverage)
    {
      மாறிலி auto vert_offsets = glyph_construction_offsets.sub_array (0, vert_Glyph_மொத்தம்);
      + hb_zip (இது+vertGlyphCoverage, vert_offsets)
      | hb_filter (glyph_set, hb_first)
      | hb_map (hb_second)
      | hb_map (hb_add (இது))
      | hb_apply ([=] (மாறிலி MathGlyphConstruction &_) { _.closure_glyphs (variant_glyphs); })
      ;
    }

    எனில் (horizGlyphCoverage)
    {
      மாறிலி auto hori_offsets = glyph_construction_offsets.sub_array (vert_Glyph_மொத்தம், horiz_Glyph_மொத்தம்);
      + hb_zip (இது+horizGlyphCoverage, hori_offsets)
      | hb_filter (glyph_set, hb_first)
      | hb_map (hb_second)
      | hb_map (hb_add (இது))
      | hb_apply ([=] (மாறிலி MathGlyphConstruction &_) { _.closure_glyphs (variant_glyphs); })
      ;
    }
  }

  வெறுமை collect_coverage_and_indices (hb_sorted_vector_t<hb_codepoint_t>& new_coverage,
                                     மாறிலி Offset16To<Coverage>& coverage,
                                     unsigned i,
                                     unsigned end_index,
                                     hb_set_t& indices,
                                     மாறிலி hb_set_t& glyphset,
                                     மாறிலி hb_map_t& glyph_map) மாறிலி
  {
    எனில் (!coverage) திரும்பு;

    க்கு (மாறிலி auto _ : (இது+coverage).iter ())
    {
      எனில் (i >= end_index) திரும்பு;
      எனில் (glyphset.has (_))
      {
        unsigned new_gid = glyph_map.get (_);
        new_coverage.push (new_gid);
        indices.add (i);
      }
      i++;
    }
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    மாறிலி hb_set_t &glyphset = c->plan->_glyphset_cmaped;
    மாறிலி hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);
    எனில் (!c->serializer->check_assign (out->minConnectorOverlap, minConnectorOverlap, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (பொய்);

    hb_sorted_vector_t<hb_codepoint_t> new_vert_coverage;
    hb_sorted_vector_t<hb_codepoint_t> new_hori_coverage;
    hb_set_t indices;
    collect_coverage_and_indices (new_vert_coverage, vertGlyphCoverage, 0, vert_Glyph_மொத்தம், indices, glyphset, glyph_map);
    collect_coverage_and_indices (new_hori_coverage, horizGlyphCoverage, vert_Glyph_மொத்தம், vert_Glyph_மொத்தம் + horiz_Glyph_மொத்தம், indices, glyphset, glyph_map);

    எனில் (!c->serializer->check_assign (out->vert_Glyph_மொத்தம், new_vert_coverage.length, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (பொய்);
    எனில் (!c->serializer->check_assign (out->horiz_Glyph_மொத்தம், new_hori_coverage.length, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (பொய்);

    க்கு (unsigned i : indices.iter ())
    {
      auto *o = c->serializer->embed (glyphConstruction[i]);
      எனில் (!o) return_trace (பொய்);
      o->serialize_subset (c, glyphConstruction[i], இது);
    }

    எனில் (new_vert_coverage)
      out->vertGlyphCoverage.serialize_serialize (c->serializer, new_vert_coverage.iter ());

    எனில் (new_hori_coverage)
    out->horizGlyphCoverage.serialize_serialize (c->serializer, new_hori_coverage.iter ());
    return_trace (உண்மை);
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி
  {
    க்கு (மாறிலி auto &entry : hb_zip (இது+vertGlyphCoverage, glyphConstruction.as_array (vert_Glyph_மொத்தம்)))
      எனில் (c->glyph_set->has (hb_first (entry)))
        (இது+hb_second (entry)).collect_variation_indices (c);
    க்கு (மாறிலி auto &entry : hb_zip (இது+horizGlyphCoverage, glyphConstruction.as_array (vert_Glyph_மொத்தம் + horiz_Glyph_மொத்தம்).sub_array (vert_Glyph_மொத்தம்)))
      எனில் (c->glyph_set->has (hb_first (entry)))
        (இது+hb_second (entry)).collect_variation_indices (c);
  }

  தருக்க sanitize_offsets (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    unsigned முழு மொத்தம் = vert_Glyph_மொத்தம் + horiz_Glyph_மொத்தம்;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
      எனில் (!glyphConstruction.arrayZ[i].sanitize (c, இது)) return_trace (பொய்);
    return_trace (உண்மை);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  vertGlyphCoverage.sanitize (c, இது) &&
		  horizGlyphCoverage.sanitize (c, இது) &&
		  hb_barrier () &&
		  c->check_array (glyphConstruction.arrayZ, vert_Glyph_மொத்தம் + horiz_Glyph_மொத்தம்) &&
		  sanitize_offsets (c));
  }

  hb_position_t get_min_connector_overlap (hb_direction_t direction,
						  hb_font_t *font) மாறிலி
  { திரும்பு font->em_scale_dir (minConnectorOverlap, direction); }

  unsigned முழு get_glyph_variants (hb_codepoint_t glyph,
				   hb_direction_t direction,
				   hb_font_t *font,
				   unsigned முழு start_offset,
				   unsigned முழு *variants_மொத்தம், /* IN/OUT */
				   hb_ot_math_glyph_variant_t *variants /* OUT */) மாறிலி
  { திரும்பு get_glyph_construction (glyph, direction, font)
	   .get_variants (direction, font, start_offset, variants_மொத்தம், variants); }

  unsigned முழு get_glyph_parts (hb_codepoint_t glyph,
				hb_direction_t direction,
				hb_font_t *font,
				unsigned முழு start_offset,
				unsigned முழு *parts_மொத்தம், /* IN/OUT */
				hb_ot_math_glyph_part_t *parts /* OUT */,
				hb_position_t *italics_correction /* OUT */) மாறிலி
  { திரும்பு get_glyph_construction (glyph, direction, font)
	   .get_assembly ()
	   .get_parts (direction, font,
		       start_offset, parts_மொத்தம், parts,
		       italics_correction); }

  தனிப்பட்ட:
  மாறிலி MathGlyphConstruction &
  get_glyph_construction (hb_codepoint_t glyph,
			  hb_direction_t direction,
			  hb_font_t *font HB_UNUSED) மாறிலி
  {
    தருக்க vertical = HB_DIRECTION_IS_VERTICAL (direction);
    unsigned முழு மொத்தம் = vertical ? vert_Glyph_மொத்தம் : horiz_Glyph_மொத்தம்;
    மாறிலி Offset16To<Coverage> &coverage = vertical ? vertGlyphCoverage
						  : horizGlyphCoverage;

    unsigned முழு index = (இது+coverage).get_coverage (glyph);
    எனில் (unlikely (index >= மொத்தம்)) திரும்பு Null (MathGlyphConstruction);

    எனில் (!vertical)
      index += vert_Glyph_மொத்தம்;

    திரும்பு இது+glyphConstruction[index];
  }

  பாதுகாக்கப்பட்ட:
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
  HBUINT16	vert_Glyph_மொத்தம்;	/* Number of glyphs for which
				 * information is provided for
				 * vertically growing variants. */
  HBUINT16	horiz_Glyph_மொத்தம்;/* Number of glyphs for which
				 * information is provided for
				 * horizontally growing variants. */

  /* Array of offsets to MathGlyphConstruction tables - from the beginning of
     the MathVariants table, for shapes growing in vertical/horizontal
     direction. */
  UnsizedArrayOf<Offset16To<MathGlyphConstruction>>
			glyphConstruction;

  பொது:
  DEFINE_SIZE_ARRAY (10, glyphConstruction);
};


/*
 * MATH -- Mathematical typesetting
 * https://docs.microsoft.com/en-us/typography/opentype/spec/math
 */

கட்டமைப்பு MATH
{
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_MATH;

  தருக்க has_data () மாறிலி { திரும்பு version.to_int (); }

  வெறுமை depend (hb_depend_data_builder_t *depend_data) மாறிலி
  {
    எனில் (mathVariants)
      (இது+mathVariants).depend (depend_data);
  }

  வெறுமை closure_glyphs (hb_set_t *glyph_set) மாறிலி
  {
    எனில் (mathVariants)
    {
      hb_set_t variant_glyphs;
      (இது+mathVariants).closure_glyphs (glyph_set, &variant_glyphs);
      hb_set_union (glyph_set, &variant_glyphs);
    }
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->embed (*இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    out->mathConstants.serialize_copy (c->serializer, mathConstants, இது, 0, hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map);
    out->mathGlyphInfo.serialize_subset (c, mathGlyphInfo, இது);
    out->mathVariants.serialize_subset (c, mathVariants, இது);
    return_trace (உண்மை);
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c, மாறிலி hb_set_t *variant_glyph_set) மாறிலி
  {
    (இது+mathConstants).collect_variation_indices (c);
    (இது+mathGlyphInfo).collect_variation_indices (c);
    auto *glyph_set = c->glyph_set;
    c->glyph_set = variant_glyph_set;
    (இது+mathVariants).collect_variation_indices (c);
    c->glyph_set = glyph_set;
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (version.sanitize (c) &&
		  likely (version.major == 1) &&
		  hb_barrier () &&
		  mathConstants.sanitize (c, இது) &&
		  mathGlyphInfo.sanitize (c, இது) &&
		  mathVariants.sanitize (c, இது));
  }

  // https://github.com/harfbuzz/harfbuzz/issues/4653
  HB_INTERNAL தருக்க is_bad_cambria (hb_font_t *font) மாறிலி
  {
#ifndef HB_NO_MATH
    தேர்வு HB_CODEPOINT_ENCODE3 (font->face->table.MATH.get_blob ()->length,
                                 (இது+mathConstants).minHeight[1], // displayOperatorMinHeight
                                 (இது+mathConstants).minHeight[0]) // delimitedSubFormulaMinHeight
    {
      /* sha1sum:ab4a4fe054d23061f3c039493d6f665cfda2ecf5  cambria.ttc
       * sha1sum:086855301bff644f9d8827b88491fcf73a6d4cb9  cambria.ttc
       * sha1sum:b1e5a3feaca2ea3dfcf79ccb377de749ecf60343  cambria.ttc */
      நிலை HB_CODEPOINT_ENCODE3 (25722, 2500, 3000):
        திரும்பு உண்மை;
    }
#endif
    திரும்பு பொய்;
  }

  hb_position_t get_constant (hb_ot_math_constant_t  constant,
			      hb_font_t		   *font) மாறிலி
  { திரும்பு (இது+mathConstants).get_மதிப்பு (constant, font); }

  மாறிலி MathGlyphInfo &get_glyph_info () மாறிலி { திரும்பு இது+mathGlyphInfo; }

  மாறிலி MathVariants &get_variants () மாறிலி    { திரும்பு இது+mathVariants; }

  பாதுகாக்கப்பட்ட:
  FixedVersion<>version;	/* Version of the MATH table
				 * initially set to 0x00010000u */
  Offset16To<MathConstants>
		mathConstants;	/* MathConstants table */
  Offset16To<MathGlyphInfo>
		mathGlyphInfo;	/* MathGlyphInfo table */
  Offset16To<MathVariants>
		mathVariants;	/* MathVariants table */

  பொது:
  DEFINE_SIZE_STATIC (10);
};

} /* namespace OT */


#endif /* HB_OT_MATH_TABLE_HH */

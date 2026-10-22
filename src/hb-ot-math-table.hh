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

#अन्तर्भाव "hb-open-type.hh"
#अन्तर्भाव "hb-ot-layout-common.hh"
#अन्तर्भाव "hb-ot-layout-gdef-table.hh"
#अन्तर्भाव "hb-ot-math.h"
#अन्तर्भाव "hb-depend-data.hh"

नामाकाश OT {


संरचना Math_मानम्_Record
{
  hb_position_t get_x_मानम् (hb_font_t *font, ध्रुव शून्य *base) ध्रुव
  { प्रत्यागम hb_saturate_add (font->em_scale_x (value), (base+deviceTable).get_x_delta (font, get_var_store (font, base))); }
  hb_position_t get_y_मानम् (hb_font_t *font, ध्रुव शून्य *base) ध्रुव
  { प्रत्यागम hb_saturate_add (font->em_scale_y (value), (base+deviceTable).get_y_delta (font, get_var_store (font, base))); }

  ध्रुव ItemVariationStore &get_var_store (hb_font_t *font, ध्रुव शून्य *base) ध्रुव
  {
#जइ !defined(HB_NO_VAR) && !defined(HB_NO_OT_LAYOUT)
    जइ ((base+deviceTable).is_variation_device ())
      प्रत्यागम font->face->table.GDEF->table->get_var_store ();
#endif
    प्रत्यागम Null (ItemVariationStore);
  }

  Math_मानम्_Record* copy (hb_serialize_context_t *c, ध्रुव शून्य *base,
                         ध्रुव hb_hashmap_t<unsigned, hb_pair_t<unsigned, पूर्णाङ्क>> *variation_map) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    auto *out = c->embed (एतत्);
    जइ (unlikely (!out)) return_trace (nullptr);
    hb_pair_t<unsigned, पूर्णाङ्क> *varidx_delta;
    जइ (variation_map->has ((base+deviceTable).get_variation_index (), &varidx_delta) &&
        !c->check_assign (out->value, (int64_t) value + hb_second (*varidx_delta),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (nullptr);
    out->deviceTable.serialize_copy (c, deviceTable, base, 0, hb_serialize_context_t::Head, variation_map);

    return_trace (out);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c, ध्रुव शून्य *base) ध्रुव
  { (base+deviceTable).collect_variation_indices (c); }

  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव शून्य *base) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && deviceTable.sanitize (c, base));
  }

  रक्षित:
  HBINT16		value;		/* The X or Y value in design units */
  Offset16To<Device>	deviceTable;	/* Offset to the device table - from the
					 * beginning of parent table.  May be NULL.
					 * Suggested format for device table is 1. */

  सार्वजनिक:
  DEFINE_SIZE_STATIC (4);
};

संरचना MathConstants
{
  friend संरचना MATH;

  MathConstants* copy (hb_serialize_context_t *c,
                      ध्रुव hb_hashmap_t<unsigned, hb_pair_t<unsigned, पूर्णाङ्क>> *variation_map) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    auto *out = c->start_embed (एतत्);

    HBINT16 *p = c->allocate_size<HBINT16> (HBINT16::static_size * 2);
    जइ (unlikely (!p)) return_trace (nullptr);
    hb_memcpy (p, percentScaleDown, HBINT16::static_size * 2);

    HBUINT16 *m = c->allocate_size<HBUINT16> (HBUINT16::static_size * 2);
    जइ (unlikely (!m)) return_trace (nullptr);
    hb_memcpy (m, minHeight, HBUINT16::static_size * 2);

    unsigned गणनम् = ARRAY_LENGTH (math_मानम्_Records);
    प्रति (unsigned i = 0; i < गणनम्; i++)
      जइ (!c->copy (math_मानम्_Records[i], एतत्, variation_map))
        return_trace (nullptr);

    जइ (!c->embed (radicalDegreeBottomRaisePercent)) return_trace (nullptr);
    return_trace (out);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव
  {
    प्रति (ध्रुव auto &record : math_मानम्_Records)
      record.collect_variation_indices (c, एतत्);
  }

  तार्किक sanitize_math_मानम्_records (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);

    unsigned पूर्णाङ्क गणनम् = ARRAY_LENGTH (math_मानम्_Records);
    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
      जइ (!math_मानम्_Records[i].sanitize (c, एतत्))
	return_trace (मिच्छ);

    return_trace (सच्च);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && sanitize_math_मानम्_records (c));
  }

  hb_position_t get_मानम् (hb_ot_math_constant_t constant,
			   hb_font_t *font) ध्रुव
  {
    विकल्प (constant) {

    पक्ष HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_DOWN:
    पक्ष HB_OT_MATH_CONSTANT_SCRIPT_SCRIPT_PERCENT_SCALE_DOWN:
      प्रत्यागम percentScaleDown[constant - HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_DOWN];

    पक्ष HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_HEIGHT:
    पक्ष HB_OT_MATH_CONSTANT_DISPLAY_OPERATOR_MIN_HEIGHT:
      प्रत्यागम font->em_scale_y (minHeight[constant - HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_HEIGHT]);

    पक्ष HB_OT_MATH_CONSTANT_RADICAL_KERN_AFTER_DEGREE:
    पक्ष HB_OT_MATH_CONSTANT_RADICAL_KERN_BEFORE_DEGREE:
    पक्ष HB_OT_MATH_CONSTANT_SKEWED_FRACTION_HORIZONTAL_GAP:
    पक्ष HB_OT_MATH_CONSTANT_SPACE_AFTER_SCRIPT:
      प्रत्यागम math_मानम्_Records[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_x_मानम् (font, एतत्);

    पक्ष HB_OT_MATH_CONSTANT_ACCENT_BASE_HEIGHT:
    पक्ष HB_OT_MATH_CONSTANT_AXIS_HEIGHT:
    पक्ष HB_OT_MATH_CONSTANT_FLATTENED_ACCENT_BASE_HEIGHT:
    पक्ष HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_DISPLAY_STYLE_SHIFT_DOWN:
    पक्ष HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_GAP_MIN:
    पक्ष HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_SHIFT_DOWN:
    पक्ष HB_OT_MATH_CONSTANT_FRACTION_DENOM_DISPLAY_STYLE_GAP_MIN:
    पक्ष HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_DISPLAY_STYLE_SHIFT_UP:
    पक्ष HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_GAP_MIN:
    पक्ष HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_SHIFT_UP:
    पक्ष HB_OT_MATH_CONSTANT_FRACTION_अङ्कः_DISPLAY_STYLE_GAP_MIN:
    पक्ष HB_OT_MATH_CONSTANT_FRACTION_RULE_THICKNESS:
    पक्ष HB_OT_MATH_CONSTANT_LOWER_LIMIT_BASELINE_DROP_MIN:
    पक्ष HB_OT_MATH_CONSTANT_LOWER_LIMIT_GAP_MIN:
    पक्ष HB_OT_MATH_CONSTANT_MATH_LEADING:
    पक्ष HB_OT_MATH_CONSTANT_OVERBAR_EXTRA_ASCENDER:
    पक्ष HB_OT_MATH_CONSTANT_OVERBAR_RULE_THICKNESS:
    पक्ष HB_OT_MATH_CONSTANT_OVERBAR_VERTICAL_GAP:
    पक्ष HB_OT_MATH_CONSTANT_RADICAL_DISPLAY_STYLE_VERTICAL_GAP:
    पक्ष HB_OT_MATH_CONSTANT_RADICAL_EXTRA_ASCENDER:
    पक्ष HB_OT_MATH_CONSTANT_RADICAL_RULE_THICKNESS:
    पक्ष HB_OT_MATH_CONSTANT_RADICAL_VERTICAL_GAP:
    पक्ष HB_OT_MATH_CONSTANT_SKEWED_FRACTION_VERTICAL_GAP:
    पक्ष HB_OT_MATH_CONSTANT_राशिः_BOTTOM_DISPLAY_STYLE_SHIFT_DOWN:
    पक्ष HB_OT_MATH_CONSTANT_राशिः_BOTTOM_SHIFT_DOWN:
    पक्ष HB_OT_MATH_CONSTANT_राशिः_DISPLAY_STYLE_GAP_MIN:
    पक्ष HB_OT_MATH_CONSTANT_राशिः_GAP_MIN:
    पक्ष HB_OT_MATH_CONSTANT_राशिः_शिखरम्_DISPLAY_STYLE_SHIFT_UP:
    पक्ष HB_OT_MATH_CONSTANT_राशिः_शिखरम्_SHIFT_UP:
    पक्ष HB_OT_MATH_CONSTANT_STRETCH_राशिः_BOTTOM_SHIFT_DOWN:
    पक्ष HB_OT_MATH_CONSTANT_STRETCH_राशिः_GAP_ABOVE_MIN:
    पक्ष HB_OT_MATH_CONSTANT_STRETCH_राशिः_GAP_BELOW_MIN:
    पक्ष HB_OT_MATH_CONSTANT_STRETCH_राशिः_शिखरम्_SHIFT_UP:
    पक्ष HB_OT_MATH_CONSTANT_SUBSCRIPT_BASELINE_DROP_MIN:
    पक्ष HB_OT_MATH_CONSTANT_SUBSCRIPT_SHIFT_DOWN:
    पक्ष HB_OT_MATH_CONSTANT_SUBSCRIPT_शिखरम्_MAX:
    पक्ष HB_OT_MATH_CONSTANT_SUB_SUPERSCRIPT_GAP_MIN:
    पक्ष HB_OT_MATH_CONSTANT_SUPERSCRIPT_BASELINE_DROP_MAX:
    पक्ष HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MAX_WITH_SUBSCRIPT:
    पक्ष HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MIN:
    पक्ष HB_OT_MATH_CONSTANT_SUPERSCRIPT_SHIFT_UP:
    पक्ष HB_OT_MATH_CONSTANT_SUPERSCRIPT_SHIFT_UP_CRAMPED:
    पक्ष HB_OT_MATH_CONSTANT_UNDERBAR_EXTRA_DESCENDER:
    पक्ष HB_OT_MATH_CONSTANT_UNDERBAR_RULE_THICKNESS:
    पक्ष HB_OT_MATH_CONSTANT_UNDERBAR_VERTICAL_GAP:
    पक्ष HB_OT_MATH_CONSTANT_UPPER_LIMIT_BASELINE_RISE_MIN:
    पक्ष HB_OT_MATH_CONSTANT_UPPER_LIMIT_GAP_MIN:
      प्रत्यागम math_मानम्_Records[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_y_मानम् (font, एतत्);

    पक्ष HB_OT_MATH_CONSTANT_RADICAL_DEGREE_BOTTOM_RAISE_PERCENT:
      प्रत्यागम radicalDegreeBottomRaisePercent;

    स्वभाव:
      प्रत्यागम 0;
    }
  }

  रक्षित:
  HBINT16 percentScaleDown[2];
  HBUINT16 minHeight[2];
  Math_मानम्_Record math_मानम्_Records[51];
  HBINT16 radicalDegreeBottomRaisePercent;

  सार्वजनिक:
  DEFINE_SIZE_STATIC (214);
};

संरचना MathItalicsCorrectionInfo
{
  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    ध्रुव hb_set_t &glyphset = c->plan->_glyphset_mathed;
    ध्रुव hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*एतत्);
    जइ (unlikely (!c->serializer->extend_min (out))) return_trace (मिच्छ);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (एतत्+coverage, italicsCorrection)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->italicsCorrection, एतत्, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (सच्च);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव
  {
    प्रति (ध्रुव auto &entry : hb_zip (एतत्+coverage, italicsCorrection))
      जइ (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, एतत्);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  coverage.sanitize (c, एतत्) &&
		  italicsCorrection.sanitize (c, एतत्));
  }

  hb_position_t get_मानम् (hb_codepoint_t glyph,
			   hb_font_t *font) ध्रुव
  {
    unsigned पूर्णाङ्क index = (एतत्+coverage).get_coverage (glyph);
    प्रत्यागम italicsCorrection[index].get_x_मानम् (font, एतत्);
  }

  रक्षित:
  Offset16To<Coverage>       coverage;		/* Offset to Coverage table -
						 * from the beginning of
						 * MathItalicsCorrectionInfo
						 * table. */
  Array16Of<Math_मानम्_Record> italicsCorrection;	/* Array of MathValueRecords
						 * defining italics correction
						 * values for each
						 * covered glyph. */

  सार्वजनिक:
  DEFINE_SIZE_ARRAY (4, italicsCorrection);
};

संरचना Math_शिखरम्_Accent_Attachment
{
  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    ध्रुव hb_set_t &glyphset = c->plan->_glyphset_mathed;
    ध्रुव hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*एतत्);
    जइ (unlikely (!c->serializer->extend_min (out))) return_trace (मिच्छ);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (एतत्+शिखरम्_Accent_Coverage, शिखरम्_Accent_Attachment)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->शिखरम्_Accent_Attachment, एतत्, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->शिखरम्_Accent_Coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (सच्च);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव
  {
    प्रति (ध्रुव auto &entry : hb_zip (एतत्+शिखरम्_Accent_Coverage, शिखरम्_Accent_Attachment))
      जइ (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, एतत्);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  शिखरम्_Accent_Coverage.sanitize (c, एतत्) &&
		  शिखरम्_Accent_Attachment.sanitize (c, एतत्));
  }

  hb_position_t get_मानम् (hb_codepoint_t glyph,
			   hb_font_t *font) ध्रुव
  {
    unsigned पूर्णाङ्क index = (एतत्+शिखरम्_Accent_Coverage).get_coverage (glyph);
    जइ (index == NOT_COVERED)
      प्रत्यागम font->get_glyph_h_advance (glyph) / 2;
    प्रत्यागम शिखरम्_Accent_Attachment[index].get_x_मानम् (font, एतत्);
  }

  रक्षित:
  Offset16To<Coverage>       शिखरम्_Accent_Coverage;   /* Offset to Coverage table -
						 * from the beginning of
						 * MathTopAccentAttachment
						 * table. */
  Array16Of<Math_मानम्_Record> शिखरम्_Accent_Attachment; /* Array of MathValueRecords
						 * defining top accent
						 * attachment points for each
						 * covered glyph. */

  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2 + 2, शिखरम्_Accent_Attachment);
};

संरचना MathKern
{
  MathKern* copy (hb_serialize_context_t *c,
                      ध्रुव hb_hashmap_t<unsigned, hb_pair_t<unsigned, पूर्णाङ्क>> *variation_map) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    auto *out = c->start_embed (एतत्);

    जइ (unlikely (!c->embed (height_गणनम्))) return_trace (nullptr);

    unsigned गणनम् = 2 * height_गणनम् + 1;
    प्रति (unsigned i = 0; i < गणनम्; i++)
      जइ (!c->copy (math_मानम्_Records_Z.arrayZ[i], एतत्, variation_map))
        return_trace (nullptr);

    return_trace (out);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव
  {
    प्रति (ध्रुव auto &record : math_मानम्_Records_Z.as_array (2 * height_गणनम् + 1))
      record.collect_variation_indices (c, एतत्);
  }

  तार्किक sanitize_math_मानम्_records (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    unsigned पूर्णाङ्क गणनम् = 2 * height_गणनम् + 1;
    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
      जइ (!math_मानम्_Records_Z.arrayZ[i].sanitize (c, एतत्)) return_trace (मिच्छ);
    return_trace (सच्च);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  c->check_array (math_मानम्_Records_Z.arrayZ, 2 * height_गणनम् + 1) &&
		  sanitize_math_मानम्_records (c));
  }

  hb_position_t get_मानम् (hb_position_t correction_height, hb_font_t *font) ध्रुव
  {
    ध्रुव Math_मानम्_Record* correctionHeight = math_मानम्_Records_Z.arrayZ;
    ध्रुव Math_मानम्_Record* kern_मानम् = math_मानम्_Records_Z.arrayZ + height_गणनम्;
    पूर्णाङ्क sign = font->y_scale < 0 ? -1 : +1;

    /* According to OpenType spec (v1.9), except for the boundary cases, the index
     * chosen for kern value should be i such that
     *    correctionHeight[i-1] <= correction_height < correctionHeight[i]
     * We can use the binary search algorithm of std::upper_bound(). Or, we can
     * use the internal hb_bsearch_impl.
     */
    unsigned पूर्णाङ्क pos;
    auto cmp = +[](ध्रुव शून्य* key, ध्रुव शून्य* p,
                   पूर्णाङ्क sign, hb_font_t* font, ध्रुव MathKern* mathKern) -> पूर्णाङ्क {
      hb_position_t a = *(hb_position_t *) key;
      hb_position_t b = ((Math_मानम्_Record *) p)->get_y_मानम् (font, mathKern);
      जइ (a == b) प्रत्यागम 0;
      प्रत्यागम sign > 0 ? (a < b ? -1 : +1) : (b < a ? -1 : +1);
    };
    unsigned पूर्णाङ्क i = hb_bsearch_impl(&pos, correction_height, correctionHeight,
                                     height_गणनम्, Math_मानम्_Record::static_size,
                                     cmp, sign, font, एतत्) ? pos + 1 : pos;
    प्रत्यागम kern_मानम्[i].get_x_मानम् (font, एतत्);
  }

  unsigned पूर्णाङ्क get_entries (unsigned पूर्णाङ्क start_offset,
			    unsigned पूर्णाङ्क *entries_गणनम्, /* IN/OUT */
			    hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			    hb_font_t *font) ध्रुव
  {
    ध्रुव Math_मानम्_Record* correctionHeight = math_मानम्_Records_Z.arrayZ;
    ध्रुव Math_मानम्_Record* kern_मानम् = math_मानम्_Records_Z.arrayZ + height_गणनम्;
    ध्रुव unsigned पूर्णाङ्क entries_गणनम्_2 = height_गणनम् + 1;

    जइ (entries_गणनम् && kern_entries)
    {
      unsigned पूर्णाङ्क start = hb_min (start_offset, entries_गणनम्_2);
      unsigned पूर्णाङ्क end = hb_min (start + *entries_गणनम्, entries_गणनम्_2);
      *entries_गणनम् = end - start;

      प्रति (unsigned पूर्णाङ्क i = 0; i < *entries_गणनम्; i++) {
	unsigned पूर्णाङ्क j = start + i;

	hb_position_t max_height;
	जइ (j == height_गणनम्) {
	  max_height = INT32_MAX;
	} अण्णहा {
	  max_height = correctionHeight[j].get_y_मानम् (font, एतत्);
	}

	kern_entries[i] = {max_height, kern_मानम्[j].get_x_मानम् (font, एतत्)};
      }
    }
    प्रत्यागम entries_गणनम्_2;
  }

  रक्षित:
  HBUINT16	height_गणनम्;
  UnsizedArrayOf<Math_मानम्_Record>
		math_मानम्_Records_Z;
				/* Array of correction heights at
				 * which the kern value changes.
				 * Sorted by the height value in
				 * design units (heightCount entries),
				 * Followed by:
				 * Array of kern values corresponding
				 * to heights. (heightCount+1 entries).
				 */

  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2, math_मानम्_Records_Z);
};

संरचना MathKernInfoRecord
{
  MathKernInfoRecord* copy (hb_serialize_context_t *c, ध्रुव शून्य *base,
                            ध्रुव hb_hashmap_t<unsigned, hb_pair_t<unsigned, पूर्णाङ्क>> *variation_map) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    auto *out = c->embed (एतत्);
    जइ (unlikely (!out)) return_trace (nullptr);

    unsigned गणनम् = ARRAY_LENGTH (mathKern);
    प्रति (unsigned i = 0; i < गणनम्; i++)
      out->mathKern[i].serialize_copy (c, mathKern[i], base, 0, hb_serialize_context_t::Head, variation_map);

    return_trace (out);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c, ध्रुव शून्य *base) ध्रुव
  {
    प्रति (ध्रुव auto &offset : mathKern)
      (base+offset).collect_variation_indices (c);
  }

  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव शून्य *base) ध्रुव
  {
    TRACE_SANITIZE (एतत्);

    unsigned पूर्णाङ्क गणनम् = ARRAY_LENGTH (mathKern);
    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
      जइ (unlikely (!mathKern[i].sanitize (c, base)))
	return_trace (मिच्छ);

    return_trace (सच्च);
  }

  hb_position_t get_kerning (hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font,
			     ध्रुव शून्य *base) ध्रुव
  {
    unsigned पूर्णाङ्क idx = kern;
    जइ (unlikely (idx >= ARRAY_LENGTH (mathKern))) प्रत्यागम 0;
    प्रत्यागम (base+mathKern[idx]).get_मानम् (correction_height, font);
  }

  unsigned पूर्णाङ्क get_kernings (hb_ot_math_kern_t kern,
			     unsigned पूर्णाङ्क start_offset,
			     unsigned पूर्णाङ्क *entries_गणनम्, /* IN/OUT */
			     hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			     hb_font_t *font,
			     ध्रुव शून्य *base) ध्रुव
  {
    unsigned पूर्णाङ्क idx = kern;
    जइ (unlikely (idx >= ARRAY_LENGTH (mathKern)) || !mathKern[idx]) {
      जइ (entries_गणनम्) *entries_गणनम् = 0;
      प्रत्यागम 0;
    }
    प्रत्यागम (base+mathKern[idx]).get_entries (start_offset,
					     entries_गणनम्,
					     kern_entries,
					     font);
  }

  रक्षित:
  /* Offset to MathKern table for each corner -
   * from the beginning of MathKernInfo table.  May be NULL. */
  Offset16To<MathKern> mathKern[4];

  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

संरचना MathKernInfo
{
  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    ध्रुव hb_set_t &glyphset = c->plan->_glyphset_mathed;
    ध्रुव hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*एतत्);
    जइ (unlikely (!c->serializer->extend_min (out))) return_trace (मिच्छ);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (एतत्+mathKernCoverage, mathKernInfoRecords)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->mathKernInfoRecords, एतत्, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->mathKernCoverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (सच्च);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव
  {
    प्रति (ध्रुव auto &entry : hb_zip (एतत्+mathKernCoverage, mathKernInfoRecords))
      जइ (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, एतत्);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  mathKernCoverage.sanitize (c, एतत्) &&
		  mathKernInfoRecords.sanitize (c, एतत्));
  }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font) ध्रुव
  {
    unsigned पूर्णाङ्क index = (एतत्+mathKernCoverage).get_coverage (glyph);
    प्रत्यागम mathKernInfoRecords[index].get_kerning (kern, correction_height, font, एतत्);
  }

  unsigned पूर्णाङ्क get_kernings (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     unsigned पूर्णाङ्क start_offset,
			     unsigned पूर्णाङ्क *entries_गणनम्, /* IN/OUT */
			     hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			     hb_font_t *font) ध्रुव
  {
    unsigned पूर्णाङ्क index = (एतत्+mathKernCoverage).get_coverage (glyph);
    प्रत्यागम mathKernInfoRecords[index].get_kernings (kern,
						    start_offset,
						    entries_गणनम्,
						    kern_entries,
						    font,
						    एतत्);
  }

  रक्षित:
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

  सार्वजनिक:
  DEFINE_SIZE_ARRAY (4, mathKernInfoRecords);
};

संरचना MathGlyphInfo
{
  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->embed (*एतत्);
    जइ (unlikely (!out)) return_trace (मिच्छ);

    out->mathItalicsCorrectionInfo.serialize_subset (c, mathItalicsCorrectionInfo, एतत्);
    out->math_शिखरम्_Accent_Attachment.serialize_subset (c, math_शिखरम्_Accent_Attachment, एतत्);

    ध्रुव hb_set_t &glyphset = c->plan->_glyphset_mathed;
    ध्रुव hb_map_t &glyph_map = *c->plan->glyph_map;

    auto it =
    + hb_iter (एतत्+extendedShapeCoverage)
    | hb_take (c->plan->source->get_अङ्कः_glyphs ())
    | hb_filter (glyphset)
    | hb_map_retains_sorting (glyph_map)
    ;

    जइ (it) out->extendedShapeCoverage.serialize_serialize (c->serializer, it);
    अण्णहा out->extendedShapeCoverage = 0;

    out->mathKernInfo.serialize_subset (c, mathKernInfo, एतत्);
    return_trace (सच्च);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव
  {
    (एतत्+mathItalicsCorrectionInfo).collect_variation_indices (c);
    (एतत्+math_शिखरम्_Accent_Attachment).collect_variation_indices (c);
    (एतत्+mathKernInfo).collect_variation_indices (c);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  mathItalicsCorrectionInfo.sanitize (c, एतत्) &&
		  math_शिखरम्_Accent_Attachment.sanitize (c, एतत्) &&
		  extendedShapeCoverage.sanitize (c, एतत्) &&
		  mathKernInfo.sanitize (c, एतत्));
  }

  hb_position_t
  get_italics_correction (hb_codepoint_t  glyph, hb_font_t *font) ध्रुव
  { प्रत्यागम (एतत्+mathItalicsCorrectionInfo).get_मानम् (glyph, font); }

  hb_position_t
  get_शिखरम्_accent_attachment (hb_codepoint_t  glyph, hb_font_t *font) ध्रुव
  { प्रत्यागम (एतत्+math_शिखरम्_Accent_Attachment).get_मानम् (glyph, font); }

  तार्किक is_extended_shape (hb_codepoint_t glyph) ध्रुव
  { प्रत्यागम (एतत्+extendedShapeCoverage).get_coverage (glyph) != NOT_COVERED; }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font) ध्रुव
  { प्रत्यागम (एतत्+mathKernInfo).get_kerning (glyph, kern, correction_height, font); }

  hb_position_t get_kernings (hb_codepoint_t glyph,
			      hb_ot_math_kern_t kern,
			      unsigned पूर्णाङ्क start_offset,
			      unsigned पूर्णाङ्क *entries_गणनम्, /* IN/OUT */
			      hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			      hb_font_t *font) ध्रुव
  { प्रत्यागम (एतत्+mathKernInfo).get_kernings (glyph,
					     kern,
					     start_offset,
					     entries_गणनम्,
					     kern_entries,
					     font); }

  रक्षित:
  /* Offset to MathItalicsCorrectionInfo table -
   * from the beginning of MathGlyphInfo table. */
  Offset16To<MathItalicsCorrectionInfo> mathItalicsCorrectionInfo;

  /* Offset to MathTopAccentAttachment table -
   * from the beginning of MathGlyphInfo table. */
  Offset16To<Math_शिखरम्_Accent_Attachment> math_शिखरम्_Accent_Attachment;

  /* Offset to coverage table for Extended Shape glyphs -
   * from the beginning of MathGlyphInfo table. When the left or right glyph of
   * a box is an extended shape variant, the (ink) box (and not the default
   * position defined by values in MathConstants table) should be used for
   * vertical positioning purposes.  May be NULL.. */
  Offset16To<Coverage> extendedShapeCoverage;

   /* Offset to MathKernInfo table -
    * from the beginning of MathGlyphInfo table. */
  Offset16To<MathKernInfo> mathKernInfo;

  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

संरचना MathGlyphVariantRecord
{
  friend संरचना MathGlyphConstruction;

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->embed (एतत्);
    जइ (unlikely (!out)) return_trace (मिच्छ);

    ध्रुव hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_assign (out->variantGlyph, glyph_map.get (variantGlyph), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  शून्य depend (hb_depend_data_builder_t *depend_data, unsigned source) ध्रुव
  { depend_data->add_depend(source, HB_OT_TAG_MATH, variantGlyph); }

  शून्य closure_glyphs (hb_set_t *variant_glyphs) ध्रुव
  { variant_glyphs->add (variantGlyph); }

  रक्षित:
  HBGlyphID16 variantGlyph;       /* Glyph ID for the variant. */
  HBUINT16  advanceMeasurement; /* Advance width/height, in design units, of the
				 * variant, in the direction of requested
				 * glyph extension. */

  सार्वजनिक:
  DEFINE_SIZE_STATIC (4);
};

संरचना PartFlags : HBUINT16
{
  गणना Flags {
    Extender	= 0x0001u, /* If set, the part can be skipped or repeated. */

    Defined	= 0x0001u, /* All defined flags. */
  };

  सार्वजनिक:
  DEFINE_SIZE_STATIC (2);
};

संरचना MathGlyphPartRecord
{
  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->embed (एतत्);
    जइ (unlikely (!out)) return_trace (मिच्छ);

    ध्रुव hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_assign (out->glyph, glyph_map.get (glyph), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  शून्य extract (hb_ot_math_glyph_part_t &out,
		int64_t mult,
		hb_font_t *font) ध्रुव
  {
    out.glyph			= glyph;

    out.start_connector_length	= font->em_mult (startConnectorLength, mult);
    out.end_connector_length	= font->em_mult (endConnectorLength, mult);
    out.full_advance		= font->em_mult (fullAdvance, mult);

    static_assert ((unsigned पूर्णाङ्क) HB_OT_MATH_GLYPH_PART_FLAG_EXTENDER ==
		   (unsigned पूर्णाङ्क) PartFlags::Extender, "");

    out.flags = (hb_ot_math_glyph_part_flags_t)
		(unsigned पूर्णाङ्क)
		(partFlags & PartFlags::Defined);
  }

  शून्य depend (hb_depend_data_builder_t *depend_data, unsigned source) ध्रुव
  { depend_data->add_depend(source, HB_OT_TAG_MATH, glyph); }

  शून्य closure_glyphs (hb_set_t *variant_glyphs) ध्रुव
  { variant_glyphs->add (glyph); }

  रक्षित:
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

  सार्वजनिक:
  DEFINE_SIZE_STATIC (10);
};

संरचना MathGlyphAssembly
{
  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);

    जइ (!c->serializer->copy (italicsCorrection, एतत्, &c->plan->layout_variation_idx_delta_map)) return_trace (मिच्छ);
    जइ (!c->serializer->copy<HBUINT16> (partRecords.len)) return_trace (मिच्छ);

    प्रति (ध्रुव auto& record : partRecords.iter ())
      जइ (!record.subset (c)) return_trace (मिच्छ);
    return_trace (सच्च);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव
  {
    italicsCorrection.collect_variation_indices (c, एतत्);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  italicsCorrection.sanitize (c, एतत्) &&
		  partRecords.sanitize (c));
  }

  unsigned पूर्णाङ्क get_parts (hb_direction_t direction,
			  hb_font_t *font,
			  unsigned पूर्णाङ्क start_offset,
			  unsigned पूर्णाङ्क *parts_गणनम्, /* IN/OUT */
			  hb_ot_math_glyph_part_t *parts /* OUT */,
			  hb_position_t *italics_correction /* OUT */) ध्रुव
  {
    जइ (parts_गणनम् && parts)
    {
      int64_t mult = font->dir_mult (direction);
      प्रति (auto _ : hb_zip (partRecords.as_array ().sub_array (start_offset, parts_गणनम्),
			    hb_array (parts, *parts_गणनम्)))
	_.first.extract (_.second, mult, font);
    }

    जइ (italics_correction)
      *italics_correction = italicsCorrection.get_x_मानम् (font, एतत्);

    प्रत्यागम partRecords.len;
  }

  शून्य depend (hb_depend_data_builder_t *depend_data, unsigned source) ध्रुव
  {
    प्रति (ध्रुव auto& _ : partRecords.iter ())
      _.depend (depend_data, source);
  }

  शून्य closure_glyphs (hb_set_t *variant_glyphs) ध्रुव
  {
    प्रति (ध्रुव auto& _ : partRecords.iter ())
      _.closure_glyphs (variant_glyphs);
  }

  रक्षित:
  Math_मानम्_Record
		italicsCorrection;
				/* Italics correction of this
				 * MathGlyphAssembly. Should not
				 * depend on the assembly size. */
  Array16Of<MathGlyphPartRecord>
		partRecords;	/* Array of part records, from
				 * left to right and bottom to
				 * top. */

  सार्वजनिक:
  DEFINE_SIZE_ARRAY (6, partRecords);
};

संरचना MathGlyphConstruction
{
  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->start_embed (*एतत्);
    जइ (unlikely (!c->serializer->extend_min (out))) return_trace (मिच्छ);

    out->glyphAssembly.serialize_subset (c, glyphAssembly, एतत्);

    जइ (!c->serializer->check_assign (out->mathGlyphVariantRecord.len, mathGlyphVariantRecord.len, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (मिच्छ);
    प्रति (ध्रुव auto& record : mathGlyphVariantRecord.iter ())
      जइ (!record.subset (c)) return_trace (मिच्छ);

    return_trace (सच्च);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव
  {
    (एतत्+glyphAssembly).collect_variation_indices (c);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  glyphAssembly.sanitize (c, एतत्) &&
		  mathGlyphVariantRecord.sanitize (c));
  }

  ध्रुव MathGlyphAssembly &get_assembly () ध्रुव { प्रत्यागम एतत्+glyphAssembly; }

  unsigned पूर्णाङ्क get_variants (hb_direction_t direction,
			     hb_font_t *font,
			     unsigned पूर्णाङ्क start_offset,
			     unsigned पूर्णाङ्क *variants_गणनम्, /* IN/OUT */
			     hb_ot_math_glyph_variant_t *variants /* OUT */) ध्रुव
  {
    जइ (variants_गणनम् && variants)
    {
      int64_t mult = font->dir_mult (direction);
      प्रति (auto _ : hb_zip (mathGlyphVariantRecord.as_array ().sub_array (start_offset, variants_गणनम्),
			    hb_array (variants, *variants_गणनम्)))
	_.second = {_.first.variantGlyph, font->em_mult (_.first.advanceMeasurement, mult)};
    }
    प्रत्यागम mathGlyphVariantRecord.len;
  }

  शून्य depend (hb_depend_data_builder_t *depend_data, unsigned source) ध्रुव
  {
    (एतत्+glyphAssembly).depend (depend_data, source);

    प्रति (ध्रुव auto& _ : mathGlyphVariantRecord.iter ())
      _.depend (depend_data, source);
  }

  शून्य closure_glyphs (hb_set_t *variant_glyphs) ध्रुव
  {
    (एतत्+glyphAssembly).closure_glyphs (variant_glyphs);

    प्रति (ध्रुव auto& _ : mathGlyphVariantRecord.iter ())
      _.closure_glyphs (variant_glyphs);
  }

  रक्षित:
  /* Offset to MathGlyphAssembly table for this shape - from the beginning of
     MathGlyphConstruction table.  May be NULL. */
  Offset16To<MathGlyphAssembly>	  glyphAssembly;

  /* MathGlyphVariantRecords for alternative variants of the glyphs. */
  Array16Of<MathGlyphVariantRecord> mathGlyphVariantRecord;

  सार्वजनिक:
  DEFINE_SIZE_ARRAY (4, mathGlyphVariantRecord);
};

संरचना MathVariants
{
  शून्य depend (hb_depend_data_builder_t *depend_data) ध्रुव
  {
    ध्रुव hb_array_t<ध्रुव Offset16To<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vert_Glyph_गणनम् + horiz_Glyph_गणनम्);

    जइ (vertGlyphCoverage)
    {
      ध्रुव auto vert_offsets = glyph_construction_offsets.sub_array (0, vert_Glyph_गणनम्);
      + hb_zip (एतत्+vertGlyphCoverage, vert_offsets)
      | hb_apply ([&] (ध्रुव hb_pair_t<hb_codepoint_t, ध्रुव Offset16To<MathGlyphConstruction>&> &_)
                  {
                    ध्रुव MathGlyphConstruction &mgc = एतत्+_.second;
                    mgc.depend (depend_data, _.first);
                  })
      ;
    }
    जइ (horizGlyphCoverage)
    {
      ध्रुव auto hori_offsets = glyph_construction_offsets.sub_array (vert_Glyph_गणनम्, horiz_Glyph_गणनम्);
      + hb_zip (एतत्+horizGlyphCoverage, hori_offsets)
      | hb_apply ([&] (ध्रुव hb_pair_t<hb_codepoint_t, ध्रुव Offset16To<MathGlyphConstruction>&> &_)
                  {
                    ध्रुव MathGlyphConstruction &mgc = एतत्+_.second;
                    mgc.depend (depend_data, _.first);
                  })
      ;
    }
  }

  शून्य closure_glyphs (ध्रुव hb_set_t *glyph_set,
                       hb_set_t *variant_glyphs) ध्रुव
  {
    ध्रुव hb_array_t<ध्रुव Offset16To<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vert_Glyph_गणनम् + horiz_Glyph_गणनम्);

    जइ (vertGlyphCoverage)
    {
      ध्रुव auto vert_offsets = glyph_construction_offsets.sub_array (0, vert_Glyph_गणनम्);
      + hb_zip (एतत्+vertGlyphCoverage, vert_offsets)
      | hb_filter (glyph_set, hb_first)
      | hb_map (hb_second)
      | hb_map (hb_add (एतत्))
      | hb_apply ([=] (ध्रुव MathGlyphConstruction &_) { _.closure_glyphs (variant_glyphs); })
      ;
    }

    जइ (horizGlyphCoverage)
    {
      ध्रुव auto hori_offsets = glyph_construction_offsets.sub_array (vert_Glyph_गणनम्, horiz_Glyph_गणनम्);
      + hb_zip (एतत्+horizGlyphCoverage, hori_offsets)
      | hb_filter (glyph_set, hb_first)
      | hb_map (hb_second)
      | hb_map (hb_add (एतत्))
      | hb_apply ([=] (ध्रुव MathGlyphConstruction &_) { _.closure_glyphs (variant_glyphs); })
      ;
    }
  }

  शून्य collect_coverage_and_indices (hb_sorted_vector_t<hb_codepoint_t>& new_coverage,
                                     ध्रुव Offset16To<Coverage>& coverage,
                                     unsigned i,
                                     unsigned end_index,
                                     hb_set_t& indices,
                                     ध्रुव hb_set_t& glyphset,
                                     ध्रुव hb_map_t& glyph_map) ध्रुव
  {
    जइ (!coverage) प्रत्यागम;

    प्रति (ध्रुव auto _ : (एतत्+coverage).iter ())
    {
      जइ (i >= end_index) प्रत्यागम;
      जइ (glyphset.has (_))
      {
        unsigned new_gid = glyph_map.get (_);
        new_coverage.push (new_gid);
        indices.add (i);
      }
      i++;
    }
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    ध्रुव hb_set_t &glyphset = c->plan->_glyphset_cmaped;
    ध्रुव hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*एतत्);
    जइ (unlikely (!c->serializer->extend_min (out))) return_trace (मिच्छ);
    जइ (!c->serializer->check_assign (out->minConnectorOverlap, minConnectorOverlap, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (मिच्छ);

    hb_sorted_vector_t<hb_codepoint_t> new_vert_coverage;
    hb_sorted_vector_t<hb_codepoint_t> new_hori_coverage;
    hb_set_t indices;
    collect_coverage_and_indices (new_vert_coverage, vertGlyphCoverage, 0, vert_Glyph_गणनम्, indices, glyphset, glyph_map);
    collect_coverage_and_indices (new_hori_coverage, horizGlyphCoverage, vert_Glyph_गणनम्, vert_Glyph_गणनम् + horiz_Glyph_गणनम्, indices, glyphset, glyph_map);

    जइ (!c->serializer->check_assign (out->vert_Glyph_गणनम्, new_vert_coverage.length, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (मिच्छ);
    जइ (!c->serializer->check_assign (out->horiz_Glyph_गणनम्, new_hori_coverage.length, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (मिच्छ);

    प्रति (unsigned i : indices.iter ())
    {
      auto *o = c->serializer->embed (glyphConstruction[i]);
      जइ (!o) return_trace (मिच्छ);
      o->serialize_subset (c, glyphConstruction[i], एतत्);
    }

    जइ (new_vert_coverage)
      out->vertGlyphCoverage.serialize_serialize (c->serializer, new_vert_coverage.iter ());

    जइ (new_hori_coverage)
    out->horizGlyphCoverage.serialize_serialize (c->serializer, new_hori_coverage.iter ());
    return_trace (सच्च);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव
  {
    प्रति (ध्रुव auto &entry : hb_zip (एतत्+vertGlyphCoverage, glyphConstruction.as_array (vert_Glyph_गणनम्)))
      जइ (c->glyph_set->has (hb_first (entry)))
        (एतत्+hb_second (entry)).collect_variation_indices (c);
    प्रति (ध्रुव auto &entry : hb_zip (एतत्+horizGlyphCoverage, glyphConstruction.as_array (vert_Glyph_गणनम् + horiz_Glyph_गणनम्).sub_array (vert_Glyph_गणनम्)))
      जइ (c->glyph_set->has (hb_first (entry)))
        (एतत्+hb_second (entry)).collect_variation_indices (c);
  }

  तार्किक sanitize_offsets (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    unsigned पूर्णाङ्क गणनम् = vert_Glyph_गणनम् + horiz_Glyph_गणनम्;
    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
      जइ (!glyphConstruction.arrayZ[i].sanitize (c, एतत्)) return_trace (मिच्छ);
    return_trace (सच्च);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  vertGlyphCoverage.sanitize (c, एतत्) &&
		  horizGlyphCoverage.sanitize (c, एतत्) &&
		  hb_barrier () &&
		  c->check_array (glyphConstruction.arrayZ, vert_Glyph_गणनम् + horiz_Glyph_गणनम्) &&
		  sanitize_offsets (c));
  }

  hb_position_t get_min_connector_overlap (hb_direction_t direction,
						  hb_font_t *font) ध्रुव
  { प्रत्यागम font->em_scale_dir (minConnectorOverlap, direction); }

  unsigned पूर्णाङ्क get_glyph_variants (hb_codepoint_t glyph,
				   hb_direction_t direction,
				   hb_font_t *font,
				   unsigned पूर्णाङ्क start_offset,
				   unsigned पूर्णाङ्क *variants_गणनम्, /* IN/OUT */
				   hb_ot_math_glyph_variant_t *variants /* OUT */) ध्रुव
  { प्रत्यागम get_glyph_construction (glyph, direction, font)
	   .get_variants (direction, font, start_offset, variants_गणनम्, variants); }

  unsigned पूर्णाङ्क get_glyph_parts (hb_codepoint_t glyph,
				hb_direction_t direction,
				hb_font_t *font,
				unsigned पूर्णाङ्क start_offset,
				unsigned पूर्णाङ्क *parts_गणनम्, /* IN/OUT */
				hb_ot_math_glyph_part_t *parts /* OUT */,
				hb_position_t *italics_correction /* OUT */) ध्रुव
  { प्रत्यागम get_glyph_construction (glyph, direction, font)
	   .get_assembly ()
	   .get_parts (direction, font,
		       start_offset, parts_गणनम्, parts,
		       italics_correction); }

  गुप्त:
  ध्रुव MathGlyphConstruction &
  get_glyph_construction (hb_codepoint_t glyph,
			  hb_direction_t direction,
			  hb_font_t *font HB_UNUSED) ध्रुव
  {
    तार्किक vertical = HB_DIRECTION_IS_VERTICAL (direction);
    unsigned पूर्णाङ्क गणनम् = vertical ? vert_Glyph_गणनम् : horiz_Glyph_गणनम्;
    ध्रुव Offset16To<Coverage> &coverage = vertical ? vertGlyphCoverage
						  : horizGlyphCoverage;

    unsigned पूर्णाङ्क index = (एतत्+coverage).get_coverage (glyph);
    जइ (unlikely (index >= गणनम्)) प्रत्यागम Null (MathGlyphConstruction);

    जइ (!vertical)
      index += vert_Glyph_गणनम्;

    प्रत्यागम एतत्+glyphConstruction[index];
  }

  रक्षित:
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
  HBUINT16	vert_Glyph_गणनम्;	/* Number of glyphs for which
				 * information is provided for
				 * vertically growing variants. */
  HBUINT16	horiz_Glyph_गणनम्;/* Number of glyphs for which
				 * information is provided for
				 * horizontally growing variants. */

  /* Array of offsets to MathGlyphConstruction tables - from the beginning of
     the MathVariants table, for shapes growing in vertical/horizontal
     direction. */
  UnsizedArrayOf<Offset16To<MathGlyphConstruction>>
			glyphConstruction;

  सार्वजनिक:
  DEFINE_SIZE_ARRAY (10, glyphConstruction);
};


/*
 * MATH -- Mathematical typesetting
 * https://docs.microsoft.com/en-us/typography/opentype/spec/math
 */

संरचना MATH
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_MATH;

  तार्किक has_data () ध्रुव { प्रत्यागम version.to_int (); }

  शून्य depend (hb_depend_data_builder_t *depend_data) ध्रुव
  {
    जइ (mathVariants)
      (एतत्+mathVariants).depend (depend_data);
  }

  शून्य closure_glyphs (hb_set_t *glyph_set) ध्रुव
  {
    जइ (mathVariants)
    {
      hb_set_t variant_glyphs;
      (एतत्+mathVariants).closure_glyphs (glyph_set, &variant_glyphs);
      hb_set_union (glyph_set, &variant_glyphs);
    }
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->embed (*एतत्);
    जइ (unlikely (!out)) return_trace (मिच्छ);

    out->mathConstants.serialize_copy (c->serializer, mathConstants, एतत्, 0, hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map);
    out->mathGlyphInfo.serialize_subset (c, mathGlyphInfo, एतत्);
    out->mathVariants.serialize_subset (c, mathVariants, एतत्);
    return_trace (सच्च);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c, ध्रुव hb_set_t *variant_glyph_set) ध्रुव
  {
    (एतत्+mathConstants).collect_variation_indices (c);
    (एतत्+mathGlyphInfo).collect_variation_indices (c);
    auto *glyph_set = c->glyph_set;
    c->glyph_set = variant_glyph_set;
    (एतत्+mathVariants).collect_variation_indices (c);
    c->glyph_set = glyph_set;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (version.sanitize (c) &&
		  likely (version.major == 1) &&
		  hb_barrier () &&
		  mathConstants.sanitize (c, एतत्) &&
		  mathGlyphInfo.sanitize (c, एतत्) &&
		  mathVariants.sanitize (c, एतत्));
  }

  // https://github.com/harfbuzz/harfbuzz/issues/4653
  HB_INTERNAL तार्किक is_bad_cambria (hb_font_t *font) ध्रुव
  {
#ifndef HB_NO_MATH
    विकल्प HB_CODEPOINT_ENCODE3 (font->face->table.MATH.get_blob ()->length,
                                 (एतत्+mathConstants).minHeight[1], // displayOperatorMinHeight
                                 (एतत्+mathConstants).minHeight[0]) // delimitedSubFormulaMinHeight
    {
      /* sha1sum:ab4a4fe054d23061f3c039493d6f665cfda2ecf5  cambria.ttc
       * sha1sum:086855301bff644f9d8827b88491fcf73a6d4cb9  cambria.ttc
       * sha1sum:b1e5a3feaca2ea3dfcf79ccb377de749ecf60343  cambria.ttc */
      पक्ष HB_CODEPOINT_ENCODE3 (25722, 2500, 3000):
        प्रत्यागम सच्च;
    }
#endif
    प्रत्यागम मिच्छ;
  }

  hb_position_t get_constant (hb_ot_math_constant_t  constant,
			      hb_font_t		   *font) ध्रुव
  { प्रत्यागम (एतत्+mathConstants).get_मानम् (constant, font); }

  ध्रुव MathGlyphInfo &get_glyph_info () ध्रुव { प्रत्यागम एतत्+mathGlyphInfo; }

  ध्रुव MathVariants &get_variants () ध्रुव    { प्रत्यागम एतत्+mathVariants; }

  रक्षित:
  FixedVersion<>version;	/* Version of the MATH table
				 * initially set to 0x00010000u */
  Offset16To<MathConstants>
		mathConstants;	/* MathConstants table */
  Offset16To<MathGlyphInfo>
		mathGlyphInfo;	/* MathGlyphInfo table */
  Offset16To<MathVariants>
		mathVariants;	/* MathVariants table */

  सार्वजनिक:
  DEFINE_SIZE_STATIC (10);
};

} /* namespace OT */


#endif /* HB_OT_MATH_TABLE_HH */

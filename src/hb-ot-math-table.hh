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


संरचना Math_मान_Record
{
  hb_position_t get_x_मान (hb_font_t *font, अचर शून्य *base) अचर
  { फर्काऊ hb_saturate_add (font->em_scale_x (value), (base+deviceTable).get_x_delta (font, get_var_store (font, base))); }
  hb_position_t get_y_मान (hb_font_t *font, अचर शून्य *base) अचर
  { फर्काऊ hb_saturate_add (font->em_scale_y (value), (base+deviceTable).get_y_delta (font, get_var_store (font, base))); }

  अचर ItemVariationStore &get_var_store (hb_font_t *font, अचर शून्य *base) अचर
  {
#यदि !defined(HB_NO_VAR) && !defined(HB_NO_OT_LAYOUT)
    यदि ((base+deviceTable).is_variation_device ())
      फर्काऊ font->face->table.GDEF->table->get_var_store ();
#endif
    फर्काऊ Null (ItemVariationStore);
  }

  Math_मान_Record* copy (hb_serialize_context_t *c, अचर शून्य *base,
                         अचर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णाङ्क>> *variation_map) अचर
  {
    TRACE_SERIALIZE (एतत्);
    स्वतः *out = c->embed (एतत्);
    यदि (unlikely (!out)) return_trace (nullptr);
    hb_pair_t<अचिन्हित, पूर्णाङ्क> *varidx_delta;
    यदि (variation_map->has ((base+deviceTable).get_variation_index (), &varidx_delta) &&
        !c->check_assign (out->value, (int64_t) value + hb_second (*varidx_delta),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (nullptr);
    out->deviceTable.serialize_copy (c, deviceTable, base, 0, hb_serialize_context_t::Head, variation_map);

    return_trace (out);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c, अचर शून्य *base) अचर
  { (base+deviceTable).collect_variation_indices (c); }

  तार्किक sanitize (hb_sanitize_context_t *c, अचर शून्य *base) अचर
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
  मित्र संरचना MATH;

  MathConstants* copy (hb_serialize_context_t *c,
                      अचर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णाङ्क>> *variation_map) अचर
  {
    TRACE_SERIALIZE (एतत्);
    स्वतः *out = c->start_embed (एतत्);

    HBINT16 *p = c->allocate_size<HBINT16> (HBINT16::static_size * 2);
    यदि (unlikely (!p)) return_trace (nullptr);
    hb_memcpy (p, percentScaleDown, HBINT16::static_size * 2);

    HBUINT16 *m = c->allocate_size<HBUINT16> (HBUINT16::static_size * 2);
    यदि (unlikely (!m)) return_trace (nullptr);
    hb_memcpy (m, minHeight, HBUINT16::static_size * 2);

    अचिन्हित गन्ती = ARRAY_LENGTH (math_मान_Records);
    लागि (अचिन्हित i = 0; i < गन्ती; i++)
      यदि (!c->copy (math_मान_Records[i], एतत्, variation_map))
        return_trace (nullptr);

    यदि (!c->embed (radicalDegreeBottomRaisePercent)) return_trace (nullptr);
    return_trace (out);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) अचर
  {
    लागि (अचर स्वतः &record : math_मान_Records)
      record.collect_variation_indices (c, एतत्);
  }

  तार्किक sanitize_math_मान_records (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);

    अचिन्हित पूर्णाङ्क गन्ती = ARRAY_LENGTH (math_मान_Records);
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
      यदि (!math_मान_Records[i].sanitize (c, एतत्))
	return_trace (असत्य);

    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && sanitize_math_मान_records (c));
  }

  hb_position_t get_मान (hb_ot_math_constant_t constant,
			   hb_font_t *font) अचर
  {
    विकल्प (constant) {

    पक्ष HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_DOWN:
    पक्ष HB_OT_MATH_CONSTANT_SCRIPT_SCRIPT_PERCENT_SCALE_DOWN:
      फर्काऊ percentScaleDown[constant - HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_DOWN];

    पक्ष HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_HEIGHT:
    पक्ष HB_OT_MATH_CONSTANT_DISPLAY_OPERATOR_MIN_HEIGHT:
      फर्काऊ font->em_scale_y (minHeight[constant - HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_HEIGHT]);

    पक्ष HB_OT_MATH_CONSTANT_RADICAL_KERN_AFTER_DEGREE:
    पक्ष HB_OT_MATH_CONSTANT_RADICAL_KERN_BEFORE_DEGREE:
    पक्ष HB_OT_MATH_CONSTANT_SKEWED_FRACTION_HORIZONTAL_GAP:
    पक्ष HB_OT_MATH_CONSTANT_SPACE_AFTER_SCRIPT:
      फर्काऊ math_मान_Records[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_x_मान (font, एतत्);

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
    पक्ष HB_OT_MATH_CONSTANT_FRACTION_अङ्क_DISPLAY_STYLE_GAP_MIN:
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
    पक्ष HB_OT_MATH_CONSTANT_थाक_BOTTOM_DISPLAY_STYLE_SHIFT_DOWN:
    पक्ष HB_OT_MATH_CONSTANT_थाक_BOTTOM_SHIFT_DOWN:
    पक्ष HB_OT_MATH_CONSTANT_थाक_DISPLAY_STYLE_GAP_MIN:
    पक्ष HB_OT_MATH_CONSTANT_थाक_GAP_MIN:
    पक्ष HB_OT_MATH_CONSTANT_थाक_शीर्ष_DISPLAY_STYLE_SHIFT_UP:
    पक्ष HB_OT_MATH_CONSTANT_थाक_शीर्ष_SHIFT_UP:
    पक्ष HB_OT_MATH_CONSTANT_STRETCH_थाक_BOTTOM_SHIFT_DOWN:
    पक्ष HB_OT_MATH_CONSTANT_STRETCH_थाक_GAP_ABOVE_MIN:
    पक्ष HB_OT_MATH_CONSTANT_STRETCH_थाक_GAP_BELOW_MIN:
    पक्ष HB_OT_MATH_CONSTANT_STRETCH_थाक_शीर्ष_SHIFT_UP:
    पक्ष HB_OT_MATH_CONSTANT_SUBSCRIPT_BASELINE_DROP_MIN:
    पक्ष HB_OT_MATH_CONSTANT_SUBSCRIPT_SHIFT_DOWN:
    पक्ष HB_OT_MATH_CONSTANT_SUBSCRIPT_शीर्ष_MAX:
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
      फर्काऊ math_मान_Records[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_y_मान (font, एतत्);

    पक्ष HB_OT_MATH_CONSTANT_RADICAL_DEGREE_BOTTOM_RAISE_PERCENT:
      फर्काऊ radicalDegreeBottomRaisePercent;

    स्वभाव:
      फर्काऊ 0;
    }
  }

  रक्षित:
  HBINT16 percentScaleDown[2];
  HBUINT16 minHeight[2];
  Math_मान_Record math_मान_Records[51];
  HBINT16 radicalDegreeBottomRaisePercent;

  सार्वजनिक:
  DEFINE_SIZE_STATIC (214);
};

संरचना MathItalicsCorrectionInfo
{
  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    अचर hb_set_t &glyphset = c->plan->_glyphset_mathed;
    अचर hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (एतत्+coverage, italicsCorrection)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->italicsCorrection, एतत्, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (सत्य);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) अचर
  {
    लागि (अचर स्वतः &entry : hb_zip (एतत्+coverage, italicsCorrection))
      यदि (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, एतत्);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  coverage.sanitize (c, एतत्) &&
		  italicsCorrection.sanitize (c, एतत्));
  }

  hb_position_t get_मान (hb_codepoint_t glyph,
			   hb_font_t *font) अचर
  {
    अचिन्हित पूर्णाङ्क index = (एतत्+coverage).get_coverage (glyph);
    फर्काऊ italicsCorrection[index].get_x_मान (font, एतत्);
  }

  रक्षित:
  Offset16To<Coverage>       coverage;		/* Offset to Coverage table -
						 * from the beginning of
						 * MathItalicsCorrectionInfo
						 * table. */
  Array16Of<Math_मान_Record> italicsCorrection;	/* Array of MathValueRecords
						 * defining italics correction
						 * values for each
						 * covered glyph. */

  सार्वजनिक:
  DEFINE_SIZE_ARRAY (4, italicsCorrection);
};

संरचना Math_शीर्ष_Accent_Attachment
{
  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    अचर hb_set_t &glyphset = c->plan->_glyphset_mathed;
    अचर hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (एतत्+शीर्ष_Accent_Coverage, शीर्ष_Accent_Attachment)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->शीर्ष_Accent_Attachment, एतत्, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->शीर्ष_Accent_Coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (सत्य);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) अचर
  {
    लागि (अचर स्वतः &entry : hb_zip (एतत्+शीर्ष_Accent_Coverage, शीर्ष_Accent_Attachment))
      यदि (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, एतत्);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  शीर्ष_Accent_Coverage.sanitize (c, एतत्) &&
		  शीर्ष_Accent_Attachment.sanitize (c, एतत्));
  }

  hb_position_t get_मान (hb_codepoint_t glyph,
			   hb_font_t *font) अचर
  {
    अचिन्हित पूर्णाङ्क index = (एतत्+शीर्ष_Accent_Coverage).get_coverage (glyph);
    यदि (index == NOT_COVERED)
      फर्काऊ font->get_glyph_h_advance (glyph) / 2;
    फर्काऊ शीर्ष_Accent_Attachment[index].get_x_मान (font, एतत्);
  }

  रक्षित:
  Offset16To<Coverage>       शीर्ष_Accent_Coverage;   /* Offset to Coverage table -
						 * from the beginning of
						 * MathTopAccentAttachment
						 * table. */
  Array16Of<Math_मान_Record> शीर्ष_Accent_Attachment; /* Array of MathValueRecords
						 * defining top accent
						 * attachment points for each
						 * covered glyph. */

  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2 + 2, शीर्ष_Accent_Attachment);
};

संरचना MathKern
{
  MathKern* copy (hb_serialize_context_t *c,
                      अचर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णाङ्क>> *variation_map) अचर
  {
    TRACE_SERIALIZE (एतत्);
    स्वतः *out = c->start_embed (एतत्);

    यदि (unlikely (!c->embed (height_गन्ती))) return_trace (nullptr);

    अचिन्हित गन्ती = 2 * height_गन्ती + 1;
    लागि (अचिन्हित i = 0; i < गन्ती; i++)
      यदि (!c->copy (math_मान_Records_Z.arrayZ[i], एतत्, variation_map))
        return_trace (nullptr);

    return_trace (out);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) अचर
  {
    लागि (अचर स्वतः &record : math_मान_Records_Z.as_array (2 * height_गन्ती + 1))
      record.collect_variation_indices (c, एतत्);
  }

  तार्किक sanitize_math_मान_records (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    अचिन्हित पूर्णाङ्क गन्ती = 2 * height_गन्ती + 1;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
      यदि (!math_मान_Records_Z.arrayZ[i].sanitize (c, एतत्)) return_trace (असत्य);
    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  c->check_array (math_मान_Records_Z.arrayZ, 2 * height_गन्ती + 1) &&
		  sanitize_math_मान_records (c));
  }

  hb_position_t get_मान (hb_position_t correction_height, hb_font_t *font) अचर
  {
    अचर Math_मान_Record* correctionHeight = math_मान_Records_Z.arrayZ;
    अचर Math_मान_Record* kern_मान = math_मान_Records_Z.arrayZ + height_गन्ती;
    पूर्णाङ्क sign = font->y_scale < 0 ? -1 : +1;

    /* According to OpenType spec (v1.9), except for the boundary cases, the index
     * chosen for kern value should be i such that
     *    correctionHeight[i-1] <= correction_height < correctionHeight[i]
     * We can use the binary search algorithm of std::upper_bound(). Or, we can
     * use the internal hb_bsearch_impl.
     */
    अचिन्हित पूर्णाङ्क pos;
    स्वतः cmp = +[](अचर शून्य* key, अचर शून्य* p,
                   पूर्णाङ्क sign, hb_font_t* font, अचर MathKern* mathKern) -> पूर्णाङ्क {
      hb_position_t a = *(hb_position_t *) key;
      hb_position_t b = ((Math_मान_Record *) p)->get_y_मान (font, mathKern);
      यदि (a == b) फर्काऊ 0;
      फर्काऊ sign > 0 ? (a < b ? -1 : +1) : (b < a ? -1 : +1);
    };
    अचिन्हित पूर्णाङ्क i = hb_bsearch_impl(&pos, correction_height, correctionHeight,
                                     height_गन्ती, Math_मान_Record::static_size,
                                     cmp, sign, font, एतत्) ? pos + 1 : pos;
    फर्काऊ kern_मान[i].get_x_मान (font, एतत्);
  }

  अचिन्हित पूर्णाङ्क get_entries (अचिन्हित पूर्णाङ्क start_offset,
			    अचिन्हित पूर्णाङ्क *entries_गन्ती, /* IN/OUT */
			    hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			    hb_font_t *font) अचर
  {
    अचर Math_मान_Record* correctionHeight = math_मान_Records_Z.arrayZ;
    अचर Math_मान_Record* kern_मान = math_मान_Records_Z.arrayZ + height_गन्ती;
    अचर अचिन्हित पूर्णाङ्क entries_गन्ती_2 = height_गन्ती + 1;

    यदि (entries_गन्ती && kern_entries)
    {
      अचिन्हित पूर्णाङ्क start = hb_min (start_offset, entries_गन्ती_2);
      अचिन्हित पूर्णाङ्क end = hb_min (start + *entries_गन्ती, entries_गन्ती_2);
      *entries_गन्ती = end - start;

      लागि (अचिन्हित पूर्णाङ्क i = 0; i < *entries_गन्ती; i++) {
	अचिन्हित पूर्णाङ्क j = start + i;

	hb_position_t max_height;
	यदि (j == height_गन्ती) {
	  max_height = INT32_MAX;
	} अन्यथा {
	  max_height = correctionHeight[j].get_y_मान (font, एतत्);
	}

	kern_entries[i] = {max_height, kern_मान[j].get_x_मान (font, एतत्)};
      }
    }
    फर्काऊ entries_गन्ती_2;
  }

  रक्षित:
  HBUINT16	height_गन्ती;
  UnsizedArrayOf<Math_मान_Record>
		math_मान_Records_Z;
				/* Array of correction heights at
				 * which the kern value changes.
				 * Sorted by the height value in
				 * design units (heightCount entries),
				 * Followed by:
				 * Array of kern values corresponding
				 * to heights. (heightCount+1 entries).
				 */

  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2, math_मान_Records_Z);
};

संरचना MathKernInfoRecord
{
  MathKernInfoRecord* copy (hb_serialize_context_t *c, अचर शून्य *base,
                            अचर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णाङ्क>> *variation_map) अचर
  {
    TRACE_SERIALIZE (एतत्);
    स्वतः *out = c->embed (एतत्);
    यदि (unlikely (!out)) return_trace (nullptr);

    अचिन्हित गन्ती = ARRAY_LENGTH (mathKern);
    लागि (अचिन्हित i = 0; i < गन्ती; i++)
      out->mathKern[i].serialize_copy (c, mathKern[i], base, 0, hb_serialize_context_t::Head, variation_map);

    return_trace (out);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c, अचर शून्य *base) अचर
  {
    लागि (अचर स्वतः &offset : mathKern)
      (base+offset).collect_variation_indices (c);
  }

  तार्किक sanitize (hb_sanitize_context_t *c, अचर शून्य *base) अचर
  {
    TRACE_SANITIZE (एतत्);

    अचिन्हित पूर्णाङ्क गन्ती = ARRAY_LENGTH (mathKern);
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
      यदि (unlikely (!mathKern[i].sanitize (c, base)))
	return_trace (असत्य);

    return_trace (सत्य);
  }

  hb_position_t get_kerning (hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font,
			     अचर शून्य *base) अचर
  {
    अचिन्हित पूर्णाङ्क idx = kern;
    यदि (unlikely (idx >= ARRAY_LENGTH (mathKern))) फर्काऊ 0;
    फर्काऊ (base+mathKern[idx]).get_मान (correction_height, font);
  }

  अचिन्हित पूर्णाङ्क get_kernings (hb_ot_math_kern_t kern,
			     अचिन्हित पूर्णाङ्क start_offset,
			     अचिन्हित पूर्णाङ्क *entries_गन्ती, /* IN/OUT */
			     hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			     hb_font_t *font,
			     अचर शून्य *base) अचर
  {
    अचिन्हित पूर्णाङ्क idx = kern;
    यदि (unlikely (idx >= ARRAY_LENGTH (mathKern)) || !mathKern[idx]) {
      यदि (entries_गन्ती) *entries_गन्ती = 0;
      फर्काऊ 0;
    }
    फर्काऊ (base+mathKern[idx]).get_entries (start_offset,
					     entries_गन्ती,
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
  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    अचर hb_set_t &glyphset = c->plan->_glyphset_mathed;
    अचर hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (एतत्+mathKernCoverage, mathKernInfoRecords)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->mathKernInfoRecords, एतत्, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->mathKernCoverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (सत्य);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) अचर
  {
    लागि (अचर स्वतः &entry : hb_zip (एतत्+mathKernCoverage, mathKernInfoRecords))
      यदि (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, एतत्);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  mathKernCoverage.sanitize (c, एतत्) &&
		  mathKernInfoRecords.sanitize (c, एतत्));
  }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font) अचर
  {
    अचिन्हित पूर्णाङ्क index = (एतत्+mathKernCoverage).get_coverage (glyph);
    फर्काऊ mathKernInfoRecords[index].get_kerning (kern, correction_height, font, एतत्);
  }

  अचिन्हित पूर्णाङ्क get_kernings (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     अचिन्हित पूर्णाङ्क start_offset,
			     अचिन्हित पूर्णाङ्क *entries_गन्ती, /* IN/OUT */
			     hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			     hb_font_t *font) अचर
  {
    अचिन्हित पूर्णाङ्क index = (एतत्+mathKernCoverage).get_coverage (glyph);
    फर्काऊ mathKernInfoRecords[index].get_kernings (kern,
						    start_offset,
						    entries_गन्ती,
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
  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->serializer->embed (*एतत्);
    यदि (unlikely (!out)) return_trace (असत्य);

    out->mathItalicsCorrectionInfo.serialize_subset (c, mathItalicsCorrectionInfo, एतत्);
    out->math_शीर्ष_Accent_Attachment.serialize_subset (c, math_शीर्ष_Accent_Attachment, एतत्);

    अचर hb_set_t &glyphset = c->plan->_glyphset_mathed;
    अचर hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः it =
    + hb_iter (एतत्+extendedShapeCoverage)
    | hb_take (c->plan->source->get_अङ्क_glyphs ())
    | hb_filter (glyphset)
    | hb_map_retains_sorting (glyph_map)
    ;

    यदि (it) out->extendedShapeCoverage.serialize_serialize (c->serializer, it);
    अन्यथा out->extendedShapeCoverage = 0;

    out->mathKernInfo.serialize_subset (c, mathKernInfo, एतत्);
    return_trace (सत्य);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) अचर
  {
    (एतत्+mathItalicsCorrectionInfo).collect_variation_indices (c);
    (एतत्+math_शीर्ष_Accent_Attachment).collect_variation_indices (c);
    (एतत्+mathKernInfo).collect_variation_indices (c);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  mathItalicsCorrectionInfo.sanitize (c, एतत्) &&
		  math_शीर्ष_Accent_Attachment.sanitize (c, एतत्) &&
		  extendedShapeCoverage.sanitize (c, एतत्) &&
		  mathKernInfo.sanitize (c, एतत्));
  }

  hb_position_t
  get_italics_correction (hb_codepoint_t  glyph, hb_font_t *font) अचर
  { फर्काऊ (एतत्+mathItalicsCorrectionInfo).get_मान (glyph, font); }

  hb_position_t
  get_शीर्ष_accent_attachment (hb_codepoint_t  glyph, hb_font_t *font) अचर
  { फर्काऊ (एतत्+math_शीर्ष_Accent_Attachment).get_मान (glyph, font); }

  तार्किक is_extended_shape (hb_codepoint_t glyph) अचर
  { फर्काऊ (एतत्+extendedShapeCoverage).get_coverage (glyph) != NOT_COVERED; }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font) अचर
  { फर्काऊ (एतत्+mathKernInfo).get_kerning (glyph, kern, correction_height, font); }

  hb_position_t get_kernings (hb_codepoint_t glyph,
			      hb_ot_math_kern_t kern,
			      अचिन्हित पूर्णाङ्क start_offset,
			      अचिन्हित पूर्णाङ्क *entries_गन्ती, /* IN/OUT */
			      hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			      hb_font_t *font) अचर
  { फर्काऊ (एतत्+mathKernInfo).get_kernings (glyph,
					     kern,
					     start_offset,
					     entries_गन्ती,
					     kern_entries,
					     font); }

  रक्षित:
  /* Offset to MathItalicsCorrectionInfo table -
   * from the beginning of MathGlyphInfo table. */
  Offset16To<MathItalicsCorrectionInfo> mathItalicsCorrectionInfo;

  /* Offset to MathTopAccentAttachment table -
   * from the beginning of MathGlyphInfo table. */
  Offset16To<Math_शीर्ष_Accent_Attachment> math_शीर्ष_Accent_Attachment;

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
  मित्र संरचना MathGlyphConstruction;

  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->serializer->embed (एतत्);
    यदि (unlikely (!out)) return_trace (असत्य);

    अचर hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_assign (out->variantGlyph, glyph_map.get (variantGlyph), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  शून्य depend (hb_depend_data_builder_t *depend_data, अचिन्हित source) अचर
  { depend_data->add_depend(source, HB_OT_TAG_MATH, variantGlyph); }

  शून्य closure_glyphs (hb_set_t *variant_glyphs) अचर
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
  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->serializer->embed (एतत्);
    यदि (unlikely (!out)) return_trace (असत्य);

    अचर hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_assign (out->glyph, glyph_map.get (glyph), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  शून्य extract (hb_ot_math_glyph_part_t &out,
		int64_t mult,
		hb_font_t *font) अचर
  {
    out.glyph			= glyph;

    out.start_connector_length	= font->em_mult (startConnectorLength, mult);
    out.end_connector_length	= font->em_mult (endConnectorLength, mult);
    out.full_advance		= font->em_mult (fullAdvance, mult);

    static_assert ((अचिन्हित पूर्णाङ्क) HB_OT_MATH_GLYPH_PART_FLAG_EXTENDER ==
		   (अचिन्हित पूर्णाङ्क) PartFlags::Extender, "");

    out.flags = (hb_ot_math_glyph_part_flags_t)
		(अचिन्हित पूर्णाङ्क)
		(partFlags & PartFlags::Defined);
  }

  शून्य depend (hb_depend_data_builder_t *depend_data, अचिन्हित source) अचर
  { depend_data->add_depend(source, HB_OT_TAG_MATH, glyph); }

  शून्य closure_glyphs (hb_set_t *variant_glyphs) अचर
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
  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);

    यदि (!c->serializer->copy (italicsCorrection, एतत्, &c->plan->layout_variation_idx_delta_map)) return_trace (असत्य);
    यदि (!c->serializer->copy<HBUINT16> (partRecords.len)) return_trace (असत्य);

    लागि (अचर स्वतः& record : partRecords.iter ())
      यदि (!record.subset (c)) return_trace (असत्य);
    return_trace (सत्य);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) अचर
  {
    italicsCorrection.collect_variation_indices (c, एतत्);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  italicsCorrection.sanitize (c, एतत्) &&
		  partRecords.sanitize (c));
  }

  अचिन्हित पूर्णाङ्क get_parts (hb_direction_t direction,
			  hb_font_t *font,
			  अचिन्हित पूर्णाङ्क start_offset,
			  अचिन्हित पूर्णाङ्क *parts_गन्ती, /* IN/OUT */
			  hb_ot_math_glyph_part_t *parts /* OUT */,
			  hb_position_t *italics_correction /* OUT */) अचर
  {
    यदि (parts_गन्ती && parts)
    {
      int64_t mult = font->dir_mult (direction);
      लागि (स्वतः _ : hb_zip (partRecords.as_array ().sub_array (start_offset, parts_गन्ती),
			    hb_array (parts, *parts_गन्ती)))
	_.first.extract (_.second, mult, font);
    }

    यदि (italics_correction)
      *italics_correction = italicsCorrection.get_x_मान (font, एतत्);

    फर्काऊ partRecords.len;
  }

  शून्य depend (hb_depend_data_builder_t *depend_data, अचिन्हित source) अचर
  {
    लागि (अचर स्वतः& _ : partRecords.iter ())
      _.depend (depend_data, source);
  }

  शून्य closure_glyphs (hb_set_t *variant_glyphs) अचर
  {
    लागि (अचर स्वतः& _ : partRecords.iter ())
      _.closure_glyphs (variant_glyphs);
  }

  रक्षित:
  Math_मान_Record
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
  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    out->glyphAssembly.serialize_subset (c, glyphAssembly, एतत्);

    यदि (!c->serializer->check_assign (out->mathGlyphVariantRecord.len, mathGlyphVariantRecord.len, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (असत्य);
    लागि (अचर स्वतः& record : mathGlyphVariantRecord.iter ())
      यदि (!record.subset (c)) return_trace (असत्य);

    return_trace (सत्य);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) अचर
  {
    (एतत्+glyphAssembly).collect_variation_indices (c);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  glyphAssembly.sanitize (c, एतत्) &&
		  mathGlyphVariantRecord.sanitize (c));
  }

  अचर MathGlyphAssembly &get_assembly () अचर { फर्काऊ एतत्+glyphAssembly; }

  अचिन्हित पूर्णाङ्क get_variants (hb_direction_t direction,
			     hb_font_t *font,
			     अचिन्हित पूर्णाङ्क start_offset,
			     अचिन्हित पूर्णाङ्क *variants_गन्ती, /* IN/OUT */
			     hb_ot_math_glyph_variant_t *variants /* OUT */) अचर
  {
    यदि (variants_गन्ती && variants)
    {
      int64_t mult = font->dir_mult (direction);
      लागि (स्वतः _ : hb_zip (mathGlyphVariantRecord.as_array ().sub_array (start_offset, variants_गन्ती),
			    hb_array (variants, *variants_गन्ती)))
	_.second = {_.first.variantGlyph, font->em_mult (_.first.advanceMeasurement, mult)};
    }
    फर्काऊ mathGlyphVariantRecord.len;
  }

  शून्य depend (hb_depend_data_builder_t *depend_data, अचिन्हित source) अचर
  {
    (एतत्+glyphAssembly).depend (depend_data, source);

    लागि (अचर स्वतः& _ : mathGlyphVariantRecord.iter ())
      _.depend (depend_data, source);
  }

  शून्य closure_glyphs (hb_set_t *variant_glyphs) अचर
  {
    (एतत्+glyphAssembly).closure_glyphs (variant_glyphs);

    लागि (अचर स्वतः& _ : mathGlyphVariantRecord.iter ())
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
  शून्य depend (hb_depend_data_builder_t *depend_data) अचर
  {
    अचर hb_array_t<अचर Offset16To<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vert_Glyph_गन्ती + horiz_Glyph_गन्ती);

    यदि (vertGlyphCoverage)
    {
      अचर स्वतः vert_offsets = glyph_construction_offsets.sub_array (0, vert_Glyph_गन्ती);
      + hb_zip (एतत्+vertGlyphCoverage, vert_offsets)
      | hb_apply ([&] (अचर hb_pair_t<hb_codepoint_t, अचर Offset16To<MathGlyphConstruction>&> &_)
                  {
                    अचर MathGlyphConstruction &mgc = एतत्+_.second;
                    mgc.depend (depend_data, _.first);
                  })
      ;
    }
    यदि (horizGlyphCoverage)
    {
      अचर स्वतः hori_offsets = glyph_construction_offsets.sub_array (vert_Glyph_गन्ती, horiz_Glyph_गन्ती);
      + hb_zip (एतत्+horizGlyphCoverage, hori_offsets)
      | hb_apply ([&] (अचर hb_pair_t<hb_codepoint_t, अचर Offset16To<MathGlyphConstruction>&> &_)
                  {
                    अचर MathGlyphConstruction &mgc = एतत्+_.second;
                    mgc.depend (depend_data, _.first);
                  })
      ;
    }
  }

  शून्य closure_glyphs (अचर hb_set_t *glyph_set,
                       hb_set_t *variant_glyphs) अचर
  {
    अचर hb_array_t<अचर Offset16To<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vert_Glyph_गन्ती + horiz_Glyph_गन्ती);

    यदि (vertGlyphCoverage)
    {
      अचर स्वतः vert_offsets = glyph_construction_offsets.sub_array (0, vert_Glyph_गन्ती);
      + hb_zip (एतत्+vertGlyphCoverage, vert_offsets)
      | hb_filter (glyph_set, hb_first)
      | hb_map (hb_second)
      | hb_map (hb_add (एतत्))
      | hb_apply ([=] (अचर MathGlyphConstruction &_) { _.closure_glyphs (variant_glyphs); })
      ;
    }

    यदि (horizGlyphCoverage)
    {
      अचर स्वतः hori_offsets = glyph_construction_offsets.sub_array (vert_Glyph_गन्ती, horiz_Glyph_गन्ती);
      + hb_zip (एतत्+horizGlyphCoverage, hori_offsets)
      | hb_filter (glyph_set, hb_first)
      | hb_map (hb_second)
      | hb_map (hb_add (एतत्))
      | hb_apply ([=] (अचर MathGlyphConstruction &_) { _.closure_glyphs (variant_glyphs); })
      ;
    }
  }

  शून्य collect_coverage_and_indices (hb_sorted_vector_t<hb_codepoint_t>& new_coverage,
                                     अचर Offset16To<Coverage>& coverage,
                                     अचिन्हित i,
                                     अचिन्हित end_index,
                                     hb_set_t& indices,
                                     अचर hb_set_t& glyphset,
                                     अचर hb_map_t& glyph_map) अचर
  {
    यदि (!coverage) फर्काऊ;

    लागि (अचर स्वतः _ : (एतत्+coverage).iter ())
    {
      यदि (i >= end_index) फर्काऊ;
      यदि (glyphset.has (_))
      {
        अचिन्हित new_gid = glyph_map.get (_);
        new_coverage.push (new_gid);
        indices.add (i);
      }
      i++;
    }
  }

  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    अचर hb_set_t &glyphset = c->plan->_glyphset_cmaped;
    अचर hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);
    यदि (!c->serializer->check_assign (out->minConnectorOverlap, minConnectorOverlap, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (असत्य);

    hb_sorted_vector_t<hb_codepoint_t> new_vert_coverage;
    hb_sorted_vector_t<hb_codepoint_t> new_hori_coverage;
    hb_set_t indices;
    collect_coverage_and_indices (new_vert_coverage, vertGlyphCoverage, 0, vert_Glyph_गन्ती, indices, glyphset, glyph_map);
    collect_coverage_and_indices (new_hori_coverage, horizGlyphCoverage, vert_Glyph_गन्ती, vert_Glyph_गन्ती + horiz_Glyph_गन्ती, indices, glyphset, glyph_map);

    यदि (!c->serializer->check_assign (out->vert_Glyph_गन्ती, new_vert_coverage.length, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (असत्य);
    यदि (!c->serializer->check_assign (out->horiz_Glyph_गन्ती, new_hori_coverage.length, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (असत्य);

    लागि (अचिन्हित i : indices.iter ())
    {
      स्वतः *o = c->serializer->embed (glyphConstruction[i]);
      यदि (!o) return_trace (असत्य);
      o->serialize_subset (c, glyphConstruction[i], एतत्);
    }

    यदि (new_vert_coverage)
      out->vertGlyphCoverage.serialize_serialize (c->serializer, new_vert_coverage.iter ());

    यदि (new_hori_coverage)
    out->horizGlyphCoverage.serialize_serialize (c->serializer, new_hori_coverage.iter ());
    return_trace (सत्य);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) अचर
  {
    लागि (अचर स्वतः &entry : hb_zip (एतत्+vertGlyphCoverage, glyphConstruction.as_array (vert_Glyph_गन्ती)))
      यदि (c->glyph_set->has (hb_first (entry)))
        (एतत्+hb_second (entry)).collect_variation_indices (c);
    लागि (अचर स्वतः &entry : hb_zip (एतत्+horizGlyphCoverage, glyphConstruction.as_array (vert_Glyph_गन्ती + horiz_Glyph_गन्ती).sub_array (vert_Glyph_गन्ती)))
      यदि (c->glyph_set->has (hb_first (entry)))
        (एतत्+hb_second (entry)).collect_variation_indices (c);
  }

  तार्किक sanitize_offsets (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    अचिन्हित पूर्णाङ्क गन्ती = vert_Glyph_गन्ती + horiz_Glyph_गन्ती;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
      यदि (!glyphConstruction.arrayZ[i].sanitize (c, एतत्)) return_trace (असत्य);
    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  vertGlyphCoverage.sanitize (c, एतत्) &&
		  horizGlyphCoverage.sanitize (c, एतत्) &&
		  hb_barrier () &&
		  c->check_array (glyphConstruction.arrayZ, vert_Glyph_गन्ती + horiz_Glyph_गन्ती) &&
		  sanitize_offsets (c));
  }

  hb_position_t get_min_connector_overlap (hb_direction_t direction,
						  hb_font_t *font) अचर
  { फर्काऊ font->em_scale_dir (minConnectorOverlap, direction); }

  अचिन्हित पूर्णाङ्क get_glyph_variants (hb_codepoint_t glyph,
				   hb_direction_t direction,
				   hb_font_t *font,
				   अचिन्हित पूर्णाङ्क start_offset,
				   अचिन्हित पूर्णाङ्क *variants_गन्ती, /* IN/OUT */
				   hb_ot_math_glyph_variant_t *variants /* OUT */) अचर
  { फर्काऊ get_glyph_construction (glyph, direction, font)
	   .get_variants (direction, font, start_offset, variants_गन्ती, variants); }

  अचिन्हित पूर्णाङ्क get_glyph_parts (hb_codepoint_t glyph,
				hb_direction_t direction,
				hb_font_t *font,
				अचिन्हित पूर्णाङ्क start_offset,
				अचिन्हित पूर्णाङ्क *parts_गन्ती, /* IN/OUT */
				hb_ot_math_glyph_part_t *parts /* OUT */,
				hb_position_t *italics_correction /* OUT */) अचर
  { फर्काऊ get_glyph_construction (glyph, direction, font)
	   .get_assembly ()
	   .get_parts (direction, font,
		       start_offset, parts_गन्ती, parts,
		       italics_correction); }

  गुप्त:
  अचर MathGlyphConstruction &
  get_glyph_construction (hb_codepoint_t glyph,
			  hb_direction_t direction,
			  hb_font_t *font HB_UNUSED) अचर
  {
    तार्किक vertical = HB_DIRECTION_IS_VERTICAL (direction);
    अचिन्हित पूर्णाङ्क गन्ती = vertical ? vert_Glyph_गन्ती : horiz_Glyph_गन्ती;
    अचर Offset16To<Coverage> &coverage = vertical ? vertGlyphCoverage
						  : horizGlyphCoverage;

    अचिन्हित पूर्णाङ्क index = (एतत्+coverage).get_coverage (glyph);
    यदि (unlikely (index >= गन्ती)) फर्काऊ Null (MathGlyphConstruction);

    यदि (!vertical)
      index += vert_Glyph_गन्ती;

    फर्काऊ एतत्+glyphConstruction[index];
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
  HBUINT16	vert_Glyph_गन्ती;	/* Number of glyphs for which
				 * information is provided for
				 * vertically growing variants. */
  HBUINT16	horiz_Glyph_गन्ती;/* Number of glyphs for which
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

  तार्किक has_data () अचर { फर्काऊ version.to_int (); }

  शून्य depend (hb_depend_data_builder_t *depend_data) अचर
  {
    यदि (mathVariants)
      (एतत्+mathVariants).depend (depend_data);
  }

  शून्य closure_glyphs (hb_set_t *glyph_set) अचर
  {
    यदि (mathVariants)
    {
      hb_set_t variant_glyphs;
      (एतत्+mathVariants).closure_glyphs (glyph_set, &variant_glyphs);
      hb_set_union (glyph_set, &variant_glyphs);
    }
  }

  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->serializer->embed (*एतत्);
    यदि (unlikely (!out)) return_trace (असत्य);

    out->mathConstants.serialize_copy (c->serializer, mathConstants, एतत्, 0, hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map);
    out->mathGlyphInfo.serialize_subset (c, mathGlyphInfo, एतत्);
    out->mathVariants.serialize_subset (c, mathVariants, एतत्);
    return_trace (सत्य);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c, अचर hb_set_t *variant_glyph_set) अचर
  {
    (एतत्+mathConstants).collect_variation_indices (c);
    (एतत्+mathGlyphInfo).collect_variation_indices (c);
    स्वतः *glyph_set = c->glyph_set;
    c->glyph_set = variant_glyph_set;
    (एतत्+mathVariants).collect_variation_indices (c);
    c->glyph_set = glyph_set;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
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
  HB_INTERNAL तार्किक is_bad_cambria (hb_font_t *font) अचर
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
        फर्काऊ सत्य;
    }
#endif
    फर्काऊ असत्य;
  }

  hb_position_t get_constant (hb_ot_math_constant_t  constant,
			      hb_font_t		   *font) अचर
  { फर्काऊ (एतत्+mathConstants).get_मान (constant, font); }

  अचर MathGlyphInfo &get_glyph_info () अचर { फर्काऊ एतत्+mathGlyphInfo; }

  अचर MathVariants &get_variants () अचर    { फर्काऊ एतत्+mathVariants; }

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

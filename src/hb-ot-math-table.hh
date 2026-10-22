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

#अगर_अघोषित HB_OT_MATH_TABLE_HH
#घोषणा HB_OT_MATH_TABLE_HH

#समावेश "hb-open-type.hh"
#समावेश "hb-ot-layout-common.hh"
#समावेश "hb-ot-layout-gdef-table.hh"
#समावेश "hb-ot-math.h"
#समावेश "hb-depend-data.hh"

namespace OT {


काष्ठा Math_मूल्य_Record
{
  hb_position_t get_x_मूल्य (hb_font_t *font, स्थिर व्योम *base) स्थिर
  { वापस hb_saturate_add (font->em_scale_x (value), (base+deviceTable).get_x_delta (font, get_var_store (font, base))); }
  hb_position_t get_y_मूल्य (hb_font_t *font, स्थिर व्योम *base) स्थिर
  { वापस hb_saturate_add (font->em_scale_y (value), (base+deviceTable).get_y_delta (font, get_var_store (font, base))); }

  स्थिर ItemVariationStore &get_var_store (hb_font_t *font, स्थिर व्योम *base) स्थिर
  {
#अगर !defined(HB_NO_VAR) && !defined(HB_NO_OT_LAYOUT)
    अगर ((base+deviceTable).is_variation_device ())
      वापस font->face->table.GDEF->table->get_var_store ();
#पूर्ण_अगर
    वापस Null (ItemVariationStore);
  }

  Math_मूल्य_Record* copy (hb_serialize_context_t *c, स्थिर व्योम *base,
                         स्थिर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *variation_map) स्थिर
  {
    TRACE_SERIALIZE (यह);
    स्वतः *out = c->embed (यह);
    अगर (unlikely (!out)) return_trace (nullptr);
    hb_pair_t<अचिन्हित, पूर्णांक> *varidx_delta;
    अगर (variation_map->has ((base+deviceTable).get_variation_सूचकांक (), &varidx_delta) &&
        !c->check_assign (out->value, (int64_t) value + hb_second (*varidx_delta),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (nullptr);
    out->deviceTable.serialize_copy (c, deviceTable, base, 0, hb_serialize_context_t::शीर्ष, variation_map);

    return_trace (out);
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c, स्थिर व्योम *base) स्थिर
  { (base+deviceTable).collect_variation_indices (c); }

  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम *base) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) && deviceTable.sanitize (c, base));
  }

  रक्षित:
  HBINT16		value;		/* The X or Y value in design units */
  Offset16To<Device>	deviceTable;	/* Offset to the device table - from the
					 * beginning of parent table.  May be NULL.
					 * Suggested format for device table is 1. */

  खुला:
  DEFINE_आकार_STATIC (4);
};

काष्ठा MathConstants
{
  मित्र काष्ठा MATH;

  MathConstants* copy (hb_serialize_context_t *c,
                      स्थिर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *variation_map) स्थिर
  {
    TRACE_SERIALIZE (यह);
    स्वतः *out = c->start_embed (यह);

    HBINT16 *p = c->allocate_आकार<HBINT16> (HBINT16::static_आकार * 2);
    अगर (unlikely (!p)) return_trace (nullptr);
    hb_memcpy (p, percentScaleDown, HBINT16::static_आकार * 2);

    HBUINT16 *m = c->allocate_आकार<HBUINT16> (HBUINT16::static_आकार * 2);
    अगर (unlikely (!m)) return_trace (nullptr);
    hb_memcpy (m, minHeight, HBUINT16::static_आकार * 2);

    अचिन्हित गिनती = ARRAY_लंबाई (math_मूल्य_Records);
    क्रम (अचिन्हित i = 0; i < गिनती; i++)
      अगर (!c->copy (math_मूल्य_Records[i], यह, variation_map))
        return_trace (nullptr);

    अगर (!c->embed (radicalDegreeBottomRaisePercent)) return_trace (nullptr);
    return_trace (out);
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर
  {
    क्रम (स्थिर स्वतः &record : math_मूल्य_Records)
      record.collect_variation_indices (c, यह);
  }

  bool sanitize_math_मूल्य_records (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);

    अचिन्हित पूर्णांक गिनती = ARRAY_लंबाई (math_मूल्य_Records);
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
      अगर (!math_मूल्य_Records[i].sanitize (c, यह))
	return_trace (false);

    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) && sanitize_math_मूल्य_records (c));
  }

  hb_position_t get_मूल्य (hb_ot_math_constant_t constant,
			   hb_font_t *font) स्थिर
  {
    चयन (constant) {

    हाल HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_DOWN:
    हाल HB_OT_MATH_CONSTANT_SCRIPT_SCRIPT_PERCENT_SCALE_DOWN:
      वापस percentScaleDown[constant - HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_DOWN];

    हाल HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_HEIGHT:
    हाल HB_OT_MATH_CONSTANT_DISPLAY_OPERATOR_MIN_HEIGHT:
      वापस font->em_scale_y (minHeight[constant - HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_HEIGHT]);

    हाल HB_OT_MATH_CONSTANT_RADICAL_KERN_AFTER_DEGREE:
    हाल HB_OT_MATH_CONSTANT_RADICAL_KERN_BEFORE_DEGREE:
    हाल HB_OT_MATH_CONSTANT_SKEWED_FRACTION_HORIZONTAL_GAP:
    हाल HB_OT_MATH_CONSTANT_SPACE_AFTER_SCRIPT:
      वापस math_मूल्य_Records[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_x_मूल्य (font, यह);

    हाल HB_OT_MATH_CONSTANT_ACCENT_BASE_HEIGHT:
    हाल HB_OT_MATH_CONSTANT_AXIS_HEIGHT:
    हाल HB_OT_MATH_CONSTANT_FLATTENED_ACCENT_BASE_HEIGHT:
    हाल HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_DISPLAY_STYLE_SHIFT_DOWN:
    हाल HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_GAP_MIN:
    हाल HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_SHIFT_DOWN:
    हाल HB_OT_MATH_CONSTANT_FRACTION_DENOM_DISPLAY_STYLE_GAP_MIN:
    हाल HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_DISPLAY_STYLE_SHIFT_UP:
    हाल HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_GAP_MIN:
    हाल HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_SHIFT_UP:
    हाल HB_OT_MATH_CONSTANT_FRACTION_अंक_DISPLAY_STYLE_GAP_MIN:
    हाल HB_OT_MATH_CONSTANT_FRACTION_RULE_THICKNESS:
    हाल HB_OT_MATH_CONSTANT_LOWER_LIMIT_BASELINE_DROP_MIN:
    हाल HB_OT_MATH_CONSTANT_LOWER_LIMIT_GAP_MIN:
    हाल HB_OT_MATH_CONSTANT_MATH_LEADING:
    हाल HB_OT_MATH_CONSTANT_OVERBAR_EXTRA_ASCENDER:
    हाल HB_OT_MATH_CONSTANT_OVERBAR_RULE_THICKNESS:
    हाल HB_OT_MATH_CONSTANT_OVERBAR_VERTICAL_GAP:
    हाल HB_OT_MATH_CONSTANT_RADICAL_DISPLAY_STYLE_VERTICAL_GAP:
    हाल HB_OT_MATH_CONSTANT_RADICAL_EXTRA_ASCENDER:
    हाल HB_OT_MATH_CONSTANT_RADICAL_RULE_THICKNESS:
    हाल HB_OT_MATH_CONSTANT_RADICAL_VERTICAL_GAP:
    हाल HB_OT_MATH_CONSTANT_SKEWED_FRACTION_VERTICAL_GAP:
    हाल HB_OT_MATH_CONSTANT_ढेर_BOTTOM_DISPLAY_STYLE_SHIFT_DOWN:
    हाल HB_OT_MATH_CONSTANT_ढेर_BOTTOM_SHIFT_DOWN:
    हाल HB_OT_MATH_CONSTANT_ढेर_DISPLAY_STYLE_GAP_MIN:
    हाल HB_OT_MATH_CONSTANT_ढेर_GAP_MIN:
    हाल HB_OT_MATH_CONSTANT_ढेर_शीर्ष_DISPLAY_STYLE_SHIFT_UP:
    हाल HB_OT_MATH_CONSTANT_ढेर_शीर्ष_SHIFT_UP:
    हाल HB_OT_MATH_CONSTANT_STRETCH_ढेर_BOTTOM_SHIFT_DOWN:
    हाल HB_OT_MATH_CONSTANT_STRETCH_ढेर_GAP_ABOVE_MIN:
    हाल HB_OT_MATH_CONSTANT_STRETCH_ढेर_GAP_BELOW_MIN:
    हाल HB_OT_MATH_CONSTANT_STRETCH_ढेर_शीर्ष_SHIFT_UP:
    हाल HB_OT_MATH_CONSTANT_SUBSCRIPT_BASELINE_DROP_MIN:
    हाल HB_OT_MATH_CONSTANT_SUBSCRIPT_SHIFT_DOWN:
    हाल HB_OT_MATH_CONSTANT_SUBSCRIPT_शीर्ष_MAX:
    हाल HB_OT_MATH_CONSTANT_SUB_SUPERSCRIPT_GAP_MIN:
    हाल HB_OT_MATH_CONSTANT_SUPERSCRIPT_BASELINE_DROP_MAX:
    हाल HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MAX_WITH_SUBSCRIPT:
    हाल HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MIN:
    हाल HB_OT_MATH_CONSTANT_SUPERSCRIPT_SHIFT_UP:
    हाल HB_OT_MATH_CONSTANT_SUPERSCRIPT_SHIFT_UP_CRAMPED:
    हाल HB_OT_MATH_CONSTANT_UNDERBAR_EXTRA_DESCENDER:
    हाल HB_OT_MATH_CONSTANT_UNDERBAR_RULE_THICKNESS:
    हाल HB_OT_MATH_CONSTANT_UNDERBAR_VERTICAL_GAP:
    हाल HB_OT_MATH_CONSTANT_UPPER_LIMIT_BASELINE_RISE_MIN:
    हाल HB_OT_MATH_CONSTANT_UPPER_LIMIT_GAP_MIN:
      वापस math_मूल्य_Records[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_y_मूल्य (font, यह);

    हाल HB_OT_MATH_CONSTANT_RADICAL_DEGREE_BOTTOM_RAISE_PERCENT:
      वापस radicalDegreeBottomRaisePercent;

    शेष:
      वापस 0;
    }
  }

  रक्षित:
  HBINT16 percentScaleDown[2];
  HBUINT16 minHeight[2];
  Math_मूल्य_Record math_मूल्य_Records[51];
  HBINT16 radicalDegreeBottomRaisePercent;

  खुला:
  DEFINE_आकार_STATIC (214);
};

काष्ठा MathItalicsCorrectionInfo
{
  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्थिर hb_set_t &glyphset = c->plan->_glyphset_mathed;
    स्थिर hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (यह+coverage, italicsCorrection)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->italicsCorrection, यह, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (true);
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर
  {
    क्रम (स्थिर स्वतः &entry : hb_zip (यह+coverage, italicsCorrection))
      अगर (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, यह);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  coverage.sanitize (c, यह) &&
		  italicsCorrection.sanitize (c, यह));
  }

  hb_position_t get_मूल्य (hb_codepoint_t glyph,
			   hb_font_t *font) स्थिर
  {
    अचिन्हित पूर्णांक सूचकांक_2 = (यह+coverage).get_coverage (glyph);
    वापस italicsCorrection[सूचकांक_2].get_x_मूल्य (font, यह);
  }

  रक्षित:
  Offset16To<Coverage>       coverage;		/* Offset to Coverage table -
						 * from the beginning of
						 * MathItalicsCorrectionInfo
						 * table. */
  Array16Of<Math_मूल्य_Record> italicsCorrection;	/* Array of MathValueRecords
						 * defining italics correction
						 * values for each
						 * covered glyph. */

  खुला:
  DEFINE_आकार_ARRAY (4, italicsCorrection);
};

काष्ठा Math_शीर्ष_Accent_Attachment
{
  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्थिर hb_set_t &glyphset = c->plan->_glyphset_mathed;
    स्थिर hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (यह+शीर्ष_Accent_Coverage, शीर्ष_Accent_Attachment)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->शीर्ष_Accent_Attachment, यह, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->शीर्ष_Accent_Coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (true);
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर
  {
    क्रम (स्थिर स्वतः &entry : hb_zip (यह+शीर्ष_Accent_Coverage, शीर्ष_Accent_Attachment))
      अगर (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, यह);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  शीर्ष_Accent_Coverage.sanitize (c, यह) &&
		  शीर्ष_Accent_Attachment.sanitize (c, यह));
  }

  hb_position_t get_मूल्य (hb_codepoint_t glyph,
			   hb_font_t *font) स्थिर
  {
    अचिन्हित पूर्णांक सूचकांक_2 = (यह+शीर्ष_Accent_Coverage).get_coverage (glyph);
    अगर (सूचकांक_2 == NOT_COVERED)
      वापस font->get_glyph_h_advance (glyph) / 2;
    वापस शीर्ष_Accent_Attachment[सूचकांक_2].get_x_मूल्य (font, यह);
  }

  रक्षित:
  Offset16To<Coverage>       शीर्ष_Accent_Coverage;   /* Offset to Coverage table -
						 * from the beginning of
						 * MathTopAccentAttachment
						 * table. */
  Array16Of<Math_मूल्य_Record> शीर्ष_Accent_Attachment; /* Array of MathValueRecords
						 * defining top accent
						 * attachment points for each
						 * covered glyph. */

  खुला:
  DEFINE_आकार_ARRAY (2 + 2, शीर्ष_Accent_Attachment);
};

काष्ठा MathKern
{
  MathKern* copy (hb_serialize_context_t *c,
                      स्थिर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *variation_map) स्थिर
  {
    TRACE_SERIALIZE (यह);
    स्वतः *out = c->start_embed (यह);

    अगर (unlikely (!c->embed (height_गिनती))) return_trace (nullptr);

    अचिन्हित गिनती = 2 * height_गिनती + 1;
    क्रम (अचिन्हित i = 0; i < गिनती; i++)
      अगर (!c->copy (math_मूल्य_Records_Z.arrayZ[i], यह, variation_map))
        return_trace (nullptr);

    return_trace (out);
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर
  {
    क्रम (स्थिर स्वतः &record : math_मूल्य_Records_Z.as_array (2 * height_गिनती + 1))
      record.collect_variation_indices (c, यह);
  }

  bool sanitize_math_मूल्य_records (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अचिन्हित पूर्णांक गिनती = 2 * height_गिनती + 1;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
      अगर (!math_मूल्य_Records_Z.arrayZ[i].sanitize (c, यह)) return_trace (false);
    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  hb_barrier () &&
		  c->check_array (math_मूल्य_Records_Z.arrayZ, 2 * height_गिनती + 1) &&
		  sanitize_math_मूल्य_records (c));
  }

  hb_position_t get_मूल्य (hb_position_t correction_height, hb_font_t *font) स्थिर
  {
    स्थिर Math_मूल्य_Record* correctionHeight = math_मूल्य_Records_Z.arrayZ;
    स्थिर Math_मूल्य_Record* kern_मूल्य = math_मूल्य_Records_Z.arrayZ + height_गिनती;
    पूर्णांक sign = font->y_scale < 0 ? -1 : +1;

    /* According to OpenType spec (v1.9), except for the boundary cases, the index
     * chosen for kern value should be i such that
     *    correctionHeight[i-1] <= correction_height < correctionHeight[i]
     * We can use the binary search algorithm of std::upper_bound(). Or, we can
     * use the internal hb_bsearch_impl.
     */
    अचिन्हित पूर्णांक pos;
    स्वतः cmp = +[](स्थिर व्योम* key, स्थिर व्योम* p,
                   पूर्णांक sign, hb_font_t* font, स्थिर MathKern* mathKern) -> पूर्णांक {
      hb_position_t a = *(hb_position_t *) key;
      hb_position_t b = ((Math_मूल्य_Record *) p)->get_y_मूल्य (font, mathKern);
      अगर (a == b) वापस 0;
      वापस sign > 0 ? (a < b ? -1 : +1) : (b < a ? -1 : +1);
    };
    अचिन्हित पूर्णांक i = hb_bsearch_impl(&pos, correction_height, correctionHeight,
                                     height_गिनती, Math_मूल्य_Record::static_आकार,
                                     cmp, sign, font, यह) ? pos + 1 : pos;
    वापस kern_मूल्य[i].get_x_मूल्य (font, यह);
  }

  अचिन्हित पूर्णांक get_entries (अचिन्हित पूर्णांक start_offset,
			    अचिन्हित पूर्णांक *entries_गिनती, /* IN/OUT */
			    hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			    hb_font_t *font) स्थिर
  {
    स्थिर Math_मूल्य_Record* correctionHeight = math_मूल्य_Records_Z.arrayZ;
    स्थिर Math_मूल्य_Record* kern_मूल्य = math_मूल्य_Records_Z.arrayZ + height_गिनती;
    स्थिर अचिन्हित पूर्णांक entries_गिनती_2 = height_गिनती + 1;

    अगर (entries_गिनती && kern_entries)
    {
      अचिन्हित पूर्णांक start = hb_min (start_offset, entries_गिनती_2);
      अचिन्हित पूर्णांक end = hb_min (start + *entries_गिनती, entries_गिनती_2);
      *entries_गिनती = end - start;

      क्रम (अचिन्हित पूर्णांक i = 0; i < *entries_गिनती; i++) {
	अचिन्हित पूर्णांक j = start + i;

	hb_position_t max_height;
	अगर (j == height_गिनती) {
	  max_height = INT32_MAX;
	} अन्यथा {
	  max_height = correctionHeight[j].get_y_मूल्य (font, यह);
	}

	kern_entries[i] = {max_height, kern_मूल्य[j].get_x_मूल्य (font, यह)};
      }
    }
    वापस entries_गिनती_2;
  }

  रक्षित:
  HBUINT16	height_गिनती;
  UnsizedArrayOf<Math_मूल्य_Record>
		math_मूल्य_Records_Z;
				/* Array of correction heights at
				 * which the kern value changes.
				 * Sorted by the height value in
				 * design units (heightCount entries),
				 * Followed by:
				 * Array of kern values corresponding
				 * to heights. (heightCount+1 entries).
				 */

  खुला:
  DEFINE_आकार_ARRAY (2, math_मूल्य_Records_Z);
};

काष्ठा MathKernInfoRecord
{
  MathKernInfoRecord* copy (hb_serialize_context_t *c, स्थिर व्योम *base,
                            स्थिर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *variation_map) स्थिर
  {
    TRACE_SERIALIZE (यह);
    स्वतः *out = c->embed (यह);
    अगर (unlikely (!out)) return_trace (nullptr);

    अचिन्हित गिनती = ARRAY_लंबाई (mathKern);
    क्रम (अचिन्हित i = 0; i < गिनती; i++)
      out->mathKern[i].serialize_copy (c, mathKern[i], base, 0, hb_serialize_context_t::शीर्ष, variation_map);

    return_trace (out);
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c, स्थिर व्योम *base) स्थिर
  {
    क्रम (स्थिर स्वतः &offset : mathKern)
      (base+offset).collect_variation_indices (c);
  }

  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम *base) स्थिर
  {
    TRACE_SANITIZE (यह);

    अचिन्हित पूर्णांक गिनती = ARRAY_लंबाई (mathKern);
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
      अगर (unlikely (!mathKern[i].sanitize (c, base)))
	return_trace (false);

    return_trace (true);
  }

  hb_position_t get_kerning (hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font,
			     स्थिर व्योम *base) स्थिर
  {
    अचिन्हित पूर्णांक idx = kern;
    अगर (unlikely (idx >= ARRAY_लंबाई (mathKern))) वापस 0;
    वापस (base+mathKern[idx]).get_मूल्य (correction_height, font);
  }

  अचिन्हित पूर्णांक get_kernings (hb_ot_math_kern_t kern,
			     अचिन्हित पूर्णांक start_offset,
			     अचिन्हित पूर्णांक *entries_गिनती, /* IN/OUT */
			     hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			     hb_font_t *font,
			     स्थिर व्योम *base) स्थिर
  {
    अचिन्हित पूर्णांक idx = kern;
    अगर (unlikely (idx >= ARRAY_लंबाई (mathKern)) || !mathKern[idx]) {
      अगर (entries_गिनती) *entries_गिनती = 0;
      वापस 0;
    }
    वापस (base+mathKern[idx]).get_entries (start_offset,
					     entries_गिनती,
					     kern_entries,
					     font);
  }

  रक्षित:
  /* Offset to MathKern table for each corner -
   * from the beginning of MathKernInfo table.  May be NULL. */
  Offset16To<MathKern> mathKern[4];

  खुला:
  DEFINE_आकार_STATIC (8);
};

काष्ठा MathKernInfo
{
  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्थिर hb_set_t &glyphset = c->plan->_glyphset_mathed;
    स्थिर hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (यह+mathKernCoverage, mathKernInfoRecords)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->mathKernInfoRecords, यह, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->mathKernCoverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (true);
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर
  {
    क्रम (स्थिर स्वतः &entry : hb_zip (यह+mathKernCoverage, mathKernInfoRecords))
      अगर (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, यह);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  mathKernCoverage.sanitize (c, यह) &&
		  mathKernInfoRecords.sanitize (c, यह));
  }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font) स्थिर
  {
    अचिन्हित पूर्णांक सूचकांक_2 = (यह+mathKernCoverage).get_coverage (glyph);
    वापस mathKernInfoRecords[सूचकांक_2].get_kerning (kern, correction_height, font, यह);
  }

  अचिन्हित पूर्णांक get_kernings (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     अचिन्हित पूर्णांक start_offset,
			     अचिन्हित पूर्णांक *entries_गिनती, /* IN/OUT */
			     hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			     hb_font_t *font) स्थिर
  {
    अचिन्हित पूर्णांक सूचकांक_2 = (यह+mathKernCoverage).get_coverage (glyph);
    वापस mathKernInfoRecords[सूचकांक_2].get_kernings (kern,
						    start_offset,
						    entries_गिनती,
						    kern_entries,
						    font,
						    यह);
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

  खुला:
  DEFINE_आकार_ARRAY (4, mathKernInfoRecords);
};

काष्ठा MathGlyphInfo
{
  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->embed (*यह);
    अगर (unlikely (!out)) return_trace (false);

    out->mathItalicsCorrectionInfo.serialize_subset (c, mathItalicsCorrectionInfo, यह);
    out->math_शीर्ष_Accent_Attachment.serialize_subset (c, math_शीर्ष_Accent_Attachment, यह);

    स्थिर hb_set_t &glyphset = c->plan->_glyphset_mathed;
    स्थिर hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः it =
    + hb_iter (यह+extendedShapeCoverage)
    | hb_take (c->plan->source->get_अंक_glyphs ())
    | hb_filter (glyphset)
    | hb_map_retains_sorting (glyph_map)
    ;

    अगर (it) out->extendedShapeCoverage.serialize_serialize (c->serializer, it);
    अन्यथा out->extendedShapeCoverage = 0;

    out->mathKernInfo.serialize_subset (c, mathKernInfo, यह);
    return_trace (true);
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर
  {
    (यह+mathItalicsCorrectionInfo).collect_variation_indices (c);
    (यह+math_शीर्ष_Accent_Attachment).collect_variation_indices (c);
    (यह+mathKernInfo).collect_variation_indices (c);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  mathItalicsCorrectionInfo.sanitize (c, यह) &&
		  math_शीर्ष_Accent_Attachment.sanitize (c, यह) &&
		  extendedShapeCoverage.sanitize (c, यह) &&
		  mathKernInfo.sanitize (c, यह));
  }

  hb_position_t
  get_italics_correction (hb_codepoint_t  glyph, hb_font_t *font) स्थिर
  { वापस (यह+mathItalicsCorrectionInfo).get_मूल्य (glyph, font); }

  hb_position_t
  get_शीर्ष_accent_attachment (hb_codepoint_t  glyph, hb_font_t *font) स्थिर
  { वापस (यह+math_शीर्ष_Accent_Attachment).get_मूल्य (glyph, font); }

  bool is_extended_shape (hb_codepoint_t glyph) स्थिर
  { वापस (यह+extendedShapeCoverage).get_coverage (glyph) != NOT_COVERED; }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font) स्थिर
  { वापस (यह+mathKernInfo).get_kerning (glyph, kern, correction_height, font); }

  hb_position_t get_kernings (hb_codepoint_t glyph,
			      hb_ot_math_kern_t kern,
			      अचिन्हित पूर्णांक start_offset,
			      अचिन्हित पूर्णांक *entries_गिनती, /* IN/OUT */
			      hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			      hb_font_t *font) स्थिर
  { वापस (यह+mathKernInfo).get_kernings (glyph,
					     kern,
					     start_offset,
					     entries_गिनती,
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

  खुला:
  DEFINE_आकार_STATIC (8);
};

काष्ठा MathGlyphVariantRecord
{
  मित्र काष्ठा MathGlyphConstruction;

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->embed (यह);
    अगर (unlikely (!out)) return_trace (false);

    स्थिर hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_assign (out->variantGlyph, glyph_map.लो (variantGlyph), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  व्योम depend (hb_depend_तथ्य_builder_t *depend_तथ्य, अचिन्हित source) स्थिर
  { depend_तथ्य->add_depend(source, HB_OT_TAG_MATH, variantGlyph); }

  व्योम closure_glyphs (hb_set_t *variant_glyphs) स्थिर
  { variant_glyphs->add (variantGlyph); }

  रक्षित:
  HBGlyphID16 variantGlyph;       /* Glyph ID for the variant. */
  HBUINT16  advanceMeasurement; /* Advance width/height, in design units, of the
				 * variant, in the direction of requested
				 * glyph extension. */

  खुला:
  DEFINE_आकार_STATIC (4);
};

काष्ठा PartFlags : HBUINT16
{
  क्रमागत Flags {
    Extender	= 0x0001u, /* If set, the part can be skipped or repeated. */

    Defined	= 0x0001u, /* All defined flags. */
  };

  खुला:
  DEFINE_आकार_STATIC (2);
};

काष्ठा MathGlyphPartRecord
{
  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->embed (यह);
    अगर (unlikely (!out)) return_trace (false);

    स्थिर hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_assign (out->glyph, glyph_map.लो (glyph), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  व्योम extract (hb_ot_math_glyph_part_t &out,
		int64_t mult,
		hb_font_t *font) स्थिर
  {
    out.glyph			= glyph;

    out.start_connector_लंबाई	= font->em_mult (start_Connector_लंबाई, mult);
    out.end_connector_लंबाई	= font->em_mult (end_Connector_लंबाई, mult);
    out.full_advance		= font->em_mult (fullAdvance, mult);

    static_assert ((अचिन्हित पूर्णांक) HB_OT_MATH_GLYPH_PART_FLAG_EXTENDER ==
		   (अचिन्हित पूर्णांक) PartFlags::Extender, "");

    out.flags = (hb_ot_math_glyph_part_flags_t)
		(अचिन्हित पूर्णांक)
		(partFlags & PartFlags::Defined);
  }

  व्योम depend (hb_depend_तथ्य_builder_t *depend_तथ्य, अचिन्हित source) स्थिर
  { depend_तथ्य->add_depend(source, HB_OT_TAG_MATH, glyph); }

  व्योम closure_glyphs (hb_set_t *variant_glyphs) स्थिर
  { variant_glyphs->add (glyph); }

  रक्षित:
  HBGlyphID16	glyph;		/* Glyph ID for the part. */
  HBUINT16	start_Connector_लंबाई;
				/* Advance width/ height of the straight bar
				 * connector material, in design units, is at
				 * the beginning of the glyph, in the
				 * direction of the extension. */
  HBUINT16	end_Connector_लंबाई;
				/* Advance width/ height of the straight bar
				 * connector material, in design units, is at
				 * the end of the glyph, in the direction of
				 * the extension. */
  HBUINT16	fullAdvance;	/* Full advance width/height for this part,
				 * in the direction of the extension.
				 * In design units. */
  PartFlags	partFlags;	/* Part qualifiers. */

  खुला:
  DEFINE_आकार_STATIC (10);
};

काष्ठा MathGlyphAssembly
{
  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);

    अगर (!c->serializer->copy (italicsCorrection, यह, &c->plan->layout_variation_idx_delta_map)) return_trace (false);
    अगर (!c->serializer->copy<HBUINT16> (partRecords.len)) return_trace (false);

    क्रम (स्थिर स्वतः& record : partRecords.iter ())
      अगर (!record.subset (c)) return_trace (false);
    return_trace (true);
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर
  {
    italicsCorrection.collect_variation_indices (c, यह);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  italicsCorrection.sanitize (c, यह) &&
		  partRecords.sanitize (c));
  }

  अचिन्हित पूर्णांक get_parts (hb_direction_t direction,
			  hb_font_t *font,
			  अचिन्हित पूर्णांक start_offset,
			  अचिन्हित पूर्णांक *parts_गिनती, /* IN/OUT */
			  hb_ot_math_glyph_part_t *parts /* OUT */,
			  hb_position_t *italics_correction /* OUT */) स्थिर
  {
    अगर (parts_गिनती && parts)
    {
      int64_t mult = font->dir_mult (direction);
      क्रम (स्वतः _ : hb_zip (partRecords.as_array ().sub_array (start_offset, parts_गिनती),
			    hb_array (parts, *parts_गिनती)))
	_.first.extract (_.second, mult, font);
    }

    अगर (italics_correction)
      *italics_correction = italicsCorrection.get_x_मूल्य (font, यह);

    वापस partRecords.len;
  }

  व्योम depend (hb_depend_तथ्य_builder_t *depend_तथ्य, अचिन्हित source) स्थिर
  {
    क्रम (स्थिर स्वतः& _ : partRecords.iter ())
      _.depend (depend_तथ्य, source);
  }

  व्योम closure_glyphs (hb_set_t *variant_glyphs) स्थिर
  {
    क्रम (स्थिर स्वतः& _ : partRecords.iter ())
      _.closure_glyphs (variant_glyphs);
  }

  रक्षित:
  Math_मूल्य_Record
		italicsCorrection;
				/* Italics correction of this
				 * MathGlyphAssembly. Should not
				 * depend on the assembly size. */
  Array16Of<MathGlyphPartRecord>
		partRecords;	/* Array of part records, from
				 * left to right and bottom to
				 * top. */

  खुला:
  DEFINE_आकार_ARRAY (6, partRecords);
};

काष्ठा MathGlyphConstruction
{
  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    out->glyphAssembly.serialize_subset (c, glyphAssembly, यह);

    अगर (!c->serializer->check_assign (out->mathGlyphVariantRecord.len, mathGlyphVariantRecord.len, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (false);
    क्रम (स्थिर स्वतः& record : mathGlyphVariantRecord.iter ())
      अगर (!record.subset (c)) return_trace (false);

    return_trace (true);
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर
  {
    (यह+glyphAssembly).collect_variation_indices (c);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  glyphAssembly.sanitize (c, यह) &&
		  mathGlyphVariantRecord.sanitize (c));
  }

  स्थिर MathGlyphAssembly &get_assembly () स्थिर { वापस यह+glyphAssembly; }

  अचिन्हित पूर्णांक get_variants (hb_direction_t direction,
			     hb_font_t *font,
			     अचिन्हित पूर्णांक start_offset,
			     अचिन्हित पूर्णांक *variants_गिनती, /* IN/OUT */
			     hb_ot_math_glyph_variant_t *variants /* OUT */) स्थिर
  {
    अगर (variants_गिनती && variants)
    {
      int64_t mult = font->dir_mult (direction);
      क्रम (स्वतः _ : hb_zip (mathGlyphVariantRecord.as_array ().sub_array (start_offset, variants_गिनती),
			    hb_array (variants, *variants_गिनती)))
	_.second = {_.first.variantGlyph, font->em_mult (_.first.advanceMeasurement, mult)};
    }
    वापस mathGlyphVariantRecord.len;
  }

  व्योम depend (hb_depend_तथ्य_builder_t *depend_तथ्य, अचिन्हित source) स्थिर
  {
    (यह+glyphAssembly).depend (depend_तथ्य, source);

    क्रम (स्थिर स्वतः& _ : mathGlyphVariantRecord.iter ())
      _.depend (depend_तथ्य, source);
  }

  व्योम closure_glyphs (hb_set_t *variant_glyphs) स्थिर
  {
    (यह+glyphAssembly).closure_glyphs (variant_glyphs);

    क्रम (स्थिर स्वतः& _ : mathGlyphVariantRecord.iter ())
      _.closure_glyphs (variant_glyphs);
  }

  रक्षित:
  /* Offset to MathGlyphAssembly table for this shape - from the beginning of
     MathGlyphConstruction table.  May be NULL. */
  Offset16To<MathGlyphAssembly>	  glyphAssembly;

  /* MathGlyphVariantRecords for alternative variants of the glyphs. */
  Array16Of<MathGlyphVariantRecord> mathGlyphVariantRecord;

  खुला:
  DEFINE_आकार_ARRAY (4, mathGlyphVariantRecord);
};

काष्ठा MathVariants
{
  व्योम depend (hb_depend_तथ्य_builder_t *depend_तथ्य) स्थिर
  {
    स्थिर hb_array_t<स्थिर Offset16To<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vert_Glyph_गिनती + horiz_Glyph_गिनती);

    अगर (vertGlyphCoverage)
    {
      स्थिर स्वतः vert_offsets = glyph_construction_offsets.sub_array (0, vert_Glyph_गिनती);
      + hb_zip (यह+vertGlyphCoverage, vert_offsets)
      | hb_apply ([&] (स्थिर hb_pair_t<hb_codepoint_t, स्थिर Offset16To<MathGlyphConstruction>&> &_)
                  {
                    स्थिर MathGlyphConstruction &mgc = यह+_.second;
                    mgc.depend (depend_तथ्य, _.first);
                  })
      ;
    }
    अगर (horizGlyphCoverage)
    {
      स्थिर स्वतः hori_offsets = glyph_construction_offsets.sub_array (vert_Glyph_गिनती, horiz_Glyph_गिनती);
      + hb_zip (यह+horizGlyphCoverage, hori_offsets)
      | hb_apply ([&] (स्थिर hb_pair_t<hb_codepoint_t, स्थिर Offset16To<MathGlyphConstruction>&> &_)
                  {
                    स्थिर MathGlyphConstruction &mgc = यह+_.second;
                    mgc.depend (depend_तथ्य, _.first);
                  })
      ;
    }
  }

  व्योम closure_glyphs (स्थिर hb_set_t *glyph_set,
                       hb_set_t *variant_glyphs) स्थिर
  {
    स्थिर hb_array_t<स्थिर Offset16To<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vert_Glyph_गिनती + horiz_Glyph_गिनती);

    अगर (vertGlyphCoverage)
    {
      स्थिर स्वतः vert_offsets = glyph_construction_offsets.sub_array (0, vert_Glyph_गिनती);
      + hb_zip (यह+vertGlyphCoverage, vert_offsets)
      | hb_filter (glyph_set, hb_first)
      | hb_map (hb_second)
      | hb_map (hb_add (यह))
      | hb_apply ([=] (स्थिर MathGlyphConstruction &_) { _.closure_glyphs (variant_glyphs); })
      ;
    }

    अगर (horizGlyphCoverage)
    {
      स्थिर स्वतः hori_offsets = glyph_construction_offsets.sub_array (vert_Glyph_गिनती, horiz_Glyph_गिनती);
      + hb_zip (यह+horizGlyphCoverage, hori_offsets)
      | hb_filter (glyph_set, hb_first)
      | hb_map (hb_second)
      | hb_map (hb_add (यह))
      | hb_apply ([=] (स्थिर MathGlyphConstruction &_) { _.closure_glyphs (variant_glyphs); })
      ;
    }
  }

  व्योम collect_coverage_and_indices (hb_sorted_vector_t<hb_codepoint_t>& new_coverage,
                                     स्थिर Offset16To<Coverage>& coverage,
                                     अचिन्हित i,
                                     अचिन्हित end_सूचकांक,
                                     hb_set_t& indices,
                                     स्थिर hb_set_t& glyphset,
                                     स्थिर hb_map_t& glyph_map) स्थिर
  {
    अगर (!coverage) वापस;

    क्रम (स्थिर स्वतः _ : (यह+coverage).iter ())
    {
      अगर (i >= end_सूचकांक) वापस;
      अगर (glyphset.has (_))
      {
        अचिन्हित new_gid = glyph_map.लो (_);
        new_coverage.push (new_gid);
        indices.add (i);
      }
      i++;
    }
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्थिर hb_set_t &glyphset = c->plan->_glyphset_cmaped;
    स्थिर hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);
    अगर (!c->serializer->check_assign (out->minConnectorOverlap, minConnectorOverlap, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (false);

    hb_sorted_vector_t<hb_codepoint_t> new_vert_coverage;
    hb_sorted_vector_t<hb_codepoint_t> new_hori_coverage;
    hb_set_t indices;
    collect_coverage_and_indices (new_vert_coverage, vertGlyphCoverage, 0, vert_Glyph_गिनती, indices, glyphset, glyph_map);
    collect_coverage_and_indices (new_hori_coverage, horizGlyphCoverage, vert_Glyph_गिनती, vert_Glyph_गिनती + horiz_Glyph_गिनती, indices, glyphset, glyph_map);

    अगर (!c->serializer->check_assign (out->vert_Glyph_गिनती, new_vert_coverage.लंबाई, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (false);
    अगर (!c->serializer->check_assign (out->horiz_Glyph_गिनती, new_hori_coverage.लंबाई, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (false);

    क्रम (अचिन्हित i : indices.iter ())
    {
      स्वतः *o = c->serializer->embed (glyphConstruction[i]);
      अगर (!o) return_trace (false);
      o->serialize_subset (c, glyphConstruction[i], यह);
    }

    अगर (new_vert_coverage)
      out->vertGlyphCoverage.serialize_serialize (c->serializer, new_vert_coverage.iter ());

    अगर (new_hori_coverage)
    out->horizGlyphCoverage.serialize_serialize (c->serializer, new_hori_coverage.iter ());
    return_trace (true);
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर
  {
    क्रम (स्थिर स्वतः &entry : hb_zip (यह+vertGlyphCoverage, glyphConstruction.as_array (vert_Glyph_गिनती)))
      अगर (c->glyph_set->has (hb_first (entry)))
        (यह+hb_second (entry)).collect_variation_indices (c);
    क्रम (स्थिर स्वतः &entry : hb_zip (यह+horizGlyphCoverage, glyphConstruction.as_array (vert_Glyph_गिनती + horiz_Glyph_गिनती).sub_array (vert_Glyph_गिनती)))
      अगर (c->glyph_set->has (hb_first (entry)))
        (यह+hb_second (entry)).collect_variation_indices (c);
  }

  bool sanitize_offsets (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अचिन्हित पूर्णांक गिनती = vert_Glyph_गिनती + horiz_Glyph_गिनती;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
      अगर (!glyphConstruction.arrayZ[i].sanitize (c, यह)) return_trace (false);
    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  vertGlyphCoverage.sanitize (c, यह) &&
		  horizGlyphCoverage.sanitize (c, यह) &&
		  hb_barrier () &&
		  c->check_array (glyphConstruction.arrayZ, vert_Glyph_गिनती + horiz_Glyph_गिनती) &&
		  sanitize_offsets (c));
  }

  hb_position_t get_min_connector_overlap (hb_direction_t direction,
						  hb_font_t *font) स्थिर
  { वापस font->em_scale_dir (minConnectorOverlap, direction); }

  अचिन्हित पूर्णांक get_glyph_variants (hb_codepoint_t glyph,
				   hb_direction_t direction,
				   hb_font_t *font,
				   अचिन्हित पूर्णांक start_offset,
				   अचिन्हित पूर्णांक *variants_गिनती, /* IN/OUT */
				   hb_ot_math_glyph_variant_t *variants /* OUT */) स्थिर
  { वापस get_glyph_construction (glyph, direction, font)
	   .get_variants (direction, font, start_offset, variants_गिनती, variants); }

  अचिन्हित पूर्णांक get_glyph_parts (hb_codepoint_t glyph,
				hb_direction_t direction,
				hb_font_t *font,
				अचिन्हित पूर्णांक start_offset,
				अचिन्हित पूर्णांक *parts_गिनती, /* IN/OUT */
				hb_ot_math_glyph_part_t *parts /* OUT */,
				hb_position_t *italics_correction /* OUT */) स्थिर
  { वापस get_glyph_construction (glyph, direction, font)
	   .get_assembly ()
	   .get_parts (direction, font,
		       start_offset, parts_गिनती, parts,
		       italics_correction); }

  निजी:
  स्थिर MathGlyphConstruction &
  get_glyph_construction (hb_codepoint_t glyph,
			  hb_direction_t direction,
			  hb_font_t *font HB_UNUSED) स्थिर
  {
    bool vertical = HB_DIRECTION_IS_VERTICAL (direction);
    अचिन्हित पूर्णांक गिनती = vertical ? vert_Glyph_गिनती : horiz_Glyph_गिनती;
    स्थिर Offset16To<Coverage> &coverage = vertical ? vertGlyphCoverage
						  : horizGlyphCoverage;

    अचिन्हित पूर्णांक सूचकांक_2 = (यह+coverage).get_coverage (glyph);
    अगर (unlikely (सूचकांक_2 >= गिनती)) वापस Null (MathGlyphConstruction);

    अगर (!vertical)
      सूचकांक_2 += vert_Glyph_गिनती;

    वापस यह+glyphConstruction[सूचकांक_2];
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
  HBUINT16	vert_Glyph_गिनती;	/* Number of glyphs for which
				 * information is provided for
				 * vertically growing variants. */
  HBUINT16	horiz_Glyph_गिनती;/* Number of glyphs for which
				 * information is provided for
				 * horizontally growing variants. */

  /* Array of offsets to MathGlyphConstruction tables - from the beginning of
     the MathVariants table, for shapes growing in vertical/horizontal
     direction. */
  UnsizedArrayOf<Offset16To<MathGlyphConstruction>>
			glyphConstruction;

  खुला:
  DEFINE_आकार_ARRAY (10, glyphConstruction);
};


/*
 * MATH -- Mathematical typesetting
 * https://docs.microsoft.com/en-us/typography/opentype/spec/math
 */

काष्ठा MATH
{
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_MATH;

  bool has_तथ्य () स्थिर { वापस version.to_int (); }

  व्योम depend (hb_depend_तथ्य_builder_t *depend_तथ्य) स्थिर
  {
    अगर (mathVariants)
      (यह+mathVariants).depend (depend_तथ्य);
  }

  व्योम closure_glyphs (hb_set_t *glyph_set) स्थिर
  {
    अगर (mathVariants)
    {
      hb_set_t variant_glyphs;
      (यह+mathVariants).closure_glyphs (glyph_set, &variant_glyphs);
      hb_set_union (glyph_set, &variant_glyphs);
    }
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->embed (*यह);
    अगर (unlikely (!out)) return_trace (false);

    out->mathConstants.serialize_copy (c->serializer, mathConstants, यह, 0, hb_serialize_context_t::शीर्ष, &c->plan->layout_variation_idx_delta_map);
    out->mathGlyphInfo.serialize_subset (c, mathGlyphInfo, यह);
    out->mathVariants.serialize_subset (c, mathVariants, यह);
    return_trace (true);
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c, स्थिर hb_set_t *variant_glyph_set) स्थिर
  {
    (यह+mathConstants).collect_variation_indices (c);
    (यह+mathGlyphInfo).collect_variation_indices (c);
    स्वतः *glyph_set = c->glyph_set;
    c->glyph_set = variant_glyph_set;
    (यह+mathVariants).collect_variation_indices (c);
    c->glyph_set = glyph_set;
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (version.sanitize (c) &&
		  likely (version.major == 1) &&
		  hb_barrier () &&
		  mathConstants.sanitize (c, यह) &&
		  mathGlyphInfo.sanitize (c, यह) &&
		  mathVariants.sanitize (c, यह));
  }

  // https://github.com/harfbuzz/harfbuzz/issues/4653
  HB_INTERNAL bool is_bad_cambria (hb_font_t *font) स्थिर
  {
#अगर_अघोषित HB_NO_MATH
    चयन HB_CODEPOINT_ENCODE3 (font->face->table.MATH.get_blob ()->लंबाई,
                                 (यह+mathConstants).minHeight[1], // displayOperatorMinHeight
                                 (यह+mathConstants).minHeight[0]) // delimitedSubFormulaMinHeight
    {
      /* sha1sum:ab4a4fe054d23061f3c039493d6f665cfda2ecf5  cambria.ttc
       * sha1sum:086855301bff644f9d8827b88491fcf73a6d4cb9  cambria.ttc
       * sha1sum:b1e5a3feaca2ea3dfcf79ccb377de749ecf60343  cambria.ttc */
      हाल HB_CODEPOINT_ENCODE3 (25722, 2500, 3000):
        वापस true;
    }
#पूर्ण_अगर
    वापस false;
  }

  hb_position_t get_constant (hb_ot_math_constant_t  constant,
			      hb_font_t		   *font) स्थिर
  { वापस (यह+mathConstants).get_मूल्य (constant, font); }

  स्थिर MathGlyphInfo &get_glyph_info () स्थिर { वापस यह+mathGlyphInfo; }

  स्थिर MathVariants &get_variants () स्थिर    { वापस यह+mathVariants; }

  रक्षित:
  FixedVersion<>version;	/* Version of the MATH table
				 * initially set to 0x00010000u */
  Offset16To<MathConstants>
		mathConstants;	/* MathConstants table */
  Offset16To<MathGlyphInfo>
		mathGlyphInfo;	/* MathGlyphInfo table */
  Offset16To<MathVariants>
		mathVariants;	/* MathVariants table */

  खुला:
  DEFINE_आकार_STATIC (10);
};

} /* namespace OT */


#पूर्ण_अगर /* HB_OT_MATH_TABLE_HH */

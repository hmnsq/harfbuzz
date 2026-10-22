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

#અન્તર્ભાવ "hb-open-type.hh"
#અન્તર્ભાવ "hb-ot-layout-common.hh"
#અન્તર્ભાવ "hb-ot-layout-gdef-table.hh"
#અન્તર્ભાવ "hb-ot-math.h"
#અન્તર્ભાવ "hb-depend-data.hh"

નામાકાશ OT {


સંરચના Math_કિંમત_Record
{
  hb_position_t get_x_કિંમત (hb_font_t *font, અચલ શૂન્ય *base) અચલ
  { પાછા hb_saturate_add (font->em_scale_x (value), (base+deviceTable).get_x_delta (font, get_var_store (font, base))); }
  hb_position_t get_y_કિંમત (hb_font_t *font, અચલ શૂન્ય *base) અચલ
  { પાછા hb_saturate_add (font->em_scale_y (value), (base+deviceTable).get_y_delta (font, get_var_store (font, base))); }

  અચલ ItemVariationStore &get_var_store (hb_font_t *font, અચલ શૂન્ય *base) અચલ
  {
#જો !defined(HB_NO_VAR) && !defined(HB_NO_OT_LAYOUT)
    જો ((base+deviceTable).is_variation_device ())
      પાછા font->face->table.GDEF->table->get_var_store ();
#endif
    પાછા Null (ItemVariationStore);
  }

  Math_કિંમત_Record* copy (hb_serialize_context_t *c, અચલ શૂન્ય *base,
                         અચલ hb_hashmap_t<unsigned, hb_pair_t<unsigned, પૂર્ણાંક>> *variation_map) અચલ
  {
    TRACE_SERIALIZE (આ);
    auto *out = c->embed (આ);
    જો (unlikely (!out)) return_trace (nullptr);
    hb_pair_t<unsigned, પૂર્ણાંક> *varidx_delta;
    જો (variation_map->has ((base+deviceTable).get_variation_index (), &varidx_delta) &&
        !c->check_assign (out->value, (int64_t) value + hb_second (*varidx_delta),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (nullptr);
    out->deviceTable.serialize_copy (c, deviceTable, base, 0, hb_serialize_context_t::Head, variation_map);

    return_trace (out);
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c, અચલ શૂન્ય *base) અચલ
  { (base+deviceTable).collect_variation_indices (c); }

  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ શૂન્ય *base) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) && deviceTable.sanitize (c, base));
  }

  સુરક્ષિત:
  HBINT16		value;		/* The X or Y value in design units */
  Offset16To<Device>	deviceTable;	/* Offset to the device table - from the
					 * beginning of parent table.  May be NULL.
					 * Suggested format for device table is 1. */

  જાહેર:
  DEFINE_SIZE_STATIC (4);
};

સંરચના MathConstants
{
  friend સંરચના MATH;

  MathConstants* copy (hb_serialize_context_t *c,
                      અચલ hb_hashmap_t<unsigned, hb_pair_t<unsigned, પૂર્ણાંક>> *variation_map) અચલ
  {
    TRACE_SERIALIZE (આ);
    auto *out = c->start_embed (આ);

    HBINT16 *p = c->allocate_size<HBINT16> (HBINT16::static_size * 2);
    જો (unlikely (!p)) return_trace (nullptr);
    hb_memcpy (p, percentScaleDown, HBINT16::static_size * 2);

    HBUINT16 *m = c->allocate_size<HBUINT16> (HBUINT16::static_size * 2);
    જો (unlikely (!m)) return_trace (nullptr);
    hb_memcpy (m, minHeight, HBUINT16::static_size * 2);

    unsigned ગણતરી = ARRAY_LENGTH (math_કિંમત_Records);
    માટે (unsigned i = 0; i < ગણતરી; i++)
      જો (!c->copy (math_કિંમત_Records[i], આ, variation_map))
        return_trace (nullptr);

    જો (!c->embed (radicalDegreeBottomRaisePercent)) return_trace (nullptr);
    return_trace (out);
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c) અચલ
  {
    માટે (અચલ auto &record : math_કિંમત_Records)
      record.collect_variation_indices (c, આ);
  }

  તાર્કિક sanitize_math_કિંમત_records (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);

    unsigned પૂર્ણાંક ગણતરી = ARRAY_LENGTH (math_કિંમત_Records);
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
      જો (!math_કિંમત_Records[i].sanitize (c, આ))
	return_trace (ખોટું);

    return_trace (સાચું);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) && sanitize_math_કિંમત_records (c));
  }

  hb_position_t get_કિંમત (hb_ot_math_constant_t constant,
			   hb_font_t *font) અચલ
  {
    પસંદગી (constant) {

    સ્થિતિ HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_DOWN:
    સ્થિતિ HB_OT_MATH_CONSTANT_SCRIPT_SCRIPT_PERCENT_SCALE_DOWN:
      પાછા percentScaleDown[constant - HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_DOWN];

    સ્થિતિ HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_HEIGHT:
    સ્થિતિ HB_OT_MATH_CONSTANT_DISPLAY_OPERATOR_MIN_HEIGHT:
      પાછા font->em_scale_y (minHeight[constant - HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_HEIGHT]);

    સ્થિતિ HB_OT_MATH_CONSTANT_RADICAL_KERN_AFTER_DEGREE:
    સ્થિતિ HB_OT_MATH_CONSTANT_RADICAL_KERN_BEFORE_DEGREE:
    સ્થિતિ HB_OT_MATH_CONSTANT_SKEWED_FRACTION_HORIZONTAL_GAP:
    સ્થિતિ HB_OT_MATH_CONSTANT_SPACE_AFTER_SCRIPT:
      પાછા math_કિંમત_Records[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_x_કિંમત (font, આ);

    સ્થિતિ HB_OT_MATH_CONSTANT_ACCENT_BASE_HEIGHT:
    સ્થિતિ HB_OT_MATH_CONSTANT_AXIS_HEIGHT:
    સ્થિતિ HB_OT_MATH_CONSTANT_FLATTENED_ACCENT_BASE_HEIGHT:
    સ્થિતિ HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_DISPLAY_STYLE_SHIFT_DOWN:
    સ્થિતિ HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_GAP_MIN:
    સ્થિતિ HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_SHIFT_DOWN:
    સ્થિતિ HB_OT_MATH_CONSTANT_FRACTION_DENOM_DISPLAY_STYLE_GAP_MIN:
    સ્થિતિ HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_DISPLAY_STYLE_SHIFT_UP:
    સ્થિતિ HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_GAP_MIN:
    સ્થિતિ HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_SHIFT_UP:
    સ્થિતિ HB_OT_MATH_CONSTANT_FRACTION_અંક_DISPLAY_STYLE_GAP_MIN:
    સ્થિતિ HB_OT_MATH_CONSTANT_FRACTION_RULE_THICKNESS:
    સ્થિતિ HB_OT_MATH_CONSTANT_LOWER_LIMIT_BASELINE_DROP_MIN:
    સ્થિતિ HB_OT_MATH_CONSTANT_LOWER_LIMIT_GAP_MIN:
    સ્થિતિ HB_OT_MATH_CONSTANT_MATH_LEADING:
    સ્થિતિ HB_OT_MATH_CONSTANT_OVERBAR_EXTRA_ASCENDER:
    સ્થિતિ HB_OT_MATH_CONSTANT_OVERBAR_RULE_THICKNESS:
    સ્થિતિ HB_OT_MATH_CONSTANT_OVERBAR_VERTICAL_GAP:
    સ્થિતિ HB_OT_MATH_CONSTANT_RADICAL_DISPLAY_STYLE_VERTICAL_GAP:
    સ્થિતિ HB_OT_MATH_CONSTANT_RADICAL_EXTRA_ASCENDER:
    સ્થિતિ HB_OT_MATH_CONSTANT_RADICAL_RULE_THICKNESS:
    સ્થિતિ HB_OT_MATH_CONSTANT_RADICAL_VERTICAL_GAP:
    સ્થિતિ HB_OT_MATH_CONSTANT_SKEWED_FRACTION_VERTICAL_GAP:
    સ્થિતિ HB_OT_MATH_CONSTANT_ઢગલો_BOTTOM_DISPLAY_STYLE_SHIFT_DOWN:
    સ્થિતિ HB_OT_MATH_CONSTANT_ઢગલો_BOTTOM_SHIFT_DOWN:
    સ્થિતિ HB_OT_MATH_CONSTANT_ઢગલો_DISPLAY_STYLE_GAP_MIN:
    સ્થિતિ HB_OT_MATH_CONSTANT_ઢગલો_GAP_MIN:
    સ્થિતિ HB_OT_MATH_CONSTANT_ઢગલો_ટોચ_DISPLAY_STYLE_SHIFT_UP:
    સ્થિતિ HB_OT_MATH_CONSTANT_ઢગલો_ટોચ_SHIFT_UP:
    સ્થિતિ HB_OT_MATH_CONSTANT_STRETCH_ઢગલો_BOTTOM_SHIFT_DOWN:
    સ્થિતિ HB_OT_MATH_CONSTANT_STRETCH_ઢગલો_GAP_ABOVE_MIN:
    સ્થિતિ HB_OT_MATH_CONSTANT_STRETCH_ઢગલો_GAP_BELOW_MIN:
    સ્થિતિ HB_OT_MATH_CONSTANT_STRETCH_ઢગલો_ટોચ_SHIFT_UP:
    સ્થિતિ HB_OT_MATH_CONSTANT_SUBSCRIPT_BASELINE_DROP_MIN:
    સ્થિતિ HB_OT_MATH_CONSTANT_SUBSCRIPT_SHIFT_DOWN:
    સ્થિતિ HB_OT_MATH_CONSTANT_SUBSCRIPT_ટોચ_MAX:
    સ્થિતિ HB_OT_MATH_CONSTANT_SUB_SUPERSCRIPT_GAP_MIN:
    સ્થિતિ HB_OT_MATH_CONSTANT_SUPERSCRIPT_BASELINE_DROP_MAX:
    સ્થિતિ HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MAX_WITH_SUBSCRIPT:
    સ્થિતિ HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MIN:
    સ્થિતિ HB_OT_MATH_CONSTANT_SUPERSCRIPT_SHIFT_UP:
    સ્થિતિ HB_OT_MATH_CONSTANT_SUPERSCRIPT_SHIFT_UP_CRAMPED:
    સ્થિતિ HB_OT_MATH_CONSTANT_UNDERBAR_EXTRA_DESCENDER:
    સ્થિતિ HB_OT_MATH_CONSTANT_UNDERBAR_RULE_THICKNESS:
    સ્થિતિ HB_OT_MATH_CONSTANT_UNDERBAR_VERTICAL_GAP:
    સ્થિતિ HB_OT_MATH_CONSTANT_UPPER_LIMIT_BASELINE_RISE_MIN:
    સ્થિતિ HB_OT_MATH_CONSTANT_UPPER_LIMIT_GAP_MIN:
      પાછા math_કિંમત_Records[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_y_કિંમત (font, આ);

    સ્થિતિ HB_OT_MATH_CONSTANT_RADICAL_DEGREE_BOTTOM_RAISE_PERCENT:
      પાછા radicalDegreeBottomRaisePercent;

    મૂળભૂત:
      પાછા 0;
    }
  }

  સુરક્ષિત:
  HBINT16 percentScaleDown[2];
  HBUINT16 minHeight[2];
  Math_કિંમત_Record math_કિંમત_Records[51];
  HBINT16 radicalDegreeBottomRaisePercent;

  જાહેર:
  DEFINE_SIZE_STATIC (214);
};

સંરચના MathItalicsCorrectionInfo
{
  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    અચલ hb_set_t &glyphset = c->plan->_glyphset_mathed;
    અચલ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (આ+coverage, italicsCorrection)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->italicsCorrection, આ, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (સાચું);
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c) અચલ
  {
    માટે (અચલ auto &entry : hb_zip (આ+coverage, italicsCorrection))
      જો (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, આ);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  coverage.sanitize (c, આ) &&
		  italicsCorrection.sanitize (c, આ));
  }

  hb_position_t get_કિંમત (hb_codepoint_t glyph,
			   hb_font_t *font) અચલ
  {
    unsigned પૂર્ણાંક index = (આ+coverage).get_coverage (glyph);
    પાછા italicsCorrection[index].get_x_કિંમત (font, આ);
  }

  સુરક્ષિત:
  Offset16To<Coverage>       coverage;		/* Offset to Coverage table -
						 * from the beginning of
						 * MathItalicsCorrectionInfo
						 * table. */
  Array16Of<Math_કિંમત_Record> italicsCorrection;	/* Array of MathValueRecords
						 * defining italics correction
						 * values for each
						 * covered glyph. */

  જાહેર:
  DEFINE_SIZE_ARRAY (4, italicsCorrection);
};

સંરચના Math_ટોચ_Accent_Attachment
{
  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    અચલ hb_set_t &glyphset = c->plan->_glyphset_mathed;
    અચલ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (આ+ટોચ_Accent_Coverage, ટોચ_Accent_Attachment)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->ટોચ_Accent_Attachment, આ, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->ટોચ_Accent_Coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (સાચું);
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c) અચલ
  {
    માટે (અચલ auto &entry : hb_zip (આ+ટોચ_Accent_Coverage, ટોચ_Accent_Attachment))
      જો (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, આ);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  ટોચ_Accent_Coverage.sanitize (c, આ) &&
		  ટોચ_Accent_Attachment.sanitize (c, આ));
  }

  hb_position_t get_કિંમત (hb_codepoint_t glyph,
			   hb_font_t *font) અચલ
  {
    unsigned પૂર્ણાંક index = (આ+ટોચ_Accent_Coverage).get_coverage (glyph);
    જો (index == NOT_COVERED)
      પાછા font->get_glyph_h_advance (glyph) / 2;
    પાછા ટોચ_Accent_Attachment[index].get_x_કિંમત (font, આ);
  }

  સુરક્ષિત:
  Offset16To<Coverage>       ટોચ_Accent_Coverage;   /* Offset to Coverage table -
						 * from the beginning of
						 * MathTopAccentAttachment
						 * table. */
  Array16Of<Math_કિંમત_Record> ટોચ_Accent_Attachment; /* Array of MathValueRecords
						 * defining top accent
						 * attachment points for each
						 * covered glyph. */

  જાહેર:
  DEFINE_SIZE_ARRAY (2 + 2, ટોચ_Accent_Attachment);
};

સંરચના MathKern
{
  MathKern* copy (hb_serialize_context_t *c,
                      અચલ hb_hashmap_t<unsigned, hb_pair_t<unsigned, પૂર્ણાંક>> *variation_map) અચલ
  {
    TRACE_SERIALIZE (આ);
    auto *out = c->start_embed (આ);

    જો (unlikely (!c->embed (height_ગણતરી))) return_trace (nullptr);

    unsigned ગણતરી = 2 * height_ગણતરી + 1;
    માટે (unsigned i = 0; i < ગણતરી; i++)
      જો (!c->copy (math_કિંમત_Records_Z.arrayZ[i], આ, variation_map))
        return_trace (nullptr);

    return_trace (out);
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c) અચલ
  {
    માટે (અચલ auto &record : math_કિંમત_Records_Z.as_array (2 * height_ગણતરી + 1))
      record.collect_variation_indices (c, આ);
  }

  તાર્કિક sanitize_math_કિંમત_records (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    unsigned પૂર્ણાંક ગણતરી = 2 * height_ગણતરી + 1;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
      જો (!math_કિંમત_Records_Z.arrayZ[i].sanitize (c, આ)) return_trace (ખોટું);
    return_trace (સાચું);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  hb_barrier () &&
		  c->check_array (math_કિંમત_Records_Z.arrayZ, 2 * height_ગણતરી + 1) &&
		  sanitize_math_કિંમત_records (c));
  }

  hb_position_t get_કિંમત (hb_position_t correction_height, hb_font_t *font) અચલ
  {
    અચલ Math_કિંમત_Record* correctionHeight = math_કિંમત_Records_Z.arrayZ;
    અચલ Math_કિંમત_Record* kern_કિંમત = math_કિંમત_Records_Z.arrayZ + height_ગણતરી;
    પૂર્ણાંક sign = font->y_scale < 0 ? -1 : +1;

    /* According to OpenType spec (v1.9), except for the boundary cases, the index
     * chosen for kern value should be i such that
     *    correctionHeight[i-1] <= correction_height < correctionHeight[i]
     * We can use the binary search algorithm of std::upper_bound(). Or, we can
     * use the internal hb_bsearch_impl.
     */
    unsigned પૂર્ણાંક pos;
    auto cmp = +[](અચલ શૂન્ય* key, અચલ શૂન્ય* p,
                   પૂર્ણાંક sign, hb_font_t* font, અચલ MathKern* mathKern) -> પૂર્ણાંક {
      hb_position_t a = *(hb_position_t *) key;
      hb_position_t b = ((Math_કિંમત_Record *) p)->get_y_કિંમત (font, mathKern);
      જો (a == b) પાછા 0;
      પાછા sign > 0 ? (a < b ? -1 : +1) : (b < a ? -1 : +1);
    };
    unsigned પૂર્ણાંક i = hb_bsearch_impl(&pos, correction_height, correctionHeight,
                                     height_ગણતરી, Math_કિંમત_Record::static_size,
                                     cmp, sign, font, આ) ? pos + 1 : pos;
    પાછા kern_કિંમત[i].get_x_કિંમત (font, આ);
  }

  unsigned પૂર્ણાંક get_entries (unsigned પૂર્ણાંક start_offset,
			    unsigned પૂર્ણાંક *entries_ગણતરી, /* IN/OUT */
			    hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			    hb_font_t *font) અચલ
  {
    અચલ Math_કિંમત_Record* correctionHeight = math_કિંમત_Records_Z.arrayZ;
    અચલ Math_કિંમત_Record* kern_કિંમત = math_કિંમત_Records_Z.arrayZ + height_ગણતરી;
    અચલ unsigned પૂર્ણાંક entries_ગણતરી_2 = height_ગણતરી + 1;

    જો (entries_ગણતરી && kern_entries)
    {
      unsigned પૂર્ણાંક start = hb_min (start_offset, entries_ગણતરી_2);
      unsigned પૂર્ણાંક end = hb_min (start + *entries_ગણતરી, entries_ગણતરી_2);
      *entries_ગણતરી = end - start;

      માટે (unsigned પૂર્ણાંક i = 0; i < *entries_ગણતરી; i++) {
	unsigned પૂર્ણાંક j = start + i;

	hb_position_t max_height;
	જો (j == height_ગણતરી) {
	  max_height = INT32_MAX;
	} નહીંતર {
	  max_height = correctionHeight[j].get_y_કિંમત (font, આ);
	}

	kern_entries[i] = {max_height, kern_કિંમત[j].get_x_કિંમત (font, આ)};
      }
    }
    પાછા entries_ગણતરી_2;
  }

  સુરક્ષિત:
  HBUINT16	height_ગણતરી;
  UnsizedArrayOf<Math_કિંમત_Record>
		math_કિંમત_Records_Z;
				/* Array of correction heights at
				 * which the kern value changes.
				 * Sorted by the height value in
				 * design units (heightCount entries),
				 * Followed by:
				 * Array of kern values corresponding
				 * to heights. (heightCount+1 entries).
				 */

  જાહેર:
  DEFINE_SIZE_ARRAY (2, math_કિંમત_Records_Z);
};

સંરચના MathKernInfoRecord
{
  MathKernInfoRecord* copy (hb_serialize_context_t *c, અચલ શૂન્ય *base,
                            અચલ hb_hashmap_t<unsigned, hb_pair_t<unsigned, પૂર્ણાંક>> *variation_map) અચલ
  {
    TRACE_SERIALIZE (આ);
    auto *out = c->embed (આ);
    જો (unlikely (!out)) return_trace (nullptr);

    unsigned ગણતરી = ARRAY_LENGTH (mathKern);
    માટે (unsigned i = 0; i < ગણતરી; i++)
      out->mathKern[i].serialize_copy (c, mathKern[i], base, 0, hb_serialize_context_t::Head, variation_map);

    return_trace (out);
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c, અચલ શૂન્ય *base) અચલ
  {
    માટે (અચલ auto &offset : mathKern)
      (base+offset).collect_variation_indices (c);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ શૂન્ય *base) અચલ
  {
    TRACE_SANITIZE (આ);

    unsigned પૂર્ણાંક ગણતરી = ARRAY_LENGTH (mathKern);
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
      જો (unlikely (!mathKern[i].sanitize (c, base)))
	return_trace (ખોટું);

    return_trace (સાચું);
  }

  hb_position_t get_kerning (hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font,
			     અચલ શૂન્ય *base) અચલ
  {
    unsigned પૂર્ણાંક idx = kern;
    જો (unlikely (idx >= ARRAY_LENGTH (mathKern))) પાછા 0;
    પાછા (base+mathKern[idx]).get_કિંમત (correction_height, font);
  }

  unsigned પૂર્ણાંક get_kernings (hb_ot_math_kern_t kern,
			     unsigned પૂર્ણાંક start_offset,
			     unsigned પૂર્ણાંક *entries_ગણતરી, /* IN/OUT */
			     hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			     hb_font_t *font,
			     અચલ શૂન્ય *base) અચલ
  {
    unsigned પૂર્ણાંક idx = kern;
    જો (unlikely (idx >= ARRAY_LENGTH (mathKern)) || !mathKern[idx]) {
      જો (entries_ગણતરી) *entries_ગણતરી = 0;
      પાછા 0;
    }
    પાછા (base+mathKern[idx]).get_entries (start_offset,
					     entries_ગણતરી,
					     kern_entries,
					     font);
  }

  સુરક્ષિત:
  /* Offset to MathKern table for each corner -
   * from the beginning of MathKernInfo table.  May be NULL. */
  Offset16To<MathKern> mathKern[4];

  જાહેર:
  DEFINE_SIZE_STATIC (8);
};

સંરચના MathKernInfo
{
  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    અચલ hb_set_t &glyphset = c->plan->_glyphset_mathed;
    અચલ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (આ+mathKernCoverage, mathKernInfoRecords)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->mathKernInfoRecords, આ, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->mathKernCoverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (સાચું);
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c) અચલ
  {
    માટે (અચલ auto &entry : hb_zip (આ+mathKernCoverage, mathKernInfoRecords))
      જો (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, આ);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  mathKernCoverage.sanitize (c, આ) &&
		  mathKernInfoRecords.sanitize (c, આ));
  }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font) અચલ
  {
    unsigned પૂર્ણાંક index = (આ+mathKernCoverage).get_coverage (glyph);
    પાછા mathKernInfoRecords[index].get_kerning (kern, correction_height, font, આ);
  }

  unsigned પૂર્ણાંક get_kernings (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     unsigned પૂર્ણાંક start_offset,
			     unsigned પૂર્ણાંક *entries_ગણતરી, /* IN/OUT */
			     hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			     hb_font_t *font) અચલ
  {
    unsigned પૂર્ણાંક index = (આ+mathKernCoverage).get_coverage (glyph);
    પાછા mathKernInfoRecords[index].get_kernings (kern,
						    start_offset,
						    entries_ગણતરી,
						    kern_entries,
						    font,
						    આ);
  }

  સુરક્ષિત:
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

  જાહેર:
  DEFINE_SIZE_ARRAY (4, mathKernInfoRecords);
};

સંરચના MathGlyphInfo
{
  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->embed (*આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    out->mathItalicsCorrectionInfo.serialize_subset (c, mathItalicsCorrectionInfo, આ);
    out->math_ટોચ_Accent_Attachment.serialize_subset (c, math_ટોચ_Accent_Attachment, આ);

    અચલ hb_set_t &glyphset = c->plan->_glyphset_mathed;
    અચલ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto it =
    + hb_iter (આ+extendedShapeCoverage)
    | hb_take (c->plan->source->get_અંક_glyphs ())
    | hb_filter (glyphset)
    | hb_map_retains_sorting (glyph_map)
    ;

    જો (it) out->extendedShapeCoverage.serialize_serialize (c->serializer, it);
    નહીંતર out->extendedShapeCoverage = 0;

    out->mathKernInfo.serialize_subset (c, mathKernInfo, આ);
    return_trace (સાચું);
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c) અચલ
  {
    (આ+mathItalicsCorrectionInfo).collect_variation_indices (c);
    (આ+math_ટોચ_Accent_Attachment).collect_variation_indices (c);
    (આ+mathKernInfo).collect_variation_indices (c);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  mathItalicsCorrectionInfo.sanitize (c, આ) &&
		  math_ટોચ_Accent_Attachment.sanitize (c, આ) &&
		  extendedShapeCoverage.sanitize (c, આ) &&
		  mathKernInfo.sanitize (c, આ));
  }

  hb_position_t
  get_italics_correction (hb_codepoint_t  glyph, hb_font_t *font) અચલ
  { પાછા (આ+mathItalicsCorrectionInfo).get_કિંમત (glyph, font); }

  hb_position_t
  get_ટોચ_accent_attachment (hb_codepoint_t  glyph, hb_font_t *font) અચલ
  { પાછા (આ+math_ટોચ_Accent_Attachment).get_કિંમત (glyph, font); }

  તાર્કિક is_extended_shape (hb_codepoint_t glyph) અચલ
  { પાછા (આ+extendedShapeCoverage).get_coverage (glyph) != NOT_COVERED; }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font) અચલ
  { પાછા (આ+mathKernInfo).get_kerning (glyph, kern, correction_height, font); }

  hb_position_t get_kernings (hb_codepoint_t glyph,
			      hb_ot_math_kern_t kern,
			      unsigned પૂર્ણાંક start_offset,
			      unsigned પૂર્ણાંક *entries_ગણતરી, /* IN/OUT */
			      hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			      hb_font_t *font) અચલ
  { પાછા (આ+mathKernInfo).get_kernings (glyph,
					     kern,
					     start_offset,
					     entries_ગણતરી,
					     kern_entries,
					     font); }

  સુરક્ષિત:
  /* Offset to MathItalicsCorrectionInfo table -
   * from the beginning of MathGlyphInfo table. */
  Offset16To<MathItalicsCorrectionInfo> mathItalicsCorrectionInfo;

  /* Offset to MathTopAccentAttachment table -
   * from the beginning of MathGlyphInfo table. */
  Offset16To<Math_ટોચ_Accent_Attachment> math_ટોચ_Accent_Attachment;

  /* Offset to coverage table for Extended Shape glyphs -
   * from the beginning of MathGlyphInfo table. When the left or right glyph of
   * a box is an extended shape variant, the (ink) box (and not the default
   * position defined by values in MathConstants table) should be used for
   * vertical positioning purposes.  May be NULL.. */
  Offset16To<Coverage> extendedShapeCoverage;

   /* Offset to MathKernInfo table -
    * from the beginning of MathGlyphInfo table. */
  Offset16To<MathKernInfo> mathKernInfo;

  જાહેર:
  DEFINE_SIZE_STATIC (8);
};

સંરચના MathGlyphVariantRecord
{
  friend સંરચના MathGlyphConstruction;

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->embed (આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    અચલ hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_assign (out->variantGlyph, glyph_map.get (variantGlyph), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  શૂન્ય depend (hb_depend_data_builder_t *depend_data, unsigned source) અચલ
  { depend_data->add_depend(source, HB_OT_TAG_MATH, variantGlyph); }

  શૂન્ય closure_glyphs (hb_set_t *variant_glyphs) અચલ
  { variant_glyphs->add (variantGlyph); }

  સુરક્ષિત:
  HBGlyphID16 variantGlyph;       /* Glyph ID for the variant. */
  HBUINT16  advanceMeasurement; /* Advance width/height, in design units, of the
				 * variant, in the direction of requested
				 * glyph extension. */

  જાહેર:
  DEFINE_SIZE_STATIC (4);
};

સંરચના PartFlags : HBUINT16
{
  ગણના Flags {
    Extender	= 0x0001u, /* If set, the part can be skipped or repeated. */

    Defined	= 0x0001u, /* All defined flags. */
  };

  જાહેર:
  DEFINE_SIZE_STATIC (2);
};

સંરચના MathGlyphPartRecord
{
  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->embed (આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    અચલ hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_assign (out->glyph, glyph_map.get (glyph), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  શૂન્ય extract (hb_ot_math_glyph_part_t &out,
		int64_t mult,
		hb_font_t *font) અચલ
  {
    out.glyph			= glyph;

    out.start_connector_length	= font->em_mult (startConnectorLength, mult);
    out.end_connector_length	= font->em_mult (endConnectorLength, mult);
    out.full_advance		= font->em_mult (fullAdvance, mult);

    static_assert ((unsigned પૂર્ણાંક) HB_OT_MATH_GLYPH_PART_FLAG_EXTENDER ==
		   (unsigned પૂર્ણાંક) PartFlags::Extender, "");

    out.flags = (hb_ot_math_glyph_part_flags_t)
		(unsigned પૂર્ણાંક)
		(partFlags & PartFlags::Defined);
  }

  શૂન્ય depend (hb_depend_data_builder_t *depend_data, unsigned source) અચલ
  { depend_data->add_depend(source, HB_OT_TAG_MATH, glyph); }

  શૂન્ય closure_glyphs (hb_set_t *variant_glyphs) અચલ
  { variant_glyphs->add (glyph); }

  સુરક્ષિત:
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

  જાહેર:
  DEFINE_SIZE_STATIC (10);
};

સંરચના MathGlyphAssembly
{
  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);

    જો (!c->serializer->copy (italicsCorrection, આ, &c->plan->layout_variation_idx_delta_map)) return_trace (ખોટું);
    જો (!c->serializer->copy<HBUINT16> (partRecords.len)) return_trace (ખોટું);

    માટે (અચલ auto& record : partRecords.iter ())
      જો (!record.subset (c)) return_trace (ખોટું);
    return_trace (સાચું);
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c) અચલ
  {
    italicsCorrection.collect_variation_indices (c, આ);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  italicsCorrection.sanitize (c, આ) &&
		  partRecords.sanitize (c));
  }

  unsigned પૂર્ણાંક get_parts (hb_direction_t direction,
			  hb_font_t *font,
			  unsigned પૂર્ણાંક start_offset,
			  unsigned પૂર્ણાંક *parts_ગણતરી, /* IN/OUT */
			  hb_ot_math_glyph_part_t *parts /* OUT */,
			  hb_position_t *italics_correction /* OUT */) અચલ
  {
    જો (parts_ગણતરી && parts)
    {
      int64_t mult = font->dir_mult (direction);
      માટે (auto _ : hb_zip (partRecords.as_array ().sub_array (start_offset, parts_ગણતરી),
			    hb_array (parts, *parts_ગણતરી)))
	_.first.extract (_.second, mult, font);
    }

    જો (italics_correction)
      *italics_correction = italicsCorrection.get_x_કિંમત (font, આ);

    પાછા partRecords.len;
  }

  શૂન્ય depend (hb_depend_data_builder_t *depend_data, unsigned source) અચલ
  {
    માટે (અચલ auto& _ : partRecords.iter ())
      _.depend (depend_data, source);
  }

  શૂન્ય closure_glyphs (hb_set_t *variant_glyphs) અચલ
  {
    માટે (અચલ auto& _ : partRecords.iter ())
      _.closure_glyphs (variant_glyphs);
  }

  સુરક્ષિત:
  Math_કિંમત_Record
		italicsCorrection;
				/* Italics correction of this
				 * MathGlyphAssembly. Should not
				 * depend on the assembly size. */
  Array16Of<MathGlyphPartRecord>
		partRecords;	/* Array of part records, from
				 * left to right and bottom to
				 * top. */

  જાહેર:
  DEFINE_SIZE_ARRAY (6, partRecords);
};

સંરચના MathGlyphConstruction
{
  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);

    out->glyphAssembly.serialize_subset (c, glyphAssembly, આ);

    જો (!c->serializer->check_assign (out->mathGlyphVariantRecord.len, mathGlyphVariantRecord.len, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (ખોટું);
    માટે (અચલ auto& record : mathGlyphVariantRecord.iter ())
      જો (!record.subset (c)) return_trace (ખોટું);

    return_trace (સાચું);
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c) અચલ
  {
    (આ+glyphAssembly).collect_variation_indices (c);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  glyphAssembly.sanitize (c, આ) &&
		  mathGlyphVariantRecord.sanitize (c));
  }

  અચલ MathGlyphAssembly &get_assembly () અચલ { પાછા આ+glyphAssembly; }

  unsigned પૂર્ણાંક get_variants (hb_direction_t direction,
			     hb_font_t *font,
			     unsigned પૂર્ણાંક start_offset,
			     unsigned પૂર્ણાંક *variants_ગણતરી, /* IN/OUT */
			     hb_ot_math_glyph_variant_t *variants /* OUT */) અચલ
  {
    જો (variants_ગણતરી && variants)
    {
      int64_t mult = font->dir_mult (direction);
      માટે (auto _ : hb_zip (mathGlyphVariantRecord.as_array ().sub_array (start_offset, variants_ગણતરી),
			    hb_array (variants, *variants_ગણતરી)))
	_.second = {_.first.variantGlyph, font->em_mult (_.first.advanceMeasurement, mult)};
    }
    પાછા mathGlyphVariantRecord.len;
  }

  શૂન્ય depend (hb_depend_data_builder_t *depend_data, unsigned source) અચલ
  {
    (આ+glyphAssembly).depend (depend_data, source);

    માટે (અચલ auto& _ : mathGlyphVariantRecord.iter ())
      _.depend (depend_data, source);
  }

  શૂન્ય closure_glyphs (hb_set_t *variant_glyphs) અચલ
  {
    (આ+glyphAssembly).closure_glyphs (variant_glyphs);

    માટે (અચલ auto& _ : mathGlyphVariantRecord.iter ())
      _.closure_glyphs (variant_glyphs);
  }

  સુરક્ષિત:
  /* Offset to MathGlyphAssembly table for this shape - from the beginning of
     MathGlyphConstruction table.  May be NULL. */
  Offset16To<MathGlyphAssembly>	  glyphAssembly;

  /* MathGlyphVariantRecords for alternative variants of the glyphs. */
  Array16Of<MathGlyphVariantRecord> mathGlyphVariantRecord;

  જાહેર:
  DEFINE_SIZE_ARRAY (4, mathGlyphVariantRecord);
};

સંરચના MathVariants
{
  શૂન્ય depend (hb_depend_data_builder_t *depend_data) અચલ
  {
    અચલ hb_array_t<અચલ Offset16To<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vert_Glyph_ગણતરી + horiz_Glyph_ગણતરી);

    જો (vertGlyphCoverage)
    {
      અચલ auto vert_offsets = glyph_construction_offsets.sub_array (0, vert_Glyph_ગણતરી);
      + hb_zip (આ+vertGlyphCoverage, vert_offsets)
      | hb_apply ([&] (અચલ hb_pair_t<hb_codepoint_t, અચલ Offset16To<MathGlyphConstruction>&> &_)
                  {
                    અચલ MathGlyphConstruction &mgc = આ+_.second;
                    mgc.depend (depend_data, _.first);
                  })
      ;
    }
    જો (horizGlyphCoverage)
    {
      અચલ auto hori_offsets = glyph_construction_offsets.sub_array (vert_Glyph_ગણતરી, horiz_Glyph_ગણતરી);
      + hb_zip (આ+horizGlyphCoverage, hori_offsets)
      | hb_apply ([&] (અચલ hb_pair_t<hb_codepoint_t, અચલ Offset16To<MathGlyphConstruction>&> &_)
                  {
                    અચલ MathGlyphConstruction &mgc = આ+_.second;
                    mgc.depend (depend_data, _.first);
                  })
      ;
    }
  }

  શૂન્ય closure_glyphs (અચલ hb_set_t *glyph_set,
                       hb_set_t *variant_glyphs) અચલ
  {
    અચલ hb_array_t<અચલ Offset16To<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vert_Glyph_ગણતરી + horiz_Glyph_ગણતરી);

    જો (vertGlyphCoverage)
    {
      અચલ auto vert_offsets = glyph_construction_offsets.sub_array (0, vert_Glyph_ગણતરી);
      + hb_zip (આ+vertGlyphCoverage, vert_offsets)
      | hb_filter (glyph_set, hb_first)
      | hb_map (hb_second)
      | hb_map (hb_add (આ))
      | hb_apply ([=] (અચલ MathGlyphConstruction &_) { _.closure_glyphs (variant_glyphs); })
      ;
    }

    જો (horizGlyphCoverage)
    {
      અચલ auto hori_offsets = glyph_construction_offsets.sub_array (vert_Glyph_ગણતરી, horiz_Glyph_ગણતરી);
      + hb_zip (આ+horizGlyphCoverage, hori_offsets)
      | hb_filter (glyph_set, hb_first)
      | hb_map (hb_second)
      | hb_map (hb_add (આ))
      | hb_apply ([=] (અચલ MathGlyphConstruction &_) { _.closure_glyphs (variant_glyphs); })
      ;
    }
  }

  શૂન્ય collect_coverage_and_indices (hb_sorted_vector_t<hb_codepoint_t>& new_coverage,
                                     અચલ Offset16To<Coverage>& coverage,
                                     unsigned i,
                                     unsigned end_index,
                                     hb_set_t& indices,
                                     અચલ hb_set_t& glyphset,
                                     અચલ hb_map_t& glyph_map) અચલ
  {
    જો (!coverage) પાછા;

    માટે (અચલ auto _ : (આ+coverage).iter ())
    {
      જો (i >= end_index) પાછા;
      જો (glyphset.has (_))
      {
        unsigned new_gid = glyph_map.get (_);
        new_coverage.push (new_gid);
        indices.add (i);
      }
      i++;
    }
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    અચલ hb_set_t &glyphset = c->plan->_glyphset_cmaped;
    અચલ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);
    જો (!c->serializer->check_assign (out->minConnectorOverlap, minConnectorOverlap, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (ખોટું);

    hb_sorted_vector_t<hb_codepoint_t> new_vert_coverage;
    hb_sorted_vector_t<hb_codepoint_t> new_hori_coverage;
    hb_set_t indices;
    collect_coverage_and_indices (new_vert_coverage, vertGlyphCoverage, 0, vert_Glyph_ગણતરી, indices, glyphset, glyph_map);
    collect_coverage_and_indices (new_hori_coverage, horizGlyphCoverage, vert_Glyph_ગણતરી, vert_Glyph_ગણતરી + horiz_Glyph_ગણતરી, indices, glyphset, glyph_map);

    જો (!c->serializer->check_assign (out->vert_Glyph_ગણતરી, new_vert_coverage.length, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (ખોટું);
    જો (!c->serializer->check_assign (out->horiz_Glyph_ગણતરી, new_hori_coverage.length, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (ખોટું);

    માટે (unsigned i : indices.iter ())
    {
      auto *o = c->serializer->embed (glyphConstruction[i]);
      જો (!o) return_trace (ખોટું);
      o->serialize_subset (c, glyphConstruction[i], આ);
    }

    જો (new_vert_coverage)
      out->vertGlyphCoverage.serialize_serialize (c->serializer, new_vert_coverage.iter ());

    જો (new_hori_coverage)
    out->horizGlyphCoverage.serialize_serialize (c->serializer, new_hori_coverage.iter ());
    return_trace (સાચું);
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c) અચલ
  {
    માટે (અચલ auto &entry : hb_zip (આ+vertGlyphCoverage, glyphConstruction.as_array (vert_Glyph_ગણતરી)))
      જો (c->glyph_set->has (hb_first (entry)))
        (આ+hb_second (entry)).collect_variation_indices (c);
    માટે (અચલ auto &entry : hb_zip (આ+horizGlyphCoverage, glyphConstruction.as_array (vert_Glyph_ગણતરી + horiz_Glyph_ગણતરી).sub_array (vert_Glyph_ગણતરી)))
      જો (c->glyph_set->has (hb_first (entry)))
        (આ+hb_second (entry)).collect_variation_indices (c);
  }

  તાર્કિક sanitize_offsets (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    unsigned પૂર્ણાંક ગણતરી = vert_Glyph_ગણતરી + horiz_Glyph_ગણતરી;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
      જો (!glyphConstruction.arrayZ[i].sanitize (c, આ)) return_trace (ખોટું);
    return_trace (સાચું);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  vertGlyphCoverage.sanitize (c, આ) &&
		  horizGlyphCoverage.sanitize (c, આ) &&
		  hb_barrier () &&
		  c->check_array (glyphConstruction.arrayZ, vert_Glyph_ગણતરી + horiz_Glyph_ગણતરી) &&
		  sanitize_offsets (c));
  }

  hb_position_t get_min_connector_overlap (hb_direction_t direction,
						  hb_font_t *font) અચલ
  { પાછા font->em_scale_dir (minConnectorOverlap, direction); }

  unsigned પૂર્ણાંક get_glyph_variants (hb_codepoint_t glyph,
				   hb_direction_t direction,
				   hb_font_t *font,
				   unsigned પૂર્ણાંક start_offset,
				   unsigned પૂર્ણાંક *variants_ગણતરી, /* IN/OUT */
				   hb_ot_math_glyph_variant_t *variants /* OUT */) અચલ
  { પાછા get_glyph_construction (glyph, direction, font)
	   .get_variants (direction, font, start_offset, variants_ગણતરી, variants); }

  unsigned પૂર્ણાંક get_glyph_parts (hb_codepoint_t glyph,
				hb_direction_t direction,
				hb_font_t *font,
				unsigned પૂર્ણાંક start_offset,
				unsigned પૂર્ણાંક *parts_ગણતરી, /* IN/OUT */
				hb_ot_math_glyph_part_t *parts /* OUT */,
				hb_position_t *italics_correction /* OUT */) અચલ
  { પાછા get_glyph_construction (glyph, direction, font)
	   .get_assembly ()
	   .get_parts (direction, font,
		       start_offset, parts_ગણતરી, parts,
		       italics_correction); }

  ખાનગી:
  અચલ MathGlyphConstruction &
  get_glyph_construction (hb_codepoint_t glyph,
			  hb_direction_t direction,
			  hb_font_t *font HB_UNUSED) અચલ
  {
    તાર્કિક vertical = HB_DIRECTION_IS_VERTICAL (direction);
    unsigned પૂર્ણાંક ગણતરી = vertical ? vert_Glyph_ગણતરી : horiz_Glyph_ગણતરી;
    અચલ Offset16To<Coverage> &coverage = vertical ? vertGlyphCoverage
						  : horizGlyphCoverage;

    unsigned પૂર્ણાંક index = (આ+coverage).get_coverage (glyph);
    જો (unlikely (index >= ગણતરી)) પાછા Null (MathGlyphConstruction);

    જો (!vertical)
      index += vert_Glyph_ગણતરી;

    પાછા આ+glyphConstruction[index];
  }

  સુરક્ષિત:
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
  HBUINT16	vert_Glyph_ગણતરી;	/* Number of glyphs for which
				 * information is provided for
				 * vertically growing variants. */
  HBUINT16	horiz_Glyph_ગણતરી;/* Number of glyphs for which
				 * information is provided for
				 * horizontally growing variants. */

  /* Array of offsets to MathGlyphConstruction tables - from the beginning of
     the MathVariants table, for shapes growing in vertical/horizontal
     direction. */
  UnsizedArrayOf<Offset16To<MathGlyphConstruction>>
			glyphConstruction;

  જાહેર:
  DEFINE_SIZE_ARRAY (10, glyphConstruction);
};


/*
 * MATH -- Mathematical typesetting
 * https://docs.microsoft.com/en-us/typography/opentype/spec/math
 */

સંરચના MATH
{
  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_MATH;

  તાર્કિક has_data () અચલ { પાછા version.to_int (); }

  શૂન્ય depend (hb_depend_data_builder_t *depend_data) અચલ
  {
    જો (mathVariants)
      (આ+mathVariants).depend (depend_data);
  }

  શૂન્ય closure_glyphs (hb_set_t *glyph_set) અચલ
  {
    જો (mathVariants)
    {
      hb_set_t variant_glyphs;
      (આ+mathVariants).closure_glyphs (glyph_set, &variant_glyphs);
      hb_set_union (glyph_set, &variant_glyphs);
    }
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->embed (*આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    out->mathConstants.serialize_copy (c->serializer, mathConstants, આ, 0, hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map);
    out->mathGlyphInfo.serialize_subset (c, mathGlyphInfo, આ);
    out->mathVariants.serialize_subset (c, mathVariants, આ);
    return_trace (સાચું);
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c, અચલ hb_set_t *variant_glyph_set) અચલ
  {
    (આ+mathConstants).collect_variation_indices (c);
    (આ+mathGlyphInfo).collect_variation_indices (c);
    auto *glyph_set = c->glyph_set;
    c->glyph_set = variant_glyph_set;
    (આ+mathVariants).collect_variation_indices (c);
    c->glyph_set = glyph_set;
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (version.sanitize (c) &&
		  likely (version.major == 1) &&
		  hb_barrier () &&
		  mathConstants.sanitize (c, આ) &&
		  mathGlyphInfo.sanitize (c, આ) &&
		  mathVariants.sanitize (c, આ));
  }

  // https://github.com/harfbuzz/harfbuzz/issues/4653
  HB_INTERNAL તાર્કિક is_bad_cambria (hb_font_t *font) અચલ
  {
#ifndef HB_NO_MATH
    પસંદગી HB_CODEPOINT_ENCODE3 (font->face->table.MATH.get_blob ()->length,
                                 (આ+mathConstants).minHeight[1], // displayOperatorMinHeight
                                 (આ+mathConstants).minHeight[0]) // delimitedSubFormulaMinHeight
    {
      /* sha1sum:ab4a4fe054d23061f3c039493d6f665cfda2ecf5  cambria.ttc
       * sha1sum:086855301bff644f9d8827b88491fcf73a6d4cb9  cambria.ttc
       * sha1sum:b1e5a3feaca2ea3dfcf79ccb377de749ecf60343  cambria.ttc */
      સ્થિતિ HB_CODEPOINT_ENCODE3 (25722, 2500, 3000):
        પાછા સાચું;
    }
#endif
    પાછા ખોટું;
  }

  hb_position_t get_constant (hb_ot_math_constant_t  constant,
			      hb_font_t		   *font) અચલ
  { પાછા (આ+mathConstants).get_કિંમત (constant, font); }

  અચલ MathGlyphInfo &get_glyph_info () અચલ { પાછા આ+mathGlyphInfo; }

  અચલ MathVariants &get_variants () અચલ    { પાછા આ+mathVariants; }

  સુરક્ષિત:
  FixedVersion<>version;	/* Version of the MATH table
				 * initially set to 0x00010000u */
  Offset16To<MathConstants>
		mathConstants;	/* MathConstants table */
  Offset16To<MathGlyphInfo>
		mathGlyphInfo;	/* MathGlyphInfo table */
  Offset16To<MathVariants>
		mathVariants;	/* MathVariants table */

  જાહેર:
  DEFINE_SIZE_STATIC (10);
};

} /* namespace OT */


#endif /* HB_OT_MATH_TABLE_HH */

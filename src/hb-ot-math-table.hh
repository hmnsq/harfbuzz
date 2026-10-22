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

#അന്തര്ഭാവ "hb-open-type.hh"
#അന്തര്ഭാവ "hb-ot-layout-common.hh"
#അന്തര്ഭാവ "hb-ot-layout-gdef-table.hh"
#അന്തര്ഭാവ "hb-ot-math.h"
#അന്തര്ഭാവ "hb-depend-data.hh"

നാമാകാശ OT {


ഘടന Math_മൂല്യം_Record
{
  hb_position_t get_x_മൂല്യം (hb_font_t *font, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  { തിരികെ hb_saturate_add (font->em_scale_x (value), (base+deviceTable).get_x_delta (font, get_var_store (font, base))); }
  hb_position_t get_y_മൂല്യം (hb_font_t *font, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  { തിരികെ hb_saturate_add (font->em_scale_y (value), (base+deviceTable).get_y_delta (font, get_var_store (font, base))); }

  സ്ഥിരം ItemVariationStore &get_var_store (hb_font_t *font, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
#എങ്കിൽ !defined(HB_NO_VAR) && !defined(HB_NO_OT_LAYOUT)
    എങ്കിൽ ((base+deviceTable).is_variation_device ())
      തിരികെ font->face->table.GDEF->table->get_var_store ();
#endif
    തിരികെ Null (ItemVariationStore);
  }

  Math_മൂല്യം_Record* copy (hb_serialize_context_t *c, സ്ഥിരം ശൂന്യം *base,
                         സ്ഥിരം hb_hashmap_t<unsigned, hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ>> *variation_map) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    auto *out = c->embed (ഇത്);
    എങ്കിൽ (unlikely (!out)) return_trace (nullptr);
    hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ> *varidx_delta;
    എങ്കിൽ (variation_map->has ((base+deviceTable).get_variation_index (), &varidx_delta) &&
        !c->check_assign (out->value, (int64_t) value + hb_second (*varidx_delta),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (nullptr);
    out->deviceTable.serialize_copy (c, deviceTable, base, 0, hb_serialize_context_t::Head, variation_map);

    return_trace (out);
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  { (base+deviceTable).collect_variation_indices (c); }

  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) && deviceTable.sanitize (c, base));
  }

  സംരക്ഷിത:
  HBINT16		value;		/* The X or Y value in design units */
  Offset16To<Device>	deviceTable;	/* Offset to the device table - from the
					 * beginning of parent table.  May be NULL.
					 * Suggested format for device table is 1. */

  പൊതു:
  DEFINE_SIZE_STATIC (4);
};

ഘടന MathConstants
{
  friend ഘടന MATH;

  MathConstants* copy (hb_serialize_context_t *c,
                      സ്ഥിരം hb_hashmap_t<unsigned, hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ>> *variation_map) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    auto *out = c->start_embed (ഇത്);

    HBINT16 *p = c->allocate_size<HBINT16> (HBINT16::static_size * 2);
    എങ്കിൽ (unlikely (!p)) return_trace (nullptr);
    hb_memcpy (p, percentScaleDown, HBINT16::static_size * 2);

    HBUINT16 *m = c->allocate_size<HBUINT16> (HBUINT16::static_size * 2);
    എങ്കിൽ (unlikely (!m)) return_trace (nullptr);
    hb_memcpy (m, minHeight, HBUINT16::static_size * 2);

    unsigned എണ്ണം = ARRAY_LENGTH (math_മൂല്യം_Records);
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
      എങ്കിൽ (!c->copy (math_മൂല്യം_Records[i], ഇത്, variation_map))
        return_trace (nullptr);

    എങ്കിൽ (!c->embed (radicalDegreeBottomRaisePercent)) return_trace (nullptr);
    return_trace (out);
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം auto &record : math_മൂല്യം_Records)
      record.collect_variation_indices (c, ഇത്);
  }

  തർക്കം sanitize_math_മൂല്യം_records (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);

    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = ARRAY_LENGTH (math_മൂല്യം_Records);
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
      എങ്കിൽ (!math_മൂല്യം_Records[i].sanitize (c, ഇത്))
	return_trace (അസത്യം);

    return_trace (സത്യം);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) && sanitize_math_മൂല്യം_records (c));
  }

  hb_position_t get_മൂല്യം (hb_ot_math_constant_t constant,
			   hb_font_t *font) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (constant) {

    അവസ്ഥ HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_DOWN:
    അവസ്ഥ HB_OT_MATH_CONSTANT_SCRIPT_SCRIPT_PERCENT_SCALE_DOWN:
      തിരികെ percentScaleDown[constant - HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_DOWN];

    അവസ്ഥ HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_HEIGHT:
    അവസ്ഥ HB_OT_MATH_CONSTANT_DISPLAY_OPERATOR_MIN_HEIGHT:
      തിരികെ font->em_scale_y (minHeight[constant - HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_HEIGHT]);

    അവസ്ഥ HB_OT_MATH_CONSTANT_RADICAL_KERN_AFTER_DEGREE:
    അവസ്ഥ HB_OT_MATH_CONSTANT_RADICAL_KERN_BEFORE_DEGREE:
    അവസ്ഥ HB_OT_MATH_CONSTANT_SKEWED_FRACTION_HORIZONTAL_GAP:
    അവസ്ഥ HB_OT_MATH_CONSTANT_SPACE_AFTER_SCRIPT:
      തിരികെ math_മൂല്യം_Records[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_x_മൂല്യം (font, ഇത്);

    അവസ്ഥ HB_OT_MATH_CONSTANT_ACCENT_BASE_HEIGHT:
    അവസ്ഥ HB_OT_MATH_CONSTANT_AXIS_HEIGHT:
    അവസ്ഥ HB_OT_MATH_CONSTANT_FLATTENED_ACCENT_BASE_HEIGHT:
    അവസ്ഥ HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_DISPLAY_STYLE_SHIFT_DOWN:
    അവസ്ഥ HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_GAP_MIN:
    അവസ്ഥ HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_SHIFT_DOWN:
    അവസ്ഥ HB_OT_MATH_CONSTANT_FRACTION_DENOM_DISPLAY_STYLE_GAP_MIN:
    അവസ്ഥ HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_DISPLAY_STYLE_SHIFT_UP:
    അവസ്ഥ HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_GAP_MIN:
    അവസ്ഥ HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_SHIFT_UP:
    അവസ്ഥ HB_OT_MATH_CONSTANT_FRACTION_അക്കം_DISPLAY_STYLE_GAP_MIN:
    അവസ്ഥ HB_OT_MATH_CONSTANT_FRACTION_RULE_THICKNESS:
    അവസ്ഥ HB_OT_MATH_CONSTANT_LOWER_LIMIT_BASELINE_DROP_MIN:
    അവസ്ഥ HB_OT_MATH_CONSTANT_LOWER_LIMIT_GAP_MIN:
    അവസ്ഥ HB_OT_MATH_CONSTANT_MATH_LEADING:
    അവസ്ഥ HB_OT_MATH_CONSTANT_OVERBAR_EXTRA_ASCENDER:
    അവസ്ഥ HB_OT_MATH_CONSTANT_OVERBAR_RULE_THICKNESS:
    അവസ്ഥ HB_OT_MATH_CONSTANT_OVERBAR_VERTICAL_GAP:
    അവസ്ഥ HB_OT_MATH_CONSTANT_RADICAL_DISPLAY_STYLE_VERTICAL_GAP:
    അവസ്ഥ HB_OT_MATH_CONSTANT_RADICAL_EXTRA_ASCENDER:
    അവസ്ഥ HB_OT_MATH_CONSTANT_RADICAL_RULE_THICKNESS:
    അവസ്ഥ HB_OT_MATH_CONSTANT_RADICAL_VERTICAL_GAP:
    അവസ്ഥ HB_OT_MATH_CONSTANT_SKEWED_FRACTION_VERTICAL_GAP:
    അവസ്ഥ HB_OT_MATH_CONSTANT_അട്ടി_BOTTOM_DISPLAY_STYLE_SHIFT_DOWN:
    അവസ്ഥ HB_OT_MATH_CONSTANT_അട്ടി_BOTTOM_SHIFT_DOWN:
    അവസ്ഥ HB_OT_MATH_CONSTANT_അട്ടി_DISPLAY_STYLE_GAP_MIN:
    അവസ്ഥ HB_OT_MATH_CONSTANT_അട്ടി_GAP_MIN:
    അവസ്ഥ HB_OT_MATH_CONSTANT_അട്ടി_മുകൾ_DISPLAY_STYLE_SHIFT_UP:
    അവസ്ഥ HB_OT_MATH_CONSTANT_അട്ടി_മുകൾ_SHIFT_UP:
    അവസ്ഥ HB_OT_MATH_CONSTANT_STRETCH_അട്ടി_BOTTOM_SHIFT_DOWN:
    അവസ്ഥ HB_OT_MATH_CONSTANT_STRETCH_അട്ടി_GAP_ABOVE_MIN:
    അവസ്ഥ HB_OT_MATH_CONSTANT_STRETCH_അട്ടി_GAP_BELOW_MIN:
    അവസ്ഥ HB_OT_MATH_CONSTANT_STRETCH_അട്ടി_മുകൾ_SHIFT_UP:
    അവസ്ഥ HB_OT_MATH_CONSTANT_SUBSCRIPT_BASELINE_DROP_MIN:
    അവസ്ഥ HB_OT_MATH_CONSTANT_SUBSCRIPT_SHIFT_DOWN:
    അവസ്ഥ HB_OT_MATH_CONSTANT_SUBSCRIPT_മുകൾ_MAX:
    അവസ്ഥ HB_OT_MATH_CONSTANT_SUB_SUPERSCRIPT_GAP_MIN:
    അവസ്ഥ HB_OT_MATH_CONSTANT_SUPERSCRIPT_BASELINE_DROP_MAX:
    അവസ്ഥ HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MAX_WITH_SUBSCRIPT:
    അവസ്ഥ HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MIN:
    അവസ്ഥ HB_OT_MATH_CONSTANT_SUPERSCRIPT_SHIFT_UP:
    അവസ്ഥ HB_OT_MATH_CONSTANT_SUPERSCRIPT_SHIFT_UP_CRAMPED:
    അവസ്ഥ HB_OT_MATH_CONSTANT_UNDERBAR_EXTRA_DESCENDER:
    അവസ്ഥ HB_OT_MATH_CONSTANT_UNDERBAR_RULE_THICKNESS:
    അവസ്ഥ HB_OT_MATH_CONSTANT_UNDERBAR_VERTICAL_GAP:
    അവസ്ഥ HB_OT_MATH_CONSTANT_UPPER_LIMIT_BASELINE_RISE_MIN:
    അവസ്ഥ HB_OT_MATH_CONSTANT_UPPER_LIMIT_GAP_MIN:
      തിരികെ math_മൂല്യം_Records[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_y_മൂല്യം (font, ഇത്);

    അവസ്ഥ HB_OT_MATH_CONSTANT_RADICAL_DEGREE_BOTTOM_RAISE_PERCENT:
      തിരികെ radicalDegreeBottomRaisePercent;

    സ്ഥിരസ്ഥിതി:
      തിരികെ 0;
    }
  }

  സംരക്ഷിത:
  HBINT16 percentScaleDown[2];
  HBUINT16 minHeight[2];
  Math_മൂല്യം_Record math_മൂല്യം_Records[51];
  HBINT16 radicalDegreeBottomRaisePercent;

  പൊതു:
  DEFINE_SIZE_STATIC (214);
};

ഘടന MathItalicsCorrectionInfo
{
  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    സ്ഥിരം hb_set_t &glyphset = c->plan->_glyphset_mathed;
    സ്ഥിരം hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (ഇത്+coverage, italicsCorrection)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->italicsCorrection, ഇത്, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (സത്യം);
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം auto &entry : hb_zip (ഇത്+coverage, italicsCorrection))
      എങ്കിൽ (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, ഇത്);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  coverage.sanitize (c, ഇത്) &&
		  italicsCorrection.sanitize (c, ഇത്));
  }

  hb_position_t get_മൂല്യം (hb_codepoint_t glyph,
			   hb_font_t *font) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ index = (ഇത്+coverage).get_coverage (glyph);
    തിരികെ italicsCorrection[index].get_x_മൂല്യം (font, ഇത്);
  }

  സംരക്ഷിത:
  Offset16To<Coverage>       coverage;		/* Offset to Coverage table -
						 * from the beginning of
						 * MathItalicsCorrectionInfo
						 * table. */
  Array16Of<Math_മൂല്യം_Record> italicsCorrection;	/* Array of MathValueRecords
						 * defining italics correction
						 * values for each
						 * covered glyph. */

  പൊതു:
  DEFINE_SIZE_ARRAY (4, italicsCorrection);
};

ഘടന Math_മുകൾ_Accent_Attachment
{
  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    സ്ഥിരം hb_set_t &glyphset = c->plan->_glyphset_mathed;
    സ്ഥിരം hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (ഇത്+മുകൾ_Accent_Coverage, മുകൾ_Accent_Attachment)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->മുകൾ_Accent_Attachment, ഇത്, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->മുകൾ_Accent_Coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (സത്യം);
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം auto &entry : hb_zip (ഇത്+മുകൾ_Accent_Coverage, മുകൾ_Accent_Attachment))
      എങ്കിൽ (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, ഇത്);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  മുകൾ_Accent_Coverage.sanitize (c, ഇത്) &&
		  മുകൾ_Accent_Attachment.sanitize (c, ഇത്));
  }

  hb_position_t get_മൂല്യം (hb_codepoint_t glyph,
			   hb_font_t *font) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ index = (ഇത്+മുകൾ_Accent_Coverage).get_coverage (glyph);
    എങ്കിൽ (index == NOT_COVERED)
      തിരികെ font->get_glyph_h_advance (glyph) / 2;
    തിരികെ മുകൾ_Accent_Attachment[index].get_x_മൂല്യം (font, ഇത്);
  }

  സംരക്ഷിത:
  Offset16To<Coverage>       മുകൾ_Accent_Coverage;   /* Offset to Coverage table -
						 * from the beginning of
						 * MathTopAccentAttachment
						 * table. */
  Array16Of<Math_മൂല്യം_Record> മുകൾ_Accent_Attachment; /* Array of MathValueRecords
						 * defining top accent
						 * attachment points for each
						 * covered glyph. */

  പൊതു:
  DEFINE_SIZE_ARRAY (2 + 2, മുകൾ_Accent_Attachment);
};

ഘടന MathKern
{
  MathKern* copy (hb_serialize_context_t *c,
                      സ്ഥിരം hb_hashmap_t<unsigned, hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ>> *variation_map) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    auto *out = c->start_embed (ഇത്);

    എങ്കിൽ (unlikely (!c->embed (height_എണ്ണം))) return_trace (nullptr);

    unsigned എണ്ണം = 2 * height_എണ്ണം + 1;
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
      എങ്കിൽ (!c->copy (math_മൂല്യം_Records_Z.arrayZ[i], ഇത്, variation_map))
        return_trace (nullptr);

    return_trace (out);
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം auto &record : math_മൂല്യം_Records_Z.as_array (2 * height_എണ്ണം + 1))
      record.collect_variation_indices (c, ഇത്);
  }

  തർക്കം sanitize_math_മൂല്യം_records (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = 2 * height_എണ്ണം + 1;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
      എങ്കിൽ (!math_മൂല്യം_Records_Z.arrayZ[i].sanitize (c, ഇത്)) return_trace (അസത്യം);
    return_trace (സത്യം);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  hb_barrier () &&
		  c->check_array (math_മൂല്യം_Records_Z.arrayZ, 2 * height_എണ്ണം + 1) &&
		  sanitize_math_മൂല്യം_records (c));
  }

  hb_position_t get_മൂല്യം (hb_position_t correction_height, hb_font_t *font) സ്ഥിരം
  {
    സ്ഥിരം Math_മൂല്യം_Record* correctionHeight = math_മൂല്യം_Records_Z.arrayZ;
    സ്ഥിരം Math_മൂല്യം_Record* kern_മൂല്യം = math_മൂല്യം_Records_Z.arrayZ + height_എണ്ണം;
    പൂർണ്ണസംഖ്യ sign = font->y_scale < 0 ? -1 : +1;

    /* According to OpenType spec (v1.9), except for the boundary cases, the index
     * chosen for kern value should be i such that
     *    correctionHeight[i-1] <= correction_height < correctionHeight[i]
     * We can use the binary search algorithm of std::upper_bound(). Or, we can
     * use the internal hb_bsearch_impl.
     */
    unsigned പൂർണ്ണസംഖ്യ pos;
    auto cmp = +[](സ്ഥിരം ശൂന്യം* key, സ്ഥിരം ശൂന്യം* p,
                   പൂർണ്ണസംഖ്യ sign, hb_font_t* font, സ്ഥിരം MathKern* mathKern) -> പൂർണ്ണസംഖ്യ {
      hb_position_t a = *(hb_position_t *) key;
      hb_position_t b = ((Math_മൂല്യം_Record *) p)->get_y_മൂല്യം (font, mathKern);
      എങ്കിൽ (a == b) തിരികെ 0;
      തിരികെ sign > 0 ? (a < b ? -1 : +1) : (b < a ? -1 : +1);
    };
    unsigned പൂർണ്ണസംഖ്യ i = hb_bsearch_impl(&pos, correction_height, correctionHeight,
                                     height_എണ്ണം, Math_മൂല്യം_Record::static_size,
                                     cmp, sign, font, ഇത്) ? pos + 1 : pos;
    തിരികെ kern_മൂല്യം[i].get_x_മൂല്യം (font, ഇത്);
  }

  unsigned പൂർണ്ണസംഖ്യ get_entries (unsigned പൂർണ്ണസംഖ്യ start_offset,
			    unsigned പൂർണ്ണസംഖ്യ *entries_എണ്ണം, /* IN/OUT */
			    hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			    hb_font_t *font) സ്ഥിരം
  {
    സ്ഥിരം Math_മൂല്യം_Record* correctionHeight = math_മൂല്യം_Records_Z.arrayZ;
    സ്ഥിരം Math_മൂല്യം_Record* kern_മൂല്യം = math_മൂല്യം_Records_Z.arrayZ + height_എണ്ണം;
    സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ entries_എണ്ണം_2 = height_എണ്ണം + 1;

    എങ്കിൽ (entries_എണ്ണം && kern_entries)
    {
      unsigned പൂർണ്ണസംഖ്യ start = hb_min (start_offset, entries_എണ്ണം_2);
      unsigned പൂർണ്ണസംഖ്യ end = hb_min (start + *entries_എണ്ണം, entries_എണ്ണം_2);
      *entries_എണ്ണം = end - start;

      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < *entries_എണ്ണം; i++) {
	unsigned പൂർണ്ണസംഖ്യ j = start + i;

	hb_position_t max_height;
	എങ്കിൽ (j == height_എണ്ണം) {
	  max_height = INT32_MAX;
	} അല്ലെങ്കിൽ {
	  max_height = correctionHeight[j].get_y_മൂല്യം (font, ഇത്);
	}

	kern_entries[i] = {max_height, kern_മൂല്യം[j].get_x_മൂല്യം (font, ഇത്)};
      }
    }
    തിരികെ entries_എണ്ണം_2;
  }

  സംരക്ഷിത:
  HBUINT16	height_എണ്ണം;
  UnsizedArrayOf<Math_മൂല്യം_Record>
		math_മൂല്യം_Records_Z;
				/* Array of correction heights at
				 * which the kern value changes.
				 * Sorted by the height value in
				 * design units (heightCount entries),
				 * Followed by:
				 * Array of kern values corresponding
				 * to heights. (heightCount+1 entries).
				 */

  പൊതു:
  DEFINE_SIZE_ARRAY (2, math_മൂല്യം_Records_Z);
};

ഘടന MathKernInfoRecord
{
  MathKernInfoRecord* copy (hb_serialize_context_t *c, സ്ഥിരം ശൂന്യം *base,
                            സ്ഥിരം hb_hashmap_t<unsigned, hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ>> *variation_map) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    auto *out = c->embed (ഇത്);
    എങ്കിൽ (unlikely (!out)) return_trace (nullptr);

    unsigned എണ്ണം = ARRAY_LENGTH (mathKern);
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
      out->mathKern[i].serialize_copy (c, mathKern[i], base, 0, hb_serialize_context_t::Head, variation_map);

    return_trace (out);
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം auto &offset : mathKern)
      (base+offset).collect_variation_indices (c);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);

    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = ARRAY_LENGTH (mathKern);
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
      എങ്കിൽ (unlikely (!mathKern[i].sanitize (c, base)))
	return_trace (അസത്യം);

    return_trace (സത്യം);
  }

  hb_position_t get_kerning (hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font,
			     സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ idx = kern;
    എങ്കിൽ (unlikely (idx >= ARRAY_LENGTH (mathKern))) തിരികെ 0;
    തിരികെ (base+mathKern[idx]).get_മൂല്യം (correction_height, font);
  }

  unsigned പൂർണ്ണസംഖ്യ get_kernings (hb_ot_math_kern_t kern,
			     unsigned പൂർണ്ണസംഖ്യ start_offset,
			     unsigned പൂർണ്ണസംഖ്യ *entries_എണ്ണം, /* IN/OUT */
			     hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			     hb_font_t *font,
			     സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ idx = kern;
    എങ്കിൽ (unlikely (idx >= ARRAY_LENGTH (mathKern)) || !mathKern[idx]) {
      എങ്കിൽ (entries_എണ്ണം) *entries_എണ്ണം = 0;
      തിരികെ 0;
    }
    തിരികെ (base+mathKern[idx]).get_entries (start_offset,
					     entries_എണ്ണം,
					     kern_entries,
					     font);
  }

  സംരക്ഷിത:
  /* Offset to MathKern table for each corner -
   * from the beginning of MathKernInfo table.  May be NULL. */
  Offset16To<MathKern> mathKern[4];

  പൊതു:
  DEFINE_SIZE_STATIC (8);
};

ഘടന MathKernInfo
{
  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    സ്ഥിരം hb_set_t &glyphset = c->plan->_glyphset_mathed;
    സ്ഥിരം hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (ഇത്+mathKernCoverage, mathKernInfoRecords)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->mathKernInfoRecords, ഇത്, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->mathKernCoverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (സത്യം);
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം auto &entry : hb_zip (ഇത്+mathKernCoverage, mathKernInfoRecords))
      എങ്കിൽ (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, ഇത്);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  mathKernCoverage.sanitize (c, ഇത്) &&
		  mathKernInfoRecords.sanitize (c, ഇത്));
  }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ index = (ഇത്+mathKernCoverage).get_coverage (glyph);
    തിരികെ mathKernInfoRecords[index].get_kerning (kern, correction_height, font, ഇത്);
  }

  unsigned പൂർണ്ണസംഖ്യ get_kernings (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     unsigned പൂർണ്ണസംഖ്യ start_offset,
			     unsigned പൂർണ്ണസംഖ്യ *entries_എണ്ണം, /* IN/OUT */
			     hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			     hb_font_t *font) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ index = (ഇത്+mathKernCoverage).get_coverage (glyph);
    തിരികെ mathKernInfoRecords[index].get_kernings (kern,
						    start_offset,
						    entries_എണ്ണം,
						    kern_entries,
						    font,
						    ഇത്);
  }

  സംരക്ഷിത:
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

  പൊതു:
  DEFINE_SIZE_ARRAY (4, mathKernInfoRecords);
};

ഘടന MathGlyphInfo
{
  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->embed (*ഇത്);
    എങ്കിൽ (unlikely (!out)) return_trace (അസത്യം);

    out->mathItalicsCorrectionInfo.serialize_subset (c, mathItalicsCorrectionInfo, ഇത്);
    out->math_മുകൾ_Accent_Attachment.serialize_subset (c, math_മുകൾ_Accent_Attachment, ഇത്);

    സ്ഥിരം hb_set_t &glyphset = c->plan->_glyphset_mathed;
    സ്ഥിരം hb_map_t &glyph_map = *c->plan->glyph_map;

    auto it =
    + hb_iter (ഇത്+extendedShapeCoverage)
    | hb_take (c->plan->source->get_അക്കം_glyphs ())
    | hb_filter (glyphset)
    | hb_map_retains_sorting (glyph_map)
    ;

    എങ്കിൽ (it) out->extendedShapeCoverage.serialize_serialize (c->serializer, it);
    അല്ലെങ്കിൽ out->extendedShapeCoverage = 0;

    out->mathKernInfo.serialize_subset (c, mathKernInfo, ഇത്);
    return_trace (സത്യം);
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം
  {
    (ഇത്+mathItalicsCorrectionInfo).collect_variation_indices (c);
    (ഇത്+math_മുകൾ_Accent_Attachment).collect_variation_indices (c);
    (ഇത്+mathKernInfo).collect_variation_indices (c);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  mathItalicsCorrectionInfo.sanitize (c, ഇത്) &&
		  math_മുകൾ_Accent_Attachment.sanitize (c, ഇത്) &&
		  extendedShapeCoverage.sanitize (c, ഇത്) &&
		  mathKernInfo.sanitize (c, ഇത്));
  }

  hb_position_t
  get_italics_correction (hb_codepoint_t  glyph, hb_font_t *font) സ്ഥിരം
  { തിരികെ (ഇത്+mathItalicsCorrectionInfo).get_മൂല്യം (glyph, font); }

  hb_position_t
  get_മുകൾ_accent_attachment (hb_codepoint_t  glyph, hb_font_t *font) സ്ഥിരം
  { തിരികെ (ഇത്+math_മുകൾ_Accent_Attachment).get_മൂല്യം (glyph, font); }

  തർക്കം is_extended_shape (hb_codepoint_t glyph) സ്ഥിരം
  { തിരികെ (ഇത്+extendedShapeCoverage).get_coverage (glyph) != NOT_COVERED; }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font) സ്ഥിരം
  { തിരികെ (ഇത്+mathKernInfo).get_kerning (glyph, kern, correction_height, font); }

  hb_position_t get_kernings (hb_codepoint_t glyph,
			      hb_ot_math_kern_t kern,
			      unsigned പൂർണ്ണസംഖ്യ start_offset,
			      unsigned പൂർണ്ണസംഖ്യ *entries_എണ്ണം, /* IN/OUT */
			      hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			      hb_font_t *font) സ്ഥിരം
  { തിരികെ (ഇത്+mathKernInfo).get_kernings (glyph,
					     kern,
					     start_offset,
					     entries_എണ്ണം,
					     kern_entries,
					     font); }

  സംരക്ഷിത:
  /* Offset to MathItalicsCorrectionInfo table -
   * from the beginning of MathGlyphInfo table. */
  Offset16To<MathItalicsCorrectionInfo> mathItalicsCorrectionInfo;

  /* Offset to MathTopAccentAttachment table -
   * from the beginning of MathGlyphInfo table. */
  Offset16To<Math_മുകൾ_Accent_Attachment> math_മുകൾ_Accent_Attachment;

  /* Offset to coverage table for Extended Shape glyphs -
   * from the beginning of MathGlyphInfo table. When the left or right glyph of
   * a box is an extended shape variant, the (ink) box (and not the default
   * position defined by values in MathConstants table) should be used for
   * vertical positioning purposes.  May be NULL.. */
  Offset16To<Coverage> extendedShapeCoverage;

   /* Offset to MathKernInfo table -
    * from the beginning of MathGlyphInfo table. */
  Offset16To<MathKernInfo> mathKernInfo;

  പൊതു:
  DEFINE_SIZE_STATIC (8);
};

ഘടന MathGlyphVariantRecord
{
  friend ഘടന MathGlyphConstruction;

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->embed (ഇത്);
    എങ്കിൽ (unlikely (!out)) return_trace (അസത്യം);

    സ്ഥിരം hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_assign (out->variantGlyph, glyph_map.get (variantGlyph), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  ശൂന്യം depend (hb_depend_data_builder_t *depend_data, unsigned source) സ്ഥിരം
  { depend_data->add_depend(source, HB_OT_TAG_MATH, variantGlyph); }

  ശൂന്യം closure_glyphs (hb_set_t *variant_glyphs) സ്ഥിരം
  { variant_glyphs->add (variantGlyph); }

  സംരക്ഷിത:
  HBGlyphID16 variantGlyph;       /* Glyph ID for the variant. */
  HBUINT16  advanceMeasurement; /* Advance width/height, in design units, of the
				 * variant, in the direction of requested
				 * glyph extension. */

  പൊതു:
  DEFINE_SIZE_STATIC (4);
};

ഘടന PartFlags : HBUINT16
{
  എണ്ണൽ Flags {
    Extender	= 0x0001u, /* If set, the part can be skipped or repeated. */

    Defined	= 0x0001u, /* All defined flags. */
  };

  പൊതു:
  DEFINE_SIZE_STATIC (2);
};

ഘടന MathGlyphPartRecord
{
  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->embed (ഇത്);
    എങ്കിൽ (unlikely (!out)) return_trace (അസത്യം);

    സ്ഥിരം hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_assign (out->glyph, glyph_map.get (glyph), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  ശൂന്യം extract (hb_ot_math_glyph_part_t &out,
		int64_t mult,
		hb_font_t *font) സ്ഥിരം
  {
    out.glyph			= glyph;

    out.start_connector_length	= font->em_mult (startConnectorLength, mult);
    out.end_connector_length	= font->em_mult (endConnectorLength, mult);
    out.full_advance		= font->em_mult (fullAdvance, mult);

    static_assert ((unsigned പൂർണ്ണസംഖ്യ) HB_OT_MATH_GLYPH_PART_FLAG_EXTENDER ==
		   (unsigned പൂർണ്ണസംഖ്യ) PartFlags::Extender, "");

    out.flags = (hb_ot_math_glyph_part_flags_t)
		(unsigned പൂർണ്ണസംഖ്യ)
		(partFlags & PartFlags::Defined);
  }

  ശൂന്യം depend (hb_depend_data_builder_t *depend_data, unsigned source) സ്ഥിരം
  { depend_data->add_depend(source, HB_OT_TAG_MATH, glyph); }

  ശൂന്യം closure_glyphs (hb_set_t *variant_glyphs) സ്ഥിരം
  { variant_glyphs->add (glyph); }

  സംരക്ഷിത:
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

  പൊതു:
  DEFINE_SIZE_STATIC (10);
};

ഘടന MathGlyphAssembly
{
  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);

    എങ്കിൽ (!c->serializer->copy (italicsCorrection, ഇത്, &c->plan->layout_variation_idx_delta_map)) return_trace (അസത്യം);
    എങ്കിൽ (!c->serializer->copy<HBUINT16> (partRecords.len)) return_trace (അസത്യം);

    വേണ്ടി (സ്ഥിരം auto& record : partRecords.iter ())
      എങ്കിൽ (!record.subset (c)) return_trace (അസത്യം);
    return_trace (സത്യം);
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം
  {
    italicsCorrection.collect_variation_indices (c, ഇത്);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  italicsCorrection.sanitize (c, ഇത്) &&
		  partRecords.sanitize (c));
  }

  unsigned പൂർണ്ണസംഖ്യ get_parts (hb_direction_t direction,
			  hb_font_t *font,
			  unsigned പൂർണ്ണസംഖ്യ start_offset,
			  unsigned പൂർണ്ണസംഖ്യ *parts_എണ്ണം, /* IN/OUT */
			  hb_ot_math_glyph_part_t *parts /* OUT */,
			  hb_position_t *italics_correction /* OUT */) സ്ഥിരം
  {
    എങ്കിൽ (parts_എണ്ണം && parts)
    {
      int64_t mult = font->dir_mult (direction);
      വേണ്ടി (auto _ : hb_zip (partRecords.as_array ().sub_array (start_offset, parts_എണ്ണം),
			    hb_array (parts, *parts_എണ്ണം)))
	_.first.extract (_.second, mult, font);
    }

    എങ്കിൽ (italics_correction)
      *italics_correction = italicsCorrection.get_x_മൂല്യം (font, ഇത്);

    തിരികെ partRecords.len;
  }

  ശൂന്യം depend (hb_depend_data_builder_t *depend_data, unsigned source) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം auto& _ : partRecords.iter ())
      _.depend (depend_data, source);
  }

  ശൂന്യം closure_glyphs (hb_set_t *variant_glyphs) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം auto& _ : partRecords.iter ())
      _.closure_glyphs (variant_glyphs);
  }

  സംരക്ഷിത:
  Math_മൂല്യം_Record
		italicsCorrection;
				/* Italics correction of this
				 * MathGlyphAssembly. Should not
				 * depend on the assembly size. */
  Array16Of<MathGlyphPartRecord>
		partRecords;	/* Array of part records, from
				 * left to right and bottom to
				 * top. */

  പൊതു:
  DEFINE_SIZE_ARRAY (6, partRecords);
};

ഘടന MathGlyphConstruction
{
  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);

    out->glyphAssembly.serialize_subset (c, glyphAssembly, ഇത്);

    എങ്കിൽ (!c->serializer->check_assign (out->mathGlyphVariantRecord.len, mathGlyphVariantRecord.len, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (അസത്യം);
    വേണ്ടി (സ്ഥിരം auto& record : mathGlyphVariantRecord.iter ())
      എങ്കിൽ (!record.subset (c)) return_trace (അസത്യം);

    return_trace (സത്യം);
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം
  {
    (ഇത്+glyphAssembly).collect_variation_indices (c);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  glyphAssembly.sanitize (c, ഇത്) &&
		  mathGlyphVariantRecord.sanitize (c));
  }

  സ്ഥിരം MathGlyphAssembly &get_assembly () സ്ഥിരം { തിരികെ ഇത്+glyphAssembly; }

  unsigned പൂർണ്ണസംഖ്യ get_variants (hb_direction_t direction,
			     hb_font_t *font,
			     unsigned പൂർണ്ണസംഖ്യ start_offset,
			     unsigned പൂർണ്ണസംഖ്യ *variants_എണ്ണം, /* IN/OUT */
			     hb_ot_math_glyph_variant_t *variants /* OUT */) സ്ഥിരം
  {
    എങ്കിൽ (variants_എണ്ണം && variants)
    {
      int64_t mult = font->dir_mult (direction);
      വേണ്ടി (auto _ : hb_zip (mathGlyphVariantRecord.as_array ().sub_array (start_offset, variants_എണ്ണം),
			    hb_array (variants, *variants_എണ്ണം)))
	_.second = {_.first.variantGlyph, font->em_mult (_.first.advanceMeasurement, mult)};
    }
    തിരികെ mathGlyphVariantRecord.len;
  }

  ശൂന്യം depend (hb_depend_data_builder_t *depend_data, unsigned source) സ്ഥിരം
  {
    (ഇത്+glyphAssembly).depend (depend_data, source);

    വേണ്ടി (സ്ഥിരം auto& _ : mathGlyphVariantRecord.iter ())
      _.depend (depend_data, source);
  }

  ശൂന്യം closure_glyphs (hb_set_t *variant_glyphs) സ്ഥിരം
  {
    (ഇത്+glyphAssembly).closure_glyphs (variant_glyphs);

    വേണ്ടി (സ്ഥിരം auto& _ : mathGlyphVariantRecord.iter ())
      _.closure_glyphs (variant_glyphs);
  }

  സംരക്ഷിത:
  /* Offset to MathGlyphAssembly table for this shape - from the beginning of
     MathGlyphConstruction table.  May be NULL. */
  Offset16To<MathGlyphAssembly>	  glyphAssembly;

  /* MathGlyphVariantRecords for alternative variants of the glyphs. */
  Array16Of<MathGlyphVariantRecord> mathGlyphVariantRecord;

  പൊതു:
  DEFINE_SIZE_ARRAY (4, mathGlyphVariantRecord);
};

ഘടന MathVariants
{
  ശൂന്യം depend (hb_depend_data_builder_t *depend_data) സ്ഥിരം
  {
    സ്ഥിരം hb_array_t<സ്ഥിരം Offset16To<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vert_Glyph_എണ്ണം + horiz_Glyph_എണ്ണം);

    എങ്കിൽ (vertGlyphCoverage)
    {
      സ്ഥിരം auto vert_offsets = glyph_construction_offsets.sub_array (0, vert_Glyph_എണ്ണം);
      + hb_zip (ഇത്+vertGlyphCoverage, vert_offsets)
      | hb_apply ([&] (സ്ഥിരം hb_pair_t<hb_codepoint_t, സ്ഥിരം Offset16To<MathGlyphConstruction>&> &_)
                  {
                    സ്ഥിരം MathGlyphConstruction &mgc = ഇത്+_.second;
                    mgc.depend (depend_data, _.first);
                  })
      ;
    }
    എങ്കിൽ (horizGlyphCoverage)
    {
      സ്ഥിരം auto hori_offsets = glyph_construction_offsets.sub_array (vert_Glyph_എണ്ണം, horiz_Glyph_എണ്ണം);
      + hb_zip (ഇത്+horizGlyphCoverage, hori_offsets)
      | hb_apply ([&] (സ്ഥിരം hb_pair_t<hb_codepoint_t, സ്ഥിരം Offset16To<MathGlyphConstruction>&> &_)
                  {
                    സ്ഥിരം MathGlyphConstruction &mgc = ഇത്+_.second;
                    mgc.depend (depend_data, _.first);
                  })
      ;
    }
  }

  ശൂന്യം closure_glyphs (സ്ഥിരം hb_set_t *glyph_set,
                       hb_set_t *variant_glyphs) സ്ഥിരം
  {
    സ്ഥിരം hb_array_t<സ്ഥിരം Offset16To<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vert_Glyph_എണ്ണം + horiz_Glyph_എണ്ണം);

    എങ്കിൽ (vertGlyphCoverage)
    {
      സ്ഥിരം auto vert_offsets = glyph_construction_offsets.sub_array (0, vert_Glyph_എണ്ണം);
      + hb_zip (ഇത്+vertGlyphCoverage, vert_offsets)
      | hb_filter (glyph_set, hb_first)
      | hb_map (hb_second)
      | hb_map (hb_add (ഇത്))
      | hb_apply ([=] (സ്ഥിരം MathGlyphConstruction &_) { _.closure_glyphs (variant_glyphs); })
      ;
    }

    എങ്കിൽ (horizGlyphCoverage)
    {
      സ്ഥിരം auto hori_offsets = glyph_construction_offsets.sub_array (vert_Glyph_എണ്ണം, horiz_Glyph_എണ്ണം);
      + hb_zip (ഇത്+horizGlyphCoverage, hori_offsets)
      | hb_filter (glyph_set, hb_first)
      | hb_map (hb_second)
      | hb_map (hb_add (ഇത്))
      | hb_apply ([=] (സ്ഥിരം MathGlyphConstruction &_) { _.closure_glyphs (variant_glyphs); })
      ;
    }
  }

  ശൂന്യം collect_coverage_and_indices (hb_sorted_vector_t<hb_codepoint_t>& new_coverage,
                                     സ്ഥിരം Offset16To<Coverage>& coverage,
                                     unsigned i,
                                     unsigned end_index,
                                     hb_set_t& indices,
                                     സ്ഥിരം hb_set_t& glyphset,
                                     സ്ഥിരം hb_map_t& glyph_map) സ്ഥിരം
  {
    എങ്കിൽ (!coverage) തിരികെ;

    വേണ്ടി (സ്ഥിരം auto _ : (ഇത്+coverage).iter ())
    {
      എങ്കിൽ (i >= end_index) തിരികെ;
      എങ്കിൽ (glyphset.has (_))
      {
        unsigned new_gid = glyph_map.get (_);
        new_coverage.push (new_gid);
        indices.add (i);
      }
      i++;
    }
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    സ്ഥിരം hb_set_t &glyphset = c->plan->_glyphset_cmaped;
    സ്ഥിരം hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);
    എങ്കിൽ (!c->serializer->check_assign (out->minConnectorOverlap, minConnectorOverlap, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (അസത്യം);

    hb_sorted_vector_t<hb_codepoint_t> new_vert_coverage;
    hb_sorted_vector_t<hb_codepoint_t> new_hori_coverage;
    hb_set_t indices;
    collect_coverage_and_indices (new_vert_coverage, vertGlyphCoverage, 0, vert_Glyph_എണ്ണം, indices, glyphset, glyph_map);
    collect_coverage_and_indices (new_hori_coverage, horizGlyphCoverage, vert_Glyph_എണ്ണം, vert_Glyph_എണ്ണം + horiz_Glyph_എണ്ണം, indices, glyphset, glyph_map);

    എങ്കിൽ (!c->serializer->check_assign (out->vert_Glyph_എണ്ണം, new_vert_coverage.length, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (അസത്യം);
    എങ്കിൽ (!c->serializer->check_assign (out->horiz_Glyph_എണ്ണം, new_hori_coverage.length, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (അസത്യം);

    വേണ്ടി (unsigned i : indices.iter ())
    {
      auto *o = c->serializer->embed (glyphConstruction[i]);
      എങ്കിൽ (!o) return_trace (അസത്യം);
      o->serialize_subset (c, glyphConstruction[i], ഇത്);
    }

    എങ്കിൽ (new_vert_coverage)
      out->vertGlyphCoverage.serialize_serialize (c->serializer, new_vert_coverage.iter ());

    എങ്കിൽ (new_hori_coverage)
    out->horizGlyphCoverage.serialize_serialize (c->serializer, new_hori_coverage.iter ());
    return_trace (സത്യം);
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം auto &entry : hb_zip (ഇത്+vertGlyphCoverage, glyphConstruction.as_array (vert_Glyph_എണ്ണം)))
      എങ്കിൽ (c->glyph_set->has (hb_first (entry)))
        (ഇത്+hb_second (entry)).collect_variation_indices (c);
    വേണ്ടി (സ്ഥിരം auto &entry : hb_zip (ഇത്+horizGlyphCoverage, glyphConstruction.as_array (vert_Glyph_എണ്ണം + horiz_Glyph_എണ്ണം).sub_array (vert_Glyph_എണ്ണം)))
      എങ്കിൽ (c->glyph_set->has (hb_first (entry)))
        (ഇത്+hb_second (entry)).collect_variation_indices (c);
  }

  തർക്കം sanitize_offsets (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = vert_Glyph_എണ്ണം + horiz_Glyph_എണ്ണം;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
      എങ്കിൽ (!glyphConstruction.arrayZ[i].sanitize (c, ഇത്)) return_trace (അസത്യം);
    return_trace (സത്യം);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  vertGlyphCoverage.sanitize (c, ഇത്) &&
		  horizGlyphCoverage.sanitize (c, ഇത്) &&
		  hb_barrier () &&
		  c->check_array (glyphConstruction.arrayZ, vert_Glyph_എണ്ണം + horiz_Glyph_എണ്ണം) &&
		  sanitize_offsets (c));
  }

  hb_position_t get_min_connector_overlap (hb_direction_t direction,
						  hb_font_t *font) സ്ഥിരം
  { തിരികെ font->em_scale_dir (minConnectorOverlap, direction); }

  unsigned പൂർണ്ണസംഖ്യ get_glyph_variants (hb_codepoint_t glyph,
				   hb_direction_t direction,
				   hb_font_t *font,
				   unsigned പൂർണ്ണസംഖ്യ start_offset,
				   unsigned പൂർണ്ണസംഖ്യ *variants_എണ്ണം, /* IN/OUT */
				   hb_ot_math_glyph_variant_t *variants /* OUT */) സ്ഥിരം
  { തിരികെ get_glyph_construction (glyph, direction, font)
	   .get_variants (direction, font, start_offset, variants_എണ്ണം, variants); }

  unsigned പൂർണ്ണസംഖ്യ get_glyph_parts (hb_codepoint_t glyph,
				hb_direction_t direction,
				hb_font_t *font,
				unsigned പൂർണ്ണസംഖ്യ start_offset,
				unsigned പൂർണ്ണസംഖ്യ *parts_എണ്ണം, /* IN/OUT */
				hb_ot_math_glyph_part_t *parts /* OUT */,
				hb_position_t *italics_correction /* OUT */) സ്ഥിരം
  { തിരികെ get_glyph_construction (glyph, direction, font)
	   .get_assembly ()
	   .get_parts (direction, font,
		       start_offset, parts_എണ്ണം, parts,
		       italics_correction); }

  സ്വകാര്യ:
  സ്ഥിരം MathGlyphConstruction &
  get_glyph_construction (hb_codepoint_t glyph,
			  hb_direction_t direction,
			  hb_font_t *font HB_UNUSED) സ്ഥിരം
  {
    തർക്കം vertical = HB_DIRECTION_IS_VERTICAL (direction);
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = vertical ? vert_Glyph_എണ്ണം : horiz_Glyph_എണ്ണം;
    സ്ഥിരം Offset16To<Coverage> &coverage = vertical ? vertGlyphCoverage
						  : horizGlyphCoverage;

    unsigned പൂർണ്ണസംഖ്യ index = (ഇത്+coverage).get_coverage (glyph);
    എങ്കിൽ (unlikely (index >= എണ്ണം)) തിരികെ Null (MathGlyphConstruction);

    എങ്കിൽ (!vertical)
      index += vert_Glyph_എണ്ണം;

    തിരികെ ഇത്+glyphConstruction[index];
  }

  സംരക്ഷിത:
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
  HBUINT16	vert_Glyph_എണ്ണം;	/* Number of glyphs for which
				 * information is provided for
				 * vertically growing variants. */
  HBUINT16	horiz_Glyph_എണ്ണം;/* Number of glyphs for which
				 * information is provided for
				 * horizontally growing variants. */

  /* Array of offsets to MathGlyphConstruction tables - from the beginning of
     the MathVariants table, for shapes growing in vertical/horizontal
     direction. */
  UnsizedArrayOf<Offset16To<MathGlyphConstruction>>
			glyphConstruction;

  പൊതു:
  DEFINE_SIZE_ARRAY (10, glyphConstruction);
};


/*
 * MATH -- Mathematical typesetting
 * https://docs.microsoft.com/en-us/typography/opentype/spec/math
 */

ഘടന MATH
{
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_MATH;

  തർക്കം has_data () സ്ഥിരം { തിരികെ version.to_int (); }

  ശൂന്യം depend (hb_depend_data_builder_t *depend_data) സ്ഥിരം
  {
    എങ്കിൽ (mathVariants)
      (ഇത്+mathVariants).depend (depend_data);
  }

  ശൂന്യം closure_glyphs (hb_set_t *glyph_set) സ്ഥിരം
  {
    എങ്കിൽ (mathVariants)
    {
      hb_set_t variant_glyphs;
      (ഇത്+mathVariants).closure_glyphs (glyph_set, &variant_glyphs);
      hb_set_union (glyph_set, &variant_glyphs);
    }
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->embed (*ഇത്);
    എങ്കിൽ (unlikely (!out)) return_trace (അസത്യം);

    out->mathConstants.serialize_copy (c->serializer, mathConstants, ഇത്, 0, hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map);
    out->mathGlyphInfo.serialize_subset (c, mathGlyphInfo, ഇത്);
    out->mathVariants.serialize_subset (c, mathVariants, ഇത്);
    return_trace (സത്യം);
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c, സ്ഥിരം hb_set_t *variant_glyph_set) സ്ഥിരം
  {
    (ഇത്+mathConstants).collect_variation_indices (c);
    (ഇത്+mathGlyphInfo).collect_variation_indices (c);
    auto *glyph_set = c->glyph_set;
    c->glyph_set = variant_glyph_set;
    (ഇത്+mathVariants).collect_variation_indices (c);
    c->glyph_set = glyph_set;
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (version.sanitize (c) &&
		  likely (version.major == 1) &&
		  hb_barrier () &&
		  mathConstants.sanitize (c, ഇത്) &&
		  mathGlyphInfo.sanitize (c, ഇത്) &&
		  mathVariants.sanitize (c, ഇത്));
  }

  // https://github.com/harfbuzz/harfbuzz/issues/4653
  HB_INTERNAL തർക്കം is_bad_cambria (hb_font_t *font) സ്ഥിരം
  {
#ifndef HB_NO_MATH
    തിരഞ്ഞെടുപ്പ് HB_CODEPOINT_ENCODE3 (font->face->table.MATH.get_blob ()->length,
                                 (ഇത്+mathConstants).minHeight[1], // displayOperatorMinHeight
                                 (ഇത്+mathConstants).minHeight[0]) // delimitedSubFormulaMinHeight
    {
      /* sha1sum:ab4a4fe054d23061f3c039493d6f665cfda2ecf5  cambria.ttc
       * sha1sum:086855301bff644f9d8827b88491fcf73a6d4cb9  cambria.ttc
       * sha1sum:b1e5a3feaca2ea3dfcf79ccb377de749ecf60343  cambria.ttc */
      അവസ്ഥ HB_CODEPOINT_ENCODE3 (25722, 2500, 3000):
        തിരികെ സത്യം;
    }
#endif
    തിരികെ അസത്യം;
  }

  hb_position_t get_constant (hb_ot_math_constant_t  constant,
			      hb_font_t		   *font) സ്ഥിരം
  { തിരികെ (ഇത്+mathConstants).get_മൂല്യം (constant, font); }

  സ്ഥിരം MathGlyphInfo &get_glyph_info () സ്ഥിരം { തിരികെ ഇത്+mathGlyphInfo; }

  സ്ഥിരം MathVariants &get_variants () സ്ഥിരം    { തിരികെ ഇത്+mathVariants; }

  സംരക്ഷിത:
  FixedVersion<>version;	/* Version of the MATH table
				 * initially set to 0x00010000u */
  Offset16To<MathConstants>
		mathConstants;	/* MathConstants table */
  Offset16To<MathGlyphInfo>
		mathGlyphInfo;	/* MathGlyphInfo table */
  Offset16To<MathVariants>
		mathVariants;	/* MathVariants table */

  പൊതു:
  DEFINE_SIZE_STATIC (10);
};

} /* namespace OT */


#endif /* HB_OT_MATH_TABLE_HH */

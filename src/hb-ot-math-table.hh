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

#include "hb-open-type.hh"
#include "hb-ot-layout-common.hh"
#include "hb-ot-layout-gdef-table.hh"
#include "hb-ot-math.h"
#include "hb-depend-data.hh"

namespace OT {


sa_nracanaa MathValueRecord
{
  hb_position_t get_x_value (hb_font_t *font, const void *base) const
  { ສົ່ງຄືນ hb_saturate_add (font->em_scale_x (value), (base+deviceTable).get_x_delta (font, get_var_store (font, base))); }
  hb_position_t get_y_value (hb_font_t *font, const void *base) const
  { ສົ່ງຄືນ hb_saturate_add (font->em_scale_y (value), (base+deviceTable).get_y_delta (font, get_var_store (font, base))); }

  const ItemVariationStore &get_var_store (hb_font_t *font, const void *base) const
  {
#ຖ້າ !defined(HB_NO_VAR) && !defined(HB_NO_OT_LAYOUT)
    ຖ້າ ((base+deviceTable).is_variation_device ())
      ສົ່ງຄືນ font->face->table.GDEF->table->get_var_store ();
#endif
    ສົ່ງຄືນ Null (ItemVariationStore);
  }

  MathValueRecord* copy (hb_serialize_context_t *c, const void *base,
                         const hb_hashmap_t<unsigned, hb_pair_t<unsigned, ຈຳນວນເຕັມ>> *variation_map) const
  {
    TRACE_SERIALIZE (this);
    auto *out = c->embed (this);
    ຖ້າ (unlikely (!out)) return_trace (nullptr);
    hb_pair_t<unsigned, ຈຳນວນເຕັມ> *varidx_delta;
    ຖ້າ (variation_map->has ((base+deviceTable).get_variation_index (), &varidx_delta) &&
        !c->check_assign (out->value, (int64_t) value + hb_second (*varidx_delta),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (nullptr);
    out->deviceTable.serialize_copy (c, deviceTable, base, 0, hb_serialize_context_t::Head, variation_map);

    return_trace (out);
  }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c, const void *base) const
  { (base+deviceTable).collect_variation_indices (c); }

  bool sanitize (hb_sanitize_context_t *c, const void *base) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) && deviceTable.sanitize (c, base));
  }

  rak_sita:
  HBINT16		value;		/* The X or Y value in design units */
  Offset16To<Device>	deviceTable;	/* Offset to the device table - from the
					 * beginning of parent table.  May be NULL.
					 * Suggested format for device table is 1. */

  public:
  DEFINE_SIZE_STATIC (4);
};

sa_nracanaa MathConstants
{
  friend sa_nracanaa MATH;

  MathConstants* copy (hb_serialize_context_t *c,
                      const hb_hashmap_t<unsigned, hb_pair_t<unsigned, ຈຳນວນເຕັມ>> *variation_map) const
  {
    TRACE_SERIALIZE (this);
    auto *out = c->start_embed (this);

    HBINT16 *p = c->allocate_size<HBINT16> (HBINT16::static_size * 2);
    ຖ້າ (unlikely (!p)) return_trace (nullptr);
    hb_memcpy (p, percentScaleDown, HBINT16::static_size * 2);

    HBUINT16 *m = c->allocate_size<HBUINT16> (HBUINT16::static_size * 2);
    ຖ້າ (unlikely (!m)) return_trace (nullptr);
    hb_memcpy (m, minHeight, HBUINT16::static_size * 2);

    unsigned count = ARRAY_LENGTH (mathValueRecords);
    ສຳລັບ (unsigned i = 0; i < count; i++)
      ຖ້າ (!c->copy (mathValueRecords[i], this, variation_map))
        return_trace (nullptr);

    ຖ້າ (!c->embed (radicalDegreeBottomRaisePercent)) return_trace (nullptr);
    return_trace (out);
  }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c) const
  {
    ສຳລັບ (const auto &record : mathValueRecords)
      record.collect_variation_indices (c, this);
  }

  bool sanitize_math_value_records (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);

    unsigned ຈຳນວນເຕັມ count = ARRAY_LENGTH (mathValueRecords);
    ສຳລັບ (unsigned ຈຳນວນເຕັມ i = 0; i < count; i++)
      ຖ້າ (!mathValueRecords[i].sanitize (c, this))
	return_trace (false);

    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) && sanitize_math_value_records (c));
  }

  hb_position_t get_value (hb_ot_math_constant_t constant,
			   hb_font_t *font) const
  {
    switch (constant) {

    pak_sa HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_DOWN:
    pak_sa HB_OT_MATH_CONSTANT_SCRIPT_SCRIPT_PERCENT_SCALE_DOWN:
      ສົ່ງຄືນ percentScaleDown[constant - HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_DOWN];

    pak_sa HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_HEIGHT:
    pak_sa HB_OT_MATH_CONSTANT_DISPLAY_OPERATOR_MIN_HEIGHT:
      ສົ່ງຄືນ font->em_scale_y (minHeight[constant - HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_HEIGHT]);

    pak_sa HB_OT_MATH_CONSTANT_RADICAL_KERN_AFTER_DEGREE:
    pak_sa HB_OT_MATH_CONSTANT_RADICAL_KERN_BEFORE_DEGREE:
    pak_sa HB_OT_MATH_CONSTANT_SKEWED_FRACTION_HORIZONTAL_GAP:
    pak_sa HB_OT_MATH_CONSTANT_SPACE_AFTER_SCRIPT:
      ສົ່ງຄືນ mathValueRecords[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_x_value (font, this);

    pak_sa HB_OT_MATH_CONSTANT_ACCENT_BASE_HEIGHT:
    pak_sa HB_OT_MATH_CONSTANT_AXIS_HEIGHT:
    pak_sa HB_OT_MATH_CONSTANT_FLATTENED_ACCENT_BASE_HEIGHT:
    pak_sa HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_DISPLAY_STYLE_SHIFT_DOWN:
    pak_sa HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_GAP_MIN:
    pak_sa HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_SHIFT_DOWN:
    pak_sa HB_OT_MATH_CONSTANT_FRACTION_DENOM_DISPLAY_STYLE_GAP_MIN:
    pak_sa HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_DISPLAY_STYLE_SHIFT_UP:
    pak_sa HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_GAP_MIN:
    pak_sa HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_SHIFT_UP:
    pak_sa HB_OT_MATH_CONSTANT_FRACTION_NUM_DISPLAY_STYLE_GAP_MIN:
    pak_sa HB_OT_MATH_CONSTANT_FRACTION_RULE_THICKNESS:
    pak_sa HB_OT_MATH_CONSTANT_LOWER_LIMIT_BASELINE_DROP_MIN:
    pak_sa HB_OT_MATH_CONSTANT_LOWER_LIMIT_GAP_MIN:
    pak_sa HB_OT_MATH_CONSTANT_MATH_LEADING:
    pak_sa HB_OT_MATH_CONSTANT_OVERBAR_EXTRA_ASCENDER:
    pak_sa HB_OT_MATH_CONSTANT_OVERBAR_RULE_THICKNESS:
    pak_sa HB_OT_MATH_CONSTANT_OVERBAR_VERTICAL_GAP:
    pak_sa HB_OT_MATH_CONSTANT_RADICAL_DISPLAY_STYLE_VERTICAL_GAP:
    pak_sa HB_OT_MATH_CONSTANT_RADICAL_EXTRA_ASCENDER:
    pak_sa HB_OT_MATH_CONSTANT_RADICAL_RULE_THICKNESS:
    pak_sa HB_OT_MATH_CONSTANT_RADICAL_VERTICAL_GAP:
    pak_sa HB_OT_MATH_CONSTANT_SKEWED_FRACTION_VERTICAL_GAP:
    pak_sa HB_OT_MATH_CONSTANT_STACK_BOTTOM_DISPLAY_STYLE_SHIFT_DOWN:
    pak_sa HB_OT_MATH_CONSTANT_STACK_BOTTOM_SHIFT_DOWN:
    pak_sa HB_OT_MATH_CONSTANT_STACK_DISPLAY_STYLE_GAP_MIN:
    pak_sa HB_OT_MATH_CONSTANT_STACK_GAP_MIN:
    pak_sa HB_OT_MATH_CONSTANT_STACK_TOP_DISPLAY_STYLE_SHIFT_UP:
    pak_sa HB_OT_MATH_CONSTANT_STACK_TOP_SHIFT_UP:
    pak_sa HB_OT_MATH_CONSTANT_STRETCH_STACK_BOTTOM_SHIFT_DOWN:
    pak_sa HB_OT_MATH_CONSTANT_STRETCH_STACK_GAP_ABOVE_MIN:
    pak_sa HB_OT_MATH_CONSTANT_STRETCH_STACK_GAP_BELOW_MIN:
    pak_sa HB_OT_MATH_CONSTANT_STRETCH_STACK_TOP_SHIFT_UP:
    pak_sa HB_OT_MATH_CONSTANT_SUBSCRIPT_BASELINE_DROP_MIN:
    pak_sa HB_OT_MATH_CONSTANT_SUBSCRIPT_SHIFT_DOWN:
    pak_sa HB_OT_MATH_CONSTANT_SUBSCRIPT_TOP_MAX:
    pak_sa HB_OT_MATH_CONSTANT_SUB_SUPERSCRIPT_GAP_MIN:
    pak_sa HB_OT_MATH_CONSTANT_SUPERSCRIPT_BASELINE_DROP_MAX:
    pak_sa HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MAX_WITH_SUBSCRIPT:
    pak_sa HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MIN:
    pak_sa HB_OT_MATH_CONSTANT_SUPERSCRIPT_SHIFT_UP:
    pak_sa HB_OT_MATH_CONSTANT_SUPERSCRIPT_SHIFT_UP_CRAMPED:
    pak_sa HB_OT_MATH_CONSTANT_UNDERBAR_EXTRA_DESCENDER:
    pak_sa HB_OT_MATH_CONSTANT_UNDERBAR_RULE_THICKNESS:
    pak_sa HB_OT_MATH_CONSTANT_UNDERBAR_VERTICAL_GAP:
    pak_sa HB_OT_MATH_CONSTANT_UPPER_LIMIT_BASELINE_RISE_MIN:
    pak_sa HB_OT_MATH_CONSTANT_UPPER_LIMIT_GAP_MIN:
      ສົ່ງຄືນ mathValueRecords[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_y_value (font, this);

    pak_sa HB_OT_MATH_CONSTANT_RADICAL_DEGREE_BOTTOM_RAISE_PERCENT:
      ສົ່ງຄືນ radicalDegreeBottomRaisePercent;

    default:
      ສົ່ງຄືນ 0;
    }
  }

  rak_sita:
  HBINT16 percentScaleDown[2];
  HBUINT16 minHeight[2];
  MathValueRecord mathValueRecords[51];
  HBINT16 radicalDegreeBottomRaisePercent;

  public:
  DEFINE_SIZE_STATIC (214);
};

sa_nracanaa MathItalicsCorrectionInfo
{
  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    const hb_set_t &glyphset = c->plan->_glyphset_mathed;
    const hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*this);
    ຖ້າ (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (this+coverage, italicsCorrection)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->italicsCorrection, this, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (true);
  }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c) const
  {
    ສຳລັບ (const auto &entry : hb_zip (this+coverage, italicsCorrection))
      ຖ້າ (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, this);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  coverage.sanitize (c, this) &&
		  italicsCorrection.sanitize (c, this));
  }

  hb_position_t get_value (hb_codepoint_t glyph,
			   hb_font_t *font) const
  {
    unsigned ຈຳນວນເຕັມ index = (this+coverage).get_coverage (glyph);
    ສົ່ງຄືນ italicsCorrection[index].get_x_value (font, this);
  }

  rak_sita:
  Offset16To<Coverage>       coverage;		/* Offset to Coverage table -
						 * from the beginning of
						 * MathItalicsCorrectionInfo
						 * table. */
  Array16Of<MathValueRecord> italicsCorrection;	/* Array of MathValueRecords
						 * defining italics correction
						 * values for each
						 * covered glyph. */

  public:
  DEFINE_SIZE_ARRAY (4, italicsCorrection);
};

sa_nracanaa MathTopAccentAttachment
{
  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    const hb_set_t &glyphset = c->plan->_glyphset_mathed;
    const hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*this);
    ຖ້າ (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (this+topAccentCoverage, topAccentAttachment)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->topAccentAttachment, this, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->topAccentCoverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (true);
  }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c) const
  {
    ສຳລັບ (const auto &entry : hb_zip (this+topAccentCoverage, topAccentAttachment))
      ຖ້າ (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, this);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  topAccentCoverage.sanitize (c, this) &&
		  topAccentAttachment.sanitize (c, this));
  }

  hb_position_t get_value (hb_codepoint_t glyph,
			   hb_font_t *font) const
  {
    unsigned ຈຳນວນເຕັມ index = (this+topAccentCoverage).get_coverage (glyph);
    ຖ້າ (index == NOT_COVERED)
      ສົ່ງຄືນ font->get_glyph_h_advance (glyph) / 2;
    ສົ່ງຄືນ topAccentAttachment[index].get_x_value (font, this);
  }

  rak_sita:
  Offset16To<Coverage>       topAccentCoverage;   /* Offset to Coverage table -
						 * from the beginning of
						 * MathTopAccentAttachment
						 * table. */
  Array16Of<MathValueRecord> topAccentAttachment; /* Array of MathValueRecords
						 * defining top accent
						 * attachment points for each
						 * covered glyph. */

  public:
  DEFINE_SIZE_ARRAY (2 + 2, topAccentAttachment);
};

sa_nracanaa MathKern
{
  MathKern* copy (hb_serialize_context_t *c,
                      const hb_hashmap_t<unsigned, hb_pair_t<unsigned, ຈຳນວນເຕັມ>> *variation_map) const
  {
    TRACE_SERIALIZE (this);
    auto *out = c->start_embed (this);

    ຖ້າ (unlikely (!c->embed (heightCount))) return_trace (nullptr);

    unsigned count = 2 * heightCount + 1;
    ສຳລັບ (unsigned i = 0; i < count; i++)
      ຖ້າ (!c->copy (mathValueRecordsZ.arrayZ[i], this, variation_map))
        return_trace (nullptr);

    return_trace (out);
  }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c) const
  {
    ສຳລັບ (const auto &record : mathValueRecordsZ.as_array (2 * heightCount + 1))
      record.collect_variation_indices (c, this);
  }

  bool sanitize_math_value_records (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    unsigned ຈຳນວນເຕັມ count = 2 * heightCount + 1;
    ສຳລັບ (unsigned ຈຳນວນເຕັມ i = 0; i < count; i++)
      ຖ້າ (!mathValueRecordsZ.arrayZ[i].sanitize (c, this)) return_trace (false);
    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  hb_barrier () &&
		  c->check_array (mathValueRecordsZ.arrayZ, 2 * heightCount + 1) &&
		  sanitize_math_value_records (c));
  }

  hb_position_t get_value (hb_position_t correction_height, hb_font_t *font) const
  {
    const MathValueRecord* correctionHeight = mathValueRecordsZ.arrayZ;
    const MathValueRecord* kernValue = mathValueRecordsZ.arrayZ + heightCount;
    ຈຳນວນເຕັມ sign = font->y_scale < 0 ? -1 : +1;

    /* According to OpenType spec (v1.9), except for the boundary cases, the index
     * chosen for kern value should be i such that
     *    correctionHeight[i-1] <= correction_height < correctionHeight[i]
     * We can use the binary search algorithm of std::upper_bound(). Or, we can
     * use the internal hb_bsearch_impl.
     */
    unsigned ຈຳນວນເຕັມ pos;
    auto cmp = +[](const void* key, const void* p,
                   ຈຳນວນເຕັມ sign, hb_font_t* font, const MathKern* mathKern) -> ຈຳນວນເຕັມ {
      hb_position_t a = *(hb_position_t *) key;
      hb_position_t b = ((MathValueRecord *) p)->get_y_value (font, mathKern);
      ຖ້າ (a == b) ສົ່ງຄືນ 0;
      ສົ່ງຄືນ sign > 0 ? (a < b ? -1 : +1) : (b < a ? -1 : +1);
    };
    unsigned ຈຳນວນເຕັມ i = hb_bsearch_impl(&pos, correction_height, correctionHeight,
                                     heightCount, MathValueRecord::static_size,
                                     cmp, sign, font, this) ? pos + 1 : pos;
    ສົ່ງຄືນ kernValue[i].get_x_value (font, this);
  }

  unsigned ຈຳນວນເຕັມ get_entries (unsigned ຈຳນວນເຕັມ start_offset,
			    unsigned ຈຳນວນເຕັມ *entries_count, /* IN/OUT */
			    hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			    hb_font_t *font) const
  {
    const MathValueRecord* correctionHeight = mathValueRecordsZ.arrayZ;
    const MathValueRecord* kernValue = mathValueRecordsZ.arrayZ + heightCount;
    const unsigned ຈຳນວນເຕັມ entriesCount = heightCount + 1;

    ຖ້າ (entries_count && kern_entries)
    {
      unsigned ຈຳນວນເຕັມ start = hb_min (start_offset, entriesCount);
      unsigned ຈຳນວນເຕັມ end = hb_min (start + *entries_count, entriesCount);
      *entries_count = end - start;

      ສຳລັບ (unsigned ຈຳນວນເຕັມ i = 0; i < *entries_count; i++) {
	unsigned ຈຳນວນເຕັມ j = start + i;

	hb_position_t max_height;
	ຖ້າ (j == heightCount) {
	  max_height = INT32_MAX;
	} ບໍ່ດັ່ງນັ້ນ {
	  max_height = correctionHeight[j].get_y_value (font, this);
	}

	kern_entries[i] = {max_height, kernValue[j].get_x_value (font, this)};
      }
    }
    ສົ່ງຄືນ entriesCount;
  }

  rak_sita:
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

  public:
  DEFINE_SIZE_ARRAY (2, mathValueRecordsZ);
};

sa_nracanaa MathKernInfoRecord
{
  MathKernInfoRecord* copy (hb_serialize_context_t *c, const void *base,
                            const hb_hashmap_t<unsigned, hb_pair_t<unsigned, ຈຳນວນເຕັມ>> *variation_map) const
  {
    TRACE_SERIALIZE (this);
    auto *out = c->embed (this);
    ຖ້າ (unlikely (!out)) return_trace (nullptr);

    unsigned count = ARRAY_LENGTH (mathKern);
    ສຳລັບ (unsigned i = 0; i < count; i++)
      out->mathKern[i].serialize_copy (c, mathKern[i], base, 0, hb_serialize_context_t::Head, variation_map);

    return_trace (out);
  }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c, const void *base) const
  {
    ສຳລັບ (const auto &offset : mathKern)
      (base+offset).collect_variation_indices (c);
  }

  bool sanitize (hb_sanitize_context_t *c, const void *base) const
  {
    TRACE_SANITIZE (this);

    unsigned ຈຳນວນເຕັມ count = ARRAY_LENGTH (mathKern);
    ສຳລັບ (unsigned ຈຳນວນເຕັມ i = 0; i < count; i++)
      ຖ້າ (unlikely (!mathKern[i].sanitize (c, base)))
	return_trace (false);

    return_trace (true);
  }

  hb_position_t get_kerning (hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font,
			     const void *base) const
  {
    unsigned ຈຳນວນເຕັມ idx = kern;
    ຖ້າ (unlikely (idx >= ARRAY_LENGTH (mathKern))) ສົ່ງຄືນ 0;
    ສົ່ງຄືນ (base+mathKern[idx]).get_value (correction_height, font);
  }

  unsigned ຈຳນວນເຕັມ get_kernings (hb_ot_math_kern_t kern,
			     unsigned ຈຳນວນເຕັມ start_offset,
			     unsigned ຈຳນວນເຕັມ *entries_count, /* IN/OUT */
			     hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			     hb_font_t *font,
			     const void *base) const
  {
    unsigned ຈຳນວນເຕັມ idx = kern;
    ຖ້າ (unlikely (idx >= ARRAY_LENGTH (mathKern)) || !mathKern[idx]) {
      ຖ້າ (entries_count) *entries_count = 0;
      ສົ່ງຄືນ 0;
    }
    ສົ່ງຄືນ (base+mathKern[idx]).get_entries (start_offset,
					     entries_count,
					     kern_entries,
					     font);
  }

  rak_sita:
  /* Offset to MathKern table for each corner -
   * from the beginning of MathKernInfo table.  May be NULL. */
  Offset16To<MathKern> mathKern[4];

  public:
  DEFINE_SIZE_STATIC (8);
};

sa_nracanaa MathKernInfo
{
  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    const hb_set_t &glyphset = c->plan->_glyphset_mathed;
    const hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*this);
    ຖ້າ (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (this+mathKernCoverage, mathKernInfoRecords)
    | hb_filter (glyphset, hb_first)
    | hb_filter (serialize_math_record_array (c->serializer, out->mathKernInfoRecords, this, &c->plan->layout_variation_idx_delta_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->mathKernCoverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (true);
  }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c) const
  {
    ສຳລັບ (const auto &entry : hb_zip (this+mathKernCoverage, mathKernInfoRecords))
      ຖ້າ (c->glyph_set->has (hb_first (entry)))
        hb_second (entry).collect_variation_indices (c, this);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  mathKernCoverage.sanitize (c, this) &&
		  mathKernInfoRecords.sanitize (c, this));
  }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font) const
  {
    unsigned ຈຳນວນເຕັມ index = (this+mathKernCoverage).get_coverage (glyph);
    ສົ່ງຄືນ mathKernInfoRecords[index].get_kerning (kern, correction_height, font, this);
  }

  unsigned ຈຳນວນເຕັມ get_kernings (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     unsigned ຈຳນວນເຕັມ start_offset,
			     unsigned ຈຳນວນເຕັມ *entries_count, /* IN/OUT */
			     hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			     hb_font_t *font) const
  {
    unsigned ຈຳນວນເຕັມ index = (this+mathKernCoverage).get_coverage (glyph);
    ສົ່ງຄືນ mathKernInfoRecords[index].get_kernings (kern,
						    start_offset,
						    entries_count,
						    kern_entries,
						    font,
						    this);
  }

  rak_sita:
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

  public:
  DEFINE_SIZE_ARRAY (4, mathKernInfoRecords);
};

sa_nracanaa MathGlyphInfo
{
  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->embed (*this);
    ຖ້າ (unlikely (!out)) return_trace (false);

    out->mathItalicsCorrectionInfo.serialize_subset (c, mathItalicsCorrectionInfo, this);
    out->mathTopAccentAttachment.serialize_subset (c, mathTopAccentAttachment, this);

    const hb_set_t &glyphset = c->plan->_glyphset_mathed;
    const hb_map_t &glyph_map = *c->plan->glyph_map;

    auto it =
    + hb_iter (this+extendedShapeCoverage)
    | hb_take (c->plan->source->get_num_glyphs ())
    | hb_filter (glyphset)
    | hb_map_retains_sorting (glyph_map)
    ;

    ຖ້າ (it) out->extendedShapeCoverage.serialize_serialize (c->serializer, it);
    ບໍ່ດັ່ງນັ້ນ out->extendedShapeCoverage = 0;

    out->mathKernInfo.serialize_subset (c, mathKernInfo, this);
    return_trace (true);
  }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c) const
  {
    (this+mathItalicsCorrectionInfo).collect_variation_indices (c);
    (this+mathTopAccentAttachment).collect_variation_indices (c);
    (this+mathKernInfo).collect_variation_indices (c);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  mathItalicsCorrectionInfo.sanitize (c, this) &&
		  mathTopAccentAttachment.sanitize (c, this) &&
		  extendedShapeCoverage.sanitize (c, this) &&
		  mathKernInfo.sanitize (c, this));
  }

  hb_position_t
  get_italics_correction (hb_codepoint_t  glyph, hb_font_t *font) const
  { ສົ່ງຄືນ (this+mathItalicsCorrectionInfo).get_value (glyph, font); }

  hb_position_t
  get_top_accent_attachment (hb_codepoint_t  glyph, hb_font_t *font) const
  { ສົ່ງຄືນ (this+mathTopAccentAttachment).get_value (glyph, font); }

  bool is_extended_shape (hb_codepoint_t glyph) const
  { ສົ່ງຄືນ (this+extendedShapeCoverage).get_coverage (glyph) != NOT_COVERED; }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_height,
			     hb_font_t *font) const
  { ສົ່ງຄືນ (this+mathKernInfo).get_kerning (glyph, kern, correction_height, font); }

  hb_position_t get_kernings (hb_codepoint_t glyph,
			      hb_ot_math_kern_t kern,
			      unsigned ຈຳນວນເຕັມ start_offset,
			      unsigned ຈຳນວນເຕັມ *entries_count, /* IN/OUT */
			      hb_ot_math_kern_entry_t *kern_entries, /* OUT */
			      hb_font_t *font) const
  { ສົ່ງຄືນ (this+mathKernInfo).get_kernings (glyph,
					     kern,
					     start_offset,
					     entries_count,
					     kern_entries,
					     font); }

  rak_sita:
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

  public:
  DEFINE_SIZE_STATIC (8);
};

sa_nracanaa MathGlyphVariantRecord
{
  friend sa_nracanaa MathGlyphConstruction;

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->embed (this);
    ຖ້າ (unlikely (!out)) return_trace (false);

    const hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_assign (out->variantGlyph, glyph_map.get (variantGlyph), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  void depend (hb_depend_data_builder_t *depend_data, unsigned source) const
  { depend_data->add_depend(source, HB_OT_TAG_MATH, variantGlyph); }

  void closure_glyphs (hb_set_t *variant_glyphs) const
  { variant_glyphs->add (variantGlyph); }

  rak_sita:
  HBGlyphID16 variantGlyph;       /* Glyph ID for the variant. */
  HBUINT16  advanceMeasurement; /* Advance width/height, in design units, of the
				 * variant, in the direction of requested
				 * glyph extension. */

  public:
  DEFINE_SIZE_STATIC (4);
};

sa_nracanaa PartFlags : HBUINT16
{
  ga_nanaa Flags {
    Extender	= 0x0001u, /* If set, the part can be skipped or repeated. */

    Defined	= 0x0001u, /* All defined flags. */
  };

  public:
  DEFINE_SIZE_STATIC (2);
};

sa_nracanaa MathGlyphPartRecord
{
  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->embed (this);
    ຖ້າ (unlikely (!out)) return_trace (false);

    const hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_assign (out->glyph, glyph_map.get (glyph), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  void extract (hb_ot_math_glyph_part_t &out,
		int64_t mult,
		hb_font_t *font) const
  {
    out.glyph			= glyph;

    out.start_connector_length	= font->em_mult (startConnectorLength, mult);
    out.end_connector_length	= font->em_mult (endConnectorLength, mult);
    out.full_advance		= font->em_mult (fullAdvance, mult);

    static_assert ((unsigned ຈຳນວນເຕັມ) HB_OT_MATH_GLYPH_PART_FLAG_EXTENDER ==
		   (unsigned ຈຳນວນເຕັມ) PartFlags::Extender, "");

    out.flags = (hb_ot_math_glyph_part_flags_t)
		(unsigned ຈຳນວນເຕັມ)
		(partFlags & PartFlags::Defined);
  }

  void depend (hb_depend_data_builder_t *depend_data, unsigned source) const
  { depend_data->add_depend(source, HB_OT_TAG_MATH, glyph); }

  void closure_glyphs (hb_set_t *variant_glyphs) const
  { variant_glyphs->add (glyph); }

  rak_sita:
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

  public:
  DEFINE_SIZE_STATIC (10);
};

sa_nracanaa MathGlyphAssembly
{
  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);

    ຖ້າ (!c->serializer->copy (italicsCorrection, this, &c->plan->layout_variation_idx_delta_map)) return_trace (false);
    ຖ້າ (!c->serializer->copy<HBUINT16> (partRecords.len)) return_trace (false);

    ສຳລັບ (const auto& record : partRecords.iter ())
      ຖ້າ (!record.subset (c)) return_trace (false);
    return_trace (true);
  }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c) const
  {
    italicsCorrection.collect_variation_indices (c, this);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  italicsCorrection.sanitize (c, this) &&
		  partRecords.sanitize (c));
  }

  unsigned ຈຳນວນເຕັມ get_parts (hb_direction_t direction,
			  hb_font_t *font,
			  unsigned ຈຳນວນເຕັມ start_offset,
			  unsigned ຈຳນວນເຕັມ *parts_count, /* IN/OUT */
			  hb_ot_math_glyph_part_t *parts /* OUT */,
			  hb_position_t *italics_correction /* OUT */) const
  {
    ຖ້າ (parts_count && parts)
    {
      int64_t mult = font->dir_mult (direction);
      ສຳລັບ (auto _ : hb_zip (partRecords.as_array ().sub_array (start_offset, parts_count),
			    hb_array (parts, *parts_count)))
	_.first.extract (_.second, mult, font);
    }

    ຖ້າ (italics_correction)
      *italics_correction = italicsCorrection.get_x_value (font, this);

    ສົ່ງຄືນ partRecords.len;
  }

  void depend (hb_depend_data_builder_t *depend_data, unsigned source) const
  {
    ສຳລັບ (const auto& _ : partRecords.iter ())
      _.depend (depend_data, source);
  }

  void closure_glyphs (hb_set_t *variant_glyphs) const
  {
    ສຳລັບ (const auto& _ : partRecords.iter ())
      _.closure_glyphs (variant_glyphs);
  }

  rak_sita:
  MathValueRecord
		italicsCorrection;
				/* Italics correction of this
				 * MathGlyphAssembly. Should not
				 * depend on the assembly size. */
  Array16Of<MathGlyphPartRecord>
		partRecords;	/* Array of part records, from
				 * left to right and bottom to
				 * top. */

  public:
  DEFINE_SIZE_ARRAY (6, partRecords);
};

sa_nracanaa MathGlyphConstruction
{
  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->start_embed (*this);
    ຖ້າ (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    out->glyphAssembly.serialize_subset (c, glyphAssembly, this);

    ຖ້າ (!c->serializer->check_assign (out->mathGlyphVariantRecord.len, mathGlyphVariantRecord.len, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (false);
    ສຳລັບ (const auto& record : mathGlyphVariantRecord.iter ())
      ຖ້າ (!record.subset (c)) return_trace (false);

    return_trace (true);
  }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c) const
  {
    (this+glyphAssembly).collect_variation_indices (c);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  glyphAssembly.sanitize (c, this) &&
		  mathGlyphVariantRecord.sanitize (c));
  }

  const MathGlyphAssembly &get_assembly () const { ສົ່ງຄືນ this+glyphAssembly; }

  unsigned ຈຳນວນເຕັມ get_variants (hb_direction_t direction,
			     hb_font_t *font,
			     unsigned ຈຳນວນເຕັມ start_offset,
			     unsigned ຈຳນວນເຕັມ *variants_count, /* IN/OUT */
			     hb_ot_math_glyph_variant_t *variants /* OUT */) const
  {
    ຖ້າ (variants_count && variants)
    {
      int64_t mult = font->dir_mult (direction);
      ສຳລັບ (auto _ : hb_zip (mathGlyphVariantRecord.as_array ().sub_array (start_offset, variants_count),
			    hb_array (variants, *variants_count)))
	_.second = {_.first.variantGlyph, font->em_mult (_.first.advanceMeasurement, mult)};
    }
    ສົ່ງຄືນ mathGlyphVariantRecord.len;
  }

  void depend (hb_depend_data_builder_t *depend_data, unsigned source) const
  {
    (this+glyphAssembly).depend (depend_data, source);

    ສຳລັບ (const auto& _ : mathGlyphVariantRecord.iter ())
      _.depend (depend_data, source);
  }

  void closure_glyphs (hb_set_t *variant_glyphs) const
  {
    (this+glyphAssembly).closure_glyphs (variant_glyphs);

    ສຳລັບ (const auto& _ : mathGlyphVariantRecord.iter ())
      _.closure_glyphs (variant_glyphs);
  }

  rak_sita:
  /* Offset to MathGlyphAssembly table for this shape - from the beginning of
     MathGlyphConstruction table.  May be NULL. */
  Offset16To<MathGlyphAssembly>	  glyphAssembly;

  /* MathGlyphVariantRecords for alternative variants of the glyphs. */
  Array16Of<MathGlyphVariantRecord> mathGlyphVariantRecord;

  public:
  DEFINE_SIZE_ARRAY (4, mathGlyphVariantRecord);
};

sa_nracanaa MathVariants
{
  void depend (hb_depend_data_builder_t *depend_data) const
  {
    const hb_array_t<const Offset16To<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vertGlyphCount + horizGlyphCount);

    ຖ້າ (vertGlyphCoverage)
    {
      const auto vert_offsets = glyph_construction_offsets.sub_array (0, vertGlyphCount);
      + hb_zip (this+vertGlyphCoverage, vert_offsets)
      | hb_apply ([&] (const hb_pair_t<hb_codepoint_t, const Offset16To<MathGlyphConstruction>&> &_)
                  {
                    const MathGlyphConstruction &mgc = this+_.second;
                    mgc.depend (depend_data, _.first);
                  })
      ;
    }
    ຖ້າ (horizGlyphCoverage)
    {
      const auto hori_offsets = glyph_construction_offsets.sub_array (vertGlyphCount, horizGlyphCount);
      + hb_zip (this+horizGlyphCoverage, hori_offsets)
      | hb_apply ([&] (const hb_pair_t<hb_codepoint_t, const Offset16To<MathGlyphConstruction>&> &_)
                  {
                    const MathGlyphConstruction &mgc = this+_.second;
                    mgc.depend (depend_data, _.first);
                  })
      ;
    }
  }

  void closure_glyphs (const hb_set_t *glyph_set,
                       hb_set_t *variant_glyphs) const
  {
    const hb_array_t<const Offset16To<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vertGlyphCount + horizGlyphCount);

    ຖ້າ (vertGlyphCoverage)
    {
      const auto vert_offsets = glyph_construction_offsets.sub_array (0, vertGlyphCount);
      + hb_zip (this+vertGlyphCoverage, vert_offsets)
      | hb_filter (glyph_set, hb_first)
      | hb_map (hb_second)
      | hb_map (hb_add (this))
      | hb_apply ([=] (const MathGlyphConstruction &_) { _.closure_glyphs (variant_glyphs); })
      ;
    }

    ຖ້າ (horizGlyphCoverage)
    {
      const auto hori_offsets = glyph_construction_offsets.sub_array (vertGlyphCount, horizGlyphCount);
      + hb_zip (this+horizGlyphCoverage, hori_offsets)
      | hb_filter (glyph_set, hb_first)
      | hb_map (hb_second)
      | hb_map (hb_add (this))
      | hb_apply ([=] (const MathGlyphConstruction &_) { _.closure_glyphs (variant_glyphs); })
      ;
    }
  }

  void collect_coverage_and_indices (hb_sorted_vector_t<hb_codepoint_t>& new_coverage,
                                     const Offset16To<Coverage>& coverage,
                                     unsigned i,
                                     unsigned end_index,
                                     hb_set_t& indices,
                                     const hb_set_t& glyphset,
                                     const hb_map_t& glyph_map) const
  {
    ຖ້າ (!coverage) ສົ່ງຄືນ;

    ສຳລັບ (const auto _ : (this+coverage).iter ())
    {
      ຖ້າ (i >= end_index) ສົ່ງຄືນ;
      ຖ້າ (glyphset.has (_))
      {
        unsigned new_gid = glyph_map.get (_);
        new_coverage.push (new_gid);
        indices.add (i);
      }
      i++;
    }
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    const hb_set_t &glyphset = c->plan->_glyphset_cmaped;
    const hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*this);
    ຖ້າ (unlikely (!c->serializer->extend_min (out))) return_trace (false);
    ຖ້າ (!c->serializer->check_assign (out->minConnectorOverlap, minConnectorOverlap, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (false);

    hb_sorted_vector_t<hb_codepoint_t> new_vert_coverage;
    hb_sorted_vector_t<hb_codepoint_t> new_hori_coverage;
    hb_set_t indices;
    collect_coverage_and_indices (new_vert_coverage, vertGlyphCoverage, 0, vertGlyphCount, indices, glyphset, glyph_map);
    collect_coverage_and_indices (new_hori_coverage, horizGlyphCoverage, vertGlyphCount, vertGlyphCount + horizGlyphCount, indices, glyphset, glyph_map);

    ຖ້າ (!c->serializer->check_assign (out->vertGlyphCount, new_vert_coverage.length, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (false);
    ຖ້າ (!c->serializer->check_assign (out->horizGlyphCount, new_hori_coverage.length, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (false);

    ສຳລັບ (unsigned i : indices.iter ())
    {
      auto *o = c->serializer->embed (glyphConstruction[i]);
      ຖ້າ (!o) return_trace (false);
      o->serialize_subset (c, glyphConstruction[i], this);
    }

    ຖ້າ (new_vert_coverage)
      out->vertGlyphCoverage.serialize_serialize (c->serializer, new_vert_coverage.iter ());

    ຖ້າ (new_hori_coverage)
    out->horizGlyphCoverage.serialize_serialize (c->serializer, new_hori_coverage.iter ());
    return_trace (true);
  }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c) const
  {
    ສຳລັບ (const auto &entry : hb_zip (this+vertGlyphCoverage, glyphConstruction.as_array (vertGlyphCount)))
      ຖ້າ (c->glyph_set->has (hb_first (entry)))
        (this+hb_second (entry)).collect_variation_indices (c);
    ສຳລັບ (const auto &entry : hb_zip (this+horizGlyphCoverage, glyphConstruction.as_array (vertGlyphCount + horizGlyphCount).sub_array (vertGlyphCount)))
      ຖ້າ (c->glyph_set->has (hb_first (entry)))
        (this+hb_second (entry)).collect_variation_indices (c);
  }

  bool sanitize_offsets (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    unsigned ຈຳນວນເຕັມ count = vertGlyphCount + horizGlyphCount;
    ສຳລັບ (unsigned ຈຳນວນເຕັມ i = 0; i < count; i++)
      ຖ້າ (!glyphConstruction.arrayZ[i].sanitize (c, this)) return_trace (false);
    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  vertGlyphCoverage.sanitize (c, this) &&
		  horizGlyphCoverage.sanitize (c, this) &&
		  hb_barrier () &&
		  c->check_array (glyphConstruction.arrayZ, vertGlyphCount + horizGlyphCount) &&
		  sanitize_offsets (c));
  }

  hb_position_t get_min_connector_overlap (hb_direction_t direction,
						  hb_font_t *font) const
  { ສົ່ງຄືນ font->em_scale_dir (minConnectorOverlap, direction); }

  unsigned ຈຳນວນເຕັມ get_glyph_variants (hb_codepoint_t glyph,
				   hb_direction_t direction,
				   hb_font_t *font,
				   unsigned ຈຳນວນເຕັມ start_offset,
				   unsigned ຈຳນວນເຕັມ *variants_count, /* IN/OUT */
				   hb_ot_math_glyph_variant_t *variants /* OUT */) const
  { ສົ່ງຄືນ get_glyph_construction (glyph, direction, font)
	   .get_variants (direction, font, start_offset, variants_count, variants); }

  unsigned ຈຳນວນເຕັມ get_glyph_parts (hb_codepoint_t glyph,
				hb_direction_t direction,
				hb_font_t *font,
				unsigned ຈຳນວນເຕັມ start_offset,
				unsigned ຈຳນວນເຕັມ *parts_count, /* IN/OUT */
				hb_ot_math_glyph_part_t *parts /* OUT */,
				hb_position_t *italics_correction /* OUT */) const
  { ສົ່ງຄືນ get_glyph_construction (glyph, direction, font)
	   .get_assembly ()
	   .get_parts (direction, font,
		       start_offset, parts_count, parts,
		       italics_correction); }

  private:
  const MathGlyphConstruction &
  get_glyph_construction (hb_codepoint_t glyph,
			  hb_direction_t direction,
			  hb_font_t *font HB_UNUSED) const
  {
    bool vertical = HB_DIRECTION_IS_VERTICAL (direction);
    unsigned ຈຳນວນເຕັມ count = vertical ? vertGlyphCount : horizGlyphCount;
    const Offset16To<Coverage> &coverage = vertical ? vertGlyphCoverage
						  : horizGlyphCoverage;

    unsigned ຈຳນວນເຕັມ index = (this+coverage).get_coverage (glyph);
    ຖ້າ (unlikely (index >= count)) ສົ່ງຄືນ Null (MathGlyphConstruction);

    ຖ້າ (!vertical)
      index += vertGlyphCount;

    ສົ່ງຄືນ this+glyphConstruction[index];
  }

  rak_sita:
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

  public:
  DEFINE_SIZE_ARRAY (10, glyphConstruction);
};


/*
 * MATH -- Mathematical typesetting
 * https://docs.microsoft.com/en-us/typography/opentype/spec/math
 */

sa_nracanaa MATH
{
  static constexpr hb_tag_t tableTag = HB_OT_TAG_MATH;

  bool has_data () const { ສົ່ງຄືນ version.to_int (); }

  void depend (hb_depend_data_builder_t *depend_data) const
  {
    ຖ້າ (mathVariants)
      (this+mathVariants).depend (depend_data);
  }

  void closure_glyphs (hb_set_t *glyph_set) const
  {
    ຖ້າ (mathVariants)
    {
      hb_set_t variant_glyphs;
      (this+mathVariants).closure_glyphs (glyph_set, &variant_glyphs);
      hb_set_union (glyph_set, &variant_glyphs);
    }
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->embed (*this);
    ຖ້າ (unlikely (!out)) return_trace (false);

    out->mathConstants.serialize_copy (c->serializer, mathConstants, this, 0, hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map);
    out->mathGlyphInfo.serialize_subset (c, mathGlyphInfo, this);
    out->mathVariants.serialize_subset (c, mathVariants, this);
    return_trace (true);
  }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c, const hb_set_t *variant_glyph_set) const
  {
    (this+mathConstants).collect_variation_indices (c);
    (this+mathGlyphInfo).collect_variation_indices (c);
    auto *glyph_set = c->glyph_set;
    c->glyph_set = variant_glyph_set;
    (this+mathVariants).collect_variation_indices (c);
    c->glyph_set = glyph_set;
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (version.sanitize (c) &&
		  likely (version.major == 1) &&
		  hb_barrier () &&
		  mathConstants.sanitize (c, this) &&
		  mathGlyphInfo.sanitize (c, this) &&
		  mathVariants.sanitize (c, this));
  }

  // https://github.com/harfbuzz/harfbuzz/issues/4653
  HB_INTERNAL bool is_bad_cambria (hb_font_t *font) const
  {
#ifndef HB_NO_MATH
    switch HB_CODEPOINT_ENCODE3 (font->face->table.MATH.get_blob ()->length,
                                 (this+mathConstants).minHeight[1], // displayOperatorMinHeight
                                 (this+mathConstants).minHeight[0]) // delimitedSubFormulaMinHeight
    {
      /* sha1sum:ab4a4fe054d23061f3c039493d6f665cfda2ecf5  cambria.ttc
       * sha1sum:086855301bff644f9d8827b88491fcf73a6d4cb9  cambria.ttc
       * sha1sum:b1e5a3feaca2ea3dfcf79ccb377de749ecf60343  cambria.ttc */
      pak_sa HB_CODEPOINT_ENCODE3 (25722, 2500, 3000):
        ສົ່ງຄືນ true;
    }
#endif
    ສົ່ງຄືນ false;
  }

  hb_position_t get_constant (hb_ot_math_constant_t  constant,
			      hb_font_t		   *font) const
  { ສົ່ງຄືນ (this+mathConstants).get_value (constant, font); }

  const MathGlyphInfo &get_glyph_info () const { ສົ່ງຄືນ this+mathGlyphInfo; }

  const MathVariants &get_variants () const    { ສົ່ງຄືນ this+mathVariants; }

  rak_sita:
  FixedVersion<>version;	/* Version of the MATH table
				 * initially set to 0x00010000u */
  Offset16To<MathConstants>
		mathConstants;	/* MathConstants table */
  Offset16To<MathGlyphInfo>
		mathGlyphInfo;	/* MathGlyphInfo table */
  Offset16To<MathVariants>
		mathVariants;	/* MathVariants table */

  public:
  DEFINE_SIZE_STATIC (10);
};

} /* namespace OT */


#endif /* HB_OT_MATH_TABLE_HH */

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

#جيڪڏهن_وصف_نه_ٿيل HB_OT_MATH_جدول_HH
#وصف_ڪريو HB_OT_MATH_جدول_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-layout-common.hh"
#ضمن "hb-ot-layout-gdef-table.hh"
#ضمن "hb-ot-math.h"
#ضمن "hb-depend-data.hh"

نطاق OT {


بنية Math_قدر_رڪارڊ
{
  hb_position_t get_x_قدر (hb_font_t *font, ثابت فراغ *بنياد) ثابت
  { أرجع hb_saturate_جوڙيو (font->em_scale_x (value), (بنياد+device_جدول).get_x_delta (font, get_var_store (font, بنياد))); }
  hb_position_t get_y_قدر (hb_font_t *font, ثابت فراغ *بنياد) ثابت
  { أرجع hb_saturate_جوڙيو (font->em_scale_y (value), (بنياد+device_جدول).get_y_delta (font, get_var_store (font, بنياد))); }

  ثابت شي_Variation_Store &get_var_store (hb_font_t *font, ثابت فراغ *بنياد) ثابت
  {
#إذا !defined(HB_NO_VAR) && !defined(HB_NO_OT_LAYOUT)
    إذا ((بنياد+device_جدول).is_variation_device ())
      أرجع font->face->جدول.GDEF->جدول->get_var_store ();
#جيڪڏهن_پورو
    أرجع Null (شي_Variation_Store);
  }

  Math_قدر_رڪارڊ* copy (hb_serialize_context_t *c, ثابت فراغ *بنياد,
                         ثابت hb_hashmap_t<بنانشان, hb_pair_t<بنانشان, صحيح>> *variation_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودڪار *ٻاهريون = c->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خالي_اشارو);
    hb_pair_t<بنانشان, صحيح> *varidx_delta;
    إذا (variation_map->has ((بنياد+device_جدول).get_variation_اشاريو (), &varidx_delta) &&
        !c->check_سونپيو (ٻاهريون->value, (int64_t) value + hb_ٻيو (*varidx_delta),
                          HB_SERIALIZE_ERROR_INT_حد_کان_مٿي))
      return_trace (خالي_اشارو);
    ٻاهريون->device_جدول.serialize_copy (c, device_جدول, بنياد, 0, hb_serialize_context_t::سر, variation_map);

    return_trace (ٻاهريون);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c, ثابت فراغ *بنياد) ثابت
  { (بنياد+device_جدول).collect_variation_indices (c); }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && device_جدول.sanitize (c, بنياد));
  }

  محمي:
  HBINT16		value;		/* The X or Y value in design units */
  Offset16_منزل<Device>	device_جدول;	/* Offset to the device table - from the
					 * beginning of parent table.  May be NULL.
					 * Suggested format for device table is 1. */

  عام:
  DEFINE_سائيز_STATIC (4);
};

بنية MathConstants
{
  دوست بنية MATH;

  MathConstants* copy (hb_serialize_context_t *c,
                      ثابت hb_hashmap_t<بنانشان, hb_pair_t<بنانشان, صحيح>> *variation_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودڪار *ٻاهريون = c->شروعات_embed (هذا);

    HBINT16 *p = c->allocate_سائيز<HBINT16> (HBINT16::static_سائيز * 2);
    إذا (unlikely (!p)) return_trace (خالي_اشارو);
    hb_memcpy (p, percent_Scale_هيٺ_ڏانهن, HBINT16::static_سائيز * 2);

    HBUINT16 *m = c->allocate_سائيز<HBUINT16> (HBUINT16::static_سائيز * 2);
    إذا (unlikely (!m)) return_trace (خالي_اشارو);
    hb_memcpy (m, min_اوچائي, HBUINT16::static_سائيز * 2);

    بنانشان ڳڻپ = ARRAY_ڊيگهائي (math_قدر_رڪارڊون);
    لكل (بنانشان i = 0; i < ڳڻپ; i++)
      إذا (!c->copy (math_قدر_رڪارڊون[i], هذا, variation_map))
        return_trace (خالي_اشارو);

    إذا (!c->embed (radicalDegreeBottomRaisePercent)) return_trace (خالي_اشارو);
    return_trace (ٻاهريون);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت خودڪار &رڪارڊ : math_قدر_رڪارڊون)
      رڪارڊ.collect_variation_indices (c, هذا);
  }

  منطقي sanitize_math_قدر_رڪارڊون (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);

    بنانشان صحيح ڳڻپ = ARRAY_ڊيگهائي (math_قدر_رڪارڊون);
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
      إذا (!math_قدر_رڪارڊون[i].sanitize (c, هذا))
	return_trace (خطأ);

    return_trace (سچ);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && sanitize_math_قدر_رڪارڊون (c));
  }

  hb_position_t get_قدر (hb_ot_math_constant_t constant,
			   hb_font_t *font) ثابت
  {
    بدل (constant) {

    حالة HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_هيٺ_ڏانهن:
    حالة HB_OT_MATH_CONSTANT_SCRIPT_SCRIPT_PERCENT_SCALE_هيٺ_ڏانهن:
      أرجع percent_Scale_هيٺ_ڏانهن[constant - HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_هيٺ_ڏانهن];

    حالة HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_اوچائي:
    حالة HB_OT_MATH_CONSTANT_DISPLAY_OPERATOR_MIN_اوچائي:
      أرجع font->em_scale_y (min_اوچائي[constant - HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_اوچائي]);

    حالة HB_OT_MATH_CONSTANT_RADICAL_KERN_AFTER_DEGREE:
    حالة HB_OT_MATH_CONSTANT_RADICAL_KERN_BEFORE_DEGREE:
    حالة HB_OT_MATH_CONSTANT_SKEWED_FRACTION_HORIZONTAL_GAP:
    حالة HB_OT_MATH_CONSTANT_SPACE_AFTER_SCRIPT:
      أرجع math_قدر_رڪارڊون[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_x_قدر (font, هذا);

    حالة HB_OT_MATH_CONSTANT_ACCENT_بنياد_اوچائي:
    حالة HB_OT_MATH_CONSTANT_AXIS_اوچائي:
    حالة HB_OT_MATH_CONSTANT_FLATTENED_ACCENT_بنياد_اوچائي:
    حالة HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_DISPLAY_STYLE_سرڪايو_هيٺ_ڏانهن:
    حالة HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_سرڪايو_هيٺ_ڏانهن:
    حالة HB_OT_MATH_CONSTANT_FRACTION_DENOM_DISPLAY_STYLE_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_DISPLAY_STYLE_سرڪايو_مٿي:
    حالة HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_سرڪايو_مٿي:
    حالة HB_OT_MATH_CONSTANT_FRACTION_انگ_DISPLAY_STYLE_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_FRACTION_قاعدو_THICKNESS:
    حالة HB_OT_MATH_CONSTANT_هيٺاهون_حد_بندي_BASELINE_DROP_MIN:
    حالة HB_OT_MATH_CONSTANT_هيٺاهون_حد_بندي_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_MATH_LEADING:
    حالة HB_OT_MATH_CONSTANT_OVERBAR_EXTRA_ASCENDER:
    حالة HB_OT_MATH_CONSTANT_OVERBAR_قاعدو_THICKNESS:
    حالة HB_OT_MATH_CONSTANT_OVERBAR_VERTICAL_GAP:
    حالة HB_OT_MATH_CONSTANT_RADICAL_DISPLAY_STYLE_VERTICAL_GAP:
    حالة HB_OT_MATH_CONSTANT_RADICAL_EXTRA_ASCENDER:
    حالة HB_OT_MATH_CONSTANT_RADICAL_قاعدو_THICKNESS:
    حالة HB_OT_MATH_CONSTANT_RADICAL_VERTICAL_GAP:
    حالة HB_OT_MATH_CONSTANT_SKEWED_FRACTION_VERTICAL_GAP:
    حالة HB_OT_MATH_CONSTANT_ڍير_BOTTOM_DISPLAY_STYLE_سرڪايو_هيٺ_ڏانهن:
    حالة HB_OT_MATH_CONSTANT_ڍير_BOTTOM_سرڪايو_هيٺ_ڏانهن:
    حالة HB_OT_MATH_CONSTANT_ڍير_DISPLAY_STYLE_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_ڍير_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_ڍير_چوٽي_DISPLAY_STYLE_سرڪايو_مٿي:
    حالة HB_OT_MATH_CONSTANT_ڍير_چوٽي_سرڪايو_مٿي:
    حالة HB_OT_MATH_CONSTANT_STRETCH_ڍير_BOTTOM_سرڪايو_هيٺ_ڏانهن:
    حالة HB_OT_MATH_CONSTANT_STRETCH_ڍير_GAP_ABOVE_MIN:
    حالة HB_OT_MATH_CONSTANT_STRETCH_ڍير_GAP_هيٺ_MIN:
    حالة HB_OT_MATH_CONSTANT_STRETCH_ڍير_چوٽي_سرڪايو_مٿي:
    حالة HB_OT_MATH_CONSTANT_SUBSCRIPT_BASELINE_DROP_MIN:
    حالة HB_OT_MATH_CONSTANT_SUBSCRIPT_سرڪايو_هيٺ_ڏانهن:
    حالة HB_OT_MATH_CONSTANT_SUBSCRIPT_چوٽي_MAX:
    حالة HB_OT_MATH_CONSTANT_SUB_SUPERSCRIPT_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_SUPERSCRIPT_BASELINE_DROP_MAX:
    حالة HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MAX_WITH_SUBSCRIPT:
    حالة HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MIN:
    حالة HB_OT_MATH_CONSTANT_SUPERSCRIPT_سرڪايو_مٿي:
    حالة HB_OT_MATH_CONSTANT_SUPERSCRIPT_سرڪايو_مٿي_CRAMPED:
    حالة HB_OT_MATH_CONSTANT_UNDERBAR_EXTRA_DESCENDER:
    حالة HB_OT_MATH_CONSTANT_UNDERBAR_قاعدو_THICKNESS:
    حالة HB_OT_MATH_CONSTANT_UNDERBAR_VERTICAL_GAP:
    حالة HB_OT_MATH_CONSTANT_مٿانهون_حد_بندي_BASELINE_RISE_MIN:
    حالة HB_OT_MATH_CONSTANT_مٿانهون_حد_بندي_GAP_MIN:
      أرجع math_قدر_رڪارڊون[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_y_قدر (font, هذا);

    حالة HB_OT_MATH_CONSTANT_RADICAL_DEGREE_BOTTOM_RAISE_PERCENT:
      أرجع radicalDegreeBottomRaisePercent;

    افتراضي:
      أرجع 0;
    }
  }

  محمي:
  HBINT16 percent_Scale_هيٺ_ڏانهن[2];
  HBUINT16 min_اوچائي[2];
  Math_قدر_رڪارڊ math_قدر_رڪارڊون[51];
  HBINT16 radicalDegreeBottomRaisePercent;

  عام:
  DEFINE_سائيز_STATIC (214);
};

بنية MathItalicsCorrectionInfo
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سيٽ_t &glyphset = c->plan->_glyphset_mathed;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);

    hb_sorted_ويڪٽر_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+coverage, italicsCorrection)
    | hb_filter (glyphset, hb_پهريون)
    | hb_filter (serialize_math_رڪارڊ_array (c->serializer, ٻاهريون->italicsCorrection, هذا, &c->plan->layout_variation_idx_delta_map), hb_ٻيو)
    | hb_map (hb_پهريون)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    ٻاهريون->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (سچ);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت خودڪار &داخلا : hb_zip (هذا+coverage, italicsCorrection))
      إذا (c->glyph_سيٽ->has (hb_پهريون (داخلا)))
        hb_ٻيو (داخلا).collect_variation_indices (c, هذا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  coverage.sanitize (c, هذا) &&
		  italicsCorrection.sanitize (c, هذا));
  }

  hb_position_t get_قدر (hb_codepoint_t glyph,
			   hb_font_t *font) ثابت
  {
    بنانشان صحيح اشاريو_2 = (هذا+coverage).get_coverage (glyph);
    أرجع italicsCorrection[اشاريو_2].get_x_قدر (font, هذا);
  }

  محمي:
  Offset16_منزل<Coverage>       coverage;		/* Offset to Coverage table -
						 * from the beginning of
						 * MathItalicsCorrectionInfo
						 * table. */
  Array16_جو<Math_قدر_رڪارڊ> italicsCorrection;	/* Array of MathValueRecords
						 * defining italics correction
						 * values for each
						 * covered glyph. */

  عام:
  DEFINE_سائيز_ARRAY (4, italicsCorrection);
};

بنية Math_چوٽي_Accent_Attachment
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سيٽ_t &glyphset = c->plan->_glyphset_mathed;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);

    hb_sorted_ويڪٽر_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+چوٽي_Accent_Coverage, چوٽي_Accent_Attachment)
    | hb_filter (glyphset, hb_پهريون)
    | hb_filter (serialize_math_رڪارڊ_array (c->serializer, ٻاهريون->چوٽي_Accent_Attachment, هذا, &c->plan->layout_variation_idx_delta_map), hb_ٻيو)
    | hb_map (hb_پهريون)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    ٻاهريون->چوٽي_Accent_Coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (سچ);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت خودڪار &داخلا : hb_zip (هذا+چوٽي_Accent_Coverage, چوٽي_Accent_Attachment))
      إذا (c->glyph_سيٽ->has (hb_پهريون (داخلا)))
        hb_ٻيو (داخلا).collect_variation_indices (c, هذا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  چوٽي_Accent_Coverage.sanitize (c, هذا) &&
		  چوٽي_Accent_Attachment.sanitize (c, هذا));
  }

  hb_position_t get_قدر (hb_codepoint_t glyph,
			   hb_font_t *font) ثابت
  {
    بنانشان صحيح اشاريو_2 = (هذا+چوٽي_Accent_Coverage).get_coverage (glyph);
    إذا (اشاريو_2 == NOT_COVERED)
      أرجع font->get_glyph_h_advance (glyph) / 2;
    أرجع چوٽي_Accent_Attachment[اشاريو_2].get_x_قدر (font, هذا);
  }

  محمي:
  Offset16_منزل<Coverage>       چوٽي_Accent_Coverage;   /* Offset to Coverage table -
						 * from the beginning of
						 * MathTopAccentAttachment
						 * table. */
  Array16_جو<Math_قدر_رڪارڊ> چوٽي_Accent_Attachment; /* Array of MathValueRecords
						 * defining top accent
						 * attachment points for each
						 * covered glyph. */

  عام:
  DEFINE_سائيز_ARRAY (2 + 2, چوٽي_Accent_Attachment);
};

بنية MathKern
{
  MathKern* copy (hb_serialize_context_t *c,
                      ثابت hb_hashmap_t<بنانشان, hb_pair_t<بنانشان, صحيح>> *variation_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودڪار *ٻاهريون = c->شروعات_embed (هذا);

    إذا (unlikely (!c->embed (اوچائي_ڳڻپ))) return_trace (خالي_اشارو);

    بنانشان ڳڻپ = 2 * اوچائي_ڳڻپ + 1;
    لكل (بنانشان i = 0; i < ڳڻپ; i++)
      إذا (!c->copy (math_قدر_رڪارڊون_Z.arrayZ[i], هذا, variation_map))
        return_trace (خالي_اشارو);

    return_trace (ٻاهريون);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت خودڪار &رڪارڊ : math_قدر_رڪارڊون_Z.as_array (2 * اوچائي_ڳڻپ + 1))
      رڪارڊ.collect_variation_indices (c, هذا);
  }

  منطقي sanitize_math_قدر_رڪارڊون (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    بنانشان صحيح ڳڻپ = 2 * اوچائي_ڳڻپ + 1;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
      إذا (!math_قدر_رڪارڊون_Z.arrayZ[i].sanitize (c, هذا)) return_trace (خطأ);
    return_trace (سچ);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  c->check_array (math_قدر_رڪارڊون_Z.arrayZ, 2 * اوچائي_ڳڻپ + 1) &&
		  sanitize_math_قدر_رڪارڊون (c));
  }

  hb_position_t get_قدر (hb_position_t correction_اوچائي, hb_font_t *font) ثابت
  {
    ثابت Math_قدر_رڪارڊ* correction_اوچائي_2 = math_قدر_رڪارڊون_Z.arrayZ;
    ثابت Math_قدر_رڪارڊ* kern_قدر = math_قدر_رڪارڊون_Z.arrayZ + اوچائي_ڳڻپ;
    صحيح sign = font->y_scale < 0 ? -1 : +1;

    /* According to OpenType spec (v1.9), except for the boundary cases, the index
     * chosen for kern value should be i such that
     *    correctionHeight[i-1] <= correction_height < correctionHeight[i]
     * We can use the binary search algorithm of std::upper_bound(). Or, we can
     * use the internal hb_bsearch_impl.
     */
    بنانشان صحيح pos;
    خودڪار cmp = +[](ثابت فراغ* key, ثابت فراغ* p,
                   صحيح sign, hb_font_t* font, ثابت MathKern* mathKern) -> صحيح {
      hb_position_t a = *(hb_position_t *) key;
      hb_position_t b = ((Math_قدر_رڪارڊ *) p)->get_y_قدر (font, mathKern);
      إذا (a == b) أرجع 0;
      أرجع sign > 0 ? (a < b ? -1 : +1) : (b < a ? -1 : +1);
    };
    بنانشان صحيح i = hb_bsearch_impl(&pos, correction_اوچائي, correction_اوچائي_2,
                                     اوچائي_ڳڻپ, Math_قدر_رڪارڊ::static_سائيز,
                                     cmp, sign, font, هذا) ? pos + 1 : pos;
    أرجع kern_قدر[i].get_x_قدر (font, هذا);
  }

  بنانشان صحيح get_entries (بنانشان صحيح شروعات_وٿي,
			    بنانشان صحيح *entries_ڳڻپ, /* IN/OUT */
			    hb_ot_math_kern_داخلا_t *kern_entries, /*ٻاهريون*/
			    hb_font_t *font) ثابت
  {
    ثابت Math_قدر_رڪارڊ* correction_اوچائي_2 = math_قدر_رڪارڊون_Z.arrayZ;
    ثابت Math_قدر_رڪارڊ* kern_قدر = math_قدر_رڪارڊون_Z.arrayZ + اوچائي_ڳڻپ;
    ثابت بنانشان صحيح entries_ڳڻپ_2 = اوچائي_ڳڻپ + 1;

    إذا (entries_ڳڻپ && kern_entries)
    {
      بنانشان صحيح شروعات = hb_min (شروعات_وٿي, entries_ڳڻپ_2);
      بنانشان صحيح پڇاڙي = hb_min (شروعات + *entries_ڳڻپ, entries_ڳڻپ_2);
      *entries_ڳڻپ = پڇاڙي - شروعات;

      لكل (بنانشان صحيح i = 0; i < *entries_ڳڻپ; i++) {
	بنانشان صحيح j = شروعات + i;

	hb_position_t max_اوچائي;
	إذا (j == اوچائي_ڳڻپ) {
	  max_اوچائي = INT32_MAX;
	} وإلا {
	  max_اوچائي = correction_اوچائي_2[j].get_y_قدر (font, هذا);
	}

	kern_entries[i] = {max_اوچائي, kern_قدر[j].get_x_قدر (font, هذا)};
      }
    }
    أرجع entries_ڳڻپ_2;
  }

  محمي:
  HBUINT16	اوچائي_ڳڻپ;
  Unsized_Array_جو<Math_قدر_رڪارڊ>
		math_قدر_رڪارڊون_Z;
				/* Array of correction heights at
				 * which the kern value changes.
				 * Sorted by the height value in
				 * design units (heightCount entries),
				 * Followed by:
				 * Array of kern values corresponding
				 * to heights. (heightCount+1 entries).
				 */

  عام:
  DEFINE_سائيز_ARRAY (2, math_قدر_رڪارڊون_Z);
};

بنية Math_Kern_Info_رڪارڊ
{
  Math_Kern_Info_رڪارڊ* copy (hb_serialize_context_t *c, ثابت فراغ *بنياد,
                            ثابت hb_hashmap_t<بنانشان, hb_pair_t<بنانشان, صحيح>> *variation_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودڪار *ٻاهريون = c->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خالي_اشارو);

    بنانشان ڳڻپ = ARRAY_ڊيگهائي (mathKern);
    لكل (بنانشان i = 0; i < ڳڻپ; i++)
      ٻاهريون->mathKern[i].serialize_copy (c, mathKern[i], بنياد, 0, hb_serialize_context_t::سر, variation_map);

    return_trace (ٻاهريون);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c, ثابت فراغ *بنياد) ثابت
  {
    لكل (ثابت خودڪار &وٿي : mathKern)
      (بنياد+وٿي).collect_variation_indices (c);
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);

    بنانشان صحيح ڳڻپ = ARRAY_ڊيگهائي (mathKern);
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
      إذا (unlikely (!mathKern[i].sanitize (c, بنياد)))
	return_trace (خطأ);

    return_trace (سچ);
  }

  hb_position_t get_kerning (hb_ot_math_kern_t kern,
			     hb_position_t correction_اوچائي,
			     hb_font_t *font,
			     ثابت فراغ *بنياد) ثابت
  {
    بنانشان صحيح idx = kern;
    إذا (unlikely (idx >= ARRAY_ڊيگهائي (mathKern))) أرجع 0;
    أرجع (بنياد+mathKern[idx]).get_قدر (correction_اوچائي, font);
  }

  بنانشان صحيح get_kernings (hb_ot_math_kern_t kern,
			     بنانشان صحيح شروعات_وٿي,
			     بنانشان صحيح *entries_ڳڻپ, /* IN/OUT */
			     hb_ot_math_kern_داخلا_t *kern_entries, /*ٻاهريون*/
			     hb_font_t *font,
			     ثابت فراغ *بنياد) ثابت
  {
    بنانشان صحيح idx = kern;
    إذا (unlikely (idx >= ARRAY_ڊيگهائي (mathKern)) || !mathKern[idx]) {
      إذا (entries_ڳڻپ) *entries_ڳڻپ = 0;
      أرجع 0;
    }
    أرجع (بنياد+mathKern[idx]).get_entries (شروعات_وٿي,
					     entries_ڳڻپ,
					     kern_entries,
					     font);
  }

  محمي:
  /* Offset to MathKern table for each corner -
   * from the beginning of MathKernInfo table.  May be NULL. */
  Offset16_منزل<MathKern> mathKern[4];

  عام:
  DEFINE_سائيز_STATIC (8);
};

بنية MathKernInfo
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سيٽ_t &glyphset = c->plan->_glyphset_mathed;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);

    hb_sorted_ويڪٽر_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+mathKernCoverage, math_Kern_Info_رڪارڊون)
    | hb_filter (glyphset, hb_پهريون)
    | hb_filter (serialize_math_رڪارڊ_array (c->serializer, ٻاهريون->math_Kern_Info_رڪارڊون, هذا, &c->plan->layout_variation_idx_delta_map), hb_ٻيو)
    | hb_map (hb_پهريون)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    ٻاهريون->mathKernCoverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (سچ);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت خودڪار &داخلا : hb_zip (هذا+mathKernCoverage, math_Kern_Info_رڪارڊون))
      إذا (c->glyph_سيٽ->has (hb_پهريون (داخلا)))
        hb_ٻيو (داخلا).collect_variation_indices (c, هذا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  mathKernCoverage.sanitize (c, هذا) &&
		  math_Kern_Info_رڪارڊون.sanitize (c, هذا));
  }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_اوچائي,
			     hb_font_t *font) ثابت
  {
    بنانشان صحيح اشاريو_2 = (هذا+mathKernCoverage).get_coverage (glyph);
    أرجع math_Kern_Info_رڪارڊون[اشاريو_2].get_kerning (kern, correction_اوچائي, font, هذا);
  }

  بنانشان صحيح get_kernings (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     بنانشان صحيح شروعات_وٿي,
			     بنانشان صحيح *entries_ڳڻپ, /* IN/OUT */
			     hb_ot_math_kern_داخلا_t *kern_entries, /*ٻاهريون*/
			     hb_font_t *font) ثابت
  {
    بنانشان صحيح اشاريو_2 = (هذا+mathKernCoverage).get_coverage (glyph);
    أرجع math_Kern_Info_رڪارڊون[اشاريو_2].get_kernings (kern,
						    شروعات_وٿي,
						    entries_ڳڻپ,
						    kern_entries,
						    font,
						    هذا);
  }

  محمي:
  Offset16_منزل<Coverage>
		mathKernCoverage;
				/* Offset to Coverage table -
				 * from the beginning of the
				 * MathKernInfo table. */
  Array16_جو<Math_Kern_Info_رڪارڊ>
		math_Kern_Info_رڪارڊون;
				/* Array of MathKernInfoRecords,
				 * per-glyph information for
				 * mathematical positioning
				 * of subscripts and
				 * superscripts. */

  عام:
  DEFINE_سائيز_ARRAY (4, math_Kern_Info_رڪارڊون);
};

بنية MathGlyphInfo
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->embed (*هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    ٻاهريون->mathItalicsCorrectionInfo.serialize_subset (c, mathItalicsCorrectionInfo, هذا);
    ٻاهريون->math_چوٽي_Accent_Attachment.serialize_subset (c, math_چوٽي_Accent_Attachment, هذا);

    ثابت hb_سيٽ_t &glyphset = c->plan->_glyphset_mathed;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودڪار it =
    + hb_iter (هذا+extendedShapeCoverage)
    | hb_take (c->plan->ذريعو->get_انگ_glyphs ())
    | hb_filter (glyphset)
    | hb_map_retains_ترتيب_ڏيڻ (glyph_map)
    ;

    إذا (it) ٻاهريون->extendedShapeCoverage.serialize_serialize (c->serializer, it);
    وإلا ٻاهريون->extendedShapeCoverage = 0;

    ٻاهريون->mathKernInfo.serialize_subset (c, mathKernInfo, هذا);
    return_trace (سچ);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    (هذا+mathItalicsCorrectionInfo).collect_variation_indices (c);
    (هذا+math_چوٽي_Accent_Attachment).collect_variation_indices (c);
    (هذا+mathKernInfo).collect_variation_indices (c);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  mathItalicsCorrectionInfo.sanitize (c, هذا) &&
		  math_چوٽي_Accent_Attachment.sanitize (c, هذا) &&
		  extendedShapeCoverage.sanitize (c, هذا) &&
		  mathKernInfo.sanitize (c, هذا));
  }

  hb_position_t
  get_italics_correction (hb_codepoint_t  glyph, hb_font_t *font) ثابت
  { أرجع (هذا+mathItalicsCorrectionInfo).get_قدر (glyph, font); }

  hb_position_t
  get_چوٽي_accent_attachment (hb_codepoint_t  glyph, hb_font_t *font) ثابت
  { أرجع (هذا+math_چوٽي_Accent_Attachment).get_قدر (glyph, font); }

  منطقي is_extended_shape (hb_codepoint_t glyph) ثابت
  { أرجع (هذا+extendedShapeCoverage).get_coverage (glyph) != NOT_COVERED; }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_اوچائي,
			     hb_font_t *font) ثابت
  { أرجع (هذا+mathKernInfo).get_kerning (glyph, kern, correction_اوچائي, font); }

  hb_position_t get_kernings (hb_codepoint_t glyph,
			      hb_ot_math_kern_t kern,
			      بنانشان صحيح شروعات_وٿي,
			      بنانشان صحيح *entries_ڳڻپ, /* IN/OUT */
			      hb_ot_math_kern_داخلا_t *kern_entries, /*ٻاهريون*/
			      hb_font_t *font) ثابت
  { أرجع (هذا+mathKernInfo).get_kernings (glyph,
					     kern,
					     شروعات_وٿي,
					     entries_ڳڻپ,
					     kern_entries,
					     font); }

  محمي:
  /* Offset to MathItalicsCorrectionInfo table -
   * from the beginning of MathGlyphInfo table. */
  Offset16_منزل<MathItalicsCorrectionInfo> mathItalicsCorrectionInfo;

  /* Offset to MathTopAccentAttachment table -
   * from the beginning of MathGlyphInfo table. */
  Offset16_منزل<Math_چوٽي_Accent_Attachment> math_چوٽي_Accent_Attachment;

  /* Offset to coverage table for Extended Shape glyphs -
   * from the beginning of MathGlyphInfo table. When the left or right glyph of
   * a box is an extended shape variant, the (ink) box (and not the default
   * position defined by values in MathConstants table) should be used for
   * vertical positioning purposes.  May be NULL.. */
  Offset16_منزل<Coverage> extendedShapeCoverage;

   /* Offset to MathKernInfo table -
    * from the beginning of MathGlyphInfo table. */
  Offset16_منزل<MathKernInfo> mathKernInfo;

  عام:
  DEFINE_سائيز_STATIC (8);
};

بنية Math_Glyph_Variant_رڪارڊ
{
  دوست بنية MathGlyphConstruction;

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    ثابت hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_سونپيو (ٻاهريون->variantGlyph, glyph_map.get (variantGlyph), HB_SERIALIZE_ERROR_INT_حد_کان_مٿي));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  فراغ depend (hb_depend_معلومات_builder_t *depend_معلومات, بنانشان ذريعو) ثابت
  { depend_معلومات->جوڙيو_depend(ذريعو, HB_OT_ٽيگ_MATH, variantGlyph); }

  فراغ گهيرو_glyphs (hb_سيٽ_t *variant_glyphs) ثابت
  { variant_glyphs->جوڙيو (variantGlyph); }

  محمي:
  HBGlyphID16 variantGlyph;       /* Glyph ID for the variant. */
  HBUINT16  advanceMeasurement; /* Advance width/height, in design units, of the
				 * variant, in the direction of requested
				 * glyph extension. */

  عام:
  DEFINE_سائيز_STATIC (4);
};

بنية PartFlags : HBUINT16
{
  تعداد Flags {
    Extender	= 0x0001u, /* If set, the part can be skipped or repeated. */

    Defined	= 0x0001u, /* All defined flags. */
  };

  عام:
  DEFINE_سائيز_STATIC (2);
};

بنية Math_Glyph_Part_رڪارڊ
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    ثابت hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_سونپيو (ٻاهريون->glyph, glyph_map.get (glyph), HB_SERIALIZE_ERROR_INT_حد_کان_مٿي));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  فراغ extract (hb_ot_math_glyph_part_t &ٻاهريون,
		int64_t mult,
		hb_font_t *font) ثابت
  {
    ٻاهريون.glyph			= glyph;

    ٻاهريون.شروعات_connector_ڊيگهائي	= font->em_mult (شروعات_Connector_ڊيگهائي, mult);
    ٻاهريون.پڇاڙي_connector_ڊيگهائي	= font->em_mult (پڇاڙي_Connector_ڊيگهائي, mult);
    ٻاهريون.ڀريل_advance		= font->em_mult (ڀريل_Advance, mult);

    static_assert ((بنانشان صحيح) HB_OT_MATH_GLYPH_PART_FLAG_EXTENDER ==
		   (بنانشان صحيح) PartFlags::Extender, "");

    ٻاهريون.flags = (hb_ot_math_glyph_part_flags_t)
		(بنانشان صحيح)
		(partFlags & PartFlags::Defined);
  }

  فراغ depend (hb_depend_معلومات_builder_t *depend_معلومات, بنانشان ذريعو) ثابت
  { depend_معلومات->جوڙيو_depend(ذريعو, HB_OT_ٽيگ_MATH, glyph); }

  فراغ گهيرو_glyphs (hb_سيٽ_t *variant_glyphs) ثابت
  { variant_glyphs->جوڙيو (glyph); }

  محمي:
  HBGlyphID16	glyph;		/* Glyph ID for the part. */
  HBUINT16	شروعات_Connector_ڊيگهائي;
				/* Advance width/ height of the straight bar
				 * connector material, in design units, is at
				 * the beginning of the glyph, in the
				 * direction of the extension. */
  HBUINT16	پڇاڙي_Connector_ڊيگهائي;
				/* Advance width/ height of the straight bar
				 * connector material, in design units, is at
				 * the end of the glyph, in the direction of
				 * the extension. */
  HBUINT16	ڀريل_Advance;	/* Full advance width/height for this part,
				 * in the direction of the extension.
				 * In design units. */
  PartFlags	partFlags;	/* Part qualifiers. */

  عام:
  DEFINE_سائيز_STATIC (10);
};

بنية MathGlyphAssembly
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);

    إذا (!c->serializer->copy (italicsCorrection, هذا, &c->plan->layout_variation_idx_delta_map)) return_trace (خطأ);
    إذا (!c->serializer->copy<HBUINT16> (part_رڪارڊون.len)) return_trace (خطأ);

    لكل (ثابت خودڪار& رڪارڊ : part_رڪارڊون.iter ())
      إذا (!رڪارڊ.subset (c)) return_trace (خطأ);
    return_trace (سچ);
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
		  part_رڪارڊون.sanitize (c));
  }

  بنانشان صحيح get_parts (hb_direction_t direction,
			  hb_font_t *font,
			  بنانشان صحيح شروعات_وٿي,
			  بنانشان صحيح *parts_ڳڻپ, /* IN/OUT */
			  hb_ot_math_glyph_part_t *parts /*ٻاهريون*/,
			  hb_position_t *italics_correction /*ٻاهريون*/) ثابت
  {
    إذا (parts_ڳڻپ && parts)
    {
      int64_t mult = font->dir_mult (direction);
      لكل (خودڪار _ : hb_zip (part_رڪارڊون.as_array ().sub_array (شروعات_وٿي, parts_ڳڻپ),
			    hb_array (parts, *parts_ڳڻپ)))
	_.پهريون.extract (_.second, mult, font);
    }

    إذا (italics_correction)
      *italics_correction = italicsCorrection.get_x_قدر (font, هذا);

    أرجع part_رڪارڊون.len;
  }

  فراغ depend (hb_depend_معلومات_builder_t *depend_معلومات, بنانشان ذريعو) ثابت
  {
    لكل (ثابت خودڪار& _ : part_رڪارڊون.iter ())
      _.depend (depend_معلومات, ذريعو);
  }

  فراغ گهيرو_glyphs (hb_سيٽ_t *variant_glyphs) ثابت
  {
    لكل (ثابت خودڪار& _ : part_رڪارڊون.iter ())
      _.گهيرو_glyphs (variant_glyphs);
  }

  محمي:
  Math_قدر_رڪارڊ
		italicsCorrection;
				/* Italics correction of this
				 * MathGlyphAssembly. Should not
				 * depend on the assembly size. */
  Array16_جو<Math_Glyph_Part_رڪارڊ>
		part_رڪارڊون;	/* Array of part records, from
				 * left to right and bottom to
				 * top. */

  عام:
  DEFINE_سائيز_ARRAY (6, part_رڪارڊون);
};

بنية MathGlyphConstruction
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);

    ٻاهريون->glyphAssembly.serialize_subset (c, glyphAssembly, هذا);

    إذا (!c->serializer->check_سونپيو (ٻاهريون->math_Glyph_Variant_رڪارڊ.len, math_Glyph_Variant_رڪارڊ.len, HB_SERIALIZE_ERROR_INT_حد_کان_مٿي))
      return_trace (خطأ);
    لكل (ثابت خودڪار& رڪارڊ : math_Glyph_Variant_رڪارڊ.iter ())
      إذا (!رڪارڊ.subset (c)) return_trace (خطأ);

    return_trace (سچ);
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
		  math_Glyph_Variant_رڪارڊ.sanitize (c));
  }

  ثابت MathGlyphAssembly &get_assembly () ثابت { أرجع هذا+glyphAssembly; }

  بنانشان صحيح get_variants (hb_direction_t direction,
			     hb_font_t *font,
			     بنانشان صحيح شروعات_وٿي,
			     بنانشان صحيح *variants_ڳڻپ, /* IN/OUT */
			     hb_ot_math_glyph_variant_t *variants /*ٻاهريون*/) ثابت
  {
    إذا (variants_ڳڻپ && variants)
    {
      int64_t mult = font->dir_mult (direction);
      لكل (خودڪار _ : hb_zip (math_Glyph_Variant_رڪارڊ.as_array ().sub_array (شروعات_وٿي, variants_ڳڻپ),
			    hb_array (variants, *variants_ڳڻپ)))
	_.second = {_.پهريون.variantGlyph, font->em_mult (_.پهريون.advanceMeasurement, mult)};
    }
    أرجع math_Glyph_Variant_رڪارڊ.len;
  }

  فراغ depend (hb_depend_معلومات_builder_t *depend_معلومات, بنانشان ذريعو) ثابت
  {
    (هذا+glyphAssembly).depend (depend_معلومات, ذريعو);

    لكل (ثابت خودڪار& _ : math_Glyph_Variant_رڪارڊ.iter ())
      _.depend (depend_معلومات, ذريعو);
  }

  فراغ گهيرو_glyphs (hb_سيٽ_t *variant_glyphs) ثابت
  {
    (هذا+glyphAssembly).گهيرو_glyphs (variant_glyphs);

    لكل (ثابت خودڪار& _ : math_Glyph_Variant_رڪارڊ.iter ())
      _.گهيرو_glyphs (variant_glyphs);
  }

  محمي:
  /* Offset to MathGlyphAssembly table for this shape - from the beginning of
     MathGlyphConstruction table.  May be NULL. */
  Offset16_منزل<MathGlyphAssembly>	  glyphAssembly;

  /* MathGlyphVariantRecords for alternative variants of the glyphs. */
  Array16_جو<Math_Glyph_Variant_رڪارڊ> math_Glyph_Variant_رڪارڊ;

  عام:
  DEFINE_سائيز_ARRAY (4, math_Glyph_Variant_رڪارڊ);
};

بنية MathVariants
{
  فراغ depend (hb_depend_معلومات_builder_t *depend_معلومات) ثابت
  {
    ثابت hb_array_t<ثابت Offset16_منزل<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vert_Glyph_ڳڻپ + horiz_Glyph_ڳڻپ);

    إذا (vertGlyphCoverage)
    {
      ثابت خودڪار vert_offsets = glyph_construction_offsets.sub_array (0, vert_Glyph_ڳڻپ);
      + hb_zip (هذا+vertGlyphCoverage, vert_offsets)
      | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, ثابت Offset16_منزل<MathGlyphConstruction>&> &_)
                  {
                    ثابت MathGlyphConstruction &mgc = هذا+_.second;
                    mgc.depend (depend_معلومات, _.پهريون);
                  })
      ;
    }
    إذا (horizGlyphCoverage)
    {
      ثابت خودڪار hori_offsets = glyph_construction_offsets.sub_array (vert_Glyph_ڳڻپ, horiz_Glyph_ڳڻپ);
      + hb_zip (هذا+horizGlyphCoverage, hori_offsets)
      | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, ثابت Offset16_منزل<MathGlyphConstruction>&> &_)
                  {
                    ثابت MathGlyphConstruction &mgc = هذا+_.second;
                    mgc.depend (depend_معلومات, _.پهريون);
                  })
      ;
    }
  }

  فراغ گهيرو_glyphs (ثابت hb_سيٽ_t *glyph_سيٽ,
                       hb_سيٽ_t *variant_glyphs) ثابت
  {
    ثابت hb_array_t<ثابت Offset16_منزل<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vert_Glyph_ڳڻپ + horiz_Glyph_ڳڻپ);

    إذا (vertGlyphCoverage)
    {
      ثابت خودڪار vert_offsets = glyph_construction_offsets.sub_array (0, vert_Glyph_ڳڻپ);
      + hb_zip (هذا+vertGlyphCoverage, vert_offsets)
      | hb_filter (glyph_سيٽ, hb_پهريون)
      | hb_map (hb_ٻيو)
      | hb_map (hb_جوڙيو (هذا))
      | hb_apply ([=] (ثابت MathGlyphConstruction &_) { _.گهيرو_glyphs (variant_glyphs); })
      ;
    }

    إذا (horizGlyphCoverage)
    {
      ثابت خودڪار hori_offsets = glyph_construction_offsets.sub_array (vert_Glyph_ڳڻپ, horiz_Glyph_ڳڻپ);
      + hb_zip (هذا+horizGlyphCoverage, hori_offsets)
      | hb_filter (glyph_سيٽ, hb_پهريون)
      | hb_map (hb_ٻيو)
      | hb_map (hb_جوڙيو (هذا))
      | hb_apply ([=] (ثابت MathGlyphConstruction &_) { _.گهيرو_glyphs (variant_glyphs); })
      ;
    }
  }

  فراغ collect_coverage_and_indices (hb_sorted_ويڪٽر_t<hb_codepoint_t>& new_coverage,
                                     ثابت Offset16_منزل<Coverage>& coverage,
                                     بنانشان i,
                                     بنانشان پڇاڙي_اشاريو,
                                     hb_سيٽ_t& indices,
                                     ثابت hb_سيٽ_t& glyphset,
                                     ثابت hb_map_t& glyph_map) ثابت
  {
    إذا (!coverage) أرجع;

    لكل (ثابت خودڪار _ : (هذا+coverage).iter ())
    {
      إذا (i >= پڇاڙي_اشاريو) أرجع;
      إذا (glyphset.has (_))
      {
        بنانشان new_gid = glyph_map.get (_);
        new_coverage.push (new_gid);
        indices.جوڙيو (i);
      }
      i++;
    }
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سيٽ_t &glyphset = c->plan->_glyphset_cmaped;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);
    إذا (!c->serializer->check_سونپيو (ٻاهريون->minConnectorOverlap, minConnectorOverlap, HB_SERIALIZE_ERROR_INT_حد_کان_مٿي))
      return_trace (خطأ);

    hb_sorted_ويڪٽر_t<hb_codepoint_t> new_vert_coverage;
    hb_sorted_ويڪٽر_t<hb_codepoint_t> new_hori_coverage;
    hb_سيٽ_t indices;
    collect_coverage_and_indices (new_vert_coverage, vertGlyphCoverage, 0, vert_Glyph_ڳڻپ, indices, glyphset, glyph_map);
    collect_coverage_and_indices (new_hori_coverage, horizGlyphCoverage, vert_Glyph_ڳڻپ, vert_Glyph_ڳڻپ + horiz_Glyph_ڳڻپ, indices, glyphset, glyph_map);

    إذا (!c->serializer->check_سونپيو (ٻاهريون->vert_Glyph_ڳڻپ, new_vert_coverage.ڊيگهائي, HB_SERIALIZE_ERROR_INT_حد_کان_مٿي))
      return_trace (خطأ);
    إذا (!c->serializer->check_سونپيو (ٻاهريون->horiz_Glyph_ڳڻپ, new_hori_coverage.ڊيگهائي, HB_SERIALIZE_ERROR_INT_حد_کان_مٿي))
      return_trace (خطأ);

    لكل (بنانشان i : indices.iter ())
    {
      خودڪار *o = c->serializer->embed (glyphConstruction[i]);
      إذا (!o) return_trace (خطأ);
      o->serialize_subset (c, glyphConstruction[i], هذا);
    }

    إذا (new_vert_coverage)
      ٻاهريون->vertGlyphCoverage.serialize_serialize (c->serializer, new_vert_coverage.iter ());

    إذا (new_hori_coverage)
    ٻاهريون->horizGlyphCoverage.serialize_serialize (c->serializer, new_hori_coverage.iter ());
    return_trace (سچ);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت خودڪار &داخلا : hb_zip (هذا+vertGlyphCoverage, glyphConstruction.as_array (vert_Glyph_ڳڻپ)))
      إذا (c->glyph_سيٽ->has (hb_پهريون (داخلا)))
        (هذا+hb_ٻيو (داخلا)).collect_variation_indices (c);
    لكل (ثابت خودڪار &داخلا : hb_zip (هذا+horizGlyphCoverage, glyphConstruction.as_array (vert_Glyph_ڳڻپ + horiz_Glyph_ڳڻپ).sub_array (vert_Glyph_ڳڻپ)))
      إذا (c->glyph_سيٽ->has (hb_پهريون (داخلا)))
        (هذا+hb_ٻيو (داخلا)).collect_variation_indices (c);
  }

  منطقي sanitize_offsets (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    بنانشان صحيح ڳڻپ = vert_Glyph_ڳڻپ + horiz_Glyph_ڳڻپ;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
      إذا (!glyphConstruction.arrayZ[i].sanitize (c, هذا)) return_trace (خطأ);
    return_trace (سچ);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  vertGlyphCoverage.sanitize (c, هذا) &&
		  horizGlyphCoverage.sanitize (c, هذا) &&
		  hb_barrier () &&
		  c->check_array (glyphConstruction.arrayZ, vert_Glyph_ڳڻپ + horiz_Glyph_ڳڻپ) &&
		  sanitize_offsets (c));
  }

  hb_position_t get_min_connector_overlap (hb_direction_t direction,
						  hb_font_t *font) ثابت
  { أرجع font->em_scale_dir (minConnectorOverlap, direction); }

  بنانشان صحيح get_glyph_variants (hb_codepoint_t glyph,
				   hb_direction_t direction,
				   hb_font_t *font,
				   بنانشان صحيح شروعات_وٿي,
				   بنانشان صحيح *variants_ڳڻپ, /* IN/OUT */
				   hb_ot_math_glyph_variant_t *variants /*ٻاهريون*/) ثابت
  { أرجع get_glyph_construction (glyph, direction, font)
	   .get_variants (direction, font, شروعات_وٿي, variants_ڳڻپ, variants); }

  بنانشان صحيح get_glyph_parts (hb_codepoint_t glyph,
				hb_direction_t direction,
				hb_font_t *font,
				بنانشان صحيح شروعات_وٿي,
				بنانشان صحيح *parts_ڳڻپ, /* IN/OUT */
				hb_ot_math_glyph_part_t *parts /*ٻاهريون*/,
				hb_position_t *italics_correction /*ٻاهريون*/) ثابت
  { أرجع get_glyph_construction (glyph, direction, font)
	   .get_assembly ()
	   .get_parts (direction, font,
		       شروعات_وٿي, parts_ڳڻپ, parts,
		       italics_correction); }

  خاص:
  ثابت MathGlyphConstruction &
  get_glyph_construction (hb_codepoint_t glyph,
			  hb_direction_t direction,
			  hb_font_t *font HB_اڻ_استعمال_ٿيل) ثابت
  {
    منطقي vertical = HB_DIRECTION_IS_VERTICAL (direction);
    بنانشان صحيح ڳڻپ = vertical ? vert_Glyph_ڳڻپ : horiz_Glyph_ڳڻپ;
    ثابت Offset16_منزل<Coverage> &coverage = vertical ? vertGlyphCoverage
						  : horizGlyphCoverage;

    بنانشان صحيح اشاريو_2 = (هذا+coverage).get_coverage (glyph);
    إذا (unlikely (اشاريو_2 >= ڳڻپ)) أرجع Null (MathGlyphConstruction);

    إذا (!vertical)
      اشاريو_2 += vert_Glyph_ڳڻپ;

    أرجع هذا+glyphConstruction[اشاريو_2];
  }

  محمي:
  HBUINT16	minConnectorOverlap;
				/* Minimum overlap of connecting
				 * glyphs during glyph construction,
				 * in design units. */
  Offset16_منزل<Coverage> vertGlyphCoverage;
				/* Offset to Coverage table -
				 * from the beginning of MathVariants
				 * table. */
  Offset16_منزل<Coverage> horizGlyphCoverage;
				/* Offset to Coverage table -
				 * from the beginning of MathVariants
				 * table. */
  HBUINT16	vert_Glyph_ڳڻپ;	/* Number of glyphs for which
				 * information is provided for
				 * vertically growing variants. */
  HBUINT16	horiz_Glyph_ڳڻپ;/* Number of glyphs for which
				 * information is provided for
				 * horizontally growing variants. */

  /* Array of offsets to MathGlyphConstruction tables - from the beginning of
     the MathVariants table, for shapes growing in vertical/horizontal
     direction. */
  Unsized_Array_جو<Offset16_منزل<MathGlyphConstruction>>
			glyphConstruction;

  عام:
  DEFINE_سائيز_ARRAY (10, glyphConstruction);
};


/*
 * MATH -- Mathematical typesetting
 * https://docs.microsoft.com/en-us/typography/opentype/spec/math
 */

بنية MATH
{
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_MATH;

  منطقي has_معلومات () ثابت { أرجع نسخو.منزل_int (); }

  فراغ depend (hb_depend_معلومات_builder_t *depend_معلومات) ثابت
  {
    إذا (mathVariants)
      (هذا+mathVariants).depend (depend_معلومات);
  }

  فراغ گهيرو_glyphs (hb_سيٽ_t *glyph_سيٽ) ثابت
  {
    إذا (mathVariants)
    {
      hb_سيٽ_t variant_glyphs;
      (هذا+mathVariants).گهيرو_glyphs (glyph_سيٽ, &variant_glyphs);
      hb_سيٽ_union (glyph_سيٽ, &variant_glyphs);
    }
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->embed (*هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    ٻاهريون->mathConstants.serialize_copy (c->serializer, mathConstants, هذا, 0, hb_serialize_context_t::سر, &c->plan->layout_variation_idx_delta_map);
    ٻاهريون->mathGlyphInfo.serialize_subset (c, mathGlyphInfo, هذا);
    ٻاهريون->mathVariants.serialize_subset (c, mathVariants, هذا);
    return_trace (سچ);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c, ثابت hb_سيٽ_t *variant_glyph_سيٽ) ثابت
  {
    (هذا+mathConstants).collect_variation_indices (c);
    (هذا+mathGlyphInfo).collect_variation_indices (c);
    خودڪار *glyph_سيٽ = c->glyph_سيٽ;
    c->glyph_سيٽ = variant_glyph_سيٽ;
    (هذا+mathVariants).collect_variation_indices (c);
    c->glyph_سيٽ = glyph_سيٽ;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (نسخو.sanitize (c) &&
		  likely (نسخو.major == 1) &&
		  hb_barrier () &&
		  mathConstants.sanitize (c, هذا) &&
		  mathGlyphInfo.sanitize (c, هذا) &&
		  mathVariants.sanitize (c, هذا));
  }

  // https://github.com/harfbuzz/harfbuzz/issues/4653
  HB_اندروني منطقي is_bad_cambria (hb_font_t *font) ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_MATH
    بدل HB_CODEPOINT_ENCODE3 (font->face->جدول.MATH.get_blob ()->ڊيگهائي,
                                 (هذا+mathConstants).min_اوچائي[1], // displayOperatorMinHeight
                                 (هذا+mathConstants).min_اوچائي[0]) // delimitedSubFormulaMinHeight
    {
      /* sha1sum:ab4a4fe054d23061f3c039493d6f665cfda2ecf5  cambria.ttc
       * sha1sum:086855301bff644f9d8827b88491fcf73a6d4cb9  cambria.ttc
       * sha1sum:b1e5a3feaca2ea3dfcf79ccb377de749ecf60343  cambria.ttc */
      حالة HB_CODEPOINT_ENCODE3 (25722, 2500, 3000):
        أرجع سچ;
    }
#جيڪڏهن_پورو
    أرجع خطأ;
  }

  hb_position_t get_constant (hb_ot_math_constant_t  constant,
			      hb_font_t		   *font) ثابت
  { أرجع (هذا+mathConstants).get_قدر (constant, font); }

  ثابت MathGlyphInfo &get_glyph_info () ثابت { أرجع هذا+mathGlyphInfo; }

  ثابت MathVariants &get_variants () ثابت    { أرجع هذا+mathVariants; }

  محمي:
  Fixed_نسخو<>نسخو;	/* Version of the MATH table
				 * initially set to 0x00010000u */
  Offset16_منزل<MathConstants>
		mathConstants;	/* MathConstants table */
  Offset16_منزل<MathGlyphInfo>
		mathGlyphInfo;	/* MathGlyphInfo table */
  Offset16_منزل<MathVariants>
		mathVariants;	/* MathVariants table */

  عام:
  DEFINE_سائيز_STATIC (10);
};

} /* namespace OT */


#جيڪڏهن_پورو /* HB_OT_MATH_TABLE_HH */

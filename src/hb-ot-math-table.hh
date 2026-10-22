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

#اگر_تعریف_نشده HB_OT_MATH_جدول_HH
#تعریف_کن HB_OT_MATH_جدول_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-layout-common.hh"
#ضمن "hb-ot-layout-gdef-table.hh"
#ضمن "hb-ot-math.h"
#ضمن "hb-depend-data.hh"

نطاق OT {


بنية Math_مقدار_رکورد
{
  hb_position_t get_x_مقدار (hb_font_t *font, ثابت فراغ *پایه) ثابت
  { أرجع hb_saturate_جمع_کن (font->em_scale_x (value), (پایه+device_جدول).get_x_delta (font, get_var_store (font, پایه))); }
  hb_position_t get_y_مقدار (hb_font_t *font, ثابت فراغ *پایه) ثابت
  { أرجع hb_saturate_جمع_کن (font->em_scale_y (value), (پایه+device_جدول).get_y_delta (font, get_var_store (font, پایه))); }

  ثابت قلم_Variation_Store &get_var_store (hb_font_t *font, ثابت فراغ *پایه) ثابت
  {
#إذا !defined(HB_NO_VAR) && !defined(HB_NO_OT_LAYOUT)
    إذا ((پایه+device_جدول).is_variation_device ())
      أرجع font->face->جدول.GDEF->جدول->get_var_store ();
#پایان_اگر
    أرجع Null (قلم_Variation_Store);
  }

  Math_مقدار_رکورد* copy (hb_serialize_context_t *c, ثابت فراغ *پایه,
                         ثابت hb_hashmap_t<بیعلامت, hb_pair_t<بیعلامت, صحيح>> *variation_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار *بیرون = c->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (اشاره_تهی);
    hb_pair_t<بیعلامت, صحيح> *varidx_delta;
    إذا (variation_map->has ((پایه+device_جدول).get_variation_شاخص (), &varidx_delta) &&
        !c->check_تخصیص_بده (بیرون->value, (int64_t) value + hb_دومی (*varidx_delta),
                          HB_SERIALIZE_ERROR_INT_سرریز))
      return_trace (اشاره_تهی);
    بیرون->device_جدول.serialize_copy (c, device_جدول, پایه, 0, hb_serialize_context_t::سر, variation_map);

    return_trace (بیرون);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c, ثابت فراغ *پایه) ثابت
  { (پایه+device_جدول).collect_variation_indices (c); }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && device_جدول.sanitize (c, پایه));
  }

  محمي:
  HBINT16		value;		/* The X or Y value in design units */
  Offset16_مقصد<Device>	device_جدول;	/* Offset to the device table - from the
					 * beginning of parent table.  May be NULL.
					 * Suggested format for device table is 1. */

  عام:
  DEFINE_اندازه_STATIC (4);
};

بنية MathConstants
{
  دوست بنية MATH;

  MathConstants* copy (hb_serialize_context_t *c,
                      ثابت hb_hashmap_t<بیعلامت, hb_pair_t<بیعلامت, صحيح>> *variation_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار *بیرون = c->شروع_embed (هذا);

    HBINT16 *p = c->allocate_اندازه<HBINT16> (HBINT16::static_اندازه * 2);
    إذا (unlikely (!p)) return_trace (اشاره_تهی);
    hb_memcpy (p, percent_Scale_پایین, HBINT16::static_اندازه * 2);

    HBUINT16 *m = c->allocate_اندازه<HBUINT16> (HBUINT16::static_اندازه * 2);
    إذا (unlikely (!m)) return_trace (اشاره_تهی);
    hb_memcpy (m, min_ارتفاع, HBUINT16::static_اندازه * 2);

    بیعلامت شمار = ARRAY_درازی (math_مقدار_رکوردها);
    لكل (بیعلامت i = 0; i < شمار; i++)
      إذا (!c->copy (math_مقدار_رکوردها[i], هذا, variation_map))
        return_trace (اشاره_تهی);

    إذا (!c->embed (radicalDegreeBottomRaisePercent)) return_trace (اشاره_تهی);
    return_trace (بیرون);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت خودکار &رکورد : math_مقدار_رکوردها)
      رکورد.collect_variation_indices (c, هذا);
  }

  منطقي sanitize_math_مقدار_رکوردها (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);

    بیعلامت صحيح شمار = ARRAY_درازی (math_مقدار_رکوردها);
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
      إذا (!math_مقدار_رکوردها[i].sanitize (c, هذا))
	return_trace (خطأ);

    return_trace (درست);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && sanitize_math_مقدار_رکوردها (c));
  }

  hb_position_t get_مقدار (hb_ot_math_constant_t constant,
			   hb_font_t *font) ثابت
  {
    بدل (constant) {

    حالة HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_پایین:
    حالة HB_OT_MATH_CONSTANT_SCRIPT_SCRIPT_PERCENT_SCALE_پایین:
      أرجع percent_Scale_پایین[constant - HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_پایین];

    حالة HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_ارتفاع:
    حالة HB_OT_MATH_CONSTANT_DISPLAY_OPERATOR_MIN_ارتفاع:
      أرجع font->em_scale_y (min_ارتفاع[constant - HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_ارتفاع]);

    حالة HB_OT_MATH_CONSTANT_RADICAL_KERN_AFTER_DEGREE:
    حالة HB_OT_MATH_CONSTANT_RADICAL_KERN_BEFORE_DEGREE:
    حالة HB_OT_MATH_CONSTANT_SKEWED_FRACTION_HORIZONTAL_GAP:
    حالة HB_OT_MATH_CONSTANT_SPACE_AFTER_SCRIPT:
      أرجع math_مقدار_رکوردها[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_x_مقدار (font, هذا);

    حالة HB_OT_MATH_CONSTANT_ACCENT_پایه_ارتفاع:
    حالة HB_OT_MATH_CONSTANT_AXIS_ارتفاع:
    حالة HB_OT_MATH_CONSTANT_FLATTENED_ACCENT_پایه_ارتفاع:
    حالة HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_DISPLAY_STYLE_لغزش_پایین:
    حالة HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_لغزش_پایین:
    حالة HB_OT_MATH_CONSTANT_FRACTION_DENOM_DISPLAY_STYLE_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_DISPLAY_STYLE_لغزش_به_بالا:
    حالة HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_لغزش_به_بالا:
    حالة HB_OT_MATH_CONSTANT_FRACTION_رقم_DISPLAY_STYLE_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_FRACTION_قاعده_THICKNESS:
    حالة HB_OT_MATH_CONSTANT_پایینی_حد_BASELINE_DROP_MIN:
    حالة HB_OT_MATH_CONSTANT_پایینی_حد_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_MATH_LEADING:
    حالة HB_OT_MATH_CONSTANT_OVERBAR_EXTRA_ASCENDER:
    حالة HB_OT_MATH_CONSTANT_OVERBAR_قاعده_THICKNESS:
    حالة HB_OT_MATH_CONSTANT_OVERBAR_VERTICAL_GAP:
    حالة HB_OT_MATH_CONSTANT_RADICAL_DISPLAY_STYLE_VERTICAL_GAP:
    حالة HB_OT_MATH_CONSTANT_RADICAL_EXTRA_ASCENDER:
    حالة HB_OT_MATH_CONSTANT_RADICAL_قاعده_THICKNESS:
    حالة HB_OT_MATH_CONSTANT_RADICAL_VERTICAL_GAP:
    حالة HB_OT_MATH_CONSTANT_SKEWED_FRACTION_VERTICAL_GAP:
    حالة HB_OT_MATH_CONSTANT_پشته_BOTTOM_DISPLAY_STYLE_لغزش_پایین:
    حالة HB_OT_MATH_CONSTANT_پشته_BOTTOM_لغزش_پایین:
    حالة HB_OT_MATH_CONSTANT_پشته_DISPLAY_STYLE_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_پشته_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_پشته_بالاترین_DISPLAY_STYLE_لغزش_به_بالا:
    حالة HB_OT_MATH_CONSTANT_پشته_بالاترین_لغزش_به_بالا:
    حالة HB_OT_MATH_CONSTANT_STRETCH_پشته_BOTTOM_لغزش_پایین:
    حالة HB_OT_MATH_CONSTANT_STRETCH_پشته_GAP_ABOVE_MIN:
    حالة HB_OT_MATH_CONSTANT_STRETCH_پشته_GAP_زیر_MIN:
    حالة HB_OT_MATH_CONSTANT_STRETCH_پشته_بالاترین_لغزش_به_بالا:
    حالة HB_OT_MATH_CONSTANT_SUBSCRIPT_BASELINE_DROP_MIN:
    حالة HB_OT_MATH_CONSTANT_SUBSCRIPT_لغزش_پایین:
    حالة HB_OT_MATH_CONSTANT_SUBSCRIPT_بالاترین_MAX:
    حالة HB_OT_MATH_CONSTANT_SUB_SUPERSCRIPT_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_SUPERSCRIPT_BASELINE_DROP_MAX:
    حالة HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MAX_WITH_SUBSCRIPT:
    حالة HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MIN:
    حالة HB_OT_MATH_CONSTANT_SUPERSCRIPT_لغزش_به_بالا:
    حالة HB_OT_MATH_CONSTANT_SUPERSCRIPT_لغزش_به_بالا_CRAMPED:
    حالة HB_OT_MATH_CONSTANT_UNDERBAR_EXTRA_DESCENDER:
    حالة HB_OT_MATH_CONSTANT_UNDERBAR_قاعده_THICKNESS:
    حالة HB_OT_MATH_CONSTANT_UNDERBAR_VERTICAL_GAP:
    حالة HB_OT_MATH_CONSTANT_فوقانی_حد_BASELINE_RISE_MIN:
    حالة HB_OT_MATH_CONSTANT_فوقانی_حد_GAP_MIN:
      أرجع math_مقدار_رکوردها[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_y_مقدار (font, هذا);

    حالة HB_OT_MATH_CONSTANT_RADICAL_DEGREE_BOTTOM_RAISE_PERCENT:
      أرجع radicalDegreeBottomRaisePercent;

    افتراضي:
      أرجع 0;
    }
  }

  محمي:
  HBINT16 percent_Scale_پایین[2];
  HBUINT16 min_ارتفاع[2];
  Math_مقدار_رکورد math_مقدار_رکوردها[51];
  HBINT16 radicalDegreeBottomRaisePercent;

  عام:
  DEFINE_اندازه_STATIC (214);
};

بنية MathItalicsCorrectionInfo
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_مجموعه_t &glyphset = c->plan->_glyphset_mathed;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);

    hb_sorted_وکتور_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+coverage, italicsCorrection)
    | hb_filter (glyphset, hb_اولی)
    | hb_filter (serialize_math_رکورد_array (c->serializer, بیرون->italicsCorrection, هذا, &c->plan->layout_variation_idx_delta_map), hb_دومی)
    | hb_map (hb_اولی)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    بیرون->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (درست);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت خودکار &مدخل : hb_zip (هذا+coverage, italicsCorrection))
      إذا (c->glyph_مجموعه->has (hb_اولی (مدخل)))
        hb_دومی (مدخل).collect_variation_indices (c, هذا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  coverage.sanitize (c, هذا) &&
		  italicsCorrection.sanitize (c, هذا));
  }

  hb_position_t get_مقدار (hb_codepoint_t glyph,
			   hb_font_t *font) ثابت
  {
    بیعلامت صحيح شاخص_2 = (هذا+coverage).get_coverage (glyph);
    أرجع italicsCorrection[شاخص_2].get_x_مقدار (font, هذا);
  }

  محمي:
  Offset16_مقصد<Coverage>       coverage;		/* Offset to Coverage table -
						 * from the beginning of
						 * MathItalicsCorrectionInfo
						 * table. */
  Array16_از<Math_مقدار_رکورد> italicsCorrection;	/* Array of MathValueRecords
						 * defining italics correction
						 * values for each
						 * covered glyph. */

  عام:
  DEFINE_اندازه_ARRAY (4, italicsCorrection);
};

بنية Math_بالاترین_Accent_Attachment
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_مجموعه_t &glyphset = c->plan->_glyphset_mathed;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);

    hb_sorted_وکتور_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+بالاترین_Accent_Coverage, بالاترین_Accent_Attachment)
    | hb_filter (glyphset, hb_اولی)
    | hb_filter (serialize_math_رکورد_array (c->serializer, بیرون->بالاترین_Accent_Attachment, هذا, &c->plan->layout_variation_idx_delta_map), hb_دومی)
    | hb_map (hb_اولی)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    بیرون->بالاترین_Accent_Coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (درست);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت خودکار &مدخل : hb_zip (هذا+بالاترین_Accent_Coverage, بالاترین_Accent_Attachment))
      إذا (c->glyph_مجموعه->has (hb_اولی (مدخل)))
        hb_دومی (مدخل).collect_variation_indices (c, هذا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  بالاترین_Accent_Coverage.sanitize (c, هذا) &&
		  بالاترین_Accent_Attachment.sanitize (c, هذا));
  }

  hb_position_t get_مقدار (hb_codepoint_t glyph,
			   hb_font_t *font) ثابت
  {
    بیعلامت صحيح شاخص_2 = (هذا+بالاترین_Accent_Coverage).get_coverage (glyph);
    إذا (شاخص_2 == NOT_COVERED)
      أرجع font->get_glyph_h_advance (glyph) / 2;
    أرجع بالاترین_Accent_Attachment[شاخص_2].get_x_مقدار (font, هذا);
  }

  محمي:
  Offset16_مقصد<Coverage>       بالاترین_Accent_Coverage;   /* Offset to Coverage table -
						 * from the beginning of
						 * MathTopAccentAttachment
						 * table. */
  Array16_از<Math_مقدار_رکورد> بالاترین_Accent_Attachment; /* Array of MathValueRecords
						 * defining top accent
						 * attachment points for each
						 * covered glyph. */

  عام:
  DEFINE_اندازه_ARRAY (2 + 2, بالاترین_Accent_Attachment);
};

بنية MathKern
{
  MathKern* copy (hb_serialize_context_t *c,
                      ثابت hb_hashmap_t<بیعلامت, hb_pair_t<بیعلامت, صحيح>> *variation_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار *بیرون = c->شروع_embed (هذا);

    إذا (unlikely (!c->embed (ارتفاع_شمار))) return_trace (اشاره_تهی);

    بیعلامت شمار = 2 * ارتفاع_شمار + 1;
    لكل (بیعلامت i = 0; i < شمار; i++)
      إذا (!c->copy (math_مقدار_رکوردها_Z.arrayZ[i], هذا, variation_map))
        return_trace (اشاره_تهی);

    return_trace (بیرون);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت خودکار &رکورد : math_مقدار_رکوردها_Z.as_array (2 * ارتفاع_شمار + 1))
      رکورد.collect_variation_indices (c, هذا);
  }

  منطقي sanitize_math_مقدار_رکوردها (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    بیعلامت صحيح شمار = 2 * ارتفاع_شمار + 1;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
      إذا (!math_مقدار_رکوردها_Z.arrayZ[i].sanitize (c, هذا)) return_trace (خطأ);
    return_trace (درست);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  c->check_array (math_مقدار_رکوردها_Z.arrayZ, 2 * ارتفاع_شمار + 1) &&
		  sanitize_math_مقدار_رکوردها (c));
  }

  hb_position_t get_مقدار (hb_position_t correction_ارتفاع, hb_font_t *font) ثابت
  {
    ثابت Math_مقدار_رکورد* correction_ارتفاع_2 = math_مقدار_رکوردها_Z.arrayZ;
    ثابت Math_مقدار_رکورد* kern_مقدار = math_مقدار_رکوردها_Z.arrayZ + ارتفاع_شمار;
    صحيح sign = font->y_scale < 0 ? -1 : +1;

    /* According to OpenType spec (v1.9), except for the boundary cases, the index
     * chosen for kern value should be i such that
     *    correctionHeight[i-1] <= correction_height < correctionHeight[i]
     * We can use the binary search algorithm of std::upper_bound(). Or, we can
     * use the internal hb_bsearch_impl.
     */
    بیعلامت صحيح pos;
    خودکار cmp = +[](ثابت فراغ* key, ثابت فراغ* p,
                   صحيح sign, hb_font_t* font, ثابت MathKern* mathKern) -> صحيح {
      hb_position_t a = *(hb_position_t *) key;
      hb_position_t b = ((Math_مقدار_رکورد *) p)->get_y_مقدار (font, mathKern);
      إذا (a == b) أرجع 0;
      أرجع sign > 0 ? (a < b ? -1 : +1) : (b < a ? -1 : +1);
    };
    بیعلامت صحيح i = hb_bsearch_impl(&pos, correction_ارتفاع, correction_ارتفاع_2,
                                     ارتفاع_شمار, Math_مقدار_رکورد::static_اندازه,
                                     cmp, sign, font, هذا) ? pos + 1 : pos;
    أرجع kern_مقدار[i].get_x_مقدار (font, هذا);
  }

  بیعلامت صحيح get_entries (بیعلامت صحيح شروع_جابه_جایی,
			    بیعلامت صحيح *entries_شمار, /* IN/OUT */
			    hb_ot_math_kern_مدخل_t *kern_entries, /*بیرون*/
			    hb_font_t *font) ثابت
  {
    ثابت Math_مقدار_رکورد* correction_ارتفاع_2 = math_مقدار_رکوردها_Z.arrayZ;
    ثابت Math_مقدار_رکورد* kern_مقدار = math_مقدار_رکوردها_Z.arrayZ + ارتفاع_شمار;
    ثابت بیعلامت صحيح entries_شمار_2 = ارتفاع_شمار + 1;

    إذا (entries_شمار && kern_entries)
    {
      بیعلامت صحيح شروع = hb_min (شروع_جابه_جایی, entries_شمار_2);
      بیعلامت صحيح انتها = hb_min (شروع + *entries_شمار, entries_شمار_2);
      *entries_شمار = انتها - شروع;

      لكل (بیعلامت صحيح i = 0; i < *entries_شمار; i++) {
	بیعلامت صحيح j = شروع + i;

	hb_position_t max_ارتفاع;
	إذا (j == ارتفاع_شمار) {
	  max_ارتفاع = INT32_MAX;
	} وإلا {
	  max_ارتفاع = correction_ارتفاع_2[j].get_y_مقدار (font, هذا);
	}

	kern_entries[i] = {max_ارتفاع, kern_مقدار[j].get_x_مقدار (font, هذا)};
      }
    }
    أرجع entries_شمار_2;
  }

  محمي:
  HBUINT16	ارتفاع_شمار;
  Unsized_Array_از<Math_مقدار_رکورد>
		math_مقدار_رکوردها_Z;
				/* Array of correction heights at
				 * which the kern value changes.
				 * Sorted by the height value in
				 * design units (heightCount entries),
				 * Followed by:
				 * Array of kern values corresponding
				 * to heights. (heightCount+1 entries).
				 */

  عام:
  DEFINE_اندازه_ARRAY (2, math_مقدار_رکوردها_Z);
};

بنية Math_Kern_Info_رکورد
{
  Math_Kern_Info_رکورد* copy (hb_serialize_context_t *c, ثابت فراغ *پایه,
                            ثابت hb_hashmap_t<بیعلامت, hb_pair_t<بیعلامت, صحيح>> *variation_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار *بیرون = c->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (اشاره_تهی);

    بیعلامت شمار = ARRAY_درازی (mathKern);
    لكل (بیعلامت i = 0; i < شمار; i++)
      بیرون->mathKern[i].serialize_copy (c, mathKern[i], پایه, 0, hb_serialize_context_t::سر, variation_map);

    return_trace (بیرون);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c, ثابت فراغ *پایه) ثابت
  {
    لكل (ثابت خودکار &جابه_جایی : mathKern)
      (پایه+جابه_جایی).collect_variation_indices (c);
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *پایه) ثابت
  {
    TRACE_SANITIZE (هذا);

    بیعلامت صحيح شمار = ARRAY_درازی (mathKern);
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
      إذا (unlikely (!mathKern[i].sanitize (c, پایه)))
	return_trace (خطأ);

    return_trace (درست);
  }

  hb_position_t get_kerning (hb_ot_math_kern_t kern,
			     hb_position_t correction_ارتفاع,
			     hb_font_t *font,
			     ثابت فراغ *پایه) ثابت
  {
    بیعلامت صحيح idx = kern;
    إذا (unlikely (idx >= ARRAY_درازی (mathKern))) أرجع 0;
    أرجع (پایه+mathKern[idx]).get_مقدار (correction_ارتفاع, font);
  }

  بیعلامت صحيح get_kernings (hb_ot_math_kern_t kern,
			     بیعلامت صحيح شروع_جابه_جایی,
			     بیعلامت صحيح *entries_شمار, /* IN/OUT */
			     hb_ot_math_kern_مدخل_t *kern_entries, /*بیرون*/
			     hb_font_t *font,
			     ثابت فراغ *پایه) ثابت
  {
    بیعلامت صحيح idx = kern;
    إذا (unlikely (idx >= ARRAY_درازی (mathKern)) || !mathKern[idx]) {
      إذا (entries_شمار) *entries_شمار = 0;
      أرجع 0;
    }
    أرجع (پایه+mathKern[idx]).get_entries (شروع_جابه_جایی,
					     entries_شمار,
					     kern_entries,
					     font);
  }

  محمي:
  /* Offset to MathKern table for each corner -
   * from the beginning of MathKernInfo table.  May be NULL. */
  Offset16_مقصد<MathKern> mathKern[4];

  عام:
  DEFINE_اندازه_STATIC (8);
};

بنية MathKernInfo
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_مجموعه_t &glyphset = c->plan->_glyphset_mathed;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);

    hb_sorted_وکتور_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+mathKernCoverage, math_Kern_Info_رکوردها)
    | hb_filter (glyphset, hb_اولی)
    | hb_filter (serialize_math_رکورد_array (c->serializer, بیرون->math_Kern_Info_رکوردها, هذا, &c->plan->layout_variation_idx_delta_map), hb_دومی)
    | hb_map (hb_اولی)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    بیرون->mathKernCoverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (درست);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت خودکار &مدخل : hb_zip (هذا+mathKernCoverage, math_Kern_Info_رکوردها))
      إذا (c->glyph_مجموعه->has (hb_اولی (مدخل)))
        hb_دومی (مدخل).collect_variation_indices (c, هذا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  mathKernCoverage.sanitize (c, هذا) &&
		  math_Kern_Info_رکوردها.sanitize (c, هذا));
  }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_ارتفاع,
			     hb_font_t *font) ثابت
  {
    بیعلامت صحيح شاخص_2 = (هذا+mathKernCoverage).get_coverage (glyph);
    أرجع math_Kern_Info_رکوردها[شاخص_2].get_kerning (kern, correction_ارتفاع, font, هذا);
  }

  بیعلامت صحيح get_kernings (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     بیعلامت صحيح شروع_جابه_جایی,
			     بیعلامت صحيح *entries_شمار, /* IN/OUT */
			     hb_ot_math_kern_مدخل_t *kern_entries, /*بیرون*/
			     hb_font_t *font) ثابت
  {
    بیعلامت صحيح شاخص_2 = (هذا+mathKernCoverage).get_coverage (glyph);
    أرجع math_Kern_Info_رکوردها[شاخص_2].get_kernings (kern,
						    شروع_جابه_جایی,
						    entries_شمار,
						    kern_entries,
						    font,
						    هذا);
  }

  محمي:
  Offset16_مقصد<Coverage>
		mathKernCoverage;
				/* Offset to Coverage table -
				 * from the beginning of the
				 * MathKernInfo table. */
  Array16_از<Math_Kern_Info_رکورد>
		math_Kern_Info_رکوردها;
				/* Array of MathKernInfoRecords,
				 * per-glyph information for
				 * mathematical positioning
				 * of subscripts and
				 * superscripts. */

  عام:
  DEFINE_اندازه_ARRAY (4, math_Kern_Info_رکوردها);
};

بنية MathGlyphInfo
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->embed (*هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    بیرون->mathItalicsCorrectionInfo.serialize_subset (c, mathItalicsCorrectionInfo, هذا);
    بیرون->math_بالاترین_Accent_Attachment.serialize_subset (c, math_بالاترین_Accent_Attachment, هذا);

    ثابت hb_مجموعه_t &glyphset = c->plan->_glyphset_mathed;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار it =
    + hb_iter (هذا+extendedShapeCoverage)
    | hb_take (c->plan->منبع->get_رقم_glyphs ())
    | hb_filter (glyphset)
    | hb_map_retains_ترتیبدهی (glyph_map)
    ;

    إذا (it) بیرون->extendedShapeCoverage.serialize_serialize (c->serializer, it);
    وإلا بیرون->extendedShapeCoverage = 0;

    بیرون->mathKernInfo.serialize_subset (c, mathKernInfo, هذا);
    return_trace (درست);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    (هذا+mathItalicsCorrectionInfo).collect_variation_indices (c);
    (هذا+math_بالاترین_Accent_Attachment).collect_variation_indices (c);
    (هذا+mathKernInfo).collect_variation_indices (c);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  mathItalicsCorrectionInfo.sanitize (c, هذا) &&
		  math_بالاترین_Accent_Attachment.sanitize (c, هذا) &&
		  extendedShapeCoverage.sanitize (c, هذا) &&
		  mathKernInfo.sanitize (c, هذا));
  }

  hb_position_t
  get_italics_correction (hb_codepoint_t  glyph, hb_font_t *font) ثابت
  { أرجع (هذا+mathItalicsCorrectionInfo).get_مقدار (glyph, font); }

  hb_position_t
  get_بالاترین_accent_attachment (hb_codepoint_t  glyph, hb_font_t *font) ثابت
  { أرجع (هذا+math_بالاترین_Accent_Attachment).get_مقدار (glyph, font); }

  منطقي is_extended_shape (hb_codepoint_t glyph) ثابت
  { أرجع (هذا+extendedShapeCoverage).get_coverage (glyph) != NOT_COVERED; }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_ارتفاع,
			     hb_font_t *font) ثابت
  { أرجع (هذا+mathKernInfo).get_kerning (glyph, kern, correction_ارتفاع, font); }

  hb_position_t get_kernings (hb_codepoint_t glyph,
			      hb_ot_math_kern_t kern,
			      بیعلامت صحيح شروع_جابه_جایی,
			      بیعلامت صحيح *entries_شمار, /* IN/OUT */
			      hb_ot_math_kern_مدخل_t *kern_entries, /*بیرون*/
			      hb_font_t *font) ثابت
  { أرجع (هذا+mathKernInfo).get_kernings (glyph,
					     kern,
					     شروع_جابه_جایی,
					     entries_شمار,
					     kern_entries,
					     font); }

  محمي:
  /* Offset to MathItalicsCorrectionInfo table -
   * from the beginning of MathGlyphInfo table. */
  Offset16_مقصد<MathItalicsCorrectionInfo> mathItalicsCorrectionInfo;

  /* Offset to MathTopAccentAttachment table -
   * from the beginning of MathGlyphInfo table. */
  Offset16_مقصد<Math_بالاترین_Accent_Attachment> math_بالاترین_Accent_Attachment;

  /* Offset to coverage table for Extended Shape glyphs -
   * from the beginning of MathGlyphInfo table. When the left or right glyph of
   * a box is an extended shape variant, the (ink) box (and not the default
   * position defined by values in MathConstants table) should be used for
   * vertical positioning purposes.  May be NULL.. */
  Offset16_مقصد<Coverage> extendedShapeCoverage;

   /* Offset to MathKernInfo table -
    * from the beginning of MathGlyphInfo table. */
  Offset16_مقصد<MathKernInfo> mathKernInfo;

  عام:
  DEFINE_اندازه_STATIC (8);
};

بنية Math_Glyph_Variant_رکورد
{
  دوست بنية MathGlyphConstruction;

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    ثابت hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_تخصیص_بده (بیرون->variantGlyph, glyph_map.get (variantGlyph), HB_SERIALIZE_ERROR_INT_سرریز));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  فراغ depend (hb_depend_داده_builder_t *depend_داده, بیعلامت منبع) ثابت
  { depend_داده->جمع_کن_depend(منبع, HB_OT_برچسب_MATH, variantGlyph); }

  فراغ بستگی_glyphs (hb_مجموعه_t *variant_glyphs) ثابت
  { variant_glyphs->جمع_کن (variantGlyph); }

  محمي:
  HBGlyphID16 variantGlyph;       /* Glyph ID for the variant. */
  HBUINT16  advanceMeasurement; /* Advance width/height, in design units, of the
				 * variant, in the direction of requested
				 * glyph extension. */

  عام:
  DEFINE_اندازه_STATIC (4);
};

بنية PartFlags : HBUINT16
{
  تعداد Flags {
    Extender	= 0x0001u, /* If set, the part can be skipped or repeated. */

    Defined	= 0x0001u, /* All defined flags. */
  };

  عام:
  DEFINE_اندازه_STATIC (2);
};

بنية Math_Glyph_Part_رکورد
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    ثابت hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_تخصیص_بده (بیرون->glyph, glyph_map.get (glyph), HB_SERIALIZE_ERROR_INT_سرریز));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  فراغ extract (hb_ot_math_glyph_part_t &بیرون,
		int64_t mult,
		hb_font_t *font) ثابت
  {
    بیرون.glyph			= glyph;

    بیرون.شروع_connector_درازی	= font->em_mult (شروع_Connector_درازی, mult);
    بیرون.انتها_connector_درازی	= font->em_mult (انتها_Connector_درازی, mult);
    بیرون.پر_advance		= font->em_mult (پر_Advance, mult);

    static_assert ((بیعلامت صحيح) HB_OT_MATH_GLYPH_PART_FLAG_EXTENDER ==
		   (بیعلامت صحيح) PartFlags::Extender, "");

    بیرون.flags = (hb_ot_math_glyph_part_flags_t)
		(بیعلامت صحيح)
		(partFlags & PartFlags::Defined);
  }

  فراغ depend (hb_depend_داده_builder_t *depend_داده, بیعلامت منبع) ثابت
  { depend_داده->جمع_کن_depend(منبع, HB_OT_برچسب_MATH, glyph); }

  فراغ بستگی_glyphs (hb_مجموعه_t *variant_glyphs) ثابت
  { variant_glyphs->جمع_کن (glyph); }

  محمي:
  HBGlyphID16	glyph;		/* Glyph ID for the part. */
  HBUINT16	شروع_Connector_درازی;
				/* Advance width/ height of the straight bar
				 * connector material, in design units, is at
				 * the beginning of the glyph, in the
				 * direction of the extension. */
  HBUINT16	انتها_Connector_درازی;
				/* Advance width/ height of the straight bar
				 * connector material, in design units, is at
				 * the end of the glyph, in the direction of
				 * the extension. */
  HBUINT16	پر_Advance;	/* Full advance width/height for this part,
				 * in the direction of the extension.
				 * In design units. */
  PartFlags	partFlags;	/* Part qualifiers. */

  عام:
  DEFINE_اندازه_STATIC (10);
};

بنية MathGlyphAssembly
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);

    إذا (!c->serializer->copy (italicsCorrection, هذا, &c->plan->layout_variation_idx_delta_map)) return_trace (خطأ);
    إذا (!c->serializer->copy<HBUINT16> (part_رکوردها.len)) return_trace (خطأ);

    لكل (ثابت خودکار& رکورد : part_رکوردها.iter ())
      إذا (!رکورد.subset (c)) return_trace (خطأ);
    return_trace (درست);
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
		  part_رکوردها.sanitize (c));
  }

  بیعلامت صحيح get_parts (hb_direction_t direction,
			  hb_font_t *font,
			  بیعلامت صحيح شروع_جابه_جایی,
			  بیعلامت صحيح *parts_شمار, /* IN/OUT */
			  hb_ot_math_glyph_part_t *parts /*بیرون*/,
			  hb_position_t *italics_correction /*بیرون*/) ثابت
  {
    إذا (parts_شمار && parts)
    {
      int64_t mult = font->dir_mult (direction);
      لكل (خودکار _ : hb_zip (part_رکوردها.as_array ().sub_array (شروع_جابه_جایی, parts_شمار),
			    hb_array (parts, *parts_شمار)))
	_.اولی.extract (_.second, mult, font);
    }

    إذا (italics_correction)
      *italics_correction = italicsCorrection.get_x_مقدار (font, هذا);

    أرجع part_رکوردها.len;
  }

  فراغ depend (hb_depend_داده_builder_t *depend_داده, بیعلامت منبع) ثابت
  {
    لكل (ثابت خودکار& _ : part_رکوردها.iter ())
      _.depend (depend_داده, منبع);
  }

  فراغ بستگی_glyphs (hb_مجموعه_t *variant_glyphs) ثابت
  {
    لكل (ثابت خودکار& _ : part_رکوردها.iter ())
      _.بستگی_glyphs (variant_glyphs);
  }

  محمي:
  Math_مقدار_رکورد
		italicsCorrection;
				/* Italics correction of this
				 * MathGlyphAssembly. Should not
				 * depend on the assembly size. */
  Array16_از<Math_Glyph_Part_رکورد>
		part_رکوردها;	/* Array of part records, from
				 * left to right and bottom to
				 * top. */

  عام:
  DEFINE_اندازه_ARRAY (6, part_رکوردها);
};

بنية MathGlyphConstruction
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);

    بیرون->glyphAssembly.serialize_subset (c, glyphAssembly, هذا);

    إذا (!c->serializer->check_تخصیص_بده (بیرون->math_Glyph_Variant_رکورد.len, math_Glyph_Variant_رکورد.len, HB_SERIALIZE_ERROR_INT_سرریز))
      return_trace (خطأ);
    لكل (ثابت خودکار& رکورد : math_Glyph_Variant_رکورد.iter ())
      إذا (!رکورد.subset (c)) return_trace (خطأ);

    return_trace (درست);
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
		  math_Glyph_Variant_رکورد.sanitize (c));
  }

  ثابت MathGlyphAssembly &get_assembly () ثابت { أرجع هذا+glyphAssembly; }

  بیعلامت صحيح get_variants (hb_direction_t direction,
			     hb_font_t *font,
			     بیعلامت صحيح شروع_جابه_جایی,
			     بیعلامت صحيح *variants_شمار, /* IN/OUT */
			     hb_ot_math_glyph_variant_t *variants /*بیرون*/) ثابت
  {
    إذا (variants_شمار && variants)
    {
      int64_t mult = font->dir_mult (direction);
      لكل (خودکار _ : hb_zip (math_Glyph_Variant_رکورد.as_array ().sub_array (شروع_جابه_جایی, variants_شمار),
			    hb_array (variants, *variants_شمار)))
	_.second = {_.اولی.variantGlyph, font->em_mult (_.اولی.advanceMeasurement, mult)};
    }
    أرجع math_Glyph_Variant_رکورد.len;
  }

  فراغ depend (hb_depend_داده_builder_t *depend_داده, بیعلامت منبع) ثابت
  {
    (هذا+glyphAssembly).depend (depend_داده, منبع);

    لكل (ثابت خودکار& _ : math_Glyph_Variant_رکورد.iter ())
      _.depend (depend_داده, منبع);
  }

  فراغ بستگی_glyphs (hb_مجموعه_t *variant_glyphs) ثابت
  {
    (هذا+glyphAssembly).بستگی_glyphs (variant_glyphs);

    لكل (ثابت خودکار& _ : math_Glyph_Variant_رکورد.iter ())
      _.بستگی_glyphs (variant_glyphs);
  }

  محمي:
  /* Offset to MathGlyphAssembly table for this shape - from the beginning of
     MathGlyphConstruction table.  May be NULL. */
  Offset16_مقصد<MathGlyphAssembly>	  glyphAssembly;

  /* MathGlyphVariantRecords for alternative variants of the glyphs. */
  Array16_از<Math_Glyph_Variant_رکورد> math_Glyph_Variant_رکورد;

  عام:
  DEFINE_اندازه_ARRAY (4, math_Glyph_Variant_رکورد);
};

بنية MathVariants
{
  فراغ depend (hb_depend_داده_builder_t *depend_داده) ثابت
  {
    ثابت hb_array_t<ثابت Offset16_مقصد<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vert_Glyph_شمار + horiz_Glyph_شمار);

    إذا (vertGlyphCoverage)
    {
      ثابت خودکار vert_offsets = glyph_construction_offsets.sub_array (0, vert_Glyph_شمار);
      + hb_zip (هذا+vertGlyphCoverage, vert_offsets)
      | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, ثابت Offset16_مقصد<MathGlyphConstruction>&> &_)
                  {
                    ثابت MathGlyphConstruction &mgc = هذا+_.second;
                    mgc.depend (depend_داده, _.اولی);
                  })
      ;
    }
    إذا (horizGlyphCoverage)
    {
      ثابت خودکار hori_offsets = glyph_construction_offsets.sub_array (vert_Glyph_شمار, horiz_Glyph_شمار);
      + hb_zip (هذا+horizGlyphCoverage, hori_offsets)
      | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, ثابت Offset16_مقصد<MathGlyphConstruction>&> &_)
                  {
                    ثابت MathGlyphConstruction &mgc = هذا+_.second;
                    mgc.depend (depend_داده, _.اولی);
                  })
      ;
    }
  }

  فراغ بستگی_glyphs (ثابت hb_مجموعه_t *glyph_مجموعه,
                       hb_مجموعه_t *variant_glyphs) ثابت
  {
    ثابت hb_array_t<ثابت Offset16_مقصد<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vert_Glyph_شمار + horiz_Glyph_شمار);

    إذا (vertGlyphCoverage)
    {
      ثابت خودکار vert_offsets = glyph_construction_offsets.sub_array (0, vert_Glyph_شمار);
      + hb_zip (هذا+vertGlyphCoverage, vert_offsets)
      | hb_filter (glyph_مجموعه, hb_اولی)
      | hb_map (hb_دومی)
      | hb_map (hb_جمع_کن (هذا))
      | hb_apply ([=] (ثابت MathGlyphConstruction &_) { _.بستگی_glyphs (variant_glyphs); })
      ;
    }

    إذا (horizGlyphCoverage)
    {
      ثابت خودکار hori_offsets = glyph_construction_offsets.sub_array (vert_Glyph_شمار, horiz_Glyph_شمار);
      + hb_zip (هذا+horizGlyphCoverage, hori_offsets)
      | hb_filter (glyph_مجموعه, hb_اولی)
      | hb_map (hb_دومی)
      | hb_map (hb_جمع_کن (هذا))
      | hb_apply ([=] (ثابت MathGlyphConstruction &_) { _.بستگی_glyphs (variant_glyphs); })
      ;
    }
  }

  فراغ collect_coverage_and_indices (hb_sorted_وکتور_t<hb_codepoint_t>& new_coverage,
                                     ثابت Offset16_مقصد<Coverage>& coverage,
                                     بیعلامت i,
                                     بیعلامت انتها_شاخص,
                                     hb_مجموعه_t& indices,
                                     ثابت hb_مجموعه_t& glyphset,
                                     ثابت hb_map_t& glyph_map) ثابت
  {
    إذا (!coverage) أرجع;

    لكل (ثابت خودکار _ : (هذا+coverage).iter ())
    {
      إذا (i >= انتها_شاخص) أرجع;
      إذا (glyphset.has (_))
      {
        بیعلامت new_gid = glyph_map.get (_);
        new_coverage.push (new_gid);
        indices.جمع_کن (i);
      }
      i++;
    }
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_مجموعه_t &glyphset = c->plan->_glyphset_cmaped;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);
    إذا (!c->serializer->check_تخصیص_بده (بیرون->minConnectorOverlap, minConnectorOverlap, HB_SERIALIZE_ERROR_INT_سرریز))
      return_trace (خطأ);

    hb_sorted_وکتور_t<hb_codepoint_t> new_vert_coverage;
    hb_sorted_وکتور_t<hb_codepoint_t> new_hori_coverage;
    hb_مجموعه_t indices;
    collect_coverage_and_indices (new_vert_coverage, vertGlyphCoverage, 0, vert_Glyph_شمار, indices, glyphset, glyph_map);
    collect_coverage_and_indices (new_hori_coverage, horizGlyphCoverage, vert_Glyph_شمار, vert_Glyph_شمار + horiz_Glyph_شمار, indices, glyphset, glyph_map);

    إذا (!c->serializer->check_تخصیص_بده (بیرون->vert_Glyph_شمار, new_vert_coverage.درازی, HB_SERIALIZE_ERROR_INT_سرریز))
      return_trace (خطأ);
    إذا (!c->serializer->check_تخصیص_بده (بیرون->horiz_Glyph_شمار, new_hori_coverage.درازی, HB_SERIALIZE_ERROR_INT_سرریز))
      return_trace (خطأ);

    لكل (بیعلامت i : indices.iter ())
    {
      خودکار *o = c->serializer->embed (glyphConstruction[i]);
      إذا (!o) return_trace (خطأ);
      o->serialize_subset (c, glyphConstruction[i], هذا);
    }

    إذا (new_vert_coverage)
      بیرون->vertGlyphCoverage.serialize_serialize (c->serializer, new_vert_coverage.iter ());

    إذا (new_hori_coverage)
    بیرون->horizGlyphCoverage.serialize_serialize (c->serializer, new_hori_coverage.iter ());
    return_trace (درست);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت خودکار &مدخل : hb_zip (هذا+vertGlyphCoverage, glyphConstruction.as_array (vert_Glyph_شمار)))
      إذا (c->glyph_مجموعه->has (hb_اولی (مدخل)))
        (هذا+hb_دومی (مدخل)).collect_variation_indices (c);
    لكل (ثابت خودکار &مدخل : hb_zip (هذا+horizGlyphCoverage, glyphConstruction.as_array (vert_Glyph_شمار + horiz_Glyph_شمار).sub_array (vert_Glyph_شمار)))
      إذا (c->glyph_مجموعه->has (hb_اولی (مدخل)))
        (هذا+hb_دومی (مدخل)).collect_variation_indices (c);
  }

  منطقي sanitize_offsets (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    بیعلامت صحيح شمار = vert_Glyph_شمار + horiz_Glyph_شمار;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
      إذا (!glyphConstruction.arrayZ[i].sanitize (c, هذا)) return_trace (خطأ);
    return_trace (درست);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  vertGlyphCoverage.sanitize (c, هذا) &&
		  horizGlyphCoverage.sanitize (c, هذا) &&
		  hb_barrier () &&
		  c->check_array (glyphConstruction.arrayZ, vert_Glyph_شمار + horiz_Glyph_شمار) &&
		  sanitize_offsets (c));
  }

  hb_position_t get_min_connector_overlap (hb_direction_t direction,
						  hb_font_t *font) ثابت
  { أرجع font->em_scale_dir (minConnectorOverlap, direction); }

  بیعلامت صحيح get_glyph_variants (hb_codepoint_t glyph,
				   hb_direction_t direction,
				   hb_font_t *font,
				   بیعلامت صحيح شروع_جابه_جایی,
				   بیعلامت صحيح *variants_شمار, /* IN/OUT */
				   hb_ot_math_glyph_variant_t *variants /*بیرون*/) ثابت
  { أرجع get_glyph_construction (glyph, direction, font)
	   .get_variants (direction, font, شروع_جابه_جایی, variants_شمار, variants); }

  بیعلامت صحيح get_glyph_parts (hb_codepoint_t glyph,
				hb_direction_t direction,
				hb_font_t *font,
				بیعلامت صحيح شروع_جابه_جایی,
				بیعلامت صحيح *parts_شمار, /* IN/OUT */
				hb_ot_math_glyph_part_t *parts /*بیرون*/,
				hb_position_t *italics_correction /*بیرون*/) ثابت
  { أرجع get_glyph_construction (glyph, direction, font)
	   .get_assembly ()
	   .get_parts (direction, font,
		       شروع_جابه_جایی, parts_شمار, parts,
		       italics_correction); }

  خاص:
  ثابت MathGlyphConstruction &
  get_glyph_construction (hb_codepoint_t glyph,
			  hb_direction_t direction,
			  hb_font_t *font HB_استفادهنشده) ثابت
  {
    منطقي vertical = HB_DIRECTION_IS_VERTICAL (direction);
    بیعلامت صحيح شمار = vertical ? vert_Glyph_شمار : horiz_Glyph_شمار;
    ثابت Offset16_مقصد<Coverage> &coverage = vertical ? vertGlyphCoverage
						  : horizGlyphCoverage;

    بیعلامت صحيح شاخص_2 = (هذا+coverage).get_coverage (glyph);
    إذا (unlikely (شاخص_2 >= شمار)) أرجع Null (MathGlyphConstruction);

    إذا (!vertical)
      شاخص_2 += vert_Glyph_شمار;

    أرجع هذا+glyphConstruction[شاخص_2];
  }

  محمي:
  HBUINT16	minConnectorOverlap;
				/* Minimum overlap of connecting
				 * glyphs during glyph construction,
				 * in design units. */
  Offset16_مقصد<Coverage> vertGlyphCoverage;
				/* Offset to Coverage table -
				 * from the beginning of MathVariants
				 * table. */
  Offset16_مقصد<Coverage> horizGlyphCoverage;
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
  Unsized_Array_از<Offset16_مقصد<MathGlyphConstruction>>
			glyphConstruction;

  عام:
  DEFINE_اندازه_ARRAY (10, glyphConstruction);
};


/*
 * MATH -- Mathematical typesetting
 * https://docs.microsoft.com/en-us/typography/opentype/spec/math
 */

بنية MATH
{
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_MATH;

  منطقي has_داده () ثابت { أرجع نسخه.مقصد_int (); }

  فراغ depend (hb_depend_داده_builder_t *depend_داده) ثابت
  {
    إذا (mathVariants)
      (هذا+mathVariants).depend (depend_داده);
  }

  فراغ بستگی_glyphs (hb_مجموعه_t *glyph_مجموعه) ثابت
  {
    إذا (mathVariants)
    {
      hb_مجموعه_t variant_glyphs;
      (هذا+mathVariants).بستگی_glyphs (glyph_مجموعه, &variant_glyphs);
      hb_مجموعه_union (glyph_مجموعه, &variant_glyphs);
    }
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->embed (*هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    بیرون->mathConstants.serialize_copy (c->serializer, mathConstants, هذا, 0, hb_serialize_context_t::سر, &c->plan->layout_variation_idx_delta_map);
    بیرون->mathGlyphInfo.serialize_subset (c, mathGlyphInfo, هذا);
    بیرون->mathVariants.serialize_subset (c, mathVariants, هذا);
    return_trace (درست);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c, ثابت hb_مجموعه_t *variant_glyph_مجموعه) ثابت
  {
    (هذا+mathConstants).collect_variation_indices (c);
    (هذا+mathGlyphInfo).collect_variation_indices (c);
    خودکار *glyph_مجموعه = c->glyph_مجموعه;
    c->glyph_مجموعه = variant_glyph_مجموعه;
    (هذا+mathVariants).collect_variation_indices (c);
    c->glyph_مجموعه = glyph_مجموعه;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (نسخه.sanitize (c) &&
		  likely (نسخه.major == 1) &&
		  hb_barrier () &&
		  mathConstants.sanitize (c, هذا) &&
		  mathGlyphInfo.sanitize (c, هذا) &&
		  mathVariants.sanitize (c, هذا));
  }

  // https://github.com/harfbuzz/harfbuzz/issues/4653
  HB_داخلی منطقي is_bad_cambria (hb_font_t *font) ثابت
  {
#اگر_تعریف_نشده HB_NO_MATH
    بدل HB_CODEPOINT_ENCODE3 (font->face->جدول.MATH.get_blob ()->درازی,
                                 (هذا+mathConstants).min_ارتفاع[1], // displayOperatorMinHeight
                                 (هذا+mathConstants).min_ارتفاع[0]) // delimitedSubFormulaMinHeight
    {
      /* sha1sum:ab4a4fe054d23061f3c039493d6f665cfda2ecf5  cambria.ttc
       * sha1sum:086855301bff644f9d8827b88491fcf73a6d4cb9  cambria.ttc
       * sha1sum:b1e5a3feaca2ea3dfcf79ccb377de749ecf60343  cambria.ttc */
      حالة HB_CODEPOINT_ENCODE3 (25722, 2500, 3000):
        أرجع درست;
    }
#پایان_اگر
    أرجع خطأ;
  }

  hb_position_t get_constant (hb_ot_math_constant_t  constant,
			      hb_font_t		   *font) ثابت
  { أرجع (هذا+mathConstants).get_مقدار (constant, font); }

  ثابت MathGlyphInfo &get_glyph_info () ثابت { أرجع هذا+mathGlyphInfo; }

  ثابت MathVariants &get_variants () ثابت    { أرجع هذا+mathVariants; }

  محمي:
  Fixed_نسخه<>نسخه;	/* Version of the MATH table
				 * initially set to 0x00010000u */
  Offset16_مقصد<MathConstants>
		mathConstants;	/* MathConstants table */
  Offset16_مقصد<MathGlyphInfo>
		mathGlyphInfo;	/* MathGlyphInfo table */
  Offset16_مقصد<MathVariants>
		mathVariants;	/* MathVariants table */

  عام:
  DEFINE_اندازه_STATIC (10);
};

} /* namespace OT */


#پایان_اگر /* HB_OT_MATH_TABLE_HH */

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

#که_نه_تعریف_شوی HB_OT_MATH_جدول_HH
#تعریف_کړه HB_OT_MATH_جدول_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-layout-common.hh"
#ضمن "hb-ot-layout-gdef-table.hh"
#ضمن "hb-ot-math.h"
#ضمن "hb-depend-data.hh"

نطاق OT {


بنية Math_ارزښت_ریکارډ
{
  hb_position_t get_x_ارزښت (hb_font_t *font, ثابت فراغ *بنسټ) ثابت
  { أرجع hb_saturate_جمع_کړه (font->em_scale_x (value), (بنسټ+device_جدول).get_x_delta (font, get_var_store (font, بنسټ))); }
  hb_position_t get_y_ارزښت (hb_font_t *font, ثابت فراغ *بنسټ) ثابت
  { أرجع hb_saturate_جمع_کړه (font->em_scale_y (value), (بنسټ+device_جدول).get_y_delta (font, get_var_store (font, بنسټ))); }

  ثابت توکی_Variation_Store &get_var_store (hb_font_t *font, ثابت فراغ *بنسټ) ثابت
  {
#إذا !defined(HB_NO_VAR) && !defined(HB_NO_OT_LAYOUT)
    إذا ((بنسټ+device_جدول).is_variation_device ())
      أرجع font->face->جدول.GDEF->جدول->get_var_store ();
#د_که_پای
    أرجع Null (توکی_Variation_Store);
  }

  Math_ارزښت_ریکارډ* copy (hb_serialize_context_t *c, ثابت فراغ *بنسټ,
                         ثابت hb_hashmap_t<بینښه, hb_pair_t<بینښه, صحيح>> *variation_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خپلکاری *بهر = c->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (تش_اشاره);
    hb_pair_t<بینښه, صحيح> *varidx_delta;
    إذا (variation_map->has ((بنسټ+device_جدول).get_variation_شاخص (), &varidx_delta) &&
        !c->check_ټاکل (بهر->value, (int64_t) value + hb_دوهم (*varidx_delta),
                          HB_SERIALIZE_ERROR_INT_ډکېدنه))
      return_trace (تش_اشاره);
    بهر->device_جدول.serialize_copy (c, device_جدول, بنسټ, 0, hb_serialize_context_t::سر, variation_map);

    return_trace (بهر);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c, ثابت فراغ *بنسټ) ثابت
  { (بنسټ+device_جدول).collect_variation_indices (c); }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && device_جدول.sanitize (c, بنسټ));
  }

  محمي:
  HBINT16		value;		/* The X or Y value in design units */
  Offset16_منزل<Device>	device_جدول;	/* Offset to the device table - from the
					 * beginning of parent table.  May be NULL.
					 * Suggested format for device table is 1. */

  عام:
  DEFINE_اندازه_STATIC (4);
};

بنية MathConstants
{
  ملګری بنية MATH;

  MathConstants* copy (hb_serialize_context_t *c,
                      ثابت hb_hashmap_t<بینښه, hb_pair_t<بینښه, صحيح>> *variation_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خپلکاری *بهر = c->پیل_embed (هذا);

    HBINT16 *p = c->allocate_اندازه<HBINT16> (HBINT16::static_اندازه * 2);
    إذا (unlikely (!p)) return_trace (تش_اشاره);
    hb_memcpy (p, percent_Scale_ښکته, HBINT16::static_اندازه * 2);

    HBUINT16 *m = c->allocate_اندازه<HBUINT16> (HBUINT16::static_اندازه * 2);
    إذا (unlikely (!m)) return_trace (تش_اشاره);
    hb_memcpy (m, min_لوړوالی, HBUINT16::static_اندازه * 2);

    بینښه شمېر = ARRAY_اوږدتیا (math_ارزښت_ریکارډونه);
    لكل (بینښه i = 0; i < شمېر; i++)
      إذا (!c->copy (math_ارزښت_ریکارډونه[i], هذا, variation_map))
        return_trace (تش_اشاره);

    إذا (!c->embed (radicalDegreeBottomRaisePercent)) return_trace (تش_اشاره);
    return_trace (بهر);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت خپلکاری &ریکارډ : math_ارزښت_ریکارډونه)
      ریکارډ.collect_variation_indices (c, هذا);
  }

  منطقي sanitize_math_ارزښت_ریکارډونه (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);

    بینښه صحيح شمېر = ARRAY_اوږدتیا (math_ارزښت_ریکارډونه);
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
      إذا (!math_ارزښت_ریکارډونه[i].sanitize (c, هذا))
	return_trace (خطأ);

    return_trace (ریښتیا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && sanitize_math_ارزښت_ریکارډونه (c));
  }

  hb_position_t get_ارزښت (hb_ot_math_constant_t constant,
			   hb_font_t *font) ثابت
  {
    بدل (constant) {

    حالة HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_ښکته:
    حالة HB_OT_MATH_CONSTANT_SCRIPT_SCRIPT_PERCENT_SCALE_ښکته:
      أرجع percent_Scale_ښکته[constant - HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_ښکته];

    حالة HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_لوړوالی:
    حالة HB_OT_MATH_CONSTANT_DISPLAY_OPERATOR_MIN_لوړوالی:
      أرجع font->em_scale_y (min_لوړوالی[constant - HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_لوړوالی]);

    حالة HB_OT_MATH_CONSTANT_RADICAL_KERN_AFTER_DEGREE:
    حالة HB_OT_MATH_CONSTANT_RADICAL_KERN_BEFORE_DEGREE:
    حالة HB_OT_MATH_CONSTANT_SKEWED_FRACTION_HORIZONTAL_GAP:
    حالة HB_OT_MATH_CONSTANT_SPACE_AFTER_SCRIPT:
      أرجع math_ارزښت_ریکارډونه[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_x_ارزښت (font, هذا);

    حالة HB_OT_MATH_CONSTANT_ACCENT_بنسټ_لوړوالی:
    حالة HB_OT_MATH_CONSTANT_AXIS_لوړوالی:
    حالة HB_OT_MATH_CONSTANT_FLATTENED_ACCENT_بنسټ_لوړوالی:
    حالة HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_DISPLAY_STYLE_ښويدنه_ښکته:
    حالة HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_ښويدنه_ښکته:
    حالة HB_OT_MATH_CONSTANT_FRACTION_DENOM_DISPLAY_STYLE_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_DISPLAY_STYLE_ښويدنه_پورته:
    حالة HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_ښويدنه_پورته:
    حالة HB_OT_MATH_CONSTANT_FRACTION_عدد_DISPLAY_STYLE_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_FRACTION_قاعده_THICKNESS:
    حالة HB_OT_MATH_CONSTANT_ښکتنی_حد_BASELINE_DROP_MIN:
    حالة HB_OT_MATH_CONSTANT_ښکتنی_حد_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_MATH_LEADING:
    حالة HB_OT_MATH_CONSTANT_OVERBAR_EXTRA_ASCENDER:
    حالة HB_OT_MATH_CONSTANT_OVERBAR_قاعده_THICKNESS:
    حالة HB_OT_MATH_CONSTANT_OVERBAR_VERTICAL_GAP:
    حالة HB_OT_MATH_CONSTANT_RADICAL_DISPLAY_STYLE_VERTICAL_GAP:
    حالة HB_OT_MATH_CONSTANT_RADICAL_EXTRA_ASCENDER:
    حالة HB_OT_MATH_CONSTANT_RADICAL_قاعده_THICKNESS:
    حالة HB_OT_MATH_CONSTANT_RADICAL_VERTICAL_GAP:
    حالة HB_OT_MATH_CONSTANT_SKEWED_FRACTION_VERTICAL_GAP:
    حالة HB_OT_MATH_CONSTANT_ډېری_BOTTOM_DISPLAY_STYLE_ښويدنه_ښکته:
    حالة HB_OT_MATH_CONSTANT_ډېری_BOTTOM_ښويدنه_ښکته:
    حالة HB_OT_MATH_CONSTANT_ډېری_DISPLAY_STYLE_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_ډېری_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_ډېری_سر_DISPLAY_STYLE_ښويدنه_پورته:
    حالة HB_OT_MATH_CONSTANT_ډېری_سر_ښويدنه_پورته:
    حالة HB_OT_MATH_CONSTANT_STRETCH_ډېری_BOTTOM_ښويدنه_ښکته:
    حالة HB_OT_MATH_CONSTANT_STRETCH_ډېری_GAP_ABOVE_MIN:
    حالة HB_OT_MATH_CONSTANT_STRETCH_ډېری_GAP_لاندې_MIN:
    حالة HB_OT_MATH_CONSTANT_STRETCH_ډېری_سر_ښويدنه_پورته:
    حالة HB_OT_MATH_CONSTANT_SUBSCRIPT_BASELINE_DROP_MIN:
    حالة HB_OT_MATH_CONSTANT_SUBSCRIPT_ښويدنه_ښکته:
    حالة HB_OT_MATH_CONSTANT_SUBSCRIPT_سر_MAX:
    حالة HB_OT_MATH_CONSTANT_SUB_SUPERSCRIPT_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_SUPERSCRIPT_BASELINE_DROP_MAX:
    حالة HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MAX_WITH_SUBSCRIPT:
    حالة HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MIN:
    حالة HB_OT_MATH_CONSTANT_SUPERSCRIPT_ښويدنه_پورته:
    حالة HB_OT_MATH_CONSTANT_SUPERSCRIPT_ښويدنه_پورته_CRAMPED:
    حالة HB_OT_MATH_CONSTANT_UNDERBAR_EXTRA_DESCENDER:
    حالة HB_OT_MATH_CONSTANT_UNDERBAR_قاعده_THICKNESS:
    حالة HB_OT_MATH_CONSTANT_UNDERBAR_VERTICAL_GAP:
    حالة HB_OT_MATH_CONSTANT_پاسنی_حد_BASELINE_RISE_MIN:
    حالة HB_OT_MATH_CONSTANT_پاسنی_حد_GAP_MIN:
      أرجع math_ارزښت_ریکارډونه[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_y_ارزښت (font, هذا);

    حالة HB_OT_MATH_CONSTANT_RADICAL_DEGREE_BOTTOM_RAISE_PERCENT:
      أرجع radicalDegreeBottomRaisePercent;

    افتراضي:
      أرجع 0;
    }
  }

  محمي:
  HBINT16 percent_Scale_ښکته[2];
  HBUINT16 min_لوړوالی[2];
  Math_ارزښت_ریکارډ math_ارزښت_ریکارډونه[51];
  HBINT16 radicalDegreeBottomRaisePercent;

  عام:
  DEFINE_اندازه_STATIC (214);
};

بنية MathItalicsCorrectionInfo
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سېټ_t &glyphset = c->plan->_glyphset_mathed;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);

    hb_sorted_ویکټور_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+coverage, italicsCorrection)
    | hb_filter (glyphset, hb_لومړی)
    | hb_filter (serialize_math_ریکارډ_array (c->serializer, بهر->italicsCorrection, هذا, &c->plan->layout_variation_idx_delta_map), hb_دوهم)
    | hb_map (hb_لومړی)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    بهر->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (ریښتیا);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت خپلکاری &ثبت : hb_zip (هذا+coverage, italicsCorrection))
      إذا (c->glyph_سېټ->has (hb_لومړی (ثبت)))
        hb_دوهم (ثبت).collect_variation_indices (c, هذا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  coverage.sanitize (c, هذا) &&
		  italicsCorrection.sanitize (c, هذا));
  }

  hb_position_t get_ارزښت (hb_codepoint_t glyph,
			   hb_font_t *font) ثابت
  {
    بینښه صحيح شاخص_2 = (هذا+coverage).get_coverage (glyph);
    أرجع italicsCorrection[شاخص_2].get_x_ارزښت (font, هذا);
  }

  محمي:
  Offset16_منزل<Coverage>       coverage;		/* Offset to Coverage table -
						 * from the beginning of
						 * MathItalicsCorrectionInfo
						 * table. */
  Array16_د<Math_ارزښت_ریکارډ> italicsCorrection;	/* Array of MathValueRecords
						 * defining italics correction
						 * values for each
						 * covered glyph. */

  عام:
  DEFINE_اندازه_ARRAY (4, italicsCorrection);
};

بنية Math_سر_Accent_Attachment
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سېټ_t &glyphset = c->plan->_glyphset_mathed;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);

    hb_sorted_ویکټور_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+سر_Accent_Coverage, سر_Accent_Attachment)
    | hb_filter (glyphset, hb_لومړی)
    | hb_filter (serialize_math_ریکارډ_array (c->serializer, بهر->سر_Accent_Attachment, هذا, &c->plan->layout_variation_idx_delta_map), hb_دوهم)
    | hb_map (hb_لومړی)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    بهر->سر_Accent_Coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (ریښتیا);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت خپلکاری &ثبت : hb_zip (هذا+سر_Accent_Coverage, سر_Accent_Attachment))
      إذا (c->glyph_سېټ->has (hb_لومړی (ثبت)))
        hb_دوهم (ثبت).collect_variation_indices (c, هذا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  سر_Accent_Coverage.sanitize (c, هذا) &&
		  سر_Accent_Attachment.sanitize (c, هذا));
  }

  hb_position_t get_ارزښت (hb_codepoint_t glyph,
			   hb_font_t *font) ثابت
  {
    بینښه صحيح شاخص_2 = (هذا+سر_Accent_Coverage).get_coverage (glyph);
    إذا (شاخص_2 == NOT_COVERED)
      أرجع font->get_glyph_h_advance (glyph) / 2;
    أرجع سر_Accent_Attachment[شاخص_2].get_x_ارزښت (font, هذا);
  }

  محمي:
  Offset16_منزل<Coverage>       سر_Accent_Coverage;   /* Offset to Coverage table -
						 * from the beginning of
						 * MathTopAccentAttachment
						 * table. */
  Array16_د<Math_ارزښت_ریکارډ> سر_Accent_Attachment; /* Array of MathValueRecords
						 * defining top accent
						 * attachment points for each
						 * covered glyph. */

  عام:
  DEFINE_اندازه_ARRAY (2 + 2, سر_Accent_Attachment);
};

بنية MathKern
{
  MathKern* copy (hb_serialize_context_t *c,
                      ثابت hb_hashmap_t<بینښه, hb_pair_t<بینښه, صحيح>> *variation_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خپلکاری *بهر = c->پیل_embed (هذا);

    إذا (unlikely (!c->embed (لوړوالی_شمېر))) return_trace (تش_اشاره);

    بینښه شمېر = 2 * لوړوالی_شمېر + 1;
    لكل (بینښه i = 0; i < شمېر; i++)
      إذا (!c->copy (math_ارزښت_ریکارډونه_Z.arrayZ[i], هذا, variation_map))
        return_trace (تش_اشاره);

    return_trace (بهر);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت خپلکاری &ریکارډ : math_ارزښت_ریکارډونه_Z.as_array (2 * لوړوالی_شمېر + 1))
      ریکارډ.collect_variation_indices (c, هذا);
  }

  منطقي sanitize_math_ارزښت_ریکارډونه (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    بینښه صحيح شمېر = 2 * لوړوالی_شمېر + 1;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
      إذا (!math_ارزښت_ریکارډونه_Z.arrayZ[i].sanitize (c, هذا)) return_trace (خطأ);
    return_trace (ریښتیا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  c->check_array (math_ارزښت_ریکارډونه_Z.arrayZ, 2 * لوړوالی_شمېر + 1) &&
		  sanitize_math_ارزښت_ریکارډونه (c));
  }

  hb_position_t get_ارزښت (hb_position_t correction_لوړوالی, hb_font_t *font) ثابت
  {
    ثابت Math_ارزښت_ریکارډ* correction_لوړوالی_2 = math_ارزښت_ریکارډونه_Z.arrayZ;
    ثابت Math_ارزښت_ریکارډ* kern_ارزښت = math_ارزښت_ریکارډونه_Z.arrayZ + لوړوالی_شمېر;
    صحيح sign = font->y_scale < 0 ? -1 : +1;

    /* According to OpenType spec (v1.9), except for the boundary cases, the index
     * chosen for kern value should be i such that
     *    correctionHeight[i-1] <= correction_height < correctionHeight[i]
     * We can use the binary search algorithm of std::upper_bound(). Or, we can
     * use the internal hb_bsearch_impl.
     */
    بینښه صحيح pos;
    خپلکاری cmp = +[](ثابت فراغ* key, ثابت فراغ* p,
                   صحيح sign, hb_font_t* font, ثابت MathKern* mathKern) -> صحيح {
      hb_position_t a = *(hb_position_t *) key;
      hb_position_t b = ((Math_ارزښت_ریکارډ *) p)->get_y_ارزښت (font, mathKern);
      إذا (a == b) أرجع 0;
      أرجع sign > 0 ? (a < b ? -1 : +1) : (b < a ? -1 : +1);
    };
    بینښه صحيح i = hb_bsearch_impl(&pos, correction_لوړوالی, correction_لوړوالی_2,
                                     لوړوالی_شمېر, Math_ارزښت_ریکارډ::static_اندازه,
                                     cmp, sign, font, هذا) ? pos + 1 : pos;
    أرجع kern_ارزښت[i].get_x_ارزښت (font, هذا);
  }

  بینښه صحيح get_entries (بینښه صحيح پیل_دننه_واټن,
			    بینښه صحيح *entries_شمېر, /* IN/OUT */
			    hb_ot_math_kern_ثبت_t *kern_entries, /*بهر*/
			    hb_font_t *font) ثابت
  {
    ثابت Math_ارزښت_ریکارډ* correction_لوړوالی_2 = math_ارزښت_ریکارډونه_Z.arrayZ;
    ثابت Math_ارزښت_ریکارډ* kern_ارزښت = math_ارزښت_ریکارډونه_Z.arrayZ + لوړوالی_شمېر;
    ثابت بینښه صحيح entries_شمېر_2 = لوړوالی_شمېر + 1;

    إذا (entries_شمېر && kern_entries)
    {
      بینښه صحيح پیل = hb_min (پیل_دننه_واټن, entries_شمېر_2);
      بینښه صحيح پای_ټکی = hb_min (پیل + *entries_شمېر, entries_شمېر_2);
      *entries_شمېر = پای_ټکی - پیل;

      لكل (بینښه صحيح i = 0; i < *entries_شمېر; i++) {
	بینښه صحيح j = پیل + i;

	hb_position_t max_لوړوالی;
	إذا (j == لوړوالی_شمېر) {
	  max_لوړوالی = INT32_MAX;
	} وإلا {
	  max_لوړوالی = correction_لوړوالی_2[j].get_y_ارزښت (font, هذا);
	}

	kern_entries[i] = {max_لوړوالی, kern_ارزښت[j].get_x_ارزښت (font, هذا)};
      }
    }
    أرجع entries_شمېر_2;
  }

  محمي:
  HBUINT16	لوړوالی_شمېر;
  Unsized_Array_د<Math_ارزښت_ریکارډ>
		math_ارزښت_ریکارډونه_Z;
				/* Array of correction heights at
				 * which the kern value changes.
				 * Sorted by the height value in
				 * design units (heightCount entries),
				 * Followed by:
				 * Array of kern values corresponding
				 * to heights. (heightCount+1 entries).
				 */

  عام:
  DEFINE_اندازه_ARRAY (2, math_ارزښت_ریکارډونه_Z);
};

بنية Math_Kern_Info_ریکارډ
{
  Math_Kern_Info_ریکارډ* copy (hb_serialize_context_t *c, ثابت فراغ *بنسټ,
                            ثابت hb_hashmap_t<بینښه, hb_pair_t<بینښه, صحيح>> *variation_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خپلکاری *بهر = c->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (تش_اشاره);

    بینښه شمېر = ARRAY_اوږدتیا (mathKern);
    لكل (بینښه i = 0; i < شمېر; i++)
      بهر->mathKern[i].serialize_copy (c, mathKern[i], بنسټ, 0, hb_serialize_context_t::سر, variation_map);

    return_trace (بهر);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c, ثابت فراغ *بنسټ) ثابت
  {
    لكل (ثابت خپلکاری &دننه_واټن : mathKern)
      (بنسټ+دننه_واټن).collect_variation_indices (c);
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);

    بینښه صحيح شمېر = ARRAY_اوږدتیا (mathKern);
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
      إذا (unlikely (!mathKern[i].sanitize (c, بنسټ)))
	return_trace (خطأ);

    return_trace (ریښتیا);
  }

  hb_position_t get_kerning (hb_ot_math_kern_t kern,
			     hb_position_t correction_لوړوالی,
			     hb_font_t *font,
			     ثابت فراغ *بنسټ) ثابت
  {
    بینښه صحيح idx = kern;
    إذا (unlikely (idx >= ARRAY_اوږدتیا (mathKern))) أرجع 0;
    أرجع (بنسټ+mathKern[idx]).get_ارزښت (correction_لوړوالی, font);
  }

  بینښه صحيح get_kernings (hb_ot_math_kern_t kern,
			     بینښه صحيح پیل_دننه_واټن,
			     بینښه صحيح *entries_شمېر, /* IN/OUT */
			     hb_ot_math_kern_ثبت_t *kern_entries, /*بهر*/
			     hb_font_t *font,
			     ثابت فراغ *بنسټ) ثابت
  {
    بینښه صحيح idx = kern;
    إذا (unlikely (idx >= ARRAY_اوږدتیا (mathKern)) || !mathKern[idx]) {
      إذا (entries_شمېر) *entries_شمېر = 0;
      أرجع 0;
    }
    أرجع (بنسټ+mathKern[idx]).get_entries (پیل_دننه_واټن,
					     entries_شمېر,
					     kern_entries,
					     font);
  }

  محمي:
  /* Offset to MathKern table for each corner -
   * from the beginning of MathKernInfo table.  May be NULL. */
  Offset16_منزل<MathKern> mathKern[4];

  عام:
  DEFINE_اندازه_STATIC (8);
};

بنية MathKernInfo
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سېټ_t &glyphset = c->plan->_glyphset_mathed;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);

    hb_sorted_ویکټور_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+mathKernCoverage, math_Kern_Info_ریکارډونه)
    | hb_filter (glyphset, hb_لومړی)
    | hb_filter (serialize_math_ریکارډ_array (c->serializer, بهر->math_Kern_Info_ریکارډونه, هذا, &c->plan->layout_variation_idx_delta_map), hb_دوهم)
    | hb_map (hb_لومړی)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    بهر->mathKernCoverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (ریښتیا);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت خپلکاری &ثبت : hb_zip (هذا+mathKernCoverage, math_Kern_Info_ریکارډونه))
      إذا (c->glyph_سېټ->has (hb_لومړی (ثبت)))
        hb_دوهم (ثبت).collect_variation_indices (c, هذا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  mathKernCoverage.sanitize (c, هذا) &&
		  math_Kern_Info_ریکارډونه.sanitize (c, هذا));
  }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_لوړوالی,
			     hb_font_t *font) ثابت
  {
    بینښه صحيح شاخص_2 = (هذا+mathKernCoverage).get_coverage (glyph);
    أرجع math_Kern_Info_ریکارډونه[شاخص_2].get_kerning (kern, correction_لوړوالی, font, هذا);
  }

  بینښه صحيح get_kernings (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     بینښه صحيح پیل_دننه_واټن,
			     بینښه صحيح *entries_شمېر, /* IN/OUT */
			     hb_ot_math_kern_ثبت_t *kern_entries, /*بهر*/
			     hb_font_t *font) ثابت
  {
    بینښه صحيح شاخص_2 = (هذا+mathKernCoverage).get_coverage (glyph);
    أرجع math_Kern_Info_ریکارډونه[شاخص_2].get_kernings (kern,
						    پیل_دننه_واټن,
						    entries_شمېر,
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
  Array16_د<Math_Kern_Info_ریکارډ>
		math_Kern_Info_ریکارډونه;
				/* Array of MathKernInfoRecords,
				 * per-glyph information for
				 * mathematical positioning
				 * of subscripts and
				 * superscripts. */

  عام:
  DEFINE_اندازه_ARRAY (4, math_Kern_Info_ریکارډونه);
};

بنية MathGlyphInfo
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->embed (*هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    بهر->mathItalicsCorrectionInfo.serialize_subset (c, mathItalicsCorrectionInfo, هذا);
    بهر->math_سر_Accent_Attachment.serialize_subset (c, math_سر_Accent_Attachment, هذا);

    ثابت hb_سېټ_t &glyphset = c->plan->_glyphset_mathed;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خپلکاری it =
    + hb_iter (هذا+extendedShapeCoverage)
    | hb_take (c->plan->سرچینه->get_عدد_glyphs ())
    | hb_filter (glyphset)
    | hb_map_retains_ترتیبونه (glyph_map)
    ;

    إذا (it) بهر->extendedShapeCoverage.serialize_serialize (c->serializer, it);
    وإلا بهر->extendedShapeCoverage = 0;

    بهر->mathKernInfo.serialize_subset (c, mathKernInfo, هذا);
    return_trace (ریښتیا);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    (هذا+mathItalicsCorrectionInfo).collect_variation_indices (c);
    (هذا+math_سر_Accent_Attachment).collect_variation_indices (c);
    (هذا+mathKernInfo).collect_variation_indices (c);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  mathItalicsCorrectionInfo.sanitize (c, هذا) &&
		  math_سر_Accent_Attachment.sanitize (c, هذا) &&
		  extendedShapeCoverage.sanitize (c, هذا) &&
		  mathKernInfo.sanitize (c, هذا));
  }

  hb_position_t
  get_italics_correction (hb_codepoint_t  glyph, hb_font_t *font) ثابت
  { أرجع (هذا+mathItalicsCorrectionInfo).get_ارزښت (glyph, font); }

  hb_position_t
  get_سر_accent_attachment (hb_codepoint_t  glyph, hb_font_t *font) ثابت
  { أرجع (هذا+math_سر_Accent_Attachment).get_ارزښت (glyph, font); }

  منطقي is_extended_shape (hb_codepoint_t glyph) ثابت
  { أرجع (هذا+extendedShapeCoverage).get_coverage (glyph) != NOT_COVERED; }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_لوړوالی,
			     hb_font_t *font) ثابت
  { أرجع (هذا+mathKernInfo).get_kerning (glyph, kern, correction_لوړوالی, font); }

  hb_position_t get_kernings (hb_codepoint_t glyph,
			      hb_ot_math_kern_t kern,
			      بینښه صحيح پیل_دننه_واټن,
			      بینښه صحيح *entries_شمېر, /* IN/OUT */
			      hb_ot_math_kern_ثبت_t *kern_entries, /*بهر*/
			      hb_font_t *font) ثابت
  { أرجع (هذا+mathKernInfo).get_kernings (glyph,
					     kern,
					     پیل_دننه_واټن,
					     entries_شمېر,
					     kern_entries,
					     font); }

  محمي:
  /* Offset to MathItalicsCorrectionInfo table -
   * from the beginning of MathGlyphInfo table. */
  Offset16_منزل<MathItalicsCorrectionInfo> mathItalicsCorrectionInfo;

  /* Offset to MathTopAccentAttachment table -
   * from the beginning of MathGlyphInfo table. */
  Offset16_منزل<Math_سر_Accent_Attachment> math_سر_Accent_Attachment;

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
  DEFINE_اندازه_STATIC (8);
};

بنية Math_Glyph_Variant_ریکارډ
{
  ملګری بنية MathGlyphConstruction;

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    ثابت hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_ټاکل (بهر->variantGlyph, glyph_map.get (variantGlyph), HB_SERIALIZE_ERROR_INT_ډکېدنه));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  فراغ depend (hb_depend_معلومات_builder_t *depend_معلومات, بینښه سرچینه) ثابت
  { depend_معلومات->جمع_کړه_depend(سرچینه, HB_OT_نښان_MATH, variantGlyph); }

  فراغ تړون_glyphs (hb_سېټ_t *variant_glyphs) ثابت
  { variant_glyphs->جمع_کړه (variantGlyph); }

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

بنية Math_Glyph_Part_ریکارډ
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    ثابت hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_ټاکل (بهر->glyph, glyph_map.get (glyph), HB_SERIALIZE_ERROR_INT_ډکېدنه));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  فراغ extract (hb_ot_math_glyph_part_t &بهر,
		int64_t mult,
		hb_font_t *font) ثابت
  {
    بهر.glyph			= glyph;

    بهر.پیل_connector_اوږدتیا	= font->em_mult (پیل_Connector_اوږدتیا, mult);
    بهر.پای_ټکی_connector_اوږدتیا	= font->em_mult (پای_ټکی_Connector_اوږدتیا, mult);
    بهر.ډک_advance		= font->em_mult (ډک_Advance, mult);

    static_assert ((بینښه صحيح) HB_OT_MATH_GLYPH_PART_FLAG_EXTENDER ==
		   (بینښه صحيح) PartFlags::Extender, "");

    بهر.flags = (hb_ot_math_glyph_part_flags_t)
		(بینښه صحيح)
		(partFlags & PartFlags::Defined);
  }

  فراغ depend (hb_depend_معلومات_builder_t *depend_معلومات, بینښه سرچینه) ثابت
  { depend_معلومات->جمع_کړه_depend(سرچینه, HB_OT_نښان_MATH, glyph); }

  فراغ تړون_glyphs (hb_سېټ_t *variant_glyphs) ثابت
  { variant_glyphs->جمع_کړه (glyph); }

  محمي:
  HBGlyphID16	glyph;		/* Glyph ID for the part. */
  HBUINT16	پیل_Connector_اوږدتیا;
				/* Advance width/ height of the straight bar
				 * connector material, in design units, is at
				 * the beginning of the glyph, in the
				 * direction of the extension. */
  HBUINT16	پای_ټکی_Connector_اوږدتیا;
				/* Advance width/ height of the straight bar
				 * connector material, in design units, is at
				 * the end of the glyph, in the direction of
				 * the extension. */
  HBUINT16	ډک_Advance;	/* Full advance width/height for this part,
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
    إذا (!c->serializer->copy<HBUINT16> (part_ریکارډونه.len)) return_trace (خطأ);

    لكل (ثابت خپلکاری& ریکارډ : part_ریکارډونه.iter ())
      إذا (!ریکارډ.subset (c)) return_trace (خطأ);
    return_trace (ریښتیا);
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
		  part_ریکارډونه.sanitize (c));
  }

  بینښه صحيح get_parts (hb_direction_t direction,
			  hb_font_t *font,
			  بینښه صحيح پیل_دننه_واټن,
			  بینښه صحيح *parts_شمېر, /* IN/OUT */
			  hb_ot_math_glyph_part_t *parts /*بهر*/,
			  hb_position_t *italics_correction /*بهر*/) ثابت
  {
    إذا (parts_شمېر && parts)
    {
      int64_t mult = font->dir_mult (direction);
      لكل (خپلکاری _ : hb_zip (part_ریکارډونه.as_array ().sub_array (پیل_دننه_واټن, parts_شمېر),
			    hb_array (parts, *parts_شمېر)))
	_.لومړی.extract (_.second, mult, font);
    }

    إذا (italics_correction)
      *italics_correction = italicsCorrection.get_x_ارزښت (font, هذا);

    أرجع part_ریکارډونه.len;
  }

  فراغ depend (hb_depend_معلومات_builder_t *depend_معلومات, بینښه سرچینه) ثابت
  {
    لكل (ثابت خپلکاری& _ : part_ریکارډونه.iter ())
      _.depend (depend_معلومات, سرچینه);
  }

  فراغ تړون_glyphs (hb_سېټ_t *variant_glyphs) ثابت
  {
    لكل (ثابت خپلکاری& _ : part_ریکارډونه.iter ())
      _.تړون_glyphs (variant_glyphs);
  }

  محمي:
  Math_ارزښت_ریکارډ
		italicsCorrection;
				/* Italics correction of this
				 * MathGlyphAssembly. Should not
				 * depend on the assembly size. */
  Array16_د<Math_Glyph_Part_ریکارډ>
		part_ریکارډونه;	/* Array of part records, from
				 * left to right and bottom to
				 * top. */

  عام:
  DEFINE_اندازه_ARRAY (6, part_ریکارډونه);
};

بنية MathGlyphConstruction
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);

    بهر->glyphAssembly.serialize_subset (c, glyphAssembly, هذا);

    إذا (!c->serializer->check_ټاکل (بهر->math_Glyph_Variant_ریکارډ.len, math_Glyph_Variant_ریکارډ.len, HB_SERIALIZE_ERROR_INT_ډکېدنه))
      return_trace (خطأ);
    لكل (ثابت خپلکاری& ریکارډ : math_Glyph_Variant_ریکارډ.iter ())
      إذا (!ریکارډ.subset (c)) return_trace (خطأ);

    return_trace (ریښتیا);
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
		  math_Glyph_Variant_ریکارډ.sanitize (c));
  }

  ثابت MathGlyphAssembly &get_assembly () ثابت { أرجع هذا+glyphAssembly; }

  بینښه صحيح get_variants (hb_direction_t direction,
			     hb_font_t *font,
			     بینښه صحيح پیل_دننه_واټن,
			     بینښه صحيح *variants_شمېر, /* IN/OUT */
			     hb_ot_math_glyph_variant_t *variants /*بهر*/) ثابت
  {
    إذا (variants_شمېر && variants)
    {
      int64_t mult = font->dir_mult (direction);
      لكل (خپلکاری _ : hb_zip (math_Glyph_Variant_ریکارډ.as_array ().sub_array (پیل_دننه_واټن, variants_شمېر),
			    hb_array (variants, *variants_شمېر)))
	_.second = {_.لومړی.variantGlyph, font->em_mult (_.لومړی.advanceMeasurement, mult)};
    }
    أرجع math_Glyph_Variant_ریکارډ.len;
  }

  فراغ depend (hb_depend_معلومات_builder_t *depend_معلومات, بینښه سرچینه) ثابت
  {
    (هذا+glyphAssembly).depend (depend_معلومات, سرچینه);

    لكل (ثابت خپلکاری& _ : math_Glyph_Variant_ریکارډ.iter ())
      _.depend (depend_معلومات, سرچینه);
  }

  فراغ تړون_glyphs (hb_سېټ_t *variant_glyphs) ثابت
  {
    (هذا+glyphAssembly).تړون_glyphs (variant_glyphs);

    لكل (ثابت خپلکاری& _ : math_Glyph_Variant_ریکارډ.iter ())
      _.تړون_glyphs (variant_glyphs);
  }

  محمي:
  /* Offset to MathGlyphAssembly table for this shape - from the beginning of
     MathGlyphConstruction table.  May be NULL. */
  Offset16_منزل<MathGlyphAssembly>	  glyphAssembly;

  /* MathGlyphVariantRecords for alternative variants of the glyphs. */
  Array16_د<Math_Glyph_Variant_ریکارډ> math_Glyph_Variant_ریکارډ;

  عام:
  DEFINE_اندازه_ARRAY (4, math_Glyph_Variant_ریکارډ);
};

بنية MathVariants
{
  فراغ depend (hb_depend_معلومات_builder_t *depend_معلومات) ثابت
  {
    ثابت hb_array_t<ثابت Offset16_منزل<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vert_Glyph_شمېر + horiz_Glyph_شمېر);

    إذا (vertGlyphCoverage)
    {
      ثابت خپلکاری vert_offsets = glyph_construction_offsets.sub_array (0, vert_Glyph_شمېر);
      + hb_zip (هذا+vertGlyphCoverage, vert_offsets)
      | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, ثابت Offset16_منزل<MathGlyphConstruction>&> &_)
                  {
                    ثابت MathGlyphConstruction &mgc = هذا+_.second;
                    mgc.depend (depend_معلومات, _.لومړی);
                  })
      ;
    }
    إذا (horizGlyphCoverage)
    {
      ثابت خپلکاری hori_offsets = glyph_construction_offsets.sub_array (vert_Glyph_شمېر, horiz_Glyph_شمېر);
      + hb_zip (هذا+horizGlyphCoverage, hori_offsets)
      | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, ثابت Offset16_منزل<MathGlyphConstruction>&> &_)
                  {
                    ثابت MathGlyphConstruction &mgc = هذا+_.second;
                    mgc.depend (depend_معلومات, _.لومړی);
                  })
      ;
    }
  }

  فراغ تړون_glyphs (ثابت hb_سېټ_t *glyph_سېټ,
                       hb_سېټ_t *variant_glyphs) ثابت
  {
    ثابت hb_array_t<ثابت Offset16_منزل<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vert_Glyph_شمېر + horiz_Glyph_شمېر);

    إذا (vertGlyphCoverage)
    {
      ثابت خپلکاری vert_offsets = glyph_construction_offsets.sub_array (0, vert_Glyph_شمېر);
      + hb_zip (هذا+vertGlyphCoverage, vert_offsets)
      | hb_filter (glyph_سېټ, hb_لومړی)
      | hb_map (hb_دوهم)
      | hb_map (hb_جمع_کړه (هذا))
      | hb_apply ([=] (ثابت MathGlyphConstruction &_) { _.تړون_glyphs (variant_glyphs); })
      ;
    }

    إذا (horizGlyphCoverage)
    {
      ثابت خپلکاری hori_offsets = glyph_construction_offsets.sub_array (vert_Glyph_شمېر, horiz_Glyph_شمېر);
      + hb_zip (هذا+horizGlyphCoverage, hori_offsets)
      | hb_filter (glyph_سېټ, hb_لومړی)
      | hb_map (hb_دوهم)
      | hb_map (hb_جمع_کړه (هذا))
      | hb_apply ([=] (ثابت MathGlyphConstruction &_) { _.تړون_glyphs (variant_glyphs); })
      ;
    }
  }

  فراغ collect_coverage_and_indices (hb_sorted_ویکټور_t<hb_codepoint_t>& new_coverage,
                                     ثابت Offset16_منزل<Coverage>& coverage,
                                     بینښه i,
                                     بینښه پای_ټکی_شاخص,
                                     hb_سېټ_t& indices,
                                     ثابت hb_سېټ_t& glyphset,
                                     ثابت hb_map_t& glyph_map) ثابت
  {
    إذا (!coverage) أرجع;

    لكل (ثابت خپلکاری _ : (هذا+coverage).iter ())
    {
      إذا (i >= پای_ټکی_شاخص) أرجع;
      إذا (glyphset.has (_))
      {
        بینښه new_gid = glyph_map.get (_);
        new_coverage.push (new_gid);
        indices.جمع_کړه (i);
      }
      i++;
    }
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سېټ_t &glyphset = c->plan->_glyphset_cmaped;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);
    إذا (!c->serializer->check_ټاکل (بهر->minConnectorOverlap, minConnectorOverlap, HB_SERIALIZE_ERROR_INT_ډکېدنه))
      return_trace (خطأ);

    hb_sorted_ویکټور_t<hb_codepoint_t> new_vert_coverage;
    hb_sorted_ویکټور_t<hb_codepoint_t> new_hori_coverage;
    hb_سېټ_t indices;
    collect_coverage_and_indices (new_vert_coverage, vertGlyphCoverage, 0, vert_Glyph_شمېر, indices, glyphset, glyph_map);
    collect_coverage_and_indices (new_hori_coverage, horizGlyphCoverage, vert_Glyph_شمېر, vert_Glyph_شمېر + horiz_Glyph_شمېر, indices, glyphset, glyph_map);

    إذا (!c->serializer->check_ټاکل (بهر->vert_Glyph_شمېر, new_vert_coverage.اوږدتیا, HB_SERIALIZE_ERROR_INT_ډکېدنه))
      return_trace (خطأ);
    إذا (!c->serializer->check_ټاکل (بهر->horiz_Glyph_شمېر, new_hori_coverage.اوږدتیا, HB_SERIALIZE_ERROR_INT_ډکېدنه))
      return_trace (خطأ);

    لكل (بینښه i : indices.iter ())
    {
      خپلکاری *o = c->serializer->embed (glyphConstruction[i]);
      إذا (!o) return_trace (خطأ);
      o->serialize_subset (c, glyphConstruction[i], هذا);
    }

    إذا (new_vert_coverage)
      بهر->vertGlyphCoverage.serialize_serialize (c->serializer, new_vert_coverage.iter ());

    إذا (new_hori_coverage)
    بهر->horizGlyphCoverage.serialize_serialize (c->serializer, new_hori_coverage.iter ());
    return_trace (ریښتیا);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت خپلکاری &ثبت : hb_zip (هذا+vertGlyphCoverage, glyphConstruction.as_array (vert_Glyph_شمېر)))
      إذا (c->glyph_سېټ->has (hb_لومړی (ثبت)))
        (هذا+hb_دوهم (ثبت)).collect_variation_indices (c);
    لكل (ثابت خپلکاری &ثبت : hb_zip (هذا+horizGlyphCoverage, glyphConstruction.as_array (vert_Glyph_شمېر + horiz_Glyph_شمېر).sub_array (vert_Glyph_شمېر)))
      إذا (c->glyph_سېټ->has (hb_لومړی (ثبت)))
        (هذا+hb_دوهم (ثبت)).collect_variation_indices (c);
  }

  منطقي sanitize_offsets (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    بینښه صحيح شمېر = vert_Glyph_شمېر + horiz_Glyph_شمېر;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
      إذا (!glyphConstruction.arrayZ[i].sanitize (c, هذا)) return_trace (خطأ);
    return_trace (ریښتیا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  vertGlyphCoverage.sanitize (c, هذا) &&
		  horizGlyphCoverage.sanitize (c, هذا) &&
		  hb_barrier () &&
		  c->check_array (glyphConstruction.arrayZ, vert_Glyph_شمېر + horiz_Glyph_شمېر) &&
		  sanitize_offsets (c));
  }

  hb_position_t get_min_connector_overlap (hb_direction_t direction,
						  hb_font_t *font) ثابت
  { أرجع font->em_scale_dir (minConnectorOverlap, direction); }

  بینښه صحيح get_glyph_variants (hb_codepoint_t glyph,
				   hb_direction_t direction,
				   hb_font_t *font,
				   بینښه صحيح پیل_دننه_واټن,
				   بینښه صحيح *variants_شمېر, /* IN/OUT */
				   hb_ot_math_glyph_variant_t *variants /*بهر*/) ثابت
  { أرجع get_glyph_construction (glyph, direction, font)
	   .get_variants (direction, font, پیل_دننه_واټن, variants_شمېر, variants); }

  بینښه صحيح get_glyph_parts (hb_codepoint_t glyph,
				hb_direction_t direction,
				hb_font_t *font,
				بینښه صحيح پیل_دننه_واټن,
				بینښه صحيح *parts_شمېر, /* IN/OUT */
				hb_ot_math_glyph_part_t *parts /*بهر*/,
				hb_position_t *italics_correction /*بهر*/) ثابت
  { أرجع get_glyph_construction (glyph, direction, font)
	   .get_assembly ()
	   .get_parts (direction, font,
		       پیل_دننه_واټن, parts_شمېر, parts,
		       italics_correction); }

  خاص:
  ثابت MathGlyphConstruction &
  get_glyph_construction (hb_codepoint_t glyph,
			  hb_direction_t direction,
			  hb_font_t *font HB_ناکارول_شوی) ثابت
  {
    منطقي vertical = HB_DIRECTION_IS_VERTICAL (direction);
    بینښه صحيح شمېر = vertical ? vert_Glyph_شمېر : horiz_Glyph_شمېر;
    ثابت Offset16_منزل<Coverage> &coverage = vertical ? vertGlyphCoverage
						  : horizGlyphCoverage;

    بینښه صحيح شاخص_2 = (هذا+coverage).get_coverage (glyph);
    إذا (unlikely (شاخص_2 >= شمېر)) أرجع Null (MathGlyphConstruction);

    إذا (!vertical)
      شاخص_2 += vert_Glyph_شمېر;

    أرجع هذا+glyphConstruction[شاخص_2];
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
  HBUINT16	vert_Glyph_شمېر;	/* Number of glyphs for which
				 * information is provided for
				 * vertically growing variants. */
  HBUINT16	horiz_Glyph_شمېر;/* Number of glyphs for which
				 * information is provided for
				 * horizontally growing variants. */

  /* Array of offsets to MathGlyphConstruction tables - from the beginning of
     the MathVariants table, for shapes growing in vertical/horizontal
     direction. */
  Unsized_Array_د<Offset16_منزل<MathGlyphConstruction>>
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
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_MATH;

  منطقي has_معلومات () ثابت { أرجع نسخه.منزل_int (); }

  فراغ depend (hb_depend_معلومات_builder_t *depend_معلومات) ثابت
  {
    إذا (mathVariants)
      (هذا+mathVariants).depend (depend_معلومات);
  }

  فراغ تړون_glyphs (hb_سېټ_t *glyph_سېټ) ثابت
  {
    إذا (mathVariants)
    {
      hb_سېټ_t variant_glyphs;
      (هذا+mathVariants).تړون_glyphs (glyph_سېټ, &variant_glyphs);
      hb_سېټ_union (glyph_سېټ, &variant_glyphs);
    }
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->embed (*هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    بهر->mathConstants.serialize_copy (c->serializer, mathConstants, هذا, 0, hb_serialize_context_t::سر, &c->plan->layout_variation_idx_delta_map);
    بهر->mathGlyphInfo.serialize_subset (c, mathGlyphInfo, هذا);
    بهر->mathVariants.serialize_subset (c, mathVariants, هذا);
    return_trace (ریښتیا);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c, ثابت hb_سېټ_t *variant_glyph_سېټ) ثابت
  {
    (هذا+mathConstants).collect_variation_indices (c);
    (هذا+mathGlyphInfo).collect_variation_indices (c);
    خپلکاری *glyph_سېټ = c->glyph_سېټ;
    c->glyph_سېټ = variant_glyph_سېټ;
    (هذا+mathVariants).collect_variation_indices (c);
    c->glyph_سېټ = glyph_سېټ;
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
  HB_دننی منطقي is_bad_cambria (hb_font_t *font) ثابت
  {
#که_نه_تعریف_شوی HB_NO_MATH
    بدل HB_CODEPOINT_ENCODE3 (font->face->جدول.MATH.get_blob ()->اوږدتیا,
                                 (هذا+mathConstants).min_لوړوالی[1], // displayOperatorMinHeight
                                 (هذا+mathConstants).min_لوړوالی[0]) // delimitedSubFormulaMinHeight
    {
      /* sha1sum:ab4a4fe054d23061f3c039493d6f665cfda2ecf5  cambria.ttc
       * sha1sum:086855301bff644f9d8827b88491fcf73a6d4cb9  cambria.ttc
       * sha1sum:b1e5a3feaca2ea3dfcf79ccb377de749ecf60343  cambria.ttc */
      حالة HB_CODEPOINT_ENCODE3 (25722, 2500, 3000):
        أرجع ریښتیا;
    }
#د_که_پای
    أرجع خطأ;
  }

  hb_position_t get_constant (hb_ot_math_constant_t  constant,
			      hb_font_t		   *font) ثابت
  { أرجع (هذا+mathConstants).get_ارزښت (constant, font); }

  ثابت MathGlyphInfo &get_glyph_info () ثابت { أرجع هذا+mathGlyphInfo; }

  ثابت MathVariants &get_variants () ثابت    { أرجع هذا+mathVariants; }

  محمي:
  Fixed_نسخه<>نسخه;	/* Version of the MATH table
				 * initially set to 0x00010000u */
  Offset16_منزل<MathConstants>
		mathConstants;	/* MathConstants table */
  Offset16_منزل<MathGlyphInfo>
		mathGlyphInfo;	/* MathGlyphInfo table */
  Offset16_منزل<MathVariants>
		mathVariants;	/* MathVariants table */

  عام:
  DEFINE_اندازه_STATIC (10);
};

} /* namespace OT */


#د_که_پای /* HB_OT_MATH_TABLE_HH */

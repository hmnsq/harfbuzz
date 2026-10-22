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

#اگر_غیر_تعریف_شدہ HB_OT_MATH_جدول_HH
#تعریف_کرو HB_OT_MATH_جدول_HH

#شامل "hb-open-type.hh"
#شامل "hb-ot-layout-common.hh"
#شامل "hb-ot-layout-gdef-table.hh"
#شامل "hb-ot-math.h"
#شامل "hb-depend-data.hh"

نامخلا OT {


ساخت Math_قدر_ریکارڈ
{
  hb_position_t get_x_قدر (hb_font_t *font, ثابت خالی *بنیاد) ثابت
  { واپس hb_saturate_جوڑو (font->em_scale_x (value), (بنیاد+device_جدول).get_x_delta (font, get_var_store (font, بنیاد))); }
  hb_position_t get_y_قدر (hb_font_t *font, ثابت خالی *بنیاد) ثابت
  { واپس hb_saturate_جوڑو (font->em_scale_y (value), (بنیاد+device_جدول).get_y_delta (font, get_var_store (font, بنیاد))); }

  ثابت شے_Variation_Store &get_var_store (hb_font_t *font, ثابت خالی *بنیاد) ثابت
  {
#اگر !defined(HB_NO_VAR) && !defined(HB_NO_OT_LAYOUT)
    اگر ((بنیاد+device_جدول).is_variation_device ())
      واپس font->face->جدول.GDEF->جدول->get_var_store ();
#اگر_ختم
    واپس Null (شے_Variation_Store);
  }

  Math_قدر_ریکارڈ* copy (hb_serialize_context_t *c, ثابت خالی *بنیاد,
                         ثابت hb_hashmap_t<غیرعلامتی, hb_pair_t<غیرعلامتی, صحیح>> *variation_map) ثابت
  {
    TRACE_SERIALIZE (یہ);
    خودکار *برآمد = c->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (خالی_اشاریہ);
    hb_pair_t<غیرعلامتی, صحیح> *varidx_delta;
    اگر (variation_map->has ((بنیاد+device_جدول).get_variation_اشاریہ (), &varidx_delta) &&
        !c->check_تفویض (برآمد->value, (int64_t) value + hb_دوسرا (*varidx_delta),
                          HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز))
      return_trace (خالی_اشاریہ);
    برآمد->device_جدول.serialize_copy (c, device_جدول, بنیاد, 0, hb_serialize_context_t::سر, variation_map);

    return_trace (برآمد);
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c, ثابت خالی *بنیاد) ثابت
  { (بنیاد+device_جدول).collect_variation_indices (c); }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) && device_جدول.sanitize (c, بنیاد));
  }

  محفوظ:
  HBINT16		value;		/* The X or Y value in design units */
  Offset16_مقصد<Device>	device_جدول;	/* Offset to the device table - from the
					 * beginning of parent table.  May be NULL.
					 * Suggested format for device table is 1. */

  عوامی:
  DEFINE_سائز_STATIC (4);
};

ساخت MathConstants
{
  دوست ساخت MATH;

  MathConstants* copy (hb_serialize_context_t *c,
                      ثابت hb_hashmap_t<غیرعلامتی, hb_pair_t<غیرعلامتی, صحیح>> *variation_map) ثابت
  {
    TRACE_SERIALIZE (یہ);
    خودکار *برآمد = c->آغاز_embed (یہ);

    HBINT16 *p = c->allocate_سائز<HBINT16> (HBINT16::static_سائز * 2);
    اگر (unlikely (!p)) return_trace (خالی_اشاریہ);
    hb_memcpy (p, percent_Scale_نیچے_کو, HBINT16::static_سائز * 2);

    HBUINT16 *m = c->allocate_سائز<HBUINT16> (HBUINT16::static_سائز * 2);
    اگر (unlikely (!m)) return_trace (خالی_اشاریہ);
    hb_memcpy (m, min_اونچائی, HBUINT16::static_سائز * 2);

    غیرعلامتی گنتی = ARRAY_لمبائی (math_قدر_اندراجات);
    برائے (غیرعلامتی i = 0; i < گنتی; i++)
      اگر (!c->copy (math_قدر_اندراجات[i], یہ, variation_map))
        return_trace (خالی_اشاریہ);

    اگر (!c->embed (radicalDegreeBottomRaisePercent)) return_trace (خالی_اشاریہ);
    return_trace (برآمد);
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    برائے (ثابت خودکار &ریکارڈ : math_قدر_اندراجات)
      ریکارڈ.collect_variation_indices (c, یہ);
  }

  منطقی sanitize_math_قدر_اندراجات (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);

    غیرعلامتی صحیح گنتی = ARRAY_لمبائی (math_قدر_اندراجات);
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
      اگر (!math_قدر_اندراجات[i].sanitize (c, یہ))
	return_trace (جھوٹ);

    return_trace (سچ);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) && sanitize_math_قدر_اندراجات (c));
  }

  hb_position_t get_قدر (hb_ot_math_constant_t constant,
			   hb_font_t *font) ثابت
  {
    انتخاب (constant) {

    حالت HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_نیچے_کو:
    حالت HB_OT_MATH_CONSTANT_SCRIPT_SCRIPT_PERCENT_SCALE_نیچے_کو:
      واپس percent_Scale_نیچے_کو[constant - HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_نیچے_کو];

    حالت HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_اونچائی:
    حالت HB_OT_MATH_CONSTANT_DISPLAY_OPERATOR_MIN_اونچائی:
      واپس font->em_scale_y (min_اونچائی[constant - HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_اونچائی]);

    حالت HB_OT_MATH_CONSTANT_RADICAL_KERN_AFTER_DEGREE:
    حالت HB_OT_MATH_CONSTANT_RADICAL_KERN_BEFORE_DEGREE:
    حالت HB_OT_MATH_CONSTANT_SKEWED_FRACTION_HORIZONTAL_GAP:
    حالت HB_OT_MATH_CONSTANT_SPACE_AFTER_SCRIPT:
      واپس math_قدر_اندراجات[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_x_قدر (font, یہ);

    حالت HB_OT_MATH_CONSTANT_ACCENT_بنیاد_اونچائی:
    حالت HB_OT_MATH_CONSTANT_AXIS_اونچائی:
    حالت HB_OT_MATH_CONSTANT_FLATTENED_ACCENT_بنیاد_اونچائی:
    حالت HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_DISPLAY_STYLE_سرکاؤ_نیچے_کو:
    حالت HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_GAP_MIN:
    حالت HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_سرکاؤ_نیچے_کو:
    حالت HB_OT_MATH_CONSTANT_FRACTION_DENOM_DISPLAY_STYLE_GAP_MIN:
    حالت HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_DISPLAY_STYLE_سرکاؤ_اوپر:
    حالت HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_GAP_MIN:
    حالت HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_سرکاؤ_اوپر:
    حالت HB_OT_MATH_CONSTANT_FRACTION_عدد_DISPLAY_STYLE_GAP_MIN:
    حالت HB_OT_MATH_CONSTANT_FRACTION_قاعدہ_THICKNESS:
    حالت HB_OT_MATH_CONSTANT_زیریں_حد_بندی_BASELINE_DROP_MIN:
    حالت HB_OT_MATH_CONSTANT_زیریں_حد_بندی_GAP_MIN:
    حالت HB_OT_MATH_CONSTANT_MATH_LEADING:
    حالت HB_OT_MATH_CONSTANT_OVERBAR_EXTRA_ASCENDER:
    حالت HB_OT_MATH_CONSTANT_OVERBAR_قاعدہ_THICKNESS:
    حالت HB_OT_MATH_CONSTANT_OVERBAR_VERTICAL_GAP:
    حالت HB_OT_MATH_CONSTANT_RADICAL_DISPLAY_STYLE_VERTICAL_GAP:
    حالت HB_OT_MATH_CONSTANT_RADICAL_EXTRA_ASCENDER:
    حالت HB_OT_MATH_CONSTANT_RADICAL_قاعدہ_THICKNESS:
    حالت HB_OT_MATH_CONSTANT_RADICAL_VERTICAL_GAP:
    حالت HB_OT_MATH_CONSTANT_SKEWED_FRACTION_VERTICAL_GAP:
    حالت HB_OT_MATH_CONSTANT_ڈھیر_BOTTOM_DISPLAY_STYLE_سرکاؤ_نیچے_کو:
    حالت HB_OT_MATH_CONSTANT_ڈھیر_BOTTOM_سرکاؤ_نیچے_کو:
    حالت HB_OT_MATH_CONSTANT_ڈھیر_DISPLAY_STYLE_GAP_MIN:
    حالت HB_OT_MATH_CONSTANT_ڈھیر_GAP_MIN:
    حالت HB_OT_MATH_CONSTANT_ڈھیر_چوٹی_DISPLAY_STYLE_سرکاؤ_اوپر:
    حالت HB_OT_MATH_CONSTANT_ڈھیر_چوٹی_سرکاؤ_اوپر:
    حالت HB_OT_MATH_CONSTANT_STRETCH_ڈھیر_BOTTOM_سرکاؤ_نیچے_کو:
    حالت HB_OT_MATH_CONSTANT_STRETCH_ڈھیر_GAP_ABOVE_MIN:
    حالت HB_OT_MATH_CONSTANT_STRETCH_ڈھیر_GAP_نیچے_MIN:
    حالت HB_OT_MATH_CONSTANT_STRETCH_ڈھیر_چوٹی_سرکاؤ_اوپر:
    حالت HB_OT_MATH_CONSTANT_SUBSCRIPT_BASELINE_DROP_MIN:
    حالت HB_OT_MATH_CONSTANT_SUBSCRIPT_سرکاؤ_نیچے_کو:
    حالت HB_OT_MATH_CONSTANT_SUBSCRIPT_چوٹی_MAX:
    حالت HB_OT_MATH_CONSTANT_SUB_SUPERSCRIPT_GAP_MIN:
    حالت HB_OT_MATH_CONSTANT_SUPERSCRIPT_BASELINE_DROP_MAX:
    حالت HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MAX_WITH_SUBSCRIPT:
    حالت HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MIN:
    حالت HB_OT_MATH_CONSTANT_SUPERSCRIPT_سرکاؤ_اوپر:
    حالت HB_OT_MATH_CONSTANT_SUPERSCRIPT_سرکاؤ_اوپر_CRAMPED:
    حالت HB_OT_MATH_CONSTANT_UNDERBAR_EXTRA_DESCENDER:
    حالت HB_OT_MATH_CONSTANT_UNDERBAR_قاعدہ_THICKNESS:
    حالت HB_OT_MATH_CONSTANT_UNDERBAR_VERTICAL_GAP:
    حالت HB_OT_MATH_CONSTANT_بالائی_حد_بندی_BASELINE_RISE_MIN:
    حالت HB_OT_MATH_CONSTANT_بالائی_حد_بندی_GAP_MIN:
      واپس math_قدر_اندراجات[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_y_قدر (font, یہ);

    حالت HB_OT_MATH_CONSTANT_RADICAL_DEGREE_BOTTOM_RAISE_PERCENT:
      واپس radicalDegreeBottomRaisePercent;

    طےشدہ:
      واپس 0;
    }
  }

  محفوظ:
  HBINT16 percent_Scale_نیچے_کو[2];
  HBUINT16 min_اونچائی[2];
  Math_قدر_ریکارڈ math_قدر_اندراجات[51];
  HBINT16 radicalDegreeBottomRaisePercent;

  عوامی:
  DEFINE_سائز_STATIC (214);
};

ساخت MathItalicsCorrectionInfo
{
  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    ثابت hb_سیٹ_t &glyphset = c->plan->_glyphset_mathed;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);

    hb_sorted_سمتیہ_t<hb_codepoint_t> new_coverage;
    + hb_zip (یہ+coverage, italicsCorrection)
    | hb_filter (glyphset, hb_پہلا)
    | hb_filter (serialize_math_ریکارڈ_array (c->serializer, برآمد->italicsCorrection, یہ, &c->plan->layout_variation_idx_delta_map), hb_دوسرا)
    | hb_map (hb_پہلا)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    برآمد->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (سچ);
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    برائے (ثابت خودکار &اندراج_2 : hb_zip (یہ+coverage, italicsCorrection))
      اگر (c->glyph_سیٹ->has (hb_پہلا (اندراج_2)))
        hb_دوسرا (اندراج_2).collect_variation_indices (c, یہ);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  coverage.sanitize (c, یہ) &&
		  italicsCorrection.sanitize (c, یہ));
  }

  hb_position_t get_قدر (hb_codepoint_t glyph,
			   hb_font_t *font) ثابت
  {
    غیرعلامتی صحیح اشاریہ_2 = (یہ+coverage).get_coverage (glyph);
    واپس italicsCorrection[اشاریہ_2].get_x_قدر (font, یہ);
  }

  محفوظ:
  Offset16_مقصد<Coverage>       coverage;		/* Offset to Coverage table -
						 * from the beginning of
						 * MathItalicsCorrectionInfo
						 * table. */
  Array16_کا<Math_قدر_ریکارڈ> italicsCorrection;	/* Array of MathValueRecords
						 * defining italics correction
						 * values for each
						 * covered glyph. */

  عوامی:
  DEFINE_سائز_ARRAY (4, italicsCorrection);
};

ساخت Math_چوٹی_Accent_Attachment
{
  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    ثابت hb_سیٹ_t &glyphset = c->plan->_glyphset_mathed;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);

    hb_sorted_سمتیہ_t<hb_codepoint_t> new_coverage;
    + hb_zip (یہ+چوٹی_Accent_Coverage, چوٹی_Accent_Attachment)
    | hb_filter (glyphset, hb_پہلا)
    | hb_filter (serialize_math_ریکارڈ_array (c->serializer, برآمد->چوٹی_Accent_Attachment, یہ, &c->plan->layout_variation_idx_delta_map), hb_دوسرا)
    | hb_map (hb_پہلا)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    برآمد->چوٹی_Accent_Coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (سچ);
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    برائے (ثابت خودکار &اندراج_2 : hb_zip (یہ+چوٹی_Accent_Coverage, چوٹی_Accent_Attachment))
      اگر (c->glyph_سیٹ->has (hb_پہلا (اندراج_2)))
        hb_دوسرا (اندراج_2).collect_variation_indices (c, یہ);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  چوٹی_Accent_Coverage.sanitize (c, یہ) &&
		  چوٹی_Accent_Attachment.sanitize (c, یہ));
  }

  hb_position_t get_قدر (hb_codepoint_t glyph,
			   hb_font_t *font) ثابت
  {
    غیرعلامتی صحیح اشاریہ_2 = (یہ+چوٹی_Accent_Coverage).get_coverage (glyph);
    اگر (اشاریہ_2 == NOT_COVERED)
      واپس font->get_glyph_h_advance (glyph) / 2;
    واپس چوٹی_Accent_Attachment[اشاریہ_2].get_x_قدر (font, یہ);
  }

  محفوظ:
  Offset16_مقصد<Coverage>       چوٹی_Accent_Coverage;   /* Offset to Coverage table -
						 * from the beginning of
						 * MathTopAccentAttachment
						 * table. */
  Array16_کا<Math_قدر_ریکارڈ> چوٹی_Accent_Attachment; /* Array of MathValueRecords
						 * defining top accent
						 * attachment points for each
						 * covered glyph. */

  عوامی:
  DEFINE_سائز_ARRAY (2 + 2, چوٹی_Accent_Attachment);
};

ساخت MathKern
{
  MathKern* copy (hb_serialize_context_t *c,
                      ثابت hb_hashmap_t<غیرعلامتی, hb_pair_t<غیرعلامتی, صحیح>> *variation_map) ثابت
  {
    TRACE_SERIALIZE (یہ);
    خودکار *برآمد = c->آغاز_embed (یہ);

    اگر (unlikely (!c->embed (اونچائی_گنتی))) return_trace (خالی_اشاریہ);

    غیرعلامتی گنتی = 2 * اونچائی_گنتی + 1;
    برائے (غیرعلامتی i = 0; i < گنتی; i++)
      اگر (!c->copy (math_قدر_اندراجات_Z.arrayZ[i], یہ, variation_map))
        return_trace (خالی_اشاریہ);

    return_trace (برآمد);
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    برائے (ثابت خودکار &ریکارڈ : math_قدر_اندراجات_Z.as_array (2 * اونچائی_گنتی + 1))
      ریکارڈ.collect_variation_indices (c, یہ);
  }

  منطقی sanitize_math_قدر_اندراجات (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    غیرعلامتی صحیح گنتی = 2 * اونچائی_گنتی + 1;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
      اگر (!math_قدر_اندراجات_Z.arrayZ[i].sanitize (c, یہ)) return_trace (جھوٹ);
    return_trace (سچ);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  hb_barrier () &&
		  c->check_array (math_قدر_اندراجات_Z.arrayZ, 2 * اونچائی_گنتی + 1) &&
		  sanitize_math_قدر_اندراجات (c));
  }

  hb_position_t get_قدر (hb_position_t correction_اونچائی, hb_font_t *font) ثابت
  {
    ثابت Math_قدر_ریکارڈ* correction_اونچائی_2 = math_قدر_اندراجات_Z.arrayZ;
    ثابت Math_قدر_ریکارڈ* kern_قدر = math_قدر_اندراجات_Z.arrayZ + اونچائی_گنتی;
    صحیح sign = font->y_scale < 0 ? -1 : +1;

    /* According to OpenType spec (v1.9), except for the boundary cases, the index
     * chosen for kern value should be i such that
     *    correctionHeight[i-1] <= correction_height < correctionHeight[i]
     * We can use the binary search algorithm of std::upper_bound(). Or, we can
     * use the internal hb_bsearch_impl.
     */
    غیرعلامتی صحیح pos;
    خودکار cmp = +[](ثابت خالی* key, ثابت خالی* p,
                   صحیح sign, hb_font_t* font, ثابت MathKern* mathKern) -> صحیح {
      hb_position_t a = *(hb_position_t *) key;
      hb_position_t b = ((Math_قدر_ریکارڈ *) p)->get_y_قدر (font, mathKern);
      اگر (a == b) واپس 0;
      واپس sign > 0 ? (a < b ? -1 : +1) : (b < a ? -1 : +1);
    };
    غیرعلامتی صحیح i = hb_bsearch_impl(&pos, correction_اونچائی, correction_اونچائی_2,
                                     اونچائی_گنتی, Math_قدر_ریکارڈ::static_سائز,
                                     cmp, sign, font, یہ) ? pos + 1 : pos;
    واپس kern_قدر[i].get_x_قدر (font, یہ);
  }

  غیرعلامتی صحیح get_entries (غیرعلامتی صحیح آغاز_انحراف,
			    غیرعلامتی صحیح *entries_گنتی, /* IN/OUT */
			    hb_ot_math_kern_اندراج_t *kern_entries, /*برآمد*/
			    hb_font_t *font) ثابت
  {
    ثابت Math_قدر_ریکارڈ* correction_اونچائی_2 = math_قدر_اندراجات_Z.arrayZ;
    ثابت Math_قدر_ریکارڈ* kern_قدر = math_قدر_اندراجات_Z.arrayZ + اونچائی_گنتی;
    ثابت غیرعلامتی صحیح entries_گنتی_2 = اونچائی_گنتی + 1;

    اگر (entries_گنتی && kern_entries)
    {
      غیرعلامتی صحیح آغاز = hb_min (آغاز_انحراف, entries_گنتی_2);
      غیرعلامتی صحیح انتہا = hb_min (آغاز + *entries_گنتی, entries_گنتی_2);
      *entries_گنتی = انتہا - آغاز;

      برائے (غیرعلامتی صحیح i = 0; i < *entries_گنتی; i++) {
	غیرعلامتی صحیح j = آغاز + i;

	hb_position_t max_اونچائی;
	اگر (j == اونچائی_گنتی) {
	  max_اونچائی = INT32_MAX;
	} ورنہ {
	  max_اونچائی = correction_اونچائی_2[j].get_y_قدر (font, یہ);
	}

	kern_entries[i] = {max_اونچائی, kern_قدر[j].get_x_قدر (font, یہ)};
      }
    }
    واپس entries_گنتی_2;
  }

  محفوظ:
  HBUINT16	اونچائی_گنتی;
  Unsized_Array_کا<Math_قدر_ریکارڈ>
		math_قدر_اندراجات_Z;
				/* Array of correction heights at
				 * which the kern value changes.
				 * Sorted by the height value in
				 * design units (heightCount entries),
				 * Followed by:
				 * Array of kern values corresponding
				 * to heights. (heightCount+1 entries).
				 */

  عوامی:
  DEFINE_سائز_ARRAY (2, math_قدر_اندراجات_Z);
};

ساخت Math_Kern_Info_ریکارڈ
{
  Math_Kern_Info_ریکارڈ* copy (hb_serialize_context_t *c, ثابت خالی *بنیاد,
                            ثابت hb_hashmap_t<غیرعلامتی, hb_pair_t<غیرعلامتی, صحیح>> *variation_map) ثابت
  {
    TRACE_SERIALIZE (یہ);
    خودکار *برآمد = c->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (خالی_اشاریہ);

    غیرعلامتی گنتی = ARRAY_لمبائی (mathKern);
    برائے (غیرعلامتی i = 0; i < گنتی; i++)
      برآمد->mathKern[i].serialize_copy (c, mathKern[i], بنیاد, 0, hb_serialize_context_t::سر, variation_map);

    return_trace (برآمد);
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c, ثابت خالی *بنیاد) ثابت
  {
    برائے (ثابت خودکار &انحراف : mathKern)
      (بنیاد+انحراف).collect_variation_indices (c);
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);

    غیرعلامتی صحیح گنتی = ARRAY_لمبائی (mathKern);
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
      اگر (unlikely (!mathKern[i].sanitize (c, بنیاد)))
	return_trace (جھوٹ);

    return_trace (سچ);
  }

  hb_position_t get_kerning (hb_ot_math_kern_t kern,
			     hb_position_t correction_اونچائی,
			     hb_font_t *font,
			     ثابت خالی *بنیاد) ثابت
  {
    غیرعلامتی صحیح idx = kern;
    اگر (unlikely (idx >= ARRAY_لمبائی (mathKern))) واپس 0;
    واپس (بنیاد+mathKern[idx]).get_قدر (correction_اونچائی, font);
  }

  غیرعلامتی صحیح get_kernings (hb_ot_math_kern_t kern,
			     غیرعلامتی صحیح آغاز_انحراف,
			     غیرعلامتی صحیح *entries_گنتی, /* IN/OUT */
			     hb_ot_math_kern_اندراج_t *kern_entries, /*برآمد*/
			     hb_font_t *font,
			     ثابت خالی *بنیاد) ثابت
  {
    غیرعلامتی صحیح idx = kern;
    اگر (unlikely (idx >= ARRAY_لمبائی (mathKern)) || !mathKern[idx]) {
      اگر (entries_گنتی) *entries_گنتی = 0;
      واپس 0;
    }
    واپس (بنیاد+mathKern[idx]).get_entries (آغاز_انحراف,
					     entries_گنتی,
					     kern_entries,
					     font);
  }

  محفوظ:
  /* Offset to MathKern table for each corner -
   * from the beginning of MathKernInfo table.  May be NULL. */
  Offset16_مقصد<MathKern> mathKern[4];

  عوامی:
  DEFINE_سائز_STATIC (8);
};

ساخت MathKernInfo
{
  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    ثابت hb_سیٹ_t &glyphset = c->plan->_glyphset_mathed;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);

    hb_sorted_سمتیہ_t<hb_codepoint_t> new_coverage;
    + hb_zip (یہ+mathKernCoverage, math_Kern_Info_اندراجات)
    | hb_filter (glyphset, hb_پہلا)
    | hb_filter (serialize_math_ریکارڈ_array (c->serializer, برآمد->math_Kern_Info_اندراجات, یہ, &c->plan->layout_variation_idx_delta_map), hb_دوسرا)
    | hb_map (hb_پہلا)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    برآمد->mathKernCoverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (سچ);
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    برائے (ثابت خودکار &اندراج_2 : hb_zip (یہ+mathKernCoverage, math_Kern_Info_اندراجات))
      اگر (c->glyph_سیٹ->has (hb_پہلا (اندراج_2)))
        hb_دوسرا (اندراج_2).collect_variation_indices (c, یہ);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  mathKernCoverage.sanitize (c, یہ) &&
		  math_Kern_Info_اندراجات.sanitize (c, یہ));
  }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_اونچائی,
			     hb_font_t *font) ثابت
  {
    غیرعلامتی صحیح اشاریہ_2 = (یہ+mathKernCoverage).get_coverage (glyph);
    واپس math_Kern_Info_اندراجات[اشاریہ_2].get_kerning (kern, correction_اونچائی, font, یہ);
  }

  غیرعلامتی صحیح get_kernings (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     غیرعلامتی صحیح آغاز_انحراف,
			     غیرعلامتی صحیح *entries_گنتی, /* IN/OUT */
			     hb_ot_math_kern_اندراج_t *kern_entries, /*برآمد*/
			     hb_font_t *font) ثابت
  {
    غیرعلامتی صحیح اشاریہ_2 = (یہ+mathKernCoverage).get_coverage (glyph);
    واپس math_Kern_Info_اندراجات[اشاریہ_2].get_kernings (kern,
						    آغاز_انحراف,
						    entries_گنتی,
						    kern_entries,
						    font,
						    یہ);
  }

  محفوظ:
  Offset16_مقصد<Coverage>
		mathKernCoverage;
				/* Offset to Coverage table -
				 * from the beginning of the
				 * MathKernInfo table. */
  Array16_کا<Math_Kern_Info_ریکارڈ>
		math_Kern_Info_اندراجات;
				/* Array of MathKernInfoRecords,
				 * per-glyph information for
				 * mathematical positioning
				 * of subscripts and
				 * superscripts. */

  عوامی:
  DEFINE_سائز_ARRAY (4, math_Kern_Info_اندراجات);
};

ساخت MathGlyphInfo
{
  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->embed (*یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    برآمد->mathItalicsCorrectionInfo.serialize_subset (c, mathItalicsCorrectionInfo, یہ);
    برآمد->math_چوٹی_Accent_Attachment.serialize_subset (c, math_چوٹی_Accent_Attachment, یہ);

    ثابت hb_سیٹ_t &glyphset = c->plan->_glyphset_mathed;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار it =
    + hb_iter (یہ+extendedShapeCoverage)
    | hb_take (c->plan->ماخذ->get_عدد_glyphs ())
    | hb_filter (glyphset)
    | hb_map_retains_ترتیب_کاری (glyph_map)
    ;

    اگر (it) برآمد->extendedShapeCoverage.serialize_serialize (c->serializer, it);
    ورنہ برآمد->extendedShapeCoverage = 0;

    برآمد->mathKernInfo.serialize_subset (c, mathKernInfo, یہ);
    return_trace (سچ);
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    (یہ+mathItalicsCorrectionInfo).collect_variation_indices (c);
    (یہ+math_چوٹی_Accent_Attachment).collect_variation_indices (c);
    (یہ+mathKernInfo).collect_variation_indices (c);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  mathItalicsCorrectionInfo.sanitize (c, یہ) &&
		  math_چوٹی_Accent_Attachment.sanitize (c, یہ) &&
		  extendedShapeCoverage.sanitize (c, یہ) &&
		  mathKernInfo.sanitize (c, یہ));
  }

  hb_position_t
  get_italics_correction (hb_codepoint_t  glyph, hb_font_t *font) ثابت
  { واپس (یہ+mathItalicsCorrectionInfo).get_قدر (glyph, font); }

  hb_position_t
  get_چوٹی_accent_attachment (hb_codepoint_t  glyph, hb_font_t *font) ثابت
  { واپس (یہ+math_چوٹی_Accent_Attachment).get_قدر (glyph, font); }

  منطقی is_extended_shape (hb_codepoint_t glyph) ثابت
  { واپس (یہ+extendedShapeCoverage).get_coverage (glyph) != NOT_COVERED; }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_اونچائی,
			     hb_font_t *font) ثابت
  { واپس (یہ+mathKernInfo).get_kerning (glyph, kern, correction_اونچائی, font); }

  hb_position_t get_kernings (hb_codepoint_t glyph,
			      hb_ot_math_kern_t kern,
			      غیرعلامتی صحیح آغاز_انحراف,
			      غیرعلامتی صحیح *entries_گنتی, /* IN/OUT */
			      hb_ot_math_kern_اندراج_t *kern_entries, /*برآمد*/
			      hb_font_t *font) ثابت
  { واپس (یہ+mathKernInfo).get_kernings (glyph,
					     kern,
					     آغاز_انحراف,
					     entries_گنتی,
					     kern_entries,
					     font); }

  محفوظ:
  /* Offset to MathItalicsCorrectionInfo table -
   * from the beginning of MathGlyphInfo table. */
  Offset16_مقصد<MathItalicsCorrectionInfo> mathItalicsCorrectionInfo;

  /* Offset to MathTopAccentAttachment table -
   * from the beginning of MathGlyphInfo table. */
  Offset16_مقصد<Math_چوٹی_Accent_Attachment> math_چوٹی_Accent_Attachment;

  /* Offset to coverage table for Extended Shape glyphs -
   * from the beginning of MathGlyphInfo table. When the left or right glyph of
   * a box is an extended shape variant, the (ink) box (and not the default
   * position defined by values in MathConstants table) should be used for
   * vertical positioning purposes.  May be NULL.. */
  Offset16_مقصد<Coverage> extendedShapeCoverage;

   /* Offset to MathKernInfo table -
    * from the beginning of MathGlyphInfo table. */
  Offset16_مقصد<MathKernInfo> mathKernInfo;

  عوامی:
  DEFINE_سائز_STATIC (8);
};

ساخت Math_Glyph_Variant_ریکارڈ
{
  دوست ساخت MathGlyphConstruction;

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    ثابت hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_تفویض (برآمد->variantGlyph, glyph_map.get (variantGlyph), HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  خالی depend (hb_depend_ڈیٹا_builder_t *depend_ڈیٹا, غیرعلامتی ماخذ) ثابت
  { depend_ڈیٹا->جوڑو_depend(ماخذ, HB_OT_لیبل_MATH, variantGlyph); }

  خالی احاطہ_glyphs (hb_سیٹ_t *variant_glyphs) ثابت
  { variant_glyphs->جوڑو (variantGlyph); }

  محفوظ:
  HBGlyphID16 variantGlyph;       /* Glyph ID for the variant. */
  HBUINT16  advanceMeasurement; /* Advance width/height, in design units, of the
				 * variant, in the direction of requested
				 * glyph extension. */

  عوامی:
  DEFINE_سائز_STATIC (4);
};

ساخت PartFlags : HBUINT16
{
  شمار Flags {
    Extender	= 0x0001u, /* If set, the part can be skipped or repeated. */

    Defined	= 0x0001u, /* All defined flags. */
  };

  عوامی:
  DEFINE_سائز_STATIC (2);
};

ساخت Math_Glyph_Part_ریکارڈ
{
  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    ثابت hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_تفویض (برآمد->glyph, glyph_map.get (glyph), HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  خالی extract (hb_ot_math_glyph_part_t &برآمد,
		int64_t mult,
		hb_font_t *font) ثابت
  {
    برآمد.glyph			= glyph;

    برآمد.آغاز_connector_لمبائی	= font->em_mult (آغاز_Connector_لمبائی, mult);
    برآمد.انتہا_connector_لمبائی	= font->em_mult (انتہا_Connector_لمبائی, mult);
    برآمد.بھرا_advance		= font->em_mult (بھرا_Advance, mult);

    static_assert ((غیرعلامتی صحیح) HB_OT_MATH_GLYPH_PART_FLAG_EXTENDER ==
		   (غیرعلامتی صحیح) PartFlags::Extender, "");

    برآمد.flags = (hb_ot_math_glyph_part_flags_t)
		(غیرعلامتی صحیح)
		(partFlags & PartFlags::Defined);
  }

  خالی depend (hb_depend_ڈیٹا_builder_t *depend_ڈیٹا, غیرعلامتی ماخذ) ثابت
  { depend_ڈیٹا->جوڑو_depend(ماخذ, HB_OT_لیبل_MATH, glyph); }

  خالی احاطہ_glyphs (hb_سیٹ_t *variant_glyphs) ثابت
  { variant_glyphs->جوڑو (glyph); }

  محفوظ:
  HBGlyphID16	glyph;		/* Glyph ID for the part. */
  HBUINT16	آغاز_Connector_لمبائی;
				/* Advance width/ height of the straight bar
				 * connector material, in design units, is at
				 * the beginning of the glyph, in the
				 * direction of the extension. */
  HBUINT16	انتہا_Connector_لمبائی;
				/* Advance width/ height of the straight bar
				 * connector material, in design units, is at
				 * the end of the glyph, in the direction of
				 * the extension. */
  HBUINT16	بھرا_Advance;	/* Full advance width/height for this part,
				 * in the direction of the extension.
				 * In design units. */
  PartFlags	partFlags;	/* Part qualifiers. */

  عوامی:
  DEFINE_سائز_STATIC (10);
};

ساخت MathGlyphAssembly
{
  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);

    اگر (!c->serializer->copy (italicsCorrection, یہ, &c->plan->layout_variation_idx_delta_map)) return_trace (جھوٹ);
    اگر (!c->serializer->copy<HBUINT16> (part_اندراجات.len)) return_trace (جھوٹ);

    برائے (ثابت خودکار& ریکارڈ : part_اندراجات.iter ())
      اگر (!ریکارڈ.subset (c)) return_trace (جھوٹ);
    return_trace (سچ);
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    italicsCorrection.collect_variation_indices (c, یہ);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  italicsCorrection.sanitize (c, یہ) &&
		  part_اندراجات.sanitize (c));
  }

  غیرعلامتی صحیح get_parts (hb_direction_t direction,
			  hb_font_t *font,
			  غیرعلامتی صحیح آغاز_انحراف,
			  غیرعلامتی صحیح *parts_گنتی, /* IN/OUT */
			  hb_ot_math_glyph_part_t *parts /*برآمد*/,
			  hb_position_t *italics_correction /*برآمد*/) ثابت
  {
    اگر (parts_گنتی && parts)
    {
      int64_t mult = font->dir_mult (direction);
      برائے (خودکار _ : hb_zip (part_اندراجات.as_array ().sub_array (آغاز_انحراف, parts_گنتی),
			    hb_array (parts, *parts_گنتی)))
	_.پہلا.extract (_.second, mult, font);
    }

    اگر (italics_correction)
      *italics_correction = italicsCorrection.get_x_قدر (font, یہ);

    واپس part_اندراجات.len;
  }

  خالی depend (hb_depend_ڈیٹا_builder_t *depend_ڈیٹا, غیرعلامتی ماخذ) ثابت
  {
    برائے (ثابت خودکار& _ : part_اندراجات.iter ())
      _.depend (depend_ڈیٹا, ماخذ);
  }

  خالی احاطہ_glyphs (hb_سیٹ_t *variant_glyphs) ثابت
  {
    برائے (ثابت خودکار& _ : part_اندراجات.iter ())
      _.احاطہ_glyphs (variant_glyphs);
  }

  محفوظ:
  Math_قدر_ریکارڈ
		italicsCorrection;
				/* Italics correction of this
				 * MathGlyphAssembly. Should not
				 * depend on the assembly size. */
  Array16_کا<Math_Glyph_Part_ریکارڈ>
		part_اندراجات;	/* Array of part records, from
				 * left to right and bottom to
				 * top. */

  عوامی:
  DEFINE_سائز_ARRAY (6, part_اندراجات);
};

ساخت MathGlyphConstruction
{
  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);

    برآمد->glyphAssembly.serialize_subset (c, glyphAssembly, یہ);

    اگر (!c->serializer->check_تفویض (برآمد->math_Glyph_Variant_ریکارڈ.len, math_Glyph_Variant_ریکارڈ.len, HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز))
      return_trace (جھوٹ);
    برائے (ثابت خودکار& ریکارڈ : math_Glyph_Variant_ریکارڈ.iter ())
      اگر (!ریکارڈ.subset (c)) return_trace (جھوٹ);

    return_trace (سچ);
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    (یہ+glyphAssembly).collect_variation_indices (c);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  glyphAssembly.sanitize (c, یہ) &&
		  math_Glyph_Variant_ریکارڈ.sanitize (c));
  }

  ثابت MathGlyphAssembly &get_assembly () ثابت { واپس یہ+glyphAssembly; }

  غیرعلامتی صحیح get_variants (hb_direction_t direction,
			     hb_font_t *font,
			     غیرعلامتی صحیح آغاز_انحراف,
			     غیرعلامتی صحیح *variants_گنتی, /* IN/OUT */
			     hb_ot_math_glyph_variant_t *variants /*برآمد*/) ثابت
  {
    اگر (variants_گنتی && variants)
    {
      int64_t mult = font->dir_mult (direction);
      برائے (خودکار _ : hb_zip (math_Glyph_Variant_ریکارڈ.as_array ().sub_array (آغاز_انحراف, variants_گنتی),
			    hb_array (variants, *variants_گنتی)))
	_.second = {_.پہلا.variantGlyph, font->em_mult (_.پہلا.advanceMeasurement, mult)};
    }
    واپس math_Glyph_Variant_ریکارڈ.len;
  }

  خالی depend (hb_depend_ڈیٹا_builder_t *depend_ڈیٹا, غیرعلامتی ماخذ) ثابت
  {
    (یہ+glyphAssembly).depend (depend_ڈیٹا, ماخذ);

    برائے (ثابت خودکار& _ : math_Glyph_Variant_ریکارڈ.iter ())
      _.depend (depend_ڈیٹا, ماخذ);
  }

  خالی احاطہ_glyphs (hb_سیٹ_t *variant_glyphs) ثابت
  {
    (یہ+glyphAssembly).احاطہ_glyphs (variant_glyphs);

    برائے (ثابت خودکار& _ : math_Glyph_Variant_ریکارڈ.iter ())
      _.احاطہ_glyphs (variant_glyphs);
  }

  محفوظ:
  /* Offset to MathGlyphAssembly table for this shape - from the beginning of
     MathGlyphConstruction table.  May be NULL. */
  Offset16_مقصد<MathGlyphAssembly>	  glyphAssembly;

  /* MathGlyphVariantRecords for alternative variants of the glyphs. */
  Array16_کا<Math_Glyph_Variant_ریکارڈ> math_Glyph_Variant_ریکارڈ;

  عوامی:
  DEFINE_سائز_ARRAY (4, math_Glyph_Variant_ریکارڈ);
};

ساخت MathVariants
{
  خالی depend (hb_depend_ڈیٹا_builder_t *depend_ڈیٹا) ثابت
  {
    ثابت hb_array_t<ثابت Offset16_مقصد<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vert_Glyph_گنتی + horiz_Glyph_گنتی);

    اگر (vertGlyphCoverage)
    {
      ثابت خودکار vert_offsets = glyph_construction_offsets.sub_array (0, vert_Glyph_گنتی);
      + hb_zip (یہ+vertGlyphCoverage, vert_offsets)
      | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, ثابت Offset16_مقصد<MathGlyphConstruction>&> &_)
                  {
                    ثابت MathGlyphConstruction &mgc = یہ+_.second;
                    mgc.depend (depend_ڈیٹا, _.پہلا);
                  })
      ;
    }
    اگر (horizGlyphCoverage)
    {
      ثابت خودکار hori_offsets = glyph_construction_offsets.sub_array (vert_Glyph_گنتی, horiz_Glyph_گنتی);
      + hb_zip (یہ+horizGlyphCoverage, hori_offsets)
      | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, ثابت Offset16_مقصد<MathGlyphConstruction>&> &_)
                  {
                    ثابت MathGlyphConstruction &mgc = یہ+_.second;
                    mgc.depend (depend_ڈیٹا, _.پہلا);
                  })
      ;
    }
  }

  خالی احاطہ_glyphs (ثابت hb_سیٹ_t *glyph_سیٹ,
                       hb_سیٹ_t *variant_glyphs) ثابت
  {
    ثابت hb_array_t<ثابت Offset16_مقصد<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vert_Glyph_گنتی + horiz_Glyph_گنتی);

    اگر (vertGlyphCoverage)
    {
      ثابت خودکار vert_offsets = glyph_construction_offsets.sub_array (0, vert_Glyph_گنتی);
      + hb_zip (یہ+vertGlyphCoverage, vert_offsets)
      | hb_filter (glyph_سیٹ, hb_پہلا)
      | hb_map (hb_دوسرا)
      | hb_map (hb_جوڑو (یہ))
      | hb_apply ([=] (ثابت MathGlyphConstruction &_) { _.احاطہ_glyphs (variant_glyphs); })
      ;
    }

    اگر (horizGlyphCoverage)
    {
      ثابت خودکار hori_offsets = glyph_construction_offsets.sub_array (vert_Glyph_گنتی, horiz_Glyph_گنتی);
      + hb_zip (یہ+horizGlyphCoverage, hori_offsets)
      | hb_filter (glyph_سیٹ, hb_پہلا)
      | hb_map (hb_دوسرا)
      | hb_map (hb_جوڑو (یہ))
      | hb_apply ([=] (ثابت MathGlyphConstruction &_) { _.احاطہ_glyphs (variant_glyphs); })
      ;
    }
  }

  خالی collect_coverage_and_indices (hb_sorted_سمتیہ_t<hb_codepoint_t>& new_coverage,
                                     ثابت Offset16_مقصد<Coverage>& coverage,
                                     غیرعلامتی i,
                                     غیرعلامتی انتہا_اشاریہ,
                                     hb_سیٹ_t& indices,
                                     ثابت hb_سیٹ_t& glyphset,
                                     ثابت hb_map_t& glyph_map) ثابت
  {
    اگر (!coverage) واپس;

    برائے (ثابت خودکار _ : (یہ+coverage).iter ())
    {
      اگر (i >= انتہا_اشاریہ) واپس;
      اگر (glyphset.has (_))
      {
        غیرعلامتی new_gid = glyph_map.get (_);
        new_coverage.push (new_gid);
        indices.جوڑو (i);
      }
      i++;
    }
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    ثابت hb_سیٹ_t &glyphset = c->plan->_glyphset_cmaped;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);
    اگر (!c->serializer->check_تفویض (برآمد->minConnectorOverlap, minConnectorOverlap, HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز))
      return_trace (جھوٹ);

    hb_sorted_سمتیہ_t<hb_codepoint_t> new_vert_coverage;
    hb_sorted_سمتیہ_t<hb_codepoint_t> new_hori_coverage;
    hb_سیٹ_t indices;
    collect_coverage_and_indices (new_vert_coverage, vertGlyphCoverage, 0, vert_Glyph_گنتی, indices, glyphset, glyph_map);
    collect_coverage_and_indices (new_hori_coverage, horizGlyphCoverage, vert_Glyph_گنتی, vert_Glyph_گنتی + horiz_Glyph_گنتی, indices, glyphset, glyph_map);

    اگر (!c->serializer->check_تفویض (برآمد->vert_Glyph_گنتی, new_vert_coverage.لمبائی, HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز))
      return_trace (جھوٹ);
    اگر (!c->serializer->check_تفویض (برآمد->horiz_Glyph_گنتی, new_hori_coverage.لمبائی, HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز))
      return_trace (جھوٹ);

    برائے (غیرعلامتی i : indices.iter ())
    {
      خودکار *o = c->serializer->embed (glyphConstruction[i]);
      اگر (!o) return_trace (جھوٹ);
      o->serialize_subset (c, glyphConstruction[i], یہ);
    }

    اگر (new_vert_coverage)
      برآمد->vertGlyphCoverage.serialize_serialize (c->serializer, new_vert_coverage.iter ());

    اگر (new_hori_coverage)
    برآمد->horizGlyphCoverage.serialize_serialize (c->serializer, new_hori_coverage.iter ());
    return_trace (سچ);
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    برائے (ثابت خودکار &اندراج_2 : hb_zip (یہ+vertGlyphCoverage, glyphConstruction.as_array (vert_Glyph_گنتی)))
      اگر (c->glyph_سیٹ->has (hb_پہلا (اندراج_2)))
        (یہ+hb_دوسرا (اندراج_2)).collect_variation_indices (c);
    برائے (ثابت خودکار &اندراج_2 : hb_zip (یہ+horizGlyphCoverage, glyphConstruction.as_array (vert_Glyph_گنتی + horiz_Glyph_گنتی).sub_array (vert_Glyph_گنتی)))
      اگر (c->glyph_سیٹ->has (hb_پہلا (اندراج_2)))
        (یہ+hb_دوسرا (اندراج_2)).collect_variation_indices (c);
  }

  منطقی sanitize_offsets (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    غیرعلامتی صحیح گنتی = vert_Glyph_گنتی + horiz_Glyph_گنتی;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
      اگر (!glyphConstruction.arrayZ[i].sanitize (c, یہ)) return_trace (جھوٹ);
    return_trace (سچ);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  vertGlyphCoverage.sanitize (c, یہ) &&
		  horizGlyphCoverage.sanitize (c, یہ) &&
		  hb_barrier () &&
		  c->check_array (glyphConstruction.arrayZ, vert_Glyph_گنتی + horiz_Glyph_گنتی) &&
		  sanitize_offsets (c));
  }

  hb_position_t get_min_connector_overlap (hb_direction_t direction,
						  hb_font_t *font) ثابت
  { واپس font->em_scale_dir (minConnectorOverlap, direction); }

  غیرعلامتی صحیح get_glyph_variants (hb_codepoint_t glyph,
				   hb_direction_t direction,
				   hb_font_t *font,
				   غیرعلامتی صحیح آغاز_انحراف,
				   غیرعلامتی صحیح *variants_گنتی, /* IN/OUT */
				   hb_ot_math_glyph_variant_t *variants /*برآمد*/) ثابت
  { واپس get_glyph_construction (glyph, direction, font)
	   .get_variants (direction, font, آغاز_انحراف, variants_گنتی, variants); }

  غیرعلامتی صحیح get_glyph_parts (hb_codepoint_t glyph,
				hb_direction_t direction,
				hb_font_t *font,
				غیرعلامتی صحیح آغاز_انحراف,
				غیرعلامتی صحیح *parts_گنتی, /* IN/OUT */
				hb_ot_math_glyph_part_t *parts /*برآمد*/,
				hb_position_t *italics_correction /*برآمد*/) ثابت
  { واپس get_glyph_construction (glyph, direction, font)
	   .get_assembly ()
	   .get_parts (direction, font,
		       آغاز_انحراف, parts_گنتی, parts,
		       italics_correction); }

  نجی:
  ثابت MathGlyphConstruction &
  get_glyph_construction (hb_codepoint_t glyph,
			  hb_direction_t direction,
			  hb_font_t *font HB_غیر_مستعمل) ثابت
  {
    منطقی vertical = HB_DIRECTION_IS_VERTICAL (direction);
    غیرعلامتی صحیح گنتی = vertical ? vert_Glyph_گنتی : horiz_Glyph_گنتی;
    ثابت Offset16_مقصد<Coverage> &coverage = vertical ? vertGlyphCoverage
						  : horizGlyphCoverage;

    غیرعلامتی صحیح اشاریہ_2 = (یہ+coverage).get_coverage (glyph);
    اگر (unlikely (اشاریہ_2 >= گنتی)) واپس Null (MathGlyphConstruction);

    اگر (!vertical)
      اشاریہ_2 += vert_Glyph_گنتی;

    واپس یہ+glyphConstruction[اشاریہ_2];
  }

  محفوظ:
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
  HBUINT16	vert_Glyph_گنتی;	/* Number of glyphs for which
				 * information is provided for
				 * vertically growing variants. */
  HBUINT16	horiz_Glyph_گنتی;/* Number of glyphs for which
				 * information is provided for
				 * horizontally growing variants. */

  /* Array of offsets to MathGlyphConstruction tables - from the beginning of
     the MathVariants table, for shapes growing in vertical/horizontal
     direction. */
  Unsized_Array_کا<Offset16_مقصد<MathGlyphConstruction>>
			glyphConstruction;

  عوامی:
  DEFINE_سائز_ARRAY (10, glyphConstruction);
};


/*
 * MATH -- Mathematical typesetting
 * https://docs.microsoft.com/en-us/typography/opentype/spec/math
 */

ساخت MATH
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_MATH;

  منطقی has_ڈیٹا () ثابت { واپس نسخہ.مقصد_int (); }

  خالی depend (hb_depend_ڈیٹا_builder_t *depend_ڈیٹا) ثابت
  {
    اگر (mathVariants)
      (یہ+mathVariants).depend (depend_ڈیٹا);
  }

  خالی احاطہ_glyphs (hb_سیٹ_t *glyph_سیٹ) ثابت
  {
    اگر (mathVariants)
    {
      hb_سیٹ_t variant_glyphs;
      (یہ+mathVariants).احاطہ_glyphs (glyph_سیٹ, &variant_glyphs);
      hb_سیٹ_union (glyph_سیٹ, &variant_glyphs);
    }
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->embed (*یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    برآمد->mathConstants.serialize_copy (c->serializer, mathConstants, یہ, 0, hb_serialize_context_t::سر, &c->plan->layout_variation_idx_delta_map);
    برآمد->mathGlyphInfo.serialize_subset (c, mathGlyphInfo, یہ);
    برآمد->mathVariants.serialize_subset (c, mathVariants, یہ);
    return_trace (سچ);
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c, ثابت hb_سیٹ_t *variant_glyph_سیٹ) ثابت
  {
    (یہ+mathConstants).collect_variation_indices (c);
    (یہ+mathGlyphInfo).collect_variation_indices (c);
    خودکار *glyph_سیٹ = c->glyph_سیٹ;
    c->glyph_سیٹ = variant_glyph_سیٹ;
    (یہ+mathVariants).collect_variation_indices (c);
    c->glyph_سیٹ = glyph_سیٹ;
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (نسخہ.sanitize (c) &&
		  likely (نسخہ.major == 1) &&
		  hb_barrier () &&
		  mathConstants.sanitize (c, یہ) &&
		  mathGlyphInfo.sanitize (c, یہ) &&
		  mathVariants.sanitize (c, یہ));
  }

  // https://github.com/harfbuzz/harfbuzz/issues/4653
  HB_اندرونی منطقی is_bad_cambria (hb_font_t *font) ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_MATH
    انتخاب HB_CODEPOINT_ENCODE3 (font->face->جدول.MATH.get_blob ()->لمبائی,
                                 (یہ+mathConstants).min_اونچائی[1], // displayOperatorMinHeight
                                 (یہ+mathConstants).min_اونچائی[0]) // delimitedSubFormulaMinHeight
    {
      /* sha1sum:ab4a4fe054d23061f3c039493d6f665cfda2ecf5  cambria.ttc
       * sha1sum:086855301bff644f9d8827b88491fcf73a6d4cb9  cambria.ttc
       * sha1sum:b1e5a3feaca2ea3dfcf79ccb377de749ecf60343  cambria.ttc */
      حالت HB_CODEPOINT_ENCODE3 (25722, 2500, 3000):
        واپس سچ;
    }
#اگر_ختم
    واپس جھوٹ;
  }

  hb_position_t get_constant (hb_ot_math_constant_t  constant,
			      hb_font_t		   *font) ثابت
  { واپس (یہ+mathConstants).get_قدر (constant, font); }

  ثابت MathGlyphInfo &get_glyph_info () ثابت { واپس یہ+mathGlyphInfo; }

  ثابت MathVariants &get_variants () ثابت    { واپس یہ+mathVariants; }

  محفوظ:
  Fixed_نسخہ<>نسخہ;	/* Version of the MATH table
				 * initially set to 0x00010000u */
  Offset16_مقصد<MathConstants>
		mathConstants;	/* MathConstants table */
  Offset16_مقصد<MathGlyphInfo>
		mathGlyphInfo;	/* MathGlyphInfo table */
  Offset16_مقصد<MathVariants>
		mathVariants;	/* MathVariants table */

  عوامی:
  DEFINE_سائز_STATIC (10);
};

} /* namespace OT */


#اگر_ختم /* HB_OT_MATH_TABLE_HH */

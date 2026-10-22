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

#جے_بنا_تعریف HB_OT_MATH_جدول_HH
#تعریف_کرو HB_OT_MATH_جدول_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-layout-common.hh"
#ضمن "hb-ot-layout-gdef-table.hh"
#ضمن "hb-ot-math.h"
#ضمن "hb-depend-data.hh"

نطاق OT {


بنية Math_قدر_ریکارڈ
{
  hb_position_t get_x_قدر (hb_font_t *font, ثابت فراغ *ادھار) ثابت
  { أرجع hb_saturate_جوڑو (font->em_scale_x (value), (ادھار+device_جدول).get_x_delta (font, get_var_store (font, ادھار))); }
  hb_position_t get_y_قدر (hb_font_t *font, ثابت فراغ *ادھار) ثابت
  { أرجع hb_saturate_جوڑو (font->em_scale_y (value), (ادھار+device_جدول).get_y_delta (font, get_var_store (font, ادھار))); }

  ثابت شے_Variation_Store &get_var_store (hb_font_t *font, ثابت فراغ *ادھار) ثابت
  {
#إذا !defined(HB_NO_VAR) && !defined(HB_NO_OT_LAYOUT)
    إذا ((ادھار+device_جدول).is_variation_device ())
      أرجع font->face->جدول.GDEF->جدول->get_var_store ();
#جے_انت
    أرجع Null (شے_Variation_Store);
  }

  Math_قدر_ریکارڈ* copy (hb_serialize_context_t *c, ثابت فراغ *ادھار,
                         ثابت hb_hashmap_t<بنانشانی, hb_pair_t<بنانشانی, صحيح>> *variation_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار *باہرلا = c->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خالی_سوچک);
    hb_pair_t<بنانشانی, صحيح> *varidx_delta;
    إذا (variation_map->has ((ادھار+device_جدول).get_variation_اشاریہ (), &varidx_delta) &&
        !c->check_سونپو (باہرلا->value, (int64_t) value + hb_دوجا (*varidx_delta),
                          HB_SERIALIZE_ERROR_INT_حد_توں_ودھ))
      return_trace (خالی_سوچک);
    باہرلا->device_جدول.serialize_copy (c, device_جدول, ادھار, 0, hb_serialize_context_t::سر, variation_map);

    return_trace (باہرلا);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c, ثابت فراغ *ادھار) ثابت
  { (ادھار+device_جدول).collect_variation_indices (c); }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *ادھار) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && device_جدول.sanitize (c, ادھار));
  }

  محمي:
  HBINT16		value;		/* The X or Y value in design units */
  Offset16_منزل<Device>	device_جدول;	/* Offset to the device table - from the
					 * beginning of parent table.  May be NULL.
					 * Suggested format for device table is 1. */

  عام:
  DEFINE_ناپ_STATIC (4);
};

بنية MathConstants
{
  یار بنية MATH;

  MathConstants* copy (hb_serialize_context_t *c,
                      ثابت hb_hashmap_t<بنانشانی, hb_pair_t<بنانشانی, صحيح>> *variation_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار *باہرلا = c->شروع_embed (هذا);

    HBINT16 *p = c->allocate_ناپ<HBINT16> (HBINT16::static_ناپ * 2);
    إذا (unlikely (!p)) return_trace (خالی_سوچک);
    hb_memcpy (p, percent_Scale_تھلے, HBINT16::static_ناپ * 2);

    HBUINT16 *m = c->allocate_ناپ<HBUINT16> (HBUINT16::static_ناپ * 2);
    إذا (unlikely (!m)) return_trace (خالی_سوچک);
    hb_memcpy (m, min_اچائی, HBUINT16::static_ناپ * 2);

    بنانشانی گنتی = ARRAY_لمائی (math_قدر_ریکارڈاں);
    لكل (بنانشانی i = 0; i < گنتی; i++)
      إذا (!c->copy (math_قدر_ریکارڈاں[i], هذا, variation_map))
        return_trace (خالی_سوچک);

    إذا (!c->embed (radicalDegreeBottomRaisePercent)) return_trace (خالی_سوچک);
    return_trace (باہرلا);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت خودکار &ریکارڈ : math_قدر_ریکارڈاں)
      ریکارڈ.collect_variation_indices (c, هذا);
  }

  منطقي sanitize_math_قدر_ریکارڈاں (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);

    بنانشانی صحيح گنتی = ARRAY_لمائی (math_قدر_ریکارڈاں);
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
      إذا (!math_قدر_ریکارڈاں[i].sanitize (c, هذا))
	return_trace (خطأ);

    return_trace (سچ);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && sanitize_math_قدر_ریکارڈاں (c));
  }

  hb_position_t get_قدر (hb_ot_math_constant_t constant,
			   hb_font_t *font) ثابت
  {
    بدل (constant) {

    حالة HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_تھلے:
    حالة HB_OT_MATH_CONSTANT_SCRIPT_SCRIPT_PERCENT_SCALE_تھلے:
      أرجع percent_Scale_تھلے[constant - HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_تھلے];

    حالة HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_اچائی:
    حالة HB_OT_MATH_CONSTANT_DISPLAY_OPERATOR_MIN_اچائی:
      أرجع font->em_scale_y (min_اچائی[constant - HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_اچائی]);

    حالة HB_OT_MATH_CONSTANT_RADICAL_KERN_AFTER_DEGREE:
    حالة HB_OT_MATH_CONSTANT_RADICAL_KERN_BEFORE_DEGREE:
    حالة HB_OT_MATH_CONSTANT_SKEWED_FRACTION_HORIZONTAL_GAP:
    حالة HB_OT_MATH_CONSTANT_SPACE_AFTER_SCRIPT:
      أرجع math_قدر_ریکارڈاں[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_x_قدر (font, هذا);

    حالة HB_OT_MATH_CONSTANT_ACCENT_ادھار_اچائی:
    حالة HB_OT_MATH_CONSTANT_AXIS_اچائی:
    حالة HB_OT_MATH_CONSTANT_FLATTENED_ACCENT_ادھار_اچائی:
    حالة HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_DISPLAY_STYLE_کھسکاؤ_تھلے:
    حالة HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_کھسکاؤ_تھلے:
    حالة HB_OT_MATH_CONSTANT_FRACTION_DENOM_DISPLAY_STYLE_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_DISPLAY_STYLE_کھسکاؤ_اتے:
    حالة HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_کھسکاؤ_اتے:
    حالة HB_OT_MATH_CONSTANT_FRACTION_عدد_DISPLAY_STYLE_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_FRACTION_قاعدہ_THICKNESS:
    حالة HB_OT_MATH_CONSTANT_ہیٹھلا_حد_بندی_BASELINE_DROP_MIN:
    حالة HB_OT_MATH_CONSTANT_ہیٹھلا_حد_بندی_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_MATH_LEADING:
    حالة HB_OT_MATH_CONSTANT_OVERBAR_EXTRA_ASCENDER:
    حالة HB_OT_MATH_CONSTANT_OVERBAR_قاعدہ_THICKNESS:
    حالة HB_OT_MATH_CONSTANT_OVERBAR_VERTICAL_GAP:
    حالة HB_OT_MATH_CONSTANT_RADICAL_DISPLAY_STYLE_VERTICAL_GAP:
    حالة HB_OT_MATH_CONSTANT_RADICAL_EXTRA_ASCENDER:
    حالة HB_OT_MATH_CONSTANT_RADICAL_قاعدہ_THICKNESS:
    حالة HB_OT_MATH_CONSTANT_RADICAL_VERTICAL_GAP:
    حالة HB_OT_MATH_CONSTANT_SKEWED_FRACTION_VERTICAL_GAP:
    حالة HB_OT_MATH_CONSTANT_ڈھیر_BOTTOM_DISPLAY_STYLE_کھسکاؤ_تھلے:
    حالة HB_OT_MATH_CONSTANT_ڈھیر_BOTTOM_کھسکاؤ_تھلے:
    حالة HB_OT_MATH_CONSTANT_ڈھیر_DISPLAY_STYLE_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_ڈھیر_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_ڈھیر_چوٹی_DISPLAY_STYLE_کھسکاؤ_اتے:
    حالة HB_OT_MATH_CONSTANT_ڈھیر_چوٹی_کھسکاؤ_اتے:
    حالة HB_OT_MATH_CONSTANT_STRETCH_ڈھیر_BOTTOM_کھسکاؤ_تھلے:
    حالة HB_OT_MATH_CONSTANT_STRETCH_ڈھیر_GAP_ABOVE_MIN:
    حالة HB_OT_MATH_CONSTANT_STRETCH_ڈھیر_GAP_ہیٹھاں_MIN:
    حالة HB_OT_MATH_CONSTANT_STRETCH_ڈھیر_چوٹی_کھسکاؤ_اتے:
    حالة HB_OT_MATH_CONSTANT_SUBSCRIPT_BASELINE_DROP_MIN:
    حالة HB_OT_MATH_CONSTANT_SUBSCRIPT_کھسکاؤ_تھلے:
    حالة HB_OT_MATH_CONSTANT_SUBSCRIPT_چوٹی_MAX:
    حالة HB_OT_MATH_CONSTANT_SUB_SUPERSCRIPT_GAP_MIN:
    حالة HB_OT_MATH_CONSTANT_SUPERSCRIPT_BASELINE_DROP_MAX:
    حالة HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MAX_WITH_SUBSCRIPT:
    حالة HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MIN:
    حالة HB_OT_MATH_CONSTANT_SUPERSCRIPT_کھسکاؤ_اتے:
    حالة HB_OT_MATH_CONSTANT_SUPERSCRIPT_کھسکاؤ_اتے_CRAMPED:
    حالة HB_OT_MATH_CONSTANT_UNDERBAR_EXTRA_DESCENDER:
    حالة HB_OT_MATH_CONSTANT_UNDERBAR_قاعدہ_THICKNESS:
    حالة HB_OT_MATH_CONSTANT_UNDERBAR_VERTICAL_GAP:
    حالة HB_OT_MATH_CONSTANT_اتلا_حد_بندی_BASELINE_RISE_MIN:
    حالة HB_OT_MATH_CONSTANT_اتلا_حد_بندی_GAP_MIN:
      أرجع math_قدر_ریکارڈاں[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_y_قدر (font, هذا);

    حالة HB_OT_MATH_CONSTANT_RADICAL_DEGREE_BOTTOM_RAISE_PERCENT:
      أرجع radicalDegreeBottomRaisePercent;

    افتراضي:
      أرجع 0;
    }
  }

  محمي:
  HBINT16 percent_Scale_تھلے[2];
  HBUINT16 min_اچائی[2];
  Math_قدر_ریکارڈ math_قدر_ریکارڈاں[51];
  HBINT16 radicalDegreeBottomRaisePercent;

  عام:
  DEFINE_ناپ_STATIC (214);
};

بنية MathItalicsCorrectionInfo
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سیٹ_t &glyphset = c->plan->_glyphset_mathed;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);

    hb_sorted_سمتیہ_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+coverage, italicsCorrection)
    | hb_filter (glyphset, hb_پہلا)
    | hb_filter (serialize_math_ریکارڈ_array (c->serializer, باہرلا->italicsCorrection, هذا, &c->plan->layout_variation_idx_delta_map), hb_دوجا)
    | hb_map (hb_پہلا)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    باہرلا->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (سچ);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت خودکار &اندراج : hb_zip (هذا+coverage, italicsCorrection))
      إذا (c->glyph_سیٹ->has (hb_پہلا (اندراج)))
        hb_دوجا (اندراج).collect_variation_indices (c, هذا);
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
    بنانشانی صحيح اشاریہ_2 = (هذا+coverage).get_coverage (glyph);
    أرجع italicsCorrection[اشاریہ_2].get_x_قدر (font, هذا);
  }

  محمي:
  Offset16_منزل<Coverage>       coverage;		/* Offset to Coverage table -
						 * from the beginning of
						 * MathItalicsCorrectionInfo
						 * table. */
  Array16_دا<Math_قدر_ریکارڈ> italicsCorrection;	/* Array of MathValueRecords
						 * defining italics correction
						 * values for each
						 * covered glyph. */

  عام:
  DEFINE_ناپ_ARRAY (4, italicsCorrection);
};

بنية Math_چوٹی_Accent_Attachment
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سیٹ_t &glyphset = c->plan->_glyphset_mathed;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);

    hb_sorted_سمتیہ_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+چوٹی_Accent_Coverage, چوٹی_Accent_Attachment)
    | hb_filter (glyphset, hb_پہلا)
    | hb_filter (serialize_math_ریکارڈ_array (c->serializer, باہرلا->چوٹی_Accent_Attachment, هذا, &c->plan->layout_variation_idx_delta_map), hb_دوجا)
    | hb_map (hb_پہلا)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    باہرلا->چوٹی_Accent_Coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (سچ);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت خودکار &اندراج : hb_zip (هذا+چوٹی_Accent_Coverage, چوٹی_Accent_Attachment))
      إذا (c->glyph_سیٹ->has (hb_پہلا (اندراج)))
        hb_دوجا (اندراج).collect_variation_indices (c, هذا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  چوٹی_Accent_Coverage.sanitize (c, هذا) &&
		  چوٹی_Accent_Attachment.sanitize (c, هذا));
  }

  hb_position_t get_قدر (hb_codepoint_t glyph,
			   hb_font_t *font) ثابت
  {
    بنانشانی صحيح اشاریہ_2 = (هذا+چوٹی_Accent_Coverage).get_coverage (glyph);
    إذا (اشاریہ_2 == NOT_COVERED)
      أرجع font->get_glyph_h_advance (glyph) / 2;
    أرجع چوٹی_Accent_Attachment[اشاریہ_2].get_x_قدر (font, هذا);
  }

  محمي:
  Offset16_منزل<Coverage>       چوٹی_Accent_Coverage;   /* Offset to Coverage table -
						 * from the beginning of
						 * MathTopAccentAttachment
						 * table. */
  Array16_دا<Math_قدر_ریکارڈ> چوٹی_Accent_Attachment; /* Array of MathValueRecords
						 * defining top accent
						 * attachment points for each
						 * covered glyph. */

  عام:
  DEFINE_ناپ_ARRAY (2 + 2, چوٹی_Accent_Attachment);
};

بنية MathKern
{
  MathKern* copy (hb_serialize_context_t *c,
                      ثابت hb_hashmap_t<بنانشانی, hb_pair_t<بنانشانی, صحيح>> *variation_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار *باہرلا = c->شروع_embed (هذا);

    إذا (unlikely (!c->embed (اچائی_گنتی))) return_trace (خالی_سوچک);

    بنانشانی گنتی = 2 * اچائی_گنتی + 1;
    لكل (بنانشانی i = 0; i < گنتی; i++)
      إذا (!c->copy (math_قدر_ریکارڈاں_Z.arrayZ[i], هذا, variation_map))
        return_trace (خالی_سوچک);

    return_trace (باہرلا);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت خودکار &ریکارڈ : math_قدر_ریکارڈاں_Z.as_array (2 * اچائی_گنتی + 1))
      ریکارڈ.collect_variation_indices (c, هذا);
  }

  منطقي sanitize_math_قدر_ریکارڈاں (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    بنانشانی صحيح گنتی = 2 * اچائی_گنتی + 1;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
      إذا (!math_قدر_ریکارڈاں_Z.arrayZ[i].sanitize (c, هذا)) return_trace (خطأ);
    return_trace (سچ);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  c->check_array (math_قدر_ریکارڈاں_Z.arrayZ, 2 * اچائی_گنتی + 1) &&
		  sanitize_math_قدر_ریکارڈاں (c));
  }

  hb_position_t get_قدر (hb_position_t correction_اچائی, hb_font_t *font) ثابت
  {
    ثابت Math_قدر_ریکارڈ* correction_اچائی_2 = math_قدر_ریکارڈاں_Z.arrayZ;
    ثابت Math_قدر_ریکارڈ* kern_قدر = math_قدر_ریکارڈاں_Z.arrayZ + اچائی_گنتی;
    صحيح sign = font->y_scale < 0 ? -1 : +1;

    /* According to OpenType spec (v1.9), except for the boundary cases, the index
     * chosen for kern value should be i such that
     *    correctionHeight[i-1] <= correction_height < correctionHeight[i]
     * We can use the binary search algorithm of std::upper_bound(). Or, we can
     * use the internal hb_bsearch_impl.
     */
    بنانشانی صحيح pos;
    خودکار cmp = +[](ثابت فراغ* key, ثابت فراغ* p,
                   صحيح sign, hb_font_t* font, ثابت MathKern* mathKern) -> صحيح {
      hb_position_t a = *(hb_position_t *) key;
      hb_position_t b = ((Math_قدر_ریکارڈ *) p)->get_y_قدر (font, mathKern);
      إذا (a == b) أرجع 0;
      أرجع sign > 0 ? (a < b ? -1 : +1) : (b < a ? -1 : +1);
    };
    بنانشانی صحيح i = hb_bsearch_impl(&pos, correction_اچائی, correction_اچائی_2,
                                     اچائی_گنتی, Math_قدر_ریکارڈ::static_ناپ,
                                     cmp, sign, font, هذا) ? pos + 1 : pos;
    أرجع kern_قدر[i].get_x_قدر (font, هذا);
  }

  بنانشانی صحيح get_entries (بنانشانی صحيح شروع_وتھ,
			    بنانشانی صحيح *entries_گنتی, /* IN/OUT */
			    hb_ot_math_kern_اندراج_t *kern_entries, /*باہرلا*/
			    hb_font_t *font) ثابت
  {
    ثابت Math_قدر_ریکارڈ* correction_اچائی_2 = math_قدر_ریکارڈاں_Z.arrayZ;
    ثابت Math_قدر_ریکارڈ* kern_قدر = math_قدر_ریکارڈاں_Z.arrayZ + اچائی_گنتی;
    ثابت بنانشانی صحيح entries_گنتی_2 = اچائی_گنتی + 1;

    إذا (entries_گنتی && kern_entries)
    {
      بنانشانی صحيح شروع = hb_min (شروع_وتھ, entries_گنتی_2);
      بنانشانی صحيح سرا = hb_min (شروع + *entries_گنتی, entries_گنتی_2);
      *entries_گنتی = سرا - شروع;

      لكل (بنانشانی صحيح i = 0; i < *entries_گنتی; i++) {
	بنانشانی صحيح j = شروع + i;

	hb_position_t max_اچائی;
	إذا (j == اچائی_گنتی) {
	  max_اچائی = INT32_MAX;
	} وإلا {
	  max_اچائی = correction_اچائی_2[j].get_y_قدر (font, هذا);
	}

	kern_entries[i] = {max_اچائی, kern_قدر[j].get_x_قدر (font, هذا)};
      }
    }
    أرجع entries_گنتی_2;
  }

  محمي:
  HBUINT16	اچائی_گنتی;
  Unsized_Array_دا<Math_قدر_ریکارڈ>
		math_قدر_ریکارڈاں_Z;
				/* Array of correction heights at
				 * which the kern value changes.
				 * Sorted by the height value in
				 * design units (heightCount entries),
				 * Followed by:
				 * Array of kern values corresponding
				 * to heights. (heightCount+1 entries).
				 */

  عام:
  DEFINE_ناپ_ARRAY (2, math_قدر_ریکارڈاں_Z);
};

بنية Math_Kern_Info_ریکارڈ
{
  Math_Kern_Info_ریکارڈ* copy (hb_serialize_context_t *c, ثابت فراغ *ادھار,
                            ثابت hb_hashmap_t<بنانشانی, hb_pair_t<بنانشانی, صحيح>> *variation_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار *باہرلا = c->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خالی_سوچک);

    بنانشانی گنتی = ARRAY_لمائی (mathKern);
    لكل (بنانشانی i = 0; i < گنتی; i++)
      باہرلا->mathKern[i].serialize_copy (c, mathKern[i], ادھار, 0, hb_serialize_context_t::سر, variation_map);

    return_trace (باہرلا);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c, ثابت فراغ *ادھار) ثابت
  {
    لكل (ثابت خودکار &وتھ : mathKern)
      (ادھار+وتھ).collect_variation_indices (c);
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *ادھار) ثابت
  {
    TRACE_SANITIZE (هذا);

    بنانشانی صحيح گنتی = ARRAY_لمائی (mathKern);
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
      إذا (unlikely (!mathKern[i].sanitize (c, ادھار)))
	return_trace (خطأ);

    return_trace (سچ);
  }

  hb_position_t get_kerning (hb_ot_math_kern_t kern,
			     hb_position_t correction_اچائی,
			     hb_font_t *font,
			     ثابت فراغ *ادھار) ثابت
  {
    بنانشانی صحيح idx = kern;
    إذا (unlikely (idx >= ARRAY_لمائی (mathKern))) أرجع 0;
    أرجع (ادھار+mathKern[idx]).get_قدر (correction_اچائی, font);
  }

  بنانشانی صحيح get_kernings (hb_ot_math_kern_t kern,
			     بنانشانی صحيح شروع_وتھ,
			     بنانشانی صحيح *entries_گنتی, /* IN/OUT */
			     hb_ot_math_kern_اندراج_t *kern_entries, /*باہرلا*/
			     hb_font_t *font,
			     ثابت فراغ *ادھار) ثابت
  {
    بنانشانی صحيح idx = kern;
    إذا (unlikely (idx >= ARRAY_لمائی (mathKern)) || !mathKern[idx]) {
      إذا (entries_گنتی) *entries_گنتی = 0;
      أرجع 0;
    }
    أرجع (ادھار+mathKern[idx]).get_entries (شروع_وتھ,
					     entries_گنتی,
					     kern_entries,
					     font);
  }

  محمي:
  /* Offset to MathKern table for each corner -
   * from the beginning of MathKernInfo table.  May be NULL. */
  Offset16_منزل<MathKern> mathKern[4];

  عام:
  DEFINE_ناپ_STATIC (8);
};

بنية MathKernInfo
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سیٹ_t &glyphset = c->plan->_glyphset_mathed;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);

    hb_sorted_سمتیہ_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+mathKernCoverage, math_Kern_Info_ریکارڈاں)
    | hb_filter (glyphset, hb_پہلا)
    | hb_filter (serialize_math_ریکارڈ_array (c->serializer, باہرلا->math_Kern_Info_ریکارڈاں, هذا, &c->plan->layout_variation_idx_delta_map), hb_دوجا)
    | hb_map (hb_پہلا)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    باہرلا->mathKernCoverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (سچ);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت خودکار &اندراج : hb_zip (هذا+mathKernCoverage, math_Kern_Info_ریکارڈاں))
      إذا (c->glyph_سیٹ->has (hb_پہلا (اندراج)))
        hb_دوجا (اندراج).collect_variation_indices (c, هذا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  mathKernCoverage.sanitize (c, هذا) &&
		  math_Kern_Info_ریکارڈاں.sanitize (c, هذا));
  }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_اچائی,
			     hb_font_t *font) ثابت
  {
    بنانشانی صحيح اشاریہ_2 = (هذا+mathKernCoverage).get_coverage (glyph);
    أرجع math_Kern_Info_ریکارڈاں[اشاریہ_2].get_kerning (kern, correction_اچائی, font, هذا);
  }

  بنانشانی صحيح get_kernings (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     بنانشانی صحيح شروع_وتھ,
			     بنانشانی صحيح *entries_گنتی, /* IN/OUT */
			     hb_ot_math_kern_اندراج_t *kern_entries, /*باہرلا*/
			     hb_font_t *font) ثابت
  {
    بنانشانی صحيح اشاریہ_2 = (هذا+mathKernCoverage).get_coverage (glyph);
    أرجع math_Kern_Info_ریکارڈاں[اشاریہ_2].get_kernings (kern,
						    شروع_وتھ,
						    entries_گنتی,
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
  Array16_دا<Math_Kern_Info_ریکارڈ>
		math_Kern_Info_ریکارڈاں;
				/* Array of MathKernInfoRecords,
				 * per-glyph information for
				 * mathematical positioning
				 * of subscripts and
				 * superscripts. */

  عام:
  DEFINE_ناپ_ARRAY (4, math_Kern_Info_ریکارڈاں);
};

بنية MathGlyphInfo
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->embed (*هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    باہرلا->mathItalicsCorrectionInfo.serialize_subset (c, mathItalicsCorrectionInfo, هذا);
    باہرلا->math_چوٹی_Accent_Attachment.serialize_subset (c, math_چوٹی_Accent_Attachment, هذا);

    ثابت hb_سیٹ_t &glyphset = c->plan->_glyphset_mathed;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار it =
    + hb_iter (هذا+extendedShapeCoverage)
    | hb_take (c->plan->سروت->get_عدد_glyphs ())
    | hb_filter (glyphset)
    | hb_map_retains_چھانٹی (glyph_map)
    ;

    إذا (it) باہرلا->extendedShapeCoverage.serialize_serialize (c->serializer, it);
    وإلا باہرلا->extendedShapeCoverage = 0;

    باہرلا->mathKernInfo.serialize_subset (c, mathKernInfo, هذا);
    return_trace (سچ);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    (هذا+mathItalicsCorrectionInfo).collect_variation_indices (c);
    (هذا+math_چوٹی_Accent_Attachment).collect_variation_indices (c);
    (هذا+mathKernInfo).collect_variation_indices (c);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  mathItalicsCorrectionInfo.sanitize (c, هذا) &&
		  math_چوٹی_Accent_Attachment.sanitize (c, هذا) &&
		  extendedShapeCoverage.sanitize (c, هذا) &&
		  mathKernInfo.sanitize (c, هذا));
  }

  hb_position_t
  get_italics_correction (hb_codepoint_t  glyph, hb_font_t *font) ثابت
  { أرجع (هذا+mathItalicsCorrectionInfo).get_قدر (glyph, font); }

  hb_position_t
  get_چوٹی_accent_attachment (hb_codepoint_t  glyph, hb_font_t *font) ثابت
  { أرجع (هذا+math_چوٹی_Accent_Attachment).get_قدر (glyph, font); }

  منطقي is_extended_shape (hb_codepoint_t glyph) ثابت
  { أرجع (هذا+extendedShapeCoverage).get_coverage (glyph) != NOT_COVERED; }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_اچائی,
			     hb_font_t *font) ثابت
  { أرجع (هذا+mathKernInfo).get_kerning (glyph, kern, correction_اچائی, font); }

  hb_position_t get_kernings (hb_codepoint_t glyph,
			      hb_ot_math_kern_t kern,
			      بنانشانی صحيح شروع_وتھ,
			      بنانشانی صحيح *entries_گنتی, /* IN/OUT */
			      hb_ot_math_kern_اندراج_t *kern_entries, /*باہرلا*/
			      hb_font_t *font) ثابت
  { أرجع (هذا+mathKernInfo).get_kernings (glyph,
					     kern,
					     شروع_وتھ,
					     entries_گنتی,
					     kern_entries,
					     font); }

  محمي:
  /* Offset to MathItalicsCorrectionInfo table -
   * from the beginning of MathGlyphInfo table. */
  Offset16_منزل<MathItalicsCorrectionInfo> mathItalicsCorrectionInfo;

  /* Offset to MathTopAccentAttachment table -
   * from the beginning of MathGlyphInfo table. */
  Offset16_منزل<Math_چوٹی_Accent_Attachment> math_چوٹی_Accent_Attachment;

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
  DEFINE_ناپ_STATIC (8);
};

بنية Math_Glyph_Variant_ریکارڈ
{
  یار بنية MathGlyphConstruction;

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    ثابت hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_سونپو (باہرلا->variantGlyph, glyph_map.get (variantGlyph), HB_SERIALIZE_ERROR_INT_حد_توں_ودھ));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  فراغ depend (hb_depend_انکڑے_builder_t *depend_انکڑے, بنانشانی سروت) ثابت
  { depend_انکڑے->جوڑو_depend(سروت, HB_OT_ٹیگ_MATH, variantGlyph); }

  فراغ گھیرا_glyphs (hb_سیٹ_t *variant_glyphs) ثابت
  { variant_glyphs->جوڑو (variantGlyph); }

  محمي:
  HBGlyphID16 variantGlyph;       /* Glyph ID for the variant. */
  HBUINT16  advanceMeasurement; /* Advance width/height, in design units, of the
				 * variant, in the direction of requested
				 * glyph extension. */

  عام:
  DEFINE_ناپ_STATIC (4);
};

بنية PartFlags : HBUINT16
{
  تعداد Flags {
    Extender	= 0x0001u, /* If set, the part can be skipped or repeated. */

    Defined	= 0x0001u, /* All defined flags. */
  };

  عام:
  DEFINE_ناپ_STATIC (2);
};

بنية Math_Glyph_Part_ریکارڈ
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    ثابت hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_سونپو (باہرلا->glyph, glyph_map.get (glyph), HB_SERIALIZE_ERROR_INT_حد_توں_ودھ));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  فراغ extract (hb_ot_math_glyph_part_t &باہرلا,
		int64_t mult,
		hb_font_t *font) ثابت
  {
    باہرلا.glyph			= glyph;

    باہرلا.شروع_connector_لمائی	= font->em_mult (شروع_Connector_لمائی, mult);
    باہرلا.سرا_connector_لمائی	= font->em_mult (سرا_Connector_لمائی, mult);
    باہرلا.بھریا_advance		= font->em_mult (بھریا_Advance, mult);

    static_assert ((بنانشانی صحيح) HB_OT_MATH_GLYPH_PART_FLAG_EXTENDER ==
		   (بنانشانی صحيح) PartFlags::Extender, "");

    باہرلا.flags = (hb_ot_math_glyph_part_flags_t)
		(بنانشانی صحيح)
		(partFlags & PartFlags::Defined);
  }

  فراغ depend (hb_depend_انکڑے_builder_t *depend_انکڑے, بنانشانی سروت) ثابت
  { depend_انکڑے->جوڑو_depend(سروت, HB_OT_ٹیگ_MATH, glyph); }

  فراغ گھیرا_glyphs (hb_سیٹ_t *variant_glyphs) ثابت
  { variant_glyphs->جوڑو (glyph); }

  محمي:
  HBGlyphID16	glyph;		/* Glyph ID for the part. */
  HBUINT16	شروع_Connector_لمائی;
				/* Advance width/ height of the straight bar
				 * connector material, in design units, is at
				 * the beginning of the glyph, in the
				 * direction of the extension. */
  HBUINT16	سرا_Connector_لمائی;
				/* Advance width/ height of the straight bar
				 * connector material, in design units, is at
				 * the end of the glyph, in the direction of
				 * the extension. */
  HBUINT16	بھریا_Advance;	/* Full advance width/height for this part,
				 * in the direction of the extension.
				 * In design units. */
  PartFlags	partFlags;	/* Part qualifiers. */

  عام:
  DEFINE_ناپ_STATIC (10);
};

بنية MathGlyphAssembly
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);

    إذا (!c->serializer->copy (italicsCorrection, هذا, &c->plan->layout_variation_idx_delta_map)) return_trace (خطأ);
    إذا (!c->serializer->copy<HBUINT16> (part_ریکارڈاں.len)) return_trace (خطأ);

    لكل (ثابت خودکار& ریکارڈ : part_ریکارڈاں.iter ())
      إذا (!ریکارڈ.subset (c)) return_trace (خطأ);
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
		  part_ریکارڈاں.sanitize (c));
  }

  بنانشانی صحيح get_parts (hb_direction_t direction,
			  hb_font_t *font,
			  بنانشانی صحيح شروع_وتھ,
			  بنانشانی صحيح *parts_گنتی, /* IN/OUT */
			  hb_ot_math_glyph_part_t *parts /*باہرلا*/,
			  hb_position_t *italics_correction /*باہرلا*/) ثابت
  {
    إذا (parts_گنتی && parts)
    {
      int64_t mult = font->dir_mult (direction);
      لكل (خودکار _ : hb_zip (part_ریکارڈاں.as_array ().sub_array (شروع_وتھ, parts_گنتی),
			    hb_array (parts, *parts_گنتی)))
	_.پہلا.extract (_.second, mult, font);
    }

    إذا (italics_correction)
      *italics_correction = italicsCorrection.get_x_قدر (font, هذا);

    أرجع part_ریکارڈاں.len;
  }

  فراغ depend (hb_depend_انکڑے_builder_t *depend_انکڑے, بنانشانی سروت) ثابت
  {
    لكل (ثابت خودکار& _ : part_ریکارڈاں.iter ())
      _.depend (depend_انکڑے, سروت);
  }

  فراغ گھیرا_glyphs (hb_سیٹ_t *variant_glyphs) ثابت
  {
    لكل (ثابت خودکار& _ : part_ریکارڈاں.iter ())
      _.گھیرا_glyphs (variant_glyphs);
  }

  محمي:
  Math_قدر_ریکارڈ
		italicsCorrection;
				/* Italics correction of this
				 * MathGlyphAssembly. Should not
				 * depend on the assembly size. */
  Array16_دا<Math_Glyph_Part_ریکارڈ>
		part_ریکارڈاں;	/* Array of part records, from
				 * left to right and bottom to
				 * top. */

  عام:
  DEFINE_ناپ_ARRAY (6, part_ریکارڈاں);
};

بنية MathGlyphConstruction
{
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);

    باہرلا->glyphAssembly.serialize_subset (c, glyphAssembly, هذا);

    إذا (!c->serializer->check_سونپو (باہرلا->math_Glyph_Variant_ریکارڈ.len, math_Glyph_Variant_ریکارڈ.len, HB_SERIALIZE_ERROR_INT_حد_توں_ودھ))
      return_trace (خطأ);
    لكل (ثابت خودکار& ریکارڈ : math_Glyph_Variant_ریکارڈ.iter ())
      إذا (!ریکارڈ.subset (c)) return_trace (خطأ);

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
		  math_Glyph_Variant_ریکارڈ.sanitize (c));
  }

  ثابت MathGlyphAssembly &get_assembly () ثابت { أرجع هذا+glyphAssembly; }

  بنانشانی صحيح get_variants (hb_direction_t direction,
			     hb_font_t *font,
			     بنانشانی صحيح شروع_وتھ,
			     بنانشانی صحيح *variants_گنتی, /* IN/OUT */
			     hb_ot_math_glyph_variant_t *variants /*باہرلا*/) ثابت
  {
    إذا (variants_گنتی && variants)
    {
      int64_t mult = font->dir_mult (direction);
      لكل (خودکار _ : hb_zip (math_Glyph_Variant_ریکارڈ.as_array ().sub_array (شروع_وتھ, variants_گنتی),
			    hb_array (variants, *variants_گنتی)))
	_.second = {_.پہلا.variantGlyph, font->em_mult (_.پہلا.advanceMeasurement, mult)};
    }
    أرجع math_Glyph_Variant_ریکارڈ.len;
  }

  فراغ depend (hb_depend_انکڑے_builder_t *depend_انکڑے, بنانشانی سروت) ثابت
  {
    (هذا+glyphAssembly).depend (depend_انکڑے, سروت);

    لكل (ثابت خودکار& _ : math_Glyph_Variant_ریکارڈ.iter ())
      _.depend (depend_انکڑے, سروت);
  }

  فراغ گھیرا_glyphs (hb_سیٹ_t *variant_glyphs) ثابت
  {
    (هذا+glyphAssembly).گھیرا_glyphs (variant_glyphs);

    لكل (ثابت خودکار& _ : math_Glyph_Variant_ریکارڈ.iter ())
      _.گھیرا_glyphs (variant_glyphs);
  }

  محمي:
  /* Offset to MathGlyphAssembly table for this shape - from the beginning of
     MathGlyphConstruction table.  May be NULL. */
  Offset16_منزل<MathGlyphAssembly>	  glyphAssembly;

  /* MathGlyphVariantRecords for alternative variants of the glyphs. */
  Array16_دا<Math_Glyph_Variant_ریکارڈ> math_Glyph_Variant_ریکارڈ;

  عام:
  DEFINE_ناپ_ARRAY (4, math_Glyph_Variant_ریکارڈ);
};

بنية MathVariants
{
  فراغ depend (hb_depend_انکڑے_builder_t *depend_انکڑے) ثابت
  {
    ثابت hb_array_t<ثابت Offset16_منزل<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vert_Glyph_گنتی + horiz_Glyph_گنتی);

    إذا (vertGlyphCoverage)
    {
      ثابت خودکار vert_offsets = glyph_construction_offsets.sub_array (0, vert_Glyph_گنتی);
      + hb_zip (هذا+vertGlyphCoverage, vert_offsets)
      | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, ثابت Offset16_منزل<MathGlyphConstruction>&> &_)
                  {
                    ثابت MathGlyphConstruction &mgc = هذا+_.second;
                    mgc.depend (depend_انکڑے, _.پہلا);
                  })
      ;
    }
    إذا (horizGlyphCoverage)
    {
      ثابت خودکار hori_offsets = glyph_construction_offsets.sub_array (vert_Glyph_گنتی, horiz_Glyph_گنتی);
      + hb_zip (هذا+horizGlyphCoverage, hori_offsets)
      | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, ثابت Offset16_منزل<MathGlyphConstruction>&> &_)
                  {
                    ثابت MathGlyphConstruction &mgc = هذا+_.second;
                    mgc.depend (depend_انکڑے, _.پہلا);
                  })
      ;
    }
  }

  فراغ گھیرا_glyphs (ثابت hb_سیٹ_t *glyph_سیٹ,
                       hb_سیٹ_t *variant_glyphs) ثابت
  {
    ثابت hb_array_t<ثابت Offset16_منزل<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vert_Glyph_گنتی + horiz_Glyph_گنتی);

    إذا (vertGlyphCoverage)
    {
      ثابت خودکار vert_offsets = glyph_construction_offsets.sub_array (0, vert_Glyph_گنتی);
      + hb_zip (هذا+vertGlyphCoverage, vert_offsets)
      | hb_filter (glyph_سیٹ, hb_پہلا)
      | hb_map (hb_دوجا)
      | hb_map (hb_جوڑو (هذا))
      | hb_apply ([=] (ثابت MathGlyphConstruction &_) { _.گھیرا_glyphs (variant_glyphs); })
      ;
    }

    إذا (horizGlyphCoverage)
    {
      ثابت خودکار hori_offsets = glyph_construction_offsets.sub_array (vert_Glyph_گنتی, horiz_Glyph_گنتی);
      + hb_zip (هذا+horizGlyphCoverage, hori_offsets)
      | hb_filter (glyph_سیٹ, hb_پہلا)
      | hb_map (hb_دوجا)
      | hb_map (hb_جوڑو (هذا))
      | hb_apply ([=] (ثابت MathGlyphConstruction &_) { _.گھیرا_glyphs (variant_glyphs); })
      ;
    }
  }

  فراغ collect_coverage_and_indices (hb_sorted_سمتیہ_t<hb_codepoint_t>& new_coverage,
                                     ثابت Offset16_منزل<Coverage>& coverage,
                                     بنانشانی i,
                                     بنانشانی سرا_اشاریہ,
                                     hb_سیٹ_t& indices,
                                     ثابت hb_سیٹ_t& glyphset,
                                     ثابت hb_map_t& glyph_map) ثابت
  {
    إذا (!coverage) أرجع;

    لكل (ثابت خودکار _ : (هذا+coverage).iter ())
    {
      إذا (i >= سرا_اشاریہ) أرجع;
      إذا (glyphset.has (_))
      {
        بنانشانی new_gid = glyph_map.get (_);
        new_coverage.push (new_gid);
        indices.جوڑو (i);
      }
      i++;
    }
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سیٹ_t &glyphset = c->plan->_glyphset_cmaped;
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);
    إذا (!c->serializer->check_سونپو (باہرلا->minConnectorOverlap, minConnectorOverlap, HB_SERIALIZE_ERROR_INT_حد_توں_ودھ))
      return_trace (خطأ);

    hb_sorted_سمتیہ_t<hb_codepoint_t> new_vert_coverage;
    hb_sorted_سمتیہ_t<hb_codepoint_t> new_hori_coverage;
    hb_سیٹ_t indices;
    collect_coverage_and_indices (new_vert_coverage, vertGlyphCoverage, 0, vert_Glyph_گنتی, indices, glyphset, glyph_map);
    collect_coverage_and_indices (new_hori_coverage, horizGlyphCoverage, vert_Glyph_گنتی, vert_Glyph_گنتی + horiz_Glyph_گنتی, indices, glyphset, glyph_map);

    إذا (!c->serializer->check_سونپو (باہرلا->vert_Glyph_گنتی, new_vert_coverage.لمائی, HB_SERIALIZE_ERROR_INT_حد_توں_ودھ))
      return_trace (خطأ);
    إذا (!c->serializer->check_سونپو (باہرلا->horiz_Glyph_گنتی, new_hori_coverage.لمائی, HB_SERIALIZE_ERROR_INT_حد_توں_ودھ))
      return_trace (خطأ);

    لكل (بنانشانی i : indices.iter ())
    {
      خودکار *o = c->serializer->embed (glyphConstruction[i]);
      إذا (!o) return_trace (خطأ);
      o->serialize_subset (c, glyphConstruction[i], هذا);
    }

    إذا (new_vert_coverage)
      باہرلا->vertGlyphCoverage.serialize_serialize (c->serializer, new_vert_coverage.iter ());

    إذا (new_hori_coverage)
    باہرلا->horizGlyphCoverage.serialize_serialize (c->serializer, new_hori_coverage.iter ());
    return_trace (سچ);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت خودکار &اندراج : hb_zip (هذا+vertGlyphCoverage, glyphConstruction.as_array (vert_Glyph_گنتی)))
      إذا (c->glyph_سیٹ->has (hb_پہلا (اندراج)))
        (هذا+hb_دوجا (اندراج)).collect_variation_indices (c);
    لكل (ثابت خودکار &اندراج : hb_zip (هذا+horizGlyphCoverage, glyphConstruction.as_array (vert_Glyph_گنتی + horiz_Glyph_گنتی).sub_array (vert_Glyph_گنتی)))
      إذا (c->glyph_سیٹ->has (hb_پہلا (اندراج)))
        (هذا+hb_دوجا (اندراج)).collect_variation_indices (c);
  }

  منطقي sanitize_offsets (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    بنانشانی صحيح گنتی = vert_Glyph_گنتی + horiz_Glyph_گنتی;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
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
		  c->check_array (glyphConstruction.arrayZ, vert_Glyph_گنتی + horiz_Glyph_گنتی) &&
		  sanitize_offsets (c));
  }

  hb_position_t get_min_connector_overlap (hb_direction_t direction,
						  hb_font_t *font) ثابت
  { أرجع font->em_scale_dir (minConnectorOverlap, direction); }

  بنانشانی صحيح get_glyph_variants (hb_codepoint_t glyph,
				   hb_direction_t direction,
				   hb_font_t *font,
				   بنانشانی صحيح شروع_وتھ,
				   بنانشانی صحيح *variants_گنتی, /* IN/OUT */
				   hb_ot_math_glyph_variant_t *variants /*باہرلا*/) ثابت
  { أرجع get_glyph_construction (glyph, direction, font)
	   .get_variants (direction, font, شروع_وتھ, variants_گنتی, variants); }

  بنانشانی صحيح get_glyph_parts (hb_codepoint_t glyph,
				hb_direction_t direction,
				hb_font_t *font,
				بنانشانی صحيح شروع_وتھ,
				بنانشانی صحيح *parts_گنتی, /* IN/OUT */
				hb_ot_math_glyph_part_t *parts /*باہرلا*/,
				hb_position_t *italics_correction /*باہرلا*/) ثابت
  { أرجع get_glyph_construction (glyph, direction, font)
	   .get_assembly ()
	   .get_parts (direction, font,
		       شروع_وتھ, parts_گنتی, parts,
		       italics_correction); }

  خاص:
  ثابت MathGlyphConstruction &
  get_glyph_construction (hb_codepoint_t glyph,
			  hb_direction_t direction,
			  hb_font_t *font HB_بن_ورتیا) ثابت
  {
    منطقي vertical = HB_DIRECTION_IS_VERTICAL (direction);
    بنانشانی صحيح گنتی = vertical ? vert_Glyph_گنتی : horiz_Glyph_گنتی;
    ثابت Offset16_منزل<Coverage> &coverage = vertical ? vertGlyphCoverage
						  : horizGlyphCoverage;

    بنانشانی صحيح اشاریہ_2 = (هذا+coverage).get_coverage (glyph);
    إذا (unlikely (اشاریہ_2 >= گنتی)) أرجع Null (MathGlyphConstruction);

    إذا (!vertical)
      اشاریہ_2 += vert_Glyph_گنتی;

    أرجع هذا+glyphConstruction[اشاریہ_2];
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
  HBUINT16	vert_Glyph_گنتی;	/* Number of glyphs for which
				 * information is provided for
				 * vertically growing variants. */
  HBUINT16	horiz_Glyph_گنتی;/* Number of glyphs for which
				 * information is provided for
				 * horizontally growing variants. */

  /* Array of offsets to MathGlyphConstruction tables - from the beginning of
     the MathVariants table, for shapes growing in vertical/horizontal
     direction. */
  Unsized_Array_دا<Offset16_منزل<MathGlyphConstruction>>
			glyphConstruction;

  عام:
  DEFINE_ناپ_ARRAY (10, glyphConstruction);
};


/*
 * MATH -- Mathematical typesetting
 * https://docs.microsoft.com/en-us/typography/opentype/spec/math
 */

بنية MATH
{
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_MATH;

  منطقي has_انکڑے () ثابت { أرجع نسخہ.منزل_int (); }

  فراغ depend (hb_depend_انکڑے_builder_t *depend_انکڑے) ثابت
  {
    إذا (mathVariants)
      (هذا+mathVariants).depend (depend_انکڑے);
  }

  فراغ گھیرا_glyphs (hb_سیٹ_t *glyph_سیٹ) ثابت
  {
    إذا (mathVariants)
    {
      hb_سیٹ_t variant_glyphs;
      (هذا+mathVariants).گھیرا_glyphs (glyph_سیٹ, &variant_glyphs);
      hb_سیٹ_union (glyph_سیٹ, &variant_glyphs);
    }
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->embed (*هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    باہرلا->mathConstants.serialize_copy (c->serializer, mathConstants, هذا, 0, hb_serialize_context_t::سر, &c->plan->layout_variation_idx_delta_map);
    باہرلا->mathGlyphInfo.serialize_subset (c, mathGlyphInfo, هذا);
    باہرلا->mathVariants.serialize_subset (c, mathVariants, هذا);
    return_trace (سچ);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c, ثابت hb_سیٹ_t *variant_glyph_سیٹ) ثابت
  {
    (هذا+mathConstants).collect_variation_indices (c);
    (هذا+mathGlyphInfo).collect_variation_indices (c);
    خودکار *glyph_سیٹ = c->glyph_سیٹ;
    c->glyph_سیٹ = variant_glyph_سیٹ;
    (هذا+mathVariants).collect_variation_indices (c);
    c->glyph_سیٹ = glyph_سیٹ;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (نسخہ.sanitize (c) &&
		  likely (نسخہ.major == 1) &&
		  hb_barrier () &&
		  mathConstants.sanitize (c, هذا) &&
		  mathGlyphInfo.sanitize (c, هذا) &&
		  mathVariants.sanitize (c, هذا));
  }

  // https://github.com/harfbuzz/harfbuzz/issues/4653
  HB_اندرونی منطقي is_bad_cambria (hb_font_t *font) ثابت
  {
#جے_بنا_تعریف HB_NO_MATH
    بدل HB_CODEPOINT_ENCODE3 (font->face->جدول.MATH.get_blob ()->لمائی,
                                 (هذا+mathConstants).min_اچائی[1], // displayOperatorMinHeight
                                 (هذا+mathConstants).min_اچائی[0]) // delimitedSubFormulaMinHeight
    {
      /* sha1sum:ab4a4fe054d23061f3c039493d6f665cfda2ecf5  cambria.ttc
       * sha1sum:086855301bff644f9d8827b88491fcf73a6d4cb9  cambria.ttc
       * sha1sum:b1e5a3feaca2ea3dfcf79ccb377de749ecf60343  cambria.ttc */
      حالة HB_CODEPOINT_ENCODE3 (25722, 2500, 3000):
        أرجع سچ;
    }
#جے_انت
    أرجع خطأ;
  }

  hb_position_t get_constant (hb_ot_math_constant_t  constant,
			      hb_font_t		   *font) ثابت
  { أرجع (هذا+mathConstants).get_قدر (constant, font); }

  ثابت MathGlyphInfo &get_glyph_info () ثابت { أرجع هذا+mathGlyphInfo; }

  ثابت MathVariants &get_variants () ثابت    { أرجع هذا+mathVariants; }

  محمي:
  Fixed_نسخہ<>نسخہ;	/* Version of the MATH table
				 * initially set to 0x00010000u */
  Offset16_منزل<MathConstants>
		mathConstants;	/* MathConstants table */
  Offset16_منزل<MathGlyphInfo>
		mathGlyphInfo;	/* MathGlyphInfo table */
  Offset16_منزل<MathVariants>
		mathVariants;	/* MathVariants table */

  عام:
  DEFINE_ناپ_STATIC (10);
};

} /* namespace OT */


#جے_انت /* HB_OT_MATH_TABLE_HH */

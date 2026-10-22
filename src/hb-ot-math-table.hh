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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_MATH_ਸਾਰਣੀ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_MATH_ਸਾਰਣੀ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-gdef-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-math.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-depend-data.hh"

ਨਾਮਾਕਾਸ਼ OT {


ਬਣਤਰ Math_ਮੁੱਲ_ਰਿਕਾਰਡ
{
  hb_position_t get_x_ਮੁੱਲ (hb_font_t *font, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  { ਵਾਪਸ hb_saturate_ਜੋੜੋ (font->em_scale_x (value), (ਅਧਾਰ+device_ਸਾਰਣੀ).get_x_delta (font, get_var_store (font, ਅਧਾਰ))); }
  hb_position_t get_y_ਮੁੱਲ (hb_font_t *font, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  { ਵਾਪਸ hb_saturate_ਜੋੜੋ (font->em_scale_y (value), (ਅਧਾਰ+device_ਸਾਰਣੀ).get_y_delta (font, get_var_store (font, ਅਧਾਰ))); }

  ਅਚਲ ਵਸਤੂ_Variation_Store &get_var_store (hb_font_t *font, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
#ਜੇ !defined(HB_NO_VAR) && !defined(HB_NO_OT_LAYOUT)
    ਜੇ ((ਅਧਾਰ+device_ਸਾਰਣੀ).is_variation_device ())
      ਵਾਪਸ font->face->ਸਾਰਣੀ.GDEF->ਸਾਰਣੀ->get_var_store ();
#ਜੇ_ਅੰਤ
    ਵਾਪਸ Null (ਵਸਤੂ_Variation_Store);
  }

  Math_ਮੁੱਲ_ਰਿਕਾਰਡ* copy (hb_serialize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
                         ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> *variation_map) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਖਾਲੀ_ਸੂਚਕ);
    hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ> *varidx_delta;
    ਜੇ (variation_map->has ((ਅਧਾਰ+device_ਸਾਰਣੀ).get_variation_ਸੂਚਕਾਂਕ (), &varidx_delta) &&
        !c->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->value, (int64_t) value + hb_ਦੂਜਾ (*varidx_delta),
                          HB_SERIALIZE_ERROR_INT_ਉਛਾਲ))
      return_trace (ਖਾਲੀ_ਸੂਚਕ);
    ਬਾਹਰਲਾ->device_ਸਾਰਣੀ.serialize_copy (c, device_ਸਾਰਣੀ, ਅਧਾਰ, 0, hb_serialize_context_t::ਸਿਰ, variation_map);

    return_trace (ਬਾਹਰਲਾ);
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  { (ਅਧਾਰ+device_ਸਾਰਣੀ).collect_variation_indices (c); }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) && device_ਸਾਰਣੀ.sanitize (c, ਅਧਾਰ));
  }

  ਸੁਰੱਖਿਅਤ:
  HBINT16		value;		/* The X or Y value in design units */
  Offset16_ਮੰਜ਼ਿਲ<Device>	device_ਸਾਰਣੀ;	/* Offset to the device table - from the
					 * beginning of parent table.  May be NULL.
					 * Suggested format for device table is 1. */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4);
};

ਬਣਤਰ MathConstants
{
  ਮਿੱਤਰ ਬਣਤਰ MATH;

  MathConstants* copy (hb_serialize_context_t *c,
                      ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> *variation_map) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->ਸ਼ੁਰੂ_embed (ਇਹ);

    HBINT16 *p = c->allocate_ਮਾਪ<HBINT16> (HBINT16::static_ਮਾਪ * 2);
    ਜੇ (unlikely (!p)) return_trace (ਖਾਲੀ_ਸੂਚਕ);
    hb_memcpy (p, percent_Scale_ਥੱਲੇ, HBINT16::static_ਮਾਪ * 2);

    HBUINT16 *m = c->allocate_ਮਾਪ<HBUINT16> (HBUINT16::static_ਮਾਪ * 2);
    ਜੇ (unlikely (!m)) return_trace (ਖਾਲੀ_ਸੂਚਕ);
    hb_memcpy (m, min_ਉਚਾਈ, HBUINT16::static_ਮਾਪ * 2);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = ARRAY_ਲੰਮਾਈ (math_ਮੁੱਲ_ਰਿਕਾਰਡਾਂ);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
      ਜੇ (!c->copy (math_ਮੁੱਲ_ਰਿਕਾਰਡਾਂ[i], ਇਹ, variation_map))
        return_trace (ਖਾਲੀ_ਸੂਚਕ);

    ਜੇ (!c->embed (radicalDegreeBottomRaisePercent)) return_trace (ਖਾਲੀ_ਸੂਚਕ);
    return_trace (ਬਾਹਰਲਾ);
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ
  {
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &ਰਿਕਾਰਡ : math_ਮੁੱਲ_ਰਿਕਾਰਡਾਂ)
      ਰਿਕਾਰਡ.collect_variation_indices (c, ਇਹ);
  }

  ਤਾਰਕਿਕ sanitize_math_ਮੁੱਲ_ਰਿਕਾਰਡਾਂ (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ARRAY_ਲੰਮਾਈ (math_ਮੁੱਲ_ਰਿਕਾਰਡਾਂ);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      ਜੇ (!math_ਮੁੱਲ_ਰਿਕਾਰਡਾਂ[i].sanitize (c, ਇਹ))
	return_trace (ਝੂਠ);

    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) && sanitize_math_ਮੁੱਲ_ਰਿਕਾਰਡਾਂ (c));
  }

  hb_position_t get_ਮੁੱਲ (hb_ot_math_constant_t constant,
			   hb_font_t *font) ਅਚਲ
  {
    ਚੋਣ (constant) {

    ਹਾਲਤ HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_ਥੱਲੇ:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_SCRIPT_SCRIPT_PERCENT_SCALE_ਥੱਲੇ:
      ਵਾਪਸ percent_Scale_ਥੱਲੇ[constant - HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_ਥੱਲੇ];

    ਹਾਲਤ HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_ਉਚਾਈ:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_DISPLAY_OPERATOR_MIN_ਉਚਾਈ:
      ਵਾਪਸ font->em_scale_y (min_ਉਚਾਈ[constant - HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_ਉਚਾਈ]);

    ਹਾਲਤ HB_OT_MATH_CONSTANT_RADICAL_KERN_AFTER_DEGREE:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_RADICAL_KERN_BEFORE_DEGREE:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_SKEWED_FRACTION_HORIZONTAL_GAP:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_SPACE_AFTER_SCRIPT:
      ਵਾਪਸ math_ਮੁੱਲ_ਰਿਕਾਰਡਾਂ[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_x_ਮੁੱਲ (font, ਇਹ);

    ਹਾਲਤ HB_OT_MATH_CONSTANT_ACCENT_ਅਧਾਰ_ਉਚਾਈ:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_AXIS_ਉਚਾਈ:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_FLATTENED_ACCENT_ਅਧਾਰ_ਉਚਾਈ:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_DISPLAY_STYLE_ਖਿਸਕਾਓ_ਥੱਲੇ:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_GAP_MIN:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_ਖਿਸਕਾਓ_ਥੱਲੇ:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_FRACTION_DENOM_DISPLAY_STYLE_GAP_MIN:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_DISPLAY_STYLE_ਖਿਸਕਾਓ_ਉੱਪਰ:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_GAP_MIN:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_ਖਿਸਕਾਓ_ਉੱਪਰ:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_FRACTION_ਅੰਕ_DISPLAY_STYLE_GAP_MIN:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_FRACTION_ਨਿਯਮ_THICKNESS:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_ਹੇਠਲਾ_ਹੱਦਬੰਦੀ_BASELINE_DROP_MIN:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_ਹੇਠਲਾ_ਹੱਦਬੰਦੀ_GAP_MIN:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_MATH_LEADING:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_OVERBAR_EXTRA_ASCENDER:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_OVERBAR_ਨਿਯਮ_THICKNESS:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_OVERBAR_VERTICAL_GAP:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_RADICAL_DISPLAY_STYLE_VERTICAL_GAP:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_RADICAL_EXTRA_ASCENDER:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_RADICAL_ਨਿਯਮ_THICKNESS:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_RADICAL_VERTICAL_GAP:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_SKEWED_FRACTION_VERTICAL_GAP:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_ਢੇਰ_BOTTOM_DISPLAY_STYLE_ਖਿਸਕਾਓ_ਥੱਲੇ:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_ਢੇਰ_BOTTOM_ਖਿਸਕਾਓ_ਥੱਲੇ:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_ਢੇਰ_DISPLAY_STYLE_GAP_MIN:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_ਢੇਰ_GAP_MIN:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_ਢੇਰ_ਸਿਖਰ_DISPLAY_STYLE_ਖਿਸਕਾਓ_ਉੱਪਰ:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_ਢੇਰ_ਸਿਖਰ_ਖਿਸਕਾਓ_ਉੱਪਰ:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_STRETCH_ਢੇਰ_BOTTOM_ਖਿਸਕਾਓ_ਥੱਲੇ:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_STRETCH_ਢੇਰ_GAP_ABOVE_MIN:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_STRETCH_ਢੇਰ_GAP_ਹੇਠਾਂ_MIN:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_STRETCH_ਢੇਰ_ਸਿਖਰ_ਖਿਸਕਾਓ_ਉੱਪਰ:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_SUBSCRIPT_BASELINE_DROP_MIN:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_SUBSCRIPT_ਖਿਸਕਾਓ_ਥੱਲੇ:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_SUBSCRIPT_ਸਿਖਰ_MAX:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_SUB_SUPERSCRIPT_GAP_MIN:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_SUPERSCRIPT_BASELINE_DROP_MAX:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MAX_WITH_SUBSCRIPT:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MIN:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_SUPERSCRIPT_ਖਿਸਕਾਓ_ਉੱਪਰ:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_SUPERSCRIPT_ਖਿਸਕਾਓ_ਉੱਪਰ_CRAMPED:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_UNDERBAR_EXTRA_DESCENDER:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_UNDERBAR_ਨਿਯਮ_THICKNESS:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_UNDERBAR_VERTICAL_GAP:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_ਉੱਤਲਾ_ਹੱਦਬੰਦੀ_BASELINE_RISE_MIN:
    ਹਾਲਤ HB_OT_MATH_CONSTANT_ਉੱਤਲਾ_ਹੱਦਬੰਦੀ_GAP_MIN:
      ਵਾਪਸ math_ਮੁੱਲ_ਰਿਕਾਰਡਾਂ[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_y_ਮੁੱਲ (font, ਇਹ);

    ਹਾਲਤ HB_OT_MATH_CONSTANT_RADICAL_DEGREE_BOTTOM_RAISE_PERCENT:
      ਵਾਪਸ radicalDegreeBottomRaisePercent;

    ਮੂਲ:
      ਵਾਪਸ 0;
    }
  }

  ਸੁਰੱਖਿਅਤ:
  HBINT16 percent_Scale_ਥੱਲੇ[2];
  HBUINT16 min_ਉਚਾਈ[2];
  Math_ਮੁੱਲ_ਰਿਕਾਰਡ math_ਮੁੱਲ_ਰਿਕਾਰਡਾਂ[51];
  HBINT16 radicalDegreeBottomRaisePercent;

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (214);
};

ਬਣਤਰ MathItalicsCorrectionInfo
{
  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਅਚਲ hb_ਸੈੱਟ_t &glyphset = c->plan->_glyphset_mathed;
    ਅਚਲ hb_map_t &glyph_map = *c->plan->glyph_map;

    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);

    hb_sorted_ਸਦਿਸ਼_t<hb_codepoint_t> new_coverage;
    + hb_zip (ਇਹ+coverage, italicsCorrection)
    | hb_filter (glyphset, hb_ਪਹਿਲਾ)
    | hb_filter (serialize_math_ਰਿਕਾਰਡ_array (c->serializer, ਬਾਹਰਲਾ->italicsCorrection, ਇਹ, &c->plan->layout_variation_idx_delta_map), hb_ਦੂਜਾ)
    | hb_map (hb_ਪਹਿਲਾ)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    ਬਾਹਰਲਾ->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (ਸੱਚ);
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ
  {
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &ਇੰਦਰਾਜ : hb_zip (ਇਹ+coverage, italicsCorrection))
      ਜੇ (c->glyph_ਸੈੱਟ->has (hb_ਪਹਿਲਾ (ਇੰਦਰਾਜ)))
        hb_ਦੂਜਾ (ਇੰਦਰਾਜ).collect_variation_indices (c, ਇਹ);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  coverage.sanitize (c, ਇਹ) &&
		  italicsCorrection.sanitize (c, ਇਹ));
  }

  hb_position_t get_ਮੁੱਲ (hb_codepoint_t glyph,
			   hb_font_t *font) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = (ਇਹ+coverage).get_coverage (glyph);
    ਵਾਪਸ italicsCorrection[ਸੂਚਕਾਂਕ_2].get_x_ਮੁੱਲ (font, ਇਹ);
  }

  ਸੁਰੱਖਿਅਤ:
  Offset16_ਮੰਜ਼ਿਲ<Coverage>       coverage;		/* Offset to Coverage table -
						 * from the beginning of
						 * MathItalicsCorrectionInfo
						 * table. */
  Array16_ਦਾ<Math_ਮੁੱਲ_ਰਿਕਾਰਡ> italicsCorrection;	/* Array of MathValueRecords
						 * defining italics correction
						 * values for each
						 * covered glyph. */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (4, italicsCorrection);
};

ਬਣਤਰ Math_ਸਿਖਰ_Accent_Attachment
{
  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਅਚਲ hb_ਸੈੱਟ_t &glyphset = c->plan->_glyphset_mathed;
    ਅਚਲ hb_map_t &glyph_map = *c->plan->glyph_map;

    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);

    hb_sorted_ਸਦਿਸ਼_t<hb_codepoint_t> new_coverage;
    + hb_zip (ਇਹ+ਸਿਖਰ_Accent_Coverage, ਸਿਖਰ_Accent_Attachment)
    | hb_filter (glyphset, hb_ਪਹਿਲਾ)
    | hb_filter (serialize_math_ਰਿਕਾਰਡ_array (c->serializer, ਬਾਹਰਲਾ->ਸਿਖਰ_Accent_Attachment, ਇਹ, &c->plan->layout_variation_idx_delta_map), hb_ਦੂਜਾ)
    | hb_map (hb_ਪਹਿਲਾ)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    ਬਾਹਰਲਾ->ਸਿਖਰ_Accent_Coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (ਸੱਚ);
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ
  {
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &ਇੰਦਰਾਜ : hb_zip (ਇਹ+ਸਿਖਰ_Accent_Coverage, ਸਿਖਰ_Accent_Attachment))
      ਜੇ (c->glyph_ਸੈੱਟ->has (hb_ਪਹਿਲਾ (ਇੰਦਰਾਜ)))
        hb_ਦੂਜਾ (ਇੰਦਰਾਜ).collect_variation_indices (c, ਇਹ);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  ਸਿਖਰ_Accent_Coverage.sanitize (c, ਇਹ) &&
		  ਸਿਖਰ_Accent_Attachment.sanitize (c, ਇਹ));
  }

  hb_position_t get_ਮੁੱਲ (hb_codepoint_t glyph,
			   hb_font_t *font) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = (ਇਹ+ਸਿਖਰ_Accent_Coverage).get_coverage (glyph);
    ਜੇ (ਸੂਚਕਾਂਕ_2 == NOT_COVERED)
      ਵਾਪਸ font->get_glyph_h_advance (glyph) / 2;
    ਵਾਪਸ ਸਿਖਰ_Accent_Attachment[ਸੂਚਕਾਂਕ_2].get_x_ਮੁੱਲ (font, ਇਹ);
  }

  ਸੁਰੱਖਿਅਤ:
  Offset16_ਮੰਜ਼ਿਲ<Coverage>       ਸਿਖਰ_Accent_Coverage;   /* Offset to Coverage table -
						 * from the beginning of
						 * MathTopAccentAttachment
						 * table. */
  Array16_ਦਾ<Math_ਮੁੱਲ_ਰਿਕਾਰਡ> ਸਿਖਰ_Accent_Attachment; /* Array of MathValueRecords
						 * defining top accent
						 * attachment points for each
						 * covered glyph. */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (2 + 2, ਸਿਖਰ_Accent_Attachment);
};

ਬਣਤਰ MathKern
{
  MathKern* copy (hb_serialize_context_t *c,
                      ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> *variation_map) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->ਸ਼ੁਰੂ_embed (ਇਹ);

    ਜੇ (unlikely (!c->embed (ਉਚਾਈ_ਗਿਣਤੀ))) return_trace (ਖਾਲੀ_ਸੂਚਕ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = 2 * ਉਚਾਈ_ਗਿਣਤੀ + 1;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
      ਜੇ (!c->copy (math_ਮੁੱਲ_ਰਿਕਾਰਡਾਂ_Z.arrayZ[i], ਇਹ, variation_map))
        return_trace (ਖਾਲੀ_ਸੂਚਕ);

    return_trace (ਬਾਹਰਲਾ);
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ
  {
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &ਰਿਕਾਰਡ : math_ਮੁੱਲ_ਰਿਕਾਰਡਾਂ_Z.as_array (2 * ਉਚਾਈ_ਗਿਣਤੀ + 1))
      ਰਿਕਾਰਡ.collect_variation_indices (c, ਇਹ);
  }

  ਤਾਰਕਿਕ sanitize_math_ਮੁੱਲ_ਰਿਕਾਰਡਾਂ (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = 2 * ਉਚਾਈ_ਗਿਣਤੀ + 1;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      ਜੇ (!math_ਮੁੱਲ_ਰਿਕਾਰਡਾਂ_Z.arrayZ[i].sanitize (c, ਇਹ)) return_trace (ਝੂਠ);
    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  hb_barrier () &&
		  c->check_array (math_ਮੁੱਲ_ਰਿਕਾਰਡਾਂ_Z.arrayZ, 2 * ਉਚਾਈ_ਗਿਣਤੀ + 1) &&
		  sanitize_math_ਮੁੱਲ_ਰਿਕਾਰਡਾਂ (c));
  }

  hb_position_t get_ਮੁੱਲ (hb_position_t correction_ਉਚਾਈ, hb_font_t *font) ਅਚਲ
  {
    ਅਚਲ Math_ਮੁੱਲ_ਰਿਕਾਰਡ* correction_ਉਚਾਈ_2 = math_ਮੁੱਲ_ਰਿਕਾਰਡਾਂ_Z.arrayZ;
    ਅਚਲ Math_ਮੁੱਲ_ਰਿਕਾਰਡ* kern_ਮੁੱਲ = math_ਮੁੱਲ_ਰਿਕਾਰਡਾਂ_Z.arrayZ + ਉਚਾਈ_ਗਿਣਤੀ;
    ਪੂਰਨ sign = font->y_scale < 0 ? -1 : +1;

    /* According to OpenType spec (v1.9), except for the boundary cases, the index
     * chosen for kern value should be i such that
     *    correctionHeight[i-1] <= correction_height < correctionHeight[i]
     * We can use the binary search algorithm of std::upper_bound(). Or, we can
     * use the internal hb_bsearch_impl.
     */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pos;
    ਸਵੈਚਾਲਿਤ cmp = +[](ਅਚਲ ਖਾਲੀ* key, ਅਚਲ ਖਾਲੀ* p,
                   ਪੂਰਨ sign, hb_font_t* font, ਅਚਲ MathKern* mathKern) -> ਪੂਰਨ {
      hb_position_t a = *(hb_position_t *) key;
      hb_position_t b = ((Math_ਮੁੱਲ_ਰਿਕਾਰਡ *) p)->get_y_ਮੁੱਲ (font, mathKern);
      ਜੇ (a == b) ਵਾਪਸ 0;
      ਵਾਪਸ sign > 0 ? (a < b ? -1 : +1) : (b < a ? -1 : +1);
    };
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = hb_bsearch_impl(&pos, correction_ਉਚਾਈ, correction_ਉਚਾਈ_2,
                                     ਉਚਾਈ_ਗਿਣਤੀ, Math_ਮੁੱਲ_ਰਿਕਾਰਡ::static_ਮਾਪ,
                                     cmp, sign, font, ਇਹ) ? pos + 1 : pos;
    ਵਾਪਸ kern_ਮੁੱਲ[i].get_x_ਮੁੱਲ (font, ਇਹ);
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_entries (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਵਿੱਥ,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *entries_ਗਿਣਤੀ, /* IN/OUT */
			    hb_ot_math_kern_ਇੰਦਰਾਜ_t *kern_entries, /*ਬਾਹਰਲਾ*/
			    hb_font_t *font) ਅਚਲ
  {
    ਅਚਲ Math_ਮੁੱਲ_ਰਿਕਾਰਡ* correction_ਉਚਾਈ_2 = math_ਮੁੱਲ_ਰਿਕਾਰਡਾਂ_Z.arrayZ;
    ਅਚਲ Math_ਮੁੱਲ_ਰਿਕਾਰਡ* kern_ਮੁੱਲ = math_ਮੁੱਲ_ਰਿਕਾਰਡਾਂ_Z.arrayZ + ਉਚਾਈ_ਗਿਣਤੀ;
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ entries_ਗਿਣਤੀ_2 = ਉਚਾਈ_ਗਿਣਤੀ + 1;

    ਜੇ (entries_ਗਿਣਤੀ && kern_entries)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ = hb_min (ਸ਼ੁਰੂ_ਵਿੱਥ, entries_ਗਿਣਤੀ_2);
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ = hb_min (ਸ਼ੁਰੂ + *entries_ਗਿਣਤੀ, entries_ਗਿਣਤੀ_2);
      *entries_ਗਿਣਤੀ = ਸਿਰਾ - ਸ਼ੁਰੂ;

      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < *entries_ਗਿਣਤੀ; i++) {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = ਸ਼ੁਰੂ + i;

	hb_position_t max_ਉਚਾਈ;
	ਜੇ (j == ਉਚਾਈ_ਗਿਣਤੀ) {
	  max_ਉਚਾਈ = INT32_MAX;
	} ਨਹੀਂਤਾਂ {
	  max_ਉਚਾਈ = correction_ਉਚਾਈ_2[j].get_y_ਮੁੱਲ (font, ਇਹ);
	}

	kern_entries[i] = {max_ਉਚਾਈ, kern_ਮੁੱਲ[j].get_x_ਮੁੱਲ (font, ਇਹ)};
      }
    }
    ਵਾਪਸ entries_ਗਿਣਤੀ_2;
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	ਉਚਾਈ_ਗਿਣਤੀ;
  Unsized_Array_ਦਾ<Math_ਮੁੱਲ_ਰਿਕਾਰਡ>
		math_ਮੁੱਲ_ਰਿਕਾਰਡਾਂ_Z;
				/* Array of correction heights at
				 * which the kern value changes.
				 * Sorted by the height value in
				 * design units (heightCount entries),
				 * Followed by:
				 * Array of kern values corresponding
				 * to heights. (heightCount+1 entries).
				 */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (2, math_ਮੁੱਲ_ਰਿਕਾਰਡਾਂ_Z);
};

ਬਣਤਰ Math_Kern_Info_ਰਿਕਾਰਡ
{
  Math_Kern_Info_ਰਿਕਾਰਡ* copy (hb_serialize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
                            ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> *variation_map) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਖਾਲੀ_ਸੂਚਕ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = ARRAY_ਲੰਮਾਈ (mathKern);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
      ਬਾਹਰਲਾ->mathKern[i].serialize_copy (c, mathKern[i], ਅਧਾਰ, 0, hb_serialize_context_t::ਸਿਰ, variation_map);

    return_trace (ਬਾਹਰਲਾ);
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &ਵਿੱਥ : mathKern)
      (ਅਧਾਰ+ਵਿੱਥ).collect_variation_indices (c);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ARRAY_ਲੰਮਾਈ (mathKern);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      ਜੇ (unlikely (!mathKern[i].sanitize (c, ਅਧਾਰ)))
	return_trace (ਝੂਠ);

    return_trace (ਸੱਚ);
  }

  hb_position_t get_kerning (hb_ot_math_kern_t kern,
			     hb_position_t correction_ਉਚਾਈ,
			     hb_font_t *font,
			     ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ idx = kern;
    ਜੇ (unlikely (idx >= ARRAY_ਲੰਮਾਈ (mathKern))) ਵਾਪਸ 0;
    ਵਾਪਸ (ਅਧਾਰ+mathKern[idx]).get_ਮੁੱਲ (correction_ਉਚਾਈ, font);
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_kernings (hb_ot_math_kern_t kern,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਵਿੱਥ,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *entries_ਗਿਣਤੀ, /* IN/OUT */
			     hb_ot_math_kern_ਇੰਦਰਾਜ_t *kern_entries, /*ਬਾਹਰਲਾ*/
			     hb_font_t *font,
			     ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ idx = kern;
    ਜੇ (unlikely (idx >= ARRAY_ਲੰਮਾਈ (mathKern)) || !mathKern[idx]) {
      ਜੇ (entries_ਗਿਣਤੀ) *entries_ਗਿਣਤੀ = 0;
      ਵਾਪਸ 0;
    }
    ਵਾਪਸ (ਅਧਾਰ+mathKern[idx]).get_entries (ਸ਼ੁਰੂ_ਵਿੱਥ,
					     entries_ਗਿਣਤੀ,
					     kern_entries,
					     font);
  }

  ਸੁਰੱਖਿਅਤ:
  /* Offset to MathKern table for each corner -
   * from the beginning of MathKernInfo table.  May be NULL. */
  Offset16_ਮੰਜ਼ਿਲ<MathKern> mathKern[4];

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (8);
};

ਬਣਤਰ MathKernInfo
{
  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਅਚਲ hb_ਸੈੱਟ_t &glyphset = c->plan->_glyphset_mathed;
    ਅਚਲ hb_map_t &glyph_map = *c->plan->glyph_map;

    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);

    hb_sorted_ਸਦਿਸ਼_t<hb_codepoint_t> new_coverage;
    + hb_zip (ਇਹ+mathKernCoverage, math_Kern_Info_ਰਿਕਾਰਡਾਂ)
    | hb_filter (glyphset, hb_ਪਹਿਲਾ)
    | hb_filter (serialize_math_ਰਿਕਾਰਡ_array (c->serializer, ਬਾਹਰਲਾ->math_Kern_Info_ਰਿਕਾਰਡਾਂ, ਇਹ, &c->plan->layout_variation_idx_delta_map), hb_ਦੂਜਾ)
    | hb_map (hb_ਪਹਿਲਾ)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    ਬਾਹਰਲਾ->mathKernCoverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (ਸੱਚ);
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ
  {
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &ਇੰਦਰਾਜ : hb_zip (ਇਹ+mathKernCoverage, math_Kern_Info_ਰਿਕਾਰਡਾਂ))
      ਜੇ (c->glyph_ਸੈੱਟ->has (hb_ਪਹਿਲਾ (ਇੰਦਰਾਜ)))
        hb_ਦੂਜਾ (ਇੰਦਰਾਜ).collect_variation_indices (c, ਇਹ);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  mathKernCoverage.sanitize (c, ਇਹ) &&
		  math_Kern_Info_ਰਿਕਾਰਡਾਂ.sanitize (c, ਇਹ));
  }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_ਉਚਾਈ,
			     hb_font_t *font) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = (ਇਹ+mathKernCoverage).get_coverage (glyph);
    ਵਾਪਸ math_Kern_Info_ਰਿਕਾਰਡਾਂ[ਸੂਚਕਾਂਕ_2].get_kerning (kern, correction_ਉਚਾਈ, font, ਇਹ);
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_kernings (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਵਿੱਥ,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *entries_ਗਿਣਤੀ, /* IN/OUT */
			     hb_ot_math_kern_ਇੰਦਰਾਜ_t *kern_entries, /*ਬਾਹਰਲਾ*/
			     hb_font_t *font) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = (ਇਹ+mathKernCoverage).get_coverage (glyph);
    ਵਾਪਸ math_Kern_Info_ਰਿਕਾਰਡਾਂ[ਸੂਚਕਾਂਕ_2].get_kernings (kern,
						    ਸ਼ੁਰੂ_ਵਿੱਥ,
						    entries_ਗਿਣਤੀ,
						    kern_entries,
						    font,
						    ਇਹ);
  }

  ਸੁਰੱਖਿਅਤ:
  Offset16_ਮੰਜ਼ਿਲ<Coverage>
		mathKernCoverage;
				/* Offset to Coverage table -
				 * from the beginning of the
				 * MathKernInfo table. */
  Array16_ਦਾ<Math_Kern_Info_ਰਿਕਾਰਡ>
		math_Kern_Info_ਰਿਕਾਰਡਾਂ;
				/* Array of MathKernInfoRecords,
				 * per-glyph information for
				 * mathematical positioning
				 * of subscripts and
				 * superscripts. */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (4, math_Kern_Info_ਰਿਕਾਰਡਾਂ);
};

ਬਣਤਰ MathGlyphInfo
{
  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (*ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਬਾਹਰਲਾ->mathItalicsCorrectionInfo.serialize_subset (c, mathItalicsCorrectionInfo, ਇਹ);
    ਬਾਹਰਲਾ->math_ਸਿਖਰ_Accent_Attachment.serialize_subset (c, math_ਸਿਖਰ_Accent_Attachment, ਇਹ);

    ਅਚਲ hb_ਸੈੱਟ_t &glyphset = c->plan->_glyphset_mathed;
    ਅਚਲ hb_map_t &glyph_map = *c->plan->glyph_map;

    ਸਵੈਚਾਲਿਤ it =
    + hb_iter (ਇਹ+extendedShapeCoverage)
    | hb_take (c->plan->ਸਰੋਤ->get_ਅੰਕ_glyphs ())
    | hb_filter (glyphset)
    | hb_map_retains_ਛਾਂਟੀ (glyph_map)
    ;

    ਜੇ (it) ਬਾਹਰਲਾ->extendedShapeCoverage.serialize_serialize (c->serializer, it);
    ਨਹੀਂਤਾਂ ਬਾਹਰਲਾ->extendedShapeCoverage = 0;

    ਬਾਹਰਲਾ->mathKernInfo.serialize_subset (c, mathKernInfo, ਇਹ);
    return_trace (ਸੱਚ);
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ
  {
    (ਇਹ+mathItalicsCorrectionInfo).collect_variation_indices (c);
    (ਇਹ+math_ਸਿਖਰ_Accent_Attachment).collect_variation_indices (c);
    (ਇਹ+mathKernInfo).collect_variation_indices (c);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  mathItalicsCorrectionInfo.sanitize (c, ਇਹ) &&
		  math_ਸਿਖਰ_Accent_Attachment.sanitize (c, ਇਹ) &&
		  extendedShapeCoverage.sanitize (c, ਇਹ) &&
		  mathKernInfo.sanitize (c, ਇਹ));
  }

  hb_position_t
  get_italics_correction (hb_codepoint_t  glyph, hb_font_t *font) ਅਚਲ
  { ਵਾਪਸ (ਇਹ+mathItalicsCorrectionInfo).get_ਮੁੱਲ (glyph, font); }

  hb_position_t
  get_ਸਿਖਰ_accent_attachment (hb_codepoint_t  glyph, hb_font_t *font) ਅਚਲ
  { ਵਾਪਸ (ਇਹ+math_ਸਿਖਰ_Accent_Attachment).get_ਮੁੱਲ (glyph, font); }

  ਤਾਰਕਿਕ is_extended_shape (hb_codepoint_t glyph) ਅਚਲ
  { ਵਾਪਸ (ਇਹ+extendedShapeCoverage).get_coverage (glyph) != NOT_COVERED; }

  hb_position_t get_kerning (hb_codepoint_t glyph,
			     hb_ot_math_kern_t kern,
			     hb_position_t correction_ਉਚਾਈ,
			     hb_font_t *font) ਅਚਲ
  { ਵਾਪਸ (ਇਹ+mathKernInfo).get_kerning (glyph, kern, correction_ਉਚਾਈ, font); }

  hb_position_t get_kernings (hb_codepoint_t glyph,
			      hb_ot_math_kern_t kern,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਵਿੱਥ,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *entries_ਗਿਣਤੀ, /* IN/OUT */
			      hb_ot_math_kern_ਇੰਦਰਾਜ_t *kern_entries, /*ਬਾਹਰਲਾ*/
			      hb_font_t *font) ਅਚਲ
  { ਵਾਪਸ (ਇਹ+mathKernInfo).get_kernings (glyph,
					     kern,
					     ਸ਼ੁਰੂ_ਵਿੱਥ,
					     entries_ਗਿਣਤੀ,
					     kern_entries,
					     font); }

  ਸੁਰੱਖਿਅਤ:
  /* Offset to MathItalicsCorrectionInfo table -
   * from the beginning of MathGlyphInfo table. */
  Offset16_ਮੰਜ਼ਿਲ<MathItalicsCorrectionInfo> mathItalicsCorrectionInfo;

  /* Offset to MathTopAccentAttachment table -
   * from the beginning of MathGlyphInfo table. */
  Offset16_ਮੰਜ਼ਿਲ<Math_ਸਿਖਰ_Accent_Attachment> math_ਸਿਖਰ_Accent_Attachment;

  /* Offset to coverage table for Extended Shape glyphs -
   * from the beginning of MathGlyphInfo table. When the left or right glyph of
   * a box is an extended shape variant, the (ink) box (and not the default
   * position defined by values in MathConstants table) should be used for
   * vertical positioning purposes.  May be NULL.. */
  Offset16_ਮੰਜ਼ਿਲ<Coverage> extendedShapeCoverage;

   /* Offset to MathKernInfo table -
    * from the beginning of MathGlyphInfo table. */
  Offset16_ਮੰਜ਼ਿਲ<MathKernInfo> mathKernInfo;

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (8);
};

ਬਣਤਰ Math_Glyph_Variant_ਰਿਕਾਰਡ
{
  ਮਿੱਤਰ ਬਣਤਰ MathGlyphConstruction;

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਅਚਲ hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->variantGlyph, glyph_map.get (variantGlyph), HB_SERIALIZE_ERROR_INT_ਉਛਾਲ));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਖਾਲੀ depend (hb_depend_ਅੰਕੜੇ_builder_t *depend_ਅੰਕੜੇ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਰੋਤ) ਅਚਲ
  { depend_ਅੰਕੜੇ->ਜੋੜੋ_depend(ਸਰੋਤ, HB_OT_ਟੈਗ_MATH, variantGlyph); }

  ਖਾਲੀ ਘੇਰਾ_glyphs (hb_ਸੈੱਟ_t *variant_glyphs) ਅਚਲ
  { variant_glyphs->ਜੋੜੋ (variantGlyph); }

  ਸੁਰੱਖਿਅਤ:
  HBGlyphID16 variantGlyph;       /* Glyph ID for the variant. */
  HBUINT16  advanceMeasurement; /* Advance width/height, in design units, of the
				 * variant, in the direction of requested
				 * glyph extension. */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4);
};

ਬਣਤਰ PartFlags : HBUINT16
{
  ਗਿਣਤੀ Flags {
    Extender	= 0x0001u, /* If set, the part can be skipped or repeated. */

    Defined	= 0x0001u, /* All defined flags. */
  };

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (2);
};

ਬਣਤਰ Math_Glyph_Part_ਰਿਕਾਰਡ
{
  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਅਚਲ hb_map_t& glyph_map = *c->plan->glyph_map;
    return_trace (c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->glyph, glyph_map.get (glyph), HB_SERIALIZE_ERROR_INT_ਉਛਾਲ));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਖਾਲੀ extract (hb_ot_math_glyph_part_t &ਬਾਹਰਲਾ,
		int64_t mult,
		hb_font_t *font) ਅਚਲ
  {
    ਬਾਹਰਲਾ.glyph			= glyph;

    ਬਾਹਰਲਾ.ਸ਼ੁਰੂ_connector_ਲੰਮਾਈ	= font->em_mult (ਸ਼ੁਰੂ_Connector_ਲੰਮਾਈ, mult);
    ਬਾਹਰਲਾ.ਸਿਰਾ_connector_ਲੰਮਾਈ	= font->em_mult (ਸਿਰਾ_Connector_ਲੰਮਾਈ, mult);
    ਬਾਹਰਲਾ.ਭਰਿਆ_advance		= font->em_mult (ਭਰਿਆ_Advance, mult);

    static_assert ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) HB_OT_MATH_GLYPH_PART_FLAG_EXTENDER ==
		   (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) PartFlags::Extender, "");

    ਬਾਹਰਲਾ.flags = (hb_ot_math_glyph_part_flags_t)
		(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)
		(partFlags & PartFlags::Defined);
  }

  ਖਾਲੀ depend (hb_depend_ਅੰਕੜੇ_builder_t *depend_ਅੰਕੜੇ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਰੋਤ) ਅਚਲ
  { depend_ਅੰਕੜੇ->ਜੋੜੋ_depend(ਸਰੋਤ, HB_OT_ਟੈਗ_MATH, glyph); }

  ਖਾਲੀ ਘੇਰਾ_glyphs (hb_ਸੈੱਟ_t *variant_glyphs) ਅਚਲ
  { variant_glyphs->ਜੋੜੋ (glyph); }

  ਸੁਰੱਖਿਅਤ:
  HBGlyphID16	glyph;		/* Glyph ID for the part. */
  HBUINT16	ਸ਼ੁਰੂ_Connector_ਲੰਮਾਈ;
				/* Advance width/ height of the straight bar
				 * connector material, in design units, is at
				 * the beginning of the glyph, in the
				 * direction of the extension. */
  HBUINT16	ਸਿਰਾ_Connector_ਲੰਮਾਈ;
				/* Advance width/ height of the straight bar
				 * connector material, in design units, is at
				 * the end of the glyph, in the direction of
				 * the extension. */
  HBUINT16	ਭਰਿਆ_Advance;	/* Full advance width/height for this part,
				 * in the direction of the extension.
				 * In design units. */
  PartFlags	partFlags;	/* Part qualifiers. */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (10);
};

ਬਣਤਰ MathGlyphAssembly
{
  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);

    ਜੇ (!c->serializer->copy (italicsCorrection, ਇਹ, &c->plan->layout_variation_idx_delta_map)) return_trace (ਝੂਠ);
    ਜੇ (!c->serializer->copy<HBUINT16> (part_ਰਿਕਾਰਡਾਂ.len)) return_trace (ਝੂਠ);

    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& ਰਿਕਾਰਡ : part_ਰਿਕਾਰਡਾਂ.iter ())
      ਜੇ (!ਰਿਕਾਰਡ.subset (c)) return_trace (ਝੂਠ);
    return_trace (ਸੱਚ);
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ
  {
    italicsCorrection.collect_variation_indices (c, ਇਹ);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  italicsCorrection.sanitize (c, ਇਹ) &&
		  part_ਰਿਕਾਰਡਾਂ.sanitize (c));
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_parts (hb_direction_t direction,
			  hb_font_t *font,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਵਿੱਥ,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *parts_ਗਿਣਤੀ, /* IN/OUT */
			  hb_ot_math_glyph_part_t *parts /*ਬਾਹਰਲਾ*/,
			  hb_position_t *italics_correction /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਜੇ (parts_ਗਿਣਤੀ && parts)
    {
      int64_t mult = font->dir_mult (direction);
      ਲਈ (ਸਵੈਚਾਲਿਤ _ : hb_zip (part_ਰਿਕਾਰਡਾਂ.as_array ().sub_array (ਸ਼ੁਰੂ_ਵਿੱਥ, parts_ਗਿਣਤੀ),
			    hb_array (parts, *parts_ਗਿਣਤੀ)))
	_.ਪਹਿਲਾ.extract (_.second, mult, font);
    }

    ਜੇ (italics_correction)
      *italics_correction = italicsCorrection.get_x_ਮੁੱਲ (font, ਇਹ);

    ਵਾਪਸ part_ਰਿਕਾਰਡਾਂ.len;
  }

  ਖਾਲੀ depend (hb_depend_ਅੰਕੜੇ_builder_t *depend_ਅੰਕੜੇ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਰੋਤ) ਅਚਲ
  {
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& _ : part_ਰਿਕਾਰਡਾਂ.iter ())
      _.depend (depend_ਅੰਕੜੇ, ਸਰੋਤ);
  }

  ਖਾਲੀ ਘੇਰਾ_glyphs (hb_ਸੈੱਟ_t *variant_glyphs) ਅਚਲ
  {
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& _ : part_ਰਿਕਾਰਡਾਂ.iter ())
      _.ਘੇਰਾ_glyphs (variant_glyphs);
  }

  ਸੁਰੱਖਿਅਤ:
  Math_ਮੁੱਲ_ਰਿਕਾਰਡ
		italicsCorrection;
				/* Italics correction of this
				 * MathGlyphAssembly. Should not
				 * depend on the assembly size. */
  Array16_ਦਾ<Math_Glyph_Part_ਰਿਕਾਰਡ>
		part_ਰਿਕਾਰਡਾਂ;	/* Array of part records, from
				 * left to right and bottom to
				 * top. */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (6, part_ਰਿਕਾਰਡਾਂ);
};

ਬਣਤਰ MathGlyphConstruction
{
  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);

    ਬਾਹਰਲਾ->glyphAssembly.serialize_subset (c, glyphAssembly, ਇਹ);

    ਜੇ (!c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->math_Glyph_Variant_ਰਿਕਾਰਡ.len, math_Glyph_Variant_ਰਿਕਾਰਡ.len, HB_SERIALIZE_ERROR_INT_ਉਛਾਲ))
      return_trace (ਝੂਠ);
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& ਰਿਕਾਰਡ : math_Glyph_Variant_ਰਿਕਾਰਡ.iter ())
      ਜੇ (!ਰਿਕਾਰਡ.subset (c)) return_trace (ਝੂਠ);

    return_trace (ਸੱਚ);
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ
  {
    (ਇਹ+glyphAssembly).collect_variation_indices (c);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  glyphAssembly.sanitize (c, ਇਹ) &&
		  math_Glyph_Variant_ਰਿਕਾਰਡ.sanitize (c));
  }

  ਅਚਲ MathGlyphAssembly &get_assembly () ਅਚਲ { ਵਾਪਸ ਇਹ+glyphAssembly; }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_variants (hb_direction_t direction,
			     hb_font_t *font,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਵਿੱਥ,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *variants_ਗਿਣਤੀ, /* IN/OUT */
			     hb_ot_math_glyph_variant_t *variants /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਜੇ (variants_ਗਿਣਤੀ && variants)
    {
      int64_t mult = font->dir_mult (direction);
      ਲਈ (ਸਵੈਚਾਲਿਤ _ : hb_zip (math_Glyph_Variant_ਰਿਕਾਰਡ.as_array ().sub_array (ਸ਼ੁਰੂ_ਵਿੱਥ, variants_ਗਿਣਤੀ),
			    hb_array (variants, *variants_ਗਿਣਤੀ)))
	_.second = {_.ਪਹਿਲਾ.variantGlyph, font->em_mult (_.ਪਹਿਲਾ.advanceMeasurement, mult)};
    }
    ਵਾਪਸ math_Glyph_Variant_ਰਿਕਾਰਡ.len;
  }

  ਖਾਲੀ depend (hb_depend_ਅੰਕੜੇ_builder_t *depend_ਅੰਕੜੇ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਰੋਤ) ਅਚਲ
  {
    (ਇਹ+glyphAssembly).depend (depend_ਅੰਕੜੇ, ਸਰੋਤ);

    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& _ : math_Glyph_Variant_ਰਿਕਾਰਡ.iter ())
      _.depend (depend_ਅੰਕੜੇ, ਸਰੋਤ);
  }

  ਖਾਲੀ ਘੇਰਾ_glyphs (hb_ਸੈੱਟ_t *variant_glyphs) ਅਚਲ
  {
    (ਇਹ+glyphAssembly).ਘੇਰਾ_glyphs (variant_glyphs);

    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& _ : math_Glyph_Variant_ਰਿਕਾਰਡ.iter ())
      _.ਘੇਰਾ_glyphs (variant_glyphs);
  }

  ਸੁਰੱਖਿਅਤ:
  /* Offset to MathGlyphAssembly table for this shape - from the beginning of
     MathGlyphConstruction table.  May be NULL. */
  Offset16_ਮੰਜ਼ਿਲ<MathGlyphAssembly>	  glyphAssembly;

  /* MathGlyphVariantRecords for alternative variants of the glyphs. */
  Array16_ਦਾ<Math_Glyph_Variant_ਰਿਕਾਰਡ> math_Glyph_Variant_ਰਿਕਾਰਡ;

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (4, math_Glyph_Variant_ਰਿਕਾਰਡ);
};

ਬਣਤਰ MathVariants
{
  ਖਾਲੀ depend (hb_depend_ਅੰਕੜੇ_builder_t *depend_ਅੰਕੜੇ) ਅਚਲ
  {
    ਅਚਲ hb_array_t<ਅਚਲ Offset16_ਮੰਜ਼ਿਲ<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vert_Glyph_ਗਿਣਤੀ + horiz_Glyph_ਗਿਣਤੀ);

    ਜੇ (vertGlyphCoverage)
    {
      ਅਚਲ ਸਵੈਚਾਲਿਤ vert_offsets = glyph_construction_offsets.sub_array (0, vert_Glyph_ਗਿਣਤੀ);
      + hb_zip (ਇਹ+vertGlyphCoverage, vert_offsets)
      | hb_apply ([&] (ਅਚਲ hb_pair_t<hb_codepoint_t, ਅਚਲ Offset16_ਮੰਜ਼ਿਲ<MathGlyphConstruction>&> &_)
                  {
                    ਅਚਲ MathGlyphConstruction &mgc = ਇਹ+_.second;
                    mgc.depend (depend_ਅੰਕੜੇ, _.ਪਹਿਲਾ);
                  })
      ;
    }
    ਜੇ (horizGlyphCoverage)
    {
      ਅਚਲ ਸਵੈਚਾਲਿਤ hori_offsets = glyph_construction_offsets.sub_array (vert_Glyph_ਗਿਣਤੀ, horiz_Glyph_ਗਿਣਤੀ);
      + hb_zip (ਇਹ+horizGlyphCoverage, hori_offsets)
      | hb_apply ([&] (ਅਚਲ hb_pair_t<hb_codepoint_t, ਅਚਲ Offset16_ਮੰਜ਼ਿਲ<MathGlyphConstruction>&> &_)
                  {
                    ਅਚਲ MathGlyphConstruction &mgc = ਇਹ+_.second;
                    mgc.depend (depend_ਅੰਕੜੇ, _.ਪਹਿਲਾ);
                  })
      ;
    }
  }

  ਖਾਲੀ ਘੇਰਾ_glyphs (ਅਚਲ hb_ਸੈੱਟ_t *glyph_ਸੈੱਟ,
                       hb_ਸੈੱਟ_t *variant_glyphs) ਅਚਲ
  {
    ਅਚਲ hb_array_t<ਅਚਲ Offset16_ਮੰਜ਼ਿਲ<MathGlyphConstruction>> glyph_construction_offsets = glyphConstruction.as_array (vert_Glyph_ਗਿਣਤੀ + horiz_Glyph_ਗਿਣਤੀ);

    ਜੇ (vertGlyphCoverage)
    {
      ਅਚਲ ਸਵੈਚਾਲਿਤ vert_offsets = glyph_construction_offsets.sub_array (0, vert_Glyph_ਗਿਣਤੀ);
      + hb_zip (ਇਹ+vertGlyphCoverage, vert_offsets)
      | hb_filter (glyph_ਸੈੱਟ, hb_ਪਹਿਲਾ)
      | hb_map (hb_ਦੂਜਾ)
      | hb_map (hb_ਜੋੜੋ (ਇਹ))
      | hb_apply ([=] (ਅਚਲ MathGlyphConstruction &_) { _.ਘੇਰਾ_glyphs (variant_glyphs); })
      ;
    }

    ਜੇ (horizGlyphCoverage)
    {
      ਅਚਲ ਸਵੈਚਾਲਿਤ hori_offsets = glyph_construction_offsets.sub_array (vert_Glyph_ਗਿਣਤੀ, horiz_Glyph_ਗਿਣਤੀ);
      + hb_zip (ਇਹ+horizGlyphCoverage, hori_offsets)
      | hb_filter (glyph_ਸੈੱਟ, hb_ਪਹਿਲਾ)
      | hb_map (hb_ਦੂਜਾ)
      | hb_map (hb_ਜੋੜੋ (ਇਹ))
      | hb_apply ([=] (ਅਚਲ MathGlyphConstruction &_) { _.ਘੇਰਾ_glyphs (variant_glyphs); })
      ;
    }
  }

  ਖਾਲੀ collect_coverage_and_indices (hb_sorted_ਸਦਿਸ਼_t<hb_codepoint_t>& new_coverage,
                                     ਅਚਲ Offset16_ਮੰਜ਼ਿਲ<Coverage>& coverage,
                                     ਬਿਨਾਨਿਸ਼ਾਨੀ i,
                                     ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ_ਸੂਚਕਾਂਕ,
                                     hb_ਸੈੱਟ_t& indices,
                                     ਅਚਲ hb_ਸੈੱਟ_t& glyphset,
                                     ਅਚਲ hb_map_t& glyph_map) ਅਚਲ
  {
    ਜੇ (!coverage) ਵਾਪਸ;

    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ _ : (ਇਹ+coverage).iter ())
    {
      ਜੇ (i >= ਸਿਰਾ_ਸੂਚਕਾਂਕ) ਵਾਪਸ;
      ਜੇ (glyphset.has (_))
      {
        ਬਿਨਾਨਿਸ਼ਾਨੀ new_gid = glyph_map.get (_);
        new_coverage.push (new_gid);
        indices.ਜੋੜੋ (i);
      }
      i++;
    }
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਅਚਲ hb_ਸੈੱਟ_t &glyphset = c->plan->_glyphset_cmaped;
    ਅਚਲ hb_map_t &glyph_map = *c->plan->glyph_map;

    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);
    ਜੇ (!c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->minConnectorOverlap, minConnectorOverlap, HB_SERIALIZE_ERROR_INT_ਉਛਾਲ))
      return_trace (ਝੂਠ);

    hb_sorted_ਸਦਿਸ਼_t<hb_codepoint_t> new_vert_coverage;
    hb_sorted_ਸਦਿਸ਼_t<hb_codepoint_t> new_hori_coverage;
    hb_ਸੈੱਟ_t indices;
    collect_coverage_and_indices (new_vert_coverage, vertGlyphCoverage, 0, vert_Glyph_ਗਿਣਤੀ, indices, glyphset, glyph_map);
    collect_coverage_and_indices (new_hori_coverage, horizGlyphCoverage, vert_Glyph_ਗਿਣਤੀ, vert_Glyph_ਗਿਣਤੀ + horiz_Glyph_ਗਿਣਤੀ, indices, glyphset, glyph_map);

    ਜੇ (!c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->vert_Glyph_ਗਿਣਤੀ, new_vert_coverage.ਲੰਮਾਈ, HB_SERIALIZE_ERROR_INT_ਉਛਾਲ))
      return_trace (ਝੂਠ);
    ਜੇ (!c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->horiz_Glyph_ਗਿਣਤੀ, new_hori_coverage.ਲੰਮਾਈ, HB_SERIALIZE_ERROR_INT_ਉਛਾਲ))
      return_trace (ਝੂਠ);

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i : indices.iter ())
    {
      ਸਵੈਚਾਲਿਤ *o = c->serializer->embed (glyphConstruction[i]);
      ਜੇ (!o) return_trace (ਝੂਠ);
      o->serialize_subset (c, glyphConstruction[i], ਇਹ);
    }

    ਜੇ (new_vert_coverage)
      ਬਾਹਰਲਾ->vertGlyphCoverage.serialize_serialize (c->serializer, new_vert_coverage.iter ());

    ਜੇ (new_hori_coverage)
    ਬਾਹਰਲਾ->horizGlyphCoverage.serialize_serialize (c->serializer, new_hori_coverage.iter ());
    return_trace (ਸੱਚ);
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ
  {
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &ਇੰਦਰਾਜ : hb_zip (ਇਹ+vertGlyphCoverage, glyphConstruction.as_array (vert_Glyph_ਗਿਣਤੀ)))
      ਜੇ (c->glyph_ਸੈੱਟ->has (hb_ਪਹਿਲਾ (ਇੰਦਰਾਜ)))
        (ਇਹ+hb_ਦੂਜਾ (ਇੰਦਰਾਜ)).collect_variation_indices (c);
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &ਇੰਦਰਾਜ : hb_zip (ਇਹ+horizGlyphCoverage, glyphConstruction.as_array (vert_Glyph_ਗਿਣਤੀ + horiz_Glyph_ਗਿਣਤੀ).sub_array (vert_Glyph_ਗਿਣਤੀ)))
      ਜੇ (c->glyph_ਸੈੱਟ->has (hb_ਪਹਿਲਾ (ਇੰਦਰਾਜ)))
        (ਇਹ+hb_ਦੂਜਾ (ਇੰਦਰਾਜ)).collect_variation_indices (c);
  }

  ਤਾਰਕਿਕ sanitize_offsets (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = vert_Glyph_ਗਿਣਤੀ + horiz_Glyph_ਗਿਣਤੀ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      ਜੇ (!glyphConstruction.arrayZ[i].sanitize (c, ਇਹ)) return_trace (ਝੂਠ);
    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  vertGlyphCoverage.sanitize (c, ਇਹ) &&
		  horizGlyphCoverage.sanitize (c, ਇਹ) &&
		  hb_barrier () &&
		  c->check_array (glyphConstruction.arrayZ, vert_Glyph_ਗਿਣਤੀ + horiz_Glyph_ਗਿਣਤੀ) &&
		  sanitize_offsets (c));
  }

  hb_position_t get_min_connector_overlap (hb_direction_t direction,
						  hb_font_t *font) ਅਚਲ
  { ਵਾਪਸ font->em_scale_dir (minConnectorOverlap, direction); }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_glyph_variants (hb_codepoint_t glyph,
				   hb_direction_t direction,
				   hb_font_t *font,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਵਿੱਥ,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *variants_ਗਿਣਤੀ, /* IN/OUT */
				   hb_ot_math_glyph_variant_t *variants /*ਬਾਹਰਲਾ*/) ਅਚਲ
  { ਵਾਪਸ get_glyph_construction (glyph, direction, font)
	   .get_variants (direction, font, ਸ਼ੁਰੂ_ਵਿੱਥ, variants_ਗਿਣਤੀ, variants); }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_glyph_parts (hb_codepoint_t glyph,
				hb_direction_t direction,
				hb_font_t *font,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਵਿੱਥ,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *parts_ਗਿਣਤੀ, /* IN/OUT */
				hb_ot_math_glyph_part_t *parts /*ਬਾਹਰਲਾ*/,
				hb_position_t *italics_correction /*ਬਾਹਰਲਾ*/) ਅਚਲ
  { ਵਾਪਸ get_glyph_construction (glyph, direction, font)
	   .get_assembly ()
	   .get_parts (direction, font,
		       ਸ਼ੁਰੂ_ਵਿੱਥ, parts_ਗਿਣਤੀ, parts,
		       italics_correction); }

  ਨਿਜੀ:
  ਅਚਲ MathGlyphConstruction &
  get_glyph_construction (hb_codepoint_t glyph,
			  hb_direction_t direction,
			  hb_font_t *font HB_ਅਣਵਰਤਿਆ) ਅਚਲ
  {
    ਤਾਰਕਿਕ vertical = HB_DIRECTION_IS_VERTICAL (direction);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = vertical ? vert_Glyph_ਗਿਣਤੀ : horiz_Glyph_ਗਿਣਤੀ;
    ਅਚਲ Offset16_ਮੰਜ਼ਿਲ<Coverage> &coverage = vertical ? vertGlyphCoverage
						  : horizGlyphCoverage;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = (ਇਹ+coverage).get_coverage (glyph);
    ਜੇ (unlikely (ਸੂਚਕਾਂਕ_2 >= ਗਿਣਤੀ_2)) ਵਾਪਸ Null (MathGlyphConstruction);

    ਜੇ (!vertical)
      ਸੂਚਕਾਂਕ_2 += vert_Glyph_ਗਿਣਤੀ;

    ਵਾਪਸ ਇਹ+glyphConstruction[ਸੂਚਕਾਂਕ_2];
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	minConnectorOverlap;
				/* Minimum overlap of connecting
				 * glyphs during glyph construction,
				 * in design units. */
  Offset16_ਮੰਜ਼ਿਲ<Coverage> vertGlyphCoverage;
				/* Offset to Coverage table -
				 * from the beginning of MathVariants
				 * table. */
  Offset16_ਮੰਜ਼ਿਲ<Coverage> horizGlyphCoverage;
				/* Offset to Coverage table -
				 * from the beginning of MathVariants
				 * table. */
  HBUINT16	vert_Glyph_ਗਿਣਤੀ;	/* Number of glyphs for which
				 * information is provided for
				 * vertically growing variants. */
  HBUINT16	horiz_Glyph_ਗਿਣਤੀ;/* Number of glyphs for which
				 * information is provided for
				 * horizontally growing variants. */

  /* Array of offsets to MathGlyphConstruction tables - from the beginning of
     the MathVariants table, for shapes growing in vertical/horizontal
     direction. */
  Unsized_Array_ਦਾ<Offset16_ਮੰਜ਼ਿਲ<MathGlyphConstruction>>
			glyphConstruction;

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (10, glyphConstruction);
};


/*
 * MATH -- Mathematical typesetting
 * https://docs.microsoft.com/en-us/typography/opentype/spec/math
 */

ਬਣਤਰ MATH
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_MATH;

  ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int (); }

  ਖਾਲੀ depend (hb_depend_ਅੰਕੜੇ_builder_t *depend_ਅੰਕੜੇ) ਅਚਲ
  {
    ਜੇ (mathVariants)
      (ਇਹ+mathVariants).depend (depend_ਅੰਕੜੇ);
  }

  ਖਾਲੀ ਘੇਰਾ_glyphs (hb_ਸੈੱਟ_t *glyph_ਸੈੱਟ) ਅਚਲ
  {
    ਜੇ (mathVariants)
    {
      hb_ਸੈੱਟ_t variant_glyphs;
      (ਇਹ+mathVariants).ਘੇਰਾ_glyphs (glyph_ਸੈੱਟ, &variant_glyphs);
      hb_ਸੈੱਟ_union (glyph_ਸੈੱਟ, &variant_glyphs);
    }
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (*ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਬਾਹਰਲਾ->mathConstants.serialize_copy (c->serializer, mathConstants, ਇਹ, 0, hb_serialize_context_t::ਸਿਰ, &c->plan->layout_variation_idx_delta_map);
    ਬਾਹਰਲਾ->mathGlyphInfo.serialize_subset (c, mathGlyphInfo, ਇਹ);
    ਬਾਹਰਲਾ->mathVariants.serialize_subset (c, mathVariants, ਇਹ);
    return_trace (ਸੱਚ);
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c, ਅਚਲ hb_ਸੈੱਟ_t *variant_glyph_ਸੈੱਟ) ਅਚਲ
  {
    (ਇਹ+mathConstants).collect_variation_indices (c);
    (ਇਹ+mathGlyphInfo).collect_variation_indices (c);
    ਸਵੈਚਾਲਿਤ *glyph_ਸੈੱਟ = c->glyph_ਸੈੱਟ;
    c->glyph_ਸੈੱਟ = variant_glyph_ਸੈੱਟ;
    (ਇਹ+mathVariants).collect_variation_indices (c);
    c->glyph_ਸੈੱਟ = glyph_ਸੈੱਟ;
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (ਸੰਸਕਰਣ.sanitize (c) &&
		  likely (ਸੰਸਕਰਣ.major == 1) &&
		  hb_barrier () &&
		  mathConstants.sanitize (c, ਇਹ) &&
		  mathGlyphInfo.sanitize (c, ਇਹ) &&
		  mathVariants.sanitize (c, ਇਹ));
  }

  // https://github.com/harfbuzz/harfbuzz/issues/4653
  HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ is_bad_cambria (hb_font_t *font) ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_MATH
    ਚੋਣ HB_CODEPOINT_ENCODE3 (font->face->ਸਾਰਣੀ.MATH.get_blob ()->ਲੰਮਾਈ,
                                 (ਇਹ+mathConstants).min_ਉਚਾਈ[1], // displayOperatorMinHeight
                                 (ਇਹ+mathConstants).min_ਉਚਾਈ[0]) // delimitedSubFormulaMinHeight
    {
      /* sha1sum:ab4a4fe054d23061f3c039493d6f665cfda2ecf5  cambria.ttc
       * sha1sum:086855301bff644f9d8827b88491fcf73a6d4cb9  cambria.ttc
       * sha1sum:b1e5a3feaca2ea3dfcf79ccb377de749ecf60343  cambria.ttc */
      ਹਾਲਤ HB_CODEPOINT_ENCODE3 (25722, 2500, 3000):
        ਵਾਪਸ ਸੱਚ;
    }
#ਜੇ_ਅੰਤ
    ਵਾਪਸ ਝੂਠ;
  }

  hb_position_t get_constant (hb_ot_math_constant_t  constant,
			      hb_font_t		   *font) ਅਚਲ
  { ਵਾਪਸ (ਇਹ+mathConstants).get_ਮੁੱਲ (constant, font); }

  ਅਚਲ MathGlyphInfo &get_glyph_info () ਅਚਲ { ਵਾਪਸ ਇਹ+mathGlyphInfo; }

  ਅਚਲ MathVariants &get_variants () ਅਚਲ    { ਵਾਪਸ ਇਹ+mathVariants; }

  ਸੁਰੱਖਿਅਤ:
  Fixed_ਸੰਸਕਰਣ<>ਸੰਸਕਰਣ;	/* Version of the MATH table
				 * initially set to 0x00010000u */
  Offset16_ਮੰਜ਼ਿਲ<MathConstants>
		mathConstants;	/* MathConstants table */
  Offset16_ਮੰਜ਼ਿਲ<MathGlyphInfo>
		mathGlyphInfo;	/* MathGlyphInfo table */
  Offset16_ਮੰਜ਼ਿਲ<MathVariants>
		mathVariants;	/* MathVariants table */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (10);
};

} /* namespace OT */


#ਜੇ_ਅੰਤ /* HB_OT_MATH_TABLE_HH */

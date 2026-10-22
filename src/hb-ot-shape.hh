/*
 * Copyright © 2010  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_SHAPE_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_SHAPE_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-map.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-map.hh"


HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ
hb_ot_layout_ਸਾਰਣੀ_get_feature_variations_state (hb_face_t             *face,
						   hb_ਟੈਗ_t               ਸਾਰਣੀ_ਟੈਗ_2,
						   ਅਚਲ ਪੂਰਨ             *coords,
						   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ           ਅੰਕ_coords,
						   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ          *variations_ਸੂਚਕਾਂਕ,
						   hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> *lookup_conditions);

HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ
_hb_ot_layout_feature_get_lookup_variations (hb_face_t *face,
                                             hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ_2,
                                             ਬਿਨਾਨਿਸ਼ਾਨੀ feature_ਸੂਚਕਾਂਕ,
                                             ਬਿਨਾਨਿਸ਼ਾਨੀ variations_ਸੂਚਕਾਂਕ,
                                             hb_sorted_array_t<ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ> conditions,
                                             hb_ਸੈੱਟ_t *lookup_indexes);

ਬਣਤਰ hb_ot_shape_plan_ਕੁੰਜੀ_t
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ variations_ਸੂਚਕਾਂਕ[2];
  /* Sorted offsets of distinct true conditions within FeatureVariations.
   * Different outcomes may conservatively separate equivalent lookup sets. */
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> lookup_conditions[2];

  ਤਾਰਕਿਕ init (hb_face_t *face,
	     ਅਚਲ ਪੂਰਨ *coords,
	     ਬਿਨਾਨਿਸ਼ਾਨੀ   ਅੰਕ_coords)
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਾਰਣੀ_ਸੂਚਕਾਂਕ = 0; ਸਾਰਣੀ_ਸੂਚਕਾਂਕ < 2; ਸਾਰਣੀ_ਸੂਚਕਾਂਕ++)
      ਜੇ (unlikely (!hb_ot_layout_ਸਾਰਣੀ_get_feature_variations_state (
			face,
			ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ],
			coords,
			ਅੰਕ_coords,
			&variations_ਸੂਚਕਾਂਕ[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ],
			&lookup_conditions[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ])))
      {
	ਲਈ (ਸਵੈਚਾਲਿਤ &v : lookup_conditions)
	  v.fini ();
	ਵਾਪਸ ਝੂਠ;
      }
    ਵਾਪਸ ਸੱਚ;
  }

  ਖਾਲੀ fini ()
  {
    ਲਈ (ਸਵੈਚਾਲਿਤ &v : lookup_conditions)
      v.fini ();
  }

  ਤਾਰਕਿਕ get_lookup_variations (hb_face_t *face,
                              ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਾਰਣੀ_ਸੂਚਕਾਂਕ,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ feature_ਸੂਚਕਾਂਕ,
			      hb_ਸੈੱਟ_t *lookups) ਅਚਲ
  {
    ਵਾਪਸ _hb_ot_layout_feature_get_lookup_variations (
	face, ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ], feature_ਸੂਚਕਾਂਕ, variations_ਸੂਚਕਾਂਕ[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ],
	lookup_conditions[ਸਾਰਣੀ_ਸੂਚਕਾਂਕ].as_sorted_array (), lookups);
  }

  ਤਾਰਕਿਕ equal (ਅਚਲ hb_ot_shape_plan_ਕੁੰਜੀ_t *other)
  {
    ਵਾਪਸ variations_ਸੂਚਕਾਂਕ[0] == other->variations_ਸੂਚਕਾਂਕ[0] &&
	   variations_ਸੂਚਕਾਂਕ[1] == other->variations_ਸੂਚਕਾਂਕ[1] &&
	   lookup_conditions[0] == other->lookup_conditions[0] &&
	   lookup_conditions[1] == other->lookup_conditions[1];
  }
};


ਬਣਤਰ hb_shape_plan_ਕੁੰਜੀ_t;

ਬਣਤਰ hb_ot_shape_plan_t
{
  ~hb_ot_shape_plan_t () { fini (); }

  hb_segment_properties_t props;
  ਅਚਲ ਬਣਤਰ hb_ot_shaper_t *shaper;
  hb_ot_map_t map;
  hb_aat_map_t aat_map;
  ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPE_FRACTIONS
  hb_ਨਕਾਬ_t frac_ਨਕਾਬ, numr_ਨਕਾਬ, dnom_ਨਕਾਬ;
#ਨਹੀਂਤਾਂ
  ਸਥਿਰ constexpr hb_ਨਕਾਬ_t frac_ਨਕਾਬ = 0;
  ਸਥਿਰ constexpr hb_ਨਕਾਬ_t numr_ਨਕਾਬ = 0;
  ਸਥਿਰ constexpr hb_ਨਕਾਬ_t dnom_ਨਕਾਬ = 0;
#ਜੇ_ਅੰਤ
  hb_ਨਕਾਬ_t rtlm_ਨਕਾਬ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_KERN
  hb_ਨਕਾਬ_t kern_ਨਕਾਬ;
#ਨਹੀਂਤਾਂ
  ਸਥਿਰ constexpr hb_ਨਕਾਬ_t kern_ਨਕਾਬ = 0;
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_KERN
  ਤਾਰਕਿਕ requested_kerning : 1;
#ਨਹੀਂਤਾਂ
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ requested_kerning = ਝੂਠ;
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPE_FRACTIONS
  ਤਾਰਕਿਕ has_frac : 1;
#ਨਹੀਂਤਾਂ
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ has_frac = ਝੂਠ;
#ਜੇ_ਅੰਤ
  ਤਾਰਕਿਕ has_vert : 1;
  ਤਾਰਕਿਕ has_gpos_mark : 1;
  ਤਾਰਕਿਕ zero_marks : 1;
  ਤਾਰਕਿਕ fallback_glyph_classes : 1;
  ਤਾਰਕਿਕ fallback_mark_positioning : 1;
  ਤਾਰਕਿਕ adjust_mark_positioning_when_zeroing : 1;

  ਤਾਰਕਿਕ apply_gpos : 1;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_KERN
  ਤਾਰਕਿਕ apply_kern : 1;
#ਨਹੀਂਤਾਂ
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ apply_kern = ਝੂਠ;
#ਜੇ_ਅੰਤ
  ਤਾਰਕਿਕ apply_fallback_kern : 1;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AAT_SHAPE
  ਤਾਰਕਿਕ apply_kerx : 1;
  ਤਾਰਕਿਕ apply_morx : 1;
  ਤਾਰਕਿਕ apply_trak : 1;
#ਨਹੀਂਤਾਂ
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ apply_kerx = ਝੂਠ;
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ apply_morx = ਝੂਠ;
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ apply_trak = ਝੂਠ;
#ਜੇ_ਅੰਤ

  ਖਾਲੀ collect_lookups (hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ_2, hb_ਸੈੱਟ_t *lookups) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਾਰਣੀ_ਸੂਚਕਾਂਕ;
    ਚੋਣ (ਸਾਰਣੀ_ਟੈਗ_2) {
      ਹਾਲਤ HB_OT_ਟੈਗ_GSUB: ਸਾਰਣੀ_ਸੂਚਕਾਂਕ = 0; ਰੋਕੋ;
      ਹਾਲਤ HB_OT_ਟੈਗ_GPOS: ਸਾਰਣੀ_ਸੂਚਕਾਂਕ = 1; ਰੋਕੋ;
      ਮੂਲ: ਵਾਪਸ;
    }
    map.collect_lookups (ਸਾਰਣੀ_ਸੂਚਕਾਂਕ, lookups);
  }

  HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ init0 (hb_face_t                     *face,
			  ਅਚਲ hb_shape_plan_ਕੁੰਜੀ_t     *key);
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ fini ();

  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ substitute (hb_font_t *font, hb_ਬਫਰ_t *ਬਫਰ) ਅਚਲ;
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ position (hb_font_t *font, hb_ਬਫਰ_t *ਬਫਰ) ਅਚਲ;
};

ਬਣਤਰ hb_shape_plan_t;

ਬਣਤਰ hb_ot_shape_planner_t
{
  /* In the order that they are filled in. */
  hb_face_t *face;
  hb_segment_properties_t props;
  hb_ot_map_builder_t map;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AAT_SHAPE
  hb_aat_map_builder_t aat_map;
  ਤਾਰਕਿਕ apply_morx : 1;
#ਨਹੀਂਤਾਂ
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ apply_morx = ਝੂਠ;
#ਜੇ_ਅੰਤ
  ਤਾਰਕਿਕ script_zero_marks : 1;
  ਤਾਰਕਿਕ script_fallback_position : 1;
  ਅਚਲ ਬਣਤਰ hb_ot_shaper_t *shaper;

  HB_ਅੰਦਰੂਨੀ hb_ot_shape_planner_t (hb_face_t                     *face,
				     ਅਚਲ hb_segment_properties_t &props);

  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ compile (hb_ot_shape_plan_t           &plan,
			    ਅਚਲ hb_ot_shape_plan_ਕੁੰਜੀ_t &key);
};


#ਜੇ_ਅੰਤ /* HB_OT_SHAPE_HH */

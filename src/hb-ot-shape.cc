/*
 * Copyright © 2009,2010  Red Hat, Inc.
 * Copyright © 2010,2011,2012  Google, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPE

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT
#ਗਲਤੀ "Cannot compile 'ot' shaper with HB_NO_OT_LAYOUT."
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-shaper-impl.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shape.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shaper.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shape-fallback.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shape-normalize.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-face.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-set.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-unicode.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-layout.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-gdef-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-stat-table.hh"


ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
_hb_codepoint_is_regional_indicator (hb_codepoint_t u)
{ ਵਾਪਸ hb_ਵਿੱਚ_range<hb_codepoint_t> (u, 0x1F1E6u, 0x1F1FFu); }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AAT_SHAPE
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
_hb_apply_morx (hb_face_t *face, ਅਚਲ hb_segment_properties_t &props)
{
  /* https://github.com/harfbuzz/harfbuzz/issues/2124 */
  ਵਾਪਸ hb_aat_layout_has_substitution (face) &&
	 (HB_DIRECTION_IS_HORIZONTAL (props.direction) || !hb_ot_layout_has_substitution (face));
}
#ਜੇ_ਅੰਤ

/**
 * SECTION:hb-ot-shape
 * @title: hb-ot-shape
 * @short_description: OpenType shaping support
 * @include: hb-ot.h
 *
 * Support functions for OpenType shaping related queries.
 **/


ਸਥਿਰ ਖਾਲੀ
hb_ot_shape_collect_features (hb_ot_shape_planner_t          *planner,
			      ਅਚਲ hb_feature_t             *user_features,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ                    ਅੰਕ_user_features);

hb_ot_shape_planner_t::hb_ot_shape_planner_t (hb_face_t                     *face,
					      ਅਚਲ hb_segment_properties_t &props) :
						face (face),
						props (props),
						map (face, props)
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AAT_SHAPE
						, aat_map (face, props)
						, apply_morx (_hb_apply_morx (face, props))
#ਜੇ_ਅੰਤ
{
  shaper = hb_ot_shaper_categorize (props.script, props.direction, map.chosen_script[0]);

  script_zero_marks = shaper->zero_ਚੌੜਾਈ_marks != HB_OT_SHAPE_ZERO_ਚੌੜਾਈ_MARKS_NONE;
  script_fallback_position = shaper->fallback_position;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AAT_SHAPE
  /* https://github.com/harfbuzz/harfbuzz/issues/1528 */
  ਜੇ (apply_morx && shaper != &_hb_ot_shaper_default)
    shaper = &_hb_ot_shaper_dumber;
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ
hb_ot_shape_planner_t::compile (hb_ot_shape_plan_t           &plan,
				ਅਚਲ hb_ot_shape_plan_ਕੁੰਜੀ_t &key)
{
  plan.props = props;
  plan.shaper = shaper;
  map.compile (plan.map, key);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AAT_SHAPE
  ਜੇ (apply_morx)
    aat_map.compile (plan.aat_map);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPE_FRACTIONS
  plan.frac_ਨਕਾਬ = plan.map.get_1_ਨਕਾਬ (HB_ਟੈਗ ('f','r','a','c'));
  plan.numr_ਨਕਾਬ = plan.map.get_1_ਨਕਾਬ (HB_ਟੈਗ ('n','u','m','r'));
  plan.dnom_ਨਕਾਬ = plan.map.get_1_ਨਕਾਬ (HB_ਟੈਗ ('d','n','o','m'));
  plan.has_frac = plan.frac_ਨਕਾਬ || (plan.numr_ਨਕਾਬ && plan.dnom_ਨਕਾਬ);
#ਜੇ_ਅੰਤ

  plan.rtlm_ਨਕਾਬ = plan.map.get_1_ਨਕਾਬ (HB_ਟੈਗ ('r','t','l','m'));
  plan.has_vert = !!plan.map.get_1_ਨਕਾਬ (HB_ਟੈਗ ('v','e','r','t'));

  hb_ਟੈਗ_t kern_ਟੈਗ = HB_DIRECTION_IS_HORIZONTAL (props.direction) ?
		      HB_ਟੈਗ ('k','e','r','n') : HB_ਟੈਗ ('v','k','r','n');
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_KERN
  plan.kern_ਨਕਾਬ = plan.map.get_ਨਕਾਬ (kern_ਟੈਗ);
  plan.requested_kerning = !!plan.kern_ਨਕਾਬ;
#ਜੇ_ਅੰਤ

  ਤਾਰਕਿਕ has_gpos_kern = plan.map.get_feature_ਸੂਚਕਾਂਕ (1, kern_ਟੈਗ) != HB_OT_LAYOUT_NO_FEATURE_ਸੂਚਕਾਂਕ;
  ਤਾਰਕਿਕ disable_gpos = plan.shaper->gpos_ਟੈਗ &&
		      plan.shaper->gpos_ਟੈਗ != plan.map.chosen_script[1];

  /*
   * Decide who provides glyph classes. GDEF or Unicode.
   */

  ਜੇ (!hb_ot_layout_has_glyph_classes (face))
    plan.fallback_glyph_classes = ਸੱਚ;

  /*
   * Decide who does substitutions. GSUB, morx, or fallback.
   */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AAT_SHAPE
  plan.apply_morx = apply_morx;
#ਜੇ_ਅੰਤ

  /*
   * Decide who does positioning. GPOS, kerx, kern, or fallback.
   */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AAT_SHAPE
  ਤਾਰਕਿਕ has_kerx = hb_aat_layout_has_positioning (face);
  ਤਾਰਕਿਕ has_gsub = !apply_morx && hb_ot_layout_has_substitution (face);
#ਜੇ_ਅੰਤ
  ਤਾਰਕਿਕ has_gpos = !disable_gpos && hb_ot_layout_has_positioning (face);
  ਜੇ (ਝੂਠ)
    {}
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AAT_SHAPE
  /* Prefer GPOS over kerx if GSUB is present;
   * https://github.com/harfbuzz/harfbuzz/issues/3008 */
  ਨਹੀਂਤਾਂ ਜੇ (has_kerx && !(has_gsub && has_gpos))
    plan.apply_kerx = ਸੱਚ;
#ਜੇ_ਅੰਤ
  ਨਹੀਂਤਾਂ ਜੇ (has_gpos)
    plan.apply_gpos = ਸੱਚ;

  ਜੇ (!plan.apply_kerx && (!has_gpos_kern || !plan.apply_gpos))
  {
    ਜੇ (ਝੂਠ) {}
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AAT_SHAPE
    ਨਹੀਂਤਾਂ ਜੇ (has_kerx)
      plan.apply_kerx = ਸੱਚ;
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_KERN
    ਨਹੀਂਤਾਂ ਜੇ (hb_ot_layout_has_kerning (face))
      plan.apply_kern = script_fallback_position; // Not all shapers apply legacy `kern`
#ਜੇ_ਅੰਤ
    ਨਹੀਂਤਾਂ {}
  }

  plan.apply_fallback_kern = script_fallback_position && !(plan.apply_gpos || plan.apply_kerx || plan.apply_kern);

  plan.zero_marks = script_zero_marks &&
		    !plan.apply_kerx &&
		    (!plan.apply_kern
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_KERN
		     || !hb_ot_layout_has_machine_kerning (face)
#ਜੇ_ਅੰਤ
		    );
  plan.has_gpos_mark = !!plan.map.get_1_ਨਕਾਬ (HB_ਟੈਗ ('m','a','r','k'));

  plan.adjust_mark_positioning_when_zeroing = !plan.apply_gpos &&
					      !plan.apply_kerx &&
					      (!plan.apply_kern
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_KERN
					       || !hb_ot_layout_has_cross_kerning (face)
#ਜੇ_ਅੰਤ
					      );

  plan.fallback_mark_positioning = plan.adjust_mark_positioning_when_zeroing &&
				   script_fallback_position;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AAT_SHAPE
  /* If we're using morx shaping, we cancel mark position adjustment because
     Apple Color Emoji assumes this will NOT be done when forming emoji sequences;
     https://github.com/harfbuzz/harfbuzz/issues/2967. */
  ਜੇ (plan.apply_morx)
    plan.adjust_mark_positioning_when_zeroing = ਝੂਠ;

  /* According to Ned, trak is applied by default for "modern fonts", as detected by presence of STAT table. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_STYLE
  plan.apply_trak = hb_aat_layout_has_tracking (face) && face->ਸਾਰਣੀ.STAT->has_ਅੰਕੜੇ ();
#ਨਹੀਂਤਾਂ
  plan.apply_trak = ਝੂਠ;
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ
}

ਤਾਰਕਿਕ
hb_ot_shape_plan_t::init0 (hb_face_t                     *face,
			   ਅਚਲ hb_shape_plan_ਕੁੰਜੀ_t     *key)
{
  map.init ();

  hb_ot_shape_planner_t planner (face,
				 key->props);

  hb_ot_shape_collect_features (&planner,
				key->user_features,
				key->ਅੰਕ_user_features);

  planner.compile (*ਇਹ, key->ot);

  ਜੇ (shaper->ਅੰਕੜੇ_create)
  {
    ਅੰਕੜੇ = shaper->ਅੰਕੜੇ_create (ਇਹ);
    ਜੇ (unlikely (!ਅੰਕੜੇ))
    {
      map.fini ();
      ਵਾਪਸ ਝੂਠ;
    }
  }

  ਵਾਪਸ ਸੱਚ;
}

ਖਾਲੀ
hb_ot_shape_plan_t::fini ()
{
  ਜੇ (shaper->ਅੰਕੜੇ_destroy)
    shaper->ਅੰਕੜੇ_destroy (const_cast<ਖਾਲੀ *> (ਅੰਕੜੇ));

  map.fini ();
}

ਖਾਲੀ
hb_ot_shape_plan_t::substitute (hb_font_t   *font,
				hb_ਬਫਰ_t *ਬਫਰ) ਅਚਲ
{
  map.substitute (ਇਹ, font, ਬਫਰ);
}

ਖਾਲੀ
hb_ot_shape_plan_t::position (hb_font_t   *font,
			      hb_ਬਫਰ_t *ਬਫਰ) ਅਚਲ
{
  ਜੇ (ਇਹ->apply_gpos)
    map.position (ਇਹ, font, ਬਫਰ);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AAT_SHAPE
  ਨਹੀਂਤਾਂ ਜੇ (ਇਹ->apply_kerx)
    hb_aat_layout_position (ਇਹ, font, ਬਫਰ);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_KERN
  ਜੇ (ਇਹ->apply_kern)
    hb_ot_layout_kern (ਇਹ, font, ਬਫਰ);
#ਜੇ_ਅੰਤ
  ਨਹੀਂਤਾਂ ਜੇ (ਇਹ->apply_fallback_kern)
    _hb_ot_shape_fallback_kern (ਇਹ, font, ਬਫਰ);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AAT_SHAPE
  ਜੇ (ਇਹ->apply_trak)
    hb_aat_layout_track (ਇਹ, font, ਬਫਰ);
#ਜੇ_ਅੰਤ
}


ਸਥਿਰ ਅਚਲ hb_ot_map_feature_t
common_features[] =
{
  {HB_ਟੈਗ('a','b','v','m'), F_GLOBAL},
  {HB_ਟੈਗ('b','l','w','m'), F_GLOBAL},
  {HB_ਟੈਗ('c','c','m','p'), F_GLOBAL},
  {HB_ਟੈਗ('l','o','c','l'), F_GLOBAL},
  {HB_ਟੈਗ('m','a','r','k'), F_GLOBAL_MANUAL_JOINERS},
  {HB_ਟੈਗ('m','k','m','k'), F_GLOBAL_MANUAL_JOINERS},
  {HB_ਟੈਗ('r','l','i','g'), F_GLOBAL},
};


ਸਥਿਰ ਅਚਲ hb_ot_map_feature_t
horizontal_features[] =
{
  {HB_ਟੈਗ('c','a','l','t'), F_GLOBAL},
  {HB_ਟੈਗ('c','l','i','g'), F_GLOBAL},
  {HB_ਟੈਗ('c','u','r','s'), F_GLOBAL},
  {HB_ਟੈਗ('d','i','s','t'), F_GLOBAL},
  {HB_ਟੈਗ('k','e','r','n'), F_GLOBAL_HAS_FALLBACK},
  {HB_ਟੈਗ('l','i','g','a'), F_GLOBAL},
  {HB_ਟੈਗ('r','c','l','t'), F_GLOBAL},
};

ਸਥਿਰ ਖਾਲੀ
hb_ot_shape_collect_features (hb_ot_shape_planner_t *planner,
			      ਅਚਲ hb_feature_t    *user_features,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ           ਅੰਕ_user_features)
{
  hb_ot_map_builder_t *map = &planner->map;

  map->is_simple = ਸੱਚ;

  map->enable_feature (HB_ਟੈਗ('r','v','r','n'));
  map->ਜੋੜੋ_gsub_pause (ਖਾਲੀ_ਸੂਚਕ);

  ਚੋਣ (planner->props.direction)
  {
    ਹਾਲਤ HB_DIRECTION_LTR:
      map->enable_feature (HB_ਟੈਗ ('l','t','r','a'));
      map->enable_feature (HB_ਟੈਗ ('l','t','r','m'));
      ਰੋਕੋ;
    ਹਾਲਤ HB_DIRECTION_RTL:
      map->enable_feature (HB_ਟੈਗ ('r','t','l','a'));
      map->ਜੋੜੋ_feature (HB_ਟੈਗ ('r','t','l','m'));
      ਰੋਕੋ;
    ਹਾਲਤ HB_DIRECTION_TTB:
    ਹਾਲਤ HB_DIRECTION_BTT:
    ਹਾਲਤ HB_DIRECTION_INVALID:
    ਮੂਲ:
      ਰੋਕੋ;
  }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPE_FRACTIONS
  /* Automatic fractions. */
  map->ਜੋੜੋ_feature (HB_ਟੈਗ ('f','r','a','c'));
  map->ਜੋੜੋ_feature (HB_ਟੈਗ ('n','u','m','r'));
  map->ਜੋੜੋ_feature (HB_ਟੈਗ ('d','n','o','m'));
#ਜੇ_ਅੰਤ

  /* Random! */
  map->enable_feature (HB_ਟੈਗ ('r','a','n','d'), F_RANDOM, HB_OT_MAP_MAX_ਮੁੱਲ);

  map->enable_feature (HB_ਟੈਗ ('H','a','r','f')); /* Considered required. */
  map->enable_feature (HB_ਟੈਗ ('H','A','R','F')); /* Considered discretionary. */

  ਜੇ (planner->shaper->collect_features)
  {
    map->is_simple = ਝੂਠ;
    planner->shaper->collect_features (planner);
  }

  map->enable_feature (HB_ਟੈਗ ('B','u','z','z')); /* Considered required. */
  map->enable_feature (HB_ਟੈਗ ('B','U','Z','Z')); /* Considered discretionary. */

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ARRAY_ਲੰਮਾਈ (common_features); i++)
    map->ਜੋੜੋ_feature (common_features[i]);

  ਜੇ (HB_DIRECTION_IS_HORIZONTAL (planner->props.direction))
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ARRAY_ਲੰਮਾਈ (horizontal_features); i++)
      map->ਜੋੜੋ_feature (horizontal_features[i]);
  ਨਹੀਂਤਾਂ
  {
    /* We only apply `vert` feature. See:
     * https://github.com/harfbuzz/harfbuzz/commit/d71c0df2d17f4590d5611239577a6cb532c26528
     * https://lists.freedesktop.org/archives/harfbuzz/2013-August/003490.html */

    /* We really want to find a 'vert' feature if there's any in the font, no
     * matter which script/langsys it is listed (or not) under.
     * See various bugs referenced from:
     * https://github.com/harfbuzz/harfbuzz/issues/63 */
    map->enable_feature (HB_ਟੈਗ ('v','e','r','t'), F_GLOBAL_ਖੋਜ);
  }

  ਜੇ (ਅੰਕ_user_features)
    map->is_simple = ਝੂਠ;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਅੰਕ_user_features; i++)
  {
    ਅਚਲ hb_feature_t *feature = &user_features[i];
    map->ਜੋੜੋ_feature (feature->ਟੈਗ_2,
		      (feature->ਸ਼ੁਰੂ == HB_FEATURE_GLOBAL_ਸ਼ੁਰੂ &&
		       feature->ਸਿਰਾ == HB_FEATURE_GLOBAL_ਸਿਰਾ) ?  F_GLOBAL : F_NONE,
		      feature->value);
  }

  ਜੇ (planner->shaper->override_features)
    planner->shaper->override_features (planner);
}


/*
 * shaper face data
 */

ਬਣਤਰ hb_ot_face_ਅੰਕੜੇ_t {};

hb_ot_face_ਅੰਕੜੇ_t *
hb_ot_shaper_face_ਅੰਕੜੇ_create (hb_face_t *face)
{
  ਵਾਪਸ (hb_ot_face_ਅੰਕੜੇ_t *) HB_SHAPER_ਅੰਕੜੇ_SUCCEEDED;
}

ਖਾਲੀ
hb_ot_shaper_face_ਅੰਕੜੇ_destroy (hb_ot_face_ਅੰਕੜੇ_t *ਅੰਕੜੇ)
{
}


/*
 * shaper font data
 */

ਬਣਤਰ hb_ot_font_ਅੰਕੜੇ_t {
  OT::hb_scalar_cache_t ਅਣਵਰਤਿਆ_2; // Just for alignment
};

hb_ot_font_ਅੰਕੜੇ_t *
hb_ot_shaper_font_ਅੰਕੜੇ_create (hb_font_t *font)
{
  ਅਚਲ OT::ਵਸਤੂ_Variation_Store &var_store = font->face->ਸਾਰਣੀ.GDEF->ਸਾਰਣੀ->get_var_store ();
  ਵਾਪਸ (hb_ot_font_ਅੰਕੜੇ_t *) var_store.create_cache ();
}

ਖਾਲੀ
hb_ot_shaper_font_ਅੰਕੜੇ_destroy (hb_ot_font_ਅੰਕੜੇ_t *ਅੰਕੜੇ)
{
  OT::ਵਸਤੂ_Variation_Store::destroy_cache ((OT::hb_scalar_cache_t *) ਅੰਕੜੇ);
}


/*
 * shaper
 */

ਬਣਤਰ hb_ot_shape_context_t
{
  hb_ot_shape_plan_t *plan;
  hb_font_t *font;
  hb_face_t *face;
  hb_ਬਫਰ_t  *ਬਫਰ;
  ਅਚਲ hb_feature_t *user_features;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ        ਅੰਕ_user_features;

  /* Transient stuff */
  hb_direction_t ਨਿਸ਼ਾਨਾ_direction;
};



/* Main shaper */


/* Prepare */

ਸਥਿਰ ਖਾਲੀ
hb_ਸੈੱਟ_unicode_props (hb_ਬਫਰ_t *ਬਫਰ,
		      hb_ਨਕਾਬ_t    global_ਨਕਾਬ)
{
  /* Implement enough of Unicode Graphemes here that shaping
   * in reverse-direction wouldn't break graphemes.  Namely,
   * we mark all marks and ZWJ and ZWJ,Extended_Pictographic
   * sequences as continuations.  The foreach_grapheme()
   * macro uses this bit.
   *
   * https://www.unicode.org/reports/tr29/#Regex_Definitions
   */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਬਫਰ->len;
  hb_glyph_info_t *info = ਬਫਰ->info;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
  {
    info[i].ਨਕਾਬ = global_ਨਕਾਬ;
    hb_glyph_info_ਸੈੱਟ_unicode_props (&info[i], ਬਫਰ);

    ਜੇ (info[i].codepoint < 0x80)
      ਜਾਰੀ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ gen_cat = _hb_glyph_info_get_general_category (&info[i]);
    ਜੇ (FLAG_UNSAFE (gen_cat) &
	(FLAG (HB_UNICODE_GENERAL_CATEGORY_LOWERCASE_LETTER) |
	 FLAG (HB_UNICODE_GENERAL_CATEGORY_UPPERCASE_LETTER) |
	 FLAG (HB_UNICODE_GENERAL_CATEGORY_TITLECASE_LETTER) |
	 FLAG (HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER) |
	 FLAG (HB_UNICODE_GENERAL_CATEGORY_SPACE_SEPARATOR)))
      ਜਾਰੀ;

    /* Marks are already set as continuation by the above line.
     * Handle Emoji_Modifier and ZWJ-continuation. */
    ਜੇ (unlikely (gen_cat == HB_UNICODE_GENERAL_CATEGORY_MODIFIER_ਚਿੰਨ੍ਹ &&
		  hb_ਵਿੱਚ_range<hb_codepoint_t> (info[i].codepoint, 0x1F3FBu, 0x1F3FFu)))
    {
      hb_glyph_info_ਸੈੱਟ_continuation (&info[i], ਬਫਰ);
    }
    /* Regional_Indicators are hairy as hell...
     * https://github.com/harfbuzz/harfbuzz/issues/2265 */
    ਨਹੀਂਤਾਂ ਜੇ (unlikely (i && _hb_codepoint_is_regional_indicator (info[i].codepoint)))
    {
      ਜੇ (_hb_codepoint_is_regional_indicator (info[i - 1].codepoint) &&
	  !_hb_glyph_info_is_continuation (&info[i - 1]))
	hb_glyph_info_ਸੈੱਟ_continuation (&info[i], ਬਫਰ);
    }
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_EMOJI_SEQUENCES
    ਨਹੀਂਤਾਂ ਜੇ (unlikely (_hb_glyph_info_is_zwj (&info[i])))
    {
      hb_glyph_info_ਸੈੱਟ_continuation (&info[i], ਬਫਰ);
      ਜੇ (i + 1 < ਗਿਣਤੀ_2 &&
	  _hb_unicode_is_emoji_Extended_Pictographic (info[i + 1].codepoint))
      {
	i++;
	info[i].ਨਕਾਬ = global_ਨਕਾਬ;
	hb_glyph_info_ਸੈੱਟ_unicode_props (&info[i], ਬਫਰ);
	hb_glyph_info_ਸੈੱਟ_continuation (&info[i], ਬਫਰ);
      }
    }
#ਜੇ_ਅੰਤ
    /* Or part of the Other_Grapheme_Extend that is not marks.
     * As of Unicode 15 that is just:
     *
     * 200C          ; Other_Grapheme_Extend # Cf       ZERO WIDTH NON-JOINER
     * FF9E..FF9F    ; Other_Grapheme_Extend # Lm   [2] HALFWIDTH KATAKANA VOICED SOUND MARK..HALFWIDTH KATAKANA SEMI-VOICED SOUND MARK
     * E0020..E007F  ; Other_Grapheme_Extend # Cf  [96] TAG SPACE..CANCEL TAG
     *
     * ZWNJ is special, we don't want to merge it as there's no need, and keeping
     * it separate results in more granular clusters.
     * Tags are used for Emoji sub-region flag sequences:
     * https://github.com/harfbuzz/harfbuzz/issues/1556
     * Katakana ones were requested:
     * https://github.com/harfbuzz/harfbuzz/issues/3844
     */
    ਨਹੀਂਤਾਂ ਜੇ (unlikely (hb_ਵਿੱਚ_ranges<hb_codepoint_t> (info[i].codepoint, 0xFF9Eu, 0xFF9Fu, 0xE0020u, 0xE007Fu)))
      hb_glyph_info_ਸੈੱਟ_continuation (&info[i], ਬਫਰ);
    ਨਹੀਂਤਾਂ ਜੇ (unlikely (info[i].codepoint == 0x2044u /* FRACTION SLASH */))
      ਬਫਰ->scratch_flags |= HB_ਬਫਰ_SCRATCH_FLAG_HAS_FRACTION_SLASH;
  }
}

ਸਥਿਰ ਖਾਲੀ
hb_ਪਾਓ_dotted_circle (hb_ਬਫਰ_t *ਬਫਰ, hb_font_t *font)
{
  ਜੇ (unlikely (ਬਫਰ->flags & HB_ਬਫਰ_FLAG_DO_NOT_ਪਾਓ_DOTTED_CIRCLE))
    ਵਾਪਸ;

  ਜੇ (!(ਬਫਰ->flags & HB_ਬਫਰ_FLAG_BOT) ||
      ਬਫਰ->context_len[0] ||
      !_hb_glyph_info_is_unicode_mark (&ਬਫਰ->info[0]))
    ਵਾਪਸ;

  ਜੇ (!font->has_glyph (0x25CCu))
    ਵਾਪਸ;

  hb_glyph_info_t dottedcircle = {0};
  dottedcircle.codepoint = 0x25CCu;
  hb_glyph_info_ਸੈੱਟ_unicode_props (&dottedcircle, ਬਫਰ);

  ਬਫਰ->clear_output ();

  ਬਫਰ->idx = 0;
  hb_glyph_info_t info = dottedcircle;
  info.cluster = ਬਫਰ->cur().cluster;
  info.ਨਕਾਬ = ਬਫਰ->cur().ਨਕਾਬ;
  (ਖਾਲੀ) ਬਫਰ->output_info (info);

  ਬਫਰ->ਸਮਕਾਲੀ ();
}

ਸਥਿਰ ਖਾਲੀ
hb_form_clusters (hb_ਬਫਰ_t *ਬਫਰ)
{
  ਜੇ (!(ਬਫਰ->scratch_flags & HB_ਬਫਰ_SCRATCH_FLAG_HAS_CONTINUATIONS))
    ਵਾਪਸ;

  foreach_grapheme (ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ)
    ਬਫਰ->ਮਿਲਾਓ_grapheme_clusters (ਸ਼ੁਰੂ, ਸਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ
hb_ensure_native_direction (hb_ਬਫਰ_t *ਬਫਰ)
{
  hb_direction_t direction = ਬਫਰ->props.direction;
  hb_direction_t horiz_dir = hb_script_get_horizontal_direction (ਬਫਰ->props.script);

  /* Numeric runs in natively-RTL scripts are actually native-LTR, so we reset
   * the horiz_dir if the run contains at least one decimal-number char, and no
   * letter chars (ideally we should be checking for chars with strong
   * directionality but hb-unicode currently lacks bidi categories).
   *
   * This allows digit sequences in Arabic etc to be shaped in "native"
   * direction, so that features like ligatures will work as intended.
   *
   * https://github.com/harfbuzz/harfbuzz/issues/501
   *
   * Similar thing about Regional_Indicators; They are bidi=L, but Script=Common.
   * If they are present in a run of natively-RTL text, they get assigned a script
   * with natively RTL direction, which would result in wrong shaping if we
   * assign such native RTL direction to them then. Detect that as well.
   *
   * https://github.com/harfbuzz/harfbuzz/issues/3314
   */
  ਜੇ (unlikely (horiz_dir == HB_DIRECTION_RTL && direction == HB_DIRECTION_LTR))
  {
    ਤਾਰਕਿਕ ਮਿਲਿਆ_ਸੰਖਿਆ = ਝੂਠ, ਮਿਲਿਆ_letter = ਝੂਠ, ਮਿਲਿਆ_ri = ਝੂਠ;
    ਅਚਲ ਸਵੈਚਾਲਿਤ* info = ਬਫਰ->info;
    ਅਚਲ ਸਵੈਚਾਲਿਤ ਗਿਣਤੀ_2 = ਬਫਰ->len;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਸਵੈਚਾਲਿਤ gc = _hb_glyph_info_get_general_category (&info[i]);
      ਜੇ (HB_UNICODE_GENERAL_CATEGORY_IS_LETTER (gc))
      {
	ਮਿਲਿਆ_letter = ਸੱਚ;
	ਰੋਕੋ;
      }
      ਨਹੀਂਤਾਂ ਜੇ (gc == HB_UNICODE_GENERAL_CATEGORY_DECIMAL_ਸੰਖਿਆ)
	ਮਿਲਿਆ_ਸੰਖਿਆ = ਸੱਚ;
      ਨਹੀਂਤਾਂ ਜੇ (unlikely (_hb_codepoint_is_regional_indicator (info[i].codepoint)))
	ਮਿਲਿਆ_ri = ਸੱਚ;
    }
    ਜੇ ((ਮਿਲਿਆ_ਸੰਖਿਆ || ਮਿਲਿਆ_ri) && !ਮਿਲਿਆ_letter)
      horiz_dir = HB_DIRECTION_LTR;
  }

  /* TODO vertical:
   * The only BTT vertical script is Ogham, but it's not clear to me whether OpenType
   * Ogham fonts are supposed to be implemented BTT or not.  Need to research that
   * first. */
  ਜੇ ((HB_DIRECTION_IS_HORIZONTAL (direction) &&
       direction != horiz_dir && HB_DIRECTION_IS_VALID (horiz_dir)) ||
      (HB_DIRECTION_IS_VERTICAL   (direction) &&
       direction != HB_DIRECTION_TTB))
  {
    hb_ot_layout_ਉਲਟ_graphemes (ਬਫਰ);
    ਬਫਰ->props.direction = HB_DIRECTION_ਉਲਟ (ਬਫਰ->props.direction);
  }
}


/*
 * Substitute
 */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_ot_rotate_chars (ਅਚਲ hb_ot_shape_context_t *c)
{
  hb_ਬਫਰ_t *ਬਫਰ = c->ਬਫਰ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਬਫਰ->len;
  hb_glyph_info_t *info = ਬਫਰ->info;

  ਜੇ (HB_DIRECTION_IS_BACKWARD (c->ਨਿਸ਼ਾਨਾ_direction))
  {
    hb_unicode_funcs_t *unicode = ਬਫਰ->unicode;
    hb_ਨਕਾਬ_t rtlm_ਨਕਾਬ = c->plan->rtlm_ਨਕਾਬ;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++) {
      hb_codepoint_t codepoint = unicode->mirroring (info[i].codepoint);
      ਜੇ (unlikely (codepoint != info[i].codepoint && c->font->has_glyph (codepoint)))
	info[i].codepoint = codepoint;
      ਨਹੀਂਤਾਂ
	info[i].ਨਕਾਬ |= rtlm_ਨਕਾਬ;
    }
  }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VERTICAL
  ਜੇ (HB_DIRECTION_IS_VERTICAL (c->ਨਿਸ਼ਾਨਾ_direction) && !c->plan->has_vert)
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++) {
      hb_codepoint_t codepoint = hb_unicode_funcs_t::vertical_char_for (info[i].codepoint);
      ਜੇ (unlikely (codepoint != info[i].codepoint && c->font->has_glyph (codepoint)))
	info[i].codepoint = codepoint;
    }
  }
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_ot_shape_ਤਿਆਰੀ_masks_fraction (ਅਚਲ hb_ot_shape_context_t *c)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPE_FRACTIONS
  ਵਾਪਸ;
#ਜੇ_ਅੰਤ

  ਜੇ (!(c->ਬਫਰ->scratch_flags & HB_ਬਫਰ_SCRATCH_FLAG_HAS_FRACTION_SLASH) ||
      !c->plan->has_frac)
    ਵਾਪਸ;

  hb_ਬਫਰ_t *ਬਫਰ = c->ਬਫਰ;

  hb_ਨਕਾਬ_t pre_ਨਕਾਬ, post_ਨਕਾਬ;
  ਜੇ (HB_DIRECTION_IS_FORWARD (ਬਫਰ->props.direction))
  {
    pre_ਨਕਾਬ = c->plan->numr_ਨਕਾਬ | c->plan->frac_ਨਕਾਬ;
    post_ਨਕਾਬ = c->plan->frac_ਨਕਾਬ | c->plan->dnom_ਨਕਾਬ;
  }
  ਨਹੀਂਤਾਂ
  {
    pre_ਨਕਾਬ = c->plan->frac_ਨਕਾਬ | c->plan->dnom_ਨਕਾਬ;
    post_ਨਕਾਬ = c->plan->numr_ਨਕਾਬ | c->plan->frac_ਨਕਾਬ;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਬਫਰ->len;
  hb_glyph_info_t *info = ਬਫਰ->info;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
  {
    ਜੇ (info[i].codepoint == 0x2044u) /* FRACTION SLASH */
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ = i, ਸਿਰਾ = i + 1;
      ਜਦੋਂ (ਸ਼ੁਰੂ &&
	     _hb_glyph_info_get_general_category (&info[ਸ਼ੁਰੂ - 1]) ==
	     HB_UNICODE_GENERAL_CATEGORY_DECIMAL_ਸੰਖਿਆ)
	ਸ਼ੁਰੂ--;
      ਜਦੋਂ (ਸਿਰਾ < ਗਿਣਤੀ_2 &&
	     _hb_glyph_info_get_general_category (&info[ਸਿਰਾ]) ==
	     HB_UNICODE_GENERAL_CATEGORY_DECIMAL_ਸੰਖਿਆ)
	ਸਿਰਾ++;
      ਜੇ (ਸ਼ੁਰੂ == i || ਸਿਰਾ == i + 1)
      {
        ਜੇ (ਸ਼ੁਰੂ == i)
	  ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat (ਸ਼ੁਰੂ, ਸ਼ੁਰੂ + 1);
	ਜੇ (ਸਿਰਾ == i + 1)
	  ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat (ਸਿਰਾ - 1, ਸਿਰਾ);
	ਜਾਰੀ;
      }

      ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_break (ਸ਼ੁਰੂ, ਸਿਰਾ);

      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = ਸ਼ੁਰੂ; j < i; j++)
	info[j].ਨਕਾਬ |= pre_ਨਕਾਬ;
      info[i].ਨਕਾਬ |= c->plan->frac_ਨਕਾਬ;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = i + 1; j < ਸਿਰਾ; j++)
	info[j].ਨਕਾਬ |= post_ਨਕਾਬ;

      i = ਸਿਰਾ - 1;
    }
  }
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_ot_shape_ਤਿਆਰੀ_masks (ਅਚਲ hb_ot_shape_context_t *c)
{
  hb_ot_map_t *map = &c->plan->map;
  hb_ਬਫਰ_t *ਬਫਰ = c->ਬਫਰ;

  hb_ot_shape_ਤਿਆਰੀ_masks_fraction (c);

  ਜੇ (c->plan->shaper->ਤਿਆਰੀ_masks)
    c->plan->shaper->ਤਿਆਰੀ_masks (c->plan, ਬਫਰ, c->font);

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < c->ਅੰਕ_user_features; i++)
  {
    ਅਚਲ hb_feature_t *feature = &c->user_features[i];
    ਜੇ (!(feature->ਸ਼ੁਰੂ == HB_FEATURE_GLOBAL_ਸ਼ੁਰੂ && feature->ਸਿਰਾ == HB_FEATURE_GLOBAL_ਸਿਰਾ)) {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਖਿਸਕਾਓ;
      hb_ਨਕਾਬ_t ਨਕਾਬ = map->get_ਨਕਾਬ (feature->ਟੈਗ_2, &ਖਿਸਕਾਓ);
      ਬਫਰ->ਸੈੱਟ_masks (feature->value << ਖਿਸਕਾਓ, ਨਕਾਬ, feature->ਸ਼ੁਰੂ, feature->ਸਿਰਾ);
    }
  }
}

ਸਥਿਰ ਖਾਲੀ
hb_ot_zero_ਚੌੜਾਈ_default_ignorables (ਅਚਲ hb_ਬਫਰ_t *ਬਫਰ)
{
  ਜੇ (!(ਬਫਰ->scratch_flags & HB_ਬਫਰ_SCRATCH_FLAG_HAS_DEFAULT_IGNORABLES) ||
      (ਬਫਰ->flags & HB_ਬਫਰ_FLAG_PRESERVE_DEFAULT_IGNORABLES) ||
      (ਬਫਰ->flags & HB_ਬਫਰ_FLAG_REMOVE_DEFAULT_IGNORABLES))
    ਵਾਪਸ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਬਫਰ->len;
  hb_glyph_info_t *info = ਬਫਰ->info;
  hb_glyph_position_t *pos = ਬਫਰ->pos;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0;
  ਲਈ (i = 0; i < ਗਿਣਤੀ_2; i++)
    ਜੇ (unlikely (_hb_glyph_info_is_default_ignorable (&info[i])))
    {
      pos[i].x_advance = pos[i].y_advance = 0;
      ਜੇ (HB_DIRECTION_IS_HORIZONTAL (ਬਫਰ->props.direction))
	pos[i].x_ਵਿੱਥ_2 = 0;
      ਨਹੀਂਤਾਂ
        pos[i].y_ਵਿੱਥ_2 = 0;
    }
}

ਸਥਿਰ ਖਾਲੀ
hb_ot_deal_with_variation_selectors (hb_ਬਫਰ_t *ਬਫਰ)
{
  ਜੇ (!(ਬਫਰ->scratch_flags & HB_ਬਫਰ_SCRATCH_FLAG_HAS_VARIATION_SELECTOR_FALLBACK) ||
	ਬਫਰ->not_ਮਿਲਿਆ_variation_selector == HB_CODEPOINT_INVALID)
    ਵਾਪਸ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਬਫਰ->len;
  hb_glyph_info_t *info = ਬਫਰ->info;
  hb_glyph_position_t *pos = ਬਫਰ->pos;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
  {
    ਜੇ (_hb_glyph_info_is_variation_selector (&info[i]))
    {
      info[i].codepoint = ਬਫਰ->not_ਮਿਲਿਆ_variation_selector;
      pos[i].x_advance = pos[i].y_advance = pos[i].x_ਵਿੱਥ_2 = pos[i].y_ਵਿੱਥ_2 = 0;
      hb_glyph_info_ਸੈੱਟ_variation_selector (&info[i], ਝੂਠ);
    }
  }
}

ਸਥਿਰ ਖਾਲੀ
hb_ot_hide_default_ignorables (hb_ਬਫਰ_t *ਬਫਰ,
			       hb_font_t   *font)
{
  ਜੇ (!(ਬਫਰ->scratch_flags & HB_ਬਫਰ_SCRATCH_FLAG_HAS_DEFAULT_IGNORABLES) ||
      (ਬਫਰ->flags & HB_ਬਫਰ_FLAG_PRESERVE_DEFAULT_IGNORABLES))
    ਵਾਪਸ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਬਫਰ->len;
  hb_glyph_info_t *info = ਬਫਰ->info;

  hb_codepoint_t invisible = ਬਫਰ->invisible;
  ਜੇ (!(ਬਫਰ->flags & HB_ਬਫਰ_FLAG_REMOVE_DEFAULT_IGNORABLES) &&
      (invisible || font->get_nominal_glyph (' ', &invisible)))
  {
    /* Replace default-ignorables with a zero-advance invisible glyph. */
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਜੇ (_hb_glyph_info_is_default_ignorable (&info[i]))
	info[i].codepoint = invisible;
    }
  }
  ਨਹੀਂਤਾਂ
    ਬਫਰ->delete_glyphs_inplace (_hb_glyph_info_is_default_ignorable);
}


ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_ot_map_glyphs_fast (hb_ਬਫਰ_t  *ਬਫਰ)
{
  /* Normalization process sets up normalizer_glyph_index(), we just copy it. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਬਫਰ->len;
  hb_glyph_info_t *info = ਬਫਰ->info;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    info[i].codepoint = info[i].normalizer_glyph_ਸੂਚਕਾਂਕ();

  ਬਫਰ->content_type = HB_ਬਫਰ_CONTENT_TYPE_GLYPHS;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_synthesize_glyph_classes (hb_ਬਫਰ_t *ਬਫਰ)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਬਫਰ->len;
  hb_glyph_info_t *info = ਬਫਰ->info;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
  {
    hb_ot_layout_glyph_props_flags_t klass;

    /* Never mark default-ignorables as marks.
     * They won't get in the way of lookups anyway,
     * but having them as mark will cause them to be skipped
     * over if the lookup-flag says so, but at least for the
     * Mongolian variation selectors, looks like Uniscribe
     * marks them as non-mark.  Some Mongolian fonts without
     * GDEF rely on this.  Another notable character that
     * this applies to is COMBINING GRAPHEME JOINER. */
    klass = (_hb_glyph_info_get_general_category (&info[i]) !=
	     HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK ||
	     _hb_glyph_info_is_default_ignorable (&info[i])) ?
	    HB_OT_LAYOUT_GLYPH_PROPS_ਅਧਾਰ_GLYPH :
	    HB_OT_LAYOUT_GLYPH_PROPS_MARK;
    hb_glyph_info_ਸੈੱਟ_glyph_props (&info[i], klass);
  }
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_ot_substitute_default (ਅਚਲ hb_ot_shape_context_t *c)
{
  hb_ਬਫਰ_t *ਬਫਰ = c->ਬਫਰ;

  hb_ot_rotate_chars (c);

  HB_ਬਫਰ_ALLOCATE_VAR (ਬਫਰ, normalizer_glyph_ਸੂਚਕਾਂਕ);

  _hb_ot_shape_normalize (c->plan, ਬਫਰ, c->font);

  hb_ot_shape_ਤਿਆਰੀ_masks (c);

  /* This is unfortunate to go here, but necessary... */
  ਜੇ (c->plan->fallback_mark_positioning)
    _hb_ot_shape_fallback_mark_position_recategorize_marks (c->plan, c->font, ਬਫਰ);

  hb_ot_map_glyphs_fast (ਬਫਰ);

  HB_ਬਫਰ_DEALLOCATE_VAR (ਬਫਰ, normalizer_glyph_ਸੂਚਕਾਂਕ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_ot_substitute_plan (ਅਚਲ hb_ot_shape_context_t *c)
{
  hb_ਬਫਰ_t *ਬਫਰ = c->ਬਫਰ;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AAT_SHAPE
  ਜੇ (unlikely (c->plan->apply_morx))
  {
    hb_ot_layout_substitute_ਸ਼ੁਰੂ (c->font, ਬਫਰ);

    ਜੇ (c->plan->fallback_glyph_classes)
      hb_synthesize_glyph_classes (ਬਫਰ);

    hb_aat_layout_substitute (c->plan, c->font, c->ਬਫਰ,
			      c->user_features, c->ਅੰਕ_user_features);
    /* The buffer digest is only used by the OT lookup-apply loop;
     * without GPOS ahead, nothing consumes it. */
    ਜੇ (c->plan->apply_gpos)
      c->ਬਫਰ->update_digest ();
  }
  ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
  {
    hb_ot_layout_substitute_ਸ਼ੁਰੂ_with_digest (c->font, ਬਫਰ);

    ਜੇ (c->plan->fallback_glyph_classes)
      hb_synthesize_glyph_classes (ਬਫਰ);

    c->plan->substitute (c->font, ਬਫਰ);
  }
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_ot_substitute_pre (ਅਚਲ hb_ot_shape_context_t *c)
{
  hb_ot_substitute_default (c);

  hb_ਬਫਰ_allocate_gsubgpos_vars (c->ਬਫਰ);

  hb_ot_substitute_plan (c);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AAT_SHAPE
  ਜੇ (c->plan->apply_morx && c->plan->apply_gpos)
    hb_aat_layout_remove_deleted_glyphs (c->ਬਫਰ);
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_ot_substitute_post (ਅਚਲ hb_ot_shape_context_t *c)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AAT_SHAPE
  ਜੇ (c->plan->apply_morx && !c->plan->apply_gpos)
    hb_aat_layout_remove_deleted_glyphs (c->ਬਫਰ);
#ਜੇ_ਅੰਤ

  hb_ot_deal_with_variation_selectors (c->ਬਫਰ);
  hb_ot_hide_default_ignorables (c->ਬਫਰ, c->font);

  ਜੇ (c->plan->shaper->postprocess_glyphs &&
    c->ਬਫਰ->ਸੁਨੇਹਾ(c->font, "start postprocess-glyphs")) {
    c->plan->shaper->postprocess_glyphs (c->plan, c->ਬਫਰ, c->font);
    (ਖਾਲੀ) c->ਬਫਰ->ਸੁਨੇਹਾ(c->font, "end postprocess-glyphs");
  }
}


/*
 * Position
 */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
adjust_mark_offsets (hb_glyph_position_t *pos)
{
  pos->x_ਵਿੱਥ_2 = hb_saturate_sub (pos->x_ਵਿੱਥ_2, pos->x_advance);
  pos->y_ਵਿੱਥ_2 = hb_saturate_sub (pos->y_ਵਿੱਥ_2, pos->y_advance);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
zero_mark_ਚੌੜਾਈ (hb_glyph_position_t *pos)
{
  pos->x_advance = 0;
  pos->y_advance = 0;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
zero_mark_widths_by_gdef (hb_ਬਫਰ_t *ਬਫਰ, ਤਾਰਕਿਕ adjust_offsets)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਬਫਰ->len;
  hb_glyph_info_t *info = ਬਫਰ->info;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    ਜੇ (_hb_glyph_info_is_mark (&info[i]))
    {
      ਜੇ (adjust_offsets)
	adjust_mark_offsets (&ਬਫਰ->pos[i]);
      zero_mark_ਚੌੜਾਈ (&ਬਫਰ->pos[i]);
    }
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_ot_position_default (ਅਚਲ hb_ot_shape_context_t *c)
{
  hb_direction_t direction = c->ਬਫਰ->props.direction;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = c->ਬਫਰ->len;
  hb_glyph_info_t *info = c->ਬਫਰ->info;
  hb_glyph_position_t *pos = c->ਬਫਰ->pos;

  ਜੇ (HB_DIRECTION_IS_HORIZONTAL (direction))
  {
    c->font->get_glyph_h_advances (ਗਿਣਤੀ_2, &info[0].codepoint, ਆਕਾਰ(info[0]),
				   &pos[0].x_advance, ਆਕਾਰ(pos[0]));
    // h_origin defaults to zero; only apply it if the font has it.
    ਜੇ (c->font->has_glyph_h_origin_func () || c->font->has_glyph_h_origins_func ())
      c->font->ਘਟਾਓ_glyph_h_origins (c->ਬਫਰ);
  }
  ਨਹੀਂਤਾਂ
  {
    c->font->get_glyph_v_advances (ਗਿਣਤੀ_2, &info[0].codepoint, ਆਕਾਰ(info[0]),
				   &pos[0].y_advance, ਆਕਾਰ(pos[0]));
    // v_origin defaults to non-zero; apply even if only fallback is there.
    c->font->ਘਟਾਓ_glyph_v_origins (c->ਬਫਰ);
  }
  ਜੇ (c->ਬਫਰ->scratch_flags & HB_ਬਫਰ_SCRATCH_FLAG_HAS_SPACE_FALLBACK)
    _hb_ot_shape_fallback_spaces (c->plan, c->font, c->ਬਫਰ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_ot_position_plan (ਅਚਲ hb_ot_shape_context_t *c)
{
  /* If the font has no GPOS and direction is forward, then when
   * zeroing mark widths, we shift the mark with it, such that the
   * mark is positioned hanging over the previous glyph.  When
   * direction is backward we don't shift and it will end up
   * hanging over the next glyph after the final reordering.
   *
   * Note: If fallback positioning happens, we don't care about
   * this as it will be overridden.
   */
  ਤਾਰਕਿਕ adjust_offsets_when_zeroing = c->plan->adjust_mark_positioning_when_zeroing &&
				     HB_DIRECTION_IS_FORWARD (c->ਬਫਰ->props.direction);

  /* We change glyph origin to what GPOS expects (horizontal), apply GPOS, change it back. */

  // h_origin defaults to zero; only apply it if the font has it.
  ਜੇ (c->font->has_glyph_h_origin_func () || c->font->has_glyph_h_origins_func ())
    c->font->ਜੋੜੋ_glyph_h_origins (c->ਬਫਰ);

  hb_ot_layout_position_ਸ਼ੁਰੂ (c->font, c->ਬਫਰ);

  ਜੇ (c->plan->zero_marks)
    ਚੋਣ (c->plan->shaper->zero_ਚੌੜਾਈ_marks)
    {
      ਹਾਲਤ HB_OT_SHAPE_ZERO_ਚੌੜਾਈ_MARKS_BY_GDEF_EARLY:
	zero_mark_widths_by_gdef (c->ਬਫਰ, adjust_offsets_when_zeroing);
	ਰੋਕੋ;

      ਮੂਲ:
      ਹਾਲਤ HB_OT_SHAPE_ZERO_ਚੌੜਾਈ_MARKS_NONE:
      ਹਾਲਤ HB_OT_SHAPE_ZERO_ਚੌੜਾਈ_MARKS_BY_GDEF_LATE:
	ਰੋਕੋ;
    }

  c->plan->position (c->font, c->ਬਫਰ);

  ਜੇ (c->plan->zero_marks)
    ਚੋਣ (c->plan->shaper->zero_ਚੌੜਾਈ_marks)
    {
      ਹਾਲਤ HB_OT_SHAPE_ZERO_ਚੌੜਾਈ_MARKS_BY_GDEF_LATE:
	zero_mark_widths_by_gdef (c->ਬਫਰ, adjust_offsets_when_zeroing);
	ਰੋਕੋ;

      ਮੂਲ:
      ਹਾਲਤ HB_OT_SHAPE_ZERO_ਚੌੜਾਈ_MARKS_NONE:
      ਹਾਲਤ HB_OT_SHAPE_ZERO_ਚੌੜਾਈ_MARKS_BY_GDEF_EARLY:
	ਰੋਕੋ;
    }

  /* Finish off.  Has to follow a certain order. */
  hb_ot_layout_position_ਅੰਤ_advances (c->font, c->ਬਫਰ);
  hb_ot_zero_ਚੌੜਾਈ_default_ignorables (c->ਬਫਰ);
  hb_ot_layout_position_ਅੰਤ_offsets (c->font, c->ਬਫਰ);

  // h_origin defaults to zero; only apply it if the font has it.
  ਜੇ (c->font->has_glyph_h_origin_func () || c->font->has_glyph_h_origins_func ())
    c->font->ਘਟਾਓ_glyph_h_origins (c->ਬਫਰ);

  ਜੇ (c->plan->fallback_mark_positioning)
    _hb_ot_shape_fallback_mark_position (c->plan, c->font, c->ਬਫਰ,
					 adjust_offsets_when_zeroing);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_ot_position (ਅਚਲ hb_ot_shape_context_t *c)
{
  c->ਬਫਰ->clear_positions ();

  hb_ot_position_default (c);

  hb_ot_position_plan (c);

  ਜੇ (HB_DIRECTION_IS_BACKWARD (c->ਬਫਰ->props.direction))
    hb_ਬਫਰ_ਉਲਟ (c->ਬਫਰ);

  hb_ਬਫਰ_deallocate_gsubgpos_vars (c->ਬਫਰ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_propagate_flags (hb_ਬਫਰ_t *ਬਫਰ)
{
  /* Propagate cluster-level glyph flags to be the same on all cluster glyphs.
   * Simplifies using them. */

  hb_ਨਕਾਬ_t and_ਨਕਾਬ = HB_GLYPH_FLAG_DEFINED;
  ਜੇ ((ਬਫਰ->flags & HB_ਬਫਰ_FLAG_PRODUCE_UNSAFE_ਮੰਜ਼ਿਲ_CONCAT) == 0)
    and_ਨਕਾਬ &= ~HB_GLYPH_FLAG_UNSAFE_ਮੰਜ਼ਿਲ_CONCAT;

  hb_glyph_info_t *info = ਬਫਰ->info;

  ਜੇ ((ਬਫਰ->flags & HB_ਬਫਰ_FLAG_PRODUCE_ਸੁਰੱਖਿਅਤ_ਮੰਜ਼ਿਲ_ਪਾਓ_TATWEEL) == 0)
  {
    foreach_cluster (ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ)
    {
      ਜੇ (ਸਿਰਾ - ਸ਼ੁਰੂ == 1)
      {
        info[ਸ਼ੁਰੂ].ਨਕਾਬ &= and_ਨਕਾਬ;
	ਜਾਰੀ;
      }

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਨਕਾਬ = 0;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ; i < ਸਿਰਾ; i++)
	ਨਕਾਬ |= info[i].ਨਕਾਬ;

      ਨਕਾਬ &= and_ਨਕਾਬ;

      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ; i < ਸਿਰਾ; i++)
	info[i].ਨਕਾਬ = ਨਕਾਬ;
    }
    ਵਾਪਸ;
  }

  /* If we are producing SAFE_TO_INSERT_TATWEEL, then do two things:
   *
   * - If the places that the Arabic shaper marked as SAFE_TO_INSERT_TATWEEL,
   *   are UNSAFE_TO_BREAK, then clear the SAFE_TO_INSERT_TATWEEL,
   * - Any place that is SAFE_TO_INSERT_TATWEEL, is also now UNSAFE_TO_BREAK.
   *
   * We couldn't make this interaction earlier. It has to be done this way.
   */
  foreach_cluster (ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਨਕਾਬ = 0;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ; i < ਸਿਰਾ; i++)
      ਨਕਾਬ |= info[i].ਨਕਾਬ;

    ਜੇ (ਨਕਾਬ & HB_GLYPH_FLAG_UNSAFE_ਮੰਜ਼ਿਲ_BREAK)
      ਨਕਾਬ &= ~HB_GLYPH_FLAG_ਸੁਰੱਖਿਅਤ_ਮੰਜ਼ਿਲ_ਪਾਓ_TATWEEL;
    ਜੇ (ਨਕਾਬ & HB_GLYPH_FLAG_ਸੁਰੱਖਿਅਤ_ਮੰਜ਼ਿਲ_ਪਾਓ_TATWEEL)
      ਨਕਾਬ |= HB_GLYPH_FLAG_UNSAFE_ਮੰਜ਼ਿਲ_BREAK | HB_GLYPH_FLAG_UNSAFE_ਮੰਜ਼ਿਲ_CONCAT;

    ਨਕਾਬ &= and_ਨਕਾਬ;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ; i < ਸਿਰਾ; i++)
      info[i].ਨਕਾਬ = ਨਕਾਬ;
  }
}

/* Pull it all together! */

ਸਥਿਰ ਖਾਲੀ
hb_ot_shape_ਅੰਦਰੂਨੀ (hb_ot_shape_context_t *c)
{
  /* Save the original direction, we use it later. */
  c->ਨਿਸ਼ਾਨਾ_direction = c->ਬਫਰ->props.direction;

  hb_ਬਫਰ_allocate_unicode_vars (c->ਬਫਰ);

  hb_ਸੈੱਟ_unicode_props (c->ਬਫਰ, c->plan->map.get_global_ਨਕਾਬ ());
  hb_ਪਾਓ_dotted_circle (c->ਬਫਰ, c->font);

  hb_form_clusters (c->ਬਫਰ);

  hb_ensure_native_direction (c->ਬਫਰ);

  ਜੇ (c->plan->shaper->preprocess_ਪਾਠ &&
      c->ਬਫਰ->ਸੁਨੇਹਾ(c->font, "start preprocess-text"))
  {
    c->plan->shaper->preprocess_ਪਾਠ (c->plan, c->ਬਫਰ, c->font);
    (ਖਾਲੀ) c->ਬਫਰ->ਸੁਨੇਹਾ(c->font, "end preprocess-text");
  }

  hb_ot_substitute_pre (c);
  hb_ot_position (c);
  hb_ot_substitute_post (c);

  hb_propagate_flags (c->ਬਫਰ);

  hb_ਬਫਰ_deallocate_unicode_vars (c->ਬਫਰ);

  c->ਬਫਰ->props.direction = c->ਨਿਸ਼ਾਨਾ_direction;
}


hb_bool_t
_hb_ot_shape (hb_shape_plan_t    *shape_plan,
	      hb_font_t          *font,
	      hb_ਬਫਰ_t        *ਬਫਰ,
	      ਅਚਲ hb_feature_t *features,
	      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ        ਅੰਕ_features)
{
  hb_ot_shape_context_t c = {&shape_plan->ot, font, font->face, ਬਫਰ, features, ਅੰਕ_features};
  hb_ot_shape_ਅੰਦਰੂਨੀ (&c);

  ਵਾਪਸ ਸੱਚ;
}


/**
 * hb_ot_shape_plan_collect_lookups:
 * @shape_plan: #hb_shape_plan_t to query
 * @table_tag: GSUB or GPOS
 * @lookup_indexes: (out): The #hb_set_t set of lookups returned
 *
 * Computes the complete set of GSUB or GPOS lookups that are applicable
 * under a given @shape_plan.
 *
 * Since: 0.9.7
 **/
ਖਾਲੀ
hb_ot_shape_plan_collect_lookups (hb_shape_plan_t *shape_plan,
				  hb_ਟੈਗ_t         ਸਾਰਣੀ_ਟੈਗ_2,
				  hb_ਸੈੱਟ_t        *lookup_indexes /*ਬਾਹਰਲਾ*/)
{
  shape_plan->ot.collect_lookups (ਸਾਰਣੀ_ਟੈਗ_2, lookup_indexes);
}


/**
 * hb_ot_shape_plan_get_feature_tags:
 * @shape_plan: A shaping plan
 * @start_offset: The index of first feature to retrieve
 * @tag_count: (inout): Input = the maximum number of features to return;
 *                      Output = the actual number of features returned (may be zero)
 * @tags: (out) (array length=tag_count): The array of enabled feature
 *
 * Fetches the list of OpenType feature tags enabled for a shaping plan, if possible.
 *
 * Return value: Total number of feature tagss.
 *
 * Since: 10.3.0
 */
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_shape_plan_get_feature_ਟੈਗ_ਸੂਚੀ (hb_shape_plan_t *shape_plan,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ     ਸ਼ੁਰੂ_ਵਿੱਥ,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    *ਟੈਗ_ਗਿਣਤੀ, /* IN/OUT */
				   hb_ਟੈਗ_t        *ਟੈਗ_ਸੂਚੀ /*ਬਾਹਰਲਾ*/)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPE
  ਵਾਪਸ shape_plan->ot.map.get_feature_ਟੈਗ_ਸੂਚੀ (ਸ਼ੁਰੂ_ਵਿੱਥ, ਟੈਗ_ਗਿਣਤੀ, ਟੈਗ_ਸੂਚੀ);
#ਨਹੀਂਤਾਂ
  ਜੇ (ਟੈਗ_ਗਿਣਤੀ)
	*ਟੈਗ_ਗਿਣਤੀ = 0;
  ਵਾਪਸ 0;
#ਜੇ_ਅੰਤ
}


/* TODO Move this to hb-ot-shape-normalize, make it do decompose, and make it public. */
ਸਥਿਰ ਖਾਲੀ
ਜੋੜੋ_char (hb_font_t          *font,
	  hb_unicode_funcs_t *unicode,
	  hb_bool_t           mirror,
	  hb_codepoint_t      u,
	  hb_ਸੈੱਟ_t           *glyphs)
{
  hb_codepoint_t glyph;
  ਜੇ (font->get_nominal_glyph (u, &glyph))
    glyphs->ਜੋੜੋ (glyph);
  ਜੇ (mirror)
  {
    hb_codepoint_t m = unicode->mirroring (u);
    ਜੇ (m != u && font->get_nominal_glyph (m, &glyph))
      glyphs->ਜੋੜੋ (glyph);
  }
}


/**
 * hb_ot_shape_glyphs_closure:
 * @font: #hb_font_t to work upon
 * @buffer: The input buffer to compute from
 * @features: (array length=num_features): The features enabled on the buffer
 * @num_features: The number of features enabled on the buffer
 * @glyphs: (out): The #hb_set_t set of glyphs comprising the transitive closure of the query
 *
 * Computes the transitive closure of glyphs needed for a specified
 * input buffer under the given font and feature list. The closure is
 * computed as a set, not as a list.
 *
 * Since: 0.9.2
 **/
ਖਾਲੀ
hb_ot_shape_glyphs_ਘੇਰਾ (hb_font_t          *font,
			    hb_ਬਫਰ_t        *ਬਫਰ,
			    ਅਚਲ hb_feature_t *features,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ        ਅੰਕ_features,
			    hb_ਸੈੱਟ_t           *glyphs)
{
  ਅਚਲ ਅੱਖਰ *shapers[] = {"ot", ਖਾਲੀ_ਸੂਚਕ};
  hb_shape_plan_t *shape_plan = hb_shape_plan_create_cached (font->face, &ਬਫਰ->props,
							     features, ਅੰਕ_features, shapers);

  ਤਾਰਕਿਕ mirror = hb_script_get_horizontal_direction (ਬਫਰ->props.script) == HB_DIRECTION_RTL;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਬਫਰ->len;
  hb_glyph_info_t *info = ਬਫਰ->info;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    ਜੋੜੋ_char (font, ਬਫਰ->unicode, mirror, info[i].codepoint, glyphs);

  hb_ਸੈੱਟ_t *lookups = hb_ਸੈੱਟ_create ();
  hb_ot_shape_plan_collect_lookups (shape_plan, HB_OT_ਟੈਗ_GSUB, lookups);
  hb_ot_layout_lookups_substitute_ਘੇਰਾ (font->face, lookups, glyphs);

  hb_ਸੈੱਟ_destroy (lookups);

  hb_shape_plan_destroy (shape_plan);
}


/**
 * hb_ot_shape_get_buffer_format_serial:
 *
 * Returns the serial number of the current internal buffer format.
 * See #HB_OT_SHAPE_BUFFER_FORMAT_SERIAL for more information.
 *
 * Return value: The current buffer-format serial number.
 *
 * Since: 13.2.0
 **/
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_shape_get_ਬਫਰ_format_serial (ਖਾਲੀ)
{
  ਵਾਪਸ HB_OT_SHAPE_ਬਫਰ_FORMAT_SERIAL;
}

#ਜੇ_ਅੰਤ

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

#అన్తర్భావ "hb.hh"

#నిర్వచితం_కాకపోతే HB_NO_OT_SHAPE

#నిర్వచితమైతే HB_NO_OT_LAYOUT
#దోషం "Cannot compile 'ot' shaper with HB_NO_OT_LAYOUT."
#అయితే_ముగింపు

#అన్తర్భావ "hb-shaper-impl.hh"

#అన్తర్భావ "hb-ot-shape.hh"
#అన్తర్భావ "hb-ot-shaper.hh"
#అన్తర్భావ "hb-ot-shape-fallback.hh"
#అన్తర్భావ "hb-ot-shape-normalize.hh"

#అన్తర్భావ "hb-ot-face.hh"

#అన్తర్భావ "hb-set.hh"
#అన్తర్భావ "hb-unicode.hh"

#అన్తర్భావ "hb-aat-layout.hh"
#అన్తర్భావ "hb-ot-layout-gdef-table.hh"
#అన్తర్భావ "hb-ot-stat-table.hh"


స్థిర అంతఃవరుస తార్కిక
_hb_codepoint_is_regional_indicator (hb_codepoint_t u)
{ తిరిగి hb_లోపల_range<hb_codepoint_t> (u, 0x1F1E6u, 0x1F1FFu); }

#నిర్వచితం_కాకపోతే HB_NO_AAT_SHAPE
స్థిర అంతఃవరుస తార్కిక
_hb_apply_morx (hb_face_t *face, స్థిరరాశి hb_segment_properties_t &props)
{
  /* https://github.com/harfbuzz/harfbuzz/issues/2124 */
  తిరిగి hb_aat_layout_has_substitution (face) &&
	 (HB_DIRECTION_IS_HORIZONTAL (props.direction) || !hb_ot_layout_has_substitution (face));
}
#అయితే_ముగింపు

/**
 * SECTION:hb-ot-shape
 * @title: hb-ot-shape
 * @short_description: OpenType shaping support
 * @include: hb-ot.h
 *
 * Support functions for OpenType shaping related queries.
 **/


స్థిర శూన్య
hb_ot_shape_collect_features (hb_ot_shape_planner_t          *planner,
			      స్థిరరాశి hb_feature_t             *user_features,
			      గుర్తులేని పూర్ణాంకం                    అంకె_user_features);

hb_ot_shape_planner_t::hb_ot_shape_planner_t (hb_face_t                     *face,
					      స్థిరరాశి hb_segment_properties_t &props) :
						face (face),
						props (props),
						map (face, props)
#నిర్వచితం_కాకపోతే HB_NO_AAT_SHAPE
						, aat_map (face, props)
						, apply_morx (_hb_apply_morx (face, props))
#అయితే_ముగింపు
{
  shaper = hb_ot_shaper_categorize (props.script, props.direction, map.chosen_script[0]);

  script_zero_marks = shaper->zero_వెడల్పు_marks != HB_OT_SHAPE_ZERO_వెడల్పు_MARKS_NONE;
  script_fallback_position = shaper->fallback_position;

#నిర్వచితం_కాకపోతే HB_NO_AAT_SHAPE
  /* https://github.com/harfbuzz/harfbuzz/issues/1528 */
  ఒకవేళ (apply_morx && shaper != &_hb_ot_shaper_default)
    shaper = &_hb_ot_shaper_dumber;
#అయితే_ముగింపు
}

శూన్య
hb_ot_shape_planner_t::compile (hb_ot_shape_plan_t           &plan,
				స్థిరరాశి hb_ot_shape_plan_కీలకం_t &key)
{
  plan.props = props;
  plan.shaper = shaper;
  map.compile (plan.map, key);
#నిర్వచితం_కాకపోతే HB_NO_AAT_SHAPE
  ఒకవేళ (apply_morx)
    aat_map.compile (plan.aat_map);
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_NO_OT_SHAPE_FRACTIONS
  plan.frac_ముసుగు = plan.map.get_1_ముసుగు (HB_ట్యాగ్ ('f','r','a','c'));
  plan.numr_ముసుగు = plan.map.get_1_ముసుగు (HB_ట్యాగ్ ('n','u','m','r'));
  plan.dnom_ముసుగు = plan.map.get_1_ముసుగు (HB_ట్యాగ్ ('d','n','o','m'));
  plan.has_frac = plan.frac_ముసుగు || (plan.numr_ముసుగు && plan.dnom_ముసుగు);
#అయితే_ముగింపు

  plan.rtlm_ముసుగు = plan.map.get_1_ముసుగు (HB_ట్యాగ్ ('r','t','l','m'));
  plan.has_vert = !!plan.map.get_1_ముసుగు (HB_ట్యాగ్ ('v','e','r','t'));

  hb_ట్యాగ్_t kern_ట్యాగ్ = HB_DIRECTION_IS_HORIZONTAL (props.direction) ?
		      HB_ట్యాగ్ ('k','e','r','n') : HB_ట్యాగ్ ('v','k','r','n');
#నిర్వచితం_కాకపోతే HB_NO_OT_KERN
  plan.kern_ముసుగు = plan.map.get_ముసుగు (kern_ట్యాగ్);
  plan.requested_kerning = !!plan.kern_ముసుగు;
#అయితే_ముగింపు

  తార్కిక has_gpos_kern = plan.map.get_feature_సూచీ (1, kern_ట్యాగ్) != HB_OT_LAYOUT_NO_FEATURE_సూచీ;
  తార్కిక disable_gpos = plan.shaper->gpos_ట్యాగ్ &&
		      plan.shaper->gpos_ట్యాగ్ != plan.map.chosen_script[1];

  /*
   * Decide who provides glyph classes. GDEF or Unicode.
   */

  ఒకవేళ (!hb_ot_layout_has_glyph_classes (face))
    plan.fallback_glyph_classes = నిజం;

  /*
   * Decide who does substitutions. GSUB, morx, or fallback.
   */

#నిర్వచితం_కాకపోతే HB_NO_AAT_SHAPE
  plan.apply_morx = apply_morx;
#అయితే_ముగింపు

  /*
   * Decide who does positioning. GPOS, kerx, kern, or fallback.
   */

#నిర్వచితం_కాకపోతే HB_NO_AAT_SHAPE
  తార్కిక has_kerx = hb_aat_layout_has_positioning (face);
  తార్కిక has_gsub = !apply_morx && hb_ot_layout_has_substitution (face);
#అయితే_ముగింపు
  తార్కిక has_gpos = !disable_gpos && hb_ot_layout_has_positioning (face);
  ఒకవేళ (అబద్ధం)
    {}
#నిర్వచితం_కాకపోతే HB_NO_AAT_SHAPE
  /* Prefer GPOS over kerx if GSUB is present;
   * https://github.com/harfbuzz/harfbuzz/issues/3008 */
  లేకపోతే ఒకవేళ (has_kerx && !(has_gsub && has_gpos))
    plan.apply_kerx = నిజం;
#అయితే_ముగింపు
  లేకపోతే ఒకవేళ (has_gpos)
    plan.apply_gpos = నిజం;

  ఒకవేళ (!plan.apply_kerx && (!has_gpos_kern || !plan.apply_gpos))
  {
    ఒకవేళ (అబద్ధం) {}
#నిర్వచితం_కాకపోతే HB_NO_AAT_SHAPE
    లేకపోతే ఒకవేళ (has_kerx)
      plan.apply_kerx = నిజం;
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే HB_NO_OT_KERN
    లేకపోతే ఒకవేళ (hb_ot_layout_has_kerning (face))
      plan.apply_kern = script_fallback_position; // Not all shapers apply legacy `kern`
#అయితే_ముగింపు
    లేకపోతే {}
  }

  plan.apply_fallback_kern = script_fallback_position && !(plan.apply_gpos || plan.apply_kerx || plan.apply_kern);

  plan.zero_marks = script_zero_marks &&
		    !plan.apply_kerx &&
		    (!plan.apply_kern
#నిర్వచితం_కాకపోతే HB_NO_OT_KERN
		     || !hb_ot_layout_has_machine_kerning (face)
#అయితే_ముగింపు
		    );
  plan.has_gpos_mark = !!plan.map.get_1_ముసుగు (HB_ట్యాగ్ ('m','a','r','k'));

  plan.adjust_mark_positioning_when_zeroing = !plan.apply_gpos &&
					      !plan.apply_kerx &&
					      (!plan.apply_kern
#నిర్వచితం_కాకపోతే HB_NO_OT_KERN
					       || !hb_ot_layout_has_cross_kerning (face)
#అయితే_ముగింపు
					      );

  plan.fallback_mark_positioning = plan.adjust_mark_positioning_when_zeroing &&
				   script_fallback_position;

#నిర్వచితం_కాకపోతే HB_NO_AAT_SHAPE
  /* If we're using morx shaping, we cancel mark position adjustment because
     Apple Color Emoji assumes this will NOT be done when forming emoji sequences;
     https://github.com/harfbuzz/harfbuzz/issues/2967. */
  ఒకవేళ (plan.apply_morx)
    plan.adjust_mark_positioning_when_zeroing = అబద్ధం;

  /* According to Ned, trak is applied by default for "modern fonts", as detected by presence of STAT table. */
#నిర్వచితం_కాకపోతే HB_NO_STYLE
  plan.apply_trak = hb_aat_layout_has_tracking (face) && face->పట్టిక.STAT->has_దత్తాంశం ();
#లేకపోతే
  plan.apply_trak = అబద్ధం;
#అయితే_ముగింపు

#అయితే_ముగింపు
}

తార్కిక
hb_ot_shape_plan_t::init0 (hb_face_t                     *face,
			   స్థిరరాశి hb_shape_plan_కీలకం_t     *key)
{
  map.init ();

  hb_ot_shape_planner_t planner (face,
				 key->props);

  hb_ot_shape_collect_features (&planner,
				key->user_features,
				key->అంకె_user_features);

  planner.compile (*ఇది, key->ot);

  ఒకవేళ (shaper->దత్తాంశం_create)
  {
    దత్తాంశం = shaper->దత్తాంశం_create (ఇది);
    ఒకవేళ (unlikely (!దత్తాంశం))
    {
      map.fini ();
      తిరిగి అబద్ధం;
    }
  }

  తిరిగి నిజం;
}

శూన్య
hb_ot_shape_plan_t::fini ()
{
  ఒకవేళ (shaper->దత్తాంశం_destroy)
    shaper->దత్తాంశం_destroy (const_cast<శూన్య *> (దత్తాంశం));

  map.fini ();
}

శూన్య
hb_ot_shape_plan_t::substitute (hb_font_t   *font,
				hb_బఫరు_t *బఫరు) స్థిరరాశి
{
  map.substitute (ఇది, font, బఫరు);
}

శూన్య
hb_ot_shape_plan_t::position (hb_font_t   *font,
			      hb_బఫరు_t *బఫరు) స్థిరరాశి
{
  ఒకవేళ (ఇది->apply_gpos)
    map.position (ఇది, font, బఫరు);
#నిర్వచితం_కాకపోతే HB_NO_AAT_SHAPE
  లేకపోతే ఒకవేళ (ఇది->apply_kerx)
    hb_aat_layout_position (ఇది, font, బఫరు);
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_NO_OT_KERN
  ఒకవేళ (ఇది->apply_kern)
    hb_ot_layout_kern (ఇది, font, బఫరు);
#అయితే_ముగింపు
  లేకపోతే ఒకవేళ (ఇది->apply_fallback_kern)
    _hb_ot_shape_fallback_kern (ఇది, font, బఫరు);

#నిర్వచితం_కాకపోతే HB_NO_AAT_SHAPE
  ఒకవేళ (ఇది->apply_trak)
    hb_aat_layout_track (ఇది, font, బఫరు);
#అయితే_ముగింపు
}


స్థిర స్థిరరాశి hb_ot_map_feature_t
common_features[] =
{
  {HB_ట్యాగ్('a','b','v','m'), F_GLOBAL},
  {HB_ట్యాగ్('b','l','w','m'), F_GLOBAL},
  {HB_ట్యాగ్('c','c','m','p'), F_GLOBAL},
  {HB_ట్యాగ్('l','o','c','l'), F_GLOBAL},
  {HB_ట్యాగ్('m','a','r','k'), F_GLOBAL_MANUAL_JOINERS},
  {HB_ట్యాగ్('m','k','m','k'), F_GLOBAL_MANUAL_JOINERS},
  {HB_ట్యాగ్('r','l','i','g'), F_GLOBAL},
};


స్థిర స్థిరరాశి hb_ot_map_feature_t
horizontal_features[] =
{
  {HB_ట్యాగ్('c','a','l','t'), F_GLOBAL},
  {HB_ట్యాగ్('c','l','i','g'), F_GLOBAL},
  {HB_ట్యాగ్('c','u','r','s'), F_GLOBAL},
  {HB_ట్యాగ్('d','i','s','t'), F_GLOBAL},
  {HB_ట్యాగ్('k','e','r','n'), F_GLOBAL_HAS_FALLBACK},
  {HB_ట్యాగ్('l','i','g','a'), F_GLOBAL},
  {HB_ట్యాగ్('r','c','l','t'), F_GLOBAL},
};

స్థిర శూన్య
hb_ot_shape_collect_features (hb_ot_shape_planner_t *planner,
			      స్థిరరాశి hb_feature_t    *user_features,
			      గుర్తులేని పూర్ణాంకం           అంకె_user_features)
{
  hb_ot_map_builder_t *map = &planner->map;

  map->is_simple = నిజం;

  map->enable_feature (HB_ట్యాగ్('r','v','r','n'));
  map->కూడు_gsub_pause (శూన్యసూచిక);

  ఎంపిక (planner->props.direction)
  {
    స్థితి HB_DIRECTION_LTR:
      map->enable_feature (HB_ట్యాగ్ ('l','t','r','a'));
      map->enable_feature (HB_ట్యాగ్ ('l','t','r','m'));
      ఆపు;
    స్థితి HB_DIRECTION_RTL:
      map->enable_feature (HB_ట్యాగ్ ('r','t','l','a'));
      map->కూడు_feature (HB_ట్యాగ్ ('r','t','l','m'));
      ఆపు;
    స్థితి HB_DIRECTION_TTB:
    స్థితి HB_DIRECTION_BTT:
    స్థితి HB_DIRECTION_INVALID:
    అప్రమేయ:
      ఆపు;
  }

#నిర్వచితం_కాకపోతే HB_NO_OT_SHAPE_FRACTIONS
  /* Automatic fractions. */
  map->కూడు_feature (HB_ట్యాగ్ ('f','r','a','c'));
  map->కూడు_feature (HB_ట్యాగ్ ('n','u','m','r'));
  map->కూడు_feature (HB_ట్యాగ్ ('d','n','o','m'));
#అయితే_ముగింపు

  /* Random! */
  map->enable_feature (HB_ట్యాగ్ ('r','a','n','d'), F_RANDOM, HB_OT_MAP_MAX_విలువ);

  map->enable_feature (HB_ట్యాగ్ ('H','a','r','f')); /* Considered required. */
  map->enable_feature (HB_ట్యాగ్ ('H','A','R','F')); /* Considered discretionary. */

  ఒకవేళ (planner->shaper->collect_features)
  {
    map->is_simple = అబద్ధం;
    planner->shaper->collect_features (planner);
  }

  map->enable_feature (HB_ట్యాగ్ ('B','u','z','z')); /* Considered required. */
  map->enable_feature (HB_ట్యాగ్ ('B','U','Z','Z')); /* Considered discretionary. */

  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < ARRAY_నిడివి (common_features); i++)
    map->కూడు_feature (common_features[i]);

  ఒకవేళ (HB_DIRECTION_IS_HORIZONTAL (planner->props.direction))
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < ARRAY_నిడివి (horizontal_features); i++)
      map->కూడు_feature (horizontal_features[i]);
  లేకపోతే
  {
    /* We only apply `vert` feature. See:
     * https://github.com/harfbuzz/harfbuzz/commit/d71c0df2d17f4590d5611239577a6cb532c26528
     * https://lists.freedesktop.org/archives/harfbuzz/2013-August/003490.html */

    /* We really want to find a 'vert' feature if there's any in the font, no
     * matter which script/langsys it is listed (or not) under.
     * See various bugs referenced from:
     * https://github.com/harfbuzz/harfbuzz/issues/63 */
    map->enable_feature (HB_ట్యాగ్ ('v','e','r','t'), F_GLOBAL_శోధన);
  }

  ఒకవేళ (అంకె_user_features)
    map->is_simple = అబద్ధం;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < అంకె_user_features; i++)
  {
    స్థిరరాశి hb_feature_t *feature = &user_features[i];
    map->కూడు_feature (feature->ట్యాగ్_2,
		      (feature->ప్రారంభం == HB_FEATURE_GLOBAL_ప్రారంభం &&
		       feature->చివర == HB_FEATURE_GLOBAL_చివర) ?  F_GLOBAL : F_NONE,
		      feature->value);
  }

  ఒకవేళ (planner->shaper->override_features)
    planner->shaper->override_features (planner);
}


/*
 * shaper face data
 */

నిర్మాణం hb_ot_face_దత్తాంశం_t {};

hb_ot_face_దత్తాంశం_t *
hb_ot_shaper_face_దత్తాంశం_create (hb_face_t *face)
{
  తిరిగి (hb_ot_face_దత్తాంశం_t *) HB_SHAPER_దత్తాంశం_SUCCEEDED;
}

శూన్య
hb_ot_shaper_face_దత్తాంశం_destroy (hb_ot_face_దత్తాంశం_t *దత్తాంశం)
{
}


/*
 * shaper font data
 */

నిర్మాణం hb_ot_font_దత్తాంశం_t {
  OT::hb_scalar_cache_t వాడని_2; // Just for alignment
};

hb_ot_font_దత్తాంశం_t *
hb_ot_shaper_font_దత్తాంశం_create (hb_font_t *font)
{
  స్థిరరాశి OT::అంశం_Variation_Store &var_store = font->face->పట్టిక.GDEF->పట్టిక->get_var_store ();
  తిరిగి (hb_ot_font_దత్తాంశం_t *) var_store.create_cache ();
}

శూన్య
hb_ot_shaper_font_దత్తాంశం_destroy (hb_ot_font_దత్తాంశం_t *దత్తాంశం)
{
  OT::అంశం_Variation_Store::destroy_cache ((OT::hb_scalar_cache_t *) దత్తాంశం);
}


/*
 * shaper
 */

నిర్మాణం hb_ot_shape_context_t
{
  hb_ot_shape_plan_t *plan;
  hb_font_t *font;
  hb_face_t *face;
  hb_బఫరు_t  *బఫరు;
  స్థిరరాశి hb_feature_t *user_features;
  గుర్తులేని పూర్ణాంకం        అంకె_user_features;

  /* Transient stuff */
  hb_direction_t లక్ష్యం_direction;
};



/* Main shaper */


/* Prepare */

స్థిర శూన్య
hb_సమితి_unicode_props (hb_బఫరు_t *బఫరు,
		      hb_ముసుగు_t    global_ముసుగు)
{
  /* Implement enough of Unicode Graphemes here that shaping
   * in reverse-direction wouldn't break graphemes.  Namely,
   * we mark all marks and ZWJ and ZWJ,Extended_Pictographic
   * sequences as continuations.  The foreach_grapheme()
   * macro uses this bit.
   *
   * https://www.unicode.org/reports/tr29/#Regex_Definitions
   */
  గుర్తులేని పూర్ణాంకం లెక్క = బఫరు->len;
  hb_glyph_info_t *info = బఫరు->info;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
  {
    info[i].ముసుగు = global_ముసుగు;
    hb_glyph_info_సమితి_unicode_props (&info[i], బఫరు);

    ఒకవేళ (info[i].codepoint < 0x80)
      కొనసాగు;

    గుర్తులేని gen_cat = _hb_glyph_info_get_general_category (&info[i]);
    ఒకవేళ (FLAG_UNSAFE (gen_cat) &
	(FLAG (HB_UNICODE_GENERAL_CATEGORY_LOWERCASE_LETTER) |
	 FLAG (HB_UNICODE_GENERAL_CATEGORY_UPPERCASE_LETTER) |
	 FLAG (HB_UNICODE_GENERAL_CATEGORY_TITLECASE_LETTER) |
	 FLAG (HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER) |
	 FLAG (HB_UNICODE_GENERAL_CATEGORY_SPACE_SEPARATOR)))
      కొనసాగు;

    /* Marks are already set as continuation by the above line.
     * Handle Emoji_Modifier and ZWJ-continuation. */
    ఒకవేళ (unlikely (gen_cat == HB_UNICODE_GENERAL_CATEGORY_MODIFIER_చిహ్నం &&
		  hb_లోపల_range<hb_codepoint_t> (info[i].codepoint, 0x1F3FBu, 0x1F3FFu)))
    {
      hb_glyph_info_సమితి_continuation (&info[i], బఫరు);
    }
    /* Regional_Indicators are hairy as hell...
     * https://github.com/harfbuzz/harfbuzz/issues/2265 */
    లేకపోతే ఒకవేళ (unlikely (i && _hb_codepoint_is_regional_indicator (info[i].codepoint)))
    {
      ఒకవేళ (_hb_codepoint_is_regional_indicator (info[i - 1].codepoint) &&
	  !_hb_glyph_info_is_continuation (&info[i - 1]))
	hb_glyph_info_సమితి_continuation (&info[i], బఫరు);
    }
#నిర్వచితం_కాకపోతే HB_NO_EMOJI_SEQUENCES
    లేకపోతే ఒకవేళ (unlikely (_hb_glyph_info_is_zwj (&info[i])))
    {
      hb_glyph_info_సమితి_continuation (&info[i], బఫరు);
      ఒకవేళ (i + 1 < లెక్క &&
	  _hb_unicode_is_emoji_Extended_Pictographic (info[i + 1].codepoint))
      {
	i++;
	info[i].ముసుగు = global_ముసుగు;
	hb_glyph_info_సమితి_unicode_props (&info[i], బఫరు);
	hb_glyph_info_సమితి_continuation (&info[i], బఫరు);
      }
    }
#అయితే_ముగింపు
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
    లేకపోతే ఒకవేళ (unlikely (hb_లోపల_ranges<hb_codepoint_t> (info[i].codepoint, 0xFF9Eu, 0xFF9Fu, 0xE0020u, 0xE007Fu)))
      hb_glyph_info_సమితి_continuation (&info[i], బఫరు);
    లేకపోతే ఒకవేళ (unlikely (info[i].codepoint == 0x2044u /* FRACTION SLASH */))
      బఫరు->scratch_flags |= HB_బఫరు_SCRATCH_FLAG_HAS_FRACTION_SLASH;
  }
}

స్థిర శూన్య
hb_చొప్పించు_dotted_circle (hb_బఫరు_t *బఫరు, hb_font_t *font)
{
  ఒకవేళ (unlikely (బఫరు->flags & HB_బఫరు_FLAG_DO_NOT_చొప్పించు_DOTTED_CIRCLE))
    తిరిగి;

  ఒకవేళ (!(బఫరు->flags & HB_బఫరు_FLAG_BOT) ||
      బఫరు->context_len[0] ||
      !_hb_glyph_info_is_unicode_mark (&బఫరు->info[0]))
    తిరిగి;

  ఒకవేళ (!font->has_glyph (0x25CCu))
    తిరిగి;

  hb_glyph_info_t dottedcircle = {0};
  dottedcircle.codepoint = 0x25CCu;
  hb_glyph_info_సమితి_unicode_props (&dottedcircle, బఫరు);

  బఫరు->clear_output ();

  బఫరు->idx = 0;
  hb_glyph_info_t info = dottedcircle;
  info.cluster = బఫరు->cur().cluster;
  info.ముసుగు = బఫరు->cur().ముసుగు;
  (శూన్య) బఫరు->output_info (info);

  బఫరు->సమకాలీకరణ ();
}

స్థిర శూన్య
hb_form_clusters (hb_బఫరు_t *బఫరు)
{
  ఒకవేళ (!(బఫరు->scratch_flags & HB_బఫరు_SCRATCH_FLAG_HAS_CONTINUATIONS))
    తిరిగి;

  foreach_grapheme (బఫరు, ప్రారంభం, చివర)
    బఫరు->విలీనం_grapheme_clusters (ప్రారంభం, చివర);
}

స్థిర శూన్య
hb_ensure_native_direction (hb_బఫరు_t *బఫరు)
{
  hb_direction_t direction = బఫరు->props.direction;
  hb_direction_t horiz_dir = hb_script_get_horizontal_direction (బఫరు->props.script);

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
  ఒకవేళ (unlikely (horiz_dir == HB_DIRECTION_RTL && direction == HB_DIRECTION_LTR))
  {
    తార్కిక దొరికింది_సంఖ్య = అబద్ధం, దొరికింది_letter = అబద్ధం, దొరికింది_ri = అబద్ధం;
    స్థిరరాశి స్వయంచాలక* info = బఫరు->info;
    స్థిరరాశి స్వయంచాలక లెక్క = బఫరు->len;
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
    {
      స్వయంచాలక gc = _hb_glyph_info_get_general_category (&info[i]);
      ఒకవేళ (HB_UNICODE_GENERAL_CATEGORY_IS_LETTER (gc))
      {
	దొరికింది_letter = నిజం;
	ఆపు;
      }
      లేకపోతే ఒకవేళ (gc == HB_UNICODE_GENERAL_CATEGORY_DECIMAL_సంఖ్య)
	దొరికింది_సంఖ్య = నిజం;
      లేకపోతే ఒకవేళ (unlikely (_hb_codepoint_is_regional_indicator (info[i].codepoint)))
	దొరికింది_ri = నిజం;
    }
    ఒకవేళ ((దొరికింది_సంఖ్య || దొరికింది_ri) && !దొరికింది_letter)
      horiz_dir = HB_DIRECTION_LTR;
  }

  /* TODO vertical:
   * The only BTT vertical script is Ogham, but it's not clear to me whether OpenType
   * Ogham fonts are supposed to be implemented BTT or not.  Need to research that
   * first. */
  ఒకవేళ ((HB_DIRECTION_IS_HORIZONTAL (direction) &&
       direction != horiz_dir && HB_DIRECTION_IS_VALID (horiz_dir)) ||
      (HB_DIRECTION_IS_VERTICAL   (direction) &&
       direction != HB_DIRECTION_TTB))
  {
    hb_ot_layout_తిరగేయి_graphemes (బఫరు);
    బఫరు->props.direction = HB_DIRECTION_తిరగేయి (బఫరు->props.direction);
  }
}


/*
 * Substitute
 */

స్థిర అంతఃవరుస శూన్య
hb_ot_rotate_chars (స్థిరరాశి hb_ot_shape_context_t *c)
{
  hb_బఫరు_t *బఫరు = c->బఫరు;
  గుర్తులేని పూర్ణాంకం లెక్క = బఫరు->len;
  hb_glyph_info_t *info = బఫరు->info;

  ఒకవేళ (HB_DIRECTION_IS_BACKWARD (c->లక్ష్యం_direction))
  {
    hb_unicode_funcs_t *unicode = బఫరు->unicode;
    hb_ముసుగు_t rtlm_ముసుగు = c->plan->rtlm_ముసుగు;

    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++) {
      hb_codepoint_t codepoint = unicode->mirroring (info[i].codepoint);
      ఒకవేళ (unlikely (codepoint != info[i].codepoint && c->font->has_glyph (codepoint)))
	info[i].codepoint = codepoint;
      లేకపోతే
	info[i].ముసుగు |= rtlm_ముసుగు;
    }
  }

#నిర్వచితం_కాకపోతే HB_NO_VERTICAL
  ఒకవేళ (HB_DIRECTION_IS_VERTICAL (c->లక్ష్యం_direction) && !c->plan->has_vert)
  {
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++) {
      hb_codepoint_t codepoint = hb_unicode_funcs_t::vertical_char_for (info[i].codepoint);
      ఒకవేళ (unlikely (codepoint != info[i].codepoint && c->font->has_glyph (codepoint)))
	info[i].codepoint = codepoint;
    }
  }
#అయితే_ముగింపు
}

స్థిర అంతఃవరుస శూన్య
hb_ot_shape_అమరిక_masks_fraction (స్థిరరాశి hb_ot_shape_context_t *c)
{
#నిర్వచితమైతే HB_NO_OT_SHAPE_FRACTIONS
  తిరిగి;
#అయితే_ముగింపు

  ఒకవేళ (!(c->బఫరు->scratch_flags & HB_బఫరు_SCRATCH_FLAG_HAS_FRACTION_SLASH) ||
      !c->plan->has_frac)
    తిరిగి;

  hb_బఫరు_t *బఫరు = c->బఫరు;

  hb_ముసుగు_t pre_ముసుగు, post_ముసుగు;
  ఒకవేళ (HB_DIRECTION_IS_FORWARD (బఫరు->props.direction))
  {
    pre_ముసుగు = c->plan->numr_ముసుగు | c->plan->frac_ముసుగు;
    post_ముసుగు = c->plan->frac_ముసుగు | c->plan->dnom_ముసుగు;
  }
  లేకపోతే
  {
    pre_ముసుగు = c->plan->frac_ముసుగు | c->plan->dnom_ముసుగు;
    post_ముసుగు = c->plan->numr_ముసుగు | c->plan->frac_ముసుగు;
  }

  గుర్తులేని పూర్ణాంకం లెక్క = బఫరు->len;
  hb_glyph_info_t *info = బఫరు->info;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
  {
    ఒకవేళ (info[i].codepoint == 0x2044u) /* FRACTION SLASH */
    {
      గుర్తులేని పూర్ణాంకం ప్రారంభం = i, చివర = i + 1;
      అయితే (ప్రారంభం &&
	     _hb_glyph_info_get_general_category (&info[ప్రారంభం - 1]) ==
	     HB_UNICODE_GENERAL_CATEGORY_DECIMAL_సంఖ్య)
	ప్రారంభం--;
      అయితే (చివర < లెక్క &&
	     _hb_glyph_info_get_general_category (&info[చివర]) ==
	     HB_UNICODE_GENERAL_CATEGORY_DECIMAL_సంఖ్య)
	చివర++;
      ఒకవేళ (ప్రారంభం == i || చివర == i + 1)
      {
        ఒకవేళ (ప్రారంభం == i)
	  బఫరు->unsafe_గమ్యం_concat (ప్రారంభం, ప్రారంభం + 1);
	ఒకవేళ (చివర == i + 1)
	  బఫరు->unsafe_గమ్యం_concat (చివర - 1, చివర);
	కొనసాగు;
      }

      బఫరు->unsafe_గమ్యం_break (ప్రారంభం, చివర);

      కొరకు (గుర్తులేని పూర్ణాంకం j = ప్రారంభం; j < i; j++)
	info[j].ముసుగు |= pre_ముసుగు;
      info[i].ముసుగు |= c->plan->frac_ముసుగు;
      కొరకు (గుర్తులేని పూర్ణాంకం j = i + 1; j < చివర; j++)
	info[j].ముసుగు |= post_ముసుగు;

      i = చివర - 1;
    }
  }
}

స్థిర అంతఃవరుస శూన్య
hb_ot_shape_అమరిక_masks (స్థిరరాశి hb_ot_shape_context_t *c)
{
  hb_ot_map_t *map = &c->plan->map;
  hb_బఫరు_t *బఫరు = c->బఫరు;

  hb_ot_shape_అమరిక_masks_fraction (c);

  ఒకవేళ (c->plan->shaper->అమరిక_masks)
    c->plan->shaper->అమరిక_masks (c->plan, బఫరు, c->font);

  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < c->అంకె_user_features; i++)
  {
    స్థిరరాశి hb_feature_t *feature = &c->user_features[i];
    ఒకవేళ (!(feature->ప్రారంభం == HB_FEATURE_GLOBAL_ప్రారంభం && feature->చివర == HB_FEATURE_GLOBAL_చివర)) {
      గుర్తులేని పూర్ణాంకం జరుపు;
      hb_ముసుగు_t ముసుగు = map->get_ముసుగు (feature->ట్యాగ్_2, &జరుపు);
      బఫరు->సమితి_masks (feature->value << జరుపు, ముసుగు, feature->ప్రారంభం, feature->చివర);
    }
  }
}

స్థిర శూన్య
hb_ot_zero_వెడల్పు_default_ignorables (స్థిరరాశి hb_బఫరు_t *బఫరు)
{
  ఒకవేళ (!(బఫరు->scratch_flags & HB_బఫరు_SCRATCH_FLAG_HAS_DEFAULT_IGNORABLES) ||
      (బఫరు->flags & HB_బఫరు_FLAG_PRESERVE_DEFAULT_IGNORABLES) ||
      (బఫరు->flags & HB_బఫరు_FLAG_REMOVE_DEFAULT_IGNORABLES))
    తిరిగి;

  గుర్తులేని పూర్ణాంకం లెక్క = బఫరు->len;
  hb_glyph_info_t *info = బఫరు->info;
  hb_glyph_position_t *pos = బఫరు->pos;
  గుర్తులేని పూర్ణాంకం i = 0;
  కొరకు (i = 0; i < లెక్క; i++)
    ఒకవేళ (unlikely (_hb_glyph_info_is_default_ignorable (&info[i])))
    {
      pos[i].x_advance = pos[i].y_advance = 0;
      ఒకవేళ (HB_DIRECTION_IS_HORIZONTAL (బఫరు->props.direction))
	pos[i].x_స్థానభ్రంశం_2 = 0;
      లేకపోతే
        pos[i].y_స్థానభ్రంశం_2 = 0;
    }
}

స్థిర శూన్య
hb_ot_deal_with_variation_selectors (hb_బఫరు_t *బఫరు)
{
  ఒకవేళ (!(బఫరు->scratch_flags & HB_బఫరు_SCRATCH_FLAG_HAS_VARIATION_SELECTOR_FALLBACK) ||
	బఫరు->not_దొరికింది_variation_selector == HB_CODEPOINT_INVALID)
    తిరిగి;

  గుర్తులేని పూర్ణాంకం లెక్క = బఫరు->len;
  hb_glyph_info_t *info = బఫరు->info;
  hb_glyph_position_t *pos = బఫరు->pos;

  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
  {
    ఒకవేళ (_hb_glyph_info_is_variation_selector (&info[i]))
    {
      info[i].codepoint = బఫరు->not_దొరికింది_variation_selector;
      pos[i].x_advance = pos[i].y_advance = pos[i].x_స్థానభ్రంశం_2 = pos[i].y_స్థానభ్రంశం_2 = 0;
      hb_glyph_info_సమితి_variation_selector (&info[i], అబద్ధం);
    }
  }
}

స్థిర శూన్య
hb_ot_hide_default_ignorables (hb_బఫరు_t *బఫరు,
			       hb_font_t   *font)
{
  ఒకవేళ (!(బఫరు->scratch_flags & HB_బఫరు_SCRATCH_FLAG_HAS_DEFAULT_IGNORABLES) ||
      (బఫరు->flags & HB_బఫరు_FLAG_PRESERVE_DEFAULT_IGNORABLES))
    తిరిగి;

  గుర్తులేని పూర్ణాంకం లెక్క = బఫరు->len;
  hb_glyph_info_t *info = బఫరు->info;

  hb_codepoint_t invisible = బఫరు->invisible;
  ఒకవేళ (!(బఫరు->flags & HB_బఫరు_FLAG_REMOVE_DEFAULT_IGNORABLES) &&
      (invisible || font->get_nominal_glyph (' ', &invisible)))
  {
    /* Replace default-ignorables with a zero-advance invisible glyph. */
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      ఒకవేళ (_hb_glyph_info_is_default_ignorable (&info[i]))
	info[i].codepoint = invisible;
    }
  }
  లేకపోతే
    బఫరు->delete_glyphs_inplace (_hb_glyph_info_is_default_ignorable);
}


స్థిర అంతఃవరుస శూన్య
hb_ot_map_glyphs_fast (hb_బఫరు_t  *బఫరు)
{
  /* Normalization process sets up normalizer_glyph_index(), we just copy it. */
  గుర్తులేని పూర్ణాంకం లెక్క = బఫరు->len;
  hb_glyph_info_t *info = బఫరు->info;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    info[i].codepoint = info[i].normalizer_glyph_సూచీ();

  బఫరు->content_type = HB_బఫరు_CONTENT_TYPE_GLYPHS;
}

స్థిర అంతఃవరుస శూన్య
hb_synthesize_glyph_classes (hb_బఫరు_t *బఫరు)
{
  గుర్తులేని పూర్ణాంకం లెక్క = బఫరు->len;
  hb_glyph_info_t *info = బఫరు->info;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
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
	    HB_OT_LAYOUT_GLYPH_PROPS_పునాది_GLYPH :
	    HB_OT_LAYOUT_GLYPH_PROPS_MARK;
    hb_glyph_info_సమితి_glyph_props (&info[i], klass);
  }
}

స్థిర అంతఃవరుస శూన్య
hb_ot_substitute_default (స్థిరరాశి hb_ot_shape_context_t *c)
{
  hb_బఫరు_t *బఫరు = c->బఫరు;

  hb_ot_rotate_chars (c);

  HB_బఫరు_ALLOCATE_VAR (బఫరు, normalizer_glyph_సూచీ);

  _hb_ot_shape_normalize (c->plan, బఫరు, c->font);

  hb_ot_shape_అమరిక_masks (c);

  /* This is unfortunate to go here, but necessary... */
  ఒకవేళ (c->plan->fallback_mark_positioning)
    _hb_ot_shape_fallback_mark_position_recategorize_marks (c->plan, c->font, బఫరు);

  hb_ot_map_glyphs_fast (బఫరు);

  HB_బఫరు_DEALLOCATE_VAR (బఫరు, normalizer_glyph_సూచీ);
}

స్థిర అంతఃవరుస శూన్య
hb_ot_substitute_plan (స్థిరరాశి hb_ot_shape_context_t *c)
{
  hb_బఫరు_t *బఫరు = c->బఫరు;

#నిర్వచితం_కాకపోతే HB_NO_AAT_SHAPE
  ఒకవేళ (unlikely (c->plan->apply_morx))
  {
    hb_ot_layout_substitute_ప్రారంభం (c->font, బఫరు);

    ఒకవేళ (c->plan->fallback_glyph_classes)
      hb_synthesize_glyph_classes (బఫరు);

    hb_aat_layout_substitute (c->plan, c->font, c->బఫరు,
			      c->user_features, c->అంకె_user_features);
    /* The buffer digest is only used by the OT lookup-apply loop;
     * without GPOS ahead, nothing consumes it. */
    ఒకవేళ (c->plan->apply_gpos)
      c->బఫరు->update_digest ();
  }
  లేకపోతే
#అయితే_ముగింపు
  {
    hb_ot_layout_substitute_ప్రారంభం_with_digest (c->font, బఫరు);

    ఒకవేళ (c->plan->fallback_glyph_classes)
      hb_synthesize_glyph_classes (బఫరు);

    c->plan->substitute (c->font, బఫరు);
  }
}

స్థిర అంతఃవరుస శూన్య
hb_ot_substitute_pre (స్థిరరాశి hb_ot_shape_context_t *c)
{
  hb_ot_substitute_default (c);

  hb_బఫరు_allocate_gsubgpos_vars (c->బఫరు);

  hb_ot_substitute_plan (c);

#నిర్వచితం_కాకపోతే HB_NO_AAT_SHAPE
  ఒకవేళ (c->plan->apply_morx && c->plan->apply_gpos)
    hb_aat_layout_remove_deleted_glyphs (c->బఫరు);
#అయితే_ముగింపు
}

స్థిర అంతఃవరుస శూన్య
hb_ot_substitute_post (స్థిరరాశి hb_ot_shape_context_t *c)
{
#నిర్వచితం_కాకపోతే HB_NO_AAT_SHAPE
  ఒకవేళ (c->plan->apply_morx && !c->plan->apply_gpos)
    hb_aat_layout_remove_deleted_glyphs (c->బఫరు);
#అయితే_ముగింపు

  hb_ot_deal_with_variation_selectors (c->బఫరు);
  hb_ot_hide_default_ignorables (c->బఫరు, c->font);

  ఒకవేళ (c->plan->shaper->postprocess_glyphs &&
    c->బఫరు->సందేశం(c->font, "start postprocess-glyphs")) {
    c->plan->shaper->postprocess_glyphs (c->plan, c->బఫరు, c->font);
    (శూన్య) c->బఫరు->సందేశం(c->font, "end postprocess-glyphs");
  }
}


/*
 * Position
 */

స్థిర అంతఃవరుస శూన్య
adjust_mark_offsets (hb_glyph_position_t *pos)
{
  pos->x_స్థానభ్రంశం_2 = hb_saturate_sub (pos->x_స్థానభ్రంశం_2, pos->x_advance);
  pos->y_స్థానభ్రంశం_2 = hb_saturate_sub (pos->y_స్థానభ్రంశం_2, pos->y_advance);
}

స్థిర అంతఃవరుస శూన్య
zero_mark_వెడల్పు (hb_glyph_position_t *pos)
{
  pos->x_advance = 0;
  pos->y_advance = 0;
}

స్థిర అంతఃవరుస శూన్య
zero_mark_widths_by_gdef (hb_బఫరు_t *బఫరు, తార్కిక adjust_offsets)
{
  గుర్తులేని పూర్ణాంకం లెక్క = బఫరు->len;
  hb_glyph_info_t *info = బఫరు->info;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    ఒకవేళ (_hb_glyph_info_is_mark (&info[i]))
    {
      ఒకవేళ (adjust_offsets)
	adjust_mark_offsets (&బఫరు->pos[i]);
      zero_mark_వెడల్పు (&బఫరు->pos[i]);
    }
}

స్థిర అంతఃవరుస శూన్య
hb_ot_position_default (స్థిరరాశి hb_ot_shape_context_t *c)
{
  hb_direction_t direction = c->బఫరు->props.direction;
  గుర్తులేని పూర్ణాంకం లెక్క = c->బఫరు->len;
  hb_glyph_info_t *info = c->బఫరు->info;
  hb_glyph_position_t *pos = c->బఫరు->pos;

  ఒకవేళ (HB_DIRECTION_IS_HORIZONTAL (direction))
  {
    c->font->get_glyph_h_advances (లెక్క, &info[0].codepoint, పరిమాణం(info[0]),
				   &pos[0].x_advance, పరిమాణం(pos[0]));
    // h_origin defaults to zero; only apply it if the font has it.
    ఒకవేళ (c->font->has_glyph_h_origin_func () || c->font->has_glyph_h_origins_func ())
      c->font->వ్యవకలనం_glyph_h_origins (c->బఫరు);
  }
  లేకపోతే
  {
    c->font->get_glyph_v_advances (లెక్క, &info[0].codepoint, పరిమాణం(info[0]),
				   &pos[0].y_advance, పరిమాణం(pos[0]));
    // v_origin defaults to non-zero; apply even if only fallback is there.
    c->font->వ్యవకలనం_glyph_v_origins (c->బఫరు);
  }
  ఒకవేళ (c->బఫరు->scratch_flags & HB_బఫరు_SCRATCH_FLAG_HAS_SPACE_FALLBACK)
    _hb_ot_shape_fallback_spaces (c->plan, c->font, c->బఫరు);
}

స్థిర అంతఃవరుస శూన్య
hb_ot_position_plan (స్థిరరాశి hb_ot_shape_context_t *c)
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
  తార్కిక adjust_offsets_when_zeroing = c->plan->adjust_mark_positioning_when_zeroing &&
				     HB_DIRECTION_IS_FORWARD (c->బఫరు->props.direction);

  /* We change glyph origin to what GPOS expects (horizontal), apply GPOS, change it back. */

  // h_origin defaults to zero; only apply it if the font has it.
  ఒకవేళ (c->font->has_glyph_h_origin_func () || c->font->has_glyph_h_origins_func ())
    c->font->కూడు_glyph_h_origins (c->బఫరు);

  hb_ot_layout_position_ప్రారంభం (c->font, c->బఫరు);

  ఒకవేళ (c->plan->zero_marks)
    ఎంపిక (c->plan->shaper->zero_వెడల్పు_marks)
    {
      స్థితి HB_OT_SHAPE_ZERO_వెడల్పు_MARKS_BY_GDEF_EARLY:
	zero_mark_widths_by_gdef (c->బఫరు, adjust_offsets_when_zeroing);
	ఆపు;

      అప్రమేయ:
      స్థితి HB_OT_SHAPE_ZERO_వెడల్పు_MARKS_NONE:
      స్థితి HB_OT_SHAPE_ZERO_వెడల్పు_MARKS_BY_GDEF_LATE:
	ఆపు;
    }

  c->plan->position (c->font, c->బఫరు);

  ఒకవేళ (c->plan->zero_marks)
    ఎంపిక (c->plan->shaper->zero_వెడల్పు_marks)
    {
      స్థితి HB_OT_SHAPE_ZERO_వెడల్పు_MARKS_BY_GDEF_LATE:
	zero_mark_widths_by_gdef (c->బఫరు, adjust_offsets_when_zeroing);
	ఆపు;

      అప్రమేయ:
      స్థితి HB_OT_SHAPE_ZERO_వెడల్పు_MARKS_NONE:
      స్థితి HB_OT_SHAPE_ZERO_వెడల్పు_MARKS_BY_GDEF_EARLY:
	ఆపు;
    }

  /* Finish off.  Has to follow a certain order. */
  hb_ot_layout_position_ముగింపు_advances (c->font, c->బఫరు);
  hb_ot_zero_వెడల్పు_default_ignorables (c->బఫరు);
  hb_ot_layout_position_ముగింపు_offsets (c->font, c->బఫరు);

  // h_origin defaults to zero; only apply it if the font has it.
  ఒకవేళ (c->font->has_glyph_h_origin_func () || c->font->has_glyph_h_origins_func ())
    c->font->వ్యవకలనం_glyph_h_origins (c->బఫరు);

  ఒకవేళ (c->plan->fallback_mark_positioning)
    _hb_ot_shape_fallback_mark_position (c->plan, c->font, c->బఫరు,
					 adjust_offsets_when_zeroing);
}

స్థిర అంతఃవరుస శూన్య
hb_ot_position (స్థిరరాశి hb_ot_shape_context_t *c)
{
  c->బఫరు->clear_positions ();

  hb_ot_position_default (c);

  hb_ot_position_plan (c);

  ఒకవేళ (HB_DIRECTION_IS_BACKWARD (c->బఫరు->props.direction))
    hb_బఫరు_తిరగేయి (c->బఫరు);

  hb_బఫరు_deallocate_gsubgpos_vars (c->బఫరు);
}

స్థిర అంతఃవరుస శూన్య
hb_propagate_flags (hb_బఫరు_t *బఫరు)
{
  /* Propagate cluster-level glyph flags to be the same on all cluster glyphs.
   * Simplifies using them. */

  hb_ముసుగు_t and_ముసుగు = HB_GLYPH_FLAG_DEFINED;
  ఒకవేళ ((బఫరు->flags & HB_బఫరు_FLAG_PRODUCE_UNSAFE_గమ్యం_CONCAT) == 0)
    and_ముసుగు &= ~HB_GLYPH_FLAG_UNSAFE_గమ్యం_CONCAT;

  hb_glyph_info_t *info = బఫరు->info;

  ఒకవేళ ((బఫరు->flags & HB_బఫరు_FLAG_PRODUCE_సురక్షిత_గమ్యం_చొప్పించు_TATWEEL) == 0)
  {
    foreach_cluster (బఫరు, ప్రారంభం, చివర)
    {
      ఒకవేళ (చివర - ప్రారంభం == 1)
      {
        info[ప్రారంభం].ముసుగు &= and_ముసుగు;
	కొనసాగు;
      }

      గుర్తులేని పూర్ణాంకం ముసుగు = 0;
      కొరకు (గుర్తులేని పూర్ణాంకం i = ప్రారంభం; i < చివర; i++)
	ముసుగు |= info[i].ముసుగు;

      ముసుగు &= and_ముసుగు;

      కొరకు (గుర్తులేని పూర్ణాంకం i = ప్రారంభం; i < చివర; i++)
	info[i].ముసుగు = ముసుగు;
    }
    తిరిగి;
  }

  /* If we are producing SAFE_TO_INSERT_TATWEEL, then do two things:
   *
   * - If the places that the Arabic shaper marked as SAFE_TO_INSERT_TATWEEL,
   *   are UNSAFE_TO_BREAK, then clear the SAFE_TO_INSERT_TATWEEL,
   * - Any place that is SAFE_TO_INSERT_TATWEEL, is also now UNSAFE_TO_BREAK.
   *
   * We couldn't make this interaction earlier. It has to be done this way.
   */
  foreach_cluster (బఫరు, ప్రారంభం, చివర)
  {
    గుర్తులేని పూర్ణాంకం ముసుగు = 0;
    కొరకు (గుర్తులేని పూర్ణాంకం i = ప్రారంభం; i < చివర; i++)
      ముసుగు |= info[i].ముసుగు;

    ఒకవేళ (ముసుగు & HB_GLYPH_FLAG_UNSAFE_గమ్యం_BREAK)
      ముసుగు &= ~HB_GLYPH_FLAG_సురక్షిత_గమ్యం_చొప్పించు_TATWEEL;
    ఒకవేళ (ముసుగు & HB_GLYPH_FLAG_సురక్షిత_గమ్యం_చొప్పించు_TATWEEL)
      ముసుగు |= HB_GLYPH_FLAG_UNSAFE_గమ్యం_BREAK | HB_GLYPH_FLAG_UNSAFE_గమ్యం_CONCAT;

    ముసుగు &= and_ముసుగు;

    కొరకు (గుర్తులేని పూర్ణాంకం i = ప్రారంభం; i < చివర; i++)
      info[i].ముసుగు = ముసుగు;
  }
}

/* Pull it all together! */

స్థిర శూన్య
hb_ot_shape_అంతర్గత (hb_ot_shape_context_t *c)
{
  /* Save the original direction, we use it later. */
  c->లక్ష్యం_direction = c->బఫరు->props.direction;

  hb_బఫరు_allocate_unicode_vars (c->బఫరు);

  hb_సమితి_unicode_props (c->బఫరు, c->plan->map.get_global_ముసుగు ());
  hb_చొప్పించు_dotted_circle (c->బఫరు, c->font);

  hb_form_clusters (c->బఫరు);

  hb_ensure_native_direction (c->బఫరు);

  ఒకవేళ (c->plan->shaper->preprocess_పాఠ్యం &&
      c->బఫరు->సందేశం(c->font, "start preprocess-text"))
  {
    c->plan->shaper->preprocess_పాఠ్యం (c->plan, c->బఫరు, c->font);
    (శూన్య) c->బఫరు->సందేశం(c->font, "end preprocess-text");
  }

  hb_ot_substitute_pre (c);
  hb_ot_position (c);
  hb_ot_substitute_post (c);

  hb_propagate_flags (c->బఫరు);

  hb_బఫరు_deallocate_unicode_vars (c->బఫరు);

  c->బఫరు->props.direction = c->లక్ష్యం_direction;
}


hb_bool_t
_hb_ot_shape (hb_shape_plan_t    *shape_plan,
	      hb_font_t          *font,
	      hb_బఫరు_t        *బఫరు,
	      స్థిరరాశి hb_feature_t *features,
	      గుర్తులేని పూర్ణాంకం        అంకె_features)
{
  hb_ot_shape_context_t c = {&shape_plan->ot, font, font->face, బఫరు, features, అంకె_features};
  hb_ot_shape_అంతర్గత (&c);

  తిరిగి నిజం;
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
శూన్య
hb_ot_shape_plan_collect_lookups (hb_shape_plan_t *shape_plan,
				  hb_ట్యాగ్_t         పట్టిక_ట్యాగ్_2,
				  hb_సమితి_t        *lookup_indexes /*బహిర్గతం*/)
{
  shape_plan->ot.collect_lookups (పట్టిక_ట్యాగ్_2, lookup_indexes);
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
గుర్తులేని పూర్ణాంకం
hb_ot_shape_plan_get_feature_ట్యాగులు (hb_shape_plan_t *shape_plan,
				   గుర్తులేని పూర్ణాంకం     ప్రారంభం_స్థానభ్రంశం,
				   గుర్తులేని పూర్ణాంకం    *ట్యాగ్_లెక్క, /* IN/OUT */
				   hb_ట్యాగ్_t        *ట్యాగులు /*బహిర్గతం*/)
{
#నిర్వచితం_కాకపోతే HB_NO_OT_SHAPE
  తిరిగి shape_plan->ot.map.get_feature_ట్యాగులు (ప్రారంభం_స్థానభ్రంశం, ట్యాగ్_లెక్క, ట్యాగులు);
#లేకపోతే
  ఒకవేళ (ట్యాగ్_లెక్క)
	*ట్యాగ్_లెక్క = 0;
  తిరిగి 0;
#అయితే_ముగింపు
}


/* TODO Move this to hb-ot-shape-normalize, make it do decompose, and make it public. */
స్థిర శూన్య
కూడు_char (hb_font_t          *font,
	  hb_unicode_funcs_t *unicode,
	  hb_bool_t           mirror,
	  hb_codepoint_t      u,
	  hb_సమితి_t           *glyphs)
{
  hb_codepoint_t glyph;
  ఒకవేళ (font->get_nominal_glyph (u, &glyph))
    glyphs->కూడు (glyph);
  ఒకవేళ (mirror)
  {
    hb_codepoint_t m = unicode->mirroring (u);
    ఒకవేళ (m != u && font->get_nominal_glyph (m, &glyph))
      glyphs->కూడు (glyph);
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
శూన్య
hb_ot_shape_glyphs_సంవృతం (hb_font_t          *font,
			    hb_బఫరు_t        *బఫరు,
			    స్థిరరాశి hb_feature_t *features,
			    గుర్తులేని పూర్ణాంకం        అంకె_features,
			    hb_సమితి_t           *glyphs)
{
  స్థిరరాశి అక్షరం *shapers[] = {"ot", శూన్యసూచిక};
  hb_shape_plan_t *shape_plan = hb_shape_plan_create_cached (font->face, &బఫరు->props,
							     features, అంకె_features, shapers);

  తార్కిక mirror = hb_script_get_horizontal_direction (బఫరు->props.script) == HB_DIRECTION_RTL;

  గుర్తులేని పూర్ణాంకం లెక్క = బఫరు->len;
  hb_glyph_info_t *info = బఫరు->info;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    కూడు_char (font, బఫరు->unicode, mirror, info[i].codepoint, glyphs);

  hb_సమితి_t *lookups = hb_సమితి_create ();
  hb_ot_shape_plan_collect_lookups (shape_plan, HB_OT_ట్యాగ్_GSUB, lookups);
  hb_ot_layout_lookups_substitute_సంవృతం (font->face, lookups, glyphs);

  hb_సమితి_destroy (lookups);

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
గుర్తులేని పూర్ణాంకం
hb_ot_shape_get_బఫరు_format_serial (శూన్య)
{
  తిరిగి HB_OT_SHAPE_బఫరు_FORMAT_SERIAL;
}

#అయితే_ముగింపు

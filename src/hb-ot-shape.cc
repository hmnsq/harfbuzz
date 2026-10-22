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

#ضمن "hb.hh"

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_SHAPE

#جيڪڏهن_وصف_ٿيل HB_NO_OT_LAYOUT
#غلطي "Cannot compile 'ot' shaper with HB_NO_OT_LAYOUT."
#جيڪڏهن_پورو

#ضمن "hb-shaper-impl.hh"

#ضمن "hb-ot-shape.hh"
#ضمن "hb-ot-shaper.hh"
#ضمن "hb-ot-shape-fallback.hh"
#ضمن "hb-ot-shape-normalize.hh"

#ضمن "hb-ot-face.hh"

#ضمن "hb-set.hh"
#ضمن "hb-unicode.hh"

#ضمن "hb-aat-layout.hh"
#ضمن "hb-ot-layout-gdef-table.hh"
#ضمن "hb-ot-stat-table.hh"


ساكن مضمن منطقي
_hb_codepoint_is_regional_indicator (hb_codepoint_t u)
{ أرجع hb_اندريون_range<hb_codepoint_t> (u, 0x1F1E6u, 0x1F1FFu); }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_AAT_SHAPE
ساكن مضمن منطقي
_hb_apply_morx (hb_face_t *face, ثابت hb_segment_properties_t &props)
{
  /* https://github.com/harfbuzz/harfbuzz/issues/2124 */
  أرجع hb_aat_layout_has_substitution (face) &&
	 (HB_DIRECTION_IS_HORIZONTAL (props.direction) || !hb_ot_layout_has_substitution (face));
}
#جيڪڏهن_پورو

/**
 * SECTION:hb-ot-shape
 * @title: hb-ot-shape
 * @short_description: OpenType shaping support
 * @include: hb-ot.h
 *
 * Support functions for OpenType shaping related queries.
 **/


ساكن فراغ
hb_ot_shape_collect_features (hb_ot_shape_planner_t          *planner,
			      ثابت hb_feature_t             *user_features,
			      بنانشان صحيح                    انگ_user_features);

hb_ot_shape_planner_t::hb_ot_shape_planner_t (hb_face_t                     *face,
					      ثابت hb_segment_properties_t &props) :
						face (face),
						props (props),
						map (face, props)
#جيڪڏهن_وصف_نه_ٿيل HB_NO_AAT_SHAPE
						, aat_map (face, props)
						, apply_morx (_hb_apply_morx (face, props))
#جيڪڏهن_پورو
{
  shaper = hb_ot_shaper_categorize (props.script, props.direction, map.chosen_script[0]);

  script_zero_marks = shaper->zero_ويڪر_marks != HB_OT_SHAPE_ZERO_ويڪر_MARKS_NONE;
  script_fallback_position = shaper->fallback_position;

#جيڪڏهن_وصف_نه_ٿيل HB_NO_AAT_SHAPE
  /* https://github.com/harfbuzz/harfbuzz/issues/1528 */
  إذا (apply_morx && shaper != &_hb_ot_shaper_default)
    shaper = &_hb_ot_shaper_dumber;
#جيڪڏهن_پورو
}

فراغ
hb_ot_shape_planner_t::compile (hb_ot_shape_plan_t           &plan,
				ثابت hb_ot_shape_plan_ڪنجي_t &key)
{
  plan.props = props;
  plan.shaper = shaper;
  map.compile (plan.map, key);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_AAT_SHAPE
  إذا (apply_morx)
    aat_map.compile (plan.aat_map);
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_SHAPE_FRACTIONS
  plan.frac_نقاب = plan.map.get_1_نقاب (HB_ٽيگ ('f','r','a','c'));
  plan.numr_نقاب = plan.map.get_1_نقاب (HB_ٽيگ ('n','u','m','r'));
  plan.dnom_نقاب = plan.map.get_1_نقاب (HB_ٽيگ ('d','n','o','m'));
  plan.has_frac = plan.frac_نقاب || (plan.numr_نقاب && plan.dnom_نقاب);
#جيڪڏهن_پورو

  plan.rtlm_نقاب = plan.map.get_1_نقاب (HB_ٽيگ ('r','t','l','m'));
  plan.has_vert = !!plan.map.get_1_نقاب (HB_ٽيگ ('v','e','r','t'));

  hb_ٽيگ_t kern_ٽيگ = HB_DIRECTION_IS_HORIZONTAL (props.direction) ?
		      HB_ٽيگ ('k','e','r','n') : HB_ٽيگ ('v','k','r','n');
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_KERN
  plan.kern_نقاب = plan.map.get_نقاب (kern_ٽيگ);
  plan.requested_kerning = !!plan.kern_نقاب;
#جيڪڏهن_پورو

  منطقي has_gpos_kern = plan.map.get_feature_اشاريو (1, kern_ٽيگ) != HB_OT_LAYOUT_NO_FEATURE_اشاريو;
  منطقي disable_gpos = plan.shaper->gpos_ٽيگ &&
		      plan.shaper->gpos_ٽيگ != plan.map.chosen_script[1];

  /*
   * Decide who provides glyph classes. GDEF or Unicode.
   */

  إذا (!hb_ot_layout_has_glyph_classes (face))
    plan.fallback_glyph_classes = سچ;

  /*
   * Decide who does substitutions. GSUB, morx, or fallback.
   */

#جيڪڏهن_وصف_نه_ٿيل HB_NO_AAT_SHAPE
  plan.apply_morx = apply_morx;
#جيڪڏهن_پورو

  /*
   * Decide who does positioning. GPOS, kerx, kern, or fallback.
   */

#جيڪڏهن_وصف_نه_ٿيل HB_NO_AAT_SHAPE
  منطقي has_kerx = hb_aat_layout_has_positioning (face);
  منطقي has_gsub = !apply_morx && hb_ot_layout_has_substitution (face);
#جيڪڏهن_پورو
  منطقي has_gpos = !disable_gpos && hb_ot_layout_has_positioning (face);
  إذا (خطأ)
    {}
#جيڪڏهن_وصف_نه_ٿيل HB_NO_AAT_SHAPE
  /* Prefer GPOS over kerx if GSUB is present;
   * https://github.com/harfbuzz/harfbuzz/issues/3008 */
  وإلا إذا (has_kerx && !(has_gsub && has_gpos))
    plan.apply_kerx = سچ;
#جيڪڏهن_پورو
  وإلا إذا (has_gpos)
    plan.apply_gpos = سچ;

  إذا (!plan.apply_kerx && (!has_gpos_kern || !plan.apply_gpos))
  {
    إذا (خطأ) {}
#جيڪڏهن_وصف_نه_ٿيل HB_NO_AAT_SHAPE
    وإلا إذا (has_kerx)
      plan.apply_kerx = سچ;
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_KERN
    وإلا إذا (hb_ot_layout_has_kerning (face))
      plan.apply_kern = script_fallback_position; // Not all shapers apply legacy `kern`
#جيڪڏهن_پورو
    وإلا {}
  }

  plan.apply_fallback_kern = script_fallback_position && !(plan.apply_gpos || plan.apply_kerx || plan.apply_kern);

  plan.zero_marks = script_zero_marks &&
		    !plan.apply_kerx &&
		    (!plan.apply_kern
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_KERN
		     || !hb_ot_layout_has_machine_kerning (face)
#جيڪڏهن_پورو
		    );
  plan.has_gpos_mark = !!plan.map.get_1_نقاب (HB_ٽيگ ('m','a','r','k'));

  plan.adjust_mark_positioning_when_zeroing = !plan.apply_gpos &&
					      !plan.apply_kerx &&
					      (!plan.apply_kern
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_KERN
					       || !hb_ot_layout_has_cross_kerning (face)
#جيڪڏهن_پورو
					      );

  plan.fallback_mark_positioning = plan.adjust_mark_positioning_when_zeroing &&
				   script_fallback_position;

#جيڪڏهن_وصف_نه_ٿيل HB_NO_AAT_SHAPE
  /* If we're using morx shaping, we cancel mark position adjustment because
     Apple Color Emoji assumes this will NOT be done when forming emoji sequences;
     https://github.com/harfbuzz/harfbuzz/issues/2967. */
  إذا (plan.apply_morx)
    plan.adjust_mark_positioning_when_zeroing = خطأ;

  /* According to Ned, trak is applied by default for "modern fonts", as detected by presence of STAT table. */
#جيڪڏهن_وصف_نه_ٿيل HB_NO_STYLE
  plan.apply_trak = hb_aat_layout_has_tracking (face) && face->جدول.STAT->has_معلومات ();
#وإلا
  plan.apply_trak = خطأ;
#جيڪڏهن_پورو

#جيڪڏهن_پورو
}

منطقي
hb_ot_shape_plan_t::init0 (hb_face_t                     *face,
			   ثابت hb_shape_plan_ڪنجي_t     *key)
{
  map.init ();

  hb_ot_shape_planner_t planner (face,
				 key->props);

  hb_ot_shape_collect_features (&planner,
				key->user_features,
				key->انگ_user_features);

  planner.compile (*هذا, key->ot);

  إذا (shaper->معلومات_create)
  {
    معلومات = shaper->معلومات_create (هذا);
    إذا (unlikely (!معلومات))
    {
      map.fini ();
      أرجع خطأ;
    }
  }

  أرجع سچ;
}

فراغ
hb_ot_shape_plan_t::fini ()
{
  إذا (shaper->معلومات_destroy)
    shaper->معلومات_destroy (const_cast<فراغ *> (معلومات));

  map.fini ();
}

فراغ
hb_ot_shape_plan_t::substitute (hb_font_t   *font,
				hb_بفر_t *بفر) ثابت
{
  map.substitute (هذا, font, بفر);
}

فراغ
hb_ot_shape_plan_t::position (hb_font_t   *font,
			      hb_بفر_t *بفر) ثابت
{
  إذا (هذا->apply_gpos)
    map.position (هذا, font, بفر);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_AAT_SHAPE
  وإلا إذا (هذا->apply_kerx)
    hb_aat_layout_position (هذا, font, بفر);
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_KERN
  إذا (هذا->apply_kern)
    hb_ot_layout_kern (هذا, font, بفر);
#جيڪڏهن_پورو
  وإلا إذا (هذا->apply_fallback_kern)
    _hb_ot_shape_fallback_kern (هذا, font, بفر);

#جيڪڏهن_وصف_نه_ٿيل HB_NO_AAT_SHAPE
  إذا (هذا->apply_trak)
    hb_aat_layout_track (هذا, font, بفر);
#جيڪڏهن_پورو
}


ساكن ثابت hb_ot_map_feature_t
common_features[] =
{
  {HB_ٽيگ('a','b','v','m'), F_GLOBAL},
  {HB_ٽيگ('b','l','w','m'), F_GLOBAL},
  {HB_ٽيگ('c','c','m','p'), F_GLOBAL},
  {HB_ٽيگ('l','o','c','l'), F_GLOBAL},
  {HB_ٽيگ('m','a','r','k'), F_GLOBAL_MANUAL_JOINERS},
  {HB_ٽيگ('m','k','m','k'), F_GLOBAL_MANUAL_JOINERS},
  {HB_ٽيگ('r','l','i','g'), F_GLOBAL},
};


ساكن ثابت hb_ot_map_feature_t
horizontal_features[] =
{
  {HB_ٽيگ('c','a','l','t'), F_GLOBAL},
  {HB_ٽيگ('c','l','i','g'), F_GLOBAL},
  {HB_ٽيگ('c','u','r','s'), F_GLOBAL},
  {HB_ٽيگ('d','i','s','t'), F_GLOBAL},
  {HB_ٽيگ('k','e','r','n'), F_GLOBAL_HAS_FALLBACK},
  {HB_ٽيگ('l','i','g','a'), F_GLOBAL},
  {HB_ٽيگ('r','c','l','t'), F_GLOBAL},
};

ساكن فراغ
hb_ot_shape_collect_features (hb_ot_shape_planner_t *planner,
			      ثابت hb_feature_t    *user_features,
			      بنانشان صحيح           انگ_user_features)
{
  hb_ot_map_builder_t *map = &planner->map;

  map->is_simple = سچ;

  map->enable_feature (HB_ٽيگ('r','v','r','n'));
  map->جوڙيو_gsub_pause (خالي_اشارو);

  بدل (planner->props.direction)
  {
    حالة HB_DIRECTION_LTR:
      map->enable_feature (HB_ٽيگ ('l','t','r','a'));
      map->enable_feature (HB_ٽيگ ('l','t','r','m'));
      اكسر;
    حالة HB_DIRECTION_RTL:
      map->enable_feature (HB_ٽيگ ('r','t','l','a'));
      map->جوڙيو_feature (HB_ٽيگ ('r','t','l','m'));
      اكسر;
    حالة HB_DIRECTION_TTB:
    حالة HB_DIRECTION_BTT:
    حالة HB_DIRECTION_INVALID:
    افتراضي:
      اكسر;
  }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_SHAPE_FRACTIONS
  /* Automatic fractions. */
  map->جوڙيو_feature (HB_ٽيگ ('f','r','a','c'));
  map->جوڙيو_feature (HB_ٽيگ ('n','u','m','r'));
  map->جوڙيو_feature (HB_ٽيگ ('d','n','o','m'));
#جيڪڏهن_پورو

  /* Random! */
  map->enable_feature (HB_ٽيگ ('r','a','n','d'), F_RANDOM, HB_OT_MAP_MAX_قدر);

  map->enable_feature (HB_ٽيگ ('H','a','r','f')); /* Considered required. */
  map->enable_feature (HB_ٽيگ ('H','A','R','F')); /* Considered discretionary. */

  إذا (planner->shaper->collect_features)
  {
    map->is_simple = خطأ;
    planner->shaper->collect_features (planner);
  }

  map->enable_feature (HB_ٽيگ ('B','u','z','z')); /* Considered required. */
  map->enable_feature (HB_ٽيگ ('B','U','Z','Z')); /* Considered discretionary. */

  لكل (بنانشان صحيح i = 0; i < ARRAY_ڊيگهائي (common_features); i++)
    map->جوڙيو_feature (common_features[i]);

  إذا (HB_DIRECTION_IS_HORIZONTAL (planner->props.direction))
    لكل (بنانشان صحيح i = 0; i < ARRAY_ڊيگهائي (horizontal_features); i++)
      map->جوڙيو_feature (horizontal_features[i]);
  وإلا
  {
    /* We only apply `vert` feature. See:
     * https://github.com/harfbuzz/harfbuzz/commit/d71c0df2d17f4590d5611239577a6cb532c26528
     * https://lists.freedesktop.org/archives/harfbuzz/2013-August/003490.html */

    /* We really want to find a 'vert' feature if there's any in the font, no
     * matter which script/langsys it is listed (or not) under.
     * See various bugs referenced from:
     * https://github.com/harfbuzz/harfbuzz/issues/63 */
    map->enable_feature (HB_ٽيگ ('v','e','r','t'), F_GLOBAL_ڳولا);
  }

  إذا (انگ_user_features)
    map->is_simple = خطأ;
  لكل (بنانشان صحيح i = 0; i < انگ_user_features; i++)
  {
    ثابت hb_feature_t *feature = &user_features[i];
    map->جوڙيو_feature (feature->ٽيگ_2,
		      (feature->شروعات == HB_FEATURE_GLOBAL_شروعات &&
		       feature->پڇاڙي == HB_FEATURE_GLOBAL_پڇاڙي) ?  F_GLOBAL : F_NONE,
		      feature->value);
  }

  إذا (planner->shaper->override_features)
    planner->shaper->override_features (planner);
}


/*
 * shaper face data
 */

بنية hb_ot_face_معلومات_t {};

hb_ot_face_معلومات_t *
hb_ot_shaper_face_معلومات_create (hb_face_t *face)
{
  أرجع (hb_ot_face_معلومات_t *) HB_SHAPER_معلومات_SUCCEEDED;
}

فراغ
hb_ot_shaper_face_معلومات_destroy (hb_ot_face_معلومات_t *معلومات)
{
}


/*
 * shaper font data
 */

بنية hb_ot_font_معلومات_t {
  OT::hb_scalar_cache_t اڻ_استعمال_ٿيل_2; // Just for alignment
};

hb_ot_font_معلومات_t *
hb_ot_shaper_font_معلومات_create (hb_font_t *font)
{
  ثابت OT::شي_Variation_Store &var_store = font->face->جدول.GDEF->جدول->get_var_store ();
  أرجع (hb_ot_font_معلومات_t *) var_store.create_cache ();
}

فراغ
hb_ot_shaper_font_معلومات_destroy (hb_ot_font_معلومات_t *معلومات)
{
  OT::شي_Variation_Store::destroy_cache ((OT::hb_scalar_cache_t *) معلومات);
}


/*
 * shaper
 */

بنية hb_ot_shape_context_t
{
  hb_ot_shape_plan_t *plan;
  hb_font_t *font;
  hb_face_t *face;
  hb_بفر_t  *بفر;
  ثابت hb_feature_t *user_features;
  بنانشان صحيح        انگ_user_features;

  /* Transient stuff */
  hb_direction_t هدف_direction;
};



/* Main shaper */


/* Prepare */

ساكن فراغ
hb_سيٽ_unicode_props (hb_بفر_t *بفر,
		      hb_نقاب_t    global_نقاب)
{
  /* Implement enough of Unicode Graphemes here that shaping
   * in reverse-direction wouldn't break graphemes.  Namely,
   * we mark all marks and ZWJ and ZWJ,Extended_Pictographic
   * sequences as continuations.  The foreach_grapheme()
   * macro uses this bit.
   *
   * https://www.unicode.org/reports/tr29/#Regex_Definitions
   */
  بنانشان صحيح ڳڻپ = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
  {
    info[i].نقاب = global_نقاب;
    hb_glyph_info_سيٽ_unicode_props (&info[i], بفر);

    إذا (info[i].codepoint < 0x80)
      استمر;

    بنانشان gen_cat = _hb_glyph_info_get_general_category (&info[i]);
    إذا (FLAG_UNSAFE (gen_cat) &
	(FLAG (HB_UNICODE_GENERAL_CATEGORY_LOWERCASE_LETTER) |
	 FLAG (HB_UNICODE_GENERAL_CATEGORY_UPPERCASE_LETTER) |
	 FLAG (HB_UNICODE_GENERAL_CATEGORY_TITLECASE_LETTER) |
	 FLAG (HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER) |
	 FLAG (HB_UNICODE_GENERAL_CATEGORY_SPACE_SEPARATOR)))
      استمر;

    /* Marks are already set as continuation by the above line.
     * Handle Emoji_Modifier and ZWJ-continuation. */
    إذا (unlikely (gen_cat == HB_UNICODE_GENERAL_CATEGORY_MODIFIER_نشان &&
		  hb_اندريون_range<hb_codepoint_t> (info[i].codepoint, 0x1F3FBu, 0x1F3FFu)))
    {
      hb_glyph_info_سيٽ_continuation (&info[i], بفر);
    }
    /* Regional_Indicators are hairy as hell...
     * https://github.com/harfbuzz/harfbuzz/issues/2265 */
    وإلا إذا (unlikely (i && _hb_codepoint_is_regional_indicator (info[i].codepoint)))
    {
      إذا (_hb_codepoint_is_regional_indicator (info[i - 1].codepoint) &&
	  !_hb_glyph_info_is_continuation (&info[i - 1]))
	hb_glyph_info_سيٽ_continuation (&info[i], بفر);
    }
#جيڪڏهن_وصف_نه_ٿيل HB_NO_EMOJI_SEQUENCES
    وإلا إذا (unlikely (_hb_glyph_info_is_zwj (&info[i])))
    {
      hb_glyph_info_سيٽ_continuation (&info[i], بفر);
      إذا (i + 1 < ڳڻپ &&
	  _hb_unicode_is_emoji_Extended_Pictographic (info[i + 1].codepoint))
      {
	i++;
	info[i].نقاب = global_نقاب;
	hb_glyph_info_سيٽ_unicode_props (&info[i], بفر);
	hb_glyph_info_سيٽ_continuation (&info[i], بفر);
      }
    }
#جيڪڏهن_پورو
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
    وإلا إذا (unlikely (hb_اندريون_ranges<hb_codepoint_t> (info[i].codepoint, 0xFF9Eu, 0xFF9Fu, 0xE0020u, 0xE007Fu)))
      hb_glyph_info_سيٽ_continuation (&info[i], بفر);
    وإلا إذا (unlikely (info[i].codepoint == 0x2044u /* FRACTION SLASH */))
      بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_HAS_FRACTION_SLASH;
  }
}

ساكن فراغ
hb_وجهو_dotted_circle (hb_بفر_t *بفر, hb_font_t *font)
{
  إذا (unlikely (بفر->flags & HB_بفر_FLAG_DO_NOT_وجهو_DOTTED_CIRCLE))
    أرجع;

  إذا (!(بفر->flags & HB_بفر_FLAG_BOT) ||
      بفر->context_len[0] ||
      !_hb_glyph_info_is_unicode_mark (&بفر->info[0]))
    أرجع;

  إذا (!font->has_glyph (0x25CCu))
    أرجع;

  hb_glyph_info_t dottedcircle = {0};
  dottedcircle.codepoint = 0x25CCu;
  hb_glyph_info_سيٽ_unicode_props (&dottedcircle, بفر);

  بفر->clear_output ();

  بفر->idx = 0;
  hb_glyph_info_t info = dottedcircle;
  info.cluster = بفر->cur().cluster;
  info.نقاب = بفر->cur().نقاب;
  (فراغ) بفر->output_info (info);

  بفر->متزامن ();
}

ساكن فراغ
hb_form_clusters (hb_بفر_t *بفر)
{
  إذا (!(بفر->scratch_flags & HB_بفر_SCRATCH_FLAG_HAS_CONTINUATIONS))
    أرجع;

  foreach_grapheme (بفر, شروعات, پڇاڙي)
    بفر->ملايو_grapheme_clusters (شروعات, پڇاڙي);
}

ساكن فراغ
hb_ensure_native_direction (hb_بفر_t *بفر)
{
  hb_direction_t direction = بفر->props.direction;
  hb_direction_t horiz_dir = hb_script_get_horizontal_direction (بفر->props.script);

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
  إذا (unlikely (horiz_dir == HB_DIRECTION_RTL && direction == HB_DIRECTION_LTR))
  {
    منطقي مليو_عدد = خطأ, مليو_letter = خطأ, مليو_ri = خطأ;
    ثابت خودڪار* info = بفر->info;
    ثابت خودڪار ڳڻپ = بفر->len;
    لكل (بنانشان i = 0; i < ڳڻپ; i++)
    {
      خودڪار gc = _hb_glyph_info_get_general_category (&info[i]);
      إذا (HB_UNICODE_GENERAL_CATEGORY_IS_LETTER (gc))
      {
	مليو_letter = سچ;
	اكسر;
      }
      وإلا إذا (gc == HB_UNICODE_GENERAL_CATEGORY_DECIMAL_عدد)
	مليو_عدد = سچ;
      وإلا إذا (unlikely (_hb_codepoint_is_regional_indicator (info[i].codepoint)))
	مليو_ri = سچ;
    }
    إذا ((مليو_عدد || مليو_ri) && !مليو_letter)
      horiz_dir = HB_DIRECTION_LTR;
  }

  /* TODO vertical:
   * The only BTT vertical script is Ogham, but it's not clear to me whether OpenType
   * Ogham fonts are supposed to be implemented BTT or not.  Need to research that
   * first. */
  إذا ((HB_DIRECTION_IS_HORIZONTAL (direction) &&
       direction != horiz_dir && HB_DIRECTION_IS_VALID (horiz_dir)) ||
      (HB_DIRECTION_IS_VERTICAL   (direction) &&
       direction != HB_DIRECTION_TTB))
  {
    hb_ot_layout_ابتو_graphemes (بفر);
    بفر->props.direction = HB_DIRECTION_ابتو (بفر->props.direction);
  }
}


/*
 * Substitute
 */

ساكن مضمن فراغ
hb_ot_rotate_chars (ثابت hb_ot_shape_context_t *c)
{
  hb_بفر_t *بفر = c->بفر;
  بنانشان صحيح ڳڻپ = بفر->len;
  hb_glyph_info_t *info = بفر->info;

  إذا (HB_DIRECTION_IS_BACKWARD (c->هدف_direction))
  {
    hb_unicode_funcs_t *unicode = بفر->unicode;
    hb_نقاب_t rtlm_نقاب = c->plan->rtlm_نقاب;

    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++) {
      hb_codepoint_t codepoint = unicode->mirroring (info[i].codepoint);
      إذا (unlikely (codepoint != info[i].codepoint && c->font->has_glyph (codepoint)))
	info[i].codepoint = codepoint;
      وإلا
	info[i].نقاب |= rtlm_نقاب;
    }
  }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VERTICAL
  إذا (HB_DIRECTION_IS_VERTICAL (c->هدف_direction) && !c->plan->has_vert)
  {
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++) {
      hb_codepoint_t codepoint = hb_unicode_funcs_t::vertical_char_for (info[i].codepoint);
      إذا (unlikely (codepoint != info[i].codepoint && c->font->has_glyph (codepoint)))
	info[i].codepoint = codepoint;
    }
  }
#جيڪڏهن_پورو
}

ساكن مضمن فراغ
hb_ot_shape_تياري_masks_fraction (ثابت hb_ot_shape_context_t *c)
{
#جيڪڏهن_وصف_ٿيل HB_NO_OT_SHAPE_FRACTIONS
  أرجع;
#جيڪڏهن_پورو

  إذا (!(c->بفر->scratch_flags & HB_بفر_SCRATCH_FLAG_HAS_FRACTION_SLASH) ||
      !c->plan->has_frac)
    أرجع;

  hb_بفر_t *بفر = c->بفر;

  hb_نقاب_t pre_نقاب, post_نقاب;
  إذا (HB_DIRECTION_IS_FORWARD (بفر->props.direction))
  {
    pre_نقاب = c->plan->numr_نقاب | c->plan->frac_نقاب;
    post_نقاب = c->plan->frac_نقاب | c->plan->dnom_نقاب;
  }
  وإلا
  {
    pre_نقاب = c->plan->frac_نقاب | c->plan->dnom_نقاب;
    post_نقاب = c->plan->numr_نقاب | c->plan->frac_نقاب;
  }

  بنانشان صحيح ڳڻپ = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
  {
    إذا (info[i].codepoint == 0x2044u) /* FRACTION SLASH */
    {
      بنانشان صحيح شروعات = i, پڇاڙي = i + 1;
      طالما (شروعات &&
	     _hb_glyph_info_get_general_category (&info[شروعات - 1]) ==
	     HB_UNICODE_GENERAL_CATEGORY_DECIMAL_عدد)
	شروعات--;
      طالما (پڇاڙي < ڳڻپ &&
	     _hb_glyph_info_get_general_category (&info[پڇاڙي]) ==
	     HB_UNICODE_GENERAL_CATEGORY_DECIMAL_عدد)
	پڇاڙي++;
      إذا (شروعات == i || پڇاڙي == i + 1)
      {
        إذا (شروعات == i)
	  بفر->unsafe_منزل_concat (شروعات, شروعات + 1);
	إذا (پڇاڙي == i + 1)
	  بفر->unsafe_منزل_concat (پڇاڙي - 1, پڇاڙي);
	استمر;
      }

      بفر->unsafe_منزل_break (شروعات, پڇاڙي);

      لكل (بنانشان صحيح j = شروعات; j < i; j++)
	info[j].نقاب |= pre_نقاب;
      info[i].نقاب |= c->plan->frac_نقاب;
      لكل (بنانشان صحيح j = i + 1; j < پڇاڙي; j++)
	info[j].نقاب |= post_نقاب;

      i = پڇاڙي - 1;
    }
  }
}

ساكن مضمن فراغ
hb_ot_shape_تياري_masks (ثابت hb_ot_shape_context_t *c)
{
  hb_ot_map_t *map = &c->plan->map;
  hb_بفر_t *بفر = c->بفر;

  hb_ot_shape_تياري_masks_fraction (c);

  إذا (c->plan->shaper->تياري_masks)
    c->plan->shaper->تياري_masks (c->plan, بفر, c->font);

  لكل (بنانشان صحيح i = 0; i < c->انگ_user_features; i++)
  {
    ثابت hb_feature_t *feature = &c->user_features[i];
    إذا (!(feature->شروعات == HB_FEATURE_GLOBAL_شروعات && feature->پڇاڙي == HB_FEATURE_GLOBAL_پڇاڙي)) {
      بنانشان صحيح سرڪايو;
      hb_نقاب_t نقاب = map->get_نقاب (feature->ٽيگ_2, &سرڪايو);
      بفر->سيٽ_masks (feature->value << سرڪايو, نقاب, feature->شروعات, feature->پڇاڙي);
    }
  }
}

ساكن فراغ
hb_ot_zero_ويڪر_default_ignorables (ثابت hb_بفر_t *بفر)
{
  إذا (!(بفر->scratch_flags & HB_بفر_SCRATCH_FLAG_HAS_DEFAULT_IGNORABLES) ||
      (بفر->flags & HB_بفر_FLAG_PRESERVE_DEFAULT_IGNORABLES) ||
      (بفر->flags & HB_بفر_FLAG_REMOVE_DEFAULT_IGNORABLES))
    أرجع;

  بنانشان صحيح ڳڻپ = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  hb_glyph_position_t *pos = بفر->pos;
  بنانشان صحيح i = 0;
  لكل (i = 0; i < ڳڻپ; i++)
    إذا (unlikely (_hb_glyph_info_is_default_ignorable (&info[i])))
    {
      pos[i].x_advance = pos[i].y_advance = 0;
      إذا (HB_DIRECTION_IS_HORIZONTAL (بفر->props.direction))
	pos[i].x_وٿي_2 = 0;
      وإلا
        pos[i].y_وٿي_2 = 0;
    }
}

ساكن فراغ
hb_ot_deal_with_variation_selectors (hb_بفر_t *بفر)
{
  إذا (!(بفر->scratch_flags & HB_بفر_SCRATCH_FLAG_HAS_VARIATION_SELECTOR_FALLBACK) ||
	بفر->not_مليو_variation_selector == HB_CODEPOINT_INVALID)
    أرجع;

  بنانشان صحيح ڳڻپ = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  hb_glyph_position_t *pos = بفر->pos;

  لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
  {
    إذا (_hb_glyph_info_is_variation_selector (&info[i]))
    {
      info[i].codepoint = بفر->not_مليو_variation_selector;
      pos[i].x_advance = pos[i].y_advance = pos[i].x_وٿي_2 = pos[i].y_وٿي_2 = 0;
      hb_glyph_info_سيٽ_variation_selector (&info[i], خطأ);
    }
  }
}

ساكن فراغ
hb_ot_hide_default_ignorables (hb_بفر_t *بفر,
			       hb_font_t   *font)
{
  إذا (!(بفر->scratch_flags & HB_بفر_SCRATCH_FLAG_HAS_DEFAULT_IGNORABLES) ||
      (بفر->flags & HB_بفر_FLAG_PRESERVE_DEFAULT_IGNORABLES))
    أرجع;

  بنانشان صحيح ڳڻپ = بفر->len;
  hb_glyph_info_t *info = بفر->info;

  hb_codepoint_t invisible = بفر->invisible;
  إذا (!(بفر->flags & HB_بفر_FLAG_REMOVE_DEFAULT_IGNORABLES) &&
      (invisible || font->get_nominal_glyph (' ', &invisible)))
  {
    /* Replace default-ignorables with a zero-advance invisible glyph. */
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    {
      إذا (_hb_glyph_info_is_default_ignorable (&info[i]))
	info[i].codepoint = invisible;
    }
  }
  وإلا
    بفر->delete_glyphs_inplace (_hb_glyph_info_is_default_ignorable);
}


ساكن مضمن فراغ
hb_ot_map_glyphs_fast (hb_بفر_t  *بفر)
{
  /* Normalization process sets up normalizer_glyph_index(), we just copy it. */
  بنانشان صحيح ڳڻپ = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    info[i].codepoint = info[i].normalizer_glyph_اشاريو();

  بفر->content_type = HB_بفر_CONTENT_TYPE_GLYPHS;
}

ساكن مضمن فراغ
hb_synthesize_glyph_classes (hb_بفر_t *بفر)
{
  بنانشان صحيح ڳڻپ = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
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
	    HB_OT_LAYOUT_GLYPH_PROPS_بنياد_GLYPH :
	    HB_OT_LAYOUT_GLYPH_PROPS_MARK;
    hb_glyph_info_سيٽ_glyph_props (&info[i], klass);
  }
}

ساكن مضمن فراغ
hb_ot_substitute_default (ثابت hb_ot_shape_context_t *c)
{
  hb_بفر_t *بفر = c->بفر;

  hb_ot_rotate_chars (c);

  HB_بفر_ALLOCATE_VAR (بفر, normalizer_glyph_اشاريو);

  _hb_ot_shape_normalize (c->plan, بفر, c->font);

  hb_ot_shape_تياري_masks (c);

  /* This is unfortunate to go here, but necessary... */
  إذا (c->plan->fallback_mark_positioning)
    _hb_ot_shape_fallback_mark_position_recategorize_marks (c->plan, c->font, بفر);

  hb_ot_map_glyphs_fast (بفر);

  HB_بفر_DEALLOCATE_VAR (بفر, normalizer_glyph_اشاريو);
}

ساكن مضمن فراغ
hb_ot_substitute_plan (ثابت hb_ot_shape_context_t *c)
{
  hb_بفر_t *بفر = c->بفر;

#جيڪڏهن_وصف_نه_ٿيل HB_NO_AAT_SHAPE
  إذا (unlikely (c->plan->apply_morx))
  {
    hb_ot_layout_substitute_شروعات (c->font, بفر);

    إذا (c->plan->fallback_glyph_classes)
      hb_synthesize_glyph_classes (بفر);

    hb_aat_layout_substitute (c->plan, c->font, c->بفر,
			      c->user_features, c->انگ_user_features);
    /* The buffer digest is only used by the OT lookup-apply loop;
     * without GPOS ahead, nothing consumes it. */
    إذا (c->plan->apply_gpos)
      c->بفر->update_digest ();
  }
  وإلا
#جيڪڏهن_پورو
  {
    hb_ot_layout_substitute_شروعات_with_digest (c->font, بفر);

    إذا (c->plan->fallback_glyph_classes)
      hb_synthesize_glyph_classes (بفر);

    c->plan->substitute (c->font, بفر);
  }
}

ساكن مضمن فراغ
hb_ot_substitute_pre (ثابت hb_ot_shape_context_t *c)
{
  hb_ot_substitute_default (c);

  hb_بفر_allocate_gsubgpos_vars (c->بفر);

  hb_ot_substitute_plan (c);

#جيڪڏهن_وصف_نه_ٿيل HB_NO_AAT_SHAPE
  إذا (c->plan->apply_morx && c->plan->apply_gpos)
    hb_aat_layout_remove_deleted_glyphs (c->بفر);
#جيڪڏهن_پورو
}

ساكن مضمن فراغ
hb_ot_substitute_post (ثابت hb_ot_shape_context_t *c)
{
#جيڪڏهن_وصف_نه_ٿيل HB_NO_AAT_SHAPE
  إذا (c->plan->apply_morx && !c->plan->apply_gpos)
    hb_aat_layout_remove_deleted_glyphs (c->بفر);
#جيڪڏهن_پورو

  hb_ot_deal_with_variation_selectors (c->بفر);
  hb_ot_hide_default_ignorables (c->بفر, c->font);

  إذا (c->plan->shaper->postprocess_glyphs &&
    c->بفر->پيغام(c->font, "start postprocess-glyphs")) {
    c->plan->shaper->postprocess_glyphs (c->plan, c->بفر, c->font);
    (فراغ) c->بفر->پيغام(c->font, "end postprocess-glyphs");
  }
}


/*
 * Position
 */

ساكن مضمن فراغ
adjust_mark_offsets (hb_glyph_position_t *pos)
{
  pos->x_وٿي_2 = hb_saturate_sub (pos->x_وٿي_2, pos->x_advance);
  pos->y_وٿي_2 = hb_saturate_sub (pos->y_وٿي_2, pos->y_advance);
}

ساكن مضمن فراغ
zero_mark_ويڪر (hb_glyph_position_t *pos)
{
  pos->x_advance = 0;
  pos->y_advance = 0;
}

ساكن مضمن فراغ
zero_mark_widths_by_gdef (hb_بفر_t *بفر, منطقي adjust_offsets)
{
  بنانشان صحيح ڳڻپ = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    إذا (_hb_glyph_info_is_mark (&info[i]))
    {
      إذا (adjust_offsets)
	adjust_mark_offsets (&بفر->pos[i]);
      zero_mark_ويڪر (&بفر->pos[i]);
    }
}

ساكن مضمن فراغ
hb_ot_position_default (ثابت hb_ot_shape_context_t *c)
{
  hb_direction_t direction = c->بفر->props.direction;
  بنانشان صحيح ڳڻپ = c->بفر->len;
  hb_glyph_info_t *info = c->بفر->info;
  hb_glyph_position_t *pos = c->بفر->pos;

  إذا (HB_DIRECTION_IS_HORIZONTAL (direction))
  {
    c->font->get_glyph_h_advances (ڳڻپ, &info[0].codepoint, حجم(info[0]),
				   &pos[0].x_advance, حجم(pos[0]));
    // h_origin defaults to zero; only apply it if the font has it.
    إذا (c->font->has_glyph_h_origin_func () || c->font->has_glyph_h_origins_func ())
      c->font->ڪٽيو_glyph_h_origins (c->بفر);
  }
  وإلا
  {
    c->font->get_glyph_v_advances (ڳڻپ, &info[0].codepoint, حجم(info[0]),
				   &pos[0].y_advance, حجم(pos[0]));
    // v_origin defaults to non-zero; apply even if only fallback is there.
    c->font->ڪٽيو_glyph_v_origins (c->بفر);
  }
  إذا (c->بفر->scratch_flags & HB_بفر_SCRATCH_FLAG_HAS_SPACE_FALLBACK)
    _hb_ot_shape_fallback_spaces (c->plan, c->font, c->بفر);
}

ساكن مضمن فراغ
hb_ot_position_plan (ثابت hb_ot_shape_context_t *c)
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
  منطقي adjust_offsets_when_zeroing = c->plan->adjust_mark_positioning_when_zeroing &&
				     HB_DIRECTION_IS_FORWARD (c->بفر->props.direction);

  /* We change glyph origin to what GPOS expects (horizontal), apply GPOS, change it back. */

  // h_origin defaults to zero; only apply it if the font has it.
  إذا (c->font->has_glyph_h_origin_func () || c->font->has_glyph_h_origins_func ())
    c->font->جوڙيو_glyph_h_origins (c->بفر);

  hb_ot_layout_position_شروعات (c->font, c->بفر);

  إذا (c->plan->zero_marks)
    بدل (c->plan->shaper->zero_ويڪر_marks)
    {
      حالة HB_OT_SHAPE_ZERO_ويڪر_MARKS_BY_GDEF_EARLY:
	zero_mark_widths_by_gdef (c->بفر, adjust_offsets_when_zeroing);
	اكسر;

      افتراضي:
      حالة HB_OT_SHAPE_ZERO_ويڪر_MARKS_NONE:
      حالة HB_OT_SHAPE_ZERO_ويڪر_MARKS_BY_GDEF_LATE:
	اكسر;
    }

  c->plan->position (c->font, c->بفر);

  إذا (c->plan->zero_marks)
    بدل (c->plan->shaper->zero_ويڪر_marks)
    {
      حالة HB_OT_SHAPE_ZERO_ويڪر_MARKS_BY_GDEF_LATE:
	zero_mark_widths_by_gdef (c->بفر, adjust_offsets_when_zeroing);
	اكسر;

      افتراضي:
      حالة HB_OT_SHAPE_ZERO_ويڪر_MARKS_NONE:
      حالة HB_OT_SHAPE_ZERO_ويڪر_MARKS_BY_GDEF_EARLY:
	اكسر;
    }

  /* Finish off.  Has to follow a certain order. */
  hb_ot_layout_position_پڄاڻي_advances (c->font, c->بفر);
  hb_ot_zero_ويڪر_default_ignorables (c->بفر);
  hb_ot_layout_position_پڄاڻي_offsets (c->font, c->بفر);

  // h_origin defaults to zero; only apply it if the font has it.
  إذا (c->font->has_glyph_h_origin_func () || c->font->has_glyph_h_origins_func ())
    c->font->ڪٽيو_glyph_h_origins (c->بفر);

  إذا (c->plan->fallback_mark_positioning)
    _hb_ot_shape_fallback_mark_position (c->plan, c->font, c->بفر,
					 adjust_offsets_when_zeroing);
}

ساكن مضمن فراغ
hb_ot_position (ثابت hb_ot_shape_context_t *c)
{
  c->بفر->clear_positions ();

  hb_ot_position_default (c);

  hb_ot_position_plan (c);

  إذا (HB_DIRECTION_IS_BACKWARD (c->بفر->props.direction))
    hb_بفر_ابتو (c->بفر);

  hb_بفر_deallocate_gsubgpos_vars (c->بفر);
}

ساكن مضمن فراغ
hb_propagate_flags (hb_بفر_t *بفر)
{
  /* Propagate cluster-level glyph flags to be the same on all cluster glyphs.
   * Simplifies using them. */

  hb_نقاب_t and_نقاب = HB_GLYPH_FLAG_DEFINED;
  إذا ((بفر->flags & HB_بفر_FLAG_PRODUCE_UNSAFE_منزل_CONCAT) == 0)
    and_نقاب &= ~HB_GLYPH_FLAG_UNSAFE_منزل_CONCAT;

  hb_glyph_info_t *info = بفر->info;

  إذا ((بفر->flags & HB_بفر_FLAG_PRODUCE_محفوظ_منزل_وجهو_TATWEEL) == 0)
  {
    foreach_cluster (بفر, شروعات, پڇاڙي)
    {
      إذا (پڇاڙي - شروعات == 1)
      {
        info[شروعات].نقاب &= and_نقاب;
	استمر;
      }

      بنانشان صحيح نقاب = 0;
      لكل (بنانشان صحيح i = شروعات; i < پڇاڙي; i++)
	نقاب |= info[i].نقاب;

      نقاب &= and_نقاب;

      لكل (بنانشان صحيح i = شروعات; i < پڇاڙي; i++)
	info[i].نقاب = نقاب;
    }
    أرجع;
  }

  /* If we are producing SAFE_TO_INSERT_TATWEEL, then do two things:
   *
   * - If the places that the Arabic shaper marked as SAFE_TO_INSERT_TATWEEL,
   *   are UNSAFE_TO_BREAK, then clear the SAFE_TO_INSERT_TATWEEL,
   * - Any place that is SAFE_TO_INSERT_TATWEEL, is also now UNSAFE_TO_BREAK.
   *
   * We couldn't make this interaction earlier. It has to be done this way.
   */
  foreach_cluster (بفر, شروعات, پڇاڙي)
  {
    بنانشان صحيح نقاب = 0;
    لكل (بنانشان صحيح i = شروعات; i < پڇاڙي; i++)
      نقاب |= info[i].نقاب;

    إذا (نقاب & HB_GLYPH_FLAG_UNSAFE_منزل_BREAK)
      نقاب &= ~HB_GLYPH_FLAG_محفوظ_منزل_وجهو_TATWEEL;
    إذا (نقاب & HB_GLYPH_FLAG_محفوظ_منزل_وجهو_TATWEEL)
      نقاب |= HB_GLYPH_FLAG_UNSAFE_منزل_BREAK | HB_GLYPH_FLAG_UNSAFE_منزل_CONCAT;

    نقاب &= and_نقاب;

    لكل (بنانشان صحيح i = شروعات; i < پڇاڙي; i++)
      info[i].نقاب = نقاب;
  }
}

/* Pull it all together! */

ساكن فراغ
hb_ot_shape_اندروني (hb_ot_shape_context_t *c)
{
  /* Save the original direction, we use it later. */
  c->هدف_direction = c->بفر->props.direction;

  hb_بفر_allocate_unicode_vars (c->بفر);

  hb_سيٽ_unicode_props (c->بفر, c->plan->map.get_global_نقاب ());
  hb_وجهو_dotted_circle (c->بفر, c->font);

  hb_form_clusters (c->بفر);

  hb_ensure_native_direction (c->بفر);

  إذا (c->plan->shaper->preprocess_متن &&
      c->بفر->پيغام(c->font, "start preprocess-text"))
  {
    c->plan->shaper->preprocess_متن (c->plan, c->بفر, c->font);
    (فراغ) c->بفر->پيغام(c->font, "end preprocess-text");
  }

  hb_ot_substitute_pre (c);
  hb_ot_position (c);
  hb_ot_substitute_post (c);

  hb_propagate_flags (c->بفر);

  hb_بفر_deallocate_unicode_vars (c->بفر);

  c->بفر->props.direction = c->هدف_direction;
}


hb_bool_t
_hb_ot_shape (hb_shape_plan_t    *shape_plan,
	      hb_font_t          *font,
	      hb_بفر_t        *بفر,
	      ثابت hb_feature_t *features,
	      بنانشان صحيح        انگ_features)
{
  hb_ot_shape_context_t c = {&shape_plan->ot, font, font->face, بفر, features, انگ_features};
  hb_ot_shape_اندروني (&c);

  أرجع سچ;
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
فراغ
hb_ot_shape_plan_collect_lookups (hb_shape_plan_t *shape_plan,
				  hb_ٽيگ_t         جدول_ٽيگ_2,
				  hb_سيٽ_t        *lookup_indexes /*ٻاهريون*/)
{
  shape_plan->ot.collect_lookups (جدول_ٽيگ_2, lookup_indexes);
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
بنانشان صحيح
hb_ot_shape_plan_get_feature_ٽيگ_فهرست (hb_shape_plan_t *shape_plan,
				   بنانشان صحيح     شروعات_وٿي,
				   بنانشان صحيح    *ٽيگ_ڳڻپ, /* IN/OUT */
				   hb_ٽيگ_t        *ٽيگ_فهرست /*ٻاهريون*/)
{
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_SHAPE
  أرجع shape_plan->ot.map.get_feature_ٽيگ_فهرست (شروعات_وٿي, ٽيگ_ڳڻپ, ٽيگ_فهرست);
#وإلا
  إذا (ٽيگ_ڳڻپ)
	*ٽيگ_ڳڻپ = 0;
  أرجع 0;
#جيڪڏهن_پورو
}


/* TODO Move this to hb-ot-shape-normalize, make it do decompose, and make it public. */
ساكن فراغ
جوڙيو_char (hb_font_t          *font,
	  hb_unicode_funcs_t *unicode,
	  hb_bool_t           mirror,
	  hb_codepoint_t      u,
	  hb_سيٽ_t           *glyphs)
{
  hb_codepoint_t glyph;
  إذا (font->get_nominal_glyph (u, &glyph))
    glyphs->جوڙيو (glyph);
  إذا (mirror)
  {
    hb_codepoint_t m = unicode->mirroring (u);
    إذا (m != u && font->get_nominal_glyph (m, &glyph))
      glyphs->جوڙيو (glyph);
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
فراغ
hb_ot_shape_glyphs_گهيرو (hb_font_t          *font,
			    hb_بفر_t        *بفر,
			    ثابت hb_feature_t *features,
			    بنانشان صحيح        انگ_features,
			    hb_سيٽ_t           *glyphs)
{
  ثابت حرف *shapers[] = {"ot", خالي_اشارو};
  hb_shape_plan_t *shape_plan = hb_shape_plan_create_cached (font->face, &بفر->props,
							     features, انگ_features, shapers);

  منطقي mirror = hb_script_get_horizontal_direction (بفر->props.script) == HB_DIRECTION_RTL;

  بنانشان صحيح ڳڻپ = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    جوڙيو_char (font, بفر->unicode, mirror, info[i].codepoint, glyphs);

  hb_سيٽ_t *lookups = hb_سيٽ_create ();
  hb_ot_shape_plan_collect_lookups (shape_plan, HB_OT_ٽيگ_GSUB, lookups);
  hb_ot_layout_lookups_substitute_گهيرو (font->face, lookups, glyphs);

  hb_سيٽ_destroy (lookups);

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
بنانشان صحيح
hb_ot_shape_get_بفر_format_serial (فراغ)
{
  أرجع HB_OT_SHAPE_بفر_FORMAT_SERIAL;
}

#جيڪڏهن_پورو

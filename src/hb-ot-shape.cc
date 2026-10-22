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

#شامل "hb.hh"

#اگر_غیر_تعریف_شدہ HB_NO_OT_SHAPE

#اگر_تعریف_شدہ HB_NO_OT_LAYOUT
#غلطی "Cannot compile 'ot' shaper with HB_NO_OT_LAYOUT."
#اگر_ختم

#شامل "hb-shaper-impl.hh"

#شامل "hb-ot-shape.hh"
#شامل "hb-ot-shaper.hh"
#شامل "hb-ot-shape-fallback.hh"
#شامل "hb-ot-shape-normalize.hh"

#شامل "hb-ot-face.hh"

#شامل "hb-set.hh"
#شامل "hb-unicode.hh"

#شامل "hb-aat-layout.hh"
#شامل "hb-ot-layout-gdef-table.hh"
#شامل "hb-ot-stat-table.hh"


ساکن اندرونی منطقی
_hb_codepoint_is_regional_indicator (hb_codepoint_t u)
{ واپس hb_اندر_range<hb_codepoint_t> (u, 0x1F1E6u, 0x1F1FFu); }

#اگر_غیر_تعریف_شدہ HB_NO_AAT_SHAPE
ساکن اندرونی منطقی
_hb_apply_morx (hb_face_t *face, ثابت hb_segment_properties_t &props)
{
  /* https://github.com/harfbuzz/harfbuzz/issues/2124 */
  واپس hb_aat_layout_has_substitution (face) &&
	 (HB_DIRECTION_IS_HORIZONTAL (props.direction) || !hb_ot_layout_has_substitution (face));
}
#اگر_ختم

/**
 * SECTION:hb-ot-shape
 * @title: hb-ot-shape
 * @short_description: OpenType shaping support
 * @include: hb-ot.h
 *
 * Support functions for OpenType shaping related queries.
 **/


ساکن خالی
hb_ot_shape_collect_features (hb_ot_shape_planner_t          *planner,
			      ثابت hb_feature_t             *user_features,
			      غیرعلامتی صحیح                    عدد_user_features);

hb_ot_shape_planner_t::hb_ot_shape_planner_t (hb_face_t                     *face,
					      ثابت hb_segment_properties_t &props) :
						face (face),
						props (props),
						map (face, props)
#اگر_غیر_تعریف_شدہ HB_NO_AAT_SHAPE
						, aat_map (face, props)
						, apply_morx (_hb_apply_morx (face, props))
#اگر_ختم
{
  shaper = hb_ot_shaper_categorize (props.script, props.direction, map.chosen_script[0]);

  script_zero_marks = shaper->zero_چوڑائی_marks != HB_OT_SHAPE_ZERO_چوڑائی_MARKS_NONE;
  script_fallback_position = shaper->fallback_position;

#اگر_غیر_تعریف_شدہ HB_NO_AAT_SHAPE
  /* https://github.com/harfbuzz/harfbuzz/issues/1528 */
  اگر (apply_morx && shaper != &_hb_ot_shaper_default)
    shaper = &_hb_ot_shaper_dumber;
#اگر_ختم
}

خالی
hb_ot_shape_planner_t::compile (hb_ot_shape_plan_t           &plan,
				ثابت hb_ot_shape_plan_کلید_t &key)
{
  plan.props = props;
  plan.shaper = shaper;
  map.compile (plan.map, key);
#اگر_غیر_تعریف_شدہ HB_NO_AAT_SHAPE
  اگر (apply_morx)
    aat_map.compile (plan.aat_map);
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_NO_OT_SHAPE_FRACTIONS
  plan.frac_نقاب = plan.map.get_1_نقاب (HB_لیبل ('f','r','a','c'));
  plan.numr_نقاب = plan.map.get_1_نقاب (HB_لیبل ('n','u','m','r'));
  plan.dnom_نقاب = plan.map.get_1_نقاب (HB_لیبل ('d','n','o','m'));
  plan.has_frac = plan.frac_نقاب || (plan.numr_نقاب && plan.dnom_نقاب);
#اگر_ختم

  plan.rtlm_نقاب = plan.map.get_1_نقاب (HB_لیبل ('r','t','l','m'));
  plan.has_vert = !!plan.map.get_1_نقاب (HB_لیبل ('v','e','r','t'));

  hb_لیبل_t kern_لیبل = HB_DIRECTION_IS_HORIZONTAL (props.direction) ?
		      HB_لیبل ('k','e','r','n') : HB_لیبل ('v','k','r','n');
#اگر_غیر_تعریف_شدہ HB_NO_OT_KERN
  plan.kern_نقاب = plan.map.get_نقاب (kern_لیبل);
  plan.requested_kerning = !!plan.kern_نقاب;
#اگر_ختم

  منطقی has_gpos_kern = plan.map.get_feature_اشاریہ (1, kern_لیبل) != HB_OT_LAYOUT_NO_FEATURE_اشاریہ;
  منطقی disable_gpos = plan.shaper->gpos_لیبل &&
		      plan.shaper->gpos_لیبل != plan.map.chosen_script[1];

  /*
   * Decide who provides glyph classes. GDEF or Unicode.
   */

  اگر (!hb_ot_layout_has_glyph_classes (face))
    plan.fallback_glyph_classes = سچ;

  /*
   * Decide who does substitutions. GSUB, morx, or fallback.
   */

#اگر_غیر_تعریف_شدہ HB_NO_AAT_SHAPE
  plan.apply_morx = apply_morx;
#اگر_ختم

  /*
   * Decide who does positioning. GPOS, kerx, kern, or fallback.
   */

#اگر_غیر_تعریف_شدہ HB_NO_AAT_SHAPE
  منطقی has_kerx = hb_aat_layout_has_positioning (face);
  منطقی has_gsub = !apply_morx && hb_ot_layout_has_substitution (face);
#اگر_ختم
  منطقی has_gpos = !disable_gpos && hb_ot_layout_has_positioning (face);
  اگر (جھوٹ)
    {}
#اگر_غیر_تعریف_شدہ HB_NO_AAT_SHAPE
  /* Prefer GPOS over kerx if GSUB is present;
   * https://github.com/harfbuzz/harfbuzz/issues/3008 */
  ورنہ اگر (has_kerx && !(has_gsub && has_gpos))
    plan.apply_kerx = سچ;
#اگر_ختم
  ورنہ اگر (has_gpos)
    plan.apply_gpos = سچ;

  اگر (!plan.apply_kerx && (!has_gpos_kern || !plan.apply_gpos))
  {
    اگر (جھوٹ) {}
#اگر_غیر_تعریف_شدہ HB_NO_AAT_SHAPE
    ورنہ اگر (has_kerx)
      plan.apply_kerx = سچ;
#اگر_ختم
#اگر_غیر_تعریف_شدہ HB_NO_OT_KERN
    ورنہ اگر (hb_ot_layout_has_kerning (face))
      plan.apply_kern = script_fallback_position; // Not all shapers apply legacy `kern`
#اگر_ختم
    ورنہ {}
  }

  plan.apply_fallback_kern = script_fallback_position && !(plan.apply_gpos || plan.apply_kerx || plan.apply_kern);

  plan.zero_marks = script_zero_marks &&
		    !plan.apply_kerx &&
		    (!plan.apply_kern
#اگر_غیر_تعریف_شدہ HB_NO_OT_KERN
		     || !hb_ot_layout_has_machine_kerning (face)
#اگر_ختم
		    );
  plan.has_gpos_mark = !!plan.map.get_1_نقاب (HB_لیبل ('m','a','r','k'));

  plan.adjust_mark_positioning_when_zeroing = !plan.apply_gpos &&
					      !plan.apply_kerx &&
					      (!plan.apply_kern
#اگر_غیر_تعریف_شدہ HB_NO_OT_KERN
					       || !hb_ot_layout_has_cross_kerning (face)
#اگر_ختم
					      );

  plan.fallback_mark_positioning = plan.adjust_mark_positioning_when_zeroing &&
				   script_fallback_position;

#اگر_غیر_تعریف_شدہ HB_NO_AAT_SHAPE
  /* If we're using morx shaping, we cancel mark position adjustment because
     Apple Color Emoji assumes this will NOT be done when forming emoji sequences;
     https://github.com/harfbuzz/harfbuzz/issues/2967. */
  اگر (plan.apply_morx)
    plan.adjust_mark_positioning_when_zeroing = جھوٹ;

  /* According to Ned, trak is applied by default for "modern fonts", as detected by presence of STAT table. */
#اگر_غیر_تعریف_شدہ HB_NO_STYLE
  plan.apply_trak = hb_aat_layout_has_tracking (face) && face->جدول.STAT->has_ڈیٹا ();
#ورنہ
  plan.apply_trak = جھوٹ;
#اگر_ختم

#اگر_ختم
}

منطقی
hb_ot_shape_plan_t::init0 (hb_face_t                     *face,
			   ثابت hb_shape_plan_کلید_t     *key)
{
  map.init ();

  hb_ot_shape_planner_t planner (face,
				 key->props);

  hb_ot_shape_collect_features (&planner,
				key->user_features,
				key->عدد_user_features);

  planner.compile (*یہ, key->ot);

  اگر (shaper->ڈیٹا_create)
  {
    ڈیٹا = shaper->ڈیٹا_create (یہ);
    اگر (unlikely (!ڈیٹا))
    {
      map.fini ();
      واپس جھوٹ;
    }
  }

  واپس سچ;
}

خالی
hb_ot_shape_plan_t::fini ()
{
  اگر (shaper->ڈیٹا_destroy)
    shaper->ڈیٹا_destroy (const_cast<خالی *> (ڈیٹا));

  map.fini ();
}

خالی
hb_ot_shape_plan_t::substitute (hb_font_t   *font,
				hb_بفر_t *بفر) ثابت
{
  map.substitute (یہ, font, بفر);
}

خالی
hb_ot_shape_plan_t::position (hb_font_t   *font,
			      hb_بفر_t *بفر) ثابت
{
  اگر (یہ->apply_gpos)
    map.position (یہ, font, بفر);
#اگر_غیر_تعریف_شدہ HB_NO_AAT_SHAPE
  ورنہ اگر (یہ->apply_kerx)
    hb_aat_layout_position (یہ, font, بفر);
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_NO_OT_KERN
  اگر (یہ->apply_kern)
    hb_ot_layout_kern (یہ, font, بفر);
#اگر_ختم
  ورنہ اگر (یہ->apply_fallback_kern)
    _hb_ot_shape_fallback_kern (یہ, font, بفر);

#اگر_غیر_تعریف_شدہ HB_NO_AAT_SHAPE
  اگر (یہ->apply_trak)
    hb_aat_layout_track (یہ, font, بفر);
#اگر_ختم
}


ساکن ثابت hb_ot_map_feature_t
common_features[] =
{
  {HB_لیبل('a','b','v','m'), F_GLOBAL},
  {HB_لیبل('b','l','w','m'), F_GLOBAL},
  {HB_لیبل('c','c','m','p'), F_GLOBAL},
  {HB_لیبل('l','o','c','l'), F_GLOBAL},
  {HB_لیبل('m','a','r','k'), F_GLOBAL_MANUAL_JOINERS},
  {HB_لیبل('m','k','m','k'), F_GLOBAL_MANUAL_JOINERS},
  {HB_لیبل('r','l','i','g'), F_GLOBAL},
};


ساکن ثابت hb_ot_map_feature_t
horizontal_features[] =
{
  {HB_لیبل('c','a','l','t'), F_GLOBAL},
  {HB_لیبل('c','l','i','g'), F_GLOBAL},
  {HB_لیبل('c','u','r','s'), F_GLOBAL},
  {HB_لیبل('d','i','s','t'), F_GLOBAL},
  {HB_لیبل('k','e','r','n'), F_GLOBAL_HAS_FALLBACK},
  {HB_لیبل('l','i','g','a'), F_GLOBAL},
  {HB_لیبل('r','c','l','t'), F_GLOBAL},
};

ساکن خالی
hb_ot_shape_collect_features (hb_ot_shape_planner_t *planner,
			      ثابت hb_feature_t    *user_features,
			      غیرعلامتی صحیح           عدد_user_features)
{
  hb_ot_map_builder_t *map = &planner->map;

  map->is_simple = سچ;

  map->enable_feature (HB_لیبل('r','v','r','n'));
  map->جوڑو_gsub_pause (خالی_اشاریہ);

  انتخاب (planner->props.direction)
  {
    حالت HB_DIRECTION_LTR:
      map->enable_feature (HB_لیبل ('l','t','r','a'));
      map->enable_feature (HB_لیبل ('l','t','r','m'));
      توڑو;
    حالت HB_DIRECTION_RTL:
      map->enable_feature (HB_لیبل ('r','t','l','a'));
      map->جوڑو_feature (HB_لیبل ('r','t','l','m'));
      توڑو;
    حالت HB_DIRECTION_TTB:
    حالت HB_DIRECTION_BTT:
    حالت HB_DIRECTION_INVALID:
    طےشدہ:
      توڑو;
  }

#اگر_غیر_تعریف_شدہ HB_NO_OT_SHAPE_FRACTIONS
  /* Automatic fractions. */
  map->جوڑو_feature (HB_لیبل ('f','r','a','c'));
  map->جوڑو_feature (HB_لیبل ('n','u','m','r'));
  map->جوڑو_feature (HB_لیبل ('d','n','o','m'));
#اگر_ختم

  /* Random! */
  map->enable_feature (HB_لیبل ('r','a','n','d'), F_RANDOM, HB_OT_MAP_MAX_قدر);

  map->enable_feature (HB_لیبل ('H','a','r','f')); /* Considered required. */
  map->enable_feature (HB_لیبل ('H','A','R','F')); /* Considered discretionary. */

  اگر (planner->shaper->collect_features)
  {
    map->is_simple = جھوٹ;
    planner->shaper->collect_features (planner);
  }

  map->enable_feature (HB_لیبل ('B','u','z','z')); /* Considered required. */
  map->enable_feature (HB_لیبل ('B','U','Z','Z')); /* Considered discretionary. */

  برائے (غیرعلامتی صحیح i = 0; i < ARRAY_لمبائی (common_features); i++)
    map->جوڑو_feature (common_features[i]);

  اگر (HB_DIRECTION_IS_HORIZONTAL (planner->props.direction))
    برائے (غیرعلامتی صحیح i = 0; i < ARRAY_لمبائی (horizontal_features); i++)
      map->جوڑو_feature (horizontal_features[i]);
  ورنہ
  {
    /* We only apply `vert` feature. See:
     * https://github.com/harfbuzz/harfbuzz/commit/d71c0df2d17f4590d5611239577a6cb532c26528
     * https://lists.freedesktop.org/archives/harfbuzz/2013-August/003490.html */

    /* We really want to find a 'vert' feature if there's any in the font, no
     * matter which script/langsys it is listed (or not) under.
     * See various bugs referenced from:
     * https://github.com/harfbuzz/harfbuzz/issues/63 */
    map->enable_feature (HB_لیبل ('v','e','r','t'), F_GLOBAL_تلاش);
  }

  اگر (عدد_user_features)
    map->is_simple = جھوٹ;
  برائے (غیرعلامتی صحیح i = 0; i < عدد_user_features; i++)
  {
    ثابت hb_feature_t *feature = &user_features[i];
    map->جوڑو_feature (feature->لیبل_2,
		      (feature->آغاز == HB_FEATURE_GLOBAL_آغاز &&
		       feature->انتہا == HB_FEATURE_GLOBAL_انتہا) ?  F_GLOBAL : F_NONE,
		      feature->value);
  }

  اگر (planner->shaper->override_features)
    planner->shaper->override_features (planner);
}


/*
 * shaper face data
 */

ساخت hb_ot_face_ڈیٹا_t {};

hb_ot_face_ڈیٹا_t *
hb_ot_shaper_face_ڈیٹا_create (hb_face_t *face)
{
  واپس (hb_ot_face_ڈیٹا_t *) HB_SHAPER_ڈیٹا_SUCCEEDED;
}

خالی
hb_ot_shaper_face_ڈیٹا_destroy (hb_ot_face_ڈیٹا_t *ڈیٹا)
{
}


/*
 * shaper font data
 */

ساخت hb_ot_font_ڈیٹا_t {
  OT::hb_scalar_cache_t غیر_مستعمل_2; // Just for alignment
};

hb_ot_font_ڈیٹا_t *
hb_ot_shaper_font_ڈیٹا_create (hb_font_t *font)
{
  ثابت OT::شے_Variation_Store &var_store = font->face->جدول.GDEF->جدول->get_var_store ();
  واپس (hb_ot_font_ڈیٹا_t *) var_store.create_cache ();
}

خالی
hb_ot_shaper_font_ڈیٹا_destroy (hb_ot_font_ڈیٹا_t *ڈیٹا)
{
  OT::شے_Variation_Store::destroy_cache ((OT::hb_scalar_cache_t *) ڈیٹا);
}


/*
 * shaper
 */

ساخت hb_ot_shape_context_t
{
  hb_ot_shape_plan_t *plan;
  hb_font_t *font;
  hb_face_t *face;
  hb_بفر_t  *بفر;
  ثابت hb_feature_t *user_features;
  غیرعلامتی صحیح        عدد_user_features;

  /* Transient stuff */
  hb_direction_t ہدف_direction;
};



/* Main shaper */


/* Prepare */

ساکن خالی
hb_سیٹ_unicode_props (hb_بفر_t *بفر,
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
  غیرعلامتی صحیح گنتی = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
  {
    info[i].نقاب = global_نقاب;
    hb_glyph_info_سیٹ_unicode_props (&info[i], بفر);

    اگر (info[i].codepoint < 0x80)
      جاری;

    غیرعلامتی gen_cat = _hb_glyph_info_get_general_category (&info[i]);
    اگر (FLAG_UNSAFE (gen_cat) &
	(FLAG (HB_UNICODE_GENERAL_CATEGORY_LOWERCASE_LETTER) |
	 FLAG (HB_UNICODE_GENERAL_CATEGORY_UPPERCASE_LETTER) |
	 FLAG (HB_UNICODE_GENERAL_CATEGORY_TITLECASE_LETTER) |
	 FLAG (HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER) |
	 FLAG (HB_UNICODE_GENERAL_CATEGORY_SPACE_SEPARATOR)))
      جاری;

    /* Marks are already set as continuation by the above line.
     * Handle Emoji_Modifier and ZWJ-continuation. */
    اگر (unlikely (gen_cat == HB_UNICODE_GENERAL_CATEGORY_MODIFIER_علامت &&
		  hb_اندر_range<hb_codepoint_t> (info[i].codepoint, 0x1F3FBu, 0x1F3FFu)))
    {
      hb_glyph_info_سیٹ_continuation (&info[i], بفر);
    }
    /* Regional_Indicators are hairy as hell...
     * https://github.com/harfbuzz/harfbuzz/issues/2265 */
    ورنہ اگر (unlikely (i && _hb_codepoint_is_regional_indicator (info[i].codepoint)))
    {
      اگر (_hb_codepoint_is_regional_indicator (info[i - 1].codepoint) &&
	  !_hb_glyph_info_is_continuation (&info[i - 1]))
	hb_glyph_info_سیٹ_continuation (&info[i], بفر);
    }
#اگر_غیر_تعریف_شدہ HB_NO_EMOJI_SEQUENCES
    ورنہ اگر (unlikely (_hb_glyph_info_is_zwj (&info[i])))
    {
      hb_glyph_info_سیٹ_continuation (&info[i], بفر);
      اگر (i + 1 < گنتی &&
	  _hb_unicode_is_emoji_Extended_Pictographic (info[i + 1].codepoint))
      {
	i++;
	info[i].نقاب = global_نقاب;
	hb_glyph_info_سیٹ_unicode_props (&info[i], بفر);
	hb_glyph_info_سیٹ_continuation (&info[i], بفر);
      }
    }
#اگر_ختم
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
    ورنہ اگر (unlikely (hb_اندر_ranges<hb_codepoint_t> (info[i].codepoint, 0xFF9Eu, 0xFF9Fu, 0xE0020u, 0xE007Fu)))
      hb_glyph_info_سیٹ_continuation (&info[i], بفر);
    ورنہ اگر (unlikely (info[i].codepoint == 0x2044u /* FRACTION SLASH */))
      بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_HAS_FRACTION_SLASH;
  }
}

ساکن خالی
hb_داخل_کرو_dotted_circle (hb_بفر_t *بفر, hb_font_t *font)
{
  اگر (unlikely (بفر->flags & HB_بفر_FLAG_DO_NOT_داخل_کرو_DOTTED_CIRCLE))
    واپس;

  اگر (!(بفر->flags & HB_بفر_FLAG_BOT) ||
      بفر->context_len[0] ||
      !_hb_glyph_info_is_unicode_mark (&بفر->info[0]))
    واپس;

  اگر (!font->has_glyph (0x25CCu))
    واپس;

  hb_glyph_info_t dottedcircle = {0};
  dottedcircle.codepoint = 0x25CCu;
  hb_glyph_info_سیٹ_unicode_props (&dottedcircle, بفر);

  بفر->clear_output ();

  بفر->idx = 0;
  hb_glyph_info_t info = dottedcircle;
  info.cluster = بفر->cur().cluster;
  info.نقاب = بفر->cur().نقاب;
  (خالی) بفر->output_info (info);

  بفر->ہمآہنگ ();
}

ساکن خالی
hb_form_clusters (hb_بفر_t *بفر)
{
  اگر (!(بفر->scratch_flags & HB_بفر_SCRATCH_FLAG_HAS_CONTINUATIONS))
    واپس;

  foreach_grapheme (بفر, آغاز, انتہا)
    بفر->ملاؤ_grapheme_clusters (آغاز, انتہا);
}

ساکن خالی
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
  اگر (unlikely (horiz_dir == HB_DIRECTION_RTL && direction == HB_DIRECTION_LTR))
  {
    منطقی ملا_نمبر = جھوٹ, ملا_letter = جھوٹ, ملا_ri = جھوٹ;
    ثابت خودکار* info = بفر->info;
    ثابت خودکار گنتی = بفر->len;
    برائے (غیرعلامتی i = 0; i < گنتی; i++)
    {
      خودکار gc = _hb_glyph_info_get_general_category (&info[i]);
      اگر (HB_UNICODE_GENERAL_CATEGORY_IS_LETTER (gc))
      {
	ملا_letter = سچ;
	توڑو;
      }
      ورنہ اگر (gc == HB_UNICODE_GENERAL_CATEGORY_DECIMAL_نمبر)
	ملا_نمبر = سچ;
      ورنہ اگر (unlikely (_hb_codepoint_is_regional_indicator (info[i].codepoint)))
	ملا_ri = سچ;
    }
    اگر ((ملا_نمبر || ملا_ri) && !ملا_letter)
      horiz_dir = HB_DIRECTION_LTR;
  }

  /* TODO vertical:
   * The only BTT vertical script is Ogham, but it's not clear to me whether OpenType
   * Ogham fonts are supposed to be implemented BTT or not.  Need to research that
   * first. */
  اگر ((HB_DIRECTION_IS_HORIZONTAL (direction) &&
       direction != horiz_dir && HB_DIRECTION_IS_VALID (horiz_dir)) ||
      (HB_DIRECTION_IS_VERTICAL   (direction) &&
       direction != HB_DIRECTION_TTB))
  {
    hb_ot_layout_الٹ_graphemes (بفر);
    بفر->props.direction = HB_DIRECTION_الٹ (بفر->props.direction);
  }
}


/*
 * Substitute
 */

ساکن اندرونی خالی
hb_ot_rotate_chars (ثابت hb_ot_shape_context_t *c)
{
  hb_بفر_t *بفر = c->بفر;
  غیرعلامتی صحیح گنتی = بفر->len;
  hb_glyph_info_t *info = بفر->info;

  اگر (HB_DIRECTION_IS_BACKWARD (c->ہدف_direction))
  {
    hb_unicode_funcs_t *unicode = بفر->unicode;
    hb_نقاب_t rtlm_نقاب = c->plan->rtlm_نقاب;

    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++) {
      hb_codepoint_t codepoint = unicode->mirroring (info[i].codepoint);
      اگر (unlikely (codepoint != info[i].codepoint && c->font->has_glyph (codepoint)))
	info[i].codepoint = codepoint;
      ورنہ
	info[i].نقاب |= rtlm_نقاب;
    }
  }

#اگر_غیر_تعریف_شدہ HB_NO_VERTICAL
  اگر (HB_DIRECTION_IS_VERTICAL (c->ہدف_direction) && !c->plan->has_vert)
  {
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++) {
      hb_codepoint_t codepoint = hb_unicode_funcs_t::vertical_char_for (info[i].codepoint);
      اگر (unlikely (codepoint != info[i].codepoint && c->font->has_glyph (codepoint)))
	info[i].codepoint = codepoint;
    }
  }
#اگر_ختم
}

ساکن اندرونی خالی
hb_ot_shape_تیاری_masks_fraction (ثابت hb_ot_shape_context_t *c)
{
#اگر_تعریف_شدہ HB_NO_OT_SHAPE_FRACTIONS
  واپس;
#اگر_ختم

  اگر (!(c->بفر->scratch_flags & HB_بفر_SCRATCH_FLAG_HAS_FRACTION_SLASH) ||
      !c->plan->has_frac)
    واپس;

  hb_بفر_t *بفر = c->بفر;

  hb_نقاب_t pre_نقاب, post_نقاب;
  اگر (HB_DIRECTION_IS_FORWARD (بفر->props.direction))
  {
    pre_نقاب = c->plan->numr_نقاب | c->plan->frac_نقاب;
    post_نقاب = c->plan->frac_نقاب | c->plan->dnom_نقاب;
  }
  ورنہ
  {
    pre_نقاب = c->plan->frac_نقاب | c->plan->dnom_نقاب;
    post_نقاب = c->plan->numr_نقاب | c->plan->frac_نقاب;
  }

  غیرعلامتی صحیح گنتی = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
  {
    اگر (info[i].codepoint == 0x2044u) /* FRACTION SLASH */
    {
      غیرعلامتی صحیح آغاز = i, انتہا = i + 1;
      جب (آغاز &&
	     _hb_glyph_info_get_general_category (&info[آغاز - 1]) ==
	     HB_UNICODE_GENERAL_CATEGORY_DECIMAL_نمبر)
	آغاز--;
      جب (انتہا < گنتی &&
	     _hb_glyph_info_get_general_category (&info[انتہا]) ==
	     HB_UNICODE_GENERAL_CATEGORY_DECIMAL_نمبر)
	انتہا++;
      اگر (آغاز == i || انتہا == i + 1)
      {
        اگر (آغاز == i)
	  بفر->unsafe_مقصد_concat (آغاز, آغاز + 1);
	اگر (انتہا == i + 1)
	  بفر->unsafe_مقصد_concat (انتہا - 1, انتہا);
	جاری;
      }

      بفر->unsafe_مقصد_break (آغاز, انتہا);

      برائے (غیرعلامتی صحیح j = آغاز; j < i; j++)
	info[j].نقاب |= pre_نقاب;
      info[i].نقاب |= c->plan->frac_نقاب;
      برائے (غیرعلامتی صحیح j = i + 1; j < انتہا; j++)
	info[j].نقاب |= post_نقاب;

      i = انتہا - 1;
    }
  }
}

ساکن اندرونی خالی
hb_ot_shape_تیاری_masks (ثابت hb_ot_shape_context_t *c)
{
  hb_ot_map_t *map = &c->plan->map;
  hb_بفر_t *بفر = c->بفر;

  hb_ot_shape_تیاری_masks_fraction (c);

  اگر (c->plan->shaper->تیاری_masks)
    c->plan->shaper->تیاری_masks (c->plan, بفر, c->font);

  برائے (غیرعلامتی صحیح i = 0; i < c->عدد_user_features; i++)
  {
    ثابت hb_feature_t *feature = &c->user_features[i];
    اگر (!(feature->آغاز == HB_FEATURE_GLOBAL_آغاز && feature->انتہا == HB_FEATURE_GLOBAL_انتہا)) {
      غیرعلامتی صحیح سرکاؤ;
      hb_نقاب_t نقاب = map->get_نقاب (feature->لیبل_2, &سرکاؤ);
      بفر->سیٹ_masks (feature->value << سرکاؤ, نقاب, feature->آغاز, feature->انتہا);
    }
  }
}

ساکن خالی
hb_ot_zero_چوڑائی_default_ignorables (ثابت hb_بفر_t *بفر)
{
  اگر (!(بفر->scratch_flags & HB_بفر_SCRATCH_FLAG_HAS_DEFAULT_IGNORABLES) ||
      (بفر->flags & HB_بفر_FLAG_PRESERVE_DEFAULT_IGNORABLES) ||
      (بفر->flags & HB_بفر_FLAG_REMOVE_DEFAULT_IGNORABLES))
    واپس;

  غیرعلامتی صحیح گنتی = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  hb_glyph_position_t *pos = بفر->pos;
  غیرعلامتی صحیح i = 0;
  برائے (i = 0; i < گنتی; i++)
    اگر (unlikely (_hb_glyph_info_is_default_ignorable (&info[i])))
    {
      pos[i].x_advance = pos[i].y_advance = 0;
      اگر (HB_DIRECTION_IS_HORIZONTAL (بفر->props.direction))
	pos[i].x_انحراف_2 = 0;
      ورنہ
        pos[i].y_انحراف_2 = 0;
    }
}

ساکن خالی
hb_ot_deal_with_variation_selectors (hb_بفر_t *بفر)
{
  اگر (!(بفر->scratch_flags & HB_بفر_SCRATCH_FLAG_HAS_VARIATION_SELECTOR_FALLBACK) ||
	بفر->not_ملا_variation_selector == HB_CODEPOINT_INVALID)
    واپس;

  غیرعلامتی صحیح گنتی = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  hb_glyph_position_t *pos = بفر->pos;

  برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
  {
    اگر (_hb_glyph_info_is_variation_selector (&info[i]))
    {
      info[i].codepoint = بفر->not_ملا_variation_selector;
      pos[i].x_advance = pos[i].y_advance = pos[i].x_انحراف_2 = pos[i].y_انحراف_2 = 0;
      hb_glyph_info_سیٹ_variation_selector (&info[i], جھوٹ);
    }
  }
}

ساکن خالی
hb_ot_hide_default_ignorables (hb_بفر_t *بفر,
			       hb_font_t   *font)
{
  اگر (!(بفر->scratch_flags & HB_بفر_SCRATCH_FLAG_HAS_DEFAULT_IGNORABLES) ||
      (بفر->flags & HB_بفر_FLAG_PRESERVE_DEFAULT_IGNORABLES))
    واپس;

  غیرعلامتی صحیح گنتی = بفر->len;
  hb_glyph_info_t *info = بفر->info;

  hb_codepoint_t invisible = بفر->invisible;
  اگر (!(بفر->flags & HB_بفر_FLAG_REMOVE_DEFAULT_IGNORABLES) &&
      (invisible || font->get_nominal_glyph (' ', &invisible)))
  {
    /* Replace default-ignorables with a zero-advance invisible glyph. */
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      اگر (_hb_glyph_info_is_default_ignorable (&info[i]))
	info[i].codepoint = invisible;
    }
  }
  ورنہ
    بفر->delete_glyphs_inplace (_hb_glyph_info_is_default_ignorable);
}


ساکن اندرونی خالی
hb_ot_map_glyphs_fast (hb_بفر_t  *بفر)
{
  /* Normalization process sets up normalizer_glyph_index(), we just copy it. */
  غیرعلامتی صحیح گنتی = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    info[i].codepoint = info[i].normalizer_glyph_اشاریہ();

  بفر->content_type = HB_بفر_CONTENT_TYPE_GLYPHS;
}

ساکن اندرونی خالی
hb_synthesize_glyph_classes (hb_بفر_t *بفر)
{
  غیرعلامتی صحیح گنتی = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
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
	    HB_OT_LAYOUT_GLYPH_PROPS_بنیاد_GLYPH :
	    HB_OT_LAYOUT_GLYPH_PROPS_MARK;
    hb_glyph_info_سیٹ_glyph_props (&info[i], klass);
  }
}

ساکن اندرونی خالی
hb_ot_substitute_default (ثابت hb_ot_shape_context_t *c)
{
  hb_بفر_t *بفر = c->بفر;

  hb_ot_rotate_chars (c);

  HB_بفر_ALLOCATE_VAR (بفر, normalizer_glyph_اشاریہ);

  _hb_ot_shape_normalize (c->plan, بفر, c->font);

  hb_ot_shape_تیاری_masks (c);

  /* This is unfortunate to go here, but necessary... */
  اگر (c->plan->fallback_mark_positioning)
    _hb_ot_shape_fallback_mark_position_recategorize_marks (c->plan, c->font, بفر);

  hb_ot_map_glyphs_fast (بفر);

  HB_بفر_DEALLOCATE_VAR (بفر, normalizer_glyph_اشاریہ);
}

ساکن اندرونی خالی
hb_ot_substitute_plan (ثابت hb_ot_shape_context_t *c)
{
  hb_بفر_t *بفر = c->بفر;

#اگر_غیر_تعریف_شدہ HB_NO_AAT_SHAPE
  اگر (unlikely (c->plan->apply_morx))
  {
    hb_ot_layout_substitute_آغاز (c->font, بفر);

    اگر (c->plan->fallback_glyph_classes)
      hb_synthesize_glyph_classes (بفر);

    hb_aat_layout_substitute (c->plan, c->font, c->بفر,
			      c->user_features, c->عدد_user_features);
    /* The buffer digest is only used by the OT lookup-apply loop;
     * without GPOS ahead, nothing consumes it. */
    اگر (c->plan->apply_gpos)
      c->بفر->update_digest ();
  }
  ورنہ
#اگر_ختم
  {
    hb_ot_layout_substitute_آغاز_with_digest (c->font, بفر);

    اگر (c->plan->fallback_glyph_classes)
      hb_synthesize_glyph_classes (بفر);

    c->plan->substitute (c->font, بفر);
  }
}

ساکن اندرونی خالی
hb_ot_substitute_pre (ثابت hb_ot_shape_context_t *c)
{
  hb_ot_substitute_default (c);

  hb_بفر_allocate_gsubgpos_vars (c->بفر);

  hb_ot_substitute_plan (c);

#اگر_غیر_تعریف_شدہ HB_NO_AAT_SHAPE
  اگر (c->plan->apply_morx && c->plan->apply_gpos)
    hb_aat_layout_remove_deleted_glyphs (c->بفر);
#اگر_ختم
}

ساکن اندرونی خالی
hb_ot_substitute_post (ثابت hb_ot_shape_context_t *c)
{
#اگر_غیر_تعریف_شدہ HB_NO_AAT_SHAPE
  اگر (c->plan->apply_morx && !c->plan->apply_gpos)
    hb_aat_layout_remove_deleted_glyphs (c->بفر);
#اگر_ختم

  hb_ot_deal_with_variation_selectors (c->بفر);
  hb_ot_hide_default_ignorables (c->بفر, c->font);

  اگر (c->plan->shaper->postprocess_glyphs &&
    c->بفر->پیغام(c->font, "start postprocess-glyphs")) {
    c->plan->shaper->postprocess_glyphs (c->plan, c->بفر, c->font);
    (خالی) c->بفر->پیغام(c->font, "end postprocess-glyphs");
  }
}


/*
 * Position
 */

ساکن اندرونی خالی
adjust_mark_offsets (hb_glyph_position_t *pos)
{
  pos->x_انحراف_2 = hb_saturate_sub (pos->x_انحراف_2, pos->x_advance);
  pos->y_انحراف_2 = hb_saturate_sub (pos->y_انحراف_2, pos->y_advance);
}

ساکن اندرونی خالی
zero_mark_چوڑائی (hb_glyph_position_t *pos)
{
  pos->x_advance = 0;
  pos->y_advance = 0;
}

ساکن اندرونی خالی
zero_mark_widths_by_gdef (hb_بفر_t *بفر, منطقی adjust_offsets)
{
  غیرعلامتی صحیح گنتی = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    اگر (_hb_glyph_info_is_mark (&info[i]))
    {
      اگر (adjust_offsets)
	adjust_mark_offsets (&بفر->pos[i]);
      zero_mark_چوڑائی (&بفر->pos[i]);
    }
}

ساکن اندرونی خالی
hb_ot_position_default (ثابت hb_ot_shape_context_t *c)
{
  hb_direction_t direction = c->بفر->props.direction;
  غیرعلامتی صحیح گنتی = c->بفر->len;
  hb_glyph_info_t *info = c->بفر->info;
  hb_glyph_position_t *pos = c->بفر->pos;

  اگر (HB_DIRECTION_IS_HORIZONTAL (direction))
  {
    c->font->get_glyph_h_advances (گنتی, &info[0].codepoint, حجم(info[0]),
				   &pos[0].x_advance, حجم(pos[0]));
    // h_origin defaults to zero; only apply it if the font has it.
    اگر (c->font->has_glyph_h_origin_func () || c->font->has_glyph_h_origins_func ())
      c->font->تفریق_glyph_h_origins (c->بفر);
  }
  ورنہ
  {
    c->font->get_glyph_v_advances (گنتی, &info[0].codepoint, حجم(info[0]),
				   &pos[0].y_advance, حجم(pos[0]));
    // v_origin defaults to non-zero; apply even if only fallback is there.
    c->font->تفریق_glyph_v_origins (c->بفر);
  }
  اگر (c->بفر->scratch_flags & HB_بفر_SCRATCH_FLAG_HAS_SPACE_FALLBACK)
    _hb_ot_shape_fallback_spaces (c->plan, c->font, c->بفر);
}

ساکن اندرونی خالی
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
  منطقی adjust_offsets_when_zeroing = c->plan->adjust_mark_positioning_when_zeroing &&
				     HB_DIRECTION_IS_FORWARD (c->بفر->props.direction);

  /* We change glyph origin to what GPOS expects (horizontal), apply GPOS, change it back. */

  // h_origin defaults to zero; only apply it if the font has it.
  اگر (c->font->has_glyph_h_origin_func () || c->font->has_glyph_h_origins_func ())
    c->font->جوڑو_glyph_h_origins (c->بفر);

  hb_ot_layout_position_آغاز (c->font, c->بفر);

  اگر (c->plan->zero_marks)
    انتخاب (c->plan->shaper->zero_چوڑائی_marks)
    {
      حالت HB_OT_SHAPE_ZERO_چوڑائی_MARKS_BY_GDEF_EARLY:
	zero_mark_widths_by_gdef (c->بفر, adjust_offsets_when_zeroing);
	توڑو;

      طےشدہ:
      حالت HB_OT_SHAPE_ZERO_چوڑائی_MARKS_NONE:
      حالت HB_OT_SHAPE_ZERO_چوڑائی_MARKS_BY_GDEF_LATE:
	توڑو;
    }

  c->plan->position (c->font, c->بفر);

  اگر (c->plan->zero_marks)
    انتخاب (c->plan->shaper->zero_چوڑائی_marks)
    {
      حالت HB_OT_SHAPE_ZERO_چوڑائی_MARKS_BY_GDEF_LATE:
	zero_mark_widths_by_gdef (c->بفر, adjust_offsets_when_zeroing);
	توڑو;

      طےشدہ:
      حالت HB_OT_SHAPE_ZERO_چوڑائی_MARKS_NONE:
      حالت HB_OT_SHAPE_ZERO_چوڑائی_MARKS_BY_GDEF_EARLY:
	توڑو;
    }

  /* Finish off.  Has to follow a certain order. */
  hb_ot_layout_position_اختتام_advances (c->font, c->بفر);
  hb_ot_zero_چوڑائی_default_ignorables (c->بفر);
  hb_ot_layout_position_اختتام_offsets (c->font, c->بفر);

  // h_origin defaults to zero; only apply it if the font has it.
  اگر (c->font->has_glyph_h_origin_func () || c->font->has_glyph_h_origins_func ())
    c->font->تفریق_glyph_h_origins (c->بفر);

  اگر (c->plan->fallback_mark_positioning)
    _hb_ot_shape_fallback_mark_position (c->plan, c->font, c->بفر,
					 adjust_offsets_when_zeroing);
}

ساکن اندرونی خالی
hb_ot_position (ثابت hb_ot_shape_context_t *c)
{
  c->بفر->clear_positions ();

  hb_ot_position_default (c);

  hb_ot_position_plan (c);

  اگر (HB_DIRECTION_IS_BACKWARD (c->بفر->props.direction))
    hb_بفر_الٹ (c->بفر);

  hb_بفر_deallocate_gsubgpos_vars (c->بفر);
}

ساکن اندرونی خالی
hb_propagate_flags (hb_بفر_t *بفر)
{
  /* Propagate cluster-level glyph flags to be the same on all cluster glyphs.
   * Simplifies using them. */

  hb_نقاب_t and_نقاب = HB_GLYPH_FLAG_DEFINED;
  اگر ((بفر->flags & HB_بفر_FLAG_PRODUCE_UNSAFE_مقصد_CONCAT) == 0)
    and_نقاب &= ~HB_GLYPH_FLAG_UNSAFE_مقصد_CONCAT;

  hb_glyph_info_t *info = بفر->info;

  اگر ((بفر->flags & HB_بفر_FLAG_PRODUCE_محفوظ_مقصد_داخل_کرو_TATWEEL) == 0)
  {
    foreach_cluster (بفر, آغاز, انتہا)
    {
      اگر (انتہا - آغاز == 1)
      {
        info[آغاز].نقاب &= and_نقاب;
	جاری;
      }

      غیرعلامتی صحیح نقاب = 0;
      برائے (غیرعلامتی صحیح i = آغاز; i < انتہا; i++)
	نقاب |= info[i].نقاب;

      نقاب &= and_نقاب;

      برائے (غیرعلامتی صحیح i = آغاز; i < انتہا; i++)
	info[i].نقاب = نقاب;
    }
    واپس;
  }

  /* If we are producing SAFE_TO_INSERT_TATWEEL, then do two things:
   *
   * - If the places that the Arabic shaper marked as SAFE_TO_INSERT_TATWEEL,
   *   are UNSAFE_TO_BREAK, then clear the SAFE_TO_INSERT_TATWEEL,
   * - Any place that is SAFE_TO_INSERT_TATWEEL, is also now UNSAFE_TO_BREAK.
   *
   * We couldn't make this interaction earlier. It has to be done this way.
   */
  foreach_cluster (بفر, آغاز, انتہا)
  {
    غیرعلامتی صحیح نقاب = 0;
    برائے (غیرعلامتی صحیح i = آغاز; i < انتہا; i++)
      نقاب |= info[i].نقاب;

    اگر (نقاب & HB_GLYPH_FLAG_UNSAFE_مقصد_BREAK)
      نقاب &= ~HB_GLYPH_FLAG_محفوظ_مقصد_داخل_کرو_TATWEEL;
    اگر (نقاب & HB_GLYPH_FLAG_محفوظ_مقصد_داخل_کرو_TATWEEL)
      نقاب |= HB_GLYPH_FLAG_UNSAFE_مقصد_BREAK | HB_GLYPH_FLAG_UNSAFE_مقصد_CONCAT;

    نقاب &= and_نقاب;

    برائے (غیرعلامتی صحیح i = آغاز; i < انتہا; i++)
      info[i].نقاب = نقاب;
  }
}

/* Pull it all together! */

ساکن خالی
hb_ot_shape_اندرونی (hb_ot_shape_context_t *c)
{
  /* Save the original direction, we use it later. */
  c->ہدف_direction = c->بفر->props.direction;

  hb_بفر_allocate_unicode_vars (c->بفر);

  hb_سیٹ_unicode_props (c->بفر, c->plan->map.get_global_نقاب ());
  hb_داخل_کرو_dotted_circle (c->بفر, c->font);

  hb_form_clusters (c->بفر);

  hb_ensure_native_direction (c->بفر);

  اگر (c->plan->shaper->preprocess_متن &&
      c->بفر->پیغام(c->font, "start preprocess-text"))
  {
    c->plan->shaper->preprocess_متن (c->plan, c->بفر, c->font);
    (خالی) c->بفر->پیغام(c->font, "end preprocess-text");
  }

  hb_ot_substitute_pre (c);
  hb_ot_position (c);
  hb_ot_substitute_post (c);

  hb_propagate_flags (c->بفر);

  hb_بفر_deallocate_unicode_vars (c->بفر);

  c->بفر->props.direction = c->ہدف_direction;
}


hb_bool_t
_hb_ot_shape (hb_shape_plan_t    *shape_plan,
	      hb_font_t          *font,
	      hb_بفر_t        *بفر,
	      ثابت hb_feature_t *features,
	      غیرعلامتی صحیح        عدد_features)
{
  hb_ot_shape_context_t c = {&shape_plan->ot, font, font->face, بفر, features, عدد_features};
  hb_ot_shape_اندرونی (&c);

  واپس سچ;
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
خالی
hb_ot_shape_plan_collect_lookups (hb_shape_plan_t *shape_plan,
				  hb_لیبل_t         جدول_لیبل_2,
				  hb_سیٹ_t        *lookup_indexes /*برآمد*/)
{
  shape_plan->ot.collect_lookups (جدول_لیبل_2, lookup_indexes);
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
غیرعلامتی صحیح
hb_ot_shape_plan_get_feature_لیبل_فہرست (hb_shape_plan_t *shape_plan,
				   غیرعلامتی صحیح     آغاز_انحراف,
				   غیرعلامتی صحیح    *لیبل_گنتی, /* IN/OUT */
				   hb_لیبل_t        *لیبل_فہرست /*برآمد*/)
{
#اگر_غیر_تعریف_شدہ HB_NO_OT_SHAPE
  واپس shape_plan->ot.map.get_feature_لیبل_فہرست (آغاز_انحراف, لیبل_گنتی, لیبل_فہرست);
#ورنہ
  اگر (لیبل_گنتی)
	*لیبل_گنتی = 0;
  واپس 0;
#اگر_ختم
}


/* TODO Move this to hb-ot-shape-normalize, make it do decompose, and make it public. */
ساکن خالی
جوڑو_char (hb_font_t          *font,
	  hb_unicode_funcs_t *unicode,
	  hb_bool_t           mirror,
	  hb_codepoint_t      u,
	  hb_سیٹ_t           *glyphs)
{
  hb_codepoint_t glyph;
  اگر (font->get_nominal_glyph (u, &glyph))
    glyphs->جوڑو (glyph);
  اگر (mirror)
  {
    hb_codepoint_t m = unicode->mirroring (u);
    اگر (m != u && font->get_nominal_glyph (m, &glyph))
      glyphs->جوڑو (glyph);
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
خالی
hb_ot_shape_glyphs_احاطہ (hb_font_t          *font,
			    hb_بفر_t        *بفر,
			    ثابت hb_feature_t *features,
			    غیرعلامتی صحیح        عدد_features,
			    hb_سیٹ_t           *glyphs)
{
  ثابت حرف *shapers[] = {"ot", خالی_اشاریہ};
  hb_shape_plan_t *shape_plan = hb_shape_plan_create_cached (font->face, &بفر->props,
							     features, عدد_features, shapers);

  منطقی mirror = hb_script_get_horizontal_direction (بفر->props.script) == HB_DIRECTION_RTL;

  غیرعلامتی صحیح گنتی = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    جوڑو_char (font, بفر->unicode, mirror, info[i].codepoint, glyphs);

  hb_سیٹ_t *lookups = hb_سیٹ_create ();
  hb_ot_shape_plan_collect_lookups (shape_plan, HB_OT_لیبل_GSUB, lookups);
  hb_ot_layout_lookups_substitute_احاطہ (font->face, lookups, glyphs);

  hb_سیٹ_destroy (lookups);

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
غیرعلامتی صحیح
hb_ot_shape_get_بفر_format_serial (خالی)
{
  واپس HB_OT_SHAPE_بفر_FORMAT_SERIAL;
}

#اگر_ختم

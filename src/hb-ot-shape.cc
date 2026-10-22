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

#که_نه_تعریف_شوی HB_NO_OT_SHAPE

#که_تعریف_شوی HB_NO_OT_LAYOUT
#تېروتنه "Cannot compile 'ot' shaper with HB_NO_OT_LAYOUT."
#د_که_پای

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
{ أرجع hb_دننه_range<hb_codepoint_t> (u, 0x1F1E6u, 0x1F1FFu); }

#که_نه_تعریف_شوی HB_NO_AAT_SHAPE
ساكن مضمن منطقي
_hb_apply_morx (hb_face_t *face, ثابت hb_segment_properties_t &props)
{
  /* https://github.com/harfbuzz/harfbuzz/issues/2124 */
  أرجع hb_aat_layout_has_substitution (face) &&
	 (HB_DIRECTION_IS_HORIZONTAL (props.direction) || !hb_ot_layout_has_substitution (face));
}
#د_که_پای

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
			      بینښه صحيح                    عدد_user_features);

hb_ot_shape_planner_t::hb_ot_shape_planner_t (hb_face_t                     *face,
					      ثابت hb_segment_properties_t &props) :
						face (face),
						props (props),
						map (face, props)
#که_نه_تعریف_شوی HB_NO_AAT_SHAPE
						, aat_map (face, props)
						, apply_morx (_hb_apply_morx (face, props))
#د_که_پای
{
  shaper = hb_ot_shaper_categorize (props.script, props.direction, map.chosen_script[0]);

  script_zero_marks = shaper->zero_پلنوالی_marks != HB_OT_SHAPE_ZERO_پلنوالی_MARKS_NONE;
  script_fallback_position = shaper->fallback_position;

#که_نه_تعریف_شوی HB_NO_AAT_SHAPE
  /* https://github.com/harfbuzz/harfbuzz/issues/1528 */
  إذا (apply_morx && shaper != &_hb_ot_shaper_default)
    shaper = &_hb_ot_shaper_dumber;
#د_که_پای
}

فراغ
hb_ot_shape_planner_t::compile (hb_ot_shape_plan_t           &plan,
				ثابت hb_ot_shape_plan_کلی_t &key)
{
  plan.props = props;
  plan.shaper = shaper;
  map.compile (plan.map, key);
#که_نه_تعریف_شوی HB_NO_AAT_SHAPE
  إذا (apply_morx)
    aat_map.compile (plan.aat_map);
#د_که_پای

#که_نه_تعریف_شوی HB_NO_OT_SHAPE_FRACTIONS
  plan.frac_ماسک = plan.map.get_1_ماسک (HB_نښان ('f','r','a','c'));
  plan.numr_ماسک = plan.map.get_1_ماسک (HB_نښان ('n','u','m','r'));
  plan.dnom_ماسک = plan.map.get_1_ماسک (HB_نښان ('d','n','o','m'));
  plan.has_frac = plan.frac_ماسک || (plan.numr_ماسک && plan.dnom_ماسک);
#د_که_پای

  plan.rtlm_ماسک = plan.map.get_1_ماسک (HB_نښان ('r','t','l','m'));
  plan.has_vert = !!plan.map.get_1_ماسک (HB_نښان ('v','e','r','t'));

  hb_نښان_t kern_نښان = HB_DIRECTION_IS_HORIZONTAL (props.direction) ?
		      HB_نښان ('k','e','r','n') : HB_نښان ('v','k','r','n');
#که_نه_تعریف_شوی HB_NO_OT_KERN
  plan.kern_ماسک = plan.map.get_ماسک (kern_نښان);
  plan.requested_kerning = !!plan.kern_ماسک;
#د_که_پای

  منطقي has_gpos_kern = plan.map.get_feature_شاخص (1, kern_نښان) != HB_OT_LAYOUT_NO_FEATURE_شاخص;
  منطقي disable_gpos = plan.shaper->gpos_نښان &&
		      plan.shaper->gpos_نښان != plan.map.chosen_script[1];

  /*
   * Decide who provides glyph classes. GDEF or Unicode.
   */

  إذا (!hb_ot_layout_has_glyph_classes (face))
    plan.fallback_glyph_classes = ریښتیا;

  /*
   * Decide who does substitutions. GSUB, morx, or fallback.
   */

#که_نه_تعریف_شوی HB_NO_AAT_SHAPE
  plan.apply_morx = apply_morx;
#د_که_پای

  /*
   * Decide who does positioning. GPOS, kerx, kern, or fallback.
   */

#که_نه_تعریف_شوی HB_NO_AAT_SHAPE
  منطقي has_kerx = hb_aat_layout_has_positioning (face);
  منطقي has_gsub = !apply_morx && hb_ot_layout_has_substitution (face);
#د_که_پای
  منطقي has_gpos = !disable_gpos && hb_ot_layout_has_positioning (face);
  إذا (خطأ)
    {}
#که_نه_تعریف_شوی HB_NO_AAT_SHAPE
  /* Prefer GPOS over kerx if GSUB is present;
   * https://github.com/harfbuzz/harfbuzz/issues/3008 */
  وإلا إذا (has_kerx && !(has_gsub && has_gpos))
    plan.apply_kerx = ریښتیا;
#د_که_پای
  وإلا إذا (has_gpos)
    plan.apply_gpos = ریښتیا;

  إذا (!plan.apply_kerx && (!has_gpos_kern || !plan.apply_gpos))
  {
    إذا (خطأ) {}
#که_نه_تعریف_شوی HB_NO_AAT_SHAPE
    وإلا إذا (has_kerx)
      plan.apply_kerx = ریښتیا;
#د_که_پای
#که_نه_تعریف_شوی HB_NO_OT_KERN
    وإلا إذا (hb_ot_layout_has_kerning (face))
      plan.apply_kern = script_fallback_position; // Not all shapers apply legacy `kern`
#د_که_پای
    وإلا {}
  }

  plan.apply_fallback_kern = script_fallback_position && !(plan.apply_gpos || plan.apply_kerx || plan.apply_kern);

  plan.zero_marks = script_zero_marks &&
		    !plan.apply_kerx &&
		    (!plan.apply_kern
#که_نه_تعریف_شوی HB_NO_OT_KERN
		     || !hb_ot_layout_has_machine_kerning (face)
#د_که_پای
		    );
  plan.has_gpos_mark = !!plan.map.get_1_ماسک (HB_نښان ('m','a','r','k'));

  plan.adjust_mark_positioning_when_zeroing = !plan.apply_gpos &&
					      !plan.apply_kerx &&
					      (!plan.apply_kern
#که_نه_تعریف_شوی HB_NO_OT_KERN
					       || !hb_ot_layout_has_cross_kerning (face)
#د_که_پای
					      );

  plan.fallback_mark_positioning = plan.adjust_mark_positioning_when_zeroing &&
				   script_fallback_position;

#که_نه_تعریف_شوی HB_NO_AAT_SHAPE
  /* If we're using morx shaping, we cancel mark position adjustment because
     Apple Color Emoji assumes this will NOT be done when forming emoji sequences;
     https://github.com/harfbuzz/harfbuzz/issues/2967. */
  إذا (plan.apply_morx)
    plan.adjust_mark_positioning_when_zeroing = خطأ;

  /* According to Ned, trak is applied by default for "modern fonts", as detected by presence of STAT table. */
#که_نه_تعریف_شوی HB_NO_STYLE
  plan.apply_trak = hb_aat_layout_has_tracking (face) && face->جدول.STAT->has_معلومات ();
#وإلا
  plan.apply_trak = خطأ;
#د_که_پای

#د_که_پای
}

منطقي
hb_ot_shape_plan_t::init0 (hb_face_t                     *face,
			   ثابت hb_shape_plan_کلی_t     *key)
{
  map.init ();

  hb_ot_shape_planner_t planner (face,
				 key->props);

  hb_ot_shape_collect_features (&planner,
				key->user_features,
				key->عدد_user_features);

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

  أرجع ریښتیا;
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
#که_نه_تعریف_شوی HB_NO_AAT_SHAPE
  وإلا إذا (هذا->apply_kerx)
    hb_aat_layout_position (هذا, font, بفر);
#د_که_پای

#که_نه_تعریف_شوی HB_NO_OT_KERN
  إذا (هذا->apply_kern)
    hb_ot_layout_kern (هذا, font, بفر);
#د_که_پای
  وإلا إذا (هذا->apply_fallback_kern)
    _hb_ot_shape_fallback_kern (هذا, font, بفر);

#که_نه_تعریف_شوی HB_NO_AAT_SHAPE
  إذا (هذا->apply_trak)
    hb_aat_layout_track (هذا, font, بفر);
#د_که_پای
}


ساكن ثابت hb_ot_map_feature_t
common_features[] =
{
  {HB_نښان('a','b','v','m'), F_GLOBAL},
  {HB_نښان('b','l','w','m'), F_GLOBAL},
  {HB_نښان('c','c','m','p'), F_GLOBAL},
  {HB_نښان('l','o','c','l'), F_GLOBAL},
  {HB_نښان('m','a','r','k'), F_GLOBAL_MANUAL_JOINERS},
  {HB_نښان('m','k','m','k'), F_GLOBAL_MANUAL_JOINERS},
  {HB_نښان('r','l','i','g'), F_GLOBAL},
};


ساكن ثابت hb_ot_map_feature_t
horizontal_features[] =
{
  {HB_نښان('c','a','l','t'), F_GLOBAL},
  {HB_نښان('c','l','i','g'), F_GLOBAL},
  {HB_نښان('c','u','r','s'), F_GLOBAL},
  {HB_نښان('d','i','s','t'), F_GLOBAL},
  {HB_نښان('k','e','r','n'), F_GLOBAL_HAS_FALLBACK},
  {HB_نښان('l','i','g','a'), F_GLOBAL},
  {HB_نښان('r','c','l','t'), F_GLOBAL},
};

ساكن فراغ
hb_ot_shape_collect_features (hb_ot_shape_planner_t *planner,
			      ثابت hb_feature_t    *user_features,
			      بینښه صحيح           عدد_user_features)
{
  hb_ot_map_builder_t *map = &planner->map;

  map->is_simple = ریښتیا;

  map->enable_feature (HB_نښان('r','v','r','n'));
  map->جمع_کړه_gsub_pause (تش_اشاره);

  بدل (planner->props.direction)
  {
    حالة HB_DIRECTION_LTR:
      map->enable_feature (HB_نښان ('l','t','r','a'));
      map->enable_feature (HB_نښان ('l','t','r','m'));
      اكسر;
    حالة HB_DIRECTION_RTL:
      map->enable_feature (HB_نښان ('r','t','l','a'));
      map->جمع_کړه_feature (HB_نښان ('r','t','l','m'));
      اكسر;
    حالة HB_DIRECTION_TTB:
    حالة HB_DIRECTION_BTT:
    حالة HB_DIRECTION_INVALID:
    افتراضي:
      اكسر;
  }

#که_نه_تعریف_شوی HB_NO_OT_SHAPE_FRACTIONS
  /* Automatic fractions. */
  map->جمع_کړه_feature (HB_نښان ('f','r','a','c'));
  map->جمع_کړه_feature (HB_نښان ('n','u','m','r'));
  map->جمع_کړه_feature (HB_نښان ('d','n','o','m'));
#د_که_پای

  /* Random! */
  map->enable_feature (HB_نښان ('r','a','n','d'), F_RANDOM, HB_OT_MAP_MAX_ارزښت);

  map->enable_feature (HB_نښان ('H','a','r','f')); /* Considered required. */
  map->enable_feature (HB_نښان ('H','A','R','F')); /* Considered discretionary. */

  إذا (planner->shaper->collect_features)
  {
    map->is_simple = خطأ;
    planner->shaper->collect_features (planner);
  }

  map->enable_feature (HB_نښان ('B','u','z','z')); /* Considered required. */
  map->enable_feature (HB_نښان ('B','U','Z','Z')); /* Considered discretionary. */

  لكل (بینښه صحيح i = 0; i < ARRAY_اوږدتیا (common_features); i++)
    map->جمع_کړه_feature (common_features[i]);

  إذا (HB_DIRECTION_IS_HORIZONTAL (planner->props.direction))
    لكل (بینښه صحيح i = 0; i < ARRAY_اوږدتیا (horizontal_features); i++)
      map->جمع_کړه_feature (horizontal_features[i]);
  وإلا
  {
    /* We only apply `vert` feature. See:
     * https://github.com/harfbuzz/harfbuzz/commit/d71c0df2d17f4590d5611239577a6cb532c26528
     * https://lists.freedesktop.org/archives/harfbuzz/2013-August/003490.html */

    /* We really want to find a 'vert' feature if there's any in the font, no
     * matter which script/langsys it is listed (or not) under.
     * See various bugs referenced from:
     * https://github.com/harfbuzz/harfbuzz/issues/63 */
    map->enable_feature (HB_نښان ('v','e','r','t'), F_GLOBAL_پلټنه);
  }

  إذا (عدد_user_features)
    map->is_simple = خطأ;
  لكل (بینښه صحيح i = 0; i < عدد_user_features; i++)
  {
    ثابت hb_feature_t *feature = &user_features[i];
    map->جمع_کړه_feature (feature->نښان_2,
		      (feature->پیل == HB_FEATURE_GLOBAL_پیل &&
		       feature->پای_ټکی == HB_FEATURE_GLOBAL_پای_ټکی) ?  F_GLOBAL : F_NONE,
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
  OT::hb_scalar_cache_t ناکارول_شوی_2; // Just for alignment
};

hb_ot_font_معلومات_t *
hb_ot_shaper_font_معلومات_create (hb_font_t *font)
{
  ثابت OT::توکی_Variation_Store &var_store = font->face->جدول.GDEF->جدول->get_var_store ();
  أرجع (hb_ot_font_معلومات_t *) var_store.create_cache ();
}

فراغ
hb_ot_shaper_font_معلومات_destroy (hb_ot_font_معلومات_t *معلومات)
{
  OT::توکی_Variation_Store::destroy_cache ((OT::hb_scalar_cache_t *) معلومات);
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
  بینښه صحيح        عدد_user_features;

  /* Transient stuff */
  hb_direction_t موخه_direction;
};



/* Main shaper */


/* Prepare */

ساكن فراغ
hb_سېټ_unicode_props (hb_بفر_t *بفر,
		      hb_ماسک_t    global_ماسک)
{
  /* Implement enough of Unicode Graphemes here that shaping
   * in reverse-direction wouldn't break graphemes.  Namely,
   * we mark all marks and ZWJ and ZWJ,Extended_Pictographic
   * sequences as continuations.  The foreach_grapheme()
   * macro uses this bit.
   *
   * https://www.unicode.org/reports/tr29/#Regex_Definitions
   */
  بینښه صحيح شمېر = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  لكل (بینښه صحيح i = 0; i < شمېر; i++)
  {
    info[i].ماسک = global_ماسک;
    hb_glyph_info_سېټ_unicode_props (&info[i], بفر);

    إذا (info[i].codepoint < 0x80)
      استمر;

    بینښه gen_cat = _hb_glyph_info_get_general_category (&info[i]);
    إذا (FLAG_UNSAFE (gen_cat) &
	(FLAG (HB_UNICODE_GENERAL_CATEGORY_LOWERCASE_LETTER) |
	 FLAG (HB_UNICODE_GENERAL_CATEGORY_UPPERCASE_LETTER) |
	 FLAG (HB_UNICODE_GENERAL_CATEGORY_TITLECASE_LETTER) |
	 FLAG (HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER) |
	 FLAG (HB_UNICODE_GENERAL_CATEGORY_SPACE_SEPARATOR)))
      استمر;

    /* Marks are already set as continuation by the above line.
     * Handle Emoji_Modifier and ZWJ-continuation. */
    إذا (unlikely (gen_cat == HB_UNICODE_GENERAL_CATEGORY_MODIFIER_نښه &&
		  hb_دننه_range<hb_codepoint_t> (info[i].codepoint, 0x1F3FBu, 0x1F3FFu)))
    {
      hb_glyph_info_سېټ_continuation (&info[i], بفر);
    }
    /* Regional_Indicators are hairy as hell...
     * https://github.com/harfbuzz/harfbuzz/issues/2265 */
    وإلا إذا (unlikely (i && _hb_codepoint_is_regional_indicator (info[i].codepoint)))
    {
      إذا (_hb_codepoint_is_regional_indicator (info[i - 1].codepoint) &&
	  !_hb_glyph_info_is_continuation (&info[i - 1]))
	hb_glyph_info_سېټ_continuation (&info[i], بفر);
    }
#که_نه_تعریف_شوی HB_NO_EMOJI_SEQUENCES
    وإلا إذا (unlikely (_hb_glyph_info_is_zwj (&info[i])))
    {
      hb_glyph_info_سېټ_continuation (&info[i], بفر);
      إذا (i + 1 < شمېر &&
	  _hb_unicode_is_emoji_Extended_Pictographic (info[i + 1].codepoint))
      {
	i++;
	info[i].ماسک = global_ماسک;
	hb_glyph_info_سېټ_unicode_props (&info[i], بفر);
	hb_glyph_info_سېټ_continuation (&info[i], بفر);
      }
    }
#د_که_پای
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
    وإلا إذا (unlikely (hb_دننه_ranges<hb_codepoint_t> (info[i].codepoint, 0xFF9Eu, 0xFF9Fu, 0xE0020u, 0xE007Fu)))
      hb_glyph_info_سېټ_continuation (&info[i], بفر);
    وإلا إذا (unlikely (info[i].codepoint == 0x2044u /* FRACTION SLASH */))
      بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_HAS_FRACTION_SLASH;
  }
}

ساكن فراغ
hb_ورننباسه_dotted_circle (hb_بفر_t *بفر, hb_font_t *font)
{
  إذا (unlikely (بفر->flags & HB_بفر_FLAG_DO_NOT_ورننباسه_DOTTED_CIRCLE))
    أرجع;

  إذا (!(بفر->flags & HB_بفر_FLAG_BOT) ||
      بفر->context_len[0] ||
      !_hb_glyph_info_is_unicode_mark (&بفر->info[0]))
    أرجع;

  إذا (!font->has_glyph (0x25CCu))
    أرجع;

  hb_glyph_info_t dottedcircle = {0};
  dottedcircle.codepoint = 0x25CCu;
  hb_glyph_info_سېټ_unicode_props (&dottedcircle, بفر);

  بفر->clear_output ();

  بفر->idx = 0;
  hb_glyph_info_t info = dottedcircle;
  info.cluster = بفر->cur().cluster;
  info.ماسک = بفر->cur().ماسک;
  (فراغ) بفر->output_info (info);

  بفر->متزامن ();
}

ساكن فراغ
hb_form_clusters (hb_بفر_t *بفر)
{
  إذا (!(بفر->scratch_flags & HB_بفر_SCRATCH_FLAG_HAS_CONTINUATIONS))
    أرجع;

  foreach_grapheme (بفر, پیل, پای_ټکی)
    بفر->یوځای_grapheme_clusters (پیل, پای_ټکی);
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
    منطقي وموندل_شمېره = خطأ, وموندل_letter = خطأ, وموندل_ri = خطأ;
    ثابت خپلکاری* info = بفر->info;
    ثابت خپلکاری شمېر = بفر->len;
    لكل (بینښه i = 0; i < شمېر; i++)
    {
      خپلکاری gc = _hb_glyph_info_get_general_category (&info[i]);
      إذا (HB_UNICODE_GENERAL_CATEGORY_IS_LETTER (gc))
      {
	وموندل_letter = ریښتیا;
	اكسر;
      }
      وإلا إذا (gc == HB_UNICODE_GENERAL_CATEGORY_DECIMAL_شمېره)
	وموندل_شمېره = ریښتیا;
      وإلا إذا (unlikely (_hb_codepoint_is_regional_indicator (info[i].codepoint)))
	وموندل_ri = ریښتیا;
    }
    إذا ((وموندل_شمېره || وموندل_ri) && !وموندل_letter)
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
    hb_ot_layout_سرچپه_graphemes (بفر);
    بفر->props.direction = HB_DIRECTION_سرچپه (بفر->props.direction);
  }
}


/*
 * Substitute
 */

ساكن مضمن فراغ
hb_ot_rotate_chars (ثابت hb_ot_shape_context_t *c)
{
  hb_بفر_t *بفر = c->بفر;
  بینښه صحيح شمېر = بفر->len;
  hb_glyph_info_t *info = بفر->info;

  إذا (HB_DIRECTION_IS_BACKWARD (c->موخه_direction))
  {
    hb_unicode_funcs_t *unicode = بفر->unicode;
    hb_ماسک_t rtlm_ماسک = c->plan->rtlm_ماسک;

    لكل (بینښه صحيح i = 0; i < شمېر; i++) {
      hb_codepoint_t codepoint = unicode->mirroring (info[i].codepoint);
      إذا (unlikely (codepoint != info[i].codepoint && c->font->has_glyph (codepoint)))
	info[i].codepoint = codepoint;
      وإلا
	info[i].ماسک |= rtlm_ماسک;
    }
  }

#که_نه_تعریف_شوی HB_NO_VERTICAL
  إذا (HB_DIRECTION_IS_VERTICAL (c->موخه_direction) && !c->plan->has_vert)
  {
    لكل (بینښه صحيح i = 0; i < شمېر; i++) {
      hb_codepoint_t codepoint = hb_unicode_funcs_t::vertical_char_for (info[i].codepoint);
      إذا (unlikely (codepoint != info[i].codepoint && c->font->has_glyph (codepoint)))
	info[i].codepoint = codepoint;
    }
  }
#د_که_پای
}

ساكن مضمن فراغ
hb_ot_shape_تیاری_masks_fraction (ثابت hb_ot_shape_context_t *c)
{
#که_تعریف_شوی HB_NO_OT_SHAPE_FRACTIONS
  أرجع;
#د_که_پای

  إذا (!(c->بفر->scratch_flags & HB_بفر_SCRATCH_FLAG_HAS_FRACTION_SLASH) ||
      !c->plan->has_frac)
    أرجع;

  hb_بفر_t *بفر = c->بفر;

  hb_ماسک_t pre_ماسک, post_ماسک;
  إذا (HB_DIRECTION_IS_FORWARD (بفر->props.direction))
  {
    pre_ماسک = c->plan->numr_ماسک | c->plan->frac_ماسک;
    post_ماسک = c->plan->frac_ماسک | c->plan->dnom_ماسک;
  }
  وإلا
  {
    pre_ماسک = c->plan->frac_ماسک | c->plan->dnom_ماسک;
    post_ماسک = c->plan->numr_ماسک | c->plan->frac_ماسک;
  }

  بینښه صحيح شمېر = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  لكل (بینښه صحيح i = 0; i < شمېر; i++)
  {
    إذا (info[i].codepoint == 0x2044u) /* FRACTION SLASH */
    {
      بینښه صحيح پیل = i, پای_ټکی = i + 1;
      طالما (پیل &&
	     _hb_glyph_info_get_general_category (&info[پیل - 1]) ==
	     HB_UNICODE_GENERAL_CATEGORY_DECIMAL_شمېره)
	پیل--;
      طالما (پای_ټکی < شمېر &&
	     _hb_glyph_info_get_general_category (&info[پای_ټکی]) ==
	     HB_UNICODE_GENERAL_CATEGORY_DECIMAL_شمېره)
	پای_ټکی++;
      إذا (پیل == i || پای_ټکی == i + 1)
      {
        إذا (پیل == i)
	  بفر->unsafe_منزل_concat (پیل, پیل + 1);
	إذا (پای_ټکی == i + 1)
	  بفر->unsafe_منزل_concat (پای_ټکی - 1, پای_ټکی);
	استمر;
      }

      بفر->unsafe_منزل_break (پیل, پای_ټکی);

      لكل (بینښه صحيح j = پیل; j < i; j++)
	info[j].ماسک |= pre_ماسک;
      info[i].ماسک |= c->plan->frac_ماسک;
      لكل (بینښه صحيح j = i + 1; j < پای_ټکی; j++)
	info[j].ماسک |= post_ماسک;

      i = پای_ټکی - 1;
    }
  }
}

ساكن مضمن فراغ
hb_ot_shape_تیاری_masks (ثابت hb_ot_shape_context_t *c)
{
  hb_ot_map_t *map = &c->plan->map;
  hb_بفر_t *بفر = c->بفر;

  hb_ot_shape_تیاری_masks_fraction (c);

  إذا (c->plan->shaper->تیاری_masks)
    c->plan->shaper->تیاری_masks (c->plan, بفر, c->font);

  لكل (بینښه صحيح i = 0; i < c->عدد_user_features; i++)
  {
    ثابت hb_feature_t *feature = &c->user_features[i];
    إذا (!(feature->پیل == HB_FEATURE_GLOBAL_پیل && feature->پای_ټکی == HB_FEATURE_GLOBAL_پای_ټکی)) {
      بینښه صحيح ښويدنه;
      hb_ماسک_t ماسک = map->get_ماسک (feature->نښان_2, &ښويدنه);
      بفر->سېټ_masks (feature->value << ښويدنه, ماسک, feature->پیل, feature->پای_ټکی);
    }
  }
}

ساكن فراغ
hb_ot_zero_پلنوالی_default_ignorables (ثابت hb_بفر_t *بفر)
{
  إذا (!(بفر->scratch_flags & HB_بفر_SCRATCH_FLAG_HAS_DEFAULT_IGNORABLES) ||
      (بفر->flags & HB_بفر_FLAG_PRESERVE_DEFAULT_IGNORABLES) ||
      (بفر->flags & HB_بفر_FLAG_REMOVE_DEFAULT_IGNORABLES))
    أرجع;

  بینښه صحيح شمېر = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  hb_glyph_position_t *pos = بفر->pos;
  بینښه صحيح i = 0;
  لكل (i = 0; i < شمېر; i++)
    إذا (unlikely (_hb_glyph_info_is_default_ignorable (&info[i])))
    {
      pos[i].x_advance = pos[i].y_advance = 0;
      إذا (HB_DIRECTION_IS_HORIZONTAL (بفر->props.direction))
	pos[i].x_دننه_واټن_2 = 0;
      وإلا
        pos[i].y_دننه_واټن_2 = 0;
    }
}

ساكن فراغ
hb_ot_deal_with_variation_selectors (hb_بفر_t *بفر)
{
  إذا (!(بفر->scratch_flags & HB_بفر_SCRATCH_FLAG_HAS_VARIATION_SELECTOR_FALLBACK) ||
	بفر->not_وموندل_variation_selector == HB_CODEPOINT_INVALID)
    أرجع;

  بینښه صحيح شمېر = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  hb_glyph_position_t *pos = بفر->pos;

  لكل (بینښه صحيح i = 0; i < شمېر; i++)
  {
    إذا (_hb_glyph_info_is_variation_selector (&info[i]))
    {
      info[i].codepoint = بفر->not_وموندل_variation_selector;
      pos[i].x_advance = pos[i].y_advance = pos[i].x_دننه_واټن_2 = pos[i].y_دننه_واټن_2 = 0;
      hb_glyph_info_سېټ_variation_selector (&info[i], خطأ);
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

  بینښه صحيح شمېر = بفر->len;
  hb_glyph_info_t *info = بفر->info;

  hb_codepoint_t invisible = بفر->invisible;
  إذا (!(بفر->flags & HB_بفر_FLAG_REMOVE_DEFAULT_IGNORABLES) &&
      (invisible || font->get_nominal_glyph (' ', &invisible)))
  {
    /* Replace default-ignorables with a zero-advance invisible glyph. */
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
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
  بینښه صحيح شمېر = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  لكل (بینښه صحيح i = 0; i < شمېر; i++)
    info[i].codepoint = info[i].normalizer_glyph_شاخص();

  بفر->content_type = HB_بفر_CONTENT_TYPE_GLYPHS;
}

ساكن مضمن فراغ
hb_synthesize_glyph_classes (hb_بفر_t *بفر)
{
  بینښه صحيح شمېر = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  لكل (بینښه صحيح i = 0; i < شمېر; i++)
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
	    HB_OT_LAYOUT_GLYPH_PROPS_بنسټ_GLYPH :
	    HB_OT_LAYOUT_GLYPH_PROPS_MARK;
    hb_glyph_info_سېټ_glyph_props (&info[i], klass);
  }
}

ساكن مضمن فراغ
hb_ot_substitute_default (ثابت hb_ot_shape_context_t *c)
{
  hb_بفر_t *بفر = c->بفر;

  hb_ot_rotate_chars (c);

  HB_بفر_ALLOCATE_VAR (بفر, normalizer_glyph_شاخص);

  _hb_ot_shape_normalize (c->plan, بفر, c->font);

  hb_ot_shape_تیاری_masks (c);

  /* This is unfortunate to go here, but necessary... */
  إذا (c->plan->fallback_mark_positioning)
    _hb_ot_shape_fallback_mark_position_recategorize_marks (c->plan, c->font, بفر);

  hb_ot_map_glyphs_fast (بفر);

  HB_بفر_DEALLOCATE_VAR (بفر, normalizer_glyph_شاخص);
}

ساكن مضمن فراغ
hb_ot_substitute_plan (ثابت hb_ot_shape_context_t *c)
{
  hb_بفر_t *بفر = c->بفر;

#که_نه_تعریف_شوی HB_NO_AAT_SHAPE
  إذا (unlikely (c->plan->apply_morx))
  {
    hb_ot_layout_substitute_پیل (c->font, بفر);

    إذا (c->plan->fallback_glyph_classes)
      hb_synthesize_glyph_classes (بفر);

    hb_aat_layout_substitute (c->plan, c->font, c->بفر,
			      c->user_features, c->عدد_user_features);
    /* The buffer digest is only used by the OT lookup-apply loop;
     * without GPOS ahead, nothing consumes it. */
    إذا (c->plan->apply_gpos)
      c->بفر->update_digest ();
  }
  وإلا
#د_که_پای
  {
    hb_ot_layout_substitute_پیل_with_digest (c->font, بفر);

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

#که_نه_تعریف_شوی HB_NO_AAT_SHAPE
  إذا (c->plan->apply_morx && c->plan->apply_gpos)
    hb_aat_layout_remove_deleted_glyphs (c->بفر);
#د_که_پای
}

ساكن مضمن فراغ
hb_ot_substitute_post (ثابت hb_ot_shape_context_t *c)
{
#که_نه_تعریف_شوی HB_NO_AAT_SHAPE
  إذا (c->plan->apply_morx && !c->plan->apply_gpos)
    hb_aat_layout_remove_deleted_glyphs (c->بفر);
#د_که_پای

  hb_ot_deal_with_variation_selectors (c->بفر);
  hb_ot_hide_default_ignorables (c->بفر, c->font);

  إذا (c->plan->shaper->postprocess_glyphs &&
    c->بفر->پیغام(c->font, "start postprocess-glyphs")) {
    c->plan->shaper->postprocess_glyphs (c->plan, c->بفر, c->font);
    (فراغ) c->بفر->پیغام(c->font, "end postprocess-glyphs");
  }
}


/*
 * Position
 */

ساكن مضمن فراغ
adjust_mark_offsets (hb_glyph_position_t *pos)
{
  pos->x_دننه_واټن_2 = hb_saturate_sub (pos->x_دننه_واټن_2, pos->x_advance);
  pos->y_دننه_واټن_2 = hb_saturate_sub (pos->y_دننه_واټن_2, pos->y_advance);
}

ساكن مضمن فراغ
zero_mark_پلنوالی (hb_glyph_position_t *pos)
{
  pos->x_advance = 0;
  pos->y_advance = 0;
}

ساكن مضمن فراغ
zero_mark_widths_by_gdef (hb_بفر_t *بفر, منطقي adjust_offsets)
{
  بینښه صحيح شمېر = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  لكل (بینښه صحيح i = 0; i < شمېر; i++)
    إذا (_hb_glyph_info_is_mark (&info[i]))
    {
      إذا (adjust_offsets)
	adjust_mark_offsets (&بفر->pos[i]);
      zero_mark_پلنوالی (&بفر->pos[i]);
    }
}

ساكن مضمن فراغ
hb_ot_position_default (ثابت hb_ot_shape_context_t *c)
{
  hb_direction_t direction = c->بفر->props.direction;
  بینښه صحيح شمېر = c->بفر->len;
  hb_glyph_info_t *info = c->بفر->info;
  hb_glyph_position_t *pos = c->بفر->pos;

  إذا (HB_DIRECTION_IS_HORIZONTAL (direction))
  {
    c->font->get_glyph_h_advances (شمېر, &info[0].codepoint, حجم(info[0]),
				   &pos[0].x_advance, حجم(pos[0]));
    // h_origin defaults to zero; only apply it if the font has it.
    إذا (c->font->has_glyph_h_origin_func () || c->font->has_glyph_h_origins_func ())
      c->font->کمول_glyph_h_origins (c->بفر);
  }
  وإلا
  {
    c->font->get_glyph_v_advances (شمېر, &info[0].codepoint, حجم(info[0]),
				   &pos[0].y_advance, حجم(pos[0]));
    // v_origin defaults to non-zero; apply even if only fallback is there.
    c->font->کمول_glyph_v_origins (c->بفر);
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
    c->font->جمع_کړه_glyph_h_origins (c->بفر);

  hb_ot_layout_position_پیل (c->font, c->بفر);

  إذا (c->plan->zero_marks)
    بدل (c->plan->shaper->zero_پلنوالی_marks)
    {
      حالة HB_OT_SHAPE_ZERO_پلنوالی_MARKS_BY_GDEF_EARLY:
	zero_mark_widths_by_gdef (c->بفر, adjust_offsets_when_zeroing);
	اكسر;

      افتراضي:
      حالة HB_OT_SHAPE_ZERO_پلنوالی_MARKS_NONE:
      حالة HB_OT_SHAPE_ZERO_پلنوالی_MARKS_BY_GDEF_LATE:
	اكسر;
    }

  c->plan->position (c->font, c->بفر);

  إذا (c->plan->zero_marks)
    بدل (c->plan->shaper->zero_پلنوالی_marks)
    {
      حالة HB_OT_SHAPE_ZERO_پلنوالی_MARKS_BY_GDEF_LATE:
	zero_mark_widths_by_gdef (c->بفر, adjust_offsets_when_zeroing);
	اكسر;

      افتراضي:
      حالة HB_OT_SHAPE_ZERO_پلنوالی_MARKS_NONE:
      حالة HB_OT_SHAPE_ZERO_پلنوالی_MARKS_BY_GDEF_EARLY:
	اكسر;
    }

  /* Finish off.  Has to follow a certain order. */
  hb_ot_layout_position_پای_advances (c->font, c->بفر);
  hb_ot_zero_پلنوالی_default_ignorables (c->بفر);
  hb_ot_layout_position_پای_offsets (c->font, c->بفر);

  // h_origin defaults to zero; only apply it if the font has it.
  إذا (c->font->has_glyph_h_origin_func () || c->font->has_glyph_h_origins_func ())
    c->font->کمول_glyph_h_origins (c->بفر);

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
    hb_بفر_سرچپه (c->بفر);

  hb_بفر_deallocate_gsubgpos_vars (c->بفر);
}

ساكن مضمن فراغ
hb_propagate_flags (hb_بفر_t *بفر)
{
  /* Propagate cluster-level glyph flags to be the same on all cluster glyphs.
   * Simplifies using them. */

  hb_ماسک_t and_ماسک = HB_GLYPH_FLAG_DEFINED;
  إذا ((بفر->flags & HB_بفر_FLAG_PRODUCE_UNSAFE_منزل_CONCAT) == 0)
    and_ماسک &= ~HB_GLYPH_FLAG_UNSAFE_منزل_CONCAT;

  hb_glyph_info_t *info = بفر->info;

  إذا ((بفر->flags & HB_بفر_FLAG_PRODUCE_خوندي_منزل_ورننباسه_TATWEEL) == 0)
  {
    foreach_cluster (بفر, پیل, پای_ټکی)
    {
      إذا (پای_ټکی - پیل == 1)
      {
        info[پیل].ماسک &= and_ماسک;
	استمر;
      }

      بینښه صحيح ماسک = 0;
      لكل (بینښه صحيح i = پیل; i < پای_ټکی; i++)
	ماسک |= info[i].ماسک;

      ماسک &= and_ماسک;

      لكل (بینښه صحيح i = پیل; i < پای_ټکی; i++)
	info[i].ماسک = ماسک;
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
  foreach_cluster (بفر, پیل, پای_ټکی)
  {
    بینښه صحيح ماسک = 0;
    لكل (بینښه صحيح i = پیل; i < پای_ټکی; i++)
      ماسک |= info[i].ماسک;

    إذا (ماسک & HB_GLYPH_FLAG_UNSAFE_منزل_BREAK)
      ماسک &= ~HB_GLYPH_FLAG_خوندي_منزل_ورننباسه_TATWEEL;
    إذا (ماسک & HB_GLYPH_FLAG_خوندي_منزل_ورننباسه_TATWEEL)
      ماسک |= HB_GLYPH_FLAG_UNSAFE_منزل_BREAK | HB_GLYPH_FLAG_UNSAFE_منزل_CONCAT;

    ماسک &= and_ماسک;

    لكل (بینښه صحيح i = پیل; i < پای_ټکی; i++)
      info[i].ماسک = ماسک;
  }
}

/* Pull it all together! */

ساكن فراغ
hb_ot_shape_دننی (hb_ot_shape_context_t *c)
{
  /* Save the original direction, we use it later. */
  c->موخه_direction = c->بفر->props.direction;

  hb_بفر_allocate_unicode_vars (c->بفر);

  hb_سېټ_unicode_props (c->بفر, c->plan->map.get_global_ماسک ());
  hb_ورننباسه_dotted_circle (c->بفر, c->font);

  hb_form_clusters (c->بفر);

  hb_ensure_native_direction (c->بفر);

  إذا (c->plan->shaper->preprocess_متن &&
      c->بفر->پیغام(c->font, "start preprocess-text"))
  {
    c->plan->shaper->preprocess_متن (c->plan, c->بفر, c->font);
    (فراغ) c->بفر->پیغام(c->font, "end preprocess-text");
  }

  hb_ot_substitute_pre (c);
  hb_ot_position (c);
  hb_ot_substitute_post (c);

  hb_propagate_flags (c->بفر);

  hb_بفر_deallocate_unicode_vars (c->بفر);

  c->بفر->props.direction = c->موخه_direction;
}


hb_bool_t
_hb_ot_shape (hb_shape_plan_t    *shape_plan,
	      hb_font_t          *font,
	      hb_بفر_t        *بفر,
	      ثابت hb_feature_t *features,
	      بینښه صحيح        عدد_features)
{
  hb_ot_shape_context_t c = {&shape_plan->ot, font, font->face, بفر, features, عدد_features};
  hb_ot_shape_دننی (&c);

  أرجع ریښتیا;
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
				  hb_نښان_t         جدول_نښان_2,
				  hb_سېټ_t        *lookup_indexes /*بهر*/)
{
  shape_plan->ot.collect_lookups (جدول_نښان_2, lookup_indexes);
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
بینښه صحيح
hb_ot_shape_plan_get_feature_نښانونه (hb_shape_plan_t *shape_plan,
				   بینښه صحيح     پیل_دننه_واټن,
				   بینښه صحيح    *نښان_شمېر, /* IN/OUT */
				   hb_نښان_t        *نښانونه /*بهر*/)
{
#که_نه_تعریف_شوی HB_NO_OT_SHAPE
  أرجع shape_plan->ot.map.get_feature_نښانونه (پیل_دننه_واټن, نښان_شمېر, نښانونه);
#وإلا
  إذا (نښان_شمېر)
	*نښان_شمېر = 0;
  أرجع 0;
#د_که_پای
}


/* TODO Move this to hb-ot-shape-normalize, make it do decompose, and make it public. */
ساكن فراغ
جمع_کړه_char (hb_font_t          *font,
	  hb_unicode_funcs_t *unicode,
	  hb_bool_t           mirror,
	  hb_codepoint_t      u,
	  hb_سېټ_t           *glyphs)
{
  hb_codepoint_t glyph;
  إذا (font->get_nominal_glyph (u, &glyph))
    glyphs->جمع_کړه (glyph);
  إذا (mirror)
  {
    hb_codepoint_t m = unicode->mirroring (u);
    إذا (m != u && font->get_nominal_glyph (m, &glyph))
      glyphs->جمع_کړه (glyph);
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
hb_ot_shape_glyphs_تړون (hb_font_t          *font,
			    hb_بفر_t        *بفر,
			    ثابت hb_feature_t *features,
			    بینښه صحيح        عدد_features,
			    hb_سېټ_t           *glyphs)
{
  ثابت حرف *shapers[] = {"ot", تش_اشاره};
  hb_shape_plan_t *shape_plan = hb_shape_plan_create_cached (font->face, &بفر->props,
							     features, عدد_features, shapers);

  منطقي mirror = hb_script_get_horizontal_direction (بفر->props.script) == HB_DIRECTION_RTL;

  بینښه صحيح شمېر = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  لكل (بینښه صحيح i = 0; i < شمېر; i++)
    جمع_کړه_char (font, بفر->unicode, mirror, info[i].codepoint, glyphs);

  hb_سېټ_t *lookups = hb_سېټ_create ();
  hb_ot_shape_plan_collect_lookups (shape_plan, HB_OT_نښان_GSUB, lookups);
  hb_ot_layout_lookups_substitute_تړون (font->face, lookups, glyphs);

  hb_سېټ_destroy (lookups);

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
بینښه صحيح
hb_ot_shape_get_بفر_format_serial (فراغ)
{
  أرجع HB_OT_SHAPE_بفر_FORMAT_SERIAL;
}

#د_که_پای

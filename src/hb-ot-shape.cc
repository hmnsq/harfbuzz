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

#اگر_تعریف_نشده HB_NO_OT_SHAPE

#اگر_تعریف_شده HB_NO_OT_LAYOUT
#خطا "Cannot compile 'ot' shaper with HB_NO_OT_LAYOUT."
#پایان_اگر

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
{ أرجع hb_درون_range<hb_codepoint_t> (u, 0x1F1E6u, 0x1F1FFu); }

#اگر_تعریف_نشده HB_NO_AAT_SHAPE
ساكن مضمن منطقي
_hb_apply_morx (hb_face_t *face, ثابت hb_segment_properties_t &props)
{
  /* https://github.com/harfbuzz/harfbuzz/issues/2124 */
  أرجع hb_aat_layout_has_substitution (face) &&
	 (HB_DIRECTION_IS_HORIZONTAL (props.direction) || !hb_ot_layout_has_substitution (face));
}
#پایان_اگر

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
			      بیعلامت صحيح                    رقم_user_features);

hb_ot_shape_planner_t::hb_ot_shape_planner_t (hb_face_t                     *face,
					      ثابت hb_segment_properties_t &props) :
						face (face),
						props (props),
						map (face, props)
#اگر_تعریف_نشده HB_NO_AAT_SHAPE
						, aat_map (face, props)
						, apply_morx (_hb_apply_morx (face, props))
#پایان_اگر
{
  shaper = hb_ot_shaper_categorize (props.script, props.direction, map.chosen_script[0]);

  script_zero_marks = shaper->zero_عرض_marks != HB_OT_SHAPE_ZERO_عرض_MARKS_NONE;
  script_fallback_position = shaper->fallback_position;

#اگر_تعریف_نشده HB_NO_AAT_SHAPE
  /* https://github.com/harfbuzz/harfbuzz/issues/1528 */
  إذا (apply_morx && shaper != &_hb_ot_shaper_default)
    shaper = &_hb_ot_shaper_dumber;
#پایان_اگر
}

فراغ
hb_ot_shape_planner_t::compile (hb_ot_shape_plan_t           &plan,
				ثابت hb_ot_shape_plan_کلید_t &key)
{
  plan.props = props;
  plan.shaper = shaper;
  map.compile (plan.map, key);
#اگر_تعریف_نشده HB_NO_AAT_SHAPE
  إذا (apply_morx)
    aat_map.compile (plan.aat_map);
#پایان_اگر

#اگر_تعریف_نشده HB_NO_OT_SHAPE_FRACTIONS
  plan.frac_ماسک = plan.map.get_1_ماسک (HB_برچسب ('f','r','a','c'));
  plan.numr_ماسک = plan.map.get_1_ماسک (HB_برچسب ('n','u','m','r'));
  plan.dnom_ماسک = plan.map.get_1_ماسک (HB_برچسب ('d','n','o','m'));
  plan.has_frac = plan.frac_ماسک || (plan.numr_ماسک && plan.dnom_ماسک);
#پایان_اگر

  plan.rtlm_ماسک = plan.map.get_1_ماسک (HB_برچسب ('r','t','l','m'));
  plan.has_vert = !!plan.map.get_1_ماسک (HB_برچسب ('v','e','r','t'));

  hb_برچسب_t kern_برچسب = HB_DIRECTION_IS_HORIZONTAL (props.direction) ?
		      HB_برچسب ('k','e','r','n') : HB_برچسب ('v','k','r','n');
#اگر_تعریف_نشده HB_NO_OT_KERN
  plan.kern_ماسک = plan.map.get_ماسک (kern_برچسب);
  plan.requested_kerning = !!plan.kern_ماسک;
#پایان_اگر

  منطقي has_gpos_kern = plan.map.get_feature_شاخص (1, kern_برچسب) != HB_OT_LAYOUT_NO_FEATURE_شاخص;
  منطقي disable_gpos = plan.shaper->gpos_برچسب &&
		      plan.shaper->gpos_برچسب != plan.map.chosen_script[1];

  /*
   * Decide who provides glyph classes. GDEF or Unicode.
   */

  إذا (!hb_ot_layout_has_glyph_classes (face))
    plan.fallback_glyph_classes = درست;

  /*
   * Decide who does substitutions. GSUB, morx, or fallback.
   */

#اگر_تعریف_نشده HB_NO_AAT_SHAPE
  plan.apply_morx = apply_morx;
#پایان_اگر

  /*
   * Decide who does positioning. GPOS, kerx, kern, or fallback.
   */

#اگر_تعریف_نشده HB_NO_AAT_SHAPE
  منطقي has_kerx = hb_aat_layout_has_positioning (face);
  منطقي has_gsub = !apply_morx && hb_ot_layout_has_substitution (face);
#پایان_اگر
  منطقي has_gpos = !disable_gpos && hb_ot_layout_has_positioning (face);
  إذا (خطأ)
    {}
#اگر_تعریف_نشده HB_NO_AAT_SHAPE
  /* Prefer GPOS over kerx if GSUB is present;
   * https://github.com/harfbuzz/harfbuzz/issues/3008 */
  وإلا إذا (has_kerx && !(has_gsub && has_gpos))
    plan.apply_kerx = درست;
#پایان_اگر
  وإلا إذا (has_gpos)
    plan.apply_gpos = درست;

  إذا (!plan.apply_kerx && (!has_gpos_kern || !plan.apply_gpos))
  {
    إذا (خطأ) {}
#اگر_تعریف_نشده HB_NO_AAT_SHAPE
    وإلا إذا (has_kerx)
      plan.apply_kerx = درست;
#پایان_اگر
#اگر_تعریف_نشده HB_NO_OT_KERN
    وإلا إذا (hb_ot_layout_has_kerning (face))
      plan.apply_kern = script_fallback_position; // Not all shapers apply legacy `kern`
#پایان_اگر
    وإلا {}
  }

  plan.apply_fallback_kern = script_fallback_position && !(plan.apply_gpos || plan.apply_kerx || plan.apply_kern);

  plan.zero_marks = script_zero_marks &&
		    !plan.apply_kerx &&
		    (!plan.apply_kern
#اگر_تعریف_نشده HB_NO_OT_KERN
		     || !hb_ot_layout_has_machine_kerning (face)
#پایان_اگر
		    );
  plan.has_gpos_mark = !!plan.map.get_1_ماسک (HB_برچسب ('m','a','r','k'));

  plan.adjust_mark_positioning_when_zeroing = !plan.apply_gpos &&
					      !plan.apply_kerx &&
					      (!plan.apply_kern
#اگر_تعریف_نشده HB_NO_OT_KERN
					       || !hb_ot_layout_has_cross_kerning (face)
#پایان_اگر
					      );

  plan.fallback_mark_positioning = plan.adjust_mark_positioning_when_zeroing &&
				   script_fallback_position;

#اگر_تعریف_نشده HB_NO_AAT_SHAPE
  /* If we're using morx shaping, we cancel mark position adjustment because
     Apple Color Emoji assumes this will NOT be done when forming emoji sequences;
     https://github.com/harfbuzz/harfbuzz/issues/2967. */
  إذا (plan.apply_morx)
    plan.adjust_mark_positioning_when_zeroing = خطأ;

  /* According to Ned, trak is applied by default for "modern fonts", as detected by presence of STAT table. */
#اگر_تعریف_نشده HB_NO_STYLE
  plan.apply_trak = hb_aat_layout_has_tracking (face) && face->جدول.STAT->has_داده ();
#وإلا
  plan.apply_trak = خطأ;
#پایان_اگر

#پایان_اگر
}

منطقي
hb_ot_shape_plan_t::init0 (hb_face_t                     *face,
			   ثابت hb_shape_plan_کلید_t     *key)
{
  map.init ();

  hb_ot_shape_planner_t planner (face,
				 key->props);

  hb_ot_shape_collect_features (&planner,
				key->user_features,
				key->رقم_user_features);

  planner.compile (*هذا, key->ot);

  إذا (shaper->داده_create)
  {
    داده = shaper->داده_create (هذا);
    إذا (unlikely (!داده))
    {
      map.fini ();
      أرجع خطأ;
    }
  }

  أرجع درست;
}

فراغ
hb_ot_shape_plan_t::fini ()
{
  إذا (shaper->داده_destroy)
    shaper->داده_destroy (const_cast<فراغ *> (داده));

  map.fini ();
}

فراغ
hb_ot_shape_plan_t::substitute (hb_font_t   *font,
				hb_میانگیر_t *میانگیر) ثابت
{
  map.substitute (هذا, font, میانگیر);
}

فراغ
hb_ot_shape_plan_t::position (hb_font_t   *font,
			      hb_میانگیر_t *میانگیر) ثابت
{
  إذا (هذا->apply_gpos)
    map.position (هذا, font, میانگیر);
#اگر_تعریف_نشده HB_NO_AAT_SHAPE
  وإلا إذا (هذا->apply_kerx)
    hb_aat_layout_position (هذا, font, میانگیر);
#پایان_اگر

#اگر_تعریف_نشده HB_NO_OT_KERN
  إذا (هذا->apply_kern)
    hb_ot_layout_kern (هذا, font, میانگیر);
#پایان_اگر
  وإلا إذا (هذا->apply_fallback_kern)
    _hb_ot_shape_fallback_kern (هذا, font, میانگیر);

#اگر_تعریف_نشده HB_NO_AAT_SHAPE
  إذا (هذا->apply_trak)
    hb_aat_layout_track (هذا, font, میانگیر);
#پایان_اگر
}


ساكن ثابت hb_ot_map_feature_t
common_features[] =
{
  {HB_برچسب('a','b','v','m'), F_GLOBAL},
  {HB_برچسب('b','l','w','m'), F_GLOBAL},
  {HB_برچسب('c','c','m','p'), F_GLOBAL},
  {HB_برچسب('l','o','c','l'), F_GLOBAL},
  {HB_برچسب('m','a','r','k'), F_GLOBAL_MANUAL_JOINERS},
  {HB_برچسب('m','k','m','k'), F_GLOBAL_MANUAL_JOINERS},
  {HB_برچسب('r','l','i','g'), F_GLOBAL},
};


ساكن ثابت hb_ot_map_feature_t
horizontal_features[] =
{
  {HB_برچسب('c','a','l','t'), F_GLOBAL},
  {HB_برچسب('c','l','i','g'), F_GLOBAL},
  {HB_برچسب('c','u','r','s'), F_GLOBAL},
  {HB_برچسب('d','i','s','t'), F_GLOBAL},
  {HB_برچسب('k','e','r','n'), F_GLOBAL_HAS_FALLBACK},
  {HB_برچسب('l','i','g','a'), F_GLOBAL},
  {HB_برچسب('r','c','l','t'), F_GLOBAL},
};

ساكن فراغ
hb_ot_shape_collect_features (hb_ot_shape_planner_t *planner,
			      ثابت hb_feature_t    *user_features,
			      بیعلامت صحيح           رقم_user_features)
{
  hb_ot_map_builder_t *map = &planner->map;

  map->is_simple = درست;

  map->enable_feature (HB_برچسب('r','v','r','n'));
  map->جمع_کن_gsub_pause (اشاره_تهی);

  بدل (planner->props.direction)
  {
    حالة HB_DIRECTION_LTR:
      map->enable_feature (HB_برچسب ('l','t','r','a'));
      map->enable_feature (HB_برچسب ('l','t','r','m'));
      اكسر;
    حالة HB_DIRECTION_RTL:
      map->enable_feature (HB_برچسب ('r','t','l','a'));
      map->جمع_کن_feature (HB_برچسب ('r','t','l','m'));
      اكسر;
    حالة HB_DIRECTION_TTB:
    حالة HB_DIRECTION_BTT:
    حالة HB_DIRECTION_INVALID:
    افتراضي:
      اكسر;
  }

#اگر_تعریف_نشده HB_NO_OT_SHAPE_FRACTIONS
  /* Automatic fractions. */
  map->جمع_کن_feature (HB_برچسب ('f','r','a','c'));
  map->جمع_کن_feature (HB_برچسب ('n','u','m','r'));
  map->جمع_کن_feature (HB_برچسب ('d','n','o','m'));
#پایان_اگر

  /* Random! */
  map->enable_feature (HB_برچسب ('r','a','n','d'), F_RANDOM, HB_OT_MAP_MAX_مقدار);

  map->enable_feature (HB_برچسب ('H','a','r','f')); /* Considered required. */
  map->enable_feature (HB_برچسب ('H','A','R','F')); /* Considered discretionary. */

  إذا (planner->shaper->collect_features)
  {
    map->is_simple = خطأ;
    planner->shaper->collect_features (planner);
  }

  map->enable_feature (HB_برچسب ('B','u','z','z')); /* Considered required. */
  map->enable_feature (HB_برچسب ('B','U','Z','Z')); /* Considered discretionary. */

  لكل (بیعلامت صحيح i = 0; i < ARRAY_درازی (common_features); i++)
    map->جمع_کن_feature (common_features[i]);

  إذا (HB_DIRECTION_IS_HORIZONTAL (planner->props.direction))
    لكل (بیعلامت صحيح i = 0; i < ARRAY_درازی (horizontal_features); i++)
      map->جمع_کن_feature (horizontal_features[i]);
  وإلا
  {
    /* We only apply `vert` feature. See:
     * https://github.com/harfbuzz/harfbuzz/commit/d71c0df2d17f4590d5611239577a6cb532c26528
     * https://lists.freedesktop.org/archives/harfbuzz/2013-August/003490.html */

    /* We really want to find a 'vert' feature if there's any in the font, no
     * matter which script/langsys it is listed (or not) under.
     * See various bugs referenced from:
     * https://github.com/harfbuzz/harfbuzz/issues/63 */
    map->enable_feature (HB_برچسب ('v','e','r','t'), F_GLOBAL_جستجو);
  }

  إذا (رقم_user_features)
    map->is_simple = خطأ;
  لكل (بیعلامت صحيح i = 0; i < رقم_user_features; i++)
  {
    ثابت hb_feature_t *feature = &user_features[i];
    map->جمع_کن_feature (feature->برچسب_2,
		      (feature->شروع == HB_FEATURE_GLOBAL_شروع &&
		       feature->انتها == HB_FEATURE_GLOBAL_انتها) ?  F_GLOBAL : F_NONE,
		      feature->value);
  }

  إذا (planner->shaper->override_features)
    planner->shaper->override_features (planner);
}


/*
 * shaper face data
 */

بنية hb_ot_face_داده_t {};

hb_ot_face_داده_t *
hb_ot_shaper_face_داده_create (hb_face_t *face)
{
  أرجع (hb_ot_face_داده_t *) HB_SHAPER_داده_SUCCEEDED;
}

فراغ
hb_ot_shaper_face_داده_destroy (hb_ot_face_داده_t *داده)
{
}


/*
 * shaper font data
 */

بنية hb_ot_font_داده_t {
  OT::hb_scalar_cache_t استفادهنشده_2; // Just for alignment
};

hb_ot_font_داده_t *
hb_ot_shaper_font_داده_create (hb_font_t *font)
{
  ثابت OT::قلم_Variation_Store &var_store = font->face->جدول.GDEF->جدول->get_var_store ();
  أرجع (hb_ot_font_داده_t *) var_store.create_cache ();
}

فراغ
hb_ot_shaper_font_داده_destroy (hb_ot_font_داده_t *داده)
{
  OT::قلم_Variation_Store::destroy_cache ((OT::hb_scalar_cache_t *) داده);
}


/*
 * shaper
 */

بنية hb_ot_shape_context_t
{
  hb_ot_shape_plan_t *plan;
  hb_font_t *font;
  hb_face_t *face;
  hb_میانگیر_t  *میانگیر;
  ثابت hb_feature_t *user_features;
  بیعلامت صحيح        رقم_user_features;

  /* Transient stuff */
  hb_direction_t هدف_direction;
};



/* Main shaper */


/* Prepare */

ساكن فراغ
hb_مجموعه_unicode_props (hb_میانگیر_t *میانگیر,
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
  بیعلامت صحيح شمار = میانگیر->len;
  hb_glyph_info_t *info = میانگیر->info;
  لكل (بیعلامت صحيح i = 0; i < شمار; i++)
  {
    info[i].ماسک = global_ماسک;
    hb_glyph_info_مجموعه_unicode_props (&info[i], میانگیر);

    إذا (info[i].codepoint < 0x80)
      استمر;

    بیعلامت gen_cat = _hb_glyph_info_get_general_category (&info[i]);
    إذا (FLAG_UNSAFE (gen_cat) &
	(FLAG (HB_UNICODE_GENERAL_CATEGORY_LOWERCASE_LETTER) |
	 FLAG (HB_UNICODE_GENERAL_CATEGORY_UPPERCASE_LETTER) |
	 FLAG (HB_UNICODE_GENERAL_CATEGORY_TITLECASE_LETTER) |
	 FLAG (HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER) |
	 FLAG (HB_UNICODE_GENERAL_CATEGORY_SPACE_SEPARATOR)))
      استمر;

    /* Marks are already set as continuation by the above line.
     * Handle Emoji_Modifier and ZWJ-continuation. */
    إذا (unlikely (gen_cat == HB_UNICODE_GENERAL_CATEGORY_MODIFIER_نماد &&
		  hb_درون_range<hb_codepoint_t> (info[i].codepoint, 0x1F3FBu, 0x1F3FFu)))
    {
      hb_glyph_info_مجموعه_continuation (&info[i], میانگیر);
    }
    /* Regional_Indicators are hairy as hell...
     * https://github.com/harfbuzz/harfbuzz/issues/2265 */
    وإلا إذا (unlikely (i && _hb_codepoint_is_regional_indicator (info[i].codepoint)))
    {
      إذا (_hb_codepoint_is_regional_indicator (info[i - 1].codepoint) &&
	  !_hb_glyph_info_is_continuation (&info[i - 1]))
	hb_glyph_info_مجموعه_continuation (&info[i], میانگیر);
    }
#اگر_تعریف_نشده HB_NO_EMOJI_SEQUENCES
    وإلا إذا (unlikely (_hb_glyph_info_is_zwj (&info[i])))
    {
      hb_glyph_info_مجموعه_continuation (&info[i], میانگیر);
      إذا (i + 1 < شمار &&
	  _hb_unicode_is_emoji_Extended_Pictographic (info[i + 1].codepoint))
      {
	i++;
	info[i].ماسک = global_ماسک;
	hb_glyph_info_مجموعه_unicode_props (&info[i], میانگیر);
	hb_glyph_info_مجموعه_continuation (&info[i], میانگیر);
      }
    }
#پایان_اگر
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
    وإلا إذا (unlikely (hb_درون_ranges<hb_codepoint_t> (info[i].codepoint, 0xFF9Eu, 0xFF9Fu, 0xE0020u, 0xE007Fu)))
      hb_glyph_info_مجموعه_continuation (&info[i], میانگیر);
    وإلا إذا (unlikely (info[i].codepoint == 0x2044u /* FRACTION SLASH */))
      میانگیر->scratch_flags |= HB_میانگیر_SCRATCH_FLAG_HAS_FRACTION_SLASH;
  }
}

ساكن فراغ
hb_درج_dotted_circle (hb_میانگیر_t *میانگیر, hb_font_t *font)
{
  إذا (unlikely (میانگیر->flags & HB_میانگیر_FLAG_DO_NOT_درج_DOTTED_CIRCLE))
    أرجع;

  إذا (!(میانگیر->flags & HB_میانگیر_FLAG_BOT) ||
      میانگیر->context_len[0] ||
      !_hb_glyph_info_is_unicode_mark (&میانگیر->info[0]))
    أرجع;

  إذا (!font->has_glyph (0x25CCu))
    أرجع;

  hb_glyph_info_t dottedcircle = {0};
  dottedcircle.codepoint = 0x25CCu;
  hb_glyph_info_مجموعه_unicode_props (&dottedcircle, میانگیر);

  میانگیر->clear_output ();

  میانگیر->idx = 0;
  hb_glyph_info_t info = dottedcircle;
  info.cluster = میانگیر->cur().cluster;
  info.ماسک = میانگیر->cur().ماسک;
  (فراغ) میانگیر->output_info (info);

  میانگیر->متزامن ();
}

ساكن فراغ
hb_form_clusters (hb_میانگیر_t *میانگیر)
{
  إذا (!(میانگیر->scratch_flags & HB_میانگیر_SCRATCH_FLAG_HAS_CONTINUATIONS))
    أرجع;

  foreach_grapheme (میانگیر, شروع, انتها)
    میانگیر->ادغام_grapheme_clusters (شروع, انتها);
}

ساكن فراغ
hb_ensure_native_direction (hb_میانگیر_t *میانگیر)
{
  hb_direction_t direction = میانگیر->props.direction;
  hb_direction_t horiz_dir = hb_script_get_horizontal_direction (میانگیر->props.script);

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
    منطقي یافت_عدد = خطأ, یافت_letter = خطأ, یافت_ri = خطأ;
    ثابت خودکار* info = میانگیر->info;
    ثابت خودکار شمار = میانگیر->len;
    لكل (بیعلامت i = 0; i < شمار; i++)
    {
      خودکار gc = _hb_glyph_info_get_general_category (&info[i]);
      إذا (HB_UNICODE_GENERAL_CATEGORY_IS_LETTER (gc))
      {
	یافت_letter = درست;
	اكسر;
      }
      وإلا إذا (gc == HB_UNICODE_GENERAL_CATEGORY_DECIMAL_عدد)
	یافت_عدد = درست;
      وإلا إذا (unlikely (_hb_codepoint_is_regional_indicator (info[i].codepoint)))
	یافت_ri = درست;
    }
    إذا ((یافت_عدد || یافت_ri) && !یافت_letter)
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
    hb_ot_layout_معکوس_graphemes (میانگیر);
    میانگیر->props.direction = HB_DIRECTION_معکوس (میانگیر->props.direction);
  }
}


/*
 * Substitute
 */

ساكن مضمن فراغ
hb_ot_rotate_chars (ثابت hb_ot_shape_context_t *c)
{
  hb_میانگیر_t *میانگیر = c->میانگیر;
  بیعلامت صحيح شمار = میانگیر->len;
  hb_glyph_info_t *info = میانگیر->info;

  إذا (HB_DIRECTION_IS_BACKWARD (c->هدف_direction))
  {
    hb_unicode_funcs_t *unicode = میانگیر->unicode;
    hb_ماسک_t rtlm_ماسک = c->plan->rtlm_ماسک;

    لكل (بیعلامت صحيح i = 0; i < شمار; i++) {
      hb_codepoint_t codepoint = unicode->mirroring (info[i].codepoint);
      إذا (unlikely (codepoint != info[i].codepoint && c->font->has_glyph (codepoint)))
	info[i].codepoint = codepoint;
      وإلا
	info[i].ماسک |= rtlm_ماسک;
    }
  }

#اگر_تعریف_نشده HB_NO_VERTICAL
  إذا (HB_DIRECTION_IS_VERTICAL (c->هدف_direction) && !c->plan->has_vert)
  {
    لكل (بیعلامت صحيح i = 0; i < شمار; i++) {
      hb_codepoint_t codepoint = hb_unicode_funcs_t::vertical_char_for (info[i].codepoint);
      إذا (unlikely (codepoint != info[i].codepoint && c->font->has_glyph (codepoint)))
	info[i].codepoint = codepoint;
    }
  }
#پایان_اگر
}

ساكن مضمن فراغ
hb_ot_shape_راهاندازی_masks_fraction (ثابت hb_ot_shape_context_t *c)
{
#اگر_تعریف_شده HB_NO_OT_SHAPE_FRACTIONS
  أرجع;
#پایان_اگر

  إذا (!(c->میانگیر->scratch_flags & HB_میانگیر_SCRATCH_FLAG_HAS_FRACTION_SLASH) ||
      !c->plan->has_frac)
    أرجع;

  hb_میانگیر_t *میانگیر = c->میانگیر;

  hb_ماسک_t pre_ماسک, post_ماسک;
  إذا (HB_DIRECTION_IS_FORWARD (میانگیر->props.direction))
  {
    pre_ماسک = c->plan->numr_ماسک | c->plan->frac_ماسک;
    post_ماسک = c->plan->frac_ماسک | c->plan->dnom_ماسک;
  }
  وإلا
  {
    pre_ماسک = c->plan->frac_ماسک | c->plan->dnom_ماسک;
    post_ماسک = c->plan->numr_ماسک | c->plan->frac_ماسک;
  }

  بیعلامت صحيح شمار = میانگیر->len;
  hb_glyph_info_t *info = میانگیر->info;
  لكل (بیعلامت صحيح i = 0; i < شمار; i++)
  {
    إذا (info[i].codepoint == 0x2044u) /* FRACTION SLASH */
    {
      بیعلامت صحيح شروع = i, انتها = i + 1;
      طالما (شروع &&
	     _hb_glyph_info_get_general_category (&info[شروع - 1]) ==
	     HB_UNICODE_GENERAL_CATEGORY_DECIMAL_عدد)
	شروع--;
      طالما (انتها < شمار &&
	     _hb_glyph_info_get_general_category (&info[انتها]) ==
	     HB_UNICODE_GENERAL_CATEGORY_DECIMAL_عدد)
	انتها++;
      إذا (شروع == i || انتها == i + 1)
      {
        إذا (شروع == i)
	  میانگیر->unsafe_مقصد_concat (شروع, شروع + 1);
	إذا (انتها == i + 1)
	  میانگیر->unsafe_مقصد_concat (انتها - 1, انتها);
	استمر;
      }

      میانگیر->unsafe_مقصد_break (شروع, انتها);

      لكل (بیعلامت صحيح j = شروع; j < i; j++)
	info[j].ماسک |= pre_ماسک;
      info[i].ماسک |= c->plan->frac_ماسک;
      لكل (بیعلامت صحيح j = i + 1; j < انتها; j++)
	info[j].ماسک |= post_ماسک;

      i = انتها - 1;
    }
  }
}

ساكن مضمن فراغ
hb_ot_shape_راهاندازی_masks (ثابت hb_ot_shape_context_t *c)
{
  hb_ot_map_t *map = &c->plan->map;
  hb_میانگیر_t *میانگیر = c->میانگیر;

  hb_ot_shape_راهاندازی_masks_fraction (c);

  إذا (c->plan->shaper->راهاندازی_masks)
    c->plan->shaper->راهاندازی_masks (c->plan, میانگیر, c->font);

  لكل (بیعلامت صحيح i = 0; i < c->رقم_user_features; i++)
  {
    ثابت hb_feature_t *feature = &c->user_features[i];
    إذا (!(feature->شروع == HB_FEATURE_GLOBAL_شروع && feature->انتها == HB_FEATURE_GLOBAL_انتها)) {
      بیعلامت صحيح لغزش;
      hb_ماسک_t ماسک = map->get_ماسک (feature->برچسب_2, &لغزش);
      میانگیر->مجموعه_masks (feature->value << لغزش, ماسک, feature->شروع, feature->انتها);
    }
  }
}

ساكن فراغ
hb_ot_zero_عرض_default_ignorables (ثابت hb_میانگیر_t *میانگیر)
{
  إذا (!(میانگیر->scratch_flags & HB_میانگیر_SCRATCH_FLAG_HAS_DEFAULT_IGNORABLES) ||
      (میانگیر->flags & HB_میانگیر_FLAG_PRESERVE_DEFAULT_IGNORABLES) ||
      (میانگیر->flags & HB_میانگیر_FLAG_REMOVE_DEFAULT_IGNORABLES))
    أرجع;

  بیعلامت صحيح شمار = میانگیر->len;
  hb_glyph_info_t *info = میانگیر->info;
  hb_glyph_position_t *pos = میانگیر->pos;
  بیعلامت صحيح i = 0;
  لكل (i = 0; i < شمار; i++)
    إذا (unlikely (_hb_glyph_info_is_default_ignorable (&info[i])))
    {
      pos[i].x_advance = pos[i].y_advance = 0;
      إذا (HB_DIRECTION_IS_HORIZONTAL (میانگیر->props.direction))
	pos[i].x_جابه_جایی_2 = 0;
      وإلا
        pos[i].y_جابه_جایی_2 = 0;
    }
}

ساكن فراغ
hb_ot_deal_with_variation_selectors (hb_میانگیر_t *میانگیر)
{
  إذا (!(میانگیر->scratch_flags & HB_میانگیر_SCRATCH_FLAG_HAS_VARIATION_SELECTOR_FALLBACK) ||
	میانگیر->not_یافت_variation_selector == HB_CODEPOINT_INVALID)
    أرجع;

  بیعلامت صحيح شمار = میانگیر->len;
  hb_glyph_info_t *info = میانگیر->info;
  hb_glyph_position_t *pos = میانگیر->pos;

  لكل (بیعلامت صحيح i = 0; i < شمار; i++)
  {
    إذا (_hb_glyph_info_is_variation_selector (&info[i]))
    {
      info[i].codepoint = میانگیر->not_یافت_variation_selector;
      pos[i].x_advance = pos[i].y_advance = pos[i].x_جابه_جایی_2 = pos[i].y_جابه_جایی_2 = 0;
      hb_glyph_info_مجموعه_variation_selector (&info[i], خطأ);
    }
  }
}

ساكن فراغ
hb_ot_hide_default_ignorables (hb_میانگیر_t *میانگیر,
			       hb_font_t   *font)
{
  إذا (!(میانگیر->scratch_flags & HB_میانگیر_SCRATCH_FLAG_HAS_DEFAULT_IGNORABLES) ||
      (میانگیر->flags & HB_میانگیر_FLAG_PRESERVE_DEFAULT_IGNORABLES))
    أرجع;

  بیعلامت صحيح شمار = میانگیر->len;
  hb_glyph_info_t *info = میانگیر->info;

  hb_codepoint_t invisible = میانگیر->invisible;
  إذا (!(میانگیر->flags & HB_میانگیر_FLAG_REMOVE_DEFAULT_IGNORABLES) &&
      (invisible || font->get_nominal_glyph (' ', &invisible)))
  {
    /* Replace default-ignorables with a zero-advance invisible glyph. */
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      إذا (_hb_glyph_info_is_default_ignorable (&info[i]))
	info[i].codepoint = invisible;
    }
  }
  وإلا
    میانگیر->delete_glyphs_inplace (_hb_glyph_info_is_default_ignorable);
}


ساكن مضمن فراغ
hb_ot_map_glyphs_fast (hb_میانگیر_t  *میانگیر)
{
  /* Normalization process sets up normalizer_glyph_index(), we just copy it. */
  بیعلامت صحيح شمار = میانگیر->len;
  hb_glyph_info_t *info = میانگیر->info;
  لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    info[i].codepoint = info[i].normalizer_glyph_شاخص();

  میانگیر->content_type = HB_میانگیر_CONTENT_TYPE_GLYPHS;
}

ساكن مضمن فراغ
hb_synthesize_glyph_classes (hb_میانگیر_t *میانگیر)
{
  بیعلامت صحيح شمار = میانگیر->len;
  hb_glyph_info_t *info = میانگیر->info;
  لكل (بیعلامت صحيح i = 0; i < شمار; i++)
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
	    HB_OT_LAYOUT_GLYPH_PROPS_پایه_GLYPH :
	    HB_OT_LAYOUT_GLYPH_PROPS_MARK;
    hb_glyph_info_مجموعه_glyph_props (&info[i], klass);
  }
}

ساكن مضمن فراغ
hb_ot_substitute_default (ثابت hb_ot_shape_context_t *c)
{
  hb_میانگیر_t *میانگیر = c->میانگیر;

  hb_ot_rotate_chars (c);

  HB_میانگیر_ALLOCATE_VAR (میانگیر, normalizer_glyph_شاخص);

  _hb_ot_shape_normalize (c->plan, میانگیر, c->font);

  hb_ot_shape_راهاندازی_masks (c);

  /* This is unfortunate to go here, but necessary... */
  إذا (c->plan->fallback_mark_positioning)
    _hb_ot_shape_fallback_mark_position_recategorize_marks (c->plan, c->font, میانگیر);

  hb_ot_map_glyphs_fast (میانگیر);

  HB_میانگیر_DEALLOCATE_VAR (میانگیر, normalizer_glyph_شاخص);
}

ساكن مضمن فراغ
hb_ot_substitute_plan (ثابت hb_ot_shape_context_t *c)
{
  hb_میانگیر_t *میانگیر = c->میانگیر;

#اگر_تعریف_نشده HB_NO_AAT_SHAPE
  إذا (unlikely (c->plan->apply_morx))
  {
    hb_ot_layout_substitute_شروع (c->font, میانگیر);

    إذا (c->plan->fallback_glyph_classes)
      hb_synthesize_glyph_classes (میانگیر);

    hb_aat_layout_substitute (c->plan, c->font, c->میانگیر,
			      c->user_features, c->رقم_user_features);
    /* The buffer digest is only used by the OT lookup-apply loop;
     * without GPOS ahead, nothing consumes it. */
    إذا (c->plan->apply_gpos)
      c->میانگیر->update_digest ();
  }
  وإلا
#پایان_اگر
  {
    hb_ot_layout_substitute_شروع_with_digest (c->font, میانگیر);

    إذا (c->plan->fallback_glyph_classes)
      hb_synthesize_glyph_classes (میانگیر);

    c->plan->substitute (c->font, میانگیر);
  }
}

ساكن مضمن فراغ
hb_ot_substitute_pre (ثابت hb_ot_shape_context_t *c)
{
  hb_ot_substitute_default (c);

  hb_میانگیر_allocate_gsubgpos_vars (c->میانگیر);

  hb_ot_substitute_plan (c);

#اگر_تعریف_نشده HB_NO_AAT_SHAPE
  إذا (c->plan->apply_morx && c->plan->apply_gpos)
    hb_aat_layout_remove_deleted_glyphs (c->میانگیر);
#پایان_اگر
}

ساكن مضمن فراغ
hb_ot_substitute_post (ثابت hb_ot_shape_context_t *c)
{
#اگر_تعریف_نشده HB_NO_AAT_SHAPE
  إذا (c->plan->apply_morx && !c->plan->apply_gpos)
    hb_aat_layout_remove_deleted_glyphs (c->میانگیر);
#پایان_اگر

  hb_ot_deal_with_variation_selectors (c->میانگیر);
  hb_ot_hide_default_ignorables (c->میانگیر, c->font);

  إذا (c->plan->shaper->postprocess_glyphs &&
    c->میانگیر->پیام(c->font, "start postprocess-glyphs")) {
    c->plan->shaper->postprocess_glyphs (c->plan, c->میانگیر, c->font);
    (فراغ) c->میانگیر->پیام(c->font, "end postprocess-glyphs");
  }
}


/*
 * Position
 */

ساكن مضمن فراغ
adjust_mark_offsets (hb_glyph_position_t *pos)
{
  pos->x_جابه_جایی_2 = hb_saturate_sub (pos->x_جابه_جایی_2, pos->x_advance);
  pos->y_جابه_جایی_2 = hb_saturate_sub (pos->y_جابه_جایی_2, pos->y_advance);
}

ساكن مضمن فراغ
zero_mark_عرض (hb_glyph_position_t *pos)
{
  pos->x_advance = 0;
  pos->y_advance = 0;
}

ساكن مضمن فراغ
zero_mark_widths_by_gdef (hb_میانگیر_t *میانگیر, منطقي adjust_offsets)
{
  بیعلامت صحيح شمار = میانگیر->len;
  hb_glyph_info_t *info = میانگیر->info;
  لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    إذا (_hb_glyph_info_is_mark (&info[i]))
    {
      إذا (adjust_offsets)
	adjust_mark_offsets (&میانگیر->pos[i]);
      zero_mark_عرض (&میانگیر->pos[i]);
    }
}

ساكن مضمن فراغ
hb_ot_position_default (ثابت hb_ot_shape_context_t *c)
{
  hb_direction_t direction = c->میانگیر->props.direction;
  بیعلامت صحيح شمار = c->میانگیر->len;
  hb_glyph_info_t *info = c->میانگیر->info;
  hb_glyph_position_t *pos = c->میانگیر->pos;

  إذا (HB_DIRECTION_IS_HORIZONTAL (direction))
  {
    c->font->get_glyph_h_advances (شمار, &info[0].codepoint, حجم(info[0]),
				   &pos[0].x_advance, حجم(pos[0]));
    // h_origin defaults to zero; only apply it if the font has it.
    إذا (c->font->has_glyph_h_origin_func () || c->font->has_glyph_h_origins_func ())
      c->font->تفریق_glyph_h_origins (c->میانگیر);
  }
  وإلا
  {
    c->font->get_glyph_v_advances (شمار, &info[0].codepoint, حجم(info[0]),
				   &pos[0].y_advance, حجم(pos[0]));
    // v_origin defaults to non-zero; apply even if only fallback is there.
    c->font->تفریق_glyph_v_origins (c->میانگیر);
  }
  إذا (c->میانگیر->scratch_flags & HB_میانگیر_SCRATCH_FLAG_HAS_SPACE_FALLBACK)
    _hb_ot_shape_fallback_spaces (c->plan, c->font, c->میانگیر);
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
				     HB_DIRECTION_IS_FORWARD (c->میانگیر->props.direction);

  /* We change glyph origin to what GPOS expects (horizontal), apply GPOS, change it back. */

  // h_origin defaults to zero; only apply it if the font has it.
  إذا (c->font->has_glyph_h_origin_func () || c->font->has_glyph_h_origins_func ())
    c->font->جمع_کن_glyph_h_origins (c->میانگیر);

  hb_ot_layout_position_شروع (c->font, c->میانگیر);

  إذا (c->plan->zero_marks)
    بدل (c->plan->shaper->zero_عرض_marks)
    {
      حالة HB_OT_SHAPE_ZERO_عرض_MARKS_BY_GDEF_EARLY:
	zero_mark_widths_by_gdef (c->میانگیر, adjust_offsets_when_zeroing);
	اكسر;

      افتراضي:
      حالة HB_OT_SHAPE_ZERO_عرض_MARKS_NONE:
      حالة HB_OT_SHAPE_ZERO_عرض_MARKS_BY_GDEF_LATE:
	اكسر;
    }

  c->plan->position (c->font, c->میانگیر);

  إذا (c->plan->zero_marks)
    بدل (c->plan->shaper->zero_عرض_marks)
    {
      حالة HB_OT_SHAPE_ZERO_عرض_MARKS_BY_GDEF_LATE:
	zero_mark_widths_by_gdef (c->میانگیر, adjust_offsets_when_zeroing);
	اكسر;

      افتراضي:
      حالة HB_OT_SHAPE_ZERO_عرض_MARKS_NONE:
      حالة HB_OT_SHAPE_ZERO_عرض_MARKS_BY_GDEF_EARLY:
	اكسر;
    }

  /* Finish off.  Has to follow a certain order. */
  hb_ot_layout_position_پایان_advances (c->font, c->میانگیر);
  hb_ot_zero_عرض_default_ignorables (c->میانگیر);
  hb_ot_layout_position_پایان_offsets (c->font, c->میانگیر);

  // h_origin defaults to zero; only apply it if the font has it.
  إذا (c->font->has_glyph_h_origin_func () || c->font->has_glyph_h_origins_func ())
    c->font->تفریق_glyph_h_origins (c->میانگیر);

  إذا (c->plan->fallback_mark_positioning)
    _hb_ot_shape_fallback_mark_position (c->plan, c->font, c->میانگیر,
					 adjust_offsets_when_zeroing);
}

ساكن مضمن فراغ
hb_ot_position (ثابت hb_ot_shape_context_t *c)
{
  c->میانگیر->clear_positions ();

  hb_ot_position_default (c);

  hb_ot_position_plan (c);

  إذا (HB_DIRECTION_IS_BACKWARD (c->میانگیر->props.direction))
    hb_میانگیر_معکوس (c->میانگیر);

  hb_میانگیر_deallocate_gsubgpos_vars (c->میانگیر);
}

ساكن مضمن فراغ
hb_propagate_flags (hb_میانگیر_t *میانگیر)
{
  /* Propagate cluster-level glyph flags to be the same on all cluster glyphs.
   * Simplifies using them. */

  hb_ماسک_t and_ماسک = HB_GLYPH_FLAG_DEFINED;
  إذا ((میانگیر->flags & HB_میانگیر_FLAG_PRODUCE_UNSAFE_مقصد_CONCAT) == 0)
    and_ماسک &= ~HB_GLYPH_FLAG_UNSAFE_مقصد_CONCAT;

  hb_glyph_info_t *info = میانگیر->info;

  إذا ((میانگیر->flags & HB_میانگیر_FLAG_PRODUCE_امن_مقصد_درج_TATWEEL) == 0)
  {
    foreach_cluster (میانگیر, شروع, انتها)
    {
      إذا (انتها - شروع == 1)
      {
        info[شروع].ماسک &= and_ماسک;
	استمر;
      }

      بیعلامت صحيح ماسک = 0;
      لكل (بیعلامت صحيح i = شروع; i < انتها; i++)
	ماسک |= info[i].ماسک;

      ماسک &= and_ماسک;

      لكل (بیعلامت صحيح i = شروع; i < انتها; i++)
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
  foreach_cluster (میانگیر, شروع, انتها)
  {
    بیعلامت صحيح ماسک = 0;
    لكل (بیعلامت صحيح i = شروع; i < انتها; i++)
      ماسک |= info[i].ماسک;

    إذا (ماسک & HB_GLYPH_FLAG_UNSAFE_مقصد_BREAK)
      ماسک &= ~HB_GLYPH_FLAG_امن_مقصد_درج_TATWEEL;
    إذا (ماسک & HB_GLYPH_FLAG_امن_مقصد_درج_TATWEEL)
      ماسک |= HB_GLYPH_FLAG_UNSAFE_مقصد_BREAK | HB_GLYPH_FLAG_UNSAFE_مقصد_CONCAT;

    ماسک &= and_ماسک;

    لكل (بیعلامت صحيح i = شروع; i < انتها; i++)
      info[i].ماسک = ماسک;
  }
}

/* Pull it all together! */

ساكن فراغ
hb_ot_shape_داخلی (hb_ot_shape_context_t *c)
{
  /* Save the original direction, we use it later. */
  c->هدف_direction = c->میانگیر->props.direction;

  hb_میانگیر_allocate_unicode_vars (c->میانگیر);

  hb_مجموعه_unicode_props (c->میانگیر, c->plan->map.get_global_ماسک ());
  hb_درج_dotted_circle (c->میانگیر, c->font);

  hb_form_clusters (c->میانگیر);

  hb_ensure_native_direction (c->میانگیر);

  إذا (c->plan->shaper->preprocess_متن &&
      c->میانگیر->پیام(c->font, "start preprocess-text"))
  {
    c->plan->shaper->preprocess_متن (c->plan, c->میانگیر, c->font);
    (فراغ) c->میانگیر->پیام(c->font, "end preprocess-text");
  }

  hb_ot_substitute_pre (c);
  hb_ot_position (c);
  hb_ot_substitute_post (c);

  hb_propagate_flags (c->میانگیر);

  hb_میانگیر_deallocate_unicode_vars (c->میانگیر);

  c->میانگیر->props.direction = c->هدف_direction;
}


hb_bool_t
_hb_ot_shape (hb_shape_plan_t    *shape_plan,
	      hb_font_t          *font,
	      hb_میانگیر_t        *میانگیر,
	      ثابت hb_feature_t *features,
	      بیعلامت صحيح        رقم_features)
{
  hb_ot_shape_context_t c = {&shape_plan->ot, font, font->face, میانگیر, features, رقم_features};
  hb_ot_shape_داخلی (&c);

  أرجع درست;
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
				  hb_برچسب_t         جدول_برچسب_2,
				  hb_مجموعه_t        *lookup_indexes /*بیرون*/)
{
  shape_plan->ot.collect_lookups (جدول_برچسب_2, lookup_indexes);
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
بیعلامت صحيح
hb_ot_shape_plan_get_feature_برچسبها (hb_shape_plan_t *shape_plan,
				   بیعلامت صحيح     شروع_جابه_جایی,
				   بیعلامت صحيح    *برچسب_شمار, /* IN/OUT */
				   hb_برچسب_t        *برچسبها /*بیرون*/)
{
#اگر_تعریف_نشده HB_NO_OT_SHAPE
  أرجع shape_plan->ot.map.get_feature_برچسبها (شروع_جابه_جایی, برچسب_شمار, برچسبها);
#وإلا
  إذا (برچسب_شمار)
	*برچسب_شمار = 0;
  أرجع 0;
#پایان_اگر
}


/* TODO Move this to hb-ot-shape-normalize, make it do decompose, and make it public. */
ساكن فراغ
جمع_کن_char (hb_font_t          *font,
	  hb_unicode_funcs_t *unicode,
	  hb_bool_t           mirror,
	  hb_codepoint_t      u,
	  hb_مجموعه_t           *glyphs)
{
  hb_codepoint_t glyph;
  إذا (font->get_nominal_glyph (u, &glyph))
    glyphs->جمع_کن (glyph);
  إذا (mirror)
  {
    hb_codepoint_t m = unicode->mirroring (u);
    إذا (m != u && font->get_nominal_glyph (m, &glyph))
      glyphs->جمع_کن (glyph);
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
hb_ot_shape_glyphs_بستگی (hb_font_t          *font,
			    hb_میانگیر_t        *میانگیر,
			    ثابت hb_feature_t *features,
			    بیعلامت صحيح        رقم_features,
			    hb_مجموعه_t           *glyphs)
{
  ثابت حرف *shapers[] = {"ot", اشاره_تهی};
  hb_shape_plan_t *shape_plan = hb_shape_plan_create_cached (font->face, &میانگیر->props,
							     features, رقم_features, shapers);

  منطقي mirror = hb_script_get_horizontal_direction (میانگیر->props.script) == HB_DIRECTION_RTL;

  بیعلامت صحيح شمار = میانگیر->len;
  hb_glyph_info_t *info = میانگیر->info;
  لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    جمع_کن_char (font, میانگیر->unicode, mirror, info[i].codepoint, glyphs);

  hb_مجموعه_t *lookups = hb_مجموعه_create ();
  hb_ot_shape_plan_collect_lookups (shape_plan, HB_OT_برچسب_GSUB, lookups);
  hb_ot_layout_lookups_substitute_بستگی (font->face, lookups, glyphs);

  hb_مجموعه_destroy (lookups);

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
بیعلامت صحيح
hb_ot_shape_get_میانگیر_format_serial (فراغ)
{
  أرجع HB_OT_SHAPE_میانگیر_FORMAT_SERIAL;
}

#پایان_اگر

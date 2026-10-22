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

#समावेश "hb.hh"

#अगर_अघोषित HB_NO_OT_SHAPE

#अगर_घोषित HB_NO_OT_LAYOUT
#त्रुटि "Cannot compile 'ot' shaper with HB_NO_OT_LAYOUT."
#पूर्ण_अगर

#समावेश "hb-shaper-impl.hh"

#समावेश "hb-ot-shape.hh"
#समावेश "hb-ot-shaper.hh"
#समावेश "hb-ot-shape-fallback.hh"
#समावेश "hb-ot-shape-normalize.hh"

#समावेश "hb-ot-face.hh"

#समावेश "hb-set.hh"
#समावेश "hb-unicode.hh"

#समावेश "hb-aat-layout.hh"
#समावेश "hb-ot-layout-gdef-table.hh"
#समावेश "hb-ot-stat-table.hh"


अटल अंतरभूत bool
_hb_codepoint_is_regional_indicator (hb_codepoint_t u)
{ वापस hb_in_range<hb_codepoint_t> (u, 0x1F1E6u, 0x1F1FFu); }

#अगर_अघोषित HB_NO_AAT_SHAPE
अटल अंतरभूत bool
_hb_apply_morx (hb_face_t *face, स्थिर hb_segment_properties_t &props)
{
  /* https://github.com/harfbuzz/harfbuzz/issues/2124 */
  वापस hb_aat_layout_has_substitution (face) &&
	 (HB_DIRECTION_IS_HORIZONTAL (props.direction) || !hb_ot_layout_has_substitution (face));
}
#पूर्ण_अगर

/**
 * SECTION:hb-ot-shape
 * @title: hb-ot-shape
 * @short_description: OpenType shaping support
 * @include: hb-ot.h
 *
 * Support functions for OpenType shaping related queries.
 **/


अटल व्योम
hb_ot_shape_collect_features (hb_ot_shape_planner_t          *planner,
			      स्थिर hb_feature_t             *user_features,
			      अचिन्हित पूर्णांक                    अंक_user_features);

hb_ot_shape_planner_t::hb_ot_shape_planner_t (hb_face_t                     *face,
					      स्थिर hb_segment_properties_t &props) :
						face (face),
						props (props),
						map (face, props)
#अगर_अघोषित HB_NO_AAT_SHAPE
						, aat_map (face, props)
						, apply_morx (_hb_apply_morx (face, props))
#पूर्ण_अगर
{
  shaper = hb_ot_shaper_categorize (props.script, props.direction, map.chosen_script[0]);

  script_zero_marks = shaper->zero_width_marks != HB_OT_SHAPE_ZERO_WIDTH_MARKS_NONE;
  script_fallback_position = shaper->fallback_position;

#अगर_अघोषित HB_NO_AAT_SHAPE
  /* https://github.com/harfbuzz/harfbuzz/issues/1528 */
  अगर (apply_morx && shaper != &_hb_ot_shaper_default)
    shaper = &_hb_ot_shaper_dumber;
#पूर्ण_अगर
}

व्योम
hb_ot_shape_planner_t::compile (hb_ot_shape_plan_t           &plan,
				स्थिर hb_ot_shape_plan_कुंजी_t &key)
{
  plan.props = props;
  plan.shaper = shaper;
  map.compile (plan.map, key);
#अगर_अघोषित HB_NO_AAT_SHAPE
  अगर (apply_morx)
    aat_map.compile (plan.aat_map);
#पूर्ण_अगर

#अगर_अघोषित HB_NO_OT_SHAPE_FRACTIONS
  plan.frac_mask = plan.map.get_1_mask (HB_TAG ('f','r','a','c'));
  plan.numr_mask = plan.map.get_1_mask (HB_TAG ('n','u','m','r'));
  plan.dnom_mask = plan.map.get_1_mask (HB_TAG ('d','n','o','m'));
  plan.has_frac = plan.frac_mask || (plan.numr_mask && plan.dnom_mask);
#पूर्ण_अगर

  plan.rtlm_mask = plan.map.get_1_mask (HB_TAG ('r','t','l','m'));
  plan.has_vert = !!plan.map.get_1_mask (HB_TAG ('v','e','r','t'));

  hb_tag_t kern_tag = HB_DIRECTION_IS_HORIZONTAL (props.direction) ?
		      HB_TAG ('k','e','r','n') : HB_TAG ('v','k','r','n');
#अगर_अघोषित HB_NO_OT_KERN
  plan.kern_mask = plan.map.get_mask (kern_tag);
  plan.requested_kerning = !!plan.kern_mask;
#पूर्ण_अगर

  bool has_gpos_kern = plan.map.get_feature_सूचकांक (1, kern_tag) != HB_OT_LAYOUT_NO_FEATURE_सूचकांक;
  bool disable_gpos = plan.shaper->gpos_tag &&
		      plan.shaper->gpos_tag != plan.map.chosen_script[1];

  /*
   * Decide who provides glyph classes. GDEF or Unicode.
   */

  अगर (!hb_ot_layout_has_glyph_classes (face))
    plan.fallback_glyph_classes = true;

  /*
   * Decide who does substitutions. GSUB, morx, or fallback.
   */

#अगर_अघोषित HB_NO_AAT_SHAPE
  plan.apply_morx = apply_morx;
#पूर्ण_अगर

  /*
   * Decide who does positioning. GPOS, kerx, kern, or fallback.
   */

#अगर_अघोषित HB_NO_AAT_SHAPE
  bool has_kerx = hb_aat_layout_has_positioning (face);
  bool has_gsub = !apply_morx && hb_ot_layout_has_substitution (face);
#पूर्ण_अगर
  bool has_gpos = !disable_gpos && hb_ot_layout_has_positioning (face);
  अगर (false)
    {}
#अगर_अघोषित HB_NO_AAT_SHAPE
  /* Prefer GPOS over kerx if GSUB is present;
   * https://github.com/harfbuzz/harfbuzz/issues/3008 */
  अन्यथा अगर (has_kerx && !(has_gsub && has_gpos))
    plan.apply_kerx = true;
#पूर्ण_अगर
  अन्यथा अगर (has_gpos)
    plan.apply_gpos = true;

  अगर (!plan.apply_kerx && (!has_gpos_kern || !plan.apply_gpos))
  {
    अगर (false) {}
#अगर_अघोषित HB_NO_AAT_SHAPE
    अन्यथा अगर (has_kerx)
      plan.apply_kerx = true;
#पूर्ण_अगर
#अगर_अघोषित HB_NO_OT_KERN
    अन्यथा अगर (hb_ot_layout_has_kerning (face))
      plan.apply_kern = script_fallback_position; // Not all shapers apply legacy `kern`
#पूर्ण_अगर
    अन्यथा {}
  }

  plan.apply_fallback_kern = script_fallback_position && !(plan.apply_gpos || plan.apply_kerx || plan.apply_kern);

  plan.zero_marks = script_zero_marks &&
		    !plan.apply_kerx &&
		    (!plan.apply_kern
#अगर_अघोषित HB_NO_OT_KERN
		     || !hb_ot_layout_has_machine_kerning (face)
#पूर्ण_अगर
		    );
  plan.has_gpos_mark = !!plan.map.get_1_mask (HB_TAG ('m','a','r','k'));

  plan.adjust_mark_positioning_when_zeroing = !plan.apply_gpos &&
					      !plan.apply_kerx &&
					      (!plan.apply_kern
#अगर_अघोषित HB_NO_OT_KERN
					       || !hb_ot_layout_has_cross_kerning (face)
#पूर्ण_अगर
					      );

  plan.fallback_mark_positioning = plan.adjust_mark_positioning_when_zeroing &&
				   script_fallback_position;

#अगर_अघोषित HB_NO_AAT_SHAPE
  /* If we're using morx shaping, we cancel mark position adjustment because
     Apple Color Emoji assumes this will NOT be done when forming emoji sequences;
     https://github.com/harfbuzz/harfbuzz/issues/2967. */
  अगर (plan.apply_morx)
    plan.adjust_mark_positioning_when_zeroing = false;

  /* According to Ned, trak is applied by default for "modern fonts", as detected by presence of STAT table. */
#अगर_अघोषित HB_NO_STYLE
  plan.apply_trak = hb_aat_layout_has_tracking (face) && face->table.STAT->has_तथ्य ();
#अन्यथा
  plan.apply_trak = false;
#पूर्ण_अगर

#पूर्ण_अगर
}

bool
hb_ot_shape_plan_t::init0 (hb_face_t                     *face,
			   स्थिर hb_shape_plan_कुंजी_t     *key)
{
  map.init ();

  hb_ot_shape_planner_t planner (face,
				 key->props);

  hb_ot_shape_collect_features (&planner,
				key->user_features,
				key->अंक_user_features);

  planner.compile (*यह, key->ot);

  अगर (shaper->तथ्य_create)
  {
    तथ्य = shaper->तथ्य_create (यह);
    अगर (unlikely (!तथ्य))
    {
      map.fini ();
      वापस false;
    }
  }

  वापस true;
}

व्योम
hb_ot_shape_plan_t::fini ()
{
  अगर (shaper->तथ्य_destroy)
    shaper->तथ्य_destroy (const_cast<व्योम *> (तथ्य));

  map.fini ();
}

व्योम
hb_ot_shape_plan_t::substitute (hb_font_t   *font,
				hb_बफर_t *बफर) स्थिर
{
  map.substitute (यह, font, बफर);
}

व्योम
hb_ot_shape_plan_t::position (hb_font_t   *font,
			      hb_बफर_t *बफर) स्थिर
{
  अगर (यह->apply_gpos)
    map.position (यह, font, बफर);
#अगर_अघोषित HB_NO_AAT_SHAPE
  अन्यथा अगर (यह->apply_kerx)
    hb_aat_layout_position (यह, font, बफर);
#पूर्ण_अगर

#अगर_अघोषित HB_NO_OT_KERN
  अगर (यह->apply_kern)
    hb_ot_layout_kern (यह, font, बफर);
#पूर्ण_अगर
  अन्यथा अगर (यह->apply_fallback_kern)
    _hb_ot_shape_fallback_kern (यह, font, बफर);

#अगर_अघोषित HB_NO_AAT_SHAPE
  अगर (यह->apply_trak)
    hb_aat_layout_track (यह, font, बफर);
#पूर्ण_अगर
}


अटल स्थिर hb_ot_map_feature_t
common_features[] =
{
  {HB_TAG('a','b','v','m'), F_GLOBAL},
  {HB_TAG('b','l','w','m'), F_GLOBAL},
  {HB_TAG('c','c','m','p'), F_GLOBAL},
  {HB_TAG('l','o','c','l'), F_GLOBAL},
  {HB_TAG('m','a','r','k'), F_GLOBAL_MANUAL_JOINERS},
  {HB_TAG('m','k','m','k'), F_GLOBAL_MANUAL_JOINERS},
  {HB_TAG('r','l','i','g'), F_GLOBAL},
};


अटल स्थिर hb_ot_map_feature_t
horizontal_features[] =
{
  {HB_TAG('c','a','l','t'), F_GLOBAL},
  {HB_TAG('c','l','i','g'), F_GLOBAL},
  {HB_TAG('c','u','r','s'), F_GLOBAL},
  {HB_TAG('d','i','s','t'), F_GLOBAL},
  {HB_TAG('k','e','r','n'), F_GLOBAL_HAS_FALLBACK},
  {HB_TAG('l','i','g','a'), F_GLOBAL},
  {HB_TAG('r','c','l','t'), F_GLOBAL},
};

अटल व्योम
hb_ot_shape_collect_features (hb_ot_shape_planner_t *planner,
			      स्थिर hb_feature_t    *user_features,
			      अचिन्हित पूर्णांक           अंक_user_features)
{
  hb_ot_map_builder_t *map = &planner->map;

  map->is_simple = true;

  map->enable_feature (HB_TAG('r','v','r','n'));
  map->add_gsub_pause (nullptr);

  चयन (planner->props.direction)
  {
    हाल HB_DIRECTION_LTR:
      map->enable_feature (HB_TAG ('l','t','r','a'));
      map->enable_feature (HB_TAG ('l','t','r','m'));
      अवरोध;
    हाल HB_DIRECTION_RTL:
      map->enable_feature (HB_TAG ('r','t','l','a'));
      map->add_feature (HB_TAG ('r','t','l','m'));
      अवरोध;
    हाल HB_DIRECTION_TTB:
    हाल HB_DIRECTION_BTT:
    हाल HB_DIRECTION_INVALID:
    शेष:
      अवरोध;
  }

#अगर_अघोषित HB_NO_OT_SHAPE_FRACTIONS
  /* Automatic fractions. */
  map->add_feature (HB_TAG ('f','r','a','c'));
  map->add_feature (HB_TAG ('n','u','m','r'));
  map->add_feature (HB_TAG ('d','n','o','m'));
#पूर्ण_अगर

  /* Random! */
  map->enable_feature (HB_TAG ('r','a','n','d'), F_RANDOM, HB_OT_MAP_MAX_मूल्य);

  map->enable_feature (HB_TAG ('H','a','r','f')); /* Considered required. */
  map->enable_feature (HB_TAG ('H','A','R','F')); /* Considered discretionary. */

  अगर (planner->shaper->collect_features)
  {
    map->is_simple = false;
    planner->shaper->collect_features (planner);
  }

  map->enable_feature (HB_TAG ('B','u','z','z')); /* Considered required. */
  map->enable_feature (HB_TAG ('B','U','Z','Z')); /* Considered discretionary. */

  क्रम (अचिन्हित पूर्णांक i = 0; i < ARRAY_लंबाई (common_features); i++)
    map->add_feature (common_features[i]);

  अगर (HB_DIRECTION_IS_HORIZONTAL (planner->props.direction))
    क्रम (अचिन्हित पूर्णांक i = 0; i < ARRAY_लंबाई (horizontal_features); i++)
      map->add_feature (horizontal_features[i]);
  अन्यथा
  {
    /* We only apply `vert` feature. See:
     * https://github.com/harfbuzz/harfbuzz/commit/d71c0df2d17f4590d5611239577a6cb532c26528
     * https://lists.freedesktop.org/archives/harfbuzz/2013-August/003490.html */

    /* We really want to find a 'vert' feature if there's any in the font, no
     * matter which script/langsys it is listed (or not) under.
     * See various bugs referenced from:
     * https://github.com/harfbuzz/harfbuzz/issues/63 */
    map->enable_feature (HB_TAG ('v','e','r','t'), F_GLOBAL_खोज);
  }

  अगर (अंक_user_features)
    map->is_simple = false;
  क्रम (अचिन्हित पूर्णांक i = 0; i < अंक_user_features; i++)
  {
    स्थिर hb_feature_t *feature = &user_features[i];
    map->add_feature (feature->tag,
		      (feature->start == HB_FEATURE_GLOBAL_START &&
		       feature->end == HB_FEATURE_GLOBAL_END) ?  F_GLOBAL : F_NONE,
		      feature->value);
  }

  अगर (planner->shaper->override_features)
    planner->shaper->override_features (planner);
}


/*
 * shaper face data
 */

काष्ठा hb_ot_face_तथ्य_t {};

hb_ot_face_तथ्य_t *
hb_ot_shaper_face_तथ्य_create (hb_face_t *face)
{
  वापस (hb_ot_face_तथ्य_t *) HB_SHAPER_तथ्य_SUCCEEDED;
}

व्योम
hb_ot_shaper_face_तथ्य_destroy (hb_ot_face_तथ्य_t *तथ्य)
{
}


/*
 * shaper font data
 */

काष्ठा hb_ot_font_तथ्य_t {
  OT::hb_scalar_cache_t unused; // Just for alignment
};

hb_ot_font_तथ्य_t *
hb_ot_shaper_font_तथ्य_create (hb_font_t *font)
{
  स्थिर OT::ItemVariationStore &var_store = font->face->table.GDEF->table->get_var_store ();
  वापस (hb_ot_font_तथ्य_t *) var_store.create_cache ();
}

व्योम
hb_ot_shaper_font_तथ्य_destroy (hb_ot_font_तथ्य_t *तथ्य)
{
  OT::ItemVariationStore::destroy_cache ((OT::hb_scalar_cache_t *) तथ्य);
}


/*
 * shaper
 */

काष्ठा hb_ot_shape_context_t
{
  hb_ot_shape_plan_t *plan;
  hb_font_t *font;
  hb_face_t *face;
  hb_बफर_t  *बफर;
  स्थिर hb_feature_t *user_features;
  अचिन्हित पूर्णांक        अंक_user_features;

  /* Transient stuff */
  hb_direction_t target_direction;
};



/* Main shaper */


/* Prepare */

अटल व्योम
hb_set_unicode_props (hb_बफर_t *बफर,
		      hb_mask_t    global_mask)
{
  /* Implement enough of Unicode Graphemes here that shaping
   * in reverse-direction wouldn't break graphemes.  Namely,
   * we mark all marks and ZWJ and ZWJ,Extended_Pictographic
   * sequences as continuations.  The foreach_grapheme()
   * macro uses this bit.
   *
   * https://www.unicode.org/reports/tr29/#Regex_Definitions
   */
  अचिन्हित पूर्णांक गिनती = बफर->len;
  hb_glyph_info_t *info = बफर->info;
  क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
  {
    info[i].mask = global_mask;
    _hb_glyph_info_set_unicode_props (&info[i], बफर);

    अगर (info[i].codepoint < 0x80)
      जारी;

    अचिन्हित gen_cat = _hb_glyph_info_get_general_category (&info[i]);
    अगर (FLAG_UNSAFE (gen_cat) &
	(FLAG (HB_UNICODE_GENERAL_CATEGORY_LOWERCASE_LETTER) |
	 FLAG (HB_UNICODE_GENERAL_CATEGORY_UPPERCASE_LETTER) |
	 FLAG (HB_UNICODE_GENERAL_CATEGORY_TITLECASE_LETTER) |
	 FLAG (HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER) |
	 FLAG (HB_UNICODE_GENERAL_CATEGORY_SPACE_SEPARATOR)))
      जारी;

    /* Marks are already set as continuation by the above line.
     * Handle Emoji_Modifier and ZWJ-continuation. */
    अगर (unlikely (gen_cat == HB_UNICODE_GENERAL_CATEGORY_MODIFIER_प्रतीक &&
		  hb_in_range<hb_codepoint_t> (info[i].codepoint, 0x1F3FBu, 0x1F3FFu)))
    {
      _hb_glyph_info_set_continuation (&info[i], बफर);
    }
    /* Regional_Indicators are hairy as hell...
     * https://github.com/harfbuzz/harfbuzz/issues/2265 */
    अन्यथा अगर (unlikely (i && _hb_codepoint_is_regional_indicator (info[i].codepoint)))
    {
      अगर (_hb_codepoint_is_regional_indicator (info[i - 1].codepoint) &&
	  !_hb_glyph_info_is_continuation (&info[i - 1]))
	_hb_glyph_info_set_continuation (&info[i], बफर);
    }
#अगर_अघोषित HB_NO_EMOJI_SEQUENCES
    अन्यथा अगर (unlikely (_hb_glyph_info_is_zwj (&info[i])))
    {
      _hb_glyph_info_set_continuation (&info[i], बफर);
      अगर (i + 1 < गिनती &&
	  _hb_unicode_is_emoji_Extended_Pictographic (info[i + 1].codepoint))
      {
	i++;
	info[i].mask = global_mask;
	_hb_glyph_info_set_unicode_props (&info[i], बफर);
	_hb_glyph_info_set_continuation (&info[i], बफर);
      }
    }
#पूर्ण_अगर
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
    अन्यथा अगर (unlikely (hb_in_ranges<hb_codepoint_t> (info[i].codepoint, 0xFF9Eu, 0xFF9Fu, 0xE0020u, 0xE007Fu)))
      _hb_glyph_info_set_continuation (&info[i], बफर);
    अन्यथा अगर (unlikely (info[i].codepoint == 0x2044u /* FRACTION SLASH */))
      बफर->scratch_flags |= HB_बफर_SCRATCH_FLAG_HAS_FRACTION_SLASH;
  }
}

अटल व्योम
hb_जोड़ो_dotted_circle (hb_बफर_t *बफर, hb_font_t *font)
{
  अगर (unlikely (बफर->flags & HB_बफर_FLAG_DO_NOT_जोड़ो_DOTTED_CIRCLE))
    वापस;

  अगर (!(बफर->flags & HB_बफर_FLAG_BOT) ||
      बफर->context_len[0] ||
      !_hb_glyph_info_is_unicode_mark (&बफर->info[0]))
    वापस;

  अगर (!font->has_glyph (0x25CCu))
    वापस;

  hb_glyph_info_t dottedcircle = {0};
  dottedcircle.codepoint = 0x25CCu;
  _hb_glyph_info_set_unicode_props (&dottedcircle, बफर);

  बफर->clear_output ();

  बफर->idx = 0;
  hb_glyph_info_t info = dottedcircle;
  info.cluster = बफर->cur().cluster;
  info.mask = बफर->cur().mask;
  (व्योम) बफर->output_info (info);

  बफर->sync ();
}

अटल व्योम
hb_form_clusters (hb_बफर_t *बफर)
{
  अगर (!(बफर->scratch_flags & HB_बफर_SCRATCH_FLAG_HAS_CONTINUATIONS))
    वापस;

  foreach_grapheme (बफर, start, end)
    बफर->merge_grapheme_clusters (start, end);
}

अटल व्योम
hb_ensure_native_direction (hb_बफर_t *बफर)
{
  hb_direction_t direction = बफर->props.direction;
  hb_direction_t horiz_dir = hb_script_get_horizontal_direction (बफर->props.script);

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
  अगर (unlikely (horiz_dir == HB_DIRECTION_RTL && direction == HB_DIRECTION_LTR))
  {
    bool found_संख्या = false, found_letter = false, found_ri = false;
    स्थिर स्वतः* info = बफर->info;
    स्थिर स्वतः गिनती = बफर->len;
    क्रम (अचिन्हित i = 0; i < गिनती; i++)
    {
      स्वतः gc = _hb_glyph_info_get_general_category (&info[i]);
      अगर (HB_UNICODE_GENERAL_CATEGORY_IS_LETTER (gc))
      {
	found_letter = true;
	अवरोध;
      }
      अन्यथा अगर (gc == HB_UNICODE_GENERAL_CATEGORY_DECIMAL_संख्या)
	found_संख्या = true;
      अन्यथा अगर (unlikely (_hb_codepoint_is_regional_indicator (info[i].codepoint)))
	found_ri = true;
    }
    अगर ((found_संख्या || found_ri) && !found_letter)
      horiz_dir = HB_DIRECTION_LTR;
  }

  /* TODO vertical:
   * The only BTT vertical script is Ogham, but it's not clear to me whether OpenType
   * Ogham fonts are supposed to be implemented BTT or not.  Need to research that
   * first. */
  अगर ((HB_DIRECTION_IS_HORIZONTAL (direction) &&
       direction != horiz_dir && HB_DIRECTION_IS_VALID (horiz_dir)) ||
      (HB_DIRECTION_IS_VERTICAL   (direction) &&
       direction != HB_DIRECTION_TTB))
  {
    _hb_ot_layout_reverse_graphemes (बफर);
    बफर->props.direction = HB_DIRECTION_REVERSE (बफर->props.direction);
  }
}


/*
 * Substitute
 */

अटल अंतरभूत व्योम
hb_ot_rotate_chars (स्थिर hb_ot_shape_context_t *c)
{
  hb_बफर_t *बफर = c->बफर;
  अचिन्हित पूर्णांक गिनती = बफर->len;
  hb_glyph_info_t *info = बफर->info;

  अगर (HB_DIRECTION_IS_BACKWARD (c->target_direction))
  {
    hb_unicode_funcs_t *unicode = बफर->unicode;
    hb_mask_t rtlm_mask = c->plan->rtlm_mask;

    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++) {
      hb_codepoint_t codepoint = unicode->mirroring (info[i].codepoint);
      अगर (unlikely (codepoint != info[i].codepoint && c->font->has_glyph (codepoint)))
	info[i].codepoint = codepoint;
      अन्यथा
	info[i].mask |= rtlm_mask;
    }
  }

#अगर_अघोषित HB_NO_VERTICAL
  अगर (HB_DIRECTION_IS_VERTICAL (c->target_direction) && !c->plan->has_vert)
  {
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++) {
      hb_codepoint_t codepoint = hb_unicode_funcs_t::vertical_char_for (info[i].codepoint);
      अगर (unlikely (codepoint != info[i].codepoint && c->font->has_glyph (codepoint)))
	info[i].codepoint = codepoint;
    }
  }
#पूर्ण_अगर
}

अटल अंतरभूत व्योम
hb_ot_shape_setup_masks_fraction (स्थिर hb_ot_shape_context_t *c)
{
#अगर_घोषित HB_NO_OT_SHAPE_FRACTIONS
  वापस;
#पूर्ण_अगर

  अगर (!(c->बफर->scratch_flags & HB_बफर_SCRATCH_FLAG_HAS_FRACTION_SLASH) ||
      !c->plan->has_frac)
    वापस;

  hb_बफर_t *बफर = c->बफर;

  hb_mask_t pre_mask, post_mask;
  अगर (HB_DIRECTION_IS_FORWARD (बफर->props.direction))
  {
    pre_mask = c->plan->numr_mask | c->plan->frac_mask;
    post_mask = c->plan->frac_mask | c->plan->dnom_mask;
  }
  अन्यथा
  {
    pre_mask = c->plan->frac_mask | c->plan->dnom_mask;
    post_mask = c->plan->numr_mask | c->plan->frac_mask;
  }

  अचिन्हित पूर्णांक गिनती = बफर->len;
  hb_glyph_info_t *info = बफर->info;
  क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
  {
    अगर (info[i].codepoint == 0x2044u) /* FRACTION SLASH */
    {
      अचिन्हित पूर्णांक start = i, end = i + 1;
      जबतक (start &&
	     _hb_glyph_info_get_general_category (&info[start - 1]) ==
	     HB_UNICODE_GENERAL_CATEGORY_DECIMAL_संख्या)
	start--;
      जबतक (end < गिनती &&
	     _hb_glyph_info_get_general_category (&info[end]) ==
	     HB_UNICODE_GENERAL_CATEGORY_DECIMAL_संख्या)
	end++;
      अगर (start == i || end == i + 1)
      {
        अगर (start == i)
	  बफर->unsafe_to_concat (start, start + 1);
	अगर (end == i + 1)
	  बफर->unsafe_to_concat (end - 1, end);
	जारी;
      }

      बफर->unsafe_to_break (start, end);

      क्रम (अचिन्हित पूर्णांक j = start; j < i; j++)
	info[j].mask |= pre_mask;
      info[i].mask |= c->plan->frac_mask;
      क्रम (अचिन्हित पूर्णांक j = i + 1; j < end; j++)
	info[j].mask |= post_mask;

      i = end - 1;
    }
  }
}

अटल अंतरभूत व्योम
hb_ot_shape_setup_masks (स्थिर hb_ot_shape_context_t *c)
{
  hb_ot_map_t *map = &c->plan->map;
  hb_बफर_t *बफर = c->बफर;

  hb_ot_shape_setup_masks_fraction (c);

  अगर (c->plan->shaper->setup_masks)
    c->plan->shaper->setup_masks (c->plan, बफर, c->font);

  क्रम (अचिन्हित पूर्णांक i = 0; i < c->अंक_user_features; i++)
  {
    स्थिर hb_feature_t *feature = &c->user_features[i];
    अगर (!(feature->start == HB_FEATURE_GLOBAL_START && feature->end == HB_FEATURE_GLOBAL_END)) {
      अचिन्हित पूर्णांक shift;
      hb_mask_t mask = map->get_mask (feature->tag, &shift);
      बफर->set_masks (feature->value << shift, mask, feature->start, feature->end);
    }
  }
}

अटल व्योम
hb_ot_zero_width_default_ignorables (स्थिर hb_बफर_t *बफर)
{
  अगर (!(बफर->scratch_flags & HB_बफर_SCRATCH_FLAG_HAS_DEFAULT_IGNORABLES) ||
      (बफर->flags & HB_बफर_FLAG_PRESERVE_DEFAULT_IGNORABLES) ||
      (बफर->flags & HB_बफर_FLAG_REMOVE_DEFAULT_IGNORABLES))
    वापस;

  अचिन्हित पूर्णांक गिनती = बफर->len;
  hb_glyph_info_t *info = बफर->info;
  hb_glyph_position_t *pos = बफर->pos;
  अचिन्हित पूर्णांक i = 0;
  क्रम (i = 0; i < गिनती; i++)
    अगर (unlikely (_hb_glyph_info_is_default_ignorable (&info[i])))
    {
      pos[i].x_advance = pos[i].y_advance = 0;
      अगर (HB_DIRECTION_IS_HORIZONTAL (बफर->props.direction))
	pos[i].x_offset = 0;
      अन्यथा
        pos[i].y_offset = 0;
    }
}

अटल व्योम
hb_ot_deal_with_variation_selectors (hb_बफर_t *बफर)
{
  अगर (!(बफर->scratch_flags & HB_बफर_SCRATCH_FLAG_HAS_VARIATION_SELECTOR_FALLBACK) ||
	बफर->not_found_variation_selector == HB_CODEPOINT_INVALID)
    वापस;

  अचिन्हित पूर्णांक गिनती = बफर->len;
  hb_glyph_info_t *info = बफर->info;
  hb_glyph_position_t *pos = बफर->pos;

  क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
  {
    अगर (_hb_glyph_info_is_variation_selector (&info[i]))
    {
      info[i].codepoint = बफर->not_found_variation_selector;
      pos[i].x_advance = pos[i].y_advance = pos[i].x_offset = pos[i].y_offset = 0;
      _hb_glyph_info_set_variation_selector (&info[i], false);
    }
  }
}

अटल व्योम
hb_ot_hide_default_ignorables (hb_बफर_t *बफर,
			       hb_font_t   *font)
{
  अगर (!(बफर->scratch_flags & HB_बफर_SCRATCH_FLAG_HAS_DEFAULT_IGNORABLES) ||
      (बफर->flags & HB_बफर_FLAG_PRESERVE_DEFAULT_IGNORABLES))
    वापस;

  अचिन्हित पूर्णांक गिनती = बफर->len;
  hb_glyph_info_t *info = बफर->info;

  hb_codepoint_t invisible = बफर->invisible;
  अगर (!(बफर->flags & HB_बफर_FLAG_REMOVE_DEFAULT_IGNORABLES) &&
      (invisible || font->get_nominal_glyph (' ', &invisible)))
  {
    /* Replace default-ignorables with a zero-advance invisible glyph. */
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      अगर (_hb_glyph_info_is_default_ignorable (&info[i]))
	info[i].codepoint = invisible;
    }
  }
  अन्यथा
    बफर->delete_glyphs_inplace (_hb_glyph_info_is_default_ignorable);
}


अटल अंतरभूत व्योम
hb_ot_map_glyphs_fast (hb_बफर_t  *बफर)
{
  /* Normalization process sets up normalizer_glyph_index(), we just copy it. */
  अचिन्हित पूर्णांक गिनती = बफर->len;
  hb_glyph_info_t *info = बफर->info;
  क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    info[i].codepoint = info[i].normalizer_glyph_सूचकांक();

  बफर->content_type = HB_बफर_CONTENT_TYPE_GLYPHS;
}

अटल अंतरभूत व्योम
hb_synthesize_glyph_classes (hb_बफर_t *बफर)
{
  अचिन्हित पूर्णांक गिनती = बफर->len;
  hb_glyph_info_t *info = बफर->info;
  क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
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
	    HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH :
	    HB_OT_LAYOUT_GLYPH_PROPS_MARK;
    _hb_glyph_info_set_glyph_props (&info[i], klass);
  }
}

अटल अंतरभूत व्योम
hb_ot_substitute_default (स्थिर hb_ot_shape_context_t *c)
{
  hb_बफर_t *बफर = c->बफर;

  hb_ot_rotate_chars (c);

  HB_बफर_ALLOCATE_VAR (बफर, normalizer_glyph_सूचकांक);

  _hb_ot_shape_normalize (c->plan, बफर, c->font);

  hb_ot_shape_setup_masks (c);

  /* This is unfortunate to go here, but necessary... */
  अगर (c->plan->fallback_mark_positioning)
    _hb_ot_shape_fallback_mark_position_recategorize_marks (c->plan, c->font, बफर);

  hb_ot_map_glyphs_fast (बफर);

  HB_बफर_DEALLOCATE_VAR (बफर, normalizer_glyph_सूचकांक);
}

अटल अंतरभूत व्योम
hb_ot_substitute_plan (स्थिर hb_ot_shape_context_t *c)
{
  hb_बफर_t *बफर = c->बफर;

#अगर_अघोषित HB_NO_AAT_SHAPE
  अगर (unlikely (c->plan->apply_morx))
  {
    hb_ot_layout_substitute_start (c->font, बफर);

    अगर (c->plan->fallback_glyph_classes)
      hb_synthesize_glyph_classes (बफर);

    hb_aat_layout_substitute (c->plan, c->font, c->बफर,
			      c->user_features, c->अंक_user_features);
    /* The buffer digest is only used by the OT lookup-apply loop;
     * without GPOS ahead, nothing consumes it. */
    अगर (c->plan->apply_gpos)
      c->बफर->update_digest ();
  }
  अन्यथा
#पूर्ण_अगर
  {
    hb_ot_layout_substitute_start_with_digest (c->font, बफर);

    अगर (c->plan->fallback_glyph_classes)
      hb_synthesize_glyph_classes (बफर);

    c->plan->substitute (c->font, बफर);
  }
}

अटल अंतरभूत व्योम
hb_ot_substitute_pre (स्थिर hb_ot_shape_context_t *c)
{
  hb_ot_substitute_default (c);

  hb_बफर_allocate_gsubgpos_vars (c->बफर);

  hb_ot_substitute_plan (c);

#अगर_अघोषित HB_NO_AAT_SHAPE
  अगर (c->plan->apply_morx && c->plan->apply_gpos)
    hb_aat_layout_remove_deleted_glyphs (c->बफर);
#पूर्ण_अगर
}

अटल अंतरभूत व्योम
hb_ot_substitute_post (स्थिर hb_ot_shape_context_t *c)
{
#अगर_अघोषित HB_NO_AAT_SHAPE
  अगर (c->plan->apply_morx && !c->plan->apply_gpos)
    hb_aat_layout_remove_deleted_glyphs (c->बफर);
#पूर्ण_अगर

  hb_ot_deal_with_variation_selectors (c->बफर);
  hb_ot_hide_default_ignorables (c->बफर, c->font);

  अगर (c->plan->shaper->postprocess_glyphs &&
    c->बफर->message(c->font, "start postprocess-glyphs")) {
    c->plan->shaper->postprocess_glyphs (c->plan, c->बफर, c->font);
    (व्योम) c->बफर->message(c->font, "end postprocess-glyphs");
  }
}


/*
 * Position
 */

अटल अंतरभूत व्योम
adjust_mark_offsets (hb_glyph_position_t *pos)
{
  pos->x_offset = hb_saturate_sub (pos->x_offset, pos->x_advance);
  pos->y_offset = hb_saturate_sub (pos->y_offset, pos->y_advance);
}

अटल अंतरभूत व्योम
zero_mark_width (hb_glyph_position_t *pos)
{
  pos->x_advance = 0;
  pos->y_advance = 0;
}

अटल अंतरभूत व्योम
zero_mark_widths_by_gdef (hb_बफर_t *बफर, bool adjust_offsets)
{
  अचिन्हित पूर्णांक गिनती = बफर->len;
  hb_glyph_info_t *info = बफर->info;
  क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    अगर (_hb_glyph_info_is_mark (&info[i]))
    {
      अगर (adjust_offsets)
	adjust_mark_offsets (&बफर->pos[i]);
      zero_mark_width (&बफर->pos[i]);
    }
}

अटल अंतरभूत व्योम
hb_ot_position_default (स्थिर hb_ot_shape_context_t *c)
{
  hb_direction_t direction = c->बफर->props.direction;
  अचिन्हित पूर्णांक गिनती = c->बफर->len;
  hb_glyph_info_t *info = c->बफर->info;
  hb_glyph_position_t *pos = c->बफर->pos;

  अगर (HB_DIRECTION_IS_HORIZONTAL (direction))
  {
    c->font->get_glyph_h_advances (गिनती, &info[0].codepoint, माप(info[0]),
				   &pos[0].x_advance, माप(pos[0]));
    // h_origin defaults to zero; only apply it if the font has it.
    अगर (c->font->has_glyph_h_origin_func () || c->font->has_glyph_h_origins_func ())
      c->font->subtract_glyph_h_origins (c->बफर);
  }
  अन्यथा
  {
    c->font->get_glyph_v_advances (गिनती, &info[0].codepoint, माप(info[0]),
				   &pos[0].y_advance, माप(pos[0]));
    // v_origin defaults to non-zero; apply even if only fallback is there.
    c->font->subtract_glyph_v_origins (c->बफर);
  }
  अगर (c->बफर->scratch_flags & HB_बफर_SCRATCH_FLAG_HAS_SPACE_FALLBACK)
    _hb_ot_shape_fallback_spaces (c->plan, c->font, c->बफर);
}

अटल अंतरभूत व्योम
hb_ot_position_plan (स्थिर hb_ot_shape_context_t *c)
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
  bool adjust_offsets_when_zeroing = c->plan->adjust_mark_positioning_when_zeroing &&
				     HB_DIRECTION_IS_FORWARD (c->बफर->props.direction);

  /* We change glyph origin to what GPOS expects (horizontal), apply GPOS, change it back. */

  // h_origin defaults to zero; only apply it if the font has it.
  अगर (c->font->has_glyph_h_origin_func () || c->font->has_glyph_h_origins_func ())
    c->font->add_glyph_h_origins (c->बफर);

  hb_ot_layout_position_start (c->font, c->बफर);

  अगर (c->plan->zero_marks)
    चयन (c->plan->shaper->zero_width_marks)
    {
      हाल HB_OT_SHAPE_ZERO_WIDTH_MARKS_BY_GDEF_EARLY:
	zero_mark_widths_by_gdef (c->बफर, adjust_offsets_when_zeroing);
	अवरोध;

      शेष:
      हाल HB_OT_SHAPE_ZERO_WIDTH_MARKS_NONE:
      हाल HB_OT_SHAPE_ZERO_WIDTH_MARKS_BY_GDEF_LATE:
	अवरोध;
    }

  c->plan->position (c->font, c->बफर);

  अगर (c->plan->zero_marks)
    चयन (c->plan->shaper->zero_width_marks)
    {
      हाल HB_OT_SHAPE_ZERO_WIDTH_MARKS_BY_GDEF_LATE:
	zero_mark_widths_by_gdef (c->बफर, adjust_offsets_when_zeroing);
	अवरोध;

      शेष:
      हाल HB_OT_SHAPE_ZERO_WIDTH_MARKS_NONE:
      हाल HB_OT_SHAPE_ZERO_WIDTH_MARKS_BY_GDEF_EARLY:
	अवरोध;
    }

  /* Finish off.  Has to follow a certain order. */
  hb_ot_layout_position_finish_advances (c->font, c->बफर);
  hb_ot_zero_width_default_ignorables (c->बफर);
  hb_ot_layout_position_finish_offsets (c->font, c->बफर);

  // h_origin defaults to zero; only apply it if the font has it.
  अगर (c->font->has_glyph_h_origin_func () || c->font->has_glyph_h_origins_func ())
    c->font->subtract_glyph_h_origins (c->बफर);

  अगर (c->plan->fallback_mark_positioning)
    _hb_ot_shape_fallback_mark_position (c->plan, c->font, c->बफर,
					 adjust_offsets_when_zeroing);
}

अटल अंतरभूत व्योम
hb_ot_position (स्थिर hb_ot_shape_context_t *c)
{
  c->बफर->clear_positions ();

  hb_ot_position_default (c);

  hb_ot_position_plan (c);

  अगर (HB_DIRECTION_IS_BACKWARD (c->बफर->props.direction))
    hb_बफर_reverse (c->बफर);

  hb_बफर_deallocate_gsubgpos_vars (c->बफर);
}

अटल अंतरभूत व्योम
hb_propagate_flags (hb_बफर_t *बफर)
{
  /* Propagate cluster-level glyph flags to be the same on all cluster glyphs.
   * Simplifies using them. */

  hb_mask_t and_mask = HB_GLYPH_FLAG_DEFINED;
  अगर ((बफर->flags & HB_बफर_FLAG_PRODUCE_UNSAFE_TO_CONCAT) == 0)
    and_mask &= ~HB_GLYPH_FLAG_UNSAFE_TO_CONCAT;

  hb_glyph_info_t *info = बफर->info;

  अगर ((बफर->flags & HB_बफर_FLAG_PRODUCE_SAFE_TO_जोड़ो_TATWEEL) == 0)
  {
    foreach_cluster (बफर, start, end)
    {
      अगर (end - start == 1)
      {
        info[start].mask &= and_mask;
	जारी;
      }

      अचिन्हित पूर्णांक mask = 0;
      क्रम (अचिन्हित पूर्णांक i = start; i < end; i++)
	mask |= info[i].mask;

      mask &= and_mask;

      क्रम (अचिन्हित पूर्णांक i = start; i < end; i++)
	info[i].mask = mask;
    }
    वापस;
  }

  /* If we are producing SAFE_TO_INSERT_TATWEEL, then do two things:
   *
   * - If the places that the Arabic shaper marked as SAFE_TO_INSERT_TATWEEL,
   *   are UNSAFE_TO_BREAK, then clear the SAFE_TO_INSERT_TATWEEL,
   * - Any place that is SAFE_TO_INSERT_TATWEEL, is also now UNSAFE_TO_BREAK.
   *
   * We couldn't make this interaction earlier. It has to be done this way.
   */
  foreach_cluster (बफर, start, end)
  {
    अचिन्हित पूर्णांक mask = 0;
    क्रम (अचिन्हित पूर्णांक i = start; i < end; i++)
      mask |= info[i].mask;

    अगर (mask & HB_GLYPH_FLAG_UNSAFE_TO_BREAK)
      mask &= ~HB_GLYPH_FLAG_SAFE_TO_जोड़ो_TATWEEL;
    अगर (mask & HB_GLYPH_FLAG_SAFE_TO_जोड़ो_TATWEEL)
      mask |= HB_GLYPH_FLAG_UNSAFE_TO_BREAK | HB_GLYPH_FLAG_UNSAFE_TO_CONCAT;

    mask &= and_mask;

    क्रम (अचिन्हित पूर्णांक i = start; i < end; i++)
      info[i].mask = mask;
  }
}

/* Pull it all together! */

अटल व्योम
hb_ot_shape_internal (hb_ot_shape_context_t *c)
{
  /* Save the original direction, we use it later. */
  c->target_direction = c->बफर->props.direction;

  hb_बफर_allocate_unicode_vars (c->बफर);

  hb_set_unicode_props (c->बफर, c->plan->map.get_global_mask ());
  hb_जोड़ो_dotted_circle (c->बफर, c->font);

  hb_form_clusters (c->बफर);

  hb_ensure_native_direction (c->बफर);

  अगर (c->plan->shaper->preprocess_text &&
      c->बफर->message(c->font, "start preprocess-text"))
  {
    c->plan->shaper->preprocess_text (c->plan, c->बफर, c->font);
    (व्योम) c->बफर->message(c->font, "end preprocess-text");
  }

  hb_ot_substitute_pre (c);
  hb_ot_position (c);
  hb_ot_substitute_post (c);

  hb_propagate_flags (c->बफर);

  hb_बफर_deallocate_unicode_vars (c->बफर);

  c->बफर->props.direction = c->target_direction;
}


hb_bool_t
_hb_ot_shape (hb_shape_plan_t    *shape_plan,
	      hb_font_t          *font,
	      hb_बफर_t        *बफर,
	      स्थिर hb_feature_t *features,
	      अचिन्हित पूर्णांक        अंक_features)
{
  hb_ot_shape_context_t c = {&shape_plan->ot, font, font->face, बफर, features, अंक_features};
  hb_ot_shape_internal (&c);

  वापस true;
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
व्योम
hb_ot_shape_plan_collect_lookups (hb_shape_plan_t *shape_plan,
				  hb_tag_t         table_tag,
				  hb_set_t        *lookup_indexes /* OUT */)
{
  shape_plan->ot.collect_lookups (table_tag, lookup_indexes);
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
अचिन्हित पूर्णांक
hb_ot_shape_plan_get_feature_tags (hb_shape_plan_t *shape_plan,
				   अचिन्हित पूर्णांक     start_offset,
				   अचिन्हित पूर्णांक    *tag_गिनती, /* IN/OUT */
				   hb_tag_t        *tags /* OUT */)
{
#अगर_अघोषित HB_NO_OT_SHAPE
  वापस shape_plan->ot.map.get_feature_tags (start_offset, tag_गिनती, tags);
#अन्यथा
  अगर (tag_गिनती)
	*tag_गिनती = 0;
  वापस 0;
#पूर्ण_अगर
}


/* TODO Move this to hb-ot-shape-normalize, make it do decompose, and make it public. */
अटल व्योम
add_char (hb_font_t          *font,
	  hb_unicode_funcs_t *unicode,
	  hb_bool_t           mirror,
	  hb_codepoint_t      u,
	  hb_set_t           *glyphs)
{
  hb_codepoint_t glyph;
  अगर (font->get_nominal_glyph (u, &glyph))
    glyphs->add (glyph);
  अगर (mirror)
  {
    hb_codepoint_t m = unicode->mirroring (u);
    अगर (m != u && font->get_nominal_glyph (m, &glyph))
      glyphs->add (glyph);
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
व्योम
hb_ot_shape_glyphs_closure (hb_font_t          *font,
			    hb_बफर_t        *बफर,
			    स्थिर hb_feature_t *features,
			    अचिन्हित पूर्णांक        अंक_features,
			    hb_set_t           *glyphs)
{
  स्थिर अक्षर *shapers[] = {"ot", nullptr};
  hb_shape_plan_t *shape_plan = hb_shape_plan_create_cached (font->face, &बफर->props,
							     features, अंक_features, shapers);

  bool mirror = hb_script_get_horizontal_direction (बफर->props.script) == HB_DIRECTION_RTL;

  अचिन्हित पूर्णांक गिनती = बफर->len;
  hb_glyph_info_t *info = बफर->info;
  क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    add_char (font, बफर->unicode, mirror, info[i].codepoint, glyphs);

  hb_set_t *lookups = hb_set_create ();
  hb_ot_shape_plan_collect_lookups (shape_plan, HB_OT_TAG_GSUB, lookups);
  hb_ot_layout_lookups_substitute_closure (font->face, lookups, glyphs);

  hb_set_destroy (lookups);

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
अचिन्हित पूर्णांक
hb_ot_shape_get_बफर_format_serial (व्योम)
{
  वापस HB_OT_SHAPE_बफर_FORMAT_SERIAL;
}

#पूर्ण_अगर

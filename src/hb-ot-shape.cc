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

#അന്തര്ഭാവ "hb.hh"

#ifndef HB_NO_OT_SHAPE

#ifdef HB_NO_OT_LAYOUT
#error "Cannot compile 'ot' shaper with HB_NO_OT_LAYOUT."
#endif

#അന്തര്ഭാവ "hb-shaper-impl.hh"

#അന്തര്ഭാവ "hb-ot-shape.hh"
#അന്തര്ഭാവ "hb-ot-shaper.hh"
#അന്തര്ഭാവ "hb-ot-shape-fallback.hh"
#അന്തര്ഭാവ "hb-ot-shape-normalize.hh"

#അന്തര്ഭാവ "hb-ot-face.hh"

#അന്തര്ഭാവ "hb-set.hh"
#അന്തര്ഭാവ "hb-unicode.hh"

#അന്തര്ഭാവ "hb-aat-layout.hh"
#അന്തര്ഭാവ "hb-ot-layout-gdef-table.hh"
#അന്തര്ഭാവ "hb-ot-stat-table.hh"


സ്ഥിതം വരിയിൽ തർക്കം
_hb_codepoint_is_regional_indicator (hb_codepoint_t u)
{ തിരികെ hb_in_range<hb_codepoint_t> (u, 0x1F1E6u, 0x1F1FFu); }

#ifndef HB_NO_AAT_SHAPE
സ്ഥിതം വരിയിൽ തർക്കം
_hb_apply_morx (hb_face_t *face, സ്ഥിരം hb_segment_properties_t &props)
{
  /* https://github.com/harfbuzz/harfbuzz/issues/2124 */
  തിരികെ hb_aat_layout_has_substitution (face) &&
	 (HB_DIRECTION_IS_HORIZONTAL (props.direction) || !hb_ot_layout_has_substitution (face));
}
#endif

/**
 * SECTION:hb-ot-shape
 * @title: hb-ot-shape
 * @short_description: OpenType shaping support
 * @include: hb-ot.h
 *
 * Support functions for OpenType shaping related queries.
 **/


സ്ഥിതം ശൂന്യം
hb_ot_shape_collect_features (hb_ot_shape_planner_t          *planner,
			      സ്ഥിരം hb_feature_t             *user_features,
			      unsigned പൂർണ്ണസംഖ്യ                    അക്കം_user_features);

hb_ot_shape_planner_t::hb_ot_shape_planner_t (hb_face_t                     *face,
					      സ്ഥിരം hb_segment_properties_t &props) :
						face (face),
						props (props),
						map (face, props)
#ifndef HB_NO_AAT_SHAPE
						, aat_map (face, props)
						, apply_morx (_hb_apply_morx (face, props))
#endif
{
  shaper = hb_ot_shaper_categorize (props.script, props.direction, map.chosen_script[0]);

  script_zero_marks = shaper->zero_width_marks != HB_OT_SHAPE_ZERO_WIDTH_MARKS_NONE;
  script_fallback_position = shaper->fallback_position;

#ifndef HB_NO_AAT_SHAPE
  /* https://github.com/harfbuzz/harfbuzz/issues/1528 */
  എങ്കിൽ (apply_morx && shaper != &_hb_ot_shaper_default)
    shaper = &_hb_ot_shaper_dumber;
#endif
}

ശൂന്യം
hb_ot_shape_planner_t::compile (hb_ot_shape_plan_t           &plan,
				സ്ഥിരം hb_ot_shape_plan_താക്കോൽ_t &key)
{
  plan.props = props;
  plan.shaper = shaper;
  map.compile (plan.map, key);
#ifndef HB_NO_AAT_SHAPE
  എങ്കിൽ (apply_morx)
    aat_map.compile (plan.aat_map);
#endif

#ifndef HB_NO_OT_SHAPE_FRACTIONS
  plan.frac_mask = plan.map.get_1_mask (HB_TAG ('f','r','a','c'));
  plan.numr_mask = plan.map.get_1_mask (HB_TAG ('n','u','m','r'));
  plan.dnom_mask = plan.map.get_1_mask (HB_TAG ('d','n','o','m'));
  plan.has_frac = plan.frac_mask || (plan.numr_mask && plan.dnom_mask);
#endif

  plan.rtlm_mask = plan.map.get_1_mask (HB_TAG ('r','t','l','m'));
  plan.has_vert = !!plan.map.get_1_mask (HB_TAG ('v','e','r','t'));

  hb_tag_t kern_tag = HB_DIRECTION_IS_HORIZONTAL (props.direction) ?
		      HB_TAG ('k','e','r','n') : HB_TAG ('v','k','r','n');
#ifndef HB_NO_OT_KERN
  plan.kern_mask = plan.map.get_mask (kern_tag);
  plan.requested_kerning = !!plan.kern_mask;
#endif

  തർക്കം has_gpos_kern = plan.map.get_feature_index (1, kern_tag) != HB_OT_LAYOUT_NO_FEATURE_INDEX;
  തർക്കം disable_gpos = plan.shaper->gpos_tag &&
		      plan.shaper->gpos_tag != plan.map.chosen_script[1];

  /*
   * Decide who provides glyph classes. GDEF or Unicode.
   */

  എങ്കിൽ (!hb_ot_layout_has_glyph_classes (face))
    plan.fallback_glyph_classes = സത്യം;

  /*
   * Decide who does substitutions. GSUB, morx, or fallback.
   */

#ifndef HB_NO_AAT_SHAPE
  plan.apply_morx = apply_morx;
#endif

  /*
   * Decide who does positioning. GPOS, kerx, kern, or fallback.
   */

#ifndef HB_NO_AAT_SHAPE
  തർക്കം has_kerx = hb_aat_layout_has_positioning (face);
  തർക്കം has_gsub = !apply_morx && hb_ot_layout_has_substitution (face);
#endif
  തർക്കം has_gpos = !disable_gpos && hb_ot_layout_has_positioning (face);
  എങ്കിൽ (അസത്യം)
    {}
#ifndef HB_NO_AAT_SHAPE
  /* Prefer GPOS over kerx if GSUB is present;
   * https://github.com/harfbuzz/harfbuzz/issues/3008 */
  അല്ലെങ്കിൽ എങ്കിൽ (has_kerx && !(has_gsub && has_gpos))
    plan.apply_kerx = സത്യം;
#endif
  അല്ലെങ്കിൽ എങ്കിൽ (has_gpos)
    plan.apply_gpos = സത്യം;

  എങ്കിൽ (!plan.apply_kerx && (!has_gpos_kern || !plan.apply_gpos))
  {
    എങ്കിൽ (അസത്യം) {}
#ifndef HB_NO_AAT_SHAPE
    അല്ലെങ്കിൽ എങ്കിൽ (has_kerx)
      plan.apply_kerx = സത്യം;
#endif
#ifndef HB_NO_OT_KERN
    അല്ലെങ്കിൽ എങ്കിൽ (hb_ot_layout_has_kerning (face))
      plan.apply_kern = script_fallback_position; // Not all shapers apply legacy `kern`
#endif
    അല്ലെങ്കിൽ {}
  }

  plan.apply_fallback_kern = script_fallback_position && !(plan.apply_gpos || plan.apply_kerx || plan.apply_kern);

  plan.zero_marks = script_zero_marks &&
		    !plan.apply_kerx &&
		    (!plan.apply_kern
#ifndef HB_NO_OT_KERN
		     || !hb_ot_layout_has_machine_kerning (face)
#endif
		    );
  plan.has_gpos_mark = !!plan.map.get_1_mask (HB_TAG ('m','a','r','k'));

  plan.adjust_mark_positioning_when_zeroing = !plan.apply_gpos &&
					      !plan.apply_kerx &&
					      (!plan.apply_kern
#ifndef HB_NO_OT_KERN
					       || !hb_ot_layout_has_cross_kerning (face)
#endif
					      );

  plan.fallback_mark_positioning = plan.adjust_mark_positioning_when_zeroing &&
				   script_fallback_position;

#ifndef HB_NO_AAT_SHAPE
  /* If we're using morx shaping, we cancel mark position adjustment because
     Apple Color Emoji assumes this will NOT be done when forming emoji sequences;
     https://github.com/harfbuzz/harfbuzz/issues/2967. */
  എങ്കിൽ (plan.apply_morx)
    plan.adjust_mark_positioning_when_zeroing = അസത്യം;

  /* According to Ned, trak is applied by default for "modern fonts", as detected by presence of STAT table. */
#ifndef HB_NO_STYLE
  plan.apply_trak = hb_aat_layout_has_tracking (face) && face->table.STAT->has_data ();
#അല്ലെങ്കിൽ
  plan.apply_trak = അസത്യം;
#endif

#endif
}

തർക്കം
hb_ot_shape_plan_t::init0 (hb_face_t                     *face,
			   സ്ഥിരം hb_shape_plan_താക്കോൽ_t     *key)
{
  map.init ();

  hb_ot_shape_planner_t planner (face,
				 key->props);

  hb_ot_shape_collect_features (&planner,
				key->user_features,
				key->അക്കം_user_features);

  planner.compile (*ഇത്, key->ot);

  എങ്കിൽ (shaper->data_create)
  {
    data = shaper->data_create (ഇത്);
    എങ്കിൽ (unlikely (!data))
    {
      map.fini ();
      തിരികെ അസത്യം;
    }
  }

  തിരികെ സത്യം;
}

ശൂന്യം
hb_ot_shape_plan_t::fini ()
{
  എങ്കിൽ (shaper->data_destroy)
    shaper->data_destroy (const_cast<ശൂന്യം *> (data));

  map.fini ();
}

ശൂന്യം
hb_ot_shape_plan_t::substitute (hb_font_t   *font,
				hb_buffer_t *buffer) സ്ഥിരം
{
  map.substitute (ഇത്, font, buffer);
}

ശൂന്യം
hb_ot_shape_plan_t::position (hb_font_t   *font,
			      hb_buffer_t *buffer) സ്ഥിരം
{
  എങ്കിൽ (ഇത്->apply_gpos)
    map.position (ഇത്, font, buffer);
#ifndef HB_NO_AAT_SHAPE
  അല്ലെങ്കിൽ എങ്കിൽ (ഇത്->apply_kerx)
    hb_aat_layout_position (ഇത്, font, buffer);
#endif

#ifndef HB_NO_OT_KERN
  എങ്കിൽ (ഇത്->apply_kern)
    hb_ot_layout_kern (ഇത്, font, buffer);
#endif
  അല്ലെങ്കിൽ എങ്കിൽ (ഇത്->apply_fallback_kern)
    _hb_ot_shape_fallback_kern (ഇത്, font, buffer);

#ifndef HB_NO_AAT_SHAPE
  എങ്കിൽ (ഇത്->apply_trak)
    hb_aat_layout_track (ഇത്, font, buffer);
#endif
}


സ്ഥിതം സ്ഥിരം hb_ot_map_feature_t
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


സ്ഥിതം സ്ഥിരം hb_ot_map_feature_t
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

സ്ഥിതം ശൂന്യം
hb_ot_shape_collect_features (hb_ot_shape_planner_t *planner,
			      സ്ഥിരം hb_feature_t    *user_features,
			      unsigned പൂർണ്ണസംഖ്യ           അക്കം_user_features)
{
  hb_ot_map_builder_t *map = &planner->map;

  map->is_simple = സത്യം;

  map->enable_feature (HB_TAG('r','v','r','n'));
  map->add_gsub_pause (nullptr);

  തിരഞ്ഞെടുപ്പ് (planner->props.direction)
  {
    അവസ്ഥ HB_DIRECTION_LTR:
      map->enable_feature (HB_TAG ('l','t','r','a'));
      map->enable_feature (HB_TAG ('l','t','r','m'));
      നിർത്തുക;
    അവസ്ഥ HB_DIRECTION_RTL:
      map->enable_feature (HB_TAG ('r','t','l','a'));
      map->add_feature (HB_TAG ('r','t','l','m'));
      നിർത്തുക;
    അവസ്ഥ HB_DIRECTION_TTB:
    അവസ്ഥ HB_DIRECTION_BTT:
    അവസ്ഥ HB_DIRECTION_INVALID:
    സ്ഥിരസ്ഥിതി:
      നിർത്തുക;
  }

#ifndef HB_NO_OT_SHAPE_FRACTIONS
  /* Automatic fractions. */
  map->add_feature (HB_TAG ('f','r','a','c'));
  map->add_feature (HB_TAG ('n','u','m','r'));
  map->add_feature (HB_TAG ('d','n','o','m'));
#endif

  /* Random! */
  map->enable_feature (HB_TAG ('r','a','n','d'), F_RANDOM, HB_OT_MAP_MAX_മൂല്യം);

  map->enable_feature (HB_TAG ('H','a','r','f')); /* Considered required. */
  map->enable_feature (HB_TAG ('H','A','R','F')); /* Considered discretionary. */

  എങ്കിൽ (planner->shaper->collect_features)
  {
    map->is_simple = അസത്യം;
    planner->shaper->collect_features (planner);
  }

  map->enable_feature (HB_TAG ('B','u','z','z')); /* Considered required. */
  map->enable_feature (HB_TAG ('B','U','Z','Z')); /* Considered discretionary. */

  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < ARRAY_LENGTH (common_features); i++)
    map->add_feature (common_features[i]);

  എങ്കിൽ (HB_DIRECTION_IS_HORIZONTAL (planner->props.direction))
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < ARRAY_LENGTH (horizontal_features); i++)
      map->add_feature (horizontal_features[i]);
  അല്ലെങ്കിൽ
  {
    /* We only apply `vert` feature. See:
     * https://github.com/harfbuzz/harfbuzz/commit/d71c0df2d17f4590d5611239577a6cb532c26528
     * https://lists.freedesktop.org/archives/harfbuzz/2013-August/003490.html */

    /* We really want to find a 'vert' feature if there's any in the font, no
     * matter which script/langsys it is listed (or not) under.
     * See various bugs referenced from:
     * https://github.com/harfbuzz/harfbuzz/issues/63 */
    map->enable_feature (HB_TAG ('v','e','r','t'), F_GLOBAL_തിരയൽ);
  }

  എങ്കിൽ (അക്കം_user_features)
    map->is_simple = അസത്യം;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_user_features; i++)
  {
    സ്ഥിരം hb_feature_t *feature = &user_features[i];
    map->add_feature (feature->tag,
		      (feature->start == HB_FEATURE_GLOBAL_START &&
		       feature->end == HB_FEATURE_GLOBAL_END) ?  F_GLOBAL : F_NONE,
		      feature->value);
  }

  എങ്കിൽ (planner->shaper->override_features)
    planner->shaper->override_features (planner);
}


/*
 * shaper face data
 */

ഘടന hb_ot_face_data_t {};

hb_ot_face_data_t *
_hb_ot_shaper_face_data_create (hb_face_t *face)
{
  തിരികെ (hb_ot_face_data_t *) HB_SHAPER_DATA_SUCCEEDED;
}

ശൂന്യം
_hb_ot_shaper_face_data_destroy (hb_ot_face_data_t *data)
{
}


/*
 * shaper font data
 */

ഘടന hb_ot_font_data_t {
  OT::hb_scalar_cache_t unused; // Just for alignment
};

hb_ot_font_data_t *
_hb_ot_shaper_font_data_create (hb_font_t *font)
{
  സ്ഥിരം OT::ItemVariationStore &var_store = font->face->table.GDEF->table->get_var_store ();
  തിരികെ (hb_ot_font_data_t *) var_store.create_cache ();
}

ശൂന്യം
_hb_ot_shaper_font_data_destroy (hb_ot_font_data_t *data)
{
  OT::ItemVariationStore::destroy_cache ((OT::hb_scalar_cache_t *) data);
}


/*
 * shaper
 */

ഘടന hb_ot_shape_context_t
{
  hb_ot_shape_plan_t *plan;
  hb_font_t *font;
  hb_face_t *face;
  hb_buffer_t  *buffer;
  സ്ഥിരം hb_feature_t *user_features;
  unsigned പൂർണ്ണസംഖ്യ        അക്കം_user_features;

  /* Transient stuff */
  hb_direction_t target_direction;
};



/* Main shaper */


/* Prepare */

സ്ഥിതം ശൂന്യം
hb_set_unicode_props (hb_buffer_t *buffer,
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
  unsigned പൂർണ്ണസംഖ്യ എണ്ണം = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
  {
    info[i].mask = global_mask;
    _hb_glyph_info_set_unicode_props (&info[i], buffer);

    എങ്കിൽ (info[i].codepoint < 0x80)
      തുടരുക;

    unsigned gen_cat = _hb_glyph_info_get_general_category (&info[i]);
    എങ്കിൽ (FLAG_UNSAFE (gen_cat) &
	(FLAG (HB_UNICODE_GENERAL_CATEGORY_LOWERCASE_LETTER) |
	 FLAG (HB_UNICODE_GENERAL_CATEGORY_UPPERCASE_LETTER) |
	 FLAG (HB_UNICODE_GENERAL_CATEGORY_TITLECASE_LETTER) |
	 FLAG (HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER) |
	 FLAG (HB_UNICODE_GENERAL_CATEGORY_SPACE_SEPARATOR)))
      തുടരുക;

    /* Marks are already set as continuation by the above line.
     * Handle Emoji_Modifier and ZWJ-continuation. */
    എങ്കിൽ (unlikely (gen_cat == HB_UNICODE_GENERAL_CATEGORY_MODIFIER_ചിഹ്നം &&
		  hb_in_range<hb_codepoint_t> (info[i].codepoint, 0x1F3FBu, 0x1F3FFu)))
    {
      _hb_glyph_info_set_continuation (&info[i], buffer);
    }
    /* Regional_Indicators are hairy as hell...
     * https://github.com/harfbuzz/harfbuzz/issues/2265 */
    അല്ലെങ്കിൽ എങ്കിൽ (unlikely (i && _hb_codepoint_is_regional_indicator (info[i].codepoint)))
    {
      എങ്കിൽ (_hb_codepoint_is_regional_indicator (info[i - 1].codepoint) &&
	  !_hb_glyph_info_is_continuation (&info[i - 1]))
	_hb_glyph_info_set_continuation (&info[i], buffer);
    }
#ifndef HB_NO_EMOJI_SEQUENCES
    അല്ലെങ്കിൽ എങ്കിൽ (unlikely (_hb_glyph_info_is_zwj (&info[i])))
    {
      _hb_glyph_info_set_continuation (&info[i], buffer);
      എങ്കിൽ (i + 1 < എണ്ണം &&
	  _hb_unicode_is_emoji_Extended_Pictographic (info[i + 1].codepoint))
      {
	i++;
	info[i].mask = global_mask;
	_hb_glyph_info_set_unicode_props (&info[i], buffer);
	_hb_glyph_info_set_continuation (&info[i], buffer);
      }
    }
#endif
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
    അല്ലെങ്കിൽ എങ്കിൽ (unlikely (hb_in_ranges<hb_codepoint_t> (info[i].codepoint, 0xFF9Eu, 0xFF9Fu, 0xE0020u, 0xE007Fu)))
      _hb_glyph_info_set_continuation (&info[i], buffer);
    അല്ലെങ്കിൽ എങ്കിൽ (unlikely (info[i].codepoint == 0x2044u /* FRACTION SLASH */))
      buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_FRACTION_SLASH;
  }
}

സ്ഥിതം ശൂന്യം
hb_തിരുകുക_dotted_circle (hb_buffer_t *buffer, hb_font_t *font)
{
  എങ്കിൽ (unlikely (buffer->flags & HB_BUFFER_FLAG_DO_NOT_തിരുകുക_DOTTED_CIRCLE))
    തിരികെ;

  എങ്കിൽ (!(buffer->flags & HB_BUFFER_FLAG_BOT) ||
      buffer->context_len[0] ||
      !_hb_glyph_info_is_unicode_mark (&buffer->info[0]))
    തിരികെ;

  എങ്കിൽ (!font->has_glyph (0x25CCu))
    തിരികെ;

  hb_glyph_info_t dottedcircle = {0};
  dottedcircle.codepoint = 0x25CCu;
  _hb_glyph_info_set_unicode_props (&dottedcircle, buffer);

  buffer->clear_output ();

  buffer->idx = 0;
  hb_glyph_info_t info = dottedcircle;
  info.cluster = buffer->cur().cluster;
  info.mask = buffer->cur().mask;
  (ശൂന്യം) buffer->output_info (info);

  buffer->സമന്വയം ();
}

സ്ഥിതം ശൂന്യം
hb_form_clusters (hb_buffer_t *buffer)
{
  എങ്കിൽ (!(buffer->scratch_flags & HB_BUFFER_SCRATCH_FLAG_HAS_CONTINUATIONS))
    തിരികെ;

  foreach_grapheme (buffer, start, end)
    buffer->merge_grapheme_clusters (start, end);
}

സ്ഥിതം ശൂന്യം
hb_ensure_native_direction (hb_buffer_t *buffer)
{
  hb_direction_t direction = buffer->props.direction;
  hb_direction_t horiz_dir = hb_script_get_horizontal_direction (buffer->props.script);

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
  എങ്കിൽ (unlikely (horiz_dir == HB_DIRECTION_RTL && direction == HB_DIRECTION_LTR))
  {
    തർക്കം found_സംഖ്യ = അസത്യം, found_letter = അസത്യം, found_ri = അസത്യം;
    സ്ഥിരം auto* info = buffer->info;
    സ്ഥിരം auto എണ്ണം = buffer->len;
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
    {
      auto gc = _hb_glyph_info_get_general_category (&info[i]);
      എങ്കിൽ (HB_UNICODE_GENERAL_CATEGORY_IS_LETTER (gc))
      {
	found_letter = സത്യം;
	നിർത്തുക;
      }
      അല്ലെങ്കിൽ എങ്കിൽ (gc == HB_UNICODE_GENERAL_CATEGORY_DECIMAL_സംഖ്യ)
	found_സംഖ്യ = സത്യം;
      അല്ലെങ്കിൽ എങ്കിൽ (unlikely (_hb_codepoint_is_regional_indicator (info[i].codepoint)))
	found_ri = സത്യം;
    }
    എങ്കിൽ ((found_സംഖ്യ || found_ri) && !found_letter)
      horiz_dir = HB_DIRECTION_LTR;
  }

  /* TODO vertical:
   * The only BTT vertical script is Ogham, but it's not clear to me whether OpenType
   * Ogham fonts are supposed to be implemented BTT or not.  Need to research that
   * first. */
  എങ്കിൽ ((HB_DIRECTION_IS_HORIZONTAL (direction) &&
       direction != horiz_dir && HB_DIRECTION_IS_VALID (horiz_dir)) ||
      (HB_DIRECTION_IS_VERTICAL   (direction) &&
       direction != HB_DIRECTION_TTB))
  {
    _hb_ot_layout_reverse_graphemes (buffer);
    buffer->props.direction = HB_DIRECTION_REVERSE (buffer->props.direction);
  }
}


/*
 * Substitute
 */

സ്ഥിതം വരിയിൽ ശൂന്യം
hb_ot_rotate_chars (സ്ഥിരം hb_ot_shape_context_t *c)
{
  hb_buffer_t *buffer = c->buffer;
  unsigned പൂർണ്ണസംഖ്യ എണ്ണം = buffer->len;
  hb_glyph_info_t *info = buffer->info;

  എങ്കിൽ (HB_DIRECTION_IS_BACKWARD (c->target_direction))
  {
    hb_unicode_funcs_t *unicode = buffer->unicode;
    hb_mask_t rtlm_mask = c->plan->rtlm_mask;

    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++) {
      hb_codepoint_t codepoint = unicode->mirroring (info[i].codepoint);
      എങ്കിൽ (unlikely (codepoint != info[i].codepoint && c->font->has_glyph (codepoint)))
	info[i].codepoint = codepoint;
      അല്ലെങ്കിൽ
	info[i].mask |= rtlm_mask;
    }
  }

#ifndef HB_NO_VERTICAL
  എങ്കിൽ (HB_DIRECTION_IS_VERTICAL (c->target_direction) && !c->plan->has_vert)
  {
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++) {
      hb_codepoint_t codepoint = hb_unicode_funcs_t::vertical_char_for (info[i].codepoint);
      എങ്കിൽ (unlikely (codepoint != info[i].codepoint && c->font->has_glyph (codepoint)))
	info[i].codepoint = codepoint;
    }
  }
#endif
}

സ്ഥിതം വരിയിൽ ശൂന്യം
hb_ot_shape_setup_masks_fraction (സ്ഥിരം hb_ot_shape_context_t *c)
{
#ifdef HB_NO_OT_SHAPE_FRACTIONS
  തിരികെ;
#endif

  എങ്കിൽ (!(c->buffer->scratch_flags & HB_BUFFER_SCRATCH_FLAG_HAS_FRACTION_SLASH) ||
      !c->plan->has_frac)
    തിരികെ;

  hb_buffer_t *buffer = c->buffer;

  hb_mask_t pre_mask, post_mask;
  എങ്കിൽ (HB_DIRECTION_IS_FORWARD (buffer->props.direction))
  {
    pre_mask = c->plan->numr_mask | c->plan->frac_mask;
    post_mask = c->plan->frac_mask | c->plan->dnom_mask;
  }
  അല്ലെങ്കിൽ
  {
    pre_mask = c->plan->frac_mask | c->plan->dnom_mask;
    post_mask = c->plan->numr_mask | c->plan->frac_mask;
  }

  unsigned പൂർണ്ണസംഖ്യ എണ്ണം = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
  {
    എങ്കിൽ (info[i].codepoint == 0x2044u) /* FRACTION SLASH */
    {
      unsigned പൂർണ്ണസംഖ്യ start = i, end = i + 1;
      ആകുമ്പോൾ (start &&
	     _hb_glyph_info_get_general_category (&info[start - 1]) ==
	     HB_UNICODE_GENERAL_CATEGORY_DECIMAL_സംഖ്യ)
	start--;
      ആകുമ്പോൾ (end < എണ്ണം &&
	     _hb_glyph_info_get_general_category (&info[end]) ==
	     HB_UNICODE_GENERAL_CATEGORY_DECIMAL_സംഖ്യ)
	end++;
      എങ്കിൽ (start == i || end == i + 1)
      {
        എങ്കിൽ (start == i)
	  buffer->unsafe_to_concat (start, start + 1);
	എങ്കിൽ (end == i + 1)
	  buffer->unsafe_to_concat (end - 1, end);
	തുടരുക;
      }

      buffer->unsafe_to_break (start, end);

      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ j = start; j < i; j++)
	info[j].mask |= pre_mask;
      info[i].mask |= c->plan->frac_mask;
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ j = i + 1; j < end; j++)
	info[j].mask |= post_mask;

      i = end - 1;
    }
  }
}

സ്ഥിതം വരിയിൽ ശൂന്യം
hb_ot_shape_setup_masks (സ്ഥിരം hb_ot_shape_context_t *c)
{
  hb_ot_map_t *map = &c->plan->map;
  hb_buffer_t *buffer = c->buffer;

  hb_ot_shape_setup_masks_fraction (c);

  എങ്കിൽ (c->plan->shaper->setup_masks)
    c->plan->shaper->setup_masks (c->plan, buffer, c->font);

  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < c->അക്കം_user_features; i++)
  {
    സ്ഥിരം hb_feature_t *feature = &c->user_features[i];
    എങ്കിൽ (!(feature->start == HB_FEATURE_GLOBAL_START && feature->end == HB_FEATURE_GLOBAL_END)) {
      unsigned പൂർണ്ണസംഖ്യ shift;
      hb_mask_t mask = map->get_mask (feature->tag, &shift);
      buffer->set_masks (feature->value << shift, mask, feature->start, feature->end);
    }
  }
}

സ്ഥിതം ശൂന്യം
hb_ot_zero_width_default_ignorables (സ്ഥിരം hb_buffer_t *buffer)
{
  എങ്കിൽ (!(buffer->scratch_flags & HB_BUFFER_SCRATCH_FLAG_HAS_DEFAULT_IGNORABLES) ||
      (buffer->flags & HB_BUFFER_FLAG_PRESERVE_DEFAULT_IGNORABLES) ||
      (buffer->flags & HB_BUFFER_FLAG_REMOVE_DEFAULT_IGNORABLES))
    തിരികെ;

  unsigned പൂർണ്ണസംഖ്യ എണ്ണം = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  hb_glyph_position_t *pos = buffer->pos;
  unsigned പൂർണ്ണസംഖ്യ i = 0;
  വേണ്ടി (i = 0; i < എണ്ണം; i++)
    എങ്കിൽ (unlikely (_hb_glyph_info_is_default_ignorable (&info[i])))
    {
      pos[i].x_advance = pos[i].y_advance = 0;
      എങ്കിൽ (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
	pos[i].x_offset = 0;
      അല്ലെങ്കിൽ
        pos[i].y_offset = 0;
    }
}

സ്ഥിതം ശൂന്യം
hb_ot_deal_with_variation_selectors (hb_buffer_t *buffer)
{
  എങ്കിൽ (!(buffer->scratch_flags & HB_BUFFER_SCRATCH_FLAG_HAS_VARIATION_SELECTOR_FALLBACK) ||
	buffer->not_found_variation_selector == HB_CODEPOINT_INVALID)
    തിരികെ;

  unsigned പൂർണ്ണസംഖ്യ എണ്ണം = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  hb_glyph_position_t *pos = buffer->pos;

  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
  {
    എങ്കിൽ (_hb_glyph_info_is_variation_selector (&info[i]))
    {
      info[i].codepoint = buffer->not_found_variation_selector;
      pos[i].x_advance = pos[i].y_advance = pos[i].x_offset = pos[i].y_offset = 0;
      _hb_glyph_info_set_variation_selector (&info[i], അസത്യം);
    }
  }
}

സ്ഥിതം ശൂന്യം
hb_ot_hide_default_ignorables (hb_buffer_t *buffer,
			       hb_font_t   *font)
{
  എങ്കിൽ (!(buffer->scratch_flags & HB_BUFFER_SCRATCH_FLAG_HAS_DEFAULT_IGNORABLES) ||
      (buffer->flags & HB_BUFFER_FLAG_PRESERVE_DEFAULT_IGNORABLES))
    തിരികെ;

  unsigned പൂർണ്ണസംഖ്യ എണ്ണം = buffer->len;
  hb_glyph_info_t *info = buffer->info;

  hb_codepoint_t invisible = buffer->invisible;
  എങ്കിൽ (!(buffer->flags & HB_BUFFER_FLAG_REMOVE_DEFAULT_IGNORABLES) &&
      (invisible || font->get_nominal_glyph (' ', &invisible)))
  {
    /* Replace default-ignorables with a zero-advance invisible glyph. */
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      എങ്കിൽ (_hb_glyph_info_is_default_ignorable (&info[i]))
	info[i].codepoint = invisible;
    }
  }
  അല്ലെങ്കിൽ
    buffer->delete_glyphs_inplace (_hb_glyph_info_is_default_ignorable);
}


സ്ഥിതം വരിയിൽ ശൂന്യം
hb_ot_map_glyphs_fast (hb_buffer_t  *buffer)
{
  /* Normalization process sets up normalizer_glyph_index(), we just copy it. */
  unsigned പൂർണ്ണസംഖ്യ എണ്ണം = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    info[i].codepoint = info[i].normalizer_glyph_index();

  buffer->content_type = HB_BUFFER_CONTENT_TYPE_GLYPHS;
}

സ്ഥിതം വരിയിൽ ശൂന്യം
hb_synthesize_glyph_classes (hb_buffer_t *buffer)
{
  unsigned പൂർണ്ണസംഖ്യ എണ്ണം = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
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

സ്ഥിതം വരിയിൽ ശൂന്യം
hb_ot_substitute_default (സ്ഥിരം hb_ot_shape_context_t *c)
{
  hb_buffer_t *buffer = c->buffer;

  hb_ot_rotate_chars (c);

  HB_BUFFER_ALLOCATE_VAR (buffer, normalizer_glyph_index);

  _hb_ot_shape_normalize (c->plan, buffer, c->font);

  hb_ot_shape_setup_masks (c);

  /* This is unfortunate to go here, but necessary... */
  എങ്കിൽ (c->plan->fallback_mark_positioning)
    _hb_ot_shape_fallback_mark_position_recategorize_marks (c->plan, c->font, buffer);

  hb_ot_map_glyphs_fast (buffer);

  HB_BUFFER_DEALLOCATE_VAR (buffer, normalizer_glyph_index);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
hb_ot_substitute_plan (സ്ഥിരം hb_ot_shape_context_t *c)
{
  hb_buffer_t *buffer = c->buffer;

#ifndef HB_NO_AAT_SHAPE
  എങ്കിൽ (unlikely (c->plan->apply_morx))
  {
    hb_ot_layout_substitute_start (c->font, buffer);

    എങ്കിൽ (c->plan->fallback_glyph_classes)
      hb_synthesize_glyph_classes (buffer);

    hb_aat_layout_substitute (c->plan, c->font, c->buffer,
			      c->user_features, c->അക്കം_user_features);
    /* The buffer digest is only used by the OT lookup-apply loop;
     * without GPOS ahead, nothing consumes it. */
    എങ്കിൽ (c->plan->apply_gpos)
      c->buffer->update_digest ();
  }
  അല്ലെങ്കിൽ
#endif
  {
    hb_ot_layout_substitute_start_with_digest (c->font, buffer);

    എങ്കിൽ (c->plan->fallback_glyph_classes)
      hb_synthesize_glyph_classes (buffer);

    c->plan->substitute (c->font, buffer);
  }
}

സ്ഥിതം വരിയിൽ ശൂന്യം
hb_ot_substitute_pre (സ്ഥിരം hb_ot_shape_context_t *c)
{
  hb_ot_substitute_default (c);

  _hb_buffer_allocate_gsubgpos_vars (c->buffer);

  hb_ot_substitute_plan (c);

#ifndef HB_NO_AAT_SHAPE
  എങ്കിൽ (c->plan->apply_morx && c->plan->apply_gpos)
    hb_aat_layout_remove_deleted_glyphs (c->buffer);
#endif
}

സ്ഥിതം വരിയിൽ ശൂന്യം
hb_ot_substitute_post (സ്ഥിരം hb_ot_shape_context_t *c)
{
#ifndef HB_NO_AAT_SHAPE
  എങ്കിൽ (c->plan->apply_morx && !c->plan->apply_gpos)
    hb_aat_layout_remove_deleted_glyphs (c->buffer);
#endif

  hb_ot_deal_with_variation_selectors (c->buffer);
  hb_ot_hide_default_ignorables (c->buffer, c->font);

  എങ്കിൽ (c->plan->shaper->postprocess_glyphs &&
    c->buffer->message(c->font, "start postprocess-glyphs")) {
    c->plan->shaper->postprocess_glyphs (c->plan, c->buffer, c->font);
    (ശൂന്യം) c->buffer->message(c->font, "end postprocess-glyphs");
  }
}


/*
 * Position
 */

സ്ഥിതം വരിയിൽ ശൂന്യം
adjust_mark_offsets (hb_glyph_position_t *pos)
{
  pos->x_offset = hb_saturate_sub (pos->x_offset, pos->x_advance);
  pos->y_offset = hb_saturate_sub (pos->y_offset, pos->y_advance);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
zero_mark_width (hb_glyph_position_t *pos)
{
  pos->x_advance = 0;
  pos->y_advance = 0;
}

സ്ഥിതം വരിയിൽ ശൂന്യം
zero_mark_widths_by_gdef (hb_buffer_t *buffer, തർക്കം adjust_offsets)
{
  unsigned പൂർണ്ണസംഖ്യ എണ്ണം = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    എങ്കിൽ (_hb_glyph_info_is_mark (&info[i]))
    {
      എങ്കിൽ (adjust_offsets)
	adjust_mark_offsets (&buffer->pos[i]);
      zero_mark_width (&buffer->pos[i]);
    }
}

സ്ഥിതം വരിയിൽ ശൂന്യം
hb_ot_position_default (സ്ഥിരം hb_ot_shape_context_t *c)
{
  hb_direction_t direction = c->buffer->props.direction;
  unsigned പൂർണ്ണസംഖ്യ എണ്ണം = c->buffer->len;
  hb_glyph_info_t *info = c->buffer->info;
  hb_glyph_position_t *pos = c->buffer->pos;

  എങ്കിൽ (HB_DIRECTION_IS_HORIZONTAL (direction))
  {
    c->font->get_glyph_h_advances (എണ്ണം, &info[0].codepoint, വലുപ്പം(info[0]),
				   &pos[0].x_advance, വലുപ്പം(pos[0]));
    // h_origin defaults to zero; only apply it if the font has it.
    എങ്കിൽ (c->font->has_glyph_h_origin_func () || c->font->has_glyph_h_origins_func ())
      c->font->subtract_glyph_h_origins (c->buffer);
  }
  അല്ലെങ്കിൽ
  {
    c->font->get_glyph_v_advances (എണ്ണം, &info[0].codepoint, വലുപ്പം(info[0]),
				   &pos[0].y_advance, വലുപ്പം(pos[0]));
    // v_origin defaults to non-zero; apply even if only fallback is there.
    c->font->subtract_glyph_v_origins (c->buffer);
  }
  എങ്കിൽ (c->buffer->scratch_flags & HB_BUFFER_SCRATCH_FLAG_HAS_SPACE_FALLBACK)
    _hb_ot_shape_fallback_spaces (c->plan, c->font, c->buffer);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
hb_ot_position_plan (സ്ഥിരം hb_ot_shape_context_t *c)
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
  തർക്കം adjust_offsets_when_zeroing = c->plan->adjust_mark_positioning_when_zeroing &&
				     HB_DIRECTION_IS_FORWARD (c->buffer->props.direction);

  /* We change glyph origin to what GPOS expects (horizontal), apply GPOS, change it back. */

  // h_origin defaults to zero; only apply it if the font has it.
  എങ്കിൽ (c->font->has_glyph_h_origin_func () || c->font->has_glyph_h_origins_func ())
    c->font->add_glyph_h_origins (c->buffer);

  hb_ot_layout_position_start (c->font, c->buffer);

  എങ്കിൽ (c->plan->zero_marks)
    തിരഞ്ഞെടുപ്പ് (c->plan->shaper->zero_width_marks)
    {
      അവസ്ഥ HB_OT_SHAPE_ZERO_WIDTH_MARKS_BY_GDEF_EARLY:
	zero_mark_widths_by_gdef (c->buffer, adjust_offsets_when_zeroing);
	നിർത്തുക;

      സ്ഥിരസ്ഥിതി:
      അവസ്ഥ HB_OT_SHAPE_ZERO_WIDTH_MARKS_NONE:
      അവസ്ഥ HB_OT_SHAPE_ZERO_WIDTH_MARKS_BY_GDEF_LATE:
	നിർത്തുക;
    }

  c->plan->position (c->font, c->buffer);

  എങ്കിൽ (c->plan->zero_marks)
    തിരഞ്ഞെടുപ്പ് (c->plan->shaper->zero_width_marks)
    {
      അവസ്ഥ HB_OT_SHAPE_ZERO_WIDTH_MARKS_BY_GDEF_LATE:
	zero_mark_widths_by_gdef (c->buffer, adjust_offsets_when_zeroing);
	നിർത്തുക;

      സ്ഥിരസ്ഥിതി:
      അവസ്ഥ HB_OT_SHAPE_ZERO_WIDTH_MARKS_NONE:
      അവസ്ഥ HB_OT_SHAPE_ZERO_WIDTH_MARKS_BY_GDEF_EARLY:
	നിർത്തുക;
    }

  /* Finish off.  Has to follow a certain order. */
  hb_ot_layout_position_finish_advances (c->font, c->buffer);
  hb_ot_zero_width_default_ignorables (c->buffer);
  hb_ot_layout_position_finish_offsets (c->font, c->buffer);

  // h_origin defaults to zero; only apply it if the font has it.
  എങ്കിൽ (c->font->has_glyph_h_origin_func () || c->font->has_glyph_h_origins_func ())
    c->font->subtract_glyph_h_origins (c->buffer);

  എങ്കിൽ (c->plan->fallback_mark_positioning)
    _hb_ot_shape_fallback_mark_position (c->plan, c->font, c->buffer,
					 adjust_offsets_when_zeroing);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
hb_ot_position (സ്ഥിരം hb_ot_shape_context_t *c)
{
  c->buffer->clear_positions ();

  hb_ot_position_default (c);

  hb_ot_position_plan (c);

  എങ്കിൽ (HB_DIRECTION_IS_BACKWARD (c->buffer->props.direction))
    hb_buffer_reverse (c->buffer);

  _hb_buffer_deallocate_gsubgpos_vars (c->buffer);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
hb_propagate_flags (hb_buffer_t *buffer)
{
  /* Propagate cluster-level glyph flags to be the same on all cluster glyphs.
   * Simplifies using them. */

  hb_mask_t and_mask = HB_GLYPH_FLAG_DEFINED;
  എങ്കിൽ ((buffer->flags & HB_BUFFER_FLAG_PRODUCE_UNSAFE_TO_CONCAT) == 0)
    and_mask &= ~HB_GLYPH_FLAG_UNSAFE_TO_CONCAT;

  hb_glyph_info_t *info = buffer->info;

  എങ്കിൽ ((buffer->flags & HB_BUFFER_FLAG_PRODUCE_SAFE_TO_തിരുകുക_TATWEEL) == 0)
  {
    foreach_cluster (buffer, start, end)
    {
      എങ്കിൽ (end - start == 1)
      {
        info[start].mask &= and_mask;
	തുടരുക;
      }

      unsigned പൂർണ്ണസംഖ്യ mask = 0;
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = start; i < end; i++)
	mask |= info[i].mask;

      mask &= and_mask;

      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = start; i < end; i++)
	info[i].mask = mask;
    }
    തിരികെ;
  }

  /* If we are producing SAFE_TO_INSERT_TATWEEL, then do two things:
   *
   * - If the places that the Arabic shaper marked as SAFE_TO_INSERT_TATWEEL,
   *   are UNSAFE_TO_BREAK, then clear the SAFE_TO_INSERT_TATWEEL,
   * - Any place that is SAFE_TO_INSERT_TATWEEL, is also now UNSAFE_TO_BREAK.
   *
   * We couldn't make this interaction earlier. It has to be done this way.
   */
  foreach_cluster (buffer, start, end)
  {
    unsigned പൂർണ്ണസംഖ്യ mask = 0;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = start; i < end; i++)
      mask |= info[i].mask;

    എങ്കിൽ (mask & HB_GLYPH_FLAG_UNSAFE_TO_BREAK)
      mask &= ~HB_GLYPH_FLAG_SAFE_TO_തിരുകുക_TATWEEL;
    എങ്കിൽ (mask & HB_GLYPH_FLAG_SAFE_TO_തിരുകുക_TATWEEL)
      mask |= HB_GLYPH_FLAG_UNSAFE_TO_BREAK | HB_GLYPH_FLAG_UNSAFE_TO_CONCAT;

    mask &= and_mask;

    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = start; i < end; i++)
      info[i].mask = mask;
  }
}

/* Pull it all together! */

സ്ഥിതം ശൂന്യം
hb_ot_shape_internal (hb_ot_shape_context_t *c)
{
  /* Save the original direction, we use it later. */
  c->target_direction = c->buffer->props.direction;

  _hb_buffer_allocate_unicode_vars (c->buffer);

  hb_set_unicode_props (c->buffer, c->plan->map.get_global_mask ());
  hb_തിരുകുക_dotted_circle (c->buffer, c->font);

  hb_form_clusters (c->buffer);

  hb_ensure_native_direction (c->buffer);

  എങ്കിൽ (c->plan->shaper->preprocess_text &&
      c->buffer->message(c->font, "start preprocess-text"))
  {
    c->plan->shaper->preprocess_text (c->plan, c->buffer, c->font);
    (ശൂന്യം) c->buffer->message(c->font, "end preprocess-text");
  }

  hb_ot_substitute_pre (c);
  hb_ot_position (c);
  hb_ot_substitute_post (c);

  hb_propagate_flags (c->buffer);

  _hb_buffer_deallocate_unicode_vars (c->buffer);

  c->buffer->props.direction = c->target_direction;
}


hb_bool_t
_hb_ot_shape (hb_shape_plan_t    *shape_plan,
	      hb_font_t          *font,
	      hb_buffer_t        *buffer,
	      സ്ഥിരം hb_feature_t *features,
	      unsigned പൂർണ്ണസംഖ്യ        അക്കം_features)
{
  hb_ot_shape_context_t c = {&shape_plan->ot, font, font->face, buffer, features, അക്കം_features};
  hb_ot_shape_internal (&c);

  തിരികെ സത്യം;
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
ശൂന്യം
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
unsigned പൂർണ്ണസംഖ്യ
hb_ot_shape_plan_get_feature_tags (hb_shape_plan_t *shape_plan,
				   unsigned പൂർണ്ണസംഖ്യ     start_offset,
				   unsigned പൂർണ്ണസംഖ്യ    *tag_എണ്ണം, /* IN/OUT */
				   hb_tag_t        *tags /* OUT */)
{
#ifndef HB_NO_OT_SHAPE
  തിരികെ shape_plan->ot.map.get_feature_tags (start_offset, tag_എണ്ണം, tags);
#അല്ലെങ്കിൽ
  എങ്കിൽ (tag_എണ്ണം)
	*tag_എണ്ണം = 0;
  തിരികെ 0;
#endif
}


/* TODO Move this to hb-ot-shape-normalize, make it do decompose, and make it public. */
സ്ഥിതം ശൂന്യം
add_char (hb_font_t          *font,
	  hb_unicode_funcs_t *unicode,
	  hb_bool_t           mirror,
	  hb_codepoint_t      u,
	  hb_set_t           *glyphs)
{
  hb_codepoint_t glyph;
  എങ്കിൽ (font->get_nominal_glyph (u, &glyph))
    glyphs->add (glyph);
  എങ്കിൽ (mirror)
  {
    hb_codepoint_t m = unicode->mirroring (u);
    എങ്കിൽ (m != u && font->get_nominal_glyph (m, &glyph))
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
ശൂന്യം
hb_ot_shape_glyphs_closure (hb_font_t          *font,
			    hb_buffer_t        *buffer,
			    സ്ഥിരം hb_feature_t *features,
			    unsigned പൂർണ്ണസംഖ്യ        അക്കം_features,
			    hb_set_t           *glyphs)
{
  സ്ഥിരം അക്ഷരം *shapers[] = {"ot", nullptr};
  hb_shape_plan_t *shape_plan = hb_shape_plan_create_cached (font->face, &buffer->props,
							     features, അക്കം_features, shapers);

  തർക്കം mirror = hb_script_get_horizontal_direction (buffer->props.script) == HB_DIRECTION_RTL;

  unsigned പൂർണ്ണസംഖ്യ എണ്ണം = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    add_char (font, buffer->unicode, mirror, info[i].codepoint, glyphs);

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
unsigned പൂർണ്ണസംഖ്യ
hb_ot_shape_get_buffer_format_serial (ശൂന്യം)
{
  തിരികെ HB_OT_SHAPE_BUFFER_FORMAT_SERIAL;
}

#endif

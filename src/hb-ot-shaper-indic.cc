/*
 * Copyright © 2011,2012  Google, Inc.
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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPE

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shaper-indic.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shaper-indic-machine.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shaper-vowel-constraints.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout.hh"


/*
 * Indic shaper.
 */


ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
ਸੈੱਟ_indic_properties (hb_glyph_info_t &info)
{
  hb_codepoint_t u = info.codepoint;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ type = hb_indic_get_categories (u);

  info.indic_category() = (indic_category_t) (type & 0xFFu);
  info.indic_position() = (indic_position_t) (type >> 8);
}


ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
is_one_ਦਾ (ਅਚਲ hb_glyph_info_t &info, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
  /* If it ligated, all bets are off. */
  ਜੇ (_hb_glyph_info_ligated (&info)) ਵਾਪਸ ਝੂਠ;
  ਵਾਪਸ !!(FLAG_UNSAFE (info.indic_category()) & flags);
}

/* Note:
 *
 * We treat Vowels and placeholders as if they were consonants.  This is safe because Vowels
 * cannot happen in a consonant syllable.  The plus side however is, we can call the
 * consonant syllable logic from the vowel syllable function and get it all right!
 *
 * Keep in sync with consonant_categories in the generator. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CONSONANT_FLAGS_INDIC (FLAG (I_Cat(C)) | FLAG (I_Cat(CS)) | FLAG (I_Cat(Ra)) | FLAG (I_Cat(CM)) | FLAG (I_Cat(V)) | FLAG (I_Cat(PLACEHOLDER)) | FLAG (I_Cat(DOTTEDCIRCLE)))

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
is_consonant (ਅਚਲ hb_glyph_info_t &info)
{
  ਵਾਪਸ is_one_ਦਾ (info, CONSONANT_FLAGS_INDIC);
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JOINER_FLAGS (FLAG (I_Cat(ZWJ)) | FLAG (I_Cat(ZWNJ)))

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
is_joiner (ਅਚਲ hb_glyph_info_t &info)
{
  ਵਾਪਸ is_one_ਦਾ (info, JOINER_FLAGS);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
is_halant (ਅਚਲ hb_glyph_info_t &info)
{
  ਵਾਪਸ is_one_ਦਾ (info, FLAG (I_Cat(H)));
}

ਬਣਤਰ hb_indic_would_substitute_feature_t
{
  ਖਾਲੀ init (ਅਚਲ hb_ot_map_t *map, hb_ਟੈਗ_t feature_ਟੈਗ, ਤਾਰਕਿਕ zero_context_)
  {
    zero_context = zero_context_;
    lookups = map->get_stage_lookups (0/*GSUB*/,
				      map->get_feature_stage (0/*GSUB*/, feature_ਟੈਗ));
  }

  ਤਾਰਕਿਕ would_substitute (ਅਚਲ hb_codepoint_t *glyphs,
			 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ          glyphs_ਗਿਣਤੀ,
			 hb_face_t            *face) ਅਚਲ
  {
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &lookup : lookups)
      ਜੇ (hb_ot_layout_lookup_would_substitute (face, lookup.ਸੂਚਕਾਂਕ_2, glyphs, glyphs_ਗਿਣਤੀ, zero_context))
	ਵਾਪਸ ਸੱਚ;
    ਵਾਪਸ ਝੂਠ;
  }

  ਨਿਜੀ:
  hb_array_t<ਅਚਲ hb_ot_map_t::lookup_map_t> lookups;
  ਤਾਰਕਿਕ zero_context;
};


/*
 * Indic configurations.  Note that we do not want to keep every single script-specific
 * behavior in these tables necessarily.  This should mainly be used for per-script
 * properties that are cheaper keeping here, than in the code.  Ie. if, say, one and
 * only one script has an exception, that one script can be if'ed directly in the code,
 * instead of adding a new flag in these structs.
 */

ਗਿਣਤੀ reph_position_t {
  REPH_POS_AFTER_MAIN  = POS_AFTER_MAIN,
  REPH_POS_BEFORE_SUB  = POS_BEFORE_SUB,
  REPH_POS_AFTER_SUB   = POS_AFTER_SUB,
  REPH_POS_BEFORE_POST = POS_BEFORE_POST,
  REPH_POS_AFTER_POST  = POS_AFTER_POST
};
ਗਿਣਤੀ reph_ਬਹੁਲਕ_t {
  REPH_ਬਹੁਲਕ_IMPLICIT,  /* Reph formed out of initial Ra,H sequence. */
  REPH_ਬਹੁਲਕ_EXPLICIT,  /* Reph formed out of initial Ra,H,ZWJ sequence. */
  REPH_ਬਹੁਲਕ_LOG_REPHA  /* Encoded Repha character, needs reordering. */
};
ਗਿਣਤੀ blwf_ਬਹੁਲਕ_t {
  BLWF_ਬਹੁਲਕ_PRE_AND_POST, /* Below-forms feature applied to pre-base and post-base. */
  BLWF_ਬਹੁਲਕ_POST_ONLY     /* Below-forms feature applied to post-base only. */
};
ਬਣਤਰ indic_config_t
{
  hb_script_t     script;
  ਤਾਰਕਿਕ            has_old_spec;
  hb_codepoint_t  virama;
  reph_position_t reph_pos;
  reph_ਬਹੁਲਕ_t     reph_ਬਹੁਲਕ;
  blwf_ਬਹੁਲਕ_t     blwf_ਬਹੁਲਕ;
};

ਸਥਿਰ ਅਚਲ indic_config_t indic_configs[] =
{
  /* Default.  Should be first. */
  {HB_SCRIPT_INVALID,	ਝੂਠ,      0,REPH_POS_BEFORE_POST,REPH_ਬਹੁਲਕ_IMPLICIT, BLWF_ਬਹੁਲਕ_PRE_AND_POST},
  {HB_SCRIPT_DEVANAGARI,ਸੱਚ, 0x094Du,REPH_POS_BEFORE_POST,REPH_ਬਹੁਲਕ_IMPLICIT, BLWF_ਬਹੁਲਕ_PRE_AND_POST},
  {HB_SCRIPT_BENGALI,	ਸੱਚ, 0x09CDu,REPH_POS_AFTER_SUB,  REPH_ਬਹੁਲਕ_IMPLICIT, BLWF_ਬਹੁਲਕ_PRE_AND_POST},
  {HB_SCRIPT_GURMUKHI,	ਸੱਚ, 0x0A4Du,REPH_POS_BEFORE_SUB, REPH_ਬਹੁਲਕ_IMPLICIT, BLWF_ਬਹੁਲਕ_PRE_AND_POST},
  {HB_SCRIPT_GUJARATI,	ਸੱਚ, 0x0ACDu,REPH_POS_BEFORE_POST,REPH_ਬਹੁਲਕ_IMPLICIT, BLWF_ਬਹੁਲਕ_PRE_AND_POST},
  {HB_SCRIPT_ORIYA,	ਸੱਚ, 0x0B4Du,REPH_POS_AFTER_MAIN, REPH_ਬਹੁਲਕ_IMPLICIT, BLWF_ਬਹੁਲਕ_PRE_AND_POST},
  {HB_SCRIPT_TAMIL,	ਸੱਚ, 0x0BCDu,REPH_POS_AFTER_POST, REPH_ਬਹੁਲਕ_IMPLICIT, BLWF_ਬਹੁਲਕ_PRE_AND_POST},
  {HB_SCRIPT_TELUGU,	ਸੱਚ, 0x0C4Du,REPH_POS_AFTER_POST, REPH_ਬਹੁਲਕ_EXPLICIT, BLWF_ਬਹੁਲਕ_POST_ONLY},
  {HB_SCRIPT_KANNADA,	ਸੱਚ, 0x0CCDu,REPH_POS_AFTER_POST, REPH_ਬਹੁਲਕ_IMPLICIT, BLWF_ਬਹੁਲਕ_POST_ONLY},
  {HB_SCRIPT_MALAYALAM,	ਸੱਚ, 0x0D4Du,REPH_POS_AFTER_MAIN, REPH_ਬਹੁਲਕ_LOG_REPHA,BLWF_ਬਹੁਲਕ_PRE_AND_POST},
};


ਸਥਿਰ ਅਚਲ hb_ot_map_feature_t
indic_features[] =
{
  /*
   * Basic features.
   * These features are applied in order, one at a time, after initial_reordering,
   * constrained to the syllable.
   */
  {HB_ਟੈਗ('n','u','k','t'), F_GLOBAL_MANUAL_JOINERS | F_ਪ੍ਰਤੀ_SYLLABLE},
  {HB_ਟੈਗ('a','k','h','n'), F_GLOBAL_MANUAL_JOINERS | F_ਪ੍ਰਤੀ_SYLLABLE},
  {HB_ਟੈਗ('r','p','h','f'),        F_MANUAL_JOINERS | F_ਪ੍ਰਤੀ_SYLLABLE},
  {HB_ਟੈਗ('r','k','r','f'), F_GLOBAL_MANUAL_JOINERS | F_ਪ੍ਰਤੀ_SYLLABLE},
  {HB_ਟੈਗ('p','r','e','f'),        F_MANUAL_JOINERS | F_ਪ੍ਰਤੀ_SYLLABLE},
  {HB_ਟੈਗ('b','l','w','f'),        F_MANUAL_JOINERS | F_ਪ੍ਰਤੀ_SYLLABLE},
  {HB_ਟੈਗ('a','b','v','f'),        F_MANUAL_JOINERS | F_ਪ੍ਰਤੀ_SYLLABLE},
  {HB_ਟੈਗ('h','a','l','f'),        F_MANUAL_JOINERS | F_ਪ੍ਰਤੀ_SYLLABLE},
  {HB_ਟੈਗ('p','s','t','f'),        F_MANUAL_JOINERS | F_ਪ੍ਰਤੀ_SYLLABLE},
  {HB_ਟੈਗ('v','a','t','u'), F_GLOBAL_MANUAL_JOINERS | F_ਪ੍ਰਤੀ_SYLLABLE},
  {HB_ਟੈਗ('c','j','c','t'), F_GLOBAL_MANUAL_JOINERS | F_ਪ੍ਰਤੀ_SYLLABLE},
  /*
   * Other features.
   * These features are applied all at once, after final_reordering, constrained
   * to the syllable.
   * Default Bengali font in Windows for example has intermixed
   * lookups for init,pres,abvs,blws features.
   */
  {HB_ਟੈਗ('i','n','i','t'),        F_MANUAL_JOINERS | F_ਪ੍ਰਤੀ_SYLLABLE},
  {HB_ਟੈਗ('p','r','e','s'), F_GLOBAL_MANUAL_JOINERS | F_ਪ੍ਰਤੀ_SYLLABLE},
  {HB_ਟੈਗ('a','b','v','s'), F_GLOBAL_MANUAL_JOINERS | F_ਪ੍ਰਤੀ_SYLLABLE},
  {HB_ਟੈਗ('b','l','w','s'), F_GLOBAL_MANUAL_JOINERS | F_ਪ੍ਰਤੀ_SYLLABLE},
  {HB_ਟੈਗ('p','s','t','s'), F_GLOBAL_MANUAL_JOINERS | F_ਪ੍ਰਤੀ_SYLLABLE},
  {HB_ਟੈਗ('h','a','l','n'), F_GLOBAL_MANUAL_JOINERS | F_ਪ੍ਰਤੀ_SYLLABLE},
};

/*
 * Must be in the same order as the indic_features array.
 */
ਗਿਣਤੀ {
  _INDIC_NUKT,
  _INDIC_AKHN,
  INDIC_RPHF,
  _INDIC_RKRF,
  INDIC_PREF,
  INDIC_BLWF,
  INDIC_ABVF,
  INDIC_HALF,
  INDIC_PSTF,
  _INDIC_VATU,
  _INDIC_CJCT,

  INDIC_INIT,
  _INDIC_PRES,
  _INDIC_ABVS,
  _INDIC_BLWS,
  _INDIC_PSTS,
  _INDIC_HALN,

  INDIC_ਅੰਕ_FEATURES,
  INDIC_BASIC_FEATURES = INDIC_INIT, /* Don't forget to update this! */
};

ਸਥਿਰ ਤਾਰਕਿਕ
ਤਿਆਰੀ_syllables_indic (ਅਚਲ hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_ਬਫਰ_t *ਬਫਰ);
ਸਥਿਰ ਤਾਰਕਿਕ
initial_reordering_indic (ਅਚਲ hb_ot_shape_plan_t *plan,
			  hb_font_t *font,
			  hb_ਬਫਰ_t *ਬਫਰ);
ਸਥਿਰ ਤਾਰਕਿਕ
final_reordering_indic (ਅਚਲ hb_ot_shape_plan_t *plan,
			hb_font_t *font,
			hb_ਬਫਰ_t *ਬਫਰ);

ਸਥਿਰ ਖਾਲੀ
collect_features_indic (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  /* Do this before any lookups have been applied. */
  map->ਜੋੜੋ_gsub_pause (ਤਿਆਰੀ_syllables_indic);

  map->enable_feature (HB_ਟੈਗ('l','o','c','l'), F_ਪ੍ਰਤੀ_SYLLABLE);
  /* The Indic specs do not require ccmp, but we apply it here since if
   * there is a use of it, it's typically at the beginning. */
  map->enable_feature (HB_ਟੈਗ('c','c','m','p'), F_ਪ੍ਰਤੀ_SYLLABLE);


  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0;
  map->ਜੋੜੋ_gsub_pause (initial_reordering_indic);

  ਲਈ (; i < INDIC_BASIC_FEATURES; i++) {
    map->ਜੋੜੋ_feature (indic_features[i]);
    map->ਜੋੜੋ_gsub_pause (ਖਾਲੀ_ਸੂਚਕ);
  }

  map->ਜੋੜੋ_gsub_pause (final_reordering_indic);

  ਲਈ (; i < INDIC_ਅੰਕ_FEATURES; i++)
    map->ਜੋੜੋ_feature (indic_features[i]);
}

ਸਥਿਰ ਖਾਲੀ
override_features_indic (hb_ot_shape_planner_t *plan)
{
  plan->map.disable_feature (HB_ਟੈਗ('l','i','g','a'));
  plan->map.ਜੋੜੋ_gsub_pause (hb_syllabic_clear_var); // Don't need syllables anymore, use stop to free buffer var
}


ਬਣਤਰ indic_shape_plan_t
{
  ਤਾਰਕਿਕ ਲੱਦੋ_virama_glyph (hb_font_t *font, hb_codepoint_t *pglyph) ਅਚਲ
  {
    hb_codepoint_t glyph = virama_glyph;
    ਜੇ (unlikely (glyph == (hb_codepoint_t) -1))
    {
      ਜੇ (!config->virama || !font->get_nominal_glyph (config->virama, &glyph))
	glyph = 0;
      /* Technically speaking, the spec says we should apply 'locl' to virama too.
       * Maybe one day... */

      /* Our get_nominal_glyph() function needs a font, so we can't get the virama glyph
       * during shape planning...  Instead, overwrite it here. */
      virama_glyph = (ਪੂਰਨ) glyph;
    }

    *pglyph = glyph;
    ਵਾਪਸ glyph != 0;
  }

  ਅਚਲ indic_config_t *config;

  ਤਾਰਕਿਕ is_old_spec;
  ਬਦਲਣਯੋਗ hb_atomic_t<hb_codepoint_t> virama_glyph;

  hb_indic_would_substitute_feature_t rphf;
  hb_indic_would_substitute_feature_t pref;
  hb_indic_would_substitute_feature_t blwf;
  hb_indic_would_substitute_feature_t pstf;
  hb_indic_would_substitute_feature_t vatu;

  hb_ਨਕਾਬ_t ਨਕਾਬ_array[INDIC_ਅੰਕ_FEATURES];
};

ਸਥਿਰ ਖਾਲੀ *
ਅੰਕੜੇ_create_indic (ਅਚਲ hb_ot_shape_plan_t *plan)
{
  indic_shape_plan_t *indic_plan = (indic_shape_plan_t *) hb_calloc (1, ਆਕਾਰ (indic_shape_plan_t));
  ਜੇ (unlikely (!indic_plan))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  indic_plan->config = &indic_configs[0];
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 1; i < ARRAY_ਲੰਮਾਈ (indic_configs); i++)
    ਜੇ (plan->props.script == indic_configs[i].script) {
      indic_plan->config = &indic_configs[i];
      ਰੋਕੋ;
    }

  indic_plan->is_old_spec = indic_plan->config->has_old_spec && ((plan->map.chosen_script[0] & 0x000000FFu) != '2');
  indic_plan->virama_glyph = -1;

  /* Use zero-context would_substitute() matching for new-spec of the main
   * Indic scripts, and scripts with one spec only, but not for old-specs.
   * The new-spec for all dual-spec scripts says zero-context matching happens.
   *
   * However, testing with Malayalam shows that old and new spec both allow
   * context.  Testing with Bengali new-spec however shows that it doesn't.
   * So, the heuristic here is the way it is.  It should *only* be changed,
   * as we discover more cases of what Windows does.  DON'T TOUCH OTHERWISE.
   */
  ਤਾਰਕਿਕ zero_context = !indic_plan->is_old_spec && plan->props.script != HB_SCRIPT_MALAYALAM;
  indic_plan->rphf.init (&plan->map, HB_ਟੈਗ('r','p','h','f'), zero_context);
  indic_plan->pref.init (&plan->map, HB_ਟੈਗ('p','r','e','f'), zero_context);
  indic_plan->blwf.init (&plan->map, HB_ਟੈਗ('b','l','w','f'), zero_context);
  indic_plan->pstf.init (&plan->map, HB_ਟੈਗ('p','s','t','f'), zero_context);
  indic_plan->vatu.init (&plan->map, HB_ਟੈਗ('v','a','t','u'), zero_context);

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ARRAY_ਲੰਮਾਈ (indic_plan->ਨਕਾਬ_array); i++)
    indic_plan->ਨਕਾਬ_array[i] = (indic_features[i].flags & F_GLOBAL) ?
				 0 : plan->map.get_1_ਨਕਾਬ (indic_features[i].ਟੈਗ_2);

  ਵਾਪਸ indic_plan;
}

ਸਥਿਰ ਖਾਲੀ
ਅੰਕੜੇ_destroy_indic (ਖਾਲੀ *ਅੰਕੜੇ)
{
  hb_free (ਅੰਕੜੇ);
}

ਸਥਿਰ indic_position_t
consonant_position_from_face (ਅਚਲ indic_shape_plan_t *indic_plan,
			      ਅਚਲ hb_codepoint_t consonant,
			      ਅਚਲ hb_codepoint_t virama,
			      hb_face_t *face)
{
  /* For old-spec, the order of glyphs is Consonant,Virama,
   * whereas for new-spec, it's Virama,Consonant.  However,
   * some broken fonts (like Free Sans) simply copied lookups
   * from old-spec to new-spec without modification.
   * And oddly enough, Uniscribe seems to respect those lookups.
   * Eg. in the sequence U+0924,U+094D,U+0930, Uniscribe finds
   * base at 0.  The font however, only has lookups matching
   * 930,94D in 'blwf', not the expected 94D,930 (with new-spec
   * table).  As such, we simply match both sequences.  Seems
   * to work.
   *
   * Vatu is done as well, for:
   * https://github.com/harfbuzz/harfbuzz/issues/1587
   */
  hb_codepoint_t glyphs[3] = {virama, consonant, virama};
  ਜੇ (indic_plan->blwf.would_substitute (glyphs  , 2, face) ||
      indic_plan->blwf.would_substitute (glyphs+1, 2, face) ||
      indic_plan->vatu.would_substitute (glyphs  , 2, face) ||
      indic_plan->vatu.would_substitute (glyphs+1, 2, face))
    ਵਾਪਸ POS_ਹੇਠਾਂ_C;
  ਜੇ (indic_plan->pstf.would_substitute (glyphs  , 2, face) ||
      indic_plan->pstf.would_substitute (glyphs+1, 2, face))
    ਵਾਪਸ POS_POST_C;
  ਜੇ (indic_plan->pref.would_substitute (glyphs  , 2, face) ||
      indic_plan->pref.would_substitute (glyphs+1, 2, face))
    ਵਾਪਸ POS_POST_C;
  ਵਾਪਸ POS_ਅਧਾਰ_C;
}

ਸਥਿਰ ਖਾਲੀ
ਤਿਆਰੀ_masks_indic (ਅਚਲ hb_ot_shape_plan_t *plan HB_ਅਣਵਰਤਿਆ,
		   hb_ਬਫਰ_t              *ਬਫਰ,
		   hb_font_t                *font HB_ਅਣਵਰਤਿਆ)
{
  HB_ਬਫਰ_ALLOCATE_VAR (ਬਫਰ, indic_category);
  HB_ਬਫਰ_ALLOCATE_VAR (ਬਫਰ, indic_position);

  /* We cannot setup masks here.  We save information about characters
   * and setup masks later on in a pause-callback. */

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਬਫਰ->len;
  hb_glyph_info_t *info = ਬਫਰ->info;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    ਸੈੱਟ_indic_properties (info[i]);
}

ਸਥਿਰ ਤਾਰਕਿਕ
ਤਿਆਰੀ_syllables_indic (ਅਚਲ hb_ot_shape_plan_t *plan HB_ਅਣਵਰਤਿਆ,
		       hb_font_t *font HB_ਅਣਵਰਤਿਆ,
		       hb_ਬਫਰ_t *ਬਫਰ)
{
  HB_ਬਫਰ_ALLOCATE_VAR (ਬਫਰ, syllable);
  ਲੱਭੋ_syllables_indic (ਬਫਰ);
  foreach_syllable (ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ)
    ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_break (ਸ਼ੁਰੂ, ਸਿਰਾ);
  ਵਾਪਸ ਝੂਠ;
}

ਸਥਿਰ ਪੂਰਨ
compare_indic_ਕ੍ਰਮ (ਅਚਲ hb_glyph_info_t *pa, ਅਚਲ hb_glyph_info_t *pb)
{
  ਪੂਰਨ a = pa->indic_position();
  ਪੂਰਨ b = pb->indic_position();

  ਵਾਪਸ (ਪੂਰਨ) a - (ਪੂਰਨ) b;
}



ਸਥਿਰ ਖਾਲੀ
update_consonant_positions_indic (ਅਚਲ hb_ot_shape_plan_t *plan,
				  hb_font_t         *font,
				  hb_ਬਫਰ_t       *ਬਫਰ)
{
  ਅਚਲ indic_shape_plan_t *indic_plan = (ਅਚਲ indic_shape_plan_t *) plan->ਅੰਕੜੇ;

  hb_codepoint_t virama;
  ਜੇ (indic_plan->ਲੱਦੋ_virama_glyph (font, &virama))
  {
    hb_face_t *face = font->face;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਬਫਰ->len;
    hb_glyph_info_t *info = ਬਫਰ->info;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      ਜੇ (info[i].indic_position() == POS_ਅਧਾਰ_C)
      {
	hb_codepoint_t consonant = info[i].codepoint;
	info[i].indic_position() = consonant_position_from_face (indic_plan, consonant, virama, face);
      }
  }
}


/* Rules from:
 * https://docs.microsqoft.com/en-us/typography/script-development/devanagari */

ਸਥਿਰ ਖਾਲੀ
initial_reordering_consonant_syllable (ਅਚਲ hb_ot_shape_plan_t *plan,
				       hb_face_t *face,
				       hb_ਬਫਰ_t *ਬਫਰ,
				       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ)
{
  ਅਚਲ indic_shape_plan_t *indic_plan = (ਅਚਲ indic_shape_plan_t *) plan->ਅੰਕੜੇ;
  hb_glyph_info_t *info = ਬਫਰ->info;

  /* https://github.com/harfbuzz/harfbuzz/issues/435#issuecomment-335560167
   * // For compatibility with legacy usage in Kannada,
   * // Ra+h+ZWJ must behave like Ra+ZWJ+h...
   */
  ਜੇ (ਬਫਰ->props.script == HB_SCRIPT_KANNADA &&
      ਸ਼ੁਰੂ + 3 <= ਸਿਰਾ &&
      is_one_ਦਾ (info[ਸ਼ੁਰੂ  ], FLAG (I_Cat(Ra))) &&
      is_one_ਦਾ (info[ਸ਼ੁਰੂ+1], FLAG (I_Cat(H))) &&
      is_one_ਦਾ (info[ਸ਼ੁਰੂ+2], FLAG (I_Cat(ZWJ))))
  {
    ਬਫਰ->ਮਿਲਾਓ_clusters (ਸ਼ੁਰੂ+1, ਸ਼ੁਰੂ+3);
    hb_ਅਦਲਾ_ਬਦਲੀ (info[ਸ਼ੁਰੂ+1], info[ਸ਼ੁਰੂ+2]);
  }

  /* 1. Find base consonant:
   *
   * The shaping engine finds the base consonant of the syllable, using the
   * following algorithm: starting from the end of the syllable, move backwards
   * until a consonant is found that does not have a below-base or post-base
   * form (post-base forms have to follow below-base forms), or that is not a
   * pre-base-reordering Ra, or arrive at the first consonant. The consonant
   * stopped at will be the base.
   *
   *   o If the syllable starts with Ra + Halant (in a script that has Reph)
   *     and has more than one consonant, Ra is excluded from candidates for
   *     base consonants.
   */

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅਧਾਰ = ਸਿਰਾ;
  ਤਾਰਕਿਕ has_reph = ਝੂਠ;

  {
    /* -> If the syllable starts with Ra + Halant (in a script that has Reph)
     *    and has more than one consonant, Ra is excluded from candidates for
     *    base consonants. */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਹੱਦਬੰਦੀ = ਸ਼ੁਰੂ;
    ਜੇ (indic_plan->ਨਕਾਬ_array[INDIC_RPHF] &&
	ਸ਼ੁਰੂ + 3 <= ਸਿਰਾ &&
	(
	 (indic_plan->config->reph_ਬਹੁਲਕ == REPH_ਬਹੁਲਕ_IMPLICIT && !is_joiner (info[ਸ਼ੁਰੂ + 2])) ||
	 (indic_plan->config->reph_ਬਹੁਲਕ == REPH_ਬਹੁਲਕ_EXPLICIT && info[ਸ਼ੁਰੂ + 2].indic_category() == I_Cat(ZWJ))
	))
    {
      /* See if it matches the 'rphf' feature. */
      hb_codepoint_t glyphs[3] = {info[ਸ਼ੁਰੂ].codepoint,
				  info[ਸ਼ੁਰੂ + 1].codepoint,
				  indic_plan->config->reph_ਬਹੁਲਕ == REPH_ਬਹੁਲਕ_EXPLICIT ?
				    info[ਸ਼ੁਰੂ + 2].codepoint : 0};
      ਜੇ (indic_plan->rphf.would_substitute (glyphs, 2, face) ||
	  (indic_plan->config->reph_ਬਹੁਲਕ == REPH_ਬਹੁਲਕ_EXPLICIT &&
	   indic_plan->rphf.would_substitute (glyphs, 3, face)))
      {
	ਹੱਦਬੰਦੀ += 2;
	ਜਦੋਂ (ਹੱਦਬੰਦੀ < ਸਿਰਾ && is_joiner (info[ਹੱਦਬੰਦੀ]))
	  ਹੱਦਬੰਦੀ++;
	ਅਧਾਰ = ਸ਼ੁਰੂ;
	has_reph = ਸੱਚ;
      }
    } ਨਹੀਂਤਾਂ ਜੇ (indic_plan->config->reph_ਬਹੁਲਕ == REPH_ਬਹੁਲਕ_LOG_REPHA && info[ਸ਼ੁਰੂ].indic_category() == I_Cat(Repha))
    {
	ਹੱਦਬੰਦੀ += 1;
	ਜਦੋਂ (ਹੱਦਬੰਦੀ < ਸਿਰਾ && is_joiner (info[ਹੱਦਬੰਦੀ]))
	  ਹੱਦਬੰਦੀ++;
	ਅਧਾਰ = ਸ਼ੁਰੂ;
	has_reph = ਸੱਚ;
    }

    {
      /* -> starting from the end of the syllable, move backwards */
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸਿਰਾ;
      ਤਾਰਕਿਕ ਵੇਖਿਆ_ਹੇਠਾਂ = ਝੂਠ;
      ਕਰੋ {
	i--;
	/* -> until a consonant is found */
	ਜੇ (is_consonant (info[i]))
	{
	  /* -> that does not have a below-base or post-base form
	   * (post-base forms have to follow below-base forms), */
	  ਜੇ (info[i].indic_position() != POS_ਹੇਠਾਂ_C &&
	      (info[i].indic_position() != POS_POST_C || ਵੇਖਿਆ_ਹੇਠਾਂ))
	  {
	    ਅਧਾਰ = i;
	    ਰੋਕੋ;
	  }
	  ਜੇ (info[i].indic_position() == POS_ਹੇਠਾਂ_C)
	    ਵੇਖਿਆ_ਹੇਠਾਂ = ਸੱਚ;

	  /* -> or that is not a pre-base-reordering Ra,
	   *
	   * IMPLEMENTATION NOTES:
	   *
	   * Our pre-base-reordering Ra's are marked POS_POST_C, so will be skipped
	   * by the logic above already.
	   */

	  /* -> or arrive at the first consonant. The consonant stopped at will
	   * be the base. */
	  ਅਧਾਰ = i;
	}
	ਨਹੀਂਤਾਂ
	{
	  /* A ZWJ after a Halant stops the base search, and requests an explicit
	   * half form.
	   * A ZWJ before a Halant, requests a subjoined form instead, and hence
	   * search continues.  This is particularly important for Bengali
	   * sequence Ra,H,Ya that should form Ya-Phalaa by subjoining Ya. */
	  ਜੇ (ਸ਼ੁਰੂ < i &&
	      info[i].indic_category() == I_Cat(ZWJ) &&
	      info[i - 1].indic_category() == I_Cat(H))
	    ਰੋਕੋ;
	}
      } ਜਦੋਂ (i > ਹੱਦਬੰਦੀ);
    }

    /* -> If the syllable starts with Ra + Halant (in a script that has Reph)
     *    and has more than one consonant, Ra is excluded from candidates for
     *    base consonants.
     *
     *  Only do this for unforced Reph. (ie. not for Ra,H,ZWJ. */
    ਜੇ (has_reph && ਅਧਾਰ == ਸ਼ੁਰੂ && ਹੱਦਬੰਦੀ - ਅਧਾਰ <= 2) {
      /* Have no other consonant, so Reph is not formed and Ra becomes base. */
      has_reph = ਝੂਠ;
    }
  }


  /* 2. Decompose and reorder Matras:
   *
   * Each matra and any syllable modifier sign in the syllable are moved to the
   * appropriate position relative to the consonant(s) in the syllable. The
   * shaping engine decomposes two- or three-part matras into their constituent
   * parts before any repositioning. Matra characters are classified by which
   * consonant in a conjunct they have affinity for and are reordered to the
   * following positions:
   *
   *   o Before first half form in the syllable
   *   o After subjoined consonants
   *   o After post-form consonant
   *   o After main consonant (for above marks)
   *
   * IMPLEMENTATION NOTES:
   *
   * The normalize() routine has already decomposed matras for us, so we don't
   * need to worry about that.
   */


  /* 3.  Reorder marks to canonical order:
   *
   * Adjacent nukta and halant or nukta and vedic sign are always repositioned
   * if necessary, so that the nukta is first.
   *
   * IMPLEMENTATION NOTES:
   *
   * We don't need to do this: the normalize() routine already did this for us.
   */


  /* Reorder characters */

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ; i < ਅਧਾਰ; i++)
    info[i].indic_position() = hb_min (POS_PRE_C, (indic_position_t) info[i].indic_position());

  ਜੇ (ਅਧਾਰ < ਸਿਰਾ)
    info[ਅਧਾਰ].indic_position() = POS_ਅਧਾਰ_C;

  /* Handle beginning Ra */
  ਜੇ (has_reph)
    info[ਸ਼ੁਰੂ].indic_position() = POS_RA_ਮੰਜ਼ਿਲ_BECOME_REPH;

  /* For old-style Indic script tags, move the first post-base Halant after
   * last consonant.
   *
   * Reports suggest that in some scripts Uniscribe does this only if there
   * is *not* a Halant after last consonant already.  We know that is the
   * case for Kannada, while it reorders unconditionally in other scripts,
   * eg. Malayalam, Bengali, and Devanagari.  We don't currently know about
   * other scripts, so we block Kannada.
   *
   * Kannada test case:
   * U+0C9A,U+0CCD,U+0C9A,U+0CCD
   * With some versions of Lohit Kannada.
   * https://bugs.freedesktop.org/show_bug.cgi?id=59118
   *
   * Malayalam test case:
   * U+0D38,U+0D4D,U+0D31,U+0D4D,U+0D31,U+0D4D
   * With lohit-ttf-20121122/Lohit-Malayalam.ttf
   *
   * Bengali test case:
   * U+0998,U+09CD,U+09AF,U+09CD
   * With Windows XP vrinda.ttf
   * https://github.com/harfbuzz/harfbuzz/issues/1073
   *
   * Devanagari test case:
   * U+091F,U+094D,U+0930,U+094D
   * With chandas.ttf
   * https://github.com/harfbuzz/harfbuzz/issues/1071
   */
  ਜੇ (indic_plan->is_old_spec)
  {
    ਤਾਰਕਿਕ disallow_double_halants = ਬਫਰ->props.script == HB_SCRIPT_KANNADA;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਅਧਾਰ + 1; i < ਸਿਰਾ; i++)
      ਜੇ (info[i].indic_category() == I_Cat(H))
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j;
	ਲਈ (j = ਸਿਰਾ - 1; j > i; j--)
	  ਜੇ (is_consonant (info[j]) ||
	      (disallow_double_halants && info[j].indic_category() == I_Cat(H)))
	    ਰੋਕੋ;
	ਜੇ (info[j].indic_category() != I_Cat(H) && j > i) {
	  /* Move Halant to after last consonant. */
	  hb_glyph_info_t t = info[i];
	  memmove (&info[i], &info[i + 1], (j - i) * ਆਕਾਰ (info[0]));
	  info[j] = t;
	}
	ਰੋਕੋ;
      }
  }

  /* Attach misc marks to previous char to move with them. */
  {
    indic_position_t ਆਖਰੀ_pos = POS_ਸ਼ੁਰੂ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ; i < ਸਿਰਾ; i++)
    {
      ਜੇ ((FLAG_UNSAFE (info[i].indic_category()) & (JOINER_FLAGS | FLAG (I_Cat(N)) | FLAG (I_Cat(RS)) | FLAG (I_Cat(CM)) | FLAG (I_Cat(H)))))
      {
	info[i].indic_position() = ਆਖਰੀ_pos;
	ਜੇ (unlikely (info[i].indic_category() == I_Cat(H) &&
		      info[i].indic_position() == POS_PRE_M))
	{
	  /*
	   * Uniscribe doesn't move the Halant with Left Matra.
	   * TEST: U+092B,U+093F,U+094D
	   * We follow.
	   */
	  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = i; j > ਸ਼ੁਰੂ; j--)
	    ਜੇ (info[j - 1].indic_position() != POS_PRE_M) {
	      info[i].indic_position() = info[j - 1].indic_position();
	      ਰੋਕੋ;
	    }
	}
      } ਨਹੀਂਤਾਂ ਜੇ (info[i].indic_position() != POS_SMVD) {
	ਜੇ (info[i].indic_category() == I_Cat(MPst) &&
	    i > ਸ਼ੁਰੂ && info[i - 1].indic_category() == I_Cat(SM))
	  info[i - 1].indic_position() = info[i].indic_position();
	ਆਖਰੀ_pos = (indic_position_t) info[i].indic_position();
      }
    }
  }
  /* For post-base consonants let them own anything before them
   * since the last consonant or matra. */
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਆਖਰੀ = ਅਧਾਰ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਅਧਾਰ + 1; i < ਸਿਰਾ; i++)
      ਜੇ (is_consonant (info[i]))
      {
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = ਆਖਰੀ + 1; j < i; j++)
	  ਜੇ (info[j].indic_position() < POS_SMVD)
	    info[j].indic_position() = info[i].indic_position();
	ਆਖਰੀ = i;
      } ਨਹੀਂਤਾਂ ਜੇ (FLAG_UNSAFE (info[i].indic_category()) & (FLAG (I_Cat(M)) | FLAG (I_Cat(MPst))))
	ਆਖਰੀ = i;
  }


  {
    /* Use syllable() for sort accounting temporarily. */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ syllable = info[ਸ਼ੁਰੂ].syllable();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ; i < ਸਿਰਾ; i++)
      info[i].syllable() = i - ਸ਼ੁਰੂ;

    /* Sit tight, rock 'n roll! */
    hb_stable_ਛਾਂਟੋ (info + ਸ਼ੁਰੂ, ਸਿਰਾ - ਸ਼ੁਰੂ, compare_indic_ਕ੍ਰਮ);

    /* Find base again; also flip left-matra sequence. */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪਹਿਲਾ_ਖੱਬਾ_matra = ਸਿਰਾ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਆਖਰੀ_ਖੱਬਾ_matra = ਸਿਰਾ;
    ਅਧਾਰ = ਸਿਰਾ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ; i < ਸਿਰਾ; i++)
    {
      ਜੇ (info[i].indic_position() == POS_ਅਧਾਰ_C)
      {
	ਅਧਾਰ = i;
	ਰੋਕੋ;
      }
      ਨਹੀਂਤਾਂ ਜੇ (info[i].indic_position() == POS_PRE_M)
      {
        ਜੇ (ਪਹਿਲਾ_ਖੱਬਾ_matra == ਸਿਰਾ)
	  ਪਹਿਲਾ_ਖੱਬਾ_matra = i;
	ਆਖਰੀ_ਖੱਬਾ_matra = i;
      }
    }
    /* https://github.com/harfbuzz/harfbuzz/issues/3863 */
    ਜੇ (ਪਹਿਲਾ_ਖੱਬਾ_matra < ਆਖਰੀ_ਖੱਬਾ_matra)
    {
      /* No need to merge clusters, handled later. */
      ਬਫਰ->ਉਲਟ_range (ਪਹਿਲਾ_ਖੱਬਾ_matra, ਆਖਰੀ_ਖੱਬਾ_matra + 1);
      /* Reverse back nuktas, etc. */
      ਬਿਨਾਨਿਸ਼ਾਨੀ i = ਪਹਿਲਾ_ਖੱਬਾ_matra;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = i; j <= ਆਖਰੀ_ਖੱਬਾ_matra; j++)
	ਜੇ (FLAG_UNSAFE (info[j].indic_category()) & (FLAG (I_Cat(M)) | FLAG (I_Cat(MPst))))
	{
	  ਬਫਰ->ਉਲਟ_range (i, j + 1);
	  i = j + 1;
	}
    }

    /* Things are out-of-control for post base positions, they may shuffle
     * around like crazy.  In old-spec mode, we move halants around, so in
     * that case merge all clusters after base.  Otherwise, check the sort
     * order and merge as needed.
     * For pre-base stuff, we handle cluster issues in final reordering.
     *
     * We could use buffer->sort() for this, if there was no special
     * reordering of pre-base stuff happening later...
     * We don't want to merge_clusters all of that, which buffer->sort()
     * would.  Here's a concrete example:
     *
     * Assume there's a pre-base consonant and explicit Halant before base,
     * followed by a prebase-reordering (left) Matra:
     *
     *   C,H,ZWNJ,B,M
     *
     * At this point in reordering we would have:
     *
     *   M,C,H,ZWNJ,B
     *
     * whereas in final reordering we will bring the Matra closer to Base:
     *
     *   C,H,ZWNJ,M,B
     *
     * That's why we don't want to merge-clusters anything before the Base
     * at this point.  But if something moved from after Base to before it,
     * we should merge clusters from base to them.  In final-reordering, we
     * only move things around before base, and merge-clusters up to base.
     * These two merge-clusters from the two sides of base will interlock
     * to merge things correctly.  See:
     * https://github.com/harfbuzz/harfbuzz/issues/2272
     */
    ਜੇ (indic_plan->is_old_spec || ਸਿਰਾ - ਸ਼ੁਰੂ > 127)
      ਬਫਰ->ਮਿਲਾਓ_clusters (ਅਧਾਰ, ਸਿਰਾ);
    ਨਹੀਂਤਾਂ
    {
      /* Note!  syllable() is a one-byte field. */
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਅਧਾਰ; i < ਸਿਰਾ; i++)
	ਜੇ (info[i].syllable() != 255)
	{
	  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ min = i;
	  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ max = i;
	  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = ਸ਼ੁਰੂ + info[i].syllable();
	  ਜਦੋਂ (j != i)
	  {
	    min = hb_min (min, j);
	    max = hb_max (max, j);
	    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ next = ਸ਼ੁਰੂ + info[j].syllable();
	    info[j].syllable() = 255; /* So we don't process j later again. */
	    j = next;
	  }
	  ਬਫਰ->ਮਿਲਾਓ_clusters (hb_max (ਅਧਾਰ, min), max + 1);
	}
    }

    /* Put syllable back in. */
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ; i < ਸਿਰਾ; i++)
      info[i].syllable() = syllable;
  }

  /* Setup masks now */

  {
    hb_ਨਕਾਬ_t ਨਕਾਬ;

    /* Reph */
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ; i < ਸਿਰਾ && info[i].indic_position() == POS_RA_ਮੰਜ਼ਿਲ_BECOME_REPH; i++)
      info[i].ਨਕਾਬ |= indic_plan->ਨਕਾਬ_array[INDIC_RPHF];

    /* Pre-base */
    ਨਕਾਬ = indic_plan->ਨਕਾਬ_array[INDIC_HALF];
    ਜੇ (!indic_plan->is_old_spec &&
	indic_plan->config->blwf_ਬਹੁਲਕ == BLWF_ਬਹੁਲਕ_PRE_AND_POST)
      ਨਕਾਬ |= indic_plan->ਨਕਾਬ_array[INDIC_BLWF];
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ; i < ਅਧਾਰ; i++)
      info[i].ਨਕਾਬ  |= ਨਕਾਬ;
    /*ਅਧਾਰ*/
    ਨਕਾਬ = 0;
    ਜੇ (ਅਧਾਰ < ਸਿਰਾ)
      info[ਅਧਾਰ].ਨਕਾਬ |= ਨਕਾਬ;
    /* Post-base */
    ਨਕਾਬ = indic_plan->ਨਕਾਬ_array[INDIC_BLWF] |
	   indic_plan->ਨਕਾਬ_array[INDIC_ABVF] |
	   indic_plan->ਨਕਾਬ_array[INDIC_PSTF];
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਅਧਾਰ + 1; i < ਸਿਰਾ; i++)
      info[i].ਨਕਾਬ  |= ਨਕਾਬ;
  }

  ਜੇ (indic_plan->is_old_spec &&
      ਬਫਰ->props.script == HB_SCRIPT_DEVANAGARI)
  {
    /* Old-spec eye-lash Ra needs special handling.  From the
     * spec:
     *
     * "The feature 'below-base form' is applied to consonants
     * having below-base forms and following the base consonant.
     * The exception is vattu, which may appear below half forms
     * as well as below the base glyph. The feature 'below-base
     * form' will be applied to all such occurrences of Ra as well."
     *
     * Test case: U+0924,U+094D,U+0930,U+094d,U+0915
     * with Sanskrit 2003 font.
     *
     * However, note that Ra,Halant,ZWJ is the correct way to
     * request eyelash form of Ra, so we wouldbn't inhibit it
     * in that sequence.
     *
     * Test case: U+0924,U+094D,U+0930,U+094d,U+200D,U+0915
     */
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ; i + 1 < ਅਧਾਰ; i++)
      ਜੇ (info[i  ].indic_category() == I_Cat(Ra) &&
	  info[i+1].indic_category() == I_Cat(H)  &&
	  (i + 2 == ਅਧਾਰ ||
	   info[i+2].indic_category() != I_Cat(ZWJ)))
      {
	info[i  ].ਨਕਾਬ |= indic_plan->ਨਕਾਬ_array[INDIC_BLWF];
	info[i+1].ਨਕਾਬ |= indic_plan->ਨਕਾਬ_array[INDIC_BLWF];
      }
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pref_len = 2;
  ਜੇ (indic_plan->ਨਕਾਬ_array[INDIC_PREF] && ਅਧਾਰ + pref_len < ਸਿਰਾ)
  {
    /* Find a Halant,Ra sequence and mark it for pre-base-reordering processing. */
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਅਧਾਰ + 1; i + pref_len - 1 < ਸਿਰਾ; i++) {
      hb_codepoint_t glyphs[2];
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = 0; j < pref_len; j++)
	glyphs[j] = info[i + j].codepoint;
      ਜੇ (indic_plan->pref.would_substitute (glyphs, pref_len, face))
      {
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = 0; j < pref_len; j++)
	  info[i++].ਨਕਾਬ |= indic_plan->ਨਕਾਬ_array[INDIC_PREF];
	ਰੋਕੋ;
      }
    }
  }

  /* Apply ZWJ/ZWNJ effects */
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ + 1; i < ਸਿਰਾ; i++)
    ਜੇ (is_joiner (info[i])) {
      ਤਾਰਕਿਕ non_joiner = info[i].indic_category() == I_Cat(ZWNJ);
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = i;

      ਕਰੋ {
	j--;

	/* ZWJ/ZWNJ should disable CJCT.  They do that by simply
	 * being there, since we don't skip them for the CJCT
	 * feature (ie. F_MANUAL_ZWJ) */

	/* A ZWNJ disables HALF. */
	ਜੇ (non_joiner)
	  info[j].ਨਕਾਬ &= ~indic_plan->ਨਕਾਬ_array[INDIC_HALF];

      } ਜਦੋਂ (j > ਸ਼ੁਰੂ && !is_consonant (info[j]));
    }
}

ਸਥਿਰ ਖਾਲੀ
initial_reordering_standalone_cluster (ਅਚਲ hb_ot_shape_plan_t *plan,
				       hb_face_t *face,
				       hb_ਬਫਰ_t *ਬਫਰ,
				       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ)
{
  /* We treat placeholder/dotted-circle as if they are consonants, so we
   * should just chain... */

  initial_reordering_consonant_syllable (plan, face, ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ
initial_reordering_syllable_indic (ਅਚਲ hb_ot_shape_plan_t *plan,
				   hb_face_t *face,
				   hb_ਬਫਰ_t *ਬਫਰ,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ)
{
  indic_syllable_type_t syllable_type = (indic_syllable_type_t) (ਬਫਰ->info[ਸ਼ੁਰੂ].syllable() & 0x0F);
  ਚੋਣ (syllable_type)
  {
    ਹਾਲਤ indic_vowel_syllable: /* We made the vowels look like consonants.  So let's call the consonant logic! */
    ਹਾਲਤ indic_consonant_syllable:
     initial_reordering_consonant_syllable (plan, face, ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ);
     ਰੋਕੋ;

    ਹਾਲਤ indic_broken_cluster: /* We already inserted dotted-circles, so just call the standalone_cluster. */
    ਹਾਲਤ indic_standalone_cluster:
     initial_reordering_standalone_cluster (plan, face, ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ);
     ਰੋਕੋ;

    ਹਾਲਤ indic_ਚਿੰਨ੍ਹ_cluster:
    ਹਾਲਤ indic_non_indic_cluster:
      ਰੋਕੋ;
  }
}

ਸਥਿਰ ਤਾਰਕਿਕ
initial_reordering_indic (ਅਚਲ hb_ot_shape_plan_t *plan,
			  hb_font_t *font,
			  hb_ਬਫਰ_t *ਬਫਰ)
{
  ਤਾਰਕਿਕ ret = ਝੂਠ;
  ਜੇ (!ਬਫਰ->ਸੁਨੇਹਾ (font, "start reordering indic initial"))
    ਵਾਪਸ ret;

  update_consonant_positions_indic (plan, font, ਬਫਰ);
  ਜੇ (hb_syllabic_ਪਾਓ_dotted_circles (font, ਬਫਰ,
					 indic_broken_cluster,
					 I_Cat(DOTTEDCIRCLE),
					 I_Cat(Repha),
					 POS_ਸਿਰਾ))
    ret = ਸੱਚ;

  foreach_syllable (ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ)
    initial_reordering_syllable_indic (plan, font->face, ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ);

  (ਖਾਲੀ) ਬਫਰ->ਸੁਨੇਹਾ (font, "end reordering indic initial");

  ਵਾਪਸ ret;
}

ਸਥਿਰ ਖਾਲੀ
final_reordering_syllable_indic (ਅਚਲ hb_ot_shape_plan_t *plan,
				 hb_ਬਫਰ_t *ਬਫਰ,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ)
{
  ਅਚਲ indic_shape_plan_t *indic_plan = (ਅਚਲ indic_shape_plan_t *) plan->ਅੰਕੜੇ;
  hb_glyph_info_t *info = ਬਫਰ->info;


  /* This function relies heavily on halant glyphs.  Lots of ligation
   * and possibly multiple substitutions happened prior to this
   * phase, and that might have messed up our properties.  Recover
   * from a particular case of that where we're fairly sure that a
   * class of I_Cat(H) is desired but has been lost. */
  /* We don't call load_virama_glyph(), since we know it's already
   * loaded. */
  hb_codepoint_t virama_glyph = indic_plan->virama_glyph;
  ਜੇ (virama_glyph)
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ; i < ਸਿਰਾ; i++)
      ਜੇ (info[i].codepoint == virama_glyph &&
	  _hb_glyph_info_ligated (&info[i]) &&
	  _hb_glyph_info_multiplied (&info[i]))
      {
	/* This will make sure that this glyph passes is_halant() test. */
	info[i].indic_category() = I_Cat(H);
	_hb_glyph_info_clear_ligated_and_multiplied (&info[i]);
      }
  }


  /* 4. Final reordering:
   *
   * After the localized forms and basic shaping forms GSUB features have been
   * applied (see below), the shaping engine performs some final glyph
   * reordering before applying all the remaining font features to the entire
   * syllable.
   */

  ਤਾਰਕਿਕ try_pref = !!indic_plan->ਨਕਾਬ_array[INDIC_PREF];

  /* Find base again */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅਧਾਰ;
  ਲਈ (ਅਧਾਰ = ਸ਼ੁਰੂ; ਅਧਾਰ < ਸਿਰਾ; ਅਧਾਰ++)
    ਜੇ (info[ਅਧਾਰ].indic_position() >= POS_ਅਧਾਰ_C)
    {
      ਜੇ (try_pref && ਅਧਾਰ + 1 < ਸਿਰਾ)
      {
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਅਧਾਰ + 1; i < ਸਿਰਾ; i++)
	  ਜੇ ((info[i].ਨਕਾਬ & indic_plan->ਨਕਾਬ_array[INDIC_PREF]) != 0)
	  {
	    ਜੇ (!(_hb_glyph_info_substituted (&info[i]) &&
		  _hb_glyph_info_ligated_and_didnt_multiply (&info[i])))
	    {
	      /* Ok, this was a 'pref' candidate but didn't form any.
	       * Base is around here... */
	      ਅਧਾਰ = i;
	      ਜਦੋਂ (ਅਧਾਰ < ਸਿਰਾ && is_halant (info[ਅਧਾਰ]))
		ਅਧਾਰ++;
	      ਜੇ (ਅਧਾਰ < ਸਿਰਾ)
		info[ਅਧਾਰ].indic_position() = POS_ਅਧਾਰ_C;

	      try_pref = ਝੂਠ;
	    }
	    ਰੋਕੋ;
	  }
	ਜੇ (ਅਧਾਰ == ਸਿਰਾ)
	  ਰੋਕੋ;
      }
      /* For Malayalam, skip over unformed below- (but NOT post-) forms. */
      ਜੇ (ਬਫਰ->props.script == HB_SCRIPT_MALAYALAM)
      {
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਅਧਾਰ + 1; i < ਸਿਰਾ; i++)
	{
	  ਜਦੋਂ (i < ਸਿਰਾ && is_joiner (info[i]))
	    i++;
	  ਜੇ (i == ਸਿਰਾ || !is_halant (info[i]))
	    ਰੋਕੋ;
	  i++; /* Skip halant. */
	  ਜਦੋਂ (i < ਸਿਰਾ && is_joiner (info[i]))
	    i++;
	  ਜੇ (i < ਸਿਰਾ && is_consonant (info[i]) && info[i].indic_position() == POS_ਹੇਠਾਂ_C)
	  {
	    ਅਧਾਰ = i;
	    info[ਅਧਾਰ].indic_position() = POS_ਅਧਾਰ_C;
	  }
	}
      }

      ਜੇ (ਸ਼ੁਰੂ < ਅਧਾਰ && info[ਅਧਾਰ].indic_position() > POS_ਅਧਾਰ_C)
	ਅਧਾਰ--;
      ਰੋਕੋ;
    }
  ਜੇ (ਅਧਾਰ == ਸਿਰਾ && ਸ਼ੁਰੂ < ਅਧਾਰ &&
      is_one_ਦਾ (info[ਅਧਾਰ - 1], FLAG (I_Cat(ZWJ))))
    ਅਧਾਰ--;
  ਜੇ (ਅਧਾਰ < ਸਿਰਾ)
    ਜਦੋਂ (ਸ਼ੁਰੂ < ਅਧਾਰ &&
	   is_one_ਦਾ (info[ਅਧਾਰ], (FLAG (I_Cat(N)) | FLAG (I_Cat(H)))))
      ਅਧਾਰ--;


  /*   o Reorder matras:
   *
   *     If a pre-base matra character had been reordered before applying basic
   *     features, the glyph can be moved closer to the main consonant based on
   *     whether half-forms had been formed. Actual position for the matra is
   *     defined as “after last standalone halant glyph, after initial matra
   *     position and before the main consonant”. If ZWJ or ZWNJ follow this
   *     halant, position is moved after it.
   *
   * IMPLEMENTATION NOTES:
   *
   * It looks like the last sentence is wrong.  Testing, with Windows 7 Uniscribe
   * and Devanagari shows that the behavior is best described as:
   *
   * "If ZWJ follows this halant, matra is NOT repositioned after this halant.
   *  If ZWNJ follows this halant, position is moved after it."
   *
   * Test case, with Adobe Devanagari or Nirmala UI:
   *
   *   U+091F,U+094D,U+200C,U+092F,U+093F
   *   (Matra moves to the middle, after ZWNJ.)
   *
   *   U+091F,U+094D,U+200D,U+092F,U+093F
   *   (Matra does NOT move, stays to the left.)
   *
   * https://github.com/harfbuzz/harfbuzz/issues/1070
   */

  ਜੇ (ਸ਼ੁਰੂ + 1 < ਸਿਰਾ && ਸ਼ੁਰੂ < ਅਧਾਰ) /* Otherwise there can't be any pre-base matra characters. */
  {
    /* If we lost track of base, alas, position before last thingy. */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ new_pos = ਅਧਾਰ == ਸਿਰਾ ? ਅਧਾਰ - 2 : ਅਧਾਰ - 1;

    /* Malayalam / Tamil do not have "half" forms or explicit virama forms.
     * The glyphs formed by 'half' are Chillus or ligated explicit viramas.
     * We want to position matra after them.
     */
    ਜੇ (ਬਫਰ->props.script != HB_SCRIPT_MALAYALAM && ਬਫਰ->props.script != HB_SCRIPT_TAMIL)
    {
    search:
      ਜਦੋਂ (new_pos > ਸ਼ੁਰੂ &&
	     !(is_one_ਦਾ (info[new_pos], (FLAG (I_Cat(M)) | FLAG (I_Cat(MPst)) | FLAG (I_Cat(H))))))
	new_pos--;

      /* If we found no Halant we are done.
       * Otherwise only proceed if the Halant does
       * not belong to the Matra itself! */
      ਜੇ (is_halant (info[new_pos]) &&
	  info[new_pos].indic_position() != POS_PRE_M)
      {
#ਜੇ 0 // See comment above
	/* -> If ZWJ or ZWNJ follow this halant, position is moved after it. */
	ਜੇ (new_pos + 1 < ਸਿਰਾ && is_joiner (info[new_pos + 1]))
	  new_pos++;
#ਜੇ_ਅੰਤ
	ਜੇ (new_pos + 1 < ਸਿਰਾ)
	{
	  /* -> If ZWJ follows this halant, matra is NOT repositioned after this halant. */
	  ਜੇ (info[new_pos + 1].indic_category() == I_Cat(ZWJ))
	  {
	    /* Keep searching. */
	    ਜੇ (new_pos > ਸ਼ੁਰੂ)
	    {
	      new_pos--;
	      ਜਾਓ search;
	    }
	  }
	  /* -> If ZWNJ follows this halant, position is moved after it.
	   *
	   * IMPLEMENTATION NOTES:
	   *
	   * This is taken care of by the state-machine. A Halant,ZWNJ is a terminating
	   * sequence for a consonant syllable; any pre-base matras occurring after it
	   * will belong to the subsequent syllable.
	   */
	}
      }
      ਨਹੀਂਤਾਂ
	new_pos = ਸ਼ੁਰੂ; /* No move. */
    }

    ਜੇ (ਸ਼ੁਰੂ < new_pos && info[new_pos].indic_position () != POS_PRE_M)
    {
      /* Now go see if there's actually any matras... */
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = new_pos; i > ਸ਼ੁਰੂ; i--)
	ਜੇ (info[i - 1].indic_position () == POS_PRE_M)
	{
	  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ old_pos = i - 1;
	  ਜੇ (old_pos < ਅਧਾਰ && ਅਧਾਰ <= new_pos) /* Shouldn't actually happen. */
	    ਅਧਾਰ--;

	  hb_glyph_info_t tmp = info[old_pos];
	  memmove (&info[old_pos], &info[old_pos + 1], (new_pos - old_pos) * ਆਕਾਰ (info[0]));
	  info[new_pos] = tmp;

	  /* Note: this merge_clusters() is intentionally *after* the reordering.
	   * Indic matra reordering is special and tricky... */
	  ਬਫਰ->ਮਿਲਾਓ_clusters (new_pos, hb_min (ਸਿਰਾ, ਅਧਾਰ + 1));

	  new_pos--;
	}
    } ਨਹੀਂਤਾਂ {
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ; i < ਅਧਾਰ; i++)
	ਜੇ (info[i].indic_position () == POS_PRE_M) {
	  ਬਫਰ->ਮਿਲਾਓ_clusters (i, hb_min (ਸਿਰਾ, ਅਧਾਰ + 1));
	  ਰੋਕੋ;
	}
    }
  }


  /*   o Reorder reph:
   *
   *     Reph’s original position is always at the beginning of the syllable,
   *     (i.e. it is not reordered at the character reordering stage). However,
   *     it will be reordered according to the basic-forms shaping results.
   *     Possible positions for reph, depending on the script, are; after main,
   *     before post-base consonant forms, and after post-base consonant forms.
   */

  /* Two cases:
   *
   * - If repha is encoded as a sequence of characters (Ra,H or Ra,H,ZWJ), then
   *   we should only move it if the sequence ligated to the repha form.
   *
   * - If repha is encoded separately and in the logical position, we should only
   *   move it if it did NOT ligate.  If it ligated, it's probably the font trying
   *   to make it work without the reordering.
   */
  ਜੇ (ਸ਼ੁਰੂ + 1 < ਸਿਰਾ &&
      info[ਸ਼ੁਰੂ].indic_position() == POS_RA_ਮੰਜ਼ਿਲ_BECOME_REPH &&
      ((info[ਸ਼ੁਰੂ].indic_category() == I_Cat(Repha)) ^
       _hb_glyph_info_ligated_and_didnt_multiply (&info[ਸ਼ੁਰੂ])))
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ new_reph_pos;
    reph_position_t reph_pos = indic_plan->config->reph_pos;

    /*       1. If reph should be positioned after post-base consonant forms,
     *          proceed to step 5.
     */
    ਜੇ (reph_pos == REPH_POS_AFTER_POST)
    {
      ਜਾਓ reph_ਕਦਮ_5;
    }

    /*       2. If the reph repositioning class is not after post-base: target
     *          position is after the first explicit halant glyph between the
     *          first post-reph consonant and last main consonant. If ZWJ or ZWNJ
     *          are following this halant, position is moved after it. If such
     *          position is found, this is the target position. Otherwise,
     *          proceed to the next step.
     *
     *          Note: in old-implementation fonts, where classifications were
     *          fixed in shaping engine, there was no case where reph position
     *          will be found on this step.
     */
    {
      new_reph_pos = ਸ਼ੁਰੂ + 1;
      ਜਦੋਂ (new_reph_pos < ਅਧਾਰ && !is_halant (info[new_reph_pos]))
	new_reph_pos++;

      ਜੇ (new_reph_pos < ਅਧਾਰ && is_halant (info[new_reph_pos]))
      {
	/* ->If ZWJ or ZWNJ are following this halant, position is moved after it. */
	ਜੇ (new_reph_pos + 1 < ਅਧਾਰ && is_joiner (info[new_reph_pos + 1]))
	  new_reph_pos++;
	ਜਾਓ reph_move;
      }
    }

    /*       3. If reph should be repositioned after the main consonant: find the
     *          first consonant not ligated with main, or find the first
     *          consonant that is not a potential pre-base-reordering Ra.
     */
    ਜੇ (reph_pos == REPH_POS_AFTER_MAIN)
    {
      new_reph_pos = ਅਧਾਰ;
      ਜਦੋਂ (new_reph_pos + 1 < ਸਿਰਾ && info[new_reph_pos + 1].indic_position() <= POS_AFTER_MAIN)
	new_reph_pos++;
      ਜੇ (new_reph_pos < ਸਿਰਾ)
	ਜਾਓ reph_move;
    }

    /*       4. If reph should be positioned before post-base consonant, find
     *          first post-base classified consonant not ligated with main. If no
     *          consonant is found, the target position should be before the
     *          first matra, syllable modifier sign or vedic sign.
     */
    /* This is our take on what step 4 is trying to say (and failing, BADLY). */
    ਜੇ (reph_pos == REPH_POS_AFTER_SUB)
    {
      new_reph_pos = ਅਧਾਰ;
      ਜਦੋਂ (new_reph_pos + 1 < ਸਿਰਾ &&
	     !( FLAG_UNSAFE (info[new_reph_pos + 1].indic_position()) & (FLAG (POS_POST_C) | FLAG (POS_AFTER_POST) | FLAG (POS_SMVD))))
	new_reph_pos++;
      ਜੇ (new_reph_pos < ਸਿਰਾ)
	ਜਾਓ reph_move;
    }

    /*       5. If no consonant is found in steps 3 or 4, move reph to a position
     *          immediately before the first post-base matra, syllable modifier
     *          sign or vedic sign that has a reordering class after the intended
     *          reph position. For example, if the reordering position for reph
     *          is post-main, it will skip above-base matras that also have a
     *          post-main position.
     */
    reph_ਕਦਮ_5:
    {
      /* Copied from step 2. */
      new_reph_pos = ਸ਼ੁਰੂ + 1;
      ਜਦੋਂ (new_reph_pos < ਅਧਾਰ && !is_halant (info[new_reph_pos]))
	new_reph_pos++;

      ਜੇ (new_reph_pos < ਅਧਾਰ && is_halant (info[new_reph_pos]))
      {
	/* ->If ZWJ or ZWNJ are following this halant, position is moved after it. */
	ਜੇ (new_reph_pos + 1 < ਅਧਾਰ && is_joiner (info[new_reph_pos + 1]))
	  new_reph_pos++;
	ਜਾਓ reph_move;
      }
    }
    /* See https://github.com/harfbuzz/harfbuzz/issues/2298#issuecomment-615318654 */

    /*       6. Otherwise, reorder reph to the end of the syllable.
     */
    {
      new_reph_pos = ਸਿਰਾ - 1;
      ਜਦੋਂ (new_reph_pos > ਸ਼ੁਰੂ && info[new_reph_pos].indic_position() == POS_SMVD)
	new_reph_pos--;

      /*
       * If the Reph is to be ending up after a Matra,Halant sequence,
       * position it before that Halant so it can interact with the Matra.
       * However, if it's a plain Consonant,Halant we shouldn't do that.
       * Uniscribe doesn't do this.
       * TEST: U+0930,U+094D,U+0915,U+094B,U+094D
       */
      ਜੇ (unlikely (is_halant (info[new_reph_pos])))
      {
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਅਧਾਰ + 1; i < new_reph_pos; i++)
	  ਜੇ (FLAG_UNSAFE (info[i].indic_category()) & (FLAG (I_Cat(M)) | FLAG (I_Cat(MPst))))
	  {
	    /* Ok, got it. */
	    new_reph_pos--;
	  }
      }

      ਜਾਓ reph_move;
    }

    reph_move:
    {
      /* Move */
      ਬਫਰ->ਮਿਲਾਓ_clusters (ਸ਼ੁਰੂ, new_reph_pos + 1);
      hb_glyph_info_t reph = info[ਸ਼ੁਰੂ];
      memmove (&info[ਸ਼ੁਰੂ], &info[ਸ਼ੁਰੂ + 1], (new_reph_pos - ਸ਼ੁਰੂ) * ਆਕਾਰ (info[0]));
      info[new_reph_pos] = reph;

      ਜੇ (ਸ਼ੁਰੂ < ਅਧਾਰ && ਅਧਾਰ <= new_reph_pos)
	ਅਧਾਰ--;
    }
  }


  /*   o Reorder pre-base-reordering consonants:
   *
   *     If a pre-base-reordering consonant is found, reorder it according to
   *     the following rules:
   */

  ਜੇ (try_pref && ਅਧਾਰ + 1 < ਸਿਰਾ) /* Otherwise there can't be any pre-base-reordering Ra. */
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਅਧਾਰ + 1; i < ਸਿਰਾ; i++)
      ਜੇ ((info[i].ਨਕਾਬ & indic_plan->ਨਕਾਬ_array[INDIC_PREF]) != 0)
      {
	/*       1. Only reorder a glyph produced by substitution during application
	 *          of the <pref> feature. (Note that a font may shape a Ra consonant with
	 *          the feature generally but block it in certain contexts.)
	 */
	/* Note: We just check that something got substituted.  We don't check that
	 * the <pref> feature actually did it...
	 *
	 * Reorder pref only if it ligated. */
	ਜੇ (_hb_glyph_info_ligated_and_didnt_multiply (&info[i]))
	{
	  /*
	   *       2. Try to find a target position the same way as for pre-base matra.
	   *          If it is found, reorder pre-base consonant glyph.
	   *
	   *       3. If position is not found, reorder immediately before main
	   *          consonant.
	   */

	  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ new_pos = ਅਧਾਰ;
	  /* Malayalam / Tamil do not have "half" forms or explicit virama forms.
	   * The glyphs formed by 'half' are Chillus or ligated explicit viramas.
	   * We want to position matra after them.
	   */
	  ਜੇ (ਬਫਰ->props.script != HB_SCRIPT_MALAYALAM && ਬਫਰ->props.script != HB_SCRIPT_TAMIL)
	  {
	    ਜਦੋਂ (new_pos > ਸ਼ੁਰੂ &&
		   !(is_one_ਦਾ (info[new_pos - 1], FLAG (I_Cat(M)) | FLAG (I_Cat(MPst)) | FLAG (I_Cat(H)))))
	      new_pos--;
	  }

	  ਜੇ (new_pos > ਸ਼ੁਰੂ && is_halant (info[new_pos - 1]))
	  {
	    /* -> If ZWJ or ZWNJ follow this halant, position is moved after it. */
	    ਜੇ (new_pos < ਸਿਰਾ && is_joiner (info[new_pos]))
	      new_pos++;
	  }

	  {
	    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ old_pos = i;

	    ਬਫਰ->ਮਿਲਾਓ_clusters (new_pos, old_pos + 1);
	    hb_glyph_info_t tmp = info[old_pos];
	    memmove (&info[new_pos + 1], &info[new_pos], (old_pos - new_pos) * ਆਕਾਰ (info[0]));
	    info[new_pos] = tmp;

	    ਜੇ (new_pos <= ਅਧਾਰ && ਅਧਾਰ < old_pos)
	      ਅਧਾਰ++;
	  }
	}

	ਰੋਕੋ;
      }
  }


  /* Apply 'init' to the Left Matra if it's a word start. */
  ਜੇ (info[ਸ਼ੁਰੂ].indic_position () == POS_PRE_M)
  {
    ਜੇ (!ਸ਼ੁਰੂ ||
	!(FLAG_UNSAFE (_hb_glyph_info_get_general_category (&info[ਸ਼ੁਰੂ - 1])) &
	 FLAG_RANGE (HB_UNICODE_GENERAL_CATEGORY_FORMAT, HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)))
      info[ਸ਼ੁਰੂ].ਨਕਾਬ |= indic_plan->ਨਕਾਬ_array[INDIC_INIT];
    ਨਹੀਂਤਾਂ
      ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_break (ਸ਼ੁਰੂ - 1, ਸ਼ੁਰੂ + 1);
  }
}


ਸਥਿਰ ਤਾਰਕਿਕ
final_reordering_indic (ਅਚਲ hb_ot_shape_plan_t *plan,
			hb_font_t *font HB_ਅਣਵਰਤਿਆ,
			hb_ਬਫਰ_t *ਬਫਰ)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਬਫਰ->len;
  ਜੇ (unlikely (!ਗਿਣਤੀ_2)) ਵਾਪਸ ਝੂਠ;

  ਜੇ (ਬਫਰ->ਸੁਨੇਹਾ (font, "start reordering indic final")) {
    foreach_syllable (ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ)
      final_reordering_syllable_indic (plan, ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ);
    (ਖਾਲੀ) ਬਫਰ->ਸੁਨੇਹਾ (font, "end reordering indic final");
  }

  HB_ਬਫਰ_DEALLOCATE_VAR (ਬਫਰ, indic_category);
  HB_ਬਫਰ_DEALLOCATE_VAR (ਬਫਰ, indic_position);

  ਵਾਪਸ ਝੂਠ;
}


ਸਥਿਰ ਖਾਲੀ
preprocess_ਪਾਠ_indic (ਅਚਲ hb_ot_shape_plan_t *plan,
		       hb_ਬਫਰ_t              *ਬਫਰ,
		       hb_font_t                *font)
{
  hb_preprocess_ਪਾਠ_vowel_constraints (plan, ਬਫਰ, font);
}

ਸਥਿਰ ਤਾਰਕਿਕ
decompose_indic (ਅਚਲ hb_ot_shape_normalize_context_t *c,
		 hb_codepoint_t  ab,
		 hb_codepoint_t *a,
		 hb_codepoint_t *b)
{
  ਚੋਣ (ab)
  {
    /* Don't decompose these. */
    ਹਾਲਤ 0x0931u  : ਵਾਪਸ ਝੂਠ; /* DEVANAGARI LETTER RRA */
    // https://github.com/harfbuzz/harfbuzz/issues/779
    ਹਾਲਤ 0x09DCu  : ਵਾਪਸ ਝੂਠ; /* BENGALI LETTER RRA */
    ਹਾਲਤ 0x09DDu  : ਵਾਪਸ ਝੂਠ; /* BENGALI LETTER RHA */
    ਹਾਲਤ 0x0B94u  : ਵਾਪਸ ਝੂਠ; /* TAMIL LETTER AU */


    /*
     * Decompose split matras that don't have Unicode decompositions.
     */

#ਜੇ 0
    /* Gujarati */
    /* This one has no decomposition in Unicode, but needs no decomposition either. */
    /* case 0x0AC9u  : return false; */

    /* Oriya */
    ਹਾਲਤ 0x0B57u  : *a = no decomp, -> ਸੱਜਾ_2; ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅੰਤ
  }

  ਵਾਪਸ (ਤਾਰਕਿਕ) c->unicode->decompose (ab, a, b);
}

ਸਥਿਰ ਤਾਰਕਿਕ
compose_indic (ਅਚਲ hb_ot_shape_normalize_context_t *c,
	       hb_codepoint_t  a,
	       hb_codepoint_t  b,
	       hb_codepoint_t *ab)
{
  /* Avoid recomposing split matras. */
  ਜੇ (HB_UNICODE_GENERAL_CATEGORY_IS_MARK (c->unicode->general_category (a)))
    ਵਾਪਸ ਝੂਠ;

  /* Composition-exclusion exceptions that we want to recompose. */
  ਜੇ (a == 0x09AFu && b == 0x09BCu) { *ab = 0x09DFu; ਵਾਪਸ ਸੱਚ; }

  ਵਾਪਸ (ਤਾਰਕਿਕ) c->unicode->compose (a, b, ab);
}


ਅਚਲ hb_ot_shaper_t _hb_ot_shaper_indic =
{
  collect_features_indic,
  override_features_indic,
  ਅੰਕੜੇ_create_indic,
  ਅੰਕੜੇ_destroy_indic,
  preprocess_ਪਾਠ_indic,
  ਖਾਲੀ_ਸੂਚਕ, /* postprocess_glyphs */
  decompose_indic,
  compose_indic,
  ਤਿਆਰੀ_masks_indic,
  ਖਾਲੀ_ਸੂਚਕ, /* reorder_marks */
  HB_ਟੈਗ_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_ਬਹੁਲਕ_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT,
  HB_OT_SHAPE_ZERO_ਚੌੜਾਈ_MARKS_NONE,
  ਝੂਠ, /* fallback_position */
};


#ਜੇ_ਅੰਤ

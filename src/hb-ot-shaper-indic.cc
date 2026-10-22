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

#ضمن "hb.hh"

#جے_بنا_تعریف HB_NO_OT_SHAPE

#ضمن "hb-ot-shaper-indic.hh"
#ضمن "hb-ot-shaper-indic-machine.hh"
#ضمن "hb-ot-shaper-vowel-constraints.hh"
#ضمن "hb-ot-layout.hh"


/*
 * Indic shaper.
 */


ساكن مضمن فراغ
سیٹ_indic_properties (hb_glyph_info_t &info)
{
  hb_codepoint_t u = info.codepoint;
  بنانشانی صحيح type = hb_indic_get_categories (u);

  info.indic_category() = (indic_category_t) (type & 0xFFu);
  info.indic_position() = (indic_position_t) (type >> 8);
}


ساكن مضمن منطقي
is_one_دا (ثابت hb_glyph_info_t &info, بنانشانی صحيح flags)
{
  /* If it ligated, all bets are off. */
  إذا (_hb_glyph_info_ligated (&info)) أرجع خطأ;
  أرجع !!(FLAG_UNSAFE (info.indic_category()) & flags);
}

/* Note:
 *
 * We treat Vowels and placeholders as if they were consonants.  This is safe because Vowels
 * cannot happen in a consonant syllable.  The plus side however is, we can call the
 * consonant syllable logic from the vowel syllable function and get it all right!
 *
 * Keep in sync with consonant_categories in the generator. */
#تعریف_کرو CONSONANT_FLAGS_INDIC (FLAG (I_Cat(C)) | FLAG (I_Cat(CS)) | FLAG (I_Cat(Ra)) | FLAG (I_Cat(CM)) | FLAG (I_Cat(V)) | FLAG (I_Cat(PLACEHOLDER)) | FLAG (I_Cat(DOTTEDCIRCLE)))

ساكن مضمن منطقي
is_consonant (ثابت hb_glyph_info_t &info)
{
  أرجع is_one_دا (info, CONSONANT_FLAGS_INDIC);
}

#تعریف_کرو JOINER_FLAGS (FLAG (I_Cat(ZWJ)) | FLAG (I_Cat(ZWNJ)))

ساكن مضمن منطقي
is_joiner (ثابت hb_glyph_info_t &info)
{
  أرجع is_one_دا (info, JOINER_FLAGS);
}

ساكن مضمن منطقي
is_halant (ثابت hb_glyph_info_t &info)
{
  أرجع is_one_دا (info, FLAG (I_Cat(H)));
}

بنية hb_indic_would_substitute_feature_t
{
  فراغ init (ثابت hb_ot_map_t *map, hb_ٹیگ_t feature_ٹیگ, منطقي zero_context_)
  {
    zero_context = zero_context_;
    lookups = map->get_stage_lookups (0/*GSUB*/,
				      map->get_feature_stage (0/*GSUB*/, feature_ٹیگ));
  }

  منطقي would_substitute (ثابت hb_codepoint_t *glyphs,
			 بنانشانی صحيح          glyphs_گنتی,
			 hb_face_t            *face) ثابت
  {
    لكل (ثابت خودکار &lookup : lookups)
      إذا (hb_ot_layout_lookup_would_substitute (face, lookup.اشاریہ_2, glyphs, glyphs_گنتی, zero_context))
	أرجع سچ;
    أرجع خطأ;
  }

  خاص:
  hb_array_t<ثابت hb_ot_map_t::lookup_map_t> lookups;
  منطقي zero_context;
};


/*
 * Indic configurations.  Note that we do not want to keep every single script-specific
 * behavior in these tables necessarily.  This should mainly be used for per-script
 * properties that are cheaper keeping here, than in the code.  Ie. if, say, one and
 * only one script has an exception, that one script can be if'ed directly in the code,
 * instead of adding a new flag in these structs.
 */

تعداد reph_position_t {
  REPH_POS_AFTER_MAIN  = POS_AFTER_MAIN,
  REPH_POS_BEFORE_SUB  = POS_BEFORE_SUB,
  REPH_POS_AFTER_SUB   = POS_AFTER_SUB,
  REPH_POS_BEFORE_POST = POS_BEFORE_POST,
  REPH_POS_AFTER_POST  = POS_AFTER_POST
};
تعداد reph_منوال_t {
  REPH_منوال_IMPLICIT,  /* Reph formed out of initial Ra,H sequence. */
  REPH_منوال_EXPLICIT,  /* Reph formed out of initial Ra,H,ZWJ sequence. */
  REPH_منوال_LOG_REPHA  /* Encoded Repha character, needs reordering. */
};
تعداد blwf_منوال_t {
  BLWF_منوال_PRE_AND_POST, /* Below-forms feature applied to pre-base and post-base. */
  BLWF_منوال_POST_ONLY     /* Below-forms feature applied to post-base only. */
};
بنية indic_config_t
{
  hb_script_t     script;
  منطقي            has_old_spec;
  hb_codepoint_t  virama;
  reph_position_t reph_pos;
  reph_منوال_t     reph_منوال;
  blwf_منوال_t     blwf_منوال;
};

ساكن ثابت indic_config_t indic_configs[] =
{
  /* Default.  Should be first. */
  {HB_SCRIPT_INVALID,	خطأ,      0,REPH_POS_BEFORE_POST,REPH_منوال_IMPLICIT, BLWF_منوال_PRE_AND_POST},
  {HB_SCRIPT_DEVANAGARI,سچ, 0x094Du,REPH_POS_BEFORE_POST,REPH_منوال_IMPLICIT, BLWF_منوال_PRE_AND_POST},
  {HB_SCRIPT_BENGALI,	سچ, 0x09CDu,REPH_POS_AFTER_SUB,  REPH_منوال_IMPLICIT, BLWF_منوال_PRE_AND_POST},
  {HB_SCRIPT_GURMUKHI,	سچ, 0x0A4Du,REPH_POS_BEFORE_SUB, REPH_منوال_IMPLICIT, BLWF_منوال_PRE_AND_POST},
  {HB_SCRIPT_GUJARATI,	سچ, 0x0ACDu,REPH_POS_BEFORE_POST,REPH_منوال_IMPLICIT, BLWF_منوال_PRE_AND_POST},
  {HB_SCRIPT_ORIYA,	سچ, 0x0B4Du,REPH_POS_AFTER_MAIN, REPH_منوال_IMPLICIT, BLWF_منوال_PRE_AND_POST},
  {HB_SCRIPT_TAMIL,	سچ, 0x0BCDu,REPH_POS_AFTER_POST, REPH_منوال_IMPLICIT, BLWF_منوال_PRE_AND_POST},
  {HB_SCRIPT_TELUGU,	سچ, 0x0C4Du,REPH_POS_AFTER_POST, REPH_منوال_EXPLICIT, BLWF_منوال_POST_ONLY},
  {HB_SCRIPT_KANNADA,	سچ, 0x0CCDu,REPH_POS_AFTER_POST, REPH_منوال_IMPLICIT, BLWF_منوال_POST_ONLY},
  {HB_SCRIPT_MALAYALAM,	سچ, 0x0D4Du,REPH_POS_AFTER_MAIN, REPH_منوال_LOG_REPHA,BLWF_منوال_PRE_AND_POST},
};


ساكن ثابت hb_ot_map_feature_t
indic_features[] =
{
  /*
   * Basic features.
   * These features are applied in order, one at a time, after initial_reordering,
   * constrained to the syllable.
   */
  {HB_ٹیگ('n','u','k','t'), F_GLOBAL_MANUAL_JOINERS | F_فی_SYLLABLE},
  {HB_ٹیگ('a','k','h','n'), F_GLOBAL_MANUAL_JOINERS | F_فی_SYLLABLE},
  {HB_ٹیگ('r','p','h','f'),        F_MANUAL_JOINERS | F_فی_SYLLABLE},
  {HB_ٹیگ('r','k','r','f'), F_GLOBAL_MANUAL_JOINERS | F_فی_SYLLABLE},
  {HB_ٹیگ('p','r','e','f'),        F_MANUAL_JOINERS | F_فی_SYLLABLE},
  {HB_ٹیگ('b','l','w','f'),        F_MANUAL_JOINERS | F_فی_SYLLABLE},
  {HB_ٹیگ('a','b','v','f'),        F_MANUAL_JOINERS | F_فی_SYLLABLE},
  {HB_ٹیگ('h','a','l','f'),        F_MANUAL_JOINERS | F_فی_SYLLABLE},
  {HB_ٹیگ('p','s','t','f'),        F_MANUAL_JOINERS | F_فی_SYLLABLE},
  {HB_ٹیگ('v','a','t','u'), F_GLOBAL_MANUAL_JOINERS | F_فی_SYLLABLE},
  {HB_ٹیگ('c','j','c','t'), F_GLOBAL_MANUAL_JOINERS | F_فی_SYLLABLE},
  /*
   * Other features.
   * These features are applied all at once, after final_reordering, constrained
   * to the syllable.
   * Default Bengali font in Windows for example has intermixed
   * lookups for init,pres,abvs,blws features.
   */
  {HB_ٹیگ('i','n','i','t'),        F_MANUAL_JOINERS | F_فی_SYLLABLE},
  {HB_ٹیگ('p','r','e','s'), F_GLOBAL_MANUAL_JOINERS | F_فی_SYLLABLE},
  {HB_ٹیگ('a','b','v','s'), F_GLOBAL_MANUAL_JOINERS | F_فی_SYLLABLE},
  {HB_ٹیگ('b','l','w','s'), F_GLOBAL_MANUAL_JOINERS | F_فی_SYLLABLE},
  {HB_ٹیگ('p','s','t','s'), F_GLOBAL_MANUAL_JOINERS | F_فی_SYLLABLE},
  {HB_ٹیگ('h','a','l','n'), F_GLOBAL_MANUAL_JOINERS | F_فی_SYLLABLE},
};

/*
 * Must be in the same order as the indic_features array.
 */
تعداد {
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

  INDIC_عدد_FEATURES,
  INDIC_BASIC_FEATURES = INDIC_INIT, /* Don't forget to update this! */
};

ساكن منطقي
تیاری_syllables_indic (ثابت hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_بفر_t *بفر);
ساكن منطقي
initial_reordering_indic (ثابت hb_ot_shape_plan_t *plan,
			  hb_font_t *font,
			  hb_بفر_t *بفر);
ساكن منطقي
final_reordering_indic (ثابت hb_ot_shape_plan_t *plan,
			hb_font_t *font,
			hb_بفر_t *بفر);

ساكن فراغ
collect_features_indic (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  /* Do this before any lookups have been applied. */
  map->جوڑو_gsub_pause (تیاری_syllables_indic);

  map->enable_feature (HB_ٹیگ('l','o','c','l'), F_فی_SYLLABLE);
  /* The Indic specs do not require ccmp, but we apply it here since if
   * there is a use of it, it's typically at the beginning. */
  map->enable_feature (HB_ٹیگ('c','c','m','p'), F_فی_SYLLABLE);


  بنانشانی صحيح i = 0;
  map->جوڑو_gsub_pause (initial_reordering_indic);

  لكل (; i < INDIC_BASIC_FEATURES; i++) {
    map->جوڑو_feature (indic_features[i]);
    map->جوڑو_gsub_pause (خالی_سوچک);
  }

  map->جوڑو_gsub_pause (final_reordering_indic);

  لكل (; i < INDIC_عدد_FEATURES; i++)
    map->جوڑو_feature (indic_features[i]);
}

ساكن فراغ
override_features_indic (hb_ot_shape_planner_t *plan)
{
  plan->map.disable_feature (HB_ٹیگ('l','i','g','a'));
  plan->map.جوڑو_gsub_pause (hb_syllabic_clear_var); // Don't need syllables anymore, use stop to free buffer var
}


بنية indic_shape_plan_t
{
  منطقي لدو_virama_glyph (hb_font_t *font, hb_codepoint_t *pglyph) ثابت
  {
    hb_codepoint_t glyph = virama_glyph;
    إذا (unlikely (glyph == (hb_codepoint_t) -1))
    {
      إذا (!config->virama || !font->get_nominal_glyph (config->virama, &glyph))
	glyph = 0;
      /* Technically speaking, the spec says we should apply 'locl' to virama too.
       * Maybe one day... */

      /* Our get_nominal_glyph() function needs a font, so we can't get the virama glyph
       * during shape planning...  Instead, overwrite it here. */
      virama_glyph = (صحيح) glyph;
    }

    *pglyph = glyph;
    أرجع glyph != 0;
  }

  ثابت indic_config_t *config;

  منطقي is_old_spec;
  بدلنجوگ hb_atomic_t<hb_codepoint_t> virama_glyph;

  hb_indic_would_substitute_feature_t rphf;
  hb_indic_would_substitute_feature_t pref;
  hb_indic_would_substitute_feature_t blwf;
  hb_indic_would_substitute_feature_t pstf;
  hb_indic_would_substitute_feature_t vatu;

  hb_نقاب_t نقاب_array[INDIC_عدد_FEATURES];
};

ساكن فراغ *
انکڑے_create_indic (ثابت hb_ot_shape_plan_t *plan)
{
  indic_shape_plan_t *indic_plan = (indic_shape_plan_t *) hb_calloc (1, حجم (indic_shape_plan_t));
  إذا (unlikely (!indic_plan))
    أرجع خالی_سوچک;

  indic_plan->config = &indic_configs[0];
  لكل (بنانشانی صحيح i = 1; i < ARRAY_لمائی (indic_configs); i++)
    إذا (plan->props.script == indic_configs[i].script) {
      indic_plan->config = &indic_configs[i];
      اكسر;
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
  منطقي zero_context = !indic_plan->is_old_spec && plan->props.script != HB_SCRIPT_MALAYALAM;
  indic_plan->rphf.init (&plan->map, HB_ٹیگ('r','p','h','f'), zero_context);
  indic_plan->pref.init (&plan->map, HB_ٹیگ('p','r','e','f'), zero_context);
  indic_plan->blwf.init (&plan->map, HB_ٹیگ('b','l','w','f'), zero_context);
  indic_plan->pstf.init (&plan->map, HB_ٹیگ('p','s','t','f'), zero_context);
  indic_plan->vatu.init (&plan->map, HB_ٹیگ('v','a','t','u'), zero_context);

  لكل (بنانشانی صحيح i = 0; i < ARRAY_لمائی (indic_plan->نقاب_array); i++)
    indic_plan->نقاب_array[i] = (indic_features[i].flags & F_GLOBAL) ?
				 0 : plan->map.get_1_نقاب (indic_features[i].ٹیگ_2);

  أرجع indic_plan;
}

ساكن فراغ
انکڑے_destroy_indic (فراغ *انکڑے)
{
  hb_free (انکڑے);
}

ساكن indic_position_t
consonant_position_from_face (ثابت indic_shape_plan_t *indic_plan,
			      ثابت hb_codepoint_t consonant,
			      ثابت hb_codepoint_t virama,
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
  إذا (indic_plan->blwf.would_substitute (glyphs  , 2, face) ||
      indic_plan->blwf.would_substitute (glyphs+1, 2, face) ||
      indic_plan->vatu.would_substitute (glyphs  , 2, face) ||
      indic_plan->vatu.would_substitute (glyphs+1, 2, face))
    أرجع POS_ہیٹھاں_C;
  إذا (indic_plan->pstf.would_substitute (glyphs  , 2, face) ||
      indic_plan->pstf.would_substitute (glyphs+1, 2, face))
    أرجع POS_POST_C;
  إذا (indic_plan->pref.would_substitute (glyphs  , 2, face) ||
      indic_plan->pref.would_substitute (glyphs+1, 2, face))
    أرجع POS_POST_C;
  أرجع POS_ادھار_C;
}

ساكن فراغ
تیاری_masks_indic (ثابت hb_ot_shape_plan_t *plan HB_بن_ورتیا,
		   hb_بفر_t              *بفر,
		   hb_font_t                *font HB_بن_ورتیا)
{
  HB_بفر_ALLOCATE_VAR (بفر, indic_category);
  HB_بفر_ALLOCATE_VAR (بفر, indic_position);

  /* We cannot setup masks here.  We save information about characters
   * and setup masks later on in a pause-callback. */

  بنانشانی صحيح گنتی = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    سیٹ_indic_properties (info[i]);
}

ساكن منطقي
تیاری_syllables_indic (ثابت hb_ot_shape_plan_t *plan HB_بن_ورتیا,
		       hb_font_t *font HB_بن_ورتیا,
		       hb_بفر_t *بفر)
{
  HB_بفر_ALLOCATE_VAR (بفر, syllable);
  لبھو_syllables_indic (بفر);
  foreach_syllable (بفر, شروع, سرا)
    بفر->unsafe_منزل_break (شروع, سرا);
  أرجع خطأ;
}

ساكن صحيح
compare_indic_ترتیب (ثابت hb_glyph_info_t *pa, ثابت hb_glyph_info_t *pb)
{
  صحيح a = pa->indic_position();
  صحيح b = pb->indic_position();

  أرجع (صحيح) a - (صحيح) b;
}



ساكن فراغ
update_consonant_positions_indic (ثابت hb_ot_shape_plan_t *plan,
				  hb_font_t         *font,
				  hb_بفر_t       *بفر)
{
  ثابت indic_shape_plan_t *indic_plan = (ثابت indic_shape_plan_t *) plan->انکڑے;

  hb_codepoint_t virama;
  إذا (indic_plan->لدو_virama_glyph (font, &virama))
  {
    hb_face_t *face = font->face;
    بنانشانی صحيح گنتی = بفر->len;
    hb_glyph_info_t *info = بفر->info;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
      إذا (info[i].indic_position() == POS_ادھار_C)
      {
	hb_codepoint_t consonant = info[i].codepoint;
	info[i].indic_position() = consonant_position_from_face (indic_plan, consonant, virama, face);
      }
  }
}


/* Rules from:
 * https://docs.microsqoft.com/en-us/typography/script-development/devanagari */

ساكن فراغ
initial_reordering_consonant_syllable (ثابت hb_ot_shape_plan_t *plan,
				       hb_face_t *face,
				       hb_بفر_t *بفر,
				       بنانشانی صحيح شروع, بنانشانی صحيح سرا)
{
  ثابت indic_shape_plan_t *indic_plan = (ثابت indic_shape_plan_t *) plan->انکڑے;
  hb_glyph_info_t *info = بفر->info;

  /* https://github.com/harfbuzz/harfbuzz/issues/435#issuecomment-335560167
   * // For compatibility with legacy usage in Kannada,
   * // Ra+h+ZWJ must behave like Ra+ZWJ+h...
   */
  إذا (بفر->props.script == HB_SCRIPT_KANNADA &&
      شروع + 3 <= سرا &&
      is_one_دا (info[شروع  ], FLAG (I_Cat(Ra))) &&
      is_one_دا (info[شروع+1], FLAG (I_Cat(H))) &&
      is_one_دا (info[شروع+2], FLAG (I_Cat(ZWJ))))
  {
    بفر->ملاؤ_clusters (شروع+1, شروع+3);
    hb_وٹاندرا (info[شروع+1], info[شروع+2]);
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

  بنانشانی صحيح ادھار = سرا;
  منطقي has_reph = خطأ;

  {
    /* -> If the syllable starts with Ra + Halant (in a script that has Reph)
     *    and has more than one consonant, Ra is excluded from candidates for
     *    base consonants. */
    بنانشانی صحيح حد_بندی = شروع;
    إذا (indic_plan->نقاب_array[INDIC_RPHF] &&
	شروع + 3 <= سرا &&
	(
	 (indic_plan->config->reph_منوال == REPH_منوال_IMPLICIT && !is_joiner (info[شروع + 2])) ||
	 (indic_plan->config->reph_منوال == REPH_منوال_EXPLICIT && info[شروع + 2].indic_category() == I_Cat(ZWJ))
	))
    {
      /* See if it matches the 'rphf' feature. */
      hb_codepoint_t glyphs[3] = {info[شروع].codepoint,
				  info[شروع + 1].codepoint,
				  indic_plan->config->reph_منوال == REPH_منوال_EXPLICIT ?
				    info[شروع + 2].codepoint : 0};
      إذا (indic_plan->rphf.would_substitute (glyphs, 2, face) ||
	  (indic_plan->config->reph_منوال == REPH_منوال_EXPLICIT &&
	   indic_plan->rphf.would_substitute (glyphs, 3, face)))
      {
	حد_بندی += 2;
	طالما (حد_بندی < سرا && is_joiner (info[حد_بندی]))
	  حد_بندی++;
	ادھار = شروع;
	has_reph = سچ;
      }
    } وإلا إذا (indic_plan->config->reph_منوال == REPH_منوال_LOG_REPHA && info[شروع].indic_category() == I_Cat(Repha))
    {
	حد_بندی += 1;
	طالما (حد_بندی < سرا && is_joiner (info[حد_بندی]))
	  حد_بندی++;
	ادھار = شروع;
	has_reph = سچ;
    }

    {
      /* -> starting from the end of the syllable, move backwards */
      بنانشانی صحيح i = سرا;
      منطقي تکیا_ہیٹھاں = خطأ;
      افعل {
	i--;
	/* -> until a consonant is found */
	إذا (is_consonant (info[i]))
	{
	  /* -> that does not have a below-base or post-base form
	   * (post-base forms have to follow below-base forms), */
	  إذا (info[i].indic_position() != POS_ہیٹھاں_C &&
	      (info[i].indic_position() != POS_POST_C || تکیا_ہیٹھاں))
	  {
	    ادھار = i;
	    اكسر;
	  }
	  إذا (info[i].indic_position() == POS_ہیٹھاں_C)
	    تکیا_ہیٹھاں = سچ;

	  /* -> or that is not a pre-base-reordering Ra,
	   *
	   * IMPLEMENTATION NOTES:
	   *
	   * Our pre-base-reordering Ra's are marked POS_POST_C, so will be skipped
	   * by the logic above already.
	   */

	  /* -> or arrive at the first consonant. The consonant stopped at will
	   * be the base. */
	  ادھار = i;
	}
	وإلا
	{
	  /* A ZWJ after a Halant stops the base search, and requests an explicit
	   * half form.
	   * A ZWJ before a Halant, requests a subjoined form instead, and hence
	   * search continues.  This is particularly important for Bengali
	   * sequence Ra,H,Ya that should form Ya-Phalaa by subjoining Ya. */
	  إذا (شروع < i &&
	      info[i].indic_category() == I_Cat(ZWJ) &&
	      info[i - 1].indic_category() == I_Cat(H))
	    اكسر;
	}
      } طالما (i > حد_بندی);
    }

    /* -> If the syllable starts with Ra + Halant (in a script that has Reph)
     *    and has more than one consonant, Ra is excluded from candidates for
     *    base consonants.
     *
     *  Only do this for unforced Reph. (ie. not for Ra,H,ZWJ. */
    إذا (has_reph && ادھار == شروع && حد_بندی - ادھار <= 2) {
      /* Have no other consonant, so Reph is not formed and Ra becomes base. */
      has_reph = خطأ;
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

  لكل (بنانشانی صحيح i = شروع; i < ادھار; i++)
    info[i].indic_position() = hb_min (POS_PRE_C, (indic_position_t) info[i].indic_position());

  إذا (ادھار < سرا)
    info[ادھار].indic_position() = POS_ادھار_C;

  /* Handle beginning Ra */
  إذا (has_reph)
    info[شروع].indic_position() = POS_RA_منزل_BECOME_REPH;

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
  إذا (indic_plan->is_old_spec)
  {
    منطقي disallow_double_halants = بفر->props.script == HB_SCRIPT_KANNADA;
    لكل (بنانشانی صحيح i = ادھار + 1; i < سرا; i++)
      إذا (info[i].indic_category() == I_Cat(H))
      {
	بنانشانی صحيح j;
	لكل (j = سرا - 1; j > i; j--)
	  إذا (is_consonant (info[j]) ||
	      (disallow_double_halants && info[j].indic_category() == I_Cat(H)))
	    اكسر;
	إذا (info[j].indic_category() != I_Cat(H) && j > i) {
	  /* Move Halant to after last consonant. */
	  hb_glyph_info_t t = info[i];
	  memmove (&info[i], &info[i + 1], (j - i) * حجم (info[0]));
	  info[j] = t;
	}
	اكسر;
      }
  }

  /* Attach misc marks to previous char to move with them. */
  {
    indic_position_t آخری_pos = POS_شروع;
    لكل (بنانشانی صحيح i = شروع; i < سرا; i++)
    {
      إذا ((FLAG_UNSAFE (info[i].indic_category()) & (JOINER_FLAGS | FLAG (I_Cat(N)) | FLAG (I_Cat(RS)) | FLAG (I_Cat(CM)) | FLAG (I_Cat(H)))))
      {
	info[i].indic_position() = آخری_pos;
	إذا (unlikely (info[i].indic_category() == I_Cat(H) &&
		      info[i].indic_position() == POS_PRE_M))
	{
	  /*
	   * Uniscribe doesn't move the Halant with Left Matra.
	   * TEST: U+092B,U+093F,U+094D
	   * We follow.
	   */
	  لكل (بنانشانی صحيح j = i; j > شروع; j--)
	    إذا (info[j - 1].indic_position() != POS_PRE_M) {
	      info[i].indic_position() = info[j - 1].indic_position();
	      اكسر;
	    }
	}
      } وإلا إذا (info[i].indic_position() != POS_SMVD) {
	إذا (info[i].indic_category() == I_Cat(MPst) &&
	    i > شروع && info[i - 1].indic_category() == I_Cat(SM))
	  info[i - 1].indic_position() = info[i].indic_position();
	آخری_pos = (indic_position_t) info[i].indic_position();
      }
    }
  }
  /* For post-base consonants let them own anything before them
   * since the last consonant or matra. */
  {
    بنانشانی صحيح آخری = ادھار;
    لكل (بنانشانی صحيح i = ادھار + 1; i < سرا; i++)
      إذا (is_consonant (info[i]))
      {
	لكل (بنانشانی صحيح j = آخری + 1; j < i; j++)
	  إذا (info[j].indic_position() < POS_SMVD)
	    info[j].indic_position() = info[i].indic_position();
	آخری = i;
      } وإلا إذا (FLAG_UNSAFE (info[i].indic_category()) & (FLAG (I_Cat(M)) | FLAG (I_Cat(MPst))))
	آخری = i;
  }


  {
    /* Use syllable() for sort accounting temporarily. */
    بنانشانی صحيح syllable = info[شروع].syllable();
    لكل (بنانشانی صحيح i = شروع; i < سرا; i++)
      info[i].syllable() = i - شروع;

    /* Sit tight, rock 'n roll! */
    hb_stable_ترتیب_دیو (info + شروع, سرا - شروع, compare_indic_ترتیب);

    /* Find base again; also flip left-matra sequence. */
    بنانشانی پہلا_بایاں_matra = سرا;
    بنانشانی آخری_بایاں_matra = سرا;
    ادھار = سرا;
    لكل (بنانشانی صحيح i = شروع; i < سرا; i++)
    {
      إذا (info[i].indic_position() == POS_ادھار_C)
      {
	ادھار = i;
	اكسر;
      }
      وإلا إذا (info[i].indic_position() == POS_PRE_M)
      {
        إذا (پہلا_بایاں_matra == سرا)
	  پہلا_بایاں_matra = i;
	آخری_بایاں_matra = i;
      }
    }
    /* https://github.com/harfbuzz/harfbuzz/issues/3863 */
    إذا (پہلا_بایاں_matra < آخری_بایاں_matra)
    {
      /* No need to merge clusters, handled later. */
      بفر->الٹ_range (پہلا_بایاں_matra, آخری_بایاں_matra + 1);
      /* Reverse back nuktas, etc. */
      بنانشانی i = پہلا_بایاں_matra;
      لكل (بنانشانی j = i; j <= آخری_بایاں_matra; j++)
	إذا (FLAG_UNSAFE (info[j].indic_category()) & (FLAG (I_Cat(M)) | FLAG (I_Cat(MPst))))
	{
	  بفر->الٹ_range (i, j + 1);
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
    إذا (indic_plan->is_old_spec || سرا - شروع > 127)
      بفر->ملاؤ_clusters (ادھار, سرا);
    وإلا
    {
      /* Note!  syllable() is a one-byte field. */
      لكل (بنانشانی صحيح i = ادھار; i < سرا; i++)
	إذا (info[i].syllable() != 255)
	{
	  بنانشانی صحيح min = i;
	  بنانشانی صحيح max = i;
	  بنانشانی صحيح j = شروع + info[i].syllable();
	  طالما (j != i)
	  {
	    min = hb_min (min, j);
	    max = hb_max (max, j);
	    بنانشانی صحيح next = شروع + info[j].syllable();
	    info[j].syllable() = 255; /* So we don't process j later again. */
	    j = next;
	  }
	  بفر->ملاؤ_clusters (hb_max (ادھار, min), max + 1);
	}
    }

    /* Put syllable back in. */
    لكل (بنانشانی صحيح i = شروع; i < سرا; i++)
      info[i].syllable() = syllable;
  }

  /* Setup masks now */

  {
    hb_نقاب_t نقاب;

    /* Reph */
    لكل (بنانشانی صحيح i = شروع; i < سرا && info[i].indic_position() == POS_RA_منزل_BECOME_REPH; i++)
      info[i].نقاب |= indic_plan->نقاب_array[INDIC_RPHF];

    /* Pre-base */
    نقاب = indic_plan->نقاب_array[INDIC_HALF];
    إذا (!indic_plan->is_old_spec &&
	indic_plan->config->blwf_منوال == BLWF_منوال_PRE_AND_POST)
      نقاب |= indic_plan->نقاب_array[INDIC_BLWF];
    لكل (بنانشانی صحيح i = شروع; i < ادھار; i++)
      info[i].نقاب  |= نقاب;
    /*ادھار*/
    نقاب = 0;
    إذا (ادھار < سرا)
      info[ادھار].نقاب |= نقاب;
    /* Post-base */
    نقاب = indic_plan->نقاب_array[INDIC_BLWF] |
	   indic_plan->نقاب_array[INDIC_ABVF] |
	   indic_plan->نقاب_array[INDIC_PSTF];
    لكل (بنانشانی صحيح i = ادھار + 1; i < سرا; i++)
      info[i].نقاب  |= نقاب;
  }

  إذا (indic_plan->is_old_spec &&
      بفر->props.script == HB_SCRIPT_DEVANAGARI)
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
    لكل (بنانشانی صحيح i = شروع; i + 1 < ادھار; i++)
      إذا (info[i  ].indic_category() == I_Cat(Ra) &&
	  info[i+1].indic_category() == I_Cat(H)  &&
	  (i + 2 == ادھار ||
	   info[i+2].indic_category() != I_Cat(ZWJ)))
      {
	info[i  ].نقاب |= indic_plan->نقاب_array[INDIC_BLWF];
	info[i+1].نقاب |= indic_plan->نقاب_array[INDIC_BLWF];
      }
  }

  بنانشانی صحيح pref_len = 2;
  إذا (indic_plan->نقاب_array[INDIC_PREF] && ادھار + pref_len < سرا)
  {
    /* Find a Halant,Ra sequence and mark it for pre-base-reordering processing. */
    لكل (بنانشانی صحيح i = ادھار + 1; i + pref_len - 1 < سرا; i++) {
      hb_codepoint_t glyphs[2];
      لكل (بنانشانی صحيح j = 0; j < pref_len; j++)
	glyphs[j] = info[i + j].codepoint;
      إذا (indic_plan->pref.would_substitute (glyphs, pref_len, face))
      {
	لكل (بنانشانی صحيح j = 0; j < pref_len; j++)
	  info[i++].نقاب |= indic_plan->نقاب_array[INDIC_PREF];
	اكسر;
      }
    }
  }

  /* Apply ZWJ/ZWNJ effects */
  لكل (بنانشانی صحيح i = شروع + 1; i < سرا; i++)
    إذا (is_joiner (info[i])) {
      منطقي non_joiner = info[i].indic_category() == I_Cat(ZWNJ);
      بنانشانی صحيح j = i;

      افعل {
	j--;

	/* ZWJ/ZWNJ should disable CJCT.  They do that by simply
	 * being there, since we don't skip them for the CJCT
	 * feature (ie. F_MANUAL_ZWJ) */

	/* A ZWNJ disables HALF. */
	إذا (non_joiner)
	  info[j].نقاب &= ~indic_plan->نقاب_array[INDIC_HALF];

      } طالما (j > شروع && !is_consonant (info[j]));
    }
}

ساكن فراغ
initial_reordering_standalone_cluster (ثابت hb_ot_shape_plan_t *plan,
				       hb_face_t *face,
				       hb_بفر_t *بفر,
				       بنانشانی صحيح شروع, بنانشانی صحيح سرا)
{
  /* We treat placeholder/dotted-circle as if they are consonants, so we
   * should just chain... */

  initial_reordering_consonant_syllable (plan, face, بفر, شروع, سرا);
}

ساكن فراغ
initial_reordering_syllable_indic (ثابت hb_ot_shape_plan_t *plan,
				   hb_face_t *face,
				   hb_بفر_t *بفر,
				   بنانشانی صحيح شروع, بنانشانی صحيح سرا)
{
  indic_syllable_type_t syllable_type = (indic_syllable_type_t) (بفر->info[شروع].syllable() & 0x0F);
  بدل (syllable_type)
  {
    حالة indic_vowel_syllable: /* We made the vowels look like consonants.  So let's call the consonant logic! */
    حالة indic_consonant_syllable:
     initial_reordering_consonant_syllable (plan, face, بفر, شروع, سرا);
     اكسر;

    حالة indic_broken_cluster: /* We already inserted dotted-circles, so just call the standalone_cluster. */
    حالة indic_standalone_cluster:
     initial_reordering_standalone_cluster (plan, face, بفر, شروع, سرا);
     اكسر;

    حالة indic_علامت_cluster:
    حالة indic_non_indic_cluster:
      اكسر;
  }
}

ساكن منطقي
initial_reordering_indic (ثابت hb_ot_shape_plan_t *plan,
			  hb_font_t *font,
			  hb_بفر_t *بفر)
{
  منطقي ret = خطأ;
  إذا (!بفر->سنیہا (font, "start reordering indic initial"))
    أرجع ret;

  update_consonant_positions_indic (plan, font, بفر);
  إذا (hb_syllabic_داخل_کرو_dotted_circles (font, بفر,
					 indic_broken_cluster,
					 I_Cat(DOTTEDCIRCLE),
					 I_Cat(Repha),
					 POS_سرا))
    ret = سچ;

  foreach_syllable (بفر, شروع, سرا)
    initial_reordering_syllable_indic (plan, font->face, بفر, شروع, سرا);

  (فراغ) بفر->سنیہا (font, "end reordering indic initial");

  أرجع ret;
}

ساكن فراغ
final_reordering_syllable_indic (ثابت hb_ot_shape_plan_t *plan,
				 hb_بفر_t *بفر,
				 بنانشانی صحيح شروع, بنانشانی صحيح سرا)
{
  ثابت indic_shape_plan_t *indic_plan = (ثابت indic_shape_plan_t *) plan->انکڑے;
  hb_glyph_info_t *info = بفر->info;


  /* This function relies heavily on halant glyphs.  Lots of ligation
   * and possibly multiple substitutions happened prior to this
   * phase, and that might have messed up our properties.  Recover
   * from a particular case of that where we're fairly sure that a
   * class of I_Cat(H) is desired but has been lost. */
  /* We don't call load_virama_glyph(), since we know it's already
   * loaded. */
  hb_codepoint_t virama_glyph = indic_plan->virama_glyph;
  إذا (virama_glyph)
  {
    لكل (بنانشانی صحيح i = شروع; i < سرا; i++)
      إذا (info[i].codepoint == virama_glyph &&
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

  منطقي try_pref = !!indic_plan->نقاب_array[INDIC_PREF];

  /* Find base again */
  بنانشانی صحيح ادھار;
  لكل (ادھار = شروع; ادھار < سرا; ادھار++)
    إذا (info[ادھار].indic_position() >= POS_ادھار_C)
    {
      إذا (try_pref && ادھار + 1 < سرا)
      {
	لكل (بنانشانی صحيح i = ادھار + 1; i < سرا; i++)
	  إذا ((info[i].نقاب & indic_plan->نقاب_array[INDIC_PREF]) != 0)
	  {
	    إذا (!(_hb_glyph_info_substituted (&info[i]) &&
		  _hb_glyph_info_ligated_and_didnt_multiply (&info[i])))
	    {
	      /* Ok, this was a 'pref' candidate but didn't form any.
	       * Base is around here... */
	      ادھار = i;
	      طالما (ادھار < سرا && is_halant (info[ادھار]))
		ادھار++;
	      إذا (ادھار < سرا)
		info[ادھار].indic_position() = POS_ادھار_C;

	      try_pref = خطأ;
	    }
	    اكسر;
	  }
	إذا (ادھار == سرا)
	  اكسر;
      }
      /* For Malayalam, skip over unformed below- (but NOT post-) forms. */
      إذا (بفر->props.script == HB_SCRIPT_MALAYALAM)
      {
	لكل (بنانشانی صحيح i = ادھار + 1; i < سرا; i++)
	{
	  طالما (i < سرا && is_joiner (info[i]))
	    i++;
	  إذا (i == سرا || !is_halant (info[i]))
	    اكسر;
	  i++; /* Skip halant. */
	  طالما (i < سرا && is_joiner (info[i]))
	    i++;
	  إذا (i < سرا && is_consonant (info[i]) && info[i].indic_position() == POS_ہیٹھاں_C)
	  {
	    ادھار = i;
	    info[ادھار].indic_position() = POS_ادھار_C;
	  }
	}
      }

      إذا (شروع < ادھار && info[ادھار].indic_position() > POS_ادھار_C)
	ادھار--;
      اكسر;
    }
  إذا (ادھار == سرا && شروع < ادھار &&
      is_one_دا (info[ادھار - 1], FLAG (I_Cat(ZWJ))))
    ادھار--;
  إذا (ادھار < سرا)
    طالما (شروع < ادھار &&
	   is_one_دا (info[ادھار], (FLAG (I_Cat(N)) | FLAG (I_Cat(H)))))
      ادھار--;


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

  إذا (شروع + 1 < سرا && شروع < ادھار) /* Otherwise there can't be any pre-base matra characters. */
  {
    /* If we lost track of base, alas, position before last thingy. */
    بنانشانی صحيح new_pos = ادھار == سرا ? ادھار - 2 : ادھار - 1;

    /* Malayalam / Tamil do not have "half" forms or explicit virama forms.
     * The glyphs formed by 'half' are Chillus or ligated explicit viramas.
     * We want to position matra after them.
     */
    إذا (بفر->props.script != HB_SCRIPT_MALAYALAM && بفر->props.script != HB_SCRIPT_TAMIL)
    {
    search:
      طالما (new_pos > شروع &&
	     !(is_one_دا (info[new_pos], (FLAG (I_Cat(M)) | FLAG (I_Cat(MPst)) | FLAG (I_Cat(H))))))
	new_pos--;

      /* If we found no Halant we are done.
       * Otherwise only proceed if the Halant does
       * not belong to the Matra itself! */
      إذا (is_halant (info[new_pos]) &&
	  info[new_pos].indic_position() != POS_PRE_M)
      {
#إذا 0 // See comment above
	/* -> If ZWJ or ZWNJ follow this halant, position is moved after it. */
	إذا (new_pos + 1 < سرا && is_joiner (info[new_pos + 1]))
	  new_pos++;
#جے_انت
	إذا (new_pos + 1 < سرا)
	{
	  /* -> If ZWJ follows this halant, matra is NOT repositioned after this halant. */
	  إذا (info[new_pos + 1].indic_category() == I_Cat(ZWJ))
	  {
	    /* Keep searching. */
	    إذا (new_pos > شروع)
	    {
	      new_pos--;
	      اذهب search;
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
      وإلا
	new_pos = شروع; /* No move. */
    }

    إذا (شروع < new_pos && info[new_pos].indic_position () != POS_PRE_M)
    {
      /* Now go see if there's actually any matras... */
      لكل (بنانشانی صحيح i = new_pos; i > شروع; i--)
	إذا (info[i - 1].indic_position () == POS_PRE_M)
	{
	  بنانشانی صحيح old_pos = i - 1;
	  إذا (old_pos < ادھار && ادھار <= new_pos) /* Shouldn't actually happen. */
	    ادھار--;

	  hb_glyph_info_t tmp = info[old_pos];
	  memmove (&info[old_pos], &info[old_pos + 1], (new_pos - old_pos) * حجم (info[0]));
	  info[new_pos] = tmp;

	  /* Note: this merge_clusters() is intentionally *after* the reordering.
	   * Indic matra reordering is special and tricky... */
	  بفر->ملاؤ_clusters (new_pos, hb_min (سرا, ادھار + 1));

	  new_pos--;
	}
    } وإلا {
      لكل (بنانشانی صحيح i = شروع; i < ادھار; i++)
	إذا (info[i].indic_position () == POS_PRE_M) {
	  بفر->ملاؤ_clusters (i, hb_min (سرا, ادھار + 1));
	  اكسر;
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
  إذا (شروع + 1 < سرا &&
      info[شروع].indic_position() == POS_RA_منزل_BECOME_REPH &&
      ((info[شروع].indic_category() == I_Cat(Repha)) ^
       _hb_glyph_info_ligated_and_didnt_multiply (&info[شروع])))
  {
    بنانشانی صحيح new_reph_pos;
    reph_position_t reph_pos = indic_plan->config->reph_pos;

    /*       1. If reph should be positioned after post-base consonant forms,
     *          proceed to step 5.
     */
    إذا (reph_pos == REPH_POS_AFTER_POST)
    {
      اذهب reph_قدم_5;
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
      new_reph_pos = شروع + 1;
      طالما (new_reph_pos < ادھار && !is_halant (info[new_reph_pos]))
	new_reph_pos++;

      إذا (new_reph_pos < ادھار && is_halant (info[new_reph_pos]))
      {
	/* ->If ZWJ or ZWNJ are following this halant, position is moved after it. */
	إذا (new_reph_pos + 1 < ادھار && is_joiner (info[new_reph_pos + 1]))
	  new_reph_pos++;
	اذهب reph_move;
      }
    }

    /*       3. If reph should be repositioned after the main consonant: find the
     *          first consonant not ligated with main, or find the first
     *          consonant that is not a potential pre-base-reordering Ra.
     */
    إذا (reph_pos == REPH_POS_AFTER_MAIN)
    {
      new_reph_pos = ادھار;
      طالما (new_reph_pos + 1 < سرا && info[new_reph_pos + 1].indic_position() <= POS_AFTER_MAIN)
	new_reph_pos++;
      إذا (new_reph_pos < سرا)
	اذهب reph_move;
    }

    /*       4. If reph should be positioned before post-base consonant, find
     *          first post-base classified consonant not ligated with main. If no
     *          consonant is found, the target position should be before the
     *          first matra, syllable modifier sign or vedic sign.
     */
    /* This is our take on what step 4 is trying to say (and failing, BADLY). */
    إذا (reph_pos == REPH_POS_AFTER_SUB)
    {
      new_reph_pos = ادھار;
      طالما (new_reph_pos + 1 < سرا &&
	     !( FLAG_UNSAFE (info[new_reph_pos + 1].indic_position()) & (FLAG (POS_POST_C) | FLAG (POS_AFTER_POST) | FLAG (POS_SMVD))))
	new_reph_pos++;
      إذا (new_reph_pos < سرا)
	اذهب reph_move;
    }

    /*       5. If no consonant is found in steps 3 or 4, move reph to a position
     *          immediately before the first post-base matra, syllable modifier
     *          sign or vedic sign that has a reordering class after the intended
     *          reph position. For example, if the reordering position for reph
     *          is post-main, it will skip above-base matras that also have a
     *          post-main position.
     */
    reph_قدم_5:
    {
      /* Copied from step 2. */
      new_reph_pos = شروع + 1;
      طالما (new_reph_pos < ادھار && !is_halant (info[new_reph_pos]))
	new_reph_pos++;

      إذا (new_reph_pos < ادھار && is_halant (info[new_reph_pos]))
      {
	/* ->If ZWJ or ZWNJ are following this halant, position is moved after it. */
	إذا (new_reph_pos + 1 < ادھار && is_joiner (info[new_reph_pos + 1]))
	  new_reph_pos++;
	اذهب reph_move;
      }
    }
    /* See https://github.com/harfbuzz/harfbuzz/issues/2298#issuecomment-615318654 */

    /*       6. Otherwise, reorder reph to the end of the syllable.
     */
    {
      new_reph_pos = سرا - 1;
      طالما (new_reph_pos > شروع && info[new_reph_pos].indic_position() == POS_SMVD)
	new_reph_pos--;

      /*
       * If the Reph is to be ending up after a Matra,Halant sequence,
       * position it before that Halant so it can interact with the Matra.
       * However, if it's a plain Consonant,Halant we shouldn't do that.
       * Uniscribe doesn't do this.
       * TEST: U+0930,U+094D,U+0915,U+094B,U+094D
       */
      إذا (unlikely (is_halant (info[new_reph_pos])))
      {
	لكل (بنانشانی صحيح i = ادھار + 1; i < new_reph_pos; i++)
	  إذا (FLAG_UNSAFE (info[i].indic_category()) & (FLAG (I_Cat(M)) | FLAG (I_Cat(MPst))))
	  {
	    /* Ok, got it. */
	    new_reph_pos--;
	  }
      }

      اذهب reph_move;
    }

    reph_move:
    {
      /* Move */
      بفر->ملاؤ_clusters (شروع, new_reph_pos + 1);
      hb_glyph_info_t reph = info[شروع];
      memmove (&info[شروع], &info[شروع + 1], (new_reph_pos - شروع) * حجم (info[0]));
      info[new_reph_pos] = reph;

      إذا (شروع < ادھار && ادھار <= new_reph_pos)
	ادھار--;
    }
  }


  /*   o Reorder pre-base-reordering consonants:
   *
   *     If a pre-base-reordering consonant is found, reorder it according to
   *     the following rules:
   */

  إذا (try_pref && ادھار + 1 < سرا) /* Otherwise there can't be any pre-base-reordering Ra. */
  {
    لكل (بنانشانی صحيح i = ادھار + 1; i < سرا; i++)
      إذا ((info[i].نقاب & indic_plan->نقاب_array[INDIC_PREF]) != 0)
      {
	/*       1. Only reorder a glyph produced by substitution during application
	 *          of the <pref> feature. (Note that a font may shape a Ra consonant with
	 *          the feature generally but block it in certain contexts.)
	 */
	/* Note: We just check that something got substituted.  We don't check that
	 * the <pref> feature actually did it...
	 *
	 * Reorder pref only if it ligated. */
	إذا (_hb_glyph_info_ligated_and_didnt_multiply (&info[i]))
	{
	  /*
	   *       2. Try to find a target position the same way as for pre-base matra.
	   *          If it is found, reorder pre-base consonant glyph.
	   *
	   *       3. If position is not found, reorder immediately before main
	   *          consonant.
	   */

	  بنانشانی صحيح new_pos = ادھار;
	  /* Malayalam / Tamil do not have "half" forms or explicit virama forms.
	   * The glyphs formed by 'half' are Chillus or ligated explicit viramas.
	   * We want to position matra after them.
	   */
	  إذا (بفر->props.script != HB_SCRIPT_MALAYALAM && بفر->props.script != HB_SCRIPT_TAMIL)
	  {
	    طالما (new_pos > شروع &&
		   !(is_one_دا (info[new_pos - 1], FLAG (I_Cat(M)) | FLAG (I_Cat(MPst)) | FLAG (I_Cat(H)))))
	      new_pos--;
	  }

	  إذا (new_pos > شروع && is_halant (info[new_pos - 1]))
	  {
	    /* -> If ZWJ or ZWNJ follow this halant, position is moved after it. */
	    إذا (new_pos < سرا && is_joiner (info[new_pos]))
	      new_pos++;
	  }

	  {
	    بنانشانی صحيح old_pos = i;

	    بفر->ملاؤ_clusters (new_pos, old_pos + 1);
	    hb_glyph_info_t tmp = info[old_pos];
	    memmove (&info[new_pos + 1], &info[new_pos], (old_pos - new_pos) * حجم (info[0]));
	    info[new_pos] = tmp;

	    إذا (new_pos <= ادھار && ادھار < old_pos)
	      ادھار++;
	  }
	}

	اكسر;
      }
  }


  /* Apply 'init' to the Left Matra if it's a word start. */
  إذا (info[شروع].indic_position () == POS_PRE_M)
  {
    إذا (!شروع ||
	!(FLAG_UNSAFE (_hb_glyph_info_get_general_category (&info[شروع - 1])) &
	 FLAG_RANGE (HB_UNICODE_GENERAL_CATEGORY_FORMAT, HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)))
      info[شروع].نقاب |= indic_plan->نقاب_array[INDIC_INIT];
    وإلا
      بفر->unsafe_منزل_break (شروع - 1, شروع + 1);
  }
}


ساكن منطقي
final_reordering_indic (ثابت hb_ot_shape_plan_t *plan,
			hb_font_t *font HB_بن_ورتیا,
			hb_بفر_t *بفر)
{
  بنانشانی صحيح گنتی = بفر->len;
  إذا (unlikely (!گنتی)) أرجع خطأ;

  إذا (بفر->سنیہا (font, "start reordering indic final")) {
    foreach_syllable (بفر, شروع, سرا)
      final_reordering_syllable_indic (plan, بفر, شروع, سرا);
    (فراغ) بفر->سنیہا (font, "end reordering indic final");
  }

  HB_بفر_DEALLOCATE_VAR (بفر, indic_category);
  HB_بفر_DEALLOCATE_VAR (بفر, indic_position);

  أرجع خطأ;
}


ساكن فراغ
preprocess_متن_indic (ثابت hb_ot_shape_plan_t *plan,
		       hb_بفر_t              *بفر,
		       hb_font_t                *font)
{
  hb_preprocess_متن_vowel_constraints (plan, بفر, font);
}

ساكن منطقي
decompose_indic (ثابت hb_ot_shape_normalize_context_t *c,
		 hb_codepoint_t  ab,
		 hb_codepoint_t *a,
		 hb_codepoint_t *b)
{
  بدل (ab)
  {
    /* Don't decompose these. */
    حالة 0x0931u  : أرجع خطأ; /* DEVANAGARI LETTER RRA */
    // https://github.com/harfbuzz/harfbuzz/issues/779
    حالة 0x09DCu  : أرجع خطأ; /* BENGALI LETTER RRA */
    حالة 0x09DDu  : أرجع خطأ; /* BENGALI LETTER RHA */
    حالة 0x0B94u  : أرجع خطأ; /* TAMIL LETTER AU */


    /*
     * Decompose split matras that don't have Unicode decompositions.
     */

#إذا 0
    /* Gujarati */
    /* This one has no decomposition in Unicode, but needs no decomposition either. */
    /* case 0x0AC9u  : return false; */

    /* Oriya */
    حالة 0x0B57u  : *a = no decomp, -> دایاں_2; أرجع سچ;
#جے_انت
  }

  أرجع (منطقي) c->unicode->decompose (ab, a, b);
}

ساكن منطقي
compose_indic (ثابت hb_ot_shape_normalize_context_t *c,
	       hb_codepoint_t  a,
	       hb_codepoint_t  b,
	       hb_codepoint_t *ab)
{
  /* Avoid recomposing split matras. */
  إذا (HB_UNICODE_GENERAL_CATEGORY_IS_MARK (c->unicode->general_category (a)))
    أرجع خطأ;

  /* Composition-exclusion exceptions that we want to recompose. */
  إذا (a == 0x09AFu && b == 0x09BCu) { *ab = 0x09DFu; أرجع سچ; }

  أرجع (منطقي) c->unicode->compose (a, b, ab);
}


ثابت hb_ot_shaper_t _hb_ot_shaper_indic =
{
  collect_features_indic,
  override_features_indic,
  انکڑے_create_indic,
  انکڑے_destroy_indic,
  preprocess_متن_indic,
  خالی_سوچک, /* postprocess_glyphs */
  decompose_indic,
  compose_indic,
  تیاری_masks_indic,
  خالی_سوچک, /* reorder_marks */
  HB_ٹیگ_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_منوال_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT,
  HB_OT_SHAPE_ZERO_چوڑائی_MARKS_NONE,
  خطأ, /* fallback_position */
};


#جے_انت

/*
 * Copyright © 2010,2012  Google, Inc.
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

#అన్తర్భావ "hb.hh"

#నిర్వచితం_కాకపోతే HB_NO_OT_SHAPE

#అన్తర్భావ "hb-ot-shaper-arabic.hh"
#అన్తర్భావ "hb-ot-shape.hh"


/* buffer var allocations */
#నిర్వచించు arabic_shaping_action() ot_shaper_var_u8_auxiliary() /* arabic shaping action */

/* buffer **position** var allocations, used after GPOS */
#నిర్వచించు stch_advance() var.i32 /* advance used for stretching */

#నిర్వచించు HB_బఫరు_SCRATCH_FLAG_ARABIC_HAS_STCH HB_బఫరు_SCRATCH_FLAG_SHAPER0

/* See:
 * https://github.com/harfbuzz/harfbuzz/commit/6e6f82b6f3dde0fc6c3c7d991d9ec6cfff57823d#commitcomment-14248516 */
#నిర్వచించు HB_ARABIC_GENERAL_CATEGORY_IS_మాట(gen_cat) \
	(FLAG_UNSAFE (gen_cat) & \
	 (FLAG (HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_వ్యక్తిగత_USE) | \
	  /*FLAG (HB_UNICODE_GENERAL_CATEGORY_LOWERCASE_LETTER) |*/ \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_MODIFIER_LETTER) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER) | \
	  /*FLAG (HB_UNICODE_GENERAL_CATEGORY_TITLECASE_LETTER) |*/ \
	  /*FLAG (HB_UNICODE_GENERAL_CATEGORY_UPPERCASE_LETTER) |*/ \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_SPACING_MARK) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_ENCLOSING_MARK) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_DECIMAL_సంఖ్య) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_LETTER_సంఖ్య) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_OTHER_సంఖ్య) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_CURRENCY_చిహ్నం) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_MODIFIER_చిహ్నం) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_MATH_చిహ్నం) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_OTHER_చిహ్నం)))


/*
 * Joining types:
 */

/*
 * Bits used in the joining tables
 */
గణన hb_arabic_joining_type_t {
  JOINING_TYPE_U		= 0,
  JOINING_TYPE_L		= 1,
  JOINING_TYPE_R		= 2,
  JOINING_TYPE_D		= 3,
  JOINING_TYPE_C		= JOINING_TYPE_D,
  JOINING_సమూహం_ALAPH		= 4,
  JOINING_సమూహం_DALATH_RISH	= 5,
  అంకె_STATE_MACHINE_నిలువుల_సంఖ్య	= 6,

  JOINING_TYPE_T = 6,
  JOINING_TYPE_X = 7  /* means: use general-category to choose between U or T. */
};

#అన్తర్భావ "hb-ot-shaper-arabic-table.hh"

స్థిర గుర్తులేని పూర్ణాంకం get_joining_type (hb_codepoint_t u, hb_unicode_general_category_t gen_cat)
{
  గుర్తులేని పూర్ణాంకం j_type = joining_type(u);
  ఒకవేళ (likely (j_type != JOINING_TYPE_X))
    తిరిగి j_type;

  తిరిగి (FLAG_UNSAFE(gen_cat) &
	  (FLAG(HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK) |
	   FLAG(HB_UNICODE_GENERAL_CATEGORY_ENCLOSING_MARK) |
	   FLAG(HB_UNICODE_GENERAL_CATEGORY_FORMAT))
	 ) ?  JOINING_TYPE_T : JOINING_TYPE_U;
}

#నిర్వచించు FEATURE_IS_SYRIAC(ట్యాగ్_2) hb_లోపల_range<గుర్తులేని అక్షరం> ((గుర్తులేని అక్షరం) (ట్యాగ్_2), '2', '3')

స్థిర స్థిరరాశి hb_ట్యాగ్_t arabic_features[] =
{
  HB_ట్యాగ్('i','s','o','l'),
  HB_ట్యాగ్('f','i','n','a'),
  HB_ట్యాగ్('f','i','n','2'),
  HB_ట్యాగ్('f','i','n','3'),
  HB_ట్యాగ్('m','e','d','i'),
  HB_ట్యాగ్('m','e','d','2'),
  HB_ట్యాగ్('i','n','i','t'),
  HB_ట్యాగ్_NONE
};


/* Same order as the feature array */
గణన arabic_action_t {
  ISOL,
  FINA,
  FIN2,
  FIN3,
  MEDI,
  MED2,
  INIT,

  NONE,

  ARABIC_అంకె_FEATURES = NONE,

  /* We abuse the same byte for other things... */
  STCH_FIXED,
  STCH_REPEATING,
};

స్థిర స్థిరరాశి నిర్మాణం arabic_state_పట్టిక_నమోదు {
	uint8_t prev_action;
	uint8_t curr_action;
	uint16_t తదుపరి_state;
} arabic_state_పట్టిక[][అంకె_STATE_MACHINE_నిలువుల_సంఖ్య] =
{
  /*   jt_U,          jt_L,          jt_R,          jt_D,          jg_ALAPH,      jg_DALATH_RISH */

  /* State 0: prev was U, not willing to join. */
  { {NONE,NONE,0}, {NONE,ISOL,2}, {NONE,ISOL,1}, {NONE,ISOL,2}, {NONE,ISOL,1}, {NONE,ISOL,6}, },

  /* State 1: prev was R or ISOL/ALAPH, not willing to join. */
  { {NONE,NONE,0}, {NONE,ISOL,2}, {NONE,ISOL,1}, {NONE,ISOL,2}, {NONE,FIN2,5}, {NONE,ISOL,6}, },

  /* State 2: prev was D/L in ISOL form, willing to join. */
  { {NONE,NONE,0}, {NONE,ISOL,2}, {INIT,FINA,1}, {INIT,FINA,3}, {INIT,FINA,4}, {INIT,FINA,6}, },

  /* State 3: prev was D in FINA form, willing to join. */
  { {NONE,NONE,0}, {NONE,ISOL,2}, {MEDI,FINA,1}, {MEDI,FINA,3}, {MEDI,FINA,4}, {MEDI,FINA,6}, },

  /* State 4: prev was FINA ALAPH, not willing to join. */
  { {NONE,NONE,0}, {NONE,ISOL,2}, {MED2,ISOL,1}, {MED2,ISOL,2}, {MED2,FIN2,5}, {MED2,ISOL,6}, },

  /* State 5: prev was FIN2/FIN3 ALAPH, not willing to join. */
  { {NONE,NONE,0}, {NONE,ISOL,2}, {ISOL,ISOL,1}, {ISOL,ISOL,2}, {ISOL,FIN2,5}, {ISOL,ISOL,6}, },

  /* State 6: prev was DALATH/RISH, not willing to join. */
  { {NONE,NONE,0}, {NONE,ISOL,2}, {NONE,ISOL,1}, {NONE,ISOL,2}, {NONE,FIN3,5}, {NONE,ISOL,6}, }
};


స్థిర తార్కిక
arabic_fallback_shape (స్థిరరాశి hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_బఫరు_t *బఫరు);

స్థిర తార్కిక
రికార్డు_stch (స్థిరరాశి hb_ot_shape_plan_t *plan,
	     hb_font_t *font,
	     hb_బఫరు_t *బఫరు);

స్థిర తార్కిక
deallocate_బఫరు_var (స్థిరరాశి hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_బఫరు_t *బఫరు)
{
  HB_బఫరు_DEALLOCATE_VAR (బఫరు, arabic_shaping_action);
  తిరిగి అబద్ధం;
}

స్థిర శూన్య
collect_features_arabic (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  /* We apply features according to the Arabic spec, with pauses
   * in between most.
   *
   * The pause between init/medi/... and rlig is required.  See eg:
   * https://bugzilla.mozilla.org/show_bug.cgi?id=644184
   *
   * The pauses between init/medi/... themselves are not necessarily
   * needed as only one of those features is applied to any character.
   * The only difference it makes is when fonts have contextual
   * substitutions.  We now follow the order of the spec, which makes
   * for better experience if that's what Uniscribe is doing.
   *
   * At least for Arabic, looks like Uniscribe has a pause between
   * rlig and calt.  Otherwise the IranNastaliq's ALLAH ligature won't
   * work.  However, testing shows that rlig and calt are applied
   * together for Mongolian in Uniscribe.  As such, we only add a
   * pause for Arabic, not other scripts.
   */


  map->enable_feature (HB_ట్యాగ్('s','t','c','h'));
  map->కూడు_gsub_pause (రికార్డు_stch);

  map->enable_feature (HB_ట్యాగ్('c','c','m','p'), F_MANUAL_ZWJ);
  map->enable_feature (HB_ట్యాగ్('l','o','c','l'), F_MANUAL_ZWJ);

  map->కూడు_gsub_pause (శూన్యసూచిక);

  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < ARABIC_అంకె_FEATURES; i++)
  {
    తార్కిక has_fallback = plan->props.script == HB_SCRIPT_ARABIC && !FEATURE_IS_SYRIAC (arabic_features[i]);
    map->కూడు_feature (arabic_features[i], F_MANUAL_ZWJ | (has_fallback ? F_HAS_FALLBACK : F_NONE));
    map->కూడు_gsub_pause (శూన్యసూచిక);
  }
   map->కూడు_gsub_pause (deallocate_బఫరు_var);

  /* Normally, Unicode says a ZWNJ means "don't ligate".  In Arabic script
   * however, it says a ZWJ should also mean "don't ligate".  So we run
   * the main ligating features as MANUAL_ZWJ. */

  map->enable_feature (HB_ట్యాగ్('r','l','i','g'), F_MANUAL_ZWJ | F_HAS_FALLBACK);

  ఒకవేళ (plan->props.script == HB_SCRIPT_ARABIC)
    map->కూడు_gsub_pause (arabic_fallback_shape);

   map->enable_feature (HB_ట్యాగ్('c','a','l','t'), F_MANUAL_ZWJ);
   /* https://github.com/harfbuzz/harfbuzz/issues/1573 */
   ఒకవేళ (!map->has_feature (HB_ట్యాగ్('r','c','l','t')))
     map->కూడు_gsub_pause (శూన్యసూచిక);

   map->enable_feature (HB_ట్యాగ్('l','i','g','a'), F_MANUAL_ZWJ);
   map->enable_feature (HB_ట్యాగ్('c','l','i','g'), F_MANUAL_ZWJ);

  /* The spec includes 'cswh'.  Earlier versions of Windows
   * used to enable this by default, but testing suggests
   * that Windows 8 and later do not enable it by default,
   * and spec now says 'Off by default'.
   * We disabled this in ae23c24c32.
   * Note that IranNastaliq uses this feature extensively
   * to fixup broken glyph sequences.  Oh well...
   * Test case: U+0643,U+0640,U+0631. */
  //map->enable_feature (HB_TAG('c','s','w','h'), F_MANUAL_ZWJ);
  map->enable_feature (HB_ట్యాగ్('m','s','e','t'), F_MANUAL_ZWJ);
}

#అన్తర్భావ "hb-ot-shaper-arabic-fallback.hh"

నిర్మాణం arabic_shape_plan_t
{
  /* The "+ 1" in the next array is to accommodate for the "NONE" command,
   * which is not an OpenType feature, but this simplifies the code by not
   * having to do a "if (... < NONE) ..." and just rely on the fact that
   * mask_array[NONE] == 0. */
  hb_ముసుగు_t ముసుగు_array[ARABIC_అంకె_FEATURES + 1];

  మార్చదగిన hb_atomic_t<arabic_fallback_plan_t *> fallback_plan;

  గుర్తులేని పూర్ణాంకం do_fallback : 1;
  గుర్తులేని పూర్ణాంకం has_stch : 1;
};

శూన్య *
దత్తాంశం_create_arabic (స్థిరరాశి hb_ot_shape_plan_t *plan)
{
  arabic_shape_plan_t *arabic_plan = (arabic_shape_plan_t *) hb_calloc (1, పరిమాణం (arabic_shape_plan_t));
  ఒకవేళ (unlikely (!arabic_plan))
    తిరిగి శూన్యసూచిక;

  arabic_plan->do_fallback = plan->props.script == HB_SCRIPT_ARABIC;
  arabic_plan->has_stch = !!plan->map.get_1_ముసుగు (HB_ట్యాగ్ ('s','t','c','h'));
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < ARABIC_అంకె_FEATURES; i++) {
    arabic_plan->ముసుగు_array[i] = plan->map.get_1_ముసుగు (arabic_features[i]);
    arabic_plan->do_fallback = arabic_plan->do_fallback &&
			       (FEATURE_IS_SYRIAC (arabic_features[i]) ||
				plan->map.needs_fallback (arabic_features[i]));
  }

  తిరిగి arabic_plan;
}

శూన్య
దత్తాంశం_destroy_arabic (శూన్య *దత్తాంశం)
{
  arabic_shape_plan_t *arabic_plan = (arabic_shape_plan_t *) దత్తాంశం;

  arabic_fallback_plan_destroy (arabic_plan->fallback_plan);

  hb_free (దత్తాంశం);
}

స్థిర శూన్య
arabic_joining (hb_బఫరు_t *బఫరు)
{
  గుర్తులేని పూర్ణాంకం లెక్క = బఫరు->len;
  hb_glyph_info_t *info = బఫరు->info;
  గుర్తులేని పూర్ణాంకం prev = UINT_MAX, state = 0;

  /* Check pre-context */
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < బఫరు->context_len[0]; i++)
  {
    గుర్తులేని పూర్ణాంకం this_type = get_joining_type (బఫరు->context[0][i], బఫరు->unicode->general_category (బఫరు->context[0][i]));

    ఒకవేళ (unlikely (this_type == JOINING_TYPE_T))
      కొనసాగు;

    స్థిరరాశి arabic_state_పట్టిక_నమోదు *నమోదు_2 = &arabic_state_పట్టిక[state][this_type];
    state = నమోదు_2->తదుపరి_state;
    ఆపు;
  }

  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
  {
    గుర్తులేని పూర్ణాంకం this_type = get_joining_type (info[i].codepoint, _hb_glyph_info_get_general_category (&info[i]));

    ఒకవేళ (unlikely (this_type == JOINING_TYPE_T)) {
      info[i].arabic_shaping_action() = NONE;
      కొనసాగు;
    }

    స్థిరరాశి arabic_state_పట్టిక_నమోదు *నమోదు_2 = &arabic_state_పట్టిక[state][this_type];

    ఒకవేళ (నమోదు_2->prev_action != NONE && prev != UINT_MAX)
    {
      info[prev].arabic_shaping_action() = నమోదు_2->prev_action;
      బఫరు->సురక్షిత_గమ్యం_చొప్పించు_tatweel (prev, i + 1);
    }
    లేకపోతే
    {
      ఒకవేళ (prev == UINT_MAX)
      {
        ఒకవేళ (this_type >= JOINING_TYPE_R)
	  బఫరు->unsafe_గమ్యం_concat_from_outbuffer (0, i + 1);
      }
      లేకపోతే
      {
	ఒకవేళ (this_type >= JOINING_TYPE_R ||
	    (2 <= state && state <= 5) /* States that have a possible prev_action. */)
	  బఫరు->unsafe_గమ్యం_concat (prev, i + 1);
      }
    }

    info[i].arabic_shaping_action() = నమోదు_2->curr_action;

    prev = i;
    state = నమోదు_2->తదుపరి_state;
  }

  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < బఫరు->context_len[1]; i++)
  {
    గుర్తులేని పూర్ణాంకం this_type = get_joining_type (బఫరు->context[1][i], బఫరు->unicode->general_category (బఫరు->context[1][i]));

    ఒకవేళ (unlikely (this_type == JOINING_TYPE_T))
      కొనసాగు;

    స్థిరరాశి arabic_state_పట్టిక_నమోదు *నమోదు_2 = &arabic_state_పట్టిక[state][this_type];
    ఒకవేళ (నమోదు_2->prev_action != NONE && prev != UINT_MAX)
    {
      info[prev].arabic_shaping_action() = నమోదు_2->prev_action;
      బఫరు->సురక్షిత_గమ్యం_చొప్పించు_tatweel (prev, బఫరు->len);
    }
    లేకపోతే ఒకవేళ (2 <= state && state <= 5) /* States that have a possible prev_action. */
    {
      బఫరు->unsafe_గమ్యం_concat (prev, బఫరు->len);
    }
    ఆపు;
  }
}

స్థిర శూన్య
mongolian_variation_selectors (hb_బఫరు_t *బఫరు)
{
  /* Copy arabic_shaping_action() from base to Mongolian variation selectors. */
  గుర్తులేని పూర్ణాంకం లెక్క = బఫరు->len;
  hb_glyph_info_t *info = బఫరు->info;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 1; i < లెక్క; i++)
    ఒకవేళ (unlikely (hb_లోపల_ranges<hb_codepoint_t> (info[i].codepoint, 0x180Bu, 0x180Du, 0x180Fu, 0x180Fu)))
      info[i].arabic_shaping_action() = info[i - 1].arabic_shaping_action();
}

శూన్య
అమరిక_masks_arabic_plan (స్థిరరాశి arabic_shape_plan_t *arabic_plan,
			 hb_బఫరు_t               *బఫరు,
			 hb_script_t                script)
{
  HB_బఫరు_ALLOCATE_VAR (బఫరు, arabic_shaping_action);

  arabic_joining (బఫరు);
  ఒకవేళ (script == HB_SCRIPT_MONGOLIAN)
    mongolian_variation_selectors (బఫరు);

  గుర్తులేని పూర్ణాంకం లెక్క = బఫరు->len;
  hb_glyph_info_t *info = బఫరు->info;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    info[i].ముసుగు |= arabic_plan->ముసుగు_array[info[i].arabic_shaping_action()];
}

స్థిర శూన్య
అమరిక_masks_arabic (స్థిరరాశి hb_ot_shape_plan_t *plan,
		    hb_బఫరు_t              *బఫరు,
		    hb_font_t                *font HB_వాడని)
{
  స్థిరరాశి arabic_shape_plan_t *arabic_plan = (స్థిరరాశి arabic_shape_plan_t *) plan->దత్తాంశం;
  అమరిక_masks_arabic_plan (arabic_plan, బఫరు, plan->props.script);
}

స్థిర తార్కిక
arabic_fallback_shape (స్థిరరాశి hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_బఫరు_t *బఫరు)
{
#నిర్వచితమైతే HB_NO_OT_SHAPER_ARABIC_FALLBACK
  తిరిగి అబద్ధం;
#అయితే_ముగింపు

  స్థిరరాశి arabic_shape_plan_t *arabic_plan = (స్థిరరాశి arabic_shape_plan_t *) plan->దత్తాంశం;

  ఒకవేళ (!arabic_plan->do_fallback)
    తిరిగి అబద్ధం;

retry:
  arabic_fallback_plan_t *fallback_plan = arabic_plan->fallback_plan;
  ఒకవేళ (unlikely (!fallback_plan))
  {
    /* This sucks.  We need a font to build the fallback plan... */
    fallback_plan = arabic_fallback_plan_create (plan, font);
    ఒకవేళ (unlikely (!arabic_plan->fallback_plan.cmpexch (శూన్యసూచిక, fallback_plan)))
    {
      arabic_fallback_plan_destroy (fallback_plan);
      వెళ్ళు retry;
    }
  }

  arabic_fallback_plan_shape (fallback_plan, font, బఫరు);
  తిరిగి నిజం;
}

/*
 * Stretch feature: "stch".
 * See example here:
 * https://docs.microsoft.com/en-us/typography/script-development/syriac
 * We implement this in a generic way, such that the Arabic subtending
 * marks can use it as well.
 */

స్థిర తార్కిక
రికార్డు_stch (స్థిరరాశి hb_ot_shape_plan_t *plan,
	     hb_font_t *font HB_వాడని,
	     hb_బఫరు_t *బఫరు)
{
  స్థిరరాశి arabic_shape_plan_t *arabic_plan = (స్థిరరాశి arabic_shape_plan_t *) plan->దత్తాంశం;
  ఒకవేళ (!arabic_plan->has_stch)
    తిరిగి అబద్ధం;

  /* 'stch' feature was just applied.  Look for anything that multiplied,
   * and record it for stch treatment later.  Note that rtlm, frac, etc
   * are applied before stch, but we assume that they didn't result in
   * anything multiplying into 5 pieces, so it's safe-ish... */

  గుర్తులేని పూర్ణాంకం లెక్క = బఫరు->len;
  hb_glyph_info_t *info = బఫరు->info;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    ఒకవేళ (unlikely (_hb_glyph_info_multiplied (&info[i])))
    {
      గుర్తులేని పూర్ణాంకం comp = _hb_glyph_info_get_lig_comp (&info[i]);
      info[i].arabic_shaping_action() = comp % 2 ? STCH_REPEATING : STCH_FIXED;
      బఫరు->scratch_flags |= HB_బఫరు_SCRATCH_FLAG_ARABIC_HAS_STCH;
    }
  తిరిగి అబద్ధం;
}

స్థిర శూన్య
apply_stch (స్థిరరాశి hb_ot_shape_plan_t *plan HB_వాడని,
	    hb_బఫరు_t              *బఫరు,
	    hb_font_t                *font)
{
  ఒకవేళ (likely (!(బఫరు->scratch_flags & HB_బఫరు_SCRATCH_FLAG_ARABIC_HAS_STCH)))
    తిరిగి;

  తార్కిక rtl = బఫరు->props.direction == HB_DIRECTION_RTL;

  ఒకవేళ (!rtl)
    బఫరు->reverse ();

  /* We do a two pass implementation:
   * First pass calculates the exact number of extra glyphs we need,
   * We then enlarge buffer to have that much room,
   * Second pass applies the stretch, copying things to the end of buffer.
   *
   * Cache glyph advances in the position scratch field during the first pass,
   * for re-use these during the second pass.
   */

  పూర్ణాంకం sign = font->x_scale < 0 ? -1 : +1;
  గుర్తులేని పూర్ణాంకం extra_glyphs_needed = 0; // Set during MEASURE, used during CUT
  గణన { MEASURE, CUT } /* step_t */;

  కొరకు (గుర్తులేని పూర్ణాంకం అడుగు = MEASURE; అడుగు <= CUT; అడుగు = అడుగు + 1)
  {
    గుర్తులేని పూర్ణాంకం లెక్క = బఫరు->len;
    hb_glyph_info_t *info = బఫరు->info;
    hb_glyph_position_t *pos = బఫరు->pos;
    గుర్తులేని పూర్ణాంకం new_len = లెక్క + extra_glyphs_needed; // write head during CUT
    గుర్తులేని పూర్ణాంకం j = new_len;
    కొరకు (గుర్తులేని పూర్ణాంకం i = లెక్క; i; i--)
    {
      ఒకవేళ (!hb_లోపల_range<uint8_t> (info[i - 1].arabic_shaping_action(), STCH_FIXED, STCH_REPEATING))
      {
	ఒకవేళ (అడుగు == CUT)
	{
	  --j;
	  info[j] = info[i - 1];
	  pos[j] = pos[i - 1];
	}
	కొనసాగు;
      }

      /* Yay, justification! */

      hb_position_t w_మొత్తం = 0; // Total to be filled
      hb_position_t w_fixed = 0; // Sum of fixed tiles
      hb_position_t w_repeating = 0; // Sum of repeating tiles
      పూర్ణాంకం n_fixed = 0;
      పూర్ణాంకం n_repeating = 0;

      గుర్తులేని పూర్ణాంకం చివర = i;
      అయితే (i &&
	     hb_లోపల_range<uint8_t> (info[i - 1].arabic_shaping_action(), STCH_FIXED, STCH_REPEATING))
      {
	i--;
        ఒకవేళ (అడుగు == MEASURE)
          pos[i].stch_advance() = font->get_glyph_h_advance (info[i].codepoint);
        hb_position_t వెడల్పు = pos[i].stch_advance();
	ఒకవేళ (info[i].arabic_shaping_action() == STCH_FIXED)
	{
	  w_fixed = hb_saturate_కూడు (w_fixed, వెడల్పు);
	  n_fixed++;
	}
	లేకపోతే
	{
	  w_repeating = hb_saturate_కూడు (w_repeating, వెడల్పు);
	  n_repeating++;
	}
      }
      గుర్తులేని పూర్ణాంకం ప్రారంభం = i;
      గుర్తులేని పూర్ణాంకం context = i;
      అయితే (context &&
	     !hb_లోపల_range<uint8_t> (info[context - 1].arabic_shaping_action(), STCH_FIXED, STCH_REPEATING) &&
	     (_hb_glyph_info_is_default_ignorable (&info[context - 1]) ||
	      HB_ARABIC_GENERAL_CATEGORY_IS_మాట (_hb_glyph_info_get_general_category (&info[context - 1]))))
      {
	context--;
	w_మొత్తం = hb_saturate_కూడు (w_మొత్తం, pos[context].x_advance);
      }
      i++; // Don't touch i again.

      DEBUG_సందేశం (ARABIC, శూన్యసూచిక, "%s stretch at (%u,%u,%u)",
		 అడుగు == MEASURE ? "measuring" : "cutting", context, ప్రారంభం, చివర);
      DEBUG_సందేశం (ARABIC, శూన్యసూచిక, "rest of word:    count=%u width %" PRId32, ప్రారంభం - context, w_మొత్తం);
      DEBUG_సందేశం (ARABIC, శూన్యసూచిక, "fixed tiles:     count=%d width=%" PRId32, n_fixed, w_fixed);
      DEBUG_సందేశం (ARABIC, శూన్యసూచిక, "repeating tiles: count=%d width=%" PRId32, n_repeating, w_repeating);

      స్థిర constexpr గుర్తులేని STCH_MAX_GLYPHS = 256;

      /* Number of additional times to repeat each repeating tile. */
      గుర్తులేని పూర్ణాంకం n_copies = 0;

      int64_t w_మిగిలిన_signed = (int64_t) w_మొత్తం - w_fixed;
      int64_t w_repeating_signed = w_repeating;
      ఒకవేళ (sign < 0)
      {
	w_మిగిలిన_signed = -w_మిగిలిన_signed;
	w_repeating_signed = -w_repeating_signed;
      }
      hb_position_t w_మిగిలిన = hb_saturate_sub (w_మొత్తం, w_fixed);
      ఒకవేళ (w_మిగిలిన_signed > w_repeating_signed && w_repeating_signed > 0)
	n_copies = w_మిగిలిన_signed / w_repeating_signed - 1;

      /* See if we can improve the fit by adding an extra repeat and squeezing them together a bit. */
      hb_position_t extra_repeat_overlap = 0;
      int64_t shortfall = w_మిగిలిన_signed - w_repeating_signed * (n_copies + 1);
      ఒకవేళ (shortfall > 0 && n_repeating > 0)
      {
	++n_copies;
	int64_t excess = (n_copies + 1) * w_repeating_signed - w_మిగిలిన_signed;
	ఒకవేళ (excess > 0)
	{
	  extra_repeat_overlap = hb_clamp_గమ్యం<hb_position_t> (excess / ((int64_t) n_copies * (int64_t) n_repeating));
	  w_మిగిలిన = 0;
	}
      }

      గుర్తులేని పూర్ణాంకం max_copies = 0;
      ఒకవేళ (n_repeating > 0)
      {
	గుర్తులేని పూర్ణాంకం పునాది_glyphs = n_fixed + n_repeating;
	ఒకవేళ (పునాది_glyphs < STCH_MAX_GLYPHS)
	  max_copies = (STCH_MAX_GLYPHS - పునాది_glyphs) / n_repeating;
      }
      n_copies = hb_min (n_copies, max_copies);

      ఒకవేళ (అడుగు == MEASURE)
      {
	గుర్తులేని పూర్ణాంకం added_glyphs = 0;
	ఒకవేళ (unlikely (hb_unsigned_mul_overflows (n_copies, n_repeating, &added_glyphs) ||
		      hb_unsigned_కూడు_overflows (extra_glyphs_needed, added_glyphs, &extra_glyphs_needed)))
	{
	  extra_glyphs_needed = UINT_MAX;
	  ఆపు;
	}
	DEBUG_సందేశం (ARABIC, శూన్యసూచిక, "will add extra %u copies of repeating tiles", n_copies);
      }
      లేకపోతే
      {
	బఫరు->unsafe_గమ్యం_break (context, చివర);
	hb_position_t x_స్థానభ్రంశం_2 = w_మిగిలిన / 2;
	కొరకు (గుర్తులేని పూర్ణాంకం k = చివర; k > ప్రారంభం; k--)
	{
          hb_position_t వెడల్పు = pos[k - 1].stch_advance();

	  గుర్తులేని పూర్ణాంకం repeat = 1;
	  ఒకవేళ (info[k - 1].arabic_shaping_action() == STCH_REPEATING)
	    repeat += n_copies;

	  DEBUG_సందేశం (ARABIC, శూన్యసూచిక, "appending %u copies of glyph %" PRIu32 "; j=%u",
		     repeat, info[k - 1].codepoint, j);
	  pos[k - 1].x_advance = 0;
	  కొరకు (గుర్తులేని పూర్ణాంకం n = 0; n < repeat; n++)
	  {
	    ఒకవేళ (rtl)
	    {
	      x_స్థానభ్రంశం_2 = hb_saturate_sub (x_స్థానభ్రంశం_2, వెడల్పు);
	      ఒకవేళ (n > 0)
		x_స్థానభ్రంశం_2 = hb_saturate_కూడు (x_స్థానభ్రంశం_2, extra_repeat_overlap);
	    }
	    pos[k - 1].x_స్థానభ్రంశం_2 = x_స్థానభ్రంశం_2;
	    /* Append copy. */
	    --j;
	    info[j] = info[k - 1];
	    pos[j] = pos[k - 1];

	    ఒకవేళ (!rtl)
	    {
	      x_స్థానభ్రంశం_2 = hb_saturate_కూడు (x_స్థానభ్రంశం_2, వెడల్పు);
	      ఒకవేళ (n > 0)
		x_స్థానభ్రంశం_2 = hb_saturate_sub (x_స్థానభ్రంశం_2, extra_repeat_overlap);
	    }
	  }
	}
      }
    }

    ఒకవేళ (అడుగు == MEASURE)
    {
      గుర్తులేని పూర్ణాంకం మొత్తం_glyphs = 0;
      ఒకవేళ (unlikely (hb_unsigned_కూడు_overflows (లెక్క, extra_glyphs_needed, &మొత్తం_glyphs) ||
		    !బఫరు->ensure (మొత్తం_glyphs)))
	ఆపు;
    }
    లేకపోతే
    {
      ప్రతిజ్ఞా (j == 0);
      బఫరు->len = new_len;
    }
  }

  ఒకవేళ (!rtl)
    బఫరు->reverse ();
}


స్థిర శూన్య
postprocess_glyphs_arabic (స్థిరరాశి hb_ot_shape_plan_t *plan,
			   hb_బఫరు_t              *బఫరు,
			   hb_font_t                *font)
{
  apply_stch (plan, బఫరు, font);
}

/* https://www.unicode.org/reports/tr53/ */

స్థిర స్థిరరాశి hb_codepoint_t
modifier_combining_marks[] =
{
  0x0654u, /* ARABIC HAMZA ABOVE */
  0x0655u, /* ARABIC HAMZA BELOW */
  0x0658u, /* ARABIC MARK NOON GHUNNA */
  0x06DCu, /* ARABIC SMALL HIGH SEEN */
  0x06E3u, /* ARABIC SMALL LOW SEEN */
  0x06E7u, /* ARABIC SMALL HIGH YEH */
  0x06E8u, /* ARABIC SMALL HIGH NOON */
  0x08CAu, /* ARABIC SMALL HIGH FARSI YEH */
  0x08CBu, /* ARABIC SMALL HIGH YEH BARREE WITH TWO DOTS BELOW */
  0x08CDu, /* ARABIC SMALL HIGH ZAH */
  0x08CEu, /* ARABIC LARGE ROUND DOT ABOVE */
  0x08CFu, /* ARABIC LARGE ROUND DOT BELOW */
  0x08D3u, /* ARABIC SMALL LOW WAW */
  0x08F3u, /* ARABIC SMALL HIGH WAW */
};

స్థిర అంతఃవరుస తార్కిక
info_is_mcm (స్థిరరాశి hb_glyph_info_t &info)
{
  hb_codepoint_t u = info.codepoint;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < ARRAY_నిడివి (modifier_combining_marks); i++)
    ఒకవేళ (u == modifier_combining_marks[i])
      తిరిగి నిజం;
  తిరిగి అబద్ధం;
}

స్థిర శూన్య
reorder_marks_arabic (స్థిరరాశి hb_ot_shape_plan_t *plan HB_వాడని,
		      hb_బఫరు_t              *బఫరు,
		      గుర్తులేని పూర్ణాంకం              ప్రారంభం,
		      గుర్తులేని పూర్ణాంకం              చివర)
{
  hb_glyph_info_t *info = బఫరు->info;

  DEBUG_సందేశం (ARABIC, బఫరు, "Reordering marks from %u to %u", ప్రారంభం, చివర);

  గుర్తులేని పూర్ణాంకం i = ప్రారంభం;
  కొరకు (గుర్తులేని పూర్ణాంకం cc = 220; cc <= 230; cc += 10)
  {
    DEBUG_సందేశం (ARABIC, బఫరు, "Looking for %u's starting at %u", cc, i);
    అయితే (i < చివర && info_cc(info[i]) < cc)
      i++;
    DEBUG_సందేశం (ARABIC, బఫరు, "Looking for %u's stopped at %u", cc, i);

    ఒకవేళ (i == చివర)
      ఆపు;

    ఒకవేళ (info_cc(info[i]) > cc)
      కొనసాగు;

    గుర్తులేని పూర్ణాంకం j = i;
    అయితే (j < చివర && info_cc(info[j]) == cc && info_is_mcm (info[j]))
      j++;

    ఒకవేళ (i == j)
      కొనసాగు;

    DEBUG_సందేశం (ARABIC, బఫరు, "Found %u's from %u to %u", cc, i, j);

    /* Shift it! */
    DEBUG_సందేశం (ARABIC, బఫరు, "Shifting %u's: %u %u", cc, i, j);
    hb_glyph_info_t తాత్కాలిక[HB_OT_SHAPE_MAX_COMBINING_MARKS];
    ప్రతిజ్ఞా (j - i <= ARRAY_నిడివి (తాత్కాలిక));
    బఫరు->విలీనం_clusters (ప్రారంభం, j);
    memmove (తాత్కాలిక, &info[i], (j - i) * పరిమాణం (hb_glyph_info_t));
    memmove (&info[ప్రారంభం + j - i], &info[ప్రారంభం], (i - ప్రారంభం) * పరిమాణం (hb_glyph_info_t));
    memmove (&info[ప్రారంభం], తాత్కాలిక, (j - i) * పరిమాణం (hb_glyph_info_t));

    /* Renumber CC such that the reordered sequence is still sorted.
     * 22 and 26 are chosen because they are smaller than all Arabic categories,
     * and are folded back to 220/230 respectively during fallback mark positioning.
     *
     * We do this because the CGJ-handling logic in the normalizer relies on
     * mark sequences having an increasing order even after this reordering.
     * https://github.com/harfbuzz/harfbuzz/issues/554
     * This, however, does break some obscure sequences, where the normalizer
     * might compose a sequence that it should not.  For example, in the seequence
     * ALEF, HAMZAH, MADDAH, we should NOT try to compose ALEF+MADDAH, but with this
     * renumbering, we will.
     */
    గుర్తులేని పూర్ణాంకం new_ప్రారంభం = ప్రారంభం + j - i;
    గుర్తులేని పూర్ణాంకం new_cc = cc == 220 ? HB_MODIFIED_COMBINING_CLASS_CCC22 : HB_MODIFIED_COMBINING_CLASS_CCC26;
    అయితే (ప్రారంభం < new_ప్రారంభం)
    {
      hb_glyph_info_సమితి_modified_combining_class (&info[ప్రారంభం], new_cc);
      ప్రారంభం++;
    }

    i = j;
  }
}

స్థిరరాశి hb_ot_shaper_t _hb_ot_shaper_arabic =
{
  collect_features_arabic,
  శూన్యసూచిక, /* override_features */
  దత్తాంశం_create_arabic,
  దత్తాంశం_destroy_arabic,
  శూన్యసూచిక, /* preprocess_text */
  postprocess_glyphs_arabic,
  శూన్యసూచిక, /* decompose */
  శూన్యసూచిక, /* compose */
  అమరిక_masks_arabic,
  reorder_marks_arabic,
  HB_ట్యాగ్_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_బహుళకం_DEFAULT,
  HB_OT_SHAPE_ZERO_వెడల్పు_MARKS_BY_GDEF_LATE,
  నిజం, /* fallback_position */
};


#అయితే_ముగింపు

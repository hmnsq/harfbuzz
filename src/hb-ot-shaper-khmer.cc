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

#అన్తర్భావ "hb.hh"

#నిర్వచితం_కాకపోతే HB_NO_OT_SHAPE

#అన్తర్భావ "hb-ot-shaper-khmer-machine.hh"
#అన్తర్భావ "hb-ot-shaper-indic.hh"
#అన్తర్భావ "hb-ot-layout.hh"


/*
 * Khmer shaper.
 */


స్థిర స్థిరరాశి hb_ot_map_feature_t
khmer_features[] =
{
  /*
   * Basic features.
   * These features are applied all at once, before reordering, constrained
   * to the syllable.
   */
  {HB_ట్యాగ్('p','r','e','f'), F_MANUAL_JOINERS | F_చొప్పున_SYLLABLE},
  {HB_ట్యాగ్('b','l','w','f'), F_MANUAL_JOINERS | F_చొప్పున_SYLLABLE},
  {HB_ట్యాగ్('a','b','v','f'), F_MANUAL_JOINERS | F_చొప్పున_SYLLABLE},
  {HB_ట్యాగ్('p','s','t','f'), F_MANUAL_JOINERS | F_చొప్పున_SYLLABLE},
  {HB_ట్యాగ్('c','f','a','r'), F_MANUAL_JOINERS | F_చొప్పున_SYLLABLE},
  /*
   * Other features.
   * These features are applied all at once after clearing syllables.
   */
  {HB_ట్యాగ్('p','r','e','s'), F_GLOBAL_MANUAL_JOINERS},
  {HB_ట్యాగ్('a','b','v','s'), F_GLOBAL_MANUAL_JOINERS},
  {HB_ట్యాగ్('b','l','w','s'), F_GLOBAL_MANUAL_JOINERS},
  {HB_ట్యాగ్('p','s','t','s'), F_GLOBAL_MANUAL_JOINERS},
};

/*
 * Must be in the same order as the khmer_features array.
 */
గణన {
  KHMER_PREF,
  KHMER_BLWF,
  KHMER_ABVF,
  KHMER_PSTF,
  KHMER_CFAR,

  _KHMER_PRES,
  _KHMER_ABVS,
  _KHMER_BLWS,
  _KHMER_PSTS,

  KHMER_అంకె_FEATURES,
  KHMER_BASIC_FEATURES = _KHMER_PRES, /* Don't forget to update this! */
};

స్థిర అంతఃవరుస శూన్య
సమితి_khmer_properties (hb_glyph_info_t &info)
{
  hb_codepoint_t u = info.codepoint;
  గుర్తులేని పూర్ణాంకం type = hb_indic_get_categories (u);

  info.khmer_category() = (khmer_category_t) (type & 0xFFu);
}

స్థిర తార్కిక
అమరిక_syllables_khmer (స్థిరరాశి hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_బఫరు_t *బఫరు);
స్థిర తార్కిక
reorder_khmer (స్థిరరాశి hb_ot_shape_plan_t *plan,
	       hb_font_t *font,
	       hb_బఫరు_t *బఫరు);

స్థిర శూన్య
collect_features_khmer (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  /* Do this before any lookups have been applied. */
  map->కూడు_gsub_pause (అమరిక_syllables_khmer);
  map->కూడు_gsub_pause (reorder_khmer);

  /* Testing suggests that Uniscribe does NOT pause between basic
   * features.  Test with KhmerUI.ttf and the following three
   * sequences:
   *
   *   U+1789,U+17BC
   *   U+1789,U+17D2,U+1789
   *   U+1789,U+17D2,U+1789,U+17BC
   *
   * https://github.com/harfbuzz/harfbuzz/issues/974
   */
  map->enable_feature (HB_ట్యాగ్('l','o','c','l'), F_చొప్పున_SYLLABLE);
  map->enable_feature (HB_ట్యాగ్('c','c','m','p'), F_చొప్పున_SYLLABLE);

  గుర్తులేని పూర్ణాంకం i = 0;
  కొరకు (; i < KHMER_BASIC_FEATURES; i++)
    map->కూడు_feature (khmer_features[i]);

  /* https://github.com/harfbuzz/harfbuzz/issues/3531 */
  map->కూడు_gsub_pause (hb_syllabic_clear_var); // Don't need syllables anymore, use stop to free buffer var

  కొరకు (; i < KHMER_అంకె_FEATURES; i++)
    map->కూడు_feature (khmer_features[i]);
}

స్థిర శూన్య
override_features_khmer (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  /* Khmer spec has 'clig' as part of required shaping features:
   * "Apply feature 'clig' to form ligatures that are desired for
   * typographical correctness.", hence in overrides... */
  map->enable_feature (HB_ట్యాగ్('c','l','i','g'));

  map->disable_feature (HB_ట్యాగ్('l','i','g','a'));
}


నిర్మాణం khmer_shape_plan_t
{
  hb_ముసుగు_t ముసుగు_array[KHMER_అంకె_FEATURES];
};

స్థిర శూన్య *
దత్తాంశం_create_khmer (స్థిరరాశి hb_ot_shape_plan_t *plan)
{
  khmer_shape_plan_t *khmer_plan = (khmer_shape_plan_t *) hb_calloc (1, పరిమాణం (khmer_shape_plan_t));
  ఒకవేళ (unlikely (!khmer_plan))
    తిరిగి శూన్యసూచిక;

  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < ARRAY_నిడివి (khmer_plan->ముసుగు_array); i++)
    khmer_plan->ముసుగు_array[i] = (khmer_features[i].flags & F_GLOBAL) ?
				 0 : plan->map.get_1_ముసుగు (khmer_features[i].ట్యాగ్_2);

  తిరిగి khmer_plan;
}

స్థిర శూన్య
దత్తాంశం_destroy_khmer (శూన్య *దత్తాంశం)
{
  hb_free (దత్తాంశం);
}

స్థిర శూన్య
అమరిక_masks_khmer (స్థిరరాశి hb_ot_shape_plan_t *plan HB_వాడని,
		   hb_బఫరు_t              *బఫరు,
		   hb_font_t                *font HB_వాడని)
{
  HB_బఫరు_ALLOCATE_VAR (బఫరు, khmer_category);

  /* We cannot setup masks here.  We save information about characters
   * and setup masks later on in a pause-callback. */

  గుర్తులేని పూర్ణాంకం లెక్క = బఫరు->len;
  hb_glyph_info_t *info = బఫరు->info;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    సమితి_khmer_properties (info[i]);
}

స్థిర తార్కిక
అమరిక_syllables_khmer (స్థిరరాశి hb_ot_shape_plan_t *plan HB_వాడని,
		       hb_font_t *font HB_వాడని,
		       hb_బఫరు_t *బఫరు)
{
  HB_బఫరు_ALLOCATE_VAR (బఫరు, syllable);
  కనుగొను_syllables_khmer (బఫరు);
  foreach_syllable (బఫరు, ప్రారంభం, చివర)
    బఫరు->unsafe_గమ్యం_break (ప్రారంభం, చివర);
  తిరిగి అబద్ధం;
}


/* Rules from:
 * https://docs.microsoft.com/en-us/typography/script-development/devanagari */

స్థిర శూన్య
reorder_consonant_syllable (స్థిరరాశి hb_ot_shape_plan_t *plan,
			    hb_face_t *face HB_వాడని,
			    hb_బఫరు_t *బఫరు,
			    గుర్తులేని పూర్ణాంకం ప్రారంభం, గుర్తులేని పూర్ణాంకం చివర)
{
  స్థిరరాశి khmer_shape_plan_t *khmer_plan = (స్థిరరాశి khmer_shape_plan_t *) plan->దత్తాంశం;
  hb_glyph_info_t *info = బఫరు->info;

  /* Setup masks. */
  {
    /* Post-base */
    hb_ముసుగు_t ముసుగు = khmer_plan->ముసుగు_array[KHMER_BLWF] |
		     khmer_plan->ముసుగు_array[KHMER_ABVF] |
		     khmer_plan->ముసుగు_array[KHMER_PSTF];
    కొరకు (గుర్తులేని పూర్ణాంకం i = ప్రారంభం + 1; i < చివర; i++)
      info[i].ముసుగు  |= ముసుగు;
  }

  గుర్తులేని పూర్ణాంకం అంకె_coengs = 0;
  కొరకు (గుర్తులేని పూర్ణాంకం i = ప్రారంభం + 1; i < చివర; i++)
  {
    /* """
     * When a COENG + (Cons | IndV) combination are found (and subscript count
     * is less than two) the character combination is handled according to the
     * subscript type of the character following the COENG.
     *
     * ...
     *
     * Subscript Type 2 - The COENG + RO characters are reordered to immediately
     * before the base glyph. Then the COENG + RO characters are assigned to have
     * the 'pref' OpenType feature applied to them.
     * """
     */
    ఒకవేళ (info[i].khmer_category() == K_Cat(H) && అంకె_coengs <= 2 && i + 1 < చివర)
    {
      అంకె_coengs++;

      ఒకవేళ (info[i + 1].khmer_category() == K_Cat(Ra))
      {
	కొరకు (గుర్తులేని పూర్ణాంకం j = 0; j < 2; j++)
	  info[i + j].ముసుగు |= khmer_plan->ముసుగు_array[KHMER_PREF];

	/* Move the Coeng,Ro sequence to the start. */
	బఫరు->విలీనం_clusters (ప్రారంభం, i + 2);
	hb_glyph_info_t t0 = info[i];
	hb_glyph_info_t t1 = info[i + 1];
	memmove (&info[ప్రారంభం + 2], &info[ప్రారంభం], (i - ప్రారంభం) * పరిమాణం (info[0]));
	info[ప్రారంభం] = t0;
	info[ప్రారంభం + 1] = t1;

	/* Mark the subsequent stuff with 'cfar'.  Used in Khmer.
	 * Read the feature spec.
	 * This allows distinguishing the following cases with MS Khmer fonts:
	 * U+1784,U+17D2,U+179A,U+17D2,U+1782
	 * U+1784,U+17D2,U+1782,U+17D2,U+179A
	 */
	ఒకవేళ (khmer_plan->ముసుగు_array[KHMER_CFAR])
	  కొరకు (గుర్తులేని పూర్ణాంకం j = i + 2; j < చివర; j++)
	    info[j].ముసుగు |= khmer_plan->ముసుగు_array[KHMER_CFAR];

	అంకె_coengs = 2; /* Done. */
      }
    }

    /* Reorder left matra piece. */
    లేకపోతే ఒకవేళ (info[i].khmer_category() == K_Cat(VPre))
    {
      /* Move to the start. */
      బఫరు->విలీనం_clusters (ప్రారంభం, i + 1);
      hb_glyph_info_t t = info[i];
      memmove (&info[ప్రారంభం + 1], &info[ప్రారంభం], (i - ప్రారంభం) * పరిమాణం (info[0]));
      info[ప్రారంభం] = t;
    }
  }
}

స్థిర శూన్య
reorder_syllable_khmer (స్థిరరాశి hb_ot_shape_plan_t *plan,
			hb_face_t *face,
			hb_బఫరు_t *బఫరు,
			గుర్తులేని పూర్ణాంకం ప్రారంభం, గుర్తులేని పూర్ణాంకం చివర)
{
  khmer_syllable_type_t syllable_type = (khmer_syllable_type_t) (బఫరు->info[ప్రారంభం].syllable() & 0x0F);
  ఎంపిక (syllable_type)
  {
    స్థితి khmer_broken_cluster: /* We already inserted dotted-circles, so just call the consonant_syllable. */
    స్థితి khmer_consonant_syllable:
     reorder_consonant_syllable (plan, face, బఫరు, ప్రారంభం, చివర);
     ఆపు;

    స్థితి khmer_non_khmer_cluster:
      ఆపు;
  }
}

స్థిర తార్కిక
reorder_khmer (స్థిరరాశి hb_ot_shape_plan_t *plan,
	       hb_font_t *font,
	       hb_బఫరు_t *బఫరు)
{
  తార్కిక ret = అబద్ధం;
  ఒకవేళ (బఫరు->సందేశం (font, "start reordering khmer"))
  {
    ఒకవేళ (hb_syllabic_చొప్పించు_dotted_circles (font, బఫరు,
					   khmer_broken_cluster,
					   K_Cat(DOTTEDCIRCLE),
					   (గుర్తులేని) -1))
      ret = నిజం;

    foreach_syllable (బఫరు, ప్రారంభం, చివర)
      reorder_syllable_khmer (plan, font->face, బఫరు, ప్రారంభం, చివర);
    (శూన్య) బఫరు->సందేశం (font, "end reordering khmer");
  }
  HB_బఫరు_DEALLOCATE_VAR (బఫరు, khmer_category);

  తిరిగి ret;
}


స్థిర తార్కిక
decompose_khmer (స్థిరరాశి hb_ot_shape_normalize_context_t *c,
		 hb_codepoint_t  ab,
		 hb_codepoint_t *a,
		 hb_codepoint_t *b)
{
  ఎంపిక (ab)
  {
    /*
     * Decompose split matras that don't have Unicode decompositions.
     */

    /* Khmer */
    స్థితి 0x17BEu  : *a = 0x17C1u; *b= 0x17BEu; తిరిగి నిజం;
    స్థితి 0x17BFu  : *a = 0x17C1u; *b= 0x17BFu; తిరిగి నిజం;
    స్థితి 0x17C0u  : *a = 0x17C1u; *b= 0x17C0u; తిరిగి నిజం;
    స్థితి 0x17C4u  : *a = 0x17C1u; *b= 0x17C4u; తిరిగి నిజం;
    స్థితి 0x17C5u  : *a = 0x17C1u; *b= 0x17C5u; తిరిగి నిజం;
  }

  తిరిగి (తార్కిక) c->unicode->decompose (ab, a, b);
}

స్థిర తార్కిక
compose_khmer (స్థిరరాశి hb_ot_shape_normalize_context_t *c,
	       hb_codepoint_t  a,
	       hb_codepoint_t  b,
	       hb_codepoint_t *ab)
{
  /* Avoid recomposing split matras. */
  ఒకవేళ (HB_UNICODE_GENERAL_CATEGORY_IS_MARK (c->unicode->general_category (a)))
    తిరిగి అబద్ధం;

  తిరిగి (తార్కిక) c->unicode->compose (a, b, ab);
}


స్థిరరాశి hb_ot_shaper_t _hb_ot_shaper_khmer =
{
  collect_features_khmer,
  override_features_khmer,
  దత్తాంశం_create_khmer,
  దత్తాంశం_destroy_khmer,
  శూన్యసూచిక, /* preprocess_text */
  శూన్యసూచిక, /* postprocess_glyphs */
  decompose_khmer,
  compose_khmer,
  అమరిక_masks_khmer,
  శూన్యసూచిక, /* reorder_marks */
  HB_ట్యాగ్_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_బహుళకం_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT,
  HB_OT_SHAPE_ZERO_వెడల్పు_MARKS_NONE,
  అబద్ధం, /* fallback_position */
};


#అయితే_ముగింపు

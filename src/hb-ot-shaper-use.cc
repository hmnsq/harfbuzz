/*
 * Copyright © 2015  Mozilla Foundation.
 * Copyright © 2015  Google, Inc.
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
 * Mozilla Author(s): Jonathan Kew
 * Google Author(s): Behdad Esfahbod
 */

#అన్తర్భావ "hb.hh"

#నిర్వచితం_కాకపోతే HB_NO_OT_SHAPE

#అన్తర్భావ "hb-ot-shaper-use-machine.hh"
#అన్తర్భావ "hb-ot-shaper-use-table.hh"
#అన్తర్భావ "hb-ot-shaper-arabic.hh"
#అన్తర్భావ "hb-ot-shaper-arabic-joining-list.hh"
#అన్తర్భావ "hb-ot-shaper-vowel-constraints.hh"


/*
 * Universal Shaping Engine.
 * https://docs.microsoft.com/en-us/typography/script-development/use
 */

స్థిర స్థిరరాశి hb_ట్యాగ్_t
use_basic_features[] =
{
  /*
   * Basic features.
   * These features are applied all at once, before reordering, constrained
   * to the syllable.
   */
  HB_ట్యాగ్('r','k','r','f'),
  HB_ట్యాగ్('a','b','v','f'),
  HB_ట్యాగ్('b','l','w','f'),
  HB_ట్యాగ్('h','a','l','f'),
  HB_ట్యాగ్('p','s','t','f'),
  HB_ట్యాగ్('v','a','t','u'),
  HB_ట్యాగ్('c','j','c','t'),
};
స్థిర స్థిరరాశి hb_ట్యాగ్_t
use_topographical_features[] =
{
  HB_ట్యాగ్('i','s','o','l'),
  HB_ట్యాగ్('i','n','i','t'),
  HB_ట్యాగ్('m','e','d','i'),
  HB_ట్యాగ్('f','i','n','a'),
};
/* Same order as use_topographical_features. */
గణన joining_form_t {
  JOINING_FORM_ISOL,
  JOINING_FORM_INIT,
  JOINING_FORM_MEDI,
  JOINING_FORM_FINA,
  _JOINING_FORM_NONE
};
స్థిర స్థిరరాశి hb_ట్యాగ్_t
use_other_features[] =
{
  /*
   * Other features.
   * These features are applied all at once, after reordering and
   * clearing syllables.
   */
  HB_ట్యాగ్('a','b','v','s'),
  HB_ట్యాగ్('b','l','w','s'),
  HB_ట్యాగ్('h','a','l','n'),
  HB_ట్యాగ్('p','r','e','s'),
  HB_ట్యాగ్('p','s','t','s'),
};

స్థిర తార్కిక
అమరిక_syllables_use (స్థిరరాశి hb_ot_shape_plan_t *plan,
		     hb_font_t *font,
		     hb_బఫరు_t *బఫరు);
స్థిర తార్కిక
రికార్డు_rphf_use (స్థిరరాశి hb_ot_shape_plan_t *plan,
		 hb_font_t *font,
		 hb_బఫరు_t *బఫరు);
స్థిర తార్కిక
రికార్డు_pref_use (స్థిరరాశి hb_ot_shape_plan_t *plan,
		 hb_font_t *font,
		 hb_బఫరు_t *బఫరు);
స్థిర తార్కిక
reorder_use (స్థిరరాశి hb_ot_shape_plan_t *plan,
	     hb_font_t *font,
	     hb_బఫరు_t *బఫరు);

స్థిర శూన్య
collect_features_use (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  /* Do this before any lookups have been applied. */
  map->కూడు_gsub_pause (అమరిక_syllables_use);

  /* "Default glyph pre-processing group" */
  map->enable_feature (HB_ట్యాగ్('l','o','c','l'), F_చొప్పున_SYLLABLE);
  map->enable_feature (HB_ట్యాగ్('c','c','m','p'), F_చొప్పున_SYLLABLE);
  map->enable_feature (HB_ట్యాగ్('n','u','k','t'), F_చొప్పున_SYLLABLE);
  map->enable_feature (HB_ట్యాగ్('a','k','h','n'), F_MANUAL_ZWJ | F_చొప్పున_SYLLABLE);

  /* "Reordering group" */
  map->కూడు_gsub_pause (_hb_clear_substitution_flags);
  map->కూడు_feature (HB_ట్యాగ్('r','p','h','f'), F_MANUAL_ZWJ | F_చొప్పున_SYLLABLE);
  map->కూడు_gsub_pause (రికార్డు_rphf_use);
  map->కూడు_gsub_pause (_hb_clear_substitution_flags);
  map->enable_feature (HB_ట్యాగ్('p','r','e','f'), F_MANUAL_ZWJ | F_చొప్పున_SYLLABLE);
  map->కూడు_gsub_pause (రికార్డు_pref_use);

  /* "Orthographic unit shaping group" */
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < ARRAY_నిడివి (use_basic_features); i++)
    map->enable_feature (use_basic_features[i], F_MANUAL_ZWJ | F_చొప్పున_SYLLABLE);

  map->కూడు_gsub_pause (reorder_use);
  map->కూడు_gsub_pause (hb_syllabic_clear_var); // Don't need syllables anymore, use stop to free buffer var

  /* "Topographical features" */
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < ARRAY_నిడివి (use_topographical_features); i++)
    map->కూడు_feature (use_topographical_features[i]);
  map->కూడు_gsub_pause (శూన్యసూచిక);

  /* "Standard typographic presentation" */
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < ARRAY_నిడివి (use_other_features); i++)
    map->enable_feature (use_other_features[i], F_MANUAL_ZWJ);
}

నిర్మాణం use_shape_plan_t
{
  hb_ముసుగు_t rphf_ముసుగు;

  arabic_shape_plan_t *arabic_plan;
};

స్థిర శూన్య *
దత్తాంశం_create_use (స్థిరరాశి hb_ot_shape_plan_t *plan)
{
  use_shape_plan_t *use_plan = (use_shape_plan_t *) hb_calloc (1, పరిమాణం (use_shape_plan_t));
  ఒకవేళ (unlikely (!use_plan))
    తిరిగి శూన్యసూచిక;

  use_plan->rphf_ముసుగు = plan->map.get_1_ముసుగు (HB_ట్యాగ్('r','p','h','f'));

  ఒకవేళ (has_arabic_joining (plan->props.script))
  {
    use_plan->arabic_plan = (arabic_shape_plan_t *) దత్తాంశం_create_arabic (plan);
    ఒకవేళ (unlikely (!use_plan->arabic_plan))
    {
      hb_free (use_plan);
      తిరిగి శూన్యసూచిక;
    }
  }

  తిరిగి use_plan;
}

స్థిర శూన్య
దత్తాంశం_destroy_use (శూన్య *దత్తాంశం)
{
  use_shape_plan_t *use_plan = (use_shape_plan_t *) దత్తాంశం;

  ఒకవేళ (use_plan->arabic_plan)
    దత్తాంశం_destroy_arabic (use_plan->arabic_plan);

  hb_free (దత్తాంశం);
}

స్థిర శూన్య
అమరిక_masks_use (స్థిరరాశి hb_ot_shape_plan_t *plan,
		 hb_బఫరు_t              *బఫరు,
		 hb_font_t                *font HB_వాడని)
{
  స్థిరరాశి use_shape_plan_t *use_plan = (స్థిరరాశి use_shape_plan_t *) plan->దత్తాంశం;

  /* Do this before allocating use_category(). */
  ఒకవేళ (use_plan->arabic_plan)
  {
    అమరిక_masks_arabic_plan (use_plan->arabic_plan, బఫరు, plan->props.script);
  }

  HB_బఫరు_ALLOCATE_VAR (బఫరు, use_category);

  /* We cannot setup masks here.  We save information about characters
   * and setup masks later on in a pause-callback. */

  గుర్తులేని పూర్ణాంకం లెక్క = బఫరు->len;
  hb_glyph_info_t *info = బఫరు->info;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    info[i].use_category() = hb_use_get_category (info[i].codepoint);
}

స్థిర శూన్య
అమరిక_rphf_ముసుగు (స్థిరరాశి hb_ot_shape_plan_t *plan,
		 hb_బఫరు_t *బఫరు)
{
  స్థిరరాశి use_shape_plan_t *use_plan = (స్థిరరాశి use_shape_plan_t *) plan->దత్తాంశం;

  hb_ముసుగు_t ముసుగు = use_plan->rphf_ముసుగు;
  ఒకవేళ (!ముసుగు) తిరిగి;

  hb_glyph_info_t *info = బఫరు->info;

  foreach_syllable (బఫరు, ప్రారంభం, చివర)
  {
    గుర్తులేని పూర్ణాంకం హద్దు = info[ప్రారంభం].use_category() == USE(R) ? 1 : hb_min (3u, చివర - ప్రారంభం);
    కొరకు (గుర్తులేని పూర్ణాంకం i = ప్రారంభం; i < ప్రారంభం + హద్దు; i++)
      info[i].ముసుగు |= ముసుగు;
  }
}

స్థిర శూన్య
అమరిక_topographical_masks (స్థిరరాశి hb_ot_shape_plan_t *plan,
			   hb_బఫరు_t *బఫరు)
{
  స్థిరరాశి use_shape_plan_t *use_plan = (స్థిరరాశి use_shape_plan_t *) plan->దత్తాంశం;
  ఒకవేళ (use_plan->arabic_plan)
    తిరిగి;

  static_assert ((JOINING_FORM_INIT < 4 && JOINING_FORM_ISOL < 4 && JOINING_FORM_MEDI < 4 && JOINING_FORM_FINA < 4), "");
  hb_ముసుగు_t masks[4], all_masks = 0;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < 4; i++)
  {
    masks[i] = plan->map.get_1_ముసుగు (use_topographical_features[i]);
    ఒకవేళ (masks[i] == plan->map.get_global_ముసుగు ())
      masks[i] = 0;
    all_masks |= masks[i];
  }
  ఒకవేళ (!all_masks)
    తిరిగి;
  hb_ముసుగు_t other_masks = ~all_masks;

  గుర్తులేని పూర్ణాంకం చివరి_ప్రారంభం = 0;
  joining_form_t చివరి_form = _JOINING_FORM_NONE;
  hb_glyph_info_t *info = బఫరు->info;
  foreach_syllable (బఫరు, ప్రారంభం, చివర)
  {
    use_syllable_type_t syllable_type = (use_syllable_type_t) (info[ప్రారంభం].syllable() & 0x0F);
    ఎంపిక (syllable_type)
    {
      స్థితి use_hieroglyph_cluster:
      స్థితి use_non_cluster:
	/* These don't join.  Nothing to do. */
	చివరి_form = _JOINING_FORM_NONE;
	ఆపు;

      స్థితి use_virama_terminated_cluster:
      స్థితి use_sakot_terminated_cluster:
      స్థితి use_standard_cluster:
      స్థితి use_సంఖ్య_joiner_terminated_cluster:
      స్థితి use_numeral_cluster:
      స్థితి use_చిహ్నం_cluster:
      స్థితి use_broken_cluster:

	తార్కిక join = చివరి_form == JOINING_FORM_FINA || చివరి_form == JOINING_FORM_ISOL;

	ఒకవేళ (join)
	{
	  /* Fixup previous syllable's form. */
	  చివరి_form = చివరి_form == JOINING_FORM_FINA ? JOINING_FORM_MEDI : JOINING_FORM_INIT;
	  కొరకు (గుర్తులేని పూర్ణాంకం i = చివరి_ప్రారంభం; i < ప్రారంభం; i++)
	    info[i].ముసుగు = (info[i].ముసుగు & other_masks) | masks[చివరి_form];
	}

	/* Form for this syllable. */
	చివరి_form = join ? JOINING_FORM_FINA : JOINING_FORM_ISOL;
	కొరకు (గుర్తులేని పూర్ణాంకం i = ప్రారంభం; i < చివర; i++)
	  info[i].ముసుగు = (info[i].ముసుగు & other_masks) | masks[చివరి_form];

	ఆపు;
    }

    చివరి_ప్రారంభం = ప్రారంభం;
  }
}

స్థిర తార్కిక
అమరిక_syllables_use (స్థిరరాశి hb_ot_shape_plan_t *plan,
		     hb_font_t *font HB_వాడని,
		     hb_బఫరు_t *బఫరు)
{
  HB_బఫరు_ALLOCATE_VAR (బఫరు, syllable);
  కనుగొను_syllables_use (బఫరు);
  foreach_syllable (బఫరు, ప్రారంభం, చివర)
    బఫరు->unsafe_గమ్యం_break (ప్రారంభం, చివర);
  అమరిక_rphf_ముసుగు (plan, బఫరు);
  అమరిక_topographical_masks (plan, బఫరు);
  తిరిగి అబద్ధం;
}

స్థిర తార్కిక
రికార్డు_rphf_use (స్థిరరాశి hb_ot_shape_plan_t *plan,
		 hb_font_t *font HB_వాడని,
		 hb_బఫరు_t *బఫరు)
{
  స్థిరరాశి use_shape_plan_t *use_plan = (స్థిరరాశి use_shape_plan_t *) plan->దత్తాంశం;

  hb_ముసుగు_t ముసుగు = use_plan->rphf_ముసుగు;
  ఒకవేళ (!ముసుగు) తిరిగి అబద్ధం;
  hb_glyph_info_t *info = బఫరు->info;

  foreach_syllable (బఫరు, ప్రారంభం, చివర)
  {
    /* Mark a substituted repha as USE(R). */
    కొరకు (గుర్తులేని పూర్ణాంకం i = ప్రారంభం; i < చివర && (info[i].ముసుగు & ముసుగు); i++)
      ఒకవేళ (_hb_glyph_info_substituted (&info[i]))
      {
	info[i].use_category() = USE(R);
	ఆపు;
      }
  }
  తిరిగి అబద్ధం;
}

స్థిర తార్కిక
రికార్డు_pref_use (స్థిరరాశి hb_ot_shape_plan_t *plan HB_వాడని,
		 hb_font_t *font HB_వాడని,
		 hb_బఫరు_t *బఫరు)
{
  hb_glyph_info_t *info = బఫరు->info;

  foreach_syllable (బఫరు, ప్రారంభం, చివర)
  {
    /* Mark a substituted pref as VPre, as they behave the same way. */
    కొరకు (గుర్తులేని పూర్ణాంకం i = ప్రారంభం; i < చివర; i++)
      ఒకవేళ (_hb_glyph_info_substituted (&info[i]))
      {
	info[i].use_category() = USE(VPre);
	ఆపు;
      }
  }
  తిరిగి అబద్ధం;
}

స్థిర అంతఃవరుస తార్కిక
is_halant_use (స్థిరరాశి hb_glyph_info_t &info)
{
  తిరిగి (info.use_category() == USE(H) || info.use_category() == USE(HVM) || info.use_category() == USE(IS)) &&
	 !_hb_glyph_info_ligated (&info);
}

స్థిర శూన్య
reorder_syllable_use (hb_బఫరు_t *బఫరు, గుర్తులేని పూర్ణాంకం ప్రారంభం, గుర్తులేని పూర్ణాంకం చివర)
{
  use_syllable_type_t syllable_type = (use_syllable_type_t) (బఫరు->info[ప్రారంభం].syllable() & 0x0F);
  /* Only a few syllable types need reordering. */
  ఒకవేళ (unlikely (!(FLAG_UNSAFE (syllable_type) &
		  (FLAG (use_virama_terminated_cluster) |
		   FLAG (use_sakot_terminated_cluster) |
		   FLAG (use_standard_cluster) |
		   FLAG (use_చిహ్నం_cluster) |
		   FLAG (use_broken_cluster) |
		   0))))
    తిరిగి;

  hb_glyph_info_t *info = బఫరు->info;

#నిర్వచించు POST_పునాది_FLAGS64 (FLAG64 (USE(FAbv)) | \
			   FLAG64 (USE(FBlw)) | \
			   FLAG64 (USE(FPst)) | \
			   FLAG64 (USE(FMAbv)) | \
			   FLAG64 (USE(FMBlw)) | \
			   FLAG64 (USE(FMPst)) | \
			   FLAG64 (USE(MAbv)) | \
			   FLAG64 (USE(MBlw)) | \
			   FLAG64 (USE(MPst)) | \
			   FLAG64 (USE(MPre)) | \
			   FLAG64 (USE(VAbv)) | \
			   FLAG64 (USE(VBlw)) | \
			   FLAG64 (USE(VPst)) | \
			   FLAG64 (USE(VPre)) | \
			   FLAG64 (USE(VMAbv)) | \
			   FLAG64 (USE(VMBlw)) | \
			   FLAG64 (USE(VMPst)) | \
			   FLAG64 (USE(VMPre)))

  /* Move things forward. */
  ఒకవేళ (info[ప్రారంభం].use_category() == USE(R) && చివర - ప్రారంభం > 1)
  {
    /* Got a repha.  Reorder it towards the end, but before the first post-base
     * glyph. */
    కొరకు (గుర్తులేని పూర్ణాంకం i = ప్రారంభం + 1; i < చివర; i++)
    {
      తార్కిక is_post_పునాది_glyph = (FLAG64_UNSAFE (info[i].use_category()) & POST_పునాది_FLAGS64) ||
				is_halant_use (info[i]);
      ఒకవేళ (is_post_పునాది_glyph || i == చివర - 1)
      {
	/* If we hit a post-base glyph, move before it; otherwise move to the
	 * end. Shift things in between backward. */

	ఒకవేళ (is_post_పునాది_glyph)
	  i--;

	బఫరు->విలీనం_clusters (ప్రారంభం, i + 1);
	hb_glyph_info_t t = info[ప్రారంభం];
	memmove (&info[ప్రారంభం], &info[ప్రారంభం + 1], (i - ప్రారంభం) * పరిమాణం (info[0]));
	info[i] = t;

	ఆపు;
      }
    }
  }

  /* Move things back. */
  గుర్తులేని పూర్ణాంకం j = ప్రారంభం;
  కొరకు (గుర్తులేని పూర్ణాంకం i = ప్రారంభం; i < చివర; i++)
  {
    uint32_t flag = FLAG_UNSAFE (info[i].use_category());
    ఒకవేళ (is_halant_use (info[i]))
    {
      /* If we hit a halant, move after it; otherwise move to the beginning, and
       * shift things in between forward. */
      j = i + 1;
    }
    లేకపోతే ఒకవేళ (((flag) & (FLAG (USE(VPre)) | FLAG (USE(VMPre)))) &&
	     /* Only move the first component of a MultipleSubst. */
	     0 == _hb_glyph_info_get_lig_comp (&info[i]) &&
	     j < i)
    {
      బఫరు->విలీనం_clusters (j, i + 1);
      hb_glyph_info_t t = info[i];
      memmove (&info[j + 1], &info[j], (i - j) * పరిమాణం (info[0]));
      info[j] = t;
    }
  }
}

స్థిర తార్కిక
reorder_use (స్థిరరాశి hb_ot_shape_plan_t *plan,
	     hb_font_t *font,
	     hb_బఫరు_t *బఫరు)
{
  తార్కిక ret = అబద్ధం;
  ఒకవేళ (బఫరు->సందేశం (font, "start reordering USE"))
  {
    ఒకవేళ (hb_syllabic_చొప్పించు_dotted_circles (font, బఫరు,
					   use_broken_cluster,
					   USE(B),
					   USE(R)))
      ret = నిజం;

    foreach_syllable (బఫరు, ప్రారంభం, చివర)
      reorder_syllable_use (బఫరు, ప్రారంభం, చివర);

    (శూన్య) బఫరు->సందేశం (font, "end reordering USE");
  }

  HB_బఫరు_DEALLOCATE_VAR (బఫరు, use_category);

  తిరిగి ret;
}


స్థిర శూన్య
preprocess_పాఠ్యం_use (స్థిరరాశి hb_ot_shape_plan_t *plan,
		     hb_బఫరు_t              *బఫరు,
		     hb_font_t                *font)
{
  hb_preprocess_పాఠ్యం_vowel_constraints (plan, బఫరు, font);
}

స్థిర తార్కిక
compose_use (స్థిరరాశి hb_ot_shape_normalize_context_t *c,
	     hb_codepoint_t  a,
	     hb_codepoint_t  b,
	     hb_codepoint_t *ab)
{
  /* Avoid recomposing split matras. */
  ఒకవేళ (HB_UNICODE_GENERAL_CATEGORY_IS_MARK (c->unicode->general_category (a)))
    తిరిగి అబద్ధం;

  తిరిగి (తార్కిక)c->unicode->compose (a, b, ab);
}


స్థిరరాశి hb_ot_shaper_t _hb_ot_shaper_use =
{
  collect_features_use,
  శూన్యసూచిక, /* override_features */
  దత్తాంశం_create_use,
  దత్తాంశం_destroy_use,
  preprocess_పాఠ్యం_use,
  శూన్యసూచిక, /* postprocess_glyphs */
  శూన్యసూచిక, /* decompose */
  compose_use,
  అమరిక_masks_use,
  శూన్యసూచిక, /* reorder_marks */
  HB_ట్యాగ్_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_బహుళకం_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT,
  HB_OT_SHAPE_ZERO_వెడల్పు_MARKS_BY_GDEF_EARLY,
  అబద్ధం, /* fallback_position */
};


#అయితే_ముగింపు

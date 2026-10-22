/*
 * Copyright © 2011,2012,2013  Google, Inc.
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

#అన్తర్భావ "hb-ot-shaper-myanmar-machine.hh"
#అన్తర్భావ "hb-ot-shaper-indic.hh"
#అన్తర్భావ "hb-ot-layout.hh"


/*
 * Myanmar shaper.
 */


స్థిర స్థిరరాశి hb_ట్యాగ్_t
myanmar_basic_features[] =
{
  /*
   * Basic features.
   * These features are applied in order, one at a time, after reordering,
   * constrained to the syllable.
   */
  HB_ట్యాగ్('r','p','h','f'),
  HB_ట్యాగ్('p','r','e','f'),
  HB_ట్యాగ్('b','l','w','f'),
  HB_ట్యాగ్('p','s','t','f'),
};
స్థిర స్థిరరాశి hb_ట్యాగ్_t
myanmar_other_features[] =
{
  /*
   * Other features.
   * These features are applied all at once, after clearing syllables.
   */
  HB_ట్యాగ్('p','r','e','s'),
  HB_ట్యాగ్('a','b','v','s'),
  HB_ట్యాగ్('b','l','w','s'),
  HB_ట్యాగ్('p','s','t','s'),
};

స్థిర అంతఃవరుస శూన్య
సమితి_myanmar_properties (hb_glyph_info_t &info)
{
  hb_codepoint_t u = info.codepoint;
  గుర్తులేని పూర్ణాంకం type = hb_indic_get_categories (u);

  info.myanmar_category() = (myanmar_category_t) (type & 0xFFu);
}


స్థిర అంతఃవరుస తార్కిక
is_one_యొక్క_myanmar (స్థిరరాశి hb_glyph_info_t &info, గుర్తులేని పూర్ణాంకం flags)
{
  /* If it ligated, all bets are off. */
  ఒకవేళ (_hb_glyph_info_ligated (&info)) తిరిగి అబద్ధం;
  తిరిగి !!(FLAG_UNSAFE (info.myanmar_category()) & flags);
}

/* Note:
 *
 * We treat Vowels and placeholders as if they were consonants.  This is safe because Vowels
 * cannot happen in a consonant syllable.  The plus side however is, we can call the
 * consonant syllable logic from the vowel syllable function and get it all right!
 *
 * Keep in sync with consonant_categories in the generator. */
#నిర్వచించు CONSONANT_FLAGS_MYANMAR (FLAG (M_Cat(C)) | FLAG (M_Cat(CS)) | FLAG (M_Cat(Ra)) | /* FLAG (M_Cat(CM)) | */ FLAG (M_Cat(IV)) | FLAG (M_Cat(GB)) | FLAG (M_Cat(DOTTEDCIRCLE)))

స్థిర అంతఃవరుస తార్కిక
is_consonant_myanmar (స్థిరరాశి hb_glyph_info_t &info)
{
  తిరిగి is_one_యొక్క_myanmar (info, CONSONANT_FLAGS_MYANMAR);
}


స్థిర తార్కిక
అమరిక_syllables_myanmar (స్థిరరాశి hb_ot_shape_plan_t *plan,
			 hb_font_t *font,
			 hb_బఫరు_t *బఫరు);
స్థిర తార్కిక
reorder_myanmar (స్థిరరాశి hb_ot_shape_plan_t *plan,
		 hb_font_t *font,
		 hb_బఫరు_t *బఫరు);

స్థిర శూన్య
collect_features_myanmar (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  /* Do this before any lookups have been applied. */
  map->కూడు_gsub_pause (అమరిక_syllables_myanmar);

  map->enable_feature (HB_ట్యాగ్('l','o','c','l'), F_చొప్పున_SYLLABLE);
  /* The Indic specs do not require ccmp, but we apply it here since if
   * there is a use of it, it's typically at the beginning. */
  map->enable_feature (HB_ట్యాగ్('c','c','m','p'), F_చొప్పున_SYLLABLE);


  map->కూడు_gsub_pause (reorder_myanmar);

  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < ARRAY_నిడివి (myanmar_basic_features); i++)
  {
    map->enable_feature (myanmar_basic_features[i], F_MANUAL_ZWJ | F_చొప్పున_SYLLABLE);
    map->కూడు_gsub_pause (శూన్యసూచిక);
  }
  map->కూడు_gsub_pause (hb_syllabic_clear_var); // Don't need syllables anymore, use stop to free buffer var

  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < ARRAY_నిడివి (myanmar_other_features); i++)
    map->enable_feature (myanmar_other_features[i], F_MANUAL_ZWJ);
}

స్థిర శూన్య
అమరిక_masks_myanmar (స్థిరరాశి hb_ot_shape_plan_t *plan HB_వాడని,
		     hb_బఫరు_t              *బఫరు,
		     hb_font_t                *font HB_వాడని)
{
  HB_బఫరు_ALLOCATE_VAR (బఫరు, myanmar_category);
  HB_బఫరు_ALLOCATE_VAR (బఫరు, myanmar_position);

  /* No masks, we just save information about characters. */

  గుర్తులేని పూర్ణాంకం లెక్క = బఫరు->len;
  hb_glyph_info_t *info = బఫరు->info;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    సమితి_myanmar_properties (info[i]);
}

స్థిర తార్కిక
అమరిక_syllables_myanmar (స్థిరరాశి hb_ot_shape_plan_t *plan HB_వాడని,
			 hb_font_t *font HB_వాడని,
			 hb_బఫరు_t *బఫరు)
{
  HB_బఫరు_ALLOCATE_VAR (బఫరు, syllable);
  కనుగొను_syllables_myanmar (బఫరు);
  foreach_syllable (బఫరు, ప్రారంభం, చివర)
    బఫరు->unsafe_గమ్యం_break (ప్రారంభం, చివర);
  తిరిగి అబద్ధం;
}

స్థిర పూర్ణాంకం
compare_myanmar_క్రమం (స్థిరరాశి hb_glyph_info_t *pa, స్థిరరాశి hb_glyph_info_t *pb)
{
  పూర్ణాంకం a = pa->myanmar_position();
  పూర్ణాంకం b = pb->myanmar_position();

  తిరిగి (పూర్ణాంకం) a - (పూర్ణాంకం) b;
}


/* Rules from:
 * https://docs.microsoft.com/en-us/typography/script-development/myanmar */

స్థిర శూన్య
initial_reordering_consonant_syllable (hb_బఫరు_t *బఫరు,
				       గుర్తులేని పూర్ణాంకం ప్రారంభం, గుర్తులేని పూర్ణాంకం చివర)
{
  hb_glyph_info_t *info = బఫరు->info;

  గుర్తులేని పూర్ణాంకం పునాది = చివర;
  తార్కిక has_reph = అబద్ధం;

  {
    గుర్తులేని పూర్ణాంకం హద్దు = ప్రారంభం;
    ఒకవేళ (ప్రారంభం + 3 <= చివర &&
	info[ప్రారంభం  ].myanmar_category() == M_Cat(Ra) &&
	info[ప్రారంభం+1].myanmar_category() == M_Cat(As) &&
	info[ప్రారంభం+2].myanmar_category() == M_Cat(H))
    {
      హద్దు += 3;
      పునాది = ప్రారంభం;
      has_reph = నిజం;
    }

    {
      ఒకవేళ (!has_reph)
	పునాది = హద్దు;

      కొరకు (గుర్తులేని పూర్ణాంకం i = హద్దు; i < చివర; i++)
	ఒకవేళ (is_consonant_myanmar (info[i]))
	{
	  పునాది = i;
	  ఆపు;
	}
    }
  }

  /* Reorder! */
  {
    గుర్తులేని పూర్ణాంకం i = ప్రారంభం;
    కొరకు (; i < ప్రారంభం + (has_reph ? 3 : 0); i++)
      info[i].myanmar_position() = POS_AFTER_MAIN;
    కొరకు (; i < పునాది; i++)
      info[i].myanmar_position() = POS_PRE_C;
    ఒకవేళ (i < చివర)
    {
      info[i].myanmar_position() = POS_పునాది_C;
      i++;
    }
    myanmar_position_t pos = POS_AFTER_MAIN;
    /* The following loop may be ugly, but it implements all of
     * Myanmar reordering! */
    కొరకు (; i < చివర; i++)
    {
      ఒకవేళ (info[i].myanmar_category() == M_Cat(MR)) /* Pre-base reordering */
      {
	info[i].myanmar_position() = POS_PRE_C;
	కొనసాగు;
      }
      ఒకవేళ (info[i].myanmar_category() == M_Cat(VPre)) /* Left matra */
      {
	info[i].myanmar_position() = POS_PRE_M;
	కొనసాగు;
      }
      ఒకవేళ (info[i].myanmar_category() == M_Cat(VS))
      {
	info[i].myanmar_position() = info[i - 1].myanmar_position();
	కొనసాగు;
      }

      ఒకవేళ (pos == POS_AFTER_MAIN && info[i].myanmar_category() == M_Cat(VBlw))
      {
	pos = POS_క్రింద_C;
	info[i].myanmar_position() = pos;
	కొనసాగు;
      }

      ఒకవేళ (pos == POS_క్రింద_C && info[i].myanmar_category() == M_Cat(A))
      {
	info[i].myanmar_position() = POS_BEFORE_SUB;
	కొనసాగు;
      }
      ఒకవేళ (pos == POS_క్రింద_C && info[i].myanmar_category() == M_Cat(VBlw))
      {
	info[i].myanmar_position() = pos;
	కొనసాగు;
      }
      ఒకవేళ (pos == POS_క్రింద_C && info[i].myanmar_category() != M_Cat(A))
      {
	pos = POS_AFTER_SUB;
	info[i].myanmar_position() = pos;
	కొనసాగు;
      }
      info[i].myanmar_position() = pos;
    }
  }

  /* Sit tight, rock 'n roll! */
  బఫరు->sort (ప్రారంభం, చివర, compare_myanmar_క్రమం);

  /* Flip left-matra sequence. */
  గుర్తులేని మొదటి_ఎడమ_matra = చివర;
  గుర్తులేని చివరి_ఎడమ_matra = చివర;
  కొరకు (గుర్తులేని పూర్ణాంకం i = ప్రారంభం; i < చివర; i++)
  {
    ఒకవేళ (info[i].myanmar_position() == POS_PRE_M)
    {
      ఒకవేళ (మొదటి_ఎడమ_matra == చివర)
	మొదటి_ఎడమ_matra = i;
      చివరి_ఎడమ_matra = i;
    }
  }
  /* https://github.com/harfbuzz/harfbuzz/issues/3863 */
  ఒకవేళ (మొదటి_ఎడమ_matra < చివరి_ఎడమ_matra)
  {
    /* No need to merge clusters, done already? */
    బఫరు->తిరగేయి_range (మొదటి_ఎడమ_matra, చివరి_ఎడమ_matra + 1);
    /* Reverse back VS, etc. */
    గుర్తులేని i = మొదటి_ఎడమ_matra;
    కొరకు (గుర్తులేని j = i; j <= చివరి_ఎడమ_matra; j++)
      ఒకవేళ (info[j].myanmar_category() == M_Cat(VPre))
      {
	బఫరు->తిరగేయి_range (i, j + 1);
	i = j + 1;
      }
  }
}

స్థిర శూన్య
reorder_syllable_myanmar (స్థిరరాశి hb_ot_shape_plan_t *plan HB_వాడని,
			  hb_face_t *face HB_వాడని,
			  hb_బఫరు_t *బఫరు,
			  గుర్తులేని పూర్ణాంకం ప్రారంభం, గుర్తులేని పూర్ణాంకం చివర)
{
  myanmar_syllable_type_t syllable_type = (myanmar_syllable_type_t) (బఫరు->info[ప్రారంభం].syllable() & 0x0F);
  ఎంపిక (syllable_type) {

    స్థితి myanmar_broken_cluster: /* We already inserted dotted-circles, so just call the consonant_syllable. */
    స్థితి myanmar_consonant_syllable:
      initial_reordering_consonant_syllable  (బఫరు, ప్రారంభం, చివర);
      ఆపు;

    స్థితి myanmar_non_myanmar_cluster:
      ఆపు;
  }
}

స్థిర తార్కిక
reorder_myanmar (స్థిరరాశి hb_ot_shape_plan_t *plan,
		 hb_font_t *font,
		 hb_బఫరు_t *బఫరు)
{
  తార్కిక ret = అబద్ధం;
  ఒకవేళ (బఫరు->సందేశం (font, "start reordering myanmar"))
  {
    ఒకవేళ (hb_syllabic_చొప్పించు_dotted_circles (font, బఫరు,
					   myanmar_broken_cluster,
					   M_Cat(DOTTEDCIRCLE)))
      ret = నిజం;

    foreach_syllable (బఫరు, ప్రారంభం, చివర)
      reorder_syllable_myanmar (plan, font->face, బఫరు, ప్రారంభం, చివర);
    (శూన్య) బఫరు->సందేశం (font, "end reordering myanmar");
  }

  HB_బఫరు_DEALLOCATE_VAR (బఫరు, myanmar_category);
  HB_బఫరు_DEALLOCATE_VAR (బఫరు, myanmar_position);

  తిరిగి ret;
}


స్థిరరాశి hb_ot_shaper_t _hb_ot_shaper_myanmar =
{
  collect_features_myanmar,
  శూన్యసూచిక, /* override_features */
  శూన్యసూచిక, /* data_create */
  శూన్యసూచిక, /* data_destroy */
  శూన్యసూచిక, /* preprocess_text */
  శూన్యసూచిక, /* postprocess_glyphs */
  శూన్యసూచిక, /* decompose */
  శూన్యసూచిక, /* compose */
  అమరిక_masks_myanmar,
  శూన్యసూచిక, /* reorder_marks */
  HB_ట్యాగ్_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_బహుళకం_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT,
  HB_OT_SHAPE_ZERO_వెడల్పు_MARKS_BY_GDEF_EARLY,
  అబద్ధం, /* fallback_position */
};


#నిర్వచితం_కాకపోతే HB_NO_OT_SHAPER_MYANMAR_ZAWGYI
/* Ugly Zawgyi encoding.
 * Disable all auto processing.
 * https://github.com/harfbuzz/harfbuzz/issues/1162 */
స్థిరరాశి hb_ot_shaper_t _hb_ot_shaper_myanmar_zawgyi =
{
  శూన్యసూచిక, /* collect_features */
  శూన్యసూచిక, /* override_features */
  శూన్యసూచిక, /* data_create */
  శూన్యసూచిక, /* data_destroy */
  శూన్యసూచిక, /* preprocess_text */
  శూన్యసూచిక, /* postprocess_glyphs */
  శూన్యసూచిక, /* decompose */
  శూన్యసూచిక, /* compose */
  శూన్యసూచిక, /* setup_masks */
  శూన్యసూచిక, /* reorder_marks */
  HB_ట్యాగ్_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_బహుళకం_NONE,
  HB_OT_SHAPE_ZERO_వెడల్పు_MARKS_NONE,
  అబద్ధం, /* fallback_position */
};
#అయితే_ముగింపు


#అయితే_ముగింపు

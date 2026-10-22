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

#समावेश "hb.hh"

#अगर_अघोषित HB_NO_OT_SHAPE

#समावेश "hb-ot-shaper-myanmar-machine.hh"
#समावेश "hb-ot-shaper-indic.hh"
#समावेश "hb-ot-layout.hh"


/*
 * Myanmar shaper.
 */


अटल स्थिर hb_tag_t
myanmar_basic_features[] =
{
  /*
   * Basic features.
   * These features are applied in order, one at a time, after reordering,
   * constrained to the syllable.
   */
  HB_TAG('r','p','h','f'),
  HB_TAG('p','r','e','f'),
  HB_TAG('b','l','w','f'),
  HB_TAG('p','s','t','f'),
};
अटल स्थिर hb_tag_t
myanmar_other_features[] =
{
  /*
   * Other features.
   * These features are applied all at once, after clearing syllables.
   */
  HB_TAG('p','r','e','s'),
  HB_TAG('a','b','v','s'),
  HB_TAG('b','l','w','s'),
  HB_TAG('p','s','t','s'),
};

अटल अंतरभूत व्योम
set_myanmar_properties (hb_glyph_info_t &info)
{
  hb_codepoint_t u = info.codepoint;
  अचिन्हित पूर्णांक type = hb_indic_get_categories (u);

  info.myanmar_category() = (myanmar_category_t) (type & 0xFFu);
}


अटल अंतरभूत bool
is_one_of_myanmar (स्थिर hb_glyph_info_t &info, अचिन्हित पूर्णांक flags)
{
  /* If it ligated, all bets are off. */
  अगर (_hb_glyph_info_ligated (&info)) वापस false;
  वापस !!(FLAG_UNSAFE (info.myanmar_category()) & flags);
}

/* Note:
 *
 * We treat Vowels and placeholders as if they were consonants.  This is safe because Vowels
 * cannot happen in a consonant syllable.  The plus side however is, we can call the
 * consonant syllable logic from the vowel syllable function and get it all right!
 *
 * Keep in sync with consonant_categories in the generator. */
#घोषणा CONSONANT_FLAGS_MYANMAR (FLAG (M_Cat(C)) | FLAG (M_Cat(CS)) | FLAG (M_Cat(Ra)) | /* FLAG (M_Cat(CM)) | */ FLAG (M_Cat(IV)) | FLAG (M_Cat(GB)) | FLAG (M_Cat(DOTTEDCIRCLE)))

अटल अंतरभूत bool
is_consonant_myanmar (स्थिर hb_glyph_info_t &info)
{
  वापस is_one_of_myanmar (info, CONSONANT_FLAGS_MYANMAR);
}


अटल bool
setup_syllables_myanmar (स्थिर hb_ot_shape_plan_t *plan,
			 hb_font_t *font,
			 hb_बफर_t *बफर);
अटल bool
reorder_myanmar (स्थिर hb_ot_shape_plan_t *plan,
		 hb_font_t *font,
		 hb_बफर_t *बफर);

अटल व्योम
collect_features_myanmar (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  /* Do this before any lookups have been applied. */
  map->add_gsub_pause (setup_syllables_myanmar);

  map->enable_feature (HB_TAG('l','o','c','l'), F_PER_SYLLABLE);
  /* The Indic specs do not require ccmp, but we apply it here since if
   * there is a use of it, it's typically at the beginning. */
  map->enable_feature (HB_TAG('c','c','m','p'), F_PER_SYLLABLE);


  map->add_gsub_pause (reorder_myanmar);

  क्रम (अचिन्हित पूर्णांक i = 0; i < ARRAY_लंबाई (myanmar_basic_features); i++)
  {
    map->enable_feature (myanmar_basic_features[i], F_MANUAL_ZWJ | F_PER_SYLLABLE);
    map->add_gsub_pause (nullptr);
  }
  map->add_gsub_pause (hb_syllabic_clear_var); // Don't need syllables anymore, use stop to free buffer var

  क्रम (अचिन्हित पूर्णांक i = 0; i < ARRAY_लंबाई (myanmar_other_features); i++)
    map->enable_feature (myanmar_other_features[i], F_MANUAL_ZWJ);
}

अटल व्योम
setup_masks_myanmar (स्थिर hb_ot_shape_plan_t *plan HB_UNUSED,
		     hb_बफर_t              *बफर,
		     hb_font_t                *font HB_UNUSED)
{
  HB_बफर_ALLOCATE_VAR (बफर, myanmar_category);
  HB_बफर_ALLOCATE_VAR (बफर, myanmar_position);

  /* No masks, we just save information about characters. */

  अचिन्हित पूर्णांक गिनती = बफर->len;
  hb_glyph_info_t *info = बफर->info;
  क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    set_myanmar_properties (info[i]);
}

अटल bool
setup_syllables_myanmar (स्थिर hb_ot_shape_plan_t *plan HB_UNUSED,
			 hb_font_t *font HB_UNUSED,
			 hb_बफर_t *बफर)
{
  HB_बफर_ALLOCATE_VAR (बफर, syllable);
  find_syllables_myanmar (बफर);
  foreach_syllable (बफर, start, end)
    बफर->unsafe_to_break (start, end);
  वापस false;
}

अटल पूर्णांक
compare_myanmar_order (स्थिर hb_glyph_info_t *pa, स्थिर hb_glyph_info_t *pb)
{
  पूर्णांक a = pa->myanmar_position();
  पूर्णांक b = pb->myanmar_position();

  वापस (पूर्णांक) a - (पूर्णांक) b;
}


/* Rules from:
 * https://docs.microsoft.com/en-us/typography/script-development/myanmar */

अटल व्योम
initial_reordering_consonant_syllable (hb_बफर_t *बफर,
				       अचिन्हित पूर्णांक start, अचिन्हित पूर्णांक end)
{
  hb_glyph_info_t *info = बफर->info;

  अचिन्हित पूर्णांक base = end;
  bool has_reph = false;

  {
    अचिन्हित पूर्णांक limit = start;
    अगर (start + 3 <= end &&
	info[start  ].myanmar_category() == M_Cat(Ra) &&
	info[start+1].myanmar_category() == M_Cat(As) &&
	info[start+2].myanmar_category() == M_Cat(H))
    {
      limit += 3;
      base = start;
      has_reph = true;
    }

    {
      अगर (!has_reph)
	base = limit;

      क्रम (अचिन्हित पूर्णांक i = limit; i < end; i++)
	अगर (is_consonant_myanmar (info[i]))
	{
	  base = i;
	  अवरोध;
	}
    }
  }

  /* Reorder! */
  {
    अचिन्हित पूर्णांक i = start;
    क्रम (; i < start + (has_reph ? 3 : 0); i++)
      info[i].myanmar_position() = POS_AFTER_MAIN;
    क्रम (; i < base; i++)
      info[i].myanmar_position() = POS_PRE_C;
    अगर (i < end)
    {
      info[i].myanmar_position() = POS_BASE_C;
      i++;
    }
    myanmar_position_t pos = POS_AFTER_MAIN;
    /* The following loop may be ugly, but it implements all of
     * Myanmar reordering! */
    क्रम (; i < end; i++)
    {
      अगर (info[i].myanmar_category() == M_Cat(MR)) /* Pre-base reordering */
      {
	info[i].myanmar_position() = POS_PRE_C;
	जारी;
      }
      अगर (info[i].myanmar_category() == M_Cat(VPre)) /* Left matra */
      {
	info[i].myanmar_position() = POS_PRE_M;
	जारी;
      }
      अगर (info[i].myanmar_category() == M_Cat(VS))
      {
	info[i].myanmar_position() = info[i - 1].myanmar_position();
	जारी;
      }

      अगर (pos == POS_AFTER_MAIN && info[i].myanmar_category() == M_Cat(VBlw))
      {
	pos = POS_BELOW_C;
	info[i].myanmar_position() = pos;
	जारी;
      }

      अगर (pos == POS_BELOW_C && info[i].myanmar_category() == M_Cat(A))
      {
	info[i].myanmar_position() = POS_BEFORE_SUB;
	जारी;
      }
      अगर (pos == POS_BELOW_C && info[i].myanmar_category() == M_Cat(VBlw))
      {
	info[i].myanmar_position() = pos;
	जारी;
      }
      अगर (pos == POS_BELOW_C && info[i].myanmar_category() != M_Cat(A))
      {
	pos = POS_AFTER_SUB;
	info[i].myanmar_position() = pos;
	जारी;
      }
      info[i].myanmar_position() = pos;
    }
  }

  /* Sit tight, rock 'n roll! */
  बफर->sort (start, end, compare_myanmar_order);

  /* Flip left-matra sequence. */
  अचिन्हित first_बायाँ_matra = end;
  अचिन्हित last_बायाँ_matra = end;
  क्रम (अचिन्हित पूर्णांक i = start; i < end; i++)
  {
    अगर (info[i].myanmar_position() == POS_PRE_M)
    {
      अगर (first_बायाँ_matra == end)
	first_बायाँ_matra = i;
      last_बायाँ_matra = i;
    }
  }
  /* https://github.com/harfbuzz/harfbuzz/issues/3863 */
  अगर (first_बायाँ_matra < last_बायाँ_matra)
  {
    /* No need to merge clusters, done already? */
    बफर->reverse_range (first_बायाँ_matra, last_बायाँ_matra + 1);
    /* Reverse back VS, etc. */
    अचिन्हित i = first_बायाँ_matra;
    क्रम (अचिन्हित j = i; j <= last_बायाँ_matra; j++)
      अगर (info[j].myanmar_category() == M_Cat(VPre))
      {
	बफर->reverse_range (i, j + 1);
	i = j + 1;
      }
  }
}

अटल व्योम
reorder_syllable_myanmar (स्थिर hb_ot_shape_plan_t *plan HB_UNUSED,
			  hb_face_t *face HB_UNUSED,
			  hb_बफर_t *बफर,
			  अचिन्हित पूर्णांक start, अचिन्हित पूर्णांक end)
{
  myanmar_syllable_type_t syllable_type = (myanmar_syllable_type_t) (बफर->info[start].syllable() & 0x0F);
  चयन (syllable_type) {

    हाल myanmar_broken_cluster: /* We already inserted dotted-circles, so just call the consonant_syllable. */
    हाल myanmar_consonant_syllable:
      initial_reordering_consonant_syllable  (बफर, start, end);
      अवरोध;

    हाल myanmar_non_myanmar_cluster:
      अवरोध;
  }
}

अटल bool
reorder_myanmar (स्थिर hb_ot_shape_plan_t *plan,
		 hb_font_t *font,
		 hb_बफर_t *बफर)
{
  bool ret = false;
  अगर (बफर->message (font, "start reordering myanmar"))
  {
    अगर (hb_syllabic_जोड़ो_dotted_circles (font, बफर,
					   myanmar_broken_cluster,
					   M_Cat(DOTTEDCIRCLE)))
      ret = true;

    foreach_syllable (बफर, start, end)
      reorder_syllable_myanmar (plan, font->face, बफर, start, end);
    (व्योम) बफर->message (font, "end reordering myanmar");
  }

  HB_बफर_DEALLOCATE_VAR (बफर, myanmar_category);
  HB_बफर_DEALLOCATE_VAR (बफर, myanmar_position);

  वापस ret;
}


स्थिर hb_ot_shaper_t _hb_ot_shaper_myanmar =
{
  collect_features_myanmar,
  nullptr, /* override_features */
  nullptr, /* data_create */
  nullptr, /* data_destroy */
  nullptr, /* preprocess_text */
  nullptr, /* postprocess_glyphs */
  nullptr, /* decompose */
  nullptr, /* compose */
  setup_masks_myanmar,
  nullptr, /* reorder_marks */
  HB_TAG_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_MODE_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT,
  HB_OT_SHAPE_ZERO_WIDTH_MARKS_BY_GDEF_EARLY,
  false, /* fallback_position */
};


#अगर_अघोषित HB_NO_OT_SHAPER_MYANMAR_ZAWGYI
/* Ugly Zawgyi encoding.
 * Disable all auto processing.
 * https://github.com/harfbuzz/harfbuzz/issues/1162 */
स्थिर hb_ot_shaper_t _hb_ot_shaper_myanmar_zawgyi =
{
  nullptr, /* collect_features */
  nullptr, /* override_features */
  nullptr, /* data_create */
  nullptr, /* data_destroy */
  nullptr, /* preprocess_text */
  nullptr, /* postprocess_glyphs */
  nullptr, /* decompose */
  nullptr, /* compose */
  nullptr, /* setup_masks */
  nullptr, /* reorder_marks */
  HB_TAG_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_MODE_NONE,
  HB_OT_SHAPE_ZERO_WIDTH_MARKS_NONE,
  false, /* fallback_position */
};
#पूर्ण_अगर


#पूर्ण_अगर

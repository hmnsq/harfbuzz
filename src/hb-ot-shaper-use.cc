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

#समावेश "hb.hh"

#अगर_अघोषित HB_NO_OT_SHAPE

#समावेश "hb-ot-shaper-use-machine.hh"
#समावेश "hb-ot-shaper-use-table.hh"
#समावेश "hb-ot-shaper-arabic.hh"
#समावेश "hb-ot-shaper-arabic-joining-list.hh"
#समावेश "hb-ot-shaper-vowel-constraints.hh"


/*
 * Universal Shaping Engine.
 * https://docs.microsoft.com/en-us/typography/script-development/use
 */

अटल स्थिर hb_tag_t
use_basic_features[] =
{
  /*
   * Basic features.
   * These features are applied all at once, before reordering, constrained
   * to the syllable.
   */
  HB_TAG('r','k','r','f'),
  HB_TAG('a','b','v','f'),
  HB_TAG('b','l','w','f'),
  HB_TAG('h','a','l','f'),
  HB_TAG('p','s','t','f'),
  HB_TAG('v','a','t','u'),
  HB_TAG('c','j','c','t'),
};
अटल स्थिर hb_tag_t
use_topographical_features[] =
{
  HB_TAG('i','s','o','l'),
  HB_TAG('i','n','i','t'),
  HB_TAG('m','e','d','i'),
  HB_TAG('f','i','n','a'),
};
/* Same order as use_topographical_features. */
क्रमागत joining_form_t {
  JOINING_FORM_ISOL,
  JOINING_FORM_INIT,
  JOINING_FORM_MEDI,
  JOINING_FORM_FINA,
  _JOINING_FORM_NONE
};
अटल स्थिर hb_tag_t
use_other_features[] =
{
  /*
   * Other features.
   * These features are applied all at once, after reordering and
   * clearing syllables.
   */
  HB_TAG('a','b','v','s'),
  HB_TAG('b','l','w','s'),
  HB_TAG('h','a','l','n'),
  HB_TAG('p','r','e','s'),
  HB_TAG('p','s','t','s'),
};

अटल bool
setup_syllables_use (स्थिर hb_ot_shape_plan_t *plan,
		     hb_font_t *font,
		     hb_बफर_t *बफर);
अटल bool
record_rphf_use (स्थिर hb_ot_shape_plan_t *plan,
		 hb_font_t *font,
		 hb_बफर_t *बफर);
अटल bool
record_pref_use (स्थिर hb_ot_shape_plan_t *plan,
		 hb_font_t *font,
		 hb_बफर_t *बफर);
अटल bool
reorder_use (स्थिर hb_ot_shape_plan_t *plan,
	     hb_font_t *font,
	     hb_बफर_t *बफर);

अटल व्योम
collect_features_use (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  /* Do this before any lookups have been applied. */
  map->add_gsub_pause (setup_syllables_use);

  /* "Default glyph pre-processing group" */
  map->enable_feature (HB_TAG('l','o','c','l'), F_PER_SYLLABLE);
  map->enable_feature (HB_TAG('c','c','m','p'), F_PER_SYLLABLE);
  map->enable_feature (HB_TAG('n','u','k','t'), F_PER_SYLLABLE);
  map->enable_feature (HB_TAG('a','k','h','n'), F_MANUAL_ZWJ | F_PER_SYLLABLE);

  /* "Reordering group" */
  map->add_gsub_pause (_hb_clear_substitution_flags);
  map->add_feature (HB_TAG('r','p','h','f'), F_MANUAL_ZWJ | F_PER_SYLLABLE);
  map->add_gsub_pause (record_rphf_use);
  map->add_gsub_pause (_hb_clear_substitution_flags);
  map->enable_feature (HB_TAG('p','r','e','f'), F_MANUAL_ZWJ | F_PER_SYLLABLE);
  map->add_gsub_pause (record_pref_use);

  /* "Orthographic unit shaping group" */
  क्रम (अचिन्हित पूर्णांक i = 0; i < ARRAY_लंबाई (use_basic_features); i++)
    map->enable_feature (use_basic_features[i], F_MANUAL_ZWJ | F_PER_SYLLABLE);

  map->add_gsub_pause (reorder_use);
  map->add_gsub_pause (hb_syllabic_clear_var); // Don't need syllables anymore, use stop to free buffer var

  /* "Topographical features" */
  क्रम (अचिन्हित पूर्णांक i = 0; i < ARRAY_लंबाई (use_topographical_features); i++)
    map->add_feature (use_topographical_features[i]);
  map->add_gsub_pause (nullptr);

  /* "Standard typographic presentation" */
  क्रम (अचिन्हित पूर्णांक i = 0; i < ARRAY_लंबाई (use_other_features); i++)
    map->enable_feature (use_other_features[i], F_MANUAL_ZWJ);
}

काष्ठा use_shape_plan_t
{
  hb_mask_t rphf_mask;

  arabic_shape_plan_t *arabic_plan;
};

अटल व्योम *
तथ्य_create_use (स्थिर hb_ot_shape_plan_t *plan)
{
  use_shape_plan_t *use_plan = (use_shape_plan_t *) hb_calloc (1, माप (use_shape_plan_t));
  अगर (unlikely (!use_plan))
    वापस nullptr;

  use_plan->rphf_mask = plan->map.get_1_mask (HB_TAG('r','p','h','f'));

  अगर (has_arabic_joining (plan->props.script))
  {
    use_plan->arabic_plan = (arabic_shape_plan_t *) तथ्य_create_arabic (plan);
    अगर (unlikely (!use_plan->arabic_plan))
    {
      hb_free (use_plan);
      वापस nullptr;
    }
  }

  वापस use_plan;
}

अटल व्योम
तथ्य_destroy_use (व्योम *तथ्य)
{
  use_shape_plan_t *use_plan = (use_shape_plan_t *) तथ्य;

  अगर (use_plan->arabic_plan)
    तथ्य_destroy_arabic (use_plan->arabic_plan);

  hb_free (तथ्य);
}

अटल व्योम
setup_masks_use (स्थिर hb_ot_shape_plan_t *plan,
		 hb_बफर_t              *बफर,
		 hb_font_t                *font HB_UNUSED)
{
  स्थिर use_shape_plan_t *use_plan = (स्थिर use_shape_plan_t *) plan->तथ्य;

  /* Do this before allocating use_category(). */
  अगर (use_plan->arabic_plan)
  {
    setup_masks_arabic_plan (use_plan->arabic_plan, बफर, plan->props.script);
  }

  HB_बफर_ALLOCATE_VAR (बफर, use_category);

  /* We cannot setup masks here.  We save information about characters
   * and setup masks later on in a pause-callback. */

  अचिन्हित पूर्णांक गिनती = बफर->len;
  hb_glyph_info_t *info = बफर->info;
  क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    info[i].use_category() = hb_use_get_category (info[i].codepoint);
}

अटल व्योम
setup_rphf_mask (स्थिर hb_ot_shape_plan_t *plan,
		 hb_बफर_t *बफर)
{
  स्थिर use_shape_plan_t *use_plan = (स्थिर use_shape_plan_t *) plan->तथ्य;

  hb_mask_t mask = use_plan->rphf_mask;
  अगर (!mask) वापस;

  hb_glyph_info_t *info = बफर->info;

  foreach_syllable (बफर, start, end)
  {
    अचिन्हित पूर्णांक limit = info[start].use_category() == USE(R) ? 1 : hb_min (3u, end - start);
    क्रम (अचिन्हित पूर्णांक i = start; i < start + limit; i++)
      info[i].mask |= mask;
  }
}

अटल व्योम
setup_topographical_masks (स्थिर hb_ot_shape_plan_t *plan,
			   hb_बफर_t *बफर)
{
  स्थिर use_shape_plan_t *use_plan = (स्थिर use_shape_plan_t *) plan->तथ्य;
  अगर (use_plan->arabic_plan)
    वापस;

  static_assert ((JOINING_FORM_INIT < 4 && JOINING_FORM_ISOL < 4 && JOINING_FORM_MEDI < 4 && JOINING_FORM_FINA < 4), "");
  hb_mask_t masks[4], all_masks = 0;
  क्रम (अचिन्हित पूर्णांक i = 0; i < 4; i++)
  {
    masks[i] = plan->map.get_1_mask (use_topographical_features[i]);
    अगर (masks[i] == plan->map.get_global_mask ())
      masks[i] = 0;
    all_masks |= masks[i];
  }
  अगर (!all_masks)
    वापस;
  hb_mask_t other_masks = ~all_masks;

  अचिन्हित पूर्णांक last_start = 0;
  joining_form_t last_form = _JOINING_FORM_NONE;
  hb_glyph_info_t *info = बफर->info;
  foreach_syllable (बफर, start, end)
  {
    use_syllable_type_t syllable_type = (use_syllable_type_t) (info[start].syllable() & 0x0F);
    चयन (syllable_type)
    {
      हाल use_hieroglyph_cluster:
      हाल use_non_cluster:
	/* These don't join.  Nothing to do. */
	last_form = _JOINING_FORM_NONE;
	अवरोध;

      हाल use_virama_terminated_cluster:
      हाल use_sakot_terminated_cluster:
      हाल use_standard_cluster:
      हाल use_संख्या_joiner_terminated_cluster:
      हाल use_numeral_cluster:
      हाल use_प्रतीक_cluster:
      हाल use_broken_cluster:

	bool join = last_form == JOINING_FORM_FINA || last_form == JOINING_FORM_ISOL;

	अगर (join)
	{
	  /* Fixup previous syllable's form. */
	  last_form = last_form == JOINING_FORM_FINA ? JOINING_FORM_MEDI : JOINING_FORM_INIT;
	  क्रम (अचिन्हित पूर्णांक i = last_start; i < start; i++)
	    info[i].mask = (info[i].mask & other_masks) | masks[last_form];
	}

	/* Form for this syllable. */
	last_form = join ? JOINING_FORM_FINA : JOINING_FORM_ISOL;
	क्रम (अचिन्हित पूर्णांक i = start; i < end; i++)
	  info[i].mask = (info[i].mask & other_masks) | masks[last_form];

	अवरोध;
    }

    last_start = start;
  }
}

अटल bool
setup_syllables_use (स्थिर hb_ot_shape_plan_t *plan,
		     hb_font_t *font HB_UNUSED,
		     hb_बफर_t *बफर)
{
  HB_बफर_ALLOCATE_VAR (बफर, syllable);
  find_syllables_use (बफर);
  foreach_syllable (बफर, start, end)
    बफर->unsafe_to_break (start, end);
  setup_rphf_mask (plan, बफर);
  setup_topographical_masks (plan, बफर);
  वापस false;
}

अटल bool
record_rphf_use (स्थिर hb_ot_shape_plan_t *plan,
		 hb_font_t *font HB_UNUSED,
		 hb_बफर_t *बफर)
{
  स्थिर use_shape_plan_t *use_plan = (स्थिर use_shape_plan_t *) plan->तथ्य;

  hb_mask_t mask = use_plan->rphf_mask;
  अगर (!mask) वापस false;
  hb_glyph_info_t *info = बफर->info;

  foreach_syllable (बफर, start, end)
  {
    /* Mark a substituted repha as USE(R). */
    क्रम (अचिन्हित पूर्णांक i = start; i < end && (info[i].mask & mask); i++)
      अगर (_hb_glyph_info_substituted (&info[i]))
      {
	info[i].use_category() = USE(R);
	अवरोध;
      }
  }
  वापस false;
}

अटल bool
record_pref_use (स्थिर hb_ot_shape_plan_t *plan HB_UNUSED,
		 hb_font_t *font HB_UNUSED,
		 hb_बफर_t *बफर)
{
  hb_glyph_info_t *info = बफर->info;

  foreach_syllable (बफर, start, end)
  {
    /* Mark a substituted pref as VPre, as they behave the same way. */
    क्रम (अचिन्हित पूर्णांक i = start; i < end; i++)
      अगर (_hb_glyph_info_substituted (&info[i]))
      {
	info[i].use_category() = USE(VPre);
	अवरोध;
      }
  }
  वापस false;
}

अटल अंतरभूत bool
is_halant_use (स्थिर hb_glyph_info_t &info)
{
  वापस (info.use_category() == USE(H) || info.use_category() == USE(HVM) || info.use_category() == USE(IS)) &&
	 !_hb_glyph_info_ligated (&info);
}

अटल व्योम
reorder_syllable_use (hb_बफर_t *बफर, अचिन्हित पूर्णांक start, अचिन्हित पूर्णांक end)
{
  use_syllable_type_t syllable_type = (use_syllable_type_t) (बफर->info[start].syllable() & 0x0F);
  /* Only a few syllable types need reordering. */
  अगर (unlikely (!(FLAG_UNSAFE (syllable_type) &
		  (FLAG (use_virama_terminated_cluster) |
		   FLAG (use_sakot_terminated_cluster) |
		   FLAG (use_standard_cluster) |
		   FLAG (use_प्रतीक_cluster) |
		   FLAG (use_broken_cluster) |
		   0))))
    वापस;

  hb_glyph_info_t *info = बफर->info;

#घोषणा POST_BASE_FLAGS64 (FLAG64 (USE(FAbv)) | \
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
  अगर (info[start].use_category() == USE(R) && end - start > 1)
  {
    /* Got a repha.  Reorder it towards the end, but before the first post-base
     * glyph. */
    क्रम (अचिन्हित पूर्णांक i = start + 1; i < end; i++)
    {
      bool is_post_base_glyph = (FLAG64_UNSAFE (info[i].use_category()) & POST_BASE_FLAGS64) ||
				is_halant_use (info[i]);
      अगर (is_post_base_glyph || i == end - 1)
      {
	/* If we hit a post-base glyph, move before it; otherwise move to the
	 * end. Shift things in between backward. */

	अगर (is_post_base_glyph)
	  i--;

	बफर->merge_clusters (start, i + 1);
	hb_glyph_info_t t = info[start];
	स_हटाओ (&info[start], &info[start + 1], (i - start) * माप (info[0]));
	info[i] = t;

	अवरोध;
      }
    }
  }

  /* Move things back. */
  अचिन्हित पूर्णांक j = start;
  क्रम (अचिन्हित पूर्णांक i = start; i < end; i++)
  {
    uint32_t flag = FLAG_UNSAFE (info[i].use_category());
    अगर (is_halant_use (info[i]))
    {
      /* If we hit a halant, move after it; otherwise move to the beginning, and
       * shift things in between forward. */
      j = i + 1;
    }
    अन्यथा अगर (((flag) & (FLAG (USE(VPre)) | FLAG (USE(VMPre)))) &&
	     /* Only move the first component of a MultipleSubst. */
	     0 == _hb_glyph_info_get_lig_comp (&info[i]) &&
	     j < i)
    {
      बफर->merge_clusters (j, i + 1);
      hb_glyph_info_t t = info[i];
      स_हटाओ (&info[j + 1], &info[j], (i - j) * माप (info[0]));
      info[j] = t;
    }
  }
}

अटल bool
reorder_use (स्थिर hb_ot_shape_plan_t *plan,
	     hb_font_t *font,
	     hb_बफर_t *बफर)
{
  bool ret = false;
  अगर (बफर->message (font, "start reordering USE"))
  {
    अगर (hb_syllabic_जोड़ो_dotted_circles (font, बफर,
					   use_broken_cluster,
					   USE(B),
					   USE(R)))
      ret = true;

    foreach_syllable (बफर, start, end)
      reorder_syllable_use (बफर, start, end);

    (व्योम) बफर->message (font, "end reordering USE");
  }

  HB_बफर_DEALLOCATE_VAR (बफर, use_category);

  वापस ret;
}


अटल व्योम
preprocess_text_use (स्थिर hb_ot_shape_plan_t *plan,
		     hb_बफर_t              *बफर,
		     hb_font_t                *font)
{
  _hb_preprocess_text_vowel_constraints (plan, बफर, font);
}

अटल bool
compose_use (स्थिर hb_ot_shape_normalize_context_t *c,
	     hb_codepoint_t  a,
	     hb_codepoint_t  b,
	     hb_codepoint_t *ab)
{
  /* Avoid recomposing split matras. */
  अगर (HB_UNICODE_GENERAL_CATEGORY_IS_MARK (c->unicode->general_category (a)))
    वापस false;

  वापस (bool)c->unicode->compose (a, b, ab);
}


स्थिर hb_ot_shaper_t _hb_ot_shaper_use =
{
  collect_features_use,
  nullptr, /* override_features */
  तथ्य_create_use,
  तथ्य_destroy_use,
  preprocess_text_use,
  nullptr, /* postprocess_glyphs */
  nullptr, /* decompose */
  compose_use,
  setup_masks_use,
  nullptr, /* reorder_marks */
  HB_TAG_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_MODE_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT,
  HB_OT_SHAPE_ZERO_WIDTH_MARKS_BY_GDEF_EARLY,
  false, /* fallback_position */
};


#पूर्ण_अगर

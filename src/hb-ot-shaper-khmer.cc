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

#समावेश "hb.hh"

#अगर_अघोषित HB_NO_OT_SHAPE

#समावेश "hb-ot-shaper-khmer-machine.hh"
#समावेश "hb-ot-shaper-indic.hh"
#समावेश "hb-ot-layout.hh"


/*
 * Khmer shaper.
 */


अटल स्थिर hb_ot_map_feature_t
khmer_features[] =
{
  /*
   * Basic features.
   * These features are applied all at once, before reordering, constrained
   * to the syllable.
   */
  {HB_TAG('p','r','e','f'), F_MANUAL_JOINERS | F_PER_SYLLABLE},
  {HB_TAG('b','l','w','f'), F_MANUAL_JOINERS | F_PER_SYLLABLE},
  {HB_TAG('a','b','v','f'), F_MANUAL_JOINERS | F_PER_SYLLABLE},
  {HB_TAG('p','s','t','f'), F_MANUAL_JOINERS | F_PER_SYLLABLE},
  {HB_TAG('c','f','a','r'), F_MANUAL_JOINERS | F_PER_SYLLABLE},
  /*
   * Other features.
   * These features are applied all at once after clearing syllables.
   */
  {HB_TAG('p','r','e','s'), F_GLOBAL_MANUAL_JOINERS},
  {HB_TAG('a','b','v','s'), F_GLOBAL_MANUAL_JOINERS},
  {HB_TAG('b','l','w','s'), F_GLOBAL_MANUAL_JOINERS},
  {HB_TAG('p','s','t','s'), F_GLOBAL_MANUAL_JOINERS},
};

/*
 * Must be in the same order as the khmer_features array.
 */
क्रमागत {
  KHMER_PREF,
  KHMER_BLWF,
  KHMER_ABVF,
  KHMER_PSTF,
  KHMER_CFAR,

  _KHMER_PRES,
  _KHMER_ABVS,
  _KHMER_BLWS,
  _KHMER_PSTS,

  KHMER_अंक_FEATURES,
  KHMER_BASIC_FEATURES = _KHMER_PRES, /* Don't forget to update this! */
};

अटल अंतरभूत व्योम
set_khmer_properties (hb_glyph_info_t &info)
{
  hb_codepoint_t u = info.codepoint;
  अचिन्हित पूर्णांक type = hb_indic_get_categories (u);

  info.khmer_category() = (khmer_category_t) (type & 0xFFu);
}

अटल bool
setup_syllables_khmer (स्थिर hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_बफर_t *बफर);
अटल bool
reorder_khmer (स्थिर hb_ot_shape_plan_t *plan,
	       hb_font_t *font,
	       hb_बफर_t *बफर);

अटल व्योम
collect_features_khmer (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  /* Do this before any lookups have been applied. */
  map->add_gsub_pause (setup_syllables_khmer);
  map->add_gsub_pause (reorder_khmer);

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
  map->enable_feature (HB_TAG('l','o','c','l'), F_PER_SYLLABLE);
  map->enable_feature (HB_TAG('c','c','m','p'), F_PER_SYLLABLE);

  अचिन्हित पूर्णांक i = 0;
  क्रम (; i < KHMER_BASIC_FEATURES; i++)
    map->add_feature (khmer_features[i]);

  /* https://github.com/harfbuzz/harfbuzz/issues/3531 */
  map->add_gsub_pause (hb_syllabic_clear_var); // Don't need syllables anymore, use stop to free buffer var

  क्रम (; i < KHMER_अंक_FEATURES; i++)
    map->add_feature (khmer_features[i]);
}

अटल व्योम
override_features_khmer (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  /* Khmer spec has 'clig' as part of required shaping features:
   * "Apply feature 'clig' to form ligatures that are desired for
   * typographical correctness.", hence in overrides... */
  map->enable_feature (HB_TAG('c','l','i','g'));

  map->disable_feature (HB_TAG('l','i','g','a'));
}


काष्ठा khmer_shape_plan_t
{
  hb_mask_t mask_array[KHMER_अंक_FEATURES];
};

अटल व्योम *
तथ्य_create_khmer (स्थिर hb_ot_shape_plan_t *plan)
{
  khmer_shape_plan_t *khmer_plan = (khmer_shape_plan_t *) hb_calloc (1, माप (khmer_shape_plan_t));
  अगर (unlikely (!khmer_plan))
    वापस nullptr;

  क्रम (अचिन्हित पूर्णांक i = 0; i < ARRAY_लंबाई (khmer_plan->mask_array); i++)
    khmer_plan->mask_array[i] = (khmer_features[i].flags & F_GLOBAL) ?
				 0 : plan->map.get_1_mask (khmer_features[i].tag);

  वापस khmer_plan;
}

अटल व्योम
तथ्य_destroy_khmer (व्योम *तथ्य)
{
  hb_free (तथ्य);
}

अटल व्योम
setup_masks_khmer (स्थिर hb_ot_shape_plan_t *plan HB_UNUSED,
		   hb_बफर_t              *बफर,
		   hb_font_t                *font HB_UNUSED)
{
  HB_बफर_ALLOCATE_VAR (बफर, khmer_category);

  /* We cannot setup masks here.  We save information about characters
   * and setup masks later on in a pause-callback. */

  अचिन्हित पूर्णांक गिनती = बफर->len;
  hb_glyph_info_t *info = बफर->info;
  क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    set_khmer_properties (info[i]);
}

अटल bool
setup_syllables_khmer (स्थिर hb_ot_shape_plan_t *plan HB_UNUSED,
		       hb_font_t *font HB_UNUSED,
		       hb_बफर_t *बफर)
{
  HB_बफर_ALLOCATE_VAR (बफर, syllable);
  find_syllables_khmer (बफर);
  foreach_syllable (बफर, start, end)
    बफर->unsafe_to_break (start, end);
  वापस false;
}


/* Rules from:
 * https://docs.microsoft.com/en-us/typography/script-development/devanagari */

अटल व्योम
reorder_consonant_syllable (स्थिर hb_ot_shape_plan_t *plan,
			    hb_face_t *face HB_UNUSED,
			    hb_बफर_t *बफर,
			    अचिन्हित पूर्णांक start, अचिन्हित पूर्णांक end)
{
  स्थिर khmer_shape_plan_t *khmer_plan = (स्थिर khmer_shape_plan_t *) plan->तथ्य;
  hb_glyph_info_t *info = बफर->info;

  /* Setup masks. */
  {
    /* Post-base */
    hb_mask_t mask = khmer_plan->mask_array[KHMER_BLWF] |
		     khmer_plan->mask_array[KHMER_ABVF] |
		     khmer_plan->mask_array[KHMER_PSTF];
    क्रम (अचिन्हित पूर्णांक i = start + 1; i < end; i++)
      info[i].mask  |= mask;
  }

  अचिन्हित पूर्णांक अंक_coengs = 0;
  क्रम (अचिन्हित पूर्णांक i = start + 1; i < end; i++)
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
    अगर (info[i].khmer_category() == K_Cat(H) && अंक_coengs <= 2 && i + 1 < end)
    {
      अंक_coengs++;

      अगर (info[i + 1].khmer_category() == K_Cat(Ra))
      {
	क्रम (अचिन्हित पूर्णांक j = 0; j < 2; j++)
	  info[i + j].mask |= khmer_plan->mask_array[KHMER_PREF];

	/* Move the Coeng,Ro sequence to the start. */
	बफर->merge_clusters (start, i + 2);
	hb_glyph_info_t t0 = info[i];
	hb_glyph_info_t t1 = info[i + 1];
	स_हटाओ (&info[start + 2], &info[start], (i - start) * माप (info[0]));
	info[start] = t0;
	info[start + 1] = t1;

	/* Mark the subsequent stuff with 'cfar'.  Used in Khmer.
	 * Read the feature spec.
	 * This allows distinguishing the following cases with MS Khmer fonts:
	 * U+1784,U+17D2,U+179A,U+17D2,U+1782
	 * U+1784,U+17D2,U+1782,U+17D2,U+179A
	 */
	अगर (khmer_plan->mask_array[KHMER_CFAR])
	  क्रम (अचिन्हित पूर्णांक j = i + 2; j < end; j++)
	    info[j].mask |= khmer_plan->mask_array[KHMER_CFAR];

	अंक_coengs = 2; /* Done. */
      }
    }

    /* Reorder left matra piece. */
    अन्यथा अगर (info[i].khmer_category() == K_Cat(VPre))
    {
      /* Move to the start. */
      बफर->merge_clusters (start, i + 1);
      hb_glyph_info_t t = info[i];
      स_हटाओ (&info[start + 1], &info[start], (i - start) * माप (info[0]));
      info[start] = t;
    }
  }
}

अटल व्योम
reorder_syllable_khmer (स्थिर hb_ot_shape_plan_t *plan,
			hb_face_t *face,
			hb_बफर_t *बफर,
			अचिन्हित पूर्णांक start, अचिन्हित पूर्णांक end)
{
  khmer_syllable_type_t syllable_type = (khmer_syllable_type_t) (बफर->info[start].syllable() & 0x0F);
  चयन (syllable_type)
  {
    हाल khmer_broken_cluster: /* We already inserted dotted-circles, so just call the consonant_syllable. */
    हाल khmer_consonant_syllable:
     reorder_consonant_syllable (plan, face, बफर, start, end);
     अवरोध;

    हाल khmer_non_khmer_cluster:
      अवरोध;
  }
}

अटल bool
reorder_khmer (स्थिर hb_ot_shape_plan_t *plan,
	       hb_font_t *font,
	       hb_बफर_t *बफर)
{
  bool ret = false;
  अगर (बफर->message (font, "start reordering khmer"))
  {
    अगर (hb_syllabic_जोड़ो_dotted_circles (font, बफर,
					   khmer_broken_cluster,
					   K_Cat(DOTTEDCIRCLE),
					   (अचिन्हित) -1))
      ret = true;

    foreach_syllable (बफर, start, end)
      reorder_syllable_khmer (plan, font->face, बफर, start, end);
    (व्योम) बफर->message (font, "end reordering khmer");
  }
  HB_बफर_DEALLOCATE_VAR (बफर, khmer_category);

  वापस ret;
}


अटल bool
decompose_khmer (स्थिर hb_ot_shape_normalize_context_t *c,
		 hb_codepoint_t  ab,
		 hb_codepoint_t *a,
		 hb_codepoint_t *b)
{
  चयन (ab)
  {
    /*
     * Decompose split matras that don't have Unicode decompositions.
     */

    /* Khmer */
    हाल 0x17BEu  : *a = 0x17C1u; *b= 0x17BEu; वापस true;
    हाल 0x17BFu  : *a = 0x17C1u; *b= 0x17BFu; वापस true;
    हाल 0x17C0u  : *a = 0x17C1u; *b= 0x17C0u; वापस true;
    हाल 0x17C4u  : *a = 0x17C1u; *b= 0x17C4u; वापस true;
    हाल 0x17C5u  : *a = 0x17C1u; *b= 0x17C5u; वापस true;
  }

  वापस (bool) c->unicode->decompose (ab, a, b);
}

अटल bool
compose_khmer (स्थिर hb_ot_shape_normalize_context_t *c,
	       hb_codepoint_t  a,
	       hb_codepoint_t  b,
	       hb_codepoint_t *ab)
{
  /* Avoid recomposing split matras. */
  अगर (HB_UNICODE_GENERAL_CATEGORY_IS_MARK (c->unicode->general_category (a)))
    वापस false;

  वापस (bool) c->unicode->compose (a, b, ab);
}


स्थिर hb_ot_shaper_t _hb_ot_shaper_khmer =
{
  collect_features_khmer,
  override_features_khmer,
  तथ्य_create_khmer,
  तथ्य_destroy_khmer,
  nullptr, /* preprocess_text */
  nullptr, /* postprocess_glyphs */
  decompose_khmer,
  compose_khmer,
  setup_masks_khmer,
  nullptr, /* reorder_marks */
  HB_TAG_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_MODE_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT,
  HB_OT_SHAPE_ZERO_WIDTH_MARKS_NONE,
  false, /* fallback_position */
};


#पूर्ण_अगर

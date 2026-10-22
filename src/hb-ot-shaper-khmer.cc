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

#অন্তর্ভাব "hb.hh"

#ifndef HB_NO_OT_SHAPE

#অন্তর্ভাব "hb-ot-shaper-khmer-machine.hh"
#অন্তর্ভাব "hb-ot-shaper-indic.hh"
#অন্তর্ভাব "hb-ot-layout.hh"


/*
 * Khmer shaper.
 */


স্থির ধ্রুবক hb_ot_map_feature_t
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
গণনা {
  KHMER_PREF,
  KHMER_BLWF,
  KHMER_ABVF,
  KHMER_PSTF,
  KHMER_CFAR,

  _KHMER_PRES,
  _KHMER_ABVS,
  _KHMER_BLWS,
  _KHMER_PSTS,

  KHMER_অঙ্ক_FEATURES,
  KHMER_BASIC_FEATURES = _KHMER_PRES, /* Don't forget to update this! */
};

স্থির অন্তঃরেখ শূন্য
set_khmer_properties (hb_glyph_info_t &info)
{
  hb_codepoint_t u = info.codepoint;
  unsigned পূর্ণসংখ্যা type = hb_indic_get_categories (u);

  info.khmer_category() = (khmer_category_t) (type & 0xFFu);
}

স্থির যৌক্তিক
setup_syllables_khmer (ধ্রুবক hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_বাফার_t *বাফার);
স্থির যৌক্তিক
reorder_khmer (ধ্রুবক hb_ot_shape_plan_t *plan,
	       hb_font_t *font,
	       hb_বাফার_t *বাফার);

স্থির শূন্য
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

  unsigned পূর্ণসংখ্যা i = 0;
  জন্য (; i < KHMER_BASIC_FEATURES; i++)
    map->add_feature (khmer_features[i]);

  /* https://github.com/harfbuzz/harfbuzz/issues/3531 */
  map->add_gsub_pause (hb_syllabic_clear_var); // Don't need syllables anymore, use stop to free buffer var

  জন্য (; i < KHMER_অঙ্ক_FEATURES; i++)
    map->add_feature (khmer_features[i]);
}

স্থির শূন্য
override_features_khmer (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  /* Khmer spec has 'clig' as part of required shaping features:
   * "Apply feature 'clig' to form ligatures that are desired for
   * typographical correctness.", hence in overrides... */
  map->enable_feature (HB_TAG('c','l','i','g'));

  map->disable_feature (HB_TAG('l','i','g','a'));
}


কাঠামো khmer_shape_plan_t
{
  hb_mask_t mask_array[KHMER_অঙ্ক_FEATURES];
};

স্থির শূন্য *
তথ্য_create_khmer (ধ্রুবক hb_ot_shape_plan_t *plan)
{
  khmer_shape_plan_t *khmer_plan = (khmer_shape_plan_t *) hb_calloc (1, আকার (khmer_shape_plan_t));
  যদি (unlikely (!khmer_plan))
    ফেরত nullptr;

  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < ARRAY_দৈর্ঘ্য (khmer_plan->mask_array); i++)
    khmer_plan->mask_array[i] = (khmer_features[i].flags & F_GLOBAL) ?
				 0 : plan->map.get_1_mask (khmer_features[i].tag);

  ফেরত khmer_plan;
}

স্থির শূন্য
তথ্য_destroy_khmer (শূন্য *তথ্য)
{
  hb_free (তথ্য);
}

স্থির শূন্য
setup_masks_khmer (ধ্রুবক hb_ot_shape_plan_t *plan HB_UNUSED,
		   hb_বাফার_t              *বাফার,
		   hb_font_t                *font HB_UNUSED)
{
  HB_বাফার_ALLOCATE_VAR (বাফার, khmer_category);

  /* We cannot setup masks here.  We save information about characters
   * and setup masks later on in a pause-callback. */

  unsigned পূর্ণসংখ্যা গুনতি = বাফার->len;
  hb_glyph_info_t *info = বাফার->info;
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    set_khmer_properties (info[i]);
}

স্থির যৌক্তিক
setup_syllables_khmer (ধ্রুবক hb_ot_shape_plan_t *plan HB_UNUSED,
		       hb_font_t *font HB_UNUSED,
		       hb_বাফার_t *বাফার)
{
  HB_বাফার_ALLOCATE_VAR (বাফার, syllable);
  find_syllables_khmer (বাফার);
  foreach_syllable (বাফার, start, end)
    বাফার->unsafe_to_break (start, end);
  ফেরত মিথ্যা;
}


/* Rules from:
 * https://docs.microsoft.com/en-us/typography/script-development/devanagari */

স্থির শূন্য
reorder_consonant_syllable (ধ্রুবক hb_ot_shape_plan_t *plan,
			    hb_face_t *face HB_UNUSED,
			    hb_বাফার_t *বাফার,
			    unsigned পূর্ণসংখ্যা start, unsigned পূর্ণসংখ্যা end)
{
  ধ্রুবক khmer_shape_plan_t *khmer_plan = (ধ্রুবক khmer_shape_plan_t *) plan->তথ্য;
  hb_glyph_info_t *info = বাফার->info;

  /* Setup masks. */
  {
    /* Post-base */
    hb_mask_t mask = khmer_plan->mask_array[KHMER_BLWF] |
		     khmer_plan->mask_array[KHMER_ABVF] |
		     khmer_plan->mask_array[KHMER_PSTF];
    জন্য (unsigned পূর্ণসংখ্যা i = start + 1; i < end; i++)
      info[i].mask  |= mask;
  }

  unsigned পূর্ণসংখ্যা অঙ্ক_coengs = 0;
  জন্য (unsigned পূর্ণসংখ্যা i = start + 1; i < end; i++)
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
    যদি (info[i].khmer_category() == K_Cat(H) && অঙ্ক_coengs <= 2 && i + 1 < end)
    {
      অঙ্ক_coengs++;

      যদি (info[i + 1].khmer_category() == K_Cat(Ra))
      {
	জন্য (unsigned পূর্ণসংখ্যা j = 0; j < 2; j++)
	  info[i + j].mask |= khmer_plan->mask_array[KHMER_PREF];

	/* Move the Coeng,Ro sequence to the start. */
	বাফার->merge_clusters (start, i + 2);
	hb_glyph_info_t t0 = info[i];
	hb_glyph_info_t t1 = info[i + 1];
	memmove (&info[start + 2], &info[start], (i - start) * আকার (info[0]));
	info[start] = t0;
	info[start + 1] = t1;

	/* Mark the subsequent stuff with 'cfar'.  Used in Khmer.
	 * Read the feature spec.
	 * This allows distinguishing the following cases with MS Khmer fonts:
	 * U+1784,U+17D2,U+179A,U+17D2,U+1782
	 * U+1784,U+17D2,U+1782,U+17D2,U+179A
	 */
	যদি (khmer_plan->mask_array[KHMER_CFAR])
	  জন্য (unsigned পূর্ণসংখ্যা j = i + 2; j < end; j++)
	    info[j].mask |= khmer_plan->mask_array[KHMER_CFAR];

	অঙ্ক_coengs = 2; /* Done. */
      }
    }

    /* Reorder left matra piece. */
    অন্যথা যদি (info[i].khmer_category() == K_Cat(VPre))
    {
      /* Move to the start. */
      বাফার->merge_clusters (start, i + 1);
      hb_glyph_info_t t = info[i];
      memmove (&info[start + 1], &info[start], (i - start) * আকার (info[0]));
      info[start] = t;
    }
  }
}

স্থির শূন্য
reorder_syllable_khmer (ধ্রুবক hb_ot_shape_plan_t *plan,
			hb_face_t *face,
			hb_বাফার_t *বাফার,
			unsigned পূর্ণসংখ্যা start, unsigned পূর্ণসংখ্যা end)
{
  khmer_syllable_type_t syllable_type = (khmer_syllable_type_t) (বাফার->info[start].syllable() & 0x0F);
  নির্বাচন (syllable_type)
  {
    ক্ষেত্র khmer_broken_cluster: /* We already inserted dotted-circles, so just call the consonant_syllable. */
    ক্ষেত্র khmer_consonant_syllable:
     reorder_consonant_syllable (plan, face, বাফার, start, end);
     বিরতি;

    ক্ষেত্র khmer_non_khmer_cluster:
      বিরতি;
  }
}

স্থির যৌক্তিক
reorder_khmer (ধ্রুবক hb_ot_shape_plan_t *plan,
	       hb_font_t *font,
	       hb_বাফার_t *বাফার)
{
  যৌক্তিক ret = মিথ্যা;
  যদি (বাফার->message (font, "start reordering khmer"))
  {
    যদি (hb_syllabic_ঢোকাও_dotted_circles (font, বাফার,
					   khmer_broken_cluster,
					   K_Cat(DOTTEDCIRCLE),
					   (unsigned) -1))
      ret = সত্য;

    foreach_syllable (বাফার, start, end)
      reorder_syllable_khmer (plan, font->face, বাফার, start, end);
    (শূন্য) বাফার->message (font, "end reordering khmer");
  }
  HB_বাফার_DEALLOCATE_VAR (বাফার, khmer_category);

  ফেরত ret;
}


স্থির যৌক্তিক
decompose_khmer (ধ্রুবক hb_ot_shape_normalize_context_t *c,
		 hb_codepoint_t  ab,
		 hb_codepoint_t *a,
		 hb_codepoint_t *b)
{
  নির্বাচন (ab)
  {
    /*
     * Decompose split matras that don't have Unicode decompositions.
     */

    /* Khmer */
    ক্ষেত্র 0x17BEu  : *a = 0x17C1u; *b= 0x17BEu; ফেরত সত্য;
    ক্ষেত্র 0x17BFu  : *a = 0x17C1u; *b= 0x17BFu; ফেরত সত্য;
    ক্ষেত্র 0x17C0u  : *a = 0x17C1u; *b= 0x17C0u; ফেরত সত্য;
    ক্ষেত্র 0x17C4u  : *a = 0x17C1u; *b= 0x17C4u; ফেরত সত্য;
    ক্ষেত্র 0x17C5u  : *a = 0x17C1u; *b= 0x17C5u; ফেরত সত্য;
  }

  ফেরত (যৌক্তিক) c->unicode->decompose (ab, a, b);
}

স্থির যৌক্তিক
compose_khmer (ধ্রুবক hb_ot_shape_normalize_context_t *c,
	       hb_codepoint_t  a,
	       hb_codepoint_t  b,
	       hb_codepoint_t *ab)
{
  /* Avoid recomposing split matras. */
  যদি (HB_UNICODE_GENERAL_CATEGORY_IS_MARK (c->unicode->general_category (a)))
    ফেরত মিথ্যা;

  ফেরত (যৌক্তিক) c->unicode->compose (a, b, ab);
}


ধ্রুবক hb_ot_shaper_t _hb_ot_shaper_khmer =
{
  collect_features_khmer,
  override_features_khmer,
  তথ্য_create_khmer,
  তথ্য_destroy_khmer,
  nullptr, /* preprocess_text */
  nullptr, /* postprocess_glyphs */
  decompose_khmer,
  compose_khmer,
  setup_masks_khmer,
  nullptr, /* reorder_marks */
  HB_TAG_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_MODE_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT,
  HB_OT_SHAPE_ZERO_WIDTH_MARKS_NONE,
  মিথ্যা, /* fallback_position */
};


#endif

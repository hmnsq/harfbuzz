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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ifndef HB_NO_OT_SHAPE

#ଅନ୍ତର୍ଭାଵ "hb-ot-shaper-khmer-machine.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-shaper-indic.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-layout.hh"


/*
 * Khmer shaper.
 */


ସ୍ଥିର ଅଚଳ hb_ot_map_feature_t
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
ଗଣନା {
  KHMER_PREF,
  KHMER_BLWF,
  KHMER_ABVF,
  KHMER_PSTF,
  KHMER_CFAR,

  _KHMER_PRES,
  _KHMER_ABVS,
  _KHMER_BLWS,
  _KHMER_PSTS,

  KHMER_ଅଙ୍କ_FEATURES,
  KHMER_BASIC_FEATURES = _KHMER_PRES, /* Don't forget to update this! */
};

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
set_khmer_properties (hb_glyph_info_t &info)
{
  hb_codepoint_t u = info.codepoint;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = hb_indic_get_categories (u);

  info.khmer_category() = (khmer_category_t) (type & 0xFFu);
}

ସ୍ଥିର ତାର୍କିକ
setup_syllables_khmer (ଅଚଳ hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_buffer_t *buffer);
ସ୍ଥିର ତାର୍କିକ
reorder_khmer (ଅଚଳ hb_ot_shape_plan_t *plan,
	       hb_font_t *font,
	       hb_buffer_t *buffer);

ସ୍ଥିର ଶୂନ୍ୟ
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

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
  ପାଇଁ (; i < KHMER_BASIC_FEATURES; i++)
    map->add_feature (khmer_features[i]);

  /* https://github.com/harfbuzz/harfbuzz/issues/3531 */
  map->add_gsub_pause (hb_syllabic_clear_var); // Don't need syllables anymore, use stop to free buffer var

  ପାଇଁ (; i < KHMER_ଅଙ୍କ_FEATURES; i++)
    map->add_feature (khmer_features[i]);
}

ସ୍ଥିର ଶୂନ୍ୟ
override_features_khmer (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  /* Khmer spec has 'clig' as part of required shaping features:
   * "Apply feature 'clig' to form ligatures that are desired for
   * typographical correctness.", hence in overrides... */
  map->enable_feature (HB_TAG('c','l','i','g'));

  map->disable_feature (HB_TAG('l','i','g','a'));
}


ସଂରଚନା khmer_shape_plan_t
{
  hb_mask_t mask_array[KHMER_ଅଙ୍କ_FEATURES];
};

ସ୍ଥିର ଶୂନ୍ୟ *
data_create_khmer (ଅଚଳ hb_ot_shape_plan_t *plan)
{
  khmer_shape_plan_t *khmer_plan = (khmer_shape_plan_t *) hb_calloc (1, ଆକାର (khmer_shape_plan_t));
  ଯଦି (unlikely (!khmer_plan))
    ଫେରନ୍ତୁ nullptr;

  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ARRAY_LENGTH (khmer_plan->mask_array); i++)
    khmer_plan->mask_array[i] = (khmer_features[i].flags & F_GLOBAL) ?
				 0 : plan->map.get_1_mask (khmer_features[i].tag);

  ଫେରନ୍ତୁ khmer_plan;
}

ସ୍ଥିର ଶୂନ୍ୟ
data_destroy_khmer (ଶୂନ୍ୟ *data)
{
  hb_free (data);
}

ସ୍ଥିର ଶୂନ୍ୟ
setup_masks_khmer (ଅଚଳ hb_ot_shape_plan_t *plan HB_UNUSED,
		   hb_buffer_t              *buffer,
		   hb_font_t                *font HB_UNUSED)
{
  HB_BUFFER_ALLOCATE_VAR (buffer, khmer_category);

  /* We cannot setup masks here.  We save information about characters
   * and setup masks later on in a pause-callback. */

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    set_khmer_properties (info[i]);
}

ସ୍ଥିର ତାର୍କିକ
setup_syllables_khmer (ଅଚଳ hb_ot_shape_plan_t *plan HB_UNUSED,
		       hb_font_t *font HB_UNUSED,
		       hb_buffer_t *buffer)
{
  HB_BUFFER_ALLOCATE_VAR (buffer, syllable);
  find_syllables_khmer (buffer);
  foreach_syllable (buffer, start, end)
    buffer->unsafe_to_break (start, end);
  ଫେରନ୍ତୁ ମିଥ୍ୟା;
}


/* Rules from:
 * https://docs.microsoft.com/en-us/typography/script-development/devanagari */

ସ୍ଥିର ଶୂନ୍ୟ
reorder_consonant_syllable (ଅଚଳ hb_ot_shape_plan_t *plan,
			    hb_face_t *face HB_UNUSED,
			    hb_buffer_t *buffer,
			    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end)
{
  ଅଚଳ khmer_shape_plan_t *khmer_plan = (ଅଚଳ khmer_shape_plan_t *) plan->data;
  hb_glyph_info_t *info = buffer->info;

  /* Setup masks. */
  {
    /* Post-base */
    hb_mask_t mask = khmer_plan->mask_array[KHMER_BLWF] |
		     khmer_plan->mask_array[KHMER_ABVF] |
		     khmer_plan->mask_array[KHMER_PSTF];
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = start + 1; i < end; i++)
      info[i].mask  |= mask;
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_coengs = 0;
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = start + 1; i < end; i++)
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
    ଯଦି (info[i].khmer_category() == K_Cat(H) && ଅଙ୍କ_coengs <= 2 && i + 1 < end)
    {
      ଅଙ୍କ_coengs++;

      ଯଦି (info[i + 1].khmer_category() == K_Cat(Ra))
      {
	ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < 2; j++)
	  info[i + j].mask |= khmer_plan->mask_array[KHMER_PREF];

	/* Move the Coeng,Ro sequence to the start. */
	buffer->merge_clusters (start, i + 2);
	hb_glyph_info_t t0 = info[i];
	hb_glyph_info_t t1 = info[i + 1];
	memmove (&info[start + 2], &info[start], (i - start) * ଆକାର (info[0]));
	info[start] = t0;
	info[start + 1] = t1;

	/* Mark the subsequent stuff with 'cfar'.  Used in Khmer.
	 * Read the feature spec.
	 * This allows distinguishing the following cases with MS Khmer fonts:
	 * U+1784,U+17D2,U+179A,U+17D2,U+1782
	 * U+1784,U+17D2,U+1782,U+17D2,U+179A
	 */
	ଯଦି (khmer_plan->mask_array[KHMER_CFAR])
	  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = i + 2; j < end; j++)
	    info[j].mask |= khmer_plan->mask_array[KHMER_CFAR];

	ଅଙ୍କ_coengs = 2; /* Done. */
      }
    }

    /* Reorder left matra piece. */
    ଅନ୍ୟଥା ଯଦି (info[i].khmer_category() == K_Cat(VPre))
    {
      /* Move to the start. */
      buffer->merge_clusters (start, i + 1);
      hb_glyph_info_t t = info[i];
      memmove (&info[start + 1], &info[start], (i - start) * ଆକାର (info[0]));
      info[start] = t;
    }
  }
}

ସ୍ଥିର ଶୂନ୍ୟ
reorder_syllable_khmer (ଅଚଳ hb_ot_shape_plan_t *plan,
			hb_face_t *face,
			hb_buffer_t *buffer,
			unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end)
{
  khmer_syllable_type_t syllable_type = (khmer_syllable_type_t) (buffer->info[start].syllable() & 0x0F);
  ବାଛ (syllable_type)
  {
    ସ୍ଥିତି khmer_broken_cluster: /* We already inserted dotted-circles, so just call the consonant_syllable. */
    ସ୍ଥିତି khmer_consonant_syllable:
     reorder_consonant_syllable (plan, face, buffer, start, end);
     ଭାଙ୍ଗ;

    ସ୍ଥିତି khmer_non_khmer_cluster:
      ଭାଙ୍ଗ;
  }
}

ସ୍ଥିର ତାର୍କିକ
reorder_khmer (ଅଚଳ hb_ot_shape_plan_t *plan,
	       hb_font_t *font,
	       hb_buffer_t *buffer)
{
  ତାର୍କିକ ret = ମିଥ୍ୟା;
  ଯଦି (buffer->message (font, "start reordering khmer"))
  {
    ଯଦି (hb_syllabic_ଭର୍ତ୍ତି_dotted_circles (font, buffer,
					   khmer_broken_cluster,
					   K_Cat(DOTTEDCIRCLE),
					   (unsigned) -1))
      ret = ସତ୍ୟ;

    foreach_syllable (buffer, start, end)
      reorder_syllable_khmer (plan, font->face, buffer, start, end);
    (ଶୂନ୍ୟ) buffer->message (font, "end reordering khmer");
  }
  HB_BUFFER_DEALLOCATE_VAR (buffer, khmer_category);

  ଫେରନ୍ତୁ ret;
}


ସ୍ଥିର ତାର୍କିକ
decompose_khmer (ଅଚଳ hb_ot_shape_normalize_context_t *c,
		 hb_codepoint_t  ab,
		 hb_codepoint_t *a,
		 hb_codepoint_t *b)
{
  ବାଛ (ab)
  {
    /*
     * Decompose split matras that don't have Unicode decompositions.
     */

    /* Khmer */
    ସ୍ଥିତି 0x17BEu  : *a = 0x17C1u; *b= 0x17BEu; ଫେରନ୍ତୁ ସତ୍ୟ;
    ସ୍ଥିତି 0x17BFu  : *a = 0x17C1u; *b= 0x17BFu; ଫେରନ୍ତୁ ସତ୍ୟ;
    ସ୍ଥିତି 0x17C0u  : *a = 0x17C1u; *b= 0x17C0u; ଫେରନ୍ତୁ ସତ୍ୟ;
    ସ୍ଥିତି 0x17C4u  : *a = 0x17C1u; *b= 0x17C4u; ଫେରନ୍ତୁ ସତ୍ୟ;
    ସ୍ଥିତି 0x17C5u  : *a = 0x17C1u; *b= 0x17C5u; ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ଫେରନ୍ତୁ (ତାର୍କିକ) c->unicode->decompose (ab, a, b);
}

ସ୍ଥିର ତାର୍କିକ
compose_khmer (ଅଚଳ hb_ot_shape_normalize_context_t *c,
	       hb_codepoint_t  a,
	       hb_codepoint_t  b,
	       hb_codepoint_t *ab)
{
  /* Avoid recomposing split matras. */
  ଯଦି (HB_UNICODE_GENERAL_CATEGORY_IS_MARK (c->unicode->general_category (a)))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  ଫେରନ୍ତୁ (ତାର୍କିକ) c->unicode->compose (a, b, ab);
}


ଅଚଳ hb_ot_shaper_t _hb_ot_shaper_khmer =
{
  collect_features_khmer,
  override_features_khmer,
  data_create_khmer,
  data_destroy_khmer,
  nullptr, /* preprocess_text */
  nullptr, /* postprocess_glyphs */
  decompose_khmer,
  compose_khmer,
  setup_masks_khmer,
  nullptr, /* reorder_marks */
  HB_TAG_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_MODE_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT,
  HB_OT_SHAPE_ZERO_WIDTH_MARKS_NONE,
  ମିଥ୍ୟା, /* fallback_position */
};


#endif

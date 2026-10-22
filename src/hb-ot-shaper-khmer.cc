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

#അന്തര്ഭാവ "hb.hh"

#ifndef HB_NO_OT_SHAPE

#അന്തര്ഭാവ "hb-ot-shaper-khmer-machine.hh"
#അന്തര്ഭാവ "hb-ot-shaper-indic.hh"
#അന്തര്ഭാവ "hb-ot-layout.hh"


/*
 * Khmer shaper.
 */


സ്ഥിതം സ്ഥിരം hb_ot_map_feature_t
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
എണ്ണൽ {
  KHMER_PREF,
  KHMER_BLWF,
  KHMER_ABVF,
  KHMER_PSTF,
  KHMER_CFAR,

  _KHMER_PRES,
  _KHMER_ABVS,
  _KHMER_BLWS,
  _KHMER_PSTS,

  KHMER_അക്കം_FEATURES,
  KHMER_BASIC_FEATURES = _KHMER_PRES, /* Don't forget to update this! */
};

സ്ഥിതം വരിയിൽ ശൂന്യം
set_khmer_properties (hb_glyph_info_t &info)
{
  hb_codepoint_t u = info.codepoint;
  unsigned പൂർണ്ണസംഖ്യ type = hb_indic_get_categories (u);

  info.khmer_category() = (khmer_category_t) (type & 0xFFu);
}

സ്ഥിതം തർക്കം
setup_syllables_khmer (സ്ഥിരം hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_buffer_t *buffer);
സ്ഥിതം തർക്കം
reorder_khmer (സ്ഥിരം hb_ot_shape_plan_t *plan,
	       hb_font_t *font,
	       hb_buffer_t *buffer);

സ്ഥിതം ശൂന്യം
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

  unsigned പൂർണ്ണസംഖ്യ i = 0;
  വേണ്ടി (; i < KHMER_BASIC_FEATURES; i++)
    map->add_feature (khmer_features[i]);

  /* https://github.com/harfbuzz/harfbuzz/issues/3531 */
  map->add_gsub_pause (hb_syllabic_clear_var); // Don't need syllables anymore, use stop to free buffer var

  വേണ്ടി (; i < KHMER_അക്കം_FEATURES; i++)
    map->add_feature (khmer_features[i]);
}

സ്ഥിതം ശൂന്യം
override_features_khmer (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  /* Khmer spec has 'clig' as part of required shaping features:
   * "Apply feature 'clig' to form ligatures that are desired for
   * typographical correctness.", hence in overrides... */
  map->enable_feature (HB_TAG('c','l','i','g'));

  map->disable_feature (HB_TAG('l','i','g','a'));
}


ഘടന khmer_shape_plan_t
{
  hb_mask_t mask_array[KHMER_അക്കം_FEATURES];
};

സ്ഥിതം ശൂന്യം *
data_create_khmer (സ്ഥിരം hb_ot_shape_plan_t *plan)
{
  khmer_shape_plan_t *khmer_plan = (khmer_shape_plan_t *) hb_calloc (1, വലുപ്പം (khmer_shape_plan_t));
  എങ്കിൽ (unlikely (!khmer_plan))
    തിരികെ nullptr;

  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < ARRAY_LENGTH (khmer_plan->mask_array); i++)
    khmer_plan->mask_array[i] = (khmer_features[i].flags & F_GLOBAL) ?
				 0 : plan->map.get_1_mask (khmer_features[i].tag);

  തിരികെ khmer_plan;
}

സ്ഥിതം ശൂന്യം
data_destroy_khmer (ശൂന്യം *data)
{
  hb_free (data);
}

സ്ഥിതം ശൂന്യം
setup_masks_khmer (സ്ഥിരം hb_ot_shape_plan_t *plan HB_UNUSED,
		   hb_buffer_t              *buffer,
		   hb_font_t                *font HB_UNUSED)
{
  HB_BUFFER_ALLOCATE_VAR (buffer, khmer_category);

  /* We cannot setup masks here.  We save information about characters
   * and setup masks later on in a pause-callback. */

  unsigned പൂർണ്ണസംഖ്യ എണ്ണം = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    set_khmer_properties (info[i]);
}

സ്ഥിതം തർക്കം
setup_syllables_khmer (സ്ഥിരം hb_ot_shape_plan_t *plan HB_UNUSED,
		       hb_font_t *font HB_UNUSED,
		       hb_buffer_t *buffer)
{
  HB_BUFFER_ALLOCATE_VAR (buffer, syllable);
  find_syllables_khmer (buffer);
  foreach_syllable (buffer, start, end)
    buffer->unsafe_to_break (start, end);
  തിരികെ അസത്യം;
}


/* Rules from:
 * https://docs.microsoft.com/en-us/typography/script-development/devanagari */

സ്ഥിതം ശൂന്യം
reorder_consonant_syllable (സ്ഥിരം hb_ot_shape_plan_t *plan,
			    hb_face_t *face HB_UNUSED,
			    hb_buffer_t *buffer,
			    unsigned പൂർണ്ണസംഖ്യ start, unsigned പൂർണ്ണസംഖ്യ end)
{
  സ്ഥിരം khmer_shape_plan_t *khmer_plan = (സ്ഥിരം khmer_shape_plan_t *) plan->data;
  hb_glyph_info_t *info = buffer->info;

  /* Setup masks. */
  {
    /* Post-base */
    hb_mask_t mask = khmer_plan->mask_array[KHMER_BLWF] |
		     khmer_plan->mask_array[KHMER_ABVF] |
		     khmer_plan->mask_array[KHMER_PSTF];
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = start + 1; i < end; i++)
      info[i].mask  |= mask;
  }

  unsigned പൂർണ്ണസംഖ്യ അക്കം_coengs = 0;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = start + 1; i < end; i++)
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
    എങ്കിൽ (info[i].khmer_category() == K_Cat(H) && അക്കം_coengs <= 2 && i + 1 < end)
    {
      അക്കം_coengs++;

      എങ്കിൽ (info[i + 1].khmer_category() == K_Cat(Ra))
      {
	വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ j = 0; j < 2; j++)
	  info[i + j].mask |= khmer_plan->mask_array[KHMER_PREF];

	/* Move the Coeng,Ro sequence to the start. */
	buffer->merge_clusters (start, i + 2);
	hb_glyph_info_t t0 = info[i];
	hb_glyph_info_t t1 = info[i + 1];
	memmove (&info[start + 2], &info[start], (i - start) * വലുപ്പം (info[0]));
	info[start] = t0;
	info[start + 1] = t1;

	/* Mark the subsequent stuff with 'cfar'.  Used in Khmer.
	 * Read the feature spec.
	 * This allows distinguishing the following cases with MS Khmer fonts:
	 * U+1784,U+17D2,U+179A,U+17D2,U+1782
	 * U+1784,U+17D2,U+1782,U+17D2,U+179A
	 */
	എങ്കിൽ (khmer_plan->mask_array[KHMER_CFAR])
	  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ j = i + 2; j < end; j++)
	    info[j].mask |= khmer_plan->mask_array[KHMER_CFAR];

	അക്കം_coengs = 2; /* Done. */
      }
    }

    /* Reorder left matra piece. */
    അല്ലെങ്കിൽ എങ്കിൽ (info[i].khmer_category() == K_Cat(VPre))
    {
      /* Move to the start. */
      buffer->merge_clusters (start, i + 1);
      hb_glyph_info_t t = info[i];
      memmove (&info[start + 1], &info[start], (i - start) * വലുപ്പം (info[0]));
      info[start] = t;
    }
  }
}

സ്ഥിതം ശൂന്യം
reorder_syllable_khmer (സ്ഥിരം hb_ot_shape_plan_t *plan,
			hb_face_t *face,
			hb_buffer_t *buffer,
			unsigned പൂർണ്ണസംഖ്യ start, unsigned പൂർണ്ണസംഖ്യ end)
{
  khmer_syllable_type_t syllable_type = (khmer_syllable_type_t) (buffer->info[start].syllable() & 0x0F);
  തിരഞ്ഞെടുപ്പ് (syllable_type)
  {
    അവസ്ഥ khmer_broken_cluster: /* We already inserted dotted-circles, so just call the consonant_syllable. */
    അവസ്ഥ khmer_consonant_syllable:
     reorder_consonant_syllable (plan, face, buffer, start, end);
     നിർത്തുക;

    അവസ്ഥ khmer_non_khmer_cluster:
      നിർത്തുക;
  }
}

സ്ഥിതം തർക്കം
reorder_khmer (സ്ഥിരം hb_ot_shape_plan_t *plan,
	       hb_font_t *font,
	       hb_buffer_t *buffer)
{
  തർക്കം ret = അസത്യം;
  എങ്കിൽ (buffer->message (font, "start reordering khmer"))
  {
    എങ്കിൽ (hb_syllabic_തിരുകുക_dotted_circles (font, buffer,
					   khmer_broken_cluster,
					   K_Cat(DOTTEDCIRCLE),
					   (unsigned) -1))
      ret = സത്യം;

    foreach_syllable (buffer, start, end)
      reorder_syllable_khmer (plan, font->face, buffer, start, end);
    (ശൂന്യം) buffer->message (font, "end reordering khmer");
  }
  HB_BUFFER_DEALLOCATE_VAR (buffer, khmer_category);

  തിരികെ ret;
}


സ്ഥിതം തർക്കം
decompose_khmer (സ്ഥിരം hb_ot_shape_normalize_context_t *c,
		 hb_codepoint_t  ab,
		 hb_codepoint_t *a,
		 hb_codepoint_t *b)
{
  തിരഞ്ഞെടുപ്പ് (ab)
  {
    /*
     * Decompose split matras that don't have Unicode decompositions.
     */

    /* Khmer */
    അവസ്ഥ 0x17BEu  : *a = 0x17C1u; *b= 0x17BEu; തിരികെ സത്യം;
    അവസ്ഥ 0x17BFu  : *a = 0x17C1u; *b= 0x17BFu; തിരികെ സത്യം;
    അവസ്ഥ 0x17C0u  : *a = 0x17C1u; *b= 0x17C0u; തിരികെ സത്യം;
    അവസ്ഥ 0x17C4u  : *a = 0x17C1u; *b= 0x17C4u; തിരികെ സത്യം;
    അവസ്ഥ 0x17C5u  : *a = 0x17C1u; *b= 0x17C5u; തിരികെ സത്യം;
  }

  തിരികെ (തർക്കം) c->unicode->decompose (ab, a, b);
}

സ്ഥിതം തർക്കം
compose_khmer (സ്ഥിരം hb_ot_shape_normalize_context_t *c,
	       hb_codepoint_t  a,
	       hb_codepoint_t  b,
	       hb_codepoint_t *ab)
{
  /* Avoid recomposing split matras. */
  എങ്കിൽ (HB_UNICODE_GENERAL_CATEGORY_IS_MARK (c->unicode->general_category (a)))
    തിരികെ അസത്യം;

  തിരികെ (തർക്കം) c->unicode->compose (a, b, ab);
}


സ്ഥിരം hb_ot_shaper_t _hb_ot_shaper_khmer =
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
  അസത്യം, /* fallback_position */
};


#endif

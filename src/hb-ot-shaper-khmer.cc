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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPE

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shaper-khmer-machine.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shaper-indic.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout.hh"


/*
 * Khmer shaper.
 */


ਸਥਿਰ ਅਚਲ hb_ot_map_feature_t
khmer_features[] =
{
  /*
   * Basic features.
   * These features are applied all at once, before reordering, constrained
   * to the syllable.
   */
  {HB_ਟੈਗ('p','r','e','f'), F_MANUAL_JOINERS | F_ਪ੍ਰਤੀ_SYLLABLE},
  {HB_ਟੈਗ('b','l','w','f'), F_MANUAL_JOINERS | F_ਪ੍ਰਤੀ_SYLLABLE},
  {HB_ਟੈਗ('a','b','v','f'), F_MANUAL_JOINERS | F_ਪ੍ਰਤੀ_SYLLABLE},
  {HB_ਟੈਗ('p','s','t','f'), F_MANUAL_JOINERS | F_ਪ੍ਰਤੀ_SYLLABLE},
  {HB_ਟੈਗ('c','f','a','r'), F_MANUAL_JOINERS | F_ਪ੍ਰਤੀ_SYLLABLE},
  /*
   * Other features.
   * These features are applied all at once after clearing syllables.
   */
  {HB_ਟੈਗ('p','r','e','s'), F_GLOBAL_MANUAL_JOINERS},
  {HB_ਟੈਗ('a','b','v','s'), F_GLOBAL_MANUAL_JOINERS},
  {HB_ਟੈਗ('b','l','w','s'), F_GLOBAL_MANUAL_JOINERS},
  {HB_ਟੈਗ('p','s','t','s'), F_GLOBAL_MANUAL_JOINERS},
};

/*
 * Must be in the same order as the khmer_features array.
 */
ਗਿਣਤੀ {
  KHMER_PREF,
  KHMER_BLWF,
  KHMER_ABVF,
  KHMER_PSTF,
  KHMER_CFAR,

  _KHMER_PRES,
  _KHMER_ABVS,
  _KHMER_BLWS,
  _KHMER_PSTS,

  KHMER_ਅੰਕ_FEATURES,
  KHMER_BASIC_FEATURES = _KHMER_PRES, /* Don't forget to update this! */
};

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
ਸੈੱਟ_khmer_properties (hb_glyph_info_t &info)
{
  hb_codepoint_t u = info.codepoint;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ type = hb_indic_get_categories (u);

  info.khmer_category() = (khmer_category_t) (type & 0xFFu);
}

ਸਥਿਰ ਤਾਰਕਿਕ
ਤਿਆਰੀ_syllables_khmer (ਅਚਲ hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_ਬਫਰ_t *ਬਫਰ);
ਸਥਿਰ ਤਾਰਕਿਕ
reorder_khmer (ਅਚਲ hb_ot_shape_plan_t *plan,
	       hb_font_t *font,
	       hb_ਬਫਰ_t *ਬਫਰ);

ਸਥਿਰ ਖਾਲੀ
collect_features_khmer (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  /* Do this before any lookups have been applied. */
  map->ਜੋੜੋ_gsub_pause (ਤਿਆਰੀ_syllables_khmer);
  map->ਜੋੜੋ_gsub_pause (reorder_khmer);

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
  map->enable_feature (HB_ਟੈਗ('l','o','c','l'), F_ਪ੍ਰਤੀ_SYLLABLE);
  map->enable_feature (HB_ਟੈਗ('c','c','m','p'), F_ਪ੍ਰਤੀ_SYLLABLE);

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0;
  ਲਈ (; i < KHMER_BASIC_FEATURES; i++)
    map->ਜੋੜੋ_feature (khmer_features[i]);

  /* https://github.com/harfbuzz/harfbuzz/issues/3531 */
  map->ਜੋੜੋ_gsub_pause (hb_syllabic_clear_var); // Don't need syllables anymore, use stop to free buffer var

  ਲਈ (; i < KHMER_ਅੰਕ_FEATURES; i++)
    map->ਜੋੜੋ_feature (khmer_features[i]);
}

ਸਥਿਰ ਖਾਲੀ
override_features_khmer (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  /* Khmer spec has 'clig' as part of required shaping features:
   * "Apply feature 'clig' to form ligatures that are desired for
   * typographical correctness.", hence in overrides... */
  map->enable_feature (HB_ਟੈਗ('c','l','i','g'));

  map->disable_feature (HB_ਟੈਗ('l','i','g','a'));
}


ਬਣਤਰ khmer_shape_plan_t
{
  hb_ਨਕਾਬ_t ਨਕਾਬ_array[KHMER_ਅੰਕ_FEATURES];
};

ਸਥਿਰ ਖਾਲੀ *
ਅੰਕੜੇ_create_khmer (ਅਚਲ hb_ot_shape_plan_t *plan)
{
  khmer_shape_plan_t *khmer_plan = (khmer_shape_plan_t *) hb_calloc (1, ਆਕਾਰ (khmer_shape_plan_t));
  ਜੇ (unlikely (!khmer_plan))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ARRAY_ਲੰਮਾਈ (khmer_plan->ਨਕਾਬ_array); i++)
    khmer_plan->ਨਕਾਬ_array[i] = (khmer_features[i].flags & F_GLOBAL) ?
				 0 : plan->map.get_1_ਨਕਾਬ (khmer_features[i].ਟੈਗ_2);

  ਵਾਪਸ khmer_plan;
}

ਸਥਿਰ ਖਾਲੀ
ਅੰਕੜੇ_destroy_khmer (ਖਾਲੀ *ਅੰਕੜੇ)
{
  hb_free (ਅੰਕੜੇ);
}

ਸਥਿਰ ਖਾਲੀ
ਤਿਆਰੀ_masks_khmer (ਅਚਲ hb_ot_shape_plan_t *plan HB_ਅਣਵਰਤਿਆ,
		   hb_ਬਫਰ_t              *ਬਫਰ,
		   hb_font_t                *font HB_ਅਣਵਰਤਿਆ)
{
  HB_ਬਫਰ_ALLOCATE_VAR (ਬਫਰ, khmer_category);

  /* We cannot setup masks here.  We save information about characters
   * and setup masks later on in a pause-callback. */

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਬਫਰ->len;
  hb_glyph_info_t *info = ਬਫਰ->info;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    ਸੈੱਟ_khmer_properties (info[i]);
}

ਸਥਿਰ ਤਾਰਕਿਕ
ਤਿਆਰੀ_syllables_khmer (ਅਚਲ hb_ot_shape_plan_t *plan HB_ਅਣਵਰਤਿਆ,
		       hb_font_t *font HB_ਅਣਵਰਤਿਆ,
		       hb_ਬਫਰ_t *ਬਫਰ)
{
  HB_ਬਫਰ_ALLOCATE_VAR (ਬਫਰ, syllable);
  ਲੱਭੋ_syllables_khmer (ਬਫਰ);
  foreach_syllable (ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ)
    ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_break (ਸ਼ੁਰੂ, ਸਿਰਾ);
  ਵਾਪਸ ਝੂਠ;
}


/* Rules from:
 * https://docs.microsoft.com/en-us/typography/script-development/devanagari */

ਸਥਿਰ ਖਾਲੀ
reorder_consonant_syllable (ਅਚਲ hb_ot_shape_plan_t *plan,
			    hb_face_t *face HB_ਅਣਵਰਤਿਆ,
			    hb_ਬਫਰ_t *ਬਫਰ,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ)
{
  ਅਚਲ khmer_shape_plan_t *khmer_plan = (ਅਚਲ khmer_shape_plan_t *) plan->ਅੰਕੜੇ;
  hb_glyph_info_t *info = ਬਫਰ->info;

  /* Setup masks. */
  {
    /* Post-base */
    hb_ਨਕਾਬ_t ਨਕਾਬ = khmer_plan->ਨਕਾਬ_array[KHMER_BLWF] |
		     khmer_plan->ਨਕਾਬ_array[KHMER_ABVF] |
		     khmer_plan->ਨਕਾਬ_array[KHMER_PSTF];
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ + 1; i < ਸਿਰਾ; i++)
      info[i].ਨਕਾਬ  |= ਨਕਾਬ;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_coengs = 0;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ + 1; i < ਸਿਰਾ; i++)
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
    ਜੇ (info[i].khmer_category() == K_Cat(H) && ਅੰਕ_coengs <= 2 && i + 1 < ਸਿਰਾ)
    {
      ਅੰਕ_coengs++;

      ਜੇ (info[i + 1].khmer_category() == K_Cat(Ra))
      {
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = 0; j < 2; j++)
	  info[i + j].ਨਕਾਬ |= khmer_plan->ਨਕਾਬ_array[KHMER_PREF];

	/* Move the Coeng,Ro sequence to the start. */
	ਬਫਰ->ਮਿਲਾਓ_clusters (ਸ਼ੁਰੂ, i + 2);
	hb_glyph_info_t t0 = info[i];
	hb_glyph_info_t t1 = info[i + 1];
	memmove (&info[ਸ਼ੁਰੂ + 2], &info[ਸ਼ੁਰੂ], (i - ਸ਼ੁਰੂ) * ਆਕਾਰ (info[0]));
	info[ਸ਼ੁਰੂ] = t0;
	info[ਸ਼ੁਰੂ + 1] = t1;

	/* Mark the subsequent stuff with 'cfar'.  Used in Khmer.
	 * Read the feature spec.
	 * This allows distinguishing the following cases with MS Khmer fonts:
	 * U+1784,U+17D2,U+179A,U+17D2,U+1782
	 * U+1784,U+17D2,U+1782,U+17D2,U+179A
	 */
	ਜੇ (khmer_plan->ਨਕਾਬ_array[KHMER_CFAR])
	  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = i + 2; j < ਸਿਰਾ; j++)
	    info[j].ਨਕਾਬ |= khmer_plan->ਨਕਾਬ_array[KHMER_CFAR];

	ਅੰਕ_coengs = 2; /* Done. */
      }
    }

    /* Reorder left matra piece. */
    ਨਹੀਂਤਾਂ ਜੇ (info[i].khmer_category() == K_Cat(VPre))
    {
      /* Move to the start. */
      ਬਫਰ->ਮਿਲਾਓ_clusters (ਸ਼ੁਰੂ, i + 1);
      hb_glyph_info_t t = info[i];
      memmove (&info[ਸ਼ੁਰੂ + 1], &info[ਸ਼ੁਰੂ], (i - ਸ਼ੁਰੂ) * ਆਕਾਰ (info[0]));
      info[ਸ਼ੁਰੂ] = t;
    }
  }
}

ਸਥਿਰ ਖਾਲੀ
reorder_syllable_khmer (ਅਚਲ hb_ot_shape_plan_t *plan,
			hb_face_t *face,
			hb_ਬਫਰ_t *ਬਫਰ,
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ)
{
  khmer_syllable_type_t syllable_type = (khmer_syllable_type_t) (ਬਫਰ->info[ਸ਼ੁਰੂ].syllable() & 0x0F);
  ਚੋਣ (syllable_type)
  {
    ਹਾਲਤ khmer_broken_cluster: /* We already inserted dotted-circles, so just call the consonant_syllable. */
    ਹਾਲਤ khmer_consonant_syllable:
     reorder_consonant_syllable (plan, face, ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ);
     ਰੋਕੋ;

    ਹਾਲਤ khmer_non_khmer_cluster:
      ਰੋਕੋ;
  }
}

ਸਥਿਰ ਤਾਰਕਿਕ
reorder_khmer (ਅਚਲ hb_ot_shape_plan_t *plan,
	       hb_font_t *font,
	       hb_ਬਫਰ_t *ਬਫਰ)
{
  ਤਾਰਕਿਕ ret = ਝੂਠ;
  ਜੇ (ਬਫਰ->ਸੁਨੇਹਾ (font, "start reordering khmer"))
  {
    ਜੇ (hb_syllabic_ਪਾਓ_dotted_circles (font, ਬਫਰ,
					   khmer_broken_cluster,
					   K_Cat(DOTTEDCIRCLE),
					   (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1))
      ret = ਸੱਚ;

    foreach_syllable (ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ)
      reorder_syllable_khmer (plan, font->face, ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ);
    (ਖਾਲੀ) ਬਫਰ->ਸੁਨੇਹਾ (font, "end reordering khmer");
  }
  HB_ਬਫਰ_DEALLOCATE_VAR (ਬਫਰ, khmer_category);

  ਵਾਪਸ ret;
}


ਸਥਿਰ ਤਾਰਕਿਕ
decompose_khmer (ਅਚਲ hb_ot_shape_normalize_context_t *c,
		 hb_codepoint_t  ab,
		 hb_codepoint_t *a,
		 hb_codepoint_t *b)
{
  ਚੋਣ (ab)
  {
    /*
     * Decompose split matras that don't have Unicode decompositions.
     */

    /* Khmer */
    ਹਾਲਤ 0x17BEu  : *a = 0x17C1u; *b= 0x17BEu; ਵਾਪਸ ਸੱਚ;
    ਹਾਲਤ 0x17BFu  : *a = 0x17C1u; *b= 0x17BFu; ਵਾਪਸ ਸੱਚ;
    ਹਾਲਤ 0x17C0u  : *a = 0x17C1u; *b= 0x17C0u; ਵਾਪਸ ਸੱਚ;
    ਹਾਲਤ 0x17C4u  : *a = 0x17C1u; *b= 0x17C4u; ਵਾਪਸ ਸੱਚ;
    ਹਾਲਤ 0x17C5u  : *a = 0x17C1u; *b= 0x17C5u; ਵਾਪਸ ਸੱਚ;
  }

  ਵਾਪਸ (ਤਾਰਕਿਕ) c->unicode->decompose (ab, a, b);
}

ਸਥਿਰ ਤਾਰਕਿਕ
compose_khmer (ਅਚਲ hb_ot_shape_normalize_context_t *c,
	       hb_codepoint_t  a,
	       hb_codepoint_t  b,
	       hb_codepoint_t *ab)
{
  /* Avoid recomposing split matras. */
  ਜੇ (HB_UNICODE_GENERAL_CATEGORY_IS_MARK (c->unicode->general_category (a)))
    ਵਾਪਸ ਝੂਠ;

  ਵਾਪਸ (ਤਾਰਕਿਕ) c->unicode->compose (a, b, ab);
}


ਅਚਲ hb_ot_shaper_t _hb_ot_shaper_khmer =
{
  collect_features_khmer,
  override_features_khmer,
  ਅੰਕੜੇ_create_khmer,
  ਅੰਕੜੇ_destroy_khmer,
  ਖਾਲੀ_ਸੂਚਕ, /* preprocess_text */
  ਖਾਲੀ_ਸੂਚਕ, /* postprocess_glyphs */
  decompose_khmer,
  compose_khmer,
  ਤਿਆਰੀ_masks_khmer,
  ਖਾਲੀ_ਸੂਚਕ, /* reorder_marks */
  HB_ਟੈਗ_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_ਬਹੁਲਕ_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT,
  HB_OT_SHAPE_ZERO_ਚੌੜਾਈ_MARKS_NONE,
  ਝੂਠ, /* fallback_position */
};


#ਜੇ_ਅੰਤ

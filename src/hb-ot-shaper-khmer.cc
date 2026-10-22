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

#شامل "hb.hh"

#اگر_غیر_تعریف_شدہ HB_NO_OT_SHAPE

#شامل "hb-ot-shaper-khmer-machine.hh"
#شامل "hb-ot-shaper-indic.hh"
#شامل "hb-ot-layout.hh"


/*
 * Khmer shaper.
 */


ساکن ثابت hb_ot_map_feature_t
khmer_features[] =
{
  /*
   * Basic features.
   * These features are applied all at once, before reordering, constrained
   * to the syllable.
   */
  {HB_لیبل('p','r','e','f'), F_MANUAL_JOINERS | F_فی_SYLLABLE},
  {HB_لیبل('b','l','w','f'), F_MANUAL_JOINERS | F_فی_SYLLABLE},
  {HB_لیبل('a','b','v','f'), F_MANUAL_JOINERS | F_فی_SYLLABLE},
  {HB_لیبل('p','s','t','f'), F_MANUAL_JOINERS | F_فی_SYLLABLE},
  {HB_لیبل('c','f','a','r'), F_MANUAL_JOINERS | F_فی_SYLLABLE},
  /*
   * Other features.
   * These features are applied all at once after clearing syllables.
   */
  {HB_لیبل('p','r','e','s'), F_GLOBAL_MANUAL_JOINERS},
  {HB_لیبل('a','b','v','s'), F_GLOBAL_MANUAL_JOINERS},
  {HB_لیبل('b','l','w','s'), F_GLOBAL_MANUAL_JOINERS},
  {HB_لیبل('p','s','t','s'), F_GLOBAL_MANUAL_JOINERS},
};

/*
 * Must be in the same order as the khmer_features array.
 */
شمار {
  KHMER_PREF,
  KHMER_BLWF,
  KHMER_ABVF,
  KHMER_PSTF,
  KHMER_CFAR,

  _KHMER_PRES,
  _KHMER_ABVS,
  _KHMER_BLWS,
  _KHMER_PSTS,

  KHMER_عدد_FEATURES,
  KHMER_BASIC_FEATURES = _KHMER_PRES, /* Don't forget to update this! */
};

ساکن اندرونی خالی
سیٹ_khmer_properties (hb_glyph_info_t &info)
{
  hb_codepoint_t u = info.codepoint;
  غیرعلامتی صحیح type = hb_indic_get_categories (u);

  info.khmer_category() = (khmer_category_t) (type & 0xFFu);
}

ساکن منطقی
تیاری_syllables_khmer (ثابت hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_بفر_t *بفر);
ساکن منطقی
reorder_khmer (ثابت hb_ot_shape_plan_t *plan,
	       hb_font_t *font,
	       hb_بفر_t *بفر);

ساکن خالی
collect_features_khmer (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  /* Do this before any lookups have been applied. */
  map->جوڑو_gsub_pause (تیاری_syllables_khmer);
  map->جوڑو_gsub_pause (reorder_khmer);

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
  map->enable_feature (HB_لیبل('l','o','c','l'), F_فی_SYLLABLE);
  map->enable_feature (HB_لیبل('c','c','m','p'), F_فی_SYLLABLE);

  غیرعلامتی صحیح i = 0;
  برائے (; i < KHMER_BASIC_FEATURES; i++)
    map->جوڑو_feature (khmer_features[i]);

  /* https://github.com/harfbuzz/harfbuzz/issues/3531 */
  map->جوڑو_gsub_pause (hb_syllabic_clear_var); // Don't need syllables anymore, use stop to free buffer var

  برائے (; i < KHMER_عدد_FEATURES; i++)
    map->جوڑو_feature (khmer_features[i]);
}

ساکن خالی
override_features_khmer (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  /* Khmer spec has 'clig' as part of required shaping features:
   * "Apply feature 'clig' to form ligatures that are desired for
   * typographical correctness.", hence in overrides... */
  map->enable_feature (HB_لیبل('c','l','i','g'));

  map->disable_feature (HB_لیبل('l','i','g','a'));
}


ساخت khmer_shape_plan_t
{
  hb_نقاب_t نقاب_array[KHMER_عدد_FEATURES];
};

ساکن خالی *
ڈیٹا_create_khmer (ثابت hb_ot_shape_plan_t *plan)
{
  khmer_shape_plan_t *khmer_plan = (khmer_shape_plan_t *) hb_calloc (1, حجم (khmer_shape_plan_t));
  اگر (unlikely (!khmer_plan))
    واپس خالی_اشاریہ;

  برائے (غیرعلامتی صحیح i = 0; i < ARRAY_لمبائی (khmer_plan->نقاب_array); i++)
    khmer_plan->نقاب_array[i] = (khmer_features[i].flags & F_GLOBAL) ?
				 0 : plan->map.get_1_نقاب (khmer_features[i].لیبل_2);

  واپس khmer_plan;
}

ساکن خالی
ڈیٹا_destroy_khmer (خالی *ڈیٹا)
{
  hb_free (ڈیٹا);
}

ساکن خالی
تیاری_masks_khmer (ثابت hb_ot_shape_plan_t *plan HB_غیر_مستعمل,
		   hb_بفر_t              *بفر,
		   hb_font_t                *font HB_غیر_مستعمل)
{
  HB_بفر_ALLOCATE_VAR (بفر, khmer_category);

  /* We cannot setup masks here.  We save information about characters
   * and setup masks later on in a pause-callback. */

  غیرعلامتی صحیح گنتی = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    سیٹ_khmer_properties (info[i]);
}

ساکن منطقی
تیاری_syllables_khmer (ثابت hb_ot_shape_plan_t *plan HB_غیر_مستعمل,
		       hb_font_t *font HB_غیر_مستعمل,
		       hb_بفر_t *بفر)
{
  HB_بفر_ALLOCATE_VAR (بفر, syllable);
  ڈھونڈو_syllables_khmer (بفر);
  foreach_syllable (بفر, آغاز, انتہا)
    بفر->unsafe_مقصد_break (آغاز, انتہا);
  واپس جھوٹ;
}


/* Rules from:
 * https://docs.microsoft.com/en-us/typography/script-development/devanagari */

ساکن خالی
reorder_consonant_syllable (ثابت hb_ot_shape_plan_t *plan,
			    hb_face_t *face HB_غیر_مستعمل,
			    hb_بفر_t *بفر,
			    غیرعلامتی صحیح آغاز, غیرعلامتی صحیح انتہا)
{
  ثابت khmer_shape_plan_t *khmer_plan = (ثابت khmer_shape_plan_t *) plan->ڈیٹا;
  hb_glyph_info_t *info = بفر->info;

  /* Setup masks. */
  {
    /* Post-base */
    hb_نقاب_t نقاب = khmer_plan->نقاب_array[KHMER_BLWF] |
		     khmer_plan->نقاب_array[KHMER_ABVF] |
		     khmer_plan->نقاب_array[KHMER_PSTF];
    برائے (غیرعلامتی صحیح i = آغاز + 1; i < انتہا; i++)
      info[i].نقاب  |= نقاب;
  }

  غیرعلامتی صحیح عدد_coengs = 0;
  برائے (غیرعلامتی صحیح i = آغاز + 1; i < انتہا; i++)
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
    اگر (info[i].khmer_category() == K_Cat(H) && عدد_coengs <= 2 && i + 1 < انتہا)
    {
      عدد_coengs++;

      اگر (info[i + 1].khmer_category() == K_Cat(Ra))
      {
	برائے (غیرعلامتی صحیح j = 0; j < 2; j++)
	  info[i + j].نقاب |= khmer_plan->نقاب_array[KHMER_PREF];

	/* Move the Coeng,Ro sequence to the start. */
	بفر->ملاؤ_clusters (آغاز, i + 2);
	hb_glyph_info_t t0 = info[i];
	hb_glyph_info_t t1 = info[i + 1];
	memmove (&info[آغاز + 2], &info[آغاز], (i - آغاز) * حجم (info[0]));
	info[آغاز] = t0;
	info[آغاز + 1] = t1;

	/* Mark the subsequent stuff with 'cfar'.  Used in Khmer.
	 * Read the feature spec.
	 * This allows distinguishing the following cases with MS Khmer fonts:
	 * U+1784,U+17D2,U+179A,U+17D2,U+1782
	 * U+1784,U+17D2,U+1782,U+17D2,U+179A
	 */
	اگر (khmer_plan->نقاب_array[KHMER_CFAR])
	  برائے (غیرعلامتی صحیح j = i + 2; j < انتہا; j++)
	    info[j].نقاب |= khmer_plan->نقاب_array[KHMER_CFAR];

	عدد_coengs = 2; /* Done. */
      }
    }

    /* Reorder left matra piece. */
    ورنہ اگر (info[i].khmer_category() == K_Cat(VPre))
    {
      /* Move to the start. */
      بفر->ملاؤ_clusters (آغاز, i + 1);
      hb_glyph_info_t t = info[i];
      memmove (&info[آغاز + 1], &info[آغاز], (i - آغاز) * حجم (info[0]));
      info[آغاز] = t;
    }
  }
}

ساکن خالی
reorder_syllable_khmer (ثابت hb_ot_shape_plan_t *plan,
			hb_face_t *face,
			hb_بفر_t *بفر,
			غیرعلامتی صحیح آغاز, غیرعلامتی صحیح انتہا)
{
  khmer_syllable_type_t syllable_type = (khmer_syllable_type_t) (بفر->info[آغاز].syllable() & 0x0F);
  انتخاب (syllable_type)
  {
    حالت khmer_broken_cluster: /* We already inserted dotted-circles, so just call the consonant_syllable. */
    حالت khmer_consonant_syllable:
     reorder_consonant_syllable (plan, face, بفر, آغاز, انتہا);
     توڑو;

    حالت khmer_non_khmer_cluster:
      توڑو;
  }
}

ساکن منطقی
reorder_khmer (ثابت hb_ot_shape_plan_t *plan,
	       hb_font_t *font,
	       hb_بفر_t *بفر)
{
  منطقی ret = جھوٹ;
  اگر (بفر->پیغام (font, "start reordering khmer"))
  {
    اگر (hb_syllabic_داخل_کرو_dotted_circles (font, بفر,
					   khmer_broken_cluster,
					   K_Cat(DOTTEDCIRCLE),
					   (غیرعلامتی) -1))
      ret = سچ;

    foreach_syllable (بفر, آغاز, انتہا)
      reorder_syllable_khmer (plan, font->face, بفر, آغاز, انتہا);
    (خالی) بفر->پیغام (font, "end reordering khmer");
  }
  HB_بفر_DEALLOCATE_VAR (بفر, khmer_category);

  واپس ret;
}


ساکن منطقی
decompose_khmer (ثابت hb_ot_shape_normalize_context_t *c,
		 hb_codepoint_t  ab,
		 hb_codepoint_t *a,
		 hb_codepoint_t *b)
{
  انتخاب (ab)
  {
    /*
     * Decompose split matras that don't have Unicode decompositions.
     */

    /* Khmer */
    حالت 0x17BEu  : *a = 0x17C1u; *b= 0x17BEu; واپس سچ;
    حالت 0x17BFu  : *a = 0x17C1u; *b= 0x17BFu; واپس سچ;
    حالت 0x17C0u  : *a = 0x17C1u; *b= 0x17C0u; واپس سچ;
    حالت 0x17C4u  : *a = 0x17C1u; *b= 0x17C4u; واپس سچ;
    حالت 0x17C5u  : *a = 0x17C1u; *b= 0x17C5u; واپس سچ;
  }

  واپس (منطقی) c->unicode->decompose (ab, a, b);
}

ساکن منطقی
compose_khmer (ثابت hb_ot_shape_normalize_context_t *c,
	       hb_codepoint_t  a,
	       hb_codepoint_t  b,
	       hb_codepoint_t *ab)
{
  /* Avoid recomposing split matras. */
  اگر (HB_UNICODE_GENERAL_CATEGORY_IS_MARK (c->unicode->general_category (a)))
    واپس جھوٹ;

  واپس (منطقی) c->unicode->compose (a, b, ab);
}


ثابت hb_ot_shaper_t _hb_ot_shaper_khmer =
{
  collect_features_khmer,
  override_features_khmer,
  ڈیٹا_create_khmer,
  ڈیٹا_destroy_khmer,
  خالی_اشاریہ, /* preprocess_text */
  خالی_اشاریہ, /* postprocess_glyphs */
  decompose_khmer,
  compose_khmer,
  تیاری_masks_khmer,
  خالی_اشاریہ, /* reorder_marks */
  HB_لیبل_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_منوال_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT,
  HB_OT_SHAPE_ZERO_چوڑائی_MARKS_NONE,
  جھوٹ, /* fallback_position */
};


#اگر_ختم

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

#ضمن "hb.hh"

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_SHAPE

#ضمن "hb-ot-shaper-khmer-machine.hh"
#ضمن "hb-ot-shaper-indic.hh"
#ضمن "hb-ot-layout.hh"


/*
 * Khmer shaper.
 */


ساكن ثابت hb_ot_map_feature_t
khmer_features[] =
{
  /*
   * Basic features.
   * These features are applied all at once, before reordering, constrained
   * to the syllable.
   */
  {HB_ٽيگ('p','r','e','f'), F_MANUAL_JOINERS | F_في_SYLLABLE},
  {HB_ٽيگ('b','l','w','f'), F_MANUAL_JOINERS | F_في_SYLLABLE},
  {HB_ٽيگ('a','b','v','f'), F_MANUAL_JOINERS | F_في_SYLLABLE},
  {HB_ٽيگ('p','s','t','f'), F_MANUAL_JOINERS | F_في_SYLLABLE},
  {HB_ٽيگ('c','f','a','r'), F_MANUAL_JOINERS | F_في_SYLLABLE},
  /*
   * Other features.
   * These features are applied all at once after clearing syllables.
   */
  {HB_ٽيگ('p','r','e','s'), F_GLOBAL_MANUAL_JOINERS},
  {HB_ٽيگ('a','b','v','s'), F_GLOBAL_MANUAL_JOINERS},
  {HB_ٽيگ('b','l','w','s'), F_GLOBAL_MANUAL_JOINERS},
  {HB_ٽيگ('p','s','t','s'), F_GLOBAL_MANUAL_JOINERS},
};

/*
 * Must be in the same order as the khmer_features array.
 */
تعداد {
  KHMER_PREF,
  KHMER_BLWF,
  KHMER_ABVF,
  KHMER_PSTF,
  KHMER_CFAR,

  _KHMER_PRES,
  _KHMER_ABVS,
  _KHMER_BLWS,
  _KHMER_PSTS,

  KHMER_انگ_FEATURES,
  KHMER_BASIC_FEATURES = _KHMER_PRES, /* Don't forget to update this! */
};

ساكن مضمن فراغ
سيٽ_khmer_properties (hb_glyph_info_t &info)
{
  hb_codepoint_t u = info.codepoint;
  بنانشان صحيح type = hb_indic_get_categories (u);

  info.khmer_category() = (khmer_category_t) (type & 0xFFu);
}

ساكن منطقي
تياري_syllables_khmer (ثابت hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_بفر_t *بفر);
ساكن منطقي
reorder_khmer (ثابت hb_ot_shape_plan_t *plan,
	       hb_font_t *font,
	       hb_بفر_t *بفر);

ساكن فراغ
collect_features_khmer (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  /* Do this before any lookups have been applied. */
  map->جوڙيو_gsub_pause (تياري_syllables_khmer);
  map->جوڙيو_gsub_pause (reorder_khmer);

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
  map->enable_feature (HB_ٽيگ('l','o','c','l'), F_في_SYLLABLE);
  map->enable_feature (HB_ٽيگ('c','c','m','p'), F_في_SYLLABLE);

  بنانشان صحيح i = 0;
  لكل (; i < KHMER_BASIC_FEATURES; i++)
    map->جوڙيو_feature (khmer_features[i]);

  /* https://github.com/harfbuzz/harfbuzz/issues/3531 */
  map->جوڙيو_gsub_pause (hb_syllabic_clear_var); // Don't need syllables anymore, use stop to free buffer var

  لكل (; i < KHMER_انگ_FEATURES; i++)
    map->جوڙيو_feature (khmer_features[i]);
}

ساكن فراغ
override_features_khmer (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  /* Khmer spec has 'clig' as part of required shaping features:
   * "Apply feature 'clig' to form ligatures that are desired for
   * typographical correctness.", hence in overrides... */
  map->enable_feature (HB_ٽيگ('c','l','i','g'));

  map->disable_feature (HB_ٽيگ('l','i','g','a'));
}


بنية khmer_shape_plan_t
{
  hb_نقاب_t نقاب_array[KHMER_انگ_FEATURES];
};

ساكن فراغ *
معلومات_create_khmer (ثابت hb_ot_shape_plan_t *plan)
{
  khmer_shape_plan_t *khmer_plan = (khmer_shape_plan_t *) hb_calloc (1, حجم (khmer_shape_plan_t));
  إذا (unlikely (!khmer_plan))
    أرجع خالي_اشارو;

  لكل (بنانشان صحيح i = 0; i < ARRAY_ڊيگهائي (khmer_plan->نقاب_array); i++)
    khmer_plan->نقاب_array[i] = (khmer_features[i].flags & F_GLOBAL) ?
				 0 : plan->map.get_1_نقاب (khmer_features[i].ٽيگ_2);

  أرجع khmer_plan;
}

ساكن فراغ
معلومات_destroy_khmer (فراغ *معلومات)
{
  hb_free (معلومات);
}

ساكن فراغ
تياري_masks_khmer (ثابت hb_ot_shape_plan_t *plan HB_اڻ_استعمال_ٿيل,
		   hb_بفر_t              *بفر,
		   hb_font_t                *font HB_اڻ_استعمال_ٿيل)
{
  HB_بفر_ALLOCATE_VAR (بفر, khmer_category);

  /* We cannot setup masks here.  We save information about characters
   * and setup masks later on in a pause-callback. */

  بنانشان صحيح ڳڻپ = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    سيٽ_khmer_properties (info[i]);
}

ساكن منطقي
تياري_syllables_khmer (ثابت hb_ot_shape_plan_t *plan HB_اڻ_استعمال_ٿيل,
		       hb_font_t *font HB_اڻ_استعمال_ٿيل,
		       hb_بفر_t *بفر)
{
  HB_بفر_ALLOCATE_VAR (بفر, syllable);
  ڳوليو_syllables_khmer (بفر);
  foreach_syllable (بفر, شروعات, پڇاڙي)
    بفر->unsafe_منزل_break (شروعات, پڇاڙي);
  أرجع خطأ;
}


/* Rules from:
 * https://docs.microsoft.com/en-us/typography/script-development/devanagari */

ساكن فراغ
reorder_consonant_syllable (ثابت hb_ot_shape_plan_t *plan,
			    hb_face_t *face HB_اڻ_استعمال_ٿيل,
			    hb_بفر_t *بفر,
			    بنانشان صحيح شروعات, بنانشان صحيح پڇاڙي)
{
  ثابت khmer_shape_plan_t *khmer_plan = (ثابت khmer_shape_plan_t *) plan->معلومات;
  hb_glyph_info_t *info = بفر->info;

  /* Setup masks. */
  {
    /* Post-base */
    hb_نقاب_t نقاب = khmer_plan->نقاب_array[KHMER_BLWF] |
		     khmer_plan->نقاب_array[KHMER_ABVF] |
		     khmer_plan->نقاب_array[KHMER_PSTF];
    لكل (بنانشان صحيح i = شروعات + 1; i < پڇاڙي; i++)
      info[i].نقاب  |= نقاب;
  }

  بنانشان صحيح انگ_coengs = 0;
  لكل (بنانشان صحيح i = شروعات + 1; i < پڇاڙي; i++)
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
    إذا (info[i].khmer_category() == K_Cat(H) && انگ_coengs <= 2 && i + 1 < پڇاڙي)
    {
      انگ_coengs++;

      إذا (info[i + 1].khmer_category() == K_Cat(Ra))
      {
	لكل (بنانشان صحيح j = 0; j < 2; j++)
	  info[i + j].نقاب |= khmer_plan->نقاب_array[KHMER_PREF];

	/* Move the Coeng,Ro sequence to the start. */
	بفر->ملايو_clusters (شروعات, i + 2);
	hb_glyph_info_t t0 = info[i];
	hb_glyph_info_t t1 = info[i + 1];
	memmove (&info[شروعات + 2], &info[شروعات], (i - شروعات) * حجم (info[0]));
	info[شروعات] = t0;
	info[شروعات + 1] = t1;

	/* Mark the subsequent stuff with 'cfar'.  Used in Khmer.
	 * Read the feature spec.
	 * This allows distinguishing the following cases with MS Khmer fonts:
	 * U+1784,U+17D2,U+179A,U+17D2,U+1782
	 * U+1784,U+17D2,U+1782,U+17D2,U+179A
	 */
	إذا (khmer_plan->نقاب_array[KHMER_CFAR])
	  لكل (بنانشان صحيح j = i + 2; j < پڇاڙي; j++)
	    info[j].نقاب |= khmer_plan->نقاب_array[KHMER_CFAR];

	انگ_coengs = 2; /* Done. */
      }
    }

    /* Reorder left matra piece. */
    وإلا إذا (info[i].khmer_category() == K_Cat(VPre))
    {
      /* Move to the start. */
      بفر->ملايو_clusters (شروعات, i + 1);
      hb_glyph_info_t t = info[i];
      memmove (&info[شروعات + 1], &info[شروعات], (i - شروعات) * حجم (info[0]));
      info[شروعات] = t;
    }
  }
}

ساكن فراغ
reorder_syllable_khmer (ثابت hb_ot_shape_plan_t *plan,
			hb_face_t *face,
			hb_بفر_t *بفر,
			بنانشان صحيح شروعات, بنانشان صحيح پڇاڙي)
{
  khmer_syllable_type_t syllable_type = (khmer_syllable_type_t) (بفر->info[شروعات].syllable() & 0x0F);
  بدل (syllable_type)
  {
    حالة khmer_broken_cluster: /* We already inserted dotted-circles, so just call the consonant_syllable. */
    حالة khmer_consonant_syllable:
     reorder_consonant_syllable (plan, face, بفر, شروعات, پڇاڙي);
     اكسر;

    حالة khmer_non_khmer_cluster:
      اكسر;
  }
}

ساكن منطقي
reorder_khmer (ثابت hb_ot_shape_plan_t *plan,
	       hb_font_t *font,
	       hb_بفر_t *بفر)
{
  منطقي ret = خطأ;
  إذا (بفر->پيغام (font, "start reordering khmer"))
  {
    إذا (hb_syllabic_وجهو_dotted_circles (font, بفر,
					   khmer_broken_cluster,
					   K_Cat(DOTTEDCIRCLE),
					   (بنانشان) -1))
      ret = سچ;

    foreach_syllable (بفر, شروعات, پڇاڙي)
      reorder_syllable_khmer (plan, font->face, بفر, شروعات, پڇاڙي);
    (فراغ) بفر->پيغام (font, "end reordering khmer");
  }
  HB_بفر_DEALLOCATE_VAR (بفر, khmer_category);

  أرجع ret;
}


ساكن منطقي
decompose_khmer (ثابت hb_ot_shape_normalize_context_t *c,
		 hb_codepoint_t  ab,
		 hb_codepoint_t *a,
		 hb_codepoint_t *b)
{
  بدل (ab)
  {
    /*
     * Decompose split matras that don't have Unicode decompositions.
     */

    /* Khmer */
    حالة 0x17BEu  : *a = 0x17C1u; *b= 0x17BEu; أرجع سچ;
    حالة 0x17BFu  : *a = 0x17C1u; *b= 0x17BFu; أرجع سچ;
    حالة 0x17C0u  : *a = 0x17C1u; *b= 0x17C0u; أرجع سچ;
    حالة 0x17C4u  : *a = 0x17C1u; *b= 0x17C4u; أرجع سچ;
    حالة 0x17C5u  : *a = 0x17C1u; *b= 0x17C5u; أرجع سچ;
  }

  أرجع (منطقي) c->unicode->decompose (ab, a, b);
}

ساكن منطقي
compose_khmer (ثابت hb_ot_shape_normalize_context_t *c,
	       hb_codepoint_t  a,
	       hb_codepoint_t  b,
	       hb_codepoint_t *ab)
{
  /* Avoid recomposing split matras. */
  إذا (HB_UNICODE_GENERAL_CATEGORY_IS_MARK (c->unicode->general_category (a)))
    أرجع خطأ;

  أرجع (منطقي) c->unicode->compose (a, b, ab);
}


ثابت hb_ot_shaper_t _hb_ot_shaper_khmer =
{
  collect_features_khmer,
  override_features_khmer,
  معلومات_create_khmer,
  معلومات_destroy_khmer,
  خالي_اشارو, /* preprocess_text */
  خالي_اشارو, /* postprocess_glyphs */
  decompose_khmer,
  compose_khmer,
  تياري_masks_khmer,
  خالي_اشارو, /* reorder_marks */
  HB_ٽيگ_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_گهڻائي_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT,
  HB_OT_SHAPE_ZERO_ويڪر_MARKS_NONE,
  خطأ, /* fallback_position */
};


#جيڪڏهن_پورو

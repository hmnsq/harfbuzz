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

#اگر_تعریف_نشده HB_NO_OT_SHAPE

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
  {HB_برچسب('p','r','e','f'), F_MANUAL_JOINERS | F_در_هر_SYLLABLE},
  {HB_برچسب('b','l','w','f'), F_MANUAL_JOINERS | F_در_هر_SYLLABLE},
  {HB_برچسب('a','b','v','f'), F_MANUAL_JOINERS | F_در_هر_SYLLABLE},
  {HB_برچسب('p','s','t','f'), F_MANUAL_JOINERS | F_در_هر_SYLLABLE},
  {HB_برچسب('c','f','a','r'), F_MANUAL_JOINERS | F_در_هر_SYLLABLE},
  /*
   * Other features.
   * These features are applied all at once after clearing syllables.
   */
  {HB_برچسب('p','r','e','s'), F_GLOBAL_MANUAL_JOINERS},
  {HB_برچسب('a','b','v','s'), F_GLOBAL_MANUAL_JOINERS},
  {HB_برچسب('b','l','w','s'), F_GLOBAL_MANUAL_JOINERS},
  {HB_برچسب('p','s','t','s'), F_GLOBAL_MANUAL_JOINERS},
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

  KHMER_رقم_FEATURES,
  KHMER_BASIC_FEATURES = _KHMER_PRES, /* Don't forget to update this! */
};

ساكن مضمن فراغ
مجموعه_khmer_properties (hb_glyph_info_t &info)
{
  hb_codepoint_t u = info.codepoint;
  بیعلامت صحيح type = hb_indic_get_categories (u);

  info.khmer_category() = (khmer_category_t) (type & 0xFFu);
}

ساكن منطقي
راهاندازی_syllables_khmer (ثابت hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_میانگیر_t *میانگیر);
ساكن منطقي
reorder_khmer (ثابت hb_ot_shape_plan_t *plan,
	       hb_font_t *font,
	       hb_میانگیر_t *میانگیر);

ساكن فراغ
collect_features_khmer (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  /* Do this before any lookups have been applied. */
  map->جمع_کن_gsub_pause (راهاندازی_syllables_khmer);
  map->جمع_کن_gsub_pause (reorder_khmer);

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
  map->enable_feature (HB_برچسب('l','o','c','l'), F_در_هر_SYLLABLE);
  map->enable_feature (HB_برچسب('c','c','m','p'), F_در_هر_SYLLABLE);

  بیعلامت صحيح i = 0;
  لكل (; i < KHMER_BASIC_FEATURES; i++)
    map->جمع_کن_feature (khmer_features[i]);

  /* https://github.com/harfbuzz/harfbuzz/issues/3531 */
  map->جمع_کن_gsub_pause (hb_syllabic_clear_var); // Don't need syllables anymore, use stop to free buffer var

  لكل (; i < KHMER_رقم_FEATURES; i++)
    map->جمع_کن_feature (khmer_features[i]);
}

ساكن فراغ
override_features_khmer (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  /* Khmer spec has 'clig' as part of required shaping features:
   * "Apply feature 'clig' to form ligatures that are desired for
   * typographical correctness.", hence in overrides... */
  map->enable_feature (HB_برچسب('c','l','i','g'));

  map->disable_feature (HB_برچسب('l','i','g','a'));
}


بنية khmer_shape_plan_t
{
  hb_ماسک_t ماسک_array[KHMER_رقم_FEATURES];
};

ساكن فراغ *
داده_create_khmer (ثابت hb_ot_shape_plan_t *plan)
{
  khmer_shape_plan_t *khmer_plan = (khmer_shape_plan_t *) hb_calloc (1, حجم (khmer_shape_plan_t));
  إذا (unlikely (!khmer_plan))
    أرجع اشاره_تهی;

  لكل (بیعلامت صحيح i = 0; i < ARRAY_درازی (khmer_plan->ماسک_array); i++)
    khmer_plan->ماسک_array[i] = (khmer_features[i].flags & F_GLOBAL) ?
				 0 : plan->map.get_1_ماسک (khmer_features[i].برچسب_2);

  أرجع khmer_plan;
}

ساكن فراغ
داده_destroy_khmer (فراغ *داده)
{
  hb_free (داده);
}

ساكن فراغ
راهاندازی_masks_khmer (ثابت hb_ot_shape_plan_t *plan HB_استفادهنشده,
		   hb_میانگیر_t              *میانگیر,
		   hb_font_t                *font HB_استفادهنشده)
{
  HB_میانگیر_ALLOCATE_VAR (میانگیر, khmer_category);

  /* We cannot setup masks here.  We save information about characters
   * and setup masks later on in a pause-callback. */

  بیعلامت صحيح شمار = میانگیر->len;
  hb_glyph_info_t *info = میانگیر->info;
  لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    مجموعه_khmer_properties (info[i]);
}

ساكن منطقي
راهاندازی_syllables_khmer (ثابت hb_ot_shape_plan_t *plan HB_استفادهنشده,
		       hb_font_t *font HB_استفادهنشده,
		       hb_میانگیر_t *میانگیر)
{
  HB_میانگیر_ALLOCATE_VAR (میانگیر, syllable);
  پیدا_کن_syllables_khmer (میانگیر);
  foreach_syllable (میانگیر, شروع, انتها)
    میانگیر->unsafe_مقصد_break (شروع, انتها);
  أرجع خطأ;
}


/* Rules from:
 * https://docs.microsoft.com/en-us/typography/script-development/devanagari */

ساكن فراغ
reorder_consonant_syllable (ثابت hb_ot_shape_plan_t *plan,
			    hb_face_t *face HB_استفادهنشده,
			    hb_میانگیر_t *میانگیر,
			    بیعلامت صحيح شروع, بیعلامت صحيح انتها)
{
  ثابت khmer_shape_plan_t *khmer_plan = (ثابت khmer_shape_plan_t *) plan->داده;
  hb_glyph_info_t *info = میانگیر->info;

  /* Setup masks. */
  {
    /* Post-base */
    hb_ماسک_t ماسک = khmer_plan->ماسک_array[KHMER_BLWF] |
		     khmer_plan->ماسک_array[KHMER_ABVF] |
		     khmer_plan->ماسک_array[KHMER_PSTF];
    لكل (بیعلامت صحيح i = شروع + 1; i < انتها; i++)
      info[i].ماسک  |= ماسک;
  }

  بیعلامت صحيح رقم_coengs = 0;
  لكل (بیعلامت صحيح i = شروع + 1; i < انتها; i++)
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
    إذا (info[i].khmer_category() == K_Cat(H) && رقم_coengs <= 2 && i + 1 < انتها)
    {
      رقم_coengs++;

      إذا (info[i + 1].khmer_category() == K_Cat(Ra))
      {
	لكل (بیعلامت صحيح j = 0; j < 2; j++)
	  info[i + j].ماسک |= khmer_plan->ماسک_array[KHMER_PREF];

	/* Move the Coeng,Ro sequence to the start. */
	میانگیر->ادغام_clusters (شروع, i + 2);
	hb_glyph_info_t t0 = info[i];
	hb_glyph_info_t t1 = info[i + 1];
	memmove (&info[شروع + 2], &info[شروع], (i - شروع) * حجم (info[0]));
	info[شروع] = t0;
	info[شروع + 1] = t1;

	/* Mark the subsequent stuff with 'cfar'.  Used in Khmer.
	 * Read the feature spec.
	 * This allows distinguishing the following cases with MS Khmer fonts:
	 * U+1784,U+17D2,U+179A,U+17D2,U+1782
	 * U+1784,U+17D2,U+1782,U+17D2,U+179A
	 */
	إذا (khmer_plan->ماسک_array[KHMER_CFAR])
	  لكل (بیعلامت صحيح j = i + 2; j < انتها; j++)
	    info[j].ماسک |= khmer_plan->ماسک_array[KHMER_CFAR];

	رقم_coengs = 2; /* Done. */
      }
    }

    /* Reorder left matra piece. */
    وإلا إذا (info[i].khmer_category() == K_Cat(VPre))
    {
      /* Move to the start. */
      میانگیر->ادغام_clusters (شروع, i + 1);
      hb_glyph_info_t t = info[i];
      memmove (&info[شروع + 1], &info[شروع], (i - شروع) * حجم (info[0]));
      info[شروع] = t;
    }
  }
}

ساكن فراغ
reorder_syllable_khmer (ثابت hb_ot_shape_plan_t *plan,
			hb_face_t *face,
			hb_میانگیر_t *میانگیر,
			بیعلامت صحيح شروع, بیعلامت صحيح انتها)
{
  khmer_syllable_type_t syllable_type = (khmer_syllable_type_t) (میانگیر->info[شروع].syllable() & 0x0F);
  بدل (syllable_type)
  {
    حالة khmer_broken_cluster: /* We already inserted dotted-circles, so just call the consonant_syllable. */
    حالة khmer_consonant_syllable:
     reorder_consonant_syllable (plan, face, میانگیر, شروع, انتها);
     اكسر;

    حالة khmer_non_khmer_cluster:
      اكسر;
  }
}

ساكن منطقي
reorder_khmer (ثابت hb_ot_shape_plan_t *plan,
	       hb_font_t *font,
	       hb_میانگیر_t *میانگیر)
{
  منطقي ret = خطأ;
  إذا (میانگیر->پیام (font, "start reordering khmer"))
  {
    إذا (hb_syllabic_درج_dotted_circles (font, میانگیر,
					   khmer_broken_cluster,
					   K_Cat(DOTTEDCIRCLE),
					   (بیعلامت) -1))
      ret = درست;

    foreach_syllable (میانگیر, شروع, انتها)
      reorder_syllable_khmer (plan, font->face, میانگیر, شروع, انتها);
    (فراغ) میانگیر->پیام (font, "end reordering khmer");
  }
  HB_میانگیر_DEALLOCATE_VAR (میانگیر, khmer_category);

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
    حالة 0x17BEu  : *a = 0x17C1u; *b= 0x17BEu; أرجع درست;
    حالة 0x17BFu  : *a = 0x17C1u; *b= 0x17BFu; أرجع درست;
    حالة 0x17C0u  : *a = 0x17C1u; *b= 0x17C0u; أرجع درست;
    حالة 0x17C4u  : *a = 0x17C1u; *b= 0x17C4u; أرجع درست;
    حالة 0x17C5u  : *a = 0x17C1u; *b= 0x17C5u; أرجع درست;
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
  داده_create_khmer,
  داده_destroy_khmer,
  اشاره_تهی, /* preprocess_text */
  اشاره_تهی, /* postprocess_glyphs */
  decompose_khmer,
  compose_khmer,
  راهاندازی_masks_khmer,
  اشاره_تهی, /* reorder_marks */
  HB_برچسب_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_نما_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT,
  HB_OT_SHAPE_ZERO_عرض_MARKS_NONE,
  خطأ, /* fallback_position */
};


#پایان_اگر

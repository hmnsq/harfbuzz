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

#ضمن "hb.hh"

#اگر_تعریف_نشده HB_NO_OT_SHAPE

#ضمن "hb-ot-shaper-use-machine.hh"
#ضمن "hb-ot-shaper-use-table.hh"
#ضمن "hb-ot-shaper-arabic.hh"
#ضمن "hb-ot-shaper-arabic-joining-list.hh"
#ضمن "hb-ot-shaper-vowel-constraints.hh"


/*
 * Universal Shaping Engine.
 * https://docs.microsoft.com/en-us/typography/script-development/use
 */

ساكن ثابت hb_برچسب_t
use_basic_features[] =
{
  /*
   * Basic features.
   * These features are applied all at once, before reordering, constrained
   * to the syllable.
   */
  HB_برچسب('r','k','r','f'),
  HB_برچسب('a','b','v','f'),
  HB_برچسب('b','l','w','f'),
  HB_برچسب('h','a','l','f'),
  HB_برچسب('p','s','t','f'),
  HB_برچسب('v','a','t','u'),
  HB_برچسب('c','j','c','t'),
};
ساكن ثابت hb_برچسب_t
use_topographical_features[] =
{
  HB_برچسب('i','s','o','l'),
  HB_برچسب('i','n','i','t'),
  HB_برچسب('m','e','d','i'),
  HB_برچسب('f','i','n','a'),
};
/* Same order as use_topographical_features. */
تعداد joining_form_t {
  JOINING_FORM_ISOL,
  JOINING_FORM_INIT,
  JOINING_FORM_MEDI,
  JOINING_FORM_FINA,
  _JOINING_FORM_NONE
};
ساكن ثابت hb_برچسب_t
use_other_features[] =
{
  /*
   * Other features.
   * These features are applied all at once, after reordering and
   * clearing syllables.
   */
  HB_برچسب('a','b','v','s'),
  HB_برچسب('b','l','w','s'),
  HB_برچسب('h','a','l','n'),
  HB_برچسب('p','r','e','s'),
  HB_برچسب('p','s','t','s'),
};

ساكن منطقي
راهاندازی_syllables_use (ثابت hb_ot_shape_plan_t *plan,
		     hb_font_t *font,
		     hb_میانگیر_t *میانگیر);
ساكن منطقي
رکورد_rphf_use (ثابت hb_ot_shape_plan_t *plan,
		 hb_font_t *font,
		 hb_میانگیر_t *میانگیر);
ساكن منطقي
رکورد_pref_use (ثابت hb_ot_shape_plan_t *plan,
		 hb_font_t *font,
		 hb_میانگیر_t *میانگیر);
ساكن منطقي
reorder_use (ثابت hb_ot_shape_plan_t *plan,
	     hb_font_t *font,
	     hb_میانگیر_t *میانگیر);

ساكن فراغ
collect_features_use (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  /* Do this before any lookups have been applied. */
  map->جمع_کن_gsub_pause (راهاندازی_syllables_use);

  /* "Default glyph pre-processing group" */
  map->enable_feature (HB_برچسب('l','o','c','l'), F_در_هر_SYLLABLE);
  map->enable_feature (HB_برچسب('c','c','m','p'), F_در_هر_SYLLABLE);
  map->enable_feature (HB_برچسب('n','u','k','t'), F_در_هر_SYLLABLE);
  map->enable_feature (HB_برچسب('a','k','h','n'), F_MANUAL_ZWJ | F_در_هر_SYLLABLE);

  /* "Reordering group" */
  map->جمع_کن_gsub_pause (_hb_clear_substitution_flags);
  map->جمع_کن_feature (HB_برچسب('r','p','h','f'), F_MANUAL_ZWJ | F_در_هر_SYLLABLE);
  map->جمع_کن_gsub_pause (رکورد_rphf_use);
  map->جمع_کن_gsub_pause (_hb_clear_substitution_flags);
  map->enable_feature (HB_برچسب('p','r','e','f'), F_MANUAL_ZWJ | F_در_هر_SYLLABLE);
  map->جمع_کن_gsub_pause (رکورد_pref_use);

  /* "Orthographic unit shaping group" */
  لكل (بیعلامت صحيح i = 0; i < ARRAY_درازی (use_basic_features); i++)
    map->enable_feature (use_basic_features[i], F_MANUAL_ZWJ | F_در_هر_SYLLABLE);

  map->جمع_کن_gsub_pause (reorder_use);
  map->جمع_کن_gsub_pause (hb_syllabic_clear_var); // Don't need syllables anymore, use stop to free buffer var

  /* "Topographical features" */
  لكل (بیعلامت صحيح i = 0; i < ARRAY_درازی (use_topographical_features); i++)
    map->جمع_کن_feature (use_topographical_features[i]);
  map->جمع_کن_gsub_pause (اشاره_تهی);

  /* "Standard typographic presentation" */
  لكل (بیعلامت صحيح i = 0; i < ARRAY_درازی (use_other_features); i++)
    map->enable_feature (use_other_features[i], F_MANUAL_ZWJ);
}

بنية use_shape_plan_t
{
  hb_ماسک_t rphf_ماسک;

  arabic_shape_plan_t *arabic_plan;
};

ساكن فراغ *
داده_create_use (ثابت hb_ot_shape_plan_t *plan)
{
  use_shape_plan_t *use_plan = (use_shape_plan_t *) hb_calloc (1, حجم (use_shape_plan_t));
  إذا (unlikely (!use_plan))
    أرجع اشاره_تهی;

  use_plan->rphf_ماسک = plan->map.get_1_ماسک (HB_برچسب('r','p','h','f'));

  إذا (has_arabic_joining (plan->props.script))
  {
    use_plan->arabic_plan = (arabic_shape_plan_t *) داده_create_arabic (plan);
    إذا (unlikely (!use_plan->arabic_plan))
    {
      hb_free (use_plan);
      أرجع اشاره_تهی;
    }
  }

  أرجع use_plan;
}

ساكن فراغ
داده_destroy_use (فراغ *داده)
{
  use_shape_plan_t *use_plan = (use_shape_plan_t *) داده;

  إذا (use_plan->arabic_plan)
    داده_destroy_arabic (use_plan->arabic_plan);

  hb_free (داده);
}

ساكن فراغ
راهاندازی_masks_use (ثابت hb_ot_shape_plan_t *plan,
		 hb_میانگیر_t              *میانگیر,
		 hb_font_t                *font HB_استفادهنشده)
{
  ثابت use_shape_plan_t *use_plan = (ثابت use_shape_plan_t *) plan->داده;

  /* Do this before allocating use_category(). */
  إذا (use_plan->arabic_plan)
  {
    راهاندازی_masks_arabic_plan (use_plan->arabic_plan, میانگیر, plan->props.script);
  }

  HB_میانگیر_ALLOCATE_VAR (میانگیر, use_category);

  /* We cannot setup masks here.  We save information about characters
   * and setup masks later on in a pause-callback. */

  بیعلامت صحيح شمار = میانگیر->len;
  hb_glyph_info_t *info = میانگیر->info;
  لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    info[i].use_category() = hb_use_get_category (info[i].codepoint);
}

ساكن فراغ
راهاندازی_rphf_ماسک (ثابت hb_ot_shape_plan_t *plan,
		 hb_میانگیر_t *میانگیر)
{
  ثابت use_shape_plan_t *use_plan = (ثابت use_shape_plan_t *) plan->داده;

  hb_ماسک_t ماسک = use_plan->rphf_ماسک;
  إذا (!ماسک) أرجع;

  hb_glyph_info_t *info = میانگیر->info;

  foreach_syllable (میانگیر, شروع, انتها)
  {
    بیعلامت صحيح حد = info[شروع].use_category() == USE(R) ? 1 : hb_min (3u, انتها - شروع);
    لكل (بیعلامت صحيح i = شروع; i < شروع + حد; i++)
      info[i].ماسک |= ماسک;
  }
}

ساكن فراغ
راهاندازی_topographical_masks (ثابت hb_ot_shape_plan_t *plan,
			   hb_میانگیر_t *میانگیر)
{
  ثابت use_shape_plan_t *use_plan = (ثابت use_shape_plan_t *) plan->داده;
  إذا (use_plan->arabic_plan)
    أرجع;

  static_assert ((JOINING_FORM_INIT < 4 && JOINING_FORM_ISOL < 4 && JOINING_FORM_MEDI < 4 && JOINING_FORM_FINA < 4), "");
  hb_ماسک_t masks[4], all_masks = 0;
  لكل (بیعلامت صحيح i = 0; i < 4; i++)
  {
    masks[i] = plan->map.get_1_ماسک (use_topographical_features[i]);
    إذا (masks[i] == plan->map.get_global_ماسک ())
      masks[i] = 0;
    all_masks |= masks[i];
  }
  إذا (!all_masks)
    أرجع;
  hb_ماسک_t other_masks = ~all_masks;

  بیعلامت صحيح آخرین_شروع = 0;
  joining_form_t آخرین_form = _JOINING_FORM_NONE;
  hb_glyph_info_t *info = میانگیر->info;
  foreach_syllable (میانگیر, شروع, انتها)
  {
    use_syllable_type_t syllable_type = (use_syllable_type_t) (info[شروع].syllable() & 0x0F);
    بدل (syllable_type)
    {
      حالة use_hieroglyph_cluster:
      حالة use_non_cluster:
	/* These don't join.  Nothing to do. */
	آخرین_form = _JOINING_FORM_NONE;
	اكسر;

      حالة use_virama_terminated_cluster:
      حالة use_sakot_terminated_cluster:
      حالة use_standard_cluster:
      حالة use_عدد_joiner_terminated_cluster:
      حالة use_numeral_cluster:
      حالة use_نماد_cluster:
      حالة use_broken_cluster:

	منطقي join = آخرین_form == JOINING_FORM_FINA || آخرین_form == JOINING_FORM_ISOL;

	إذا (join)
	{
	  /* Fixup previous syllable's form. */
	  آخرین_form = آخرین_form == JOINING_FORM_FINA ? JOINING_FORM_MEDI : JOINING_FORM_INIT;
	  لكل (بیعلامت صحيح i = آخرین_شروع; i < شروع; i++)
	    info[i].ماسک = (info[i].ماسک & other_masks) | masks[آخرین_form];
	}

	/* Form for this syllable. */
	آخرین_form = join ? JOINING_FORM_FINA : JOINING_FORM_ISOL;
	لكل (بیعلامت صحيح i = شروع; i < انتها; i++)
	  info[i].ماسک = (info[i].ماسک & other_masks) | masks[آخرین_form];

	اكسر;
    }

    آخرین_شروع = شروع;
  }
}

ساكن منطقي
راهاندازی_syllables_use (ثابت hb_ot_shape_plan_t *plan,
		     hb_font_t *font HB_استفادهنشده,
		     hb_میانگیر_t *میانگیر)
{
  HB_میانگیر_ALLOCATE_VAR (میانگیر, syllable);
  پیدا_کن_syllables_use (میانگیر);
  foreach_syllable (میانگیر, شروع, انتها)
    میانگیر->unsafe_مقصد_break (شروع, انتها);
  راهاندازی_rphf_ماسک (plan, میانگیر);
  راهاندازی_topographical_masks (plan, میانگیر);
  أرجع خطأ;
}

ساكن منطقي
رکورد_rphf_use (ثابت hb_ot_shape_plan_t *plan,
		 hb_font_t *font HB_استفادهنشده,
		 hb_میانگیر_t *میانگیر)
{
  ثابت use_shape_plan_t *use_plan = (ثابت use_shape_plan_t *) plan->داده;

  hb_ماسک_t ماسک = use_plan->rphf_ماسک;
  إذا (!ماسک) أرجع خطأ;
  hb_glyph_info_t *info = میانگیر->info;

  foreach_syllable (میانگیر, شروع, انتها)
  {
    /* Mark a substituted repha as USE(R). */
    لكل (بیعلامت صحيح i = شروع; i < انتها && (info[i].ماسک & ماسک); i++)
      إذا (_hb_glyph_info_substituted (&info[i]))
      {
	info[i].use_category() = USE(R);
	اكسر;
      }
  }
  أرجع خطأ;
}

ساكن منطقي
رکورد_pref_use (ثابت hb_ot_shape_plan_t *plan HB_استفادهنشده,
		 hb_font_t *font HB_استفادهنشده,
		 hb_میانگیر_t *میانگیر)
{
  hb_glyph_info_t *info = میانگیر->info;

  foreach_syllable (میانگیر, شروع, انتها)
  {
    /* Mark a substituted pref as VPre, as they behave the same way. */
    لكل (بیعلامت صحيح i = شروع; i < انتها; i++)
      إذا (_hb_glyph_info_substituted (&info[i]))
      {
	info[i].use_category() = USE(VPre);
	اكسر;
      }
  }
  أرجع خطأ;
}

ساكن مضمن منطقي
is_halant_use (ثابت hb_glyph_info_t &info)
{
  أرجع (info.use_category() == USE(H) || info.use_category() == USE(HVM) || info.use_category() == USE(IS)) &&
	 !_hb_glyph_info_ligated (&info);
}

ساكن فراغ
reorder_syllable_use (hb_میانگیر_t *میانگیر, بیعلامت صحيح شروع, بیعلامت صحيح انتها)
{
  use_syllable_type_t syllable_type = (use_syllable_type_t) (میانگیر->info[شروع].syllable() & 0x0F);
  /* Only a few syllable types need reordering. */
  إذا (unlikely (!(FLAG_UNSAFE (syllable_type) &
		  (FLAG (use_virama_terminated_cluster) |
		   FLAG (use_sakot_terminated_cluster) |
		   FLAG (use_standard_cluster) |
		   FLAG (use_نماد_cluster) |
		   FLAG (use_broken_cluster) |
		   0))))
    أرجع;

  hb_glyph_info_t *info = میانگیر->info;

#تعریف_کن POST_پایه_FLAGS64 (FLAG64 (USE(FAbv)) | \
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
  إذا (info[شروع].use_category() == USE(R) && انتها - شروع > 1)
  {
    /* Got a repha.  Reorder it towards the end, but before the first post-base
     * glyph. */
    لكل (بیعلامت صحيح i = شروع + 1; i < انتها; i++)
    {
      منطقي is_post_پایه_glyph = (FLAG64_UNSAFE (info[i].use_category()) & POST_پایه_FLAGS64) ||
				is_halant_use (info[i]);
      إذا (is_post_پایه_glyph || i == انتها - 1)
      {
	/* If we hit a post-base glyph, move before it; otherwise move to the
	 * end. Shift things in between backward. */

	إذا (is_post_پایه_glyph)
	  i--;

	میانگیر->ادغام_clusters (شروع, i + 1);
	hb_glyph_info_t t = info[شروع];
	memmove (&info[شروع], &info[شروع + 1], (i - شروع) * حجم (info[0]));
	info[i] = t;

	اكسر;
      }
    }
  }

  /* Move things back. */
  بیعلامت صحيح j = شروع;
  لكل (بیعلامت صحيح i = شروع; i < انتها; i++)
  {
    uint32_t flag = FLAG_UNSAFE (info[i].use_category());
    إذا (is_halant_use (info[i]))
    {
      /* If we hit a halant, move after it; otherwise move to the beginning, and
       * shift things in between forward. */
      j = i + 1;
    }
    وإلا إذا (((flag) & (FLAG (USE(VPre)) | FLAG (USE(VMPre)))) &&
	     /* Only move the first component of a MultipleSubst. */
	     0 == _hb_glyph_info_get_lig_comp (&info[i]) &&
	     j < i)
    {
      میانگیر->ادغام_clusters (j, i + 1);
      hb_glyph_info_t t = info[i];
      memmove (&info[j + 1], &info[j], (i - j) * حجم (info[0]));
      info[j] = t;
    }
  }
}

ساكن منطقي
reorder_use (ثابت hb_ot_shape_plan_t *plan,
	     hb_font_t *font,
	     hb_میانگیر_t *میانگیر)
{
  منطقي ret = خطأ;
  إذا (میانگیر->پیام (font, "start reordering USE"))
  {
    إذا (hb_syllabic_درج_dotted_circles (font, میانگیر,
					   use_broken_cluster,
					   USE(B),
					   USE(R)))
      ret = درست;

    foreach_syllable (میانگیر, شروع, انتها)
      reorder_syllable_use (میانگیر, شروع, انتها);

    (فراغ) میانگیر->پیام (font, "end reordering USE");
  }

  HB_میانگیر_DEALLOCATE_VAR (میانگیر, use_category);

  أرجع ret;
}


ساكن فراغ
preprocess_متن_use (ثابت hb_ot_shape_plan_t *plan,
		     hb_میانگیر_t              *میانگیر,
		     hb_font_t                *font)
{
  hb_preprocess_متن_vowel_constraints (plan, میانگیر, font);
}

ساكن منطقي
compose_use (ثابت hb_ot_shape_normalize_context_t *c,
	     hb_codepoint_t  a,
	     hb_codepoint_t  b,
	     hb_codepoint_t *ab)
{
  /* Avoid recomposing split matras. */
  إذا (HB_UNICODE_GENERAL_CATEGORY_IS_MARK (c->unicode->general_category (a)))
    أرجع خطأ;

  أرجع (منطقي)c->unicode->compose (a, b, ab);
}


ثابت hb_ot_shaper_t _hb_ot_shaper_use =
{
  collect_features_use,
  اشاره_تهی, /* override_features */
  داده_create_use,
  داده_destroy_use,
  preprocess_متن_use,
  اشاره_تهی, /* postprocess_glyphs */
  اشاره_تهی, /* decompose */
  compose_use,
  راهاندازی_masks_use,
  اشاره_تهی, /* reorder_marks */
  HB_برچسب_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_نما_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT,
  HB_OT_SHAPE_ZERO_عرض_MARKS_BY_GDEF_EARLY,
  خطأ, /* fallback_position */
};


#پایان_اگر

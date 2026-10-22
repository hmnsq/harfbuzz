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

#شامل "hb.hh"

#اگر_غیر_تعریف_شدہ HB_NO_OT_SHAPE

#شامل "hb-ot-shaper-use-machine.hh"
#شامل "hb-ot-shaper-use-table.hh"
#شامل "hb-ot-shaper-arabic.hh"
#شامل "hb-ot-shaper-arabic-joining-list.hh"
#شامل "hb-ot-shaper-vowel-constraints.hh"


/*
 * Universal Shaping Engine.
 * https://docs.microsoft.com/en-us/typography/script-development/use
 */

ساکن ثابت hb_لیبل_t
use_basic_features[] =
{
  /*
   * Basic features.
   * These features are applied all at once, before reordering, constrained
   * to the syllable.
   */
  HB_لیبل('r','k','r','f'),
  HB_لیبل('a','b','v','f'),
  HB_لیبل('b','l','w','f'),
  HB_لیبل('h','a','l','f'),
  HB_لیبل('p','s','t','f'),
  HB_لیبل('v','a','t','u'),
  HB_لیبل('c','j','c','t'),
};
ساکن ثابت hb_لیبل_t
use_topographical_features[] =
{
  HB_لیبل('i','s','o','l'),
  HB_لیبل('i','n','i','t'),
  HB_لیبل('m','e','d','i'),
  HB_لیبل('f','i','n','a'),
};
/* Same order as use_topographical_features. */
شمار joining_form_t {
  JOINING_FORM_ISOL,
  JOINING_FORM_INIT,
  JOINING_FORM_MEDI,
  JOINING_FORM_FINA,
  _JOINING_FORM_NONE
};
ساکن ثابت hb_لیبل_t
use_other_features[] =
{
  /*
   * Other features.
   * These features are applied all at once, after reordering and
   * clearing syllables.
   */
  HB_لیبل('a','b','v','s'),
  HB_لیبل('b','l','w','s'),
  HB_لیبل('h','a','l','n'),
  HB_لیبل('p','r','e','s'),
  HB_لیبل('p','s','t','s'),
};

ساکن منطقی
تیاری_syllables_use (ثابت hb_ot_shape_plan_t *plan,
		     hb_font_t *font,
		     hb_بفر_t *بفر);
ساکن منطقی
ریکارڈ_rphf_use (ثابت hb_ot_shape_plan_t *plan,
		 hb_font_t *font,
		 hb_بفر_t *بفر);
ساکن منطقی
ریکارڈ_pref_use (ثابت hb_ot_shape_plan_t *plan,
		 hb_font_t *font,
		 hb_بفر_t *بفر);
ساکن منطقی
reorder_use (ثابت hb_ot_shape_plan_t *plan,
	     hb_font_t *font,
	     hb_بفر_t *بفر);

ساکن خالی
collect_features_use (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  /* Do this before any lookups have been applied. */
  map->جوڑو_gsub_pause (تیاری_syllables_use);

  /* "Default glyph pre-processing group" */
  map->enable_feature (HB_لیبل('l','o','c','l'), F_فی_SYLLABLE);
  map->enable_feature (HB_لیبل('c','c','m','p'), F_فی_SYLLABLE);
  map->enable_feature (HB_لیبل('n','u','k','t'), F_فی_SYLLABLE);
  map->enable_feature (HB_لیبل('a','k','h','n'), F_MANUAL_ZWJ | F_فی_SYLLABLE);

  /* "Reordering group" */
  map->جوڑو_gsub_pause (_hb_clear_substitution_flags);
  map->جوڑو_feature (HB_لیبل('r','p','h','f'), F_MANUAL_ZWJ | F_فی_SYLLABLE);
  map->جوڑو_gsub_pause (ریکارڈ_rphf_use);
  map->جوڑو_gsub_pause (_hb_clear_substitution_flags);
  map->enable_feature (HB_لیبل('p','r','e','f'), F_MANUAL_ZWJ | F_فی_SYLLABLE);
  map->جوڑو_gsub_pause (ریکارڈ_pref_use);

  /* "Orthographic unit shaping group" */
  برائے (غیرعلامتی صحیح i = 0; i < ARRAY_لمبائی (use_basic_features); i++)
    map->enable_feature (use_basic_features[i], F_MANUAL_ZWJ | F_فی_SYLLABLE);

  map->جوڑو_gsub_pause (reorder_use);
  map->جوڑو_gsub_pause (hb_syllabic_clear_var); // Don't need syllables anymore, use stop to free buffer var

  /* "Topographical features" */
  برائے (غیرعلامتی صحیح i = 0; i < ARRAY_لمبائی (use_topographical_features); i++)
    map->جوڑو_feature (use_topographical_features[i]);
  map->جوڑو_gsub_pause (خالی_اشاریہ);

  /* "Standard typographic presentation" */
  برائے (غیرعلامتی صحیح i = 0; i < ARRAY_لمبائی (use_other_features); i++)
    map->enable_feature (use_other_features[i], F_MANUAL_ZWJ);
}

ساخت use_shape_plan_t
{
  hb_نقاب_t rphf_نقاب;

  arabic_shape_plan_t *arabic_plan;
};

ساکن خالی *
ڈیٹا_create_use (ثابت hb_ot_shape_plan_t *plan)
{
  use_shape_plan_t *use_plan = (use_shape_plan_t *) hb_calloc (1, حجم (use_shape_plan_t));
  اگر (unlikely (!use_plan))
    واپس خالی_اشاریہ;

  use_plan->rphf_نقاب = plan->map.get_1_نقاب (HB_لیبل('r','p','h','f'));

  اگر (has_arabic_joining (plan->props.script))
  {
    use_plan->arabic_plan = (arabic_shape_plan_t *) ڈیٹا_create_arabic (plan);
    اگر (unlikely (!use_plan->arabic_plan))
    {
      hb_free (use_plan);
      واپس خالی_اشاریہ;
    }
  }

  واپس use_plan;
}

ساکن خالی
ڈیٹا_destroy_use (خالی *ڈیٹا)
{
  use_shape_plan_t *use_plan = (use_shape_plan_t *) ڈیٹا;

  اگر (use_plan->arabic_plan)
    ڈیٹا_destroy_arabic (use_plan->arabic_plan);

  hb_free (ڈیٹا);
}

ساکن خالی
تیاری_masks_use (ثابت hb_ot_shape_plan_t *plan,
		 hb_بفر_t              *بفر,
		 hb_font_t                *font HB_غیر_مستعمل)
{
  ثابت use_shape_plan_t *use_plan = (ثابت use_shape_plan_t *) plan->ڈیٹا;

  /* Do this before allocating use_category(). */
  اگر (use_plan->arabic_plan)
  {
    تیاری_masks_arabic_plan (use_plan->arabic_plan, بفر, plan->props.script);
  }

  HB_بفر_ALLOCATE_VAR (بفر, use_category);

  /* We cannot setup masks here.  We save information about characters
   * and setup masks later on in a pause-callback. */

  غیرعلامتی صحیح گنتی = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    info[i].use_category() = hb_use_get_category (info[i].codepoint);
}

ساکن خالی
تیاری_rphf_نقاب (ثابت hb_ot_shape_plan_t *plan,
		 hb_بفر_t *بفر)
{
  ثابت use_shape_plan_t *use_plan = (ثابت use_shape_plan_t *) plan->ڈیٹا;

  hb_نقاب_t نقاب = use_plan->rphf_نقاب;
  اگر (!نقاب) واپس;

  hb_glyph_info_t *info = بفر->info;

  foreach_syllable (بفر, آغاز, انتہا)
  {
    غیرعلامتی صحیح حد_بندی = info[آغاز].use_category() == USE(R) ? 1 : hb_min (3u, انتہا - آغاز);
    برائے (غیرعلامتی صحیح i = آغاز; i < آغاز + حد_بندی; i++)
      info[i].نقاب |= نقاب;
  }
}

ساکن خالی
تیاری_topographical_masks (ثابت hb_ot_shape_plan_t *plan,
			   hb_بفر_t *بفر)
{
  ثابت use_shape_plan_t *use_plan = (ثابت use_shape_plan_t *) plan->ڈیٹا;
  اگر (use_plan->arabic_plan)
    واپس;

  static_assert ((JOINING_FORM_INIT < 4 && JOINING_FORM_ISOL < 4 && JOINING_FORM_MEDI < 4 && JOINING_FORM_FINA < 4), "");
  hb_نقاب_t masks[4], all_masks = 0;
  برائے (غیرعلامتی صحیح i = 0; i < 4; i++)
  {
    masks[i] = plan->map.get_1_نقاب (use_topographical_features[i]);
    اگر (masks[i] == plan->map.get_global_نقاب ())
      masks[i] = 0;
    all_masks |= masks[i];
  }
  اگر (!all_masks)
    واپس;
  hb_نقاب_t other_masks = ~all_masks;

  غیرعلامتی صحیح آخری_آغاز = 0;
  joining_form_t آخری_form = _JOINING_FORM_NONE;
  hb_glyph_info_t *info = بفر->info;
  foreach_syllable (بفر, آغاز, انتہا)
  {
    use_syllable_type_t syllable_type = (use_syllable_type_t) (info[آغاز].syllable() & 0x0F);
    انتخاب (syllable_type)
    {
      حالت use_hieroglyph_cluster:
      حالت use_non_cluster:
	/* These don't join.  Nothing to do. */
	آخری_form = _JOINING_FORM_NONE;
	توڑو;

      حالت use_virama_terminated_cluster:
      حالت use_sakot_terminated_cluster:
      حالت use_standard_cluster:
      حالت use_نمبر_joiner_terminated_cluster:
      حالت use_numeral_cluster:
      حالت use_علامت_cluster:
      حالت use_broken_cluster:

	منطقی join = آخری_form == JOINING_FORM_FINA || آخری_form == JOINING_FORM_ISOL;

	اگر (join)
	{
	  /* Fixup previous syllable's form. */
	  آخری_form = آخری_form == JOINING_FORM_FINA ? JOINING_FORM_MEDI : JOINING_FORM_INIT;
	  برائے (غیرعلامتی صحیح i = آخری_آغاز; i < آغاز; i++)
	    info[i].نقاب = (info[i].نقاب & other_masks) | masks[آخری_form];
	}

	/* Form for this syllable. */
	آخری_form = join ? JOINING_FORM_FINA : JOINING_FORM_ISOL;
	برائے (غیرعلامتی صحیح i = آغاز; i < انتہا; i++)
	  info[i].نقاب = (info[i].نقاب & other_masks) | masks[آخری_form];

	توڑو;
    }

    آخری_آغاز = آغاز;
  }
}

ساکن منطقی
تیاری_syllables_use (ثابت hb_ot_shape_plan_t *plan,
		     hb_font_t *font HB_غیر_مستعمل,
		     hb_بفر_t *بفر)
{
  HB_بفر_ALLOCATE_VAR (بفر, syllable);
  ڈھونڈو_syllables_use (بفر);
  foreach_syllable (بفر, آغاز, انتہا)
    بفر->unsafe_مقصد_break (آغاز, انتہا);
  تیاری_rphf_نقاب (plan, بفر);
  تیاری_topographical_masks (plan, بفر);
  واپس جھوٹ;
}

ساکن منطقی
ریکارڈ_rphf_use (ثابت hb_ot_shape_plan_t *plan,
		 hb_font_t *font HB_غیر_مستعمل,
		 hb_بفر_t *بفر)
{
  ثابت use_shape_plan_t *use_plan = (ثابت use_shape_plan_t *) plan->ڈیٹا;

  hb_نقاب_t نقاب = use_plan->rphf_نقاب;
  اگر (!نقاب) واپس جھوٹ;
  hb_glyph_info_t *info = بفر->info;

  foreach_syllable (بفر, آغاز, انتہا)
  {
    /* Mark a substituted repha as USE(R). */
    برائے (غیرعلامتی صحیح i = آغاز; i < انتہا && (info[i].نقاب & نقاب); i++)
      اگر (_hb_glyph_info_substituted (&info[i]))
      {
	info[i].use_category() = USE(R);
	توڑو;
      }
  }
  واپس جھوٹ;
}

ساکن منطقی
ریکارڈ_pref_use (ثابت hb_ot_shape_plan_t *plan HB_غیر_مستعمل,
		 hb_font_t *font HB_غیر_مستعمل,
		 hb_بفر_t *بفر)
{
  hb_glyph_info_t *info = بفر->info;

  foreach_syllable (بفر, آغاز, انتہا)
  {
    /* Mark a substituted pref as VPre, as they behave the same way. */
    برائے (غیرعلامتی صحیح i = آغاز; i < انتہا; i++)
      اگر (_hb_glyph_info_substituted (&info[i]))
      {
	info[i].use_category() = USE(VPre);
	توڑو;
      }
  }
  واپس جھوٹ;
}

ساکن اندرونی منطقی
is_halant_use (ثابت hb_glyph_info_t &info)
{
  واپس (info.use_category() == USE(H) || info.use_category() == USE(HVM) || info.use_category() == USE(IS)) &&
	 !_hb_glyph_info_ligated (&info);
}

ساکن خالی
reorder_syllable_use (hb_بفر_t *بفر, غیرعلامتی صحیح آغاز, غیرعلامتی صحیح انتہا)
{
  use_syllable_type_t syllable_type = (use_syllable_type_t) (بفر->info[آغاز].syllable() & 0x0F);
  /* Only a few syllable types need reordering. */
  اگر (unlikely (!(FLAG_UNSAFE (syllable_type) &
		  (FLAG (use_virama_terminated_cluster) |
		   FLAG (use_sakot_terminated_cluster) |
		   FLAG (use_standard_cluster) |
		   FLAG (use_علامت_cluster) |
		   FLAG (use_broken_cluster) |
		   0))))
    واپس;

  hb_glyph_info_t *info = بفر->info;

#تعریف_کرو POST_بنیاد_FLAGS64 (FLAG64 (USE(FAbv)) | \
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
  اگر (info[آغاز].use_category() == USE(R) && انتہا - آغاز > 1)
  {
    /* Got a repha.  Reorder it towards the end, but before the first post-base
     * glyph. */
    برائے (غیرعلامتی صحیح i = آغاز + 1; i < انتہا; i++)
    {
      منطقی is_post_بنیاد_glyph = (FLAG64_UNSAFE (info[i].use_category()) & POST_بنیاد_FLAGS64) ||
				is_halant_use (info[i]);
      اگر (is_post_بنیاد_glyph || i == انتہا - 1)
      {
	/* If we hit a post-base glyph, move before it; otherwise move to the
	 * end. Shift things in between backward. */

	اگر (is_post_بنیاد_glyph)
	  i--;

	بفر->ملاؤ_clusters (آغاز, i + 1);
	hb_glyph_info_t t = info[آغاز];
	memmove (&info[آغاز], &info[آغاز + 1], (i - آغاز) * حجم (info[0]));
	info[i] = t;

	توڑو;
      }
    }
  }

  /* Move things back. */
  غیرعلامتی صحیح j = آغاز;
  برائے (غیرعلامتی صحیح i = آغاز; i < انتہا; i++)
  {
    uint32_t flag = FLAG_UNSAFE (info[i].use_category());
    اگر (is_halant_use (info[i]))
    {
      /* If we hit a halant, move after it; otherwise move to the beginning, and
       * shift things in between forward. */
      j = i + 1;
    }
    ورنہ اگر (((flag) & (FLAG (USE(VPre)) | FLAG (USE(VMPre)))) &&
	     /* Only move the first component of a MultipleSubst. */
	     0 == _hb_glyph_info_get_lig_comp (&info[i]) &&
	     j < i)
    {
      بفر->ملاؤ_clusters (j, i + 1);
      hb_glyph_info_t t = info[i];
      memmove (&info[j + 1], &info[j], (i - j) * حجم (info[0]));
      info[j] = t;
    }
  }
}

ساکن منطقی
reorder_use (ثابت hb_ot_shape_plan_t *plan,
	     hb_font_t *font,
	     hb_بفر_t *بفر)
{
  منطقی ret = جھوٹ;
  اگر (بفر->پیغام (font, "start reordering USE"))
  {
    اگر (hb_syllabic_داخل_کرو_dotted_circles (font, بفر,
					   use_broken_cluster,
					   USE(B),
					   USE(R)))
      ret = سچ;

    foreach_syllable (بفر, آغاز, انتہا)
      reorder_syllable_use (بفر, آغاز, انتہا);

    (خالی) بفر->پیغام (font, "end reordering USE");
  }

  HB_بفر_DEALLOCATE_VAR (بفر, use_category);

  واپس ret;
}


ساکن خالی
preprocess_متن_use (ثابت hb_ot_shape_plan_t *plan,
		     hb_بفر_t              *بفر,
		     hb_font_t                *font)
{
  hb_preprocess_متن_vowel_constraints (plan, بفر, font);
}

ساکن منطقی
compose_use (ثابت hb_ot_shape_normalize_context_t *c,
	     hb_codepoint_t  a,
	     hb_codepoint_t  b,
	     hb_codepoint_t *ab)
{
  /* Avoid recomposing split matras. */
  اگر (HB_UNICODE_GENERAL_CATEGORY_IS_MARK (c->unicode->general_category (a)))
    واپس جھوٹ;

  واپس (منطقی)c->unicode->compose (a, b, ab);
}


ثابت hb_ot_shaper_t _hb_ot_shaper_use =
{
  collect_features_use,
  خالی_اشاریہ, /* override_features */
  ڈیٹا_create_use,
  ڈیٹا_destroy_use,
  preprocess_متن_use,
  خالی_اشاریہ, /* postprocess_glyphs */
  خالی_اشاریہ, /* decompose */
  compose_use,
  تیاری_masks_use,
  خالی_اشاریہ, /* reorder_marks */
  HB_لیبل_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_منوال_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT,
  HB_OT_SHAPE_ZERO_چوڑائی_MARKS_BY_GDEF_EARLY,
  جھوٹ, /* fallback_position */
};


#اگر_ختم

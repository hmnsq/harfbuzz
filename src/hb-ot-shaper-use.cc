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

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_SHAPE

#ضمن "hb-ot-shaper-use-machine.hh"
#ضمن "hb-ot-shaper-use-table.hh"
#ضمن "hb-ot-shaper-arabic.hh"
#ضمن "hb-ot-shaper-arabic-joining-list.hh"
#ضمن "hb-ot-shaper-vowel-constraints.hh"


/*
 * Universal Shaping Engine.
 * https://docs.microsoft.com/en-us/typography/script-development/use
 */

ساكن ثابت hb_ٽيگ_t
use_basic_features[] =
{
  /*
   * Basic features.
   * These features are applied all at once, before reordering, constrained
   * to the syllable.
   */
  HB_ٽيگ('r','k','r','f'),
  HB_ٽيگ('a','b','v','f'),
  HB_ٽيگ('b','l','w','f'),
  HB_ٽيگ('h','a','l','f'),
  HB_ٽيگ('p','s','t','f'),
  HB_ٽيگ('v','a','t','u'),
  HB_ٽيگ('c','j','c','t'),
};
ساكن ثابت hb_ٽيگ_t
use_topographical_features[] =
{
  HB_ٽيگ('i','s','o','l'),
  HB_ٽيگ('i','n','i','t'),
  HB_ٽيگ('m','e','d','i'),
  HB_ٽيگ('f','i','n','a'),
};
/* Same order as use_topographical_features. */
تعداد joining_form_t {
  JOINING_FORM_ISOL,
  JOINING_FORM_INIT,
  JOINING_FORM_MEDI,
  JOINING_FORM_FINA,
  _JOINING_FORM_NONE
};
ساكن ثابت hb_ٽيگ_t
use_other_features[] =
{
  /*
   * Other features.
   * These features are applied all at once, after reordering and
   * clearing syllables.
   */
  HB_ٽيگ('a','b','v','s'),
  HB_ٽيگ('b','l','w','s'),
  HB_ٽيگ('h','a','l','n'),
  HB_ٽيگ('p','r','e','s'),
  HB_ٽيگ('p','s','t','s'),
};

ساكن منطقي
تياري_syllables_use (ثابت hb_ot_shape_plan_t *plan,
		     hb_font_t *font,
		     hb_بفر_t *بفر);
ساكن منطقي
رڪارڊ_rphf_use (ثابت hb_ot_shape_plan_t *plan,
		 hb_font_t *font,
		 hb_بفر_t *بفر);
ساكن منطقي
رڪارڊ_pref_use (ثابت hb_ot_shape_plan_t *plan,
		 hb_font_t *font,
		 hb_بفر_t *بفر);
ساكن منطقي
reorder_use (ثابت hb_ot_shape_plan_t *plan,
	     hb_font_t *font,
	     hb_بفر_t *بفر);

ساكن فراغ
collect_features_use (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  /* Do this before any lookups have been applied. */
  map->جوڙيو_gsub_pause (تياري_syllables_use);

  /* "Default glyph pre-processing group" */
  map->enable_feature (HB_ٽيگ('l','o','c','l'), F_في_SYLLABLE);
  map->enable_feature (HB_ٽيگ('c','c','m','p'), F_في_SYLLABLE);
  map->enable_feature (HB_ٽيگ('n','u','k','t'), F_في_SYLLABLE);
  map->enable_feature (HB_ٽيگ('a','k','h','n'), F_MANUAL_ZWJ | F_في_SYLLABLE);

  /* "Reordering group" */
  map->جوڙيو_gsub_pause (_hb_clear_substitution_flags);
  map->جوڙيو_feature (HB_ٽيگ('r','p','h','f'), F_MANUAL_ZWJ | F_في_SYLLABLE);
  map->جوڙيو_gsub_pause (رڪارڊ_rphf_use);
  map->جوڙيو_gsub_pause (_hb_clear_substitution_flags);
  map->enable_feature (HB_ٽيگ('p','r','e','f'), F_MANUAL_ZWJ | F_في_SYLLABLE);
  map->جوڙيو_gsub_pause (رڪارڊ_pref_use);

  /* "Orthographic unit shaping group" */
  لكل (بنانشان صحيح i = 0; i < ARRAY_ڊيگهائي (use_basic_features); i++)
    map->enable_feature (use_basic_features[i], F_MANUAL_ZWJ | F_في_SYLLABLE);

  map->جوڙيو_gsub_pause (reorder_use);
  map->جوڙيو_gsub_pause (hb_syllabic_clear_var); // Don't need syllables anymore, use stop to free buffer var

  /* "Topographical features" */
  لكل (بنانشان صحيح i = 0; i < ARRAY_ڊيگهائي (use_topographical_features); i++)
    map->جوڙيو_feature (use_topographical_features[i]);
  map->جوڙيو_gsub_pause (خالي_اشارو);

  /* "Standard typographic presentation" */
  لكل (بنانشان صحيح i = 0; i < ARRAY_ڊيگهائي (use_other_features); i++)
    map->enable_feature (use_other_features[i], F_MANUAL_ZWJ);
}

بنية use_shape_plan_t
{
  hb_نقاب_t rphf_نقاب;

  arabic_shape_plan_t *arabic_plan;
};

ساكن فراغ *
معلومات_create_use (ثابت hb_ot_shape_plan_t *plan)
{
  use_shape_plan_t *use_plan = (use_shape_plan_t *) hb_calloc (1, حجم (use_shape_plan_t));
  إذا (unlikely (!use_plan))
    أرجع خالي_اشارو;

  use_plan->rphf_نقاب = plan->map.get_1_نقاب (HB_ٽيگ('r','p','h','f'));

  إذا (has_arabic_joining (plan->props.script))
  {
    use_plan->arabic_plan = (arabic_shape_plan_t *) معلومات_create_arabic (plan);
    إذا (unlikely (!use_plan->arabic_plan))
    {
      hb_free (use_plan);
      أرجع خالي_اشارو;
    }
  }

  أرجع use_plan;
}

ساكن فراغ
معلومات_destroy_use (فراغ *معلومات)
{
  use_shape_plan_t *use_plan = (use_shape_plan_t *) معلومات;

  إذا (use_plan->arabic_plan)
    معلومات_destroy_arabic (use_plan->arabic_plan);

  hb_free (معلومات);
}

ساكن فراغ
تياري_masks_use (ثابت hb_ot_shape_plan_t *plan,
		 hb_بفر_t              *بفر,
		 hb_font_t                *font HB_اڻ_استعمال_ٿيل)
{
  ثابت use_shape_plan_t *use_plan = (ثابت use_shape_plan_t *) plan->معلومات;

  /* Do this before allocating use_category(). */
  إذا (use_plan->arabic_plan)
  {
    تياري_masks_arabic_plan (use_plan->arabic_plan, بفر, plan->props.script);
  }

  HB_بفر_ALLOCATE_VAR (بفر, use_category);

  /* We cannot setup masks here.  We save information about characters
   * and setup masks later on in a pause-callback. */

  بنانشان صحيح ڳڻپ = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    info[i].use_category() = hb_use_get_category (info[i].codepoint);
}

ساكن فراغ
تياري_rphf_نقاب (ثابت hb_ot_shape_plan_t *plan,
		 hb_بفر_t *بفر)
{
  ثابت use_shape_plan_t *use_plan = (ثابت use_shape_plan_t *) plan->معلومات;

  hb_نقاب_t نقاب = use_plan->rphf_نقاب;
  إذا (!نقاب) أرجع;

  hb_glyph_info_t *info = بفر->info;

  foreach_syllable (بفر, شروعات, پڇاڙي)
  {
    بنانشان صحيح حد_بندي = info[شروعات].use_category() == USE(R) ? 1 : hb_min (3u, پڇاڙي - شروعات);
    لكل (بنانشان صحيح i = شروعات; i < شروعات + حد_بندي; i++)
      info[i].نقاب |= نقاب;
  }
}

ساكن فراغ
تياري_topographical_masks (ثابت hb_ot_shape_plan_t *plan,
			   hb_بفر_t *بفر)
{
  ثابت use_shape_plan_t *use_plan = (ثابت use_shape_plan_t *) plan->معلومات;
  إذا (use_plan->arabic_plan)
    أرجع;

  static_assert ((JOINING_FORM_INIT < 4 && JOINING_FORM_ISOL < 4 && JOINING_FORM_MEDI < 4 && JOINING_FORM_FINA < 4), "");
  hb_نقاب_t masks[4], all_masks = 0;
  لكل (بنانشان صحيح i = 0; i < 4; i++)
  {
    masks[i] = plan->map.get_1_نقاب (use_topographical_features[i]);
    إذا (masks[i] == plan->map.get_global_نقاب ())
      masks[i] = 0;
    all_masks |= masks[i];
  }
  إذا (!all_masks)
    أرجع;
  hb_نقاب_t other_masks = ~all_masks;

  بنانشان صحيح آخري_شروعات = 0;
  joining_form_t آخري_form = _JOINING_FORM_NONE;
  hb_glyph_info_t *info = بفر->info;
  foreach_syllable (بفر, شروعات, پڇاڙي)
  {
    use_syllable_type_t syllable_type = (use_syllable_type_t) (info[شروعات].syllable() & 0x0F);
    بدل (syllable_type)
    {
      حالة use_hieroglyph_cluster:
      حالة use_non_cluster:
	/* These don't join.  Nothing to do. */
	آخري_form = _JOINING_FORM_NONE;
	اكسر;

      حالة use_virama_terminated_cluster:
      حالة use_sakot_terminated_cluster:
      حالة use_standard_cluster:
      حالة use_عدد_joiner_terminated_cluster:
      حالة use_numeral_cluster:
      حالة use_نشان_cluster:
      حالة use_broken_cluster:

	منطقي join = آخري_form == JOINING_FORM_FINA || آخري_form == JOINING_FORM_ISOL;

	إذا (join)
	{
	  /* Fixup previous syllable's form. */
	  آخري_form = آخري_form == JOINING_FORM_FINA ? JOINING_FORM_MEDI : JOINING_FORM_INIT;
	  لكل (بنانشان صحيح i = آخري_شروعات; i < شروعات; i++)
	    info[i].نقاب = (info[i].نقاب & other_masks) | masks[آخري_form];
	}

	/* Form for this syllable. */
	آخري_form = join ? JOINING_FORM_FINA : JOINING_FORM_ISOL;
	لكل (بنانشان صحيح i = شروعات; i < پڇاڙي; i++)
	  info[i].نقاب = (info[i].نقاب & other_masks) | masks[آخري_form];

	اكسر;
    }

    آخري_شروعات = شروعات;
  }
}

ساكن منطقي
تياري_syllables_use (ثابت hb_ot_shape_plan_t *plan,
		     hb_font_t *font HB_اڻ_استعمال_ٿيل,
		     hb_بفر_t *بفر)
{
  HB_بفر_ALLOCATE_VAR (بفر, syllable);
  ڳوليو_syllables_use (بفر);
  foreach_syllable (بفر, شروعات, پڇاڙي)
    بفر->unsafe_منزل_break (شروعات, پڇاڙي);
  تياري_rphf_نقاب (plan, بفر);
  تياري_topographical_masks (plan, بفر);
  أرجع خطأ;
}

ساكن منطقي
رڪارڊ_rphf_use (ثابت hb_ot_shape_plan_t *plan,
		 hb_font_t *font HB_اڻ_استعمال_ٿيل,
		 hb_بفر_t *بفر)
{
  ثابت use_shape_plan_t *use_plan = (ثابت use_shape_plan_t *) plan->معلومات;

  hb_نقاب_t نقاب = use_plan->rphf_نقاب;
  إذا (!نقاب) أرجع خطأ;
  hb_glyph_info_t *info = بفر->info;

  foreach_syllable (بفر, شروعات, پڇاڙي)
  {
    /* Mark a substituted repha as USE(R). */
    لكل (بنانشان صحيح i = شروعات; i < پڇاڙي && (info[i].نقاب & نقاب); i++)
      إذا (_hb_glyph_info_substituted (&info[i]))
      {
	info[i].use_category() = USE(R);
	اكسر;
      }
  }
  أرجع خطأ;
}

ساكن منطقي
رڪارڊ_pref_use (ثابت hb_ot_shape_plan_t *plan HB_اڻ_استعمال_ٿيل,
		 hb_font_t *font HB_اڻ_استعمال_ٿيل,
		 hb_بفر_t *بفر)
{
  hb_glyph_info_t *info = بفر->info;

  foreach_syllable (بفر, شروعات, پڇاڙي)
  {
    /* Mark a substituted pref as VPre, as they behave the same way. */
    لكل (بنانشان صحيح i = شروعات; i < پڇاڙي; i++)
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
reorder_syllable_use (hb_بفر_t *بفر, بنانشان صحيح شروعات, بنانشان صحيح پڇاڙي)
{
  use_syllable_type_t syllable_type = (use_syllable_type_t) (بفر->info[شروعات].syllable() & 0x0F);
  /* Only a few syllable types need reordering. */
  إذا (unlikely (!(FLAG_UNSAFE (syllable_type) &
		  (FLAG (use_virama_terminated_cluster) |
		   FLAG (use_sakot_terminated_cluster) |
		   FLAG (use_standard_cluster) |
		   FLAG (use_نشان_cluster) |
		   FLAG (use_broken_cluster) |
		   0))))
    أرجع;

  hb_glyph_info_t *info = بفر->info;

#وصف_ڪريو POST_بنياد_FLAGS64 (FLAG64 (USE(FAbv)) | \
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
  إذا (info[شروعات].use_category() == USE(R) && پڇاڙي - شروعات > 1)
  {
    /* Got a repha.  Reorder it towards the end, but before the first post-base
     * glyph. */
    لكل (بنانشان صحيح i = شروعات + 1; i < پڇاڙي; i++)
    {
      منطقي is_post_بنياد_glyph = (FLAG64_UNSAFE (info[i].use_category()) & POST_بنياد_FLAGS64) ||
				is_halant_use (info[i]);
      إذا (is_post_بنياد_glyph || i == پڇاڙي - 1)
      {
	/* If we hit a post-base glyph, move before it; otherwise move to the
	 * end. Shift things in between backward. */

	إذا (is_post_بنياد_glyph)
	  i--;

	بفر->ملايو_clusters (شروعات, i + 1);
	hb_glyph_info_t t = info[شروعات];
	memmove (&info[شروعات], &info[شروعات + 1], (i - شروعات) * حجم (info[0]));
	info[i] = t;

	اكسر;
      }
    }
  }

  /* Move things back. */
  بنانشان صحيح j = شروعات;
  لكل (بنانشان صحيح i = شروعات; i < پڇاڙي; i++)
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
      بفر->ملايو_clusters (j, i + 1);
      hb_glyph_info_t t = info[i];
      memmove (&info[j + 1], &info[j], (i - j) * حجم (info[0]));
      info[j] = t;
    }
  }
}

ساكن منطقي
reorder_use (ثابت hb_ot_shape_plan_t *plan,
	     hb_font_t *font,
	     hb_بفر_t *بفر)
{
  منطقي ret = خطأ;
  إذا (بفر->پيغام (font, "start reordering USE"))
  {
    إذا (hb_syllabic_وجهو_dotted_circles (font, بفر,
					   use_broken_cluster,
					   USE(B),
					   USE(R)))
      ret = سچ;

    foreach_syllable (بفر, شروعات, پڇاڙي)
      reorder_syllable_use (بفر, شروعات, پڇاڙي);

    (فراغ) بفر->پيغام (font, "end reordering USE");
  }

  HB_بفر_DEALLOCATE_VAR (بفر, use_category);

  أرجع ret;
}


ساكن فراغ
preprocess_متن_use (ثابت hb_ot_shape_plan_t *plan,
		     hb_بفر_t              *بفر,
		     hb_font_t                *font)
{
  hb_preprocess_متن_vowel_constraints (plan, بفر, font);
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
  خالي_اشارو, /* override_features */
  معلومات_create_use,
  معلومات_destroy_use,
  preprocess_متن_use,
  خالي_اشارو, /* postprocess_glyphs */
  خالي_اشارو, /* decompose */
  compose_use,
  تياري_masks_use,
  خالي_اشارو, /* reorder_marks */
  HB_ٽيگ_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_گهڻائي_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT,
  HB_OT_SHAPE_ZERO_ويڪر_MARKS_BY_GDEF_EARLY,
  خطأ, /* fallback_position */
};


#جيڪڏهن_پورو

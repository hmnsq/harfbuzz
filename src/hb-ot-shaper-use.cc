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

#অন্তর্ভাব "hb.hh"

#ifndef HB_NO_OT_SHAPE

#অন্তর্ভাব "hb-ot-shaper-use-machine.hh"
#অন্তর্ভাব "hb-ot-shaper-use-table.hh"
#অন্তর্ভাব "hb-ot-shaper-arabic.hh"
#অন্তর্ভাব "hb-ot-shaper-arabic-joining-list.hh"
#অন্তর্ভাব "hb-ot-shaper-vowel-constraints.hh"


/*
 * Universal Shaping Engine.
 * https://docs.microsoft.com/en-us/typography/script-development/use
 */

স্থির ধ্রুবক hb_tag_t
use_basic_features[] =
{
  /*
   * Basic features.
   * These features are applied all at once, before reordering, constrained
   * to the syllable.
   */
  HB_TAG('r','k','r','f'),
  HB_TAG('a','b','v','f'),
  HB_TAG('b','l','w','f'),
  HB_TAG('h','a','l','f'),
  HB_TAG('p','s','t','f'),
  HB_TAG('v','a','t','u'),
  HB_TAG('c','j','c','t'),
};
স্থির ধ্রুবক hb_tag_t
use_topographical_features[] =
{
  HB_TAG('i','s','o','l'),
  HB_TAG('i','n','i','t'),
  HB_TAG('m','e','d','i'),
  HB_TAG('f','i','n','a'),
};
/* Same order as use_topographical_features. */
গণনা joining_form_t {
  JOINING_FORM_ISOL,
  JOINING_FORM_INIT,
  JOINING_FORM_MEDI,
  JOINING_FORM_FINA,
  _JOINING_FORM_NONE
};
স্থির ধ্রুবক hb_tag_t
use_other_features[] =
{
  /*
   * Other features.
   * These features are applied all at once, after reordering and
   * clearing syllables.
   */
  HB_TAG('a','b','v','s'),
  HB_TAG('b','l','w','s'),
  HB_TAG('h','a','l','n'),
  HB_TAG('p','r','e','s'),
  HB_TAG('p','s','t','s'),
};

স্থির যৌক্তিক
setup_syllables_use (ধ্রুবক hb_ot_shape_plan_t *plan,
		     hb_font_t *font,
		     hb_বাফার_t *বাফার);
স্থির যৌক্তিক
record_rphf_use (ধ্রুবক hb_ot_shape_plan_t *plan,
		 hb_font_t *font,
		 hb_বাফার_t *বাফার);
স্থির যৌক্তিক
record_pref_use (ধ্রুবক hb_ot_shape_plan_t *plan,
		 hb_font_t *font,
		 hb_বাফার_t *বাফার);
স্থির যৌক্তিক
reorder_use (ধ্রুবক hb_ot_shape_plan_t *plan,
	     hb_font_t *font,
	     hb_বাফার_t *বাফার);

স্থির শূন্য
collect_features_use (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  /* Do this before any lookups have been applied. */
  map->add_gsub_pause (setup_syllables_use);

  /* "Default glyph pre-processing group" */
  map->enable_feature (HB_TAG('l','o','c','l'), F_PER_SYLLABLE);
  map->enable_feature (HB_TAG('c','c','m','p'), F_PER_SYLLABLE);
  map->enable_feature (HB_TAG('n','u','k','t'), F_PER_SYLLABLE);
  map->enable_feature (HB_TAG('a','k','h','n'), F_MANUAL_ZWJ | F_PER_SYLLABLE);

  /* "Reordering group" */
  map->add_gsub_pause (_hb_clear_substitution_flags);
  map->add_feature (HB_TAG('r','p','h','f'), F_MANUAL_ZWJ | F_PER_SYLLABLE);
  map->add_gsub_pause (record_rphf_use);
  map->add_gsub_pause (_hb_clear_substitution_flags);
  map->enable_feature (HB_TAG('p','r','e','f'), F_MANUAL_ZWJ | F_PER_SYLLABLE);
  map->add_gsub_pause (record_pref_use);

  /* "Orthographic unit shaping group" */
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < ARRAY_দৈর্ঘ্য (use_basic_features); i++)
    map->enable_feature (use_basic_features[i], F_MANUAL_ZWJ | F_PER_SYLLABLE);

  map->add_gsub_pause (reorder_use);
  map->add_gsub_pause (hb_syllabic_clear_var); // Don't need syllables anymore, use stop to free buffer var

  /* "Topographical features" */
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < ARRAY_দৈর্ঘ্য (use_topographical_features); i++)
    map->add_feature (use_topographical_features[i]);
  map->add_gsub_pause (nullptr);

  /* "Standard typographic presentation" */
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < ARRAY_দৈর্ঘ্য (use_other_features); i++)
    map->enable_feature (use_other_features[i], F_MANUAL_ZWJ);
}

কাঠামো use_shape_plan_t
{
  hb_mask_t rphf_mask;

  arabic_shape_plan_t *arabic_plan;
};

স্থির শূন্য *
তথ্য_create_use (ধ্রুবক hb_ot_shape_plan_t *plan)
{
  use_shape_plan_t *use_plan = (use_shape_plan_t *) hb_calloc (1, আকার (use_shape_plan_t));
  যদি (unlikely (!use_plan))
    ফেরত nullptr;

  use_plan->rphf_mask = plan->map.get_1_mask (HB_TAG('r','p','h','f'));

  যদি (has_arabic_joining (plan->props.script))
  {
    use_plan->arabic_plan = (arabic_shape_plan_t *) তথ্য_create_arabic (plan);
    যদি (unlikely (!use_plan->arabic_plan))
    {
      hb_free (use_plan);
      ফেরত nullptr;
    }
  }

  ফেরত use_plan;
}

স্থির শূন্য
তথ্য_destroy_use (শূন্য *তথ্য)
{
  use_shape_plan_t *use_plan = (use_shape_plan_t *) তথ্য;

  যদি (use_plan->arabic_plan)
    তথ্য_destroy_arabic (use_plan->arabic_plan);

  hb_free (তথ্য);
}

স্থির শূন্য
setup_masks_use (ধ্রুবক hb_ot_shape_plan_t *plan,
		 hb_বাফার_t              *বাফার,
		 hb_font_t                *font HB_UNUSED)
{
  ধ্রুবক use_shape_plan_t *use_plan = (ধ্রুবক use_shape_plan_t *) plan->তথ্য;

  /* Do this before allocating use_category(). */
  যদি (use_plan->arabic_plan)
  {
    setup_masks_arabic_plan (use_plan->arabic_plan, বাফার, plan->props.script);
  }

  HB_বাফার_ALLOCATE_VAR (বাফার, use_category);

  /* We cannot setup masks here.  We save information about characters
   * and setup masks later on in a pause-callback. */

  unsigned পূর্ণসংখ্যা গুনতি = বাফার->len;
  hb_glyph_info_t *info = বাফার->info;
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    info[i].use_category() = hb_use_get_category (info[i].codepoint);
}

স্থির শূন্য
setup_rphf_mask (ধ্রুবক hb_ot_shape_plan_t *plan,
		 hb_বাফার_t *বাফার)
{
  ধ্রুবক use_shape_plan_t *use_plan = (ধ্রুবক use_shape_plan_t *) plan->তথ্য;

  hb_mask_t mask = use_plan->rphf_mask;
  যদি (!mask) ফেরত;

  hb_glyph_info_t *info = বাফার->info;

  foreach_syllable (বাফার, start, end)
  {
    unsigned পূর্ণসংখ্যা limit = info[start].use_category() == USE(R) ? 1 : hb_min (3u, end - start);
    জন্য (unsigned পূর্ণসংখ্যা i = start; i < start + limit; i++)
      info[i].mask |= mask;
  }
}

স্থির শূন্য
setup_topographical_masks (ধ্রুবক hb_ot_shape_plan_t *plan,
			   hb_বাফার_t *বাফার)
{
  ধ্রুবক use_shape_plan_t *use_plan = (ধ্রুবক use_shape_plan_t *) plan->তথ্য;
  যদি (use_plan->arabic_plan)
    ফেরত;

  static_assert ((JOINING_FORM_INIT < 4 && JOINING_FORM_ISOL < 4 && JOINING_FORM_MEDI < 4 && JOINING_FORM_FINA < 4), "");
  hb_mask_t masks[4], all_masks = 0;
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < 4; i++)
  {
    masks[i] = plan->map.get_1_mask (use_topographical_features[i]);
    যদি (masks[i] == plan->map.get_global_mask ())
      masks[i] = 0;
    all_masks |= masks[i];
  }
  যদি (!all_masks)
    ফেরত;
  hb_mask_t other_masks = ~all_masks;

  unsigned পূর্ণসংখ্যা last_start = 0;
  joining_form_t last_form = _JOINING_FORM_NONE;
  hb_glyph_info_t *info = বাফার->info;
  foreach_syllable (বাফার, start, end)
  {
    use_syllable_type_t syllable_type = (use_syllable_type_t) (info[start].syllable() & 0x0F);
    নির্বাচন (syllable_type)
    {
      ক্ষেত্র use_hieroglyph_cluster:
      ক্ষেত্র use_non_cluster:
	/* These don't join.  Nothing to do. */
	last_form = _JOINING_FORM_NONE;
	বিরতি;

      ক্ষেত্র use_virama_terminated_cluster:
      ক্ষেত্র use_sakot_terminated_cluster:
      ক্ষেত্র use_standard_cluster:
      ক্ষেত্র use_সংখ্যা_joiner_terminated_cluster:
      ক্ষেত্র use_numeral_cluster:
      ক্ষেত্র use_প্রতীক_cluster:
      ক্ষেত্র use_broken_cluster:

	যৌক্তিক join = last_form == JOINING_FORM_FINA || last_form == JOINING_FORM_ISOL;

	যদি (join)
	{
	  /* Fixup previous syllable's form. */
	  last_form = last_form == JOINING_FORM_FINA ? JOINING_FORM_MEDI : JOINING_FORM_INIT;
	  জন্য (unsigned পূর্ণসংখ্যা i = last_start; i < start; i++)
	    info[i].mask = (info[i].mask & other_masks) | masks[last_form];
	}

	/* Form for this syllable. */
	last_form = join ? JOINING_FORM_FINA : JOINING_FORM_ISOL;
	জন্য (unsigned পূর্ণসংখ্যা i = start; i < end; i++)
	  info[i].mask = (info[i].mask & other_masks) | masks[last_form];

	বিরতি;
    }

    last_start = start;
  }
}

স্থির যৌক্তিক
setup_syllables_use (ধ্রুবক hb_ot_shape_plan_t *plan,
		     hb_font_t *font HB_UNUSED,
		     hb_বাফার_t *বাফার)
{
  HB_বাফার_ALLOCATE_VAR (বাফার, syllable);
  find_syllables_use (বাফার);
  foreach_syllable (বাফার, start, end)
    বাফার->unsafe_to_break (start, end);
  setup_rphf_mask (plan, বাফার);
  setup_topographical_masks (plan, বাফার);
  ফেরত মিথ্যা;
}

স্থির যৌক্তিক
record_rphf_use (ধ্রুবক hb_ot_shape_plan_t *plan,
		 hb_font_t *font HB_UNUSED,
		 hb_বাফার_t *বাফার)
{
  ধ্রুবক use_shape_plan_t *use_plan = (ধ্রুবক use_shape_plan_t *) plan->তথ্য;

  hb_mask_t mask = use_plan->rphf_mask;
  যদি (!mask) ফেরত মিথ্যা;
  hb_glyph_info_t *info = বাফার->info;

  foreach_syllable (বাফার, start, end)
  {
    /* Mark a substituted repha as USE(R). */
    জন্য (unsigned পূর্ণসংখ্যা i = start; i < end && (info[i].mask & mask); i++)
      যদি (_hb_glyph_info_substituted (&info[i]))
      {
	info[i].use_category() = USE(R);
	বিরতি;
      }
  }
  ফেরত মিথ্যা;
}

স্থির যৌক্তিক
record_pref_use (ধ্রুবক hb_ot_shape_plan_t *plan HB_UNUSED,
		 hb_font_t *font HB_UNUSED,
		 hb_বাফার_t *বাফার)
{
  hb_glyph_info_t *info = বাফার->info;

  foreach_syllable (বাফার, start, end)
  {
    /* Mark a substituted pref as VPre, as they behave the same way. */
    জন্য (unsigned পূর্ণসংখ্যা i = start; i < end; i++)
      যদি (_hb_glyph_info_substituted (&info[i]))
      {
	info[i].use_category() = USE(VPre);
	বিরতি;
      }
  }
  ফেরত মিথ্যা;
}

স্থির অন্তঃরেখ যৌক্তিক
is_halant_use (ধ্রুবক hb_glyph_info_t &info)
{
  ফেরত (info.use_category() == USE(H) || info.use_category() == USE(HVM) || info.use_category() == USE(IS)) &&
	 !_hb_glyph_info_ligated (&info);
}

স্থির শূন্য
reorder_syllable_use (hb_বাফার_t *বাফার, unsigned পূর্ণসংখ্যা start, unsigned পূর্ণসংখ্যা end)
{
  use_syllable_type_t syllable_type = (use_syllable_type_t) (বাফার->info[start].syllable() & 0x0F);
  /* Only a few syllable types need reordering. */
  যদি (unlikely (!(FLAG_UNSAFE (syllable_type) &
		  (FLAG (use_virama_terminated_cluster) |
		   FLAG (use_sakot_terminated_cluster) |
		   FLAG (use_standard_cluster) |
		   FLAG (use_প্রতীক_cluster) |
		   FLAG (use_broken_cluster) |
		   0))))
    ফেরত;

  hb_glyph_info_t *info = বাফার->info;

#define POST_BASE_FLAGS64 (FLAG64 (USE(FAbv)) | \
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
  যদি (info[start].use_category() == USE(R) && end - start > 1)
  {
    /* Got a repha.  Reorder it towards the end, but before the first post-base
     * glyph. */
    জন্য (unsigned পূর্ণসংখ্যা i = start + 1; i < end; i++)
    {
      যৌক্তিক is_post_base_glyph = (FLAG64_UNSAFE (info[i].use_category()) & POST_BASE_FLAGS64) ||
				is_halant_use (info[i]);
      যদি (is_post_base_glyph || i == end - 1)
      {
	/* If we hit a post-base glyph, move before it; otherwise move to the
	 * end. Shift things in between backward. */

	যদি (is_post_base_glyph)
	  i--;

	বাফার->merge_clusters (start, i + 1);
	hb_glyph_info_t t = info[start];
	memmove (&info[start], &info[start + 1], (i - start) * আকার (info[0]));
	info[i] = t;

	বিরতি;
      }
    }
  }

  /* Move things back. */
  unsigned পূর্ণসংখ্যা j = start;
  জন্য (unsigned পূর্ণসংখ্যা i = start; i < end; i++)
  {
    uint32_t flag = FLAG_UNSAFE (info[i].use_category());
    যদি (is_halant_use (info[i]))
    {
      /* If we hit a halant, move after it; otherwise move to the beginning, and
       * shift things in between forward. */
      j = i + 1;
    }
    অন্যথা যদি (((flag) & (FLAG (USE(VPre)) | FLAG (USE(VMPre)))) &&
	     /* Only move the first component of a MultipleSubst. */
	     0 == _hb_glyph_info_get_lig_comp (&info[i]) &&
	     j < i)
    {
      বাফার->merge_clusters (j, i + 1);
      hb_glyph_info_t t = info[i];
      memmove (&info[j + 1], &info[j], (i - j) * আকার (info[0]));
      info[j] = t;
    }
  }
}

স্থির যৌক্তিক
reorder_use (ধ্রুবক hb_ot_shape_plan_t *plan,
	     hb_font_t *font,
	     hb_বাফার_t *বাফার)
{
  যৌক্তিক ret = মিথ্যা;
  যদি (বাফার->message (font, "start reordering USE"))
  {
    যদি (hb_syllabic_ঢোকাও_dotted_circles (font, বাফার,
					   use_broken_cluster,
					   USE(B),
					   USE(R)))
      ret = সত্য;

    foreach_syllable (বাফার, start, end)
      reorder_syllable_use (বাফার, start, end);

    (শূন্য) বাফার->message (font, "end reordering USE");
  }

  HB_বাফার_DEALLOCATE_VAR (বাফার, use_category);

  ফেরত ret;
}


স্থির শূন্য
preprocess_text_use (ধ্রুবক hb_ot_shape_plan_t *plan,
		     hb_বাফার_t              *বাফার,
		     hb_font_t                *font)
{
  _hb_preprocess_text_vowel_constraints (plan, বাফার, font);
}

স্থির যৌক্তিক
compose_use (ধ্রুবক hb_ot_shape_normalize_context_t *c,
	     hb_codepoint_t  a,
	     hb_codepoint_t  b,
	     hb_codepoint_t *ab)
{
  /* Avoid recomposing split matras. */
  যদি (HB_UNICODE_GENERAL_CATEGORY_IS_MARK (c->unicode->general_category (a)))
    ফেরত মিথ্যা;

  ফেরত (যৌক্তিক)c->unicode->compose (a, b, ab);
}


ধ্রুবক hb_ot_shaper_t _hb_ot_shaper_use =
{
  collect_features_use,
  nullptr, /* override_features */
  তথ্য_create_use,
  তথ্য_destroy_use,
  preprocess_text_use,
  nullptr, /* postprocess_glyphs */
  nullptr, /* decompose */
  compose_use,
  setup_masks_use,
  nullptr, /* reorder_marks */
  HB_TAG_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_MODE_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT,
  HB_OT_SHAPE_ZERO_WIDTH_MARKS_BY_GDEF_EARLY,
  মিথ্যা, /* fallback_position */
};


#endif

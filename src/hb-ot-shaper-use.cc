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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPE

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shaper-use-machine.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shaper-use-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shaper-arabic.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shaper-arabic-joining-list.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shaper-vowel-constraints.hh"


/*
 * Universal Shaping Engine.
 * https://docs.microsoft.com/en-us/typography/script-development/use
 */

ਸਥਿਰ ਅਚਲ hb_ਟੈਗ_t
use_basic_features[] =
{
  /*
   * Basic features.
   * These features are applied all at once, before reordering, constrained
   * to the syllable.
   */
  HB_ਟੈਗ('r','k','r','f'),
  HB_ਟੈਗ('a','b','v','f'),
  HB_ਟੈਗ('b','l','w','f'),
  HB_ਟੈਗ('h','a','l','f'),
  HB_ਟੈਗ('p','s','t','f'),
  HB_ਟੈਗ('v','a','t','u'),
  HB_ਟੈਗ('c','j','c','t'),
};
ਸਥਿਰ ਅਚਲ hb_ਟੈਗ_t
use_topographical_features[] =
{
  HB_ਟੈਗ('i','s','o','l'),
  HB_ਟੈਗ('i','n','i','t'),
  HB_ਟੈਗ('m','e','d','i'),
  HB_ਟੈਗ('f','i','n','a'),
};
/* Same order as use_topographical_features. */
ਗਿਣਤੀ joining_form_t {
  JOINING_FORM_ISOL,
  JOINING_FORM_INIT,
  JOINING_FORM_MEDI,
  JOINING_FORM_FINA,
  _JOINING_FORM_NONE
};
ਸਥਿਰ ਅਚਲ hb_ਟੈਗ_t
use_other_features[] =
{
  /*
   * Other features.
   * These features are applied all at once, after reordering and
   * clearing syllables.
   */
  HB_ਟੈਗ('a','b','v','s'),
  HB_ਟੈਗ('b','l','w','s'),
  HB_ਟੈਗ('h','a','l','n'),
  HB_ਟੈਗ('p','r','e','s'),
  HB_ਟੈਗ('p','s','t','s'),
};

ਸਥਿਰ ਤਾਰਕਿਕ
ਤਿਆਰੀ_syllables_use (ਅਚਲ hb_ot_shape_plan_t *plan,
		     hb_font_t *font,
		     hb_ਬਫਰ_t *ਬਫਰ);
ਸਥਿਰ ਤਾਰਕਿਕ
ਰਿਕਾਰਡ_rphf_use (ਅਚਲ hb_ot_shape_plan_t *plan,
		 hb_font_t *font,
		 hb_ਬਫਰ_t *ਬਫਰ);
ਸਥਿਰ ਤਾਰਕਿਕ
ਰਿਕਾਰਡ_pref_use (ਅਚਲ hb_ot_shape_plan_t *plan,
		 hb_font_t *font,
		 hb_ਬਫਰ_t *ਬਫਰ);
ਸਥਿਰ ਤਾਰਕਿਕ
reorder_use (ਅਚਲ hb_ot_shape_plan_t *plan,
	     hb_font_t *font,
	     hb_ਬਫਰ_t *ਬਫਰ);

ਸਥਿਰ ਖਾਲੀ
collect_features_use (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  /* Do this before any lookups have been applied. */
  map->ਜੋੜੋ_gsub_pause (ਤਿਆਰੀ_syllables_use);

  /* "Default glyph pre-processing group" */
  map->enable_feature (HB_ਟੈਗ('l','o','c','l'), F_ਪ੍ਰਤੀ_SYLLABLE);
  map->enable_feature (HB_ਟੈਗ('c','c','m','p'), F_ਪ੍ਰਤੀ_SYLLABLE);
  map->enable_feature (HB_ਟੈਗ('n','u','k','t'), F_ਪ੍ਰਤੀ_SYLLABLE);
  map->enable_feature (HB_ਟੈਗ('a','k','h','n'), F_MANUAL_ZWJ | F_ਪ੍ਰਤੀ_SYLLABLE);

  /* "Reordering group" */
  map->ਜੋੜੋ_gsub_pause (_hb_clear_substitution_flags);
  map->ਜੋੜੋ_feature (HB_ਟੈਗ('r','p','h','f'), F_MANUAL_ZWJ | F_ਪ੍ਰਤੀ_SYLLABLE);
  map->ਜੋੜੋ_gsub_pause (ਰਿਕਾਰਡ_rphf_use);
  map->ਜੋੜੋ_gsub_pause (_hb_clear_substitution_flags);
  map->enable_feature (HB_ਟੈਗ('p','r','e','f'), F_MANUAL_ZWJ | F_ਪ੍ਰਤੀ_SYLLABLE);
  map->ਜੋੜੋ_gsub_pause (ਰਿਕਾਰਡ_pref_use);

  /* "Orthographic unit shaping group" */
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ARRAY_ਲੰਮਾਈ (use_basic_features); i++)
    map->enable_feature (use_basic_features[i], F_MANUAL_ZWJ | F_ਪ੍ਰਤੀ_SYLLABLE);

  map->ਜੋੜੋ_gsub_pause (reorder_use);
  map->ਜੋੜੋ_gsub_pause (hb_syllabic_clear_var); // Don't need syllables anymore, use stop to free buffer var

  /* "Topographical features" */
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ARRAY_ਲੰਮਾਈ (use_topographical_features); i++)
    map->ਜੋੜੋ_feature (use_topographical_features[i]);
  map->ਜੋੜੋ_gsub_pause (ਖਾਲੀ_ਸੂਚਕ);

  /* "Standard typographic presentation" */
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ARRAY_ਲੰਮਾਈ (use_other_features); i++)
    map->enable_feature (use_other_features[i], F_MANUAL_ZWJ);
}

ਬਣਤਰ use_shape_plan_t
{
  hb_ਨਕਾਬ_t rphf_ਨਕਾਬ;

  arabic_shape_plan_t *arabic_plan;
};

ਸਥਿਰ ਖਾਲੀ *
ਅੰਕੜੇ_create_use (ਅਚਲ hb_ot_shape_plan_t *plan)
{
  use_shape_plan_t *use_plan = (use_shape_plan_t *) hb_calloc (1, ਆਕਾਰ (use_shape_plan_t));
  ਜੇ (unlikely (!use_plan))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  use_plan->rphf_ਨਕਾਬ = plan->map.get_1_ਨਕਾਬ (HB_ਟੈਗ('r','p','h','f'));

  ਜੇ (has_arabic_joining (plan->props.script))
  {
    use_plan->arabic_plan = (arabic_shape_plan_t *) ਅੰਕੜੇ_create_arabic (plan);
    ਜੇ (unlikely (!use_plan->arabic_plan))
    {
      hb_free (use_plan);
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    }
  }

  ਵਾਪਸ use_plan;
}

ਸਥਿਰ ਖਾਲੀ
ਅੰਕੜੇ_destroy_use (ਖਾਲੀ *ਅੰਕੜੇ)
{
  use_shape_plan_t *use_plan = (use_shape_plan_t *) ਅੰਕੜੇ;

  ਜੇ (use_plan->arabic_plan)
    ਅੰਕੜੇ_destroy_arabic (use_plan->arabic_plan);

  hb_free (ਅੰਕੜੇ);
}

ਸਥਿਰ ਖਾਲੀ
ਤਿਆਰੀ_masks_use (ਅਚਲ hb_ot_shape_plan_t *plan,
		 hb_ਬਫਰ_t              *ਬਫਰ,
		 hb_font_t                *font HB_ਅਣਵਰਤਿਆ)
{
  ਅਚਲ use_shape_plan_t *use_plan = (ਅਚਲ use_shape_plan_t *) plan->ਅੰਕੜੇ;

  /* Do this before allocating use_category(). */
  ਜੇ (use_plan->arabic_plan)
  {
    ਤਿਆਰੀ_masks_arabic_plan (use_plan->arabic_plan, ਬਫਰ, plan->props.script);
  }

  HB_ਬਫਰ_ALLOCATE_VAR (ਬਫਰ, use_category);

  /* We cannot setup masks here.  We save information about characters
   * and setup masks later on in a pause-callback. */

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਬਫਰ->len;
  hb_glyph_info_t *info = ਬਫਰ->info;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    info[i].use_category() = hb_use_get_category (info[i].codepoint);
}

ਸਥਿਰ ਖਾਲੀ
ਤਿਆਰੀ_rphf_ਨਕਾਬ (ਅਚਲ hb_ot_shape_plan_t *plan,
		 hb_ਬਫਰ_t *ਬਫਰ)
{
  ਅਚਲ use_shape_plan_t *use_plan = (ਅਚਲ use_shape_plan_t *) plan->ਅੰਕੜੇ;

  hb_ਨਕਾਬ_t ਨਕਾਬ = use_plan->rphf_ਨਕਾਬ;
  ਜੇ (!ਨਕਾਬ) ਵਾਪਸ;

  hb_glyph_info_t *info = ਬਫਰ->info;

  foreach_syllable (ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਹੱਦਬੰਦੀ = info[ਸ਼ੁਰੂ].use_category() == USE(R) ? 1 : hb_min (3u, ਸਿਰਾ - ਸ਼ੁਰੂ);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ; i < ਸ਼ੁਰੂ + ਹੱਦਬੰਦੀ; i++)
      info[i].ਨਕਾਬ |= ਨਕਾਬ;
  }
}

ਸਥਿਰ ਖਾਲੀ
ਤਿਆਰੀ_topographical_masks (ਅਚਲ hb_ot_shape_plan_t *plan,
			   hb_ਬਫਰ_t *ਬਫਰ)
{
  ਅਚਲ use_shape_plan_t *use_plan = (ਅਚਲ use_shape_plan_t *) plan->ਅੰਕੜੇ;
  ਜੇ (use_plan->arabic_plan)
    ਵਾਪਸ;

  static_assert ((JOINING_FORM_INIT < 4 && JOINING_FORM_ISOL < 4 && JOINING_FORM_MEDI < 4 && JOINING_FORM_FINA < 4), "");
  hb_ਨਕਾਬ_t masks[4], all_masks = 0;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < 4; i++)
  {
    masks[i] = plan->map.get_1_ਨਕਾਬ (use_topographical_features[i]);
    ਜੇ (masks[i] == plan->map.get_global_ਨਕਾਬ ())
      masks[i] = 0;
    all_masks |= masks[i];
  }
  ਜੇ (!all_masks)
    ਵਾਪਸ;
  hb_ਨਕਾਬ_t other_masks = ~all_masks;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਆਖਰੀ_ਸ਼ੁਰੂ = 0;
  joining_form_t ਆਖਰੀ_form = _JOINING_FORM_NONE;
  hb_glyph_info_t *info = ਬਫਰ->info;
  foreach_syllable (ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ)
  {
    use_syllable_type_t syllable_type = (use_syllable_type_t) (info[ਸ਼ੁਰੂ].syllable() & 0x0F);
    ਚੋਣ (syllable_type)
    {
      ਹਾਲਤ use_hieroglyph_cluster:
      ਹਾਲਤ use_non_cluster:
	/* These don't join.  Nothing to do. */
	ਆਖਰੀ_form = _JOINING_FORM_NONE;
	ਰੋਕੋ;

      ਹਾਲਤ use_virama_terminated_cluster:
      ਹਾਲਤ use_sakot_terminated_cluster:
      ਹਾਲਤ use_standard_cluster:
      ਹਾਲਤ use_ਸੰਖਿਆ_joiner_terminated_cluster:
      ਹਾਲਤ use_numeral_cluster:
      ਹਾਲਤ use_ਚਿੰਨ੍ਹ_cluster:
      ਹਾਲਤ use_broken_cluster:

	ਤਾਰਕਿਕ join = ਆਖਰੀ_form == JOINING_FORM_FINA || ਆਖਰੀ_form == JOINING_FORM_ISOL;

	ਜੇ (join)
	{
	  /* Fixup previous syllable's form. */
	  ਆਖਰੀ_form = ਆਖਰੀ_form == JOINING_FORM_FINA ? JOINING_FORM_MEDI : JOINING_FORM_INIT;
	  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਆਖਰੀ_ਸ਼ੁਰੂ; i < ਸ਼ੁਰੂ; i++)
	    info[i].ਨਕਾਬ = (info[i].ਨਕਾਬ & other_masks) | masks[ਆਖਰੀ_form];
	}

	/* Form for this syllable. */
	ਆਖਰੀ_form = join ? JOINING_FORM_FINA : JOINING_FORM_ISOL;
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ; i < ਸਿਰਾ; i++)
	  info[i].ਨਕਾਬ = (info[i].ਨਕਾਬ & other_masks) | masks[ਆਖਰੀ_form];

	ਰੋਕੋ;
    }

    ਆਖਰੀ_ਸ਼ੁਰੂ = ਸ਼ੁਰੂ;
  }
}

ਸਥਿਰ ਤਾਰਕਿਕ
ਤਿਆਰੀ_syllables_use (ਅਚਲ hb_ot_shape_plan_t *plan,
		     hb_font_t *font HB_ਅਣਵਰਤਿਆ,
		     hb_ਬਫਰ_t *ਬਫਰ)
{
  HB_ਬਫਰ_ALLOCATE_VAR (ਬਫਰ, syllable);
  ਲੱਭੋ_syllables_use (ਬਫਰ);
  foreach_syllable (ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ)
    ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_break (ਸ਼ੁਰੂ, ਸਿਰਾ);
  ਤਿਆਰੀ_rphf_ਨਕਾਬ (plan, ਬਫਰ);
  ਤਿਆਰੀ_topographical_masks (plan, ਬਫਰ);
  ਵਾਪਸ ਝੂਠ;
}

ਸਥਿਰ ਤਾਰਕਿਕ
ਰਿਕਾਰਡ_rphf_use (ਅਚਲ hb_ot_shape_plan_t *plan,
		 hb_font_t *font HB_ਅਣਵਰਤਿਆ,
		 hb_ਬਫਰ_t *ਬਫਰ)
{
  ਅਚਲ use_shape_plan_t *use_plan = (ਅਚਲ use_shape_plan_t *) plan->ਅੰਕੜੇ;

  hb_ਨਕਾਬ_t ਨਕਾਬ = use_plan->rphf_ਨਕਾਬ;
  ਜੇ (!ਨਕਾਬ) ਵਾਪਸ ਝੂਠ;
  hb_glyph_info_t *info = ਬਫਰ->info;

  foreach_syllable (ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ)
  {
    /* Mark a substituted repha as USE(R). */
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ; i < ਸਿਰਾ && (info[i].ਨਕਾਬ & ਨਕਾਬ); i++)
      ਜੇ (_hb_glyph_info_substituted (&info[i]))
      {
	info[i].use_category() = USE(R);
	ਰੋਕੋ;
      }
  }
  ਵਾਪਸ ਝੂਠ;
}

ਸਥਿਰ ਤਾਰਕਿਕ
ਰਿਕਾਰਡ_pref_use (ਅਚਲ hb_ot_shape_plan_t *plan HB_ਅਣਵਰਤਿਆ,
		 hb_font_t *font HB_ਅਣਵਰਤਿਆ,
		 hb_ਬਫਰ_t *ਬਫਰ)
{
  hb_glyph_info_t *info = ਬਫਰ->info;

  foreach_syllable (ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ)
  {
    /* Mark a substituted pref as VPre, as they behave the same way. */
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ; i < ਸਿਰਾ; i++)
      ਜੇ (_hb_glyph_info_substituted (&info[i]))
      {
	info[i].use_category() = USE(VPre);
	ਰੋਕੋ;
      }
  }
  ਵਾਪਸ ਝੂਠ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
is_halant_use (ਅਚਲ hb_glyph_info_t &info)
{
  ਵਾਪਸ (info.use_category() == USE(H) || info.use_category() == USE(HVM) || info.use_category() == USE(IS)) &&
	 !_hb_glyph_info_ligated (&info);
}

ਸਥਿਰ ਖਾਲੀ
reorder_syllable_use (hb_ਬਫਰ_t *ਬਫਰ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ)
{
  use_syllable_type_t syllable_type = (use_syllable_type_t) (ਬਫਰ->info[ਸ਼ੁਰੂ].syllable() & 0x0F);
  /* Only a few syllable types need reordering. */
  ਜੇ (unlikely (!(FLAG_UNSAFE (syllable_type) &
		  (FLAG (use_virama_terminated_cluster) |
		   FLAG (use_sakot_terminated_cluster) |
		   FLAG (use_standard_cluster) |
		   FLAG (use_ਚਿੰਨ੍ਹ_cluster) |
		   FLAG (use_broken_cluster) |
		   0))))
    ਵਾਪਸ;

  hb_glyph_info_t *info = ਬਫਰ->info;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ POST_ਅਧਾਰ_FLAGS64 (FLAG64 (USE(FAbv)) | \
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
  ਜੇ (info[ਸ਼ੁਰੂ].use_category() == USE(R) && ਸਿਰਾ - ਸ਼ੁਰੂ > 1)
  {
    /* Got a repha.  Reorder it towards the end, but before the first post-base
     * glyph. */
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ + 1; i < ਸਿਰਾ; i++)
    {
      ਤਾਰਕਿਕ is_post_ਅਧਾਰ_glyph = (FLAG64_UNSAFE (info[i].use_category()) & POST_ਅਧਾਰ_FLAGS64) ||
				is_halant_use (info[i]);
      ਜੇ (is_post_ਅਧਾਰ_glyph || i == ਸਿਰਾ - 1)
      {
	/* If we hit a post-base glyph, move before it; otherwise move to the
	 * end. Shift things in between backward. */

	ਜੇ (is_post_ਅਧਾਰ_glyph)
	  i--;

	ਬਫਰ->ਮਿਲਾਓ_clusters (ਸ਼ੁਰੂ, i + 1);
	hb_glyph_info_t t = info[ਸ਼ੁਰੂ];
	memmove (&info[ਸ਼ੁਰੂ], &info[ਸ਼ੁਰੂ + 1], (i - ਸ਼ੁਰੂ) * ਆਕਾਰ (info[0]));
	info[i] = t;

	ਰੋਕੋ;
      }
    }
  }

  /* Move things back. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = ਸ਼ੁਰੂ;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ; i < ਸਿਰਾ; i++)
  {
    uint32_t flag = FLAG_UNSAFE (info[i].use_category());
    ਜੇ (is_halant_use (info[i]))
    {
      /* If we hit a halant, move after it; otherwise move to the beginning, and
       * shift things in between forward. */
      j = i + 1;
    }
    ਨਹੀਂਤਾਂ ਜੇ (((flag) & (FLAG (USE(VPre)) | FLAG (USE(VMPre)))) &&
	     /* Only move the first component of a MultipleSubst. */
	     0 == _hb_glyph_info_get_lig_comp (&info[i]) &&
	     j < i)
    {
      ਬਫਰ->ਮਿਲਾਓ_clusters (j, i + 1);
      hb_glyph_info_t t = info[i];
      memmove (&info[j + 1], &info[j], (i - j) * ਆਕਾਰ (info[0]));
      info[j] = t;
    }
  }
}

ਸਥਿਰ ਤਾਰਕਿਕ
reorder_use (ਅਚਲ hb_ot_shape_plan_t *plan,
	     hb_font_t *font,
	     hb_ਬਫਰ_t *ਬਫਰ)
{
  ਤਾਰਕਿਕ ret = ਝੂਠ;
  ਜੇ (ਬਫਰ->ਸੁਨੇਹਾ (font, "start reordering USE"))
  {
    ਜੇ (hb_syllabic_ਪਾਓ_dotted_circles (font, ਬਫਰ,
					   use_broken_cluster,
					   USE(B),
					   USE(R)))
      ret = ਸੱਚ;

    foreach_syllable (ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ)
      reorder_syllable_use (ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ);

    (ਖਾਲੀ) ਬਫਰ->ਸੁਨੇਹਾ (font, "end reordering USE");
  }

  HB_ਬਫਰ_DEALLOCATE_VAR (ਬਫਰ, use_category);

  ਵਾਪਸ ret;
}


ਸਥਿਰ ਖਾਲੀ
preprocess_ਪਾਠ_use (ਅਚਲ hb_ot_shape_plan_t *plan,
		     hb_ਬਫਰ_t              *ਬਫਰ,
		     hb_font_t                *font)
{
  hb_preprocess_ਪਾਠ_vowel_constraints (plan, ਬਫਰ, font);
}

ਸਥਿਰ ਤਾਰਕਿਕ
compose_use (ਅਚਲ hb_ot_shape_normalize_context_t *c,
	     hb_codepoint_t  a,
	     hb_codepoint_t  b,
	     hb_codepoint_t *ab)
{
  /* Avoid recomposing split matras. */
  ਜੇ (HB_UNICODE_GENERAL_CATEGORY_IS_MARK (c->unicode->general_category (a)))
    ਵਾਪਸ ਝੂਠ;

  ਵਾਪਸ (ਤਾਰਕਿਕ)c->unicode->compose (a, b, ab);
}


ਅਚਲ hb_ot_shaper_t _hb_ot_shaper_use =
{
  collect_features_use,
  ਖਾਲੀ_ਸੂਚਕ, /* override_features */
  ਅੰਕੜੇ_create_use,
  ਅੰਕੜੇ_destroy_use,
  preprocess_ਪਾਠ_use,
  ਖਾਲੀ_ਸੂਚਕ, /* postprocess_glyphs */
  ਖਾਲੀ_ਸੂਚਕ, /* decompose */
  compose_use,
  ਤਿਆਰੀ_masks_use,
  ਖਾਲੀ_ਸੂਚਕ, /* reorder_marks */
  HB_ਟੈਗ_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_ਬਹੁਲਕ_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT,
  HB_OT_SHAPE_ZERO_ਚੌੜਾਈ_MARKS_BY_GDEF_EARLY,
  ਝੂਠ, /* fallback_position */
};


#ਜੇ_ਅੰਤ

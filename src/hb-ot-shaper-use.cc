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

#અન્તર્ભાવ "hb.hh"

#ifndef HB_NO_OT_SHAPE

#અન્તર્ભાવ "hb-ot-shaper-use-machine.hh"
#અન્તર્ભાવ "hb-ot-shaper-use-table.hh"
#અન્તર્ભાવ "hb-ot-shaper-arabic.hh"
#અન્તર્ભાવ "hb-ot-shaper-arabic-joining-list.hh"
#અન્તર્ભાવ "hb-ot-shaper-vowel-constraints.hh"


/*
 * Universal Shaping Engine.
 * https://docs.microsoft.com/en-us/typography/script-development/use
 */

સ્થિર અચલ hb_tag_t
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
સ્થિર અચલ hb_tag_t
use_topographical_features[] =
{
  HB_TAG('i','s','o','l'),
  HB_TAG('i','n','i','t'),
  HB_TAG('m','e','d','i'),
  HB_TAG('f','i','n','a'),
};
/* Same order as use_topographical_features. */
ગણના joining_form_t {
  JOINING_FORM_ISOL,
  JOINING_FORM_INIT,
  JOINING_FORM_MEDI,
  JOINING_FORM_FINA,
  _JOINING_FORM_NONE
};
સ્થિર અચલ hb_tag_t
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

સ્થિર તાર્કિક
setup_syllables_use (અચલ hb_ot_shape_plan_t *plan,
		     hb_font_t *font,
		     hb_buffer_t *buffer);
સ્થિર તાર્કિક
record_rphf_use (અચલ hb_ot_shape_plan_t *plan,
		 hb_font_t *font,
		 hb_buffer_t *buffer);
સ્થિર તાર્કિક
record_pref_use (અચલ hb_ot_shape_plan_t *plan,
		 hb_font_t *font,
		 hb_buffer_t *buffer);
સ્થિર તાર્કિક
reorder_use (અચલ hb_ot_shape_plan_t *plan,
	     hb_font_t *font,
	     hb_buffer_t *buffer);

સ્થિર શૂન્ય
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
  માટે (unsigned પૂર્ણાંક i = 0; i < ARRAY_LENGTH (use_basic_features); i++)
    map->enable_feature (use_basic_features[i], F_MANUAL_ZWJ | F_PER_SYLLABLE);

  map->add_gsub_pause (reorder_use);
  map->add_gsub_pause (hb_syllabic_clear_var); // Don't need syllables anymore, use stop to free buffer var

  /* "Topographical features" */
  માટે (unsigned પૂર્ણાંક i = 0; i < ARRAY_LENGTH (use_topographical_features); i++)
    map->add_feature (use_topographical_features[i]);
  map->add_gsub_pause (nullptr);

  /* "Standard typographic presentation" */
  માટે (unsigned પૂર્ણાંક i = 0; i < ARRAY_LENGTH (use_other_features); i++)
    map->enable_feature (use_other_features[i], F_MANUAL_ZWJ);
}

સંરચના use_shape_plan_t
{
  hb_mask_t rphf_mask;

  arabic_shape_plan_t *arabic_plan;
};

સ્થિર શૂન્ય *
data_create_use (અચલ hb_ot_shape_plan_t *plan)
{
  use_shape_plan_t *use_plan = (use_shape_plan_t *) hb_calloc (1, કદ (use_shape_plan_t));
  જો (unlikely (!use_plan))
    પાછા nullptr;

  use_plan->rphf_mask = plan->map.get_1_mask (HB_TAG('r','p','h','f'));

  જો (has_arabic_joining (plan->props.script))
  {
    use_plan->arabic_plan = (arabic_shape_plan_t *) data_create_arabic (plan);
    જો (unlikely (!use_plan->arabic_plan))
    {
      hb_free (use_plan);
      પાછા nullptr;
    }
  }

  પાછા use_plan;
}

સ્થિર શૂન્ય
data_destroy_use (શૂન્ય *data)
{
  use_shape_plan_t *use_plan = (use_shape_plan_t *) data;

  જો (use_plan->arabic_plan)
    data_destroy_arabic (use_plan->arabic_plan);

  hb_free (data);
}

સ્થિર શૂન્ય
setup_masks_use (અચલ hb_ot_shape_plan_t *plan,
		 hb_buffer_t              *buffer,
		 hb_font_t                *font HB_UNUSED)
{
  અચલ use_shape_plan_t *use_plan = (અચલ use_shape_plan_t *) plan->data;

  /* Do this before allocating use_category(). */
  જો (use_plan->arabic_plan)
  {
    setup_masks_arabic_plan (use_plan->arabic_plan, buffer, plan->props.script);
  }

  HB_BUFFER_ALLOCATE_VAR (buffer, use_category);

  /* We cannot setup masks here.  We save information about characters
   * and setup masks later on in a pause-callback. */

  unsigned પૂર્ણાંક ગણતરી = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    info[i].use_category() = hb_use_get_category (info[i].codepoint);
}

સ્થિર શૂન્ય
setup_rphf_mask (અચલ hb_ot_shape_plan_t *plan,
		 hb_buffer_t *buffer)
{
  અચલ use_shape_plan_t *use_plan = (અચલ use_shape_plan_t *) plan->data;

  hb_mask_t mask = use_plan->rphf_mask;
  જો (!mask) પાછા;

  hb_glyph_info_t *info = buffer->info;

  foreach_syllable (buffer, start, end)
  {
    unsigned પૂર્ણાંક limit = info[start].use_category() == USE(R) ? 1 : hb_min (3u, end - start);
    માટે (unsigned પૂર્ણાંક i = start; i < start + limit; i++)
      info[i].mask |= mask;
  }
}

સ્થિર શૂન્ય
setup_topographical_masks (અચલ hb_ot_shape_plan_t *plan,
			   hb_buffer_t *buffer)
{
  અચલ use_shape_plan_t *use_plan = (અચલ use_shape_plan_t *) plan->data;
  જો (use_plan->arabic_plan)
    પાછા;

  static_assert ((JOINING_FORM_INIT < 4 && JOINING_FORM_ISOL < 4 && JOINING_FORM_MEDI < 4 && JOINING_FORM_FINA < 4), "");
  hb_mask_t masks[4], all_masks = 0;
  માટે (unsigned પૂર્ણાંક i = 0; i < 4; i++)
  {
    masks[i] = plan->map.get_1_mask (use_topographical_features[i]);
    જો (masks[i] == plan->map.get_global_mask ())
      masks[i] = 0;
    all_masks |= masks[i];
  }
  જો (!all_masks)
    પાછા;
  hb_mask_t other_masks = ~all_masks;

  unsigned પૂર્ણાંક last_start = 0;
  joining_form_t last_form = _JOINING_FORM_NONE;
  hb_glyph_info_t *info = buffer->info;
  foreach_syllable (buffer, start, end)
  {
    use_syllable_type_t syllable_type = (use_syllable_type_t) (info[start].syllable() & 0x0F);
    પસંદગી (syllable_type)
    {
      સ્થિતિ use_hieroglyph_cluster:
      સ્થિતિ use_non_cluster:
	/* These don't join.  Nothing to do. */
	last_form = _JOINING_FORM_NONE;
	તોડો;

      સ્થિતિ use_virama_terminated_cluster:
      સ્થિતિ use_sakot_terminated_cluster:
      સ્થિતિ use_standard_cluster:
      સ્થિતિ use_સંખ્યા_joiner_terminated_cluster:
      સ્થિતિ use_numeral_cluster:
      સ્થિતિ use_પ્રતીક_cluster:
      સ્થિતિ use_broken_cluster:

	તાર્કિક join = last_form == JOINING_FORM_FINA || last_form == JOINING_FORM_ISOL;

	જો (join)
	{
	  /* Fixup previous syllable's form. */
	  last_form = last_form == JOINING_FORM_FINA ? JOINING_FORM_MEDI : JOINING_FORM_INIT;
	  માટે (unsigned પૂર્ણાંક i = last_start; i < start; i++)
	    info[i].mask = (info[i].mask & other_masks) | masks[last_form];
	}

	/* Form for this syllable. */
	last_form = join ? JOINING_FORM_FINA : JOINING_FORM_ISOL;
	માટે (unsigned પૂર્ણાંક i = start; i < end; i++)
	  info[i].mask = (info[i].mask & other_masks) | masks[last_form];

	તોડો;
    }

    last_start = start;
  }
}

સ્થિર તાર્કિક
setup_syllables_use (અચલ hb_ot_shape_plan_t *plan,
		     hb_font_t *font HB_UNUSED,
		     hb_buffer_t *buffer)
{
  HB_BUFFER_ALLOCATE_VAR (buffer, syllable);
  find_syllables_use (buffer);
  foreach_syllable (buffer, start, end)
    buffer->unsafe_to_break (start, end);
  setup_rphf_mask (plan, buffer);
  setup_topographical_masks (plan, buffer);
  પાછા ખોટું;
}

સ્થિર તાર્કિક
record_rphf_use (અચલ hb_ot_shape_plan_t *plan,
		 hb_font_t *font HB_UNUSED,
		 hb_buffer_t *buffer)
{
  અચલ use_shape_plan_t *use_plan = (અચલ use_shape_plan_t *) plan->data;

  hb_mask_t mask = use_plan->rphf_mask;
  જો (!mask) પાછા ખોટું;
  hb_glyph_info_t *info = buffer->info;

  foreach_syllable (buffer, start, end)
  {
    /* Mark a substituted repha as USE(R). */
    માટે (unsigned પૂર્ણાંક i = start; i < end && (info[i].mask & mask); i++)
      જો (_hb_glyph_info_substituted (&info[i]))
      {
	info[i].use_category() = USE(R);
	તોડો;
      }
  }
  પાછા ખોટું;
}

સ્થિર તાર્કિક
record_pref_use (અચલ hb_ot_shape_plan_t *plan HB_UNUSED,
		 hb_font_t *font HB_UNUSED,
		 hb_buffer_t *buffer)
{
  hb_glyph_info_t *info = buffer->info;

  foreach_syllable (buffer, start, end)
  {
    /* Mark a substituted pref as VPre, as they behave the same way. */
    માટે (unsigned પૂર્ણાંક i = start; i < end; i++)
      જો (_hb_glyph_info_substituted (&info[i]))
      {
	info[i].use_category() = USE(VPre);
	તોડો;
      }
  }
  પાછા ખોટું;
}

સ્થિર અંતઃરેખ તાર્કિક
is_halant_use (અચલ hb_glyph_info_t &info)
{
  પાછા (info.use_category() == USE(H) || info.use_category() == USE(HVM) || info.use_category() == USE(IS)) &&
	 !_hb_glyph_info_ligated (&info);
}

સ્થિર શૂન્ય
reorder_syllable_use (hb_buffer_t *buffer, unsigned પૂર્ણાંક start, unsigned પૂર્ણાંક end)
{
  use_syllable_type_t syllable_type = (use_syllable_type_t) (buffer->info[start].syllable() & 0x0F);
  /* Only a few syllable types need reordering. */
  જો (unlikely (!(FLAG_UNSAFE (syllable_type) &
		  (FLAG (use_virama_terminated_cluster) |
		   FLAG (use_sakot_terminated_cluster) |
		   FLAG (use_standard_cluster) |
		   FLAG (use_પ્રતીક_cluster) |
		   FLAG (use_broken_cluster) |
		   0))))
    પાછા;

  hb_glyph_info_t *info = buffer->info;

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
  જો (info[start].use_category() == USE(R) && end - start > 1)
  {
    /* Got a repha.  Reorder it towards the end, but before the first post-base
     * glyph. */
    માટે (unsigned પૂર્ણાંક i = start + 1; i < end; i++)
    {
      તાર્કિક is_post_base_glyph = (FLAG64_UNSAFE (info[i].use_category()) & POST_BASE_FLAGS64) ||
				is_halant_use (info[i]);
      જો (is_post_base_glyph || i == end - 1)
      {
	/* If we hit a post-base glyph, move before it; otherwise move to the
	 * end. Shift things in between backward. */

	જો (is_post_base_glyph)
	  i--;

	buffer->merge_clusters (start, i + 1);
	hb_glyph_info_t t = info[start];
	memmove (&info[start], &info[start + 1], (i - start) * કદ (info[0]));
	info[i] = t;

	તોડો;
      }
    }
  }

  /* Move things back. */
  unsigned પૂર્ણાંક j = start;
  માટે (unsigned પૂર્ણાંક i = start; i < end; i++)
  {
    uint32_t flag = FLAG_UNSAFE (info[i].use_category());
    જો (is_halant_use (info[i]))
    {
      /* If we hit a halant, move after it; otherwise move to the beginning, and
       * shift things in between forward. */
      j = i + 1;
    }
    નહીંતર જો (((flag) & (FLAG (USE(VPre)) | FLAG (USE(VMPre)))) &&
	     /* Only move the first component of a MultipleSubst. */
	     0 == _hb_glyph_info_get_lig_comp (&info[i]) &&
	     j < i)
    {
      buffer->merge_clusters (j, i + 1);
      hb_glyph_info_t t = info[i];
      memmove (&info[j + 1], &info[j], (i - j) * કદ (info[0]));
      info[j] = t;
    }
  }
}

સ્થિર તાર્કિક
reorder_use (અચલ hb_ot_shape_plan_t *plan,
	     hb_font_t *font,
	     hb_buffer_t *buffer)
{
  તાર્કિક ret = ખોટું;
  જો (buffer->message (font, "start reordering USE"))
  {
    જો (hb_syllabic_ઉમેરો_dotted_circles (font, buffer,
					   use_broken_cluster,
					   USE(B),
					   USE(R)))
      ret = સાચું;

    foreach_syllable (buffer, start, end)
      reorder_syllable_use (buffer, start, end);

    (શૂન્ય) buffer->message (font, "end reordering USE");
  }

  HB_BUFFER_DEALLOCATE_VAR (buffer, use_category);

  પાછા ret;
}


સ્થિર શૂન્ય
preprocess_text_use (અચલ hb_ot_shape_plan_t *plan,
		     hb_buffer_t              *buffer,
		     hb_font_t                *font)
{
  _hb_preprocess_text_vowel_constraints (plan, buffer, font);
}

સ્થિર તાર્કિક
compose_use (અચલ hb_ot_shape_normalize_context_t *c,
	     hb_codepoint_t  a,
	     hb_codepoint_t  b,
	     hb_codepoint_t *ab)
{
  /* Avoid recomposing split matras. */
  જો (HB_UNICODE_GENERAL_CATEGORY_IS_MARK (c->unicode->general_category (a)))
    પાછા ખોટું;

  પાછા (તાર્કિક)c->unicode->compose (a, b, ab);
}


અચલ hb_ot_shaper_t _hb_ot_shaper_use =
{
  collect_features_use,
  nullptr, /* override_features */
  data_create_use,
  data_destroy_use,
  preprocess_text_use,
  nullptr, /* postprocess_glyphs */
  nullptr, /* decompose */
  compose_use,
  setup_masks_use,
  nullptr, /* reorder_marks */
  HB_TAG_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_MODE_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT,
  HB_OT_SHAPE_ZERO_WIDTH_MARKS_BY_GDEF_EARLY,
  ખોટું, /* fallback_position */
};


#endif

/*
 * Copyright © 2011,2012,2013  Google, Inc.
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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shaper-myanmar-machine.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shaper-indic.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout.hh"


/*
 * Myanmar shaper.
 */


ਸਥਿਰ ਅਚਲ hb_ਟੈਗ_t
myanmar_basic_features[] =
{
  /*
   * Basic features.
   * These features are applied in order, one at a time, after reordering,
   * constrained to the syllable.
   */
  HB_ਟੈਗ('r','p','h','f'),
  HB_ਟੈਗ('p','r','e','f'),
  HB_ਟੈਗ('b','l','w','f'),
  HB_ਟੈਗ('p','s','t','f'),
};
ਸਥਿਰ ਅਚਲ hb_ਟੈਗ_t
myanmar_other_features[] =
{
  /*
   * Other features.
   * These features are applied all at once, after clearing syllables.
   */
  HB_ਟੈਗ('p','r','e','s'),
  HB_ਟੈਗ('a','b','v','s'),
  HB_ਟੈਗ('b','l','w','s'),
  HB_ਟੈਗ('p','s','t','s'),
};

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
ਸੈੱਟ_myanmar_properties (hb_glyph_info_t &info)
{
  hb_codepoint_t u = info.codepoint;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ type = hb_indic_get_categories (u);

  info.myanmar_category() = (myanmar_category_t) (type & 0xFFu);
}


ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
is_one_ਦਾ_myanmar (ਅਚਲ hb_glyph_info_t &info, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
  /* If it ligated, all bets are off. */
  ਜੇ (_hb_glyph_info_ligated (&info)) ਵਾਪਸ ਝੂਠ;
  ਵਾਪਸ !!(FLAG_UNSAFE (info.myanmar_category()) & flags);
}

/* Note:
 *
 * We treat Vowels and placeholders as if they were consonants.  This is safe because Vowels
 * cannot happen in a consonant syllable.  The plus side however is, we can call the
 * consonant syllable logic from the vowel syllable function and get it all right!
 *
 * Keep in sync with consonant_categories in the generator. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CONSONANT_FLAGS_MYANMAR (FLAG (M_Cat(C)) | FLAG (M_Cat(CS)) | FLAG (M_Cat(Ra)) | /* FLAG (M_Cat(CM)) | */ FLAG (M_Cat(IV)) | FLAG (M_Cat(GB)) | FLAG (M_Cat(DOTTEDCIRCLE)))

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
is_consonant_myanmar (ਅਚਲ hb_glyph_info_t &info)
{
  ਵਾਪਸ is_one_ਦਾ_myanmar (info, CONSONANT_FLAGS_MYANMAR);
}


ਸਥਿਰ ਤਾਰਕਿਕ
ਤਿਆਰੀ_syllables_myanmar (ਅਚਲ hb_ot_shape_plan_t *plan,
			 hb_font_t *font,
			 hb_ਬਫਰ_t *ਬਫਰ);
ਸਥਿਰ ਤਾਰਕਿਕ
reorder_myanmar (ਅਚਲ hb_ot_shape_plan_t *plan,
		 hb_font_t *font,
		 hb_ਬਫਰ_t *ਬਫਰ);

ਸਥਿਰ ਖਾਲੀ
collect_features_myanmar (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  /* Do this before any lookups have been applied. */
  map->ਜੋੜੋ_gsub_pause (ਤਿਆਰੀ_syllables_myanmar);

  map->enable_feature (HB_ਟੈਗ('l','o','c','l'), F_ਪ੍ਰਤੀ_SYLLABLE);
  /* The Indic specs do not require ccmp, but we apply it here since if
   * there is a use of it, it's typically at the beginning. */
  map->enable_feature (HB_ਟੈਗ('c','c','m','p'), F_ਪ੍ਰਤੀ_SYLLABLE);


  map->ਜੋੜੋ_gsub_pause (reorder_myanmar);

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ARRAY_ਲੰਮਾਈ (myanmar_basic_features); i++)
  {
    map->enable_feature (myanmar_basic_features[i], F_MANUAL_ZWJ | F_ਪ੍ਰਤੀ_SYLLABLE);
    map->ਜੋੜੋ_gsub_pause (ਖਾਲੀ_ਸੂਚਕ);
  }
  map->ਜੋੜੋ_gsub_pause (hb_syllabic_clear_var); // Don't need syllables anymore, use stop to free buffer var

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ARRAY_ਲੰਮਾਈ (myanmar_other_features); i++)
    map->enable_feature (myanmar_other_features[i], F_MANUAL_ZWJ);
}

ਸਥਿਰ ਖਾਲੀ
ਤਿਆਰੀ_masks_myanmar (ਅਚਲ hb_ot_shape_plan_t *plan HB_ਅਣਵਰਤਿਆ,
		     hb_ਬਫਰ_t              *ਬਫਰ,
		     hb_font_t                *font HB_ਅਣਵਰਤਿਆ)
{
  HB_ਬਫਰ_ALLOCATE_VAR (ਬਫਰ, myanmar_category);
  HB_ਬਫਰ_ALLOCATE_VAR (ਬਫਰ, myanmar_position);

  /* No masks, we just save information about characters. */

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਬਫਰ->len;
  hb_glyph_info_t *info = ਬਫਰ->info;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    ਸੈੱਟ_myanmar_properties (info[i]);
}

ਸਥਿਰ ਤਾਰਕਿਕ
ਤਿਆਰੀ_syllables_myanmar (ਅਚਲ hb_ot_shape_plan_t *plan HB_ਅਣਵਰਤਿਆ,
			 hb_font_t *font HB_ਅਣਵਰਤਿਆ,
			 hb_ਬਫਰ_t *ਬਫਰ)
{
  HB_ਬਫਰ_ALLOCATE_VAR (ਬਫਰ, syllable);
  ਲੱਭੋ_syllables_myanmar (ਬਫਰ);
  foreach_syllable (ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ)
    ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_break (ਸ਼ੁਰੂ, ਸਿਰਾ);
  ਵਾਪਸ ਝੂਠ;
}

ਸਥਿਰ ਪੂਰਨ
compare_myanmar_ਕ੍ਰਮ (ਅਚਲ hb_glyph_info_t *pa, ਅਚਲ hb_glyph_info_t *pb)
{
  ਪੂਰਨ a = pa->myanmar_position();
  ਪੂਰਨ b = pb->myanmar_position();

  ਵਾਪਸ (ਪੂਰਨ) a - (ਪੂਰਨ) b;
}


/* Rules from:
 * https://docs.microsoft.com/en-us/typography/script-development/myanmar */

ਸਥਿਰ ਖਾਲੀ
initial_reordering_consonant_syllable (hb_ਬਫਰ_t *ਬਫਰ,
				       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ)
{
  hb_glyph_info_t *info = ਬਫਰ->info;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅਧਾਰ = ਸਿਰਾ;
  ਤਾਰਕਿਕ has_reph = ਝੂਠ;

  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਹੱਦਬੰਦੀ = ਸ਼ੁਰੂ;
    ਜੇ (ਸ਼ੁਰੂ + 3 <= ਸਿਰਾ &&
	info[ਸ਼ੁਰੂ  ].myanmar_category() == M_Cat(Ra) &&
	info[ਸ਼ੁਰੂ+1].myanmar_category() == M_Cat(As) &&
	info[ਸ਼ੁਰੂ+2].myanmar_category() == M_Cat(H))
    {
      ਹੱਦਬੰਦੀ += 3;
      ਅਧਾਰ = ਸ਼ੁਰੂ;
      has_reph = ਸੱਚ;
    }

    {
      ਜੇ (!has_reph)
	ਅਧਾਰ = ਹੱਦਬੰਦੀ;

      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਹੱਦਬੰਦੀ; i < ਸਿਰਾ; i++)
	ਜੇ (is_consonant_myanmar (info[i]))
	{
	  ਅਧਾਰ = i;
	  ਰੋਕੋ;
	}
    }
  }

  /* Reorder! */
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ;
    ਲਈ (; i < ਸ਼ੁਰੂ + (has_reph ? 3 : 0); i++)
      info[i].myanmar_position() = POS_AFTER_MAIN;
    ਲਈ (; i < ਅਧਾਰ; i++)
      info[i].myanmar_position() = POS_PRE_C;
    ਜੇ (i < ਸਿਰਾ)
    {
      info[i].myanmar_position() = POS_ਅਧਾਰ_C;
      i++;
    }
    myanmar_position_t pos = POS_AFTER_MAIN;
    /* The following loop may be ugly, but it implements all of
     * Myanmar reordering! */
    ਲਈ (; i < ਸਿਰਾ; i++)
    {
      ਜੇ (info[i].myanmar_category() == M_Cat(MR)) /* Pre-base reordering */
      {
	info[i].myanmar_position() = POS_PRE_C;
	ਜਾਰੀ;
      }
      ਜੇ (info[i].myanmar_category() == M_Cat(VPre)) /* Left matra */
      {
	info[i].myanmar_position() = POS_PRE_M;
	ਜਾਰੀ;
      }
      ਜੇ (info[i].myanmar_category() == M_Cat(VS))
      {
	info[i].myanmar_position() = info[i - 1].myanmar_position();
	ਜਾਰੀ;
      }

      ਜੇ (pos == POS_AFTER_MAIN && info[i].myanmar_category() == M_Cat(VBlw))
      {
	pos = POS_ਹੇਠਾਂ_C;
	info[i].myanmar_position() = pos;
	ਜਾਰੀ;
      }

      ਜੇ (pos == POS_ਹੇਠਾਂ_C && info[i].myanmar_category() == M_Cat(A))
      {
	info[i].myanmar_position() = POS_BEFORE_SUB;
	ਜਾਰੀ;
      }
      ਜੇ (pos == POS_ਹੇਠਾਂ_C && info[i].myanmar_category() == M_Cat(VBlw))
      {
	info[i].myanmar_position() = pos;
	ਜਾਰੀ;
      }
      ਜੇ (pos == POS_ਹੇਠਾਂ_C && info[i].myanmar_category() != M_Cat(A))
      {
	pos = POS_AFTER_SUB;
	info[i].myanmar_position() = pos;
	ਜਾਰੀ;
      }
      info[i].myanmar_position() = pos;
    }
  }

  /* Sit tight, rock 'n roll! */
  ਬਫਰ->sort (ਸ਼ੁਰੂ, ਸਿਰਾ, compare_myanmar_ਕ੍ਰਮ);

  /* Flip left-matra sequence. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪਹਿਲਾ_ਖੱਬਾ_matra = ਸਿਰਾ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਆਖਰੀ_ਖੱਬਾ_matra = ਸਿਰਾ;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ; i < ਸਿਰਾ; i++)
  {
    ਜੇ (info[i].myanmar_position() == POS_PRE_M)
    {
      ਜੇ (ਪਹਿਲਾ_ਖੱਬਾ_matra == ਸਿਰਾ)
	ਪਹਿਲਾ_ਖੱਬਾ_matra = i;
      ਆਖਰੀ_ਖੱਬਾ_matra = i;
    }
  }
  /* https://github.com/harfbuzz/harfbuzz/issues/3863 */
  ਜੇ (ਪਹਿਲਾ_ਖੱਬਾ_matra < ਆਖਰੀ_ਖੱਬਾ_matra)
  {
    /* No need to merge clusters, done already? */
    ਬਫਰ->ਉਲਟ_range (ਪਹਿਲਾ_ਖੱਬਾ_matra, ਆਖਰੀ_ਖੱਬਾ_matra + 1);
    /* Reverse back VS, etc. */
    ਬਿਨਾਨਿਸ਼ਾਨੀ i = ਪਹਿਲਾ_ਖੱਬਾ_matra;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = i; j <= ਆਖਰੀ_ਖੱਬਾ_matra; j++)
      ਜੇ (info[j].myanmar_category() == M_Cat(VPre))
      {
	ਬਫਰ->ਉਲਟ_range (i, j + 1);
	i = j + 1;
      }
  }
}

ਸਥਿਰ ਖਾਲੀ
reorder_syllable_myanmar (ਅਚਲ hb_ot_shape_plan_t *plan HB_ਅਣਵਰਤਿਆ,
			  hb_face_t *face HB_ਅਣਵਰਤਿਆ,
			  hb_ਬਫਰ_t *ਬਫਰ,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ)
{
  myanmar_syllable_type_t syllable_type = (myanmar_syllable_type_t) (ਬਫਰ->info[ਸ਼ੁਰੂ].syllable() & 0x0F);
  ਚੋਣ (syllable_type) {

    ਹਾਲਤ myanmar_broken_cluster: /* We already inserted dotted-circles, so just call the consonant_syllable. */
    ਹਾਲਤ myanmar_consonant_syllable:
      initial_reordering_consonant_syllable  (ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ);
      ਰੋਕੋ;

    ਹਾਲਤ myanmar_non_myanmar_cluster:
      ਰੋਕੋ;
  }
}

ਸਥਿਰ ਤਾਰਕਿਕ
reorder_myanmar (ਅਚਲ hb_ot_shape_plan_t *plan,
		 hb_font_t *font,
		 hb_ਬਫਰ_t *ਬਫਰ)
{
  ਤਾਰਕਿਕ ret = ਝੂਠ;
  ਜੇ (ਬਫਰ->ਸੁਨੇਹਾ (font, "start reordering myanmar"))
  {
    ਜੇ (hb_syllabic_ਪਾਓ_dotted_circles (font, ਬਫਰ,
					   myanmar_broken_cluster,
					   M_Cat(DOTTEDCIRCLE)))
      ret = ਸੱਚ;

    foreach_syllable (ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ)
      reorder_syllable_myanmar (plan, font->face, ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ);
    (ਖਾਲੀ) ਬਫਰ->ਸੁਨੇਹਾ (font, "end reordering myanmar");
  }

  HB_ਬਫਰ_DEALLOCATE_VAR (ਬਫਰ, myanmar_category);
  HB_ਬਫਰ_DEALLOCATE_VAR (ਬਫਰ, myanmar_position);

  ਵਾਪਸ ret;
}


ਅਚਲ hb_ot_shaper_t _hb_ot_shaper_myanmar =
{
  collect_features_myanmar,
  ਖਾਲੀ_ਸੂਚਕ, /* override_features */
  ਖਾਲੀ_ਸੂਚਕ, /* data_create */
  ਖਾਲੀ_ਸੂਚਕ, /* data_destroy */
  ਖਾਲੀ_ਸੂਚਕ, /* preprocess_text */
  ਖਾਲੀ_ਸੂਚਕ, /* postprocess_glyphs */
  ਖਾਲੀ_ਸੂਚਕ, /* decompose */
  ਖਾਲੀ_ਸੂਚਕ, /* compose */
  ਤਿਆਰੀ_masks_myanmar,
  ਖਾਲੀ_ਸੂਚਕ, /* reorder_marks */
  HB_ਟੈਗ_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_ਬਹੁਲਕ_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT,
  HB_OT_SHAPE_ZERO_ਚੌੜਾਈ_MARKS_BY_GDEF_EARLY,
  ਝੂਠ, /* fallback_position */
};


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPER_MYANMAR_ZAWGYI
/* Ugly Zawgyi encoding.
 * Disable all auto processing.
 * https://github.com/harfbuzz/harfbuzz/issues/1162 */
ਅਚਲ hb_ot_shaper_t _hb_ot_shaper_myanmar_zawgyi =
{
  ਖਾਲੀ_ਸੂਚਕ, /* collect_features */
  ਖਾਲੀ_ਸੂਚਕ, /* override_features */
  ਖਾਲੀ_ਸੂਚਕ, /* data_create */
  ਖਾਲੀ_ਸੂਚਕ, /* data_destroy */
  ਖਾਲੀ_ਸੂਚਕ, /* preprocess_text */
  ਖਾਲੀ_ਸੂਚਕ, /* postprocess_glyphs */
  ਖਾਲੀ_ਸੂਚਕ, /* decompose */
  ਖਾਲੀ_ਸੂਚਕ, /* compose */
  ਖਾਲੀ_ਸੂਚਕ, /* setup_masks */
  ਖਾਲੀ_ਸੂਚਕ, /* reorder_marks */
  HB_ਟੈਗ_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_ਬਹੁਲਕ_NONE,
  HB_OT_SHAPE_ZERO_ਚੌੜਾਈ_MARKS_NONE,
  ਝੂਠ, /* fallback_position */
};
#ਜੇ_ਅੰਤ


#ਜੇ_ਅੰਤ

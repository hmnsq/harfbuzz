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

#شامل "hb.hh"

#اگر_غیر_تعریف_شدہ HB_NO_OT_SHAPE

#شامل "hb-ot-shaper-myanmar-machine.hh"
#شامل "hb-ot-shaper-indic.hh"
#شامل "hb-ot-layout.hh"


/*
 * Myanmar shaper.
 */


ساکن ثابت hb_لیبل_t
myanmar_basic_features[] =
{
  /*
   * Basic features.
   * These features are applied in order, one at a time, after reordering,
   * constrained to the syllable.
   */
  HB_لیبل('r','p','h','f'),
  HB_لیبل('p','r','e','f'),
  HB_لیبل('b','l','w','f'),
  HB_لیبل('p','s','t','f'),
};
ساکن ثابت hb_لیبل_t
myanmar_other_features[] =
{
  /*
   * Other features.
   * These features are applied all at once, after clearing syllables.
   */
  HB_لیبل('p','r','e','s'),
  HB_لیبل('a','b','v','s'),
  HB_لیبل('b','l','w','s'),
  HB_لیبل('p','s','t','s'),
};

ساکن اندرونی خالی
سیٹ_myanmar_properties (hb_glyph_info_t &info)
{
  hb_codepoint_t u = info.codepoint;
  غیرعلامتی صحیح type = hb_indic_get_categories (u);

  info.myanmar_category() = (myanmar_category_t) (type & 0xFFu);
}


ساکن اندرونی منطقی
is_one_کا_myanmar (ثابت hb_glyph_info_t &info, غیرعلامتی صحیح flags)
{
  /* If it ligated, all bets are off. */
  اگر (_hb_glyph_info_ligated (&info)) واپس جھوٹ;
  واپس !!(FLAG_UNSAFE (info.myanmar_category()) & flags);
}

/* Note:
 *
 * We treat Vowels and placeholders as if they were consonants.  This is safe because Vowels
 * cannot happen in a consonant syllable.  The plus side however is, we can call the
 * consonant syllable logic from the vowel syllable function and get it all right!
 *
 * Keep in sync with consonant_categories in the generator. */
#تعریف_کرو CONSONANT_FLAGS_MYANMAR (FLAG (M_Cat(C)) | FLAG (M_Cat(CS)) | FLAG (M_Cat(Ra)) | /* FLAG (M_Cat(CM)) | */ FLAG (M_Cat(IV)) | FLAG (M_Cat(GB)) | FLAG (M_Cat(DOTTEDCIRCLE)))

ساکن اندرونی منطقی
is_consonant_myanmar (ثابت hb_glyph_info_t &info)
{
  واپس is_one_کا_myanmar (info, CONSONANT_FLAGS_MYANMAR);
}


ساکن منطقی
تیاری_syllables_myanmar (ثابت hb_ot_shape_plan_t *plan,
			 hb_font_t *font,
			 hb_بفر_t *بفر);
ساکن منطقی
reorder_myanmar (ثابت hb_ot_shape_plan_t *plan,
		 hb_font_t *font,
		 hb_بفر_t *بفر);

ساکن خالی
collect_features_myanmar (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  /* Do this before any lookups have been applied. */
  map->جوڑو_gsub_pause (تیاری_syllables_myanmar);

  map->enable_feature (HB_لیبل('l','o','c','l'), F_فی_SYLLABLE);
  /* The Indic specs do not require ccmp, but we apply it here since if
   * there is a use of it, it's typically at the beginning. */
  map->enable_feature (HB_لیبل('c','c','m','p'), F_فی_SYLLABLE);


  map->جوڑو_gsub_pause (reorder_myanmar);

  برائے (غیرعلامتی صحیح i = 0; i < ARRAY_لمبائی (myanmar_basic_features); i++)
  {
    map->enable_feature (myanmar_basic_features[i], F_MANUAL_ZWJ | F_فی_SYLLABLE);
    map->جوڑو_gsub_pause (خالی_اشاریہ);
  }
  map->جوڑو_gsub_pause (hb_syllabic_clear_var); // Don't need syllables anymore, use stop to free buffer var

  برائے (غیرعلامتی صحیح i = 0; i < ARRAY_لمبائی (myanmar_other_features); i++)
    map->enable_feature (myanmar_other_features[i], F_MANUAL_ZWJ);
}

ساکن خالی
تیاری_masks_myanmar (ثابت hb_ot_shape_plan_t *plan HB_غیر_مستعمل,
		     hb_بفر_t              *بفر,
		     hb_font_t                *font HB_غیر_مستعمل)
{
  HB_بفر_ALLOCATE_VAR (بفر, myanmar_category);
  HB_بفر_ALLOCATE_VAR (بفر, myanmar_position);

  /* No masks, we just save information about characters. */

  غیرعلامتی صحیح گنتی = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    سیٹ_myanmar_properties (info[i]);
}

ساکن منطقی
تیاری_syllables_myanmar (ثابت hb_ot_shape_plan_t *plan HB_غیر_مستعمل,
			 hb_font_t *font HB_غیر_مستعمل,
			 hb_بفر_t *بفر)
{
  HB_بفر_ALLOCATE_VAR (بفر, syllable);
  ڈھونڈو_syllables_myanmar (بفر);
  foreach_syllable (بفر, آغاز, انتہا)
    بفر->unsafe_مقصد_break (آغاز, انتہا);
  واپس جھوٹ;
}

ساکن صحیح
compare_myanmar_ترتیب (ثابت hb_glyph_info_t *pa, ثابت hb_glyph_info_t *pb)
{
  صحیح a = pa->myanmar_position();
  صحیح b = pb->myanmar_position();

  واپس (صحیح) a - (صحیح) b;
}


/* Rules from:
 * https://docs.microsoft.com/en-us/typography/script-development/myanmar */

ساکن خالی
initial_reordering_consonant_syllable (hb_بفر_t *بفر,
				       غیرعلامتی صحیح آغاز, غیرعلامتی صحیح انتہا)
{
  hb_glyph_info_t *info = بفر->info;

  غیرعلامتی صحیح بنیاد = انتہا;
  منطقی has_reph = جھوٹ;

  {
    غیرعلامتی صحیح حد_بندی = آغاز;
    اگر (آغاز + 3 <= انتہا &&
	info[آغاز  ].myanmar_category() == M_Cat(Ra) &&
	info[آغاز+1].myanmar_category() == M_Cat(As) &&
	info[آغاز+2].myanmar_category() == M_Cat(H))
    {
      حد_بندی += 3;
      بنیاد = آغاز;
      has_reph = سچ;
    }

    {
      اگر (!has_reph)
	بنیاد = حد_بندی;

      برائے (غیرعلامتی صحیح i = حد_بندی; i < انتہا; i++)
	اگر (is_consonant_myanmar (info[i]))
	{
	  بنیاد = i;
	  توڑو;
	}
    }
  }

  /* Reorder! */
  {
    غیرعلامتی صحیح i = آغاز;
    برائے (; i < آغاز + (has_reph ? 3 : 0); i++)
      info[i].myanmar_position() = POS_AFTER_MAIN;
    برائے (; i < بنیاد; i++)
      info[i].myanmar_position() = POS_PRE_C;
    اگر (i < انتہا)
    {
      info[i].myanmar_position() = POS_بنیاد_C;
      i++;
    }
    myanmar_position_t pos = POS_AFTER_MAIN;
    /* The following loop may be ugly, but it implements all of
     * Myanmar reordering! */
    برائے (; i < انتہا; i++)
    {
      اگر (info[i].myanmar_category() == M_Cat(MR)) /* Pre-base reordering */
      {
	info[i].myanmar_position() = POS_PRE_C;
	جاری;
      }
      اگر (info[i].myanmar_category() == M_Cat(VPre)) /* Left matra */
      {
	info[i].myanmar_position() = POS_PRE_M;
	جاری;
      }
      اگر (info[i].myanmar_category() == M_Cat(VS))
      {
	info[i].myanmar_position() = info[i - 1].myanmar_position();
	جاری;
      }

      اگر (pos == POS_AFTER_MAIN && info[i].myanmar_category() == M_Cat(VBlw))
      {
	pos = POS_نیچے_C;
	info[i].myanmar_position() = pos;
	جاری;
      }

      اگر (pos == POS_نیچے_C && info[i].myanmar_category() == M_Cat(A))
      {
	info[i].myanmar_position() = POS_BEFORE_SUB;
	جاری;
      }
      اگر (pos == POS_نیچے_C && info[i].myanmar_category() == M_Cat(VBlw))
      {
	info[i].myanmar_position() = pos;
	جاری;
      }
      اگر (pos == POS_نیچے_C && info[i].myanmar_category() != M_Cat(A))
      {
	pos = POS_AFTER_SUB;
	info[i].myanmar_position() = pos;
	جاری;
      }
      info[i].myanmar_position() = pos;
    }
  }

  /* Sit tight, rock 'n roll! */
  بفر->sort (آغاز, انتہا, compare_myanmar_ترتیب);

  /* Flip left-matra sequence. */
  غیرعلامتی پہلا_بایاں_matra = انتہا;
  غیرعلامتی آخری_بایاں_matra = انتہا;
  برائے (غیرعلامتی صحیح i = آغاز; i < انتہا; i++)
  {
    اگر (info[i].myanmar_position() == POS_PRE_M)
    {
      اگر (پہلا_بایاں_matra == انتہا)
	پہلا_بایاں_matra = i;
      آخری_بایاں_matra = i;
    }
  }
  /* https://github.com/harfbuzz/harfbuzz/issues/3863 */
  اگر (پہلا_بایاں_matra < آخری_بایاں_matra)
  {
    /* No need to merge clusters, done already? */
    بفر->الٹ_range (پہلا_بایاں_matra, آخری_بایاں_matra + 1);
    /* Reverse back VS, etc. */
    غیرعلامتی i = پہلا_بایاں_matra;
    برائے (غیرعلامتی j = i; j <= آخری_بایاں_matra; j++)
      اگر (info[j].myanmar_category() == M_Cat(VPre))
      {
	بفر->الٹ_range (i, j + 1);
	i = j + 1;
      }
  }
}

ساکن خالی
reorder_syllable_myanmar (ثابت hb_ot_shape_plan_t *plan HB_غیر_مستعمل,
			  hb_face_t *face HB_غیر_مستعمل,
			  hb_بفر_t *بفر,
			  غیرعلامتی صحیح آغاز, غیرعلامتی صحیح انتہا)
{
  myanmar_syllable_type_t syllable_type = (myanmar_syllable_type_t) (بفر->info[آغاز].syllable() & 0x0F);
  انتخاب (syllable_type) {

    حالت myanmar_broken_cluster: /* We already inserted dotted-circles, so just call the consonant_syllable. */
    حالت myanmar_consonant_syllable:
      initial_reordering_consonant_syllable  (بفر, آغاز, انتہا);
      توڑو;

    حالت myanmar_non_myanmar_cluster:
      توڑو;
  }
}

ساکن منطقی
reorder_myanmar (ثابت hb_ot_shape_plan_t *plan,
		 hb_font_t *font,
		 hb_بفر_t *بفر)
{
  منطقی ret = جھوٹ;
  اگر (بفر->پیغام (font, "start reordering myanmar"))
  {
    اگر (hb_syllabic_داخل_کرو_dotted_circles (font, بفر,
					   myanmar_broken_cluster,
					   M_Cat(DOTTEDCIRCLE)))
      ret = سچ;

    foreach_syllable (بفر, آغاز, انتہا)
      reorder_syllable_myanmar (plan, font->face, بفر, آغاز, انتہا);
    (خالی) بفر->پیغام (font, "end reordering myanmar");
  }

  HB_بفر_DEALLOCATE_VAR (بفر, myanmar_category);
  HB_بفر_DEALLOCATE_VAR (بفر, myanmar_position);

  واپس ret;
}


ثابت hb_ot_shaper_t _hb_ot_shaper_myanmar =
{
  collect_features_myanmar,
  خالی_اشاریہ, /* override_features */
  خالی_اشاریہ, /* data_create */
  خالی_اشاریہ, /* data_destroy */
  خالی_اشاریہ, /* preprocess_text */
  خالی_اشاریہ, /* postprocess_glyphs */
  خالی_اشاریہ, /* decompose */
  خالی_اشاریہ, /* compose */
  تیاری_masks_myanmar,
  خالی_اشاریہ, /* reorder_marks */
  HB_لیبل_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_منوال_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT,
  HB_OT_SHAPE_ZERO_چوڑائی_MARKS_BY_GDEF_EARLY,
  جھوٹ, /* fallback_position */
};


#اگر_غیر_تعریف_شدہ HB_NO_OT_SHAPER_MYANMAR_ZAWGYI
/* Ugly Zawgyi encoding.
 * Disable all auto processing.
 * https://github.com/harfbuzz/harfbuzz/issues/1162 */
ثابت hb_ot_shaper_t _hb_ot_shaper_myanmar_zawgyi =
{
  خالی_اشاریہ, /* collect_features */
  خالی_اشاریہ, /* override_features */
  خالی_اشاریہ, /* data_create */
  خالی_اشاریہ, /* data_destroy */
  خالی_اشاریہ, /* preprocess_text */
  خالی_اشاریہ, /* postprocess_glyphs */
  خالی_اشاریہ, /* decompose */
  خالی_اشاریہ, /* compose */
  خالی_اشاریہ, /* setup_masks */
  خالی_اشاریہ, /* reorder_marks */
  HB_لیبل_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_منوال_NONE,
  HB_OT_SHAPE_ZERO_چوڑائی_MARKS_NONE,
  جھوٹ, /* fallback_position */
};
#اگر_ختم


#اگر_ختم

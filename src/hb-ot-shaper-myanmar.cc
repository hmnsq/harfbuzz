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

#ضمن "hb.hh"

#که_نه_تعریف_شوی HB_NO_OT_SHAPE

#ضمن "hb-ot-shaper-myanmar-machine.hh"
#ضمن "hb-ot-shaper-indic.hh"
#ضمن "hb-ot-layout.hh"


/*
 * Myanmar shaper.
 */


ساكن ثابت hb_نښان_t
myanmar_basic_features[] =
{
  /*
   * Basic features.
   * These features are applied in order, one at a time, after reordering,
   * constrained to the syllable.
   */
  HB_نښان('r','p','h','f'),
  HB_نښان('p','r','e','f'),
  HB_نښان('b','l','w','f'),
  HB_نښان('p','s','t','f'),
};
ساكن ثابت hb_نښان_t
myanmar_other_features[] =
{
  /*
   * Other features.
   * These features are applied all at once, after clearing syllables.
   */
  HB_نښان('p','r','e','s'),
  HB_نښان('a','b','v','s'),
  HB_نښان('b','l','w','s'),
  HB_نښان('p','s','t','s'),
};

ساكن مضمن فراغ
سېټ_myanmar_properties (hb_glyph_info_t &info)
{
  hb_codepoint_t u = info.codepoint;
  بینښه صحيح type = hb_indic_get_categories (u);

  info.myanmar_category() = (myanmar_category_t) (type & 0xFFu);
}


ساكن مضمن منطقي
is_one_د_myanmar (ثابت hb_glyph_info_t &info, بینښه صحيح flags)
{
  /* If it ligated, all bets are off. */
  إذا (_hb_glyph_info_ligated (&info)) أرجع خطأ;
  أرجع !!(FLAG_UNSAFE (info.myanmar_category()) & flags);
}

/* Note:
 *
 * We treat Vowels and placeholders as if they were consonants.  This is safe because Vowels
 * cannot happen in a consonant syllable.  The plus side however is, we can call the
 * consonant syllable logic from the vowel syllable function and get it all right!
 *
 * Keep in sync with consonant_categories in the generator. */
#تعریف_کړه CONSONANT_FLAGS_MYANMAR (FLAG (M_Cat(C)) | FLAG (M_Cat(CS)) | FLAG (M_Cat(Ra)) | /* FLAG (M_Cat(CM)) | */ FLAG (M_Cat(IV)) | FLAG (M_Cat(GB)) | FLAG (M_Cat(DOTTEDCIRCLE)))

ساكن مضمن منطقي
is_consonant_myanmar (ثابت hb_glyph_info_t &info)
{
  أرجع is_one_د_myanmar (info, CONSONANT_FLAGS_MYANMAR);
}


ساكن منطقي
تیاری_syllables_myanmar (ثابت hb_ot_shape_plan_t *plan,
			 hb_font_t *font,
			 hb_بفر_t *بفر);
ساكن منطقي
reorder_myanmar (ثابت hb_ot_shape_plan_t *plan,
		 hb_font_t *font,
		 hb_بفر_t *بفر);

ساكن فراغ
collect_features_myanmar (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  /* Do this before any lookups have been applied. */
  map->جمع_کړه_gsub_pause (تیاری_syllables_myanmar);

  map->enable_feature (HB_نښان('l','o','c','l'), F_فی_SYLLABLE);
  /* The Indic specs do not require ccmp, but we apply it here since if
   * there is a use of it, it's typically at the beginning. */
  map->enable_feature (HB_نښان('c','c','m','p'), F_فی_SYLLABLE);


  map->جمع_کړه_gsub_pause (reorder_myanmar);

  لكل (بینښه صحيح i = 0; i < ARRAY_اوږدتیا (myanmar_basic_features); i++)
  {
    map->enable_feature (myanmar_basic_features[i], F_MANUAL_ZWJ | F_فی_SYLLABLE);
    map->جمع_کړه_gsub_pause (تش_اشاره);
  }
  map->جمع_کړه_gsub_pause (hb_syllabic_clear_var); // Don't need syllables anymore, use stop to free buffer var

  لكل (بینښه صحيح i = 0; i < ARRAY_اوږدتیا (myanmar_other_features); i++)
    map->enable_feature (myanmar_other_features[i], F_MANUAL_ZWJ);
}

ساكن فراغ
تیاری_masks_myanmar (ثابت hb_ot_shape_plan_t *plan HB_ناکارول_شوی,
		     hb_بفر_t              *بفر,
		     hb_font_t                *font HB_ناکارول_شوی)
{
  HB_بفر_ALLOCATE_VAR (بفر, myanmar_category);
  HB_بفر_ALLOCATE_VAR (بفر, myanmar_position);

  /* No masks, we just save information about characters. */

  بینښه صحيح شمېر = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  لكل (بینښه صحيح i = 0; i < شمېر; i++)
    سېټ_myanmar_properties (info[i]);
}

ساكن منطقي
تیاری_syllables_myanmar (ثابت hb_ot_shape_plan_t *plan HB_ناکارول_شوی,
			 hb_font_t *font HB_ناکارول_شوی,
			 hb_بفر_t *بفر)
{
  HB_بفر_ALLOCATE_VAR (بفر, syllable);
  پیدا_کړه_syllables_myanmar (بفر);
  foreach_syllable (بفر, پیل, پای_ټکی)
    بفر->unsafe_منزل_break (پیل, پای_ټکی);
  أرجع خطأ;
}

ساكن صحيح
compare_myanmar_ترتیب (ثابت hb_glyph_info_t *pa, ثابت hb_glyph_info_t *pb)
{
  صحيح a = pa->myanmar_position();
  صحيح b = pb->myanmar_position();

  أرجع (صحيح) a - (صحيح) b;
}


/* Rules from:
 * https://docs.microsoft.com/en-us/typography/script-development/myanmar */

ساكن فراغ
initial_reordering_consonant_syllable (hb_بفر_t *بفر,
				       بینښه صحيح پیل, بینښه صحيح پای_ټکی)
{
  hb_glyph_info_t *info = بفر->info;

  بینښه صحيح بنسټ = پای_ټکی;
  منطقي has_reph = خطأ;

  {
    بینښه صحيح حد = پیل;
    إذا (پیل + 3 <= پای_ټکی &&
	info[پیل  ].myanmar_category() == M_Cat(Ra) &&
	info[پیل+1].myanmar_category() == M_Cat(As) &&
	info[پیل+2].myanmar_category() == M_Cat(H))
    {
      حد += 3;
      بنسټ = پیل;
      has_reph = ریښتیا;
    }

    {
      إذا (!has_reph)
	بنسټ = حد;

      لكل (بینښه صحيح i = حد; i < پای_ټکی; i++)
	إذا (is_consonant_myanmar (info[i]))
	{
	  بنسټ = i;
	  اكسر;
	}
    }
  }

  /* Reorder! */
  {
    بینښه صحيح i = پیل;
    لكل (; i < پیل + (has_reph ? 3 : 0); i++)
      info[i].myanmar_position() = POS_AFTER_MAIN;
    لكل (; i < بنسټ; i++)
      info[i].myanmar_position() = POS_PRE_C;
    إذا (i < پای_ټکی)
    {
      info[i].myanmar_position() = POS_بنسټ_C;
      i++;
    }
    myanmar_position_t pos = POS_AFTER_MAIN;
    /* The following loop may be ugly, but it implements all of
     * Myanmar reordering! */
    لكل (; i < پای_ټکی; i++)
    {
      إذا (info[i].myanmar_category() == M_Cat(MR)) /* Pre-base reordering */
      {
	info[i].myanmar_position() = POS_PRE_C;
	استمر;
      }
      إذا (info[i].myanmar_category() == M_Cat(VPre)) /* Left matra */
      {
	info[i].myanmar_position() = POS_PRE_M;
	استمر;
      }
      إذا (info[i].myanmar_category() == M_Cat(VS))
      {
	info[i].myanmar_position() = info[i - 1].myanmar_position();
	استمر;
      }

      إذا (pos == POS_AFTER_MAIN && info[i].myanmar_category() == M_Cat(VBlw))
      {
	pos = POS_لاندې_C;
	info[i].myanmar_position() = pos;
	استمر;
      }

      إذا (pos == POS_لاندې_C && info[i].myanmar_category() == M_Cat(A))
      {
	info[i].myanmar_position() = POS_BEFORE_SUB;
	استمر;
      }
      إذا (pos == POS_لاندې_C && info[i].myanmar_category() == M_Cat(VBlw))
      {
	info[i].myanmar_position() = pos;
	استمر;
      }
      إذا (pos == POS_لاندې_C && info[i].myanmar_category() != M_Cat(A))
      {
	pos = POS_AFTER_SUB;
	info[i].myanmar_position() = pos;
	استمر;
      }
      info[i].myanmar_position() = pos;
    }
  }

  /* Sit tight, rock 'n roll! */
  بفر->sort (پیل, پای_ټکی, compare_myanmar_ترتیب);

  /* Flip left-matra sequence. */
  بینښه لومړی_کیڼ_matra = پای_ټکی;
  بینښه وروستی_کیڼ_matra = پای_ټکی;
  لكل (بینښه صحيح i = پیل; i < پای_ټکی; i++)
  {
    إذا (info[i].myanmar_position() == POS_PRE_M)
    {
      إذا (لومړی_کیڼ_matra == پای_ټکی)
	لومړی_کیڼ_matra = i;
      وروستی_کیڼ_matra = i;
    }
  }
  /* https://github.com/harfbuzz/harfbuzz/issues/3863 */
  إذا (لومړی_کیڼ_matra < وروستی_کیڼ_matra)
  {
    /* No need to merge clusters, done already? */
    بفر->سرچپه_range (لومړی_کیڼ_matra, وروستی_کیڼ_matra + 1);
    /* Reverse back VS, etc. */
    بینښه i = لومړی_کیڼ_matra;
    لكل (بینښه j = i; j <= وروستی_کیڼ_matra; j++)
      إذا (info[j].myanmar_category() == M_Cat(VPre))
      {
	بفر->سرچپه_range (i, j + 1);
	i = j + 1;
      }
  }
}

ساكن فراغ
reorder_syllable_myanmar (ثابت hb_ot_shape_plan_t *plan HB_ناکارول_شوی,
			  hb_face_t *face HB_ناکارول_شوی,
			  hb_بفر_t *بفر,
			  بینښه صحيح پیل, بینښه صحيح پای_ټکی)
{
  myanmar_syllable_type_t syllable_type = (myanmar_syllable_type_t) (بفر->info[پیل].syllable() & 0x0F);
  بدل (syllable_type) {

    حالة myanmar_broken_cluster: /* We already inserted dotted-circles, so just call the consonant_syllable. */
    حالة myanmar_consonant_syllable:
      initial_reordering_consonant_syllable  (بفر, پیل, پای_ټکی);
      اكسر;

    حالة myanmar_non_myanmar_cluster:
      اكسر;
  }
}

ساكن منطقي
reorder_myanmar (ثابت hb_ot_shape_plan_t *plan,
		 hb_font_t *font,
		 hb_بفر_t *بفر)
{
  منطقي ret = خطأ;
  إذا (بفر->پیغام (font, "start reordering myanmar"))
  {
    إذا (hb_syllabic_ورننباسه_dotted_circles (font, بفر,
					   myanmar_broken_cluster,
					   M_Cat(DOTTEDCIRCLE)))
      ret = ریښتیا;

    foreach_syllable (بفر, پیل, پای_ټکی)
      reorder_syllable_myanmar (plan, font->face, بفر, پیل, پای_ټکی);
    (فراغ) بفر->پیغام (font, "end reordering myanmar");
  }

  HB_بفر_DEALLOCATE_VAR (بفر, myanmar_category);
  HB_بفر_DEALLOCATE_VAR (بفر, myanmar_position);

  أرجع ret;
}


ثابت hb_ot_shaper_t _hb_ot_shaper_myanmar =
{
  collect_features_myanmar,
  تش_اشاره, /* override_features */
  تش_اشاره, /* data_create */
  تش_اشاره, /* data_destroy */
  تش_اشاره, /* preprocess_text */
  تش_اشاره, /* postprocess_glyphs */
  تش_اشاره, /* decompose */
  تش_اشاره, /* compose */
  تیاری_masks_myanmar,
  تش_اشاره, /* reorder_marks */
  HB_نښان_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_ډېرپېښ_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT,
  HB_OT_SHAPE_ZERO_پلنوالی_MARKS_BY_GDEF_EARLY,
  خطأ, /* fallback_position */
};


#که_نه_تعریف_شوی HB_NO_OT_SHAPER_MYANMAR_ZAWGYI
/* Ugly Zawgyi encoding.
 * Disable all auto processing.
 * https://github.com/harfbuzz/harfbuzz/issues/1162 */
ثابت hb_ot_shaper_t _hb_ot_shaper_myanmar_zawgyi =
{
  تش_اشاره, /* collect_features */
  تش_اشاره, /* override_features */
  تش_اشاره, /* data_create */
  تش_اشاره, /* data_destroy */
  تش_اشاره, /* preprocess_text */
  تش_اشاره, /* postprocess_glyphs */
  تش_اشاره, /* decompose */
  تش_اشاره, /* compose */
  تش_اشاره, /* setup_masks */
  تش_اشاره, /* reorder_marks */
  HB_نښان_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_ډېرپېښ_NONE,
  HB_OT_SHAPE_ZERO_پلنوالی_MARKS_NONE,
  خطأ, /* fallback_position */
};
#د_که_پای


#د_که_پای

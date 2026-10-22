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

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_SHAPE

#ضمن "hb-ot-shaper-myanmar-machine.hh"
#ضمن "hb-ot-shaper-indic.hh"
#ضمن "hb-ot-layout.hh"


/*
 * Myanmar shaper.
 */


ساكن ثابت hb_ٽيگ_t
myanmar_basic_features[] =
{
  /*
   * Basic features.
   * These features are applied in order, one at a time, after reordering,
   * constrained to the syllable.
   */
  HB_ٽيگ('r','p','h','f'),
  HB_ٽيگ('p','r','e','f'),
  HB_ٽيگ('b','l','w','f'),
  HB_ٽيگ('p','s','t','f'),
};
ساكن ثابت hb_ٽيگ_t
myanmar_other_features[] =
{
  /*
   * Other features.
   * These features are applied all at once, after clearing syllables.
   */
  HB_ٽيگ('p','r','e','s'),
  HB_ٽيگ('a','b','v','s'),
  HB_ٽيگ('b','l','w','s'),
  HB_ٽيگ('p','s','t','s'),
};

ساكن مضمن فراغ
سيٽ_myanmar_properties (hb_glyph_info_t &info)
{
  hb_codepoint_t u = info.codepoint;
  بنانشان صحيح type = hb_indic_get_categories (u);

  info.myanmar_category() = (myanmar_category_t) (type & 0xFFu);
}


ساكن مضمن منطقي
is_one_جو_myanmar (ثابت hb_glyph_info_t &info, بنانشان صحيح flags)
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
#وصف_ڪريو CONSONANT_FLAGS_MYANMAR (FLAG (M_Cat(C)) | FLAG (M_Cat(CS)) | FLAG (M_Cat(Ra)) | /* FLAG (M_Cat(CM)) | */ FLAG (M_Cat(IV)) | FLAG (M_Cat(GB)) | FLAG (M_Cat(DOTTEDCIRCLE)))

ساكن مضمن منطقي
is_consonant_myanmar (ثابت hb_glyph_info_t &info)
{
  أرجع is_one_جو_myanmar (info, CONSONANT_FLAGS_MYANMAR);
}


ساكن منطقي
تياري_syllables_myanmar (ثابت hb_ot_shape_plan_t *plan,
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
  map->جوڙيو_gsub_pause (تياري_syllables_myanmar);

  map->enable_feature (HB_ٽيگ('l','o','c','l'), F_في_SYLLABLE);
  /* The Indic specs do not require ccmp, but we apply it here since if
   * there is a use of it, it's typically at the beginning. */
  map->enable_feature (HB_ٽيگ('c','c','m','p'), F_في_SYLLABLE);


  map->جوڙيو_gsub_pause (reorder_myanmar);

  لكل (بنانشان صحيح i = 0; i < ARRAY_ڊيگهائي (myanmar_basic_features); i++)
  {
    map->enable_feature (myanmar_basic_features[i], F_MANUAL_ZWJ | F_في_SYLLABLE);
    map->جوڙيو_gsub_pause (خالي_اشارو);
  }
  map->جوڙيو_gsub_pause (hb_syllabic_clear_var); // Don't need syllables anymore, use stop to free buffer var

  لكل (بنانشان صحيح i = 0; i < ARRAY_ڊيگهائي (myanmar_other_features); i++)
    map->enable_feature (myanmar_other_features[i], F_MANUAL_ZWJ);
}

ساكن فراغ
تياري_masks_myanmar (ثابت hb_ot_shape_plan_t *plan HB_اڻ_استعمال_ٿيل,
		     hb_بفر_t              *بفر,
		     hb_font_t                *font HB_اڻ_استعمال_ٿيل)
{
  HB_بفر_ALLOCATE_VAR (بفر, myanmar_category);
  HB_بفر_ALLOCATE_VAR (بفر, myanmar_position);

  /* No masks, we just save information about characters. */

  بنانشان صحيح ڳڻپ = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    سيٽ_myanmar_properties (info[i]);
}

ساكن منطقي
تياري_syllables_myanmar (ثابت hb_ot_shape_plan_t *plan HB_اڻ_استعمال_ٿيل,
			 hb_font_t *font HB_اڻ_استعمال_ٿيل,
			 hb_بفر_t *بفر)
{
  HB_بفر_ALLOCATE_VAR (بفر, syllable);
  ڳوليو_syllables_myanmar (بفر);
  foreach_syllable (بفر, شروعات, پڇاڙي)
    بفر->unsafe_منزل_break (شروعات, پڇاڙي);
  أرجع خطأ;
}

ساكن صحيح
compare_myanmar_ترتيب (ثابت hb_glyph_info_t *pa, ثابت hb_glyph_info_t *pb)
{
  صحيح a = pa->myanmar_position();
  صحيح b = pb->myanmar_position();

  أرجع (صحيح) a - (صحيح) b;
}


/* Rules from:
 * https://docs.microsoft.com/en-us/typography/script-development/myanmar */

ساكن فراغ
initial_reordering_consonant_syllable (hb_بفر_t *بفر,
				       بنانشان صحيح شروعات, بنانشان صحيح پڇاڙي)
{
  hb_glyph_info_t *info = بفر->info;

  بنانشان صحيح بنياد = پڇاڙي;
  منطقي has_reph = خطأ;

  {
    بنانشان صحيح حد_بندي = شروعات;
    إذا (شروعات + 3 <= پڇاڙي &&
	info[شروعات  ].myanmar_category() == M_Cat(Ra) &&
	info[شروعات+1].myanmar_category() == M_Cat(As) &&
	info[شروعات+2].myanmar_category() == M_Cat(H))
    {
      حد_بندي += 3;
      بنياد = شروعات;
      has_reph = سچ;
    }

    {
      إذا (!has_reph)
	بنياد = حد_بندي;

      لكل (بنانشان صحيح i = حد_بندي; i < پڇاڙي; i++)
	إذا (is_consonant_myanmar (info[i]))
	{
	  بنياد = i;
	  اكسر;
	}
    }
  }

  /* Reorder! */
  {
    بنانشان صحيح i = شروعات;
    لكل (; i < شروعات + (has_reph ? 3 : 0); i++)
      info[i].myanmar_position() = POS_AFTER_MAIN;
    لكل (; i < بنياد; i++)
      info[i].myanmar_position() = POS_PRE_C;
    إذا (i < پڇاڙي)
    {
      info[i].myanmar_position() = POS_بنياد_C;
      i++;
    }
    myanmar_position_t pos = POS_AFTER_MAIN;
    /* The following loop may be ugly, but it implements all of
     * Myanmar reordering! */
    لكل (; i < پڇاڙي; i++)
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
	pos = POS_هيٺ_C;
	info[i].myanmar_position() = pos;
	استمر;
      }

      إذا (pos == POS_هيٺ_C && info[i].myanmar_category() == M_Cat(A))
      {
	info[i].myanmar_position() = POS_BEFORE_SUB;
	استمر;
      }
      إذا (pos == POS_هيٺ_C && info[i].myanmar_category() == M_Cat(VBlw))
      {
	info[i].myanmar_position() = pos;
	استمر;
      }
      إذا (pos == POS_هيٺ_C && info[i].myanmar_category() != M_Cat(A))
      {
	pos = POS_AFTER_SUB;
	info[i].myanmar_position() = pos;
	استمر;
      }
      info[i].myanmar_position() = pos;
    }
  }

  /* Sit tight, rock 'n roll! */
  بفر->sort (شروعات, پڇاڙي, compare_myanmar_ترتيب);

  /* Flip left-matra sequence. */
  بنانشان پهريون_کاٻو_matra = پڇاڙي;
  بنانشان آخري_کاٻو_matra = پڇاڙي;
  لكل (بنانشان صحيح i = شروعات; i < پڇاڙي; i++)
  {
    إذا (info[i].myanmar_position() == POS_PRE_M)
    {
      إذا (پهريون_کاٻو_matra == پڇاڙي)
	پهريون_کاٻو_matra = i;
      آخري_کاٻو_matra = i;
    }
  }
  /* https://github.com/harfbuzz/harfbuzz/issues/3863 */
  إذا (پهريون_کاٻو_matra < آخري_کاٻو_matra)
  {
    /* No need to merge clusters, done already? */
    بفر->ابتو_range (پهريون_کاٻو_matra, آخري_کاٻو_matra + 1);
    /* Reverse back VS, etc. */
    بنانشان i = پهريون_کاٻو_matra;
    لكل (بنانشان j = i; j <= آخري_کاٻو_matra; j++)
      إذا (info[j].myanmar_category() == M_Cat(VPre))
      {
	بفر->ابتو_range (i, j + 1);
	i = j + 1;
      }
  }
}

ساكن فراغ
reorder_syllable_myanmar (ثابت hb_ot_shape_plan_t *plan HB_اڻ_استعمال_ٿيل,
			  hb_face_t *face HB_اڻ_استعمال_ٿيل,
			  hb_بفر_t *بفر,
			  بنانشان صحيح شروعات, بنانشان صحيح پڇاڙي)
{
  myanmar_syllable_type_t syllable_type = (myanmar_syllable_type_t) (بفر->info[شروعات].syllable() & 0x0F);
  بدل (syllable_type) {

    حالة myanmar_broken_cluster: /* We already inserted dotted-circles, so just call the consonant_syllable. */
    حالة myanmar_consonant_syllable:
      initial_reordering_consonant_syllable  (بفر, شروعات, پڇاڙي);
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
  إذا (بفر->پيغام (font, "start reordering myanmar"))
  {
    إذا (hb_syllabic_وجهو_dotted_circles (font, بفر,
					   myanmar_broken_cluster,
					   M_Cat(DOTTEDCIRCLE)))
      ret = سچ;

    foreach_syllable (بفر, شروعات, پڇاڙي)
      reorder_syllable_myanmar (plan, font->face, بفر, شروعات, پڇاڙي);
    (فراغ) بفر->پيغام (font, "end reordering myanmar");
  }

  HB_بفر_DEALLOCATE_VAR (بفر, myanmar_category);
  HB_بفر_DEALLOCATE_VAR (بفر, myanmar_position);

  أرجع ret;
}


ثابت hb_ot_shaper_t _hb_ot_shaper_myanmar =
{
  collect_features_myanmar,
  خالي_اشارو, /* override_features */
  خالي_اشارو, /* data_create */
  خالي_اشارو, /* data_destroy */
  خالي_اشارو, /* preprocess_text */
  خالي_اشارو, /* postprocess_glyphs */
  خالي_اشارو, /* decompose */
  خالي_اشارو, /* compose */
  تياري_masks_myanmar,
  خالي_اشارو, /* reorder_marks */
  HB_ٽيگ_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_گهڻائي_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT,
  HB_OT_SHAPE_ZERO_ويڪر_MARKS_BY_GDEF_EARLY,
  خطأ, /* fallback_position */
};


#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_SHAPER_MYANMAR_ZAWGYI
/* Ugly Zawgyi encoding.
 * Disable all auto processing.
 * https://github.com/harfbuzz/harfbuzz/issues/1162 */
ثابت hb_ot_shaper_t _hb_ot_shaper_myanmar_zawgyi =
{
  خالي_اشارو, /* collect_features */
  خالي_اشارو, /* override_features */
  خالي_اشارو, /* data_create */
  خالي_اشارو, /* data_destroy */
  خالي_اشارو, /* preprocess_text */
  خالي_اشارو, /* postprocess_glyphs */
  خالي_اشارو, /* decompose */
  خالي_اشارو, /* compose */
  خالي_اشارو, /* setup_masks */
  خالي_اشارو, /* reorder_marks */
  HB_ٽيگ_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_گهڻائي_NONE,
  HB_OT_SHAPE_ZERO_ويڪر_MARKS_NONE,
  خطأ, /* fallback_position */
};
#جيڪڏهن_پورو


#جيڪڏهن_پورو

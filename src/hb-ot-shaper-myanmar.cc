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

#جے_بنا_تعریف HB_NO_OT_SHAPE

#ضمن "hb-ot-shaper-myanmar-machine.hh"
#ضمن "hb-ot-shaper-indic.hh"
#ضمن "hb-ot-layout.hh"


/*
 * Myanmar shaper.
 */


ساكن ثابت hb_ٹیگ_t
myanmar_basic_features[] =
{
  /*
   * Basic features.
   * These features are applied in order, one at a time, after reordering,
   * constrained to the syllable.
   */
  HB_ٹیگ('r','p','h','f'),
  HB_ٹیگ('p','r','e','f'),
  HB_ٹیگ('b','l','w','f'),
  HB_ٹیگ('p','s','t','f'),
};
ساكن ثابت hb_ٹیگ_t
myanmar_other_features[] =
{
  /*
   * Other features.
   * These features are applied all at once, after clearing syllables.
   */
  HB_ٹیگ('p','r','e','s'),
  HB_ٹیگ('a','b','v','s'),
  HB_ٹیگ('b','l','w','s'),
  HB_ٹیگ('p','s','t','s'),
};

ساكن مضمن فراغ
سیٹ_myanmar_properties (hb_glyph_info_t &info)
{
  hb_codepoint_t u = info.codepoint;
  بنانشانی صحيح type = hb_indic_get_categories (u);

  info.myanmar_category() = (myanmar_category_t) (type & 0xFFu);
}


ساكن مضمن منطقي
is_one_دا_myanmar (ثابت hb_glyph_info_t &info, بنانشانی صحيح flags)
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
#تعریف_کرو CONSONANT_FLAGS_MYANMAR (FLAG (M_Cat(C)) | FLAG (M_Cat(CS)) | FLAG (M_Cat(Ra)) | /* FLAG (M_Cat(CM)) | */ FLAG (M_Cat(IV)) | FLAG (M_Cat(GB)) | FLAG (M_Cat(DOTTEDCIRCLE)))

ساكن مضمن منطقي
is_consonant_myanmar (ثابت hb_glyph_info_t &info)
{
  أرجع is_one_دا_myanmar (info, CONSONANT_FLAGS_MYANMAR);
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
  map->جوڑو_gsub_pause (تیاری_syllables_myanmar);

  map->enable_feature (HB_ٹیگ('l','o','c','l'), F_فی_SYLLABLE);
  /* The Indic specs do not require ccmp, but we apply it here since if
   * there is a use of it, it's typically at the beginning. */
  map->enable_feature (HB_ٹیگ('c','c','m','p'), F_فی_SYLLABLE);


  map->جوڑو_gsub_pause (reorder_myanmar);

  لكل (بنانشانی صحيح i = 0; i < ARRAY_لمائی (myanmar_basic_features); i++)
  {
    map->enable_feature (myanmar_basic_features[i], F_MANUAL_ZWJ | F_فی_SYLLABLE);
    map->جوڑو_gsub_pause (خالی_سوچک);
  }
  map->جوڑو_gsub_pause (hb_syllabic_clear_var); // Don't need syllables anymore, use stop to free buffer var

  لكل (بنانشانی صحيح i = 0; i < ARRAY_لمائی (myanmar_other_features); i++)
    map->enable_feature (myanmar_other_features[i], F_MANUAL_ZWJ);
}

ساكن فراغ
تیاری_masks_myanmar (ثابت hb_ot_shape_plan_t *plan HB_بن_ورتیا,
		     hb_بفر_t              *بفر,
		     hb_font_t                *font HB_بن_ورتیا)
{
  HB_بفر_ALLOCATE_VAR (بفر, myanmar_category);
  HB_بفر_ALLOCATE_VAR (بفر, myanmar_position);

  /* No masks, we just save information about characters. */

  بنانشانی صحيح گنتی = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    سیٹ_myanmar_properties (info[i]);
}

ساكن منطقي
تیاری_syllables_myanmar (ثابت hb_ot_shape_plan_t *plan HB_بن_ورتیا,
			 hb_font_t *font HB_بن_ورتیا,
			 hb_بفر_t *بفر)
{
  HB_بفر_ALLOCATE_VAR (بفر, syllable);
  لبھو_syllables_myanmar (بفر);
  foreach_syllable (بفر, شروع, سرا)
    بفر->unsafe_منزل_break (شروع, سرا);
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
				       بنانشانی صحيح شروع, بنانشانی صحيح سرا)
{
  hb_glyph_info_t *info = بفر->info;

  بنانشانی صحيح ادھار = سرا;
  منطقي has_reph = خطأ;

  {
    بنانشانی صحيح حد_بندی = شروع;
    إذا (شروع + 3 <= سرا &&
	info[شروع  ].myanmar_category() == M_Cat(Ra) &&
	info[شروع+1].myanmar_category() == M_Cat(As) &&
	info[شروع+2].myanmar_category() == M_Cat(H))
    {
      حد_بندی += 3;
      ادھار = شروع;
      has_reph = سچ;
    }

    {
      إذا (!has_reph)
	ادھار = حد_بندی;

      لكل (بنانشانی صحيح i = حد_بندی; i < سرا; i++)
	إذا (is_consonant_myanmar (info[i]))
	{
	  ادھار = i;
	  اكسر;
	}
    }
  }

  /* Reorder! */
  {
    بنانشانی صحيح i = شروع;
    لكل (; i < شروع + (has_reph ? 3 : 0); i++)
      info[i].myanmar_position() = POS_AFTER_MAIN;
    لكل (; i < ادھار; i++)
      info[i].myanmar_position() = POS_PRE_C;
    إذا (i < سرا)
    {
      info[i].myanmar_position() = POS_ادھار_C;
      i++;
    }
    myanmar_position_t pos = POS_AFTER_MAIN;
    /* The following loop may be ugly, but it implements all of
     * Myanmar reordering! */
    لكل (; i < سرا; i++)
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
	pos = POS_ہیٹھاں_C;
	info[i].myanmar_position() = pos;
	استمر;
      }

      إذا (pos == POS_ہیٹھاں_C && info[i].myanmar_category() == M_Cat(A))
      {
	info[i].myanmar_position() = POS_BEFORE_SUB;
	استمر;
      }
      إذا (pos == POS_ہیٹھاں_C && info[i].myanmar_category() == M_Cat(VBlw))
      {
	info[i].myanmar_position() = pos;
	استمر;
      }
      إذا (pos == POS_ہیٹھاں_C && info[i].myanmar_category() != M_Cat(A))
      {
	pos = POS_AFTER_SUB;
	info[i].myanmar_position() = pos;
	استمر;
      }
      info[i].myanmar_position() = pos;
    }
  }

  /* Sit tight, rock 'n roll! */
  بفر->sort (شروع, سرا, compare_myanmar_ترتیب);

  /* Flip left-matra sequence. */
  بنانشانی پہلا_بایاں_matra = سرا;
  بنانشانی آخری_بایاں_matra = سرا;
  لكل (بنانشانی صحيح i = شروع; i < سرا; i++)
  {
    إذا (info[i].myanmar_position() == POS_PRE_M)
    {
      إذا (پہلا_بایاں_matra == سرا)
	پہلا_بایاں_matra = i;
      آخری_بایاں_matra = i;
    }
  }
  /* https://github.com/harfbuzz/harfbuzz/issues/3863 */
  إذا (پہلا_بایاں_matra < آخری_بایاں_matra)
  {
    /* No need to merge clusters, done already? */
    بفر->الٹ_range (پہلا_بایاں_matra, آخری_بایاں_matra + 1);
    /* Reverse back VS, etc. */
    بنانشانی i = پہلا_بایاں_matra;
    لكل (بنانشانی j = i; j <= آخری_بایاں_matra; j++)
      إذا (info[j].myanmar_category() == M_Cat(VPre))
      {
	بفر->الٹ_range (i, j + 1);
	i = j + 1;
      }
  }
}

ساكن فراغ
reorder_syllable_myanmar (ثابت hb_ot_shape_plan_t *plan HB_بن_ورتیا,
			  hb_face_t *face HB_بن_ورتیا,
			  hb_بفر_t *بفر,
			  بنانشانی صحيح شروع, بنانشانی صحيح سرا)
{
  myanmar_syllable_type_t syllable_type = (myanmar_syllable_type_t) (بفر->info[شروع].syllable() & 0x0F);
  بدل (syllable_type) {

    حالة myanmar_broken_cluster: /* We already inserted dotted-circles, so just call the consonant_syllable. */
    حالة myanmar_consonant_syllable:
      initial_reordering_consonant_syllable  (بفر, شروع, سرا);
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
  إذا (بفر->سنیہا (font, "start reordering myanmar"))
  {
    إذا (hb_syllabic_داخل_کرو_dotted_circles (font, بفر,
					   myanmar_broken_cluster,
					   M_Cat(DOTTEDCIRCLE)))
      ret = سچ;

    foreach_syllable (بفر, شروع, سرا)
      reorder_syllable_myanmar (plan, font->face, بفر, شروع, سرا);
    (فراغ) بفر->سنیہا (font, "end reordering myanmar");
  }

  HB_بفر_DEALLOCATE_VAR (بفر, myanmar_category);
  HB_بفر_DEALLOCATE_VAR (بفر, myanmar_position);

  أرجع ret;
}


ثابت hb_ot_shaper_t _hb_ot_shaper_myanmar =
{
  collect_features_myanmar,
  خالی_سوچک, /* override_features */
  خالی_سوچک, /* data_create */
  خالی_سوچک, /* data_destroy */
  خالی_سوچک, /* preprocess_text */
  خالی_سوچک, /* postprocess_glyphs */
  خالی_سوچک, /* decompose */
  خالی_سوچک, /* compose */
  تیاری_masks_myanmar,
  خالی_سوچک, /* reorder_marks */
  HB_ٹیگ_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_منوال_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT,
  HB_OT_SHAPE_ZERO_چوڑائی_MARKS_BY_GDEF_EARLY,
  خطأ, /* fallback_position */
};


#جے_بنا_تعریف HB_NO_OT_SHAPER_MYANMAR_ZAWGYI
/* Ugly Zawgyi encoding.
 * Disable all auto processing.
 * https://github.com/harfbuzz/harfbuzz/issues/1162 */
ثابت hb_ot_shaper_t _hb_ot_shaper_myanmar_zawgyi =
{
  خالی_سوچک, /* collect_features */
  خالی_سوچک, /* override_features */
  خالی_سوچک, /* data_create */
  خالی_سوچک, /* data_destroy */
  خالی_سوچک, /* preprocess_text */
  خالی_سوچک, /* postprocess_glyphs */
  خالی_سوچک, /* decompose */
  خالی_سوچک, /* compose */
  خالی_سوچک, /* setup_masks */
  خالی_سوچک, /* reorder_marks */
  HB_ٹیگ_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_منوال_NONE,
  HB_OT_SHAPE_ZERO_چوڑائی_MARKS_NONE,
  خطأ, /* fallback_position */
};
#جے_انت


#جے_انت

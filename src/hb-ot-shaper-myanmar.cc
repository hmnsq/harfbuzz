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

#اگر_تعریف_نشده HB_NO_OT_SHAPE

#ضمن "hb-ot-shaper-myanmar-machine.hh"
#ضمن "hb-ot-shaper-indic.hh"
#ضمن "hb-ot-layout.hh"


/*
 * Myanmar shaper.
 */


ساكن ثابت hb_برچسب_t
myanmar_basic_features[] =
{
  /*
   * Basic features.
   * These features are applied in order, one at a time, after reordering,
   * constrained to the syllable.
   */
  HB_برچسب('r','p','h','f'),
  HB_برچسب('p','r','e','f'),
  HB_برچسب('b','l','w','f'),
  HB_برچسب('p','s','t','f'),
};
ساكن ثابت hb_برچسب_t
myanmar_other_features[] =
{
  /*
   * Other features.
   * These features are applied all at once, after clearing syllables.
   */
  HB_برچسب('p','r','e','s'),
  HB_برچسب('a','b','v','s'),
  HB_برچسب('b','l','w','s'),
  HB_برچسب('p','s','t','s'),
};

ساكن مضمن فراغ
مجموعه_myanmar_properties (hb_glyph_info_t &info)
{
  hb_codepoint_t u = info.codepoint;
  بیعلامت صحيح type = hb_indic_get_categories (u);

  info.myanmar_category() = (myanmar_category_t) (type & 0xFFu);
}


ساكن مضمن منطقي
is_one_از_myanmar (ثابت hb_glyph_info_t &info, بیعلامت صحيح flags)
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
#تعریف_کن CONSONANT_FLAGS_MYANMAR (FLAG (M_Cat(C)) | FLAG (M_Cat(CS)) | FLAG (M_Cat(Ra)) | /* FLAG (M_Cat(CM)) | */ FLAG (M_Cat(IV)) | FLAG (M_Cat(GB)) | FLAG (M_Cat(DOTTEDCIRCLE)))

ساكن مضمن منطقي
is_consonant_myanmar (ثابت hb_glyph_info_t &info)
{
  أرجع is_one_از_myanmar (info, CONSONANT_FLAGS_MYANMAR);
}


ساكن منطقي
راهاندازی_syllables_myanmar (ثابت hb_ot_shape_plan_t *plan,
			 hb_font_t *font,
			 hb_میانگیر_t *میانگیر);
ساكن منطقي
reorder_myanmar (ثابت hb_ot_shape_plan_t *plan,
		 hb_font_t *font,
		 hb_میانگیر_t *میانگیر);

ساكن فراغ
collect_features_myanmar (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  /* Do this before any lookups have been applied. */
  map->جمع_کن_gsub_pause (راهاندازی_syllables_myanmar);

  map->enable_feature (HB_برچسب('l','o','c','l'), F_در_هر_SYLLABLE);
  /* The Indic specs do not require ccmp, but we apply it here since if
   * there is a use of it, it's typically at the beginning. */
  map->enable_feature (HB_برچسب('c','c','m','p'), F_در_هر_SYLLABLE);


  map->جمع_کن_gsub_pause (reorder_myanmar);

  لكل (بیعلامت صحيح i = 0; i < ARRAY_درازی (myanmar_basic_features); i++)
  {
    map->enable_feature (myanmar_basic_features[i], F_MANUAL_ZWJ | F_در_هر_SYLLABLE);
    map->جمع_کن_gsub_pause (اشاره_تهی);
  }
  map->جمع_کن_gsub_pause (hb_syllabic_clear_var); // Don't need syllables anymore, use stop to free buffer var

  لكل (بیعلامت صحيح i = 0; i < ARRAY_درازی (myanmar_other_features); i++)
    map->enable_feature (myanmar_other_features[i], F_MANUAL_ZWJ);
}

ساكن فراغ
راهاندازی_masks_myanmar (ثابت hb_ot_shape_plan_t *plan HB_استفادهنشده,
		     hb_میانگیر_t              *میانگیر,
		     hb_font_t                *font HB_استفادهنشده)
{
  HB_میانگیر_ALLOCATE_VAR (میانگیر, myanmar_category);
  HB_میانگیر_ALLOCATE_VAR (میانگیر, myanmar_position);

  /* No masks, we just save information about characters. */

  بیعلامت صحيح شمار = میانگیر->len;
  hb_glyph_info_t *info = میانگیر->info;
  لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    مجموعه_myanmar_properties (info[i]);
}

ساكن منطقي
راهاندازی_syllables_myanmar (ثابت hb_ot_shape_plan_t *plan HB_استفادهنشده,
			 hb_font_t *font HB_استفادهنشده,
			 hb_میانگیر_t *میانگیر)
{
  HB_میانگیر_ALLOCATE_VAR (میانگیر, syllable);
  پیدا_کن_syllables_myanmar (میانگیر);
  foreach_syllable (میانگیر, شروع, انتها)
    میانگیر->unsafe_مقصد_break (شروع, انتها);
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
initial_reordering_consonant_syllable (hb_میانگیر_t *میانگیر,
				       بیعلامت صحيح شروع, بیعلامت صحيح انتها)
{
  hb_glyph_info_t *info = میانگیر->info;

  بیعلامت صحيح پایه = انتها;
  منطقي has_reph = خطأ;

  {
    بیعلامت صحيح حد = شروع;
    إذا (شروع + 3 <= انتها &&
	info[شروع  ].myanmar_category() == M_Cat(Ra) &&
	info[شروع+1].myanmar_category() == M_Cat(As) &&
	info[شروع+2].myanmar_category() == M_Cat(H))
    {
      حد += 3;
      پایه = شروع;
      has_reph = درست;
    }

    {
      إذا (!has_reph)
	پایه = حد;

      لكل (بیعلامت صحيح i = حد; i < انتها; i++)
	إذا (is_consonant_myanmar (info[i]))
	{
	  پایه = i;
	  اكسر;
	}
    }
  }

  /* Reorder! */
  {
    بیعلامت صحيح i = شروع;
    لكل (; i < شروع + (has_reph ? 3 : 0); i++)
      info[i].myanmar_position() = POS_AFTER_MAIN;
    لكل (; i < پایه; i++)
      info[i].myanmar_position() = POS_PRE_C;
    إذا (i < انتها)
    {
      info[i].myanmar_position() = POS_پایه_C;
      i++;
    }
    myanmar_position_t pos = POS_AFTER_MAIN;
    /* The following loop may be ugly, but it implements all of
     * Myanmar reordering! */
    لكل (; i < انتها; i++)
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
	pos = POS_زیر_C;
	info[i].myanmar_position() = pos;
	استمر;
      }

      إذا (pos == POS_زیر_C && info[i].myanmar_category() == M_Cat(A))
      {
	info[i].myanmar_position() = POS_BEFORE_SUB;
	استمر;
      }
      إذا (pos == POS_زیر_C && info[i].myanmar_category() == M_Cat(VBlw))
      {
	info[i].myanmar_position() = pos;
	استمر;
      }
      إذا (pos == POS_زیر_C && info[i].myanmar_category() != M_Cat(A))
      {
	pos = POS_AFTER_SUB;
	info[i].myanmar_position() = pos;
	استمر;
      }
      info[i].myanmar_position() = pos;
    }
  }

  /* Sit tight, rock 'n roll! */
  میانگیر->sort (شروع, انتها, compare_myanmar_ترتیب);

  /* Flip left-matra sequence. */
  بیعلامت اولی_چپ_matra = انتها;
  بیعلامت آخرین_چپ_matra = انتها;
  لكل (بیعلامت صحيح i = شروع; i < انتها; i++)
  {
    إذا (info[i].myanmar_position() == POS_PRE_M)
    {
      إذا (اولی_چپ_matra == انتها)
	اولی_چپ_matra = i;
      آخرین_چپ_matra = i;
    }
  }
  /* https://github.com/harfbuzz/harfbuzz/issues/3863 */
  إذا (اولی_چپ_matra < آخرین_چپ_matra)
  {
    /* No need to merge clusters, done already? */
    میانگیر->معکوس_range (اولی_چپ_matra, آخرین_چپ_matra + 1);
    /* Reverse back VS, etc. */
    بیعلامت i = اولی_چپ_matra;
    لكل (بیعلامت j = i; j <= آخرین_چپ_matra; j++)
      إذا (info[j].myanmar_category() == M_Cat(VPre))
      {
	میانگیر->معکوس_range (i, j + 1);
	i = j + 1;
      }
  }
}

ساكن فراغ
reorder_syllable_myanmar (ثابت hb_ot_shape_plan_t *plan HB_استفادهنشده,
			  hb_face_t *face HB_استفادهنشده,
			  hb_میانگیر_t *میانگیر,
			  بیعلامت صحيح شروع, بیعلامت صحيح انتها)
{
  myanmar_syllable_type_t syllable_type = (myanmar_syllable_type_t) (میانگیر->info[شروع].syllable() & 0x0F);
  بدل (syllable_type) {

    حالة myanmar_broken_cluster: /* We already inserted dotted-circles, so just call the consonant_syllable. */
    حالة myanmar_consonant_syllable:
      initial_reordering_consonant_syllable  (میانگیر, شروع, انتها);
      اكسر;

    حالة myanmar_non_myanmar_cluster:
      اكسر;
  }
}

ساكن منطقي
reorder_myanmar (ثابت hb_ot_shape_plan_t *plan,
		 hb_font_t *font,
		 hb_میانگیر_t *میانگیر)
{
  منطقي ret = خطأ;
  إذا (میانگیر->پیام (font, "start reordering myanmar"))
  {
    إذا (hb_syllabic_درج_dotted_circles (font, میانگیر,
					   myanmar_broken_cluster,
					   M_Cat(DOTTEDCIRCLE)))
      ret = درست;

    foreach_syllable (میانگیر, شروع, انتها)
      reorder_syllable_myanmar (plan, font->face, میانگیر, شروع, انتها);
    (فراغ) میانگیر->پیام (font, "end reordering myanmar");
  }

  HB_میانگیر_DEALLOCATE_VAR (میانگیر, myanmar_category);
  HB_میانگیر_DEALLOCATE_VAR (میانگیر, myanmar_position);

  أرجع ret;
}


ثابت hb_ot_shaper_t _hb_ot_shaper_myanmar =
{
  collect_features_myanmar,
  اشاره_تهی, /* override_features */
  اشاره_تهی, /* data_create */
  اشاره_تهی, /* data_destroy */
  اشاره_تهی, /* preprocess_text */
  اشاره_تهی, /* postprocess_glyphs */
  اشاره_تهی, /* decompose */
  اشاره_تهی, /* compose */
  راهاندازی_masks_myanmar,
  اشاره_تهی, /* reorder_marks */
  HB_برچسب_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_نما_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT,
  HB_OT_SHAPE_ZERO_عرض_MARKS_BY_GDEF_EARLY,
  خطأ, /* fallback_position */
};


#اگر_تعریف_نشده HB_NO_OT_SHAPER_MYANMAR_ZAWGYI
/* Ugly Zawgyi encoding.
 * Disable all auto processing.
 * https://github.com/harfbuzz/harfbuzz/issues/1162 */
ثابت hb_ot_shaper_t _hb_ot_shaper_myanmar_zawgyi =
{
  اشاره_تهی, /* collect_features */
  اشاره_تهی, /* override_features */
  اشاره_تهی, /* data_create */
  اشاره_تهی, /* data_destroy */
  اشاره_تهی, /* preprocess_text */
  اشاره_تهی, /* postprocess_glyphs */
  اشاره_تهی, /* decompose */
  اشاره_تهی, /* compose */
  اشاره_تهی, /* setup_masks */
  اشاره_تهی, /* reorder_marks */
  HB_برچسب_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_نما_NONE,
  HB_OT_SHAPE_ZERO_عرض_MARKS_NONE,
  خطأ, /* fallback_position */
};
#پایان_اگر


#پایان_اگر

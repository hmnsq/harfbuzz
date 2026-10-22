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

#অন্তর্ভাব "hb.hh"

#ifndef HB_NO_OT_SHAPE

#অন্তর্ভাব "hb-ot-shaper-myanmar-machine.hh"
#অন্তর্ভাব "hb-ot-shaper-indic.hh"
#অন্তর্ভাব "hb-ot-layout.hh"


/*
 * Myanmar shaper.
 */


স্থির ধ্রুবক hb_tag_t
myanmar_basic_features[] =
{
  /*
   * Basic features.
   * These features are applied in order, one at a time, after reordering,
   * constrained to the syllable.
   */
  HB_TAG('r','p','h','f'),
  HB_TAG('p','r','e','f'),
  HB_TAG('b','l','w','f'),
  HB_TAG('p','s','t','f'),
};
স্থির ধ্রুবক hb_tag_t
myanmar_other_features[] =
{
  /*
   * Other features.
   * These features are applied all at once, after clearing syllables.
   */
  HB_TAG('p','r','e','s'),
  HB_TAG('a','b','v','s'),
  HB_TAG('b','l','w','s'),
  HB_TAG('p','s','t','s'),
};

স্থির অন্তঃরেখ শূন্য
set_myanmar_properties (hb_glyph_info_t &info)
{
  hb_codepoint_t u = info.codepoint;
  unsigned পূর্ণসংখ্যা type = hb_indic_get_categories (u);

  info.myanmar_category() = (myanmar_category_t) (type & 0xFFu);
}


স্থির অন্তঃরেখ যৌক্তিক
is_one_of_myanmar (ধ্রুবক hb_glyph_info_t &info, unsigned পূর্ণসংখ্যা flags)
{
  /* If it ligated, all bets are off. */
  যদি (_hb_glyph_info_ligated (&info)) ফেরত মিথ্যা;
  ফেরত !!(FLAG_UNSAFE (info.myanmar_category()) & flags);
}

/* Note:
 *
 * We treat Vowels and placeholders as if they were consonants.  This is safe because Vowels
 * cannot happen in a consonant syllable.  The plus side however is, we can call the
 * consonant syllable logic from the vowel syllable function and get it all right!
 *
 * Keep in sync with consonant_categories in the generator. */
#define CONSONANT_FLAGS_MYANMAR (FLAG (M_Cat(C)) | FLAG (M_Cat(CS)) | FLAG (M_Cat(Ra)) | /* FLAG (M_Cat(CM)) | */ FLAG (M_Cat(IV)) | FLAG (M_Cat(GB)) | FLAG (M_Cat(DOTTEDCIRCLE)))

স্থির অন্তঃরেখ যৌক্তিক
is_consonant_myanmar (ধ্রুবক hb_glyph_info_t &info)
{
  ফেরত is_one_of_myanmar (info, CONSONANT_FLAGS_MYANMAR);
}


স্থির যৌক্তিক
setup_syllables_myanmar (ধ্রুবক hb_ot_shape_plan_t *plan,
			 hb_font_t *font,
			 hb_বাফার_t *বাফার);
স্থির যৌক্তিক
reorder_myanmar (ধ্রুবক hb_ot_shape_plan_t *plan,
		 hb_font_t *font,
		 hb_বাফার_t *বাফার);

স্থির শূন্য
collect_features_myanmar (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  /* Do this before any lookups have been applied. */
  map->add_gsub_pause (setup_syllables_myanmar);

  map->enable_feature (HB_TAG('l','o','c','l'), F_PER_SYLLABLE);
  /* The Indic specs do not require ccmp, but we apply it here since if
   * there is a use of it, it's typically at the beginning. */
  map->enable_feature (HB_TAG('c','c','m','p'), F_PER_SYLLABLE);


  map->add_gsub_pause (reorder_myanmar);

  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < ARRAY_দৈর্ঘ্য (myanmar_basic_features); i++)
  {
    map->enable_feature (myanmar_basic_features[i], F_MANUAL_ZWJ | F_PER_SYLLABLE);
    map->add_gsub_pause (nullptr);
  }
  map->add_gsub_pause (hb_syllabic_clear_var); // Don't need syllables anymore, use stop to free buffer var

  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < ARRAY_দৈর্ঘ্য (myanmar_other_features); i++)
    map->enable_feature (myanmar_other_features[i], F_MANUAL_ZWJ);
}

স্থির শূন্য
setup_masks_myanmar (ধ্রুবক hb_ot_shape_plan_t *plan HB_UNUSED,
		     hb_বাফার_t              *বাফার,
		     hb_font_t                *font HB_UNUSED)
{
  HB_বাফার_ALLOCATE_VAR (বাফার, myanmar_category);
  HB_বাফার_ALLOCATE_VAR (বাফার, myanmar_position);

  /* No masks, we just save information about characters. */

  unsigned পূর্ণসংখ্যা গুনতি = বাফার->len;
  hb_glyph_info_t *info = বাফার->info;
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    set_myanmar_properties (info[i]);
}

স্থির যৌক্তিক
setup_syllables_myanmar (ধ্রুবক hb_ot_shape_plan_t *plan HB_UNUSED,
			 hb_font_t *font HB_UNUSED,
			 hb_বাফার_t *বাফার)
{
  HB_বাফার_ALLOCATE_VAR (বাফার, syllable);
  find_syllables_myanmar (বাফার);
  foreach_syllable (বাফার, start, end)
    বাফার->unsafe_to_break (start, end);
  ফেরত মিথ্যা;
}

স্থির পূর্ণসংখ্যা
compare_myanmar_order (ধ্রুবক hb_glyph_info_t *pa, ধ্রুবক hb_glyph_info_t *pb)
{
  পূর্ণসংখ্যা a = pa->myanmar_position();
  পূর্ণসংখ্যা b = pb->myanmar_position();

  ফেরত (পূর্ণসংখ্যা) a - (পূর্ণসংখ্যা) b;
}


/* Rules from:
 * https://docs.microsoft.com/en-us/typography/script-development/myanmar */

স্থির শূন্য
initial_reordering_consonant_syllable (hb_বাফার_t *বাফার,
				       unsigned পূর্ণসংখ্যা start, unsigned পূর্ণসংখ্যা end)
{
  hb_glyph_info_t *info = বাফার->info;

  unsigned পূর্ণসংখ্যা base = end;
  যৌক্তিক has_reph = মিথ্যা;

  {
    unsigned পূর্ণসংখ্যা limit = start;
    যদি (start + 3 <= end &&
	info[start  ].myanmar_category() == M_Cat(Ra) &&
	info[start+1].myanmar_category() == M_Cat(As) &&
	info[start+2].myanmar_category() == M_Cat(H))
    {
      limit += 3;
      base = start;
      has_reph = সত্য;
    }

    {
      যদি (!has_reph)
	base = limit;

      জন্য (unsigned পূর্ণসংখ্যা i = limit; i < end; i++)
	যদি (is_consonant_myanmar (info[i]))
	{
	  base = i;
	  বিরতি;
	}
    }
  }

  /* Reorder! */
  {
    unsigned পূর্ণসংখ্যা i = start;
    জন্য (; i < start + (has_reph ? 3 : 0); i++)
      info[i].myanmar_position() = POS_AFTER_MAIN;
    জন্য (; i < base; i++)
      info[i].myanmar_position() = POS_PRE_C;
    যদি (i < end)
    {
      info[i].myanmar_position() = POS_BASE_C;
      i++;
    }
    myanmar_position_t pos = POS_AFTER_MAIN;
    /* The following loop may be ugly, but it implements all of
     * Myanmar reordering! */
    জন্য (; i < end; i++)
    {
      যদি (info[i].myanmar_category() == M_Cat(MR)) /* Pre-base reordering */
      {
	info[i].myanmar_position() = POS_PRE_C;
	চালু;
      }
      যদি (info[i].myanmar_category() == M_Cat(VPre)) /* Left matra */
      {
	info[i].myanmar_position() = POS_PRE_M;
	চালু;
      }
      যদি (info[i].myanmar_category() == M_Cat(VS))
      {
	info[i].myanmar_position() = info[i - 1].myanmar_position();
	চালু;
      }

      যদি (pos == POS_AFTER_MAIN && info[i].myanmar_category() == M_Cat(VBlw))
      {
	pos = POS_BELOW_C;
	info[i].myanmar_position() = pos;
	চালু;
      }

      যদি (pos == POS_BELOW_C && info[i].myanmar_category() == M_Cat(A))
      {
	info[i].myanmar_position() = POS_BEFORE_SUB;
	চালু;
      }
      যদি (pos == POS_BELOW_C && info[i].myanmar_category() == M_Cat(VBlw))
      {
	info[i].myanmar_position() = pos;
	চালু;
      }
      যদি (pos == POS_BELOW_C && info[i].myanmar_category() != M_Cat(A))
      {
	pos = POS_AFTER_SUB;
	info[i].myanmar_position() = pos;
	চালু;
      }
      info[i].myanmar_position() = pos;
    }
  }

  /* Sit tight, rock 'n roll! */
  বাফার->sort (start, end, compare_myanmar_order);

  /* Flip left-matra sequence. */
  unsigned first_বাম_matra = end;
  unsigned last_বাম_matra = end;
  জন্য (unsigned পূর্ণসংখ্যা i = start; i < end; i++)
  {
    যদি (info[i].myanmar_position() == POS_PRE_M)
    {
      যদি (first_বাম_matra == end)
	first_বাম_matra = i;
      last_বাম_matra = i;
    }
  }
  /* https://github.com/harfbuzz/harfbuzz/issues/3863 */
  যদি (first_বাম_matra < last_বাম_matra)
  {
    /* No need to merge clusters, done already? */
    বাফার->reverse_range (first_বাম_matra, last_বাম_matra + 1);
    /* Reverse back VS, etc. */
    unsigned i = first_বাম_matra;
    জন্য (unsigned j = i; j <= last_বাম_matra; j++)
      যদি (info[j].myanmar_category() == M_Cat(VPre))
      {
	বাফার->reverse_range (i, j + 1);
	i = j + 1;
      }
  }
}

স্থির শূন্য
reorder_syllable_myanmar (ধ্রুবক hb_ot_shape_plan_t *plan HB_UNUSED,
			  hb_face_t *face HB_UNUSED,
			  hb_বাফার_t *বাফার,
			  unsigned পূর্ণসংখ্যা start, unsigned পূর্ণসংখ্যা end)
{
  myanmar_syllable_type_t syllable_type = (myanmar_syllable_type_t) (বাফার->info[start].syllable() & 0x0F);
  নির্বাচন (syllable_type) {

    ক্ষেত্র myanmar_broken_cluster: /* We already inserted dotted-circles, so just call the consonant_syllable. */
    ক্ষেত্র myanmar_consonant_syllable:
      initial_reordering_consonant_syllable  (বাফার, start, end);
      বিরতি;

    ক্ষেত্র myanmar_non_myanmar_cluster:
      বিরতি;
  }
}

স্থির যৌক্তিক
reorder_myanmar (ধ্রুবক hb_ot_shape_plan_t *plan,
		 hb_font_t *font,
		 hb_বাফার_t *বাফার)
{
  যৌক্তিক ret = মিথ্যা;
  যদি (বাফার->message (font, "start reordering myanmar"))
  {
    যদি (hb_syllabic_ঢোকাও_dotted_circles (font, বাফার,
					   myanmar_broken_cluster,
					   M_Cat(DOTTEDCIRCLE)))
      ret = সত্য;

    foreach_syllable (বাফার, start, end)
      reorder_syllable_myanmar (plan, font->face, বাফার, start, end);
    (শূন্য) বাফার->message (font, "end reordering myanmar");
  }

  HB_বাফার_DEALLOCATE_VAR (বাফার, myanmar_category);
  HB_বাফার_DEALLOCATE_VAR (বাফার, myanmar_position);

  ফেরত ret;
}


ধ্রুবক hb_ot_shaper_t _hb_ot_shaper_myanmar =
{
  collect_features_myanmar,
  nullptr, /* override_features */
  nullptr, /* data_create */
  nullptr, /* data_destroy */
  nullptr, /* preprocess_text */
  nullptr, /* postprocess_glyphs */
  nullptr, /* decompose */
  nullptr, /* compose */
  setup_masks_myanmar,
  nullptr, /* reorder_marks */
  HB_TAG_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_MODE_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT,
  HB_OT_SHAPE_ZERO_WIDTH_MARKS_BY_GDEF_EARLY,
  মিথ্যা, /* fallback_position */
};


#ifndef HB_NO_OT_SHAPER_MYANMAR_ZAWGYI
/* Ugly Zawgyi encoding.
 * Disable all auto processing.
 * https://github.com/harfbuzz/harfbuzz/issues/1162 */
ধ্রুবক hb_ot_shaper_t _hb_ot_shaper_myanmar_zawgyi =
{
  nullptr, /* collect_features */
  nullptr, /* override_features */
  nullptr, /* data_create */
  nullptr, /* data_destroy */
  nullptr, /* preprocess_text */
  nullptr, /* postprocess_glyphs */
  nullptr, /* decompose */
  nullptr, /* compose */
  nullptr, /* setup_masks */
  nullptr, /* reorder_marks */
  HB_TAG_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_MODE_NONE,
  HB_OT_SHAPE_ZERO_WIDTH_MARKS_NONE,
  মিথ্যা, /* fallback_position */
};
#endif


#endif

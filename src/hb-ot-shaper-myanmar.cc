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

#ifndef HB_NO_OT_SHAPE

#ضمن "hb-ot-shaper-myanmar-machine.hh"
#ضمن "hb-ot-shaper-indic.hh"
#ضمن "hb-ot-layout.hh"


/*
 * Myanmar shaper.
 */


ساكن ثابت hb_tag_t
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
ساكن ثابت hb_tag_t
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

ساكن مضمن فراغ
set_myanmar_properties (hb_glyph_info_t &info)
{
  hb_codepoint_t u = info.codepoint;
  unsigned صحيح type = hb_indic_get_categories (u);

  info.myanmar_category() = (myanmar_category_t) (type & 0xFFu);
}


ساكن مضمن منطقي
is_one_of_myanmar (ثابت hb_glyph_info_t &info, unsigned صحيح flags)
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
#define CONSONANT_FLAGS_MYANMAR (FLAG (M_Cat(C)) | FLAG (M_Cat(CS)) | FLAG (M_Cat(Ra)) | /* FLAG (M_Cat(CM)) | */ FLAG (M_Cat(IV)) | FLAG (M_Cat(GB)) | FLAG (M_Cat(DOTTEDCIRCLE)))

ساكن مضمن منطقي
is_consonant_myanmar (ثابت hb_glyph_info_t &info)
{
  أرجع is_one_of_myanmar (info, CONSONANT_FLAGS_MYANMAR);
}


ساكن منطقي
setup_syllables_myanmar (ثابت hb_ot_shape_plan_t *plan,
			 hb_font_t *font,
			 hb_buffer_t *buffer);
ساكن منطقي
reorder_myanmar (ثابت hb_ot_shape_plan_t *plan,
		 hb_font_t *font,
		 hb_buffer_t *buffer);

ساكن فراغ
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

  لكل (unsigned صحيح i = 0; i < ARRAY_LENGTH (myanmar_basic_features); i++)
  {
    map->enable_feature (myanmar_basic_features[i], F_MANUAL_ZWJ | F_PER_SYLLABLE);
    map->add_gsub_pause (nullptr);
  }
  map->add_gsub_pause (hb_syllabic_clear_var); // Don't need syllables anymore, use stop to free buffer var

  لكل (unsigned صحيح i = 0; i < ARRAY_LENGTH (myanmar_other_features); i++)
    map->enable_feature (myanmar_other_features[i], F_MANUAL_ZWJ);
}

ساكن فراغ
setup_masks_myanmar (ثابت hb_ot_shape_plan_t *plan HB_UNUSED,
		     hb_buffer_t              *buffer,
		     hb_font_t                *font HB_UNUSED)
{
  HB_BUFFER_ALLOCATE_VAR (buffer, myanmar_category);
  HB_BUFFER_ALLOCATE_VAR (buffer, myanmar_position);

  /* No masks, we just save information about characters. */

  unsigned صحيح عدد = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  لكل (unsigned صحيح i = 0; i < عدد; i++)
    set_myanmar_properties (info[i]);
}

ساكن منطقي
setup_syllables_myanmar (ثابت hb_ot_shape_plan_t *plan HB_UNUSED,
			 hb_font_t *font HB_UNUSED,
			 hb_buffer_t *buffer)
{
  HB_BUFFER_ALLOCATE_VAR (buffer, syllable);
  find_syllables_myanmar (buffer);
  foreach_syllable (buffer, start, end)
    buffer->unsafe_to_break (start, end);
  أرجع خطأ;
}

ساكن صحيح
compare_myanmar_order (ثابت hb_glyph_info_t *pa, ثابت hb_glyph_info_t *pb)
{
  صحيح a = pa->myanmar_position();
  صحيح b = pb->myanmar_position();

  أرجع (صحيح) a - (صحيح) b;
}


/* Rules from:
 * https://docs.microsoft.com/en-us/typography/script-development/myanmar */

ساكن فراغ
initial_reordering_consonant_syllable (hb_buffer_t *buffer,
				       unsigned صحيح start, unsigned صحيح end)
{
  hb_glyph_info_t *info = buffer->info;

  unsigned صحيح base = end;
  منطقي has_reph = خطأ;

  {
    unsigned صحيح limit = start;
    إذا (start + 3 <= end &&
	info[start  ].myanmar_category() == M_Cat(Ra) &&
	info[start+1].myanmar_category() == M_Cat(As) &&
	info[start+2].myanmar_category() == M_Cat(H))
    {
      limit += 3;
      base = start;
      has_reph = true;
    }

    {
      إذا (!has_reph)
	base = limit;

      لكل (unsigned صحيح i = limit; i < end; i++)
	إذا (is_consonant_myanmar (info[i]))
	{
	  base = i;
	  اكسر;
	}
    }
  }

  /* Reorder! */
  {
    unsigned صحيح i = start;
    لكل (; i < start + (has_reph ? 3 : 0); i++)
      info[i].myanmar_position() = POS_AFTER_MAIN;
    لكل (; i < base; i++)
      info[i].myanmar_position() = POS_PRE_C;
    إذا (i < end)
    {
      info[i].myanmar_position() = POS_BASE_C;
      i++;
    }
    myanmar_position_t pos = POS_AFTER_MAIN;
    /* The following loop may be ugly, but it implements all of
     * Myanmar reordering! */
    لكل (; i < end; i++)
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
	pos = POS_BELOW_C;
	info[i].myanmar_position() = pos;
	استمر;
      }

      إذا (pos == POS_BELOW_C && info[i].myanmar_category() == M_Cat(A))
      {
	info[i].myanmar_position() = POS_BEFORE_SUB;
	استمر;
      }
      إذا (pos == POS_BELOW_C && info[i].myanmar_category() == M_Cat(VBlw))
      {
	info[i].myanmar_position() = pos;
	استمر;
      }
      إذا (pos == POS_BELOW_C && info[i].myanmar_category() != M_Cat(A))
      {
	pos = POS_AFTER_SUB;
	info[i].myanmar_position() = pos;
	استمر;
      }
      info[i].myanmar_position() = pos;
    }
  }

  /* Sit tight, rock 'n roll! */
  buffer->sort (start, end, compare_myanmar_order);

  /* Flip left-matra sequence. */
  unsigned first_يسار_matra = end;
  unsigned last_يسار_matra = end;
  لكل (unsigned صحيح i = start; i < end; i++)
  {
    إذا (info[i].myanmar_position() == POS_PRE_M)
    {
      إذا (first_يسار_matra == end)
	first_يسار_matra = i;
      last_يسار_matra = i;
    }
  }
  /* https://github.com/harfbuzz/harfbuzz/issues/3863 */
  إذا (first_يسار_matra < last_يسار_matra)
  {
    /* No need to merge clusters, done already? */
    buffer->reverse_range (first_يسار_matra, last_يسار_matra + 1);
    /* Reverse back VS, etc. */
    unsigned i = first_يسار_matra;
    لكل (unsigned j = i; j <= last_يسار_matra; j++)
      إذا (info[j].myanmar_category() == M_Cat(VPre))
      {
	buffer->reverse_range (i, j + 1);
	i = j + 1;
      }
  }
}

ساكن فراغ
reorder_syllable_myanmar (ثابت hb_ot_shape_plan_t *plan HB_UNUSED,
			  hb_face_t *face HB_UNUSED,
			  hb_buffer_t *buffer,
			  unsigned صحيح start, unsigned صحيح end)
{
  myanmar_syllable_type_t syllable_type = (myanmar_syllable_type_t) (buffer->info[start].syllable() & 0x0F);
  بدل (syllable_type) {

    حالة myanmar_broken_cluster: /* We already inserted dotted-circles, so just call the consonant_syllable. */
    حالة myanmar_consonant_syllable:
      initial_reordering_consonant_syllable  (buffer, start, end);
      اكسر;

    حالة myanmar_non_myanmar_cluster:
      اكسر;
  }
}

ساكن منطقي
reorder_myanmar (ثابت hb_ot_shape_plan_t *plan,
		 hb_font_t *font,
		 hb_buffer_t *buffer)
{
  منطقي ret = خطأ;
  إذا (buffer->message (font, "start reordering myanmar"))
  {
    إذا (hb_syllabic_أدرج_dotted_circles (font, buffer,
					   myanmar_broken_cluster,
					   M_Cat(DOTTEDCIRCLE)))
      ret = true;

    foreach_syllable (buffer, start, end)
      reorder_syllable_myanmar (plan, font->face, buffer, start, end);
    (فراغ) buffer->message (font, "end reordering myanmar");
  }

  HB_BUFFER_DEALLOCATE_VAR (buffer, myanmar_category);
  HB_BUFFER_DEALLOCATE_VAR (buffer, myanmar_position);

  أرجع ret;
}


ثابت hb_ot_shaper_t _hb_ot_shaper_myanmar =
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
  خطأ, /* fallback_position */
};


#ifndef HB_NO_OT_SHAPER_MYANMAR_ZAWGYI
/* Ugly Zawgyi encoding.
 * Disable all auto processing.
 * https://github.com/harfbuzz/harfbuzz/issues/1162 */
ثابت hb_ot_shaper_t _hb_ot_shaper_myanmar_zawgyi =
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
  خطأ, /* fallback_position */
};
#endif


#endif

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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ifndef HB_NO_OT_SHAPE

#ଅନ୍ତର୍ଭାଵ "hb-ot-shaper-myanmar-machine.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-shaper-indic.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-layout.hh"


/*
 * Myanmar shaper.
 */


ସ୍ଥିର ଅଚଳ hb_tag_t
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
ସ୍ଥିର ଅଚଳ hb_tag_t
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

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
set_myanmar_properties (hb_glyph_info_t &info)
{
  hb_codepoint_t u = info.codepoint;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = hb_indic_get_categories (u);

  info.myanmar_category() = (myanmar_category_t) (type & 0xFFu);
}


ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
is_one_of_myanmar (ଅଚଳ hb_glyph_info_t &info, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
  /* If it ligated, all bets are off. */
  ଯଦି (_hb_glyph_info_ligated (&info)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
  ଫେରନ୍ତୁ !!(FLAG_UNSAFE (info.myanmar_category()) & flags);
}

/* Note:
 *
 * We treat Vowels and placeholders as if they were consonants.  This is safe because Vowels
 * cannot happen in a consonant syllable.  The plus side however is, we can call the
 * consonant syllable logic from the vowel syllable function and get it all right!
 *
 * Keep in sync with consonant_categories in the generator. */
#define CONSONANT_FLAGS_MYANMAR (FLAG (M_Cat(C)) | FLAG (M_Cat(CS)) | FLAG (M_Cat(Ra)) | /* FLAG (M_Cat(CM)) | */ FLAG (M_Cat(IV)) | FLAG (M_Cat(GB)) | FLAG (M_Cat(DOTTEDCIRCLE)))

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
is_consonant_myanmar (ଅଚଳ hb_glyph_info_t &info)
{
  ଫେରନ୍ତୁ is_one_of_myanmar (info, CONSONANT_FLAGS_MYANMAR);
}


ସ୍ଥିର ତାର୍କିକ
setup_syllables_myanmar (ଅଚଳ hb_ot_shape_plan_t *plan,
			 hb_font_t *font,
			 hb_buffer_t *buffer);
ସ୍ଥିର ତାର୍କିକ
reorder_myanmar (ଅଚଳ hb_ot_shape_plan_t *plan,
		 hb_font_t *font,
		 hb_buffer_t *buffer);

ସ୍ଥିର ଶୂନ୍ୟ
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

  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ARRAY_LENGTH (myanmar_basic_features); i++)
  {
    map->enable_feature (myanmar_basic_features[i], F_MANUAL_ZWJ | F_PER_SYLLABLE);
    map->add_gsub_pause (nullptr);
  }
  map->add_gsub_pause (hb_syllabic_clear_var); // Don't need syllables anymore, use stop to free buffer var

  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ARRAY_LENGTH (myanmar_other_features); i++)
    map->enable_feature (myanmar_other_features[i], F_MANUAL_ZWJ);
}

ସ୍ଥିର ଶୂନ୍ୟ
setup_masks_myanmar (ଅଚଳ hb_ot_shape_plan_t *plan HB_UNUSED,
		     hb_buffer_t              *buffer,
		     hb_font_t                *font HB_UNUSED)
{
  HB_BUFFER_ALLOCATE_VAR (buffer, myanmar_category);
  HB_BUFFER_ALLOCATE_VAR (buffer, myanmar_position);

  /* No masks, we just save information about characters. */

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    set_myanmar_properties (info[i]);
}

ସ୍ଥିର ତାର୍କିକ
setup_syllables_myanmar (ଅଚଳ hb_ot_shape_plan_t *plan HB_UNUSED,
			 hb_font_t *font HB_UNUSED,
			 hb_buffer_t *buffer)
{
  HB_BUFFER_ALLOCATE_VAR (buffer, syllable);
  find_syllables_myanmar (buffer);
  foreach_syllable (buffer, start, end)
    buffer->unsafe_to_break (start, end);
  ଫେରନ୍ତୁ ମିଥ୍ୟା;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
compare_myanmar_order (ଅଚଳ hb_glyph_info_t *pa, ଅଚଳ hb_glyph_info_t *pb)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା a = pa->myanmar_position();
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା b = pb->myanmar_position();

  ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) a - (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) b;
}


/* Rules from:
 * https://docs.microsoft.com/en-us/typography/script-development/myanmar */

ସ୍ଥିର ଶୂନ୍ୟ
initial_reordering_consonant_syllable (hb_buffer_t *buffer,
				       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end)
{
  hb_glyph_info_t *info = buffer->info;

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା base = end;
  ତାର୍କିକ has_reph = ମିଥ୍ୟା;

  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା limit = start;
    ଯଦି (start + 3 <= end &&
	info[start  ].myanmar_category() == M_Cat(Ra) &&
	info[start+1].myanmar_category() == M_Cat(As) &&
	info[start+2].myanmar_category() == M_Cat(H))
    {
      limit += 3;
      base = start;
      has_reph = ସତ୍ୟ;
    }

    {
      ଯଦି (!has_reph)
	base = limit;

      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = limit; i < end; i++)
	ଯଦି (is_consonant_myanmar (info[i]))
	{
	  base = i;
	  ଭାଙ୍ଗ;
	}
    }
  }

  /* Reorder! */
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = start;
    ପାଇଁ (; i < start + (has_reph ? 3 : 0); i++)
      info[i].myanmar_position() = POS_AFTER_MAIN;
    ପାଇଁ (; i < base; i++)
      info[i].myanmar_position() = POS_PRE_C;
    ଯଦି (i < end)
    {
      info[i].myanmar_position() = POS_BASE_C;
      i++;
    }
    myanmar_position_t pos = POS_AFTER_MAIN;
    /* The following loop may be ugly, but it implements all of
     * Myanmar reordering! */
    ପାଇଁ (; i < end; i++)
    {
      ଯଦି (info[i].myanmar_category() == M_Cat(MR)) /* Pre-base reordering */
      {
	info[i].myanmar_position() = POS_PRE_C;
	ଜାରି;
      }
      ଯଦି (info[i].myanmar_category() == M_Cat(VPre)) /* Left matra */
      {
	info[i].myanmar_position() = POS_PRE_M;
	ଜାରି;
      }
      ଯଦି (info[i].myanmar_category() == M_Cat(VS))
      {
	info[i].myanmar_position() = info[i - 1].myanmar_position();
	ଜାରି;
      }

      ଯଦି (pos == POS_AFTER_MAIN && info[i].myanmar_category() == M_Cat(VBlw))
      {
	pos = POS_BELOW_C;
	info[i].myanmar_position() = pos;
	ଜାରି;
      }

      ଯଦି (pos == POS_BELOW_C && info[i].myanmar_category() == M_Cat(A))
      {
	info[i].myanmar_position() = POS_BEFORE_SUB;
	ଜାରି;
      }
      ଯଦି (pos == POS_BELOW_C && info[i].myanmar_category() == M_Cat(VBlw))
      {
	info[i].myanmar_position() = pos;
	ଜାରି;
      }
      ଯଦି (pos == POS_BELOW_C && info[i].myanmar_category() != M_Cat(A))
      {
	pos = POS_AFTER_SUB;
	info[i].myanmar_position() = pos;
	ଜାରି;
      }
      info[i].myanmar_position() = pos;
    }
  }

  /* Sit tight, rock 'n roll! */
  buffer->sort (start, end, compare_myanmar_order);

  /* Flip left-matra sequence. */
  unsigned first_ବାମ_matra = end;
  unsigned last_ବାମ_matra = end;
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = start; i < end; i++)
  {
    ଯଦି (info[i].myanmar_position() == POS_PRE_M)
    {
      ଯଦି (first_ବାମ_matra == end)
	first_ବାମ_matra = i;
      last_ବାମ_matra = i;
    }
  }
  /* https://github.com/harfbuzz/harfbuzz/issues/3863 */
  ଯଦି (first_ବାମ_matra < last_ବାମ_matra)
  {
    /* No need to merge clusters, done already? */
    buffer->reverse_range (first_ବାମ_matra, last_ବାମ_matra + 1);
    /* Reverse back VS, etc. */
    unsigned i = first_ବାମ_matra;
    ପାଇଁ (unsigned j = i; j <= last_ବାମ_matra; j++)
      ଯଦି (info[j].myanmar_category() == M_Cat(VPre))
      {
	buffer->reverse_range (i, j + 1);
	i = j + 1;
      }
  }
}

ସ୍ଥିର ଶୂନ୍ୟ
reorder_syllable_myanmar (ଅଚଳ hb_ot_shape_plan_t *plan HB_UNUSED,
			  hb_face_t *face HB_UNUSED,
			  hb_buffer_t *buffer,
			  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end)
{
  myanmar_syllable_type_t syllable_type = (myanmar_syllable_type_t) (buffer->info[start].syllable() & 0x0F);
  ବାଛ (syllable_type) {

    ସ୍ଥିତି myanmar_broken_cluster: /* We already inserted dotted-circles, so just call the consonant_syllable. */
    ସ୍ଥିତି myanmar_consonant_syllable:
      initial_reordering_consonant_syllable  (buffer, start, end);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି myanmar_non_myanmar_cluster:
      ଭାଙ୍ଗ;
  }
}

ସ୍ଥିର ତାର୍କିକ
reorder_myanmar (ଅଚଳ hb_ot_shape_plan_t *plan,
		 hb_font_t *font,
		 hb_buffer_t *buffer)
{
  ତାର୍କିକ ret = ମିଥ୍ୟା;
  ଯଦି (buffer->message (font, "start reordering myanmar"))
  {
    ଯଦି (hb_syllabic_ଭର୍ତ୍ତି_dotted_circles (font, buffer,
					   myanmar_broken_cluster,
					   M_Cat(DOTTEDCIRCLE)))
      ret = ସତ୍ୟ;

    foreach_syllable (buffer, start, end)
      reorder_syllable_myanmar (plan, font->face, buffer, start, end);
    (ଶୂନ୍ୟ) buffer->message (font, "end reordering myanmar");
  }

  HB_BUFFER_DEALLOCATE_VAR (buffer, myanmar_category);
  HB_BUFFER_DEALLOCATE_VAR (buffer, myanmar_position);

  ଫେରନ୍ତୁ ret;
}


ଅଚଳ hb_ot_shaper_t _hb_ot_shaper_myanmar =
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
  ମିଥ୍ୟା, /* fallback_position */
};


#ifndef HB_NO_OT_SHAPER_MYANMAR_ZAWGYI
/* Ugly Zawgyi encoding.
 * Disable all auto processing.
 * https://github.com/harfbuzz/harfbuzz/issues/1162 */
ଅଚଳ hb_ot_shaper_t _hb_ot_shaper_myanmar_zawgyi =
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
  ମିଥ୍ୟା, /* fallback_position */
};
#endif


#endif

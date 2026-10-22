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

#अन्तर्भाव "hb.hh"

#ifndef HB_NO_OT_SHAPE

#अन्तर्भाव "hb-ot-shaper-myanmar-machine.hh"
#अन्तर्भाव "hb-ot-shaper-indic.hh"
#अन्तर्भाव "hb-ot-layout.hh"


/*
 * Myanmar shaper.
 */


स्थिर ध्रुव hb_tag_t
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
स्थिर ध्रुव hb_tag_t
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

स्थिर अन्तःस्थ शून्य
set_myanmar_properties (hb_glyph_info_t &info)
{
  hb_codepoint_t u = info.codepoint;
  अचिन्हित पूर्णाङ्क type = hb_indic_get_categories (u);

  info.myanmar_category() = (myanmar_category_t) (type & 0xFFu);
}


स्थिर अन्तःस्थ तार्किक
is_one_of_myanmar (ध्रुव hb_glyph_info_t &info, अचिन्हित पूर्णाङ्क flags)
{
  /* If it ligated, all bets are off. */
  यदि (_hb_glyph_info_ligated (&info)) प्रत्यागम असत्य;
  प्रत्यागम !!(FLAG_UNSAFE (info.myanmar_category()) & flags);
}

/* Note:
 *
 * We treat Vowels and placeholders as if they were consonants.  This is safe because Vowels
 * cannot happen in a consonant syllable.  The plus side however is, we can call the
 * consonant syllable logic from the vowel syllable function and get it all right!
 *
 * Keep in sync with consonant_categories in the generator. */
#define CONSONANT_FLAGS_MYANMAR (FLAG (M_Cat(C)) | FLAG (M_Cat(CS)) | FLAG (M_Cat(Ra)) | /* FLAG (M_Cat(CM)) | */ FLAG (M_Cat(IV)) | FLAG (M_Cat(GB)) | FLAG (M_Cat(DOTTEDCIRCLE)))

स्थिर अन्तःस्थ तार्किक
is_consonant_myanmar (ध्रुव hb_glyph_info_t &info)
{
  प्रत्यागम is_one_of_myanmar (info, CONSONANT_FLAGS_MYANMAR);
}


स्थिर तार्किक
setup_syllables_myanmar (ध्रुव hb_ot_shape_plan_t *plan,
			 hb_font_t *font,
			 hb_buffer_t *buffer);
स्थिर तार्किक
reorder_myanmar (ध्रुव hb_ot_shape_plan_t *plan,
		 hb_font_t *font,
		 hb_buffer_t *buffer);

स्थिर शून्य
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

  प्रति (अचिन्हित पूर्णाङ्क i = 0; i < ARRAY_LENGTH (myanmar_basic_features); i++)
  {
    map->enable_feature (myanmar_basic_features[i], F_MANUAL_ZWJ | F_PER_SYLLABLE);
    map->add_gsub_pause (nullptr);
  }
  map->add_gsub_pause (hb_syllabic_clear_var); // Don't need syllables anymore, use stop to free buffer var

  प्रति (अचिन्हित पूर्णाङ्क i = 0; i < ARRAY_LENGTH (myanmar_other_features); i++)
    map->enable_feature (myanmar_other_features[i], F_MANUAL_ZWJ);
}

स्थिर शून्य
setup_masks_myanmar (ध्रुव hb_ot_shape_plan_t *plan HB_UNUSED,
		     hb_buffer_t              *buffer,
		     hb_font_t                *font HB_UNUSED)
{
  HB_BUFFER_ALLOCATE_VAR (buffer, myanmar_category);
  HB_BUFFER_ALLOCATE_VAR (buffer, myanmar_position);

  /* No masks, we just save information about characters. */

  अचिन्हित पूर्णाङ्क गिनती = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  प्रति (अचिन्हित पूर्णाङ्क i = 0; i < गिनती; i++)
    set_myanmar_properties (info[i]);
}

स्थिर तार्किक
setup_syllables_myanmar (ध्रुव hb_ot_shape_plan_t *plan HB_UNUSED,
			 hb_font_t *font HB_UNUSED,
			 hb_buffer_t *buffer)
{
  HB_BUFFER_ALLOCATE_VAR (buffer, syllable);
  find_syllables_myanmar (buffer);
  foreach_syllable (buffer, start, end)
    buffer->unsafe_to_break (start, end);
  प्रत्यागम असत्य;
}

स्थिर पूर्णाङ्क
compare_myanmar_order (ध्रुव hb_glyph_info_t *pa, ध्रुव hb_glyph_info_t *pb)
{
  पूर्णाङ्क a = pa->myanmar_position();
  पूर्णाङ्क b = pb->myanmar_position();

  प्रत्यागम (पूर्णाङ्क) a - (पूर्णाङ्क) b;
}


/* Rules from:
 * https://docs.microsoft.com/en-us/typography/script-development/myanmar */

स्थिर शून्य
initial_reordering_consonant_syllable (hb_buffer_t *buffer,
				       अचिन्हित पूर्णाङ्क start, अचिन्हित पूर्णाङ्क end)
{
  hb_glyph_info_t *info = buffer->info;

  अचिन्हित पूर्णाङ्क base = end;
  तार्किक has_reph = असत्य;

  {
    अचिन्हित पूर्णाङ्क limit = start;
    यदि (start + 3 <= end &&
	info[start  ].myanmar_category() == M_Cat(Ra) &&
	info[start+1].myanmar_category() == M_Cat(As) &&
	info[start+2].myanmar_category() == M_Cat(H))
    {
      limit += 3;
      base = start;
      has_reph = सत्य;
    }

    {
      यदि (!has_reph)
	base = limit;

      प्रति (अचिन्हित पूर्णाङ्क i = limit; i < end; i++)
	यदि (is_consonant_myanmar (info[i]))
	{
	  base = i;
	  भङ्ग;
	}
    }
  }

  /* Reorder! */
  {
    अचिन्हित पूर्णाङ्क i = start;
    प्रति (; i < start + (has_reph ? 3 : 0); i++)
      info[i].myanmar_position() = POS_AFTER_MAIN;
    प्रति (; i < base; i++)
      info[i].myanmar_position() = POS_PRE_C;
    यदि (i < end)
    {
      info[i].myanmar_position() = POS_BASE_C;
      i++;
    }
    myanmar_position_t pos = POS_AFTER_MAIN;
    /* The following loop may be ugly, but it implements all of
     * Myanmar reordering! */
    प्रति (; i < end; i++)
    {
      यदि (info[i].myanmar_category() == M_Cat(MR)) /* Pre-base reordering */
      {
	info[i].myanmar_position() = POS_PRE_C;
	अनुवर्त;
      }
      यदि (info[i].myanmar_category() == M_Cat(VPre)) /* Left matra */
      {
	info[i].myanmar_position() = POS_PRE_M;
	अनुवर्त;
      }
      यदि (info[i].myanmar_category() == M_Cat(VS))
      {
	info[i].myanmar_position() = info[i - 1].myanmar_position();
	अनुवर्त;
      }

      यदि (pos == POS_AFTER_MAIN && info[i].myanmar_category() == M_Cat(VBlw))
      {
	pos = POS_BELOW_C;
	info[i].myanmar_position() = pos;
	अनुवर्त;
      }

      यदि (pos == POS_BELOW_C && info[i].myanmar_category() == M_Cat(A))
      {
	info[i].myanmar_position() = POS_BEFORE_SUB;
	अनुवर्त;
      }
      यदि (pos == POS_BELOW_C && info[i].myanmar_category() == M_Cat(VBlw))
      {
	info[i].myanmar_position() = pos;
	अनुवर्त;
      }
      यदि (pos == POS_BELOW_C && info[i].myanmar_category() != M_Cat(A))
      {
	pos = POS_AFTER_SUB;
	info[i].myanmar_position() = pos;
	अनुवर्त;
      }
      info[i].myanmar_position() = pos;
    }
  }

  /* Sit tight, rock 'n roll! */
  buffer->sort (start, end, compare_myanmar_order);

  /* Flip left-matra sequence. */
  अचिन्हित first_बायाँ_matra = end;
  अचिन्हित last_बायाँ_matra = end;
  प्रति (अचिन्हित पूर्णाङ्क i = start; i < end; i++)
  {
    यदि (info[i].myanmar_position() == POS_PRE_M)
    {
      यदि (first_बायाँ_matra == end)
	first_बायाँ_matra = i;
      last_बायाँ_matra = i;
    }
  }
  /* https://github.com/harfbuzz/harfbuzz/issues/3863 */
  यदि (first_बायाँ_matra < last_बायाँ_matra)
  {
    /* No need to merge clusters, done already? */
    buffer->reverse_range (first_बायाँ_matra, last_बायाँ_matra + 1);
    /* Reverse back VS, etc. */
    अचिन्हित i = first_बायाँ_matra;
    प्रति (अचिन्हित j = i; j <= last_बायाँ_matra; j++)
      यदि (info[j].myanmar_category() == M_Cat(VPre))
      {
	buffer->reverse_range (i, j + 1);
	i = j + 1;
      }
  }
}

स्थिर शून्य
reorder_syllable_myanmar (ध्रुव hb_ot_shape_plan_t *plan HB_UNUSED,
			  hb_face_t *face HB_UNUSED,
			  hb_buffer_t *buffer,
			  अचिन्हित पूर्णाङ्क start, अचिन्हित पूर्णाङ्क end)
{
  myanmar_syllable_type_t syllable_type = (myanmar_syllable_type_t) (buffer->info[start].syllable() & 0x0F);
  विकल्प (syllable_type) {

    पक्ष myanmar_broken_cluster: /* We already inserted dotted-circles, so just call the consonant_syllable. */
    पक्ष myanmar_consonant_syllable:
      initial_reordering_consonant_syllable  (buffer, start, end);
      भङ्ग;

    पक्ष myanmar_non_myanmar_cluster:
      भङ्ग;
  }
}

स्थिर तार्किक
reorder_myanmar (ध्रुव hb_ot_shape_plan_t *plan,
		 hb_font_t *font,
		 hb_buffer_t *buffer)
{
  तार्किक ret = असत्य;
  यदि (buffer->message (font, "start reordering myanmar"))
  {
    यदि (hb_syllabic_जोड़ो_dotted_circles (font, buffer,
					   myanmar_broken_cluster,
					   M_Cat(DOTTEDCIRCLE)))
      ret = सत्य;

    foreach_syllable (buffer, start, end)
      reorder_syllable_myanmar (plan, font->face, buffer, start, end);
    (शून्य) buffer->message (font, "end reordering myanmar");
  }

  HB_BUFFER_DEALLOCATE_VAR (buffer, myanmar_category);
  HB_BUFFER_DEALLOCATE_VAR (buffer, myanmar_position);

  प्रत्यागम ret;
}


ध्रुव hb_ot_shaper_t _hb_ot_shaper_myanmar =
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
  असत्य, /* fallback_position */
};


#ifndef HB_NO_OT_SHAPER_MYANMAR_ZAWGYI
/* Ugly Zawgyi encoding.
 * Disable all auto processing.
 * https://github.com/harfbuzz/harfbuzz/issues/1162 */
ध्रुव hb_ot_shaper_t _hb_ot_shaper_myanmar_zawgyi =
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
  असत्य, /* fallback_position */
};
#endif


#endif

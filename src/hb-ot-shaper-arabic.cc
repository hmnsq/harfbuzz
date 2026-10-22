/*
 * Copyright © 2010,2012  Google, Inc.
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

#अन्तर्भाव "hb-ot-shaper-arabic.hh"
#अन्तर्भाव "hb-ot-shape.hh"


/* buffer var allocations */
#define arabic_shaping_action() ot_shaper_var_u8_auxiliary() /* arabic shaping action */

/* buffer **position** var allocations, used after GPOS */
#define stch_advance() var.i32 /* advance used for stretching */

#define HB_BUFFER_SCRATCH_FLAG_ARABIC_HAS_STCH HB_BUFFER_SCRATCH_FLAG_SHAPER0

/* See:
 * https://github.com/harfbuzz/harfbuzz/commit/6e6f82b6f3dde0fc6c3c7d991d9ec6cfff57823d#commitcomment-14248516 */
#define HB_ARABIC_GENERAL_CATEGORY_IS_WORD(gen_cat) \
	(FLAG_UNSAFE (gen_cat) & \
	 (FLAG (HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_PRIVATE_USE) | \
	  /*FLAG (HB_UNICODE_GENERAL_CATEGORY_LOWERCASE_LETTER) |*/ \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_MODIFIER_LETTER) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER) | \
	  /*FLAG (HB_UNICODE_GENERAL_CATEGORY_TITLECASE_LETTER) |*/ \
	  /*FLAG (HB_UNICODE_GENERAL_CATEGORY_UPPERCASE_LETTER) |*/ \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_SPACING_MARK) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_ENCLOSING_MARK) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_DECIMAL_संख्या) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_LETTER_संख्या) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_OTHER_संख्या) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_CURRENCY_चिन्ह) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_MODIFIER_चिन्ह) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_MATH_चिन्ह) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_OTHER_चिन्ह)))


/*
 * Joining types:
 */

/*
 * Bits used in the joining tables
 */
गणना hb_arabic_joining_type_t {
  JOINING_TYPE_U		= 0,
  JOINING_TYPE_L		= 1,
  JOINING_TYPE_R		= 2,
  JOINING_TYPE_D		= 3,
  JOINING_TYPE_C		= JOINING_TYPE_D,
  JOINING_GROUP_ALAPH		= 4,
  JOINING_GROUP_DALATH_RISH	= 5,
  अंक_STATE_MACHINE_COLS	= 6,

  JOINING_TYPE_T = 6,
  JOINING_TYPE_X = 7  /* means: use general-category to choose between U or T. */
};

#अन्तर्भाव "hb-ot-shaper-arabic-table.hh"

स्थिर अचिन्हित पूर्णांक get_joining_type (hb_codepoint_t u, hb_unicode_general_category_t gen_cat)
{
  अचिन्हित पूर्णांक j_type = joining_type(u);
  जर (likely (j_type != JOINING_TYPE_X))
    परत j_type;

  परत (FLAG_UNSAFE(gen_cat) &
	  (FLAG(HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK) |
	   FLAG(HB_UNICODE_GENERAL_CATEGORY_ENCLOSING_MARK) |
	   FLAG(HB_UNICODE_GENERAL_CATEGORY_FORMAT))
	 ) ?  JOINING_TYPE_T : JOINING_TYPE_U;
}

#define FEATURE_IS_SYRIAC(tag) hb_in_range<अचिन्हित अक्षर> ((अचिन्हित अक्षर) (tag), '2', '3')

स्थिर स्थिरांक hb_tag_t arabic_features[] =
{
  HB_TAG('i','s','o','l'),
  HB_TAG('f','i','n','a'),
  HB_TAG('f','i','n','2'),
  HB_TAG('f','i','n','3'),
  HB_TAG('m','e','d','i'),
  HB_TAG('m','e','d','2'),
  HB_TAG('i','n','i','t'),
  HB_TAG_NONE
};


/* Same order as the feature array */
गणना arabic_action_t {
  ISOL,
  FINA,
  FIN2,
  FIN3,
  MEDI,
  MED2,
  INIT,

  NONE,

  ARABIC_अंक_FEATURES = NONE,

  /* We abuse the same byte for other things... */
  STCH_FIXED,
  STCH_REPEATING,
};

स्थिर स्थिरांक संरचना arabic_state_table_entry {
	uint8_t prev_action;
	uint8_t curr_action;
	uint16_t next_state;
} arabic_state_table[][अंक_STATE_MACHINE_COLS] =
{
  /*   jt_U,          jt_L,          jt_R,          jt_D,          jg_ALAPH,      jg_DALATH_RISH */

  /* State 0: prev was U, not willing to join. */
  { {NONE,NONE,0}, {NONE,ISOL,2}, {NONE,ISOL,1}, {NONE,ISOL,2}, {NONE,ISOL,1}, {NONE,ISOL,6}, },

  /* State 1: prev was R or ISOL/ALAPH, not willing to join. */
  { {NONE,NONE,0}, {NONE,ISOL,2}, {NONE,ISOL,1}, {NONE,ISOL,2}, {NONE,FIN2,5}, {NONE,ISOL,6}, },

  /* State 2: prev was D/L in ISOL form, willing to join. */
  { {NONE,NONE,0}, {NONE,ISOL,2}, {INIT,FINA,1}, {INIT,FINA,3}, {INIT,FINA,4}, {INIT,FINA,6}, },

  /* State 3: prev was D in FINA form, willing to join. */
  { {NONE,NONE,0}, {NONE,ISOL,2}, {MEDI,FINA,1}, {MEDI,FINA,3}, {MEDI,FINA,4}, {MEDI,FINA,6}, },

  /* State 4: prev was FINA ALAPH, not willing to join. */
  { {NONE,NONE,0}, {NONE,ISOL,2}, {MED2,ISOL,1}, {MED2,ISOL,2}, {MED2,FIN2,5}, {MED2,ISOL,6}, },

  /* State 5: prev was FIN2/FIN3 ALAPH, not willing to join. */
  { {NONE,NONE,0}, {NONE,ISOL,2}, {ISOL,ISOL,1}, {ISOL,ISOL,2}, {ISOL,FIN2,5}, {ISOL,ISOL,6}, },

  /* State 6: prev was DALATH/RISH, not willing to join. */
  { {NONE,NONE,0}, {NONE,ISOL,2}, {NONE,ISOL,1}, {NONE,ISOL,2}, {NONE,FIN3,5}, {NONE,ISOL,6}, }
};


स्थिर तार्किक
arabic_fallback_shape (स्थिरांक hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_buffer_t *buffer);

स्थिर तार्किक
record_stch (स्थिरांक hb_ot_shape_plan_t *plan,
	     hb_font_t *font,
	     hb_buffer_t *buffer);

स्थिर तार्किक
deallocate_buffer_var (स्थिरांक hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_buffer_t *buffer)
{
  HB_BUFFER_DEALLOCATE_VAR (buffer, arabic_shaping_action);
  परत असत्य;
}

स्थिर शून्य
collect_features_arabic (hb_ot_shape_planner_t *plan)
{
  hb_ot_map_builder_t *map = &plan->map;

  /* We apply features according to the Arabic spec, with pauses
   * in between most.
   *
   * The pause between init/medi/... and rlig is required.  See eg:
   * https://bugzilla.mozilla.org/show_bug.cgi?id=644184
   *
   * The pauses between init/medi/... themselves are not necessarily
   * needed as only one of those features is applied to any character.
   * The only difference it makes is when fonts have contextual
   * substitutions.  We now follow the order of the spec, which makes
   * for better experience if that's what Uniscribe is doing.
   *
   * At least for Arabic, looks like Uniscribe has a pause between
   * rlig and calt.  Otherwise the IranNastaliq's ALLAH ligature won't
   * work.  However, testing shows that rlig and calt are applied
   * together for Mongolian in Uniscribe.  As such, we only add a
   * pause for Arabic, not other scripts.
   */


  map->enable_feature (HB_TAG('s','t','c','h'));
  map->add_gsub_pause (record_stch);

  map->enable_feature (HB_TAG('c','c','m','p'), F_MANUAL_ZWJ);
  map->enable_feature (HB_TAG('l','o','c','l'), F_MANUAL_ZWJ);

  map->add_gsub_pause (nullptr);

  साठी (अचिन्हित पूर्णांक i = 0; i < ARABIC_अंक_FEATURES; i++)
  {
    तार्किक has_fallback = plan->props.script == HB_SCRIPT_ARABIC && !FEATURE_IS_SYRIAC (arabic_features[i]);
    map->add_feature (arabic_features[i], F_MANUAL_ZWJ | (has_fallback ? F_HAS_FALLBACK : F_NONE));
    map->add_gsub_pause (nullptr);
  }
   map->add_gsub_pause (deallocate_buffer_var);

  /* Normally, Unicode says a ZWNJ means "don't ligate".  In Arabic script
   * however, it says a ZWJ should also mean "don't ligate".  So we run
   * the main ligating features as MANUAL_ZWJ. */

  map->enable_feature (HB_TAG('r','l','i','g'), F_MANUAL_ZWJ | F_HAS_FALLBACK);

  जर (plan->props.script == HB_SCRIPT_ARABIC)
    map->add_gsub_pause (arabic_fallback_shape);

   map->enable_feature (HB_TAG('c','a','l','t'), F_MANUAL_ZWJ);
   /* https://github.com/harfbuzz/harfbuzz/issues/1573 */
   जर (!map->has_feature (HB_TAG('r','c','l','t')))
     map->add_gsub_pause (nullptr);

   map->enable_feature (HB_TAG('l','i','g','a'), F_MANUAL_ZWJ);
   map->enable_feature (HB_TAG('c','l','i','g'), F_MANUAL_ZWJ);

  /* The spec includes 'cswh'.  Earlier versions of Windows
   * used to enable this by default, but testing suggests
   * that Windows 8 and later do not enable it by default,
   * and spec now says 'Off by default'.
   * We disabled this in ae23c24c32.
   * Note that IranNastaliq uses this feature extensively
   * to fixup broken glyph sequences.  Oh well...
   * Test case: U+0643,U+0640,U+0631. */
  //map->enable_feature (HB_TAG('c','s','w','h'), F_MANUAL_ZWJ);
  map->enable_feature (HB_TAG('m','s','e','t'), F_MANUAL_ZWJ);
}

#अन्तर्भाव "hb-ot-shaper-arabic-fallback.hh"

संरचना arabic_shape_plan_t
{
  /* The "+ 1" in the next array is to accommodate for the "NONE" command,
   * which is not an OpenType feature, but this simplifies the code by not
   * having to do a "if (... < NONE) ..." and just rely on the fact that
   * mask_array[NONE] == 0. */
  hb_mask_t mask_array[ARABIC_अंक_FEATURES + 1];

  mutable hb_atomic_t<arabic_fallback_plan_t *> fallback_plan;

  अचिन्हित पूर्णांक do_fallback : 1;
  अचिन्हित पूर्णांक has_stch : 1;
};

शून्य *
data_create_arabic (स्थिरांक hb_ot_shape_plan_t *plan)
{
  arabic_shape_plan_t *arabic_plan = (arabic_shape_plan_t *) hb_calloc (1, आकार (arabic_shape_plan_t));
  जर (unlikely (!arabic_plan))
    परत nullptr;

  arabic_plan->do_fallback = plan->props.script == HB_SCRIPT_ARABIC;
  arabic_plan->has_stch = !!plan->map.get_1_mask (HB_TAG ('s','t','c','h'));
  साठी (अचिन्हित पूर्णांक i = 0; i < ARABIC_अंक_FEATURES; i++) {
    arabic_plan->mask_array[i] = plan->map.get_1_mask (arabic_features[i]);
    arabic_plan->do_fallback = arabic_plan->do_fallback &&
			       (FEATURE_IS_SYRIAC (arabic_features[i]) ||
				plan->map.needs_fallback (arabic_features[i]));
  }

  परत arabic_plan;
}

शून्य
data_destroy_arabic (शून्य *data)
{
  arabic_shape_plan_t *arabic_plan = (arabic_shape_plan_t *) data;

  arabic_fallback_plan_destroy (arabic_plan->fallback_plan);

  hb_free (data);
}

स्थिर शून्य
arabic_joining (hb_buffer_t *buffer)
{
  अचिन्हित पूर्णांक मोजणी = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  अचिन्हित पूर्णांक prev = अच_पूर्णांक_उच्च, state = 0;

  /* Check pre-context */
  साठी (अचिन्हित पूर्णांक i = 0; i < buffer->context_len[0]; i++)
  {
    अचिन्हित पूर्णांक this_type = get_joining_type (buffer->context[0][i], buffer->unicode->general_category (buffer->context[0][i]));

    जर (unlikely (this_type == JOINING_TYPE_T))
      सुरूठेवा;

    स्थिरांक arabic_state_table_entry *entry = &arabic_state_table[state][this_type];
    state = entry->next_state;
    थांबा;
  }

  साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
  {
    अचिन्हित पूर्णांक this_type = get_joining_type (info[i].codepoint, _hb_glyph_info_get_general_category (&info[i]));

    जर (unlikely (this_type == JOINING_TYPE_T)) {
      info[i].arabic_shaping_action() = NONE;
      सुरूठेवा;
    }

    स्थिरांक arabic_state_table_entry *entry = &arabic_state_table[state][this_type];

    जर (entry->prev_action != NONE && prev != अच_पूर्णांक_उच्च)
    {
      info[prev].arabic_shaping_action() = entry->prev_action;
      buffer->safe_to_घाला_tatweel (prev, i + 1);
    }
    अन्यथा
    {
      जर (prev == अच_पूर्णांक_उच्च)
      {
        जर (this_type >= JOINING_TYPE_R)
	  buffer->unsafe_to_concat_from_outbuffer (0, i + 1);
      }
      अन्यथा
      {
	जर (this_type >= JOINING_TYPE_R ||
	    (2 <= state && state <= 5) /* States that have a possible prev_action. */)
	  buffer->unsafe_to_concat (prev, i + 1);
      }
    }

    info[i].arabic_shaping_action() = entry->curr_action;

    prev = i;
    state = entry->next_state;
  }

  साठी (अचिन्हित पूर्णांक i = 0; i < buffer->context_len[1]; i++)
  {
    अचिन्हित पूर्णांक this_type = get_joining_type (buffer->context[1][i], buffer->unicode->general_category (buffer->context[1][i]));

    जर (unlikely (this_type == JOINING_TYPE_T))
      सुरूठेवा;

    स्थिरांक arabic_state_table_entry *entry = &arabic_state_table[state][this_type];
    जर (entry->prev_action != NONE && prev != अच_पूर्णांक_उच्च)
    {
      info[prev].arabic_shaping_action() = entry->prev_action;
      buffer->safe_to_घाला_tatweel (prev, buffer->len);
    }
    अन्यथा जर (2 <= state && state <= 5) /* States that have a possible prev_action. */
    {
      buffer->unsafe_to_concat (prev, buffer->len);
    }
    थांबा;
  }
}

स्थिर शून्य
mongolian_variation_selectors (hb_buffer_t *buffer)
{
  /* Copy arabic_shaping_action() from base to Mongolian variation selectors. */
  अचिन्हित पूर्णांक मोजणी = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  साठी (अचिन्हित पूर्णांक i = 1; i < मोजणी; i++)
    जर (unlikely (hb_in_ranges<hb_codepoint_t> (info[i].codepoint, 0x180Bu, 0x180Du, 0x180Fu, 0x180Fu)))
      info[i].arabic_shaping_action() = info[i - 1].arabic_shaping_action();
}

शून्य
setup_masks_arabic_plan (स्थिरांक arabic_shape_plan_t *arabic_plan,
			 hb_buffer_t               *buffer,
			 hb_script_t                script)
{
  HB_BUFFER_ALLOCATE_VAR (buffer, arabic_shaping_action);

  arabic_joining (buffer);
  जर (script == HB_SCRIPT_MONGOLIAN)
    mongolian_variation_selectors (buffer);

  अचिन्हित पूर्णांक मोजणी = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    info[i].mask |= arabic_plan->mask_array[info[i].arabic_shaping_action()];
}

स्थिर शून्य
setup_masks_arabic (स्थिरांक hb_ot_shape_plan_t *plan,
		    hb_buffer_t              *buffer,
		    hb_font_t                *font HB_UNUSED)
{
  स्थिरांक arabic_shape_plan_t *arabic_plan = (स्थिरांक arabic_shape_plan_t *) plan->data;
  setup_masks_arabic_plan (arabic_plan, buffer, plan->props.script);
}

स्थिर तार्किक
arabic_fallback_shape (स्थिरांक hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_buffer_t *buffer)
{
#ifdef HB_NO_OT_SHAPER_ARABIC_FALLBACK
  परत असत्य;
#endif

  स्थिरांक arabic_shape_plan_t *arabic_plan = (स्थिरांक arabic_shape_plan_t *) plan->data;

  जर (!arabic_plan->do_fallback)
    परत असत्य;

retry:
  arabic_fallback_plan_t *fallback_plan = arabic_plan->fallback_plan;
  जर (unlikely (!fallback_plan))
  {
    /* This sucks.  We need a font to build the fallback plan... */
    fallback_plan = arabic_fallback_plan_create (plan, font);
    जर (unlikely (!arabic_plan->fallback_plan.cmpexch (nullptr, fallback_plan)))
    {
      arabic_fallback_plan_destroy (fallback_plan);
      जा retry;
    }
  }

  arabic_fallback_plan_shape (fallback_plan, font, buffer);
  परत सत्य;
}

/*
 * Stretch feature: "stch".
 * See example here:
 * https://docs.microsoft.com/en-us/typography/script-development/syriac
 * We implement this in a generic way, such that the Arabic subtending
 * marks can use it as well.
 */

स्थिर तार्किक
record_stch (स्थिरांक hb_ot_shape_plan_t *plan,
	     hb_font_t *font HB_UNUSED,
	     hb_buffer_t *buffer)
{
  स्थिरांक arabic_shape_plan_t *arabic_plan = (स्थिरांक arabic_shape_plan_t *) plan->data;
  जर (!arabic_plan->has_stch)
    परत असत्य;

  /* 'stch' feature was just applied.  Look for anything that multiplied,
   * and record it for stch treatment later.  Note that rtlm, frac, etc
   * are applied before stch, but we assume that they didn't result in
   * anything multiplying into 5 pieces, so it's safe-ish... */

  अचिन्हित पूर्णांक मोजणी = buffer->len;
  hb_glyph_info_t *info = buffer->info;
  साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    जर (unlikely (_hb_glyph_info_multiplied (&info[i])))
    {
      अचिन्हित पूर्णांक comp = _hb_glyph_info_get_lig_comp (&info[i]);
      info[i].arabic_shaping_action() = comp % 2 ? STCH_REPEATING : STCH_FIXED;
      buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_ARABIC_HAS_STCH;
    }
  परत असत्य;
}

स्थिर शून्य
apply_stch (स्थिरांक hb_ot_shape_plan_t *plan HB_UNUSED,
	    hb_buffer_t              *buffer,
	    hb_font_t                *font)
{
  जर (likely (!(buffer->scratch_flags & HB_BUFFER_SCRATCH_FLAG_ARABIC_HAS_STCH)))
    परत;

  तार्किक rtl = buffer->props.direction == HB_DIRECTION_RTL;

  जर (!rtl)
    buffer->reverse ();

  /* We do a two pass implementation:
   * First pass calculates the exact number of extra glyphs we need,
   * We then enlarge buffer to have that much room,
   * Second pass applies the stretch, copying things to the end of buffer.
   *
   * Cache glyph advances in the position scratch field during the first pass,
   * for re-use these during the second pass.
   */

  पूर्णांक sign = font->x_scale < 0 ? -1 : +1;
  अचिन्हित पूर्णांक extra_glyphs_needed = 0; // Set during MEASURE, used during CUT
  गणना { MEASURE, CUT } /* step_t */;

  साठी (अचिन्हित पूर्णांक step = MEASURE; step <= CUT; step = step + 1)
  {
    अचिन्हित पूर्णांक मोजणी = buffer->len;
    hb_glyph_info_t *info = buffer->info;
    hb_glyph_position_t *pos = buffer->pos;
    अचिन्हित पूर्णांक new_len = मोजणी + extra_glyphs_needed; // write head during CUT
    अचिन्हित पूर्णांक j = new_len;
    साठी (अचिन्हित पूर्णांक i = मोजणी; i; i--)
    {
      जर (!hb_in_range<uint8_t> (info[i - 1].arabic_shaping_action(), STCH_FIXED, STCH_REPEATING))
      {
	जर (step == CUT)
	{
	  --j;
	  info[j] = info[i - 1];
	  pos[j] = pos[i - 1];
	}
	सुरूठेवा;
      }

      /* Yay, justification! */

      hb_position_t w_total = 0; // Total to be filled
      hb_position_t w_fixed = 0; // Sum of fixed tiles
      hb_position_t w_repeating = 0; // Sum of repeating tiles
      पूर्णांक n_fixed = 0;
      पूर्णांक n_repeating = 0;

      अचिन्हित पूर्णांक end = i;
      पर्यंत (i &&
	     hb_in_range<uint8_t> (info[i - 1].arabic_shaping_action(), STCH_FIXED, STCH_REPEATING))
      {
	i--;
        जर (step == MEASURE)
          pos[i].stch_advance() = font->get_glyph_h_advance (info[i].codepoint);
        hb_position_t width = pos[i].stch_advance();
	जर (info[i].arabic_shaping_action() == STCH_FIXED)
	{
	  w_fixed = hb_saturate_add (w_fixed, width);
	  n_fixed++;
	}
	अन्यथा
	{
	  w_repeating = hb_saturate_add (w_repeating, width);
	  n_repeating++;
	}
      }
      अचिन्हित पूर्णांक start = i;
      अचिन्हित पूर्णांक context = i;
      पर्यंत (context &&
	     !hb_in_range<uint8_t> (info[context - 1].arabic_shaping_action(), STCH_FIXED, STCH_REPEATING) &&
	     (_hb_glyph_info_is_default_ignorable (&info[context - 1]) ||
	      HB_ARABIC_GENERAL_CATEGORY_IS_WORD (_hb_glyph_info_get_general_category (&info[context - 1]))))
      {
	context--;
	w_total = hb_saturate_add (w_total, pos[context].x_advance);
      }
      i++; // Don't touch i again.

      DEBUG_संदेश (ARABIC, nullptr, "%s stretch at (%u,%u,%u)",
		 step == MEASURE ? "measuring" : "cutting", context, start, end);
      DEBUG_संदेश (ARABIC, nullptr, "rest of word:    count=%u width %" PRId32, start - context, w_total);
      DEBUG_संदेश (ARABIC, nullptr, "fixed tiles:     count=%d width=%" PRId32, n_fixed, w_fixed);
      DEBUG_संदेश (ARABIC, nullptr, "repeating tiles: count=%d width=%" PRId32, n_repeating, w_repeating);

      स्थिर constexpr अचिन्हित STCH_MAX_GLYPHS = 256;

      /* Number of additional times to repeat each repeating tile. */
      अचिन्हित पूर्णांक n_copies = 0;

      int64_t w_remaining_signed = (int64_t) w_total - w_fixed;
      int64_t w_repeating_signed = w_repeating;
      जर (sign < 0)
      {
	w_remaining_signed = -w_remaining_signed;
	w_repeating_signed = -w_repeating_signed;
      }
      hb_position_t w_remaining = hb_saturate_sub (w_total, w_fixed);
      जर (w_remaining_signed > w_repeating_signed && w_repeating_signed > 0)
	n_copies = w_remaining_signed / w_repeating_signed - 1;

      /* See if we can improve the fit by adding an extra repeat and squeezing them together a bit. */
      hb_position_t extra_repeat_overlap = 0;
      int64_t shortfall = w_remaining_signed - w_repeating_signed * (n_copies + 1);
      जर (shortfall > 0 && n_repeating > 0)
      {
	++n_copies;
	int64_t excess = (n_copies + 1) * w_repeating_signed - w_remaining_signed;
	जर (excess > 0)
	{
	  extra_repeat_overlap = hb_clamp_to<hb_position_t> (excess / ((int64_t) n_copies * (int64_t) n_repeating));
	  w_remaining = 0;
	}
      }

      अचिन्हित पूर्णांक max_copies = 0;
      जर (n_repeating > 0)
      {
	अचिन्हित पूर्णांक base_glyphs = n_fixed + n_repeating;
	जर (base_glyphs < STCH_MAX_GLYPHS)
	  max_copies = (STCH_MAX_GLYPHS - base_glyphs) / n_repeating;
      }
      n_copies = hb_min (n_copies, max_copies);

      जर (step == MEASURE)
      {
	अचिन्हित पूर्णांक added_glyphs = 0;
	जर (unlikely (hb_unsigned_mul_overflows (n_copies, n_repeating, &added_glyphs) ||
		      hb_unsigned_add_overflows (extra_glyphs_needed, added_glyphs, &extra_glyphs_needed)))
	{
	  extra_glyphs_needed = अच_पूर्णांक_उच्च;
	  थांबा;
	}
	DEBUG_संदेश (ARABIC, nullptr, "will add extra %u copies of repeating tiles", n_copies);
      }
      अन्यथा
      {
	buffer->unsafe_to_break (context, end);
	hb_position_t x_offset = w_remaining / 2;
	साठी (अचिन्हित पूर्णांक k = end; k > start; k--)
	{
          hb_position_t width = pos[k - 1].stch_advance();

	  अचिन्हित पूर्णांक repeat = 1;
	  जर (info[k - 1].arabic_shaping_action() == STCH_REPEATING)
	    repeat += n_copies;

	  DEBUG_संदेश (ARABIC, nullptr, "appending %u copies of glyph %" PRIu32 "; j=%u",
		     repeat, info[k - 1].codepoint, j);
	  pos[k - 1].x_advance = 0;
	  साठी (अचिन्हित पूर्णांक n = 0; n < repeat; n++)
	  {
	    जर (rtl)
	    {
	      x_offset = hb_saturate_sub (x_offset, width);
	      जर (n > 0)
		x_offset = hb_saturate_add (x_offset, extra_repeat_overlap);
	    }
	    pos[k - 1].x_offset = x_offset;
	    /* Append copy. */
	    --j;
	    info[j] = info[k - 1];
	    pos[j] = pos[k - 1];

	    जर (!rtl)
	    {
	      x_offset = hb_saturate_add (x_offset, width);
	      जर (n > 0)
		x_offset = hb_saturate_sub (x_offset, extra_repeat_overlap);
	    }
	  }
	}
      }
    }

    जर (step == MEASURE)
    {
      अचिन्हित पूर्णांक total_glyphs = 0;
      जर (unlikely (hb_unsigned_add_overflows (मोजणी, extra_glyphs_needed, &total_glyphs) ||
		    !buffer->ensure (total_glyphs)))
	थांबा;
    }
    अन्यथा
    {
      प्रतिज्ञा (j == 0);
      buffer->len = new_len;
    }
  }

  जर (!rtl)
    buffer->reverse ();
}


स्थिर शून्य
postprocess_glyphs_arabic (स्थिरांक hb_ot_shape_plan_t *plan,
			   hb_buffer_t              *buffer,
			   hb_font_t                *font)
{
  apply_stch (plan, buffer, font);
}

/* https://www.unicode.org/reports/tr53/ */

स्थिर स्थिरांक hb_codepoint_t
modifier_combining_marks[] =
{
  0x0654u, /* ARABIC HAMZA ABOVE */
  0x0655u, /* ARABIC HAMZA BELOW */
  0x0658u, /* ARABIC MARK NOON GHUNNA */
  0x06DCu, /* ARABIC SMALL HIGH SEEN */
  0x06E3u, /* ARABIC SMALL LOW SEEN */
  0x06E7u, /* ARABIC SMALL HIGH YEH */
  0x06E8u, /* ARABIC SMALL HIGH NOON */
  0x08CAu, /* ARABIC SMALL HIGH FARSI YEH */
  0x08CBu, /* ARABIC SMALL HIGH YEH BARREE WITH TWO DOTS BELOW */
  0x08CDu, /* ARABIC SMALL HIGH ZAH */
  0x08CEu, /* ARABIC LARGE ROUND DOT ABOVE */
  0x08CFu, /* ARABIC LARGE ROUND DOT BELOW */
  0x08D3u, /* ARABIC SMALL LOW WAW */
  0x08F3u, /* ARABIC SMALL HIGH WAW */
};

स्थिर ओळीत तार्किक
info_is_mcm (स्थिरांक hb_glyph_info_t &info)
{
  hb_codepoint_t u = info.codepoint;
  साठी (अचिन्हित पूर्णांक i = 0; i < ARRAY_LENGTH (modifier_combining_marks); i++)
    जर (u == modifier_combining_marks[i])
      परत सत्य;
  परत असत्य;
}

स्थिर शून्य
reorder_marks_arabic (स्थिरांक hb_ot_shape_plan_t *plan HB_UNUSED,
		      hb_buffer_t              *buffer,
		      अचिन्हित पूर्णांक              start,
		      अचिन्हित पूर्णांक              end)
{
  hb_glyph_info_t *info = buffer->info;

  DEBUG_संदेश (ARABIC, buffer, "Reordering marks from %u to %u", start, end);

  अचिन्हित पूर्णांक i = start;
  साठी (अचिन्हित पूर्णांक cc = 220; cc <= 230; cc += 10)
  {
    DEBUG_संदेश (ARABIC, buffer, "Looking for %u's starting at %u", cc, i);
    पर्यंत (i < end && info_cc(info[i]) < cc)
      i++;
    DEBUG_संदेश (ARABIC, buffer, "Looking for %u's stopped at %u", cc, i);

    जर (i == end)
      थांबा;

    जर (info_cc(info[i]) > cc)
      सुरूठेवा;

    अचिन्हित पूर्णांक j = i;
    पर्यंत (j < end && info_cc(info[j]) == cc && info_is_mcm (info[j]))
      j++;

    जर (i == j)
      सुरूठेवा;

    DEBUG_संदेश (ARABIC, buffer, "Found %u's from %u to %u", cc, i, j);

    /* Shift it! */
    DEBUG_संदेश (ARABIC, buffer, "Shifting %u's: %u %u", cc, i, j);
    hb_glyph_info_t temp[HB_OT_SHAPE_MAX_COMBINING_MARKS];
    प्रतिज्ञा (j - i <= ARRAY_LENGTH (temp));
    buffer->merge_clusters (start, j);
    स_हटाओ (temp, &info[i], (j - i) * आकार (hb_glyph_info_t));
    स_हटाओ (&info[start + j - i], &info[start], (i - start) * आकार (hb_glyph_info_t));
    स_हटाओ (&info[start], temp, (j - i) * आकार (hb_glyph_info_t));

    /* Renumber CC such that the reordered sequence is still sorted.
     * 22 and 26 are chosen because they are smaller than all Arabic categories,
     * and are folded back to 220/230 respectively during fallback mark positioning.
     *
     * We do this because the CGJ-handling logic in the normalizer relies on
     * mark sequences having an increasing order even after this reordering.
     * https://github.com/harfbuzz/harfbuzz/issues/554
     * This, however, does break some obscure sequences, where the normalizer
     * might compose a sequence that it should not.  For example, in the seequence
     * ALEF, HAMZAH, MADDAH, we should NOT try to compose ALEF+MADDAH, but with this
     * renumbering, we will.
     */
    अचिन्हित पूर्णांक new_start = start + j - i;
    अचिन्हित पूर्णांक new_cc = cc == 220 ? HB_MODIFIED_COMBINING_CLASS_CCC22 : HB_MODIFIED_COMBINING_CLASS_CCC26;
    पर्यंत (start < new_start)
    {
      _hb_glyph_info_set_modified_combining_class (&info[start], new_cc);
      start++;
    }

    i = j;
  }
}

स्थिरांक hb_ot_shaper_t _hb_ot_shaper_arabic =
{
  collect_features_arabic,
  nullptr, /* override_features */
  data_create_arabic,
  data_destroy_arabic,
  nullptr, /* preprocess_text */
  postprocess_glyphs_arabic,
  nullptr, /* decompose */
  nullptr, /* compose */
  setup_masks_arabic,
  reorder_marks_arabic,
  HB_TAG_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_MODE_DEFAULT,
  HB_OT_SHAPE_ZERO_WIDTH_MARKS_BY_GDEF_LATE,
  सत्य, /* fallback_position */
};


#endif

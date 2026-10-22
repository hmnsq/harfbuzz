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

#ضمن "hb.hh"

#که_نه_تعریف_شوی HB_NO_OT_SHAPE

#ضمن "hb-ot-shaper-arabic.hh"
#ضمن "hb-ot-shape.hh"


/* buffer var allocations */
#تعریف_کړه arabic_shaping_action() ot_shaper_var_u8_auxiliary() /* arabic shaping action */

/* buffer **position** var allocations, used after GPOS */
#تعریف_کړه stch_advance() var.i32 /* advance used for stretching */

#تعریف_کړه HB_بفر_SCRATCH_FLAG_ARABIC_HAS_STCH HB_بفر_SCRATCH_FLAG_SHAPER0

/* See:
 * https://github.com/harfbuzz/harfbuzz/commit/6e6f82b6f3dde0fc6c3c7d991d9ec6cfff57823d#commitcomment-14248516 */
#تعریف_کړه HB_ARABIC_GENERAL_CATEGORY_IS_کلمه(gen_cat) \
	(FLAG_UNSAFE (gen_cat) & \
	 (FLAG (HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_شخصي_USE) | \
	  /*FLAG (HB_UNICODE_GENERAL_CATEGORY_LOWERCASE_LETTER) |*/ \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_MODIFIER_LETTER) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER) | \
	  /*FLAG (HB_UNICODE_GENERAL_CATEGORY_TITLECASE_LETTER) |*/ \
	  /*FLAG (HB_UNICODE_GENERAL_CATEGORY_UPPERCASE_LETTER) |*/ \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_SPACING_MARK) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_ENCLOSING_MARK) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_DECIMAL_شمېره) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_LETTER_شمېره) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_OTHER_شمېره) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_CURRENCY_نښه) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_MODIFIER_نښه) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_MATH_نښه) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_OTHER_نښه)))


/*
 * Joining types:
 */

/*
 * Bits used in the joining tables
 */
تعداد hb_arabic_joining_type_t {
  JOINING_TYPE_U		= 0,
  JOINING_TYPE_L		= 1,
  JOINING_TYPE_R		= 2,
  JOINING_TYPE_D		= 3,
  JOINING_TYPE_C		= JOINING_TYPE_D,
  JOINING_ډله_ALAPH		= 4,
  JOINING_ډله_DALATH_RISH	= 5,
  عدد_STATE_MACHINE_ستون_شمېر	= 6,

  JOINING_TYPE_T = 6,
  JOINING_TYPE_X = 7  /* means: use general-category to choose between U or T. */
};

#ضمن "hb-ot-shaper-arabic-table.hh"

ساكن بینښه صحيح get_joining_type (hb_codepoint_t u, hb_unicode_general_category_t gen_cat)
{
  بینښه صحيح j_type = joining_type(u);
  إذا (likely (j_type != JOINING_TYPE_X))
    أرجع j_type;

  أرجع (FLAG_UNSAFE(gen_cat) &
	  (FLAG(HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK) |
	   FLAG(HB_UNICODE_GENERAL_CATEGORY_ENCLOSING_MARK) |
	   FLAG(HB_UNICODE_GENERAL_CATEGORY_FORMAT))
	 ) ?  JOINING_TYPE_T : JOINING_TYPE_U;
}

#تعریف_کړه FEATURE_IS_SYRIAC(نښان_2) hb_دننه_range<بینښه حرف> ((بینښه حرف) (نښان_2), '2', '3')

ساكن ثابت hb_نښان_t arabic_features[] =
{
  HB_نښان('i','s','o','l'),
  HB_نښان('f','i','n','a'),
  HB_نښان('f','i','n','2'),
  HB_نښان('f','i','n','3'),
  HB_نښان('m','e','d','i'),
  HB_نښان('m','e','d','2'),
  HB_نښان('i','n','i','t'),
  HB_نښان_NONE
};


/* Same order as the feature array */
تعداد arabic_action_t {
  ISOL,
  FINA,
  FIN2,
  FIN3,
  MEDI,
  MED2,
  INIT,

  NONE,

  ARABIC_عدد_FEATURES = NONE,

  /* We abuse the same byte for other things... */
  STCH_FIXED,
  STCH_REPEATING,
};

ساكن ثابت بنية arabic_state_جدول_ثبت {
	uint8_t prev_action;
	uint8_t curr_action;
	uint16_t بل_state;
} arabic_state_جدول[][عدد_STATE_MACHINE_ستون_شمېر] =
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


ساكن منطقي
arabic_fallback_shape (ثابت hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_بفر_t *بفر);

ساكن منطقي
ریکارډ_stch (ثابت hb_ot_shape_plan_t *plan,
	     hb_font_t *font,
	     hb_بفر_t *بفر);

ساكن منطقي
deallocate_بفر_var (ثابت hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_بفر_t *بفر)
{
  HB_بفر_DEALLOCATE_VAR (بفر, arabic_shaping_action);
  أرجع خطأ;
}

ساكن فراغ
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


  map->enable_feature (HB_نښان('s','t','c','h'));
  map->جمع_کړه_gsub_pause (ریکارډ_stch);

  map->enable_feature (HB_نښان('c','c','m','p'), F_MANUAL_ZWJ);
  map->enable_feature (HB_نښان('l','o','c','l'), F_MANUAL_ZWJ);

  map->جمع_کړه_gsub_pause (تش_اشاره);

  لكل (بینښه صحيح i = 0; i < ARABIC_عدد_FEATURES; i++)
  {
    منطقي has_fallback = plan->props.script == HB_SCRIPT_ARABIC && !FEATURE_IS_SYRIAC (arabic_features[i]);
    map->جمع_کړه_feature (arabic_features[i], F_MANUAL_ZWJ | (has_fallback ? F_HAS_FALLBACK : F_NONE));
    map->جمع_کړه_gsub_pause (تش_اشاره);
  }
   map->جمع_کړه_gsub_pause (deallocate_بفر_var);

  /* Normally, Unicode says a ZWNJ means "don't ligate".  In Arabic script
   * however, it says a ZWJ should also mean "don't ligate".  So we run
   * the main ligating features as MANUAL_ZWJ. */

  map->enable_feature (HB_نښان('r','l','i','g'), F_MANUAL_ZWJ | F_HAS_FALLBACK);

  إذا (plan->props.script == HB_SCRIPT_ARABIC)
    map->جمع_کړه_gsub_pause (arabic_fallback_shape);

   map->enable_feature (HB_نښان('c','a','l','t'), F_MANUAL_ZWJ);
   /* https://github.com/harfbuzz/harfbuzz/issues/1573 */
   إذا (!map->has_feature (HB_نښان('r','c','l','t')))
     map->جمع_کړه_gsub_pause (تش_اشاره);

   map->enable_feature (HB_نښان('l','i','g','a'), F_MANUAL_ZWJ);
   map->enable_feature (HB_نښان('c','l','i','g'), F_MANUAL_ZWJ);

  /* The spec includes 'cswh'.  Earlier versions of Windows
   * used to enable this by default, but testing suggests
   * that Windows 8 and later do not enable it by default,
   * and spec now says 'Off by default'.
   * We disabled this in ae23c24c32.
   * Note that IranNastaliq uses this feature extensively
   * to fixup broken glyph sequences.  Oh well...
   * Test case: U+0643,U+0640,U+0631. */
  //map->enable_feature (HB_TAG('c','s','w','h'), F_MANUAL_ZWJ);
  map->enable_feature (HB_نښان('m','s','e','t'), F_MANUAL_ZWJ);
}

#ضمن "hb-ot-shaper-arabic-fallback.hh"

بنية arabic_shape_plan_t
{
  /* The "+ 1" in the next array is to accommodate for the "NONE" command,
   * which is not an OpenType feature, but this simplifies the code by not
   * having to do a "if (... < NONE) ..." and just rely on the fact that
   * mask_array[NONE] == 0. */
  hb_ماسک_t ماسک_array[ARABIC_عدد_FEATURES + 1];

  بدلیدونکی hb_atomic_t<arabic_fallback_plan_t *> fallback_plan;

  بینښه صحيح do_fallback : 1;
  بینښه صحيح has_stch : 1;
};

فراغ *
معلومات_create_arabic (ثابت hb_ot_shape_plan_t *plan)
{
  arabic_shape_plan_t *arabic_plan = (arabic_shape_plan_t *) hb_calloc (1, حجم (arabic_shape_plan_t));
  إذا (unlikely (!arabic_plan))
    أرجع تش_اشاره;

  arabic_plan->do_fallback = plan->props.script == HB_SCRIPT_ARABIC;
  arabic_plan->has_stch = !!plan->map.get_1_ماسک (HB_نښان ('s','t','c','h'));
  لكل (بینښه صحيح i = 0; i < ARABIC_عدد_FEATURES; i++) {
    arabic_plan->ماسک_array[i] = plan->map.get_1_ماسک (arabic_features[i]);
    arabic_plan->do_fallback = arabic_plan->do_fallback &&
			       (FEATURE_IS_SYRIAC (arabic_features[i]) ||
				plan->map.needs_fallback (arabic_features[i]));
  }

  أرجع arabic_plan;
}

فراغ
معلومات_destroy_arabic (فراغ *معلومات)
{
  arabic_shape_plan_t *arabic_plan = (arabic_shape_plan_t *) معلومات;

  arabic_fallback_plan_destroy (arabic_plan->fallback_plan);

  hb_free (معلومات);
}

ساكن فراغ
arabic_joining (hb_بفر_t *بفر)
{
  بینښه صحيح شمېر = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  بینښه صحيح prev = UINT_MAX, state = 0;

  /* Check pre-context */
  لكل (بینښه صحيح i = 0; i < بفر->context_len[0]; i++)
  {
    بینښه صحيح this_type = get_joining_type (بفر->context[0][i], بفر->unicode->general_category (بفر->context[0][i]));

    إذا (unlikely (this_type == JOINING_TYPE_T))
      استمر;

    ثابت arabic_state_جدول_ثبت *ثبت = &arabic_state_جدول[state][this_type];
    state = ثبت->بل_state;
    اكسر;
  }

  لكل (بینښه صحيح i = 0; i < شمېر; i++)
  {
    بینښه صحيح this_type = get_joining_type (info[i].codepoint, _hb_glyph_info_get_general_category (&info[i]));

    إذا (unlikely (this_type == JOINING_TYPE_T)) {
      info[i].arabic_shaping_action() = NONE;
      استمر;
    }

    ثابت arabic_state_جدول_ثبت *ثبت = &arabic_state_جدول[state][this_type];

    إذا (ثبت->prev_action != NONE && prev != UINT_MAX)
    {
      info[prev].arabic_shaping_action() = ثبت->prev_action;
      بفر->خوندي_منزل_ورننباسه_tatweel (prev, i + 1);
    }
    وإلا
    {
      إذا (prev == UINT_MAX)
      {
        إذا (this_type >= JOINING_TYPE_R)
	  بفر->unsafe_منزل_concat_from_outbuffer (0, i + 1);
      }
      وإلا
      {
	إذا (this_type >= JOINING_TYPE_R ||
	    (2 <= state && state <= 5) /* States that have a possible prev_action. */)
	  بفر->unsafe_منزل_concat (prev, i + 1);
      }
    }

    info[i].arabic_shaping_action() = ثبت->curr_action;

    prev = i;
    state = ثبت->بل_state;
  }

  لكل (بینښه صحيح i = 0; i < بفر->context_len[1]; i++)
  {
    بینښه صحيح this_type = get_joining_type (بفر->context[1][i], بفر->unicode->general_category (بفر->context[1][i]));

    إذا (unlikely (this_type == JOINING_TYPE_T))
      استمر;

    ثابت arabic_state_جدول_ثبت *ثبت = &arabic_state_جدول[state][this_type];
    إذا (ثبت->prev_action != NONE && prev != UINT_MAX)
    {
      info[prev].arabic_shaping_action() = ثبت->prev_action;
      بفر->خوندي_منزل_ورننباسه_tatweel (prev, بفر->len);
    }
    وإلا إذا (2 <= state && state <= 5) /* States that have a possible prev_action. */
    {
      بفر->unsafe_منزل_concat (prev, بفر->len);
    }
    اكسر;
  }
}

ساكن فراغ
mongolian_variation_selectors (hb_بفر_t *بفر)
{
  /* Copy arabic_shaping_action() from base to Mongolian variation selectors. */
  بینښه صحيح شمېر = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  لكل (بینښه صحيح i = 1; i < شمېر; i++)
    إذا (unlikely (hb_دننه_ranges<hb_codepoint_t> (info[i].codepoint, 0x180Bu, 0x180Du, 0x180Fu, 0x180Fu)))
      info[i].arabic_shaping_action() = info[i - 1].arabic_shaping_action();
}

فراغ
تیاری_masks_arabic_plan (ثابت arabic_shape_plan_t *arabic_plan,
			 hb_بفر_t               *بفر,
			 hb_script_t                script)
{
  HB_بفر_ALLOCATE_VAR (بفر, arabic_shaping_action);

  arabic_joining (بفر);
  إذا (script == HB_SCRIPT_MONGOLIAN)
    mongolian_variation_selectors (بفر);

  بینښه صحيح شمېر = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  لكل (بینښه صحيح i = 0; i < شمېر; i++)
    info[i].ماسک |= arabic_plan->ماسک_array[info[i].arabic_shaping_action()];
}

ساكن فراغ
تیاری_masks_arabic (ثابت hb_ot_shape_plan_t *plan,
		    hb_بفر_t              *بفر,
		    hb_font_t                *font HB_ناکارول_شوی)
{
  ثابت arabic_shape_plan_t *arabic_plan = (ثابت arabic_shape_plan_t *) plan->معلومات;
  تیاری_masks_arabic_plan (arabic_plan, بفر, plan->props.script);
}

ساكن منطقي
arabic_fallback_shape (ثابت hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_بفر_t *بفر)
{
#که_تعریف_شوی HB_NO_OT_SHAPER_ARABIC_FALLBACK
  أرجع خطأ;
#د_که_پای

  ثابت arabic_shape_plan_t *arabic_plan = (ثابت arabic_shape_plan_t *) plan->معلومات;

  إذا (!arabic_plan->do_fallback)
    أرجع خطأ;

retry:
  arabic_fallback_plan_t *fallback_plan = arabic_plan->fallback_plan;
  إذا (unlikely (!fallback_plan))
  {
    /* This sucks.  We need a font to build the fallback plan... */
    fallback_plan = arabic_fallback_plan_create (plan, font);
    إذا (unlikely (!arabic_plan->fallback_plan.cmpexch (تش_اشاره, fallback_plan)))
    {
      arabic_fallback_plan_destroy (fallback_plan);
      اذهب retry;
    }
  }

  arabic_fallback_plan_shape (fallback_plan, font, بفر);
  أرجع ریښتیا;
}

/*
 * Stretch feature: "stch".
 * See example here:
 * https://docs.microsoft.com/en-us/typography/script-development/syriac
 * We implement this in a generic way, such that the Arabic subtending
 * marks can use it as well.
 */

ساكن منطقي
ریکارډ_stch (ثابت hb_ot_shape_plan_t *plan,
	     hb_font_t *font HB_ناکارول_شوی,
	     hb_بفر_t *بفر)
{
  ثابت arabic_shape_plan_t *arabic_plan = (ثابت arabic_shape_plan_t *) plan->معلومات;
  إذا (!arabic_plan->has_stch)
    أرجع خطأ;

  /* 'stch' feature was just applied.  Look for anything that multiplied,
   * and record it for stch treatment later.  Note that rtlm, frac, etc
   * are applied before stch, but we assume that they didn't result in
   * anything multiplying into 5 pieces, so it's safe-ish... */

  بینښه صحيح شمېر = بفر->len;
  hb_glyph_info_t *info = بفر->info;
  لكل (بینښه صحيح i = 0; i < شمېر; i++)
    إذا (unlikely (_hb_glyph_info_multiplied (&info[i])))
    {
      بینښه صحيح comp = _hb_glyph_info_get_lig_comp (&info[i]);
      info[i].arabic_shaping_action() = comp % 2 ? STCH_REPEATING : STCH_FIXED;
      بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_ARABIC_HAS_STCH;
    }
  أرجع خطأ;
}

ساكن فراغ
apply_stch (ثابت hb_ot_shape_plan_t *plan HB_ناکارول_شوی,
	    hb_بفر_t              *بفر,
	    hb_font_t                *font)
{
  إذا (likely (!(بفر->scratch_flags & HB_بفر_SCRATCH_FLAG_ARABIC_HAS_STCH)))
    أرجع;

  منطقي rtl = بفر->props.direction == HB_DIRECTION_RTL;

  إذا (!rtl)
    بفر->reverse ();

  /* We do a two pass implementation:
   * First pass calculates the exact number of extra glyphs we need,
   * We then enlarge buffer to have that much room,
   * Second pass applies the stretch, copying things to the end of buffer.
   *
   * Cache glyph advances in the position scratch field during the first pass,
   * for re-use these during the second pass.
   */

  صحيح sign = font->x_scale < 0 ? -1 : +1;
  بینښه صحيح extra_glyphs_needed = 0; // Set during MEASURE, used during CUT
  تعداد { MEASURE, CUT } /* step_t */;

  لكل (بینښه صحيح ګام = MEASURE; ګام <= CUT; ګام = ګام + 1)
  {
    بینښه صحيح شمېر = بفر->len;
    hb_glyph_info_t *info = بفر->info;
    hb_glyph_position_t *pos = بفر->pos;
    بینښه صحيح new_len = شمېر + extra_glyphs_needed; // write head during CUT
    بینښه صحيح j = new_len;
    لكل (بینښه صحيح i = شمېر; i; i--)
    {
      إذا (!hb_دننه_range<uint8_t> (info[i - 1].arabic_shaping_action(), STCH_FIXED, STCH_REPEATING))
      {
	إذا (ګام == CUT)
	{
	  --j;
	  info[j] = info[i - 1];
	  pos[j] = pos[i - 1];
	}
	استمر;
      }

      /* Yay, justification! */

      hb_position_t w_ټول = 0; // Total to be filled
      hb_position_t w_fixed = 0; // Sum of fixed tiles
      hb_position_t w_repeating = 0; // Sum of repeating tiles
      صحيح n_fixed = 0;
      صحيح n_repeating = 0;

      بینښه صحيح پای_ټکی = i;
      طالما (i &&
	     hb_دننه_range<uint8_t> (info[i - 1].arabic_shaping_action(), STCH_FIXED, STCH_REPEATING))
      {
	i--;
        إذا (ګام == MEASURE)
          pos[i].stch_advance() = font->get_glyph_h_advance (info[i].codepoint);
        hb_position_t پلنوالی = pos[i].stch_advance();
	إذا (info[i].arabic_shaping_action() == STCH_FIXED)
	{
	  w_fixed = hb_saturate_جمع_کړه (w_fixed, پلنوالی);
	  n_fixed++;
	}
	وإلا
	{
	  w_repeating = hb_saturate_جمع_کړه (w_repeating, پلنوالی);
	  n_repeating++;
	}
      }
      بینښه صحيح پیل = i;
      بینښه صحيح context = i;
      طالما (context &&
	     !hb_دننه_range<uint8_t> (info[context - 1].arabic_shaping_action(), STCH_FIXED, STCH_REPEATING) &&
	     (_hb_glyph_info_is_default_ignorable (&info[context - 1]) ||
	      HB_ARABIC_GENERAL_CATEGORY_IS_کلمه (_hb_glyph_info_get_general_category (&info[context - 1]))))
      {
	context--;
	w_ټول = hb_saturate_جمع_کړه (w_ټول, pos[context].x_advance);
      }
      i++; // Don't touch i again.

      DEBUG_پیغام (ARABIC, تش_اشاره, "%s stretch at (%u,%u,%u)",
		 ګام == MEASURE ? "measuring" : "cutting", context, پیل, پای_ټکی);
      DEBUG_پیغام (ARABIC, تش_اشاره, "rest of word:    count=%u width %" PRId32, پیل - context, w_ټول);
      DEBUG_پیغام (ARABIC, تش_اشاره, "fixed tiles:     count=%d width=%" PRId32, n_fixed, w_fixed);
      DEBUG_پیغام (ARABIC, تش_اشاره, "repeating tiles: count=%d width=%" PRId32, n_repeating, w_repeating);

      ساكن constexpr بینښه STCH_MAX_GLYPHS = 256;

      /* Number of additional times to repeat each repeating tile. */
      بینښه صحيح n_copies = 0;

      int64_t w_پاتې_شوی_signed = (int64_t) w_ټول - w_fixed;
      int64_t w_repeating_signed = w_repeating;
      إذا (sign < 0)
      {
	w_پاتې_شوی_signed = -w_پاتې_شوی_signed;
	w_repeating_signed = -w_repeating_signed;
      }
      hb_position_t w_پاتې_شوی = hb_saturate_sub (w_ټول, w_fixed);
      إذا (w_پاتې_شوی_signed > w_repeating_signed && w_repeating_signed > 0)
	n_copies = w_پاتې_شوی_signed / w_repeating_signed - 1;

      /* See if we can improve the fit by adding an extra repeat and squeezing them together a bit. */
      hb_position_t extra_repeat_overlap = 0;
      int64_t shortfall = w_پاتې_شوی_signed - w_repeating_signed * (n_copies + 1);
      إذا (shortfall > 0 && n_repeating > 0)
      {
	++n_copies;
	int64_t excess = (n_copies + 1) * w_repeating_signed - w_پاتې_شوی_signed;
	إذا (excess > 0)
	{
	  extra_repeat_overlap = hb_clamp_منزل<hb_position_t> (excess / ((int64_t) n_copies * (int64_t) n_repeating));
	  w_پاتې_شوی = 0;
	}
      }

      بینښه صحيح max_copies = 0;
      إذا (n_repeating > 0)
      {
	بینښه صحيح بنسټ_glyphs = n_fixed + n_repeating;
	إذا (بنسټ_glyphs < STCH_MAX_GLYPHS)
	  max_copies = (STCH_MAX_GLYPHS - بنسټ_glyphs) / n_repeating;
      }
      n_copies = hb_min (n_copies, max_copies);

      إذا (ګام == MEASURE)
      {
	بینښه صحيح added_glyphs = 0;
	إذا (unlikely (hb_unsigned_mul_overflows (n_copies, n_repeating, &added_glyphs) ||
		      hb_unsigned_جمع_کړه_overflows (extra_glyphs_needed, added_glyphs, &extra_glyphs_needed)))
	{
	  extra_glyphs_needed = UINT_MAX;
	  اكسر;
	}
	DEBUG_پیغام (ARABIC, تش_اشاره, "will add extra %u copies of repeating tiles", n_copies);
      }
      وإلا
      {
	بفر->unsafe_منزل_break (context, پای_ټکی);
	hb_position_t x_دننه_واټن_2 = w_پاتې_شوی / 2;
	لكل (بینښه صحيح k = پای_ټکی; k > پیل; k--)
	{
          hb_position_t پلنوالی = pos[k - 1].stch_advance();

	  بینښه صحيح repeat = 1;
	  إذا (info[k - 1].arabic_shaping_action() == STCH_REPEATING)
	    repeat += n_copies;

	  DEBUG_پیغام (ARABIC, تش_اشاره, "appending %u copies of glyph %" PRIu32 "; j=%u",
		     repeat, info[k - 1].codepoint, j);
	  pos[k - 1].x_advance = 0;
	  لكل (بینښه صحيح n = 0; n < repeat; n++)
	  {
	    إذا (rtl)
	    {
	      x_دننه_واټن_2 = hb_saturate_sub (x_دننه_واټن_2, پلنوالی);
	      إذا (n > 0)
		x_دننه_واټن_2 = hb_saturate_جمع_کړه (x_دننه_واټن_2, extra_repeat_overlap);
	    }
	    pos[k - 1].x_دننه_واټن_2 = x_دننه_واټن_2;
	    /* Append copy. */
	    --j;
	    info[j] = info[k - 1];
	    pos[j] = pos[k - 1];

	    إذا (!rtl)
	    {
	      x_دننه_واټن_2 = hb_saturate_جمع_کړه (x_دننه_واټن_2, پلنوالی);
	      إذا (n > 0)
		x_دننه_واټن_2 = hb_saturate_sub (x_دننه_واټن_2, extra_repeat_overlap);
	    }
	  }
	}
      }
    }

    إذا (ګام == MEASURE)
    {
      بینښه صحيح ټول_glyphs = 0;
      إذا (unlikely (hb_unsigned_جمع_کړه_overflows (شمېر, extra_glyphs_needed, &ټول_glyphs) ||
		    !بفر->ensure (ټول_glyphs)))
	اكسر;
    }
    وإلا
    {
      أكد (j == 0);
      بفر->len = new_len;
    }
  }

  إذا (!rtl)
    بفر->reverse ();
}


ساكن فراغ
postprocess_glyphs_arabic (ثابت hb_ot_shape_plan_t *plan,
			   hb_بفر_t              *بفر,
			   hb_font_t                *font)
{
  apply_stch (plan, بفر, font);
}

/* https://www.unicode.org/reports/tr53/ */

ساكن ثابت hb_codepoint_t
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

ساكن مضمن منطقي
info_is_mcm (ثابت hb_glyph_info_t &info)
{
  hb_codepoint_t u = info.codepoint;
  لكل (بینښه صحيح i = 0; i < ARRAY_اوږدتیا (modifier_combining_marks); i++)
    إذا (u == modifier_combining_marks[i])
      أرجع ریښتیا;
  أرجع خطأ;
}

ساكن فراغ
reorder_marks_arabic (ثابت hb_ot_shape_plan_t *plan HB_ناکارول_شوی,
		      hb_بفر_t              *بفر,
		      بینښه صحيح              پیل,
		      بینښه صحيح              پای_ټکی)
{
  hb_glyph_info_t *info = بفر->info;

  DEBUG_پیغام (ARABIC, بفر, "Reordering marks from %u to %u", پیل, پای_ټکی);

  بینښه صحيح i = پیل;
  لكل (بینښه صحيح cc = 220; cc <= 230; cc += 10)
  {
    DEBUG_پیغام (ARABIC, بفر, "Looking for %u's starting at %u", cc, i);
    طالما (i < پای_ټکی && info_cc(info[i]) < cc)
      i++;
    DEBUG_پیغام (ARABIC, بفر, "Looking for %u's stopped at %u", cc, i);

    إذا (i == پای_ټکی)
      اكسر;

    إذا (info_cc(info[i]) > cc)
      استمر;

    بینښه صحيح j = i;
    طالما (j < پای_ټکی && info_cc(info[j]) == cc && info_is_mcm (info[j]))
      j++;

    إذا (i == j)
      استمر;

    DEBUG_پیغام (ARABIC, بفر, "Found %u's from %u to %u", cc, i, j);

    /* Shift it! */
    DEBUG_پیغام (ARABIC, بفر, "Shifting %u's: %u %u", cc, i, j);
    hb_glyph_info_t لنډمهاله[HB_OT_SHAPE_MAX_COMBINING_MARKS];
    أكد (j - i <= ARRAY_اوږدتیا (لنډمهاله));
    بفر->یوځای_clusters (پیل, j);
    memmove (لنډمهاله, &info[i], (j - i) * حجم (hb_glyph_info_t));
    memmove (&info[پیل + j - i], &info[پیل], (i - پیل) * حجم (hb_glyph_info_t));
    memmove (&info[پیل], لنډمهاله, (j - i) * حجم (hb_glyph_info_t));

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
    بینښه صحيح new_پیل = پیل + j - i;
    بینښه صحيح new_cc = cc == 220 ? HB_MODIFIED_COMBINING_CLASS_CCC22 : HB_MODIFIED_COMBINING_CLASS_CCC26;
    طالما (پیل < new_پیل)
    {
      hb_glyph_info_سېټ_modified_combining_class (&info[پیل], new_cc);
      پیل++;
    }

    i = j;
  }
}

ثابت hb_ot_shaper_t _hb_ot_shaper_arabic =
{
  collect_features_arabic,
  تش_اشاره, /* override_features */
  معلومات_create_arabic,
  معلومات_destroy_arabic,
  تش_اشاره, /* preprocess_text */
  postprocess_glyphs_arabic,
  تش_اشاره, /* decompose */
  تش_اشاره, /* compose */
  تیاری_masks_arabic,
  reorder_marks_arabic,
  HB_نښان_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_ډېرپېښ_DEFAULT,
  HB_OT_SHAPE_ZERO_پلنوالی_MARKS_BY_GDEF_LATE,
  ریښتیا, /* fallback_position */
};


#د_که_پای

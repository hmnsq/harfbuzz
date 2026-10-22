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

#اگر_تعریف_نشده HB_NO_OT_SHAPE

#ضمن "hb-ot-shaper-arabic.hh"
#ضمن "hb-ot-shape.hh"


/* buffer var allocations */
#تعریف_کن arabic_shaping_action() ot_shaper_var_u8_auxiliary() /* arabic shaping action */

/* buffer **position** var allocations, used after GPOS */
#تعریف_کن stch_advance() var.i32 /* advance used for stretching */

#تعریف_کن HB_میانگیر_SCRATCH_FLAG_ARABIC_HAS_STCH HB_میانگیر_SCRATCH_FLAG_SHAPER0

/* See:
 * https://github.com/harfbuzz/harfbuzz/commit/6e6f82b6f3dde0fc6c3c7d991d9ec6cfff57823d#commitcomment-14248516 */
#تعریف_کن HB_ARABIC_GENERAL_CATEGORY_IS_کلمه(gen_cat) \
	(FLAG_UNSAFE (gen_cat) & \
	 (FLAG (HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_خصوصی_USE) | \
	  /*FLAG (HB_UNICODE_GENERAL_CATEGORY_LOWERCASE_LETTER) |*/ \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_MODIFIER_LETTER) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER) | \
	  /*FLAG (HB_UNICODE_GENERAL_CATEGORY_TITLECASE_LETTER) |*/ \
	  /*FLAG (HB_UNICODE_GENERAL_CATEGORY_UPPERCASE_LETTER) |*/ \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_SPACING_MARK) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_ENCLOSING_MARK) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_DECIMAL_عدد) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_LETTER_عدد) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_OTHER_عدد) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_CURRENCY_نماد) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_MODIFIER_نماد) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_MATH_نماد) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_OTHER_نماد)))


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
  JOINING_گروه_ALAPH		= 4,
  JOINING_گروه_DALATH_RISH	= 5,
  رقم_STATE_MACHINE_تعداد_ستون	= 6,

  JOINING_TYPE_T = 6,
  JOINING_TYPE_X = 7  /* means: use general-category to choose between U or T. */
};

#ضمن "hb-ot-shaper-arabic-table.hh"

ساكن بیعلامت صحيح get_joining_type (hb_codepoint_t u, hb_unicode_general_category_t gen_cat)
{
  بیعلامت صحيح j_type = joining_type(u);
  إذا (likely (j_type != JOINING_TYPE_X))
    أرجع j_type;

  أرجع (FLAG_UNSAFE(gen_cat) &
	  (FLAG(HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK) |
	   FLAG(HB_UNICODE_GENERAL_CATEGORY_ENCLOSING_MARK) |
	   FLAG(HB_UNICODE_GENERAL_CATEGORY_FORMAT))
	 ) ?  JOINING_TYPE_T : JOINING_TYPE_U;
}

#تعریف_کن FEATURE_IS_SYRIAC(برچسب_2) hb_درون_range<بیعلامت حرف> ((بیعلامت حرف) (برچسب_2), '2', '3')

ساكن ثابت hb_برچسب_t arabic_features[] =
{
  HB_برچسب('i','s','o','l'),
  HB_برچسب('f','i','n','a'),
  HB_برچسب('f','i','n','2'),
  HB_برچسب('f','i','n','3'),
  HB_برچسب('m','e','d','i'),
  HB_برچسب('m','e','d','2'),
  HB_برچسب('i','n','i','t'),
  HB_برچسب_NONE
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

  ARABIC_رقم_FEATURES = NONE,

  /* We abuse the same byte for other things... */
  STCH_FIXED,
  STCH_REPEATING,
};

ساكن ثابت بنية arabic_state_جدول_مدخل {
	uint8_t prev_action;
	uint8_t curr_action;
	uint16_t بعدی_state;
} arabic_state_جدول[][رقم_STATE_MACHINE_تعداد_ستون] =
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
		       hb_میانگیر_t *میانگیر);

ساكن منطقي
رکورد_stch (ثابت hb_ot_shape_plan_t *plan,
	     hb_font_t *font,
	     hb_میانگیر_t *میانگیر);

ساكن منطقي
deallocate_میانگیر_var (ثابت hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_میانگیر_t *میانگیر)
{
  HB_میانگیر_DEALLOCATE_VAR (میانگیر, arabic_shaping_action);
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


  map->enable_feature (HB_برچسب('s','t','c','h'));
  map->جمع_کن_gsub_pause (رکورد_stch);

  map->enable_feature (HB_برچسب('c','c','m','p'), F_MANUAL_ZWJ);
  map->enable_feature (HB_برچسب('l','o','c','l'), F_MANUAL_ZWJ);

  map->جمع_کن_gsub_pause (اشاره_تهی);

  لكل (بیعلامت صحيح i = 0; i < ARABIC_رقم_FEATURES; i++)
  {
    منطقي has_fallback = plan->props.script == HB_SCRIPT_ARABIC && !FEATURE_IS_SYRIAC (arabic_features[i]);
    map->جمع_کن_feature (arabic_features[i], F_MANUAL_ZWJ | (has_fallback ? F_HAS_FALLBACK : F_NONE));
    map->جمع_کن_gsub_pause (اشاره_تهی);
  }
   map->جمع_کن_gsub_pause (deallocate_میانگیر_var);

  /* Normally, Unicode says a ZWNJ means "don't ligate".  In Arabic script
   * however, it says a ZWJ should also mean "don't ligate".  So we run
   * the main ligating features as MANUAL_ZWJ. */

  map->enable_feature (HB_برچسب('r','l','i','g'), F_MANUAL_ZWJ | F_HAS_FALLBACK);

  إذا (plan->props.script == HB_SCRIPT_ARABIC)
    map->جمع_کن_gsub_pause (arabic_fallback_shape);

   map->enable_feature (HB_برچسب('c','a','l','t'), F_MANUAL_ZWJ);
   /* https://github.com/harfbuzz/harfbuzz/issues/1573 */
   إذا (!map->has_feature (HB_برچسب('r','c','l','t')))
     map->جمع_کن_gsub_pause (اشاره_تهی);

   map->enable_feature (HB_برچسب('l','i','g','a'), F_MANUAL_ZWJ);
   map->enable_feature (HB_برچسب('c','l','i','g'), F_MANUAL_ZWJ);

  /* The spec includes 'cswh'.  Earlier versions of Windows
   * used to enable this by default, but testing suggests
   * that Windows 8 and later do not enable it by default,
   * and spec now says 'Off by default'.
   * We disabled this in ae23c24c32.
   * Note that IranNastaliq uses this feature extensively
   * to fixup broken glyph sequences.  Oh well...
   * Test case: U+0643,U+0640,U+0631. */
  //map->enable_feature (HB_TAG('c','s','w','h'), F_MANUAL_ZWJ);
  map->enable_feature (HB_برچسب('m','s','e','t'), F_MANUAL_ZWJ);
}

#ضمن "hb-ot-shaper-arabic-fallback.hh"

بنية arabic_shape_plan_t
{
  /* The "+ 1" in the next array is to accommodate for the "NONE" command,
   * which is not an OpenType feature, but this simplifies the code by not
   * having to do a "if (... < NONE) ..." and just rely on the fact that
   * mask_array[NONE] == 0. */
  hb_ماسک_t ماسک_array[ARABIC_رقم_FEATURES + 1];

  تغییرپذیر hb_atomic_t<arabic_fallback_plan_t *> fallback_plan;

  بیعلامت صحيح do_fallback : 1;
  بیعلامت صحيح has_stch : 1;
};

فراغ *
داده_create_arabic (ثابت hb_ot_shape_plan_t *plan)
{
  arabic_shape_plan_t *arabic_plan = (arabic_shape_plan_t *) hb_calloc (1, حجم (arabic_shape_plan_t));
  إذا (unlikely (!arabic_plan))
    أرجع اشاره_تهی;

  arabic_plan->do_fallback = plan->props.script == HB_SCRIPT_ARABIC;
  arabic_plan->has_stch = !!plan->map.get_1_ماسک (HB_برچسب ('s','t','c','h'));
  لكل (بیعلامت صحيح i = 0; i < ARABIC_رقم_FEATURES; i++) {
    arabic_plan->ماسک_array[i] = plan->map.get_1_ماسک (arabic_features[i]);
    arabic_plan->do_fallback = arabic_plan->do_fallback &&
			       (FEATURE_IS_SYRIAC (arabic_features[i]) ||
				plan->map.needs_fallback (arabic_features[i]));
  }

  أرجع arabic_plan;
}

فراغ
داده_destroy_arabic (فراغ *داده)
{
  arabic_shape_plan_t *arabic_plan = (arabic_shape_plan_t *) داده;

  arabic_fallback_plan_destroy (arabic_plan->fallback_plan);

  hb_free (داده);
}

ساكن فراغ
arabic_joining (hb_میانگیر_t *میانگیر)
{
  بیعلامت صحيح شمار = میانگیر->len;
  hb_glyph_info_t *info = میانگیر->info;
  بیعلامت صحيح prev = UINT_MAX, state = 0;

  /* Check pre-context */
  لكل (بیعلامت صحيح i = 0; i < میانگیر->context_len[0]; i++)
  {
    بیعلامت صحيح this_type = get_joining_type (میانگیر->context[0][i], میانگیر->unicode->general_category (میانگیر->context[0][i]));

    إذا (unlikely (this_type == JOINING_TYPE_T))
      استمر;

    ثابت arabic_state_جدول_مدخل *مدخل = &arabic_state_جدول[state][this_type];
    state = مدخل->بعدی_state;
    اكسر;
  }

  لكل (بیعلامت صحيح i = 0; i < شمار; i++)
  {
    بیعلامت صحيح this_type = get_joining_type (info[i].codepoint, _hb_glyph_info_get_general_category (&info[i]));

    إذا (unlikely (this_type == JOINING_TYPE_T)) {
      info[i].arabic_shaping_action() = NONE;
      استمر;
    }

    ثابت arabic_state_جدول_مدخل *مدخل = &arabic_state_جدول[state][this_type];

    إذا (مدخل->prev_action != NONE && prev != UINT_MAX)
    {
      info[prev].arabic_shaping_action() = مدخل->prev_action;
      میانگیر->امن_مقصد_درج_tatweel (prev, i + 1);
    }
    وإلا
    {
      إذا (prev == UINT_MAX)
      {
        إذا (this_type >= JOINING_TYPE_R)
	  میانگیر->unsafe_مقصد_concat_from_outbuffer (0, i + 1);
      }
      وإلا
      {
	إذا (this_type >= JOINING_TYPE_R ||
	    (2 <= state && state <= 5) /* States that have a possible prev_action. */)
	  میانگیر->unsafe_مقصد_concat (prev, i + 1);
      }
    }

    info[i].arabic_shaping_action() = مدخل->curr_action;

    prev = i;
    state = مدخل->بعدی_state;
  }

  لكل (بیعلامت صحيح i = 0; i < میانگیر->context_len[1]; i++)
  {
    بیعلامت صحيح this_type = get_joining_type (میانگیر->context[1][i], میانگیر->unicode->general_category (میانگیر->context[1][i]));

    إذا (unlikely (this_type == JOINING_TYPE_T))
      استمر;

    ثابت arabic_state_جدول_مدخل *مدخل = &arabic_state_جدول[state][this_type];
    إذا (مدخل->prev_action != NONE && prev != UINT_MAX)
    {
      info[prev].arabic_shaping_action() = مدخل->prev_action;
      میانگیر->امن_مقصد_درج_tatweel (prev, میانگیر->len);
    }
    وإلا إذا (2 <= state && state <= 5) /* States that have a possible prev_action. */
    {
      میانگیر->unsafe_مقصد_concat (prev, میانگیر->len);
    }
    اكسر;
  }
}

ساكن فراغ
mongolian_variation_selectors (hb_میانگیر_t *میانگیر)
{
  /* Copy arabic_shaping_action() from base to Mongolian variation selectors. */
  بیعلامت صحيح شمار = میانگیر->len;
  hb_glyph_info_t *info = میانگیر->info;
  لكل (بیعلامت صحيح i = 1; i < شمار; i++)
    إذا (unlikely (hb_درون_ranges<hb_codepoint_t> (info[i].codepoint, 0x180Bu, 0x180Du, 0x180Fu, 0x180Fu)))
      info[i].arabic_shaping_action() = info[i - 1].arabic_shaping_action();
}

فراغ
راهاندازی_masks_arabic_plan (ثابت arabic_shape_plan_t *arabic_plan,
			 hb_میانگیر_t               *میانگیر,
			 hb_script_t                script)
{
  HB_میانگیر_ALLOCATE_VAR (میانگیر, arabic_shaping_action);

  arabic_joining (میانگیر);
  إذا (script == HB_SCRIPT_MONGOLIAN)
    mongolian_variation_selectors (میانگیر);

  بیعلامت صحيح شمار = میانگیر->len;
  hb_glyph_info_t *info = میانگیر->info;
  لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    info[i].ماسک |= arabic_plan->ماسک_array[info[i].arabic_shaping_action()];
}

ساكن فراغ
راهاندازی_masks_arabic (ثابت hb_ot_shape_plan_t *plan,
		    hb_میانگیر_t              *میانگیر,
		    hb_font_t                *font HB_استفادهنشده)
{
  ثابت arabic_shape_plan_t *arabic_plan = (ثابت arabic_shape_plan_t *) plan->داده;
  راهاندازی_masks_arabic_plan (arabic_plan, میانگیر, plan->props.script);
}

ساكن منطقي
arabic_fallback_shape (ثابت hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_میانگیر_t *میانگیر)
{
#اگر_تعریف_شده HB_NO_OT_SHAPER_ARABIC_FALLBACK
  أرجع خطأ;
#پایان_اگر

  ثابت arabic_shape_plan_t *arabic_plan = (ثابت arabic_shape_plan_t *) plan->داده;

  إذا (!arabic_plan->do_fallback)
    أرجع خطأ;

retry:
  arabic_fallback_plan_t *fallback_plan = arabic_plan->fallback_plan;
  إذا (unlikely (!fallback_plan))
  {
    /* This sucks.  We need a font to build the fallback plan... */
    fallback_plan = arabic_fallback_plan_create (plan, font);
    إذا (unlikely (!arabic_plan->fallback_plan.cmpexch (اشاره_تهی, fallback_plan)))
    {
      arabic_fallback_plan_destroy (fallback_plan);
      اذهب retry;
    }
  }

  arabic_fallback_plan_shape (fallback_plan, font, میانگیر);
  أرجع درست;
}

/*
 * Stretch feature: "stch".
 * See example here:
 * https://docs.microsoft.com/en-us/typography/script-development/syriac
 * We implement this in a generic way, such that the Arabic subtending
 * marks can use it as well.
 */

ساكن منطقي
رکورد_stch (ثابت hb_ot_shape_plan_t *plan,
	     hb_font_t *font HB_استفادهنشده,
	     hb_میانگیر_t *میانگیر)
{
  ثابت arabic_shape_plan_t *arabic_plan = (ثابت arabic_shape_plan_t *) plan->داده;
  إذا (!arabic_plan->has_stch)
    أرجع خطأ;

  /* 'stch' feature was just applied.  Look for anything that multiplied,
   * and record it for stch treatment later.  Note that rtlm, frac, etc
   * are applied before stch, but we assume that they didn't result in
   * anything multiplying into 5 pieces, so it's safe-ish... */

  بیعلامت صحيح شمار = میانگیر->len;
  hb_glyph_info_t *info = میانگیر->info;
  لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    إذا (unlikely (_hb_glyph_info_multiplied (&info[i])))
    {
      بیعلامت صحيح comp = _hb_glyph_info_get_lig_comp (&info[i]);
      info[i].arabic_shaping_action() = comp % 2 ? STCH_REPEATING : STCH_FIXED;
      میانگیر->scratch_flags |= HB_میانگیر_SCRATCH_FLAG_ARABIC_HAS_STCH;
    }
  أرجع خطأ;
}

ساكن فراغ
apply_stch (ثابت hb_ot_shape_plan_t *plan HB_استفادهنشده,
	    hb_میانگیر_t              *میانگیر,
	    hb_font_t                *font)
{
  إذا (likely (!(میانگیر->scratch_flags & HB_میانگیر_SCRATCH_FLAG_ARABIC_HAS_STCH)))
    أرجع;

  منطقي rtl = میانگیر->props.direction == HB_DIRECTION_RTL;

  إذا (!rtl)
    میانگیر->reverse ();

  /* We do a two pass implementation:
   * First pass calculates the exact number of extra glyphs we need,
   * We then enlarge buffer to have that much room,
   * Second pass applies the stretch, copying things to the end of buffer.
   *
   * Cache glyph advances in the position scratch field during the first pass,
   * for re-use these during the second pass.
   */

  صحيح sign = font->x_scale < 0 ? -1 : +1;
  بیعلامت صحيح extra_glyphs_needed = 0; // Set during MEASURE, used during CUT
  تعداد { MEASURE, CUT } /* step_t */;

  لكل (بیعلامت صحيح گام = MEASURE; گام <= CUT; گام = گام + 1)
  {
    بیعلامت صحيح شمار = میانگیر->len;
    hb_glyph_info_t *info = میانگیر->info;
    hb_glyph_position_t *pos = میانگیر->pos;
    بیعلامت صحيح new_len = شمار + extra_glyphs_needed; // write head during CUT
    بیعلامت صحيح j = new_len;
    لكل (بیعلامت صحيح i = شمار; i; i--)
    {
      إذا (!hb_درون_range<uint8_t> (info[i - 1].arabic_shaping_action(), STCH_FIXED, STCH_REPEATING))
      {
	إذا (گام == CUT)
	{
	  --j;
	  info[j] = info[i - 1];
	  pos[j] = pos[i - 1];
	}
	استمر;
      }

      /* Yay, justification! */

      hb_position_t w_مجموع = 0; // Total to be filled
      hb_position_t w_fixed = 0; // Sum of fixed tiles
      hb_position_t w_repeating = 0; // Sum of repeating tiles
      صحيح n_fixed = 0;
      صحيح n_repeating = 0;

      بیعلامت صحيح انتها = i;
      طالما (i &&
	     hb_درون_range<uint8_t> (info[i - 1].arabic_shaping_action(), STCH_FIXED, STCH_REPEATING))
      {
	i--;
        إذا (گام == MEASURE)
          pos[i].stch_advance() = font->get_glyph_h_advance (info[i].codepoint);
        hb_position_t عرض = pos[i].stch_advance();
	إذا (info[i].arabic_shaping_action() == STCH_FIXED)
	{
	  w_fixed = hb_saturate_جمع_کن (w_fixed, عرض);
	  n_fixed++;
	}
	وإلا
	{
	  w_repeating = hb_saturate_جمع_کن (w_repeating, عرض);
	  n_repeating++;
	}
      }
      بیعلامت صحيح شروع = i;
      بیعلامت صحيح context = i;
      طالما (context &&
	     !hb_درون_range<uint8_t> (info[context - 1].arabic_shaping_action(), STCH_FIXED, STCH_REPEATING) &&
	     (_hb_glyph_info_is_default_ignorable (&info[context - 1]) ||
	      HB_ARABIC_GENERAL_CATEGORY_IS_کلمه (_hb_glyph_info_get_general_category (&info[context - 1]))))
      {
	context--;
	w_مجموع = hb_saturate_جمع_کن (w_مجموع, pos[context].x_advance);
      }
      i++; // Don't touch i again.

      DEBUG_پیام (ARABIC, اشاره_تهی, "%s stretch at (%u,%u,%u)",
		 گام == MEASURE ? "measuring" : "cutting", context, شروع, انتها);
      DEBUG_پیام (ARABIC, اشاره_تهی, "rest of word:    count=%u width %" PRId32, شروع - context, w_مجموع);
      DEBUG_پیام (ARABIC, اشاره_تهی, "fixed tiles:     count=%d width=%" PRId32, n_fixed, w_fixed);
      DEBUG_پیام (ARABIC, اشاره_تهی, "repeating tiles: count=%d width=%" PRId32, n_repeating, w_repeating);

      ساكن constexpr بیعلامت STCH_MAX_GLYPHS = 256;

      /* Number of additional times to repeat each repeating tile. */
      بیعلامت صحيح n_copies = 0;

      int64_t w_مانده_signed = (int64_t) w_مجموع - w_fixed;
      int64_t w_repeating_signed = w_repeating;
      إذا (sign < 0)
      {
	w_مانده_signed = -w_مانده_signed;
	w_repeating_signed = -w_repeating_signed;
      }
      hb_position_t w_مانده = hb_saturate_sub (w_مجموع, w_fixed);
      إذا (w_مانده_signed > w_repeating_signed && w_repeating_signed > 0)
	n_copies = w_مانده_signed / w_repeating_signed - 1;

      /* See if we can improve the fit by adding an extra repeat and squeezing them together a bit. */
      hb_position_t extra_repeat_overlap = 0;
      int64_t shortfall = w_مانده_signed - w_repeating_signed * (n_copies + 1);
      إذا (shortfall > 0 && n_repeating > 0)
      {
	++n_copies;
	int64_t excess = (n_copies + 1) * w_repeating_signed - w_مانده_signed;
	إذا (excess > 0)
	{
	  extra_repeat_overlap = hb_clamp_مقصد<hb_position_t> (excess / ((int64_t) n_copies * (int64_t) n_repeating));
	  w_مانده = 0;
	}
      }

      بیعلامت صحيح max_copies = 0;
      إذا (n_repeating > 0)
      {
	بیعلامت صحيح پایه_glyphs = n_fixed + n_repeating;
	إذا (پایه_glyphs < STCH_MAX_GLYPHS)
	  max_copies = (STCH_MAX_GLYPHS - پایه_glyphs) / n_repeating;
      }
      n_copies = hb_min (n_copies, max_copies);

      إذا (گام == MEASURE)
      {
	بیعلامت صحيح added_glyphs = 0;
	إذا (unlikely (hb_unsigned_mul_overflows (n_copies, n_repeating, &added_glyphs) ||
		      hb_unsigned_جمع_کن_overflows (extra_glyphs_needed, added_glyphs, &extra_glyphs_needed)))
	{
	  extra_glyphs_needed = UINT_MAX;
	  اكسر;
	}
	DEBUG_پیام (ARABIC, اشاره_تهی, "will add extra %u copies of repeating tiles", n_copies);
      }
      وإلا
      {
	میانگیر->unsafe_مقصد_break (context, انتها);
	hb_position_t x_جابه_جایی_2 = w_مانده / 2;
	لكل (بیعلامت صحيح k = انتها; k > شروع; k--)
	{
          hb_position_t عرض = pos[k - 1].stch_advance();

	  بیعلامت صحيح repeat = 1;
	  إذا (info[k - 1].arabic_shaping_action() == STCH_REPEATING)
	    repeat += n_copies;

	  DEBUG_پیام (ARABIC, اشاره_تهی, "appending %u copies of glyph %" PRIu32 "; j=%u",
		     repeat, info[k - 1].codepoint, j);
	  pos[k - 1].x_advance = 0;
	  لكل (بیعلامت صحيح n = 0; n < repeat; n++)
	  {
	    إذا (rtl)
	    {
	      x_جابه_جایی_2 = hb_saturate_sub (x_جابه_جایی_2, عرض);
	      إذا (n > 0)
		x_جابه_جایی_2 = hb_saturate_جمع_کن (x_جابه_جایی_2, extra_repeat_overlap);
	    }
	    pos[k - 1].x_جابه_جایی_2 = x_جابه_جایی_2;
	    /* Append copy. */
	    --j;
	    info[j] = info[k - 1];
	    pos[j] = pos[k - 1];

	    إذا (!rtl)
	    {
	      x_جابه_جایی_2 = hb_saturate_جمع_کن (x_جابه_جایی_2, عرض);
	      إذا (n > 0)
		x_جابه_جایی_2 = hb_saturate_sub (x_جابه_جایی_2, extra_repeat_overlap);
	    }
	  }
	}
      }
    }

    إذا (گام == MEASURE)
    {
      بیعلامت صحيح مجموع_glyphs = 0;
      إذا (unlikely (hb_unsigned_جمع_کن_overflows (شمار, extra_glyphs_needed, &مجموع_glyphs) ||
		    !میانگیر->ensure (مجموع_glyphs)))
	اكسر;
    }
    وإلا
    {
      أكد (j == 0);
      میانگیر->len = new_len;
    }
  }

  إذا (!rtl)
    میانگیر->reverse ();
}


ساكن فراغ
postprocess_glyphs_arabic (ثابت hb_ot_shape_plan_t *plan,
			   hb_میانگیر_t              *میانگیر,
			   hb_font_t                *font)
{
  apply_stch (plan, میانگیر, font);
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
  لكل (بیعلامت صحيح i = 0; i < ARRAY_درازی (modifier_combining_marks); i++)
    إذا (u == modifier_combining_marks[i])
      أرجع درست;
  أرجع خطأ;
}

ساكن فراغ
reorder_marks_arabic (ثابت hb_ot_shape_plan_t *plan HB_استفادهنشده,
		      hb_میانگیر_t              *میانگیر,
		      بیعلامت صحيح              شروع,
		      بیعلامت صحيح              انتها)
{
  hb_glyph_info_t *info = میانگیر->info;

  DEBUG_پیام (ARABIC, میانگیر, "Reordering marks from %u to %u", شروع, انتها);

  بیعلامت صحيح i = شروع;
  لكل (بیعلامت صحيح cc = 220; cc <= 230; cc += 10)
  {
    DEBUG_پیام (ARABIC, میانگیر, "Looking for %u's starting at %u", cc, i);
    طالما (i < انتها && info_cc(info[i]) < cc)
      i++;
    DEBUG_پیام (ARABIC, میانگیر, "Looking for %u's stopped at %u", cc, i);

    إذا (i == انتها)
      اكسر;

    إذا (info_cc(info[i]) > cc)
      استمر;

    بیعلامت صحيح j = i;
    طالما (j < انتها && info_cc(info[j]) == cc && info_is_mcm (info[j]))
      j++;

    إذا (i == j)
      استمر;

    DEBUG_پیام (ARABIC, میانگیر, "Found %u's from %u to %u", cc, i, j);

    /* Shift it! */
    DEBUG_پیام (ARABIC, میانگیر, "Shifting %u's: %u %u", cc, i, j);
    hb_glyph_info_t موقت[HB_OT_SHAPE_MAX_COMBINING_MARKS];
    أكد (j - i <= ARRAY_درازی (موقت));
    میانگیر->ادغام_clusters (شروع, j);
    memmove (موقت, &info[i], (j - i) * حجم (hb_glyph_info_t));
    memmove (&info[شروع + j - i], &info[شروع], (i - شروع) * حجم (hb_glyph_info_t));
    memmove (&info[شروع], موقت, (j - i) * حجم (hb_glyph_info_t));

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
    بیعلامت صحيح new_شروع = شروع + j - i;
    بیعلامت صحيح new_cc = cc == 220 ? HB_MODIFIED_COMBINING_CLASS_CCC22 : HB_MODIFIED_COMBINING_CLASS_CCC26;
    طالما (شروع < new_شروع)
    {
      hb_glyph_info_مجموعه_modified_combining_class (&info[شروع], new_cc);
      شروع++;
    }

    i = j;
  }
}

ثابت hb_ot_shaper_t _hb_ot_shaper_arabic =
{
  collect_features_arabic,
  اشاره_تهی, /* override_features */
  داده_create_arabic,
  داده_destroy_arabic,
  اشاره_تهی, /* preprocess_text */
  postprocess_glyphs_arabic,
  اشاره_تهی, /* decompose */
  اشاره_تهی, /* compose */
  راهاندازی_masks_arabic,
  reorder_marks_arabic,
  HB_برچسب_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_نما_DEFAULT,
  HB_OT_SHAPE_ZERO_عرض_MARKS_BY_GDEF_LATE,
  درست, /* fallback_position */
};


#پایان_اگر

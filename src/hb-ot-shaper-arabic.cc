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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPE

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shaper-arabic.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shape.hh"


/* buffer var allocations */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arabic_shaping_action() ot_shaper_var_u8_auxiliary() /* arabic shaping action */

/* buffer **position** var allocations, used after GPOS */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ stch_advance() var.i32 /* advance used for stretching */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਬਫਰ_SCRATCH_FLAG_ARABIC_HAS_STCH HB_ਬਫਰ_SCRATCH_FLAG_SHAPER0

/* See:
 * https://github.com/harfbuzz/harfbuzz/commit/6e6f82b6f3dde0fc6c3c7d991d9ec6cfff57823d#commitcomment-14248516 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ARABIC_GENERAL_CATEGORY_IS_ਸ਼ਬਦ(gen_cat) \
	(FLAG_UNSAFE (gen_cat) & \
	 (FLAG (HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_ਨਿੱਜੀ_USE) | \
	  /*FLAG (HB_UNICODE_GENERAL_CATEGORY_LOWERCASE_LETTER) |*/ \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_MODIFIER_LETTER) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER) | \
	  /*FLAG (HB_UNICODE_GENERAL_CATEGORY_TITLECASE_LETTER) |*/ \
	  /*FLAG (HB_UNICODE_GENERAL_CATEGORY_UPPERCASE_LETTER) |*/ \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_SPACING_MARK) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_ENCLOSING_MARK) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_DECIMAL_ਸੰਖਿਆ) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_LETTER_ਸੰਖਿਆ) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_OTHER_ਸੰਖਿਆ) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_CURRENCY_ਚਿੰਨ੍ਹ) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_MODIFIER_ਚਿੰਨ੍ਹ) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_MATH_ਚਿੰਨ੍ਹ) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_OTHER_ਚਿੰਨ੍ਹ)))


/*
 * Joining types:
 */

/*
 * Bits used in the joining tables
 */
ਗਿਣਤੀ hb_arabic_joining_type_t {
  JOINING_TYPE_U		= 0,
  JOINING_TYPE_L		= 1,
  JOINING_TYPE_R		= 2,
  JOINING_TYPE_D		= 3,
  JOINING_TYPE_C		= JOINING_TYPE_D,
  JOINING_ਸਮੂਹ_ALAPH		= 4,
  JOINING_ਸਮੂਹ_DALATH_RISH	= 5,
  ਅੰਕ_STATE_MACHINE_ਖਾਨੇ_ਗਿਣਤੀ	= 6,

  JOINING_TYPE_T = 6,
  JOINING_TYPE_X = 7  /* means: use general-category to choose between U or T. */
};

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shaper-arabic-table.hh"

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_joining_type (hb_codepoint_t u, hb_unicode_general_category_t gen_cat)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j_type = joining_type(u);
  ਜੇ (likely (j_type != JOINING_TYPE_X))
    ਵਾਪਸ j_type;

  ਵਾਪਸ (FLAG_UNSAFE(gen_cat) &
	  (FLAG(HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK) |
	   FLAG(HB_UNICODE_GENERAL_CATEGORY_ENCLOSING_MARK) |
	   FLAG(HB_UNICODE_GENERAL_CATEGORY_FORMAT))
	 ) ?  JOINING_TYPE_T : JOINING_TYPE_U;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FEATURE_IS_SYRIAC(ਟੈਗ_2) hb_ਵਿੱਚ_range<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ> ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ) (ਟੈਗ_2), '2', '3')

ਸਥਿਰ ਅਚਲ hb_ਟੈਗ_t arabic_features[] =
{
  HB_ਟੈਗ('i','s','o','l'),
  HB_ਟੈਗ('f','i','n','a'),
  HB_ਟੈਗ('f','i','n','2'),
  HB_ਟੈਗ('f','i','n','3'),
  HB_ਟੈਗ('m','e','d','i'),
  HB_ਟੈਗ('m','e','d','2'),
  HB_ਟੈਗ('i','n','i','t'),
  HB_ਟੈਗ_NONE
};


/* Same order as the feature array */
ਗਿਣਤੀ arabic_action_t {
  ISOL,
  FINA,
  FIN2,
  FIN3,
  MEDI,
  MED2,
  INIT,

  NONE,

  ARABIC_ਅੰਕ_FEATURES = NONE,

  /* We abuse the same byte for other things... */
  STCH_FIXED,
  STCH_REPEATING,
};

ਸਥਿਰ ਅਚਲ ਬਣਤਰ arabic_state_ਸਾਰਣੀ_ਇੰਦਰਾਜ {
	uint8_t prev_action;
	uint8_t curr_action;
	uint16_t ਅਗਲਾ_state;
} arabic_state_ਸਾਰਣੀ[][ਅੰਕ_STATE_MACHINE_ਖਾਨੇ_ਗਿਣਤੀ] =
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


ਸਥਿਰ ਤਾਰਕਿਕ
arabic_fallback_shape (ਅਚਲ hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_ਬਫਰ_t *ਬਫਰ);

ਸਥਿਰ ਤਾਰਕਿਕ
ਰਿਕਾਰਡ_stch (ਅਚਲ hb_ot_shape_plan_t *plan,
	     hb_font_t *font,
	     hb_ਬਫਰ_t *ਬਫਰ);

ਸਥਿਰ ਤਾਰਕਿਕ
deallocate_ਬਫਰ_var (ਅਚਲ hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_ਬਫਰ_t *ਬਫਰ)
{
  HB_ਬਫਰ_DEALLOCATE_VAR (ਬਫਰ, arabic_shaping_action);
  ਵਾਪਸ ਝੂਠ;
}

ਸਥਿਰ ਖਾਲੀ
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


  map->enable_feature (HB_ਟੈਗ('s','t','c','h'));
  map->ਜੋੜੋ_gsub_pause (ਰਿਕਾਰਡ_stch);

  map->enable_feature (HB_ਟੈਗ('c','c','m','p'), F_MANUAL_ZWJ);
  map->enable_feature (HB_ਟੈਗ('l','o','c','l'), F_MANUAL_ZWJ);

  map->ਜੋੜੋ_gsub_pause (ਖਾਲੀ_ਸੂਚਕ);

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ARABIC_ਅੰਕ_FEATURES; i++)
  {
    ਤਾਰਕਿਕ has_fallback = plan->props.script == HB_SCRIPT_ARABIC && !FEATURE_IS_SYRIAC (arabic_features[i]);
    map->ਜੋੜੋ_feature (arabic_features[i], F_MANUAL_ZWJ | (has_fallback ? F_HAS_FALLBACK : F_NONE));
    map->ਜੋੜੋ_gsub_pause (ਖਾਲੀ_ਸੂਚਕ);
  }
   map->ਜੋੜੋ_gsub_pause (deallocate_ਬਫਰ_var);

  /* Normally, Unicode says a ZWNJ means "don't ligate".  In Arabic script
   * however, it says a ZWJ should also mean "don't ligate".  So we run
   * the main ligating features as MANUAL_ZWJ. */

  map->enable_feature (HB_ਟੈਗ('r','l','i','g'), F_MANUAL_ZWJ | F_HAS_FALLBACK);

  ਜੇ (plan->props.script == HB_SCRIPT_ARABIC)
    map->ਜੋੜੋ_gsub_pause (arabic_fallback_shape);

   map->enable_feature (HB_ਟੈਗ('c','a','l','t'), F_MANUAL_ZWJ);
   /* https://github.com/harfbuzz/harfbuzz/issues/1573 */
   ਜੇ (!map->has_feature (HB_ਟੈਗ('r','c','l','t')))
     map->ਜੋੜੋ_gsub_pause (ਖਾਲੀ_ਸੂਚਕ);

   map->enable_feature (HB_ਟੈਗ('l','i','g','a'), F_MANUAL_ZWJ);
   map->enable_feature (HB_ਟੈਗ('c','l','i','g'), F_MANUAL_ZWJ);

  /* The spec includes 'cswh'.  Earlier versions of Windows
   * used to enable this by default, but testing suggests
   * that Windows 8 and later do not enable it by default,
   * and spec now says 'Off by default'.
   * We disabled this in ae23c24c32.
   * Note that IranNastaliq uses this feature extensively
   * to fixup broken glyph sequences.  Oh well...
   * Test case: U+0643,U+0640,U+0631. */
  //map->enable_feature (HB_TAG('c','s','w','h'), F_MANUAL_ZWJ);
  map->enable_feature (HB_ਟੈਗ('m','s','e','t'), F_MANUAL_ZWJ);
}

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shaper-arabic-fallback.hh"

ਬਣਤਰ arabic_shape_plan_t
{
  /* The "+ 1" in the next array is to accommodate for the "NONE" command,
   * which is not an OpenType feature, but this simplifies the code by not
   * having to do a "if (... < NONE) ..." and just rely on the fact that
   * mask_array[NONE] == 0. */
  hb_ਨਕਾਬ_t ਨਕਾਬ_array[ARABIC_ਅੰਕ_FEATURES + 1];

  ਬਦਲਣਯੋਗ hb_atomic_t<arabic_fallback_plan_t *> fallback_plan;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ do_fallback : 1;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ has_stch : 1;
};

ਖਾਲੀ *
ਅੰਕੜੇ_create_arabic (ਅਚਲ hb_ot_shape_plan_t *plan)
{
  arabic_shape_plan_t *arabic_plan = (arabic_shape_plan_t *) hb_calloc (1, ਆਕਾਰ (arabic_shape_plan_t));
  ਜੇ (unlikely (!arabic_plan))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  arabic_plan->do_fallback = plan->props.script == HB_SCRIPT_ARABIC;
  arabic_plan->has_stch = !!plan->map.get_1_ਨਕਾਬ (HB_ਟੈਗ ('s','t','c','h'));
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ARABIC_ਅੰਕ_FEATURES; i++) {
    arabic_plan->ਨਕਾਬ_array[i] = plan->map.get_1_ਨਕਾਬ (arabic_features[i]);
    arabic_plan->do_fallback = arabic_plan->do_fallback &&
			       (FEATURE_IS_SYRIAC (arabic_features[i]) ||
				plan->map.needs_fallback (arabic_features[i]));
  }

  ਵਾਪਸ arabic_plan;
}

ਖਾਲੀ
ਅੰਕੜੇ_destroy_arabic (ਖਾਲੀ *ਅੰਕੜੇ)
{
  arabic_shape_plan_t *arabic_plan = (arabic_shape_plan_t *) ਅੰਕੜੇ;

  arabic_fallback_plan_destroy (arabic_plan->fallback_plan);

  hb_free (ਅੰਕੜੇ);
}

ਸਥਿਰ ਖਾਲੀ
arabic_joining (hb_ਬਫਰ_t *ਬਫਰ)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਬਫਰ->len;
  hb_glyph_info_t *info = ਬਫਰ->info;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ prev = UINT_MAX, state = 0;

  /* Check pre-context */
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਬਫਰ->context_len[0]; i++)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ this_type = get_joining_type (ਬਫਰ->context[0][i], ਬਫਰ->unicode->general_category (ਬਫਰ->context[0][i]));

    ਜੇ (unlikely (this_type == JOINING_TYPE_T))
      ਜਾਰੀ;

    ਅਚਲ arabic_state_ਸਾਰਣੀ_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ = &arabic_state_ਸਾਰਣੀ[state][this_type];
    state = ਇੰਦਰਾਜ->ਅਗਲਾ_state;
    ਰੋਕੋ;
  }

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ this_type = get_joining_type (info[i].codepoint, _hb_glyph_info_get_general_category (&info[i]));

    ਜੇ (unlikely (this_type == JOINING_TYPE_T)) {
      info[i].arabic_shaping_action() = NONE;
      ਜਾਰੀ;
    }

    ਅਚਲ arabic_state_ਸਾਰਣੀ_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ = &arabic_state_ਸਾਰਣੀ[state][this_type];

    ਜੇ (ਇੰਦਰਾਜ->prev_action != NONE && prev != UINT_MAX)
    {
      info[prev].arabic_shaping_action() = ਇੰਦਰਾਜ->prev_action;
      ਬਫਰ->ਸੁਰੱਖਿਅਤ_ਮੰਜ਼ਿਲ_ਪਾਓ_tatweel (prev, i + 1);
    }
    ਨਹੀਂਤਾਂ
    {
      ਜੇ (prev == UINT_MAX)
      {
        ਜੇ (this_type >= JOINING_TYPE_R)
	  ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat_from_outbuffer (0, i + 1);
      }
      ਨਹੀਂਤਾਂ
      {
	ਜੇ (this_type >= JOINING_TYPE_R ||
	    (2 <= state && state <= 5) /* States that have a possible prev_action. */)
	  ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat (prev, i + 1);
      }
    }

    info[i].arabic_shaping_action() = ਇੰਦਰਾਜ->curr_action;

    prev = i;
    state = ਇੰਦਰਾਜ->ਅਗਲਾ_state;
  }

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਬਫਰ->context_len[1]; i++)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ this_type = get_joining_type (ਬਫਰ->context[1][i], ਬਫਰ->unicode->general_category (ਬਫਰ->context[1][i]));

    ਜੇ (unlikely (this_type == JOINING_TYPE_T))
      ਜਾਰੀ;

    ਅਚਲ arabic_state_ਸਾਰਣੀ_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ = &arabic_state_ਸਾਰਣੀ[state][this_type];
    ਜੇ (ਇੰਦਰਾਜ->prev_action != NONE && prev != UINT_MAX)
    {
      info[prev].arabic_shaping_action() = ਇੰਦਰਾਜ->prev_action;
      ਬਫਰ->ਸੁਰੱਖਿਅਤ_ਮੰਜ਼ਿਲ_ਪਾਓ_tatweel (prev, ਬਫਰ->len);
    }
    ਨਹੀਂਤਾਂ ਜੇ (2 <= state && state <= 5) /* States that have a possible prev_action. */
    {
      ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat (prev, ਬਫਰ->len);
    }
    ਰੋਕੋ;
  }
}

ਸਥਿਰ ਖਾਲੀ
mongolian_variation_selectors (hb_ਬਫਰ_t *ਬਫਰ)
{
  /* Copy arabic_shaping_action() from base to Mongolian variation selectors. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਬਫਰ->len;
  hb_glyph_info_t *info = ਬਫਰ->info;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 1; i < ਗਿਣਤੀ_2; i++)
    ਜੇ (unlikely (hb_ਵਿੱਚ_ranges<hb_codepoint_t> (info[i].codepoint, 0x180Bu, 0x180Du, 0x180Fu, 0x180Fu)))
      info[i].arabic_shaping_action() = info[i - 1].arabic_shaping_action();
}

ਖਾਲੀ
ਤਿਆਰੀ_masks_arabic_plan (ਅਚਲ arabic_shape_plan_t *arabic_plan,
			 hb_ਬਫਰ_t               *ਬਫਰ,
			 hb_script_t                script)
{
  HB_ਬਫਰ_ALLOCATE_VAR (ਬਫਰ, arabic_shaping_action);

  arabic_joining (ਬਫਰ);
  ਜੇ (script == HB_SCRIPT_MONGOLIAN)
    mongolian_variation_selectors (ਬਫਰ);

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਬਫਰ->len;
  hb_glyph_info_t *info = ਬਫਰ->info;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    info[i].ਨਕਾਬ |= arabic_plan->ਨਕਾਬ_array[info[i].arabic_shaping_action()];
}

ਸਥਿਰ ਖਾਲੀ
ਤਿਆਰੀ_masks_arabic (ਅਚਲ hb_ot_shape_plan_t *plan,
		    hb_ਬਫਰ_t              *ਬਫਰ,
		    hb_font_t                *font HB_ਅਣਵਰਤਿਆ)
{
  ਅਚਲ arabic_shape_plan_t *arabic_plan = (ਅਚਲ arabic_shape_plan_t *) plan->ਅੰਕੜੇ;
  ਤਿਆਰੀ_masks_arabic_plan (arabic_plan, ਬਫਰ, plan->props.script);
}

ਸਥਿਰ ਤਾਰਕਿਕ
arabic_fallback_shape (ਅਚਲ hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_ਬਫਰ_t *ਬਫਰ)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPER_ARABIC_FALLBACK
  ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ

  ਅਚਲ arabic_shape_plan_t *arabic_plan = (ਅਚਲ arabic_shape_plan_t *) plan->ਅੰਕੜੇ;

  ਜੇ (!arabic_plan->do_fallback)
    ਵਾਪਸ ਝੂਠ;

retry:
  arabic_fallback_plan_t *fallback_plan = arabic_plan->fallback_plan;
  ਜੇ (unlikely (!fallback_plan))
  {
    /* This sucks.  We need a font to build the fallback plan... */
    fallback_plan = arabic_fallback_plan_create (plan, font);
    ਜੇ (unlikely (!arabic_plan->fallback_plan.cmpexch (ਖਾਲੀ_ਸੂਚਕ, fallback_plan)))
    {
      arabic_fallback_plan_destroy (fallback_plan);
      ਜਾਓ retry;
    }
  }

  arabic_fallback_plan_shape (fallback_plan, font, ਬਫਰ);
  ਵਾਪਸ ਸੱਚ;
}

/*
 * Stretch feature: "stch".
 * See example here:
 * https://docs.microsoft.com/en-us/typography/script-development/syriac
 * We implement this in a generic way, such that the Arabic subtending
 * marks can use it as well.
 */

ਸਥਿਰ ਤਾਰਕਿਕ
ਰਿਕਾਰਡ_stch (ਅਚਲ hb_ot_shape_plan_t *plan,
	     hb_font_t *font HB_ਅਣਵਰਤਿਆ,
	     hb_ਬਫਰ_t *ਬਫਰ)
{
  ਅਚਲ arabic_shape_plan_t *arabic_plan = (ਅਚਲ arabic_shape_plan_t *) plan->ਅੰਕੜੇ;
  ਜੇ (!arabic_plan->has_stch)
    ਵਾਪਸ ਝੂਠ;

  /* 'stch' feature was just applied.  Look for anything that multiplied,
   * and record it for stch treatment later.  Note that rtlm, frac, etc
   * are applied before stch, but we assume that they didn't result in
   * anything multiplying into 5 pieces, so it's safe-ish... */

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਬਫਰ->len;
  hb_glyph_info_t *info = ਬਫਰ->info;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    ਜੇ (unlikely (_hb_glyph_info_multiplied (&info[i])))
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ comp = _hb_glyph_info_get_lig_comp (&info[i]);
      info[i].arabic_shaping_action() = comp % 2 ? STCH_REPEATING : STCH_FIXED;
      ਬਫਰ->scratch_flags |= HB_ਬਫਰ_SCRATCH_FLAG_ARABIC_HAS_STCH;
    }
  ਵਾਪਸ ਝੂਠ;
}

ਸਥਿਰ ਖਾਲੀ
apply_stch (ਅਚਲ hb_ot_shape_plan_t *plan HB_ਅਣਵਰਤਿਆ,
	    hb_ਬਫਰ_t              *ਬਫਰ,
	    hb_font_t                *font)
{
  ਜੇ (likely (!(ਬਫਰ->scratch_flags & HB_ਬਫਰ_SCRATCH_FLAG_ARABIC_HAS_STCH)))
    ਵਾਪਸ;

  ਤਾਰਕਿਕ rtl = ਬਫਰ->props.direction == HB_DIRECTION_RTL;

  ਜੇ (!rtl)
    ਬਫਰ->reverse ();

  /* We do a two pass implementation:
   * First pass calculates the exact number of extra glyphs we need,
   * We then enlarge buffer to have that much room,
   * Second pass applies the stretch, copying things to the end of buffer.
   *
   * Cache glyph advances in the position scratch field during the first pass,
   * for re-use these during the second pass.
   */

  ਪੂਰਨ sign = font->x_scale < 0 ? -1 : +1;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ extra_glyphs_needed = 0; // Set during MEASURE, used during CUT
  ਗਿਣਤੀ { MEASURE, CUT } /* step_t */;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਕਦਮ = MEASURE; ਕਦਮ <= CUT; ਕਦਮ = ਕਦਮ + 1)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਬਫਰ->len;
    hb_glyph_info_t *info = ਬਫਰ->info;
    hb_glyph_position_t *pos = ਬਫਰ->pos;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ new_len = ਗਿਣਤੀ_2 + extra_glyphs_needed; // write head during CUT
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = new_len;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਗਿਣਤੀ_2; i; i--)
    {
      ਜੇ (!hb_ਵਿੱਚ_range<uint8_t> (info[i - 1].arabic_shaping_action(), STCH_FIXED, STCH_REPEATING))
      {
	ਜੇ (ਕਦਮ == CUT)
	{
	  --j;
	  info[j] = info[i - 1];
	  pos[j] = pos[i - 1];
	}
	ਜਾਰੀ;
      }

      /* Yay, justification! */

      hb_position_t w_ਕੁੱਲ = 0; // Total to be filled
      hb_position_t w_fixed = 0; // Sum of fixed tiles
      hb_position_t w_repeating = 0; // Sum of repeating tiles
      ਪੂਰਨ n_fixed = 0;
      ਪੂਰਨ n_repeating = 0;

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ = i;
      ਜਦੋਂ (i &&
	     hb_ਵਿੱਚ_range<uint8_t> (info[i - 1].arabic_shaping_action(), STCH_FIXED, STCH_REPEATING))
      {
	i--;
        ਜੇ (ਕਦਮ == MEASURE)
          pos[i].stch_advance() = font->get_glyph_h_advance (info[i].codepoint);
        hb_position_t ਚੌੜਾਈ = pos[i].stch_advance();
	ਜੇ (info[i].arabic_shaping_action() == STCH_FIXED)
	{
	  w_fixed = hb_saturate_ਜੋੜੋ (w_fixed, ਚੌੜਾਈ);
	  n_fixed++;
	}
	ਨਹੀਂਤਾਂ
	{
	  w_repeating = hb_saturate_ਜੋੜੋ (w_repeating, ਚੌੜਾਈ);
	  n_repeating++;
	}
      }
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ = i;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ context = i;
      ਜਦੋਂ (context &&
	     !hb_ਵਿੱਚ_range<uint8_t> (info[context - 1].arabic_shaping_action(), STCH_FIXED, STCH_REPEATING) &&
	     (_hb_glyph_info_is_default_ignorable (&info[context - 1]) ||
	      HB_ARABIC_GENERAL_CATEGORY_IS_ਸ਼ਬਦ (_hb_glyph_info_get_general_category (&info[context - 1]))))
      {
	context--;
	w_ਕੁੱਲ = hb_saturate_ਜੋੜੋ (w_ਕੁੱਲ, pos[context].x_advance);
      }
      i++; // Don't touch i again.

      DEBUG_ਸੁਨੇਹਾ (ARABIC, ਖਾਲੀ_ਸੂਚਕ, "%s stretch at (%u,%u,%u)",
		 ਕਦਮ == MEASURE ? "measuring" : "cutting", context, ਸ਼ੁਰੂ, ਸਿਰਾ);
      DEBUG_ਸੁਨੇਹਾ (ARABIC, ਖਾਲੀ_ਸੂਚਕ, "rest of word:    count=%u width %" PRId32, ਸ਼ੁਰੂ - context, w_ਕੁੱਲ);
      DEBUG_ਸੁਨੇਹਾ (ARABIC, ਖਾਲੀ_ਸੂਚਕ, "fixed tiles:     count=%d width=%" PRId32, n_fixed, w_fixed);
      DEBUG_ਸੁਨੇਹਾ (ARABIC, ਖਾਲੀ_ਸੂਚਕ, "repeating tiles: count=%d width=%" PRId32, n_repeating, w_repeating);

      ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ STCH_MAX_GLYPHS = 256;

      /* Number of additional times to repeat each repeating tile. */
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ n_copies = 0;

      int64_t w_ਬਚਿਆ_signed = (int64_t) w_ਕੁੱਲ - w_fixed;
      int64_t w_repeating_signed = w_repeating;
      ਜੇ (sign < 0)
      {
	w_ਬਚਿਆ_signed = -w_ਬਚਿਆ_signed;
	w_repeating_signed = -w_repeating_signed;
      }
      hb_position_t w_ਬਚਿਆ = hb_saturate_sub (w_ਕੁੱਲ, w_fixed);
      ਜੇ (w_ਬਚਿਆ_signed > w_repeating_signed && w_repeating_signed > 0)
	n_copies = w_ਬਚਿਆ_signed / w_repeating_signed - 1;

      /* See if we can improve the fit by adding an extra repeat and squeezing them together a bit. */
      hb_position_t extra_repeat_overlap = 0;
      int64_t shortfall = w_ਬਚਿਆ_signed - w_repeating_signed * (n_copies + 1);
      ਜੇ (shortfall > 0 && n_repeating > 0)
      {
	++n_copies;
	int64_t excess = (n_copies + 1) * w_repeating_signed - w_ਬਚਿਆ_signed;
	ਜੇ (excess > 0)
	{
	  extra_repeat_overlap = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (excess / ((int64_t) n_copies * (int64_t) n_repeating));
	  w_ਬਚਿਆ = 0;
	}
      }

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ max_copies = 0;
      ਜੇ (n_repeating > 0)
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅਧਾਰ_glyphs = n_fixed + n_repeating;
	ਜੇ (ਅਧਾਰ_glyphs < STCH_MAX_GLYPHS)
	  max_copies = (STCH_MAX_GLYPHS - ਅਧਾਰ_glyphs) / n_repeating;
      }
      n_copies = hb_min (n_copies, max_copies);

      ਜੇ (ਕਦਮ == MEASURE)
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ added_glyphs = 0;
	ਜੇ (unlikely (hb_unsigned_mul_overflows (n_copies, n_repeating, &added_glyphs) ||
		      hb_unsigned_ਜੋੜੋ_overflows (extra_glyphs_needed, added_glyphs, &extra_glyphs_needed)))
	{
	  extra_glyphs_needed = UINT_MAX;
	  ਰੋਕੋ;
	}
	DEBUG_ਸੁਨੇਹਾ (ARABIC, ਖਾਲੀ_ਸੂਚਕ, "will add extra %u copies of repeating tiles", n_copies);
      }
      ਨਹੀਂਤਾਂ
      {
	ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_break (context, ਸਿਰਾ);
	hb_position_t x_ਵਿੱਥ_2 = w_ਬਚਿਆ / 2;
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ k = ਸਿਰਾ; k > ਸ਼ੁਰੂ; k--)
	{
          hb_position_t ਚੌੜਾਈ = pos[k - 1].stch_advance();

	  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ repeat = 1;
	  ਜੇ (info[k - 1].arabic_shaping_action() == STCH_REPEATING)
	    repeat += n_copies;

	  DEBUG_ਸੁਨੇਹਾ (ARABIC, ਖਾਲੀ_ਸੂਚਕ, "appending %u copies of glyph %" PRIu32 "; j=%u",
		     repeat, info[k - 1].codepoint, j);
	  pos[k - 1].x_advance = 0;
	  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ n = 0; n < repeat; n++)
	  {
	    ਜੇ (rtl)
	    {
	      x_ਵਿੱਥ_2 = hb_saturate_sub (x_ਵਿੱਥ_2, ਚੌੜਾਈ);
	      ਜੇ (n > 0)
		x_ਵਿੱਥ_2 = hb_saturate_ਜੋੜੋ (x_ਵਿੱਥ_2, extra_repeat_overlap);
	    }
	    pos[k - 1].x_ਵਿੱਥ_2 = x_ਵਿੱਥ_2;
	    /* Append copy. */
	    --j;
	    info[j] = info[k - 1];
	    pos[j] = pos[k - 1];

	    ਜੇ (!rtl)
	    {
	      x_ਵਿੱਥ_2 = hb_saturate_ਜੋੜੋ (x_ਵਿੱਥ_2, ਚੌੜਾਈ);
	      ਜੇ (n > 0)
		x_ਵਿੱਥ_2 = hb_saturate_sub (x_ਵਿੱਥ_2, extra_repeat_overlap);
	    }
	  }
	}
      }
    }

    ਜੇ (ਕਦਮ == MEASURE)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਕੁੱਲ_glyphs = 0;
      ਜੇ (unlikely (hb_unsigned_ਜੋੜੋ_overflows (ਗਿਣਤੀ_2, extra_glyphs_needed, &ਕੁੱਲ_glyphs) ||
		    !ਬਫਰ->ensure (ਕੁੱਲ_glyphs)))
	ਰੋਕੋ;
    }
    ਨਹੀਂਤਾਂ
    {
      ਪ੍ਰਤਿਜ੍ਞਾ (j == 0);
      ਬਫਰ->len = new_len;
    }
  }

  ਜੇ (!rtl)
    ਬਫਰ->reverse ();
}


ਸਥਿਰ ਖਾਲੀ
postprocess_glyphs_arabic (ਅਚਲ hb_ot_shape_plan_t *plan,
			   hb_ਬਫਰ_t              *ਬਫਰ,
			   hb_font_t                *font)
{
  apply_stch (plan, ਬਫਰ, font);
}

/* https://www.unicode.org/reports/tr53/ */

ਸਥਿਰ ਅਚਲ hb_codepoint_t
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

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
info_is_mcm (ਅਚਲ hb_glyph_info_t &info)
{
  hb_codepoint_t u = info.codepoint;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ARRAY_ਲੰਮਾਈ (modifier_combining_marks); i++)
    ਜੇ (u == modifier_combining_marks[i])
      ਵਾਪਸ ਸੱਚ;
  ਵਾਪਸ ਝੂਠ;
}

ਸਥਿਰ ਖਾਲੀ
reorder_marks_arabic (ਅਚਲ hb_ot_shape_plan_t *plan HB_ਅਣਵਰਤਿਆ,
		      hb_ਬਫਰ_t              *ਬਫਰ,
		      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ              ਸ਼ੁਰੂ,
		      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ              ਸਿਰਾ)
{
  hb_glyph_info_t *info = ਬਫਰ->info;

  DEBUG_ਸੁਨੇਹਾ (ARABIC, ਬਫਰ, "Reordering marks from %u to %u", ਸ਼ੁਰੂ, ਸਿਰਾ);

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cc = 220; cc <= 230; cc += 10)
  {
    DEBUG_ਸੁਨੇਹਾ (ARABIC, ਬਫਰ, "Looking for %u's starting at %u", cc, i);
    ਜਦੋਂ (i < ਸਿਰਾ && info_cc(info[i]) < cc)
      i++;
    DEBUG_ਸੁਨੇਹਾ (ARABIC, ਬਫਰ, "Looking for %u's stopped at %u", cc, i);

    ਜੇ (i == ਸਿਰਾ)
      ਰੋਕੋ;

    ਜੇ (info_cc(info[i]) > cc)
      ਜਾਰੀ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = i;
    ਜਦੋਂ (j < ਸਿਰਾ && info_cc(info[j]) == cc && info_is_mcm (info[j]))
      j++;

    ਜੇ (i == j)
      ਜਾਰੀ;

    DEBUG_ਸੁਨੇਹਾ (ARABIC, ਬਫਰ, "Found %u's from %u to %u", cc, i, j);

    /* Shift it! */
    DEBUG_ਸੁਨੇਹਾ (ARABIC, ਬਫਰ, "Shifting %u's: %u %u", cc, i, j);
    hb_glyph_info_t ਆਰਜ਼ੀ[HB_OT_SHAPE_MAX_COMBINING_MARKS];
    ਪ੍ਰਤਿਜ੍ਞਾ (j - i <= ARRAY_ਲੰਮਾਈ (ਆਰਜ਼ੀ));
    ਬਫਰ->ਮਿਲਾਓ_clusters (ਸ਼ੁਰੂ, j);
    memmove (ਆਰਜ਼ੀ, &info[i], (j - i) * ਆਕਾਰ (hb_glyph_info_t));
    memmove (&info[ਸ਼ੁਰੂ + j - i], &info[ਸ਼ੁਰੂ], (i - ਸ਼ੁਰੂ) * ਆਕਾਰ (hb_glyph_info_t));
    memmove (&info[ਸ਼ੁਰੂ], ਆਰਜ਼ੀ, (j - i) * ਆਕਾਰ (hb_glyph_info_t));

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
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ new_ਸ਼ੁਰੂ = ਸ਼ੁਰੂ + j - i;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ new_cc = cc == 220 ? HB_MODIFIED_COMBINING_CLASS_CCC22 : HB_MODIFIED_COMBINING_CLASS_CCC26;
    ਜਦੋਂ (ਸ਼ੁਰੂ < new_ਸ਼ੁਰੂ)
    {
      hb_glyph_info_ਸੈੱਟ_modified_combining_class (&info[ਸ਼ੁਰੂ], new_cc);
      ਸ਼ੁਰੂ++;
    }

    i = j;
  }
}

ਅਚਲ hb_ot_shaper_t _hb_ot_shaper_arabic =
{
  collect_features_arabic,
  ਖਾਲੀ_ਸੂਚਕ, /* override_features */
  ਅੰਕੜੇ_create_arabic,
  ਅੰਕੜੇ_destroy_arabic,
  ਖਾਲੀ_ਸੂਚਕ, /* preprocess_text */
  postprocess_glyphs_arabic,
  ਖਾਲੀ_ਸੂਚਕ, /* decompose */
  ਖਾਲੀ_ਸੂਚਕ, /* compose */
  ਤਿਆਰੀ_masks_arabic,
  reorder_marks_arabic,
  HB_ਟੈਗ_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_ਬਹੁਲਕ_DEFAULT,
  HB_OT_SHAPE_ZERO_ਚੌੜਾਈ_MARKS_BY_GDEF_LATE,
  ਸੱਚ, /* fallback_position */
};


#ਜੇ_ਅੰਤ

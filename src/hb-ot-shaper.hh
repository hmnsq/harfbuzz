/*
 * Copyright © 2010,2011,2012  Google, Inc.
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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_SHAPER_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_SHAPER_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shape.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shape-normalize.hh"


/* buffer var allocations, used by all OT shapers */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ot_shaper_var_u8_category()	var2.u8[2]
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ot_shaper_var_u8_auxiliary()	var2.u8[3]


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_SHAPE_MAX_COMBINING_MARKS 32

ਗਿਣਤੀ hb_ot_shape_zero_ਚੌੜਾਈ_marks_type_t {
  HB_OT_SHAPE_ZERO_ਚੌੜਾਈ_MARKS_NONE,
  HB_OT_SHAPE_ZERO_ਚੌੜਾਈ_MARKS_BY_GDEF_EARLY,
  HB_OT_SHAPE_ZERO_ਚੌੜਾਈ_MARKS_BY_GDEF_LATE
};


/* Master OT shaper list */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_SHAPERS_IMPLEMENT_SHAPERS \
  HB_OT_SHAPER_IMPLEMENT (arabic) \
  HB_OT_SHAPER_IMPLEMENT (ਮੂਲ) \
  HB_OT_SHAPER_IMPLEMENT (dumber) \
  HB_OT_SHAPER_IMPLEMENT (hangul) \
  HB_OT_SHAPER_IMPLEMENT (hebrew) \
  HB_OT_SHAPER_IMPLEMENT (indic) \
  HB_OT_SHAPER_IMPLEMENT (khmer) \
  HB_OT_SHAPER_IMPLEMENT (myanmar) \
  HB_OT_SHAPER_IMPLEMENT (myanmar_zawgyi) \
  HB_OT_SHAPER_IMPLEMENT (thai) \
  HB_OT_SHAPER_IMPLEMENT (use) \
  /* ^--- Add new shapers here; keep sorted. */


ਬਣਤਰ hb_ot_shaper_t
{
  /* collect_features()
   * Called during shape_plan().
   * Shapers should use plan->map to add their features and callbacks.
   * May be NULL.
   */
  ਖਾਲੀ (*collect_features) (hb_ot_shape_planner_t *plan);

  /* override_features()
   * Called during shape_plan().
   * Shapers should use plan->map to override features and add callbacks after
   * common features are added.
   * May be NULL.
   */
  ਖਾਲੀ (*override_features) (hb_ot_shape_planner_t *plan);


  /* data_create()
   * Called at the end of shape_plan().
   * Whatever shapers return will be accessible through plan->data later.
   * If nullptr is returned, means a plan failure.
   */
  ਖਾਲੀ *(*ਅੰਕੜੇ_create) (ਅਚਲ hb_ot_shape_plan_t *plan);

  /* data_destroy()
   * Called when the shape_plan is being destroyed.
   * plan->data is passed here for destruction.
   * If nullptr is returned, means a plan failure.
   * May be NULL.
   */
  ਖਾਲੀ (*ਅੰਕੜੇ_destroy) (ਖਾਲੀ *ਅੰਕੜੇ);


  /* preprocess_text()
   * Called during shape().
   * Shapers can use to modify text before shaping starts.
   * May be NULL.
   */
  ਖਾਲੀ (*preprocess_ਪਾਠ) (ਅਚਲ hb_ot_shape_plan_t *plan,
			   hb_ਬਫਰ_t              *ਬਫਰ,
			   hb_font_t                *font);

  /* postprocess_glyphs()
   * Called during shape().
   * Shapers can use to modify glyphs after shaping ends.
   * May be NULL.
   */
  ਖਾਲੀ (*postprocess_glyphs) (ਅਚਲ hb_ot_shape_plan_t *plan,
			      hb_ਬਫਰ_t              *ਬਫਰ,
			      hb_font_t                *font);


  /* decompose()
   * Called during shape()'s normalization.
   * May be NULL.
   */
  ਤਾਰਕਿਕ (*decompose) (ਅਚਲ hb_ot_shape_normalize_context_t *c,
		     hb_codepoint_t  ab,
		     hb_codepoint_t *a,
		     hb_codepoint_t *b);

  /* compose()
   * Called during shape()'s normalization.
   * May be NULL.
   */
  ਤਾਰਕਿਕ (*compose) (ਅਚਲ hb_ot_shape_normalize_context_t *c,
		   hb_codepoint_t  a,
		   hb_codepoint_t  b,
		   hb_codepoint_t *ab);

  /* setup_masks()
   * Called during shape().
   * Shapers should use map to get feature masks and set on buffer.
   * Shapers may NOT modify characters.
   * May be NULL.
   */
  ਖਾਲੀ (*ਤਿਆਰੀ_masks) (ਅਚਲ hb_ot_shape_plan_t *plan,
		       hb_ਬਫਰ_t              *ਬਫਰ,
		       hb_font_t                *font);

  /* reorder_marks()
   * Called during shape().
   * Shapers can use to modify ordering of combining marks.
   * May be NULL.
   */
  ਖਾਲੀ (*reorder_marks) (ਅਚਲ hb_ot_shape_plan_t *plan,
			 hb_ਬਫਰ_t              *ਬਫਰ,
			 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ              ਸ਼ੁਰੂ,
			 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ              ਸਿਰਾ);

  /* gpos_tag()
   * If not HB_TAG_NONE, then must match found GPOS script tag for
   * GPOS to be applied.  Otherwise, fallback positioning will be used.
   */
  hb_ਟੈਗ_t gpos_ਟੈਗ;

  hb_ot_shape_normalization_ਬਹੁਲਕ_t normalization_preference;

  hb_ot_shape_zero_ਚੌੜਾਈ_marks_type_t zero_ਚੌੜਾਈ_marks;

  ਤਾਰਕਿਕ fallback_position;
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_SHAPER_IMPLEMENT(name) ਬਾਹਰੀ HB_ਅੰਦਰੂਨੀ ਅਚਲ hb_ot_shaper_t _hb_ot_shaper_##name;
HB_OT_SHAPERS_IMPLEMENT_SHAPERS
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_OT_SHAPER_IMPLEMENT


ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ hb_ot_shaper_t *
hb_ot_shaper_categorize (hb_script_t script,
			 hb_direction_t direction,
			 hb_ਟੈਗ_t gsub_script)
{
  ਚੋਣ ((hb_ਟੈਗ_t) script)
  {
    ਮੂਲ:
      ਵਾਪਸ &_hb_ot_shaper_default;


    /* Unicode-1.1 additions */
    ਹਾਲਤ HB_SCRIPT_ARABIC:

    /* Unicode-3.0 additions */
    ਹਾਲਤ HB_SCRIPT_SYRIAC:

      /* For Arabic script, use the Arabic shaper even if no OT script tag was found.
       * This is because we do fallback shaping for Arabic script (and not others).
       * But note that Arabic shaping is applicable only to horizontal layout; for
       * vertical text, just use the generic shaper instead. */
      ਜੇ ((gsub_script != HB_OT_ਟੈਗ_DEFAULT_SCRIPT || script == HB_SCRIPT_ARABIC) &&
	  HB_DIRECTION_IS_HORIZONTAL (direction))
	ਵਾਪਸ &_hb_ot_shaper_arabic;
      ਨਹੀਂਤਾਂ
	ਵਾਪਸ &_hb_ot_shaper_default;


    /* Unicode-1.1 additions */
    ਹਾਲਤ HB_SCRIPT_THAI:
    ਹਾਲਤ HB_SCRIPT_LAO:

      ਵਾਪਸ &_hb_ot_shaper_thai;


    /* Unicode-1.1 additions */
    ਹਾਲਤ HB_SCRIPT_HANGUL:

      ਵਾਪਸ &_hb_ot_shaper_hangul;


    /* Unicode-1.1 additions */
    ਹਾਲਤ HB_SCRIPT_HEBREW:

      ਵਾਪਸ &_hb_ot_shaper_hebrew;


    /* Unicode-1.1 additions */
    ਹਾਲਤ HB_SCRIPT_BENGALI:
    ਹਾਲਤ HB_SCRIPT_DEVANAGARI:
    ਹਾਲਤ HB_SCRIPT_GUJARATI:
    ਹਾਲਤ HB_SCRIPT_GURMUKHI:
    ਹਾਲਤ HB_SCRIPT_KANNADA:
    ਹਾਲਤ HB_SCRIPT_MALAYALAM:
    ਹਾਲਤ HB_SCRIPT_ORIYA:
    ਹਾਲਤ HB_SCRIPT_TAMIL:
    ਹਾਲਤ HB_SCRIPT_TELUGU:

      /* If the designer designed the font for the 'DFLT' script,
       * (or we ended up arbitrarily pick 'latn'), use the default shaper.
       * Otherwise, use the specific shaper.
       *
       * If it's indy3 tag, send to USE. */
      ਜੇ (gsub_script == HB_ਟੈਗ ('D','F','L','T') ||
	  gsub_script == HB_ਟੈਗ ('l','a','t','n'))
	ਵਾਪਸ &_hb_ot_shaper_default;
      ਨਹੀਂਤਾਂ ਜੇ ((gsub_script & 0x000000FF) == '3')
	ਵਾਪਸ &_hb_ot_shaper_use;
      ਨਹੀਂਤਾਂ
	ਵਾਪਸ &_hb_ot_shaper_indic;

    ਹਾਲਤ HB_SCRIPT_KHMER:
	ਵਾਪਸ &_hb_ot_shaper_khmer;

    ਹਾਲਤ HB_SCRIPT_MYANMAR:
      /* If the designer designed the font for the 'DFLT' script,
       * (or we ended up arbitrarily pick 'latn'), use the default shaper.
       * Otherwise, use the specific shaper.
       *
       * If designer designed for 'mymr' tag, also send to default
       * shaper.  That's tag used from before Myanmar shaping spec
       * was developed.  The shaping spec uses 'mym2' tag. */
      ਜੇ (gsub_script == HB_ਟੈਗ ('D','F','L','T') ||
	  gsub_script == HB_ਟੈਗ ('l','a','t','n') ||
	  gsub_script == HB_ਟੈਗ ('m','y','m','r'))
	ਵਾਪਸ &_hb_ot_shaper_default;
      ਨਹੀਂਤਾਂ
	ਵਾਪਸ &_hb_ot_shaper_myanmar;


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPER_MYANMAR_ZAWGYI
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SCRIPT_MYANMAR_ZAWGYI	((hb_script_t) HB_ਟੈਗ ('Q','a','a','g'))
    ਹਾਲਤ HB_SCRIPT_MYANMAR_ZAWGYI:
    /* https://github.com/harfbuzz/harfbuzz/issues/1162 */

      ਵਾਪਸ &_hb_ot_shaper_myanmar_zawgyi;
#ਜੇ_ਅੰਤ


    /* Unicode-2.0 additions */
    ਹਾਲਤ HB_SCRIPT_TIBETAN:

    /* Unicode-3.0 additions */
    ਹਾਲਤ HB_SCRIPT_MONGOLIAN:
    ਹਾਲਤ HB_SCRIPT_SINHALA:

    /* Unicode-3.2 additions */
    ਹਾਲਤ HB_SCRIPT_BUHID:
    ਹਾਲਤ HB_SCRIPT_HANUNOO:
    ਹਾਲਤ HB_SCRIPT_TAGALOG:
    ਹਾਲਤ HB_SCRIPT_TAGBANWA:

    /* Unicode-4.0 additions */
    ਹਾਲਤ HB_SCRIPT_LIMBU:
    ਹਾਲਤ HB_SCRIPT_TAI_LE:

    /* Unicode-4.1 additions */
    ਹਾਲਤ HB_SCRIPT_BUGINESE:
    ਹਾਲਤ HB_SCRIPT_KHAROSHTHI:
    ਹਾਲਤ HB_SCRIPT_SYLOTI_NAGRI:
    ਹਾਲਤ HB_SCRIPT_TIFINAGH:

    /* Unicode-5.0 additions */
    ਹਾਲਤ HB_SCRIPT_BALINESE:
    ਹਾਲਤ HB_SCRIPT_NKO:
    ਹਾਲਤ HB_SCRIPT_PHAGS_PA:

    /* Unicode-5.1 additions */
    ਹਾਲਤ HB_SCRIPT_CHAM:
    ਹਾਲਤ HB_SCRIPT_KAYAH_LI:
    ਹਾਲਤ HB_SCRIPT_LEPCHA:
    ਹਾਲਤ HB_SCRIPT_REJANG:
    ਹਾਲਤ HB_SCRIPT_SAURASHTRA:
    ਹਾਲਤ HB_SCRIPT_SUNDANESE:

    /* Unicode-5.2 additions */
    ਹਾਲਤ HB_SCRIPT_EGYPTIAN_HIEROGLYPHS:
    ਹਾਲਤ HB_SCRIPT_JAVANESE:
    ਹਾਲਤ HB_SCRIPT_KAITHI:
    ਹਾਲਤ HB_SCRIPT_MEETEI_MAYEK:
    ਹਾਲਤ HB_SCRIPT_TAI_THAM:
    ਹਾਲਤ HB_SCRIPT_TAI_VIET:

    /* Unicode-6.0 additions */
    ਹਾਲਤ HB_SCRIPT_BATAK:
    ਹਾਲਤ HB_SCRIPT_BRAHMI:
    ਹਾਲਤ HB_SCRIPT_MANDAIC:

    /* Unicode-6.1 additions */
    ਹਾਲਤ HB_SCRIPT_CHAKMA:
    ਹਾਲਤ HB_SCRIPT_MIAO:
    ਹਾਲਤ HB_SCRIPT_SHARADA:
    ਹਾਲਤ HB_SCRIPT_TAKRI:

    /* Unicode-7.0 additions */
    ਹਾਲਤ HB_SCRIPT_DUPLOYAN:
    ਹਾਲਤ HB_SCRIPT_GRANTHA:
    ਹਾਲਤ HB_SCRIPT_KHOJKI:
    ਹਾਲਤ HB_SCRIPT_KHUDAWADI:
    ਹਾਲਤ HB_SCRIPT_MAHAJANI:
    ਹਾਲਤ HB_SCRIPT_MANICHAEAN:
    ਹਾਲਤ HB_SCRIPT_MODI:
    ਹਾਲਤ HB_SCRIPT_PAHAWH_HMONG:
    ਹਾਲਤ HB_SCRIPT_PSALTER_PAHLAVI:
    ਹਾਲਤ HB_SCRIPT_SIDDHAM:
    ਹਾਲਤ HB_SCRIPT_TIRHUTA:

    /* Unicode-8.0 additions */
    ਹਾਲਤ HB_SCRIPT_AHOM:
    ਹਾਲਤ HB_SCRIPT_MULTANI:

    /* Unicode-9.0 additions */
    ਹਾਲਤ HB_SCRIPT_ADLAM:
    ਹਾਲਤ HB_SCRIPT_BHAIKSUKI:
    ਹਾਲਤ HB_SCRIPT_MARCHEN:
    ਹਾਲਤ HB_SCRIPT_NEWA:

    /* Unicode-10.0 additions */
    ਹਾਲਤ HB_SCRIPT_MASARAM_GONDI:
    ਹਾਲਤ HB_SCRIPT_SOYOMBO:
    ਹਾਲਤ HB_SCRIPT_ZANABAZAR_ਵਰਗਫਲ:

    /* Unicode-11.0 additions */
    ਹਾਲਤ HB_SCRIPT_DOGRA:
    ਹਾਲਤ HB_SCRIPT_GUNJALA_GONDI:
    ਹਾਲਤ HB_SCRIPT_HANIFI_ROHINGYA:
    ਹਾਲਤ HB_SCRIPT_MAKASAR:
    ਹਾਲਤ HB_SCRIPT_MEDEFAIDRIN:
    ਹਾਲਤ HB_SCRIPT_OLD_SOGDIAN:
    ਹਾਲਤ HB_SCRIPT_SOGDIAN:

    /* Unicode-12.0 additions */
    ਹਾਲਤ HB_SCRIPT_ELYMAIC:
    ਹਾਲਤ HB_SCRIPT_NANDINAGARI:
    ਹਾਲਤ HB_SCRIPT_NYIAKENG_PUACHUE_HMONG:
    ਹਾਲਤ HB_SCRIPT_WANCHO:

    /* Unicode-13.0 additions */
    ਹਾਲਤ HB_SCRIPT_CHORASMIAN:
    ਹਾਲਤ HB_SCRIPT_DIVES_AKURU:
    ਹਾਲਤ HB_SCRIPT_KHITAN_SMALL_SCRIPT:
    ਹਾਲਤ HB_SCRIPT_YEZIDI:

    /* Unicode-14.0 additions */
    ਹਾਲਤ HB_SCRIPT_CYPRO_MINOAN:
    ਹਾਲਤ HB_SCRIPT_OLD_UYGHUR:
    ਹਾਲਤ HB_SCRIPT_TANGSA:
    ਹਾਲਤ HB_SCRIPT_TOTO:
    ਹਾਲਤ HB_SCRIPT_VITHKUQI:

    /* Unicode-15.0 additions */
    ਹਾਲਤ HB_SCRIPT_KAWI:
    ਹਾਲਤ HB_SCRIPT_NAG_MUNDARI:

    /* Unicode-16.0 additions */
    ਹਾਲਤ HB_SCRIPT_GARAY:
    ਹਾਲਤ HB_SCRIPT_GURUNG_KHEMA:
    ਹਾਲਤ HB_SCRIPT_KIRAT_RAI:
    ਹਾਲਤ HB_SCRIPT_OL_ONAL:
    ਹਾਲਤ HB_SCRIPT_SUNUWAR:
    ਹਾਲਤ HB_SCRIPT_TODHRI:
    ਹਾਲਤ HB_SCRIPT_TULU_TIGALARI:

    /* Unicode-17.0 additions */
    ਹਾਲਤ HB_SCRIPT_BERIA_ERFE:
    ਹਾਲਤ HB_SCRIPT_SIDETIC:
    ਹਾਲਤ HB_SCRIPT_TAI_YO:
    ਹਾਲਤ HB_SCRIPT_TOLONG_SIKI:

    /* Unicode-18.0 additions */
    ਹਾਲਤ HB_SCRIPT_JURCHEN:
    ਹਾਲਤ HB_SCRIPT_PROTO_CUNEIFORM:
    ਹਾਲਤ HB_SCRIPT_SEAL:

      /* If the designer designed the font for the 'DFLT' script,
       * (or we ended up arbitrarily pick 'latn'), use the default shaper.
       * Otherwise, use the specific shaper.
       * Note that for some simple scripts, there may not be *any*
       * GSUB/GPOS needed, so there may be no scripts found! */
      ਜੇ (gsub_script == HB_ਟੈਗ ('D','F','L','T') ||
	  gsub_script == HB_ਟੈਗ ('l','a','t','n'))
	ਵਾਪਸ &_hb_ot_shaper_default;
      ਨਹੀਂਤਾਂ
	ਵਾਪਸ &_hb_ot_shaper_use;
  }
}


#ਜੇ_ਅੰਤ /* HB_OT_SHAPER_HH */

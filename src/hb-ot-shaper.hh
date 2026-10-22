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

#اگر_غیر_تعریف_شدہ HB_OT_SHAPER_HH
#تعریف_کرو HB_OT_SHAPER_HH

#شامل "hb.hh"

#شامل "hb-ot-layout.hh"
#شامل "hb-ot-shape.hh"
#شامل "hb-ot-shape-normalize.hh"


/* buffer var allocations, used by all OT shapers */
#تعریف_کرو ot_shaper_var_u8_category()	var2.u8[2]
#تعریف_کرو ot_shaper_var_u8_auxiliary()	var2.u8[3]


#تعریف_کرو HB_OT_SHAPE_MAX_COMBINING_MARKS 32

شمار hb_ot_shape_zero_چوڑائی_marks_type_t {
  HB_OT_SHAPE_ZERO_چوڑائی_MARKS_NONE,
  HB_OT_SHAPE_ZERO_چوڑائی_MARKS_BY_GDEF_EARLY,
  HB_OT_SHAPE_ZERO_چوڑائی_MARKS_BY_GDEF_LATE
};


/* Master OT shaper list */
#تعریف_کرو HB_OT_SHAPERS_IMPLEMENT_SHAPERS \
  HB_OT_SHAPER_IMPLEMENT (arabic) \
  HB_OT_SHAPER_IMPLEMENT (طےشدہ) \
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


ساخت hb_ot_shaper_t
{
  /* collect_features()
   * Called during shape_plan().
   * Shapers should use plan->map to add their features and callbacks.
   * May be NULL.
   */
  خالی (*collect_features) (hb_ot_shape_planner_t *plan);

  /* override_features()
   * Called during shape_plan().
   * Shapers should use plan->map to override features and add callbacks after
   * common features are added.
   * May be NULL.
   */
  خالی (*override_features) (hb_ot_shape_planner_t *plan);


  /* data_create()
   * Called at the end of shape_plan().
   * Whatever shapers return will be accessible through plan->data later.
   * If nullptr is returned, means a plan failure.
   */
  خالی *(*ڈیٹا_create) (ثابت hb_ot_shape_plan_t *plan);

  /* data_destroy()
   * Called when the shape_plan is being destroyed.
   * plan->data is passed here for destruction.
   * If nullptr is returned, means a plan failure.
   * May be NULL.
   */
  خالی (*ڈیٹا_destroy) (خالی *ڈیٹا);


  /* preprocess_text()
   * Called during shape().
   * Shapers can use to modify text before shaping starts.
   * May be NULL.
   */
  خالی (*preprocess_متن) (ثابت hb_ot_shape_plan_t *plan,
			   hb_بفر_t              *بفر,
			   hb_font_t                *font);

  /* postprocess_glyphs()
   * Called during shape().
   * Shapers can use to modify glyphs after shaping ends.
   * May be NULL.
   */
  خالی (*postprocess_glyphs) (ثابت hb_ot_shape_plan_t *plan,
			      hb_بفر_t              *بفر,
			      hb_font_t                *font);


  /* decompose()
   * Called during shape()'s normalization.
   * May be NULL.
   */
  منطقی (*decompose) (ثابت hb_ot_shape_normalize_context_t *c,
		     hb_codepoint_t  ab,
		     hb_codepoint_t *a,
		     hb_codepoint_t *b);

  /* compose()
   * Called during shape()'s normalization.
   * May be NULL.
   */
  منطقی (*compose) (ثابت hb_ot_shape_normalize_context_t *c,
		   hb_codepoint_t  a,
		   hb_codepoint_t  b,
		   hb_codepoint_t *ab);

  /* setup_masks()
   * Called during shape().
   * Shapers should use map to get feature masks and set on buffer.
   * Shapers may NOT modify characters.
   * May be NULL.
   */
  خالی (*تیاری_masks) (ثابت hb_ot_shape_plan_t *plan,
		       hb_بفر_t              *بفر,
		       hb_font_t                *font);

  /* reorder_marks()
   * Called during shape().
   * Shapers can use to modify ordering of combining marks.
   * May be NULL.
   */
  خالی (*reorder_marks) (ثابت hb_ot_shape_plan_t *plan,
			 hb_بفر_t              *بفر,
			 غیرعلامتی صحیح              آغاز,
			 غیرعلامتی صحیح              انتہا);

  /* gpos_tag()
   * If not HB_TAG_NONE, then must match found GPOS script tag for
   * GPOS to be applied.  Otherwise, fallback positioning will be used.
   */
  hb_لیبل_t gpos_لیبل;

  hb_ot_shape_normalization_منوال_t normalization_preference;

  hb_ot_shape_zero_چوڑائی_marks_type_t zero_چوڑائی_marks;

  منطقی fallback_position;
};

#تعریف_کرو HB_OT_SHAPER_IMPLEMENT(name) خارجی HB_اندرونی ثابت hb_ot_shaper_t _hb_ot_shaper_##name;
HB_OT_SHAPERS_IMPLEMENT_SHAPERS
#تعریف_منسوخ HB_OT_SHAPER_IMPLEMENT


ساکن اندرونی ثابت hb_ot_shaper_t *
hb_ot_shaper_categorize (hb_script_t script,
			 hb_direction_t direction,
			 hb_لیبل_t gsub_script)
{
  انتخاب ((hb_لیبل_t) script)
  {
    طےشدہ:
      واپس &_hb_ot_shaper_default;


    /* Unicode-1.1 additions */
    حالت HB_SCRIPT_ARABIC:

    /* Unicode-3.0 additions */
    حالت HB_SCRIPT_SYRIAC:

      /* For Arabic script, use the Arabic shaper even if no OT script tag was found.
       * This is because we do fallback shaping for Arabic script (and not others).
       * But note that Arabic shaping is applicable only to horizontal layout; for
       * vertical text, just use the generic shaper instead. */
      اگر ((gsub_script != HB_OT_لیبل_DEFAULT_SCRIPT || script == HB_SCRIPT_ARABIC) &&
	  HB_DIRECTION_IS_HORIZONTAL (direction))
	واپس &_hb_ot_shaper_arabic;
      ورنہ
	واپس &_hb_ot_shaper_default;


    /* Unicode-1.1 additions */
    حالت HB_SCRIPT_THAI:
    حالت HB_SCRIPT_LAO:

      واپس &_hb_ot_shaper_thai;


    /* Unicode-1.1 additions */
    حالت HB_SCRIPT_HANGUL:

      واپس &_hb_ot_shaper_hangul;


    /* Unicode-1.1 additions */
    حالت HB_SCRIPT_HEBREW:

      واپس &_hb_ot_shaper_hebrew;


    /* Unicode-1.1 additions */
    حالت HB_SCRIPT_BENGALI:
    حالت HB_SCRIPT_DEVANAGARI:
    حالت HB_SCRIPT_GUJARATI:
    حالت HB_SCRIPT_GURMUKHI:
    حالت HB_SCRIPT_KANNADA:
    حالت HB_SCRIPT_MALAYALAM:
    حالت HB_SCRIPT_ORIYA:
    حالت HB_SCRIPT_TAMIL:
    حالت HB_SCRIPT_TELUGU:

      /* If the designer designed the font for the 'DFLT' script,
       * (or we ended up arbitrarily pick 'latn'), use the default shaper.
       * Otherwise, use the specific shaper.
       *
       * If it's indy3 tag, send to USE. */
      اگر (gsub_script == HB_لیبل ('D','F','L','T') ||
	  gsub_script == HB_لیبل ('l','a','t','n'))
	واپس &_hb_ot_shaper_default;
      ورنہ اگر ((gsub_script & 0x000000FF) == '3')
	واپس &_hb_ot_shaper_use;
      ورنہ
	واپس &_hb_ot_shaper_indic;

    حالت HB_SCRIPT_KHMER:
	واپس &_hb_ot_shaper_khmer;

    حالت HB_SCRIPT_MYANMAR:
      /* If the designer designed the font for the 'DFLT' script,
       * (or we ended up arbitrarily pick 'latn'), use the default shaper.
       * Otherwise, use the specific shaper.
       *
       * If designer designed for 'mymr' tag, also send to default
       * shaper.  That's tag used from before Myanmar shaping spec
       * was developed.  The shaping spec uses 'mym2' tag. */
      اگر (gsub_script == HB_لیبل ('D','F','L','T') ||
	  gsub_script == HB_لیبل ('l','a','t','n') ||
	  gsub_script == HB_لیبل ('m','y','m','r'))
	واپس &_hb_ot_shaper_default;
      ورنہ
	واپس &_hb_ot_shaper_myanmar;


#اگر_غیر_تعریف_شدہ HB_NO_OT_SHAPER_MYANMAR_ZAWGYI
#تعریف_کرو HB_SCRIPT_MYANMAR_ZAWGYI	((hb_script_t) HB_لیبل ('Q','a','a','g'))
    حالت HB_SCRIPT_MYANMAR_ZAWGYI:
    /* https://github.com/harfbuzz/harfbuzz/issues/1162 */

      واپس &_hb_ot_shaper_myanmar_zawgyi;
#اگر_ختم


    /* Unicode-2.0 additions */
    حالت HB_SCRIPT_TIBETAN:

    /* Unicode-3.0 additions */
    حالت HB_SCRIPT_MONGOLIAN:
    حالت HB_SCRIPT_SINHALA:

    /* Unicode-3.2 additions */
    حالت HB_SCRIPT_BUHID:
    حالت HB_SCRIPT_HANUNOO:
    حالت HB_SCRIPT_TAGALOG:
    حالت HB_SCRIPT_TAGBANWA:

    /* Unicode-4.0 additions */
    حالت HB_SCRIPT_LIMBU:
    حالت HB_SCRIPT_TAI_LE:

    /* Unicode-4.1 additions */
    حالت HB_SCRIPT_BUGINESE:
    حالت HB_SCRIPT_KHAROSHTHI:
    حالت HB_SCRIPT_SYLOTI_NAGRI:
    حالت HB_SCRIPT_TIFINAGH:

    /* Unicode-5.0 additions */
    حالت HB_SCRIPT_BALINESE:
    حالت HB_SCRIPT_NKO:
    حالت HB_SCRIPT_PHAGS_PA:

    /* Unicode-5.1 additions */
    حالت HB_SCRIPT_CHAM:
    حالت HB_SCRIPT_KAYAH_LI:
    حالت HB_SCRIPT_LEPCHA:
    حالت HB_SCRIPT_REJANG:
    حالت HB_SCRIPT_SAURASHTRA:
    حالت HB_SCRIPT_SUNDANESE:

    /* Unicode-5.2 additions */
    حالت HB_SCRIPT_EGYPTIAN_HIEROGLYPHS:
    حالت HB_SCRIPT_JAVANESE:
    حالت HB_SCRIPT_KAITHI:
    حالت HB_SCRIPT_MEETEI_MAYEK:
    حالت HB_SCRIPT_TAI_THAM:
    حالت HB_SCRIPT_TAI_VIET:

    /* Unicode-6.0 additions */
    حالت HB_SCRIPT_BATAK:
    حالت HB_SCRIPT_BRAHMI:
    حالت HB_SCRIPT_MANDAIC:

    /* Unicode-6.1 additions */
    حالت HB_SCRIPT_CHAKMA:
    حالت HB_SCRIPT_MIAO:
    حالت HB_SCRIPT_SHARADA:
    حالت HB_SCRIPT_TAKRI:

    /* Unicode-7.0 additions */
    حالت HB_SCRIPT_DUPLOYAN:
    حالت HB_SCRIPT_GRANTHA:
    حالت HB_SCRIPT_KHOJKI:
    حالت HB_SCRIPT_KHUDAWADI:
    حالت HB_SCRIPT_MAHAJANI:
    حالت HB_SCRIPT_MANICHAEAN:
    حالت HB_SCRIPT_MODI:
    حالت HB_SCRIPT_PAHAWH_HMONG:
    حالت HB_SCRIPT_PSALTER_PAHLAVI:
    حالت HB_SCRIPT_SIDDHAM:
    حالت HB_SCRIPT_TIRHUTA:

    /* Unicode-8.0 additions */
    حالت HB_SCRIPT_AHOM:
    حالت HB_SCRIPT_MULTANI:

    /* Unicode-9.0 additions */
    حالت HB_SCRIPT_ADLAM:
    حالت HB_SCRIPT_BHAIKSUKI:
    حالت HB_SCRIPT_MARCHEN:
    حالت HB_SCRIPT_NEWA:

    /* Unicode-10.0 additions */
    حالت HB_SCRIPT_MASARAM_GONDI:
    حالت HB_SCRIPT_SOYOMBO:
    حالت HB_SCRIPT_ZANABAZAR_مربع:

    /* Unicode-11.0 additions */
    حالت HB_SCRIPT_DOGRA:
    حالت HB_SCRIPT_GUNJALA_GONDI:
    حالت HB_SCRIPT_HANIFI_ROHINGYA:
    حالت HB_SCRIPT_MAKASAR:
    حالت HB_SCRIPT_MEDEFAIDRIN:
    حالت HB_SCRIPT_OLD_SOGDIAN:
    حالت HB_SCRIPT_SOGDIAN:

    /* Unicode-12.0 additions */
    حالت HB_SCRIPT_ELYMAIC:
    حالت HB_SCRIPT_NANDINAGARI:
    حالت HB_SCRIPT_NYIAKENG_PUACHUE_HMONG:
    حالت HB_SCRIPT_WANCHO:

    /* Unicode-13.0 additions */
    حالت HB_SCRIPT_CHORASMIAN:
    حالت HB_SCRIPT_DIVES_AKURU:
    حالت HB_SCRIPT_KHITAN_SMALL_SCRIPT:
    حالت HB_SCRIPT_YEZIDI:

    /* Unicode-14.0 additions */
    حالت HB_SCRIPT_CYPRO_MINOAN:
    حالت HB_SCRIPT_OLD_UYGHUR:
    حالت HB_SCRIPT_TANGSA:
    حالت HB_SCRIPT_TOTO:
    حالت HB_SCRIPT_VITHKUQI:

    /* Unicode-15.0 additions */
    حالت HB_SCRIPT_KAWI:
    حالت HB_SCRIPT_NAG_MUNDARI:

    /* Unicode-16.0 additions */
    حالت HB_SCRIPT_GARAY:
    حالت HB_SCRIPT_GURUNG_KHEMA:
    حالت HB_SCRIPT_KIRAT_RAI:
    حالت HB_SCRIPT_OL_ONAL:
    حالت HB_SCRIPT_SUNUWAR:
    حالت HB_SCRIPT_TODHRI:
    حالت HB_SCRIPT_TULU_TIGALARI:

    /* Unicode-17.0 additions */
    حالت HB_SCRIPT_BERIA_ERFE:
    حالت HB_SCRIPT_SIDETIC:
    حالت HB_SCRIPT_TAI_YO:
    حالت HB_SCRIPT_TOLONG_SIKI:

    /* Unicode-18.0 additions */
    حالت HB_SCRIPT_JURCHEN:
    حالت HB_SCRIPT_PROTO_CUNEIFORM:
    حالت HB_SCRIPT_SEAL:

      /* If the designer designed the font for the 'DFLT' script,
       * (or we ended up arbitrarily pick 'latn'), use the default shaper.
       * Otherwise, use the specific shaper.
       * Note that for some simple scripts, there may not be *any*
       * GSUB/GPOS needed, so there may be no scripts found! */
      اگر (gsub_script == HB_لیبل ('D','F','L','T') ||
	  gsub_script == HB_لیبل ('l','a','t','n'))
	واپس &_hb_ot_shaper_default;
      ورنہ
	واپس &_hb_ot_shaper_use;
  }
}


#اگر_ختم /* HB_OT_SHAPER_HH */

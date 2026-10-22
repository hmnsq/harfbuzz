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

#నిర్వచితం_కాకపోతే HB_OT_SHAPER_HH
#నిర్వచించు HB_OT_SHAPER_HH

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-ot-layout.hh"
#అన్తర్భావ "hb-ot-shape.hh"
#అన్తర్భావ "hb-ot-shape-normalize.hh"


/* buffer var allocations, used by all OT shapers */
#నిర్వచించు ot_shaper_var_u8_category()	var2.u8[2]
#నిర్వచించు ot_shaper_var_u8_auxiliary()	var2.u8[3]


#నిర్వచించు HB_OT_SHAPE_MAX_COMBINING_MARKS 32

గణన hb_ot_shape_zero_వెడల్పు_marks_type_t {
  HB_OT_SHAPE_ZERO_వెడల్పు_MARKS_NONE,
  HB_OT_SHAPE_ZERO_వెడల్పు_MARKS_BY_GDEF_EARLY,
  HB_OT_SHAPE_ZERO_వెడల్పు_MARKS_BY_GDEF_LATE
};


/* Master OT shaper list */
#నిర్వచించు HB_OT_SHAPERS_IMPLEMENT_SHAPERS \
  HB_OT_SHAPER_IMPLEMENT (arabic) \
  HB_OT_SHAPER_IMPLEMENT (అప్రమేయ) \
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


నిర్మాణం hb_ot_shaper_t
{
  /* collect_features()
   * Called during shape_plan().
   * Shapers should use plan->map to add their features and callbacks.
   * May be NULL.
   */
  శూన్య (*collect_features) (hb_ot_shape_planner_t *plan);

  /* override_features()
   * Called during shape_plan().
   * Shapers should use plan->map to override features and add callbacks after
   * common features are added.
   * May be NULL.
   */
  శూన్య (*override_features) (hb_ot_shape_planner_t *plan);


  /* data_create()
   * Called at the end of shape_plan().
   * Whatever shapers return will be accessible through plan->data later.
   * If nullptr is returned, means a plan failure.
   */
  శూన్య *(*దత్తాంశం_create) (స్థిరరాశి hb_ot_shape_plan_t *plan);

  /* data_destroy()
   * Called when the shape_plan is being destroyed.
   * plan->data is passed here for destruction.
   * If nullptr is returned, means a plan failure.
   * May be NULL.
   */
  శూన్య (*దత్తాంశం_destroy) (శూన్య *దత్తాంశం);


  /* preprocess_text()
   * Called during shape().
   * Shapers can use to modify text before shaping starts.
   * May be NULL.
   */
  శూన్య (*preprocess_పాఠ్యం) (స్థిరరాశి hb_ot_shape_plan_t *plan,
			   hb_బఫరు_t              *బఫరు,
			   hb_font_t                *font);

  /* postprocess_glyphs()
   * Called during shape().
   * Shapers can use to modify glyphs after shaping ends.
   * May be NULL.
   */
  శూన్య (*postprocess_glyphs) (స్థిరరాశి hb_ot_shape_plan_t *plan,
			      hb_బఫరు_t              *బఫరు,
			      hb_font_t                *font);


  /* decompose()
   * Called during shape()'s normalization.
   * May be NULL.
   */
  తార్కిక (*decompose) (స్థిరరాశి hb_ot_shape_normalize_context_t *c,
		     hb_codepoint_t  ab,
		     hb_codepoint_t *a,
		     hb_codepoint_t *b);

  /* compose()
   * Called during shape()'s normalization.
   * May be NULL.
   */
  తార్కిక (*compose) (స్థిరరాశి hb_ot_shape_normalize_context_t *c,
		   hb_codepoint_t  a,
		   hb_codepoint_t  b,
		   hb_codepoint_t *ab);

  /* setup_masks()
   * Called during shape().
   * Shapers should use map to get feature masks and set on buffer.
   * Shapers may NOT modify characters.
   * May be NULL.
   */
  శూన్య (*అమరిక_masks) (స్థిరరాశి hb_ot_shape_plan_t *plan,
		       hb_బఫరు_t              *బఫరు,
		       hb_font_t                *font);

  /* reorder_marks()
   * Called during shape().
   * Shapers can use to modify ordering of combining marks.
   * May be NULL.
   */
  శూన్య (*reorder_marks) (స్థిరరాశి hb_ot_shape_plan_t *plan,
			 hb_బఫరు_t              *బఫరు,
			 గుర్తులేని పూర్ణాంకం              ప్రారంభం,
			 గుర్తులేని పూర్ణాంకం              చివర);

  /* gpos_tag()
   * If not HB_TAG_NONE, then must match found GPOS script tag for
   * GPOS to be applied.  Otherwise, fallback positioning will be used.
   */
  hb_ట్యాగ్_t gpos_ట్యాగ్;

  hb_ot_shape_normalization_బహుళకం_t normalization_preference;

  hb_ot_shape_zero_వెడల్పు_marks_type_t zero_వెడల్పు_marks;

  తార్కిక fallback_position;
};

#నిర్వచించు HB_OT_SHAPER_IMPLEMENT(name) బాహ్య HB_అంతర్గత స్థిరరాశి hb_ot_shaper_t _hb_ot_shaper_##name;
HB_OT_SHAPERS_IMPLEMENT_SHAPERS
#నిర్వచనం_రద్దు HB_OT_SHAPER_IMPLEMENT


స్థిర అంతఃవరుస స్థిరరాశి hb_ot_shaper_t *
hb_ot_shaper_categorize (hb_script_t script,
			 hb_direction_t direction,
			 hb_ట్యాగ్_t gsub_script)
{
  ఎంపిక ((hb_ట్యాగ్_t) script)
  {
    అప్రమేయ:
      తిరిగి &_hb_ot_shaper_default;


    /* Unicode-1.1 additions */
    స్థితి HB_SCRIPT_ARABIC:

    /* Unicode-3.0 additions */
    స్థితి HB_SCRIPT_SYRIAC:

      /* For Arabic script, use the Arabic shaper even if no OT script tag was found.
       * This is because we do fallback shaping for Arabic script (and not others).
       * But note that Arabic shaping is applicable only to horizontal layout; for
       * vertical text, just use the generic shaper instead. */
      ఒకవేళ ((gsub_script != HB_OT_ట్యాగ్_DEFAULT_SCRIPT || script == HB_SCRIPT_ARABIC) &&
	  HB_DIRECTION_IS_HORIZONTAL (direction))
	తిరిగి &_hb_ot_shaper_arabic;
      లేకపోతే
	తిరిగి &_hb_ot_shaper_default;


    /* Unicode-1.1 additions */
    స్థితి HB_SCRIPT_THAI:
    స్థితి HB_SCRIPT_LAO:

      తిరిగి &_hb_ot_shaper_thai;


    /* Unicode-1.1 additions */
    స్థితి HB_SCRIPT_HANGUL:

      తిరిగి &_hb_ot_shaper_hangul;


    /* Unicode-1.1 additions */
    స్థితి HB_SCRIPT_HEBREW:

      తిరిగి &_hb_ot_shaper_hebrew;


    /* Unicode-1.1 additions */
    స్థితి HB_SCRIPT_BENGALI:
    స్థితి HB_SCRIPT_DEVANAGARI:
    స్థితి HB_SCRIPT_GUJARATI:
    స్థితి HB_SCRIPT_GURMUKHI:
    స్థితి HB_SCRIPT_KANNADA:
    స్థితి HB_SCRIPT_MALAYALAM:
    స్థితి HB_SCRIPT_ORIYA:
    స్థితి HB_SCRIPT_TAMIL:
    స్థితి HB_SCRIPT_TELUGU:

      /* If the designer designed the font for the 'DFLT' script,
       * (or we ended up arbitrarily pick 'latn'), use the default shaper.
       * Otherwise, use the specific shaper.
       *
       * If it's indy3 tag, send to USE. */
      ఒకవేళ (gsub_script == HB_ట్యాగ్ ('D','F','L','T') ||
	  gsub_script == HB_ట్యాగ్ ('l','a','t','n'))
	తిరిగి &_hb_ot_shaper_default;
      లేకపోతే ఒకవేళ ((gsub_script & 0x000000FF) == '3')
	తిరిగి &_hb_ot_shaper_use;
      లేకపోతే
	తిరిగి &_hb_ot_shaper_indic;

    స్థితి HB_SCRIPT_KHMER:
	తిరిగి &_hb_ot_shaper_khmer;

    స్థితి HB_SCRIPT_MYANMAR:
      /* If the designer designed the font for the 'DFLT' script,
       * (or we ended up arbitrarily pick 'latn'), use the default shaper.
       * Otherwise, use the specific shaper.
       *
       * If designer designed for 'mymr' tag, also send to default
       * shaper.  That's tag used from before Myanmar shaping spec
       * was developed.  The shaping spec uses 'mym2' tag. */
      ఒకవేళ (gsub_script == HB_ట్యాగ్ ('D','F','L','T') ||
	  gsub_script == HB_ట్యాగ్ ('l','a','t','n') ||
	  gsub_script == HB_ట్యాగ్ ('m','y','m','r'))
	తిరిగి &_hb_ot_shaper_default;
      లేకపోతే
	తిరిగి &_hb_ot_shaper_myanmar;


#నిర్వచితం_కాకపోతే HB_NO_OT_SHAPER_MYANMAR_ZAWGYI
#నిర్వచించు HB_SCRIPT_MYANMAR_ZAWGYI	((hb_script_t) HB_ట్యాగ్ ('Q','a','a','g'))
    స్థితి HB_SCRIPT_MYANMAR_ZAWGYI:
    /* https://github.com/harfbuzz/harfbuzz/issues/1162 */

      తిరిగి &_hb_ot_shaper_myanmar_zawgyi;
#అయితే_ముగింపు


    /* Unicode-2.0 additions */
    స్థితి HB_SCRIPT_TIBETAN:

    /* Unicode-3.0 additions */
    స్థితి HB_SCRIPT_MONGOLIAN:
    స్థితి HB_SCRIPT_SINHALA:

    /* Unicode-3.2 additions */
    స్థితి HB_SCRIPT_BUHID:
    స్థితి HB_SCRIPT_HANUNOO:
    స్థితి HB_SCRIPT_TAGALOG:
    స్థితి HB_SCRIPT_TAGBANWA:

    /* Unicode-4.0 additions */
    స్థితి HB_SCRIPT_LIMBU:
    స్థితి HB_SCRIPT_TAI_LE:

    /* Unicode-4.1 additions */
    స్థితి HB_SCRIPT_BUGINESE:
    స్థితి HB_SCRIPT_KHAROSHTHI:
    స్థితి HB_SCRIPT_SYLOTI_NAGRI:
    స్థితి HB_SCRIPT_TIFINAGH:

    /* Unicode-5.0 additions */
    స్థితి HB_SCRIPT_BALINESE:
    స్థితి HB_SCRIPT_NKO:
    స్థితి HB_SCRIPT_PHAGS_PA:

    /* Unicode-5.1 additions */
    స్థితి HB_SCRIPT_CHAM:
    స్థితి HB_SCRIPT_KAYAH_LI:
    స్థితి HB_SCRIPT_LEPCHA:
    స్థితి HB_SCRIPT_REJANG:
    స్థితి HB_SCRIPT_SAURASHTRA:
    స్థితి HB_SCRIPT_SUNDANESE:

    /* Unicode-5.2 additions */
    స్థితి HB_SCRIPT_EGYPTIAN_HIEROGLYPHS:
    స్థితి HB_SCRIPT_JAVANESE:
    స్థితి HB_SCRIPT_KAITHI:
    స్థితి HB_SCRIPT_MEETEI_MAYEK:
    స్థితి HB_SCRIPT_TAI_THAM:
    స్థితి HB_SCRIPT_TAI_VIET:

    /* Unicode-6.0 additions */
    స్థితి HB_SCRIPT_BATAK:
    స్థితి HB_SCRIPT_BRAHMI:
    స్థితి HB_SCRIPT_MANDAIC:

    /* Unicode-6.1 additions */
    స్థితి HB_SCRIPT_CHAKMA:
    స్థితి HB_SCRIPT_MIAO:
    స్థితి HB_SCRIPT_SHARADA:
    స్థితి HB_SCRIPT_TAKRI:

    /* Unicode-7.0 additions */
    స్థితి HB_SCRIPT_DUPLOYAN:
    స్థితి HB_SCRIPT_GRANTHA:
    స్థితి HB_SCRIPT_KHOJKI:
    స్థితి HB_SCRIPT_KHUDAWADI:
    స్థితి HB_SCRIPT_MAHAJANI:
    స్థితి HB_SCRIPT_MANICHAEAN:
    స్థితి HB_SCRIPT_MODI:
    స్థితి HB_SCRIPT_PAHAWH_HMONG:
    స్థితి HB_SCRIPT_PSALTER_PAHLAVI:
    స్థితి HB_SCRIPT_SIDDHAM:
    స్థితి HB_SCRIPT_TIRHUTA:

    /* Unicode-8.0 additions */
    స్థితి HB_SCRIPT_AHOM:
    స్థితి HB_SCRIPT_MULTANI:

    /* Unicode-9.0 additions */
    స్థితి HB_SCRIPT_ADLAM:
    స్థితి HB_SCRIPT_BHAIKSUKI:
    స్థితి HB_SCRIPT_MARCHEN:
    స్థితి HB_SCRIPT_NEWA:

    /* Unicode-10.0 additions */
    స్థితి HB_SCRIPT_MASARAM_GONDI:
    స్థితి HB_SCRIPT_SOYOMBO:
    స్థితి HB_SCRIPT_ZANABAZAR_వర్గం:

    /* Unicode-11.0 additions */
    స్థితి HB_SCRIPT_DOGRA:
    స్థితి HB_SCRIPT_GUNJALA_GONDI:
    స్థితి HB_SCRIPT_HANIFI_ROHINGYA:
    స్థితి HB_SCRIPT_MAKASAR:
    స్థితి HB_SCRIPT_MEDEFAIDRIN:
    స్థితి HB_SCRIPT_OLD_SOGDIAN:
    స్థితి HB_SCRIPT_SOGDIAN:

    /* Unicode-12.0 additions */
    స్థితి HB_SCRIPT_ELYMAIC:
    స్థితి HB_SCRIPT_NANDINAGARI:
    స్థితి HB_SCRIPT_NYIAKENG_PUACHUE_HMONG:
    స్థితి HB_SCRIPT_WANCHO:

    /* Unicode-13.0 additions */
    స్థితి HB_SCRIPT_CHORASMIAN:
    స్థితి HB_SCRIPT_DIVES_AKURU:
    స్థితి HB_SCRIPT_KHITAN_SMALL_SCRIPT:
    స్థితి HB_SCRIPT_YEZIDI:

    /* Unicode-14.0 additions */
    స్థితి HB_SCRIPT_CYPRO_MINOAN:
    స్థితి HB_SCRIPT_OLD_UYGHUR:
    స్థితి HB_SCRIPT_TANGSA:
    స్థితి HB_SCRIPT_TOTO:
    స్థితి HB_SCRIPT_VITHKUQI:

    /* Unicode-15.0 additions */
    స్థితి HB_SCRIPT_KAWI:
    స్థితి HB_SCRIPT_NAG_MUNDARI:

    /* Unicode-16.0 additions */
    స్థితి HB_SCRIPT_GARAY:
    స్థితి HB_SCRIPT_GURUNG_KHEMA:
    స్థితి HB_SCRIPT_KIRAT_RAI:
    స్థితి HB_SCRIPT_OL_ONAL:
    స్థితి HB_SCRIPT_SUNUWAR:
    స్థితి HB_SCRIPT_TODHRI:
    స్థితి HB_SCRIPT_TULU_TIGALARI:

    /* Unicode-17.0 additions */
    స్థితి HB_SCRIPT_BERIA_ERFE:
    స్థితి HB_SCRIPT_SIDETIC:
    స్థితి HB_SCRIPT_TAI_YO:
    స్థితి HB_SCRIPT_TOLONG_SIKI:

    /* Unicode-18.0 additions */
    స్థితి HB_SCRIPT_JURCHEN:
    స్థితి HB_SCRIPT_PROTO_CUNEIFORM:
    స్థితి HB_SCRIPT_SEAL:

      /* If the designer designed the font for the 'DFLT' script,
       * (or we ended up arbitrarily pick 'latn'), use the default shaper.
       * Otherwise, use the specific shaper.
       * Note that for some simple scripts, there may not be *any*
       * GSUB/GPOS needed, so there may be no scripts found! */
      ఒకవేళ (gsub_script == HB_ట్యాగ్ ('D','F','L','T') ||
	  gsub_script == HB_ట్యాగ్ ('l','a','t','n'))
	తిరిగి &_hb_ot_shaper_default;
      లేకపోతే
	తిరిగి &_hb_ot_shaper_use;
  }
}


#అయితే_ముగింపు /* HB_OT_SHAPER_HH */

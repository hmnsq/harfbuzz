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

#ifndef HB_OT_SHAPER_HH
#define HB_OT_SHAPER_HH

#अन्तर्भाव "hb.hh"

#अन्तर्भाव "hb-ot-layout.hh"
#अन्तर्भाव "hb-ot-shape.hh"
#अन्तर्भाव "hb-ot-shape-normalize.hh"


/* buffer var allocations, used by all OT shapers */
#define ot_shaper_var_u8_category()	var2.u8[2]
#define ot_shaper_var_u8_auxiliary()	var2.u8[3]


#define HB_OT_SHAPE_MAX_COMBINING_MARKS 32

गणना hb_ot_shape_zero_width_marks_type_t {
  HB_OT_SHAPE_ZERO_WIDTH_MARKS_NONE,
  HB_OT_SHAPE_ZERO_WIDTH_MARKS_BY_GDEF_EARLY,
  HB_OT_SHAPE_ZERO_WIDTH_MARKS_BY_GDEF_LATE
};


/* Master OT shaper list */
#define HB_OT_SHAPERS_IMPLEMENT_SHAPERS \
  HB_OT_SHAPER_IMPLEMENT (arabic) \
  HB_OT_SHAPER_IMPLEMENT (स्वभाव) \
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


संरचना hb_ot_shaper_t
{
  /* collect_features()
   * Called during shape_plan().
   * Shapers should use plan->map to add their features and callbacks.
   * May be NULL.
   */
  शून्य (*collect_features) (hb_ot_shape_planner_t *plan);

  /* override_features()
   * Called during shape_plan().
   * Shapers should use plan->map to override features and add callbacks after
   * common features are added.
   * May be NULL.
   */
  शून्य (*override_features) (hb_ot_shape_planner_t *plan);


  /* data_create()
   * Called at the end of shape_plan().
   * Whatever shapers return will be accessible through plan->data later.
   * If nullptr is returned, means a plan failure.
   */
  शून्य *(*data_create) (ध्रुव hb_ot_shape_plan_t *plan);

  /* data_destroy()
   * Called when the shape_plan is being destroyed.
   * plan->data is passed here for destruction.
   * If nullptr is returned, means a plan failure.
   * May be NULL.
   */
  शून्य (*data_destroy) (शून्य *data);


  /* preprocess_text()
   * Called during shape().
   * Shapers can use to modify text before shaping starts.
   * May be NULL.
   */
  शून्य (*preprocess_text) (ध्रुव hb_ot_shape_plan_t *plan,
			   hb_buffer_t              *buffer,
			   hb_font_t                *font);

  /* postprocess_glyphs()
   * Called during shape().
   * Shapers can use to modify glyphs after shaping ends.
   * May be NULL.
   */
  शून्य (*postprocess_glyphs) (ध्रुव hb_ot_shape_plan_t *plan,
			      hb_buffer_t              *buffer,
			      hb_font_t                *font);


  /* decompose()
   * Called during shape()'s normalization.
   * May be NULL.
   */
  तार्किक (*decompose) (ध्रुव hb_ot_shape_normalize_context_t *c,
		     hb_codepoint_t  ab,
		     hb_codepoint_t *a,
		     hb_codepoint_t *b);

  /* compose()
   * Called during shape()'s normalization.
   * May be NULL.
   */
  तार्किक (*compose) (ध्रुव hb_ot_shape_normalize_context_t *c,
		   hb_codepoint_t  a,
		   hb_codepoint_t  b,
		   hb_codepoint_t *ab);

  /* setup_masks()
   * Called during shape().
   * Shapers should use map to get feature masks and set on buffer.
   * Shapers may NOT modify characters.
   * May be NULL.
   */
  शून्य (*setup_masks) (ध्रुव hb_ot_shape_plan_t *plan,
		       hb_buffer_t              *buffer,
		       hb_font_t                *font);

  /* reorder_marks()
   * Called during shape().
   * Shapers can use to modify ordering of combining marks.
   * May be NULL.
   */
  शून्य (*reorder_marks) (ध्रुव hb_ot_shape_plan_t *plan,
			 hb_buffer_t              *buffer,
			 unsigned पूर्णाङ्क              start,
			 unsigned पूर्णाङ्क              end);

  /* gpos_tag()
   * If not HB_TAG_NONE, then must match found GPOS script tag for
   * GPOS to be applied.  Otherwise, fallback positioning will be used.
   */
  hb_tag_t gpos_tag;

  hb_ot_shape_normalization_mode_t normalization_preference;

  hb_ot_shape_zero_width_marks_type_t zero_width_marks;

  तार्किक fallback_position;
};

#define HB_OT_SHAPER_IMPLEMENT(name) बाह्य HB_INTERNAL ध्रुव hb_ot_shaper_t _hb_ot_shaper_##name;
HB_OT_SHAPERS_IMPLEMENT_SHAPERS
#undef HB_OT_SHAPER_IMPLEMENT


स्थिर अन्तःस्थ ध्रुव hb_ot_shaper_t *
hb_ot_shaper_categorize (hb_script_t script,
			 hb_direction_t direction,
			 hb_tag_t gsub_script)
{
  विकल्प ((hb_tag_t) script)
  {
    स्वभाव:
      प्रत्यागम &_hb_ot_shaper_default;


    /* Unicode-1.1 additions */
    पक्ष HB_SCRIPT_ARABIC:

    /* Unicode-3.0 additions */
    पक्ष HB_SCRIPT_SYRIAC:

      /* For Arabic script, use the Arabic shaper even if no OT script tag was found.
       * This is because we do fallback shaping for Arabic script (and not others).
       * But note that Arabic shaping is applicable only to horizontal layout; for
       * vertical text, just use the generic shaper instead. */
      जइ ((gsub_script != HB_OT_TAG_DEFAULT_SCRIPT || script == HB_SCRIPT_ARABIC) &&
	  HB_DIRECTION_IS_HORIZONTAL (direction))
	प्रत्यागम &_hb_ot_shaper_arabic;
      अण्णहा
	प्रत्यागम &_hb_ot_shaper_default;


    /* Unicode-1.1 additions */
    पक्ष HB_SCRIPT_THAI:
    पक्ष HB_SCRIPT_LAO:

      प्रत्यागम &_hb_ot_shaper_thai;


    /* Unicode-1.1 additions */
    पक्ष HB_SCRIPT_HANGUL:

      प्रत्यागम &_hb_ot_shaper_hangul;


    /* Unicode-1.1 additions */
    पक्ष HB_SCRIPT_HEBREW:

      प्रत्यागम &_hb_ot_shaper_hebrew;


    /* Unicode-1.1 additions */
    पक्ष HB_SCRIPT_BENGALI:
    पक्ष HB_SCRIPT_DEVANAGARI:
    पक्ष HB_SCRIPT_GUJARATI:
    पक्ष HB_SCRIPT_GURMUKHI:
    पक्ष HB_SCRIPT_KANNADA:
    पक्ष HB_SCRIPT_MALAYALAM:
    पक्ष HB_SCRIPT_ORIYA:
    पक्ष HB_SCRIPT_TAMIL:
    पक्ष HB_SCRIPT_TELUGU:

      /* If the designer designed the font for the 'DFLT' script,
       * (or we ended up arbitrarily pick 'latn'), use the default shaper.
       * Otherwise, use the specific shaper.
       *
       * If it's indy3 tag, send to USE. */
      जइ (gsub_script == HB_TAG ('D','F','L','T') ||
	  gsub_script == HB_TAG ('l','a','t','n'))
	प्रत्यागम &_hb_ot_shaper_default;
      अण्णहा जइ ((gsub_script & 0x000000FF) == '3')
	प्रत्यागम &_hb_ot_shaper_use;
      अण्णहा
	प्रत्यागम &_hb_ot_shaper_indic;

    पक्ष HB_SCRIPT_KHMER:
	प्रत्यागम &_hb_ot_shaper_khmer;

    पक्ष HB_SCRIPT_MYANMAR:
      /* If the designer designed the font for the 'DFLT' script,
       * (or we ended up arbitrarily pick 'latn'), use the default shaper.
       * Otherwise, use the specific shaper.
       *
       * If designer designed for 'mymr' tag, also send to default
       * shaper.  That's tag used from before Myanmar shaping spec
       * was developed.  The shaping spec uses 'mym2' tag. */
      जइ (gsub_script == HB_TAG ('D','F','L','T') ||
	  gsub_script == HB_TAG ('l','a','t','n') ||
	  gsub_script == HB_TAG ('m','y','m','r'))
	प्रत्यागम &_hb_ot_shaper_default;
      अण्णहा
	प्रत्यागम &_hb_ot_shaper_myanmar;


#ifndef HB_NO_OT_SHAPER_MYANMAR_ZAWGYI
#define HB_SCRIPT_MYANMAR_ZAWGYI	((hb_script_t) HB_TAG ('Q','a','a','g'))
    पक्ष HB_SCRIPT_MYANMAR_ZAWGYI:
    /* https://github.com/harfbuzz/harfbuzz/issues/1162 */

      प्रत्यागम &_hb_ot_shaper_myanmar_zawgyi;
#endif


    /* Unicode-2.0 additions */
    पक्ष HB_SCRIPT_TIBETAN:

    /* Unicode-3.0 additions */
    पक्ष HB_SCRIPT_MONGOLIAN:
    पक्ष HB_SCRIPT_SINHALA:

    /* Unicode-3.2 additions */
    पक्ष HB_SCRIPT_BUHID:
    पक्ष HB_SCRIPT_HANUNOO:
    पक्ष HB_SCRIPT_TAGALOG:
    पक्ष HB_SCRIPT_TAGBANWA:

    /* Unicode-4.0 additions */
    पक्ष HB_SCRIPT_LIMBU:
    पक्ष HB_SCRIPT_TAI_LE:

    /* Unicode-4.1 additions */
    पक्ष HB_SCRIPT_BUGINESE:
    पक्ष HB_SCRIPT_KHAROSHTHI:
    पक्ष HB_SCRIPT_SYLOTI_NAGRI:
    पक्ष HB_SCRIPT_TIFINAGH:

    /* Unicode-5.0 additions */
    पक्ष HB_SCRIPT_BALINESE:
    पक्ष HB_SCRIPT_NKO:
    पक्ष HB_SCRIPT_PHAGS_PA:

    /* Unicode-5.1 additions */
    पक्ष HB_SCRIPT_CHAM:
    पक्ष HB_SCRIPT_KAYAH_LI:
    पक्ष HB_SCRIPT_LEPCHA:
    पक्ष HB_SCRIPT_REJANG:
    पक्ष HB_SCRIPT_SAURASHTRA:
    पक्ष HB_SCRIPT_SUNDANESE:

    /* Unicode-5.2 additions */
    पक्ष HB_SCRIPT_EGYPTIAN_HIEROGLYPHS:
    पक्ष HB_SCRIPT_JAVANESE:
    पक्ष HB_SCRIPT_KAITHI:
    पक्ष HB_SCRIPT_MEETEI_MAYEK:
    पक्ष HB_SCRIPT_TAI_THAM:
    पक्ष HB_SCRIPT_TAI_VIET:

    /* Unicode-6.0 additions */
    पक्ष HB_SCRIPT_BATAK:
    पक्ष HB_SCRIPT_BRAHMI:
    पक्ष HB_SCRIPT_MANDAIC:

    /* Unicode-6.1 additions */
    पक्ष HB_SCRIPT_CHAKMA:
    पक्ष HB_SCRIPT_MIAO:
    पक्ष HB_SCRIPT_SHARADA:
    पक्ष HB_SCRIPT_TAKRI:

    /* Unicode-7.0 additions */
    पक्ष HB_SCRIPT_DUPLOYAN:
    पक्ष HB_SCRIPT_GRANTHA:
    पक्ष HB_SCRIPT_KHOJKI:
    पक्ष HB_SCRIPT_KHUDAWADI:
    पक्ष HB_SCRIPT_MAHAJANI:
    पक्ष HB_SCRIPT_MANICHAEAN:
    पक्ष HB_SCRIPT_MODI:
    पक्ष HB_SCRIPT_PAHAWH_HMONG:
    पक्ष HB_SCRIPT_PSALTER_PAHLAVI:
    पक्ष HB_SCRIPT_SIDDHAM:
    पक्ष HB_SCRIPT_TIRHUTA:

    /* Unicode-8.0 additions */
    पक्ष HB_SCRIPT_AHOM:
    पक्ष HB_SCRIPT_MULTANI:

    /* Unicode-9.0 additions */
    पक्ष HB_SCRIPT_ADLAM:
    पक्ष HB_SCRIPT_BHAIKSUKI:
    पक्ष HB_SCRIPT_MARCHEN:
    पक्ष HB_SCRIPT_NEWA:

    /* Unicode-10.0 additions */
    पक्ष HB_SCRIPT_MASARAM_GONDI:
    पक्ष HB_SCRIPT_SOYOMBO:
    पक्ष HB_SCRIPT_ZANABAZAR_SQUARE:

    /* Unicode-11.0 additions */
    पक्ष HB_SCRIPT_DOGRA:
    पक्ष HB_SCRIPT_GUNJALA_GONDI:
    पक्ष HB_SCRIPT_HANIFI_ROHINGYA:
    पक्ष HB_SCRIPT_MAKASAR:
    पक्ष HB_SCRIPT_MEDEFAIDRIN:
    पक्ष HB_SCRIPT_OLD_SOGDIAN:
    पक्ष HB_SCRIPT_SOGDIAN:

    /* Unicode-12.0 additions */
    पक्ष HB_SCRIPT_ELYMAIC:
    पक्ष HB_SCRIPT_NANDINAGARI:
    पक्ष HB_SCRIPT_NYIAKENG_PUACHUE_HMONG:
    पक्ष HB_SCRIPT_WANCHO:

    /* Unicode-13.0 additions */
    पक्ष HB_SCRIPT_CHORASMIAN:
    पक्ष HB_SCRIPT_DIVES_AKURU:
    पक्ष HB_SCRIPT_KHITAN_SMALL_SCRIPT:
    पक्ष HB_SCRIPT_YEZIDI:

    /* Unicode-14.0 additions */
    पक्ष HB_SCRIPT_CYPRO_MINOAN:
    पक्ष HB_SCRIPT_OLD_UYGHUR:
    पक्ष HB_SCRIPT_TANGSA:
    पक्ष HB_SCRIPT_TOTO:
    पक्ष HB_SCRIPT_VITHKUQI:

    /* Unicode-15.0 additions */
    पक्ष HB_SCRIPT_KAWI:
    पक्ष HB_SCRIPT_NAG_MUNDARI:

    /* Unicode-16.0 additions */
    पक्ष HB_SCRIPT_GARAY:
    पक्ष HB_SCRIPT_GURUNG_KHEMA:
    पक्ष HB_SCRIPT_KIRAT_RAI:
    पक्ष HB_SCRIPT_OL_ONAL:
    पक्ष HB_SCRIPT_SUNUWAR:
    पक्ष HB_SCRIPT_TODHRI:
    पक्ष HB_SCRIPT_TULU_TIGALARI:

    /* Unicode-17.0 additions */
    पक्ष HB_SCRIPT_BERIA_ERFE:
    पक्ष HB_SCRIPT_SIDETIC:
    पक्ष HB_SCRIPT_TAI_YO:
    पक्ष HB_SCRIPT_TOLONG_SIKI:

    /* Unicode-18.0 additions */
    पक्ष HB_SCRIPT_JURCHEN:
    पक्ष HB_SCRIPT_PROTO_CUNEIFORM:
    पक्ष HB_SCRIPT_SEAL:

      /* If the designer designed the font for the 'DFLT' script,
       * (or we ended up arbitrarily pick 'latn'), use the default shaper.
       * Otherwise, use the specific shaper.
       * Note that for some simple scripts, there may not be *any*
       * GSUB/GPOS needed, so there may be no scripts found! */
      जइ (gsub_script == HB_TAG ('D','F','L','T') ||
	  gsub_script == HB_TAG ('l','a','t','n'))
	प्रत्यागम &_hb_ot_shaper_default;
      अण्णहा
	प्रत्यागम &_hb_ot_shaper_use;
  }
}


#endif /* HB_OT_SHAPER_HH */

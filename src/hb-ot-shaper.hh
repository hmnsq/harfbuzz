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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ಅನ್ತರ್ಭಾವ "hb-ot-layout.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-shape.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-shape-normalize.hh"


/* buffer var allocations, used by all OT shapers */
#define ot_shaper_var_u8_category()	var2.u8[2]
#define ot_shaper_var_u8_auxiliary()	var2.u8[3]


#define HB_OT_SHAPE_MAX_COMBINING_MARKS 32

ಗಣನಾ hb_ot_shape_zero_width_marks_type_t {
  HB_OT_SHAPE_ZERO_WIDTH_MARKS_NONE,
  HB_OT_SHAPE_ZERO_WIDTH_MARKS_BY_GDEF_EARLY,
  HB_OT_SHAPE_ZERO_WIDTH_MARKS_BY_GDEF_LATE
};


/* Master OT shaper list */
#define HB_OT_SHAPERS_IMPLEMENT_SHAPERS \
  HB_OT_SHAPER_IMPLEMENT (arabic) \
  HB_OT_SHAPER_IMPLEMENT (ಸ್ವಭಾವ) \
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


ಸಂರಚನಾ hb_ot_shaper_t
{
  /* collect_features()
   * Called during shape_plan().
   * Shapers should use plan->map to add their features and callbacks.
   * May be NULL.
   */
  ಶೂನ್ಯ (*collect_features) (hb_ot_shape_planner_t *plan);

  /* override_features()
   * Called during shape_plan().
   * Shapers should use plan->map to override features and add callbacks after
   * common features are added.
   * May be NULL.
   */
  ಶೂನ್ಯ (*override_features) (hb_ot_shape_planner_t *plan);


  /* data_create()
   * Called at the end of shape_plan().
   * Whatever shapers return will be accessible through plan->data later.
   * If nullptr is returned, means a plan failure.
   */
  ಶೂನ್ಯ *(*data_create) (ಧ್ರುವ hb_ot_shape_plan_t *plan);

  /* data_destroy()
   * Called when the shape_plan is being destroyed.
   * plan->data is passed here for destruction.
   * If nullptr is returned, means a plan failure.
   * May be NULL.
   */
  ಶೂನ್ಯ (*data_destroy) (ಶೂನ್ಯ *data);


  /* preprocess_text()
   * Called during shape().
   * Shapers can use to modify text before shaping starts.
   * May be NULL.
   */
  ಶೂನ್ಯ (*preprocess_text) (ಧ್ರುವ hb_ot_shape_plan_t *plan,
			   hb_buffer_t              *buffer,
			   hb_font_t                *font);

  /* postprocess_glyphs()
   * Called during shape().
   * Shapers can use to modify glyphs after shaping ends.
   * May be NULL.
   */
  ಶೂನ್ಯ (*postprocess_glyphs) (ಧ್ರುವ hb_ot_shape_plan_t *plan,
			      hb_buffer_t              *buffer,
			      hb_font_t                *font);


  /* decompose()
   * Called during shape()'s normalization.
   * May be NULL.
   */
  ತಾರ್ಕಿಕ (*decompose) (ಧ್ರುವ hb_ot_shape_normalize_context_t *c,
		     hb_codepoint_t  ab,
		     hb_codepoint_t *a,
		     hb_codepoint_t *b);

  /* compose()
   * Called during shape()'s normalization.
   * May be NULL.
   */
  ತಾರ್ಕಿಕ (*compose) (ಧ್ರುವ hb_ot_shape_normalize_context_t *c,
		   hb_codepoint_t  a,
		   hb_codepoint_t  b,
		   hb_codepoint_t *ab);

  /* setup_masks()
   * Called during shape().
   * Shapers should use map to get feature masks and set on buffer.
   * Shapers may NOT modify characters.
   * May be NULL.
   */
  ಶೂನ್ಯ (*setup_masks) (ಧ್ರುವ hb_ot_shape_plan_t *plan,
		       hb_buffer_t              *buffer,
		       hb_font_t                *font);

  /* reorder_marks()
   * Called during shape().
   * Shapers can use to modify ordering of combining marks.
   * May be NULL.
   */
  ಶೂನ್ಯ (*reorder_marks) (ಧ್ರುವ hb_ot_shape_plan_t *plan,
			 hb_buffer_t              *buffer,
			 unsigned ಪೂರ್ಣಾಙ್ಕ              start,
			 unsigned ಪೂರ್ಣಾಙ್ಕ              end);

  /* gpos_tag()
   * If not HB_TAG_NONE, then must match found GPOS script tag for
   * GPOS to be applied.  Otherwise, fallback positioning will be used.
   */
  hb_tag_t gpos_tag;

  hb_ot_shape_normalization_mode_t normalization_preference;

  hb_ot_shape_zero_width_marks_type_t zero_width_marks;

  ತಾರ್ಕಿಕ fallback_position;
};

#define HB_OT_SHAPER_IMPLEMENT(name) ಬಾಹ್ಯ HB_INTERNAL ಧ್ರುವ hb_ot_shaper_t _hb_ot_shaper_##name;
HB_OT_SHAPERS_IMPLEMENT_SHAPERS
#undef HB_OT_SHAPER_IMPLEMENT


ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ hb_ot_shaper_t *
hb_ot_shaper_categorize (hb_script_t script,
			 hb_direction_t direction,
			 hb_tag_t gsub_script)
{
  ವಿಕಲ್ಪ ((hb_tag_t) script)
  {
    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ &_hb_ot_shaper_default;


    /* Unicode-1.1 additions */
    ಪಕ್ಷ HB_SCRIPT_ARABIC:

    /* Unicode-3.0 additions */
    ಪಕ್ಷ HB_SCRIPT_SYRIAC:

      /* For Arabic script, use the Arabic shaper even if no OT script tag was found.
       * This is because we do fallback shaping for Arabic script (and not others).
       * But note that Arabic shaping is applicable only to horizontal layout; for
       * vertical text, just use the generic shaper instead. */
      ಯದಿ ((gsub_script != HB_OT_TAG_DEFAULT_SCRIPT || script == HB_SCRIPT_ARABIC) &&
	  HB_DIRECTION_IS_HORIZONTAL (direction))
	ಪ್ರತ್ಯಾಗಮ &_hb_ot_shaper_arabic;
      ಅನ್ಯಥಾ
	ಪ್ರತ್ಯಾಗಮ &_hb_ot_shaper_default;


    /* Unicode-1.1 additions */
    ಪಕ್ಷ HB_SCRIPT_THAI:
    ಪಕ್ಷ HB_SCRIPT_LAO:

      ಪ್ರತ್ಯಾಗಮ &_hb_ot_shaper_thai;


    /* Unicode-1.1 additions */
    ಪಕ್ಷ HB_SCRIPT_HANGUL:

      ಪ್ರತ್ಯಾಗಮ &_hb_ot_shaper_hangul;


    /* Unicode-1.1 additions */
    ಪಕ್ಷ HB_SCRIPT_HEBREW:

      ಪ್ರತ್ಯಾಗಮ &_hb_ot_shaper_hebrew;


    /* Unicode-1.1 additions */
    ಪಕ್ಷ HB_SCRIPT_BENGALI:
    ಪಕ್ಷ HB_SCRIPT_DEVANAGARI:
    ಪಕ್ಷ HB_SCRIPT_GUJARATI:
    ಪಕ್ಷ HB_SCRIPT_GURMUKHI:
    ಪಕ್ಷ HB_SCRIPT_KANNADA:
    ಪಕ್ಷ HB_SCRIPT_MALAYALAM:
    ಪಕ್ಷ HB_SCRIPT_ORIYA:
    ಪಕ್ಷ HB_SCRIPT_TAMIL:
    ಪಕ್ಷ HB_SCRIPT_TELUGU:

      /* If the designer designed the font for the 'DFLT' script,
       * (or we ended up arbitrarily pick 'latn'), use the default shaper.
       * Otherwise, use the specific shaper.
       *
       * If it's indy3 tag, send to USE. */
      ಯದಿ (gsub_script == HB_TAG ('D','F','L','T') ||
	  gsub_script == HB_TAG ('l','a','t','n'))
	ಪ್ರತ್ಯಾಗಮ &_hb_ot_shaper_default;
      ಅನ್ಯಥಾ ಯದಿ ((gsub_script & 0x000000FF) == '3')
	ಪ್ರತ್ಯಾಗಮ &_hb_ot_shaper_use;
      ಅನ್ಯಥಾ
	ಪ್ರತ್ಯಾಗಮ &_hb_ot_shaper_indic;

    ಪಕ್ಷ HB_SCRIPT_KHMER:
	ಪ್ರತ್ಯಾಗಮ &_hb_ot_shaper_khmer;

    ಪಕ್ಷ HB_SCRIPT_MYANMAR:
      /* If the designer designed the font for the 'DFLT' script,
       * (or we ended up arbitrarily pick 'latn'), use the default shaper.
       * Otherwise, use the specific shaper.
       *
       * If designer designed for 'mymr' tag, also send to default
       * shaper.  That's tag used from before Myanmar shaping spec
       * was developed.  The shaping spec uses 'mym2' tag. */
      ಯದಿ (gsub_script == HB_TAG ('D','F','L','T') ||
	  gsub_script == HB_TAG ('l','a','t','n') ||
	  gsub_script == HB_TAG ('m','y','m','r'))
	ಪ್ರತ್ಯಾಗಮ &_hb_ot_shaper_default;
      ಅನ್ಯಥಾ
	ಪ್ರತ್ಯಾಗಮ &_hb_ot_shaper_myanmar;


#ifndef HB_NO_OT_SHAPER_MYANMAR_ZAWGYI
#define HB_SCRIPT_MYANMAR_ZAWGYI	((hb_script_t) HB_TAG ('Q','a','a','g'))
    ಪಕ್ಷ HB_SCRIPT_MYANMAR_ZAWGYI:
    /* https://github.com/harfbuzz/harfbuzz/issues/1162 */

      ಪ್ರತ್ಯಾಗಮ &_hb_ot_shaper_myanmar_zawgyi;
#endif


    /* Unicode-2.0 additions */
    ಪಕ್ಷ HB_SCRIPT_TIBETAN:

    /* Unicode-3.0 additions */
    ಪಕ್ಷ HB_SCRIPT_MONGOLIAN:
    ಪಕ್ಷ HB_SCRIPT_SINHALA:

    /* Unicode-3.2 additions */
    ಪಕ್ಷ HB_SCRIPT_BUHID:
    ಪಕ್ಷ HB_SCRIPT_HANUNOO:
    ಪಕ್ಷ HB_SCRIPT_TAGALOG:
    ಪಕ್ಷ HB_SCRIPT_TAGBANWA:

    /* Unicode-4.0 additions */
    ಪಕ್ಷ HB_SCRIPT_LIMBU:
    ಪಕ್ಷ HB_SCRIPT_TAI_LE:

    /* Unicode-4.1 additions */
    ಪಕ್ಷ HB_SCRIPT_BUGINESE:
    ಪಕ್ಷ HB_SCRIPT_KHAROSHTHI:
    ಪಕ್ಷ HB_SCRIPT_SYLOTI_NAGRI:
    ಪಕ್ಷ HB_SCRIPT_TIFINAGH:

    /* Unicode-5.0 additions */
    ಪಕ್ಷ HB_SCRIPT_BALINESE:
    ಪಕ್ಷ HB_SCRIPT_NKO:
    ಪಕ್ಷ HB_SCRIPT_PHAGS_PA:

    /* Unicode-5.1 additions */
    ಪಕ್ಷ HB_SCRIPT_CHAM:
    ಪಕ್ಷ HB_SCRIPT_KAYAH_LI:
    ಪಕ್ಷ HB_SCRIPT_LEPCHA:
    ಪಕ್ಷ HB_SCRIPT_REJANG:
    ಪಕ್ಷ HB_SCRIPT_SAURASHTRA:
    ಪಕ್ಷ HB_SCRIPT_SUNDANESE:

    /* Unicode-5.2 additions */
    ಪಕ್ಷ HB_SCRIPT_EGYPTIAN_HIEROGLYPHS:
    ಪಕ್ಷ HB_SCRIPT_JAVANESE:
    ಪಕ್ಷ HB_SCRIPT_KAITHI:
    ಪಕ್ಷ HB_SCRIPT_MEETEI_MAYEK:
    ಪಕ್ಷ HB_SCRIPT_TAI_THAM:
    ಪಕ್ಷ HB_SCRIPT_TAI_VIET:

    /* Unicode-6.0 additions */
    ಪಕ್ಷ HB_SCRIPT_BATAK:
    ಪಕ್ಷ HB_SCRIPT_BRAHMI:
    ಪಕ್ಷ HB_SCRIPT_MANDAIC:

    /* Unicode-6.1 additions */
    ಪಕ್ಷ HB_SCRIPT_CHAKMA:
    ಪಕ್ಷ HB_SCRIPT_MIAO:
    ಪಕ್ಷ HB_SCRIPT_SHARADA:
    ಪಕ್ಷ HB_SCRIPT_TAKRI:

    /* Unicode-7.0 additions */
    ಪಕ್ಷ HB_SCRIPT_DUPLOYAN:
    ಪಕ್ಷ HB_SCRIPT_GRANTHA:
    ಪಕ್ಷ HB_SCRIPT_KHOJKI:
    ಪಕ್ಷ HB_SCRIPT_KHUDAWADI:
    ಪಕ್ಷ HB_SCRIPT_MAHAJANI:
    ಪಕ್ಷ HB_SCRIPT_MANICHAEAN:
    ಪಕ್ಷ HB_SCRIPT_MODI:
    ಪಕ್ಷ HB_SCRIPT_PAHAWH_HMONG:
    ಪಕ್ಷ HB_SCRIPT_PSALTER_PAHLAVI:
    ಪಕ್ಷ HB_SCRIPT_SIDDHAM:
    ಪಕ್ಷ HB_SCRIPT_TIRHUTA:

    /* Unicode-8.0 additions */
    ಪಕ್ಷ HB_SCRIPT_AHOM:
    ಪಕ್ಷ HB_SCRIPT_MULTANI:

    /* Unicode-9.0 additions */
    ಪಕ್ಷ HB_SCRIPT_ADLAM:
    ಪಕ್ಷ HB_SCRIPT_BHAIKSUKI:
    ಪಕ್ಷ HB_SCRIPT_MARCHEN:
    ಪಕ್ಷ HB_SCRIPT_NEWA:

    /* Unicode-10.0 additions */
    ಪಕ್ಷ HB_SCRIPT_MASARAM_GONDI:
    ಪಕ್ಷ HB_SCRIPT_SOYOMBO:
    ಪಕ್ಷ HB_SCRIPT_ZANABAZAR_SQUARE:

    /* Unicode-11.0 additions */
    ಪಕ್ಷ HB_SCRIPT_DOGRA:
    ಪಕ್ಷ HB_SCRIPT_GUNJALA_GONDI:
    ಪಕ್ಷ HB_SCRIPT_HANIFI_ROHINGYA:
    ಪಕ್ಷ HB_SCRIPT_MAKASAR:
    ಪಕ್ಷ HB_SCRIPT_MEDEFAIDRIN:
    ಪಕ್ಷ HB_SCRIPT_OLD_SOGDIAN:
    ಪಕ್ಷ HB_SCRIPT_SOGDIAN:

    /* Unicode-12.0 additions */
    ಪಕ್ಷ HB_SCRIPT_ELYMAIC:
    ಪಕ್ಷ HB_SCRIPT_NANDINAGARI:
    ಪಕ್ಷ HB_SCRIPT_NYIAKENG_PUACHUE_HMONG:
    ಪಕ್ಷ HB_SCRIPT_WANCHO:

    /* Unicode-13.0 additions */
    ಪಕ್ಷ HB_SCRIPT_CHORASMIAN:
    ಪಕ್ಷ HB_SCRIPT_DIVES_AKURU:
    ಪಕ್ಷ HB_SCRIPT_KHITAN_SMALL_SCRIPT:
    ಪಕ್ಷ HB_SCRIPT_YEZIDI:

    /* Unicode-14.0 additions */
    ಪಕ್ಷ HB_SCRIPT_CYPRO_MINOAN:
    ಪಕ್ಷ HB_SCRIPT_OLD_UYGHUR:
    ಪಕ್ಷ HB_SCRIPT_TANGSA:
    ಪಕ್ಷ HB_SCRIPT_TOTO:
    ಪಕ್ಷ HB_SCRIPT_VITHKUQI:

    /* Unicode-15.0 additions */
    ಪಕ್ಷ HB_SCRIPT_KAWI:
    ಪಕ್ಷ HB_SCRIPT_NAG_MUNDARI:

    /* Unicode-16.0 additions */
    ಪಕ್ಷ HB_SCRIPT_GARAY:
    ಪಕ್ಷ HB_SCRIPT_GURUNG_KHEMA:
    ಪಕ್ಷ HB_SCRIPT_KIRAT_RAI:
    ಪಕ್ಷ HB_SCRIPT_OL_ONAL:
    ಪಕ್ಷ HB_SCRIPT_SUNUWAR:
    ಪಕ್ಷ HB_SCRIPT_TODHRI:
    ಪಕ್ಷ HB_SCRIPT_TULU_TIGALARI:

    /* Unicode-17.0 additions */
    ಪಕ್ಷ HB_SCRIPT_BERIA_ERFE:
    ಪಕ್ಷ HB_SCRIPT_SIDETIC:
    ಪಕ್ಷ HB_SCRIPT_TAI_YO:
    ಪಕ್ಷ HB_SCRIPT_TOLONG_SIKI:

    /* Unicode-18.0 additions */
    ಪಕ್ಷ HB_SCRIPT_JURCHEN:
    ಪಕ್ಷ HB_SCRIPT_PROTO_CUNEIFORM:
    ಪಕ್ಷ HB_SCRIPT_SEAL:

      /* If the designer designed the font for the 'DFLT' script,
       * (or we ended up arbitrarily pick 'latn'), use the default shaper.
       * Otherwise, use the specific shaper.
       * Note that for some simple scripts, there may not be *any*
       * GSUB/GPOS needed, so there may be no scripts found! */
      ಯದಿ (gsub_script == HB_TAG ('D','F','L','T') ||
	  gsub_script == HB_TAG ('l','a','t','n'))
	ಪ್ರತ್ಯಾಗಮ &_hb_ot_shaper_default;
      ಅನ್ಯಥಾ
	ಪ್ರತ್ಯಾಗಮ &_hb_ot_shaper_use;
  }
}


#endif /* HB_OT_SHAPER_HH */

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

#अगर_अघोषित HB_OT_SHAPER_HH
#घोषणा HB_OT_SHAPER_HH

#समावेश "hb.hh"

#समावेश "hb-ot-layout.hh"
#समावेश "hb-ot-shape.hh"
#समावेश "hb-ot-shape-normalize.hh"


/* buffer var allocations, used by all OT shapers */
#घोषणा ot_shaper_var_u8_category()	var2.u8[2]
#घोषणा ot_shaper_var_u8_auxiliary()	var2.u8[3]


#घोषणा HB_OT_SHAPE_MAX_COMBINING_MARKS 32

क्रमागत hb_ot_shape_zero_width_marks_type_t {
  HB_OT_SHAPE_ZERO_WIDTH_MARKS_NONE,
  HB_OT_SHAPE_ZERO_WIDTH_MARKS_BY_GDEF_EARLY,
  HB_OT_SHAPE_ZERO_WIDTH_MARKS_BY_GDEF_LATE
};


/* Master OT shaper list */
#घोषणा HB_OT_SHAPERS_IMPLEMENT_SHAPERS \
  HB_OT_SHAPER_IMPLEMENT (arabic) \
  HB_OT_SHAPER_IMPLEMENT (शेष) \
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


काष्ठा hb_ot_shaper_t
{
  /* collect_features()
   * Called during shape_plan().
   * Shapers should use plan->map to add their features and callbacks.
   * May be NULL.
   */
  व्योम (*collect_features) (hb_ot_shape_planner_t *plan);

  /* override_features()
   * Called during shape_plan().
   * Shapers should use plan->map to override features and add callbacks after
   * common features are added.
   * May be NULL.
   */
  व्योम (*override_features) (hb_ot_shape_planner_t *plan);


  /* data_create()
   * Called at the end of shape_plan().
   * Whatever shapers return will be accessible through plan->data later.
   * If nullptr is returned, means a plan failure.
   */
  व्योम *(*तथ्य_create) (स्थिर hb_ot_shape_plan_t *plan);

  /* data_destroy()
   * Called when the shape_plan is being destroyed.
   * plan->data is passed here for destruction.
   * If nullptr is returned, means a plan failure.
   * May be NULL.
   */
  व्योम (*तथ्य_destroy) (व्योम *तथ्य);


  /* preprocess_text()
   * Called during shape().
   * Shapers can use to modify text before shaping starts.
   * May be NULL.
   */
  व्योम (*preprocess_text) (स्थिर hb_ot_shape_plan_t *plan,
			   hb_बफर_t              *बफर,
			   hb_font_t                *font);

  /* postprocess_glyphs()
   * Called during shape().
   * Shapers can use to modify glyphs after shaping ends.
   * May be NULL.
   */
  व्योम (*postprocess_glyphs) (स्थिर hb_ot_shape_plan_t *plan,
			      hb_बफर_t              *बफर,
			      hb_font_t                *font);


  /* decompose()
   * Called during shape()'s normalization.
   * May be NULL.
   */
  bool (*decompose) (स्थिर hb_ot_shape_normalize_context_t *c,
		     hb_codepoint_t  ab,
		     hb_codepoint_t *a,
		     hb_codepoint_t *b);

  /* compose()
   * Called during shape()'s normalization.
   * May be NULL.
   */
  bool (*compose) (स्थिर hb_ot_shape_normalize_context_t *c,
		   hb_codepoint_t  a,
		   hb_codepoint_t  b,
		   hb_codepoint_t *ab);

  /* setup_masks()
   * Called during shape().
   * Shapers should use map to get feature masks and set on buffer.
   * Shapers may NOT modify characters.
   * May be NULL.
   */
  व्योम (*setup_masks) (स्थिर hb_ot_shape_plan_t *plan,
		       hb_बफर_t              *बफर,
		       hb_font_t                *font);

  /* reorder_marks()
   * Called during shape().
   * Shapers can use to modify ordering of combining marks.
   * May be NULL.
   */
  व्योम (*reorder_marks) (स्थिर hb_ot_shape_plan_t *plan,
			 hb_बफर_t              *बफर,
			 अचिन्हित पूर्णांक              start,
			 अचिन्हित पूर्णांक              end);

  /* gpos_tag()
   * If not HB_TAG_NONE, then must match found GPOS script tag for
   * GPOS to be applied.  Otherwise, fallback positioning will be used.
   */
  hb_tag_t gpos_tag;

  hb_ot_shape_normalization_mode_t normalization_preference;

  hb_ot_shape_zero_width_marks_type_t zero_width_marks;

  bool fallback_position;
};

#घोषणा HB_OT_SHAPER_IMPLEMENT(name) बाह्य HB_INTERNAL स्थिर hb_ot_shaper_t _hb_ot_shaper_##name;
HB_OT_SHAPERS_IMPLEMENT_SHAPERS
#अघोषित HB_OT_SHAPER_IMPLEMENT


अटल अंतरभूत स्थिर hb_ot_shaper_t *
hb_ot_shaper_categorize (hb_script_t script,
			 hb_direction_t direction,
			 hb_tag_t gsub_script)
{
  चयन ((hb_tag_t) script)
  {
    शेष:
      वापस &_hb_ot_shaper_default;


    /* Unicode-1.1 additions */
    हाल HB_SCRIPT_ARABIC:

    /* Unicode-3.0 additions */
    हाल HB_SCRIPT_SYRIAC:

      /* For Arabic script, use the Arabic shaper even if no OT script tag was found.
       * This is because we do fallback shaping for Arabic script (and not others).
       * But note that Arabic shaping is applicable only to horizontal layout; for
       * vertical text, just use the generic shaper instead. */
      अगर ((gsub_script != HB_OT_TAG_DEFAULT_SCRIPT || script == HB_SCRIPT_ARABIC) &&
	  HB_DIRECTION_IS_HORIZONTAL (direction))
	वापस &_hb_ot_shaper_arabic;
      अन्यथा
	वापस &_hb_ot_shaper_default;


    /* Unicode-1.1 additions */
    हाल HB_SCRIPT_THAI:
    हाल HB_SCRIPT_LAO:

      वापस &_hb_ot_shaper_thai;


    /* Unicode-1.1 additions */
    हाल HB_SCRIPT_HANGUL:

      वापस &_hb_ot_shaper_hangul;


    /* Unicode-1.1 additions */
    हाल HB_SCRIPT_HEBREW:

      वापस &_hb_ot_shaper_hebrew;


    /* Unicode-1.1 additions */
    हाल HB_SCRIPT_BENGALI:
    हाल HB_SCRIPT_DEVANAGARI:
    हाल HB_SCRIPT_GUJARATI:
    हाल HB_SCRIPT_GURMUKHI:
    हाल HB_SCRIPT_KANNADA:
    हाल HB_SCRIPT_MALAYALAM:
    हाल HB_SCRIPT_ORIYA:
    हाल HB_SCRIPT_TAMIL:
    हाल HB_SCRIPT_TELUGU:

      /* If the designer designed the font for the 'DFLT' script,
       * (or we ended up arbitrarily pick 'latn'), use the default shaper.
       * Otherwise, use the specific shaper.
       *
       * If it's indy3 tag, send to USE. */
      अगर (gsub_script == HB_TAG ('D','F','L','T') ||
	  gsub_script == HB_TAG ('l','a','t','n'))
	वापस &_hb_ot_shaper_default;
      अन्यथा अगर ((gsub_script & 0x000000FF) == '3')
	वापस &_hb_ot_shaper_use;
      अन्यथा
	वापस &_hb_ot_shaper_indic;

    हाल HB_SCRIPT_KHMER:
	वापस &_hb_ot_shaper_khmer;

    हाल HB_SCRIPT_MYANMAR:
      /* If the designer designed the font for the 'DFLT' script,
       * (or we ended up arbitrarily pick 'latn'), use the default shaper.
       * Otherwise, use the specific shaper.
       *
       * If designer designed for 'mymr' tag, also send to default
       * shaper.  That's tag used from before Myanmar shaping spec
       * was developed.  The shaping spec uses 'mym2' tag. */
      अगर (gsub_script == HB_TAG ('D','F','L','T') ||
	  gsub_script == HB_TAG ('l','a','t','n') ||
	  gsub_script == HB_TAG ('m','y','m','r'))
	वापस &_hb_ot_shaper_default;
      अन्यथा
	वापस &_hb_ot_shaper_myanmar;


#अगर_अघोषित HB_NO_OT_SHAPER_MYANMAR_ZAWGYI
#घोषणा HB_SCRIPT_MYANMAR_ZAWGYI	((hb_script_t) HB_TAG ('Q','a','a','g'))
    हाल HB_SCRIPT_MYANMAR_ZAWGYI:
    /* https://github.com/harfbuzz/harfbuzz/issues/1162 */

      वापस &_hb_ot_shaper_myanmar_zawgyi;
#पूर्ण_अगर


    /* Unicode-2.0 additions */
    हाल HB_SCRIPT_TIBETAN:

    /* Unicode-3.0 additions */
    हाल HB_SCRIPT_MONGOLIAN:
    हाल HB_SCRIPT_SINHALA:

    /* Unicode-3.2 additions */
    हाल HB_SCRIPT_BUHID:
    हाल HB_SCRIPT_HANUNOO:
    हाल HB_SCRIPT_TAGALOG:
    हाल HB_SCRIPT_TAGBANWA:

    /* Unicode-4.0 additions */
    हाल HB_SCRIPT_LIMBU:
    हाल HB_SCRIPT_TAI_LE:

    /* Unicode-4.1 additions */
    हाल HB_SCRIPT_BUGINESE:
    हाल HB_SCRIPT_KHAROSHTHI:
    हाल HB_SCRIPT_SYLOTI_NAGRI:
    हाल HB_SCRIPT_TIFINAGH:

    /* Unicode-5.0 additions */
    हाल HB_SCRIPT_BALINESE:
    हाल HB_SCRIPT_NKO:
    हाल HB_SCRIPT_PHAGS_PA:

    /* Unicode-5.1 additions */
    हाल HB_SCRIPT_CHAM:
    हाल HB_SCRIPT_KAYAH_LI:
    हाल HB_SCRIPT_LEPCHA:
    हाल HB_SCRIPT_REJANG:
    हाल HB_SCRIPT_SAURASHTRA:
    हाल HB_SCRIPT_SUNDANESE:

    /* Unicode-5.2 additions */
    हाल HB_SCRIPT_EGYPTIAN_HIEROGLYPHS:
    हाल HB_SCRIPT_JAVANESE:
    हाल HB_SCRIPT_KAITHI:
    हाल HB_SCRIPT_MEETEI_MAYEK:
    हाल HB_SCRIPT_TAI_THAM:
    हाल HB_SCRIPT_TAI_VIET:

    /* Unicode-6.0 additions */
    हाल HB_SCRIPT_BATAK:
    हाल HB_SCRIPT_BRAHMI:
    हाल HB_SCRIPT_MANDAIC:

    /* Unicode-6.1 additions */
    हाल HB_SCRIPT_CHAKMA:
    हाल HB_SCRIPT_MIAO:
    हाल HB_SCRIPT_SHARADA:
    हाल HB_SCRIPT_TAKRI:

    /* Unicode-7.0 additions */
    हाल HB_SCRIPT_DUPLOYAN:
    हाल HB_SCRIPT_GRANTHA:
    हाल HB_SCRIPT_KHOJKI:
    हाल HB_SCRIPT_KHUDAWADI:
    हाल HB_SCRIPT_MAHAJANI:
    हाल HB_SCRIPT_MANICHAEAN:
    हाल HB_SCRIPT_MODI:
    हाल HB_SCRIPT_PAHAWH_HMONG:
    हाल HB_SCRIPT_PSALTER_PAHLAVI:
    हाल HB_SCRIPT_SIDDHAM:
    हाल HB_SCRIPT_TIRHUTA:

    /* Unicode-8.0 additions */
    हाल HB_SCRIPT_AHOM:
    हाल HB_SCRIPT_MULTANI:

    /* Unicode-9.0 additions */
    हाल HB_SCRIPT_ADLAM:
    हाल HB_SCRIPT_BHAIKSUKI:
    हाल HB_SCRIPT_MARCHEN:
    हाल HB_SCRIPT_NEWA:

    /* Unicode-10.0 additions */
    हाल HB_SCRIPT_MASARAM_GONDI:
    हाल HB_SCRIPT_SOYOMBO:
    हाल HB_SCRIPT_ZANABAZAR_SQUARE:

    /* Unicode-11.0 additions */
    हाल HB_SCRIPT_DOGRA:
    हाल HB_SCRIPT_GUNJALA_GONDI:
    हाल HB_SCRIPT_HANIFI_ROHINGYA:
    हाल HB_SCRIPT_MAKASAR:
    हाल HB_SCRIPT_MEDEFAIDRIN:
    हाल HB_SCRIPT_OLD_SOGDIAN:
    हाल HB_SCRIPT_SOGDIAN:

    /* Unicode-12.0 additions */
    हाल HB_SCRIPT_ELYMAIC:
    हाल HB_SCRIPT_NANDINAGARI:
    हाल HB_SCRIPT_NYIAKENG_PUACHUE_HMONG:
    हाल HB_SCRIPT_WANCHO:

    /* Unicode-13.0 additions */
    हाल HB_SCRIPT_CHORASMIAN:
    हाल HB_SCRIPT_DIVES_AKURU:
    हाल HB_SCRIPT_KHITAN_SMALL_SCRIPT:
    हाल HB_SCRIPT_YEZIDI:

    /* Unicode-14.0 additions */
    हाल HB_SCRIPT_CYPRO_MINOAN:
    हाल HB_SCRIPT_OLD_UYGHUR:
    हाल HB_SCRIPT_TANGSA:
    हाल HB_SCRIPT_TOTO:
    हाल HB_SCRIPT_VITHKUQI:

    /* Unicode-15.0 additions */
    हाल HB_SCRIPT_KAWI:
    हाल HB_SCRIPT_NAG_MUNDARI:

    /* Unicode-16.0 additions */
    हाल HB_SCRIPT_GARAY:
    हाल HB_SCRIPT_GURUNG_KHEMA:
    हाल HB_SCRIPT_KIRAT_RAI:
    हाल HB_SCRIPT_OL_ONAL:
    हाल HB_SCRIPT_SUNUWAR:
    हाल HB_SCRIPT_TODHRI:
    हाल HB_SCRIPT_TULU_TIGALARI:

    /* Unicode-17.0 additions */
    हाल HB_SCRIPT_BERIA_ERFE:
    हाल HB_SCRIPT_SIDETIC:
    हाल HB_SCRIPT_TAI_YO:
    हाल HB_SCRIPT_TOLONG_SIKI:

    /* Unicode-18.0 additions */
    हाल HB_SCRIPT_JURCHEN:
    हाल HB_SCRIPT_PROTO_CUNEIFORM:
    हाल HB_SCRIPT_SEAL:

      /* If the designer designed the font for the 'DFLT' script,
       * (or we ended up arbitrarily pick 'latn'), use the default shaper.
       * Otherwise, use the specific shaper.
       * Note that for some simple scripts, there may not be *any*
       * GSUB/GPOS needed, so there may be no scripts found! */
      अगर (gsub_script == HB_TAG ('D','F','L','T') ||
	  gsub_script == HB_TAG ('l','a','t','n'))
	वापस &_hb_ot_shaper_default;
      अन्यथा
	वापस &_hb_ot_shaper_use;
  }
}


#पूर्ण_अगर /* HB_OT_SHAPER_HH */

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
  HB_OT_SHAPER_IMPLEMENT (पूर्वनिर्धारित) \
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
  शून्य *(*data_create) (स्थिरांक hb_ot_shape_plan_t *plan);

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
  शून्य (*preprocess_text) (स्थिरांक hb_ot_shape_plan_t *plan,
			   hb_buffer_t              *buffer,
			   hb_font_t                *font);

  /* postprocess_glyphs()
   * Called during shape().
   * Shapers can use to modify glyphs after shaping ends.
   * May be NULL.
   */
  शून्य (*postprocess_glyphs) (स्थिरांक hb_ot_shape_plan_t *plan,
			      hb_buffer_t              *buffer,
			      hb_font_t                *font);


  /* decompose()
   * Called during shape()'s normalization.
   * May be NULL.
   */
  तार्किक (*decompose) (स्थिरांक hb_ot_shape_normalize_context_t *c,
		     hb_codepoint_t  ab,
		     hb_codepoint_t *a,
		     hb_codepoint_t *b);

  /* compose()
   * Called during shape()'s normalization.
   * May be NULL.
   */
  तार्किक (*compose) (स्थिरांक hb_ot_shape_normalize_context_t *c,
		   hb_codepoint_t  a,
		   hb_codepoint_t  b,
		   hb_codepoint_t *ab);

  /* setup_masks()
   * Called during shape().
   * Shapers should use map to get feature masks and set on buffer.
   * Shapers may NOT modify characters.
   * May be NULL.
   */
  शून्य (*setup_masks) (स्थिरांक hb_ot_shape_plan_t *plan,
		       hb_buffer_t              *buffer,
		       hb_font_t                *font);

  /* reorder_marks()
   * Called during shape().
   * Shapers can use to modify ordering of combining marks.
   * May be NULL.
   */
  शून्य (*reorder_marks) (स्थिरांक hb_ot_shape_plan_t *plan,
			 hb_buffer_t              *buffer,
			 अचिन्हित पूर्णांक              start,
			 अचिन्हित पूर्णांक              end);

  /* gpos_tag()
   * If not HB_TAG_NONE, then must match found GPOS script tag for
   * GPOS to be applied.  Otherwise, fallback positioning will be used.
   */
  hb_tag_t gpos_tag;

  hb_ot_shape_normalization_mode_t normalization_preference;

  hb_ot_shape_zero_width_marks_type_t zero_width_marks;

  तार्किक fallback_position;
};

#define HB_OT_SHAPER_IMPLEMENT(name) बाह्य HB_INTERNAL स्थिरांक hb_ot_shaper_t _hb_ot_shaper_##name;
HB_OT_SHAPERS_IMPLEMENT_SHAPERS
#undef HB_OT_SHAPER_IMPLEMENT


स्थिर ओळीत स्थिरांक hb_ot_shaper_t *
hb_ot_shaper_categorize (hb_script_t script,
			 hb_direction_t direction,
			 hb_tag_t gsub_script)
{
  निवड ((hb_tag_t) script)
  {
    पूर्वनिर्धारित:
      परत &_hb_ot_shaper_default;


    /* Unicode-1.1 additions */
    स्थिती HB_SCRIPT_ARABIC:

    /* Unicode-3.0 additions */
    स्थिती HB_SCRIPT_SYRIAC:

      /* For Arabic script, use the Arabic shaper even if no OT script tag was found.
       * This is because we do fallback shaping for Arabic script (and not others).
       * But note that Arabic shaping is applicable only to horizontal layout; for
       * vertical text, just use the generic shaper instead. */
      जर ((gsub_script != HB_OT_TAG_DEFAULT_SCRIPT || script == HB_SCRIPT_ARABIC) &&
	  HB_DIRECTION_IS_HORIZONTAL (direction))
	परत &_hb_ot_shaper_arabic;
      अन्यथा
	परत &_hb_ot_shaper_default;


    /* Unicode-1.1 additions */
    स्थिती HB_SCRIPT_THAI:
    स्थिती HB_SCRIPT_LAO:

      परत &_hb_ot_shaper_thai;


    /* Unicode-1.1 additions */
    स्थिती HB_SCRIPT_HANGUL:

      परत &_hb_ot_shaper_hangul;


    /* Unicode-1.1 additions */
    स्थिती HB_SCRIPT_HEBREW:

      परत &_hb_ot_shaper_hebrew;


    /* Unicode-1.1 additions */
    स्थिती HB_SCRIPT_BENGALI:
    स्थिती HB_SCRIPT_DEVANAGARI:
    स्थिती HB_SCRIPT_GUJARATI:
    स्थिती HB_SCRIPT_GURMUKHI:
    स्थिती HB_SCRIPT_KANNADA:
    स्थिती HB_SCRIPT_MALAYALAM:
    स्थिती HB_SCRIPT_ORIYA:
    स्थिती HB_SCRIPT_TAMIL:
    स्थिती HB_SCRIPT_TELUGU:

      /* If the designer designed the font for the 'DFLT' script,
       * (or we ended up arbitrarily pick 'latn'), use the default shaper.
       * Otherwise, use the specific shaper.
       *
       * If it's indy3 tag, send to USE. */
      जर (gsub_script == HB_TAG ('D','F','L','T') ||
	  gsub_script == HB_TAG ('l','a','t','n'))
	परत &_hb_ot_shaper_default;
      अन्यथा जर ((gsub_script & 0x000000FF) == '3')
	परत &_hb_ot_shaper_use;
      अन्यथा
	परत &_hb_ot_shaper_indic;

    स्थिती HB_SCRIPT_KHMER:
	परत &_hb_ot_shaper_khmer;

    स्थिती HB_SCRIPT_MYANMAR:
      /* If the designer designed the font for the 'DFLT' script,
       * (or we ended up arbitrarily pick 'latn'), use the default shaper.
       * Otherwise, use the specific shaper.
       *
       * If designer designed for 'mymr' tag, also send to default
       * shaper.  That's tag used from before Myanmar shaping spec
       * was developed.  The shaping spec uses 'mym2' tag. */
      जर (gsub_script == HB_TAG ('D','F','L','T') ||
	  gsub_script == HB_TAG ('l','a','t','n') ||
	  gsub_script == HB_TAG ('m','y','m','r'))
	परत &_hb_ot_shaper_default;
      अन्यथा
	परत &_hb_ot_shaper_myanmar;


#ifndef HB_NO_OT_SHAPER_MYANMAR_ZAWGYI
#define HB_SCRIPT_MYANMAR_ZAWGYI	((hb_script_t) HB_TAG ('Q','a','a','g'))
    स्थिती HB_SCRIPT_MYANMAR_ZAWGYI:
    /* https://github.com/harfbuzz/harfbuzz/issues/1162 */

      परत &_hb_ot_shaper_myanmar_zawgyi;
#endif


    /* Unicode-2.0 additions */
    स्थिती HB_SCRIPT_TIBETAN:

    /* Unicode-3.0 additions */
    स्थिती HB_SCRIPT_MONGOLIAN:
    स्थिती HB_SCRIPT_SINHALA:

    /* Unicode-3.2 additions */
    स्थिती HB_SCRIPT_BUHID:
    स्थिती HB_SCRIPT_HANUNOO:
    स्थिती HB_SCRIPT_TAGALOG:
    स्थिती HB_SCRIPT_TAGBANWA:

    /* Unicode-4.0 additions */
    स्थिती HB_SCRIPT_LIMBU:
    स्थिती HB_SCRIPT_TAI_LE:

    /* Unicode-4.1 additions */
    स्थिती HB_SCRIPT_BUGINESE:
    स्थिती HB_SCRIPT_KHAROSHTHI:
    स्थिती HB_SCRIPT_SYLOTI_NAGRI:
    स्थिती HB_SCRIPT_TIFINAGH:

    /* Unicode-5.0 additions */
    स्थिती HB_SCRIPT_BALINESE:
    स्थिती HB_SCRIPT_NKO:
    स्थिती HB_SCRIPT_PHAGS_PA:

    /* Unicode-5.1 additions */
    स्थिती HB_SCRIPT_CHAM:
    स्थिती HB_SCRIPT_KAYAH_LI:
    स्थिती HB_SCRIPT_LEPCHA:
    स्थिती HB_SCRIPT_REJANG:
    स्थिती HB_SCRIPT_SAURASHTRA:
    स्थिती HB_SCRIPT_SUNDANESE:

    /* Unicode-5.2 additions */
    स्थिती HB_SCRIPT_EGYPTIAN_HIEROGLYPHS:
    स्थिती HB_SCRIPT_JAVANESE:
    स्थिती HB_SCRIPT_KAITHI:
    स्थिती HB_SCRIPT_MEETEI_MAYEK:
    स्थिती HB_SCRIPT_TAI_THAM:
    स्थिती HB_SCRIPT_TAI_VIET:

    /* Unicode-6.0 additions */
    स्थिती HB_SCRIPT_BATAK:
    स्थिती HB_SCRIPT_BRAHMI:
    स्थिती HB_SCRIPT_MANDAIC:

    /* Unicode-6.1 additions */
    स्थिती HB_SCRIPT_CHAKMA:
    स्थिती HB_SCRIPT_MIAO:
    स्थिती HB_SCRIPT_SHARADA:
    स्थिती HB_SCRIPT_TAKRI:

    /* Unicode-7.0 additions */
    स्थिती HB_SCRIPT_DUPLOYAN:
    स्थिती HB_SCRIPT_GRANTHA:
    स्थिती HB_SCRIPT_KHOJKI:
    स्थिती HB_SCRIPT_KHUDAWADI:
    स्थिती HB_SCRIPT_MAHAJANI:
    स्थिती HB_SCRIPT_MANICHAEAN:
    स्थिती HB_SCRIPT_MODI:
    स्थिती HB_SCRIPT_PAHAWH_HMONG:
    स्थिती HB_SCRIPT_PSALTER_PAHLAVI:
    स्थिती HB_SCRIPT_SIDDHAM:
    स्थिती HB_SCRIPT_TIRHUTA:

    /* Unicode-8.0 additions */
    स्थिती HB_SCRIPT_AHOM:
    स्थिती HB_SCRIPT_MULTANI:

    /* Unicode-9.0 additions */
    स्थिती HB_SCRIPT_ADLAM:
    स्थिती HB_SCRIPT_BHAIKSUKI:
    स्थिती HB_SCRIPT_MARCHEN:
    स्थिती HB_SCRIPT_NEWA:

    /* Unicode-10.0 additions */
    स्थिती HB_SCRIPT_MASARAM_GONDI:
    स्थिती HB_SCRIPT_SOYOMBO:
    स्थिती HB_SCRIPT_ZANABAZAR_SQUARE:

    /* Unicode-11.0 additions */
    स्थिती HB_SCRIPT_DOGRA:
    स्थिती HB_SCRIPT_GUNJALA_GONDI:
    स्थिती HB_SCRIPT_HANIFI_ROHINGYA:
    स्थिती HB_SCRIPT_MAKASAR:
    स्थिती HB_SCRIPT_MEDEFAIDRIN:
    स्थिती HB_SCRIPT_OLD_SOGDIAN:
    स्थिती HB_SCRIPT_SOGDIAN:

    /* Unicode-12.0 additions */
    स्थिती HB_SCRIPT_ELYMAIC:
    स्थिती HB_SCRIPT_NANDINAGARI:
    स्थिती HB_SCRIPT_NYIAKENG_PUACHUE_HMONG:
    स्थिती HB_SCRIPT_WANCHO:

    /* Unicode-13.0 additions */
    स्थिती HB_SCRIPT_CHORASMIAN:
    स्थिती HB_SCRIPT_DIVES_AKURU:
    स्थिती HB_SCRIPT_KHITAN_SMALL_SCRIPT:
    स्थिती HB_SCRIPT_YEZIDI:

    /* Unicode-14.0 additions */
    स्थिती HB_SCRIPT_CYPRO_MINOAN:
    स्थिती HB_SCRIPT_OLD_UYGHUR:
    स्थिती HB_SCRIPT_TANGSA:
    स्थिती HB_SCRIPT_TOTO:
    स्थिती HB_SCRIPT_VITHKUQI:

    /* Unicode-15.0 additions */
    स्थिती HB_SCRIPT_KAWI:
    स्थिती HB_SCRIPT_NAG_MUNDARI:

    /* Unicode-16.0 additions */
    स्थिती HB_SCRIPT_GARAY:
    स्थिती HB_SCRIPT_GURUNG_KHEMA:
    स्थिती HB_SCRIPT_KIRAT_RAI:
    स्थिती HB_SCRIPT_OL_ONAL:
    स्थिती HB_SCRIPT_SUNUWAR:
    स्थिती HB_SCRIPT_TODHRI:
    स्थिती HB_SCRIPT_TULU_TIGALARI:

    /* Unicode-17.0 additions */
    स्थिती HB_SCRIPT_BERIA_ERFE:
    स्थिती HB_SCRIPT_SIDETIC:
    स्थिती HB_SCRIPT_TAI_YO:
    स्थिती HB_SCRIPT_TOLONG_SIKI:

    /* Unicode-18.0 additions */
    स्थिती HB_SCRIPT_JURCHEN:
    स्थिती HB_SCRIPT_PROTO_CUNEIFORM:
    स्थिती HB_SCRIPT_SEAL:

      /* If the designer designed the font for the 'DFLT' script,
       * (or we ended up arbitrarily pick 'latn'), use the default shaper.
       * Otherwise, use the specific shaper.
       * Note that for some simple scripts, there may not be *any*
       * GSUB/GPOS needed, so there may be no scripts found! */
      जर (gsub_script == HB_TAG ('D','F','L','T') ||
	  gsub_script == HB_TAG ('l','a','t','n'))
	परत &_hb_ot_shaper_default;
      अन्यथा
	परत &_hb_ot_shaper_use;
  }
}


#endif /* HB_OT_SHAPER_HH */

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

#که_نه_تعریف_شوی HB_OT_SHAPER_HH
#تعریف_کړه HB_OT_SHAPER_HH

#ضمن "hb.hh"

#ضمن "hb-ot-layout.hh"
#ضمن "hb-ot-shape.hh"
#ضمن "hb-ot-shape-normalize.hh"


/* buffer var allocations, used by all OT shapers */
#تعریف_کړه ot_shaper_var_u8_category()	var2.u8[2]
#تعریف_کړه ot_shaper_var_u8_auxiliary()	var2.u8[3]


#تعریف_کړه HB_OT_SHAPE_MAX_COMBINING_MARKS 32

تعداد hb_ot_shape_zero_پلنوالی_marks_type_t {
  HB_OT_SHAPE_ZERO_پلنوالی_MARKS_NONE,
  HB_OT_SHAPE_ZERO_پلنوالی_MARKS_BY_GDEF_EARLY,
  HB_OT_SHAPE_ZERO_پلنوالی_MARKS_BY_GDEF_LATE
};


/* Master OT shaper list */
#تعریف_کړه HB_OT_SHAPERS_IMPLEMENT_SHAPERS \
  HB_OT_SHAPER_IMPLEMENT (arabic) \
  HB_OT_SHAPER_IMPLEMENT (افتراضي) \
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


بنية hb_ot_shaper_t
{
  /* collect_features()
   * Called during shape_plan().
   * Shapers should use plan->map to add their features and callbacks.
   * May be NULL.
   */
  فراغ (*collect_features) (hb_ot_shape_planner_t *plan);

  /* override_features()
   * Called during shape_plan().
   * Shapers should use plan->map to override features and add callbacks after
   * common features are added.
   * May be NULL.
   */
  فراغ (*override_features) (hb_ot_shape_planner_t *plan);


  /* data_create()
   * Called at the end of shape_plan().
   * Whatever shapers return will be accessible through plan->data later.
   * If nullptr is returned, means a plan failure.
   */
  فراغ *(*معلومات_create) (ثابت hb_ot_shape_plan_t *plan);

  /* data_destroy()
   * Called when the shape_plan is being destroyed.
   * plan->data is passed here for destruction.
   * If nullptr is returned, means a plan failure.
   * May be NULL.
   */
  فراغ (*معلومات_destroy) (فراغ *معلومات);


  /* preprocess_text()
   * Called during shape().
   * Shapers can use to modify text before shaping starts.
   * May be NULL.
   */
  فراغ (*preprocess_متن) (ثابت hb_ot_shape_plan_t *plan,
			   hb_بفر_t              *بفر,
			   hb_font_t                *font);

  /* postprocess_glyphs()
   * Called during shape().
   * Shapers can use to modify glyphs after shaping ends.
   * May be NULL.
   */
  فراغ (*postprocess_glyphs) (ثابت hb_ot_shape_plan_t *plan,
			      hb_بفر_t              *بفر,
			      hb_font_t                *font);


  /* decompose()
   * Called during shape()'s normalization.
   * May be NULL.
   */
  منطقي (*decompose) (ثابت hb_ot_shape_normalize_context_t *c,
		     hb_codepoint_t  ab,
		     hb_codepoint_t *a,
		     hb_codepoint_t *b);

  /* compose()
   * Called during shape()'s normalization.
   * May be NULL.
   */
  منطقي (*compose) (ثابت hb_ot_shape_normalize_context_t *c,
		   hb_codepoint_t  a,
		   hb_codepoint_t  b,
		   hb_codepoint_t *ab);

  /* setup_masks()
   * Called during shape().
   * Shapers should use map to get feature masks and set on buffer.
   * Shapers may NOT modify characters.
   * May be NULL.
   */
  فراغ (*تیاری_masks) (ثابت hb_ot_shape_plan_t *plan,
		       hb_بفر_t              *بفر,
		       hb_font_t                *font);

  /* reorder_marks()
   * Called during shape().
   * Shapers can use to modify ordering of combining marks.
   * May be NULL.
   */
  فراغ (*reorder_marks) (ثابت hb_ot_shape_plan_t *plan,
			 hb_بفر_t              *بفر,
			 بینښه صحيح              پیل,
			 بینښه صحيح              پای_ټکی);

  /* gpos_tag()
   * If not HB_TAG_NONE, then must match found GPOS script tag for
   * GPOS to be applied.  Otherwise, fallback positioning will be used.
   */
  hb_نښان_t gpos_نښان;

  hb_ot_shape_normalization_ډېرپېښ_t normalization_preference;

  hb_ot_shape_zero_پلنوالی_marks_type_t zero_پلنوالی_marks;

  منطقي fallback_position;
};

#تعریف_کړه HB_OT_SHAPER_IMPLEMENT(name) خارجي HB_دننی ثابت hb_ot_shaper_t _hb_ot_shaper_##name;
HB_OT_SHAPERS_IMPLEMENT_SHAPERS
#تعریف_لغوه HB_OT_SHAPER_IMPLEMENT


ساكن مضمن ثابت hb_ot_shaper_t *
hb_ot_shaper_categorize (hb_script_t script,
			 hb_direction_t direction,
			 hb_نښان_t gsub_script)
{
  بدل ((hb_نښان_t) script)
  {
    افتراضي:
      أرجع &_hb_ot_shaper_default;


    /* Unicode-1.1 additions */
    حالة HB_SCRIPT_ARABIC:

    /* Unicode-3.0 additions */
    حالة HB_SCRIPT_SYRIAC:

      /* For Arabic script, use the Arabic shaper even if no OT script tag was found.
       * This is because we do fallback shaping for Arabic script (and not others).
       * But note that Arabic shaping is applicable only to horizontal layout; for
       * vertical text, just use the generic shaper instead. */
      إذا ((gsub_script != HB_OT_نښان_DEFAULT_SCRIPT || script == HB_SCRIPT_ARABIC) &&
	  HB_DIRECTION_IS_HORIZONTAL (direction))
	أرجع &_hb_ot_shaper_arabic;
      وإلا
	أرجع &_hb_ot_shaper_default;


    /* Unicode-1.1 additions */
    حالة HB_SCRIPT_THAI:
    حالة HB_SCRIPT_LAO:

      أرجع &_hb_ot_shaper_thai;


    /* Unicode-1.1 additions */
    حالة HB_SCRIPT_HANGUL:

      أرجع &_hb_ot_shaper_hangul;


    /* Unicode-1.1 additions */
    حالة HB_SCRIPT_HEBREW:

      أرجع &_hb_ot_shaper_hebrew;


    /* Unicode-1.1 additions */
    حالة HB_SCRIPT_BENGALI:
    حالة HB_SCRIPT_DEVANAGARI:
    حالة HB_SCRIPT_GUJARATI:
    حالة HB_SCRIPT_GURMUKHI:
    حالة HB_SCRIPT_KANNADA:
    حالة HB_SCRIPT_MALAYALAM:
    حالة HB_SCRIPT_ORIYA:
    حالة HB_SCRIPT_TAMIL:
    حالة HB_SCRIPT_TELUGU:

      /* If the designer designed the font for the 'DFLT' script,
       * (or we ended up arbitrarily pick 'latn'), use the default shaper.
       * Otherwise, use the specific shaper.
       *
       * If it's indy3 tag, send to USE. */
      إذا (gsub_script == HB_نښان ('D','F','L','T') ||
	  gsub_script == HB_نښان ('l','a','t','n'))
	أرجع &_hb_ot_shaper_default;
      وإلا إذا ((gsub_script & 0x000000FF) == '3')
	أرجع &_hb_ot_shaper_use;
      وإلا
	أرجع &_hb_ot_shaper_indic;

    حالة HB_SCRIPT_KHMER:
	أرجع &_hb_ot_shaper_khmer;

    حالة HB_SCRIPT_MYANMAR:
      /* If the designer designed the font for the 'DFLT' script,
       * (or we ended up arbitrarily pick 'latn'), use the default shaper.
       * Otherwise, use the specific shaper.
       *
       * If designer designed for 'mymr' tag, also send to default
       * shaper.  That's tag used from before Myanmar shaping spec
       * was developed.  The shaping spec uses 'mym2' tag. */
      إذا (gsub_script == HB_نښان ('D','F','L','T') ||
	  gsub_script == HB_نښان ('l','a','t','n') ||
	  gsub_script == HB_نښان ('m','y','m','r'))
	أرجع &_hb_ot_shaper_default;
      وإلا
	أرجع &_hb_ot_shaper_myanmar;


#که_نه_تعریف_شوی HB_NO_OT_SHAPER_MYANMAR_ZAWGYI
#تعریف_کړه HB_SCRIPT_MYANMAR_ZAWGYI	((hb_script_t) HB_نښان ('Q','a','a','g'))
    حالة HB_SCRIPT_MYANMAR_ZAWGYI:
    /* https://github.com/harfbuzz/harfbuzz/issues/1162 */

      أرجع &_hb_ot_shaper_myanmar_zawgyi;
#د_که_پای


    /* Unicode-2.0 additions */
    حالة HB_SCRIPT_TIBETAN:

    /* Unicode-3.0 additions */
    حالة HB_SCRIPT_MONGOLIAN:
    حالة HB_SCRIPT_SINHALA:

    /* Unicode-3.2 additions */
    حالة HB_SCRIPT_BUHID:
    حالة HB_SCRIPT_HANUNOO:
    حالة HB_SCRIPT_TAGALOG:
    حالة HB_SCRIPT_TAGBANWA:

    /* Unicode-4.0 additions */
    حالة HB_SCRIPT_LIMBU:
    حالة HB_SCRIPT_TAI_LE:

    /* Unicode-4.1 additions */
    حالة HB_SCRIPT_BUGINESE:
    حالة HB_SCRIPT_KHAROSHTHI:
    حالة HB_SCRIPT_SYLOTI_NAGRI:
    حالة HB_SCRIPT_TIFINAGH:

    /* Unicode-5.0 additions */
    حالة HB_SCRIPT_BALINESE:
    حالة HB_SCRIPT_NKO:
    حالة HB_SCRIPT_PHAGS_PA:

    /* Unicode-5.1 additions */
    حالة HB_SCRIPT_CHAM:
    حالة HB_SCRIPT_KAYAH_LI:
    حالة HB_SCRIPT_LEPCHA:
    حالة HB_SCRIPT_REJANG:
    حالة HB_SCRIPT_SAURASHTRA:
    حالة HB_SCRIPT_SUNDANESE:

    /* Unicode-5.2 additions */
    حالة HB_SCRIPT_EGYPTIAN_HIEROGLYPHS:
    حالة HB_SCRIPT_JAVANESE:
    حالة HB_SCRIPT_KAITHI:
    حالة HB_SCRIPT_MEETEI_MAYEK:
    حالة HB_SCRIPT_TAI_THAM:
    حالة HB_SCRIPT_TAI_VIET:

    /* Unicode-6.0 additions */
    حالة HB_SCRIPT_BATAK:
    حالة HB_SCRIPT_BRAHMI:
    حالة HB_SCRIPT_MANDAIC:

    /* Unicode-6.1 additions */
    حالة HB_SCRIPT_CHAKMA:
    حالة HB_SCRIPT_MIAO:
    حالة HB_SCRIPT_SHARADA:
    حالة HB_SCRIPT_TAKRI:

    /* Unicode-7.0 additions */
    حالة HB_SCRIPT_DUPLOYAN:
    حالة HB_SCRIPT_GRANTHA:
    حالة HB_SCRIPT_KHOJKI:
    حالة HB_SCRIPT_KHUDAWADI:
    حالة HB_SCRIPT_MAHAJANI:
    حالة HB_SCRIPT_MANICHAEAN:
    حالة HB_SCRIPT_MODI:
    حالة HB_SCRIPT_PAHAWH_HMONG:
    حالة HB_SCRIPT_PSALTER_PAHLAVI:
    حالة HB_SCRIPT_SIDDHAM:
    حالة HB_SCRIPT_TIRHUTA:

    /* Unicode-8.0 additions */
    حالة HB_SCRIPT_AHOM:
    حالة HB_SCRIPT_MULTANI:

    /* Unicode-9.0 additions */
    حالة HB_SCRIPT_ADLAM:
    حالة HB_SCRIPT_BHAIKSUKI:
    حالة HB_SCRIPT_MARCHEN:
    حالة HB_SCRIPT_NEWA:

    /* Unicode-10.0 additions */
    حالة HB_SCRIPT_MASARAM_GONDI:
    حالة HB_SCRIPT_SOYOMBO:
    حالة HB_SCRIPT_ZANABAZAR_مربع:

    /* Unicode-11.0 additions */
    حالة HB_SCRIPT_DOGRA:
    حالة HB_SCRIPT_GUNJALA_GONDI:
    حالة HB_SCRIPT_HANIFI_ROHINGYA:
    حالة HB_SCRIPT_MAKASAR:
    حالة HB_SCRIPT_MEDEFAIDRIN:
    حالة HB_SCRIPT_OLD_SOGDIAN:
    حالة HB_SCRIPT_SOGDIAN:

    /* Unicode-12.0 additions */
    حالة HB_SCRIPT_ELYMAIC:
    حالة HB_SCRIPT_NANDINAGARI:
    حالة HB_SCRIPT_NYIAKENG_PUACHUE_HMONG:
    حالة HB_SCRIPT_WANCHO:

    /* Unicode-13.0 additions */
    حالة HB_SCRIPT_CHORASMIAN:
    حالة HB_SCRIPT_DIVES_AKURU:
    حالة HB_SCRIPT_KHITAN_SMALL_SCRIPT:
    حالة HB_SCRIPT_YEZIDI:

    /* Unicode-14.0 additions */
    حالة HB_SCRIPT_CYPRO_MINOAN:
    حالة HB_SCRIPT_OLD_UYGHUR:
    حالة HB_SCRIPT_TANGSA:
    حالة HB_SCRIPT_TOTO:
    حالة HB_SCRIPT_VITHKUQI:

    /* Unicode-15.0 additions */
    حالة HB_SCRIPT_KAWI:
    حالة HB_SCRIPT_NAG_MUNDARI:

    /* Unicode-16.0 additions */
    حالة HB_SCRIPT_GARAY:
    حالة HB_SCRIPT_GURUNG_KHEMA:
    حالة HB_SCRIPT_KIRAT_RAI:
    حالة HB_SCRIPT_OL_ONAL:
    حالة HB_SCRIPT_SUNUWAR:
    حالة HB_SCRIPT_TODHRI:
    حالة HB_SCRIPT_TULU_TIGALARI:

    /* Unicode-17.0 additions */
    حالة HB_SCRIPT_BERIA_ERFE:
    حالة HB_SCRIPT_SIDETIC:
    حالة HB_SCRIPT_TAI_YO:
    حالة HB_SCRIPT_TOLONG_SIKI:

    /* Unicode-18.0 additions */
    حالة HB_SCRIPT_JURCHEN:
    حالة HB_SCRIPT_PROTO_CUNEIFORM:
    حالة HB_SCRIPT_SEAL:

      /* If the designer designed the font for the 'DFLT' script,
       * (or we ended up arbitrarily pick 'latn'), use the default shaper.
       * Otherwise, use the specific shaper.
       * Note that for some simple scripts, there may not be *any*
       * GSUB/GPOS needed, so there may be no scripts found! */
      إذا (gsub_script == HB_نښان ('D','F','L','T') ||
	  gsub_script == HB_نښان ('l','a','t','n'))
	أرجع &_hb_ot_shaper_default;
      وإلا
	أرجع &_hb_ot_shaper_use;
  }
}


#د_که_پای /* HB_OT_SHAPER_HH */

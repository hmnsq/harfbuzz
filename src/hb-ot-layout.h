/*
 * Copyright © 2007,2008,2009  Red Hat, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 */

#إذا !defined(HB_OT_H_درون) && !defined(HB_NO_SINGLE_سرآیند_ERROR)
#خطا "Include <hb-ot.h> instead."
#پایان_اگر

#اگر_تعریف_نشده HB_OT_LAYOUT_H
#تعریف_کن HB_OT_LAYOUT_H

#ضمن "hb.h"

#ضمن "hb-ot-name.h"

HB_BEGIN_DECLS


/**
 * HB_OT_TAG_BASE:
 *
 * OpenType [Baseline Table](https://docs.microsoft.com/en-us/typography/opentype/spec/base).
 */
#تعریف_کن HB_OT_برچسب_پایه HB_برچسب('B','A','S','E')
/**
 * HB_OT_TAG_GDEF:
 *
 * OpenType [Glyph Definition Table](https://docs.microsoft.com/en-us/typography/opentype/spec/gdef).
 */
#تعریف_کن HB_OT_برچسب_GDEF HB_برچسب('G','D','E','F')
/**
 * HB_OT_TAG_GSUB:
 *
 * OpenType [Glyph Substitution Table](https://docs.microsoft.com/en-us/typography/opentype/spec/gsub).
 */
#تعریف_کن HB_OT_برچسب_GSUB HB_برچسب('G','S','U','B')
/**
 * HB_OT_TAG_GPOS:
 *
 * OpenType [Glyph Positioning Table](https://docs.microsoft.com/en-us/typography/opentype/spec/gpos).
 */
#تعریف_کن HB_OT_برچسب_GPOS HB_برچسب('G','P','O','S')
/**
 * HB_OT_TAG_JSTF:
 *
 * OpenType [Justification Table](https://docs.microsoft.com/en-us/typography/opentype/spec/jstf).
 */
#تعریف_کن HB_OT_برچسب_JSTF HB_برچسب('J','S','T','F')


/*
 * Script & Language tags.
 */

/**
 * HB_OT_TAG_DEFAULT_SCRIPT:
 *
 * OpenType script tag, `DFLT`, for features that are not script-specific.
 *
 */
#تعریف_کن HB_OT_برچسب_DEFAULT_SCRIPT	HB_برچسب ('D', 'F', 'L', 'T')
/**
 * HB_OT_TAG_DEFAULT_LANGUAGE:
 *
 * OpenType language tag, `dflt`. Not a valid language tag, but some fonts
 * mistakenly use it.
 */
#تعریف_کن HB_OT_برچسب_DEFAULT_LANGUAGE	HB_برچسب ('d', 'f', 'l', 't')

/**
 * HB_OT_MAX_TAGS_PER_SCRIPT:
 *
 * Maximum number of OpenType tags that can correspond to a give #hb_script_t.
 *
 * Since: 2.0.0
 **/
#تعریف_کن HB_OT_MAX_برچسبها_در_هر_SCRIPT	3u
/**
 * HB_OT_MAX_TAGS_PER_LANGUAGE:
 *
 * Maximum number of OpenType tags that can correspond to a give #hb_language_t.
 *
 * Since: 2.0.0
 **/
#تعریف_کن HB_OT_MAX_برچسبها_در_هر_LANGUAGE	3u

HB_EXTERN فراغ
hb_ot_برچسبها_from_script_and_language (hb_script_t   script,
				     hb_language_t language,
				     بیعلامت صحيح *script_شمار /* IN/OUT */,
				     hb_برچسب_t     *script_برچسبها /*بیرون*/,
				     بیعلامت صحيح *language_شمار /* IN/OUT */,
				     hb_برچسب_t     *language_برچسبها /*بیرون*/);

HB_EXTERN hb_script_t
hb_ot_برچسب_مقصد_script (hb_برچسب_t برچسب_2);

HB_EXTERN hb_language_t
hb_ot_برچسب_مقصد_language (hb_برچسب_t برچسب_2);

HB_EXTERN فراغ
hb_ot_برچسبها_مقصد_script_and_language (hb_برچسب_t       script_برچسب,
				   hb_برچسب_t       language_برچسب,
				   hb_script_t   *script /*بیرون*/,
				   hb_language_t *language /*بیرون*/);


/*
 * GDEF
 */

HB_EXTERN hb_bool_t
hb_ot_layout_has_glyph_classes (hb_face_t *face);

/**
 * hb_ot_layout_glyph_class_t:
 * @HB_OT_LAYOUT_GLYPH_CLASS_UNCLASSIFIED: Glyphs not matching the other classifications
 * @HB_OT_LAYOUT_GLYPH_CLASS_BASE_GLYPH: Spacing, single characters, capable of accepting marks
 * @HB_OT_LAYOUT_GLYPH_CLASS_LIGATURE: Glyphs that represent ligation of multiple characters
 * @HB_OT_LAYOUT_GLYPH_CLASS_MARK: Non-spacing, combining glyphs that represent marks
 * @HB_OT_LAYOUT_GLYPH_CLASS_COMPONENT: Spacing glyphs that represent part of a single character
 *
 * The GDEF classes defined for glyphs.
 *
 **/
تعريفنوع تعداد {
  HB_OT_LAYOUT_GLYPH_CLASS_UNCLASSIFIED	= 0,
  HB_OT_LAYOUT_GLYPH_CLASS_پایه_GLYPH	= 1,
  HB_OT_LAYOUT_GLYPH_CLASS_LIGATURE	= 2,
  HB_OT_LAYOUT_GLYPH_CLASS_MARK		= 3,
  HB_OT_LAYOUT_GLYPH_CLASS_COMPONENT	= 4
} hb_ot_layout_glyph_class_t;

HB_EXTERN hb_ot_layout_glyph_class_t
hb_ot_layout_get_glyph_class (hb_face_t      *face,
			      hb_codepoint_t  glyph);

HB_EXTERN فراغ
hb_ot_layout_get_glyphs_درون_class (hb_face_t                  *face,
				  hb_ot_layout_glyph_class_t  klass,
				  hb_مجموعه_t                   *glyphs /*بیرون*/);

/* Not that useful.  Provides list of attach points for a glyph that a
 * client may want to cache */
HB_EXTERN بیعلامت صحيح
hb_ot_layout_get_attach_points (hb_face_t      *face,
				hb_codepoint_t  glyph,
				بیعلامت صحيح    شروع_جابه_جایی,
				بیعلامت صحيح   *نقطه_شمار /* IN/OUT */,
				بیعلامت صحيح   *نقطه_array /*بیرون*/);

/* Ligature caret positions */
HB_EXTERN بیعلامت صحيح
hb_ot_layout_get_ligature_carets (hb_font_t      *font,
				  hb_direction_t  direction,
				  hb_codepoint_t  glyph,
				  بیعلامت صحيح    شروع_جابه_جایی,
				  بیعلامت صحيح   *caret_شمار /* IN/OUT */,
				  hb_position_t  *caret_array /*بیرون*/);


/*
 * GSUB/GPOS feature query and enumeration interface
 */

/**
 * HB_OT_LAYOUT_NO_SCRIPT_INDEX:
 *
 * Special value for script index indicating unsupported script.
 */
#تعریف_کن HB_OT_LAYOUT_NO_SCRIPT_شاخص		0xFFFFu
/**
 * HB_OT_LAYOUT_NO_FEATURE_INDEX:
 *
 * Special value for feature index indicating unsupported feature.
 */
#تعریف_کن HB_OT_LAYOUT_NO_FEATURE_شاخص		0xFFFFu
/**
 * HB_OT_LAYOUT_DEFAULT_LANGUAGE_INDEX:
 *
 * Special value for language index indicating default or unsupported language.
 */
#تعریف_کن HB_OT_LAYOUT_DEFAULT_LANGUAGE_شاخص	0xFFFFu
/**
 * HB_OT_LAYOUT_NO_VARIATIONS_INDEX:
 *
 * Special value for variations index indicating unsupported variation.
 */
#تعریف_کن HB_OT_LAYOUT_NO_VARIATIONS_شاخص	0xFFFFFFFFu

HB_EXTERN بیعلامت صحيح
hb_ot_layout_جدول_get_script_برچسبها (hb_face_t    *face,
				    hb_برچسب_t      جدول_برچسب_2,
				    بیعلامت صحيح  شروع_جابه_جایی,
				    بیعلامت صحيح *script_شمار /* IN/OUT */,
				    hb_برچسب_t     *script_برچسبها /*بیرون*/);

HB_EXTERN hb_bool_t
hb_ot_layout_جدول_پیدا_کن_script (hb_face_t    *face,
				hb_برچسب_t      جدول_برچسب_2,
				hb_برچسب_t      script_برچسب,
				بیعلامت صحيح *script_شاخص /*بیرون*/);

HB_EXTERN hb_bool_t
hb_ot_layout_جدول_select_script (hb_face_t      *face,
				  hb_برچسب_t        جدول_برچسب_2,
				  بیعلامت صحيح    script_شمار,
				  ثابت hb_برچسب_t *script_برچسبها,
				  بیعلامت صحيح   *script_شاخص /*بیرون*/,
				  hb_برچسب_t       *chosen_script /*بیرون*/);

HB_EXTERN بیعلامت صحيح
hb_ot_layout_جدول_get_feature_برچسبها (hb_face_t    *face,
				     hb_برچسب_t      جدول_برچسب_2,
				     بیعلامت صحيح  شروع_جابه_جایی,
				     بیعلامت صحيح *feature_شمار_2 /* IN/OUT */,
				     hb_برچسب_t     *feature_برچسبها /*بیرون*/);

HB_EXTERN بیعلامت صحيح
hb_ot_layout_script_get_language_برچسبها (hb_face_t    *face,
				       hb_برچسب_t      جدول_برچسب_2,
				       بیعلامت صحيح  script_شاخص,
				       بیعلامت صحيح  شروع_جابه_جایی,
				       بیعلامت صحيح *language_شمار /* IN/OUT */,
				       hb_برچسب_t     *language_برچسبها /*بیرون*/);

HB_EXTERN hb_bool_t
hb_ot_layout_script_select_language (hb_face_t      *face,
				     hb_برچسب_t        جدول_برچسب_2,
				     بیعلامت صحيح    script_شاخص,
				     بیعلامت صحيح    language_شمار,
				     ثابت hb_برچسب_t *language_برچسبها,
				     بیعلامت صحيح   *language_شاخص /*بیرون*/);

HB_EXTERN hb_bool_t
hb_ot_layout_script_select_language2 (hb_face_t      *face,
				     hb_برچسب_t        جدول_برچسب_2,
				     بیعلامت صحيح    script_شاخص,
				     بیعلامت صحيح    language_شمار,
				     ثابت hb_برچسب_t *language_برچسبها,
				     بیعلامت صحيح   *language_شاخص /*بیرون*/,
				     hb_برچسب_t       *chosen_language /*بیرون*/);

HB_EXTERN hb_bool_t
hb_ot_layout_language_get_required_feature_شاخص (hb_face_t    *face,
						  hb_برچسب_t      جدول_برچسب_2,
						  بیعلامت صحيح  script_شاخص,
						  بیعلامت صحيح  language_شاخص,
						  بیعلامت صحيح *feature_شاخص /*بیرون*/);

HB_EXTERN hb_bool_t
hb_ot_layout_language_get_required_feature (hb_face_t    *face,
					    hb_برچسب_t      جدول_برچسب_2,
					    بیعلامت صحيح  script_شاخص,
					    بیعلامت صحيح  language_شاخص,
					    بیعلامت صحيح *feature_شاخص /*بیرون*/,
					    hb_برچسب_t     *feature_برچسب /*بیرون*/);

HB_EXTERN بیعلامت صحيح
hb_ot_layout_language_get_feature_indexes (hb_face_t    *face,
					   hb_برچسب_t      جدول_برچسب_2,
					   بیعلامت صحيح  script_شاخص,
					   بیعلامت صحيح  language_شاخص,
					   بیعلامت صحيح  شروع_جابه_جایی,
					   بیعلامت صحيح *feature_شمار_2 /* IN/OUT */,
					   بیعلامت صحيح *feature_indexes /*بیرون*/);

HB_EXTERN بیعلامت صحيح
hb_ot_layout_language_get_feature_برچسبها (hb_face_t    *face,
					hb_برچسب_t      جدول_برچسب_2,
					بیعلامت صحيح  script_شاخص,
					بیعلامت صحيح  language_شاخص,
					بیعلامت صحيح  شروع_جابه_جایی,
					بیعلامت صحيح *feature_شمار_2 /* IN/OUT */,
					hb_برچسب_t     *feature_برچسبها /*بیرون*/);

HB_EXTERN hb_bool_t
hb_ot_layout_language_پیدا_کن_feature (hb_face_t    *face,
				    hb_برچسب_t      جدول_برچسب_2,
				    بیعلامت صحيح  script_شاخص,
				    بیعلامت صحيح  language_شاخص,
				    hb_برچسب_t      feature_برچسب,
				    بیعلامت صحيح *feature_شاخص /*بیرون*/);

HB_EXTERN بیعلامت صحيح
hb_ot_layout_feature_get_lookups (hb_face_t    *face,
				  hb_برچسب_t      جدول_برچسب_2,
				  بیعلامت صحيح  feature_شاخص,
				  بیعلامت صحيح  شروع_جابه_جایی,
				  بیعلامت صحيح *lookup_شمار /* IN/OUT */,
				  بیعلامت صحيح *lookup_indexes /*بیرون*/);

HB_EXTERN بیعلامت صحيح
hb_ot_layout_جدول_get_lookup_شمار (hb_face_t    *face,
				     hb_برچسب_t      جدول_برچسب_2);

HB_EXTERN فراغ
hb_ot_layout_collect_features (hb_face_t      *face,
			       hb_برچسب_t        جدول_برچسب_2,
			       ثابت hb_برچسب_t *scripts,
			       ثابت hb_برچسب_t *languages,
			       ثابت hb_برچسب_t *features,
			       hb_مجموعه_t       *feature_indexes /*بیرون*/);

HB_EXTERN فراغ
hb_ot_layout_collect_features_map (hb_face_t      *face,
				   hb_برچسب_t        جدول_برچسب_2,
				   بیعلامت        script_شاخص,
				   بیعلامت        language_شاخص,
				   hb_map_t       *feature_map /*بیرون*/);

HB_EXTERN فراغ
hb_ot_layout_collect_lookups (hb_face_t      *face,
			      hb_برچسب_t        جدول_برچسب_2,
			      ثابت hb_برچسب_t *scripts,
			      ثابت hb_برچسب_t *languages,
			      ثابت hb_برچسب_t *features,
			      hb_مجموعه_t       *lookup_indexes /*بیرون*/);

HB_EXTERN فراغ
hb_ot_layout_lookup_collect_glyphs (hb_face_t    *face,
				    hb_برچسب_t      جدول_برچسب_2,
				    بیعلامت صحيح  lookup_شاخص,
				    hb_مجموعه_t     *glyphs_before, /* OUT.  May be NULL */
				    hb_مجموعه_t     *glyphs_درون_داد,  /* OUT.  May be NULL */
				    hb_مجموعه_t     *glyphs_after,  /* OUT.  May be NULL */
				    hb_مجموعه_t     *glyphs_output  /* OUT.  May be NULL */);


/* Variations support */

HB_EXTERN hb_bool_t
hb_ot_layout_جدول_پیدا_کن_feature_variations (hb_face_t    *face,
					    hb_برچسب_t      جدول_برچسب_2,
					    ثابت صحيح    *coords,
					    بیعلامت صحيح  رقم_coords,
					    بیعلامت صحيح *variations_شاخص /* out */);

HB_EXTERN بیعلامت صحيح
hb_ot_layout_feature_with_variations_get_lookups (hb_face_t    *face,
						  hb_برچسب_t      جدول_برچسب_2,
						  بیعلامت صحيح  feature_شاخص,
						  بیعلامت صحيح  variations_شاخص,
						  بیعلامت صحيح  شروع_جابه_جایی,
						  بیعلامت صحيح *lookup_شمار /* IN/OUT */,
						  بیعلامت صحيح *lookup_indexes /*بیرون*/);


/*
 * GSUB
 */

HB_EXTERN hb_bool_t
hb_ot_layout_has_substitution (hb_face_t *face);

HB_EXTERN بیعلامت
hb_ot_layout_lookup_get_glyph_alternates (hb_face_t      *face,
					  بیعلامت        lookup_شاخص,
					  hb_codepoint_t  glyph,
					  بیعلامت        شروع_جابه_جایی,
					  بیعلامت       *alternate_شمار /* IN/OUT */,
					  hb_codepoint_t *alternate_glyphs /*بیرون*/);

HB_EXTERN hb_bool_t
hb_ot_layout_lookup_collect_glyph_alternates (hb_face_t *face,
					      بیعلامت   lookup_شاخص,
					      hb_map_t  *alternate_شمار /* IN/OUT */,
					      hb_map_t  *alternate_glyphs /* IN/OUT */);

HB_EXTERN hb_bool_t
hb_ot_layout_lookup_would_substitute (hb_face_t            *face,
				      بیعلامت صحيح          lookup_شاخص,
				      ثابت hb_codepoint_t *glyphs,
				      بیعلامت صحيح          glyphs_درازی,
				      hb_bool_t             zero_context);

HB_EXTERN فراغ
hb_ot_layout_lookup_substitute_بستگی (hb_face_t    *face,
					بیعلامت صحيح  lookup_شاخص,
					hb_مجموعه_t     *glyphs
					/*TODO , hb_bool_t  inclusive */);

HB_EXTERN فراغ
hb_ot_layout_lookups_substitute_بستگی (hb_face_t      *face,
					 ثابت hb_مجموعه_t *lookups,
					 hb_مجموعه_t       *glyphs);


/*
 * GPOS
 */

HB_EXTERN hb_bool_t
hb_ot_layout_has_positioning (hb_face_t *face);

/* Optical 'size' feature info.  Returns true if found.
 * https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#size */
HB_EXTERN hb_bool_t
hb_ot_layout_get_اندازه_params (hb_face_t       *face,
			      بیعلامت صحيح    *design_اندازه_2,       /* OUT.  May be NULL */
			      بیعلامت صحيح    *subfamily_id,      /* OUT.  May be NULL */
			      hb_ot_name_id_t *subfamily_name_id, /* OUT.  May be NULL */
			      بیعلامت صحيح    *range_شروع_2,       /* OUT.  May be NULL */
			      بیعلامت صحيح    *range_انتها_2          /* OUT.  May be NULL */);

HB_EXTERN hb_position_t
hb_ot_layout_lookup_get_optical_bound (hb_font_t      *font,
				       بیعلامت        lookup_شاخص,
				       hb_direction_t  direction,
				       hb_codepoint_t  glyph);


/*
 * GSUB/GPOS
 */

HB_EXTERN hb_bool_t
hb_ot_layout_feature_get_name_ids (hb_face_t       *face,
				   hb_برچسب_t         جدول_برچسب_2,
				   بیعلامت صحيح     feature_شاخص,
				   hb_ot_name_id_t *label_id             /* OUT.  May be NULL */,
				   hb_ot_name_id_t *tooltip_id           /* OUT.  May be NULL */,
				   hb_ot_name_id_t *sample_id            /* OUT.  May be NULL */,
				   بیعلامت صحيح    *رقم_named_parameters /* OUT.  May be NULL */,
				   hb_ot_name_id_t *اولی_param_id       /* OUT.  May be NULL */);


HB_EXTERN بیعلامت صحيح
hb_ot_layout_feature_get_characters (hb_face_t      *face,
				     hb_برچسب_t        جدول_برچسب_2,
				     بیعلامت صحيح    feature_شاخص,
				     بیعلامت صحيح    شروع_جابه_جایی,
				     بیعلامت صحيح   *char_شمار    /* IN/OUT.  May be NULL */,
				     hb_codepoint_t *characters    /* OUT.     May be NULL */);


/*
 * BASE
 */

HB_EXTERN hb_bool_t
hb_ot_layout_get_font_extents (hb_font_t         *font,
			       hb_direction_t     direction,
			       hb_برچسب_t           script_برچسب,
			       hb_برچسب_t           language_برچسب,
			       hb_font_extents_t *extents);

HB_EXTERN hb_bool_t
hb_ot_layout_get_font_extents2 (hb_font_t         *font,
				hb_direction_t     direction,
				hb_script_t        script,
				hb_language_t      language,
				hb_font_extents_t *extents);

/**
 * hb_ot_layout_baseline_tag_t:
 * @HB_OT_LAYOUT_BASELINE_TAG_ROMAN: The baseline used by alphabetic scripts such as Latin, Cyrillic and Greek.
 * In vertical writing mode, the alphabetic baseline for characters rotated 90 degrees clockwise.
 * (This would not apply to alphabetic characters that remain upright in vertical writing mode, since these
 * characters are not rotated.)
 * @HB_OT_LAYOUT_BASELINE_TAG_HANGING: The hanging baseline. In horizontal direction, this is the horizontal
 * line from which syllables seem, to hang in Tibetan and other similar scripts. In vertical writing mode,
 * for Tibetan (or some other similar script) characters rotated 90 degrees clockwise.
 * @HB_OT_LAYOUT_BASELINE_TAG_IDEO_FACE_BOTTOM_OR_LEFT: Ideographic character face bottom or left edge,
 * if the direction is horizontal or vertical, respectively.
 * @HB_OT_LAYOUT_BASELINE_TAG_IDEO_FACE_TOP_OR_RIGHT: Ideographic character face top or right edge,
 * if the direction is horizontal or vertical, respectively.
 * @HB_OT_LAYOUT_BASELINE_TAG_IDEO_FACE_CENTRAL: The center of the ideographic character face. Since: 4.0.0
 * @HB_OT_LAYOUT_BASELINE_TAG_IDEO_EMBOX_BOTTOM_OR_LEFT: Ideographic em-box bottom or left edge,
 * if the direction is horizontal or vertical, respectively.
 * @HB_OT_LAYOUT_BASELINE_TAG_IDEO_EMBOX_TOP_OR_RIGHT: Ideographic em-box top or right edge baseline,
 * @HB_OT_LAYOUT_BASELINE_TAG_IDEO_EMBOX_CENTRAL: The center of the ideographic em-box. Since: 4.0.0
 * if the direction is horizontal or vertical, respectively.
 * @HB_OT_LAYOUT_BASELINE_TAG_MATH: The baseline about which mathematical characters are centered.
 * In vertical writing mode when mathematical characters rotated 90 degrees clockwise, are centered.
 *
 * Baseline tags from [Baseline Tags](https://docs.microsoft.com/en-us/typography/opentype/spec/baselinetags) registry.
 *
 * Since: 2.6.0
 */
تعريفنوع تعداد {
  HB_OT_LAYOUT_BASELINE_برچسب_ROMAN			= HB_برچسب ('r','o','m','n'),
  HB_OT_LAYOUT_BASELINE_برچسب_HANGING			= HB_برچسب ('h','a','n','g'),
  HB_OT_LAYOUT_BASELINE_برچسب_IDEO_FACE_BOTTOM_OR_چپ	= HB_برچسب ('i','c','f','b'),
  HB_OT_LAYOUT_BASELINE_برچسب_IDEO_FACE_بالاترین_OR_راست	= HB_برچسب ('i','c','f','t'),
  HB_OT_LAYOUT_BASELINE_برچسب_IDEO_FACE_CENTRAL		= HB_برچسب ('I','c','f','c'),
  HB_OT_LAYOUT_BASELINE_برچسب_IDEO_EMBOX_BOTTOM_OR_چپ	= HB_برچسب ('i','d','e','o'),
  HB_OT_LAYOUT_BASELINE_برچسب_IDEO_EMBOX_بالاترین_OR_راست	= HB_برچسب ('i','d','t','p'),
  HB_OT_LAYOUT_BASELINE_برچسب_IDEO_EMBOX_CENTRAL		= HB_برچسب ('I','d','c','e'),
  HB_OT_LAYOUT_BASELINE_برچسب_MATH			= HB_برچسب ('m','a','t','h'),

  /*< private >*/
  HB_OT_LAYOUT_BASELINE_برچسب_MAX_مقدار = HB_برچسب_MAX_SIGNED /*< skip >*/
} hb_ot_layout_baseline_برچسب_t;

HB_EXTERN hb_ot_layout_baseline_برچسب_t
hb_ot_layout_get_horizontal_baseline_برچسب_for_script (hb_script_t script);

HB_EXTERN hb_bool_t
hb_ot_layout_get_baseline (hb_font_t                   *font,
			   hb_ot_layout_baseline_برچسب_t  baseline_برچسب,
			   hb_direction_t               direction,
			   hb_برچسب_t                     script_برچسب,
			   hb_برچسب_t                     language_برچسب,
			   hb_position_t               *coord        /* OUT.  May be NULL. */);

HB_EXTERN hb_bool_t
hb_ot_layout_get_baseline2 (hb_font_t                   *font,
			    hb_ot_layout_baseline_برچسب_t  baseline_برچسب,
			    hb_direction_t               direction,
			    hb_script_t                  script,
			    hb_language_t                language,
			    hb_position_t               *coord        /* OUT.  May be NULL. */);

HB_EXTERN فراغ
hb_ot_layout_get_baseline_with_fallback (hb_font_t                   *font,
					 hb_ot_layout_baseline_برچسب_t  baseline_برچسب,
					 hb_direction_t               direction,
					 hb_برچسب_t                     script_برچسب,
					 hb_برچسب_t                     language_برچسب,
					 hb_position_t               *coord        /*بیرون*/);

HB_EXTERN فراغ
hb_ot_layout_get_baseline_with_fallback2 (hb_font_t                   *font,
					  hb_ot_layout_baseline_برچسب_t  baseline_برچسب,
					  hb_direction_t               direction,
					  hb_script_t                  script,
					  hb_language_t                language,
					  hb_position_t               *coord        /*بیرون*/);

HB_انتها_DECLS

#پایان_اگر /* HB_OT_LAYOUT_H */

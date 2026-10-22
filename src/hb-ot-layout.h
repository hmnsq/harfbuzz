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

#إذا !defined(HB_OT_H_دننه) && !defined(HB_NO_SINGLE_سرلیک_ERROR)
#تېروتنه "Include <hb-ot.h> instead."
#د_که_پای

#که_نه_تعریف_شوی HB_OT_LAYOUT_H
#تعریف_کړه HB_OT_LAYOUT_H

#ضمن "hb.h"

#ضمن "hb-ot-name.h"

HB_BEGIN_DECLS


/**
 * HB_OT_TAG_BASE:
 *
 * OpenType [Baseline Table](https://docs.microsoft.com/en-us/typography/opentype/spec/base).
 */
#تعریف_کړه HB_OT_نښان_بنسټ HB_نښان('B','A','S','E')
/**
 * HB_OT_TAG_GDEF:
 *
 * OpenType [Glyph Definition Table](https://docs.microsoft.com/en-us/typography/opentype/spec/gdef).
 */
#تعریف_کړه HB_OT_نښان_GDEF HB_نښان('G','D','E','F')
/**
 * HB_OT_TAG_GSUB:
 *
 * OpenType [Glyph Substitution Table](https://docs.microsoft.com/en-us/typography/opentype/spec/gsub).
 */
#تعریف_کړه HB_OT_نښان_GSUB HB_نښان('G','S','U','B')
/**
 * HB_OT_TAG_GPOS:
 *
 * OpenType [Glyph Positioning Table](https://docs.microsoft.com/en-us/typography/opentype/spec/gpos).
 */
#تعریف_کړه HB_OT_نښان_GPOS HB_نښان('G','P','O','S')
/**
 * HB_OT_TAG_JSTF:
 *
 * OpenType [Justification Table](https://docs.microsoft.com/en-us/typography/opentype/spec/jstf).
 */
#تعریف_کړه HB_OT_نښان_JSTF HB_نښان('J','S','T','F')


/*
 * Script & Language tags.
 */

/**
 * HB_OT_TAG_DEFAULT_SCRIPT:
 *
 * OpenType script tag, `DFLT`, for features that are not script-specific.
 *
 */
#تعریف_کړه HB_OT_نښان_DEFAULT_SCRIPT	HB_نښان ('D', 'F', 'L', 'T')
/**
 * HB_OT_TAG_DEFAULT_LANGUAGE:
 *
 * OpenType language tag, `dflt`. Not a valid language tag, but some fonts
 * mistakenly use it.
 */
#تعریف_کړه HB_OT_نښان_DEFAULT_LANGUAGE	HB_نښان ('d', 'f', 'l', 't')

/**
 * HB_OT_MAX_TAGS_PER_SCRIPT:
 *
 * Maximum number of OpenType tags that can correspond to a give #hb_script_t.
 *
 * Since: 2.0.0
 **/
#تعریف_کړه HB_OT_MAX_نښانونه_فی_SCRIPT	3u
/**
 * HB_OT_MAX_TAGS_PER_LANGUAGE:
 *
 * Maximum number of OpenType tags that can correspond to a give #hb_language_t.
 *
 * Since: 2.0.0
 **/
#تعریف_کړه HB_OT_MAX_نښانونه_فی_LANGUAGE	3u

HB_EXTERN فراغ
hb_ot_نښانونه_from_script_and_language (hb_script_t   script,
				     hb_language_t language,
				     بینښه صحيح *script_شمېر /* IN/OUT */,
				     hb_نښان_t     *script_نښانونه /*بهر*/,
				     بینښه صحيح *language_شمېر /* IN/OUT */,
				     hb_نښان_t     *language_نښانونه /*بهر*/);

HB_EXTERN hb_script_t
hb_ot_نښان_منزل_script (hb_نښان_t نښان_2);

HB_EXTERN hb_language_t
hb_ot_نښان_منزل_language (hb_نښان_t نښان_2);

HB_EXTERN فراغ
hb_ot_نښانونه_منزل_script_and_language (hb_نښان_t       script_نښان,
				   hb_نښان_t       language_نښان,
				   hb_script_t   *script /*بهر*/,
				   hb_language_t *language /*بهر*/);


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
  HB_OT_LAYOUT_GLYPH_CLASS_بنسټ_GLYPH	= 1,
  HB_OT_LAYOUT_GLYPH_CLASS_LIGATURE	= 2,
  HB_OT_LAYOUT_GLYPH_CLASS_MARK		= 3,
  HB_OT_LAYOUT_GLYPH_CLASS_COMPONENT	= 4
} hb_ot_layout_glyph_class_t;

HB_EXTERN hb_ot_layout_glyph_class_t
hb_ot_layout_get_glyph_class (hb_face_t      *face,
			      hb_codepoint_t  glyph);

HB_EXTERN فراغ
hb_ot_layout_get_glyphs_دننه_class (hb_face_t                  *face,
				  hb_ot_layout_glyph_class_t  klass,
				  hb_سېټ_t                   *glyphs /*بهر*/);

/* Not that useful.  Provides list of attach points for a glyph that a
 * client may want to cache */
HB_EXTERN بینښه صحيح
hb_ot_layout_get_attach_points (hb_face_t      *face,
				hb_codepoint_t  glyph,
				بینښه صحيح    پیل_دننه_واټن,
				بینښه صحيح   *ټکی_شمېر /* IN/OUT */,
				بینښه صحيح   *ټکی_array /*بهر*/);

/* Ligature caret positions */
HB_EXTERN بینښه صحيح
hb_ot_layout_get_ligature_carets (hb_font_t      *font,
				  hb_direction_t  direction,
				  hb_codepoint_t  glyph,
				  بینښه صحيح    پیل_دننه_واټن,
				  بینښه صحيح   *caret_شمېر /* IN/OUT */,
				  hb_position_t  *caret_array /*بهر*/);


/*
 * GSUB/GPOS feature query and enumeration interface
 */

/**
 * HB_OT_LAYOUT_NO_SCRIPT_INDEX:
 *
 * Special value for script index indicating unsupported script.
 */
#تعریف_کړه HB_OT_LAYOUT_NO_SCRIPT_شاخص		0xFFFFu
/**
 * HB_OT_LAYOUT_NO_FEATURE_INDEX:
 *
 * Special value for feature index indicating unsupported feature.
 */
#تعریف_کړه HB_OT_LAYOUT_NO_FEATURE_شاخص		0xFFFFu
/**
 * HB_OT_LAYOUT_DEFAULT_LANGUAGE_INDEX:
 *
 * Special value for language index indicating default or unsupported language.
 */
#تعریف_کړه HB_OT_LAYOUT_DEFAULT_LANGUAGE_شاخص	0xFFFFu
/**
 * HB_OT_LAYOUT_NO_VARIATIONS_INDEX:
 *
 * Special value for variations index indicating unsupported variation.
 */
#تعریف_کړه HB_OT_LAYOUT_NO_VARIATIONS_شاخص	0xFFFFFFFFu

HB_EXTERN بینښه صحيح
hb_ot_layout_جدول_get_script_نښانونه (hb_face_t    *face,
				    hb_نښان_t      جدول_نښان_2,
				    بینښه صحيح  پیل_دننه_واټن,
				    بینښه صحيح *script_شمېر /* IN/OUT */,
				    hb_نښان_t     *script_نښانونه /*بهر*/);

HB_EXTERN hb_bool_t
hb_ot_layout_جدول_پیدا_کړه_script (hb_face_t    *face,
				hb_نښان_t      جدول_نښان_2,
				hb_نښان_t      script_نښان,
				بینښه صحيح *script_شاخص /*بهر*/);

HB_EXTERN hb_bool_t
hb_ot_layout_جدول_select_script (hb_face_t      *face,
				  hb_نښان_t        جدول_نښان_2,
				  بینښه صحيح    script_شمېر,
				  ثابت hb_نښان_t *script_نښانونه,
				  بینښه صحيح   *script_شاخص /*بهر*/,
				  hb_نښان_t       *chosen_script /*بهر*/);

HB_EXTERN بینښه صحيح
hb_ot_layout_جدول_get_feature_نښانونه (hb_face_t    *face,
				     hb_نښان_t      جدول_نښان_2,
				     بینښه صحيح  پیل_دننه_واټن,
				     بینښه صحيح *feature_شمېر_2 /* IN/OUT */,
				     hb_نښان_t     *feature_نښانونه /*بهر*/);

HB_EXTERN بینښه صحيح
hb_ot_layout_script_get_language_نښانونه (hb_face_t    *face,
				       hb_نښان_t      جدول_نښان_2,
				       بینښه صحيح  script_شاخص,
				       بینښه صحيح  پیل_دننه_واټن,
				       بینښه صحيح *language_شمېر /* IN/OUT */,
				       hb_نښان_t     *language_نښانونه /*بهر*/);

HB_EXTERN hb_bool_t
hb_ot_layout_script_select_language (hb_face_t      *face,
				     hb_نښان_t        جدول_نښان_2,
				     بینښه صحيح    script_شاخص,
				     بینښه صحيح    language_شمېر,
				     ثابت hb_نښان_t *language_نښانونه,
				     بینښه صحيح   *language_شاخص /*بهر*/);

HB_EXTERN hb_bool_t
hb_ot_layout_script_select_language2 (hb_face_t      *face,
				     hb_نښان_t        جدول_نښان_2,
				     بینښه صحيح    script_شاخص,
				     بینښه صحيح    language_شمېر,
				     ثابت hb_نښان_t *language_نښانونه,
				     بینښه صحيح   *language_شاخص /*بهر*/,
				     hb_نښان_t       *chosen_language /*بهر*/);

HB_EXTERN hb_bool_t
hb_ot_layout_language_get_required_feature_شاخص (hb_face_t    *face,
						  hb_نښان_t      جدول_نښان_2,
						  بینښه صحيح  script_شاخص,
						  بینښه صحيح  language_شاخص,
						  بینښه صحيح *feature_شاخص /*بهر*/);

HB_EXTERN hb_bool_t
hb_ot_layout_language_get_required_feature (hb_face_t    *face,
					    hb_نښان_t      جدول_نښان_2,
					    بینښه صحيح  script_شاخص,
					    بینښه صحيح  language_شاخص,
					    بینښه صحيح *feature_شاخص /*بهر*/,
					    hb_نښان_t     *feature_نښان /*بهر*/);

HB_EXTERN بینښه صحيح
hb_ot_layout_language_get_feature_indexes (hb_face_t    *face,
					   hb_نښان_t      جدول_نښان_2,
					   بینښه صحيح  script_شاخص,
					   بینښه صحيح  language_شاخص,
					   بینښه صحيح  پیل_دننه_واټن,
					   بینښه صحيح *feature_شمېر_2 /* IN/OUT */,
					   بینښه صحيح *feature_indexes /*بهر*/);

HB_EXTERN بینښه صحيح
hb_ot_layout_language_get_feature_نښانونه (hb_face_t    *face,
					hb_نښان_t      جدول_نښان_2,
					بینښه صحيح  script_شاخص,
					بینښه صحيح  language_شاخص,
					بینښه صحيح  پیل_دننه_واټن,
					بینښه صحيح *feature_شمېر_2 /* IN/OUT */,
					hb_نښان_t     *feature_نښانونه /*بهر*/);

HB_EXTERN hb_bool_t
hb_ot_layout_language_پیدا_کړه_feature (hb_face_t    *face,
				    hb_نښان_t      جدول_نښان_2,
				    بینښه صحيح  script_شاخص,
				    بینښه صحيح  language_شاخص,
				    hb_نښان_t      feature_نښان,
				    بینښه صحيح *feature_شاخص /*بهر*/);

HB_EXTERN بینښه صحيح
hb_ot_layout_feature_get_lookups (hb_face_t    *face,
				  hb_نښان_t      جدول_نښان_2,
				  بینښه صحيح  feature_شاخص,
				  بینښه صحيح  پیل_دننه_واټن,
				  بینښه صحيح *lookup_شمېر /* IN/OUT */,
				  بینښه صحيح *lookup_indexes /*بهر*/);

HB_EXTERN بینښه صحيح
hb_ot_layout_جدول_get_lookup_شمېر (hb_face_t    *face,
				     hb_نښان_t      جدول_نښان_2);

HB_EXTERN فراغ
hb_ot_layout_collect_features (hb_face_t      *face,
			       hb_نښان_t        جدول_نښان_2,
			       ثابت hb_نښان_t *scripts,
			       ثابت hb_نښان_t *languages,
			       ثابت hb_نښان_t *features,
			       hb_سېټ_t       *feature_indexes /*بهر*/);

HB_EXTERN فراغ
hb_ot_layout_collect_features_map (hb_face_t      *face,
				   hb_نښان_t        جدول_نښان_2,
				   بینښه        script_شاخص,
				   بینښه        language_شاخص,
				   hb_map_t       *feature_map /*بهر*/);

HB_EXTERN فراغ
hb_ot_layout_collect_lookups (hb_face_t      *face,
			      hb_نښان_t        جدول_نښان_2,
			      ثابت hb_نښان_t *scripts,
			      ثابت hb_نښان_t *languages,
			      ثابت hb_نښان_t *features,
			      hb_سېټ_t       *lookup_indexes /*بهر*/);

HB_EXTERN فراغ
hb_ot_layout_lookup_collect_glyphs (hb_face_t    *face,
				    hb_نښان_t      جدول_نښان_2,
				    بینښه صحيح  lookup_شاخص,
				    hb_سېټ_t     *glyphs_before, /* OUT.  May be NULL */
				    hb_سېټ_t     *glyphs_ننوت,  /* OUT.  May be NULL */
				    hb_سېټ_t     *glyphs_after,  /* OUT.  May be NULL */
				    hb_سېټ_t     *glyphs_output  /* OUT.  May be NULL */);


/* Variations support */

HB_EXTERN hb_bool_t
hb_ot_layout_جدول_پیدا_کړه_feature_variations (hb_face_t    *face,
					    hb_نښان_t      جدول_نښان_2,
					    ثابت صحيح    *coords,
					    بینښه صحيح  عدد_coords,
					    بینښه صحيح *variations_شاخص /* out */);

HB_EXTERN بینښه صحيح
hb_ot_layout_feature_with_variations_get_lookups (hb_face_t    *face,
						  hb_نښان_t      جدول_نښان_2,
						  بینښه صحيح  feature_شاخص,
						  بینښه صحيح  variations_شاخص,
						  بینښه صحيح  پیل_دننه_واټن,
						  بینښه صحيح *lookup_شمېر /* IN/OUT */,
						  بینښه صحيح *lookup_indexes /*بهر*/);


/*
 * GSUB
 */

HB_EXTERN hb_bool_t
hb_ot_layout_has_substitution (hb_face_t *face);

HB_EXTERN بینښه
hb_ot_layout_lookup_get_glyph_alternates (hb_face_t      *face,
					  بینښه        lookup_شاخص,
					  hb_codepoint_t  glyph,
					  بینښه        پیل_دننه_واټن,
					  بینښه       *alternate_شمېر /* IN/OUT */,
					  hb_codepoint_t *alternate_glyphs /*بهر*/);

HB_EXTERN hb_bool_t
hb_ot_layout_lookup_collect_glyph_alternates (hb_face_t *face,
					      بینښه   lookup_شاخص,
					      hb_map_t  *alternate_شمېر /* IN/OUT */,
					      hb_map_t  *alternate_glyphs /* IN/OUT */);

HB_EXTERN hb_bool_t
hb_ot_layout_lookup_would_substitute (hb_face_t            *face,
				      بینښه صحيح          lookup_شاخص,
				      ثابت hb_codepoint_t *glyphs,
				      بینښه صحيح          glyphs_اوږدتیا,
				      hb_bool_t             zero_context);

HB_EXTERN فراغ
hb_ot_layout_lookup_substitute_تړون (hb_face_t    *face,
					بینښه صحيح  lookup_شاخص,
					hb_سېټ_t     *glyphs
					/*TODO , hb_bool_t  inclusive */);

HB_EXTERN فراغ
hb_ot_layout_lookups_substitute_تړون (hb_face_t      *face,
					 ثابت hb_سېټ_t *lookups,
					 hb_سېټ_t       *glyphs);


/*
 * GPOS
 */

HB_EXTERN hb_bool_t
hb_ot_layout_has_positioning (hb_face_t *face);

/* Optical 'size' feature info.  Returns true if found.
 * https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#size */
HB_EXTERN hb_bool_t
hb_ot_layout_get_اندازه_params (hb_face_t       *face,
			      بینښه صحيح    *design_اندازه_2,       /* OUT.  May be NULL */
			      بینښه صحيح    *subfamily_id,      /* OUT.  May be NULL */
			      hb_ot_name_id_t *subfamily_name_id, /* OUT.  May be NULL */
			      بینښه صحيح    *range_پیل_2,       /* OUT.  May be NULL */
			      بینښه صحيح    *range_پای_ټکی_2          /* OUT.  May be NULL */);

HB_EXTERN hb_position_t
hb_ot_layout_lookup_get_optical_bound (hb_font_t      *font,
				       بینښه        lookup_شاخص,
				       hb_direction_t  direction,
				       hb_codepoint_t  glyph);


/*
 * GSUB/GPOS
 */

HB_EXTERN hb_bool_t
hb_ot_layout_feature_get_name_ids (hb_face_t       *face,
				   hb_نښان_t         جدول_نښان_2,
				   بینښه صحيح     feature_شاخص,
				   hb_ot_name_id_t *label_id             /* OUT.  May be NULL */,
				   hb_ot_name_id_t *tooltip_id           /* OUT.  May be NULL */,
				   hb_ot_name_id_t *sample_id            /* OUT.  May be NULL */,
				   بینښه صحيح    *عدد_named_parameters /* OUT.  May be NULL */,
				   hb_ot_name_id_t *لومړی_param_id       /* OUT.  May be NULL */);


HB_EXTERN بینښه صحيح
hb_ot_layout_feature_get_characters (hb_face_t      *face,
				     hb_نښان_t        جدول_نښان_2,
				     بینښه صحيح    feature_شاخص,
				     بینښه صحيح    پیل_دننه_واټن,
				     بینښه صحيح   *char_شمېر    /* IN/OUT.  May be NULL */,
				     hb_codepoint_t *characters    /* OUT.     May be NULL */);


/*
 * BASE
 */

HB_EXTERN hb_bool_t
hb_ot_layout_get_font_extents (hb_font_t         *font,
			       hb_direction_t     direction,
			       hb_نښان_t           script_نښان,
			       hb_نښان_t           language_نښان,
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
  HB_OT_LAYOUT_BASELINE_نښان_ROMAN			= HB_نښان ('r','o','m','n'),
  HB_OT_LAYOUT_BASELINE_نښان_HANGING			= HB_نښان ('h','a','n','g'),
  HB_OT_LAYOUT_BASELINE_نښان_IDEO_FACE_BOTTOM_OR_کیڼ	= HB_نښان ('i','c','f','b'),
  HB_OT_LAYOUT_BASELINE_نښان_IDEO_FACE_سر_OR_ښی	= HB_نښان ('i','c','f','t'),
  HB_OT_LAYOUT_BASELINE_نښان_IDEO_FACE_CENTRAL		= HB_نښان ('I','c','f','c'),
  HB_OT_LAYOUT_BASELINE_نښان_IDEO_EMBOX_BOTTOM_OR_کیڼ	= HB_نښان ('i','d','e','o'),
  HB_OT_LAYOUT_BASELINE_نښان_IDEO_EMBOX_سر_OR_ښی	= HB_نښان ('i','d','t','p'),
  HB_OT_LAYOUT_BASELINE_نښان_IDEO_EMBOX_CENTRAL		= HB_نښان ('I','d','c','e'),
  HB_OT_LAYOUT_BASELINE_نښان_MATH			= HB_نښان ('m','a','t','h'),

  /*< private >*/
  HB_OT_LAYOUT_BASELINE_نښان_MAX_ارزښت = HB_نښان_MAX_SIGNED /*< skip >*/
} hb_ot_layout_baseline_نښان_t;

HB_EXTERN hb_ot_layout_baseline_نښان_t
hb_ot_layout_get_horizontal_baseline_نښان_for_script (hb_script_t script);

HB_EXTERN hb_bool_t
hb_ot_layout_get_baseline (hb_font_t                   *font,
			   hb_ot_layout_baseline_نښان_t  baseline_نښان,
			   hb_direction_t               direction,
			   hb_نښان_t                     script_نښان,
			   hb_نښان_t                     language_نښان,
			   hb_position_t               *coord        /* OUT.  May be NULL. */);

HB_EXTERN hb_bool_t
hb_ot_layout_get_baseline2 (hb_font_t                   *font,
			    hb_ot_layout_baseline_نښان_t  baseline_نښان,
			    hb_direction_t               direction,
			    hb_script_t                  script,
			    hb_language_t                language,
			    hb_position_t               *coord        /* OUT.  May be NULL. */);

HB_EXTERN فراغ
hb_ot_layout_get_baseline_with_fallback (hb_font_t                   *font,
					 hb_ot_layout_baseline_نښان_t  baseline_نښان,
					 hb_direction_t               direction,
					 hb_نښان_t                     script_نښان,
					 hb_نښان_t                     language_نښان,
					 hb_position_t               *coord        /*بهر*/);

HB_EXTERN فراغ
hb_ot_layout_get_baseline_with_fallback2 (hb_font_t                   *font,
					  hb_ot_layout_baseline_نښان_t  baseline_نښان,
					  hb_direction_t               direction,
					  hb_script_t                  script,
					  hb_language_t                language,
					  hb_position_t               *coord        /*بهر*/);

HB_پای_ټکی_DECLS

#د_که_پای /* HB_OT_LAYOUT_H */

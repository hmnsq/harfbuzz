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

#اگر !defined(HB_OT_H_اندر) && !defined(HB_NO_SINGLE_سرخی_ERROR)
#غلطی "Include <hb-ot.h> instead."
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_OT_LAYOUT_H
#تعریف_کرو HB_OT_LAYOUT_H

#شامل "hb.h"

#شامل "hb-ot-name.h"

HB_BEGIN_DECLS


/**
 * HB_OT_TAG_BASE:
 *
 * OpenType [Baseline Table](https://docs.microsoft.com/en-us/typography/opentype/spec/base).
 */
#تعریف_کرو HB_OT_لیبل_بنیاد HB_لیبل('B','A','S','E')
/**
 * HB_OT_TAG_GDEF:
 *
 * OpenType [Glyph Definition Table](https://docs.microsoft.com/en-us/typography/opentype/spec/gdef).
 */
#تعریف_کرو HB_OT_لیبل_GDEF HB_لیبل('G','D','E','F')
/**
 * HB_OT_TAG_GSUB:
 *
 * OpenType [Glyph Substitution Table](https://docs.microsoft.com/en-us/typography/opentype/spec/gsub).
 */
#تعریف_کرو HB_OT_لیبل_GSUB HB_لیبل('G','S','U','B')
/**
 * HB_OT_TAG_GPOS:
 *
 * OpenType [Glyph Positioning Table](https://docs.microsoft.com/en-us/typography/opentype/spec/gpos).
 */
#تعریف_کرو HB_OT_لیبل_GPOS HB_لیبل('G','P','O','S')
/**
 * HB_OT_TAG_JSTF:
 *
 * OpenType [Justification Table](https://docs.microsoft.com/en-us/typography/opentype/spec/jstf).
 */
#تعریف_کرو HB_OT_لیبل_JSTF HB_لیبل('J','S','T','F')


/*
 * Script & Language tags.
 */

/**
 * HB_OT_TAG_DEFAULT_SCRIPT:
 *
 * OpenType script tag, `DFLT`, for features that are not script-specific.
 *
 */
#تعریف_کرو HB_OT_لیبل_DEFAULT_SCRIPT	HB_لیبل ('D', 'F', 'L', 'T')
/**
 * HB_OT_TAG_DEFAULT_LANGUAGE:
 *
 * OpenType language tag, `dflt`. Not a valid language tag, but some fonts
 * mistakenly use it.
 */
#تعریف_کرو HB_OT_لیبل_DEFAULT_LANGUAGE	HB_لیبل ('d', 'f', 'l', 't')

/**
 * HB_OT_MAX_TAGS_PER_SCRIPT:
 *
 * Maximum number of OpenType tags that can correspond to a give #hb_script_t.
 *
 * Since: 2.0.0
 **/
#تعریف_کرو HB_OT_MAX_لیبل_فہرست_فی_SCRIPT	3u
/**
 * HB_OT_MAX_TAGS_PER_LANGUAGE:
 *
 * Maximum number of OpenType tags that can correspond to a give #hb_language_t.
 *
 * Since: 2.0.0
 **/
#تعریف_کرو HB_OT_MAX_لیبل_فہرست_فی_LANGUAGE	3u

HB_EXTERN خالی
hb_ot_لیبل_فہرست_from_script_and_language (hb_script_t   script,
				     hb_language_t language,
				     غیرعلامتی صحیح *script_گنتی /* IN/OUT */,
				     hb_لیبل_t     *script_لیبل_فہرست /*برآمد*/,
				     غیرعلامتی صحیح *language_گنتی /* IN/OUT */,
				     hb_لیبل_t     *language_لیبل_فہرست /*برآمد*/);

HB_EXTERN hb_script_t
hb_ot_لیبل_مقصد_script (hb_لیبل_t لیبل_2);

HB_EXTERN hb_language_t
hb_ot_لیبل_مقصد_language (hb_لیبل_t لیبل_2);

HB_EXTERN خالی
hb_ot_لیبل_فہرست_مقصد_script_and_language (hb_لیبل_t       script_لیبل,
				   hb_لیبل_t       language_لیبل,
				   hb_script_t   *script /*برآمد*/,
				   hb_language_t *language /*برآمد*/);


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
قسمتعریف شمار {
  HB_OT_LAYOUT_GLYPH_CLASS_UNCLASSIFIED	= 0,
  HB_OT_LAYOUT_GLYPH_CLASS_بنیاد_GLYPH	= 1,
  HB_OT_LAYOUT_GLYPH_CLASS_LIGATURE	= 2,
  HB_OT_LAYOUT_GLYPH_CLASS_MARK		= 3,
  HB_OT_LAYOUT_GLYPH_CLASS_COMPONENT	= 4
} hb_ot_layout_glyph_class_t;

HB_EXTERN hb_ot_layout_glyph_class_t
hb_ot_layout_get_glyph_class (hb_face_t      *face,
			      hb_codepoint_t  glyph);

HB_EXTERN خالی
hb_ot_layout_get_glyphs_اندر_class (hb_face_t                  *face,
				  hb_ot_layout_glyph_class_t  klass,
				  hb_سیٹ_t                   *glyphs /*برآمد*/);

/* Not that useful.  Provides list of attach points for a glyph that a
 * client may want to cache */
HB_EXTERN غیرعلامتی صحیح
hb_ot_layout_get_attach_points (hb_face_t      *face,
				hb_codepoint_t  glyph,
				غیرعلامتی صحیح    آغاز_انحراف,
				غیرعلامتی صحیح   *نقطہ_گنتی /* IN/OUT */,
				غیرعلامتی صحیح   *نقطہ_array /*برآمد*/);

/* Ligature caret positions */
HB_EXTERN غیرعلامتی صحیح
hb_ot_layout_get_ligature_carets (hb_font_t      *font,
				  hb_direction_t  direction,
				  hb_codepoint_t  glyph,
				  غیرعلامتی صحیح    آغاز_انحراف,
				  غیرعلامتی صحیح   *caret_گنتی /* IN/OUT */,
				  hb_position_t  *caret_array /*برآمد*/);


/*
 * GSUB/GPOS feature query and enumeration interface
 */

/**
 * HB_OT_LAYOUT_NO_SCRIPT_INDEX:
 *
 * Special value for script index indicating unsupported script.
 */
#تعریف_کرو HB_OT_LAYOUT_NO_SCRIPT_اشاریہ		0xFFFFu
/**
 * HB_OT_LAYOUT_NO_FEATURE_INDEX:
 *
 * Special value for feature index indicating unsupported feature.
 */
#تعریف_کرو HB_OT_LAYOUT_NO_FEATURE_اشاریہ		0xFFFFu
/**
 * HB_OT_LAYOUT_DEFAULT_LANGUAGE_INDEX:
 *
 * Special value for language index indicating default or unsupported language.
 */
#تعریف_کرو HB_OT_LAYOUT_DEFAULT_LANGUAGE_اشاریہ	0xFFFFu
/**
 * HB_OT_LAYOUT_NO_VARIATIONS_INDEX:
 *
 * Special value for variations index indicating unsupported variation.
 */
#تعریف_کرو HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ	0xFFFFFFFFu

HB_EXTERN غیرعلامتی صحیح
hb_ot_layout_جدول_get_script_لیبل_فہرست (hb_face_t    *face,
				    hb_لیبل_t      جدول_لیبل_2,
				    غیرعلامتی صحیح  آغاز_انحراف,
				    غیرعلامتی صحیح *script_گنتی /* IN/OUT */,
				    hb_لیبل_t     *script_لیبل_فہرست /*برآمد*/);

HB_EXTERN hb_bool_t
hb_ot_layout_جدول_ڈھونڈو_script (hb_face_t    *face,
				hb_لیبل_t      جدول_لیبل_2,
				hb_لیبل_t      script_لیبل,
				غیرعلامتی صحیح *script_اشاریہ /*برآمد*/);

HB_EXTERN hb_bool_t
hb_ot_layout_جدول_select_script (hb_face_t      *face,
				  hb_لیبل_t        جدول_لیبل_2,
				  غیرعلامتی صحیح    script_گنتی,
				  ثابت hb_لیبل_t *script_لیبل_فہرست,
				  غیرعلامتی صحیح   *script_اشاریہ /*برآمد*/,
				  hb_لیبل_t       *chosen_script /*برآمد*/);

HB_EXTERN غیرعلامتی صحیح
hb_ot_layout_جدول_get_feature_لیبل_فہرست (hb_face_t    *face,
				     hb_لیبل_t      جدول_لیبل_2,
				     غیرعلامتی صحیح  آغاز_انحراف,
				     غیرعلامتی صحیح *feature_گنتی_2 /* IN/OUT */,
				     hb_لیبل_t     *feature_لیبل_فہرست /*برآمد*/);

HB_EXTERN غیرعلامتی صحیح
hb_ot_layout_script_get_language_لیبل_فہرست (hb_face_t    *face,
				       hb_لیبل_t      جدول_لیبل_2,
				       غیرعلامتی صحیح  script_اشاریہ,
				       غیرعلامتی صحیح  آغاز_انحراف,
				       غیرعلامتی صحیح *language_گنتی /* IN/OUT */,
				       hb_لیبل_t     *language_لیبل_فہرست /*برآمد*/);

HB_EXTERN hb_bool_t
hb_ot_layout_script_select_language (hb_face_t      *face,
				     hb_لیبل_t        جدول_لیبل_2,
				     غیرعلامتی صحیح    script_اشاریہ,
				     غیرعلامتی صحیح    language_گنتی,
				     ثابت hb_لیبل_t *language_لیبل_فہرست,
				     غیرعلامتی صحیح   *language_اشاریہ /*برآمد*/);

HB_EXTERN hb_bool_t
hb_ot_layout_script_select_language2 (hb_face_t      *face,
				     hb_لیبل_t        جدول_لیبل_2,
				     غیرعلامتی صحیح    script_اشاریہ,
				     غیرعلامتی صحیح    language_گنتی,
				     ثابت hb_لیبل_t *language_لیبل_فہرست,
				     غیرعلامتی صحیح   *language_اشاریہ /*برآمد*/,
				     hb_لیبل_t       *chosen_language /*برآمد*/);

HB_EXTERN hb_bool_t
hb_ot_layout_language_get_required_feature_اشاریہ (hb_face_t    *face,
						  hb_لیبل_t      جدول_لیبل_2,
						  غیرعلامتی صحیح  script_اشاریہ,
						  غیرعلامتی صحیح  language_اشاریہ,
						  غیرعلامتی صحیح *feature_اشاریہ /*برآمد*/);

HB_EXTERN hb_bool_t
hb_ot_layout_language_get_required_feature (hb_face_t    *face,
					    hb_لیبل_t      جدول_لیبل_2,
					    غیرعلامتی صحیح  script_اشاریہ,
					    غیرعلامتی صحیح  language_اشاریہ,
					    غیرعلامتی صحیح *feature_اشاریہ /*برآمد*/,
					    hb_لیبل_t     *feature_لیبل /*برآمد*/);

HB_EXTERN غیرعلامتی صحیح
hb_ot_layout_language_get_feature_indexes (hb_face_t    *face,
					   hb_لیبل_t      جدول_لیبل_2,
					   غیرعلامتی صحیح  script_اشاریہ,
					   غیرعلامتی صحیح  language_اشاریہ,
					   غیرعلامتی صحیح  آغاز_انحراف,
					   غیرعلامتی صحیح *feature_گنتی_2 /* IN/OUT */,
					   غیرعلامتی صحیح *feature_indexes /*برآمد*/);

HB_EXTERN غیرعلامتی صحیح
hb_ot_layout_language_get_feature_لیبل_فہرست (hb_face_t    *face,
					hb_لیبل_t      جدول_لیبل_2,
					غیرعلامتی صحیح  script_اشاریہ,
					غیرعلامتی صحیح  language_اشاریہ,
					غیرعلامتی صحیح  آغاز_انحراف,
					غیرعلامتی صحیح *feature_گنتی_2 /* IN/OUT */,
					hb_لیبل_t     *feature_لیبل_فہرست /*برآمد*/);

HB_EXTERN hb_bool_t
hb_ot_layout_language_ڈھونڈو_feature (hb_face_t    *face,
				    hb_لیبل_t      جدول_لیبل_2,
				    غیرعلامتی صحیح  script_اشاریہ,
				    غیرعلامتی صحیح  language_اشاریہ,
				    hb_لیبل_t      feature_لیبل,
				    غیرعلامتی صحیح *feature_اشاریہ /*برآمد*/);

HB_EXTERN غیرعلامتی صحیح
hb_ot_layout_feature_get_lookups (hb_face_t    *face,
				  hb_لیبل_t      جدول_لیبل_2,
				  غیرعلامتی صحیح  feature_اشاریہ,
				  غیرعلامتی صحیح  آغاز_انحراف,
				  غیرعلامتی صحیح *lookup_گنتی /* IN/OUT */,
				  غیرعلامتی صحیح *lookup_indexes /*برآمد*/);

HB_EXTERN غیرعلامتی صحیح
hb_ot_layout_جدول_get_lookup_گنتی (hb_face_t    *face,
				     hb_لیبل_t      جدول_لیبل_2);

HB_EXTERN خالی
hb_ot_layout_collect_features (hb_face_t      *face,
			       hb_لیبل_t        جدول_لیبل_2,
			       ثابت hb_لیبل_t *scripts,
			       ثابت hb_لیبل_t *languages,
			       ثابت hb_لیبل_t *features,
			       hb_سیٹ_t       *feature_indexes /*برآمد*/);

HB_EXTERN خالی
hb_ot_layout_collect_features_map (hb_face_t      *face,
				   hb_لیبل_t        جدول_لیبل_2,
				   غیرعلامتی        script_اشاریہ,
				   غیرعلامتی        language_اشاریہ,
				   hb_map_t       *feature_map /*برآمد*/);

HB_EXTERN خالی
hb_ot_layout_collect_lookups (hb_face_t      *face,
			      hb_لیبل_t        جدول_لیبل_2,
			      ثابت hb_لیبل_t *scripts,
			      ثابت hb_لیبل_t *languages,
			      ثابت hb_لیبل_t *features,
			      hb_سیٹ_t       *lookup_indexes /*برآمد*/);

HB_EXTERN خالی
hb_ot_layout_lookup_collect_glyphs (hb_face_t    *face,
				    hb_لیبل_t      جدول_لیبل_2,
				    غیرعلامتی صحیح  lookup_اشاریہ,
				    hb_سیٹ_t     *glyphs_before, /* OUT.  May be NULL */
				    hb_سیٹ_t     *glyphs_داخلہ,  /* OUT.  May be NULL */
				    hb_سیٹ_t     *glyphs_after,  /* OUT.  May be NULL */
				    hb_سیٹ_t     *glyphs_output  /* OUT.  May be NULL */);


/* Variations support */

HB_EXTERN hb_bool_t
hb_ot_layout_جدول_ڈھونڈو_feature_variations (hb_face_t    *face,
					    hb_لیبل_t      جدول_لیبل_2,
					    ثابت صحیح    *coords,
					    غیرعلامتی صحیح  عدد_coords,
					    غیرعلامتی صحیح *variations_اشاریہ /* out */);

HB_EXTERN غیرعلامتی صحیح
hb_ot_layout_feature_with_variations_get_lookups (hb_face_t    *face,
						  hb_لیبل_t      جدول_لیبل_2,
						  غیرعلامتی صحیح  feature_اشاریہ,
						  غیرعلامتی صحیح  variations_اشاریہ,
						  غیرعلامتی صحیح  آغاز_انحراف,
						  غیرعلامتی صحیح *lookup_گنتی /* IN/OUT */,
						  غیرعلامتی صحیح *lookup_indexes /*برآمد*/);


/*
 * GSUB
 */

HB_EXTERN hb_bool_t
hb_ot_layout_has_substitution (hb_face_t *face);

HB_EXTERN غیرعلامتی
hb_ot_layout_lookup_get_glyph_alternates (hb_face_t      *face,
					  غیرعلامتی        lookup_اشاریہ,
					  hb_codepoint_t  glyph,
					  غیرعلامتی        آغاز_انحراف,
					  غیرعلامتی       *alternate_گنتی /* IN/OUT */,
					  hb_codepoint_t *alternate_glyphs /*برآمد*/);

HB_EXTERN hb_bool_t
hb_ot_layout_lookup_collect_glyph_alternates (hb_face_t *face,
					      غیرعلامتی   lookup_اشاریہ,
					      hb_map_t  *alternate_گنتی /* IN/OUT */,
					      hb_map_t  *alternate_glyphs /* IN/OUT */);

HB_EXTERN hb_bool_t
hb_ot_layout_lookup_would_substitute (hb_face_t            *face,
				      غیرعلامتی صحیح          lookup_اشاریہ,
				      ثابت hb_codepoint_t *glyphs,
				      غیرعلامتی صحیح          glyphs_لمبائی,
				      hb_bool_t             zero_context);

HB_EXTERN خالی
hb_ot_layout_lookup_substitute_احاطہ (hb_face_t    *face,
					غیرعلامتی صحیح  lookup_اشاریہ,
					hb_سیٹ_t     *glyphs
					/*TODO , hb_bool_t  inclusive */);

HB_EXTERN خالی
hb_ot_layout_lookups_substitute_احاطہ (hb_face_t      *face,
					 ثابت hb_سیٹ_t *lookups,
					 hb_سیٹ_t       *glyphs);


/*
 * GPOS
 */

HB_EXTERN hb_bool_t
hb_ot_layout_has_positioning (hb_face_t *face);

/* Optical 'size' feature info.  Returns true if found.
 * https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#size */
HB_EXTERN hb_bool_t
hb_ot_layout_get_سائز_params (hb_face_t       *face,
			      غیرعلامتی صحیح    *design_سائز_2,       /* OUT.  May be NULL */
			      غیرعلامتی صحیح    *subfamily_id,      /* OUT.  May be NULL */
			      hb_ot_name_id_t *subfamily_name_id, /* OUT.  May be NULL */
			      غیرعلامتی صحیح    *range_آغاز_2,       /* OUT.  May be NULL */
			      غیرعلامتی صحیح    *range_انتہا_2          /* OUT.  May be NULL */);

HB_EXTERN hb_position_t
hb_ot_layout_lookup_get_optical_bound (hb_font_t      *font,
				       غیرعلامتی        lookup_اشاریہ,
				       hb_direction_t  direction,
				       hb_codepoint_t  glyph);


/*
 * GSUB/GPOS
 */

HB_EXTERN hb_bool_t
hb_ot_layout_feature_get_name_ids (hb_face_t       *face,
				   hb_لیبل_t         جدول_لیبل_2,
				   غیرعلامتی صحیح     feature_اشاریہ,
				   hb_ot_name_id_t *label_id             /* OUT.  May be NULL */,
				   hb_ot_name_id_t *tooltip_id           /* OUT.  May be NULL */,
				   hb_ot_name_id_t *sample_id            /* OUT.  May be NULL */,
				   غیرعلامتی صحیح    *عدد_named_parameters /* OUT.  May be NULL */,
				   hb_ot_name_id_t *پہلا_param_id       /* OUT.  May be NULL */);


HB_EXTERN غیرعلامتی صحیح
hb_ot_layout_feature_get_characters (hb_face_t      *face,
				     hb_لیبل_t        جدول_لیبل_2,
				     غیرعلامتی صحیح    feature_اشاریہ,
				     غیرعلامتی صحیح    آغاز_انحراف,
				     غیرعلامتی صحیح   *char_گنتی    /* IN/OUT.  May be NULL */,
				     hb_codepoint_t *characters    /* OUT.     May be NULL */);


/*
 * BASE
 */

HB_EXTERN hb_bool_t
hb_ot_layout_get_font_extents (hb_font_t         *font,
			       hb_direction_t     direction,
			       hb_لیبل_t           script_لیبل,
			       hb_لیبل_t           language_لیبل,
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
قسمتعریف شمار {
  HB_OT_LAYOUT_BASELINE_لیبل_ROMAN			= HB_لیبل ('r','o','m','n'),
  HB_OT_LAYOUT_BASELINE_لیبل_HANGING			= HB_لیبل ('h','a','n','g'),
  HB_OT_LAYOUT_BASELINE_لیبل_IDEO_FACE_BOTTOM_OR_بایاں	= HB_لیبل ('i','c','f','b'),
  HB_OT_LAYOUT_BASELINE_لیبل_IDEO_FACE_چوٹی_OR_دایاں	= HB_لیبل ('i','c','f','t'),
  HB_OT_LAYOUT_BASELINE_لیبل_IDEO_FACE_CENTRAL		= HB_لیبل ('I','c','f','c'),
  HB_OT_LAYOUT_BASELINE_لیبل_IDEO_EMBOX_BOTTOM_OR_بایاں	= HB_لیبل ('i','d','e','o'),
  HB_OT_LAYOUT_BASELINE_لیبل_IDEO_EMBOX_چوٹی_OR_دایاں	= HB_لیبل ('i','d','t','p'),
  HB_OT_LAYOUT_BASELINE_لیبل_IDEO_EMBOX_CENTRAL		= HB_لیبل ('I','d','c','e'),
  HB_OT_LAYOUT_BASELINE_لیبل_MATH			= HB_لیبل ('m','a','t','h'),

  /*< private >*/
  HB_OT_LAYOUT_BASELINE_لیبل_MAX_قدر = HB_لیبل_MAX_SIGNED /*< skip >*/
} hb_ot_layout_baseline_لیبل_t;

HB_EXTERN hb_ot_layout_baseline_لیبل_t
hb_ot_layout_get_horizontal_baseline_لیبل_for_script (hb_script_t script);

HB_EXTERN hb_bool_t
hb_ot_layout_get_baseline (hb_font_t                   *font,
			   hb_ot_layout_baseline_لیبل_t  baseline_لیبل,
			   hb_direction_t               direction,
			   hb_لیبل_t                     script_لیبل,
			   hb_لیبل_t                     language_لیبل,
			   hb_position_t               *coord        /* OUT.  May be NULL. */);

HB_EXTERN hb_bool_t
hb_ot_layout_get_baseline2 (hb_font_t                   *font,
			    hb_ot_layout_baseline_لیبل_t  baseline_لیبل,
			    hb_direction_t               direction,
			    hb_script_t                  script,
			    hb_language_t                language,
			    hb_position_t               *coord        /* OUT.  May be NULL. */);

HB_EXTERN خالی
hb_ot_layout_get_baseline_with_fallback (hb_font_t                   *font,
					 hb_ot_layout_baseline_لیبل_t  baseline_لیبل,
					 hb_direction_t               direction,
					 hb_لیبل_t                     script_لیبل,
					 hb_لیبل_t                     language_لیبل,
					 hb_position_t               *coord        /*برآمد*/);

HB_EXTERN خالی
hb_ot_layout_get_baseline_with_fallback2 (hb_font_t                   *font,
					  hb_ot_layout_baseline_لیبل_t  baseline_لیبل,
					  hb_direction_t               direction,
					  hb_script_t                  script,
					  hb_language_t                language,
					  hb_position_t               *coord        /*برآمد*/);

HB_انتہا_DECLS

#اگر_ختم /* HB_OT_LAYOUT_H */

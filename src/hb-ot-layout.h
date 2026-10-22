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

#إذا !defined(HB_OT_H_وچ) && !defined(HB_NO_SINGLE_سرخی_ERROR)
#غلطی "Include <hb-ot.h> instead."
#جے_انت

#جے_بنا_تعریف HB_OT_LAYOUT_H
#تعریف_کرو HB_OT_LAYOUT_H

#ضمن "hb.h"

#ضمن "hb-ot-name.h"

HB_BEGIN_DECLS


/**
 * HB_OT_TAG_BASE:
 *
 * OpenType [Baseline Table](https://docs.microsoft.com/en-us/typography/opentype/spec/base).
 */
#تعریف_کرو HB_OT_ٹیگ_ادھار HB_ٹیگ('B','A','S','E')
/**
 * HB_OT_TAG_GDEF:
 *
 * OpenType [Glyph Definition Table](https://docs.microsoft.com/en-us/typography/opentype/spec/gdef).
 */
#تعریف_کرو HB_OT_ٹیگ_GDEF HB_ٹیگ('G','D','E','F')
/**
 * HB_OT_TAG_GSUB:
 *
 * OpenType [Glyph Substitution Table](https://docs.microsoft.com/en-us/typography/opentype/spec/gsub).
 */
#تعریف_کرو HB_OT_ٹیگ_GSUB HB_ٹیگ('G','S','U','B')
/**
 * HB_OT_TAG_GPOS:
 *
 * OpenType [Glyph Positioning Table](https://docs.microsoft.com/en-us/typography/opentype/spec/gpos).
 */
#تعریف_کرو HB_OT_ٹیگ_GPOS HB_ٹیگ('G','P','O','S')
/**
 * HB_OT_TAG_JSTF:
 *
 * OpenType [Justification Table](https://docs.microsoft.com/en-us/typography/opentype/spec/jstf).
 */
#تعریف_کرو HB_OT_ٹیگ_JSTF HB_ٹیگ('J','S','T','F')


/*
 * Script & Language tags.
 */

/**
 * HB_OT_TAG_DEFAULT_SCRIPT:
 *
 * OpenType script tag, `DFLT`, for features that are not script-specific.
 *
 */
#تعریف_کرو HB_OT_ٹیگ_DEFAULT_SCRIPT	HB_ٹیگ ('D', 'F', 'L', 'T')
/**
 * HB_OT_TAG_DEFAULT_LANGUAGE:
 *
 * OpenType language tag, `dflt`. Not a valid language tag, but some fonts
 * mistakenly use it.
 */
#تعریف_کرو HB_OT_ٹیگ_DEFAULT_LANGUAGE	HB_ٹیگ ('d', 'f', 'l', 't')

/**
 * HB_OT_MAX_TAGS_PER_SCRIPT:
 *
 * Maximum number of OpenType tags that can correspond to a give #hb_script_t.
 *
 * Since: 2.0.0
 **/
#تعریف_کرو HB_OT_MAX_ٹیگ_سوچی_فی_SCRIPT	3u
/**
 * HB_OT_MAX_TAGS_PER_LANGUAGE:
 *
 * Maximum number of OpenType tags that can correspond to a give #hb_language_t.
 *
 * Since: 2.0.0
 **/
#تعریف_کرو HB_OT_MAX_ٹیگ_سوچی_فی_LANGUAGE	3u

HB_EXTERN فراغ
hb_ot_ٹیگ_سوچی_from_script_and_language (hb_script_t   script,
				     hb_language_t language,
				     بنانشانی صحيح *script_گنتی /* IN/OUT */,
				     hb_ٹیگ_t     *script_ٹیگ_سوچی /*باہرلا*/,
				     بنانشانی صحيح *language_گنتی /* IN/OUT */,
				     hb_ٹیگ_t     *language_ٹیگ_سوچی /*باہرلا*/);

HB_EXTERN hb_script_t
hb_ot_ٹیگ_منزل_script (hb_ٹیگ_t ٹیگ_2);

HB_EXTERN hb_language_t
hb_ot_ٹیگ_منزل_language (hb_ٹیگ_t ٹیگ_2);

HB_EXTERN فراغ
hb_ot_ٹیگ_سوچی_منزل_script_and_language (hb_ٹیگ_t       script_ٹیگ,
				   hb_ٹیگ_t       language_ٹیگ,
				   hb_script_t   *script /*باہرلا*/,
				   hb_language_t *language /*باہرلا*/);


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
  HB_OT_LAYOUT_GLYPH_CLASS_ادھار_GLYPH	= 1,
  HB_OT_LAYOUT_GLYPH_CLASS_LIGATURE	= 2,
  HB_OT_LAYOUT_GLYPH_CLASS_MARK		= 3,
  HB_OT_LAYOUT_GLYPH_CLASS_COMPONENT	= 4
} hb_ot_layout_glyph_class_t;

HB_EXTERN hb_ot_layout_glyph_class_t
hb_ot_layout_get_glyph_class (hb_face_t      *face,
			      hb_codepoint_t  glyph);

HB_EXTERN فراغ
hb_ot_layout_get_glyphs_وچ_class (hb_face_t                  *face,
				  hb_ot_layout_glyph_class_t  klass,
				  hb_سیٹ_t                   *glyphs /*باہرلا*/);

/* Not that useful.  Provides list of attach points for a glyph that a
 * client may want to cache */
HB_EXTERN بنانشانی صحيح
hb_ot_layout_get_attach_points (hb_face_t      *face,
				hb_codepoint_t  glyph,
				بنانشانی صحيح    شروع_وتھ,
				بنانشانی صحيح   *نقطہ_گنتی /* IN/OUT */,
				بنانشانی صحيح   *نقطہ_array /*باہرلا*/);

/* Ligature caret positions */
HB_EXTERN بنانشانی صحيح
hb_ot_layout_get_ligature_carets (hb_font_t      *font,
				  hb_direction_t  direction,
				  hb_codepoint_t  glyph,
				  بنانشانی صحيح    شروع_وتھ,
				  بنانشانی صحيح   *caret_گنتی /* IN/OUT */,
				  hb_position_t  *caret_array /*باہرلا*/);


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

HB_EXTERN بنانشانی صحيح
hb_ot_layout_جدول_get_script_ٹیگ_سوچی (hb_face_t    *face,
				    hb_ٹیگ_t      جدول_ٹیگ_2,
				    بنانشانی صحيح  شروع_وتھ,
				    بنانشانی صحيح *script_گنتی /* IN/OUT */,
				    hb_ٹیگ_t     *script_ٹیگ_سوچی /*باہرلا*/);

HB_EXTERN hb_bool_t
hb_ot_layout_جدول_لبھو_script (hb_face_t    *face,
				hb_ٹیگ_t      جدول_ٹیگ_2,
				hb_ٹیگ_t      script_ٹیگ,
				بنانشانی صحيح *script_اشاریہ /*باہرلا*/);

HB_EXTERN hb_bool_t
hb_ot_layout_جدول_select_script (hb_face_t      *face,
				  hb_ٹیگ_t        جدول_ٹیگ_2,
				  بنانشانی صحيح    script_گنتی,
				  ثابت hb_ٹیگ_t *script_ٹیگ_سوچی,
				  بنانشانی صحيح   *script_اشاریہ /*باہرلا*/,
				  hb_ٹیگ_t       *chosen_script /*باہرلا*/);

HB_EXTERN بنانشانی صحيح
hb_ot_layout_جدول_get_feature_ٹیگ_سوچی (hb_face_t    *face,
				     hb_ٹیگ_t      جدول_ٹیگ_2,
				     بنانشانی صحيح  شروع_وتھ,
				     بنانشانی صحيح *feature_گنتی_2 /* IN/OUT */,
				     hb_ٹیگ_t     *feature_ٹیگ_سوچی /*باہرلا*/);

HB_EXTERN بنانشانی صحيح
hb_ot_layout_script_get_language_ٹیگ_سوچی (hb_face_t    *face,
				       hb_ٹیگ_t      جدول_ٹیگ_2,
				       بنانشانی صحيح  script_اشاریہ,
				       بنانشانی صحيح  شروع_وتھ,
				       بنانشانی صحيح *language_گنتی /* IN/OUT */,
				       hb_ٹیگ_t     *language_ٹیگ_سوچی /*باہرلا*/);

HB_EXTERN hb_bool_t
hb_ot_layout_script_select_language (hb_face_t      *face,
				     hb_ٹیگ_t        جدول_ٹیگ_2,
				     بنانشانی صحيح    script_اشاریہ,
				     بنانشانی صحيح    language_گنتی,
				     ثابت hb_ٹیگ_t *language_ٹیگ_سوچی,
				     بنانشانی صحيح   *language_اشاریہ /*باہرلا*/);

HB_EXTERN hb_bool_t
hb_ot_layout_script_select_language2 (hb_face_t      *face,
				     hb_ٹیگ_t        جدول_ٹیگ_2,
				     بنانشانی صحيح    script_اشاریہ,
				     بنانشانی صحيح    language_گنتی,
				     ثابت hb_ٹیگ_t *language_ٹیگ_سوچی,
				     بنانشانی صحيح   *language_اشاریہ /*باہرلا*/,
				     hb_ٹیگ_t       *chosen_language /*باہرلا*/);

HB_EXTERN hb_bool_t
hb_ot_layout_language_get_required_feature_اشاریہ (hb_face_t    *face,
						  hb_ٹیگ_t      جدول_ٹیگ_2,
						  بنانشانی صحيح  script_اشاریہ,
						  بنانشانی صحيح  language_اشاریہ,
						  بنانشانی صحيح *feature_اشاریہ /*باہرلا*/);

HB_EXTERN hb_bool_t
hb_ot_layout_language_get_required_feature (hb_face_t    *face,
					    hb_ٹیگ_t      جدول_ٹیگ_2,
					    بنانشانی صحيح  script_اشاریہ,
					    بنانشانی صحيح  language_اشاریہ,
					    بنانشانی صحيح *feature_اشاریہ /*باہرلا*/,
					    hb_ٹیگ_t     *feature_ٹیگ /*باہرلا*/);

HB_EXTERN بنانشانی صحيح
hb_ot_layout_language_get_feature_indexes (hb_face_t    *face,
					   hb_ٹیگ_t      جدول_ٹیگ_2,
					   بنانشانی صحيح  script_اشاریہ,
					   بنانشانی صحيح  language_اشاریہ,
					   بنانشانی صحيح  شروع_وتھ,
					   بنانشانی صحيح *feature_گنتی_2 /* IN/OUT */,
					   بنانشانی صحيح *feature_indexes /*باہرلا*/);

HB_EXTERN بنانشانی صحيح
hb_ot_layout_language_get_feature_ٹیگ_سوچی (hb_face_t    *face,
					hb_ٹیگ_t      جدول_ٹیگ_2,
					بنانشانی صحيح  script_اشاریہ,
					بنانشانی صحيح  language_اشاریہ,
					بنانشانی صحيح  شروع_وتھ,
					بنانشانی صحيح *feature_گنتی_2 /* IN/OUT */,
					hb_ٹیگ_t     *feature_ٹیگ_سوچی /*باہرلا*/);

HB_EXTERN hb_bool_t
hb_ot_layout_language_لبھو_feature (hb_face_t    *face,
				    hb_ٹیگ_t      جدول_ٹیگ_2,
				    بنانشانی صحيح  script_اشاریہ,
				    بنانشانی صحيح  language_اشاریہ,
				    hb_ٹیگ_t      feature_ٹیگ,
				    بنانشانی صحيح *feature_اشاریہ /*باہرلا*/);

HB_EXTERN بنانشانی صحيح
hb_ot_layout_feature_get_lookups (hb_face_t    *face,
				  hb_ٹیگ_t      جدول_ٹیگ_2,
				  بنانشانی صحيح  feature_اشاریہ,
				  بنانشانی صحيح  شروع_وتھ,
				  بنانشانی صحيح *lookup_گنتی /* IN/OUT */,
				  بنانشانی صحيح *lookup_indexes /*باہرلا*/);

HB_EXTERN بنانشانی صحيح
hb_ot_layout_جدول_get_lookup_گنتی (hb_face_t    *face,
				     hb_ٹیگ_t      جدول_ٹیگ_2);

HB_EXTERN فراغ
hb_ot_layout_collect_features (hb_face_t      *face,
			       hb_ٹیگ_t        جدول_ٹیگ_2,
			       ثابت hb_ٹیگ_t *scripts,
			       ثابت hb_ٹیگ_t *languages,
			       ثابت hb_ٹیگ_t *features,
			       hb_سیٹ_t       *feature_indexes /*باہرلا*/);

HB_EXTERN فراغ
hb_ot_layout_collect_features_map (hb_face_t      *face,
				   hb_ٹیگ_t        جدول_ٹیگ_2,
				   بنانشانی        script_اشاریہ,
				   بنانشانی        language_اشاریہ,
				   hb_map_t       *feature_map /*باہرلا*/);

HB_EXTERN فراغ
hb_ot_layout_collect_lookups (hb_face_t      *face,
			      hb_ٹیگ_t        جدول_ٹیگ_2,
			      ثابت hb_ٹیگ_t *scripts,
			      ثابت hb_ٹیگ_t *languages,
			      ثابت hb_ٹیگ_t *features,
			      hb_سیٹ_t       *lookup_indexes /*باہرلا*/);

HB_EXTERN فراغ
hb_ot_layout_lookup_collect_glyphs (hb_face_t    *face,
				    hb_ٹیگ_t      جدول_ٹیگ_2,
				    بنانشانی صحيح  lookup_اشاریہ,
				    hb_سیٹ_t     *glyphs_before, /* OUT.  May be NULL */
				    hb_سیٹ_t     *glyphs_داخلہ,  /* OUT.  May be NULL */
				    hb_سیٹ_t     *glyphs_after,  /* OUT.  May be NULL */
				    hb_سیٹ_t     *glyphs_output  /* OUT.  May be NULL */);


/* Variations support */

HB_EXTERN hb_bool_t
hb_ot_layout_جدول_لبھو_feature_variations (hb_face_t    *face,
					    hb_ٹیگ_t      جدول_ٹیگ_2,
					    ثابت صحيح    *coords,
					    بنانشانی صحيح  عدد_coords,
					    بنانشانی صحيح *variations_اشاریہ /* out */);

HB_EXTERN بنانشانی صحيح
hb_ot_layout_feature_with_variations_get_lookups (hb_face_t    *face,
						  hb_ٹیگ_t      جدول_ٹیگ_2,
						  بنانشانی صحيح  feature_اشاریہ,
						  بنانشانی صحيح  variations_اشاریہ,
						  بنانشانی صحيح  شروع_وتھ,
						  بنانشانی صحيح *lookup_گنتی /* IN/OUT */,
						  بنانشانی صحيح *lookup_indexes /*باہرلا*/);


/*
 * GSUB
 */

HB_EXTERN hb_bool_t
hb_ot_layout_has_substitution (hb_face_t *face);

HB_EXTERN بنانشانی
hb_ot_layout_lookup_get_glyph_alternates (hb_face_t      *face,
					  بنانشانی        lookup_اشاریہ,
					  hb_codepoint_t  glyph,
					  بنانشانی        شروع_وتھ,
					  بنانشانی       *alternate_گنتی /* IN/OUT */,
					  hb_codepoint_t *alternate_glyphs /*باہرلا*/);

HB_EXTERN hb_bool_t
hb_ot_layout_lookup_collect_glyph_alternates (hb_face_t *face,
					      بنانشانی   lookup_اشاریہ,
					      hb_map_t  *alternate_گنتی /* IN/OUT */,
					      hb_map_t  *alternate_glyphs /* IN/OUT */);

HB_EXTERN hb_bool_t
hb_ot_layout_lookup_would_substitute (hb_face_t            *face,
				      بنانشانی صحيح          lookup_اشاریہ,
				      ثابت hb_codepoint_t *glyphs,
				      بنانشانی صحيح          glyphs_لمائی,
				      hb_bool_t             zero_context);

HB_EXTERN فراغ
hb_ot_layout_lookup_substitute_گھیرا (hb_face_t    *face,
					بنانشانی صحيح  lookup_اشاریہ,
					hb_سیٹ_t     *glyphs
					/*TODO , hb_bool_t  inclusive */);

HB_EXTERN فراغ
hb_ot_layout_lookups_substitute_گھیرا (hb_face_t      *face,
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
hb_ot_layout_get_ناپ_params (hb_face_t       *face,
			      بنانشانی صحيح    *design_ناپ_2,       /* OUT.  May be NULL */
			      بنانشانی صحيح    *subfamily_id,      /* OUT.  May be NULL */
			      hb_ot_name_id_t *subfamily_name_id, /* OUT.  May be NULL */
			      بنانشانی صحيح    *range_شروع_2,       /* OUT.  May be NULL */
			      بنانشانی صحيح    *range_سرا_2          /* OUT.  May be NULL */);

HB_EXTERN hb_position_t
hb_ot_layout_lookup_get_optical_bound (hb_font_t      *font,
				       بنانشانی        lookup_اشاریہ,
				       hb_direction_t  direction,
				       hb_codepoint_t  glyph);


/*
 * GSUB/GPOS
 */

HB_EXTERN hb_bool_t
hb_ot_layout_feature_get_name_ids (hb_face_t       *face,
				   hb_ٹیگ_t         جدول_ٹیگ_2,
				   بنانشانی صحيح     feature_اشاریہ,
				   hb_ot_name_id_t *label_id             /* OUT.  May be NULL */,
				   hb_ot_name_id_t *tooltip_id           /* OUT.  May be NULL */,
				   hb_ot_name_id_t *sample_id            /* OUT.  May be NULL */,
				   بنانشانی صحيح    *عدد_named_parameters /* OUT.  May be NULL */,
				   hb_ot_name_id_t *پہلا_param_id       /* OUT.  May be NULL */);


HB_EXTERN بنانشانی صحيح
hb_ot_layout_feature_get_characters (hb_face_t      *face,
				     hb_ٹیگ_t        جدول_ٹیگ_2,
				     بنانشانی صحيح    feature_اشاریہ,
				     بنانشانی صحيح    شروع_وتھ,
				     بنانشانی صحيح   *char_گنتی    /* IN/OUT.  May be NULL */,
				     hb_codepoint_t *characters    /* OUT.     May be NULL */);


/*
 * BASE
 */

HB_EXTERN hb_bool_t
hb_ot_layout_get_font_extents (hb_font_t         *font,
			       hb_direction_t     direction,
			       hb_ٹیگ_t           script_ٹیگ,
			       hb_ٹیگ_t           language_ٹیگ,
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
  HB_OT_LAYOUT_BASELINE_ٹیگ_ROMAN			= HB_ٹیگ ('r','o','m','n'),
  HB_OT_LAYOUT_BASELINE_ٹیگ_HANGING			= HB_ٹیگ ('h','a','n','g'),
  HB_OT_LAYOUT_BASELINE_ٹیگ_IDEO_FACE_BOTTOM_OR_بایاں	= HB_ٹیگ ('i','c','f','b'),
  HB_OT_LAYOUT_BASELINE_ٹیگ_IDEO_FACE_چوٹی_OR_دایاں	= HB_ٹیگ ('i','c','f','t'),
  HB_OT_LAYOUT_BASELINE_ٹیگ_IDEO_FACE_CENTRAL		= HB_ٹیگ ('I','c','f','c'),
  HB_OT_LAYOUT_BASELINE_ٹیگ_IDEO_EMBOX_BOTTOM_OR_بایاں	= HB_ٹیگ ('i','d','e','o'),
  HB_OT_LAYOUT_BASELINE_ٹیگ_IDEO_EMBOX_چوٹی_OR_دایاں	= HB_ٹیگ ('i','d','t','p'),
  HB_OT_LAYOUT_BASELINE_ٹیگ_IDEO_EMBOX_CENTRAL		= HB_ٹیگ ('I','d','c','e'),
  HB_OT_LAYOUT_BASELINE_ٹیگ_MATH			= HB_ٹیگ ('m','a','t','h'),

  /*< private >*/
  HB_OT_LAYOUT_BASELINE_ٹیگ_MAX_قدر = HB_ٹیگ_MAX_SIGNED /*< skip >*/
} hb_ot_layout_baseline_ٹیگ_t;

HB_EXTERN hb_ot_layout_baseline_ٹیگ_t
hb_ot_layout_get_horizontal_baseline_ٹیگ_for_script (hb_script_t script);

HB_EXTERN hb_bool_t
hb_ot_layout_get_baseline (hb_font_t                   *font,
			   hb_ot_layout_baseline_ٹیگ_t  baseline_ٹیگ,
			   hb_direction_t               direction,
			   hb_ٹیگ_t                     script_ٹیگ,
			   hb_ٹیگ_t                     language_ٹیگ,
			   hb_position_t               *coord        /* OUT.  May be NULL. */);

HB_EXTERN hb_bool_t
hb_ot_layout_get_baseline2 (hb_font_t                   *font,
			    hb_ot_layout_baseline_ٹیگ_t  baseline_ٹیگ,
			    hb_direction_t               direction,
			    hb_script_t                  script,
			    hb_language_t                language,
			    hb_position_t               *coord        /* OUT.  May be NULL. */);

HB_EXTERN فراغ
hb_ot_layout_get_baseline_with_fallback (hb_font_t                   *font,
					 hb_ot_layout_baseline_ٹیگ_t  baseline_ٹیگ,
					 hb_direction_t               direction,
					 hb_ٹیگ_t                     script_ٹیگ,
					 hb_ٹیگ_t                     language_ٹیگ,
					 hb_position_t               *coord        /*باہرلا*/);

HB_EXTERN فراغ
hb_ot_layout_get_baseline_with_fallback2 (hb_font_t                   *font,
					  hb_ot_layout_baseline_ٹیگ_t  baseline_ٹیگ,
					  hb_direction_t               direction,
					  hb_script_t                  script,
					  hb_language_t                language,
					  hb_position_t               *coord        /*باہرلا*/);

HB_سرا_DECLS

#جے_انت /* HB_OT_LAYOUT_H */

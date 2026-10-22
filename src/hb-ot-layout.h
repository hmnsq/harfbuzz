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

#إذا !defined(HB_OT_H_اندريون) && !defined(HB_NO_SINGLE_سرو_ERROR)
#غلطي "Include <hb-ot.h> instead."
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_OT_LAYOUT_H
#وصف_ڪريو HB_OT_LAYOUT_H

#ضمن "hb.h"

#ضمن "hb-ot-name.h"

HB_BEGIN_DECLS


/**
 * HB_OT_TAG_BASE:
 *
 * OpenType [Baseline Table](https://docs.microsoft.com/en-us/typography/opentype/spec/base).
 */
#وصف_ڪريو HB_OT_ٽيگ_بنياد HB_ٽيگ('B','A','S','E')
/**
 * HB_OT_TAG_GDEF:
 *
 * OpenType [Glyph Definition Table](https://docs.microsoft.com/en-us/typography/opentype/spec/gdef).
 */
#وصف_ڪريو HB_OT_ٽيگ_GDEF HB_ٽيگ('G','D','E','F')
/**
 * HB_OT_TAG_GSUB:
 *
 * OpenType [Glyph Substitution Table](https://docs.microsoft.com/en-us/typography/opentype/spec/gsub).
 */
#وصف_ڪريو HB_OT_ٽيگ_GSUB HB_ٽيگ('G','S','U','B')
/**
 * HB_OT_TAG_GPOS:
 *
 * OpenType [Glyph Positioning Table](https://docs.microsoft.com/en-us/typography/opentype/spec/gpos).
 */
#وصف_ڪريو HB_OT_ٽيگ_GPOS HB_ٽيگ('G','P','O','S')
/**
 * HB_OT_TAG_JSTF:
 *
 * OpenType [Justification Table](https://docs.microsoft.com/en-us/typography/opentype/spec/jstf).
 */
#وصف_ڪريو HB_OT_ٽيگ_JSTF HB_ٽيگ('J','S','T','F')


/*
 * Script & Language tags.
 */

/**
 * HB_OT_TAG_DEFAULT_SCRIPT:
 *
 * OpenType script tag, `DFLT`, for features that are not script-specific.
 *
 */
#وصف_ڪريو HB_OT_ٽيگ_DEFAULT_SCRIPT	HB_ٽيگ ('D', 'F', 'L', 'T')
/**
 * HB_OT_TAG_DEFAULT_LANGUAGE:
 *
 * OpenType language tag, `dflt`. Not a valid language tag, but some fonts
 * mistakenly use it.
 */
#وصف_ڪريو HB_OT_ٽيگ_DEFAULT_LANGUAGE	HB_ٽيگ ('d', 'f', 'l', 't')

/**
 * HB_OT_MAX_TAGS_PER_SCRIPT:
 *
 * Maximum number of OpenType tags that can correspond to a give #hb_script_t.
 *
 * Since: 2.0.0
 **/
#وصف_ڪريو HB_OT_MAX_ٽيگ_فهرست_في_SCRIPT	3u
/**
 * HB_OT_MAX_TAGS_PER_LANGUAGE:
 *
 * Maximum number of OpenType tags that can correspond to a give #hb_language_t.
 *
 * Since: 2.0.0
 **/
#وصف_ڪريو HB_OT_MAX_ٽيگ_فهرست_في_LANGUAGE	3u

HB_EXTERN فراغ
hb_ot_ٽيگ_فهرست_from_script_and_language (hb_script_t   script,
				     hb_language_t language,
				     بنانشان صحيح *script_ڳڻپ /* IN/OUT */,
				     hb_ٽيگ_t     *script_ٽيگ_فهرست /*ٻاهريون*/,
				     بنانشان صحيح *language_ڳڻپ /* IN/OUT */,
				     hb_ٽيگ_t     *language_ٽيگ_فهرست /*ٻاهريون*/);

HB_EXTERN hb_script_t
hb_ot_ٽيگ_منزل_script (hb_ٽيگ_t ٽيگ_2);

HB_EXTERN hb_language_t
hb_ot_ٽيگ_منزل_language (hb_ٽيگ_t ٽيگ_2);

HB_EXTERN فراغ
hb_ot_ٽيگ_فهرست_منزل_script_and_language (hb_ٽيگ_t       script_ٽيگ,
				   hb_ٽيگ_t       language_ٽيگ,
				   hb_script_t   *script /*ٻاهريون*/,
				   hb_language_t *language /*ٻاهريون*/);


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
  HB_OT_LAYOUT_GLYPH_CLASS_بنياد_GLYPH	= 1,
  HB_OT_LAYOUT_GLYPH_CLASS_LIGATURE	= 2,
  HB_OT_LAYOUT_GLYPH_CLASS_MARK		= 3,
  HB_OT_LAYOUT_GLYPH_CLASS_COMPONENT	= 4
} hb_ot_layout_glyph_class_t;

HB_EXTERN hb_ot_layout_glyph_class_t
hb_ot_layout_get_glyph_class (hb_face_t      *face,
			      hb_codepoint_t  glyph);

HB_EXTERN فراغ
hb_ot_layout_get_glyphs_اندريون_class (hb_face_t                  *face,
				  hb_ot_layout_glyph_class_t  klass,
				  hb_سيٽ_t                   *glyphs /*ٻاهريون*/);

/* Not that useful.  Provides list of attach points for a glyph that a
 * client may want to cache */
HB_EXTERN بنانشان صحيح
hb_ot_layout_get_attach_points (hb_face_t      *face,
				hb_codepoint_t  glyph,
				بنانشان صحيح    شروعات_وٿي,
				بنانشان صحيح   *نقطو_ڳڻپ /* IN/OUT */,
				بنانشان صحيح   *نقطو_array /*ٻاهريون*/);

/* Ligature caret positions */
HB_EXTERN بنانشان صحيح
hb_ot_layout_get_ligature_carets (hb_font_t      *font,
				  hb_direction_t  direction,
				  hb_codepoint_t  glyph,
				  بنانشان صحيح    شروعات_وٿي,
				  بنانشان صحيح   *caret_ڳڻپ /* IN/OUT */,
				  hb_position_t  *caret_array /*ٻاهريون*/);


/*
 * GSUB/GPOS feature query and enumeration interface
 */

/**
 * HB_OT_LAYOUT_NO_SCRIPT_INDEX:
 *
 * Special value for script index indicating unsupported script.
 */
#وصف_ڪريو HB_OT_LAYOUT_NO_SCRIPT_اشاريو		0xFFFFu
/**
 * HB_OT_LAYOUT_NO_FEATURE_INDEX:
 *
 * Special value for feature index indicating unsupported feature.
 */
#وصف_ڪريو HB_OT_LAYOUT_NO_FEATURE_اشاريو		0xFFFFu
/**
 * HB_OT_LAYOUT_DEFAULT_LANGUAGE_INDEX:
 *
 * Special value for language index indicating default or unsupported language.
 */
#وصف_ڪريو HB_OT_LAYOUT_DEFAULT_LANGUAGE_اشاريو	0xFFFFu
/**
 * HB_OT_LAYOUT_NO_VARIATIONS_INDEX:
 *
 * Special value for variations index indicating unsupported variation.
 */
#وصف_ڪريو HB_OT_LAYOUT_NO_VARIATIONS_اشاريو	0xFFFFFFFFu

HB_EXTERN بنانشان صحيح
hb_ot_layout_جدول_get_script_ٽيگ_فهرست (hb_face_t    *face,
				    hb_ٽيگ_t      جدول_ٽيگ_2,
				    بنانشان صحيح  شروعات_وٿي,
				    بنانشان صحيح *script_ڳڻپ /* IN/OUT */,
				    hb_ٽيگ_t     *script_ٽيگ_فهرست /*ٻاهريون*/);

HB_EXTERN hb_bool_t
hb_ot_layout_جدول_ڳوليو_script (hb_face_t    *face,
				hb_ٽيگ_t      جدول_ٽيگ_2,
				hb_ٽيگ_t      script_ٽيگ,
				بنانشان صحيح *script_اشاريو /*ٻاهريون*/);

HB_EXTERN hb_bool_t
hb_ot_layout_جدول_select_script (hb_face_t      *face,
				  hb_ٽيگ_t        جدول_ٽيگ_2,
				  بنانشان صحيح    script_ڳڻپ,
				  ثابت hb_ٽيگ_t *script_ٽيگ_فهرست,
				  بنانشان صحيح   *script_اشاريو /*ٻاهريون*/,
				  hb_ٽيگ_t       *chosen_script /*ٻاهريون*/);

HB_EXTERN بنانشان صحيح
hb_ot_layout_جدول_get_feature_ٽيگ_فهرست (hb_face_t    *face,
				     hb_ٽيگ_t      جدول_ٽيگ_2,
				     بنانشان صحيح  شروعات_وٿي,
				     بنانشان صحيح *feature_ڳڻپ_2 /* IN/OUT */,
				     hb_ٽيگ_t     *feature_ٽيگ_فهرست /*ٻاهريون*/);

HB_EXTERN بنانشان صحيح
hb_ot_layout_script_get_language_ٽيگ_فهرست (hb_face_t    *face,
				       hb_ٽيگ_t      جدول_ٽيگ_2,
				       بنانشان صحيح  script_اشاريو,
				       بنانشان صحيح  شروعات_وٿي,
				       بنانشان صحيح *language_ڳڻپ /* IN/OUT */,
				       hb_ٽيگ_t     *language_ٽيگ_فهرست /*ٻاهريون*/);

HB_EXTERN hb_bool_t
hb_ot_layout_script_select_language (hb_face_t      *face,
				     hb_ٽيگ_t        جدول_ٽيگ_2,
				     بنانشان صحيح    script_اشاريو,
				     بنانشان صحيح    language_ڳڻپ,
				     ثابت hb_ٽيگ_t *language_ٽيگ_فهرست,
				     بنانشان صحيح   *language_اشاريو /*ٻاهريون*/);

HB_EXTERN hb_bool_t
hb_ot_layout_script_select_language2 (hb_face_t      *face,
				     hb_ٽيگ_t        جدول_ٽيگ_2,
				     بنانشان صحيح    script_اشاريو,
				     بنانشان صحيح    language_ڳڻپ,
				     ثابت hb_ٽيگ_t *language_ٽيگ_فهرست,
				     بنانشان صحيح   *language_اشاريو /*ٻاهريون*/,
				     hb_ٽيگ_t       *chosen_language /*ٻاهريون*/);

HB_EXTERN hb_bool_t
hb_ot_layout_language_get_required_feature_اشاريو (hb_face_t    *face,
						  hb_ٽيگ_t      جدول_ٽيگ_2,
						  بنانشان صحيح  script_اشاريو,
						  بنانشان صحيح  language_اشاريو,
						  بنانشان صحيح *feature_اشاريو /*ٻاهريون*/);

HB_EXTERN hb_bool_t
hb_ot_layout_language_get_required_feature (hb_face_t    *face,
					    hb_ٽيگ_t      جدول_ٽيگ_2,
					    بنانشان صحيح  script_اشاريو,
					    بنانشان صحيح  language_اشاريو,
					    بنانشان صحيح *feature_اشاريو /*ٻاهريون*/,
					    hb_ٽيگ_t     *feature_ٽيگ /*ٻاهريون*/);

HB_EXTERN بنانشان صحيح
hb_ot_layout_language_get_feature_indexes (hb_face_t    *face,
					   hb_ٽيگ_t      جدول_ٽيگ_2,
					   بنانشان صحيح  script_اشاريو,
					   بنانشان صحيح  language_اشاريو,
					   بنانشان صحيح  شروعات_وٿي,
					   بنانشان صحيح *feature_ڳڻپ_2 /* IN/OUT */,
					   بنانشان صحيح *feature_indexes /*ٻاهريون*/);

HB_EXTERN بنانشان صحيح
hb_ot_layout_language_get_feature_ٽيگ_فهرست (hb_face_t    *face,
					hb_ٽيگ_t      جدول_ٽيگ_2,
					بنانشان صحيح  script_اشاريو,
					بنانشان صحيح  language_اشاريو,
					بنانشان صحيح  شروعات_وٿي,
					بنانشان صحيح *feature_ڳڻپ_2 /* IN/OUT */,
					hb_ٽيگ_t     *feature_ٽيگ_فهرست /*ٻاهريون*/);

HB_EXTERN hb_bool_t
hb_ot_layout_language_ڳوليو_feature (hb_face_t    *face,
				    hb_ٽيگ_t      جدول_ٽيگ_2,
				    بنانشان صحيح  script_اشاريو,
				    بنانشان صحيح  language_اشاريو,
				    hb_ٽيگ_t      feature_ٽيگ,
				    بنانشان صحيح *feature_اشاريو /*ٻاهريون*/);

HB_EXTERN بنانشان صحيح
hb_ot_layout_feature_get_lookups (hb_face_t    *face,
				  hb_ٽيگ_t      جدول_ٽيگ_2,
				  بنانشان صحيح  feature_اشاريو,
				  بنانشان صحيح  شروعات_وٿي,
				  بنانشان صحيح *lookup_ڳڻپ /* IN/OUT */,
				  بنانشان صحيح *lookup_indexes /*ٻاهريون*/);

HB_EXTERN بنانشان صحيح
hb_ot_layout_جدول_get_lookup_ڳڻپ (hb_face_t    *face,
				     hb_ٽيگ_t      جدول_ٽيگ_2);

HB_EXTERN فراغ
hb_ot_layout_collect_features (hb_face_t      *face,
			       hb_ٽيگ_t        جدول_ٽيگ_2,
			       ثابت hb_ٽيگ_t *scripts,
			       ثابت hb_ٽيگ_t *languages,
			       ثابت hb_ٽيگ_t *features,
			       hb_سيٽ_t       *feature_indexes /*ٻاهريون*/);

HB_EXTERN فراغ
hb_ot_layout_collect_features_map (hb_face_t      *face,
				   hb_ٽيگ_t        جدول_ٽيگ_2,
				   بنانشان        script_اشاريو,
				   بنانشان        language_اشاريو,
				   hb_map_t       *feature_map /*ٻاهريون*/);

HB_EXTERN فراغ
hb_ot_layout_collect_lookups (hb_face_t      *face,
			      hb_ٽيگ_t        جدول_ٽيگ_2,
			      ثابت hb_ٽيگ_t *scripts,
			      ثابت hb_ٽيگ_t *languages,
			      ثابت hb_ٽيگ_t *features,
			      hb_سيٽ_t       *lookup_indexes /*ٻاهريون*/);

HB_EXTERN فراغ
hb_ot_layout_lookup_collect_glyphs (hb_face_t    *face,
				    hb_ٽيگ_t      جدول_ٽيگ_2,
				    بنانشان صحيح  lookup_اشاريو,
				    hb_سيٽ_t     *glyphs_before, /* OUT.  May be NULL */
				    hb_سيٽ_t     *glyphs_داخلا,  /* OUT.  May be NULL */
				    hb_سيٽ_t     *glyphs_after,  /* OUT.  May be NULL */
				    hb_سيٽ_t     *glyphs_output  /* OUT.  May be NULL */);


/* Variations support */

HB_EXTERN hb_bool_t
hb_ot_layout_جدول_ڳوليو_feature_variations (hb_face_t    *face,
					    hb_ٽيگ_t      جدول_ٽيگ_2,
					    ثابت صحيح    *coords,
					    بنانشان صحيح  انگ_coords,
					    بنانشان صحيح *variations_اشاريو /* out */);

HB_EXTERN بنانشان صحيح
hb_ot_layout_feature_with_variations_get_lookups (hb_face_t    *face,
						  hb_ٽيگ_t      جدول_ٽيگ_2,
						  بنانشان صحيح  feature_اشاريو,
						  بنانشان صحيح  variations_اشاريو,
						  بنانشان صحيح  شروعات_وٿي,
						  بنانشان صحيح *lookup_ڳڻپ /* IN/OUT */,
						  بنانشان صحيح *lookup_indexes /*ٻاهريون*/);


/*
 * GSUB
 */

HB_EXTERN hb_bool_t
hb_ot_layout_has_substitution (hb_face_t *face);

HB_EXTERN بنانشان
hb_ot_layout_lookup_get_glyph_alternates (hb_face_t      *face,
					  بنانشان        lookup_اشاريو,
					  hb_codepoint_t  glyph,
					  بنانشان        شروعات_وٿي,
					  بنانشان       *alternate_ڳڻپ /* IN/OUT */,
					  hb_codepoint_t *alternate_glyphs /*ٻاهريون*/);

HB_EXTERN hb_bool_t
hb_ot_layout_lookup_collect_glyph_alternates (hb_face_t *face,
					      بنانشان   lookup_اشاريو,
					      hb_map_t  *alternate_ڳڻپ /* IN/OUT */,
					      hb_map_t  *alternate_glyphs /* IN/OUT */);

HB_EXTERN hb_bool_t
hb_ot_layout_lookup_would_substitute (hb_face_t            *face,
				      بنانشان صحيح          lookup_اشاريو,
				      ثابت hb_codepoint_t *glyphs,
				      بنانشان صحيح          glyphs_ڊيگهائي,
				      hb_bool_t             zero_context);

HB_EXTERN فراغ
hb_ot_layout_lookup_substitute_گهيرو (hb_face_t    *face,
					بنانشان صحيح  lookup_اشاريو,
					hb_سيٽ_t     *glyphs
					/*TODO , hb_bool_t  inclusive */);

HB_EXTERN فراغ
hb_ot_layout_lookups_substitute_گهيرو (hb_face_t      *face,
					 ثابت hb_سيٽ_t *lookups,
					 hb_سيٽ_t       *glyphs);


/*
 * GPOS
 */

HB_EXTERN hb_bool_t
hb_ot_layout_has_positioning (hb_face_t *face);

/* Optical 'size' feature info.  Returns true if found.
 * https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#size */
HB_EXTERN hb_bool_t
hb_ot_layout_get_سائيز_params (hb_face_t       *face,
			      بنانشان صحيح    *design_سائيز_2,       /* OUT.  May be NULL */
			      بنانشان صحيح    *subfamily_id,      /* OUT.  May be NULL */
			      hb_ot_name_id_t *subfamily_name_id, /* OUT.  May be NULL */
			      بنانشان صحيح    *range_شروعات_2,       /* OUT.  May be NULL */
			      بنانشان صحيح    *range_پڇاڙي_2          /* OUT.  May be NULL */);

HB_EXTERN hb_position_t
hb_ot_layout_lookup_get_optical_bound (hb_font_t      *font,
				       بنانشان        lookup_اشاريو,
				       hb_direction_t  direction,
				       hb_codepoint_t  glyph);


/*
 * GSUB/GPOS
 */

HB_EXTERN hb_bool_t
hb_ot_layout_feature_get_name_ids (hb_face_t       *face,
				   hb_ٽيگ_t         جدول_ٽيگ_2,
				   بنانشان صحيح     feature_اشاريو,
				   hb_ot_name_id_t *label_id             /* OUT.  May be NULL */,
				   hb_ot_name_id_t *tooltip_id           /* OUT.  May be NULL */,
				   hb_ot_name_id_t *sample_id            /* OUT.  May be NULL */,
				   بنانشان صحيح    *انگ_named_parameters /* OUT.  May be NULL */,
				   hb_ot_name_id_t *پهريون_param_id       /* OUT.  May be NULL */);


HB_EXTERN بنانشان صحيح
hb_ot_layout_feature_get_characters (hb_face_t      *face,
				     hb_ٽيگ_t        جدول_ٽيگ_2,
				     بنانشان صحيح    feature_اشاريو,
				     بنانشان صحيح    شروعات_وٿي,
				     بنانشان صحيح   *char_ڳڻپ    /* IN/OUT.  May be NULL */,
				     hb_codepoint_t *characters    /* OUT.     May be NULL */);


/*
 * BASE
 */

HB_EXTERN hb_bool_t
hb_ot_layout_get_font_extents (hb_font_t         *font,
			       hb_direction_t     direction,
			       hb_ٽيگ_t           script_ٽيگ,
			       hb_ٽيگ_t           language_ٽيگ,
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
  HB_OT_LAYOUT_BASELINE_ٽيگ_ROMAN			= HB_ٽيگ ('r','o','m','n'),
  HB_OT_LAYOUT_BASELINE_ٽيگ_HANGING			= HB_ٽيگ ('h','a','n','g'),
  HB_OT_LAYOUT_BASELINE_ٽيگ_IDEO_FACE_BOTTOM_OR_کاٻو	= HB_ٽيگ ('i','c','f','b'),
  HB_OT_LAYOUT_BASELINE_ٽيگ_IDEO_FACE_چوٽي_OR_ساڄو	= HB_ٽيگ ('i','c','f','t'),
  HB_OT_LAYOUT_BASELINE_ٽيگ_IDEO_FACE_CENTRAL		= HB_ٽيگ ('I','c','f','c'),
  HB_OT_LAYOUT_BASELINE_ٽيگ_IDEO_EMBOX_BOTTOM_OR_کاٻو	= HB_ٽيگ ('i','d','e','o'),
  HB_OT_LAYOUT_BASELINE_ٽيگ_IDEO_EMBOX_چوٽي_OR_ساڄو	= HB_ٽيگ ('i','d','t','p'),
  HB_OT_LAYOUT_BASELINE_ٽيگ_IDEO_EMBOX_CENTRAL		= HB_ٽيگ ('I','d','c','e'),
  HB_OT_LAYOUT_BASELINE_ٽيگ_MATH			= HB_ٽيگ ('m','a','t','h'),

  /*< private >*/
  HB_OT_LAYOUT_BASELINE_ٽيگ_MAX_قدر = HB_ٽيگ_MAX_SIGNED /*< skip >*/
} hb_ot_layout_baseline_ٽيگ_t;

HB_EXTERN hb_ot_layout_baseline_ٽيگ_t
hb_ot_layout_get_horizontal_baseline_ٽيگ_for_script (hb_script_t script);

HB_EXTERN hb_bool_t
hb_ot_layout_get_baseline (hb_font_t                   *font,
			   hb_ot_layout_baseline_ٽيگ_t  baseline_ٽيگ,
			   hb_direction_t               direction,
			   hb_ٽيگ_t                     script_ٽيگ,
			   hb_ٽيگ_t                     language_ٽيگ,
			   hb_position_t               *coord        /* OUT.  May be NULL. */);

HB_EXTERN hb_bool_t
hb_ot_layout_get_baseline2 (hb_font_t                   *font,
			    hb_ot_layout_baseline_ٽيگ_t  baseline_ٽيگ,
			    hb_direction_t               direction,
			    hb_script_t                  script,
			    hb_language_t                language,
			    hb_position_t               *coord        /* OUT.  May be NULL. */);

HB_EXTERN فراغ
hb_ot_layout_get_baseline_with_fallback (hb_font_t                   *font,
					 hb_ot_layout_baseline_ٽيگ_t  baseline_ٽيگ,
					 hb_direction_t               direction,
					 hb_ٽيگ_t                     script_ٽيگ,
					 hb_ٽيگ_t                     language_ٽيگ,
					 hb_position_t               *coord        /*ٻاهريون*/);

HB_EXTERN فراغ
hb_ot_layout_get_baseline_with_fallback2 (hb_font_t                   *font,
					  hb_ot_layout_baseline_ٽيگ_t  baseline_ٽيگ,
					  hb_direction_t               direction,
					  hb_script_t                  script,
					  hb_language_t                language,
					  hb_position_t               *coord        /*ٻاهريون*/);

HB_پڇاڙي_DECLS

#جيڪڏهن_پورو /* HB_OT_LAYOUT_H */

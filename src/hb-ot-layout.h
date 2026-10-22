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

#ਜੇ !defined(HB_OT_H_ਵਿੱਚ) && !defined(HB_NO_SINGLE_ਸਿਰਲੇਖ_ERROR)
#ਗਲਤੀ "Include <hb-ot.h> instead."
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_LAYOUT_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_LAYOUT_H

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-name.h"

HB_BEGIN_DECLS


/**
 * HB_OT_TAG_BASE:
 *
 * OpenType [Baseline Table](https://docs.microsoft.com/en-us/typography/opentype/spec/base).
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_ਅਧਾਰ HB_ਟੈਗ('B','A','S','E')
/**
 * HB_OT_TAG_GDEF:
 *
 * OpenType [Glyph Definition Table](https://docs.microsoft.com/en-us/typography/opentype/spec/gdef).
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_GDEF HB_ਟੈਗ('G','D','E','F')
/**
 * HB_OT_TAG_GSUB:
 *
 * OpenType [Glyph Substitution Table](https://docs.microsoft.com/en-us/typography/opentype/spec/gsub).
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_GSUB HB_ਟੈਗ('G','S','U','B')
/**
 * HB_OT_TAG_GPOS:
 *
 * OpenType [Glyph Positioning Table](https://docs.microsoft.com/en-us/typography/opentype/spec/gpos).
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_GPOS HB_ਟੈਗ('G','P','O','S')
/**
 * HB_OT_TAG_JSTF:
 *
 * OpenType [Justification Table](https://docs.microsoft.com/en-us/typography/opentype/spec/jstf).
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_JSTF HB_ਟੈਗ('J','S','T','F')


/*
 * Script & Language tags.
 */

/**
 * HB_OT_TAG_DEFAULT_SCRIPT:
 *
 * OpenType script tag, `DFLT`, for features that are not script-specific.
 *
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_DEFAULT_SCRIPT	HB_ਟੈਗ ('D', 'F', 'L', 'T')
/**
 * HB_OT_TAG_DEFAULT_LANGUAGE:
 *
 * OpenType language tag, `dflt`. Not a valid language tag, but some fonts
 * mistakenly use it.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_DEFAULT_LANGUAGE	HB_ਟੈਗ ('d', 'f', 'l', 't')

/**
 * HB_OT_MAX_TAGS_PER_SCRIPT:
 *
 * Maximum number of OpenType tags that can correspond to a give #hb_script_t.
 *
 * Since: 2.0.0
 **/
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_MAX_ਟੈਗ_ਸੂਚੀ_ਪ੍ਰਤੀ_SCRIPT	3u
/**
 * HB_OT_MAX_TAGS_PER_LANGUAGE:
 *
 * Maximum number of OpenType tags that can correspond to a give #hb_language_t.
 *
 * Since: 2.0.0
 **/
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_MAX_ਟੈਗ_ਸੂਚੀ_ਪ੍ਰਤੀ_LANGUAGE	3u

HB_EXTERN ਖਾਲੀ
hb_ot_ਟੈਗ_ਸੂਚੀ_from_script_and_language (hb_script_t   script,
				     hb_language_t language,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *script_ਗਿਣਤੀ /* IN/OUT */,
				     hb_ਟੈਗ_t     *script_ਟੈਗ_ਸੂਚੀ /*ਬਾਹਰਲਾ*/,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *language_ਗਿਣਤੀ /* IN/OUT */,
				     hb_ਟੈਗ_t     *language_ਟੈਗ_ਸੂਚੀ /*ਬਾਹਰਲਾ*/);

HB_EXTERN hb_script_t
hb_ot_ਟੈਗ_ਮੰਜ਼ਿਲ_script (hb_ਟੈਗ_t ਟੈਗ_2);

HB_EXTERN hb_language_t
hb_ot_ਟੈਗ_ਮੰਜ਼ਿਲ_language (hb_ਟੈਗ_t ਟੈਗ_2);

HB_EXTERN ਖਾਲੀ
hb_ot_ਟੈਗ_ਸੂਚੀ_ਮੰਜ਼ਿਲ_script_and_language (hb_ਟੈਗ_t       script_ਟੈਗ,
				   hb_ਟੈਗ_t       language_ਟੈਗ,
				   hb_script_t   *script /*ਬਾਹਰਲਾ*/,
				   hb_language_t *language /*ਬਾਹਰਲਾ*/);


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
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਗਿਣਤੀ {
  HB_OT_LAYOUT_GLYPH_CLASS_UNCLASSIFIED	= 0,
  HB_OT_LAYOUT_GLYPH_CLASS_ਅਧਾਰ_GLYPH	= 1,
  HB_OT_LAYOUT_GLYPH_CLASS_LIGATURE	= 2,
  HB_OT_LAYOUT_GLYPH_CLASS_MARK		= 3,
  HB_OT_LAYOUT_GLYPH_CLASS_COMPONENT	= 4
} hb_ot_layout_glyph_class_t;

HB_EXTERN hb_ot_layout_glyph_class_t
hb_ot_layout_get_glyph_class (hb_face_t      *face,
			      hb_codepoint_t  glyph);

HB_EXTERN ਖਾਲੀ
hb_ot_layout_get_glyphs_ਵਿੱਚ_class (hb_face_t                  *face,
				  hb_ot_layout_glyph_class_t  klass,
				  hb_ਸੈੱਟ_t                   *glyphs /*ਬਾਹਰਲਾ*/);

/* Not that useful.  Provides list of attach points for a glyph that a
 * client may want to cache */
HB_EXTERN ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_layout_get_attach_points (hb_face_t      *face,
				hb_codepoint_t  glyph,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    ਸ਼ੁਰੂ_ਵਿੱਥ,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ   *ਬਿੰਦੂ_ਗਿਣਤੀ /* IN/OUT */,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ   *ਬਿੰਦੂ_array /*ਬਾਹਰਲਾ*/);

/* Ligature caret positions */
HB_EXTERN ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_layout_get_ligature_carets (hb_font_t      *font,
				  hb_direction_t  direction,
				  hb_codepoint_t  glyph,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    ਸ਼ੁਰੂ_ਵਿੱਥ,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ   *caret_ਗਿਣਤੀ /* IN/OUT */,
				  hb_position_t  *caret_array /*ਬਾਹਰਲਾ*/);


/*
 * GSUB/GPOS feature query and enumeration interface
 */

/**
 * HB_OT_LAYOUT_NO_SCRIPT_INDEX:
 *
 * Special value for script index indicating unsupported script.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_LAYOUT_NO_SCRIPT_ਸੂਚਕਾਂਕ		0xFFFFu
/**
 * HB_OT_LAYOUT_NO_FEATURE_INDEX:
 *
 * Special value for feature index indicating unsupported feature.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_LAYOUT_NO_FEATURE_ਸੂਚਕਾਂਕ		0xFFFFu
/**
 * HB_OT_LAYOUT_DEFAULT_LANGUAGE_INDEX:
 *
 * Special value for language index indicating default or unsupported language.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_LAYOUT_DEFAULT_LANGUAGE_ਸੂਚਕਾਂਕ	0xFFFFu
/**
 * HB_OT_LAYOUT_NO_VARIATIONS_INDEX:
 *
 * Special value for variations index indicating unsupported variation.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_LAYOUT_NO_VARIATIONS_ਸੂਚਕਾਂਕ	0xFFFFFFFFu

HB_EXTERN ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_layout_ਸਾਰਣੀ_get_script_ਟੈਗ_ਸੂਚੀ (hb_face_t    *face,
				    hb_ਟੈਗ_t      ਸਾਰਣੀ_ਟੈਗ_2,
				    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸ਼ੁਰੂ_ਵਿੱਥ,
				    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *script_ਗਿਣਤੀ /* IN/OUT */,
				    hb_ਟੈਗ_t     *script_ਟੈਗ_ਸੂਚੀ /*ਬਾਹਰਲਾ*/);

HB_EXTERN hb_bool_t
hb_ot_layout_ਸਾਰਣੀ_ਲੱਭੋ_script (hb_face_t    *face,
				hb_ਟੈਗ_t      ਸਾਰਣੀ_ਟੈਗ_2,
				hb_ਟੈਗ_t      script_ਟੈਗ,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *script_ਸੂਚਕਾਂਕ /*ਬਾਹਰਲਾ*/);

HB_EXTERN hb_bool_t
hb_ot_layout_ਸਾਰਣੀ_select_script (hb_face_t      *face,
				  hb_ਟੈਗ_t        ਸਾਰਣੀ_ਟੈਗ_2,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    script_ਗਿਣਤੀ,
				  ਅਚਲ hb_ਟੈਗ_t *script_ਟੈਗ_ਸੂਚੀ,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ   *script_ਸੂਚਕਾਂਕ /*ਬਾਹਰਲਾ*/,
				  hb_ਟੈਗ_t       *chosen_script /*ਬਾਹਰਲਾ*/);

HB_EXTERN ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_layout_ਸਾਰਣੀ_get_feature_ਟੈਗ_ਸੂਚੀ (hb_face_t    *face,
				     hb_ਟੈਗ_t      ਸਾਰਣੀ_ਟੈਗ_2,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸ਼ੁਰੂ_ਵਿੱਥ,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *feature_ਗਿਣਤੀ_2 /* IN/OUT */,
				     hb_ਟੈਗ_t     *feature_ਟੈਗ_ਸੂਚੀ /*ਬਾਹਰਲਾ*/);

HB_EXTERN ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_layout_script_get_language_ਟੈਗ_ਸੂਚੀ (hb_face_t    *face,
				       hb_ਟੈਗ_t      ਸਾਰਣੀ_ਟੈਗ_2,
				       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  script_ਸੂਚਕਾਂਕ,
				       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸ਼ੁਰੂ_ਵਿੱਥ,
				       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *language_ਗਿਣਤੀ /* IN/OUT */,
				       hb_ਟੈਗ_t     *language_ਟੈਗ_ਸੂਚੀ /*ਬਾਹਰਲਾ*/);

HB_EXTERN hb_bool_t
hb_ot_layout_script_select_language (hb_face_t      *face,
				     hb_ਟੈਗ_t        ਸਾਰਣੀ_ਟੈਗ_2,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    script_ਸੂਚਕਾਂਕ,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    language_ਗਿਣਤੀ,
				     ਅਚਲ hb_ਟੈਗ_t *language_ਟੈਗ_ਸੂਚੀ,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ   *language_ਸੂਚਕਾਂਕ /*ਬਾਹਰਲਾ*/);

HB_EXTERN hb_bool_t
hb_ot_layout_script_select_language2 (hb_face_t      *face,
				     hb_ਟੈਗ_t        ਸਾਰਣੀ_ਟੈਗ_2,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    script_ਸੂਚਕਾਂਕ,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    language_ਗਿਣਤੀ,
				     ਅਚਲ hb_ਟੈਗ_t *language_ਟੈਗ_ਸੂਚੀ,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ   *language_ਸੂਚਕਾਂਕ /*ਬਾਹਰਲਾ*/,
				     hb_ਟੈਗ_t       *chosen_language /*ਬਾਹਰਲਾ*/);

HB_EXTERN hb_bool_t
hb_ot_layout_language_get_required_feature_ਸੂਚਕਾਂਕ (hb_face_t    *face,
						  hb_ਟੈਗ_t      ਸਾਰਣੀ_ਟੈਗ_2,
						  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  script_ਸੂਚਕਾਂਕ,
						  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  language_ਸੂਚਕਾਂਕ,
						  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *feature_ਸੂਚਕਾਂਕ /*ਬਾਹਰਲਾ*/);

HB_EXTERN hb_bool_t
hb_ot_layout_language_get_required_feature (hb_face_t    *face,
					    hb_ਟੈਗ_t      ਸਾਰਣੀ_ਟੈਗ_2,
					    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  script_ਸੂਚਕਾਂਕ,
					    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  language_ਸੂਚਕਾਂਕ,
					    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *feature_ਸੂਚਕਾਂਕ /*ਬਾਹਰਲਾ*/,
					    hb_ਟੈਗ_t     *feature_ਟੈਗ /*ਬਾਹਰਲਾ*/);

HB_EXTERN ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_layout_language_get_feature_indexes (hb_face_t    *face,
					   hb_ਟੈਗ_t      ਸਾਰਣੀ_ਟੈਗ_2,
					   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  script_ਸੂਚਕਾਂਕ,
					   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  language_ਸੂਚਕਾਂਕ,
					   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸ਼ੁਰੂ_ਵਿੱਥ,
					   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *feature_ਗਿਣਤੀ_2 /* IN/OUT */,
					   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *feature_indexes /*ਬਾਹਰਲਾ*/);

HB_EXTERN ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_layout_language_get_feature_ਟੈਗ_ਸੂਚੀ (hb_face_t    *face,
					hb_ਟੈਗ_t      ਸਾਰਣੀ_ਟੈਗ_2,
					ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  script_ਸੂਚਕਾਂਕ,
					ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  language_ਸੂਚਕਾਂਕ,
					ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸ਼ੁਰੂ_ਵਿੱਥ,
					ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *feature_ਗਿਣਤੀ_2 /* IN/OUT */,
					hb_ਟੈਗ_t     *feature_ਟੈਗ_ਸੂਚੀ /*ਬਾਹਰਲਾ*/);

HB_EXTERN hb_bool_t
hb_ot_layout_language_ਲੱਭੋ_feature (hb_face_t    *face,
				    hb_ਟੈਗ_t      ਸਾਰਣੀ_ਟੈਗ_2,
				    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  script_ਸੂਚਕਾਂਕ,
				    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  language_ਸੂਚਕਾਂਕ,
				    hb_ਟੈਗ_t      feature_ਟੈਗ,
				    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *feature_ਸੂਚਕਾਂਕ /*ਬਾਹਰਲਾ*/);

HB_EXTERN ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_layout_feature_get_lookups (hb_face_t    *face,
				  hb_ਟੈਗ_t      ਸਾਰਣੀ_ਟੈਗ_2,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  feature_ਸੂਚਕਾਂਕ,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸ਼ੁਰੂ_ਵਿੱਥ,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *lookup_ਗਿਣਤੀ /* IN/OUT */,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *lookup_indexes /*ਬਾਹਰਲਾ*/);

HB_EXTERN ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_layout_ਸਾਰਣੀ_get_lookup_ਗਿਣਤੀ (hb_face_t    *face,
				     hb_ਟੈਗ_t      ਸਾਰਣੀ_ਟੈਗ_2);

HB_EXTERN ਖਾਲੀ
hb_ot_layout_collect_features (hb_face_t      *face,
			       hb_ਟੈਗ_t        ਸਾਰਣੀ_ਟੈਗ_2,
			       ਅਚਲ hb_ਟੈਗ_t *scripts,
			       ਅਚਲ hb_ਟੈਗ_t *languages,
			       ਅਚਲ hb_ਟੈਗ_t *features,
			       hb_ਸੈੱਟ_t       *feature_indexes /*ਬਾਹਰਲਾ*/);

HB_EXTERN ਖਾਲੀ
hb_ot_layout_collect_features_map (hb_face_t      *face,
				   hb_ਟੈਗ_t        ਸਾਰਣੀ_ਟੈਗ_2,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ        script_ਸੂਚਕਾਂਕ,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ        language_ਸੂਚਕਾਂਕ,
				   hb_map_t       *feature_map /*ਬਾਹਰਲਾ*/);

HB_EXTERN ਖਾਲੀ
hb_ot_layout_collect_lookups (hb_face_t      *face,
			      hb_ਟੈਗ_t        ਸਾਰਣੀ_ਟੈਗ_2,
			      ਅਚਲ hb_ਟੈਗ_t *scripts,
			      ਅਚਲ hb_ਟੈਗ_t *languages,
			      ਅਚਲ hb_ਟੈਗ_t *features,
			      hb_ਸੈੱਟ_t       *lookup_indexes /*ਬਾਹਰਲਾ*/);

HB_EXTERN ਖਾਲੀ
hb_ot_layout_lookup_collect_glyphs (hb_face_t    *face,
				    hb_ਟੈਗ_t      ਸਾਰਣੀ_ਟੈਗ_2,
				    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  lookup_ਸੂਚਕਾਂਕ,
				    hb_ਸੈੱਟ_t     *glyphs_before, /* OUT.  May be NULL */
				    hb_ਸੈੱਟ_t     *glyphs_ਨਿਵੇਸ਼,  /* OUT.  May be NULL */
				    hb_ਸੈੱਟ_t     *glyphs_after,  /* OUT.  May be NULL */
				    hb_ਸੈੱਟ_t     *glyphs_output  /* OUT.  May be NULL */);


/* Variations support */

HB_EXTERN hb_bool_t
hb_ot_layout_ਸਾਰਣੀ_ਲੱਭੋ_feature_variations (hb_face_t    *face,
					    hb_ਟੈਗ_t      ਸਾਰਣੀ_ਟੈਗ_2,
					    ਅਚਲ ਪੂਰਨ    *coords,
					    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਅੰਕ_coords,
					    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *variations_ਸੂਚਕਾਂਕ /* out */);

HB_EXTERN ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_layout_feature_with_variations_get_lookups (hb_face_t    *face,
						  hb_ਟੈਗ_t      ਸਾਰਣੀ_ਟੈਗ_2,
						  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  feature_ਸੂਚਕਾਂਕ,
						  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  variations_ਸੂਚਕਾਂਕ,
						  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸ਼ੁਰੂ_ਵਿੱਥ,
						  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *lookup_ਗਿਣਤੀ /* IN/OUT */,
						  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *lookup_indexes /*ਬਾਹਰਲਾ*/);


/*
 * GSUB
 */

HB_EXTERN hb_bool_t
hb_ot_layout_has_substitution (hb_face_t *face);

HB_EXTERN ਬਿਨਾਨਿਸ਼ਾਨੀ
hb_ot_layout_lookup_get_glyph_alternates (hb_face_t      *face,
					  ਬਿਨਾਨਿਸ਼ਾਨੀ        lookup_ਸੂਚਕਾਂਕ,
					  hb_codepoint_t  glyph,
					  ਬਿਨਾਨਿਸ਼ਾਨੀ        ਸ਼ੁਰੂ_ਵਿੱਥ,
					  ਬਿਨਾਨਿਸ਼ਾਨੀ       *alternate_ਗਿਣਤੀ /* IN/OUT */,
					  hb_codepoint_t *alternate_glyphs /*ਬਾਹਰਲਾ*/);

HB_EXTERN hb_bool_t
hb_ot_layout_lookup_collect_glyph_alternates (hb_face_t *face,
					      ਬਿਨਾਨਿਸ਼ਾਨੀ   lookup_ਸੂਚਕਾਂਕ,
					      hb_map_t  *alternate_ਗਿਣਤੀ /* IN/OUT */,
					      hb_map_t  *alternate_glyphs /* IN/OUT */);

HB_EXTERN hb_bool_t
hb_ot_layout_lookup_would_substitute (hb_face_t            *face,
				      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ          lookup_ਸੂਚਕਾਂਕ,
				      ਅਚਲ hb_codepoint_t *glyphs,
				      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ          glyphs_ਲੰਮਾਈ,
				      hb_bool_t             zero_context);

HB_EXTERN ਖਾਲੀ
hb_ot_layout_lookup_substitute_ਘੇਰਾ (hb_face_t    *face,
					ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  lookup_ਸੂਚਕਾਂਕ,
					hb_ਸੈੱਟ_t     *glyphs
					/*TODO , hb_bool_t  inclusive */);

HB_EXTERN ਖਾਲੀ
hb_ot_layout_lookups_substitute_ਘੇਰਾ (hb_face_t      *face,
					 ਅਚਲ hb_ਸੈੱਟ_t *lookups,
					 hb_ਸੈੱਟ_t       *glyphs);


/*
 * GPOS
 */

HB_EXTERN hb_bool_t
hb_ot_layout_has_positioning (hb_face_t *face);

/* Optical 'size' feature info.  Returns true if found.
 * https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#size */
HB_EXTERN hb_bool_t
hb_ot_layout_get_ਮਾਪ_params (hb_face_t       *face,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    *design_ਮਾਪ_2,       /* OUT.  May be NULL */
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    *subfamily_id,      /* OUT.  May be NULL */
			      hb_ot_name_id_t *subfamily_name_id, /* OUT.  May be NULL */
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    *range_ਸ਼ੁਰੂ_2,       /* OUT.  May be NULL */
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    *range_ਸਿਰਾ_2          /* OUT.  May be NULL */);

HB_EXTERN hb_position_t
hb_ot_layout_lookup_get_optical_bound (hb_font_t      *font,
				       ਬਿਨਾਨਿਸ਼ਾਨੀ        lookup_ਸੂਚਕਾਂਕ,
				       hb_direction_t  direction,
				       hb_codepoint_t  glyph);


/*
 * GSUB/GPOS
 */

HB_EXTERN hb_bool_t
hb_ot_layout_feature_get_name_ids (hb_face_t       *face,
				   hb_ਟੈਗ_t         ਸਾਰਣੀ_ਟੈਗ_2,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ     feature_ਸੂਚਕਾਂਕ,
				   hb_ot_name_id_t *label_id             /* OUT.  May be NULL */,
				   hb_ot_name_id_t *tooltip_id           /* OUT.  May be NULL */,
				   hb_ot_name_id_t *sample_id            /* OUT.  May be NULL */,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    *ਅੰਕ_named_parameters /* OUT.  May be NULL */,
				   hb_ot_name_id_t *ਪਹਿਲਾ_param_id       /* OUT.  May be NULL */);


HB_EXTERN ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_layout_feature_get_characters (hb_face_t      *face,
				     hb_ਟੈਗ_t        ਸਾਰਣੀ_ਟੈਗ_2,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    feature_ਸੂਚਕਾਂਕ,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    ਸ਼ੁਰੂ_ਵਿੱਥ,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ   *char_ਗਿਣਤੀ    /* IN/OUT.  May be NULL */,
				     hb_codepoint_t *characters    /* OUT.     May be NULL */);


/*
 * BASE
 */

HB_EXTERN hb_bool_t
hb_ot_layout_get_font_extents (hb_font_t         *font,
			       hb_direction_t     direction,
			       hb_ਟੈਗ_t           script_ਟੈਗ,
			       hb_ਟੈਗ_t           language_ਟੈਗ,
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
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਗਿਣਤੀ {
  HB_OT_LAYOUT_BASELINE_ਟੈਗ_ROMAN			= HB_ਟੈਗ ('r','o','m','n'),
  HB_OT_LAYOUT_BASELINE_ਟੈਗ_HANGING			= HB_ਟੈਗ ('h','a','n','g'),
  HB_OT_LAYOUT_BASELINE_ਟੈਗ_IDEO_FACE_BOTTOM_OR_ਖੱਬਾ	= HB_ਟੈਗ ('i','c','f','b'),
  HB_OT_LAYOUT_BASELINE_ਟੈਗ_IDEO_FACE_ਸਿਖਰ_OR_ਸੱਜਾ	= HB_ਟੈਗ ('i','c','f','t'),
  HB_OT_LAYOUT_BASELINE_ਟੈਗ_IDEO_FACE_CENTRAL		= HB_ਟੈਗ ('I','c','f','c'),
  HB_OT_LAYOUT_BASELINE_ਟੈਗ_IDEO_EMBOX_BOTTOM_OR_ਖੱਬਾ	= HB_ਟੈਗ ('i','d','e','o'),
  HB_OT_LAYOUT_BASELINE_ਟੈਗ_IDEO_EMBOX_ਸਿਖਰ_OR_ਸੱਜਾ	= HB_ਟੈਗ ('i','d','t','p'),
  HB_OT_LAYOUT_BASELINE_ਟੈਗ_IDEO_EMBOX_CENTRAL		= HB_ਟੈਗ ('I','d','c','e'),
  HB_OT_LAYOUT_BASELINE_ਟੈਗ_MATH			= HB_ਟੈਗ ('m','a','t','h'),

  /*< private >*/
  HB_OT_LAYOUT_BASELINE_ਟੈਗ_MAX_ਮੁੱਲ = HB_ਟੈਗ_MAX_SIGNED /*< skip >*/
} hb_ot_layout_baseline_ਟੈਗ_t;

HB_EXTERN hb_ot_layout_baseline_ਟੈਗ_t
hb_ot_layout_get_horizontal_baseline_ਟੈਗ_for_script (hb_script_t script);

HB_EXTERN hb_bool_t
hb_ot_layout_get_baseline (hb_font_t                   *font,
			   hb_ot_layout_baseline_ਟੈਗ_t  baseline_ਟੈਗ,
			   hb_direction_t               direction,
			   hb_ਟੈਗ_t                     script_ਟੈਗ,
			   hb_ਟੈਗ_t                     language_ਟੈਗ,
			   hb_position_t               *coord        /* OUT.  May be NULL. */);

HB_EXTERN hb_bool_t
hb_ot_layout_get_baseline2 (hb_font_t                   *font,
			    hb_ot_layout_baseline_ਟੈਗ_t  baseline_ਟੈਗ,
			    hb_direction_t               direction,
			    hb_script_t                  script,
			    hb_language_t                language,
			    hb_position_t               *coord        /* OUT.  May be NULL. */);

HB_EXTERN ਖਾਲੀ
hb_ot_layout_get_baseline_with_fallback (hb_font_t                   *font,
					 hb_ot_layout_baseline_ਟੈਗ_t  baseline_ਟੈਗ,
					 hb_direction_t               direction,
					 hb_ਟੈਗ_t                     script_ਟੈਗ,
					 hb_ਟੈਗ_t                     language_ਟੈਗ,
					 hb_position_t               *coord        /*ਬਾਹਰਲਾ*/);

HB_EXTERN ਖਾਲੀ
hb_ot_layout_get_baseline_with_fallback2 (hb_font_t                   *font,
					  hb_ot_layout_baseline_ਟੈਗ_t  baseline_ਟੈਗ,
					  hb_direction_t               direction,
					  hb_script_t                  script,
					  hb_language_t                language,
					  hb_position_t               *coord        /*ਬਾਹਰਲਾ*/);

HB_ਸਿਰਾ_DECLS

#ਜੇ_ਅੰਤ /* HB_OT_LAYOUT_H */

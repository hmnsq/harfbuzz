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

#ఒకవేళ !defined(HB_OT_H_లోపల) && !defined(HB_NO_SINGLE_శీర్షిక_ERROR)
#దోషం "Include <hb-ot.h> instead."
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_OT_LAYOUT_H
#నిర్వచించు HB_OT_LAYOUT_H

#అన్తర్భావ "hb.h"

#అన్తర్భావ "hb-ot-name.h"

HB_BEGIN_DECLS


/**
 * HB_OT_TAG_BASE:
 *
 * OpenType [Baseline Table](https://docs.microsoft.com/en-us/typography/opentype/spec/base).
 */
#నిర్వచించు HB_OT_ట్యాగ్_పునాది HB_ట్యాగ్('B','A','S','E')
/**
 * HB_OT_TAG_GDEF:
 *
 * OpenType [Glyph Definition Table](https://docs.microsoft.com/en-us/typography/opentype/spec/gdef).
 */
#నిర్వచించు HB_OT_ట్యాగ్_GDEF HB_ట్యాగ్('G','D','E','F')
/**
 * HB_OT_TAG_GSUB:
 *
 * OpenType [Glyph Substitution Table](https://docs.microsoft.com/en-us/typography/opentype/spec/gsub).
 */
#నిర్వచించు HB_OT_ట్యాగ్_GSUB HB_ట్యాగ్('G','S','U','B')
/**
 * HB_OT_TAG_GPOS:
 *
 * OpenType [Glyph Positioning Table](https://docs.microsoft.com/en-us/typography/opentype/spec/gpos).
 */
#నిర్వచించు HB_OT_ట్యాగ్_GPOS HB_ట్యాగ్('G','P','O','S')
/**
 * HB_OT_TAG_JSTF:
 *
 * OpenType [Justification Table](https://docs.microsoft.com/en-us/typography/opentype/spec/jstf).
 */
#నిర్వచించు HB_OT_ట్యాగ్_JSTF HB_ట్యాగ్('J','S','T','F')


/*
 * Script & Language tags.
 */

/**
 * HB_OT_TAG_DEFAULT_SCRIPT:
 *
 * OpenType script tag, `DFLT`, for features that are not script-specific.
 *
 */
#నిర్వచించు HB_OT_ట్యాగ్_DEFAULT_SCRIPT	HB_ట్యాగ్ ('D', 'F', 'L', 'T')
/**
 * HB_OT_TAG_DEFAULT_LANGUAGE:
 *
 * OpenType language tag, `dflt`. Not a valid language tag, but some fonts
 * mistakenly use it.
 */
#నిర్వచించు HB_OT_ట్యాగ్_DEFAULT_LANGUAGE	HB_ట్యాగ్ ('d', 'f', 'l', 't')

/**
 * HB_OT_MAX_TAGS_PER_SCRIPT:
 *
 * Maximum number of OpenType tags that can correspond to a give #hb_script_t.
 *
 * Since: 2.0.0
 **/
#నిర్వచించు HB_OT_MAX_ట్యాగులు_చొప్పున_SCRIPT	3u
/**
 * HB_OT_MAX_TAGS_PER_LANGUAGE:
 *
 * Maximum number of OpenType tags that can correspond to a give #hb_language_t.
 *
 * Since: 2.0.0
 **/
#నిర్వచించు HB_OT_MAX_ట్యాగులు_చొప్పున_LANGUAGE	3u

HB_EXTERN శూన్య
hb_ot_ట్యాగులు_from_script_and_language (hb_script_t   script,
				     hb_language_t language,
				     గుర్తులేని పూర్ణాంకం *script_లెక్క /* IN/OUT */,
				     hb_ట్యాగ్_t     *script_ట్యాగులు /*బహిర్గతం*/,
				     గుర్తులేని పూర్ణాంకం *language_లెక్క /* IN/OUT */,
				     hb_ట్యాగ్_t     *language_ట్యాగులు /*బహిర్గతం*/);

HB_EXTERN hb_script_t
hb_ot_ట్యాగ్_గమ్యం_script (hb_ట్యాగ్_t ట్యాగ్_2);

HB_EXTERN hb_language_t
hb_ot_ట్యాగ్_గమ్యం_language (hb_ట్యాగ్_t ట్యాగ్_2);

HB_EXTERN శూన్య
hb_ot_ట్యాగులు_గమ్యం_script_and_language (hb_ట్యాగ్_t       script_ట్యాగ్,
				   hb_ట్యాగ్_t       language_ట్యాగ్,
				   hb_script_t   *script /*బహిర్గతం*/,
				   hb_language_t *language /*బహిర్గతం*/);


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
రకనిర్వచనం గణన {
  HB_OT_LAYOUT_GLYPH_CLASS_UNCLASSIFIED	= 0,
  HB_OT_LAYOUT_GLYPH_CLASS_పునాది_GLYPH	= 1,
  HB_OT_LAYOUT_GLYPH_CLASS_LIGATURE	= 2,
  HB_OT_LAYOUT_GLYPH_CLASS_MARK		= 3,
  HB_OT_LAYOUT_GLYPH_CLASS_COMPONENT	= 4
} hb_ot_layout_glyph_class_t;

HB_EXTERN hb_ot_layout_glyph_class_t
hb_ot_layout_get_glyph_class (hb_face_t      *face,
			      hb_codepoint_t  glyph);

HB_EXTERN శూన్య
hb_ot_layout_get_glyphs_లోపల_class (hb_face_t                  *face,
				  hb_ot_layout_glyph_class_t  klass,
				  hb_సమితి_t                   *glyphs /*బహిర్గతం*/);

/* Not that useful.  Provides list of attach points for a glyph that a
 * client may want to cache */
HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_ot_layout_get_attach_points (hb_face_t      *face,
				hb_codepoint_t  glyph,
				గుర్తులేని పూర్ణాంకం    ప్రారంభం_స్థానభ్రంశం,
				గుర్తులేని పూర్ణాంకం   *బిందువు_లెక్క /* IN/OUT */,
				గుర్తులేని పూర్ణాంకం   *బిందువు_array /*బహిర్గతం*/);

/* Ligature caret positions */
HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_ot_layout_get_ligature_carets (hb_font_t      *font,
				  hb_direction_t  direction,
				  hb_codepoint_t  glyph,
				  గుర్తులేని పూర్ణాంకం    ప్రారంభం_స్థానభ్రంశం,
				  గుర్తులేని పూర్ణాంకం   *caret_లెక్క /* IN/OUT */,
				  hb_position_t  *caret_array /*బహిర్గతం*/);


/*
 * GSUB/GPOS feature query and enumeration interface
 */

/**
 * HB_OT_LAYOUT_NO_SCRIPT_INDEX:
 *
 * Special value for script index indicating unsupported script.
 */
#నిర్వచించు HB_OT_LAYOUT_NO_SCRIPT_సూచీ		0xFFFFu
/**
 * HB_OT_LAYOUT_NO_FEATURE_INDEX:
 *
 * Special value for feature index indicating unsupported feature.
 */
#నిర్వచించు HB_OT_LAYOUT_NO_FEATURE_సూచీ		0xFFFFu
/**
 * HB_OT_LAYOUT_DEFAULT_LANGUAGE_INDEX:
 *
 * Special value for language index indicating default or unsupported language.
 */
#నిర్వచించు HB_OT_LAYOUT_DEFAULT_LANGUAGE_సూచీ	0xFFFFu
/**
 * HB_OT_LAYOUT_NO_VARIATIONS_INDEX:
 *
 * Special value for variations index indicating unsupported variation.
 */
#నిర్వచించు HB_OT_LAYOUT_NO_VARIATIONS_సూచీ	0xFFFFFFFFu

HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_ot_layout_పట్టిక_get_script_ట్యాగులు (hb_face_t    *face,
				    hb_ట్యాగ్_t      పట్టిక_ట్యాగ్_2,
				    గుర్తులేని పూర్ణాంకం  ప్రారంభం_స్థానభ్రంశం,
				    గుర్తులేని పూర్ణాంకం *script_లెక్క /* IN/OUT */,
				    hb_ట్యాగ్_t     *script_ట్యాగులు /*బహిర్గతం*/);

HB_EXTERN hb_bool_t
hb_ot_layout_పట్టిక_కనుగొను_script (hb_face_t    *face,
				hb_ట్యాగ్_t      పట్టిక_ట్యాగ్_2,
				hb_ట్యాగ్_t      script_ట్యాగ్,
				గుర్తులేని పూర్ణాంకం *script_సూచీ /*బహిర్గతం*/);

HB_EXTERN hb_bool_t
hb_ot_layout_పట్టిక_select_script (hb_face_t      *face,
				  hb_ట్యాగ్_t        పట్టిక_ట్యాగ్_2,
				  గుర్తులేని పూర్ణాంకం    script_లెక్క,
				  స్థిరరాశి hb_ట్యాగ్_t *script_ట్యాగులు,
				  గుర్తులేని పూర్ణాంకం   *script_సూచీ /*బహిర్గతం*/,
				  hb_ట్యాగ్_t       *chosen_script /*బహిర్గతం*/);

HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_ot_layout_పట్టిక_get_feature_ట్యాగులు (hb_face_t    *face,
				     hb_ట్యాగ్_t      పట్టిక_ట్యాగ్_2,
				     గుర్తులేని పూర్ణాంకం  ప్రారంభం_స్థానభ్రంశం,
				     గుర్తులేని పూర్ణాంకం *feature_లెక్క_2 /* IN/OUT */,
				     hb_ట్యాగ్_t     *feature_ట్యాగులు /*బహిర్గతం*/);

HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_ot_layout_script_get_language_ట్యాగులు (hb_face_t    *face,
				       hb_ట్యాగ్_t      పట్టిక_ట్యాగ్_2,
				       గుర్తులేని పూర్ణాంకం  script_సూచీ,
				       గుర్తులేని పూర్ణాంకం  ప్రారంభం_స్థానభ్రంశం,
				       గుర్తులేని పూర్ణాంకం *language_లెక్క /* IN/OUT */,
				       hb_ట్యాగ్_t     *language_ట్యాగులు /*బహిర్గతం*/);

HB_EXTERN hb_bool_t
hb_ot_layout_script_select_language (hb_face_t      *face,
				     hb_ట్యాగ్_t        పట్టిక_ట్యాగ్_2,
				     గుర్తులేని పూర్ణాంకం    script_సూచీ,
				     గుర్తులేని పూర్ణాంకం    language_లెక్క,
				     స్థిరరాశి hb_ట్యాగ్_t *language_ట్యాగులు,
				     గుర్తులేని పూర్ణాంకం   *language_సూచీ /*బహిర్గతం*/);

HB_EXTERN hb_bool_t
hb_ot_layout_script_select_language2 (hb_face_t      *face,
				     hb_ట్యాగ్_t        పట్టిక_ట్యాగ్_2,
				     గుర్తులేని పూర్ణాంకం    script_సూచీ,
				     గుర్తులేని పూర్ణాంకం    language_లెక్క,
				     స్థిరరాశి hb_ట్యాగ్_t *language_ట్యాగులు,
				     గుర్తులేని పూర్ణాంకం   *language_సూచీ /*బహిర్గతం*/,
				     hb_ట్యాగ్_t       *chosen_language /*బహిర్గతం*/);

HB_EXTERN hb_bool_t
hb_ot_layout_language_get_required_feature_సూచీ (hb_face_t    *face,
						  hb_ట్యాగ్_t      పట్టిక_ట్యాగ్_2,
						  గుర్తులేని పూర్ణాంకం  script_సూచీ,
						  గుర్తులేని పూర్ణాంకం  language_సూచీ,
						  గుర్తులేని పూర్ణాంకం *feature_సూచీ /*బహిర్గతం*/);

HB_EXTERN hb_bool_t
hb_ot_layout_language_get_required_feature (hb_face_t    *face,
					    hb_ట్యాగ్_t      పట్టిక_ట్యాగ్_2,
					    గుర్తులేని పూర్ణాంకం  script_సూచీ,
					    గుర్తులేని పూర్ణాంకం  language_సూచీ,
					    గుర్తులేని పూర్ణాంకం *feature_సూచీ /*బహిర్గతం*/,
					    hb_ట్యాగ్_t     *feature_ట్యాగ్ /*బహిర్గతం*/);

HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_ot_layout_language_get_feature_indexes (hb_face_t    *face,
					   hb_ట్యాగ్_t      పట్టిక_ట్యాగ్_2,
					   గుర్తులేని పూర్ణాంకం  script_సూచీ,
					   గుర్తులేని పూర్ణాంకం  language_సూచీ,
					   గుర్తులేని పూర్ణాంకం  ప్రారంభం_స్థానభ్రంశం,
					   గుర్తులేని పూర్ణాంకం *feature_లెక్క_2 /* IN/OUT */,
					   గుర్తులేని పూర్ణాంకం *feature_indexes /*బహిర్గతం*/);

HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_ot_layout_language_get_feature_ట్యాగులు (hb_face_t    *face,
					hb_ట్యాగ్_t      పట్టిక_ట్యాగ్_2,
					గుర్తులేని పూర్ణాంకం  script_సూచీ,
					గుర్తులేని పూర్ణాంకం  language_సూచీ,
					గుర్తులేని పూర్ణాంకం  ప్రారంభం_స్థానభ్రంశం,
					గుర్తులేని పూర్ణాంకం *feature_లెక్క_2 /* IN/OUT */,
					hb_ట్యాగ్_t     *feature_ట్యాగులు /*బహిర్గతం*/);

HB_EXTERN hb_bool_t
hb_ot_layout_language_కనుగొను_feature (hb_face_t    *face,
				    hb_ట్యాగ్_t      పట్టిక_ట్యాగ్_2,
				    గుర్తులేని పూర్ణాంకం  script_సూచీ,
				    గుర్తులేని పూర్ణాంకం  language_సూచీ,
				    hb_ట్యాగ్_t      feature_ట్యాగ్,
				    గుర్తులేని పూర్ణాంకం *feature_సూచీ /*బహిర్గతం*/);

HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_ot_layout_feature_get_lookups (hb_face_t    *face,
				  hb_ట్యాగ్_t      పట్టిక_ట్యాగ్_2,
				  గుర్తులేని పూర్ణాంకం  feature_సూచీ,
				  గుర్తులేని పూర్ణాంకం  ప్రారంభం_స్థానభ్రంశం,
				  గుర్తులేని పూర్ణాంకం *lookup_లెక్క /* IN/OUT */,
				  గుర్తులేని పూర్ణాంకం *lookup_indexes /*బహిర్గతం*/);

HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_ot_layout_పట్టిక_get_lookup_లెక్క (hb_face_t    *face,
				     hb_ట్యాగ్_t      పట్టిక_ట్యాగ్_2);

HB_EXTERN శూన్య
hb_ot_layout_collect_features (hb_face_t      *face,
			       hb_ట్యాగ్_t        పట్టిక_ట్యాగ్_2,
			       స్థిరరాశి hb_ట్యాగ్_t *scripts,
			       స్థిరరాశి hb_ట్యాగ్_t *languages,
			       స్థిరరాశి hb_ట్యాగ్_t *features,
			       hb_సమితి_t       *feature_indexes /*బహిర్గతం*/);

HB_EXTERN శూన్య
hb_ot_layout_collect_features_map (hb_face_t      *face,
				   hb_ట్యాగ్_t        పట్టిక_ట్యాగ్_2,
				   గుర్తులేని        script_సూచీ,
				   గుర్తులేని        language_సూచీ,
				   hb_map_t       *feature_map /*బహిర్గతం*/);

HB_EXTERN శూన్య
hb_ot_layout_collect_lookups (hb_face_t      *face,
			      hb_ట్యాగ్_t        పట్టిక_ట్యాగ్_2,
			      స్థిరరాశి hb_ట్యాగ్_t *scripts,
			      స్థిరరాశి hb_ట్యాగ్_t *languages,
			      స్థిరరాశి hb_ట్యాగ్_t *features,
			      hb_సమితి_t       *lookup_indexes /*బహిర్గతం*/);

HB_EXTERN శూన్య
hb_ot_layout_lookup_collect_glyphs (hb_face_t    *face,
				    hb_ట్యాగ్_t      పట్టిక_ట్యాగ్_2,
				    గుర్తులేని పూర్ణాంకం  lookup_సూచీ,
				    hb_సమితి_t     *glyphs_before, /* OUT.  May be NULL */
				    hb_సమితి_t     *glyphs_ఆదానం,  /* OUT.  May be NULL */
				    hb_సమితి_t     *glyphs_after,  /* OUT.  May be NULL */
				    hb_సమితి_t     *glyphs_output  /* OUT.  May be NULL */);


/* Variations support */

HB_EXTERN hb_bool_t
hb_ot_layout_పట్టిక_కనుగొను_feature_variations (hb_face_t    *face,
					    hb_ట్యాగ్_t      పట్టిక_ట్యాగ్_2,
					    స్థిరరాశి పూర్ణాంకం    *coords,
					    గుర్తులేని పూర్ణాంకం  అంకె_coords,
					    గుర్తులేని పూర్ణాంకం *variations_సూచీ /* out */);

HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_ot_layout_feature_with_variations_get_lookups (hb_face_t    *face,
						  hb_ట్యాగ్_t      పట్టిక_ట్యాగ్_2,
						  గుర్తులేని పూర్ణాంకం  feature_సూచీ,
						  గుర్తులేని పూర్ణాంకం  variations_సూచీ,
						  గుర్తులేని పూర్ణాంకం  ప్రారంభం_స్థానభ్రంశం,
						  గుర్తులేని పూర్ణాంకం *lookup_లెక్క /* IN/OUT */,
						  గుర్తులేని పూర్ణాంకం *lookup_indexes /*బహిర్గతం*/);


/*
 * GSUB
 */

HB_EXTERN hb_bool_t
hb_ot_layout_has_substitution (hb_face_t *face);

HB_EXTERN గుర్తులేని
hb_ot_layout_lookup_get_glyph_alternates (hb_face_t      *face,
					  గుర్తులేని        lookup_సూచీ,
					  hb_codepoint_t  glyph,
					  గుర్తులేని        ప్రారంభం_స్థానభ్రంశం,
					  గుర్తులేని       *alternate_లెక్క /* IN/OUT */,
					  hb_codepoint_t *alternate_glyphs /*బహిర్గతం*/);

HB_EXTERN hb_bool_t
hb_ot_layout_lookup_collect_glyph_alternates (hb_face_t *face,
					      గుర్తులేని   lookup_సూచీ,
					      hb_map_t  *alternate_లెక్క /* IN/OUT */,
					      hb_map_t  *alternate_glyphs /* IN/OUT */);

HB_EXTERN hb_bool_t
hb_ot_layout_lookup_would_substitute (hb_face_t            *face,
				      గుర్తులేని పూర్ణాంకం          lookup_సూచీ,
				      స్థిరరాశి hb_codepoint_t *glyphs,
				      గుర్తులేని పూర్ణాంకం          glyphs_నిడివి,
				      hb_bool_t             zero_context);

HB_EXTERN శూన్య
hb_ot_layout_lookup_substitute_సంవృతం (hb_face_t    *face,
					గుర్తులేని పూర్ణాంకం  lookup_సూచీ,
					hb_సమితి_t     *glyphs
					/*TODO , hb_bool_t  inclusive */);

HB_EXTERN శూన్య
hb_ot_layout_lookups_substitute_సంవృతం (hb_face_t      *face,
					 స్థిరరాశి hb_సమితి_t *lookups,
					 hb_సమితి_t       *glyphs);


/*
 * GPOS
 */

HB_EXTERN hb_bool_t
hb_ot_layout_has_positioning (hb_face_t *face);

/* Optical 'size' feature info.  Returns true if found.
 * https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#size */
HB_EXTERN hb_bool_t
hb_ot_layout_get_కొలత_params (hb_face_t       *face,
			      గుర్తులేని పూర్ణాంకం    *design_కొలత_2,       /* OUT.  May be NULL */
			      గుర్తులేని పూర్ణాంకం    *subfamily_id,      /* OUT.  May be NULL */
			      hb_ot_name_id_t *subfamily_name_id, /* OUT.  May be NULL */
			      గుర్తులేని పూర్ణాంకం    *range_ప్రారంభం_2,       /* OUT.  May be NULL */
			      గుర్తులేని పూర్ణాంకం    *range_చివర_2          /* OUT.  May be NULL */);

HB_EXTERN hb_position_t
hb_ot_layout_lookup_get_optical_bound (hb_font_t      *font,
				       గుర్తులేని        lookup_సూచీ,
				       hb_direction_t  direction,
				       hb_codepoint_t  glyph);


/*
 * GSUB/GPOS
 */

HB_EXTERN hb_bool_t
hb_ot_layout_feature_get_name_ids (hb_face_t       *face,
				   hb_ట్యాగ్_t         పట్టిక_ట్యాగ్_2,
				   గుర్తులేని పూర్ణాంకం     feature_సూచీ,
				   hb_ot_name_id_t *label_id             /* OUT.  May be NULL */,
				   hb_ot_name_id_t *tooltip_id           /* OUT.  May be NULL */,
				   hb_ot_name_id_t *sample_id            /* OUT.  May be NULL */,
				   గుర్తులేని పూర్ణాంకం    *అంకె_named_parameters /* OUT.  May be NULL */,
				   hb_ot_name_id_t *మొదటి_param_id       /* OUT.  May be NULL */);


HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_ot_layout_feature_get_characters (hb_face_t      *face,
				     hb_ట్యాగ్_t        పట్టిక_ట్యాగ్_2,
				     గుర్తులేని పూర్ణాంకం    feature_సూచీ,
				     గుర్తులేని పూర్ణాంకం    ప్రారంభం_స్థానభ్రంశం,
				     గుర్తులేని పూర్ణాంకం   *char_లెక్క    /* IN/OUT.  May be NULL */,
				     hb_codepoint_t *characters    /* OUT.     May be NULL */);


/*
 * BASE
 */

HB_EXTERN hb_bool_t
hb_ot_layout_get_font_extents (hb_font_t         *font,
			       hb_direction_t     direction,
			       hb_ట్యాగ్_t           script_ట్యాగ్,
			       hb_ట్యాగ్_t           language_ట్యాగ్,
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
రకనిర్వచనం గణన {
  HB_OT_LAYOUT_BASELINE_ట్యాగ్_ROMAN			= HB_ట్యాగ్ ('r','o','m','n'),
  HB_OT_LAYOUT_BASELINE_ట్యాగ్_HANGING			= HB_ట్యాగ్ ('h','a','n','g'),
  HB_OT_LAYOUT_BASELINE_ట్యాగ్_IDEO_FACE_BOTTOM_OR_ఎడమ	= HB_ట్యాగ్ ('i','c','f','b'),
  HB_OT_LAYOUT_BASELINE_ట్యాగ్_IDEO_FACE_శిఖరం_OR_కుడి	= HB_ట్యాగ్ ('i','c','f','t'),
  HB_OT_LAYOUT_BASELINE_ట్యాగ్_IDEO_FACE_CENTRAL		= HB_ట్యాగ్ ('I','c','f','c'),
  HB_OT_LAYOUT_BASELINE_ట్యాగ్_IDEO_EMBOX_BOTTOM_OR_ఎడమ	= HB_ట్యాగ్ ('i','d','e','o'),
  HB_OT_LAYOUT_BASELINE_ట్యాగ్_IDEO_EMBOX_శిఖరం_OR_కుడి	= HB_ట్యాగ్ ('i','d','t','p'),
  HB_OT_LAYOUT_BASELINE_ట్యాగ్_IDEO_EMBOX_CENTRAL		= HB_ట్యాగ్ ('I','d','c','e'),
  HB_OT_LAYOUT_BASELINE_ట్యాగ్_MATH			= HB_ట్యాగ్ ('m','a','t','h'),

  /*< private >*/
  HB_OT_LAYOUT_BASELINE_ట్యాగ్_MAX_విలువ = HB_ట్యాగ్_MAX_SIGNED /*< skip >*/
} hb_ot_layout_baseline_ట్యాగ్_t;

HB_EXTERN hb_ot_layout_baseline_ట్యాగ్_t
hb_ot_layout_get_horizontal_baseline_ట్యాగ్_for_script (hb_script_t script);

HB_EXTERN hb_bool_t
hb_ot_layout_get_baseline (hb_font_t                   *font,
			   hb_ot_layout_baseline_ట్యాగ్_t  baseline_ట్యాగ్,
			   hb_direction_t               direction,
			   hb_ట్యాగ్_t                     script_ట్యాగ్,
			   hb_ట్యాగ్_t                     language_ట్యాగ్,
			   hb_position_t               *coord        /* OUT.  May be NULL. */);

HB_EXTERN hb_bool_t
hb_ot_layout_get_baseline2 (hb_font_t                   *font,
			    hb_ot_layout_baseline_ట్యాగ్_t  baseline_ట్యాగ్,
			    hb_direction_t               direction,
			    hb_script_t                  script,
			    hb_language_t                language,
			    hb_position_t               *coord        /* OUT.  May be NULL. */);

HB_EXTERN శూన్య
hb_ot_layout_get_baseline_with_fallback (hb_font_t                   *font,
					 hb_ot_layout_baseline_ట్యాగ్_t  baseline_ట్యాగ్,
					 hb_direction_t               direction,
					 hb_ట్యాగ్_t                     script_ట్యాగ్,
					 hb_ట్యాగ్_t                     language_ట్యాగ్,
					 hb_position_t               *coord        /*బహిర్గతం*/);

HB_EXTERN శూన్య
hb_ot_layout_get_baseline_with_fallback2 (hb_font_t                   *font,
					  hb_ot_layout_baseline_ట్యాగ్_t  baseline_ట్యాగ్,
					  hb_direction_t               direction,
					  hb_script_t                  script,
					  hb_language_t                language,
					  hb_position_t               *coord        /*బహిర్గతం*/);

HB_చివర_DECLS

#అయితే_ముగింపు /* HB_OT_LAYOUT_H */

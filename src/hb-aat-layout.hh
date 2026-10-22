/*
 * Copyright © 2017  Google, Inc.
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

#నిర్వచితం_కాకపోతే HB_AAT_LAYOUT_HH
#నిర్వచించు HB_AAT_LAYOUT_HH

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-ot-shape.hh"
#అన్తర్భావ "hb-aat-ltag-table.hh"

/* https://developer.apple.com/documentation/coretext/1508745-ctfontcreatewithgraphicsfont */
#నిర్వచించు HB_CORETEXT_DEFAULT_FONT_కొలత 12.f

నిర్మాణం hb_aat_feature_mapping_t
{
  hb_ట్యాగ్_t ot_Feature_ట్యాగ్;
  hb_aat_layout_feature_type_t aatFeatureType;
  hb_aat_layout_feature_selector_t selector_గమ్యం_Enable;
  hb_aat_layout_feature_selector_t selector_గమ్యం_Disable;

  పూర్ణాంకం cmp (hb_ట్యాగ్_t key) స్థిరరాశి
  { తిరిగి key < ot_Feature_ట్యాగ్ ? -1 : key > ot_Feature_ట్యాగ్ ? 1 : 0; }
};

HB_అంతర్గత స్థిరరాశి hb_aat_feature_mapping_t *
hb_aat_layout_కనుగొను_feature_mapping (hb_ట్యాగ్_t ట్యాగ్_2);

HB_అంతర్గత శూన్య
hb_aat_layout_compile_map (స్థిరరాశి hb_aat_map_builder_t *mapper,
			   hb_aat_map_t *map);

HB_అంతర్గత శూన్య
hb_aat_layout_substitute (స్థిరరాశి hb_ot_shape_plan_t *plan,
			  hb_font_t *font,
			  hb_బఫరు_t *బఫరు,
			  స్థిరరాశి hb_feature_t *features,
			  గుర్తులేని అంకె_features);

HB_అంతర్గత శూన్య
hb_aat_layout_remove_deleted_glyphs (hb_బఫరు_t *బఫరు);

HB_అంతర్గత శూన్య
hb_aat_layout_position (స్థిరరాశి hb_ot_shape_plan_t *plan,
			hb_font_t *font,
			hb_బఫరు_t *బఫరు);

HB_అంతర్గత శూన్య
hb_aat_layout_track (స్థిరరాశి hb_ot_shape_plan_t *plan,
		     hb_font_t *font,
		     hb_బఫరు_t *బఫరు);


#అయితే_ముగింపు /* HB_AAT_LAYOUT_HH */

/*
 * Copyright © 2010  Google, Inc.
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

#నిర్వచితం_కాకపోతే HB_OT_SHAPE_HH
#నిర్వచించు HB_OT_SHAPE_HH

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-ot-map.hh"
#అన్తర్భావ "hb-aat-map.hh"


HB_అంతర్గత తార్కిక
hb_ot_layout_పట్టిక_get_feature_variations_state (hb_face_t             *face,
						   hb_ట్యాగ్_t               పట్టిక_ట్యాగ్_2,
						   స్థిరరాశి పూర్ణాంకం             *coords,
						   గుర్తులేని పూర్ణాంకం           అంకె_coords,
						   గుర్తులేని పూర్ణాంకం          *variations_సూచీ,
						   hb_సదిశ_t<గుర్తులేని> *lookup_conditions);

HB_అంతర్గత తార్కిక
_hb_ot_layout_feature_get_lookup_variations (hb_face_t *face,
                                             hb_ట్యాగ్_t పట్టిక_ట్యాగ్_2,
                                             గుర్తులేని feature_సూచీ,
                                             గుర్తులేని variations_సూచీ,
                                             hb_sorted_array_t<స్థిరరాశి గుర్తులేని> conditions,
                                             hb_సమితి_t *lookup_indexes);

నిర్మాణం hb_ot_shape_plan_కీలకం_t
{
  గుర్తులేని పూర్ణాంకం variations_సూచీ[2];
  /* Sorted offsets of distinct true conditions within FeatureVariations.
   * Different outcomes may conservatively separate equivalent lookup sets. */
  hb_సదిశ_t<గుర్తులేని> lookup_conditions[2];

  తార్కిక init (hb_face_t *face,
	     స్థిరరాశి పూర్ణాంకం *coords,
	     గుర్తులేని   అంకె_coords)
  {
    కొరకు (గుర్తులేని పూర్ణాంకం పట్టిక_సూచీ = 0; పట్టిక_సూచీ < 2; పట్టిక_సూచీ++)
      ఒకవేళ (unlikely (!hb_ot_layout_పట్టిక_get_feature_variations_state (
			face,
			పట్టిక_ట్యాగులు[పట్టిక_సూచీ],
			coords,
			అంకె_coords,
			&variations_సూచీ[పట్టిక_సూచీ],
			&lookup_conditions[పట్టిక_సూచీ])))
      {
	కొరకు (స్వయంచాలక &v : lookup_conditions)
	  v.fini ();
	తిరిగి అబద్ధం;
      }
    తిరిగి నిజం;
  }

  శూన్య fini ()
  {
    కొరకు (స్వయంచాలక &v : lookup_conditions)
      v.fini ();
  }

  తార్కిక get_lookup_variations (hb_face_t *face,
                              గుర్తులేని పూర్ణాంకం పట్టిక_సూచీ,
			      గుర్తులేని పూర్ణాంకం feature_సూచీ,
			      hb_సమితి_t *lookups) స్థిరరాశి
  {
    తిరిగి _hb_ot_layout_feature_get_lookup_variations (
	face, పట్టిక_ట్యాగులు[పట్టిక_సూచీ], feature_సూచీ, variations_సూచీ[పట్టిక_సూచీ],
	lookup_conditions[పట్టిక_సూచీ].as_sorted_array (), lookups);
  }

  తార్కిక equal (స్థిరరాశి hb_ot_shape_plan_కీలకం_t *other)
  {
    తిరిగి variations_సూచీ[0] == other->variations_సూచీ[0] &&
	   variations_సూచీ[1] == other->variations_సూచీ[1] &&
	   lookup_conditions[0] == other->lookup_conditions[0] &&
	   lookup_conditions[1] == other->lookup_conditions[1];
  }
};


నిర్మాణం hb_shape_plan_కీలకం_t;

నిర్మాణం hb_ot_shape_plan_t
{
  ~hb_ot_shape_plan_t () { fini (); }

  hb_segment_properties_t props;
  స్థిరరాశి నిర్మాణం hb_ot_shaper_t *shaper;
  hb_ot_map_t map;
  hb_aat_map_t aat_map;
  స్థిరరాశి శూన్య *దత్తాంశం;
#నిర్వచితం_కాకపోతే HB_NO_OT_SHAPE_FRACTIONS
  hb_ముసుగు_t frac_ముసుగు, numr_ముసుగు, dnom_ముసుగు;
#లేకపోతే
  స్థిర constexpr hb_ముసుగు_t frac_ముసుగు = 0;
  స్థిర constexpr hb_ముసుగు_t numr_ముసుగు = 0;
  స్థిర constexpr hb_ముసుగు_t dnom_ముసుగు = 0;
#అయితే_ముగింపు
  hb_ముసుగు_t rtlm_ముసుగు;
#నిర్వచితం_కాకపోతే HB_NO_OT_KERN
  hb_ముసుగు_t kern_ముసుగు;
#లేకపోతే
  స్థిర constexpr hb_ముసుగు_t kern_ముసుగు = 0;
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_NO_OT_KERN
  తార్కిక requested_kerning : 1;
#లేకపోతే
  స్థిర constexpr తార్కిక requested_kerning = అబద్ధం;
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే HB_NO_OT_SHAPE_FRACTIONS
  తార్కిక has_frac : 1;
#లేకపోతే
  స్థిర constexpr తార్కిక has_frac = అబద్ధం;
#అయితే_ముగింపు
  తార్కిక has_vert : 1;
  తార్కిక has_gpos_mark : 1;
  తార్కిక zero_marks : 1;
  తార్కిక fallback_glyph_classes : 1;
  తార్కిక fallback_mark_positioning : 1;
  తార్కిక adjust_mark_positioning_when_zeroing : 1;

  తార్కిక apply_gpos : 1;
#నిర్వచితం_కాకపోతే HB_NO_OT_KERN
  తార్కిక apply_kern : 1;
#లేకపోతే
  స్థిర constexpr తార్కిక apply_kern = అబద్ధం;
#అయితే_ముగింపు
  తార్కిక apply_fallback_kern : 1;
#నిర్వచితం_కాకపోతే HB_NO_AAT_SHAPE
  తార్కిక apply_kerx : 1;
  తార్కిక apply_morx : 1;
  తార్కిక apply_trak : 1;
#లేకపోతే
  స్థిర constexpr తార్కిక apply_kerx = అబద్ధం;
  స్థిర constexpr తార్కిక apply_morx = అబద్ధం;
  స్థిర constexpr తార్కిక apply_trak = అబద్ధం;
#అయితే_ముగింపు

  శూన్య collect_lookups (hb_ట్యాగ్_t పట్టిక_ట్యాగ్_2, hb_సమితి_t *lookups) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం పట్టిక_సూచీ;
    ఎంపిక (పట్టిక_ట్యాగ్_2) {
      స్థితి HB_OT_ట్యాగ్_GSUB: పట్టిక_సూచీ = 0; ఆపు;
      స్థితి HB_OT_ట్యాగ్_GPOS: పట్టిక_సూచీ = 1; ఆపు;
      అప్రమేయ: తిరిగి;
    }
    map.collect_lookups (పట్టిక_సూచీ, lookups);
  }

  HB_అంతర్గత తార్కిక init0 (hb_face_t                     *face,
			  స్థిరరాశి hb_shape_plan_కీలకం_t     *key);
  HB_అంతర్గత శూన్య fini ();

  HB_అంతర్గత శూన్య substitute (hb_font_t *font, hb_బఫరు_t *బఫరు) స్థిరరాశి;
  HB_అంతర్గత శూన్య position (hb_font_t *font, hb_బఫరు_t *బఫరు) స్థిరరాశి;
};

నిర్మాణం hb_shape_plan_t;

నిర్మాణం hb_ot_shape_planner_t
{
  /* In the order that they are filled in. */
  hb_face_t *face;
  hb_segment_properties_t props;
  hb_ot_map_builder_t map;
#నిర్వచితం_కాకపోతే HB_NO_AAT_SHAPE
  hb_aat_map_builder_t aat_map;
  తార్కిక apply_morx : 1;
#లేకపోతే
  స్థిర constexpr తార్కిక apply_morx = అబద్ధం;
#అయితే_ముగింపు
  తార్కిక script_zero_marks : 1;
  తార్కిక script_fallback_position : 1;
  స్థిరరాశి నిర్మాణం hb_ot_shaper_t *shaper;

  HB_అంతర్గత hb_ot_shape_planner_t (hb_face_t                     *face,
				     స్థిరరాశి hb_segment_properties_t &props);

  HB_అంతర్గత శూన్య compile (hb_ot_shape_plan_t           &plan,
			    స్థిరరాశి hb_ot_shape_plan_కీలకం_t &key);
};


#అయితే_ముగింపు /* HB_OT_SHAPE_HH */

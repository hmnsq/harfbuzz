/*
 * Copyright © 2018  Google, Inc.
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
 * Google Author(s): Garret Rieger, Roderick Sheeter
 */

#నిర్వచితం_కాకపోతే HB_SUBSET_PLAN_HH
#నిర్వచించు HB_SUBSET_PLAN_HH

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-subset.h"
#అన్తర్భావ "hb-subset-input.hh"
#అన్తర్భావ "hb-subset-accelerator.hh"

#అన్తర్భావ "hb-map.hh"
#అన్తర్భావ "hb-bimap.hh"
#అన్తర్భావ "hb-set.hh"

నామాకాశ OT {
నిర్మాణం Feature;
}

నిర్మాణం os2_info_t {
  hb_codepoint_t min_cmap_codepoint;
  hb_codepoint_t max_cmap_codepoint;
};

రకనిర్వచనం నిర్మాణం os2_info_t os2_info_t;

నిర్మాణం తల_maxp_info_t
{
  తల_maxp_info_t ()
      :xMin (0x7FFF), xMax (-0x7FFF), yMin (0x7FFF), yMax (-0x7FFF),
      maxPoints (0), maxContours (0),
      max_సంయుక్త_Points (0),
      max_సంయుక్త_Contours (0),
      maxComponentElements (0),
      max_Component_లోతు (0),
      allXMinIsLsb (నిజం) {}

  పూర్ణాంకం xMin;
  పూర్ణాంకం xMax;
  పూర్ణాంకం yMin;
  పూర్ణాంకం yMax;
  గుర్తులేని maxPoints;
  గుర్తులేని maxContours;
  గుర్తులేని max_సంయుక్త_Points;
  గుర్తులేని max_సంయుక్త_Contours;
  గుర్తులేని maxComponentElements;
  గుర్తులేని max_Component_లోతు;
  తార్కిక allXMinIsLsb;
};

రకనిర్వచనం నిర్మాణం తల_maxp_info_t తల_maxp_info_t;

నిర్మాణం contour_బిందువు_t
{
  శూన్య init (దశాంశం x_ = 0.f, దశాంశం y_ = 0.f, తార్కిక is_చివర_బిందువు_2 = అబద్ధం)
  { flag = 0; x = x_; y = y_; is_చివర_బిందువు = is_చివర_బిందువు_2; }

  శూన్య transform (స్థిరరాశి దశాంశం (&మాతృక)[4])
  {
    దశాంశం x_ = x * మాతృక[0] + y * మాతృక[2];
	  y  = x * మాతృక[1] + y * మాతృక[3];
    x  = x_;
  }

  శూన్య కూడు_delta (దశాంశం delta_x, దశాంశం delta_y)
  {
    x += delta_x;
    y += delta_y;
  }

  HB_ALWAYS_INLINE
  శూన్య translate (స్థిరరాశి contour_బిందువు_t &p) { x += p.x; y += p.y; }

  దశాంశం x;
  దశాంశం y;
  uint8_t flag;
  తార్కిక is_చివర_బిందువు;
};

నిర్మాణం contour_బిందువు_సదిశ_t : hb_సదిశ_t<contour_బిందువు_t>
{
  తార్కిక కూడు_deltas (hb_array_t<స్థిరరాశి దశాంశం> deltas_x,
                   hb_array_t<స్థిరరాశి దశాంశం> deltas_y,
                   hb_array_t<స్థిరరాశి తార్కిక> indices)
  {
    ఒకవేళ (indices.నిడివి != deltas_x.నిడివి ||
        indices.నిడివి != deltas_y.నిడివి)
      తిరిగి అబద్ధం;

    కొరకు (గుర్తులేని i = 0; i < indices.నిడివి; i++)
    {
      ఒకవేళ (!indices.arrayZ[i]) కొనసాగు;
      arrayZ[i].కూడు_delta (deltas_x.arrayZ[i], deltas_y.arrayZ[i]);
    }
    తిరిగి నిజం;
  }
};

నామాకాశ OT {
  నిర్మాణం cff1_subset_accelerator_t;
  నిర్మాణం cff2_subset_accelerator_t;
}

నిర్మాణం hb_subset_plan_t
{
  HB_అంతర్గత hb_subset_plan_t (hb_face_t *,
				స్థిరరాశి hb_subset_ఆదానం_t *ఆదానం);

  HB_అంతర్గత ~hb_subset_plan_t();

  hb_object_శీర్షిక_t శీర్షిక;

  తార్కిక successful;
  గుర్తులేని flags;
  తార్కిక attach_accelerator_దత్తాంశం = అబద్ధం;
  తార్కిక force_long_loca = అబద్ధం;

  // The glyph subset
  hb_map_t *codepoint_గమ్యం_glyph; // Needs to be heap-allocated

  // Old -> New glyph id mapping
  hb_map_t *glyph_map; // Needs to be heap-allocated
  hb_map_t *తిరగేయి_glyph_map; // Needs to be heap-allocated

  // Plan is only good for a specific source/dest so keep them with it
  hb_face_t *ఆధారం;
#నిర్వచితం_కాకపోతే HB_NO_SUBSET_CFF
  // These have to be immediately after source:
  hb_face_lazy_loader_t<OT::cff1_subset_accelerator_t, 1> cff1_accel;
  hb_face_lazy_loader_t<OT::cff2_subset_accelerator_t, 2> cff2_accel;
#అయితే_ముగింపు

  hb_face_t *dest;

  గుర్తులేని పూర్ణాంకం అంకె_output_glyphs_2;

  తార్కిక all_axes_pinned;
  తార్కిక pinned_at_default;
  తార్కిక has_seac;

  // whether to insert a catch-all FeatureVariationRecord
  తార్కిక gsub_చొప్పించు_catch_all_feature_variation_rec;
  తార్కిక gpos_చొప్పించు_catch_all_feature_variation_rec;

  // whether GDEF ItemVariationStore is retained
  మార్చదగిన తార్కిక has_gdef_varstore;

  // whether we're doing avar2 partial instancing
  తార్కిక has_avar2;

#నిర్వచించు HB_SUBSET_PLAN_MEMBER(Type, Name) Type Name;
#అన్తర్భావ "hb-subset-plan-member-list.hh"
#నిర్వచనం_రద్దు HB_SUBSET_PLAN_MEMBER

  //recalculated head/maxp table info after instancing
  మార్చదగిన తల_maxp_info_t తల_maxp_info;

  os2_info_t os2_info;

  స్థిరరాశి hb_subset_accelerator_t* accelerator = శూన్యసూచిక;
  hb_subset_accelerator_t* inprogress_accelerator = శూన్యసూచిక;

 బహిరంగ:

  మూస<రకం_పేరు T>
  నిర్మాణం ఆధారం_పట్టిక_loader
  {
    hb_blob_ptr_t<T> క్రియాకారకం () (hb_subset_plan_t *plan)
    {
      hb_తాళం_t lock (plan->accelerator ? &plan->accelerator->sanitized_పట్టిక_cache_తాళం : శూన్యసూచిక);

      స్వయంచాలక *cache = plan->accelerator ? &plan->accelerator->sanitized_పట్టిక_cache : &plan->sanitized_పట్టిక_cache;
      ఒకవేళ (cache
	  && !cache->లోపల_error ()
	  && cache->has (+T::పట్టిక_ట్యాగ్)) {
	తిరిగి hb_blob_reference (cache->get (+T::పట్టిక_ట్యాగ్).get ());
      }

      hb::unique_ptr<hb_blob_t> పట్టిక_blob {hb_sanitize_context_t ().reference_పట్టిక<T> (plan->ఆధారం)};
      hb_blob_t* ret = hb_blob_reference (పట్టిక_blob.get ());

      ఒకవేళ (likely (cache))
	cache->సమితి (+T::పట్టిక_ట్యాగ్, std::move (పట్టిక_blob));

      తిరిగి ret;
    }
  };

  మూస<రకం_పేరు T>
  స్వయంచాలక ఆధారం_పట్టిక() HB_AUTO_RETURN (ఆధారం_పట్టిక_loader<T> {} (ఇది))

  తార్కిక లోపల_error () స్థిరరాశి { తిరిగి !successful; }

  HB_అంతర్గత ద్విగుణ layout_variation_delta (గుర్తులేని var_idx);

  తార్కిక check_success(తార్కిక success)
  {
    successful = (successful && success);
    తిరిగి successful;
  }

  /*
   * The set of input glyph ids which will be retained in the subset.
   * Does NOT include ids kept due to retain_gids. You probably want to use
   * glyph_map/reverse_glyph_map.
   */
  అంతఃవరుస స్థిరరాశి hb_సమితి_t *
  glyphset () స్థిరరాశి
  {
    తిరిగి &_glyphset;
  }

  /*
   * The set of input glyph ids which will be retained in the subset.
   */
  అంతఃవరుస స్థిరరాశి hb_సమితి_t *
  glyphset_gsub () స్థిరరాశి
  {
    తిరిగి &_glyphset_gsub;
  }

  /*
   * Old-to-new glyph id mapping for GSUB glyphs, through the flat
   * array; returns HB_MAP_VALUE_INVALID for glyphs not retained.
   */
  అంతఃవరుస hb_codepoint_t
  map_gsub_glyph (hb_codepoint_t old_gid) స్థిరరాశి
  {
    తిరిగి old_gid < glyph_map_gsub_flat.నిడివి
	 ? glyph_map_gsub_flat.arrayZ[old_gid]
	 : HB_MAP_విలువ_INVALID;
  }

  /*
   * The total number of output glyphs in the final subset.
   */
  అంతఃవరుస గుర్తులేని పూర్ణాంకం
  అంకె_output_glyphs () స్థిరరాశి
  {
    తిరిగి అంకె_output_glyphs_2;
  }

  అంతఃవరుస తార్కిక new_gid_for_codepoint (hb_codepoint_t codepoint,
				     hb_codepoint_t *new_gid) స్థిరరాశి
  {
    hb_codepoint_t old_gid = codepoint_గమ్యం_glyph->get (codepoint);
    ఒకవేళ (old_gid == HB_MAP_విలువ_INVALID)
      తిరిగి అబద్ధం;

    తిరిగి new_gid_for_old_gid (old_gid, new_gid);
  }

  అంతఃవరుస తార్కిక new_gid_for_old_gid (hb_codepoint_t old_gid,
				   hb_codepoint_t *new_gid) స్థిరరాశి
  {
    hb_codepoint_t gid = glyph_map->get (old_gid);
    ఒకవేళ (gid == HB_MAP_విలువ_INVALID)
      తిరిగి అబద్ధం;

    *new_gid = gid;
    తిరిగి నిజం;
  }

  అంతఃవరుస తార్కిక old_gid_for_new_gid (hb_codepoint_t  new_gid,
				   hb_codepoint_t *old_gid) స్థిరరాశి
  {
    hb_codepoint_t gid = తిరగేయి_glyph_map->get (new_gid);
    ఒకవేళ (gid == HB_MAP_విలువ_INVALID)
      తిరిగి అబద్ధం;

    *old_gid = gid;
    తిరిగి నిజం;
  }

  అంతఃవరుస తార్కిక
  కూడు_పట్టిక (hb_ట్యాగ్_t ట్యాగ్_2,
	     hb_blob_t *contents)
  {
    ఒకవేళ (HB_DEBUG_SUBSET)
    {
      hb_blob_t *ఆధారం_blob = ఆధారం->reference_పట్టిక (ట్యాగ్_2);
      DEBUG_సందేశం(SUBSET, శూన్యసూచిక, "add table %c%c%c%c, dest %u bytes, source %u bytes",
		HB_UNTAG(ట్యాగ్_2),
		hb_blob_get_నిడివి (contents),
		hb_blob_get_నిడివి (ఆధారం_blob));
      hb_blob_destroy (ఆధారం_blob);
    }
    తిరిగి hb_face_builder_కూడు_పట్టిక (dest, ట్యాగ్_2, contents);
  }
};

// hb-subset-plan implementation is split into multiple files to keep
// compile times more reasonable:
// - hb-subset-plan.cc
// - hb-subset-plan-layout.cc
// - hb-subset-plan-var.cc
//
// The functions below are those needed to connect the split files
// above together.
HB_అంతర్గత శూన్య
remap_indexes (స్థిరరాశి hb_సమితి_t *indexes,
               hb_map_t       *mapping /*బహిర్గతం*/);


#నిర్వచితం_కాకపోతే HB_NO_VAR
మూస<రకం_పేరు అంశం_Var_Store>
HB_అంతర్గత శూన్య
remap_variation_indices (స్థిరరాశి అంశం_Var_Store &var_store,
                         స్థిరరాశి hb_సమితి_t &variation_indices,
                         స్థిరరాశి hb_సదిశ_t<పూర్ణాంకం>& normalized_coords,
                         తార్కిక calculate_delta, /* not pinned at default */
                         తార్కిక no_variations, /* all axes pinned */
                         hb_hashmap_t<గుర్తులేని, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> &variation_idx_delta_map /*బహిర్గతం*/);


మూస<రకం_పేరు Delta_సమితి_సూచీ_Map>
HB_అంతర్గత శూన్య
remap_colrv1_delta_సమితి_సూచీ_indices (స్థిరరాశి Delta_సమితి_సూచీ_Map &సూచీ_map,
                                      స్థిరరాశి hb_సమితి_t &delta_సమితి_idxes,
                                      hb_hashmap_t<గుర్తులేని, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /*బహిర్గతం*/);


HB_అంతర్గత శూన్య
generate_varstore_inner_maps (స్థిరరాశి hb_సమితి_t& varidx_సమితి,
                              గుర్తులేని subtable_లెక్క,
                              hb_సదిశ_t<hb_inc_bimap_t> &inner_maps /*బహిర్గతం*/);

HB_అంతర్గత తార్కిక
normalize_axes_location (hb_face_t *face, hb_subset_plan_t *plan);

HB_అంతర్గత శూన్య
update_instance_metrics_map_from_cff2 (hb_subset_plan_t *plan);

HB_అంతర్గత తార్కిక
get_instance_glyphs_contour_points (hb_subset_plan_t *plan);

#నిర్వచితం_కాకపోతే HB_NO_పునాది
HB_అంతర్గత శూన్య
collect_పునాది_variation_indices (hb_subset_plan_t* plan);
#అయితే_ముగింపు
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_NO_SUBSET_LAYOUT
రకనిర్వచనం hb_hashmap_t<గుర్తులేని, hb::unique_ptr<hb_సమితి_t>> script_langsys_map;

HB_అంతర్గత శూన్య
remap_వాడిన_mark_sets (hb_subset_plan_t *plan,
                      hb_map_t& వాడిన_mark_sets_map);

HB_అంతర్గత శూన్య
layout_nameid_సంవృతం (hb_subset_plan_t* plan,
                       hb_సమితి_t* drop_tables);

HB_అంతర్గత శూన్య
layout_populate_gids_గమ్యం_retain (hb_subset_plan_t* plan,
                                hb_సమితి_t* drop_tables);

HB_అంతర్గత శూన్య
collect_layout_variation_indices (hb_subset_plan_t* plan);
#అయితే_ముగింపు


#అయితే_ముగింపు /* HB_SUBSET_PLAN_HH */

/*
 * Copyright © 2023  Google, Inc.
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
 * Google Author(s): Garret Rieger, Qunxin Liu, Roderick Sheeter
 */

#అన్తర్భావ "hb-subset-plan.hh"

#అన్తర్భావ "hb-ot-layout-gdef-table.hh"
#అన్తర్భావ "hb-ot-layout-gpos-table.hh"
#అన్తర్భావ "hb-ot-layout-gsub-table.hh"
#అన్తర్భావ "hb-ot-math-table.hh"

ఉపయోగించి OT::Layout::GSUB;
ఉపయోగించి OT::Layout::GPOS;

#నిర్వచితం_కాకపోతే HB_NO_SUBSET_LAYOUT

ద్విగుణ
hb_subset_plan_t::layout_variation_delta (గుర్తులేని var_idx HB_వాడని)
{
#నిర్వచితం_కాకపోతే HB_NO_VAR
  hb_blob_ptr_t<OT::GDEF> gdef = ఆధారం_పట్టిక<OT::GDEF> ();
  ద్విగుణ delta = gdef->get_var_store ().get_delta<ద్విగుణ> (var_idx,
                                                normalized_coords.as_array ());
  gdef.destroy ();
  తిరిగి delta;
#లేకపోతే
  తిరిగి 0.f;
#అయితే_ముగింపు
}

శూన్య
remap_వాడిన_mark_sets (hb_subset_plan_t *plan,
                      hb_map_t& వాడిన_mark_sets_map)
{
  hb_blob_ptr_t<OT::GDEF> gdef = plan->ఆధారం_పట్టిక<OT::GDEF> ();

  ఒకవేళ (!gdef->has_దత్తాంశం () || !gdef->has_mark_glyph_sets ())
  {
    gdef.destroy ();
    తిరిగి;
  }

  hb_సమితి_t వాడిన_mark_sets;
  gdef->get_mark_glyph_sets ().collect_వాడిన_mark_sets (plan->_glyphset_gsub, వాడిన_mark_sets);
  gdef.destroy ();

  remap_indexes (&వాడిన_mark_sets, &వాడిన_mark_sets_map);
}

/*
 * Removes all tags from 'tags' that are not in filter. Additionally eliminates any duplicates.
 * Returns true if anything was removed (not including duplicates).
 */
స్థిర తార్కిక filter_ట్యాగ్_list(hb_సదిశ_t<hb_ట్యాగ్_t>* ట్యాగులు, /* IN/OUT */
                             స్థిరరాశి hb_సమితి_t* filter)
{
  hb_సదిశ_t<hb_ట్యాగ్_t> బహిర్గతం;
  బహిర్గతం.alloc (ట్యాగులు->get_కొలత() + 1); // +1 is to allocate room for the null terminator.

  తార్కిక removed = అబద్ధం;
  hb_సమితి_t సందర్శించిన;

  కొరకు (hb_ట్యాగ్_t ట్యాగ్_2 : *ట్యాగులు)
  {
    ఒకవేళ (!ట్యాగ్_2) కొనసాగు;
    ఒకవేళ (సందర్శించిన.has (ట్యాగ్_2)) కొనసాగు;

    ఒకవేళ (!filter->has (ట్యాగ్_2))
    {
      removed = నిజం;
      కొనసాగు;
    }

    సందర్శించిన.కూడు (ట్యాగ్_2);
    బహిర్గతం.push (ట్యాగ్_2);
  }

  // The collect function needs a null element to signal end of the array.
  బహిర్గతం.push (HB_ట్యాగ్_NONE);

  hb_మార్పిడి (బహిర్గతం, *ట్యాగులు);
  తిరిగి removed;
}

మూస <రకం_పేరు T>
స్థిర తార్కిక
_collect_instanced_lookup_variations (
    hb_subset_plan_t *plan,
    స్థిరరాశి T& పట్టిక,
    స్థిరరాశి hb_సమితి_t *feature_indices,
    hb_hashmap_t<గుర్తులేని, hb::shared_ptr<hb_సమితి_t>> *lookup_variations)
{
  hb_blob_ptr_t<OT::GDEF> gdef = plan->ఆధారం_పట్టిక<OT::GDEF> ();
  OT::అంశం_Var_Store_Instancer instancer (&gdef->get_var_store (), శూన్యసూచిక,
				       plan->normalized_coords.as_array ());
  hb_సదిశ_t<గుర్తులేని> state;
  గుర్తులేని variations_సూచీ;
  తార్కిక success = పట్టిక.get_feature_variations_state (
	plan->normalized_coords.arrayZ,
	plan->normalized_coords.నిడివి,
	&variations_సూచీ,
	&instancer,
	&state);
  gdef.destroy ();

  ఒకవేళ (unlikely (!success || state.లోపల_error ()))
    తిరిగి plan->check_success (అబద్ధం);

  స్థిరరాశి స్వయంచాలక &variations = పట్టిక.get_feature_variations ();
  కొరకు (స్థిరరాశి స్వయంచాలక &రికార్డు : variations.get_lookup_variation_రికార్డులు ())
  {
    గుర్తులేని feature_సూచీ = రికార్డు.get_feature_సూచీ ();
    ఒకవేళ (!feature_indices->has (feature_సూచీ)) కొనసాగు;

    hb::shared_ptr<hb_సమితి_t> lookups {hb_సమితి_create ()};
    రికార్డు.collect_lookups (&variations,
                           పట్టిక.get_feature_variation (feature_సూచీ, variations_సూచీ),
                           state.as_sorted_array (), lookups);

    ఒకవేళ (unlikely (lookups->లోపల_error () ||
		  !lookup_variations->సమితి (feature_సూచీ, lookups)))
      తిరిగి plan->check_success (అబద్ధం);
  }

  తిరిగి నిజం;
}

మూస <రకం_పేరు T>
స్థిర శూన్య _collect_layout_indices (hb_subset_plan_t     *plan,
                                     స్థిరరాశి T&              పట్టిక,
                                     hb_సమితి_t		  *lookup_indices, /*బహిర్గతం*/
                                     hb_సమితి_t		  *feature_indices, /*బహిర్గతం*/
                                     hb_hashmap_t<గుర్తులేని, hb::shared_ptr<hb_సమితి_t>> *feature_రికార్డు_cond_idx_map, /*బహిర్గతం*/
                                     hb_hashmap_t<గుర్తులేని, స్థిరరాశి OT::Feature*> *feature_substitutes_map, /*బహిర్గతం*/
                                     hb_hashmap_t<గుర్తులేని, hb::shared_ptr<hb_సమితి_t>> *lookup_variations, /*బహిర్గతం*/
                                     hb_సమితి_t& catch_all_రికార్డు_feature_idxes, /*బహిర్గతం*/
                                     hb_hashmap_t<గుర్తులేని, hb_pair_t<స్థిరరాశి శూన్య*, స్థిరరాశి శూన్య*>>& catch_all_రికార్డు_idx_feature_map /*బహిర్గతం*/)
{
  గుర్తులేని అంకె_features = పట్టిక.get_feature_లెక్క ();
  hb_సదిశ_t<hb_ట్యాగ్_t> features;
  ఒకవేళ (!plan->check_success (features.resize (అంకె_features))) తిరిగి;
  పట్టిక.get_feature_ట్యాగులు (0, &అంకె_features, features.arrayZ);
  తార్కిక retain_all_features = !filter_ట్యాగ్_list (&features, &plan->layout_features);

  గుర్తులేని అంకె_scripts = పట్టిక.get_script_లెక్క ();
  hb_సదిశ_t<hb_ట్యాగ్_t> scripts;
  ఒకవేళ (!plan->check_success (scripts.resize (అంకె_scripts))) తిరిగి;
  పట్టిక.get_script_ట్యాగులు (0, &అంకె_scripts, scripts.arrayZ);
  తార్కిక retain_all_scripts = !filter_ట్యాగ్_list (&scripts, &plan->layout_scripts);

  ఒకవేళ (!plan->check_success (!features.లోపల_error ()) || !features
      || !plan->check_success (!scripts.లోపల_error ()) || !scripts)
    తిరిగి;

  hb_ot_layout_collect_features (plan->ఆధారం,
                                 T::పట్టిక_ట్యాగ్,
                                 retain_all_scripts ? శూన్యసూచిక : scripts.arrayZ,
                                 శూన్యసూచిక,
                                 retain_all_features ? శూన్యసూచిక : features.arrayZ,
                                 feature_indices);

#నిర్వచితం_కాకపోతే HB_NO_VAR
  // collect feature substitutes with variations
  ఒకవేళ (!plan->user_axes_location.is_empty ())
  {
    hb_blob_ptr_t<OT::GDEF> gdef = plan->ఆధారం_పట్టిక<OT::GDEF> ();
    OT::అంశం_Var_Store_Instancer instancer (&gdef->get_var_store (), శూన్యసూచిక,
					 plan->normalized_coords.as_array ());
    hb_hashmap_t<hb::shared_ptr<hb_map_t>, గుర్తులేని> conditionset_map;
    OT::hb_collect_feature_substitutes_with_var_context_t c =
    {
      &plan->axes_old_సూచీ_ట్యాగ్_map,
      &plan->axes_location,
      feature_రికార్డు_cond_idx_map,
      feature_substitutes_map,
      catch_all_రికార్డు_feature_idxes,
      feature_indices,
      అబద్ధం,
      అబద్ధం,
      అబద్ధం,
      0,
      &conditionset_map,
      &instancer,
      plan->all_axes_pinned
    };
    పట్టిక.collect_feature_substitutes_with_variations (&c);
    gdef.destroy ();
  }
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_NO_VAR
  ఒకవేళ (plan->all_axes_pinned &&
      !_collect_instanced_lookup_variations (plan, పట్టిక, feature_indices, lookup_variations))
    తిరిగి;
#అయితే_ముగింపు

  కొరకు (గుర్తులేని feature_సూచీ : *feature_indices)
  {
    hb::shared_ptr<hb_సమితి_t> *lookups;
    ఒకవేళ (lookup_variations->has (feature_సూచీ, &lookups))
    {
      lookup_indices->union_ (**lookups);
      కొనసాగు;
    }

    స్థిరరాశి OT::Feature* f = &(పట్టిక.get_feature (feature_సూచీ));
    స్థిరరాశి OT::Feature **p = శూన్యసూచిక;
    ఒకవేళ (feature_substitutes_map->has (feature_సూచీ, &p))
      f = *p;

    f->కూడు_lookup_indexes_గమ్యం (lookup_indices);
  }

#నిర్వచితం_కాకపోతే HB_NO_VAR
  ఒకవేళ (catch_all_రికార్డు_feature_idxes)
  {
    కొరకు (గుర్తులేని feature_సూచీ : catch_all_రికార్డు_feature_idxes)
    {
      స్థిరరాశి OT::Feature& f = పట్టిక.get_feature (feature_సూచీ);
      f.కూడు_lookup_indexes_గమ్యం (lookup_indices);
      స్థిరరాశి శూన్య *ట్యాగ్_2 = reinterpret_cast<స్థిరరాశి శూన్య*> (&(పట్టిక.get_feature_list ().get_ట్యాగ్ (feature_సూచీ)));
      catch_all_రికార్డు_idx_feature_map.సమితి (feature_సూచీ, hb_pair (&f, ట్యాగ్_2));
    }
  }

  // If all axes are pinned then all feature variations will be dropped so there's no need
  // to collect lookups from them.
  ఒకవేళ (!plan->all_axes_pinned)
    పట్టిక.feature_variation_collect_lookups (feature_indices,
                                             plan->user_axes_location.is_empty () ? శూన్యసూచిక: feature_రికార్డు_cond_idx_map,
                                             lookup_indices);
#అయితే_ముగింపు
}


స్థిర తార్కిక
_features_have_equal_params (స్థిరరాశి OT::Feature &a HB_వాడని,
                            స్థిరరాశి OT::Feature &b HB_వాడని,
                            hb_ట్యాగ్_t ట్యాగ్_2 HB_వాడని)
{
#నిర్వచితం_కాకపోతే HB_NO_LAYOUT_FEATURE_PARAMS
  ఒకవేళ (తార్కిక (a.featureParams) != తార్కిక (b.featureParams)) తిరిగి అబద్ధం;
  ఒకవేళ (!a.featureParams) తిరిగి నిజం;
  స్థిరరాశి OT::FeatureParams &ap = a.get_feature_params ();
  స్థిరరాశి OT::FeatureParams &bp = b.get_feature_params ();
  ఒకవేళ (ట్యాగ్_2 == HB_ట్యాగ్ ('s','i','z','e'))
    తిరిగి !hb_memcmp (&ap.get_కొలత_params (ట్యాగ్_2), &bp.get_కొలత_params (ట్యాగ్_2),
                       OT::Feature_Params_కొలత::static_కొలత);
  ఒకవేళ ((ట్యాగ్_2 & 0xFFFF0000u) == HB_ట్యాగ్ ('s','s','\0','\0'))
    తిరిగి !hb_memcmp (&ap.get_stylistic_సమితి_params (ట్యాగ్_2), &bp.get_stylistic_సమితి_params (ట్యాగ్_2),
                       OT::Feature_Params_Stylistic_సమితి::static_కొలత);
  ఒకవేళ ((ట్యాగ్_2 & 0xFFFF0000u) == HB_ట్యాగ్ ('c','v','\0','\0'))
  {
    స్థిరరాశి స్వయంచాలక &ac = ap.get_character_variants_params (ట్యాగ్_2);
    స్థిరరాశి స్వయంచాలక &bc = bp.get_character_variants_params (ట్యాగ్_2);
    తిరిగి ac.get_కొలత () == bc.get_కొలత () &&
           !hb_memcmp (&ac, &bc, ac.get_కొలత ());
  }
#అయితే_ముగింపు
  తిరిగి నిజం;
}

స్థిర అంతఃవరుస శూన్య
GSUBGPOS_కనుగొను_duplicate_features (స్థిరరాశి OT::GSUBGPOS &g,
				   స్థిరరాశి hb_map_t *lookup_indices,
				   స్థిరరాశి hb_సమితి_t *feature_indices,
				   స్థిరరాశి hb_hashmap_t<గుర్తులేని, స్థిరరాశి OT::Feature*> *feature_substitutes_map,
                                   తార్కిక all_axes_pinned HB_వాడని,
				   hb_map_t *duplicate_feature_map /*బహిర్గతం*/)
{
  ఒకవేళ (feature_indices->is_empty ()) తిరిగి;
#నిర్వచితం_కాకపోతే HB_NO_VAR
  hb_సమితి_t variable_features;
  ఒకవేళ (!all_axes_pinned)
    g.get_feature_variations ().collect_feature_indices (&variable_features);
  ఒకవేళ (unlikely (variable_features.లోపల_error ())) తిరిగి;
#అయితే_ముగింపు
  hb_hashmap_t<hb_ట్యాగ్_t, hb::unique_ptr<hb_సమితి_t>> unique_features;
  //find out duplicate features after subset
  కొరకు (గుర్తులేని i : feature_indices->iter ())
  {
    hb_ట్యాగ్_t t = g.get_feature_ట్యాగ్ (i);
    ఒకవేళ (t == HB_MAP_విలువ_INVALID) కొనసాగు;
    ఒకవేళ (!unique_features.has (t))
    {
      ఒకవేళ (unlikely (!unique_features.సమితి (t, hb::unique_ptr<hb_సమితి_t> {hb_సమితి_create ()})))
	తిరిగి;
      ఒకవేళ (unique_features.has (t))
	unique_features.get (t)->కూడు (i);
      duplicate_feature_map->సమితి (i, i);
      కొనసాగు;
    }

    తార్కిక దొరికింది = అబద్ధం;

    hb_సమితి_t* same_ట్యాగ్_features = unique_features.get (t);
    కొరకు (గుర్తులేని other_f_సూచీ : same_ట్యాగ్_features->iter ())
    {
#నిర్వచితం_కాకపోతే HB_NO_VAR
      /* Matching base lookups do not imply matching alternate behavior. */
      ఒకవేళ (variable_features.has (i) || variable_features.has (other_f_సూచీ) ||
          g.get_feature_variations ().has_lookup_variations (i) ||
	  g.get_feature_variations ().has_lookup_variations (other_f_సూచీ))
	కొనసాగు;
#అయితే_ముగింపు

      స్థిరరాశి OT::Feature* f = &(g.get_feature (i));
      స్థిరరాశి OT::Feature **p = శూన్యసూచిక;
      ఒకవేళ (feature_substitutes_map->has (i, &p))
        f = *p;

      స్థిరరాశి OT::Feature* other_f = &(g.get_feature (other_f_సూచీ));
      ఒకవేళ (feature_substitutes_map->has (other_f_సూచీ, &p))
        other_f = *p;

      ఒకవేళ (!_features_have_equal_params (*f, *other_f, t)) కొనసాగు;

      స్వయంచాలక f_iter =
      + hb_iter (f->lookup_సూచీ_2)
      | hb_filter (lookup_indices)
      ;

      స్వయంచాలక other_f_iter =
      + hb_iter (other_f->lookup_సూచీ_2)
      | hb_filter (lookup_indices)
      ;

      తార్కిక is_equal = నిజం;
      కొరకు (; f_iter && other_f_iter; f_iter++, other_f_iter++)
      {
	గుర్తులేని a = *f_iter;
	గుర్తులేని b = *other_f_iter;
	ఒకవేళ (a != b) { is_equal = అబద్ధం; ఆపు; }
      }

      ఒకవేళ (is_equal == అబద్ధం || f_iter || other_f_iter) కొనసాగు;

      దొరికింది = నిజం;
      duplicate_feature_map->సమితి (i, other_f_సూచీ);
      ఆపు;
    }

    ఒకవేళ (దొరికింది == అబద్ధం)
    {
      same_ట్యాగ్_features->కూడు (i);
      duplicate_feature_map->సమితి (i, i);
    }
  }
}

స్థిర శూన్య
remap_feature_indices (స్థిరరాశి hb_సమితి_t &feature_indices,
                       స్థిరరాశి hb_map_t &duplicate_feature_map,
                       స్థిరరాశి hb_hashmap_t<గుర్తులేని, hb_pair_t<స్థిరరాశి శూన్య*, స్థిరరాశి శూన్య*>>& catch_all_రికార్డు_idx_feature_map,
                       hb_map_t       *mapping, /*బహిర్గతం*/
                       hb_map_t       *mapping_w_duplicates /*బహిర్గతం*/)
{
  గుర్తులేని i = 0;
  కొరకు (స్థిరరాశి స్వయంచాలక _ : feature_indices)
  {
    // retain those features in case we need to insert a catch-all record to reinstate the old features
    ఒకవేళ (catch_all_రికార్డు_idx_feature_map.has (_))
    {
      mapping->సమితి (_, i);
      mapping_w_duplicates->సమితి (_, i);
      i++;
    }
    లేకపోతే
    {
      uint32_t f_idx = duplicate_feature_map.get (_);
      uint32_t *new_idx;
      ఒకవేళ (mapping-> has (f_idx, &new_idx))
      {
        mapping_w_duplicates->సమితి (_, *new_idx);
      }
      లేకపోతే
      {
        mapping->సమితి (_, i);
        mapping_w_duplicates->సమితి (_, i);
        i++;
      }
    }
  }
}

మూస <రకం_పేరు T>
స్థిర శూన్య
సంవృతం_glyphs_lookups_features (hb_subset_plan_t   *plan,
				 hb_సమితి_t	     *gids_గమ్యం_retain,
				 hb_map_t	     *lookups,
				 hb_map_t	     *features,
				 hb_map_t	     *features_w_duplicates,
				 script_langsys_map *langsys_map,
				 hb_hashmap_t<గుర్తులేని, hb::shared_ptr<hb_సమితి_t>> *feature_రికార్డు_cond_idx_map,
				 hb_hashmap_t<గుర్తులేని, స్థిరరాశి OT::Feature*> *feature_substitutes_map,
                                 hb_hashmap_t<గుర్తులేని, hb::shared_ptr<hb_సమితి_t>> *lookup_variations,
                                 hb_సమితి_t &catch_all_రికార్డు_feature_idxes,
                                 hb_hashmap_t<గుర్తులేని, hb_pair_t<స్థిరరాశి శూన్య*, స్థిరరాశి శూన్య*>>& catch_all_రికార్డు_idx_feature_map)
{
  hb_blob_ptr_t<T> పట్టిక = plan->ఆధారం_పట్టిక<T> ();
  hb_ట్యాగ్_t పట్టిక_ట్యాగ్_2 = పట్టిక->పట్టిక_ట్యాగ్;
  hb_సమితి_t lookup_indices, feature_indices;
  _collect_layout_indices<T> (plan,
                              *పట్టిక,
                              &lookup_indices,
                              &feature_indices,
                              feature_రికార్డు_cond_idx_map,
                              feature_substitutes_map,
                              lookup_variations,
                              catch_all_రికార్డు_feature_idxes,
                              catch_all_రికార్డు_idx_feature_map);

  ఒకవేళ (పట్టిక_ట్యాగ్_2 == HB_OT_ట్యాగ్_GSUB && !(plan->flags & HB_SUBSET_FLAGS_NO_LAYOUT_సంవృతం))
    hb_ot_layout_lookups_substitute_సంవృతం (plan->ఆధారం,
                                             &lookup_indices,
					     gids_గమ్యం_retain);
  పట్టిక->సంవృతం_lookups (plan->ఆధారం,
			  gids_గమ్యం_retain,
                          &lookup_indices);
  remap_indexes (&lookup_indices, lookups);

  // prune features
  పట్టిక->prune_features (lookups,
                         plan->user_axes_location.is_empty () ? శూన్యసూచిక : feature_రికార్డు_cond_idx_map,
                         feature_substitutes_map,
                         &feature_indices);
  hb_map_t duplicate_feature_map;
  GSUBGPOS_కనుగొను_duplicate_features (*పట్టిక, lookups, &feature_indices, feature_substitutes_map,
                                    plan->all_axes_pinned, &duplicate_feature_map);

  feature_indices.clear ();
  పట్టిక->prune_langsys (&duplicate_feature_map, &plan->layout_scripts, langsys_map, &feature_indices);
  remap_feature_indices (feature_indices, duplicate_feature_map, catch_all_రికార్డు_idx_feature_map, features, features_w_duplicates);

#నిర్వచితం_కాకపోతే HB_NO_VAR
  // Populate alternate parameter tags after deduplication. Before this point
  // the map contains only catch-all features and also controls their retention.
  ఒకవేళ (పట్టిక->get_feature_variations ().రికార్డు_లెక్క ())
    కొరకు (స్వయంచాలక p : *features_w_duplicates)
    {
      స్థిరరాశి OT::Feature& f = పట్టిక->get_feature (p.మొదటి);
      స్థిరరాశి శూన్య *ట్యాగ్_2 = &(పట్టిక->get_feature_list ().get_ట్యాగ్ (p.మొదటి));
      catch_all_రికార్డు_idx_feature_map.సమితి (p.మొదటి, hb_pair (&f, ట్యాగ్_2));
    }
#అయితే_ముగింపు

  పట్టిక.destroy ();
}

శూన్య layout_nameid_సంవృతం (hb_subset_plan_t* plan,
                             hb_సమితి_t* drop_tables)
{
  ఒకవేళ (!drop_tables->has (HB_OT_ట్యాగ్_GPOS))
  {
    hb_blob_ptr_t<GPOS> gpos = plan->ఆధారం_పట్టిక<GPOS> ();
    gpos->collect_name_ids (&plan->gpos_features, &plan->name_ids,
                            &plan->gpos_feature_substitutes_map,
                            plan->user_axes_location.is_empty () ? శూన్యసూచిక : &plan->gpos_feature_రికార్డు_cond_idx_map,
                            &plan->gpos_old_features,
                            plan->all_axes_pinned);
    gpos.destroy ();
  }
  ఒకవేళ (!drop_tables->has (HB_OT_ట్యాగ్_GSUB))
  {
    hb_blob_ptr_t<GSUB> gsub = plan->ఆధారం_పట్టిక<GSUB> ();
    gsub->collect_name_ids (&plan->gsub_features, &plan->name_ids,
                            &plan->gsub_feature_substitutes_map,
                            plan->user_axes_location.is_empty () ? శూన్యసూచిక : &plan->gsub_feature_రికార్డు_cond_idx_map,
                            &plan->gsub_old_features,
                            plan->all_axes_pinned);
    gsub.destroy ();
  }
}

శూన్య
layout_populate_gids_గమ్యం_retain (hb_subset_plan_t* plan,
		                hb_సమితి_t* drop_tables) {
  ఒకవేళ (!drop_tables->has (HB_OT_ట్యాగ్_GSUB))
    // closure all glyphs/lookups/features needed for GSUB substitutions.
    సంవృతం_glyphs_lookups_features<GSUB> (
        plan,
        &plan->_glyphset_gsub,
        &plan->gsub_lookups,
        &plan->gsub_features,
        &plan->gsub_features_w_duplicates,
        &plan->gsub_langsys,
        &plan->gsub_feature_రికార్డు_cond_idx_map,
        &plan->gsub_feature_substitutes_map,
        &plan->gsub_lookup_variations,
        plan->gsub_old_features,
        plan->gsub_old_feature_idx_ట్యాగ్_map);

  ఒకవేళ (!drop_tables->has (HB_OT_ట్యాగ్_GPOS))
    సంవృతం_glyphs_lookups_features<GPOS> (
        plan,
        &plan->_glyphset_gsub,
        &plan->gpos_lookups,
        &plan->gpos_features,
        &plan->gpos_features_w_duplicates,
        &plan->gpos_langsys,
        &plan->gpos_feature_రికార్డు_cond_idx_map,
        &plan->gpos_feature_substitutes_map,
        &plan->gpos_lookup_variations,
        plan->gpos_old_features,
        plan->gpos_old_feature_idx_ట్యాగ్_map);
}

#నిర్వచితం_కాకపోతే HB_NO_VAR
శూన్య
collect_layout_variation_indices (hb_subset_plan_t* plan)
{
  hb_blob_ptr_t<OT::GDEF> gdef = plan->ఆధారం_పట్టిక<OT::GDEF> ();
  hb_blob_ptr_t<GSUB> gsub = plan->ఆధారం_పట్టిక<GSUB> ();
  hb_blob_ptr_t<GPOS> gpos = plan->ఆధారం_పట్టిక<GPOS> ();

  ఒకవేళ (!gdef->has_దత్తాంశం () || !gdef->has_var_store ())
  {
    gdef.destroy ();
    gsub.destroy ();
    gpos.destroy ();
    తిరిగి;
  }

  hb_సమితి_t varidx_సమితి;
  OT::hb_collect_variation_indices_context_t c (&varidx_సమితి,
                                                &plan->_glyphset_gsub,
                                                &plan->gpos_lookups);
  ఒకవేళ (plan->normalized_coords && !plan->all_axes_pinned)
    c.condition_విలువలు = &plan->layout_condition_విలువలు;
  gdef->collect_variation_indices (&c);

  ఒకవేళ (hb_ot_layout_has_substitution (plan->ఆధారం))
    gsub->collect_feature_variation_indices (&c);
  ఒకవేళ (hb_ot_layout_has_positioning (plan->ఆధారం))
  {
    gpos->collect_feature_variation_indices (&c);
    gpos->collect_variation_indices (&c);
  }

#నిర్వచితం_కాకపోతే HB_NO_MATH
  ఒకవేళ (!plan->drop_tables.has (HB_OT_ట్యాగ్_MATH))
  {
    hb_blob_ptr_t<OT::MATH> math = plan->ఆధారం_పట్టిక<OT::MATH> ();
    స్వయంచాలక *glyph_సమితి = c.glyph_సమితి;
    c.glyph_సమితి = &plan->_glyphset_mathed;
    math->collect_variation_indices (&c, &plan->_glyphset_cmaped);
    c.glyph_సమితి = glyph_సమితి;
    math.destroy ();
  }
#అయితే_ముగింపు

  remap_variation_indices (gdef->get_var_store (),
                           varidx_సమితి, plan->normalized_coords,
                           !plan->pinned_at_default,
                           plan->all_axes_pinned,
                           plan->layout_variation_idx_delta_map);

  గుర్తులేని subtable_లెక్క = gdef->get_var_store ().get_sub_పట్టిక_లెక్క ();
  generate_varstore_inner_maps (varidx_సమితి, subtable_లెక్క, plan->gdef_varstore_inner_maps);
  plan->check_success (!plan->layout_condition_విలువలు.లోపల_error ());

  gdef.destroy ();
  gsub.destroy ();
  gpos.destroy ();
}
#అయితే_ముగింపు

#అయితే_ముగింపు

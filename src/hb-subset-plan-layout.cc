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

#ಅನ್ತರ್ಭಾವ "hb-subset-plan.hh"

#ಅನ್ತರ್ಭಾವ "hb-ot-layout-gdef-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-layout-gpos-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-layout-gsub-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-math-table.hh"

using OT::Layout::GSUB;
using OT::Layout::GPOS;

#ifndef HB_NO_SUBSET_LAYOUT

ದ್ವಿಗುಣ
hb_subset_plan_t::layout_variation_delta (unsigned var_idx HB_UNUSED)
{
#ifndef HB_NO_VAR
  hb_blob_ptr_t<OT::GDEF> gdef = source_table<OT::GDEF> ();
  ದ್ವಿಗುಣ delta = gdef->get_var_store ().get_delta<ದ್ವಿಗುಣ> (var_idx,
                                                normalized_coords.as_array ());
  gdef.destroy ();
  ಪ್ರತ್ಯಾಗಮ delta;
#ಅನ್ಯಥಾ
  ಪ್ರತ್ಯಾಗಮ 0.f;
#endif
}

ಶೂನ್ಯ
remap_used_mark_sets (hb_subset_plan_t *plan,
                      hb_map_t& used_mark_sets_map)
{
  hb_blob_ptr_t<OT::GDEF> gdef = plan->source_table<OT::GDEF> ();

  ಯದಿ (!gdef->has_data () || !gdef->has_mark_glyph_sets ())
  {
    gdef.destroy ();
    ಪ್ರತ್ಯಾಗಮ;
  }

  hb_set_t used_mark_sets;
  gdef->get_mark_glyph_sets ().collect_used_mark_sets (plan->_glyphset_gsub, used_mark_sets);
  gdef.destroy ();

  remap_indexes (&used_mark_sets, &used_mark_sets_map);
}

/*
 * Removes all tags from 'tags' that are not in filter. Additionally eliminates any duplicates.
 * Returns true if anything was removed (not including duplicates).
 */
ಸ್ಥಿರ ತಾರ್ಕಿಕ _filter_tag_list(hb_vector_t<hb_tag_t>* tags, /* IN/OUT */
                             ಧ್ರುವ hb_set_t* filter)
{
  hb_vector_t<hb_tag_t> out;
  out.alloc (tags->get_size() + 1); // +1 is to allocate room for the null terminator.

  ತಾರ್ಕಿಕ removed = ಅಸತ್ಯ;
  hb_set_t visited;

  ಪ್ರತಿ (hb_tag_t tag : *tags)
  {
    ಯದಿ (!tag) ಅನುವರ್ತ;
    ಯದಿ (visited.has (tag)) ಅನುವರ್ತ;

    ಯದಿ (!filter->has (tag))
    {
      removed = ಸತ್ಯ;
      ಅನುವರ್ತ;
    }

    visited.add (tag);
    out.push (tag);
  }

  // The collect function needs a null element to signal end of the array.
  out.push (HB_TAG_NONE);

  hb_swap (out, *tags);
  ಪ್ರತ್ಯಾಗಮ removed;
}

template <typename T>
ಸ್ಥಿರ ತಾರ್ಕಿಕ
_collect_instanced_lookup_variations (
    hb_subset_plan_t *plan,
    ಧ್ರುವ T& table,
    ಧ್ರುವ hb_set_t *feature_indices,
    hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *lookup_variations)
{
  hb_blob_ptr_t<OT::GDEF> gdef = plan->source_table<OT::GDEF> ();
  OT::ItemVarStoreInstancer instancer (&gdef->get_var_store (), nullptr,
				       plan->normalized_coords.as_array ());
  hb_vector_t<unsigned> state;
  unsigned variations_index;
  ತಾರ್ಕಿಕ success = table.get_feature_variations_state (
	plan->normalized_coords.arrayZ,
	plan->normalized_coords.length,
	&variations_index,
	&instancer,
	&state);
  gdef.destroy ();

  ಯದಿ (unlikely (!success || state.in_error ()))
    ಪ್ರತ್ಯಾಗಮ plan->check_success (ಅಸತ್ಯ);

  ಧ್ರುವ auto &variations = table.get_feature_variations ();
  ಪ್ರತಿ (ಧ್ರುವ auto &record : variations.get_lookup_variation_records ())
  {
    unsigned feature_index = record.get_feature_index ();
    ಯದಿ (!feature_indices->has (feature_index)) ಅನುವರ್ತ;

    hb::shared_ptr<hb_set_t> lookups {hb_set_create ()};
    record.collect_lookups (&variations,
                           table.get_feature_variation (feature_index, variations_index),
                           state.as_sorted_array (), lookups);

    ಯದಿ (unlikely (lookups->in_error () ||
		  !lookup_variations->set (feature_index, lookups)))
      ಪ್ರತ್ಯಾಗಮ plan->check_success (ಅಸತ್ಯ);
  }

  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

template <typename T>
ಸ್ಥಿರ ಶೂನ್ಯ _collect_layout_indices (hb_subset_plan_t     *plan,
                                     ಧ್ರುವ T&              table,
                                     hb_set_t		  *lookup_indices, /* OUT */
                                     hb_set_t		  *feature_indices, /* OUT */
                                     hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map, /* OUT */
                                     hb_hashmap_t<unsigned, ಧ್ರುವ OT::Feature*> *feature_substitutes_map, /* OUT */
                                     hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *lookup_variations, /* OUT */
                                     hb_set_t& catch_all_record_feature_idxes, /* OUT */
                                     hb_hashmap_t<unsigned, hb_pair_t<ಧ್ರುವ ಶೂನ್ಯ*, ಧ್ರುವ ಶೂನ್ಯ*>>& catch_all_record_idx_feature_map /* OUT */)
{
  unsigned ಅಂಕೆ_features = table.get_feature_ಎಣಿಕೆ ();
  hb_vector_t<hb_tag_t> features;
  ಯದಿ (!plan->check_success (features.resize (ಅಂಕೆ_features))) ಪ್ರತ್ಯಾಗಮ;
  table.get_feature_tags (0, &ಅಂಕೆ_features, features.arrayZ);
  ತಾರ್ಕಿಕ retain_all_features = !_filter_tag_list (&features, &plan->layout_features);

  unsigned ಅಂಕೆ_scripts = table.get_script_ಎಣಿಕೆ ();
  hb_vector_t<hb_tag_t> scripts;
  ಯದಿ (!plan->check_success (scripts.resize (ಅಂಕೆ_scripts))) ಪ್ರತ್ಯಾಗಮ;
  table.get_script_tags (0, &ಅಂಕೆ_scripts, scripts.arrayZ);
  ತಾರ್ಕಿಕ retain_all_scripts = !_filter_tag_list (&scripts, &plan->layout_scripts);

  ಯದಿ (!plan->check_success (!features.in_error ()) || !features
      || !plan->check_success (!scripts.in_error ()) || !scripts)
    ಪ್ರತ್ಯಾಗಮ;

  hb_ot_layout_collect_features (plan->source,
                                 T::tableTag,
                                 retain_all_scripts ? nullptr : scripts.arrayZ,
                                 nullptr,
                                 retain_all_features ? nullptr : features.arrayZ,
                                 feature_indices);

#ifndef HB_NO_VAR
  // collect feature substitutes with variations
  ಯದಿ (!plan->user_axes_location.is_empty ())
  {
    hb_blob_ptr_t<OT::GDEF> gdef = plan->source_table<OT::GDEF> ();
    OT::ItemVarStoreInstancer instancer (&gdef->get_var_store (), nullptr,
					 plan->normalized_coords.as_array ());
    hb_hashmap_t<hb::shared_ptr<hb_map_t>, unsigned> conditionset_map;
    OT::hb_collect_feature_substitutes_with_var_context_t c =
    {
      &plan->axes_old_index_tag_map,
      &plan->axes_location,
      feature_record_cond_idx_map,
      feature_substitutes_map,
      catch_all_record_feature_idxes,
      feature_indices,
      ಅಸತ್ಯ,
      ಅಸತ್ಯ,
      ಅಸತ್ಯ,
      0,
      &conditionset_map,
      &instancer,
      plan->all_axes_pinned
    };
    table.collect_feature_substitutes_with_variations (&c);
    gdef.destroy ();
  }
#endif

#ifndef HB_NO_VAR
  ಯದಿ (plan->all_axes_pinned &&
      !_collect_instanced_lookup_variations (plan, table, feature_indices, lookup_variations))
    ಪ್ರತ್ಯಾಗಮ;
#endif

  ಪ್ರತಿ (unsigned feature_index : *feature_indices)
  {
    hb::shared_ptr<hb_set_t> *lookups;
    ಯದಿ (lookup_variations->has (feature_index, &lookups))
    {
      lookup_indices->union_ (**lookups);
      ಅನುವರ್ತ;
    }

    ಧ್ರುವ OT::Feature* f = &(table.get_feature (feature_index));
    ಧ್ರುವ OT::Feature **p = nullptr;
    ಯದಿ (feature_substitutes_map->has (feature_index, &p))
      f = *p;

    f->add_lookup_indexes_to (lookup_indices);
  }

#ifndef HB_NO_VAR
  ಯದಿ (catch_all_record_feature_idxes)
  {
    ಪ್ರತಿ (unsigned feature_index : catch_all_record_feature_idxes)
    {
      ಧ್ರುವ OT::Feature& f = table.get_feature (feature_index);
      f.add_lookup_indexes_to (lookup_indices);
      ಧ್ರುವ ಶೂನ್ಯ *tag = reinterpret_cast<ಧ್ರುವ ಶೂನ್ಯ*> (&(table.get_feature_list ().get_tag (feature_index)));
      catch_all_record_idx_feature_map.set (feature_index, hb_pair (&f, tag));
    }
  }

  // If all axes are pinned then all feature variations will be dropped so there's no need
  // to collect lookups from them.
  ಯದಿ (!plan->all_axes_pinned)
    table.feature_variation_collect_lookups (feature_indices,
                                             plan->user_axes_location.is_empty () ? nullptr: feature_record_cond_idx_map,
                                             lookup_indices);
#endif
}


ಸ್ಥಿರ ತಾರ್ಕಿಕ
_features_have_equal_params (ಧ್ರುವ OT::Feature &a HB_UNUSED,
                            ಧ್ರುವ OT::Feature &b HB_UNUSED,
                            hb_tag_t tag HB_UNUSED)
{
#ifndef HB_NO_LAYOUT_FEATURE_PARAMS
  ಯದಿ (ತಾರ್ಕಿಕ (a.featureParams) != ತಾರ್ಕಿಕ (b.featureParams)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  ಯದಿ (!a.featureParams) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  ಧ್ರುವ OT::FeatureParams &ap = a.get_feature_params ();
  ಧ್ರುವ OT::FeatureParams &bp = b.get_feature_params ();
  ಯದಿ (tag == HB_TAG ('s','i','z','e'))
    ಪ್ರತ್ಯಾಗಮ !hb_memcmp (&ap.get_size_params (tag), &bp.get_size_params (tag),
                       OT::FeatureParamsSize::static_size);
  ಯದಿ ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0'))
    ಪ್ರತ್ಯಾಗಮ !hb_memcmp (&ap.get_stylistic_set_params (tag), &bp.get_stylistic_set_params (tag),
                       OT::FeatureParamsStylisticSet::static_size);
  ಯದಿ ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0'))
  {
    ಧ್ರುವ auto &ac = ap.get_character_variants_params (tag);
    ಧ್ರುವ auto &bc = bp.get_character_variants_params (tag);
    ಪ್ರತ್ಯಾಗಮ ac.get_size () == bc.get_size () &&
           !hb_memcmp (&ac, &bc, ac.get_size ());
  }
#endif
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
_GSUBGPOS_find_duplicate_features (ಧ್ರುವ OT::GSUBGPOS &g,
				   ಧ್ರುವ hb_map_t *lookup_indices,
				   ಧ್ರುವ hb_set_t *feature_indices,
				   ಧ್ರುವ hb_hashmap_t<unsigned, ಧ್ರುವ OT::Feature*> *feature_substitutes_map,
                                   ತಾರ್ಕಿಕ all_axes_pinned HB_UNUSED,
				   hb_map_t *duplicate_feature_map /* OUT */)
{
  ಯದಿ (feature_indices->is_empty ()) ಪ್ರತ್ಯಾಗಮ;
#ifndef HB_NO_VAR
  hb_set_t variable_features;
  ಯದಿ (!all_axes_pinned)
    g.get_feature_variations ().collect_feature_indices (&variable_features);
  ಯದಿ (unlikely (variable_features.in_error ())) ಪ್ರತ್ಯಾಗಮ;
#endif
  hb_hashmap_t<hb_tag_t, hb::unique_ptr<hb_set_t>> unique_features;
  //find out duplicate features after subset
  ಪ್ರತಿ (unsigned i : feature_indices->iter ())
  {
    hb_tag_t t = g.get_feature_tag (i);
    ಯದಿ (t == HB_MAP_ಮೌಲ್ಯ_INVALID) ಅನುವರ್ತ;
    ಯದಿ (!unique_features.has (t))
    {
      ಯದಿ (unlikely (!unique_features.set (t, hb::unique_ptr<hb_set_t> {hb_set_create ()})))
	ಪ್ರತ್ಯಾಗಮ;
      ಯದಿ (unique_features.has (t))
	unique_features.get (t)->add (i);
      duplicate_feature_map->set (i, i);
      ಅನುವರ್ತ;
    }

    ತಾರ್ಕಿಕ found = ಅಸತ್ಯ;

    hb_set_t* same_tag_features = unique_features.get (t);
    ಪ್ರತಿ (unsigned other_f_index : same_tag_features->iter ())
    {
#ifndef HB_NO_VAR
      /* Matching base lookups do not imply matching alternate behavior. */
      ಯದಿ (variable_features.has (i) || variable_features.has (other_f_index) ||
          g.get_feature_variations ().has_lookup_variations (i) ||
	  g.get_feature_variations ().has_lookup_variations (other_f_index))
	ಅನುವರ್ತ;
#endif

      ಧ್ರುವ OT::Feature* f = &(g.get_feature (i));
      ಧ್ರುವ OT::Feature **p = nullptr;
      ಯದಿ (feature_substitutes_map->has (i, &p))
        f = *p;

      ಧ್ರುವ OT::Feature* other_f = &(g.get_feature (other_f_index));
      ಯದಿ (feature_substitutes_map->has (other_f_index, &p))
        other_f = *p;

      ಯದಿ (!_features_have_equal_params (*f, *other_f, t)) ಅನುವರ್ತ;

      auto f_iter =
      + hb_iter (f->lookupIndex)
      | hb_filter (lookup_indices)
      ;

      auto other_f_iter =
      + hb_iter (other_f->lookupIndex)
      | hb_filter (lookup_indices)
      ;

      ತಾರ್ಕಿಕ is_equal = ಸತ್ಯ;
      ಪ್ರತಿ (; f_iter && other_f_iter; f_iter++, other_f_iter++)
      {
	unsigned a = *f_iter;
	unsigned b = *other_f_iter;
	ಯದಿ (a != b) { is_equal = ಅಸತ್ಯ; ಭಙ್ಗ; }
      }

      ಯದಿ (is_equal == ಅಸತ್ಯ || f_iter || other_f_iter) ಅನುವರ್ತ;

      found = ಸತ್ಯ;
      duplicate_feature_map->set (i, other_f_index);
      ಭಙ್ಗ;
    }

    ಯದಿ (found == ಅಸತ್ಯ)
    {
      same_tag_features->add (i);
      duplicate_feature_map->set (i, i);
    }
  }
}

ಸ್ಥಿರ ಶೂನ್ಯ
remap_feature_indices (ಧ್ರುವ hb_set_t &feature_indices,
                       ಧ್ರುವ hb_map_t &duplicate_feature_map,
                       ಧ್ರುವ hb_hashmap_t<unsigned, hb_pair_t<ಧ್ರುವ ಶೂನ್ಯ*, ಧ್ರುವ ಶೂನ್ಯ*>>& catch_all_record_idx_feature_map,
                       hb_map_t       *mapping, /* OUT */
                       hb_map_t       *mapping_w_duplicates /* OUT */)
{
  unsigned i = 0;
  ಪ್ರತಿ (ಧ್ರುವ auto _ : feature_indices)
  {
    // retain those features in case we need to insert a catch-all record to reinstate the old features
    ಯದಿ (catch_all_record_idx_feature_map.has (_))
    {
      mapping->set (_, i);
      mapping_w_duplicates->set (_, i);
      i++;
    }
    ಅನ್ಯಥಾ
    {
      uint32_t f_idx = duplicate_feature_map.get (_);
      uint32_t *new_idx;
      ಯದಿ (mapping-> has (f_idx, &new_idx))
      {
        mapping_w_duplicates->set (_, *new_idx);
      }
      ಅನ್ಯಥಾ
      {
        mapping->set (_, i);
        mapping_w_duplicates->set (_, i);
        i++;
      }
    }
  }
}

template <typename T>
ಸ್ಥಿರ ಶೂನ್ಯ
_closure_glyphs_lookups_features (hb_subset_plan_t   *plan,
				 hb_set_t	     *gids_to_retain,
				 hb_map_t	     *lookups,
				 hb_map_t	     *features,
				 hb_map_t	     *features_w_duplicates,
				 script_langsys_map *langsys_map,
				 hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
				 hb_hashmap_t<unsigned, ಧ್ರುವ OT::Feature*> *feature_substitutes_map,
                                 hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *lookup_variations,
                                 hb_set_t &catch_all_record_feature_idxes,
                                 hb_hashmap_t<unsigned, hb_pair_t<ಧ್ರುವ ಶೂನ್ಯ*, ಧ್ರುವ ಶೂನ್ಯ*>>& catch_all_record_idx_feature_map)
{
  hb_blob_ptr_t<T> table = plan->source_table<T> ();
  hb_tag_t table_tag = table->tableTag;
  hb_set_t lookup_indices, feature_indices;
  _collect_layout_indices<T> (plan,
                              *table,
                              &lookup_indices,
                              &feature_indices,
                              feature_record_cond_idx_map,
                              feature_substitutes_map,
                              lookup_variations,
                              catch_all_record_feature_idxes,
                              catch_all_record_idx_feature_map);

  ಯದಿ (table_tag == HB_OT_TAG_GSUB && !(plan->flags & HB_SUBSET_FLAGS_NO_LAYOUT_CLOSURE))
    hb_ot_layout_lookups_substitute_closure (plan->source,
                                             &lookup_indices,
					     gids_to_retain);
  table->closure_lookups (plan->source,
			  gids_to_retain,
                          &lookup_indices);
  remap_indexes (&lookup_indices, lookups);

  // prune features
  table->prune_features (lookups,
                         plan->user_axes_location.is_empty () ? nullptr : feature_record_cond_idx_map,
                         feature_substitutes_map,
                         &feature_indices);
  hb_map_t duplicate_feature_map;
  _GSUBGPOS_find_duplicate_features (*table, lookups, &feature_indices, feature_substitutes_map,
                                    plan->all_axes_pinned, &duplicate_feature_map);

  feature_indices.clear ();
  table->prune_langsys (&duplicate_feature_map, &plan->layout_scripts, langsys_map, &feature_indices);
  remap_feature_indices (feature_indices, duplicate_feature_map, catch_all_record_idx_feature_map, features, features_w_duplicates);

#ifndef HB_NO_VAR
  // Populate alternate parameter tags after deduplication. Before this point
  // the map contains only catch-all features and also controls their retention.
  ಯದಿ (table->get_feature_variations ().record_ಎಣಿಕೆ ())
    ಪ್ರತಿ (auto p : *features_w_duplicates)
    {
      ಧ್ರುವ OT::Feature& f = table->get_feature (p.first);
      ಧ್ರುವ ಶೂನ್ಯ *tag = &(table->get_feature_list ().get_tag (p.first));
      catch_all_record_idx_feature_map.set (p.first, hb_pair (&f, tag));
    }
#endif

  table.destroy ();
}

ಶೂನ್ಯ layout_nameid_closure (hb_subset_plan_t* plan,
                             hb_set_t* drop_tables)
{
  ಯದಿ (!drop_tables->has (HB_OT_TAG_GPOS))
  {
    hb_blob_ptr_t<GPOS> gpos = plan->source_table<GPOS> ();
    gpos->collect_name_ids (&plan->gpos_features, &plan->name_ids,
                            &plan->gpos_feature_substitutes_map,
                            plan->user_axes_location.is_empty () ? nullptr : &plan->gpos_feature_record_cond_idx_map,
                            &plan->gpos_old_features,
                            plan->all_axes_pinned);
    gpos.destroy ();
  }
  ಯದಿ (!drop_tables->has (HB_OT_TAG_GSUB))
  {
    hb_blob_ptr_t<GSUB> gsub = plan->source_table<GSUB> ();
    gsub->collect_name_ids (&plan->gsub_features, &plan->name_ids,
                            &plan->gsub_feature_substitutes_map,
                            plan->user_axes_location.is_empty () ? nullptr : &plan->gsub_feature_record_cond_idx_map,
                            &plan->gsub_old_features,
                            plan->all_axes_pinned);
    gsub.destroy ();
  }
}

ಶೂನ್ಯ
layout_populate_gids_to_retain (hb_subset_plan_t* plan,
		                hb_set_t* drop_tables) {
  ಯದಿ (!drop_tables->has (HB_OT_TAG_GSUB))
    // closure all glyphs/lookups/features needed for GSUB substitutions.
    _closure_glyphs_lookups_features<GSUB> (
        plan,
        &plan->_glyphset_gsub,
        &plan->gsub_lookups,
        &plan->gsub_features,
        &plan->gsub_features_w_duplicates,
        &plan->gsub_langsys,
        &plan->gsub_feature_record_cond_idx_map,
        &plan->gsub_feature_substitutes_map,
        &plan->gsub_lookup_variations,
        plan->gsub_old_features,
        plan->gsub_old_feature_idx_tag_map);

  ಯದಿ (!drop_tables->has (HB_OT_TAG_GPOS))
    _closure_glyphs_lookups_features<GPOS> (
        plan,
        &plan->_glyphset_gsub,
        &plan->gpos_lookups,
        &plan->gpos_features,
        &plan->gpos_features_w_duplicates,
        &plan->gpos_langsys,
        &plan->gpos_feature_record_cond_idx_map,
        &plan->gpos_feature_substitutes_map,
        &plan->gpos_lookup_variations,
        plan->gpos_old_features,
        plan->gpos_old_feature_idx_tag_map);
}

#ifndef HB_NO_VAR
ಶೂನ್ಯ
collect_layout_variation_indices (hb_subset_plan_t* plan)
{
  hb_blob_ptr_t<OT::GDEF> gdef = plan->source_table<OT::GDEF> ();
  hb_blob_ptr_t<GSUB> gsub = plan->source_table<GSUB> ();
  hb_blob_ptr_t<GPOS> gpos = plan->source_table<GPOS> ();

  ಯದಿ (!gdef->has_data () || !gdef->has_var_store ())
  {
    gdef.destroy ();
    gsub.destroy ();
    gpos.destroy ();
    ಪ್ರತ್ಯಾಗಮ;
  }

  hb_set_t varidx_set;
  OT::hb_collect_variation_indices_context_t c (&varidx_set,
                                                &plan->_glyphset_gsub,
                                                &plan->gpos_lookups);
  ಯದಿ (plan->normalized_coords && !plan->all_axes_pinned)
    c.condition_ಮೌಲ್ಯಗಳು = &plan->layout_condition_ಮೌಲ್ಯಗಳು;
  gdef->collect_variation_indices (&c);

  ಯದಿ (hb_ot_layout_has_substitution (plan->source))
    gsub->collect_feature_variation_indices (&c);
  ಯದಿ (hb_ot_layout_has_positioning (plan->source))
  {
    gpos->collect_feature_variation_indices (&c);
    gpos->collect_variation_indices (&c);
  }

#ifndef HB_NO_MATH
  ಯದಿ (!plan->drop_tables.has (HB_OT_TAG_MATH))
  {
    hb_blob_ptr_t<OT::MATH> math = plan->source_table<OT::MATH> ();
    auto *glyph_set = c.glyph_set;
    c.glyph_set = &plan->_glyphset_mathed;
    math->collect_variation_indices (&c, &plan->_glyphset_cmaped);
    c.glyph_set = glyph_set;
    math.destroy ();
  }
#endif

  remap_variation_indices (gdef->get_var_store (),
                           varidx_set, plan->normalized_coords,
                           !plan->pinned_at_default,
                           plan->all_axes_pinned,
                           plan->layout_variation_idx_delta_map);

  unsigned subtable_ಎಣಿಕೆ = gdef->get_var_store ().get_sub_table_ಎಣಿಕೆ ();
  generate_varstore_inner_maps (varidx_set, subtable_ಎಣಿಕೆ, plan->gdef_varstore_inner_maps);
  plan->check_success (!plan->layout_condition_ಮೌಲ್ಯಗಳು.in_error ());

  gdef.destroy ();
  gsub.destroy ();
  gpos.destroy ();
}
#endif

#endif

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

#समावेश "hb-subset-plan.hh"

#समावेश "hb-ot-layout-gdef-table.hh"
#समावेश "hb-ot-layout-gpos-table.hh"
#समावेश "hb-ot-layout-gsub-table.hh"
#समावेश "hb-ot-math-table.hh"

using OT::Layout::GSUB;
using OT::Layout::GPOS;

#अगर_अघोषित HB_NO_SUBSET_LAYOUT

द्विगुन
hb_subset_plan_t::layout_variation_delta (अचिन्हित var_idx HB_UNUSED)
{
#अगर_अघोषित HB_NO_VAR
  hb_blob_ptr_t<OT::GDEF> gdef = source_table<OT::GDEF> ();
  द्विगुन delta = gdef->get_var_store ().get_delta<द्विगुन> (var_idx,
                                                normalized_coords.as_array ());
  gdef.destroy ();
  वापस delta;
#अन्यथा
  वापस 0.f;
#पूर्ण_अगर
}

व्योम
remap_used_mark_sets (hb_subset_plan_t *plan,
                      hb_map_t& used_mark_sets_map)
{
  hb_blob_ptr_t<OT::GDEF> gdef = plan->source_table<OT::GDEF> ();

  अगर (!gdef->has_तथ्य () || !gdef->has_mark_glyph_sets ())
  {
    gdef.destroy ();
    वापस;
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
अटल bool _filter_tag_list(hb_vector_t<hb_tag_t>* tags, /* IN/OUT */
                             स्थिर hb_set_t* filter)
{
  hb_vector_t<hb_tag_t> out;
  out.alloc (tags->get_आकार() + 1); // +1 is to allocate room for the null terminator.

  bool removed = false;
  hb_set_t visited;

  क्रम (hb_tag_t tag : *tags)
  {
    अगर (!tag) जारी;
    अगर (visited.has (tag)) जारी;

    अगर (!filter->has (tag))
    {
      removed = true;
      जारी;
    }

    visited.add (tag);
    out.push (tag);
  }

  // The collect function needs a null element to signal end of the array.
  out.push (HB_TAG_NONE);

  hb_swap (out, *tags);
  वापस removed;
}

ढाँचा <typename T>
अटल bool
_collect_instanced_lookup_variations (
    hb_subset_plan_t *plan,
    स्थिर T& table,
    स्थिर hb_set_t *feature_indices,
    hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *lookup_variations)
{
  hb_blob_ptr_t<OT::GDEF> gdef = plan->source_table<OT::GDEF> ();
  OT::ItemVarStoreInstancer instancer (&gdef->get_var_store (), nullptr,
				       plan->normalized_coords.as_array ());
  hb_vector_t<अचिन्हित> state;
  अचिन्हित variations_सूचकांक;
  bool success = table.get_feature_variations_state (
	plan->normalized_coords.arrayZ,
	plan->normalized_coords.लंबाई,
	&variations_सूचकांक,
	&instancer,
	&state);
  gdef.destroy ();

  अगर (unlikely (!success || state.in_error ()))
    वापस plan->check_success (false);

  स्थिर स्वतः &variations = table.get_feature_variations ();
  क्रम (स्थिर स्वतः &record : variations.get_lookup_variation_records ())
  {
    अचिन्हित feature_सूचकांक = record.get_feature_सूचकांक ();
    अगर (!feature_indices->has (feature_सूचकांक)) जारी;

    hb::shared_ptr<hb_set_t> lookups {hb_set_create ()};
    record.collect_lookups (&variations,
                           table.get_feature_variation (feature_सूचकांक, variations_सूचकांक),
                           state.as_sorted_array (), lookups);

    अगर (unlikely (lookups->in_error () ||
		  !lookup_variations->set (feature_सूचकांक, lookups)))
      वापस plan->check_success (false);
  }

  वापस true;
}

ढाँचा <typename T>
अटल व्योम _collect_layout_indices (hb_subset_plan_t     *plan,
                                     स्थिर T&              table,
                                     hb_set_t		  *lookup_indices, /* OUT */
                                     hb_set_t		  *feature_indices, /* OUT */
                                     hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map, /* OUT */
                                     hb_hashmap_t<अचिन्हित, स्थिर OT::Feature*> *feature_substitutes_map, /* OUT */
                                     hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *lookup_variations, /* OUT */
                                     hb_set_t& catch_all_record_feature_idxes, /* OUT */
                                     hb_hashmap_t<अचिन्हित, hb_pair_t<स्थिर व्योम*, स्थिर व्योम*>>& catch_all_record_idx_feature_map /* OUT */)
{
  अचिन्हित अंक_features = table.get_feature_गिनती ();
  hb_vector_t<hb_tag_t> features;
  अगर (!plan->check_success (features.resize (अंक_features))) वापस;
  table.get_feature_tags (0, &अंक_features, features.arrayZ);
  bool retain_all_features = !_filter_tag_list (&features, &plan->layout_features);

  अचिन्हित अंक_scripts = table.get_script_गिनती ();
  hb_vector_t<hb_tag_t> scripts;
  अगर (!plan->check_success (scripts.resize (अंक_scripts))) वापस;
  table.get_script_tags (0, &अंक_scripts, scripts.arrayZ);
  bool retain_all_scripts = !_filter_tag_list (&scripts, &plan->layout_scripts);

  अगर (!plan->check_success (!features.in_error ()) || !features
      || !plan->check_success (!scripts.in_error ()) || !scripts)
    वापस;

  hb_ot_layout_collect_features (plan->source,
                                 T::tableTag,
                                 retain_all_scripts ? nullptr : scripts.arrayZ,
                                 nullptr,
                                 retain_all_features ? nullptr : features.arrayZ,
                                 feature_indices);

#अगर_अघोषित HB_NO_VAR
  // collect feature substitutes with variations
  अगर (!plan->user_axes_location.is_empty ())
  {
    hb_blob_ptr_t<OT::GDEF> gdef = plan->source_table<OT::GDEF> ();
    OT::ItemVarStoreInstancer instancer (&gdef->get_var_store (), nullptr,
					 plan->normalized_coords.as_array ());
    hb_hashmap_t<hb::shared_ptr<hb_map_t>, अचिन्हित> conditionset_map;
    OT::hb_collect_feature_substitutes_with_var_context_t c =
    {
      &plan->axes_old_सूचकांक_tag_map,
      &plan->axes_location,
      feature_record_cond_idx_map,
      feature_substitutes_map,
      catch_all_record_feature_idxes,
      feature_indices,
      false,
      false,
      false,
      0,
      &conditionset_map,
      &instancer,
      plan->all_axes_pinned
    };
    table.collect_feature_substitutes_with_variations (&c);
    gdef.destroy ();
  }
#पूर्ण_अगर

#अगर_अघोषित HB_NO_VAR
  अगर (plan->all_axes_pinned &&
      !_collect_instanced_lookup_variations (plan, table, feature_indices, lookup_variations))
    वापस;
#पूर्ण_अगर

  क्रम (अचिन्हित feature_सूचकांक : *feature_indices)
  {
    hb::shared_ptr<hb_set_t> *lookups;
    अगर (lookup_variations->has (feature_सूचकांक, &lookups))
    {
      lookup_indices->union_ (**lookups);
      जारी;
    }

    स्थिर OT::Feature* f = &(table.get_feature (feature_सूचकांक));
    स्थिर OT::Feature **p = nullptr;
    अगर (feature_substitutes_map->has (feature_सूचकांक, &p))
      f = *p;

    f->add_lookup_indexes_to (lookup_indices);
  }

#अगर_अघोषित HB_NO_VAR
  अगर (catch_all_record_feature_idxes)
  {
    क्रम (अचिन्हित feature_सूचकांक : catch_all_record_feature_idxes)
    {
      स्थिर OT::Feature& f = table.get_feature (feature_सूचकांक);
      f.add_lookup_indexes_to (lookup_indices);
      स्थिर व्योम *tag = reinterpret_cast<स्थिर व्योम*> (&(table.get_feature_list ().get_tag (feature_सूचकांक)));
      catch_all_record_idx_feature_map.set (feature_सूचकांक, hb_pair (&f, tag));
    }
  }

  // If all axes are pinned then all feature variations will be dropped so there's no need
  // to collect lookups from them.
  अगर (!plan->all_axes_pinned)
    table.feature_variation_collect_lookups (feature_indices,
                                             plan->user_axes_location.is_empty () ? nullptr: feature_record_cond_idx_map,
                                             lookup_indices);
#पूर्ण_अगर
}


अटल bool
_features_have_equal_params (स्थिर OT::Feature &a HB_UNUSED,
                            स्थिर OT::Feature &b HB_UNUSED,
                            hb_tag_t tag HB_UNUSED)
{
#अगर_अघोषित HB_NO_LAYOUT_FEATURE_PARAMS
  अगर (bool (a.featureParams) != bool (b.featureParams)) वापस false;
  अगर (!a.featureParams) वापस true;
  स्थिर OT::FeatureParams &ap = a.get_feature_params ();
  स्थिर OT::FeatureParams &bp = b.get_feature_params ();
  अगर (tag == HB_TAG ('s','i','z','e'))
    वापस !hb_memcmp (&ap.get_आकार_params (tag), &bp.get_आकार_params (tag),
                       OT::Feature_Params_आकार::static_आकार);
  अगर ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0'))
    वापस !hb_memcmp (&ap.get_stylistic_set_params (tag), &bp.get_stylistic_set_params (tag),
                       OT::FeatureParamsStylisticSet::static_आकार);
  अगर ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0'))
  {
    स्थिर स्वतः &ac = ap.get_character_variants_params (tag);
    स्थिर स्वतः &bc = bp.get_character_variants_params (tag);
    वापस ac.get_आकार () == bc.get_आकार () &&
           !hb_memcmp (&ac, &bc, ac.get_आकार ());
  }
#पूर्ण_अगर
  वापस true;
}

अटल अंतरभूत व्योम
_GSUBGPOS_find_duplicate_features (स्थिर OT::GSUBGPOS &g,
				   स्थिर hb_map_t *lookup_indices,
				   स्थिर hb_set_t *feature_indices,
				   स्थिर hb_hashmap_t<अचिन्हित, स्थिर OT::Feature*> *feature_substitutes_map,
                                   bool all_axes_pinned HB_UNUSED,
				   hb_map_t *duplicate_feature_map /* OUT */)
{
  अगर (feature_indices->is_empty ()) वापस;
#अगर_अघोषित HB_NO_VAR
  hb_set_t variable_features;
  अगर (!all_axes_pinned)
    g.get_feature_variations ().collect_feature_indices (&variable_features);
  अगर (unlikely (variable_features.in_error ())) वापस;
#पूर्ण_अगर
  hb_hashmap_t<hb_tag_t, hb::unique_ptr<hb_set_t>> unique_features;
  //find out duplicate features after subset
  क्रम (अचिन्हित i : feature_indices->iter ())
  {
    hb_tag_t t = g.get_feature_tag (i);
    अगर (t == HB_MAP_मूल्य_INVALID) जारी;
    अगर (!unique_features.has (t))
    {
      अगर (unlikely (!unique_features.set (t, hb::unique_ptr<hb_set_t> {hb_set_create ()})))
	वापस;
      अगर (unique_features.has (t))
	unique_features.लो (t)->add (i);
      duplicate_feature_map->set (i, i);
      जारी;
    }

    bool found = false;

    hb_set_t* same_tag_features = unique_features.लो (t);
    क्रम (अचिन्हित other_f_सूचकांक : same_tag_features->iter ())
    {
#अगर_अघोषित HB_NO_VAR
      /* Matching base lookups do not imply matching alternate behavior. */
      अगर (variable_features.has (i) || variable_features.has (other_f_सूचकांक) ||
          g.get_feature_variations ().has_lookup_variations (i) ||
	  g.get_feature_variations ().has_lookup_variations (other_f_सूचकांक))
	जारी;
#पूर्ण_अगर

      स्थिर OT::Feature* f = &(g.get_feature (i));
      स्थिर OT::Feature **p = nullptr;
      अगर (feature_substitutes_map->has (i, &p))
        f = *p;

      स्थिर OT::Feature* other_f = &(g.get_feature (other_f_सूचकांक));
      अगर (feature_substitutes_map->has (other_f_सूचकांक, &p))
        other_f = *p;

      अगर (!_features_have_equal_params (*f, *other_f, t)) जारी;

      स्वतः f_iter =
      + hb_iter (f->lookup_सूचकांक_2)
      | hb_filter (lookup_indices)
      ;

      स्वतः other_f_iter =
      + hb_iter (other_f->lookup_सूचकांक_2)
      | hb_filter (lookup_indices)
      ;

      bool is_equal = true;
      क्रम (; f_iter && other_f_iter; f_iter++, other_f_iter++)
      {
	अचिन्हित a = *f_iter;
	अचिन्हित b = *other_f_iter;
	अगर (a != b) { is_equal = false; अवरोध; }
      }

      अगर (is_equal == false || f_iter || other_f_iter) जारी;

      found = true;
      duplicate_feature_map->set (i, other_f_सूचकांक);
      अवरोध;
    }

    अगर (found == false)
    {
      same_tag_features->add (i);
      duplicate_feature_map->set (i, i);
    }
  }
}

अटल व्योम
remap_feature_indices (स्थिर hb_set_t &feature_indices,
                       स्थिर hb_map_t &duplicate_feature_map,
                       स्थिर hb_hashmap_t<अचिन्हित, hb_pair_t<स्थिर व्योम*, स्थिर व्योम*>>& catch_all_record_idx_feature_map,
                       hb_map_t       *mapping, /* OUT */
                       hb_map_t       *mapping_w_duplicates /* OUT */)
{
  अचिन्हित i = 0;
  क्रम (स्थिर स्वतः _ : feature_indices)
  {
    // retain those features in case we need to insert a catch-all record to reinstate the old features
    अगर (catch_all_record_idx_feature_map.has (_))
    {
      mapping->set (_, i);
      mapping_w_duplicates->set (_, i);
      i++;
    }
    अन्यथा
    {
      uint32_t f_idx = duplicate_feature_map.लो (_);
      uint32_t *new_idx;
      अगर (mapping-> has (f_idx, &new_idx))
      {
        mapping_w_duplicates->set (_, *new_idx);
      }
      अन्यथा
      {
        mapping->set (_, i);
        mapping_w_duplicates->set (_, i);
        i++;
      }
    }
  }
}

ढाँचा <typename T>
अटल व्योम
_closure_glyphs_lookups_features (hb_subset_plan_t   *plan,
				 hb_set_t	     *gids_to_retain,
				 hb_map_t	     *lookups,
				 hb_map_t	     *features,
				 hb_map_t	     *features_w_duplicates,
				 script_langsys_map *langsys_map,
				 hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
				 hb_hashmap_t<अचिन्हित, स्थिर OT::Feature*> *feature_substitutes_map,
                                 hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *lookup_variations,
                                 hb_set_t &catch_all_record_feature_idxes,
                                 hb_hashmap_t<अचिन्हित, hb_pair_t<स्थिर व्योम*, स्थिर व्योम*>>& catch_all_record_idx_feature_map)
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

  अगर (table_tag == HB_OT_TAG_GSUB && !(plan->flags & HB_SUBSET_FLAGS_NO_LAYOUT_CLOSURE))
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

  feature_indices.सफा ();
  table->prune_langsys (&duplicate_feature_map, &plan->layout_scripts, langsys_map, &feature_indices);
  remap_feature_indices (feature_indices, duplicate_feature_map, catch_all_record_idx_feature_map, features, features_w_duplicates);

#अगर_अघोषित HB_NO_VAR
  // Populate alternate parameter tags after deduplication. Before this point
  // the map contains only catch-all features and also controls their retention.
  अगर (table->get_feature_variations ().record_गिनती ())
    क्रम (स्वतः p : *features_w_duplicates)
    {
      स्थिर OT::Feature& f = table->get_feature (p.first);
      स्थिर व्योम *tag = &(table->get_feature_list ().get_tag (p.first));
      catch_all_record_idx_feature_map.set (p.first, hb_pair (&f, tag));
    }
#पूर्ण_अगर

  table.destroy ();
}

व्योम layout_nameid_closure (hb_subset_plan_t* plan,
                             hb_set_t* drop_tables)
{
  अगर (!drop_tables->has (HB_OT_TAG_GPOS))
  {
    hb_blob_ptr_t<GPOS> gpos = plan->source_table<GPOS> ();
    gpos->collect_name_ids (&plan->gpos_features, &plan->name_ids,
                            &plan->gpos_feature_substitutes_map,
                            plan->user_axes_location.is_empty () ? nullptr : &plan->gpos_feature_record_cond_idx_map,
                            &plan->gpos_old_features,
                            plan->all_axes_pinned);
    gpos.destroy ();
  }
  अगर (!drop_tables->has (HB_OT_TAG_GSUB))
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

व्योम
layout_populate_gids_to_retain (hb_subset_plan_t* plan,
		                hb_set_t* drop_tables) {
  अगर (!drop_tables->has (HB_OT_TAG_GSUB))
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

  अगर (!drop_tables->has (HB_OT_TAG_GPOS))
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

#अगर_अघोषित HB_NO_VAR
व्योम
collect_layout_variation_indices (hb_subset_plan_t* plan)
{
  hb_blob_ptr_t<OT::GDEF> gdef = plan->source_table<OT::GDEF> ();
  hb_blob_ptr_t<GSUB> gsub = plan->source_table<GSUB> ();
  hb_blob_ptr_t<GPOS> gpos = plan->source_table<GPOS> ();

  अगर (!gdef->has_तथ्य () || !gdef->has_var_store ())
  {
    gdef.destroy ();
    gsub.destroy ();
    gpos.destroy ();
    वापस;
  }

  hb_set_t varidx_set;
  OT::hb_collect_variation_indices_context_t c (&varidx_set,
                                                &plan->_glyphset_gsub,
                                                &plan->gpos_lookups);
  अगर (plan->normalized_coords && !plan->all_axes_pinned)
    c.condition_मूल्यावली = &plan->layout_condition_मूल्यावली;
  gdef->collect_variation_indices (&c);

  अगर (hb_ot_layout_has_substitution (plan->source))
    gsub->collect_feature_variation_indices (&c);
  अगर (hb_ot_layout_has_positioning (plan->source))
  {
    gpos->collect_feature_variation_indices (&c);
    gpos->collect_variation_indices (&c);
  }

#अगर_अघोषित HB_NO_MATH
  अगर (!plan->drop_tables.has (HB_OT_TAG_MATH))
  {
    hb_blob_ptr_t<OT::MATH> math = plan->source_table<OT::MATH> ();
    स्वतः *glyph_set = c.glyph_set;
    c.glyph_set = &plan->_glyphset_mathed;
    math->collect_variation_indices (&c, &plan->_glyphset_cmaped);
    c.glyph_set = glyph_set;
    math.destroy ();
  }
#पूर्ण_अगर

  remap_variation_indices (gdef->get_var_store (),
                           varidx_set, plan->normalized_coords,
                           !plan->pinned_at_default,
                           plan->all_axes_pinned,
                           plan->layout_variation_idx_delta_map);

  अचिन्हित subtable_गिनती = gdef->get_var_store ().get_sub_table_गिनती ();
  generate_varstore_inner_maps (varidx_set, subtable_गिनती, plan->gdef_varstore_inner_maps);
  plan->check_success (!plan->layout_condition_मूल्यावली.in_error ());

  gdef.destroy ();
  gsub.destroy ();
  gpos.destroy ();
}
#पूर्ण_अगर

#पूर्ण_अगर

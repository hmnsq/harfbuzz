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

#अन्तर्भाव "hb-subset-plan.hh"

#अन्तर्भाव "hb-ot-layout-gdef-table.hh"
#अन्तर्भाव "hb-ot-layout-gpos-table.hh"
#अन्तर्भाव "hb-ot-layout-gsub-table.hh"
#अन्तर्भाव "hb-ot-math-table.hh"

using OT::Layout::GSUB;
using OT::Layout::GPOS;

#ifndef HB_NO_SUBSET_LAYOUT

द्विगुण
hb_subset_plan_t::layout_variation_delta (अचिन्हित var_idx HB_UNUSED)
{
#ifndef HB_NO_VAR
  hb_blob_ptr_t<OT::GDEF> gdef = source_table<OT::GDEF> ();
  द्विगुण delta = gdef->get_var_store ().get_delta<द्विगुण> (var_idx,
                                                normalized_coords.as_array ());
  gdef.destroy ();
  फर्काऊ delta;
#अन्यथा
  फर्काऊ 0.f;
#endif
}

शून्य
remap_used_mark_sets (hb_subset_plan_t *plan,
                      hb_map_t& used_mark_sets_map)
{
  hb_blob_ptr_t<OT::GDEF> gdef = plan->source_table<OT::GDEF> ();

  यदि (!gdef->has_data () || !gdef->has_mark_glyph_sets ())
  {
    gdef.destroy ();
    फर्काऊ;
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
स्थिर तार्किक _filter_tag_list(hb_vector_t<hb_tag_t>* tags, /* IN/OUT */
                             अचर hb_set_t* filter)
{
  hb_vector_t<hb_tag_t> out;
  out.alloc (tags->get_size() + 1); // +1 is to allocate room for the null terminator.

  तार्किक removed = असत्य;
  hb_set_t visited;

  लागि (hb_tag_t tag : *tags)
  {
    यदि (!tag) जारी;
    यदि (visited.has (tag)) जारी;

    यदि (!filter->has (tag))
    {
      removed = सत्य;
      जारी;
    }

    visited.add (tag);
    out.push (tag);
  }

  // The collect function needs a null element to signal end of the array.
  out.push (HB_TAG_NONE);

  hb_swap (out, *tags);
  फर्काऊ removed;
}

ढाँचा <typename T>
स्थिर तार्किक
_collect_instanced_lookup_variations (
    hb_subset_plan_t *plan,
    अचर T& table,
    अचर hb_set_t *feature_indices,
    hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *lookup_variations)
{
  hb_blob_ptr_t<OT::GDEF> gdef = plan->source_table<OT::GDEF> ();
  OT::ItemVarStoreInstancer instancer (&gdef->get_var_store (), nullptr,
				       plan->normalized_coords.as_array ());
  hb_vector_t<अचिन्हित> state;
  अचिन्हित variations_index;
  तार्किक success = table.get_feature_variations_state (
	plan->normalized_coords.arrayZ,
	plan->normalized_coords.length,
	&variations_index,
	&instancer,
	&state);
  gdef.destroy ();

  यदि (unlikely (!success || state.in_error ()))
    फर्काऊ plan->check_success (असत्य);

  अचर स्वतः &variations = table.get_feature_variations ();
  लागि (अचर स्वतः &record : variations.get_lookup_variation_records ())
  {
    अचिन्हित feature_index = record.get_feature_index ();
    यदि (!feature_indices->has (feature_index)) जारी;

    hb::shared_ptr<hb_set_t> lookups {hb_set_create ()};
    record.collect_lookups (&variations,
                           table.get_feature_variation (feature_index, variations_index),
                           state.as_sorted_array (), lookups);

    यदि (unlikely (lookups->in_error () ||
		  !lookup_variations->set (feature_index, lookups)))
      फर्काऊ plan->check_success (असत्य);
  }

  फर्काऊ सत्य;
}

ढाँचा <typename T>
स्थिर शून्य _collect_layout_indices (hb_subset_plan_t     *plan,
                                     अचर T&              table,
                                     hb_set_t		  *lookup_indices, /* OUT */
                                     hb_set_t		  *feature_indices, /* OUT */
                                     hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map, /* OUT */
                                     hb_hashmap_t<अचिन्हित, अचर OT::Feature*> *feature_substitutes_map, /* OUT */
                                     hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *lookup_variations, /* OUT */
                                     hb_set_t& catch_all_record_feature_idxes, /* OUT */
                                     hb_hashmap_t<अचिन्हित, hb_pair_t<अचर शून्य*, अचर शून्य*>>& catch_all_record_idx_feature_map /* OUT */)
{
  अचिन्हित अङ्क_features = table.get_feature_गन्ती ();
  hb_vector_t<hb_tag_t> features;
  यदि (!plan->check_success (features.resize (अङ्क_features))) फर्काऊ;
  table.get_feature_tags (0, &अङ्क_features, features.arrayZ);
  तार्किक retain_all_features = !_filter_tag_list (&features, &plan->layout_features);

  अचिन्हित अङ्क_scripts = table.get_script_गन्ती ();
  hb_vector_t<hb_tag_t> scripts;
  यदि (!plan->check_success (scripts.resize (अङ्क_scripts))) फर्काऊ;
  table.get_script_tags (0, &अङ्क_scripts, scripts.arrayZ);
  तार्किक retain_all_scripts = !_filter_tag_list (&scripts, &plan->layout_scripts);

  यदि (!plan->check_success (!features.in_error ()) || !features
      || !plan->check_success (!scripts.in_error ()) || !scripts)
    फर्काऊ;

  hb_ot_layout_collect_features (plan->source,
                                 T::tableTag,
                                 retain_all_scripts ? nullptr : scripts.arrayZ,
                                 nullptr,
                                 retain_all_features ? nullptr : features.arrayZ,
                                 feature_indices);

#ifndef HB_NO_VAR
  // collect feature substitutes with variations
  यदि (!plan->user_axes_location.is_empty ())
  {
    hb_blob_ptr_t<OT::GDEF> gdef = plan->source_table<OT::GDEF> ();
    OT::ItemVarStoreInstancer instancer (&gdef->get_var_store (), nullptr,
					 plan->normalized_coords.as_array ());
    hb_hashmap_t<hb::shared_ptr<hb_map_t>, अचिन्हित> conditionset_map;
    OT::hb_collect_feature_substitutes_with_var_context_t c =
    {
      &plan->axes_old_index_tag_map,
      &plan->axes_location,
      feature_record_cond_idx_map,
      feature_substitutes_map,
      catch_all_record_feature_idxes,
      feature_indices,
      असत्य,
      असत्य,
      असत्य,
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
  यदि (plan->all_axes_pinned &&
      !_collect_instanced_lookup_variations (plan, table, feature_indices, lookup_variations))
    फर्काऊ;
#endif

  लागि (अचिन्हित feature_index : *feature_indices)
  {
    hb::shared_ptr<hb_set_t> *lookups;
    यदि (lookup_variations->has (feature_index, &lookups))
    {
      lookup_indices->union_ (**lookups);
      जारी;
    }

    अचर OT::Feature* f = &(table.get_feature (feature_index));
    अचर OT::Feature **p = nullptr;
    यदि (feature_substitutes_map->has (feature_index, &p))
      f = *p;

    f->add_lookup_indexes_to (lookup_indices);
  }

#ifndef HB_NO_VAR
  यदि (catch_all_record_feature_idxes)
  {
    लागि (अचिन्हित feature_index : catch_all_record_feature_idxes)
    {
      अचर OT::Feature& f = table.get_feature (feature_index);
      f.add_lookup_indexes_to (lookup_indices);
      अचर शून्य *tag = reinterpret_cast<अचर शून्य*> (&(table.get_feature_list ().get_tag (feature_index)));
      catch_all_record_idx_feature_map.set (feature_index, hb_pair (&f, tag));
    }
  }

  // If all axes are pinned then all feature variations will be dropped so there's no need
  // to collect lookups from them.
  यदि (!plan->all_axes_pinned)
    table.feature_variation_collect_lookups (feature_indices,
                                             plan->user_axes_location.is_empty () ? nullptr: feature_record_cond_idx_map,
                                             lookup_indices);
#endif
}


स्थिर तार्किक
_features_have_equal_params (अचर OT::Feature &a HB_UNUSED,
                            अचर OT::Feature &b HB_UNUSED,
                            hb_tag_t tag HB_UNUSED)
{
#ifndef HB_NO_LAYOUT_FEATURE_PARAMS
  यदि (तार्किक (a.featureParams) != तार्किक (b.featureParams)) फर्काऊ असत्य;
  यदि (!a.featureParams) फर्काऊ सत्य;
  अचर OT::FeatureParams &ap = a.get_feature_params ();
  अचर OT::FeatureParams &bp = b.get_feature_params ();
  यदि (tag == HB_TAG ('s','i','z','e'))
    फर्काऊ !hb_memcmp (&ap.get_size_params (tag), &bp.get_size_params (tag),
                       OT::FeatureParamsSize::static_size);
  यदि ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0'))
    फर्काऊ !hb_memcmp (&ap.get_stylistic_set_params (tag), &bp.get_stylistic_set_params (tag),
                       OT::FeatureParamsStylisticSet::static_size);
  यदि ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0'))
  {
    अचर स्वतः &ac = ap.get_character_variants_params (tag);
    अचर स्वतः &bc = bp.get_character_variants_params (tag);
    फर्काऊ ac.get_size () == bc.get_size () &&
           !hb_memcmp (&ac, &bc, ac.get_size ());
  }
#endif
  फर्काऊ सत्य;
}

स्थिर अन्तःस्थ शून्य
_GSUBGPOS_find_duplicate_features (अचर OT::GSUBGPOS &g,
				   अचर hb_map_t *lookup_indices,
				   अचर hb_set_t *feature_indices,
				   अचर hb_hashmap_t<अचिन्हित, अचर OT::Feature*> *feature_substitutes_map,
                                   तार्किक all_axes_pinned HB_UNUSED,
				   hb_map_t *duplicate_feature_map /* OUT */)
{
  यदि (feature_indices->is_empty ()) फर्काऊ;
#ifndef HB_NO_VAR
  hb_set_t variable_features;
  यदि (!all_axes_pinned)
    g.get_feature_variations ().collect_feature_indices (&variable_features);
  यदि (unlikely (variable_features.in_error ())) फर्काऊ;
#endif
  hb_hashmap_t<hb_tag_t, hb::unique_ptr<hb_set_t>> unique_features;
  //find out duplicate features after subset
  लागि (अचिन्हित i : feature_indices->iter ())
  {
    hb_tag_t t = g.get_feature_tag (i);
    यदि (t == HB_MAP_मान_INVALID) जारी;
    यदि (!unique_features.has (t))
    {
      यदि (unlikely (!unique_features.set (t, hb::unique_ptr<hb_set_t> {hb_set_create ()})))
	फर्काऊ;
      यदि (unique_features.has (t))
	unique_features.get (t)->add (i);
      duplicate_feature_map->set (i, i);
      जारी;
    }

    तार्किक found = असत्य;

    hb_set_t* same_tag_features = unique_features.get (t);
    लागि (अचिन्हित other_f_index : same_tag_features->iter ())
    {
#ifndef HB_NO_VAR
      /* Matching base lookups do not imply matching alternate behavior. */
      यदि (variable_features.has (i) || variable_features.has (other_f_index) ||
          g.get_feature_variations ().has_lookup_variations (i) ||
	  g.get_feature_variations ().has_lookup_variations (other_f_index))
	जारी;
#endif

      अचर OT::Feature* f = &(g.get_feature (i));
      अचर OT::Feature **p = nullptr;
      यदि (feature_substitutes_map->has (i, &p))
        f = *p;

      अचर OT::Feature* other_f = &(g.get_feature (other_f_index));
      यदि (feature_substitutes_map->has (other_f_index, &p))
        other_f = *p;

      यदि (!_features_have_equal_params (*f, *other_f, t)) जारी;

      स्वतः f_iter =
      + hb_iter (f->lookupIndex)
      | hb_filter (lookup_indices)
      ;

      स्वतः other_f_iter =
      + hb_iter (other_f->lookupIndex)
      | hb_filter (lookup_indices)
      ;

      तार्किक is_equal = सत्य;
      लागि (; f_iter && other_f_iter; f_iter++, other_f_iter++)
      {
	अचिन्हित a = *f_iter;
	अचिन्हित b = *other_f_iter;
	यदि (a != b) { is_equal = असत्य; रोक; }
      }

      यदि (is_equal == असत्य || f_iter || other_f_iter) जारी;

      found = सत्य;
      duplicate_feature_map->set (i, other_f_index);
      रोक;
    }

    यदि (found == असत्य)
    {
      same_tag_features->add (i);
      duplicate_feature_map->set (i, i);
    }
  }
}

स्थिर शून्य
remap_feature_indices (अचर hb_set_t &feature_indices,
                       अचर hb_map_t &duplicate_feature_map,
                       अचर hb_hashmap_t<अचिन्हित, hb_pair_t<अचर शून्य*, अचर शून्य*>>& catch_all_record_idx_feature_map,
                       hb_map_t       *mapping, /* OUT */
                       hb_map_t       *mapping_w_duplicates /* OUT */)
{
  अचिन्हित i = 0;
  लागि (अचर स्वतः _ : feature_indices)
  {
    // retain those features in case we need to insert a catch-all record to reinstate the old features
    यदि (catch_all_record_idx_feature_map.has (_))
    {
      mapping->set (_, i);
      mapping_w_duplicates->set (_, i);
      i++;
    }
    अन्यथा
    {
      uint32_t f_idx = duplicate_feature_map.get (_);
      uint32_t *new_idx;
      यदि (mapping-> has (f_idx, &new_idx))
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
स्थिर शून्य
_closure_glyphs_lookups_features (hb_subset_plan_t   *plan,
				 hb_set_t	     *gids_to_retain,
				 hb_map_t	     *lookups,
				 hb_map_t	     *features,
				 hb_map_t	     *features_w_duplicates,
				 script_langsys_map *langsys_map,
				 hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
				 hb_hashmap_t<अचिन्हित, अचर OT::Feature*> *feature_substitutes_map,
                                 hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *lookup_variations,
                                 hb_set_t &catch_all_record_feature_idxes,
                                 hb_hashmap_t<अचिन्हित, hb_pair_t<अचर शून्य*, अचर शून्य*>>& catch_all_record_idx_feature_map)
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

  यदि (table_tag == HB_OT_TAG_GSUB && !(plan->flags & HB_SUBSET_FLAGS_NO_LAYOUT_CLOSURE))
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
  यदि (table->get_feature_variations ().record_गन्ती ())
    लागि (स्वतः p : *features_w_duplicates)
    {
      अचर OT::Feature& f = table->get_feature (p.first);
      अचर शून्य *tag = &(table->get_feature_list ().get_tag (p.first));
      catch_all_record_idx_feature_map.set (p.first, hb_pair (&f, tag));
    }
#endif

  table.destroy ();
}

शून्य layout_nameid_closure (hb_subset_plan_t* plan,
                             hb_set_t* drop_tables)
{
  यदि (!drop_tables->has (HB_OT_TAG_GPOS))
  {
    hb_blob_ptr_t<GPOS> gpos = plan->source_table<GPOS> ();
    gpos->collect_name_ids (&plan->gpos_features, &plan->name_ids,
                            &plan->gpos_feature_substitutes_map,
                            plan->user_axes_location.is_empty () ? nullptr : &plan->gpos_feature_record_cond_idx_map,
                            &plan->gpos_old_features,
                            plan->all_axes_pinned);
    gpos.destroy ();
  }
  यदि (!drop_tables->has (HB_OT_TAG_GSUB))
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

शून्य
layout_populate_gids_to_retain (hb_subset_plan_t* plan,
		                hb_set_t* drop_tables) {
  यदि (!drop_tables->has (HB_OT_TAG_GSUB))
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

  यदि (!drop_tables->has (HB_OT_TAG_GPOS))
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
शून्य
collect_layout_variation_indices (hb_subset_plan_t* plan)
{
  hb_blob_ptr_t<OT::GDEF> gdef = plan->source_table<OT::GDEF> ();
  hb_blob_ptr_t<GSUB> gsub = plan->source_table<GSUB> ();
  hb_blob_ptr_t<GPOS> gpos = plan->source_table<GPOS> ();

  यदि (!gdef->has_data () || !gdef->has_var_store ())
  {
    gdef.destroy ();
    gsub.destroy ();
    gpos.destroy ();
    फर्काऊ;
  }

  hb_set_t varidx_set;
  OT::hb_collect_variation_indices_context_t c (&varidx_set,
                                                &plan->_glyphset_gsub,
                                                &plan->gpos_lookups);
  यदि (plan->normalized_coords && !plan->all_axes_pinned)
    c.condition_मानहरू = &plan->layout_condition_मानहरू;
  gdef->collect_variation_indices (&c);

  यदि (hb_ot_layout_has_substitution (plan->source))
    gsub->collect_feature_variation_indices (&c);
  यदि (hb_ot_layout_has_positioning (plan->source))
  {
    gpos->collect_feature_variation_indices (&c);
    gpos->collect_variation_indices (&c);
  }

#ifndef HB_NO_MATH
  यदि (!plan->drop_tables.has (HB_OT_TAG_MATH))
  {
    hb_blob_ptr_t<OT::MATH> math = plan->source_table<OT::MATH> ();
    स्वतः *glyph_set = c.glyph_set;
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

  अचिन्हित subtable_गन्ती = gdef->get_var_store ().get_sub_table_गन्ती ();
  generate_varstore_inner_maps (varidx_set, subtable_गन्ती, plan->gdef_varstore_inner_maps);
  plan->check_success (!plan->layout_condition_मानहरू.in_error ());

  gdef.destroy ();
  gsub.destroy ();
  gpos.destroy ();
}
#endif

#endif

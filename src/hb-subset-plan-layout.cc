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

#شامل "hb-subset-plan.hh"

#شامل "hb-ot-layout-gdef-table.hh"
#شامل "hb-ot-layout-gpos-table.hh"
#شامل "hb-ot-layout-gsub-table.hh"
#شامل "hb-ot-math-table.hh"

استعمال OT::Layout::GSUB;
استعمال OT::Layout::GPOS;

#اگر_غیر_تعریف_شدہ HB_NO_SUBSET_LAYOUT

دوہرا
hb_subset_plan_t::layout_variation_delta (غیرعلامتی var_idx HB_غیر_مستعمل)
{
#اگر_غیر_تعریف_شدہ HB_NO_VAR
  hb_blob_ptr_t<OT::GDEF> gdef = ماخذ_جدول<OT::GDEF> ();
  دوہرا delta = gdef->get_var_store ().get_delta<دوہرا> (var_idx,
                                                normalized_coords.as_array ());
  gdef.destroy ();
  واپس delta;
#ورنہ
  واپس 0.f;
#اگر_ختم
}

خالی
remap_مستعمل_mark_sets (hb_subset_plan_t *plan,
                      hb_map_t& مستعمل_mark_sets_map)
{
  hb_blob_ptr_t<OT::GDEF> gdef = plan->ماخذ_جدول<OT::GDEF> ();

  اگر (!gdef->has_ڈیٹا () || !gdef->has_mark_glyph_sets ())
  {
    gdef.destroy ();
    واپس;
  }

  hb_سیٹ_t مستعمل_mark_sets;
  gdef->get_mark_glyph_sets ().collect_مستعمل_mark_sets (plan->_glyphset_gsub, مستعمل_mark_sets);
  gdef.destroy ();

  remap_indexes (&مستعمل_mark_sets, &مستعمل_mark_sets_map);
}

/*
 * Removes all tags from 'tags' that are not in filter. Additionally eliminates any duplicates.
 * Returns true if anything was removed (not including duplicates).
 */
ساکن منطقی filter_لیبل_list(hb_سمتیہ_t<hb_لیبل_t>* لیبل_فہرست, /* IN/OUT */
                             ثابت hb_سیٹ_t* filter)
{
  hb_سمتیہ_t<hb_لیبل_t> برآمد;
  برآمد.alloc (لیبل_فہرست->get_سائز() + 1); // +1 is to allocate room for the null terminator.

  منطقی removed = جھوٹ;
  hb_سیٹ_t دیکھا;

  برائے (hb_لیبل_t لیبل_2 : *لیبل_فہرست)
  {
    اگر (!لیبل_2) جاری;
    اگر (دیکھا.has (لیبل_2)) جاری;

    اگر (!filter->has (لیبل_2))
    {
      removed = سچ;
      جاری;
    }

    دیکھا.جوڑو (لیبل_2);
    برآمد.push (لیبل_2);
  }

  // The collect function needs a null element to signal end of the array.
  برآمد.push (HB_لیبل_NONE);

  hb_تبادلہ (برآمد, *لیبل_فہرست);
  واپس removed;
}

سانچہ <قسم_نام T>
ساکن منطقی
_collect_instanced_lookup_variations (
    hb_subset_plan_t *plan,
    ثابت T& جدول,
    ثابت hb_سیٹ_t *feature_indices,
    hb_hashmap_t<غیرعلامتی, hb::shared_ptr<hb_سیٹ_t>> *lookup_variations)
{
  hb_blob_ptr_t<OT::GDEF> gdef = plan->ماخذ_جدول<OT::GDEF> ();
  OT::شے_Var_Store_Instancer instancer (&gdef->get_var_store (), خالی_اشاریہ,
				       plan->normalized_coords.as_array ());
  hb_سمتیہ_t<غیرعلامتی> state;
  غیرعلامتی variations_اشاریہ;
  منطقی success = جدول.get_feature_variations_state (
	plan->normalized_coords.arrayZ,
	plan->normalized_coords.لمبائی,
	&variations_اشاریہ,
	&instancer,
	&state);
  gdef.destroy ();

  اگر (unlikely (!success || state.اندر_error ()))
    واپس plan->check_success (جھوٹ);

  ثابت خودکار &variations = جدول.get_feature_variations ();
  برائے (ثابت خودکار &ریکارڈ : variations.get_lookup_variation_اندراجات ())
  {
    غیرعلامتی feature_اشاریہ = ریکارڈ.get_feature_اشاریہ ();
    اگر (!feature_indices->has (feature_اشاریہ)) جاری;

    hb::shared_ptr<hb_سیٹ_t> lookups {hb_سیٹ_create ()};
    ریکارڈ.collect_lookups (&variations,
                           جدول.get_feature_variation (feature_اشاریہ, variations_اشاریہ),
                           state.as_sorted_array (), lookups);

    اگر (unlikely (lookups->اندر_error () ||
		  !lookup_variations->سیٹ (feature_اشاریہ, lookups)))
      واپس plan->check_success (جھوٹ);
  }

  واپس سچ;
}

سانچہ <قسم_نام T>
ساکن خالی _collect_layout_indices (hb_subset_plan_t     *plan,
                                     ثابت T&              جدول,
                                     hb_سیٹ_t		  *lookup_indices, /*برآمد*/
                                     hb_سیٹ_t		  *feature_indices, /*برآمد*/
                                     hb_hashmap_t<غیرعلامتی, hb::shared_ptr<hb_سیٹ_t>> *feature_ریکارڈ_cond_idx_map, /*برآمد*/
                                     hb_hashmap_t<غیرعلامتی, ثابت OT::Feature*> *feature_substitutes_map, /*برآمد*/
                                     hb_hashmap_t<غیرعلامتی, hb::shared_ptr<hb_سیٹ_t>> *lookup_variations, /*برآمد*/
                                     hb_سیٹ_t& catch_all_ریکارڈ_feature_idxes, /*برآمد*/
                                     hb_hashmap_t<غیرعلامتی, hb_pair_t<ثابت خالی*, ثابت خالی*>>& catch_all_ریکارڈ_idx_feature_map /*برآمد*/)
{
  غیرعلامتی عدد_features = جدول.get_feature_گنتی ();
  hb_سمتیہ_t<hb_لیبل_t> features;
  اگر (!plan->check_success (features.resize (عدد_features))) واپس;
  جدول.get_feature_لیبل_فہرست (0, &عدد_features, features.arrayZ);
  منطقی retain_all_features = !filter_لیبل_list (&features, &plan->layout_features);

  غیرعلامتی عدد_scripts = جدول.get_script_گنتی ();
  hb_سمتیہ_t<hb_لیبل_t> scripts;
  اگر (!plan->check_success (scripts.resize (عدد_scripts))) واپس;
  جدول.get_script_لیبل_فہرست (0, &عدد_scripts, scripts.arrayZ);
  منطقی retain_all_scripts = !filter_لیبل_list (&scripts, &plan->layout_scripts);

  اگر (!plan->check_success (!features.اندر_error ()) || !features
      || !plan->check_success (!scripts.اندر_error ()) || !scripts)
    واپس;

  hb_ot_layout_collect_features (plan->ماخذ,
                                 T::جدول_لیبل,
                                 retain_all_scripts ? خالی_اشاریہ : scripts.arrayZ,
                                 خالی_اشاریہ,
                                 retain_all_features ? خالی_اشاریہ : features.arrayZ,
                                 feature_indices);

#اگر_غیر_تعریف_شدہ HB_NO_VAR
  // collect feature substitutes with variations
  اگر (!plan->user_axes_location.is_empty ())
  {
    hb_blob_ptr_t<OT::GDEF> gdef = plan->ماخذ_جدول<OT::GDEF> ();
    OT::شے_Var_Store_Instancer instancer (&gdef->get_var_store (), خالی_اشاریہ,
					 plan->normalized_coords.as_array ());
    hb_hashmap_t<hb::shared_ptr<hb_map_t>, غیرعلامتی> conditionset_map;
    OT::hb_collect_feature_substitutes_with_var_context_t c =
    {
      &plan->axes_old_اشاریہ_لیبل_map,
      &plan->axes_location,
      feature_ریکارڈ_cond_idx_map,
      feature_substitutes_map,
      catch_all_ریکارڈ_feature_idxes,
      feature_indices,
      جھوٹ,
      جھوٹ,
      جھوٹ,
      0,
      &conditionset_map,
      &instancer,
      plan->all_axes_pinned
    };
    جدول.collect_feature_substitutes_with_variations (&c);
    gdef.destroy ();
  }
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_NO_VAR
  اگر (plan->all_axes_pinned &&
      !_collect_instanced_lookup_variations (plan, جدول, feature_indices, lookup_variations))
    واپس;
#اگر_ختم

  برائے (غیرعلامتی feature_اشاریہ : *feature_indices)
  {
    hb::shared_ptr<hb_سیٹ_t> *lookups;
    اگر (lookup_variations->has (feature_اشاریہ, &lookups))
    {
      lookup_indices->union_ (**lookups);
      جاری;
    }

    ثابت OT::Feature* f = &(جدول.get_feature (feature_اشاریہ));
    ثابت OT::Feature **p = خالی_اشاریہ;
    اگر (feature_substitutes_map->has (feature_اشاریہ, &p))
      f = *p;

    f->جوڑو_lookup_indexes_مقصد (lookup_indices);
  }

#اگر_غیر_تعریف_شدہ HB_NO_VAR
  اگر (catch_all_ریکارڈ_feature_idxes)
  {
    برائے (غیرعلامتی feature_اشاریہ : catch_all_ریکارڈ_feature_idxes)
    {
      ثابت OT::Feature& f = جدول.get_feature (feature_اشاریہ);
      f.جوڑو_lookup_indexes_مقصد (lookup_indices);
      ثابت خالی *لیبل_2 = reinterpret_cast<ثابت خالی*> (&(جدول.get_feature_list ().get_لیبل (feature_اشاریہ)));
      catch_all_ریکارڈ_idx_feature_map.سیٹ (feature_اشاریہ, hb_pair (&f, لیبل_2));
    }
  }

  // If all axes are pinned then all feature variations will be dropped so there's no need
  // to collect lookups from them.
  اگر (!plan->all_axes_pinned)
    جدول.feature_variation_collect_lookups (feature_indices,
                                             plan->user_axes_location.is_empty () ? خالی_اشاریہ: feature_ریکارڈ_cond_idx_map,
                                             lookup_indices);
#اگر_ختم
}


ساکن منطقی
_features_have_equal_params (ثابت OT::Feature &a HB_غیر_مستعمل,
                            ثابت OT::Feature &b HB_غیر_مستعمل,
                            hb_لیبل_t لیبل_2 HB_غیر_مستعمل)
{
#اگر_غیر_تعریف_شدہ HB_NO_LAYOUT_FEATURE_PARAMS
  اگر (منطقی (a.featureParams) != منطقی (b.featureParams)) واپس جھوٹ;
  اگر (!a.featureParams) واپس سچ;
  ثابت OT::FeatureParams &ap = a.get_feature_params ();
  ثابت OT::FeatureParams &bp = b.get_feature_params ();
  اگر (لیبل_2 == HB_لیبل ('s','i','z','e'))
    واپس !hb_memcmp (&ap.get_سائز_params (لیبل_2), &bp.get_سائز_params (لیبل_2),
                       OT::Feature_Params_سائز::static_سائز);
  اگر ((لیبل_2 & 0xFFFF0000u) == HB_لیبل ('s','s','\0','\0'))
    واپس !hb_memcmp (&ap.get_stylistic_سیٹ_params (لیبل_2), &bp.get_stylistic_سیٹ_params (لیبل_2),
                       OT::Feature_Params_Stylistic_سیٹ::static_سائز);
  اگر ((لیبل_2 & 0xFFFF0000u) == HB_لیبل ('c','v','\0','\0'))
  {
    ثابت خودکار &ac = ap.get_character_variants_params (لیبل_2);
    ثابت خودکار &bc = bp.get_character_variants_params (لیبل_2);
    واپس ac.get_سائز () == bc.get_سائز () &&
           !hb_memcmp (&ac, &bc, ac.get_سائز ());
  }
#اگر_ختم
  واپس سچ;
}

ساکن اندرونی خالی
GSUBGPOS_ڈھونڈو_duplicate_features (ثابت OT::GSUBGPOS &g,
				   ثابت hb_map_t *lookup_indices,
				   ثابت hb_سیٹ_t *feature_indices,
				   ثابت hb_hashmap_t<غیرعلامتی, ثابت OT::Feature*> *feature_substitutes_map,
                                   منطقی all_axes_pinned HB_غیر_مستعمل,
				   hb_map_t *duplicate_feature_map /*برآمد*/)
{
  اگر (feature_indices->is_empty ()) واپس;
#اگر_غیر_تعریف_شدہ HB_NO_VAR
  hb_سیٹ_t variable_features;
  اگر (!all_axes_pinned)
    g.get_feature_variations ().collect_feature_indices (&variable_features);
  اگر (unlikely (variable_features.اندر_error ())) واپس;
#اگر_ختم
  hb_hashmap_t<hb_لیبل_t, hb::unique_ptr<hb_سیٹ_t>> unique_features;
  //find out duplicate features after subset
  برائے (غیرعلامتی i : feature_indices->iter ())
  {
    hb_لیبل_t t = g.get_feature_لیبل (i);
    اگر (t == HB_MAP_قدر_INVALID) جاری;
    اگر (!unique_features.has (t))
    {
      اگر (unlikely (!unique_features.سیٹ (t, hb::unique_ptr<hb_سیٹ_t> {hb_سیٹ_create ()})))
	واپس;
      اگر (unique_features.has (t))
	unique_features.get (t)->جوڑو (i);
      duplicate_feature_map->سیٹ (i, i);
      جاری;
    }

    منطقی ملا = جھوٹ;

    hb_سیٹ_t* same_لیبل_features = unique_features.get (t);
    برائے (غیرعلامتی other_f_اشاریہ : same_لیبل_features->iter ())
    {
#اگر_غیر_تعریف_شدہ HB_NO_VAR
      /* Matching base lookups do not imply matching alternate behavior. */
      اگر (variable_features.has (i) || variable_features.has (other_f_اشاریہ) ||
          g.get_feature_variations ().has_lookup_variations (i) ||
	  g.get_feature_variations ().has_lookup_variations (other_f_اشاریہ))
	جاری;
#اگر_ختم

      ثابت OT::Feature* f = &(g.get_feature (i));
      ثابت OT::Feature **p = خالی_اشاریہ;
      اگر (feature_substitutes_map->has (i, &p))
        f = *p;

      ثابت OT::Feature* other_f = &(g.get_feature (other_f_اشاریہ));
      اگر (feature_substitutes_map->has (other_f_اشاریہ, &p))
        other_f = *p;

      اگر (!_features_have_equal_params (*f, *other_f, t)) جاری;

      خودکار f_iter =
      + hb_iter (f->lookup_اشاریہ_2)
      | hb_filter (lookup_indices)
      ;

      خودکار other_f_iter =
      + hb_iter (other_f->lookup_اشاریہ_2)
      | hb_filter (lookup_indices)
      ;

      منطقی is_equal = سچ;
      برائے (; f_iter && other_f_iter; f_iter++, other_f_iter++)
      {
	غیرعلامتی a = *f_iter;
	غیرعلامتی b = *other_f_iter;
	اگر (a != b) { is_equal = جھوٹ; توڑو; }
      }

      اگر (is_equal == جھوٹ || f_iter || other_f_iter) جاری;

      ملا = سچ;
      duplicate_feature_map->سیٹ (i, other_f_اشاریہ);
      توڑو;
    }

    اگر (ملا == جھوٹ)
    {
      same_لیبل_features->جوڑو (i);
      duplicate_feature_map->سیٹ (i, i);
    }
  }
}

ساکن خالی
remap_feature_indices (ثابت hb_سیٹ_t &feature_indices,
                       ثابت hb_map_t &duplicate_feature_map,
                       ثابت hb_hashmap_t<غیرعلامتی, hb_pair_t<ثابت خالی*, ثابت خالی*>>& catch_all_ریکارڈ_idx_feature_map,
                       hb_map_t       *mapping, /*برآمد*/
                       hb_map_t       *mapping_w_duplicates /*برآمد*/)
{
  غیرعلامتی i = 0;
  برائے (ثابت خودکار _ : feature_indices)
  {
    // retain those features in case we need to insert a catch-all record to reinstate the old features
    اگر (catch_all_ریکارڈ_idx_feature_map.has (_))
    {
      mapping->سیٹ (_, i);
      mapping_w_duplicates->سیٹ (_, i);
      i++;
    }
    ورنہ
    {
      uint32_t f_idx = duplicate_feature_map.get (_);
      uint32_t *new_idx;
      اگر (mapping-> has (f_idx, &new_idx))
      {
        mapping_w_duplicates->سیٹ (_, *new_idx);
      }
      ورنہ
      {
        mapping->سیٹ (_, i);
        mapping_w_duplicates->سیٹ (_, i);
        i++;
      }
    }
  }
}

سانچہ <قسم_نام T>
ساکن خالی
احاطہ_glyphs_lookups_features (hb_subset_plan_t   *plan,
				 hb_سیٹ_t	     *gids_مقصد_retain,
				 hb_map_t	     *lookups,
				 hb_map_t	     *features,
				 hb_map_t	     *features_w_duplicates,
				 script_langsys_map *langsys_map,
				 hb_hashmap_t<غیرعلامتی, hb::shared_ptr<hb_سیٹ_t>> *feature_ریکارڈ_cond_idx_map,
				 hb_hashmap_t<غیرعلامتی, ثابت OT::Feature*> *feature_substitutes_map,
                                 hb_hashmap_t<غیرعلامتی, hb::shared_ptr<hb_سیٹ_t>> *lookup_variations,
                                 hb_سیٹ_t &catch_all_ریکارڈ_feature_idxes,
                                 hb_hashmap_t<غیرعلامتی, hb_pair_t<ثابت خالی*, ثابت خالی*>>& catch_all_ریکارڈ_idx_feature_map)
{
  hb_blob_ptr_t<T> جدول = plan->ماخذ_جدول<T> ();
  hb_لیبل_t جدول_لیبل_2 = جدول->جدول_لیبل;
  hb_سیٹ_t lookup_indices, feature_indices;
  _collect_layout_indices<T> (plan,
                              *جدول,
                              &lookup_indices,
                              &feature_indices,
                              feature_ریکارڈ_cond_idx_map,
                              feature_substitutes_map,
                              lookup_variations,
                              catch_all_ریکارڈ_feature_idxes,
                              catch_all_ریکارڈ_idx_feature_map);

  اگر (جدول_لیبل_2 == HB_OT_لیبل_GSUB && !(plan->flags & HB_SUBSET_FLAGS_NO_LAYOUT_احاطہ))
    hb_ot_layout_lookups_substitute_احاطہ (plan->ماخذ,
                                             &lookup_indices,
					     gids_مقصد_retain);
  جدول->احاطہ_lookups (plan->ماخذ,
			  gids_مقصد_retain,
                          &lookup_indices);
  remap_indexes (&lookup_indices, lookups);

  // prune features
  جدول->prune_features (lookups,
                         plan->user_axes_location.is_empty () ? خالی_اشاریہ : feature_ریکارڈ_cond_idx_map,
                         feature_substitutes_map,
                         &feature_indices);
  hb_map_t duplicate_feature_map;
  GSUBGPOS_ڈھونڈو_duplicate_features (*جدول, lookups, &feature_indices, feature_substitutes_map,
                                    plan->all_axes_pinned, &duplicate_feature_map);

  feature_indices.clear ();
  جدول->prune_langsys (&duplicate_feature_map, &plan->layout_scripts, langsys_map, &feature_indices);
  remap_feature_indices (feature_indices, duplicate_feature_map, catch_all_ریکارڈ_idx_feature_map, features, features_w_duplicates);

#اگر_غیر_تعریف_شدہ HB_NO_VAR
  // Populate alternate parameter tags after deduplication. Before this point
  // the map contains only catch-all features and also controls their retention.
  اگر (جدول->get_feature_variations ().ریکارڈ_گنتی ())
    برائے (خودکار p : *features_w_duplicates)
    {
      ثابت OT::Feature& f = جدول->get_feature (p.پہلا);
      ثابت خالی *لیبل_2 = &(جدول->get_feature_list ().get_لیبل (p.پہلا));
      catch_all_ریکارڈ_idx_feature_map.سیٹ (p.پہلا, hb_pair (&f, لیبل_2));
    }
#اگر_ختم

  جدول.destroy ();
}

خالی layout_nameid_احاطہ (hb_subset_plan_t* plan,
                             hb_سیٹ_t* drop_tables)
{
  اگر (!drop_tables->has (HB_OT_لیبل_GPOS))
  {
    hb_blob_ptr_t<GPOS> gpos = plan->ماخذ_جدول<GPOS> ();
    gpos->collect_name_ids (&plan->gpos_features, &plan->name_ids,
                            &plan->gpos_feature_substitutes_map,
                            plan->user_axes_location.is_empty () ? خالی_اشاریہ : &plan->gpos_feature_ریکارڈ_cond_idx_map,
                            &plan->gpos_old_features,
                            plan->all_axes_pinned);
    gpos.destroy ();
  }
  اگر (!drop_tables->has (HB_OT_لیبل_GSUB))
  {
    hb_blob_ptr_t<GSUB> gsub = plan->ماخذ_جدول<GSUB> ();
    gsub->collect_name_ids (&plan->gsub_features, &plan->name_ids,
                            &plan->gsub_feature_substitutes_map,
                            plan->user_axes_location.is_empty () ? خالی_اشاریہ : &plan->gsub_feature_ریکارڈ_cond_idx_map,
                            &plan->gsub_old_features,
                            plan->all_axes_pinned);
    gsub.destroy ();
  }
}

خالی
layout_populate_gids_مقصد_retain (hb_subset_plan_t* plan,
		                hb_سیٹ_t* drop_tables) {
  اگر (!drop_tables->has (HB_OT_لیبل_GSUB))
    // closure all glyphs/lookups/features needed for GSUB substitutions.
    احاطہ_glyphs_lookups_features<GSUB> (
        plan,
        &plan->_glyphset_gsub,
        &plan->gsub_lookups,
        &plan->gsub_features,
        &plan->gsub_features_w_duplicates,
        &plan->gsub_langsys,
        &plan->gsub_feature_ریکارڈ_cond_idx_map,
        &plan->gsub_feature_substitutes_map,
        &plan->gsub_lookup_variations,
        plan->gsub_old_features,
        plan->gsub_old_feature_idx_لیبل_map);

  اگر (!drop_tables->has (HB_OT_لیبل_GPOS))
    احاطہ_glyphs_lookups_features<GPOS> (
        plan,
        &plan->_glyphset_gsub,
        &plan->gpos_lookups,
        &plan->gpos_features,
        &plan->gpos_features_w_duplicates,
        &plan->gpos_langsys,
        &plan->gpos_feature_ریکارڈ_cond_idx_map,
        &plan->gpos_feature_substitutes_map,
        &plan->gpos_lookup_variations,
        plan->gpos_old_features,
        plan->gpos_old_feature_idx_لیبل_map);
}

#اگر_غیر_تعریف_شدہ HB_NO_VAR
خالی
collect_layout_variation_indices (hb_subset_plan_t* plan)
{
  hb_blob_ptr_t<OT::GDEF> gdef = plan->ماخذ_جدول<OT::GDEF> ();
  hb_blob_ptr_t<GSUB> gsub = plan->ماخذ_جدول<GSUB> ();
  hb_blob_ptr_t<GPOS> gpos = plan->ماخذ_جدول<GPOS> ();

  اگر (!gdef->has_ڈیٹا () || !gdef->has_var_store ())
  {
    gdef.destroy ();
    gsub.destroy ();
    gpos.destroy ();
    واپس;
  }

  hb_سیٹ_t varidx_سیٹ;
  OT::hb_collect_variation_indices_context_t c (&varidx_سیٹ,
                                                &plan->_glyphset_gsub,
                                                &plan->gpos_lookups);
  اگر (plan->normalized_coords && !plan->all_axes_pinned)
    c.condition_اقدار = &plan->layout_condition_اقدار;
  gdef->collect_variation_indices (&c);

  اگر (hb_ot_layout_has_substitution (plan->ماخذ))
    gsub->collect_feature_variation_indices (&c);
  اگر (hb_ot_layout_has_positioning (plan->ماخذ))
  {
    gpos->collect_feature_variation_indices (&c);
    gpos->collect_variation_indices (&c);
  }

#اگر_غیر_تعریف_شدہ HB_NO_MATH
  اگر (!plan->drop_tables.has (HB_OT_لیبل_MATH))
  {
    hb_blob_ptr_t<OT::MATH> math = plan->ماخذ_جدول<OT::MATH> ();
    خودکار *glyph_سیٹ = c.glyph_سیٹ;
    c.glyph_سیٹ = &plan->_glyphset_mathed;
    math->collect_variation_indices (&c, &plan->_glyphset_cmaped);
    c.glyph_سیٹ = glyph_سیٹ;
    math.destroy ();
  }
#اگر_ختم

  remap_variation_indices (gdef->get_var_store (),
                           varidx_سیٹ, plan->normalized_coords,
                           !plan->pinned_at_default,
                           plan->all_axes_pinned,
                           plan->layout_variation_idx_delta_map);

  غیرعلامتی subtable_گنتی = gdef->get_var_store ().get_sub_جدول_گنتی ();
  generate_varstore_inner_maps (varidx_سیٹ, subtable_گنتی, plan->gdef_varstore_inner_maps);
  plan->check_success (!plan->layout_condition_اقدار.اندر_error ());

  gdef.destroy ();
  gsub.destroy ();
  gpos.destroy ();
}
#اگر_ختم

#اگر_ختم

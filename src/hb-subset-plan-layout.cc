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

#ضمن "hb-subset-plan.hh"

#ضمن "hb-ot-layout-gdef-table.hh"
#ضمن "hb-ot-layout-gpos-table.hh"
#ضمن "hb-ot-layout-gsub-table.hh"
#ضمن "hb-ot-math-table.hh"

ورتوں OT::Layout::GSUB;
ورتوں OT::Layout::GPOS;

#جے_بنا_تعریف HB_NO_SUBSET_LAYOUT

مزدوج
hb_subset_plan_t::layout_variation_delta (بنانشانی var_idx HB_بن_ورتیا)
{
#جے_بنا_تعریف HB_NO_VAR
  hb_blob_ptr_t<OT::GDEF> gdef = سروت_جدول<OT::GDEF> ();
  مزدوج delta = gdef->get_var_store ().get_delta<مزدوج> (var_idx,
                                                normalized_coords.as_array ());
  gdef.destroy ();
  أرجع delta;
#وإلا
  أرجع 0.f;
#جے_انت
}

فراغ
remap_ورتیا_mark_sets (hb_subset_plan_t *plan,
                      hb_map_t& ورتیا_mark_sets_map)
{
  hb_blob_ptr_t<OT::GDEF> gdef = plan->سروت_جدول<OT::GDEF> ();

  إذا (!gdef->has_انکڑے () || !gdef->has_mark_glyph_sets ())
  {
    gdef.destroy ();
    أرجع;
  }

  hb_سیٹ_t ورتیا_mark_sets;
  gdef->get_mark_glyph_sets ().collect_ورتیا_mark_sets (plan->_glyphset_gsub, ورتیا_mark_sets);
  gdef.destroy ();

  remap_indexes (&ورتیا_mark_sets, &ورتیا_mark_sets_map);
}

/*
 * Removes all tags from 'tags' that are not in filter. Additionally eliminates any duplicates.
 * Returns true if anything was removed (not including duplicates).
 */
ساكن منطقي filter_ٹیگ_list(hb_سمتیہ_t<hb_ٹیگ_t>* ٹیگ_سوچی, /* IN/OUT */
                             ثابت hb_سیٹ_t* filter)
{
  hb_سمتیہ_t<hb_ٹیگ_t> باہرلا;
  باہرلا.alloc (ٹیگ_سوچی->get_ناپ() + 1); // +1 is to allocate room for the null terminator.

  منطقي removed = خطأ;
  hb_سیٹ_t ویکھیا;

  لكل (hb_ٹیگ_t ٹیگ_2 : *ٹیگ_سوچی)
  {
    إذا (!ٹیگ_2) استمر;
    إذا (ویکھیا.has (ٹیگ_2)) استمر;

    إذا (!filter->has (ٹیگ_2))
    {
      removed = سچ;
      استمر;
    }

    ویکھیا.جوڑو (ٹیگ_2);
    باہرلا.push (ٹیگ_2);
  }

  // The collect function needs a null element to signal end of the array.
  باہرلا.push (HB_ٹیگ_NONE);

  hb_وٹاندرا (باہرلا, *ٹیگ_سوچی);
  أرجع removed;
}

سانچا <قسم_ناں T>
ساكن منطقي
_collect_instanced_lookup_variations (
    hb_subset_plan_t *plan,
    ثابت T& جدول,
    ثابت hb_سیٹ_t *feature_indices,
    hb_hashmap_t<بنانشانی, hb::shared_ptr<hb_سیٹ_t>> *lookup_variations)
{
  hb_blob_ptr_t<OT::GDEF> gdef = plan->سروت_جدول<OT::GDEF> ();
  OT::شے_Var_Store_Instancer instancer (&gdef->get_var_store (), خالی_سوچک,
				       plan->normalized_coords.as_array ());
  hb_سمتیہ_t<بنانشانی> state;
  بنانشانی variations_اشاریہ;
  منطقي success = جدول.get_feature_variations_state (
	plan->normalized_coords.arrayZ,
	plan->normalized_coords.لمائی,
	&variations_اشاریہ,
	&instancer,
	&state);
  gdef.destroy ();

  إذا (unlikely (!success || state.وچ_error ()))
    أرجع plan->check_success (خطأ);

  ثابت خودکار &variations = جدول.get_feature_variations ();
  لكل (ثابت خودکار &ریکارڈ : variations.get_lookup_variation_ریکارڈاں ())
  {
    بنانشانی feature_اشاریہ = ریکارڈ.get_feature_اشاریہ ();
    إذا (!feature_indices->has (feature_اشاریہ)) استمر;

    hb::shared_ptr<hb_سیٹ_t> lookups {hb_سیٹ_create ()};
    ریکارڈ.collect_lookups (&variations,
                           جدول.get_feature_variation (feature_اشاریہ, variations_اشاریہ),
                           state.as_sorted_array (), lookups);

    إذا (unlikely (lookups->وچ_error () ||
		  !lookup_variations->سیٹ (feature_اشاریہ, lookups)))
      أرجع plan->check_success (خطأ);
  }

  أرجع سچ;
}

سانچا <قسم_ناں T>
ساكن فراغ _collect_layout_indices (hb_subset_plan_t     *plan,
                                     ثابت T&              جدول,
                                     hb_سیٹ_t		  *lookup_indices, /*باہرلا*/
                                     hb_سیٹ_t		  *feature_indices, /*باہرلا*/
                                     hb_hashmap_t<بنانشانی, hb::shared_ptr<hb_سیٹ_t>> *feature_ریکارڈ_cond_idx_map, /*باہرلا*/
                                     hb_hashmap_t<بنانشانی, ثابت OT::Feature*> *feature_substitutes_map, /*باہرلا*/
                                     hb_hashmap_t<بنانشانی, hb::shared_ptr<hb_سیٹ_t>> *lookup_variations, /*باہرلا*/
                                     hb_سیٹ_t& catch_all_ریکارڈ_feature_idxes, /*باہرلا*/
                                     hb_hashmap_t<بنانشانی, hb_pair_t<ثابت فراغ*, ثابت فراغ*>>& catch_all_ریکارڈ_idx_feature_map /*باہرلا*/)
{
  بنانشانی عدد_features = جدول.get_feature_گنتی ();
  hb_سمتیہ_t<hb_ٹیگ_t> features;
  إذا (!plan->check_success (features.resize (عدد_features))) أرجع;
  جدول.get_feature_ٹیگ_سوچی (0, &عدد_features, features.arrayZ);
  منطقي retain_all_features = !filter_ٹیگ_list (&features, &plan->layout_features);

  بنانشانی عدد_scripts = جدول.get_script_گنتی ();
  hb_سمتیہ_t<hb_ٹیگ_t> scripts;
  إذا (!plan->check_success (scripts.resize (عدد_scripts))) أرجع;
  جدول.get_script_ٹیگ_سوچی (0, &عدد_scripts, scripts.arrayZ);
  منطقي retain_all_scripts = !filter_ٹیگ_list (&scripts, &plan->layout_scripts);

  إذا (!plan->check_success (!features.وچ_error ()) || !features
      || !plan->check_success (!scripts.وچ_error ()) || !scripts)
    أرجع;

  hb_ot_layout_collect_features (plan->سروت,
                                 T::جدول_ٹیگ,
                                 retain_all_scripts ? خالی_سوچک : scripts.arrayZ,
                                 خالی_سوچک,
                                 retain_all_features ? خالی_سوچک : features.arrayZ,
                                 feature_indices);

#جے_بنا_تعریف HB_NO_VAR
  // collect feature substitutes with variations
  إذا (!plan->user_axes_location.is_empty ())
  {
    hb_blob_ptr_t<OT::GDEF> gdef = plan->سروت_جدول<OT::GDEF> ();
    OT::شے_Var_Store_Instancer instancer (&gdef->get_var_store (), خالی_سوچک,
					 plan->normalized_coords.as_array ());
    hb_hashmap_t<hb::shared_ptr<hb_map_t>, بنانشانی> conditionset_map;
    OT::hb_collect_feature_substitutes_with_var_context_t c =
    {
      &plan->axes_old_اشاریہ_ٹیگ_map,
      &plan->axes_location,
      feature_ریکارڈ_cond_idx_map,
      feature_substitutes_map,
      catch_all_ریکارڈ_feature_idxes,
      feature_indices,
      خطأ,
      خطأ,
      خطأ,
      0,
      &conditionset_map,
      &instancer,
      plan->all_axes_pinned
    };
    جدول.collect_feature_substitutes_with_variations (&c);
    gdef.destroy ();
  }
#جے_انت

#جے_بنا_تعریف HB_NO_VAR
  إذا (plan->all_axes_pinned &&
      !_collect_instanced_lookup_variations (plan, جدول, feature_indices, lookup_variations))
    أرجع;
#جے_انت

  لكل (بنانشانی feature_اشاریہ : *feature_indices)
  {
    hb::shared_ptr<hb_سیٹ_t> *lookups;
    إذا (lookup_variations->has (feature_اشاریہ, &lookups))
    {
      lookup_indices->union_ (**lookups);
      استمر;
    }

    ثابت OT::Feature* f = &(جدول.get_feature (feature_اشاریہ));
    ثابت OT::Feature **p = خالی_سوچک;
    إذا (feature_substitutes_map->has (feature_اشاریہ, &p))
      f = *p;

    f->جوڑو_lookup_indexes_منزل (lookup_indices);
  }

#جے_بنا_تعریف HB_NO_VAR
  إذا (catch_all_ریکارڈ_feature_idxes)
  {
    لكل (بنانشانی feature_اشاریہ : catch_all_ریکارڈ_feature_idxes)
    {
      ثابت OT::Feature& f = جدول.get_feature (feature_اشاریہ);
      f.جوڑو_lookup_indexes_منزل (lookup_indices);
      ثابت فراغ *ٹیگ_2 = reinterpret_cast<ثابت فراغ*> (&(جدول.get_feature_list ().get_ٹیگ (feature_اشاریہ)));
      catch_all_ریکارڈ_idx_feature_map.سیٹ (feature_اشاریہ, hb_pair (&f, ٹیگ_2));
    }
  }

  // If all axes are pinned then all feature variations will be dropped so there's no need
  // to collect lookups from them.
  إذا (!plan->all_axes_pinned)
    جدول.feature_variation_collect_lookups (feature_indices,
                                             plan->user_axes_location.is_empty () ? خالی_سوچک: feature_ریکارڈ_cond_idx_map,
                                             lookup_indices);
#جے_انت
}


ساكن منطقي
_features_have_equal_params (ثابت OT::Feature &a HB_بن_ورتیا,
                            ثابت OT::Feature &b HB_بن_ورتیا,
                            hb_ٹیگ_t ٹیگ_2 HB_بن_ورتیا)
{
#جے_بنا_تعریف HB_NO_LAYOUT_FEATURE_PARAMS
  إذا (منطقي (a.featureParams) != منطقي (b.featureParams)) أرجع خطأ;
  إذا (!a.featureParams) أرجع سچ;
  ثابت OT::FeatureParams &ap = a.get_feature_params ();
  ثابت OT::FeatureParams &bp = b.get_feature_params ();
  إذا (ٹیگ_2 == HB_ٹیگ ('s','i','z','e'))
    أرجع !hb_memcmp (&ap.get_ناپ_params (ٹیگ_2), &bp.get_ناپ_params (ٹیگ_2),
                       OT::Feature_Params_ناپ::static_ناپ);
  إذا ((ٹیگ_2 & 0xFFFF0000u) == HB_ٹیگ ('s','s','\0','\0'))
    أرجع !hb_memcmp (&ap.get_stylistic_سیٹ_params (ٹیگ_2), &bp.get_stylistic_سیٹ_params (ٹیگ_2),
                       OT::Feature_Params_Stylistic_سیٹ::static_ناپ);
  إذا ((ٹیگ_2 & 0xFFFF0000u) == HB_ٹیگ ('c','v','\0','\0'))
  {
    ثابت خودکار &ac = ap.get_character_variants_params (ٹیگ_2);
    ثابت خودکار &bc = bp.get_character_variants_params (ٹیگ_2);
    أرجع ac.get_ناپ () == bc.get_ناپ () &&
           !hb_memcmp (&ac, &bc, ac.get_ناپ ());
  }
#جے_انت
  أرجع سچ;
}

ساكن مضمن فراغ
GSUBGPOS_لبھو_duplicate_features (ثابت OT::GSUBGPOS &g,
				   ثابت hb_map_t *lookup_indices,
				   ثابت hb_سیٹ_t *feature_indices,
				   ثابت hb_hashmap_t<بنانشانی, ثابت OT::Feature*> *feature_substitutes_map,
                                   منطقي all_axes_pinned HB_بن_ورتیا,
				   hb_map_t *duplicate_feature_map /*باہرلا*/)
{
  إذا (feature_indices->is_empty ()) أرجع;
#جے_بنا_تعریف HB_NO_VAR
  hb_سیٹ_t variable_features;
  إذا (!all_axes_pinned)
    g.get_feature_variations ().collect_feature_indices (&variable_features);
  إذا (unlikely (variable_features.وچ_error ())) أرجع;
#جے_انت
  hb_hashmap_t<hb_ٹیگ_t, hb::unique_ptr<hb_سیٹ_t>> unique_features;
  //find out duplicate features after subset
  لكل (بنانشانی i : feature_indices->iter ())
  {
    hb_ٹیگ_t t = g.get_feature_ٹیگ (i);
    إذا (t == HB_MAP_قدر_INVALID) استمر;
    إذا (!unique_features.has (t))
    {
      إذا (unlikely (!unique_features.سیٹ (t, hb::unique_ptr<hb_سیٹ_t> {hb_سیٹ_create ()})))
	أرجع;
      إذا (unique_features.has (t))
	unique_features.get (t)->جوڑو (i);
      duplicate_feature_map->سیٹ (i, i);
      استمر;
    }

    منطقي لبھیا = خطأ;

    hb_سیٹ_t* same_ٹیگ_features = unique_features.get (t);
    لكل (بنانشانی other_f_اشاریہ : same_ٹیگ_features->iter ())
    {
#جے_بنا_تعریف HB_NO_VAR
      /* Matching base lookups do not imply matching alternate behavior. */
      إذا (variable_features.has (i) || variable_features.has (other_f_اشاریہ) ||
          g.get_feature_variations ().has_lookup_variations (i) ||
	  g.get_feature_variations ().has_lookup_variations (other_f_اشاریہ))
	استمر;
#جے_انت

      ثابت OT::Feature* f = &(g.get_feature (i));
      ثابت OT::Feature **p = خالی_سوچک;
      إذا (feature_substitutes_map->has (i, &p))
        f = *p;

      ثابت OT::Feature* other_f = &(g.get_feature (other_f_اشاریہ));
      إذا (feature_substitutes_map->has (other_f_اشاریہ, &p))
        other_f = *p;

      إذا (!_features_have_equal_params (*f, *other_f, t)) استمر;

      خودکار f_iter =
      + hb_iter (f->lookup_اشاریہ_2)
      | hb_filter (lookup_indices)
      ;

      خودکار other_f_iter =
      + hb_iter (other_f->lookup_اشاریہ_2)
      | hb_filter (lookup_indices)
      ;

      منطقي is_equal = سچ;
      لكل (; f_iter && other_f_iter; f_iter++, other_f_iter++)
      {
	بنانشانی a = *f_iter;
	بنانشانی b = *other_f_iter;
	إذا (a != b) { is_equal = خطأ; اكسر; }
      }

      إذا (is_equal == خطأ || f_iter || other_f_iter) استمر;

      لبھیا = سچ;
      duplicate_feature_map->سیٹ (i, other_f_اشاریہ);
      اكسر;
    }

    إذا (لبھیا == خطأ)
    {
      same_ٹیگ_features->جوڑو (i);
      duplicate_feature_map->سیٹ (i, i);
    }
  }
}

ساكن فراغ
remap_feature_indices (ثابت hb_سیٹ_t &feature_indices,
                       ثابت hb_map_t &duplicate_feature_map,
                       ثابت hb_hashmap_t<بنانشانی, hb_pair_t<ثابت فراغ*, ثابت فراغ*>>& catch_all_ریکارڈ_idx_feature_map,
                       hb_map_t       *mapping, /*باہرلا*/
                       hb_map_t       *mapping_w_duplicates /*باہرلا*/)
{
  بنانشانی i = 0;
  لكل (ثابت خودکار _ : feature_indices)
  {
    // retain those features in case we need to insert a catch-all record to reinstate the old features
    إذا (catch_all_ریکارڈ_idx_feature_map.has (_))
    {
      mapping->سیٹ (_, i);
      mapping_w_duplicates->سیٹ (_, i);
      i++;
    }
    وإلا
    {
      uint32_t f_idx = duplicate_feature_map.get (_);
      uint32_t *new_idx;
      إذا (mapping-> has (f_idx, &new_idx))
      {
        mapping_w_duplicates->سیٹ (_, *new_idx);
      }
      وإلا
      {
        mapping->سیٹ (_, i);
        mapping_w_duplicates->سیٹ (_, i);
        i++;
      }
    }
  }
}

سانچا <قسم_ناں T>
ساكن فراغ
گھیرا_glyphs_lookups_features (hb_subset_plan_t   *plan,
				 hb_سیٹ_t	     *gids_منزل_retain,
				 hb_map_t	     *lookups,
				 hb_map_t	     *features,
				 hb_map_t	     *features_w_duplicates,
				 script_langsys_map *langsys_map,
				 hb_hashmap_t<بنانشانی, hb::shared_ptr<hb_سیٹ_t>> *feature_ریکارڈ_cond_idx_map,
				 hb_hashmap_t<بنانشانی, ثابت OT::Feature*> *feature_substitutes_map,
                                 hb_hashmap_t<بنانشانی, hb::shared_ptr<hb_سیٹ_t>> *lookup_variations,
                                 hb_سیٹ_t &catch_all_ریکارڈ_feature_idxes,
                                 hb_hashmap_t<بنانشانی, hb_pair_t<ثابت فراغ*, ثابت فراغ*>>& catch_all_ریکارڈ_idx_feature_map)
{
  hb_blob_ptr_t<T> جدول = plan->سروت_جدول<T> ();
  hb_ٹیگ_t جدول_ٹیگ_2 = جدول->جدول_ٹیگ;
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

  إذا (جدول_ٹیگ_2 == HB_OT_ٹیگ_GSUB && !(plan->flags & HB_SUBSET_FLAGS_NO_LAYOUT_گھیرا))
    hb_ot_layout_lookups_substitute_گھیرا (plan->سروت,
                                             &lookup_indices,
					     gids_منزل_retain);
  جدول->گھیرا_lookups (plan->سروت,
			  gids_منزل_retain,
                          &lookup_indices);
  remap_indexes (&lookup_indices, lookups);

  // prune features
  جدول->prune_features (lookups,
                         plan->user_axes_location.is_empty () ? خالی_سوچک : feature_ریکارڈ_cond_idx_map,
                         feature_substitutes_map,
                         &feature_indices);
  hb_map_t duplicate_feature_map;
  GSUBGPOS_لبھو_duplicate_features (*جدول, lookups, &feature_indices, feature_substitutes_map,
                                    plan->all_axes_pinned, &duplicate_feature_map);

  feature_indices.clear ();
  جدول->prune_langsys (&duplicate_feature_map, &plan->layout_scripts, langsys_map, &feature_indices);
  remap_feature_indices (feature_indices, duplicate_feature_map, catch_all_ریکارڈ_idx_feature_map, features, features_w_duplicates);

#جے_بنا_تعریف HB_NO_VAR
  // Populate alternate parameter tags after deduplication. Before this point
  // the map contains only catch-all features and also controls their retention.
  إذا (جدول->get_feature_variations ().ریکارڈ_گنتی ())
    لكل (خودکار p : *features_w_duplicates)
    {
      ثابت OT::Feature& f = جدول->get_feature (p.پہلا);
      ثابت فراغ *ٹیگ_2 = &(جدول->get_feature_list ().get_ٹیگ (p.پہلا));
      catch_all_ریکارڈ_idx_feature_map.سیٹ (p.پہلا, hb_pair (&f, ٹیگ_2));
    }
#جے_انت

  جدول.destroy ();
}

فراغ layout_nameid_گھیرا (hb_subset_plan_t* plan,
                             hb_سیٹ_t* drop_tables)
{
  إذا (!drop_tables->has (HB_OT_ٹیگ_GPOS))
  {
    hb_blob_ptr_t<GPOS> gpos = plan->سروت_جدول<GPOS> ();
    gpos->collect_name_ids (&plan->gpos_features, &plan->name_ids,
                            &plan->gpos_feature_substitutes_map,
                            plan->user_axes_location.is_empty () ? خالی_سوچک : &plan->gpos_feature_ریکارڈ_cond_idx_map,
                            &plan->gpos_old_features,
                            plan->all_axes_pinned);
    gpos.destroy ();
  }
  إذا (!drop_tables->has (HB_OT_ٹیگ_GSUB))
  {
    hb_blob_ptr_t<GSUB> gsub = plan->سروت_جدول<GSUB> ();
    gsub->collect_name_ids (&plan->gsub_features, &plan->name_ids,
                            &plan->gsub_feature_substitutes_map,
                            plan->user_axes_location.is_empty () ? خالی_سوچک : &plan->gsub_feature_ریکارڈ_cond_idx_map,
                            &plan->gsub_old_features,
                            plan->all_axes_pinned);
    gsub.destroy ();
  }
}

فراغ
layout_populate_gids_منزل_retain (hb_subset_plan_t* plan,
		                hb_سیٹ_t* drop_tables) {
  إذا (!drop_tables->has (HB_OT_ٹیگ_GSUB))
    // closure all glyphs/lookups/features needed for GSUB substitutions.
    گھیرا_glyphs_lookups_features<GSUB> (
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
        plan->gsub_old_feature_idx_ٹیگ_map);

  إذا (!drop_tables->has (HB_OT_ٹیگ_GPOS))
    گھیرا_glyphs_lookups_features<GPOS> (
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
        plan->gpos_old_feature_idx_ٹیگ_map);
}

#جے_بنا_تعریف HB_NO_VAR
فراغ
collect_layout_variation_indices (hb_subset_plan_t* plan)
{
  hb_blob_ptr_t<OT::GDEF> gdef = plan->سروت_جدول<OT::GDEF> ();
  hb_blob_ptr_t<GSUB> gsub = plan->سروت_جدول<GSUB> ();
  hb_blob_ptr_t<GPOS> gpos = plan->سروت_جدول<GPOS> ();

  إذا (!gdef->has_انکڑے () || !gdef->has_var_store ())
  {
    gdef.destroy ();
    gsub.destroy ();
    gpos.destroy ();
    أرجع;
  }

  hb_سیٹ_t varidx_سیٹ;
  OT::hb_collect_variation_indices_context_t c (&varidx_سیٹ,
                                                &plan->_glyphset_gsub,
                                                &plan->gpos_lookups);
  إذا (plan->normalized_coords && !plan->all_axes_pinned)
    c.condition_اقدار = &plan->layout_condition_اقدار;
  gdef->collect_variation_indices (&c);

  إذا (hb_ot_layout_has_substitution (plan->سروت))
    gsub->collect_feature_variation_indices (&c);
  إذا (hb_ot_layout_has_positioning (plan->سروت))
  {
    gpos->collect_feature_variation_indices (&c);
    gpos->collect_variation_indices (&c);
  }

#جے_بنا_تعریف HB_NO_MATH
  إذا (!plan->drop_tables.has (HB_OT_ٹیگ_MATH))
  {
    hb_blob_ptr_t<OT::MATH> math = plan->سروت_جدول<OT::MATH> ();
    خودکار *glyph_سیٹ = c.glyph_سیٹ;
    c.glyph_سیٹ = &plan->_glyphset_mathed;
    math->collect_variation_indices (&c, &plan->_glyphset_cmaped);
    c.glyph_سیٹ = glyph_سیٹ;
    math.destroy ();
  }
#جے_انت

  remap_variation_indices (gdef->get_var_store (),
                           varidx_سیٹ, plan->normalized_coords,
                           !plan->pinned_at_default,
                           plan->all_axes_pinned,
                           plan->layout_variation_idx_delta_map);

  بنانشانی subtable_گنتی = gdef->get_var_store ().get_sub_جدول_گنتی ();
  generate_varstore_inner_maps (varidx_سیٹ, subtable_گنتی, plan->gdef_varstore_inner_maps);
  plan->check_success (!plan->layout_condition_اقدار.وچ_error ());

  gdef.destroy ();
  gsub.destroy ();
  gpos.destroy ();
}
#جے_انت

#جے_انت

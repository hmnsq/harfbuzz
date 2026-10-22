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

کارول OT::Layout::GSUB;
کارول OT::Layout::GPOS;

#که_نه_تعریف_شوی HB_NO_SUBSET_LAYOUT

مزدوج
hb_subset_plan_t::layout_variation_delta (بینښه var_idx HB_ناکارول_شوی)
{
#که_نه_تعریف_شوی HB_NO_VAR
  hb_blob_ptr_t<OT::GDEF> gdef = سرچینه_جدول<OT::GDEF> ();
  مزدوج delta = gdef->get_var_store ().get_delta<مزدوج> (var_idx,
                                                normalized_coords.as_array ());
  gdef.destroy ();
  أرجع delta;
#وإلا
  أرجع 0.f;
#د_که_پای
}

فراغ
remap_کارول_شوی_mark_sets (hb_subset_plan_t *plan,
                      hb_map_t& کارول_شوی_mark_sets_map)
{
  hb_blob_ptr_t<OT::GDEF> gdef = plan->سرچینه_جدول<OT::GDEF> ();

  إذا (!gdef->has_معلومات () || !gdef->has_mark_glyph_sets ())
  {
    gdef.destroy ();
    أرجع;
  }

  hb_سېټ_t کارول_شوی_mark_sets;
  gdef->get_mark_glyph_sets ().collect_کارول_شوی_mark_sets (plan->_glyphset_gsub, کارول_شوی_mark_sets);
  gdef.destroy ();

  remap_indexes (&کارول_شوی_mark_sets, &کارول_شوی_mark_sets_map);
}

/*
 * Removes all tags from 'tags' that are not in filter. Additionally eliminates any duplicates.
 * Returns true if anything was removed (not including duplicates).
 */
ساكن منطقي filter_نښان_list(hb_ویکټور_t<hb_نښان_t>* نښانونه, /* IN/OUT */
                             ثابت hb_سېټ_t* filter)
{
  hb_ویکټور_t<hb_نښان_t> بهر;
  بهر.alloc (نښانونه->get_اندازه() + 1); // +1 is to allocate room for the null terminator.

  منطقي removed = خطأ;
  hb_سېټ_t لیدل_شوی;

  لكل (hb_نښان_t نښان_2 : *نښانونه)
  {
    إذا (!نښان_2) استمر;
    إذا (لیدل_شوی.has (نښان_2)) استمر;

    إذا (!filter->has (نښان_2))
    {
      removed = ریښتیا;
      استمر;
    }

    لیدل_شوی.جمع_کړه (نښان_2);
    بهر.push (نښان_2);
  }

  // The collect function needs a null element to signal end of the array.
  بهر.push (HB_نښان_NONE);

  hb_بدلون (بهر, *نښانونه);
  أرجع removed;
}

کينډۍ <ډول_نوم T>
ساكن منطقي
_collect_instanced_lookup_variations (
    hb_subset_plan_t *plan,
    ثابت T& جدول,
    ثابت hb_سېټ_t *feature_indices,
    hb_hashmap_t<بینښه, hb::shared_ptr<hb_سېټ_t>> *lookup_variations)
{
  hb_blob_ptr_t<OT::GDEF> gdef = plan->سرچینه_جدول<OT::GDEF> ();
  OT::توکی_Var_Store_Instancer instancer (&gdef->get_var_store (), تش_اشاره,
				       plan->normalized_coords.as_array ());
  hb_ویکټور_t<بینښه> state;
  بینښه variations_شاخص;
  منطقي success = جدول.get_feature_variations_state (
	plan->normalized_coords.arrayZ,
	plan->normalized_coords.اوږدتیا,
	&variations_شاخص,
	&instancer,
	&state);
  gdef.destroy ();

  إذا (unlikely (!success || state.دننه_error ()))
    أرجع plan->check_success (خطأ);

  ثابت خپلکاری &variations = جدول.get_feature_variations ();
  لكل (ثابت خپلکاری &ریکارډ : variations.get_lookup_variation_ریکارډونه ())
  {
    بینښه feature_شاخص = ریکارډ.get_feature_شاخص ();
    إذا (!feature_indices->has (feature_شاخص)) استمر;

    hb::shared_ptr<hb_سېټ_t> lookups {hb_سېټ_create ()};
    ریکارډ.collect_lookups (&variations,
                           جدول.get_feature_variation (feature_شاخص, variations_شاخص),
                           state.as_sorted_array (), lookups);

    إذا (unlikely (lookups->دننه_error () ||
		  !lookup_variations->سېټ (feature_شاخص, lookups)))
      أرجع plan->check_success (خطأ);
  }

  أرجع ریښتیا;
}

کينډۍ <ډول_نوم T>
ساكن فراغ _collect_layout_indices (hb_subset_plan_t     *plan,
                                     ثابت T&              جدول,
                                     hb_سېټ_t		  *lookup_indices, /*بهر*/
                                     hb_سېټ_t		  *feature_indices, /*بهر*/
                                     hb_hashmap_t<بینښه, hb::shared_ptr<hb_سېټ_t>> *feature_ریکارډ_cond_idx_map, /*بهر*/
                                     hb_hashmap_t<بینښه, ثابت OT::Feature*> *feature_substitutes_map, /*بهر*/
                                     hb_hashmap_t<بینښه, hb::shared_ptr<hb_سېټ_t>> *lookup_variations, /*بهر*/
                                     hb_سېټ_t& catch_all_ریکارډ_feature_idxes, /*بهر*/
                                     hb_hashmap_t<بینښه, hb_pair_t<ثابت فراغ*, ثابت فراغ*>>& catch_all_ریکارډ_idx_feature_map /*بهر*/)
{
  بینښه عدد_features = جدول.get_feature_شمېر ();
  hb_ویکټور_t<hb_نښان_t> features;
  إذا (!plan->check_success (features.resize (عدد_features))) أرجع;
  جدول.get_feature_نښانونه (0, &عدد_features, features.arrayZ);
  منطقي retain_all_features = !filter_نښان_list (&features, &plan->layout_features);

  بینښه عدد_scripts = جدول.get_script_شمېر ();
  hb_ویکټور_t<hb_نښان_t> scripts;
  إذا (!plan->check_success (scripts.resize (عدد_scripts))) أرجع;
  جدول.get_script_نښانونه (0, &عدد_scripts, scripts.arrayZ);
  منطقي retain_all_scripts = !filter_نښان_list (&scripts, &plan->layout_scripts);

  إذا (!plan->check_success (!features.دننه_error ()) || !features
      || !plan->check_success (!scripts.دننه_error ()) || !scripts)
    أرجع;

  hb_ot_layout_collect_features (plan->سرچینه,
                                 T::جدول_نښان,
                                 retain_all_scripts ? تش_اشاره : scripts.arrayZ,
                                 تش_اشاره,
                                 retain_all_features ? تش_اشاره : features.arrayZ,
                                 feature_indices);

#که_نه_تعریف_شوی HB_NO_VAR
  // collect feature substitutes with variations
  إذا (!plan->user_axes_location.is_empty ())
  {
    hb_blob_ptr_t<OT::GDEF> gdef = plan->سرچینه_جدول<OT::GDEF> ();
    OT::توکی_Var_Store_Instancer instancer (&gdef->get_var_store (), تش_اشاره,
					 plan->normalized_coords.as_array ());
    hb_hashmap_t<hb::shared_ptr<hb_map_t>, بینښه> conditionset_map;
    OT::hb_collect_feature_substitutes_with_var_context_t c =
    {
      &plan->axes_old_شاخص_نښان_map,
      &plan->axes_location,
      feature_ریکارډ_cond_idx_map,
      feature_substitutes_map,
      catch_all_ریکارډ_feature_idxes,
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
#د_که_پای

#که_نه_تعریف_شوی HB_NO_VAR
  إذا (plan->all_axes_pinned &&
      !_collect_instanced_lookup_variations (plan, جدول, feature_indices, lookup_variations))
    أرجع;
#د_که_پای

  لكل (بینښه feature_شاخص : *feature_indices)
  {
    hb::shared_ptr<hb_سېټ_t> *lookups;
    إذا (lookup_variations->has (feature_شاخص, &lookups))
    {
      lookup_indices->union_ (**lookups);
      استمر;
    }

    ثابت OT::Feature* f = &(جدول.get_feature (feature_شاخص));
    ثابت OT::Feature **p = تش_اشاره;
    إذا (feature_substitutes_map->has (feature_شاخص, &p))
      f = *p;

    f->جمع_کړه_lookup_indexes_منزل (lookup_indices);
  }

#که_نه_تعریف_شوی HB_NO_VAR
  إذا (catch_all_ریکارډ_feature_idxes)
  {
    لكل (بینښه feature_شاخص : catch_all_ریکارډ_feature_idxes)
    {
      ثابت OT::Feature& f = جدول.get_feature (feature_شاخص);
      f.جمع_کړه_lookup_indexes_منزل (lookup_indices);
      ثابت فراغ *نښان_2 = reinterpret_cast<ثابت فراغ*> (&(جدول.get_feature_list ().get_نښان (feature_شاخص)));
      catch_all_ریکارډ_idx_feature_map.سېټ (feature_شاخص, hb_pair (&f, نښان_2));
    }
  }

  // If all axes are pinned then all feature variations will be dropped so there's no need
  // to collect lookups from them.
  إذا (!plan->all_axes_pinned)
    جدول.feature_variation_collect_lookups (feature_indices,
                                             plan->user_axes_location.is_empty () ? تش_اشاره: feature_ریکارډ_cond_idx_map,
                                             lookup_indices);
#د_که_پای
}


ساكن منطقي
_features_have_equal_params (ثابت OT::Feature &a HB_ناکارول_شوی,
                            ثابت OT::Feature &b HB_ناکارول_شوی,
                            hb_نښان_t نښان_2 HB_ناکارول_شوی)
{
#که_نه_تعریف_شوی HB_NO_LAYOUT_FEATURE_PARAMS
  إذا (منطقي (a.featureParams) != منطقي (b.featureParams)) أرجع خطأ;
  إذا (!a.featureParams) أرجع ریښتیا;
  ثابت OT::FeatureParams &ap = a.get_feature_params ();
  ثابت OT::FeatureParams &bp = b.get_feature_params ();
  إذا (نښان_2 == HB_نښان ('s','i','z','e'))
    أرجع !hb_memcmp (&ap.get_اندازه_params (نښان_2), &bp.get_اندازه_params (نښان_2),
                       OT::Feature_Params_اندازه::static_اندازه);
  إذا ((نښان_2 & 0xFFFF0000u) == HB_نښان ('s','s','\0','\0'))
    أرجع !hb_memcmp (&ap.get_stylistic_سېټ_params (نښان_2), &bp.get_stylistic_سېټ_params (نښان_2),
                       OT::Feature_Params_Stylistic_سېټ::static_اندازه);
  إذا ((نښان_2 & 0xFFFF0000u) == HB_نښان ('c','v','\0','\0'))
  {
    ثابت خپلکاری &ac = ap.get_character_variants_params (نښان_2);
    ثابت خپلکاری &bc = bp.get_character_variants_params (نښان_2);
    أرجع ac.get_اندازه () == bc.get_اندازه () &&
           !hb_memcmp (&ac, &bc, ac.get_اندازه ());
  }
#د_که_پای
  أرجع ریښتیا;
}

ساكن مضمن فراغ
GSUBGPOS_پیدا_کړه_duplicate_features (ثابت OT::GSUBGPOS &g,
				   ثابت hb_map_t *lookup_indices,
				   ثابت hb_سېټ_t *feature_indices,
				   ثابت hb_hashmap_t<بینښه, ثابت OT::Feature*> *feature_substitutes_map,
                                   منطقي all_axes_pinned HB_ناکارول_شوی,
				   hb_map_t *duplicate_feature_map /*بهر*/)
{
  إذا (feature_indices->is_empty ()) أرجع;
#که_نه_تعریف_شوی HB_NO_VAR
  hb_سېټ_t variable_features;
  إذا (!all_axes_pinned)
    g.get_feature_variations ().collect_feature_indices (&variable_features);
  إذا (unlikely (variable_features.دننه_error ())) أرجع;
#د_که_پای
  hb_hashmap_t<hb_نښان_t, hb::unique_ptr<hb_سېټ_t>> unique_features;
  //find out duplicate features after subset
  لكل (بینښه i : feature_indices->iter ())
  {
    hb_نښان_t t = g.get_feature_نښان (i);
    إذا (t == HB_MAP_ارزښت_INVALID) استمر;
    إذا (!unique_features.has (t))
    {
      إذا (unlikely (!unique_features.سېټ (t, hb::unique_ptr<hb_سېټ_t> {hb_سېټ_create ()})))
	أرجع;
      إذا (unique_features.has (t))
	unique_features.get (t)->جمع_کړه (i);
      duplicate_feature_map->سېټ (i, i);
      استمر;
    }

    منطقي وموندل = خطأ;

    hb_سېټ_t* same_نښان_features = unique_features.get (t);
    لكل (بینښه other_f_شاخص : same_نښان_features->iter ())
    {
#که_نه_تعریف_شوی HB_NO_VAR
      /* Matching base lookups do not imply matching alternate behavior. */
      إذا (variable_features.has (i) || variable_features.has (other_f_شاخص) ||
          g.get_feature_variations ().has_lookup_variations (i) ||
	  g.get_feature_variations ().has_lookup_variations (other_f_شاخص))
	استمر;
#د_که_پای

      ثابت OT::Feature* f = &(g.get_feature (i));
      ثابت OT::Feature **p = تش_اشاره;
      إذا (feature_substitutes_map->has (i, &p))
        f = *p;

      ثابت OT::Feature* other_f = &(g.get_feature (other_f_شاخص));
      إذا (feature_substitutes_map->has (other_f_شاخص, &p))
        other_f = *p;

      إذا (!_features_have_equal_params (*f, *other_f, t)) استمر;

      خپلکاری f_iter =
      + hb_iter (f->lookup_شاخص_2)
      | hb_filter (lookup_indices)
      ;

      خپلکاری other_f_iter =
      + hb_iter (other_f->lookup_شاخص_2)
      | hb_filter (lookup_indices)
      ;

      منطقي is_equal = ریښتیا;
      لكل (; f_iter && other_f_iter; f_iter++, other_f_iter++)
      {
	بینښه a = *f_iter;
	بینښه b = *other_f_iter;
	إذا (a != b) { is_equal = خطأ; اكسر; }
      }

      إذا (is_equal == خطأ || f_iter || other_f_iter) استمر;

      وموندل = ریښتیا;
      duplicate_feature_map->سېټ (i, other_f_شاخص);
      اكسر;
    }

    إذا (وموندل == خطأ)
    {
      same_نښان_features->جمع_کړه (i);
      duplicate_feature_map->سېټ (i, i);
    }
  }
}

ساكن فراغ
remap_feature_indices (ثابت hb_سېټ_t &feature_indices,
                       ثابت hb_map_t &duplicate_feature_map,
                       ثابت hb_hashmap_t<بینښه, hb_pair_t<ثابت فراغ*, ثابت فراغ*>>& catch_all_ریکارډ_idx_feature_map,
                       hb_map_t       *mapping, /*بهر*/
                       hb_map_t       *mapping_w_duplicates /*بهر*/)
{
  بینښه i = 0;
  لكل (ثابت خپلکاری _ : feature_indices)
  {
    // retain those features in case we need to insert a catch-all record to reinstate the old features
    إذا (catch_all_ریکارډ_idx_feature_map.has (_))
    {
      mapping->سېټ (_, i);
      mapping_w_duplicates->سېټ (_, i);
      i++;
    }
    وإلا
    {
      uint32_t f_idx = duplicate_feature_map.get (_);
      uint32_t *new_idx;
      إذا (mapping-> has (f_idx, &new_idx))
      {
        mapping_w_duplicates->سېټ (_, *new_idx);
      }
      وإلا
      {
        mapping->سېټ (_, i);
        mapping_w_duplicates->سېټ (_, i);
        i++;
      }
    }
  }
}

کينډۍ <ډول_نوم T>
ساكن فراغ
تړون_glyphs_lookups_features (hb_subset_plan_t   *plan,
				 hb_سېټ_t	     *gids_منزل_retain,
				 hb_map_t	     *lookups,
				 hb_map_t	     *features,
				 hb_map_t	     *features_w_duplicates,
				 script_langsys_map *langsys_map,
				 hb_hashmap_t<بینښه, hb::shared_ptr<hb_سېټ_t>> *feature_ریکارډ_cond_idx_map,
				 hb_hashmap_t<بینښه, ثابت OT::Feature*> *feature_substitutes_map,
                                 hb_hashmap_t<بینښه, hb::shared_ptr<hb_سېټ_t>> *lookup_variations,
                                 hb_سېټ_t &catch_all_ریکارډ_feature_idxes,
                                 hb_hashmap_t<بینښه, hb_pair_t<ثابت فراغ*, ثابت فراغ*>>& catch_all_ریکارډ_idx_feature_map)
{
  hb_blob_ptr_t<T> جدول = plan->سرچینه_جدول<T> ();
  hb_نښان_t جدول_نښان_2 = جدول->جدول_نښان;
  hb_سېټ_t lookup_indices, feature_indices;
  _collect_layout_indices<T> (plan,
                              *جدول,
                              &lookup_indices,
                              &feature_indices,
                              feature_ریکارډ_cond_idx_map,
                              feature_substitutes_map,
                              lookup_variations,
                              catch_all_ریکارډ_feature_idxes,
                              catch_all_ریکارډ_idx_feature_map);

  إذا (جدول_نښان_2 == HB_OT_نښان_GSUB && !(plan->flags & HB_SUBSET_FLAGS_NO_LAYOUT_تړون))
    hb_ot_layout_lookups_substitute_تړون (plan->سرچینه,
                                             &lookup_indices,
					     gids_منزل_retain);
  جدول->تړون_lookups (plan->سرچینه,
			  gids_منزل_retain,
                          &lookup_indices);
  remap_indexes (&lookup_indices, lookups);

  // prune features
  جدول->prune_features (lookups,
                         plan->user_axes_location.is_empty () ? تش_اشاره : feature_ریکارډ_cond_idx_map,
                         feature_substitutes_map,
                         &feature_indices);
  hb_map_t duplicate_feature_map;
  GSUBGPOS_پیدا_کړه_duplicate_features (*جدول, lookups, &feature_indices, feature_substitutes_map,
                                    plan->all_axes_pinned, &duplicate_feature_map);

  feature_indices.clear ();
  جدول->prune_langsys (&duplicate_feature_map, &plan->layout_scripts, langsys_map, &feature_indices);
  remap_feature_indices (feature_indices, duplicate_feature_map, catch_all_ریکارډ_idx_feature_map, features, features_w_duplicates);

#که_نه_تعریف_شوی HB_NO_VAR
  // Populate alternate parameter tags after deduplication. Before this point
  // the map contains only catch-all features and also controls their retention.
  إذا (جدول->get_feature_variations ().ریکارډ_شمېر ())
    لكل (خپلکاری p : *features_w_duplicates)
    {
      ثابت OT::Feature& f = جدول->get_feature (p.لومړی);
      ثابت فراغ *نښان_2 = &(جدول->get_feature_list ().get_نښان (p.لومړی));
      catch_all_ریکارډ_idx_feature_map.سېټ (p.لومړی, hb_pair (&f, نښان_2));
    }
#د_که_پای

  جدول.destroy ();
}

فراغ layout_nameid_تړون (hb_subset_plan_t* plan,
                             hb_سېټ_t* drop_tables)
{
  إذا (!drop_tables->has (HB_OT_نښان_GPOS))
  {
    hb_blob_ptr_t<GPOS> gpos = plan->سرچینه_جدول<GPOS> ();
    gpos->collect_name_ids (&plan->gpos_features, &plan->name_ids,
                            &plan->gpos_feature_substitutes_map,
                            plan->user_axes_location.is_empty () ? تش_اشاره : &plan->gpos_feature_ریکارډ_cond_idx_map,
                            &plan->gpos_old_features,
                            plan->all_axes_pinned);
    gpos.destroy ();
  }
  إذا (!drop_tables->has (HB_OT_نښان_GSUB))
  {
    hb_blob_ptr_t<GSUB> gsub = plan->سرچینه_جدول<GSUB> ();
    gsub->collect_name_ids (&plan->gsub_features, &plan->name_ids,
                            &plan->gsub_feature_substitutes_map,
                            plan->user_axes_location.is_empty () ? تش_اشاره : &plan->gsub_feature_ریکارډ_cond_idx_map,
                            &plan->gsub_old_features,
                            plan->all_axes_pinned);
    gsub.destroy ();
  }
}

فراغ
layout_populate_gids_منزل_retain (hb_subset_plan_t* plan,
		                hb_سېټ_t* drop_tables) {
  إذا (!drop_tables->has (HB_OT_نښان_GSUB))
    // closure all glyphs/lookups/features needed for GSUB substitutions.
    تړون_glyphs_lookups_features<GSUB> (
        plan,
        &plan->_glyphset_gsub,
        &plan->gsub_lookups,
        &plan->gsub_features,
        &plan->gsub_features_w_duplicates,
        &plan->gsub_langsys,
        &plan->gsub_feature_ریکارډ_cond_idx_map,
        &plan->gsub_feature_substitutes_map,
        &plan->gsub_lookup_variations,
        plan->gsub_old_features,
        plan->gsub_old_feature_idx_نښان_map);

  إذا (!drop_tables->has (HB_OT_نښان_GPOS))
    تړون_glyphs_lookups_features<GPOS> (
        plan,
        &plan->_glyphset_gsub,
        &plan->gpos_lookups,
        &plan->gpos_features,
        &plan->gpos_features_w_duplicates,
        &plan->gpos_langsys,
        &plan->gpos_feature_ریکارډ_cond_idx_map,
        &plan->gpos_feature_substitutes_map,
        &plan->gpos_lookup_variations,
        plan->gpos_old_features,
        plan->gpos_old_feature_idx_نښان_map);
}

#که_نه_تعریف_شوی HB_NO_VAR
فراغ
collect_layout_variation_indices (hb_subset_plan_t* plan)
{
  hb_blob_ptr_t<OT::GDEF> gdef = plan->سرچینه_جدول<OT::GDEF> ();
  hb_blob_ptr_t<GSUB> gsub = plan->سرچینه_جدول<GSUB> ();
  hb_blob_ptr_t<GPOS> gpos = plan->سرچینه_جدول<GPOS> ();

  إذا (!gdef->has_معلومات () || !gdef->has_var_store ())
  {
    gdef.destroy ();
    gsub.destroy ();
    gpos.destroy ();
    أرجع;
  }

  hb_سېټ_t varidx_سېټ;
  OT::hb_collect_variation_indices_context_t c (&varidx_سېټ,
                                                &plan->_glyphset_gsub,
                                                &plan->gpos_lookups);
  إذا (plan->normalized_coords && !plan->all_axes_pinned)
    c.condition_ارزښتونه = &plan->layout_condition_ارزښتونه;
  gdef->collect_variation_indices (&c);

  إذا (hb_ot_layout_has_substitution (plan->سرچینه))
    gsub->collect_feature_variation_indices (&c);
  إذا (hb_ot_layout_has_positioning (plan->سرچینه))
  {
    gpos->collect_feature_variation_indices (&c);
    gpos->collect_variation_indices (&c);
  }

#که_نه_تعریف_شوی HB_NO_MATH
  إذا (!plan->drop_tables.has (HB_OT_نښان_MATH))
  {
    hb_blob_ptr_t<OT::MATH> math = plan->سرچینه_جدول<OT::MATH> ();
    خپلکاری *glyph_سېټ = c.glyph_سېټ;
    c.glyph_سېټ = &plan->_glyphset_mathed;
    math->collect_variation_indices (&c, &plan->_glyphset_cmaped);
    c.glyph_سېټ = glyph_سېټ;
    math.destroy ();
  }
#د_که_پای

  remap_variation_indices (gdef->get_var_store (),
                           varidx_سېټ, plan->normalized_coords,
                           !plan->pinned_at_default,
                           plan->all_axes_pinned,
                           plan->layout_variation_idx_delta_map);

  بینښه subtable_شمېر = gdef->get_var_store ().get_sub_جدول_شمېر ();
  generate_varstore_inner_maps (varidx_سېټ, subtable_شمېر, plan->gdef_varstore_inner_maps);
  plan->check_success (!plan->layout_condition_ارزښتونه.دننه_error ());

  gdef.destroy ();
  gsub.destroy ();
  gpos.destroy ();
}
#د_که_پای

#د_که_پای

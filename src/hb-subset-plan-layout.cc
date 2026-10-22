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

استفاده OT::Layout::GSUB;
استفاده OT::Layout::GPOS;

#اگر_تعریف_نشده HB_NO_SUBSET_LAYOUT

مزدوج
hb_subset_plan_t::layout_variation_delta (بیعلامت var_idx HB_استفادهنشده)
{
#اگر_تعریف_نشده HB_NO_VAR
  hb_blob_ptr_t<OT::GDEF> gdef = منبع_جدول<OT::GDEF> ();
  مزدوج delta = gdef->get_var_store ().get_delta<مزدوج> (var_idx,
                                                normalized_coords.as_array ());
  gdef.destroy ();
  أرجع delta;
#وإلا
  أرجع 0.f;
#پایان_اگر
}

فراغ
remap_استفاده_شده_mark_sets (hb_subset_plan_t *plan,
                      hb_map_t& استفاده_شده_mark_sets_map)
{
  hb_blob_ptr_t<OT::GDEF> gdef = plan->منبع_جدول<OT::GDEF> ();

  إذا (!gdef->has_داده () || !gdef->has_mark_glyph_sets ())
  {
    gdef.destroy ();
    أرجع;
  }

  hb_مجموعه_t استفاده_شده_mark_sets;
  gdef->get_mark_glyph_sets ().collect_استفاده_شده_mark_sets (plan->_glyphset_gsub, استفاده_شده_mark_sets);
  gdef.destroy ();

  remap_indexes (&استفاده_شده_mark_sets, &استفاده_شده_mark_sets_map);
}

/*
 * Removes all tags from 'tags' that are not in filter. Additionally eliminates any duplicates.
 * Returns true if anything was removed (not including duplicates).
 */
ساكن منطقي filter_برچسب_list(hb_وکتور_t<hb_برچسب_t>* برچسبها, /* IN/OUT */
                             ثابت hb_مجموعه_t* filter)
{
  hb_وکتور_t<hb_برچسب_t> بیرون;
  بیرون.alloc (برچسبها->get_اندازه() + 1); // +1 is to allocate room for the null terminator.

  منطقي removed = خطأ;
  hb_مجموعه_t دیدهشده;

  لكل (hb_برچسب_t برچسب_2 : *برچسبها)
  {
    إذا (!برچسب_2) استمر;
    إذا (دیدهشده.has (برچسب_2)) استمر;

    إذا (!filter->has (برچسب_2))
    {
      removed = درست;
      استمر;
    }

    دیدهشده.جمع_کن (برچسب_2);
    بیرون.push (برچسب_2);
  }

  // The collect function needs a null element to signal end of the array.
  بیرون.push (HB_برچسب_NONE);

  hb_جابجا (بیرون, *برچسبها);
  أرجع removed;
}

قالب <نام_نوع T>
ساكن منطقي
_collect_instanced_lookup_variations (
    hb_subset_plan_t *plan,
    ثابت T& جدول,
    ثابت hb_مجموعه_t *feature_indices,
    hb_hashmap_t<بیعلامت, hb::shared_ptr<hb_مجموعه_t>> *lookup_variations)
{
  hb_blob_ptr_t<OT::GDEF> gdef = plan->منبع_جدول<OT::GDEF> ();
  OT::قلم_Var_Store_Instancer instancer (&gdef->get_var_store (), اشاره_تهی,
				       plan->normalized_coords.as_array ());
  hb_وکتور_t<بیعلامت> state;
  بیعلامت variations_شاخص;
  منطقي success = جدول.get_feature_variations_state (
	plan->normalized_coords.arrayZ,
	plan->normalized_coords.درازی,
	&variations_شاخص,
	&instancer,
	&state);
  gdef.destroy ();

  إذا (unlikely (!success || state.درون_error ()))
    أرجع plan->check_success (خطأ);

  ثابت خودکار &variations = جدول.get_feature_variations ();
  لكل (ثابت خودکار &رکورد : variations.get_lookup_variation_رکوردها ())
  {
    بیعلامت feature_شاخص = رکورد.get_feature_شاخص ();
    إذا (!feature_indices->has (feature_شاخص)) استمر;

    hb::shared_ptr<hb_مجموعه_t> lookups {hb_مجموعه_create ()};
    رکورد.collect_lookups (&variations,
                           جدول.get_feature_variation (feature_شاخص, variations_شاخص),
                           state.as_sorted_array (), lookups);

    إذا (unlikely (lookups->درون_error () ||
		  !lookup_variations->مجموعه (feature_شاخص, lookups)))
      أرجع plan->check_success (خطأ);
  }

  أرجع درست;
}

قالب <نام_نوع T>
ساكن فراغ _collect_layout_indices (hb_subset_plan_t     *plan,
                                     ثابت T&              جدول,
                                     hb_مجموعه_t		  *lookup_indices, /*بیرون*/
                                     hb_مجموعه_t		  *feature_indices, /*بیرون*/
                                     hb_hashmap_t<بیعلامت, hb::shared_ptr<hb_مجموعه_t>> *feature_رکورد_cond_idx_map, /*بیرون*/
                                     hb_hashmap_t<بیعلامت, ثابت OT::Feature*> *feature_substitutes_map, /*بیرون*/
                                     hb_hashmap_t<بیعلامت, hb::shared_ptr<hb_مجموعه_t>> *lookup_variations, /*بیرون*/
                                     hb_مجموعه_t& catch_all_رکورد_feature_idxes, /*بیرون*/
                                     hb_hashmap_t<بیعلامت, hb_pair_t<ثابت فراغ*, ثابت فراغ*>>& catch_all_رکورد_idx_feature_map /*بیرون*/)
{
  بیعلامت رقم_features = جدول.get_feature_شمار ();
  hb_وکتور_t<hb_برچسب_t> features;
  إذا (!plan->check_success (features.resize (رقم_features))) أرجع;
  جدول.get_feature_برچسبها (0, &رقم_features, features.arrayZ);
  منطقي retain_all_features = !filter_برچسب_list (&features, &plan->layout_features);

  بیعلامت رقم_scripts = جدول.get_script_شمار ();
  hb_وکتور_t<hb_برچسب_t> scripts;
  إذا (!plan->check_success (scripts.resize (رقم_scripts))) أرجع;
  جدول.get_script_برچسبها (0, &رقم_scripts, scripts.arrayZ);
  منطقي retain_all_scripts = !filter_برچسب_list (&scripts, &plan->layout_scripts);

  إذا (!plan->check_success (!features.درون_error ()) || !features
      || !plan->check_success (!scripts.درون_error ()) || !scripts)
    أرجع;

  hb_ot_layout_collect_features (plan->منبع,
                                 T::جدول_برچسب,
                                 retain_all_scripts ? اشاره_تهی : scripts.arrayZ,
                                 اشاره_تهی,
                                 retain_all_features ? اشاره_تهی : features.arrayZ,
                                 feature_indices);

#اگر_تعریف_نشده HB_NO_VAR
  // collect feature substitutes with variations
  إذا (!plan->user_axes_location.is_empty ())
  {
    hb_blob_ptr_t<OT::GDEF> gdef = plan->منبع_جدول<OT::GDEF> ();
    OT::قلم_Var_Store_Instancer instancer (&gdef->get_var_store (), اشاره_تهی,
					 plan->normalized_coords.as_array ());
    hb_hashmap_t<hb::shared_ptr<hb_map_t>, بیعلامت> conditionset_map;
    OT::hb_collect_feature_substitutes_with_var_context_t c =
    {
      &plan->axes_old_شاخص_برچسب_map,
      &plan->axes_location,
      feature_رکورد_cond_idx_map,
      feature_substitutes_map,
      catch_all_رکورد_feature_idxes,
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
#پایان_اگر

#اگر_تعریف_نشده HB_NO_VAR
  إذا (plan->all_axes_pinned &&
      !_collect_instanced_lookup_variations (plan, جدول, feature_indices, lookup_variations))
    أرجع;
#پایان_اگر

  لكل (بیعلامت feature_شاخص : *feature_indices)
  {
    hb::shared_ptr<hb_مجموعه_t> *lookups;
    إذا (lookup_variations->has (feature_شاخص, &lookups))
    {
      lookup_indices->union_ (**lookups);
      استمر;
    }

    ثابت OT::Feature* f = &(جدول.get_feature (feature_شاخص));
    ثابت OT::Feature **p = اشاره_تهی;
    إذا (feature_substitutes_map->has (feature_شاخص, &p))
      f = *p;

    f->جمع_کن_lookup_indexes_مقصد (lookup_indices);
  }

#اگر_تعریف_نشده HB_NO_VAR
  إذا (catch_all_رکورد_feature_idxes)
  {
    لكل (بیعلامت feature_شاخص : catch_all_رکورد_feature_idxes)
    {
      ثابت OT::Feature& f = جدول.get_feature (feature_شاخص);
      f.جمع_کن_lookup_indexes_مقصد (lookup_indices);
      ثابت فراغ *برچسب_2 = reinterpret_cast<ثابت فراغ*> (&(جدول.get_feature_list ().get_برچسب (feature_شاخص)));
      catch_all_رکورد_idx_feature_map.مجموعه (feature_شاخص, hb_pair (&f, برچسب_2));
    }
  }

  // If all axes are pinned then all feature variations will be dropped so there's no need
  // to collect lookups from them.
  إذا (!plan->all_axes_pinned)
    جدول.feature_variation_collect_lookups (feature_indices,
                                             plan->user_axes_location.is_empty () ? اشاره_تهی: feature_رکورد_cond_idx_map,
                                             lookup_indices);
#پایان_اگر
}


ساكن منطقي
_features_have_equal_params (ثابت OT::Feature &a HB_استفادهنشده,
                            ثابت OT::Feature &b HB_استفادهنشده,
                            hb_برچسب_t برچسب_2 HB_استفادهنشده)
{
#اگر_تعریف_نشده HB_NO_LAYOUT_FEATURE_PARAMS
  إذا (منطقي (a.featureParams) != منطقي (b.featureParams)) أرجع خطأ;
  إذا (!a.featureParams) أرجع درست;
  ثابت OT::FeatureParams &ap = a.get_feature_params ();
  ثابت OT::FeatureParams &bp = b.get_feature_params ();
  إذا (برچسب_2 == HB_برچسب ('s','i','z','e'))
    أرجع !hb_memcmp (&ap.get_اندازه_params (برچسب_2), &bp.get_اندازه_params (برچسب_2),
                       OT::Feature_Params_اندازه::static_اندازه);
  إذا ((برچسب_2 & 0xFFFF0000u) == HB_برچسب ('s','s','\0','\0'))
    أرجع !hb_memcmp (&ap.get_stylistic_مجموعه_params (برچسب_2), &bp.get_stylistic_مجموعه_params (برچسب_2),
                       OT::Feature_Params_Stylistic_مجموعه::static_اندازه);
  إذا ((برچسب_2 & 0xFFFF0000u) == HB_برچسب ('c','v','\0','\0'))
  {
    ثابت خودکار &ac = ap.get_character_variants_params (برچسب_2);
    ثابت خودکار &bc = bp.get_character_variants_params (برچسب_2);
    أرجع ac.get_اندازه () == bc.get_اندازه () &&
           !hb_memcmp (&ac, &bc, ac.get_اندازه ());
  }
#پایان_اگر
  أرجع درست;
}

ساكن مضمن فراغ
GSUBGPOS_پیدا_کن_duplicate_features (ثابت OT::GSUBGPOS &g,
				   ثابت hb_map_t *lookup_indices,
				   ثابت hb_مجموعه_t *feature_indices,
				   ثابت hb_hashmap_t<بیعلامت, ثابت OT::Feature*> *feature_substitutes_map,
                                   منطقي all_axes_pinned HB_استفادهنشده,
				   hb_map_t *duplicate_feature_map /*بیرون*/)
{
  إذا (feature_indices->is_empty ()) أرجع;
#اگر_تعریف_نشده HB_NO_VAR
  hb_مجموعه_t variable_features;
  إذا (!all_axes_pinned)
    g.get_feature_variations ().collect_feature_indices (&variable_features);
  إذا (unlikely (variable_features.درون_error ())) أرجع;
#پایان_اگر
  hb_hashmap_t<hb_برچسب_t, hb::unique_ptr<hb_مجموعه_t>> unique_features;
  //find out duplicate features after subset
  لكل (بیعلامت i : feature_indices->iter ())
  {
    hb_برچسب_t t = g.get_feature_برچسب (i);
    إذا (t == HB_MAP_مقدار_INVALID) استمر;
    إذا (!unique_features.has (t))
    {
      إذا (unlikely (!unique_features.مجموعه (t, hb::unique_ptr<hb_مجموعه_t> {hb_مجموعه_create ()})))
	أرجع;
      إذا (unique_features.has (t))
	unique_features.get (t)->جمع_کن (i);
      duplicate_feature_map->مجموعه (i, i);
      استمر;
    }

    منطقي یافت = خطأ;

    hb_مجموعه_t* same_برچسب_features = unique_features.get (t);
    لكل (بیعلامت other_f_شاخص : same_برچسب_features->iter ())
    {
#اگر_تعریف_نشده HB_NO_VAR
      /* Matching base lookups do not imply matching alternate behavior. */
      إذا (variable_features.has (i) || variable_features.has (other_f_شاخص) ||
          g.get_feature_variations ().has_lookup_variations (i) ||
	  g.get_feature_variations ().has_lookup_variations (other_f_شاخص))
	استمر;
#پایان_اگر

      ثابت OT::Feature* f = &(g.get_feature (i));
      ثابت OT::Feature **p = اشاره_تهی;
      إذا (feature_substitutes_map->has (i, &p))
        f = *p;

      ثابت OT::Feature* other_f = &(g.get_feature (other_f_شاخص));
      إذا (feature_substitutes_map->has (other_f_شاخص, &p))
        other_f = *p;

      إذا (!_features_have_equal_params (*f, *other_f, t)) استمر;

      خودکار f_iter =
      + hb_iter (f->lookup_شاخص_2)
      | hb_filter (lookup_indices)
      ;

      خودکار other_f_iter =
      + hb_iter (other_f->lookup_شاخص_2)
      | hb_filter (lookup_indices)
      ;

      منطقي is_equal = درست;
      لكل (; f_iter && other_f_iter; f_iter++, other_f_iter++)
      {
	بیعلامت a = *f_iter;
	بیعلامت b = *other_f_iter;
	إذا (a != b) { is_equal = خطأ; اكسر; }
      }

      إذا (is_equal == خطأ || f_iter || other_f_iter) استمر;

      یافت = درست;
      duplicate_feature_map->مجموعه (i, other_f_شاخص);
      اكسر;
    }

    إذا (یافت == خطأ)
    {
      same_برچسب_features->جمع_کن (i);
      duplicate_feature_map->مجموعه (i, i);
    }
  }
}

ساكن فراغ
remap_feature_indices (ثابت hb_مجموعه_t &feature_indices,
                       ثابت hb_map_t &duplicate_feature_map,
                       ثابت hb_hashmap_t<بیعلامت, hb_pair_t<ثابت فراغ*, ثابت فراغ*>>& catch_all_رکورد_idx_feature_map,
                       hb_map_t       *mapping, /*بیرون*/
                       hb_map_t       *mapping_w_duplicates /*بیرون*/)
{
  بیعلامت i = 0;
  لكل (ثابت خودکار _ : feature_indices)
  {
    // retain those features in case we need to insert a catch-all record to reinstate the old features
    إذا (catch_all_رکورد_idx_feature_map.has (_))
    {
      mapping->مجموعه (_, i);
      mapping_w_duplicates->مجموعه (_, i);
      i++;
    }
    وإلا
    {
      uint32_t f_idx = duplicate_feature_map.get (_);
      uint32_t *new_idx;
      إذا (mapping-> has (f_idx, &new_idx))
      {
        mapping_w_duplicates->مجموعه (_, *new_idx);
      }
      وإلا
      {
        mapping->مجموعه (_, i);
        mapping_w_duplicates->مجموعه (_, i);
        i++;
      }
    }
  }
}

قالب <نام_نوع T>
ساكن فراغ
بستگی_glyphs_lookups_features (hb_subset_plan_t   *plan,
				 hb_مجموعه_t	     *gids_مقصد_retain,
				 hb_map_t	     *lookups,
				 hb_map_t	     *features,
				 hb_map_t	     *features_w_duplicates,
				 script_langsys_map *langsys_map,
				 hb_hashmap_t<بیعلامت, hb::shared_ptr<hb_مجموعه_t>> *feature_رکورد_cond_idx_map,
				 hb_hashmap_t<بیعلامت, ثابت OT::Feature*> *feature_substitutes_map,
                                 hb_hashmap_t<بیعلامت, hb::shared_ptr<hb_مجموعه_t>> *lookup_variations,
                                 hb_مجموعه_t &catch_all_رکورد_feature_idxes,
                                 hb_hashmap_t<بیعلامت, hb_pair_t<ثابت فراغ*, ثابت فراغ*>>& catch_all_رکورد_idx_feature_map)
{
  hb_blob_ptr_t<T> جدول = plan->منبع_جدول<T> ();
  hb_برچسب_t جدول_برچسب_2 = جدول->جدول_برچسب;
  hb_مجموعه_t lookup_indices, feature_indices;
  _collect_layout_indices<T> (plan,
                              *جدول,
                              &lookup_indices,
                              &feature_indices,
                              feature_رکورد_cond_idx_map,
                              feature_substitutes_map,
                              lookup_variations,
                              catch_all_رکورد_feature_idxes,
                              catch_all_رکورد_idx_feature_map);

  إذا (جدول_برچسب_2 == HB_OT_برچسب_GSUB && !(plan->flags & HB_SUBSET_FLAGS_NO_LAYOUT_بستگی))
    hb_ot_layout_lookups_substitute_بستگی (plan->منبع,
                                             &lookup_indices,
					     gids_مقصد_retain);
  جدول->بستگی_lookups (plan->منبع,
			  gids_مقصد_retain,
                          &lookup_indices);
  remap_indexes (&lookup_indices, lookups);

  // prune features
  جدول->prune_features (lookups,
                         plan->user_axes_location.is_empty () ? اشاره_تهی : feature_رکورد_cond_idx_map,
                         feature_substitutes_map,
                         &feature_indices);
  hb_map_t duplicate_feature_map;
  GSUBGPOS_پیدا_کن_duplicate_features (*جدول, lookups, &feature_indices, feature_substitutes_map,
                                    plan->all_axes_pinned, &duplicate_feature_map);

  feature_indices.clear ();
  جدول->prune_langsys (&duplicate_feature_map, &plan->layout_scripts, langsys_map, &feature_indices);
  remap_feature_indices (feature_indices, duplicate_feature_map, catch_all_رکورد_idx_feature_map, features, features_w_duplicates);

#اگر_تعریف_نشده HB_NO_VAR
  // Populate alternate parameter tags after deduplication. Before this point
  // the map contains only catch-all features and also controls their retention.
  إذا (جدول->get_feature_variations ().رکورد_شمار ())
    لكل (خودکار p : *features_w_duplicates)
    {
      ثابت OT::Feature& f = جدول->get_feature (p.اولی);
      ثابت فراغ *برچسب_2 = &(جدول->get_feature_list ().get_برچسب (p.اولی));
      catch_all_رکورد_idx_feature_map.مجموعه (p.اولی, hb_pair (&f, برچسب_2));
    }
#پایان_اگر

  جدول.destroy ();
}

فراغ layout_nameid_بستگی (hb_subset_plan_t* plan,
                             hb_مجموعه_t* drop_tables)
{
  إذا (!drop_tables->has (HB_OT_برچسب_GPOS))
  {
    hb_blob_ptr_t<GPOS> gpos = plan->منبع_جدول<GPOS> ();
    gpos->collect_name_ids (&plan->gpos_features, &plan->name_ids,
                            &plan->gpos_feature_substitutes_map,
                            plan->user_axes_location.is_empty () ? اشاره_تهی : &plan->gpos_feature_رکورد_cond_idx_map,
                            &plan->gpos_old_features,
                            plan->all_axes_pinned);
    gpos.destroy ();
  }
  إذا (!drop_tables->has (HB_OT_برچسب_GSUB))
  {
    hb_blob_ptr_t<GSUB> gsub = plan->منبع_جدول<GSUB> ();
    gsub->collect_name_ids (&plan->gsub_features, &plan->name_ids,
                            &plan->gsub_feature_substitutes_map,
                            plan->user_axes_location.is_empty () ? اشاره_تهی : &plan->gsub_feature_رکورد_cond_idx_map,
                            &plan->gsub_old_features,
                            plan->all_axes_pinned);
    gsub.destroy ();
  }
}

فراغ
layout_populate_gids_مقصد_retain (hb_subset_plan_t* plan,
		                hb_مجموعه_t* drop_tables) {
  إذا (!drop_tables->has (HB_OT_برچسب_GSUB))
    // closure all glyphs/lookups/features needed for GSUB substitutions.
    بستگی_glyphs_lookups_features<GSUB> (
        plan,
        &plan->_glyphset_gsub,
        &plan->gsub_lookups,
        &plan->gsub_features,
        &plan->gsub_features_w_duplicates,
        &plan->gsub_langsys,
        &plan->gsub_feature_رکورد_cond_idx_map,
        &plan->gsub_feature_substitutes_map,
        &plan->gsub_lookup_variations,
        plan->gsub_old_features,
        plan->gsub_old_feature_idx_برچسب_map);

  إذا (!drop_tables->has (HB_OT_برچسب_GPOS))
    بستگی_glyphs_lookups_features<GPOS> (
        plan,
        &plan->_glyphset_gsub,
        &plan->gpos_lookups,
        &plan->gpos_features,
        &plan->gpos_features_w_duplicates,
        &plan->gpos_langsys,
        &plan->gpos_feature_رکورد_cond_idx_map,
        &plan->gpos_feature_substitutes_map,
        &plan->gpos_lookup_variations,
        plan->gpos_old_features,
        plan->gpos_old_feature_idx_برچسب_map);
}

#اگر_تعریف_نشده HB_NO_VAR
فراغ
collect_layout_variation_indices (hb_subset_plan_t* plan)
{
  hb_blob_ptr_t<OT::GDEF> gdef = plan->منبع_جدول<OT::GDEF> ();
  hb_blob_ptr_t<GSUB> gsub = plan->منبع_جدول<GSUB> ();
  hb_blob_ptr_t<GPOS> gpos = plan->منبع_جدول<GPOS> ();

  إذا (!gdef->has_داده () || !gdef->has_var_store ())
  {
    gdef.destroy ();
    gsub.destroy ();
    gpos.destroy ();
    أرجع;
  }

  hb_مجموعه_t varidx_مجموعه;
  OT::hb_collect_variation_indices_context_t c (&varidx_مجموعه,
                                                &plan->_glyphset_gsub,
                                                &plan->gpos_lookups);
  إذا (plan->normalized_coords && !plan->all_axes_pinned)
    c.condition_مقادیر = &plan->layout_condition_مقادیر;
  gdef->collect_variation_indices (&c);

  إذا (hb_ot_layout_has_substitution (plan->منبع))
    gsub->collect_feature_variation_indices (&c);
  إذا (hb_ot_layout_has_positioning (plan->منبع))
  {
    gpos->collect_feature_variation_indices (&c);
    gpos->collect_variation_indices (&c);
  }

#اگر_تعریف_نشده HB_NO_MATH
  إذا (!plan->drop_tables.has (HB_OT_برچسب_MATH))
  {
    hb_blob_ptr_t<OT::MATH> math = plan->منبع_جدول<OT::MATH> ();
    خودکار *glyph_مجموعه = c.glyph_مجموعه;
    c.glyph_مجموعه = &plan->_glyphset_mathed;
    math->collect_variation_indices (&c, &plan->_glyphset_cmaped);
    c.glyph_مجموعه = glyph_مجموعه;
    math.destroy ();
  }
#پایان_اگر

  remap_variation_indices (gdef->get_var_store (),
                           varidx_مجموعه, plan->normalized_coords,
                           !plan->pinned_at_default,
                           plan->all_axes_pinned,
                           plan->layout_variation_idx_delta_map);

  بیعلامت subtable_شمار = gdef->get_var_store ().get_sub_جدول_شمار ();
  generate_varstore_inner_maps (varidx_مجموعه, subtable_شمار, plan->gdef_varstore_inner_maps);
  plan->check_success (!plan->layout_condition_مقادیر.درون_error ());

  gdef.destroy ();
  gsub.destroy ();
  gpos.destroy ();
}
#پایان_اگر

#پایان_اگر

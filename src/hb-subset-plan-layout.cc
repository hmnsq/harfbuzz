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

استعمال OT::Layout::GSUB;
استعمال OT::Layout::GPOS;

#جيڪڏهن_وصف_نه_ٿيل HB_NO_SUBSET_LAYOUT

مزدوج
hb_subset_plan_t::layout_variation_delta (بنانشان var_idx HB_اڻ_استعمال_ٿيل)
{
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
  hb_blob_ptr_t<OT::GDEF> gdef = ذريعو_جدول<OT::GDEF> ();
  مزدوج delta = gdef->get_var_store ().get_delta<مزدوج> (var_idx,
                                                normalized_coords.as_array ());
  gdef.destroy ();
  أرجع delta;
#وإلا
  أرجع 0.f;
#جيڪڏهن_پورو
}

فراغ
remap_استعمال_ٿيل_mark_sets (hb_subset_plan_t *plan,
                      hb_map_t& استعمال_ٿيل_mark_sets_map)
{
  hb_blob_ptr_t<OT::GDEF> gdef = plan->ذريعو_جدول<OT::GDEF> ();

  إذا (!gdef->has_معلومات () || !gdef->has_mark_glyph_sets ())
  {
    gdef.destroy ();
    أرجع;
  }

  hb_سيٽ_t استعمال_ٿيل_mark_sets;
  gdef->get_mark_glyph_sets ().collect_استعمال_ٿيل_mark_sets (plan->_glyphset_gsub, استعمال_ٿيل_mark_sets);
  gdef.destroy ();

  remap_indexes (&استعمال_ٿيل_mark_sets, &استعمال_ٿيل_mark_sets_map);
}

/*
 * Removes all tags from 'tags' that are not in filter. Additionally eliminates any duplicates.
 * Returns true if anything was removed (not including duplicates).
 */
ساكن منطقي filter_ٽيگ_list(hb_ويڪٽر_t<hb_ٽيگ_t>* ٽيگ_فهرست, /* IN/OUT */
                             ثابت hb_سيٽ_t* filter)
{
  hb_ويڪٽر_t<hb_ٽيگ_t> ٻاهريون;
  ٻاهريون.alloc (ٽيگ_فهرست->get_سائيز() + 1); // +1 is to allocate room for the null terminator.

  منطقي removed = خطأ;
  hb_سيٽ_t گهميل;

  لكل (hb_ٽيگ_t ٽيگ_2 : *ٽيگ_فهرست)
  {
    إذا (!ٽيگ_2) استمر;
    إذا (گهميل.has (ٽيگ_2)) استمر;

    إذا (!filter->has (ٽيگ_2))
    {
      removed = سچ;
      استمر;
    }

    گهميل.جوڙيو (ٽيگ_2);
    ٻاهريون.push (ٽيگ_2);
  }

  // The collect function needs a null element to signal end of the array.
  ٻاهريون.push (HB_ٽيگ_NONE);

  hb_مٽا_سٽا (ٻاهريون, *ٽيگ_فهرست);
  أرجع removed;
}

سانچو <قسم_نالو T>
ساكن منطقي
_collect_instanced_lookup_variations (
    hb_subset_plan_t *plan,
    ثابت T& جدول,
    ثابت hb_سيٽ_t *feature_indices,
    hb_hashmap_t<بنانشان, hb::shared_ptr<hb_سيٽ_t>> *lookup_variations)
{
  hb_blob_ptr_t<OT::GDEF> gdef = plan->ذريعو_جدول<OT::GDEF> ();
  OT::شي_Var_Store_Instancer instancer (&gdef->get_var_store (), خالي_اشارو,
				       plan->normalized_coords.as_array ());
  hb_ويڪٽر_t<بنانشان> state;
  بنانشان variations_اشاريو;
  منطقي success = جدول.get_feature_variations_state (
	plan->normalized_coords.arrayZ,
	plan->normalized_coords.ڊيگهائي,
	&variations_اشاريو,
	&instancer,
	&state);
  gdef.destroy ();

  إذا (unlikely (!success || state.اندريون_error ()))
    أرجع plan->check_success (خطأ);

  ثابت خودڪار &variations = جدول.get_feature_variations ();
  لكل (ثابت خودڪار &رڪارڊ : variations.get_lookup_variation_رڪارڊون ())
  {
    بنانشان feature_اشاريو = رڪارڊ.get_feature_اشاريو ();
    إذا (!feature_indices->has (feature_اشاريو)) استمر;

    hb::shared_ptr<hb_سيٽ_t> lookups {hb_سيٽ_create ()};
    رڪارڊ.collect_lookups (&variations,
                           جدول.get_feature_variation (feature_اشاريو, variations_اشاريو),
                           state.as_sorted_array (), lookups);

    إذا (unlikely (lookups->اندريون_error () ||
		  !lookup_variations->سيٽ (feature_اشاريو, lookups)))
      أرجع plan->check_success (خطأ);
  }

  أرجع سچ;
}

سانچو <قسم_نالو T>
ساكن فراغ _collect_layout_indices (hb_subset_plan_t     *plan,
                                     ثابت T&              جدول,
                                     hb_سيٽ_t		  *lookup_indices, /*ٻاهريون*/
                                     hb_سيٽ_t		  *feature_indices, /*ٻاهريون*/
                                     hb_hashmap_t<بنانشان, hb::shared_ptr<hb_سيٽ_t>> *feature_رڪارڊ_cond_idx_map, /*ٻاهريون*/
                                     hb_hashmap_t<بنانشان, ثابت OT::Feature*> *feature_substitutes_map, /*ٻاهريون*/
                                     hb_hashmap_t<بنانشان, hb::shared_ptr<hb_سيٽ_t>> *lookup_variations, /*ٻاهريون*/
                                     hb_سيٽ_t& catch_all_رڪارڊ_feature_idxes, /*ٻاهريون*/
                                     hb_hashmap_t<بنانشان, hb_pair_t<ثابت فراغ*, ثابت فراغ*>>& catch_all_رڪارڊ_idx_feature_map /*ٻاهريون*/)
{
  بنانشان انگ_features = جدول.get_feature_ڳڻپ ();
  hb_ويڪٽر_t<hb_ٽيگ_t> features;
  إذا (!plan->check_success (features.resize (انگ_features))) أرجع;
  جدول.get_feature_ٽيگ_فهرست (0, &انگ_features, features.arrayZ);
  منطقي retain_all_features = !filter_ٽيگ_list (&features, &plan->layout_features);

  بنانشان انگ_scripts = جدول.get_script_ڳڻپ ();
  hb_ويڪٽر_t<hb_ٽيگ_t> scripts;
  إذا (!plan->check_success (scripts.resize (انگ_scripts))) أرجع;
  جدول.get_script_ٽيگ_فهرست (0, &انگ_scripts, scripts.arrayZ);
  منطقي retain_all_scripts = !filter_ٽيگ_list (&scripts, &plan->layout_scripts);

  إذا (!plan->check_success (!features.اندريون_error ()) || !features
      || !plan->check_success (!scripts.اندريون_error ()) || !scripts)
    أرجع;

  hb_ot_layout_collect_features (plan->ذريعو,
                                 T::جدول_ٽيگ,
                                 retain_all_scripts ? خالي_اشارو : scripts.arrayZ,
                                 خالي_اشارو,
                                 retain_all_features ? خالي_اشارو : features.arrayZ,
                                 feature_indices);

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
  // collect feature substitutes with variations
  إذا (!plan->user_axes_location.is_empty ())
  {
    hb_blob_ptr_t<OT::GDEF> gdef = plan->ذريعو_جدول<OT::GDEF> ();
    OT::شي_Var_Store_Instancer instancer (&gdef->get_var_store (), خالي_اشارو,
					 plan->normalized_coords.as_array ());
    hb_hashmap_t<hb::shared_ptr<hb_map_t>, بنانشان> conditionset_map;
    OT::hb_collect_feature_substitutes_with_var_context_t c =
    {
      &plan->axes_old_اشاريو_ٽيگ_map,
      &plan->axes_location,
      feature_رڪارڊ_cond_idx_map,
      feature_substitutes_map,
      catch_all_رڪارڊ_feature_idxes,
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
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
  إذا (plan->all_axes_pinned &&
      !_collect_instanced_lookup_variations (plan, جدول, feature_indices, lookup_variations))
    أرجع;
#جيڪڏهن_پورو

  لكل (بنانشان feature_اشاريو : *feature_indices)
  {
    hb::shared_ptr<hb_سيٽ_t> *lookups;
    إذا (lookup_variations->has (feature_اشاريو, &lookups))
    {
      lookup_indices->union_ (**lookups);
      استمر;
    }

    ثابت OT::Feature* f = &(جدول.get_feature (feature_اشاريو));
    ثابت OT::Feature **p = خالي_اشارو;
    إذا (feature_substitutes_map->has (feature_اشاريو, &p))
      f = *p;

    f->جوڙيو_lookup_indexes_منزل (lookup_indices);
  }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
  إذا (catch_all_رڪارڊ_feature_idxes)
  {
    لكل (بنانشان feature_اشاريو : catch_all_رڪارڊ_feature_idxes)
    {
      ثابت OT::Feature& f = جدول.get_feature (feature_اشاريو);
      f.جوڙيو_lookup_indexes_منزل (lookup_indices);
      ثابت فراغ *ٽيگ_2 = reinterpret_cast<ثابت فراغ*> (&(جدول.get_feature_list ().get_ٽيگ (feature_اشاريو)));
      catch_all_رڪارڊ_idx_feature_map.سيٽ (feature_اشاريو, hb_pair (&f, ٽيگ_2));
    }
  }

  // If all axes are pinned then all feature variations will be dropped so there's no need
  // to collect lookups from them.
  إذا (!plan->all_axes_pinned)
    جدول.feature_variation_collect_lookups (feature_indices,
                                             plan->user_axes_location.is_empty () ? خالي_اشارو: feature_رڪارڊ_cond_idx_map,
                                             lookup_indices);
#جيڪڏهن_پورو
}


ساكن منطقي
_features_have_equal_params (ثابت OT::Feature &a HB_اڻ_استعمال_ٿيل,
                            ثابت OT::Feature &b HB_اڻ_استعمال_ٿيل,
                            hb_ٽيگ_t ٽيگ_2 HB_اڻ_استعمال_ٿيل)
{
#جيڪڏهن_وصف_نه_ٿيل HB_NO_LAYOUT_FEATURE_PARAMS
  إذا (منطقي (a.featureParams) != منطقي (b.featureParams)) أرجع خطأ;
  إذا (!a.featureParams) أرجع سچ;
  ثابت OT::FeatureParams &ap = a.get_feature_params ();
  ثابت OT::FeatureParams &bp = b.get_feature_params ();
  إذا (ٽيگ_2 == HB_ٽيگ ('s','i','z','e'))
    أرجع !hb_memcmp (&ap.get_سائيز_params (ٽيگ_2), &bp.get_سائيز_params (ٽيگ_2),
                       OT::Feature_Params_سائيز::static_سائيز);
  إذا ((ٽيگ_2 & 0xFFFF0000u) == HB_ٽيگ ('s','s','\0','\0'))
    أرجع !hb_memcmp (&ap.get_stylistic_سيٽ_params (ٽيگ_2), &bp.get_stylistic_سيٽ_params (ٽيگ_2),
                       OT::Feature_Params_Stylistic_سيٽ::static_سائيز);
  إذا ((ٽيگ_2 & 0xFFFF0000u) == HB_ٽيگ ('c','v','\0','\0'))
  {
    ثابت خودڪار &ac = ap.get_character_variants_params (ٽيگ_2);
    ثابت خودڪار &bc = bp.get_character_variants_params (ٽيگ_2);
    أرجع ac.get_سائيز () == bc.get_سائيز () &&
           !hb_memcmp (&ac, &bc, ac.get_سائيز ());
  }
#جيڪڏهن_پورو
  أرجع سچ;
}

ساكن مضمن فراغ
GSUBGPOS_ڳوليو_duplicate_features (ثابت OT::GSUBGPOS &g,
				   ثابت hb_map_t *lookup_indices,
				   ثابت hb_سيٽ_t *feature_indices,
				   ثابت hb_hashmap_t<بنانشان, ثابت OT::Feature*> *feature_substitutes_map,
                                   منطقي all_axes_pinned HB_اڻ_استعمال_ٿيل,
				   hb_map_t *duplicate_feature_map /*ٻاهريون*/)
{
  إذا (feature_indices->is_empty ()) أرجع;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
  hb_سيٽ_t variable_features;
  إذا (!all_axes_pinned)
    g.get_feature_variations ().collect_feature_indices (&variable_features);
  إذا (unlikely (variable_features.اندريون_error ())) أرجع;
#جيڪڏهن_پورو
  hb_hashmap_t<hb_ٽيگ_t, hb::unique_ptr<hb_سيٽ_t>> unique_features;
  //find out duplicate features after subset
  لكل (بنانشان i : feature_indices->iter ())
  {
    hb_ٽيگ_t t = g.get_feature_ٽيگ (i);
    إذا (t == HB_MAP_قدر_INVALID) استمر;
    إذا (!unique_features.has (t))
    {
      إذا (unlikely (!unique_features.سيٽ (t, hb::unique_ptr<hb_سيٽ_t> {hb_سيٽ_create ()})))
	أرجع;
      إذا (unique_features.has (t))
	unique_features.get (t)->جوڙيو (i);
      duplicate_feature_map->سيٽ (i, i);
      استمر;
    }

    منطقي مليو = خطأ;

    hb_سيٽ_t* same_ٽيگ_features = unique_features.get (t);
    لكل (بنانشان other_f_اشاريو : same_ٽيگ_features->iter ())
    {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
      /* Matching base lookups do not imply matching alternate behavior. */
      إذا (variable_features.has (i) || variable_features.has (other_f_اشاريو) ||
          g.get_feature_variations ().has_lookup_variations (i) ||
	  g.get_feature_variations ().has_lookup_variations (other_f_اشاريو))
	استمر;
#جيڪڏهن_پورو

      ثابت OT::Feature* f = &(g.get_feature (i));
      ثابت OT::Feature **p = خالي_اشارو;
      إذا (feature_substitutes_map->has (i, &p))
        f = *p;

      ثابت OT::Feature* other_f = &(g.get_feature (other_f_اشاريو));
      إذا (feature_substitutes_map->has (other_f_اشاريو, &p))
        other_f = *p;

      إذا (!_features_have_equal_params (*f, *other_f, t)) استمر;

      خودڪار f_iter =
      + hb_iter (f->lookup_اشاريو_2)
      | hb_filter (lookup_indices)
      ;

      خودڪار other_f_iter =
      + hb_iter (other_f->lookup_اشاريو_2)
      | hb_filter (lookup_indices)
      ;

      منطقي is_equal = سچ;
      لكل (; f_iter && other_f_iter; f_iter++, other_f_iter++)
      {
	بنانشان a = *f_iter;
	بنانشان b = *other_f_iter;
	إذا (a != b) { is_equal = خطأ; اكسر; }
      }

      إذا (is_equal == خطأ || f_iter || other_f_iter) استمر;

      مليو = سچ;
      duplicate_feature_map->سيٽ (i, other_f_اشاريو);
      اكسر;
    }

    إذا (مليو == خطأ)
    {
      same_ٽيگ_features->جوڙيو (i);
      duplicate_feature_map->سيٽ (i, i);
    }
  }
}

ساكن فراغ
remap_feature_indices (ثابت hb_سيٽ_t &feature_indices,
                       ثابت hb_map_t &duplicate_feature_map,
                       ثابت hb_hashmap_t<بنانشان, hb_pair_t<ثابت فراغ*, ثابت فراغ*>>& catch_all_رڪارڊ_idx_feature_map,
                       hb_map_t       *mapping, /*ٻاهريون*/
                       hb_map_t       *mapping_w_duplicates /*ٻاهريون*/)
{
  بنانشان i = 0;
  لكل (ثابت خودڪار _ : feature_indices)
  {
    // retain those features in case we need to insert a catch-all record to reinstate the old features
    إذا (catch_all_رڪارڊ_idx_feature_map.has (_))
    {
      mapping->سيٽ (_, i);
      mapping_w_duplicates->سيٽ (_, i);
      i++;
    }
    وإلا
    {
      uint32_t f_idx = duplicate_feature_map.get (_);
      uint32_t *new_idx;
      إذا (mapping-> has (f_idx, &new_idx))
      {
        mapping_w_duplicates->سيٽ (_, *new_idx);
      }
      وإلا
      {
        mapping->سيٽ (_, i);
        mapping_w_duplicates->سيٽ (_, i);
        i++;
      }
    }
  }
}

سانچو <قسم_نالو T>
ساكن فراغ
گهيرو_glyphs_lookups_features (hb_subset_plan_t   *plan,
				 hb_سيٽ_t	     *gids_منزل_retain,
				 hb_map_t	     *lookups,
				 hb_map_t	     *features,
				 hb_map_t	     *features_w_duplicates,
				 script_langsys_map *langsys_map,
				 hb_hashmap_t<بنانشان, hb::shared_ptr<hb_سيٽ_t>> *feature_رڪارڊ_cond_idx_map,
				 hb_hashmap_t<بنانشان, ثابت OT::Feature*> *feature_substitutes_map,
                                 hb_hashmap_t<بنانشان, hb::shared_ptr<hb_سيٽ_t>> *lookup_variations,
                                 hb_سيٽ_t &catch_all_رڪارڊ_feature_idxes,
                                 hb_hashmap_t<بنانشان, hb_pair_t<ثابت فراغ*, ثابت فراغ*>>& catch_all_رڪارڊ_idx_feature_map)
{
  hb_blob_ptr_t<T> جدول = plan->ذريعو_جدول<T> ();
  hb_ٽيگ_t جدول_ٽيگ_2 = جدول->جدول_ٽيگ;
  hb_سيٽ_t lookup_indices, feature_indices;
  _collect_layout_indices<T> (plan,
                              *جدول,
                              &lookup_indices,
                              &feature_indices,
                              feature_رڪارڊ_cond_idx_map,
                              feature_substitutes_map,
                              lookup_variations,
                              catch_all_رڪارڊ_feature_idxes,
                              catch_all_رڪارڊ_idx_feature_map);

  إذا (جدول_ٽيگ_2 == HB_OT_ٽيگ_GSUB && !(plan->flags & HB_SUBSET_FLAGS_NO_LAYOUT_گهيرو))
    hb_ot_layout_lookups_substitute_گهيرو (plan->ذريعو,
                                             &lookup_indices,
					     gids_منزل_retain);
  جدول->گهيرو_lookups (plan->ذريعو,
			  gids_منزل_retain,
                          &lookup_indices);
  remap_indexes (&lookup_indices, lookups);

  // prune features
  جدول->prune_features (lookups,
                         plan->user_axes_location.is_empty () ? خالي_اشارو : feature_رڪارڊ_cond_idx_map,
                         feature_substitutes_map,
                         &feature_indices);
  hb_map_t duplicate_feature_map;
  GSUBGPOS_ڳوليو_duplicate_features (*جدول, lookups, &feature_indices, feature_substitutes_map,
                                    plan->all_axes_pinned, &duplicate_feature_map);

  feature_indices.clear ();
  جدول->prune_langsys (&duplicate_feature_map, &plan->layout_scripts, langsys_map, &feature_indices);
  remap_feature_indices (feature_indices, duplicate_feature_map, catch_all_رڪارڊ_idx_feature_map, features, features_w_duplicates);

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
  // Populate alternate parameter tags after deduplication. Before this point
  // the map contains only catch-all features and also controls their retention.
  إذا (جدول->get_feature_variations ().رڪارڊ_ڳڻپ ())
    لكل (خودڪار p : *features_w_duplicates)
    {
      ثابت OT::Feature& f = جدول->get_feature (p.پهريون);
      ثابت فراغ *ٽيگ_2 = &(جدول->get_feature_list ().get_ٽيگ (p.پهريون));
      catch_all_رڪارڊ_idx_feature_map.سيٽ (p.پهريون, hb_pair (&f, ٽيگ_2));
    }
#جيڪڏهن_پورو

  جدول.destroy ();
}

فراغ layout_nameid_گهيرو (hb_subset_plan_t* plan,
                             hb_سيٽ_t* drop_tables)
{
  إذا (!drop_tables->has (HB_OT_ٽيگ_GPOS))
  {
    hb_blob_ptr_t<GPOS> gpos = plan->ذريعو_جدول<GPOS> ();
    gpos->collect_name_ids (&plan->gpos_features, &plan->name_ids,
                            &plan->gpos_feature_substitutes_map,
                            plan->user_axes_location.is_empty () ? خالي_اشارو : &plan->gpos_feature_رڪارڊ_cond_idx_map,
                            &plan->gpos_old_features,
                            plan->all_axes_pinned);
    gpos.destroy ();
  }
  إذا (!drop_tables->has (HB_OT_ٽيگ_GSUB))
  {
    hb_blob_ptr_t<GSUB> gsub = plan->ذريعو_جدول<GSUB> ();
    gsub->collect_name_ids (&plan->gsub_features, &plan->name_ids,
                            &plan->gsub_feature_substitutes_map,
                            plan->user_axes_location.is_empty () ? خالي_اشارو : &plan->gsub_feature_رڪارڊ_cond_idx_map,
                            &plan->gsub_old_features,
                            plan->all_axes_pinned);
    gsub.destroy ();
  }
}

فراغ
layout_populate_gids_منزل_retain (hb_subset_plan_t* plan,
		                hb_سيٽ_t* drop_tables) {
  إذا (!drop_tables->has (HB_OT_ٽيگ_GSUB))
    // closure all glyphs/lookups/features needed for GSUB substitutions.
    گهيرو_glyphs_lookups_features<GSUB> (
        plan,
        &plan->_glyphset_gsub,
        &plan->gsub_lookups,
        &plan->gsub_features,
        &plan->gsub_features_w_duplicates,
        &plan->gsub_langsys,
        &plan->gsub_feature_رڪارڊ_cond_idx_map,
        &plan->gsub_feature_substitutes_map,
        &plan->gsub_lookup_variations,
        plan->gsub_old_features,
        plan->gsub_old_feature_idx_ٽيگ_map);

  إذا (!drop_tables->has (HB_OT_ٽيگ_GPOS))
    گهيرو_glyphs_lookups_features<GPOS> (
        plan,
        &plan->_glyphset_gsub,
        &plan->gpos_lookups,
        &plan->gpos_features,
        &plan->gpos_features_w_duplicates,
        &plan->gpos_langsys,
        &plan->gpos_feature_رڪارڊ_cond_idx_map,
        &plan->gpos_feature_substitutes_map,
        &plan->gpos_lookup_variations,
        plan->gpos_old_features,
        plan->gpos_old_feature_idx_ٽيگ_map);
}

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
فراغ
collect_layout_variation_indices (hb_subset_plan_t* plan)
{
  hb_blob_ptr_t<OT::GDEF> gdef = plan->ذريعو_جدول<OT::GDEF> ();
  hb_blob_ptr_t<GSUB> gsub = plan->ذريعو_جدول<GSUB> ();
  hb_blob_ptr_t<GPOS> gpos = plan->ذريعو_جدول<GPOS> ();

  إذا (!gdef->has_معلومات () || !gdef->has_var_store ())
  {
    gdef.destroy ();
    gsub.destroy ();
    gpos.destroy ();
    أرجع;
  }

  hb_سيٽ_t varidx_سيٽ;
  OT::hb_collect_variation_indices_context_t c (&varidx_سيٽ,
                                                &plan->_glyphset_gsub,
                                                &plan->gpos_lookups);
  إذا (plan->normalized_coords && !plan->all_axes_pinned)
    c.condition_قدرون = &plan->layout_condition_قدرون;
  gdef->collect_variation_indices (&c);

  إذا (hb_ot_layout_has_substitution (plan->ذريعو))
    gsub->collect_feature_variation_indices (&c);
  إذا (hb_ot_layout_has_positioning (plan->ذريعو))
  {
    gpos->collect_feature_variation_indices (&c);
    gpos->collect_variation_indices (&c);
  }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_MATH
  إذا (!plan->drop_tables.has (HB_OT_ٽيگ_MATH))
  {
    hb_blob_ptr_t<OT::MATH> math = plan->ذريعو_جدول<OT::MATH> ();
    خودڪار *glyph_سيٽ = c.glyph_سيٽ;
    c.glyph_سيٽ = &plan->_glyphset_mathed;
    math->collect_variation_indices (&c, &plan->_glyphset_cmaped);
    c.glyph_سيٽ = glyph_سيٽ;
    math.destroy ();
  }
#جيڪڏهن_پورو

  remap_variation_indices (gdef->get_var_store (),
                           varidx_سيٽ, plan->normalized_coords,
                           !plan->pinned_at_default,
                           plan->all_axes_pinned,
                           plan->layout_variation_idx_delta_map);

  بنانشان subtable_ڳڻپ = gdef->get_var_store ().get_sub_جدول_ڳڻپ ();
  generate_varstore_inner_maps (varidx_سيٽ, subtable_ڳڻپ, plan->gdef_varstore_inner_maps);
  plan->check_success (!plan->layout_condition_قدرون.اندريون_error ());

  gdef.destroy ();
  gsub.destroy ();
  gpos.destroy ();
}
#جيڪڏهن_پورو

#جيڪڏهن_پورو

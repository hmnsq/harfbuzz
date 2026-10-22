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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset-plan.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-gdef-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-gpos-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-gsub-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-math-table.hh"

ਵਰਤੋਂ OT::Layout::GSUB;
ਵਰਤੋਂ OT::Layout::GPOS;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_SUBSET_LAYOUT

ਦੁੱਗਣਾ
hb_subset_plan_t::layout_variation_delta (ਬਿਨਾਨਿਸ਼ਾਨੀ var_idx HB_ਅਣਵਰਤਿਆ)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
  hb_blob_ptr_t<OT::GDEF> gdef = ਸਰੋਤ_ਸਾਰਣੀ<OT::GDEF> ();
  ਦੁੱਗਣਾ delta = gdef->get_var_store ().get_delta<ਦੁੱਗਣਾ> (var_idx,
                                                normalized_coords.as_array ());
  gdef.destroy ();
  ਵਾਪਸ delta;
#ਨਹੀਂਤਾਂ
  ਵਾਪਸ 0.f;
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ
remap_ਵਰਤਿਆ_mark_sets (hb_subset_plan_t *plan,
                      hb_map_t& ਵਰਤਿਆ_mark_sets_map)
{
  hb_blob_ptr_t<OT::GDEF> gdef = plan->ਸਰੋਤ_ਸਾਰਣੀ<OT::GDEF> ();

  ਜੇ (!gdef->has_ਅੰਕੜੇ () || !gdef->has_mark_glyph_sets ())
  {
    gdef.destroy ();
    ਵਾਪਸ;
  }

  hb_ਸੈੱਟ_t ਵਰਤਿਆ_mark_sets;
  gdef->get_mark_glyph_sets ().collect_ਵਰਤਿਆ_mark_sets (plan->_glyphset_gsub, ਵਰਤਿਆ_mark_sets);
  gdef.destroy ();

  remap_indexes (&ਵਰਤਿਆ_mark_sets, &ਵਰਤਿਆ_mark_sets_map);
}

/*
 * Removes all tags from 'tags' that are not in filter. Additionally eliminates any duplicates.
 * Returns true if anything was removed (not including duplicates).
 */
ਸਥਿਰ ਤਾਰਕਿਕ filter_ਟੈਗ_list(hb_ਸਦਿਸ਼_t<hb_ਟੈਗ_t>* ਟੈਗ_ਸੂਚੀ, /* IN/OUT */
                             ਅਚਲ hb_ਸੈੱਟ_t* filter)
{
  hb_ਸਦਿਸ਼_t<hb_ਟੈਗ_t> ਬਾਹਰਲਾ;
  ਬਾਹਰਲਾ.alloc (ਟੈਗ_ਸੂਚੀ->get_ਮਾਪ() + 1); // +1 is to allocate room for the null terminator.

  ਤਾਰਕਿਕ removed = ਝੂਠ;
  hb_ਸੈੱਟ_t ਦੇਖਿਆ;

  ਲਈ (hb_ਟੈਗ_t ਟੈਗ_2 : *ਟੈਗ_ਸੂਚੀ)
  {
    ਜੇ (!ਟੈਗ_2) ਜਾਰੀ;
    ਜੇ (ਦੇਖਿਆ.has (ਟੈਗ_2)) ਜਾਰੀ;

    ਜੇ (!filter->has (ਟੈਗ_2))
    {
      removed = ਸੱਚ;
      ਜਾਰੀ;
    }

    ਦੇਖਿਆ.ਜੋੜੋ (ਟੈਗ_2);
    ਬਾਹਰਲਾ.push (ਟੈਗ_2);
  }

  // The collect function needs a null element to signal end of the array.
  ਬਾਹਰਲਾ.push (HB_ਟੈਗ_NONE);

  hb_ਅਦਲਾ_ਬਦਲੀ (ਬਾਹਰਲਾ, *ਟੈਗ_ਸੂਚੀ);
  ਵਾਪਸ removed;
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ ਤਾਰਕਿਕ
_collect_instanced_lookup_variations (
    hb_subset_plan_t *plan,
    ਅਚਲ T& ਸਾਰਣੀ,
    ਅਚਲ hb_ਸੈੱਟ_t *feature_indices,
    hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb::shared_ptr<hb_ਸੈੱਟ_t>> *lookup_variations)
{
  hb_blob_ptr_t<OT::GDEF> gdef = plan->ਸਰੋਤ_ਸਾਰਣੀ<OT::GDEF> ();
  OT::ਵਸਤੂ_Var_Store_Instancer instancer (&gdef->get_var_store (), ਖਾਲੀ_ਸੂਚਕ,
				       plan->normalized_coords.as_array ());
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> state;
  ਬਿਨਾਨਿਸ਼ਾਨੀ variations_ਸੂਚਕਾਂਕ;
  ਤਾਰਕਿਕ success = ਸਾਰਣੀ.get_feature_variations_state (
	plan->normalized_coords.arrayZ,
	plan->normalized_coords.ਲੰਮਾਈ,
	&variations_ਸੂਚਕਾਂਕ,
	&instancer,
	&state);
  gdef.destroy ();

  ਜੇ (unlikely (!success || state.ਵਿੱਚ_error ()))
    ਵਾਪਸ plan->check_success (ਝੂਠ);

  ਅਚਲ ਸਵੈਚਾਲਿਤ &variations = ਸਾਰਣੀ.get_feature_variations ();
  ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &ਰਿਕਾਰਡ : variations.get_lookup_variation_ਰਿਕਾਰਡਾਂ ())
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ feature_ਸੂਚਕਾਂਕ = ਰਿਕਾਰਡ.get_feature_ਸੂਚਕਾਂਕ ();
    ਜੇ (!feature_indices->has (feature_ਸੂਚਕਾਂਕ)) ਜਾਰੀ;

    hb::shared_ptr<hb_ਸੈੱਟ_t> lookups {hb_ਸੈੱਟ_create ()};
    ਰਿਕਾਰਡ.collect_lookups (&variations,
                           ਸਾਰਣੀ.get_feature_variation (feature_ਸੂਚਕਾਂਕ, variations_ਸੂਚਕਾਂਕ),
                           state.as_sorted_array (), lookups);

    ਜੇ (unlikely (lookups->ਵਿੱਚ_error () ||
		  !lookup_variations->ਸੈੱਟ (feature_ਸੂਚਕਾਂਕ, lookups)))
      ਵਾਪਸ plan->check_success (ਝੂਠ);
  }

  ਵਾਪਸ ਸੱਚ;
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ ਖਾਲੀ _collect_layout_indices (hb_subset_plan_t     *plan,
                                     ਅਚਲ T&              ਸਾਰਣੀ,
                                     hb_ਸੈੱਟ_t		  *lookup_indices, /*ਬਾਹਰਲਾ*/
                                     hb_ਸੈੱਟ_t		  *feature_indices, /*ਬਾਹਰਲਾ*/
                                     hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb::shared_ptr<hb_ਸੈੱਟ_t>> *feature_ਰਿਕਾਰਡ_cond_idx_map, /*ਬਾਹਰਲਾ*/
                                     hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਅਚਲ OT::Feature*> *feature_substitutes_map, /*ਬਾਹਰਲਾ*/
                                     hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb::shared_ptr<hb_ਸੈੱਟ_t>> *lookup_variations, /*ਬਾਹਰਲਾ*/
                                     hb_ਸੈੱਟ_t& catch_all_ਰਿਕਾਰਡ_feature_idxes, /*ਬਾਹਰਲਾ*/
                                     hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_pair_t<ਅਚਲ ਖਾਲੀ*, ਅਚਲ ਖਾਲੀ*>>& catch_all_ਰਿਕਾਰਡ_idx_feature_map /*ਬਾਹਰਲਾ*/)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_features = ਸਾਰਣੀ.get_feature_ਗਿਣਤੀ ();
  hb_ਸਦਿਸ਼_t<hb_ਟੈਗ_t> features;
  ਜੇ (!plan->check_success (features.resize (ਅੰਕ_features))) ਵਾਪਸ;
  ਸਾਰਣੀ.get_feature_ਟੈਗ_ਸੂਚੀ (0, &ਅੰਕ_features, features.arrayZ);
  ਤਾਰਕਿਕ retain_all_features = !filter_ਟੈਗ_list (&features, &plan->layout_features);

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_scripts = ਸਾਰਣੀ.get_script_ਗਿਣਤੀ ();
  hb_ਸਦਿਸ਼_t<hb_ਟੈਗ_t> scripts;
  ਜੇ (!plan->check_success (scripts.resize (ਅੰਕ_scripts))) ਵਾਪਸ;
  ਸਾਰਣੀ.get_script_ਟੈਗ_ਸੂਚੀ (0, &ਅੰਕ_scripts, scripts.arrayZ);
  ਤਾਰਕਿਕ retain_all_scripts = !filter_ਟੈਗ_list (&scripts, &plan->layout_scripts);

  ਜੇ (!plan->check_success (!features.ਵਿੱਚ_error ()) || !features
      || !plan->check_success (!scripts.ਵਿੱਚ_error ()) || !scripts)
    ਵਾਪਸ;

  hb_ot_layout_collect_features (plan->ਸਰੋਤ,
                                 T::ਸਾਰਣੀ_ਟੈਗ,
                                 retain_all_scripts ? ਖਾਲੀ_ਸੂਚਕ : scripts.arrayZ,
                                 ਖਾਲੀ_ਸੂਚਕ,
                                 retain_all_features ? ਖਾਲੀ_ਸੂਚਕ : features.arrayZ,
                                 feature_indices);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
  // collect feature substitutes with variations
  ਜੇ (!plan->user_axes_location.is_empty ())
  {
    hb_blob_ptr_t<OT::GDEF> gdef = plan->ਸਰੋਤ_ਸਾਰਣੀ<OT::GDEF> ();
    OT::ਵਸਤੂ_Var_Store_Instancer instancer (&gdef->get_var_store (), ਖਾਲੀ_ਸੂਚਕ,
					 plan->normalized_coords.as_array ());
    hb_hashmap_t<hb::shared_ptr<hb_map_t>, ਬਿਨਾਨਿਸ਼ਾਨੀ> conditionset_map;
    OT::hb_collect_feature_substitutes_with_var_context_t c =
    {
      &plan->axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map,
      &plan->axes_location,
      feature_ਰਿਕਾਰਡ_cond_idx_map,
      feature_substitutes_map,
      catch_all_ਰਿਕਾਰਡ_feature_idxes,
      feature_indices,
      ਝੂਠ,
      ਝੂਠ,
      ਝੂਠ,
      0,
      &conditionset_map,
      &instancer,
      plan->all_axes_pinned
    };
    ਸਾਰਣੀ.collect_feature_substitutes_with_variations (&c);
    gdef.destroy ();
  }
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
  ਜੇ (plan->all_axes_pinned &&
      !_collect_instanced_lookup_variations (plan, ਸਾਰਣੀ, feature_indices, lookup_variations))
    ਵਾਪਸ;
#ਜੇ_ਅੰਤ

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ feature_ਸੂਚਕਾਂਕ : *feature_indices)
  {
    hb::shared_ptr<hb_ਸੈੱਟ_t> *lookups;
    ਜੇ (lookup_variations->has (feature_ਸੂਚਕਾਂਕ, &lookups))
    {
      lookup_indices->union_ (**lookups);
      ਜਾਰੀ;
    }

    ਅਚਲ OT::Feature* f = &(ਸਾਰਣੀ.get_feature (feature_ਸੂਚਕਾਂਕ));
    ਅਚਲ OT::Feature **p = ਖਾਲੀ_ਸੂਚਕ;
    ਜੇ (feature_substitutes_map->has (feature_ਸੂਚਕਾਂਕ, &p))
      f = *p;

    f->ਜੋੜੋ_lookup_indexes_ਮੰਜ਼ਿਲ (lookup_indices);
  }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
  ਜੇ (catch_all_ਰਿਕਾਰਡ_feature_idxes)
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ feature_ਸੂਚਕਾਂਕ : catch_all_ਰਿਕਾਰਡ_feature_idxes)
    {
      ਅਚਲ OT::Feature& f = ਸਾਰਣੀ.get_feature (feature_ਸੂਚਕਾਂਕ);
      f.ਜੋੜੋ_lookup_indexes_ਮੰਜ਼ਿਲ (lookup_indices);
      ਅਚਲ ਖਾਲੀ *ਟੈਗ_2 = reinterpret_cast<ਅਚਲ ਖਾਲੀ*> (&(ਸਾਰਣੀ.get_feature_list ().get_ਟੈਗ (feature_ਸੂਚਕਾਂਕ)));
      catch_all_ਰਿਕਾਰਡ_idx_feature_map.ਸੈੱਟ (feature_ਸੂਚਕਾਂਕ, hb_pair (&f, ਟੈਗ_2));
    }
  }

  // If all axes are pinned then all feature variations will be dropped so there's no need
  // to collect lookups from them.
  ਜੇ (!plan->all_axes_pinned)
    ਸਾਰਣੀ.feature_variation_collect_lookups (feature_indices,
                                             plan->user_axes_location.is_empty () ? ਖਾਲੀ_ਸੂਚਕ: feature_ਰਿਕਾਰਡ_cond_idx_map,
                                             lookup_indices);
#ਜੇ_ਅੰਤ
}


ਸਥਿਰ ਤਾਰਕਿਕ
_features_have_equal_params (ਅਚਲ OT::Feature &a HB_ਅਣਵਰਤਿਆ,
                            ਅਚਲ OT::Feature &b HB_ਅਣਵਰਤਿਆ,
                            hb_ਟੈਗ_t ਟੈਗ_2 HB_ਅਣਵਰਤਿਆ)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_LAYOUT_FEATURE_PARAMS
  ਜੇ (ਤਾਰਕਿਕ (a.featureParams) != ਤਾਰਕਿਕ (b.featureParams)) ਵਾਪਸ ਝੂਠ;
  ਜੇ (!a.featureParams) ਵਾਪਸ ਸੱਚ;
  ਅਚਲ OT::FeatureParams &ap = a.get_feature_params ();
  ਅਚਲ OT::FeatureParams &bp = b.get_feature_params ();
  ਜੇ (ਟੈਗ_2 == HB_ਟੈਗ ('s','i','z','e'))
    ਵਾਪਸ !hb_memcmp (&ap.get_ਮਾਪ_params (ਟੈਗ_2), &bp.get_ਮਾਪ_params (ਟੈਗ_2),
                       OT::Feature_Params_ਮਾਪ::static_ਮਾਪ);
  ਜੇ ((ਟੈਗ_2 & 0xFFFF0000u) == HB_ਟੈਗ ('s','s','\0','\0'))
    ਵਾਪਸ !hb_memcmp (&ap.get_stylistic_ਸੈੱਟ_params (ਟੈਗ_2), &bp.get_stylistic_ਸੈੱਟ_params (ਟੈਗ_2),
                       OT::Feature_Params_Stylistic_ਸੈੱਟ::static_ਮਾਪ);
  ਜੇ ((ਟੈਗ_2 & 0xFFFF0000u) == HB_ਟੈਗ ('c','v','\0','\0'))
  {
    ਅਚਲ ਸਵੈਚਾਲਿਤ &ac = ap.get_character_variants_params (ਟੈਗ_2);
    ਅਚਲ ਸਵੈਚਾਲਿਤ &bc = bp.get_character_variants_params (ਟੈਗ_2);
    ਵਾਪਸ ac.get_ਮਾਪ () == bc.get_ਮਾਪ () &&
           !hb_memcmp (&ac, &bc, ac.get_ਮਾਪ ());
  }
#ਜੇ_ਅੰਤ
  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
GSUBGPOS_ਲੱਭੋ_duplicate_features (ਅਚਲ OT::GSUBGPOS &g,
				   ਅਚਲ hb_map_t *lookup_indices,
				   ਅਚਲ hb_ਸੈੱਟ_t *feature_indices,
				   ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਅਚਲ OT::Feature*> *feature_substitutes_map,
                                   ਤਾਰਕਿਕ all_axes_pinned HB_ਅਣਵਰਤਿਆ,
				   hb_map_t *duplicate_feature_map /*ਬਾਹਰਲਾ*/)
{
  ਜੇ (feature_indices->is_empty ()) ਵਾਪਸ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
  hb_ਸੈੱਟ_t variable_features;
  ਜੇ (!all_axes_pinned)
    g.get_feature_variations ().collect_feature_indices (&variable_features);
  ਜੇ (unlikely (variable_features.ਵਿੱਚ_error ())) ਵਾਪਸ;
#ਜੇ_ਅੰਤ
  hb_hashmap_t<hb_ਟੈਗ_t, hb::unique_ptr<hb_ਸੈੱਟ_t>> unique_features;
  //find out duplicate features after subset
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i : feature_indices->iter ())
  {
    hb_ਟੈਗ_t t = g.get_feature_ਟੈਗ (i);
    ਜੇ (t == HB_MAP_ਮੁੱਲ_INVALID) ਜਾਰੀ;
    ਜੇ (!unique_features.has (t))
    {
      ਜੇ (unlikely (!unique_features.ਸੈੱਟ (t, hb::unique_ptr<hb_ਸੈੱਟ_t> {hb_ਸੈੱਟ_create ()})))
	ਵਾਪਸ;
      ਜੇ (unique_features.has (t))
	unique_features.get (t)->ਜੋੜੋ (i);
      duplicate_feature_map->ਸੈੱਟ (i, i);
      ਜਾਰੀ;
    }

    ਤਾਰਕਿਕ ਮਿਲਿਆ = ਝੂਠ;

    hb_ਸੈੱਟ_t* same_ਟੈਗ_features = unique_features.get (t);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ other_f_ਸੂਚਕਾਂਕ : same_ਟੈਗ_features->iter ())
    {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
      /* Matching base lookups do not imply matching alternate behavior. */
      ਜੇ (variable_features.has (i) || variable_features.has (other_f_ਸੂਚਕਾਂਕ) ||
          g.get_feature_variations ().has_lookup_variations (i) ||
	  g.get_feature_variations ().has_lookup_variations (other_f_ਸੂਚਕਾਂਕ))
	ਜਾਰੀ;
#ਜੇ_ਅੰਤ

      ਅਚਲ OT::Feature* f = &(g.get_feature (i));
      ਅਚਲ OT::Feature **p = ਖਾਲੀ_ਸੂਚਕ;
      ਜੇ (feature_substitutes_map->has (i, &p))
        f = *p;

      ਅਚਲ OT::Feature* other_f = &(g.get_feature (other_f_ਸੂਚਕਾਂਕ));
      ਜੇ (feature_substitutes_map->has (other_f_ਸੂਚਕਾਂਕ, &p))
        other_f = *p;

      ਜੇ (!_features_have_equal_params (*f, *other_f, t)) ਜਾਰੀ;

      ਸਵੈਚਾਲਿਤ f_iter =
      + hb_iter (f->lookup_ਸੂਚਕਾਂਕ_2)
      | hb_filter (lookup_indices)
      ;

      ਸਵੈਚਾਲਿਤ other_f_iter =
      + hb_iter (other_f->lookup_ਸੂਚਕਾਂਕ_2)
      | hb_filter (lookup_indices)
      ;

      ਤਾਰਕਿਕ is_equal = ਸੱਚ;
      ਲਈ (; f_iter && other_f_iter; f_iter++, other_f_iter++)
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ a = *f_iter;
	ਬਿਨਾਨਿਸ਼ਾਨੀ b = *other_f_iter;
	ਜੇ (a != b) { is_equal = ਝੂਠ; ਰੋਕੋ; }
      }

      ਜੇ (is_equal == ਝੂਠ || f_iter || other_f_iter) ਜਾਰੀ;

      ਮਿਲਿਆ = ਸੱਚ;
      duplicate_feature_map->ਸੈੱਟ (i, other_f_ਸੂਚਕਾਂਕ);
      ਰੋਕੋ;
    }

    ਜੇ (ਮਿਲਿਆ == ਝੂਠ)
    {
      same_ਟੈਗ_features->ਜੋੜੋ (i);
      duplicate_feature_map->ਸੈੱਟ (i, i);
    }
  }
}

ਸਥਿਰ ਖਾਲੀ
remap_feature_indices (ਅਚਲ hb_ਸੈੱਟ_t &feature_indices,
                       ਅਚਲ hb_map_t &duplicate_feature_map,
                       ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_pair_t<ਅਚਲ ਖਾਲੀ*, ਅਚਲ ਖਾਲੀ*>>& catch_all_ਰਿਕਾਰਡ_idx_feature_map,
                       hb_map_t       *mapping, /*ਬਾਹਰਲਾ*/
                       hb_map_t       *mapping_w_duplicates /*ਬਾਹਰਲਾ*/)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0;
  ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ _ : feature_indices)
  {
    // retain those features in case we need to insert a catch-all record to reinstate the old features
    ਜੇ (catch_all_ਰਿਕਾਰਡ_idx_feature_map.has (_))
    {
      mapping->ਸੈੱਟ (_, i);
      mapping_w_duplicates->ਸੈੱਟ (_, i);
      i++;
    }
    ਨਹੀਂਤਾਂ
    {
      uint32_t f_idx = duplicate_feature_map.get (_);
      uint32_t *new_idx;
      ਜੇ (mapping-> has (f_idx, &new_idx))
      {
        mapping_w_duplicates->ਸੈੱਟ (_, *new_idx);
      }
      ਨਹੀਂਤਾਂ
      {
        mapping->ਸੈੱਟ (_, i);
        mapping_w_duplicates->ਸੈੱਟ (_, i);
        i++;
      }
    }
  }
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ ਖਾਲੀ
ਘੇਰਾ_glyphs_lookups_features (hb_subset_plan_t   *plan,
				 hb_ਸੈੱਟ_t	     *gids_ਮੰਜ਼ਿਲ_retain,
				 hb_map_t	     *lookups,
				 hb_map_t	     *features,
				 hb_map_t	     *features_w_duplicates,
				 script_langsys_map *langsys_map,
				 hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb::shared_ptr<hb_ਸੈੱਟ_t>> *feature_ਰਿਕਾਰਡ_cond_idx_map,
				 hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਅਚਲ OT::Feature*> *feature_substitutes_map,
                                 hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb::shared_ptr<hb_ਸੈੱਟ_t>> *lookup_variations,
                                 hb_ਸੈੱਟ_t &catch_all_ਰਿਕਾਰਡ_feature_idxes,
                                 hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_pair_t<ਅਚਲ ਖਾਲੀ*, ਅਚਲ ਖਾਲੀ*>>& catch_all_ਰਿਕਾਰਡ_idx_feature_map)
{
  hb_blob_ptr_t<T> ਸਾਰਣੀ = plan->ਸਰੋਤ_ਸਾਰਣੀ<T> ();
  hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ_2 = ਸਾਰਣੀ->ਸਾਰਣੀ_ਟੈਗ;
  hb_ਸੈੱਟ_t lookup_indices, feature_indices;
  _collect_layout_indices<T> (plan,
                              *ਸਾਰਣੀ,
                              &lookup_indices,
                              &feature_indices,
                              feature_ਰਿਕਾਰਡ_cond_idx_map,
                              feature_substitutes_map,
                              lookup_variations,
                              catch_all_ਰਿਕਾਰਡ_feature_idxes,
                              catch_all_ਰਿਕਾਰਡ_idx_feature_map);

  ਜੇ (ਸਾਰਣੀ_ਟੈਗ_2 == HB_OT_ਟੈਗ_GSUB && !(plan->flags & HB_SUBSET_FLAGS_NO_LAYOUT_ਘੇਰਾ))
    hb_ot_layout_lookups_substitute_ਘੇਰਾ (plan->ਸਰੋਤ,
                                             &lookup_indices,
					     gids_ਮੰਜ਼ਿਲ_retain);
  ਸਾਰਣੀ->ਘੇਰਾ_lookups (plan->ਸਰੋਤ,
			  gids_ਮੰਜ਼ਿਲ_retain,
                          &lookup_indices);
  remap_indexes (&lookup_indices, lookups);

  // prune features
  ਸਾਰਣੀ->prune_features (lookups,
                         plan->user_axes_location.is_empty () ? ਖਾਲੀ_ਸੂਚਕ : feature_ਰਿਕਾਰਡ_cond_idx_map,
                         feature_substitutes_map,
                         &feature_indices);
  hb_map_t duplicate_feature_map;
  GSUBGPOS_ਲੱਭੋ_duplicate_features (*ਸਾਰਣੀ, lookups, &feature_indices, feature_substitutes_map,
                                    plan->all_axes_pinned, &duplicate_feature_map);

  feature_indices.clear ();
  ਸਾਰਣੀ->prune_langsys (&duplicate_feature_map, &plan->layout_scripts, langsys_map, &feature_indices);
  remap_feature_indices (feature_indices, duplicate_feature_map, catch_all_ਰਿਕਾਰਡ_idx_feature_map, features, features_w_duplicates);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
  // Populate alternate parameter tags after deduplication. Before this point
  // the map contains only catch-all features and also controls their retention.
  ਜੇ (ਸਾਰਣੀ->get_feature_variations ().ਰਿਕਾਰਡ_ਗਿਣਤੀ ())
    ਲਈ (ਸਵੈਚਾਲਿਤ p : *features_w_duplicates)
    {
      ਅਚਲ OT::Feature& f = ਸਾਰਣੀ->get_feature (p.ਪਹਿਲਾ);
      ਅਚਲ ਖਾਲੀ *ਟੈਗ_2 = &(ਸਾਰਣੀ->get_feature_list ().get_ਟੈਗ (p.ਪਹਿਲਾ));
      catch_all_ਰਿਕਾਰਡ_idx_feature_map.ਸੈੱਟ (p.ਪਹਿਲਾ, hb_pair (&f, ਟੈਗ_2));
    }
#ਜੇ_ਅੰਤ

  ਸਾਰਣੀ.destroy ();
}

ਖਾਲੀ layout_nameid_ਘੇਰਾ (hb_subset_plan_t* plan,
                             hb_ਸੈੱਟ_t* drop_tables)
{
  ਜੇ (!drop_tables->has (HB_OT_ਟੈਗ_GPOS))
  {
    hb_blob_ptr_t<GPOS> gpos = plan->ਸਰੋਤ_ਸਾਰਣੀ<GPOS> ();
    gpos->collect_name_ids (&plan->gpos_features, &plan->name_ids,
                            &plan->gpos_feature_substitutes_map,
                            plan->user_axes_location.is_empty () ? ਖਾਲੀ_ਸੂਚਕ : &plan->gpos_feature_ਰਿਕਾਰਡ_cond_idx_map,
                            &plan->gpos_old_features,
                            plan->all_axes_pinned);
    gpos.destroy ();
  }
  ਜੇ (!drop_tables->has (HB_OT_ਟੈਗ_GSUB))
  {
    hb_blob_ptr_t<GSUB> gsub = plan->ਸਰੋਤ_ਸਾਰਣੀ<GSUB> ();
    gsub->collect_name_ids (&plan->gsub_features, &plan->name_ids,
                            &plan->gsub_feature_substitutes_map,
                            plan->user_axes_location.is_empty () ? ਖਾਲੀ_ਸੂਚਕ : &plan->gsub_feature_ਰਿਕਾਰਡ_cond_idx_map,
                            &plan->gsub_old_features,
                            plan->all_axes_pinned);
    gsub.destroy ();
  }
}

ਖਾਲੀ
layout_populate_gids_ਮੰਜ਼ਿਲ_retain (hb_subset_plan_t* plan,
		                hb_ਸੈੱਟ_t* drop_tables) {
  ਜੇ (!drop_tables->has (HB_OT_ਟੈਗ_GSUB))
    // closure all glyphs/lookups/features needed for GSUB substitutions.
    ਘੇਰਾ_glyphs_lookups_features<GSUB> (
        plan,
        &plan->_glyphset_gsub,
        &plan->gsub_lookups,
        &plan->gsub_features,
        &plan->gsub_features_w_duplicates,
        &plan->gsub_langsys,
        &plan->gsub_feature_ਰਿਕਾਰਡ_cond_idx_map,
        &plan->gsub_feature_substitutes_map,
        &plan->gsub_lookup_variations,
        plan->gsub_old_features,
        plan->gsub_old_feature_idx_ਟੈਗ_map);

  ਜੇ (!drop_tables->has (HB_OT_ਟੈਗ_GPOS))
    ਘੇਰਾ_glyphs_lookups_features<GPOS> (
        plan,
        &plan->_glyphset_gsub,
        &plan->gpos_lookups,
        &plan->gpos_features,
        &plan->gpos_features_w_duplicates,
        &plan->gpos_langsys,
        &plan->gpos_feature_ਰਿਕਾਰਡ_cond_idx_map,
        &plan->gpos_feature_substitutes_map,
        &plan->gpos_lookup_variations,
        plan->gpos_old_features,
        plan->gpos_old_feature_idx_ਟੈਗ_map);
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
ਖਾਲੀ
collect_layout_variation_indices (hb_subset_plan_t* plan)
{
  hb_blob_ptr_t<OT::GDEF> gdef = plan->ਸਰੋਤ_ਸਾਰਣੀ<OT::GDEF> ();
  hb_blob_ptr_t<GSUB> gsub = plan->ਸਰੋਤ_ਸਾਰਣੀ<GSUB> ();
  hb_blob_ptr_t<GPOS> gpos = plan->ਸਰੋਤ_ਸਾਰਣੀ<GPOS> ();

  ਜੇ (!gdef->has_ਅੰਕੜੇ () || !gdef->has_var_store ())
  {
    gdef.destroy ();
    gsub.destroy ();
    gpos.destroy ();
    ਵਾਪਸ;
  }

  hb_ਸੈੱਟ_t varidx_ਸੈੱਟ;
  OT::hb_collect_variation_indices_context_t c (&varidx_ਸੈੱਟ,
                                                &plan->_glyphset_gsub,
                                                &plan->gpos_lookups);
  ਜੇ (plan->normalized_coords && !plan->all_axes_pinned)
    c.condition_ਮੁੱਲਾਂ = &plan->layout_condition_ਮੁੱਲਾਂ;
  gdef->collect_variation_indices (&c);

  ਜੇ (hb_ot_layout_has_substitution (plan->ਸਰੋਤ))
    gsub->collect_feature_variation_indices (&c);
  ਜੇ (hb_ot_layout_has_positioning (plan->ਸਰੋਤ))
  {
    gpos->collect_feature_variation_indices (&c);
    gpos->collect_variation_indices (&c);
  }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_MATH
  ਜੇ (!plan->drop_tables.has (HB_OT_ਟੈਗ_MATH))
  {
    hb_blob_ptr_t<OT::MATH> math = plan->ਸਰੋਤ_ਸਾਰਣੀ<OT::MATH> ();
    ਸਵੈਚਾਲਿਤ *glyph_ਸੈੱਟ = c.glyph_ਸੈੱਟ;
    c.glyph_ਸੈੱਟ = &plan->_glyphset_mathed;
    math->collect_variation_indices (&c, &plan->_glyphset_cmaped);
    c.glyph_ਸੈੱਟ = glyph_ਸੈੱਟ;
    math.destroy ();
  }
#ਜੇ_ਅੰਤ

  remap_variation_indices (gdef->get_var_store (),
                           varidx_ਸੈੱਟ, plan->normalized_coords,
                           !plan->pinned_at_default,
                           plan->all_axes_pinned,
                           plan->layout_variation_idx_delta_map);

  ਬਿਨਾਨਿਸ਼ਾਨੀ subtable_ਗਿਣਤੀ = gdef->get_var_store ().get_sub_ਸਾਰਣੀ_ਗਿਣਤੀ ();
  generate_varstore_inner_maps (varidx_ਸੈੱਟ, subtable_ਗਿਣਤੀ, plan->gdef_varstore_inner_maps);
  plan->check_success (!plan->layout_condition_ਮੁੱਲਾਂ.ਵਿੱਚ_error ());

  gdef.destroy ();
  gsub.destroy ();
  gpos.destroy ();
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ

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

 #अन्तर्भाव "hb-ot-layout-common.hh"
#अन्तर्भाव "hb-subset-plan.hh"

 #अन्तर्भाव "hb-ot-var-common.hh"
 #अन्तर्भाव "hb-ot-layout-base-table.hh"
 #अन्तर्भाव "hb-ot-glyf-table.hh"
 #अन्तर्भाव "hb-ot-var-fvar-table.hh"
 #अन्तर्भाव "hb-ot-var-avar-table.hh"
 #अन्तर्भाव "hb-ot-cff2-table.hh"

 #ifndef HB_NO_VAR

 शून्य
 generate_varstore_inner_maps (स्थिरांक hb_set_t& varidx_set,
                               अचिन्हित subtable_मोजणी,
                               hb_vector_t<hb_inc_bimap_t> &inner_maps /* OUT */)
 {
   जर (varidx_set.is_empty () || subtable_मोजणी == 0) परत;

   जर (unlikely (!inner_maps.resize (subtable_मोजणी))) परत;
   साठी (अचिन्हित idx : varidx_set)
   {
     uint16_t major = idx >> 16;
     uint16_t minor = idx & 0xFFFF;

     जर (major >= subtable_मोजणी)
       सुरूठेवा;
     inner_maps[major].add (minor);
   }
 }

 /* Evaluate a variation-region tent at v, mirroring VarRegionAxis::evaluate
  * (invalid regions evaluate as constant 1). */
 स्थिर द्विगुण
 _tent_eval (स्थिरांक Triple &tent, द्विगुण v)
 {
   द्विगुण start = (द्विगुण) tent.minimum;
   द्विगुण peak = (द्विगुण) tent.मधला;
   द्विगुण end = (द्विगुण) tent.maximum;
   जर (unlikely (start > peak || peak > end)) परत 1.0;
   जर (unlikely (start < 0.0 && end > 0.0 && peak != 0.0)) परत 1.0;
   जर (peak == 0.0 || v == peak) परत 1.0;
   जर (v <= start || end <= v) परत 0.0;
   जर (v < peak) परत (v - start) / (peak - start);
   परत (end - v) / (end - peak);
 }

 /* Range of a tent's scalar over an input interval [lo, hi]. The tent is
  * unimodal, so the extremes are at the interval endpoints, plus the peak
  * when it falls inside. */
 स्थिर शून्य
 tent_मूल्य_range (स्थिरांक Triple &tent, द्विगुण lo, द्विगुण hi,
                    द्विगुण *tmin, द्विगुण *tmax)
 {
   द्विगुण a = _tent_eval (tent, lo);
   द्विगुण b = _tent_eval (tent, hi);
   *tmin = hb_min (a, b);
   *tmax = hb_max (a, b);
   जर (lo <= (द्विगुण) tent.मधला && (द्विगुण) tent.मधला <= hi)
     *tmax = hb_max (*tmax, 1.0);
 }

 /* Compute conservative reachable old-space final-coord ranges per axis for
  * avar2 partial instancing. With offset compensation, the instanced font's
  * final coordinates equal the original font's over the retained user box,
  * so the reachable range of axis i's final coordinate is the range of
  *   final_i = intermediate_i + delta_i(intermediates)
  * over the box of retained old-intermediate ranges: restricted axes span
  * their retained [a, b], free public axes [-1, +1], pinned axes sit at
  * their d_i, and hidden (private) axes at 0. Interval arithmetic over the
  * avar2 VarStore regions bounds delta_i. Mirrors fontTools'
  * _computeReachableRangesForAvar2, but computed from the original store at
  * plan time (the same quantity fontTools bounds via getExtremes on the
  * instanced store). Only constraining ranges (narrower than [-1, +1]) are
  * recorded. */
 स्थिर तार्किक
 _compute_avar2_reachable_ranges (hb_subset_plan_t *plan,
                                  hb_array_t<स्थिरांक OT::AxisRecord> axes,
                                  स्थिरांक OT::avar *avar_table,
                                  तार्किक detect_self_contained)
 {
   स्थिरांक OT::ItemVariationStore *var_store;
   स्थिरांक OT::DeltaSetIndexMap *varidx_map;
   जर (!avar_table->get_v2_store_and_map (&var_store, &varidx_map))
     परत असत्य;

   /* Old-intermediate input box per axis. */
   hb_hashmap_t<hb_tag_t, hb_pair_t<द्विगुण, द्विगुण>> box;
   साठी (स्थिरांक स्वतः &axis : axes)
   {
     hb_tag_t tag = axis.get_axis_tag ();
     द्विगुण lo = -1.0, hi = +1.0;
     Triple *old_int;
     जर (plan->user_axes_location.has (tag))
     {
       जर (!plan->old_intermediates.has (tag, &old_int)) परत असत्य;
       lo = (द्विगुण) old_int->minimum;
       hi = (द्विगुण) old_int->maximum;
     }
     अन्यथा जर (axis.is_hidden ())
       lo = hi = 0.0; /* private axis: intermediate is always 0 */
     जर (!box.set (tag, hb_pair (lo, hi))) परत असत्य;
   }

   hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> regions;
   जर (!var_store->get_region_list ().get_var_regions (plan->axes_old_index_tag_map, regions))
     परत असत्य;

   hb_hashmap_t<hb_tag_t, अचिन्हित> grid_pos;
   hb_vector_t<hb_vector_t<द्विगुण>> grid_points;
   साठी (अचिन्हित i = 0; i < axes.length; i++)
   {
     grid_pos.reset ();
     grid_points.reset ();

     hb_tag_t tag = axes[i].get_axis_tag ();
     hb_pair_t<द्विगुण, द्विगुण> *identity;
     जर (!box.has (tag, &identity)) परत असत्य;

     /* Collect the row's active regions (non-zero delta). A varidx (or the
      * implicit identity mapping) that doesn't resolve to a real store row
      * behaves as "no variation" at runtime, so it contributes no regions;
      * this also covers a NULL VarStore. */
     hb_vector_t<hb_pair_t<पूर्णांक, अचिन्हित>> active; /* (delta, region index) */
     uint32_t varidx = varidx_map->map (i);
     /* Runtime treats out-of-range delta-set indices as zero. */
     जर (varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX &&
	 !var_store->has_delta_set (varidx))
       varidx = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
     जर (varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
     {
       अचिन्हित outer = varidx >> 16;
       अचिन्हित inner = varidx & 0xFFFF;
       स्थिरांक OT::VarData &vd = var_store->get_sub_table (outer);
       स्थिरांक OT::HBUINT8 *delta_bytes = vd.get_delta_bytes ();
       अचिन्हित row_size = vd.get_row_size ();
       अचिन्हित region_मोजणी_2 = vd.get_region_index_मोजणी ();
       साठी (अचिन्हित r = 0; r < region_मोजणी_2; r++)
       {
	 पूर्णांक delta = vd.get_item_delta_fast (inner, r, delta_bytes, row_size);
	 जर (!delta) सुरूठेवा;
	 अचिन्हित region_idx = vd.get_region_index (r);
	 जर (region_idx >= regions.length) परत असत्य;
	 active.push (hb_pair (delta, region_idx));
       }
       जर (active.in_error ()) परत असत्य;
     }

     /* The delta is piecewise multilinear in the axis coordinates (each
      * region scalar is a product of independent per-axis tents), so over
      * the box its exact extremes are attained at vertices of the grid of
      * per-axis tent breakpoints. Enumerate that grid when it is small
      * (real fonts); otherwise fall back to per-region interval
      * arithmetic, which ignores correlations between regions and is
      * looser but still conservative. Mirrors fontTools'
      * VarStore.getExtremes. */
     constexpr अचिन्हित MAX_GRID = 1u << 14;
     तार्किक exact = सत्य;
     द्विगुण dmin = 0.0, dmax = 0.0; /* delta extremes, F2Dot14 units */
     द्विगुण vmin = 0.0, vmax = 0.0; /* identity + delta extremes, coord units */

     /* Grid axes: the target axis (identity term), plus every axis a valid
      * tent of an active region references. */
     स्वतः add_grid_axis = [&] (hb_tag_t t) -> पूर्णांक
     {
       अचिन्हित *pos;
       जर (grid_pos.has (t, &pos)) परत (पूर्णांक) *pos;
       अचिन्हित new_pos = grid_pos.get_population ();
       hb_pair_t<द्विगुण, द्विगुण> *आदान;
       द्विगुण blo = -1.0, bhi = +1.0;
       जर (box.has (t, &आदान)) { blo = आदान->first; bhi = आदान->second; }
       grid_points.push (hb_vector_t<द्विगुण> ());
       जर (grid_points.in_error ()) परत -1;
       स्वतः &points = grid_points[new_pos];
       points.push (blo);
       points.push (bhi);
       जर (points.in_error () || !grid_pos.set (t, new_pos)) परत -1;
       परत (पूर्णांक) new_pos;
     };

     जर (add_grid_axis (tag) < 0) परत असत्य;
     /* Per active region: (grid axis position, tent) for its valid tents. */
     hb_vector_t<hb_vector_t<hb_pair_t<अचिन्हित, Triple>>> region_tents;
     साठी (स्थिरांक स्वतः &ar : active)
     {
       region_tents.push (hb_vector_t<hb_pair_t<अचिन्हित, Triple>> ());
       जर (region_tents.in_error ()) परत असत्य;
       स्वतः &tents = region_tents[region_tents.length - 1];
       साठी (स्थिरांक स्वतः &_ : regions[ar.second])
       {
	 स्थिरांक Triple &tent = _.second;
	 /* Tents the runtime ignores (constant scalar 1) add no breakpoints. */
	 जर (tent.मधला == 0.f ||
	     tent.minimum > tent.मधला || tent.मधला > tent.maximum ||
	     (tent.minimum < 0.f && tent.maximum > 0.f))
	   सुरूठेवा;
	 पूर्णांक pos = add_grid_axis (_.first);
	 जर (pos < 0) परत असत्य;
	 स्वतः &points = grid_points[pos];
	 द्विगुण blo = points[0], bhi = points[1];
	 points.push (hb_clamp ((द्विगुण) tent.minimum, blo, bhi));
	 points.push (hb_clamp ((द्विगुण) tent.मधला, blo, bhi));
	 points.push (hb_clamp ((द्विगुण) tent.maximum, blo, bhi));
	 tents.push (hb_pair ((अचिन्हित) pos, tent));
	 जर (points.in_error () || tents.in_error ()) परत असत्य;
       }
     }

     अचिन्हित grid_size = 1;
     साठी (स्वतः &points : grid_points)
     {
       points.क्विक ([] (स्थिरांक द्विगुण &a, स्थिरांक द्विगुण &b) -> पूर्णांक
		     { परत a < b ? -1 : a > b ? +1 : 0; });
       अचिन्हित n = 0;
       साठी (अचिन्हित j = 0; j < points.length; j++)
	 जर (!n || points.arrayZ[j] != points.arrayZ[n - 1])
	   points.arrayZ[n++] = points.arrayZ[j];
       points.shrink (n);
       जर (grid_size > MAX_GRID / points.length) { exact = असत्य; थांबा; }
       grid_size *= points.length;
     }

     जर (exact)
     {
       अचिन्हित target_pos = 0; /* the target axis was added first */
       hb_vector_t<अचिन्हित> odometer;
       जर (!odometer.resize (grid_pos.get_population ())) परत असत्य;
       तार्किक first = सत्य;
       पर्यंत (सत्य)
       {
	 द्विगुण delta_sum = 0.0;
	 साठी (अचिन्हित r = 0; r < active.length; r++)
	 {
	   द्विगुण scalar = 1.0;
	   साठी (स्थिरांक स्वतः &pt : region_tents[r])
	   {
	     द्विगुण v = grid_points[pt.first][odometer[pt.first]];
	     scalar *= _tent_eval (pt.second, v);
	     जर (scalar == 0.0) थांबा;
	   }
	   delta_sum += scalar * active[r].first;
	 }
	 द्विगुण coord = grid_points[target_pos][odometer[target_pos]];
	 द्विगुण value = coord + delta_sum / 16384.0;
	 जर (first)
	 {
	   dmin = dmax = delta_sum;
	   vmin = vmax = value;
	   first = असत्य;
	 }
	 अन्यथा
	 {
	   dmin = hb_min (dmin, delta_sum); dmax = hb_max (dmax, delta_sum);
	   vmin = hb_min (vmin, value);     vmax = hb_max (vmax, value);
	 }
	 अचिन्हित k = 0;
	 साठी (; k < odometer.length; k++)
	 {
	   जर (++odometer[k] < grid_points[k].length) थांबा;
	   odometer[k] = 0;
	 }
	 जर (k == odometer.length) थांबा;
       }
     }
     अन्यथा
     {
       साठी (स्थिरांक स्वतः &ar : active)
       {
	 पूर्णांक delta = ar.first;
	 द्विगुण smin = 1.0, smax = 1.0;
	 साठी (स्थिरांक स्वतः &_ : regions[ar.second])
	 {
	   hb_pair_t<द्विगुण, द्विगुण> *आदान;
	   द्विगुण blo = -1.0, bhi = +1.0;
	   जर (box.has (_.first, &आदान)) { blo = आदान->first; bhi = आदान->second; }
	   द्विगुण tmin, tmax;
	   tent_मूल्य_range (_.second, blo, bhi, &tmin, &tmax);
	   smin *= tmin;
	   smax *= tmax;
	   जर (smax == 0.0) थांबा;
	 }
	 जर (delta > 0) { dmin += smin * delta; dmax += smax * delta; }
	 अन्यथा           { dmin += smax * delta; dmax += smin * delta; }
       }
       vmin = identity->first + dmin / 16384.0;
       vmax = identity->second + dmax / 16384.0;
     }
     /* A pinned axis whose delta is constant over the box is self-contained:
      * its final coordinate is a constant. It can be removed from fvar/avar
      * and its contribution baked into the variation tables like an ordinary
      * pin at that coordinate (in old final space). Round the delta alone
      * and add it to the exact quantized intermediate, matching the runtime
      * (map_coords_2_14) and fontTools. */
     Triple *user;
     जर (detect_self_contained && dmin == dmax &&
	 plan->user_axes_location.has (tag, &user) && user->is_point ())
     {
       पूर्णांक v_int = (पूर्णांक) roundf ((दशांश) identity->first * 16384.f) +
		   (पूर्णांक) roundf ((दशांश) dmin);
       v_int = hb_clamp (v_int, -(1 << 14), +(1 << 14));
       जर (!plan->avar2_self_contained.set (tag, v_int / 16384.0)) परत असत्य;
       सुरूठेवा; /* axis is dropped; no region can reference it afterwards */
     }

     /* Pad by one F2Dot14 unit against rounding differences with the
      * runtime's fixed-point evaluation. */
     द्विगुण lo = hb_clamp (vmin - 1.0 / 16384.0, -1.0, +1.0);
     द्विगुण hi = hb_clamp (vmax + 1.0 / 16384.0, -1.0, +1.0);
     lo = न्यूनमान (lo * 16384.0) / 16384.0;
     hi = उच्चमान (hi * 16384.0) / 16384.0;
     जर (lo <= -1.0 && hi >= +1.0)
       सुरूठेवा; /* not constraining */
     जर (!plan->avar2_reachable_ranges.set (tag, Triple (lo, hb_clamp (0.0, lo, hi), hi)))
       परत असत्य;
   }
   परत सत्य;
 }

#ifndef HB_NO_OT_FONT_CFF
 स्थिर ओळीत hb_font_t*
 _get_hb_font_with_variations (स्थिरांक hb_subset_plan_t *plan)
 {
   hb_font_t *font = hb_font_create (plan->source);

   जर (plan->has_avar2)
   {
     /* Under avar2, instancing applies only to the self-contained pins,
      * whose constant final coordinates the plan holds in normalized_coords.
      * Setting user-space variations would run the full avar2 mapping and
      * bake contributions that remain live in the instance's variations. */
     hb_font_set_var_coords_normalized (font, plan->normalized_coords.arrayZ,
                                        plan->normalized_coords.length);
     परत font;
   }

   hb_vector_t<hb_variation_t> vars;
   जर (!vars.alloc (plan->user_axes_location.get_population ())) {
     hb_font_destroy (font);
     परत nullptr;
   }

   साठी (स्वतः _ : plan->user_axes_location)
   {
     hb_variation_t var;
     var.tag = _.first;
     var.value = _.second.मधला;
     vars.push (var);
   }

   hb_font_set_variations (font, vars.arrayZ, plan->user_axes_location.get_population ());
   परत font;
 }
#endif

 ढाँचा<typename ItemVarStore>
 शून्य
 remap_variation_indices (स्थिरांक ItemVarStore &var_store,
                          स्थिरांक hb_set_t &variation_indices,
                          स्थिरांक hb_vector_t<पूर्णांक>& normalized_coords,
                          तार्किक calculate_delta, /* not pinned at default */
                          तार्किक no_variations, /* all axes pinned */
                          hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> &variation_idx_delta_map /* OUT */)
 {
   जर (&var_store == &Null (OT::ItemVariationStore)) परत;
   अचिन्हित subtable_मोजणी = var_store.get_sub_table_मोजणी ();
   स्वतः *store_cache = var_store.create_cache ();

   अचिन्हित new_major = 0, new_minor = 0;
   अचिन्हित last_major = (variation_indices.get_min ()) >> 16;
   साठी (अचिन्हित idx : variation_indices)
   {
     पूर्णांक delta = 0;
     जर (calculate_delta)
       delta = roundf (var_store.get_delta (idx, normalized_coords.arrayZ,
                                            normalized_coords.length, store_cache));

     जर (no_variations)
     {
       variation_idx_delta_map.set (idx, hb_pair_t<अचिन्हित, पूर्णांक> (HB_OT_LAYOUT_NO_VARIATIONS_INDEX, delta));
       सुरूठेवा;
     }

     uint16_t major = idx >> 16;
     जर (major >= subtable_मोजणी) थांबा;
     जर (major != last_major)
     {
       new_minor = 0;
       ++new_major;
     }

     अचिन्हित new_idx = (new_major << 16) + new_minor;
     variation_idx_delta_map.set (idx, hb_pair_t<अचिन्हित, पूर्णांक> (new_idx, delta));
     ++new_minor;
     last_major = major;
   }
   var_store.destroy_cache (store_cache);
 }

 ढाँचा
 शून्य
 remap_variation_indices<OT::ItemVariationStore> (स्थिरांक OT::ItemVariationStore &var_store,
                          स्थिरांक hb_set_t &variation_indices,
                          स्थिरांक hb_vector_t<पूर्णांक>& normalized_coords,
                          तार्किक calculate_delta, /* not pinned at default */
                          तार्किक no_variations, /* all axes pinned */
                          hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> &variation_idx_delta_map /* OUT */);

 #ifndef HB_NO_BASE
 शून्य
 collect_base_variation_indices (hb_subset_plan_t* plan)
 {
   hb_blob_ptr_t<OT::BASE> base = plan->source_table<OT::BASE> ();
   जर (!base->has_var_store ())
   {
     base.destroy ();
     परत;
   }

   hb_set_t varidx_set;
   base->collect_variation_indices (plan, varidx_set);
   स्थिरांक OT::ItemVariationStore &var_store = base->get_var_store ();
   अचिन्हित subtable_मोजणी = var_store.get_sub_table_मोजणी ();


   remap_variation_indices (var_store, varidx_set,
                             plan->normalized_coords,
                             !plan->pinned_at_default,
                             plan->all_axes_pinned,
                             plan->base_variation_idx_map);
   generate_varstore_inner_maps (varidx_set, subtable_मोजणी, plan->base_varstore_inner_maps);

   base.destroy ();
 }

 #endif

तार्किक
normalize_axes_location (hb_face_t *face, hb_subset_plan_t *plan)
{
  जर (plan->user_axes_location.is_empty ())
    परत सत्य;

  hb_array_t<स्थिरांक OT::AxisRecord> axes = face->table.fvar->get_axes ();
  साठी (स्थिरांक स्वतः &axis : axes)
  {
    hb_tag_t tag = axis.get_axis_tag ();
    स्थिरांक Triple *range;
    दशांश minimum, मधला, maximum;
    axis.get_coordinates (minimum, मधला, maximum);
    जर (plan->user_axes_location.has (tag, &range) && !range->is_point () &&
        range->minimum == minimum &&
        range->मधला == मधला &&
        range->maximum == maximum)
      plan->user_axes_location.del (tag);
  }
  जर (plan->user_axes_location.is_empty ())
    परत सत्य;

  जर (!plan->check_success (plan->normalized_coords.resize (axes.length)))
    परत असत्य;

  तार्किक has_avar = face->table.avar->has_data ();
  hb_vector_t<दशांश> normalized_mins;
  hb_vector_t<दशांश> normalized_defaults;
  hb_vector_t<दशांश> normalized_maxs;
  जर (has_avar)
  {
    जर (!plan->check_success (normalized_mins.resize (axes.length)) ||
        !plan->check_success (normalized_defaults.resize (axes.length)) ||
        !plan->check_success (normalized_maxs.resize (axes.length)))
      परत असत्य;
  }

  तार्किक axis_not_pinned = असत्य;
  अचिन्हित new_axis_idx = 0;
  अचिन्हित last_idx = 0;
  साठी (स्थिरांक स्वतः& _ : + hb_enumerate (axes))
  {
    अचिन्हित i = _.first;
    स्थिरांक OT::AxisRecord &axis = _.second;
    hb_tag_t axis_tag = axis.get_axis_tag ();
    plan->axes_old_index_tag_map.set (i, axis_tag);

    जर (!plan->user_axes_location.has (axis_tag) ||
        !plan->user_axes_location.get (axis_tag).is_point ())
    {
      axis_not_pinned = सत्य;
      plan->axes_index_map.set (i, new_axis_idx);
      plan->axis_tags.push (axis_tag);
      new_axis_idx++;
    }

    Triple *axis_range;
    जर (plan->user_axes_location.has (axis_tag, &axis_range))
    {
      plan->axes_triple_distances.set (axis_tag, axis.get_triple_distances ());

      दशांश normalized_min = axis.normalize_axis_मूल्य (axis_range->minimum);
      दशांश normalized_default = axis.normalize_axis_मूल्य (axis_range->मधला);
      दशांश normalized_max = axis.normalize_axis_मूल्य (axis_range->maximum);

      // TODO(behdad): Spec says axis normalization should be done in 16.16;
      // We used to do it in 2.14, but that's not correct.  I fixed this in
      // the fvar/avar code, but keeping 2.14 here for now to keep tests
      // happy. We might need to adjust fonttools as well.
      // I'm only fairly confident in the above statement. Anyway,
      // we should look deeper into this, and also update fonttools if
      // needed.

      // Round to 2.14
      normalized_min = roundf (normalized_min * 16384.f) / 16384.f;
      normalized_default = roundf (normalized_default * 16384.f) / 16384.f;
      normalized_max = roundf (normalized_max * 16384.f) / 16384.f;

      जर (has_avar)
      {
        normalized_mins[i] = normalized_min;
        normalized_defaults[i] = normalized_default;
        normalized_maxs[i] = normalized_max;
        last_idx = i;
      }
      अन्यथा
      {
        plan->axes_location.set (axis_tag, Triple ((द्विगुण) normalized_min,
                                                   (द्विगुण) normalized_default,
                                                   (द्विगुण) normalized_max));
        जर (normalized_default == -0.f)
          normalized_default = 0.f; // Normalize -0 to 0
        जर (normalized_default != 0.f)
          plan->pinned_at_default = असत्य;

        plan->normalized_coords[i] = roundf (normalized_default * 16384.f);
      }
    }
  }
  plan->all_axes_pinned = !axis_not_pinned;

  // TODO: use avar map_coords_16_16() when normalization is changed to 16.16
  // in fonttools
  जर (has_avar)
  {
    स्थिरांक OT::avar* avar_table = face->table.avar;
    अचिन्हित coords_len = last_idx + 1;

    जर (avar_table->has_v2_data () && !plan->all_axes_pinned)
    {
      /* avar2 partial instancing: use v1-only mapping to get intermediate-space
       * coords. These are used for IVS rebasing and offset compensation. */
      plan->has_avar2 = सत्य;

      जर (!plan->check_success (avar_table->map_coords_2_14 (normalized_mins.arrayZ, coords_len, सत्य)) ||
          !plan->check_success (avar_table->map_coords_2_14 (normalized_defaults.arrayZ, coords_len, सत्य)) ||
          !plan->check_success (avar_table->map_coords_2_14 (normalized_maxs.arrayZ, coords_len, सत्य)))
        परत असत्य;

      साठी (स्थिरांक स्वतः& _ : + hb_enumerate (axes))
      {
        अचिन्हित i = _.first;
        hb_tag_t axis_tag = _.second.get_axis_tag ();
        जर (plan->user_axes_location.has (axis_tag))
        {
          /* Store intermediate-space coords for offset compensation,
           * segment-map renormalization, and IVS rebasing. */
          plan->old_intermediates.set (axis_tag, Triple ((द्विगुण) normalized_mins[i],
                                                         (द्विगुण) normalized_defaults[i],
                                                         (द्विगुण) normalized_maxs[i]));
        }
      }

      /* Reachable-range computation also detects self-contained pinned
       * axes: pinned axes whose final coordinate is constant. Those are
       * removed from fvar, so suppress the detection when the face has
       * tables that cannot follow: CFF2 has no partial-pin instancing path
       * (its 'pinned' path flattens ALL blends; lift this once
       * https://github.com/harfbuzz/harfbuzz/pull/4710 lands, porting
       * fontTools' instantiateCFF2 from
       * https://github.com/fonttools/fonttools/pull/3506). Such axes stay
       * in fvar as ordinary hidden pins. VARC remaps its axis indices and
       * rejects instancing of axes it references. */
      hb_blob_t *cff2_blob = hb_face_reference_table (face, HB_TAG ('C','F','F','2'));
      तार्किक detect_self_contained = !hb_blob_get_length (cff2_blob);
      hb_blob_destroy (cff2_blob);
      जर (!_compute_avar2_reachable_ranges (plan, axes, avar_table,
                                            detect_self_contained))
        परत असत्य;

#ifndef HB_NO_VAR_COMPOSITES
      /* VARC overrides can reach coordinates outside the font-level
       * ranges. Do not cull variation data using those ranges. */
      जर (!plan->drop_tables.has (HB_TAG ('V','A','R','C')))
      {
        hb_blob_t *varc_blob = hb_face_reference_table (face, HB_TAG ('V','A','R','C'));
        जर (hb_blob_get_length (varc_blob))
          plan->avar2_reachable_ranges.reset ();
        hb_blob_destroy (varc_blob);
      }
#endif

      /* Keep all axes in fvar (pinned ones as hidden), EXCEPT self-contained
       * pinned axes, which are removed entirely. */
      plan->axes_index_map.reset ();
      plan->axis_tags.reset ();
      अचिन्हित retained_axis_idx = 0;
      साठी (स्थिरांक स्वतः& _ : + hb_enumerate (axes))
      {
        अचिन्हित i = _.first;
        hb_tag_t axis_tag = _.second.get_axis_tag ();
        जर (plan->avar2_self_contained.has (axis_tag))
          सुरूठेवा;
        plan->axes_index_map.set (i, retained_axis_idx++);
        plan->axis_tags.push (axis_tag);
      }
      /* The other tables see ONLY the self-contained pins, as an ordinary
       * partial instancing in old final-coordinate space; the standard
       * instancing machinery bakes those axes' contributions in. The
       * remaining restriction is carried entirely by avar. */
      plan->axes_location.reset ();
      जर (plan->avar2_self_contained.get_population ())
      {
        तार्किक sc_pinned_at_default = सत्य;
        साठी (स्वतः _ : plan->avar2_self_contained)
        {
          plan->axes_location.set (_.first, Triple (_.second, _.second, _.second));
          जर (_.second != 0.0)
            sc_pinned_at_default = असत्य;
        }
        साठी (स्थिरांक स्वतः& _ : + hb_enumerate (axes))
        {
          द्विगुण *v;
          plan->normalized_coords[_.first] =
              plan->avar2_self_contained.has (_.second.get_axis_tag (), &v)
              ? (पूर्णांक) roundf ((दशांश) (*v * 16384.0)) : 0;
        }
        plan->pinned_at_default = sc_pinned_at_default;
      }
      अन्यथा
        plan->normalized_coords.shrink (0);
    }
    अन्यथा
    {
      /* Standard avar v1 (or v1+v2 with all axes pinned) */
      जर (!plan->check_success (avar_table->map_coords_2_14 (normalized_mins.arrayZ, coords_len)) ||
          !plan->check_success (avar_table->map_coords_2_14 (normalized_defaults.arrayZ, coords_len)) ||
          !plan->check_success (avar_table->map_coords_2_14 (normalized_maxs.arrayZ, coords_len)))
        परत असत्य;

      साठी (स्थिरांक स्वतः& _ : + hb_enumerate (axes))
      {
        अचिन्हित i = _.first;
        hb_tag_t axis_tag = _.second.get_axis_tag ();
        जर (plan->user_axes_location.has (axis_tag))
        {
          plan->axes_location.set (axis_tag, Triple ((द्विगुण) normalized_mins[i],
                                                     (द्विगुण) normalized_defaults[i],
                                                     (द्विगुण) normalized_maxs[i]));
          दशांश normalized_default = normalized_defaults[i];
          जर (normalized_default == -0.f)
            normalized_default = 0.f;
          जर (normalized_default != 0.f)
            plan->pinned_at_default = असत्य;

          plan->normalized_coords[i] = roundf (normalized_default * 16384.f);
        }
      }
    }
  }
  परत सत्य;
}

#ifndef HB_NO_OT_FONT_CFF
शून्य
update_instance_metrics_map_from_cff2 (hb_subset_plan_t *plan)
{
  जर (!plan->normalized_coords) परत;
  OT::cff2::accelerator_t cff2 (plan->source);
  जर (!cff2.is_valid ()) परत;

  hb_font_t *font = _get_hb_font_with_variations (plan);
  जर (unlikely (!plan->check_success (font != nullptr)))
  {
    hb_font_destroy (font);
    परत;
  }

  hb_glyph_extents_t extents = {0x7FFF, -0x7FFF};
  OT::hmtx_accelerator_t _hmtx (plan->source);
  OT::hb_scalar_cache_t *hvar_store_cache = nullptr;
  जर (_hmtx.has_data () && _hmtx.var_table.get_length ())
    hvar_store_cache = _hmtx.var_table->get_var_store ().create_cache ();

  OT::vmtx_accelerator_t _vmtx (plan->source);
  OT::hb_scalar_cache_t *vvar_store_cache = nullptr;
  जर (_vmtx.has_data () && _vmtx.var_table.get_length ())
    vvar_store_cache = _vmtx.var_table->get_var_store ().create_cache ();

  साठी (स्वतः p : *plan->glyph_map)
  {
    hb_codepoint_t old_gid = p.first;
    hb_codepoint_t new_gid = p.second;
    जर (!cff2.get_extents (font, old_gid, &extents)) सुरूठेवा;
    तार्किक has_bounds_info = सत्य;
    जर (extents.x_bearing == 0 && extents.width == 0 &&
        extents.height == 0 && extents.y_bearing == 0)
      has_bounds_info = असत्य;

    जर (has_bounds_info)
    {
      plan->head_maxp_info.xMin = hb_min (plan->head_maxp_info.xMin, extents.x_bearing);
      plan->head_maxp_info.xMax = hb_max (plan->head_maxp_info.xMax,
						 hb_saturate_add (extents.x_bearing, extents.width));
      plan->head_maxp_info.yMax = hb_max (plan->head_maxp_info.yMax, extents.y_bearing);
      plan->head_maxp_info.yMin = hb_min (plan->head_maxp_info.yMin,
						 hb_saturate_add (extents.y_bearing, extents.height));
    }

    जर (_hmtx.has_data ())
    {
      द्विगुण hori_aw = _hmtx.get_advance_without_var_unscaled (old_gid);
      जर (_hmtx.var_table.get_length ())
        hori_aw += (द्विगुण) roundf (_hmtx.var_table->get_advance_delta_unscaled (old_gid, font->coords, font->अंक_coords,
									 hvar_store_cache));
      /* Malicious deltas can take the advance out of the UFWORD range that
       * hmtx serialization stores.  Clamp, to keep downstream consumers
       * (e.g. CFF width optimizer) bounded and consistent with hmtx. */
      hori_aw = hb_clamp (hori_aw, 0., 65535.);
      पूर्णांक lsb = extents.x_bearing;
      जर (!has_bounds_info)
      {
        _hmtx.get_leading_bearing_without_var_unscaled (old_gid, &lsb);
      }
      plan->hmtx_map.set (new_gid, hb_pair ((अचिन्हित) hori_aw, lsb));
      plan->bounds_width_vec[new_gid] = extents.width;
    }

    जर (_vmtx.has_data ())
    {
      द्विगुण vert_aw = _vmtx.get_advance_without_var_unscaled (old_gid);
      जर (_vmtx.var_table.get_length ())
        vert_aw += (द्विगुण) roundf (_vmtx.var_table->get_advance_delta_unscaled (old_gid, font->coords, font->अंक_coords,
									 vvar_store_cache));
      vert_aw = hb_clamp (vert_aw, 0., 65535.);
      hb_position_t vorg_x = 0;
      hb_position_t vorg_y = 0;
      पूर्णांक tsb = 0;
      जर (has_bounds_info &&
           hb_font_get_glyph_v_origin (font, old_gid, &vorg_x, &vorg_y))
      {
        tsb = hb_saturate_sub (vorg_y, extents.y_bearing);
      } अन्यथा {
        _vmtx.get_leading_bearing_without_var_unscaled (old_gid, &tsb);
      }

      plan->vmtx_map.set (new_gid, hb_pair ((अचिन्हित) vert_aw, tsb));
      plan->bounds_height_vec[new_gid] = extents.height;
    }
  }
  /* No glyph contributed bounds: do not serialize the accumulator's
   * inverted sentinels into head (or a downgraded CFF FontBBox). */
  जर (plan->head_maxp_info.xMin > plan->head_maxp_info.xMax)
  {
    plan->head_maxp_info.xMin = plan->head_maxp_info.xMax = 0;
    plan->head_maxp_info.yMin = plan->head_maxp_info.yMax = 0;
  }
  hb_font_destroy (font);
  जर (hvar_store_cache)
    _hmtx.var_table->get_var_store ().destroy_cache (hvar_store_cache);
  जर (vvar_store_cache)
    _vmtx.var_table->get_var_store ().destroy_cache (vvar_store_cache);
}
#endif

तार्किक
get_instance_glyphs_contour_points (hb_subset_plan_t *plan)
{
  /* contour_points vector only needed for updating gvar table (infer delta and
   * iup delta optimization) during partial instancing */
  जर (plan->user_axes_location.is_empty () || plan->all_axes_pinned)
    परत सत्य;

  OT::glyf_accelerator_t glyf (plan->source);

  साठी (स्वतः &_ : plan->new_to_old_gid_list)
  {
    hb_codepoint_t new_gid = _.first;
    contour_point_vector_t all_points;
    जर (new_gid == 0 && !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      जर (unlikely (!plan->new_gid_contour_points_map.set (new_gid, all_points)))
        परत असत्य;
      सुरूठेवा;
    }

    hb_codepoint_t old_gid = _.second;
    स्वतः glyph = glyf.glyph_for_gid (old_gid);
    जर (unlikely (!glyph.get_all_points_without_var (plan->source, all_points)))
      परत असत्य;
    जर (unlikely (!plan->new_gid_contour_points_map.set (new_gid, all_points)))
      परत असत्य;

    /* composite new gids are only needed by iup delta optimization */
    जर ((plan->flags & HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS) && glyph.is_composite ())
      plan->composite_new_gids.add (new_gid);
  }
  परत सत्य;
}

ढाँचा<typename DeltaSetIndexMap>
शून्य
remap_colrv1_delta_set_index_indices (स्थिरांक DeltaSetIndexMap &index_map,
                                      स्थिरांक hb_set_t &delta_set_idxes,
                                      hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /* OUT */)
{
  जर (!index_map.get_map_मोजणी ())
    परत;

  hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> delta_set_idx_delta_map;
  अचिन्हित new_delta_set_idx = 0;
  साठी (अचिन्हित delta_set_idx : delta_set_idxes)
  {
    अचिन्हित var_idx = index_map.map (delta_set_idx);
    अचिन्हित new_varidx = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    पूर्णांक delta = 0;

    जर (var_idx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
    {
      hb_pair_t<अचिन्हित, पूर्णांक> *new_varidx_delta;
      जर (!variation_idx_delta_map.has (var_idx, &new_varidx_delta)) सुरूठेवा;

      new_varidx = hb_first (*new_varidx_delta);
      delta = hb_second (*new_varidx_delta);
    }

    new_deltaset_idx_varidx_map.set (new_delta_set_idx, new_varidx);
    delta_set_idx_delta_map.set (delta_set_idx, hb_pair_t<अचिन्हित, पूर्णांक> (new_delta_set_idx, delta));
    new_delta_set_idx++;
  }
  variation_idx_delta_map = std::move (delta_set_idx_delta_map);
}

ढाँचा शून्य
remap_colrv1_delta_set_index_indices<OT::DeltaSetIndexMap> (स्थिरांक OT::DeltaSetIndexMap &index_map,
                                      स्थिरांक hb_set_t &delta_set_idxes,
                                      hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /* OUT */);

 #endif

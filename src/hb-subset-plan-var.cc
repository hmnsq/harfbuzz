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

 #அந்தர்பாவ "hb-ot-layout-common.hh"
#அந்தர்பாவ "hb-subset-plan.hh"

 #அந்தர்பாவ "hb-ot-var-common.hh"
 #அந்தர்பாவ "hb-ot-layout-base-table.hh"
 #அந்தர்பாவ "hb-ot-glyf-table.hh"
 #அந்தர்பாவ "hb-ot-var-fvar-table.hh"
 #அந்தர்பாவ "hb-ot-var-avar-table.hh"
 #அந்தர்பாவ "hb-ot-cff2-table.hh"

 #ifndef HB_NO_VAR

 வெறுமை
 generate_varstore_inner_maps (மாறிலி hb_set_t& varidx_set,
                               unsigned subtable_மொத்தம்,
                               hb_vector_t<hb_inc_bimap_t> &inner_maps /* OUT */)
 {
   எனில் (varidx_set.is_empty () || subtable_மொத்தம் == 0) திரும்பு;

   எனில் (unlikely (!inner_maps.resize (subtable_மொத்தம்))) திரும்பு;
   க்கு (unsigned idx : varidx_set)
   {
     uint16_t major = idx >> 16;
     uint16_t minor = idx & 0xFFFF;

     எனில் (major >= subtable_மொத்தம்)
       தொடர்;
     inner_maps[major].add (minor);
   }
 }

 /* Evaluate a variation-region tent at v, mirroring VarRegionAxis::evaluate
  * (invalid regions evaluate as constant 1). */
 நிலையான இரட்டை
 _tent_eval (மாறிலி Triple &tent, இரட்டை v)
 {
   இரட்டை start = (இரட்டை) tent.minimum;
   இரட்டை peak = (இரட்டை) tent.நடு;
   இரட்டை end = (இரட்டை) tent.maximum;
   எனில் (unlikely (start > peak || peak > end)) திரும்பு 1.0;
   எனில் (unlikely (start < 0.0 && end > 0.0 && peak != 0.0)) திரும்பு 1.0;
   எனில் (peak == 0.0 || v == peak) திரும்பு 1.0;
   எனில் (v <= start || end <= v) திரும்பு 0.0;
   எனில் (v < peak) திரும்பு (v - start) / (peak - start);
   திரும்பு (end - v) / (end - peak);
 }

 /* Range of a tent's scalar over an input interval [lo, hi]. The tent is
  * unimodal, so the extremes are at the interval endpoints, plus the peak
  * when it falls inside. */
 நிலையான வெறுமை
 tent_மதிப்பு_range (மாறிலி Triple &tent, இரட்டை lo, இரட்டை hi,
                    இரட்டை *tmin, இரட்டை *tmax)
 {
   இரட்டை a = _tent_eval (tent, lo);
   இரட்டை b = _tent_eval (tent, hi);
   *tmin = hb_min (a, b);
   *tmax = hb_max (a, b);
   எனில் (lo <= (இரட்டை) tent.நடு && (இரட்டை) tent.நடு <= hi)
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
 நிலையான தருக்க
 _compute_avar2_reachable_ranges (hb_subset_plan_t *plan,
                                  hb_array_t<மாறிலி OT::AxisRecord> axes,
                                  மாறிலி OT::avar *avar_table,
                                  தருக்க detect_self_contained)
 {
   மாறிலி OT::ItemVariationStore *var_store;
   மாறிலி OT::DeltaSetIndexMap *varidx_map;
   எனில் (!avar_table->get_v2_store_and_map (&var_store, &varidx_map))
     திரும்பு பொய்;

   /* Old-intermediate input box per axis. */
   hb_hashmap_t<hb_tag_t, hb_pair_t<இரட்டை, இரட்டை>> box;
   க்கு (மாறிலி auto &axis : axes)
   {
     hb_tag_t tag = axis.get_axis_tag ();
     இரட்டை lo = -1.0, hi = +1.0;
     Triple *old_int;
     எனில் (plan->user_axes_location.has (tag))
     {
       எனில் (!plan->old_intermediates.has (tag, &old_int)) திரும்பு பொய்;
       lo = (இரட்டை) old_int->minimum;
       hi = (இரட்டை) old_int->maximum;
     }
     இல்லையெனில் எனில் (axis.is_hidden ())
       lo = hi = 0.0; /* private axis: intermediate is always 0 */
     எனில் (!box.set (tag, hb_pair (lo, hi))) திரும்பு பொய்;
   }

   hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> regions;
   எனில் (!var_store->get_region_list ().get_var_regions (plan->axes_old_index_tag_map, regions))
     திரும்பு பொய்;

   hb_hashmap_t<hb_tag_t, unsigned> grid_pos;
   hb_vector_t<hb_vector_t<இரட்டை>> grid_points;
   க்கு (unsigned i = 0; i < axes.length; i++)
   {
     grid_pos.reset ();
     grid_points.reset ();

     hb_tag_t tag = axes[i].get_axis_tag ();
     hb_pair_t<இரட்டை, இரட்டை> *identity;
     எனில் (!box.has (tag, &identity)) திரும்பு பொய்;

     /* Collect the row's active regions (non-zero delta). A varidx (or the
      * implicit identity mapping) that doesn't resolve to a real store row
      * behaves as "no variation" at runtime, so it contributes no regions;
      * this also covers a NULL VarStore. */
     hb_vector_t<hb_pair_t<முழு, unsigned>> active; /* (delta, region index) */
     uint32_t varidx = varidx_map->map (i);
     /* Runtime treats out-of-range delta-set indices as zero. */
     எனில் (varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX &&
	 !var_store->has_delta_set (varidx))
       varidx = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
     எனில் (varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
     {
       unsigned outer = varidx >> 16;
       unsigned inner = varidx & 0xFFFF;
       மாறிலி OT::VarData &vd = var_store->get_sub_table (outer);
       மாறிலி OT::HBUINT8 *delta_bytes = vd.get_delta_bytes ();
       unsigned row_size = vd.get_row_size ();
       unsigned region_மொத்தம்_2 = vd.get_region_index_மொத்தம் ();
       க்கு (unsigned r = 0; r < region_மொத்தம்_2; r++)
       {
	 முழு delta = vd.get_item_delta_fast (inner, r, delta_bytes, row_size);
	 எனில் (!delta) தொடர்;
	 unsigned region_idx = vd.get_region_index (r);
	 எனில் (region_idx >= regions.length) திரும்பு பொய்;
	 active.push (hb_pair (delta, region_idx));
       }
       எனில் (active.in_error ()) திரும்பு பொய்;
     }

     /* The delta is piecewise multilinear in the axis coordinates (each
      * region scalar is a product of independent per-axis tents), so over
      * the box its exact extremes are attained at vertices of the grid of
      * per-axis tent breakpoints. Enumerate that grid when it is small
      * (real fonts); otherwise fall back to per-region interval
      * arithmetic, which ignores correlations between regions and is
      * looser but still conservative. Mirrors fontTools'
      * VarStore.getExtremes. */
     constexpr unsigned MAX_GRID = 1u << 14;
     தருக்க exact = உண்மை;
     இரட்டை dmin = 0.0, dmax = 0.0; /* delta extremes, F2Dot14 units */
     இரட்டை vmin = 0.0, vmax = 0.0; /* identity + delta extremes, coord units */

     /* Grid axes: the target axis (identity term), plus every axis a valid
      * tent of an active region references. */
     auto add_grid_axis = [&] (hb_tag_t t) -> முழு
     {
       unsigned *pos;
       எனில் (grid_pos.has (t, &pos)) திரும்பு (முழு) *pos;
       unsigned new_pos = grid_pos.get_population ();
       hb_pair_t<இரட்டை, இரட்டை> *உள்ளீடு;
       இரட்டை blo = -1.0, bhi = +1.0;
       எனில் (box.has (t, &உள்ளீடு)) { blo = உள்ளீடு->first; bhi = உள்ளீடு->second; }
       grid_points.push (hb_vector_t<இரட்டை> ());
       எனில் (grid_points.in_error ()) திரும்பு -1;
       auto &points = grid_points[new_pos];
       points.push (blo);
       points.push (bhi);
       எனில் (points.in_error () || !grid_pos.set (t, new_pos)) திரும்பு -1;
       திரும்பு (முழு) new_pos;
     };

     எனில் (add_grid_axis (tag) < 0) திரும்பு பொய்;
     /* Per active region: (grid axis position, tent) for its valid tents. */
     hb_vector_t<hb_vector_t<hb_pair_t<unsigned, Triple>>> region_tents;
     க்கு (மாறிலி auto &ar : active)
     {
       region_tents.push (hb_vector_t<hb_pair_t<unsigned, Triple>> ());
       எனில் (region_tents.in_error ()) திரும்பு பொய்;
       auto &tents = region_tents[region_tents.length - 1];
       க்கு (மாறிலி auto &_ : regions[ar.second])
       {
	 மாறிலி Triple &tent = _.second;
	 /* Tents the runtime ignores (constant scalar 1) add no breakpoints. */
	 எனில் (tent.நடு == 0.f ||
	     tent.minimum > tent.நடு || tent.நடு > tent.maximum ||
	     (tent.minimum < 0.f && tent.maximum > 0.f))
	   தொடர்;
	 முழு pos = add_grid_axis (_.first);
	 எனில் (pos < 0) திரும்பு பொய்;
	 auto &points = grid_points[pos];
	 இரட்டை blo = points[0], bhi = points[1];
	 points.push (hb_clamp ((இரட்டை) tent.minimum, blo, bhi));
	 points.push (hb_clamp ((இரட்டை) tent.நடு, blo, bhi));
	 points.push (hb_clamp ((இரட்டை) tent.maximum, blo, bhi));
	 tents.push (hb_pair ((unsigned) pos, tent));
	 எனில் (points.in_error () || tents.in_error ()) திரும்பு பொய்;
       }
     }

     unsigned grid_size = 1;
     க்கு (auto &points : grid_points)
     {
       points.qsort ([] (மாறிலி இரட்டை &a, மாறிலி இரட்டை &b) -> முழு
		     { திரும்பு a < b ? -1 : a > b ? +1 : 0; });
       unsigned n = 0;
       க்கு (unsigned j = 0; j < points.length; j++)
	 எனில் (!n || points.arrayZ[j] != points.arrayZ[n - 1])
	   points.arrayZ[n++] = points.arrayZ[j];
       points.shrink (n);
       எனில் (grid_size > MAX_GRID / points.length) { exact = பொய்; நிறுத்து; }
       grid_size *= points.length;
     }

     எனில் (exact)
     {
       unsigned target_pos = 0; /* the target axis was added first */
       hb_vector_t<unsigned> odometer;
       எனில் (!odometer.resize (grid_pos.get_population ())) திரும்பு பொய்;
       தருக்க first = உண்மை;
       போது (உண்மை)
       {
	 இரட்டை delta_sum = 0.0;
	 க்கு (unsigned r = 0; r < active.length; r++)
	 {
	   இரட்டை scalar = 1.0;
	   க்கு (மாறிலி auto &pt : region_tents[r])
	   {
	     இரட்டை v = grid_points[pt.first][odometer[pt.first]];
	     scalar *= _tent_eval (pt.second, v);
	     எனில் (scalar == 0.0) நிறுத்து;
	   }
	   delta_sum += scalar * active[r].first;
	 }
	 இரட்டை coord = grid_points[target_pos][odometer[target_pos]];
	 இரட்டை value = coord + delta_sum / 16384.0;
	 எனில் (first)
	 {
	   dmin = dmax = delta_sum;
	   vmin = vmax = value;
	   first = பொய்;
	 }
	 இல்லையெனில்
	 {
	   dmin = hb_min (dmin, delta_sum); dmax = hb_max (dmax, delta_sum);
	   vmin = hb_min (vmin, value);     vmax = hb_max (vmax, value);
	 }
	 unsigned k = 0;
	 க்கு (; k < odometer.length; k++)
	 {
	   எனில் (++odometer[k] < grid_points[k].length) நிறுத்து;
	   odometer[k] = 0;
	 }
	 எனில் (k == odometer.length) நிறுத்து;
       }
     }
     இல்லையெனில்
     {
       க்கு (மாறிலி auto &ar : active)
       {
	 முழு delta = ar.first;
	 இரட்டை smin = 1.0, smax = 1.0;
	 க்கு (மாறிலி auto &_ : regions[ar.second])
	 {
	   hb_pair_t<இரட்டை, இரட்டை> *உள்ளீடு;
	   இரட்டை blo = -1.0, bhi = +1.0;
	   எனில் (box.has (_.first, &உள்ளீடு)) { blo = உள்ளீடு->first; bhi = உள்ளீடு->second; }
	   இரட்டை tmin, tmax;
	   tent_மதிப்பு_range (_.second, blo, bhi, &tmin, &tmax);
	   smin *= tmin;
	   smax *= tmax;
	   எனில் (smax == 0.0) நிறுத்து;
	 }
	 எனில் (delta > 0) { dmin += smin * delta; dmax += smax * delta; }
	 இல்லையெனில்           { dmin += smax * delta; dmax += smin * delta; }
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
     எனில் (detect_self_contained && dmin == dmax &&
	 plan->user_axes_location.has (tag, &user) && user->is_point ())
     {
       முழு v_int = (முழு) roundf ((தசம) identity->first * 16384.f) +
		   (முழு) roundf ((தசம) dmin);
       v_int = hb_clamp (v_int, -(1 << 14), +(1 << 14));
       எனில் (!plan->avar2_self_contained.set (tag, v_int / 16384.0)) திரும்பு பொய்;
       தொடர்; /* axis is dropped; no region can reference it afterwards */
     }

     /* Pad by one F2Dot14 unit against rounding differences with the
      * runtime's fixed-point evaluation. */
     இரட்டை lo = hb_clamp (vmin - 1.0 / 16384.0, -1.0, +1.0);
     இரட்டை hi = hb_clamp (vmax + 1.0 / 16384.0, -1.0, +1.0);
     lo = floor (lo * 16384.0) / 16384.0;
     hi = ceil (hi * 16384.0) / 16384.0;
     எனில் (lo <= -1.0 && hi >= +1.0)
       தொடர்; /* not constraining */
     எனில் (!plan->avar2_reachable_ranges.set (tag, Triple (lo, hb_clamp (0.0, lo, hi), hi)))
       திரும்பு பொய்;
   }
   திரும்பு உண்மை;
 }

#ifndef HB_NO_OT_FONT_CFF
 நிலையான உள்வரி hb_font_t*
 _get_hb_font_with_variations (மாறிலி hb_subset_plan_t *plan)
 {
   hb_font_t *font = hb_font_create (plan->source);

   எனில் (plan->has_avar2)
   {
     /* Under avar2, instancing applies only to the self-contained pins,
      * whose constant final coordinates the plan holds in normalized_coords.
      * Setting user-space variations would run the full avar2 mapping and
      * bake contributions that remain live in the instance's variations. */
     hb_font_set_var_coords_normalized (font, plan->normalized_coords.arrayZ,
                                        plan->normalized_coords.length);
     திரும்பு font;
   }

   hb_vector_t<hb_variation_t> vars;
   எனில் (!vars.alloc (plan->user_axes_location.get_population ())) {
     hb_font_destroy (font);
     திரும்பு nullptr;
   }

   க்கு (auto _ : plan->user_axes_location)
   {
     hb_variation_t var;
     var.tag = _.first;
     var.value = _.second.நடு;
     vars.push (var);
   }

   hb_font_set_variations (font, vars.arrayZ, plan->user_axes_location.get_population ());
   திரும்பு font;
 }
#endif

 template<typename ItemVarStore>
 வெறுமை
 remap_variation_indices (மாறிலி ItemVarStore &var_store,
                          மாறிலி hb_set_t &variation_indices,
                          மாறிலி hb_vector_t<முழு>& normalized_coords,
                          தருக்க calculate_delta, /* not pinned at default */
                          தருக்க no_variations, /* all axes pinned */
                          hb_hashmap_t<unsigned, hb_pair_t<unsigned, முழு>> &variation_idx_delta_map /* OUT */)
 {
   எனில் (&var_store == &Null (OT::ItemVariationStore)) திரும்பு;
   unsigned subtable_மொத்தம் = var_store.get_sub_table_மொத்தம் ();
   auto *store_cache = var_store.create_cache ();

   unsigned new_major = 0, new_minor = 0;
   unsigned last_major = (variation_indices.get_min ()) >> 16;
   க்கு (unsigned idx : variation_indices)
   {
     முழு delta = 0;
     எனில் (calculate_delta)
       delta = roundf (var_store.get_delta (idx, normalized_coords.arrayZ,
                                            normalized_coords.length, store_cache));

     எனில் (no_variations)
     {
       variation_idx_delta_map.set (idx, hb_pair_t<unsigned, முழு> (HB_OT_LAYOUT_NO_VARIATIONS_INDEX, delta));
       தொடர்;
     }

     uint16_t major = idx >> 16;
     எனில் (major >= subtable_மொத்தம்) நிறுத்து;
     எனில் (major != last_major)
     {
       new_minor = 0;
       ++new_major;
     }

     unsigned new_idx = (new_major << 16) + new_minor;
     variation_idx_delta_map.set (idx, hb_pair_t<unsigned, முழு> (new_idx, delta));
     ++new_minor;
     last_major = major;
   }
   var_store.destroy_cache (store_cache);
 }

 template
 வெறுமை
 remap_variation_indices<OT::ItemVariationStore> (மாறிலி OT::ItemVariationStore &var_store,
                          மாறிலி hb_set_t &variation_indices,
                          மாறிலி hb_vector_t<முழு>& normalized_coords,
                          தருக்க calculate_delta, /* not pinned at default */
                          தருக்க no_variations, /* all axes pinned */
                          hb_hashmap_t<unsigned, hb_pair_t<unsigned, முழு>> &variation_idx_delta_map /* OUT */);

 #ifndef HB_NO_BASE
 வெறுமை
 collect_base_variation_indices (hb_subset_plan_t* plan)
 {
   hb_blob_ptr_t<OT::BASE> base = plan->source_table<OT::BASE> ();
   எனில் (!base->has_var_store ())
   {
     base.destroy ();
     திரும்பு;
   }

   hb_set_t varidx_set;
   base->collect_variation_indices (plan, varidx_set);
   மாறிலி OT::ItemVariationStore &var_store = base->get_var_store ();
   unsigned subtable_மொத்தம் = var_store.get_sub_table_மொத்தம் ();


   remap_variation_indices (var_store, varidx_set,
                             plan->normalized_coords,
                             !plan->pinned_at_default,
                             plan->all_axes_pinned,
                             plan->base_variation_idx_map);
   generate_varstore_inner_maps (varidx_set, subtable_மொத்தம், plan->base_varstore_inner_maps);

   base.destroy ();
 }

 #endif

தருக்க
normalize_axes_location (hb_face_t *face, hb_subset_plan_t *plan)
{
  எனில் (plan->user_axes_location.is_empty ())
    திரும்பு உண்மை;

  hb_array_t<மாறிலி OT::AxisRecord> axes = face->table.fvar->get_axes ();
  க்கு (மாறிலி auto &axis : axes)
  {
    hb_tag_t tag = axis.get_axis_tag ();
    மாறிலி Triple *range;
    தசம minimum, நடு, maximum;
    axis.get_coordinates (minimum, நடு, maximum);
    எனில் (plan->user_axes_location.has (tag, &range) && !range->is_point () &&
        range->minimum == minimum &&
        range->நடு == நடு &&
        range->maximum == maximum)
      plan->user_axes_location.del (tag);
  }
  எனில் (plan->user_axes_location.is_empty ())
    திரும்பு உண்மை;

  எனில் (!plan->check_success (plan->normalized_coords.resize (axes.length)))
    திரும்பு பொய்;

  தருக்க has_avar = face->table.avar->has_data ();
  hb_vector_t<தசம> normalized_mins;
  hb_vector_t<தசம> normalized_defaults;
  hb_vector_t<தசம> normalized_maxs;
  எனில் (has_avar)
  {
    எனில் (!plan->check_success (normalized_mins.resize (axes.length)) ||
        !plan->check_success (normalized_defaults.resize (axes.length)) ||
        !plan->check_success (normalized_maxs.resize (axes.length)))
      திரும்பு பொய்;
  }

  தருக்க axis_not_pinned = பொய்;
  unsigned new_axis_idx = 0;
  unsigned last_idx = 0;
  க்கு (மாறிலி auto& _ : + hb_enumerate (axes))
  {
    unsigned i = _.first;
    மாறிலி OT::AxisRecord &axis = _.second;
    hb_tag_t axis_tag = axis.get_axis_tag ();
    plan->axes_old_index_tag_map.set (i, axis_tag);

    எனில் (!plan->user_axes_location.has (axis_tag) ||
        !plan->user_axes_location.get (axis_tag).is_point ())
    {
      axis_not_pinned = உண்மை;
      plan->axes_index_map.set (i, new_axis_idx);
      plan->axis_tags.push (axis_tag);
      new_axis_idx++;
    }

    Triple *axis_range;
    எனில் (plan->user_axes_location.has (axis_tag, &axis_range))
    {
      plan->axes_triple_distances.set (axis_tag, axis.get_triple_distances ());

      தசம normalized_min = axis.normalize_axis_மதிப்பு (axis_range->minimum);
      தசம normalized_default = axis.normalize_axis_மதிப்பு (axis_range->நடு);
      தசம normalized_max = axis.normalize_axis_மதிப்பு (axis_range->maximum);

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

      எனில் (has_avar)
      {
        normalized_mins[i] = normalized_min;
        normalized_defaults[i] = normalized_default;
        normalized_maxs[i] = normalized_max;
        last_idx = i;
      }
      இல்லையெனில்
      {
        plan->axes_location.set (axis_tag, Triple ((இரட்டை) normalized_min,
                                                   (இரட்டை) normalized_default,
                                                   (இரட்டை) normalized_max));
        எனில் (normalized_default == -0.f)
          normalized_default = 0.f; // Normalize -0 to 0
        எனில் (normalized_default != 0.f)
          plan->pinned_at_default = பொய்;

        plan->normalized_coords[i] = roundf (normalized_default * 16384.f);
      }
    }
  }
  plan->all_axes_pinned = !axis_not_pinned;

  // TODO: use avar map_coords_16_16() when normalization is changed to 16.16
  // in fonttools
  எனில் (has_avar)
  {
    மாறிலி OT::avar* avar_table = face->table.avar;
    unsigned coords_len = last_idx + 1;

    எனில் (avar_table->has_v2_data () && !plan->all_axes_pinned)
    {
      /* avar2 partial instancing: use v1-only mapping to get intermediate-space
       * coords. These are used for IVS rebasing and offset compensation. */
      plan->has_avar2 = உண்மை;

      எனில் (!plan->check_success (avar_table->map_coords_2_14 (normalized_mins.arrayZ, coords_len, உண்மை)) ||
          !plan->check_success (avar_table->map_coords_2_14 (normalized_defaults.arrayZ, coords_len, உண்மை)) ||
          !plan->check_success (avar_table->map_coords_2_14 (normalized_maxs.arrayZ, coords_len, உண்மை)))
        திரும்பு பொய்;

      க்கு (மாறிலி auto& _ : + hb_enumerate (axes))
      {
        unsigned i = _.first;
        hb_tag_t axis_tag = _.second.get_axis_tag ();
        எனில் (plan->user_axes_location.has (axis_tag))
        {
          /* Store intermediate-space coords for offset compensation,
           * segment-map renormalization, and IVS rebasing. */
          plan->old_intermediates.set (axis_tag, Triple ((இரட்டை) normalized_mins[i],
                                                         (இரட்டை) normalized_defaults[i],
                                                         (இரட்டை) normalized_maxs[i]));
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
      தருக்க detect_self_contained = !hb_blob_get_length (cff2_blob);
      hb_blob_destroy (cff2_blob);
      எனில் (!_compute_avar2_reachable_ranges (plan, axes, avar_table,
                                            detect_self_contained))
        திரும்பு பொய்;

#ifndef HB_NO_VAR_COMPOSITES
      /* VARC overrides can reach coordinates outside the font-level
       * ranges. Do not cull variation data using those ranges. */
      எனில் (!plan->drop_tables.has (HB_TAG ('V','A','R','C')))
      {
        hb_blob_t *varc_blob = hb_face_reference_table (face, HB_TAG ('V','A','R','C'));
        எனில் (hb_blob_get_length (varc_blob))
          plan->avar2_reachable_ranges.reset ();
        hb_blob_destroy (varc_blob);
      }
#endif

      /* Keep all axes in fvar (pinned ones as hidden), EXCEPT self-contained
       * pinned axes, which are removed entirely. */
      plan->axes_index_map.reset ();
      plan->axis_tags.reset ();
      unsigned retained_axis_idx = 0;
      க்கு (மாறிலி auto& _ : + hb_enumerate (axes))
      {
        unsigned i = _.first;
        hb_tag_t axis_tag = _.second.get_axis_tag ();
        எனில் (plan->avar2_self_contained.has (axis_tag))
          தொடர்;
        plan->axes_index_map.set (i, retained_axis_idx++);
        plan->axis_tags.push (axis_tag);
      }
      /* The other tables see ONLY the self-contained pins, as an ordinary
       * partial instancing in old final-coordinate space; the standard
       * instancing machinery bakes those axes' contributions in. The
       * remaining restriction is carried entirely by avar. */
      plan->axes_location.reset ();
      எனில் (plan->avar2_self_contained.get_population ())
      {
        தருக்க sc_pinned_at_default = உண்மை;
        க்கு (auto _ : plan->avar2_self_contained)
        {
          plan->axes_location.set (_.first, Triple (_.second, _.second, _.second));
          எனில் (_.second != 0.0)
            sc_pinned_at_default = பொய்;
        }
        க்கு (மாறிலி auto& _ : + hb_enumerate (axes))
        {
          இரட்டை *v;
          plan->normalized_coords[_.first] =
              plan->avar2_self_contained.has (_.second.get_axis_tag (), &v)
              ? (முழு) roundf ((தசம) (*v * 16384.0)) : 0;
        }
        plan->pinned_at_default = sc_pinned_at_default;
      }
      இல்லையெனில்
        plan->normalized_coords.shrink (0);
    }
    இல்லையெனில்
    {
      /* Standard avar v1 (or v1+v2 with all axes pinned) */
      எனில் (!plan->check_success (avar_table->map_coords_2_14 (normalized_mins.arrayZ, coords_len)) ||
          !plan->check_success (avar_table->map_coords_2_14 (normalized_defaults.arrayZ, coords_len)) ||
          !plan->check_success (avar_table->map_coords_2_14 (normalized_maxs.arrayZ, coords_len)))
        திரும்பு பொய்;

      க்கு (மாறிலி auto& _ : + hb_enumerate (axes))
      {
        unsigned i = _.first;
        hb_tag_t axis_tag = _.second.get_axis_tag ();
        எனில் (plan->user_axes_location.has (axis_tag))
        {
          plan->axes_location.set (axis_tag, Triple ((இரட்டை) normalized_mins[i],
                                                     (இரட்டை) normalized_defaults[i],
                                                     (இரட்டை) normalized_maxs[i]));
          தசம normalized_default = normalized_defaults[i];
          எனில் (normalized_default == -0.f)
            normalized_default = 0.f;
          எனில் (normalized_default != 0.f)
            plan->pinned_at_default = பொய்;

          plan->normalized_coords[i] = roundf (normalized_default * 16384.f);
        }
      }
    }
  }
  திரும்பு உண்மை;
}

#ifndef HB_NO_OT_FONT_CFF
வெறுமை
update_instance_metrics_map_from_cff2 (hb_subset_plan_t *plan)
{
  எனில் (!plan->normalized_coords) திரும்பு;
  OT::cff2::accelerator_t cff2 (plan->source);
  எனில் (!cff2.is_valid ()) திரும்பு;

  hb_font_t *font = _get_hb_font_with_variations (plan);
  எனில் (unlikely (!plan->check_success (font != nullptr)))
  {
    hb_font_destroy (font);
    திரும்பு;
  }

  hb_glyph_extents_t extents = {0x7FFF, -0x7FFF};
  OT::hmtx_accelerator_t _hmtx (plan->source);
  OT::hb_scalar_cache_t *hvar_store_cache = nullptr;
  எனில் (_hmtx.has_data () && _hmtx.var_table.get_length ())
    hvar_store_cache = _hmtx.var_table->get_var_store ().create_cache ();

  OT::vmtx_accelerator_t _vmtx (plan->source);
  OT::hb_scalar_cache_t *vvar_store_cache = nullptr;
  எனில் (_vmtx.has_data () && _vmtx.var_table.get_length ())
    vvar_store_cache = _vmtx.var_table->get_var_store ().create_cache ();

  க்கு (auto p : *plan->glyph_map)
  {
    hb_codepoint_t old_gid = p.first;
    hb_codepoint_t new_gid = p.second;
    எனில் (!cff2.get_extents (font, old_gid, &extents)) தொடர்;
    தருக்க has_bounds_info = உண்மை;
    எனில் (extents.x_bearing == 0 && extents.width == 0 &&
        extents.height == 0 && extents.y_bearing == 0)
      has_bounds_info = பொய்;

    எனில் (has_bounds_info)
    {
      plan->head_maxp_info.xMin = hb_min (plan->head_maxp_info.xMin, extents.x_bearing);
      plan->head_maxp_info.xMax = hb_max (plan->head_maxp_info.xMax,
						 hb_saturate_add (extents.x_bearing, extents.width));
      plan->head_maxp_info.yMax = hb_max (plan->head_maxp_info.yMax, extents.y_bearing);
      plan->head_maxp_info.yMin = hb_min (plan->head_maxp_info.yMin,
						 hb_saturate_add (extents.y_bearing, extents.height));
    }

    எனில் (_hmtx.has_data ())
    {
      இரட்டை hori_aw = _hmtx.get_advance_without_var_unscaled (old_gid);
      எனில் (_hmtx.var_table.get_length ())
        hori_aw += (இரட்டை) roundf (_hmtx.var_table->get_advance_delta_unscaled (old_gid, font->coords, font->இலக்கம்_coords,
									 hvar_store_cache));
      /* Malicious deltas can take the advance out of the UFWORD range that
       * hmtx serialization stores.  Clamp, to keep downstream consumers
       * (e.g. CFF width optimizer) bounded and consistent with hmtx. */
      hori_aw = hb_clamp (hori_aw, 0., 65535.);
      முழு lsb = extents.x_bearing;
      எனில் (!has_bounds_info)
      {
        _hmtx.get_leading_bearing_without_var_unscaled (old_gid, &lsb);
      }
      plan->hmtx_map.set (new_gid, hb_pair ((unsigned) hori_aw, lsb));
      plan->bounds_width_vec[new_gid] = extents.width;
    }

    எனில் (_vmtx.has_data ())
    {
      இரட்டை vert_aw = _vmtx.get_advance_without_var_unscaled (old_gid);
      எனில் (_vmtx.var_table.get_length ())
        vert_aw += (இரட்டை) roundf (_vmtx.var_table->get_advance_delta_unscaled (old_gid, font->coords, font->இலக்கம்_coords,
									 vvar_store_cache));
      vert_aw = hb_clamp (vert_aw, 0., 65535.);
      hb_position_t vorg_x = 0;
      hb_position_t vorg_y = 0;
      முழு tsb = 0;
      எனில் (has_bounds_info &&
           hb_font_get_glyph_v_origin (font, old_gid, &vorg_x, &vorg_y))
      {
        tsb = hb_saturate_sub (vorg_y, extents.y_bearing);
      } இல்லையெனில் {
        _vmtx.get_leading_bearing_without_var_unscaled (old_gid, &tsb);
      }

      plan->vmtx_map.set (new_gid, hb_pair ((unsigned) vert_aw, tsb));
      plan->bounds_height_vec[new_gid] = extents.height;
    }
  }
  /* No glyph contributed bounds: do not serialize the accumulator's
   * inverted sentinels into head (or a downgraded CFF FontBBox). */
  எனில் (plan->head_maxp_info.xMin > plan->head_maxp_info.xMax)
  {
    plan->head_maxp_info.xMin = plan->head_maxp_info.xMax = 0;
    plan->head_maxp_info.yMin = plan->head_maxp_info.yMax = 0;
  }
  hb_font_destroy (font);
  எனில் (hvar_store_cache)
    _hmtx.var_table->get_var_store ().destroy_cache (hvar_store_cache);
  எனில் (vvar_store_cache)
    _vmtx.var_table->get_var_store ().destroy_cache (vvar_store_cache);
}
#endif

தருக்க
get_instance_glyphs_contour_points (hb_subset_plan_t *plan)
{
  /* contour_points vector only needed for updating gvar table (infer delta and
   * iup delta optimization) during partial instancing */
  எனில் (plan->user_axes_location.is_empty () || plan->all_axes_pinned)
    திரும்பு உண்மை;

  OT::glyf_accelerator_t glyf (plan->source);

  க்கு (auto &_ : plan->new_to_old_gid_list)
  {
    hb_codepoint_t new_gid = _.first;
    contour_point_vector_t all_points;
    எனில் (new_gid == 0 && !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      எனில் (unlikely (!plan->new_gid_contour_points_map.set (new_gid, all_points)))
        திரும்பு பொய்;
      தொடர்;
    }

    hb_codepoint_t old_gid = _.second;
    auto glyph = glyf.glyph_for_gid (old_gid);
    எனில் (unlikely (!glyph.get_all_points_without_var (plan->source, all_points)))
      திரும்பு பொய்;
    எனில் (unlikely (!plan->new_gid_contour_points_map.set (new_gid, all_points)))
      திரும்பு பொய்;

    /* composite new gids are only needed by iup delta optimization */
    எனில் ((plan->flags & HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS) && glyph.is_composite ())
      plan->composite_new_gids.add (new_gid);
  }
  திரும்பு உண்மை;
}

template<typename DeltaSetIndexMap>
வெறுமை
remap_colrv1_delta_set_index_indices (மாறிலி DeltaSetIndexMap &index_map,
                                      மாறிலி hb_set_t &delta_set_idxes,
                                      hb_hashmap_t<unsigned, hb_pair_t<unsigned, முழு>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /* OUT */)
{
  எனில் (!index_map.get_map_மொத்தம் ())
    திரும்பு;

  hb_hashmap_t<unsigned, hb_pair_t<unsigned, முழு>> delta_set_idx_delta_map;
  unsigned new_delta_set_idx = 0;
  க்கு (unsigned delta_set_idx : delta_set_idxes)
  {
    unsigned var_idx = index_map.map (delta_set_idx);
    unsigned new_varidx = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    முழு delta = 0;

    எனில் (var_idx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
    {
      hb_pair_t<unsigned, முழு> *new_varidx_delta;
      எனில் (!variation_idx_delta_map.has (var_idx, &new_varidx_delta)) தொடர்;

      new_varidx = hb_first (*new_varidx_delta);
      delta = hb_second (*new_varidx_delta);
    }

    new_deltaset_idx_varidx_map.set (new_delta_set_idx, new_varidx);
    delta_set_idx_delta_map.set (delta_set_idx, hb_pair_t<unsigned, முழு> (new_delta_set_idx, delta));
    new_delta_set_idx++;
  }
  variation_idx_delta_map = std::move (delta_set_idx_delta_map);
}

template வெறுமை
remap_colrv1_delta_set_index_indices<OT::DeltaSetIndexMap> (மாறிலி OT::DeltaSetIndexMap &index_map,
                                      மாறிலி hb_set_t &delta_set_idxes,
                                      hb_hashmap_t<unsigned, hb_pair_t<unsigned, முழு>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /* OUT */);

 #endif

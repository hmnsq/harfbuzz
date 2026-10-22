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

 #অন্তর্ভাৱ "hb-ot-layout-common.hh"
#অন্তর্ভাৱ "hb-subset-plan.hh"

 #অন্তর্ভাৱ "hb-ot-var-common.hh"
 #অন্তর্ভাৱ "hb-ot-layout-base-table.hh"
 #অন্তর্ভাৱ "hb-ot-glyf-table.hh"
 #অন্তর্ভাৱ "hb-ot-var-fvar-table.hh"
 #অন্তর্ভাৱ "hb-ot-var-avar-table.hh"
 #অন্তর্ভাৱ "hb-ot-cff2-table.hh"

 #ifndef HB_NO_VAR

 শূন্য
 generate_varstore_inner_maps (ধ্রুৱ hb_set_t& varidx_set,
                               unsigned subtable_গুনতি,
                               hb_vector_t<hb_inc_bimap_t> &inner_maps /* OUT */)
 {
   যদি (varidx_set.is_empty () || subtable_গুনতি == 0) ঘূৰাই;

   যদি (unlikely (!inner_maps.resize (subtable_গুনতি))) ঘূৰাই;
   বাবে (unsigned idx : varidx_set)
   {
     uint16_t major = idx >> 16;
     uint16_t minor = idx & 0xFFFF;

     যদি (major >= subtable_গুনতি)
       অনুৱর্ত;
     inner_maps[major].add (minor);
   }
 }

 /* Evaluate a variation-region tent at v, mirroring VarRegionAxis::evaluate
  * (invalid regions evaluate as constant 1). */
 স্থির দ্ৱিগুণ
 _tent_eval (ধ্রুৱ Triple &tent, দ্ৱিগুণ v)
 {
   দ্ৱিগুণ start = (দ্ৱিগুণ) tent.minimum;
   দ্ৱিগুণ peak = (দ্ৱিগুণ) tent.মধ্য;
   দ্ৱিগুণ end = (দ্ৱিগুণ) tent.maximum;
   যদি (unlikely (start > peak || peak > end)) ঘূৰাই 1.0;
   যদি (unlikely (start < 0.0 && end > 0.0 && peak != 0.0)) ঘূৰাই 1.0;
   যদি (peak == 0.0 || v == peak) ঘূৰাই 1.0;
   যদি (v <= start || end <= v) ঘূৰাই 0.0;
   যদি (v < peak) ঘূৰাই (v - start) / (peak - start);
   ঘূৰাই (end - v) / (end - peak);
 }

 /* Range of a tent's scalar over an input interval [lo, hi]. The tent is
  * unimodal, so the extremes are at the interval endpoints, plus the peak
  * when it falls inside. */
 স্থির শূন্য
 tent_মান_range (ধ্রুৱ Triple &tent, দ্ৱিগুণ lo, দ্ৱিগুণ hi,
                    দ্ৱিগুণ *tmin, দ্ৱিগুণ *tmax)
 {
   দ্ৱিগুণ a = _tent_eval (tent, lo);
   দ্ৱিগুণ b = _tent_eval (tent, hi);
   *tmin = hb_min (a, b);
   *tmax = hb_max (a, b);
   যদি (lo <= (দ্ৱিগুণ) tent.মধ্য && (দ্ৱিগুণ) tent.মধ্য <= hi)
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
 স্থির তার্কিক
 _compute_avar2_reachable_ranges (hb_subset_plan_t *plan,
                                  hb_array_t<ধ্রুৱ OT::AxisRecord> axes,
                                  ধ্রুৱ OT::avar *avar_table,
                                  তার্কিক detect_self_contained)
 {
   ধ্রুৱ OT::ItemVariationStore *var_store;
   ধ্রুৱ OT::DeltaSetIndexMap *varidx_map;
   যদি (!avar_table->get_v2_store_and_map (&var_store, &varidx_map))
     ঘূৰাই মিছা;

   /* Old-intermediate input box per axis. */
   hb_hashmap_t<hb_tag_t, hb_pair_t<দ্ৱিগুণ, দ্ৱিগুণ>> box;
   বাবে (ধ্রুৱ auto &axis : axes)
   {
     hb_tag_t tag = axis.get_axis_tag ();
     দ্ৱিগুণ lo = -1.0, hi = +1.0;
     Triple *old_int;
     যদি (plan->user_axes_location.has (tag))
     {
       যদি (!plan->old_intermediates.has (tag, &old_int)) ঘূৰাই মিছা;
       lo = (দ্ৱিগুণ) old_int->minimum;
       hi = (দ্ৱিগুণ) old_int->maximum;
     }
     অন্যথা যদি (axis.is_hidden ())
       lo = hi = 0.0; /* private axis: intermediate is always 0 */
     যদি (!box.set (tag, hb_pair (lo, hi))) ঘূৰাই মিছা;
   }

   hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> regions;
   যদি (!var_store->get_region_list ().get_var_regions (plan->axes_old_index_tag_map, regions))
     ঘূৰাই মিছা;

   hb_hashmap_t<hb_tag_t, unsigned> grid_pos;
   hb_vector_t<hb_vector_t<দ্ৱিগুণ>> grid_points;
   বাবে (unsigned i = 0; i < axes.length; i++)
   {
     grid_pos.reset ();
     grid_points.reset ();

     hb_tag_t tag = axes[i].get_axis_tag ();
     hb_pair_t<দ্ৱিগুণ, দ্ৱিগুণ> *identity;
     যদি (!box.has (tag, &identity)) ঘূৰাই মিছা;

     /* Collect the row's active regions (non-zero delta). A varidx (or the
      * implicit identity mapping) that doesn't resolve to a real store row
      * behaves as "no variation" at runtime, so it contributes no regions;
      * this also covers a NULL VarStore. */
     hb_vector_t<hb_pair_t<পূর্ণাঙ্ক, unsigned>> active; /* (delta, region index) */
     uint32_t varidx = varidx_map->map (i);
     /* Runtime treats out-of-range delta-set indices as zero. */
     যদি (varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX &&
	 !var_store->has_delta_set (varidx))
       varidx = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
     যদি (varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
     {
       unsigned outer = varidx >> 16;
       unsigned inner = varidx & 0xFFFF;
       ধ্রুৱ OT::VarData &vd = var_store->get_sub_table (outer);
       ধ্রুৱ OT::HBUINT8 *delta_bytes = vd.get_delta_bytes ();
       unsigned row_size = vd.get_row_size ();
       unsigned region_গুনতি_2 = vd.get_region_index_গুনতি ();
       বাবে (unsigned r = 0; r < region_গুনতি_2; r++)
       {
	 পূর্ণাঙ্ক delta = vd.get_item_delta_fast (inner, r, delta_bytes, row_size);
	 যদি (!delta) অনুৱর্ত;
	 unsigned region_idx = vd.get_region_index (r);
	 যদি (region_idx >= regions.length) ঘূৰাই মিছা;
	 active.push (hb_pair (delta, region_idx));
       }
       যদি (active.in_error ()) ঘূৰাই মিছা;
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
     তার্কিক exact = সঁচা;
     দ্ৱিগুণ dmin = 0.0, dmax = 0.0; /* delta extremes, F2Dot14 units */
     দ্ৱিগুণ vmin = 0.0, vmax = 0.0; /* identity + delta extremes, coord units */

     /* Grid axes: the target axis (identity term), plus every axis a valid
      * tent of an active region references. */
     auto add_grid_axis = [&] (hb_tag_t t) -> পূর্ণাঙ্ক
     {
       unsigned *pos;
       যদি (grid_pos.has (t, &pos)) ঘূৰাই (পূর্ণাঙ্ক) *pos;
       unsigned new_pos = grid_pos.get_population ();
       hb_pair_t<দ্ৱিগুণ, দ্ৱিগুণ> *প্ৰবেশ;
       দ্ৱিগুণ blo = -1.0, bhi = +1.0;
       যদি (box.has (t, &প্ৰবেশ)) { blo = প্ৰবেশ->first; bhi = প্ৰবেশ->second; }
       grid_points.push (hb_vector_t<দ্ৱিগুণ> ());
       যদি (grid_points.in_error ()) ঘূৰাই -1;
       auto &points = grid_points[new_pos];
       points.push (blo);
       points.push (bhi);
       যদি (points.in_error () || !grid_pos.set (t, new_pos)) ঘূৰাই -1;
       ঘূৰাই (পূর্ণাঙ্ক) new_pos;
     };

     যদি (add_grid_axis (tag) < 0) ঘূৰাই মিছা;
     /* Per active region: (grid axis position, tent) for its valid tents. */
     hb_vector_t<hb_vector_t<hb_pair_t<unsigned, Triple>>> region_tents;
     বাবে (ধ্রুৱ auto &ar : active)
     {
       region_tents.push (hb_vector_t<hb_pair_t<unsigned, Triple>> ());
       যদি (region_tents.in_error ()) ঘূৰাই মিছা;
       auto &tents = region_tents[region_tents.length - 1];
       বাবে (ধ্রুৱ auto &_ : regions[ar.second])
       {
	 ধ্রুৱ Triple &tent = _.second;
	 /* Tents the runtime ignores (constant scalar 1) add no breakpoints. */
	 যদি (tent.মধ্য == 0.f ||
	     tent.minimum > tent.মধ্য || tent.মধ্য > tent.maximum ||
	     (tent.minimum < 0.f && tent.maximum > 0.f))
	   অনুৱর্ত;
	 পূর্ণাঙ্ক pos = add_grid_axis (_.first);
	 যদি (pos < 0) ঘূৰাই মিছা;
	 auto &points = grid_points[pos];
	 দ্ৱিগুণ blo = points[0], bhi = points[1];
	 points.push (hb_clamp ((দ্ৱিগুণ) tent.minimum, blo, bhi));
	 points.push (hb_clamp ((দ্ৱিগুণ) tent.মধ্য, blo, bhi));
	 points.push (hb_clamp ((দ্ৱিগুণ) tent.maximum, blo, bhi));
	 tents.push (hb_pair ((unsigned) pos, tent));
	 যদি (points.in_error () || tents.in_error ()) ঘূৰাই মিছা;
       }
     }

     unsigned grid_size = 1;
     বাবে (auto &points : grid_points)
     {
       points.qsort ([] (ধ্রুৱ দ্ৱিগুণ &a, ধ্রুৱ দ্ৱিগুণ &b) -> পূর্ণাঙ্ক
		     { ঘূৰাই a < b ? -1 : a > b ? +1 : 0; });
       unsigned n = 0;
       বাবে (unsigned j = 0; j < points.length; j++)
	 যদি (!n || points.arrayZ[j] != points.arrayZ[n - 1])
	   points.arrayZ[n++] = points.arrayZ[j];
       points.shrink (n);
       যদি (grid_size > MAX_GRID / points.length) { exact = মিছা; ভঙ্গ; }
       grid_size *= points.length;
     }

     যদি (exact)
     {
       unsigned target_pos = 0; /* the target axis was added first */
       hb_vector_t<unsigned> odometer;
       যদি (!odometer.resize (grid_pos.get_population ())) ঘূৰাই মিছা;
       তার্কিক first = সঁচা;
       যেতিয়ালৈ (সঁচা)
       {
	 দ্ৱিগুণ delta_sum = 0.0;
	 বাবে (unsigned r = 0; r < active.length; r++)
	 {
	   দ্ৱিগুণ scalar = 1.0;
	   বাবে (ধ্রুৱ auto &pt : region_tents[r])
	   {
	     দ্ৱিগুণ v = grid_points[pt.first][odometer[pt.first]];
	     scalar *= _tent_eval (pt.second, v);
	     যদি (scalar == 0.0) ভঙ্গ;
	   }
	   delta_sum += scalar * active[r].first;
	 }
	 দ্ৱিগুণ coord = grid_points[target_pos][odometer[target_pos]];
	 দ্ৱিগুণ value = coord + delta_sum / 16384.0;
	 যদি (first)
	 {
	   dmin = dmax = delta_sum;
	   vmin = vmax = value;
	   first = মিছা;
	 }
	 অন্যথা
	 {
	   dmin = hb_min (dmin, delta_sum); dmax = hb_max (dmax, delta_sum);
	   vmin = hb_min (vmin, value);     vmax = hb_max (vmax, value);
	 }
	 unsigned k = 0;
	 বাবে (; k < odometer.length; k++)
	 {
	   যদি (++odometer[k] < grid_points[k].length) ভঙ্গ;
	   odometer[k] = 0;
	 }
	 যদি (k == odometer.length) ভঙ্গ;
       }
     }
     অন্যথা
     {
       বাবে (ধ্রুৱ auto &ar : active)
       {
	 পূর্ণাঙ্ক delta = ar.first;
	 দ্ৱিগুণ smin = 1.0, smax = 1.0;
	 বাবে (ধ্রুৱ auto &_ : regions[ar.second])
	 {
	   hb_pair_t<দ্ৱিগুণ, দ্ৱিগুণ> *প্ৰবেশ;
	   দ্ৱিগুণ blo = -1.0, bhi = +1.0;
	   যদি (box.has (_.first, &প্ৰবেশ)) { blo = প্ৰবেশ->first; bhi = প্ৰবেশ->second; }
	   দ্ৱিগুণ tmin, tmax;
	   tent_মান_range (_.second, blo, bhi, &tmin, &tmax);
	   smin *= tmin;
	   smax *= tmax;
	   যদি (smax == 0.0) ভঙ্গ;
	 }
	 যদি (delta > 0) { dmin += smin * delta; dmax += smax * delta; }
	 অন্যথা           { dmin += smax * delta; dmax += smin * delta; }
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
     যদি (detect_self_contained && dmin == dmax &&
	 plan->user_axes_location.has (tag, &user) && user->is_point ())
     {
       পূর্ণাঙ্ক v_int = (পূর্ণাঙ্ক) roundf ((ভিন্ন) identity->first * 16384.f) +
		   (পূর্ণাঙ্ক) roundf ((ভিন্ন) dmin);
       v_int = hb_clamp (v_int, -(1 << 14), +(1 << 14));
       যদি (!plan->avar2_self_contained.set (tag, v_int / 16384.0)) ঘূৰাই মিছা;
       অনুৱর্ত; /* axis is dropped; no region can reference it afterwards */
     }

     /* Pad by one F2Dot14 unit against rounding differences with the
      * runtime's fixed-point evaluation. */
     দ্ৱিগুণ lo = hb_clamp (vmin - 1.0 / 16384.0, -1.0, +1.0);
     দ্ৱিগুণ hi = hb_clamp (vmax + 1.0 / 16384.0, -1.0, +1.0);
     lo = floor (lo * 16384.0) / 16384.0;
     hi = ceil (hi * 16384.0) / 16384.0;
     যদি (lo <= -1.0 && hi >= +1.0)
       অনুৱর্ত; /* not constraining */
     যদি (!plan->avar2_reachable_ranges.set (tag, Triple (lo, hb_clamp (0.0, lo, hi), hi)))
       ঘূৰাই মিছা;
   }
   ঘূৰাই সঁচা;
 }

#ifndef HB_NO_OT_FONT_CFF
 স্থির অন্তঃস্থ hb_font_t*
 _get_hb_font_with_variations (ধ্রুৱ hb_subset_plan_t *plan)
 {
   hb_font_t *font = hb_font_create (plan->source);

   যদি (plan->has_avar2)
   {
     /* Under avar2, instancing applies only to the self-contained pins,
      * whose constant final coordinates the plan holds in normalized_coords.
      * Setting user-space variations would run the full avar2 mapping and
      * bake contributions that remain live in the instance's variations. */
     hb_font_set_var_coords_normalized (font, plan->normalized_coords.arrayZ,
                                        plan->normalized_coords.length);
     ঘূৰাই font;
   }

   hb_vector_t<hb_variation_t> vars;
   যদি (!vars.alloc (plan->user_axes_location.get_population ())) {
     hb_font_destroy (font);
     ঘূৰাই nullptr;
   }

   বাবে (auto _ : plan->user_axes_location)
   {
     hb_variation_t var;
     var.tag = _.first;
     var.value = _.second.মধ্য;
     vars.push (var);
   }

   hb_font_set_variations (font, vars.arrayZ, plan->user_axes_location.get_population ());
   ঘূৰাই font;
 }
#endif

 template<typename ItemVarStore>
 শূন্য
 remap_variation_indices (ধ্রুৱ ItemVarStore &var_store,
                          ধ্রুৱ hb_set_t &variation_indices,
                          ধ্রুৱ hb_vector_t<পূর্ণাঙ্ক>& normalized_coords,
                          তার্কিক calculate_delta, /* not pinned at default */
                          তার্কিক no_variations, /* all axes pinned */
                          hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণাঙ্ক>> &variation_idx_delta_map /* OUT */)
 {
   যদি (&var_store == &Null (OT::ItemVariationStore)) ঘূৰাই;
   unsigned subtable_গুনতি = var_store.get_sub_table_গুনতি ();
   auto *store_cache = var_store.create_cache ();

   unsigned new_major = 0, new_minor = 0;
   unsigned last_major = (variation_indices.get_min ()) >> 16;
   বাবে (unsigned idx : variation_indices)
   {
     পূর্ণাঙ্ক delta = 0;
     যদি (calculate_delta)
       delta = roundf (var_store.get_delta (idx, normalized_coords.arrayZ,
                                            normalized_coords.length, store_cache));

     যদি (no_variations)
     {
       variation_idx_delta_map.set (idx, hb_pair_t<unsigned, পূর্ণাঙ্ক> (HB_OT_LAYOUT_NO_VARIATIONS_INDEX, delta));
       অনুৱর্ত;
     }

     uint16_t major = idx >> 16;
     যদি (major >= subtable_গুনতি) ভঙ্গ;
     যদি (major != last_major)
     {
       new_minor = 0;
       ++new_major;
     }

     unsigned new_idx = (new_major << 16) + new_minor;
     variation_idx_delta_map.set (idx, hb_pair_t<unsigned, পূর্ণাঙ্ক> (new_idx, delta));
     ++new_minor;
     last_major = major;
   }
   var_store.destroy_cache (store_cache);
 }

 template
 শূন্য
 remap_variation_indices<OT::ItemVariationStore> (ধ্রুৱ OT::ItemVariationStore &var_store,
                          ধ্রুৱ hb_set_t &variation_indices,
                          ধ্রুৱ hb_vector_t<পূর্ণাঙ্ক>& normalized_coords,
                          তার্কিক calculate_delta, /* not pinned at default */
                          তার্কিক no_variations, /* all axes pinned */
                          hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণাঙ্ক>> &variation_idx_delta_map /* OUT */);

 #ifndef HB_NO_BASE
 শূন্য
 collect_base_variation_indices (hb_subset_plan_t* plan)
 {
   hb_blob_ptr_t<OT::BASE> base = plan->source_table<OT::BASE> ();
   যদি (!base->has_var_store ())
   {
     base.destroy ();
     ঘূৰাই;
   }

   hb_set_t varidx_set;
   base->collect_variation_indices (plan, varidx_set);
   ধ্রুৱ OT::ItemVariationStore &var_store = base->get_var_store ();
   unsigned subtable_গুনতি = var_store.get_sub_table_গুনতি ();


   remap_variation_indices (var_store, varidx_set,
                             plan->normalized_coords,
                             !plan->pinned_at_default,
                             plan->all_axes_pinned,
                             plan->base_variation_idx_map);
   generate_varstore_inner_maps (varidx_set, subtable_গুনতি, plan->base_varstore_inner_maps);

   base.destroy ();
 }

 #endif

তার্কিক
normalize_axes_location (hb_face_t *face, hb_subset_plan_t *plan)
{
  যদি (plan->user_axes_location.is_empty ())
    ঘূৰাই সঁচা;

  hb_array_t<ধ্রুৱ OT::AxisRecord> axes = face->table.fvar->get_axes ();
  বাবে (ধ্রুৱ auto &axis : axes)
  {
    hb_tag_t tag = axis.get_axis_tag ();
    ধ্রুৱ Triple *range;
    ভিন্ন minimum, মধ্য, maximum;
    axis.get_coordinates (minimum, মধ্য, maximum);
    যদি (plan->user_axes_location.has (tag, &range) && !range->is_point () &&
        range->minimum == minimum &&
        range->মধ্য == মধ্য &&
        range->maximum == maximum)
      plan->user_axes_location.del (tag);
  }
  যদি (plan->user_axes_location.is_empty ())
    ঘূৰাই সঁচা;

  যদি (!plan->check_success (plan->normalized_coords.resize (axes.length)))
    ঘূৰাই মিছা;

  তার্কিক has_avar = face->table.avar->has_data ();
  hb_vector_t<ভিন্ন> normalized_mins;
  hb_vector_t<ভিন্ন> normalized_defaults;
  hb_vector_t<ভিন্ন> normalized_maxs;
  যদি (has_avar)
  {
    যদি (!plan->check_success (normalized_mins.resize (axes.length)) ||
        !plan->check_success (normalized_defaults.resize (axes.length)) ||
        !plan->check_success (normalized_maxs.resize (axes.length)))
      ঘূৰাই মিছা;
  }

  তার্কিক axis_not_pinned = মিছা;
  unsigned new_axis_idx = 0;
  unsigned last_idx = 0;
  বাবে (ধ্রুৱ auto& _ : + hb_enumerate (axes))
  {
    unsigned i = _.first;
    ধ্রুৱ OT::AxisRecord &axis = _.second;
    hb_tag_t axis_tag = axis.get_axis_tag ();
    plan->axes_old_index_tag_map.set (i, axis_tag);

    যদি (!plan->user_axes_location.has (axis_tag) ||
        !plan->user_axes_location.get (axis_tag).is_point ())
    {
      axis_not_pinned = সঁচা;
      plan->axes_index_map.set (i, new_axis_idx);
      plan->axis_tags.push (axis_tag);
      new_axis_idx++;
    }

    Triple *axis_range;
    যদি (plan->user_axes_location.has (axis_tag, &axis_range))
    {
      plan->axes_triple_distances.set (axis_tag, axis.get_triple_distances ());

      ভিন্ন normalized_min = axis.normalize_axis_মান (axis_range->minimum);
      ভিন্ন normalized_default = axis.normalize_axis_মান (axis_range->মধ্য);
      ভিন্ন normalized_max = axis.normalize_axis_মান (axis_range->maximum);

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

      যদি (has_avar)
      {
        normalized_mins[i] = normalized_min;
        normalized_defaults[i] = normalized_default;
        normalized_maxs[i] = normalized_max;
        last_idx = i;
      }
      অন্যথা
      {
        plan->axes_location.set (axis_tag, Triple ((দ্ৱিগুণ) normalized_min,
                                                   (দ্ৱিগুণ) normalized_default,
                                                   (দ্ৱিগুণ) normalized_max));
        যদি (normalized_default == -0.f)
          normalized_default = 0.f; // Normalize -0 to 0
        যদি (normalized_default != 0.f)
          plan->pinned_at_default = মিছা;

        plan->normalized_coords[i] = roundf (normalized_default * 16384.f);
      }
    }
  }
  plan->all_axes_pinned = !axis_not_pinned;

  // TODO: use avar map_coords_16_16() when normalization is changed to 16.16
  // in fonttools
  যদি (has_avar)
  {
    ধ্রুৱ OT::avar* avar_table = face->table.avar;
    unsigned coords_len = last_idx + 1;

    যদি (avar_table->has_v2_data () && !plan->all_axes_pinned)
    {
      /* avar2 partial instancing: use v1-only mapping to get intermediate-space
       * coords. These are used for IVS rebasing and offset compensation. */
      plan->has_avar2 = সঁচা;

      যদি (!plan->check_success (avar_table->map_coords_2_14 (normalized_mins.arrayZ, coords_len, সঁচা)) ||
          !plan->check_success (avar_table->map_coords_2_14 (normalized_defaults.arrayZ, coords_len, সঁচা)) ||
          !plan->check_success (avar_table->map_coords_2_14 (normalized_maxs.arrayZ, coords_len, সঁচা)))
        ঘূৰাই মিছা;

      বাবে (ধ্রুৱ auto& _ : + hb_enumerate (axes))
      {
        unsigned i = _.first;
        hb_tag_t axis_tag = _.second.get_axis_tag ();
        যদি (plan->user_axes_location.has (axis_tag))
        {
          /* Store intermediate-space coords for offset compensation,
           * segment-map renormalization, and IVS rebasing. */
          plan->old_intermediates.set (axis_tag, Triple ((দ্ৱিগুণ) normalized_mins[i],
                                                         (দ্ৱিগুণ) normalized_defaults[i],
                                                         (দ্ৱিগুণ) normalized_maxs[i]));
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
      তার্কিক detect_self_contained = !hb_blob_get_length (cff2_blob);
      hb_blob_destroy (cff2_blob);
      যদি (!_compute_avar2_reachable_ranges (plan, axes, avar_table,
                                            detect_self_contained))
        ঘূৰাই মিছা;

#ifndef HB_NO_VAR_COMPOSITES
      /* VARC overrides can reach coordinates outside the font-level
       * ranges. Do not cull variation data using those ranges. */
      যদি (!plan->drop_tables.has (HB_TAG ('V','A','R','C')))
      {
        hb_blob_t *varc_blob = hb_face_reference_table (face, HB_TAG ('V','A','R','C'));
        যদি (hb_blob_get_length (varc_blob))
          plan->avar2_reachable_ranges.reset ();
        hb_blob_destroy (varc_blob);
      }
#endif

      /* Keep all axes in fvar (pinned ones as hidden), EXCEPT self-contained
       * pinned axes, which are removed entirely. */
      plan->axes_index_map.reset ();
      plan->axis_tags.reset ();
      unsigned retained_axis_idx = 0;
      বাবে (ধ্রুৱ auto& _ : + hb_enumerate (axes))
      {
        unsigned i = _.first;
        hb_tag_t axis_tag = _.second.get_axis_tag ();
        যদি (plan->avar2_self_contained.has (axis_tag))
          অনুৱর্ত;
        plan->axes_index_map.set (i, retained_axis_idx++);
        plan->axis_tags.push (axis_tag);
      }
      /* The other tables see ONLY the self-contained pins, as an ordinary
       * partial instancing in old final-coordinate space; the standard
       * instancing machinery bakes those axes' contributions in. The
       * remaining restriction is carried entirely by avar. */
      plan->axes_location.reset ();
      যদি (plan->avar2_self_contained.get_population ())
      {
        তার্কিক sc_pinned_at_default = সঁচা;
        বাবে (auto _ : plan->avar2_self_contained)
        {
          plan->axes_location.set (_.first, Triple (_.second, _.second, _.second));
          যদি (_.second != 0.0)
            sc_pinned_at_default = মিছা;
        }
        বাবে (ধ্রুৱ auto& _ : + hb_enumerate (axes))
        {
          দ্ৱিগুণ *v;
          plan->normalized_coords[_.first] =
              plan->avar2_self_contained.has (_.second.get_axis_tag (), &v)
              ? (পূর্ণাঙ্ক) roundf ((ভিন্ন) (*v * 16384.0)) : 0;
        }
        plan->pinned_at_default = sc_pinned_at_default;
      }
      অন্যথা
        plan->normalized_coords.shrink (0);
    }
    অন্যথা
    {
      /* Standard avar v1 (or v1+v2 with all axes pinned) */
      যদি (!plan->check_success (avar_table->map_coords_2_14 (normalized_mins.arrayZ, coords_len)) ||
          !plan->check_success (avar_table->map_coords_2_14 (normalized_defaults.arrayZ, coords_len)) ||
          !plan->check_success (avar_table->map_coords_2_14 (normalized_maxs.arrayZ, coords_len)))
        ঘূৰাই মিছা;

      বাবে (ধ্রুৱ auto& _ : + hb_enumerate (axes))
      {
        unsigned i = _.first;
        hb_tag_t axis_tag = _.second.get_axis_tag ();
        যদি (plan->user_axes_location.has (axis_tag))
        {
          plan->axes_location.set (axis_tag, Triple ((দ্ৱিগুণ) normalized_mins[i],
                                                     (দ্ৱিগুণ) normalized_defaults[i],
                                                     (দ্ৱিগুণ) normalized_maxs[i]));
          ভিন্ন normalized_default = normalized_defaults[i];
          যদি (normalized_default == -0.f)
            normalized_default = 0.f;
          যদি (normalized_default != 0.f)
            plan->pinned_at_default = মিছা;

          plan->normalized_coords[i] = roundf (normalized_default * 16384.f);
        }
      }
    }
  }
  ঘূৰাই সঁচা;
}

#ifndef HB_NO_OT_FONT_CFF
শূন্য
update_instance_metrics_map_from_cff2 (hb_subset_plan_t *plan)
{
  যদি (!plan->normalized_coords) ঘূৰাই;
  OT::cff2::accelerator_t cff2 (plan->source);
  যদি (!cff2.is_valid ()) ঘূৰাই;

  hb_font_t *font = _get_hb_font_with_variations (plan);
  যদি (unlikely (!plan->check_success (font != nullptr)))
  {
    hb_font_destroy (font);
    ঘূৰাই;
  }

  hb_glyph_extents_t extents = {0x7FFF, -0x7FFF};
  OT::hmtx_accelerator_t _hmtx (plan->source);
  OT::hb_scalar_cache_t *hvar_store_cache = nullptr;
  যদি (_hmtx.has_data () && _hmtx.var_table.get_length ())
    hvar_store_cache = _hmtx.var_table->get_var_store ().create_cache ();

  OT::vmtx_accelerator_t _vmtx (plan->source);
  OT::hb_scalar_cache_t *vvar_store_cache = nullptr;
  যদি (_vmtx.has_data () && _vmtx.var_table.get_length ())
    vvar_store_cache = _vmtx.var_table->get_var_store ().create_cache ();

  বাবে (auto p : *plan->glyph_map)
  {
    hb_codepoint_t old_gid = p.first;
    hb_codepoint_t new_gid = p.second;
    যদি (!cff2.get_extents (font, old_gid, &extents)) অনুৱর্ত;
    তার্কিক has_bounds_info = সঁচা;
    যদি (extents.x_bearing == 0 && extents.width == 0 &&
        extents.height == 0 && extents.y_bearing == 0)
      has_bounds_info = মিছা;

    যদি (has_bounds_info)
    {
      plan->head_maxp_info.xMin = hb_min (plan->head_maxp_info.xMin, extents.x_bearing);
      plan->head_maxp_info.xMax = hb_max (plan->head_maxp_info.xMax,
						 hb_saturate_add (extents.x_bearing, extents.width));
      plan->head_maxp_info.yMax = hb_max (plan->head_maxp_info.yMax, extents.y_bearing);
      plan->head_maxp_info.yMin = hb_min (plan->head_maxp_info.yMin,
						 hb_saturate_add (extents.y_bearing, extents.height));
    }

    যদি (_hmtx.has_data ())
    {
      দ্ৱিগুণ hori_aw = _hmtx.get_advance_without_var_unscaled (old_gid);
      যদি (_hmtx.var_table.get_length ())
        hori_aw += (দ্ৱিগুণ) roundf (_hmtx.var_table->get_advance_delta_unscaled (old_gid, font->coords, font->অঙ্ক_coords,
									 hvar_store_cache));
      /* Malicious deltas can take the advance out of the UFWORD range that
       * hmtx serialization stores.  Clamp, to keep downstream consumers
       * (e.g. CFF width optimizer) bounded and consistent with hmtx. */
      hori_aw = hb_clamp (hori_aw, 0., 65535.);
      পূর্ণাঙ্ক lsb = extents.x_bearing;
      যদি (!has_bounds_info)
      {
        _hmtx.get_leading_bearing_without_var_unscaled (old_gid, &lsb);
      }
      plan->hmtx_map.set (new_gid, hb_pair ((unsigned) hori_aw, lsb));
      plan->bounds_width_vec[new_gid] = extents.width;
    }

    যদি (_vmtx.has_data ())
    {
      দ্ৱিগুণ vert_aw = _vmtx.get_advance_without_var_unscaled (old_gid);
      যদি (_vmtx.var_table.get_length ())
        vert_aw += (দ্ৱিগুণ) roundf (_vmtx.var_table->get_advance_delta_unscaled (old_gid, font->coords, font->অঙ্ক_coords,
									 vvar_store_cache));
      vert_aw = hb_clamp (vert_aw, 0., 65535.);
      hb_position_t vorg_x = 0;
      hb_position_t vorg_y = 0;
      পূর্ণাঙ্ক tsb = 0;
      যদি (has_bounds_info &&
           hb_font_get_glyph_v_origin (font, old_gid, &vorg_x, &vorg_y))
      {
        tsb = hb_saturate_sub (vorg_y, extents.y_bearing);
      } অন্যথা {
        _vmtx.get_leading_bearing_without_var_unscaled (old_gid, &tsb);
      }

      plan->vmtx_map.set (new_gid, hb_pair ((unsigned) vert_aw, tsb));
      plan->bounds_height_vec[new_gid] = extents.height;
    }
  }
  /* No glyph contributed bounds: do not serialize the accumulator's
   * inverted sentinels into head (or a downgraded CFF FontBBox). */
  যদি (plan->head_maxp_info.xMin > plan->head_maxp_info.xMax)
  {
    plan->head_maxp_info.xMin = plan->head_maxp_info.xMax = 0;
    plan->head_maxp_info.yMin = plan->head_maxp_info.yMax = 0;
  }
  hb_font_destroy (font);
  যদি (hvar_store_cache)
    _hmtx.var_table->get_var_store ().destroy_cache (hvar_store_cache);
  যদি (vvar_store_cache)
    _vmtx.var_table->get_var_store ().destroy_cache (vvar_store_cache);
}
#endif

তার্কিক
get_instance_glyphs_contour_points (hb_subset_plan_t *plan)
{
  /* contour_points vector only needed for updating gvar table (infer delta and
   * iup delta optimization) during partial instancing */
  যদি (plan->user_axes_location.is_empty () || plan->all_axes_pinned)
    ঘূৰাই সঁচা;

  OT::glyf_accelerator_t glyf (plan->source);

  বাবে (auto &_ : plan->new_to_old_gid_list)
  {
    hb_codepoint_t new_gid = _.first;
    contour_point_vector_t all_points;
    যদি (new_gid == 0 && !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      যদি (unlikely (!plan->new_gid_contour_points_map.set (new_gid, all_points)))
        ঘূৰাই মিছা;
      অনুৱর্ত;
    }

    hb_codepoint_t old_gid = _.second;
    auto glyph = glyf.glyph_for_gid (old_gid);
    যদি (unlikely (!glyph.get_all_points_without_var (plan->source, all_points)))
      ঘূৰাই মিছা;
    যদি (unlikely (!plan->new_gid_contour_points_map.set (new_gid, all_points)))
      ঘূৰাই মিছা;

    /* composite new gids are only needed by iup delta optimization */
    যদি ((plan->flags & HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS) && glyph.is_composite ())
      plan->composite_new_gids.add (new_gid);
  }
  ঘূৰাই সঁচা;
}

template<typename DeltaSetIndexMap>
শূন্য
remap_colrv1_delta_set_index_indices (ধ্রুৱ DeltaSetIndexMap &index_map,
                                      ধ্রুৱ hb_set_t &delta_set_idxes,
                                      hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণাঙ্ক>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /* OUT */)
{
  যদি (!index_map.get_map_গুনতি ())
    ঘূৰাই;

  hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণাঙ্ক>> delta_set_idx_delta_map;
  unsigned new_delta_set_idx = 0;
  বাবে (unsigned delta_set_idx : delta_set_idxes)
  {
    unsigned var_idx = index_map.map (delta_set_idx);
    unsigned new_varidx = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    পূর্ণাঙ্ক delta = 0;

    যদি (var_idx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
    {
      hb_pair_t<unsigned, পূর্ণাঙ্ক> *new_varidx_delta;
      যদি (!variation_idx_delta_map.has (var_idx, &new_varidx_delta)) অনুৱর্ত;

      new_varidx = hb_first (*new_varidx_delta);
      delta = hb_second (*new_varidx_delta);
    }

    new_deltaset_idx_varidx_map.set (new_delta_set_idx, new_varidx);
    delta_set_idx_delta_map.set (delta_set_idx, hb_pair_t<unsigned, পূর্ণাঙ্ক> (new_delta_set_idx, delta));
    new_delta_set_idx++;
  }
  variation_idx_delta_map = std::move (delta_set_idx_delta_map);
}

template শূন্য
remap_colrv1_delta_set_index_indices<OT::DeltaSetIndexMap> (ধ্রুৱ OT::DeltaSetIndexMap &index_map,
                                      ধ্রুৱ hb_set_t &delta_set_idxes,
                                      hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণাঙ্ক>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /* OUT */);

 #endif

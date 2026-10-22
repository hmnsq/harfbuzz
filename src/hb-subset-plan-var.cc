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

 #ضمن "hb-ot-layout-common.hh"
#ضمن "hb-subset-plan.hh"

 #ضمن "hb-ot-var-common.hh"
 #ضمن "hb-ot-layout-base-table.hh"
 #ضمن "hb-ot-glyf-table.hh"
 #ضمن "hb-ot-var-fvar-table.hh"
 #ضمن "hb-ot-var-avar-table.hh"
 #ضمن "hb-ot-cff2-table.hh"

 #ifndef HB_NO_VAR

 فراغ
 generate_varstore_inner_maps (ثابت hb_set_t& varidx_set,
                               unsigned subtable_گنتی,
                               hb_vector_t<hb_inc_bimap_t> &inner_maps /* OUT */)
 {
   إذا (varidx_set.is_empty () || subtable_گنتی == 0) أرجع;

   إذا (unlikely (!inner_maps.resize (subtable_گنتی))) أرجع;
   لكل (unsigned idx : varidx_set)
   {
     uint16_t major = idx >> 16;
     uint16_t minor = idx & 0xFFFF;

     إذا (major >= subtable_گنتی)
       استمر;
     inner_maps[major].add (minor);
   }
 }

 /* Evaluate a variation-region tent at v, mirroring VarRegionAxis::evaluate
  * (invalid regions evaluate as constant 1). */
 ساكن مزدوج
 _tent_eval (ثابت Triple &tent, مزدوج v)
 {
   مزدوج start = (مزدوج) tent.minimum;
   مزدوج peak = (مزدوج) tent.درمیان;
   مزدوج end = (مزدوج) tent.maximum;
   إذا (unlikely (start > peak || peak > end)) أرجع 1.0;
   إذا (unlikely (start < 0.0 && end > 0.0 && peak != 0.0)) أرجع 1.0;
   إذا (peak == 0.0 || v == peak) أرجع 1.0;
   إذا (v <= start || end <= v) أرجع 0.0;
   إذا (v < peak) أرجع (v - start) / (peak - start);
   أرجع (end - v) / (end - peak);
 }

 /* Range of a tent's scalar over an input interval [lo, hi]. The tent is
  * unimodal, so the extremes are at the interval endpoints, plus the peak
  * when it falls inside. */
 ساكن فراغ
 tent_قدر_range (ثابت Triple &tent, مزدوج lo, مزدوج hi,
                    مزدوج *tmin, مزدوج *tmax)
 {
   مزدوج a = _tent_eval (tent, lo);
   مزدوج b = _tent_eval (tent, hi);
   *tmin = hb_min (a, b);
   *tmax = hb_max (a, b);
   إذا (lo <= (مزدوج) tent.درمیان && (مزدوج) tent.درمیان <= hi)
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
 ساكن منطقي
 _compute_avar2_reachable_ranges (hb_subset_plan_t *plan,
                                  hb_array_t<ثابت OT::AxisRecord> axes,
                                  ثابت OT::avar *avar_table,
                                  منطقي detect_self_contained)
 {
   ثابت OT::ItemVariationStore *var_store;
   ثابت OT::DeltaSetIndexMap *varidx_map;
   إذا (!avar_table->get_v2_store_and_map (&var_store, &varidx_map))
     أرجع خطأ;

   /* Old-intermediate input box per axis. */
   hb_hashmap_t<hb_tag_t, hb_pair_t<مزدوج, مزدوج>> box;
   لكل (ثابت auto &axis : axes)
   {
     hb_tag_t tag = axis.get_axis_tag ();
     مزدوج lo = -1.0, hi = +1.0;
     Triple *old_int;
     إذا (plan->user_axes_location.has (tag))
     {
       إذا (!plan->old_intermediates.has (tag, &old_int)) أرجع خطأ;
       lo = (مزدوج) old_int->minimum;
       hi = (مزدوج) old_int->maximum;
     }
     وإلا إذا (axis.is_hidden ())
       lo = hi = 0.0; /* private axis: intermediate is always 0 */
     إذا (!box.set (tag, hb_pair (lo, hi))) أرجع خطأ;
   }

   hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> regions;
   إذا (!var_store->get_region_list ().get_var_regions (plan->axes_old_index_tag_map, regions))
     أرجع خطأ;

   hb_hashmap_t<hb_tag_t, unsigned> grid_pos;
   hb_vector_t<hb_vector_t<مزدوج>> grid_points;
   لكل (unsigned i = 0; i < axes.length; i++)
   {
     grid_pos.reset ();
     grid_points.reset ();

     hb_tag_t tag = axes[i].get_axis_tag ();
     hb_pair_t<مزدوج, مزدوج> *identity;
     إذا (!box.has (tag, &identity)) أرجع خطأ;

     /* Collect the row's active regions (non-zero delta). A varidx (or the
      * implicit identity mapping) that doesn't resolve to a real store row
      * behaves as "no variation" at runtime, so it contributes no regions;
      * this also covers a NULL VarStore. */
     hb_vector_t<hb_pair_t<صحيح, unsigned>> active; /* (delta, region index) */
     uint32_t varidx = varidx_map->map (i);
     /* Runtime treats out-of-range delta-set indices as zero. */
     إذا (varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX &&
	 !var_store->has_delta_set (varidx))
       varidx = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
     إذا (varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
     {
       unsigned outer = varidx >> 16;
       unsigned inner = varidx & 0xFFFF;
       ثابت OT::VarData &vd = var_store->get_sub_table (outer);
       ثابت OT::HBUINT8 *delta_bytes = vd.get_delta_bytes ();
       unsigned row_size = vd.get_row_size ();
       unsigned region_گنتی_2 = vd.get_region_index_گنتی ();
       لكل (unsigned r = 0; r < region_گنتی_2; r++)
       {
	 صحيح delta = vd.get_item_delta_fast (inner, r, delta_bytes, row_size);
	 إذا (!delta) استمر;
	 unsigned region_idx = vd.get_region_index (r);
	 إذا (region_idx >= regions.length) أرجع خطأ;
	 active.push (hb_pair (delta, region_idx));
       }
       إذا (active.in_error ()) أرجع خطأ;
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
     منطقي exact = true;
     مزدوج dmin = 0.0, dmax = 0.0; /* delta extremes, F2Dot14 units */
     مزدوج vmin = 0.0, vmax = 0.0; /* identity + delta extremes, coord units */

     /* Grid axes: the target axis (identity term), plus every axis a valid
      * tent of an active region references. */
     auto add_grid_axis = [&] (hb_tag_t t) -> صحيح
     {
       unsigned *pos;
       إذا (grid_pos.has (t, &pos)) أرجع (صحيح) *pos;
       unsigned new_pos = grid_pos.get_population ();
       hb_pair_t<مزدوج, مزدوج> *داخلہ;
       مزدوج blo = -1.0, bhi = +1.0;
       إذا (box.has (t, &داخلہ)) { blo = داخلہ->first; bhi = داخلہ->second; }
       grid_points.push (hb_vector_t<مزدوج> ());
       إذا (grid_points.in_error ()) أرجع -1;
       auto &points = grid_points[new_pos];
       points.push (blo);
       points.push (bhi);
       إذا (points.in_error () || !grid_pos.set (t, new_pos)) أرجع -1;
       أرجع (صحيح) new_pos;
     };

     إذا (add_grid_axis (tag) < 0) أرجع خطأ;
     /* Per active region: (grid axis position, tent) for its valid tents. */
     hb_vector_t<hb_vector_t<hb_pair_t<unsigned, Triple>>> region_tents;
     لكل (ثابت auto &ar : active)
     {
       region_tents.push (hb_vector_t<hb_pair_t<unsigned, Triple>> ());
       إذا (region_tents.in_error ()) أرجع خطأ;
       auto &tents = region_tents[region_tents.length - 1];
       لكل (ثابت auto &_ : regions[ar.second])
       {
	 ثابت Triple &tent = _.second;
	 /* Tents the runtime ignores (constant scalar 1) add no breakpoints. */
	 إذا (tent.درمیان == 0.f ||
	     tent.minimum > tent.درمیان || tent.درمیان > tent.maximum ||
	     (tent.minimum < 0.f && tent.maximum > 0.f))
	   استمر;
	 صحيح pos = add_grid_axis (_.first);
	 إذا (pos < 0) أرجع خطأ;
	 auto &points = grid_points[pos];
	 مزدوج blo = points[0], bhi = points[1];
	 points.push (hb_clamp ((مزدوج) tent.minimum, blo, bhi));
	 points.push (hb_clamp ((مزدوج) tent.درمیان, blo, bhi));
	 points.push (hb_clamp ((مزدوج) tent.maximum, blo, bhi));
	 tents.push (hb_pair ((unsigned) pos, tent));
	 إذا (points.in_error () || tents.in_error ()) أرجع خطأ;
       }
     }

     unsigned grid_size = 1;
     لكل (auto &points : grid_points)
     {
       points.qsort ([] (ثابت مزدوج &a, ثابت مزدوج &b) -> صحيح
		     { أرجع a < b ? -1 : a > b ? +1 : 0; });
       unsigned n = 0;
       لكل (unsigned j = 0; j < points.length; j++)
	 إذا (!n || points.arrayZ[j] != points.arrayZ[n - 1])
	   points.arrayZ[n++] = points.arrayZ[j];
       points.shrink (n);
       إذا (grid_size > MAX_GRID / points.length) { exact = خطأ; اكسر; }
       grid_size *= points.length;
     }

     إذا (exact)
     {
       unsigned target_pos = 0; /* the target axis was added first */
       hb_vector_t<unsigned> odometer;
       إذا (!odometer.resize (grid_pos.get_population ())) أرجع خطأ;
       منطقي first = true;
       طالما (true)
       {
	 مزدوج delta_sum = 0.0;
	 لكل (unsigned r = 0; r < active.length; r++)
	 {
	   مزدوج scalar = 1.0;
	   لكل (ثابت auto &pt : region_tents[r])
	   {
	     مزدوج v = grid_points[pt.first][odometer[pt.first]];
	     scalar *= _tent_eval (pt.second, v);
	     إذا (scalar == 0.0) اكسر;
	   }
	   delta_sum += scalar * active[r].first;
	 }
	 مزدوج coord = grid_points[target_pos][odometer[target_pos]];
	 مزدوج value = coord + delta_sum / 16384.0;
	 إذا (first)
	 {
	   dmin = dmax = delta_sum;
	   vmin = vmax = value;
	   first = خطأ;
	 }
	 وإلا
	 {
	   dmin = hb_min (dmin, delta_sum); dmax = hb_max (dmax, delta_sum);
	   vmin = hb_min (vmin, value);     vmax = hb_max (vmax, value);
	 }
	 unsigned k = 0;
	 لكل (; k < odometer.length; k++)
	 {
	   إذا (++odometer[k] < grid_points[k].length) اكسر;
	   odometer[k] = 0;
	 }
	 إذا (k == odometer.length) اكسر;
       }
     }
     وإلا
     {
       لكل (ثابت auto &ar : active)
       {
	 صحيح delta = ar.first;
	 مزدوج smin = 1.0, smax = 1.0;
	 لكل (ثابت auto &_ : regions[ar.second])
	 {
	   hb_pair_t<مزدوج, مزدوج> *داخلہ;
	   مزدوج blo = -1.0, bhi = +1.0;
	   إذا (box.has (_.first, &داخلہ)) { blo = داخلہ->first; bhi = داخلہ->second; }
	   مزدوج tmin, tmax;
	   tent_قدر_range (_.second, blo, bhi, &tmin, &tmax);
	   smin *= tmin;
	   smax *= tmax;
	   إذا (smax == 0.0) اكسر;
	 }
	 إذا (delta > 0) { dmin += smin * delta; dmax += smax * delta; }
	 وإلا           { dmin += smax * delta; dmax += smin * delta; }
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
     إذا (detect_self_contained && dmin == dmax &&
	 plan->user_axes_location.has (tag, &user) && user->is_point ())
     {
       صحيح v_int = (صحيح) roundf ((عائم) identity->first * 16384.f) +
		   (صحيح) roundf ((عائم) dmin);
       v_int = hb_clamp (v_int, -(1 << 14), +(1 << 14));
       إذا (!plan->avar2_self_contained.set (tag, v_int / 16384.0)) أرجع خطأ;
       استمر; /* axis is dropped; no region can reference it afterwards */
     }

     /* Pad by one F2Dot14 unit against rounding differences with the
      * runtime's fixed-point evaluation. */
     مزدوج lo = hb_clamp (vmin - 1.0 / 16384.0, -1.0, +1.0);
     مزدوج hi = hb_clamp (vmax + 1.0 / 16384.0, -1.0, +1.0);
     lo = floor (lo * 16384.0) / 16384.0;
     hi = ceil (hi * 16384.0) / 16384.0;
     إذا (lo <= -1.0 && hi >= +1.0)
       استمر; /* not constraining */
     إذا (!plan->avar2_reachable_ranges.set (tag, Triple (lo, hb_clamp (0.0, lo, hi), hi)))
       أرجع خطأ;
   }
   أرجع true;
 }

#ifndef HB_NO_OT_FONT_CFF
 ساكن مضمن hb_font_t*
 _get_hb_font_with_variations (ثابت hb_subset_plan_t *plan)
 {
   hb_font_t *font = hb_font_create (plan->source);

   إذا (plan->has_avar2)
   {
     /* Under avar2, instancing applies only to the self-contained pins,
      * whose constant final coordinates the plan holds in normalized_coords.
      * Setting user-space variations would run the full avar2 mapping and
      * bake contributions that remain live in the instance's variations. */
     hb_font_set_var_coords_normalized (font, plan->normalized_coords.arrayZ,
                                        plan->normalized_coords.length);
     أرجع font;
   }

   hb_vector_t<hb_variation_t> vars;
   إذا (!vars.alloc (plan->user_axes_location.get_population ())) {
     hb_font_destroy (font);
     أرجع nullptr;
   }

   لكل (auto _ : plan->user_axes_location)
   {
     hb_variation_t var;
     var.tag = _.first;
     var.value = _.second.درمیان;
     vars.push (var);
   }

   hb_font_set_variations (font, vars.arrayZ, plan->user_axes_location.get_population ());
   أرجع font;
 }
#endif

 template<typename ItemVarStore>
 فراغ
 remap_variation_indices (ثابت ItemVarStore &var_store,
                          ثابت hb_set_t &variation_indices,
                          ثابت hb_vector_t<صحيح>& normalized_coords,
                          منطقي calculate_delta, /* not pinned at default */
                          منطقي no_variations, /* all axes pinned */
                          hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> &variation_idx_delta_map /* OUT */)
 {
   إذا (&var_store == &Null (OT::ItemVariationStore)) أرجع;
   unsigned subtable_گنتی = var_store.get_sub_table_گنتی ();
   auto *store_cache = var_store.create_cache ();

   unsigned new_major = 0, new_minor = 0;
   unsigned last_major = (variation_indices.get_min ()) >> 16;
   لكل (unsigned idx : variation_indices)
   {
     صحيح delta = 0;
     إذا (calculate_delta)
       delta = roundf (var_store.get_delta (idx, normalized_coords.arrayZ,
                                            normalized_coords.length, store_cache));

     إذا (no_variations)
     {
       variation_idx_delta_map.set (idx, hb_pair_t<unsigned, صحيح> (HB_OT_LAYOUT_NO_VARIATIONS_INDEX, delta));
       استمر;
     }

     uint16_t major = idx >> 16;
     إذا (major >= subtable_گنتی) اكسر;
     إذا (major != last_major)
     {
       new_minor = 0;
       ++new_major;
     }

     unsigned new_idx = (new_major << 16) + new_minor;
     variation_idx_delta_map.set (idx, hb_pair_t<unsigned, صحيح> (new_idx, delta));
     ++new_minor;
     last_major = major;
   }
   var_store.destroy_cache (store_cache);
 }

 template
 فراغ
 remap_variation_indices<OT::ItemVariationStore> (ثابت OT::ItemVariationStore &var_store,
                          ثابت hb_set_t &variation_indices,
                          ثابت hb_vector_t<صحيح>& normalized_coords,
                          منطقي calculate_delta, /* not pinned at default */
                          منطقي no_variations, /* all axes pinned */
                          hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> &variation_idx_delta_map /* OUT */);

 #ifndef HB_NO_BASE
 فراغ
 collect_base_variation_indices (hb_subset_plan_t* plan)
 {
   hb_blob_ptr_t<OT::BASE> base = plan->source_table<OT::BASE> ();
   إذا (!base->has_var_store ())
   {
     base.destroy ();
     أرجع;
   }

   hb_set_t varidx_set;
   base->collect_variation_indices (plan, varidx_set);
   ثابت OT::ItemVariationStore &var_store = base->get_var_store ();
   unsigned subtable_گنتی = var_store.get_sub_table_گنتی ();


   remap_variation_indices (var_store, varidx_set,
                             plan->normalized_coords,
                             !plan->pinned_at_default,
                             plan->all_axes_pinned,
                             plan->base_variation_idx_map);
   generate_varstore_inner_maps (varidx_set, subtable_گنتی, plan->base_varstore_inner_maps);

   base.destroy ();
 }

 #endif

منطقي
normalize_axes_location (hb_face_t *face, hb_subset_plan_t *plan)
{
  إذا (plan->user_axes_location.is_empty ())
    أرجع true;

  hb_array_t<ثابت OT::AxisRecord> axes = face->table.fvar->get_axes ();
  لكل (ثابت auto &axis : axes)
  {
    hb_tag_t tag = axis.get_axis_tag ();
    ثابت Triple *range;
    عائم minimum, درمیان, maximum;
    axis.get_coordinates (minimum, درمیان, maximum);
    إذا (plan->user_axes_location.has (tag, &range) && !range->is_point () &&
        range->minimum == minimum &&
        range->درمیان == درمیان &&
        range->maximum == maximum)
      plan->user_axes_location.del (tag);
  }
  إذا (plan->user_axes_location.is_empty ())
    أرجع true;

  إذا (!plan->check_success (plan->normalized_coords.resize (axes.length)))
    أرجع خطأ;

  منطقي has_avar = face->table.avar->has_data ();
  hb_vector_t<عائم> normalized_mins;
  hb_vector_t<عائم> normalized_defaults;
  hb_vector_t<عائم> normalized_maxs;
  إذا (has_avar)
  {
    إذا (!plan->check_success (normalized_mins.resize (axes.length)) ||
        !plan->check_success (normalized_defaults.resize (axes.length)) ||
        !plan->check_success (normalized_maxs.resize (axes.length)))
      أرجع خطأ;
  }

  منطقي axis_not_pinned = خطأ;
  unsigned new_axis_idx = 0;
  unsigned last_idx = 0;
  لكل (ثابت auto& _ : + hb_enumerate (axes))
  {
    unsigned i = _.first;
    ثابت OT::AxisRecord &axis = _.second;
    hb_tag_t axis_tag = axis.get_axis_tag ();
    plan->axes_old_index_tag_map.set (i, axis_tag);

    إذا (!plan->user_axes_location.has (axis_tag) ||
        !plan->user_axes_location.get (axis_tag).is_point ())
    {
      axis_not_pinned = true;
      plan->axes_index_map.set (i, new_axis_idx);
      plan->axis_tags.push (axis_tag);
      new_axis_idx++;
    }

    Triple *axis_range;
    إذا (plan->user_axes_location.has (axis_tag, &axis_range))
    {
      plan->axes_triple_distances.set (axis_tag, axis.get_triple_distances ());

      عائم normalized_min = axis.normalize_axis_قدر (axis_range->minimum);
      عائم normalized_default = axis.normalize_axis_قدر (axis_range->درمیان);
      عائم normalized_max = axis.normalize_axis_قدر (axis_range->maximum);

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

      إذا (has_avar)
      {
        normalized_mins[i] = normalized_min;
        normalized_defaults[i] = normalized_default;
        normalized_maxs[i] = normalized_max;
        last_idx = i;
      }
      وإلا
      {
        plan->axes_location.set (axis_tag, Triple ((مزدوج) normalized_min,
                                                   (مزدوج) normalized_default,
                                                   (مزدوج) normalized_max));
        إذا (normalized_default == -0.f)
          normalized_default = 0.f; // Normalize -0 to 0
        إذا (normalized_default != 0.f)
          plan->pinned_at_default = خطأ;

        plan->normalized_coords[i] = roundf (normalized_default * 16384.f);
      }
    }
  }
  plan->all_axes_pinned = !axis_not_pinned;

  // TODO: use avar map_coords_16_16() when normalization is changed to 16.16
  // in fonttools
  إذا (has_avar)
  {
    ثابت OT::avar* avar_table = face->table.avar;
    unsigned coords_len = last_idx + 1;

    إذا (avar_table->has_v2_data () && !plan->all_axes_pinned)
    {
      /* avar2 partial instancing: use v1-only mapping to get intermediate-space
       * coords. These are used for IVS rebasing and offset compensation. */
      plan->has_avar2 = true;

      إذا (!plan->check_success (avar_table->map_coords_2_14 (normalized_mins.arrayZ, coords_len, true)) ||
          !plan->check_success (avar_table->map_coords_2_14 (normalized_defaults.arrayZ, coords_len, true)) ||
          !plan->check_success (avar_table->map_coords_2_14 (normalized_maxs.arrayZ, coords_len, true)))
        أرجع خطأ;

      لكل (ثابت auto& _ : + hb_enumerate (axes))
      {
        unsigned i = _.first;
        hb_tag_t axis_tag = _.second.get_axis_tag ();
        إذا (plan->user_axes_location.has (axis_tag))
        {
          /* Store intermediate-space coords for offset compensation,
           * segment-map renormalization, and IVS rebasing. */
          plan->old_intermediates.set (axis_tag, Triple ((مزدوج) normalized_mins[i],
                                                         (مزدوج) normalized_defaults[i],
                                                         (مزدوج) normalized_maxs[i]));
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
      منطقي detect_self_contained = !hb_blob_get_length (cff2_blob);
      hb_blob_destroy (cff2_blob);
      إذا (!_compute_avar2_reachable_ranges (plan, axes, avar_table,
                                            detect_self_contained))
        أرجع خطأ;

#ifndef HB_NO_VAR_COMPOSITES
      /* VARC overrides can reach coordinates outside the font-level
       * ranges. Do not cull variation data using those ranges. */
      إذا (!plan->drop_tables.has (HB_TAG ('V','A','R','C')))
      {
        hb_blob_t *varc_blob = hb_face_reference_table (face, HB_TAG ('V','A','R','C'));
        إذا (hb_blob_get_length (varc_blob))
          plan->avar2_reachable_ranges.reset ();
        hb_blob_destroy (varc_blob);
      }
#endif

      /* Keep all axes in fvar (pinned ones as hidden), EXCEPT self-contained
       * pinned axes, which are removed entirely. */
      plan->axes_index_map.reset ();
      plan->axis_tags.reset ();
      unsigned retained_axis_idx = 0;
      لكل (ثابت auto& _ : + hb_enumerate (axes))
      {
        unsigned i = _.first;
        hb_tag_t axis_tag = _.second.get_axis_tag ();
        إذا (plan->avar2_self_contained.has (axis_tag))
          استمر;
        plan->axes_index_map.set (i, retained_axis_idx++);
        plan->axis_tags.push (axis_tag);
      }
      /* The other tables see ONLY the self-contained pins, as an ordinary
       * partial instancing in old final-coordinate space; the standard
       * instancing machinery bakes those axes' contributions in. The
       * remaining restriction is carried entirely by avar. */
      plan->axes_location.reset ();
      إذا (plan->avar2_self_contained.get_population ())
      {
        منطقي sc_pinned_at_default = true;
        لكل (auto _ : plan->avar2_self_contained)
        {
          plan->axes_location.set (_.first, Triple (_.second, _.second, _.second));
          إذا (_.second != 0.0)
            sc_pinned_at_default = خطأ;
        }
        لكل (ثابت auto& _ : + hb_enumerate (axes))
        {
          مزدوج *v;
          plan->normalized_coords[_.first] =
              plan->avar2_self_contained.has (_.second.get_axis_tag (), &v)
              ? (صحيح) roundf ((عائم) (*v * 16384.0)) : 0;
        }
        plan->pinned_at_default = sc_pinned_at_default;
      }
      وإلا
        plan->normalized_coords.shrink (0);
    }
    وإلا
    {
      /* Standard avar v1 (or v1+v2 with all axes pinned) */
      إذا (!plan->check_success (avar_table->map_coords_2_14 (normalized_mins.arrayZ, coords_len)) ||
          !plan->check_success (avar_table->map_coords_2_14 (normalized_defaults.arrayZ, coords_len)) ||
          !plan->check_success (avar_table->map_coords_2_14 (normalized_maxs.arrayZ, coords_len)))
        أرجع خطأ;

      لكل (ثابت auto& _ : + hb_enumerate (axes))
      {
        unsigned i = _.first;
        hb_tag_t axis_tag = _.second.get_axis_tag ();
        إذا (plan->user_axes_location.has (axis_tag))
        {
          plan->axes_location.set (axis_tag, Triple ((مزدوج) normalized_mins[i],
                                                     (مزدوج) normalized_defaults[i],
                                                     (مزدوج) normalized_maxs[i]));
          عائم normalized_default = normalized_defaults[i];
          إذا (normalized_default == -0.f)
            normalized_default = 0.f;
          إذا (normalized_default != 0.f)
            plan->pinned_at_default = خطأ;

          plan->normalized_coords[i] = roundf (normalized_default * 16384.f);
        }
      }
    }
  }
  أرجع true;
}

#ifndef HB_NO_OT_FONT_CFF
فراغ
update_instance_metrics_map_from_cff2 (hb_subset_plan_t *plan)
{
  إذا (!plan->normalized_coords) أرجع;
  OT::cff2::accelerator_t cff2 (plan->source);
  إذا (!cff2.is_valid ()) أرجع;

  hb_font_t *font = _get_hb_font_with_variations (plan);
  إذا (unlikely (!plan->check_success (font != nullptr)))
  {
    hb_font_destroy (font);
    أرجع;
  }

  hb_glyph_extents_t extents = {0x7FFF, -0x7FFF};
  OT::hmtx_accelerator_t _hmtx (plan->source);
  OT::hb_scalar_cache_t *hvar_store_cache = nullptr;
  إذا (_hmtx.has_data () && _hmtx.var_table.get_length ())
    hvar_store_cache = _hmtx.var_table->get_var_store ().create_cache ();

  OT::vmtx_accelerator_t _vmtx (plan->source);
  OT::hb_scalar_cache_t *vvar_store_cache = nullptr;
  إذا (_vmtx.has_data () && _vmtx.var_table.get_length ())
    vvar_store_cache = _vmtx.var_table->get_var_store ().create_cache ();

  لكل (auto p : *plan->glyph_map)
  {
    hb_codepoint_t old_gid = p.first;
    hb_codepoint_t new_gid = p.second;
    إذا (!cff2.get_extents (font, old_gid, &extents)) استمر;
    منطقي has_bounds_info = true;
    إذا (extents.x_bearing == 0 && extents.width == 0 &&
        extents.height == 0 && extents.y_bearing == 0)
      has_bounds_info = خطأ;

    إذا (has_bounds_info)
    {
      plan->head_maxp_info.xMin = hb_min (plan->head_maxp_info.xMin, extents.x_bearing);
      plan->head_maxp_info.xMax = hb_max (plan->head_maxp_info.xMax,
						 hb_saturate_add (extents.x_bearing, extents.width));
      plan->head_maxp_info.yMax = hb_max (plan->head_maxp_info.yMax, extents.y_bearing);
      plan->head_maxp_info.yMin = hb_min (plan->head_maxp_info.yMin,
						 hb_saturate_add (extents.y_bearing, extents.height));
    }

    إذا (_hmtx.has_data ())
    {
      مزدوج hori_aw = _hmtx.get_advance_without_var_unscaled (old_gid);
      إذا (_hmtx.var_table.get_length ())
        hori_aw += (مزدوج) roundf (_hmtx.var_table->get_advance_delta_unscaled (old_gid, font->coords, font->عدد_coords,
									 hvar_store_cache));
      /* Malicious deltas can take the advance out of the UFWORD range that
       * hmtx serialization stores.  Clamp, to keep downstream consumers
       * (e.g. CFF width optimizer) bounded and consistent with hmtx. */
      hori_aw = hb_clamp (hori_aw, 0., 65535.);
      صحيح lsb = extents.x_bearing;
      إذا (!has_bounds_info)
      {
        _hmtx.get_leading_bearing_without_var_unscaled (old_gid, &lsb);
      }
      plan->hmtx_map.set (new_gid, hb_pair ((unsigned) hori_aw, lsb));
      plan->bounds_width_vec[new_gid] = extents.width;
    }

    إذا (_vmtx.has_data ())
    {
      مزدوج vert_aw = _vmtx.get_advance_without_var_unscaled (old_gid);
      إذا (_vmtx.var_table.get_length ())
        vert_aw += (مزدوج) roundf (_vmtx.var_table->get_advance_delta_unscaled (old_gid, font->coords, font->عدد_coords,
									 vvar_store_cache));
      vert_aw = hb_clamp (vert_aw, 0., 65535.);
      hb_position_t vorg_x = 0;
      hb_position_t vorg_y = 0;
      صحيح tsb = 0;
      إذا (has_bounds_info &&
           hb_font_get_glyph_v_origin (font, old_gid, &vorg_x, &vorg_y))
      {
        tsb = hb_saturate_sub (vorg_y, extents.y_bearing);
      } وإلا {
        _vmtx.get_leading_bearing_without_var_unscaled (old_gid, &tsb);
      }

      plan->vmtx_map.set (new_gid, hb_pair ((unsigned) vert_aw, tsb));
      plan->bounds_height_vec[new_gid] = extents.height;
    }
  }
  /* No glyph contributed bounds: do not serialize the accumulator's
   * inverted sentinels into head (or a downgraded CFF FontBBox). */
  إذا (plan->head_maxp_info.xMin > plan->head_maxp_info.xMax)
  {
    plan->head_maxp_info.xMin = plan->head_maxp_info.xMax = 0;
    plan->head_maxp_info.yMin = plan->head_maxp_info.yMax = 0;
  }
  hb_font_destroy (font);
  إذا (hvar_store_cache)
    _hmtx.var_table->get_var_store ().destroy_cache (hvar_store_cache);
  إذا (vvar_store_cache)
    _vmtx.var_table->get_var_store ().destroy_cache (vvar_store_cache);
}
#endif

منطقي
get_instance_glyphs_contour_points (hb_subset_plan_t *plan)
{
  /* contour_points vector only needed for updating gvar table (infer delta and
   * iup delta optimization) during partial instancing */
  إذا (plan->user_axes_location.is_empty () || plan->all_axes_pinned)
    أرجع true;

  OT::glyf_accelerator_t glyf (plan->source);

  لكل (auto &_ : plan->new_to_old_gid_list)
  {
    hb_codepoint_t new_gid = _.first;
    contour_point_vector_t all_points;
    إذا (new_gid == 0 && !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      إذا (unlikely (!plan->new_gid_contour_points_map.set (new_gid, all_points)))
        أرجع خطأ;
      استمر;
    }

    hb_codepoint_t old_gid = _.second;
    auto glyph = glyf.glyph_for_gid (old_gid);
    إذا (unlikely (!glyph.get_all_points_without_var (plan->source, all_points)))
      أرجع خطأ;
    إذا (unlikely (!plan->new_gid_contour_points_map.set (new_gid, all_points)))
      أرجع خطأ;

    /* composite new gids are only needed by iup delta optimization */
    إذا ((plan->flags & HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS) && glyph.is_composite ())
      plan->composite_new_gids.add (new_gid);
  }
  أرجع true;
}

template<typename DeltaSetIndexMap>
فراغ
remap_colrv1_delta_set_index_indices (ثابت DeltaSetIndexMap &index_map,
                                      ثابت hb_set_t &delta_set_idxes,
                                      hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /* OUT */)
{
  إذا (!index_map.get_map_گنتی ())
    أرجع;

  hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> delta_set_idx_delta_map;
  unsigned new_delta_set_idx = 0;
  لكل (unsigned delta_set_idx : delta_set_idxes)
  {
    unsigned var_idx = index_map.map (delta_set_idx);
    unsigned new_varidx = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    صحيح delta = 0;

    إذا (var_idx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
    {
      hb_pair_t<unsigned, صحيح> *new_varidx_delta;
      إذا (!variation_idx_delta_map.has (var_idx, &new_varidx_delta)) استمر;

      new_varidx = hb_first (*new_varidx_delta);
      delta = hb_second (*new_varidx_delta);
    }

    new_deltaset_idx_varidx_map.set (new_delta_set_idx, new_varidx);
    delta_set_idx_delta_map.set (delta_set_idx, hb_pair_t<unsigned, صحيح> (new_delta_set_idx, delta));
    new_delta_set_idx++;
  }
  variation_idx_delta_map = std::move (delta_set_idx_delta_map);
}

template فراغ
remap_colrv1_delta_set_index_indices<OT::DeltaSetIndexMap> (ثابت OT::DeltaSetIndexMap &index_map,
                                      ثابت hb_set_t &delta_set_idxes,
                                      hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /* OUT */);

 #endif

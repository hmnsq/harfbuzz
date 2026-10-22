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

 #అన్తర్భావ "hb-ot-layout-common.hh"
#అన్తర్భావ "hb-subset-plan.hh"

 #అన్తర్భావ "hb-ot-var-common.hh"
 #అన్తర్భావ "hb-ot-layout-base-table.hh"
 #అన్తర్భావ "hb-ot-glyf-table.hh"
 #అన్తర్భావ "hb-ot-var-fvar-table.hh"
 #అన్తర్భావ "hb-ot-var-avar-table.hh"
 #అన్తర్భావ "hb-ot-cff2-table.hh"

 #నిర్వచితం_కాకపోతే HB_NO_VAR

 శూన్య
 generate_varstore_inner_maps (స్థిరరాశి hb_సమితి_t& varidx_సమితి,
                               గుర్తులేని subtable_లెక్క,
                               hb_సదిశ_t<hb_inc_bimap_t> &inner_maps /*బహిర్గతం*/)
 {
   ఒకవేళ (varidx_సమితి.is_empty () || subtable_లెక్క == 0) తిరిగి;

   ఒకవేళ (unlikely (!inner_maps.resize (subtable_లెక్క))) తిరిగి;
   కొరకు (గుర్తులేని idx : varidx_సమితి)
   {
     uint16_t major = idx >> 16;
     uint16_t minor = idx & 0xFFFF;

     ఒకవేళ (major >= subtable_లెక్క)
       కొనసాగు;
     inner_maps[major].కూడు (minor);
   }
 }

 /* Evaluate a variation-region tent at v, mirroring VarRegionAxis::evaluate
  * (invalid regions evaluate as constant 1). */
 స్థిర ద్విగుణ
 _tent_eval (స్థిరరాశి Triple &tent, ద్విగుణ v)
 {
   ద్విగుణ ప్రారంభం = (ద్విగుణ) tent.minimum;
   ద్విగుణ peak = (ద్విగుణ) tent.మధ్య;
   ద్విగుణ చివర = (ద్విగుణ) tent.గరిష్ఠ;
   ఒకవేళ (unlikely (ప్రారంభం > peak || peak > చివర)) తిరిగి 1.0;
   ఒకవేళ (unlikely (ప్రారంభం < 0.0 && చివర > 0.0 && peak != 0.0)) తిరిగి 1.0;
   ఒకవేళ (peak == 0.0 || v == peak) తిరిగి 1.0;
   ఒకవేళ (v <= ప్రారంభం || చివర <= v) తిరిగి 0.0;
   ఒకవేళ (v < peak) తిరిగి (v - ప్రారంభం) / (peak - ప్రారంభం);
   తిరిగి (చివర - v) / (చివర - peak);
 }

 /* Range of a tent's scalar over an input interval [lo, hi]. The tent is
  * unimodal, so the extremes are at the interval endpoints, plus the peak
  * when it falls inside. */
 స్థిర శూన్య
 tent_విలువ_range (స్థిరరాశి Triple &tent, ద్విగుణ lo, ద్విగుణ hi,
                    ద్విగుణ *tmin, ద్విగుణ *tmax)
 {
   ద్విగుణ a = _tent_eval (tent, lo);
   ద్విగుణ b = _tent_eval (tent, hi);
   *tmin = hb_min (a, b);
   *tmax = hb_max (a, b);
   ఒకవేళ (lo <= (ద్విగుణ) tent.మధ్య && (ద్విగుణ) tent.మధ్య <= hi)
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
 స్థిర తార్కిక
 _compute_avar2_reachable_ranges (hb_subset_plan_t *plan,
                                  hb_array_t<స్థిరరాశి OT::Axis_రికార్డు> axes,
                                  స్థిరరాశి OT::avar *avar_పట్టిక,
                                  తార్కిక detect_self_contained)
 {
   స్థిరరాశి OT::అంశం_Variation_Store *var_store;
   స్థిరరాశి OT::Delta_సమితి_సూచీ_Map *varidx_map;
   ఒకవేళ (!avar_పట్టిక->get_v2_store_and_map (&var_store, &varidx_map))
     తిరిగి అబద్ధం;

   /* Old-intermediate input box per axis. */
   hb_hashmap_t<hb_ట్యాగ్_t, hb_pair_t<ద్విగుణ, ద్విగుణ>> box;
   కొరకు (స్థిరరాశి స్వయంచాలక &axis : axes)
   {
     hb_ట్యాగ్_t ట్యాగ్_2 = axis.get_axis_ట్యాగ్ ();
     ద్విగుణ lo = -1.0, hi = +1.0;
     Triple *old_int;
     ఒకవేళ (plan->user_axes_location.has (ట్యాగ్_2))
     {
       ఒకవేళ (!plan->old_intermediates.has (ట్యాగ్_2, &old_int)) తిరిగి అబద్ధం;
       lo = (ద్విగుణ) old_int->minimum;
       hi = (ద్విగుణ) old_int->గరిష్ఠ;
     }
     లేకపోతే ఒకవేళ (axis.is_hidden ())
       lo = hi = 0.0; /* private axis: intermediate is always 0 */
     ఒకవేళ (!box.సమితి (ట్యాగ్_2, hb_pair (lo, hi))) తిరిగి అబద్ధం;
   }

   hb_సదిశ_t<hb_hashmap_t<hb_ట్యాగ్_t, Triple>> regions;
   ఒకవేళ (!var_store->get_region_list ().get_var_regions (plan->axes_old_సూచీ_ట్యాగ్_map, regions))
     తిరిగి అబద్ధం;

   hb_hashmap_t<hb_ట్యాగ్_t, గుర్తులేని> grid_pos;
   hb_సదిశ_t<hb_సదిశ_t<ద్విగుణ>> grid_points;
   కొరకు (గుర్తులేని i = 0; i < axes.నిడివి; i++)
   {
     grid_pos.reset ();
     grid_points.reset ();

     hb_ట్యాగ్_t ట్యాగ్_2 = axes[i].get_axis_ట్యాగ్ ();
     hb_pair_t<ద్విగుణ, ద్విగుణ> *తటస్థం;
     ఒకవేళ (!box.has (ట్యాగ్_2, &తటస్థం)) తిరిగి అబద్ధం;

     /* Collect the row's active regions (non-zero delta). A varidx (or the
      * implicit identity mapping) that doesn't resolve to a real store row
      * behaves as "no variation" at runtime, so it contributes no regions;
      * this also covers a NULL VarStore. */
     hb_సదిశ_t<hb_pair_t<పూర్ణాంకం, గుర్తులేని>> చురుకైన; /* (delta, region index) */
     uint32_t varidx = varidx_map->map (i);
     /* Runtime treats out-of-range delta-set indices as zero. */
     ఒకవేళ (varidx != HB_OT_LAYOUT_NO_VARIATIONS_సూచీ &&
	 !var_store->has_delta_సమితి (varidx))
       varidx = HB_OT_LAYOUT_NO_VARIATIONS_సూచీ;
     ఒకవేళ (varidx != HB_OT_LAYOUT_NO_VARIATIONS_సూచీ)
     {
       గుర్తులేని outer = varidx >> 16;
       గుర్తులేని inner = varidx & 0xFFFF;
       స్థిరరాశి OT::Var_దత్తాంశం &vd = var_store->get_sub_పట్టిక (outer);
       స్థిరరాశి OT::HBUINT8 *delta_బైట్లు = vd.get_delta_బైట్లు ();
       గుర్తులేని అడ్డువరుస_కొలత = vd.get_అడ్డువరుస_కొలత ();
       గుర్తులేని region_లెక్క_2 = vd.get_region_సూచీ_లెక్క ();
       కొరకు (గుర్తులేని r = 0; r < region_లెక్క_2; r++)
       {
	 పూర్ణాంకం delta = vd.get_అంశం_delta_fast (inner, r, delta_బైట్లు, అడ్డువరుస_కొలత);
	 ఒకవేళ (!delta) కొనసాగు;
	 గుర్తులేని region_idx = vd.get_region_సూచీ (r);
	 ఒకవేళ (region_idx >= regions.నిడివి) తిరిగి అబద్ధం;
	 చురుకైన.push (hb_pair (delta, region_idx));
       }
       ఒకవేళ (చురుకైన.లోపల_error ()) తిరిగి అబద్ధం;
     }

     /* The delta is piecewise multilinear in the axis coordinates (each
      * region scalar is a product of independent per-axis tents), so over
      * the box its exact extremes are attained at vertices of the grid of
      * per-axis tent breakpoints. Enumerate that grid when it is small
      * (real fonts); otherwise fall back to per-region interval
      * arithmetic, which ignores correlations between regions and is
      * looser but still conservative. Mirrors fontTools'
      * VarStore.getExtremes. */
     constexpr గుర్తులేని MAX_GRID = 1u << 14;
     తార్కిక exact = నిజం;
     ద్విగుణ dmin = 0.0, dmax = 0.0; /* delta extremes, F2Dot14 units */
     ద్విగుణ vmin = 0.0, vmax = 0.0; /* identity + delta extremes, coord units */

     /* Grid axes: the target axis (identity term), plus every axis a valid
      * tent of an active region references. */
     స్వయంచాలక కూడు_grid_axis = [&] (hb_ట్యాగ్_t t) -> పూర్ణాంకం
     {
       గుర్తులేని *pos;
       ఒకవేళ (grid_pos.has (t, &pos)) తిరిగి (పూర్ణాంకం) *pos;
       గుర్తులేని new_pos = grid_pos.get_population ();
       hb_pair_t<ద్విగుణ, ద్విగుణ> *ఆదానం;
       ద్విగుణ blo = -1.0, bhi = +1.0;
       ఒకవేళ (box.has (t, &ఆదానం)) { blo = ఆదానం->మొదటి; bhi = ఆదానం->second; }
       grid_points.push (hb_సదిశ_t<ద్విగుణ> ());
       ఒకవేళ (grid_points.లోపల_error ()) తిరిగి -1;
       స్వయంచాలక &points = grid_points[new_pos];
       points.push (blo);
       points.push (bhi);
       ఒకవేళ (points.లోపల_error () || !grid_pos.సమితి (t, new_pos)) తిరిగి -1;
       తిరిగి (పూర్ణాంకం) new_pos;
     };

     ఒకవేళ (కూడు_grid_axis (ట్యాగ్_2) < 0) తిరిగి అబద్ధం;
     /* Per active region: (grid axis position, tent) for its valid tents. */
     hb_సదిశ_t<hb_సదిశ_t<hb_pair_t<గుర్తులేని, Triple>>> region_tents;
     కొరకు (స్థిరరాశి స్వయంచాలక &ar : చురుకైన)
     {
       region_tents.push (hb_సదిశ_t<hb_pair_t<గుర్తులేని, Triple>> ());
       ఒకవేళ (region_tents.లోపల_error ()) తిరిగి అబద్ధం;
       స్వయంచాలక &tents = region_tents[region_tents.నిడివి - 1];
       కొరకు (స్థిరరాశి స్వయంచాలక &_ : regions[ar.second])
       {
	 స్థిరరాశి Triple &tent = _.second;
	 /* Tents the runtime ignores (constant scalar 1) add no breakpoints. */
	 ఒకవేళ (tent.మధ్య == 0.f ||
	     tent.minimum > tent.మధ్య || tent.మధ్య > tent.గరిష్ఠ ||
	     (tent.minimum < 0.f && tent.గరిష్ఠ > 0.f))
	   కొనసాగు;
	 పూర్ణాంకం pos = కూడు_grid_axis (_.మొదటి);
	 ఒకవేళ (pos < 0) తిరిగి అబద్ధం;
	 స్వయంచాలక &points = grid_points[pos];
	 ద్విగుణ blo = points[0], bhi = points[1];
	 points.push (hb_clamp ((ద్విగుణ) tent.minimum, blo, bhi));
	 points.push (hb_clamp ((ద్విగుణ) tent.మధ్య, blo, bhi));
	 points.push (hb_clamp ((ద్విగుణ) tent.గరిష్ఠ, blo, bhi));
	 tents.push (hb_pair ((గుర్తులేని) pos, tent));
	 ఒకవేళ (points.లోపల_error () || tents.లోపల_error ()) తిరిగి అబద్ధం;
       }
     }

     గుర్తులేని grid_కొలత = 1;
     కొరకు (స్వయంచాలక &points : grid_points)
     {
       points.qsort ([] (స్థిరరాశి ద్విగుణ &a, స్థిరరాశి ద్విగుణ &b) -> పూర్ణాంకం
		     { తిరిగి a < b ? -1 : a > b ? +1 : 0; });
       గుర్తులేని n = 0;
       కొరకు (గుర్తులేని j = 0; j < points.నిడివి; j++)
	 ఒకవేళ (!n || points.arrayZ[j] != points.arrayZ[n - 1])
	   points.arrayZ[n++] = points.arrayZ[j];
       points.shrink (n);
       ఒకవేళ (grid_కొలత > MAX_GRID / points.నిడివి) { exact = అబద్ధం; ఆపు; }
       grid_కొలత *= points.నిడివి;
     }

     ఒకవేళ (exact)
     {
       గుర్తులేని లక్ష్యం_pos = 0; /* the target axis was added first */
       hb_సదిశ_t<గుర్తులేని> odometer;
       ఒకవేళ (!odometer.resize (grid_pos.get_population ())) తిరిగి అబద్ధం;
       తార్కిక మొదటి = నిజం;
       అయితే (నిజం)
       {
	 ద్విగుణ delta_కూడిక = 0.0;
	 కొరకు (గుర్తులేని r = 0; r < చురుకైన.నిడివి; r++)
	 {
	   ద్విగుణ scalar = 1.0;
	   కొరకు (స్థిరరాశి స్వయంచాలక &pt : region_tents[r])
	   {
	     ద్విగుణ v = grid_points[pt.మొదటి][odometer[pt.మొదటి]];
	     scalar *= _tent_eval (pt.second, v);
	     ఒకవేళ (scalar == 0.0) ఆపు;
	   }
	   delta_కూడిక += scalar * చురుకైన[r].మొదటి;
	 }
	 ద్విగుణ coord = grid_points[లక్ష్యం_pos][odometer[లక్ష్యం_pos]];
	 ద్విగుణ value = coord + delta_కూడిక / 16384.0;
	 ఒకవేళ (మొదటి)
	 {
	   dmin = dmax = delta_కూడిక;
	   vmin = vmax = value;
	   మొదటి = అబద్ధం;
	 }
	 లేకపోతే
	 {
	   dmin = hb_min (dmin, delta_కూడిక); dmax = hb_max (dmax, delta_కూడిక);
	   vmin = hb_min (vmin, value);     vmax = hb_max (vmax, value);
	 }
	 గుర్తులేని k = 0;
	 కొరకు (; k < odometer.నిడివి; k++)
	 {
	   ఒకవేళ (++odometer[k] < grid_points[k].నిడివి) ఆపు;
	   odometer[k] = 0;
	 }
	 ఒకవేళ (k == odometer.నిడివి) ఆపు;
       }
     }
     లేకపోతే
     {
       కొరకు (స్థిరరాశి స్వయంచాలక &ar : చురుకైన)
       {
	 పూర్ణాంకం delta = ar.మొదటి;
	 ద్విగుణ smin = 1.0, smax = 1.0;
	 కొరకు (స్థిరరాశి స్వయంచాలక &_ : regions[ar.second])
	 {
	   hb_pair_t<ద్విగుణ, ద్విగుణ> *ఆదానం;
	   ద్విగుణ blo = -1.0, bhi = +1.0;
	   ఒకవేళ (box.has (_.మొదటి, &ఆదానం)) { blo = ఆదానం->మొదటి; bhi = ఆదానం->second; }
	   ద్విగుణ tmin, tmax;
	   tent_విలువ_range (_.second, blo, bhi, &tmin, &tmax);
	   smin *= tmin;
	   smax *= tmax;
	   ఒకవేళ (smax == 0.0) ఆపు;
	 }
	 ఒకవేళ (delta > 0) { dmin += smin * delta; dmax += smax * delta; }
	 లేకపోతే           { dmin += smax * delta; dmax += smin * delta; }
       }
       vmin = తటస్థం->మొదటి + dmin / 16384.0;
       vmax = తటస్థం->second + dmax / 16384.0;
     }
     /* A pinned axis whose delta is constant over the box is self-contained:
      * its final coordinate is a constant. It can be removed from fvar/avar
      * and its contribution baked into the variation tables like an ordinary
      * pin at that coordinate (in old final space). Round the delta alone
      * and add it to the exact quantized intermediate, matching the runtime
      * (map_coords_2_14) and fontTools. */
     Triple *user;
     ఒకవేళ (detect_self_contained && dmin == dmax &&
	 plan->user_axes_location.has (ట్యాగ్_2, &user) && user->is_బిందువు ())
     {
       పూర్ణాంకం v_int = (పూర్ణాంకం) roundf ((దశాంశం) తటస్థం->మొదటి * 16384.f) +
		   (పూర్ణాంకం) roundf ((దశాంశం) dmin);
       v_int = hb_clamp (v_int, -(1 << 14), +(1 << 14));
       ఒకవేళ (!plan->avar2_self_contained.సమితి (ట్యాగ్_2, v_int / 16384.0)) తిరిగి అబద్ధం;
       కొనసాగు; /* axis is dropped; no region can reference it afterwards */
     }

     /* Pad by one F2Dot14 unit against rounding differences with the
      * runtime's fixed-point evaluation. */
     ద్విగుణ lo = hb_clamp (vmin - 1.0 / 16384.0, -1.0, +1.0);
     ద్విగుణ hi = hb_clamp (vmax + 1.0 / 16384.0, -1.0, +1.0);
     lo = floor (lo * 16384.0) / 16384.0;
     hi = ceil (hi * 16384.0) / 16384.0;
     ఒకవేళ (lo <= -1.0 && hi >= +1.0)
       కొనసాగు; /* not constraining */
     ఒకవేళ (!plan->avar2_reachable_ranges.సమితి (ట్యాగ్_2, Triple (lo, hb_clamp (0.0, lo, hi), hi)))
       తిరిగి అబద్ధం;
   }
   తిరిగి నిజం;
 }

#నిర్వచితం_కాకపోతే HB_NO_OT_FONT_CFF
 స్థిర అంతఃవరుస hb_font_t*
 _get_hb_font_with_variations (స్థిరరాశి hb_subset_plan_t *plan)
 {
   hb_font_t *font = hb_font_create (plan->ఆధారం);

   ఒకవేళ (plan->has_avar2)
   {
     /* Under avar2, instancing applies only to the self-contained pins,
      * whose constant final coordinates the plan holds in normalized_coords.
      * Setting user-space variations would run the full avar2 mapping and
      * bake contributions that remain live in the instance's variations. */
     hb_font_సమితి_var_coords_normalized (font, plan->normalized_coords.arrayZ,
                                        plan->normalized_coords.నిడివి);
     తిరిగి font;
   }

   hb_సదిశ_t<hb_variation_t> vars;
   ఒకవేళ (!vars.alloc (plan->user_axes_location.get_population ())) {
     hb_font_destroy (font);
     తిరిగి శూన్యసూచిక;
   }

   కొరకు (స్వయంచాలక _ : plan->user_axes_location)
   {
     hb_variation_t var;
     var.ట్యాగ్_2 = _.మొదటి;
     var.value = _.second.మధ్య;
     vars.push (var);
   }

   hb_font_సమితి_variations (font, vars.arrayZ, plan->user_axes_location.get_population ());
   తిరిగి font;
 }
#అయితే_ముగింపు

 మూస<రకం_పేరు అంశం_Var_Store>
 శూన్య
 remap_variation_indices (స్థిరరాశి అంశం_Var_Store &var_store,
                          స్థిరరాశి hb_సమితి_t &variation_indices,
                          స్థిరరాశి hb_సదిశ_t<పూర్ణాంకం>& normalized_coords,
                          తార్కిక calculate_delta, /* not pinned at default */
                          తార్కిక no_variations, /* all axes pinned */
                          hb_hashmap_t<గుర్తులేని, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> &variation_idx_delta_map /*బహిర్గతం*/)
 {
   ఒకవేళ (&var_store == &Null (OT::అంశం_Variation_Store)) తిరిగి;
   గుర్తులేని subtable_లెక్క = var_store.get_sub_పట్టిక_లెక్క ();
   స్వయంచాలక *store_cache = var_store.create_cache ();

   గుర్తులేని new_major = 0, new_minor = 0;
   గుర్తులేని చివరి_major = (variation_indices.get_min ()) >> 16;
   కొరకు (గుర్తులేని idx : variation_indices)
   {
     పూర్ణాంకం delta = 0;
     ఒకవేళ (calculate_delta)
       delta = roundf (var_store.get_delta (idx, normalized_coords.arrayZ,
                                            normalized_coords.నిడివి, store_cache));

     ఒకవేళ (no_variations)
     {
       variation_idx_delta_map.సమితి (idx, hb_pair_t<గుర్తులేని, పూర్ణాంకం> (HB_OT_LAYOUT_NO_VARIATIONS_సూచీ, delta));
       కొనసాగు;
     }

     uint16_t major = idx >> 16;
     ఒకవేళ (major >= subtable_లెక్క) ఆపు;
     ఒకవేళ (major != చివరి_major)
     {
       new_minor = 0;
       ++new_major;
     }

     గుర్తులేని new_idx = (new_major << 16) + new_minor;
     variation_idx_delta_map.సమితి (idx, hb_pair_t<గుర్తులేని, పూర్ణాంకం> (new_idx, delta));
     ++new_minor;
     చివరి_major = major;
   }
   var_store.destroy_cache (store_cache);
 }

 మూస
 శూన్య
 remap_variation_indices<OT::అంశం_Variation_Store> (స్థిరరాశి OT::అంశం_Variation_Store &var_store,
                          స్థిరరాశి hb_సమితి_t &variation_indices,
                          స్థిరరాశి hb_సదిశ_t<పూర్ణాంకం>& normalized_coords,
                          తార్కిక calculate_delta, /* not pinned at default */
                          తార్కిక no_variations, /* all axes pinned */
                          hb_hashmap_t<గుర్తులేని, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> &variation_idx_delta_map /*బహిర్గతం*/);

 #నిర్వచితం_కాకపోతే HB_NO_పునాది
 శూన్య
 collect_పునాది_variation_indices (hb_subset_plan_t* plan)
 {
   hb_blob_ptr_t<OT::పునాది_4> పునాది = plan->ఆధారం_పట్టిక<OT::పునాది_4> ();
   ఒకవేళ (!పునాది->has_var_store ())
   {
     పునాది.destroy ();
     తిరిగి;
   }

   hb_సమితి_t varidx_సమితి;
   పునాది->collect_variation_indices (plan, varidx_సమితి);
   స్థిరరాశి OT::అంశం_Variation_Store &var_store = పునాది->get_var_store ();
   గుర్తులేని subtable_లెక్క = var_store.get_sub_పట్టిక_లెక్క ();


   remap_variation_indices (var_store, varidx_సమితి,
                             plan->normalized_coords,
                             !plan->pinned_at_default,
                             plan->all_axes_pinned,
                             plan->పునాది_variation_idx_map);
   generate_varstore_inner_maps (varidx_సమితి, subtable_లెక్క, plan->పునాది_varstore_inner_maps);

   పునాది.destroy ();
 }

 #అయితే_ముగింపు

తార్కిక
normalize_axes_location (hb_face_t *face, hb_subset_plan_t *plan)
{
  ఒకవేళ (plan->user_axes_location.is_empty ())
    తిరిగి నిజం;

  hb_array_t<స్థిరరాశి OT::Axis_రికార్డు> axes = face->పట్టిక.fvar->get_axes ();
  కొరకు (స్థిరరాశి స్వయంచాలక &axis : axes)
  {
    hb_ట్యాగ్_t ట్యాగ్_2 = axis.get_axis_ట్యాగ్ ();
    స్థిరరాశి Triple *range;
    దశాంశం minimum, మధ్య, గరిష్ఠ;
    axis.get_coordinates (minimum, మధ్య, గరిష్ఠ);
    ఒకవేళ (plan->user_axes_location.has (ట్యాగ్_2, &range) && !range->is_బిందువు () &&
        range->minimum == minimum &&
        range->మధ్య == మధ్య &&
        range->గరిష్ఠ == గరిష్ఠ)
      plan->user_axes_location.del (ట్యాగ్_2);
  }
  ఒకవేళ (plan->user_axes_location.is_empty ())
    తిరిగి నిజం;

  ఒకవేళ (!plan->check_success (plan->normalized_coords.resize (axes.నిడివి)))
    తిరిగి అబద్ధం;

  తార్కిక has_avar = face->పట్టిక.avar->has_దత్తాంశం ();
  hb_సదిశ_t<దశాంశం> normalized_mins;
  hb_సదిశ_t<దశాంశం> normalized_defaults;
  hb_సదిశ_t<దశాంశం> normalized_maxs;
  ఒకవేళ (has_avar)
  {
    ఒకవేళ (!plan->check_success (normalized_mins.resize (axes.నిడివి)) ||
        !plan->check_success (normalized_defaults.resize (axes.నిడివి)) ||
        !plan->check_success (normalized_maxs.resize (axes.నిడివి)))
      తిరిగి అబద్ధం;
  }

  తార్కిక axis_not_pinned = అబద్ధం;
  గుర్తులేని new_axis_idx = 0;
  గుర్తులేని చివరి_idx = 0;
  కొరకు (స్థిరరాశి స్వయంచాలక& _ : + hb_enumerate (axes))
  {
    గుర్తులేని i = _.మొదటి;
    స్థిరరాశి OT::Axis_రికార్డు &axis = _.second;
    hb_ట్యాగ్_t axis_ట్యాగ్_2 = axis.get_axis_ట్యాగ్ ();
    plan->axes_old_సూచీ_ట్యాగ్_map.సమితి (i, axis_ట్యాగ్_2);

    ఒకవేళ (!plan->user_axes_location.has (axis_ట్యాగ్_2) ||
        !plan->user_axes_location.get (axis_ట్యాగ్_2).is_బిందువు ())
    {
      axis_not_pinned = నిజం;
      plan->axes_సూచీ_map.సమితి (i, new_axis_idx);
      plan->axis_ట్యాగులు.push (axis_ట్యాగ్_2);
      new_axis_idx++;
    }

    Triple *axis_range;
    ఒకవేళ (plan->user_axes_location.has (axis_ట్యాగ్_2, &axis_range))
    {
      plan->axes_triple_distances.సమితి (axis_ట్యాగ్_2, axis.get_triple_distances ());

      దశాంశం normalized_min = axis.normalize_axis_విలువ (axis_range->minimum);
      దశాంశం normalized_default = axis.normalize_axis_విలువ (axis_range->మధ్య);
      దశాంశం normalized_max = axis.normalize_axis_విలువ (axis_range->గరిష్ఠ);

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

      ఒకవేళ (has_avar)
      {
        normalized_mins[i] = normalized_min;
        normalized_defaults[i] = normalized_default;
        normalized_maxs[i] = normalized_max;
        చివరి_idx = i;
      }
      లేకపోతే
      {
        plan->axes_location.సమితి (axis_ట్యాగ్_2, Triple ((ద్విగుణ) normalized_min,
                                                   (ద్విగుణ) normalized_default,
                                                   (ద్విగుణ) normalized_max));
        ఒకవేళ (normalized_default == -0.f)
          normalized_default = 0.f; // Normalize -0 to 0
        ఒకవేళ (normalized_default != 0.f)
          plan->pinned_at_default = అబద్ధం;

        plan->normalized_coords[i] = roundf (normalized_default * 16384.f);
      }
    }
  }
  plan->all_axes_pinned = !axis_not_pinned;

  // TODO: use avar map_coords_16_16() when normalization is changed to 16.16
  // in fonttools
  ఒకవేళ (has_avar)
  {
    స్థిరరాశి OT::avar* avar_పట్టిక = face->పట్టిక.avar;
    గుర్తులేని coords_len = చివరి_idx + 1;

    ఒకవేళ (avar_పట్టిక->has_v2_దత్తాంశం () && !plan->all_axes_pinned)
    {
      /* avar2 partial instancing: use v1-only mapping to get intermediate-space
       * coords. These are used for IVS rebasing and offset compensation. */
      plan->has_avar2 = నిజం;

      ఒకవేళ (!plan->check_success (avar_పట్టిక->map_coords_2_14 (normalized_mins.arrayZ, coords_len, నిజం)) ||
          !plan->check_success (avar_పట్టిక->map_coords_2_14 (normalized_defaults.arrayZ, coords_len, నిజం)) ||
          !plan->check_success (avar_పట్టిక->map_coords_2_14 (normalized_maxs.arrayZ, coords_len, నిజం)))
        తిరిగి అబద్ధం;

      కొరకు (స్థిరరాశి స్వయంచాలక& _ : + hb_enumerate (axes))
      {
        గుర్తులేని i = _.మొదటి;
        hb_ట్యాగ్_t axis_ట్యాగ్_2 = _.second.get_axis_ట్యాగ్ ();
        ఒకవేళ (plan->user_axes_location.has (axis_ట్యాగ్_2))
        {
          /* Store intermediate-space coords for offset compensation,
           * segment-map renormalization, and IVS rebasing. */
          plan->old_intermediates.సమితి (axis_ట్యాగ్_2, Triple ((ద్విగుణ) normalized_mins[i],
                                                         (ద్విగుణ) normalized_defaults[i],
                                                         (ద్విగుణ) normalized_maxs[i]));
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
      hb_blob_t *cff2_blob = hb_face_reference_పట్టిక (face, HB_ట్యాగ్ ('C','F','F','2'));
      తార్కిక detect_self_contained = !hb_blob_get_నిడివి (cff2_blob);
      hb_blob_destroy (cff2_blob);
      ఒకవేళ (!_compute_avar2_reachable_ranges (plan, axes, avar_పట్టిక,
                                            detect_self_contained))
        తిరిగి అబద్ధం;

#నిర్వచితం_కాకపోతే HB_NO_VAR_COMPOSITES
      /* VARC overrides can reach coordinates outside the font-level
       * ranges. Do not cull variation data using those ranges. */
      ఒకవేళ (!plan->drop_tables.has (HB_ట్యాగ్ ('V','A','R','C')))
      {
        hb_blob_t *varc_blob = hb_face_reference_పట్టిక (face, HB_ట్యాగ్ ('V','A','R','C'));
        ఒకవేళ (hb_blob_get_నిడివి (varc_blob))
          plan->avar2_reachable_ranges.reset ();
        hb_blob_destroy (varc_blob);
      }
#అయితే_ముగింపు

      /* Keep all axes in fvar (pinned ones as hidden), EXCEPT self-contained
       * pinned axes, which are removed entirely. */
      plan->axes_సూచీ_map.reset ();
      plan->axis_ట్యాగులు.reset ();
      గుర్తులేని retained_axis_idx = 0;
      కొరకు (స్థిరరాశి స్వయంచాలక& _ : + hb_enumerate (axes))
      {
        గుర్తులేని i = _.మొదటి;
        hb_ట్యాగ్_t axis_ట్యాగ్_2 = _.second.get_axis_ట్యాగ్ ();
        ఒకవేళ (plan->avar2_self_contained.has (axis_ట్యాగ్_2))
          కొనసాగు;
        plan->axes_సూచీ_map.సమితి (i, retained_axis_idx++);
        plan->axis_ట్యాగులు.push (axis_ట్యాగ్_2);
      }
      /* The other tables see ONLY the self-contained pins, as an ordinary
       * partial instancing in old final-coordinate space; the standard
       * instancing machinery bakes those axes' contributions in. The
       * remaining restriction is carried entirely by avar. */
      plan->axes_location.reset ();
      ఒకవేళ (plan->avar2_self_contained.get_population ())
      {
        తార్కిక sc_pinned_at_default = నిజం;
        కొరకు (స్వయంచాలక _ : plan->avar2_self_contained)
        {
          plan->axes_location.సమితి (_.మొదటి, Triple (_.second, _.second, _.second));
          ఒకవేళ (_.second != 0.0)
            sc_pinned_at_default = అబద్ధం;
        }
        కొరకు (స్థిరరాశి స్వయంచాలక& _ : + hb_enumerate (axes))
        {
          ద్విగుణ *v;
          plan->normalized_coords[_.మొదటి] =
              plan->avar2_self_contained.has (_.second.get_axis_ట్యాగ్ (), &v)
              ? (పూర్ణాంకం) roundf ((దశాంశం) (*v * 16384.0)) : 0;
        }
        plan->pinned_at_default = sc_pinned_at_default;
      }
      లేకపోతే
        plan->normalized_coords.shrink (0);
    }
    లేకపోతే
    {
      /* Standard avar v1 (or v1+v2 with all axes pinned) */
      ఒకవేళ (!plan->check_success (avar_పట్టిక->map_coords_2_14 (normalized_mins.arrayZ, coords_len)) ||
          !plan->check_success (avar_పట్టిక->map_coords_2_14 (normalized_defaults.arrayZ, coords_len)) ||
          !plan->check_success (avar_పట్టిక->map_coords_2_14 (normalized_maxs.arrayZ, coords_len)))
        తిరిగి అబద్ధం;

      కొరకు (స్థిరరాశి స్వయంచాలక& _ : + hb_enumerate (axes))
      {
        గుర్తులేని i = _.మొదటి;
        hb_ట్యాగ్_t axis_ట్యాగ్_2 = _.second.get_axis_ట్యాగ్ ();
        ఒకవేళ (plan->user_axes_location.has (axis_ట్యాగ్_2))
        {
          plan->axes_location.సమితి (axis_ట్యాగ్_2, Triple ((ద్విగుణ) normalized_mins[i],
                                                     (ద్విగుణ) normalized_defaults[i],
                                                     (ద్విగుణ) normalized_maxs[i]));
          దశాంశం normalized_default = normalized_defaults[i];
          ఒకవేళ (normalized_default == -0.f)
            normalized_default = 0.f;
          ఒకవేళ (normalized_default != 0.f)
            plan->pinned_at_default = అబద్ధం;

          plan->normalized_coords[i] = roundf (normalized_default * 16384.f);
        }
      }
    }
  }
  తిరిగి నిజం;
}

#నిర్వచితం_కాకపోతే HB_NO_OT_FONT_CFF
శూన్య
update_instance_metrics_map_from_cff2 (hb_subset_plan_t *plan)
{
  ఒకవేళ (!plan->normalized_coords) తిరిగి;
  OT::cff2::accelerator_t cff2 (plan->ఆధారం);
  ఒకవేళ (!cff2.is_valid ()) తిరిగి;

  hb_font_t *font = _get_hb_font_with_variations (plan);
  ఒకవేళ (unlikely (!plan->check_success (font != శూన్యసూచిక)))
  {
    hb_font_destroy (font);
    తిరిగి;
  }

  hb_glyph_extents_t extents = {0x7FFF, -0x7FFF};
  OT::hmtx_accelerator_t _hmtx (plan->ఆధారం);
  OT::hb_scalar_cache_t *hvar_store_cache = శూన్యసూచిక;
  ఒకవేళ (_hmtx.has_దత్తాంశం () && _hmtx.var_పట్టిక.get_నిడివి ())
    hvar_store_cache = _hmtx.var_పట్టిక->get_var_store ().create_cache ();

  OT::vmtx_accelerator_t _vmtx (plan->ఆధారం);
  OT::hb_scalar_cache_t *vvar_store_cache = శూన్యసూచిక;
  ఒకవేళ (_vmtx.has_దత్తాంశం () && _vmtx.var_పట్టిక.get_నిడివి ())
    vvar_store_cache = _vmtx.var_పట్టిక->get_var_store ().create_cache ();

  కొరకు (స్వయంచాలక p : *plan->glyph_map)
  {
    hb_codepoint_t old_gid = p.మొదటి;
    hb_codepoint_t new_gid = p.second;
    ఒకవేళ (!cff2.get_extents (font, old_gid, &extents)) కొనసాగు;
    తార్కిక has_bounds_info = నిజం;
    ఒకవేళ (extents.x_bearing == 0 && extents.వెడల్పు == 0 &&
        extents.ఎత్తు == 0 && extents.y_bearing == 0)
      has_bounds_info = అబద్ధం;

    ఒకవేళ (has_bounds_info)
    {
      plan->తల_maxp_info.xMin = hb_min (plan->తల_maxp_info.xMin, extents.x_bearing);
      plan->తల_maxp_info.xMax = hb_max (plan->తల_maxp_info.xMax,
						 hb_saturate_కూడు (extents.x_bearing, extents.వెడల్పు));
      plan->తల_maxp_info.yMax = hb_max (plan->తల_maxp_info.yMax, extents.y_bearing);
      plan->తల_maxp_info.yMin = hb_min (plan->తల_maxp_info.yMin,
						 hb_saturate_కూడు (extents.y_bearing, extents.ఎత్తు));
    }

    ఒకవేళ (_hmtx.has_దత్తాంశం ())
    {
      ద్విగుణ hori_aw = _hmtx.get_advance_without_var_unscaled (old_gid);
      ఒకవేళ (_hmtx.var_పట్టిక.get_నిడివి ())
        hori_aw += (ద్విగుణ) roundf (_hmtx.var_పట్టిక->get_advance_delta_unscaled (old_gid, font->coords, font->అంకె_coords,
									 hvar_store_cache));
      /* Malicious deltas can take the advance out of the UFWORD range that
       * hmtx serialization stores.  Clamp, to keep downstream consumers
       * (e.g. CFF width optimizer) bounded and consistent with hmtx. */
      hori_aw = hb_clamp (hori_aw, 0., 65535.);
      పూర్ణాంకం lsb = extents.x_bearing;
      ఒకవేళ (!has_bounds_info)
      {
        _hmtx.get_leading_bearing_without_var_unscaled (old_gid, &lsb);
      }
      plan->hmtx_map.సమితి (new_gid, hb_pair ((గుర్తులేని) hori_aw, lsb));
      plan->bounds_వెడల్పు_vec[new_gid] = extents.వెడల్పు;
    }

    ఒకవేళ (_vmtx.has_దత్తాంశం ())
    {
      ద్విగుణ vert_aw = _vmtx.get_advance_without_var_unscaled (old_gid);
      ఒకవేళ (_vmtx.var_పట్టిక.get_నిడివి ())
        vert_aw += (ద్విగుణ) roundf (_vmtx.var_పట్టిక->get_advance_delta_unscaled (old_gid, font->coords, font->అంకె_coords,
									 vvar_store_cache));
      vert_aw = hb_clamp (vert_aw, 0., 65535.);
      hb_position_t vorg_x = 0;
      hb_position_t vorg_y = 0;
      పూర్ణాంకం tsb = 0;
      ఒకవేళ (has_bounds_info &&
           hb_font_get_glyph_v_origin (font, old_gid, &vorg_x, &vorg_y))
      {
        tsb = hb_saturate_sub (vorg_y, extents.y_bearing);
      } లేకపోతే {
        _vmtx.get_leading_bearing_without_var_unscaled (old_gid, &tsb);
      }

      plan->vmtx_map.సమితి (new_gid, hb_pair ((గుర్తులేని) vert_aw, tsb));
      plan->bounds_ఎత్తు_vec[new_gid] = extents.ఎత్తు;
    }
  }
  /* No glyph contributed bounds: do not serialize the accumulator's
   * inverted sentinels into head (or a downgraded CFF FontBBox). */
  ఒకవేళ (plan->తల_maxp_info.xMin > plan->తల_maxp_info.xMax)
  {
    plan->తల_maxp_info.xMin = plan->తల_maxp_info.xMax = 0;
    plan->తల_maxp_info.yMin = plan->తల_maxp_info.yMax = 0;
  }
  hb_font_destroy (font);
  ఒకవేళ (hvar_store_cache)
    _hmtx.var_పట్టిక->get_var_store ().destroy_cache (hvar_store_cache);
  ఒకవేళ (vvar_store_cache)
    _vmtx.var_పట్టిక->get_var_store ().destroy_cache (vvar_store_cache);
}
#అయితే_ముగింపు

తార్కిక
get_instance_glyphs_contour_points (hb_subset_plan_t *plan)
{
  /* contour_points vector only needed for updating gvar table (infer delta and
   * iup delta optimization) during partial instancing */
  ఒకవేళ (plan->user_axes_location.is_empty () || plan->all_axes_pinned)
    తిరిగి నిజం;

  OT::glyf_accelerator_t glyf (plan->ఆధారం);

  కొరకు (స్వయంచాలక &_ : plan->new_గమ్యం_old_gid_list)
  {
    hb_codepoint_t new_gid = _.మొదటి;
    contour_బిందువు_సదిశ_t all_points;
    ఒకవేళ (new_gid == 0 && !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      ఒకవేళ (unlikely (!plan->new_gid_contour_points_map.సమితి (new_gid, all_points)))
        తిరిగి అబద్ధం;
      కొనసాగు;
    }

    hb_codepoint_t old_gid = _.second;
    స్వయంచాలక glyph = glyf.glyph_for_gid (old_gid);
    ఒకవేళ (unlikely (!glyph.get_all_points_without_var (plan->ఆధారం, all_points)))
      తిరిగి అబద్ధం;
    ఒకవేళ (unlikely (!plan->new_gid_contour_points_map.సమితి (new_gid, all_points)))
      తిరిగి అబద్ధం;

    /* composite new gids are only needed by iup delta optimization */
    ఒకవేళ ((plan->flags & HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS) && glyph.is_సంయుక్త ())
      plan->సంయుక్త_new_gids.కూడు (new_gid);
  }
  తిరిగి నిజం;
}

మూస<రకం_పేరు Delta_సమితి_సూచీ_Map>
శూన్య
remap_colrv1_delta_సమితి_సూచీ_indices (స్థిరరాశి Delta_సమితి_సూచీ_Map &సూచీ_map,
                                      స్థిరరాశి hb_సమితి_t &delta_సమితి_idxes,
                                      hb_hashmap_t<గుర్తులేని, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /*బహిర్గతం*/)
{
  ఒకవేళ (!సూచీ_map.get_map_లెక్క ())
    తిరిగి;

  hb_hashmap_t<గుర్తులేని, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> delta_సమితి_idx_delta_map;
  గుర్తులేని new_delta_సమితి_idx = 0;
  కొరకు (గుర్తులేని delta_సమితి_idx : delta_సమితి_idxes)
  {
    గుర్తులేని var_idx = సూచీ_map.map (delta_సమితి_idx);
    గుర్తులేని new_varidx = HB_OT_LAYOUT_NO_VARIATIONS_సూచీ;
    పూర్ణాంకం delta = 0;

    ఒకవేళ (var_idx != HB_OT_LAYOUT_NO_VARIATIONS_సూచీ)
    {
      hb_pair_t<గుర్తులేని, పూర్ణాంకం> *new_varidx_delta;
      ఒకవేళ (!variation_idx_delta_map.has (var_idx, &new_varidx_delta)) కొనసాగు;

      new_varidx = hb_మొదటి (*new_varidx_delta);
      delta = hb_రెండవ (*new_varidx_delta);
    }

    new_deltaset_idx_varidx_map.సమితి (new_delta_సమితి_idx, new_varidx);
    delta_సమితి_idx_delta_map.సమితి (delta_సమితి_idx, hb_pair_t<గుర్తులేని, పూర్ణాంకం> (new_delta_సమితి_idx, delta));
    new_delta_సమితి_idx++;
  }
  variation_idx_delta_map = std::move (delta_సమితి_idx_delta_map);
}

మూస శూన్య
remap_colrv1_delta_సమితి_సూచీ_indices<OT::Delta_సమితి_సూచీ_Map> (స్థిరరాశి OT::Delta_సమితి_సూచీ_Map &సూచీ_map,
                                      స్థిరరాశి hb_సమితి_t &delta_సమితి_idxes,
                                      hb_hashmap_t<గుర్తులేని, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /*బహిర్గతం*/);

 #అయితే_ముగింపు

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

 #ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset-plan.hh"

 #ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-var-common.hh"
 #ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-base-table.hh"
 #ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-glyf-table.hh"
 #ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-var-fvar-table.hh"
 #ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-var-avar-table.hh"
 #ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-cff2-table.hh"

 #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR

 ਖਾਲੀ
 generate_varstore_inner_maps (ਅਚਲ hb_ਸੈੱਟ_t& varidx_ਸੈੱਟ,
                               ਬਿਨਾਨਿਸ਼ਾਨੀ subtable_ਗਿਣਤੀ,
                               hb_ਸਦਿਸ਼_t<hb_inc_bimap_t> &inner_maps /*ਬਾਹਰਲਾ*/)
 {
   ਜੇ (varidx_ਸੈੱਟ.is_empty () || subtable_ਗਿਣਤੀ == 0) ਵਾਪਸ;

   ਜੇ (unlikely (!inner_maps.resize (subtable_ਗਿਣਤੀ))) ਵਾਪਸ;
   ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ idx : varidx_ਸੈੱਟ)
   {
     uint16_t major = idx >> 16;
     uint16_t minor = idx & 0xFFFF;

     ਜੇ (major >= subtable_ਗਿਣਤੀ)
       ਜਾਰੀ;
     inner_maps[major].ਜੋੜੋ (minor);
   }
 }

 /* Evaluate a variation-region tent at v, mirroring VarRegionAxis::evaluate
  * (invalid regions evaluate as constant 1). */
 ਸਥਿਰ ਦੁੱਗਣਾ
 _tent_eval (ਅਚਲ Triple &tent, ਦੁੱਗਣਾ v)
 {
   ਦੁੱਗਣਾ ਸ਼ੁਰੂ = (ਦੁੱਗਣਾ) tent.minimum;
   ਦੁੱਗਣਾ peak = (ਦੁੱਗਣਾ) tent.ਵਿਚਕਾਰ;
   ਦੁੱਗਣਾ ਸਿਰਾ = (ਦੁੱਗਣਾ) tent.ਵੱਧ_ਤੋਂ_ਵੱਧ;
   ਜੇ (unlikely (ਸ਼ੁਰੂ > peak || peak > ਸਿਰਾ)) ਵਾਪਸ 1.0;
   ਜੇ (unlikely (ਸ਼ੁਰੂ < 0.0 && ਸਿਰਾ > 0.0 && peak != 0.0)) ਵਾਪਸ 1.0;
   ਜੇ (peak == 0.0 || v == peak) ਵਾਪਸ 1.0;
   ਜੇ (v <= ਸ਼ੁਰੂ || ਸਿਰਾ <= v) ਵਾਪਸ 0.0;
   ਜੇ (v < peak) ਵਾਪਸ (v - ਸ਼ੁਰੂ) / (peak - ਸ਼ੁਰੂ);
   ਵਾਪਸ (ਸਿਰਾ - v) / (ਸਿਰਾ - peak);
 }

 /* Range of a tent's scalar over an input interval [lo, hi]. The tent is
  * unimodal, so the extremes are at the interval endpoints, plus the peak
  * when it falls inside. */
 ਸਥਿਰ ਖਾਲੀ
 tent_ਮੁੱਲ_range (ਅਚਲ Triple &tent, ਦੁੱਗਣਾ lo, ਦੁੱਗਣਾ hi,
                    ਦੁੱਗਣਾ *tmin, ਦੁੱਗਣਾ *tmax)
 {
   ਦੁੱਗਣਾ a = _tent_eval (tent, lo);
   ਦੁੱਗਣਾ b = _tent_eval (tent, hi);
   *tmin = hb_min (a, b);
   *tmax = hb_max (a, b);
   ਜੇ (lo <= (ਦੁੱਗਣਾ) tent.ਵਿਚਕਾਰ && (ਦੁੱਗਣਾ) tent.ਵਿਚਕਾਰ <= hi)
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
 ਸਥਿਰ ਤਾਰਕਿਕ
 _compute_avar2_reachable_ranges (hb_subset_plan_t *plan,
                                  hb_array_t<ਅਚਲ OT::Axis_ਰਿਕਾਰਡ> axes,
                                  ਅਚਲ OT::avar *avar_ਸਾਰਣੀ,
                                  ਤਾਰਕਿਕ detect_self_contained)
 {
   ਅਚਲ OT::ਵਸਤੂ_Variation_Store *var_store;
   ਅਚਲ OT::Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map *varidx_map;
   ਜੇ (!avar_ਸਾਰਣੀ->get_v2_store_and_map (&var_store, &varidx_map))
     ਵਾਪਸ ਝੂਠ;

   /* Old-intermediate input box per axis. */
   hb_hashmap_t<hb_ਟੈਗ_t, hb_pair_t<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ>> box;
   ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &axis : axes)
   {
     hb_ਟੈਗ_t ਟੈਗ_2 = axis.get_axis_ਟੈਗ ();
     ਦੁੱਗਣਾ lo = -1.0, hi = +1.0;
     Triple *old_int;
     ਜੇ (plan->user_axes_location.has (ਟੈਗ_2))
     {
       ਜੇ (!plan->old_intermediates.has (ਟੈਗ_2, &old_int)) ਵਾਪਸ ਝੂਠ;
       lo = (ਦੁੱਗਣਾ) old_int->minimum;
       hi = (ਦੁੱਗਣਾ) old_int->ਵੱਧ_ਤੋਂ_ਵੱਧ;
     }
     ਨਹੀਂਤਾਂ ਜੇ (axis.is_hidden ())
       lo = hi = 0.0; /* private axis: intermediate is always 0 */
     ਜੇ (!box.ਸੈੱਟ (ਟੈਗ_2, hb_pair (lo, hi))) ਵਾਪਸ ਝੂਠ;
   }

   hb_ਸਦਿਸ਼_t<hb_hashmap_t<hb_ਟੈਗ_t, Triple>> regions;
   ਜੇ (!var_store->get_region_list ().get_var_regions (plan->axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map, regions))
     ਵਾਪਸ ਝੂਠ;

   hb_hashmap_t<hb_ਟੈਗ_t, ਬਿਨਾਨਿਸ਼ਾਨੀ> grid_pos;
   hb_ਸਦਿਸ਼_t<hb_ਸਦਿਸ਼_t<ਦੁੱਗਣਾ>> grid_points;
   ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < axes.ਲੰਮਾਈ; i++)
   {
     grid_pos.reset ();
     grid_points.reset ();

     hb_ਟੈਗ_t ਟੈਗ_2 = axes[i].get_axis_ਟੈਗ ();
     hb_pair_t<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ> *ਪਛਾਣ_ਤੱਤ;
     ਜੇ (!box.has (ਟੈਗ_2, &ਪਛਾਣ_ਤੱਤ)) ਵਾਪਸ ਝੂਠ;

     /* Collect the row's active regions (non-zero delta). A varidx (or the
      * implicit identity mapping) that doesn't resolve to a real store row
      * behaves as "no variation" at runtime, so it contributes no regions;
      * this also covers a NULL VarStore. */
     hb_ਸਦਿਸ਼_t<hb_pair_t<ਪੂਰਨ, ਬਿਨਾਨਿਸ਼ਾਨੀ>> ਸਰਗਰਮ; /* (delta, region index) */
     uint32_t varidx = varidx_map->map (i);
     /* Runtime treats out-of-range delta-set indices as zero. */
     ਜੇ (varidx != HB_OT_LAYOUT_NO_VARIATIONS_ਸੂਚਕਾਂਕ &&
	 !var_store->has_delta_ਸੈੱਟ (varidx))
       varidx = HB_OT_LAYOUT_NO_VARIATIONS_ਸੂਚਕਾਂਕ;
     ਜੇ (varidx != HB_OT_LAYOUT_NO_VARIATIONS_ਸੂਚਕਾਂਕ)
     {
       ਬਿਨਾਨਿਸ਼ਾਨੀ outer = varidx >> 16;
       ਬਿਨਾਨਿਸ਼ਾਨੀ inner = varidx & 0xFFFF;
       ਅਚਲ OT::Var_ਅੰਕੜੇ &vd = var_store->get_sub_ਸਾਰਣੀ (outer);
       ਅਚਲ OT::HBUINT8 *delta_ਬਾਈਟਾਂ = vd.get_delta_ਬਾਈਟਾਂ ();
       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੰਕਤੀ_ਮਾਪ = vd.get_ਪੰਕਤੀ_ਮਾਪ ();
       ਬਿਨਾਨਿਸ਼ਾਨੀ region_ਗਿਣਤੀ_2 = vd.get_region_ਸੂਚਕਾਂਕ_ਗਿਣਤੀ ();
       ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ r = 0; r < region_ਗਿਣਤੀ_2; r++)
       {
	 ਪੂਰਨ delta = vd.get_ਵਸਤੂ_delta_fast (inner, r, delta_ਬਾਈਟਾਂ, ਪੰਕਤੀ_ਮਾਪ);
	 ਜੇ (!delta) ਜਾਰੀ;
	 ਬਿਨਾਨਿਸ਼ਾਨੀ region_idx = vd.get_region_ਸੂਚਕਾਂਕ (r);
	 ਜੇ (region_idx >= regions.ਲੰਮਾਈ) ਵਾਪਸ ਝੂਠ;
	 ਸਰਗਰਮ.push (hb_pair (delta, region_idx));
       }
       ਜੇ (ਸਰਗਰਮ.ਵਿੱਚ_error ()) ਵਾਪਸ ਝੂਠ;
     }

     /* The delta is piecewise multilinear in the axis coordinates (each
      * region scalar is a product of independent per-axis tents), so over
      * the box its exact extremes are attained at vertices of the grid of
      * per-axis tent breakpoints. Enumerate that grid when it is small
      * (real fonts); otherwise fall back to per-region interval
      * arithmetic, which ignores correlations between regions and is
      * looser but still conservative. Mirrors fontTools'
      * VarStore.getExtremes. */
     constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ MAX_GRID = 1u << 14;
     ਤਾਰਕਿਕ exact = ਸੱਚ;
     ਦੁੱਗਣਾ dmin = 0.0, dmax = 0.0; /* delta extremes, F2Dot14 units */
     ਦੁੱਗਣਾ vmin = 0.0, vmax = 0.0; /* identity + delta extremes, coord units */

     /* Grid axes: the target axis (identity term), plus every axis a valid
      * tent of an active region references. */
     ਸਵੈਚਾਲਿਤ ਜੋੜੋ_grid_axis = [&] (hb_ਟੈਗ_t t) -> ਪੂਰਨ
     {
       ਬਿਨਾਨਿਸ਼ਾਨੀ *pos;
       ਜੇ (grid_pos.has (t, &pos)) ਵਾਪਸ (ਪੂਰਨ) *pos;
       ਬਿਨਾਨਿਸ਼ਾਨੀ new_pos = grid_pos.get_population ();
       hb_pair_t<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ> *ਨਿਵੇਸ਼;
       ਦੁੱਗਣਾ blo = -1.0, bhi = +1.0;
       ਜੇ (box.has (t, &ਨਿਵੇਸ਼)) { blo = ਨਿਵੇਸ਼->ਪਹਿਲਾ; bhi = ਨਿਵੇਸ਼->second; }
       grid_points.push (hb_ਸਦਿਸ਼_t<ਦੁੱਗਣਾ> ());
       ਜੇ (grid_points.ਵਿੱਚ_error ()) ਵਾਪਸ -1;
       ਸਵੈਚਾਲਿਤ &points = grid_points[new_pos];
       points.push (blo);
       points.push (bhi);
       ਜੇ (points.ਵਿੱਚ_error () || !grid_pos.ਸੈੱਟ (t, new_pos)) ਵਾਪਸ -1;
       ਵਾਪਸ (ਪੂਰਨ) new_pos;
     };

     ਜੇ (ਜੋੜੋ_grid_axis (ਟੈਗ_2) < 0) ਵਾਪਸ ਝੂਠ;
     /* Per active region: (grid axis position, tent) for its valid tents. */
     hb_ਸਦਿਸ਼_t<hb_ਸਦਿਸ਼_t<hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, Triple>>> region_tents;
     ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &ar : ਸਰਗਰਮ)
     {
       region_tents.push (hb_ਸਦਿਸ਼_t<hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, Triple>> ());
       ਜੇ (region_tents.ਵਿੱਚ_error ()) ਵਾਪਸ ਝੂਠ;
       ਸਵੈਚਾਲਿਤ &tents = region_tents[region_tents.ਲੰਮਾਈ - 1];
       ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &_ : regions[ar.second])
       {
	 ਅਚਲ Triple &tent = _.second;
	 /* Tents the runtime ignores (constant scalar 1) add no breakpoints. */
	 ਜੇ (tent.ਵਿਚਕਾਰ == 0.f ||
	     tent.minimum > tent.ਵਿਚਕਾਰ || tent.ਵਿਚਕਾਰ > tent.ਵੱਧ_ਤੋਂ_ਵੱਧ ||
	     (tent.minimum < 0.f && tent.ਵੱਧ_ਤੋਂ_ਵੱਧ > 0.f))
	   ਜਾਰੀ;
	 ਪੂਰਨ pos = ਜੋੜੋ_grid_axis (_.ਪਹਿਲਾ);
	 ਜੇ (pos < 0) ਵਾਪਸ ਝੂਠ;
	 ਸਵੈਚਾਲਿਤ &points = grid_points[pos];
	 ਦੁੱਗਣਾ blo = points[0], bhi = points[1];
	 points.push (hb_clamp ((ਦੁੱਗਣਾ) tent.minimum, blo, bhi));
	 points.push (hb_clamp ((ਦੁੱਗਣਾ) tent.ਵਿਚਕਾਰ, blo, bhi));
	 points.push (hb_clamp ((ਦੁੱਗਣਾ) tent.ਵੱਧ_ਤੋਂ_ਵੱਧ, blo, bhi));
	 tents.push (hb_pair ((ਬਿਨਾਨਿਸ਼ਾਨੀ) pos, tent));
	 ਜੇ (points.ਵਿੱਚ_error () || tents.ਵਿੱਚ_error ()) ਵਾਪਸ ਝੂਠ;
       }
     }

     ਬਿਨਾਨਿਸ਼ਾਨੀ grid_ਮਾਪ = 1;
     ਲਈ (ਸਵੈਚਾਲਿਤ &points : grid_points)
     {
       points.qsort ([] (ਅਚਲ ਦੁੱਗਣਾ &a, ਅਚਲ ਦੁੱਗਣਾ &b) -> ਪੂਰਨ
		     { ਵਾਪਸ a < b ? -1 : a > b ? +1 : 0; });
       ਬਿਨਾਨਿਸ਼ਾਨੀ n = 0;
       ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < points.ਲੰਮਾਈ; j++)
	 ਜੇ (!n || points.arrayZ[j] != points.arrayZ[n - 1])
	   points.arrayZ[n++] = points.arrayZ[j];
       points.shrink (n);
       ਜੇ (grid_ਮਾਪ > MAX_GRID / points.ਲੰਮਾਈ) { exact = ਝੂਠ; ਰੋਕੋ; }
       grid_ਮਾਪ *= points.ਲੰਮਾਈ;
     }

     ਜੇ (exact)
     {
       ਬਿਨਾਨਿਸ਼ਾਨੀ ਨਿਸ਼ਾਨਾ_pos = 0; /* the target axis was added first */
       hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> odometer;
       ਜੇ (!odometer.resize (grid_pos.get_population ())) ਵਾਪਸ ਝੂਠ;
       ਤਾਰਕਿਕ ਪਹਿਲਾ = ਸੱਚ;
       ਜਦੋਂ (ਸੱਚ)
       {
	 ਦੁੱਗਣਾ delta_ਜੋੜ = 0.0;
	 ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ r = 0; r < ਸਰਗਰਮ.ਲੰਮਾਈ; r++)
	 {
	   ਦੁੱਗਣਾ scalar = 1.0;
	   ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &pt : region_tents[r])
	   {
	     ਦੁੱਗਣਾ v = grid_points[pt.ਪਹਿਲਾ][odometer[pt.ਪਹਿਲਾ]];
	     scalar *= _tent_eval (pt.second, v);
	     ਜੇ (scalar == 0.0) ਰੋਕੋ;
	   }
	   delta_ਜੋੜ += scalar * ਸਰਗਰਮ[r].ਪਹਿਲਾ;
	 }
	 ਦੁੱਗਣਾ coord = grid_points[ਨਿਸ਼ਾਨਾ_pos][odometer[ਨਿਸ਼ਾਨਾ_pos]];
	 ਦੁੱਗਣਾ value = coord + delta_ਜੋੜ / 16384.0;
	 ਜੇ (ਪਹਿਲਾ)
	 {
	   dmin = dmax = delta_ਜੋੜ;
	   vmin = vmax = value;
	   ਪਹਿਲਾ = ਝੂਠ;
	 }
	 ਨਹੀਂਤਾਂ
	 {
	   dmin = hb_min (dmin, delta_ਜੋੜ); dmax = hb_max (dmax, delta_ਜੋੜ);
	   vmin = hb_min (vmin, value);     vmax = hb_max (vmax, value);
	 }
	 ਬਿਨਾਨਿਸ਼ਾਨੀ k = 0;
	 ਲਈ (; k < odometer.ਲੰਮਾਈ; k++)
	 {
	   ਜੇ (++odometer[k] < grid_points[k].ਲੰਮਾਈ) ਰੋਕੋ;
	   odometer[k] = 0;
	 }
	 ਜੇ (k == odometer.ਲੰਮਾਈ) ਰੋਕੋ;
       }
     }
     ਨਹੀਂਤਾਂ
     {
       ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &ar : ਸਰਗਰਮ)
       {
	 ਪੂਰਨ delta = ar.ਪਹਿਲਾ;
	 ਦੁੱਗਣਾ smin = 1.0, smax = 1.0;
	 ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &_ : regions[ar.second])
	 {
	   hb_pair_t<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ> *ਨਿਵੇਸ਼;
	   ਦੁੱਗਣਾ blo = -1.0, bhi = +1.0;
	   ਜੇ (box.has (_.ਪਹਿਲਾ, &ਨਿਵੇਸ਼)) { blo = ਨਿਵੇਸ਼->ਪਹਿਲਾ; bhi = ਨਿਵੇਸ਼->second; }
	   ਦੁੱਗਣਾ tmin, tmax;
	   tent_ਮੁੱਲ_range (_.second, blo, bhi, &tmin, &tmax);
	   smin *= tmin;
	   smax *= tmax;
	   ਜੇ (smax == 0.0) ਰੋਕੋ;
	 }
	 ਜੇ (delta > 0) { dmin += smin * delta; dmax += smax * delta; }
	 ਨਹੀਂਤਾਂ           { dmin += smax * delta; dmax += smin * delta; }
       }
       vmin = ਪਛਾਣ_ਤੱਤ->ਪਹਿਲਾ + dmin / 16384.0;
       vmax = ਪਛਾਣ_ਤੱਤ->second + dmax / 16384.0;
     }
     /* A pinned axis whose delta is constant over the box is self-contained:
      * its final coordinate is a constant. It can be removed from fvar/avar
      * and its contribution baked into the variation tables like an ordinary
      * pin at that coordinate (in old final space). Round the delta alone
      * and add it to the exact quantized intermediate, matching the runtime
      * (map_coords_2_14) and fontTools. */
     Triple *user;
     ਜੇ (detect_self_contained && dmin == dmax &&
	 plan->user_axes_location.has (ਟੈਗ_2, &user) && user->is_ਬਿੰਦੂ ())
     {
       ਪੂਰਨ v_int = (ਪੂਰਨ) roundf ((ਦਸ਼ਮਲਵ) ਪਛਾਣ_ਤੱਤ->ਪਹਿਲਾ * 16384.f) +
		   (ਪੂਰਨ) roundf ((ਦਸ਼ਮਲਵ) dmin);
       v_int = hb_clamp (v_int, -(1 << 14), +(1 << 14));
       ਜੇ (!plan->avar2_self_contained.ਸੈੱਟ (ਟੈਗ_2, v_int / 16384.0)) ਵਾਪਸ ਝੂਠ;
       ਜਾਰੀ; /* axis is dropped; no region can reference it afterwards */
     }

     /* Pad by one F2Dot14 unit against rounding differences with the
      * runtime's fixed-point evaluation. */
     ਦੁੱਗਣਾ lo = hb_clamp (vmin - 1.0 / 16384.0, -1.0, +1.0);
     ਦੁੱਗਣਾ hi = hb_clamp (vmax + 1.0 / 16384.0, -1.0, +1.0);
     lo = floor (lo * 16384.0) / 16384.0;
     hi = ceil (hi * 16384.0) / 16384.0;
     ਜੇ (lo <= -1.0 && hi >= +1.0)
       ਜਾਰੀ; /* not constraining */
     ਜੇ (!plan->avar2_reachable_ranges.ਸੈੱਟ (ਟੈਗ_2, Triple (lo, hb_clamp (0.0, lo, hi), hi)))
       ਵਾਪਸ ਝੂਠ;
   }
   ਵਾਪਸ ਸੱਚ;
 }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_FONT_CFF
 ਸਥਿਰ ਸਤਰਅੰਦਰ hb_font_t*
 _get_hb_font_with_variations (ਅਚਲ hb_subset_plan_t *plan)
 {
   hb_font_t *font = hb_font_create (plan->ਸਰੋਤ);

   ਜੇ (plan->has_avar2)
   {
     /* Under avar2, instancing applies only to the self-contained pins,
      * whose constant final coordinates the plan holds in normalized_coords.
      * Setting user-space variations would run the full avar2 mapping and
      * bake contributions that remain live in the instance's variations. */
     hb_font_ਸੈੱਟ_var_coords_normalized (font, plan->normalized_coords.arrayZ,
                                        plan->normalized_coords.ਲੰਮਾਈ);
     ਵਾਪਸ font;
   }

   hb_ਸਦਿਸ਼_t<hb_variation_t> vars;
   ਜੇ (!vars.alloc (plan->user_axes_location.get_population ())) {
     hb_font_destroy (font);
     ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
   }

   ਲਈ (ਸਵੈਚਾਲਿਤ _ : plan->user_axes_location)
   {
     hb_variation_t var;
     var.ਟੈਗ_2 = _.ਪਹਿਲਾ;
     var.value = _.second.ਵਿਚਕਾਰ;
     vars.push (var);
   }

   hb_font_ਸੈੱਟ_variations (font, vars.arrayZ, plan->user_axes_location.get_population ());
   ਵਾਪਸ font;
 }
#ਜੇ_ਅੰਤ

 ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ ਵਸਤੂ_Var_Store>
 ਖਾਲੀ
 remap_variation_indices (ਅਚਲ ਵਸਤੂ_Var_Store &var_store,
                          ਅਚਲ hb_ਸੈੱਟ_t &variation_indices,
                          ਅਚਲ hb_ਸਦਿਸ਼_t<ਪੂਰਨ>& normalized_coords,
                          ਤਾਰਕਿਕ calculate_delta, /* not pinned at default */
                          ਤਾਰਕਿਕ no_variations, /* all axes pinned */
                          hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> &variation_idx_delta_map /*ਬਾਹਰਲਾ*/)
 {
   ਜੇ (&var_store == &Null (OT::ਵਸਤੂ_Variation_Store)) ਵਾਪਸ;
   ਬਿਨਾਨਿਸ਼ਾਨੀ subtable_ਗਿਣਤੀ = var_store.get_sub_ਸਾਰਣੀ_ਗਿਣਤੀ ();
   ਸਵੈਚਾਲਿਤ *store_cache = var_store.create_cache ();

   ਬਿਨਾਨਿਸ਼ਾਨੀ new_major = 0, new_minor = 0;
   ਬਿਨਾਨਿਸ਼ਾਨੀ ਆਖਰੀ_major = (variation_indices.get_min ()) >> 16;
   ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ idx : variation_indices)
   {
     ਪੂਰਨ delta = 0;
     ਜੇ (calculate_delta)
       delta = roundf (var_store.get_delta (idx, normalized_coords.arrayZ,
                                            normalized_coords.ਲੰਮਾਈ, store_cache));

     ਜੇ (no_variations)
     {
       variation_idx_delta_map.ਸੈੱਟ (idx, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ> (HB_OT_LAYOUT_NO_VARIATIONS_ਸੂਚਕਾਂਕ, delta));
       ਜਾਰੀ;
     }

     uint16_t major = idx >> 16;
     ਜੇ (major >= subtable_ਗਿਣਤੀ) ਰੋਕੋ;
     ਜੇ (major != ਆਖਰੀ_major)
     {
       new_minor = 0;
       ++new_major;
     }

     ਬਿਨਾਨਿਸ਼ਾਨੀ new_idx = (new_major << 16) + new_minor;
     variation_idx_delta_map.ਸੈੱਟ (idx, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ> (new_idx, delta));
     ++new_minor;
     ਆਖਰੀ_major = major;
   }
   var_store.destroy_cache (store_cache);
 }

 ਸਾਂਚਾ
 ਖਾਲੀ
 remap_variation_indices<OT::ਵਸਤੂ_Variation_Store> (ਅਚਲ OT::ਵਸਤੂ_Variation_Store &var_store,
                          ਅਚਲ hb_ਸੈੱਟ_t &variation_indices,
                          ਅਚਲ hb_ਸਦਿਸ਼_t<ਪੂਰਨ>& normalized_coords,
                          ਤਾਰਕਿਕ calculate_delta, /* not pinned at default */
                          ਤਾਰਕਿਕ no_variations, /* all axes pinned */
                          hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> &variation_idx_delta_map /*ਬਾਹਰਲਾ*/);

 #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_ਅਧਾਰ
 ਖਾਲੀ
 collect_ਅਧਾਰ_variation_indices (hb_subset_plan_t* plan)
 {
   hb_blob_ptr_t<OT::ਅਧਾਰ_4> ਅਧਾਰ = plan->ਸਰੋਤ_ਸਾਰਣੀ<OT::ਅਧਾਰ_4> ();
   ਜੇ (!ਅਧਾਰ->has_var_store ())
   {
     ਅਧਾਰ.destroy ();
     ਵਾਪਸ;
   }

   hb_ਸੈੱਟ_t varidx_ਸੈੱਟ;
   ਅਧਾਰ->collect_variation_indices (plan, varidx_ਸੈੱਟ);
   ਅਚਲ OT::ਵਸਤੂ_Variation_Store &var_store = ਅਧਾਰ->get_var_store ();
   ਬਿਨਾਨਿਸ਼ਾਨੀ subtable_ਗਿਣਤੀ = var_store.get_sub_ਸਾਰਣੀ_ਗਿਣਤੀ ();


   remap_variation_indices (var_store, varidx_ਸੈੱਟ,
                             plan->normalized_coords,
                             !plan->pinned_at_default,
                             plan->all_axes_pinned,
                             plan->ਅਧਾਰ_variation_idx_map);
   generate_varstore_inner_maps (varidx_ਸੈੱਟ, subtable_ਗਿਣਤੀ, plan->ਅਧਾਰ_varstore_inner_maps);

   ਅਧਾਰ.destroy ();
 }

 #ਜੇ_ਅੰਤ

ਤਾਰਕਿਕ
normalize_axes_location (hb_face_t *face, hb_subset_plan_t *plan)
{
  ਜੇ (plan->user_axes_location.is_empty ())
    ਵਾਪਸ ਸੱਚ;

  hb_array_t<ਅਚਲ OT::Axis_ਰਿਕਾਰਡ> axes = face->ਸਾਰਣੀ.fvar->get_axes ();
  ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &axis : axes)
  {
    hb_ਟੈਗ_t ਟੈਗ_2 = axis.get_axis_ਟੈਗ ();
    ਅਚਲ Triple *range;
    ਦਸ਼ਮਲਵ minimum, ਵਿਚਕਾਰ, ਵੱਧ_ਤੋਂ_ਵੱਧ;
    axis.get_coordinates (minimum, ਵਿਚਕਾਰ, ਵੱਧ_ਤੋਂ_ਵੱਧ);
    ਜੇ (plan->user_axes_location.has (ਟੈਗ_2, &range) && !range->is_ਬਿੰਦੂ () &&
        range->minimum == minimum &&
        range->ਵਿਚਕਾਰ == ਵਿਚਕਾਰ &&
        range->ਵੱਧ_ਤੋਂ_ਵੱਧ == ਵੱਧ_ਤੋਂ_ਵੱਧ)
      plan->user_axes_location.del (ਟੈਗ_2);
  }
  ਜੇ (plan->user_axes_location.is_empty ())
    ਵਾਪਸ ਸੱਚ;

  ਜੇ (!plan->check_success (plan->normalized_coords.resize (axes.ਲੰਮਾਈ)))
    ਵਾਪਸ ਝੂਠ;

  ਤਾਰਕਿਕ has_avar = face->ਸਾਰਣੀ.avar->has_ਅੰਕੜੇ ();
  hb_ਸਦਿਸ਼_t<ਦਸ਼ਮਲਵ> normalized_mins;
  hb_ਸਦਿਸ਼_t<ਦਸ਼ਮਲਵ> normalized_defaults;
  hb_ਸਦਿਸ਼_t<ਦਸ਼ਮਲਵ> normalized_maxs;
  ਜੇ (has_avar)
  {
    ਜੇ (!plan->check_success (normalized_mins.resize (axes.ਲੰਮਾਈ)) ||
        !plan->check_success (normalized_defaults.resize (axes.ਲੰਮਾਈ)) ||
        !plan->check_success (normalized_maxs.resize (axes.ਲੰਮਾਈ)))
      ਵਾਪਸ ਝੂਠ;
  }

  ਤਾਰਕਿਕ axis_not_pinned = ਝੂਠ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ new_axis_idx = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਆਖਰੀ_idx = 0;
  ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& _ : + hb_enumerate (axes))
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ i = _.ਪਹਿਲਾ;
    ਅਚਲ OT::Axis_ਰਿਕਾਰਡ &axis = _.second;
    hb_ਟੈਗ_t axis_ਟੈਗ_2 = axis.get_axis_ਟੈਗ ();
    plan->axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map.ਸੈੱਟ (i, axis_ਟੈਗ_2);

    ਜੇ (!plan->user_axes_location.has (axis_ਟੈਗ_2) ||
        !plan->user_axes_location.get (axis_ਟੈਗ_2).is_ਬਿੰਦੂ ())
    {
      axis_not_pinned = ਸੱਚ;
      plan->axes_ਸੂਚਕਾਂਕ_map.ਸੈੱਟ (i, new_axis_idx);
      plan->axis_ਟੈਗ_ਸੂਚੀ.push (axis_ਟੈਗ_2);
      new_axis_idx++;
    }

    Triple *axis_range;
    ਜੇ (plan->user_axes_location.has (axis_ਟੈਗ_2, &axis_range))
    {
      plan->axes_triple_distances.ਸੈੱਟ (axis_ਟੈਗ_2, axis.get_triple_distances ());

      ਦਸ਼ਮਲਵ normalized_min = axis.normalize_axis_ਮੁੱਲ (axis_range->minimum);
      ਦਸ਼ਮਲਵ normalized_default = axis.normalize_axis_ਮੁੱਲ (axis_range->ਵਿਚਕਾਰ);
      ਦਸ਼ਮਲਵ normalized_max = axis.normalize_axis_ਮੁੱਲ (axis_range->ਵੱਧ_ਤੋਂ_ਵੱਧ);

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

      ਜੇ (has_avar)
      {
        normalized_mins[i] = normalized_min;
        normalized_defaults[i] = normalized_default;
        normalized_maxs[i] = normalized_max;
        ਆਖਰੀ_idx = i;
      }
      ਨਹੀਂਤਾਂ
      {
        plan->axes_location.ਸੈੱਟ (axis_ਟੈਗ_2, Triple ((ਦੁੱਗਣਾ) normalized_min,
                                                   (ਦੁੱਗਣਾ) normalized_default,
                                                   (ਦੁੱਗਣਾ) normalized_max));
        ਜੇ (normalized_default == -0.f)
          normalized_default = 0.f; // Normalize -0 to 0
        ਜੇ (normalized_default != 0.f)
          plan->pinned_at_default = ਝੂਠ;

        plan->normalized_coords[i] = roundf (normalized_default * 16384.f);
      }
    }
  }
  plan->all_axes_pinned = !axis_not_pinned;

  // TODO: use avar map_coords_16_16() when normalization is changed to 16.16
  // in fonttools
  ਜੇ (has_avar)
  {
    ਅਚਲ OT::avar* avar_ਸਾਰਣੀ = face->ਸਾਰਣੀ.avar;
    ਬਿਨਾਨਿਸ਼ਾਨੀ coords_len = ਆਖਰੀ_idx + 1;

    ਜੇ (avar_ਸਾਰਣੀ->has_v2_ਅੰਕੜੇ () && !plan->all_axes_pinned)
    {
      /* avar2 partial instancing: use v1-only mapping to get intermediate-space
       * coords. These are used for IVS rebasing and offset compensation. */
      plan->has_avar2 = ਸੱਚ;

      ਜੇ (!plan->check_success (avar_ਸਾਰਣੀ->map_coords_2_14 (normalized_mins.arrayZ, coords_len, ਸੱਚ)) ||
          !plan->check_success (avar_ਸਾਰਣੀ->map_coords_2_14 (normalized_defaults.arrayZ, coords_len, ਸੱਚ)) ||
          !plan->check_success (avar_ਸਾਰਣੀ->map_coords_2_14 (normalized_maxs.arrayZ, coords_len, ਸੱਚ)))
        ਵਾਪਸ ਝੂਠ;

      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& _ : + hb_enumerate (axes))
      {
        ਬਿਨਾਨਿਸ਼ਾਨੀ i = _.ਪਹਿਲਾ;
        hb_ਟੈਗ_t axis_ਟੈਗ_2 = _.second.get_axis_ਟੈਗ ();
        ਜੇ (plan->user_axes_location.has (axis_ਟੈਗ_2))
        {
          /* Store intermediate-space coords for offset compensation,
           * segment-map renormalization, and IVS rebasing. */
          plan->old_intermediates.ਸੈੱਟ (axis_ਟੈਗ_2, Triple ((ਦੁੱਗਣਾ) normalized_mins[i],
                                                         (ਦੁੱਗਣਾ) normalized_defaults[i],
                                                         (ਦੁੱਗਣਾ) normalized_maxs[i]));
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
      hb_blob_t *cff2_blob = hb_face_reference_ਸਾਰਣੀ (face, HB_ਟੈਗ ('C','F','F','2'));
      ਤਾਰਕਿਕ detect_self_contained = !hb_blob_get_ਲੰਮਾਈ (cff2_blob);
      hb_blob_destroy (cff2_blob);
      ਜੇ (!_compute_avar2_reachable_ranges (plan, axes, avar_ਸਾਰਣੀ,
                                            detect_self_contained))
        ਵਾਪਸ ਝੂਠ;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR_COMPOSITES
      /* VARC overrides can reach coordinates outside the font-level
       * ranges. Do not cull variation data using those ranges. */
      ਜੇ (!plan->drop_tables.has (HB_ਟੈਗ ('V','A','R','C')))
      {
        hb_blob_t *varc_blob = hb_face_reference_ਸਾਰਣੀ (face, HB_ਟੈਗ ('V','A','R','C'));
        ਜੇ (hb_blob_get_ਲੰਮਾਈ (varc_blob))
          plan->avar2_reachable_ranges.reset ();
        hb_blob_destroy (varc_blob);
      }
#ਜੇ_ਅੰਤ

      /* Keep all axes in fvar (pinned ones as hidden), EXCEPT self-contained
       * pinned axes, which are removed entirely. */
      plan->axes_ਸੂਚਕਾਂਕ_map.reset ();
      plan->axis_ਟੈਗ_ਸੂਚੀ.reset ();
      ਬਿਨਾਨਿਸ਼ਾਨੀ retained_axis_idx = 0;
      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& _ : + hb_enumerate (axes))
      {
        ਬਿਨਾਨਿਸ਼ਾਨੀ i = _.ਪਹਿਲਾ;
        hb_ਟੈਗ_t axis_ਟੈਗ_2 = _.second.get_axis_ਟੈਗ ();
        ਜੇ (plan->avar2_self_contained.has (axis_ਟੈਗ_2))
          ਜਾਰੀ;
        plan->axes_ਸੂਚਕਾਂਕ_map.ਸੈੱਟ (i, retained_axis_idx++);
        plan->axis_ਟੈਗ_ਸੂਚੀ.push (axis_ਟੈਗ_2);
      }
      /* The other tables see ONLY the self-contained pins, as an ordinary
       * partial instancing in old final-coordinate space; the standard
       * instancing machinery bakes those axes' contributions in. The
       * remaining restriction is carried entirely by avar. */
      plan->axes_location.reset ();
      ਜੇ (plan->avar2_self_contained.get_population ())
      {
        ਤਾਰਕਿਕ sc_pinned_at_default = ਸੱਚ;
        ਲਈ (ਸਵੈਚਾਲਿਤ _ : plan->avar2_self_contained)
        {
          plan->axes_location.ਸੈੱਟ (_.ਪਹਿਲਾ, Triple (_.second, _.second, _.second));
          ਜੇ (_.second != 0.0)
            sc_pinned_at_default = ਝੂਠ;
        }
        ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& _ : + hb_enumerate (axes))
        {
          ਦੁੱਗਣਾ *v;
          plan->normalized_coords[_.ਪਹਿਲਾ] =
              plan->avar2_self_contained.has (_.second.get_axis_ਟੈਗ (), &v)
              ? (ਪੂਰਨ) roundf ((ਦਸ਼ਮਲਵ) (*v * 16384.0)) : 0;
        }
        plan->pinned_at_default = sc_pinned_at_default;
      }
      ਨਹੀਂਤਾਂ
        plan->normalized_coords.shrink (0);
    }
    ਨਹੀਂਤਾਂ
    {
      /* Standard avar v1 (or v1+v2 with all axes pinned) */
      ਜੇ (!plan->check_success (avar_ਸਾਰਣੀ->map_coords_2_14 (normalized_mins.arrayZ, coords_len)) ||
          !plan->check_success (avar_ਸਾਰਣੀ->map_coords_2_14 (normalized_defaults.arrayZ, coords_len)) ||
          !plan->check_success (avar_ਸਾਰਣੀ->map_coords_2_14 (normalized_maxs.arrayZ, coords_len)))
        ਵਾਪਸ ਝੂਠ;

      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& _ : + hb_enumerate (axes))
      {
        ਬਿਨਾਨਿਸ਼ਾਨੀ i = _.ਪਹਿਲਾ;
        hb_ਟੈਗ_t axis_ਟੈਗ_2 = _.second.get_axis_ਟੈਗ ();
        ਜੇ (plan->user_axes_location.has (axis_ਟੈਗ_2))
        {
          plan->axes_location.ਸੈੱਟ (axis_ਟੈਗ_2, Triple ((ਦੁੱਗਣਾ) normalized_mins[i],
                                                     (ਦੁੱਗਣਾ) normalized_defaults[i],
                                                     (ਦੁੱਗਣਾ) normalized_maxs[i]));
          ਦਸ਼ਮਲਵ normalized_default = normalized_defaults[i];
          ਜੇ (normalized_default == -0.f)
            normalized_default = 0.f;
          ਜੇ (normalized_default != 0.f)
            plan->pinned_at_default = ਝੂਠ;

          plan->normalized_coords[i] = roundf (normalized_default * 16384.f);
        }
      }
    }
  }
  ਵਾਪਸ ਸੱਚ;
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_FONT_CFF
ਖਾਲੀ
update_instance_metrics_map_from_cff2 (hb_subset_plan_t *plan)
{
  ਜੇ (!plan->normalized_coords) ਵਾਪਸ;
  OT::cff2::accelerator_t cff2 (plan->ਸਰੋਤ);
  ਜੇ (!cff2.is_valid ()) ਵਾਪਸ;

  hb_font_t *font = _get_hb_font_with_variations (plan);
  ਜੇ (unlikely (!plan->check_success (font != ਖਾਲੀ_ਸੂਚਕ)))
  {
    hb_font_destroy (font);
    ਵਾਪਸ;
  }

  hb_glyph_extents_t extents = {0x7FFF, -0x7FFF};
  OT::hmtx_accelerator_t _hmtx (plan->ਸਰੋਤ);
  OT::hb_scalar_cache_t *hvar_store_cache = ਖਾਲੀ_ਸੂਚਕ;
  ਜੇ (_hmtx.has_ਅੰਕੜੇ () && _hmtx.var_ਸਾਰਣੀ.get_ਲੰਮਾਈ ())
    hvar_store_cache = _hmtx.var_ਸਾਰਣੀ->get_var_store ().create_cache ();

  OT::vmtx_accelerator_t _vmtx (plan->ਸਰੋਤ);
  OT::hb_scalar_cache_t *vvar_store_cache = ਖਾਲੀ_ਸੂਚਕ;
  ਜੇ (_vmtx.has_ਅੰਕੜੇ () && _vmtx.var_ਸਾਰਣੀ.get_ਲੰਮਾਈ ())
    vvar_store_cache = _vmtx.var_ਸਾਰਣੀ->get_var_store ().create_cache ();

  ਲਈ (ਸਵੈਚਾਲਿਤ p : *plan->glyph_map)
  {
    hb_codepoint_t old_gid = p.ਪਹਿਲਾ;
    hb_codepoint_t new_gid = p.second;
    ਜੇ (!cff2.get_extents (font, old_gid, &extents)) ਜਾਰੀ;
    ਤਾਰਕਿਕ has_bounds_info = ਸੱਚ;
    ਜੇ (extents.x_bearing == 0 && extents.ਚੌੜਾਈ == 0 &&
        extents.ਉਚਾਈ == 0 && extents.y_bearing == 0)
      has_bounds_info = ਝੂਠ;

    ਜੇ (has_bounds_info)
    {
      plan->ਸਿਰ_maxp_info.xMin = hb_min (plan->ਸਿਰ_maxp_info.xMin, extents.x_bearing);
      plan->ਸਿਰ_maxp_info.xMax = hb_max (plan->ਸਿਰ_maxp_info.xMax,
						 hb_saturate_ਜੋੜੋ (extents.x_bearing, extents.ਚੌੜਾਈ));
      plan->ਸਿਰ_maxp_info.yMax = hb_max (plan->ਸਿਰ_maxp_info.yMax, extents.y_bearing);
      plan->ਸਿਰ_maxp_info.yMin = hb_min (plan->ਸਿਰ_maxp_info.yMin,
						 hb_saturate_ਜੋੜੋ (extents.y_bearing, extents.ਉਚਾਈ));
    }

    ਜੇ (_hmtx.has_ਅੰਕੜੇ ())
    {
      ਦੁੱਗਣਾ hori_aw = _hmtx.get_advance_without_var_unscaled (old_gid);
      ਜੇ (_hmtx.var_ਸਾਰਣੀ.get_ਲੰਮਾਈ ())
        hori_aw += (ਦੁੱਗਣਾ) roundf (_hmtx.var_ਸਾਰਣੀ->get_advance_delta_unscaled (old_gid, font->coords, font->ਅੰਕ_coords,
									 hvar_store_cache));
      /* Malicious deltas can take the advance out of the UFWORD range that
       * hmtx serialization stores.  Clamp, to keep downstream consumers
       * (e.g. CFF width optimizer) bounded and consistent with hmtx. */
      hori_aw = hb_clamp (hori_aw, 0., 65535.);
      ਪੂਰਨ lsb = extents.x_bearing;
      ਜੇ (!has_bounds_info)
      {
        _hmtx.get_leading_bearing_without_var_unscaled (old_gid, &lsb);
      }
      plan->hmtx_map.ਸੈੱਟ (new_gid, hb_pair ((ਬਿਨਾਨਿਸ਼ਾਨੀ) hori_aw, lsb));
      plan->bounds_ਚੌੜਾਈ_vec[new_gid] = extents.ਚੌੜਾਈ;
    }

    ਜੇ (_vmtx.has_ਅੰਕੜੇ ())
    {
      ਦੁੱਗਣਾ vert_aw = _vmtx.get_advance_without_var_unscaled (old_gid);
      ਜੇ (_vmtx.var_ਸਾਰਣੀ.get_ਲੰਮਾਈ ())
        vert_aw += (ਦੁੱਗਣਾ) roundf (_vmtx.var_ਸਾਰਣੀ->get_advance_delta_unscaled (old_gid, font->coords, font->ਅੰਕ_coords,
									 vvar_store_cache));
      vert_aw = hb_clamp (vert_aw, 0., 65535.);
      hb_position_t vorg_x = 0;
      hb_position_t vorg_y = 0;
      ਪੂਰਨ tsb = 0;
      ਜੇ (has_bounds_info &&
           hb_font_get_glyph_v_origin (font, old_gid, &vorg_x, &vorg_y))
      {
        tsb = hb_saturate_sub (vorg_y, extents.y_bearing);
      } ਨਹੀਂਤਾਂ {
        _vmtx.get_leading_bearing_without_var_unscaled (old_gid, &tsb);
      }

      plan->vmtx_map.ਸੈੱਟ (new_gid, hb_pair ((ਬਿਨਾਨਿਸ਼ਾਨੀ) vert_aw, tsb));
      plan->bounds_ਉਚਾਈ_vec[new_gid] = extents.ਉਚਾਈ;
    }
  }
  /* No glyph contributed bounds: do not serialize the accumulator's
   * inverted sentinels into head (or a downgraded CFF FontBBox). */
  ਜੇ (plan->ਸਿਰ_maxp_info.xMin > plan->ਸਿਰ_maxp_info.xMax)
  {
    plan->ਸਿਰ_maxp_info.xMin = plan->ਸਿਰ_maxp_info.xMax = 0;
    plan->ਸਿਰ_maxp_info.yMin = plan->ਸਿਰ_maxp_info.yMax = 0;
  }
  hb_font_destroy (font);
  ਜੇ (hvar_store_cache)
    _hmtx.var_ਸਾਰਣੀ->get_var_store ().destroy_cache (hvar_store_cache);
  ਜੇ (vvar_store_cache)
    _vmtx.var_ਸਾਰਣੀ->get_var_store ().destroy_cache (vvar_store_cache);
}
#ਜੇ_ਅੰਤ

ਤਾਰਕਿਕ
get_instance_glyphs_contour_points (hb_subset_plan_t *plan)
{
  /* contour_points vector only needed for updating gvar table (infer delta and
   * iup delta optimization) during partial instancing */
  ਜੇ (plan->user_axes_location.is_empty () || plan->all_axes_pinned)
    ਵਾਪਸ ਸੱਚ;

  OT::glyf_accelerator_t glyf (plan->ਸਰੋਤ);

  ਲਈ (ਸਵੈਚਾਲਿਤ &_ : plan->new_ਮੰਜ਼ਿਲ_old_gid_list)
  {
    hb_codepoint_t new_gid = _.ਪਹਿਲਾ;
    contour_ਬਿੰਦੂ_ਸਦਿਸ਼_t all_points;
    ਜੇ (new_gid == 0 && !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      ਜੇ (unlikely (!plan->new_gid_contour_points_map.ਸੈੱਟ (new_gid, all_points)))
        ਵਾਪਸ ਝੂਠ;
      ਜਾਰੀ;
    }

    hb_codepoint_t old_gid = _.second;
    ਸਵੈਚਾਲਿਤ glyph = glyf.glyph_for_gid (old_gid);
    ਜੇ (unlikely (!glyph.get_all_points_without_var (plan->ਸਰੋਤ, all_points)))
      ਵਾਪਸ ਝੂਠ;
    ਜੇ (unlikely (!plan->new_gid_contour_points_map.ਸੈੱਟ (new_gid, all_points)))
      ਵਾਪਸ ਝੂਠ;

    /* composite new gids are only needed by iup delta optimization */
    ਜੇ ((plan->flags & HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS) && glyph.is_ਸੰਯੁਕਤ ())
      plan->ਸੰਯੁਕਤ_new_gids.ਜੋੜੋ (new_gid);
  }
  ਵਾਪਸ ਸੱਚ;
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map>
ਖਾਲੀ
remap_colrv1_delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_indices (ਅਚਲ Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map &ਸੂਚਕਾਂਕ_map,
                                      ਅਚਲ hb_ਸੈੱਟ_t &delta_ਸੈੱਟ_idxes,
                                      hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /*ਬਾਹਰਲਾ*/)
{
  ਜੇ (!ਸੂਚਕਾਂਕ_map.get_map_ਗਿਣਤੀ ())
    ਵਾਪਸ;

  hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> delta_ਸੈੱਟ_idx_delta_map;
  ਬਿਨਾਨਿਸ਼ਾਨੀ new_delta_ਸੈੱਟ_idx = 0;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ delta_ਸੈੱਟ_idx : delta_ਸੈੱਟ_idxes)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ var_idx = ਸੂਚਕਾਂਕ_map.map (delta_ਸੈੱਟ_idx);
    ਬਿਨਾਨਿਸ਼ਾਨੀ new_varidx = HB_OT_LAYOUT_NO_VARIATIONS_ਸੂਚਕਾਂਕ;
    ਪੂਰਨ delta = 0;

    ਜੇ (var_idx != HB_OT_LAYOUT_NO_VARIATIONS_ਸੂਚਕਾਂਕ)
    {
      hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ> *new_varidx_delta;
      ਜੇ (!variation_idx_delta_map.has (var_idx, &new_varidx_delta)) ਜਾਰੀ;

      new_varidx = hb_ਪਹਿਲਾ (*new_varidx_delta);
      delta = hb_ਦੂਜਾ (*new_varidx_delta);
    }

    new_deltaset_idx_varidx_map.ਸੈੱਟ (new_delta_ਸੈੱਟ_idx, new_varidx);
    delta_ਸੈੱਟ_idx_delta_map.ਸੈੱਟ (delta_ਸੈੱਟ_idx, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ> (new_delta_ਸੈੱਟ_idx, delta));
    new_delta_ਸੈੱਟ_idx++;
  }
  variation_idx_delta_map = std::move (delta_ਸੈੱਟ_idx_delta_map);
}

ਸਾਂਚਾ ਖਾਲੀ
remap_colrv1_delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_indices<OT::Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map> (ਅਚਲ OT::Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map &ਸੂਚਕਾਂਕ_map,
                                      ਅਚਲ hb_ਸੈੱਟ_t &delta_ਸੈੱਟ_idxes,
                                      hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /*ਬਾਹਰਲਾ*/);

 #ਜੇ_ਅੰਤ

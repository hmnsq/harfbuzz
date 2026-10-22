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

 #ଅନ୍ତର୍ଭାଵ "hb-ot-layout-common.hh"
#ଅନ୍ତର୍ଭାଵ "hb-subset-plan.hh"

 #ଅନ୍ତର୍ଭାଵ "hb-ot-var-common.hh"
 #ଅନ୍ତର୍ଭାଵ "hb-ot-layout-base-table.hh"
 #ଅନ୍ତର୍ଭାଵ "hb-ot-glyf-table.hh"
 #ଅନ୍ତର୍ଭାଵ "hb-ot-var-fvar-table.hh"
 #ଅନ୍ତର୍ଭାଵ "hb-ot-var-avar-table.hh"
 #ଅନ୍ତର୍ଭାଵ "hb-ot-cff2-table.hh"

 #ifndef HB_NO_VAR

 ଶୂନ୍ୟ
 generate_varstore_inner_maps (ଅଚଳ hb_set_t& varidx_set,
                               unsigned subtable_ଗଣନା,
                               hb_vector_t<hb_inc_bimap_t> &inner_maps /* OUT */)
 {
   ଯଦି (varidx_set.is_empty () || subtable_ଗଣନା == 0) ଫେରନ୍ତୁ;

   ଯଦି (unlikely (!inner_maps.resize (subtable_ଗଣନା))) ଫେରନ୍ତୁ;
   ପାଇଁ (unsigned idx : varidx_set)
   {
     uint16_t major = idx >> 16;
     uint16_t minor = idx & 0xFFFF;

     ଯଦି (major >= subtable_ଗଣନା)
       ଜାରି;
     inner_maps[major].add (minor);
   }
 }

 /* Evaluate a variation-region tent at v, mirroring VarRegionAxis::evaluate
  * (invalid regions evaluate as constant 1). */
 ସ୍ଥିର ଦ୍ୱିଗୁଣ
 _tent_eval (ଅଚଳ Triple &tent, ଦ୍ୱିଗୁଣ v)
 {
   ଦ୍ୱିଗୁଣ start = (ଦ୍ୱିଗୁଣ) tent.minimum;
   ଦ୍ୱିଗୁଣ peak = (ଦ୍ୱିଗୁଣ) tent.ମଧ୍ୟ;
   ଦ୍ୱିଗୁଣ end = (ଦ୍ୱିଗୁଣ) tent.maximum;
   ଯଦି (unlikely (start > peak || peak > end)) ଫେରନ୍ତୁ 1.0;
   ଯଦି (unlikely (start < 0.0 && end > 0.0 && peak != 0.0)) ଫେରନ୍ତୁ 1.0;
   ଯଦି (peak == 0.0 || v == peak) ଫେରନ୍ତୁ 1.0;
   ଯଦି (v <= start || end <= v) ଫେରନ୍ତୁ 0.0;
   ଯଦି (v < peak) ଫେରନ୍ତୁ (v - start) / (peak - start);
   ଫେରନ୍ତୁ (end - v) / (end - peak);
 }

 /* Range of a tent's scalar over an input interval [lo, hi]. The tent is
  * unimodal, so the extremes are at the interval endpoints, plus the peak
  * when it falls inside. */
 ସ୍ଥିର ଶୂନ୍ୟ
 tent_ମୂଲ୍ୟ_range (ଅଚଳ Triple &tent, ଦ୍ୱିଗୁଣ lo, ଦ୍ୱିଗୁଣ hi,
                    ଦ୍ୱିଗୁଣ *tmin, ଦ୍ୱିଗୁଣ *tmax)
 {
   ଦ୍ୱିଗୁଣ a = _tent_eval (tent, lo);
   ଦ୍ୱିଗୁଣ b = _tent_eval (tent, hi);
   *tmin = hb_min (a, b);
   *tmax = hb_max (a, b);
   ଯଦି (lo <= (ଦ୍ୱିଗୁଣ) tent.ମଧ୍ୟ && (ଦ୍ୱିଗୁଣ) tent.ମଧ୍ୟ <= hi)
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
 ସ୍ଥିର ତାର୍କିକ
 _compute_avar2_reachable_ranges (hb_subset_plan_t *plan,
                                  hb_array_t<ଅଚଳ OT::AxisRecord> axes,
                                  ଅଚଳ OT::avar *avar_table,
                                  ତାର୍କିକ detect_self_contained)
 {
   ଅଚଳ OT::ItemVariationStore *var_store;
   ଅଚଳ OT::DeltaSetIndexMap *varidx_map;
   ଯଦି (!avar_table->get_v2_store_and_map (&var_store, &varidx_map))
     ଫେରନ୍ତୁ ମିଥ୍ୟା;

   /* Old-intermediate input box per axis. */
   hb_hashmap_t<hb_tag_t, hb_pair_t<ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ>> box;
   ପାଇଁ (ଅଚଳ auto &axis : axes)
   {
     hb_tag_t tag = axis.get_axis_tag ();
     ଦ୍ୱିଗୁଣ lo = -1.0, hi = +1.0;
     Triple *old_int;
     ଯଦି (plan->user_axes_location.has (tag))
     {
       ଯଦି (!plan->old_intermediates.has (tag, &old_int)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
       lo = (ଦ୍ୱିଗୁଣ) old_int->minimum;
       hi = (ଦ୍ୱିଗୁଣ) old_int->maximum;
     }
     ଅନ୍ୟଥା ଯଦି (axis.is_hidden ())
       lo = hi = 0.0; /* private axis: intermediate is always 0 */
     ଯଦି (!box.set (tag, hb_pair (lo, hi))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
   }

   hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> regions;
   ଯଦି (!var_store->get_region_list ().get_var_regions (plan->axes_old_index_tag_map, regions))
     ଫେରନ୍ତୁ ମିଥ୍ୟା;

   hb_hashmap_t<hb_tag_t, unsigned> grid_pos;
   hb_vector_t<hb_vector_t<ଦ୍ୱିଗୁଣ>> grid_points;
   ପାଇଁ (unsigned i = 0; i < axes.length; i++)
   {
     grid_pos.reset ();
     grid_points.reset ();

     hb_tag_t tag = axes[i].get_axis_tag ();
     hb_pair_t<ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ> *identity;
     ଯଦି (!box.has (tag, &identity)) ଫେରନ୍ତୁ ମିଥ୍ୟା;

     /* Collect the row's active regions (non-zero delta). A varidx (or the
      * implicit identity mapping) that doesn't resolve to a real store row
      * behaves as "no variation" at runtime, so it contributes no regions;
      * this also covers a NULL VarStore. */
     hb_vector_t<hb_pair_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, unsigned>> active; /* (delta, region index) */
     uint32_t varidx = varidx_map->map (i);
     /* Runtime treats out-of-range delta-set indices as zero. */
     ଯଦି (varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX &&
	 !var_store->has_delta_set (varidx))
       varidx = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
     ଯଦି (varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
     {
       unsigned outer = varidx >> 16;
       unsigned inner = varidx & 0xFFFF;
       ଅଚଳ OT::VarData &vd = var_store->get_sub_table (outer);
       ଅଚଳ OT::HBUINT8 *delta_bytes = vd.get_delta_bytes ();
       unsigned row_size = vd.get_row_size ();
       unsigned region_ଗଣନା_2 = vd.get_region_index_ଗଣନା ();
       ପାଇଁ (unsigned r = 0; r < region_ଗଣନା_2; r++)
       {
	 ପୂର୍ଣ୍ଣସଂଖ୍ୟା delta = vd.get_item_delta_fast (inner, r, delta_bytes, row_size);
	 ଯଦି (!delta) ଜାରି;
	 unsigned region_idx = vd.get_region_index (r);
	 ଯଦି (region_idx >= regions.length) ଫେରନ୍ତୁ ମିଥ୍ୟା;
	 active.push (hb_pair (delta, region_idx));
       }
       ଯଦି (active.in_error ()) ଫେରନ୍ତୁ ମିଥ୍ୟା;
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
     ତାର୍କିକ exact = ସତ୍ୟ;
     ଦ୍ୱିଗୁଣ dmin = 0.0, dmax = 0.0; /* delta extremes, F2Dot14 units */
     ଦ୍ୱିଗୁଣ vmin = 0.0, vmax = 0.0; /* identity + delta extremes, coord units */

     /* Grid axes: the target axis (identity term), plus every axis a valid
      * tent of an active region references. */
     auto add_grid_axis = [&] (hb_tag_t t) -> ପୂର୍ଣ୍ଣସଂଖ୍ୟା
     {
       unsigned *pos;
       ଯଦି (grid_pos.has (t, &pos)) ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) *pos;
       unsigned new_pos = grid_pos.get_population ();
       hb_pair_t<ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ> *ନିବେଶ;
       ଦ୍ୱିଗୁଣ blo = -1.0, bhi = +1.0;
       ଯଦି (box.has (t, &ନିବେଶ)) { blo = ନିବେଶ->first; bhi = ନିବେଶ->second; }
       grid_points.push (hb_vector_t<ଦ୍ୱିଗୁଣ> ());
       ଯଦି (grid_points.in_error ()) ଫେରନ୍ତୁ -1;
       auto &points = grid_points[new_pos];
       points.push (blo);
       points.push (bhi);
       ଯଦି (points.in_error () || !grid_pos.set (t, new_pos)) ଫେରନ୍ତୁ -1;
       ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) new_pos;
     };

     ଯଦି (add_grid_axis (tag) < 0) ଫେରନ୍ତୁ ମିଥ୍ୟା;
     /* Per active region: (grid axis position, tent) for its valid tents. */
     hb_vector_t<hb_vector_t<hb_pair_t<unsigned, Triple>>> region_tents;
     ପାଇଁ (ଅଚଳ auto &ar : active)
     {
       region_tents.push (hb_vector_t<hb_pair_t<unsigned, Triple>> ());
       ଯଦି (region_tents.in_error ()) ଫେରନ୍ତୁ ମିଥ୍ୟା;
       auto &tents = region_tents[region_tents.length - 1];
       ପାଇଁ (ଅଚଳ auto &_ : regions[ar.second])
       {
	 ଅଚଳ Triple &tent = _.second;
	 /* Tents the runtime ignores (constant scalar 1) add no breakpoints. */
	 ଯଦି (tent.ମଧ୍ୟ == 0.f ||
	     tent.minimum > tent.ମଧ୍ୟ || tent.ମଧ୍ୟ > tent.maximum ||
	     (tent.minimum < 0.f && tent.maximum > 0.f))
	   ଜାରି;
	 ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos = add_grid_axis (_.first);
	 ଯଦି (pos < 0) ଫେରନ୍ତୁ ମିଥ୍ୟା;
	 auto &points = grid_points[pos];
	 ଦ୍ୱିଗୁଣ blo = points[0], bhi = points[1];
	 points.push (hb_clamp ((ଦ୍ୱିଗୁଣ) tent.minimum, blo, bhi));
	 points.push (hb_clamp ((ଦ୍ୱିଗୁଣ) tent.ମଧ୍ୟ, blo, bhi));
	 points.push (hb_clamp ((ଦ୍ୱିଗୁଣ) tent.maximum, blo, bhi));
	 tents.push (hb_pair ((unsigned) pos, tent));
	 ଯଦି (points.in_error () || tents.in_error ()) ଫେରନ୍ତୁ ମିଥ୍ୟା;
       }
     }

     unsigned grid_size = 1;
     ପାଇଁ (auto &points : grid_points)
     {
       points.qsort ([] (ଅଚଳ ଦ୍ୱିଗୁଣ &a, ଅଚଳ ଦ୍ୱିଗୁଣ &b) -> ପୂର୍ଣ୍ଣସଂଖ୍ୟା
		     { ଫେରନ୍ତୁ a < b ? -1 : a > b ? +1 : 0; });
       unsigned n = 0;
       ପାଇଁ (unsigned j = 0; j < points.length; j++)
	 ଯଦି (!n || points.arrayZ[j] != points.arrayZ[n - 1])
	   points.arrayZ[n++] = points.arrayZ[j];
       points.shrink (n);
       ଯଦି (grid_size > MAX_GRID / points.length) { exact = ମିଥ୍ୟା; ଭାଙ୍ଗ; }
       grid_size *= points.length;
     }

     ଯଦି (exact)
     {
       unsigned target_pos = 0; /* the target axis was added first */
       hb_vector_t<unsigned> odometer;
       ଯଦି (!odometer.resize (grid_pos.get_population ())) ଫେରନ୍ତୁ ମିଥ୍ୟା;
       ତାର୍କିକ first = ସତ୍ୟ;
       ଯେତେବେଳେ (ସତ୍ୟ)
       {
	 ଦ୍ୱିଗୁଣ delta_sum = 0.0;
	 ପାଇଁ (unsigned r = 0; r < active.length; r++)
	 {
	   ଦ୍ୱିଗୁଣ scalar = 1.0;
	   ପାଇଁ (ଅଚଳ auto &pt : region_tents[r])
	   {
	     ଦ୍ୱିଗୁଣ v = grid_points[pt.first][odometer[pt.first]];
	     scalar *= _tent_eval (pt.second, v);
	     ଯଦି (scalar == 0.0) ଭାଙ୍ଗ;
	   }
	   delta_sum += scalar * active[r].first;
	 }
	 ଦ୍ୱିଗୁଣ coord = grid_points[target_pos][odometer[target_pos]];
	 ଦ୍ୱିଗୁଣ value = coord + delta_sum / 16384.0;
	 ଯଦି (first)
	 {
	   dmin = dmax = delta_sum;
	   vmin = vmax = value;
	   first = ମିଥ୍ୟା;
	 }
	 ଅନ୍ୟଥା
	 {
	   dmin = hb_min (dmin, delta_sum); dmax = hb_max (dmax, delta_sum);
	   vmin = hb_min (vmin, value);     vmax = hb_max (vmax, value);
	 }
	 unsigned k = 0;
	 ପାଇଁ (; k < odometer.length; k++)
	 {
	   ଯଦି (++odometer[k] < grid_points[k].length) ଭାଙ୍ଗ;
	   odometer[k] = 0;
	 }
	 ଯଦି (k == odometer.length) ଭାଙ୍ଗ;
       }
     }
     ଅନ୍ୟଥା
     {
       ପାଇଁ (ଅଚଳ auto &ar : active)
       {
	 ପୂର୍ଣ୍ଣସଂଖ୍ୟା delta = ar.first;
	 ଦ୍ୱିଗୁଣ smin = 1.0, smax = 1.0;
	 ପାଇଁ (ଅଚଳ auto &_ : regions[ar.second])
	 {
	   hb_pair_t<ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ> *ନିବେଶ;
	   ଦ୍ୱିଗୁଣ blo = -1.0, bhi = +1.0;
	   ଯଦି (box.has (_.first, &ନିବେଶ)) { blo = ନିବେଶ->first; bhi = ନିବେଶ->second; }
	   ଦ୍ୱିଗୁଣ tmin, tmax;
	   tent_ମୂଲ୍ୟ_range (_.second, blo, bhi, &tmin, &tmax);
	   smin *= tmin;
	   smax *= tmax;
	   ଯଦି (smax == 0.0) ଭାଙ୍ଗ;
	 }
	 ଯଦି (delta > 0) { dmin += smin * delta; dmax += smax * delta; }
	 ଅନ୍ୟଥା           { dmin += smax * delta; dmax += smin * delta; }
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
     ଯଦି (detect_self_contained && dmin == dmax &&
	 plan->user_axes_location.has (tag, &user) && user->is_point ())
     {
       ପୂର୍ଣ୍ଣସଂଖ୍ୟା v_int = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) roundf ((ଦଶମିକ) identity->first * 16384.f) +
		   (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) roundf ((ଦଶମିକ) dmin);
       v_int = hb_clamp (v_int, -(1 << 14), +(1 << 14));
       ଯଦି (!plan->avar2_self_contained.set (tag, v_int / 16384.0)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
       ଜାରି; /* axis is dropped; no region can reference it afterwards */
     }

     /* Pad by one F2Dot14 unit against rounding differences with the
      * runtime's fixed-point evaluation. */
     ଦ୍ୱିଗୁଣ lo = hb_clamp (vmin - 1.0 / 16384.0, -1.0, +1.0);
     ଦ୍ୱିଗୁଣ hi = hb_clamp (vmax + 1.0 / 16384.0, -1.0, +1.0);
     lo = floor (lo * 16384.0) / 16384.0;
     hi = ceil (hi * 16384.0) / 16384.0;
     ଯଦି (lo <= -1.0 && hi >= +1.0)
       ଜାରି; /* not constraining */
     ଯଦି (!plan->avar2_reachable_ranges.set (tag, Triple (lo, hb_clamp (0.0, lo, hi), hi)))
       ଫେରନ୍ତୁ ମିଥ୍ୟା;
   }
   ଫେରନ୍ତୁ ସତ୍ୟ;
 }

#ifndef HB_NO_OT_FONT_CFF
 ସ୍ଥିର ଧାଡ଼ିଭିତରେ hb_font_t*
 _get_hb_font_with_variations (ଅଚଳ hb_subset_plan_t *plan)
 {
   hb_font_t *font = hb_font_create (plan->source);

   ଯଦି (plan->has_avar2)
   {
     /* Under avar2, instancing applies only to the self-contained pins,
      * whose constant final coordinates the plan holds in normalized_coords.
      * Setting user-space variations would run the full avar2 mapping and
      * bake contributions that remain live in the instance's variations. */
     hb_font_set_var_coords_normalized (font, plan->normalized_coords.arrayZ,
                                        plan->normalized_coords.length);
     ଫେରନ୍ତୁ font;
   }

   hb_vector_t<hb_variation_t> vars;
   ଯଦି (!vars.alloc (plan->user_axes_location.get_population ())) {
     hb_font_destroy (font);
     ଫେରନ୍ତୁ nullptr;
   }

   ପାଇଁ (auto _ : plan->user_axes_location)
   {
     hb_variation_t var;
     var.tag = _.first;
     var.value = _.second.ମଧ୍ୟ;
     vars.push (var);
   }

   hb_font_set_variations (font, vars.arrayZ, plan->user_axes_location.get_population ());
   ଫେରନ୍ତୁ font;
 }
#endif

 template<typename ItemVarStore>
 ଶୂନ୍ୟ
 remap_variation_indices (ଅଚଳ ItemVarStore &var_store,
                          ଅଚଳ hb_set_t &variation_indices,
                          ଅଚଳ hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& normalized_coords,
                          ତାର୍କିକ calculate_delta, /* not pinned at default */
                          ତାର୍କିକ no_variations, /* all axes pinned */
                          hb_hashmap_t<unsigned, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> &variation_idx_delta_map /* OUT */)
 {
   ଯଦି (&var_store == &Null (OT::ItemVariationStore)) ଫେରନ୍ତୁ;
   unsigned subtable_ଗଣନା = var_store.get_sub_table_ଗଣନା ();
   auto *store_cache = var_store.create_cache ();

   unsigned new_major = 0, new_minor = 0;
   unsigned last_major = (variation_indices.get_min ()) >> 16;
   ପାଇଁ (unsigned idx : variation_indices)
   {
     ପୂର୍ଣ୍ଣସଂଖ୍ୟା delta = 0;
     ଯଦି (calculate_delta)
       delta = roundf (var_store.get_delta (idx, normalized_coords.arrayZ,
                                            normalized_coords.length, store_cache));

     ଯଦି (no_variations)
     {
       variation_idx_delta_map.set (idx, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> (HB_OT_LAYOUT_NO_VARIATIONS_INDEX, delta));
       ଜାରି;
     }

     uint16_t major = idx >> 16;
     ଯଦି (major >= subtable_ଗଣନା) ଭାଙ୍ଗ;
     ଯଦି (major != last_major)
     {
       new_minor = 0;
       ++new_major;
     }

     unsigned new_idx = (new_major << 16) + new_minor;
     variation_idx_delta_map.set (idx, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> (new_idx, delta));
     ++new_minor;
     last_major = major;
   }
   var_store.destroy_cache (store_cache);
 }

 template
 ଶୂନ୍ୟ
 remap_variation_indices<OT::ItemVariationStore> (ଅଚଳ OT::ItemVariationStore &var_store,
                          ଅଚଳ hb_set_t &variation_indices,
                          ଅଚଳ hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& normalized_coords,
                          ତାର୍କିକ calculate_delta, /* not pinned at default */
                          ତାର୍କିକ no_variations, /* all axes pinned */
                          hb_hashmap_t<unsigned, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> &variation_idx_delta_map /* OUT */);

 #ifndef HB_NO_BASE
 ଶୂନ୍ୟ
 collect_base_variation_indices (hb_subset_plan_t* plan)
 {
   hb_blob_ptr_t<OT::BASE> base = plan->source_table<OT::BASE> ();
   ଯଦି (!base->has_var_store ())
   {
     base.destroy ();
     ଫେରନ୍ତୁ;
   }

   hb_set_t varidx_set;
   base->collect_variation_indices (plan, varidx_set);
   ଅଚଳ OT::ItemVariationStore &var_store = base->get_var_store ();
   unsigned subtable_ଗଣନା = var_store.get_sub_table_ଗଣନା ();


   remap_variation_indices (var_store, varidx_set,
                             plan->normalized_coords,
                             !plan->pinned_at_default,
                             plan->all_axes_pinned,
                             plan->base_variation_idx_map);
   generate_varstore_inner_maps (varidx_set, subtable_ଗଣନା, plan->base_varstore_inner_maps);

   base.destroy ();
 }

 #endif

ତାର୍କିକ
normalize_axes_location (hb_face_t *face, hb_subset_plan_t *plan)
{
  ଯଦି (plan->user_axes_location.is_empty ())
    ଫେରନ୍ତୁ ସତ୍ୟ;

  hb_array_t<ଅଚଳ OT::AxisRecord> axes = face->table.fvar->get_axes ();
  ପାଇଁ (ଅଚଳ auto &axis : axes)
  {
    hb_tag_t tag = axis.get_axis_tag ();
    ଅଚଳ Triple *range;
    ଦଶମିକ minimum, ମଧ୍ୟ, maximum;
    axis.get_coordinates (minimum, ମଧ୍ୟ, maximum);
    ଯଦି (plan->user_axes_location.has (tag, &range) && !range->is_point () &&
        range->minimum == minimum &&
        range->ମଧ୍ୟ == ମଧ୍ୟ &&
        range->maximum == maximum)
      plan->user_axes_location.del (tag);
  }
  ଯଦି (plan->user_axes_location.is_empty ())
    ଫେରନ୍ତୁ ସତ୍ୟ;

  ଯଦି (!plan->check_success (plan->normalized_coords.resize (axes.length)))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  ତାର୍କିକ has_avar = face->table.avar->has_data ();
  hb_vector_t<ଦଶମିକ> normalized_mins;
  hb_vector_t<ଦଶମିକ> normalized_defaults;
  hb_vector_t<ଦଶମିକ> normalized_maxs;
  ଯଦି (has_avar)
  {
    ଯଦି (!plan->check_success (normalized_mins.resize (axes.length)) ||
        !plan->check_success (normalized_defaults.resize (axes.length)) ||
        !plan->check_success (normalized_maxs.resize (axes.length)))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ତାର୍କିକ axis_not_pinned = ମିଥ୍ୟା;
  unsigned new_axis_idx = 0;
  unsigned last_idx = 0;
  ପାଇଁ (ଅଚଳ auto& _ : + hb_enumerate (axes))
  {
    unsigned i = _.first;
    ଅଚଳ OT::AxisRecord &axis = _.second;
    hb_tag_t axis_tag = axis.get_axis_tag ();
    plan->axes_old_index_tag_map.set (i, axis_tag);

    ଯଦି (!plan->user_axes_location.has (axis_tag) ||
        !plan->user_axes_location.get (axis_tag).is_point ())
    {
      axis_not_pinned = ସତ୍ୟ;
      plan->axes_index_map.set (i, new_axis_idx);
      plan->axis_tags.push (axis_tag);
      new_axis_idx++;
    }

    Triple *axis_range;
    ଯଦି (plan->user_axes_location.has (axis_tag, &axis_range))
    {
      plan->axes_triple_distances.set (axis_tag, axis.get_triple_distances ());

      ଦଶମିକ normalized_min = axis.normalize_axis_ମୂଲ୍ୟ (axis_range->minimum);
      ଦଶମିକ normalized_default = axis.normalize_axis_ମୂଲ୍ୟ (axis_range->ମଧ୍ୟ);
      ଦଶମିକ normalized_max = axis.normalize_axis_ମୂଲ୍ୟ (axis_range->maximum);

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

      ଯଦି (has_avar)
      {
        normalized_mins[i] = normalized_min;
        normalized_defaults[i] = normalized_default;
        normalized_maxs[i] = normalized_max;
        last_idx = i;
      }
      ଅନ୍ୟଥା
      {
        plan->axes_location.set (axis_tag, Triple ((ଦ୍ୱିଗୁଣ) normalized_min,
                                                   (ଦ୍ୱିଗୁଣ) normalized_default,
                                                   (ଦ୍ୱିଗୁଣ) normalized_max));
        ଯଦି (normalized_default == -0.f)
          normalized_default = 0.f; // Normalize -0 to 0
        ଯଦି (normalized_default != 0.f)
          plan->pinned_at_default = ମିଥ୍ୟା;

        plan->normalized_coords[i] = roundf (normalized_default * 16384.f);
      }
    }
  }
  plan->all_axes_pinned = !axis_not_pinned;

  // TODO: use avar map_coords_16_16() when normalization is changed to 16.16
  // in fonttools
  ଯଦି (has_avar)
  {
    ଅଚଳ OT::avar* avar_table = face->table.avar;
    unsigned coords_len = last_idx + 1;

    ଯଦି (avar_table->has_v2_data () && !plan->all_axes_pinned)
    {
      /* avar2 partial instancing: use v1-only mapping to get intermediate-space
       * coords. These are used for IVS rebasing and offset compensation. */
      plan->has_avar2 = ସତ୍ୟ;

      ଯଦି (!plan->check_success (avar_table->map_coords_2_14 (normalized_mins.arrayZ, coords_len, ସତ୍ୟ)) ||
          !plan->check_success (avar_table->map_coords_2_14 (normalized_defaults.arrayZ, coords_len, ସତ୍ୟ)) ||
          !plan->check_success (avar_table->map_coords_2_14 (normalized_maxs.arrayZ, coords_len, ସତ୍ୟ)))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

      ପାଇଁ (ଅଚଳ auto& _ : + hb_enumerate (axes))
      {
        unsigned i = _.first;
        hb_tag_t axis_tag = _.second.get_axis_tag ();
        ଯଦି (plan->user_axes_location.has (axis_tag))
        {
          /* Store intermediate-space coords for offset compensation,
           * segment-map renormalization, and IVS rebasing. */
          plan->old_intermediates.set (axis_tag, Triple ((ଦ୍ୱିଗୁଣ) normalized_mins[i],
                                                         (ଦ୍ୱିଗୁଣ) normalized_defaults[i],
                                                         (ଦ୍ୱିଗୁଣ) normalized_maxs[i]));
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
      ତାର୍କିକ detect_self_contained = !hb_blob_get_length (cff2_blob);
      hb_blob_destroy (cff2_blob);
      ଯଦି (!_compute_avar2_reachable_ranges (plan, axes, avar_table,
                                            detect_self_contained))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

#ifndef HB_NO_VAR_COMPOSITES
      /* VARC overrides can reach coordinates outside the font-level
       * ranges. Do not cull variation data using those ranges. */
      ଯଦି (!plan->drop_tables.has (HB_TAG ('V','A','R','C')))
      {
        hb_blob_t *varc_blob = hb_face_reference_table (face, HB_TAG ('V','A','R','C'));
        ଯଦି (hb_blob_get_length (varc_blob))
          plan->avar2_reachable_ranges.reset ();
        hb_blob_destroy (varc_blob);
      }
#endif

      /* Keep all axes in fvar (pinned ones as hidden), EXCEPT self-contained
       * pinned axes, which are removed entirely. */
      plan->axes_index_map.reset ();
      plan->axis_tags.reset ();
      unsigned retained_axis_idx = 0;
      ପାଇଁ (ଅଚଳ auto& _ : + hb_enumerate (axes))
      {
        unsigned i = _.first;
        hb_tag_t axis_tag = _.second.get_axis_tag ();
        ଯଦି (plan->avar2_self_contained.has (axis_tag))
          ଜାରି;
        plan->axes_index_map.set (i, retained_axis_idx++);
        plan->axis_tags.push (axis_tag);
      }
      /* The other tables see ONLY the self-contained pins, as an ordinary
       * partial instancing in old final-coordinate space; the standard
       * instancing machinery bakes those axes' contributions in. The
       * remaining restriction is carried entirely by avar. */
      plan->axes_location.reset ();
      ଯଦି (plan->avar2_self_contained.get_population ())
      {
        ତାର୍କିକ sc_pinned_at_default = ସତ୍ୟ;
        ପାଇଁ (auto _ : plan->avar2_self_contained)
        {
          plan->axes_location.set (_.first, Triple (_.second, _.second, _.second));
          ଯଦି (_.second != 0.0)
            sc_pinned_at_default = ମିଥ୍ୟା;
        }
        ପାଇଁ (ଅଚଳ auto& _ : + hb_enumerate (axes))
        {
          ଦ୍ୱିଗୁଣ *v;
          plan->normalized_coords[_.first] =
              plan->avar2_self_contained.has (_.second.get_axis_tag (), &v)
              ? (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) roundf ((ଦଶମିକ) (*v * 16384.0)) : 0;
        }
        plan->pinned_at_default = sc_pinned_at_default;
      }
      ଅନ୍ୟଥା
        plan->normalized_coords.shrink (0);
    }
    ଅନ୍ୟଥା
    {
      /* Standard avar v1 (or v1+v2 with all axes pinned) */
      ଯଦି (!plan->check_success (avar_table->map_coords_2_14 (normalized_mins.arrayZ, coords_len)) ||
          !plan->check_success (avar_table->map_coords_2_14 (normalized_defaults.arrayZ, coords_len)) ||
          !plan->check_success (avar_table->map_coords_2_14 (normalized_maxs.arrayZ, coords_len)))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

      ପାଇଁ (ଅଚଳ auto& _ : + hb_enumerate (axes))
      {
        unsigned i = _.first;
        hb_tag_t axis_tag = _.second.get_axis_tag ();
        ଯଦି (plan->user_axes_location.has (axis_tag))
        {
          plan->axes_location.set (axis_tag, Triple ((ଦ୍ୱିଗୁଣ) normalized_mins[i],
                                                     (ଦ୍ୱିଗୁଣ) normalized_defaults[i],
                                                     (ଦ୍ୱିଗୁଣ) normalized_maxs[i]));
          ଦଶମିକ normalized_default = normalized_defaults[i];
          ଯଦି (normalized_default == -0.f)
            normalized_default = 0.f;
          ଯଦି (normalized_default != 0.f)
            plan->pinned_at_default = ମିଥ୍ୟା;

          plan->normalized_coords[i] = roundf (normalized_default * 16384.f);
        }
      }
    }
  }
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

#ifndef HB_NO_OT_FONT_CFF
ଶୂନ୍ୟ
update_instance_metrics_map_from_cff2 (hb_subset_plan_t *plan)
{
  ଯଦି (!plan->normalized_coords) ଫେରନ୍ତୁ;
  OT::cff2::accelerator_t cff2 (plan->source);
  ଯଦି (!cff2.is_valid ()) ଫେରନ୍ତୁ;

  hb_font_t *font = _get_hb_font_with_variations (plan);
  ଯଦି (unlikely (!plan->check_success (font != nullptr)))
  {
    hb_font_destroy (font);
    ଫେରନ୍ତୁ;
  }

  hb_glyph_extents_t extents = {0x7FFF, -0x7FFF};
  OT::hmtx_accelerator_t _hmtx (plan->source);
  OT::hb_scalar_cache_t *hvar_store_cache = nullptr;
  ଯଦି (_hmtx.has_data () && _hmtx.var_table.get_length ())
    hvar_store_cache = _hmtx.var_table->get_var_store ().create_cache ();

  OT::vmtx_accelerator_t _vmtx (plan->source);
  OT::hb_scalar_cache_t *vvar_store_cache = nullptr;
  ଯଦି (_vmtx.has_data () && _vmtx.var_table.get_length ())
    vvar_store_cache = _vmtx.var_table->get_var_store ().create_cache ();

  ପାଇଁ (auto p : *plan->glyph_map)
  {
    hb_codepoint_t old_gid = p.first;
    hb_codepoint_t new_gid = p.second;
    ଯଦି (!cff2.get_extents (font, old_gid, &extents)) ଜାରି;
    ତାର୍କିକ has_bounds_info = ସତ୍ୟ;
    ଯଦି (extents.x_bearing == 0 && extents.width == 0 &&
        extents.height == 0 && extents.y_bearing == 0)
      has_bounds_info = ମିଥ୍ୟା;

    ଯଦି (has_bounds_info)
    {
      plan->head_maxp_info.xMin = hb_min (plan->head_maxp_info.xMin, extents.x_bearing);
      plan->head_maxp_info.xMax = hb_max (plan->head_maxp_info.xMax,
						 hb_saturate_add (extents.x_bearing, extents.width));
      plan->head_maxp_info.yMax = hb_max (plan->head_maxp_info.yMax, extents.y_bearing);
      plan->head_maxp_info.yMin = hb_min (plan->head_maxp_info.yMin,
						 hb_saturate_add (extents.y_bearing, extents.height));
    }

    ଯଦି (_hmtx.has_data ())
    {
      ଦ୍ୱିଗୁଣ hori_aw = _hmtx.get_advance_without_var_unscaled (old_gid);
      ଯଦି (_hmtx.var_table.get_length ())
        hori_aw += (ଦ୍ୱିଗୁଣ) roundf (_hmtx.var_table->get_advance_delta_unscaled (old_gid, font->coords, font->ଅଙ୍କ_coords,
									 hvar_store_cache));
      /* Malicious deltas can take the advance out of the UFWORD range that
       * hmtx serialization stores.  Clamp, to keep downstream consumers
       * (e.g. CFF width optimizer) bounded and consistent with hmtx. */
      hori_aw = hb_clamp (hori_aw, 0., 65535.);
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା lsb = extents.x_bearing;
      ଯଦି (!has_bounds_info)
      {
        _hmtx.get_leading_bearing_without_var_unscaled (old_gid, &lsb);
      }
      plan->hmtx_map.set (new_gid, hb_pair ((unsigned) hori_aw, lsb));
      plan->bounds_width_vec[new_gid] = extents.width;
    }

    ଯଦି (_vmtx.has_data ())
    {
      ଦ୍ୱିଗୁଣ vert_aw = _vmtx.get_advance_without_var_unscaled (old_gid);
      ଯଦି (_vmtx.var_table.get_length ())
        vert_aw += (ଦ୍ୱିଗୁଣ) roundf (_vmtx.var_table->get_advance_delta_unscaled (old_gid, font->coords, font->ଅଙ୍କ_coords,
									 vvar_store_cache));
      vert_aw = hb_clamp (vert_aw, 0., 65535.);
      hb_position_t vorg_x = 0;
      hb_position_t vorg_y = 0;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା tsb = 0;
      ଯଦି (has_bounds_info &&
           hb_font_get_glyph_v_origin (font, old_gid, &vorg_x, &vorg_y))
      {
        tsb = hb_saturate_sub (vorg_y, extents.y_bearing);
      } ଅନ୍ୟଥା {
        _vmtx.get_leading_bearing_without_var_unscaled (old_gid, &tsb);
      }

      plan->vmtx_map.set (new_gid, hb_pair ((unsigned) vert_aw, tsb));
      plan->bounds_height_vec[new_gid] = extents.height;
    }
  }
  /* No glyph contributed bounds: do not serialize the accumulator's
   * inverted sentinels into head (or a downgraded CFF FontBBox). */
  ଯଦି (plan->head_maxp_info.xMin > plan->head_maxp_info.xMax)
  {
    plan->head_maxp_info.xMin = plan->head_maxp_info.xMax = 0;
    plan->head_maxp_info.yMin = plan->head_maxp_info.yMax = 0;
  }
  hb_font_destroy (font);
  ଯଦି (hvar_store_cache)
    _hmtx.var_table->get_var_store ().destroy_cache (hvar_store_cache);
  ଯଦି (vvar_store_cache)
    _vmtx.var_table->get_var_store ().destroy_cache (vvar_store_cache);
}
#endif

ତାର୍କିକ
get_instance_glyphs_contour_points (hb_subset_plan_t *plan)
{
  /* contour_points vector only needed for updating gvar table (infer delta and
   * iup delta optimization) during partial instancing */
  ଯଦି (plan->user_axes_location.is_empty () || plan->all_axes_pinned)
    ଫେରନ୍ତୁ ସତ୍ୟ;

  OT::glyf_accelerator_t glyf (plan->source);

  ପାଇଁ (auto &_ : plan->new_to_old_gid_list)
  {
    hb_codepoint_t new_gid = _.first;
    contour_point_vector_t all_points;
    ଯଦି (new_gid == 0 && !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      ଯଦି (unlikely (!plan->new_gid_contour_points_map.set (new_gid, all_points)))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ଜାରି;
    }

    hb_codepoint_t old_gid = _.second;
    auto glyph = glyf.glyph_for_gid (old_gid);
    ଯଦି (unlikely (!glyph.get_all_points_without_var (plan->source, all_points)))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଯଦି (unlikely (!plan->new_gid_contour_points_map.set (new_gid, all_points)))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    /* composite new gids are only needed by iup delta optimization */
    ଯଦି ((plan->flags & HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS) && glyph.is_composite ())
      plan->composite_new_gids.add (new_gid);
  }
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

template<typename DeltaSetIndexMap>
ଶୂନ୍ୟ
remap_colrv1_delta_set_index_indices (ଅଚଳ DeltaSetIndexMap &index_map,
                                      ଅଚଳ hb_set_t &delta_set_idxes,
                                      hb_hashmap_t<unsigned, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /* OUT */)
{
  ଯଦି (!index_map.get_map_ଗଣନା ())
    ଫେରନ୍ତୁ;

  hb_hashmap_t<unsigned, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> delta_set_idx_delta_map;
  unsigned new_delta_set_idx = 0;
  ପାଇଁ (unsigned delta_set_idx : delta_set_idxes)
  {
    unsigned var_idx = index_map.map (delta_set_idx);
    unsigned new_varidx = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା delta = 0;

    ଯଦି (var_idx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
    {
      hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> *new_varidx_delta;
      ଯଦି (!variation_idx_delta_map.has (var_idx, &new_varidx_delta)) ଜାରି;

      new_varidx = hb_first (*new_varidx_delta);
      delta = hb_second (*new_varidx_delta);
    }

    new_deltaset_idx_varidx_map.set (new_delta_set_idx, new_varidx);
    delta_set_idx_delta_map.set (delta_set_idx, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> (new_delta_set_idx, delta));
    new_delta_set_idx++;
  }
  variation_idx_delta_map = std::move (delta_set_idx_delta_map);
}

template ଶୂନ୍ୟ
remap_colrv1_delta_set_index_indices<OT::DeltaSetIndexMap> (ଅଚଳ OT::DeltaSetIndexMap &index_map,
                                      ଅଚଳ hb_set_t &delta_set_idxes,
                                      hb_hashmap_t<unsigned, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /* OUT */);

 #endif

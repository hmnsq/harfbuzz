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

 #અન્તર્ભાવ "hb-ot-layout-common.hh"
#અન્તર્ભાવ "hb-subset-plan.hh"

 #અન્તર્ભાવ "hb-ot-var-common.hh"
 #અન્તર્ભાવ "hb-ot-layout-base-table.hh"
 #અન્તર્ભાવ "hb-ot-glyf-table.hh"
 #અન્તર્ભાવ "hb-ot-var-fvar-table.hh"
 #અન્તર્ભાવ "hb-ot-var-avar-table.hh"
 #અન્તર્ભાવ "hb-ot-cff2-table.hh"

 #ifndef HB_NO_VAR

 શૂન્ય
 generate_varstore_inner_maps (અચલ hb_set_t& varidx_set,
                               unsigned subtable_ગણતરી,
                               hb_vector_t<hb_inc_bimap_t> &inner_maps /* OUT */)
 {
   જો (varidx_set.is_empty () || subtable_ગણતરી == 0) પાછા;

   જો (unlikely (!inner_maps.resize (subtable_ગણતરી))) પાછા;
   માટે (unsigned idx : varidx_set)
   {
     uint16_t major = idx >> 16;
     uint16_t minor = idx & 0xFFFF;

     જો (major >= subtable_ગણતરી)
       ચાલુ;
     inner_maps[major].add (minor);
   }
 }

 /* Evaluate a variation-region tent at v, mirroring VarRegionAxis::evaluate
  * (invalid regions evaluate as constant 1). */
 સ્થિર દ્વિગુણ
 _tent_eval (અચલ Triple &tent, દ્વિગુણ v)
 {
   દ્વિગુણ start = (દ્વિગુણ) tent.minimum;
   દ્વિગુણ peak = (દ્વિગુણ) tent.મધ્ય;
   દ્વિગુણ end = (દ્વિગુણ) tent.maximum;
   જો (unlikely (start > peak || peak > end)) પાછા 1.0;
   જો (unlikely (start < 0.0 && end > 0.0 && peak != 0.0)) પાછા 1.0;
   જો (peak == 0.0 || v == peak) પાછા 1.0;
   જો (v <= start || end <= v) પાછા 0.0;
   જો (v < peak) પાછા (v - start) / (peak - start);
   પાછા (end - v) / (end - peak);
 }

 /* Range of a tent's scalar over an input interval [lo, hi]. The tent is
  * unimodal, so the extremes are at the interval endpoints, plus the peak
  * when it falls inside. */
 સ્થિર શૂન્ય
 tent_કિંમત_range (અચલ Triple &tent, દ્વિગુણ lo, દ્વિગુણ hi,
                    દ્વિગુણ *tmin, દ્વિગુણ *tmax)
 {
   દ્વિગુણ a = _tent_eval (tent, lo);
   દ્વિગુણ b = _tent_eval (tent, hi);
   *tmin = hb_min (a, b);
   *tmax = hb_max (a, b);
   જો (lo <= (દ્વિગુણ) tent.મધ્ય && (દ્વિગુણ) tent.મધ્ય <= hi)
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
 સ્થિર તાર્કિક
 _compute_avar2_reachable_ranges (hb_subset_plan_t *plan,
                                  hb_array_t<અચલ OT::AxisRecord> axes,
                                  અચલ OT::avar *avar_table,
                                  તાર્કિક detect_self_contained)
 {
   અચલ OT::ItemVariationStore *var_store;
   અચલ OT::DeltaSetIndexMap *varidx_map;
   જો (!avar_table->get_v2_store_and_map (&var_store, &varidx_map))
     પાછા ખોટું;

   /* Old-intermediate input box per axis. */
   hb_hashmap_t<hb_tag_t, hb_pair_t<દ્વિગુણ, દ્વિગુણ>> box;
   માટે (અચલ auto &axis : axes)
   {
     hb_tag_t tag = axis.get_axis_tag ();
     દ્વિગુણ lo = -1.0, hi = +1.0;
     Triple *old_int;
     જો (plan->user_axes_location.has (tag))
     {
       જો (!plan->old_intermediates.has (tag, &old_int)) પાછા ખોટું;
       lo = (દ્વિગુણ) old_int->minimum;
       hi = (દ્વિગુણ) old_int->maximum;
     }
     નહીંતર જો (axis.is_hidden ())
       lo = hi = 0.0; /* private axis: intermediate is always 0 */
     જો (!box.set (tag, hb_pair (lo, hi))) પાછા ખોટું;
   }

   hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> regions;
   જો (!var_store->get_region_list ().get_var_regions (plan->axes_old_index_tag_map, regions))
     પાછા ખોટું;

   hb_hashmap_t<hb_tag_t, unsigned> grid_pos;
   hb_vector_t<hb_vector_t<દ્વિગુણ>> grid_points;
   માટે (unsigned i = 0; i < axes.length; i++)
   {
     grid_pos.reset ();
     grid_points.reset ();

     hb_tag_t tag = axes[i].get_axis_tag ();
     hb_pair_t<દ્વિગુણ, દ્વિગુણ> *identity;
     જો (!box.has (tag, &identity)) પાછા ખોટું;

     /* Collect the row's active regions (non-zero delta). A varidx (or the
      * implicit identity mapping) that doesn't resolve to a real store row
      * behaves as "no variation" at runtime, so it contributes no regions;
      * this also covers a NULL VarStore. */
     hb_vector_t<hb_pair_t<પૂર્ણાંક, unsigned>> active; /* (delta, region index) */
     uint32_t varidx = varidx_map->map (i);
     /* Runtime treats out-of-range delta-set indices as zero. */
     જો (varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX &&
	 !var_store->has_delta_set (varidx))
       varidx = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
     જો (varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
     {
       unsigned outer = varidx >> 16;
       unsigned inner = varidx & 0xFFFF;
       અચલ OT::VarData &vd = var_store->get_sub_table (outer);
       અચલ OT::HBUINT8 *delta_bytes = vd.get_delta_bytes ();
       unsigned row_size = vd.get_row_size ();
       unsigned region_ગણતરી_2 = vd.get_region_index_ગણતરી ();
       માટે (unsigned r = 0; r < region_ગણતરી_2; r++)
       {
	 પૂર્ણાંક delta = vd.get_item_delta_fast (inner, r, delta_bytes, row_size);
	 જો (!delta) ચાલુ;
	 unsigned region_idx = vd.get_region_index (r);
	 જો (region_idx >= regions.length) પાછા ખોટું;
	 active.push (hb_pair (delta, region_idx));
       }
       જો (active.in_error ()) પાછા ખોટું;
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
     તાર્કિક exact = સાચું;
     દ્વિગુણ dmin = 0.0, dmax = 0.0; /* delta extremes, F2Dot14 units */
     દ્વિગુણ vmin = 0.0, vmax = 0.0; /* identity + delta extremes, coord units */

     /* Grid axes: the target axis (identity term), plus every axis a valid
      * tent of an active region references. */
     auto add_grid_axis = [&] (hb_tag_t t) -> પૂર્ણાંક
     {
       unsigned *pos;
       જો (grid_pos.has (t, &pos)) પાછા (પૂર્ણાંક) *pos;
       unsigned new_pos = grid_pos.get_population ();
       hb_pair_t<દ્વિગુણ, દ્વિગુણ> *આગત;
       દ્વિગુણ blo = -1.0, bhi = +1.0;
       જો (box.has (t, &આગત)) { blo = આગત->first; bhi = આગત->second; }
       grid_points.push (hb_vector_t<દ્વિગુણ> ());
       જો (grid_points.in_error ()) પાછા -1;
       auto &points = grid_points[new_pos];
       points.push (blo);
       points.push (bhi);
       જો (points.in_error () || !grid_pos.set (t, new_pos)) પાછા -1;
       પાછા (પૂર્ણાંક) new_pos;
     };

     જો (add_grid_axis (tag) < 0) પાછા ખોટું;
     /* Per active region: (grid axis position, tent) for its valid tents. */
     hb_vector_t<hb_vector_t<hb_pair_t<unsigned, Triple>>> region_tents;
     માટે (અચલ auto &ar : active)
     {
       region_tents.push (hb_vector_t<hb_pair_t<unsigned, Triple>> ());
       જો (region_tents.in_error ()) પાછા ખોટું;
       auto &tents = region_tents[region_tents.length - 1];
       માટે (અચલ auto &_ : regions[ar.second])
       {
	 અચલ Triple &tent = _.second;
	 /* Tents the runtime ignores (constant scalar 1) add no breakpoints. */
	 જો (tent.મધ્ય == 0.f ||
	     tent.minimum > tent.મધ્ય || tent.મધ્ય > tent.maximum ||
	     (tent.minimum < 0.f && tent.maximum > 0.f))
	   ચાલુ;
	 પૂર્ણાંક pos = add_grid_axis (_.first);
	 જો (pos < 0) પાછા ખોટું;
	 auto &points = grid_points[pos];
	 દ્વિગુણ blo = points[0], bhi = points[1];
	 points.push (hb_clamp ((દ્વિગુણ) tent.minimum, blo, bhi));
	 points.push (hb_clamp ((દ્વિગુણ) tent.મધ્ય, blo, bhi));
	 points.push (hb_clamp ((દ્વિગુણ) tent.maximum, blo, bhi));
	 tents.push (hb_pair ((unsigned) pos, tent));
	 જો (points.in_error () || tents.in_error ()) પાછા ખોટું;
       }
     }

     unsigned grid_size = 1;
     માટે (auto &points : grid_points)
     {
       points.qsort ([] (અચલ દ્વિગુણ &a, અચલ દ્વિગુણ &b) -> પૂર્ણાંક
		     { પાછા a < b ? -1 : a > b ? +1 : 0; });
       unsigned n = 0;
       માટે (unsigned j = 0; j < points.length; j++)
	 જો (!n || points.arrayZ[j] != points.arrayZ[n - 1])
	   points.arrayZ[n++] = points.arrayZ[j];
       points.shrink (n);
       જો (grid_size > MAX_GRID / points.length) { exact = ખોટું; તોડો; }
       grid_size *= points.length;
     }

     જો (exact)
     {
       unsigned target_pos = 0; /* the target axis was added first */
       hb_vector_t<unsigned> odometer;
       જો (!odometer.resize (grid_pos.get_population ())) પાછા ખોટું;
       તાર્કિક first = સાચું;
       જ્યારે (સાચું)
       {
	 દ્વિગુણ delta_sum = 0.0;
	 માટે (unsigned r = 0; r < active.length; r++)
	 {
	   દ્વિગુણ scalar = 1.0;
	   માટે (અચલ auto &pt : region_tents[r])
	   {
	     દ્વિગુણ v = grid_points[pt.first][odometer[pt.first]];
	     scalar *= _tent_eval (pt.second, v);
	     જો (scalar == 0.0) તોડો;
	   }
	   delta_sum += scalar * active[r].first;
	 }
	 દ્વિગુણ coord = grid_points[target_pos][odometer[target_pos]];
	 દ્વિગુણ value = coord + delta_sum / 16384.0;
	 જો (first)
	 {
	   dmin = dmax = delta_sum;
	   vmin = vmax = value;
	   first = ખોટું;
	 }
	 નહીંતર
	 {
	   dmin = hb_min (dmin, delta_sum); dmax = hb_max (dmax, delta_sum);
	   vmin = hb_min (vmin, value);     vmax = hb_max (vmax, value);
	 }
	 unsigned k = 0;
	 માટે (; k < odometer.length; k++)
	 {
	   જો (++odometer[k] < grid_points[k].length) તોડો;
	   odometer[k] = 0;
	 }
	 જો (k == odometer.length) તોડો;
       }
     }
     નહીંતર
     {
       માટે (અચલ auto &ar : active)
       {
	 પૂર્ણાંક delta = ar.first;
	 દ્વિગુણ smin = 1.0, smax = 1.0;
	 માટે (અચલ auto &_ : regions[ar.second])
	 {
	   hb_pair_t<દ્વિગુણ, દ્વિગુણ> *આગત;
	   દ્વિગુણ blo = -1.0, bhi = +1.0;
	   જો (box.has (_.first, &આગત)) { blo = આગત->first; bhi = આગત->second; }
	   દ્વિગુણ tmin, tmax;
	   tent_કિંમત_range (_.second, blo, bhi, &tmin, &tmax);
	   smin *= tmin;
	   smax *= tmax;
	   જો (smax == 0.0) તોડો;
	 }
	 જો (delta > 0) { dmin += smin * delta; dmax += smax * delta; }
	 નહીંતર           { dmin += smax * delta; dmax += smin * delta; }
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
     જો (detect_self_contained && dmin == dmax &&
	 plan->user_axes_location.has (tag, &user) && user->is_point ())
     {
       પૂર્ણાંક v_int = (પૂર્ણાંક) roundf ((દશાંશ) identity->first * 16384.f) +
		   (પૂર્ણાંક) roundf ((દશાંશ) dmin);
       v_int = hb_clamp (v_int, -(1 << 14), +(1 << 14));
       જો (!plan->avar2_self_contained.set (tag, v_int / 16384.0)) પાછા ખોટું;
       ચાલુ; /* axis is dropped; no region can reference it afterwards */
     }

     /* Pad by one F2Dot14 unit against rounding differences with the
      * runtime's fixed-point evaluation. */
     દ્વિગુણ lo = hb_clamp (vmin - 1.0 / 16384.0, -1.0, +1.0);
     દ્વિગુણ hi = hb_clamp (vmax + 1.0 / 16384.0, -1.0, +1.0);
     lo = floor (lo * 16384.0) / 16384.0;
     hi = ceil (hi * 16384.0) / 16384.0;
     જો (lo <= -1.0 && hi >= +1.0)
       ચાલુ; /* not constraining */
     જો (!plan->avar2_reachable_ranges.set (tag, Triple (lo, hb_clamp (0.0, lo, hi), hi)))
       પાછા ખોટું;
   }
   પાછા સાચું;
 }

#ifndef HB_NO_OT_FONT_CFF
 સ્થિર અંતઃરેખ hb_font_t*
 _get_hb_font_with_variations (અચલ hb_subset_plan_t *plan)
 {
   hb_font_t *font = hb_font_create (plan->source);

   જો (plan->has_avar2)
   {
     /* Under avar2, instancing applies only to the self-contained pins,
      * whose constant final coordinates the plan holds in normalized_coords.
      * Setting user-space variations would run the full avar2 mapping and
      * bake contributions that remain live in the instance's variations. */
     hb_font_set_var_coords_normalized (font, plan->normalized_coords.arrayZ,
                                        plan->normalized_coords.length);
     પાછા font;
   }

   hb_vector_t<hb_variation_t> vars;
   જો (!vars.alloc (plan->user_axes_location.get_population ())) {
     hb_font_destroy (font);
     પાછા nullptr;
   }

   માટે (auto _ : plan->user_axes_location)
   {
     hb_variation_t var;
     var.tag = _.first;
     var.value = _.second.મધ્ય;
     vars.push (var);
   }

   hb_font_set_variations (font, vars.arrayZ, plan->user_axes_location.get_population ());
   પાછા font;
 }
#endif

 template<typename ItemVarStore>
 શૂન્ય
 remap_variation_indices (અચલ ItemVarStore &var_store,
                          અચલ hb_set_t &variation_indices,
                          અચલ hb_vector_t<પૂર્ણાંક>& normalized_coords,
                          તાર્કિક calculate_delta, /* not pinned at default */
                          તાર્કિક no_variations, /* all axes pinned */
                          hb_hashmap_t<unsigned, hb_pair_t<unsigned, પૂર્ણાંક>> &variation_idx_delta_map /* OUT */)
 {
   જો (&var_store == &Null (OT::ItemVariationStore)) પાછા;
   unsigned subtable_ગણતરી = var_store.get_sub_table_ગણતરી ();
   auto *store_cache = var_store.create_cache ();

   unsigned new_major = 0, new_minor = 0;
   unsigned last_major = (variation_indices.get_min ()) >> 16;
   માટે (unsigned idx : variation_indices)
   {
     પૂર્ણાંક delta = 0;
     જો (calculate_delta)
       delta = roundf (var_store.get_delta (idx, normalized_coords.arrayZ,
                                            normalized_coords.length, store_cache));

     જો (no_variations)
     {
       variation_idx_delta_map.set (idx, hb_pair_t<unsigned, પૂર્ણાંક> (HB_OT_LAYOUT_NO_VARIATIONS_INDEX, delta));
       ચાલુ;
     }

     uint16_t major = idx >> 16;
     જો (major >= subtable_ગણતરી) તોડો;
     જો (major != last_major)
     {
       new_minor = 0;
       ++new_major;
     }

     unsigned new_idx = (new_major << 16) + new_minor;
     variation_idx_delta_map.set (idx, hb_pair_t<unsigned, પૂર્ણાંક> (new_idx, delta));
     ++new_minor;
     last_major = major;
   }
   var_store.destroy_cache (store_cache);
 }

 template
 શૂન્ય
 remap_variation_indices<OT::ItemVariationStore> (અચલ OT::ItemVariationStore &var_store,
                          અચલ hb_set_t &variation_indices,
                          અચલ hb_vector_t<પૂર્ણાંક>& normalized_coords,
                          તાર્કિક calculate_delta, /* not pinned at default */
                          તાર્કિક no_variations, /* all axes pinned */
                          hb_hashmap_t<unsigned, hb_pair_t<unsigned, પૂર્ણાંક>> &variation_idx_delta_map /* OUT */);

 #ifndef HB_NO_BASE
 શૂન્ય
 collect_base_variation_indices (hb_subset_plan_t* plan)
 {
   hb_blob_ptr_t<OT::BASE> base = plan->source_table<OT::BASE> ();
   જો (!base->has_var_store ())
   {
     base.destroy ();
     પાછા;
   }

   hb_set_t varidx_set;
   base->collect_variation_indices (plan, varidx_set);
   અચલ OT::ItemVariationStore &var_store = base->get_var_store ();
   unsigned subtable_ગણતરી = var_store.get_sub_table_ગણતરી ();


   remap_variation_indices (var_store, varidx_set,
                             plan->normalized_coords,
                             !plan->pinned_at_default,
                             plan->all_axes_pinned,
                             plan->base_variation_idx_map);
   generate_varstore_inner_maps (varidx_set, subtable_ગણતરી, plan->base_varstore_inner_maps);

   base.destroy ();
 }

 #endif

તાર્કિક
normalize_axes_location (hb_face_t *face, hb_subset_plan_t *plan)
{
  જો (plan->user_axes_location.is_empty ())
    પાછા સાચું;

  hb_array_t<અચલ OT::AxisRecord> axes = face->table.fvar->get_axes ();
  માટે (અચલ auto &axis : axes)
  {
    hb_tag_t tag = axis.get_axis_tag ();
    અચલ Triple *range;
    દશાંશ minimum, મધ્ય, maximum;
    axis.get_coordinates (minimum, મધ્ય, maximum);
    જો (plan->user_axes_location.has (tag, &range) && !range->is_point () &&
        range->minimum == minimum &&
        range->મધ્ય == મધ્ય &&
        range->maximum == maximum)
      plan->user_axes_location.del (tag);
  }
  જો (plan->user_axes_location.is_empty ())
    પાછા સાચું;

  જો (!plan->check_success (plan->normalized_coords.resize (axes.length)))
    પાછા ખોટું;

  તાર્કિક has_avar = face->table.avar->has_data ();
  hb_vector_t<દશાંશ> normalized_mins;
  hb_vector_t<દશાંશ> normalized_defaults;
  hb_vector_t<દશાંશ> normalized_maxs;
  જો (has_avar)
  {
    જો (!plan->check_success (normalized_mins.resize (axes.length)) ||
        !plan->check_success (normalized_defaults.resize (axes.length)) ||
        !plan->check_success (normalized_maxs.resize (axes.length)))
      પાછા ખોટું;
  }

  તાર્કિક axis_not_pinned = ખોટું;
  unsigned new_axis_idx = 0;
  unsigned last_idx = 0;
  માટે (અચલ auto& _ : + hb_enumerate (axes))
  {
    unsigned i = _.first;
    અચલ OT::AxisRecord &axis = _.second;
    hb_tag_t axis_tag = axis.get_axis_tag ();
    plan->axes_old_index_tag_map.set (i, axis_tag);

    જો (!plan->user_axes_location.has (axis_tag) ||
        !plan->user_axes_location.get (axis_tag).is_point ())
    {
      axis_not_pinned = સાચું;
      plan->axes_index_map.set (i, new_axis_idx);
      plan->axis_tags.push (axis_tag);
      new_axis_idx++;
    }

    Triple *axis_range;
    જો (plan->user_axes_location.has (axis_tag, &axis_range))
    {
      plan->axes_triple_distances.set (axis_tag, axis.get_triple_distances ());

      દશાંશ normalized_min = axis.normalize_axis_કિંમત (axis_range->minimum);
      દશાંશ normalized_default = axis.normalize_axis_કિંમત (axis_range->મધ્ય);
      દશાંશ normalized_max = axis.normalize_axis_કિંમત (axis_range->maximum);

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

      જો (has_avar)
      {
        normalized_mins[i] = normalized_min;
        normalized_defaults[i] = normalized_default;
        normalized_maxs[i] = normalized_max;
        last_idx = i;
      }
      નહીંતર
      {
        plan->axes_location.set (axis_tag, Triple ((દ્વિગુણ) normalized_min,
                                                   (દ્વિગુણ) normalized_default,
                                                   (દ્વિગુણ) normalized_max));
        જો (normalized_default == -0.f)
          normalized_default = 0.f; // Normalize -0 to 0
        જો (normalized_default != 0.f)
          plan->pinned_at_default = ખોટું;

        plan->normalized_coords[i] = roundf (normalized_default * 16384.f);
      }
    }
  }
  plan->all_axes_pinned = !axis_not_pinned;

  // TODO: use avar map_coords_16_16() when normalization is changed to 16.16
  // in fonttools
  જો (has_avar)
  {
    અચલ OT::avar* avar_table = face->table.avar;
    unsigned coords_len = last_idx + 1;

    જો (avar_table->has_v2_data () && !plan->all_axes_pinned)
    {
      /* avar2 partial instancing: use v1-only mapping to get intermediate-space
       * coords. These are used for IVS rebasing and offset compensation. */
      plan->has_avar2 = સાચું;

      જો (!plan->check_success (avar_table->map_coords_2_14 (normalized_mins.arrayZ, coords_len, સાચું)) ||
          !plan->check_success (avar_table->map_coords_2_14 (normalized_defaults.arrayZ, coords_len, સાચું)) ||
          !plan->check_success (avar_table->map_coords_2_14 (normalized_maxs.arrayZ, coords_len, સાચું)))
        પાછા ખોટું;

      માટે (અચલ auto& _ : + hb_enumerate (axes))
      {
        unsigned i = _.first;
        hb_tag_t axis_tag = _.second.get_axis_tag ();
        જો (plan->user_axes_location.has (axis_tag))
        {
          /* Store intermediate-space coords for offset compensation,
           * segment-map renormalization, and IVS rebasing. */
          plan->old_intermediates.set (axis_tag, Triple ((દ્વિગુણ) normalized_mins[i],
                                                         (દ્વિગુણ) normalized_defaults[i],
                                                         (દ્વિગુણ) normalized_maxs[i]));
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
      તાર્કિક detect_self_contained = !hb_blob_get_length (cff2_blob);
      hb_blob_destroy (cff2_blob);
      જો (!_compute_avar2_reachable_ranges (plan, axes, avar_table,
                                            detect_self_contained))
        પાછા ખોટું;

#ifndef HB_NO_VAR_COMPOSITES
      /* VARC overrides can reach coordinates outside the font-level
       * ranges. Do not cull variation data using those ranges. */
      જો (!plan->drop_tables.has (HB_TAG ('V','A','R','C')))
      {
        hb_blob_t *varc_blob = hb_face_reference_table (face, HB_TAG ('V','A','R','C'));
        જો (hb_blob_get_length (varc_blob))
          plan->avar2_reachable_ranges.reset ();
        hb_blob_destroy (varc_blob);
      }
#endif

      /* Keep all axes in fvar (pinned ones as hidden), EXCEPT self-contained
       * pinned axes, which are removed entirely. */
      plan->axes_index_map.reset ();
      plan->axis_tags.reset ();
      unsigned retained_axis_idx = 0;
      માટે (અચલ auto& _ : + hb_enumerate (axes))
      {
        unsigned i = _.first;
        hb_tag_t axis_tag = _.second.get_axis_tag ();
        જો (plan->avar2_self_contained.has (axis_tag))
          ચાલુ;
        plan->axes_index_map.set (i, retained_axis_idx++);
        plan->axis_tags.push (axis_tag);
      }
      /* The other tables see ONLY the self-contained pins, as an ordinary
       * partial instancing in old final-coordinate space; the standard
       * instancing machinery bakes those axes' contributions in. The
       * remaining restriction is carried entirely by avar. */
      plan->axes_location.reset ();
      જો (plan->avar2_self_contained.get_population ())
      {
        તાર્કિક sc_pinned_at_default = સાચું;
        માટે (auto _ : plan->avar2_self_contained)
        {
          plan->axes_location.set (_.first, Triple (_.second, _.second, _.second));
          જો (_.second != 0.0)
            sc_pinned_at_default = ખોટું;
        }
        માટે (અચલ auto& _ : + hb_enumerate (axes))
        {
          દ્વિગુણ *v;
          plan->normalized_coords[_.first] =
              plan->avar2_self_contained.has (_.second.get_axis_tag (), &v)
              ? (પૂર્ણાંક) roundf ((દશાંશ) (*v * 16384.0)) : 0;
        }
        plan->pinned_at_default = sc_pinned_at_default;
      }
      નહીંતર
        plan->normalized_coords.shrink (0);
    }
    નહીંતર
    {
      /* Standard avar v1 (or v1+v2 with all axes pinned) */
      જો (!plan->check_success (avar_table->map_coords_2_14 (normalized_mins.arrayZ, coords_len)) ||
          !plan->check_success (avar_table->map_coords_2_14 (normalized_defaults.arrayZ, coords_len)) ||
          !plan->check_success (avar_table->map_coords_2_14 (normalized_maxs.arrayZ, coords_len)))
        પાછા ખોટું;

      માટે (અચલ auto& _ : + hb_enumerate (axes))
      {
        unsigned i = _.first;
        hb_tag_t axis_tag = _.second.get_axis_tag ();
        જો (plan->user_axes_location.has (axis_tag))
        {
          plan->axes_location.set (axis_tag, Triple ((દ્વિગુણ) normalized_mins[i],
                                                     (દ્વિગુણ) normalized_defaults[i],
                                                     (દ્વિગુણ) normalized_maxs[i]));
          દશાંશ normalized_default = normalized_defaults[i];
          જો (normalized_default == -0.f)
            normalized_default = 0.f;
          જો (normalized_default != 0.f)
            plan->pinned_at_default = ખોટું;

          plan->normalized_coords[i] = roundf (normalized_default * 16384.f);
        }
      }
    }
  }
  પાછા સાચું;
}

#ifndef HB_NO_OT_FONT_CFF
શૂન્ય
update_instance_metrics_map_from_cff2 (hb_subset_plan_t *plan)
{
  જો (!plan->normalized_coords) પાછા;
  OT::cff2::accelerator_t cff2 (plan->source);
  જો (!cff2.is_valid ()) પાછા;

  hb_font_t *font = _get_hb_font_with_variations (plan);
  જો (unlikely (!plan->check_success (font != nullptr)))
  {
    hb_font_destroy (font);
    પાછા;
  }

  hb_glyph_extents_t extents = {0x7FFF, -0x7FFF};
  OT::hmtx_accelerator_t _hmtx (plan->source);
  OT::hb_scalar_cache_t *hvar_store_cache = nullptr;
  જો (_hmtx.has_data () && _hmtx.var_table.get_length ())
    hvar_store_cache = _hmtx.var_table->get_var_store ().create_cache ();

  OT::vmtx_accelerator_t _vmtx (plan->source);
  OT::hb_scalar_cache_t *vvar_store_cache = nullptr;
  જો (_vmtx.has_data () && _vmtx.var_table.get_length ())
    vvar_store_cache = _vmtx.var_table->get_var_store ().create_cache ();

  માટે (auto p : *plan->glyph_map)
  {
    hb_codepoint_t old_gid = p.first;
    hb_codepoint_t new_gid = p.second;
    જો (!cff2.get_extents (font, old_gid, &extents)) ચાલુ;
    તાર્કિક has_bounds_info = સાચું;
    જો (extents.x_bearing == 0 && extents.width == 0 &&
        extents.height == 0 && extents.y_bearing == 0)
      has_bounds_info = ખોટું;

    જો (has_bounds_info)
    {
      plan->head_maxp_info.xMin = hb_min (plan->head_maxp_info.xMin, extents.x_bearing);
      plan->head_maxp_info.xMax = hb_max (plan->head_maxp_info.xMax,
						 hb_saturate_add (extents.x_bearing, extents.width));
      plan->head_maxp_info.yMax = hb_max (plan->head_maxp_info.yMax, extents.y_bearing);
      plan->head_maxp_info.yMin = hb_min (plan->head_maxp_info.yMin,
						 hb_saturate_add (extents.y_bearing, extents.height));
    }

    જો (_hmtx.has_data ())
    {
      દ્વિગુણ hori_aw = _hmtx.get_advance_without_var_unscaled (old_gid);
      જો (_hmtx.var_table.get_length ())
        hori_aw += (દ્વિગુણ) roundf (_hmtx.var_table->get_advance_delta_unscaled (old_gid, font->coords, font->અંક_coords,
									 hvar_store_cache));
      /* Malicious deltas can take the advance out of the UFWORD range that
       * hmtx serialization stores.  Clamp, to keep downstream consumers
       * (e.g. CFF width optimizer) bounded and consistent with hmtx. */
      hori_aw = hb_clamp (hori_aw, 0., 65535.);
      પૂર્ણાંક lsb = extents.x_bearing;
      જો (!has_bounds_info)
      {
        _hmtx.get_leading_bearing_without_var_unscaled (old_gid, &lsb);
      }
      plan->hmtx_map.set (new_gid, hb_pair ((unsigned) hori_aw, lsb));
      plan->bounds_width_vec[new_gid] = extents.width;
    }

    જો (_vmtx.has_data ())
    {
      દ્વિગુણ vert_aw = _vmtx.get_advance_without_var_unscaled (old_gid);
      જો (_vmtx.var_table.get_length ())
        vert_aw += (દ્વિગુણ) roundf (_vmtx.var_table->get_advance_delta_unscaled (old_gid, font->coords, font->અંક_coords,
									 vvar_store_cache));
      vert_aw = hb_clamp (vert_aw, 0., 65535.);
      hb_position_t vorg_x = 0;
      hb_position_t vorg_y = 0;
      પૂર્ણાંક tsb = 0;
      જો (has_bounds_info &&
           hb_font_get_glyph_v_origin (font, old_gid, &vorg_x, &vorg_y))
      {
        tsb = hb_saturate_sub (vorg_y, extents.y_bearing);
      } નહીંતર {
        _vmtx.get_leading_bearing_without_var_unscaled (old_gid, &tsb);
      }

      plan->vmtx_map.set (new_gid, hb_pair ((unsigned) vert_aw, tsb));
      plan->bounds_height_vec[new_gid] = extents.height;
    }
  }
  /* No glyph contributed bounds: do not serialize the accumulator's
   * inverted sentinels into head (or a downgraded CFF FontBBox). */
  જો (plan->head_maxp_info.xMin > plan->head_maxp_info.xMax)
  {
    plan->head_maxp_info.xMin = plan->head_maxp_info.xMax = 0;
    plan->head_maxp_info.yMin = plan->head_maxp_info.yMax = 0;
  }
  hb_font_destroy (font);
  જો (hvar_store_cache)
    _hmtx.var_table->get_var_store ().destroy_cache (hvar_store_cache);
  જો (vvar_store_cache)
    _vmtx.var_table->get_var_store ().destroy_cache (vvar_store_cache);
}
#endif

તાર્કિક
get_instance_glyphs_contour_points (hb_subset_plan_t *plan)
{
  /* contour_points vector only needed for updating gvar table (infer delta and
   * iup delta optimization) during partial instancing */
  જો (plan->user_axes_location.is_empty () || plan->all_axes_pinned)
    પાછા સાચું;

  OT::glyf_accelerator_t glyf (plan->source);

  માટે (auto &_ : plan->new_to_old_gid_list)
  {
    hb_codepoint_t new_gid = _.first;
    contour_point_vector_t all_points;
    જો (new_gid == 0 && !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      જો (unlikely (!plan->new_gid_contour_points_map.set (new_gid, all_points)))
        પાછા ખોટું;
      ચાલુ;
    }

    hb_codepoint_t old_gid = _.second;
    auto glyph = glyf.glyph_for_gid (old_gid);
    જો (unlikely (!glyph.get_all_points_without_var (plan->source, all_points)))
      પાછા ખોટું;
    જો (unlikely (!plan->new_gid_contour_points_map.set (new_gid, all_points)))
      પાછા ખોટું;

    /* composite new gids are only needed by iup delta optimization */
    જો ((plan->flags & HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS) && glyph.is_composite ())
      plan->composite_new_gids.add (new_gid);
  }
  પાછા સાચું;
}

template<typename DeltaSetIndexMap>
શૂન્ય
remap_colrv1_delta_set_index_indices (અચલ DeltaSetIndexMap &index_map,
                                      અચલ hb_set_t &delta_set_idxes,
                                      hb_hashmap_t<unsigned, hb_pair_t<unsigned, પૂર્ણાંક>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /* OUT */)
{
  જો (!index_map.get_map_ગણતરી ())
    પાછા;

  hb_hashmap_t<unsigned, hb_pair_t<unsigned, પૂર્ણાંક>> delta_set_idx_delta_map;
  unsigned new_delta_set_idx = 0;
  માટે (unsigned delta_set_idx : delta_set_idxes)
  {
    unsigned var_idx = index_map.map (delta_set_idx);
    unsigned new_varidx = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    પૂર્ણાંક delta = 0;

    જો (var_idx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
    {
      hb_pair_t<unsigned, પૂર્ણાંક> *new_varidx_delta;
      જો (!variation_idx_delta_map.has (var_idx, &new_varidx_delta)) ચાલુ;

      new_varidx = hb_first (*new_varidx_delta);
      delta = hb_second (*new_varidx_delta);
    }

    new_deltaset_idx_varidx_map.set (new_delta_set_idx, new_varidx);
    delta_set_idx_delta_map.set (delta_set_idx, hb_pair_t<unsigned, પૂર્ણાંક> (new_delta_set_idx, delta));
    new_delta_set_idx++;
  }
  variation_idx_delta_map = std::move (delta_set_idx_delta_map);
}

template શૂન્ય
remap_colrv1_delta_set_index_indices<OT::DeltaSetIndexMap> (અચલ OT::DeltaSetIndexMap &index_map,
                                      અચલ hb_set_t &delta_set_idxes,
                                      hb_hashmap_t<unsigned, hb_pair_t<unsigned, પૂર્ણાંક>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /* OUT */);

 #endif

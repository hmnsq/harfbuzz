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

 #समावेश "hb-ot-layout-common.hh"
#समावेश "hb-subset-plan.hh"

 #समावेश "hb-ot-var-common.hh"
 #समावेश "hb-ot-layout-base-table.hh"
 #समावेश "hb-ot-glyf-table.hh"
 #समावेश "hb-ot-var-fvar-table.hh"
 #समावेश "hb-ot-var-avar-table.hh"
 #समावेश "hb-ot-cff2-table.hh"

 #अगर_अघोषित HB_NO_VAR

 व्योम
 generate_varstore_inner_maps (स्थिर hb_set_t& varidx_set,
                               अचिन्हित subtable_गिनती,
                               hb_vector_t<hb_inc_bimap_t> &inner_maps /* OUT */)
 {
   अगर (varidx_set.is_empty () || subtable_गिनती == 0) वापस;

   अगर (unlikely (!inner_maps.resize (subtable_गिनती))) वापस;
   क्रम (अचिन्हित idx : varidx_set)
   {
     uint16_t major = idx >> 16;
     uint16_t minor = idx & 0xFFFF;

     अगर (major >= subtable_गिनती)
       जारी;
     inner_maps[major].add (minor);
   }
 }

 /* Evaluate a variation-region tent at v, mirroring VarRegionAxis::evaluate
  * (invalid regions evaluate as constant 1). */
 अटल द्विगुन
 _tent_eval (स्थिर Triple &tent, द्विगुन v)
 {
   द्विगुन start = (द्विगुन) tent.minimum;
   द्विगुन peak = (द्विगुन) tent.मध्य;
   द्विगुन end = (द्विगुन) tent.maximum;
   अगर (unlikely (start > peak || peak > end)) वापस 1.0;
   अगर (unlikely (start < 0.0 && end > 0.0 && peak != 0.0)) वापस 1.0;
   अगर (peak == 0.0 || v == peak) वापस 1.0;
   अगर (v <= start || end <= v) वापस 0.0;
   अगर (v < peak) वापस (v - start) / (peak - start);
   वापस (end - v) / (end - peak);
 }

 /* Range of a tent's scalar over an input interval [lo, hi]. The tent is
  * unimodal, so the extremes are at the interval endpoints, plus the peak
  * when it falls inside. */
 अटल व्योम
 tent_मूल्य_range (स्थिर Triple &tent, द्विगुन lo, द्विगुन hi,
                    द्विगुन *tmin, द्विगुन *tmax)
 {
   द्विगुन a = _tent_eval (tent, lo);
   द्विगुन b = _tent_eval (tent, hi);
   *tmin = hb_min (a, b);
   *tmax = hb_max (a, b);
   अगर (lo <= (द्विगुन) tent.मध्य && (द्विगुन) tent.मध्य <= hi)
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
 अटल bool
 _compute_avar2_reachable_ranges (hb_subset_plan_t *plan,
                                  hb_array_t<स्थिर OT::AxisRecord> axes,
                                  स्थिर OT::avar *avar_table,
                                  bool detect_self_contained)
 {
   स्थिर OT::ItemVariationStore *var_store;
   स्थिर OT::Delta_Set_सूचकांक_Map *varidx_map;
   अगर (!avar_table->get_v2_store_and_map (&var_store, &varidx_map))
     वापस false;

   /* Old-intermediate input box per axis. */
   hb_hashmap_t<hb_tag_t, hb_pair_t<द्विगुन, द्विगुन>> box;
   क्रम (स्थिर स्वतः &axis : axes)
   {
     hb_tag_t tag = axis.get_axis_tag ();
     द्विगुन lo = -1.0, hi = +1.0;
     Triple *old_int;
     अगर (plan->user_axes_location.has (tag))
     {
       अगर (!plan->old_intermediates.has (tag, &old_int)) वापस false;
       lo = (द्विगुन) old_int->minimum;
       hi = (द्विगुन) old_int->maximum;
     }
     अन्यथा अगर (axis.is_hidden ())
       lo = hi = 0.0; /* private axis: intermediate is always 0 */
     अगर (!box.set (tag, hb_pair (lo, hi))) वापस false;
   }

   hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> regions;
   अगर (!var_store->get_region_list ().get_var_regions (plan->axes_old_सूचकांक_tag_map, regions))
     वापस false;

   hb_hashmap_t<hb_tag_t, अचिन्हित> grid_pos;
   hb_vector_t<hb_vector_t<द्विगुन>> grid_points;
   क्रम (अचिन्हित i = 0; i < axes.लंबाई; i++)
   {
     grid_pos.reset ();
     grid_points.reset ();

     hb_tag_t tag = axes[i].get_axis_tag ();
     hb_pair_t<द्विगुन, द्विगुन> *identity;
     अगर (!box.has (tag, &identity)) वापस false;

     /* Collect the row's active regions (non-zero delta). A varidx (or the
      * implicit identity mapping) that doesn't resolve to a real store row
      * behaves as "no variation" at runtime, so it contributes no regions;
      * this also covers a NULL VarStore. */
     hb_vector_t<hb_pair_t<पूर्णांक, अचिन्हित>> active; /* (delta, region index) */
     uint32_t varidx = varidx_map->map (i);
     /* Runtime treats out-of-range delta-set indices as zero. */
     अगर (varidx != HB_OT_LAYOUT_NO_VARIATIONS_सूचकांक &&
	 !var_store->has_delta_set (varidx))
       varidx = HB_OT_LAYOUT_NO_VARIATIONS_सूचकांक;
     अगर (varidx != HB_OT_LAYOUT_NO_VARIATIONS_सूचकांक)
     {
       अचिन्हित outer = varidx >> 16;
       अचिन्हित inner = varidx & 0xFFFF;
       स्थिर OT::Var_तथ्य &vd = var_store->get_sub_table (outer);
       स्थिर OT::HBUINT8 *delta_bytes = vd.get_delta_bytes ();
       अचिन्हित row_आकार = vd.get_row_आकार ();
       अचिन्हित region_गिनती_2 = vd.get_region_सूचकांक_गिनती ();
       क्रम (अचिन्हित r = 0; r < region_गिनती_2; r++)
       {
	 पूर्णांक delta = vd.get_item_delta_fast (inner, r, delta_bytes, row_आकार);
	 अगर (!delta) जारी;
	 अचिन्हित region_idx = vd.get_region_सूचकांक (r);
	 अगर (region_idx >= regions.लंबाई) वापस false;
	 active.push (hb_pair (delta, region_idx));
       }
       अगर (active.in_error ()) वापस false;
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
     bool exact = true;
     द्विगुन dmin = 0.0, dmax = 0.0; /* delta extremes, F2Dot14 units */
     द्विगुन vmin = 0.0, vmax = 0.0; /* identity + delta extremes, coord units */

     /* Grid axes: the target axis (identity term), plus every axis a valid
      * tent of an active region references. */
     स्वतः add_grid_axis = [&] (hb_tag_t t) -> पूर्णांक
     {
       अचिन्हित *pos;
       अगर (grid_pos.has (t, &pos)) वापस (पूर्णांक) *pos;
       अचिन्हित new_pos = grid_pos.get_population ();
       hb_pair_t<द्विगुन, द्विगुन> *निवेश;
       द्विगुन blo = -1.0, bhi = +1.0;
       अगर (box.has (t, &निवेश)) { blo = निवेश->first; bhi = निवेश->second; }
       grid_points.push (hb_vector_t<द्विगुन> ());
       अगर (grid_points.in_error ()) वापस -1;
       स्वतः &points = grid_points[new_pos];
       points.push (blo);
       points.push (bhi);
       अगर (points.in_error () || !grid_pos.set (t, new_pos)) वापस -1;
       वापस (पूर्णांक) new_pos;
     };

     अगर (add_grid_axis (tag) < 0) वापस false;
     /* Per active region: (grid axis position, tent) for its valid tents. */
     hb_vector_t<hb_vector_t<hb_pair_t<अचिन्हित, Triple>>> region_tents;
     क्रम (स्थिर स्वतः &ar : active)
     {
       region_tents.push (hb_vector_t<hb_pair_t<अचिन्हित, Triple>> ());
       अगर (region_tents.in_error ()) वापस false;
       स्वतः &tents = region_tents[region_tents.लंबाई - 1];
       क्रम (स्थिर स्वतः &_ : regions[ar.second])
       {
	 स्थिर Triple &tent = _.second;
	 /* Tents the runtime ignores (constant scalar 1) add no breakpoints. */
	 अगर (tent.मध्य == 0.f ||
	     tent.minimum > tent.मध्य || tent.मध्य > tent.maximum ||
	     (tent.minimum < 0.f && tent.maximum > 0.f))
	   जारी;
	 पूर्णांक pos = add_grid_axis (_.first);
	 अगर (pos < 0) वापस false;
	 स्वतः &points = grid_points[pos];
	 द्विगुन blo = points[0], bhi = points[1];
	 points.push (hb_clamp ((द्विगुन) tent.minimum, blo, bhi));
	 points.push (hb_clamp ((द्विगुन) tent.मध्य, blo, bhi));
	 points.push (hb_clamp ((द्विगुन) tent.maximum, blo, bhi));
	 tents.push (hb_pair ((अचिन्हित) pos, tent));
	 अगर (points.in_error () || tents.in_error ()) वापस false;
       }
     }

     अचिन्हित grid_आकार = 1;
     क्रम (स्वतः &points : grid_points)
     {
       points.क्विक ([] (स्थिर द्विगुन &a, स्थिर द्विगुन &b) -> पूर्णांक
		     { वापस a < b ? -1 : a > b ? +1 : 0; });
       अचिन्हित n = 0;
       क्रम (अचिन्हित j = 0; j < points.लंबाई; j++)
	 अगर (!n || points.arrayZ[j] != points.arrayZ[n - 1])
	   points.arrayZ[n++] = points.arrayZ[j];
       points.shrink (n);
       अगर (grid_आकार > MAX_GRID / points.लंबाई) { exact = false; अवरोध; }
       grid_आकार *= points.लंबाई;
     }

     अगर (exact)
     {
       अचिन्हित target_pos = 0; /* the target axis was added first */
       hb_vector_t<अचिन्हित> odometer;
       अगर (!odometer.resize (grid_pos.get_population ())) वापस false;
       bool first = true;
       जबतक (true)
       {
	 द्विगुन delta_योग = 0.0;
	 क्रम (अचिन्हित r = 0; r < active.लंबाई; r++)
	 {
	   द्विगुन scalar = 1.0;
	   क्रम (स्थिर स्वतः &pt : region_tents[r])
	   {
	     द्विगुन v = grid_points[pt.first][odometer[pt.first]];
	     scalar *= _tent_eval (pt.second, v);
	     अगर (scalar == 0.0) अवरोध;
	   }
	   delta_योग += scalar * active[r].first;
	 }
	 द्विगुन coord = grid_points[target_pos][odometer[target_pos]];
	 द्विगुन value = coord + delta_योग / 16384.0;
	 अगर (first)
	 {
	   dmin = dmax = delta_योग;
	   vmin = vmax = value;
	   first = false;
	 }
	 अन्यथा
	 {
	   dmin = hb_min (dmin, delta_योग); dmax = hb_max (dmax, delta_योग);
	   vmin = hb_min (vmin, value);     vmax = hb_max (vmax, value);
	 }
	 अचिन्हित k = 0;
	 क्रम (; k < odometer.लंबाई; k++)
	 {
	   अगर (++odometer[k] < grid_points[k].लंबाई) अवरोध;
	   odometer[k] = 0;
	 }
	 अगर (k == odometer.लंबाई) अवरोध;
       }
     }
     अन्यथा
     {
       क्रम (स्थिर स्वतः &ar : active)
       {
	 पूर्णांक delta = ar.first;
	 द्विगुन smin = 1.0, smax = 1.0;
	 क्रम (स्थिर स्वतः &_ : regions[ar.second])
	 {
	   hb_pair_t<द्विगुन, द्विगुन> *निवेश;
	   द्विगुन blo = -1.0, bhi = +1.0;
	   अगर (box.has (_.first, &निवेश)) { blo = निवेश->first; bhi = निवेश->second; }
	   द्विगुन tmin, tmax;
	   tent_मूल्य_range (_.second, blo, bhi, &tmin, &tmax);
	   smin *= tmin;
	   smax *= tmax;
	   अगर (smax == 0.0) अवरोध;
	 }
	 अगर (delta > 0) { dmin += smin * delta; dmax += smax * delta; }
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
     अगर (detect_self_contained && dmin == dmax &&
	 plan->user_axes_location.has (tag, &user) && user->is_point ())
     {
       पूर्णांक v_int = (पूर्णांक) roundf ((भग्न) identity->first * 16384.f) +
		   (पूर्णांक) roundf ((भग्न) dmin);
       v_int = hb_clamp (v_int, -(1 << 14), +(1 << 14));
       अगर (!plan->avar2_self_contained.set (tag, v_int / 16384.0)) वापस false;
       जारी; /* axis is dropped; no region can reference it afterwards */
     }

     /* Pad by one F2Dot14 unit against rounding differences with the
      * runtime's fixed-point evaluation. */
     द्विगुन lo = hb_clamp (vmin - 1.0 / 16384.0, -1.0, +1.0);
     द्विगुन hi = hb_clamp (vmax + 1.0 / 16384.0, -1.0, +1.0);
     lo = न्यूनमान (lo * 16384.0) / 16384.0;
     hi = उच्चमान (hi * 16384.0) / 16384.0;
     अगर (lo <= -1.0 && hi >= +1.0)
       जारी; /* not constraining */
     अगर (!plan->avar2_reachable_ranges.set (tag, Triple (lo, hb_clamp (0.0, lo, hi), hi)))
       वापस false;
   }
   वापस true;
 }

#अगर_अघोषित HB_NO_OT_FONT_CFF
 अटल अंतरभूत hb_font_t*
 _get_hb_font_with_variations (स्थिर hb_subset_plan_t *plan)
 {
   hb_font_t *font = hb_font_create (plan->source);

   अगर (plan->has_avar2)
   {
     /* Under avar2, instancing applies only to the self-contained pins,
      * whose constant final coordinates the plan holds in normalized_coords.
      * Setting user-space variations would run the full avar2 mapping and
      * bake contributions that remain live in the instance's variations. */
     hb_font_set_var_coords_normalized (font, plan->normalized_coords.arrayZ,
                                        plan->normalized_coords.लंबाई);
     वापस font;
   }

   hb_vector_t<hb_variation_t> vars;
   अगर (!vars.alloc (plan->user_axes_location.get_population ())) {
     hb_font_destroy (font);
     वापस nullptr;
   }

   क्रम (स्वतः _ : plan->user_axes_location)
   {
     hb_variation_t var;
     var.tag = _.first;
     var.value = _.second.मध्य;
     vars.push (var);
   }

   hb_font_set_variations (font, vars.arrayZ, plan->user_axes_location.get_population ());
   वापस font;
 }
#पूर्ण_अगर

 ढाँचा<typename ItemVarStore>
 व्योम
 remap_variation_indices (स्थिर ItemVarStore &var_store,
                          स्थिर hb_set_t &variation_indices,
                          स्थिर hb_vector_t<पूर्णांक>& normalized_coords,
                          bool calculate_delta, /* not pinned at default */
                          bool no_variations, /* all axes pinned */
                          hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> &variation_idx_delta_map /* OUT */)
 {
   अगर (&var_store == &Null (OT::ItemVariationStore)) वापस;
   अचिन्हित subtable_गिनती = var_store.get_sub_table_गिनती ();
   स्वतः *store_cache = var_store.create_cache ();

   अचिन्हित new_major = 0, new_minor = 0;
   अचिन्हित last_major = (variation_indices.get_min ()) >> 16;
   क्रम (अचिन्हित idx : variation_indices)
   {
     पूर्णांक delta = 0;
     अगर (calculate_delta)
       delta = roundf (var_store.get_delta (idx, normalized_coords.arrayZ,
                                            normalized_coords.लंबाई, store_cache));

     अगर (no_variations)
     {
       variation_idx_delta_map.set (idx, hb_pair_t<अचिन्हित, पूर्णांक> (HB_OT_LAYOUT_NO_VARIATIONS_सूचकांक, delta));
       जारी;
     }

     uint16_t major = idx >> 16;
     अगर (major >= subtable_गिनती) अवरोध;
     अगर (major != last_major)
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
 व्योम
 remap_variation_indices<OT::ItemVariationStore> (स्थिर OT::ItemVariationStore &var_store,
                          स्थिर hb_set_t &variation_indices,
                          स्थिर hb_vector_t<पूर्णांक>& normalized_coords,
                          bool calculate_delta, /* not pinned at default */
                          bool no_variations, /* all axes pinned */
                          hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> &variation_idx_delta_map /* OUT */);

 #अगर_अघोषित HB_NO_BASE
 व्योम
 collect_base_variation_indices (hb_subset_plan_t* plan)
 {
   hb_blob_ptr_t<OT::BASE> base = plan->source_table<OT::BASE> ();
   अगर (!base->has_var_store ())
   {
     base.destroy ();
     वापस;
   }

   hb_set_t varidx_set;
   base->collect_variation_indices (plan, varidx_set);
   स्थिर OT::ItemVariationStore &var_store = base->get_var_store ();
   अचिन्हित subtable_गिनती = var_store.get_sub_table_गिनती ();


   remap_variation_indices (var_store, varidx_set,
                             plan->normalized_coords,
                             !plan->pinned_at_default,
                             plan->all_axes_pinned,
                             plan->base_variation_idx_map);
   generate_varstore_inner_maps (varidx_set, subtable_गिनती, plan->base_varstore_inner_maps);

   base.destroy ();
 }

 #पूर्ण_अगर

bool
normalize_axes_location (hb_face_t *face, hb_subset_plan_t *plan)
{
  अगर (plan->user_axes_location.is_empty ())
    वापस true;

  hb_array_t<स्थिर OT::AxisRecord> axes = face->table.fvar->get_axes ();
  क्रम (स्थिर स्वतः &axis : axes)
  {
    hb_tag_t tag = axis.get_axis_tag ();
    स्थिर Triple *range;
    भग्न minimum, मध्य, maximum;
    axis.get_coordinates (minimum, मध्य, maximum);
    अगर (plan->user_axes_location.has (tag, &range) && !range->is_point () &&
        range->minimum == minimum &&
        range->मध्य == मध्य &&
        range->maximum == maximum)
      plan->user_axes_location.del (tag);
  }
  अगर (plan->user_axes_location.is_empty ())
    वापस true;

  अगर (!plan->check_success (plan->normalized_coords.resize (axes.लंबाई)))
    वापस false;

  bool has_avar = face->table.avar->has_तथ्य ();
  hb_vector_t<भग्न> normalized_mins;
  hb_vector_t<भग्न> normalized_defaults;
  hb_vector_t<भग्न> normalized_maxs;
  अगर (has_avar)
  {
    अगर (!plan->check_success (normalized_mins.resize (axes.लंबाई)) ||
        !plan->check_success (normalized_defaults.resize (axes.लंबाई)) ||
        !plan->check_success (normalized_maxs.resize (axes.लंबाई)))
      वापस false;
  }

  bool axis_not_pinned = false;
  अचिन्हित new_axis_idx = 0;
  अचिन्हित last_idx = 0;
  क्रम (स्थिर स्वतः& _ : + hb_enumerate (axes))
  {
    अचिन्हित i = _.first;
    स्थिर OT::AxisRecord &axis = _.second;
    hb_tag_t axis_tag = axis.get_axis_tag ();
    plan->axes_old_सूचकांक_tag_map.set (i, axis_tag);

    अगर (!plan->user_axes_location.has (axis_tag) ||
        !plan->user_axes_location.लो (axis_tag).is_point ())
    {
      axis_not_pinned = true;
      plan->axes_सूचकांक_map.set (i, new_axis_idx);
      plan->axis_tags.push (axis_tag);
      new_axis_idx++;
    }

    Triple *axis_range;
    अगर (plan->user_axes_location.has (axis_tag, &axis_range))
    {
      plan->axes_triple_distances.set (axis_tag, axis.get_triple_distances ());

      भग्न normalized_min = axis.normalize_axis_मूल्य (axis_range->minimum);
      भग्न normalized_default = axis.normalize_axis_मूल्य (axis_range->मध्य);
      भग्न normalized_max = axis.normalize_axis_मूल्य (axis_range->maximum);

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

      अगर (has_avar)
      {
        normalized_mins[i] = normalized_min;
        normalized_defaults[i] = normalized_default;
        normalized_maxs[i] = normalized_max;
        last_idx = i;
      }
      अन्यथा
      {
        plan->axes_location.set (axis_tag, Triple ((द्विगुन) normalized_min,
                                                   (द्विगुन) normalized_default,
                                                   (द्विगुन) normalized_max));
        अगर (normalized_default == -0.f)
          normalized_default = 0.f; // Normalize -0 to 0
        अगर (normalized_default != 0.f)
          plan->pinned_at_default = false;

        plan->normalized_coords[i] = roundf (normalized_default * 16384.f);
      }
    }
  }
  plan->all_axes_pinned = !axis_not_pinned;

  // TODO: use avar map_coords_16_16() when normalization is changed to 16.16
  // in fonttools
  अगर (has_avar)
  {
    स्थिर OT::avar* avar_table = face->table.avar;
    अचिन्हित coords_len = last_idx + 1;

    अगर (avar_table->has_v2_तथ्य () && !plan->all_axes_pinned)
    {
      /* avar2 partial instancing: use v1-only mapping to get intermediate-space
       * coords. These are used for IVS rebasing and offset compensation. */
      plan->has_avar2 = true;

      अगर (!plan->check_success (avar_table->map_coords_2_14 (normalized_mins.arrayZ, coords_len, true)) ||
          !plan->check_success (avar_table->map_coords_2_14 (normalized_defaults.arrayZ, coords_len, true)) ||
          !plan->check_success (avar_table->map_coords_2_14 (normalized_maxs.arrayZ, coords_len, true)))
        वापस false;

      क्रम (स्थिर स्वतः& _ : + hb_enumerate (axes))
      {
        अचिन्हित i = _.first;
        hb_tag_t axis_tag = _.second.get_axis_tag ();
        अगर (plan->user_axes_location.has (axis_tag))
        {
          /* Store intermediate-space coords for offset compensation,
           * segment-map renormalization, and IVS rebasing. */
          plan->old_intermediates.set (axis_tag, Triple ((द्विगुन) normalized_mins[i],
                                                         (द्विगुन) normalized_defaults[i],
                                                         (द्विगुन) normalized_maxs[i]));
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
      bool detect_self_contained = !hb_blob_get_लंबाई (cff2_blob);
      hb_blob_destroy (cff2_blob);
      अगर (!_compute_avar2_reachable_ranges (plan, axes, avar_table,
                                            detect_self_contained))
        वापस false;

#अगर_अघोषित HB_NO_VAR_COMPOSITES
      /* VARC overrides can reach coordinates outside the font-level
       * ranges. Do not cull variation data using those ranges. */
      अगर (!plan->drop_tables.has (HB_TAG ('V','A','R','C')))
      {
        hb_blob_t *varc_blob = hb_face_reference_table (face, HB_TAG ('V','A','R','C'));
        अगर (hb_blob_get_लंबाई (varc_blob))
          plan->avar2_reachable_ranges.reset ();
        hb_blob_destroy (varc_blob);
      }
#पूर्ण_अगर

      /* Keep all axes in fvar (pinned ones as hidden), EXCEPT self-contained
       * pinned axes, which are removed entirely. */
      plan->axes_सूचकांक_map.reset ();
      plan->axis_tags.reset ();
      अचिन्हित retained_axis_idx = 0;
      क्रम (स्थिर स्वतः& _ : + hb_enumerate (axes))
      {
        अचिन्हित i = _.first;
        hb_tag_t axis_tag = _.second.get_axis_tag ();
        अगर (plan->avar2_self_contained.has (axis_tag))
          जारी;
        plan->axes_सूचकांक_map.set (i, retained_axis_idx++);
        plan->axis_tags.push (axis_tag);
      }
      /* The other tables see ONLY the self-contained pins, as an ordinary
       * partial instancing in old final-coordinate space; the standard
       * instancing machinery bakes those axes' contributions in. The
       * remaining restriction is carried entirely by avar. */
      plan->axes_location.reset ();
      अगर (plan->avar2_self_contained.get_population ())
      {
        bool sc_pinned_at_default = true;
        क्रम (स्वतः _ : plan->avar2_self_contained)
        {
          plan->axes_location.set (_.first, Triple (_.second, _.second, _.second));
          अगर (_.second != 0.0)
            sc_pinned_at_default = false;
        }
        क्रम (स्थिर स्वतः& _ : + hb_enumerate (axes))
        {
          द्विगुन *v;
          plan->normalized_coords[_.first] =
              plan->avar2_self_contained.has (_.second.get_axis_tag (), &v)
              ? (पूर्णांक) roundf ((भग्न) (*v * 16384.0)) : 0;
        }
        plan->pinned_at_default = sc_pinned_at_default;
      }
      अन्यथा
        plan->normalized_coords.shrink (0);
    }
    अन्यथा
    {
      /* Standard avar v1 (or v1+v2 with all axes pinned) */
      अगर (!plan->check_success (avar_table->map_coords_2_14 (normalized_mins.arrayZ, coords_len)) ||
          !plan->check_success (avar_table->map_coords_2_14 (normalized_defaults.arrayZ, coords_len)) ||
          !plan->check_success (avar_table->map_coords_2_14 (normalized_maxs.arrayZ, coords_len)))
        वापस false;

      क्रम (स्थिर स्वतः& _ : + hb_enumerate (axes))
      {
        अचिन्हित i = _.first;
        hb_tag_t axis_tag = _.second.get_axis_tag ();
        अगर (plan->user_axes_location.has (axis_tag))
        {
          plan->axes_location.set (axis_tag, Triple ((द्विगुन) normalized_mins[i],
                                                     (द्विगुन) normalized_defaults[i],
                                                     (द्विगुन) normalized_maxs[i]));
          भग्न normalized_default = normalized_defaults[i];
          अगर (normalized_default == -0.f)
            normalized_default = 0.f;
          अगर (normalized_default != 0.f)
            plan->pinned_at_default = false;

          plan->normalized_coords[i] = roundf (normalized_default * 16384.f);
        }
      }
    }
  }
  वापस true;
}

#अगर_अघोषित HB_NO_OT_FONT_CFF
व्योम
update_instance_metrics_map_from_cff2 (hb_subset_plan_t *plan)
{
  अगर (!plan->normalized_coords) वापस;
  OT::cff2::accelerator_t cff2 (plan->source);
  अगर (!cff2.is_valid ()) वापस;

  hb_font_t *font = _get_hb_font_with_variations (plan);
  अगर (unlikely (!plan->check_success (font != nullptr)))
  {
    hb_font_destroy (font);
    वापस;
  }

  hb_glyph_extents_t extents = {0x7FFF, -0x7FFF};
  OT::hmtx_accelerator_t _hmtx (plan->source);
  OT::hb_scalar_cache_t *hvar_store_cache = nullptr;
  अगर (_hmtx.has_तथ्य () && _hmtx.var_table.get_लंबाई ())
    hvar_store_cache = _hmtx.var_table->get_var_store ().create_cache ();

  OT::vmtx_accelerator_t _vmtx (plan->source);
  OT::hb_scalar_cache_t *vvar_store_cache = nullptr;
  अगर (_vmtx.has_तथ्य () && _vmtx.var_table.get_लंबाई ())
    vvar_store_cache = _vmtx.var_table->get_var_store ().create_cache ();

  क्रम (स्वतः p : *plan->glyph_map)
  {
    hb_codepoint_t old_gid = p.first;
    hb_codepoint_t new_gid = p.second;
    अगर (!cff2.get_extents (font, old_gid, &extents)) जारी;
    bool has_bounds_info = true;
    अगर (extents.x_bearing == 0 && extents.width == 0 &&
        extents.height == 0 && extents.y_bearing == 0)
      has_bounds_info = false;

    अगर (has_bounds_info)
    {
      plan->शीर्ष_maxp_info.xMin = hb_min (plan->शीर्ष_maxp_info.xMin, extents.x_bearing);
      plan->शीर्ष_maxp_info.xMax = hb_max (plan->शीर्ष_maxp_info.xMax,
						 hb_saturate_add (extents.x_bearing, extents.width));
      plan->शीर्ष_maxp_info.yMax = hb_max (plan->शीर्ष_maxp_info.yMax, extents.y_bearing);
      plan->शीर्ष_maxp_info.yMin = hb_min (plan->शीर्ष_maxp_info.yMin,
						 hb_saturate_add (extents.y_bearing, extents.height));
    }

    अगर (_hmtx.has_तथ्य ())
    {
      द्विगुन hori_aw = _hmtx.get_advance_without_var_unscaled (old_gid);
      अगर (_hmtx.var_table.get_लंबाई ())
        hori_aw += (द्विगुन) roundf (_hmtx.var_table->get_advance_delta_unscaled (old_gid, font->coords, font->अंक_coords,
									 hvar_store_cache));
      /* Malicious deltas can take the advance out of the UFWORD range that
       * hmtx serialization stores.  Clamp, to keep downstream consumers
       * (e.g. CFF width optimizer) bounded and consistent with hmtx. */
      hori_aw = hb_clamp (hori_aw, 0., 65535.);
      पूर्णांक lsb = extents.x_bearing;
      अगर (!has_bounds_info)
      {
        _hmtx.get_leading_bearing_without_var_unscaled (old_gid, &lsb);
      }
      plan->hmtx_map.set (new_gid, hb_pair ((अचिन्हित) hori_aw, lsb));
      plan->bounds_width_vec[new_gid] = extents.width;
    }

    अगर (_vmtx.has_तथ्य ())
    {
      द्विगुन vert_aw = _vmtx.get_advance_without_var_unscaled (old_gid);
      अगर (_vmtx.var_table.get_लंबाई ())
        vert_aw += (द्विगुन) roundf (_vmtx.var_table->get_advance_delta_unscaled (old_gid, font->coords, font->अंक_coords,
									 vvar_store_cache));
      vert_aw = hb_clamp (vert_aw, 0., 65535.);
      hb_position_t vorg_x = 0;
      hb_position_t vorg_y = 0;
      पूर्णांक tsb = 0;
      अगर (has_bounds_info &&
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
  अगर (plan->शीर्ष_maxp_info.xMin > plan->शीर्ष_maxp_info.xMax)
  {
    plan->शीर्ष_maxp_info.xMin = plan->शीर्ष_maxp_info.xMax = 0;
    plan->शीर्ष_maxp_info.yMin = plan->शीर्ष_maxp_info.yMax = 0;
  }
  hb_font_destroy (font);
  अगर (hvar_store_cache)
    _hmtx.var_table->get_var_store ().destroy_cache (hvar_store_cache);
  अगर (vvar_store_cache)
    _vmtx.var_table->get_var_store ().destroy_cache (vvar_store_cache);
}
#पूर्ण_अगर

bool
get_instance_glyphs_contour_points (hb_subset_plan_t *plan)
{
  /* contour_points vector only needed for updating gvar table (infer delta and
   * iup delta optimization) during partial instancing */
  अगर (plan->user_axes_location.is_empty () || plan->all_axes_pinned)
    वापस true;

  OT::glyf_accelerator_t glyf (plan->source);

  क्रम (स्वतः &_ : plan->new_to_old_gid_list)
  {
    hb_codepoint_t new_gid = _.first;
    contour_point_vector_t all_points;
    अगर (new_gid == 0 && !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      अगर (unlikely (!plan->new_gid_contour_points_map.set (new_gid, all_points)))
        वापस false;
      जारी;
    }

    hb_codepoint_t old_gid = _.second;
    स्वतः glyph = glyf.glyph_for_gid (old_gid);
    अगर (unlikely (!glyph.get_all_points_without_var (plan->source, all_points)))
      वापस false;
    अगर (unlikely (!plan->new_gid_contour_points_map.set (new_gid, all_points)))
      वापस false;

    /* composite new gids are only needed by iup delta optimization */
    अगर ((plan->flags & HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS) && glyph.is_composite ())
      plan->composite_new_gids.add (new_gid);
  }
  वापस true;
}

ढाँचा<typename Delta_Set_सूचकांक_Map>
व्योम
remap_colrv1_delta_set_सूचकांक_indices (स्थिर Delta_Set_सूचकांक_Map &सूचकांक_map,
                                      स्थिर hb_set_t &delta_set_idxes,
                                      hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /* OUT */)
{
  अगर (!सूचकांक_map.get_map_गिनती ())
    वापस;

  hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> delta_set_idx_delta_map;
  अचिन्हित new_delta_set_idx = 0;
  क्रम (अचिन्हित delta_set_idx : delta_set_idxes)
  {
    अचिन्हित var_idx = सूचकांक_map.map (delta_set_idx);
    अचिन्हित new_varidx = HB_OT_LAYOUT_NO_VARIATIONS_सूचकांक;
    पूर्णांक delta = 0;

    अगर (var_idx != HB_OT_LAYOUT_NO_VARIATIONS_सूचकांक)
    {
      hb_pair_t<अचिन्हित, पूर्णांक> *new_varidx_delta;
      अगर (!variation_idx_delta_map.has (var_idx, &new_varidx_delta)) जारी;

      new_varidx = hb_first (*new_varidx_delta);
      delta = hb_second (*new_varidx_delta);
    }

    new_deltaset_idx_varidx_map.set (new_delta_set_idx, new_varidx);
    delta_set_idx_delta_map.set (delta_set_idx, hb_pair_t<अचिन्हित, पूर्णांक> (new_delta_set_idx, delta));
    new_delta_set_idx++;
  }
  variation_idx_delta_map = std::move (delta_set_idx_delta_map);
}

ढाँचा व्योम
remap_colrv1_delta_set_सूचकांक_indices<OT::Delta_Set_सूचकांक_Map> (स्थिर OT::Delta_Set_सूचकांक_Map &सूचकांक_map,
                                      स्थिर hb_set_t &delta_set_idxes,
                                      hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /* OUT */);

 #पूर्ण_अगर

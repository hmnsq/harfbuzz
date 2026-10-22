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
 generate_varstore_inner_maps (ध्रुव hb_set_t& varidx_set,
                               अचिन्हित subtable_गिनती,
                               hb_vector_t<hb_inc_bimap_t> &inner_maps /* OUT */)
 {
   यदि (varidx_set.is_empty () || subtable_गिनती == 0) प्रत्यागम;

   यदि (unlikely (!inner_maps.resize (subtable_गिनती))) प्रत्यागम;
   प्रति (अचिन्हित idx : varidx_set)
   {
     uint16_t major = idx >> 16;
     uint16_t minor = idx & 0xFFFF;

     यदि (major >= subtable_गिनती)
       अनुवर्त;
     inner_maps[major].add (minor);
   }
 }

 /* Evaluate a variation-region tent at v, mirroring VarRegionAxis::evaluate
  * (invalid regions evaluate as constant 1). */
 स्थिर द्विगुण
 _tent_eval (ध्रुव Triple &tent, द्विगुण v)
 {
   द्विगुण start = (द्विगुण) tent.minimum;
   द्विगुण peak = (द्विगुण) tent.मध्य;
   द्विगुण end = (द्विगुण) tent.maximum;
   यदि (unlikely (start > peak || peak > end)) प्रत्यागम 1.0;
   यदि (unlikely (start < 0.0 && end > 0.0 && peak != 0.0)) प्रत्यागम 1.0;
   यदि (peak == 0.0 || v == peak) प्रत्यागम 1.0;
   यदि (v <= start || end <= v) प्रत्यागम 0.0;
   यदि (v < peak) प्रत्यागम (v - start) / (peak - start);
   प्रत्यागम (end - v) / (end - peak);
 }

 /* Range of a tent's scalar over an input interval [lo, hi]. The tent is
  * unimodal, so the extremes are at the interval endpoints, plus the peak
  * when it falls inside. */
 स्थिर शून्य
 tent_मूल्य_range (ध्रुव Triple &tent, द्विगुण lo, द्विगुण hi,
                    द्विगुण *tmin, द्विगुण *tmax)
 {
   द्विगुण a = _tent_eval (tent, lo);
   द्विगुण b = _tent_eval (tent, hi);
   *tmin = hb_min (a, b);
   *tmax = hb_max (a, b);
   यदि (lo <= (द्विगुण) tent.मध्य && (द्विगुण) tent.मध्य <= hi)
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
                                  hb_array_t<ध्रुव OT::AxisRecord> axes,
                                  ध्रुव OT::avar *avar_table,
                                  तार्किक detect_self_contained)
 {
   ध्रुव OT::ItemVariationStore *var_store;
   ध्रुव OT::DeltaSetIndexMap *varidx_map;
   यदि (!avar_table->get_v2_store_and_map (&var_store, &varidx_map))
     प्रत्यागम असत्य;

   /* Old-intermediate input box per axis. */
   hb_hashmap_t<hb_tag_t, hb_pair_t<द्विगुण, द्विगुण>> box;
   प्रति (ध्रुव स्वतः &axis : axes)
   {
     hb_tag_t tag = axis.get_axis_tag ();
     द्विगुण lo = -1.0, hi = +1.0;
     Triple *old_int;
     यदि (plan->user_axes_location.has (tag))
     {
       यदि (!plan->old_intermediates.has (tag, &old_int)) प्रत्यागम असत्य;
       lo = (द्विगुण) old_int->minimum;
       hi = (द्विगुण) old_int->maximum;
     }
     अन्यथा यदि (axis.is_hidden ())
       lo = hi = 0.0; /* private axis: intermediate is always 0 */
     यदि (!box.set (tag, hb_pair (lo, hi))) प्रत्यागम असत्य;
   }

   hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> regions;
   यदि (!var_store->get_region_list ().get_var_regions (plan->axes_old_index_tag_map, regions))
     प्रत्यागम असत्य;

   hb_hashmap_t<hb_tag_t, अचिन्हित> grid_pos;
   hb_vector_t<hb_vector_t<द्विगुण>> grid_points;
   प्रति (अचिन्हित i = 0; i < axes.length; i++)
   {
     grid_pos.reset ();
     grid_points.reset ();

     hb_tag_t tag = axes[i].get_axis_tag ();
     hb_pair_t<द्विगुण, द्विगुण> *identity;
     यदि (!box.has (tag, &identity)) प्रत्यागम असत्य;

     /* Collect the row's active regions (non-zero delta). A varidx (or the
      * implicit identity mapping) that doesn't resolve to a real store row
      * behaves as "no variation" at runtime, so it contributes no regions;
      * this also covers a NULL VarStore. */
     hb_vector_t<hb_pair_t<पूर्णाङ्क, अचिन्हित>> active; /* (delta, region index) */
     uint32_t varidx = varidx_map->map (i);
     /* Runtime treats out-of-range delta-set indices as zero. */
     यदि (varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX &&
	 !var_store->has_delta_set (varidx))
       varidx = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
     यदि (varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
     {
       अचिन्हित outer = varidx >> 16;
       अचिन्हित inner = varidx & 0xFFFF;
       ध्रुव OT::VarData &vd = var_store->get_sub_table (outer);
       ध्रुव OT::HBUINT8 *delta_bytes = vd.get_delta_bytes ();
       अचिन्हित row_size = vd.get_row_size ();
       अचिन्हित region_गिनती_2 = vd.get_region_index_गिनती ();
       प्रति (अचिन्हित r = 0; r < region_गिनती_2; r++)
       {
	 पूर्णाङ्क delta = vd.get_item_delta_fast (inner, r, delta_bytes, row_size);
	 यदि (!delta) अनुवर्त;
	 अचिन्हित region_idx = vd.get_region_index (r);
	 यदि (region_idx >= regions.length) प्रत्यागम असत्य;
	 active.push (hb_pair (delta, region_idx));
       }
       यदि (active.in_error ()) प्रत्यागम असत्य;
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
     स्वतः add_grid_axis = [&] (hb_tag_t t) -> पूर्णाङ्क
     {
       अचिन्हित *pos;
       यदि (grid_pos.has (t, &pos)) प्रत्यागम (पूर्णाङ्क) *pos;
       अचिन्हित new_pos = grid_pos.get_population ();
       hb_pair_t<द्विगुण, द्विगुण> *निवेश;
       द्विगुण blo = -1.0, bhi = +1.0;
       यदि (box.has (t, &निवेश)) { blo = निवेश->first; bhi = निवेश->second; }
       grid_points.push (hb_vector_t<द्विगुण> ());
       यदि (grid_points.in_error ()) प्रत्यागम -1;
       स्वतः &points = grid_points[new_pos];
       points.push (blo);
       points.push (bhi);
       यदि (points.in_error () || !grid_pos.set (t, new_pos)) प्रत्यागम -1;
       प्रत्यागम (पूर्णाङ्क) new_pos;
     };

     यदि (add_grid_axis (tag) < 0) प्रत्यागम असत्य;
     /* Per active region: (grid axis position, tent) for its valid tents. */
     hb_vector_t<hb_vector_t<hb_pair_t<अचिन्हित, Triple>>> region_tents;
     प्रति (ध्रुव स्वतः &ar : active)
     {
       region_tents.push (hb_vector_t<hb_pair_t<अचिन्हित, Triple>> ());
       यदि (region_tents.in_error ()) प्रत्यागम असत्य;
       स्वतः &tents = region_tents[region_tents.length - 1];
       प्रति (ध्रुव स्वतः &_ : regions[ar.second])
       {
	 ध्रुव Triple &tent = _.second;
	 /* Tents the runtime ignores (constant scalar 1) add no breakpoints. */
	 यदि (tent.मध्य == 0.f ||
	     tent.minimum > tent.मध्य || tent.मध्य > tent.maximum ||
	     (tent.minimum < 0.f && tent.maximum > 0.f))
	   अनुवर्त;
	 पूर्णाङ्क pos = add_grid_axis (_.first);
	 यदि (pos < 0) प्रत्यागम असत्य;
	 स्वतः &points = grid_points[pos];
	 द्विगुण blo = points[0], bhi = points[1];
	 points.push (hb_clamp ((द्विगुण) tent.minimum, blo, bhi));
	 points.push (hb_clamp ((द्विगुण) tent.मध्य, blo, bhi));
	 points.push (hb_clamp ((द्विगुण) tent.maximum, blo, bhi));
	 tents.push (hb_pair ((अचिन्हित) pos, tent));
	 यदि (points.in_error () || tents.in_error ()) प्रत्यागम असत्य;
       }
     }

     अचिन्हित grid_size = 1;
     प्रति (स्वतः &points : grid_points)
     {
       points.क्विक ([] (ध्रुव द्विगुण &a, ध्रुव द्विगुण &b) -> पूर्णाङ्क
		     { प्रत्यागम a < b ? -1 : a > b ? +1 : 0; });
       अचिन्हित n = 0;
       प्रति (अचिन्हित j = 0; j < points.length; j++)
	 यदि (!n || points.arrayZ[j] != points.arrayZ[n - 1])
	   points.arrayZ[n++] = points.arrayZ[j];
       points.shrink (n);
       यदि (grid_size > MAX_GRID / points.length) { exact = असत्य; भङ्ग; }
       grid_size *= points.length;
     }

     यदि (exact)
     {
       अचिन्हित target_pos = 0; /* the target axis was added first */
       hb_vector_t<अचिन्हित> odometer;
       यदि (!odometer.resize (grid_pos.get_population ())) प्रत्यागम असत्य;
       तार्किक first = सत्य;
       यावत् (सत्य)
       {
	 द्विगुण delta_sum = 0.0;
	 प्रति (अचिन्हित r = 0; r < active.length; r++)
	 {
	   द्विगुण scalar = 1.0;
	   प्रति (ध्रुव स्वतः &pt : region_tents[r])
	   {
	     द्विगुण v = grid_points[pt.first][odometer[pt.first]];
	     scalar *= _tent_eval (pt.second, v);
	     यदि (scalar == 0.0) भङ्ग;
	   }
	   delta_sum += scalar * active[r].first;
	 }
	 द्विगुण coord = grid_points[target_pos][odometer[target_pos]];
	 द्विगुण value = coord + delta_sum / 16384.0;
	 यदि (first)
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
	 प्रति (; k < odometer.length; k++)
	 {
	   यदि (++odometer[k] < grid_points[k].length) भङ्ग;
	   odometer[k] = 0;
	 }
	 यदि (k == odometer.length) भङ्ग;
       }
     }
     अन्यथा
     {
       प्रति (ध्रुव स्वतः &ar : active)
       {
	 पूर्णाङ्क delta = ar.first;
	 द्विगुण smin = 1.0, smax = 1.0;
	 प्रति (ध्रुव स्वतः &_ : regions[ar.second])
	 {
	   hb_pair_t<द्विगुण, द्विगुण> *निवेश;
	   द्विगुण blo = -1.0, bhi = +1.0;
	   यदि (box.has (_.first, &निवेश)) { blo = निवेश->first; bhi = निवेश->second; }
	   द्विगुण tmin, tmax;
	   tent_मूल्य_range (_.second, blo, bhi, &tmin, &tmax);
	   smin *= tmin;
	   smax *= tmax;
	   यदि (smax == 0.0) भङ्ग;
	 }
	 यदि (delta > 0) { dmin += smin * delta; dmax += smax * delta; }
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
     यदि (detect_self_contained && dmin == dmax &&
	 plan->user_axes_location.has (tag, &user) && user->is_point ())
     {
       पूर्णाङ्क v_int = (पूर्णाङ्क) roundf ((भिन्न) identity->first * 16384.f) +
		   (पूर्णाङ्क) roundf ((भिन्न) dmin);
       v_int = hb_clamp (v_int, -(1 << 14), +(1 << 14));
       यदि (!plan->avar2_self_contained.set (tag, v_int / 16384.0)) प्रत्यागम असत्य;
       अनुवर्त; /* axis is dropped; no region can reference it afterwards */
     }

     /* Pad by one F2Dot14 unit against rounding differences with the
      * runtime's fixed-point evaluation. */
     द्विगुण lo = hb_clamp (vmin - 1.0 / 16384.0, -1.0, +1.0);
     द्विगुण hi = hb_clamp (vmax + 1.0 / 16384.0, -1.0, +1.0);
     lo = न्यूनमान (lo * 16384.0) / 16384.0;
     hi = उच्चमान (hi * 16384.0) / 16384.0;
     यदि (lo <= -1.0 && hi >= +1.0)
       अनुवर्त; /* not constraining */
     यदि (!plan->avar2_reachable_ranges.set (tag, Triple (lo, hb_clamp (0.0, lo, hi), hi)))
       प्रत्यागम असत्य;
   }
   प्रत्यागम सत्य;
 }

#ifndef HB_NO_OT_FONT_CFF
 स्थिर अन्तःस्थ hb_font_t*
 _get_hb_font_with_variations (ध्रुव hb_subset_plan_t *plan)
 {
   hb_font_t *font = hb_font_create (plan->source);

   यदि (plan->has_avar2)
   {
     /* Under avar2, instancing applies only to the self-contained pins,
      * whose constant final coordinates the plan holds in normalized_coords.
      * Setting user-space variations would run the full avar2 mapping and
      * bake contributions that remain live in the instance's variations. */
     hb_font_set_var_coords_normalized (font, plan->normalized_coords.arrayZ,
                                        plan->normalized_coords.length);
     प्रत्यागम font;
   }

   hb_vector_t<hb_variation_t> vars;
   यदि (!vars.alloc (plan->user_axes_location.get_population ())) {
     hb_font_destroy (font);
     प्रत्यागम nullptr;
   }

   प्रति (स्वतः _ : plan->user_axes_location)
   {
     hb_variation_t var;
     var.tag = _.first;
     var.value = _.second.मध्य;
     vars.push (var);
   }

   hb_font_set_variations (font, vars.arrayZ, plan->user_axes_location.get_population ());
   प्रत्यागम font;
 }
#endif

 ढाँचा<typename ItemVarStore>
 शून्य
 remap_variation_indices (ध्रुव ItemVarStore &var_store,
                          ध्रुव hb_set_t &variation_indices,
                          ध्रुव hb_vector_t<पूर्णाङ्क>& normalized_coords,
                          तार्किक calculate_delta, /* not pinned at default */
                          तार्किक no_variations, /* all axes pinned */
                          hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णाङ्क>> &variation_idx_delta_map /* OUT */)
 {
   यदि (&var_store == &Null (OT::ItemVariationStore)) प्रत्यागम;
   अचिन्हित subtable_गिनती = var_store.get_sub_table_गिनती ();
   स्वतः *store_cache = var_store.create_cache ();

   अचिन्हित new_major = 0, new_minor = 0;
   अचिन्हित last_major = (variation_indices.get_min ()) >> 16;
   प्रति (अचिन्हित idx : variation_indices)
   {
     पूर्णाङ्क delta = 0;
     यदि (calculate_delta)
       delta = roundf (var_store.get_delta (idx, normalized_coords.arrayZ,
                                            normalized_coords.length, store_cache));

     यदि (no_variations)
     {
       variation_idx_delta_map.set (idx, hb_pair_t<अचिन्हित, पूर्णाङ्क> (HB_OT_LAYOUT_NO_VARIATIONS_INDEX, delta));
       अनुवर्त;
     }

     uint16_t major = idx >> 16;
     यदि (major >= subtable_गिनती) भङ्ग;
     यदि (major != last_major)
     {
       new_minor = 0;
       ++new_major;
     }

     अचिन्हित new_idx = (new_major << 16) + new_minor;
     variation_idx_delta_map.set (idx, hb_pair_t<अचिन्हित, पूर्णाङ्क> (new_idx, delta));
     ++new_minor;
     last_major = major;
   }
   var_store.destroy_cache (store_cache);
 }

 ढाँचा
 शून्य
 remap_variation_indices<OT::ItemVariationStore> (ध्रुव OT::ItemVariationStore &var_store,
                          ध्रुव hb_set_t &variation_indices,
                          ध्रुव hb_vector_t<पूर्णाङ्क>& normalized_coords,
                          तार्किक calculate_delta, /* not pinned at default */
                          तार्किक no_variations, /* all axes pinned */
                          hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णाङ्क>> &variation_idx_delta_map /* OUT */);

 #ifndef HB_NO_BASE
 शून्य
 collect_base_variation_indices (hb_subset_plan_t* plan)
 {
   hb_blob_ptr_t<OT::BASE> base = plan->source_table<OT::BASE> ();
   यदि (!base->has_var_store ())
   {
     base.destroy ();
     प्रत्यागम;
   }

   hb_set_t varidx_set;
   base->collect_variation_indices (plan, varidx_set);
   ध्रुव OT::ItemVariationStore &var_store = base->get_var_store ();
   अचिन्हित subtable_गिनती = var_store.get_sub_table_गिनती ();


   remap_variation_indices (var_store, varidx_set,
                             plan->normalized_coords,
                             !plan->pinned_at_default,
                             plan->all_axes_pinned,
                             plan->base_variation_idx_map);
   generate_varstore_inner_maps (varidx_set, subtable_गिनती, plan->base_varstore_inner_maps);

   base.destroy ();
 }

 #endif

तार्किक
normalize_axes_location (hb_face_t *face, hb_subset_plan_t *plan)
{
  यदि (plan->user_axes_location.is_empty ())
    प्रत्यागम सत्य;

  hb_array_t<ध्रुव OT::AxisRecord> axes = face->table.fvar->get_axes ();
  प्रति (ध्रुव स्वतः &axis : axes)
  {
    hb_tag_t tag = axis.get_axis_tag ();
    ध्रुव Triple *range;
    भिन्न minimum, मध्य, maximum;
    axis.get_coordinates (minimum, मध्य, maximum);
    यदि (plan->user_axes_location.has (tag, &range) && !range->is_point () &&
        range->minimum == minimum &&
        range->मध्य == मध्य &&
        range->maximum == maximum)
      plan->user_axes_location.del (tag);
  }
  यदि (plan->user_axes_location.is_empty ())
    प्रत्यागम सत्य;

  यदि (!plan->check_success (plan->normalized_coords.resize (axes.length)))
    प्रत्यागम असत्य;

  तार्किक has_avar = face->table.avar->has_data ();
  hb_vector_t<भिन्न> normalized_mins;
  hb_vector_t<भिन्न> normalized_defaults;
  hb_vector_t<भिन्न> normalized_maxs;
  यदि (has_avar)
  {
    यदि (!plan->check_success (normalized_mins.resize (axes.length)) ||
        !plan->check_success (normalized_defaults.resize (axes.length)) ||
        !plan->check_success (normalized_maxs.resize (axes.length)))
      प्रत्यागम असत्य;
  }

  तार्किक axis_not_pinned = असत्य;
  अचिन्हित new_axis_idx = 0;
  अचिन्हित last_idx = 0;
  प्रति (ध्रुव स्वतः& _ : + hb_enumerate (axes))
  {
    अचिन्हित i = _.first;
    ध्रुव OT::AxisRecord &axis = _.second;
    hb_tag_t axis_tag = axis.get_axis_tag ();
    plan->axes_old_index_tag_map.set (i, axis_tag);

    यदि (!plan->user_axes_location.has (axis_tag) ||
        !plan->user_axes_location.get (axis_tag).is_point ())
    {
      axis_not_pinned = सत्य;
      plan->axes_index_map.set (i, new_axis_idx);
      plan->axis_tags.push (axis_tag);
      new_axis_idx++;
    }

    Triple *axis_range;
    यदि (plan->user_axes_location.has (axis_tag, &axis_range))
    {
      plan->axes_triple_distances.set (axis_tag, axis.get_triple_distances ());

      भिन्न normalized_min = axis.normalize_axis_मूल्य (axis_range->minimum);
      भिन्न normalized_default = axis.normalize_axis_मूल्य (axis_range->मध्य);
      भिन्न normalized_max = axis.normalize_axis_मूल्य (axis_range->maximum);

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

      यदि (has_avar)
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
        यदि (normalized_default == -0.f)
          normalized_default = 0.f; // Normalize -0 to 0
        यदि (normalized_default != 0.f)
          plan->pinned_at_default = असत्य;

        plan->normalized_coords[i] = roundf (normalized_default * 16384.f);
      }
    }
  }
  plan->all_axes_pinned = !axis_not_pinned;

  // TODO: use avar map_coords_16_16() when normalization is changed to 16.16
  // in fonttools
  यदि (has_avar)
  {
    ध्रुव OT::avar* avar_table = face->table.avar;
    अचिन्हित coords_len = last_idx + 1;

    यदि (avar_table->has_v2_data () && !plan->all_axes_pinned)
    {
      /* avar2 partial instancing: use v1-only mapping to get intermediate-space
       * coords. These are used for IVS rebasing and offset compensation. */
      plan->has_avar2 = सत्य;

      यदि (!plan->check_success (avar_table->map_coords_2_14 (normalized_mins.arrayZ, coords_len, सत्य)) ||
          !plan->check_success (avar_table->map_coords_2_14 (normalized_defaults.arrayZ, coords_len, सत्य)) ||
          !plan->check_success (avar_table->map_coords_2_14 (normalized_maxs.arrayZ, coords_len, सत्य)))
        प्रत्यागम असत्य;

      प्रति (ध्रुव स्वतः& _ : + hb_enumerate (axes))
      {
        अचिन्हित i = _.first;
        hb_tag_t axis_tag = _.second.get_axis_tag ();
        यदि (plan->user_axes_location.has (axis_tag))
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
      यदि (!_compute_avar2_reachable_ranges (plan, axes, avar_table,
                                            detect_self_contained))
        प्रत्यागम असत्य;

#ifndef HB_NO_VAR_COMPOSITES
      /* VARC overrides can reach coordinates outside the font-level
       * ranges. Do not cull variation data using those ranges. */
      यदि (!plan->drop_tables.has (HB_TAG ('V','A','R','C')))
      {
        hb_blob_t *varc_blob = hb_face_reference_table (face, HB_TAG ('V','A','R','C'));
        यदि (hb_blob_get_length (varc_blob))
          plan->avar2_reachable_ranges.reset ();
        hb_blob_destroy (varc_blob);
      }
#endif

      /* Keep all axes in fvar (pinned ones as hidden), EXCEPT self-contained
       * pinned axes, which are removed entirely. */
      plan->axes_index_map.reset ();
      plan->axis_tags.reset ();
      अचिन्हित retained_axis_idx = 0;
      प्रति (ध्रुव स्वतः& _ : + hb_enumerate (axes))
      {
        अचिन्हित i = _.first;
        hb_tag_t axis_tag = _.second.get_axis_tag ();
        यदि (plan->avar2_self_contained.has (axis_tag))
          अनुवर्त;
        plan->axes_index_map.set (i, retained_axis_idx++);
        plan->axis_tags.push (axis_tag);
      }
      /* The other tables see ONLY the self-contained pins, as an ordinary
       * partial instancing in old final-coordinate space; the standard
       * instancing machinery bakes those axes' contributions in. The
       * remaining restriction is carried entirely by avar. */
      plan->axes_location.reset ();
      यदि (plan->avar2_self_contained.get_population ())
      {
        तार्किक sc_pinned_at_default = सत्य;
        प्रति (स्वतः _ : plan->avar2_self_contained)
        {
          plan->axes_location.set (_.first, Triple (_.second, _.second, _.second));
          यदि (_.second != 0.0)
            sc_pinned_at_default = असत्य;
        }
        प्रति (ध्रुव स्वतः& _ : + hb_enumerate (axes))
        {
          द्विगुण *v;
          plan->normalized_coords[_.first] =
              plan->avar2_self_contained.has (_.second.get_axis_tag (), &v)
              ? (पूर्णाङ्क) roundf ((भिन्न) (*v * 16384.0)) : 0;
        }
        plan->pinned_at_default = sc_pinned_at_default;
      }
      अन्यथा
        plan->normalized_coords.shrink (0);
    }
    अन्यथा
    {
      /* Standard avar v1 (or v1+v2 with all axes pinned) */
      यदि (!plan->check_success (avar_table->map_coords_2_14 (normalized_mins.arrayZ, coords_len)) ||
          !plan->check_success (avar_table->map_coords_2_14 (normalized_defaults.arrayZ, coords_len)) ||
          !plan->check_success (avar_table->map_coords_2_14 (normalized_maxs.arrayZ, coords_len)))
        प्रत्यागम असत्य;

      प्रति (ध्रुव स्वतः& _ : + hb_enumerate (axes))
      {
        अचिन्हित i = _.first;
        hb_tag_t axis_tag = _.second.get_axis_tag ();
        यदि (plan->user_axes_location.has (axis_tag))
        {
          plan->axes_location.set (axis_tag, Triple ((द्विगुण) normalized_mins[i],
                                                     (द्विगुण) normalized_defaults[i],
                                                     (द्विगुण) normalized_maxs[i]));
          भिन्न normalized_default = normalized_defaults[i];
          यदि (normalized_default == -0.f)
            normalized_default = 0.f;
          यदि (normalized_default != 0.f)
            plan->pinned_at_default = असत्य;

          plan->normalized_coords[i] = roundf (normalized_default * 16384.f);
        }
      }
    }
  }
  प्रत्यागम सत्य;
}

#ifndef HB_NO_OT_FONT_CFF
शून्य
update_instance_metrics_map_from_cff2 (hb_subset_plan_t *plan)
{
  यदि (!plan->normalized_coords) प्रत्यागम;
  OT::cff2::accelerator_t cff2 (plan->source);
  यदि (!cff2.is_valid ()) प्रत्यागम;

  hb_font_t *font = _get_hb_font_with_variations (plan);
  यदि (unlikely (!plan->check_success (font != nullptr)))
  {
    hb_font_destroy (font);
    प्रत्यागम;
  }

  hb_glyph_extents_t extents = {0x7FFF, -0x7FFF};
  OT::hmtx_accelerator_t _hmtx (plan->source);
  OT::hb_scalar_cache_t *hvar_store_cache = nullptr;
  यदि (_hmtx.has_data () && _hmtx.var_table.get_length ())
    hvar_store_cache = _hmtx.var_table->get_var_store ().create_cache ();

  OT::vmtx_accelerator_t _vmtx (plan->source);
  OT::hb_scalar_cache_t *vvar_store_cache = nullptr;
  यदि (_vmtx.has_data () && _vmtx.var_table.get_length ())
    vvar_store_cache = _vmtx.var_table->get_var_store ().create_cache ();

  प्रति (स्वतः p : *plan->glyph_map)
  {
    hb_codepoint_t old_gid = p.first;
    hb_codepoint_t new_gid = p.second;
    यदि (!cff2.get_extents (font, old_gid, &extents)) अनुवर्त;
    तार्किक has_bounds_info = सत्य;
    यदि (extents.x_bearing == 0 && extents.width == 0 &&
        extents.height == 0 && extents.y_bearing == 0)
      has_bounds_info = असत्य;

    यदि (has_bounds_info)
    {
      plan->head_maxp_info.xMin = hb_min (plan->head_maxp_info.xMin, extents.x_bearing);
      plan->head_maxp_info.xMax = hb_max (plan->head_maxp_info.xMax,
						 hb_saturate_add (extents.x_bearing, extents.width));
      plan->head_maxp_info.yMax = hb_max (plan->head_maxp_info.yMax, extents.y_bearing);
      plan->head_maxp_info.yMin = hb_min (plan->head_maxp_info.yMin,
						 hb_saturate_add (extents.y_bearing, extents.height));
    }

    यदि (_hmtx.has_data ())
    {
      द्विगुण hori_aw = _hmtx.get_advance_without_var_unscaled (old_gid);
      यदि (_hmtx.var_table.get_length ())
        hori_aw += (द्विगुण) roundf (_hmtx.var_table->get_advance_delta_unscaled (old_gid, font->coords, font->अंक_coords,
									 hvar_store_cache));
      /* Malicious deltas can take the advance out of the UFWORD range that
       * hmtx serialization stores.  Clamp, to keep downstream consumers
       * (e.g. CFF width optimizer) bounded and consistent with hmtx. */
      hori_aw = hb_clamp (hori_aw, 0., 65535.);
      पूर्णाङ्क lsb = extents.x_bearing;
      यदि (!has_bounds_info)
      {
        _hmtx.get_leading_bearing_without_var_unscaled (old_gid, &lsb);
      }
      plan->hmtx_map.set (new_gid, hb_pair ((अचिन्हित) hori_aw, lsb));
      plan->bounds_width_vec[new_gid] = extents.width;
    }

    यदि (_vmtx.has_data ())
    {
      द्विगुण vert_aw = _vmtx.get_advance_without_var_unscaled (old_gid);
      यदि (_vmtx.var_table.get_length ())
        vert_aw += (द्विगुण) roundf (_vmtx.var_table->get_advance_delta_unscaled (old_gid, font->coords, font->अंक_coords,
									 vvar_store_cache));
      vert_aw = hb_clamp (vert_aw, 0., 65535.);
      hb_position_t vorg_x = 0;
      hb_position_t vorg_y = 0;
      पूर्णाङ्क tsb = 0;
      यदि (has_bounds_info &&
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
  यदि (plan->head_maxp_info.xMin > plan->head_maxp_info.xMax)
  {
    plan->head_maxp_info.xMin = plan->head_maxp_info.xMax = 0;
    plan->head_maxp_info.yMin = plan->head_maxp_info.yMax = 0;
  }
  hb_font_destroy (font);
  यदि (hvar_store_cache)
    _hmtx.var_table->get_var_store ().destroy_cache (hvar_store_cache);
  यदि (vvar_store_cache)
    _vmtx.var_table->get_var_store ().destroy_cache (vvar_store_cache);
}
#endif

तार्किक
get_instance_glyphs_contour_points (hb_subset_plan_t *plan)
{
  /* contour_points vector only needed for updating gvar table (infer delta and
   * iup delta optimization) during partial instancing */
  यदि (plan->user_axes_location.is_empty () || plan->all_axes_pinned)
    प्रत्यागम सत्य;

  OT::glyf_accelerator_t glyf (plan->source);

  प्रति (स्वतः &_ : plan->new_to_old_gid_list)
  {
    hb_codepoint_t new_gid = _.first;
    contour_point_vector_t all_points;
    यदि (new_gid == 0 && !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      यदि (unlikely (!plan->new_gid_contour_points_map.set (new_gid, all_points)))
        प्रत्यागम असत्य;
      अनुवर्त;
    }

    hb_codepoint_t old_gid = _.second;
    स्वतः glyph = glyf.glyph_for_gid (old_gid);
    यदि (unlikely (!glyph.get_all_points_without_var (plan->source, all_points)))
      प्रत्यागम असत्य;
    यदि (unlikely (!plan->new_gid_contour_points_map.set (new_gid, all_points)))
      प्रत्यागम असत्य;

    /* composite new gids are only needed by iup delta optimization */
    यदि ((plan->flags & HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS) && glyph.is_composite ())
      plan->composite_new_gids.add (new_gid);
  }
  प्रत्यागम सत्य;
}

ढाँचा<typename DeltaSetIndexMap>
शून्य
remap_colrv1_delta_set_index_indices (ध्रुव DeltaSetIndexMap &index_map,
                                      ध्रुव hb_set_t &delta_set_idxes,
                                      hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णाङ्क>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /* OUT */)
{
  यदि (!index_map.get_map_गिनती ())
    प्रत्यागम;

  hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णाङ्क>> delta_set_idx_delta_map;
  अचिन्हित new_delta_set_idx = 0;
  प्रति (अचिन्हित delta_set_idx : delta_set_idxes)
  {
    अचिन्हित var_idx = index_map.map (delta_set_idx);
    अचिन्हित new_varidx = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    पूर्णाङ्क delta = 0;

    यदि (var_idx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
    {
      hb_pair_t<अचिन्हित, पूर्णाङ्क> *new_varidx_delta;
      यदि (!variation_idx_delta_map.has (var_idx, &new_varidx_delta)) अनुवर्त;

      new_varidx = hb_first (*new_varidx_delta);
      delta = hb_second (*new_varidx_delta);
    }

    new_deltaset_idx_varidx_map.set (new_delta_set_idx, new_varidx);
    delta_set_idx_delta_map.set (delta_set_idx, hb_pair_t<अचिन्हित, पूर्णाङ्क> (new_delta_set_idx, delta));
    new_delta_set_idx++;
  }
  variation_idx_delta_map = std::move (delta_set_idx_delta_map);
}

ढाँचा शून्य
remap_colrv1_delta_set_index_indices<OT::DeltaSetIndexMap> (ध्रुव OT::DeltaSetIndexMap &index_map,
                                      ध्रुव hb_set_t &delta_set_idxes,
                                      hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णाङ्क>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /* OUT */);

 #endif

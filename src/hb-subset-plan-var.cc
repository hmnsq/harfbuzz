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

 #جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR

 فراغ
 generate_varstore_inner_maps (ثابت hb_سيٽ_t& varidx_سيٽ,
                               بنانشان subtable_ڳڻپ,
                               hb_ويڪٽر_t<hb_inc_bimap_t> &inner_maps /*ٻاهريون*/)
 {
   إذا (varidx_سيٽ.is_empty () || subtable_ڳڻپ == 0) أرجع;

   إذا (unlikely (!inner_maps.resize (subtable_ڳڻپ))) أرجع;
   لكل (بنانشان idx : varidx_سيٽ)
   {
     uint16_t major = idx >> 16;
     uint16_t minor = idx & 0xFFFF;

     إذا (major >= subtable_ڳڻپ)
       استمر;
     inner_maps[major].جوڙيو (minor);
   }
 }

 /* Evaluate a variation-region tent at v, mirroring VarRegionAxis::evaluate
  * (invalid regions evaluate as constant 1). */
 ساكن مزدوج
 _tent_eval (ثابت Triple &tent, مزدوج v)
 {
   مزدوج شروعات = (مزدوج) tent.minimum;
   مزدوج peak = (مزدوج) tent.وچ;
   مزدوج پڇاڙي = (مزدوج) tent.وڌ_کان_وڌ;
   إذا (unlikely (شروعات > peak || peak > پڇاڙي)) أرجع 1.0;
   إذا (unlikely (شروعات < 0.0 && پڇاڙي > 0.0 && peak != 0.0)) أرجع 1.0;
   إذا (peak == 0.0 || v == peak) أرجع 1.0;
   إذا (v <= شروعات || پڇاڙي <= v) أرجع 0.0;
   إذا (v < peak) أرجع (v - شروعات) / (peak - شروعات);
   أرجع (پڇاڙي - v) / (پڇاڙي - peak);
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
   إذا (lo <= (مزدوج) tent.وچ && (مزدوج) tent.وچ <= hi)
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
                                  hb_array_t<ثابت OT::Axis_رڪارڊ> axes,
                                  ثابت OT::avar *avar_جدول,
                                  منطقي detect_self_contained)
 {
   ثابت OT::شي_Variation_Store *var_store;
   ثابت OT::Delta_سيٽ_اشاريو_Map *varidx_map;
   إذا (!avar_جدول->get_v2_store_and_map (&var_store, &varidx_map))
     أرجع خطأ;

   /* Old-intermediate input box per axis. */
   hb_hashmap_t<hb_ٽيگ_t, hb_pair_t<مزدوج, مزدوج>> box;
   لكل (ثابت خودڪار &axis : axes)
   {
     hb_ٽيگ_t ٽيگ_2 = axis.get_axis_ٽيگ ();
     مزدوج lo = -1.0, hi = +1.0;
     Triple *old_int;
     إذا (plan->user_axes_location.has (ٽيگ_2))
     {
       إذا (!plan->old_intermediates.has (ٽيگ_2, &old_int)) أرجع خطأ;
       lo = (مزدوج) old_int->minimum;
       hi = (مزدوج) old_int->وڌ_کان_وڌ;
     }
     وإلا إذا (axis.is_hidden ())
       lo = hi = 0.0; /* private axis: intermediate is always 0 */
     إذا (!box.سيٽ (ٽيگ_2, hb_pair (lo, hi))) أرجع خطأ;
   }

   hb_ويڪٽر_t<hb_hashmap_t<hb_ٽيگ_t, Triple>> regions;
   إذا (!var_store->get_region_list ().get_var_regions (plan->axes_old_اشاريو_ٽيگ_map, regions))
     أرجع خطأ;

   hb_hashmap_t<hb_ٽيگ_t, بنانشان> grid_pos;
   hb_ويڪٽر_t<hb_ويڪٽر_t<مزدوج>> grid_points;
   لكل (بنانشان i = 0; i < axes.ڊيگهائي; i++)
   {
     grid_pos.reset ();
     grid_points.reset ();

     hb_ٽيگ_t ٽيگ_2 = axes[i].get_axis_ٽيگ ();
     hb_pair_t<مزدوج, مزدوج> *سڃاڻپ_عنصر;
     إذا (!box.has (ٽيگ_2, &سڃاڻپ_عنصر)) أرجع خطأ;

     /* Collect the row's active regions (non-zero delta). A varidx (or the
      * implicit identity mapping) that doesn't resolve to a real store row
      * behaves as "no variation" at runtime, so it contributes no regions;
      * this also covers a NULL VarStore. */
     hb_ويڪٽر_t<hb_pair_t<صحيح, بنانشان>> سرگرم; /* (delta, region index) */
     uint32_t varidx = varidx_map->map (i);
     /* Runtime treats out-of-range delta-set indices as zero. */
     إذا (varidx != HB_OT_LAYOUT_NO_VARIATIONS_اشاريو &&
	 !var_store->has_delta_سيٽ (varidx))
       varidx = HB_OT_LAYOUT_NO_VARIATIONS_اشاريو;
     إذا (varidx != HB_OT_LAYOUT_NO_VARIATIONS_اشاريو)
     {
       بنانشان outer = varidx >> 16;
       بنانشان inner = varidx & 0xFFFF;
       ثابت OT::Var_معلومات &vd = var_store->get_sub_جدول (outer);
       ثابت OT::HBUINT8 *delta_بائيٽون = vd.get_delta_بائيٽون ();
       بنانشان صف_سائيز = vd.get_صف_سائيز ();
       بنانشان region_ڳڻپ_2 = vd.get_region_اشاريو_ڳڻپ ();
       لكل (بنانشان r = 0; r < region_ڳڻپ_2; r++)
       {
	 صحيح delta = vd.get_شي_delta_fast (inner, r, delta_بائيٽون, صف_سائيز);
	 إذا (!delta) استمر;
	 بنانشان region_idx = vd.get_region_اشاريو (r);
	 إذا (region_idx >= regions.ڊيگهائي) أرجع خطأ;
	 سرگرم.push (hb_pair (delta, region_idx));
       }
       إذا (سرگرم.اندريون_error ()) أرجع خطأ;
     }

     /* The delta is piecewise multilinear in the axis coordinates (each
      * region scalar is a product of independent per-axis tents), so over
      * the box its exact extremes are attained at vertices of the grid of
      * per-axis tent breakpoints. Enumerate that grid when it is small
      * (real fonts); otherwise fall back to per-region interval
      * arithmetic, which ignores correlations between regions and is
      * looser but still conservative. Mirrors fontTools'
      * VarStore.getExtremes. */
     constexpr بنانشان MAX_GRID = 1u << 14;
     منطقي exact = سچ;
     مزدوج dmin = 0.0, dmax = 0.0; /* delta extremes, F2Dot14 units */
     مزدوج vmin = 0.0, vmax = 0.0; /* identity + delta extremes, coord units */

     /* Grid axes: the target axis (identity term), plus every axis a valid
      * tent of an active region references. */
     خودڪار جوڙيو_grid_axis = [&] (hb_ٽيگ_t t) -> صحيح
     {
       بنانشان *pos;
       إذا (grid_pos.has (t, &pos)) أرجع (صحيح) *pos;
       بنانشان new_pos = grid_pos.get_population ();
       hb_pair_t<مزدوج, مزدوج> *داخلا_2;
       مزدوج blo = -1.0, bhi = +1.0;
       إذا (box.has (t, &داخلا_2)) { blo = داخلا_2->پهريون; bhi = داخلا_2->second; }
       grid_points.push (hb_ويڪٽر_t<مزدوج> ());
       إذا (grid_points.اندريون_error ()) أرجع -1;
       خودڪار &points = grid_points[new_pos];
       points.push (blo);
       points.push (bhi);
       إذا (points.اندريون_error () || !grid_pos.سيٽ (t, new_pos)) أرجع -1;
       أرجع (صحيح) new_pos;
     };

     إذا (جوڙيو_grid_axis (ٽيگ_2) < 0) أرجع خطأ;
     /* Per active region: (grid axis position, tent) for its valid tents. */
     hb_ويڪٽر_t<hb_ويڪٽر_t<hb_pair_t<بنانشان, Triple>>> region_tents;
     لكل (ثابت خودڪار &ar : سرگرم)
     {
       region_tents.push (hb_ويڪٽر_t<hb_pair_t<بنانشان, Triple>> ());
       إذا (region_tents.اندريون_error ()) أرجع خطأ;
       خودڪار &tents = region_tents[region_tents.ڊيگهائي - 1];
       لكل (ثابت خودڪار &_ : regions[ar.second])
       {
	 ثابت Triple &tent = _.second;
	 /* Tents the runtime ignores (constant scalar 1) add no breakpoints. */
	 إذا (tent.وچ == 0.f ||
	     tent.minimum > tent.وچ || tent.وچ > tent.وڌ_کان_وڌ ||
	     (tent.minimum < 0.f && tent.وڌ_کان_وڌ > 0.f))
	   استمر;
	 صحيح pos = جوڙيو_grid_axis (_.پهريون);
	 إذا (pos < 0) أرجع خطأ;
	 خودڪار &points = grid_points[pos];
	 مزدوج blo = points[0], bhi = points[1];
	 points.push (hb_clamp ((مزدوج) tent.minimum, blo, bhi));
	 points.push (hb_clamp ((مزدوج) tent.وچ, blo, bhi));
	 points.push (hb_clamp ((مزدوج) tent.وڌ_کان_وڌ, blo, bhi));
	 tents.push (hb_pair ((بنانشان) pos, tent));
	 إذا (points.اندريون_error () || tents.اندريون_error ()) أرجع خطأ;
       }
     }

     بنانشان grid_سائيز = 1;
     لكل (خودڪار &points : grid_points)
     {
       points.qsort ([] (ثابت مزدوج &a, ثابت مزدوج &b) -> صحيح
		     { أرجع a < b ? -1 : a > b ? +1 : 0; });
       بنانشان n = 0;
       لكل (بنانشان j = 0; j < points.ڊيگهائي; j++)
	 إذا (!n || points.arrayZ[j] != points.arrayZ[n - 1])
	   points.arrayZ[n++] = points.arrayZ[j];
       points.shrink (n);
       إذا (grid_سائيز > MAX_GRID / points.ڊيگهائي) { exact = خطأ; اكسر; }
       grid_سائيز *= points.ڊيگهائي;
     }

     إذا (exact)
     {
       بنانشان هدف_pos = 0; /* the target axis was added first */
       hb_ويڪٽر_t<بنانشان> odometer;
       إذا (!odometer.resize (grid_pos.get_population ())) أرجع خطأ;
       منطقي پهريون = سچ;
       طالما (سچ)
       {
	 مزدوج delta_جوڙ = 0.0;
	 لكل (بنانشان r = 0; r < سرگرم.ڊيگهائي; r++)
	 {
	   مزدوج scalar = 1.0;
	   لكل (ثابت خودڪار &pt : region_tents[r])
	   {
	     مزدوج v = grid_points[pt.پهريون][odometer[pt.پهريون]];
	     scalar *= _tent_eval (pt.second, v);
	     إذا (scalar == 0.0) اكسر;
	   }
	   delta_جوڙ += scalar * سرگرم[r].پهريون;
	 }
	 مزدوج coord = grid_points[هدف_pos][odometer[هدف_pos]];
	 مزدوج value = coord + delta_جوڙ / 16384.0;
	 إذا (پهريون)
	 {
	   dmin = dmax = delta_جوڙ;
	   vmin = vmax = value;
	   پهريون = خطأ;
	 }
	 وإلا
	 {
	   dmin = hb_min (dmin, delta_جوڙ); dmax = hb_max (dmax, delta_جوڙ);
	   vmin = hb_min (vmin, value);     vmax = hb_max (vmax, value);
	 }
	 بنانشان k = 0;
	 لكل (; k < odometer.ڊيگهائي; k++)
	 {
	   إذا (++odometer[k] < grid_points[k].ڊيگهائي) اكسر;
	   odometer[k] = 0;
	 }
	 إذا (k == odometer.ڊيگهائي) اكسر;
       }
     }
     وإلا
     {
       لكل (ثابت خودڪار &ar : سرگرم)
       {
	 صحيح delta = ar.پهريون;
	 مزدوج smin = 1.0, smax = 1.0;
	 لكل (ثابت خودڪار &_ : regions[ar.second])
	 {
	   hb_pair_t<مزدوج, مزدوج> *داخلا_2;
	   مزدوج blo = -1.0, bhi = +1.0;
	   إذا (box.has (_.پهريون, &داخلا_2)) { blo = داخلا_2->پهريون; bhi = داخلا_2->second; }
	   مزدوج tmin, tmax;
	   tent_قدر_range (_.second, blo, bhi, &tmin, &tmax);
	   smin *= tmin;
	   smax *= tmax;
	   إذا (smax == 0.0) اكسر;
	 }
	 إذا (delta > 0) { dmin += smin * delta; dmax += smax * delta; }
	 وإلا           { dmin += smax * delta; dmax += smin * delta; }
       }
       vmin = سڃاڻپ_عنصر->پهريون + dmin / 16384.0;
       vmax = سڃاڻپ_عنصر->second + dmax / 16384.0;
     }
     /* A pinned axis whose delta is constant over the box is self-contained:
      * its final coordinate is a constant. It can be removed from fvar/avar
      * and its contribution baked into the variation tables like an ordinary
      * pin at that coordinate (in old final space). Round the delta alone
      * and add it to the exact quantized intermediate, matching the runtime
      * (map_coords_2_14) and fontTools. */
     Triple *user;
     إذا (detect_self_contained && dmin == dmax &&
	 plan->user_axes_location.has (ٽيگ_2, &user) && user->is_نقطو ())
     {
       صحيح v_int = (صحيح) roundf ((عائم) سڃاڻپ_عنصر->پهريون * 16384.f) +
		   (صحيح) roundf ((عائم) dmin);
       v_int = hb_clamp (v_int, -(1 << 14), +(1 << 14));
       إذا (!plan->avar2_self_contained.سيٽ (ٽيگ_2, v_int / 16384.0)) أرجع خطأ;
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
     إذا (!plan->avar2_reachable_ranges.سيٽ (ٽيگ_2, Triple (lo, hb_clamp (0.0, lo, hi), hi)))
       أرجع خطأ;
   }
   أرجع سچ;
 }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_FONT_CFF
 ساكن مضمن hb_font_t*
 _get_hb_font_with_variations (ثابت hb_subset_plan_t *plan)
 {
   hb_font_t *font = hb_font_create (plan->ذريعو);

   إذا (plan->has_avar2)
   {
     /* Under avar2, instancing applies only to the self-contained pins,
      * whose constant final coordinates the plan holds in normalized_coords.
      * Setting user-space variations would run the full avar2 mapping and
      * bake contributions that remain live in the instance's variations. */
     hb_font_سيٽ_var_coords_normalized (font, plan->normalized_coords.arrayZ,
                                        plan->normalized_coords.ڊيگهائي);
     أرجع font;
   }

   hb_ويڪٽر_t<hb_variation_t> vars;
   إذا (!vars.alloc (plan->user_axes_location.get_population ())) {
     hb_font_destroy (font);
     أرجع خالي_اشارو;
   }

   لكل (خودڪار _ : plan->user_axes_location)
   {
     hb_variation_t var;
     var.ٽيگ_2 = _.پهريون;
     var.value = _.second.وچ;
     vars.push (var);
   }

   hb_font_سيٽ_variations (font, vars.arrayZ, plan->user_axes_location.get_population ());
   أرجع font;
 }
#جيڪڏهن_پورو

 سانچو<قسم_نالو شي_Var_Store>
 فراغ
 remap_variation_indices (ثابت شي_Var_Store &var_store,
                          ثابت hb_سيٽ_t &variation_indices,
                          ثابت hb_ويڪٽر_t<صحيح>& normalized_coords,
                          منطقي calculate_delta, /* not pinned at default */
                          منطقي no_variations, /* all axes pinned */
                          hb_hashmap_t<بنانشان, hb_pair_t<بنانشان, صحيح>> &variation_idx_delta_map /*ٻاهريون*/)
 {
   إذا (&var_store == &Null (OT::شي_Variation_Store)) أرجع;
   بنانشان subtable_ڳڻپ = var_store.get_sub_جدول_ڳڻپ ();
   خودڪار *store_cache = var_store.create_cache ();

   بنانشان new_major = 0, new_minor = 0;
   بنانشان آخري_major = (variation_indices.get_min ()) >> 16;
   لكل (بنانشان idx : variation_indices)
   {
     صحيح delta = 0;
     إذا (calculate_delta)
       delta = roundf (var_store.get_delta (idx, normalized_coords.arrayZ,
                                            normalized_coords.ڊيگهائي, store_cache));

     إذا (no_variations)
     {
       variation_idx_delta_map.سيٽ (idx, hb_pair_t<بنانشان, صحيح> (HB_OT_LAYOUT_NO_VARIATIONS_اشاريو, delta));
       استمر;
     }

     uint16_t major = idx >> 16;
     إذا (major >= subtable_ڳڻپ) اكسر;
     إذا (major != آخري_major)
     {
       new_minor = 0;
       ++new_major;
     }

     بنانشان new_idx = (new_major << 16) + new_minor;
     variation_idx_delta_map.سيٽ (idx, hb_pair_t<بنانشان, صحيح> (new_idx, delta));
     ++new_minor;
     آخري_major = major;
   }
   var_store.destroy_cache (store_cache);
 }

 سانچو
 فراغ
 remap_variation_indices<OT::شي_Variation_Store> (ثابت OT::شي_Variation_Store &var_store,
                          ثابت hb_سيٽ_t &variation_indices,
                          ثابت hb_ويڪٽر_t<صحيح>& normalized_coords,
                          منطقي calculate_delta, /* not pinned at default */
                          منطقي no_variations, /* all axes pinned */
                          hb_hashmap_t<بنانشان, hb_pair_t<بنانشان, صحيح>> &variation_idx_delta_map /*ٻاهريون*/);

 #جيڪڏهن_وصف_نه_ٿيل HB_NO_بنياد
 فراغ
 collect_بنياد_variation_indices (hb_subset_plan_t* plan)
 {
   hb_blob_ptr_t<OT::بنياد_4> بنياد = plan->ذريعو_جدول<OT::بنياد_4> ();
   إذا (!بنياد->has_var_store ())
   {
     بنياد.destroy ();
     أرجع;
   }

   hb_سيٽ_t varidx_سيٽ;
   بنياد->collect_variation_indices (plan, varidx_سيٽ);
   ثابت OT::شي_Variation_Store &var_store = بنياد->get_var_store ();
   بنانشان subtable_ڳڻپ = var_store.get_sub_جدول_ڳڻپ ();


   remap_variation_indices (var_store, varidx_سيٽ,
                             plan->normalized_coords,
                             !plan->pinned_at_default,
                             plan->all_axes_pinned,
                             plan->بنياد_variation_idx_map);
   generate_varstore_inner_maps (varidx_سيٽ, subtable_ڳڻپ, plan->بنياد_varstore_inner_maps);

   بنياد.destroy ();
 }

 #جيڪڏهن_پورو

منطقي
normalize_axes_location (hb_face_t *face, hb_subset_plan_t *plan)
{
  إذا (plan->user_axes_location.is_empty ())
    أرجع سچ;

  hb_array_t<ثابت OT::Axis_رڪارڊ> axes = face->جدول.fvar->get_axes ();
  لكل (ثابت خودڪار &axis : axes)
  {
    hb_ٽيگ_t ٽيگ_2 = axis.get_axis_ٽيگ ();
    ثابت Triple *range;
    عائم minimum, وچ, وڌ_کان_وڌ;
    axis.get_coordinates (minimum, وچ, وڌ_کان_وڌ);
    إذا (plan->user_axes_location.has (ٽيگ_2, &range) && !range->is_نقطو () &&
        range->minimum == minimum &&
        range->وچ == وچ &&
        range->وڌ_کان_وڌ == وڌ_کان_وڌ)
      plan->user_axes_location.del (ٽيگ_2);
  }
  إذا (plan->user_axes_location.is_empty ())
    أرجع سچ;

  إذا (!plan->check_success (plan->normalized_coords.resize (axes.ڊيگهائي)))
    أرجع خطأ;

  منطقي has_avar = face->جدول.avar->has_معلومات ();
  hb_ويڪٽر_t<عائم> normalized_mins;
  hb_ويڪٽر_t<عائم> normalized_defaults;
  hb_ويڪٽر_t<عائم> normalized_maxs;
  إذا (has_avar)
  {
    إذا (!plan->check_success (normalized_mins.resize (axes.ڊيگهائي)) ||
        !plan->check_success (normalized_defaults.resize (axes.ڊيگهائي)) ||
        !plan->check_success (normalized_maxs.resize (axes.ڊيگهائي)))
      أرجع خطأ;
  }

  منطقي axis_not_pinned = خطأ;
  بنانشان new_axis_idx = 0;
  بنانشان آخري_idx = 0;
  لكل (ثابت خودڪار& _ : + hb_enumerate (axes))
  {
    بنانشان i = _.پهريون;
    ثابت OT::Axis_رڪارڊ &axis = _.second;
    hb_ٽيگ_t axis_ٽيگ_2 = axis.get_axis_ٽيگ ();
    plan->axes_old_اشاريو_ٽيگ_map.سيٽ (i, axis_ٽيگ_2);

    إذا (!plan->user_axes_location.has (axis_ٽيگ_2) ||
        !plan->user_axes_location.get (axis_ٽيگ_2).is_نقطو ())
    {
      axis_not_pinned = سچ;
      plan->axes_اشاريو_map.سيٽ (i, new_axis_idx);
      plan->axis_ٽيگ_فهرست.push (axis_ٽيگ_2);
      new_axis_idx++;
    }

    Triple *axis_range;
    إذا (plan->user_axes_location.has (axis_ٽيگ_2, &axis_range))
    {
      plan->axes_triple_distances.سيٽ (axis_ٽيگ_2, axis.get_triple_distances ());

      عائم normalized_min = axis.normalize_axis_قدر (axis_range->minimum);
      عائم normalized_default = axis.normalize_axis_قدر (axis_range->وچ);
      عائم normalized_max = axis.normalize_axis_قدر (axis_range->وڌ_کان_وڌ);

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
        آخري_idx = i;
      }
      وإلا
      {
        plan->axes_location.سيٽ (axis_ٽيگ_2, Triple ((مزدوج) normalized_min,
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
    ثابت OT::avar* avar_جدول = face->جدول.avar;
    بنانشان coords_len = آخري_idx + 1;

    إذا (avar_جدول->has_v2_معلومات () && !plan->all_axes_pinned)
    {
      /* avar2 partial instancing: use v1-only mapping to get intermediate-space
       * coords. These are used for IVS rebasing and offset compensation. */
      plan->has_avar2 = سچ;

      إذا (!plan->check_success (avar_جدول->map_coords_2_14 (normalized_mins.arrayZ, coords_len, سچ)) ||
          !plan->check_success (avar_جدول->map_coords_2_14 (normalized_defaults.arrayZ, coords_len, سچ)) ||
          !plan->check_success (avar_جدول->map_coords_2_14 (normalized_maxs.arrayZ, coords_len, سچ)))
        أرجع خطأ;

      لكل (ثابت خودڪار& _ : + hb_enumerate (axes))
      {
        بنانشان i = _.پهريون;
        hb_ٽيگ_t axis_ٽيگ_2 = _.second.get_axis_ٽيگ ();
        إذا (plan->user_axes_location.has (axis_ٽيگ_2))
        {
          /* Store intermediate-space coords for offset compensation,
           * segment-map renormalization, and IVS rebasing. */
          plan->old_intermediates.سيٽ (axis_ٽيگ_2, Triple ((مزدوج) normalized_mins[i],
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
      hb_blob_t *cff2_blob = hb_face_reference_جدول (face, HB_ٽيگ ('C','F','F','2'));
      منطقي detect_self_contained = !hb_blob_get_ڊيگهائي (cff2_blob);
      hb_blob_destroy (cff2_blob);
      إذا (!_compute_avar2_reachable_ranges (plan, axes, avar_جدول,
                                            detect_self_contained))
        أرجع خطأ;

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR_COMPOSITES
      /* VARC overrides can reach coordinates outside the font-level
       * ranges. Do not cull variation data using those ranges. */
      إذا (!plan->drop_tables.has (HB_ٽيگ ('V','A','R','C')))
      {
        hb_blob_t *varc_blob = hb_face_reference_جدول (face, HB_ٽيگ ('V','A','R','C'));
        إذا (hb_blob_get_ڊيگهائي (varc_blob))
          plan->avar2_reachable_ranges.reset ();
        hb_blob_destroy (varc_blob);
      }
#جيڪڏهن_پورو

      /* Keep all axes in fvar (pinned ones as hidden), EXCEPT self-contained
       * pinned axes, which are removed entirely. */
      plan->axes_اشاريو_map.reset ();
      plan->axis_ٽيگ_فهرست.reset ();
      بنانشان retained_axis_idx = 0;
      لكل (ثابت خودڪار& _ : + hb_enumerate (axes))
      {
        بنانشان i = _.پهريون;
        hb_ٽيگ_t axis_ٽيگ_2 = _.second.get_axis_ٽيگ ();
        إذا (plan->avar2_self_contained.has (axis_ٽيگ_2))
          استمر;
        plan->axes_اشاريو_map.سيٽ (i, retained_axis_idx++);
        plan->axis_ٽيگ_فهرست.push (axis_ٽيگ_2);
      }
      /* The other tables see ONLY the self-contained pins, as an ordinary
       * partial instancing in old final-coordinate space; the standard
       * instancing machinery bakes those axes' contributions in. The
       * remaining restriction is carried entirely by avar. */
      plan->axes_location.reset ();
      إذا (plan->avar2_self_contained.get_population ())
      {
        منطقي sc_pinned_at_default = سچ;
        لكل (خودڪار _ : plan->avar2_self_contained)
        {
          plan->axes_location.سيٽ (_.پهريون, Triple (_.second, _.second, _.second));
          إذا (_.second != 0.0)
            sc_pinned_at_default = خطأ;
        }
        لكل (ثابت خودڪار& _ : + hb_enumerate (axes))
        {
          مزدوج *v;
          plan->normalized_coords[_.پهريون] =
              plan->avar2_self_contained.has (_.second.get_axis_ٽيگ (), &v)
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
      إذا (!plan->check_success (avar_جدول->map_coords_2_14 (normalized_mins.arrayZ, coords_len)) ||
          !plan->check_success (avar_جدول->map_coords_2_14 (normalized_defaults.arrayZ, coords_len)) ||
          !plan->check_success (avar_جدول->map_coords_2_14 (normalized_maxs.arrayZ, coords_len)))
        أرجع خطأ;

      لكل (ثابت خودڪار& _ : + hb_enumerate (axes))
      {
        بنانشان i = _.پهريون;
        hb_ٽيگ_t axis_ٽيگ_2 = _.second.get_axis_ٽيگ ();
        إذا (plan->user_axes_location.has (axis_ٽيگ_2))
        {
          plan->axes_location.سيٽ (axis_ٽيگ_2, Triple ((مزدوج) normalized_mins[i],
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
  أرجع سچ;
}

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_FONT_CFF
فراغ
update_instance_metrics_map_from_cff2 (hb_subset_plan_t *plan)
{
  إذا (!plan->normalized_coords) أرجع;
  OT::cff2::accelerator_t cff2 (plan->ذريعو);
  إذا (!cff2.is_valid ()) أرجع;

  hb_font_t *font = _get_hb_font_with_variations (plan);
  إذا (unlikely (!plan->check_success (font != خالي_اشارو)))
  {
    hb_font_destroy (font);
    أرجع;
  }

  hb_glyph_extents_t extents = {0x7FFF, -0x7FFF};
  OT::hmtx_accelerator_t _hmtx (plan->ذريعو);
  OT::hb_scalar_cache_t *hvar_store_cache = خالي_اشارو;
  إذا (_hmtx.has_معلومات () && _hmtx.var_جدول.get_ڊيگهائي ())
    hvar_store_cache = _hmtx.var_جدول->get_var_store ().create_cache ();

  OT::vmtx_accelerator_t _vmtx (plan->ذريعو);
  OT::hb_scalar_cache_t *vvar_store_cache = خالي_اشارو;
  إذا (_vmtx.has_معلومات () && _vmtx.var_جدول.get_ڊيگهائي ())
    vvar_store_cache = _vmtx.var_جدول->get_var_store ().create_cache ();

  لكل (خودڪار p : *plan->glyph_map)
  {
    hb_codepoint_t old_gid = p.پهريون;
    hb_codepoint_t new_gid = p.second;
    إذا (!cff2.get_extents (font, old_gid, &extents)) استمر;
    منطقي has_bounds_info = سچ;
    إذا (extents.x_bearing == 0 && extents.ويڪر == 0 &&
        extents.اوچائي == 0 && extents.y_bearing == 0)
      has_bounds_info = خطأ;

    إذا (has_bounds_info)
    {
      plan->سر_maxp_info.xMin = hb_min (plan->سر_maxp_info.xMin, extents.x_bearing);
      plan->سر_maxp_info.xMax = hb_max (plan->سر_maxp_info.xMax,
						 hb_saturate_جوڙيو (extents.x_bearing, extents.ويڪر));
      plan->سر_maxp_info.yMax = hb_max (plan->سر_maxp_info.yMax, extents.y_bearing);
      plan->سر_maxp_info.yMin = hb_min (plan->سر_maxp_info.yMin,
						 hb_saturate_جوڙيو (extents.y_bearing, extents.اوچائي));
    }

    إذا (_hmtx.has_معلومات ())
    {
      مزدوج hori_aw = _hmtx.get_advance_without_var_unscaled (old_gid);
      إذا (_hmtx.var_جدول.get_ڊيگهائي ())
        hori_aw += (مزدوج) roundf (_hmtx.var_جدول->get_advance_delta_unscaled (old_gid, font->coords, font->انگ_coords,
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
      plan->hmtx_map.سيٽ (new_gid, hb_pair ((بنانشان) hori_aw, lsb));
      plan->bounds_ويڪر_vec[new_gid] = extents.ويڪر;
    }

    إذا (_vmtx.has_معلومات ())
    {
      مزدوج vert_aw = _vmtx.get_advance_without_var_unscaled (old_gid);
      إذا (_vmtx.var_جدول.get_ڊيگهائي ())
        vert_aw += (مزدوج) roundf (_vmtx.var_جدول->get_advance_delta_unscaled (old_gid, font->coords, font->انگ_coords,
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

      plan->vmtx_map.سيٽ (new_gid, hb_pair ((بنانشان) vert_aw, tsb));
      plan->bounds_اوچائي_vec[new_gid] = extents.اوچائي;
    }
  }
  /* No glyph contributed bounds: do not serialize the accumulator's
   * inverted sentinels into head (or a downgraded CFF FontBBox). */
  إذا (plan->سر_maxp_info.xMin > plan->سر_maxp_info.xMax)
  {
    plan->سر_maxp_info.xMin = plan->سر_maxp_info.xMax = 0;
    plan->سر_maxp_info.yMin = plan->سر_maxp_info.yMax = 0;
  }
  hb_font_destroy (font);
  إذا (hvar_store_cache)
    _hmtx.var_جدول->get_var_store ().destroy_cache (hvar_store_cache);
  إذا (vvar_store_cache)
    _vmtx.var_جدول->get_var_store ().destroy_cache (vvar_store_cache);
}
#جيڪڏهن_پورو

منطقي
get_instance_glyphs_contour_points (hb_subset_plan_t *plan)
{
  /* contour_points vector only needed for updating gvar table (infer delta and
   * iup delta optimization) during partial instancing */
  إذا (plan->user_axes_location.is_empty () || plan->all_axes_pinned)
    أرجع سچ;

  OT::glyf_accelerator_t glyf (plan->ذريعو);

  لكل (خودڪار &_ : plan->new_منزل_old_gid_list)
  {
    hb_codepoint_t new_gid = _.پهريون;
    contour_نقطو_ويڪٽر_t all_points;
    إذا (new_gid == 0 && !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      إذا (unlikely (!plan->new_gid_contour_points_map.سيٽ (new_gid, all_points)))
        أرجع خطأ;
      استمر;
    }

    hb_codepoint_t old_gid = _.second;
    خودڪار glyph = glyf.glyph_for_gid (old_gid);
    إذا (unlikely (!glyph.get_all_points_without_var (plan->ذريعو, all_points)))
      أرجع خطأ;
    إذا (unlikely (!plan->new_gid_contour_points_map.سيٽ (new_gid, all_points)))
      أرجع خطأ;

    /* composite new gids are only needed by iup delta optimization */
    إذا ((plan->flags & HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS) && glyph.is_مرکب ())
      plan->مرکب_new_gids.جوڙيو (new_gid);
  }
  أرجع سچ;
}

سانچو<قسم_نالو Delta_سيٽ_اشاريو_Map>
فراغ
remap_colrv1_delta_سيٽ_اشاريو_indices (ثابت Delta_سيٽ_اشاريو_Map &اشاريو_map,
                                      ثابت hb_سيٽ_t &delta_سيٽ_idxes,
                                      hb_hashmap_t<بنانشان, hb_pair_t<بنانشان, صحيح>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /*ٻاهريون*/)
{
  إذا (!اشاريو_map.get_map_ڳڻپ ())
    أرجع;

  hb_hashmap_t<بنانشان, hb_pair_t<بنانشان, صحيح>> delta_سيٽ_idx_delta_map;
  بنانشان new_delta_سيٽ_idx = 0;
  لكل (بنانشان delta_سيٽ_idx : delta_سيٽ_idxes)
  {
    بنانشان var_idx = اشاريو_map.map (delta_سيٽ_idx);
    بنانشان new_varidx = HB_OT_LAYOUT_NO_VARIATIONS_اشاريو;
    صحيح delta = 0;

    إذا (var_idx != HB_OT_LAYOUT_NO_VARIATIONS_اشاريو)
    {
      hb_pair_t<بنانشان, صحيح> *new_varidx_delta;
      إذا (!variation_idx_delta_map.has (var_idx, &new_varidx_delta)) استمر;

      new_varidx = hb_پهريون (*new_varidx_delta);
      delta = hb_ٻيو (*new_varidx_delta);
    }

    new_deltaset_idx_varidx_map.سيٽ (new_delta_سيٽ_idx, new_varidx);
    delta_سيٽ_idx_delta_map.سيٽ (delta_سيٽ_idx, hb_pair_t<بنانشان, صحيح> (new_delta_سيٽ_idx, delta));
    new_delta_سيٽ_idx++;
  }
  variation_idx_delta_map = std::move (delta_سيٽ_idx_delta_map);
}

سانچو فراغ
remap_colrv1_delta_سيٽ_اشاريو_indices<OT::Delta_سيٽ_اشاريو_Map> (ثابت OT::Delta_سيٽ_اشاريو_Map &اشاريو_map,
                                      ثابت hb_سيٽ_t &delta_سيٽ_idxes,
                                      hb_hashmap_t<بنانشان, hb_pair_t<بنانشان, صحيح>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /*ٻاهريون*/);

 #جيڪڏهن_پورو

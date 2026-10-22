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

 #شامل "hb-ot-layout-common.hh"
#شامل "hb-subset-plan.hh"

 #شامل "hb-ot-var-common.hh"
 #شامل "hb-ot-layout-base-table.hh"
 #شامل "hb-ot-glyf-table.hh"
 #شامل "hb-ot-var-fvar-table.hh"
 #شامل "hb-ot-var-avar-table.hh"
 #شامل "hb-ot-cff2-table.hh"

 #اگر_غیر_تعریف_شدہ HB_NO_VAR

 خالی
 generate_varstore_inner_maps (ثابت hb_سیٹ_t& varidx_سیٹ,
                               غیرعلامتی subtable_گنتی,
                               hb_سمتیہ_t<hb_inc_bimap_t> &inner_maps /*برآمد*/)
 {
   اگر (varidx_سیٹ.is_empty () || subtable_گنتی == 0) واپس;

   اگر (unlikely (!inner_maps.resize (subtable_گنتی))) واپس;
   برائے (غیرعلامتی idx : varidx_سیٹ)
   {
     uint16_t major = idx >> 16;
     uint16_t minor = idx & 0xFFFF;

     اگر (major >= subtable_گنتی)
       جاری;
     inner_maps[major].جوڑو (minor);
   }
 }

 /* Evaluate a variation-region tent at v, mirroring VarRegionAxis::evaluate
  * (invalid regions evaluate as constant 1). */
 ساکن دوہرا
 _tent_eval (ثابت Triple &tent, دوہرا v)
 {
   دوہرا آغاز = (دوہرا) tent.minimum;
   دوہرا peak = (دوہرا) tent.درمیان;
   دوہرا انتہا = (دوہرا) tent.زیادہ_سے_زیادہ;
   اگر (unlikely (آغاز > peak || peak > انتہا)) واپس 1.0;
   اگر (unlikely (آغاز < 0.0 && انتہا > 0.0 && peak != 0.0)) واپس 1.0;
   اگر (peak == 0.0 || v == peak) واپس 1.0;
   اگر (v <= آغاز || انتہا <= v) واپس 0.0;
   اگر (v < peak) واپس (v - آغاز) / (peak - آغاز);
   واپس (انتہا - v) / (انتہا - peak);
 }

 /* Range of a tent's scalar over an input interval [lo, hi]. The tent is
  * unimodal, so the extremes are at the interval endpoints, plus the peak
  * when it falls inside. */
 ساکن خالی
 tent_قدر_range (ثابت Triple &tent, دوہرا lo, دوہرا hi,
                    دوہرا *tmin, دوہرا *tmax)
 {
   دوہرا a = _tent_eval (tent, lo);
   دوہرا b = _tent_eval (tent, hi);
   *tmin = hb_min (a, b);
   *tmax = hb_max (a, b);
   اگر (lo <= (دوہرا) tent.درمیان && (دوہرا) tent.درمیان <= hi)
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
 ساکن منطقی
 _compute_avar2_reachable_ranges (hb_subset_plan_t *plan,
                                  hb_array_t<ثابت OT::Axis_ریکارڈ> axes,
                                  ثابت OT::avar *avar_جدول,
                                  منطقی detect_self_contained)
 {
   ثابت OT::شے_Variation_Store *var_store;
   ثابت OT::Delta_سیٹ_اشاریہ_Map *varidx_map;
   اگر (!avar_جدول->get_v2_store_and_map (&var_store, &varidx_map))
     واپس جھوٹ;

   /* Old-intermediate input box per axis. */
   hb_hashmap_t<hb_لیبل_t, hb_pair_t<دوہرا, دوہرا>> box;
   برائے (ثابت خودکار &axis : axes)
   {
     hb_لیبل_t لیبل_2 = axis.get_axis_لیبل ();
     دوہرا lo = -1.0, hi = +1.0;
     Triple *old_int;
     اگر (plan->user_axes_location.has (لیبل_2))
     {
       اگر (!plan->old_intermediates.has (لیبل_2, &old_int)) واپس جھوٹ;
       lo = (دوہرا) old_int->minimum;
       hi = (دوہرا) old_int->زیادہ_سے_زیادہ;
     }
     ورنہ اگر (axis.is_hidden ())
       lo = hi = 0.0; /* private axis: intermediate is always 0 */
     اگر (!box.سیٹ (لیبل_2, hb_pair (lo, hi))) واپس جھوٹ;
   }

   hb_سمتیہ_t<hb_hashmap_t<hb_لیبل_t, Triple>> regions;
   اگر (!var_store->get_region_list ().get_var_regions (plan->axes_old_اشاریہ_لیبل_map, regions))
     واپس جھوٹ;

   hb_hashmap_t<hb_لیبل_t, غیرعلامتی> grid_pos;
   hb_سمتیہ_t<hb_سمتیہ_t<دوہرا>> grid_points;
   برائے (غیرعلامتی i = 0; i < axes.لمبائی; i++)
   {
     grid_pos.reset ();
     grid_points.reset ();

     hb_لیبل_t لیبل_2 = axes[i].get_axis_لیبل ();
     hb_pair_t<دوہرا, دوہرا> *شناختی_عنصر;
     اگر (!box.has (لیبل_2, &شناختی_عنصر)) واپس جھوٹ;

     /* Collect the row's active regions (non-zero delta). A varidx (or the
      * implicit identity mapping) that doesn't resolve to a real store row
      * behaves as "no variation" at runtime, so it contributes no regions;
      * this also covers a NULL VarStore. */
     hb_سمتیہ_t<hb_pair_t<صحیح, غیرعلامتی>> فعال; /* (delta, region index) */
     uint32_t varidx = varidx_map->map (i);
     /* Runtime treats out-of-range delta-set indices as zero. */
     اگر (varidx != HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ &&
	 !var_store->has_delta_سیٹ (varidx))
       varidx = HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ;
     اگر (varidx != HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ)
     {
       غیرعلامتی outer = varidx >> 16;
       غیرعلامتی inner = varidx & 0xFFFF;
       ثابت OT::Var_ڈیٹا &vd = var_store->get_sub_جدول (outer);
       ثابت OT::HBUINT8 *delta_بائٹیں = vd.get_delta_بائٹیں ();
       غیرعلامتی صف_سائز = vd.get_صف_سائز ();
       غیرعلامتی region_گنتی_2 = vd.get_region_اشاریہ_گنتی ();
       برائے (غیرعلامتی r = 0; r < region_گنتی_2; r++)
       {
	 صحیح delta = vd.get_شے_delta_fast (inner, r, delta_بائٹیں, صف_سائز);
	 اگر (!delta) جاری;
	 غیرعلامتی region_idx = vd.get_region_اشاریہ (r);
	 اگر (region_idx >= regions.لمبائی) واپس جھوٹ;
	 فعال.push (hb_pair (delta, region_idx));
       }
       اگر (فعال.اندر_error ()) واپس جھوٹ;
     }

     /* The delta is piecewise multilinear in the axis coordinates (each
      * region scalar is a product of independent per-axis tents), so over
      * the box its exact extremes are attained at vertices of the grid of
      * per-axis tent breakpoints. Enumerate that grid when it is small
      * (real fonts); otherwise fall back to per-region interval
      * arithmetic, which ignores correlations between regions and is
      * looser but still conservative. Mirrors fontTools'
      * VarStore.getExtremes. */
     constexpr غیرعلامتی MAX_GRID = 1u << 14;
     منطقی exact = سچ;
     دوہرا dmin = 0.0, dmax = 0.0; /* delta extremes, F2Dot14 units */
     دوہرا vmin = 0.0, vmax = 0.0; /* identity + delta extremes, coord units */

     /* Grid axes: the target axis (identity term), plus every axis a valid
      * tent of an active region references. */
     خودکار جوڑو_grid_axis = [&] (hb_لیبل_t t) -> صحیح
     {
       غیرعلامتی *pos;
       اگر (grid_pos.has (t, &pos)) واپس (صحیح) *pos;
       غیرعلامتی new_pos = grid_pos.get_population ();
       hb_pair_t<دوہرا, دوہرا> *داخلہ;
       دوہرا blo = -1.0, bhi = +1.0;
       اگر (box.has (t, &داخلہ)) { blo = داخلہ->پہلا; bhi = داخلہ->second; }
       grid_points.push (hb_سمتیہ_t<دوہرا> ());
       اگر (grid_points.اندر_error ()) واپس -1;
       خودکار &points = grid_points[new_pos];
       points.push (blo);
       points.push (bhi);
       اگر (points.اندر_error () || !grid_pos.سیٹ (t, new_pos)) واپس -1;
       واپس (صحیح) new_pos;
     };

     اگر (جوڑو_grid_axis (لیبل_2) < 0) واپس جھوٹ;
     /* Per active region: (grid axis position, tent) for its valid tents. */
     hb_سمتیہ_t<hb_سمتیہ_t<hb_pair_t<غیرعلامتی, Triple>>> region_tents;
     برائے (ثابت خودکار &ar : فعال)
     {
       region_tents.push (hb_سمتیہ_t<hb_pair_t<غیرعلامتی, Triple>> ());
       اگر (region_tents.اندر_error ()) واپس جھوٹ;
       خودکار &tents = region_tents[region_tents.لمبائی - 1];
       برائے (ثابت خودکار &_ : regions[ar.second])
       {
	 ثابت Triple &tent = _.second;
	 /* Tents the runtime ignores (constant scalar 1) add no breakpoints. */
	 اگر (tent.درمیان == 0.f ||
	     tent.minimum > tent.درمیان || tent.درمیان > tent.زیادہ_سے_زیادہ ||
	     (tent.minimum < 0.f && tent.زیادہ_سے_زیادہ > 0.f))
	   جاری;
	 صحیح pos = جوڑو_grid_axis (_.پہلا);
	 اگر (pos < 0) واپس جھوٹ;
	 خودکار &points = grid_points[pos];
	 دوہرا blo = points[0], bhi = points[1];
	 points.push (hb_clamp ((دوہرا) tent.minimum, blo, bhi));
	 points.push (hb_clamp ((دوہرا) tent.درمیان, blo, bhi));
	 points.push (hb_clamp ((دوہرا) tent.زیادہ_سے_زیادہ, blo, bhi));
	 tents.push (hb_pair ((غیرعلامتی) pos, tent));
	 اگر (points.اندر_error () || tents.اندر_error ()) واپس جھوٹ;
       }
     }

     غیرعلامتی grid_سائز = 1;
     برائے (خودکار &points : grid_points)
     {
       points.qsort ([] (ثابت دوہرا &a, ثابت دوہرا &b) -> صحیح
		     { واپس a < b ? -1 : a > b ? +1 : 0; });
       غیرعلامتی n = 0;
       برائے (غیرعلامتی j = 0; j < points.لمبائی; j++)
	 اگر (!n || points.arrayZ[j] != points.arrayZ[n - 1])
	   points.arrayZ[n++] = points.arrayZ[j];
       points.shrink (n);
       اگر (grid_سائز > MAX_GRID / points.لمبائی) { exact = جھوٹ; توڑو; }
       grid_سائز *= points.لمبائی;
     }

     اگر (exact)
     {
       غیرعلامتی ہدف_pos = 0; /* the target axis was added first */
       hb_سمتیہ_t<غیرعلامتی> odometer;
       اگر (!odometer.resize (grid_pos.get_population ())) واپس جھوٹ;
       منطقی پہلا = سچ;
       جب (سچ)
       {
	 دوہرا delta_مجموع = 0.0;
	 برائے (غیرعلامتی r = 0; r < فعال.لمبائی; r++)
	 {
	   دوہرا scalar = 1.0;
	   برائے (ثابت خودکار &pt : region_tents[r])
	   {
	     دوہرا v = grid_points[pt.پہلا][odometer[pt.پہلا]];
	     scalar *= _tent_eval (pt.second, v);
	     اگر (scalar == 0.0) توڑو;
	   }
	   delta_مجموع += scalar * فعال[r].پہلا;
	 }
	 دوہرا coord = grid_points[ہدف_pos][odometer[ہدف_pos]];
	 دوہرا value = coord + delta_مجموع / 16384.0;
	 اگر (پہلا)
	 {
	   dmin = dmax = delta_مجموع;
	   vmin = vmax = value;
	   پہلا = جھوٹ;
	 }
	 ورنہ
	 {
	   dmin = hb_min (dmin, delta_مجموع); dmax = hb_max (dmax, delta_مجموع);
	   vmin = hb_min (vmin, value);     vmax = hb_max (vmax, value);
	 }
	 غیرعلامتی k = 0;
	 برائے (; k < odometer.لمبائی; k++)
	 {
	   اگر (++odometer[k] < grid_points[k].لمبائی) توڑو;
	   odometer[k] = 0;
	 }
	 اگر (k == odometer.لمبائی) توڑو;
       }
     }
     ورنہ
     {
       برائے (ثابت خودکار &ar : فعال)
       {
	 صحیح delta = ar.پہلا;
	 دوہرا smin = 1.0, smax = 1.0;
	 برائے (ثابت خودکار &_ : regions[ar.second])
	 {
	   hb_pair_t<دوہرا, دوہرا> *داخلہ;
	   دوہرا blo = -1.0, bhi = +1.0;
	   اگر (box.has (_.پہلا, &داخلہ)) { blo = داخلہ->پہلا; bhi = داخلہ->second; }
	   دوہرا tmin, tmax;
	   tent_قدر_range (_.second, blo, bhi, &tmin, &tmax);
	   smin *= tmin;
	   smax *= tmax;
	   اگر (smax == 0.0) توڑو;
	 }
	 اگر (delta > 0) { dmin += smin * delta; dmax += smax * delta; }
	 ورنہ           { dmin += smax * delta; dmax += smin * delta; }
       }
       vmin = شناختی_عنصر->پہلا + dmin / 16384.0;
       vmax = شناختی_عنصر->second + dmax / 16384.0;
     }
     /* A pinned axis whose delta is constant over the box is self-contained:
      * its final coordinate is a constant. It can be removed from fvar/avar
      * and its contribution baked into the variation tables like an ordinary
      * pin at that coordinate (in old final space). Round the delta alone
      * and add it to the exact quantized intermediate, matching the runtime
      * (map_coords_2_14) and fontTools. */
     Triple *user;
     اگر (detect_self_contained && dmin == dmax &&
	 plan->user_axes_location.has (لیبل_2, &user) && user->is_نقطہ ())
     {
       صحیح v_int = (صحیح) roundf ((اعشاری) شناختی_عنصر->پہلا * 16384.f) +
		   (صحیح) roundf ((اعشاری) dmin);
       v_int = hb_clamp (v_int, -(1 << 14), +(1 << 14));
       اگر (!plan->avar2_self_contained.سیٹ (لیبل_2, v_int / 16384.0)) واپس جھوٹ;
       جاری; /* axis is dropped; no region can reference it afterwards */
     }

     /* Pad by one F2Dot14 unit against rounding differences with the
      * runtime's fixed-point evaluation. */
     دوہرا lo = hb_clamp (vmin - 1.0 / 16384.0, -1.0, +1.0);
     دوہرا hi = hb_clamp (vmax + 1.0 / 16384.0, -1.0, +1.0);
     lo = floor (lo * 16384.0) / 16384.0;
     hi = ceil (hi * 16384.0) / 16384.0;
     اگر (lo <= -1.0 && hi >= +1.0)
       جاری; /* not constraining */
     اگر (!plan->avar2_reachable_ranges.سیٹ (لیبل_2, Triple (lo, hb_clamp (0.0, lo, hi), hi)))
       واپس جھوٹ;
   }
   واپس سچ;
 }

#اگر_غیر_تعریف_شدہ HB_NO_OT_FONT_CFF
 ساکن اندرونی hb_font_t*
 _get_hb_font_with_variations (ثابت hb_subset_plan_t *plan)
 {
   hb_font_t *font = hb_font_create (plan->ماخذ);

   اگر (plan->has_avar2)
   {
     /* Under avar2, instancing applies only to the self-contained pins,
      * whose constant final coordinates the plan holds in normalized_coords.
      * Setting user-space variations would run the full avar2 mapping and
      * bake contributions that remain live in the instance's variations. */
     hb_font_سیٹ_var_coords_normalized (font, plan->normalized_coords.arrayZ,
                                        plan->normalized_coords.لمبائی);
     واپس font;
   }

   hb_سمتیہ_t<hb_variation_t> vars;
   اگر (!vars.alloc (plan->user_axes_location.get_population ())) {
     hb_font_destroy (font);
     واپس خالی_اشاریہ;
   }

   برائے (خودکار _ : plan->user_axes_location)
   {
     hb_variation_t var;
     var.لیبل_2 = _.پہلا;
     var.value = _.second.درمیان;
     vars.push (var);
   }

   hb_font_سیٹ_variations (font, vars.arrayZ, plan->user_axes_location.get_population ());
   واپس font;
 }
#اگر_ختم

 سانچہ<قسم_نام شے_Var_Store>
 خالی
 remap_variation_indices (ثابت شے_Var_Store &var_store,
                          ثابت hb_سیٹ_t &variation_indices,
                          ثابت hb_سمتیہ_t<صحیح>& normalized_coords,
                          منطقی calculate_delta, /* not pinned at default */
                          منطقی no_variations, /* all axes pinned */
                          hb_hashmap_t<غیرعلامتی, hb_pair_t<غیرعلامتی, صحیح>> &variation_idx_delta_map /*برآمد*/)
 {
   اگر (&var_store == &Null (OT::شے_Variation_Store)) واپس;
   غیرعلامتی subtable_گنتی = var_store.get_sub_جدول_گنتی ();
   خودکار *store_cache = var_store.create_cache ();

   غیرعلامتی new_major = 0, new_minor = 0;
   غیرعلامتی آخری_major = (variation_indices.get_min ()) >> 16;
   برائے (غیرعلامتی idx : variation_indices)
   {
     صحیح delta = 0;
     اگر (calculate_delta)
       delta = roundf (var_store.get_delta (idx, normalized_coords.arrayZ,
                                            normalized_coords.لمبائی, store_cache));

     اگر (no_variations)
     {
       variation_idx_delta_map.سیٹ (idx, hb_pair_t<غیرعلامتی, صحیح> (HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ, delta));
       جاری;
     }

     uint16_t major = idx >> 16;
     اگر (major >= subtable_گنتی) توڑو;
     اگر (major != آخری_major)
     {
       new_minor = 0;
       ++new_major;
     }

     غیرعلامتی new_idx = (new_major << 16) + new_minor;
     variation_idx_delta_map.سیٹ (idx, hb_pair_t<غیرعلامتی, صحیح> (new_idx, delta));
     ++new_minor;
     آخری_major = major;
   }
   var_store.destroy_cache (store_cache);
 }

 سانچہ
 خالی
 remap_variation_indices<OT::شے_Variation_Store> (ثابت OT::شے_Variation_Store &var_store,
                          ثابت hb_سیٹ_t &variation_indices,
                          ثابت hb_سمتیہ_t<صحیح>& normalized_coords,
                          منطقی calculate_delta, /* not pinned at default */
                          منطقی no_variations, /* all axes pinned */
                          hb_hashmap_t<غیرعلامتی, hb_pair_t<غیرعلامتی, صحیح>> &variation_idx_delta_map /*برآمد*/);

 #اگر_غیر_تعریف_شدہ HB_NO_بنیاد
 خالی
 collect_بنیاد_variation_indices (hb_subset_plan_t* plan)
 {
   hb_blob_ptr_t<OT::بنیاد_4> بنیاد = plan->ماخذ_جدول<OT::بنیاد_4> ();
   اگر (!بنیاد->has_var_store ())
   {
     بنیاد.destroy ();
     واپس;
   }

   hb_سیٹ_t varidx_سیٹ;
   بنیاد->collect_variation_indices (plan, varidx_سیٹ);
   ثابت OT::شے_Variation_Store &var_store = بنیاد->get_var_store ();
   غیرعلامتی subtable_گنتی = var_store.get_sub_جدول_گنتی ();


   remap_variation_indices (var_store, varidx_سیٹ,
                             plan->normalized_coords,
                             !plan->pinned_at_default,
                             plan->all_axes_pinned,
                             plan->بنیاد_variation_idx_map);
   generate_varstore_inner_maps (varidx_سیٹ, subtable_گنتی, plan->بنیاد_varstore_inner_maps);

   بنیاد.destroy ();
 }

 #اگر_ختم

منطقی
normalize_axes_location (hb_face_t *face, hb_subset_plan_t *plan)
{
  اگر (plan->user_axes_location.is_empty ())
    واپس سچ;

  hb_array_t<ثابت OT::Axis_ریکارڈ> axes = face->جدول.fvar->get_axes ();
  برائے (ثابت خودکار &axis : axes)
  {
    hb_لیبل_t لیبل_2 = axis.get_axis_لیبل ();
    ثابت Triple *range;
    اعشاری minimum, درمیان, زیادہ_سے_زیادہ;
    axis.get_coordinates (minimum, درمیان, زیادہ_سے_زیادہ);
    اگر (plan->user_axes_location.has (لیبل_2, &range) && !range->is_نقطہ () &&
        range->minimum == minimum &&
        range->درمیان == درمیان &&
        range->زیادہ_سے_زیادہ == زیادہ_سے_زیادہ)
      plan->user_axes_location.del (لیبل_2);
  }
  اگر (plan->user_axes_location.is_empty ())
    واپس سچ;

  اگر (!plan->check_success (plan->normalized_coords.resize (axes.لمبائی)))
    واپس جھوٹ;

  منطقی has_avar = face->جدول.avar->has_ڈیٹا ();
  hb_سمتیہ_t<اعشاری> normalized_mins;
  hb_سمتیہ_t<اعشاری> normalized_defaults;
  hb_سمتیہ_t<اعشاری> normalized_maxs;
  اگر (has_avar)
  {
    اگر (!plan->check_success (normalized_mins.resize (axes.لمبائی)) ||
        !plan->check_success (normalized_defaults.resize (axes.لمبائی)) ||
        !plan->check_success (normalized_maxs.resize (axes.لمبائی)))
      واپس جھوٹ;
  }

  منطقی axis_not_pinned = جھوٹ;
  غیرعلامتی new_axis_idx = 0;
  غیرعلامتی آخری_idx = 0;
  برائے (ثابت خودکار& _ : + hb_enumerate (axes))
  {
    غیرعلامتی i = _.پہلا;
    ثابت OT::Axis_ریکارڈ &axis = _.second;
    hb_لیبل_t axis_لیبل_2 = axis.get_axis_لیبل ();
    plan->axes_old_اشاریہ_لیبل_map.سیٹ (i, axis_لیبل_2);

    اگر (!plan->user_axes_location.has (axis_لیبل_2) ||
        !plan->user_axes_location.get (axis_لیبل_2).is_نقطہ ())
    {
      axis_not_pinned = سچ;
      plan->axes_اشاریہ_map.سیٹ (i, new_axis_idx);
      plan->axis_لیبل_فہرست.push (axis_لیبل_2);
      new_axis_idx++;
    }

    Triple *axis_range;
    اگر (plan->user_axes_location.has (axis_لیبل_2, &axis_range))
    {
      plan->axes_triple_distances.سیٹ (axis_لیبل_2, axis.get_triple_distances ());

      اعشاری normalized_min = axis.normalize_axis_قدر (axis_range->minimum);
      اعشاری normalized_default = axis.normalize_axis_قدر (axis_range->درمیان);
      اعشاری normalized_max = axis.normalize_axis_قدر (axis_range->زیادہ_سے_زیادہ);

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

      اگر (has_avar)
      {
        normalized_mins[i] = normalized_min;
        normalized_defaults[i] = normalized_default;
        normalized_maxs[i] = normalized_max;
        آخری_idx = i;
      }
      ورنہ
      {
        plan->axes_location.سیٹ (axis_لیبل_2, Triple ((دوہرا) normalized_min,
                                                   (دوہرا) normalized_default,
                                                   (دوہرا) normalized_max));
        اگر (normalized_default == -0.f)
          normalized_default = 0.f; // Normalize -0 to 0
        اگر (normalized_default != 0.f)
          plan->pinned_at_default = جھوٹ;

        plan->normalized_coords[i] = roundf (normalized_default * 16384.f);
      }
    }
  }
  plan->all_axes_pinned = !axis_not_pinned;

  // TODO: use avar map_coords_16_16() when normalization is changed to 16.16
  // in fonttools
  اگر (has_avar)
  {
    ثابت OT::avar* avar_جدول = face->جدول.avar;
    غیرعلامتی coords_len = آخری_idx + 1;

    اگر (avar_جدول->has_v2_ڈیٹا () && !plan->all_axes_pinned)
    {
      /* avar2 partial instancing: use v1-only mapping to get intermediate-space
       * coords. These are used for IVS rebasing and offset compensation. */
      plan->has_avar2 = سچ;

      اگر (!plan->check_success (avar_جدول->map_coords_2_14 (normalized_mins.arrayZ, coords_len, سچ)) ||
          !plan->check_success (avar_جدول->map_coords_2_14 (normalized_defaults.arrayZ, coords_len, سچ)) ||
          !plan->check_success (avar_جدول->map_coords_2_14 (normalized_maxs.arrayZ, coords_len, سچ)))
        واپس جھوٹ;

      برائے (ثابت خودکار& _ : + hb_enumerate (axes))
      {
        غیرعلامتی i = _.پہلا;
        hb_لیبل_t axis_لیبل_2 = _.second.get_axis_لیبل ();
        اگر (plan->user_axes_location.has (axis_لیبل_2))
        {
          /* Store intermediate-space coords for offset compensation,
           * segment-map renormalization, and IVS rebasing. */
          plan->old_intermediates.سیٹ (axis_لیبل_2, Triple ((دوہرا) normalized_mins[i],
                                                         (دوہرا) normalized_defaults[i],
                                                         (دوہرا) normalized_maxs[i]));
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
      hb_blob_t *cff2_blob = hb_face_reference_جدول (face, HB_لیبل ('C','F','F','2'));
      منطقی detect_self_contained = !hb_blob_get_لمبائی (cff2_blob);
      hb_blob_destroy (cff2_blob);
      اگر (!_compute_avar2_reachable_ranges (plan, axes, avar_جدول,
                                            detect_self_contained))
        واپس جھوٹ;

#اگر_غیر_تعریف_شدہ HB_NO_VAR_COMPOSITES
      /* VARC overrides can reach coordinates outside the font-level
       * ranges. Do not cull variation data using those ranges. */
      اگر (!plan->drop_tables.has (HB_لیبل ('V','A','R','C')))
      {
        hb_blob_t *varc_blob = hb_face_reference_جدول (face, HB_لیبل ('V','A','R','C'));
        اگر (hb_blob_get_لمبائی (varc_blob))
          plan->avar2_reachable_ranges.reset ();
        hb_blob_destroy (varc_blob);
      }
#اگر_ختم

      /* Keep all axes in fvar (pinned ones as hidden), EXCEPT self-contained
       * pinned axes, which are removed entirely. */
      plan->axes_اشاریہ_map.reset ();
      plan->axis_لیبل_فہرست.reset ();
      غیرعلامتی retained_axis_idx = 0;
      برائے (ثابت خودکار& _ : + hb_enumerate (axes))
      {
        غیرعلامتی i = _.پہلا;
        hb_لیبل_t axis_لیبل_2 = _.second.get_axis_لیبل ();
        اگر (plan->avar2_self_contained.has (axis_لیبل_2))
          جاری;
        plan->axes_اشاریہ_map.سیٹ (i, retained_axis_idx++);
        plan->axis_لیبل_فہرست.push (axis_لیبل_2);
      }
      /* The other tables see ONLY the self-contained pins, as an ordinary
       * partial instancing in old final-coordinate space; the standard
       * instancing machinery bakes those axes' contributions in. The
       * remaining restriction is carried entirely by avar. */
      plan->axes_location.reset ();
      اگر (plan->avar2_self_contained.get_population ())
      {
        منطقی sc_pinned_at_default = سچ;
        برائے (خودکار _ : plan->avar2_self_contained)
        {
          plan->axes_location.سیٹ (_.پہلا, Triple (_.second, _.second, _.second));
          اگر (_.second != 0.0)
            sc_pinned_at_default = جھوٹ;
        }
        برائے (ثابت خودکار& _ : + hb_enumerate (axes))
        {
          دوہرا *v;
          plan->normalized_coords[_.پہلا] =
              plan->avar2_self_contained.has (_.second.get_axis_لیبل (), &v)
              ? (صحیح) roundf ((اعشاری) (*v * 16384.0)) : 0;
        }
        plan->pinned_at_default = sc_pinned_at_default;
      }
      ورنہ
        plan->normalized_coords.shrink (0);
    }
    ورنہ
    {
      /* Standard avar v1 (or v1+v2 with all axes pinned) */
      اگر (!plan->check_success (avar_جدول->map_coords_2_14 (normalized_mins.arrayZ, coords_len)) ||
          !plan->check_success (avar_جدول->map_coords_2_14 (normalized_defaults.arrayZ, coords_len)) ||
          !plan->check_success (avar_جدول->map_coords_2_14 (normalized_maxs.arrayZ, coords_len)))
        واپس جھوٹ;

      برائے (ثابت خودکار& _ : + hb_enumerate (axes))
      {
        غیرعلامتی i = _.پہلا;
        hb_لیبل_t axis_لیبل_2 = _.second.get_axis_لیبل ();
        اگر (plan->user_axes_location.has (axis_لیبل_2))
        {
          plan->axes_location.سیٹ (axis_لیبل_2, Triple ((دوہرا) normalized_mins[i],
                                                     (دوہرا) normalized_defaults[i],
                                                     (دوہرا) normalized_maxs[i]));
          اعشاری normalized_default = normalized_defaults[i];
          اگر (normalized_default == -0.f)
            normalized_default = 0.f;
          اگر (normalized_default != 0.f)
            plan->pinned_at_default = جھوٹ;

          plan->normalized_coords[i] = roundf (normalized_default * 16384.f);
        }
      }
    }
  }
  واپس سچ;
}

#اگر_غیر_تعریف_شدہ HB_NO_OT_FONT_CFF
خالی
update_instance_metrics_map_from_cff2 (hb_subset_plan_t *plan)
{
  اگر (!plan->normalized_coords) واپس;
  OT::cff2::accelerator_t cff2 (plan->ماخذ);
  اگر (!cff2.is_valid ()) واپس;

  hb_font_t *font = _get_hb_font_with_variations (plan);
  اگر (unlikely (!plan->check_success (font != خالی_اشاریہ)))
  {
    hb_font_destroy (font);
    واپس;
  }

  hb_glyph_extents_t extents = {0x7FFF, -0x7FFF};
  OT::hmtx_accelerator_t _hmtx (plan->ماخذ);
  OT::hb_scalar_cache_t *hvar_store_cache = خالی_اشاریہ;
  اگر (_hmtx.has_ڈیٹا () && _hmtx.var_جدول.get_لمبائی ())
    hvar_store_cache = _hmtx.var_جدول->get_var_store ().create_cache ();

  OT::vmtx_accelerator_t _vmtx (plan->ماخذ);
  OT::hb_scalar_cache_t *vvar_store_cache = خالی_اشاریہ;
  اگر (_vmtx.has_ڈیٹا () && _vmtx.var_جدول.get_لمبائی ())
    vvar_store_cache = _vmtx.var_جدول->get_var_store ().create_cache ();

  برائے (خودکار p : *plan->glyph_map)
  {
    hb_codepoint_t old_gid = p.پہلا;
    hb_codepoint_t new_gid = p.second;
    اگر (!cff2.get_extents (font, old_gid, &extents)) جاری;
    منطقی has_bounds_info = سچ;
    اگر (extents.x_bearing == 0 && extents.چوڑائی == 0 &&
        extents.اونچائی == 0 && extents.y_bearing == 0)
      has_bounds_info = جھوٹ;

    اگر (has_bounds_info)
    {
      plan->سر_maxp_info.xMin = hb_min (plan->سر_maxp_info.xMin, extents.x_bearing);
      plan->سر_maxp_info.xMax = hb_max (plan->سر_maxp_info.xMax,
						 hb_saturate_جوڑو (extents.x_bearing, extents.چوڑائی));
      plan->سر_maxp_info.yMax = hb_max (plan->سر_maxp_info.yMax, extents.y_bearing);
      plan->سر_maxp_info.yMin = hb_min (plan->سر_maxp_info.yMin,
						 hb_saturate_جوڑو (extents.y_bearing, extents.اونچائی));
    }

    اگر (_hmtx.has_ڈیٹا ())
    {
      دوہرا hori_aw = _hmtx.get_advance_without_var_unscaled (old_gid);
      اگر (_hmtx.var_جدول.get_لمبائی ())
        hori_aw += (دوہرا) roundf (_hmtx.var_جدول->get_advance_delta_unscaled (old_gid, font->coords, font->عدد_coords,
									 hvar_store_cache));
      /* Malicious deltas can take the advance out of the UFWORD range that
       * hmtx serialization stores.  Clamp, to keep downstream consumers
       * (e.g. CFF width optimizer) bounded and consistent with hmtx. */
      hori_aw = hb_clamp (hori_aw, 0., 65535.);
      صحیح lsb = extents.x_bearing;
      اگر (!has_bounds_info)
      {
        _hmtx.get_leading_bearing_without_var_unscaled (old_gid, &lsb);
      }
      plan->hmtx_map.سیٹ (new_gid, hb_pair ((غیرعلامتی) hori_aw, lsb));
      plan->bounds_چوڑائی_vec[new_gid] = extents.چوڑائی;
    }

    اگر (_vmtx.has_ڈیٹا ())
    {
      دوہرا vert_aw = _vmtx.get_advance_without_var_unscaled (old_gid);
      اگر (_vmtx.var_جدول.get_لمبائی ())
        vert_aw += (دوہرا) roundf (_vmtx.var_جدول->get_advance_delta_unscaled (old_gid, font->coords, font->عدد_coords,
									 vvar_store_cache));
      vert_aw = hb_clamp (vert_aw, 0., 65535.);
      hb_position_t vorg_x = 0;
      hb_position_t vorg_y = 0;
      صحیح tsb = 0;
      اگر (has_bounds_info &&
           hb_font_get_glyph_v_origin (font, old_gid, &vorg_x, &vorg_y))
      {
        tsb = hb_saturate_sub (vorg_y, extents.y_bearing);
      } ورنہ {
        _vmtx.get_leading_bearing_without_var_unscaled (old_gid, &tsb);
      }

      plan->vmtx_map.سیٹ (new_gid, hb_pair ((غیرعلامتی) vert_aw, tsb));
      plan->bounds_اونچائی_vec[new_gid] = extents.اونچائی;
    }
  }
  /* No glyph contributed bounds: do not serialize the accumulator's
   * inverted sentinels into head (or a downgraded CFF FontBBox). */
  اگر (plan->سر_maxp_info.xMin > plan->سر_maxp_info.xMax)
  {
    plan->سر_maxp_info.xMin = plan->سر_maxp_info.xMax = 0;
    plan->سر_maxp_info.yMin = plan->سر_maxp_info.yMax = 0;
  }
  hb_font_destroy (font);
  اگر (hvar_store_cache)
    _hmtx.var_جدول->get_var_store ().destroy_cache (hvar_store_cache);
  اگر (vvar_store_cache)
    _vmtx.var_جدول->get_var_store ().destroy_cache (vvar_store_cache);
}
#اگر_ختم

منطقی
get_instance_glyphs_contour_points (hb_subset_plan_t *plan)
{
  /* contour_points vector only needed for updating gvar table (infer delta and
   * iup delta optimization) during partial instancing */
  اگر (plan->user_axes_location.is_empty () || plan->all_axes_pinned)
    واپس سچ;

  OT::glyf_accelerator_t glyf (plan->ماخذ);

  برائے (خودکار &_ : plan->new_مقصد_old_gid_list)
  {
    hb_codepoint_t new_gid = _.پہلا;
    contour_نقطہ_سمتیہ_t all_points;
    اگر (new_gid == 0 && !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      اگر (unlikely (!plan->new_gid_contour_points_map.سیٹ (new_gid, all_points)))
        واپس جھوٹ;
      جاری;
    }

    hb_codepoint_t old_gid = _.second;
    خودکار glyph = glyf.glyph_for_gid (old_gid);
    اگر (unlikely (!glyph.get_all_points_without_var (plan->ماخذ, all_points)))
      واپس جھوٹ;
    اگر (unlikely (!plan->new_gid_contour_points_map.سیٹ (new_gid, all_points)))
      واپس جھوٹ;

    /* composite new gids are only needed by iup delta optimization */
    اگر ((plan->flags & HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS) && glyph.is_مرکب ())
      plan->مرکب_new_gids.جوڑو (new_gid);
  }
  واپس سچ;
}

سانچہ<قسم_نام Delta_سیٹ_اشاریہ_Map>
خالی
remap_colrv1_delta_سیٹ_اشاریہ_indices (ثابت Delta_سیٹ_اشاریہ_Map &اشاریہ_map,
                                      ثابت hb_سیٹ_t &delta_سیٹ_idxes,
                                      hb_hashmap_t<غیرعلامتی, hb_pair_t<غیرعلامتی, صحیح>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /*برآمد*/)
{
  اگر (!اشاریہ_map.get_map_گنتی ())
    واپس;

  hb_hashmap_t<غیرعلامتی, hb_pair_t<غیرعلامتی, صحیح>> delta_سیٹ_idx_delta_map;
  غیرعلامتی new_delta_سیٹ_idx = 0;
  برائے (غیرعلامتی delta_سیٹ_idx : delta_سیٹ_idxes)
  {
    غیرعلامتی var_idx = اشاریہ_map.map (delta_سیٹ_idx);
    غیرعلامتی new_varidx = HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ;
    صحیح delta = 0;

    اگر (var_idx != HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ)
    {
      hb_pair_t<غیرعلامتی, صحیح> *new_varidx_delta;
      اگر (!variation_idx_delta_map.has (var_idx, &new_varidx_delta)) جاری;

      new_varidx = hb_پہلا (*new_varidx_delta);
      delta = hb_دوسرا (*new_varidx_delta);
    }

    new_deltaset_idx_varidx_map.سیٹ (new_delta_سیٹ_idx, new_varidx);
    delta_سیٹ_idx_delta_map.سیٹ (delta_سیٹ_idx, hb_pair_t<غیرعلامتی, صحیح> (new_delta_سیٹ_idx, delta));
    new_delta_سیٹ_idx++;
  }
  variation_idx_delta_map = std::move (delta_سیٹ_idx_delta_map);
}

سانچہ خالی
remap_colrv1_delta_سیٹ_اشاریہ_indices<OT::Delta_سیٹ_اشاریہ_Map> (ثابت OT::Delta_سیٹ_اشاریہ_Map &اشاریہ_map,
                                      ثابت hb_سیٹ_t &delta_سیٹ_idxes,
                                      hb_hashmap_t<غیرعلامتی, hb_pair_t<غیرعلامتی, صحیح>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /*برآمد*/);

 #اگر_ختم

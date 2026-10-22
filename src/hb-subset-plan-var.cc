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

 #که_نه_تعریف_شوی HB_NO_VAR

 فراغ
 generate_varstore_inner_maps (ثابت hb_سېټ_t& varidx_سېټ,
                               بینښه subtable_شمېر,
                               hb_ویکټور_t<hb_inc_bimap_t> &inner_maps /*بهر*/)
 {
   إذا (varidx_سېټ.is_empty () || subtable_شمېر == 0) أرجع;

   إذا (unlikely (!inner_maps.resize (subtable_شمېر))) أرجع;
   لكل (بینښه idx : varidx_سېټ)
   {
     uint16_t major = idx >> 16;
     uint16_t minor = idx & 0xFFFF;

     إذا (major >= subtable_شمېر)
       استمر;
     inner_maps[major].جمع_کړه (minor);
   }
 }

 /* Evaluate a variation-region tent at v, mirroring VarRegionAxis::evaluate
  * (invalid regions evaluate as constant 1). */
 ساكن مزدوج
 _tent_eval (ثابت Triple &tent, مزدوج v)
 {
   مزدوج پیل = (مزدوج) tent.minimum;
   مزدوج peak = (مزدوج) tent.منځ;
   مزدوج پای_ټکی = (مزدوج) tent.اعظمي;
   إذا (unlikely (پیل > peak || peak > پای_ټکی)) أرجع 1.0;
   إذا (unlikely (پیل < 0.0 && پای_ټکی > 0.0 && peak != 0.0)) أرجع 1.0;
   إذا (peak == 0.0 || v == peak) أرجع 1.0;
   إذا (v <= پیل || پای_ټکی <= v) أرجع 0.0;
   إذا (v < peak) أرجع (v - پیل) / (peak - پیل);
   أرجع (پای_ټکی - v) / (پای_ټکی - peak);
 }

 /* Range of a tent's scalar over an input interval [lo, hi]. The tent is
  * unimodal, so the extremes are at the interval endpoints, plus the peak
  * when it falls inside. */
 ساكن فراغ
 tent_ارزښت_range (ثابت Triple &tent, مزدوج lo, مزدوج hi,
                    مزدوج *tmin, مزدوج *tmax)
 {
   مزدوج a = _tent_eval (tent, lo);
   مزدوج b = _tent_eval (tent, hi);
   *tmin = hb_min (a, b);
   *tmax = hb_max (a, b);
   إذا (lo <= (مزدوج) tent.منځ && (مزدوج) tent.منځ <= hi)
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
                                  hb_array_t<ثابت OT::Axis_ریکارډ> axes,
                                  ثابت OT::avar *avar_جدول,
                                  منطقي detect_self_contained)
 {
   ثابت OT::توکی_Variation_Store *var_store;
   ثابت OT::Delta_سېټ_شاخص_Map *varidx_map;
   إذا (!avar_جدول->get_v2_store_and_map (&var_store, &varidx_map))
     أرجع خطأ;

   /* Old-intermediate input box per axis. */
   hb_hashmap_t<hb_نښان_t, hb_pair_t<مزدوج, مزدوج>> box;
   لكل (ثابت خپلکاری &axis : axes)
   {
     hb_نښان_t نښان_2 = axis.get_axis_نښان ();
     مزدوج lo = -1.0, hi = +1.0;
     Triple *old_int;
     إذا (plan->user_axes_location.has (نښان_2))
     {
       إذا (!plan->old_intermediates.has (نښان_2, &old_int)) أرجع خطأ;
       lo = (مزدوج) old_int->minimum;
       hi = (مزدوج) old_int->اعظمي;
     }
     وإلا إذا (axis.is_hidden ())
       lo = hi = 0.0; /* private axis: intermediate is always 0 */
     إذا (!box.سېټ (نښان_2, hb_pair (lo, hi))) أرجع خطأ;
   }

   hb_ویکټور_t<hb_hashmap_t<hb_نښان_t, Triple>> regions;
   إذا (!var_store->get_region_list ().get_var_regions (plan->axes_old_شاخص_نښان_map, regions))
     أرجع خطأ;

   hb_hashmap_t<hb_نښان_t, بینښه> grid_pos;
   hb_ویکټور_t<hb_ویکټور_t<مزدوج>> grid_points;
   لكل (بینښه i = 0; i < axes.اوږدتیا; i++)
   {
     grid_pos.reset ();
     grid_points.reset ();

     hb_نښان_t نښان_2 = axes[i].get_axis_نښان ();
     hb_pair_t<مزدوج, مزدوج> *بې_اغېزه_عنصر;
     إذا (!box.has (نښان_2, &بې_اغېزه_عنصر)) أرجع خطأ;

     /* Collect the row's active regions (non-zero delta). A varidx (or the
      * implicit identity mapping) that doesn't resolve to a real store row
      * behaves as "no variation" at runtime, so it contributes no regions;
      * this also covers a NULL VarStore. */
     hb_ویکټور_t<hb_pair_t<صحيح, بینښه>> فعال; /* (delta, region index) */
     uint32_t varidx = varidx_map->map (i);
     /* Runtime treats out-of-range delta-set indices as zero. */
     إذا (varidx != HB_OT_LAYOUT_NO_VARIATIONS_شاخص &&
	 !var_store->has_delta_سېټ (varidx))
       varidx = HB_OT_LAYOUT_NO_VARIATIONS_شاخص;
     إذا (varidx != HB_OT_LAYOUT_NO_VARIATIONS_شاخص)
     {
       بینښه outer = varidx >> 16;
       بینښه inner = varidx & 0xFFFF;
       ثابت OT::Var_معلومات &vd = var_store->get_sub_جدول (outer);
       ثابت OT::HBUINT8 *delta_بایټونه = vd.get_delta_بایټونه ();
       بینښه لیکه_اندازه = vd.get_لیکه_اندازه ();
       بینښه region_شمېر_2 = vd.get_region_شاخص_شمېر ();
       لكل (بینښه r = 0; r < region_شمېر_2; r++)
       {
	 صحيح delta = vd.get_توکی_delta_fast (inner, r, delta_بایټونه, لیکه_اندازه);
	 إذا (!delta) استمر;
	 بینښه region_idx = vd.get_region_شاخص (r);
	 إذا (region_idx >= regions.اوږدتیا) أرجع خطأ;
	 فعال.push (hb_pair (delta, region_idx));
       }
       إذا (فعال.دننه_error ()) أرجع خطأ;
     }

     /* The delta is piecewise multilinear in the axis coordinates (each
      * region scalar is a product of independent per-axis tents), so over
      * the box its exact extremes are attained at vertices of the grid of
      * per-axis tent breakpoints. Enumerate that grid when it is small
      * (real fonts); otherwise fall back to per-region interval
      * arithmetic, which ignores correlations between regions and is
      * looser but still conservative. Mirrors fontTools'
      * VarStore.getExtremes. */
     constexpr بینښه MAX_GRID = 1u << 14;
     منطقي exact = ریښتیا;
     مزدوج dmin = 0.0, dmax = 0.0; /* delta extremes, F2Dot14 units */
     مزدوج vmin = 0.0, vmax = 0.0; /* identity + delta extremes, coord units */

     /* Grid axes: the target axis (identity term), plus every axis a valid
      * tent of an active region references. */
     خپلکاری جمع_کړه_grid_axis = [&] (hb_نښان_t t) -> صحيح
     {
       بینښه *pos;
       إذا (grid_pos.has (t, &pos)) أرجع (صحيح) *pos;
       بینښه new_pos = grid_pos.get_population ();
       hb_pair_t<مزدوج, مزدوج> *ننوت;
       مزدوج blo = -1.0, bhi = +1.0;
       إذا (box.has (t, &ننوت)) { blo = ننوت->لومړی; bhi = ننوت->second; }
       grid_points.push (hb_ویکټور_t<مزدوج> ());
       إذا (grid_points.دننه_error ()) أرجع -1;
       خپلکاری &points = grid_points[new_pos];
       points.push (blo);
       points.push (bhi);
       إذا (points.دننه_error () || !grid_pos.سېټ (t, new_pos)) أرجع -1;
       أرجع (صحيح) new_pos;
     };

     إذا (جمع_کړه_grid_axis (نښان_2) < 0) أرجع خطأ;
     /* Per active region: (grid axis position, tent) for its valid tents. */
     hb_ویکټور_t<hb_ویکټور_t<hb_pair_t<بینښه, Triple>>> region_tents;
     لكل (ثابت خپلکاری &ar : فعال)
     {
       region_tents.push (hb_ویکټور_t<hb_pair_t<بینښه, Triple>> ());
       إذا (region_tents.دننه_error ()) أرجع خطأ;
       خپلکاری &tents = region_tents[region_tents.اوږدتیا - 1];
       لكل (ثابت خپلکاری &_ : regions[ar.second])
       {
	 ثابت Triple &tent = _.second;
	 /* Tents the runtime ignores (constant scalar 1) add no breakpoints. */
	 إذا (tent.منځ == 0.f ||
	     tent.minimum > tent.منځ || tent.منځ > tent.اعظمي ||
	     (tent.minimum < 0.f && tent.اعظمي > 0.f))
	   استمر;
	 صحيح pos = جمع_کړه_grid_axis (_.لومړی);
	 إذا (pos < 0) أرجع خطأ;
	 خپلکاری &points = grid_points[pos];
	 مزدوج blo = points[0], bhi = points[1];
	 points.push (hb_clamp ((مزدوج) tent.minimum, blo, bhi));
	 points.push (hb_clamp ((مزدوج) tent.منځ, blo, bhi));
	 points.push (hb_clamp ((مزدوج) tent.اعظمي, blo, bhi));
	 tents.push (hb_pair ((بینښه) pos, tent));
	 إذا (points.دننه_error () || tents.دننه_error ()) أرجع خطأ;
       }
     }

     بینښه grid_اندازه = 1;
     لكل (خپلکاری &points : grid_points)
     {
       points.qsort ([] (ثابت مزدوج &a, ثابت مزدوج &b) -> صحيح
		     { أرجع a < b ? -1 : a > b ? +1 : 0; });
       بینښه n = 0;
       لكل (بینښه j = 0; j < points.اوږدتیا; j++)
	 إذا (!n || points.arrayZ[j] != points.arrayZ[n - 1])
	   points.arrayZ[n++] = points.arrayZ[j];
       points.shrink (n);
       إذا (grid_اندازه > MAX_GRID / points.اوږدتیا) { exact = خطأ; اكسر; }
       grid_اندازه *= points.اوږدتیا;
     }

     إذا (exact)
     {
       بینښه موخه_pos = 0; /* the target axis was added first */
       hb_ویکټور_t<بینښه> odometer;
       إذا (!odometer.resize (grid_pos.get_population ())) أرجع خطأ;
       منطقي لومړی = ریښتیا;
       طالما (ریښتیا)
       {
	 مزدوج delta_مجموعه = 0.0;
	 لكل (بینښه r = 0; r < فعال.اوږدتیا; r++)
	 {
	   مزدوج scalar = 1.0;
	   لكل (ثابت خپلکاری &pt : region_tents[r])
	   {
	     مزدوج v = grid_points[pt.لومړی][odometer[pt.لومړی]];
	     scalar *= _tent_eval (pt.second, v);
	     إذا (scalar == 0.0) اكسر;
	   }
	   delta_مجموعه += scalar * فعال[r].لومړی;
	 }
	 مزدوج coord = grid_points[موخه_pos][odometer[موخه_pos]];
	 مزدوج value = coord + delta_مجموعه / 16384.0;
	 إذا (لومړی)
	 {
	   dmin = dmax = delta_مجموعه;
	   vmin = vmax = value;
	   لومړی = خطأ;
	 }
	 وإلا
	 {
	   dmin = hb_min (dmin, delta_مجموعه); dmax = hb_max (dmax, delta_مجموعه);
	   vmin = hb_min (vmin, value);     vmax = hb_max (vmax, value);
	 }
	 بینښه k = 0;
	 لكل (; k < odometer.اوږدتیا; k++)
	 {
	   إذا (++odometer[k] < grid_points[k].اوږدتیا) اكسر;
	   odometer[k] = 0;
	 }
	 إذا (k == odometer.اوږدتیا) اكسر;
       }
     }
     وإلا
     {
       لكل (ثابت خپلکاری &ar : فعال)
       {
	 صحيح delta = ar.لومړی;
	 مزدوج smin = 1.0, smax = 1.0;
	 لكل (ثابت خپلکاری &_ : regions[ar.second])
	 {
	   hb_pair_t<مزدوج, مزدوج> *ننوت;
	   مزدوج blo = -1.0, bhi = +1.0;
	   إذا (box.has (_.لومړی, &ننوت)) { blo = ننوت->لومړی; bhi = ننوت->second; }
	   مزدوج tmin, tmax;
	   tent_ارزښت_range (_.second, blo, bhi, &tmin, &tmax);
	   smin *= tmin;
	   smax *= tmax;
	   إذا (smax == 0.0) اكسر;
	 }
	 إذا (delta > 0) { dmin += smin * delta; dmax += smax * delta; }
	 وإلا           { dmin += smax * delta; dmax += smin * delta; }
       }
       vmin = بې_اغېزه_عنصر->لومړی + dmin / 16384.0;
       vmax = بې_اغېزه_عنصر->second + dmax / 16384.0;
     }
     /* A pinned axis whose delta is constant over the box is self-contained:
      * its final coordinate is a constant. It can be removed from fvar/avar
      * and its contribution baked into the variation tables like an ordinary
      * pin at that coordinate (in old final space). Round the delta alone
      * and add it to the exact quantized intermediate, matching the runtime
      * (map_coords_2_14) and fontTools. */
     Triple *user;
     إذا (detect_self_contained && dmin == dmax &&
	 plan->user_axes_location.has (نښان_2, &user) && user->is_ټکی ())
     {
       صحيح v_int = (صحيح) roundf ((عائم) بې_اغېزه_عنصر->لومړی * 16384.f) +
		   (صحيح) roundf ((عائم) dmin);
       v_int = hb_clamp (v_int, -(1 << 14), +(1 << 14));
       إذا (!plan->avar2_self_contained.سېټ (نښان_2, v_int / 16384.0)) أرجع خطأ;
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
     إذا (!plan->avar2_reachable_ranges.سېټ (نښان_2, Triple (lo, hb_clamp (0.0, lo, hi), hi)))
       أرجع خطأ;
   }
   أرجع ریښتیا;
 }

#که_نه_تعریف_شوی HB_NO_OT_FONT_CFF
 ساكن مضمن hb_font_t*
 _get_hb_font_with_variations (ثابت hb_subset_plan_t *plan)
 {
   hb_font_t *font = hb_font_create (plan->سرچینه);

   إذا (plan->has_avar2)
   {
     /* Under avar2, instancing applies only to the self-contained pins,
      * whose constant final coordinates the plan holds in normalized_coords.
      * Setting user-space variations would run the full avar2 mapping and
      * bake contributions that remain live in the instance's variations. */
     hb_font_سېټ_var_coords_normalized (font, plan->normalized_coords.arrayZ,
                                        plan->normalized_coords.اوږدتیا);
     أرجع font;
   }

   hb_ویکټور_t<hb_variation_t> vars;
   إذا (!vars.alloc (plan->user_axes_location.get_population ())) {
     hb_font_destroy (font);
     أرجع تش_اشاره;
   }

   لكل (خپلکاری _ : plan->user_axes_location)
   {
     hb_variation_t var;
     var.نښان_2 = _.لومړی;
     var.value = _.second.منځ;
     vars.push (var);
   }

   hb_font_سېټ_variations (font, vars.arrayZ, plan->user_axes_location.get_population ());
   أرجع font;
 }
#د_که_پای

 کينډۍ<ډول_نوم توکی_Var_Store>
 فراغ
 remap_variation_indices (ثابت توکی_Var_Store &var_store,
                          ثابت hb_سېټ_t &variation_indices,
                          ثابت hb_ویکټور_t<صحيح>& normalized_coords,
                          منطقي calculate_delta, /* not pinned at default */
                          منطقي no_variations, /* all axes pinned */
                          hb_hashmap_t<بینښه, hb_pair_t<بینښه, صحيح>> &variation_idx_delta_map /*بهر*/)
 {
   إذا (&var_store == &Null (OT::توکی_Variation_Store)) أرجع;
   بینښه subtable_شمېر = var_store.get_sub_جدول_شمېر ();
   خپلکاری *store_cache = var_store.create_cache ();

   بینښه new_major = 0, new_minor = 0;
   بینښه وروستی_major = (variation_indices.get_min ()) >> 16;
   لكل (بینښه idx : variation_indices)
   {
     صحيح delta = 0;
     إذا (calculate_delta)
       delta = roundf (var_store.get_delta (idx, normalized_coords.arrayZ,
                                            normalized_coords.اوږدتیا, store_cache));

     إذا (no_variations)
     {
       variation_idx_delta_map.سېټ (idx, hb_pair_t<بینښه, صحيح> (HB_OT_LAYOUT_NO_VARIATIONS_شاخص, delta));
       استمر;
     }

     uint16_t major = idx >> 16;
     إذا (major >= subtable_شمېر) اكسر;
     إذا (major != وروستی_major)
     {
       new_minor = 0;
       ++new_major;
     }

     بینښه new_idx = (new_major << 16) + new_minor;
     variation_idx_delta_map.سېټ (idx, hb_pair_t<بینښه, صحيح> (new_idx, delta));
     ++new_minor;
     وروستی_major = major;
   }
   var_store.destroy_cache (store_cache);
 }

 کينډۍ
 فراغ
 remap_variation_indices<OT::توکی_Variation_Store> (ثابت OT::توکی_Variation_Store &var_store,
                          ثابت hb_سېټ_t &variation_indices,
                          ثابت hb_ویکټور_t<صحيح>& normalized_coords,
                          منطقي calculate_delta, /* not pinned at default */
                          منطقي no_variations, /* all axes pinned */
                          hb_hashmap_t<بینښه, hb_pair_t<بینښه, صحيح>> &variation_idx_delta_map /*بهر*/);

 #که_نه_تعریف_شوی HB_NO_بنسټ
 فراغ
 collect_بنسټ_variation_indices (hb_subset_plan_t* plan)
 {
   hb_blob_ptr_t<OT::بنسټ_4> بنسټ = plan->سرچینه_جدول<OT::بنسټ_4> ();
   إذا (!بنسټ->has_var_store ())
   {
     بنسټ.destroy ();
     أرجع;
   }

   hb_سېټ_t varidx_سېټ;
   بنسټ->collect_variation_indices (plan, varidx_سېټ);
   ثابت OT::توکی_Variation_Store &var_store = بنسټ->get_var_store ();
   بینښه subtable_شمېر = var_store.get_sub_جدول_شمېر ();


   remap_variation_indices (var_store, varidx_سېټ,
                             plan->normalized_coords,
                             !plan->pinned_at_default,
                             plan->all_axes_pinned,
                             plan->بنسټ_variation_idx_map);
   generate_varstore_inner_maps (varidx_سېټ, subtable_شمېر, plan->بنسټ_varstore_inner_maps);

   بنسټ.destroy ();
 }

 #د_که_پای

منطقي
normalize_axes_location (hb_face_t *face, hb_subset_plan_t *plan)
{
  إذا (plan->user_axes_location.is_empty ())
    أرجع ریښتیا;

  hb_array_t<ثابت OT::Axis_ریکارډ> axes = face->جدول.fvar->get_axes ();
  لكل (ثابت خپلکاری &axis : axes)
  {
    hb_نښان_t نښان_2 = axis.get_axis_نښان ();
    ثابت Triple *range;
    عائم minimum, منځ, اعظمي;
    axis.get_coordinates (minimum, منځ, اعظمي);
    إذا (plan->user_axes_location.has (نښان_2, &range) && !range->is_ټکی () &&
        range->minimum == minimum &&
        range->منځ == منځ &&
        range->اعظمي == اعظمي)
      plan->user_axes_location.del (نښان_2);
  }
  إذا (plan->user_axes_location.is_empty ())
    أرجع ریښتیا;

  إذا (!plan->check_success (plan->normalized_coords.resize (axes.اوږدتیا)))
    أرجع خطأ;

  منطقي has_avar = face->جدول.avar->has_معلومات ();
  hb_ویکټور_t<عائم> normalized_mins;
  hb_ویکټور_t<عائم> normalized_defaults;
  hb_ویکټور_t<عائم> normalized_maxs;
  إذا (has_avar)
  {
    إذا (!plan->check_success (normalized_mins.resize (axes.اوږدتیا)) ||
        !plan->check_success (normalized_defaults.resize (axes.اوږدتیا)) ||
        !plan->check_success (normalized_maxs.resize (axes.اوږدتیا)))
      أرجع خطأ;
  }

  منطقي axis_not_pinned = خطأ;
  بینښه new_axis_idx = 0;
  بینښه وروستی_idx = 0;
  لكل (ثابت خپلکاری& _ : + hb_enumerate (axes))
  {
    بینښه i = _.لومړی;
    ثابت OT::Axis_ریکارډ &axis = _.second;
    hb_نښان_t axis_نښان_2 = axis.get_axis_نښان ();
    plan->axes_old_شاخص_نښان_map.سېټ (i, axis_نښان_2);

    إذا (!plan->user_axes_location.has (axis_نښان_2) ||
        !plan->user_axes_location.get (axis_نښان_2).is_ټکی ())
    {
      axis_not_pinned = ریښتیا;
      plan->axes_شاخص_map.سېټ (i, new_axis_idx);
      plan->axis_نښانونه.push (axis_نښان_2);
      new_axis_idx++;
    }

    Triple *axis_range;
    إذا (plan->user_axes_location.has (axis_نښان_2, &axis_range))
    {
      plan->axes_triple_distances.سېټ (axis_نښان_2, axis.get_triple_distances ());

      عائم normalized_min = axis.normalize_axis_ارزښت (axis_range->minimum);
      عائم normalized_default = axis.normalize_axis_ارزښت (axis_range->منځ);
      عائم normalized_max = axis.normalize_axis_ارزښت (axis_range->اعظمي);

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
        وروستی_idx = i;
      }
      وإلا
      {
        plan->axes_location.سېټ (axis_نښان_2, Triple ((مزدوج) normalized_min,
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
    بینښه coords_len = وروستی_idx + 1;

    إذا (avar_جدول->has_v2_معلومات () && !plan->all_axes_pinned)
    {
      /* avar2 partial instancing: use v1-only mapping to get intermediate-space
       * coords. These are used for IVS rebasing and offset compensation. */
      plan->has_avar2 = ریښتیا;

      إذا (!plan->check_success (avar_جدول->map_coords_2_14 (normalized_mins.arrayZ, coords_len, ریښتیا)) ||
          !plan->check_success (avar_جدول->map_coords_2_14 (normalized_defaults.arrayZ, coords_len, ریښتیا)) ||
          !plan->check_success (avar_جدول->map_coords_2_14 (normalized_maxs.arrayZ, coords_len, ریښتیا)))
        أرجع خطأ;

      لكل (ثابت خپلکاری& _ : + hb_enumerate (axes))
      {
        بینښه i = _.لومړی;
        hb_نښان_t axis_نښان_2 = _.second.get_axis_نښان ();
        إذا (plan->user_axes_location.has (axis_نښان_2))
        {
          /* Store intermediate-space coords for offset compensation,
           * segment-map renormalization, and IVS rebasing. */
          plan->old_intermediates.سېټ (axis_نښان_2, Triple ((مزدوج) normalized_mins[i],
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
      hb_blob_t *cff2_blob = hb_face_reference_جدول (face, HB_نښان ('C','F','F','2'));
      منطقي detect_self_contained = !hb_blob_get_اوږدتیا (cff2_blob);
      hb_blob_destroy (cff2_blob);
      إذا (!_compute_avar2_reachable_ranges (plan, axes, avar_جدول,
                                            detect_self_contained))
        أرجع خطأ;

#که_نه_تعریف_شوی HB_NO_VAR_COMPOSITES
      /* VARC overrides can reach coordinates outside the font-level
       * ranges. Do not cull variation data using those ranges. */
      إذا (!plan->drop_tables.has (HB_نښان ('V','A','R','C')))
      {
        hb_blob_t *varc_blob = hb_face_reference_جدول (face, HB_نښان ('V','A','R','C'));
        إذا (hb_blob_get_اوږدتیا (varc_blob))
          plan->avar2_reachable_ranges.reset ();
        hb_blob_destroy (varc_blob);
      }
#د_که_پای

      /* Keep all axes in fvar (pinned ones as hidden), EXCEPT self-contained
       * pinned axes, which are removed entirely. */
      plan->axes_شاخص_map.reset ();
      plan->axis_نښانونه.reset ();
      بینښه retained_axis_idx = 0;
      لكل (ثابت خپلکاری& _ : + hb_enumerate (axes))
      {
        بینښه i = _.لومړی;
        hb_نښان_t axis_نښان_2 = _.second.get_axis_نښان ();
        إذا (plan->avar2_self_contained.has (axis_نښان_2))
          استمر;
        plan->axes_شاخص_map.سېټ (i, retained_axis_idx++);
        plan->axis_نښانونه.push (axis_نښان_2);
      }
      /* The other tables see ONLY the self-contained pins, as an ordinary
       * partial instancing in old final-coordinate space; the standard
       * instancing machinery bakes those axes' contributions in. The
       * remaining restriction is carried entirely by avar. */
      plan->axes_location.reset ();
      إذا (plan->avar2_self_contained.get_population ())
      {
        منطقي sc_pinned_at_default = ریښتیا;
        لكل (خپلکاری _ : plan->avar2_self_contained)
        {
          plan->axes_location.سېټ (_.لومړی, Triple (_.second, _.second, _.second));
          إذا (_.second != 0.0)
            sc_pinned_at_default = خطأ;
        }
        لكل (ثابت خپلکاری& _ : + hb_enumerate (axes))
        {
          مزدوج *v;
          plan->normalized_coords[_.لومړی] =
              plan->avar2_self_contained.has (_.second.get_axis_نښان (), &v)
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

      لكل (ثابت خپلکاری& _ : + hb_enumerate (axes))
      {
        بینښه i = _.لومړی;
        hb_نښان_t axis_نښان_2 = _.second.get_axis_نښان ();
        إذا (plan->user_axes_location.has (axis_نښان_2))
        {
          plan->axes_location.سېټ (axis_نښان_2, Triple ((مزدوج) normalized_mins[i],
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
  أرجع ریښتیا;
}

#که_نه_تعریف_شوی HB_NO_OT_FONT_CFF
فراغ
update_instance_metrics_map_from_cff2 (hb_subset_plan_t *plan)
{
  إذا (!plan->normalized_coords) أرجع;
  OT::cff2::accelerator_t cff2 (plan->سرچینه);
  إذا (!cff2.is_valid ()) أرجع;

  hb_font_t *font = _get_hb_font_with_variations (plan);
  إذا (unlikely (!plan->check_success (font != تش_اشاره)))
  {
    hb_font_destroy (font);
    أرجع;
  }

  hb_glyph_extents_t extents = {0x7FFF, -0x7FFF};
  OT::hmtx_accelerator_t _hmtx (plan->سرچینه);
  OT::hb_scalar_cache_t *hvar_store_cache = تش_اشاره;
  إذا (_hmtx.has_معلومات () && _hmtx.var_جدول.get_اوږدتیا ())
    hvar_store_cache = _hmtx.var_جدول->get_var_store ().create_cache ();

  OT::vmtx_accelerator_t _vmtx (plan->سرچینه);
  OT::hb_scalar_cache_t *vvar_store_cache = تش_اشاره;
  إذا (_vmtx.has_معلومات () && _vmtx.var_جدول.get_اوږدتیا ())
    vvar_store_cache = _vmtx.var_جدول->get_var_store ().create_cache ();

  لكل (خپلکاری p : *plan->glyph_map)
  {
    hb_codepoint_t old_gid = p.لومړی;
    hb_codepoint_t new_gid = p.second;
    إذا (!cff2.get_extents (font, old_gid, &extents)) استمر;
    منطقي has_bounds_info = ریښتیا;
    إذا (extents.x_bearing == 0 && extents.پلنوالی == 0 &&
        extents.لوړوالی == 0 && extents.y_bearing == 0)
      has_bounds_info = خطأ;

    إذا (has_bounds_info)
    {
      plan->سر_maxp_info.xMin = hb_min (plan->سر_maxp_info.xMin, extents.x_bearing);
      plan->سر_maxp_info.xMax = hb_max (plan->سر_maxp_info.xMax,
						 hb_saturate_جمع_کړه (extents.x_bearing, extents.پلنوالی));
      plan->سر_maxp_info.yMax = hb_max (plan->سر_maxp_info.yMax, extents.y_bearing);
      plan->سر_maxp_info.yMin = hb_min (plan->سر_maxp_info.yMin,
						 hb_saturate_جمع_کړه (extents.y_bearing, extents.لوړوالی));
    }

    إذا (_hmtx.has_معلومات ())
    {
      مزدوج hori_aw = _hmtx.get_advance_without_var_unscaled (old_gid);
      إذا (_hmtx.var_جدول.get_اوږدتیا ())
        hori_aw += (مزدوج) roundf (_hmtx.var_جدول->get_advance_delta_unscaled (old_gid, font->coords, font->عدد_coords,
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
      plan->hmtx_map.سېټ (new_gid, hb_pair ((بینښه) hori_aw, lsb));
      plan->bounds_پلنوالی_vec[new_gid] = extents.پلنوالی;
    }

    إذا (_vmtx.has_معلومات ())
    {
      مزدوج vert_aw = _vmtx.get_advance_without_var_unscaled (old_gid);
      إذا (_vmtx.var_جدول.get_اوږدتیا ())
        vert_aw += (مزدوج) roundf (_vmtx.var_جدول->get_advance_delta_unscaled (old_gid, font->coords, font->عدد_coords,
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

      plan->vmtx_map.سېټ (new_gid, hb_pair ((بینښه) vert_aw, tsb));
      plan->bounds_لوړوالی_vec[new_gid] = extents.لوړوالی;
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
#د_که_پای

منطقي
get_instance_glyphs_contour_points (hb_subset_plan_t *plan)
{
  /* contour_points vector only needed for updating gvar table (infer delta and
   * iup delta optimization) during partial instancing */
  إذا (plan->user_axes_location.is_empty () || plan->all_axes_pinned)
    أرجع ریښتیا;

  OT::glyf_accelerator_t glyf (plan->سرچینه);

  لكل (خپلکاری &_ : plan->new_منزل_old_gid_list)
  {
    hb_codepoint_t new_gid = _.لومړی;
    contour_ټکی_ویکټور_t all_points;
    إذا (new_gid == 0 && !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      إذا (unlikely (!plan->new_gid_contour_points_map.سېټ (new_gid, all_points)))
        أرجع خطأ;
      استمر;
    }

    hb_codepoint_t old_gid = _.second;
    خپلکاری glyph = glyf.glyph_for_gid (old_gid);
    إذا (unlikely (!glyph.get_all_points_without_var (plan->سرچینه, all_points)))
      أرجع خطأ;
    إذا (unlikely (!plan->new_gid_contour_points_map.سېټ (new_gid, all_points)))
      أرجع خطأ;

    /* composite new gids are only needed by iup delta optimization */
    إذا ((plan->flags & HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS) && glyph.is_مرکب ())
      plan->مرکب_new_gids.جمع_کړه (new_gid);
  }
  أرجع ریښتیا;
}

کينډۍ<ډول_نوم Delta_سېټ_شاخص_Map>
فراغ
remap_colrv1_delta_سېټ_شاخص_indices (ثابت Delta_سېټ_شاخص_Map &شاخص_map,
                                      ثابت hb_سېټ_t &delta_سېټ_idxes,
                                      hb_hashmap_t<بینښه, hb_pair_t<بینښه, صحيح>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /*بهر*/)
{
  إذا (!شاخص_map.get_map_شمېر ())
    أرجع;

  hb_hashmap_t<بینښه, hb_pair_t<بینښه, صحيح>> delta_سېټ_idx_delta_map;
  بینښه new_delta_سېټ_idx = 0;
  لكل (بینښه delta_سېټ_idx : delta_سېټ_idxes)
  {
    بینښه var_idx = شاخص_map.map (delta_سېټ_idx);
    بینښه new_varidx = HB_OT_LAYOUT_NO_VARIATIONS_شاخص;
    صحيح delta = 0;

    إذا (var_idx != HB_OT_LAYOUT_NO_VARIATIONS_شاخص)
    {
      hb_pair_t<بینښه, صحيح> *new_varidx_delta;
      إذا (!variation_idx_delta_map.has (var_idx, &new_varidx_delta)) استمر;

      new_varidx = hb_لومړی (*new_varidx_delta);
      delta = hb_دوهم (*new_varidx_delta);
    }

    new_deltaset_idx_varidx_map.سېټ (new_delta_سېټ_idx, new_varidx);
    delta_سېټ_idx_delta_map.سېټ (delta_سېټ_idx, hb_pair_t<بینښه, صحيح> (new_delta_سېټ_idx, delta));
    new_delta_سېټ_idx++;
  }
  variation_idx_delta_map = std::move (delta_سېټ_idx_delta_map);
}

کينډۍ فراغ
remap_colrv1_delta_سېټ_شاخص_indices<OT::Delta_سېټ_شاخص_Map> (ثابت OT::Delta_سېټ_شاخص_Map &شاخص_map,
                                      ثابت hb_سېټ_t &delta_سېټ_idxes,
                                      hb_hashmap_t<بینښه, hb_pair_t<بینښه, صحيح>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /*بهر*/);

 #د_که_پای

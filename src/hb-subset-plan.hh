/*
 * Copyright © 2018  Google, Inc.
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
 * Google Author(s): Garret Rieger, Roderick Sheeter
 */

#اگر_تعریف_نشده HB_SUBSET_PLAN_HH
#تعریف_کن HB_SUBSET_PLAN_HH

#ضمن "hb.hh"

#ضمن "hb-subset.h"
#ضمن "hb-subset-input.hh"
#ضمن "hb-subset-accelerator.hh"

#ضمن "hb-map.hh"
#ضمن "hb-bimap.hh"
#ضمن "hb-set.hh"

نطاق OT {
بنية Feature;
}

بنية os2_info_t {
  hb_codepoint_t min_cmap_codepoint;
  hb_codepoint_t max_cmap_codepoint;
};

تعريفنوع بنية os2_info_t os2_info_t;

بنية سر_maxp_info_t
{
  سر_maxp_info_t ()
      :xMin (0x7FFF), xMax (-0x7FFF), yMin (0x7FFF), yMax (-0x7FFF),
      maxPoints (0), maxContours (0),
      max_مرکب_Points (0),
      max_مرکب_Contours (0),
      maxComponentElements (0),
      max_Component_عمق (0),
      allXMinIsLsb (درست) {}

  صحيح xMin;
  صحيح xMax;
  صحيح yMin;
  صحيح yMax;
  بیعلامت maxPoints;
  بیعلامت maxContours;
  بیعلامت max_مرکب_Points;
  بیعلامت max_مرکب_Contours;
  بیعلامت maxComponentElements;
  بیعلامت max_Component_عمق;
  منطقي allXMinIsLsb;
};

تعريفنوع بنية سر_maxp_info_t سر_maxp_info_t;

بنية contour_نقطه_t
{
  فراغ init (عائم x_ = 0.f, عائم y_ = 0.f, منطقي is_انتها_نقطه_2 = خطأ)
  { flag = 0; x = x_; y = y_; is_انتها_نقطه = is_انتها_نقطه_2; }

  فراغ transform (ثابت عائم (&ماتریس)[4])
  {
    عائم x_ = x * ماتریس[0] + y * ماتریس[2];
	  y  = x * ماتریس[1] + y * ماتریس[3];
    x  = x_;
  }

  فراغ جمع_کن_delta (عائم delta_x, عائم delta_y)
  {
    x += delta_x;
    y += delta_y;
  }

  HB_ALWAYS_INLINE
  فراغ translate (ثابت contour_نقطه_t &p) { x += p.x; y += p.y; }

  عائم x;
  عائم y;
  uint8_t flag;
  منطقي is_انتها_نقطه;
};

بنية contour_نقطه_وکتور_t : hb_وکتور_t<contour_نقطه_t>
{
  منطقي جمع_کن_deltas (hb_array_t<ثابت عائم> deltas_x,
                   hb_array_t<ثابت عائم> deltas_y,
                   hb_array_t<ثابت منطقي> indices)
  {
    إذا (indices.درازی != deltas_x.درازی ||
        indices.درازی != deltas_y.درازی)
      أرجع خطأ;

    لكل (بیعلامت i = 0; i < indices.درازی; i++)
    {
      إذا (!indices.arrayZ[i]) استمر;
      arrayZ[i].جمع_کن_delta (deltas_x.arrayZ[i], deltas_y.arrayZ[i]);
    }
    أرجع درست;
  }
};

نطاق OT {
  بنية cff1_subset_accelerator_t;
  بنية cff2_subset_accelerator_t;
}

بنية hb_subset_plan_t
{
  HB_داخلی hb_subset_plan_t (hb_face_t *,
				ثابت hb_subset_درون_داد_t *درون_داد);

  HB_داخلی ~hb_subset_plan_t();

  hb_object_سرآیند_t سرآیند;

  منطقي successful;
  بیعلامت flags;
  منطقي attach_accelerator_داده = خطأ;
  منطقي force_long_loca = خطأ;

  // The glyph subset
  hb_map_t *codepoint_مقصد_glyph; // Needs to be heap-allocated

  // Old -> New glyph id mapping
  hb_map_t *glyph_map; // Needs to be heap-allocated
  hb_map_t *معکوس_glyph_map; // Needs to be heap-allocated

  // Plan is only good for a specific source/dest so keep them with it
  hb_face_t *منبع;
#اگر_تعریف_نشده HB_NO_SUBSET_CFF
  // These have to be immediately after source:
  hb_face_lazy_loader_t<OT::cff1_subset_accelerator_t, 1> cff1_accel;
  hb_face_lazy_loader_t<OT::cff2_subset_accelerator_t, 2> cff2_accel;
#پایان_اگر

  hb_face_t *dest;

  بیعلامت صحيح رقم_output_glyphs_2;

  منطقي all_axes_pinned;
  منطقي pinned_at_default;
  منطقي has_seac;

  // whether to insert a catch-all FeatureVariationRecord
  منطقي gsub_درج_catch_all_feature_variation_rec;
  منطقي gpos_درج_catch_all_feature_variation_rec;

  // whether GDEF ItemVariationStore is retained
  تغییرپذیر منطقي has_gdef_varstore;

  // whether we're doing avar2 partial instancing
  منطقي has_avar2;

#تعریف_کن HB_SUBSET_PLAN_MEMBER(Type, Name) Type Name;
#ضمن "hb-subset-plan-member-list.hh"
#لغو_تعریف HB_SUBSET_PLAN_MEMBER

  //recalculated head/maxp table info after instancing
  تغییرپذیر سر_maxp_info_t سر_maxp_info;

  os2_info_t os2_info;

  ثابت hb_subset_accelerator_t* accelerator = اشاره_تهی;
  hb_subset_accelerator_t* inprogress_accelerator = اشاره_تهی;

 عام:

  قالب<نام_نوع T>
  بنية منبع_جدول_loader
  {
    hb_blob_ptr_t<T> عملگر () (hb_subset_plan_t *plan)
    {
      hb_قفل_t lock (plan->accelerator ? &plan->accelerator->sanitized_جدول_cache_قفل : اشاره_تهی);

      خودکار *cache = plan->accelerator ? &plan->accelerator->sanitized_جدول_cache : &plan->sanitized_جدول_cache;
      إذا (cache
	  && !cache->درون_error ()
	  && cache->has (+T::جدول_برچسب)) {
	أرجع hb_blob_reference (cache->get (+T::جدول_برچسب).get ());
      }

      hb::unique_ptr<hb_blob_t> جدول_blob {hb_sanitize_context_t ().reference_جدول<T> (plan->منبع)};
      hb_blob_t* ret = hb_blob_reference (جدول_blob.get ());

      إذا (likely (cache))
	cache->مجموعه (+T::جدول_برچسب, std::move (جدول_blob));

      أرجع ret;
    }
  };

  قالب<نام_نوع T>
  خودکار منبع_جدول() HB_AUTO_RETURN (منبع_جدول_loader<T> {} (هذا))

  منطقي درون_error () ثابت { أرجع !successful; }

  HB_داخلی مزدوج layout_variation_delta (بیعلامت var_idx);

  منطقي check_success(منطقي success)
  {
    successful = (successful && success);
    أرجع successful;
  }

  /*
   * The set of input glyph ids which will be retained in the subset.
   * Does NOT include ids kept due to retain_gids. You probably want to use
   * glyph_map/reverse_glyph_map.
   */
  مضمن ثابت hb_مجموعه_t *
  glyphset () ثابت
  {
    أرجع &_glyphset;
  }

  /*
   * The set of input glyph ids which will be retained in the subset.
   */
  مضمن ثابت hb_مجموعه_t *
  glyphset_gsub () ثابت
  {
    أرجع &_glyphset_gsub;
  }

  /*
   * Old-to-new glyph id mapping for GSUB glyphs, through the flat
   * array; returns HB_MAP_VALUE_INVALID for glyphs not retained.
   */
  مضمن hb_codepoint_t
  map_gsub_glyph (hb_codepoint_t old_gid) ثابت
  {
    أرجع old_gid < glyph_map_gsub_flat.درازی
	 ? glyph_map_gsub_flat.arrayZ[old_gid]
	 : HB_MAP_مقدار_INVALID;
  }

  /*
   * The total number of output glyphs in the final subset.
   */
  مضمن بیعلامت صحيح
  رقم_output_glyphs () ثابت
  {
    أرجع رقم_output_glyphs_2;
  }

  مضمن منطقي new_gid_for_codepoint (hb_codepoint_t codepoint,
				     hb_codepoint_t *new_gid) ثابت
  {
    hb_codepoint_t old_gid = codepoint_مقصد_glyph->get (codepoint);
    إذا (old_gid == HB_MAP_مقدار_INVALID)
      أرجع خطأ;

    أرجع new_gid_for_old_gid (old_gid, new_gid);
  }

  مضمن منطقي new_gid_for_old_gid (hb_codepoint_t old_gid,
				   hb_codepoint_t *new_gid) ثابت
  {
    hb_codepoint_t gid = glyph_map->get (old_gid);
    إذا (gid == HB_MAP_مقدار_INVALID)
      أرجع خطأ;

    *new_gid = gid;
    أرجع درست;
  }

  مضمن منطقي old_gid_for_new_gid (hb_codepoint_t  new_gid,
				   hb_codepoint_t *old_gid) ثابت
  {
    hb_codepoint_t gid = معکوس_glyph_map->get (new_gid);
    إذا (gid == HB_MAP_مقدار_INVALID)
      أرجع خطأ;

    *old_gid = gid;
    أرجع درست;
  }

  مضمن منطقي
  جمع_کن_جدول (hb_برچسب_t برچسب_2,
	     hb_blob_t *contents)
  {
    إذا (HB_DEBUG_SUBSET)
    {
      hb_blob_t *منبع_blob = منبع->reference_جدول (برچسب_2);
      DEBUG_پیام(SUBSET, اشاره_تهی, "add table %c%c%c%c, dest %u bytes, source %u bytes",
		HB_UNTAG(برچسب_2),
		hb_blob_get_درازی (contents),
		hb_blob_get_درازی (منبع_blob));
      hb_blob_destroy (منبع_blob);
    }
    أرجع hb_face_builder_جمع_کن_جدول (dest, برچسب_2, contents);
  }
};

// hb-subset-plan implementation is split into multiple files to keep
// compile times more reasonable:
// - hb-subset-plan.cc
// - hb-subset-plan-layout.cc
// - hb-subset-plan-var.cc
//
// The functions below are those needed to connect the split files
// above together.
HB_داخلی فراغ
remap_indexes (ثابت hb_مجموعه_t *indexes,
               hb_map_t       *mapping /*بیرون*/);


#اگر_تعریف_نشده HB_NO_VAR
قالب<نام_نوع قلم_Var_Store>
HB_داخلی فراغ
remap_variation_indices (ثابت قلم_Var_Store &var_store,
                         ثابت hb_مجموعه_t &variation_indices,
                         ثابت hb_وکتور_t<صحيح>& normalized_coords,
                         منطقي calculate_delta, /* not pinned at default */
                         منطقي no_variations, /* all axes pinned */
                         hb_hashmap_t<بیعلامت, hb_pair_t<بیعلامت, صحيح>> &variation_idx_delta_map /*بیرون*/);


قالب<نام_نوع Delta_مجموعه_شاخص_Map>
HB_داخلی فراغ
remap_colrv1_delta_مجموعه_شاخص_indices (ثابت Delta_مجموعه_شاخص_Map &شاخص_map,
                                      ثابت hb_مجموعه_t &delta_مجموعه_idxes,
                                      hb_hashmap_t<بیعلامت, hb_pair_t<بیعلامت, صحيح>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /*بیرون*/);


HB_داخلی فراغ
generate_varstore_inner_maps (ثابت hb_مجموعه_t& varidx_مجموعه,
                              بیعلامت subtable_شمار,
                              hb_وکتور_t<hb_inc_bimap_t> &inner_maps /*بیرون*/);

HB_داخلی منطقي
normalize_axes_location (hb_face_t *face, hb_subset_plan_t *plan);

HB_داخلی فراغ
update_instance_metrics_map_from_cff2 (hb_subset_plan_t *plan);

HB_داخلی منطقي
get_instance_glyphs_contour_points (hb_subset_plan_t *plan);

#اگر_تعریف_نشده HB_NO_پایه
HB_داخلی فراغ
collect_پایه_variation_indices (hb_subset_plan_t* plan);
#پایان_اگر
#پایان_اگر

#اگر_تعریف_نشده HB_NO_SUBSET_LAYOUT
تعريفنوع hb_hashmap_t<بیعلامت, hb::unique_ptr<hb_مجموعه_t>> script_langsys_map;

HB_داخلی فراغ
remap_استفاده_شده_mark_sets (hb_subset_plan_t *plan,
                      hb_map_t& استفاده_شده_mark_sets_map);

HB_داخلی فراغ
layout_nameid_بستگی (hb_subset_plan_t* plan,
                       hb_مجموعه_t* drop_tables);

HB_داخلی فراغ
layout_populate_gids_مقصد_retain (hb_subset_plan_t* plan,
                                hb_مجموعه_t* drop_tables);

HB_داخلی فراغ
collect_layout_variation_indices (hb_subset_plan_t* plan);
#پایان_اگر


#پایان_اگر /* HB_SUBSET_PLAN_HH */

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

#جے_بنا_تعریف HB_SUBSET_PLAN_HH
#تعریف_کرو HB_SUBSET_PLAN_HH

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
      max_Component_ڈونگھائی (0),
      allXMinIsLsb (سچ) {}

  صحيح xMin;
  صحيح xMax;
  صحيح yMin;
  صحيح yMax;
  بنانشانی maxPoints;
  بنانشانی maxContours;
  بنانشانی max_مرکب_Points;
  بنانشانی max_مرکب_Contours;
  بنانشانی maxComponentElements;
  بنانشانی max_Component_ڈونگھائی;
  منطقي allXMinIsLsb;
};

تعريفنوع بنية سر_maxp_info_t سر_maxp_info_t;

بنية contour_نقطہ_t
{
  فراغ init (عائم x_ = 0.f, عائم y_ = 0.f, منطقي is_سرا_نقطہ_2 = خطأ)
  { flag = 0; x = x_; y = y_; is_سرا_نقطہ = is_سرا_نقطہ_2; }

  فراغ transform (ثابت عائم (&میٹرکس)[4])
  {
    عائم x_ = x * میٹرکس[0] + y * میٹرکس[2];
	  y  = x * میٹرکس[1] + y * میٹرکس[3];
    x  = x_;
  }

  فراغ جوڑو_delta (عائم delta_x, عائم delta_y)
  {
    x += delta_x;
    y += delta_y;
  }

  HB_ALWAYS_INLINE
  فراغ translate (ثابت contour_نقطہ_t &p) { x += p.x; y += p.y; }

  عائم x;
  عائم y;
  uint8_t flag;
  منطقي is_سرا_نقطہ;
};

بنية contour_نقطہ_سمتیہ_t : hb_سمتیہ_t<contour_نقطہ_t>
{
  منطقي جوڑو_deltas (hb_array_t<ثابت عائم> deltas_x,
                   hb_array_t<ثابت عائم> deltas_y,
                   hb_array_t<ثابت منطقي> indices)
  {
    إذا (indices.لمائی != deltas_x.لمائی ||
        indices.لمائی != deltas_y.لمائی)
      أرجع خطأ;

    لكل (بنانشانی i = 0; i < indices.لمائی; i++)
    {
      إذا (!indices.arrayZ[i]) استمر;
      arrayZ[i].جوڑو_delta (deltas_x.arrayZ[i], deltas_y.arrayZ[i]);
    }
    أرجع سچ;
  }
};

نطاق OT {
  بنية cff1_subset_accelerator_t;
  بنية cff2_subset_accelerator_t;
}

بنية hb_subset_plan_t
{
  HB_اندرونی hb_subset_plan_t (hb_face_t *,
				ثابت hb_subset_داخلہ_t *داخلہ);

  HB_اندرونی ~hb_subset_plan_t();

  hb_object_سرخی_t سرخی;

  منطقي successful;
  بنانشانی flags;
  منطقي attach_accelerator_انکڑے = خطأ;
  منطقي force_long_loca = خطأ;

  // The glyph subset
  hb_map_t *codepoint_منزل_glyph; // Needs to be heap-allocated

  // Old -> New glyph id mapping
  hb_map_t *glyph_map; // Needs to be heap-allocated
  hb_map_t *الٹ_glyph_map; // Needs to be heap-allocated

  // Plan is only good for a specific source/dest so keep them with it
  hb_face_t *سروت;
#جے_بنا_تعریف HB_NO_SUBSET_CFF
  // These have to be immediately after source:
  hb_face_lazy_loader_t<OT::cff1_subset_accelerator_t, 1> cff1_accel;
  hb_face_lazy_loader_t<OT::cff2_subset_accelerator_t, 2> cff2_accel;
#جے_انت

  hb_face_t *dest;

  بنانشانی صحيح عدد_output_glyphs_2;

  منطقي all_axes_pinned;
  منطقي pinned_at_default;
  منطقي has_seac;

  // whether to insert a catch-all FeatureVariationRecord
  منطقي gsub_داخل_کرو_catch_all_feature_variation_rec;
  منطقي gpos_داخل_کرو_catch_all_feature_variation_rec;

  // whether GDEF ItemVariationStore is retained
  بدلنجوگ منطقي has_gdef_varstore;

  // whether we're doing avar2 partial instancing
  منطقي has_avar2;

#تعریف_کرو HB_SUBSET_PLAN_MEMBER(Type, Name) Type Name;
#ضمن "hb-subset-plan-member-list.hh"
#تعریف_رد HB_SUBSET_PLAN_MEMBER

  //recalculated head/maxp table info after instancing
  بدلنجوگ سر_maxp_info_t سر_maxp_info;

  os2_info_t os2_info;

  ثابت hb_subset_accelerator_t* accelerator = خالی_سوچک;
  hb_subset_accelerator_t* inprogress_accelerator = خالی_سوچک;

 عام:

  سانچا<قسم_ناں T>
  بنية سروت_جدول_loader
  {
    hb_blob_ptr_t<T> عامل () (hb_subset_plan_t *plan)
    {
      hb_جندرا_t lock (plan->accelerator ? &plan->accelerator->sanitized_جدول_cache_جندرا : خالی_سوچک);

      خودکار *cache = plan->accelerator ? &plan->accelerator->sanitized_جدول_cache : &plan->sanitized_جدول_cache;
      إذا (cache
	  && !cache->وچ_error ()
	  && cache->has (+T::جدول_ٹیگ)) {
	أرجع hb_blob_reference (cache->get (+T::جدول_ٹیگ).get ());
      }

      hb::unique_ptr<hb_blob_t> جدول_blob {hb_sanitize_context_t ().reference_جدول<T> (plan->سروت)};
      hb_blob_t* ret = hb_blob_reference (جدول_blob.get ());

      إذا (likely (cache))
	cache->سیٹ (+T::جدول_ٹیگ, std::move (جدول_blob));

      أرجع ret;
    }
  };

  سانچا<قسم_ناں T>
  خودکار سروت_جدول() HB_AUTO_RETURN (سروت_جدول_loader<T> {} (هذا))

  منطقي وچ_error () ثابت { أرجع !successful; }

  HB_اندرونی مزدوج layout_variation_delta (بنانشانی var_idx);

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
  مضمن ثابت hb_سیٹ_t *
  glyphset () ثابت
  {
    أرجع &_glyphset;
  }

  /*
   * The set of input glyph ids which will be retained in the subset.
   */
  مضمن ثابت hb_سیٹ_t *
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
    أرجع old_gid < glyph_map_gsub_flat.لمائی
	 ? glyph_map_gsub_flat.arrayZ[old_gid]
	 : HB_MAP_قدر_INVALID;
  }

  /*
   * The total number of output glyphs in the final subset.
   */
  مضمن بنانشانی صحيح
  عدد_output_glyphs () ثابت
  {
    أرجع عدد_output_glyphs_2;
  }

  مضمن منطقي new_gid_for_codepoint (hb_codepoint_t codepoint,
				     hb_codepoint_t *new_gid) ثابت
  {
    hb_codepoint_t old_gid = codepoint_منزل_glyph->get (codepoint);
    إذا (old_gid == HB_MAP_قدر_INVALID)
      أرجع خطأ;

    أرجع new_gid_for_old_gid (old_gid, new_gid);
  }

  مضمن منطقي new_gid_for_old_gid (hb_codepoint_t old_gid,
				   hb_codepoint_t *new_gid) ثابت
  {
    hb_codepoint_t gid = glyph_map->get (old_gid);
    إذا (gid == HB_MAP_قدر_INVALID)
      أرجع خطأ;

    *new_gid = gid;
    أرجع سچ;
  }

  مضمن منطقي old_gid_for_new_gid (hb_codepoint_t  new_gid,
				   hb_codepoint_t *old_gid) ثابت
  {
    hb_codepoint_t gid = الٹ_glyph_map->get (new_gid);
    إذا (gid == HB_MAP_قدر_INVALID)
      أرجع خطأ;

    *old_gid = gid;
    أرجع سچ;
  }

  مضمن منطقي
  جوڑو_جدول (hb_ٹیگ_t ٹیگ_2,
	     hb_blob_t *contents)
  {
    إذا (HB_DEBUG_SUBSET)
    {
      hb_blob_t *سروت_blob = سروت->reference_جدول (ٹیگ_2);
      DEBUG_پیغام(SUBSET, خالی_سوچک, "add table %c%c%c%c, dest %u bytes, source %u bytes",
		HB_UNTAG(ٹیگ_2),
		hb_blob_get_لمائی (contents),
		hb_blob_get_لمائی (سروت_blob));
      hb_blob_destroy (سروت_blob);
    }
    أرجع hb_face_builder_جوڑو_جدول (dest, ٹیگ_2, contents);
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
HB_اندرونی فراغ
remap_indexes (ثابت hb_سیٹ_t *indexes,
               hb_map_t       *mapping /*باہرلا*/);


#جے_بنا_تعریف HB_NO_VAR
سانچا<قسم_ناں شے_Var_Store>
HB_اندرونی فراغ
remap_variation_indices (ثابت شے_Var_Store &var_store,
                         ثابت hb_سیٹ_t &variation_indices,
                         ثابت hb_سمتیہ_t<صحيح>& normalized_coords,
                         منطقي calculate_delta, /* not pinned at default */
                         منطقي no_variations, /* all axes pinned */
                         hb_hashmap_t<بنانشانی, hb_pair_t<بنانشانی, صحيح>> &variation_idx_delta_map /*باہرلا*/);


سانچا<قسم_ناں Delta_سیٹ_اشاریہ_Map>
HB_اندرونی فراغ
remap_colrv1_delta_سیٹ_اشاریہ_indices (ثابت Delta_سیٹ_اشاریہ_Map &اشاریہ_map,
                                      ثابت hb_سیٹ_t &delta_سیٹ_idxes,
                                      hb_hashmap_t<بنانشانی, hb_pair_t<بنانشانی, صحيح>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /*باہرلا*/);


HB_اندرونی فراغ
generate_varstore_inner_maps (ثابت hb_سیٹ_t& varidx_سیٹ,
                              بنانشانی subtable_گنتی,
                              hb_سمتیہ_t<hb_inc_bimap_t> &inner_maps /*باہرلا*/);

HB_اندرونی منطقي
normalize_axes_location (hb_face_t *face, hb_subset_plan_t *plan);

HB_اندرونی فراغ
update_instance_metrics_map_from_cff2 (hb_subset_plan_t *plan);

HB_اندرونی منطقي
get_instance_glyphs_contour_points (hb_subset_plan_t *plan);

#جے_بنا_تعریف HB_NO_ادھار
HB_اندرونی فراغ
collect_ادھار_variation_indices (hb_subset_plan_t* plan);
#جے_انت
#جے_انت

#جے_بنا_تعریف HB_NO_SUBSET_LAYOUT
تعريفنوع hb_hashmap_t<بنانشانی, hb::unique_ptr<hb_سیٹ_t>> script_langsys_map;

HB_اندرونی فراغ
remap_ورتیا_mark_sets (hb_subset_plan_t *plan,
                      hb_map_t& ورتیا_mark_sets_map);

HB_اندرونی فراغ
layout_nameid_گھیرا (hb_subset_plan_t* plan,
                       hb_سیٹ_t* drop_tables);

HB_اندرونی فراغ
layout_populate_gids_منزل_retain (hb_subset_plan_t* plan,
                                hb_سیٹ_t* drop_tables);

HB_اندرونی فراغ
collect_layout_variation_indices (hb_subset_plan_t* plan);
#جے_انت


#جے_انت /* HB_SUBSET_PLAN_HH */

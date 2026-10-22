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

#که_نه_تعریف_شوی HB_SUBSET_PLAN_HH
#تعریف_کړه HB_SUBSET_PLAN_HH

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
      max_Component_ژوروالی (0),
      allXMinIsLsb (ریښتیا) {}

  صحيح xMin;
  صحيح xMax;
  صحيح yMin;
  صحيح yMax;
  بینښه maxPoints;
  بینښه maxContours;
  بینښه max_مرکب_Points;
  بینښه max_مرکب_Contours;
  بینښه maxComponentElements;
  بینښه max_Component_ژوروالی;
  منطقي allXMinIsLsb;
};

تعريفنوع بنية سر_maxp_info_t سر_maxp_info_t;

بنية contour_ټکی_t
{
  فراغ init (عائم x_ = 0.f, عائم y_ = 0.f, منطقي is_پای_ټکی_ټکی_2 = خطأ)
  { flag = 0; x = x_; y = y_; is_پای_ټکی_ټکی = is_پای_ټکی_ټکی_2; }

  فراغ transform (ثابت عائم (&ماتریکس)[4])
  {
    عائم x_ = x * ماتریکس[0] + y * ماتریکس[2];
	  y  = x * ماتریکس[1] + y * ماتریکس[3];
    x  = x_;
  }

  فراغ جمع_کړه_delta (عائم delta_x, عائم delta_y)
  {
    x += delta_x;
    y += delta_y;
  }

  HB_ALWAYS_INLINE
  فراغ translate (ثابت contour_ټکی_t &p) { x += p.x; y += p.y; }

  عائم x;
  عائم y;
  uint8_t flag;
  منطقي is_پای_ټکی_ټکی;
};

بنية contour_ټکی_ویکټور_t : hb_ویکټور_t<contour_ټکی_t>
{
  منطقي جمع_کړه_deltas (hb_array_t<ثابت عائم> deltas_x,
                   hb_array_t<ثابت عائم> deltas_y,
                   hb_array_t<ثابت منطقي> indices)
  {
    إذا (indices.اوږدتیا != deltas_x.اوږدتیا ||
        indices.اوږدتیا != deltas_y.اوږدتیا)
      أرجع خطأ;

    لكل (بینښه i = 0; i < indices.اوږدتیا; i++)
    {
      إذا (!indices.arrayZ[i]) استمر;
      arrayZ[i].جمع_کړه_delta (deltas_x.arrayZ[i], deltas_y.arrayZ[i]);
    }
    أرجع ریښتیا;
  }
};

نطاق OT {
  بنية cff1_subset_accelerator_t;
  بنية cff2_subset_accelerator_t;
}

بنية hb_subset_plan_t
{
  HB_دننی hb_subset_plan_t (hb_face_t *,
				ثابت hb_subset_ننوت_t *ننوت);

  HB_دننی ~hb_subset_plan_t();

  hb_object_سرلیک_t سرلیک;

  منطقي successful;
  بینښه flags;
  منطقي attach_accelerator_معلومات = خطأ;
  منطقي force_long_loca = خطأ;

  // The glyph subset
  hb_map_t *codepoint_منزل_glyph; // Needs to be heap-allocated

  // Old -> New glyph id mapping
  hb_map_t *glyph_map; // Needs to be heap-allocated
  hb_map_t *سرچپه_glyph_map; // Needs to be heap-allocated

  // Plan is only good for a specific source/dest so keep them with it
  hb_face_t *سرچینه;
#که_نه_تعریف_شوی HB_NO_SUBSET_CFF
  // These have to be immediately after source:
  hb_face_lazy_loader_t<OT::cff1_subset_accelerator_t, 1> cff1_accel;
  hb_face_lazy_loader_t<OT::cff2_subset_accelerator_t, 2> cff2_accel;
#د_که_پای

  hb_face_t *dest;

  بینښه صحيح عدد_output_glyphs_2;

  منطقي all_axes_pinned;
  منطقي pinned_at_default;
  منطقي has_seac;

  // whether to insert a catch-all FeatureVariationRecord
  منطقي gsub_ورننباسه_catch_all_feature_variation_rec;
  منطقي gpos_ورننباسه_catch_all_feature_variation_rec;

  // whether GDEF ItemVariationStore is retained
  بدلیدونکی منطقي has_gdef_varstore;

  // whether we're doing avar2 partial instancing
  منطقي has_avar2;

#تعریف_کړه HB_SUBSET_PLAN_MEMBER(Type, Name) Type Name;
#ضمن "hb-subset-plan-member-list.hh"
#تعریف_لغوه HB_SUBSET_PLAN_MEMBER

  //recalculated head/maxp table info after instancing
  بدلیدونکی سر_maxp_info_t سر_maxp_info;

  os2_info_t os2_info;

  ثابت hb_subset_accelerator_t* accelerator = تش_اشاره;
  hb_subset_accelerator_t* inprogress_accelerator = تش_اشاره;

 عام:

  کينډۍ<ډول_نوم T>
  بنية سرچینه_جدول_loader
  {
    hb_blob_ptr_t<T> چلوونکی () (hb_subset_plan_t *plan)
    {
      hb_قلف_t lock (plan->accelerator ? &plan->accelerator->sanitized_جدول_cache_قلف : تش_اشاره);

      خپلکاری *cache = plan->accelerator ? &plan->accelerator->sanitized_جدول_cache : &plan->sanitized_جدول_cache;
      إذا (cache
	  && !cache->دننه_error ()
	  && cache->has (+T::جدول_نښان)) {
	أرجع hb_blob_reference (cache->get (+T::جدول_نښان).get ());
      }

      hb::unique_ptr<hb_blob_t> جدول_blob {hb_sanitize_context_t ().reference_جدول<T> (plan->سرچینه)};
      hb_blob_t* ret = hb_blob_reference (جدول_blob.get ());

      إذا (likely (cache))
	cache->سېټ (+T::جدول_نښان, std::move (جدول_blob));

      أرجع ret;
    }
  };

  کينډۍ<ډول_نوم T>
  خپلکاری سرچینه_جدول() HB_AUTO_RETURN (سرچینه_جدول_loader<T> {} (هذا))

  منطقي دننه_error () ثابت { أرجع !successful; }

  HB_دننی مزدوج layout_variation_delta (بینښه var_idx);

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
  مضمن ثابت hb_سېټ_t *
  glyphset () ثابت
  {
    أرجع &_glyphset;
  }

  /*
   * The set of input glyph ids which will be retained in the subset.
   */
  مضمن ثابت hb_سېټ_t *
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
    أرجع old_gid < glyph_map_gsub_flat.اوږدتیا
	 ? glyph_map_gsub_flat.arrayZ[old_gid]
	 : HB_MAP_ارزښت_INVALID;
  }

  /*
   * The total number of output glyphs in the final subset.
   */
  مضمن بینښه صحيح
  عدد_output_glyphs () ثابت
  {
    أرجع عدد_output_glyphs_2;
  }

  مضمن منطقي new_gid_for_codepoint (hb_codepoint_t codepoint,
				     hb_codepoint_t *new_gid) ثابت
  {
    hb_codepoint_t old_gid = codepoint_منزل_glyph->get (codepoint);
    إذا (old_gid == HB_MAP_ارزښت_INVALID)
      أرجع خطأ;

    أرجع new_gid_for_old_gid (old_gid, new_gid);
  }

  مضمن منطقي new_gid_for_old_gid (hb_codepoint_t old_gid,
				   hb_codepoint_t *new_gid) ثابت
  {
    hb_codepoint_t gid = glyph_map->get (old_gid);
    إذا (gid == HB_MAP_ارزښت_INVALID)
      أرجع خطأ;

    *new_gid = gid;
    أرجع ریښتیا;
  }

  مضمن منطقي old_gid_for_new_gid (hb_codepoint_t  new_gid,
				   hb_codepoint_t *old_gid) ثابت
  {
    hb_codepoint_t gid = سرچپه_glyph_map->get (new_gid);
    إذا (gid == HB_MAP_ارزښت_INVALID)
      أرجع خطأ;

    *old_gid = gid;
    أرجع ریښتیا;
  }

  مضمن منطقي
  جمع_کړه_جدول (hb_نښان_t نښان_2,
	     hb_blob_t *contents)
  {
    إذا (HB_DEBUG_SUBSET)
    {
      hb_blob_t *سرچینه_blob = سرچینه->reference_جدول (نښان_2);
      DEBUG_پیغام(SUBSET, تش_اشاره, "add table %c%c%c%c, dest %u bytes, source %u bytes",
		HB_UNTAG(نښان_2),
		hb_blob_get_اوږدتیا (contents),
		hb_blob_get_اوږدتیا (سرچینه_blob));
      hb_blob_destroy (سرچینه_blob);
    }
    أرجع hb_face_builder_جمع_کړه_جدول (dest, نښان_2, contents);
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
HB_دننی فراغ
remap_indexes (ثابت hb_سېټ_t *indexes,
               hb_map_t       *mapping /*بهر*/);


#که_نه_تعریف_شوی HB_NO_VAR
کينډۍ<ډول_نوم توکی_Var_Store>
HB_دننی فراغ
remap_variation_indices (ثابت توکی_Var_Store &var_store,
                         ثابت hb_سېټ_t &variation_indices,
                         ثابت hb_ویکټور_t<صحيح>& normalized_coords,
                         منطقي calculate_delta, /* not pinned at default */
                         منطقي no_variations, /* all axes pinned */
                         hb_hashmap_t<بینښه, hb_pair_t<بینښه, صحيح>> &variation_idx_delta_map /*بهر*/);


کينډۍ<ډول_نوم Delta_سېټ_شاخص_Map>
HB_دننی فراغ
remap_colrv1_delta_سېټ_شاخص_indices (ثابت Delta_سېټ_شاخص_Map &شاخص_map,
                                      ثابت hb_سېټ_t &delta_سېټ_idxes,
                                      hb_hashmap_t<بینښه, hb_pair_t<بینښه, صحيح>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /*بهر*/);


HB_دننی فراغ
generate_varstore_inner_maps (ثابت hb_سېټ_t& varidx_سېټ,
                              بینښه subtable_شمېر,
                              hb_ویکټور_t<hb_inc_bimap_t> &inner_maps /*بهر*/);

HB_دننی منطقي
normalize_axes_location (hb_face_t *face, hb_subset_plan_t *plan);

HB_دننی فراغ
update_instance_metrics_map_from_cff2 (hb_subset_plan_t *plan);

HB_دننی منطقي
get_instance_glyphs_contour_points (hb_subset_plan_t *plan);

#که_نه_تعریف_شوی HB_NO_بنسټ
HB_دننی فراغ
collect_بنسټ_variation_indices (hb_subset_plan_t* plan);
#د_که_پای
#د_که_پای

#که_نه_تعریف_شوی HB_NO_SUBSET_LAYOUT
تعريفنوع hb_hashmap_t<بینښه, hb::unique_ptr<hb_سېټ_t>> script_langsys_map;

HB_دننی فراغ
remap_کارول_شوی_mark_sets (hb_subset_plan_t *plan,
                      hb_map_t& کارول_شوی_mark_sets_map);

HB_دننی فراغ
layout_nameid_تړون (hb_subset_plan_t* plan,
                       hb_سېټ_t* drop_tables);

HB_دننی فراغ
layout_populate_gids_منزل_retain (hb_subset_plan_t* plan,
                                hb_سېټ_t* drop_tables);

HB_دننی فراغ
collect_layout_variation_indices (hb_subset_plan_t* plan);
#د_که_پای


#د_که_پای /* HB_SUBSET_PLAN_HH */

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

#اگر_غیر_تعریف_شدہ HB_SUBSET_PLAN_HH
#تعریف_کرو HB_SUBSET_PLAN_HH

#شامل "hb.hh"

#شامل "hb-subset.h"
#شامل "hb-subset-input.hh"
#شامل "hb-subset-accelerator.hh"

#شامل "hb-map.hh"
#شامل "hb-bimap.hh"
#شامل "hb-set.hh"

نامخلا OT {
ساخت Feature;
}

ساخت os2_info_t {
  hb_codepoint_t min_cmap_codepoint;
  hb_codepoint_t max_cmap_codepoint;
};

قسمتعریف ساخت os2_info_t os2_info_t;

ساخت سر_maxp_info_t
{
  سر_maxp_info_t ()
      :xMin (0x7FFF), xMax (-0x7FFF), yMin (0x7FFF), yMax (-0x7FFF),
      maxPoints (0), maxContours (0),
      max_مرکب_Points (0),
      max_مرکب_Contours (0),
      maxComponentElements (0),
      max_Component_گہرائی (0),
      allXMinIsLsb (سچ) {}

  صحیح xMin;
  صحیح xMax;
  صحیح yMin;
  صحیح yMax;
  غیرعلامتی maxPoints;
  غیرعلامتی maxContours;
  غیرعلامتی max_مرکب_Points;
  غیرعلامتی max_مرکب_Contours;
  غیرعلامتی maxComponentElements;
  غیرعلامتی max_Component_گہرائی;
  منطقی allXMinIsLsb;
};

قسمتعریف ساخت سر_maxp_info_t سر_maxp_info_t;

ساخت contour_نقطہ_t
{
  خالی init (اعشاری x_ = 0.f, اعشاری y_ = 0.f, منطقی is_انتہا_نقطہ_2 = جھوٹ)
  { flag = 0; x = x_; y = y_; is_انتہا_نقطہ = is_انتہا_نقطہ_2; }

  خالی transform (ثابت اعشاری (&میٹرکس)[4])
  {
    اعشاری x_ = x * میٹرکس[0] + y * میٹرکس[2];
	  y  = x * میٹرکس[1] + y * میٹرکس[3];
    x  = x_;
  }

  خالی جوڑو_delta (اعشاری delta_x, اعشاری delta_y)
  {
    x += delta_x;
    y += delta_y;
  }

  HB_ALWAYS_INLINE
  خالی translate (ثابت contour_نقطہ_t &p) { x += p.x; y += p.y; }

  اعشاری x;
  اعشاری y;
  uint8_t flag;
  منطقی is_انتہا_نقطہ;
};

ساخت contour_نقطہ_سمتیہ_t : hb_سمتیہ_t<contour_نقطہ_t>
{
  منطقی جوڑو_deltas (hb_array_t<ثابت اعشاری> deltas_x,
                   hb_array_t<ثابت اعشاری> deltas_y,
                   hb_array_t<ثابت منطقی> indices)
  {
    اگر (indices.لمبائی != deltas_x.لمبائی ||
        indices.لمبائی != deltas_y.لمبائی)
      واپس جھوٹ;

    برائے (غیرعلامتی i = 0; i < indices.لمبائی; i++)
    {
      اگر (!indices.arrayZ[i]) جاری;
      arrayZ[i].جوڑو_delta (deltas_x.arrayZ[i], deltas_y.arrayZ[i]);
    }
    واپس سچ;
  }
};

نامخلا OT {
  ساخت cff1_subset_accelerator_t;
  ساخت cff2_subset_accelerator_t;
}

ساخت hb_subset_plan_t
{
  HB_اندرونی hb_subset_plan_t (hb_face_t *,
				ثابت hb_subset_داخلہ_t *داخلہ);

  HB_اندرونی ~hb_subset_plan_t();

  hb_object_سرخی_t سرخی;

  منطقی successful;
  غیرعلامتی flags;
  منطقی attach_accelerator_ڈیٹا = جھوٹ;
  منطقی force_long_loca = جھوٹ;

  // The glyph subset
  hb_map_t *codepoint_مقصد_glyph; // Needs to be heap-allocated

  // Old -> New glyph id mapping
  hb_map_t *glyph_map; // Needs to be heap-allocated
  hb_map_t *الٹ_glyph_map; // Needs to be heap-allocated

  // Plan is only good for a specific source/dest so keep them with it
  hb_face_t *ماخذ;
#اگر_غیر_تعریف_شدہ HB_NO_SUBSET_CFF
  // These have to be immediately after source:
  hb_face_lazy_loader_t<OT::cff1_subset_accelerator_t, 1> cff1_accel;
  hb_face_lazy_loader_t<OT::cff2_subset_accelerator_t, 2> cff2_accel;
#اگر_ختم

  hb_face_t *dest;

  غیرعلامتی صحیح عدد_output_glyphs_2;

  منطقی all_axes_pinned;
  منطقی pinned_at_default;
  منطقی has_seac;

  // whether to insert a catch-all FeatureVariationRecord
  منطقی gsub_داخل_کرو_catch_all_feature_variation_rec;
  منطقی gpos_داخل_کرو_catch_all_feature_variation_rec;

  // whether GDEF ItemVariationStore is retained
  تغیرپذیر منطقی has_gdef_varstore;

  // whether we're doing avar2 partial instancing
  منطقی has_avar2;

#تعریف_کرو HB_SUBSET_PLAN_MEMBER(Type, Name) Type Name;
#شامل "hb-subset-plan-member-list.hh"
#تعریف_منسوخ HB_SUBSET_PLAN_MEMBER

  //recalculated head/maxp table info after instancing
  تغیرپذیر سر_maxp_info_t سر_maxp_info;

  os2_info_t os2_info;

  ثابت hb_subset_accelerator_t* accelerator = خالی_اشاریہ;
  hb_subset_accelerator_t* inprogress_accelerator = خالی_اشاریہ;

 عوامی:

  سانچہ<قسم_نام T>
  ساخت ماخذ_جدول_loader
  {
    hb_blob_ptr_t<T> عامل () (hb_subset_plan_t *plan)
    {
      hb_تالا_t lock (plan->accelerator ? &plan->accelerator->sanitized_جدول_cache_تالا : خالی_اشاریہ);

      خودکار *cache = plan->accelerator ? &plan->accelerator->sanitized_جدول_cache : &plan->sanitized_جدول_cache;
      اگر (cache
	  && !cache->اندر_error ()
	  && cache->has (+T::جدول_لیبل)) {
	واپس hb_blob_reference (cache->get (+T::جدول_لیبل).get ());
      }

      hb::unique_ptr<hb_blob_t> جدول_blob {hb_sanitize_context_t ().reference_جدول<T> (plan->ماخذ)};
      hb_blob_t* ret = hb_blob_reference (جدول_blob.get ());

      اگر (likely (cache))
	cache->سیٹ (+T::جدول_لیبل, std::move (جدول_blob));

      واپس ret;
    }
  };

  سانچہ<قسم_نام T>
  خودکار ماخذ_جدول() HB_AUTO_RETURN (ماخذ_جدول_loader<T> {} (یہ))

  منطقی اندر_error () ثابت { واپس !successful; }

  HB_اندرونی دوہرا layout_variation_delta (غیرعلامتی var_idx);

  منطقی check_success(منطقی success)
  {
    successful = (successful && success);
    واپس successful;
  }

  /*
   * The set of input glyph ids which will be retained in the subset.
   * Does NOT include ids kept due to retain_gids. You probably want to use
   * glyph_map/reverse_glyph_map.
   */
  اندرونی ثابت hb_سیٹ_t *
  glyphset () ثابت
  {
    واپس &_glyphset;
  }

  /*
   * The set of input glyph ids which will be retained in the subset.
   */
  اندرونی ثابت hb_سیٹ_t *
  glyphset_gsub () ثابت
  {
    واپس &_glyphset_gsub;
  }

  /*
   * Old-to-new glyph id mapping for GSUB glyphs, through the flat
   * array; returns HB_MAP_VALUE_INVALID for glyphs not retained.
   */
  اندرونی hb_codepoint_t
  map_gsub_glyph (hb_codepoint_t old_gid) ثابت
  {
    واپس old_gid < glyph_map_gsub_flat.لمبائی
	 ? glyph_map_gsub_flat.arrayZ[old_gid]
	 : HB_MAP_قدر_INVALID;
  }

  /*
   * The total number of output glyphs in the final subset.
   */
  اندرونی غیرعلامتی صحیح
  عدد_output_glyphs () ثابت
  {
    واپس عدد_output_glyphs_2;
  }

  اندرونی منطقی new_gid_for_codepoint (hb_codepoint_t codepoint,
				     hb_codepoint_t *new_gid) ثابت
  {
    hb_codepoint_t old_gid = codepoint_مقصد_glyph->get (codepoint);
    اگر (old_gid == HB_MAP_قدر_INVALID)
      واپس جھوٹ;

    واپس new_gid_for_old_gid (old_gid, new_gid);
  }

  اندرونی منطقی new_gid_for_old_gid (hb_codepoint_t old_gid,
				   hb_codepoint_t *new_gid) ثابت
  {
    hb_codepoint_t gid = glyph_map->get (old_gid);
    اگر (gid == HB_MAP_قدر_INVALID)
      واپس جھوٹ;

    *new_gid = gid;
    واپس سچ;
  }

  اندرونی منطقی old_gid_for_new_gid (hb_codepoint_t  new_gid,
				   hb_codepoint_t *old_gid) ثابت
  {
    hb_codepoint_t gid = الٹ_glyph_map->get (new_gid);
    اگر (gid == HB_MAP_قدر_INVALID)
      واپس جھوٹ;

    *old_gid = gid;
    واپس سچ;
  }

  اندرونی منطقی
  جوڑو_جدول (hb_لیبل_t لیبل_2,
	     hb_blob_t *contents)
  {
    اگر (HB_DEBUG_SUBSET)
    {
      hb_blob_t *ماخذ_blob = ماخذ->reference_جدول (لیبل_2);
      DEBUG_پیغام(SUBSET, خالی_اشاریہ, "add table %c%c%c%c, dest %u bytes, source %u bytes",
		HB_UNTAG(لیبل_2),
		hb_blob_get_لمبائی (contents),
		hb_blob_get_لمبائی (ماخذ_blob));
      hb_blob_destroy (ماخذ_blob);
    }
    واپس hb_face_builder_جوڑو_جدول (dest, لیبل_2, contents);
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
HB_اندرونی خالی
remap_indexes (ثابت hb_سیٹ_t *indexes,
               hb_map_t       *mapping /*برآمد*/);


#اگر_غیر_تعریف_شدہ HB_NO_VAR
سانچہ<قسم_نام شے_Var_Store>
HB_اندرونی خالی
remap_variation_indices (ثابت شے_Var_Store &var_store,
                         ثابت hb_سیٹ_t &variation_indices,
                         ثابت hb_سمتیہ_t<صحیح>& normalized_coords,
                         منطقی calculate_delta, /* not pinned at default */
                         منطقی no_variations, /* all axes pinned */
                         hb_hashmap_t<غیرعلامتی, hb_pair_t<غیرعلامتی, صحیح>> &variation_idx_delta_map /*برآمد*/);


سانچہ<قسم_نام Delta_سیٹ_اشاریہ_Map>
HB_اندرونی خالی
remap_colrv1_delta_سیٹ_اشاریہ_indices (ثابت Delta_سیٹ_اشاریہ_Map &اشاریہ_map,
                                      ثابت hb_سیٹ_t &delta_سیٹ_idxes,
                                      hb_hashmap_t<غیرعلامتی, hb_pair_t<غیرعلامتی, صحیح>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /*برآمد*/);


HB_اندرونی خالی
generate_varstore_inner_maps (ثابت hb_سیٹ_t& varidx_سیٹ,
                              غیرعلامتی subtable_گنتی,
                              hb_سمتیہ_t<hb_inc_bimap_t> &inner_maps /*برآمد*/);

HB_اندرونی منطقی
normalize_axes_location (hb_face_t *face, hb_subset_plan_t *plan);

HB_اندرونی خالی
update_instance_metrics_map_from_cff2 (hb_subset_plan_t *plan);

HB_اندرونی منطقی
get_instance_glyphs_contour_points (hb_subset_plan_t *plan);

#اگر_غیر_تعریف_شدہ HB_NO_بنیاد
HB_اندرونی خالی
collect_بنیاد_variation_indices (hb_subset_plan_t* plan);
#اگر_ختم
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_NO_SUBSET_LAYOUT
قسمتعریف hb_hashmap_t<غیرعلامتی, hb::unique_ptr<hb_سیٹ_t>> script_langsys_map;

HB_اندرونی خالی
remap_مستعمل_mark_sets (hb_subset_plan_t *plan,
                      hb_map_t& مستعمل_mark_sets_map);

HB_اندرونی خالی
layout_nameid_احاطہ (hb_subset_plan_t* plan,
                       hb_سیٹ_t* drop_tables);

HB_اندرونی خالی
layout_populate_gids_مقصد_retain (hb_subset_plan_t* plan,
                                hb_سیٹ_t* drop_tables);

HB_اندرونی خالی
collect_layout_variation_indices (hb_subset_plan_t* plan);
#اگر_ختم


#اگر_ختم /* HB_SUBSET_PLAN_HH */

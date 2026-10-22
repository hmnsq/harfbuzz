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

#ifndef HB_SUBSET_PLAN_HH
#define HB_SUBSET_PLAN_HH

#شامل "hb.hh"

#شامل "hb-subset.h"
#شامل "hb-subset-input.hh"
#شامل "hb-subset-accelerator.hh"

#شامل "hb-map.hh"
#شامل "hb-bimap.hh"
#شامل "hb-set.hh"

فضاینام OT {
ساختار Feature;
}

ساختار os2_info_t {
  hb_codepoint_t min_cmap_codepoint;
  hb_codepoint_t max_cmap_codepoint;
};

تعریفنوع ساختار os2_info_t os2_info_t;

ساختار head_maxp_info_t
{
  head_maxp_info_t ()
      :xMin (0x7FFF), xMax (-0x7FFF), yMin (0x7FFF), yMax (-0x7FFF),
      maxPoints (0), maxContours (0),
      maxCompositePoints (0),
      maxCompositeContours (0),
      maxComponentElements (0),
      maxComponentDepth (0),
      allXMinIsLsb (درست) {}

  صحیح xMin;
  صحیح xMax;
  صحیح yMin;
  صحیح yMax;
  unsigned maxPoints;
  unsigned maxContours;
  unsigned maxCompositePoints;
  unsigned maxCompositeContours;
  unsigned maxComponentElements;
  unsigned maxComponentDepth;
  منطقی allXMinIsLsb;
};

تعریفنوع ساختار head_maxp_info_t head_maxp_info_t;

ساختار contour_point_t
{
  تهی init (اعشاری x_ = 0.f, اعشاری y_ = 0.f, منطقی is_end_point_ = نادرست)
  { flag = 0; x = x_; y = y_; is_end_point = is_end_point_; }

  تهی transform (ثابت اعشاری (&matrix)[4])
  {
    اعشاری x_ = x * matrix[0] + y * matrix[2];
	  y  = x * matrix[1] + y * matrix[3];
    x  = x_;
  }

  تهی add_delta (اعشاری delta_x, اعشاری delta_y)
  {
    x += delta_x;
    y += delta_y;
  }

  HB_ALWAYS_INLINE
  تهی translate (ثابت contour_point_t &p) { x += p.x; y += p.y; }

  اعشاری x;
  اعشاری y;
  uint8_t flag;
  منطقی is_end_point;
};

ساختار contour_point_vector_t : hb_vector_t<contour_point_t>
{
  منطقی add_deltas (hb_array_t<ثابت اعشاری> deltas_x,
                   hb_array_t<ثابت اعشاری> deltas_y,
                   hb_array_t<ثابت منطقی> indices)
  {
    اگر (indices.length != deltas_x.length ||
        indices.length != deltas_y.length)
      بازگشت نادرست;

    برای (unsigned i = 0; i < indices.length; i++)
    {
      اگر (!indices.arrayZ[i]) ادامه;
      arrayZ[i].add_delta (deltas_x.arrayZ[i], deltas_y.arrayZ[i]);
    }
    بازگشت درست;
  }
};

فضاینام OT {
  ساختار cff1_subset_accelerator_t;
  ساختار cff2_subset_accelerator_t;
}

ساختار hb_subset_plan_t
{
  HB_INTERNAL hb_subset_plan_t (hb_face_t *,
				ثابت hb_subset_درون_داد_t *درون_داد);

  HB_INTERNAL ~hb_subset_plan_t();

  hb_object_header_t header;

  منطقی successful;
  unsigned flags;
  منطقی attach_accelerator_data = نادرست;
  منطقی force_long_loca = نادرست;

  // The glyph subset
  hb_map_t *codepoint_to_glyph; // Needs to be heap-allocated

  // Old -> New glyph id mapping
  hb_map_t *glyph_map; // Needs to be heap-allocated
  hb_map_t *reverse_glyph_map; // Needs to be heap-allocated

  // Plan is only good for a specific source/dest so keep them with it
  hb_face_t *source;
#ifndef HB_NO_SUBSET_CFF
  // These have to be immediately after source:
  hb_face_lazy_loader_t<OT::cff1_subset_accelerator_t, 1> cff1_accel;
  hb_face_lazy_loader_t<OT::cff2_subset_accelerator_t, 2> cff2_accel;
#endif

  hb_face_t *dest;

  unsigned صحیح رقم_output_glyphs_2;

  منطقی all_axes_pinned;
  منطقی pinned_at_default;
  منطقی has_seac;

  // whether to insert a catch-all FeatureVariationRecord
  منطقی gsub_درج_catch_all_feature_variation_rec;
  منطقی gpos_درج_catch_all_feature_variation_rec;

  // whether GDEF ItemVariationStore is retained
  mutable منطقی has_gdef_varstore;

  // whether we're doing avar2 partial instancing
  منطقی has_avar2;

#define HB_SUBSET_PLAN_MEMBER(Type, Name) Type Name;
#شامل "hb-subset-plan-member-list.hh"
#undef HB_SUBSET_PLAN_MEMBER

  //recalculated head/maxp table info after instancing
  mutable head_maxp_info_t head_maxp_info;

  os2_info_t os2_info;

  ثابت hb_subset_accelerator_t* accelerator = nullptr;
  hb_subset_accelerator_t* inprogress_accelerator = nullptr;

 عمومی:

  template<typename T>
  ساختار source_table_loader
  {
    hb_blob_ptr_t<T> operator () (hb_subset_plan_t *plan)
    {
      hb_lock_t lock (plan->accelerator ? &plan->accelerator->sanitized_table_cache_lock : nullptr);

      auto *cache = plan->accelerator ? &plan->accelerator->sanitized_table_cache : &plan->sanitized_table_cache;
      اگر (cache
	  && !cache->in_error ()
	  && cache->has (+T::tableTag)) {
	بازگشت hb_blob_reference (cache->get (+T::tableTag).get ());
      }

      hb::unique_ptr<hb_blob_t> table_blob {hb_sanitize_context_t ().reference_table<T> (plan->source)};
      hb_blob_t* ret = hb_blob_reference (table_blob.get ());

      اگر (likely (cache))
	cache->set (+T::tableTag, std::move (table_blob));

      بازگشت ret;
    }
  };

  template<typename T>
  auto source_table() HB_AUTO_RETURN (source_table_loader<T> {} (این))

  منطقی in_error () ثابت { بازگشت !successful; }

  HB_INTERNAL دوگانه layout_variation_delta (unsigned var_idx);

  منطقی check_success(منطقی success)
  {
    successful = (successful && success);
    بازگشت successful;
  }

  /*
   * The set of input glyph ids which will be retained in the subset.
   * Does NOT include ids kept due to retain_gids. You probably want to use
   * glyph_map/reverse_glyph_map.
   */
  درونخطی ثابت hb_set_t *
  glyphset () ثابت
  {
    بازگشت &_glyphset;
  }

  /*
   * The set of input glyph ids which will be retained in the subset.
   */
  درونخطی ثابت hb_set_t *
  glyphset_gsub () ثابت
  {
    بازگشت &_glyphset_gsub;
  }

  /*
   * Old-to-new glyph id mapping for GSUB glyphs, through the flat
   * array; returns HB_MAP_VALUE_INVALID for glyphs not retained.
   */
  درونخطی hb_codepoint_t
  map_gsub_glyph (hb_codepoint_t old_gid) ثابت
  {
    بازگشت old_gid < glyph_map_gsub_flat.length
	 ? glyph_map_gsub_flat.arrayZ[old_gid]
	 : HB_MAP_مقدار_INVALID;
  }

  /*
   * The total number of output glyphs in the final subset.
   */
  درونخطی unsigned صحیح
  رقم_output_glyphs () ثابت
  {
    بازگشت رقم_output_glyphs_2;
  }

  درونخطی منطقی new_gid_for_codepoint (hb_codepoint_t codepoint,
				     hb_codepoint_t *new_gid) ثابت
  {
    hb_codepoint_t old_gid = codepoint_to_glyph->get (codepoint);
    اگر (old_gid == HB_MAP_مقدار_INVALID)
      بازگشت نادرست;

    بازگشت new_gid_for_old_gid (old_gid, new_gid);
  }

  درونخطی منطقی new_gid_for_old_gid (hb_codepoint_t old_gid,
				   hb_codepoint_t *new_gid) ثابت
  {
    hb_codepoint_t gid = glyph_map->get (old_gid);
    اگر (gid == HB_MAP_مقدار_INVALID)
      بازگشت نادرست;

    *new_gid = gid;
    بازگشت درست;
  }

  درونخطی منطقی old_gid_for_new_gid (hb_codepoint_t  new_gid,
				   hb_codepoint_t *old_gid) ثابت
  {
    hb_codepoint_t gid = reverse_glyph_map->get (new_gid);
    اگر (gid == HB_MAP_مقدار_INVALID)
      بازگشت نادرست;

    *old_gid = gid;
    بازگشت درست;
  }

  درونخطی منطقی
  add_table (hb_tag_t tag,
	     hb_blob_t *contents)
  {
    اگر (HB_DEBUG_SUBSET)
    {
      hb_blob_t *source_blob = source->reference_table (tag);
      DEBUG_پیام(SUBSET, nullptr, "add table %c%c%c%c, dest %u bytes, source %u bytes",
		HB_UNTAG(tag),
		hb_blob_get_length (contents),
		hb_blob_get_length (source_blob));
      hb_blob_destroy (source_blob);
    }
    بازگشت hb_face_builder_add_table (dest, tag, contents);
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
HB_INTERNAL تهی
remap_indexes (ثابت hb_set_t *indexes,
               hb_map_t       *mapping /* OUT */);


#ifndef HB_NO_VAR
template<typename ItemVarStore>
HB_INTERNAL تهی
remap_variation_indices (ثابت ItemVarStore &var_store,
                         ثابت hb_set_t &variation_indices,
                         ثابت hb_vector_t<صحیح>& normalized_coords,
                         منطقی calculate_delta, /* not pinned at default */
                         منطقی no_variations, /* all axes pinned */
                         hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحیح>> &variation_idx_delta_map /* OUT */);


template<typename DeltaSetIndexMap>
HB_INTERNAL تهی
remap_colrv1_delta_set_index_indices (ثابت DeltaSetIndexMap &index_map,
                                      ثابت hb_set_t &delta_set_idxes,
                                      hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحیح>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /* OUT */);


HB_INTERNAL تهی
generate_varstore_inner_maps (ثابت hb_set_t& varidx_set,
                              unsigned subtable_شمار,
                              hb_vector_t<hb_inc_bimap_t> &inner_maps /* OUT */);

HB_INTERNAL منطقی
normalize_axes_location (hb_face_t *face, hb_subset_plan_t *plan);

HB_INTERNAL تهی
update_instance_metrics_map_from_cff2 (hb_subset_plan_t *plan);

HB_INTERNAL منطقی
get_instance_glyphs_contour_points (hb_subset_plan_t *plan);

#ifndef HB_NO_BASE
HB_INTERNAL تهی
collect_base_variation_indices (hb_subset_plan_t* plan);
#endif
#endif

#ifndef HB_NO_SUBSET_LAYOUT
تعریفنوع hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> script_langsys_map;

HB_INTERNAL تهی
remap_used_mark_sets (hb_subset_plan_t *plan,
                      hb_map_t& used_mark_sets_map);

HB_INTERNAL تهی
layout_nameid_closure (hb_subset_plan_t* plan,
                       hb_set_t* drop_tables);

HB_INTERNAL تهی
layout_populate_gids_to_retain (hb_subset_plan_t* plan,
                                hb_set_t* drop_tables);

HB_INTERNAL تهی
collect_layout_variation_indices (hb_subset_plan_t* plan);
#endif


#endif /* HB_SUBSET_PLAN_HH */

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

#அந்தர்பாவ "hb.hh"

#அந்தர்பாவ "hb-subset.h"
#அந்தர்பாவ "hb-subset-input.hh"
#அந்தர்பாவ "hb-subset-accelerator.hh"

#அந்தர்பாவ "hb-map.hh"
#அந்தர்பாவ "hb-bimap.hh"
#அந்தர்பாவ "hb-set.hh"

நாமாகாஶ OT {
கட்டமைப்பு Feature;
}

கட்டமைப்பு os2_info_t {
  hb_codepoint_t min_cmap_codepoint;
  hb_codepoint_t max_cmap_codepoint;
};

வகைவரை கட்டமைப்பு os2_info_t os2_info_t;

கட்டமைப்பு head_maxp_info_t
{
  head_maxp_info_t ()
      :xMin (0x7FFF), xMax (-0x7FFF), yMin (0x7FFF), yMax (-0x7FFF),
      maxPoints (0), maxContours (0),
      maxCompositePoints (0),
      maxCompositeContours (0),
      maxComponentElements (0),
      maxComponentDepth (0),
      allXMinIsLsb (உண்மை) {}

  முழு xMin;
  முழு xMax;
  முழு yMin;
  முழு yMax;
  unsigned maxPoints;
  unsigned maxContours;
  unsigned maxCompositePoints;
  unsigned maxCompositeContours;
  unsigned maxComponentElements;
  unsigned maxComponentDepth;
  தருக்க allXMinIsLsb;
};

வகைவரை கட்டமைப்பு head_maxp_info_t head_maxp_info_t;

கட்டமைப்பு contour_point_t
{
  வெறுமை init (தசம x_ = 0.f, தசம y_ = 0.f, தருக்க is_end_point_ = பொய்)
  { flag = 0; x = x_; y = y_; is_end_point = is_end_point_; }

  வெறுமை transform (மாறிலி தசம (&matrix)[4])
  {
    தசம x_ = x * matrix[0] + y * matrix[2];
	  y  = x * matrix[1] + y * matrix[3];
    x  = x_;
  }

  வெறுமை add_delta (தசம delta_x, தசம delta_y)
  {
    x += delta_x;
    y += delta_y;
  }

  HB_ALWAYS_INLINE
  வெறுமை translate (மாறிலி contour_point_t &p) { x += p.x; y += p.y; }

  தசம x;
  தசம y;
  uint8_t flag;
  தருக்க is_end_point;
};

கட்டமைப்பு contour_point_vector_t : hb_vector_t<contour_point_t>
{
  தருக்க add_deltas (hb_array_t<மாறிலி தசம> deltas_x,
                   hb_array_t<மாறிலி தசம> deltas_y,
                   hb_array_t<மாறிலி தருக்க> indices)
  {
    எனில் (indices.length != deltas_x.length ||
        indices.length != deltas_y.length)
      திரும்பு பொய்;

    க்கு (unsigned i = 0; i < indices.length; i++)
    {
      எனில் (!indices.arrayZ[i]) தொடர்;
      arrayZ[i].add_delta (deltas_x.arrayZ[i], deltas_y.arrayZ[i]);
    }
    திரும்பு உண்மை;
  }
};

நாமாகாஶ OT {
  கட்டமைப்பு cff1_subset_accelerator_t;
  கட்டமைப்பு cff2_subset_accelerator_t;
}

கட்டமைப்பு hb_subset_plan_t
{
  HB_INTERNAL hb_subset_plan_t (hb_face_t *,
				மாறிலி hb_subset_உள்ளீடு_t *உள்ளீடு);

  HB_INTERNAL ~hb_subset_plan_t();

  hb_object_header_t header;

  தருக்க successful;
  unsigned flags;
  தருக்க attach_accelerator_data = பொய்;
  தருக்க force_long_loca = பொய்;

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

  unsigned முழு இலக்கம்_output_glyphs_2;

  தருக்க all_axes_pinned;
  தருக்க pinned_at_default;
  தருக்க has_seac;

  // whether to insert a catch-all FeatureVariationRecord
  தருக்க gsub_செருகு_catch_all_feature_variation_rec;
  தருக்க gpos_செருகு_catch_all_feature_variation_rec;

  // whether GDEF ItemVariationStore is retained
  mutable தருக்க has_gdef_varstore;

  // whether we're doing avar2 partial instancing
  தருக்க has_avar2;

#define HB_SUBSET_PLAN_MEMBER(Type, Name) Type Name;
#அந்தர்பாவ "hb-subset-plan-member-list.hh"
#undef HB_SUBSET_PLAN_MEMBER

  //recalculated head/maxp table info after instancing
  mutable head_maxp_info_t head_maxp_info;

  os2_info_t os2_info;

  மாறிலி hb_subset_accelerator_t* accelerator = nullptr;
  hb_subset_accelerator_t* inprogress_accelerator = nullptr;

 பொது:

  template<typename T>
  கட்டமைப்பு source_table_loader
  {
    hb_blob_ptr_t<T> operator () (hb_subset_plan_t *plan)
    {
      hb_lock_t lock (plan->accelerator ? &plan->accelerator->sanitized_table_cache_lock : nullptr);

      auto *cache = plan->accelerator ? &plan->accelerator->sanitized_table_cache : &plan->sanitized_table_cache;
      எனில் (cache
	  && !cache->in_error ()
	  && cache->has (+T::tableTag)) {
	திரும்பு hb_blob_reference (cache->get (+T::tableTag).get ());
      }

      hb::unique_ptr<hb_blob_t> table_blob {hb_sanitize_context_t ().reference_table<T> (plan->source)};
      hb_blob_t* ret = hb_blob_reference (table_blob.get ());

      எனில் (likely (cache))
	cache->set (+T::tableTag, std::move (table_blob));

      திரும்பு ret;
    }
  };

  template<typename T>
  auto source_table() HB_AUTO_RETURN (source_table_loader<T> {} (இது))

  தருக்க in_error () மாறிலி { திரும்பு !successful; }

  HB_INTERNAL இரட்டை layout_variation_delta (unsigned var_idx);

  தருக்க check_success(தருக்க success)
  {
    successful = (successful && success);
    திரும்பு successful;
  }

  /*
   * The set of input glyph ids which will be retained in the subset.
   * Does NOT include ids kept due to retain_gids. You probably want to use
   * glyph_map/reverse_glyph_map.
   */
  உள்வரி மாறிலி hb_set_t *
  glyphset () மாறிலி
  {
    திரும்பு &_glyphset;
  }

  /*
   * The set of input glyph ids which will be retained in the subset.
   */
  உள்வரி மாறிலி hb_set_t *
  glyphset_gsub () மாறிலி
  {
    திரும்பு &_glyphset_gsub;
  }

  /*
   * Old-to-new glyph id mapping for GSUB glyphs, through the flat
   * array; returns HB_MAP_VALUE_INVALID for glyphs not retained.
   */
  உள்வரி hb_codepoint_t
  map_gsub_glyph (hb_codepoint_t old_gid) மாறிலி
  {
    திரும்பு old_gid < glyph_map_gsub_flat.length
	 ? glyph_map_gsub_flat.arrayZ[old_gid]
	 : HB_MAP_மதிப்பு_INVALID;
  }

  /*
   * The total number of output glyphs in the final subset.
   */
  உள்வரி unsigned முழு
  இலக்கம்_output_glyphs () மாறிலி
  {
    திரும்பு இலக்கம்_output_glyphs_2;
  }

  உள்வரி தருக்க new_gid_for_codepoint (hb_codepoint_t codepoint,
				     hb_codepoint_t *new_gid) மாறிலி
  {
    hb_codepoint_t old_gid = codepoint_to_glyph->get (codepoint);
    எனில் (old_gid == HB_MAP_மதிப்பு_INVALID)
      திரும்பு பொய்;

    திரும்பு new_gid_for_old_gid (old_gid, new_gid);
  }

  உள்வரி தருக்க new_gid_for_old_gid (hb_codepoint_t old_gid,
				   hb_codepoint_t *new_gid) மாறிலி
  {
    hb_codepoint_t gid = glyph_map->get (old_gid);
    எனில் (gid == HB_MAP_மதிப்பு_INVALID)
      திரும்பு பொய்;

    *new_gid = gid;
    திரும்பு உண்மை;
  }

  உள்வரி தருக்க old_gid_for_new_gid (hb_codepoint_t  new_gid,
				   hb_codepoint_t *old_gid) மாறிலி
  {
    hb_codepoint_t gid = reverse_glyph_map->get (new_gid);
    எனில் (gid == HB_MAP_மதிப்பு_INVALID)
      திரும்பு பொய்;

    *old_gid = gid;
    திரும்பு உண்மை;
  }

  உள்வரி தருக்க
  add_table (hb_tag_t tag,
	     hb_blob_t *contents)
  {
    எனில் (HB_DEBUG_SUBSET)
    {
      hb_blob_t *source_blob = source->reference_table (tag);
      DEBUG_செய்தி(SUBSET, nullptr, "add table %c%c%c%c, dest %u bytes, source %u bytes",
		HB_UNTAG(tag),
		hb_blob_get_length (contents),
		hb_blob_get_length (source_blob));
      hb_blob_destroy (source_blob);
    }
    திரும்பு hb_face_builder_add_table (dest, tag, contents);
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
HB_INTERNAL வெறுமை
remap_indexes (மாறிலி hb_set_t *indexes,
               hb_map_t       *mapping /* OUT */);


#ifndef HB_NO_VAR
template<typename ItemVarStore>
HB_INTERNAL வெறுமை
remap_variation_indices (மாறிலி ItemVarStore &var_store,
                         மாறிலி hb_set_t &variation_indices,
                         மாறிலி hb_vector_t<முழு>& normalized_coords,
                         தருக்க calculate_delta, /* not pinned at default */
                         தருக்க no_variations, /* all axes pinned */
                         hb_hashmap_t<unsigned, hb_pair_t<unsigned, முழு>> &variation_idx_delta_map /* OUT */);


template<typename DeltaSetIndexMap>
HB_INTERNAL வெறுமை
remap_colrv1_delta_set_index_indices (மாறிலி DeltaSetIndexMap &index_map,
                                      மாறிலி hb_set_t &delta_set_idxes,
                                      hb_hashmap_t<unsigned, hb_pair_t<unsigned, முழு>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /* OUT */);


HB_INTERNAL வெறுமை
generate_varstore_inner_maps (மாறிலி hb_set_t& varidx_set,
                              unsigned subtable_மொத்தம்,
                              hb_vector_t<hb_inc_bimap_t> &inner_maps /* OUT */);

HB_INTERNAL தருக்க
normalize_axes_location (hb_face_t *face, hb_subset_plan_t *plan);

HB_INTERNAL வெறுமை
update_instance_metrics_map_from_cff2 (hb_subset_plan_t *plan);

HB_INTERNAL தருக்க
get_instance_glyphs_contour_points (hb_subset_plan_t *plan);

#ifndef HB_NO_BASE
HB_INTERNAL வெறுமை
collect_base_variation_indices (hb_subset_plan_t* plan);
#endif
#endif

#ifndef HB_NO_SUBSET_LAYOUT
வகைவரை hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> script_langsys_map;

HB_INTERNAL வெறுமை
remap_used_mark_sets (hb_subset_plan_t *plan,
                      hb_map_t& used_mark_sets_map);

HB_INTERNAL வெறுமை
layout_nameid_closure (hb_subset_plan_t* plan,
                       hb_set_t* drop_tables);

HB_INTERNAL வெறுமை
layout_populate_gids_to_retain (hb_subset_plan_t* plan,
                                hb_set_t* drop_tables);

HB_INTERNAL வெறுமை
collect_layout_variation_indices (hb_subset_plan_t* plan);
#endif


#endif /* HB_SUBSET_PLAN_HH */

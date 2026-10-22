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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ಅನ್ತರ್ಭಾವ "hb-subset.h"
#ಅನ್ತರ್ಭಾವ "hb-subset-input.hh"
#ಅನ್ತರ್ಭಾವ "hb-subset-accelerator.hh"

#ಅನ್ತರ್ಭಾವ "hb-map.hh"
#ಅನ್ತರ್ಭಾವ "hb-bimap.hh"
#ಅನ್ತರ್ಭಾವ "hb-set.hh"

ನಾಮಾಕಾಶ OT {
ಸಂರಚನಾ Feature;
}

ಸಂರಚನಾ os2_info_t {
  hb_codepoint_t min_cmap_codepoint;
  hb_codepoint_t max_cmap_codepoint;
};

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ os2_info_t os2_info_t;

ಸಂರಚನಾ head_maxp_info_t
{
  head_maxp_info_t ()
      :xMin (0x7FFF), xMax (-0x7FFF), yMin (0x7FFF), yMax (-0x7FFF),
      maxPoints (0), maxContours (0),
      maxCompositePoints (0),
      maxCompositeContours (0),
      maxComponentElements (0),
      maxComponentDepth (0),
      allXMinIsLsb (ಸತ್ಯ) {}

  ಪೂರ್ಣಾಙ್ಕ xMin;
  ಪೂರ್ಣಾಙ್ಕ xMax;
  ಪೂರ್ಣಾಙ್ಕ yMin;
  ಪೂರ್ಣಾಙ್ಕ yMax;
  unsigned maxPoints;
  unsigned maxContours;
  unsigned maxCompositePoints;
  unsigned maxCompositeContours;
  unsigned maxComponentElements;
  unsigned maxComponentDepth;
  ತಾರ್ಕಿಕ allXMinIsLsb;
};

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ head_maxp_info_t head_maxp_info_t;

ಸಂರಚನಾ contour_point_t
{
  ಶೂನ್ಯ init (ಭಿನ್ನ x_ = 0.f, ಭಿನ್ನ y_ = 0.f, ತಾರ್ಕಿಕ is_end_point_ = ಅಸತ್ಯ)
  { flag = 0; x = x_; y = y_; is_end_point = is_end_point_; }

  ಶೂನ್ಯ transform (ಧ್ರುವ ಭಿನ್ನ (&matrix)[4])
  {
    ಭಿನ್ನ x_ = x * matrix[0] + y * matrix[2];
	  y  = x * matrix[1] + y * matrix[3];
    x  = x_;
  }

  ಶೂನ್ಯ add_delta (ಭಿನ್ನ delta_x, ಭಿನ್ನ delta_y)
  {
    x += delta_x;
    y += delta_y;
  }

  HB_ALWAYS_INLINE
  ಶೂನ್ಯ translate (ಧ್ರುವ contour_point_t &p) { x += p.x; y += p.y; }

  ಭಿನ್ನ x;
  ಭಿನ್ನ y;
  uint8_t flag;
  ತಾರ್ಕಿಕ is_end_point;
};

ಸಂರಚನಾ contour_point_vector_t : hb_vector_t<contour_point_t>
{
  ತಾರ್ಕಿಕ add_deltas (hb_array_t<ಧ್ರುವ ಭಿನ್ನ> deltas_x,
                   hb_array_t<ಧ್ರುವ ಭಿನ್ನ> deltas_y,
                   hb_array_t<ಧ್ರುವ ತಾರ್ಕಿಕ> indices)
  {
    ಯದಿ (indices.length != deltas_x.length ||
        indices.length != deltas_y.length)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪ್ರತಿ (unsigned i = 0; i < indices.length; i++)
    {
      ಯದಿ (!indices.arrayZ[i]) ಅನುವರ್ತ;
      arrayZ[i].add_delta (deltas_x.arrayZ[i], deltas_y.arrayZ[i]);
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }
};

ನಾಮಾಕಾಶ OT {
  ಸಂರಚನಾ cff1_subset_accelerator_t;
  ಸಂರಚನಾ cff2_subset_accelerator_t;
}

ಸಂರಚನಾ hb_subset_plan_t
{
  HB_INTERNAL hb_subset_plan_t (hb_face_t *,
				ಧ್ರುವ hb_subset_ಒಳಹರಿವು_t *ಒಳಹರಿವು);

  HB_INTERNAL ~hb_subset_plan_t();

  hb_object_header_t header;

  ತಾರ್ಕಿಕ successful;
  unsigned flags;
  ತಾರ್ಕಿಕ attach_accelerator_data = ಅಸತ್ಯ;
  ತಾರ್ಕಿಕ force_long_loca = ಅಸತ್ಯ;

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

  unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_output_glyphs_2;

  ತಾರ್ಕಿಕ all_axes_pinned;
  ತಾರ್ಕಿಕ pinned_at_default;
  ತಾರ್ಕಿಕ has_seac;

  // whether to insert a catch-all FeatureVariationRecord
  ತಾರ್ಕಿಕ gsub_ಸೇರಿಸು_catch_all_feature_variation_rec;
  ತಾರ್ಕಿಕ gpos_ಸೇರಿಸು_catch_all_feature_variation_rec;

  // whether GDEF ItemVariationStore is retained
  mutable ತಾರ್ಕಿಕ has_gdef_varstore;

  // whether we're doing avar2 partial instancing
  ತಾರ್ಕಿಕ has_avar2;

#define HB_SUBSET_PLAN_MEMBER(Type, Name) Type Name;
#ಅನ್ತರ್ಭಾವ "hb-subset-plan-member-list.hh"
#undef HB_SUBSET_PLAN_MEMBER

  //recalculated head/maxp table info after instancing
  mutable head_maxp_info_t head_maxp_info;

  os2_info_t os2_info;

  ಧ್ರುವ hb_subset_accelerator_t* accelerator = nullptr;
  hb_subset_accelerator_t* inprogress_accelerator = nullptr;

 ಸಾರ್ವಜನಿಕ:

  template<typename T>
  ಸಂರಚನಾ source_table_loader
  {
    hb_blob_ptr_t<T> operator () (hb_subset_plan_t *plan)
    {
      hb_lock_t lock (plan->accelerator ? &plan->accelerator->sanitized_table_cache_lock : nullptr);

      auto *cache = plan->accelerator ? &plan->accelerator->sanitized_table_cache : &plan->sanitized_table_cache;
      ಯದಿ (cache
	  && !cache->in_error ()
	  && cache->has (+T::tableTag)) {
	ಪ್ರತ್ಯಾಗಮ hb_blob_reference (cache->get (+T::tableTag).get ());
      }

      hb::unique_ptr<hb_blob_t> table_blob {hb_sanitize_context_t ().reference_table<T> (plan->source)};
      hb_blob_t* ret = hb_blob_reference (table_blob.get ());

      ಯದಿ (likely (cache))
	cache->set (+T::tableTag, std::move (table_blob));

      ಪ್ರತ್ಯಾಗಮ ret;
    }
  };

  template<typename T>
  auto source_table() HB_AUTO_RETURN (source_table_loader<T> {} (ಏತತ್))

  ತಾರ್ಕಿಕ in_error () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ !successful; }

  HB_INTERNAL ದ್ವಿಗುಣ layout_variation_delta (unsigned var_idx);

  ತಾರ್ಕಿಕ check_success(ತಾರ್ಕಿಕ success)
  {
    successful = (successful && success);
    ಪ್ರತ್ಯಾಗಮ successful;
  }

  /*
   * The set of input glyph ids which will be retained in the subset.
   * Does NOT include ids kept due to retain_gids. You probably want to use
   * glyph_map/reverse_glyph_map.
   */
  ಅನ್ತಃಸ್ಥ ಧ್ರುವ hb_set_t *
  glyphset () ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ &_glyphset;
  }

  /*
   * The set of input glyph ids which will be retained in the subset.
   */
  ಅನ್ತಃಸ್ಥ ಧ್ರುವ hb_set_t *
  glyphset_gsub () ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ &_glyphset_gsub;
  }

  /*
   * Old-to-new glyph id mapping for GSUB glyphs, through the flat
   * array; returns HB_MAP_VALUE_INVALID for glyphs not retained.
   */
  ಅನ್ತಃಸ್ಥ hb_codepoint_t
  map_gsub_glyph (hb_codepoint_t old_gid) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ old_gid < glyph_map_gsub_flat.length
	 ? glyph_map_gsub_flat.arrayZ[old_gid]
	 : HB_MAP_ಮೌಲ್ಯ_INVALID;
  }

  /*
   * The total number of output glyphs in the final subset.
   */
  ಅನ್ತಃಸ್ಥ unsigned ಪೂರ್ಣಾಙ್ಕ
  ಅಂಕೆ_output_glyphs () ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ ಅಂಕೆ_output_glyphs_2;
  }

  ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ new_gid_for_codepoint (hb_codepoint_t codepoint,
				     hb_codepoint_t *new_gid) ಧ್ರುವ
  {
    hb_codepoint_t old_gid = codepoint_to_glyph->get (codepoint);
    ಯದಿ (old_gid == HB_MAP_ಮೌಲ್ಯ_INVALID)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪ್ರತ್ಯಾಗಮ new_gid_for_old_gid (old_gid, new_gid);
  }

  ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ new_gid_for_old_gid (hb_codepoint_t old_gid,
				   hb_codepoint_t *new_gid) ಧ್ರುವ
  {
    hb_codepoint_t gid = glyph_map->get (old_gid);
    ಯದಿ (gid == HB_MAP_ಮೌಲ್ಯ_INVALID)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    *new_gid = gid;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ old_gid_for_new_gid (hb_codepoint_t  new_gid,
				   hb_codepoint_t *old_gid) ಧ್ರುವ
  {
    hb_codepoint_t gid = reverse_glyph_map->get (new_gid);
    ಯದಿ (gid == HB_MAP_ಮೌಲ್ಯ_INVALID)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    *old_gid = gid;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ
  add_table (hb_tag_t tag,
	     hb_blob_t *contents)
  {
    ಯದಿ (HB_DEBUG_SUBSET)
    {
      hb_blob_t *source_blob = source->reference_table (tag);
      DEBUG_ಸಂದೇಶ(SUBSET, nullptr, "add table %c%c%c%c, dest %u bytes, source %u bytes",
		HB_UNTAG(tag),
		hb_blob_get_length (contents),
		hb_blob_get_length (source_blob));
      hb_blob_destroy (source_blob);
    }
    ಪ್ರತ್ಯಾಗಮ hb_face_builder_add_table (dest, tag, contents);
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
HB_INTERNAL ಶೂನ್ಯ
remap_indexes (ಧ್ರುವ hb_set_t *indexes,
               hb_map_t       *mapping /* OUT */);


#ifndef HB_NO_VAR
template<typename ItemVarStore>
HB_INTERNAL ಶೂನ್ಯ
remap_variation_indices (ಧ್ರುವ ItemVarStore &var_store,
                         ಧ್ರುವ hb_set_t &variation_indices,
                         ಧ್ರುವ hb_vector_t<ಪೂರ್ಣಾಙ್ಕ>& normalized_coords,
                         ತಾರ್ಕಿಕ calculate_delta, /* not pinned at default */
                         ತಾರ್ಕಿಕ no_variations, /* all axes pinned */
                         hb_hashmap_t<unsigned, hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ>> &variation_idx_delta_map /* OUT */);


template<typename DeltaSetIndexMap>
HB_INTERNAL ಶೂನ್ಯ
remap_colrv1_delta_set_index_indices (ಧ್ರುವ DeltaSetIndexMap &index_map,
                                      ಧ್ರುವ hb_set_t &delta_set_idxes,
                                      hb_hashmap_t<unsigned, hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /* OUT */);


HB_INTERNAL ಶೂನ್ಯ
generate_varstore_inner_maps (ಧ್ರುವ hb_set_t& varidx_set,
                              unsigned subtable_ಎಣಿಕೆ,
                              hb_vector_t<hb_inc_bimap_t> &inner_maps /* OUT */);

HB_INTERNAL ತಾರ್ಕಿಕ
normalize_axes_location (hb_face_t *face, hb_subset_plan_t *plan);

HB_INTERNAL ಶೂನ್ಯ
update_instance_metrics_map_from_cff2 (hb_subset_plan_t *plan);

HB_INTERNAL ತಾರ್ಕಿಕ
get_instance_glyphs_contour_points (hb_subset_plan_t *plan);

#ifndef HB_NO_BASE
HB_INTERNAL ಶೂನ್ಯ
collect_base_variation_indices (hb_subset_plan_t* plan);
#endif
#endif

#ifndef HB_NO_SUBSET_LAYOUT
ಪ್ರಕಾರನಿರ್ದೇಶ hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> script_langsys_map;

HB_INTERNAL ಶೂನ್ಯ
remap_used_mark_sets (hb_subset_plan_t *plan,
                      hb_map_t& used_mark_sets_map);

HB_INTERNAL ಶೂನ್ಯ
layout_nameid_closure (hb_subset_plan_t* plan,
                       hb_set_t* drop_tables);

HB_INTERNAL ಶೂನ್ಯ
layout_populate_gids_to_retain (hb_subset_plan_t* plan,
                                hb_set_t* drop_tables);

HB_INTERNAL ಶೂನ್ಯ
collect_layout_variation_indices (hb_subset_plan_t* plan);
#endif


#endif /* HB_SUBSET_PLAN_HH */

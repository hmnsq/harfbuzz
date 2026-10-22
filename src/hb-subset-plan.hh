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

#অন্তর্ভাৱ "hb.hh"

#অন্তর্ভাৱ "hb-subset.h"
#অন্তর্ভাৱ "hb-subset-input.hh"
#অন্তর্ভাৱ "hb-subset-accelerator.hh"

#অন্তর্ভাৱ "hb-map.hh"
#অন্তর্ভাৱ "hb-bimap.hh"
#অন্তর্ভাৱ "hb-set.hh"

নামাকাশ OT {
সংরচনা Feature;
}

সংরচনা os2_info_t {
  hb_codepoint_t min_cmap_codepoint;
  hb_codepoint_t max_cmap_codepoint;
};

প্রকারনির্দেশ সংরচনা os2_info_t os2_info_t;

সংরচনা head_maxp_info_t
{
  head_maxp_info_t ()
      :xMin (0x7FFF), xMax (-0x7FFF), yMin (0x7FFF), yMax (-0x7FFF),
      maxPoints (0), maxContours (0),
      maxCompositePoints (0),
      maxCompositeContours (0),
      maxComponentElements (0),
      maxComponentDepth (0),
      allXMinIsLsb (সঁচা) {}

  পূর্ণাঙ্ক xMin;
  পূর্ণাঙ্ক xMax;
  পূর্ণাঙ্ক yMin;
  পূর্ণাঙ্ক yMax;
  unsigned maxPoints;
  unsigned maxContours;
  unsigned maxCompositePoints;
  unsigned maxCompositeContours;
  unsigned maxComponentElements;
  unsigned maxComponentDepth;
  তার্কিক allXMinIsLsb;
};

প্রকারনির্দেশ সংরচনা head_maxp_info_t head_maxp_info_t;

সংরচনা contour_point_t
{
  শূন্য init (ভিন্ন x_ = 0.f, ভিন্ন y_ = 0.f, তার্কিক is_end_point_ = মিছা)
  { flag = 0; x = x_; y = y_; is_end_point = is_end_point_; }

  শূন্য transform (ধ্রুৱ ভিন্ন (&matrix)[4])
  {
    ভিন্ন x_ = x * matrix[0] + y * matrix[2];
	  y  = x * matrix[1] + y * matrix[3];
    x  = x_;
  }

  শূন্য add_delta (ভিন্ন delta_x, ভিন্ন delta_y)
  {
    x += delta_x;
    y += delta_y;
  }

  HB_ALWAYS_INLINE
  শূন্য translate (ধ্রুৱ contour_point_t &p) { x += p.x; y += p.y; }

  ভিন্ন x;
  ভিন্ন y;
  uint8_t flag;
  তার্কিক is_end_point;
};

সংরচনা contour_point_vector_t : hb_vector_t<contour_point_t>
{
  তার্কিক add_deltas (hb_array_t<ধ্রুৱ ভিন্ন> deltas_x,
                   hb_array_t<ধ্রুৱ ভিন্ন> deltas_y,
                   hb_array_t<ধ্রুৱ তার্কিক> indices)
  {
    যদি (indices.length != deltas_x.length ||
        indices.length != deltas_y.length)
      ঘূৰাই মিছা;

    বাবে (unsigned i = 0; i < indices.length; i++)
    {
      যদি (!indices.arrayZ[i]) অনুৱর্ত;
      arrayZ[i].add_delta (deltas_x.arrayZ[i], deltas_y.arrayZ[i]);
    }
    ঘূৰাই সঁচা;
  }
};

নামাকাশ OT {
  সংরচনা cff1_subset_accelerator_t;
  সংরচনা cff2_subset_accelerator_t;
}

সংরচনা hb_subset_plan_t
{
  HB_INTERNAL hb_subset_plan_t (hb_face_t *,
				ধ্রুৱ hb_subset_প্ৰবেশ_t *প্ৰবেশ);

  HB_INTERNAL ~hb_subset_plan_t();

  hb_object_header_t header;

  তার্কিক successful;
  unsigned flags;
  তার্কিক attach_accelerator_data = মিছা;
  তার্কিক force_long_loca = মিছা;

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

  unsigned পূর্ণাঙ্ক অঙ্ক_output_glyphs_2;

  তার্কিক all_axes_pinned;
  তার্কিক pinned_at_default;
  তার্কিক has_seac;

  // whether to insert a catch-all FeatureVariationRecord
  তার্কিক gsub_ঢোকাও_catch_all_feature_variation_rec;
  তার্কিক gpos_ঢোকাও_catch_all_feature_variation_rec;

  // whether GDEF ItemVariationStore is retained
  mutable তার্কিক has_gdef_varstore;

  // whether we're doing avar2 partial instancing
  তার্কিক has_avar2;

#define HB_SUBSET_PLAN_MEMBER(Type, Name) Type Name;
#অন্তর্ভাৱ "hb-subset-plan-member-list.hh"
#undef HB_SUBSET_PLAN_MEMBER

  //recalculated head/maxp table info after instancing
  mutable head_maxp_info_t head_maxp_info;

  os2_info_t os2_info;

  ধ্রুৱ hb_subset_accelerator_t* accelerator = nullptr;
  hb_subset_accelerator_t* inprogress_accelerator = nullptr;

 সার্ৱজনিক:

  template<typename T>
  সংরচনা source_table_loader
  {
    hb_blob_ptr_t<T> operator () (hb_subset_plan_t *plan)
    {
      hb_lock_t lock (plan->accelerator ? &plan->accelerator->sanitized_table_cache_lock : nullptr);

      auto *cache = plan->accelerator ? &plan->accelerator->sanitized_table_cache : &plan->sanitized_table_cache;
      যদি (cache
	  && !cache->in_error ()
	  && cache->has (+T::tableTag)) {
	ঘূৰাই hb_blob_reference (cache->get (+T::tableTag).get ());
      }

      hb::unique_ptr<hb_blob_t> table_blob {hb_sanitize_context_t ().reference_table<T> (plan->source)};
      hb_blob_t* ret = hb_blob_reference (table_blob.get ());

      যদি (likely (cache))
	cache->set (+T::tableTag, std::move (table_blob));

      ঘূৰাই ret;
    }
  };

  template<typename T>
  auto source_table() HB_AUTO_RETURN (source_table_loader<T> {} (এতত্))

  তার্কিক in_error () ধ্রুৱ { ঘূৰাই !successful; }

  HB_INTERNAL দ্ৱিগুণ layout_variation_delta (unsigned var_idx);

  তার্কিক check_success(তার্কিক success)
  {
    successful = (successful && success);
    ঘূৰাই successful;
  }

  /*
   * The set of input glyph ids which will be retained in the subset.
   * Does NOT include ids kept due to retain_gids. You probably want to use
   * glyph_map/reverse_glyph_map.
   */
  অন্তঃস্থ ধ্রুৱ hb_set_t *
  glyphset () ধ্রুৱ
  {
    ঘূৰাই &_glyphset;
  }

  /*
   * The set of input glyph ids which will be retained in the subset.
   */
  অন্তঃস্থ ধ্রুৱ hb_set_t *
  glyphset_gsub () ধ্রুৱ
  {
    ঘূৰাই &_glyphset_gsub;
  }

  /*
   * Old-to-new glyph id mapping for GSUB glyphs, through the flat
   * array; returns HB_MAP_VALUE_INVALID for glyphs not retained.
   */
  অন্তঃস্থ hb_codepoint_t
  map_gsub_glyph (hb_codepoint_t old_gid) ধ্রুৱ
  {
    ঘূৰাই old_gid < glyph_map_gsub_flat.length
	 ? glyph_map_gsub_flat.arrayZ[old_gid]
	 : HB_MAP_মান_INVALID;
  }

  /*
   * The total number of output glyphs in the final subset.
   */
  অন্তঃস্থ unsigned পূর্ণাঙ্ক
  অঙ্ক_output_glyphs () ধ্রুৱ
  {
    ঘূৰাই অঙ্ক_output_glyphs_2;
  }

  অন্তঃস্থ তার্কিক new_gid_for_codepoint (hb_codepoint_t codepoint,
				     hb_codepoint_t *new_gid) ধ্রুৱ
  {
    hb_codepoint_t old_gid = codepoint_to_glyph->get (codepoint);
    যদি (old_gid == HB_MAP_মান_INVALID)
      ঘূৰাই মিছা;

    ঘূৰাই new_gid_for_old_gid (old_gid, new_gid);
  }

  অন্তঃস্থ তার্কিক new_gid_for_old_gid (hb_codepoint_t old_gid,
				   hb_codepoint_t *new_gid) ধ্রুৱ
  {
    hb_codepoint_t gid = glyph_map->get (old_gid);
    যদি (gid == HB_MAP_মান_INVALID)
      ঘূৰাই মিছা;

    *new_gid = gid;
    ঘূৰাই সঁচা;
  }

  অন্তঃস্থ তার্কিক old_gid_for_new_gid (hb_codepoint_t  new_gid,
				   hb_codepoint_t *old_gid) ধ্রুৱ
  {
    hb_codepoint_t gid = reverse_glyph_map->get (new_gid);
    যদি (gid == HB_MAP_মান_INVALID)
      ঘূৰাই মিছা;

    *old_gid = gid;
    ঘূৰাই সঁচা;
  }

  অন্তঃস্থ তার্কিক
  add_table (hb_tag_t tag,
	     hb_blob_t *contents)
  {
    যদি (HB_DEBUG_SUBSET)
    {
      hb_blob_t *source_blob = source->reference_table (tag);
      DEBUG_বাৰ্তা(SUBSET, nullptr, "add table %c%c%c%c, dest %u bytes, source %u bytes",
		HB_UNTAG(tag),
		hb_blob_get_length (contents),
		hb_blob_get_length (source_blob));
      hb_blob_destroy (source_blob);
    }
    ঘূৰাই hb_face_builder_add_table (dest, tag, contents);
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
HB_INTERNAL শূন্য
remap_indexes (ধ্রুৱ hb_set_t *indexes,
               hb_map_t       *mapping /* OUT */);


#ifndef HB_NO_VAR
template<typename ItemVarStore>
HB_INTERNAL শূন্য
remap_variation_indices (ধ্রুৱ ItemVarStore &var_store,
                         ধ্রুৱ hb_set_t &variation_indices,
                         ধ্রুৱ hb_vector_t<পূর্ণাঙ্ক>& normalized_coords,
                         তার্কিক calculate_delta, /* not pinned at default */
                         তার্কিক no_variations, /* all axes pinned */
                         hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণাঙ্ক>> &variation_idx_delta_map /* OUT */);


template<typename DeltaSetIndexMap>
HB_INTERNAL শূন্য
remap_colrv1_delta_set_index_indices (ধ্রুৱ DeltaSetIndexMap &index_map,
                                      ধ্রুৱ hb_set_t &delta_set_idxes,
                                      hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণাঙ্ক>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /* OUT */);


HB_INTERNAL শূন্য
generate_varstore_inner_maps (ধ্রুৱ hb_set_t& varidx_set,
                              unsigned subtable_গুনতি,
                              hb_vector_t<hb_inc_bimap_t> &inner_maps /* OUT */);

HB_INTERNAL তার্কিক
normalize_axes_location (hb_face_t *face, hb_subset_plan_t *plan);

HB_INTERNAL শূন্য
update_instance_metrics_map_from_cff2 (hb_subset_plan_t *plan);

HB_INTERNAL তার্কিক
get_instance_glyphs_contour_points (hb_subset_plan_t *plan);

#ifndef HB_NO_BASE
HB_INTERNAL শূন্য
collect_base_variation_indices (hb_subset_plan_t* plan);
#endif
#endif

#ifndef HB_NO_SUBSET_LAYOUT
প্রকারনির্দেশ hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> script_langsys_map;

HB_INTERNAL শূন্য
remap_used_mark_sets (hb_subset_plan_t *plan,
                      hb_map_t& used_mark_sets_map);

HB_INTERNAL শূন্য
layout_nameid_closure (hb_subset_plan_t* plan,
                       hb_set_t* drop_tables);

HB_INTERNAL শূন্য
layout_populate_gids_to_retain (hb_subset_plan_t* plan,
                                hb_set_t* drop_tables);

HB_INTERNAL শূন্য
collect_layout_variation_indices (hb_subset_plan_t* plan);
#endif


#endif /* HB_SUBSET_PLAN_HH */

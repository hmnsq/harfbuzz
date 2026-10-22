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

#অন্তর্ভাব "hb.hh"

#অন্তর্ভাব "hb-subset.h"
#অন্তর্ভাব "hb-subset-input.hh"
#অন্তর্ভাব "hb-subset-accelerator.hh"

#অন্তর্ভাব "hb-map.hh"
#অন্তর্ভাব "hb-bimap.hh"
#অন্তর্ভাব "hb-set.hh"

নামাকাশ OT {
কাঠামো Feature;
}

কাঠামো os2_info_t {
  hb_codepoint_t min_cmap_codepoint;
  hb_codepoint_t max_cmap_codepoint;
};

প্রকারলেখ কাঠামো os2_info_t os2_info_t;

কাঠামো শীর্ষ_maxp_info_t
{
  শীর্ষ_maxp_info_t ()
      :xMin (0x7FFF), xMax (-0x7FFF), yMin (0x7FFF), yMax (-0x7FFF),
      maxPoints (0), maxContours (0),
      maxCompositePoints (0),
      maxCompositeContours (0),
      maxComponentElements (0),
      maxComponentDepth (0),
      allXMinIsLsb (সত্য) {}

  পূর্ণসংখ্যা xMin;
  পূর্ণসংখ্যা xMax;
  পূর্ণসংখ্যা yMin;
  পূর্ণসংখ্যা yMax;
  unsigned maxPoints;
  unsigned maxContours;
  unsigned maxCompositePoints;
  unsigned maxCompositeContours;
  unsigned maxComponentElements;
  unsigned maxComponentDepth;
  যৌক্তিক allXMinIsLsb;
};

প্রকারলেখ কাঠামো শীর্ষ_maxp_info_t শীর্ষ_maxp_info_t;

কাঠামো contour_point_t
{
  শূন্য init (দশমিক x_ = 0.f, দশমিক y_ = 0.f, যৌক্তিক is_end_point_ = মিথ্যা)
  { flag = 0; x = x_; y = y_; is_end_point = is_end_point_; }

  শূন্য transform (ধ্রুবক দশমিক (&matrix)[4])
  {
    দশমিক x_ = x * matrix[0] + y * matrix[2];
	  y  = x * matrix[1] + y * matrix[3];
    x  = x_;
  }

  শূন্য add_delta (দশমিক delta_x, দশমিক delta_y)
  {
    x += delta_x;
    y += delta_y;
  }

  HB_ALWAYS_INLINE
  শূন্য translate (ধ্রুবক contour_point_t &p) { x += p.x; y += p.y; }

  দশমিক x;
  দশমিক y;
  uint8_t flag;
  যৌক্তিক is_end_point;
};

কাঠামো contour_point_vector_t : hb_vector_t<contour_point_t>
{
  যৌক্তিক add_deltas (hb_array_t<ধ্রুবক দশমিক> deltas_x,
                   hb_array_t<ধ্রুবক দশমিক> deltas_y,
                   hb_array_t<ধ্রুবক যৌক্তিক> indices)
  {
    যদি (indices.দৈর্ঘ্য != deltas_x.দৈর্ঘ্য ||
        indices.দৈর্ঘ্য != deltas_y.দৈর্ঘ্য)
      ফেরত মিথ্যা;

    জন্য (unsigned i = 0; i < indices.দৈর্ঘ্য; i++)
    {
      যদি (!indices.arrayZ[i]) চালু;
      arrayZ[i].add_delta (deltas_x.arrayZ[i], deltas_y.arrayZ[i]);
    }
    ফেরত সত্য;
  }
};

নামাকাশ OT {
  কাঠামো cff1_subset_accelerator_t;
  কাঠামো cff2_subset_accelerator_t;
}

কাঠামো hb_subset_plan_t
{
  HB_INTERNAL hb_subset_plan_t (hb_face_t *,
				ধ্রুবক hb_subset_প্রবেশ_t *প্রবেশ);

  HB_INTERNAL ~hb_subset_plan_t();

  hb_object_header_t header;

  যৌক্তিক successful;
  unsigned flags;
  যৌক্তিক attach_accelerator_তথ্য = মিথ্যা;
  যৌক্তিক force_long_loca = মিথ্যা;

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

  unsigned পূর্ণসংখ্যা অঙ্ক_output_glyphs_2;

  যৌক্তিক all_axes_pinned;
  যৌক্তিক pinned_at_default;
  যৌক্তিক has_seac;

  // whether to insert a catch-all FeatureVariationRecord
  যৌক্তিক gsub_ঢোকাও_catch_all_feature_variation_rec;
  যৌক্তিক gpos_ঢোকাও_catch_all_feature_variation_rec;

  // whether GDEF ItemVariationStore is retained
  mutable যৌক্তিক has_gdef_varstore;

  // whether we're doing avar2 partial instancing
  যৌক্তিক has_avar2;

#define HB_SUBSET_PLAN_MEMBER(Type, Name) Type Name;
#অন্তর্ভাব "hb-subset-plan-member-list.hh"
#undef HB_SUBSET_PLAN_MEMBER

  //recalculated head/maxp table info after instancing
  mutable শীর্ষ_maxp_info_t শীর্ষ_maxp_info;

  os2_info_t os2_info;

  ধ্রুবক hb_subset_accelerator_t* accelerator = nullptr;
  hb_subset_accelerator_t* inprogress_accelerator = nullptr;

 সর্বজনীন:

  template<typename T>
  কাঠামো source_table_loader
  {
    hb_blob_ptr_t<T> operator () (hb_subset_plan_t *plan)
    {
      hb_lock_t lock (plan->accelerator ? &plan->accelerator->sanitized_table_cache_lock : nullptr);

      auto *cache = plan->accelerator ? &plan->accelerator->sanitized_table_cache : &plan->sanitized_table_cache;
      যদি (cache
	  && !cache->in_error ()
	  && cache->has (+T::tableTag)) {
	ফেরত hb_blob_reference (cache->get (+T::tableTag).get ());
      }

      hb::unique_ptr<hb_blob_t> table_blob {hb_sanitize_context_t ().reference_table<T> (plan->source)};
      hb_blob_t* ret = hb_blob_reference (table_blob.get ());

      যদি (likely (cache))
	cache->set (+T::tableTag, std::move (table_blob));

      ফেরত ret;
    }
  };

  template<typename T>
  auto source_table() HB_AUTO_RETURN (source_table_loader<T> {} (এই))

  যৌক্তিক in_error () ধ্রুবক { ফেরত !successful; }

  HB_INTERNAL দ্বিগুণ layout_variation_delta (unsigned var_idx);

  যৌক্তিক check_success(যৌক্তিক success)
  {
    successful = (successful && success);
    ফেরত successful;
  }

  /*
   * The set of input glyph ids which will be retained in the subset.
   * Does NOT include ids kept due to retain_gids. You probably want to use
   * glyph_map/reverse_glyph_map.
   */
  অন্তঃরেখ ধ্রুবক hb_set_t *
  glyphset () ধ্রুবক
  {
    ফেরত &_glyphset;
  }

  /*
   * The set of input glyph ids which will be retained in the subset.
   */
  অন্তঃরেখ ধ্রুবক hb_set_t *
  glyphset_gsub () ধ্রুবক
  {
    ফেরত &_glyphset_gsub;
  }

  /*
   * Old-to-new glyph id mapping for GSUB glyphs, through the flat
   * array; returns HB_MAP_VALUE_INVALID for glyphs not retained.
   */
  অন্তঃরেখ hb_codepoint_t
  map_gsub_glyph (hb_codepoint_t old_gid) ধ্রুবক
  {
    ফেরত old_gid < glyph_map_gsub_flat.দৈর্ঘ্য
	 ? glyph_map_gsub_flat.arrayZ[old_gid]
	 : HB_MAP_মান_INVALID;
  }

  /*
   * The total number of output glyphs in the final subset.
   */
  অন্তঃরেখ unsigned পূর্ণসংখ্যা
  অঙ্ক_output_glyphs () ধ্রুবক
  {
    ফেরত অঙ্ক_output_glyphs_2;
  }

  অন্তঃরেখ যৌক্তিক new_gid_for_codepoint (hb_codepoint_t codepoint,
				     hb_codepoint_t *new_gid) ধ্রুবক
  {
    hb_codepoint_t old_gid = codepoint_to_glyph->get (codepoint);
    যদি (old_gid == HB_MAP_মান_INVALID)
      ফেরত মিথ্যা;

    ফেরত new_gid_for_old_gid (old_gid, new_gid);
  }

  অন্তঃরেখ যৌক্তিক new_gid_for_old_gid (hb_codepoint_t old_gid,
				   hb_codepoint_t *new_gid) ধ্রুবক
  {
    hb_codepoint_t gid = glyph_map->get (old_gid);
    যদি (gid == HB_MAP_মান_INVALID)
      ফেরত মিথ্যা;

    *new_gid = gid;
    ফেরত সত্য;
  }

  অন্তঃরেখ যৌক্তিক old_gid_for_new_gid (hb_codepoint_t  new_gid,
				   hb_codepoint_t *old_gid) ধ্রুবক
  {
    hb_codepoint_t gid = reverse_glyph_map->get (new_gid);
    যদি (gid == HB_MAP_মান_INVALID)
      ফেরত মিথ্যা;

    *old_gid = gid;
    ফেরত সত্য;
  }

  অন্তঃরেখ যৌক্তিক
  add_table (hb_tag_t tag,
	     hb_blob_t *contents)
  {
    যদি (HB_DEBUG_SUBSET)
    {
      hb_blob_t *source_blob = source->reference_table (tag);
      DEBUG_বার্তা(SUBSET, nullptr, "add table %c%c%c%c, dest %u bytes, source %u bytes",
		HB_UNTAG(tag),
		hb_blob_get_দৈর্ঘ্য (contents),
		hb_blob_get_দৈর্ঘ্য (source_blob));
      hb_blob_destroy (source_blob);
    }
    ফেরত hb_face_builder_add_table (dest, tag, contents);
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
remap_indexes (ধ্রুবক hb_set_t *indexes,
               hb_map_t       *mapping /* OUT */);


#ifndef HB_NO_VAR
template<typename ItemVarStore>
HB_INTERNAL শূন্য
remap_variation_indices (ধ্রুবক ItemVarStore &var_store,
                         ধ্রুবক hb_set_t &variation_indices,
                         ধ্রুবক hb_vector_t<পূর্ণসংখ্যা>& normalized_coords,
                         যৌক্তিক calculate_delta, /* not pinned at default */
                         যৌক্তিক no_variations, /* all axes pinned */
                         hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণসংখ্যা>> &variation_idx_delta_map /* OUT */);


template<typename Delta_Set_সূচক_Map>
HB_INTERNAL শূন্য
remap_colrv1_delta_set_সূচক_indices (ধ্রুবক Delta_Set_সূচক_Map &সূচক_map,
                                      ধ্রুবক hb_set_t &delta_set_idxes,
                                      hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণসংখ্যা>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /* OUT */);


HB_INTERNAL শূন্য
generate_varstore_inner_maps (ধ্রুবক hb_set_t& varidx_set,
                              unsigned subtable_গুনতি,
                              hb_vector_t<hb_inc_bimap_t> &inner_maps /* OUT */);

HB_INTERNAL যৌক্তিক
normalize_axes_location (hb_face_t *face, hb_subset_plan_t *plan);

HB_INTERNAL শূন্য
update_instance_metrics_map_from_cff2 (hb_subset_plan_t *plan);

HB_INTERNAL যৌক্তিক
get_instance_glyphs_contour_points (hb_subset_plan_t *plan);

#ifndef HB_NO_BASE
HB_INTERNAL শূন্য
collect_base_variation_indices (hb_subset_plan_t* plan);
#endif
#endif

#ifndef HB_NO_SUBSET_LAYOUT
প্রকারলেখ hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> script_langsys_map;

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

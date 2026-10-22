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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ଅନ୍ତର୍ଭାଵ "hb-subset.h"
#ଅନ୍ତର୍ଭାଵ "hb-subset-input.hh"
#ଅନ୍ତର୍ଭାଵ "hb-subset-accelerator.hh"

#ଅନ୍ତର୍ଭାଵ "hb-map.hh"
#ଅନ୍ତର୍ଭାଵ "hb-bimap.hh"
#ଅନ୍ତର୍ଭାଵ "hb-set.hh"

ନାମାକାଶ OT {
ସଂରଚନା Feature;
}

ସଂରଚନା os2_info_t {
  hb_codepoint_t min_cmap_codepoint;
  hb_codepoint_t max_cmap_codepoint;
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା os2_info_t os2_info_t;

ସଂରଚନା head_maxp_info_t
{
  head_maxp_info_t ()
      :xMin (0x7FFF), xMax (-0x7FFF), yMin (0x7FFF), yMax (-0x7FFF),
      maxPoints (0), maxContours (0),
      maxCompositePoints (0),
      maxCompositeContours (0),
      maxComponentElements (0),
      maxComponentDepth (0),
      allXMinIsLsb (ସତ୍ୟ) {}

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା xMin;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା xMax;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା yMin;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା yMax;
  unsigned maxPoints;
  unsigned maxContours;
  unsigned maxCompositePoints;
  unsigned maxCompositeContours;
  unsigned maxComponentElements;
  unsigned maxComponentDepth;
  ତାର୍କିକ allXMinIsLsb;
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା head_maxp_info_t head_maxp_info_t;

ସଂରଚନା contour_point_t
{
  ଶୂନ୍ୟ init (ଦଶମିକ x_ = 0.f, ଦଶମିକ y_ = 0.f, ତାର୍କିକ is_end_point_ = ମିଥ୍ୟା)
  { flag = 0; x = x_; y = y_; is_end_point = is_end_point_; }

  ଶୂନ୍ୟ transform (ଅଚଳ ଦଶମିକ (&matrix)[4])
  {
    ଦଶମିକ x_ = x * matrix[0] + y * matrix[2];
	  y  = x * matrix[1] + y * matrix[3];
    x  = x_;
  }

  ଶୂନ୍ୟ add_delta (ଦଶମିକ delta_x, ଦଶମିକ delta_y)
  {
    x += delta_x;
    y += delta_y;
  }

  HB_ALWAYS_INLINE
  ଶୂନ୍ୟ translate (ଅଚଳ contour_point_t &p) { x += p.x; y += p.y; }

  ଦଶମିକ x;
  ଦଶମିକ y;
  uint8_t flag;
  ତାର୍କିକ is_end_point;
};

ସଂରଚନା contour_point_vector_t : hb_vector_t<contour_point_t>
{
  ତାର୍କିକ add_deltas (hb_array_t<ଅଚଳ ଦଶମିକ> deltas_x,
                   hb_array_t<ଅଚଳ ଦଶମିକ> deltas_y,
                   hb_array_t<ଅଚଳ ତାର୍କିକ> indices)
  {
    ଯଦି (indices.length != deltas_x.length ||
        indices.length != deltas_y.length)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ପାଇଁ (unsigned i = 0; i < indices.length; i++)
    {
      ଯଦି (!indices.arrayZ[i]) ଜାରି;
      arrayZ[i].add_delta (deltas_x.arrayZ[i], deltas_y.arrayZ[i]);
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }
};

ନାମାକାଶ OT {
  ସଂରଚନା cff1_subset_accelerator_t;
  ସଂରଚନା cff2_subset_accelerator_t;
}

ସଂରଚନା hb_subset_plan_t
{
  HB_INTERNAL hb_subset_plan_t (hb_face_t *,
				ଅଚଳ hb_subset_ନିବେଶ_t *ନିବେଶ);

  HB_INTERNAL ~hb_subset_plan_t();

  hb_object_header_t header;

  ତାର୍କିକ successful;
  unsigned flags;
  ତାର୍କିକ attach_accelerator_data = ମିଥ୍ୟା;
  ତାର୍କିକ force_long_loca = ମିଥ୍ୟା;

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

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_output_glyphs_2;

  ତାର୍କିକ all_axes_pinned;
  ତାର୍କିକ pinned_at_default;
  ତାର୍କିକ has_seac;

  // whether to insert a catch-all FeatureVariationRecord
  ତାର୍କିକ gsub_ଭର୍ତ୍ତି_catch_all_feature_variation_rec;
  ତାର୍କିକ gpos_ଭର୍ତ୍ତି_catch_all_feature_variation_rec;

  // whether GDEF ItemVariationStore is retained
  mutable ତାର୍କିକ has_gdef_varstore;

  // whether we're doing avar2 partial instancing
  ତାର୍କିକ has_avar2;

#define HB_SUBSET_PLAN_MEMBER(Type, Name) Type Name;
#ଅନ୍ତର୍ଭାଵ "hb-subset-plan-member-list.hh"
#undef HB_SUBSET_PLAN_MEMBER

  //recalculated head/maxp table info after instancing
  mutable head_maxp_info_t head_maxp_info;

  os2_info_t os2_info;

  ଅଚଳ hb_subset_accelerator_t* accelerator = nullptr;
  hb_subset_accelerator_t* inprogress_accelerator = nullptr;

 ସର୍ବସାଧାରଣ:

  template<typename T>
  ସଂରଚନା source_table_loader
  {
    hb_blob_ptr_t<T> operator () (hb_subset_plan_t *plan)
    {
      hb_lock_t lock (plan->accelerator ? &plan->accelerator->sanitized_table_cache_lock : nullptr);

      auto *cache = plan->accelerator ? &plan->accelerator->sanitized_table_cache : &plan->sanitized_table_cache;
      ଯଦି (cache
	  && !cache->in_error ()
	  && cache->has (+T::tableTag)) {
	ଫେରନ୍ତୁ hb_blob_reference (cache->get (+T::tableTag).get ());
      }

      hb::unique_ptr<hb_blob_t> table_blob {hb_sanitize_context_t ().reference_table<T> (plan->source)};
      hb_blob_t* ret = hb_blob_reference (table_blob.get ());

      ଯଦି (likely (cache))
	cache->set (+T::tableTag, std::move (table_blob));

      ଫେରନ୍ତୁ ret;
    }
  };

  template<typename T>
  auto source_table() HB_AUTO_RETURN (source_table_loader<T> {} (ଏହି))

  ତାର୍କିକ in_error () ଅଚଳ { ଫେରନ୍ତୁ !successful; }

  HB_INTERNAL ଦ୍ୱିଗୁଣ layout_variation_delta (unsigned var_idx);

  ତାର୍କିକ check_success(ତାର୍କିକ success)
  {
    successful = (successful && success);
    ଫେରନ୍ତୁ successful;
  }

  /*
   * The set of input glyph ids which will be retained in the subset.
   * Does NOT include ids kept due to retain_gids. You probably want to use
   * glyph_map/reverse_glyph_map.
   */
  ଧାଡ଼ିଭିତରେ ଅଚଳ hb_set_t *
  glyphset () ଅଚଳ
  {
    ଫେରନ୍ତୁ &_glyphset;
  }

  /*
   * The set of input glyph ids which will be retained in the subset.
   */
  ଧାଡ଼ିଭିତରେ ଅଚଳ hb_set_t *
  glyphset_gsub () ଅଚଳ
  {
    ଫେରନ୍ତୁ &_glyphset_gsub;
  }

  /*
   * Old-to-new glyph id mapping for GSUB glyphs, through the flat
   * array; returns HB_MAP_VALUE_INVALID for glyphs not retained.
   */
  ଧାଡ଼ିଭିତରେ hb_codepoint_t
  map_gsub_glyph (hb_codepoint_t old_gid) ଅଚଳ
  {
    ଫେରନ୍ତୁ old_gid < glyph_map_gsub_flat.length
	 ? glyph_map_gsub_flat.arrayZ[old_gid]
	 : HB_MAP_ମୂଲ୍ୟ_INVALID;
  }

  /*
   * The total number of output glyphs in the final subset.
   */
  ଧାଡ଼ିଭିତରେ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
  ଅଙ୍କ_output_glyphs () ଅଚଳ
  {
    ଫେରନ୍ତୁ ଅଙ୍କ_output_glyphs_2;
  }

  ଧାଡ଼ିଭିତରେ ତାର୍କିକ new_gid_for_codepoint (hb_codepoint_t codepoint,
				     hb_codepoint_t *new_gid) ଅଚଳ
  {
    hb_codepoint_t old_gid = codepoint_to_glyph->get (codepoint);
    ଯଦି (old_gid == HB_MAP_ମୂଲ୍ୟ_INVALID)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଫେରନ୍ତୁ new_gid_for_old_gid (old_gid, new_gid);
  }

  ଧାଡ଼ିଭିତରେ ତାର୍କିକ new_gid_for_old_gid (hb_codepoint_t old_gid,
				   hb_codepoint_t *new_gid) ଅଚଳ
  {
    hb_codepoint_t gid = glyph_map->get (old_gid);
    ଯଦି (gid == HB_MAP_ମୂଲ୍ୟ_INVALID)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    *new_gid = gid;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ଧାଡ଼ିଭିତରେ ତାର୍କିକ old_gid_for_new_gid (hb_codepoint_t  new_gid,
				   hb_codepoint_t *old_gid) ଅଚଳ
  {
    hb_codepoint_t gid = reverse_glyph_map->get (new_gid);
    ଯଦି (gid == HB_MAP_ମୂଲ୍ୟ_INVALID)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    *old_gid = gid;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ଧାଡ଼ିଭିତରେ ତାର୍କିକ
  add_table (hb_tag_t tag,
	     hb_blob_t *contents)
  {
    ଯଦି (HB_DEBUG_SUBSET)
    {
      hb_blob_t *source_blob = source->reference_table (tag);
      DEBUG_ସନ୍ଦେଶ(SUBSET, nullptr, "add table %c%c%c%c, dest %u bytes, source %u bytes",
		HB_UNTAG(tag),
		hb_blob_get_length (contents),
		hb_blob_get_length (source_blob));
      hb_blob_destroy (source_blob);
    }
    ଫେରନ୍ତୁ hb_face_builder_add_table (dest, tag, contents);
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
HB_INTERNAL ଶୂନ୍ୟ
remap_indexes (ଅଚଳ hb_set_t *indexes,
               hb_map_t       *mapping /* OUT */);


#ifndef HB_NO_VAR
template<typename ItemVarStore>
HB_INTERNAL ଶୂନ୍ୟ
remap_variation_indices (ଅଚଳ ItemVarStore &var_store,
                         ଅଚଳ hb_set_t &variation_indices,
                         ଅଚଳ hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& normalized_coords,
                         ତାର୍କିକ calculate_delta, /* not pinned at default */
                         ତାର୍କିକ no_variations, /* all axes pinned */
                         hb_hashmap_t<unsigned, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> &variation_idx_delta_map /* OUT */);


template<typename DeltaSetIndexMap>
HB_INTERNAL ଶୂନ୍ୟ
remap_colrv1_delta_set_index_indices (ଅଚଳ DeltaSetIndexMap &index_map,
                                      ଅଚଳ hb_set_t &delta_set_idxes,
                                      hb_hashmap_t<unsigned, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /* OUT */);


HB_INTERNAL ଶୂନ୍ୟ
generate_varstore_inner_maps (ଅଚଳ hb_set_t& varidx_set,
                              unsigned subtable_ଗଣନା,
                              hb_vector_t<hb_inc_bimap_t> &inner_maps /* OUT */);

HB_INTERNAL ତାର୍କିକ
normalize_axes_location (hb_face_t *face, hb_subset_plan_t *plan);

HB_INTERNAL ଶୂନ୍ୟ
update_instance_metrics_map_from_cff2 (hb_subset_plan_t *plan);

HB_INTERNAL ତାର୍କିକ
get_instance_glyphs_contour_points (hb_subset_plan_t *plan);

#ifndef HB_NO_BASE
HB_INTERNAL ଶୂନ୍ୟ
collect_base_variation_indices (hb_subset_plan_t* plan);
#endif
#endif

#ifndef HB_NO_SUBSET_LAYOUT
ପ୍ରକାରବ୍ୟାଖ୍ୟା hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> script_langsys_map;

HB_INTERNAL ଶୂନ୍ୟ
remap_used_mark_sets (hb_subset_plan_t *plan,
                      hb_map_t& used_mark_sets_map);

HB_INTERNAL ଶୂନ୍ୟ
layout_nameid_closure (hb_subset_plan_t* plan,
                       hb_set_t* drop_tables);

HB_INTERNAL ଶୂନ୍ୟ
layout_populate_gids_to_retain (hb_subset_plan_t* plan,
                                hb_set_t* drop_tables);

HB_INTERNAL ଶୂନ୍ୟ
collect_layout_variation_indices (hb_subset_plan_t* plan);
#endif


#endif /* HB_SUBSET_PLAN_HH */

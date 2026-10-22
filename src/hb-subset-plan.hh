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

#અન્તર્ભાવ "hb.hh"

#અન્તર્ભાવ "hb-subset.h"
#અન્તર્ભાવ "hb-subset-input.hh"
#અન્તર્ભાવ "hb-subset-accelerator.hh"

#અન્તર્ભાવ "hb-map.hh"
#અન્તર્ભાવ "hb-bimap.hh"
#અન્તર્ભાવ "hb-set.hh"

નામાકાશ OT {
સંરચના Feature;
}

સંરચના os2_info_t {
  hb_codepoint_t min_cmap_codepoint;
  hb_codepoint_t max_cmap_codepoint;
};

પ્રકારવ્યાખ્યા સંરચના os2_info_t os2_info_t;

સંરચના head_maxp_info_t
{
  head_maxp_info_t ()
      :xMin (0x7FFF), xMax (-0x7FFF), yMin (0x7FFF), yMax (-0x7FFF),
      maxPoints (0), maxContours (0),
      maxCompositePoints (0),
      maxCompositeContours (0),
      maxComponentElements (0),
      maxComponentDepth (0),
      allXMinIsLsb (સાચું) {}

  પૂર્ણાંક xMin;
  પૂર્ણાંક xMax;
  પૂર્ણાંક yMin;
  પૂર્ણાંક yMax;
  unsigned maxPoints;
  unsigned maxContours;
  unsigned maxCompositePoints;
  unsigned maxCompositeContours;
  unsigned maxComponentElements;
  unsigned maxComponentDepth;
  તાર્કિક allXMinIsLsb;
};

પ્રકારવ્યાખ્યા સંરચના head_maxp_info_t head_maxp_info_t;

સંરચના contour_point_t
{
  શૂન્ય init (દશાંશ x_ = 0.f, દશાંશ y_ = 0.f, તાર્કિક is_end_point_ = ખોટું)
  { flag = 0; x = x_; y = y_; is_end_point = is_end_point_; }

  શૂન્ય transform (અચલ દશાંશ (&matrix)[4])
  {
    દશાંશ x_ = x * matrix[0] + y * matrix[2];
	  y  = x * matrix[1] + y * matrix[3];
    x  = x_;
  }

  શૂન્ય add_delta (દશાંશ delta_x, દશાંશ delta_y)
  {
    x += delta_x;
    y += delta_y;
  }

  HB_ALWAYS_INLINE
  શૂન્ય translate (અચલ contour_point_t &p) { x += p.x; y += p.y; }

  દશાંશ x;
  દશાંશ y;
  uint8_t flag;
  તાર્કિક is_end_point;
};

સંરચના contour_point_vector_t : hb_vector_t<contour_point_t>
{
  તાર્કિક add_deltas (hb_array_t<અચલ દશાંશ> deltas_x,
                   hb_array_t<અચલ દશાંશ> deltas_y,
                   hb_array_t<અચલ તાર્કિક> indices)
  {
    જો (indices.length != deltas_x.length ||
        indices.length != deltas_y.length)
      પાછા ખોટું;

    માટે (unsigned i = 0; i < indices.length; i++)
    {
      જો (!indices.arrayZ[i]) ચાલુ;
      arrayZ[i].add_delta (deltas_x.arrayZ[i], deltas_y.arrayZ[i]);
    }
    પાછા સાચું;
  }
};

નામાકાશ OT {
  સંરચના cff1_subset_accelerator_t;
  સંરચના cff2_subset_accelerator_t;
}

સંરચના hb_subset_plan_t
{
  HB_INTERNAL hb_subset_plan_t (hb_face_t *,
				અચલ hb_subset_આગત_t *આગત);

  HB_INTERNAL ~hb_subset_plan_t();

  hb_object_header_t header;

  તાર્કિક successful;
  unsigned flags;
  તાર્કિક attach_accelerator_data = ખોટું;
  તાર્કિક force_long_loca = ખોટું;

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

  unsigned પૂર્ણાંક અંક_output_glyphs_2;

  તાર્કિક all_axes_pinned;
  તાર્કિક pinned_at_default;
  તાર્કિક has_seac;

  // whether to insert a catch-all FeatureVariationRecord
  તાર્કિક gsub_ઉમેરો_catch_all_feature_variation_rec;
  તાર્કિક gpos_ઉમેરો_catch_all_feature_variation_rec;

  // whether GDEF ItemVariationStore is retained
  mutable તાર્કિક has_gdef_varstore;

  // whether we're doing avar2 partial instancing
  તાર્કિક has_avar2;

#define HB_SUBSET_PLAN_MEMBER(Type, Name) Type Name;
#અન્તર્ભાવ "hb-subset-plan-member-list.hh"
#undef HB_SUBSET_PLAN_MEMBER

  //recalculated head/maxp table info after instancing
  mutable head_maxp_info_t head_maxp_info;

  os2_info_t os2_info;

  અચલ hb_subset_accelerator_t* accelerator = nullptr;
  hb_subset_accelerator_t* inprogress_accelerator = nullptr;

 જાહેર:

  template<typename T>
  સંરચના source_table_loader
  {
    hb_blob_ptr_t<T> operator () (hb_subset_plan_t *plan)
    {
      hb_lock_t lock (plan->accelerator ? &plan->accelerator->sanitized_table_cache_lock : nullptr);

      auto *cache = plan->accelerator ? &plan->accelerator->sanitized_table_cache : &plan->sanitized_table_cache;
      જો (cache
	  && !cache->in_error ()
	  && cache->has (+T::tableTag)) {
	પાછા hb_blob_reference (cache->get (+T::tableTag).get ());
      }

      hb::unique_ptr<hb_blob_t> table_blob {hb_sanitize_context_t ().reference_table<T> (plan->source)};
      hb_blob_t* ret = hb_blob_reference (table_blob.get ());

      જો (likely (cache))
	cache->set (+T::tableTag, std::move (table_blob));

      પાછા ret;
    }
  };

  template<typename T>
  auto source_table() HB_AUTO_RETURN (source_table_loader<T> {} (આ))

  તાર્કિક in_error () અચલ { પાછા !successful; }

  HB_INTERNAL દ્વિગુણ layout_variation_delta (unsigned var_idx);

  તાર્કિક check_success(તાર્કિક success)
  {
    successful = (successful && success);
    પાછા successful;
  }

  /*
   * The set of input glyph ids which will be retained in the subset.
   * Does NOT include ids kept due to retain_gids. You probably want to use
   * glyph_map/reverse_glyph_map.
   */
  અંતઃરેખ અચલ hb_set_t *
  glyphset () અચલ
  {
    પાછા &_glyphset;
  }

  /*
   * The set of input glyph ids which will be retained in the subset.
   */
  અંતઃરેખ અચલ hb_set_t *
  glyphset_gsub () અચલ
  {
    પાછા &_glyphset_gsub;
  }

  /*
   * Old-to-new glyph id mapping for GSUB glyphs, through the flat
   * array; returns HB_MAP_VALUE_INVALID for glyphs not retained.
   */
  અંતઃરેખ hb_codepoint_t
  map_gsub_glyph (hb_codepoint_t old_gid) અચલ
  {
    પાછા old_gid < glyph_map_gsub_flat.length
	 ? glyph_map_gsub_flat.arrayZ[old_gid]
	 : HB_MAP_કિંમત_INVALID;
  }

  /*
   * The total number of output glyphs in the final subset.
   */
  અંતઃરેખ unsigned પૂર્ણાંક
  અંક_output_glyphs () અચલ
  {
    પાછા અંક_output_glyphs_2;
  }

  અંતઃરેખ તાર્કિક new_gid_for_codepoint (hb_codepoint_t codepoint,
				     hb_codepoint_t *new_gid) અચલ
  {
    hb_codepoint_t old_gid = codepoint_to_glyph->get (codepoint);
    જો (old_gid == HB_MAP_કિંમત_INVALID)
      પાછા ખોટું;

    પાછા new_gid_for_old_gid (old_gid, new_gid);
  }

  અંતઃરેખ તાર્કિક new_gid_for_old_gid (hb_codepoint_t old_gid,
				   hb_codepoint_t *new_gid) અચલ
  {
    hb_codepoint_t gid = glyph_map->get (old_gid);
    જો (gid == HB_MAP_કિંમત_INVALID)
      પાછા ખોટું;

    *new_gid = gid;
    પાછા સાચું;
  }

  અંતઃરેખ તાર્કિક old_gid_for_new_gid (hb_codepoint_t  new_gid,
				   hb_codepoint_t *old_gid) અચલ
  {
    hb_codepoint_t gid = reverse_glyph_map->get (new_gid);
    જો (gid == HB_MAP_કિંમત_INVALID)
      પાછા ખોટું;

    *old_gid = gid;
    પાછા સાચું;
  }

  અંતઃરેખ તાર્કિક
  add_table (hb_tag_t tag,
	     hb_blob_t *contents)
  {
    જો (HB_DEBUG_SUBSET)
    {
      hb_blob_t *source_blob = source->reference_table (tag);
      DEBUG_સંદેશ(SUBSET, nullptr, "add table %c%c%c%c, dest %u bytes, source %u bytes",
		HB_UNTAG(tag),
		hb_blob_get_length (contents),
		hb_blob_get_length (source_blob));
      hb_blob_destroy (source_blob);
    }
    પાછા hb_face_builder_add_table (dest, tag, contents);
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
HB_INTERNAL શૂન્ય
remap_indexes (અચલ hb_set_t *indexes,
               hb_map_t       *mapping /* OUT */);


#ifndef HB_NO_VAR
template<typename ItemVarStore>
HB_INTERNAL શૂન્ય
remap_variation_indices (અચલ ItemVarStore &var_store,
                         અચલ hb_set_t &variation_indices,
                         અચલ hb_vector_t<પૂર્ણાંક>& normalized_coords,
                         તાર્કિક calculate_delta, /* not pinned at default */
                         તાર્કિક no_variations, /* all axes pinned */
                         hb_hashmap_t<unsigned, hb_pair_t<unsigned, પૂર્ણાંક>> &variation_idx_delta_map /* OUT */);


template<typename DeltaSetIndexMap>
HB_INTERNAL શૂન્ય
remap_colrv1_delta_set_index_indices (અચલ DeltaSetIndexMap &index_map,
                                      અચલ hb_set_t &delta_set_idxes,
                                      hb_hashmap_t<unsigned, hb_pair_t<unsigned, પૂર્ણાંક>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /* OUT */);


HB_INTERNAL શૂન્ય
generate_varstore_inner_maps (અચલ hb_set_t& varidx_set,
                              unsigned subtable_ગણતરી,
                              hb_vector_t<hb_inc_bimap_t> &inner_maps /* OUT */);

HB_INTERNAL તાર્કિક
normalize_axes_location (hb_face_t *face, hb_subset_plan_t *plan);

HB_INTERNAL શૂન્ય
update_instance_metrics_map_from_cff2 (hb_subset_plan_t *plan);

HB_INTERNAL તાર્કિક
get_instance_glyphs_contour_points (hb_subset_plan_t *plan);

#ifndef HB_NO_BASE
HB_INTERNAL શૂન્ય
collect_base_variation_indices (hb_subset_plan_t* plan);
#endif
#endif

#ifndef HB_NO_SUBSET_LAYOUT
પ્રકારવ્યાખ્યા hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> script_langsys_map;

HB_INTERNAL શૂન્ય
remap_used_mark_sets (hb_subset_plan_t *plan,
                      hb_map_t& used_mark_sets_map);

HB_INTERNAL શૂન્ય
layout_nameid_closure (hb_subset_plan_t* plan,
                       hb_set_t* drop_tables);

HB_INTERNAL શૂન્ય
layout_populate_gids_to_retain (hb_subset_plan_t* plan,
                                hb_set_t* drop_tables);

HB_INTERNAL શૂન્ય
collect_layout_variation_indices (hb_subset_plan_t* plan);
#endif


#endif /* HB_SUBSET_PLAN_HH */

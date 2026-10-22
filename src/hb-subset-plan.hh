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

#അന്തര്ഭാവ "hb.hh"

#അന്തര്ഭാവ "hb-subset.h"
#അന്തര്ഭാവ "hb-subset-input.hh"
#അന്തര്ഭാവ "hb-subset-accelerator.hh"

#അന്തര്ഭാവ "hb-map.hh"
#അന്തര്ഭാവ "hb-bimap.hh"
#അന്തര്ഭാവ "hb-set.hh"

നാമാകാശ OT {
ഘടന Feature;
}

ഘടന os2_info_t {
  hb_codepoint_t min_cmap_codepoint;
  hb_codepoint_t max_cmap_codepoint;
};

തരനിർവചനം ഘടന os2_info_t os2_info_t;

ഘടന head_maxp_info_t
{
  head_maxp_info_t ()
      :xMin (0x7FFF), xMax (-0x7FFF), yMin (0x7FFF), yMax (-0x7FFF),
      maxPoints (0), maxContours (0),
      maxCompositePoints (0),
      maxCompositeContours (0),
      maxComponentElements (0),
      maxComponentDepth (0),
      allXMinIsLsb (സത്യം) {}

  പൂർണ്ണസംഖ്യ xMin;
  പൂർണ്ണസംഖ്യ xMax;
  പൂർണ്ണസംഖ്യ yMin;
  പൂർണ്ണസംഖ്യ yMax;
  unsigned maxPoints;
  unsigned maxContours;
  unsigned maxCompositePoints;
  unsigned maxCompositeContours;
  unsigned maxComponentElements;
  unsigned maxComponentDepth;
  തർക്കം allXMinIsLsb;
};

തരനിർവചനം ഘടന head_maxp_info_t head_maxp_info_t;

ഘടന contour_point_t
{
  ശൂന്യം init (ദശാംശം x_ = 0.f, ദശാംശം y_ = 0.f, തർക്കം is_end_point_ = അസത്യം)
  { flag = 0; x = x_; y = y_; is_end_point = is_end_point_; }

  ശൂന്യം transform (സ്ഥിരം ദശാംശം (&matrix)[4])
  {
    ദശാംശം x_ = x * matrix[0] + y * matrix[2];
	  y  = x * matrix[1] + y * matrix[3];
    x  = x_;
  }

  ശൂന്യം add_delta (ദശാംശം delta_x, ദശാംശം delta_y)
  {
    x += delta_x;
    y += delta_y;
  }

  HB_ALWAYS_INLINE
  ശൂന്യം translate (സ്ഥിരം contour_point_t &p) { x += p.x; y += p.y; }

  ദശാംശം x;
  ദശാംശം y;
  uint8_t flag;
  തർക്കം is_end_point;
};

ഘടന contour_point_vector_t : hb_vector_t<contour_point_t>
{
  തർക്കം add_deltas (hb_array_t<സ്ഥിരം ദശാംശം> deltas_x,
                   hb_array_t<സ്ഥിരം ദശാംശം> deltas_y,
                   hb_array_t<സ്ഥിരം തർക്കം> indices)
  {
    എങ്കിൽ (indices.length != deltas_x.length ||
        indices.length != deltas_y.length)
      തിരികെ അസത്യം;

    വേണ്ടി (unsigned i = 0; i < indices.length; i++)
    {
      എങ്കിൽ (!indices.arrayZ[i]) തുടരുക;
      arrayZ[i].add_delta (deltas_x.arrayZ[i], deltas_y.arrayZ[i]);
    }
    തിരികെ സത്യം;
  }
};

നാമാകാശ OT {
  ഘടന cff1_subset_accelerator_t;
  ഘടന cff2_subset_accelerator_t;
}

ഘടന hb_subset_plan_t
{
  HB_INTERNAL hb_subset_plan_t (hb_face_t *,
				സ്ഥിരം hb_subset_നിവേശം_t *നിവേശം);

  HB_INTERNAL ~hb_subset_plan_t();

  hb_object_header_t header;

  തർക്കം successful;
  unsigned flags;
  തർക്കം attach_accelerator_data = അസത്യം;
  തർക്കം force_long_loca = അസത്യം;

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

  unsigned പൂർണ്ണസംഖ്യ അക്കം_output_glyphs_2;

  തർക്കം all_axes_pinned;
  തർക്കം pinned_at_default;
  തർക്കം has_seac;

  // whether to insert a catch-all FeatureVariationRecord
  തർക്കം gsub_തിരുകുക_catch_all_feature_variation_rec;
  തർക്കം gpos_തിരുകുക_catch_all_feature_variation_rec;

  // whether GDEF ItemVariationStore is retained
  mutable തർക്കം has_gdef_varstore;

  // whether we're doing avar2 partial instancing
  തർക്കം has_avar2;

#define HB_SUBSET_PLAN_MEMBER(Type, Name) Type Name;
#അന്തര്ഭാവ "hb-subset-plan-member-list.hh"
#undef HB_SUBSET_PLAN_MEMBER

  //recalculated head/maxp table info after instancing
  mutable head_maxp_info_t head_maxp_info;

  os2_info_t os2_info;

  സ്ഥിരം hb_subset_accelerator_t* accelerator = nullptr;
  hb_subset_accelerator_t* inprogress_accelerator = nullptr;

 പൊതു:

  template<typename T>
  ഘടന source_table_loader
  {
    hb_blob_ptr_t<T> operator () (hb_subset_plan_t *plan)
    {
      hb_lock_t lock (plan->accelerator ? &plan->accelerator->sanitized_table_cache_lock : nullptr);

      auto *cache = plan->accelerator ? &plan->accelerator->sanitized_table_cache : &plan->sanitized_table_cache;
      എങ്കിൽ (cache
	  && !cache->in_error ()
	  && cache->has (+T::tableTag)) {
	തിരികെ hb_blob_reference (cache->get (+T::tableTag).get ());
      }

      hb::unique_ptr<hb_blob_t> table_blob {hb_sanitize_context_t ().reference_table<T> (plan->source)};
      hb_blob_t* ret = hb_blob_reference (table_blob.get ());

      എങ്കിൽ (likely (cache))
	cache->set (+T::tableTag, std::move (table_blob));

      തിരികെ ret;
    }
  };

  template<typename T>
  auto source_table() HB_AUTO_RETURN (source_table_loader<T> {} (ഇത്))

  തർക്കം in_error () സ്ഥിരം { തിരികെ !successful; }

  HB_INTERNAL ഇരട്ട layout_variation_delta (unsigned var_idx);

  തർക്കം check_success(തർക്കം success)
  {
    successful = (successful && success);
    തിരികെ successful;
  }

  /*
   * The set of input glyph ids which will be retained in the subset.
   * Does NOT include ids kept due to retain_gids. You probably want to use
   * glyph_map/reverse_glyph_map.
   */
  വരിയിൽ സ്ഥിരം hb_set_t *
  glyphset () സ്ഥിരം
  {
    തിരികെ &_glyphset;
  }

  /*
   * The set of input glyph ids which will be retained in the subset.
   */
  വരിയിൽ സ്ഥിരം hb_set_t *
  glyphset_gsub () സ്ഥിരം
  {
    തിരികെ &_glyphset_gsub;
  }

  /*
   * Old-to-new glyph id mapping for GSUB glyphs, through the flat
   * array; returns HB_MAP_VALUE_INVALID for glyphs not retained.
   */
  വരിയിൽ hb_codepoint_t
  map_gsub_glyph (hb_codepoint_t old_gid) സ്ഥിരം
  {
    തിരികെ old_gid < glyph_map_gsub_flat.length
	 ? glyph_map_gsub_flat.arrayZ[old_gid]
	 : HB_MAP_മൂല്യം_INVALID;
  }

  /*
   * The total number of output glyphs in the final subset.
   */
  വരിയിൽ unsigned പൂർണ്ണസംഖ്യ
  അക്കം_output_glyphs () സ്ഥിരം
  {
    തിരികെ അക്കം_output_glyphs_2;
  }

  വരിയിൽ തർക്കം new_gid_for_codepoint (hb_codepoint_t codepoint,
				     hb_codepoint_t *new_gid) സ്ഥിരം
  {
    hb_codepoint_t old_gid = codepoint_to_glyph->get (codepoint);
    എങ്കിൽ (old_gid == HB_MAP_മൂല്യം_INVALID)
      തിരികെ അസത്യം;

    തിരികെ new_gid_for_old_gid (old_gid, new_gid);
  }

  വരിയിൽ തർക്കം new_gid_for_old_gid (hb_codepoint_t old_gid,
				   hb_codepoint_t *new_gid) സ്ഥിരം
  {
    hb_codepoint_t gid = glyph_map->get (old_gid);
    എങ്കിൽ (gid == HB_MAP_മൂല്യം_INVALID)
      തിരികെ അസത്യം;

    *new_gid = gid;
    തിരികെ സത്യം;
  }

  വരിയിൽ തർക്കം old_gid_for_new_gid (hb_codepoint_t  new_gid,
				   hb_codepoint_t *old_gid) സ്ഥിരം
  {
    hb_codepoint_t gid = reverse_glyph_map->get (new_gid);
    എങ്കിൽ (gid == HB_MAP_മൂല്യം_INVALID)
      തിരികെ അസത്യം;

    *old_gid = gid;
    തിരികെ സത്യം;
  }

  വരിയിൽ തർക്കം
  add_table (hb_tag_t tag,
	     hb_blob_t *contents)
  {
    എങ്കിൽ (HB_DEBUG_SUBSET)
    {
      hb_blob_t *source_blob = source->reference_table (tag);
      DEBUG_സന്ദേശം(SUBSET, nullptr, "add table %c%c%c%c, dest %u bytes, source %u bytes",
		HB_UNTAG(tag),
		hb_blob_get_length (contents),
		hb_blob_get_length (source_blob));
      hb_blob_destroy (source_blob);
    }
    തിരികെ hb_face_builder_add_table (dest, tag, contents);
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
HB_INTERNAL ശൂന്യം
remap_indexes (സ്ഥിരം hb_set_t *indexes,
               hb_map_t       *mapping /* OUT */);


#ifndef HB_NO_VAR
template<typename ItemVarStore>
HB_INTERNAL ശൂന്യം
remap_variation_indices (സ്ഥിരം ItemVarStore &var_store,
                         സ്ഥിരം hb_set_t &variation_indices,
                         സ്ഥിരം hb_vector_t<പൂർണ്ണസംഖ്യ>& normalized_coords,
                         തർക്കം calculate_delta, /* not pinned at default */
                         തർക്കം no_variations, /* all axes pinned */
                         hb_hashmap_t<unsigned, hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ>> &variation_idx_delta_map /* OUT */);


template<typename DeltaSetIndexMap>
HB_INTERNAL ശൂന്യം
remap_colrv1_delta_set_index_indices (സ്ഥിരം DeltaSetIndexMap &index_map,
                                      സ്ഥിരം hb_set_t &delta_set_idxes,
                                      hb_hashmap_t<unsigned, hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /* OUT */);


HB_INTERNAL ശൂന്യം
generate_varstore_inner_maps (സ്ഥിരം hb_set_t& varidx_set,
                              unsigned subtable_എണ്ണം,
                              hb_vector_t<hb_inc_bimap_t> &inner_maps /* OUT */);

HB_INTERNAL തർക്കം
normalize_axes_location (hb_face_t *face, hb_subset_plan_t *plan);

HB_INTERNAL ശൂന്യം
update_instance_metrics_map_from_cff2 (hb_subset_plan_t *plan);

HB_INTERNAL തർക്കം
get_instance_glyphs_contour_points (hb_subset_plan_t *plan);

#ifndef HB_NO_BASE
HB_INTERNAL ശൂന്യം
collect_base_variation_indices (hb_subset_plan_t* plan);
#endif
#endif

#ifndef HB_NO_SUBSET_LAYOUT
തരനിർവചനം hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> script_langsys_map;

HB_INTERNAL ശൂന്യം
remap_used_mark_sets (hb_subset_plan_t *plan,
                      hb_map_t& used_mark_sets_map);

HB_INTERNAL ശൂന്യം
layout_nameid_closure (hb_subset_plan_t* plan,
                       hb_set_t* drop_tables);

HB_INTERNAL ശൂന്യം
layout_populate_gids_to_retain (hb_subset_plan_t* plan,
                                hb_set_t* drop_tables);

HB_INTERNAL ശൂന്യം
collect_layout_variation_indices (hb_subset_plan_t* plan);
#endif


#endif /* HB_SUBSET_PLAN_HH */

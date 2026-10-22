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

#अगर_अघोषित HB_SUBSET_PLAN_HH
#घोषणा HB_SUBSET_PLAN_HH

#समावेश "hb.hh"

#समावेश "hb-subset.h"
#समावेश "hb-subset-input.hh"
#समावेश "hb-subset-accelerator.hh"

#समावेश "hb-map.hh"
#समावेश "hb-bimap.hh"
#समावेश "hb-set.hh"

namespace OT {
काष्ठा Feature;
}

काष्ठा os2_info_t {
  hb_codepoint_t min_cmap_codepoint;
  hb_codepoint_t max_cmap_codepoint;
};

प्रकार काष्ठा os2_info_t os2_info_t;

काष्ठा शीर्ष_maxp_info_t
{
  शीर्ष_maxp_info_t ()
      :xMin (0x7FFF), xMax (-0x7FFF), yMin (0x7FFF), yMax (-0x7FFF),
      maxPoints (0), maxContours (0),
      maxCompositePoints (0),
      maxCompositeContours (0),
      maxComponentElements (0),
      maxComponentDepth (0),
      allXMinIsLsb (true) {}

  पूर्णांक xMin;
  पूर्णांक xMax;
  पूर्णांक yMin;
  पूर्णांक yMax;
  अचिन्हित maxPoints;
  अचिन्हित maxContours;
  अचिन्हित maxCompositePoints;
  अचिन्हित maxCompositeContours;
  अचिन्हित maxComponentElements;
  अचिन्हित maxComponentDepth;
  bool allXMinIsLsb;
};

प्रकार काष्ठा शीर्ष_maxp_info_t शीर्ष_maxp_info_t;

काष्ठा contour_point_t
{
  व्योम init (भग्न x_ = 0.f, भग्न y_ = 0.f, bool is_end_point_ = false)
  { flag = 0; x = x_; y = y_; is_end_point = is_end_point_; }

  व्योम transform (स्थिर भग्न (&matrix)[4])
  {
    भग्न x_ = x * matrix[0] + y * matrix[2];
	  y  = x * matrix[1] + y * matrix[3];
    x  = x_;
  }

  व्योम add_delta (भग्न delta_x, भग्न delta_y)
  {
    x += delta_x;
    y += delta_y;
  }

  HB_ALWAYS_INLINE
  व्योम translate (स्थिर contour_point_t &p) { x += p.x; y += p.y; }

  भग्न x;
  भग्न y;
  uint8_t flag;
  bool is_end_point;
};

काष्ठा contour_point_vector_t : hb_vector_t<contour_point_t>
{
  bool add_deltas (hb_array_t<स्थिर भग्न> deltas_x,
                   hb_array_t<स्थिर भग्न> deltas_y,
                   hb_array_t<स्थिर bool> indices)
  {
    अगर (indices.लंबाई != deltas_x.लंबाई ||
        indices.लंबाई != deltas_y.लंबाई)
      वापस false;

    क्रम (अचिन्हित i = 0; i < indices.लंबाई; i++)
    {
      अगर (!indices.arrayZ[i]) जारी;
      arrayZ[i].add_delta (deltas_x.arrayZ[i], deltas_y.arrayZ[i]);
    }
    वापस true;
  }
};

namespace OT {
  काष्ठा cff1_subset_accelerator_t;
  काष्ठा cff2_subset_accelerator_t;
}

काष्ठा hb_subset_plan_t
{
  HB_INTERNAL hb_subset_plan_t (hb_face_t *,
				स्थिर hb_subset_निवेश_t *निवेश);

  HB_INTERNAL ~hb_subset_plan_t();

  hb_object_header_t header;

  bool successful;
  अचिन्हित flags;
  bool attach_accelerator_तथ्य = false;
  bool force_long_loca = false;

  // The glyph subset
  hb_map_t *codepoint_to_glyph; // Needs to be heap-allocated

  // Old -> New glyph id mapping
  hb_map_t *glyph_map; // Needs to be heap-allocated
  hb_map_t *reverse_glyph_map; // Needs to be heap-allocated

  // Plan is only good for a specific source/dest so keep them with it
  hb_face_t *source;
#अगर_अघोषित HB_NO_SUBSET_CFF
  // These have to be immediately after source:
  hb_face_lazy_loader_t<OT::cff1_subset_accelerator_t, 1> cff1_accel;
  hb_face_lazy_loader_t<OT::cff2_subset_accelerator_t, 2> cff2_accel;
#पूर्ण_अगर

  hb_face_t *dest;

  अचिन्हित पूर्णांक अंक_output_glyphs_2;

  bool all_axes_pinned;
  bool pinned_at_default;
  bool has_seac;

  // whether to insert a catch-all FeatureVariationRecord
  bool gsub_जोड़ो_catch_all_feature_variation_rec;
  bool gpos_जोड़ो_catch_all_feature_variation_rec;

  // whether GDEF ItemVariationStore is retained
  mutable bool has_gdef_varstore;

  // whether we're doing avar2 partial instancing
  bool has_avar2;

#घोषणा HB_SUBSET_PLAN_MEMBER(Type, Name) Type Name;
#समावेश "hb-subset-plan-member-list.hh"
#अघोषित HB_SUBSET_PLAN_MEMBER

  //recalculated head/maxp table info after instancing
  mutable शीर्ष_maxp_info_t शीर्ष_maxp_info;

  os2_info_t os2_info;

  स्थिर hb_subset_accelerator_t* accelerator = nullptr;
  hb_subset_accelerator_t* inprogress_accelerator = nullptr;

 खुला:

  ढाँचा<typename T>
  काष्ठा source_table_loader
  {
    hb_blob_ptr_t<T> चालक () (hb_subset_plan_t *plan)
    {
      hb_lock_t lock (plan->accelerator ? &plan->accelerator->sanitized_table_cache_lock : nullptr);

      स्वतः *cache = plan->accelerator ? &plan->accelerator->sanitized_table_cache : &plan->sanitized_table_cache;
      अगर (cache
	  && !cache->in_error ()
	  && cache->has (+T::tableTag)) {
	वापस hb_blob_reference (cache->लो (+T::tableTag).लो ());
      }

      hb::unique_ptr<hb_blob_t> table_blob {hb_sanitize_context_t ().reference_table<T> (plan->source)};
      hb_blob_t* ret = hb_blob_reference (table_blob.लो ());

      अगर (likely (cache))
	cache->set (+T::tableTag, std::move (table_blob));

      वापस ret;
    }
  };

  ढाँचा<typename T>
  स्वतः source_table() HB_AUTO_RETURN (source_table_loader<T> {} (यह))

  bool in_error () स्थिर { वापस !successful; }

  HB_INTERNAL द्विगुन layout_variation_delta (अचिन्हित var_idx);

  bool check_success(bool success)
  {
    successful = (successful && success);
    वापस successful;
  }

  /*
   * The set of input glyph ids which will be retained in the subset.
   * Does NOT include ids kept due to retain_gids. You probably want to use
   * glyph_map/reverse_glyph_map.
   */
  अंतरभूत स्थिर hb_set_t *
  glyphset () स्थिर
  {
    वापस &_glyphset;
  }

  /*
   * The set of input glyph ids which will be retained in the subset.
   */
  अंतरभूत स्थिर hb_set_t *
  glyphset_gsub () स्थिर
  {
    वापस &_glyphset_gsub;
  }

  /*
   * Old-to-new glyph id mapping for GSUB glyphs, through the flat
   * array; returns HB_MAP_VALUE_INVALID for glyphs not retained.
   */
  अंतरभूत hb_codepoint_t
  map_gsub_glyph (hb_codepoint_t old_gid) स्थिर
  {
    वापस old_gid < glyph_map_gsub_flat.लंबाई
	 ? glyph_map_gsub_flat.arrayZ[old_gid]
	 : HB_MAP_मूल्य_INVALID;
  }

  /*
   * The total number of output glyphs in the final subset.
   */
  अंतरभूत अचिन्हित पूर्णांक
  अंक_output_glyphs () स्थिर
  {
    वापस अंक_output_glyphs_2;
  }

  अंतरभूत bool new_gid_for_codepoint (hb_codepoint_t codepoint,
				     hb_codepoint_t *new_gid) स्थिर
  {
    hb_codepoint_t old_gid = codepoint_to_glyph->लो (codepoint);
    अगर (old_gid == HB_MAP_मूल्य_INVALID)
      वापस false;

    वापस new_gid_for_old_gid (old_gid, new_gid);
  }

  अंतरभूत bool new_gid_for_old_gid (hb_codepoint_t old_gid,
				   hb_codepoint_t *new_gid) स्थिर
  {
    hb_codepoint_t gid = glyph_map->लो (old_gid);
    अगर (gid == HB_MAP_मूल्य_INVALID)
      वापस false;

    *new_gid = gid;
    वापस true;
  }

  अंतरभूत bool old_gid_for_new_gid (hb_codepoint_t  new_gid,
				   hb_codepoint_t *old_gid) स्थिर
  {
    hb_codepoint_t gid = reverse_glyph_map->लो (new_gid);
    अगर (gid == HB_MAP_मूल्य_INVALID)
      वापस false;

    *old_gid = gid;
    वापस true;
  }

  अंतरभूत bool
  add_table (hb_tag_t tag,
	     hb_blob_t *contents)
  {
    अगर (HB_DEBUG_SUBSET)
    {
      hb_blob_t *source_blob = source->reference_table (tag);
      DEBUG_संदेश(SUBSET, nullptr, "add table %c%c%c%c, dest %u bytes, source %u bytes",
		HB_UNTAG(tag),
		hb_blob_get_लंबाई (contents),
		hb_blob_get_लंबाई (source_blob));
      hb_blob_destroy (source_blob);
    }
    वापस hb_face_builder_add_table (dest, tag, contents);
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
HB_INTERNAL व्योम
remap_indexes (स्थिर hb_set_t *indexes,
               hb_map_t       *mapping /* OUT */);


#अगर_अघोषित HB_NO_VAR
ढाँचा<typename ItemVarStore>
HB_INTERNAL व्योम
remap_variation_indices (स्थिर ItemVarStore &var_store,
                         स्थिर hb_set_t &variation_indices,
                         स्थिर hb_vector_t<पूर्णांक>& normalized_coords,
                         bool calculate_delta, /* not pinned at default */
                         bool no_variations, /* all axes pinned */
                         hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> &variation_idx_delta_map /* OUT */);


ढाँचा<typename Delta_Set_सूचकांक_Map>
HB_INTERNAL व्योम
remap_colrv1_delta_set_सूचकांक_indices (स्थिर Delta_Set_सूचकांक_Map &सूचकांक_map,
                                      स्थिर hb_set_t &delta_set_idxes,
                                      hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /* OUT */);


HB_INTERNAL व्योम
generate_varstore_inner_maps (स्थिर hb_set_t& varidx_set,
                              अचिन्हित subtable_गिनती,
                              hb_vector_t<hb_inc_bimap_t> &inner_maps /* OUT */);

HB_INTERNAL bool
normalize_axes_location (hb_face_t *face, hb_subset_plan_t *plan);

HB_INTERNAL व्योम
update_instance_metrics_map_from_cff2 (hb_subset_plan_t *plan);

HB_INTERNAL bool
get_instance_glyphs_contour_points (hb_subset_plan_t *plan);

#अगर_अघोषित HB_NO_BASE
HB_INTERNAL व्योम
collect_base_variation_indices (hb_subset_plan_t* plan);
#पूर्ण_अगर
#पूर्ण_अगर

#अगर_अघोषित HB_NO_SUBSET_LAYOUT
प्रकार hb_hashmap_t<अचिन्हित, hb::unique_ptr<hb_set_t>> script_langsys_map;

HB_INTERNAL व्योम
remap_used_mark_sets (hb_subset_plan_t *plan,
                      hb_map_t& used_mark_sets_map);

HB_INTERNAL व्योम
layout_nameid_closure (hb_subset_plan_t* plan,
                       hb_set_t* drop_tables);

HB_INTERNAL व्योम
layout_populate_gids_to_retain (hb_subset_plan_t* plan,
                                hb_set_t* drop_tables);

HB_INTERNAL व्योम
collect_layout_variation_indices (hb_subset_plan_t* plan);
#पूर्ण_अगर


#पूर्ण_अगर /* HB_SUBSET_PLAN_HH */

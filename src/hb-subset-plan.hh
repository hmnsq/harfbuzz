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

#अन्तर्भाव "hb.hh"

#अन्तर्भाव "hb-subset.h"
#अन्तर्भाव "hb-subset-input.hh"
#अन्तर्भाव "hb-subset-accelerator.hh"

#अन्तर्भाव "hb-map.hh"
#अन्तर्भाव "hb-bimap.hh"
#अन्तर्भाव "hb-set.hh"

नामाकाश OT {
संरचना Feature;
}

संरचना os2_info_t {
  hb_codepoint_t min_cmap_codepoint;
  hb_codepoint_t max_cmap_codepoint;
};

प्रकारनिर्देश संरचना os2_info_t os2_info_t;

संरचना head_maxp_info_t
{
  head_maxp_info_t ()
      :xMin (0x7FFF), xMax (-0x7FFF), yMin (0x7FFF), yMax (-0x7FFF),
      maxPoints (0), maxContours (0),
      maxCompositePoints (0),
      maxCompositeContours (0),
      maxComponentElements (0),
      maxComponentDepth (0),
      allXMinIsLsb (सत्य) {}

  पूर्णाङ्क xMin;
  पूर्णाङ्क xMax;
  पूर्णाङ्क yMin;
  पूर्णाङ्क yMax;
  अचिन्हित maxPoints;
  अचिन्हित maxContours;
  अचिन्हित maxCompositePoints;
  अचिन्हित maxCompositeContours;
  अचिन्हित maxComponentElements;
  अचिन्हित maxComponentDepth;
  तार्किक allXMinIsLsb;
};

प्रकारनिर्देश संरचना head_maxp_info_t head_maxp_info_t;

संरचना contour_point_t
{
  शून्य init (भिन्न x_ = 0.f, भिन्न y_ = 0.f, तार्किक is_end_point_ = असत्य)
  { flag = 0; x = x_; y = y_; is_end_point = is_end_point_; }

  शून्य transform (अचर भिन्न (&matrix)[4])
  {
    भिन्न x_ = x * matrix[0] + y * matrix[2];
	  y  = x * matrix[1] + y * matrix[3];
    x  = x_;
  }

  शून्य add_delta (भिन्न delta_x, भिन्न delta_y)
  {
    x += delta_x;
    y += delta_y;
  }

  HB_ALWAYS_INLINE
  शून्य translate (अचर contour_point_t &p) { x += p.x; y += p.y; }

  भिन्न x;
  भिन्न y;
  uint8_t flag;
  तार्किक is_end_point;
};

संरचना contour_point_vector_t : hb_vector_t<contour_point_t>
{
  तार्किक add_deltas (hb_array_t<अचर भिन्न> deltas_x,
                   hb_array_t<अचर भिन्न> deltas_y,
                   hb_array_t<अचर तार्किक> indices)
  {
    यदि (indices.length != deltas_x.length ||
        indices.length != deltas_y.length)
      फर्काऊ असत्य;

    लागि (अचिन्हित i = 0; i < indices.length; i++)
    {
      यदि (!indices.arrayZ[i]) जारी;
      arrayZ[i].add_delta (deltas_x.arrayZ[i], deltas_y.arrayZ[i]);
    }
    फर्काऊ सत्य;
  }
};

नामाकाश OT {
  संरचना cff1_subset_accelerator_t;
  संरचना cff2_subset_accelerator_t;
}

संरचना hb_subset_plan_t
{
  HB_INTERNAL hb_subset_plan_t (hb_face_t *,
				अचर hb_subset_आगत_t *आगत);

  HB_INTERNAL ~hb_subset_plan_t();

  hb_object_header_t header;

  तार्किक successful;
  अचिन्हित flags;
  तार्किक attach_accelerator_data = असत्य;
  तार्किक force_long_loca = असत्य;

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

  अचिन्हित पूर्णाङ्क अङ्क_output_glyphs_2;

  तार्किक all_axes_pinned;
  तार्किक pinned_at_default;
  तार्किक has_seac;

  // whether to insert a catch-all FeatureVariationRecord
  तार्किक gsub_घुसाउ_catch_all_feature_variation_rec;
  तार्किक gpos_घुसाउ_catch_all_feature_variation_rec;

  // whether GDEF ItemVariationStore is retained
  mutable तार्किक has_gdef_varstore;

  // whether we're doing avar2 partial instancing
  तार्किक has_avar2;

#define HB_SUBSET_PLAN_MEMBER(Type, Name) Type Name;
#अन्तर्भाव "hb-subset-plan-member-list.hh"
#undef HB_SUBSET_PLAN_MEMBER

  //recalculated head/maxp table info after instancing
  mutable head_maxp_info_t head_maxp_info;

  os2_info_t os2_info;

  अचर hb_subset_accelerator_t* accelerator = nullptr;
  hb_subset_accelerator_t* inprogress_accelerator = nullptr;

 सार्वजनिक:

  ढाँचा<typename T>
  संरचना source_table_loader
  {
    hb_blob_ptr_t<T> चालक () (hb_subset_plan_t *plan)
    {
      hb_lock_t lock (plan->accelerator ? &plan->accelerator->sanitized_table_cache_lock : nullptr);

      स्वतः *cache = plan->accelerator ? &plan->accelerator->sanitized_table_cache : &plan->sanitized_table_cache;
      यदि (cache
	  && !cache->in_error ()
	  && cache->has (+T::tableTag)) {
	फर्काऊ hb_blob_reference (cache->get (+T::tableTag).get ());
      }

      hb::unique_ptr<hb_blob_t> table_blob {hb_sanitize_context_t ().reference_table<T> (plan->source)};
      hb_blob_t* ret = hb_blob_reference (table_blob.get ());

      यदि (likely (cache))
	cache->set (+T::tableTag, std::move (table_blob));

      फर्काऊ ret;
    }
  };

  ढाँचा<typename T>
  स्वतः source_table() HB_AUTO_RETURN (source_table_loader<T> {} (एतत्))

  तार्किक in_error () अचर { फर्काऊ !successful; }

  HB_INTERNAL द्विगुण layout_variation_delta (अचिन्हित var_idx);

  तार्किक check_success(तार्किक success)
  {
    successful = (successful && success);
    फर्काऊ successful;
  }

  /*
   * The set of input glyph ids which will be retained in the subset.
   * Does NOT include ids kept due to retain_gids. You probably want to use
   * glyph_map/reverse_glyph_map.
   */
  अन्तःस्थ अचर hb_set_t *
  glyphset () अचर
  {
    फर्काऊ &_glyphset;
  }

  /*
   * The set of input glyph ids which will be retained in the subset.
   */
  अन्तःस्थ अचर hb_set_t *
  glyphset_gsub () अचर
  {
    फर्काऊ &_glyphset_gsub;
  }

  /*
   * Old-to-new glyph id mapping for GSUB glyphs, through the flat
   * array; returns HB_MAP_VALUE_INVALID for glyphs not retained.
   */
  अन्तःस्थ hb_codepoint_t
  map_gsub_glyph (hb_codepoint_t old_gid) अचर
  {
    फर्काऊ old_gid < glyph_map_gsub_flat.length
	 ? glyph_map_gsub_flat.arrayZ[old_gid]
	 : HB_MAP_मान_INVALID;
  }

  /*
   * The total number of output glyphs in the final subset.
   */
  अन्तःस्थ अचिन्हित पूर्णाङ्क
  अङ्क_output_glyphs () अचर
  {
    फर्काऊ अङ्क_output_glyphs_2;
  }

  अन्तःस्थ तार्किक new_gid_for_codepoint (hb_codepoint_t codepoint,
				     hb_codepoint_t *new_gid) अचर
  {
    hb_codepoint_t old_gid = codepoint_to_glyph->get (codepoint);
    यदि (old_gid == HB_MAP_मान_INVALID)
      फर्काऊ असत्य;

    फर्काऊ new_gid_for_old_gid (old_gid, new_gid);
  }

  अन्तःस्थ तार्किक new_gid_for_old_gid (hb_codepoint_t old_gid,
				   hb_codepoint_t *new_gid) अचर
  {
    hb_codepoint_t gid = glyph_map->get (old_gid);
    यदि (gid == HB_MAP_मान_INVALID)
      फर्काऊ असत्य;

    *new_gid = gid;
    फर्काऊ सत्य;
  }

  अन्तःस्थ तार्किक old_gid_for_new_gid (hb_codepoint_t  new_gid,
				   hb_codepoint_t *old_gid) अचर
  {
    hb_codepoint_t gid = reverse_glyph_map->get (new_gid);
    यदि (gid == HB_MAP_मान_INVALID)
      फर्काऊ असत्य;

    *old_gid = gid;
    फर्काऊ सत्य;
  }

  अन्तःस्थ तार्किक
  add_table (hb_tag_t tag,
	     hb_blob_t *contents)
  {
    यदि (HB_DEBUG_SUBSET)
    {
      hb_blob_t *source_blob = source->reference_table (tag);
      DEBUG_सन्देश(SUBSET, nullptr, "add table %c%c%c%c, dest %u bytes, source %u bytes",
		HB_UNTAG(tag),
		hb_blob_get_length (contents),
		hb_blob_get_length (source_blob));
      hb_blob_destroy (source_blob);
    }
    फर्काऊ hb_face_builder_add_table (dest, tag, contents);
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
HB_INTERNAL शून्य
remap_indexes (अचर hb_set_t *indexes,
               hb_map_t       *mapping /* OUT */);


#ifndef HB_NO_VAR
ढाँचा<typename ItemVarStore>
HB_INTERNAL शून्य
remap_variation_indices (अचर ItemVarStore &var_store,
                         अचर hb_set_t &variation_indices,
                         अचर hb_vector_t<पूर्णाङ्क>& normalized_coords,
                         तार्किक calculate_delta, /* not pinned at default */
                         तार्किक no_variations, /* all axes pinned */
                         hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णाङ्क>> &variation_idx_delta_map /* OUT */);


ढाँचा<typename DeltaSetIndexMap>
HB_INTERNAL शून्य
remap_colrv1_delta_set_index_indices (अचर DeltaSetIndexMap &index_map,
                                      अचर hb_set_t &delta_set_idxes,
                                      hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णाङ्क>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /* OUT */);


HB_INTERNAL शून्य
generate_varstore_inner_maps (अचर hb_set_t& varidx_set,
                              अचिन्हित subtable_गन्ती,
                              hb_vector_t<hb_inc_bimap_t> &inner_maps /* OUT */);

HB_INTERNAL तार्किक
normalize_axes_location (hb_face_t *face, hb_subset_plan_t *plan);

HB_INTERNAL शून्य
update_instance_metrics_map_from_cff2 (hb_subset_plan_t *plan);

HB_INTERNAL तार्किक
get_instance_glyphs_contour_points (hb_subset_plan_t *plan);

#ifndef HB_NO_BASE
HB_INTERNAL शून्य
collect_base_variation_indices (hb_subset_plan_t* plan);
#endif
#endif

#ifndef HB_NO_SUBSET_LAYOUT
प्रकारनिर्देश hb_hashmap_t<अचिन्हित, hb::unique_ptr<hb_set_t>> script_langsys_map;

HB_INTERNAL शून्य
remap_used_mark_sets (hb_subset_plan_t *plan,
                      hb_map_t& used_mark_sets_map);

HB_INTERNAL शून्य
layout_nameid_closure (hb_subset_plan_t* plan,
                       hb_set_t* drop_tables);

HB_INTERNAL शून्य
layout_populate_gids_to_retain (hb_subset_plan_t* plan,
                                hb_set_t* drop_tables);

HB_INTERNAL शून्य
collect_layout_variation_indices (hb_subset_plan_t* plan);
#endif


#endif /* HB_SUBSET_PLAN_HH */

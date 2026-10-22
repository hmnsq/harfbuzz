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

#ضمن "hb.hh"

#ضمن "hb-subset.h"
#ضمن "hb-subset-input.hh"
#ضمن "hb-subset-accelerator.hh"

#ضمن "hb-map.hh"
#ضمن "hb-bimap.hh"
#ضمن "hb-set.hh"

نطاق OT {
بنية Feature;
}

بنية os2_info_t {
  hb_codepoint_t min_cmap_codepoint;
  hb_codepoint_t max_cmap_codepoint;
};

تعريفنوع بنية os2_info_t os2_info_t;

بنية head_maxp_info_t
{
  head_maxp_info_t ()
      :xMin (0x7FFF), xMax (-0x7FFF), yMin (0x7FFF), yMax (-0x7FFF),
      maxPoints (0), maxContours (0),
      maxCompositePoints (0),
      maxCompositeContours (0),
      maxComponentElements (0),
      maxComponentDepth (0),
      allXMinIsLsb (𐡒𐡔𐡈) {}

  صحيح xMin;
  صحيح xMax;
  صحيح yMin;
  صحيح yMax;
  unsigned maxPoints;
  unsigned maxContours;
  unsigned maxCompositePoints;
  unsigned maxCompositeContours;
  unsigned maxComponentElements;
  unsigned maxComponentDepth;
  منطقي allXMinIsLsb;
};

تعريفنوع بنية head_maxp_info_t head_maxp_info_t;

بنية contour_point_t
{
  فراغ init (عائم x_ = 0.f, عائم y_ = 0.f, منطقي is_end_point_ = خطأ)
  { flag = 0; x = x_; y = y_; is_end_point = is_end_point_; }

  فراغ transform (ثابت عائم (&matrix)[4])
  {
    عائم x_ = x * matrix[0] + y * matrix[2];
	  y  = x * matrix[1] + y * matrix[3];
    x  = x_;
  }

  فراغ add_delta (عائم delta_x, عائم delta_y)
  {
    x += delta_x;
    y += delta_y;
  }

  HB_ALWAYS_INLINE
  فراغ translate (ثابت contour_point_t &p) { x += p.x; y += p.y; }

  عائم x;
  عائم y;
  uint8_t flag;
  منطقي is_end_point;
};

بنية contour_point_vector_t : hb_vector_t<contour_point_t>
{
  منطقي add_deltas (hb_array_t<ثابت عائم> deltas_x,
                   hb_array_t<ثابت عائم> deltas_y,
                   hb_array_t<ثابت منطقي> indices)
  {
    𐡄𐡍 (indices.length != deltas_x.length ||
        indices.length != deltas_y.length)
      أرجع خطأ;

    لكل (unsigned i = 0; i < indices.length; i++)
    {
      𐡄𐡍 (!indices.arrayZ[i]) استمر;
      arrayZ[i].add_delta (deltas_x.arrayZ[i], deltas_y.arrayZ[i]);
    }
    أرجع 𐡒𐡔𐡈;
  }
};

نطاق OT {
  بنية cff1_subset_accelerator_t;
  بنية cff2_subset_accelerator_t;
}

بنية hb_subset_plan_t
{
  HB_INTERNAL hb_subset_plan_t (hb_face_t *,
				ثابت hb_subset_input_t *input);

  HB_INTERNAL ~hb_subset_plan_t();

  hb_object_header_t header;

  منطقي successful;
  unsigned flags;
  منطقي attach_accelerator_data = خطأ;
  منطقي force_long_loca = خطأ;

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

  unsigned صحيح _num_output_glyphs;

  منطقي all_axes_pinned;
  منطقي pinned_at_default;
  منطقي has_seac;

  // whether to insert a catch-all FeatureVariationRecord
  منطقي gsub_insert_catch_all_feature_variation_rec;
  منطقي gpos_insert_catch_all_feature_variation_rec;

  // whether GDEF ItemVariationStore is retained
  mutable منطقي has_gdef_varstore;

  // whether we're doing avar2 partial instancing
  منطقي has_avar2;

#define HB_SUBSET_PLAN_MEMBER(Type, Name) Type Name;
#ضمن "hb-subset-plan-member-list.hh"
#undef HB_SUBSET_PLAN_MEMBER

  //recalculated head/maxp table info after instancing
  mutable head_maxp_info_t head_maxp_info;

  os2_info_t os2_info;

  ثابت hb_subset_accelerator_t* accelerator = nullptr;
  hb_subset_accelerator_t* inprogress_accelerator = nullptr;

 عام:

  template<typename T>
  بنية source_table_loader
  {
    hb_blob_ptr_t<T> operator () (hb_subset_plan_t *plan)
    {
      hb_lock_t lock (plan->accelerator ? &plan->accelerator->sanitized_table_cache_lock : nullptr);

      auto *cache = plan->accelerator ? &plan->accelerator->sanitized_table_cache : &plan->sanitized_table_cache;
      𐡄𐡍 (cache
	  && !cache->in_error ()
	  && cache->has (+T::tableTag)) {
	أرجع hb_blob_reference (cache->get (+T::tableTag).get ());
      }

      hb::unique_ptr<hb_blob_t> table_blob {hb_sanitize_context_t ().reference_table<T> (plan->source)};
      hb_blob_t* ret = hb_blob_reference (table_blob.get ());

      𐡄𐡍 (likely (cache))
	cache->set (+T::tableTag, std::move (table_blob));

      أرجع ret;
    }
  };

  template<typename T>
  auto source_table() HB_AUTO_RETURN (source_table_loader<T> {} (هذا))

  منطقي in_error () ثابت { أرجع !successful; }

  HB_INTERNAL مزدوج layout_variation_delta (unsigned var_idx);

  منطقي check_success(منطقي success)
  {
    successful = (successful && success);
    أرجع successful;
  }

  /*
   * The set of input glyph ids which will be retained in the subset.
   * Does NOT include ids kept due to retain_gids. You probably want to use
   * glyph_map/reverse_glyph_map.
   */
  مضمن ثابت hb_set_t *
  glyphset () ثابت
  {
    أرجع &_glyphset;
  }

  /*
   * The set of input glyph ids which will be retained in the subset.
   */
  مضمن ثابت hb_set_t *
  glyphset_gsub () ثابت
  {
    أرجع &_glyphset_gsub;
  }

  /*
   * Old-to-new glyph id mapping for GSUB glyphs, through the flat
   * array; returns HB_MAP_VALUE_INVALID for glyphs not retained.
   */
  مضمن hb_codepoint_t
  map_gsub_glyph (hb_codepoint_t old_gid) ثابت
  {
    أرجع old_gid < glyph_map_gsub_flat.length
	 ? glyph_map_gsub_flat.arrayZ[old_gid]
	 : HB_MAP_VALUE_INVALID;
  }

  /*
   * The total number of output glyphs in the final subset.
   */
  مضمن unsigned صحيح
  num_output_glyphs () ثابت
  {
    أرجع _num_output_glyphs;
  }

  مضمن منطقي new_gid_for_codepoint (hb_codepoint_t codepoint,
				     hb_codepoint_t *new_gid) ثابت
  {
    hb_codepoint_t old_gid = codepoint_to_glyph->get (codepoint);
    𐡄𐡍 (old_gid == HB_MAP_VALUE_INVALID)
      أرجع خطأ;

    أرجع new_gid_for_old_gid (old_gid, new_gid);
  }

  مضمن منطقي new_gid_for_old_gid (hb_codepoint_t old_gid,
				   hb_codepoint_t *new_gid) ثابت
  {
    hb_codepoint_t gid = glyph_map->get (old_gid);
    𐡄𐡍 (gid == HB_MAP_VALUE_INVALID)
      أرجع خطأ;

    *new_gid = gid;
    أرجع 𐡒𐡔𐡈;
  }

  مضمن منطقي old_gid_for_new_gid (hb_codepoint_t  new_gid,
				   hb_codepoint_t *old_gid) ثابت
  {
    hb_codepoint_t gid = reverse_glyph_map->get (new_gid);
    𐡄𐡍 (gid == HB_MAP_VALUE_INVALID)
      أرجع خطأ;

    *old_gid = gid;
    أرجع 𐡒𐡔𐡈;
  }

  مضمن منطقي
  add_table (hb_tag_t tag,
	     hb_blob_t *contents)
  {
    𐡄𐡍 (HB_DEBUG_SUBSET)
    {
      hb_blob_t *source_blob = source->reference_table (tag);
      DEBUG_MSG(SUBSET, nullptr, "add table %c%c%c%c, dest %u bytes, source %u bytes",
		HB_UNTAG(tag),
		hb_blob_get_length (contents),
		hb_blob_get_length (source_blob));
      hb_blob_destroy (source_blob);
    }
    أرجع hb_face_builder_add_table (dest, tag, contents);
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
HB_INTERNAL فراغ
remap_indexes (ثابت hb_set_t *indexes,
               hb_map_t       *mapping /* OUT */);


#ifndef HB_NO_VAR
template<typename ItemVarStore>
HB_INTERNAL فراغ
remap_variation_indices (ثابت ItemVarStore &var_store,
                         ثابت hb_set_t &variation_indices,
                         ثابت hb_vector_t<صحيح>& normalized_coords,
                         منطقي calculate_delta, /* not pinned at default */
                         منطقي no_variations, /* all axes pinned */
                         hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> &variation_idx_delta_map /* OUT */);


template<typename DeltaSetIndexMap>
HB_INTERNAL فراغ
remap_colrv1_delta_set_index_indices (ثابت DeltaSetIndexMap &index_map,
                                      ثابت hb_set_t &delta_set_idxes,
                                      hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /* OUT */);


HB_INTERNAL فراغ
generate_varstore_inner_maps (ثابت hb_set_t& varidx_set,
                              unsigned subtable_count,
                              hb_vector_t<hb_inc_bimap_t> &inner_maps /* OUT */);

HB_INTERNAL منطقي
normalize_axes_location (hb_face_t *face, hb_subset_plan_t *plan);

HB_INTERNAL فراغ
update_instance_metrics_map_from_cff2 (hb_subset_plan_t *plan);

HB_INTERNAL منطقي
get_instance_glyphs_contour_points (hb_subset_plan_t *plan);

#ifndef HB_NO_BASE
HB_INTERNAL فراغ
collect_base_variation_indices (hb_subset_plan_t* plan);
#endif
#endif

#ifndef HB_NO_SUBSET_LAYOUT
تعريفنوع hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> script_langsys_map;

HB_INTERNAL فراغ
remap_used_mark_sets (hb_subset_plan_t *plan,
                      hb_map_t& used_mark_sets_map);

HB_INTERNAL فراغ
layout_nameid_closure (hb_subset_plan_t* plan,
                       hb_set_t* drop_tables);

HB_INTERNAL فراغ
layout_populate_gids_to_retain (hb_subset_plan_t* plan,
                                hb_set_t* drop_tables);

HB_INTERNAL فراغ
collect_layout_variation_indices (hb_subset_plan_t* plan);
#endif


#endif /* HB_SUBSET_PLAN_HH */

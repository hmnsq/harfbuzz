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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_SUBSET_PLAN_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SUBSET_PLAN_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset-input.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset-accelerator.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-map.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-bimap.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-set.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਬਣਤਰ Feature;
}

ਬਣਤਰ os2_info_t {
  hb_codepoint_t min_cmap_codepoint;
  hb_codepoint_t max_cmap_codepoint;
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ os2_info_t os2_info_t;

ਬਣਤਰ ਸਿਰ_maxp_info_t
{
  ਸਿਰ_maxp_info_t ()
      :xMin (0x7FFF), xMax (-0x7FFF), yMin (0x7FFF), yMax (-0x7FFF),
      maxPoints (0), maxContours (0),
      max_ਸੰਯੁਕਤ_Points (0),
      max_ਸੰਯੁਕਤ_Contours (0),
      maxComponentElements (0),
      max_Component_ਡੂੰਘਾਈ (0),
      allXMinIsLsb (ਸੱਚ) {}

  ਪੂਰਨ xMin;
  ਪੂਰਨ xMax;
  ਪੂਰਨ yMin;
  ਪੂਰਨ yMax;
  ਬਿਨਾਨਿਸ਼ਾਨੀ maxPoints;
  ਬਿਨਾਨਿਸ਼ਾਨੀ maxContours;
  ਬਿਨਾਨਿਸ਼ਾਨੀ max_ਸੰਯੁਕਤ_Points;
  ਬਿਨਾਨਿਸ਼ਾਨੀ max_ਸੰਯੁਕਤ_Contours;
  ਬਿਨਾਨਿਸ਼ਾਨੀ maxComponentElements;
  ਬਿਨਾਨਿਸ਼ਾਨੀ max_Component_ਡੂੰਘਾਈ;
  ਤਾਰਕਿਕ allXMinIsLsb;
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ਸਿਰ_maxp_info_t ਸਿਰ_maxp_info_t;

ਬਣਤਰ contour_ਬਿੰਦੂ_t
{
  ਖਾਲੀ init (ਦਸ਼ਮਲਵ x_ = 0.f, ਦਸ਼ਮਲਵ y_ = 0.f, ਤਾਰਕਿਕ is_ਸਿਰਾ_ਬਿੰਦੂ_2 = ਝੂਠ)
  { flag = 0; x = x_; y = y_; is_ਸਿਰਾ_ਬਿੰਦੂ = is_ਸਿਰਾ_ਬਿੰਦੂ_2; }

  ਖਾਲੀ transform (ਅਚਲ ਦਸ਼ਮਲਵ (&ਮੈਟ੍ਰਿਕਸ)[4])
  {
    ਦਸ਼ਮਲਵ x_ = x * ਮੈਟ੍ਰਿਕਸ[0] + y * ਮੈਟ੍ਰਿਕਸ[2];
	  y  = x * ਮੈਟ੍ਰਿਕਸ[1] + y * ਮੈਟ੍ਰਿਕਸ[3];
    x  = x_;
  }

  ਖਾਲੀ ਜੋੜੋ_delta (ਦਸ਼ਮਲਵ delta_x, ਦਸ਼ਮਲਵ delta_y)
  {
    x += delta_x;
    y += delta_y;
  }

  HB_ALWAYS_INLINE
  ਖਾਲੀ translate (ਅਚਲ contour_ਬਿੰਦੂ_t &p) { x += p.x; y += p.y; }

  ਦਸ਼ਮਲਵ x;
  ਦਸ਼ਮਲਵ y;
  uint8_t flag;
  ਤਾਰਕਿਕ is_ਸਿਰਾ_ਬਿੰਦੂ;
};

ਬਣਤਰ contour_ਬਿੰਦੂ_ਸਦਿਸ਼_t : hb_ਸਦਿਸ਼_t<contour_ਬਿੰਦੂ_t>
{
  ਤਾਰਕਿਕ ਜੋੜੋ_deltas (hb_array_t<ਅਚਲ ਦਸ਼ਮਲਵ> deltas_x,
                   hb_array_t<ਅਚਲ ਦਸ਼ਮਲਵ> deltas_y,
                   hb_array_t<ਅਚਲ ਤਾਰਕਿਕ> indices)
  {
    ਜੇ (indices.ਲੰਮਾਈ != deltas_x.ਲੰਮਾਈ ||
        indices.ਲੰਮਾਈ != deltas_y.ਲੰਮਾਈ)
      ਵਾਪਸ ਝੂਠ;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < indices.ਲੰਮਾਈ; i++)
    {
      ਜੇ (!indices.arrayZ[i]) ਜਾਰੀ;
      arrayZ[i].ਜੋੜੋ_delta (deltas_x.arrayZ[i], deltas_y.arrayZ[i]);
    }
    ਵਾਪਸ ਸੱਚ;
  }
};

ਨਾਮਾਕਾਸ਼ OT {
  ਬਣਤਰ cff1_subset_accelerator_t;
  ਬਣਤਰ cff2_subset_accelerator_t;
}

ਬਣਤਰ hb_subset_plan_t
{
  HB_ਅੰਦਰੂਨੀ hb_subset_plan_t (hb_face_t *,
				ਅਚਲ hb_subset_ਨਿਵੇਸ਼_t *ਨਿਵੇਸ਼);

  HB_ਅੰਦਰੂਨੀ ~hb_subset_plan_t();

  hb_object_ਸਿਰਲੇਖ_t ਸਿਰਲੇਖ;

  ਤਾਰਕਿਕ successful;
  ਬਿਨਾਨਿਸ਼ਾਨੀ flags;
  ਤਾਰਕਿਕ attach_accelerator_ਅੰਕੜੇ = ਝੂਠ;
  ਤਾਰਕਿਕ force_long_loca = ਝੂਠ;

  // The glyph subset
  hb_map_t *codepoint_ਮੰਜ਼ਿਲ_glyph; // Needs to be heap-allocated

  // Old -> New glyph id mapping
  hb_map_t *glyph_map; // Needs to be heap-allocated
  hb_map_t *ਉਲਟ_glyph_map; // Needs to be heap-allocated

  // Plan is only good for a specific source/dest so keep them with it
  hb_face_t *ਸਰੋਤ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_SUBSET_CFF
  // These have to be immediately after source:
  hb_face_lazy_loader_t<OT::cff1_subset_accelerator_t, 1> cff1_accel;
  hb_face_lazy_loader_t<OT::cff2_subset_accelerator_t, 2> cff2_accel;
#ਜੇ_ਅੰਤ

  hb_face_t *dest;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_output_glyphs_2;

  ਤਾਰਕਿਕ all_axes_pinned;
  ਤਾਰਕਿਕ pinned_at_default;
  ਤਾਰਕਿਕ has_seac;

  // whether to insert a catch-all FeatureVariationRecord
  ਤਾਰਕਿਕ gsub_ਪਾਓ_catch_all_feature_variation_rec;
  ਤਾਰਕਿਕ gpos_ਪਾਓ_catch_all_feature_variation_rec;

  // whether GDEF ItemVariationStore is retained
  ਬਦਲਣਯੋਗ ਤਾਰਕਿਕ has_gdef_varstore;

  // whether we're doing avar2 partial instancing
  ਤਾਰਕਿਕ has_avar2;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SUBSET_PLAN_MEMBER(Type, Name) Type Name;
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset-plan-member-list.hh"
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_SUBSET_PLAN_MEMBER

  //recalculated head/maxp table info after instancing
  ਬਦਲਣਯੋਗ ਸਿਰ_maxp_info_t ਸਿਰ_maxp_info;

  os2_info_t os2_info;

  ਅਚਲ hb_subset_accelerator_t* accelerator = ਖਾਲੀ_ਸੂਚਕ;
  hb_subset_accelerator_t* inprogress_accelerator = ਖਾਲੀ_ਸੂਚਕ;

 ਜਨਤਕ:

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T>
  ਬਣਤਰ ਸਰੋਤ_ਸਾਰਣੀ_loader
  {
    hb_blob_ptr_t<T> ਸੰਚਾਲਕ () (hb_subset_plan_t *plan)
    {
      hb_ਤਾਲਾ_t lock (plan->accelerator ? &plan->accelerator->sanitized_ਸਾਰਣੀ_cache_ਤਾਲਾ : ਖਾਲੀ_ਸੂਚਕ);

      ਸਵੈਚਾਲਿਤ *cache = plan->accelerator ? &plan->accelerator->sanitized_ਸਾਰਣੀ_cache : &plan->sanitized_ਸਾਰਣੀ_cache;
      ਜੇ (cache
	  && !cache->ਵਿੱਚ_error ()
	  && cache->has (+T::ਸਾਰਣੀ_ਟੈਗ)) {
	ਵਾਪਸ hb_blob_reference (cache->get (+T::ਸਾਰਣੀ_ਟੈਗ).get ());
      }

      hb::unique_ptr<hb_blob_t> ਸਾਰਣੀ_blob {hb_sanitize_context_t ().reference_ਸਾਰਣੀ<T> (plan->ਸਰੋਤ)};
      hb_blob_t* ret = hb_blob_reference (ਸਾਰਣੀ_blob.get ());

      ਜੇ (likely (cache))
	cache->ਸੈੱਟ (+T::ਸਾਰਣੀ_ਟੈਗ, std::move (ਸਾਰਣੀ_blob));

      ਵਾਪਸ ret;
    }
  };

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T>
  ਸਵੈਚਾਲਿਤ ਸਰੋਤ_ਸਾਰਣੀ() HB_AUTO_RETURN (ਸਰੋਤ_ਸਾਰਣੀ_loader<T> {} (ਇਹ))

  ਤਾਰਕਿਕ ਵਿੱਚ_error () ਅਚਲ { ਵਾਪਸ !successful; }

  HB_ਅੰਦਰੂਨੀ ਦੁੱਗਣਾ layout_variation_delta (ਬਿਨਾਨਿਸ਼ਾਨੀ var_idx);

  ਤਾਰਕਿਕ check_success(ਤਾਰਕਿਕ success)
  {
    successful = (successful && success);
    ਵਾਪਸ successful;
  }

  /*
   * The set of input glyph ids which will be retained in the subset.
   * Does NOT include ids kept due to retain_gids. You probably want to use
   * glyph_map/reverse_glyph_map.
   */
  ਸਤਰਅੰਦਰ ਅਚਲ hb_ਸੈੱਟ_t *
  glyphset () ਅਚਲ
  {
    ਵਾਪਸ &_glyphset;
  }

  /*
   * The set of input glyph ids which will be retained in the subset.
   */
  ਸਤਰਅੰਦਰ ਅਚਲ hb_ਸੈੱਟ_t *
  glyphset_gsub () ਅਚਲ
  {
    ਵਾਪਸ &_glyphset_gsub;
  }

  /*
   * Old-to-new glyph id mapping for GSUB glyphs, through the flat
   * array; returns HB_MAP_VALUE_INVALID for glyphs not retained.
   */
  ਸਤਰਅੰਦਰ hb_codepoint_t
  map_gsub_glyph (hb_codepoint_t old_gid) ਅਚਲ
  {
    ਵਾਪਸ old_gid < glyph_map_gsub_flat.ਲੰਮਾਈ
	 ? glyph_map_gsub_flat.arrayZ[old_gid]
	 : HB_MAP_ਮੁੱਲ_INVALID;
  }

  /*
   * The total number of output glyphs in the final subset.
   */
  ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
  ਅੰਕ_output_glyphs () ਅਚਲ
  {
    ਵਾਪਸ ਅੰਕ_output_glyphs_2;
  }

  ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ new_gid_for_codepoint (hb_codepoint_t codepoint,
				     hb_codepoint_t *new_gid) ਅਚਲ
  {
    hb_codepoint_t old_gid = codepoint_ਮੰਜ਼ਿਲ_glyph->get (codepoint);
    ਜੇ (old_gid == HB_MAP_ਮੁੱਲ_INVALID)
      ਵਾਪਸ ਝੂਠ;

    ਵਾਪਸ new_gid_for_old_gid (old_gid, new_gid);
  }

  ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ new_gid_for_old_gid (hb_codepoint_t old_gid,
				   hb_codepoint_t *new_gid) ਅਚਲ
  {
    hb_codepoint_t gid = glyph_map->get (old_gid);
    ਜੇ (gid == HB_MAP_ਮੁੱਲ_INVALID)
      ਵਾਪਸ ਝੂਠ;

    *new_gid = gid;
    ਵਾਪਸ ਸੱਚ;
  }

  ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ old_gid_for_new_gid (hb_codepoint_t  new_gid,
				   hb_codepoint_t *old_gid) ਅਚਲ
  {
    hb_codepoint_t gid = ਉਲਟ_glyph_map->get (new_gid);
    ਜੇ (gid == HB_MAP_ਮੁੱਲ_INVALID)
      ਵਾਪਸ ਝੂਠ;

    *old_gid = gid;
    ਵਾਪਸ ਸੱਚ;
  }

  ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
  ਜੋੜੋ_ਸਾਰਣੀ (hb_ਟੈਗ_t ਟੈਗ_2,
	     hb_blob_t *contents)
  {
    ਜੇ (HB_DEBUG_SUBSET)
    {
      hb_blob_t *ਸਰੋਤ_blob = ਸਰੋਤ->reference_ਸਾਰਣੀ (ਟੈਗ_2);
      DEBUG_ਸੁਨੇਹਾ(SUBSET, ਖਾਲੀ_ਸੂਚਕ, "add table %c%c%c%c, dest %u bytes, source %u bytes",
		HB_UNTAG(ਟੈਗ_2),
		hb_blob_get_ਲੰਮਾਈ (contents),
		hb_blob_get_ਲੰਮਾਈ (ਸਰੋਤ_blob));
      hb_blob_destroy (ਸਰੋਤ_blob);
    }
    ਵਾਪਸ hb_face_builder_ਜੋੜੋ_ਸਾਰਣੀ (dest, ਟੈਗ_2, contents);
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
HB_ਅੰਦਰੂਨੀ ਖਾਲੀ
remap_indexes (ਅਚਲ hb_ਸੈੱਟ_t *indexes,
               hb_map_t       *mapping /*ਬਾਹਰਲਾ*/);


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ ਵਸਤੂ_Var_Store>
HB_ਅੰਦਰੂਨੀ ਖਾਲੀ
remap_variation_indices (ਅਚਲ ਵਸਤੂ_Var_Store &var_store,
                         ਅਚਲ hb_ਸੈੱਟ_t &variation_indices,
                         ਅਚਲ hb_ਸਦਿਸ਼_t<ਪੂਰਨ>& normalized_coords,
                         ਤਾਰਕਿਕ calculate_delta, /* not pinned at default */
                         ਤਾਰਕਿਕ no_variations, /* all axes pinned */
                         hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> &variation_idx_delta_map /*ਬਾਹਰਲਾ*/);


ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map>
HB_ਅੰਦਰੂਨੀ ਖਾਲੀ
remap_colrv1_delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_indices (ਅਚਲ Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map &ਸੂਚਕਾਂਕ_map,
                                      ਅਚਲ hb_ਸੈੱਟ_t &delta_ਸੈੱਟ_idxes,
                                      hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> &variation_idx_delta_map, /* IN/OUT */
                                      hb_map_t &new_deltaset_idx_varidx_map /*ਬਾਹਰਲਾ*/);


HB_ਅੰਦਰੂਨੀ ਖਾਲੀ
generate_varstore_inner_maps (ਅਚਲ hb_ਸੈੱਟ_t& varidx_ਸੈੱਟ,
                              ਬਿਨਾਨਿਸ਼ਾਨੀ subtable_ਗਿਣਤੀ,
                              hb_ਸਦਿਸ਼_t<hb_inc_bimap_t> &inner_maps /*ਬਾਹਰਲਾ*/);

HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ
normalize_axes_location (hb_face_t *face, hb_subset_plan_t *plan);

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ
update_instance_metrics_map_from_cff2 (hb_subset_plan_t *plan);

HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ
get_instance_glyphs_contour_points (hb_subset_plan_t *plan);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_ਅਧਾਰ
HB_ਅੰਦਰੂਨੀ ਖਾਲੀ
collect_ਅਧਾਰ_variation_indices (hb_subset_plan_t* plan);
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_SUBSET_LAYOUT
ਕਿਸਮਪਰਿਭਾਸ਼ਾ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb::unique_ptr<hb_ਸੈੱਟ_t>> script_langsys_map;

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ
remap_ਵਰਤਿਆ_mark_sets (hb_subset_plan_t *plan,
                      hb_map_t& ਵਰਤਿਆ_mark_sets_map);

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ
layout_nameid_ਘੇਰਾ (hb_subset_plan_t* plan,
                       hb_ਸੈੱਟ_t* drop_tables);

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ
layout_populate_gids_ਮੰਜ਼ਿਲ_retain (hb_subset_plan_t* plan,
                                hb_ਸੈੱਟ_t* drop_tables);

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ
collect_layout_variation_indices (hb_subset_plan_t* plan);
#ਜੇ_ਅੰਤ


#ਜੇ_ਅੰਤ /* HB_SUBSET_PLAN_HH */

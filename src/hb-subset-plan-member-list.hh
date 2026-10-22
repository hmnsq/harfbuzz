/*
 * Copyright © 2018  Google, Inc.
 * Copyright © 2023  Behdad Esfahbod
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

#جے_بنا_تعریف HB_SUBSET_PLAN_MEMBER_LIST_HH
#تعریف_کرو HB_SUBSET_PLAN_MEMBER_LIST_HH
#جے_انت /* HB_SUBSET_PLAN_MEMBER_LIST_HH */ /* Dummy header guards */

#تعریف_کرو E(x, y) x, y

// For each cp that we'd like to retain maps to the corresponding gid.
HB_SUBSET_PLAN_MEMBER (hb_سیٹ_t, unicodes)
HB_SUBSET_PLAN_MEMBER (hb_sorted_سمتیہ_t<hb_codepoint_pair_t>, unicode_منزل_new_gid_list)

HB_SUBSET_PLAN_MEMBER (hb_sorted_سمتیہ_t<hb_codepoint_pair_t>, new_منزل_old_gid_list)

// name_ids we would like to retain
HB_SUBSET_PLAN_MEMBER (hb_سیٹ_t, name_ids)

// name_languages we would like to retain
HB_SUBSET_PLAN_MEMBER (hb_سیٹ_t, name_languages)

//layout features which will be preserved
HB_SUBSET_PLAN_MEMBER (hb_سیٹ_t, layout_features)

// layout scripts which will be preserved.
HB_SUBSET_PLAN_MEMBER (hb_سیٹ_t, layout_scripts)

//glyph ids requested to retain
HB_SUBSET_PLAN_MEMBER (hb_سیٹ_t, glyphs_requested)

// Tables which should not be processed, just pass them through.
HB_SUBSET_PLAN_MEMBER (hb_سیٹ_t, no_subset_tables)

// Tables which should be dropped.
HB_SUBSET_PLAN_MEMBER (hb_سیٹ_t, drop_tables)

// Old -> New glyph id mapping
HB_SUBSET_PLAN_MEMBER (hb_map_t, glyph_map_gsub)

// Same mapping as glyph_map_gsub, as a flat array indexed by old gid,
// with HB_MAP_VALUE_INVALID entries for glyphs not retained.  Sized to
// the highest retained GSUB gid plus one.
HB_SUBSET_PLAN_MEMBER (hb_سمتیہ_t<hb_codepoint_t>, glyph_map_gsub_flat)

HB_SUBSET_PLAN_MEMBER (hb_سیٹ_t, _glyphset)
HB_SUBSET_PLAN_MEMBER (hb_سیٹ_t, _glyphset_gsub)
HB_SUBSET_PLAN_MEMBER (hb_سیٹ_t, _glyphset_mathed)
HB_SUBSET_PLAN_MEMBER (hb_سیٹ_t, _glyphset_colred)
HB_SUBSET_PLAN_MEMBER (hb_سیٹ_t, _glyphset_varced)
HB_SUBSET_PLAN_MEMBER (hb_سیٹ_t, _glyphset_cmaped)

//active lookups we'd like to retain
HB_SUBSET_PLAN_MEMBER (hb_map_t, gsub_lookups)
HB_SUBSET_PLAN_MEMBER (hb_map_t, gpos_lookups)

//use_mark_sets mapping: old->new
HB_SUBSET_PLAN_MEMBER (hb_map_t, ورتیا_mark_sets_map)

//active langsys we'd like to retain
HB_SUBSET_PLAN_MEMBER (hb_hashmap_t E(<بنانشانی, hb::unique_ptr<hb_سیٹ_t>>), gsub_langsys)
HB_SUBSET_PLAN_MEMBER (hb_hashmap_t E(<بنانشانی, hb::unique_ptr<hb_سیٹ_t>>), gpos_langsys)

//active features after removing redundant langsys and prune_features
HB_SUBSET_PLAN_MEMBER (hb_map_t, gsub_features)
HB_SUBSET_PLAN_MEMBER (hb_map_t, gpos_features)

//active features(with duplicates) old index -> new index mapping
HB_SUBSET_PLAN_MEMBER (hb_map_t, gsub_features_w_duplicates)
HB_SUBSET_PLAN_MEMBER (hb_map_t, gpos_features_w_duplicates)

//active feature variation records/condition index with variations
HB_SUBSET_PLAN_MEMBER (hb_hashmap_t E(<بنانشانی, hb::shared_ptr<hb_سیٹ_t>>), gsub_feature_ریکارڈ_cond_idx_map)
HB_SUBSET_PLAN_MEMBER (hb_hashmap_t E(<بنانشانی, hb::shared_ptr<hb_سیٹ_t>>), gpos_feature_ریکارڈ_cond_idx_map)

//feature index-> address of substituation feature table mapping with
//variations
HB_SUBSET_PLAN_MEMBER (hb_hashmap_t E(<بنانشانی, ثابت OT::Feature*>), gsub_feature_substitutes_map)
HB_SUBSET_PLAN_MEMBER (hb_hashmap_t E(<بنانشانی, ثابت OT::Feature*>), gpos_feature_substitutes_map)

//feature index->resolved lookup set mapping for fully-instanced lookup variations
HB_SUBSET_PLAN_MEMBER (hb_hashmap_t E(<بنانشانی, hb::shared_ptr<hb_سیٹ_t>>), gsub_lookup_variations)
HB_SUBSET_PLAN_MEMBER (hb_hashmap_t E(<بنانشانی, hb::shared_ptr<hb_سیٹ_t>>), gpos_lookup_variations)

// old feature_indexes set, used to reinstate the old features
HB_SUBSET_PLAN_MEMBER (hb_سیٹ_t, gsub_old_features)
HB_SUBSET_PLAN_MEMBER (hb_سیٹ_t, gpos_old_features)

//feature_index->pair of (address of old feature, feature tag), used for alternate feature
//parameter formats and inserting a catch all record if necessary
HB_SUBSET_PLAN_MEMBER (hb_hashmap_t E(<بنانشانی, hb_pair_t E(<ثابت فراغ*, ثابت فراغ*>)>), gsub_old_feature_idx_ٹیگ_map)
HB_SUBSET_PLAN_MEMBER (hb_hashmap_t E(<بنانشانی, hb_pair_t E(<ثابت فراغ*, ثابت فراغ*>)>), gpos_old_feature_idx_ٹیگ_map)

//active layers/palettes we'd like to retain
HB_SUBSET_PLAN_MEMBER (hb_map_t, colrv1_layers)
HB_SUBSET_PLAN_MEMBER (hb_map_t, colr_palettes)
//colrv1 varstore retained varidx mapping
HB_SUBSET_PLAN_MEMBER (hb_سمتیہ_t<hb_inc_bimap_t>, colrv1_varstore_inner_maps)
//colrv1 retained varidx -> (new varidx, delta) mapping
HB_SUBSET_PLAN_MEMBER (بدلنجوگ hb_hashmap_t E(<بنانشانی, hb_pair_t E(<بنانشانی, صحيح>)>), colrv1_variation_idx_delta_map)
//colrv1 retained new delta set index -> new varidx mapping
HB_SUBSET_PLAN_MEMBER (hb_map_t, colrv1_new_deltaset_idx_varidx_map)

//Old layout item variation index -> (New varidx, delta) mapping
HB_SUBSET_PLAN_MEMBER (بدلنجوگ hb_hashmap_t E(<بنانشانی, hb_pair_t E(<بنانشانی, صحيح>)>), layout_variation_idx_delta_map)

//Layout condition -> (source variation index, default value)
HB_SUBSET_PLAN_MEMBER (hb_hashmap_t E(<uintptr_t, hb_pair_t E(<بنانشانی, صحيح>)>), layout_condition_اقدار)
//Layout condition -> (private variation index, rescaled default value)
HB_SUBSET_PLAN_MEMBER (بدلنجوگ hb_hashmap_t E(<uintptr_t, hb_pair_t E(<بنانشانی, صحيح>)>), layout_condition_idx_قدر_map)

//gdef varstore retained varidx mapping
HB_SUBSET_PLAN_MEMBER (hb_سمتیہ_t<hb_inc_bimap_t>, gdef_varstore_inner_maps)

HB_SUBSET_PLAN_MEMBER (hb_hashmap_t E(<hb_ٹیگ_t, hb::unique_ptr<hb_blob_t>>), sanitized_جدول_cache)

//normalized axes range map
HB_SUBSET_PLAN_MEMBER (hb_hashmap_t E(<hb_ٹیگ_t, Triple>), axes_location)
HB_SUBSET_PLAN_MEMBER (hb_سمتیہ_t<صحيح>, normalized_coords)

//user specified axes range map
HB_SUBSET_PLAN_MEMBER (hb_hashmap_t E(<hb_ٹیگ_t, Triple>), user_axes_location)
//axis->TripleDistances map (distances in the pre-normalized space)
HB_SUBSET_PLAN_MEMBER (hb_hashmap_t E(<hb_ٹیگ_t, TripleDistances>), axes_triple_distances)

//retained old axis index -> new axis index mapping in fvar axis array
HB_SUBSET_PLAN_MEMBER (hb_map_t, axes_اشاریہ_map)

//old intermediate coords (post-fvar, post-avar-v1, pre-avar-v2) at new min/default/max
HB_SUBSET_PLAN_MEMBER (hb_hashmap_t E(<hb_ٹیگ_t, Triple>), old_intermediates)

//avar2: reachable old-space final-coord range per axis, as Triple
//(minimum, unused middle, maximum), F2Dot14-quantized. Only axes with a
//constraining range (narrower than [-1, +1]) have entries. Used to cull
//variation tuples/regions that a partial instance can never reach.
HB_SUBSET_PLAN_MEMBER (hb_hashmap_t E(<hb_ٹیگ_t, Triple>), avar2_reachable_ranges)

//avar2: self-contained pinned axes -> constant old-space final coord.
//These axes are removed from fvar/avar entirely; their contribution is
//baked into the variation tables like an ordinary pin at that coord.
HB_SUBSET_PLAN_MEMBER (hb_hashmap_t E(<hb_ٹیگ_t, مزدوج>), avar2_self_contained)

//axis_index->axis_tag mapping in fvar axis array
HB_SUBSET_PLAN_MEMBER (hb_map_t, axes_old_اشاریہ_ٹیگ_map)
//vector of retained axis tags in the order of axes given in the 'fvar' table
HB_SUBSET_PLAN_MEMBER (hb_سمتیہ_t<hb_ٹیگ_t>, axis_ٹیگ_سوچی)

//hmtx metrics map: new gid->(advance, lsb)
HB_SUBSET_PLAN_MEMBER (بدلنجوگ hb_hashmap_t E(<hb_codepoint_t, hb_pair_t E(<بنانشانی, صحيح>)>), hmtx_map)
//vmtx metrics map: new gid->(advance, lsb)
HB_SUBSET_PLAN_MEMBER (بدلنجوگ hb_hashmap_t E(<hb_codepoint_t, hb_pair_t E(<بنانشانی, صحيح>)>), vmtx_map)
//boundsWidth map: new gid->boundsWidth, boundWidth=xMax - xMin
HB_SUBSET_PLAN_MEMBER (بدلنجوگ hb_سمتیہ_t<بنانشانی>, bounds_چوڑائی_vec)
//boundsHeight map: new gid->boundsHeight, boundsHeight=yMax - yMin
HB_SUBSET_PLAN_MEMBER (بدلنجوگ hb_سمتیہ_t<بنانشانی>, bounds_اچائی_vec)

//map: new_gid -> contour points vector
HB_SUBSET_PLAN_MEMBER (بدلنجوگ hb_hashmap_t E(<hb_codepoint_t, contour_نقطہ_سمتیہ_t>), new_gid_contour_points_map)

//new gids set for composite glyphs
HB_SUBSET_PLAN_MEMBER (hb_سیٹ_t, مرکب_new_gids)

//Old BASE item variation index -> (New varidx, 0) mapping
HB_SUBSET_PLAN_MEMBER (بدلنجوگ hb_hashmap_t E(<بنانشانی, hb_pair_t E(<بنانشانی, صحيح>)>), ادھار_variation_idx_map)

//BASE table varstore retained varidx mapping
HB_SUBSET_PLAN_MEMBER (hb_سمتیہ_t<hb_inc_bimap_t>, ادھار_varstore_inner_maps)

#جے_تعریف_شدہ HB_EXPERIMENTAL_API
// name table overrides map: hb_ot_name_record_ids_t-> name string new value or
// None to indicate should remove
HB_SUBSET_PLAN_MEMBER (hb_hashmap_t E(<hb_ot_name_ریکارڈ_ids_t, hb_بائٹاں_t>), name_جدول_overrides)
#جے_انت

#تعریف_رد E

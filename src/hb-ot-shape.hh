/*
 * Copyright © 2010  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#ifndef HB_OT_SHAPE_HH
#define HB_OT_SHAPE_HH

#অন্তর্ভাব "hb.hh"

#অন্তর্ভাব "hb-ot-map.hh"
#অন্তর্ভাব "hb-aat-map.hh"


HB_INTERNAL তার্কিক
_hb_ot_layout_table_get_feature_variations_state (hb_face_t             *face,
						   hb_tag_t               table_tag,
						   ধ্রুব পূর্ণাঙ্ক             *coords,
						   unsigned পূর্ণাঙ্ক           অঙ্ক_coords,
						   unsigned পূর্ণাঙ্ক          *variations_index,
						   hb_vector_t<unsigned> *lookup_conditions);

HB_INTERNAL তার্কিক
_hb_ot_layout_feature_get_lookup_variations (hb_face_t *face,
                                             hb_tag_t table_tag,
                                             unsigned feature_index,
                                             unsigned variations_index,
                                             hb_sorted_array_t<ধ্রুব unsigned> conditions,
                                             hb_set_t *lookup_indexes);

সংরচনা hb_ot_shape_plan_চাবি_t
{
  unsigned পূর্ণাঙ্ক variations_index[2];
  /* Sorted offsets of distinct true conditions within FeatureVariations.
   * Different outcomes may conservatively separate equivalent lookup sets. */
  hb_vector_t<unsigned> lookup_conditions[2];

  তার্কিক init (hb_face_t *face,
	     ধ্রুব পূর্ণাঙ্ক *coords,
	     unsigned   অঙ্ক_coords)
  {
    প্রতি (unsigned পূর্ণাঙ্ক table_index = 0; table_index < 2; table_index++)
      যদি (unlikely (!_hb_ot_layout_table_get_feature_variations_state (
			face,
			table_tags[table_index],
			coords,
			অঙ্ক_coords,
			&variations_index[table_index],
			&lookup_conditions[table_index])))
      {
	প্রতি (auto &v : lookup_conditions)
	  v.fini ();
	প্রত্যাগম অসত্য;
      }
    প্রত্যাগম সত্য;
  }

  শূন্য fini ()
  {
    প্রতি (auto &v : lookup_conditions)
      v.fini ();
  }

  তার্কিক get_lookup_variations (hb_face_t *face,
                              unsigned পূর্ণাঙ্ক table_index,
			      unsigned পূর্ণাঙ্ক feature_index,
			      hb_set_t *lookups) ধ্রুব
  {
    প্রত্যাগম _hb_ot_layout_feature_get_lookup_variations (
	face, table_tags[table_index], feature_index, variations_index[table_index],
	lookup_conditions[table_index].as_sorted_array (), lookups);
  }

  তার্কিক equal (ধ্রুব hb_ot_shape_plan_চাবি_t *other)
  {
    প্রত্যাগম variations_index[0] == other->variations_index[0] &&
	   variations_index[1] == other->variations_index[1] &&
	   lookup_conditions[0] == other->lookup_conditions[0] &&
	   lookup_conditions[1] == other->lookup_conditions[1];
  }
};


সংরচনা hb_shape_plan_চাবি_t;

সংরচনা hb_ot_shape_plan_t
{
  ~hb_ot_shape_plan_t () { fini (); }

  hb_segment_properties_t props;
  ধ্রুব সংরচনা hb_ot_shaper_t *shaper;
  hb_ot_map_t map;
  hb_aat_map_t aat_map;
  ধ্রুব শূন্য *data;
#ifndef HB_NO_OT_SHAPE_FRACTIONS
  hb_mask_t frac_mask, numr_mask, dnom_mask;
#অন্যথা
  স্থির constexpr hb_mask_t frac_mask = 0;
  স্থির constexpr hb_mask_t numr_mask = 0;
  স্থির constexpr hb_mask_t dnom_mask = 0;
#endif
  hb_mask_t rtlm_mask;
#ifndef HB_NO_OT_KERN
  hb_mask_t kern_mask;
#অন্যথা
  স্থির constexpr hb_mask_t kern_mask = 0;
#endif

#ifndef HB_NO_OT_KERN
  তার্কিক requested_kerning : 1;
#অন্যথা
  স্থির constexpr তার্কিক requested_kerning = অসত্য;
#endif
#ifndef HB_NO_OT_SHAPE_FRACTIONS
  তার্কিক has_frac : 1;
#অন্যথা
  স্থির constexpr তার্কিক has_frac = অসত্য;
#endif
  তার্কিক has_vert : 1;
  তার্কিক has_gpos_mark : 1;
  তার্কিক zero_marks : 1;
  তার্কিক fallback_glyph_classes : 1;
  তার্কিক fallback_mark_positioning : 1;
  তার্কিক adjust_mark_positioning_when_zeroing : 1;

  তার্কিক apply_gpos : 1;
#ifndef HB_NO_OT_KERN
  তার্কিক apply_kern : 1;
#অন্যথা
  স্থির constexpr তার্কিক apply_kern = অসত্য;
#endif
  তার্কিক apply_fallback_kern : 1;
#ifndef HB_NO_AAT_SHAPE
  তার্কিক apply_kerx : 1;
  তার্কিক apply_morx : 1;
  তার্কিক apply_trak : 1;
#অন্যথা
  স্থির constexpr তার্কিক apply_kerx = অসত্য;
  স্থির constexpr তার্কিক apply_morx = অসত্য;
  স্থির constexpr তার্কিক apply_trak = অসত্য;
#endif

  শূন্য collect_lookups (hb_tag_t table_tag, hb_set_t *lookups) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক table_index;
    বিকল্প (table_tag) {
      পক্ষ HB_OT_TAG_GSUB: table_index = 0; ভঙ্গ;
      পক্ষ HB_OT_TAG_GPOS: table_index = 1; ভঙ্গ;
      স্বভাব: প্রত্যাগম;
    }
    map.collect_lookups (table_index, lookups);
  }

  HB_INTERNAL তার্কিক init0 (hb_face_t                     *face,
			  ধ্রুব hb_shape_plan_চাবি_t     *key);
  HB_INTERNAL শূন্য fini ();

  HB_INTERNAL শূন্য substitute (hb_font_t *font, hb_buffer_t *buffer) ধ্রুব;
  HB_INTERNAL শূন্য position (hb_font_t *font, hb_buffer_t *buffer) ধ্রুব;
};

সংরচনা hb_shape_plan_t;

সংরচনা hb_ot_shape_planner_t
{
  /* In the order that they are filled in. */
  hb_face_t *face;
  hb_segment_properties_t props;
  hb_ot_map_builder_t map;
#ifndef HB_NO_AAT_SHAPE
  hb_aat_map_builder_t aat_map;
  তার্কিক apply_morx : 1;
#অন্যথা
  স্থির constexpr তার্কিক apply_morx = অসত্য;
#endif
  তার্কিক script_zero_marks : 1;
  তার্কিক script_fallback_position : 1;
  ধ্রুব সংরচনা hb_ot_shaper_t *shaper;

  HB_INTERNAL hb_ot_shape_planner_t (hb_face_t                     *face,
				     ধ্রুব hb_segment_properties_t &props);

  HB_INTERNAL শূন্য compile (hb_ot_shape_plan_t           &plan,
			    ধ্রুব hb_ot_shape_plan_চাবি_t &key);
};


#endif /* HB_OT_SHAPE_HH */

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

#ضمن "hb.hh"

#ضمن "hb-ot-map.hh"
#ضمن "hb-aat-map.hh"


HB_INTERNAL منطقي
_hb_ot_layout_table_get_feature_variations_state (hb_face_t             *face,
						   hb_tag_t               table_tag,
						   ثابت صحيح             *coords,
						   unsigned صحيح           num_coords,
						   unsigned صحيح          *variations_index,
						   hb_vector_t<unsigned> *lookup_conditions);

HB_INTERNAL منطقي
_hb_ot_layout_feature_get_lookup_variations (hb_face_t *face,
                                             hb_tag_t table_tag,
                                             unsigned feature_index,
                                             unsigned variations_index,
                                             hb_sorted_array_t<ثابت unsigned> conditions,
                                             hb_set_t *lookup_indexes);

بنية hb_ot_shape_plan_key_t
{
  unsigned صحيح variations_index[2];
  /* Sorted offsets of distinct true conditions within FeatureVariations.
   * Different outcomes may conservatively separate equivalent lookup sets. */
  hb_vector_t<unsigned> lookup_conditions[2];

  منطقي init (hb_face_t *face,
	     ثابت صحيح *coords,
	     unsigned   num_coords)
  {
    لكل (unsigned صحيح table_index = 0; table_index < 2; table_index++)
      𐡄𐡍 (unlikely (!_hb_ot_layout_table_get_feature_variations_state (
			face,
			table_tags[table_index],
			coords,
			num_coords,
			&variations_index[table_index],
			&lookup_conditions[table_index])))
      {
	لكل (auto &v : lookup_conditions)
	  v.fini ();
	أرجع خطأ;
      }
    أرجع 𐡒𐡔𐡈;
  }

  فراغ fini ()
  {
    لكل (auto &v : lookup_conditions)
      v.fini ();
  }

  منطقي get_lookup_variations (hb_face_t *face,
                              unsigned صحيح table_index,
			      unsigned صحيح feature_index,
			      hb_set_t *lookups) ثابت
  {
    أرجع _hb_ot_layout_feature_get_lookup_variations (
	face, table_tags[table_index], feature_index, variations_index[table_index],
	lookup_conditions[table_index].as_sorted_array (), lookups);
  }

  منطقي equal (ثابت hb_ot_shape_plan_key_t *other)
  {
    أرجع variations_index[0] == other->variations_index[0] &&
	   variations_index[1] == other->variations_index[1] &&
	   lookup_conditions[0] == other->lookup_conditions[0] &&
	   lookup_conditions[1] == other->lookup_conditions[1];
  }
};


بنية hb_shape_plan_key_t;

بنية hb_ot_shape_plan_t
{
  ~hb_ot_shape_plan_t () { fini (); }

  hb_segment_properties_t props;
  ثابت بنية hb_ot_shaper_t *shaper;
  hb_ot_map_t map;
  hb_aat_map_t aat_map;
  ثابت فراغ *data;
#ifndef HB_NO_OT_SHAPE_FRACTIONS
  hb_mask_t frac_mask, numr_mask, dnom_mask;
#وإلا
  ساكن constexpr hb_mask_t frac_mask = 0;
  ساكن constexpr hb_mask_t numr_mask = 0;
  ساكن constexpr hb_mask_t dnom_mask = 0;
#endif
  hb_mask_t rtlm_mask;
#ifndef HB_NO_OT_KERN
  hb_mask_t kern_mask;
#وإلا
  ساكن constexpr hb_mask_t kern_mask = 0;
#endif

#ifndef HB_NO_OT_KERN
  منطقي requested_kerning : 1;
#وإلا
  ساكن constexpr منطقي requested_kerning = خطأ;
#endif
#ifndef HB_NO_OT_SHAPE_FRACTIONS
  منطقي has_frac : 1;
#وإلا
  ساكن constexpr منطقي has_frac = خطأ;
#endif
  منطقي has_vert : 1;
  منطقي has_gpos_mark : 1;
  منطقي zero_marks : 1;
  منطقي fallback_glyph_classes : 1;
  منطقي fallback_mark_positioning : 1;
  منطقي adjust_mark_positioning_when_zeroing : 1;

  منطقي apply_gpos : 1;
#ifndef HB_NO_OT_KERN
  منطقي apply_kern : 1;
#وإلا
  ساكن constexpr منطقي apply_kern = خطأ;
#endif
  منطقي apply_fallback_kern : 1;
#ifndef HB_NO_AAT_SHAPE
  منطقي apply_kerx : 1;
  منطقي apply_morx : 1;
  منطقي apply_trak : 1;
#وإلا
  ساكن constexpr منطقي apply_kerx = خطأ;
  ساكن constexpr منطقي apply_morx = خطأ;
  ساكن constexpr منطقي apply_trak = خطأ;
#endif

  فراغ collect_lookups (hb_tag_t table_tag, hb_set_t *lookups) ثابت
  {
    unsigned صحيح table_index;
    بدل (table_tag) {
      حالة HB_OT_TAG_GSUB: table_index = 0; اكسر;
      حالة HB_OT_TAG_GPOS: table_index = 1; اكسر;
      افتراضي: أرجع;
    }
    map.collect_lookups (table_index, lookups);
  }

  HB_INTERNAL منطقي init0 (hb_face_t                     *face,
			  ثابت hb_shape_plan_key_t     *key);
  HB_INTERNAL فراغ fini ();

  HB_INTERNAL فراغ substitute (hb_font_t *font, hb_buffer_t *buffer) ثابت;
  HB_INTERNAL فراغ position (hb_font_t *font, hb_buffer_t *buffer) ثابت;
};

بنية hb_shape_plan_t;

بنية hb_ot_shape_planner_t
{
  /* In the order that they are filled in. */
  hb_face_t *face;
  hb_segment_properties_t props;
  hb_ot_map_builder_t map;
#ifndef HB_NO_AAT_SHAPE
  hb_aat_map_builder_t aat_map;
  منطقي apply_morx : 1;
#وإلا
  ساكن constexpr منطقي apply_morx = خطأ;
#endif
  منطقي script_zero_marks : 1;
  منطقي script_fallback_position : 1;
  ثابت بنية hb_ot_shaper_t *shaper;

  HB_INTERNAL hb_ot_shape_planner_t (hb_face_t                     *face,
				     ثابت hb_segment_properties_t &props);

  HB_INTERNAL فراغ compile (hb_ot_shape_plan_t           &plan,
			    ثابت hb_ot_shape_plan_key_t &key);
};


#endif /* HB_OT_SHAPE_HH */
